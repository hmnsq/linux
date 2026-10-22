// SPDX-License-Identifier: GPL-2.0-only
/*
 *  Housekeeping management. Manage the targets for routine code that can run on
 *  any CPU: unbound workqueues, timers, kthreads and any offloadable work.
 *
 * Copyright (C) 2017 Red Hat, Inc., Frederic Weisbecker
 * Copyright (C) 2017-2018 SUSE, Frederic Weisbecker
 *
 */
#অন্তর্ভাব <linux/sched/isolation.h>
#অন্তর্ভাব <linux/llist.h>
#অন্তর্ভাব <linux/pci.h>
#অন্তর্ভাব "sched.h"

গণনা hk_flags {
	HK_FLAG_DOMAIN_BOOT	= BIT(HK_TYPE_DOMAIN_BOOT),
	HK_FLAG_DOMAIN		= BIT(HK_TYPE_DOMAIN),
	HK_FLAG_MANAGED_IRQ	= BIT(HK_TYPE_MANAGED_IRQ),
	HK_FLAG_KERNEL_NOISE	= BIT(HK_TYPE_KERNEL_NOISE),
};

DEFINE_STATIC_চাবি_FALSE(housekeeping_overridden);
EXPORT_প্রতীক_GPL(housekeeping_overridden);

কাঠামো housekeeping {
	কাঠামো cpumask __rcu *cpumasks[HK_TYPE_MAX];
	unsigned long flags;
};

স্থির কাঠামো housekeeping housekeeping;
স্থির __initdata LLIST_শীর্ষ(memblock_freelist);

bool housekeeping_enabled(গণনা hk_type type)
{
	ফেরত !!(READ_ONCE(housekeeping.flags) & BIT(type));
}
EXPORT_প্রতীক_GPL(housekeeping_enabled);

স্থির bool housekeeping_dereference_check(গণনা hk_type type)
{
	যদি (IS_ENABLED(CONFIG_LOCKDEP) && type == HK_TYPE_DOMAIN) {
		/* Cpuset isn't even writable yet? */
		যদি (system_state <= SYSTEM_SCHEDULING)
			ফেরত true;

		/* CPU hotplug write locked, so cpuset partition can't be overwritten */
		যদি (IS_ENABLED(CONFIG_HOTPLUG_CPU) && lockdep_is_cpus_write_held())
			ফেরত true;

		/* Cpuset lock held, partitions not writable */
		যদি (IS_ENABLED(CONFIG_CPUSETS) && lockdep_is_cpuset_held())
			ফেরত true;

		ফেরত false;
	}

	ফেরত true;
}

স্থির অন্তঃরেখ কাঠামো cpumask *housekeeping_cpumask_dereference(গণনা hk_type type)
{
	ফেরত rcu_dereference_all_check(housekeeping.cpumasks[type],
					 housekeeping_dereference_check(type));
}

ধ্রুবক কাঠামো cpumask *housekeeping_cpumask(গণনা hk_type type)
{
	ধ্রুবক কাঠামো cpumask *mask = ফাঁকা;

	যদি (static_branch_unlikely(&housekeeping_overridden)) {
		যদি (READ_ONCE(housekeeping.flags) & BIT(type))
			mask = housekeeping_cpumask_dereference(type);
	}
	যদি (!mask)
		mask = cpu_possible_mask;
	ফেরত mask;
}
EXPORT_প্রতীক_GPL(housekeeping_cpumask);

পূর্ণসংখ্যা housekeeping_any_cpu(গণনা hk_type type)
{
	পূর্ণসংখ্যা cpu;

	যদি (static_branch_unlikely(&housekeeping_overridden)) {
		যদি (housekeeping.flags & BIT(type)) {
			cpu = sched_numa_find_closest(housekeeping_cpumask(type), smp_processor_id());
			যদি (cpu < nr_cpu_ids)
				ফেরত cpu;

			cpu = cpumask_any_and_distribute(housekeeping_cpumask(type), cpu_online_mask);
			যদি (likely(cpu < nr_cpu_ids))
				ফেরত cpu;
			/*
			 * Unless we have another problem this can only happen
			 * at boot time before start_secondary() brings the 1st
			 * housekeeping CPU up.
			 */
			WARN_ON_ONCE(system_state == SYSTEM_RUNNING ||
				     type != HK_TYPE_TIMER);
		}
	}
	ফেরত smp_processor_id();
}
EXPORT_প্রতীক_GPL(housekeeping_any_cpu);

শূন্য housekeeping_affine(কাঠামো task_struct *t, গণনা hk_type type)
{
	যদি (static_branch_unlikely(&housekeeping_overridden))
		যদি (housekeeping.flags & BIT(type))
			set_cpus_allowed_ptr(t, housekeeping_cpumask(type));
}
EXPORT_প্রতীক_GPL(housekeeping_affine);

bool housekeeping_test_cpu(পূর্ণসংখ্যা cpu, গণনা hk_type type)
{
	যদি (static_branch_unlikely(&housekeeping_overridden) &&
	    READ_ONCE(housekeeping.flags) & BIT(type))
		ফেরত cpumask_test_cpu(cpu, housekeeping_cpumask(type));
	ফেরত true;
}
EXPORT_প্রতীক_GPL(housekeeping_test_cpu);

