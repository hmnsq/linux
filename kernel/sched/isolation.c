// SPDX-License-Identifier: GPL-2.0-only
/*
 *  Housekeeping management. Manage the targets for routine code that can run on
 *  any CPU: unbound workqueues, timers, kthreads and any offloadable work.
 *
 * Copyright (C) 2017 Red Hat, Inc., Frederic Weisbecker
 * Copyright (C) 2017-2018 SUSE, Frederic Weisbecker
 *
 */
#અન્તર્ભાવ <linux/sched/isolation.h>
#અન્તર્ભાવ <linux/llist.h>
#અન્તર્ભાવ <linux/pci.h>
#અન્તર્ભાવ "sched.h"

ગણના hk_flags {
	HK_FLAG_DOMAIN_BOOT	= BIT(HK_TYPE_DOMAIN_BOOT),
	HK_FLAG_DOMAIN		= BIT(HK_TYPE_DOMAIN),
	HK_FLAG_MANAGED_IRQ	= BIT(HK_TYPE_MANAGED_IRQ),
	HK_FLAG_KERNEL_NOISE	= BIT(HK_TYPE_KERNEL_NOISE),
};

DEFINE_STATIC_ચાવી_FALSE(housekeeping_overridden);
EXPORT_પ્રતીક_GPL(housekeeping_overridden);

સંરચના housekeeping {
	સંરચના cpumask __rcu *cpumasks[HK_TYPE_MAX];
	unsigned long flags;
};

સ્થિર સંરચના housekeeping housekeeping;
સ્થિર __initdata LLIST_HEAD(memblock_freelist);

bool housekeeping_enabled(ગણના hk_type type)
{
	પાછા !!(READ_ONCE(housekeeping.flags) & BIT(type));
}
EXPORT_પ્રતીક_GPL(housekeeping_enabled);

સ્થિર bool housekeeping_dereference_check(ગણના hk_type type)
{
	જો (IS_ENABLED(CONFIG_LOCKDEP) && type == HK_TYPE_DOMAIN) {
		/* Cpuset isn't even writable yet? */
		જો (system_state <= SYSTEM_SCHEDULING)
			પાછા true;

		/* CPU hotplug write locked, so cpuset partition can't be overwritten */
		જો (IS_ENABLED(CONFIG_HOTPLUG_CPU) && lockdep_is_cpus_write_held())
			પાછા true;

		/* Cpuset lock held, partitions not writable */
		જો (IS_ENABLED(CONFIG_CPUSETS) && lockdep_is_cpuset_held())
			પાછા true;

		પાછા false;
	}

	પાછા true;
}

સ્થિર અંતઃરેખ સંરચના cpumask *housekeeping_cpumask_dereference(ગણના hk_type type)
{
	પાછા rcu_dereference_all_check(housekeeping.cpumasks[type],
					 housekeeping_dereference_check(type));
}

અચલ સંરચના cpumask *housekeeping_cpumask(ગણના hk_type type)
{
	અચલ સંરચના cpumask *mask = ખાલી;

	જો (static_branch_unlikely(&housekeeping_overridden)) {
		જો (READ_ONCE(housekeeping.flags) & BIT(type))
			mask = housekeeping_cpumask_dereference(type);
	}
	જો (!mask)
		mask = cpu_possible_mask;
	પાછા mask;
}
EXPORT_પ્રતીક_GPL(housekeeping_cpumask);

પૂર્ણાંક housekeeping_any_cpu(ગણના hk_type type)
{
	પૂર્ણાંક cpu;

	જો (static_branch_unlikely(&housekeeping_overridden)) {
		જો (housekeeping.flags & BIT(type)) {
			cpu = sched_numa_find_closest(housekeeping_cpumask(type), smp_processor_id());
			જો (cpu < nr_cpu_ids)
				પાછા cpu;

			cpu = cpumask_any_and_distribute(housekeeping_cpumask(type), cpu_online_mask);
			જો (likely(cpu < nr_cpu_ids))
				પાછા cpu;
			/*
			 * Unless we have another problem this can only happen
			 * at boot time before start_secondary() brings the 1st
			 * housekeeping CPU up.
			 */
			WARN_ON_ONCE(system_state == SYSTEM_RUNNING ||
				     type != HK_TYPE_TIMER);
		}
	}
	પાછા smp_processor_id();
}
EXPORT_પ્રતીક_GPL(housekeeping_any_cpu);

