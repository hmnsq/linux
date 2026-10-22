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

ಎಣಿಕೆ hk_flags {
	HK_FLAG_DOMAIN_BOOT	= ಬಿಟ್(HK_TYPE_DOMAIN_BOOT),
	HK_FLAG_DOMAIN		= ಬಿಟ್(HK_TYPE_DOMAIN),
	HK_FLAG_MANAGED_IRQ	= ಬಿಟ್(HK_TYPE_MANAGED_IRQ),
	HK_FLAG_KERNEL_NOISE	= ಬಿಟ್(HK_TYPE_KERNEL_NOISE),
};

DEFINE_STATIC_ಕೀಲಿ_FALSE(housekeeping_overridden);
EXPORT_ಚಿಹ್ನೆ_GPL(housekeeping_overridden);

ರಚನೆ housekeeping {
	ರಚನೆ cpumask __rcu *cpumasks[HK_TYPE_MAX];
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags;
};

ಸ್ಥಿರ ರಚನೆ housekeeping housekeeping;
ಸ್ಥಿರ __initdata LLIST_ತಲೆ(memblock_freelist);

bool housekeeping_enabled(ಎಣಿಕೆ hk_type type)
{
	ಹಿಂತಿರುಗಿ !!(READ_ONCE(housekeeping.flags) & ಬಿಟ್(type));
}
EXPORT_ಚಿಹ್ನೆ_GPL(housekeeping_enabled);

ಸ್ಥಿರ bool housekeeping_dereference_check(ಎಣಿಕೆ hk_type type)
{
	ಆದರೆ (IS_ENABLED(CONFIG_LOCKDEP) && type == HK_TYPE_DOMAIN) {
		/* Cpuset isn't even writable yet? */
		ಆದರೆ (system_state <= SYSTEM_SCHEDULING)
			ಹಿಂತಿರುಗಿ true;

		/* CPU hotplug write locked, so cpuset partition can't be overwritten */
		ಆದರೆ (IS_ENABLED(CONFIG_HOTPLUG_CPU) && lockdep_is_cpus_write_held())
			ಹಿಂತಿರುಗಿ true;

		/* Cpuset lock held, partitions not writable */
		ಆದರೆ (IS_ENABLED(CONFIG_CPUSETS) && lockdep_is_cpuset_held())
			ಹಿಂತಿರುಗಿ true;

		ಹಿಂತಿರುಗಿ false;
	}

	ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ cpumask *housekeeping_cpumask_dereference(ಎಣಿಕೆ hk_type type)
{
	ಹಿಂತಿರುಗಿ rcu_dereference_all_check(housekeeping.cpumasks[type],
					 housekeeping_dereference_check(type));
}

ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *housekeeping_cpumask(ಎಣಿಕೆ hk_type type)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *ಮುಖವಾಡ = ಖಾಲಿ;

	ಆದರೆ (static_branch_unlikely(&housekeeping_overridden)) {
		ಆದರೆ (READ_ONCE(housekeeping.flags) & ಬಿಟ್(type))
			ಮುಖವಾಡ = housekeeping_cpumask_dereference(type);
	}
	ಆದರೆ (!ಮುಖವಾಡ)
		ಮುಖವಾಡ = cpu_possible_ಮುಖವಾಡ;
	ಹಿಂತಿರುಗಿ ಮುಖವಾಡ;
}
EXPORT_ಚಿಹ್ನೆ_GPL(housekeeping_cpumask);

ಪೂರ್ಣಾಂಕ housekeeping_any_cpu(ಎಣಿಕೆ hk_type type)
{
	ಪೂರ್ಣಾಂಕ cpu;

	ಆದರೆ (static_branch_unlikely(&housekeeping_overridden)) {
		ಆದರೆ (housekeeping.flags & ಬಿಟ್(type)) {
			cpu = sched_numa_ಕಂಡುಹಿಡಿ_closest(housekeeping_cpumask(type), smp_processor_id());
			ಆದರೆ (cpu < nr_cpu_ids)
				ಹಿಂತಿರುಗಿ cpu;

			cpu = cpumask_any_and_distribute(housekeeping_cpumask(type), cpu_online_ಮುಖವಾಡ);
			ಆದರೆ (likely(cpu < nr_cpu_ids))
				ಹಿಂತಿರುಗಿ cpu;
			/*
			 * Unless we have another problem this can only happen
			 * at boot time before start_secondary() brings the 1st
			 * housekeeping CPU up.
			 */
			WARN_ON_ONCE(system_state == SYSTEM_RUNNING ||
				     type != HK_TYPE_TIMER);
		}
	}
	ಹಿಂತಿರುಗಿ smp_processor_id();
}
EXPORT_ಚಿಹ್ನೆ_GPL(housekeeping_any_cpu);

