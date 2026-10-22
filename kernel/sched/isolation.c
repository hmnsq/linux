// SPDX-License-Identifier: GPL-2.0-only
/*
 *  Housekeeping management. Manage the targets for routine code that can run on
 *  any CPU: unbound workqueues, timers, kthreads and any offloadable work.
 *
 * Copyright (C) 2017 Red Hat, Inc., Frederic Weisbecker
 * Copyright (C) 2017-2018 SUSE, Frederic Weisbecker
 *
 */
#அந்தர்பாவ <linux/sched/isolation.h>
#அந்தர்பாவ <linux/llist.h>
#அந்தர்பாவ <linux/pci.h>
#அந்தர்பாவ "sched.h"

எண்ணிக்கை hk_flags {
	HK_FLAG_DOMAIN_BOOT	= BIT(HK_TYPE_DOMAIN_BOOT),
	HK_FLAG_DOMAIN		= BIT(HK_TYPE_DOMAIN),
	HK_FLAG_MANAGED_IRQ	= BIT(HK_TYPE_MANAGED_IRQ),
	HK_FLAG_KERNEL_NOISE	= BIT(HK_TYPE_KERNEL_NOISE),
};

DEFINE_STATIC_சாவி_FALSE(housekeeping_overridden);
EXPORT_குறியீடு_GPL(housekeeping_overridden);

கட்டமைப்பு housekeeping {
	கட்டமைப்பு cpumask __rcu *cpumasks[HK_TYPE_MAX];
	unsigned long flags;
};

நிலையான கட்டமைப்பு housekeeping housekeeping;
நிலையான __initdata LLIST_HEAD(memblock_freelist);

bool housekeeping_enabled(எண்ணிக்கை hk_type type)
{
	திரும்பு !!(READ_ONCE(housekeeping.flags) & BIT(type));
}
EXPORT_குறியீடு_GPL(housekeeping_enabled);

நிலையான bool housekeeping_dereference_check(எண்ணிக்கை hk_type type)
{
	எனில் (IS_ENABLED(CONFIG_LOCKDEP) && type == HK_TYPE_DOMAIN) {
		/* Cpuset isn't even writable yet? */
		எனில் (system_state <= SYSTEM_SCHEDULING)
			திரும்பு true;

		/* CPU hotplug write locked, so cpuset partition can't be overwritten */
		எனில் (IS_ENABLED(CONFIG_HOTPLUG_CPU) && lockdep_is_cpus_write_held())
			திரும்பு true;

		/* Cpuset lock held, partitions not writable */
		எனில் (IS_ENABLED(CONFIG_CPUSETS) && lockdep_is_cpuset_held())
			திரும்பு true;

		திரும்பு false;
	}

	திரும்பு true;
}

நிலையான உள்வரி கட்டமைப்பு cpumask *housekeeping_cpumask_dereference(எண்ணிக்கை hk_type type)
{
	திரும்பு rcu_dereference_all_check(housekeeping.cpumasks[type],
					 housekeeping_dereference_check(type));
}

மாறிலி கட்டமைப்பு cpumask *housekeeping_cpumask(எண்ணிக்கை hk_type type)
{
	மாறிலி கட்டமைப்பு cpumask *mask = வெற்று;

	எனில் (static_branch_unlikely(&housekeeping_overridden)) {
		எனில் (READ_ONCE(housekeeping.flags) & BIT(type))
			mask = housekeeping_cpumask_dereference(type);
	}
	எனில் (!mask)
		mask = cpu_possible_mask;
	திரும்பு mask;
}
EXPORT_குறியீடு_GPL(housekeeping_cpumask);

முழு housekeeping_any_cpu(எண்ணிக்கை hk_type type)
{
	முழு cpu;

	எனில் (static_branch_unlikely(&housekeeping_overridden)) {
		எனில் (housekeeping.flags & BIT(type)) {
			cpu = sched_numa_find_closest(housekeeping_cpumask(type), smp_processor_id());
			எனில் (cpu < nr_cpu_ids)
				திரும்பு cpu;

			cpu = cpumask_any_and_distribute(housekeeping_cpumask(type), cpu_online_mask);
			எனில் (likely(cpu < nr_cpu_ids))
				திரும்பு cpu;
			/*
			 * Unless we have another problem this can only happen
			 * at boot time before start_secondary() brings the 1st
			 * housekeeping CPU up.
			 */
			WARN_ON_ONCE(system_state == SYSTEM_RUNNING ||
				     type != HK_TYPE_TIMER);
		}
	}
	திரும்பு smp_processor_id();
}
EXPORT_குறியீடு_GPL(housekeeping_any_cpu);