શૂન્ય housekeeping_affine(સંરચના task_struct *t, ગણના hk_type type)
{
	જો (static_branch_unlikely(&housekeeping_overridden))
		જો (housekeeping.flags & BIT(type))
			set_cpus_allowed_ptr(t, housekeeping_cpumask(type));
}
EXPORT_પ્રતીક_GPL(housekeeping_affine);

bool housekeeping_test_cpu(પૂર્ણાંક cpu, ગણના hk_type type)
{
	જો (static_branch_unlikely(&housekeeping_overridden) &&
	    READ_ONCE(housekeeping.flags) & BIT(type))
		પાછા cpumask_test_cpu(cpu, housekeeping_cpumask(type));
	પાછા true;
}
EXPORT_પ્રતીક_GPL(housekeeping_test_cpu);

પૂર્ણાંક housekeeping_update(સંરચના cpumask *isol_mask)
{
	સંરચના cpumask *trial, *old = ખાલી;
	પૂર્ણાંક err;

	trial = kmalloc(cpumask_size(), GFP_KERNEL);
	જો (!trial)
		પાછા -ENOMEM;

	cpumask_andnot(trial, housekeeping_cpumask(HK_TYPE_DOMAIN_BOOT), isol_mask);
	જો (!cpumask_intersects(trial, cpu_online_mask)) {
		kfree(trial);
		પાછા -EINVAL;
	}

	જો (!housekeeping.flags)
		static_branch_enable(&housekeeping_overridden);

	જો (housekeeping.flags & HK_FLAG_DOMAIN)
		old = housekeeping_cpumask_dereference(HK_TYPE_DOMAIN);
	નહીંતર
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

	પાછા 0;
}

શૂન્ય __init housekeeping_init(શૂન્ય)
{
	ગણના hk_type type;

	જો (!housekeeping.flags)
		પાછા;

	static_branch_enable(&housekeeping_overridden);

	જો (housekeeping.flags & HK_FLAG_KERNEL_NOISE)
		sched_tick_offload_init();
	/*
	 * Realloc with a proper allocator so that any cpumask update
	 * can indifferently free the old version with kfree().
	 */
	for_each_set_bit(type, &housekeeping.flags, HK_TYPE_MAX) {
		સંરચના cpumask *omask, *nmask = kmalloc(cpumask_size(), GFP_KERNEL);

		જો (WARN_ON_ONCE(!nmask))
			પાછા;

		omask = rcu_dereference(housekeeping.cpumasks[type]);

		/* We need at least one CPU to handle housekeeping work */
		WARN_ON_ONCE(cpumask_empty(omask));
		cpumask_copy(nmask, omask);
		RCU_INIT_POINTER(housekeeping.cpumasks[type], nmask);
		__llist_add((સંરચના llist_ગાંઠ *)omask, &memblock_freelist);
	}
}

સ્થિર પૂર્ણાંક __init housekeeping_late_init(શૂન્ય)
{
	સંરચના llist_ગાંઠ *llnode, *pos, *t;

	/* Free allocated memblock memory, if any */
	llnode = __llist_del_all(&memblock_freelist);
	llist_for_each_safe(pos, t, llnode)
		memblock_free(pos, cpumask_size());
	પાછા 0;
}
pure_initcall(housekeeping_late_init);

સ્થિર શૂન્ય __init housekeeping_setup_type(ગણના hk_type type,
					   cpumask_var_t housekeeping_staging)
{
	સંરચના cpumask *mask = memblock_alloc_or_panic(cpumask_size(), SMP_CACHE_BYTES);

	cpumask_copy(mask, housekeeping_staging);
	RCU_INIT_POINTER(housekeeping.cpumasks[type], mask);
}