ಶೂನ್ಯ housekeeping_affine(ರಚನೆ task_struct *t, ಎಣಿಕೆ hk_type type)
{
	ಆದರೆ (static_branch_unlikely(&housekeeping_overridden))
		ಆದರೆ (housekeeping.flags & ಬಿಟ್(type))
			ಗಣ_cpus_allowed_ptr(t, housekeeping_cpumask(type));
}
EXPORT_ಚಿಹ್ನೆ_GPL(housekeeping_affine);

bool housekeeping_test_cpu(ಪೂರ್ಣಾಂಕ cpu, ಎಣಿಕೆ hk_type type)
{
	ಆದರೆ (static_branch_unlikely(&housekeeping_overridden) &&
	    READ_ONCE(housekeeping.flags) & ಬಿಟ್(type))
		ಹಿಂತಿರುಗಿ cpumask_test_cpu(cpu, housekeeping_cpumask(type));
	ಹಿಂತಿರುಗಿ true;
}
EXPORT_ಚಿಹ್ನೆ_GPL(housekeeping_test_cpu);

ಪೂರ್ಣಾಂಕ housekeeping_update(ರಚನೆ cpumask *isol_ಮುಖವಾಡ)
{
	ರಚನೆ cpumask *trial, *old = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ err;

	trial = kmalloc(cpumask_ಗಾತ್ರ(), GFP_KERNEL);
	ಆದರೆ (!trial)
		ಹಿಂತಿರುಗಿ -ENOMEM;

	cpumask_andnot(trial, housekeeping_cpumask(HK_TYPE_DOMAIN_BOOT), isol_ಮುಖವಾಡ);
	ಆದರೆ (!cpumask_intersects(trial, cpu_online_ಮುಖವಾಡ)) {
		kfree(trial);
		ಹಿಂತಿರುಗಿ -EINVAL;
	}

	ಆದರೆ (!housekeeping.flags)
		static_branch_enable(&housekeeping_overridden);

	ಆದರೆ (housekeeping.flags & HK_FLAG_DOMAIN)
		old = housekeeping_cpumask_dereference(HK_TYPE_DOMAIN);
	ಇಲ್ಲದಿದ್ದರೆ
		WRITE_ONCE(housekeeping.flags, housekeeping.flags | HK_FLAG_DOMAIN);
	rcu_ನಿಯೋಜಿಸು_ಸೂಚಕ(housekeeping.cpumasks[HK_TYPE_DOMAIN], trial);

	synchronize_rcu();

	pci_probe_flush_workqueue();
	mem_cgroup_flush_workqueue();
	vmstat_flush_workqueue();

	err = workqueue_unbound_housekeeping_update(housekeeping_cpumask(HK_TYPE_DOMAIN));
	WARN_ON_ONCE(err < 0);

	err = tmigr_isolated_exclude_cpumask(isol_ಮುಖವಾಡ);
	WARN_ON_ONCE(err < 0);

	err = kthreads_update_housekeeping();
	WARN_ON_ONCE(err < 0);

	kfree(old);

	ಹಿಂತಿರುಗಿ 0;
}

