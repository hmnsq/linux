// SPDX-License-Identifier: GPL-2.0
/*
 * Scheduler topology setup/handling methods
 */

#অন্তর্ভাব <linux/sched/isolation.h>
#অন্তর্ভাব <linux/sched/clock.h>
#অন্তর্ভাব <linux/bsearch.h>
#অন্তর্ভাব "sched.h"

DEFINE_MUTEX(sched_domains_mutex);
শূন্য sched_domains_mutex_lock(শূন্য)
{
	mutex_lock(&sched_domains_mutex);
}
শূন্য sched_domains_mutex_unlock(শূন্য)
{
	mutex_unlock(&sched_domains_mutex);
}

/* Protected by sched_domains_mutex: */
স্থির cpumask_var_t sched_domains_llc_id_allocmask;
স্থির cpumask_var_t sched_domains_tmpmask;
স্থির cpumask_var_t sched_domains_tmpmask2;
পূর্ণসংখ্যা max_lid;

স্থির পূর্ণসংখ্যা __init sched_debug_setup(অক্ষর *str)
{
	sched_debug_verbose = true;

	ফেরত 0;
}
early_param("sched_verbose", sched_debug_setup);

#ifdef CONFIG_SCHED_SMT
স্থির bool sched_smt_asym_packing __read_mostly;

স্থির পূর্ণসংখ্যা __init setup_sched_smt_asym_packing(অক্ষর *str)
{
	যদি (strcmp(str, "on"))
		ফেরত 0;

	sched_smt_asym_packing = true;
	ফেরত 1;
}
__setup("sched_smt_asym_packing=", setup_sched_smt_asym_packing);
#endif

স্থির অন্তঃরেখ bool sched_debug(শূন্য)
{
	ফেরত sched_debug_verbose;
}

#define SD_FLAG(_name, mflags) [__##_name] = { .meta_flags = mflags, .name = #_name },
ধ্রুবক কাঠামো sd_flag_debug sd_flag_debug[] = {
#অন্তর্ভাব <linux/sched/sd_flags.h>
};
#undef SD_FLAG

স্থির পূর্ণসংখ্যা sched_domain_debug_one(কাঠামো sched_domain *sd, পূর্ণসংখ্যা cpu, পূর্ণসংখ্যা level,
				  কাঠামো cpumask *groupmask)
{
	কাঠামো sched_group *group = sd->groups;
	unsigned long flags = sd->flags;
	unsigned পূর্ণসংখ্যা idx;

	cpumask_clear(groupmask);

	printk(KERN_DEBUG "%*s domain-%d: ", level, "", level);
	printk(KERN_CONT "span=%*pbl level=%s\n",
	       cpumask_pr_args(sched_domain_span(sd)), sd->name);

	যদি (!cpumask_test_cpu(cpu, sched_domain_span(sd))) {
		printk(KERN_ERR "ERROR: domain->span does not contain CPU%d\n", cpu);
	}
	যদি (group && !cpumask_test_cpu(cpu, sched_group_span(group))) {
		printk(KERN_ERR "ERROR: domain->groups does not contain CPU%d\n", cpu);
	}

	for_each_set_bit(idx, &flags, __SD_FLAG_CNT) {
		unsigned পূর্ণসংখ্যা flag = BIT(idx);
		unsigned পূর্ণসংখ্যা meta_flags = sd_flag_debug[idx].meta_flags;

		যদি ((meta_flags & SDF_SHARED_CHILD) && sd->child &&
		    !(sd->child->flags & flag))
			printk(KERN_ERR "ERROR: flag %s set here but not in child\n",
			       sd_flag_debug[idx].name);

		যদি ((meta_flags & SDF_SHARED_PARENT) && sd->parent &&
		    !(sd->parent->flags & flag))
			printk(KERN_ERR "ERROR: flag %s set here but not in parent\n",
			       sd_flag_debug[idx].name);
	}

	printk(KERN_DEBUG "%*s groups:", level + 1, "");
	করো {
		যদি (!group) {
			printk("\n");
			printk(KERN_ERR "ERROR: group is NULL\n");
			বিরতি;
		}

		যদি (cpumask_empty(sched_group_span(group))) {
			printk(KERN_CONT "\n");
			printk(KERN_ERR "ERROR: empty group\n");
			বিরতি;
		}

		যদি (!(sd->flags & SD_NUMA) &&
		    cpumask_intersects(groupmask, sched_group_span(group))) {
			printk(KERN_CONT "\n");
			printk(KERN_ERR "ERROR: repeated CPUs\n");
			বিরতি;
		}

		cpumask_or(groupmask, groupmask, sched_group_span(group));

		printk(KERN_CONT " %d:{ span=%*pbl",
				group->sgc->id,
				cpumask_pr_args(sched_group_span(group)));

		যদি ((sd->flags & SD_NUMA) &&
		    !cpumask_equal(group_balance_mask(group), sched_group_span(group))) {
			printk(KERN_CONT " mask=%*pbl",
				cpumask_pr_args(group_balance_mask(group)));
		}

		যদি (group->sgc->capacity != SCHED_CAPACITY_SCALE)
			printk(KERN_CONT " cap=%lu", group->sgc->capacity);

		যদি (group == sd->groups && sd->child &&
		    !cpumask_equal(sched_domain_span(sd->child),
				   sched_group_span(group))) {
			printk(KERN_ERR "ERROR: domain->groups does not match domain->child\n");
		}

		printk(KERN_CONT " }");

		group = group->পরবর্তী;

		যদি (group != sd->groups)
			printk(KERN_CONT ",");

	} যখন (group != sd->groups);
	printk(KERN_CONT "\n");

	যদি (!cpumask_equal(sched_domain_span(sd), groupmask))
		printk(KERN_ERR "ERROR: groups don't span domain->span\n");

	যদি (sd->parent &&
	    !cpumask_subset(groupmask, sched_domain_span(sd->parent)))
		printk(KERN_ERR "ERROR: parent span is not a superset of domain->span\n");
	ফেরত 0;
}

স্থির শূন্য sched_domain_debug(কাঠামো sched_domain *sd, পূর্ণসংখ্যা cpu)
{
	পূর্ণসংখ্যা level = 0;

	যদি (!sched_debug_verbose)
		ফেরত;

	যদি (!sd) {
		printk(KERN_DEBUG "CPU%d attaching NULL sched-domain.\n", cpu);
		ফেরত;
	}

	printk(KERN_DEBUG "CPU%d attaching sched-domain(s):\n", cpu);

	জন্য (;;) {
		যদি (sched_domain_debug_one(sd, cpu, level, sched_domains_tmpmask))
			বিরতি;
		level++;
		sd = sd->parent;
		যদি (!sd)
			বিরতি;
	}
}

/* Generate a mask of SD flags with the SDF_NEEDS_GROUPS metaflag */
#define SD_FLAG(name, mflags) (name * !!((mflags) & SDF_NEEDS_GROUPS)) |
স্থির ধ্রুবক unsigned পূর্ণসংখ্যা SD_DEGENERATE_GROUPS_MASK =
#অন্তর্ভাব <linux/sched/sd_flags.h>
0;
#undef SD_FLAG

স্থির পূর্ণসংখ্যা sd_degenerate(কাঠামো sched_domain *sd)
{
	যদি (cpumask_weight(sched_domain_span(sd)) == 1)
		ফেরত 1;

	/* Following flags need at least 2 groups */
	যদি ((sd->flags & SD_DEGENERATE_GROUPS_MASK) &&
	    (sd->groups != sd->groups->পরবর্তী))
		ফেরত 0;

	/* Following flags don't use groups */
	যদি (sd->flags & (SD_WAKE_AFFINE))
		ফেরত 0;

	ফেরত 1;
}

স্থির পূর্ণসংখ্যা
sd_parent_degenerate(কাঠামো sched_domain *sd, কাঠামো sched_domain *parent)
{
	unsigned long cflags = sd->flags, pflags = parent->flags;

	যদি (sd_degenerate(parent))
		ফেরত 1;

	যদি (!cpumask_equal(sched_domain_span(sd), sched_domain_span(parent)))
		ফেরত 0;

	/* Flags needing groups don't count if only 1 group in parent */
	যদি (parent->groups == parent->groups->পরবর্তী)
		pflags &= ~SD_DEGENERATE_GROUPS_MASK;

	যদি (~cflags & pflags)
		ফেরত 0;

	ফেরত 1;
}

#যদি defined(CONFIG_ENERGY_MODEL) && defined(CONFIG_CPU_FREQ_GOV_SCHEDUTIL)
DEFINE_STATIC_চাবি_FALSE(sched_energy_present);
স্থির unsigned পূর্ণসংখ্যা sysctl_sched_energy_aware = 1;
স্থির DEFINE_MUTEX(sched_energy_mutex);
স্থির bool sched_energy_update;

স্থির bool sched_is_eas_possible(ধ্রুবক কাঠামো cpumask *cpu_mask)
{
	bool any_asym_capacity = false;
	পূর্ণসংখ্যা i;

	/* EAS is enabled for asymmetric CPU capacity topologies. */
	for_each_cpu(i, cpu_mask) {
		যদি (rcu_access_pointer(per_cpu(sd_asym_cpucapacity, i))) {
			any_asym_capacity = true;
			বিরতি;
		}
	}
	যদি (!any_asym_capacity) {
		যদি (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS, CPUs do not have asymmetric capacities\n",
				cpumask_pr_args(cpu_mask));
		}
		ফেরত false;
	}

	/* EAS definitely does *not* handle SMT */
	যদি (sched_smt_active()) {
		যদি (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS, SMT is not supported\n",
				cpumask_pr_args(cpu_mask));
		}
		ফেরত false;
	}

	যদি (!arch_scale_freq_invariant()) {
		যদি (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS: frequency-invariant load tracking not yet supported",
				cpumask_pr_args(cpu_mask));
		}
		ফেরত false;
	}

	যদি (!cpufreq_ready_for_eas(cpu_mask)) {
		যদি (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS: cpufreq is not ready\n",
				cpumask_pr_args(cpu_mask));
		}
		ফেরত false;
	}

	ফেরত true;
}

শূন্য rebuild_sched_domains_energy(শূন্য)
{
	mutex_lock(&sched_energy_mutex);
	sched_energy_update = true;
	rebuild_sched_domains();
	sched_energy_update = false;
	mutex_unlock(&sched_energy_mutex);
}

#ifdef CONFIG_SYSCTL
স্থির পূর্ণসংখ্যা sched_energy_aware_handler(ধ্রুবক কাঠামো ctl_table *table, পূর্ণসংখ্যা লিখ,
		শূন্য *বাফার, আকার_t *lenp, loff_t *ppos)
{
	পূর্ণসংখ্যা ret;

	যদি (লিখ && !capable(CAP_SYS_ADMIN))
		ফেরত -EPERM;

	যদি (!sched_is_eas_possible(cpu_active_mask)) {
		যদি (লিখ) {
			ফেরত -EOPNOTSUPP;
		} অন্যথা {
			*lenp = 0;
			ফেরত 0;
		}
	}

	ret = proc_dointvec_minmax(table, লিখ, বাফার, lenp, ppos);
	যদি (!ret && লিখ) {
		যদি (sysctl_sched_energy_aware != sched_energy_enabled())
			rebuild_sched_domains_energy();
	}

	ফেরত ret;
}

স্থির ধ্রুবক কাঠামো ctl_table sched_energy_aware_sysctls[] = {
	{
		.procname       = "sched_energy_aware",
		.তথ্য           = &sysctl_sched_energy_aware,
		.maxlen         = আকার(unsigned পূর্ণসংখ্যা),
		.mode           = 0644,
		.proc_handler   = sched_energy_aware_handler,
		.extra1         = SYSCTL_ZERO,
		.extra2         = SYSCTL_ONE,
	},
};

স্থির পূর্ণসংখ্যা __init sched_energy_aware_sysctl_init(শূন্য)
{
	register_sysctl_init("kernel", sched_energy_aware_sysctls);
	ফেরত 0;
}

late_initcall(sched_energy_aware_sysctl_init);
#endif /* CONFIG_SYSCTL */

স্থির শূন্য free_pd(কাঠামো perf_domain *pd)
{
	কাঠামো perf_domain *tmp;

	যখন (pd) {
		tmp = pd->পরবর্তী;
		kfree(pd);
		pd = tmp;
	}
}

স্থির কাঠামো perf_domain *find_pd(কাঠামো perf_domain *pd, পূর্ণসংখ্যা cpu)
{
	যখন (pd) {
		যদি (cpumask_test_cpu(cpu, perf_domain_span(pd)))
			ফেরত pd;
		pd = pd->পরবর্তী;
	}

	ফেরত ফাঁকা;
}

স্থির কাঠামো perf_domain *pd_init(পূর্ণসংখ্যা cpu)
{
	কাঠামো em_perf_domain *obj = em_cpu_get(cpu);
	কাঠামো perf_domain *pd;

	যদি (!obj) {
		যদি (sched_debug())
			pr_info("%s: no EM found for CPU%d\n", __func__, cpu);
		ফেরত ফাঁকা;
	}

	pd = kzalloc_obj(*pd);
	যদি (!pd)
		ফেরত ফাঁকা;
	pd->em_pd = obj;

	ফেরত pd;
}

স্থির শূন্য perf_domain_debug(ধ্রুবক কাঠামো cpumask *cpu_map,
						কাঠামো perf_domain *pd)
{
	যদি (!sched_debug() || !pd)
		ফেরত;

	printk(KERN_DEBUG "root_domain %*pbl:", cpumask_pr_args(cpu_map));

	যখন (pd) {
		printk(KERN_CONT " pd%d:{ cpus=%*pbl nr_pstate=%d }",
				cpumask_first(perf_domain_span(pd)),
				cpumask_pr_args(perf_domain_span(pd)),
				em_pd_nr_perf_states(pd->em_pd));
		pd = pd->পরবর্তী;
	}

	printk(KERN_CONT "\n");
}

স্থির শূন্য destroy_perf_domain_rcu(কাঠামো rcu_শীর্ষ *rp)
{
	কাঠামো perf_domain *pd;

	pd = container_of(rp, কাঠামো perf_domain, rcu);
	free_pd(pd);
}

স্থির শূন্য sched_energy_set(bool has_eas)
{
	যদি (!has_eas && sched_energy_enabled()) {
		যদি (sched_debug())
			pr_info("%s: stopping EAS\n", __func__);
		static_branch_disable_cpuslocked(&sched_energy_present);
	} অন্যথা যদি (has_eas && !sched_energy_enabled()) {
		যদি (sched_debug())
			pr_info("%s: starting EAS\n", __func__);
		static_branch_enable_cpuslocked(&sched_energy_present);
	}
}

/*
 * EAS can be used on a root domain if it meets all the following conditions:
 *    1. an Energy Model (EM) is available;
 *    2. the SD_ASYM_CPUCAPACITY flag is set in the sched_domain hierarchy.
 *    3. no SMT is detected.
 *    4. schedutil is driving the frequency of all CPUs of the rd;
 *    5. frequency invariance support is present;
 */
স্থির bool build_perf_domains(ধ্রুবক কাঠামো cpumask *cpu_map)
{
	পূর্ণসংখ্যা i;
	কাঠামো perf_domain *pd = ফাঁকা, *tmp;
	পূর্ণসংখ্যা cpu = cpumask_first(cpu_map);
	কাঠামো মূল_domain *rd = cpu_rq(cpu)->rd;

	যদি (!sysctl_sched_energy_aware)
		যাও মুক্ত;

	যদি (!sched_is_eas_possible(cpu_map))
		যাও মুক্ত;

	for_each_cpu(i, cpu_map) {
		/* Skip already covered CPUs. */
		যদি (find_pd(pd, i))
			চালু;

		/* Create the new pd and add it to the local list. */
		tmp = pd_init(i);
		যদি (!tmp)
			যাও মুক্ত;
		tmp->পরবর্তী = pd;
		pd = tmp;
	}

	perf_domain_debug(cpu_map, pd);

	/* Attach the new list of performance domains to the root domain. */
	tmp = rd->pd;
	rcu_assign_pointer(rd->pd, pd);
	যদি (tmp)
		call_rcu(&tmp->rcu, destroy_perf_domain_rcu);

	ফেরত !!pd;

মুক্ত:
	free_pd(pd);
	tmp = rd->pd;
	rcu_assign_pointer(rd->pd, ফাঁকা);
	যদি (tmp)
		call_rcu(&tmp->rcu, destroy_perf_domain_rcu);

	ফেরত false;
}
#অন্যথা /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL): */
স্থির শূন্য free_pd(কাঠামো perf_domain *pd) { }
#endif /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL) */

