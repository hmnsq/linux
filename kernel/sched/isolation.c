// SPDX-License-Identifier: GPL-2.0-only
/*
 *  Housekeeping management. Manage the targets for routine code that can run on
 *  any CPU: unbound workqueues, timers, kthreads and any offloadable work.
 *
 * Copyright (C) 2017 Red Hat, Inc., Frederic Weisbecker
 * Copyright (C) 2017-2018 SUSE, Frederic Weisbecker
 *
 */
#അന്തര്ഭാവ <linux/sched/isolation.h>
#അന്തര്ഭാവ <linux/llist.h>
#അന്തര്ഭാവ <linux/pci.h>
#അന്തര്ഭാവ "sched.h"

എണ്ണൽ hk_flags {
	HK_FLAG_DOMAIN_BOOT	= BIT(HK_TYPE_DOMAIN_BOOT),
	HK_FLAG_DOMAIN		= BIT(HK_TYPE_DOMAIN),
	HK_FLAG_MANAGED_IRQ	= BIT(HK_TYPE_MANAGED_IRQ),
	HK_FLAG_KERNEL_NOISE	= BIT(HK_TYPE_KERNEL_NOISE),
};

DEFINE_STATIC_താക്കോൽ_FALSE(housekeeping_overridden);
EXPORT_ചിഹ്നം_GPL(housekeeping_overridden);

ഘടന housekeeping {
	ഘടന cpumask __rcu *cpumasks[HK_TYPE_MAX];
	unsigned long flags;
};

സ്ഥിതം ഘടന housekeeping housekeeping;
സ്ഥിതം __initdata LLIST_HEAD(memblock_freelist);

bool housekeeping_enabled(എണ്ണൽ hk_type type)
{
	തിരികെ !!(READ_ONCE(housekeeping.flags) & BIT(type));
}
EXPORT_ചിഹ്നം_GPL(housekeeping_enabled);

സ്ഥിതം bool housekeeping_dereference_check(എണ്ണൽ hk_type type)
{
	എങ്കിൽ (IS_ENABLED(CONFIG_LOCKDEP) && type == HK_TYPE_DOMAIN) {
		/* Cpuset isn't even writable yet? */
		എങ്കിൽ (system_state <= SYSTEM_SCHEDULING)
			തിരികെ true;

		/* CPU hotplug write locked, so cpuset partition can't be overwritten */
		എങ്കിൽ (IS_ENABLED(CONFIG_HOTPLUG_CPU) && lockdep_is_cpus_write_held())
			തിരികെ true;

		/* Cpuset lock held, partitions not writable */
		എങ്കിൽ (IS_ENABLED(CONFIG_CPUSETS) && lockdep_is_cpuset_held())
			തിരികെ true;

		തിരികെ false;
	}

	തിരികെ true;
}

സ്ഥിതം വരിയിൽ ഘടന cpumask *housekeeping_cpumask_dereference(എണ്ണൽ hk_type type)
{
	തിരികെ rcu_dereference_all_check(housekeeping.cpumasks[type],
					 housekeeping_dereference_check(type));
}

സ്ഥിരം ഘടന cpumask *housekeeping_cpumask(എണ്ണൽ hk_type type)
{
	സ്ഥിരം ഘടന cpumask *mask = ശൂന്യ;

	എങ്കിൽ (static_branch_unlikely(&housekeeping_overridden)) {
		എങ്കിൽ (READ_ONCE(housekeeping.flags) & BIT(type))
			mask = housekeeping_cpumask_dereference(type);
	}
	എങ്കിൽ (!mask)
		mask = cpu_possible_mask;
	തിരികെ mask;
}
EXPORT_ചിഹ്നം_GPL(housekeeping_cpumask);

പൂർണ്ണസംഖ്യ housekeeping_any_cpu(എണ്ണൽ hk_type type)
{
	പൂർണ്ണസംഖ്യ cpu;

	എങ്കിൽ (static_branch_unlikely(&housekeeping_overridden)) {
		എങ്കിൽ (housekeeping.flags & BIT(type)) {
			cpu = sched_numa_find_closest(housekeeping_cpumask(type), smp_processor_id());
			എങ്കിൽ (cpu < nr_cpu_ids)
				തിരികെ cpu;

			cpu = cpumask_any_and_distribute(housekeeping_cpumask(type), cpu_online_mask);
			എങ്കിൽ (likely(cpu < nr_cpu_ids))
				തിരികെ cpu;
			/*
			 * Unless we have another problem this can only happen
			 * at boot time before start_secondary() brings the 1st
			 * housekeeping CPU up.
			 */
			WARN_ON_ONCE(system_state == SYSTEM_RUNNING ||
				     type != HK_TYPE_TIMER);
		}
	}
	തിരികെ smp_processor_id();
}
EXPORT_ചിഹ്നം_GPL(housekeeping_any_cpu);

