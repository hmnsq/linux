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

DEFINE_STATIC_किल्ली_FALSE(housekeeping_overridden);
EXPORT_चिन्ह_GPL(housekeeping_overridden);

संरचना housekeeping {
	संरचना cpumask __rcu *cpumasks[HK_TYPE_MAX];
	अचिन्हित दीर्घ flags;
};

स्थिर संरचना housekeeping housekeeping;
स्थिर __initdata LLIST_HEAD(memblock_freelist);

bool housekeeping_enabled(गणना hk_type type)
{
	परत !!(READ_ONCE(housekeeping.flags) & BIT(type));
}
EXPORT_चिन्ह_GPL(housekeeping_enabled);

स्थिर bool housekeeping_dereference_check(गणना hk_type type)
{
	जर (IS_ENABLED(CONFIG_LOCKDEP) && type == HK_TYPE_DOMAIN) {
		/* Cpuset isn't even writable yet? */
		जर (system_state <= SYSTEM_SCHEDULING)
			परत true;

		/* CPU hotplug write locked, so cpuset partition can't be overwritten */
		जर (IS_ENABLED(CONFIG_HOTPLUG_CPU) && lockdep_is_cpus_write_held())
			परत true;

		/* Cpuset lock held, partitions not writable */
		जर (IS_ENABLED(CONFIG_CPUSETS) && lockdep_is_cpuset_held())
			परत true;

		परत false;
	}

	परत true;
}

स्थिर ओळीत संरचना cpumask *housekeeping_cpumask_dereference(गणना hk_type type)
{
	परत rcu_dereference_all_check(housekeeping.cpumasks[type],
					 housekeeping_dereference_check(type));
}

स्थिरांक संरचना cpumask *housekeeping_cpumask(गणना hk_type type)
{
	स्थिरांक संरचना cpumask *mask = रिक्त;

	जर (static_branch_unlikely(&housekeeping_overridden)) {
		जर (READ_ONCE(housekeeping.flags) & BIT(type))
			mask = housekeeping_cpumask_dereference(type);
	}
	जर (!mask)
		mask = cpu_possible_mask;
	परत mask;
}
EXPORT_चिन्ह_GPL(housekeeping_cpumask);

पूर्णांक housekeeping_any_cpu(गणना hk_type type)
{
	पूर्णांक cpu;

	जर (static_branch_unlikely(&housekeeping_overridden)) {
		जर (housekeeping.flags & BIT(type)) {
			cpu = sched_numa_find_closest(housekeeping_cpumask(type), smp_processor_id());
			जर (cpu < nr_cpu_ids)
				परत cpu;

			cpu = cpumask_any_and_distribute(housekeeping_cpumask(type), cpu_online_mask);
			जर (likely(cpu < nr_cpu_ids))
				परत cpu;
			/*
			 * Unless we have another problem this can only happen
			 * at boot time before start_secondary() brings the 1st
			 * housekeeping CPU up.
			 */
			WARN_ON_ONCE(system_state == SYSTEM_RUNNING ||
				     type != HK_TYPE_TIMER);
		}
	}
	परत smp_processor_id();
}
EXPORT_चिन्ह_GPL(housekeeping_any_cpu);

शून्य housekeeping_affine(संरचना task_struct *t, गणना hk_type type)
{
	जर (static_branch_unlikely(&housekeeping_overridden))
		जर (housekeeping.flags & BIT(type))
			set_cpus_allowed_ptr(t, housekeeping_cpumask(type));
}
EXPORT_चिन्ह_GPL(housekeeping_affine);

bool housekeeping_test_cpu(पूर्णांक cpu, गणना hk_type type)
{
	जर (static_branch_unlikely(&housekeeping_overridden) &&
	    READ_ONCE(housekeeping.flags) & BIT(type))
		परत cpumask_test_cpu(cpu, housekeeping_cpumask(type));
	परत true;
}
EXPORT_चिन्ह_GPL(housekeeping_test_cpu);