স্থির শূন্য free_rootdomain(কাঠামো rcu_শীর্ষ *rcu)
{
	কাঠামো মূল_domain *rd = container_of(rcu, কাঠামো মূল_domain, rcu);

	cpupri_cleanup(&rd->cpupri);
	cpudl_cleanup(&rd->cpudl);
	free_cpumask_var(rd->dlo_mask);
	free_cpumask_var(rd->rto_mask);
	free_cpumask_var(rd->online);
	free_cpumask_var(rd->span);
	free_pd(rd->pd);
	kfree(rd);
}

শূন্য rq_attach_মূল(কাঠামো rq *rq, কাঠামো মূল_domain *rd)
{
	কাঠামো মূল_domain *old_rd = ফাঁকা;
	কাঠামো rq_flags rf;

	rq_lock_irqsave(rq, &rf);

	যদি (rq->rd) {
		old_rd = rq->rd;

		যদি (cpumask_test_cpu(rq->cpu, old_rd->online))
			set_rq_offline(rq);

		cpumask_clear_cpu(rq->cpu, old_rd->span);

		/*
		 * If we don't want to free the old_rd yet then
		 * set old_rd to NULL to skip the freeing later
		 * in this function:
		 */
		যদি (!atomic_dec_and_test(&old_rd->refcount))
			old_rd = ফাঁকা;
	}

	atomic_inc(&rd->refcount);
	rq->rd = rd;

	cpumask_set_cpu(rq->cpu, rd->span);
	যদি (cpumask_test_cpu(rq->cpu, cpu_active_mask))
		set_rq_online(rq);

	/*
	 * Because the rq is not a task, dl_add_task_root_domain() did not
	 * move the fair server bw to the rd if it already started.
	 * Add it now.
	 */
	যদি (rq->fair_server.dl_server)
		__dl_server_attach_root(&rq->fair_server, rq);

#ifdef CONFIG_SCHED_CLASS_EXT
	যদি (rq->ext_server.dl_server)
		__dl_server_attach_root(&rq->ext_server, rq);
#endif

	rq_unlock_irqrestore(rq, &rf);

	যদি (old_rd)
		call_rcu(&old_rd->rcu, free_rootdomain);
}

শূন্য sched_get_rd(কাঠামো মূল_domain *rd)
{
	atomic_inc(&rd->refcount);
}

শূন্য sched_put_rd(কাঠামো মূল_domain *rd)
{
	যদি (!atomic_dec_and_test(&rd->refcount))
		ফেরত;

	call_rcu(&rd->rcu, free_rootdomain);
}

স্থির পূর্ণসংখ্যা init_rootdomain(কাঠামো মূল_domain *rd)
{
	যদি (!zalloc_cpumask_var(&rd->span, GFP_KERNEL))
		যাও out;
	যদি (!zalloc_cpumask_var(&rd->online, GFP_KERNEL))
		যাও free_span;
	যদি (!zalloc_cpumask_var(&rd->dlo_mask, GFP_KERNEL))
		যাও free_online;
	যদি (!zalloc_cpumask_var(&rd->rto_mask, GFP_KERNEL))
		যাও free_dlo_mask;

#ifdef HAVE_RT_ঠেলো_IPI
	rd->rto_cpu = -1;
	raw_spin_lock_init(&rd->rto_lock);
	rd->rto_ঠেলো_work = IRQ_WORK_INIT_HARD(rto_ঠেলো_irq_work_func);
#endif

	rd->visit_cookie = 0;
	init_dl_bw(&rd->dl_bw);
	যদি (cpudl_init(&rd->cpudl) != 0)
		যাও free_rto_mask;

	যদি (cpupri_init(&rd->cpupri) != 0)
		যাও free_cpudl;
	ফেরত 0;

free_cpudl:
	cpudl_cleanup(&rd->cpudl);
free_rto_mask:
	free_cpumask_var(rd->rto_mask);
free_dlo_mask:
	free_cpumask_var(rd->dlo_mask);
free_online:
	free_cpumask_var(rd->online);
free_span:
	free_cpumask_var(rd->span);
out:
	ফেরত -ENOMEM;
}

/*
 * By default the system creates a single root-domain with all CPUs as
 * members (mimicking the global state we have today).
 */
কাঠামো মূল_domain def_মূল_domain;

শূন্য __init init_defrootdomain(শূন্য)
{
	init_rootdomain(&def_মূল_domain);

	atomic_set(&def_মূল_domain.refcount, 1);
}

স্থির কাঠামো মূল_domain *alloc_rootdomain(শূন্য)
{
	কাঠামো মূল_domain *rd;

	rd = kzalloc_obj(*rd);
	যদি (!rd)
		ফেরত ফাঁকা;

	যদি (init_rootdomain(rd) != 0) {
		kfree(rd);
		ফেরত ফাঁকা;
	}

	ফেরত rd;
}

স্থির শূন্য free_sched_groups(কাঠামো sched_group *sg, পূর্ণসংখ্যা free_sgc)
{
	কাঠামো sched_group *tmp, *first;

	যদি (!sg)
		ফেরত;

	first = sg;
	করো {
		tmp = sg->পরবর্তী;

		যদি (free_sgc && atomic_dec_and_test(&sg->sgc->ref))
			kfree(sg->sgc);

		যদি (atomic_dec_and_test(&sg->ref))
			kfree(sg);
		sg = tmp;
	} যখন (sg != first);
}

স্থির শূন্য free_sched_domain_shared(কাঠামো sched_domain_shared *sds)
{
	যদি (sds && atomic_dec_and_test(&sds->ref))
		kfree(sds);
}

স্থির শূন্য destroy_sched_domain(কাঠামো sched_domain *sd)
{
	/*
	 * A normal sched domain may have multiple group references, an
	 * overlapping domain, having private groups, only one.  Iterate,
	 * dropping group/capacity references, freeing where none remain.
	 */
	free_sched_groups(sd->groups, 1);
	free_sched_domain_shared(sd->shared);

#ifdef CONFIG_SCHED_CACHE
	/* only the bottom sd has llc_counts array */
	kfree(sd->llc_counts);
#endif
	kfree(sd);
}

স্থির শূন্য destroy_sched_domains_rcu(কাঠামো rcu_শীর্ষ *rcu)
{
	কাঠামো sched_domain *sd = container_of(rcu, কাঠামো sched_domain, rcu);

	যখন (sd) {
		কাঠামো sched_domain *parent = sd->parent;
		destroy_sched_domain(sd);
		sd = parent;
	}
}

স্থির শূন্য destroy_sched_domains(কাঠামো sched_domain *sd)
{
	যদি (sd)
		call_rcu(&sd->rcu, destroy_sched_domains_rcu);
}

/*
 * Keep a special pointer to the highest sched_domain that has SD_SHARE_LLC set
 * (Last Level Cache Domain) for this allows us to avoid some pointer chasing
 * select_idle_sibling().
 *
 * Also keep a unique ID per domain (we use the first CPU number in the cpumask
 * of the domain), this allows us to quickly tell if two CPUs are in the same
 * cache domain, see cpus_share_cache().
 */
DEFINE_PER_CPU(কাঠামো sched_domain __rcu *, sd_llc);
DEFINE_PER_CPU(পূর্ণসংখ্যা, sd_llc_আকার);
DEFINE_PER_CPU(পূর্ণসংখ্যা, sd_llc_id) = -1;
DEFINE_PER_CPU(পূর্ণসংখ্যা, sd_share_id);
DEFINE_PER_CPU(কাঠামো sched_domain_shared __rcu *, sd_llc_shared);
DEFINE_PER_CPU(কাঠামো sched_domain_shared __rcu *, sd_balance_shared);
DEFINE_PER_CPU(কাঠামো sched_domain __rcu *, sd_numa);
DEFINE_PER_CPU(কাঠামো sched_domain __rcu *, sd_asym_packing);
DEFINE_PER_CPU(কাঠামো sched_domain __rcu *, sd_asym_cpucapacity);

DEFINE_STATIC_চাবি_FALSE(sched_asym_cpucapacity);
DEFINE_STATIC_চাবি_FALSE(sched_cluster_active);

স্থির শূন্য update_শীর্ষ_cache_domain(পূর্ণসংখ্যা cpu)
{
	কাঠামো sched_domain_shared *sds = ফাঁকা;
	কাঠামো sched_domain *sd;
	পূর্ণসংখ্যা id = cpu;
	পূর্ণসংখ্যা আকার_2 = 1;

	sd = highest_flag_domain(cpu, SD_SHARE_LLC);
	যদি (sd) {
		id = cpumask_first(sched_domain_span(sd));
		আকার_2 = cpumask_weight(sched_domain_span(sd));

		/* If sd_llc exists, sd_llc_shared should exist too. */
		WARN_ON_ONCE(!sd->shared);
		sds = sd->shared;
	}

	rcu_assign_pointer(per_cpu(sd_llc, cpu), sd);
	per_cpu(sd_llc_আকার, cpu) = আকার_2;
	rcu_assign_pointer(per_cpu(sd_llc_shared, cpu), sds);

	sd = lowest_flag_domain(cpu, SD_CLUSTER);
	যদি (sd)
		id = cpumask_first(sched_domain_span(sd));

	/*
	 * This assignment should be placed after the sd_llc_id as
	 * we want this id equals to cluster id on cluster machines
	 * but equals to LLC id on non-Cluster machines.
	 */
	per_cpu(sd_share_id, cpu) = id;

	sd = lowest_flag_domain(cpu, SD_NUMA);
	rcu_assign_pointer(per_cpu(sd_numa, cpu), sd);

	sd = highest_flag_domain(cpu, SD_ASYM_PACKING);
	rcu_assign_pointer(per_cpu(sd_asym_packing, cpu), sd);

	sd = lowest_flag_domain(cpu, SD_ASYM_CPUCAPACITY_FULL);
	/*
	 * The shared object is attached to sd_asym_cpucapacity only when the
	 * asym domain is non-overlapping (i.e., not built from SD_NUMA).
	 * On overlapping (NUMA) asym domains we fall back to letting the
	 * SD_SHARE_LLC path own the shared object, so sd->shared may be NULL
	 * here.
	 */
	যদি (sd && sd->shared)
		sds = sd->shared;

	rcu_assign_pointer(per_cpu(sd_asym_cpucapacity, cpu), sd);
	rcu_assign_pointer(per_cpu(sd_balance_shared, cpu), sds);
}

/*
 * Attach the domain 'sd' to 'cpu' as its base domain. Callers must
 * hold the hotplug lock.
 */
স্থির শূন্য
cpu_attach_domain(কাঠামো sched_domain *sd, কাঠামো মূল_domain *rd, পূর্ণসংখ্যা cpu)
{
	কাঠামো rq *rq = cpu_rq(cpu);
	কাঠামো sched_domain *tmp;

	/* Remove the sched domains which do not contribute to scheduling. */
	জন্য (tmp = sd; tmp; ) {
		কাঠামো sched_domain *parent = tmp->parent;
		যদি (!parent)
			বিরতি;

		যদি (sd_parent_degenerate(tmp, parent)) {
			tmp->parent = parent->parent;

			/* Pick reference to parent->shared. */
			যদি (parent->shared) {
				/*
				 * It is safe to free a sd->shared that
				 * has not been published yet. If a
				 * sd->shared was published, the refcount
				 * will end up being non-zero and it will
				 * not be freed here.
				 */
				free_sched_domain_shared(tmp->shared);
				tmp->shared = parent->shared;
				parent->shared = ফাঁকা;
			}

			যদি (parent->parent) {
				parent->parent->child = tmp;
				parent->parent->groups->flags = tmp->flags;
			}

			/*
			 * Transfer SD_PREFER_SIBLING down in case of a
			 * degenerate parent; the spans match for this
			 * so the property transfers.
			 */
			যদি (parent->flags & SD_PREFER_SIBLING)
				tmp->flags |= SD_PREFER_SIBLING;
			destroy_sched_domain(parent);
		} অন্যথা
			tmp = tmp->parent;
	}

	যদি (sd && sd_degenerate(sd)) {
		tmp = sd;
		sd = sd->parent;

		যদি (sd) {
			কাঠামো sched_group *sg = sd->groups;

#ifdef CONFIG_SCHED_CACHE
			/* move buffer to parent as child is being destroyed */
			sd->llc_counts = tmp->llc_counts;
			sd->llc_max = tmp->llc_max;
			sd->llc_bytes = tmp->llc_bytes;
			/* make sure destroy_sched_domain() does not free it */
			tmp->llc_counts = ফাঁকা;
			tmp->llc_max = 0;
			tmp->llc_bytes = 0;
#endif
			/*
			 * sched groups hold the flags of the child sched
			 * domain for convenience. Clear such flags since
			 * the child is being destroyed.
			 */
			করো {
				sg->flags = 0;
			} যখন (sg != sd->groups);

			sd->child = ফাঁকা;
		}

		destroy_sched_domain(tmp);
	}

	sched_domain_debug(sd, cpu);

	rq_attach_মূল(rq, rd);
	tmp = rq->sd;
	rcu_assign_pointer(rq->sd, sd);
	dirty_sched_domain_sysctl(cpu);
	destroy_sched_domains(tmp);

	update_শীর্ষ_cache_domain(cpu);
}

কাঠামো s_তথ্য {
	কাঠামো sched_domain_shared * __percpu *sds;
	কাঠামো sched_domain * __percpu *sd;
	কাঠামো মূল_domain	*rd;
};

গণনা s_alloc {
	sa_rootdomain,
	sa_sd,
	sa_sd_shared,
	sa_sd_storage,
	sa_none,
};

#ifdef CONFIG_SCHED_CACHE
/* hardware support for cache aware scheduling */
DEFINE_STATIC_চাবি_FALSE(sched_cache_present);
/*
 * Indicator of whether cache aware scheduling
 * is active, used by the scheduler.
 */
DEFINE_STATIC_চাবি_FALSE(sched_cache_active);
/* user wants cache aware scheduling [0 or 1] */
পূর্ণসংখ্যা sysctl_sched_cache_user = 1;

/*
 * Get the effective LLC size in bytes that @cpu's bottom sched_domain
 * can use. A CPU within a cpuset partition can only use a proportion
 * of the physical LLC, scaled by the ratio of the partition's span
 * weight to the hardware LLC sharing weight. @sd should be the
 * topmost domain with SD_SHARE_LLC.
 *
 * Returns 0 if cacheinfo is not yet populated. This happens during
 * early boot when build_sched_domains() runs before the generic
 * cacheinfo framework has been initialized (cacheinfo_cpu_online()
 * is a device_initcall cpuhp callback). In that case,
 * cacheinfo_cpu_online() will later call sched_update_llc_bytes()
 * to fill in the bottom domain's llc_bytes once the cache attributes
 * are available.
 */
স্থির unsigned long get_effective_llc_bytes(পূর্ণসংখ্যা cpu,
					     কাঠামো sched_domain *sd)
{
	কাঠামো cacheinfo *ci;
	unsigned পূর্ণসংখ্যা hw_weight;

	ci = get_cpu_cacheinfo_llc(cpu);
	যদি (!ci)
		ফেরত 0;

	hw_weight = cpumask_weight(&ci->shared_cpu_map);
	যদি (!hw_weight)
		ফেরত 0;

	ফেরত div_u64((u64)ci->আকার_2 * sd->span_weight, hw_weight);
}

