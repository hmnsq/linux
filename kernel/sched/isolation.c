// SPDX-License-Identifier: GPL-2.0-only
/*
 *  Housekeeping management. Manage the targets for routine code that can run on
 *  any CPU: unbound workqueues, timers, kthreads and any offloadable work.
 *
 * Copyright (C) 2017 Red Hat, Inc., Frederic Weisbecker
 * Copyright (C) 2017-2018 SUSE, Frederic Weisbecker
 *
 */
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/isolation.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/llist.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/pci.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "sched.h"

ਗਿਣਤੀ hk_flags {
	HK_FLAG_DOMAIN_BOOT	= ਦੋਆਧਾਰੀ_ਅੰਕ(HK_TYPE_DOMAIN_BOOT),
	HK_FLAG_DOMAIN		= ਦੋਆਧਾਰੀ_ਅੰਕ(HK_TYPE_DOMAIN),
	HK_FLAG_MANAGED_IRQ	= ਦੋਆਧਾਰੀ_ਅੰਕ(HK_TYPE_MANAGED_IRQ),
	HK_FLAG_KERNEL_NOISE	= ਦੋਆਧਾਰੀ_ਅੰਕ(HK_TYPE_KERNEL_NOISE),
};

DEFINE_STATIC_ਕੁੰਜੀ_FALSE(housekeeping_overridden);
EXPORT_ਚਿੰਨ੍ਹ_GPL(housekeeping_overridden);

ਬਣਤਰ housekeeping {
	ਬਣਤਰ cpumask __rcu *cpumasks[HK_TYPE_MAX];
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags;
};

ਸਥਿਰ ਬਣਤਰ housekeeping housekeeping;
ਸਥਿਰ __initdata LLIST_ਸਿਰ(memblock_freelist);

bool housekeeping_enabled(ਗਿਣਤੀ hk_type type)
{
	ਵਾਪਸ !!(READ_ONCE(housekeeping.flags) & ਦੋਆਧਾਰੀ_ਅੰਕ(type));
}
EXPORT_ਚਿੰਨ੍ਹ_GPL(housekeeping_enabled);

