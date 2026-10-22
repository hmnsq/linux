// SPDX-License-Identifier: GPL-2.0-only
/*
 *  Housekeeping management. Manage the targets for routine code that can run on
 *  any CPU: unbound workqueues, timers, kthreads and any offloadable work.
 *
 * Copyright (C) 2017 Red Hat, Inc., Frederic Weisbecker
 * Copyright (C) 2017-2018 SUSE, Frederic Weisbecker
 *
 */
#অন্তর্ভাৱ <linux/sched/isolation.h>
#অন্তর্ভাৱ <linux/llist.h>
#অন্তর্ভাৱ <linux/pci.h>
#অন্তর্ভাৱ "sched.h"

গণনা hk_flags {
	HK_FLAG_DOMAIN_BOOT	= BIT(HK_TYPE_DOMAIN_BOOT),
	HK_FLAG_DOMAIN		= BIT(HK_TYPE_DOMAIN),
	HK_FLAG_MANAGED_IRQ	= BIT(HK_TYPE_MANAGED_IRQ),
	HK_FLAG_KERNEL_NOISE	= BIT(HK_TYPE_KERNEL_NOISE),
};

DEFINE_STATIC_চাবি_FALSE(housekeeping_overridden);
EXPORT_প্ৰতীক_GPL(housekeeping_overridden);

সংরচনা housekeeping {
	সংরচনা cpumask __rcu *cpumasks[HK_TYPE_MAX];
	unsigned long flags;
};

স্থির সংরচনা housekeeping housekeeping;
স্থির __initdata LLIST_HEAD(memblock_freelist);

bool housekeeping_enabled(গণনা hk_type type)
{
	ঘূৰাই !!(READ_ONCE(housekeeping.flags) & BIT(type));
}
EXPORT_প্ৰতীক_GPL(housekeeping_enabled);

স্থির bool housekeeping_dereference_check(গণনা hk_type type)
{
	যদি (IS_ENABLED(CONFIG_LOCKDEP) && type == HK_TYPE_DOMAIN) {
		/* Cpuset isn't even writable yet? */
		যদি (system_state <= SYSTEM_SCHEDULING)
			ঘূৰাই true;

		/* CPU hotplug write locked, so cpuset partition can't be overwritten */
		যদি (IS_ENABLED(CONFIG_HOTPLUG_CPU) && lockdep_is_cpus_write_held())
			ঘূৰাই true;

		/* Cpuset lock held, partitions not writable */
		যদি (IS_ENABLED(CONFIG_CPUSETS) && lockdep_is_cpuset_held())
			ঘূৰাই true;

		ঘূৰাই false;
	}

	ঘূৰাই true;
}

স্থির অন্তঃস্থ সংরচনা cpumask *housekeeping_cpumask_dereference(গণনা hk_type type)
{
	ঘূৰাই rcu_dereference_all_check(housekeeping.cpumasks[type],
					 housekeeping_dereference_check(type));
}

ধ্রুৱ সংরচনা cpumask *housekeeping_cpumask(গণনা hk_type type)
{
	ধ্রুৱ সংরচনা cpumask *mask = রিক্ত;

	যদি (static_branch_unlikely(&housekeeping_overridden)) {
		যদি (READ_ONCE(housekeeping.flags) & BIT(type))
			mask = housekeeping_cpumask_dereference(type);
	}
	যদি (!mask)
		mask = cpu_possible_mask;
	ঘূৰাই mask;
}
EXPORT_প্ৰতীক_GPL(housekeeping_cpumask);

