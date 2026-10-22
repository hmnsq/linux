// SPDX-License-Identifier: GPL-2.0-only
/*
 *  Housekeeping management. Manage the targets for routine code that can run on
 *  any CPU: unbound workqueues, timers, kthreads and any offloadable work.
 *
 * Copyright (C) 2017 Red Hat, Inc., Frederic Weisbecker
 * Copyright (C) 2017-2018 SUSE, Frederic Weisbecker
 *
 */
#అన్తర్భావ <linux/sched/isolation.h>
#అన్తర్భావ <linux/llist.h>
#అన్తర్భావ <linux/pci.h>
#అన్తర్భావ "sched.h"

గణన hk_flags {
	HK_FLAG_DOMAIN_BOOT	= బిట్టు(HK_TYPE_DOMAIN_BOOT),
	HK_FLAG_DOMAIN		= బిట్టు(HK_TYPE_DOMAIN),
	HK_FLAG_MANAGED_IRQ	= బిట్టు(HK_TYPE_MANAGED_IRQ),
	HK_FLAG_KERNEL_NOISE	= బిట్టు(HK_TYPE_KERNEL_NOISE),
};

DEFINE_STATIC_కీలకం_FALSE(housekeeping_overridden);
EXPORT_చిహ్నం_GPL(housekeeping_overridden);

నిర్మాణం housekeeping {
	నిర్మాణం cpumask __rcu *cpumasks[HK_TYPE_MAX];
	గుర్తులేని దీర్ఘ flags;
};

స్థిర నిర్మాణం housekeeping housekeeping;
స్థిర __initdata LLIST_తల(memblock_freelist);

bool housekeeping_enabled(గణన hk_type type)
{
	తిరిగి !!(READ_ONCE(housekeeping.flags) & బిట్టు(type));
}
EXPORT_చిహ్నం_GPL(housekeeping_enabled);

స్థిర bool housekeeping_dereference_check(గణన hk_type type)
{
	ఒకవేళ (IS_ENABLED(CONFIG_LOCKDEP) && type == HK_TYPE_DOMAIN) {
		/* Cpuset isn't even writable yet? */
		ఒకవేళ (system_state <= SYSTEM_SCHEDULING)
			తిరిగి true;

		/* CPU hotplug write locked, so cpuset partition can't be overwritten */
		ఒకవేళ (IS_ENABLED(CONFIG_HOTPLUG_CPU) && lockdep_is_cpus_write_held())
			తిరిగి true;

		/* Cpuset lock held, partitions not writable */
		ఒకవేళ (IS_ENABLED(CONFIG_CPUSETS) && lockdep_is_cpuset_held())
			తిరిగి true;

		తిరిగి false;
	}

	తిరిగి true;
}

స్థిర అంతఃవరుస నిర్మాణం cpumask *housekeeping_cpumask_dereference(గణన hk_type type)
{
	తిరిగి rcu_dereference_all_check(housekeeping.cpumasks[type],
					 housekeeping_dereference_check(type));
}

స్థిరరాశి నిర్మాణం cpumask *housekeeping_cpumask(గణన hk_type type)
{
	స్థిరరాశి నిర్మాణం cpumask *ముసుగు = ఖాళీ;

	ఒకవేళ (static_branch_unlikely(&housekeeping_overridden)) {
		ఒకవేళ (READ_ONCE(housekeeping.flags) & బిట్టు(type))
			ముసుగు = housekeeping_cpumask_dereference(type);
	}
	ఒకవేళ (!ముసుగు)
		ముసుగు = cpu_possible_ముసుగు;
	తిరిగి ముసుగు;
}
EXPORT_చిహ్నం_GPL(housekeeping_cpumask);

పూర్ణాంకం housekeeping_any_cpu(గణన hk_type type)
{
	పూర్ణాంకం cpu;

	ఒకవేళ (static_branch_unlikely(&housekeeping_overridden)) {
		ఒకవేళ (housekeeping.flags & బిట్టు(type)) {
			cpu = sched_numa_కనుగొను_closest(housekeeping_cpumask(type), smp_processor_id());
			ఒకవేళ (cpu < nr_cpu_ids)
				తిరిగి cpu;

			cpu = cpumask_any_and_distribute(housekeeping_cpumask(type), cpu_online_ముసుగు);
			ఒకవేళ (likely(cpu < nr_cpu_ids))
				తిరిగి cpu;
			/*
			 * Unless we have another problem this can only happen
			 * at boot time before start_secondary() brings the 1st
			 * housekeeping CPU up.
			 */
			WARN_ON_ONCE(system_state == SYSTEM_RUNNING ||
				     type != HK_TYPE_TIMER);
		}
	}
	తిరిగి smp_processor_id();
}
EXPORT_చిహ్నం_GPL(housekeeping_any_cpu);