ਸਥਿਰ bool housekeeping_dereference_check(ਗਿਣਤੀ hk_type type)
{
	ਜੇ (IS_ENABLED(CONFIG_LOCKDEP) && type == HK_TYPE_DOMAIN) {
		/* Cpuset isn't even writable yet? */
		ਜੇ (system_state <= SYSTEM_SCHEDULING)
			ਵਾਪਸ true;

		/* CPU hotplug write locked, so cpuset partition can't be overwritten */
		ਜੇ (IS_ENABLED(CONFIG_HOTPLUG_CPU) && lockdep_is_cpus_write_held())
			ਵਾਪਸ true;

		/* Cpuset lock held, partitions not writable */
		ਜੇ (IS_ENABLED(CONFIG_CPUSETS) && lockdep_is_cpuset_held())
			ਵਾਪਸ true;

		ਵਾਪਸ false;
	}

	ਵਾਪਸ true;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ cpumask *housekeeping_cpumask_dereference(ਗਿਣਤੀ hk_type type)
{
	ਵਾਪਸ rcu_dereference_all_check(housekeeping.cpumasks[type],
					 housekeeping_dereference_check(type));
}

ਅਚਲ ਬਣਤਰ cpumask *housekeeping_cpumask(ਗਿਣਤੀ hk_type type)
{
	ਅਚਲ ਬਣਤਰ cpumask *ਨਕਾਬ = ਨਿਰਾ;

	ਜੇ (static_branch_unlikely(&housekeeping_overridden)) {
		ਜੇ (READ_ONCE(housekeeping.flags) & ਦੋਆਧਾਰੀ_ਅੰਕ(type))
			ਨਕਾਬ = housekeeping_cpumask_dereference(type);
	}
	ਜੇ (!ਨਕਾਬ)
		ਨਕਾਬ = cpu_possible_ਨਕਾਬ;
	ਵਾਪਸ ਨਕਾਬ;
}
EXPORT_ਚਿੰਨ੍ਹ_GPL(housekeeping_cpumask);

ਪੂਰਨ housekeeping_any_cpu(ਗਿਣਤੀ hk_type type)
{
	ਪੂਰਨ cpu;

	ਜੇ (static_branch_unlikely(&housekeeping_overridden)) {
		ਜੇ (housekeeping.flags & ਦੋਆਧਾਰੀ_ਅੰਕ(type)) {
			cpu = sched_numa_ਲੱਭੋ_closest(housekeeping_cpumask(type), smp_processor_id());
			ਜੇ (cpu < nr_cpu_ids)
				ਵਾਪਸ cpu;

			cpu = cpumask_any_and_distribute(housekeeping_cpumask(type), cpu_online_ਨਕਾਬ);
			ਜੇ (likely(cpu < nr_cpu_ids))
				ਵਾਪਸ cpu;
			/*
			 * Unless we have another problem this can only happen
			 * at boot time before start_secondary() brings the 1st
			 * housekeeping CPU up.
			 */
			WARN_ON_ONCE(system_state == SYSTEM_RUNNING ||
				     type != HK_TYPE_TIMER);
		}
	}
	ਵਾਪਸ smp_processor_id();
}
EXPORT_ਚਿੰਨ੍ਹ_GPL(housekeeping_any_cpu);

ਖਾਲੀ housekeeping_affine(ਬਣਤਰ task_struct *t, ਗਿਣਤੀ hk_type type)
{
	ਜੇ (static_branch_unlikely(&housekeeping_overridden))
		ਜੇ (housekeeping.flags & ਦੋਆਧਾਰੀ_ਅੰਕ(type))
			ਸੈੱਟ_cpus_allowed_ptr(t, housekeeping_cpumask(type));
}
EXPORT_ਚਿੰਨ੍ਹ_GPL(housekeeping_affine);

bool housekeeping_test_cpu(ਪੂਰਨ cpu, ਗਿਣਤੀ hk_type type)
{
	ਜੇ (static_branch_unlikely(&housekeeping_overridden) &&
	    READ_ONCE(housekeeping.flags) & ਦੋਆਧਾਰੀ_ਅੰਕ(type))
		ਵਾਪਸ cpumask_test_cpu(cpu, housekeeping_cpumask(type));
	ਵਾਪਸ true;
}
EXPORT_ਚਿੰਨ੍ਹ_GPL(housekeeping_test_cpu);

ਪੂਰਨ housekeeping_update(ਬਣਤਰ cpumask *isol_ਨਕਾਬ)
{
	ਬਣਤਰ cpumask *trial, *old = ਨਿਰਾ;
	ਪੂਰਨ err;

	trial = kmalloc(cpumask_ਮਾਪ(), GFP_KERNEL);
	ਜੇ (!trial)
		ਵਾਪਸ -ENOMEM;

	cpumask_andnot(trial, housekeeping_cpumask(HK_TYPE_DOMAIN_BOOT), isol_ਨਕਾਬ);
	ਜੇ (!cpumask_intersects(trial, cpu_online_ਨਕਾਬ)) {
		kfree(trial);
		ਵਾਪਸ -EINVAL;
	}

	ਜੇ (!housekeeping.flags)
		static_branch_enable(&housekeeping_overridden);

	ਜੇ (housekeeping.flags & HK_FLAG_DOMAIN)
		old = housekeeping_cpumask_dereference(HK_TYPE_DOMAIN);
	ਨਹੀਂਤਾਂ
		WRITE_ONCE(housekeeping.flags, housekeeping.flags | HK_FLAG_DOMAIN);
	rcu_ਸੌਂਪੋ_ਸੂਚਕ(housekeeping.cpumasks[HK_TYPE_DOMAIN], trial);

	synchronize_rcu();

	pci_probe_flush_workqueue();
	mem_cgroup_flush_workqueue();
	vmstat_flush_workqueue();

	err = workqueue_unbound_housekeeping_update(housekeeping_cpumask(HK_TYPE_DOMAIN));
	WARN_ON_ONCE(err < 0);

	err = tmigr_isolated_exclude_cpumask(isol_ਨਕਾਬ);
	WARN_ON_ONCE(err < 0);

	err = kthreads_update_housekeeping();
	WARN_ON_ONCE(err < 0);

	kfree(old);

	ਵਾਪਸ 0;
}

ਖਾਲੀ __init housekeeping_init(ਖਾਲੀ)
{
	ਗਿਣਤੀ hk_type type;

	ਜੇ (!housekeeping.flags)
		ਵਾਪਸ;

	static_branch_enable(&housekeeping_overridden);

	ਜੇ (housekeeping.flags & HK_FLAG_KERNEL_NOISE)
		sched_tick_offload_init();
	/*
	 * Realloc with a proper allocator so that any cpumask update
	 * can indifferently free the old version with kfree().
	 */
	for_each_ਸੈੱਟ_ਦੋਆਧਾਰੀ_ਅੰਕ(type, &housekeeping.flags, HK_TYPE_MAX) {
		ਬਣਤਰ cpumask *omask, *nmask = kmalloc(cpumask_ਮਾਪ(), GFP_KERNEL);

		ਜੇ (WARN_ON_ONCE(!nmask))
			ਵਾਪਸ;

		omask = rcu_dereference(housekeeping.cpumasks[type]);

		/* We need at least one CPU to handle housekeeping work */
		WARN_ON_ONCE(cpumask_ਖਾਲੀ_ਥਾਵਾਂ(omask));
		cpumask_copy(nmask, omask);
		RCU_INIT_ਸੂਚਕ(housekeeping.cpumasks[type], nmask);
		__llist_add((ਬਣਤਰ llist_ਗੰਢ *)omask, &memblock_freelist);
	}
}

ਸਥਿਰ ਪੂਰਨ __init housekeeping_late_init(ਖਾਲੀ)
{
	ਬਣਤਰ llist_ਗੰਢ *llnode, *pos, *t;

	/* Free allocated memblock memory, if any */
	llnode = __llist_del_all(&memblock_freelist);
	llist_for_each_ਸੁਰੱਖਿਅਤ(pos, t, llnode)
		memblock_free(pos, cpumask_ਮਾਪ());
	ਵਾਪਸ 0;
}
ਸ਼ੁੱਧ_initcall(housekeeping_late_init);

ਸਥਿਰ ਖਾਲੀ __init housekeeping_ਤਿਆਰੀ_type(ਗਿਣਤੀ hk_type type,
					   cpumask_var_t housekeeping_staging)
{
	ਬਣਤਰ cpumask *ਨਕਾਬ = memblock_alloc_or_panic(cpumask_ਮਾਪ(), SMP_CACHE_ਬਾਈਟਾਂ);

	cpumask_copy(ਨਕਾਬ, housekeeping_staging);
	RCU_INIT_ਸੂਚਕ(housekeeping.cpumasks[type], ਨਕਾਬ);
}

ਸਥਿਰ ਪੂਰਨ __init housekeeping_ਤਿਆਰੀ(ਅੱਖਰ *str, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags)
{
	cpumask_var_t non_housekeeping_ਨਕਾਬ, housekeeping_staging;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਪਹਿਲਾ_cpu;
	ਪੂਰਨ err = 0;

	ਜੇ ((flags & HK_FLAG_KERNEL_NOISE) && !(housekeeping.flags & HK_FLAG_KERNEL_NOISE)) {
		ਜੇ (!IS_ENABLED(CONFIG_NO_HZ_ਭਰਿਆ)) {
			pr_warn("Housekeeping: nohz unsupported."
				" Build with CONFIG_NO_HZ_FULL\n");
			ਵਾਪਸ 0;
		}
	}

	alloc_bootmem_cpumask_var(&non_housekeeping_ਨਕਾਬ);
	ਜੇ (cpulist_parse(str, non_housekeeping_ਨਕਾਬ) < 0) {
		pr_warn("Housekeeping: nohz_full= or isolcpus= incorrect CPU range\n");
		ਜਾਓ free_non_housekeeping_ਨਕਾਬ;
	}

	alloc_bootmem_cpumask_var(&housekeeping_staging);
	cpumask_andnot(housekeeping_staging,
		       cpu_possible_ਨਕਾਬ, non_housekeeping_ਨਕਾਬ);

	ਪਹਿਲਾ_cpu = cpumask_ਪਹਿਲਾ_and(cpu_present_ਨਕਾਬ, housekeeping_staging);
	ਜੇ (ਪਹਿਲਾ_cpu >= nr_cpu_ids || ਪਹਿਲਾ_cpu >= ਤਿਆਰੀ_max_cpus) {
		__cpumask_set_cpu(smp_processor_id(), housekeeping_staging);
		__cpumask_clear_cpu(smp_processor_id(), non_housekeeping_ਨਕਾਬ);
		ਜੇ (!housekeeping.flags) {
			pr_warn("Housekeeping: must include one present CPU, "
				"using boot CPU:%d\n", smp_processor_id());
		}
	}

	ਜੇ (cpumask_ਖਾਲੀ_ਥਾਵਾਂ(non_housekeeping_ਨਕਾਬ))
		ਜਾਓ free_housekeeping_staging;

	ਜੇ (!housekeeping.flags) {
		/* First setup call ("nohz_full=" or "isolcpus=") */
		ਗਿਣਤੀ hk_type type;

		for_each_ਸੈੱਟ_ਦੋਆਧਾਰੀ_ਅੰਕ(type, &flags, HK_TYPE_MAX)
			housekeeping_ਤਿਆਰੀ_type(type, housekeeping_staging);
	} ਨਹੀਂਤਾਂ {
		/* Second setup call ("nohz_full=" after "isolcpus=" or the reverse) */
		ਗਿਣਤੀ hk_type type;
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ iter_flags = flags & housekeeping.flags;

		for_each_ਸੈੱਟ_ਦੋਆਧਾਰੀ_ਅੰਕ(type, &iter_flags, HK_TYPE_MAX) {
			ਜੇ (!cpumask_equal(housekeeping_staging,
					   housekeeping_cpumask(type))) {
				pr_warn("Housekeeping: nohz_full= must match isolcpus=\n");
				ਜਾਓ free_housekeeping_staging;
			}
		}

		/*
		 * Check the combination of nohz_full and isolcpus=domain,
		 * necessary to avoid problems with the timer migration
		 * hierarchy. managed_irq is ignored by this check since it
		 * isn't considered in the timer migration logic.
		 */
		iter_flags = housekeeping.flags & (HK_FLAG_KERNEL_NOISE | HK_FLAG_DOMAIN);
		type = ਲੱਭੋ_ਪਹਿਲਾ_ਦੋਆਧਾਰੀ_ਅੰਕ(&iter_flags, HK_TYPE_MAX);
		/*
		 * Pass the check if none of these flags were previously set or
		 * are not in the current selection.
		 */
		iter_flags = flags & (HK_FLAG_KERNEL_NOISE | HK_FLAG_DOMAIN);
		ਪਹਿਲਾ_cpu = (type == HK_TYPE_MAX || !iter_flags) ? 0 :
			    cpumask_ਪਹਿਲਾ_and_and(cpu_present_ਨਕਾਬ,
						  housekeeping_staging, housekeeping_cpumask(type));
		ਜੇ (ਪਹਿਲਾ_cpu >= min(nr_cpu_ids, ਤਿਆਰੀ_max_cpus)) {
			pr_warn("Housekeeping: must include one present CPU "
				"neither in nohz_full= nor in isolcpus=domain, "
				"ignoring setting %s\n", str);
			ਜਾਓ free_housekeeping_staging;
		}

		iter_flags = flags & ~housekeeping.flags;

		for_each_ਸੈੱਟ_ਦੋਆਧਾਰੀ_ਅੰਕ(type, &iter_flags, HK_TYPE_MAX)
			housekeeping_ਤਿਆਰੀ_type(type, housekeeping_staging);
	}

	ਜੇ ((flags & HK_FLAG_KERNEL_NOISE) && !(housekeeping.flags & HK_FLAG_KERNEL_NOISE))
		tick_nohz_ਭਰਿਆ_ਤਿਆਰੀ(non_housekeeping_ਨਕਾਬ);

	housekeeping.flags |= flags;
	err = 1;

free_housekeeping_staging:
	free_bootmem_cpumask_var(housekeeping_staging);
free_non_housekeeping_ਨਕਾਬ:
	free_bootmem_cpumask_var(non_housekeeping_ਨਕਾਬ);

	ਵਾਪਸ err;
}

ਸਥਿਰ ਪੂਰਨ __init housekeeping_nohz_ਭਰਿਆ_ਤਿਆਰੀ(ਅੱਖਰ *str)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags;

	flags = HK_FLAG_KERNEL_NOISE;

	ਵਾਪਸ housekeeping_ਤਿਆਰੀ(str, flags);
}
__setup("nohz_full=", housekeeping_nohz_ਭਰਿਆ_ਤਿਆਰੀ);