পূর্ণাঙ্ক housekeeping_any_cpu(গণনা hk_type type)
{
	পূর্ণাঙ্ক cpu;

	যদি (static_branch_unlikely(&housekeeping_overridden)) {
		যদি (housekeeping.flags & BIT(type)) {
			cpu = sched_numa_find_closest(housekeeping_cpumask(type), smp_processor_id());
			যদি (cpu < nr_cpu_ids)
				ঘূৰাই cpu;

			cpu = cpumask_any_and_distribute(housekeeping_cpumask(type), cpu_online_mask);
			যদি (likely(cpu < nr_cpu_ids))
				ঘূৰাই cpu;
			/*
			 * Unless we have another problem this can only happen
			 * at boot time before start_secondary() brings the 1st
			 * housekeeping CPU up.
			 */
			WARN_ON_ONCE(system_state == SYSTEM_RUNNING ||
				     type != HK_TYPE_TIMER);
		}
	}
	ঘূৰাই smp_processor_id();
}
EXPORT_প্ৰতীক_GPL(housekeeping_any_cpu);

শূন্য housekeeping_affine(সংরচনা task_struct *t, গণনা hk_type type)
{
	যদি (static_branch_unlikely(&housekeeping_overridden))
		যদি (housekeeping.flags & BIT(type))
			set_cpus_allowed_ptr(t, housekeeping_cpumask(type));
}
EXPORT_প্ৰতীক_GPL(housekeeping_affine);

bool housekeeping_test_cpu(পূর্ণাঙ্ক cpu, গণনা hk_type type)
{
	যদি (static_branch_unlikely(&housekeeping_overridden) &&
	    READ_ONCE(housekeeping.flags) & BIT(type))
		ঘূৰাই cpumask_test_cpu(cpu, housekeeping_cpumask(type));
	ঘূৰাই true;
}
EXPORT_প্ৰতীক_GPL(housekeeping_test_cpu);

পূর্ণাঙ্ক housekeeping_update(সংরচনা cpumask *isol_mask)
{
	সংরচনা cpumask *trial, *old = রিক্ত;
	পূর্ণাঙ্ক err;

	trial = kmalloc(cpumask_size(), GFP_KERNEL);
	যদি (!trial)
		ঘূৰাই -ENOMEM;

	cpumask_andnot(trial, housekeeping_cpumask(HK_TYPE_DOMAIN_BOOT), isol_mask);
	যদি (!cpumask_intersects(trial, cpu_online_mask)) {
		kfree(trial);
		ঘূৰাই -EINVAL;
	}

	যদি (!housekeeping.flags)
		static_branch_enable(&housekeeping_overridden);

	যদি (housekeeping.flags & HK_FLAG_DOMAIN)
		old = housekeeping_cpumask_dereference(HK_TYPE_DOMAIN);
	অন্যথা
		WRITE_ONCE(housekeeping.flags, housekeeping.flags | HK_FLAG_DOMAIN);
	rcu_assign_pointer(housekeeping.cpumasks[HK_TYPE_DOMAIN], trial);

	synchronize_rcu();

	pci_probe_flush_workqueue();
	mem_cgroup_flush_workqueue();
	vmstat_flush_workqueue();

	err = workqueue_unbound_housekeeping_update(housekeeping_cpumask(HK_TYPE_DOMAIN));
	WARN_ON_ONCE(err < 0);

	err = tmigr_isolated_exclude_cpumask(isol_mask);
	WARN_ON_ONCE(err < 0);

	err = kthreads_update_housekeeping();
	WARN_ON_ONCE(err < 0);

	kfree(old);

	ঘূৰাই 0;
}

শূন্য __init housekeeping_init(শূন্য)
{
	গণনা hk_type type;

	যদি (!housekeeping.flags)
		ঘূৰাই;

	static_branch_enable(&housekeeping_overridden);

	যদি (housekeeping.flags & HK_FLAG_KERNEL_NOISE)
		sched_tick_offload_init();
	/*
	 * Realloc with a proper allocator so that any cpumask update
	 * can indifferently free the old version with kfree().
	 */
	for_each_set_bit(type, &housekeeping.flags, HK_TYPE_MAX) {
		সংরচনা cpumask *omask, *nmask = kmalloc(cpumask_size(), GFP_KERNEL);

		যদি (WARN_ON_ONCE(!nmask))
			ঘূৰাই;

		omask = rcu_dereference(housekeeping.cpumasks[type]);

		/* We need at least one CPU to handle housekeeping work */
		WARN_ON_ONCE(cpumask_empty(omask));
		cpumask_copy(nmask, omask);
		RCU_INIT_POINTER(housekeeping.cpumasks[type], nmask);
		__llist_add((সংরচনা llist_গ্ৰন্থি *)omask, &memblock_freelist);
	}
}

