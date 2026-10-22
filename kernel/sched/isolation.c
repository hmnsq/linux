// SPDX-License-Identifier: GPL-2.0-only
/*
 *  Housekeeping management. Manage the targets for routine code that can run on
 *  any CPU: unbound workqueues, timers, kthreads and any offloadable work.
 *
 * Copyright (C) 2017 Red Hat, Inc., Frederic Weisbecker
 * Copyright (C) 2017-2018 SUSE, Frederic Weisbecker
 *
 */
#شامل <linux/sched/isolation.h>
#شامل <linux/llist.h>
#شامل <linux/pci.h>
#شامل "sched.h"

شمارش hk_flags {
	HK_FLAG_DOMAIN_BOOT	= BIT(HK_TYPE_DOMAIN_BOOT),
	HK_FLAG_DOMAIN		= BIT(HK_TYPE_DOMAIN),
	HK_FLAG_MANAGED_IRQ	= BIT(HK_TYPE_MANAGED_IRQ),
	HK_FLAG_KERNEL_NOISE	= BIT(HK_TYPE_KERNEL_NOISE),
};

DEFINE_STATIC_کلید_FALSE(housekeeping_overridden);
EXPORT_نماد_GPL(housekeeping_overridden);

ساختار housekeeping {
	ساختار cpumask __rcu *cpumasks[HK_TYPE_MAX];
	unsigned long flags;
};

ایستا ساختار housekeeping housekeeping;
ایستا __initdata LLIST_HEAD(memblock_freelist);

bool housekeeping_enabled(شمارش hk_type type)
{
	بازگشت !!(READ_ONCE(housekeeping.flags) & BIT(type));
}
EXPORT_نماد_GPL(housekeeping_enabled);

ایستا bool housekeeping_dereference_check(شمارش hk_type type)
{
	اگر (IS_ENABLED(CONFIG_LOCKDEP) && type == HK_TYPE_DOMAIN) {
		/* Cpuset isn't even writable yet? */
		اگر (system_state <= SYSTEM_SCHEDULING)
			بازگشت true;

		/* CPU hotplug write locked, so cpuset partition can't be overwritten */
		اگر (IS_ENABLED(CONFIG_HOTPLUG_CPU) && lockdep_is_cpus_write_held())
			بازگشت true;

		/* Cpuset lock held, partitions not writable */
		اگر (IS_ENABLED(CONFIG_CPUSETS) && lockdep_is_cpuset_held())
			بازگشت true;

		بازگشت false;
	}

	بازگشت true;
}

ایستا درونخطی ساختار cpumask *housekeeping_cpumask_dereference(شمارش hk_type type)
{
	بازگشت rcu_dereference_all_check(housekeeping.cpumasks[type],
					 housekeeping_dereference_check(type));
}

ثابت ساختار cpumask *housekeeping_cpumask(شمارش hk_type type)
{
	ثابت ساختار cpumask *mask = پوچ;

	اگر (static_branch_unlikely(&housekeeping_overridden)) {
		اگر (READ_ONCE(housekeeping.flags) & BIT(type))
			mask = housekeeping_cpumask_dereference(type);
	}
	اگر (!mask)
		mask = cpu_possible_mask;
	بازگشت mask;
}
EXPORT_نماد_GPL(housekeeping_cpumask);

صحیح housekeeping_any_cpu(شمارش hk_type type)
{
	صحیح cpu;

	اگر (static_branch_unlikely(&housekeeping_overridden)) {
		اگر (housekeeping.flags & BIT(type)) {
			cpu = sched_numa_find_closest(housekeeping_cpumask(type), smp_processor_id());
			اگر (cpu < nr_cpu_ids)
				بازگشت cpu;

			cpu = cpumask_any_and_distribute(housekeeping_cpumask(type), cpu_online_mask);
			اگر (likely(cpu < nr_cpu_ids))
				بازگشت cpu;
			/*
			 * Unless we have another problem this can only happen
			 * at boot time before start_secondary() brings the 1st
			 * housekeeping CPU up.
			 */
			WARN_ON_ONCE(system_state == SYSTEM_RUNNING ||
				     type != HK_TYPE_TIMER);
		}
	}
	بازگشت smp_processor_id();
}
EXPORT_نماد_GPL(housekeeping_any_cpu);

