// SPDX-License-Identifier: GPL-2.0-only
/*
 *  Housekeeping management. Manage the targets for routine code that can run on
 *  any CPU: unbound workqueues, timers, kthreads and any offloadable work.
 *
 * Copyright (C) 2017 Red Hat, Inc., Frederic Weisbecker
 * Copyright (C) 2017-2018 SUSE, Frederic Weisbecker
 *
 */
#समावेश <linux/sched/isolation.h>
#समावेश <linux/llist.h>
#समावेश <linux/pci.h>
#समावेश "sched.h"

क्रमागत hk_flags {
	HK_FLAG_DOMAIN_BOOT	= BIT(HK_TYPE_DOMAIN_BOOT),
	HK_FLAG_DOMAIN		= BIT(HK_TYPE_DOMAIN),
	HK_FLAG_MANAGED_IRQ	= BIT(HK_TYPE_MANAGED_IRQ),
	HK_FLAG_KERNEL_NOISE	= BIT(HK_TYPE_KERNEL_NOISE),
};

DEFINE_STATIC_कुंजी_FALSE(housekeeping_overridden);
EXPORT_प्रतीक_GPL(housekeeping_overridden);

काष्ठा housekeeping {
	काष्ठा cpumask __rcu *cpumasks[HK_TYPE_MAX];
	अचिन्हित दीर्घ flags;
};

अटल काष्ठा housekeeping housekeeping;
अटल __initdata LLIST_शीर्ष(memblock_freelist);

bool housekeeping_enabled(क्रमागत hk_type type)
{
	वापस !!(READ_ONCE(housekeeping.flags) & BIT(type));
}
EXPORT_प्रतीक_GPL(housekeeping_enabled);

अटल bool housekeeping_dereference_check(क्रमागत hk_type type)
{
	अगर (IS_ENABLED(CONFIG_LOCKDEP) && type == HK_TYPE_DOMAIN) {
		/* Cpuset isn't even writable yet? */
		अगर (system_state <= SYSTEM_SCHEDULING)
			वापस true;

		/* CPU hotplug write locked, so cpuset partition can't be overwritten */
		अगर (IS_ENABLED(CONFIG_HOTPLUG_CPU) && lockdep_is_cpus_write_held())
			वापस true;

		/* Cpuset lock held, partitions not writable */
		अगर (IS_ENABLED(CONFIG_CPUSETS) && lockdep_is_cpuset_held())
			वापस true;

		वापस false;
	}

	वापस true;
}

अटल अंतरभूत काष्ठा cpumask *housekeeping_cpumask_dereference(क्रमागत hk_type type)
{
	वापस rcu_dereference_all_check(housekeeping.cpumasks[type],
					 housekeeping_dereference_check(type));
}

स्थिर काष्ठा cpumask *housekeeping_cpumask(क्रमागत hk_type type)
{
	स्थिर काष्ठा cpumask *mask = शून्य;

	अगर (static_branch_unlikely(&housekeeping_overridden)) {
		अगर (READ_ONCE(housekeeping.flags) & BIT(type))
			mask = housekeeping_cpumask_dereference(type);
	}
	अगर (!mask)
		mask = cpu_possible_mask;
	वापस mask;
}
EXPORT_प्रतीक_GPL(housekeeping_cpumask);

पूर्णांक housekeeping_any_cpu(क्रमागत hk_type type)
{
	पूर्णांक cpu;

	अगर (static_branch_unlikely(&housekeeping_overridden)) {
		अगर (housekeeping.flags & BIT(type)) {
			cpu = sched_numa_find_closest(housekeeping_cpumask(type), smp_processor_id());
			अगर (cpu < nr_cpu_ids)
				वापस cpu;

			cpu = cpumask_any_and_distribute(housekeeping_cpumask(type), cpu_online_mask);
			अगर (likely(cpu < nr_cpu_ids))
				वापस cpu;
			/*
			 * Unless we have another problem this can only happen
			 * at boot time before start_secondary() brings the 1st
			 * housekeeping CPU up.
			 */
			WARN_ON_ONCE(system_state == SYSTEM_RUNNING ||
				     type != HK_TYPE_TIMER);
		}
	}
	वापस smp_processor_id();
}
EXPORT_प्रतीक_GPL(housekeeping_any_cpu);

व्योम housekeeping_affine(काष्ठा task_struct *t, क्रमागत hk_type type)
{
	अगर (static_branch_unlikely(&housekeeping_overridden))
		अगर (housekeeping.flags & BIT(type))
			set_cpus_allowed_ptr(t, housekeeping_cpumask(type));
}
EXPORT_प्रतीक_GPL(housekeeping_affine);

