// SPDX-License-Identifier: GPL-2.0-only
/*
 *  Housekeeping management. Manage the targets for routine code that can run on
 *  any CPU: unbound workqueues, timers, kthreads and any offloadable work.
 *
 * Copyright (C) 2017 Red Hat, Inc., Frederic Weisbecker
 * Copyright (C) 2017-2018 SUSE, Frederic Weisbecker
 *
 */
#ଅନ୍ତର୍ଭାଵ <linux/sched/isolation.h>
#ଅନ୍ତର୍ଭାଵ <linux/llist.h>
#ଅନ୍ତର୍ଭାଵ <linux/pci.h>
#ଅନ୍ତର୍ଭାଵ "sched.h"

ଗଣନା hk_flags {
	HK_FLAG_DOMAIN_BOOT	= BIT(HK_TYPE_DOMAIN_BOOT),
	HK_FLAG_DOMAIN		= BIT(HK_TYPE_DOMAIN),
	HK_FLAG_MANAGED_IRQ	= BIT(HK_TYPE_MANAGED_IRQ),
	HK_FLAG_KERNEL_NOISE	= BIT(HK_TYPE_KERNEL_NOISE),
};

DEFINE_STATIC_ଚାବି_FALSE(housekeeping_overridden);
EXPORT_ପ୍ରତୀକ_GPL(housekeeping_overridden);

ସଂରଚନା housekeeping {
	ସଂରଚନା cpumask __rcu *cpumasks[HK_TYPE_MAX];
	unsigned long flags;
};

ସ୍ଥିର ସଂରଚନା housekeeping housekeeping;
ସ୍ଥିର __initdata LLIST_HEAD(memblock_freelist);

bool housekeeping_enabled(ଗଣନା hk_type type)
{
	ଫେରନ୍ତୁ !!(READ_ONCE(housekeeping.flags) & BIT(type));
}
EXPORT_ପ୍ରତୀକ_GPL(housekeeping_enabled);

ସ୍ଥିର bool housekeeping_dereference_check(ଗଣନା hk_type type)
{
	ଯଦି (IS_ENABLED(CONFIG_LOCKDEP) && type == HK_TYPE_DOMAIN) {
		/* Cpuset isn't even writable yet? */
		ଯଦି (system_state <= SYSTEM_SCHEDULING)
			ଫେରନ୍ତୁ true;

		/* CPU hotplug write locked, so cpuset partition can't be overwritten */
		ଯଦି (IS_ENABLED(CONFIG_HOTPLUG_CPU) && lockdep_is_cpus_write_held())
			ଫେରନ୍ତୁ true;

		/* Cpuset lock held, partitions not writable */
		ଯଦି (IS_ENABLED(CONFIG_CPUSETS) && lockdep_is_cpuset_held())
			ଫେରନ୍ତୁ true;

		ଫେରନ୍ତୁ false;
	}

	ଫେରନ୍ତୁ true;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା cpumask *housekeeping_cpumask_dereference(ଗଣନା hk_type type)
{
	ଫେରନ୍ତୁ rcu_dereference_all_check(housekeeping.cpumasks[type],
					 housekeeping_dereference_check(type));
}

ଅଚଳ ସଂରଚନା cpumask *housekeeping_cpumask(ଗଣନା hk_type type)
{
	ଅଚଳ ସଂରଚନା cpumask *mask = ଖାଲି;

	ଯଦି (static_branch_unlikely(&housekeeping_overridden)) {
		ଯଦି (READ_ONCE(housekeeping.flags) & BIT(type))
			mask = housekeeping_cpumask_dereference(type);
	}
	ଯଦି (!mask)
		mask = cpu_possible_mask;
	ଫେରନ୍ତୁ mask;
}
EXPORT_ପ୍ରତୀକ_GPL(housekeeping_cpumask);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା housekeeping_any_cpu(ଗଣନା hk_type type)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu;

	ଯଦି (static_branch_unlikely(&housekeeping_overridden)) {
		ଯଦି (housekeeping.flags & BIT(type)) {
			cpu = sched_numa_find_closest(housekeeping_cpumask(type), smp_processor_id());
			ଯଦି (cpu < nr_cpu_ids)
				ଫେରନ୍ତୁ cpu;

			cpu = cpumask_any_and_distribute(housekeeping_cpumask(type), cpu_online_mask);
			ଯଦି (likely(cpu < nr_cpu_ids))
				ଫେରନ୍ତୁ cpu;
			/*
			 * Unless we have another problem this can only happen
			 * at boot time before start_secondary() brings the 1st
			 * housekeeping CPU up.
			 */
			WARN_ON_ONCE(system_state == SYSTEM_RUNNING ||
				     type != HK_TYPE_TIMER);
		}
	}
	ଫେରନ୍ତୁ smp_processor_id();
}
EXPORT_ପ୍ରତୀକ_GPL(housekeeping_any_cpu);