ശൂന്യം housekeeping_affine(ഘടന task_struct *t, എണ്ണൽ hk_type type)
{
	എങ്കിൽ (static_branch_unlikely(&housekeeping_overridden))
		എങ്കിൽ (housekeeping.flags & BIT(type))
			set_cpus_allowed_ptr(t, housekeeping_cpumask(type));
}
EXPORT_ചിഹ്നം_GPL(housekeeping_affine);

bool housekeeping_test_cpu(പൂർണ്ണസംഖ്യ cpu, എണ്ണൽ hk_type type)
{
	എങ്കിൽ (static_branch_unlikely(&housekeeping_overridden) &&
	    READ_ONCE(housekeeping.flags) & BIT(type))
		തിരികെ cpumask_test_cpu(cpu, housekeeping_cpumask(type));
	തിരികെ true;
}
EXPORT_ചിഹ്നം_GPL(housekeeping_test_cpu);

പൂർണ്ണസംഖ്യ housekeeping_update(ഘടന cpumask *isol_mask)
{
	ഘടന cpumask *trial, *old = ശൂന്യ;
	പൂർണ്ണസംഖ്യ err;

	trial = kmalloc(cpumask_size(), GFP_KERNEL);
	എങ്കിൽ (!trial)
		തിരികെ -ENOMEM;

	cpumask_andnot(trial, housekeeping_cpumask(HK_TYPE_DOMAIN_BOOT), isol_mask);
	എങ്കിൽ (!cpumask_intersects(trial, cpu_online_mask)) {
		kfree(trial);
		തിരികെ -EINVAL;
	}

	എങ്കിൽ (!housekeeping.flags)
		static_branch_enable(&housekeeping_overridden);

	എങ്കിൽ (housekeeping.flags & HK_FLAG_DOMAIN)
		old = housekeeping_cpumask_dereference(HK_TYPE_DOMAIN);
	അല്ലെങ്കിൽ
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

	തിരികെ 0;
}

ശൂന്യം __init housekeeping_init(ശൂന്യം)
{
	എണ്ണൽ hk_type type;

	എങ്കിൽ (!housekeeping.flags)
		തിരികെ;

	static_branch_enable(&housekeeping_overridden);

	എങ്കിൽ (housekeeping.flags & HK_FLAG_KERNEL_NOISE)
		sched_tick_offload_init();
	/*
	 * Realloc with a proper allocator so that any cpumask update
	 * can indifferently free the old version with kfree().
	 */
	for_each_set_bit(type, &housekeeping.flags, HK_TYPE_MAX) {
		ഘടന cpumask *omask, *nmask = kmalloc(cpumask_size(), GFP_KERNEL);

		എങ്കിൽ (WARN_ON_ONCE(!nmask))
			തിരികെ;

		omask = rcu_dereference(housekeeping.cpumasks[type]);

		/* We need at least one CPU to handle housekeeping work */
		WARN_ON_ONCE(cpumask_empty(omask));
		cpumask_copy(nmask, omask);
		RCU_INIT_POINTER(housekeeping.cpumasks[type], nmask);
		__llist_add((ഘടന llist_കണ്ണി *)omask, &memblock_freelist);
	}
}

സ്ഥിതം പൂർണ്ണസംഖ്യ __init housekeeping_late_init(ശൂന്യം)
{
	ഘടന llist_കണ്ണി *llnode, *pos, *t;

	/* Free allocated memblock memory, if any */
	llnode = __llist_del_all(&memblock_freelist);
	llist_for_each_safe(pos, t, llnode)
		memblock_free(pos, cpumask_size());
	തിരികെ 0;
}
pure_initcall(housekeeping_late_init);

