// SPDX-License-Identifier: GPL-2.0-only
/*
 *  Housekeeping management. Manage the targets for routine code that can run on
 *  any CPU: unbound workqueues, timers, kthreads and any offloadable work.
 *
 * Copyright (C) 2017 Red Hat, Inc., Frederic Weisbecker
 * Copyright (C) 2017-2018 SUSE, Frederic Weisbecker
 *
 */
#अन्तर्भाव <linux/sched/isolation.h>
#अन्तर्भाव <linux/llist.h>
#अन्तर्भाव <linux/pci.h>
#अन्तर्भाव "sched.h"

गणना hk_flags {
	HK_FLAG_DOMAIN_BOOT	= BIT(HK_TYPE_DOMAIN_BOOT),
	HK_FLAG_DOMAIN		= BIT(HK_TYPE_DOMAIN),
	HK_FLAG_MANAGED_IRQ	= BIT(HK_TYPE_MANAGED_IRQ),
	HK_FLAG_KERNEL_NOISE	= BIT(HK_TYPE_KERNEL_NOISE),
};

DEFINE_STATIC_कुञ्चिका_FALSE(housekeeping_overridden);
EXPORT_चिह्नम्_GPL(housekeeping_overridden);

संरचना housekeeping {
	संरचना cpumask __rcu *cpumasks[HK_TYPE_MAX];
	unsigned long flags;
};

स्थिर संरचना housekeeping housekeeping;
स्थिर __initdata LLIST_HEAD(memblock_freelist);

bool housekeeping_enabled(गणना hk_type type)
{
	पटिनिवत्त !!(READ_ONCE(housekeeping.flags) & BIT(type));
}
EXPORT_चिह्नम्_GPL(housekeeping_enabled);

स्थिर bool housekeeping_dereference_check(गणना hk_type type)
{
	सचे (IS_ENABLED(CONFIG_LOCKDEP) && type == HK_TYPE_DOMAIN) {
		/* Cpuset isn't even writable yet? */
		सचे (system_state <= SYSTEM_SCHEDULING)
			पटिनिवत्त true;

		/* CPU hotplug write locked, so cpuset partition can't be overwritten */
		सचे (IS_ENABLED(CONFIG_HOTPLUG_CPU) && lockdep_is_cpus_write_held())
			पटिनिवत्त true;

		/* Cpuset lock held, partitions not writable */
		सचे (IS_ENABLED(CONFIG_CPUSETS) && lockdep_is_cpuset_held())
			पटिनिवत्त true;

		पटिनिवत्त false;
	}

	पटिनिवत्त true;
}

स्थिर अन्तःस्थ संरचना cpumask *housekeeping_cpumask_dereference(गणना hk_type type)
{
	पटिनिवत्त rcu_dereference_all_check(housekeeping.cpumasks[type],
					 housekeeping_dereference_check(type));
}

ध्रुव संरचना cpumask *housekeeping_cpumask(गणना hk_type type)
{
	ध्रुव संरचना cpumask *mask = रिक्त;

	सचे (static_branch_unlikely(&housekeeping_overridden)) {
		सचे (READ_ONCE(housekeeping.flags) & BIT(type))
			mask = housekeeping_cpumask_dereference(type);
	}
	सचे (!mask)
		mask = cpu_possible_mask;
	पटिनिवत्त mask;
}
EXPORT_चिह्नम्_GPL(housekeeping_cpumask);

पूर्णाङ्क housekeeping_any_cpu(गणना hk_type type)
{
	पूर्णाङ्क cpu;

	सचे (static_branch_unlikely(&housekeeping_overridden)) {
		सचे (housekeeping.flags & BIT(type)) {
			cpu = sched_numa_find_closest(housekeeping_cpumask(type), smp_processor_id());
			सचे (cpu < nr_cpu_ids)
				पटिनिवत्त cpu;

			cpu = cpumask_any_and_distribute(housekeeping_cpumask(type), cpu_online_mask);
			सचे (likely(cpu < nr_cpu_ids))
				पटिनिवत्त cpu;
			/*
			 * Unless we have another problem this can only happen
			 * at boot time before start_secondary() brings the 1st
			 * housekeeping CPU up.
			 */
			WARN_ON_ONCE(system_state == SYSTEM_RUNNING ||
				     type != HK_TYPE_TIMER);
		}
	}
	पटिनिवत्त smp_processor_id();
}
EXPORT_चिह्नम्_GPL(housekeeping_any_cpu);

शून्य housekeeping_affine(संरचना task_struct *t, गणना hk_type type)
{
	सचे (static_branch_unlikely(&housekeeping_overridden))
		सचे (housekeeping.flags & BIT(type))
			set_cpus_allowed_ptr(t, housekeeping_cpumask(type));
}
EXPORT_चिह्नम्_GPL(housekeeping_affine);