வெறுமை housekeeping_affine(கட்டமைப்பு task_struct *t, எண்ணிக்கை hk_type type)
{
	எனில் (static_branch_unlikely(&housekeeping_overridden))
		எனில் (housekeeping.flags & BIT(type))
			set_cpus_allowed_ptr(t, housekeeping_cpumask(type));
}
EXPORT_குறியீடு_GPL(housekeeping_affine);

bool housekeeping_test_cpu(முழு cpu, எண்ணிக்கை hk_type type)
{
	எனில் (static_branch_unlikely(&housekeeping_overridden) &&
	    READ_ONCE(housekeeping.flags) & BIT(type))
		திரும்பு cpumask_test_cpu(cpu, housekeeping_cpumask(type));
	திரும்பு true;
}
EXPORT_குறியீடு_GPL(housekeeping_test_cpu);

முழு housekeeping_update(கட்டமைப்பு cpumask *isol_mask)
{
	கட்டமைப்பு cpumask *trial, *old = வெற்று;
	முழு err;

	trial = kmalloc(cpumask_size(), GFP_KERNEL);
	எனில் (!trial)
		திரும்பு -ENOMEM;

	cpumask_andnot(trial, housekeeping_cpumask(HK_TYPE_DOMAIN_BOOT), isol_mask);
	எனில் (!cpumask_intersects(trial, cpu_online_mask)) {
		kfree(trial);
		திரும்பு -EINVAL;
	}

	எனில் (!housekeeping.flags)
		static_branch_enable(&housekeeping_overridden);

	எனில் (housekeeping.flags & HK_FLAG_DOMAIN)
		old = housekeeping_cpumask_dereference(HK_TYPE_DOMAIN);
	இல்லையெனில்
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

	திரும்பு 0;
}

வெறுமை __init housekeeping_init(வெறுமை)
{
	எண்ணிக்கை hk_type type;

	எனில் (!housekeeping.flags)
		திரும்பு;

	static_branch_enable(&housekeeping_overridden);

	எனில் (housekeeping.flags & HK_FLAG_KERNEL_NOISE)
		sched_tick_offload_init();
	/*
	 * Realloc with a proper allocator so that any cpumask update
	 * can indifferently free the old version with kfree().
	 */
	for_each_set_bit(type, &housekeeping.flags, HK_TYPE_MAX) {
		கட்டமைப்பு cpumask *omask, *nmask = kmalloc(cpumask_size(), GFP_KERNEL);

		எனில் (WARN_ON_ONCE(!nmask))
			திரும்பு;

		omask = rcu_dereference(housekeeping.cpumasks[type]);

		/* We need at least one CPU to handle housekeeping work */
		WARN_ON_ONCE(cpumask_empty(omask));
		cpumask_copy(nmask, omask);
		RCU_INIT_POINTER(housekeeping.cpumasks[type], nmask);
		__llist_add((கட்டமைப்பு llist_முனை *)omask, &memblock_freelist);
	}
}

நிலையான முழு __init housekeeping_late_init(வெறுமை)
{
	கட்டமைப்பு llist_முனை *llnode, *pos, *t;

	/* Free allocated memblock memory, if any */
	llnode = __llist_del_all(&memblock_freelist);
	llist_for_each_safe(pos, t, llnode)
		memblock_free(pos, cpumask_size());
	திரும்பு 0;
}
pure_initcall(housekeeping_late_init);

நிலையான வெறுமை __init housekeeping_setup_type(எண்ணிக்கை hk_type type,
					   cpumask_var_t housekeeping_staging)
{
	கட்டமைப்பு cpumask *mask = memblock_alloc_or_panic(cpumask_size(), SMP_CACHE_BYTES);

	cpumask_copy(mask, housekeeping_staging);
	RCU_INIT_POINTER(housekeeping.cpumasks[type], mask);
}