স্থির bool alloc_sd_llc(ধ্রুবক কাঠামো cpumask *cpu_map,
			 কাঠামো s_তথ্য *d)
{
	কাঠামো sched_domain *sd, *শীর্ষ_llc, *parent;
	unsigned পূর্ণসংখ্যা *p;
	পূর্ণসংখ্যা i;

	for_each_cpu(i, cpu_map) {
		sd = *per_cpu_ptr(d->sd, i);
		যদি (!sd)
			যাও err;

		p = kcalloc_গ্রন্থি(max_lid + 1, আকার(unsigned পূর্ণসংখ্যা),
				 GFP_KERNEL, cpu_to_গ্রন্থি(i));
		যদি (!p)
			যাও err;

		শীর্ষ_llc = sd;
		/*
		 * Find the topmost SD_SHARE_LLC domain.
		 * Not yet attached to the CPU, so per_cpu(sd_llc, i)
		 * can not be used.
		 */
		যখন ((parent = rcu_dereference_protected(শীর্ষ_llc->parent, true)) &&
		       (parent->flags & SD_SHARE_LLC))
			শীর্ষ_llc = parent;

		যদি (শীর্ষ_llc->flags & SD_SHARE_LLC) {
			sd->llc_max = max_lid + 1;
			sd->llc_counts = p;
			sd->llc_bytes = get_effective_llc_bytes(i, শীর্ষ_llc);
		} অন্যথা {
			/* avoid memory leak */
			kfree(p);
		}
	}

	ফেরত true;
err:
	for_each_cpu(i, cpu_map) {
		sd = *per_cpu_ptr(d->sd, i);
		যদি (sd) {
			kfree(sd->llc_counts);
			sd->llc_counts = ফাঁকা;
			sd->llc_max = 0;
			sd->llc_bytes = 0;
		}
	}

	ফেরত false;
}

/*
 * Enable/disable cache aware scheduling according to
 * user input and the presence of hardware support.
 */
স্থির শূন্য _sched_cache_active_set(শূন্য)
{
	lockdep_assert_cpus_held();
	lockdep_assert_held(&sched_domains_mutex);

	/* hardware does not support */
	যদি (!static_branch_likely(&sched_cache_present)) {
		static_branch_disable_cpuslocked(&sched_cache_active);
		যদি (sched_debug())
			pr_info("%s: cache aware scheduling not supported on this platform\n", __func__);
		ফেরত;
	}

	/*
	 * user wants it or not ?
	 * TBD: read before writing the static key.
	 * It is not in the critical path, leave as-is
	 * for now.
	 */
	যদি (sysctl_sched_cache_user) {
		static_branch_enable_cpuslocked(&sched_cache_active);
		যদি (sched_debug())
			pr_info("%s: enabling cache aware scheduling\n", __func__);
	} অন্যথা {
		static_branch_disable_cpuslocked(&sched_cache_active);
		যদি (sched_debug())
			pr_info("%s: disabling cache aware scheduling\n", __func__);
	}
}

/* used by debugfs */
শূন্য sched_cache_active_set(শূন্য)
{
	cpus_read_lock();
	sched_domains_mutex_lock();
	_sched_cache_active_set();
	sched_domains_mutex_unlock();
	cpus_read_unlock();
}

/*
 * Update the bottom sched_domain's llc_bytes for @cpus sharing a physical
 * LLC. Called from cacheinfo_cpu_online() or
 * cacheinfo_cpu_pre_down() with cpu hotplug lock held.
 *
 * Note: get_effective_llc_bytes() returns 0 on PowerPC.
 * thus cache aware scheduling is disabled on PowerPC for
 * now. PowerPC does not use the generic cacheinfo framework --
 * it has its own cacheinfo with a separate struct cache hierarchy
 * and does not populates the per-CPU struct cpu_cacheinfo array
 * that get_cpu_cacheinfo_llc() reads.
 */
শূন্য sched_update_llc_bytes(ধ্রুবক কাঠামো cpumask *cpus)
{
	কাঠামো sched_domain *sd, *sdp;
	unsigned পূর্ণসংখ্যা i;

	sched_domains_mutex_lock();

	/*
	 * ci->shared_cpu_map is built incrementally as CPUs come
	 * online, so the first CPU in an LLC initially sees
	 * hw_weight == 1 and computes an inflated llc_bytes in
	 * get_effective_llc_bytes().  Re-evaluating every LLC
	 * sibling on each online event corrects this once the full
	 * shared_cpu_map is known.
	 *
	 * The departing CPU's domains have already been detached when
	 * cacheinfo removes it. Use the surviving cache siblings instead.
	 * They may belong to different cpuset partitions, so use each CPU's
	 * own LLC domain to scale its share of the physical cache.
	 */
	for_each_cpu(i, cpus) {
		sdp = rcu_dereference_sched_domain(per_cpu(sd_llc, i));
		যদি (!sdp)
			চালু;

		sd = rcu_dereference_sched_domain(cpu_rq(i)->sd);
		যদি (sd)
			sd->llc_bytes = get_effective_llc_bytes(i, sdp);
	}

	sched_domains_mutex_unlock();
}

স্থির শূন্য sched_cache_set(bool has_multi_llcs)
{
	/*
	 * TBD: check before writing to it. sched domain rebuild
	 * is not in the critical path, leave as-is for now.
	 */
	যদি (has_multi_llcs)
		static_branch_enable_cpuslocked(&sched_cache_present);
	অন্যথা
		static_branch_disable_cpuslocked(&sched_cache_present);

	_sched_cache_active_set();
}
#অন্যথা
স্থির bool alloc_sd_llc(ধ্রুবক কাঠামো cpumask *cpu_map,
			 কাঠামো s_তথ্য *d)
{
	ফেরত false;
}
স্থির অন্তঃরেখ শূন্য sched_cache_set(bool has_multi_llcs) { }
#endif

/*
 * Return true if @sd belongs to an LLC group whose enclosing
 * partition spans more than one LLC. @sd must be the topmost
 * SD_SHARE_LLC domain.
 *
 * Any duplicated parent domains with the same span as @sd are
 * skipped: before cpu_attach_domain() degeneration these still
 * exist, after degeneration the loop is a no-op. This makes the
 * helper usable both during sched domain build and against an
 * already-attached domain tree.
 *
 * Note: For systems with a single LLC per node, cache-aware
 * scheduling is still enabled when multiple nodes exist.
 * However, NUMA balancing decisions take precedence over
 * cache-aware scheduling. Conversely, if there is only one
 * LLC per partition, cache-aware scheduling should be disabled.
 */
স্থির bool sd_in_multi_llcs(কাঠামো sched_domain *sd)
{
	কাঠামো sched_domain *sdp = sd->parent;

	/* it does not make sense to aggregate to 1 CPU */
	যদি (sd->span_weight == 1)
		ফেরত false;

	যখন (sdp && sdp->span_weight == sd->span_weight)
		sdp = sdp->parent;

	ফেরত !!sdp;
}

/*
 * Return the canonical balance CPU for this group, this is the first CPU
 * of this group that's also in the balance mask.
 *
 * The balance mask are all those CPUs that could actually end up at this
 * group. See build_balance_mask().
 *
 * Also see should_we_balance().
 */
পূর্ণসংখ্যা group_balance_cpu(কাঠামো sched_group *sg)
{
	ফেরত cpumask_first(group_balance_mask(sg));
}


/*
 * NUMA topology (first read the regular topology blurb below)
 *
 * Given a node-distance table, for example:
 *
 *   node   0   1   2   3
 *     0:  10  20  30  20
 *     1:  20  10  20  30
 *     2:  30  20  10  20
 *     3:  20  30  20  10
 *
 * which represents a 4 node ring topology like:
 *
 *   0 ----- 1
 *   |       |
 *   |       |
 *   |       |
 *   3 ----- 2
 *
 * We want to construct domains and groups to represent this. The way we go
 * about doing this is to build the domains on 'hops'. For each NUMA level we
 * construct the mask of all nodes reachable in @level hops.
 *
 * For the above NUMA topology that gives 3 levels:
 *
 * NUMA-2	0-3		0-3		0-3		0-3
 *  groups:	{0-1,3},{1-3}	{0-2},{0,2-3}	{1-3},{0-1,3}	{0,2-3},{0-2}
 *
 * NUMA-1	0-1,3		0-2		1-3		0,2-3
 *  groups:	{0},{1},{3}	{0},{1},{2}	{1},{2},{3}	{0},{2},{3}
 *
 * NUMA-0	0		1		2		3
 *
 *
 * As can be seen; things don't nicely line up as with the regular topology.
 * When we iterate a domain in child domain chunks some nodes can be
 * represented multiple times -- hence the "overlap" naming for this part of
 * the topology.
 *
 * In order to minimize this overlap, we only build enough groups to cover the
 * domain. For instance Node-0 NUMA-2 would only get groups: 0-1,3 and 1-3.
 *
 * Because:
 *
 *  - the first group of each domain is its child domain; this
 *    gets us the first 0-1,3
 *  - the only uncovered node is 2, who's child domain is 1-3.
 *
 * However, because of the overlap, computing a unique CPU for each group is
 * more complicated. Consider for instance the groups of NODE-1 NUMA-2, both
 * groups include the CPUs of Node-0, while those CPUs would not in fact ever
 * end up at those groups (they would end up in group: 0-1,3).
 *
 * To correct this we have to introduce the group balance mask. This mask
 * will contain those CPUs in the group that can reach this group given the
 * (child) domain tree.
 *
 * With this we can once again compute balance_cpu and sched_group_capacity
 * relations.
 *
 * XXX include words on how balance_cpu is unique and therefore can be
 * used for sched_group_capacity links.
 *
 *
 * Another 'interesting' topology is:
 *
 *   node   0   1   2   3
 *     0:  10  20  20  30
 *     1:  20  10  20  20
 *     2:  20  20  10  20
 *     3:  30  20  20  10
 *
 * Which looks a little like:
 *
 *   0 ----- 1
 *   |     / |
 *   |   /   |
 *   | /     |
 *   2 ----- 3
 *
 * This topology is asymmetric, nodes 1,2 are fully connected, but nodes 0,3
 * are not.
 *
 * This leads to a few particularly weird cases where the sched_domain's are
 * not of the same number for each CPU. Consider:
 *
 * NUMA-2	0-3						0-3
 *  groups:	{0-2},{1-3}					{1-3},{0-2}
 *
 * NUMA-1	0-2		0-3		0-3		1-3
 *
 * NUMA-0	0		1		2		3
 *
 */


/*
 * Build the balance mask; it contains only those CPUs that can arrive at this
 * group and should be considered to continue balancing.
 *
 * We do this during the group creation pass, therefore the group information
 * isn't complete yet, however since each group represents a (child) domain we
 * can fully construct this using the sched_domain bits (which are already
 * complete).
 */
স্থির শূন্য
build_balance_mask(কাঠামো sched_domain *sd, কাঠামো sched_group *sg, কাঠামো cpumask *mask)
{
	ধ্রুবক কাঠামো cpumask *sg_span = sched_group_span(sg);
	কাঠামো sd_তথ্য *sdd = sd->ব্যক্তিগত;
	কাঠামো sched_domain *sibling;
	পূর্ণসংখ্যা i;

	cpumask_clear(mask);

	for_each_cpu(i, sg_span) {
		sibling = *per_cpu_ptr(sdd->sd, i);

		/*
		 * Can happen in the asymmetric case, where these siblings are
		 * unused. The mask will not be empty because those CPUs that
		 * do have the top domain _should_ span the domain.
		 */
		যদি (!sibling->child)
			চালু;

		/* If we would not end up here, we can't continue from here */
		যদি (!cpumask_equal(sg_span, sched_domain_span(sibling->child)))
			চালু;

		cpumask_set_cpu(i, mask);
	}

	/* We must not have empty masks here */
	WARN_ON_ONCE(cpumask_empty(mask));
}

/*
 * XXX: This creates per-node group entries; since the load-balancer will
 * immediately access remote memory to construct this group's load-balance
 * statistics having the groups node local is of dubious benefit.
 */
স্থির কাঠামো sched_group *
build_group_from_child_sched_domain(কাঠামো sched_domain *sd, পূর্ণসংখ্যা cpu)
{
	কাঠামো sched_group *sg;
	কাঠামো cpumask *sg_span;

	sg = kzalloc_গ্রন্থি(আকার(কাঠামো sched_group) + cpumask_আকার(),
			GFP_KERNEL, cpu_to_গ্রন্থি(cpu));

	যদি (!sg)
		ফেরত ফাঁকা;

	sg_span = sched_group_span(sg);
	যদি (sd->child) {
		cpumask_copy(sg_span, sched_domain_span(sd->child));
		sg->flags = sd->child->flags;
	} অন্যথা {
		cpumask_copy(sg_span, sched_domain_span(sd));
	}

	atomic_inc(&sg->ref);
	ফেরত sg;
}

স্থির শূন্য init_overlap_sched_group(কাঠামো sched_domain *sd,
				     কাঠামো sched_group *sg)
{
	কাঠামো cpumask *mask = sched_domains_tmpmask2;
	কাঠামো sd_তথ্য *sdd = sd->ব্যক্তিগত;
	কাঠামো cpumask *sg_span;
	পূর্ণসংখ্যা cpu;

	build_balance_mask(sd, sg, mask);
	cpu = cpumask_first(mask);

	sg->sgc = *per_cpu_ptr(sdd->sgc, cpu);
	যদি (atomic_inc_return(&sg->sgc->ref) == 1)
		cpumask_copy(group_balance_mask(sg), mask);
	অন্যথা
		WARN_ON_ONCE(!cpumask_equal(group_balance_mask(sg), mask));

	/*
	 * Initialize sgc->capacity such that even if we mess up the
	 * domains and no possible iteration will get us here, we won't
	 * die on a /0 trap.
	 */
	sg_span = sched_group_span(sg);
	sg->sgc->capacity = SCHED_CAPACITY_SCALE * cpumask_weight(sg_span);
	sg->sgc->min_capacity = SCHED_CAPACITY_SCALE;
	sg->sgc->max_capacity = SCHED_CAPACITY_SCALE;
}

স্থির কাঠামো sched_domain *
find_descended_sibling(কাঠামো sched_domain *sd, কাঠামো sched_domain *sibling)
{
	/*
	 * The proper descendant would be the one whose child won't span out
	 * of sd
	 */
	যখন (sibling->child &&
	       !cpumask_subset(sched_domain_span(sibling->child),
			       sched_domain_span(sd)))
		sibling = sibling->child;

	/*
	 * As we are referencing sgc across different topology level, we need
	 * to go down to skip those sched_domains which don't contribute to
	 * scheduling because they will be degenerated in cpu_attach_domain
	 */
	যখন (sibling->child &&
	       cpumask_equal(sched_domain_span(sibling->child),
			     sched_domain_span(sibling)))
		sibling = sibling->child;

	ফেরত sibling;
}

