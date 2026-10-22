// SPDX-License-Identifier: GPL-2.0
/*
 * Scheduler topology setup/handling methods
 */

#அந்தர்பாவ <linux/sched/isolation.h>
#அந்தர்பாவ <linux/sched/clock.h>
#அந்தர்பாவ <linux/bsearch.h>
#அந்தர்பாவ "sched.h"

DEFINE_MUTEX(sched_domains_mutex);
வெறுமை sched_domains_mutex_lock(வெறுமை)
{
	mutex_lock(&sched_domains_mutex);
}
வெறுமை sched_domains_mutex_unlock(வெறுமை)
{
	mutex_unlock(&sched_domains_mutex);
}

/* Protected by sched_domains_mutex: */
நிலையான cpumask_var_t sched_domains_llc_id_allocmask;
நிலையான cpumask_var_t sched_domains_tmpmask;
நிலையான cpumask_var_t sched_domains_tmpmask2;
முழு max_lid;

நிலையான முழு __init sched_debug_setup(எழுத்து *str)
{
	sched_debug_verbose = true;

	திரும்பு 0;
}
early_param("sched_verbose", sched_debug_setup);

#ifdef CONFIG_SCHED_SMT
நிலையான bool sched_smt_asym_packing __read_mostly;

நிலையான முழு __init setup_sched_smt_asym_packing(எழுத்து *str)
{
	எனில் (strcmp(str, "on"))
		திரும்பு 0;

	sched_smt_asym_packing = true;
	திரும்பு 1;
}
__setup("sched_smt_asym_packing=", setup_sched_smt_asym_packing);
#endif

நிலையான உள்வரி bool sched_debug(வெறுமை)
{
	திரும்பு sched_debug_verbose;
}

#define SD_FLAG(_name, mflags) [__##_name] = { .meta_flags = mflags, .name = #_name },
மாறிலி கட்டமைப்பு sd_flag_debug sd_flag_debug[] = {
#அந்தர்பாவ <linux/sched/sd_flags.h>
};
#undef SD_FLAG

நிலையான முழு sched_domain_debug_one(கட்டமைப்பு sched_domain *sd, முழு cpu, முழு level,
				  கட்டமைப்பு cpumask *groupmask)
{
	கட்டமைப்பு sched_group *group = sd->groups;
	unsigned long flags = sd->flags;
	unsigned முழு idx;

	cpumask_clear(groupmask);

	printk(KERN_DEBUG "%*s domain-%d: ", level, "", level);
	printk(KERN_CONT "span=%*pbl level=%s\n",
	       cpumask_pr_args(sched_domain_span(sd)), sd->name);

	எனில் (!cpumask_test_cpu(cpu, sched_domain_span(sd))) {
		printk(KERN_ERR "ERROR: domain->span does not contain CPU%d\n", cpu);
	}
	எனில் (group && !cpumask_test_cpu(cpu, sched_group_span(group))) {
		printk(KERN_ERR "ERROR: domain->groups does not contain CPU%d\n", cpu);
	}

	for_each_set_bit(idx, &flags, __SD_FLAG_CNT) {
		unsigned முழு flag = BIT(idx);
		unsigned முழு meta_flags = sd_flag_debug[idx].meta_flags;

		எனில் ((meta_flags & SDF_SHARED_CHILD) && sd->child &&
		    !(sd->child->flags & flag))
			printk(KERN_ERR "ERROR: flag %s set here but not in child\n",
			       sd_flag_debug[idx].name);

		எனில் ((meta_flags & SDF_SHARED_PARENT) && sd->parent &&
		    !(sd->parent->flags & flag))
			printk(KERN_ERR "ERROR: flag %s set here but not in parent\n",
			       sd_flag_debug[idx].name);
	}

	printk(KERN_DEBUG "%*s groups:", level + 1, "");
	செய் {
		எனில் (!group) {
			printk("\n");
			printk(KERN_ERR "ERROR: group is NULL\n");
			நிறுத்து;
		}

		எனில் (cpumask_empty(sched_group_span(group))) {
			printk(KERN_CONT "\n");
			printk(KERN_ERR "ERROR: empty group\n");
			நிறுத்து;
		}

		எனில் (!(sd->flags & SD_NUMA) &&
		    cpumask_intersects(groupmask, sched_group_span(group))) {
			printk(KERN_CONT "\n");
			printk(KERN_ERR "ERROR: repeated CPUs\n");
			நிறுத்து;
		}

		cpumask_or(groupmask, groupmask, sched_group_span(group));

		printk(KERN_CONT " %d:{ span=%*pbl",
				group->sgc->id,
				cpumask_pr_args(sched_group_span(group)));

		எனில் ((sd->flags & SD_NUMA) &&
		    !cpumask_equal(group_balance_mask(group), sched_group_span(group))) {
			printk(KERN_CONT " mask=%*pbl",
				cpumask_pr_args(group_balance_mask(group)));
		}

		எனில் (group->sgc->capacity != SCHED_CAPACITY_SCALE)
			printk(KERN_CONT " cap=%lu", group->sgc->capacity);

		எனில் (group == sd->groups && sd->child &&
		    !cpumask_equal(sched_domain_span(sd->child),
				   sched_group_span(group))) {
			printk(KERN_ERR "ERROR: domain->groups does not match domain->child\n");
		}

		printk(KERN_CONT " }");

		group = group->next;

		எனில் (group != sd->groups)
			printk(KERN_CONT ",");

	} போது (group != sd->groups);
	printk(KERN_CONT "\n");

	எனில் (!cpumask_equal(sched_domain_span(sd), groupmask))
		printk(KERN_ERR "ERROR: groups don't span domain->span\n");

	எனில் (sd->parent &&
	    !cpumask_subset(groupmask, sched_domain_span(sd->parent)))
		printk(KERN_ERR "ERROR: parent span is not a superset of domain->span\n");
	திரும்பு 0;
}

நிலையான வெறுமை sched_domain_debug(கட்டமைப்பு sched_domain *sd, முழு cpu)
{
	முழு level = 0;

	எனில் (!sched_debug_verbose)
		திரும்பு;

	எனில் (!sd) {
		printk(KERN_DEBUG "CPU%d attaching NULL sched-domain.\n", cpu);
		திரும்பு;
	}

	printk(KERN_DEBUG "CPU%d attaching sched-domain(s):\n", cpu);

	க்கு (;;) {
		எனில் (sched_domain_debug_one(sd, cpu, level, sched_domains_tmpmask))
			நிறுத்து;
		level++;
		sd = sd->parent;
		எனில் (!sd)
			நிறுத்து;
	}
}

/* Generate a mask of SD flags with the SDF_NEEDS_GROUPS metaflag */
#define SD_FLAG(name, mflags) (name * !!((mflags) & SDF_NEEDS_GROUPS)) |
நிலையான மாறிலி unsigned முழு SD_DEGENERATE_GROUPS_MASK =
#அந்தர்பாவ <linux/sched/sd_flags.h>
0;
#undef SD_FLAG

நிலையான முழு sd_degenerate(கட்டமைப்பு sched_domain *sd)
{
	எனில் (cpumask_weight(sched_domain_span(sd)) == 1)
		திரும்பு 1;

	/* Following flags need at least 2 groups */
	எனில் ((sd->flags & SD_DEGENERATE_GROUPS_MASK) &&
	    (sd->groups != sd->groups->next))
		திரும்பு 0;

	/* Following flags don't use groups */
	எனில் (sd->flags & (SD_WAKE_AFFINE))
		திரும்பு 0;

	திரும்பு 1;
}

நிலையான முழு
sd_parent_degenerate(கட்டமைப்பு sched_domain *sd, கட்டமைப்பு sched_domain *parent)
{
	unsigned long cflags = sd->flags, pflags = parent->flags;

	எனில் (sd_degenerate(parent))
		திரும்பு 1;

	எனில் (!cpumask_equal(sched_domain_span(sd), sched_domain_span(parent)))
		திரும்பு 0;

	/* Flags needing groups don't count if only 1 group in parent */
	எனில் (parent->groups == parent->groups->next)
		pflags &= ~SD_DEGENERATE_GROUPS_MASK;

	எனில் (~cflags & pflags)
		திரும்பு 0;

	திரும்பு 1;
}

#எனில் defined(CONFIG_ENERGY_MODEL) && defined(CONFIG_CPU_FREQ_GOV_SCHEDUTIL)
DEFINE_STATIC_சாவி_FALSE(sched_energy_present);
நிலையான unsigned முழு sysctl_sched_energy_aware = 1;
நிலையான DEFINE_MUTEX(sched_energy_mutex);
நிலையான bool sched_energy_update;

நிலையான bool sched_is_eas_possible(மாறிலி கட்டமைப்பு cpumask *cpu_mask)
{
	bool any_asym_capacity = false;
	முழு i;

	/* EAS is enabled for asymmetric CPU capacity topologies. */
	for_each_cpu(i, cpu_mask) {
		எனில் (rcu_access_pointer(per_cpu(sd_asym_cpucapacity, i))) {
			any_asym_capacity = true;
			நிறுத்து;
		}
	}
	எனில் (!any_asym_capacity) {
		எனில் (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS, CPUs do not have asymmetric capacities\n",
				cpumask_pr_args(cpu_mask));
		}
		திரும்பு false;
	}

	/* EAS definitely does *not* handle SMT */
	எனில் (sched_smt_active()) {
		எனில் (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS, SMT is not supported\n",
				cpumask_pr_args(cpu_mask));
		}
		திரும்பு false;
	}

	எனில் (!arch_scale_freq_invariant()) {
		எனில் (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS: frequency-invariant load tracking not yet supported",
				cpumask_pr_args(cpu_mask));
		}
		திரும்பு false;
	}

	எனில் (!cpufreq_ready_for_eas(cpu_mask)) {
		எனில் (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS: cpufreq is not ready\n",
				cpumask_pr_args(cpu_mask));
		}
		திரும்பு false;
	}

	திரும்பு true;
}

வெறுமை rebuild_sched_domains_energy(வெறுமை)
{
	mutex_lock(&sched_energy_mutex);
	sched_energy_update = true;
	rebuild_sched_domains();
	sched_energy_update = false;
	mutex_unlock(&sched_energy_mutex);
}

#ifdef CONFIG_SYSCTL
நிலையான முழு sched_energy_aware_handler(மாறிலி கட்டமைப்பு ctl_table *table, முழு லிக,
		வெறுமை *buffer, size_t *lenp, loff_t *ppos)
{
	முழு ret;

	எனில் (லிக && !capable(CAP_SYS_ADMIN))
		திரும்பு -EPERM;

	எனில் (!sched_is_eas_possible(cpu_active_mask)) {
		எனில் (லிக) {
			திரும்பு -EOPNOTSUPP;
		} இல்லையெனில் {
			*lenp = 0;
			திரும்பு 0;
		}
	}

	ret = proc_dointvec_minmax(table, லிக, buffer, lenp, ppos);
	எனில் (!ret && லிக) {
		எனில் (sysctl_sched_energy_aware != sched_energy_enabled())
			rebuild_sched_domains_energy();
	}

	திரும்பு ret;
}

நிலையான மாறிலி கட்டமைப்பு ctl_table sched_energy_aware_sysctls[] = {
	{
		.procname       = "sched_energy_aware",
		.data           = &sysctl_sched_energy_aware,
		.maxlen         = அளவு(unsigned முழு),
		.mode           = 0644,
		.proc_handler   = sched_energy_aware_handler,
		.extra1         = SYSCTL_ZERO,
		.extra2         = SYSCTL_ONE,
	},
};

நிலையான முழு __init sched_energy_aware_sysctl_init(வெறுமை)
{
	register_sysctl_init("kernel", sched_energy_aware_sysctls);
	திரும்பு 0;
}

late_initcall(sched_energy_aware_sysctl_init);
#endif /* CONFIG_SYSCTL */

நிலையான வெறுமை free_pd(கட்டமைப்பு perf_domain *pd)
{
	கட்டமைப்பு perf_domain *tmp;

	போது (pd) {
		tmp = pd->next;
		kfree(pd);
		pd = tmp;
	}
}

நிலையான கட்டமைப்பு perf_domain *find_pd(கட்டமைப்பு perf_domain *pd, முழு cpu)
{
	போது (pd) {
		எனில் (cpumask_test_cpu(cpu, perf_domain_span(pd)))
			திரும்பு pd;
		pd = pd->next;
	}

	திரும்பு வெற்று;
}