శూన్య housekeeping_affine(నిర్మాణం task_struct *t, గణన hk_type type)
{
	ఒకవేళ (static_branch_unlikely(&housekeeping_overridden))
		ఒకవేళ (housekeeping.flags & బిట్టు(type))
			సమితి_cpus_allowed_ptr(t, housekeeping_cpumask(type));
}
EXPORT_చిహ్నం_GPL(housekeeping_affine);

bool housekeeping_test_cpu(పూర్ణాంకం cpu, గణన hk_type type)
{
	ఒకవేళ (static_branch_unlikely(&housekeeping_overridden) &&
	    READ_ONCE(housekeeping.flags) & బిట్టు(type))
		తిరిగి cpumask_test_cpu(cpu, housekeeping_cpumask(type));
	తిరిగి true;
}
EXPORT_చిహ్నం_GPL(housekeeping_test_cpu);

పూర్ణాంకం housekeeping_update(నిర్మాణం cpumask *isol_ముసుగు)
{
	నిర్మాణం cpumask *trial, *old = ఖాళీ;
	పూర్ణాంకం err;

	trial = kmalloc(cpumask_కొలత(), GFP_KERNEL);
	ఒకవేళ (!trial)
		తిరిగి -ENOMEM;

	cpumask_andnot(trial, housekeeping_cpumask(HK_TYPE_DOMAIN_BOOT), isol_ముసుగు);
	ఒకవేళ (!cpumask_intersects(trial, cpu_online_ముసుగు)) {
		kfree(trial);
		తిరిగి -EINVAL;
	}

	ఒకవేళ (!housekeeping.flags)
		static_branch_enable(&housekeeping_overridden);

	ఒకవేళ (housekeeping.flags & HK_FLAG_DOMAIN)
		old = housekeeping_cpumask_dereference(HK_TYPE_DOMAIN);
	లేకపోతే
		WRITE_ONCE(housekeeping.flags, housekeeping.flags | HK_FLAG_DOMAIN);
	rcu_కేటాయించు_సూచిక(housekeeping.cpumasks[HK_TYPE_DOMAIN], trial);

	synchronize_rcu();

	pci_probe_flush_workqueue();
	mem_cgroup_flush_workqueue();
	vmstat_flush_workqueue();

	err = workqueue_unbound_housekeeping_update(housekeeping_cpumask(HK_TYPE_DOMAIN));
	WARN_ON_ONCE(err < 0);

	err = tmigr_isolated_exclude_cpumask(isol_ముసుగు);
	WARN_ON_ONCE(err < 0);

	err = kthreads_update_housekeeping();
	WARN_ON_ONCE(err < 0);

	kfree(old);

	తిరిగి 0;
}

శూన్య __init housekeeping_init(శూన్య)
{
	గణన hk_type type;

	ఒకవేళ (!housekeeping.flags)
		తిరిగి;

	static_branch_enable(&housekeeping_overridden);

	ఒకవేళ (housekeeping.flags & HK_FLAG_KERNEL_NOISE)
		sched_tick_offload_init();
	/*
	 * Realloc with a proper allocator so that any cpumask update
	 * can indifferently free the old version with kfree().
	 */
	for_each_సమితి_బిట్టు(type, &housekeeping.flags, HK_TYPE_MAX) {
		నిర్మాణం cpumask *omask, *nmask = kmalloc(cpumask_కొలత(), GFP_KERNEL);

		ఒకవేళ (WARN_ON_ONCE(!nmask))
			తిరిగి;

		omask = rcu_dereference(housekeeping.cpumasks[type]);

		/* We need at least one CPU to handle housekeeping work */
		WARN_ON_ONCE(cpumask_ఖాళీలు(omask));
		cpumask_copy(nmask, omask);
		RCU_INIT_సూచిక(housekeeping.cpumasks[type], nmask);
		__llist_add((నిర్మాణం llist_కణుపు *)omask, &memblock_freelist);
	}
}

