// SPDX-License-Identifier: GPL-2.0
/*
 * Scheduler topology setup/handling methods
 */

#شامل <linux/sched/isolation.h>
#شامل <linux/sched/clock.h>
#شامل <linux/bsearch.h>
#شامل "sched.h"

DEFINE_MUTEX(sched_domains_mutex);
تهی sched_domains_mutex_lock(تهی)
{
	mutex_lock(&sched_domains_mutex);
}
تهی sched_domains_mutex_unlock(تهی)
{
	mutex_unlock(&sched_domains_mutex);
}

/* Protected by sched_domains_mutex: */
ایستا cpumask_var_t sched_domains_llc_id_allocmask;
ایستا cpumask_var_t sched_domains_tmpmask;
ایستا cpumask_var_t sched_domains_tmpmask2;
صحیح max_lid;

ایستا صحیح __init sched_debug_setup(نویسه *str)
{
	sched_debug_verbose = true;

	بازگشت 0;
}
early_param("sched_verbose", sched_debug_setup);

#ifdef CONFIG_SCHED_SMT
ایستا bool sched_smt_asym_packing __read_mostly;

ایستا صحیح __init setup_sched_smt_asym_packing(نویسه *str)
{
	اگر (strcmp(str, "on"))
		بازگشت 0;

	sched_smt_asym_packing = true;
	بازگشت 1;
}
__setup("sched_smt_asym_packing=", setup_sched_smt_asym_packing);
#endif

ایستا درونخطی bool sched_debug(تهی)
{
	بازگشت sched_debug_verbose;
}

#define SD_FLAG(_name, mflags) [__##_name] = { .meta_flags = mflags, .name = #_name },
ثابت ساختار sd_flag_debug sd_flag_debug[] = {
#شامل <linux/sched/sd_flags.h>
};
#undef SD_FLAG

ایستا صحیح sched_domain_debug_one(ساختار sched_domain *sd, صحیح cpu, صحیح level,
				  ساختار cpumask *groupmask)
{
	ساختار sched_group *group = sd->groups;
	unsigned long flags = sd->flags;
	unsigned صحیح idx;

	cpumask_clear(groupmask);

	printk(KERN_DEBUG "%*s domain-%d: ", level, "", level);
	printk(KERN_CONT "span=%*pbl level=%s\n",
	       cpumask_pr_args(sched_domain_span(sd)), sd->name);

	اگر (!cpumask_test_cpu(cpu, sched_domain_span(sd))) {
		printk(KERN_ERR "ERROR: domain->span does not contain CPU%d\n", cpu);
	}
	اگر (group && !cpumask_test_cpu(cpu, sched_group_span(group))) {
		printk(KERN_ERR "ERROR: domain->groups does not contain CPU%d\n", cpu);
	}

	for_each_set_bit(idx, &flags, __SD_FLAG_CNT) {
		unsigned صحیح flag = BIT(idx);
		unsigned صحیح meta_flags = sd_flag_debug[idx].meta_flags;

		اگر ((meta_flags & SDF_SHARED_CHILD) && sd->child &&
		    !(sd->child->flags & flag))
			printk(KERN_ERR "ERROR: flag %s set here but not in child\n",
			       sd_flag_debug[idx].name);

		اگر ((meta_flags & SDF_SHARED_PARENT) && sd->parent &&
		    !(sd->parent->flags & flag))
			printk(KERN_ERR "ERROR: flag %s set here but not in parent\n",
			       sd_flag_debug[idx].name);
	}

	printk(KERN_DEBUG "%*s groups:", level + 1, "");
	انجامبده {
		اگر (!group) {
			printk("\n");
			printk(KERN_ERR "ERROR: group is NULL\n");
			بشکن;
		}

		اگر (cpumask_empty(sched_group_span(group))) {
			printk(KERN_CONT "\n");
			printk(KERN_ERR "ERROR: empty group\n");
			بشکن;
		}

		اگر (!(sd->flags & SD_NUMA) &&
		    cpumask_intersects(groupmask, sched_group_span(group))) {
			printk(KERN_CONT "\n");
			printk(KERN_ERR "ERROR: repeated CPUs\n");
			بشکن;
		}

		cpumask_or(groupmask, groupmask, sched_group_span(group));

		printk(KERN_CONT " %d:{ span=%*pbl",
				group->sgc->id,
				cpumask_pr_args(sched_group_span(group)));

		اگر ((sd->flags & SD_NUMA) &&
		    !cpumask_equal(group_balance_mask(group), sched_group_span(group))) {
			printk(KERN_CONT " mask=%*pbl",
				cpumask_pr_args(group_balance_mask(group)));
		}

		اگر (group->sgc->capacity != SCHED_CAPACITY_SCALE)
			printk(KERN_CONT " cap=%lu", group->sgc->capacity);

		اگر (group == sd->groups && sd->child &&
		    !cpumask_equal(sched_domain_span(sd->child),
				   sched_group_span(group))) {
			printk(KERN_ERR "ERROR: domain->groups does not match domain->child\n");
		}

		printk(KERN_CONT " }");

		group = group->next;

		اگر (group != sd->groups)
			printk(KERN_CONT ",");

	} وقتی (group != sd->groups);
	printk(KERN_CONT "\n");

	اگر (!cpumask_equal(sched_domain_span(sd), groupmask))
		printk(KERN_ERR "ERROR: groups don't span domain->span\n");

	اگر (sd->parent &&
	    !cpumask_subset(groupmask, sched_domain_span(sd->parent)))
		printk(KERN_ERR "ERROR: parent span is not a superset of domain->span\n");
	بازگشت 0;
}

ایستا تهی sched_domain_debug(ساختار sched_domain *sd, صحیح cpu)
{
	صحیح level = 0;

	اگر (!sched_debug_verbose)
		بازگشت;

	اگر (!sd) {
		printk(KERN_DEBUG "CPU%d attaching NULL sched-domain.\n", cpu);
		بازگشت;
	}

	printk(KERN_DEBUG "CPU%d attaching sched-domain(s):\n", cpu);

	برای (;;) {
		اگر (sched_domain_debug_one(sd, cpu, level, sched_domains_tmpmask))
			بشکن;
		level++;
		sd = sd->parent;
		اگر (!sd)
			بشکن;
	}
}

/* Generate a mask of SD flags with the SDF_NEEDS_GROUPS metaflag */
#define SD_FLAG(name, mflags) (name * !!((mflags) & SDF_NEEDS_GROUPS)) |
ایستا ثابت unsigned صحیح SD_DEGENERATE_GROUPS_MASK =
#شامل <linux/sched/sd_flags.h>
0;
#undef SD_FLAG

ایستا صحیح sd_degenerate(ساختار sched_domain *sd)
{
	اگر (cpumask_weight(sched_domain_span(sd)) == 1)
		بازگشت 1;

	/* Following flags need at least 2 groups */
	اگر ((sd->flags & SD_DEGENERATE_GROUPS_MASK) &&
	    (sd->groups != sd->groups->next))
		بازگشت 0;

	/* Following flags don't use groups */
	اگر (sd->flags & (SD_WAKE_AFFINE))
		بازگشت 0;

	بازگشت 1;
}

ایستا صحیح
sd_parent_degenerate(ساختار sched_domain *sd, ساختار sched_domain *parent)
{
	unsigned long cflags = sd->flags, pflags = parent->flags;

	اگر (sd_degenerate(parent))
		بازگشت 1;

	اگر (!cpumask_equal(sched_domain_span(sd), sched_domain_span(parent)))
		بازگشت 0;

	/* Flags needing groups don't count if only 1 group in parent */
	اگر (parent->groups == parent->groups->next)
		pflags &= ~SD_DEGENERATE_GROUPS_MASK;

	اگر (~cflags & pflags)
		بازگشت 0;

	بازگشت 1;
}

#اگر defined(CONFIG_ENERGY_MODEL) && defined(CONFIG_CPU_FREQ_GOV_SCHEDUTIL)
DEFINE_STATIC_کلید_FALSE(sched_energy_present);
ایستا unsigned صحیح sysctl_sched_energy_aware = 1;
ایستا DEFINE_MUTEX(sched_energy_mutex);
ایستا bool sched_energy_update;

ایستا bool sched_is_eas_possible(ثابت ساختار cpumask *cpu_mask)
{
	bool any_asym_capacity = false;
	صحیح i;

	/* EAS is enabled for asymmetric CPU capacity topologies. */
	for_each_cpu(i, cpu_mask) {
		اگر (rcu_access_pointer(per_cpu(sd_asym_cpucapacity, i))) {
			any_asym_capacity = true;
			بشکن;
		}
	}
	اگر (!any_asym_capacity) {
		اگر (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS, CPUs do not have asymmetric capacities\n",
				cpumask_pr_args(cpu_mask));
		}
		بازگشت false;
	}

	/* EAS definitely does *not* handle SMT */
	اگر (sched_smt_active()) {
		اگر (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS, SMT is not supported\n",
				cpumask_pr_args(cpu_mask));
		}
		بازگشت false;
	}

	اگر (!arch_scale_freq_invariant()) {
		اگر (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS: frequency-invariant load tracking not yet supported",
				cpumask_pr_args(cpu_mask));
		}
		بازگشت false;
	}

	اگر (!cpufreq_ready_for_eas(cpu_mask)) {
		اگر (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS: cpufreq is not ready\n",
				cpumask_pr_args(cpu_mask));
		}
		بازگشت false;
	}

	بازگشت true;
}

تهی rebuild_sched_domains_energy(تهی)
{
	mutex_lock(&sched_energy_mutex);
	sched_energy_update = true;
	rebuild_sched_domains();
	sched_energy_update = false;
	mutex_unlock(&sched_energy_mutex);
}

#ifdef CONFIG_SYSCTL
ایستا صحیح sched_energy_aware_handler(ثابت ساختار ctl_table *table, صحیح اكتب,
		تهی *buffer, size_t *lenp, loff_t *ppos)
{
	صحیح ret;

	اگر (اكتب && !capable(CAP_SYS_ADMIN))
		بازگشت -EPERM;

	اگر (!sched_is_eas_possible(cpu_active_mask)) {
		اگر (اكتب) {
			بازگشت -EOPNOTSUPP;
		} وگرنه {
			*lenp = 0;
			بازگشت 0;
		}
	}

	ret = proc_dointvec_minmax(table, اكتب, buffer, lenp, ppos);
	اگر (!ret && اكتب) {
		اگر (sysctl_sched_energy_aware != sched_energy_enabled())
			rebuild_sched_domains_energy();
	}

	بازگشت ret;
}

ایستا ثابت ساختار ctl_table sched_energy_aware_sysctls[] = {
	{
		.procname       = "sched_energy_aware",
		.data           = &sysctl_sched_energy_aware,
		.maxlen         = اندازه(unsigned صحیح),
		.mode           = 0644,
		.proc_handler   = sched_energy_aware_handler,
		.extra1         = SYSCTL_ZERO,
		.extra2         = SYSCTL_ONE,
	},
};

ایستا صحیح __init sched_energy_aware_sysctl_init(تهی)
{
	register_sysctl_init("kernel", sched_energy_aware_sysctls);
	بازگشت 0;
}

late_initcall(sched_energy_aware_sysctl_init);
#endif /* CONFIG_SYSCTL */

ایستا تهی free_pd(ساختار perf_domain *pd)
{
	ساختار perf_domain *tmp;

	وقتی (pd) {
		tmp = pd->next;
		kfree(pd);
		pd = tmp;
	}
}

