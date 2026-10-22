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

شمار hk_flags {
	HK_FLAG_DOMAIN_BOOT	= ثنائی_ہندسہ(HK_TYPE_DOMAIN_BOOT),
	HK_FLAG_DOMAIN		= ثنائی_ہندسہ(HK_TYPE_DOMAIN),
	HK_FLAG_MANAGED_IRQ	= ثنائی_ہندسہ(HK_TYPE_MANAGED_IRQ),
	HK_FLAG_KERNEL_NOISE	= ثنائی_ہندسہ(HK_TYPE_KERNEL_NOISE),
};

DEFINE_STATIC_کلید_FALSE(housekeeping_overridden);
EXPORT_علامت_GPL(housekeeping_overridden);

ساخت housekeeping {
	ساخت cpumask __rcu *cpumasks[HK_TYPE_MAX];
	غیرعلامتی طویل flags;
};

ساکن ساخت housekeeping housekeeping;
ساکن __initdata LLIST_سر(memblock_freelist);

bool housekeeping_enabled(شمار hk_type type)
{
	واپس !!(READ_ONCE(housekeeping.flags) & ثنائی_ہندسہ(type));
}
EXPORT_علامت_GPL(housekeeping_enabled);

ساکن bool housekeeping_dereference_check(شمار hk_type type)
{
	اگر (IS_ENABLED(CONFIG_LOCKDEP) && type == HK_TYPE_DOMAIN) {
		/* Cpuset isn't even writable yet? */
		اگر (system_state <= SYSTEM_SCHEDULING)
			واپس true;

		/* CPU hotplug write locked, so cpuset partition can't be overwritten */
		اگر (IS_ENABLED(CONFIG_HOTPLUG_CPU) && lockdep_is_cpus_write_held())
			واپس true;

		/* Cpuset lock held, partitions not writable */
		اگر (IS_ENABLED(CONFIG_CPUSETS) && lockdep_is_cpuset_held())
			واپس true;

		واپس false;
	}

	واپس true;
}

ساکن اندرونی ساخت cpumask *housekeeping_cpumask_dereference(شمار hk_type type)
{
	واپس rcu_dereference_all_check(housekeeping.cpumasks[type],
					 housekeeping_dereference_check(type));
}

ثابت ساخت cpumask *housekeeping_cpumask(شمار hk_type type)
{
	ثابت ساخت cpumask *نقاب = تہی;

	اگر (static_branch_unlikely(&housekeeping_overridden)) {
		اگر (READ_ONCE(housekeeping.flags) & ثنائی_ہندسہ(type))
			نقاب = housekeeping_cpumask_dereference(type);
	}
	اگر (!نقاب)
		نقاب = cpu_possible_نقاب;
	واپس نقاب;
}
EXPORT_علامت_GPL(housekeeping_cpumask);

صحیح housekeeping_any_cpu(شمار hk_type type)
{
	صحیح cpu;

	اگر (static_branch_unlikely(&housekeeping_overridden)) {
		اگر (housekeeping.flags & ثنائی_ہندسہ(type)) {
			cpu = sched_numa_ڈھونڈو_closest(housekeeping_cpumask(type), smp_processor_id());
			اگر (cpu < nr_cpu_ids)
				واپس cpu;

			cpu = cpumask_any_and_distribute(housekeeping_cpumask(type), cpu_online_نقاب);
			اگر (likely(cpu < nr_cpu_ids))
				واپس cpu;
			/*
			 * Unless we have another problem this can only happen
			 * at boot time before start_secondary() brings the 1st
			 * housekeeping CPU up.
			 */
			WARN_ON_ONCE(system_state == SYSTEM_RUNNING ||
				     type != HK_TYPE_TIMER);
		}
	}
	واپس smp_processor_id();
}
EXPORT_علامت_GPL(housekeeping_any_cpu);

خالی housekeeping_affine(ساخت task_struct *t, شمار hk_type type)
{
	اگر (static_branch_unlikely(&housekeeping_overridden))
		اگر (housekeeping.flags & ثنائی_ہندسہ(type))
			سیٹ_cpus_allowed_ptr(t, housekeeping_cpumask(type));
}
EXPORT_علامت_GPL(housekeeping_affine);

bool housekeeping_test_cpu(صحیح cpu, شمار hk_type type)
{
	اگر (static_branch_unlikely(&housekeeping_overridden) &&
	    READ_ONCE(housekeeping.flags) & ثنائی_ہندسہ(type))
		واپس cpumask_test_cpu(cpu, housekeeping_cpumask(type));
	واپس true;
}
EXPORT_علامت_GPL(housekeeping_test_cpu);