స్థిర పూర్ణాంకం __init housekeeping_late_init(శూన్య)
{
	నిర్మాణం llist_కణుపు *llnode, *pos, *t;

	/* Free allocated memblock memory, if any */
	llnode = __llist_del_all(&memblock_freelist);
	llist_for_each_సురక్షిత(pos, t, llnode)
		memblock_free(pos, cpumask_కొలత());
	తిరిగి 0;
}
శుద్ధ_initcall(housekeeping_late_init);

స్థిర శూన్య __init housekeeping_అమరిక_type(గణన hk_type type,
					   cpumask_var_t housekeeping_staging)
{
	నిర్మాణం cpumask *ముసుగు = memblock_alloc_or_panic(cpumask_కొలత(), SMP_CACHE_బైట్లు);

	cpumask_copy(ముసుగు, housekeeping_staging);
	RCU_INIT_సూచిక(housekeeping.cpumasks[type], ముసుగు);
}

స్థిర పూర్ణాంకం __init housekeeping_అమరిక(అక్షరం *str, గుర్తులేని దీర్ఘ flags)
{
	cpumask_var_t non_housekeeping_ముసుగు, housekeeping_staging;
	గుర్తులేని పూర్ణాంకం మొదటి_cpu;
	పూర్ణాంకం err = 0;

	ఒకవేళ ((flags & HK_FLAG_KERNEL_NOISE) && !(housekeeping.flags & HK_FLAG_KERNEL_NOISE)) {
		ఒకవేళ (!IS_ENABLED(CONFIG_NO_HZ_నిండిన)) {
			pr_warn("Housekeeping: nohz unsupported."
				" Build with CONFIG_NO_HZ_FULL\n");
			తిరిగి 0;
		}
	}

	alloc_bootmem_cpumask_var(&non_housekeeping_ముసుగు);
	ఒకవేళ (cpulist_parse(str, non_housekeeping_ముసుగు) < 0) {
		pr_warn("Housekeeping: nohz_full= or isolcpus= incorrect CPU range\n");
		వెళ్ళు free_non_housekeeping_ముసుగు;
	}

	alloc_bootmem_cpumask_var(&housekeeping_staging);
	cpumask_andnot(housekeeping_staging,
		       cpu_possible_ముసుగు, non_housekeeping_ముసుగు);

	మొదటి_cpu = cpumask_మొదటి_and(cpu_present_ముసుగు, housekeeping_staging);
	ఒకవేళ (మొదటి_cpu >= nr_cpu_ids || మొదటి_cpu >= అమరిక_max_cpus) {
		__cpumask_set_cpu(smp_processor_id(), housekeeping_staging);
		__cpumask_clear_cpu(smp_processor_id(), non_housekeeping_ముసుగు);
		ఒకవేళ (!housekeeping.flags) {
			pr_warn("Housekeeping: must include one present CPU, "
				"using boot CPU:%d\n", smp_processor_id());
		}
	}

	ఒకవేళ (cpumask_ఖాళీలు(non_housekeeping_ముసుగు))
		వెళ్ళు free_housekeeping_staging;

	ఒకవేళ (!housekeeping.flags) {
		/* First setup call ("nohz_full=" or "isolcpus=") */
		గణన hk_type type;

		for_each_సమితి_బిట్టు(type, &flags, HK_TYPE_MAX)
			housekeeping_అమరిక_type(type, housekeeping_staging);
	} లేకపోతే {
		/* Second setup call ("nohz_full=" after "isolcpus=" or the reverse) */
		గణన hk_type type;
		గుర్తులేని దీర్ఘ iter_flags = flags & housekeeping.flags;

		for_each_సమితి_బిట్టు(type, &iter_flags, HK_TYPE_MAX) {
			ఒకవేళ (!cpumask_equal(housekeeping_staging,
					   housekeeping_cpumask(type))) {
				pr_warn("Housekeeping: nohz_full= must match isolcpus=\n");
				వెళ్ళు free_housekeeping_staging;
			}
		}

		/*
		 * Check the combination of nohz_full and isolcpus=domain,
		 * necessary to avoid problems with the timer migration
		 * hierarchy. managed_irq is ignored by this check since it
		 * isn't considered in the timer migration logic.
		 */
		iter_flags = housekeeping.flags & (HK_FLAG_KERNEL_NOISE | HK_FLAG_DOMAIN);
		type = కనుగొను_మొదటి_బిట్టు(&iter_flags, HK_TYPE_MAX);
		/*
		 * Pass the check if none of these flags were previously set or
		 * are not in the current selection.
		 */
		iter_flags = flags & (HK_FLAG_KERNEL_NOISE | HK_FLAG_DOMAIN);
		మొదటి_cpu = (type == HK_TYPE_MAX || !iter_flags) ? 0 :
			    cpumask_మొదటి_and_and(cpu_present_ముసుగు,
						  housekeeping_staging, housekeeping_cpumask(type));
		ఒకవేళ (మొదటి_cpu >= min(nr_cpu_ids, అమరిక_max_cpus)) {
			pr_warn("Housekeeping: must include one present CPU "
				"neither in nohz_full= nor in isolcpus=domain, "
				"ignoring setting %s\n", str);
			వెళ్ళు free_housekeeping_staging;
		}

		iter_flags = flags & ~housekeeping.flags;

		for_each_సమితి_బిట్టు(type, &iter_flags, HK_TYPE_MAX)
			housekeeping_అమరిక_type(type, housekeeping_staging);
	}

	ఒకవేళ ((flags & HK_FLAG_KERNEL_NOISE) && !(housekeeping.flags & HK_FLAG_KERNEL_NOISE))
		tick_nohz_నిండిన_అమరిక(non_housekeeping_ముసుగు);

	housekeeping.flags |= flags;
	err = 1;

free_housekeeping_staging:
	free_bootmem_cpumask_var(housekeeping_staging);
free_non_housekeeping_ముసుగు:
	free_bootmem_cpumask_var(non_housekeeping_ముసుగు);

	తిరిగి err;
}