স্থির পূর্ণাঙ্ক __init housekeeping_late_init(শূন্য)
{
	সংরচনা llist_গ্ৰন্থি *llnode, *pos, *t;

	/* Free allocated memblock memory, if any */
	llnode = __llist_del_all(&memblock_freelist);
	llist_for_each_safe(pos, t, llnode)
		memblock_free(pos, cpumask_size());
	ঘূৰাই 0;
}
pure_initcall(housekeeping_late_init);

স্থির শূন্য __init housekeeping_setup_type(গণনা hk_type type,
					   cpumask_var_t housekeeping_staging)
{
	সংরচনা cpumask *mask = memblock_alloc_or_panic(cpumask_size(), SMP_CACHE_BYTES);

	cpumask_copy(mask, housekeeping_staging);
	RCU_INIT_POINTER(housekeeping.cpumasks[type], mask);
}

স্থির পূর্ণাঙ্ক __init housekeeping_setup(ৱর্ণ *str, unsigned long flags)
{
	cpumask_var_t non_housekeeping_mask, housekeeping_staging;
	unsigned পূর্ণাঙ্ক first_cpu;
	পূর্ণাঙ্ক err = 0;

	যদি ((flags & HK_FLAG_KERNEL_NOISE) && !(housekeeping.flags & HK_FLAG_KERNEL_NOISE)) {
		যদি (!IS_ENABLED(CONFIG_NO_HZ_FULL)) {
			pr_warn("Housekeeping: nohz unsupported."
				" Build with CONFIG_NO_HZ_FULL\n");
			ঘূৰাই 0;
		}
	}

	alloc_bootmem_cpumask_var(&non_housekeeping_mask);
	যদি (cpulist_parse(str, non_housekeeping_mask) < 0) {
		pr_warn("Housekeeping: nohz_full= or isolcpus= incorrect CPU range\n");
		গচ্ছ free_non_housekeeping_mask;
	}

	alloc_bootmem_cpumask_var(&housekeeping_staging);
	cpumask_andnot(housekeeping_staging,
		       cpu_possible_mask, non_housekeeping_mask);

	first_cpu = cpumask_first_and(cpu_present_mask, housekeeping_staging);
	যদি (first_cpu >= nr_cpu_ids || first_cpu >= setup_max_cpus) {
		__cpumask_set_cpu(smp_processor_id(), housekeeping_staging);
		__cpumask_clear_cpu(smp_processor_id(), non_housekeeping_mask);
		যদি (!housekeeping.flags) {
			pr_warn("Housekeeping: must include one present CPU, "
				"using boot CPU:%d\n", smp_processor_id());
		}
	}

	যদি (cpumask_empty(non_housekeeping_mask))
		গচ্ছ free_housekeeping_staging;

	যদি (!housekeeping.flags) {
		/* First setup call ("nohz_full=" or "isolcpus=") */
		গণনা hk_type type;

		for_each_set_bit(type, &flags, HK_TYPE_MAX)
			housekeeping_setup_type(type, housekeeping_staging);
	} অন্যথা {
		/* Second setup call ("nohz_full=" after "isolcpus=" or the reverse) */
		গণনা hk_type type;
		unsigned long iter_flags = flags & housekeeping.flags;

		for_each_set_bit(type, &iter_flags, HK_TYPE_MAX) {
			যদি (!cpumask_equal(housekeeping_staging,
					   housekeeping_cpumask(type))) {
				pr_warn("Housekeeping: nohz_full= must match isolcpus=\n");
				গচ্ছ free_housekeeping_staging;
			}
		}

		/*
		 * Check the combination of nohz_full and isolcpus=domain,
		 * necessary to avoid problems with the timer migration
		 * hierarchy. managed_irq is ignored by this check since it
		 * isn't considered in the timer migration logic.
		 */
		iter_flags = housekeeping.flags & (HK_FLAG_KERNEL_NOISE | HK_FLAG_DOMAIN);
		type = find_first_bit(&iter_flags, HK_TYPE_MAX);
		/*
		 * Pass the check if none of these flags were previously set or
		 * are not in the current selection.
		 */
		iter_flags = flags & (HK_FLAG_KERNEL_NOISE | HK_FLAG_DOMAIN);
		first_cpu = (type == HK_TYPE_MAX || !iter_flags) ? 0 :
			    cpumask_first_and_and(cpu_present_mask,
						  housekeeping_staging, housekeeping_cpumask(type));
		যদি (first_cpu >= min(nr_cpu_ids, setup_max_cpus)) {
			pr_warn("Housekeeping: must include one present CPU "
				"neither in nohz_full= nor in isolcpus=domain, "
				"ignoring setting %s\n", str);
			গচ্ছ free_housekeeping_staging;
		}

		iter_flags = flags & ~housekeeping.flags;

		for_each_set_bit(type, &iter_flags, HK_TYPE_MAX)
			housekeeping_setup_type(type, housekeeping_staging);
	}

	যদি ((flags & HK_FLAG_KERNEL_NOISE) && !(housekeeping.flags & HK_FLAG_KERNEL_NOISE))
		tick_nohz_full_setup(non_housekeeping_mask);

	housekeeping.flags |= flags;
	err = 1;

free_housekeeping_staging:
	free_bootmem_cpumask_var(housekeeping_staging);
free_non_housekeeping_mask:
	free_bootmem_cpumask_var(non_housekeeping_mask);

	ঘূৰাই err;
}