पूर्णांक housekeeping_update(संरचना cpumask *isol_mask)
{
	संरचना cpumask *trial, *old = रिक्त;
	पूर्णांक err;

	trial = kmalloc(cpumask_size(), GFP_KERNEL);
	जर (!trial)
		परत -ENOMEM;

	cpumask_andnot(trial, housekeeping_cpumask(HK_TYPE_DOMAIN_BOOT), isol_mask);
	जर (!cpumask_intersects(trial, cpu_online_mask)) {
		kfree(trial);
		परत -EINVAL;
	}

	जर (!housekeeping.flags)
		static_branch_enable(&housekeeping_overridden);

	जर (housekeeping.flags & HK_FLAG_DOMAIN)
		old = housekeeping_cpumask_dereference(HK_TYPE_DOMAIN);
	अन्यथा
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

	परत 0;
}

शून्य __init housekeeping_init(शून्य)
{
	गणना hk_type type;

	जर (!housekeeping.flags)
		परत;

	static_branch_enable(&housekeeping_overridden);

	जर (housekeeping.flags & HK_FLAG_KERNEL_NOISE)
		sched_tick_offload_init();
	/*
	 * Realloc with a proper allocator so that any cpumask update
	 * can indifferently free the old version with kfree().
	 */
	for_each_set_bit(type, &housekeeping.flags, HK_TYPE_MAX) {
		संरचना cpumask *omask, *nmask = kmalloc(cpumask_size(), GFP_KERNEL);

		जर (WARN_ON_ONCE(!nmask))
			परत;

		omask = rcu_dereference(housekeeping.cpumasks[type]);

		/* We need at least one CPU to handle housekeeping work */
		WARN_ON_ONCE(cpumask_empty(omask));
		cpumask_copy(nmask, omask);
		RCU_INIT_POINTER(housekeeping.cpumasks[type], nmask);
		__llist_add((संरचना llist_गाठ *)omask, &memblock_freelist);
	}
}

स्थिर पूर्णांक __init housekeeping_late_init(शून्य)
{
	संरचना llist_गाठ *llnode, *pos, *t;

	/* Free allocated memblock memory, if any */
	llnode = __llist_del_all(&memblock_freelist);
	llist_for_each_safe(pos, t, llnode)
		memblock_free(pos, cpumask_size());
	परत 0;
}
pure_initcall(housekeeping_late_init);

स्थिर शून्य __init housekeeping_setup_type(गणना hk_type type,
					   cpumask_var_t housekeeping_staging)
{
	संरचना cpumask *mask = memblock_alloc_or_panic(cpumask_size(), SMP_CACHE_BYTES);

	cpumask_copy(mask, housekeeping_staging);
	RCU_INIT_POINTER(housekeeping.cpumasks[type], mask);
}