ایستا ساختار perf_domain *find_pd(ساختار perf_domain *pd, صحیح cpu)
{
	وقتی (pd) {
		اگر (cpumask_test_cpu(cpu, perf_domain_span(pd)))
			بازگشت pd;
		pd = pd->next;
	}

	بازگشت پوچ;
}

ایستا ساختار perf_domain *pd_init(صحیح cpu)
{
	ساختار em_perf_domain *obj = em_cpu_get(cpu);
	ساختار perf_domain *pd;

	اگر (!obj) {
		اگر (sched_debug())
			pr_info("%s: no EM found for CPU%d\n", __func__, cpu);
		بازگشت پوچ;
	}

	pd = kzalloc_obj(*pd);
	اگر (!pd)
		بازگشت پوچ;
	pd->em_pd = obj;

	بازگشت pd;
}

ایستا تهی perf_domain_debug(ثابت ساختار cpumask *cpu_map,
						ساختار perf_domain *pd)
{
	اگر (!sched_debug() || !pd)
		بازگشت;

	printk(KERN_DEBUG "root_domain %*pbl:", cpumask_pr_args(cpu_map));

	وقتی (pd) {
		printk(KERN_CONT " pd%d:{ cpus=%*pbl nr_pstate=%d }",
				cpumask_first(perf_domain_span(pd)),
				cpumask_pr_args(perf_domain_span(pd)),
				em_pd_nr_perf_states(pd->em_pd));
		pd = pd->next;
	}

	printk(KERN_CONT "\n");
}

ایستا تهی destroy_perf_domain_rcu(ساختار rcu_head *rp)
{
	ساختار perf_domain *pd;

	pd = container_of(rp, ساختار perf_domain, rcu);
	free_pd(pd);
}

