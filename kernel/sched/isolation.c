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
	HK_FLAG_DOMAIN_BOOT	= بټ(HK_TYPE_DOMAIN_BOOT),
	HK_FLAG_DOMAIN		= بټ(HK_TYPE_DOMAIN),
	HK_FLAG_MANAGED_IRQ	= بټ(HK_TYPE_MANAGED_IRQ),
	HK_FLAG_KERNEL_NOISE	= بټ(HK_TYPE_KERNEL_NOISE),
};

DEFINE_STATIC_کلی_FALSE(housekeeping_overridden);
EXPORT_نښه_GPL(housekeeping_overridden);

بنية housekeeping {
	بنية cpumask __rcu *cpumasks[HK_TYPE_MAX];
	بینښه اوږد flags;
};

ساكن بنية housekeeping housekeeping;
ساكن __initdata LLIST_سر(memblock_freelist);

bool housekeeping_enabled(تعداد hk_type type)
{
	أرجع !!(READ_ONCE(housekeeping.flags) & بټ(type));
}
EXPORT_نښه_GPL(housekeeping_enabled);

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
	ثابت بنية cpumask *ماسک = عدم;

	إذا (static_branch_unlikely(&housekeeping_overridden)) {
		إذا (READ_ONCE(housekeeping.flags) & بټ(type))
			ماسک = housekeeping_cpumask_dereference(type);
	}
	إذا (!ماسک)
		ماسک = cpu_possible_ماسک;
	أرجع ماسک;
}
EXPORT_نښه_GPL(housekeeping_cpumask);