நிலையான கட்டமைப்பு perf_domain *pd_init(முழு cpu)
{
	கட்டமைப்பு em_perf_domain *obj = em_cpu_get(cpu);
	கட்டமைப்பு perf_domain *pd;

	எனில் (!obj) {
		எனில் (sched_debug())
			pr_info("%s: no EM found for CPU%d\n", __func__, cpu);
		திரும்பு வெற்று;
	}

	pd = kzalloc_obj(*pd);
	எனில் (!pd)
		திரும்பு வெற்று;
	pd->em_pd = obj;

	திரும்பு pd;
}

நிலையான வெறுமை perf_domain_debug(மாறிலி கட்டமைப்பு cpumask *cpu_map,
						கட்டமைப்பு perf_domain *pd)
{
	எனில் (!sched_debug() || !pd)
		திரும்பு;

	printk(KERN_DEBUG "root_domain %*pbl:", cpumask_pr_args(cpu_map));

	போது (pd) {
		printk(KERN_CONT " pd%d:{ cpus=%*pbl nr_pstate=%d }",
				cpumask_first(perf_domain_span(pd)),
				cpumask_pr_args(perf_domain_span(pd)),
				em_pd_nr_perf_states(pd->em_pd));
		pd = pd->next;
	}

	printk(KERN_CONT "\n");
}

நிலையான வெறுமை destroy_perf_domain_rcu(கட்டமைப்பு rcu_head *rp)
{
	கட்டமைப்பு perf_domain *pd;

	pd = container_of(rp, கட்டமைப்பு perf_domain, rcu);
	free_pd(pd);
}

