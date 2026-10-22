// SPDX-License-Identifier: GPL-2.0-only
/*
 *  Housekeeping management. Manage the targets for routine code that can run on
 *  any CPU: unbound workqueues, timers, kthreads and any offloadable work.
 *
 * Copyright (C) 2017 Red Hat, Inc., Frederic Weisbecker
 * Copyright (C) 2017-2018 SUSE, Frederic Weisbecker
 *
 */
#ضمن <linux/sched/isolation.h>
#ضمن <linux/llist.h>
#ضمن <linux/pci.h>
#ضمن "sched.h"

تعداد hk_flags {
	HK_FLAG_DOMAIN_BOOT	= BIT(HK_TYPE_DOMAIN_BOOT),
	HK_FLAG_DOMAIN		= BIT(HK_TYPE_DOMAIN),
	HK_FLAG_MANAGED_IRQ	= BIT(HK_TYPE_MANAGED_IRQ),
	HK_FLAG_KERNEL_NOISE	= BIT(HK_TYPE_KERNEL_NOISE),
};

DEFINE_STATIC_مفتاح_FALSE(housekeeping_overridden);
EXPORT_رمز_GPL(housekeeping_overridden);

بنية housekeeping {
	بنية cpumask __rcu *cpumasks[HK_TYPE_MAX];
	unsigned long flags;
};

ساكن بنية housekeeping housekeeping;
ساكن __initdata LLIST_HEAD(memblock_freelist);

bool housekeeping_enabled(تعداد hk_type type)
{
	أرجع !!(READ_ONCE(housekeeping.flags) & BIT(type));
}
EXPORT_رمز_GPL(housekeeping_enabled);

ساكن bool housekeeping_dereference_check(تعداد hk_type type)
{
	إذا (IS_ENABLED(CONFIG_LOCKDEP) && type == HK_TYPE_DOMAIN) {
		/* Cpuset isn't even writable yet? */
		إذا (system_state <= SYSTEM_SCHEDULING)
			أرجع true;

		/* CPU hotplug write locked, so cpuset partition can't be overwritten */
		إذا (IS_ENABLED(CONFIG_HOTPLUG_CPU) && lockdep_is_cpus_write_held())
			أرجع true;

		/* Cpuset lock held, partitions not writable */
		إذا (IS_ENABLED(CONFIG_CPUSETS) && lockdep_is_cpuset_held())
			أرجع true;

		أرجع false;
	}

	أرجع true;
}

ساكن مضمن بنية cpumask *housekeeping_cpumask_dereference(تعداد hk_type type)
{
	أرجع rcu_dereference_all_check(housekeeping.cpumasks[type],
					 housekeeping_dereference_check(type));
}

ثابت بنية cpumask *housekeeping_cpumask(تعداد hk_type type)
{
	ثابت بنية cpumask *mask = عدم;

	إذا (static_branch_unlikely(&housekeeping_overridden)) {
		إذا (READ_ONCE(housekeeping.flags) & BIT(type))
			mask = housekeeping_cpumask_dereference(type);
	}
	إذا (!mask)
		mask = cpu_possible_mask;
	أرجع mask;
}
EXPORT_رمز_GPL(housekeeping_cpumask);

صحيح housekeeping_any_cpu(تعداد hk_type type)
{
	صحيح cpu;

	إذا (static_branch_unlikely(&housekeeping_overridden)) {
		إذا (housekeeping.flags & BIT(type)) {
			cpu = sched_numa_find_closest(housekeeping_cpumask(type), smp_processor_id());
			إذا (cpu < nr_cpu_ids)
				أرجع cpu;

			cpu = cpumask_any_and_distribute(housekeeping_cpumask(type), cpu_online_mask);
			إذا (likely(cpu < nr_cpu_ids))
				أرجع cpu;
			/*
			 * Unless we have another problem this can only happen
			 * at boot time before start_secondary() brings the 1st
			 * housekeeping CPU up.
			 */
			WARN_ON_ONCE(system_state == SYSTEM_RUNNING ||
				     type != HK_TYPE_TIMER);
		}
	}
	أرجع smp_processor_id();
}
EXPORT_رمز_GPL(housekeeping_any_cpu);

فراغ housekeeping_affine(بنية task_struct *t, تعداد hk_type type)
{
	إذا (static_branch_unlikely(&housekeeping_overridden))
		إذا (housekeeping.flags & BIT(type))
			set_cpus_allowed_ptr(t, housekeeping_cpumask(type));
}
EXPORT_رمز_GPL(housekeeping_affine);