صحیح housekeeping_update(ساخت cpumask *isol_نقاب)
{
	ساخت cpumask *trial, *old = تہی;
	صحیح err;

	trial = kmalloc(cpumask_سائز(), GFP_KERNEL);
	اگر (!trial)
		واپس -ENOMEM;

	cpumask_andnot(trial, housekeeping_cpumask(HK_TYPE_DOMAIN_BOOT), isol_نقاب);
	اگر (!cpumask_intersects(trial, cpu_online_نقاب)) {
		kfree(trial);
		واپس -EINVAL;
	}

	اگر (!housekeeping.flags)
		static_branch_enable(&housekeeping_overridden);

	اگر (housekeeping.flags & HK_FLAG_DOMAIN)
		old = housekeeping_cpumask_dereference(HK_TYPE_DOMAIN);
	ورنہ
		WRITE_ONCE(housekeeping.flags, housekeeping.flags | HK_FLAG_DOMAIN);
	rcu_تفویض_اشارہ_گر(housekeeping.cpumasks[HK_TYPE_DOMAIN], trial);

	synchronize_rcu();

	pci_probe_flush_workqueue();
	mem_cgroup_flush_workqueue();
	vmstat_flush_workqueue();

	err = workqueue_unbound_housekeeping_update(housekeeping_cpumask(HK_TYPE_DOMAIN));
	WARN_ON_ONCE(err < 0);

	err = tmigr_isolated_exclude_cpumask(isol_نقاب);
	WARN_ON_ONCE(err < 0);

	err = kthreads_update_housekeeping();
	WARN_ON_ONCE(err < 0);

	kfree(old);

	واپس 0;
}

خالی __init housekeeping_init(خالی)
{
	شمار hk_type type;

	اگر (!housekeeping.flags)
		واپس;

	static_branch_enable(&housekeeping_overridden);

	اگر (housekeeping.flags & HK_FLAG_KERNEL_NOISE)
		sched_tick_offload_init();
	/*
	 * Realloc with a proper allocator so that any cpumask update
	 * can indifferently free the old version with kfree().
	 */
	for_each_سیٹ_ثنائی_ہندسہ(type, &housekeeping.flags, HK_TYPE_MAX) {
		ساخت cpumask *omask, *nmask = kmalloc(cpumask_سائز(), GFP_KERNEL);

		اگر (WARN_ON_ONCE(!nmask))
			واپس;

		omask = rcu_dereference(housekeeping.cpumasks[type]);

		/* We need at least one CPU to handle housekeeping work */
		WARN_ON_ONCE(cpumask_خالی_خانے(omask));
		cpumask_copy(nmask, omask);
		RCU_INIT_اشارہ_گر(housekeeping.cpumasks[type], nmask);
		__llist_add((ساخت llist_گرہ *)omask, &memblock_freelist);
	}
}

ساکن صحیح __init housekeeping_late_init(خالی)
{
	ساخت llist_گرہ *llnode, *pos, *t;

	/* Free allocated memblock memory, if any */
	llnode = __llist_del_all(&memblock_freelist);
	llist_for_each_محفوظ(pos, t, llnode)
		memblock_free(pos, cpumask_سائز());
	واپس 0;
}
خالص_initcall(housekeeping_late_init);

ساکن خالی __init housekeeping_تیاری_type(شمار hk_type type,
					   cpumask_var_t housekeeping_staging)
{
	ساخت cpumask *نقاب = memblock_alloc_or_panic(cpumask_سائز(), SMP_CACHE_بائٹیں);

	cpumask_copy(نقاب, housekeeping_staging);
	RCU_INIT_اشارہ_گر(housekeeping.cpumasks[type], نقاب);
}