ଶୂନ୍ୟ housekeeping_affine(ସଂରଚନା task_struct *t, ଗଣନା hk_type type)
{
	ଯଦି (static_branch_unlikely(&housekeeping_overridden))
		ଯଦି (housekeeping.flags & BIT(type))
			set_cpus_allowed_ptr(t, housekeeping_cpumask(type));
}
EXPORT_ପ୍ରତୀକ_GPL(housekeeping_affine);

bool housekeeping_test_cpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu, ଗଣନା hk_type type)
{
	ଯଦି (static_branch_unlikely(&housekeeping_overridden) &&
	    READ_ONCE(housekeeping.flags) & BIT(type))
		ଫେରନ୍ତୁ cpumask_test_cpu(cpu, housekeeping_cpumask(type));
	ଫେରନ୍ତୁ true;
}
EXPORT_ପ୍ରତୀକ_GPL(housekeeping_test_cpu);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା housekeeping_update(ସଂରଚନା cpumask *isol_mask)
{
	ସଂରଚନା cpumask *trial, *old = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା err;

	trial = kmalloc(cpumask_size(), GFP_KERNEL);
	ଯଦି (!trial)
		ଫେରନ୍ତୁ -ENOMEM;

	cpumask_andnot(trial, housekeeping_cpumask(HK_TYPE_DOMAIN_BOOT), isol_mask);
	ଯଦି (!cpumask_intersects(trial, cpu_online_mask)) {
		kfree(trial);
		ଫେରନ୍ତୁ -EINVAL;
	}

	ଯଦି (!housekeeping.flags)
		static_branch_enable(&housekeeping_overridden);

	ଯଦି (housekeeping.flags & HK_FLAG_DOMAIN)
		old = housekeeping_cpumask_dereference(HK_TYPE_DOMAIN);
	ଅନ୍ୟଥା
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

	ଫେରନ୍ତୁ 0;
}