bool housekeeping_test_cpu(صحيح cpu, تعداد hk_type type)
{
	إذا (static_branch_unlikely(&housekeeping_overridden) &&
	    READ_ONCE(housekeeping.flags) & BIT(type))
		أرجع cpumask_test_cpu(cpu, housekeeping_cpumask(type));
	أرجع true;
}
EXPORT_رمز_GPL(housekeeping_test_cpu);

صحيح housekeeping_update(بنية cpumask *isol_mask)
{
	بنية cpumask *trial, *old = عدم;
	صحيح err;

	trial = kmalloc(cpumask_size(), GFP_KERNEL);
	إذا (!trial)
		أرجع -ENOMEM;

	cpumask_andnot(trial, housekeeping_cpumask(HK_TYPE_DOMAIN_BOOT), isol_mask);
	إذا (!cpumask_intersects(trial, cpu_online_mask)) {
		kfree(trial);
		أرجع -EINVAL;
	}

	إذا (!housekeeping.flags)
		static_branch_enable(&housekeeping_overridden);

	إذا (housekeeping.flags & HK_FLAG_DOMAIN)
		old = housekeeping_cpumask_dereference(HK_TYPE_DOMAIN);
	وإلا
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

	أرجع 0;
}

فراغ __init housekeeping_init(فراغ)
{
	تعداد hk_type type;

	إذا (!housekeeping.flags)
		أرجع;

	static_branch_enable(&housekeeping_overridden);

	إذا (housekeeping.flags & HK_FLAG_KERNEL_NOISE)
		sched_tick_offload_init();
	/*
	 * Realloc with a proper allocator so that any cpumask update
	 * can indifferently free the old version with kfree().
	 */
	for_each_set_bit(type, &housekeeping.flags, HK_TYPE_MAX) {
		بنية cpumask *omask, *nmask = kmalloc(cpumask_size(), GFP_KERNEL);

		إذا (WARN_ON_ONCE(!nmask))
			أرجع;

		omask = rcu_dereference(housekeeping.cpumasks[type]);

		/* We need at least one CPU to handle housekeeping work */
		WARN_ON_ONCE(cpumask_empty(omask));
		cpumask_copy(nmask, omask);
		RCU_INIT_POINTER(housekeeping.cpumasks[type], nmask);
		__llist_add((بنية llist_عقدة *)omask, &memblock_freelist);
	}
}

ساكن صحيح __init housekeeping_late_init(فراغ)
{
	بنية llist_عقدة *llnode, *pos, *t;

	/* Free allocated memblock memory, if any */
	llnode = __llist_del_all(&memblock_freelist);
	llist_for_each_safe(pos, t, llnode)
		memblock_free(pos, cpumask_size());
	أرجع 0;
}
pure_initcall(housekeeping_late_init);

ساكن فراغ __init housekeeping_setup_type(تعداد hk_type type,
					   cpumask_var_t housekeeping_staging)
{
	بنية cpumask *mask = memblock_alloc_or_panic(cpumask_size(), SMP_CACHE_BYTES);

	cpumask_copy(mask, housekeeping_staging);
	RCU_INIT_POINTER(housekeeping.cpumasks[type], mask);
}