நிலையான வெறுமை sched_energy_set(bool has_eas)
{
	எனில் (!has_eas && sched_energy_enabled()) {
		எனில் (sched_debug())
			pr_info("%s: stopping EAS\n", __func__);
		static_branch_disable_cpuslocked(&sched_energy_present);
	} இல்லையெனில் எனில் (has_eas && !sched_energy_enabled()) {
		எனில் (sched_debug())
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
நிலையான bool build_perf_domains(மாறிலி கட்டமைப்பு cpumask *cpu_map)
{
	முழு i;
	கட்டமைப்பு perf_domain *pd = வெற்று, *tmp;
	முழு cpu = cpumask_first(cpu_map);
	கட்டமைப்பு வேர்_domain *rd = cpu_rq(cpu)->rd;

	எனில் (!sysctl_sched_energy_aware)
		செல் விடுவி;

	எனில் (!sched_is_eas_possible(cpu_map))
		செல் விடுவி;

	for_each_cpu(i, cpu_map) {
		/* Skip already covered CPUs. */
		எனில் (find_pd(pd, i))
			தொடர்;

		/* Create the new pd and add it to the local list. */
		tmp = pd_init(i);
		எனில் (!tmp)
			செல் விடுவி;
		tmp->next = pd;
		pd = tmp;
	}

	perf_domain_debug(cpu_map, pd);

	/* Attach the new list of performance domains to the root domain. */
	tmp = rd->pd;
	rcu_assign_pointer(rd->pd, pd);
	எனில் (tmp)
		call_rcu(&tmp->rcu, destroy_perf_domain_rcu);

	திரும்பு !!pd;

விடுவி:
	free_pd(pd);
	tmp = rd->pd;
	rcu_assign_pointer(rd->pd, வெற்று);
	எனில் (tmp)
		call_rcu(&tmp->rcu, destroy_perf_domain_rcu);

	திரும்பு false;
}
#இல்லையெனில் /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL): */
நிலையான வெறுமை free_pd(கட்டமைப்பு perf_domain *pd) { }
#endif /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL) */

நிலையான வெறுமை free_rootdomain(கட்டமைப்பு rcu_head *rcu)
{
	கட்டமைப்பு வேர்_domain *rd = container_of(rcu, கட்டமைப்பு வேர்_domain, rcu);

	cpupri_cleanup(&rd->cpupri);
	cpudl_cleanup(&rd->cpudl);
	free_cpumask_var(rd->dlo_mask);
	free_cpumask_var(rd->rto_mask);
	free_cpumask_var(rd->online);
	free_cpumask_var(rd->span);
	free_pd(rd->pd);
	kfree(rd);
}

வெறுமை rq_attach_வேர்(கட்டமைப்பு rq *rq, கட்டமைப்பு வேர்_domain *rd)
{
	கட்டமைப்பு வேர்_domain *old_rd = வெற்று;
	கட்டமைப்பு rq_flags rf;

	rq_lock_irqsave(rq, &rf);

	எனில் (rq->rd) {
		old_rd = rq->rd;

		எனில் (cpumask_test_cpu(rq->cpu, old_rd->online))
			set_rq_offline(rq);

		cpumask_clear_cpu(rq->cpu, old_rd->span);

		/*
		 * If we don't want to free the old_rd yet then
		 * set old_rd to NULL to skip the freeing later
		 * in this function:
		 */
		எனில் (!atomic_dec_and_test(&old_rd->refcount))
			old_rd = வெற்று;
	}

	atomic_inc(&rd->refcount);
	rq->rd = rd;

	cpumask_set_cpu(rq->cpu, rd->span);
	எனில் (cpumask_test_cpu(rq->cpu, cpu_active_mask))
		set_rq_online(rq);

	/*
	 * Because the rq is not a task, dl_add_task_root_domain() did not
	 * move the fair server bw to the rd if it already started.
	 * Add it now.
	 */
	எனில் (rq->fair_server.dl_server)
		__dl_server_attach_root(&rq->fair_server, rq);

#ifdef CONFIG_SCHED_CLASS_EXT
	எனில் (rq->ext_server.dl_server)
		__dl_server_attach_root(&rq->ext_server, rq);
#endif

	rq_unlock_irqrestore(rq, &rf);

	எனில் (old_rd)
		call_rcu(&old_rd->rcu, free_rootdomain);
}

வெறுமை sched_get_rd(கட்டமைப்பு வேர்_domain *rd)
{
	atomic_inc(&rd->refcount);
}

வெறுமை sched_put_rd(கட்டமைப்பு வேர்_domain *rd)
{
	எனில் (!atomic_dec_and_test(&rd->refcount))
		திரும்பு;

	call_rcu(&rd->rcu, free_rootdomain);
}

நிலையான முழு init_rootdomain(கட்டமைப்பு வேர்_domain *rd)
{
	எனில் (!zalloc_cpumask_var(&rd->span, GFP_KERNEL))
		செல் out;
	எனில் (!zalloc_cpumask_var(&rd->online, GFP_KERNEL))
		செல் free_span;
	எனில் (!zalloc_cpumask_var(&rd->dlo_mask, GFP_KERNEL))
		செல் free_online;
	எனில் (!zalloc_cpumask_var(&rd->rto_mask, GFP_KERNEL))
		செல் free_dlo_mask;

#ifdef HAVE_RT_தள்ளு_IPI
	rd->rto_cpu = -1;
	raw_spin_lock_init(&rd->rto_lock);
	rd->rto_தள்ளு_work = IRQ_WORK_INIT_HARD(rto_தள்ளு_irq_work_func);
#endif

	rd->visit_cookie = 0;
	init_dl_bw(&rd->dl_bw);
	எனில் (cpudl_init(&rd->cpudl) != 0)
		செல் free_rto_mask;

	எனில் (cpupri_init(&rd->cpupri) != 0)
		செல் free_cpudl;
	திரும்பு 0;

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
	திரும்பு -ENOMEM;
}

/*
 * By default the system creates a single root-domain with all CPUs as
 * members (mimicking the global state we have today).
 */
கட்டமைப்பு வேர்_domain def_வேர்_domain;

வெறுமை __init init_defrootdomain(வெறுமை)
{
	init_rootdomain(&def_வேர்_domain);

	atomic_set(&def_வேர்_domain.refcount, 1);
}

நிலையான கட்டமைப்பு வேர்_domain *alloc_rootdomain(வெறுமை)
{
	கட்டமைப்பு வேர்_domain *rd;

	rd = kzalloc_obj(*rd);
	எனில் (!rd)
		திரும்பு வெற்று;

	எனில் (init_rootdomain(rd) != 0) {
		kfree(rd);
		திரும்பு வெற்று;
	}

	திரும்பு rd;
}

நிலையான வெறுமை free_sched_groups(கட்டமைப்பு sched_group *sg, முழு free_sgc)
{
	கட்டமைப்பு sched_group *tmp, *first;

	எனில் (!sg)
		திரும்பு;

	first = sg;
	செய் {
		tmp = sg->next;

		எனில் (free_sgc && atomic_dec_and_test(&sg->sgc->ref))
			kfree(sg->sgc);

		எனில் (atomic_dec_and_test(&sg->ref))
			kfree(sg);
		sg = tmp;
	} போது (sg != first);
}

நிலையான வெறுமை free_sched_domain_shared(கட்டமைப்பு sched_domain_shared *sds)
{
	எனில் (sds && atomic_dec_and_test(&sds->ref))
		kfree(sds);
}

நிலையான வெறுமை destroy_sched_domain(கட்டமைப்பு sched_domain *sd)
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

நிலையான வெறுமை destroy_sched_domains_rcu(கட்டமைப்பு rcu_head *rcu)
{
	கட்டமைப்பு sched_domain *sd = container_of(rcu, கட்டமைப்பு sched_domain, rcu);

	போது (sd) {
		கட்டமைப்பு sched_domain *parent = sd->parent;
		destroy_sched_domain(sd);
		sd = parent;
	}
}

நிலையான வெறுமை destroy_sched_domains(கட்டமைப்பு sched_domain *sd)
{
	எனில் (sd)
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
DEFINE_PER_CPU(கட்டமைப்பு sched_domain __rcu *, sd_llc);
DEFINE_PER_CPU(முழு, sd_llc_size);
DEFINE_PER_CPU(முழு, sd_llc_id) = -1;
DEFINE_PER_CPU(முழு, sd_share_id);
DEFINE_PER_CPU(கட்டமைப்பு sched_domain_shared __rcu *, sd_llc_shared);
DEFINE_PER_CPU(கட்டமைப்பு sched_domain_shared __rcu *, sd_balance_shared);
DEFINE_PER_CPU(கட்டமைப்பு sched_domain __rcu *, sd_numa);
DEFINE_PER_CPU(கட்டமைப்பு sched_domain __rcu *, sd_asym_packing);
DEFINE_PER_CPU(கட்டமைப்பு sched_domain __rcu *, sd_asym_cpucapacity);

DEFINE_STATIC_சாவி_FALSE(sched_asym_cpucapacity);
DEFINE_STATIC_சாவி_FALSE(sched_cluster_active);

நிலையான வெறுமை update_உச்சி_cache_domain(முழு cpu)
{
	கட்டமைப்பு sched_domain_shared *sds = வெற்று;
	கட்டமைப்பு sched_domain *sd;
	முழு id = cpu;
	முழு size = 1;

	sd = highest_flag_domain(cpu, SD_SHARE_LLC);
	எனில் (sd) {
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
	எனில் (sd)
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
	எனில் (sd && sd->shared)
		sds = sd->shared;

	rcu_assign_pointer(per_cpu(sd_asym_cpucapacity, cpu), sd);
	rcu_assign_pointer(per_cpu(sd_balance_shared, cpu), sds);
}

/*
 * Attach the domain 'sd' to 'cpu' as its base domain. Callers must
 * hold the hotplug lock.
 */
நிலையான வெறுமை
cpu_attach_domain(கட்டமைப்பு sched_domain *sd, கட்டமைப்பு வேர்_domain *rd, முழு cpu)
{
	கட்டமைப்பு rq *rq = cpu_rq(cpu);
	கட்டமைப்பு sched_domain *tmp;

	/* Remove the sched domains which do not contribute to scheduling. */
	க்கு (tmp = sd; tmp; ) {
		கட்டமைப்பு sched_domain *parent = tmp->parent;
		எனில் (!parent)
			நிறுத்து;

		எனில் (sd_parent_degenerate(tmp, parent)) {
			tmp->parent = parent->parent;

			/* Pick reference to parent->shared. */
			எனில் (parent->shared) {
				/*
				 * It is safe to free a sd->shared that
				 * has not been published yet. If a
				 * sd->shared was published, the refcount
				 * will end up being non-zero and it will
				 * not be freed here.
				 */
				free_sched_domain_shared(tmp->shared);
				tmp->shared = parent->shared;
				parent->shared = வெற்று;
			}

			எனில் (parent->parent) {
				parent->parent->child = tmp;
				parent->parent->groups->flags = tmp->flags;
			}

			/*
			 * Transfer SD_PREFER_SIBLING down in case of a
			 * degenerate parent; the spans match for this
			 * so the property transfers.
			 */
			எனில் (parent->flags & SD_PREFER_SIBLING)
				tmp->flags |= SD_PREFER_SIBLING;
			destroy_sched_domain(parent);
		} இல்லையெனில்
			tmp = tmp->parent;
	}

	எனில் (sd && sd_degenerate(sd)) {
		tmp = sd;
		sd = sd->parent;

		எனில் (sd) {
			கட்டமைப்பு sched_group *sg = sd->groups;

#ifdef CONFIG_SCHED_CACHE
			/* move buffer to parent as child is being destroyed */
			sd->llc_counts = tmp->llc_counts;
			sd->llc_max = tmp->llc_max;
			sd->llc_bytes = tmp->llc_bytes;
			/* make sure destroy_sched_domain() does not free it */
			tmp->llc_counts = வெற்று;
			tmp->llc_max = 0;
			tmp->llc_bytes = 0;
#endif
			/*
			 * sched groups hold the flags of the child sched
			 * domain for convenience. Clear such flags since
			 * the child is being destroyed.
			 */
			செய் {
				sg->flags = 0;
			} போது (sg != sd->groups);

			sd->child = வெற்று;
		}

		destroy_sched_domain(tmp);
	}

	sched_domain_debug(sd, cpu);

	rq_attach_வேர்(rq, rd);
	tmp = rq->sd;
	rcu_assign_pointer(rq->sd, sd);
	dirty_sched_domain_sysctl(cpu);
	destroy_sched_domains(tmp);

	update_உச்சி_cache_domain(cpu);
}

கட்டமைப்பு s_data {
	கட்டமைப்பு sched_domain_shared * __percpu *sds;
	கட்டமைப்பு sched_domain * __percpu *sd;
	கட்டமைப்பு வேர்_domain	*rd;
};

எண்ணிக்கை s_alloc {
	sa_rootdomain,
	sa_sd,
	sa_sd_shared,
	sa_sd_storage,
	sa_none,
};

#ifdef CONFIG_SCHED_CACHE
/* hardware support for cache aware scheduling */
DEFINE_STATIC_சாவி_FALSE(sched_cache_present);
/*
 * Indicator of whether cache aware scheduling
 * is active, used by the scheduler.
 */
DEFINE_STATIC_சாவி_FALSE(sched_cache_active);
/* user wants cache aware scheduling [0 or 1] */
முழு sysctl_sched_cache_user = 1;

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
நிலையான unsigned long get_effective_llc_bytes(முழு cpu,
					     கட்டமைப்பு sched_domain *sd)
{
	கட்டமைப்பு cacheinfo *ci;
	unsigned முழு hw_weight;

	ci = get_cpu_cacheinfo_llc(cpu);
	எனில் (!ci)
		திரும்பு 0;

	hw_weight = cpumask_weight(&ci->shared_cpu_map);
	எனில் (!hw_weight)
		திரும்பு 0;

	திரும்பு div_u64((u64)ci->size * sd->span_weight, hw_weight);
}

நிலையான bool alloc_sd_llc(மாறிலி கட்டமைப்பு cpumask *cpu_map,
			 கட்டமைப்பு s_data *d)
{
	கட்டமைப்பு sched_domain *sd, *உச்சி_llc, *parent;
	unsigned முழு *p;
	முழு i;

	for_each_cpu(i, cpu_map) {
		sd = *per_cpu_ptr(d->sd, i);
		எனில் (!sd)
			செல் err;

		p = kcalloc_முனை(max_lid + 1, அளவு(unsigned முழு),
				 GFP_KERNEL, cpu_to_முனை(i));
		எனில் (!p)
			செல் err;

		உச்சி_llc = sd;
		/*
		 * Find the topmost SD_SHARE_LLC domain.
		 * Not yet attached to the CPU, so per_cpu(sd_llc, i)
		 * can not be used.
		 */
		போது ((parent = rcu_dereference_protected(உச்சி_llc->parent, true)) &&
		       (parent->flags & SD_SHARE_LLC))
			உச்சி_llc = parent;

		எனில் (உச்சி_llc->flags & SD_SHARE_LLC) {
			sd->llc_max = max_lid + 1;
			sd->llc_counts = p;
			sd->llc_bytes = get_effective_llc_bytes(i, உச்சி_llc);
		} இல்லையெனில் {
			/* avoid memory leak */
			kfree(p);
		}
	}

	திரும்பு true;
err:
	for_each_cpu(i, cpu_map) {
		sd = *per_cpu_ptr(d->sd, i);
		எனில் (sd) {
			kfree(sd->llc_counts);
			sd->llc_counts = வெற்று;
			sd->llc_max = 0;
			sd->llc_bytes = 0;
		}
	}

	திரும்பு false;
}

/*
 * Enable/disable cache aware scheduling according to
 * user input and the presence of hardware support.
 */
நிலையான வெறுமை _sched_cache_active_set(வெறுமை)
{
	lockdep_assert_cpus_held();
	lockdep_assert_held(&sched_domains_mutex);

	/* hardware does not support */
	எனில் (!static_branch_likely(&sched_cache_present)) {
		static_branch_disable_cpuslocked(&sched_cache_active);
		எனில் (sched_debug())
			pr_info("%s: cache aware scheduling not supported on this platform\n", __func__);
		திரும்பு;
	}

	/*
	 * user wants it or not ?
	 * TBD: read before writing the static key.
	 * It is not in the critical path, leave as-is
	 * for now.
	 */
	எனில் (sysctl_sched_cache_user) {
		static_branch_enable_cpuslocked(&sched_cache_active);
		எனில் (sched_debug())
			pr_info("%s: enabling cache aware scheduling\n", __func__);
	} இல்லையெனில் {
		static_branch_disable_cpuslocked(&sched_cache_active);
		எனில் (sched_debug())
			pr_info("%s: disabling cache aware scheduling\n", __func__);
	}
}

/* used by debugfs */
வெறுமை sched_cache_active_set(வெறுமை)
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
வெறுமை sched_update_llc_bytes(மாறிலி கட்டமைப்பு cpumask *cpus)
{
	கட்டமைப்பு sched_domain *sd, *sdp;
	unsigned முழு i;

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
		எனில் (!sdp)
			தொடர்;

		sd = rcu_dereference_sched_domain(cpu_rq(i)->sd);
		எனில் (sd)
			sd->llc_bytes = get_effective_llc_bytes(i, sdp);
	}

	sched_domains_mutex_unlock();
}

நிலையான வெறுமை sched_cache_set(bool has_multi_llcs)
{
	/*
	 * TBD: check before writing to it. sched domain rebuild
	 * is not in the critical path, leave as-is for now.
	 */
	எனில் (has_multi_llcs)
		static_branch_enable_cpuslocked(&sched_cache_present);
	இல்லையெனில்
		static_branch_disable_cpuslocked(&sched_cache_present);

	_sched_cache_active_set();
}
#இல்லையெனில்
நிலையான bool alloc_sd_llc(மாறிலி கட்டமைப்பு cpumask *cpu_map,
			 கட்டமைப்பு s_data *d)
{
	திரும்பு false;
}
நிலையான உள்வரி வெறுமை sched_cache_set(bool has_multi_llcs) { }
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
நிலையான bool sd_in_multi_llcs(கட்டமைப்பு sched_domain *sd)
{
	கட்டமைப்பு sched_domain *sdp = sd->parent;

	/* it does not make sense to aggregate to 1 CPU */
	எனில் (sd->span_weight == 1)
		திரும்பு false;

	போது (sdp && sdp->span_weight == sd->span_weight)
		sdp = sdp->parent;

	திரும்பு !!sdp;
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
முழு group_balance_cpu(கட்டமைப்பு sched_group *sg)
{
	திரும்பு cpumask_first(group_balance_mask(sg));
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
நிலையான வெறுமை
build_balance_mask(கட்டமைப்பு sched_domain *sd, கட்டமைப்பு sched_group *sg, கட்டமைப்பு cpumask *mask)
{
	மாறிலி கட்டமைப்பு cpumask *sg_span = sched_group_span(sg);
	கட்டமைப்பு sd_data *sdd = sd->தனிப்பட்ட;
	கட்டமைப்பு sched_domain *sibling;
	முழு i;

	cpumask_clear(mask);

	for_each_cpu(i, sg_span) {
		sibling = *per_cpu_ptr(sdd->sd, i);

		/*
		 * Can happen in the asymmetric case, where these siblings are
		 * unused. The mask will not be empty because those CPUs that
		 * do have the top domain _should_ span the domain.
		 */
		எனில் (!sibling->child)
			தொடர்;

		/* If we would not end up here, we can't continue from here */
		எனில் (!cpumask_equal(sg_span, sched_domain_span(sibling->child)))
			தொடர்;

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
நிலையான கட்டமைப்பு sched_group *
build_group_from_child_sched_domain(கட்டமைப்பு sched_domain *sd, முழு cpu)
{
	கட்டமைப்பு sched_group *sg;
	கட்டமைப்பு cpumask *sg_span;

	sg = kzalloc_முனை(அளவு(கட்டமைப்பு sched_group) + cpumask_size(),
			GFP_KERNEL, cpu_to_முனை(cpu));

	எனில் (!sg)
		திரும்பு வெற்று;

	sg_span = sched_group_span(sg);
	எனில் (sd->child) {
		cpumask_copy(sg_span, sched_domain_span(sd->child));
		sg->flags = sd->child->flags;
	} இல்லையெனில் {
		cpumask_copy(sg_span, sched_domain_span(sd));
	}

	atomic_inc(&sg->ref);
	திரும்பு sg;
}

நிலையான வெறுமை init_overlap_sched_group(கட்டமைப்பு sched_domain *sd,
				     கட்டமைப்பு sched_group *sg)
{
	கட்டமைப்பு cpumask *mask = sched_domains_tmpmask2;
	கட்டமைப்பு sd_data *sdd = sd->தனிப்பட்ட;
	கட்டமைப்பு cpumask *sg_span;
	முழு cpu;

	build_balance_mask(sd, sg, mask);
	cpu = cpumask_first(mask);

	sg->sgc = *per_cpu_ptr(sdd->sgc, cpu);
	எனில் (atomic_inc_return(&sg->sgc->ref) == 1)
		cpumask_copy(group_balance_mask(sg), mask);
	இல்லையெனில்
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

நிலையான கட்டமைப்பு sched_domain *
find_descended_sibling(கட்டமைப்பு sched_domain *sd, கட்டமைப்பு sched_domain *sibling)
{
	/*
	 * The proper descendant would be the one whose child won't span out
	 * of sd
	 */
	போது (sibling->child &&
	       !cpumask_subset(sched_domain_span(sibling->child),
			       sched_domain_span(sd)))
		sibling = sibling->child;

	/*
	 * As we are referencing sgc across different topology level, we need
	 * to go down to skip those sched_domains which don't contribute to
	 * scheduling because they will be degenerated in cpu_attach_domain
	 */
	போது (sibling->child &&
	       cpumask_equal(sched_domain_span(sibling->child),
			     sched_domain_span(sibling)))
		sibling = sibling->child;

	திரும்பு sibling;
}

நிலையான முழு
build_overlap_sched_groups(கட்டமைப்பு sched_domain *sd, முழு cpu)
{
	கட்டமைப்பு sched_group *first = வெற்று, *last = வெற்று, *sg;
	மாறிலி கட்டமைப்பு cpumask *span = sched_domain_span(sd);
	கட்டமைப்பு cpumask *covered = sched_domains_tmpmask;
	கட்டமைப்பு sd_data *sdd = sd->தனிப்பட்ட;
	கட்டமைப்பு sched_domain *sibling;
	முழு i;

	cpumask_clear(covered);

	for_each_cpu_wrap(i, span, cpu) {
		கட்டமைப்பு cpumask *sg_span;

		எனில் (cpumask_test_cpu(i, covered))
			தொடர்;

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
		எனில் (!cpumask_test_cpu(i, sched_domain_span(sibling)))
			தொடர்;

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
		எனில் (sibling->child &&
		    !cpumask_subset(sched_domain_span(sibling->child), span))
			sibling = find_descended_sibling(sd, sibling);

		sg = build_group_from_child_sched_domain(sibling, cpu);
		எனில் (!sg)
			செல் fail;

		sg_span = sched_group_span(sg);
		cpumask_or(covered, covered, sg_span);

		init_overlap_sched_group(sibling, sg);

		எனில் (!first)
			first = sg;
		எனில் (last)
			last->next = sg;
		last = sg;
		last->next = first;
	}
	sd->groups = first;

	திரும்பு 0;

fail:
	free_sched_groups(first, 0);

	திரும்பு -ENOMEM;
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

நிலையான கட்டமைப்பு sched_group *get_group(முழு cpu, கட்டமைப்பு sd_data *sdd)
{
	கட்டமைப்பு sched_domain *sd = *per_cpu_ptr(sdd->sd, cpu);
	கட்டமைப்பு sched_domain *child = sd->child;
	கட்டமைப்பு sched_group *sg;
	bool already_visited;

	எனில் (child)
		cpu = cpumask_first(sched_domain_span(child));

	sg = *per_cpu_ptr(sdd->sg, cpu);
	sg->sgc = *per_cpu_ptr(sdd->sgc, cpu);

	/* Increase refcounts for claim_allocations: */
	already_visited = atomic_inc_return(&sg->ref) > 1;
	/* sgc visits should follow a similar trend as sg */
	WARN_ON(already_visited != (atomic_inc_return(&sg->sgc->ref) > 1));

	/* If we have already visited that group, it's already initialized. */
	எனில் (already_visited)
		திரும்பு sg;

	எனில் (child) {
		cpumask_copy(sched_group_span(sg), sched_domain_span(child));
		cpumask_copy(group_balance_mask(sg), sched_group_span(sg));
		sg->flags = child->flags;
	} இல்லையெனில் {
		cpumask_set_cpu(cpu, sched_group_span(sg));
		cpumask_set_cpu(cpu, group_balance_mask(sg));
	}

	sg->sgc->capacity = SCHED_CAPACITY_SCALE * cpumask_weight(sched_group_span(sg));
	sg->sgc->min_capacity = SCHED_CAPACITY_SCALE;
	sg->sgc->max_capacity = SCHED_CAPACITY_SCALE;

	திரும்பு sg;
}

/*
 * build_sched_groups will build a circular linked list of the groups
 * covered by the given span, will set each group's ->cpumask correctly,
 * and will initialize their ->sgc.
 *
 * Assumes the sched_domain tree is fully constructed
 */
நிலையான முழு
build_sched_groups(கட்டமைப்பு sched_domain *sd, முழு cpu)
{
	கட்டமைப்பு sched_group *first = வெற்று, *last = வெற்று;
	கட்டமைப்பு sd_data *sdd = sd->தனிப்பட்ட;
	மாறிலி கட்டமைப்பு cpumask *span = sched_domain_span(sd);
	கட்டமைப்பு cpumask *covered;
	முழு i;

	lockdep_assert_held(&sched_domains_mutex);
	covered = sched_domains_tmpmask;

	cpumask_clear(covered);

	for_each_cpu_wrap(i, span, cpu) {
		கட்டமைப்பு sched_group *sg;

		எனில் (cpumask_test_cpu(i, covered))
			தொடர்;

		sg = get_group(i, sdd);

		cpumask_or(covered, covered, sched_group_span(sg));

		எனில் (!first)
			first = sg;
		எனில் (last)
			last->next = sg;
		last = sg;
	}
	last->next = first;
	sd->groups = first;

	திரும்பு 0;
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
நிலையான வெறுமை init_sched_groups_capacity(முழு cpu, கட்டமைப்பு sched_domain *sd)
{
	கட்டமைப்பு sched_group *sg = sd->groups;
	கட்டமைப்பு cpumask *mask = sched_domains_tmpmask2;

	WARN_ON(!sg);

	செய் {
		முழு cpu, cores = 0, max_cpu = -1;

		sg->group_weight = cpumask_weight(sched_group_span(sg));

		cpumask_copy(mask, sched_group_span(sg));
		for_each_cpu(cpu, mask) {
			cores++;
			cpumask_andnot(mask, mask, cpu_smt_mask(cpu));
		}
		sg->cores = cores;

		எனில் (!(sd->flags & SD_ASYM_PACKING))
			செல் next;

		for_each_cpu(cpu, sched_group_span(sg)) {
			எனில் (max_cpu < 0)
				max_cpu = cpu;
			இல்லையெனில் எனில் (sched_asym_prefer(cpu, max_cpu))
				max_cpu = cpu;
		}
		sg->asym_prefer_cpu = max_cpu;

next:
		sg = sg->next;
	} போது (sg != sd->groups);

	எனில் (cpu != group_balance_cpu(sg))
		திரும்பு;

	update_group_capacity(sd, cpu);
}

/* Update the "asym_prefer_cpu" when arch_asym_cpu_priority() changes. */
வெறுமை sched_update_asym_prefer_cpu(முழு cpu, முழு old_prio, முழு new_prio)
{
	முழு asym_prefer_cpu = cpu;
	கட்டமைப்பு sched_domain *sd;

	guard(rcu)();

	for_each_domain(cpu, sd) {
		கட்டமைப்பு sched_group *sg;
		முழு group_cpu;

		எனில் (!(sd->flags & SD_ASYM_PACKING))
			தொடர்;

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
		எனில் (cpu != sg->asym_prefer_cpu) {
			/*
			 * Since the parent is a superset of the current group,
			 * if the cpu is not the "asym_prefer_cpu" at the
			 * current level, it cannot be the preferred CPU at a
			 * higher levels either.
			 */
			எனில் (!sched_asym_prefer(cpu, sg->asym_prefer_cpu))
				திரும்பு;

			WRITE_ONCE(sg->asym_prefer_cpu, cpu);
			தொடர்;
		}

		/* Ranking has improved; CPU is still the preferred one. */
		எனில் (new_prio >= old_prio)
			தொடர்;

		for_each_cpu(group_cpu, sched_group_span(sg)) {
			எனில் (sched_asym_prefer(group_cpu, asym_prefer_cpu))
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
நிலையான உள்வரி முழு
asym_cpu_capacity_classify(மாறிலி கட்டமைப்பு cpumask *sd_span,
			   மாறிலி கட்டமைப்பு cpumask *cpu_map)
{
	கட்டமைப்பு asym_cap_data *entry;
	முழு மொத்தம் = 0, miss = 0;

	/*
	 * Count how many unique CPU capacities this domain spans across
	 * (compare sched_domain CPUs mask with ones representing  available
	 * CPUs capacities). Take into account CPUs that might be offline:
	 * skip those.
	 */
	list_for_each_entry(entry, &asym_cap_list, link) {
		எனில் (cpumask_intersects(sd_span, cpu_capacity_span(entry)))
			++மொத்தம்;
		இல்லையெனில் எனில் (cpumask_intersects(cpu_map, cpu_capacity_span(entry)))
			++miss;
	}

	WARN_ON_ONCE(!மொத்தம் && !list_empty(&asym_cap_list));

	/* No asymmetry detected */
	எனில் (மொத்தம் < 2)
		திரும்பு 0;
	/* Some of the available CPU capacity values have not been detected */
	எனில் (miss)
		திரும்பு SD_ASYM_CPUCAPACITY;

	/* Full asymmetry */
	திரும்பு SD_ASYM_CPUCAPACITY | SD_ASYM_CPUCAPACITY_FULL;

}

நிலையான வெறுமை free_asym_cap_entry(கட்டமைப்பு rcu_head *head)
{
	கட்டமைப்பு asym_cap_data *entry = container_of(head, கட்டமைப்பு asym_cap_data, rcu);
	kfree(entry);
}

நிலையான உள்வரி வெறுமை asym_cpu_capacity_update_data(முழு cpu)
{
	unsigned long capacity = arch_scale_cpu_capacity(cpu);
	கட்டமைப்பு asym_cap_data *செருகு_entry = வெற்று;
	கட்டமைப்பு asym_cap_data *entry;

	/*
	 * Search if capacity already exits. If not, track which the entry
	 * where we should insert to keep the list ordered descending.
	 */
	list_for_each_entry(entry, &asym_cap_list, link) {
		எனில் (capacity == entry->capacity)
			செல் done;
		இல்லையெனில் எனில் (!செருகு_entry && capacity > entry->capacity)
			செருகு_entry = list_prev_entry(entry, link);
	}

	entry = kzalloc(அளவு(*entry) + cpumask_size(), GFP_KERNEL);
	எனில் (WARN_ONCE(!entry, "Failed to allocate memory for asymmetry data\n"))
		திரும்பு;
	entry->capacity = capacity;

	/* If NULL then the new capacity is the smallest, add last. */
	எனில் (!செருகு_entry)
		list_add_tail_rcu(&entry->link, &asym_cap_list);
	இல்லையெனில்
		list_add_rcu(&entry->link, &செருகு_entry->link);
done:
	__cpumask_set_cpu(cpu, cpu_capacity_span(entry));
}

/*
 * Build-up/update list of CPUs grouped by their capacities
 * An update requires explicit request to rebuild sched domains
 * with state indicating CPU topology changes.
 */
நிலையான வெறுமை asym_cpu_capacity_scan(வெறுமை)
{
	கட்டமைப்பு asym_cap_data *entry, *next;
	முழு cpu;

	list_for_each_entry(entry, &asym_cap_list, link)
		cpumask_clear(cpu_capacity_span(entry));

	for_each_cpu_and(cpu, cpu_possible_mask, housekeeping_cpumask(HK_TYPE_DOMAIN))
		asym_cpu_capacity_update_data(cpu);

	list_for_each_entry_safe(entry, next, &asym_cap_list, link) {
		எனில் (cpumask_empty(cpu_capacity_span(entry))) {
			list_del_rcu(&entry->link);
			call_rcu(&entry->rcu, free_asym_cap_entry);
		}
	}

	/*
	 * Only one capacity value has been detected i.e. this system is symmetric.
	 * No need to keep this data around.
	 */
	எனில் (list_is_singular(&asym_cap_list)) {
		entry = list_first_entry(&asym_cap_list, typeof(*entry), link);
		list_del_rcu(&entry->link);
		call_rcu(&entry->rcu, free_asym_cap_entry);
	}
}

/*
 * Initializers for schedule domains
 * Non-inlined to reduce accumulated stack pressure in build_sched_domains()
 */

நிலையான முழு default_relax_domain_level = -1;
முழு sched_domain_level_max;

நிலையான முழு __init setup_relax_domain_level(எழுத்து *str)
{
	எனில் (kstrtoint(str, 0, &default_relax_domain_level))
		pr_warn("Unable to set relax_domain_level\n");

	திரும்பு 1;
}
__setup("relax_domain_level=", setup_relax_domain_level);

நிலையான வெறுமை set_domain_attribute(கட்டமைப்பு sched_domain *sd,
				 கட்டமைப்பு sched_domain_attr *attr)
{
	முழு request;

	எனில் (!attr || attr->relax_domain_level < 0) {
		எனில் (default_relax_domain_level < 0)
			திரும்பு;
		request = default_relax_domain_level;
	} இல்லையெனில்
		request = attr->relax_domain_level;

	எனில் (sd->level >= request) {
		/* Turn off idle balance on this domain: */
		sd->flags &= ~(SD_BALANCE_WAKE|SD_BALANCE_NEWIDLE);
	}
}

நிலையான வெறுமை __sdt_free(மாறிலி கட்டமைப்பு cpumask *cpu_map);
நிலையான முழு __sdt_alloc(மாறிலி கட்டமைப்பு cpumask *cpu_map);

நிலையான வெறுமை __sds_free(கட்டமைப்பு s_data *d, மாறிலி கட்டமைப்பு cpumask *cpu_map);
நிலையான முழு __sds_alloc(கட்டமைப்பு s_data *d, மாறிலி கட்டமைப்பு cpumask *cpu_map);

நிலையான வெறுமை __free_domain_allocs(கட்டமைப்பு s_data *d, எண்ணிக்கை s_alloc what,
				 மாறிலி கட்டமைப்பு cpumask *cpu_map)
{
	தேர்வு (what) {
	நிலை sa_rootdomain:
		எனில் (!atomic_read(&d->rd->refcount))
			free_rootdomain(&d->rd->rcu);
		fallthrough;
	நிலை sa_sd:
		free_percpu(d->sd);
		fallthrough;
	நிலை sa_sd_shared:
		__sds_free(d, cpu_map);
		fallthrough;
	நிலை sa_sd_storage:
		__sdt_free(cpu_map);
		fallthrough;
	நிலை sa_none:
		நிறுத்து;
	}
}

நிலையான எண்ணிக்கை s_alloc
__visit_domain_allocation_hell(கட்டமைப்பு s_data *d, மாறிலி கட்டமைப்பு cpumask *cpu_map)
{
	memset(d, 0, அளவு(*d));

	எனில் (__sdt_alloc(cpu_map))
		திரும்பு sa_sd_storage;
	எனில் (__sds_alloc(d, cpu_map))
		திரும்பு sa_sd_shared;
	d->sd = alloc_percpu(கட்டமைப்பு sched_domain *);
	எனில் (!d->sd)
		திரும்பு sa_sd_shared;
	d->rd = alloc_rootdomain();
	எனில் (!d->rd)
		திரும்பு sa_sd;

	திரும்பு sa_rootdomain;
}

/*
 * NULL the sd_data elements we've used to build the sched_domain and
 * sched_group structure so that the subsequent __free_domain_allocs()
 * will not free the data we're using.
 */
நிலையான வெறுமை claim_allocations(முழு cpu, கட்டமைப்பு s_data *d)
{
	கட்டமைப்பு sched_domain *sd;

	எனில் (atomic_read(&(*per_cpu_ptr(d->sds, cpu))->ref))
		*per_cpu_ptr(d->sds, cpu) = வெற்று;

	க்கு (sd = *per_cpu_ptr(d->sd, cpu); sd; sd = sd->parent) {
		கட்டமைப்பு sd_data *sdd = sd->தனிப்பட்ட;

		WARN_ON_ONCE(*per_cpu_ptr(sdd->sd, cpu) != sd);
		*per_cpu_ptr(sdd->sd, cpu) = வெற்று;

		எனில் (atomic_read(&(*per_cpu_ptr(sdd->sg, cpu))->ref))
			*per_cpu_ptr(sdd->sg, cpu) = வெற்று;

		எனில் (atomic_read(&(*per_cpu_ptr(sdd->sgc, cpu))->ref))
			*per_cpu_ptr(sdd->sgc, cpu) = வெற்று;
	}
}

#ifdef CONFIG_NUMA
எண்ணிக்கை numa_topology_type sched_numa_topology_type;

/*
 * sched_domains_numa_distance is derived from sched_numa_node_distance
 * and provides a simplified view of NUMA distances used specifically
 * for building NUMA scheduling domains.
 */
நிலையான முழு			sched_domains_numa_levels;
நிலையான முழு			sched_numa_முனை_levels;

முழு				sched_max_numa_distance;
நிலையான முழு			*sched_domains_numa_distance;
நிலையான முழு			*sched_numa_முனை_distance;
நிலையான கட்டமைப்பு cpumask		***sched_domains_numa_masks;
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

நிலையான கட்டமைப்பு sched_domain *
sd_init(கட்டமைப்பு sched_domain_topology_level *tl,
	மாறிலி கட்டமைப்பு cpumask *cpu_map,
	கட்டமைப்பு sched_domain *child, முழு cpu)
{
	கட்டமைப்பு sd_data *sdd = &tl->data;
	கட்டமைப்பு sched_domain *sd = *per_cpu_ptr(sdd->sd, cpu);
	முழு sd_id, sd_weight, sd_flags = 0;
	கட்டமைப்பு cpumask *sd_span;
	u64 now = sched_clock();

	sd_span = sched_domain_span(sd);
	cpumask_and(sd_span, cpu_map, tl->mask(tl, cpu));
	sd_weight = cpumask_weight(sd_span);
	sd_id = cpumask_first(sd_span);

	எனில் (tl->sd_flags)
		sd_flags = (*tl->sd_flags)();
	எனில் (WARN_ONCE(sd_flags & ~TOPOLOGY_SD_FLAGS,
		      "wrong sd_flags in topology description\n"))
		sd_flags &= TOPOLOGY_SD_FLAGS;
#ifdef CONFIG_SCHED_SMT
	எனில் (sched_smt_asym_packing && (sd_flags & SD_SHARE_CPUCAPACITY))
		sd_flags |= SD_ASYM_PACKING;
#endif
	sd_flags |= asym_cpu_capacity_classify(sd_span, cpu_map);

	*sd = (கட்டமைப்பு sched_domain){
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
	எனில் (sd->flags & SD_SHARE_CPUCAPACITY) {
		sd->imbalance_pct = 110;

	} இல்லையெனில் எனில் (sd->flags & SD_SHARE_LLC) {
		sd->imbalance_pct = 117;
		sd->cache_nice_tries = 1;

#ifdef CONFIG_NUMA
	} இல்லையெனில் எனில் (sd->flags & SD_NUMA) {
		sd->cache_nice_tries = 2;

		sd->flags &= ~SD_PREFER_SIBLING;
		sd->flags |= SD_SERIALIZE;
		எனில் (sched_domains_numa_distance[tl->numa_level] > முனை_reclaim_distance) {
			sd->flags &= ~(SD_BALANCE_EXEC |
				       SD_BALANCE_FORK |
				       SD_WAKE_AFFINE);
		}

#endif /* CONFIG_NUMA */
	} இல்லையெனில் {
		sd->cache_nice_tries = 1;
	}

	sd->தனிப்பட்ட = sdd;

	திரும்பு sd;
}

#ifdef CONFIG_SCHED_SMT
முழு cpu_smt_flags(வெறுமை)
{
	திரும்பு SD_SHARE_CPUCAPACITY | SD_SHARE_LLC;
}

மாறிலி கட்டமைப்பு cpumask *tl_smt_mask(கட்டமைப்பு sched_domain_topology_level *tl, முழு cpu)
{
	திரும்பு cpu_smt_mask(cpu);
}
#endif

#ifdef CONFIG_SCHED_CLUSTER
முழு cpu_cluster_flags(வெறுமை)
{
	திரும்பு SD_CLUSTER | SD_SHARE_LLC;
}

மாறிலி கட்டமைப்பு cpumask *tl_cls_mask(கட்டமைப்பு sched_domain_topology_level *tl, முழு cpu)
{
	திரும்பு cpu_clustergroup_mask(cpu);
}
#endif

#ifdef CONFIG_SCHED_MC
முழு cpu_core_flags(வெறுமை)
{
	திரும்பு SD_SHARE_LLC;
}

மாறிலி கட்டமைப்பு cpumask *tl_mc_mask(கட்டமைப்பு sched_domain_topology_level *tl, முழு cpu)
{
	திரும்பு cpu_coregroup_mask(cpu);
}

/*
 * Majority of architectures have LLC at MC domain level with exception
 * such as powerpc. Provide a way for arch to specify where its LLC is
 * if it falls in exception category
 */
# ifndef arch_llc_mask
#define arch_llc_mask(cpu) cpu_coregroup_mask(cpu)
# endif

#இல்லையெனில்
#define arch_llc_mask(cpu) cpumask_of(cpu)
#endif

#define llc_mask(cpu) arch_llc_mask(cpu)

மாறிலி கட்டமைப்பு cpumask *tl_pkg_mask(கட்டமைப்பு sched_domain_topology_level *tl, முழு cpu)
{
	திரும்பு cpu_முனை_mask(cpu);
}

/*
 * Topology list, bottom-up.
 */
நிலையான கட்டமைப்பு sched_domain_topology_level default_topology[] = {
#ifdef CONFIG_SCHED_SMT
	SDTL_INIT(tl_smt_mask, cpu_smt_flags, SMT),
#endif

#ifdef CONFIG_SCHED_CLUSTER
	SDTL_INIT(tl_cls_mask, cpu_cluster_flags, CLS),
#endif

#ifdef CONFIG_SCHED_MC
	SDTL_INIT(tl_mc_mask, cpu_core_flags, MC),
#endif
	SDTL_INIT(tl_pkg_mask, வெற்று, PKG),
	{ வெற்று, },
};

நிலையான கட்டமைப்பு sched_domain_topology_level *sched_domain_topology =
	default_topology;
நிலையான கட்டமைப்பு sched_domain_topology_level *sched_domain_topology_saved;

#define for_each_sd_topology(tl)			\
	க்கு (tl = sched_domain_topology; tl->mask; tl++)

வெறுமை __init set_sched_topology(கட்டமைப்பு sched_domain_topology_level *tl)
{
	எனில் (WARN_ON_ONCE(sched_smp_initialized))
		திரும்பு;

	sched_domain_topology = tl;
	sched_domain_topology_saved = வெற்று;
}

#ifdef CONFIG_NUMA
நிலையான முழு cpu_numa_flags(வெறுமை)
{
	திரும்பு SD_NUMA;
}

நிலையான மாறிலி கட்டமைப்பு cpumask *sd_numa_mask(கட்டமைப்பு sched_domain_topology_level *tl, முழு cpu)
{
	திரும்பு sched_domains_numa_masks[tl->numa_level][cpu_to_முனை(cpu)];
}

நிலையான வெறுமை sched_numa_warn(மாறிலி எழுத்து *str)
{
	நிலையான முழு done = false;
	முழு i,j;

	எனில் (done)
		திரும்பு;

	done = true;

	printk(KERN_WARNING "ERROR: %s\n\n", str);

	க்கு (i = 0; i < nr_முனை_ids; i++) {
		printk(KERN_WARNING "  ");
		க்கு (j = 0; j < nr_முனை_ids; j++) {
			எனில் (!முனை_state(i, N_CPU) || !முனை_state(j, N_CPU))
				printk(KERN_CONT "(%02d) ", முனை_distance(i,j));
			இல்லையெனில்
				printk(KERN_CONT " %02d  ", முனை_distance(i,j));
		}
		printk(KERN_CONT "\n");
	}
	printk(KERN_WARNING "\n");
}

bool find_numa_distance(முழு distance)
{
	bool found = false;
	முழு i, *distances;

	எனில் (distance == முனை_distance(0, 0))
		திரும்பு true;

	rcu_read_lock();
	distances = rcu_dereference(sched_numa_முனை_distance);
	எனில் (!distances)
		செல் unlock;
	க்கு (i = 0; i < sched_numa_முனை_levels; i++) {
		எனில் (distances[i] == distance) {
			found = true;
			நிறுத்து;
		}
	}
unlock:
	rcu_read_unlock();

	திரும்பு found;
}

#define for_each_cpu_முனை_but(n, nbut)		\
	for_each_முனை_state(n, N_CPU)		\
		எனில் (n == nbut)			\
			தொடர்;		\
		இல்லையெனில்

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
நிலையான வெறுமை init_numa_topology_type(முழு offline_முனை)
{
	முழு a, b, c, n;

	n = sched_max_numa_distance;

	எனில் (sched_domains_numa_levels <= 2) {
		sched_numa_topology_type = NUMA_DIRECT;
		திரும்பு;
	}

	for_each_cpu_முனை_but(a, offline_முனை) {
		for_each_cpu_முனை_but(b, offline_முனை) {
			/* Find two nodes furthest removed from each other. */
			எனில் (முனை_distance(a, b) < n)
				தொடர்;

			/* Is there an intermediary node between a and b? */
			for_each_cpu_முனை_but(c, offline_முனை) {
				எனில் (முனை_distance(a, c) < n &&
				    முனை_distance(b, c) < n) {
					sched_numa_topology_type =
							NUMA_GLUELESS_MESH;
					திரும்பு;
				}
			}

			sched_numa_topology_type = NUMA_BACKPLANE;
			திரும்பு;
		}
	}

	pr_err("Failed to find a NUMA topology type, defaulting to DIRECT\n");
	sched_numa_topology_type = NUMA_DIRECT;
}


#define NR_DISTANCE_மதிப்புகள் (1 << DISTANCE_BITS)

/*
 * An architecture could modify its NUMA distance, to change
 * grouping of NUMA nodes and number of NUMA levels when creating
 * NUMA level sched domains.
 *
 * A NUMA level is created for each unique
 * arch_sched_node_distance.
 */
நிலையான முழு numa_முனை_dist(முழு i, முழு j)
{
	திரும்பு முனை_distance(i, j);
}

முழு arch_sched_முனை_distance(முழு from, முழு to)
			     __weak __alias(numa_முனை_dist);

நிலையான bool modified_sched_முனை_distance(வெறுமை)
{
	திரும்பு numa_முனை_dist != arch_sched_முனை_distance;
}

நிலையான முழு sched_record_numa_dist(முழு offline_முனை, முழு (*n_dist)(முழு, முழு),
				  முழு **dist, முழு *levels)
{
	unsigned long *distance_map __free(bitmap) = வெற்று;
	முழு nr_levels = 0;
	முழு i, j;
	முழு *distances;

	/*
	 * O(nr_nodes^2) de-duplicating selection sort -- in order to find the
	 * unique distances in the node_distance() table.
	 */
	distance_map = bitmap_alloc(NR_DISTANCE_மதிப்புகள், GFP_KERNEL);
	எனில் (!distance_map)
		திரும்பு -ENOMEM;

	bitmap_zero(distance_map, NR_DISTANCE_மதிப்புகள்);
	for_each_cpu_முனை_but(i, offline_முனை) {
		for_each_cpu_முனை_but(j, offline_முனை) {
			முழு distance = n_dist(i, j);

			எனில் (distance < LOCAL_DISTANCE || distance >= NR_DISTANCE_மதிப்புகள்) {
				sched_numa_warn("Invalid distance value range");
				திரும்பு -EINVAL;
			}

			bitmap_set(distance_map, distance, 1);
		}
	}
	/*
	 * We can now figure out how many unique distance values there are and
	 * allocate memory accordingly.
	 */
	nr_levels = bitmap_weight(distance_map, NR_DISTANCE_மதிப்புகள்);

	distances = kzalloc_objs(முழு, nr_levels);
	எனில் (!distances)
		திரும்பு -ENOMEM;

	க்கு (i = 0, j = 0; i < nr_levels; i++, j++) {
		j = find_next_bit(distance_map, NR_DISTANCE_மதிப்புகள், j);
		distances[i] = j;
	}
	*dist = distances;
	*levels = nr_levels;

	திரும்பு 0;
}

வெறுமை sched_init_numa(முழு offline_முனை)
{
	கட்டமைப்பு sched_domain_topology_level *tl;
	முழு nr_levels, nr_முனை_levels;
	முழு i, j;
	முழு *distances, *domain_distances;
	கட்டமைப்பு cpumask ***masks;

	/* Record the NUMA distances from SLIT table */
	எனில் (sched_record_numa_dist(offline_முனை, numa_முனை_dist, &distances,
				   &nr_முனை_levels))
		திரும்பு;

	/* Record modified NUMA distances for building sched domains */
	எனில் (modified_sched_முனை_distance()) {
		எனில் (sched_record_numa_dist(offline_முனை, arch_sched_முனை_distance,
					   &domain_distances, &nr_levels)) {
			kfree(distances);
			திரும்பு;
		}
	} இல்லையெனில் {
		domain_distances = distances;
		nr_levels = nr_முனை_levels;
	}
	rcu_assign_pointer(sched_numa_முனை_distance, distances);
	WRITE_ONCE(sched_max_numa_distance, distances[nr_முனை_levels - 1]);
	WRITE_ONCE(sched_numa_முனை_levels, nr_முனை_levels);

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

	masks = kzalloc(அளவு(வெறுமை *) * nr_levels, GFP_KERNEL);
	எனில் (!masks)
		திரும்பு;

	/*
	 * Now for each level, construct a mask per node which contains all
	 * CPUs of nodes that are that many hops away from us.
	 */
	க்கு (i = 0; i < nr_levels; i++) {
		masks[i] = kzalloc(nr_முனை_ids * அளவு(வெறுமை *), GFP_KERNEL);
		எனில் (!masks[i])
			திரும்பு;

		for_each_cpu_முனை_but(j, offline_முனை) {
			கட்டமைப்பு cpumask *mask = kzalloc(cpumask_size(), GFP_KERNEL);
			முழு k;

			எனில் (!mask)
				திரும்பு;

			masks[i][j] = mask;

			for_each_cpu_முனை_but(k, offline_முனை) {
				எனில் (sched_debug() &&
				    (arch_sched_முனை_distance(j, k) !=
				     arch_sched_முனை_distance(k, j)))
					sched_numa_warn("Node-distance not symmetric");

				எனில் (arch_sched_முனை_distance(j, k) >
				    sched_domains_numa_distance[i])
					தொடர்;

				cpumask_or(mask, mask, cpumask_of_முனை(k));
			}
		}
	}
	rcu_assign_pointer(sched_domains_numa_masks, masks);

	/* Compute default topology size */
	க்கு (i = 0; sched_domain_topology[i].mask; i++);

	tl = kzalloc((i + nr_levels + 1) *
			அளவு(கட்டமைப்பு sched_domain_topology_level), GFP_KERNEL);
	எனில் (!tl)
		திரும்பு;

	/*
	 * Copy the default topology bits..
	 */
	க்கு (i = 0; sched_domain_topology[i].mask; i++)
		tl[i] = sched_domain_topology[i];

	/*
	 * Add the NUMA identity distance, aka single NODE.
	 */
	tl[i++] = SDTL_INIT(sd_numa_mask, வெற்று, முனை_2);

	/*
	 * .. and append 'j' levels of NUMA goodness.
	 */
	க்கு (j = 1; j < nr_levels; i++, j++) {
		tl[i] = SDTL_INIT(sd_numa_mask, cpu_numa_flags, NUMA);
		tl[i].numa_level = j;
	}

	sched_domain_topology_saved = sched_domain_topology;
	sched_domain_topology = tl;

	sched_domains_numa_levels = nr_levels;

	init_numa_topology_type(offline_முனை);
}


நிலையான வெறுமை sched_reset_numa(வெறுமை)
{
	முழு nr_levels, *distances, *dom_distances = வெற்று;
	கட்டமைப்பு cpumask ***masks;

	nr_levels = sched_domains_numa_levels;
	sched_numa_முனை_levels = 0;
	sched_domains_numa_levels = 0;
	sched_max_numa_distance = 0;
	sched_numa_topology_type = NUMA_DIRECT;
	distances = sched_numa_முனை_distance;
	எனில் (sched_numa_முனை_distance != sched_domains_numa_distance)
		dom_distances = sched_domains_numa_distance;
	rcu_assign_pointer(sched_numa_முனை_distance, வெற்று);
	rcu_assign_pointer(sched_domains_numa_distance, வெற்று);
	masks = sched_domains_numa_masks;
	rcu_assign_pointer(sched_domains_numa_masks, வெற்று);
	எனில் (distances || masks) {
		முழு i, j;

		synchronize_rcu();
		kfree(distances);
		kfree(dom_distances);
		க்கு (i = 0; i < nr_levels && masks; i++) {
			எனில் (!masks[i])
				தொடர்;
			for_each_முனை(j)
				kfree(masks[i][j]);
			kfree(masks[i]);
		}
		kfree(masks);
	}
	எனில் (sched_domain_topology_saved) {
		kfree(sched_domain_topology);
		sched_domain_topology = sched_domain_topology_saved;
		sched_domain_topology_saved = வெற்று;
	}
}

/*
 * Call with hotplug lock held
 */
வெறுமை sched_update_numa(முழு cpu, bool online)
{
	முழு முனை;

	முனை = cpu_to_முனை(cpu);
	/*
	 * Scheduler NUMA topology is updated when the first CPU of a
	 * node is onlined or the last CPU of a node is offlined.
	 */
	எனில் (cpumask_weight(cpumask_of_முனை(முனை)) != 1)
		திரும்பு;

	sched_reset_numa();
	sched_init_numa(online ? NUMA_NO_முனை : முனை);
}

வெறுமை sched_domains_numa_masks_set(unsigned முழு cpu)
{
	முழு முனை = cpu_to_முனை(cpu);
	முழு i, j;

	க்கு (i = 0; i < sched_domains_numa_levels; i++) {
		க்கு (j = 0; j < nr_முனை_ids; j++) {
			எனில் (!முனை_state(j, N_CPU))
				தொடர்;

			/* Set ourselves in the remote node's masks */
			எனில் (arch_sched_முனை_distance(j, முனை) <=
			    sched_domains_numa_distance[i])
				cpumask_set_cpu(cpu, sched_domains_numa_masks[i][j]);
		}
	}
}

வெறுமை sched_domains_numa_masks_clear(unsigned முழு cpu)
{
	முழு i, j;

	க்கு (i = 0; i < sched_domains_numa_levels; i++) {
		க்கு (j = 0; j < nr_முனை_ids; j++) {
			எனில் (sched_domains_numa_masks[i][j])
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
முழு sched_numa_find_closest(மாறிலி கட்டமைப்பு cpumask *cpus, முழு cpu)
{
	முழு i, j = cpu_to_முனை(cpu), found = nr_cpu_ids;
	கட்டமைப்பு cpumask ***masks;

	rcu_read_lock();
	masks = rcu_dereference(sched_domains_numa_masks);
	எனில் (!masks)
		செல் unlock;
	க்கு (i = 0; i < sched_domains_numa_levels; i++) {
		எனில் (!masks[i][j])
			நிறுத்து;
		cpu = cpumask_any_and_distribute(cpus, masks[i][j]);
		எனில் (cpu < nr_cpu_ids) {
			found = cpu;
			நிறுத்து;
		}
	}
unlock:
	rcu_read_unlock();

	திரும்பு found;
}

கட்டமைப்பு __cmp_key {
	மாறிலி கட்டமைப்பு cpumask *cpus;
	கட்டமைப்பு cpumask ***masks;
	முழு முனை;
	முழு cpu;
	முழு w;
};

நிலையான முழு hop_cmp(மாறிலி வெறுமை *a, மாறிலி வெறுமை *b)
{
	கட்டமைப்பு cpumask **prev_hop, **cur_hop = *(கட்டமைப்பு cpumask ***)b;
	கட்டமைப்பு __cmp_key *k = (கட்டமைப்பு __cmp_key *)a;

	எனில் (cpumask_weight_and(k->cpus, cur_hop[k->முனை]) <= k->cpu)
		திரும்பு 1;

	எனில் (b == k->masks) {
		k->w = 0;
		திரும்பு 0;
	}

	prev_hop = *((கட்டமைப்பு cpumask ***)b - 1);
	k->w = cpumask_weight_and(k->cpus, prev_hop[k->முனை]);
	எனில் (k->w <= k->cpu)
		திரும்பு 0;

	திரும்பு -1;
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
முழு sched_numa_find_nth_cpu(மாறிலி கட்டமைப்பு cpumask *cpus, முழு cpu, முழு முனை)
{
	கட்டமைப்பு __cmp_key k = { .cpus = cpus, .cpu = cpu };
	கட்டமைப்பு cpumask ***hop_masks;
	முழு hop, ret = nr_cpu_ids;

	எனில் (முனை == NUMA_NO_முனை)
		திரும்பு cpumask_nth_and(cpu, cpus, cpu_online_mask);

	rcu_read_lock();

	/* CPU-less node entries are uninitialized in sched_domains_numa_masks */
	முனை = numa_nearest_முனை(முனை, N_CPU);
	k.முனை = முனை;

	k.masks = rcu_dereference(sched_domains_numa_masks);
	எனில் (!k.masks)
		செல் unlock;

	hop_masks = bsearch(&k, k.masks, sched_domains_numa_levels, அளவு(k.masks[0]), hop_cmp);
	எனில் (!hop_masks)
		செல் unlock;
	hop = hop_masks	- k.masks;

	ret = hop ?
		cpumask_nth_and_andnot(cpu - k.w, cpus, k.masks[hop][முனை], k.masks[hop-1][முனை]) :
		cpumask_nth_and(cpu, cpus, k.masks[0][முனை]);
unlock:
	rcu_read_unlock();
	திரும்பு ret;
}
EXPORT_குறியீடு_GPL(sched_numa_find_nth_cpu);

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
மாறிலி கட்டமைப்பு cpumask *sched_numa_hop_mask(unsigned முழு முனை, unsigned முழு hops)
{
	கட்டமைப்பு cpumask ***masks;

	எனில் (முனை >= nr_முனை_ids || hops >= sched_domains_numa_levels)
		திரும்பு ERR_PTR(-EINVAL);

	masks = rcu_dereference(sched_domains_numa_masks);
	எனில் (!masks)
		திரும்பு ERR_PTR(-EBUSY);

	திரும்பு masks[hops][முனை];
}
EXPORT_குறியீடு_GPL(sched_numa_hop_mask);

#endif /* CONFIG_NUMA */

நிலையான முழு __sdt_alloc(மாறிலி கட்டமைப்பு cpumask *cpu_map)
{
	கட்டமைப்பு sched_domain_topology_level *tl;
	முழு j;

	for_each_sd_topology(tl) {
		கட்டமைப்பு sd_data *sdd = &tl->data;

		sdd->sd = alloc_percpu(கட்டமைப்பு sched_domain *);
		எனில் (!sdd->sd)
			திரும்பு -ENOMEM;

		sdd->sg = alloc_percpu(கட்டமைப்பு sched_group *);
		எனில் (!sdd->sg)
			திரும்பு -ENOMEM;

		sdd->sgc = alloc_percpu(கட்டமைப்பு sched_group_capacity *);
		எனில் (!sdd->sgc)
			திரும்பு -ENOMEM;

		for_each_cpu(j, cpu_map) {
			கட்டமைப்பு sched_domain *sd;
			கட்டமைப்பு sched_group *sg;
			கட்டமைப்பு sched_group_capacity *sgc;

			sd = kzalloc_முனை(அளவு(கட்டமைப்பு sched_domain) + cpumask_size(),
					GFP_KERNEL, cpu_to_முனை(j));
			எனில் (!sd)
				திரும்பு -ENOMEM;

			*per_cpu_ptr(sdd->sd, j) = sd;

			sg = kzalloc_முனை(அளவு(கட்டமைப்பு sched_group) + cpumask_size(),
					GFP_KERNEL, cpu_to_முனை(j));
			எனில் (!sg)
				திரும்பு -ENOMEM;

			sg->next = sg;

			*per_cpu_ptr(sdd->sg, j) = sg;

			sgc = kzalloc_முனை(அளவு(கட்டமைப்பு sched_group_capacity) + cpumask_size(),
					GFP_KERNEL, cpu_to_முனை(j));
			எனில் (!sgc)
				திரும்பு -ENOMEM;

			sgc->id = j;

			*per_cpu_ptr(sdd->sgc, j) = sgc;
		}
	}

	திரும்பு 0;
}

நிலையான வெறுமை __sdt_free(மாறிலி கட்டமைப்பு cpumask *cpu_map)
{
	கட்டமைப்பு sched_domain_topology_level *tl;
	முழு j;

	for_each_sd_topology(tl) {
		கட்டமைப்பு sd_data *sdd = &tl->data;

		for_each_cpu(j, cpu_map) {
			கட்டமைப்பு sched_domain *sd;

			எனில் (sdd->sd) {
				sd = *per_cpu_ptr(sdd->sd, j);
				எனில் (sd && (sd->flags & SD_NUMA))
					free_sched_groups(sd->groups, 0);
				kfree(*per_cpu_ptr(sdd->sd, j));
			}

			எனில் (sdd->sg)
				kfree(*per_cpu_ptr(sdd->sg, j));
			எனில் (sdd->sgc)
				kfree(*per_cpu_ptr(sdd->sgc, j));
		}
		free_percpu(sdd->sd);
		sdd->sd = வெற்று;
		free_percpu(sdd->sg);
		sdd->sg = வெற்று;
		free_percpu(sdd->sgc);
		sdd->sgc = வெற்று;
	}
}

நிலையான முழு __sds_alloc(கட்டமைப்பு s_data *d, மாறிலி கட்டமைப்பு cpumask *cpu_map)
{
	முழு j;

	d->sds = alloc_percpu(கட்டமைப்பு sched_domain_shared *);
	எனில் (!d->sds)
		திரும்பு -ENOMEM;

	for_each_cpu(j, cpu_map) {
		கட்டமைப்பு sched_domain_shared *sds;

		sds = kzalloc_முனை(அளவு(கட்டமைப்பு sched_domain_shared),
				GFP_KERNEL, cpu_to_முனை(j));
		எனில் (!sds)
			திரும்பு -ENOMEM;

		*per_cpu_ptr(d->sds, j) = sds;
	}

	திரும்பு 0;
}

நிலையான வெறுமை __sds_free(கட்டமைப்பு s_data *d, மாறிலி கட்டமைப்பு cpumask *cpu_map)
{
	முழு j;

	எனில் (!d->sds)
		திரும்பு;

	for_each_cpu(j, cpu_map)
		kfree(*per_cpu_ptr(d->sds, j));

	free_percpu(d->sds);
	d->sds = வெற்று;
}

நிலையான கட்டமைப்பு sched_domain *build_sched_domain(கட்டமைப்பு sched_domain_topology_level *tl,
		மாறிலி கட்டமைப்பு cpumask *cpu_map, கட்டமைப்பு sched_domain_attr *attr,
		கட்டமைப்பு sched_domain *child, முழு cpu)
{
	கட்டமைப்பு sched_domain *sd = sd_init(tl, cpu_map, child, cpu);

	எனில் (child) {
		sd->level = child->level + 1;
		sched_domain_level_max = max(sched_domain_level_max, sd->level);
		child->parent = sd;

		எனில் (!cpumask_subset(sched_domain_span(child),
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

	திரும்பு sd;
}

/*
 * Ensure topology masks are sane, i.e. there are no conflicts (overlaps) for
 * any two given CPUs on non-NUMA topology levels.
 */
நிலையான bool topology_span_sane(மாறிலி கட்டமைப்பு cpumask *cpu_map)
{
	கட்டமைப்பு sched_domain_topology_level *tl;
	கட்டமைப்பு cpumask *covered, *id_seen;
	முழு cpu;

	lockdep_assert_held(&sched_domains_mutex);
	covered = sched_domains_tmpmask;
	id_seen = sched_domains_tmpmask2;

	for_each_sd_topology(tl) {
		முழு tl_common_flags = 0;

		எனில் (tl->sd_flags)
			tl_common_flags = (*tl->sd_flags)();

		/* NUMA levels are allowed to overlap */
		எனில் (tl_common_flags & SD_NUMA)
			தொடர்;

		cpumask_clear(covered);
		cpumask_clear(id_seen);

		/*
		 * Non-NUMA levels cannot partially overlap - they must be either
		 * completely equal or completely disjoint. Otherwise we can end up
		 * breaking the sched_group lists - i.e. a later get_group() pass
		 * breaks the linking done for an earlier span.
		 */
		for_each_cpu(cpu, cpu_map) {
			மாறிலி கட்டமைப்பு cpumask *tl_cpu_mask = tl->mask(tl, cpu);
			முழு id;

			/* lowest bit set in this mask is used as a unique id */
			id = cpumask_first(tl_cpu_mask);

			எனில் (cpumask_test_cpu(id, id_seen)) {
				/* First CPU has already been seen, ensure identical spans */
				எனில் (!cpumask_equal(tl->mask(tl, id), tl_cpu_mask))
					திரும்பு false;
			} இல்லையெனில் {
				/* First CPU hasn't been seen before, ensure it's a completely new span */
				எனில் (cpumask_intersects(tl_cpu_mask, covered))
					திரும்பு false;

				cpumask_or(covered, covered, tl_cpu_mask);
				cpumask_set_cpu(id, id_seen);
			}
		}
	}
	திரும்பு true;
}

/*
 * Calculate an allowed NUMA imbalance such that LLCs do not get
 * imbalanced.
 */
நிலையான வெறுமை adjust_numa_imbalance(கட்டமைப்பு sched_domain *sd_llc)
{
	கட்டமைப்பு sched_domain *parent;
	unsigned முழு imb_span = 1;
	unsigned முழு imb = 0;
	unsigned முழு nr_llcs;

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
	எனில் (nr_llcs == 1)
		imb = sd_llc->parent->span_weight >> 3;
	இல்லையெனில்
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
	போது (parent && !(parent->flags & SD_NUMA))
		parent = parent->parent;

	imb_span = parent ? parent->span_weight : sd_llc->parent->span_weight;

	/* Update the upper remainder of the topology */
	parent = sd_llc->parent;
	போது (parent) {
		முழு factor = max(1U, (parent->span_weight / imb_span));

		parent->imb_numa_nr = imb * factor;
		parent = parent->parent;
	}
}

நிலையான வெறுமை
init_sched_domain_shared(கட்டமைப்பு s_data *d, கட்டமைப்பு sched_domain *sd, முழு flags)
{
	கட்டமைப்பு sched_domain_shared *sds = வெற்று;
	முழு cpu;

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

		எனில் (!sds->alloc_flags ||
		    sd->span_weight == 1 ||
		    sds->alloc_flags == flags) {
			sds->alloc_flags = flags;
			sd->shared = sds;
			நிறுத்து;
		}
	}

	/*
	 * Use the sd_shared corresponding to the last
	 * CPU in the span if none are avaialable.
	 */
	எனில் (WARN_ON_ONCE(!sd->shared))
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
நிலையான bool claim_asym_sched_domain_shared(கட்டமைப்பு s_data *d, முழு cpu)
{
	கட்டமைப்பு sched_domain *sd = *per_cpu_ptr(d->sd, cpu);
	கட்டமைப்பு sched_domain *sd_asym;

	எனில் (!sd)
		திரும்பு false;

	sd_asym = sd;
	போது (sd_asym && !(sd_asym->flags & SD_ASYM_CPUCAPACITY_FULL))
		sd_asym = sd_asym->parent;

	எனில் (!sd_asym || (sd_asym->flags & SD_NUMA))
		திரும்பு false;

	init_sched_domain_shared(d, sd_asym, SD_ASYM_CPUCAPACITY);
	திரும்பு true;
}

நிலையான முழு __sched_domains_alloc_llc_id(வெறுமை)
{
	முழு lid, max;

	lockdep_assert_held(&sched_domains_mutex);

	lid = cpumask_first_zero(sched_domains_llc_id_allocmask);
	/*
	 * llc_id space should never grow larger than the
	 * possible number of CPUs in the system.
	 */
	எனில் (lid >= nr_cpu_ids)
		திரும்பு -1;

	__cpumask_set_cpu(lid, sched_domains_llc_id_allocmask);
	max = cpumask_last(sched_domains_llc_id_allocmask);
	எனில் (max > max_lid)
		max_lid = max;

	திரும்பு lid;
}

நிலையான வெறுமை __sched_domains_free_llc_id(முழு cpu)
{
	முழு i, lid, max;

	lockdep_assert_held(&sched_domains_mutex);

	lid = per_cpu(sd_llc_id, cpu);
	எனில் (lid == -1 || lid >= nr_cpu_ids)
		திரும்பு;

	per_cpu(sd_llc_id, cpu) = -1;

	for_each_cpu(i, llc_mask(cpu)) {
		/* An online CPU owns the llc_id. */
		எனில் (per_cpu(sd_llc_id, i) == lid)
			திரும்பு;
	}

	__cpumask_clear_cpu(lid, sched_domains_llc_id_allocmask);

	max = cpumask_last(sched_domains_llc_id_allocmask);
	/* shrink max lid to save memory */
	எனில் (max < max_lid)
		max_lid = max;
}

வெறுமை sched_domains_free_llc_id(முழு cpu)
{
	sched_domains_mutex_lock();
	__sched_domains_free_llc_id(cpu);
	sched_domains_mutex_unlock();
}

/*
 * Build sched domains for a given set of CPUs and attach the sched domains
 * to the individual CPUs
 */
நிலையான முழு
build_sched_domains(மாறிலி கட்டமைப்பு cpumask *cpu_map, கட்டமைப்பு sched_domain_attr *attr,
		    bool *multi_llcs)
{
	எண்ணிக்கை s_alloc alloc_state = sa_none;
	bool has_multi_llcs = false;
	கட்டமைப்பு sched_domain *sd;
	கட்டமைப்பு s_data d;
	கட்டமைப்பு rq *rq = வெற்று;
	முழு i, ret = -ENOMEM;
	bool has_asym = false;
	bool has_cluster = false;

	எனில் (WARN_ON(cpumask_empty(cpu_map)))
		செல் error;

	alloc_state = __visit_domain_allocation_hell(&d, cpu_map);
	எனில் (alloc_state != sa_rootdomain)
		செல் error;

	/* Set up domains for CPUs specified by the cpu_map: */
	for_each_cpu(i, cpu_map) {
		கட்டமைப்பு sched_domain_topology_level *tl;
		முழு lid;

		sd = வெற்று;
		for_each_sd_topology(tl) {

			sd = build_sched_domain(tl, cpu_map, attr, sd, i);

			has_asym |= sd->flags & SD_ASYM_CPUCAPACITY;

			எனில் (tl == sched_domain_topology)
				*per_cpu_ptr(d.sd, i) = sd;
			எனில் (cpumask_equal(cpu_map, sched_domain_span(sd)))
				நிறுத்து;
		}

		lid = per_cpu(sd_llc_id, i);
		எனில் (lid == -1) {
			/* try to reuse the llc_id of its siblings */
			க்கு (முழு j = cpumask_first(llc_mask(i));
			     j < nr_cpu_ids;
			     j = cpumask_next(j, llc_mask(i))) {
				எனில் (i == j)
					தொடர்;

				lid = per_cpu(sd_llc_id, j);

				எனில் (lid != -1) {
					per_cpu(sd_llc_id, i) = lid;

					நிறுத்து;
				}
			}

			/* a new LLC is detected */
			எனில் (lid == -1)
				per_cpu(sd_llc_id, i) = __sched_domains_alloc_llc_id();
		}
	}

	எனில் (WARN_ON(!topology_span_sane(cpu_map)))
		செல் error;

	/* Build the groups for the domains */
	for_each_cpu(i, cpu_map) {
		க்கு (sd = *per_cpu_ptr(d.sd, i); sd; sd = sd->parent) {
			sd->span_weight = cpumask_weight(sched_domain_span(sd));
			எனில் (sd->flags & SD_NUMA) {
				எனில் (build_overlap_sched_groups(sd, i))
					செல் error;
			} இல்லையெனில் {
				எனில் (build_sched_groups(sd, i))
					செல் error;
			}
		}
	}

	for_each_cpu(i, cpu_map) {
		sd = *per_cpu_ptr(d.sd, i);
		எனில் (!sd)
			தொடர்;

		எனில் (has_asym)
			claim_asym_sched_domain_shared(&d, i);

		/* First, find the topmost SD_SHARE_LLC domain */
		போது (sd->parent && (sd->parent->flags & SD_SHARE_LLC))
			sd = sd->parent;

		எனில் (sd->flags & SD_SHARE_LLC) {
			init_sched_domain_shared(&d, sd, SD_SHARE_LLC);

			/*
			 * In presence of higher domains, adjust the
			 * NUMA imbalance stats for the hierarchy.
			 */
			எனில் (sd->parent) {
				எனில் (IS_ENABLED(CONFIG_NUMA))
					adjust_numa_imbalance(sd);

				எனில் (sd_in_multi_llcs(sd))
					has_multi_llcs = true;
			}
		}
	}

	/* Calculate CPU capacity for physical packages and nodes */
	க்கு (i = nr_cpumask_bits-1; i >= 0; i--) {
		எனில் (!cpumask_test_cpu(i, cpu_map))
			தொடர்;

		claim_allocations(i, &d);

		க்கு (sd = *per_cpu_ptr(d.sd, i); sd; sd = sd->parent)
			init_sched_groups_capacity(i, sd);
	}

	alloc_sd_llc(cpu_map, &d);

	/* Attach the domains */
	rcu_read_lock();
	for_each_cpu(i, cpu_map) {
		rq = cpu_rq(i);
		sd = *per_cpu_ptr(d.sd, i);

		cpu_attach_domain(sd, d.rd, i);

		எனில் (lowest_flag_domain(i, SD_CLUSTER))
			has_cluster = true;
	}
	rcu_read_unlock();

	எனில் (has_asym)
		static_branch_inc_cpuslocked(&sched_asym_cpucapacity);

	எனில் (has_cluster)
		static_branch_inc_cpuslocked(&sched_cluster_active);

	எனில் (rq && sched_debug_verbose)
		pr_info("root domain span: %*pbl\n", cpumask_pr_args(cpu_map));

	ret = 0;
error:
	*multi_llcs = has_multi_llcs;
	__free_domain_allocs(&d, alloc_state, cpu_map);

	திரும்பு ret;
}

/* Current sched domains: */
நிலையான cpumask_var_t			*doms_cur;

/* Number of sched domains in 'doms_cur': */
நிலையான முழு				ndoms_cur;

/* Attributes of custom domains in 'doms_cur' */
நிலையான கட்டமைப்பு sched_domain_attr		*dattr_cur;

/*
 * Special case: If a kmalloc() of a doms_cur partition (array of
 * cpumask) fails, then fallback to a single sched domain,
 * as determined by the single cpumask fallback_doms.
 */
நிலையான cpumask_var_t			fallback_doms;

/*
 * arch_update_cpu_topology lets virtualized architectures update the
 * CPU core maps. It is supposed to return 1 if the topology changed
 * or 0 if it stayed the same.
 */
முழு __weak arch_update_cpu_topology(வெறுமை)
{
	திரும்பு 0;
}

cpumask_var_t *alloc_sched_domains(unsigned முழு ndoms)
{
	முழு i;
	cpumask_var_t *doms;

	doms = kmalloc_objs(*doms, ndoms);
	எனில் (!doms)
		திரும்பு வெற்று;
	க்கு (i = 0; i < ndoms; i++) {
		எனில் (!alloc_cpumask_var(&doms[i], GFP_KERNEL)) {
			free_sched_domains(doms, i);
			திரும்பு வெற்று;
		}
	}
	திரும்பு doms;
}

வெறுமை free_sched_domains(cpumask_var_t doms[], unsigned முழு ndoms)
{
	unsigned முழு i;
	க்கு (i = 0; i < ndoms; i++)
		free_cpumask_var(doms[i]);
	kfree(doms);
}

/*
 * Set up scheduler domains and groups.  For now this just excludes isolated
 * CPUs, but could be used to exclude other special cases in the future.
 */
முழு __init sched_init_domains(மாறிலி கட்டமைப்பு cpumask *cpu_map)
{
	bool multi_llcs;
	முழு err;

	zalloc_cpumask_var(&sched_domains_llc_id_allocmask, GFP_KERNEL);
	zalloc_cpumask_var(&sched_domains_tmpmask, GFP_KERNEL);
	zalloc_cpumask_var(&sched_domains_tmpmask2, GFP_KERNEL);
	zalloc_cpumask_var(&fallback_doms, GFP_KERNEL);

	arch_update_cpu_topology();
	asym_cpu_capacity_scan();
	ndoms_cur = 1;
	doms_cur = alloc_sched_domains(ndoms_cur);
	எனில் (!doms_cur)
		doms_cur = &fallback_doms;
	cpumask_and(doms_cur[0], cpu_map, housekeeping_cpumask(HK_TYPE_DOMAIN));
	err = build_sched_domains(doms_cur[0], வெற்று, &multi_llcs);
	எனில் (!err)
		sched_cache_set(multi_llcs);

	திரும்பு err;
}

/*
 * Detach sched domains from a group of CPUs specified in cpu_map
 * These CPUs will now be attached to the NULL domain
 */
நிலையான வெறுமை detach_destroy_domains(மாறிலி கட்டமைப்பு cpumask *cpu_map)
{
	unsigned முழு cpu = cpumask_any(cpu_map);
	முழு i;

	எனில் (rcu_access_pointer(per_cpu(sd_asym_cpucapacity, cpu)))
		static_branch_dec_cpuslocked(&sched_asym_cpucapacity);

	எனில் (static_branch_unlikely(&sched_cluster_active))
		static_branch_dec_cpuslocked(&sched_cluster_active);

	rcu_read_lock();
	for_each_cpu(i, cpu_map)
		cpu_attach_domain(வெற்று, &def_வேர்_domain, i);
	rcu_read_unlock();
}

/* handle null as "default" */
நிலையான முழு dattrs_equal(கட்டமைப்பு sched_domain_attr *cur, முழு idx_cur,
			கட்டமைப்பு sched_domain_attr *new, முழு idx_new)
{
	கட்டமைப்பு sched_domain_attr tmp;

	/* Fast path: */
	எனில் (!new && !cur)
		திரும்பு 1;

	tmp = SD_ATTR_INIT;

	திரும்பு !memcmp(cur ? (cur + idx_cur) : &tmp,
			new ? (new + idx_new) : &tmp,
			அளவு(கட்டமைப்பு sched_domain_attr));
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
நிலையான வெறுமை partition_sched_domains_locked(முழு ndoms_new, cpumask_var_t doms_new[],
				    கட்டமைப்பு sched_domain_attr *dattr_new)
{
	bool __maybe_unused has_eas = false;
	bool has_multi_llcs = false, multi_llcs;
	முழு i, j, n;
	முழு new_topology;

	lockdep_assert_held(&sched_domains_mutex);

	/* Let the architecture update CPU core mappings: */
	new_topology = arch_update_cpu_topology();
	/* Trigger rebuilding CPU capacity asymmetry data */
	எனில் (new_topology)
		asym_cpu_capacity_scan();

	எனில் (!doms_new) {
		WARN_ON_ONCE(dattr_new);
		n = 0;
		doms_new = alloc_sched_domains(1);
		எனில் (doms_new) {
			n = 1;
			cpumask_and(doms_new[0], cpu_active_mask,
				    housekeeping_cpumask(HK_TYPE_DOMAIN));
		}
	} இல்லையெனில் {
		n = ndoms_new;
	}

	/* Destroy deleted domains: */
	க்கு (i = 0; i < ndoms_cur; i++) {
		க்கு (j = 0; j < n && !new_topology; j++) {
			எனில் (cpumask_equal(doms_cur[i], doms_new[j]) &&
			    dattrs_equal(dattr_cur, i, dattr_new, j))
				செல் match1;
		}
		/* No match - a current sched domain not in new doms_new[] */
		detach_destroy_domains(doms_cur[i]);
match1:
		;
	}

	n = ndoms_cur;
	எனில் (!doms_new) {
		n = 0;
		doms_new = &fallback_doms;
		cpumask_and(doms_new[0], cpu_active_mask,
			    housekeeping_cpumask(HK_TYPE_DOMAIN));
	}

	/* Build new domains: */
	க்கு (i = 0; i < ndoms_new; i++) {
		க்கு (j = 0; j < n && !new_topology; j++) {
			எனில் (cpumask_equal(doms_new[i], doms_cur[j]) &&
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
				கட்டமைப்பு sched_domain *sd;
				முழு cpu = cpumask_first(doms_cur[j]);

				guard(rcu)();
				sd = rcu_dereference(cpu_rq(cpu)->sd);
				போது (sd && sd->parent && (sd->parent->flags & SD_SHARE_LLC))
					sd = sd->parent;
				எனில் (sd && (sd->flags & SD_SHARE_LLC) && sd->parent &&
				    sd_in_multi_llcs(sd))
					has_multi_llcs = true;
				செல் match2;
			}
		}
		/* No match - add a new doms_new */
		build_sched_domains(doms_new[i], dattr_new ? dattr_new + i : வெற்று,
				    &multi_llcs);
		has_multi_llcs |= multi_llcs;
match2:
		;
	}
	sched_cache_set(has_multi_llcs);

#எனில் defined(CONFIG_ENERGY_MODEL) && defined(CONFIG_CPU_FREQ_GOV_SCHEDUTIL)
	/* Build perf domains: */
	க்கு (i = 0; i < ndoms_new; i++) {
		க்கு (j = 0; j < n && !sched_energy_update; j++) {
			எனில் (cpumask_equal(doms_new[i], doms_cur[j]) &&
			    cpu_rq(cpumask_first(doms_cur[j]))->rd->pd) {
				has_eas = true;
				செல் match3;
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
	எனில் (doms_cur != &fallback_doms)
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
வெறுமை partition_sched_domains(முழு ndoms_new, cpumask_var_t doms_new[],
			     கட்டமைப்பு sched_domain_attr *dattr_new)
{
	sched_domains_mutex_lock();
	partition_sched_domains_locked(ndoms_new, doms_new, dattr_new);
	sched_domains_mutex_unlock();
}