স্থির পূর্ণসংখ্যা
build_overlap_sched_groups(কাঠামো sched_domain *sd, পূর্ণসংখ্যা cpu)
{
	কাঠামো sched_group *first = ফাঁকা, *last = ফাঁকা, *sg;
	ধ্রুবক কাঠামো cpumask *span = sched_domain_span(sd);
	কাঠামো cpumask *covered = sched_domains_tmpmask;
	কাঠামো sd_তথ্য *sdd = sd->ব্যক্তিগত;
	কাঠামো sched_domain *sibling;
	পূর্ণসংখ্যা i;

	cpumask_clear(covered);

	for_each_cpu_wrap(i, span, cpu) {
		কাঠামো cpumask *sg_span;

		যদি (cpumask_test_cpu(i, covered))
			চালু;

		sibling = *per_cpu_ptr(sdd->sd, i);

		/*
		 * Asymmetric node setups can result in situations where the
		 * domain tree is of unequal depth, make sure to skip domains
		 * that already cover the entire range.
		 *
		 * In that case build_sched_domains() will have terminated the
		 * iteration early and our sibling sd spans will be empty.
		 * Domains should always include the CPU they're built on, so
		 * check that.
		 */
		যদি (!cpumask_test_cpu(i, sched_domain_span(sibling)))
			চালু;

		/*
		 * Usually we build sched_group by sibling's child sched_domain
		 * But for machines whose NUMA diameter are 3 or above, we move
		 * to build sched_group by sibling's proper descendant's child
		 * domain because sibling's child sched_domain will span out of
		 * the sched_domain being built as below.
		 *
		 * Smallest diameter=3 topology is:
		 *
		 *   node   0   1   2   3
		 *     0:  10  20  30  40
		 *     1:  20  10  20  30
		 *     2:  30  20  10  20
		 *     3:  40  30  20  10
		 *
		 *   0 --- 1 --- 2 --- 3
		 *
		 * NUMA-3       0-3             N/A             N/A             0-3
		 *  groups:     {0-2},{1-3}                                     {1-3},{0-2}
		 *
		 * NUMA-2       0-2             0-3             0-3             1-3
		 *  groups:     {0-1},{1-3}     {0-2},{2-3}     {1-3},{0-1}     {2-3},{0-2}
		 *
		 * NUMA-1       0-1             0-2             1-3             2-3
		 *  groups:     {0},{1}         {1},{2},{0}     {2},{3},{1}     {3},{2}
		 *
		 * NUMA-0       0               1               2               3
		 *
		 * The NUMA-2 groups for nodes 0 and 3 are obviously buggered, as the
		 * group span isn't a subset of the domain span.
		 */
		যদি (sibling->child &&
		    !cpumask_subset(sched_domain_span(sibling->child), span))
			sibling = find_descended_sibling(sd, sibling);

		sg = build_group_from_child_sched_domain(sibling, cpu);
		যদি (!sg)
			যাও fail;

		sg_span = sched_group_span(sg);
		cpumask_or(covered, covered, sg_span);

		init_overlap_sched_group(sibling, sg);

		যদি (!first)
			first = sg;
		যদি (last)
			last->পরবর্তী = sg;
		last = sg;
		last->পরবর্তী = first;
	}
	sd->groups = first;

	ফেরত 0;

fail:
	free_sched_groups(first, 0);

	ফেরত -ENOMEM;
}


/*
 * Package topology (also see the load-balance blurb in fair.c)
 *
 * The scheduler builds a tree structure to represent a number of important
 * topology features. By default (default_topology[]) these include:
 *
 *  - Simultaneous multithreading (SMT)
 *  - Multi-Core Cache (MC)
 *  - Package (PKG)
 *
 * Where the last one more or less denotes everything up to a NUMA node.
 *
 * The tree consists of 3 primary data structures:
 *
 *	sched_domain -> sched_group -> sched_group_capacity
 *	    ^ ^             ^ ^
 *          `-'             `-'
 *
 * The sched_domains are per-CPU and have a two way link (parent & child) and
 * denote the ever growing mask of CPUs belonging to that level of topology.
 *
 * Each sched_domain has a circular (double) linked list of sched_group's, each
 * denoting the domains of the level below (or individual CPUs in case of the
 * first domain level). The sched_group linked by a sched_domain includes the
 * CPU of that sched_domain [*].
 *
 * Take for instance a 2 threaded, 2 core, 2 cache cluster part:
 *
 * CPU   0   1   2   3   4   5   6   7
 *
 * PKG  [                             ]
 * MC   [             ] [             ]
 * SMT  [     ] [     ] [     ] [     ]
 *
 *  - or -
 *
 * PKG  0-7 0-7 0-7 0-7 0-7 0-7 0-7 0-7
 * MC	0-3 0-3 0-3 0-3 4-7 4-7 4-7 4-7
 * SMT  0-1 0-1 2-3 2-3 4-5 4-5 6-7 6-7
 *
 * CPU   0   1   2   3   4   5   6   7
 *
 * One way to think about it is: sched_domain moves you up and down among these
 * topology levels, while sched_group moves you sideways through it, at child
 * domain granularity.
 *
 * sched_group_capacity ensures each unique sched_group has shared storage.
 *
 * There are two related construction problems, both require a CPU that
 * uniquely identify each group (for a given domain):
 *
 *  - The first is the balance_cpu (see should_we_balance() and the
 *    load-balance blurb in fair.c); for each group we only want 1 CPU to
 *    continue balancing at a higher domain.
 *
 *  - The second is the sched_group_capacity; we want all identical groups
 *    to share a single sched_group_capacity.
 *
 * Since these topologies are exclusive by construction. That is, its
 * impossible for an SMT thread to belong to multiple cores, and cores to
 * be part of multiple caches. There is a very clear and unique location
 * for each CPU in the hierarchy.
 *
 * Therefore computing a unique CPU for each group is trivial (the iteration
 * mask is redundant and set all 1s; all CPUs in a group will end up at _that_
 * group), we can simply pick the first CPU in each group.
 *
 *
 * [*] in other words, the first group of each domain is its child domain.
 */

স্থির কাঠামো sched_group *get_group(পূর্ণসংখ্যা cpu, কাঠামো sd_তথ্য *sdd)
{
	কাঠামো sched_domain *sd = *per_cpu_ptr(sdd->sd, cpu);
	কাঠামো sched_domain *child = sd->child;
	কাঠামো sched_group *sg;
	bool already_visited;

	যদি (child)
		cpu = cpumask_first(sched_domain_span(child));

	sg = *per_cpu_ptr(sdd->sg, cpu);
	sg->sgc = *per_cpu_ptr(sdd->sgc, cpu);

	/* Increase refcounts for claim_allocations: */
	already_visited = atomic_inc_return(&sg->ref) > 1;
	/* sgc visits should follow a similar trend as sg */
	WARN_ON(already_visited != (atomic_inc_return(&sg->sgc->ref) > 1));

	/* If we have already visited that group, it's already initialized. */
	যদি (already_visited)
		ফেরত sg;

	যদি (child) {
		cpumask_copy(sched_group_span(sg), sched_domain_span(child));
		cpumask_copy(group_balance_mask(sg), sched_group_span(sg));
		sg->flags = child->flags;
	} অন্যথা {
		cpumask_set_cpu(cpu, sched_group_span(sg));
		cpumask_set_cpu(cpu, group_balance_mask(sg));
	}

	sg->sgc->capacity = SCHED_CAPACITY_SCALE * cpumask_weight(sched_group_span(sg));
	sg->sgc->min_capacity = SCHED_CAPACITY_SCALE;
	sg->sgc->max_capacity = SCHED_CAPACITY_SCALE;

	ফেরত sg;
}

/*
 * build_sched_groups will build a circular linked list of the groups
 * covered by the given span, will set each group's ->cpumask correctly,
 * and will initialize their ->sgc.
 *
 * Assumes the sched_domain tree is fully constructed
 */
স্থির পূর্ণসংখ্যা
build_sched_groups(কাঠামো sched_domain *sd, পূর্ণসংখ্যা cpu)
{
	কাঠামো sched_group *first = ফাঁকা, *last = ফাঁকা;
	কাঠামো sd_তথ্য *sdd = sd->ব্যক্তিগত;
	ধ্রুবক কাঠামো cpumask *span = sched_domain_span(sd);
	কাঠামো cpumask *covered;
	পূর্ণসংখ্যা i;

	lockdep_assert_held(&sched_domains_mutex);
	covered = sched_domains_tmpmask;

	cpumask_clear(covered);

	for_each_cpu_wrap(i, span, cpu) {
		কাঠামো sched_group *sg;

		যদি (cpumask_test_cpu(i, covered))
			চালু;

		sg = get_group(i, sdd);

		cpumask_or(covered, covered, sched_group_span(sg));

		যদি (!first)
			first = sg;
		যদি (last)
			last->পরবর্তী = sg;
		last = sg;
	}
	last->পরবর্তী = first;
	sd->groups = first;

	ফেরত 0;
}

/*
 * Initialize sched groups cpu_capacity.
 *
 * cpu_capacity indicates the capacity of sched group, which is used while
 * distributing the load between different sched groups in a sched domain.
 * Typically cpu_capacity for all the groups in a sched domain will be same
 * unless there are asymmetries in the topology. If there are asymmetries,
 * group having more cpu_capacity will pickup more load compared to the
 * group having less cpu_capacity.
 */
স্থির শূন্য init_sched_groups_capacity(পূর্ণসংখ্যা cpu, কাঠামো sched_domain *sd)
{
	কাঠামো sched_group *sg = sd->groups;
	কাঠামো cpumask *mask = sched_domains_tmpmask2;

	WARN_ON(!sg);

	করো {
		পূর্ণসংখ্যা cpu, cores = 0, max_cpu = -1;

		sg->group_weight = cpumask_weight(sched_group_span(sg));

		cpumask_copy(mask, sched_group_span(sg));
		for_each_cpu(cpu, mask) {
			cores++;
			cpumask_andnot(mask, mask, cpu_smt_mask(cpu));
		}
		sg->cores = cores;

		যদি (!(sd->flags & SD_ASYM_PACKING))
			যাও পরবর্তী;

		for_each_cpu(cpu, sched_group_span(sg)) {
			যদি (max_cpu < 0)
				max_cpu = cpu;
			অন্যথা যদি (sched_asym_prefer(cpu, max_cpu))
				max_cpu = cpu;
		}
		sg->asym_prefer_cpu = max_cpu;

পরবর্তী:
		sg = sg->পরবর্তী;
	} যখন (sg != sd->groups);

	যদি (cpu != group_balance_cpu(sg))
		ফেরত;

	update_group_capacity(sd, cpu);
}

/* Update the "asym_prefer_cpu" when arch_asym_cpu_priority() changes. */
শূন্য sched_update_asym_prefer_cpu(পূর্ণসংখ্যা cpu, পূর্ণসংখ্যা old_prio, পূর্ণসংখ্যা new_prio)
{
	পূর্ণসংখ্যা asym_prefer_cpu = cpu;
	কাঠামো sched_domain *sd;

	guard(rcu)();

	for_each_domain(cpu, sd) {
		কাঠামো sched_group *sg;
		পূর্ণসংখ্যা group_cpu;

		যদি (!(sd->flags & SD_ASYM_PACKING))
			চালু;

		/*
		 * Groups of overlapping domain are replicated per NUMA
		 * node and will require updating "asym_prefer_cpu" on
		 * each local copy.
		 *
		 * If you are hitting this warning, consider moving
		 * "sg->asym_prefer_cpu" to "sg->sgc->asym_prefer_cpu"
		 * which is shared by all the overlapping groups.
		 */
		WARN_ON_ONCE(sd->flags & SD_NUMA);

		sg = sd->groups;
		যদি (cpu != sg->asym_prefer_cpu) {
			/*
			 * Since the parent is a superset of the current group,
			 * if the cpu is not the "asym_prefer_cpu" at the
			 * current level, it cannot be the preferred CPU at a
			 * higher levels either.
			 */
			যদি (!sched_asym_prefer(cpu, sg->asym_prefer_cpu))
				ফেরত;

			WRITE_ONCE(sg->asym_prefer_cpu, cpu);
			চালু;
		}

		/* Ranking has improved; CPU is still the preferred one. */
		যদি (new_prio >= old_prio)
			চালু;

		for_each_cpu(group_cpu, sched_group_span(sg)) {
			যদি (sched_asym_prefer(group_cpu, asym_prefer_cpu))
				asym_prefer_cpu = group_cpu;
		}

		WRITE_ONCE(sg->asym_prefer_cpu, asym_prefer_cpu);
	}
}

/*
 * Set of available CPUs grouped by their corresponding capacities
 * Each list entry contains a CPU mask reflecting CPUs that share the same
 * capacity.
 * The lifespan of data is unlimited.
 */
LIST_শীর্ষ(asym_cap_list);

/*
 * Verify whether there is any CPU capacity asymmetry in a given sched domain.
 * Provides sd_flags reflecting the asymmetry scope.
 */
স্থির অন্তঃরেখ পূর্ণসংখ্যা
asym_cpu_capacity_classify(ধ্রুবক কাঠামো cpumask *sd_span,
			   ধ্রুবক কাঠামো cpumask *cpu_map)
{
	কাঠামো asym_cap_তথ্য *entry;
	পূর্ণসংখ্যা গুনতি = 0, miss = 0;

	/*
	 * Count how many unique CPU capacities this domain spans across
	 * (compare sched_domain CPUs mask with ones representing  available
	 * CPUs capacities). Take into account CPUs that might be offline:
	 * skip those.
	 */
	list_for_each_entry(entry, &asym_cap_list, link) {
		যদি (cpumask_intersects(sd_span, cpu_capacity_span(entry)))
			++গুনতি;
		অন্যথা যদি (cpumask_intersects(cpu_map, cpu_capacity_span(entry)))
			++miss;
	}

	WARN_ON_ONCE(!গুনতি && !list_empty(&asym_cap_list));

	/* No asymmetry detected */
	যদি (গুনতি < 2)
		ফেরত 0;
	/* Some of the available CPU capacity values have not been detected */
	যদি (miss)
		ফেরত SD_ASYM_CPUCAPACITY;

	/* Full asymmetry */
	ফেরত SD_ASYM_CPUCAPACITY | SD_ASYM_CPUCAPACITY_FULL;

}

স্থির শূন্য free_asym_cap_entry(কাঠামো rcu_শীর্ষ *শীর্ষ)
{
	কাঠামো asym_cap_তথ্য *entry = container_of(শীর্ষ, কাঠামো asym_cap_তথ্য, rcu);
	kfree(entry);
}

স্থির অন্তঃরেখ শূন্য asym_cpu_capacity_update_তথ্য(পূর্ণসংখ্যা cpu)
{
	unsigned long capacity = arch_scale_cpu_capacity(cpu);
	কাঠামো asym_cap_তথ্য *ঢোকাও_entry = ফাঁকা;
	কাঠামো asym_cap_তথ্য *entry;

	/*
	 * Search if capacity already exits. If not, track which the entry
	 * where we should insert to keep the list ordered descending.
	 */
	list_for_each_entry(entry, &asym_cap_list, link) {
		যদি (capacity == entry->capacity)
			যাও done;
		অন্যথা যদি (!ঢোকাও_entry && capacity > entry->capacity)
			ঢোকাও_entry = list_পূর্ব_entry(entry, link);
	}

	entry = kzalloc(আকার(*entry) + cpumask_আকার(), GFP_KERNEL);
	যদি (WARN_ONCE(!entry, "Failed to allocate memory for asymmetry data\n"))
		ফেরত;
	entry->capacity = capacity;

	/* If NULL then the new capacity is the smallest, add last. */
	যদি (!ঢোকাও_entry)
		list_add_লেজ_rcu(&entry->link, &asym_cap_list);
	অন্যথা
		list_add_rcu(&entry->link, &ঢোকাও_entry->link);
done:
	__cpumask_set_cpu(cpu, cpu_capacity_span(entry));
}

/*
 * Build-up/update list of CPUs grouped by their capacities
 * An update requires explicit request to rebuild sched domains
 * with state indicating CPU topology changes.
 */
স্থির শূন্য asym_cpu_capacity_scan(শূন্য)
{
	কাঠামো asym_cap_তথ্য *entry, *পরবর্তী;
	পূর্ণসংখ্যা cpu;

	list_for_each_entry(entry, &asym_cap_list, link)
		cpumask_clear(cpu_capacity_span(entry));

	for_each_cpu_and(cpu, cpu_possible_mask, housekeeping_cpumask(HK_TYPE_DOMAIN))
		asym_cpu_capacity_update_তথ্য(cpu);

	list_for_each_entry_safe(entry, পরবর্তী, &asym_cap_list, link) {
		যদি (cpumask_empty(cpu_capacity_span(entry))) {
			list_del_rcu(&entry->link);
			call_rcu(&entry->rcu, free_asym_cap_entry);
		}
	}

	/*
	 * Only one capacity value has been detected i.e. this system is symmetric.
	 * No need to keep this data around.
	 */
	যদি (list_is_singular(&asym_cap_list)) {
		entry = list_first_entry(&asym_cap_list, typeof(*entry), link);
		list_del_rcu(&entry->link);
		call_rcu(&entry->rcu, free_asym_cap_entry);
	}
}

/*
 * Initializers for schedule domains
 * Non-inlined to reduce accumulated stack pressure in build_sched_domains()
 */

স্থির পূর্ণসংখ্যা default_relax_domain_level = -1;
পূর্ণসংখ্যা sched_domain_level_max;