صحيح housekeeping_any_cpu(تعداد hk_type type)
{
	صحيح cpu;

	إذا (static_branch_unlikely(&housekeeping_overridden)) {
		إذا (housekeeping.flags & بټ(type)) {
			cpu = sched_numa_پیدا_کړه_closest(housekeeping_cpumask(type), smp_processor_id());
			إذا (cpu < nr_cpu_ids)
				أرجع cpu;

			cpu = cpumask_any_and_distribute(housekeeping_cpumask(type), cpu_online_ماسک);
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
EXPORT_نښه_GPL(housekeeping_any_cpu);

فراغ housekeeping_affine(بنية task_struct *t, تعداد hk_type type)
{
	إذا (static_branch_unlikely(&housekeeping_overridden))
		إذا (housekeeping.flags & بټ(type))
			سېټ_cpus_allowed_ptr(t, housekeeping_cpumask(type));
}
EXPORT_نښه_GPL(housekeeping_affine);

bool housekeeping_test_cpu(صحيح cpu, تعداد hk_type type)
{
	إذا (static_branch_unlikely(&housekeeping_overridden) &&
	    READ_ONCE(housekeeping.flags) & بټ(type))
		أرجع cpumask_test_cpu(cpu, housekeeping_cpumask(type));
	أرجع true;
}
EXPORT_نښه_GPL(housekeeping_test_cpu);

صحيح housekeeping_update(بنية cpumask *isol_ماسک)
{
	بنية cpumask *trial, *old = عدم;
	صحيح err;

	trial = kmalloc(cpumask_اندازه(), GFP_KERNEL);
	إذا (!trial)
		أرجع -ENOMEM;

	cpumask_andnot(trial, housekeeping_cpumask(HK_TYPE_DOMAIN_BOOT), isol_ماسک);
	إذا (!cpumask_intersects(trial, cpu_online_ماسک)) {
		kfree(trial);
		أرجع -EINVAL;
	}

	إذا (!housekeeping.flags)
		static_branch_enable(&housekeeping_overridden);

	إذا (housekeeping.flags & HK_FLAG_DOMAIN)
		old = housekeeping_cpumask_dereference(HK_TYPE_DOMAIN);
	وإلا
		WRITE_ONCE(housekeeping.flags, housekeeping.flags | HK_FLAG_DOMAIN);
	rcu_ټاکل_اشاره_ګر(housekeeping.cpumasks[HK_TYPE_DOMAIN], trial);

	synchronize_rcu();

	pci_probe_flush_workqueue();
	mem_cgroup_flush_workqueue();
	vmstat_flush_workqueue();

	err = workqueue_unbound_housekeeping_update(housekeeping_cpumask(HK_TYPE_DOMAIN));
	WARN_ON_ONCE(err < 0);

	err = tmigr_isolated_exclude_cpumask(isol_ماسک);
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
	for_each_سېټ_بټ(type, &housekeeping.flags, HK_TYPE_MAX) {
		بنية cpumask *omask, *nmask = kmalloc(cpumask_اندازه(), GFP_KERNEL);

		إذا (WARN_ON_ONCE(!nmask))
			أرجع;

		omask = rcu_dereference(housekeeping.cpumasks[type]);

		/* We need at least one CPU to handle housekeeping work */
		WARN_ON_ONCE(cpumask_تشه(omask));
		cpumask_copy(nmask, omask);
		RCU_INIT_اشاره_ګر(housekeeping.cpumasks[type], nmask);
		__llist_add((بنية llist_غوټه *)omask, &memblock_freelist);
	}
}

ساكن صحيح __init housekeeping_late_init(فراغ)
{
	بنية llist_غوټه *llnode, *pos, *t;

	/* Free allocated memblock memory, if any */
	llnode = __llist_del_all(&memblock_freelist);
	llist_for_each_خوندي(pos, t, llnode)
		memblock_free(pos, cpumask_اندازه());
	أرجع 0;
}
خالص_initcall(housekeeping_late_init);

ساكن فراغ __init housekeeping_تیاری_type(تعداد hk_type type,
					   cpumask_var_t housekeeping_staging)
{
	بنية cpumask *ماسک = memblock_alloc_or_panic(cpumask_اندازه(), SMP_CACHE_بایټونه);

	cpumask_copy(ماسک, housekeeping_staging);
	RCU_INIT_اشاره_ګر(housekeeping.cpumasks[type], ماسک);
}

ساكن صحيح __init housekeeping_تیاری(حرف *str, بینښه اوږد flags)
{
	cpumask_var_t non_housekeeping_ماسک, housekeeping_staging;
	بینښه صحيح لومړی_cpu;
	صحيح err = 0;

	إذا ((flags & HK_FLAG_KERNEL_NOISE) && !(housekeeping.flags & HK_FLAG_KERNEL_NOISE)) {
		إذا (!IS_ENABLED(CONFIG_NO_HZ_ډک)) {
			pr_warn("Housekeeping: nohz unsupported."
				" Build with CONFIG_NO_HZ_FULL\n");
			أرجع 0;
		}
	}

	alloc_bootmem_cpumask_var(&non_housekeeping_ماسک);
	إذا (cpulist_parse(str, non_housekeeping_ماسک) < 0) {
		pr_warn("Housekeeping: nohz_full= or isolcpus= incorrect CPU range\n");
		اذهب free_non_housekeeping_ماسک;
	}

	alloc_bootmem_cpumask_var(&housekeeping_staging);
	cpumask_andnot(housekeeping_staging,
		       cpu_possible_ماسک, non_housekeeping_ماسک);

	لومړی_cpu = cpumask_لومړی_and(cpu_present_ماسک, housekeeping_staging);
	إذا (لومړی_cpu >= nr_cpu_ids || لومړی_cpu >= تیاری_max_cpus) {
		__cpumask_set_cpu(smp_processor_id(), housekeeping_staging);
		__cpumask_clear_cpu(smp_processor_id(), non_housekeeping_ماسک);
		إذا (!housekeeping.flags) {
			pr_warn("Housekeeping: must include one present CPU, "
				"using boot CPU:%d\n", smp_processor_id());
		}
	}

	إذا (cpumask_تشه(non_housekeeping_ماسک))
		اذهب free_housekeeping_staging;

	إذا (!housekeeping.flags) {
		/* First setup call ("nohz_full=" or "isolcpus=") */
		تعداد hk_type type;

		for_each_سېټ_بټ(type, &flags, HK_TYPE_MAX)
			housekeeping_تیاری_type(type, housekeeping_staging);
	} وإلا {
		/* Second setup call ("nohz_full=" after "isolcpus=" or the reverse) */
		تعداد hk_type type;
		بینښه اوږد iter_flags = flags & housekeeping.flags;

		for_each_سېټ_بټ(type, &iter_flags, HK_TYPE_MAX) {
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
		type = پیدا_کړه_لومړی_بټ(&iter_flags, HK_TYPE_MAX);
		/*
		 * Pass the check if none of these flags were previously set or
		 * are not in the current selection.
		 */
		iter_flags = flags & (HK_FLAG_KERNEL_NOISE | HK_FLAG_DOMAIN);
		لومړی_cpu = (type == HK_TYPE_MAX || !iter_flags) ? 0 :
			    cpumask_لومړی_and_and(cpu_present_ماسک,
						  housekeeping_staging, housekeeping_cpumask(type));
		إذا (لومړی_cpu >= min(nr_cpu_ids, تیاری_max_cpus)) {
			pr_warn("Housekeeping: must include one present CPU "
				"neither in nohz_full= nor in isolcpus=domain, "
				"ignoring setting %s\n", str);
			اذهب free_housekeeping_staging;
		}

		iter_flags = flags & ~housekeeping.flags;

		for_each_سېټ_بټ(type, &iter_flags, HK_TYPE_MAX)
			housekeeping_تیاری_type(type, housekeeping_staging);
	}

	إذا ((flags & HK_FLAG_KERNEL_NOISE) && !(housekeeping.flags & HK_FLAG_KERNEL_NOISE))
		tick_nohz_ډک_تیاری(non_housekeeping_ماسک);

	housekeeping.flags |= flags;
	err = 1;

free_housekeeping_staging:
	free_bootmem_cpumask_var(housekeeping_staging);
free_non_housekeeping_ماسک:
	free_bootmem_cpumask_var(non_housekeeping_ماسک);

	أرجع err;
}

ساكن صحيح __init housekeeping_nohz_ډک_تیاری(حرف *str)
{
	بینښه اوږد flags;

	flags = HK_FLAG_KERNEL_NOISE;

	أرجع housekeeping_تیاری(str, flags);
}
__setup("nohz_full=", housekeeping_nohz_ډک_تیاری);

ساكن صحيح __init housekeeping_isolcpus_تیاری(حرف *str)
{
	بینښه اوږد flags = 0;
	bool illegal = false;
	حرف *par;
	صحيح len;

	طالما (که_توری(*str)) {
		/*
		 * isolcpus=nohz is equivalent to nohz_full.
		 */
		إذا (!مزي_پرتله(str, "nohz,", 5)) {
			str += 5;
			flags |= HK_FLAG_KERNEL_NOISE;
			استمر;
		}

		إذا (!مزي_پرتله(str, "domain,", 7)) {
			str += 7;
			flags |= HK_FLAG_DOMAIN | HK_FLAG_DOMAIN_BOOT;
			استمر;
		}

		إذا (!مزي_پرتله(str, "managed_irq,", 12)) {
			str += 12;
			flags |= HK_FLAG_MANAGED_IRQ;
			استمر;
		}

		/*
		 * Skip unknown sub-parameter and validate that it is not
		 * containing an invalid character.
		 */
		لكل (par = str, len = 0; *str && *str != ','; str++, len++) {
			إذا (!که_توری(*str) && *str != '_')
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

	أرجع housekeeping_تیاری(str, flags);
}
__setup("isolcpus=", housekeeping_isolcpus_تیاری);