bool housekeeping_test_cpu(पूर्णाङ्क cpu, गणना hk_type type)
{
	सचे (static_branch_unlikely(&housekeeping_overridden) &&
	    READ_ONCE(housekeeping.flags) & BIT(type))
		पटिनिवत्त cpumask_test_cpu(cpu, housekeeping_cpumask(type));
	पटिनिवत्त true;
}
EXPORT_चिह्नम्_GPL(housekeeping_test_cpu);

पूर्णाङ्क housekeeping_update(संरचना cpumask *isol_mask)
{
	संरचना cpumask *trial, *old = रिक्त;
	पूर्णाङ्क err;

	trial = kmalloc(cpumask_size(), GFP_KERNEL);
	सचे (!trial)
		पटिनिवत्त -ENOMEM;

	cpumask_andnot(trial, housekeeping_cpumask(HK_TYPE_DOMAIN_BOOT), isol_mask);
	सचे (!cpumask_intersects(trial, cpu_online_mask)) {
		kfree(trial);
		पटिनिवत्त -EINVAL;
	}

	सचे (!housekeeping.flags)
		static_branch_enable(&housekeeping_overridden);

	सचे (housekeeping.flags & HK_FLAG_DOMAIN)
		old = housekeeping_cpumask_dereference(HK_TYPE_DOMAIN);
	अञ्ञथा
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

	पटिनिवत्त 0;
}

शून्य __init housekeeping_init(शून्य)
{
	गणना hk_type type;

	सचे (!housekeeping.flags)
		पटिनिवत्त;

	static_branch_enable(&housekeeping_overridden);

	सचे (housekeeping.flags & HK_FLAG_KERNEL_NOISE)
		sched_tick_offload_init();
	/*
	 * Realloc with a proper allocator so that any cpumask update
	 * can indifferently free the old version with kfree().
	 */
	for_each_set_bit(type, &housekeeping.flags, HK_TYPE_MAX) {
		संरचना cpumask *omask, *nmask = kmalloc(cpumask_size(), GFP_KERNEL);

		सचे (WARN_ON_ONCE(!nmask))
			पटिनिवत्त;

		omask = rcu_dereference(housekeeping.cpumasks[type]);

		/* We need at least one CPU to handle housekeeping work */
		WARN_ON_ONCE(cpumask_empty(omask));
		cpumask_copy(nmask, omask);
		RCU_INIT_POINTER(housekeeping.cpumasks[type], nmask);
		__llist_add((संरचना llist_ग्रन्थिः *)omask, &memblock_freelist);
	}
}

स्थिर पूर्णाङ्क __init housekeeping_late_init(शून्य)
{
	संरचना llist_ग्रन्थिः *llnode, *pos, *t;

	/* Free allocated memblock memory, if any */
	llnode = __llist_del_all(&memblock_freelist);
	llist_for_each_safe(pos, t, llnode)
		memblock_free(pos, cpumask_size());
	पटिनिवत्त 0;
}
pure_initcall(housekeeping_late_init);

स्थिर शून्य __init housekeeping_setup_type(गणना hk_type type,
					   cpumask_var_t housekeeping_staging)
{
	संरचना cpumask *mask = memblock_alloc_or_panic(cpumask_size(), SMP_CACHE_BYTES);

	cpumask_copy(mask, housekeeping_staging);
	RCU_INIT_POINTER(housekeeping.cpumasks[type], mask);
}