ਸਥਿਰ ਪੂਰਨ __init housekeeping_isolcpus_ਤਿਆਰੀ(ਅੱਖਰ *str)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags = 0;
	bool illegal = false;
	ਅੱਖਰ *par;
	ਪੂਰਨ len;

	ਜਦੋਂ (ਅੱਖਰ_ਹੈ(*str)) {
		/*
		 * isolcpus=nohz is equivalent to nohz_full.
		 */
		ਜੇ (!ਲੜੀ_ਤੁਲਨਾ(str, "nohz,", 5)) {
			str += 5;
			flags |= HK_FLAG_KERNEL_NOISE;
			ਜਾਰੀ;
		}

		ਜੇ (!ਲੜੀ_ਤੁਲਨਾ(str, "domain,", 7)) {
			str += 7;
			flags |= HK_FLAG_DOMAIN | HK_FLAG_DOMAIN_BOOT;
			ਜਾਰੀ;
		}

		ਜੇ (!ਲੜੀ_ਤੁਲਨਾ(str, "managed_irq,", 12)) {
			str += 12;
			flags |= HK_FLAG_MANAGED_IRQ;
			ਜਾਰੀ;
		}

		/*
		 * Skip unknown sub-parameter and validate that it is not
		 * containing an invalid character.
		 */
		ਲਈ (par = str, len = 0; *str && *str != ','; str++, len++) {
			ਜੇ (!ਅੱਖਰ_ਹੈ(*str) && *str != '_')
				illegal = true;
		}

		ਜੇ (illegal) {
			pr_warn("isolcpus: Invalid flag %.*s\n", len, par);
			ਵਾਪਸ 0;
		}

		pr_info("isolcpus: Skipped unknown flag %.*s\n", len, par);
		str++;
	}

	/* Default behaviour for isolcpus without flags */
	ਜੇ (!flags)
		flags |= HK_FLAG_DOMAIN | HK_FLAG_DOMAIN_BOOT;

	ਵਾਪਸ housekeeping_ਤਿਆਰੀ(str, flags);
}
__setup("isolcpus=", housekeeping_isolcpus_ਤਿਆਰੀ);