ساكن صحيح __init housekeeping_setup(حرف *str, unsigned long flags)
{
	cpumask_var_t non_housekeeping_mask, housekeeping_staging;
	unsigned صحيح first_cpu;
	صحيح err = 0;

	إذا ((flags & HK_FLAG_KERNEL_NOISE) && !(housekeeping.flags & HK_FLAG_KERNEL_NOISE)) {
		إذا (!IS_ENABLED(CONFIG_NO_HZ_FULL)) {
			pr_warn("Housekeeping: nohz unsupported."
				" Build with CONFIG_NO_HZ_FULL\n");
			أرجع 0;
		}
	}

	alloc_bootmem_cpumask_var(&non_housekeeping_mask);
	إذا (cpulist_parse(str, non_housekeeping_mask) < 0) {
		pr_warn("Housekeeping: nohz_full= or isolcpus= incorrect CPU range\n");
		اذهب free_non_housekeeping_mask;
	}

	alloc_bootmem_cpumask_var(&housekeeping_staging);
	cpumask_andnot(housekeeping_staging,
		       cpu_possible_mask, non_housekeeping_mask);

	first_cpu = cpumask_first_and(cpu_present_mask, housekeeping_staging);
	إذا (first_cpu >= nr_cpu_ids || first_cpu >= setup_max_cpus) {
		__cpumask_set_cpu(smp_processor_id(), housekeeping_staging);
		__cpumask_clear_cpu(smp_processor_id(), non_housekeeping_mask);
		إذا (!housekeeping.flags) {
			pr_warn("Housekeeping: must include one present CPU, "
				"using boot CPU:%d\n", smp_processor_id());
		}
	}

	إذا (cpumask_empty(non_housekeeping_mask))
		اذهب free_housekeeping_staging;

	إذا (!housekeeping.flags) {
		/* First setup call ("nohz_full=" or "isolcpus=") */
		تعداد hk_type type;

		for_each_set_bit(type, &flags, HK_TYPE_MAX)
			housekeeping_setup_type(type, housekeeping_staging);
	} وإلا {
		/* Second setup call ("nohz_full=" after "isolcpus=" or the reverse) */
		تعداد hk_type type;
		unsigned long iter_flags = flags & housekeeping.flags;

		for_each_set_bit(type, &iter_flags, HK_TYPE_MAX) {
			إذا (!cpumask_equal(housekeeping_staging,
					   housekeeping_cpumask(type))) {
				pr_warn("Housekeeping: nohz_full= must match isolcpus=\n");
				اذهب free_housekeeping_staging;
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
		إذا (first_cpu >= min(nr_cpu_ids, setup_max_cpus)) {
			pr_warn("Housekeeping: must include one present CPU "
				"neither in nohz_full= nor in isolcpus=domain, "
				"ignoring setting %s\n", str);
			اذهب free_housekeeping_staging;
		}

		iter_flags = flags & ~housekeeping.flags;

		for_each_set_bit(type, &iter_flags, HK_TYPE_MAX)
			housekeeping_setup_type(type, housekeeping_staging);
	}

	إذا ((flags & HK_FLAG_KERNEL_NOISE) && !(housekeeping.flags & HK_FLAG_KERNEL_NOISE))
		tick_nohz_full_setup(non_housekeeping_mask);

	housekeeping.flags |= flags;
	err = 1;

free_housekeeping_staging:
	free_bootmem_cpumask_var(housekeeping_staging);
free_non_housekeeping_mask:
	free_bootmem_cpumask_var(non_housekeeping_mask);

	أرجع err;
}

ساكن صحيح __init housekeeping_nohz_full_setup(حرف *str)
{
	unsigned long flags;

	flags = HK_FLAG_KERNEL_NOISE;

	أرجع housekeeping_setup(str, flags);
}
__setup("nohz_full=", housekeeping_nohz_full_setup);

ساكن صحيح __init housekeeping_isolcpus_setup(حرف *str)
{
	unsigned long flags = 0;
	bool illegal = false;
	حرف *par;
	صحيح len;

	طالما (isalpha(*str)) {
		/*
		 * isolcpus=nohz is equivalent to nohz_full.
		 */
		إذا (!strncmp(str, "nohz,", 5)) {
			str += 5;
			flags |= HK_FLAG_KERNEL_NOISE;
			استمر;
		}

		إذا (!strncmp(str, "domain,", 7)) {
			str += 7;
			flags |= HK_FLAG_DOMAIN | HK_FLAG_DOMAIN_BOOT;
			استمر;
		}

		إذا (!strncmp(str, "managed_irq,", 12)) {
			str += 12;
			flags |= HK_FLAG_MANAGED_IRQ;
			استمر;
		}

		/*
		 * Skip unknown sub-parameter and validate that it is not
		 * containing an invalid character.
		 */
		لكل (par = str, len = 0; *str && *str != ','; str++, len++) {
			إذا (!isalpha(*str) && *str != '_')
				illegal = true;
		}

		إذا (illegal) {
			pr_warn("isolcpus: Invalid flag %.*s\n", len, par);
			أرجع 0;
		}

		pr_info("isolcpus: Skipped unknown flag %.*s\n", len, par);
		str++;
	}

	/* Default behaviour for isolcpus without flags */
	إذا (!flags)
		flags |= HK_FLAG_DOMAIN | HK_FLAG_DOMAIN_BOOT;

	أرجع housekeeping_setup(str, flags);
}
__setup("isolcpus=", housekeeping_isolcpus_setup);