ساکن صحیح __init housekeeping_تیاری(حرف *str, غیرعلامتی طویل flags)
{
	cpumask_var_t non_housekeeping_نقاب, housekeeping_staging;
	غیرعلامتی صحیح پہلا_cpu;
	صحیح err = 0;

	اگر ((flags & HK_FLAG_KERNEL_NOISE) && !(housekeeping.flags & HK_FLAG_KERNEL_NOISE)) {
		اگر (!IS_ENABLED(CONFIG_NO_HZ_بھرا)) {
			pr_warn("Housekeeping: nohz unsupported."
				" Build with CONFIG_NO_HZ_FULL\n");
			واپس 0;
		}
	}

	alloc_bootmem_cpumask_var(&non_housekeeping_نقاب);
	اگر (cpulist_parse(str, non_housekeeping_نقاب) < 0) {
		pr_warn("Housekeeping: nohz_full= or isolcpus= incorrect CPU range\n");
		جاؤ free_non_housekeeping_نقاب;
	}

	alloc_bootmem_cpumask_var(&housekeeping_staging);
	cpumask_andnot(housekeeping_staging,
		       cpu_possible_نقاب, non_housekeeping_نقاب);

	پہلا_cpu = cpumask_پہلا_and(cpu_present_نقاب, housekeeping_staging);
	اگر (پہلا_cpu >= nr_cpu_ids || پہلا_cpu >= تیاری_max_cpus) {
		__cpumask_set_cpu(smp_processor_id(), housekeeping_staging);
		__cpumask_clear_cpu(smp_processor_id(), non_housekeeping_نقاب);
		اگر (!housekeeping.flags) {
			pr_warn("Housekeeping: must include one present CPU, "
				"using boot CPU:%d\n", smp_processor_id());
		}
	}

	اگر (cpumask_خالی_خانے(non_housekeeping_نقاب))
		جاؤ free_housekeeping_staging;

	اگر (!housekeeping.flags) {
		/* First setup call ("nohz_full=" or "isolcpus=") */
		شمار hk_type type;

		for_each_سیٹ_ثنائی_ہندسہ(type, &flags, HK_TYPE_MAX)
			housekeeping_تیاری_type(type, housekeeping_staging);
	} ورنہ {
		/* Second setup call ("nohz_full=" after "isolcpus=" or the reverse) */
		شمار hk_type type;
		غیرعلامتی طویل iter_flags = flags & housekeeping.flags;

		for_each_سیٹ_ثنائی_ہندسہ(type, &iter_flags, HK_TYPE_MAX) {
			اگر (!cpumask_equal(housekeeping_staging,
					   housekeeping_cpumask(type))) {
				pr_warn("Housekeeping: nohz_full= must match isolcpus=\n");
				جاؤ free_housekeeping_staging;
			}
		}

		/*
		 * Check the combination of nohz_full and isolcpus=domain,
		 * necessary to avoid problems with the timer migration
		 * hierarchy. managed_irq is ignored by this check since it
		 * isn't considered in the timer migration logic.
		 */
		iter_flags = housekeeping.flags & (HK_FLAG_KERNEL_NOISE | HK_FLAG_DOMAIN);
		type = ڈھونڈو_پہلا_ثنائی_ہندسہ(&iter_flags, HK_TYPE_MAX);
		/*
		 * Pass the check if none of these flags were previously set or
		 * are not in the current selection.
		 */
		iter_flags = flags & (HK_FLAG_KERNEL_NOISE | HK_FLAG_DOMAIN);
		پہلا_cpu = (type == HK_TYPE_MAX || !iter_flags) ? 0 :
			    cpumask_پہلا_and_and(cpu_present_نقاب,
						  housekeeping_staging, housekeeping_cpumask(type));
		اگر (پہلا_cpu >= min(nr_cpu_ids, تیاری_max_cpus)) {
			pr_warn("Housekeeping: must include one present CPU "
				"neither in nohz_full= nor in isolcpus=domain, "
				"ignoring setting %s\n", str);
			جاؤ free_housekeeping_staging;
		}

		iter_flags = flags & ~housekeeping.flags;

		for_each_سیٹ_ثنائی_ہندسہ(type, &iter_flags, HK_TYPE_MAX)
			housekeeping_تیاری_type(type, housekeeping_staging);
	}

	اگر ((flags & HK_FLAG_KERNEL_NOISE) && !(housekeeping.flags & HK_FLAG_KERNEL_NOISE))
		tick_nohz_بھرا_تیاری(non_housekeeping_نقاب);

	housekeeping.flags |= flags;
	err = 1;

free_housekeeping_staging:
	free_bootmem_cpumask_var(housekeeping_staging);
free_non_housekeeping_نقاب:
	free_bootmem_cpumask_var(non_housekeeping_نقاب);

	واپس err;
}

ساکن صحیح __init housekeeping_nohz_بھرا_تیاری(حرف *str)
{
	غیرعلامتی طویل flags;

	flags = HK_FLAG_KERNEL_NOISE;

	واپس housekeeping_تیاری(str, flags);
}
__setup("nohz_full=", housekeeping_nohz_بھرا_تیاری);

ساکن صحیح __init housekeeping_isolcpus_تیاری(حرف *str)
{
	غیرعلامتی طویل flags = 0;
	bool illegal = false;
	حرف *par;
	صحیح len;

	جب (حرف_ہے(*str)) {
		/*
		 * isolcpus=nohz is equivalent to nohz_full.
		 */
		اگر (!لڑی_موازنہ(str, "nohz,", 5)) {
			str += 5;
			flags |= HK_FLAG_KERNEL_NOISE;
			جاری;
		}

		اگر (!لڑی_موازنہ(str, "domain,", 7)) {
			str += 7;
			flags |= HK_FLAG_DOMAIN | HK_FLAG_DOMAIN_BOOT;
			جاری;
		}

		اگر (!لڑی_موازنہ(str, "managed_irq,", 12)) {
			str += 12;
			flags |= HK_FLAG_MANAGED_IRQ;
			جاری;
		}

		/*
		 * Skip unknown sub-parameter and validate that it is not
		 * containing an invalid character.
		 */
		برائے (par = str, len = 0; *str && *str != ','; str++, len++) {
			اگر (!حرف_ہے(*str) && *str != '_')
				illegal = true;
		}

		اگر (illegal) {
			pr_warn("isolcpus: Invalid flag %.*s\n", len, par);
			واپس 0;
		}

		pr_info("isolcpus: Skipped unknown flag %.*s\n", len, par);
		str++;
	}

	/* Default behaviour for isolcpus without flags */
	اگر (!flags)
		flags |= HK_FLAG_DOMAIN | HK_FLAG_DOMAIN_BOOT;

	واپس housekeeping_تیاری(str, flags);
}
__setup("isolcpus=", housekeeping_isolcpus_تیاری);