স্থির পূর্ণসংখ্যা __init setup_relax_domain_level(অক্ষর *str)
{
	যদি (kstrtoint(str, 0, &default_relax_domain_level))
		pr_warn("Unable to set relax_domain_level\n");

	ফেরত 1;
}
__setup("relax_domain_level=", setup_relax_domain_level);

স্থির শূন্য set_domain_attribute(কাঠামো sched_domain *sd,
				 কাঠামো sched_domain_attr *attr)
{
	পূর্ণসংখ্যা request;

	যদি (!attr || attr->relax_domain_level < 0) {
		যদি (default_relax_domain_level < 0)
			ফেরত;
		request = default_relax_domain_level;
	} অন্যথা
		request = attr->relax_domain_level;

	যদি (sd->level >= request) {
		/* Turn off idle balance on this domain: */
		sd->flags &= ~(SD_BALANCE_WAKE|SD_BALANCE_NEWIDLE);
	}
}

স্থির শূন্য __sdt_free(ধ্রুবক কাঠামো cpumask *cpu_map);
স্থির পূর্ণসংখ্যা __sdt_alloc(ধ্রুবক কাঠামো cpumask *cpu_map);

স্থির শূন্য __sds_free(কাঠামো s_তথ্য *d, ধ্রুবক কাঠামো cpumask *cpu_map);
স্থির পূর্ণসংখ্যা __sds_alloc(কাঠামো s_তথ্য *d, ধ্রুবক কাঠামো cpumask *cpu_map);

স্থির শূন্য __free_domain_allocs(কাঠামো s_তথ্য *d, গণনা s_alloc what,
				 ধ্রুবক কাঠামো cpumask *cpu_map)
{
	নির্বাচন (what) {
	ক্ষেত্র sa_rootdomain:
		যদি (!atomic_read(&d->rd->refcount))
			free_rootdomain(&d->rd->rcu);
		fallthrough;
	ক্ষেত্র sa_sd:
		free_percpu(d->sd);
		fallthrough;
	ক্ষেত্র sa_sd_shared:
		__sds_free(d, cpu_map);
		fallthrough;
	ক্ষেত্র sa_sd_storage:
		__sdt_free(cpu_map);
		fallthrough;
	ক্ষেত্র sa_none:
		বিরতি;
	}
}

স্থির গণনা s_alloc
__visit_domain_allocation_hell(কাঠামো s_তথ্য *d, ধ্রুবক কাঠামো cpumask *cpu_map)
{
	memset(d, 0, আকার(*d));

	যদি (__sdt_alloc(cpu_map))
		ফেরত sa_sd_storage;
	যদি (__sds_alloc(d, cpu_map))
		ফেরত sa_sd_shared;
	d->sd = alloc_percpu(কাঠামো sched_domain *);
	যদি (!d->sd)
		ফেরত sa_sd_shared;
	d->rd = alloc_rootdomain();
	যদি (!d->rd)
		ফেরত sa_sd;

	ফেরত sa_rootdomain;
}

/*
 * NULL the sd_data elements we've used to build the sched_domain and
 * sched_group structure so that the subsequent __free_domain_allocs()
 * will not free the data we're using.
 */
স্থির শূন্য claim_allocations(পূর্ণসংখ্যা cpu, কাঠামো s_তথ্য *d)
{
	কাঠামো sched_domain *sd;

	যদি (atomic_read(&(*per_cpu_ptr(d->sds, cpu))->ref))
		*per_cpu_ptr(d->sds, cpu) = ফাঁকা;

	জন্য (sd = *per_cpu_ptr(d->sd, cpu); sd; sd = sd->parent) {
		কাঠামো sd_তথ্য *sdd = sd->ব্যক্তিগত;

		WARN_ON_ONCE(*per_cpu_ptr(sdd->sd, cpu) != sd);
		*per_cpu_ptr(sdd->sd, cpu) = ফাঁকা;

		যদি (atomic_read(&(*per_cpu_ptr(sdd->sg, cpu))->ref))
			*per_cpu_ptr(sdd->sg, cpu) = ফাঁকা;

		যদি (atomic_read(&(*per_cpu_ptr(sdd->sgc, cpu))->ref))
			*per_cpu_ptr(sdd->sgc, cpu) = ফাঁকা;
	}
}

#ifdef CONFIG_NUMA
গণনা numa_topology_type sched_numa_topology_type;

/*
 * sched_domains_numa_distance is derived from sched_numa_node_distance
 * and provides a simplified view of NUMA distances used specifically
 * for building NUMA scheduling domains.
 */
স্থির পূর্ণসংখ্যা			sched_domains_numa_levels;
স্থির পূর্ণসংখ্যা			sched_numa_গ্রন্থি_levels;

পূর্ণসংখ্যা				sched_max_numa_distance;
স্থির পূর্ণসংখ্যা			*sched_domains_numa_distance;
স্থির পূর্ণসংখ্যা			*sched_numa_গ্রন্থি_distance;
স্থির কাঠামো cpumask		***sched_domains_numa_masks;
#endif /* CONFIG_NUMA */

/*
 * SD_flags allowed in topology descriptions.
 *
 * These flags are purely descriptive of the topology and do not prescribe
 * behaviour. Behaviour is artificial and mapped in the below sd_init()
 * function. For details, see include/linux/sched/sd_flags.h.
 *
 *   SD_SHARE_CPUCAPACITY
 *   SD_SHARE_LLC
 *   SD_CLUSTER
 *   SD_NUMA
 *
 * Odd one out, which beside describing the topology has a quirk also
 * prescribes the desired behaviour that goes along with it:
 *
 *   SD_ASYM_PACKING        - describes SMT quirks
 */
#define TOPOLOGY_SD_FLAGS		\
	(SD_SHARE_CPUCAPACITY	|	\
	 SD_CLUSTER		|	\
	 SD_SHARE_LLC		|	\
	 SD_NUMA		|	\
	 SD_ASYM_PACKING)

স্থির কাঠামো sched_domain *
sd_init(কাঠামো sched_domain_topology_level *tl,
	ধ্রুবক কাঠামো cpumask *cpu_map,
	কাঠামো sched_domain *child, পূর্ণসংখ্যা cpu)
{
	কাঠামো sd_তথ্য *sdd = &tl->তথ্য;
	কাঠামো sched_domain *sd = *per_cpu_ptr(sdd->sd, cpu);
	পূর্ণসংখ্যা sd_id, sd_weight, sd_flags = 0;
	কাঠামো cpumask *sd_span;
	u64 now = sched_clock();

	sd_span = sched_domain_span(sd);
	cpumask_and(sd_span, cpu_map, tl->mask(tl, cpu));
	sd_weight = cpumask_weight(sd_span);
	sd_id = cpumask_first(sd_span);

	যদি (tl->sd_flags)
		sd_flags = (*tl->sd_flags)();
	যদি (WARN_ONCE(sd_flags & ~TOPOLOGY_SD_FLAGS,
		      "wrong sd_flags in topology description\n"))
		sd_flags &= TOPOLOGY_SD_FLAGS;
#ifdef CONFIG_SCHED_SMT
	যদি (sched_smt_asym_packing && (sd_flags & SD_SHARE_CPUCAPACITY))
		sd_flags |= SD_ASYM_PACKING;
#endif
	sd_flags |= asym_cpu_capacity_classify(sd_span, cpu_map);

	*sd = (কাঠামো sched_domain){
		.min_interval		= sd_weight,
		.max_interval		= 2*sd_weight,
		.busy_factor		= 16,
		.imbalance_pct		= 117,

		.cache_nice_tries	= 0,

		.flags			= 1*SD_BALANCE_NEWIDLE
					| 1*SD_BALANCE_EXEC
					| 1*SD_BALANCE_FORK
					| 0*SD_BALANCE_WAKE
					| 1*SD_WAKE_AFFINE
					| 0*SD_SHARE_CPUCAPACITY
					| 0*SD_SHARE_LLC
					| 0*SD_SERIALIZE
					| 1*SD_PREFER_SIBLING
					| 0*SD_NUMA
					| sd_flags
					,

		.last_balance		= jiffies,
		.balance_interval	= sd_weight,

		/* 50% success rate */
		.newidle_call		= 512,
		.newidle_success	= 256,
		.newidle_ratio		= 512,
		.newidle_stamp		= now,

		.max_newidle_lb_cost	= 0,
		.last_decay_max_lb_cost	= jiffies,
		.child			= child,
		.name			= tl->name,
	};

	WARN_ONCE((sd->flags & (SD_SHARE_CPUCAPACITY | SD_ASYM_CPUCAPACITY)) ==
		  (SD_SHARE_CPUCAPACITY | SD_ASYM_CPUCAPACITY),
		  "CPU capacity asymmetry not supported on SMT\n");

	/*
	 * Convert topological properties into behaviour.
	 */
	যদি (sd->flags & SD_SHARE_CPUCAPACITY) {
		sd->imbalance_pct = 110;

	} অন্যথা যদি (sd->flags & SD_SHARE_LLC) {
		sd->imbalance_pct = 117;
		sd->cache_nice_tries = 1;

#ifdef CONFIG_NUMA
	} অন্যথা যদি (sd->flags & SD_NUMA) {
		sd->cache_nice_tries = 2;

		sd->flags &= ~SD_PREFER_SIBLING;
		sd->flags |= SD_SERIALIZE;
		যদি (sched_domains_numa_distance[tl->numa_level] > গ্রন্থি_reclaim_distance) {
			sd->flags &= ~(SD_BALANCE_EXEC |
				       SD_BALANCE_FORK |
				       SD_WAKE_AFFINE);
		}

#endif /* CONFIG_NUMA */
	} অন্যথা {
		sd->cache_nice_tries = 1;
	}

	sd->ব্যক্তিগত = sdd;

	ফেরত sd;
}

#ifdef CONFIG_SCHED_SMT
পূর্ণসংখ্যা cpu_smt_flags(শূন্য)
{
	ফেরত SD_SHARE_CPUCAPACITY | SD_SHARE_LLC;
}

ধ্রুবক কাঠামো cpumask *tl_smt_mask(কাঠামো sched_domain_topology_level *tl, পূর্ণসংখ্যা cpu)
{
	ফেরত cpu_smt_mask(cpu);
}
#endif

#ifdef CONFIG_SCHED_CLUSTER
পূর্ণসংখ্যা cpu_cluster_flags(শূন্য)
{
	ফেরত SD_CLUSTER | SD_SHARE_LLC;
}

ধ্রুবক কাঠামো cpumask *tl_cls_mask(কাঠামো sched_domain_topology_level *tl, পূর্ণসংখ্যা cpu)
{
	ফেরত cpu_clustergroup_mask(cpu);
}
#endif

#ifdef CONFIG_SCHED_MC
পূর্ণসংখ্যা cpu_core_flags(শূন্য)
{
	ফেরত SD_SHARE_LLC;
}

ধ্রুবক কাঠামো cpumask *tl_mc_mask(কাঠামো sched_domain_topology_level *tl, পূর্ণসংখ্যা cpu)
{
	ফেরত cpu_coregroup_mask(cpu);
}

/*
 * Majority of architectures have LLC at MC domain level with exception
 * such as powerpc. Provide a way for arch to specify where its LLC is
 * if it falls in exception category
 */
# ifndef arch_llc_mask
#define arch_llc_mask(cpu) cpu_coregroup_mask(cpu)
# endif

#অন্যথা
#define arch_llc_mask(cpu) cpumask_of(cpu)
#endif

#define llc_mask(cpu) arch_llc_mask(cpu)

ধ্রুবক কাঠামো cpumask *tl_pkg_mask(কাঠামো sched_domain_topology_level *tl, পূর্ণসংখ্যা cpu)
{
	ফেরত cpu_গ্রন্থি_mask(cpu);
}

/*
 * Topology list, bottom-up.
 */
স্থির কাঠামো sched_domain_topology_level default_topology[] = {
#ifdef CONFIG_SCHED_SMT
	SDTL_INIT(tl_smt_mask, cpu_smt_flags, SMT),
#endif

#ifdef CONFIG_SCHED_CLUSTER
	SDTL_INIT(tl_cls_mask, cpu_cluster_flags, CLS),
#endif

#ifdef CONFIG_SCHED_MC
	SDTL_INIT(tl_mc_mask, cpu_core_flags, MC),
#endif
	SDTL_INIT(tl_pkg_mask, ফাঁকা, PKG),
	{ ফাঁকা, },
};

স্থির কাঠামো sched_domain_topology_level *sched_domain_topology =
	default_topology;
স্থির কাঠামো sched_domain_topology_level *sched_domain_topology_saved;

#define for_each_sd_topology(tl)			\
	জন্য (tl = sched_domain_topology; tl->mask; tl++)

শূন্য __init set_sched_topology(কাঠামো sched_domain_topology_level *tl)
{
	যদি (WARN_ON_ONCE(sched_smp_initialized))
		ফেরত;

	sched_domain_topology = tl;
	sched_domain_topology_saved = ফাঁকা;
}

#ifdef CONFIG_NUMA
স্থির পূর্ণসংখ্যা cpu_numa_flags(শূন্য)
{
	ফেরত SD_NUMA;
}

স্থির ধ্রুবক কাঠামো cpumask *sd_numa_mask(কাঠামো sched_domain_topology_level *tl, পূর্ণসংখ্যা cpu)
{
	ফেরত sched_domains_numa_masks[tl->numa_level][cpu_to_গ্রন্থি(cpu)];
}

স্থির শূন্য sched_numa_warn(ধ্রুবক অক্ষর *str)
{
	স্থির পূর্ণসংখ্যা done = false;
	পূর্ণসংখ্যা i,j;

	যদি (done)
		ফেরত;

	done = true;

	printk(KERN_WARNING "ERROR: %s\n\n", str);

	জন্য (i = 0; i < nr_গ্রন্থি_ids; i++) {
		printk(KERN_WARNING "  ");
		জন্য (j = 0; j < nr_গ্রন্থি_ids; j++) {
			যদি (!গ্রন্থি_state(i, N_CPU) || !গ্রন্থি_state(j, N_CPU))
				printk(KERN_CONT "(%02d) ", গ্রন্থি_distance(i,j));
			অন্যথা
				printk(KERN_CONT " %02d  ", গ্রন্থি_distance(i,j));
		}
		printk(KERN_CONT "\n");
	}
	printk(KERN_WARNING "\n");
}

bool find_numa_distance(পূর্ণসংখ্যা distance)
{
	bool found = false;
	পূর্ণসংখ্যা i, *distances;

	যদি (distance == গ্রন্থি_distance(0, 0))
		ফেরত true;

	rcu_read_lock();
	distances = rcu_dereference(sched_numa_গ্রন্থি_distance);
	যদি (!distances)
		যাও unlock;
	জন্য (i = 0; i < sched_numa_গ্রন্থি_levels; i++) {
		যদি (distances[i] == distance) {
			found = true;
			বিরতি;
		}
	}
unlock:
	rcu_read_unlock();

	ফেরত found;
}

#define for_each_cpu_গ্রন্থি_but(n, nbut)		\
	for_each_গ্রন্থি_state(n, N_CPU)		\
		যদি (n == nbut)			\
			চালু;		\
		অন্যথা

/*
 * A system can have three types of NUMA topology:
 * NUMA_DIRECT: all nodes are directly connected, or not a NUMA system
 * NUMA_GLUELESS_MESH: some nodes reachable through intermediary nodes
 * NUMA_BACKPLANE: nodes can reach other nodes through a backplane
 *
 * The difference between a glueless mesh topology and a backplane
 * topology lies in whether communication between not directly
 * connected nodes goes through intermediary nodes (where programs
 * could run), or through backplane controllers. This affects
 * placement of programs.
 *
 * The type of topology can be discerned with the following tests:
 * - If the maximum distance between any nodes is 1 hop, the system
 *   is directly connected.
 * - If for two nodes A and B, located N > 1 hops away from each other,
 *   there is an intermediary node C, which is < N hops away from both
 *   nodes A and B, the system is a glueless mesh.
 */
