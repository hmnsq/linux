// SPDX-License-Identifier: GPL-2.0-only
/*
 *  Housekeeping management. Manage the targets for routine code that can run on
 *  any CPU: unbound workqueues, timers, kthreads and any offloadable work.
 *
 * Copyright (C) 2017 Red Hat, Inc., Frederic Weisbecker
 * Copyright (C) 2017-2018 SUSE, Frederic Weisbecker
 *
 */
#ಅನ್ತರ್ಭಾವ <linux/sched/isolation.h>
#ಅನ್ತರ್ಭಾವ <linux/llist.h>
#ಅನ್ತರ್ಭಾವ <linux/pci.h>
#ಅನ್ತರ್ಭಾವ "sched.h"

ಗಣನಾ hk_flags {
	HK_FLAG_DOMAIN_BOOT	= BIT(HK_TYPE_DOMAIN_BOOT),
	HK_FLAG_DOMAIN		= BIT(HK_TYPE_DOMAIN),
	HK_FLAG_MANAGED_IRQ	= BIT(HK_TYPE_MANAGED_IRQ),
	HK_FLAG_KERNEL_NOISE	= BIT(HK_TYPE_KERNEL_NOISE),
};

DEFINE_STATIC_ಕೀಲಿ_FALSE(housekeeping_overridden);
EXPORT_ಚಿಹ್ನೆ_GPL(housekeeping_overridden);

ಸಂರಚನಾ housekeeping {
	ಸಂರಚನಾ cpumask __rcu *cpumasks[HK_TYPE_MAX];
	unsigned long flags;
};

ಸ್ಥಿರ ಸಂರಚನಾ housekeeping housekeeping;
ಸ್ಥಿರ __initdata LLIST_HEAD(memblock_freelist);

bool housekeeping_enabled(ಗಣನಾ hk_type type)
{
	ಪ್ರತ್ಯಾಗಮ !!(READ_ONCE(housekeeping.flags) & BIT(type));
}
EXPORT_ಚಿಹ್ನೆ_GPL(housekeeping_enabled);