ଶୂନ୍ୟ __init housekeeping_init(ଶୂନ୍ୟ)
{
	ଗଣନା hk_type type;

	ଯଦି (!housekeeping.flags)
		ଫେରନ୍ତୁ;

	static_branch_enable(&housekeeping_overridden);

	ଯଦି (housekeeping.flags & HK_FLAG_KERNEL_NOISE)
		sched_tick_offload_init();
	/*
	 * Realloc with a proper allocator so that any cpumask update
	 * can indifferently free the old version with kfree().
	 */
	for_each_set_bit(type, &housekeeping.flags, HK_TYPE_MAX) {
		ସଂରଚନା cpumask *omask, *nmask = kmalloc(cpumask_size(), GFP_KERNEL);

		ଯଦି (WARN_ON_ONCE(!nmask))
			ଫେରନ୍ତୁ;

		omask = rcu_dereference(housekeeping.cpumasks[type]);

		/* We need at least one CPU to handle housekeeping work */
		WARN_ON_ONCE(cpumask_empty(omask));
		cpumask_copy(nmask, omask);
		RCU_INIT_POINTER(housekeeping.cpumasks[type], nmask);
		__llist_add((ସଂରଚନା llist_ଗଣ୍ଠି *)omask, &memblock_freelist);
	}
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା __init housekeeping_late_init(ଶୂନ୍ୟ)
{
	ସଂରଚନା llist_ଗଣ୍ଠି *llnode, *pos, *t;

	/* Free allocated memblock memory, if any */
	llnode = __llist_del_all(&memblock_freelist);
	llist_for_each_safe(pos, t, llnode)
		memblock_free(pos, cpumask_size());
	ଫେରନ୍ତୁ 0;
}
pure_initcall(housekeeping_late_init);

ସ୍ଥିର ଶୂନ୍ୟ __init housekeeping_setup_type(ଗଣନା hk_type type,
					   cpumask_var_t housekeeping_staging)
{
	ସଂରଚନା cpumask *mask = memblock_alloc_or_panic(cpumask_size(), SMP_CACHE_BYTES);

	cpumask_copy(mask, housekeeping_staging);
	RCU_INIT_POINTER(housekeeping.cpumasks[type], mask);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା __init housekeeping_setup(ଅକ୍ଷର *str, unsigned long flags)
{
	cpumask_var_t non_housekeeping_mask, housekeeping_staging;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା first_cpu;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା err = 0;

	ଯଦି ((flags & HK_FLAG_KERNEL_NOISE) && !(housekeeping.flags & HK_FLAG_KERNEL_NOISE)) {
		ଯଦି (!IS_ENABLED(CONFIG_NO_HZ_FULL)) {
			pr_warn("Housekeeping: nohz unsupported."
				" Build with CONFIG_NO_HZ_FULL\n");
			ଫେରନ୍ତୁ 0;
		}
	}

	alloc_bootmem_cpumask_var(&non_housekeeping_mask);
	ଯଦି (cpulist_parse(str, non_housekeeping_mask) < 0) {
		pr_warn("Housekeeping: nohz_full= or isolcpus= incorrect CPU range\n");
		ଯାଅ free_non_housekeeping_mask;
	}

	alloc_bootmem_cpumask_var(&housekeeping_staging);
	cpumask_andnot(housekeeping_staging,
		       cpu_possible_mask, non_housekeeping_mask);

	first_cpu = cpumask_first_and(cpu_present_mask, housekeeping_staging);
	ଯଦି (first_cpu >= nr_cpu_ids || first_cpu >= setup_max_cpus) {
		__cpumask_set_cpu(smp_processor_id(), housekeeping_staging);
		__cpumask_clear_cpu(smp_processor_id(), non_housekeeping_mask);
		ଯଦି (!housekeeping.flags) {
			pr_warn("Housekeeping: must include one present CPU, "
				"using boot CPU:%d\n", smp_processor_id());
		}
	}

	ଯଦି (cpumask_empty(non_housekeeping_mask))
		ଯାଅ free_housekeeping_staging;

	ଯଦି (!housekeeping.flags) {
		/* First setup call ("nohz_full=" or "isolcpus=") */
		ଗଣନା hk_type type;

		for_each_set_bit(type, &flags, HK_TYPE_MAX)
			housekeeping_setup_type(type, housekeeping_staging);
	} ଅନ୍ୟଥା {
		/* Second setup call ("nohz_full=" after "isolcpus=" or the reverse) */
		ଗଣନା hk_type type;
		unsigned long iter_flags = flags & housekeeping.flags;

		for_each_set_bit(type, &iter_flags, HK_TYPE_MAX) {
			ଯଦି (!cpumask_equal(housekeeping_staging,
					   housekeeping_cpumask(type))) {
				pr_warn("Housekeeping: nohz_full= must match isolcpus=\n");
				ଯାଅ free_housekeeping_staging;
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
		ଯଦି (first_cpu >= min(nr_cpu_ids, setup_max_cpus)) {
			pr_warn("Housekeeping: must include one present CPU "
				"neither in nohz_full= nor in isolcpus=domain, "
				"ignoring setting %s\n", str);
			ଯାଅ free_housekeeping_staging;
		}

		iter_flags = flags & ~housekeeping.flags;

		for_each_set_bit(type, &iter_flags, HK_TYPE_MAX)
			housekeeping_setup_type(type, housekeeping_staging);
	}

	ଯଦି ((flags & HK_FLAG_KERNEL_NOISE) && !(housekeeping.flags & HK_FLAG_KERNEL_NOISE))
		tick_nohz_full_setup(non_housekeeping_mask);

	housekeeping.flags |= flags;
	err = 1;

free_housekeeping_staging:
	free_bootmem_cpumask_var(housekeeping_staging);
free_non_housekeeping_mask:
	free_bootmem_cpumask_var(non_housekeeping_mask);

	ଫେରନ୍ତୁ err;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା __init housekeeping_nohz_full_setup(ଅକ୍ଷର *str)
{
	unsigned long flags;

	flags = HK_FLAG_KERNEL_NOISE;

	ଫେରନ୍ତୁ housekeeping_setup(str, flags);
}
__setup("nohz_full=", housekeeping_nohz_full_setup);

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା __init housekeeping_isolcpus_setup(ଅକ୍ଷର *str)
{
	unsigned long flags = 0;
	bool illegal = false;
	ଅକ୍ଷର *par;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା len;

	ଯେତେବେଳେ (isalpha(*str)) {
		/*
		 * isolcpus=nohz is equivalent to nohz_full.
		 */
		ଯଦି (!strncmp(str, "nohz,", 5)) {
			str += 5;
			flags |= HK_FLAG_KERNEL_NOISE;
			ଜାରି;
		}

		ଯଦି (!strncmp(str, "domain,", 7)) {
			str += 7;
			flags |= HK_FLAG_DOMAIN | HK_FLAG_DOMAIN_BOOT;
			ଜାରି;
		}

		ଯଦି (!strncmp(str, "managed_irq,", 12)) {
			str += 12;
			flags |= HK_FLAG_MANAGED_IRQ;
			ଜାରି;
		}

		/*
		 * Skip unknown sub-parameter and validate that it is not
		 * containing an invalid character.
		 */
		ପାଇଁ (par = str, len = 0; *str && *str != ','; str++, len++) {
			ଯଦି (!isalpha(*str) && *str != '_')
				illegal = true;
		}

		ଯଦି (illegal) {
			pr_warn("isolcpus: Invalid flag %.*s\n", len, par);
			ଫେରନ୍ତୁ 0;
		}

		pr_info("isolcpus: Skipped unknown flag %.*s\n", len, par);
		str++;
	}

	/* Default behaviour for isolcpus without flags */
	ଯଦି (!flags)
		flags |= HK_FLAG_DOMAIN | HK_FLAG_DOMAIN_BOOT;

	ଫେରନ୍ତୁ housekeeping_setup(str, flags);
}
__setup("isolcpus=", housekeeping_isolcpus_setup);