تهی housekeeping_affine(ساختار task_struct *t, شمارش hk_type type)
{
	اگر (static_branch_unlikely(&housekeeping_overridden))
		اگر (housekeeping.flags & BIT(type))
			set_cpus_allowed_ptr(t, housekeeping_cpumask(type));
}
EXPORT_نماد_GPL(housekeeping_affine);

bool housekeeping_test_cpu(صحیح cpu, شمارش hk_type type)
{
	اگر (static_branch_unlikely(&housekeeping_overridden) &&
	    READ_ONCE(housekeeping.flags) & BIT(type))
		بازگشت cpumask_test_cpu(cpu, housekeeping_cpumask(type));
	بازگشت true;
}
EXPORT_نماد_GPL(housekeeping_test_cpu);

صحیح housekeeping_update(ساختار cpumask *isol_mask)
{
	ساختار cpumask *trial, *old = پوچ;
	صحیح err;

	trial = kmalloc(cpumask_size(), GFP_KERNEL);
	اگر (!trial)
		بازگشت -ENOMEM;

	cpumask_andnot(trial, housekeeping_cpumask(HK_TYPE_DOMAIN_BOOT), isol_mask);
	اگر (!cpumask_intersects(trial, cpu_online_mask)) {
		kfree(trial);
		بازگشت -EINVAL;
	}

	اگر (!housekeeping.flags)
		static_branch_enable(&housekeeping_overridden);

	اگر (housekeeping.flags & HK_FLAG_DOMAIN)
		old = housekeeping_cpumask_dereference(HK_TYPE_DOMAIN);
	وگرنه
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

	بازگشت 0;
}

تهی __init housekeeping_init(تهی)
{
	شمارش hk_type type;

	اگر (!housekeeping.flags)
		بازگشت;

	static_branch_enable(&housekeeping_overridden);

	اگر (housekeeping.flags & HK_FLAG_KERNEL_NOISE)
		sched_tick_offload_init();
	/*
	 * Realloc with a proper allocator so that any cpumask update
	 * can indifferently free the old version with kfree().
	 */
	for_each_set_bit(type, &housekeeping.flags, HK_TYPE_MAX) {
		ساختار cpumask *omask, *nmask = kmalloc(cpumask_size(), GFP_KERNEL);

		اگر (WARN_ON_ONCE(!nmask))
			بازگشت;

		omask = rcu_dereference(housekeeping.cpumasks[type]);

		/* We need at least one CPU to handle housekeeping work */
		WARN_ON_ONCE(cpumask_empty(omask));
		cpumask_copy(nmask, omask);
		RCU_INIT_POINTER(housekeeping.cpumasks[type], nmask);
		__llist_add((ساختار llist_گره *)omask, &memblock_freelist);
	}
}

ایستا صحیح __init housekeeping_late_init(تهی)
{
	ساختار llist_گره *llnode, *pos, *t;

	/* Free allocated memblock memory, if any */
	llnode = __llist_del_all(&memblock_freelist);
	llist_for_each_safe(pos, t, llnode)
		memblock_free(pos, cpumask_size());
	بازگشت 0;
}
pure_initcall(housekeeping_late_init);

ایستا تهی __init housekeeping_setup_type(شمارش hk_type type,
					   cpumask_var_t housekeeping_staging)
{
	ساختار cpumask *mask = memblock_alloc_or_panic(cpumask_size(), SMP_CACHE_BYTES);

	cpumask_copy(mask, housekeeping_staging);
	RCU_INIT_POINTER(housekeeping.cpumasks[type], mask);
}