স্থির পূর্ণাঙ্ক __init housekeeping_nohz_full_setup(ৱর্ণ *str)
{
	unsigned long flags;

	flags = HK_FLAG_KERNEL_NOISE;

	ঘূৰাই housekeeping_setup(str, flags);
}
__setup("nohz_full=", housekeeping_nohz_full_setup);

স্থির পূর্ণাঙ্ক __init housekeeping_isolcpus_setup(ৱর্ণ *str)
{
	unsigned long flags = 0;
	bool illegal = false;
	ৱর্ণ *par;
	পূর্ণাঙ্ক len;

	যেতিয়ালৈ (isalpha(*str)) {
		/*
		 * isolcpus=nohz is equivalent to nohz_full.
		 */
		যদি (!strncmp(str, "nohz,", 5)) {
			str += 5;
			flags |= HK_FLAG_KERNEL_NOISE;
			অনুৱর্ত;
		}

		যদি (!strncmp(str, "domain,", 7)) {
			str += 7;
			flags |= HK_FLAG_DOMAIN | HK_FLAG_DOMAIN_BOOT;
			অনুৱর্ত;
		}

		যদি (!strncmp(str, "managed_irq,", 12)) {
			str += 12;
			flags |= HK_FLAG_MANAGED_IRQ;
			অনুৱর্ত;
		}

		/*
		 * Skip unknown sub-parameter and validate that it is not
		 * containing an invalid character.
		 */
		বাবে (par = str, len = 0; *str && *str != ','; str++, len++) {
			যদি (!isalpha(*str) && *str != '_')
				illegal = true;
		}

		যদি (illegal) {
			pr_warn("isolcpus: Invalid flag %.*s\n", len, par);
			ঘূৰাই 0;
		}

		pr_info("isolcpus: Skipped unknown flag %.*s\n", len, par);
		str++;
	}

	/* Default behaviour for isolcpus without flags */
	যদি (!flags)
		flags |= HK_FLAG_DOMAIN | HK_FLAG_DOMAIN_BOOT;

	ঘূৰাই housekeeping_setup(str, flags);
}
__setup("isolcpus=", housekeeping_isolcpus_setup);