bool housekeeping_test_cpu(पूर्णांक cpu, क्रमागत hk_type type)
{
	अगर (static_branch_unlikely(&housekeeping_overridden) &&
	    READ_ONCE(housekeeping.flags) & BIT(type))
		वापस cpumask_test_cpu(cpu, housekeeping_cpumask(type));
	वापस true;
}
EXPORT_प्रतीक_GPL(housekeeping_test_cpu);

पूर्णांक housekeeping_update(काष्ठा cpumask *isol_mask)
{
	काष्ठा cpumask *trial, *old = शून्य;
	पूर्णांक err;

	trial = kmalloc(cpumask_आकार(), GFP_KERNEL);
	अगर (!trial)
		वापस -ENOMEM;

	cpumask_andnot(trial, housekeeping_cpumask(HK_TYPE_DOMAIN_BOOT), isol_mask);
	अगर (!cpumask_intersects(trial, cpu_online_mask)) {
		kfree(trial);
		वापस -EINVAL;
	}

	अगर (!housekeeping.flags)
		static_branch_enable(&housekeeping_overridden);

	अगर (housekeeping.flags & HK_FLAG_DOMAIN)
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

	वापस 0;
}

व्योम __init housekeeping_init(व्योम)
{
	क्रमागत hk_type type;

	अगर (!housekeeping.flags)
		वापस;

	static_branch_enable(&housekeeping_overridden);

	अगर (housekeeping.flags & HK_FLAG_KERNEL_NOISE)
		sched_tick_offload_init();
	/*
	 * Realloc with a proper allocator so that any cpumask update
	 * can indifferently free the old version with kfree().
	 */
	for_each_set_bit(type, &housekeeping.flags, HK_TYPE_MAX) {
		काष्ठा cpumask *omask, *nmask = kmalloc(cpumask_आकार(), GFP_KERNEL);

		अगर (WARN_ON_ONCE(!nmask))
			वापस;

		omask = rcu_dereference(housekeeping.cpumasks[type]);

		/* We need at least one CPU to handle housekeeping work */
		WARN_ON_ONCE(cpumask_empty(omask));
		cpumask_copy(nmask, omask);
		RCU_INIT_POINTER(housekeeping.cpumasks[type], nmask);
		__llist_add((काष्ठा llist_गाँठ *)omask, &memblock_freelist);
	}
}

अटल पूर्णांक __init housekeeping_late_init(व्योम)
{
	काष्ठा llist_गाँठ *llnode, *pos, *t;

	/* Free allocated memblock memory, if any */
	llnode = __llist_del_all(&memblock_freelist);
	llist_for_each_safe(pos, t, llnode)
		memblock_free(pos, cpumask_आकार());
	वापस 0;
}
pure_initcall(housekeeping_late_init);

अटल व्योम __init housekeeping_setup_type(क्रमागत hk_type type,
					   cpumask_var_t housekeeping_staging)
{
	काष्ठा cpumask *mask = memblock_alloc_or_panic(cpumask_आकार(), SMP_CACHE_BYTES);

	cpumask_copy(mask, housekeeping_staging);
	RCU_INIT_POINTER(housekeeping.cpumasks[type], mask);
}