ایستا صحیح __init housekeeping_setup(نویسه *str, unsigned long flags)
{
	cpumask_var_t non_housekeeping_mask, housekeeping_staging;
	unsigned صحیح first_cpu;
	صحیح err = 0;

	اگر ((flags & HK_FLAG_KERNEL_NOISE) && !(housekeeping.flags & HK_FLAG_KERNEL_NOISE)) {
		اگر (!IS_ENABLED(CONFIG_NO_HZ_FULL)) {
			pr_warn("Housekeeping: nohz unsupported."
				" Build with CONFIG_NO_HZ_FULL\n");
			بازگشت 0;
		}
	}

	alloc_bootmem_cpumask_var(&non_housekeeping_mask);
	اگر (cpulist_parse(str, non_housekeeping_mask) < 0) {
		pr_warn("Housekeeping: nohz_full= or isolcpus= incorrect CPU range\n");
		برو free_non_housekeeping_mask;
	}

	alloc_bootmem_cpumask_var(&housekeeping_staging);
	cpumask_andnot(housekeeping_staging,
		       cpu_possible_mask, non_housekeeping_mask);

	first_cpu = cpumask_first_and(cpu_present_mask, housekeeping_staging);
	اگر (first_cpu >= nr_cpu_ids || first_cpu >= setup_max_cpus) {
		__cpumask_set_cpu(smp_processor_id(), housekeeping_staging);
		__cpumask_clear_cpu(smp_processor_id(), non_housekeeping_mask);
		اگر (!housekeeping.flags) {
			pr_warn("Housekeeping: must include one present CPU, "
				"using boot CPU:%d\n", smp_processor_id());
		}
	}

	اگر (cpumask_empty(non_housekeeping_mask))
		برو free_housekeeping_staging;

	اگر (!housekeeping.flags) {
		/* First setup call ("nohz_full=" or "isolcpus=") */
		شمارش hk_type type;

		for_each_set_bit(type, &flags, HK_TYPE_MAX)
			housekeeping_setup_type(type, housekeeping_staging);
	} وگرنه {
		/* Second setup call ("nohz_full=" after "isolcpus=" or the reverse) */
		شمارش hk_type type;
		unsigned long iter_flags = flags & housekeeping.flags;

		for_each_set_bit(type, &iter_flags, HK_TYPE_MAX) {
			اگر (!cpumask_equal(housekeeping_staging,
					   housekeeping_cpumask(type))) {
				pr_warn("Housekeeping: nohz_full= must match isolcpus=\n");
				برو free_housekeeping_staging;
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
		اگر (first_cpu >= min(nr_cpu_ids, setup_max_cpus)) {
			pr_warn("Housekeeping: must include one present CPU "
				"neither in nohz_full= nor in isolcpus=domain, "
				"ignoring setting %s\n", str);
			برو free_housekeeping_staging;
		}

		iter_flags = flags & ~housekeeping.flags;

		for_each_set_bit(type, &iter_flags, HK_TYPE_MAX)
			housekeeping_setup_type(type, housekeeping_staging);
	}

	اگر ((flags & HK_FLAG_KERNEL_NOISE) && !(housekeeping.flags & HK_FLAG_KERNEL_NOISE))
		tick_nohz_full_setup(non_housekeeping_mask);

	housekeeping.flags |= flags;
	err = 1;

free_housekeeping_staging:
	free_bootmem_cpumask_var(housekeeping_staging);
free_non_housekeeping_mask:
	free_bootmem_cpumask_var(non_housekeeping_mask);

	بازگشت err;
}

ایستا صحیح __init housekeeping_nohz_full_setup(نویسه *str)
{
	unsigned long flags;

	flags = HK_FLAG_KERNEL_NOISE;

	بازگشت housekeeping_setup(str, flags);
}
__setup("nohz_full=", housekeeping_nohz_full_setup);

ایستا صحیح __init housekeeping_isolcpus_setup(نویسه *str)
{
	unsigned long flags = 0;
	bool illegal = false;
	نویسه *par;
	صحیح len;

	وقتی (isalpha(*str)) {
		/*
		 * isolcpus=nohz is equivalent to nohz_full.
		 */
		اگر (!strncmp(str, "nohz,", 5)) {
			str += 5;
			flags |= HK_FLAG_KERNEL_NOISE;
			ادامه;
		}

		اگر (!strncmp(str, "domain,", 7)) {
			str += 7;
			flags |= HK_FLAG_DOMAIN | HK_FLAG_DOMAIN_BOOT;
			ادامه;
		}

		اگر (!strncmp(str, "managed_irq,", 12)) {
			str += 12;
			flags |= HK_FLAG_MANAGED_IRQ;
			ادامه;
		}

		/*
		 * Skip unknown sub-parameter and validate that it is not
		 * containing an invalid character.
		 */
		برای (par = str, len = 0; *str && *str != ','; str++, len++) {
			اگر (!isalpha(*str) && *str != '_')
				illegal = true;
		}

		اگر (illegal) {
			pr_warn("isolcpus: Invalid flag %.*s\n", len, par);
			بازگشت 0;
		}

		pr_info("isolcpus: Skipped unknown flag %.*s\n", len, par);
		str++;
	}

	/* Default behaviour for isolcpus without flags */
	اگر (!flags)
		flags |= HK_FLAG_DOMAIN | HK_FLAG_DOMAIN_BOOT;

	بازگشت housekeeping_setup(str, flags);
}
__setup("isolcpus=", housekeeping_isolcpus_setup);