ಶೂನ್ಯ __init housekeeping_init(ಶೂನ್ಯ)
{
	ಎಣಿಕೆ hk_type type;

	ಆದರೆ (!housekeeping.flags)
		ಹಿಂತಿರುಗಿ;

	static_branch_enable(&housekeeping_overridden);

	ಆದರೆ (housekeeping.flags & HK_FLAG_KERNEL_NOISE)
		sched_tick_offload_init();
	/*
	 * Realloc with a proper allocator so that any cpumask update
	 * can indifferently free the old version with kfree().
	 */
	for_each_ಗಣ_ಬಿಟ್(type, &housekeeping.flags, HK_TYPE_MAX) {
		ರಚನೆ cpumask *omask, *nmask = kmalloc(cpumask_ಗಾತ್ರ(), GFP_KERNEL);

		ಆದರೆ (WARN_ON_ONCE(!nmask))
			ಹಿಂತಿರುಗಿ;

		omask = rcu_dereference(housekeeping.cpumasks[type]);

		/* We need at least one CPU to handle housekeeping work */
		WARN_ON_ONCE(cpumask_ಖಾಲಿಗಳು(omask));
		cpumask_copy(nmask, omask);
		RCU_INIT_ಸೂಚಕ(housekeeping.cpumasks[type], nmask);
		__llist_add((ರಚನೆ llist_ಗಂಟು *)omask, &memblock_freelist);
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ __init housekeeping_late_init(ಶೂನ್ಯ)
{
	ರಚನೆ llist_ಗಂಟು *llnode, *pos, *t;

	/* Free allocated memblock memory, if any */
	llnode = __llist_del_all(&memblock_freelist);
	llist_for_each_ಸುರಕ್ಷಿತ(pos, t, llnode)
		memblock_free(pos, cpumask_ಗಾತ್ರ());
	ಹಿಂತಿರುಗಿ 0;
}
ಶುದ್ಧ_initcall(housekeeping_late_init);

ಸ್ಥಿರ ಶೂನ್ಯ __init housekeeping_ಸಿದ್ಧತೆ_type(ಎಣಿಕೆ hk_type type,
					   cpumask_var_t housekeeping_staging)
{
	ರಚನೆ cpumask *ಮುಖವಾಡ = memblock_alloc_or_panic(cpumask_ಗಾತ್ರ(), SMP_CACHE_ಬೈಟುಗಳು);

	cpumask_copy(ಮುಖವಾಡ, housekeeping_staging);
	RCU_INIT_ಸೂಚಕ(housekeeping.cpumasks[type], ಮುಖವಾಡ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ __init housekeeping_ಸಿದ್ಧತೆ(ಅಕ್ಷರ *str, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags)
{
	cpumask_var_t non_housekeeping_ಮುಖವಾಡ, housekeeping_staging;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಮೊದಲ_cpu;
	ಪೂರ್ಣಾಂಕ err = 0;

	ಆದರೆ ((flags & HK_FLAG_KERNEL_NOISE) && !(housekeeping.flags & HK_FLAG_KERNEL_NOISE)) {
		ಆದರೆ (!IS_ENABLED(CONFIG_NO_HZ_ತುಂಬಿದ)) {
			pr_warn("Housekeeping: nohz unsupported."
				" Build with CONFIG_NO_HZ_FULL\n");
			ಹಿಂತಿರುಗಿ 0;
		}
	}

	alloc_bootmem_cpumask_var(&non_housekeeping_ಮುಖವಾಡ);
	ಆದರೆ (cpulist_parse(str, non_housekeeping_ಮುಖವಾಡ) < 0) {
		pr_warn("Housekeeping: nohz_full= or isolcpus= incorrect CPU range\n");
		ಹೋಗು free_non_housekeeping_ಮುಖವಾಡ;
	}

	alloc_bootmem_cpumask_var(&housekeeping_staging);
	cpumask_andnot(housekeeping_staging,
		       cpu_possible_ಮುಖವಾಡ, non_housekeeping_ಮುಖವಾಡ);

	ಮೊದಲ_cpu = cpumask_ಮೊದಲ_and(cpu_present_ಮುಖವಾಡ, housekeeping_staging);
	ಆದರೆ (ಮೊದಲ_cpu >= nr_cpu_ids || ಮೊದಲ_cpu >= ಸಿದ್ಧತೆ_max_cpus) {
		__cpumask_set_cpu(smp_processor_id(), housekeeping_staging);
		__cpumask_clear_cpu(smp_processor_id(), non_housekeeping_ಮುಖವಾಡ);
		ಆದರೆ (!housekeeping.flags) {
			pr_warn("Housekeeping: must include one present CPU, "
				"using boot CPU:%d\n", smp_processor_id());
		}
	}

	ಆದರೆ (cpumask_ಖಾಲಿಗಳು(non_housekeeping_ಮುಖವಾಡ))
		ಹೋಗು free_housekeeping_staging;

	ಆದರೆ (!housekeeping.flags) {
		/* First setup call ("nohz_full=" or "isolcpus=") */
		ಎಣಿಕೆ hk_type type;

		for_each_ಗಣ_ಬಿಟ್(type, &flags, HK_TYPE_MAX)
			housekeeping_ಸಿದ್ಧತೆ_type(type, housekeeping_staging);
	} ಇಲ್ಲದಿದ್ದರೆ {
		/* Second setup call ("nohz_full=" after "isolcpus=" or the reverse) */
		ಎಣಿಕೆ hk_type type;
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ iter_flags = flags & housekeeping.flags;

		for_each_ಗಣ_ಬಿಟ್(type, &iter_flags, HK_TYPE_MAX) {
			ಆದರೆ (!cpumask_equal(housekeeping_staging,
					   housekeeping_cpumask(type))) {
				pr_warn("Housekeeping: nohz_full= must match isolcpus=\n");
				ಹೋಗು free_housekeeping_staging;
			}
		}

		/*
		 * Check the combination of nohz_full and isolcpus=domain,
		 * necessary to avoid problems with the timer migration
		 * hierarchy. managed_irq is ignored by this check since it
		 * isn't considered in the timer migration logic.
		 */
		iter_flags = housekeeping.flags & (HK_FLAG_KERNEL_NOISE | HK_FLAG_DOMAIN);
		type = ಕಂಡುಹಿಡಿ_ಮೊದಲ_ಬಿಟ್(&iter_flags, HK_TYPE_MAX);
		/*
		 * Pass the check if none of these flags were previously set or
		 * are not in the current selection.
		 */
		iter_flags = flags & (HK_FLAG_KERNEL_NOISE | HK_FLAG_DOMAIN);
		ಮೊದಲ_cpu = (type == HK_TYPE_MAX || !iter_flags) ? 0 :
			    cpumask_ಮೊದಲ_and_and(cpu_present_ಮುಖವಾಡ,
						  housekeeping_staging, housekeeping_cpumask(type));
		ಆದರೆ (ಮೊದಲ_cpu >= min(nr_cpu_ids, ಸಿದ್ಧತೆ_max_cpus)) {
			pr_warn("Housekeeping: must include one present CPU "
				"neither in nohz_full= nor in isolcpus=domain, "
				"ignoring setting %s\n", str);
			ಹೋಗು free_housekeeping_staging;
		}

		iter_flags = flags & ~housekeeping.flags;

		for_each_ಗಣ_ಬಿಟ್(type, &iter_flags, HK_TYPE_MAX)
			housekeeping_ಸಿದ್ಧತೆ_type(type, housekeeping_staging);
	}

	ಆದರೆ ((flags & HK_FLAG_KERNEL_NOISE) && !(housekeeping.flags & HK_FLAG_KERNEL_NOISE))
		tick_nohz_ತುಂಬಿದ_ಸಿದ್ಧತೆ(non_housekeeping_ಮುಖವಾಡ);

	housekeeping.flags |= flags;
	err = 1;

free_housekeeping_staging:
	free_bootmem_cpumask_var(housekeeping_staging);
free_non_housekeeping_ಮುಖವಾಡ:
	free_bootmem_cpumask_var(non_housekeeping_ಮುಖವಾಡ);

	ಹಿಂತಿರುಗಿ err;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ __init housekeeping_nohz_ತುಂಬಿದ_ಸಿದ್ಧತೆ(ಅಕ್ಷರ *str)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags;

	flags = HK_FLAG_KERNEL_NOISE;

	ಹಿಂತಿರುಗಿ housekeeping_ಸಿದ್ಧತೆ(str, flags);
}
__setup("nohz_full=", housekeeping_nohz_ತುಂಬಿದ_ಸಿದ್ಧತೆ);

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ __init housekeeping_isolcpus_ಸಿದ್ಧತೆ(ಅಕ್ಷರ *str)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags = 0;
	bool illegal = false;
	ಅಕ್ಷರ *par;
	ಪೂರ್ಣಾಂಕ len;

	ಆಗ (ಅಕ್ಷರವೇ(*str)) {
		/*
		 * isolcpus=nohz is equivalent to nohz_full.
		 */
		ಆದರೆ (!ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ(str, "nohz,", 5)) {
			str += 5;
			flags |= HK_FLAG_KERNEL_NOISE;
			ಮುಂದುವರಿ;
		}

		ಆದರೆ (!ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ(str, "domain,", 7)) {
			str += 7;
			flags |= HK_FLAG_DOMAIN | HK_FLAG_DOMAIN_BOOT;
			ಮುಂದುವರಿ;
		}

		ಆದರೆ (!ಅಕ್ಷರಸರಣಿ_ಹೋಲಿಕೆ(str, "managed_irq,", 12)) {
			str += 12;
			flags |= HK_FLAG_MANAGED_IRQ;
			ಮುಂದುವರಿ;
		}

		/*
		 * Skip unknown sub-parameter and validate that it is not
		 * containing an invalid character.
		 */
		ಗಾಗಿ (par = str, len = 0; *str && *str != ','; str++, len++) {
			ಆದರೆ (!ಅಕ್ಷರವೇ(*str) && *str != '_')
				illegal = true;
		}

		ಆದರೆ (illegal) {
			pr_warn("isolcpus: Invalid flag %.*s\n", len, par);
			ಹಿಂತಿರುಗಿ 0;
		}

		pr_info("isolcpus: Skipped unknown flag %.*s\n", len, par);
		str++;
	}

	/* Default behaviour for isolcpus without flags */
	ಆದರೆ (!flags)
		flags |= HK_FLAG_DOMAIN | HK_FLAG_DOMAIN_BOOT;

	ಹಿಂತಿರುಗಿ housekeeping_ಸಿದ್ಧತೆ(str, flags);
}
__setup("isolcpus=", housekeeping_isolcpus_ಸಿದ್ಧತೆ);