अटल पूर्णांक __init housekeeping_setup(अक्षर *str, अचिन्हित दीर्घ flags)
{
	cpumask_var_t non_housekeeping_mask, housekeeping_staging;
	अचिन्हित पूर्णांक first_cpu;
	पूर्णांक err = 0;

	अगर ((flags & HK_FLAG_KERNEL_NOISE) && !(housekeeping.flags & HK_FLAG_KERNEL_NOISE)) {
		अगर (!IS_ENABLED(CONFIG_NO_HZ_FULL)) {
			pr_warn("Housekeeping: nohz unsupported."
				" Build with CONFIG_NO_HZ_FULL\n");
			वापस 0;
		}
	}

	alloc_bootmem_cpumask_var(&non_housekeeping_mask);
	अगर (cpulist_parse(str, non_housekeeping_mask) < 0) {
		pr_warn("Housekeeping: nohz_full= or isolcpus= incorrect CPU range\n");
		जाओ free_non_housekeeping_mask;
	}

	alloc_bootmem_cpumask_var(&housekeeping_staging);
	cpumask_andnot(housekeeping_staging,
		       cpu_possible_mask, non_housekeeping_mask);

	first_cpu = cpumask_first_and(cpu_present_mask, housekeeping_staging);
	अगर (first_cpu >= nr_cpu_ids || first_cpu >= setup_max_cpus) {
		__cpumask_set_cpu(smp_processor_id(), housekeeping_staging);
		__cpumask_clear_cpu(smp_processor_id(), non_housekeeping_mask);
		अगर (!housekeeping.flags) {
			pr_warn("Housekeeping: must include one present CPU, "
				"using boot CPU:%d\n", smp_processor_id());
		}
	}

	अगर (cpumask_empty(non_housekeeping_mask))
		जाओ free_housekeeping_staging;

	अगर (!housekeeping.flags) {
		/* First setup call ("nohz_full=" or "isolcpus=") */
		क्रमागत hk_type type;

		for_each_set_bit(type, &flags, HK_TYPE_MAX)
			housekeeping_setup_type(type, housekeeping_staging);
	} अन्यथा {
		/* Second setup call ("nohz_full=" after "isolcpus=" or the reverse) */
		क्रमागत hk_type type;
		अचिन्हित दीर्घ iter_flags = flags & housekeeping.flags;

		for_each_set_bit(type, &iter_flags, HK_TYPE_MAX) {
			अगर (!cpumask_equal(housekeeping_staging,
					   housekeeping_cpumask(type))) {
				pr_warn("Housekeeping: nohz_full= must match isolcpus=\n");
				जाओ free_housekeeping_staging;
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
		अगर (first_cpu >= min(nr_cpu_ids, setup_max_cpus)) {
			pr_warn("Housekeeping: must include one present CPU "
				"neither in nohz_full= nor in isolcpus=domain, "
				"ignoring setting %s\n", str);
			जाओ free_housekeeping_staging;
		}

		iter_flags = flags & ~housekeeping.flags;

		for_each_set_bit(type, &iter_flags, HK_TYPE_MAX)
			housekeeping_setup_type(type, housekeeping_staging);
	}

	अगर ((flags & HK_FLAG_KERNEL_NOISE) && !(housekeeping.flags & HK_FLAG_KERNEL_NOISE))
		tick_nohz_full_setup(non_housekeeping_mask);

	housekeeping.flags |= flags;
	err = 1;

free_housekeeping_staging:
	free_bootmem_cpumask_var(housekeeping_staging);
free_non_housekeeping_mask:
	free_bootmem_cpumask_var(non_housekeeping_mask);

	वापस err;
}

अटल पूर्णांक __init housekeeping_nohz_full_setup(अक्षर *str)
{
	अचिन्हित दीर्घ flags;

	flags = HK_FLAG_KERNEL_NOISE;

	वापस housekeeping_setup(str, flags);
}
__setup("nohz_full=", housekeeping_nohz_full_setup);

अटल पूर्णांक __init housekeeping_isolcpus_setup(अक्षर *str)
{
	अचिन्हित दीर्घ flags = 0;
	bool illegal = false;
	अक्षर *par;
	पूर्णांक len;

	जबतक (है_अक्षर(*str)) {
		/*
		 * isolcpus=nohz is equivalent to nohz_full.
		 */
		अगर (!म_भेदन(str, "nohz,", 5)) {
			str += 5;
			flags |= HK_FLAG_KERNEL_NOISE;
			जारी;
		}

		अगर (!म_भेदन(str, "domain,", 7)) {
			str += 7;
			flags |= HK_FLAG_DOMAIN | HK_FLAG_DOMAIN_BOOT;
			जारी;
		}

		अगर (!म_भेदन(str, "managed_irq,", 12)) {
			str += 12;
			flags |= HK_FLAG_MANAGED_IRQ;
			जारी;
		}

		/*
		 * Skip unknown sub-parameter and validate that it is not
		 * containing an invalid character.
		 */
		क्रम (par = str, len = 0; *str && *str != ','; str++, len++) {
			अगर (!है_अक्षर(*str) && *str != '_')
				illegal = true;
		}

		अगर (illegal) {
			pr_warn("isolcpus: Invalid flag %.*s\n", len, par);
			वापस 0;
		}

		pr_info("isolcpus: Skipped unknown flag %.*s\n", len, par);
		str++;
	}

	/* Default behaviour for isolcpus without flags */
	अगर (!flags)
		flags |= HK_FLAG_DOMAIN | HK_FLAG_DOMAIN_BOOT;

	वापस housekeeping_setup(str, flags);
}
__setup("isolcpus=", housekeeping_isolcpus_setup);