స్థిర పూర్ణాంకం __init housekeeping_nohz_నిండిన_అమరిక(అక్షరం *str)
{
	గుర్తులేని దీర్ఘ flags;

	flags = HK_FLAG_KERNEL_NOISE;

	తిరిగి housekeeping_అమరిక(str, flags);
}
__setup("nohz_full=", housekeeping_nohz_నిండిన_అమరిక);

స్థిర పూర్ణాంకం __init housekeeping_isolcpus_అమరిక(అక్షరం *str)
{
	గుర్తులేని దీర్ఘ flags = 0;
	bool illegal = false;
	అక్షరం *par;
	పూర్ణాంకం len;

	అయితే (అక్షరమా(*str)) {
		/*
		 * isolcpus=nohz is equivalent to nohz_full.
		 */
		ఒకవేళ (!అక్షరశ్రేణి_పోలిక(str, "nohz,", 5)) {
			str += 5;
			flags |= HK_FLAG_KERNEL_NOISE;
			కొనసాగు;
		}

		ఒకవేళ (!అక్షరశ్రేణి_పోలిక(str, "domain,", 7)) {
			str += 7;
			flags |= HK_FLAG_DOMAIN | HK_FLAG_DOMAIN_BOOT;
			కొనసాగు;
		}

		ఒకవేళ (!అక్షరశ్రేణి_పోలిక(str, "managed_irq,", 12)) {
			str += 12;
			flags |= HK_FLAG_MANAGED_IRQ;
			కొనసాగు;
		}

		/*
		 * Skip unknown sub-parameter and validate that it is not
		 * containing an invalid character.
		 */
		కొరకు (par = str, len = 0; *str && *str != ','; str++, len++) {
			ఒకవేళ (!అక్షరమా(*str) && *str != '_')
				illegal = true;
		}

		ఒకవేళ (illegal) {
			pr_warn("isolcpus: Invalid flag %.*s\n", len, par);
			తిరిగి 0;
		}

		pr_info("isolcpus: Skipped unknown flag %.*s\n", len, par);
		str++;
	}

	/* Default behaviour for isolcpus without flags */
	ఒకవేళ (!flags)
		flags |= HK_FLAG_DOMAIN | HK_FLAG_DOMAIN_BOOT;

	తిరిగి housekeeping_అమరిక(str, flags);
}
__setup("isolcpus=", housekeeping_isolcpus_అమరిక);