স্থির শূন্য init_numa_topology_type(পূর্ণসংখ্যা offline_গ্রন্থি)
{
	পূর্ণসংখ্যা a, b, c, n;

	n = sched_max_numa_distance;

	যদি (sched_domains_numa_levels <= 2) {
		sched_numa_topology_type = NUMA_DIRECT;
		ফেরত;
	}

	for_each_cpu_গ্রন্থি_but(a, offline_গ্রন্থি) {
		for_each_cpu_গ্রন্থি_but(b, offline_গ্রন্থি) {
			/* Find two nodes furthest removed from each other. */
			যদি (গ্রন্থি_distance(a, b) < n)
				চালু;

			/* Is there an intermediary node between a and b? */
			for_each_cpu_গ্রন্থি_but(c, offline_গ্রন্থি) {
				যদি (গ্রন্থি_distance(a, c) < n &&
				    গ্রন্থি_distance(b, c) < n) {
					sched_numa_topology_type =
							NUMA_GLUELESS_MESH;
					ফেরত;
				}
			}

			sched_numa_topology_type = NUMA_BACKPLANE;
			ফেরত;
		}
	}

	pr_err("Failed to find a NUMA topology type, defaulting to DIRECT\n");
	sched_numa_topology_type = NUMA_DIRECT;
}


#define NR_DISTANCE_মানসমূহ (1 << DISTANCE_BITS)

/*
 * An architecture could modify its NUMA distance, to change
 * grouping of NUMA nodes and number of NUMA levels when creating
 * NUMA level sched domains.
 *
 * A NUMA level is created for each unique
 * arch_sched_node_distance.
 */
স্থির পূর্ণসংখ্যা numa_গ্রন্থি_dist(পূর্ণসংখ্যা i, পূর্ণসংখ্যা j)
{
	ফেরত গ্রন্থি_distance(i, j);
}

পূর্ণসংখ্যা arch_sched_গ্রন্থি_distance(পূর্ণসংখ্যা from, পূর্ণসংখ্যা to)
			     __weak __alias(numa_গ্রন্থি_dist);

স্থির bool modified_sched_গ্রন্থি_distance(শূন্য)
{
	ফেরত numa_গ্রন্থি_dist != arch_sched_গ্রন্থি_distance;
}

স্থির পূর্ণসংখ্যা sched_record_numa_dist(পূর্ণসংখ্যা offline_গ্রন্থি, পূর্ণসংখ্যা (*n_dist)(পূর্ণসংখ্যা, পূর্ণসংখ্যা),
				  পূর্ণসংখ্যা **dist, পূর্ণসংখ্যা *levels)
{
	unsigned long *distance_map __free(bitmap) = ফাঁকা;
	পূর্ণসংখ্যা nr_levels = 0;
	পূর্ণসংখ্যা i, j;
	পূর্ণসংখ্যা *distances;

	/*
	 * O(nr_nodes^2) de-duplicating selection sort -- in order to find the
	 * unique distances in the node_distance() table.
	 */
	distance_map = bitmap_alloc(NR_DISTANCE_মানসমূহ, GFP_KERNEL);
	যদি (!distance_map)
		ফেরত -ENOMEM;

	bitmap_zero(distance_map, NR_DISTANCE_মানসমূহ);
	for_each_cpu_গ্রন্থি_but(i, offline_গ্রন্থি) {
		for_each_cpu_গ্রন্থি_but(j, offline_গ্রন্থি) {
			পূর্ণসংখ্যা distance = n_dist(i, j);

			যদি (distance < LOCAL_DISTANCE || distance >= NR_DISTANCE_মানসমূহ) {
				sched_numa_warn("Invalid distance value range");
				ফেরত -EINVAL;
			}

			bitmap_set(distance_map, distance, 1);
		}
	}
	/*
	 * We can now figure out how many unique distance values there are and
	 * allocate memory accordingly.
	 */
	nr_levels = bitmap_weight(distance_map, NR_DISTANCE_মানসমূহ);

	distances = kzalloc_objs(পূর্ণসংখ্যা, nr_levels);
	যদি (!distances)
		ফেরত -ENOMEM;

	জন্য (i = 0, j = 0; i < nr_levels; i++, j++) {
		j = find_পরবর্তী_bit(distance_map, NR_DISTANCE_মানসমূহ, j);
		distances[i] = j;
	}
	*dist = distances;
	*levels = nr_levels;

	ফেরত 0;
}

শূন্য sched_init_numa(পূর্ণসংখ্যা offline_গ্রন্থি)
{
	কাঠামো sched_domain_topology_level *tl;
	পূর্ণসংখ্যা nr_levels, nr_গ্রন্থি_levels;
	পূর্ণসংখ্যা i, j;
	পূর্ণসংখ্যা *distances, *domain_distances;
	কাঠামো cpumask ***masks;

	/* Record the NUMA distances from SLIT table */
	যদি (sched_record_numa_dist(offline_গ্রন্থি, numa_গ্রন্থি_dist, &distances,
				   &nr_গ্রন্থি_levels))
		ফেরত;

	/* Record modified NUMA distances for building sched domains */
	যদি (modified_sched_গ্রন্থি_distance()) {
		যদি (sched_record_numa_dist(offline_গ্রন্থি, arch_sched_গ্রন্থি_distance,
					   &domain_distances, &nr_levels)) {
			kfree(distances);
			ফেরত;
		}
	} অন্যথা {
		domain_distances = distances;
		nr_levels = nr_গ্রন্থি_levels;
	}
	rcu_assign_pointer(sched_numa_গ্রন্থি_distance, distances);
	WRITE_ONCE(sched_max_numa_distance, distances[nr_গ্রন্থি_levels - 1]);
	WRITE_ONCE(sched_numa_গ্রন্থি_levels, nr_গ্রন্থি_levels);

	/*
	 * 'nr_levels' contains the number of unique distances
	 *
	 * The sched_domains_numa_distance[] array includes the actual distance
	 * numbers.
	 */

	/*
	 * Here, we should temporarily reset sched_domains_numa_levels to 0.
	 * If it fails to allocate memory for array sched_domains_numa_masks[][],
	 * the array will contain less then 'nr_levels' members. This could be
	 * dangerous when we use it to iterate array sched_domains_numa_masks[][]
	 * in other functions.
	 *
	 * We reset it to 'nr_levels' at the end of this function.
	 */
	rcu_assign_pointer(sched_domains_numa_distance, domain_distances);

	sched_domains_numa_levels = 0;

	masks = kzalloc(আকার(শূন্য *) * nr_levels, GFP_KERNEL);
	যদি (!masks)
		ফেরত;

	/*
	 * Now for each level, construct a mask per node which contains all
	 * CPUs of nodes that are that many hops away from us.
	 */
	জন্য (i = 0; i < nr_levels; i++) {
		masks[i] = kzalloc(nr_গ্রন্থি_ids * আকার(শূন্য *), GFP_KERNEL);
		যদি (!masks[i])
			ফেরত;

		for_each_cpu_গ্রন্থি_but(j, offline_গ্রন্থি) {
			কাঠামো cpumask *mask = kzalloc(cpumask_আকার(), GFP_KERNEL);
			পূর্ণসংখ্যা k;

			যদি (!mask)
				ফেরত;

			masks[i][j] = mask;

			for_each_cpu_গ্রন্থি_but(k, offline_গ্রন্থি) {
				যদি (sched_debug() &&
				    (arch_sched_গ্রন্থি_distance(j, k) !=
				     arch_sched_গ্রন্থি_distance(k, j)))
					sched_numa_warn("Node-distance not symmetric");

				যদি (arch_sched_গ্রন্থি_distance(j, k) >
				    sched_domains_numa_distance[i])
					চালু;

				cpumask_or(mask, mask, cpumask_of_গ্রন্থি(k));
			}
		}
	}
	rcu_assign_pointer(sched_domains_numa_masks, masks);

	/* Compute default topology size */
	জন্য (i = 0; sched_domain_topology[i].mask; i++);

	tl = kzalloc((i + nr_levels + 1) *
			আকার(কাঠামো sched_domain_topology_level), GFP_KERNEL);
	যদি (!tl)
		ফেরত;

	/*
	 * Copy the default topology bits..
	 */
	জন্য (i = 0; sched_domain_topology[i].mask; i++)
		tl[i] = sched_domain_topology[i];

	/*
	 * Add the NUMA identity distance, aka single NODE.
	 */
	tl[i++] = SDTL_INIT(sd_numa_mask, ফাঁকা, গ্রন্থি_2);

	/*
	 * .. and append 'j' levels of NUMA goodness.
	 */
	জন্য (j = 1; j < nr_levels; i++, j++) {
		tl[i] = SDTL_INIT(sd_numa_mask, cpu_numa_flags, NUMA);
		tl[i].numa_level = j;
	}

	sched_domain_topology_saved = sched_domain_topology;
	sched_domain_topology = tl;

	sched_domains_numa_levels = nr_levels;

	init_numa_topology_type(offline_গ্রন্থি);
}


স্থির শূন্য sched_reset_numa(শূন্য)
{
	পূর্ণসংখ্যা nr_levels, *distances, *dom_distances = ফাঁকা;
	কাঠামো cpumask ***masks;

	nr_levels = sched_domains_numa_levels;
	sched_numa_গ্রন্থি_levels = 0;
	sched_domains_numa_levels = 0;
	sched_max_numa_distance = 0;
	sched_numa_topology_type = NUMA_DIRECT;
	distances = sched_numa_গ্রন্থি_distance;
	যদি (sched_numa_গ্রন্থি_distance != sched_domains_numa_distance)
		dom_distances = sched_domains_numa_distance;
	rcu_assign_pointer(sched_numa_গ্রন্থি_distance, ফাঁকা);
	rcu_assign_pointer(sched_domains_numa_distance, ফাঁকা);
	masks = sched_domains_numa_masks;
	rcu_assign_pointer(sched_domains_numa_masks, ফাঁকা);
	যদি (distances || masks) {
		পূর্ণসংখ্যা i, j;

		synchronize_rcu();
		kfree(distances);
		kfree(dom_distances);
		জন্য (i = 0; i < nr_levels && masks; i++) {
			যদি (!masks[i])
				চালু;
			for_each_গ্রন্থি(j)
				kfree(masks[i][j]);
			kfree(masks[i]);
		}
		kfree(masks);
	}
	যদি (sched_domain_topology_saved) {
		kfree(sched_domain_topology);
		sched_domain_topology = sched_domain_topology_saved;
		sched_domain_topology_saved = ফাঁকা;
	}
}

/*
 * Call with hotplug lock held
 */
শূন্য sched_update_numa(পূর্ণসংখ্যা cpu, bool online)
{
	পূর্ণসংখ্যা গ্রন্থি;

	গ্রন্থি = cpu_to_গ্রন্থি(cpu);
	/*
	 * Scheduler NUMA topology is updated when the first CPU of a
	 * node is onlined or the last CPU of a node is offlined.
	 */
	যদি (cpumask_weight(cpumask_of_গ্রন্থি(গ্রন্থি)) != 1)
		ফেরত;

	sched_reset_numa();
	sched_init_numa(online ? NUMA_NO_গ্রন্থি : গ্রন্থি);
}

শূন্য sched_domains_numa_masks_set(unsigned পূর্ণসংখ্যা cpu)
{
	পূর্ণসংখ্যা গ্রন্থি = cpu_to_গ্রন্থি(cpu);
	পূর্ণসংখ্যা i, j;

	জন্য (i = 0; i < sched_domains_numa_levels; i++) {
		জন্য (j = 0; j < nr_গ্রন্থি_ids; j++) {
			যদি (!গ্রন্থি_state(j, N_CPU))
				চালু;

			/* Set ourselves in the remote node's masks */
			যদি (arch_sched_গ্রন্থি_distance(j, গ্রন্থি) <=
			    sched_domains_numa_distance[i])
				cpumask_set_cpu(cpu, sched_domains_numa_masks[i][j]);
		}
	}
}

শূন্য sched_domains_numa_masks_clear(unsigned পূর্ণসংখ্যা cpu)
{
	পূর্ণসংখ্যা i, j;

	জন্য (i = 0; i < sched_domains_numa_levels; i++) {
		জন্য (j = 0; j < nr_গ্রন্থি_ids; j++) {
			যদি (sched_domains_numa_masks[i][j])
				cpumask_clear_cpu(cpu, sched_domains_numa_masks[i][j]);
		}
	}
}

/*
 * sched_numa_find_closest() - given the NUMA topology, find the cpu
 *                             closest to @cpu from @cpumask.
 * cpumask: cpumask to find a cpu from
 * cpu: cpu to be close to
 *
 * returns: cpu, or nr_cpu_ids when nothing found.
 */
পূর্ণসংখ্যা sched_numa_find_closest(ধ্রুবক কাঠামো cpumask *cpus, পূর্ণসংখ্যা cpu)
{
	পূর্ণসংখ্যা i, j = cpu_to_গ্রন্থি(cpu), found = nr_cpu_ids;
	কাঠামো cpumask ***masks;

	rcu_read_lock();
	masks = rcu_dereference(sched_domains_numa_masks);
	যদি (!masks)
		যাও unlock;
	জন্য (i = 0; i < sched_domains_numa_levels; i++) {
		যদি (!masks[i][j])
			বিরতি;
		cpu = cpumask_any_and_distribute(cpus, masks[i][j]);
		যদি (cpu < nr_cpu_ids) {
			found = cpu;
			বিরতি;
		}
	}
unlock:
	rcu_read_unlock();

	ফেরত found;
}

কাঠামো __cmp_key {
	ধ্রুবক কাঠামো cpumask *cpus;
	কাঠামো cpumask ***masks;
	পূর্ণসংখ্যা গ্রন্থি;
	পূর্ণসংখ্যা cpu;
	পূর্ণসংখ্যা w;
};

স্থির পূর্ণসংখ্যা hop_cmp(ধ্রুবক শূন্য *a, ধ্রুবক শূন্য *b)
{
	কাঠামো cpumask **পূর্ব_hop, **cur_hop = *(কাঠামো cpumask ***)b;
	কাঠামো __cmp_key *k = (কাঠামো __cmp_key *)a;

	যদি (cpumask_weight_and(k->cpus, cur_hop[k->গ্রন্থি]) <= k->cpu)
		ফেরত 1;

	যদি (b == k->masks) {
		k->w = 0;
		ফেরত 0;
	}

	পূর্ব_hop = *((কাঠামো cpumask ***)b - 1);
	k->w = cpumask_weight_and(k->cpus, পূর্ব_hop[k->গ্রন্থি]);
	যদি (k->w <= k->cpu)
		ফেরত 0;

	ফেরত -1;
}

/**
 * sched_numa_find_nth_cpu() - given the NUMA topology, find the Nth closest CPU
 *                             from @cpus to @cpu, taking into account distance
 *                             from a given @node.
 * @cpus: cpumask to find a cpu from
 * @cpu: CPU to start searching
 * @node: NUMA node to order CPUs by distance
 *
 * Return: cpu, or nr_cpu_ids when nothing found.
 */
পূর্ণসংখ্যা sched_numa_find_nth_cpu(ধ্রুবক কাঠামো cpumask *cpus, পূর্ণসংখ্যা cpu, পূর্ণসংখ্যা গ্রন্থি)
{
	কাঠামো __cmp_key k = { .cpus = cpus, .cpu = cpu };
	কাঠামো cpumask ***hop_masks;
	পূর্ণসংখ্যা hop, ret = nr_cpu_ids;

	যদি (গ্রন্থি == NUMA_NO_গ্রন্থি)
		ফেরত cpumask_nth_and(cpu, cpus, cpu_online_mask);

	rcu_read_lock();

	/* CPU-less node entries are uninitialized in sched_domains_numa_masks */
	গ্রন্থি = numa_nearest_গ্রন্থি(গ্রন্থি, N_CPU);
	k.গ্রন্থি = গ্রন্থি;

	k.masks = rcu_dereference(sched_domains_numa_masks);
	যদি (!k.masks)
		যাও unlock;

	hop_masks = bsearch(&k, k.masks, sched_domains_numa_levels, আকার(k.masks[0]), hop_cmp);
	যদি (!hop_masks)
		যাও unlock;
	hop = hop_masks	- k.masks;

	ret = hop ?
		cpumask_nth_and_andnot(cpu - k.w, cpus, k.masks[hop][গ্রন্থি], k.masks[hop-1][গ্রন্থি]) :
		cpumask_nth_and(cpu, cpus, k.masks[0][গ্রন্থি]);
unlock:
	rcu_read_unlock();
	ফেরত ret;
}
EXPORT_প্রতীক_GPL(sched_numa_find_nth_cpu);