स्थिर पूर्णांक __init housekeeping_setup(अक्षर *str, अचिन्हित दीर्घ flags)
{
	cpumask_var_t non_housekeeping_mask, housekeeping_staging;
	अचिन्हित पूर्णांक first_cpu;
	पूर्णांक err = 0;

	जर ((flags & HK_FLAG_KERNEL_NOISE) && !(housekeeping.flags & HK_FLAG_KERNEL_NOISE)) {
		जर (!IS_ENABLED(CONFIG_NO_HZ_FULL)) {
			pr_warn("Housekeeping: nohz unsupported."
				" Build with CONFIG_NO_HZ_FULL\n");
			परत 0;
		}
	}

	alloc_bootmem_cpumask_var(&non_housekeeping_mask);
	जर (cpulist_parse(str, non_housekeeping_mask) < 0) {
		pr_warn("Housekeeping: nohz_full= or isolcpus= incorrect CPU range\n");
		जा free_non_housekeeping_mask;
	}

	alloc_bootmem_cpumask_var(&housekeeping_staging);
	cpumask_andnot(housekeeping_staging,
		       cpu_possible_mask, non_housekeeping_mask);

	first_cpu = cpumask_first_and(cpu_present_mask, housekeeping_staging);
	जर (first_cpu >= nr_cpu_ids || first_cpu >= setup_max_cpus) {
		__cpumask_set_cpu(smp_processor_id(), housekeeping_staging);
		__cpumask_clear_cpu(smp_processor_id(), non_housekeeping_mask);
		जर (!housekeeping.flags) {
			pr_warn("Housekeeping: must include one present CPU, "
				"using boot CPU:%d\n", smp_processor_id());
		}
	}

	जर (cpumask_empty(non_housekeeping_mask))
		जा free_housekeeping_staging;

	जर (!housekeeping.flags) {
		/* First setup call ("nohz_full=" or "isolcpus=") */
		गणना hk_type type;

		for_each_set_bit(type, &flags, HK_TYPE_MAX)
			housekeeping_setup_type(type, housekeeping_staging);
	} अन्यथा {
		/* Second setup call ("nohz_full=" after "isolcpus=" or the reverse) */
		गणना hk_type type;
		अचिन्हित दीर्घ iter_flags = flags & housekeeping.flags;

		for_each_set_bit(type, &iter_flags, HK_TYPE_MAX) {
			जर (!cpumask_equal(housekeeping_staging,
					   housekeeping_cpumask(type))) {
				pr_warn("Housekeeping: nohz_full= must match isolcpus=\n");
				जा free_housekeeping_staging;
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
		जर (first_cpu >= min(nr_cpu_ids, setup_max_cpus)) {
			pr_warn("Housekeeping: must include one present CPU "
				"neither in nohz_full= nor in isolcpus=domain, "
				"ignoring setting %s\n", str);
			जा free_housekeeping_staging;
		}

		iter_flags = flags & ~housekeeping.flags;

		for_each_set_bit(type, &iter_flags, HK_TYPE_MAX)
			housekeeping_setup_type(type, housekeeping_staging);
	}

	जर ((flags & HK_FLAG_KERNEL_NOISE) && !(housekeeping.flags & HK_FLAG_KERNEL_NOISE))
		tick_nohz_full_setup(non_housekeeping_mask);

	housekeeping.flags |= flags;
	err = 1;

free_housekeeping_staging:
	free_bootmem_cpumask_var(housekeeping_staging);
free_non_housekeeping_mask:
	free_bootmem_cpumask_var(non_housekeeping_mask);

	परत err;
}

स्थिर पूर्णांक __init housekeeping_nohz_full_setup(अक्षर *str)
{
	अचिन्हित दीर्घ flags;

	flags = HK_FLAG_KERNEL_NOISE;

	परत housekeeping_setup(str, flags);
}
__setup("nohz_full=", housekeeping_nohz_full_setup);

स्थिर पूर्णांक __init housekeeping_isolcpus_setup(अक्षर *str)
{
	अचिन्हित दीर्घ flags = 0;
	bool illegal = false;
	अक्षर *par;
	पूर्णांक len;

	पर्यंत (है_अक्षर(*str)) {
		/*
		 * isolcpus=nohz is equivalent to nohz_full.
		 */
		जर (!म_भेदन(str, "nohz,", 5)) {
			str += 5;
			flags |= HK_FLAG_KERNEL_NOISE;
			सुरूठेवा;
		}

		जर (!म_भेदन(str, "domain,", 7)) {
			str += 7;
			flags |= HK_FLAG_DOMAIN | HK_FLAG_DOMAIN_BOOT;
			सुरूठेवा;
		}

		जर (!म_भेदन(str, "managed_irq,", 12)) {
			str += 12;
			flags |= HK_FLAG_MANAGED_IRQ;
			सुरूठेवा;
		}

		/*
		 * Skip unknown sub-parameter and validate that it is not
		 * containing an invalid character.
		 */
		साठी (par = str, len = 0; *str && *str != ','; str++, len++) {
			जर (!है_अक्षर(*str) && *str != '_')
				illegal = true;
		}

		जर (illegal) {
			pr_warn("isolcpus: Invalid flag %.*s\n", len, par);
			परत 0;
		}

		pr_info("isolcpus: Skipped unknown flag %.*s\n", len, par);
		str++;
	}

	/* Default behaviour for isolcpus without flags */
	जर (!flags)
		flags |= HK_FLAG_DOMAIN | HK_FLAG_DOMAIN_BOOT;

	परत housekeeping_setup(str, flags);
}
__setup("isolcpus=", housekeeping_isolcpus_setup);