நிலையான முழு __init housekeeping_setup(எழுத்து *str, unsigned long flags)
{
	cpumask_var_t non_housekeeping_mask, housekeeping_staging;
	unsigned முழு first_cpu;
	முழு err = 0;

	எனில் ((flags & HK_FLAG_KERNEL_NOISE) && !(housekeeping.flags & HK_FLAG_KERNEL_NOISE)) {
		எனில் (!IS_ENABLED(CONFIG_NO_HZ_FULL)) {
			pr_warn("Housekeeping: nohz unsupported."
				" Build with CONFIG_NO_HZ_FULL\n");
			திரும்பு 0;
		}
	}

	alloc_bootmem_cpumask_var(&non_housekeeping_mask);
	எனில் (cpulist_parse(str, non_housekeeping_mask) < 0) {
		pr_warn("Housekeeping: nohz_full= or isolcpus= incorrect CPU range\n");
		செல் free_non_housekeeping_mask;
	}

	alloc_bootmem_cpumask_var(&housekeeping_staging);
	cpumask_andnot(housekeeping_staging,
		       cpu_possible_mask, non_housekeeping_mask);

	first_cpu = cpumask_first_and(cpu_present_mask, housekeeping_staging);
	எனில் (first_cpu >= nr_cpu_ids || first_cpu >= setup_max_cpus) {
		__cpumask_set_cpu(smp_processor_id(), housekeeping_staging);
		__cpumask_clear_cpu(smp_processor_id(), non_housekeeping_mask);
		எனில் (!housekeeping.flags) {
			pr_warn("Housekeeping: must include one present CPU, "
				"using boot CPU:%d\n", smp_processor_id());
		}
	}

	எனில் (cpumask_empty(non_housekeeping_mask))
		செல் free_housekeeping_staging;

	எனில் (!housekeeping.flags) {
		/* First setup call ("nohz_full=" or "isolcpus=") */
		எண்ணிக்கை hk_type type;

		for_each_set_bit(type, &flags, HK_TYPE_MAX)
			housekeeping_setup_type(type, housekeeping_staging);
	} இல்லையெனில் {
		/* Second setup call ("nohz_full=" after "isolcpus=" or the reverse) */
		எண்ணிக்கை hk_type type;
		unsigned long iter_flags = flags & housekeeping.flags;

		for_each_set_bit(type, &iter_flags, HK_TYPE_MAX) {
			எனில் (!cpumask_equal(housekeeping_staging,
					   housekeeping_cpumask(type))) {
				pr_warn("Housekeeping: nohz_full= must match isolcpus=\n");
				செல் free_housekeeping_staging;
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
		எனில் (first_cpu >= min(nr_cpu_ids, setup_max_cpus)) {
			pr_warn("Housekeeping: must include one present CPU "
				"neither in nohz_full= nor in isolcpus=domain, "
				"ignoring setting %s\n", str);
			செல் free_housekeeping_staging;
		}

		iter_flags = flags & ~housekeeping.flags;

		for_each_set_bit(type, &iter_flags, HK_TYPE_MAX)
			housekeeping_setup_type(type, housekeeping_staging);
	}

	எனில் ((flags & HK_FLAG_KERNEL_NOISE) && !(housekeeping.flags & HK_FLAG_KERNEL_NOISE))
		tick_nohz_full_setup(non_housekeeping_mask);

	housekeeping.flags |= flags;
	err = 1;

free_housekeeping_staging:
	free_bootmem_cpumask_var(housekeeping_staging);
free_non_housekeeping_mask:
	free_bootmem_cpumask_var(non_housekeeping_mask);

	திரும்பு err;
}

நிலையான முழு __init housekeeping_nohz_full_setup(எழுத்து *str)
{
	unsigned long flags;

	flags = HK_FLAG_KERNEL_NOISE;

	திரும்பு housekeeping_setup(str, flags);
}
__setup("nohz_full=", housekeeping_nohz_full_setup);

நிலையான முழு __init housekeeping_isolcpus_setup(எழுத்து *str)
{
	unsigned long flags = 0;
	bool illegal = false;
	எழுத்து *par;
	முழு len;

	போது (isalpha(*str)) {
		/*
		 * isolcpus=nohz is equivalent to nohz_full.
		 */
		எனில் (!strncmp(str, "nohz,", 5)) {
			str += 5;
			flags |= HK_FLAG_KERNEL_NOISE;
			தொடர்;
		}

		எனில் (!strncmp(str, "domain,", 7)) {
			str += 7;
			flags |= HK_FLAG_DOMAIN | HK_FLAG_DOMAIN_BOOT;
			தொடர்;
		}

		எனில் (!strncmp(str, "managed_irq,", 12)) {
			str += 12;
			flags |= HK_FLAG_MANAGED_IRQ;
			தொடர்;
		}

		/*
		 * Skip unknown sub-parameter and validate that it is not
		 * containing an invalid character.
		 */
		க்கு (par = str, len = 0; *str && *str != ','; str++, len++) {
			எனில் (!isalpha(*str) && *str != '_')
				illegal = true;
		}

		எனில் (illegal) {
			pr_warn("isolcpus: Invalid flag %.*s\n", len, par);
			திரும்பு 0;
		}

		pr_info("isolcpus: Skipped unknown flag %.*s\n", len, par);
		str++;
	}

	/* Default behaviour for isolcpus without flags */
	எனில் (!flags)
		flags |= HK_FLAG_DOMAIN | HK_FLAG_DOMAIN_BOOT;

	திரும்பு housekeeping_setup(str, flags);
}
__setup("isolcpus=", housekeeping_isolcpus_setup);