পূর্ণসংখ্যা housekeeping_update(কাঠামো cpumask *isol_mask)
{
	কাঠামো cpumask *trial, *old = ফাঁকা;
	পূর্ণসংখ্যা err;

	trial = kmalloc(cpumask_আকার(), GFP_KERNEL);
	যদি (!trial)
		ফেরত -ENOMEM;

	cpumask_andnot(trial, housekeeping_cpumask(HK_TYPE_DOMAIN_BOOT), isol_mask);
	যদি (!cpumask_intersects(trial, cpu_online_mask)) {
		kfree(trial);
		ফেরত -EINVAL;
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

	ফেরত 0;
}

শূন্য __init housekeeping_init(শূন্য)
{
	গণনা hk_type type;

	যদি (!housekeeping.flags)
		ফেরত;

	static_branch_enable(&housekeeping_overridden);

	যদি (housekeeping.flags & HK_FLAG_KERNEL_NOISE)
		sched_tick_offload_init();
	/*
	 * Realloc with a proper allocator so that any cpumask update
	 * can indifferently free the old version with kfree().
	 */
	for_each_set_bit(type, &housekeeping.flags, HK_TYPE_MAX) {
		কাঠামো cpumask *omask, *nmask = kmalloc(cpumask_আকার(), GFP_KERNEL);

		যদি (WARN_ON_ONCE(!nmask))
			ফেরত;

		omask = rcu_dereference(housekeeping.cpumasks[type]);

		/* We need at least one CPU to handle housekeeping work */
		WARN_ON_ONCE(cpumask_empty(omask));
		cpumask_copy(nmask, omask);
		RCU_INIT_POINTER(housekeeping.cpumasks[type], nmask);
		__llist_add((কাঠামো llist_গ্রন্থি *)omask, &memblock_freelist);
	}
}

স্থির পূর্ণসংখ্যা __init housekeeping_late_init(শূন্য)
{
	কাঠামো llist_গ্রন্থি *llnode, *pos, *t;

	/* Free allocated memblock memory, if any */
	llnode = __llist_del_all(&memblock_freelist);
	llist_for_each_safe(pos, t, llnode)
		memblock_free(pos, cpumask_আকার());
	ফেরত 0;
}
pure_initcall(housekeeping_late_init);

স্থির শূন্য __init housekeeping_setup_type(গণনা hk_type type,
					   cpumask_var_t housekeeping_staging)
{
	কাঠামো cpumask *mask = memblock_alloc_or_panic(cpumask_আকার(), SMP_CACHE_BYTES);

	cpumask_copy(mask, housekeeping_staging);
	RCU_INIT_POINTER(housekeeping.cpumasks[type], mask);
}

স্থির পূর্ণসংখ্যা __init housekeeping_setup(অক্ষর *str, unsigned long flags)
{
	cpumask_var_t non_housekeeping_mask, housekeeping_staging;
	unsigned পূর্ণসংখ্যা first_cpu;
	পূর্ণসংখ্যা err = 0;

	যদি ((flags & HK_FLAG_KERNEL_NOISE) && !(housekeeping.flags & HK_FLAG_KERNEL_NOISE)) {
		যদি (!IS_ENABLED(CONFIG_NO_HZ_FULL)) {
			pr_warn("Housekeeping: nohz unsupported."
				" Build with CONFIG_NO_HZ_FULL\n");
			ফেরত 0;
		}
	}

	alloc_bootmem_cpumask_var(&non_housekeeping_mask);
	যদি (cpulist_parse(str, non_housekeeping_mask) < 0) {
		pr_warn("Housekeeping: nohz_full= or isolcpus= incorrect CPU range\n");
		যাও free_non_housekeeping_mask;
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
		যাও free_housekeeping_staging;

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
				যাও free_housekeeping_staging;
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
			যাও free_housekeeping_staging;
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

	ফেরত err;
}

স্থির পূর্ণসংখ্যা __init housekeeping_nohz_full_setup(অক্ষর *str)
{
	unsigned long flags;

	flags = HK_FLAG_KERNEL_NOISE;

	ফেরত housekeeping_setup(str, flags);
}
__setup("nohz_full=", housekeeping_nohz_full_setup);

স্থির পূর্ণসংখ্যা __init housekeeping_isolcpus_setup(অক্ষর *str)
{
	unsigned long flags = 0;
	bool illegal = false;
	অক্ষর *par;
	পূর্ণসংখ্যা len;

	যখন (isalpha(*str)) {
		/*
		 * isolcpus=nohz is equivalent to nohz_full.
		 */
		যদি (!strncmp(str, "nohz,", 5)) {
			str += 5;
			flags |= HK_FLAG_KERNEL_NOISE;
			চালু;
		}

		যদি (!strncmp(str, "domain,", 7)) {
			str += 7;
			flags |= HK_FLAG_DOMAIN | HK_FLAG_DOMAIN_BOOT;
			চালু;
		}

		যদি (!strncmp(str, "managed_irq,", 12)) {
			str += 12;
			flags |= HK_FLAG_MANAGED_IRQ;
			চালু;
		}

		/*
		 * Skip unknown sub-parameter and validate that it is not
		 * containing an invalid character.
		 */
		জন্য (par = str, len = 0; *str && *str != ','; str++, len++) {
			যদি (!isalpha(*str) && *str != '_')
				illegal = true;
		}

		যদি (illegal) {
			pr_warn("isolcpus: Invalid flag %.*s\n", len, par);
			ফেরত 0;
		}

		pr_info("isolcpus: Skipped unknown flag %.*s\n", len, par);
		str++;
	}

	/* Default behaviour for isolcpus without flags */
	যদি (!flags)
		flags |= HK_FLAG_DOMAIN | HK_FLAG_DOMAIN_BOOT;

	ফেরত housekeeping_setup(str, flags);
}
__setup("isolcpus=", housekeeping_isolcpus_setup);