സ്ഥിതം ശൂന്യം __init housekeeping_setup_type(എണ്ണൽ hk_type type,
					   cpumask_var_t housekeeping_staging)
{
	ഘടന cpumask *mask = memblock_alloc_or_panic(cpumask_size(), SMP_CACHE_BYTES);

	cpumask_copy(mask, housekeeping_staging);
	RCU_INIT_POINTER(housekeeping.cpumasks[type], mask);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ __init housekeeping_setup(അക്ഷരം *str, unsigned long flags)
{
	cpumask_var_t non_housekeeping_mask, housekeeping_staging;
	unsigned പൂർണ്ണസംഖ്യ first_cpu;
	പൂർണ്ണസംഖ്യ err = 0;

	എങ്കിൽ ((flags & HK_FLAG_KERNEL_NOISE) && !(housekeeping.flags & HK_FLAG_KERNEL_NOISE)) {
		എങ്കിൽ (!IS_ENABLED(CONFIG_NO_HZ_FULL)) {
			pr_warn("Housekeeping: nohz unsupported."
				" Build with CONFIG_NO_HZ_FULL\n");
			തിരികെ 0;
		}
	}

	alloc_bootmem_cpumask_var(&non_housekeeping_mask);
	എങ്കിൽ (cpulist_parse(str, non_housekeeping_mask) < 0) {
		pr_warn("Housekeeping: nohz_full= or isolcpus= incorrect CPU range\n");
		പോകുക free_non_housekeeping_mask;
	}

	alloc_bootmem_cpumask_var(&housekeeping_staging);
	cpumask_andnot(housekeeping_staging,
		       cpu_possible_mask, non_housekeeping_mask);

	first_cpu = cpumask_first_and(cpu_present_mask, housekeeping_staging);
	എങ്കിൽ (first_cpu >= nr_cpu_ids || first_cpu >= setup_max_cpus) {
		__cpumask_set_cpu(smp_processor_id(), housekeeping_staging);
		__cpumask_clear_cpu(smp_processor_id(), non_housekeeping_mask);
		എങ്കിൽ (!housekeeping.flags) {
			pr_warn("Housekeeping: must include one present CPU, "
				"using boot CPU:%d\n", smp_processor_id());
		}
	}

	എങ്കിൽ (cpumask_empty(non_housekeeping_mask))
		പോകുക free_housekeeping_staging;

	എങ്കിൽ (!housekeeping.flags) {
		/* First setup call ("nohz_full=" or "isolcpus=") */
		എണ്ണൽ hk_type type;

		for_each_set_bit(type, &flags, HK_TYPE_MAX)
			housekeeping_setup_type(type, housekeeping_staging);
	} അല്ലെങ്കിൽ {
		/* Second setup call ("nohz_full=" after "isolcpus=" or the reverse) */
		എണ്ണൽ hk_type type;
		unsigned long iter_flags = flags & housekeeping.flags;

		for_each_set_bit(type, &iter_flags, HK_TYPE_MAX) {
			എങ്കിൽ (!cpumask_equal(housekeeping_staging,
					   housekeeping_cpumask(type))) {
				pr_warn("Housekeeping: nohz_full= must match isolcpus=\n");
				പോകുക free_housekeeping_staging;
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
		എങ്കിൽ (first_cpu >= min(nr_cpu_ids, setup_max_cpus)) {
			pr_warn("Housekeeping: must include one present CPU "
				"neither in nohz_full= nor in isolcpus=domain, "
				"ignoring setting %s\n", str);
			പോകുക free_housekeeping_staging;
		}

		iter_flags = flags & ~housekeeping.flags;

		for_each_set_bit(type, &iter_flags, HK_TYPE_MAX)
			housekeeping_setup_type(type, housekeeping_staging);
	}

	എങ്കിൽ ((flags & HK_FLAG_KERNEL_NOISE) && !(housekeeping.flags & HK_FLAG_KERNEL_NOISE))
		tick_nohz_full_setup(non_housekeeping_mask);

	housekeeping.flags |= flags;
	err = 1;

free_housekeeping_staging:
	free_bootmem_cpumask_var(housekeeping_staging);
free_non_housekeeping_mask:
	free_bootmem_cpumask_var(non_housekeeping_mask);

	തിരികെ err;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ __init housekeeping_nohz_full_setup(അക്ഷരം *str)
{
	unsigned long flags;

	flags = HK_FLAG_KERNEL_NOISE;

	തിരികെ housekeeping_setup(str, flags);
}
__setup("nohz_full=", housekeeping_nohz_full_setup);

സ്ഥിതം പൂർണ്ണസംഖ്യ __init housekeeping_isolcpus_setup(അക്ഷരം *str)
{
	unsigned long flags = 0;
	bool illegal = false;
	അക്ഷരം *par;
	പൂർണ്ണസംഖ്യ len;

	ആകുമ്പോൾ (isalpha(*str)) {
		/*
		 * isolcpus=nohz is equivalent to nohz_full.
		 */
		എങ്കിൽ (!strncmp(str, "nohz,", 5)) {
			str += 5;
			flags |= HK_FLAG_KERNEL_NOISE;
			തുടരുക;
		}

		എങ്കിൽ (!strncmp(str, "domain,", 7)) {
			str += 7;
			flags |= HK_FLAG_DOMAIN | HK_FLAG_DOMAIN_BOOT;
			തുടരുക;
		}

		എങ്കിൽ (!strncmp(str, "managed_irq,", 12)) {
			str += 12;
			flags |= HK_FLAG_MANAGED_IRQ;
			തുടരുക;
		}

		/*
		 * Skip unknown sub-parameter and validate that it is not
		 * containing an invalid character.
		 */
		വേണ്ടി (par = str, len = 0; *str && *str != ','; str++, len++) {
			എങ്കിൽ (!isalpha(*str) && *str != '_')
				illegal = true;
		}

		എങ്കിൽ (illegal) {
			pr_warn("isolcpus: Invalid flag %.*s\n", len, par);
			തിരികെ 0;
		}

		pr_info("isolcpus: Skipped unknown flag %.*s\n", len, par);
		str++;
	}

	/* Default behaviour for isolcpus without flags */
	എങ്കിൽ (!flags)
		flags |= HK_FLAG_DOMAIN | HK_FLAG_DOMAIN_BOOT;

	തിരികെ housekeeping_setup(str, flags);
}
__setup("isolcpus=", housekeeping_isolcpus_setup);