/**
 * sched_numa_hop_mask() - Get the cpumask of CPUs at most @hops hops away from
 *                         @node
 * @node: The node to count hops from.
 * @hops: Include CPUs up to that many hops away. 0 means local node.
 *
 * Return: On success, a pointer to a cpumask of CPUs at most @hops away from
 * @node, an error value otherwise.
 *
 * Requires rcu_lock to be held. Returned cpumask is only valid within that
 * read-side section, copy it if required beyond that.
 *
 * Note that not all hops are equal in distance; see sched_init_numa() for how
 * distances and masks are handled.
 * Also note that this is a reflection of sched_domains_numa_masks, which may change
 * during the lifetime of the system (offline nodes are taken out of the masks).
 */
ধ্রুবক কাঠামো cpumask *sched_numa_hop_mask(unsigned পূর্ণসংখ্যা গ্রন্থি, unsigned পূর্ণসংখ্যা hops)
{
	কাঠামো cpumask ***masks;

	যদি (গ্রন্থি >= nr_গ্রন্থি_ids || hops >= sched_domains_numa_levels)
		ফেরত ERR_PTR(-EINVAL);

	masks = rcu_dereference(sched_domains_numa_masks);
	যদি (!masks)
		ফেরত ERR_PTR(-EBUSY);

	ফেরত masks[hops][গ্রন্থি];
}
EXPORT_প্রতীক_GPL(sched_numa_hop_mask);

#endif /* CONFIG_NUMA */

স্থির পূর্ণসংখ্যা __sdt_alloc(ধ্রুবক কাঠামো cpumask *cpu_map)
{
	কাঠামো sched_domain_topology_level *tl;
	পূর্ণসংখ্যা j;

	for_each_sd_topology(tl) {
		কাঠামো sd_তথ্য *sdd = &tl->তথ্য;

		sdd->sd = alloc_percpu(কাঠামো sched_domain *);
		যদি (!sdd->sd)
			ফেরত -ENOMEM;

		sdd->sg = alloc_percpu(কাঠামো sched_group *);
		যদি (!sdd->sg)
			ফেরত -ENOMEM;

		sdd->sgc = alloc_percpu(কাঠামো sched_group_capacity *);
		যদি (!sdd->sgc)
			ফেরত -ENOMEM;

		for_each_cpu(j, cpu_map) {
			কাঠামো sched_domain *sd;
			কাঠামো sched_group *sg;
			কাঠামো sched_group_capacity *sgc;

			sd = kzalloc_গ্রন্থি(আকার(কাঠামো sched_domain) + cpumask_আকার(),
					GFP_KERNEL, cpu_to_গ্রন্থি(j));
			যদি (!sd)
				ফেরত -ENOMEM;

			*per_cpu_ptr(sdd->sd, j) = sd;

			sg = kzalloc_গ্রন্থি(আকার(কাঠামো sched_group) + cpumask_আকার(),
					GFP_KERNEL, cpu_to_গ্রন্থি(j));
			যদি (!sg)
				ফেরত -ENOMEM;

			sg->পরবর্তী = sg;

			*per_cpu_ptr(sdd->sg, j) = sg;

			sgc = kzalloc_গ্রন্থি(আকার(কাঠামো sched_group_capacity) + cpumask_আকার(),
					GFP_KERNEL, cpu_to_গ্রন্থি(j));
			যদি (!sgc)
				ফেরত -ENOMEM;

			sgc->id = j;

			*per_cpu_ptr(sdd->sgc, j) = sgc;
		}
	}

	ফেরত 0;
}

স্থির শূন্য __sdt_free(ধ্রুবক কাঠামো cpumask *cpu_map)
{
	কাঠামো sched_domain_topology_level *tl;
	পূর্ণসংখ্যা j;

	for_each_sd_topology(tl) {
		কাঠামো sd_তথ্য *sdd = &tl->তথ্য;

		for_each_cpu(j, cpu_map) {
			কাঠামো sched_domain *sd;

			যদি (sdd->sd) {
				sd = *per_cpu_ptr(sdd->sd, j);
				যদি (sd && (sd->flags & SD_NUMA))
					free_sched_groups(sd->groups, 0);
				kfree(*per_cpu_ptr(sdd->sd, j));
			}

			যদি (sdd->sg)
				kfree(*per_cpu_ptr(sdd->sg, j));
			যদি (sdd->sgc)
				kfree(*per_cpu_ptr(sdd->sgc, j));
		}
		free_percpu(sdd->sd);
		sdd->sd = ফাঁকা;
		free_percpu(sdd->sg);
		sdd->sg = ফাঁকা;
		free_percpu(sdd->sgc);
		sdd->sgc = ফাঁকা;
	}
}

স্থির পূর্ণসংখ্যা __sds_alloc(কাঠামো s_তথ্য *d, ধ্রুবক কাঠামো cpumask *cpu_map)
{
	পূর্ণসংখ্যা j;

	d->sds = alloc_percpu(কাঠামো sched_domain_shared *);
	যদি (!d->sds)
		ফেরত -ENOMEM;

	for_each_cpu(j, cpu_map) {
		কাঠামো sched_domain_shared *sds;

		sds = kzalloc_গ্রন্থি(আকার(কাঠামো sched_domain_shared),
				GFP_KERNEL, cpu_to_গ্রন্থি(j));
		যদি (!sds)
			ফেরত -ENOMEM;

		*per_cpu_ptr(d->sds, j) = sds;
	}

	ফেরত 0;
}

স্থির শূন্য __sds_free(কাঠামো s_তথ্য *d, ধ্রুবক কাঠামো cpumask *cpu_map)
{
	পূর্ণসংখ্যা j;

	যদি (!d->sds)
		ফেরত;

	for_each_cpu(j, cpu_map)
		kfree(*per_cpu_ptr(d->sds, j));

	free_percpu(d->sds);
	d->sds = ফাঁকা;
}

স্থির কাঠামো sched_domain *build_sched_domain(কাঠামো sched_domain_topology_level *tl,
		ধ্রুবক কাঠামো cpumask *cpu_map, কাঠামো sched_domain_attr *attr,
		কাঠামো sched_domain *child, পূর্ণসংখ্যা cpu)
{
	কাঠামো sched_domain *sd = sd_init(tl, cpu_map, child, cpu);

	যদি (child) {
		sd->level = child->level + 1;
		sched_domain_level_max = max(sched_domain_level_max, sd->level);
		child->parent = sd;

		যদি (!cpumask_subset(sched_domain_span(child),
				    sched_domain_span(sd))) {
			pr_err("BUG: arch topology borken\n");
			pr_err("     the %s domain not a subset of the %s domain\n",
					child->name, sd->name);
			/* Fixup, ensure @sd has at least @child CPUs. */
			cpumask_or(sched_domain_span(sd),
				   sched_domain_span(sd),
				   sched_domain_span(child));
		}

	}
	set_domain_attribute(sd, attr);

	ফেরত sd;
}

/*
 * Ensure topology masks are sane, i.e. there are no conflicts (overlaps) for
 * any two given CPUs on non-NUMA topology levels.
 */
স্থির bool topology_span_sane(ধ্রুবক কাঠামো cpumask *cpu_map)
{
	কাঠামো sched_domain_topology_level *tl;
	কাঠামো cpumask *covered, *id_seen;
	পূর্ণসংখ্যা cpu;

	lockdep_assert_held(&sched_domains_mutex);
	covered = sched_domains_tmpmask;
	id_seen = sched_domains_tmpmask2;

	for_each_sd_topology(tl) {
		পূর্ণসংখ্যা tl_common_flags = 0;

		যদি (tl->sd_flags)
			tl_common_flags = (*tl->sd_flags)();

		/* NUMA levels are allowed to overlap */
		যদি (tl_common_flags & SD_NUMA)
			চালু;

		cpumask_clear(covered);
		cpumask_clear(id_seen);

		/*
		 * Non-NUMA levels cannot partially overlap - they must be either
		 * completely equal or completely disjoint. Otherwise we can end up
		 * breaking the sched_group lists - i.e. a later get_group() pass
		 * breaks the linking done for an earlier span.
		 */
		for_each_cpu(cpu, cpu_map) {
			ধ্রুবক কাঠামো cpumask *tl_cpu_mask = tl->mask(tl, cpu);
			পূর্ণসংখ্যা id;

			/* lowest bit set in this mask is used as a unique id */
			id = cpumask_first(tl_cpu_mask);

			যদি (cpumask_test_cpu(id, id_seen)) {
				/* First CPU has already been seen, ensure identical spans */
				যদি (!cpumask_equal(tl->mask(tl, id), tl_cpu_mask))
					ফেরত false;
			} অন্যথা {
				/* First CPU hasn't been seen before, ensure it's a completely new span */
				যদি (cpumask_intersects(tl_cpu_mask, covered))
					ফেরত false;

				cpumask_or(covered, covered, tl_cpu_mask);
				cpumask_set_cpu(id, id_seen);
			}
		}
	}
	ফেরত true;
}

/*
 * Calculate an allowed NUMA imbalance such that LLCs do not get
 * imbalanced.
 */
স্থির শূন্য adjust_numa_imbalance(কাঠামো sched_domain *sd_llc)
{
	কাঠামো sched_domain *parent;
	unsigned পূর্ণসংখ্যা imb_span = 1;
	unsigned পূর্ণসংখ্যা imb = 0;
	unsigned পূর্ণসংখ্যা nr_llcs;

	WARN_ON(!(sd_llc->flags & SD_SHARE_LLC));
	WARN_ON(!sd_llc->parent);

	/*
	 * For a single LLC per node, allow an
	 * imbalance up to 12.5% of the node. This is
	 * arbitrary cutoff based two factors -- SMT and
	 * memory channels. For SMT-2, the intent is to
	 * avoid premature sharing of HT resources but
	 * SMT-4 or SMT-8 *may* benefit from a different
	 * cutoff. For memory channels, this is a very
	 * rough estimate of how many channels may be
	 * active and is based on recent CPUs with
	 * many cores.
	 *
	 * For multiple LLCs, allow an imbalance
	 * until multiple tasks would share an LLC
	 * on one node while LLCs on another node
	 * remain idle. This assumes that there are
	 * enough logical CPUs per LLC to avoid SMT
	 * factors and that there is a correlation
	 * between LLCs and memory channels.
	 */
	nr_llcs = sd_llc->parent->span_weight / sd_llc->span_weight;
	যদি (nr_llcs == 1)
		imb = sd_llc->parent->span_weight >> 3;
	অন্যথা
		imb = nr_llcs;

	imb = max(1U, imb);
	sd_llc->parent->imb_numa_nr = imb;

	/*
	 * Set span based on the first NUMA domain.
	 *
	 * NUMA systems always add a NODE domain before
	 * iterating the NUMA domains. Since this is before
	 * degeneration, start from sd_llc's parent's
	 * parent which is the lowest an SD_NUMA domain can
	 * be relative to sd_llc.
	 */
	parent = sd_llc->parent->parent;
	যখন (parent && !(parent->flags & SD_NUMA))
		parent = parent->parent;

	imb_span = parent ? parent->span_weight : sd_llc->parent->span_weight;

	/* Update the upper remainder of the topology */
	parent = sd_llc->parent;
	যখন (parent) {
		পূর্ণসংখ্যা factor = max(1U, (parent->span_weight / imb_span));

		parent->imb_numa_nr = imb * factor;
		parent = parent->parent;
	}
}

স্থির শূন্য
init_sched_domain_shared(কাঠামো s_তথ্য *d, কাঠামো sched_domain *sd, পূর্ণসংখ্যা flags)
{
	কাঠামো sched_domain_shared *sds = ফাঁকা;
	পূর্ণসংখ্যা cpu;

	/*
	 * Multiple domains can try to claim a shared object like
	 * SD_ASYM_CPUCAPACITY and SD_SHARE_LLC which can alias to
	 * same cpumask_first(sched_domain_span(sd)) CPU and can
	 * cause "nr_idle_scan" to be populated incorrectly during
	 * load balancing.
	 *
	 * Find the first CPU in sched_domain_span(sd) with an
	 * unclaimed domain (!alloc_flags) or where the alloc_flag
	 * matches the requested flag (SD_* flag)
	 *
	 * If the domain only has single CPU, allow temporary overlap
	 * in allocation since the domains will be degenerated later.
	 */
	for_each_cpu(cpu, sched_domain_span(sd)) {
		sds = *per_cpu_ptr(d->sds, cpu);

		যদি (!sds->alloc_flags ||
		    sd->span_weight == 1 ||
		    sds->alloc_flags == flags) {
			sds->alloc_flags = flags;
			sd->shared = sds;
			বিরতি;
		}
	}

	/*
	 * Use the sd_shared corresponding to the last
	 * CPU in the span if none are avaialable.
	 */
	যদি (WARN_ON_ONCE(!sd->shared))
		sd->shared = sds;

	/*
	 * nr_busy_cpus is consumed only by the NOHZ kick path via
	 * sd_balance_shared; on the asym-capacity path it is initialized but
	 * never read.
	 */
	atomic_set(&sd->shared->nr_busy_cpus, sd->span_weight);
	atomic_inc(&sd->shared->ref);
}

/*
 * For asymmetric CPU capacity, attach sched_domain_shared on the innermost
 * SD_ASYM_CPUCAPACITY_FULL ancestor of @cpu's base domain when that ancestor is
 * not an overlapping NUMA-built domain (then LLC should claim shared).
 *
 * A CPU may lack any FULL ancestor (e.g., exclusive cpuset symmetric island),
 * then LLC must claim shared instead.
 *
 * Note: SD_ASYM_CPUCAPACITY_FULL is only set when all CPU capacity values
 * are present in the domain span, so the asym domain we attach to cannot
 * degenerate into a single-capacity group. The relevant edge cases are instead
 * covered by the caveats above.
 *
 * Return true if this CPU's asym path claimed sd->shared, false otherwise.
 */
স্থির bool claim_asym_sched_domain_shared(কাঠামো s_তথ্য *d, পূর্ণসংখ্যা cpu)
{
	কাঠামো sched_domain *sd = *per_cpu_ptr(d->sd, cpu);
	কাঠামো sched_domain *sd_asym;

	যদি (!sd)
		ফেরত false;

	sd_asym = sd;
	যখন (sd_asym && !(sd_asym->flags & SD_ASYM_CPUCAPACITY_FULL))
		sd_asym = sd_asym->parent;

	যদি (!sd_asym || (sd_asym->flags & SD_NUMA))
		ফেরত false;

	init_sched_domain_shared(d, sd_asym, SD_ASYM_CPUCAPACITY);
	ফেরত true;
}

স্থির পূর্ণসংখ্যা __sched_domains_alloc_llc_id(শূন্য)
{
	পূর্ণসংখ্যা lid, max;

	lockdep_assert_held(&sched_domains_mutex);

	lid = cpumask_first_zero(sched_domains_llc_id_allocmask);
	/*
	 * llc_id space should never grow larger than the
	 * possible number of CPUs in the system.
	 */
	যদি (lid >= nr_cpu_ids)
		ফেরত -1;

	__cpumask_set_cpu(lid, sched_domains_llc_id_allocmask);
	max = cpumask_last(sched_domains_llc_id_allocmask);
	যদি (max > max_lid)
		max_lid = max;

	ফেরত lid;
}

স্থির শূন্য __sched_domains_free_llc_id(পূর্ণসংখ্যা cpu)
{
	পূর্ণসংখ্যা i, lid, max;

	lockdep_assert_held(&sched_domains_mutex);

	lid = per_cpu(sd_llc_id, cpu);
	যদি (lid == -1 || lid >= nr_cpu_ids)
		ফেরত;

	per_cpu(sd_llc_id, cpu) = -1;

	for_each_cpu(i, llc_mask(cpu)) {
		/* An online CPU owns the llc_id. */
		যদি (per_cpu(sd_llc_id, i) == lid)
			ফেরত;
	}

	__cpumask_clear_cpu(lid, sched_domains_llc_id_allocmask);

	max = cpumask_last(sched_domains_llc_id_allocmask);
	/* shrink max lid to save memory */
	যদি (max < max_lid)
		max_lid = max;
}