ایستا تهی sched_energy_set(bool has_eas)
{
	اگر (!has_eas && sched_energy_enabled()) {
		اگر (sched_debug())
			pr_info("%s: stopping EAS\n", __func__);
		static_branch_disable_cpuslocked(&sched_energy_present);
	} وگرنه اگر (has_eas && !sched_energy_enabled()) {
		اگر (sched_debug())
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
ایستا bool build_perf_domains(ثابت ساختار cpumask *cpu_map)
{
	صحیح i;
	ساختار perf_domain *pd = پوچ, *tmp;
	صحیح cpu = cpumask_first(cpu_map);
	ساختار ریشه_domain *rd = cpu_rq(cpu)->rd;

	اگر (!sysctl_sched_energy_aware)
		برو آزاد;

	اگر (!sched_is_eas_possible(cpu_map))
		برو آزاد;

	for_each_cpu(i, cpu_map) {
		/* Skip already covered CPUs. */
		اگر (find_pd(pd, i))
			ادامه;

		/* Create the new pd and add it to the local list. */
		tmp = pd_init(i);
		اگر (!tmp)
			برو آزاد;
		tmp->next = pd;
		pd = tmp;
	}

	perf_domain_debug(cpu_map, pd);

	/* Attach the new list of performance domains to the root domain. */
	tmp = rd->pd;
	rcu_assign_pointer(rd->pd, pd);
	اگر (tmp)
		call_rcu(&tmp->rcu, destroy_perf_domain_rcu);

	بازگشت !!pd;

آزاد:
	free_pd(pd);
	tmp = rd->pd;
	rcu_assign_pointer(rd->pd, پوچ);
	اگر (tmp)
		call_rcu(&tmp->rcu, destroy_perf_domain_rcu);

	بازگشت false;
}
#وگرنه /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL): */
ایستا تهی free_pd(ساختار perf_domain *pd) { }
#endif /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL) */

ایستا تهی free_rootdomain(ساختار rcu_head *rcu)
{
	ساختار ریشه_domain *rd = container_of(rcu, ساختار ریشه_domain, rcu);

	cpupri_cleanup(&rd->cpupri);
	cpudl_cleanup(&rd->cpudl);
	free_cpumask_var(rd->dlo_mask);
	free_cpumask_var(rd->rto_mask);
	free_cpumask_var(rd->online);
	free_cpumask_var(rd->span);
	free_pd(rd->pd);
	kfree(rd);
}

تهی rq_attach_ریشه(ساختار rq *rq, ساختار ریشه_domain *rd)
{
	ساختار ریشه_domain *old_rd = پوچ;
	ساختار rq_flags rf;

	rq_lock_irqsave(rq, &rf);

	اگر (rq->rd) {
		old_rd = rq->rd;

		اگر (cpumask_test_cpu(rq->cpu, old_rd->online))
			set_rq_offline(rq);

		cpumask_clear_cpu(rq->cpu, old_rd->span);

		/*
		 * If we don't want to free the old_rd yet then
		 * set old_rd to NULL to skip the freeing later
		 * in this function:
		 */
		اگر (!atomic_dec_and_test(&old_rd->refcount))
			old_rd = پوچ;
	}

	atomic_inc(&rd->refcount);
	rq->rd = rd;

	cpumask_set_cpu(rq->cpu, rd->span);
	اگر (cpumask_test_cpu(rq->cpu, cpu_active_mask))
		set_rq_online(rq);

	/*
	 * Because the rq is not a task, dl_add_task_root_domain() did not
	 * move the fair server bw to the rd if it already started.
	 * Add it now.
	 */
	اگر (rq->fair_server.dl_server)
		__dl_server_attach_root(&rq->fair_server, rq);

#ifdef CONFIG_SCHED_CLASS_EXT
	اگر (rq->ext_server.dl_server)
		__dl_server_attach_root(&rq->ext_server, rq);
#endif

	rq_unlock_irqrestore(rq, &rf);

	اگر (old_rd)
		call_rcu(&old_rd->rcu, free_rootdomain);
}

تهی sched_get_rd(ساختار ریشه_domain *rd)
{
	atomic_inc(&rd->refcount);
}

تهی sched_put_rd(ساختار ریشه_domain *rd)
{
	اگر (!atomic_dec_and_test(&rd->refcount))
		بازگشت;

	call_rcu(&rd->rcu, free_rootdomain);
}

ایستا صحیح init_rootdomain(ساختار ریشه_domain *rd)
{
	اگر (!zalloc_cpumask_var(&rd->span, GFP_KERNEL))
		برو out;
	اگر (!zalloc_cpumask_var(&rd->online, GFP_KERNEL))
		برو free_span;
	اگر (!zalloc_cpumask_var(&rd->dlo_mask, GFP_KERNEL))
		برو free_online;
	اگر (!zalloc_cpumask_var(&rd->rto_mask, GFP_KERNEL))
		برو free_dlo_mask;

#ifdef HAVE_RT_افزودن_IPI
	rd->rto_cpu = -1;
	raw_spin_lock_init(&rd->rto_lock);
	rd->rto_افزودن_work = IRQ_WORK_INIT_HARD(rto_افزودن_irq_work_func);
#endif

	rd->visit_cookie = 0;
	init_dl_bw(&rd->dl_bw);
	اگر (cpudl_init(&rd->cpudl) != 0)
		برو free_rto_mask;

	اگر (cpupri_init(&rd->cpupri) != 0)
		برو free_cpudl;
	بازگشت 0;

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
	بازگشت -ENOMEM;
}

/*
 * By default the system creates a single root-domain with all CPUs as
 * members (mimicking the global state we have today).
 */
ساختار ریشه_domain def_ریشه_domain;

تهی __init init_defrootdomain(تهی)
{
	init_rootdomain(&def_ریشه_domain);

	atomic_set(&def_ریشه_domain.refcount, 1);
}

ایستا ساختار ریشه_domain *alloc_rootdomain(تهی)
{
	ساختار ریشه_domain *rd;

	rd = kzalloc_obj(*rd);
	اگر (!rd)
		بازگشت پوچ;

	اگر (init_rootdomain(rd) != 0) {
		kfree(rd);
		بازگشت پوچ;
	}

	بازگشت rd;
}

ایستا تهی free_sched_groups(ساختار sched_group *sg, صحیح free_sgc)
{
	ساختار sched_group *tmp, *first;

	اگر (!sg)
		بازگشت;

	first = sg;
	انجامبده {
		tmp = sg->next;

		اگر (free_sgc && atomic_dec_and_test(&sg->sgc->ref))
			kfree(sg->sgc);

		اگر (atomic_dec_and_test(&sg->ref))
			kfree(sg);
		sg = tmp;
	} وقتی (sg != first);
}

ایستا تهی free_sched_domain_shared(ساختار sched_domain_shared *sds)
{
	اگر (sds && atomic_dec_and_test(&sds->ref))
		kfree(sds);
}

ایستا تهی destroy_sched_domain(ساختار sched_domain *sd)
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

ایستا تهی destroy_sched_domains_rcu(ساختار rcu_head *rcu)
{
	ساختار sched_domain *sd = container_of(rcu, ساختار sched_domain, rcu);

	وقتی (sd) {
		ساختار sched_domain *parent = sd->parent;
		destroy_sched_domain(sd);
		sd = parent;
	}
}

ایستا تهی destroy_sched_domains(ساختار sched_domain *sd)
{
	اگر (sd)
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
DEFINE_PER_CPU(ساختار sched_domain __rcu *, sd_llc);
DEFINE_PER_CPU(صحیح, sd_llc_size);
DEFINE_PER_CPU(صحیح, sd_llc_id) = -1;
DEFINE_PER_CPU(صحیح, sd_share_id);
DEFINE_PER_CPU(ساختار sched_domain_shared __rcu *, sd_llc_shared);
DEFINE_PER_CPU(ساختار sched_domain_shared __rcu *, sd_balance_shared);
DEFINE_PER_CPU(ساختار sched_domain __rcu *, sd_numa);
DEFINE_PER_CPU(ساختار sched_domain __rcu *, sd_asym_packing);
DEFINE_PER_CPU(ساختار sched_domain __rcu *, sd_asym_cpucapacity);

DEFINE_STATIC_کلید_FALSE(sched_asym_cpucapacity);
DEFINE_STATIC_کلید_FALSE(sched_cluster_active);

ایستا تهی update_بالاترین_cache_domain(صحیح cpu)
{
	ساختار sched_domain_shared *sds = پوچ;
	ساختار sched_domain *sd;
	صحیح id = cpu;
	صحیح size = 1;

	sd = highest_flag_domain(cpu, SD_SHARE_LLC);
	اگر (sd) {
		id = cpumask_first(sched_domain_span(sd));
		size = cpumask_weight(sched_domain_span(sd));

		/* If sd_llc exists, sd_llc_shared should exist too. */
		WARN_ON_ONCE(!sd->shared);
		sds = sd->shared;
	}

	rcu_assign_pointer(per_cpu(sd_llc, cpu), sd);
	per_cpu(sd_llc_size, cpu) = size;
	rcu_assign_pointer(per_cpu(sd_llc_shared, cpu), sds);

	sd = lowest_flag_domain(cpu, SD_CLUSTER);
	اگر (sd)
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
	اگر (sd && sd->shared)
		sds = sd->shared;

	rcu_assign_pointer(per_cpu(sd_asym_cpucapacity, cpu), sd);
	rcu_assign_pointer(per_cpu(sd_balance_shared, cpu), sds);
}

/*
 * Attach the domain 'sd' to 'cpu' as its base domain. Callers must
 * hold the hotplug lock.
 */
ایستا تهی
cpu_attach_domain(ساختار sched_domain *sd, ساختار ریشه_domain *rd, صحیح cpu)
{
	ساختار rq *rq = cpu_rq(cpu);
	ساختار sched_domain *tmp;

	/* Remove the sched domains which do not contribute to scheduling. */
	برای (tmp = sd; tmp; ) {
		ساختار sched_domain *parent = tmp->parent;
		اگر (!parent)
			بشکن;

		اگر (sd_parent_degenerate(tmp, parent)) {
			tmp->parent = parent->parent;

			/* Pick reference to parent->shared. */
			اگر (parent->shared) {
				/*
				 * It is safe to free a sd->shared that
				 * has not been published yet. If a
				 * sd->shared was published, the refcount
				 * will end up being non-zero and it will
				 * not be freed here.
				 */
				free_sched_domain_shared(tmp->shared);
				tmp->shared = parent->shared;
				parent->shared = پوچ;
			}

			اگر (parent->parent) {
				parent->parent->child = tmp;
				parent->parent->groups->flags = tmp->flags;
			}

			/*
			 * Transfer SD_PREFER_SIBLING down in case of a
			 * degenerate parent; the spans match for this
			 * so the property transfers.
			 */
			اگر (parent->flags & SD_PREFER_SIBLING)
				tmp->flags |= SD_PREFER_SIBLING;
			destroy_sched_domain(parent);
		} وگرنه
			tmp = tmp->parent;
	}

	اگر (sd && sd_degenerate(sd)) {
		tmp = sd;
		sd = sd->parent;

		اگر (sd) {
			ساختار sched_group *sg = sd->groups;

#ifdef CONFIG_SCHED_CACHE
			/* move buffer to parent as child is being destroyed */
			sd->llc_counts = tmp->llc_counts;
			sd->llc_max = tmp->llc_max;
			sd->llc_bytes = tmp->llc_bytes;
			/* make sure destroy_sched_domain() does not free it */
			tmp->llc_counts = پوچ;
			tmp->llc_max = 0;
			tmp->llc_bytes = 0;
#endif
			/*
			 * sched groups hold the flags of the child sched
			 * domain for convenience. Clear such flags since
			 * the child is being destroyed.
			 */
			انجامبده {
				sg->flags = 0;
			} وقتی (sg != sd->groups);

			sd->child = پوچ;
		}

		destroy_sched_domain(tmp);
	}

	sched_domain_debug(sd, cpu);

	rq_attach_ریشه(rq, rd);
	tmp = rq->sd;
	rcu_assign_pointer(rq->sd, sd);
	dirty_sched_domain_sysctl(cpu);
	destroy_sched_domains(tmp);

	update_بالاترین_cache_domain(cpu);
}

ساختار s_data {
	ساختار sched_domain_shared * __percpu *sds;
	ساختار sched_domain * __percpu *sd;
	ساختار ریشه_domain	*rd;
};

شمارش s_alloc {
	sa_rootdomain,
	sa_sd,
	sa_sd_shared,
	sa_sd_storage,
	sa_none,
};

#ifdef CONFIG_SCHED_CACHE
/* hardware support for cache aware scheduling */
DEFINE_STATIC_کلید_FALSE(sched_cache_present);
/*
 * Indicator of whether cache aware scheduling
 * is active, used by the scheduler.
 */
DEFINE_STATIC_کلید_FALSE(sched_cache_active);
/* user wants cache aware scheduling [0 or 1] */
صحیح sysctl_sched_cache_user = 1;

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
ایستا unsigned long get_effective_llc_bytes(صحیح cpu,
					     ساختار sched_domain *sd)
{
	ساختار cacheinfo *ci;
	unsigned صحیح hw_weight;

	ci = get_cpu_cacheinfo_llc(cpu);
	اگر (!ci)
		بازگشت 0;

	hw_weight = cpumask_weight(&ci->shared_cpu_map);
	اگر (!hw_weight)
		بازگشت 0;

	بازگشت div_u64((u64)ci->size * sd->span_weight, hw_weight);
}

ایستا bool alloc_sd_llc(ثابت ساختار cpumask *cpu_map,
			 ساختار s_data *d)
{
	ساختار sched_domain *sd, *بالاترین_llc, *parent;
	unsigned صحیح *p;
	صحیح i;

	for_each_cpu(i, cpu_map) {
		sd = *per_cpu_ptr(d->sd, i);
		اگر (!sd)
			برو err;

		p = kcalloc_گره(max_lid + 1, اندازه(unsigned صحیح),
				 GFP_KERNEL, cpu_to_گره(i));
		اگر (!p)
			برو err;

		بالاترین_llc = sd;
		/*
		 * Find the topmost SD_SHARE_LLC domain.
		 * Not yet attached to the CPU, so per_cpu(sd_llc, i)
		 * can not be used.
		 */
		وقتی ((parent = rcu_dereference_protected(بالاترین_llc->parent, true)) &&
		       (parent->flags & SD_SHARE_LLC))
			بالاترین_llc = parent;

		اگر (بالاترین_llc->flags & SD_SHARE_LLC) {
			sd->llc_max = max_lid + 1;
			sd->llc_counts = p;
			sd->llc_bytes = get_effective_llc_bytes(i, بالاترین_llc);
		} وگرنه {
			/* avoid memory leak */
			kfree(p);
		}
	}

	بازگشت true;
err:
	for_each_cpu(i, cpu_map) {
		sd = *per_cpu_ptr(d->sd, i);
		اگر (sd) {
			kfree(sd->llc_counts);
			sd->llc_counts = پوچ;
			sd->llc_max = 0;
			sd->llc_bytes = 0;
		}
	}

	بازگشت false;
}

/*
 * Enable/disable cache aware scheduling according to
 * user input and the presence of hardware support.
 */
ایستا تهی _sched_cache_active_set(تهی)
{
	lockdep_assert_cpus_held();
	lockdep_assert_held(&sched_domains_mutex);

	/* hardware does not support */
	اگر (!static_branch_likely(&sched_cache_present)) {
		static_branch_disable_cpuslocked(&sched_cache_active);
		اگر (sched_debug())
			pr_info("%s: cache aware scheduling not supported on this platform\n", __func__);
		بازگشت;
	}

	/*
	 * user wants it or not ?
	 * TBD: read before writing the static key.
	 * It is not in the critical path, leave as-is
	 * for now.
	 */
	اگر (sysctl_sched_cache_user) {
		static_branch_enable_cpuslocked(&sched_cache_active);
		اگر (sched_debug())
			pr_info("%s: enabling cache aware scheduling\n", __func__);
	} وگرنه {
		static_branch_disable_cpuslocked(&sched_cache_active);
		اگر (sched_debug())
			pr_info("%s: disabling cache aware scheduling\n", __func__);
	}
}

/* used by debugfs */
تهی sched_cache_active_set(تهی)
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
تهی sched_update_llc_bytes(ثابت ساختار cpumask *cpus)
{
	ساختار sched_domain *sd, *sdp;
	unsigned صحیح i;

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
		اگر (!sdp)
			ادامه;

		sd = rcu_dereference_sched_domain(cpu_rq(i)->sd);
		اگر (sd)
			sd->llc_bytes = get_effective_llc_bytes(i, sdp);
	}

	sched_domains_mutex_unlock();
}

ایستا تهی sched_cache_set(bool has_multi_llcs)
{
	/*
	 * TBD: check before writing to it. sched domain rebuild
	 * is not in the critical path, leave as-is for now.
	 */
	اگر (has_multi_llcs)
		static_branch_enable_cpuslocked(&sched_cache_present);
	وگرنه
		static_branch_disable_cpuslocked(&sched_cache_present);

	_sched_cache_active_set();
}
#وگرنه
ایستا bool alloc_sd_llc(ثابت ساختار cpumask *cpu_map,
			 ساختار s_data *d)
{
	بازگشت false;
}
ایستا درونخطی تهی sched_cache_set(bool has_multi_llcs) { }
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
ایستا bool sd_in_multi_llcs(ساختار sched_domain *sd)
{
	ساختار sched_domain *sdp = sd->parent;

	/* it does not make sense to aggregate to 1 CPU */
	اگر (sd->span_weight == 1)
		بازگشت false;

	وقتی (sdp && sdp->span_weight == sd->span_weight)
		sdp = sdp->parent;

	بازگشت !!sdp;
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
صحیح group_balance_cpu(ساختار sched_group *sg)
{
	بازگشت cpumask_first(group_balance_mask(sg));
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
ایستا تهی
build_balance_mask(ساختار sched_domain *sd, ساختار sched_group *sg, ساختار cpumask *mask)
{
	ثابت ساختار cpumask *sg_span = sched_group_span(sg);
	ساختار sd_data *sdd = sd->خصوصی;
	ساختار sched_domain *sibling;
	صحیح i;

	cpumask_clear(mask);

	for_each_cpu(i, sg_span) {
		sibling = *per_cpu_ptr(sdd->sd, i);

		/*
		 * Can happen in the asymmetric case, where these siblings are
		 * unused. The mask will not be empty because those CPUs that
		 * do have the top domain _should_ span the domain.
		 */
		اگر (!sibling->child)
			ادامه;

		/* If we would not end up here, we can't continue from here */
		اگر (!cpumask_equal(sg_span, sched_domain_span(sibling->child)))
			ادامه;

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
ایستا ساختار sched_group *
build_group_from_child_sched_domain(ساختار sched_domain *sd, صحیح cpu)
{
	ساختار sched_group *sg;
	ساختار cpumask *sg_span;

	sg = kzalloc_گره(اندازه(ساختار sched_group) + cpumask_size(),
			GFP_KERNEL, cpu_to_گره(cpu));

	اگر (!sg)
		بازگشت پوچ;

	sg_span = sched_group_span(sg);
	اگر (sd->child) {
		cpumask_copy(sg_span, sched_domain_span(sd->child));
		sg->flags = sd->child->flags;
	} وگرنه {
		cpumask_copy(sg_span, sched_domain_span(sd));
	}

	atomic_inc(&sg->ref);
	بازگشت sg;
}

ایستا تهی init_overlap_sched_group(ساختار sched_domain *sd,
				     ساختار sched_group *sg)
{
	ساختار cpumask *mask = sched_domains_tmpmask2;
	ساختار sd_data *sdd = sd->خصوصی;
	ساختار cpumask *sg_span;
	صحیح cpu;

	build_balance_mask(sd, sg, mask);
	cpu = cpumask_first(mask);

	sg->sgc = *per_cpu_ptr(sdd->sgc, cpu);
	اگر (atomic_inc_return(&sg->sgc->ref) == 1)
		cpumask_copy(group_balance_mask(sg), mask);
	وگرنه
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

ایستا ساختار sched_domain *
find_descended_sibling(ساختار sched_domain *sd, ساختار sched_domain *sibling)
{
	/*
	 * The proper descendant would be the one whose child won't span out
	 * of sd
	 */
	وقتی (sibling->child &&
	       !cpumask_subset(sched_domain_span(sibling->child),
			       sched_domain_span(sd)))
		sibling = sibling->child;

	/*
	 * As we are referencing sgc across different topology level, we need
	 * to go down to skip those sched_domains which don't contribute to
	 * scheduling because they will be degenerated in cpu_attach_domain
	 */
	وقتی (sibling->child &&
	       cpumask_equal(sched_domain_span(sibling->child),
			     sched_domain_span(sibling)))
		sibling = sibling->child;

	بازگشت sibling;
}

ایستا صحیح
build_overlap_sched_groups(ساختار sched_domain *sd, صحیح cpu)
{
	ساختار sched_group *first = پوچ, *last = پوچ, *sg;
	ثابت ساختار cpumask *span = sched_domain_span(sd);
	ساختار cpumask *covered = sched_domains_tmpmask;
	ساختار sd_data *sdd = sd->خصوصی;
	ساختار sched_domain *sibling;
	صحیح i;

	cpumask_clear(covered);

	for_each_cpu_wrap(i, span, cpu) {
		ساختار cpumask *sg_span;

		اگر (cpumask_test_cpu(i, covered))
			ادامه;

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
		اگر (!cpumask_test_cpu(i, sched_domain_span(sibling)))
			ادامه;

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
		اگر (sibling->child &&
		    !cpumask_subset(sched_domain_span(sibling->child), span))
			sibling = find_descended_sibling(sd, sibling);

		sg = build_group_from_child_sched_domain(sibling, cpu);
		اگر (!sg)
			برو fail;

		sg_span = sched_group_span(sg);
		cpumask_or(covered, covered, sg_span);

		init_overlap_sched_group(sibling, sg);

		اگر (!first)
			first = sg;
		اگر (last)
			last->next = sg;
		last = sg;
		last->next = first;
	}
	sd->groups = first;

	بازگشت 0;

fail:
	free_sched_groups(first, 0);

	بازگشت -ENOMEM;
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

ایستا ساختار sched_group *get_group(صحیح cpu, ساختار sd_data *sdd)
{
	ساختار sched_domain *sd = *per_cpu_ptr(sdd->sd, cpu);
	ساختار sched_domain *child = sd->child;
	ساختار sched_group *sg;
	bool already_visited;

	اگر (child)
		cpu = cpumask_first(sched_domain_span(child));

	sg = *per_cpu_ptr(sdd->sg, cpu);
	sg->sgc = *per_cpu_ptr(sdd->sgc, cpu);

	/* Increase refcounts for claim_allocations: */
	already_visited = atomic_inc_return(&sg->ref) > 1;
	/* sgc visits should follow a similar trend as sg */
	WARN_ON(already_visited != (atomic_inc_return(&sg->sgc->ref) > 1));

	/* If we have already visited that group, it's already initialized. */
	اگر (already_visited)
		بازگشت sg;

	اگر (child) {
		cpumask_copy(sched_group_span(sg), sched_domain_span(child));
		cpumask_copy(group_balance_mask(sg), sched_group_span(sg));
		sg->flags = child->flags;
	} وگرنه {
		cpumask_set_cpu(cpu, sched_group_span(sg));
		cpumask_set_cpu(cpu, group_balance_mask(sg));
	}

	sg->sgc->capacity = SCHED_CAPACITY_SCALE * cpumask_weight(sched_group_span(sg));
	sg->sgc->min_capacity = SCHED_CAPACITY_SCALE;
	sg->sgc->max_capacity = SCHED_CAPACITY_SCALE;

	بازگشت sg;
}

/*
 * build_sched_groups will build a circular linked list of the groups
 * covered by the given span, will set each group's ->cpumask correctly,
 * and will initialize their ->sgc.
 *
 * Assumes the sched_domain tree is fully constructed
 */
ایستا صحیح
build_sched_groups(ساختار sched_domain *sd, صحیح cpu)
{
	ساختار sched_group *first = پوچ, *last = پوچ;
	ساختار sd_data *sdd = sd->خصوصی;
	ثابت ساختار cpumask *span = sched_domain_span(sd);
	ساختار cpumask *covered;
	صحیح i;

	lockdep_assert_held(&sched_domains_mutex);
	covered = sched_domains_tmpmask;

	cpumask_clear(covered);

	for_each_cpu_wrap(i, span, cpu) {
		ساختار sched_group *sg;

		اگر (cpumask_test_cpu(i, covered))
			ادامه;

		sg = get_group(i, sdd);

		cpumask_or(covered, covered, sched_group_span(sg));

		اگر (!first)
			first = sg;
		اگر (last)
			last->next = sg;
		last = sg;
	}
	last->next = first;
	sd->groups = first;

	بازگشت 0;
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
ایستا تهی init_sched_groups_capacity(صحیح cpu, ساختار sched_domain *sd)
{
	ساختار sched_group *sg = sd->groups;
	ساختار cpumask *mask = sched_domains_tmpmask2;

	WARN_ON(!sg);

	انجامبده {
		صحیح cpu, cores = 0, max_cpu = -1;

		sg->group_weight = cpumask_weight(sched_group_span(sg));

		cpumask_copy(mask, sched_group_span(sg));
		for_each_cpu(cpu, mask) {
			cores++;
			cpumask_andnot(mask, mask, cpu_smt_mask(cpu));
		}
		sg->cores = cores;

		اگر (!(sd->flags & SD_ASYM_PACKING))
			برو next;

		for_each_cpu(cpu, sched_group_span(sg)) {
			اگر (max_cpu < 0)
				max_cpu = cpu;
			وگرنه اگر (sched_asym_prefer(cpu, max_cpu))
				max_cpu = cpu;
		}
		sg->asym_prefer_cpu = max_cpu;

next:
		sg = sg->next;
	} وقتی (sg != sd->groups);

	اگر (cpu != group_balance_cpu(sg))
		بازگشت;

	update_group_capacity(sd, cpu);
}

/* Update the "asym_prefer_cpu" when arch_asym_cpu_priority() changes. */
تهی sched_update_asym_prefer_cpu(صحیح cpu, صحیح old_prio, صحیح new_prio)
{
	صحیح asym_prefer_cpu = cpu;
	ساختار sched_domain *sd;

	guard(rcu)();

	for_each_domain(cpu, sd) {
		ساختار sched_group *sg;
		صحیح group_cpu;

		اگر (!(sd->flags & SD_ASYM_PACKING))
			ادامه;

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
		اگر (cpu != sg->asym_prefer_cpu) {
			/*
			 * Since the parent is a superset of the current group,
			 * if the cpu is not the "asym_prefer_cpu" at the
			 * current level, it cannot be the preferred CPU at a
			 * higher levels either.
			 */
			اگر (!sched_asym_prefer(cpu, sg->asym_prefer_cpu))
				بازگشت;

			WRITE_ONCE(sg->asym_prefer_cpu, cpu);
			ادامه;
		}

		/* Ranking has improved; CPU is still the preferred one. */
		اگر (new_prio >= old_prio)
			ادامه;

		for_each_cpu(group_cpu, sched_group_span(sg)) {
			اگر (sched_asym_prefer(group_cpu, asym_prefer_cpu))
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
LIST_HEAD(asym_cap_list);

/*
 * Verify whether there is any CPU capacity asymmetry in a given sched domain.
 * Provides sd_flags reflecting the asymmetry scope.
 */
ایستا درونخطی صحیح
asym_cpu_capacity_classify(ثابت ساختار cpumask *sd_span,
			   ثابت ساختار cpumask *cpu_map)
{
	ساختار asym_cap_data *entry;
	صحیح شمار = 0, miss = 0;

	/*
	 * Count how many unique CPU capacities this domain spans across
	 * (compare sched_domain CPUs mask with ones representing  available
	 * CPUs capacities). Take into account CPUs that might be offline:
	 * skip those.
	 */
	list_for_each_entry(entry, &asym_cap_list, link) {
		اگر (cpumask_intersects(sd_span, cpu_capacity_span(entry)))
			++شمار;
		وگرنه اگر (cpumask_intersects(cpu_map, cpu_capacity_span(entry)))
			++miss;
	}

	WARN_ON_ONCE(!شمار && !list_empty(&asym_cap_list));

	/* No asymmetry detected */
	اگر (شمار < 2)
		بازگشت 0;
	/* Some of the available CPU capacity values have not been detected */
	اگر (miss)
		بازگشت SD_ASYM_CPUCAPACITY;

	/* Full asymmetry */
	بازگشت SD_ASYM_CPUCAPACITY | SD_ASYM_CPUCAPACITY_FULL;

}

ایستا تهی free_asym_cap_entry(ساختار rcu_head *head)
{
	ساختار asym_cap_data *entry = container_of(head, ساختار asym_cap_data, rcu);
	kfree(entry);
}

ایستا درونخطی تهی asym_cpu_capacity_update_data(صحیح cpu)
{
	unsigned long capacity = arch_scale_cpu_capacity(cpu);
	ساختار asym_cap_data *درج_entry = پوچ;
	ساختار asym_cap_data *entry;

	/*
	 * Search if capacity already exits. If not, track which the entry
	 * where we should insert to keep the list ordered descending.
	 */
	list_for_each_entry(entry, &asym_cap_list, link) {
		اگر (capacity == entry->capacity)
			برو done;
		وگرنه اگر (!درج_entry && capacity > entry->capacity)
			درج_entry = list_prev_entry(entry, link);
	}

	entry = kzalloc(اندازه(*entry) + cpumask_size(), GFP_KERNEL);
	اگر (WARN_ONCE(!entry, "Failed to allocate memory for asymmetry data\n"))
		بازگشت;
	entry->capacity = capacity;

	/* If NULL then the new capacity is the smallest, add last. */
	اگر (!درج_entry)
		list_add_tail_rcu(&entry->link, &asym_cap_list);
	وگرنه
		list_add_rcu(&entry->link, &درج_entry->link);
done:
	__cpumask_set_cpu(cpu, cpu_capacity_span(entry));
}

/*
 * Build-up/update list of CPUs grouped by their capacities
 * An update requires explicit request to rebuild sched domains
 * with state indicating CPU topology changes.
 */
ایستا تهی asym_cpu_capacity_scan(تهی)
{
	ساختار asym_cap_data *entry, *next;
	صحیح cpu;

	list_for_each_entry(entry, &asym_cap_list, link)
		cpumask_clear(cpu_capacity_span(entry));

	for_each_cpu_and(cpu, cpu_possible_mask, housekeeping_cpumask(HK_TYPE_DOMAIN))
		asym_cpu_capacity_update_data(cpu);

	list_for_each_entry_safe(entry, next, &asym_cap_list, link) {
		اگر (cpumask_empty(cpu_capacity_span(entry))) {
			list_del_rcu(&entry->link);
			call_rcu(&entry->rcu, free_asym_cap_entry);
		}
	}

	/*
	 * Only one capacity value has been detected i.e. this system is symmetric.
	 * No need to keep this data around.
	 */
	اگر (list_is_singular(&asym_cap_list)) {
		entry = list_first_entry(&asym_cap_list, typeof(*entry), link);
		list_del_rcu(&entry->link);
		call_rcu(&entry->rcu, free_asym_cap_entry);
	}
}

/*
 * Initializers for schedule domains
 * Non-inlined to reduce accumulated stack pressure in build_sched_domains()
 */

ایستا صحیح default_relax_domain_level = -1;
صحیح sched_domain_level_max;

ایستا صحیح __init setup_relax_domain_level(نویسه *str)
{
	اگر (kstrtoint(str, 0, &default_relax_domain_level))
		pr_warn("Unable to set relax_domain_level\n");

	بازگشت 1;
}
__setup("relax_domain_level=", setup_relax_domain_level);

ایستا تهی set_domain_attribute(ساختار sched_domain *sd,
				 ساختار sched_domain_attr *attr)
{
	صحیح request;

	اگر (!attr || attr->relax_domain_level < 0) {
		اگر (default_relax_domain_level < 0)
			بازگشت;
		request = default_relax_domain_level;
	} وگرنه
		request = attr->relax_domain_level;

	اگر (sd->level >= request) {
		/* Turn off idle balance on this domain: */
		sd->flags &= ~(SD_BALANCE_WAKE|SD_BALANCE_NEWIDLE);
	}
}

ایستا تهی __sdt_free(ثابت ساختار cpumask *cpu_map);
ایستا صحیح __sdt_alloc(ثابت ساختار cpumask *cpu_map);

ایستا تهی __sds_free(ساختار s_data *d, ثابت ساختار cpumask *cpu_map);
ایستا صحیح __sds_alloc(ساختار s_data *d, ثابت ساختار cpumask *cpu_map);

ایستا تهی __free_domain_allocs(ساختار s_data *d, شمارش s_alloc what,
				 ثابت ساختار cpumask *cpu_map)
{
	انتخاب (what) {
	حالت sa_rootdomain:
		اگر (!atomic_read(&d->rd->refcount))
			free_rootdomain(&d->rd->rcu);
		fallthrough;
	حالت sa_sd:
		free_percpu(d->sd);
		fallthrough;
	حالت sa_sd_shared:
		__sds_free(d, cpu_map);
		fallthrough;
	حالت sa_sd_storage:
		__sdt_free(cpu_map);
		fallthrough;
	حالت sa_none:
		بشکن;
	}
}

ایستا شمارش s_alloc
__visit_domain_allocation_hell(ساختار s_data *d, ثابت ساختار cpumask *cpu_map)
{
	memset(d, 0, اندازه(*d));

	اگر (__sdt_alloc(cpu_map))
		بازگشت sa_sd_storage;
	اگر (__sds_alloc(d, cpu_map))
		بازگشت sa_sd_shared;
	d->sd = alloc_percpu(ساختار sched_domain *);
	اگر (!d->sd)
		بازگشت sa_sd_shared;
	d->rd = alloc_rootdomain();
	اگر (!d->rd)
		بازگشت sa_sd;

	بازگشت sa_rootdomain;
}

/*
 * NULL the sd_data elements we've used to build the sched_domain and
 * sched_group structure so that the subsequent __free_domain_allocs()
 * will not free the data we're using.
 */
ایستا تهی claim_allocations(صحیح cpu, ساختار s_data *d)
{
	ساختار sched_domain *sd;

	اگر (atomic_read(&(*per_cpu_ptr(d->sds, cpu))->ref))
		*per_cpu_ptr(d->sds, cpu) = پوچ;

	برای (sd = *per_cpu_ptr(d->sd, cpu); sd; sd = sd->parent) {
		ساختار sd_data *sdd = sd->خصوصی;

		WARN_ON_ONCE(*per_cpu_ptr(sdd->sd, cpu) != sd);
		*per_cpu_ptr(sdd->sd, cpu) = پوچ;

		اگر (atomic_read(&(*per_cpu_ptr(sdd->sg, cpu))->ref))
			*per_cpu_ptr(sdd->sg, cpu) = پوچ;

		اگر (atomic_read(&(*per_cpu_ptr(sdd->sgc, cpu))->ref))
			*per_cpu_ptr(sdd->sgc, cpu) = پوچ;
	}
}

#ifdef CONFIG_NUMA
شمارش numa_topology_type sched_numa_topology_type;

/*
 * sched_domains_numa_distance is derived from sched_numa_node_distance
 * and provides a simplified view of NUMA distances used specifically
 * for building NUMA scheduling domains.
 */
ایستا صحیح			sched_domains_numa_levels;
ایستا صحیح			sched_numa_گره_levels;

صحیح				sched_max_numa_distance;
ایستا صحیح			*sched_domains_numa_distance;
ایستا صحیح			*sched_numa_گره_distance;
ایستا ساختار cpumask		***sched_domains_numa_masks;
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

ایستا ساختار sched_domain *
sd_init(ساختار sched_domain_topology_level *tl,
	ثابت ساختار cpumask *cpu_map,
	ساختار sched_domain *child, صحیح cpu)
{
	ساختار sd_data *sdd = &tl->data;
	ساختار sched_domain *sd = *per_cpu_ptr(sdd->sd, cpu);
	صحیح sd_id, sd_weight, sd_flags = 0;
	ساختار cpumask *sd_span;
	u64 now = sched_clock();

	sd_span = sched_domain_span(sd);
	cpumask_and(sd_span, cpu_map, tl->mask(tl, cpu));
	sd_weight = cpumask_weight(sd_span);
	sd_id = cpumask_first(sd_span);

	اگر (tl->sd_flags)
		sd_flags = (*tl->sd_flags)();
	اگر (WARN_ONCE(sd_flags & ~TOPOLOGY_SD_FLAGS,
		      "wrong sd_flags in topology description\n"))
		sd_flags &= TOPOLOGY_SD_FLAGS;
#ifdef CONFIG_SCHED_SMT
	اگر (sched_smt_asym_packing && (sd_flags & SD_SHARE_CPUCAPACITY))
		sd_flags |= SD_ASYM_PACKING;
#endif
	sd_flags |= asym_cpu_capacity_classify(sd_span, cpu_map);

	*sd = (ساختار sched_domain){
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
	اگر (sd->flags & SD_SHARE_CPUCAPACITY) {
		sd->imbalance_pct = 110;

	} وگرنه اگر (sd->flags & SD_SHARE_LLC) {
		sd->imbalance_pct = 117;
		sd->cache_nice_tries = 1;

#ifdef CONFIG_NUMA
	} وگرنه اگر (sd->flags & SD_NUMA) {
		sd->cache_nice_tries = 2;

		sd->flags &= ~SD_PREFER_SIBLING;
		sd->flags |= SD_SERIALIZE;
		اگر (sched_domains_numa_distance[tl->numa_level] > گره_reclaim_distance) {
			sd->flags &= ~(SD_BALANCE_EXEC |
				       SD_BALANCE_FORK |
				       SD_WAKE_AFFINE);
		}

#endif /* CONFIG_NUMA */
	} وگرنه {
		sd->cache_nice_tries = 1;
	}

	sd->خصوصی = sdd;

	بازگشت sd;
}

#ifdef CONFIG_SCHED_SMT
صحیح cpu_smt_flags(تهی)
{
	بازگشت SD_SHARE_CPUCAPACITY | SD_SHARE_LLC;
}

ثابت ساختار cpumask *tl_smt_mask(ساختار sched_domain_topology_level *tl, صحیح cpu)
{
	بازگشت cpu_smt_mask(cpu);
}
#endif

#ifdef CONFIG_SCHED_CLUSTER
صحیح cpu_cluster_flags(تهی)
{
	بازگشت SD_CLUSTER | SD_SHARE_LLC;
}

ثابت ساختار cpumask *tl_cls_mask(ساختار sched_domain_topology_level *tl, صحیح cpu)
{
	بازگشت cpu_clustergroup_mask(cpu);
}
#endif

#ifdef CONFIG_SCHED_MC
صحیح cpu_core_flags(تهی)
{
	بازگشت SD_SHARE_LLC;
}

ثابت ساختار cpumask *tl_mc_mask(ساختار sched_domain_topology_level *tl, صحیح cpu)
{
	بازگشت cpu_coregroup_mask(cpu);
}

/*
 * Majority of architectures have LLC at MC domain level with exception
 * such as powerpc. Provide a way for arch to specify where its LLC is
 * if it falls in exception category
 */
# ifndef arch_llc_mask
#define arch_llc_mask(cpu) cpu_coregroup_mask(cpu)
# endif

#وگرنه
#define arch_llc_mask(cpu) cpumask_of(cpu)
#endif

#define llc_mask(cpu) arch_llc_mask(cpu)

ثابت ساختار cpumask *tl_pkg_mask(ساختار sched_domain_topology_level *tl, صحیح cpu)
{
	بازگشت cpu_گره_mask(cpu);
}

/*
 * Topology list, bottom-up.
 */
ایستا ساختار sched_domain_topology_level default_topology[] = {
#ifdef CONFIG_SCHED_SMT
	SDTL_INIT(tl_smt_mask, cpu_smt_flags, SMT),
#endif

#ifdef CONFIG_SCHED_CLUSTER
	SDTL_INIT(tl_cls_mask, cpu_cluster_flags, CLS),
#endif

#ifdef CONFIG_SCHED_MC
	SDTL_INIT(tl_mc_mask, cpu_core_flags, MC),
#endif
	SDTL_INIT(tl_pkg_mask, پوچ, PKG),
	{ پوچ, },
};

ایستا ساختار sched_domain_topology_level *sched_domain_topology =
	default_topology;
ایستا ساختار sched_domain_topology_level *sched_domain_topology_saved;

#define for_each_sd_topology(tl)			\
	برای (tl = sched_domain_topology; tl->mask; tl++)

تهی __init set_sched_topology(ساختار sched_domain_topology_level *tl)
{
	اگر (WARN_ON_ONCE(sched_smp_initialized))
		بازگشت;

	sched_domain_topology = tl;
	sched_domain_topology_saved = پوچ;
}

#ifdef CONFIG_NUMA
ایستا صحیح cpu_numa_flags(تهی)
{
	بازگشت SD_NUMA;
}

ایستا ثابت ساختار cpumask *sd_numa_mask(ساختار sched_domain_topology_level *tl, صحیح cpu)
{
	بازگشت sched_domains_numa_masks[tl->numa_level][cpu_to_گره(cpu)];
}

ایستا تهی sched_numa_warn(ثابت نویسه *str)
{
	ایستا صحیح done = false;
	صحیح i,j;

	اگر (done)
		بازگشت;

	done = true;

	printk(KERN_WARNING "ERROR: %s\n\n", str);

	برای (i = 0; i < nr_گره_ids; i++) {
		printk(KERN_WARNING "  ");
		برای (j = 0; j < nr_گره_ids; j++) {
			اگر (!گره_state(i, N_CPU) || !گره_state(j, N_CPU))
				printk(KERN_CONT "(%02d) ", گره_distance(i,j));
			وگرنه
				printk(KERN_CONT " %02d  ", گره_distance(i,j));
		}
		printk(KERN_CONT "\n");
	}
	printk(KERN_WARNING "\n");
}

bool find_numa_distance(صحیح distance)
{
	bool found = false;
	صحیح i, *distances;

	اگر (distance == گره_distance(0, 0))
		بازگشت true;

	rcu_read_lock();
	distances = rcu_dereference(sched_numa_گره_distance);
	اگر (!distances)
		برو unlock;
	برای (i = 0; i < sched_numa_گره_levels; i++) {
		اگر (distances[i] == distance) {
			found = true;
			بشکن;
		}
	}
unlock:
	rcu_read_unlock();

	بازگشت found;
}

#define for_each_cpu_گره_but(n, nbut)		\
	for_each_گره_state(n, N_CPU)		\
		اگر (n == nbut)			\
			ادامه;		\
		وگرنه

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
ایستا تهی init_numa_topology_type(صحیح offline_گره)
{
	صحیح a, b, c, n;

	n = sched_max_numa_distance;

	اگر (sched_domains_numa_levels <= 2) {
		sched_numa_topology_type = NUMA_DIRECT;
		بازگشت;
	}

	for_each_cpu_گره_but(a, offline_گره) {
		for_each_cpu_گره_but(b, offline_گره) {
			/* Find two nodes furthest removed from each other. */
			اگر (گره_distance(a, b) < n)
				ادامه;

			/* Is there an intermediary node between a and b? */
			for_each_cpu_گره_but(c, offline_گره) {
				اگر (گره_distance(a, c) < n &&
				    گره_distance(b, c) < n) {
					sched_numa_topology_type =
							NUMA_GLUELESS_MESH;
					بازگشت;
				}
			}

			sched_numa_topology_type = NUMA_BACKPLANE;
			بازگشت;
		}
	}

	pr_err("Failed to find a NUMA topology type, defaulting to DIRECT\n");
	sched_numa_topology_type = NUMA_DIRECT;
}


#define NR_DISTANCE_مقادیر (1 << DISTANCE_BITS)

/*
 * An architecture could modify its NUMA distance, to change
 * grouping of NUMA nodes and number of NUMA levels when creating
 * NUMA level sched domains.
 *
 * A NUMA level is created for each unique
 * arch_sched_node_distance.
 */
ایستا صحیح numa_گره_dist(صحیح i, صحیح j)
{
	بازگشت گره_distance(i, j);
}

صحیح arch_sched_گره_distance(صحیح from, صحیح to)
			     __weak __alias(numa_گره_dist);

ایستا bool modified_sched_گره_distance(تهی)
{
	بازگشت numa_گره_dist != arch_sched_گره_distance;
}

ایستا صحیح sched_record_numa_dist(صحیح offline_گره, صحیح (*n_dist)(صحیح, صحیح),
				  صحیح **dist, صحیح *levels)
{
	unsigned long *distance_map __free(bitmap) = پوچ;
	صحیح nr_levels = 0;
	صحیح i, j;
	صحیح *distances;

	/*
	 * O(nr_nodes^2) de-duplicating selection sort -- in order to find the
	 * unique distances in the node_distance() table.
	 */
	distance_map = bitmap_alloc(NR_DISTANCE_مقادیر, GFP_KERNEL);
	اگر (!distance_map)
		بازگشت -ENOMEM;

	bitmap_zero(distance_map, NR_DISTANCE_مقادیر);
	for_each_cpu_گره_but(i, offline_گره) {
		for_each_cpu_گره_but(j, offline_گره) {
			صحیح distance = n_dist(i, j);

			اگر (distance < LOCAL_DISTANCE || distance >= NR_DISTANCE_مقادیر) {
				sched_numa_warn("Invalid distance value range");
				بازگشت -EINVAL;
			}

			bitmap_set(distance_map, distance, 1);
		}
	}
	/*
	 * We can now figure out how many unique distance values there are and
	 * allocate memory accordingly.
	 */
	nr_levels = bitmap_weight(distance_map, NR_DISTANCE_مقادیر);

	distances = kzalloc_objs(صحیح, nr_levels);
	اگر (!distances)
		بازگشت -ENOMEM;

	برای (i = 0, j = 0; i < nr_levels; i++, j++) {
		j = find_next_bit(distance_map, NR_DISTANCE_مقادیر, j);
		distances[i] = j;
	}
	*dist = distances;
	*levels = nr_levels;

	بازگشت 0;
}

تهی sched_init_numa(صحیح offline_گره)
{
	ساختار sched_domain_topology_level *tl;
	صحیح nr_levels, nr_گره_levels;
	صحیح i, j;
	صحیح *distances, *domain_distances;
	ساختار cpumask ***masks;

	/* Record the NUMA distances from SLIT table */
	اگر (sched_record_numa_dist(offline_گره, numa_گره_dist, &distances,
				   &nr_گره_levels))
		بازگشت;

	/* Record modified NUMA distances for building sched domains */
	اگر (modified_sched_گره_distance()) {
		اگر (sched_record_numa_dist(offline_گره, arch_sched_گره_distance,
					   &domain_distances, &nr_levels)) {
			kfree(distances);
			بازگشت;
		}
	} وگرنه {
		domain_distances = distances;
		nr_levels = nr_گره_levels;
	}
	rcu_assign_pointer(sched_numa_گره_distance, distances);
	WRITE_ONCE(sched_max_numa_distance, distances[nr_گره_levels - 1]);
	WRITE_ONCE(sched_numa_گره_levels, nr_گره_levels);

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

	masks = kzalloc(اندازه(تهی *) * nr_levels, GFP_KERNEL);
	اگر (!masks)
		بازگشت;

	/*
	 * Now for each level, construct a mask per node which contains all
	 * CPUs of nodes that are that many hops away from us.
	 */
	برای (i = 0; i < nr_levels; i++) {
		masks[i] = kzalloc(nr_گره_ids * اندازه(تهی *), GFP_KERNEL);
		اگر (!masks[i])
			بازگشت;

		for_each_cpu_گره_but(j, offline_گره) {
			ساختار cpumask *mask = kzalloc(cpumask_size(), GFP_KERNEL);
			صحیح k;

			اگر (!mask)
				بازگشت;

			masks[i][j] = mask;

			for_each_cpu_گره_but(k, offline_گره) {
				اگر (sched_debug() &&
				    (arch_sched_گره_distance(j, k) !=
				     arch_sched_گره_distance(k, j)))
					sched_numa_warn("Node-distance not symmetric");

				اگر (arch_sched_گره_distance(j, k) >
				    sched_domains_numa_distance[i])
					ادامه;

				cpumask_or(mask, mask, cpumask_of_گره(k));
			}
		}
	}
	rcu_assign_pointer(sched_domains_numa_masks, masks);

	/* Compute default topology size */
	برای (i = 0; sched_domain_topology[i].mask; i++);

	tl = kzalloc((i + nr_levels + 1) *
			اندازه(ساختار sched_domain_topology_level), GFP_KERNEL);
	اگر (!tl)
		بازگشت;

	/*
	 * Copy the default topology bits..
	 */
	برای (i = 0; sched_domain_topology[i].mask; i++)
		tl[i] = sched_domain_topology[i];

	/*
	 * Add the NUMA identity distance, aka single NODE.
	 */
	tl[i++] = SDTL_INIT(sd_numa_mask, پوچ, گره_2);

	/*
	 * .. and append 'j' levels of NUMA goodness.
	 */
	برای (j = 1; j < nr_levels; i++, j++) {
		tl[i] = SDTL_INIT(sd_numa_mask, cpu_numa_flags, NUMA);
		tl[i].numa_level = j;
	}

	sched_domain_topology_saved = sched_domain_topology;
	sched_domain_topology = tl;

	sched_domains_numa_levels = nr_levels;

	init_numa_topology_type(offline_گره);
}


ایستا تهی sched_reset_numa(تهی)
{
	صحیح nr_levels, *distances, *dom_distances = پوچ;
	ساختار cpumask ***masks;

	nr_levels = sched_domains_numa_levels;
	sched_numa_گره_levels = 0;
	sched_domains_numa_levels = 0;
	sched_max_numa_distance = 0;
	sched_numa_topology_type = NUMA_DIRECT;
	distances = sched_numa_گره_distance;
	اگر (sched_numa_گره_distance != sched_domains_numa_distance)
		dom_distances = sched_domains_numa_distance;
	rcu_assign_pointer(sched_numa_گره_distance, پوچ);
	rcu_assign_pointer(sched_domains_numa_distance, پوچ);
	masks = sched_domains_numa_masks;
	rcu_assign_pointer(sched_domains_numa_masks, پوچ);
	اگر (distances || masks) {
		صحیح i, j;

		synchronize_rcu();
		kfree(distances);
		kfree(dom_distances);
		برای (i = 0; i < nr_levels && masks; i++) {
			اگر (!masks[i])
				ادامه;
			for_each_گره(j)
				kfree(masks[i][j]);
			kfree(masks[i]);
		}
		kfree(masks);
	}
	اگر (sched_domain_topology_saved) {
		kfree(sched_domain_topology);
		sched_domain_topology = sched_domain_topology_saved;
		sched_domain_topology_saved = پوچ;
	}
}

/*
 * Call with hotplug lock held
 */
تهی sched_update_numa(صحیح cpu, bool online)
{
	صحیح گره;

	گره = cpu_to_گره(cpu);
	/*
	 * Scheduler NUMA topology is updated when the first CPU of a
	 * node is onlined or the last CPU of a node is offlined.
	 */
	اگر (cpumask_weight(cpumask_of_گره(گره)) != 1)
		بازگشت;

	sched_reset_numa();
	sched_init_numa(online ? NUMA_NO_گره : گره);
}

تهی sched_domains_numa_masks_set(unsigned صحیح cpu)
{
	صحیح گره = cpu_to_گره(cpu);
	صحیح i, j;

	برای (i = 0; i < sched_domains_numa_levels; i++) {
		برای (j = 0; j < nr_گره_ids; j++) {
			اگر (!گره_state(j, N_CPU))
				ادامه;

			/* Set ourselves in the remote node's masks */
			اگر (arch_sched_گره_distance(j, گره) <=
			    sched_domains_numa_distance[i])
				cpumask_set_cpu(cpu, sched_domains_numa_masks[i][j]);
		}
	}
}

تهی sched_domains_numa_masks_clear(unsigned صحیح cpu)
{
	صحیح i, j;

	برای (i = 0; i < sched_domains_numa_levels; i++) {
		برای (j = 0; j < nr_گره_ids; j++) {
			اگر (sched_domains_numa_masks[i][j])
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
صحیح sched_numa_find_closest(ثابت ساختار cpumask *cpus, صحیح cpu)
{
	صحیح i, j = cpu_to_گره(cpu), found = nr_cpu_ids;
	ساختار cpumask ***masks;

	rcu_read_lock();
	masks = rcu_dereference(sched_domains_numa_masks);
	اگر (!masks)
		برو unlock;
	برای (i = 0; i < sched_domains_numa_levels; i++) {
		اگر (!masks[i][j])
			بشکن;
		cpu = cpumask_any_and_distribute(cpus, masks[i][j]);
		اگر (cpu < nr_cpu_ids) {
			found = cpu;
			بشکن;
		}
	}
unlock:
	rcu_read_unlock();

	بازگشت found;
}

ساختار __cmp_key {
	ثابت ساختار cpumask *cpus;
	ساختار cpumask ***masks;
	صحیح گره;
	صحیح cpu;
	صحیح w;
};

ایستا صحیح hop_cmp(ثابت تهی *a, ثابت تهی *b)
{
	ساختار cpumask **prev_hop, **cur_hop = *(ساختار cpumask ***)b;
	ساختار __cmp_key *k = (ساختار __cmp_key *)a;

	اگر (cpumask_weight_and(k->cpus, cur_hop[k->گره]) <= k->cpu)
		بازگشت 1;

	اگر (b == k->masks) {
		k->w = 0;
		بازگشت 0;
	}

	prev_hop = *((ساختار cpumask ***)b - 1);
	k->w = cpumask_weight_and(k->cpus, prev_hop[k->گره]);
	اگر (k->w <= k->cpu)
		بازگشت 0;

	بازگشت -1;
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
صحیح sched_numa_find_nth_cpu(ثابت ساختار cpumask *cpus, صحیح cpu, صحیح گره)
{
	ساختار __cmp_key k = { .cpus = cpus, .cpu = cpu };
	ساختار cpumask ***hop_masks;
	صحیح hop, ret = nr_cpu_ids;

	اگر (گره == NUMA_NO_گره)
		بازگشت cpumask_nth_and(cpu, cpus, cpu_online_mask);

	rcu_read_lock();

	/* CPU-less node entries are uninitialized in sched_domains_numa_masks */
	گره = numa_nearest_گره(گره, N_CPU);
	k.گره = گره;

	k.masks = rcu_dereference(sched_domains_numa_masks);
	اگر (!k.masks)
		برو unlock;

	hop_masks = bsearch(&k, k.masks, sched_domains_numa_levels, اندازه(k.masks[0]), hop_cmp);
	اگر (!hop_masks)
		برو unlock;
	hop = hop_masks	- k.masks;

	ret = hop ?
		cpumask_nth_and_andnot(cpu - k.w, cpus, k.masks[hop][گره], k.masks[hop-1][گره]) :
		cpumask_nth_and(cpu, cpus, k.masks[0][گره]);
unlock:
	rcu_read_unlock();
	بازگشت ret;
}
EXPORT_نماد_GPL(sched_numa_find_nth_cpu);

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
ثابت ساختار cpumask *sched_numa_hop_mask(unsigned صحیح گره, unsigned صحیح hops)
{
	ساختار cpumask ***masks;

	اگر (گره >= nr_گره_ids || hops >= sched_domains_numa_levels)
		بازگشت ERR_PTR(-EINVAL);

	masks = rcu_dereference(sched_domains_numa_masks);
	اگر (!masks)
		بازگشت ERR_PTR(-EBUSY);

	بازگشت masks[hops][گره];
}
EXPORT_نماد_GPL(sched_numa_hop_mask);

#endif /* CONFIG_NUMA */

ایستا صحیح __sdt_alloc(ثابت ساختار cpumask *cpu_map)
{
	ساختار sched_domain_topology_level *tl;
	صحیح j;

	for_each_sd_topology(tl) {
		ساختار sd_data *sdd = &tl->data;

		sdd->sd = alloc_percpu(ساختار sched_domain *);
		اگر (!sdd->sd)
			بازگشت -ENOMEM;

		sdd->sg = alloc_percpu(ساختار sched_group *);
		اگر (!sdd->sg)
			بازگشت -ENOMEM;

		sdd->sgc = alloc_percpu(ساختار sched_group_capacity *);
		اگر (!sdd->sgc)
			بازگشت -ENOMEM;

		for_each_cpu(j, cpu_map) {
			ساختار sched_domain *sd;
			ساختار sched_group *sg;
			ساختار sched_group_capacity *sgc;

			sd = kzalloc_گره(اندازه(ساختار sched_domain) + cpumask_size(),
					GFP_KERNEL, cpu_to_گره(j));
			اگر (!sd)
				بازگشت -ENOMEM;

			*per_cpu_ptr(sdd->sd, j) = sd;

			sg = kzalloc_گره(اندازه(ساختار sched_group) + cpumask_size(),
					GFP_KERNEL, cpu_to_گره(j));
			اگر (!sg)
				بازگشت -ENOMEM;

			sg->next = sg;

			*per_cpu_ptr(sdd->sg, j) = sg;

			sgc = kzalloc_گره(اندازه(ساختار sched_group_capacity) + cpumask_size(),
					GFP_KERNEL, cpu_to_گره(j));
			اگر (!sgc)
				بازگشت -ENOMEM;

			sgc->id = j;

			*per_cpu_ptr(sdd->sgc, j) = sgc;
		}
	}

	بازگشت 0;
}

ایستا تهی __sdt_free(ثابت ساختار cpumask *cpu_map)
{
	ساختار sched_domain_topology_level *tl;
	صحیح j;

	for_each_sd_topology(tl) {
		ساختار sd_data *sdd = &tl->data;

		for_each_cpu(j, cpu_map) {
			ساختار sched_domain *sd;

			اگر (sdd->sd) {
				sd = *per_cpu_ptr(sdd->sd, j);
				اگر (sd && (sd->flags & SD_NUMA))
					free_sched_groups(sd->groups, 0);
				kfree(*per_cpu_ptr(sdd->sd, j));
			}

			اگر (sdd->sg)
				kfree(*per_cpu_ptr(sdd->sg, j));
			اگر (sdd->sgc)
				kfree(*per_cpu_ptr(sdd->sgc, j));
		}
		free_percpu(sdd->sd);
		sdd->sd = پوچ;
		free_percpu(sdd->sg);
		sdd->sg = پوچ;
		free_percpu(sdd->sgc);
		sdd->sgc = پوچ;
	}
}

ایستا صحیح __sds_alloc(ساختار s_data *d, ثابت ساختار cpumask *cpu_map)
{
	صحیح j;

	d->sds = alloc_percpu(ساختار sched_domain_shared *);
	اگر (!d->sds)
		بازگشت -ENOMEM;

	for_each_cpu(j, cpu_map) {
		ساختار sched_domain_shared *sds;

		sds = kzalloc_گره(اندازه(ساختار sched_domain_shared),
				GFP_KERNEL, cpu_to_گره(j));
		اگر (!sds)
			بازگشت -ENOMEM;

		*per_cpu_ptr(d->sds, j) = sds;
	}

	بازگشت 0;
}

ایستا تهی __sds_free(ساختار s_data *d, ثابت ساختار cpumask *cpu_map)
{
	صحیح j;

	اگر (!d->sds)
		بازگشت;

	for_each_cpu(j, cpu_map)
		kfree(*per_cpu_ptr(d->sds, j));

	free_percpu(d->sds);
	d->sds = پوچ;
}

ایستا ساختار sched_domain *build_sched_domain(ساختار sched_domain_topology_level *tl,
		ثابت ساختار cpumask *cpu_map, ساختار sched_domain_attr *attr,
		ساختار sched_domain *child, صحیح cpu)
{
	ساختار sched_domain *sd = sd_init(tl, cpu_map, child, cpu);

	اگر (child) {
		sd->level = child->level + 1;
		sched_domain_level_max = max(sched_domain_level_max, sd->level);
		child->parent = sd;

		اگر (!cpumask_subset(sched_domain_span(child),
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

	بازگشت sd;
}

/*
 * Ensure topology masks are sane, i.e. there are no conflicts (overlaps) for
 * any two given CPUs on non-NUMA topology levels.
 */
ایستا bool topology_span_sane(ثابت ساختار cpumask *cpu_map)
{
	ساختار sched_domain_topology_level *tl;
	ساختار cpumask *covered, *id_seen;
	صحیح cpu;

	lockdep_assert_held(&sched_domains_mutex);
	covered = sched_domains_tmpmask;
	id_seen = sched_domains_tmpmask2;

	for_each_sd_topology(tl) {
		صحیح tl_common_flags = 0;

		اگر (tl->sd_flags)
			tl_common_flags = (*tl->sd_flags)();

		/* NUMA levels are allowed to overlap */
		اگر (tl_common_flags & SD_NUMA)
			ادامه;

		cpumask_clear(covered);
		cpumask_clear(id_seen);

		/*
		 * Non-NUMA levels cannot partially overlap - they must be either
		 * completely equal or completely disjoint. Otherwise we can end up
		 * breaking the sched_group lists - i.e. a later get_group() pass
		 * breaks the linking done for an earlier span.
		 */
		for_each_cpu(cpu, cpu_map) {
			ثابت ساختار cpumask *tl_cpu_mask = tl->mask(tl, cpu);
			صحیح id;

			/* lowest bit set in this mask is used as a unique id */
			id = cpumask_first(tl_cpu_mask);

			اگر (cpumask_test_cpu(id, id_seen)) {
				/* First CPU has already been seen, ensure identical spans */
				اگر (!cpumask_equal(tl->mask(tl, id), tl_cpu_mask))
					بازگشت false;
			} وگرنه {
				/* First CPU hasn't been seen before, ensure it's a completely new span */
				اگر (cpumask_intersects(tl_cpu_mask, covered))
					بازگشت false;

				cpumask_or(covered, covered, tl_cpu_mask);
				cpumask_set_cpu(id, id_seen);
			}
		}
	}
	بازگشت true;
}

/*
 * Calculate an allowed NUMA imbalance such that LLCs do not get
 * imbalanced.
 */
ایستا تهی adjust_numa_imbalance(ساختار sched_domain *sd_llc)
{
	ساختار sched_domain *parent;
	unsigned صحیح imb_span = 1;
	unsigned صحیح imb = 0;
	unsigned صحیح nr_llcs;

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
	اگر (nr_llcs == 1)
		imb = sd_llc->parent->span_weight >> 3;
	وگرنه
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
	وقتی (parent && !(parent->flags & SD_NUMA))
		parent = parent->parent;

	imb_span = parent ? parent->span_weight : sd_llc->parent->span_weight;

	/* Update the upper remainder of the topology */
	parent = sd_llc->parent;
	وقتی (parent) {
		صحیح factor = max(1U, (parent->span_weight / imb_span));

		parent->imb_numa_nr = imb * factor;
		parent = parent->parent;
	}
}

ایستا تهی
init_sched_domain_shared(ساختار s_data *d, ساختار sched_domain *sd, صحیح flags)
{
	ساختار sched_domain_shared *sds = پوچ;
	صحیح cpu;

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

		اگر (!sds->alloc_flags ||
		    sd->span_weight == 1 ||
		    sds->alloc_flags == flags) {
			sds->alloc_flags = flags;
			sd->shared = sds;
			بشکن;
		}
	}

	/*
	 * Use the sd_shared corresponding to the last
	 * CPU in the span if none are avaialable.
	 */
	اگر (WARN_ON_ONCE(!sd->shared))
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
ایستا bool claim_asym_sched_domain_shared(ساختار s_data *d, صحیح cpu)
{
	ساختار sched_domain *sd = *per_cpu_ptr(d->sd, cpu);
	ساختار sched_domain *sd_asym;

	اگر (!sd)
		بازگشت false;

	sd_asym = sd;
	وقتی (sd_asym && !(sd_asym->flags & SD_ASYM_CPUCAPACITY_FULL))
		sd_asym = sd_asym->parent;

	اگر (!sd_asym || (sd_asym->flags & SD_NUMA))
		بازگشت false;

	init_sched_domain_shared(d, sd_asym, SD_ASYM_CPUCAPACITY);
	بازگشت true;
}

ایستا صحیح __sched_domains_alloc_llc_id(تهی)
{
	صحیح lid, max;

	lockdep_assert_held(&sched_domains_mutex);

	lid = cpumask_first_zero(sched_domains_llc_id_allocmask);
	/*
	 * llc_id space should never grow larger than the
	 * possible number of CPUs in the system.
	 */
	اگر (lid >= nr_cpu_ids)
		بازگشت -1;

	__cpumask_set_cpu(lid, sched_domains_llc_id_allocmask);
	max = cpumask_last(sched_domains_llc_id_allocmask);
	اگر (max > max_lid)
		max_lid = max;

	بازگشت lid;
}

ایستا تهی __sched_domains_free_llc_id(صحیح cpu)
{
	صحیح i, lid, max;

	lockdep_assert_held(&sched_domains_mutex);

	lid = per_cpu(sd_llc_id, cpu);
	اگر (lid == -1 || lid >= nr_cpu_ids)
		بازگشت;

	per_cpu(sd_llc_id, cpu) = -1;

	for_each_cpu(i, llc_mask(cpu)) {
		/* An online CPU owns the llc_id. */
		اگر (per_cpu(sd_llc_id, i) == lid)
			بازگشت;
	}

	__cpumask_clear_cpu(lid, sched_domains_llc_id_allocmask);

	max = cpumask_last(sched_domains_llc_id_allocmask);
	/* shrink max lid to save memory */
	اگر (max < max_lid)
		max_lid = max;
}

تهی sched_domains_free_llc_id(صحیح cpu)
{
	sched_domains_mutex_lock();
	__sched_domains_free_llc_id(cpu);
	sched_domains_mutex_unlock();
}

/*
 * Build sched domains for a given set of CPUs and attach the sched domains
 * to the individual CPUs
 */
ایستا صحیح
build_sched_domains(ثابت ساختار cpumask *cpu_map, ساختار sched_domain_attr *attr,
		    bool *multi_llcs)
{
	شمارش s_alloc alloc_state = sa_none;
	bool has_multi_llcs = false;
	ساختار sched_domain *sd;
	ساختار s_data d;
	ساختار rq *rq = پوچ;
	صحیح i, ret = -ENOMEM;
	bool has_asym = false;
	bool has_cluster = false;

	اگر (WARN_ON(cpumask_empty(cpu_map)))
		برو error;

	alloc_state = __visit_domain_allocation_hell(&d, cpu_map);
	اگر (alloc_state != sa_rootdomain)
		برو error;

	/* Set up domains for CPUs specified by the cpu_map: */
	for_each_cpu(i, cpu_map) {
		ساختار sched_domain_topology_level *tl;
		صحیح lid;

		sd = پوچ;
		for_each_sd_topology(tl) {

			sd = build_sched_domain(tl, cpu_map, attr, sd, i);

			has_asym |= sd->flags & SD_ASYM_CPUCAPACITY;

			اگر (tl == sched_domain_topology)
				*per_cpu_ptr(d.sd, i) = sd;
			اگر (cpumask_equal(cpu_map, sched_domain_span(sd)))
				بشکن;
		}

		lid = per_cpu(sd_llc_id, i);
		اگر (lid == -1) {
			/* try to reuse the llc_id of its siblings */
			برای (صحیح j = cpumask_first(llc_mask(i));
			     j < nr_cpu_ids;
			     j = cpumask_next(j, llc_mask(i))) {
				اگر (i == j)
					ادامه;

				lid = per_cpu(sd_llc_id, j);

				اگر (lid != -1) {
					per_cpu(sd_llc_id, i) = lid;

					بشکن;
				}
			}

			/* a new LLC is detected */
			اگر (lid == -1)
				per_cpu(sd_llc_id, i) = __sched_domains_alloc_llc_id();
		}
	}

	اگر (WARN_ON(!topology_span_sane(cpu_map)))
		برو error;

	/* Build the groups for the domains */
	for_each_cpu(i, cpu_map) {
		برای (sd = *per_cpu_ptr(d.sd, i); sd; sd = sd->parent) {
			sd->span_weight = cpumask_weight(sched_domain_span(sd));
			اگر (sd->flags & SD_NUMA) {
				اگر (build_overlap_sched_groups(sd, i))
					برو error;
			} وگرنه {
				اگر (build_sched_groups(sd, i))
					برو error;
			}
		}
	}

	for_each_cpu(i, cpu_map) {
		sd = *per_cpu_ptr(d.sd, i);
		اگر (!sd)
			ادامه;

		اگر (has_asym)
			claim_asym_sched_domain_shared(&d, i);

		/* First, find the topmost SD_SHARE_LLC domain */
		وقتی (sd->parent && (sd->parent->flags & SD_SHARE_LLC))
			sd = sd->parent;

		اگر (sd->flags & SD_SHARE_LLC) {
			init_sched_domain_shared(&d, sd, SD_SHARE_LLC);

			/*
			 * In presence of higher domains, adjust the
			 * NUMA imbalance stats for the hierarchy.
			 */
			اگر (sd->parent) {
				اگر (IS_ENABLED(CONFIG_NUMA))
					adjust_numa_imbalance(sd);

				اگر (sd_in_multi_llcs(sd))
					has_multi_llcs = true;
			}
		}
	}

	/* Calculate CPU capacity for physical packages and nodes */
	برای (i = nr_cpumask_bits-1; i >= 0; i--) {
		اگر (!cpumask_test_cpu(i, cpu_map))
			ادامه;

		claim_allocations(i, &d);

		برای (sd = *per_cpu_ptr(d.sd, i); sd; sd = sd->parent)
			init_sched_groups_capacity(i, sd);
	}

	alloc_sd_llc(cpu_map, &d);

	/* Attach the domains */
	rcu_read_lock();
	for_each_cpu(i, cpu_map) {
		rq = cpu_rq(i);
		sd = *per_cpu_ptr(d.sd, i);

		cpu_attach_domain(sd, d.rd, i);

		اگر (lowest_flag_domain(i, SD_CLUSTER))
			has_cluster = true;
	}
	rcu_read_unlock();

	اگر (has_asym)
		static_branch_inc_cpuslocked(&sched_asym_cpucapacity);

	اگر (has_cluster)
		static_branch_inc_cpuslocked(&sched_cluster_active);

	اگر (rq && sched_debug_verbose)
		pr_info("root domain span: %*pbl\n", cpumask_pr_args(cpu_map));

	ret = 0;
error:
	*multi_llcs = has_multi_llcs;
	__free_domain_allocs(&d, alloc_state, cpu_map);

	بازگشت ret;
}

/* Current sched domains: */
ایستا cpumask_var_t			*doms_cur;

/* Number of sched domains in 'doms_cur': */
ایستا صحیح				ndoms_cur;

/* Attributes of custom domains in 'doms_cur' */
ایستا ساختار sched_domain_attr		*dattr_cur;

/*
 * Special case: If a kmalloc() of a doms_cur partition (array of
 * cpumask) fails, then fallback to a single sched domain,
 * as determined by the single cpumask fallback_doms.
 */
ایستا cpumask_var_t			fallback_doms;

/*
 * arch_update_cpu_topology lets virtualized architectures update the
 * CPU core maps. It is supposed to return 1 if the topology changed
 * or 0 if it stayed the same.
 */
صحیح __weak arch_update_cpu_topology(تهی)
{
	بازگشت 0;
}

cpumask_var_t *alloc_sched_domains(unsigned صحیح ndoms)
{
	صحیح i;
	cpumask_var_t *doms;

	doms = kmalloc_objs(*doms, ndoms);
	اگر (!doms)
		بازگشت پوچ;
	برای (i = 0; i < ndoms; i++) {
		اگر (!alloc_cpumask_var(&doms[i], GFP_KERNEL)) {
			free_sched_domains(doms, i);
			بازگشت پوچ;
		}
	}
	بازگشت doms;
}

تهی free_sched_domains(cpumask_var_t doms[], unsigned صحیح ndoms)
{
	unsigned صحیح i;
	برای (i = 0; i < ndoms; i++)
		free_cpumask_var(doms[i]);
	kfree(doms);
}

/*
 * Set up scheduler domains and groups.  For now this just excludes isolated
 * CPUs, but could be used to exclude other special cases in the future.
 */
صحیح __init sched_init_domains(ثابت ساختار cpumask *cpu_map)
{
	bool multi_llcs;
	صحیح err;

	zalloc_cpumask_var(&sched_domains_llc_id_allocmask, GFP_KERNEL);
	zalloc_cpumask_var(&sched_domains_tmpmask, GFP_KERNEL);
	zalloc_cpumask_var(&sched_domains_tmpmask2, GFP_KERNEL);
	zalloc_cpumask_var(&fallback_doms, GFP_KERNEL);

	arch_update_cpu_topology();
	asym_cpu_capacity_scan();
	ndoms_cur = 1;
	doms_cur = alloc_sched_domains(ndoms_cur);
	اگر (!doms_cur)
		doms_cur = &fallback_doms;
	cpumask_and(doms_cur[0], cpu_map, housekeeping_cpumask(HK_TYPE_DOMAIN));
	err = build_sched_domains(doms_cur[0], پوچ, &multi_llcs);
	اگر (!err)
		sched_cache_set(multi_llcs);

	بازگشت err;
}

/*
 * Detach sched domains from a group of CPUs specified in cpu_map
 * These CPUs will now be attached to the NULL domain
 */
ایستا تهی detach_destroy_domains(ثابت ساختار cpumask *cpu_map)
{
	unsigned صحیح cpu = cpumask_any(cpu_map);
	صحیح i;

	اگر (rcu_access_pointer(per_cpu(sd_asym_cpucapacity, cpu)))
		static_branch_dec_cpuslocked(&sched_asym_cpucapacity);

	اگر (static_branch_unlikely(&sched_cluster_active))
		static_branch_dec_cpuslocked(&sched_cluster_active);

	rcu_read_lock();
	for_each_cpu(i, cpu_map)
		cpu_attach_domain(پوچ, &def_ریشه_domain, i);
	rcu_read_unlock();
}

/* handle null as "default" */
ایستا صحیح dattrs_equal(ساختار sched_domain_attr *cur, صحیح idx_cur,
			ساختار sched_domain_attr *new, صحیح idx_new)
{
	ساختار sched_domain_attr tmp;

	/* Fast path: */
	اگر (!new && !cur)
		بازگشت 1;

	tmp = SD_ATTR_INIT;

	بازگشت !memcmp(cur ? (cur + idx_cur) : &tmp,
			new ? (new + idx_new) : &tmp,
			اندازه(ساختار sched_domain_attr));
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
ایستا تهی partition_sched_domains_locked(صحیح ndoms_new, cpumask_var_t doms_new[],
				    ساختار sched_domain_attr *dattr_new)
{
	bool __maybe_unused has_eas = false;
	bool has_multi_llcs = false, multi_llcs;
	صحیح i, j, n;
	صحیح new_topology;

	lockdep_assert_held(&sched_domains_mutex);

	/* Let the architecture update CPU core mappings: */
	new_topology = arch_update_cpu_topology();
	/* Trigger rebuilding CPU capacity asymmetry data */
	اگر (new_topology)
		asym_cpu_capacity_scan();

	اگر (!doms_new) {
		WARN_ON_ONCE(dattr_new);
		n = 0;
		doms_new = alloc_sched_domains(1);
		اگر (doms_new) {
			n = 1;
			cpumask_and(doms_new[0], cpu_active_mask,
				    housekeeping_cpumask(HK_TYPE_DOMAIN));
		}
	} وگرنه {
		n = ndoms_new;
	}

	/* Destroy deleted domains: */
	برای (i = 0; i < ndoms_cur; i++) {
		برای (j = 0; j < n && !new_topology; j++) {
			اگر (cpumask_equal(doms_cur[i], doms_new[j]) &&
			    dattrs_equal(dattr_cur, i, dattr_new, j))
				برو match1;
		}
		/* No match - a current sched domain not in new doms_new[] */
		detach_destroy_domains(doms_cur[i]);
match1:
		;
	}

	n = ndoms_cur;
	اگر (!doms_new) {
		n = 0;
		doms_new = &fallback_doms;
		cpumask_and(doms_new[0], cpu_active_mask,
			    housekeeping_cpumask(HK_TYPE_DOMAIN));
	}

	/* Build new domains: */
	برای (i = 0; i < ndoms_new; i++) {
		برای (j = 0; j < n && !new_topology; j++) {
			اگر (cpumask_equal(doms_new[i], doms_cur[j]) &&
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
				ساختار sched_domain *sd;
				صحیح cpu = cpumask_first(doms_cur[j]);

				guard(rcu)();
				sd = rcu_dereference(cpu_rq(cpu)->sd);
				وقتی (sd && sd->parent && (sd->parent->flags & SD_SHARE_LLC))
					sd = sd->parent;
				اگر (sd && (sd->flags & SD_SHARE_LLC) && sd->parent &&
				    sd_in_multi_llcs(sd))
					has_multi_llcs = true;
				برو match2;
			}
		}
		/* No match - add a new doms_new */
		build_sched_domains(doms_new[i], dattr_new ? dattr_new + i : پوچ,
				    &multi_llcs);
		has_multi_llcs |= multi_llcs;
match2:
		;
	}
	sched_cache_set(has_multi_llcs);

#اگر defined(CONFIG_ENERGY_MODEL) && defined(CONFIG_CPU_FREQ_GOV_SCHEDUTIL)
	/* Build perf domains: */
	برای (i = 0; i < ndoms_new; i++) {
		برای (j = 0; j < n && !sched_energy_update; j++) {
			اگر (cpumask_equal(doms_new[i], doms_cur[j]) &&
			    cpu_rq(cpumask_first(doms_cur[j]))->rd->pd) {
				has_eas = true;
				برو match3;
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
	اگر (doms_cur != &fallback_doms)
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
تهی partition_sched_domains(صحیح ndoms_new, cpumask_var_t doms_new[],
			     ساختار sched_domain_attr *dattr_new)
{
	sched_domains_mutex_lock();
	partition_sched_domains_locked(ndoms_new, doms_new, dattr_new);
	sched_domains_mutex_unlock();
}