ಸ್ಥಿರ bool housekeeping_dereference_check(ಗಣನಾ hk_type type)
{
	ಯದಿ (IS_ENABLED(CONFIG_LOCKDEP) && type == HK_TYPE_DOMAIN) {
		/* Cpuset isn't even writable yet? */
		ಯದಿ (system_state <= SYSTEM_SCHEDULING)
			ಪ್ರತ್ಯಾಗಮ true;

		/* CPU hotplug write locked, so cpuset partition can't be overwritten */
		ಯದಿ (IS_ENABLED(CONFIG_HOTPLUG_CPU) && lockdep_is_cpus_write_held())
			ಪ್ರತ್ಯಾಗಮ true;

		/* Cpuset lock held, partitions not writable */
		ಯದಿ (IS_ENABLED(CONFIG_CPUSETS) && lockdep_is_cpuset_held())
			ಪ್ರತ್ಯಾಗಮ true;

		ಪ್ರತ್ಯಾಗಮ false;
	}

	ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ cpumask *housekeeping_cpumask_dereference(ಗಣನಾ hk_type type)
{
	ಪ್ರತ್ಯಾಗಮ rcu_dereference_all_check(housekeeping.cpumasks[type],
					 housekeeping_dereference_check(type));
}

ಧ್ರುವ ಸಂರಚನಾ cpumask *housekeeping_cpumask(ಗಣನಾ hk_type type)
{
	ಧ್ರುವ ಸಂರಚನಾ cpumask *mask = ರಿಕ್ತ;

	ಯದಿ (static_branch_unlikely(&housekeeping_overridden)) {
		ಯದಿ (READ_ONCE(housekeeping.flags) & BIT(type))
			mask = housekeeping_cpumask_dereference(type);
	}
	ಯದಿ (!mask)
		mask = cpu_possible_mask;
	ಪ್ರತ್ಯಾಗಮ mask;
}
EXPORT_ಚಿಹ್ನೆ_GPL(housekeeping_cpumask);

ಪೂರ್ಣಾಙ್ಕ housekeeping_any_cpu(ಗಣನಾ hk_type type)
{
	ಪೂರ್ಣಾಙ್ಕ cpu;

	ಯದಿ (static_branch_unlikely(&housekeeping_overridden)) {
		ಯದಿ (housekeeping.flags & BIT(type)) {
			cpu = sched_numa_find_closest(housekeeping_cpumask(type), smp_processor_id());
			ಯದಿ (cpu < nr_cpu_ids)
				ಪ್ರತ್ಯಾಗಮ cpu;

			cpu = cpumask_any_and_distribute(housekeeping_cpumask(type), cpu_online_mask);
			ಯದಿ (likely(cpu < nr_cpu_ids))
				ಪ್ರತ್ಯಾಗಮ cpu;
			/*
			 * Unless we have another problem this can only happen
			 * at boot time before start_secondary() brings the 1st
			 * housekeeping CPU up.
			 */
			WARN_ON_ONCE(system_state == SYSTEM_RUNNING ||
				     type != HK_TYPE_TIMER);
		}
	}
	ಪ್ರತ್ಯಾಗಮ smp_processor_id();
}
EXPORT_ಚಿಹ್ನೆ_GPL(housekeeping_any_cpu);

ಶೂನ್ಯ housekeeping_affine(ಸಂರಚನಾ task_struct *t, ಗಣನಾ hk_type type)
{
	ಯದಿ (static_branch_unlikely(&housekeeping_overridden))
		ಯದಿ (housekeeping.flags & BIT(type))
			set_cpus_allowed_ptr(t, housekeeping_cpumask(type));
}
EXPORT_ಚಿಹ್ನೆ_GPL(housekeeping_affine);

bool housekeeping_test_cpu(ಪೂರ್ಣಾಙ್ಕ cpu, ಗಣನಾ hk_type type)
{
	ಯದಿ (static_branch_unlikely(&housekeeping_overridden) &&
	    READ_ONCE(housekeeping.flags) & BIT(type))
		ಪ್ರತ್ಯಾಗಮ cpumask_test_cpu(cpu, housekeeping_cpumask(type));
	ಪ್ರತ್ಯಾಗಮ true;
}
EXPORT_ಚಿಹ್ನೆ_GPL(housekeeping_test_cpu);

ಪೂರ್ಣಾಙ್ಕ housekeeping_update(ಸಂರಚನಾ cpumask *isol_mask)
{
	ಸಂರಚನಾ cpumask *trial, *old = ರಿಕ್ತ;
	ಪೂರ್ಣಾಙ್ಕ err;

	trial = kmalloc(cpumask_size(), GFP_KERNEL);
	ಯದಿ (!trial)
		ಪ್ರತ್ಯಾಗಮ -ENOMEM;

	cpumask_andnot(trial, housekeeping_cpumask(HK_TYPE_DOMAIN_BOOT), isol_mask);
	ಯದಿ (!cpumask_intersects(trial, cpu_online_mask)) {
		kfree(trial);
		ಪ್ರತ್ಯಾಗಮ -EINVAL;
	}

	ಯದಿ (!housekeeping.flags)
		static_branch_enable(&housekeeping_overridden);

	ಯದಿ (housekeeping.flags & HK_FLAG_DOMAIN)
		old = housekeeping_cpumask_dereference(HK_TYPE_DOMAIN);
	ಅನ್ಯಥಾ
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

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಶೂನ್ಯ __init housekeeping_init(ಶೂನ್ಯ)
{
	ಗಣನಾ hk_type type;

	ಯದಿ (!housekeeping.flags)
		ಪ್ರತ್ಯಾಗಮ;

	static_branch_enable(&housekeeping_overridden);

	ಯದಿ (housekeeping.flags & HK_FLAG_KERNEL_NOISE)
		sched_tick_offload_init();
	/*
	 * Realloc with a proper allocator so that any cpumask update
	 * can indifferently free the old version with kfree().
	 */
	for_each_set_bit(type, &housekeeping.flags, HK_TYPE_MAX) {
		ಸಂರಚನಾ cpumask *omask, *nmask = kmalloc(cpumask_size(), GFP_KERNEL);

		ಯದಿ (WARN_ON_ONCE(!nmask))
			ಪ್ರತ್ಯಾಗಮ;

		omask = rcu_dereference(housekeeping.cpumasks[type]);

		/* We need at least one CPU to handle housekeeping work */
		WARN_ON_ONCE(cpumask_empty(omask));
		cpumask_copy(nmask, omask);
		RCU_INIT_POINTER(housekeeping.cpumasks[type], nmask);
		__llist_add((ಸಂರಚನಾ llist_ಗಂಟು *)omask, &memblock_freelist);
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ __init housekeeping_late_init(ಶೂನ್ಯ)
{
	ಸಂರಚನಾ llist_ಗಂಟು *llnode, *pos, *t;

	/* Free allocated memblock memory, if any */
	llnode = __llist_del_all(&memblock_freelist);
	llist_for_each_safe(pos, t, llnode)
		memblock_free(pos, cpumask_size());
	ಪ್ರತ್ಯಾಗಮ 0;
}
pure_initcall(housekeeping_late_init);

ಸ್ಥಿರ ಶೂನ್ಯ __init housekeeping_setup_type(ಗಣನಾ hk_type type,
					   cpumask_var_t housekeeping_staging)
{
	ಸಂರಚನಾ cpumask *mask = memblock_alloc_or_panic(cpumask_size(), SMP_CACHE_BYTES);

	cpumask_copy(mask, housekeeping_staging);
	RCU_INIT_POINTER(housekeeping.cpumasks[type], mask);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ __init housekeeping_setup(ವರ್ಣ *str, unsigned long flags)
{
	cpumask_var_t non_housekeeping_mask, housekeeping_staging;
	unsigned ಪೂರ್ಣಾಙ್ಕ first_cpu;
	ಪೂರ್ಣಾಙ್ಕ err = 0;

	ಯದಿ ((flags & HK_FLAG_KERNEL_NOISE) && !(housekeeping.flags & HK_FLAG_KERNEL_NOISE)) {
		ಯದಿ (!IS_ENABLED(CONFIG_NO_HZ_FULL)) {
			pr_warn("Housekeeping: nohz unsupported."
				" Build with CONFIG_NO_HZ_FULL\n");
			ಪ್ರತ್ಯಾಗಮ 0;
		}
	}

	alloc_bootmem_cpumask_var(&non_housekeeping_mask);
	ಯದಿ (cpulist_parse(str, non_housekeeping_mask) < 0) {
		pr_warn("Housekeeping: nohz_full= or isolcpus= incorrect CPU range\n");
		ಗಚ್ಛ free_non_housekeeping_mask;
	}

	alloc_bootmem_cpumask_var(&housekeeping_staging);
	cpumask_andnot(housekeeping_staging,
		       cpu_possible_mask, non_housekeeping_mask);

	first_cpu = cpumask_first_and(cpu_present_mask, housekeeping_staging);
	ಯದಿ (first_cpu >= nr_cpu_ids || first_cpu >= setup_max_cpus) {
		__cpumask_set_cpu(smp_processor_id(), housekeeping_staging);
		__cpumask_clear_cpu(smp_processor_id(), non_housekeeping_mask);
		ಯದಿ (!housekeeping.flags) {
			pr_warn("Housekeeping: must include one present CPU, "
				"using boot CPU:%d\n", smp_processor_id());
		}
	}

	ಯದಿ (cpumask_empty(non_housekeeping_mask))
		ಗಚ್ಛ free_housekeeping_staging;

	ಯದಿ (!housekeeping.flags) {
		/* First setup call ("nohz_full=" or "isolcpus=") */
		ಗಣನಾ hk_type type;

		for_each_set_bit(type, &flags, HK_TYPE_MAX)
			housekeeping_setup_type(type, housekeeping_staging);
	} ಅನ್ಯಥಾ {
		/* Second setup call ("nohz_full=" after "isolcpus=" or the reverse) */
		ಗಣನಾ hk_type type;
		unsigned long iter_flags = flags & housekeeping.flags;

		for_each_set_bit(type, &iter_flags, HK_TYPE_MAX) {
			ಯದಿ (!cpumask_equal(housekeeping_staging,
					   housekeeping_cpumask(type))) {
				pr_warn("Housekeeping: nohz_full= must match isolcpus=\n");
				ಗಚ್ಛ free_housekeeping_staging;
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
		ಯದಿ (first_cpu >= min(nr_cpu_ids, setup_max_cpus)) {
			pr_warn("Housekeeping: must include one present CPU "
				"neither in nohz_full= nor in isolcpus=domain, "
				"ignoring setting %s\n", str);
			ಗಚ್ಛ free_housekeeping_staging;
		}

		iter_flags = flags & ~housekeeping.flags;

		for_each_set_bit(type, &iter_flags, HK_TYPE_MAX)
			housekeeping_setup_type(type, housekeeping_staging);
	}

	ಯದಿ ((flags & HK_FLAG_KERNEL_NOISE) && !(housekeeping.flags & HK_FLAG_KERNEL_NOISE))
		tick_nohz_full_setup(non_housekeeping_mask);

	housekeeping.flags |= flags;
	err = 1;

free_housekeeping_staging:
	free_bootmem_cpumask_var(housekeeping_staging);
free_non_housekeeping_mask:
	free_bootmem_cpumask_var(non_housekeeping_mask);

	ಪ್ರತ್ಯಾಗಮ err;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ __init housekeeping_nohz_full_setup(ವರ್ಣ *str)
{
	unsigned long flags;

	flags = HK_FLAG_KERNEL_NOISE;

	ಪ್ರತ್ಯಾಗಮ housekeeping_setup(str, flags);
}
__setup("nohz_full=", housekeeping_nohz_full_setup);

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ __init housekeeping_isolcpus_setup(ವರ್ಣ *str)
{
	unsigned long flags = 0;
	bool illegal = false;
	ವರ್ಣ *par;
	ಪೂರ್ಣಾಙ್ಕ len;

	ಯಾವತ್ (isalpha(*str)) {
		/*
		 * isolcpus=nohz is equivalent to nohz_full.
		 */
		ಯದಿ (!strncmp(str, "nohz,", 5)) {
			str += 5;
			flags |= HK_FLAG_KERNEL_NOISE;
			ಅನುವರ್ತ;
		}

		ಯದಿ (!strncmp(str, "domain,", 7)) {
			str += 7;
			flags |= HK_FLAG_DOMAIN | HK_FLAG_DOMAIN_BOOT;
			ಅನುವರ್ತ;
		}

		ಯದಿ (!strncmp(str, "managed_irq,", 12)) {
			str += 12;
			flags |= HK_FLAG_MANAGED_IRQ;
			ಅನುವರ್ತ;
		}

		/*
		 * Skip unknown sub-parameter and validate that it is not
		 * containing an invalid character.
		 */
		ಪ್ರತಿ (par = str, len = 0; *str && *str != ','; str++, len++) {
			ಯದಿ (!isalpha(*str) && *str != '_')
				illegal = true;
		}

		ಯದಿ (illegal) {
			pr_warn("isolcpus: Invalid flag %.*s\n", len, par);
			ಪ್ರತ್ಯಾಗಮ 0;
		}

		pr_info("isolcpus: Skipped unknown flag %.*s\n", len, par);
		str++;
	}

	/* Default behaviour for isolcpus without flags */
	ಯದಿ (!flags)
		flags |= HK_FLAG_DOMAIN | HK_FLAG_DOMAIN_BOOT;

	ಪ್ರತ್ಯಾಗಮ housekeeping_setup(str, flags);
}
__setup("isolcpus=", housekeeping_isolcpus_setup);