শূন্য sched_domains_free_llc_id(পূর্ণসংখ্যা cpu)
{
	sched_domains_mutex_lock();
	__sched_domains_free_llc_id(cpu);
	sched_domains_mutex_unlock();
}

/*
 * Build sched domains for a given set of CPUs and attach the sched domains
 * to the individual CPUs
 */
স্থির পূর্ণসংখ্যা
build_sched_domains(ধ্রুবক কাঠামো cpumask *cpu_map, কাঠামো sched_domain_attr *attr,
		    bool *multi_llcs)
{
	গণনা s_alloc alloc_state = sa_none;
	bool has_multi_llcs = false;
	কাঠামো sched_domain *sd;
	কাঠামো s_তথ্য d;
	কাঠামো rq *rq = ফাঁকা;
	পূর্ণসংখ্যা i, ret = -ENOMEM;
	bool has_asym = false;
	bool has_cluster = false;

	যদি (WARN_ON(cpumask_empty(cpu_map)))
		যাও error;

	alloc_state = __visit_domain_allocation_hell(&d, cpu_map);
	যদি (alloc_state != sa_rootdomain)
		যাও error;

	/* Set up domains for CPUs specified by the cpu_map: */
	for_each_cpu(i, cpu_map) {
		কাঠামো sched_domain_topology_level *tl;
		পূর্ণসংখ্যা lid;

		sd = ফাঁকা;
		for_each_sd_topology(tl) {

			sd = build_sched_domain(tl, cpu_map, attr, sd, i);

			has_asym |= sd->flags & SD_ASYM_CPUCAPACITY;

			যদি (tl == sched_domain_topology)
				*per_cpu_ptr(d.sd, i) = sd;
			যদি (cpumask_equal(cpu_map, sched_domain_span(sd)))
				বিরতি;
		}

		lid = per_cpu(sd_llc_id, i);
		যদি (lid == -1) {
			/* try to reuse the llc_id of its siblings */
			জন্য (পূর্ণসংখ্যা j = cpumask_first(llc_mask(i));
			     j < nr_cpu_ids;
			     j = cpumask_পরবর্তী(j, llc_mask(i))) {
				যদি (i == j)
					চালু;

				lid = per_cpu(sd_llc_id, j);

				যদি (lid != -1) {
					per_cpu(sd_llc_id, i) = lid;

					বিরতি;
				}
			}

			/* a new LLC is detected */
			যদি (lid == -1)
				per_cpu(sd_llc_id, i) = __sched_domains_alloc_llc_id();
		}
	}

	যদি (WARN_ON(!topology_span_sane(cpu_map)))
		যাও error;

	/* Build the groups for the domains */
	for_each_cpu(i, cpu_map) {
		জন্য (sd = *per_cpu_ptr(d.sd, i); sd; sd = sd->parent) {
			sd->span_weight = cpumask_weight(sched_domain_span(sd));
			যদি (sd->flags & SD_NUMA) {
				যদি (build_overlap_sched_groups(sd, i))
					যাও error;
			} অন্যথা {
				যদি (build_sched_groups(sd, i))
					যাও error;
			}
		}
	}

	for_each_cpu(i, cpu_map) {
		sd = *per_cpu_ptr(d.sd, i);
		যদি (!sd)
			চালু;

		যদি (has_asym)
			claim_asym_sched_domain_shared(&d, i);

		/* First, find the topmost SD_SHARE_LLC domain */
		যখন (sd->parent && (sd->parent->flags & SD_SHARE_LLC))
			sd = sd->parent;

		যদি (sd->flags & SD_SHARE_LLC) {
			init_sched_domain_shared(&d, sd, SD_SHARE_LLC);

			/*
			 * In presence of higher domains, adjust the
			 * NUMA imbalance stats for the hierarchy.
			 */
			যদি (sd->parent) {
				যদি (IS_ENABLED(CONFIG_NUMA))
					adjust_numa_imbalance(sd);

				যদি (sd_in_multi_llcs(sd))
					has_multi_llcs = true;
			}
		}
	}

	/* Calculate CPU capacity for physical packages and nodes */
	জন্য (i = nr_cpumask_bits-1; i >= 0; i--) {
		যদি (!cpumask_test_cpu(i, cpu_map))
			চালু;

		claim_allocations(i, &d);

		জন্য (sd = *per_cpu_ptr(d.sd, i); sd; sd = sd->parent)
			init_sched_groups_capacity(i, sd);
	}

	alloc_sd_llc(cpu_map, &d);

	/* Attach the domains */
	rcu_read_lock();
	for_each_cpu(i, cpu_map) {
		rq = cpu_rq(i);
		sd = *per_cpu_ptr(d.sd, i);

		cpu_attach_domain(sd, d.rd, i);

		যদি (lowest_flag_domain(i, SD_CLUSTER))
			has_cluster = true;
	}
	rcu_read_unlock();

	যদি (has_asym)
		static_branch_inc_cpuslocked(&sched_asym_cpucapacity);

	যদি (has_cluster)
		static_branch_inc_cpuslocked(&sched_cluster_active);

	যদি (rq && sched_debug_verbose)
		pr_info("root domain span: %*pbl\n", cpumask_pr_args(cpu_map));

	ret = 0;
error:
	*multi_llcs = has_multi_llcs;
	__free_domain_allocs(&d, alloc_state, cpu_map);

	ফেরত ret;
}

/* Current sched domains: */
স্থির cpumask_var_t			*doms_cur;

/* Number of sched domains in 'doms_cur': */
স্থির পূর্ণসংখ্যা				ndoms_cur;

/* Attributes of custom domains in 'doms_cur' */
স্থির কাঠামো sched_domain_attr		*dattr_cur;

/*
 * Special case: If a kmalloc() of a doms_cur partition (array of
 * cpumask) fails, then fallback to a single sched domain,
 * as determined by the single cpumask fallback_doms.
 */
স্থির cpumask_var_t			fallback_doms;

/*
 * arch_update_cpu_topology lets virtualized architectures update the
 * CPU core maps. It is supposed to return 1 if the topology changed
 * or 0 if it stayed the same.
 */
পূর্ণসংখ্যা __weak arch_update_cpu_topology(শূন্য)
{
	ফেরত 0;
}

cpumask_var_t *alloc_sched_domains(unsigned পূর্ণসংখ্যা ndoms)
{
	পূর্ণসংখ্যা i;
	cpumask_var_t *doms;

	doms = kmalloc_objs(*doms, ndoms);
	যদি (!doms)
		ফেরত ফাঁকা;
	জন্য (i = 0; i < ndoms; i++) {
		যদি (!alloc_cpumask_var(&doms[i], GFP_KERNEL)) {
			free_sched_domains(doms, i);
			ফেরত ফাঁকা;
		}
	}
	ফেরত doms;
}

শূন্য free_sched_domains(cpumask_var_t doms[], unsigned পূর্ণসংখ্যা ndoms)
{
	unsigned পূর্ণসংখ্যা i;
	জন্য (i = 0; i < ndoms; i++)
		free_cpumask_var(doms[i]);
	kfree(doms);
}

/*
 * Set up scheduler domains and groups.  For now this just excludes isolated
 * CPUs, but could be used to exclude other special cases in the future.
 */
পূর্ণসংখ্যা __init sched_init_domains(ধ্রুবক কাঠামো cpumask *cpu_map)
{
	bool multi_llcs;
	পূর্ণসংখ্যা err;

	zalloc_cpumask_var(&sched_domains_llc_id_allocmask, GFP_KERNEL);
	zalloc_cpumask_var(&sched_domains_tmpmask, GFP_KERNEL);
	zalloc_cpumask_var(&sched_domains_tmpmask2, GFP_KERNEL);
	zalloc_cpumask_var(&fallback_doms, GFP_KERNEL);

	arch_update_cpu_topology();
	asym_cpu_capacity_scan();
	ndoms_cur = 1;
	doms_cur = alloc_sched_domains(ndoms_cur);
	যদি (!doms_cur)
		doms_cur = &fallback_doms;
	cpumask_and(doms_cur[0], cpu_map, housekeeping_cpumask(HK_TYPE_DOMAIN));
	err = build_sched_domains(doms_cur[0], ফাঁকা, &multi_llcs);
	যদি (!err)
		sched_cache_set(multi_llcs);

	ফেরত err;
}

/*
 * Detach sched domains from a group of CPUs specified in cpu_map
 * These CPUs will now be attached to the NULL domain
 */
স্থির শূন্য detach_destroy_domains(ধ্রুবক কাঠামো cpumask *cpu_map)
{
	unsigned পূর্ণসংখ্যা cpu = cpumask_any(cpu_map);
	পূর্ণসংখ্যা i;

	যদি (rcu_access_pointer(per_cpu(sd_asym_cpucapacity, cpu)))
		static_branch_dec_cpuslocked(&sched_asym_cpucapacity);

	যদি (static_branch_unlikely(&sched_cluster_active))
		static_branch_dec_cpuslocked(&sched_cluster_active);

	rcu_read_lock();
	for_each_cpu(i, cpu_map)
		cpu_attach_domain(ফাঁকা, &def_মূল_domain, i);
	rcu_read_unlock();
}

/* handle null as "default" */
স্থির পূর্ণসংখ্যা dattrs_equal(কাঠামো sched_domain_attr *cur, পূর্ণসংখ্যা idx_cur,
			কাঠামো sched_domain_attr *new, পূর্ণসংখ্যা idx_new)
{
	কাঠামো sched_domain_attr tmp;

	/* Fast path: */
	যদি (!new && !cur)
		ফেরত 1;

	tmp = SD_ATTR_INIT;

	ফেরত !memcmp(cur ? (cur + idx_cur) : &tmp,
			new ? (new + idx_new) : &tmp,
			আকার(কাঠামো sched_domain_attr));
}

/*
 * Partition sched domains as specified by the 'ndoms_new'
 * cpumasks in the array doms_new[] of cpumasks. This compares
 * doms_new[] to the current sched domain partitioning, doms_cur[].
 * It destroys each deleted domain and builds each new domain.
 *
 * 'doms_new' is an array of cpumask_var_t's of length 'ndoms_new'.
 * The masks don't intersect (don't overlap.) We should setup one
 * sched domain for each mask. CPUs not in any of the cpumasks will
 * not be load balanced. If the same cpumask appears both in the
 * current 'doms_cur' domains and in the new 'doms_new', we can leave
 * it as it is.
 *
 * The passed in 'doms_new' should be allocated using
 * alloc_sched_domains.  This routine takes ownership of it and will
 * free_sched_domains it when done with it. If the caller failed the
 * alloc call, then it can pass in doms_new == NULL && ndoms_new == 1,
 * and partition_sched_domains() will fallback to the single partition
 * 'fallback_doms', it also forces the domains to be rebuilt.
 *
 * If doms_new == NULL it will be replaced with cpu_online_mask.
 * ndoms_new == 0 is a special case for destroying existing domains,
 * and it will not create the default domain.
 *
 * Call with hotplug lock and sched_domains_mutex held
 */
স্থির শূন্য partition_sched_domains_locked(পূর্ণসংখ্যা ndoms_new, cpumask_var_t doms_new[],
				    কাঠামো sched_domain_attr *dattr_new)
{
	bool __maybe_unused has_eas = false;
	bool has_multi_llcs = false, multi_llcs;
	পূর্ণসংখ্যা i, j, n;
	পূর্ণসংখ্যা new_topology;

	lockdep_assert_held(&sched_domains_mutex);

	/* Let the architecture update CPU core mappings: */
	new_topology = arch_update_cpu_topology();
	/* Trigger rebuilding CPU capacity asymmetry data */
	যদি (new_topology)
		asym_cpu_capacity_scan();

	যদি (!doms_new) {
		WARN_ON_ONCE(dattr_new);
		n = 0;
		doms_new = alloc_sched_domains(1);
		যদি (doms_new) {
			n = 1;
			cpumask_and(doms_new[0], cpu_active_mask,
				    housekeeping_cpumask(HK_TYPE_DOMAIN));
		}
	} অন্যথা {
		n = ndoms_new;
	}

	/* Destroy deleted domains: */
	জন্য (i = 0; i < ndoms_cur; i++) {
		জন্য (j = 0; j < n && !new_topology; j++) {
			যদি (cpumask_equal(doms_cur[i], doms_new[j]) &&
			    dattrs_equal(dattr_cur, i, dattr_new, j))
				যাও match1;
		}
		/* No match - a current sched domain not in new doms_new[] */
		detach_destroy_domains(doms_cur[i]);
match1:
		;
	}

	n = ndoms_cur;
	যদি (!doms_new) {
		n = 0;
		doms_new = &fallback_doms;
		cpumask_and(doms_new[0], cpu_active_mask,
			    housekeeping_cpumask(HK_TYPE_DOMAIN));
	}

	/* Build new domains: */
	জন্য (i = 0; i < ndoms_new; i++) {
		জন্য (j = 0; j < n && !new_topology; j++) {
			যদি (cpumask_equal(doms_new[i], doms_cur[j]) &&
			    dattrs_equal(dattr_new, i, dattr_cur, j)) {
				/*
				 * Reused partition has to be taken care
				 * of here, because there could be a corner
				 * case that if the reused partition is skipped
				 * and only new partition is considered, an
				 * incorrect has_multi_llcs would be set. For
				 * example:
				 * If the only multi-LLC partition is reused
				 * and a new single-LLC partition is built,
				 * sched_cache_set(false) disables cache-aware
				 * scheduling globally despite the reused
				 * multi-LLC partition still being active.
				 */
				কাঠামো sched_domain *sd;
				পূর্ণসংখ্যা cpu = cpumask_first(doms_cur[j]);

				guard(rcu)();
				sd = rcu_dereference(cpu_rq(cpu)->sd);
				যখন (sd && sd->parent && (sd->parent->flags & SD_SHARE_LLC))
					sd = sd->parent;
				যদি (sd && (sd->flags & SD_SHARE_LLC) && sd->parent &&
				    sd_in_multi_llcs(sd))
					has_multi_llcs = true;
				যাও match2;
			}
		}
		/* No match - add a new doms_new */
		build_sched_domains(doms_new[i], dattr_new ? dattr_new + i : ফাঁকা,
				    &multi_llcs);
		has_multi_llcs |= multi_llcs;
match2:
		;
	}
	sched_cache_set(has_multi_llcs);

#যদি defined(CONFIG_ENERGY_MODEL) && defined(CONFIG_CPU_FREQ_GOV_SCHEDUTIL)
	/* Build perf domains: */
	জন্য (i = 0; i < ndoms_new; i++) {
		জন্য (j = 0; j < n && !sched_energy_update; j++) {
			যদি (cpumask_equal(doms_new[i], doms_cur[j]) &&
			    cpu_rq(cpumask_first(doms_cur[j]))->rd->pd) {
				has_eas = true;
				যাও match3;
			}
		}
		/* No match - add perf domains for a new rd */
		has_eas |= build_perf_domains(doms_new[i]);
match3:
		;
	}
	sched_energy_set(has_eas);
#endif

	/* Remember the new sched domains: */
	যদি (doms_cur != &fallback_doms)
		free_sched_domains(doms_cur, ndoms_cur);

	kfree(dattr_cur);
	doms_cur = doms_new;
	dattr_cur = dattr_new;
	ndoms_cur = ndoms_new;

	update_sched_domain_debugfs();
	dl_rebuild_rd_accounting();
}

/*
 * Call with hotplug lock held
 */
শূন্য partition_sched_domains(পূর্ণসংখ্যা ndoms_new, cpumask_var_t doms_new[],
			     কাঠামো sched_domain_attr *dattr_new)
{
	sched_domains_mutex_lock();
	partition_sched_domains_locked(ndoms_new, doms_new, dattr_new);
	sched_domains_mutex_unlock();
}