स्थिर पूर्णाङ्क __init housekeeping_setup(वर्ण *str, unsigned long flags)
{
	cpumask_var_t non_housekeeping_mask, housekeeping_staging;
	unsigned पूर्णाङ्क first_cpu;
	पूर्णाङ्क err = 0;

	सचे ((flags & HK_FLAG_KERNEL_NOISE) && !(housekeeping.flags & HK_FLAG_KERNEL_NOISE)) {
		सचे (!IS_ENABLED(CONFIG_NO_HZ_FULL)) {
			pr_warn("Housekeeping: nohz unsupported."
				" Build with CONFIG_NO_HZ_FULL\n");
			पटिनिवत्त 0;
		}
	}

	alloc_bootmem_cpumask_var(&non_housekeeping_mask);
	सचे (cpulist_parse(str, non_housekeeping_mask) < 0) {
		pr_warn("Housekeeping: nohz_full= or isolcpus= incorrect CPU range\n");
		गच्छ free_non_housekeeping_mask;
	}

	alloc_bootmem_cpumask_var(&housekeeping_staging);
	cpumask_andnot(housekeeping_staging,
		       cpu_possible_mask, non_housekeeping_mask);

	first_cpu = cpumask_first_and(cpu_present_mask, housekeeping_staging);
	सचे (first_cpu >= nr_cpu_ids || first_cpu >= setup_max_cpus) {
		__cpumask_set_cpu(smp_processor_id(), housekeeping_staging);
		__cpumask_clear_cpu(smp_processor_id(), non_housekeeping_mask);
		सचे (!housekeeping.flags) {
			pr_warn("Housekeeping: must include one present CPU, "
				"using boot CPU:%d\n", smp_processor_id());
		}
	}

	सचे (cpumask_empty(non_housekeeping_mask))
		गच्छ free_housekeeping_staging;

	सचे (!housekeeping.flags) {
		/* First setup call ("nohz_full=" or "isolcpus=") */
		गणना hk_type type;

		for_each_set_bit(type, &flags, HK_TYPE_MAX)
			housekeeping_setup_type(type, housekeeping_staging);
	} अञ्ञथा {
		/* Second setup call ("nohz_full=" after "isolcpus=" or the reverse) */
		गणना hk_type type;
		unsigned long iter_flags = flags & housekeeping.flags;

		for_each_set_bit(type, &iter_flags, HK_TYPE_MAX) {
			सचे (!cpumask_equal(housekeeping_staging,
					   housekeeping_cpumask(type))) {
				pr_warn("Housekeeping: nohz_full= must match isolcpus=\n");
				गच्छ free_housekeeping_staging;
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
		सचे (first_cpu >= min(nr_cpu_ids, setup_max_cpus)) {
			pr_warn("Housekeeping: must include one present CPU "
				"neither in nohz_full= nor in isolcpus=domain, "
				"ignoring setting %s\n", str);
			गच्छ free_housekeeping_staging;
		}

		iter_flags = flags & ~housekeeping.flags;

		for_each_set_bit(type, &iter_flags, HK_TYPE_MAX)
			housekeeping_setup_type(type, housekeeping_staging);
	}

	सचे ((flags & HK_FLAG_KERNEL_NOISE) && !(housekeeping.flags & HK_FLAG_KERNEL_NOISE))
		tick_nohz_full_setup(non_housekeeping_mask);

	housekeeping.flags |= flags;
	err = 1;

free_housekeeping_staging:
	free_bootmem_cpumask_var(housekeeping_staging);
free_non_housekeeping_mask:
	free_bootmem_cpumask_var(non_housekeeping_mask);

	पटिनिवत्त err;
}

स्थिर पूर्णाङ्क __init housekeeping_nohz_full_setup(वर्ण *str)
{
	unsigned long flags;

	flags = HK_FLAG_KERNEL_NOISE;

	पटिनिवत्त housekeeping_setup(str, flags);
}
__setup("nohz_full=", housekeeping_nohz_full_setup);

स्थिर पूर्णाङ्क __init housekeeping_isolcpus_setup(वर्ण *str)
{
	unsigned long flags = 0;
	bool illegal = false;
	वर्ण *par;
	पूर्णाङ्क len;

	याव (isalpha(*str)) {
		/*
		 * isolcpus=nohz is equivalent to nohz_full.
		 */
		सचे (!strncmp(str, "nohz,", 5)) {
			str += 5;
			flags |= HK_FLAG_KERNEL_NOISE;
			अनुवर्त;
		}

		सचे (!strncmp(str, "domain,", 7)) {
			str += 7;
			flags |= HK_FLAG_DOMAIN | HK_FLAG_DOMAIN_BOOT;
			अनुवर्त;
		}

		सचे (!strncmp(str, "managed_irq,", 12)) {
			str += 12;
			flags |= HK_FLAG_MANAGED_IRQ;
			अनुवर्त;
		}

		/*
		 * Skip unknown sub-parameter and validate that it is not
		 * containing an invalid character.
		 */
		अत्थाय (par = str, len = 0; *str && *str != ','; str++, len++) {
			सचे (!isalpha(*str) && *str != '_')
				illegal = true;
		}

		सचे (illegal) {
			pr_warn("isolcpus: Invalid flag %.*s\n", len, par);
			पटिनिवत्त 0;
		}

		pr_info("isolcpus: Skipped unknown flag %.*s\n", len, par);
		str++;
	}

	/* Default behaviour for isolcpus without flags */
	सचे (!flags)
		flags |= HK_FLAG_DOMAIN | HK_FLAG_DOMAIN_BOOT;

	पटिनिवत्त housekeeping_setup(str, flags);
}
__setup("isolcpus=", housekeeping_isolcpus_setup);