સ્થિર પૂર્ણાંક __init housekeeping_setup(અક્ષર *str, unsigned long flags)
{
	cpumask_var_t non_housekeeping_mask, housekeeping_staging;
	unsigned પૂર્ણાંક first_cpu;
	પૂર્ણાંક err = 0;

	જો ((flags & HK_FLAG_KERNEL_NOISE) && !(housekeeping.flags & HK_FLAG_KERNEL_NOISE)) {
		જો (!IS_ENABLED(CONFIG_NO_HZ_FULL)) {
			pr_warn("Housekeeping: nohz unsupported."
				" Build with CONFIG_NO_HZ_FULL\n");
			પાછા 0;
		}
	}

	alloc_bootmem_cpumask_var(&non_housekeeping_mask);
	જો (cpulist_parse(str, non_housekeeping_mask) < 0) {
		pr_warn("Housekeeping: nohz_full= or isolcpus= incorrect CPU range\n");
		જાઓ free_non_housekeeping_mask;
	}

	alloc_bootmem_cpumask_var(&housekeeping_staging);
	cpumask_andnot(housekeeping_staging,
		       cpu_possible_mask, non_housekeeping_mask);

	first_cpu = cpumask_first_and(cpu_present_mask, housekeeping_staging);
	જો (first_cpu >= nr_cpu_ids || first_cpu >= setup_max_cpus) {
		__cpumask_set_cpu(smp_processor_id(), housekeeping_staging);
		__cpumask_clear_cpu(smp_processor_id(), non_housekeeping_mask);
		જો (!housekeeping.flags) {
			pr_warn("Housekeeping: must include one present CPU, "
				"using boot CPU:%d\n", smp_processor_id());
		}
	}

	જો (cpumask_empty(non_housekeeping_mask))
		જાઓ free_housekeeping_staging;

	જો (!housekeeping.flags) {
		/* First setup call ("nohz_full=" or "isolcpus=") */
		ગણના hk_type type;

		for_each_set_bit(type, &flags, HK_TYPE_MAX)
			housekeeping_setup_type(type, housekeeping_staging);
	} નહીંતર {
		/* Second setup call ("nohz_full=" after "isolcpus=" or the reverse) */
		ગણના hk_type type;
		unsigned long iter_flags = flags & housekeeping.flags;

		for_each_set_bit(type, &iter_flags, HK_TYPE_MAX) {
			જો (!cpumask_equal(housekeeping_staging,
					   housekeeping_cpumask(type))) {
				pr_warn("Housekeeping: nohz_full= must match isolcpus=\n");
				જાઓ free_housekeeping_staging;
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
		જો (first_cpu >= min(nr_cpu_ids, setup_max_cpus)) {
			pr_warn("Housekeeping: must include one present CPU "
				"neither in nohz_full= nor in isolcpus=domain, "
				"ignoring setting %s\n", str);
			જાઓ free_housekeeping_staging;
		}

		iter_flags = flags & ~housekeeping.flags;

		for_each_set_bit(type, &iter_flags, HK_TYPE_MAX)
			housekeeping_setup_type(type, housekeeping_staging);
	}

	જો ((flags & HK_FLAG_KERNEL_NOISE) && !(housekeeping.flags & HK_FLAG_KERNEL_NOISE))
		tick_nohz_full_setup(non_housekeeping_mask);

	housekeeping.flags |= flags;
	err = 1;

free_housekeeping_staging:
	free_bootmem_cpumask_var(housekeeping_staging);
free_non_housekeeping_mask:
	free_bootmem_cpumask_var(non_housekeeping_mask);

	પાછા err;
}

સ્થિર પૂર્ણાંક __init housekeeping_nohz_full_setup(અક્ષર *str)
{
	unsigned long flags;

	flags = HK_FLAG_KERNEL_NOISE;

	પાછા housekeeping_setup(str, flags);
}
__setup("nohz_full=", housekeeping_nohz_full_setup);

સ્થિર પૂર્ણાંક __init housekeeping_isolcpus_setup(અક્ષર *str)
{
	unsigned long flags = 0;
	bool illegal = false;
	અક્ષર *par;
	પૂર્ણાંક len;

	જ્યારે (isalpha(*str)) {
		/*
		 * isolcpus=nohz is equivalent to nohz_full.
		 */
		જો (!strncmp(str, "nohz,", 5)) {
			str += 5;
			flags |= HK_FLAG_KERNEL_NOISE;
			ચાલુ;
		}

		જો (!strncmp(str, "domain,", 7)) {
			str += 7;
			flags |= HK_FLAG_DOMAIN | HK_FLAG_DOMAIN_BOOT;
			ચાલુ;
		}

		જો (!strncmp(str, "managed_irq,", 12)) {
			str += 12;
			flags |= HK_FLAG_MANAGED_IRQ;
			ચાલુ;
		}

		/*
		 * Skip unknown sub-parameter and validate that it is not
		 * containing an invalid character.
		 */
		માટે (par = str, len = 0; *str && *str != ','; str++, len++) {
			જો (!isalpha(*str) && *str != '_')
				illegal = true;
		}

		જો (illegal) {
			pr_warn("isolcpus: Invalid flag %.*s\n", len, par);
			પાછા 0;
		}

		pr_info("isolcpus: Skipped unknown flag %.*s\n", len, par);
		str++;
	}

	/* Default behaviour for isolcpus without flags */
	જો (!flags)
		flags |= HK_FLAG_DOMAIN | HK_FLAG_DOMAIN_BOOT;

	પાછા housekeeping_setup(str, flags);
}
__setup("isolcpus=", housekeeping_isolcpus_setup);
