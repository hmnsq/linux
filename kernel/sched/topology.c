// SPDX-License-Identifier: GPL-2.0
/*
 * Scheduler topology setup/handling methods
 */

#અન્તર્ભાવ <linux/sched/isolation.h>
#અન્તર્ભાવ <linux/sched/clock.h>
#અન્તર્ભાવ <linux/bsearch.h>
#અન્તર્ભાવ "sched.h"

DEFINE_MUTEX(sched_domains_mutex);
શૂન્ય sched_domains_mutex_lock(શૂન્ય)
{
	mutex_lock(&sched_domains_mutex);
}
શૂન્ય sched_domains_mutex_unlock(શૂન્ય)
{
	mutex_unlock(&sched_domains_mutex);
}

/* Protected by sched_domains_mutex: */
સ્થિર cpumask_var_t sched_domains_llc_id_allocmask;
સ્થિર cpumask_var_t sched_domains_tmpmask;
સ્થિર cpumask_var_t sched_domains_tmpmask2;
પૂર્ણાંક max_lid;

સ્થિર પૂર્ણાંક __init sched_debug_setup(અક્ષર *str)
{
	sched_debug_verbose = true;

	પાછા 0;
}
early_param("sched_verbose", sched_debug_setup);

#ifdef CONFIG_SCHED_SMT
સ્થિર bool sched_smt_asym_packing __read_mostly;

સ્થિર પૂર્ણાંક __init setup_sched_smt_asym_packing(અક્ષર *str)
{
	જો (strcmp(str, "on"))
		પાછા 0;

	sched_smt_asym_packing = true;
	પાછા 1;
}
__setup("sched_smt_asym_packing=", setup_sched_smt_asym_packing);
#endif

સ્થિર અંતઃરેખ bool sched_debug(શૂન્ય)
{
	પાછા sched_debug_verbose;
}

#define SD_FLAG(_name, mflags) [__##_name] = { .meta_flags = mflags, .name = #_name },
અચલ સંરચના sd_flag_debug sd_flag_debug[] = {
#અન્તર્ભાવ <linux/sched/sd_flags.h>
};
#undef SD_FLAG

સ્થિર પૂર્ણાંક sched_domain_debug_one(સંરચના sched_domain *sd, પૂર્ણાંક cpu, પૂર્ણાંક level,
				  સંરચના cpumask *groupmask)
{
	સંરચના sched_group *group = sd->groups;
	unsigned long flags = sd->flags;
	unsigned પૂર્ણાંક idx;

	cpumask_clear(groupmask);

	printk(KERN_DEBUG "%*s domain-%d: ", level, "", level);
	printk(KERN_CONT "span=%*pbl level=%s\n",
	       cpumask_pr_args(sched_domain_span(sd)), sd->name);

	જો (!cpumask_test_cpu(cpu, sched_domain_span(sd))) {
		printk(KERN_ERR "ERROR: domain->span does not contain CPU%d\n", cpu);
	}
	જો (group && !cpumask_test_cpu(cpu, sched_group_span(group))) {
		printk(KERN_ERR "ERROR: domain->groups does not contain CPU%d\n", cpu);
	}

	for_each_set_bit(idx, &flags, __SD_FLAG_CNT) {
		unsigned પૂર્ણાંક flag = BIT(idx);
		unsigned પૂર્ણાંક meta_flags = sd_flag_debug[idx].meta_flags;

		જો ((meta_flags & SDF_SHARED_CHILD) && sd->child &&
		    !(sd->child->flags & flag))
			printk(KERN_ERR "ERROR: flag %s set here but not in child\n",
			       sd_flag_debug[idx].name);

		જો ((meta_flags & SDF_SHARED_PARENT) && sd->parent &&
		    !(sd->parent->flags & flag))
			printk(KERN_ERR "ERROR: flag %s set here but not in parent\n",
			       sd_flag_debug[idx].name);
	}

	printk(KERN_DEBUG "%*s groups:", level + 1, "");
	કરો {
		જો (!group) {
			printk("\n");
			printk(KERN_ERR "ERROR: group is NULL\n");
			તોડો;
		}

		જો (cpumask_empty(sched_group_span(group))) {
			printk(KERN_CONT "\n");
			printk(KERN_ERR "ERROR: empty group\n");
			તોડો;
		}

		જો (!(sd->flags & SD_NUMA) &&
		    cpumask_intersects(groupmask, sched_group_span(group))) {
			printk(KERN_CONT "\n");
			printk(KERN_ERR "ERROR: repeated CPUs\n");
			તોડો;
		}

		cpumask_or(groupmask, groupmask, sched_group_span(group));

		printk(KERN_CONT " %d:{ span=%*pbl",
				group->sgc->id,
				cpumask_pr_args(sched_group_span(group)));

		જો ((sd->flags & SD_NUMA) &&
		    !cpumask_equal(group_balance_mask(group), sched_group_span(group))) {
			printk(KERN_CONT " mask=%*pbl",
				cpumask_pr_args(group_balance_mask(group)));
		}

		જો (group->sgc->capacity != SCHED_CAPACITY_SCALE)
			printk(KERN_CONT " cap=%lu", group->sgc->capacity);

		જો (group == sd->groups && sd->child &&
		    !cpumask_equal(sched_domain_span(sd->child),
				   sched_group_span(group))) {
			printk(KERN_ERR "ERROR: domain->groups does not match domain->child\n");
		}

		printk(KERN_CONT " }");

		group = group->next;

		જો (group != sd->groups)
			printk(KERN_CONT ",");

	} જ્યારે (group != sd->groups);
	printk(KERN_CONT "\n");

	જો (!cpumask_equal(sched_domain_span(sd), groupmask))
		printk(KERN_ERR "ERROR: groups don't span domain->span\n");

	જો (sd->parent &&
	    !cpumask_subset(groupmask, sched_domain_span(sd->parent)))
		printk(KERN_ERR "ERROR: parent span is not a superset of domain->span\n");
	પાછા 0;
}

સ્થિર શૂન્ય sched_domain_debug(સંરચના sched_domain *sd, પૂર્ણાંક cpu)
{
	પૂર્ણાંક level = 0;

	જો (!sched_debug_verbose)
		પાછા;

	જો (!sd) {
		printk(KERN_DEBUG "CPU%d attaching NULL sched-domain.\n", cpu);
		પાછા;
	}

	printk(KERN_DEBUG "CPU%d attaching sched-domain(s):\n", cpu);

	માટે (;;) {
		જો (sched_domain_debug_one(sd, cpu, level, sched_domains_tmpmask))
			તોડો;
		level++;
		sd = sd->parent;
		જો (!sd)
			તોડો;
	}
}

/* Generate a mask of SD flags with the SDF_NEEDS_GROUPS metaflag */
#define SD_FLAG(name, mflags) (name * !!((mflags) & SDF_NEEDS_GROUPS)) |
સ્થિર અચલ unsigned પૂર્ણાંક SD_DEGENERATE_GROUPS_MASK =
#અન્તર્ભાવ <linux/sched/sd_flags.h>
0;
#undef SD_FLAG

સ્થિર પૂર્ણાંક sd_degenerate(સંરચના sched_domain *sd)
{
	જો (cpumask_weight(sched_domain_span(sd)) == 1)
		પાછા 1;

	/* Following flags need at least 2 groups */
	જો ((sd->flags & SD_DEGENERATE_GROUPS_MASK) &&
	    (sd->groups != sd->groups->next))
		પાછા 0;

	/* Following flags don't use groups */
	જો (sd->flags & (SD_WAKE_AFFINE))
		પાછા 0;

	પાછા 1;
}

સ્થિર પૂર્ણાંક
sd_parent_degenerate(સંરચના sched_domain *sd, સંરચના sched_domain *parent)
{
	unsigned long cflags = sd->flags, pflags = parent->flags;

	જો (sd_degenerate(parent))
		પાછા 1;

	જો (!cpumask_equal(sched_domain_span(sd), sched_domain_span(parent)))
		પાછા 0;

	/* Flags needing groups don't count if only 1 group in parent */
	જો (parent->groups == parent->groups->next)
		pflags &= ~SD_DEGENERATE_GROUPS_MASK;

	જો (~cflags & pflags)
		પાછા 0;

	પાછા 1;
}

#જો defined(CONFIG_ENERGY_MODEL) && defined(CONFIG_CPU_FREQ_GOV_SCHEDUTIL)
DEFINE_STATIC_ચાવી_FALSE(sched_energy_present);
સ્થિર unsigned પૂર્ણાંક sysctl_sched_energy_aware = 1;
સ્થિર DEFINE_MUTEX(sched_energy_mutex);
સ્થિર bool sched_energy_update;

સ્થિર bool sched_is_eas_possible(અચલ સંરચના cpumask *cpu_mask)
{
	bool any_asym_capacity = false;
	પૂર્ણાંક i;

	/* EAS is enabled for asymmetric CPU capacity topologies. */
	for_each_cpu(i, cpu_mask) {
		જો (rcu_access_pointer(per_cpu(sd_asym_cpucapacity, i))) {
			any_asym_capacity = true;
			તોડો;
		}
	}
	જો (!any_asym_capacity) {
		જો (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS, CPUs do not have asymmetric capacities\n",
				cpumask_pr_args(cpu_mask));
		}
		પાછા false;
	}

	/* EAS definitely does *not* handle SMT */
	જો (sched_smt_active()) {
		જો (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS, SMT is not supported\n",
				cpumask_pr_args(cpu_mask));
		}
		પાછા false;
	}

	જો (!arch_scale_freq_invariant()) {
		જો (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS: frequency-invariant load tracking not yet supported",
				cpumask_pr_args(cpu_mask));
		}
		પાછા false;
	}

	જો (!cpufreq_ready_for_eas(cpu_mask)) {
		જો (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS: cpufreq is not ready\n",
				cpumask_pr_args(cpu_mask));
		}
		પાછા false;
	}

	પાછા true;
}

શૂન્ય rebuild_sched_domains_energy(શૂન્ય)
{
	mutex_lock(&sched_energy_mutex);
	sched_energy_update = true;
	rebuild_sched_domains();
	sched_energy_update = false;
	mutex_unlock(&sched_energy_mutex);
}

#ifdef CONFIG_SYSCTL
સ્થિર પૂર્ણાંક sched_energy_aware_handler(અચલ સંરચના ctl_table *table, પૂર્ણાંક લિખ,
		શૂન્ય *buffer, size_t *lenp, loff_t *ppos)
{
	પૂર્ણાંક ret;

	જો (લિખ && !capable(CAP_SYS_ADMIN))
		પાછા -EPERM;

	જો (!sched_is_eas_possible(cpu_active_mask)) {
		જો (લિખ) {
			પાછા -EOPNOTSUPP;
		} નહીંતર {
			*lenp = 0;
			પાછા 0;
		}
	}

	ret = proc_dointvec_minmax(table, લિખ, buffer, lenp, ppos);
	જો (!ret && લિખ) {
		જો (sysctl_sched_energy_aware != sched_energy_enabled())
			rebuild_sched_domains_energy();
	}

	પાછા ret;
}

સ્થિર અચલ સંરચના ctl_table sched_energy_aware_sysctls[] = {
	{
		.procname       = "sched_energy_aware",
		.data           = &sysctl_sched_energy_aware,
		.maxlen         = કદ(unsigned પૂર્ણાંક),
		.mode           = 0644,
		.proc_handler   = sched_energy_aware_handler,
		.extra1         = SYSCTL_ZERO,
		.extra2         = SYSCTL_ONE,
	},
};

સ્થિર પૂર્ણાંક __init sched_energy_aware_sysctl_init(શૂન્ય)
{
	register_sysctl_init("kernel", sched_energy_aware_sysctls);
	પાછા 0;
}

late_initcall(sched_energy_aware_sysctl_init);
#endif /* CONFIG_SYSCTL */

સ્થિર શૂન્ય free_pd(સંરચના perf_domain *pd)
{
	સંરચના perf_domain *tmp;

	જ્યારે (pd) {
		tmp = pd->next;
		kfree(pd);
		pd = tmp;
	}
}

સ્થિર સંરચના perf_domain *find_pd(સંરચના perf_domain *pd, પૂર્ણાંક cpu)
{
	જ્યારે (pd) {
		જો (cpumask_test_cpu(cpu, perf_domain_span(pd)))
			પાછા pd;
		pd = pd->next;
	}

	પાછા ખાલી;
}

સ્થિર સંરચના perf_domain *pd_init(પૂર્ણાંક cpu)
{
	સંરચના em_perf_domain *obj = em_cpu_get(cpu);
	સંરચના perf_domain *pd;

	જો (!obj) {
		જો (sched_debug())
			pr_info("%s: no EM found for CPU%d\n", __func__, cpu);
		પાછા ખાલી;
	}

	pd = kzalloc_obj(*pd);
	જો (!pd)
		પાછા ખાલી;
	pd->em_pd = obj;

	પાછા pd;
}

સ્થિર શૂન્ય perf_domain_debug(અચલ સંરચના cpumask *cpu_map,
						સંરચના perf_domain *pd)
{
	જો (!sched_debug() || !pd)
		પાછા;

	printk(KERN_DEBUG "root_domain %*pbl:", cpumask_pr_args(cpu_map));

	જ્યારે (pd) {
		printk(KERN_CONT " pd%d:{ cpus=%*pbl nr_pstate=%d }",
				cpumask_first(perf_domain_span(pd)),
				cpumask_pr_args(perf_domain_span(pd)),
				em_pd_nr_perf_states(pd->em_pd));
		pd = pd->next;
	}

	printk(KERN_CONT "\n");
}

સ્થિર શૂન્ય destroy_perf_domain_rcu(સંરચના rcu_head *rp)
{
	સંરચના perf_domain *pd;

	pd = container_of(rp, સંરચના perf_domain, rcu);
	free_pd(pd);
}

સ્થિર શૂન્ય sched_energy_set(bool has_eas)
{
	જો (!has_eas && sched_energy_enabled()) {
		જો (sched_debug())
			pr_info("%s: stopping EAS\n", __func__);
		static_branch_disable_cpuslocked(&sched_energy_present);
	} નહીંતર જો (has_eas && !sched_energy_enabled()) {
		જો (sched_debug())
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
સ્થિર bool build_perf_domains(અચલ સંરચના cpumask *cpu_map)
{
	પૂર્ણાંક i;
	સંરચના perf_domain *pd = ખાલી, *tmp;
	પૂર્ણાંક cpu = cpumask_first(cpu_map);
	સંરચના મૂળ_domain *rd = cpu_rq(cpu)->rd;

	જો (!sysctl_sched_energy_aware)
		જાઓ મુક્ત;

	જો (!sched_is_eas_possible(cpu_map))
		જાઓ મુક્ત;

	for_each_cpu(i, cpu_map) {
		/* Skip already covered CPUs. */
		જો (find_pd(pd, i))
			ચાલુ;

		/* Create the new pd and add it to the local list. */
		tmp = pd_init(i);
		જો (!tmp)
			જાઓ મુક્ત;
		tmp->next = pd;
		pd = tmp;
	}

	perf_domain_debug(cpu_map, pd);

	/* Attach the new list of performance domains to the root domain. */
	tmp = rd->pd;
	rcu_assign_pointer(rd->pd, pd);
	જો (tmp)
		call_rcu(&tmp->rcu, destroy_perf_domain_rcu);

	પાછા !!pd;

મુક્ત:
	free_pd(pd);
	tmp = rd->pd;
	rcu_assign_pointer(rd->pd, ખાલી);
	જો (tmp)
		call_rcu(&tmp->rcu, destroy_perf_domain_rcu);

	પાછા false;
}
#નહીંતર /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL): */
સ્થિર શૂન્ય free_pd(સંરચના perf_domain *pd) { }
#endif /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL) */

સ્થિર શૂન્ય free_rootdomain(સંરચના rcu_head *rcu)
{
	સંરચના મૂળ_domain *rd = container_of(rcu, સંરચના મૂળ_domain, rcu);

	cpupri_cleanup(&rd->cpupri);
	cpudl_cleanup(&rd->cpudl);
	free_cpumask_var(rd->dlo_mask);
	free_cpumask_var(rd->rto_mask);
	free_cpumask_var(rd->online);
	free_cpumask_var(rd->span);
	free_pd(rd->pd);
	kfree(rd);
}

શૂન્ય rq_attach_મૂળ(સંરચના rq *rq, સંરચના મૂળ_domain *rd)
{
	સંરચના મૂળ_domain *old_rd = ખાલી;
	સંરચના rq_flags rf;

	rq_lock_irqsave(rq, &rf);

	જો (rq->rd) {
		old_rd = rq->rd;

		જો (cpumask_test_cpu(rq->cpu, old_rd->online))
			set_rq_offline(rq);

		cpumask_clear_cpu(rq->cpu, old_rd->span);

		/*
		 * If we don't want to free the old_rd yet then
		 * set old_rd to NULL to skip the freeing later
		 * in this function:
		 */
		જો (!atomic_dec_and_test(&old_rd->refcount))
			old_rd = ખાલી;
	}

	atomic_inc(&rd->refcount);
	rq->rd = rd;

	cpumask_set_cpu(rq->cpu, rd->span);
	જો (cpumask_test_cpu(rq->cpu, cpu_active_mask))
		set_rq_online(rq);

	/*
	 * Because the rq is not a task, dl_add_task_root_domain() did not
	 * move the fair server bw to the rd if it already started.
	 * Add it now.
	 */
	જો (rq->fair_server.dl_server)
		__dl_server_attach_root(&rq->fair_server, rq);

#ifdef CONFIG_SCHED_CLASS_EXT
	જો (rq->ext_server.dl_server)
		__dl_server_attach_root(&rq->ext_server, rq);
#endif

	rq_unlock_irqrestore(rq, &rf);

	જો (old_rd)
		call_rcu(&old_rd->rcu, free_rootdomain);
}

શૂન્ય sched_get_rd(સંરચના મૂળ_domain *rd)
{
	atomic_inc(&rd->refcount);
}

શૂન્ય sched_put_rd(સંરચના મૂળ_domain *rd)
{
	જો (!atomic_dec_and_test(&rd->refcount))
		પાછા;

	call_rcu(&rd->rcu, free_rootdomain);
}

સ્થિર પૂર્ણાંક init_rootdomain(સંરચના મૂળ_domain *rd)
{
	જો (!zalloc_cpumask_var(&rd->span, GFP_KERNEL))
		જાઓ out;
	જો (!zalloc_cpumask_var(&rd->online, GFP_KERNEL))
		જાઓ free_span;
	જો (!zalloc_cpumask_var(&rd->dlo_mask, GFP_KERNEL))
		જાઓ free_online;
	જો (!zalloc_cpumask_var(&rd->rto_mask, GFP_KERNEL))
		જાઓ free_dlo_mask;

#ifdef HAVE_RT_ધકેલો_IPI
	rd->rto_cpu = -1;
	raw_spin_lock_init(&rd->rto_lock);
	rd->rto_ધકેલો_work = IRQ_WORK_INIT_HARD(rto_ધકેલો_irq_work_func);
#endif

	rd->visit_cookie = 0;
	init_dl_bw(&rd->dl_bw);
	જો (cpudl_init(&rd->cpudl) != 0)
		જાઓ free_rto_mask;

	જો (cpupri_init(&rd->cpupri) != 0)
		જાઓ free_cpudl;
	પાછા 0;

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
	પાછા -ENOMEM;
}

/*
 * By default the system creates a single root-domain with all CPUs as
 * members (mimicking the global state we have today).
 */
સંરચના મૂળ_domain def_મૂળ_domain;

શૂન્ય __init init_defrootdomain(શૂન્ય)
{
	init_rootdomain(&def_મૂળ_domain);

	atomic_set(&def_મૂળ_domain.refcount, 1);
}

સ્થિર સંરચના મૂળ_domain *alloc_rootdomain(શૂન્ય)
{
	સંરચના મૂળ_domain *rd;

	rd = kzalloc_obj(*rd);
	જો (!rd)
		પાછા ખાલી;

	જો (init_rootdomain(rd) != 0) {
		kfree(rd);
		પાછા ખાલી;
	}

	પાછા rd;
}

સ્થિર શૂન્ય free_sched_groups(સંરચના sched_group *sg, પૂર્ણાંક free_sgc)
{
	સંરચના sched_group *tmp, *first;

	જો (!sg)
		પાછા;

	first = sg;
	કરો {
		tmp = sg->next;

		જો (free_sgc && atomic_dec_and_test(&sg->sgc->ref))
			kfree(sg->sgc);

		જો (atomic_dec_and_test(&sg->ref))
			kfree(sg);
		sg = tmp;
	} જ્યારે (sg != first);
}

સ્થિર શૂન્ય free_sched_domain_shared(સંરચના sched_domain_shared *sds)
{
	જો (sds && atomic_dec_and_test(&sds->ref))
		kfree(sds);
}

સ્થિર શૂન્ય destroy_sched_domain(સંરચના sched_domain *sd)
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

સ્થિર શૂન્ય destroy_sched_domains_rcu(સંરચના rcu_head *rcu)
{
	સંરચના sched_domain *sd = container_of(rcu, સંરચના sched_domain, rcu);

	જ્યારે (sd) {
		સંરચના sched_domain *parent = sd->parent;
		destroy_sched_domain(sd);
		sd = parent;
	}
}

સ્થિર શૂન્ય destroy_sched_domains(સંરચના sched_domain *sd)
{
	જો (sd)
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
DEFINE_PER_CPU(સંરચના sched_domain __rcu *, sd_llc);
DEFINE_PER_CPU(પૂર્ણાંક, sd_llc_size);
DEFINE_PER_CPU(પૂર્ણાંક, sd_llc_id) = -1;
DEFINE_PER_CPU(પૂર્ણાંક, sd_share_id);
DEFINE_PER_CPU(સંરચના sched_domain_shared __rcu *, sd_llc_shared);
DEFINE_PER_CPU(સંરચના sched_domain_shared __rcu *, sd_balance_shared);
DEFINE_PER_CPU(સંરચના sched_domain __rcu *, sd_numa);
DEFINE_PER_CPU(સંરચના sched_domain __rcu *, sd_asym_packing);
DEFINE_PER_CPU(સંરચના sched_domain __rcu *, sd_asym_cpucapacity);

DEFINE_STATIC_ચાવી_FALSE(sched_asym_cpucapacity);
DEFINE_STATIC_ચાવી_FALSE(sched_cluster_active);

સ્થિર શૂન્ય update_ટોચ_cache_domain(પૂર્ણાંક cpu)
{
	સંરચના sched_domain_shared *sds = ખાલી;
	સંરચના sched_domain *sd;
	પૂર્ણાંક id = cpu;
	પૂર્ણાંક size = 1;

	sd = highest_flag_domain(cpu, SD_SHARE_LLC);
	જો (sd) {
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
	જો (sd)
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
	જો (sd && sd->shared)
		sds = sd->shared;

	rcu_assign_pointer(per_cpu(sd_asym_cpucapacity, cpu), sd);
	rcu_assign_pointer(per_cpu(sd_balance_shared, cpu), sds);
}

/*
 * Attach the domain 'sd' to 'cpu' as its base domain. Callers must
 * hold the hotplug lock.
 */
સ્થિર શૂન્ય
cpu_attach_domain(સંરચના sched_domain *sd, સંરચના મૂળ_domain *rd, પૂર્ણાંક cpu)
{
	સંરચના rq *rq = cpu_rq(cpu);
	સંરચના sched_domain *tmp;

	/* Remove the sched domains which do not contribute to scheduling. */
	માટે (tmp = sd; tmp; ) {
		સંરચના sched_domain *parent = tmp->parent;
		જો (!parent)
			તોડો;

		જો (sd_parent_degenerate(tmp, parent)) {
			tmp->parent = parent->parent;

			/* Pick reference to parent->shared. */
			જો (parent->shared) {
				/*
				 * It is safe to free a sd->shared that
				 * has not been published yet. If a
				 * sd->shared was published, the refcount
				 * will end up being non-zero and it will
				 * not be freed here.
				 */
				free_sched_domain_shared(tmp->shared);
				tmp->shared = parent->shared;
				parent->shared = ખાલી;
			}

			જો (parent->parent) {
				parent->parent->child = tmp;
				parent->parent->groups->flags = tmp->flags;
			}

			/*
			 * Transfer SD_PREFER_SIBLING down in case of a
			 * degenerate parent; the spans match for this
			 * so the property transfers.
			 */
			જો (parent->flags & SD_PREFER_SIBLING)
				tmp->flags |= SD_PREFER_SIBLING;
			destroy_sched_domain(parent);
		} નહીંતર
			tmp = tmp->parent;
	}

	જો (sd && sd_degenerate(sd)) {
		tmp = sd;
		sd = sd->parent;

		જો (sd) {
			સંરચના sched_group *sg = sd->groups;

#ifdef CONFIG_SCHED_CACHE
			/* move buffer to parent as child is being destroyed */
			sd->llc_counts = tmp->llc_counts;
			sd->llc_max = tmp->llc_max;
			sd->llc_bytes = tmp->llc_bytes;
			/* make sure destroy_sched_domain() does not free it */
			tmp->llc_counts = ખાલી;
			tmp->llc_max = 0;
			tmp->llc_bytes = 0;
#endif
			/*
			 * sched groups hold the flags of the child sched
			 * domain for convenience. Clear such flags since
			 * the child is being destroyed.
			 */
			કરો {
				sg->flags = 0;
			} જ્યારે (sg != sd->groups);

			sd->child = ખાલી;
		}

		destroy_sched_domain(tmp);
	}

	sched_domain_debug(sd, cpu);

	rq_attach_મૂળ(rq, rd);
	tmp = rq->sd;
	rcu_assign_pointer(rq->sd, sd);
	dirty_sched_domain_sysctl(cpu);
	destroy_sched_domains(tmp);

	update_ટોચ_cache_domain(cpu);
}

સંરચના s_data {
	સંરચના sched_domain_shared * __percpu *sds;
	સંરચના sched_domain * __percpu *sd;
	સંરચના મૂળ_domain	*rd;
};

ગણના s_alloc {
	sa_rootdomain,
	sa_sd,
	sa_sd_shared,
	sa_sd_storage,
	sa_none,
};

#ifdef CONFIG_SCHED_CACHE
/* hardware support for cache aware scheduling */
DEFINE_STATIC_ચાવી_FALSE(sched_cache_present);
/*
 * Indicator of whether cache aware scheduling
 * is active, used by the scheduler.
 */
DEFINE_STATIC_ચાવી_FALSE(sched_cache_active);
/* user wants cache aware scheduling [0 or 1] */
પૂર્ણાંક sysctl_sched_cache_user = 1;

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
સ્થિર unsigned long get_effective_llc_bytes(પૂર્ણાંક cpu,
					     સંરચના sched_domain *sd)
{
	સંરચના cacheinfo *ci;
	unsigned પૂર્ણાંક hw_weight;

	ci = get_cpu_cacheinfo_llc(cpu);
	જો (!ci)
		પાછા 0;

	hw_weight = cpumask_weight(&ci->shared_cpu_map);
	જો (!hw_weight)
		પાછા 0;

	પાછા div_u64((u64)ci->size * sd->span_weight, hw_weight);
}

સ્થિર bool alloc_sd_llc(અચલ સંરચના cpumask *cpu_map,
			 સંરચના s_data *d)
{
	સંરચના sched_domain *sd, *ટોચ_llc, *parent;
	unsigned પૂર્ણાંક *p;
	પૂર્ણાંક i;

	for_each_cpu(i, cpu_map) {
		sd = *per_cpu_ptr(d->sd, i);
		જો (!sd)
			જાઓ err;

		p = kcalloc_ગાંઠ(max_lid + 1, કદ(unsigned પૂર્ણાંક),
				 GFP_KERNEL, cpu_to_ગાંઠ(i));
		જો (!p)
			જાઓ err;

		ટોચ_llc = sd;
		/*
		 * Find the topmost SD_SHARE_LLC domain.
		 * Not yet attached to the CPU, so per_cpu(sd_llc, i)
		 * can not be used.
		 */
		જ્યારે ((parent = rcu_dereference_protected(ટોચ_llc->parent, true)) &&
		       (parent->flags & SD_SHARE_LLC))
			ટોચ_llc = parent;

		જો (ટોચ_llc->flags & SD_SHARE_LLC) {
			sd->llc_max = max_lid + 1;
			sd->llc_counts = p;
			sd->llc_bytes = get_effective_llc_bytes(i, ટોચ_llc);
		} નહીંતર {
			/* avoid memory leak */
			kfree(p);
		}
	}

	પાછા true;
err:
	for_each_cpu(i, cpu_map) {
		sd = *per_cpu_ptr(d->sd, i);
		જો (sd) {
			kfree(sd->llc_counts);
			sd->llc_counts = ખાલી;
			sd->llc_max = 0;
			sd->llc_bytes = 0;
		}
	}

	પાછા false;
}

/*
 * Enable/disable cache aware scheduling according to
 * user input and the presence of hardware support.
 */
સ્થિર શૂન્ય _sched_cache_active_set(શૂન્ય)
{
	lockdep_assert_cpus_held();
	lockdep_assert_held(&sched_domains_mutex);

	/* hardware does not support */
	જો (!static_branch_likely(&sched_cache_present)) {
		static_branch_disable_cpuslocked(&sched_cache_active);
		જો (sched_debug())
			pr_info("%s: cache aware scheduling not supported on this platform\n", __func__);
		પાછા;
	}

	/*
	 * user wants it or not ?
	 * TBD: read before writing the static key.
	 * It is not in the critical path, leave as-is
	 * for now.
	 */
	જો (sysctl_sched_cache_user) {
		static_branch_enable_cpuslocked(&sched_cache_active);
		જો (sched_debug())
			pr_info("%s: enabling cache aware scheduling\n", __func__);
	} નહીંતર {
		static_branch_disable_cpuslocked(&sched_cache_active);
		જો (sched_debug())
			pr_info("%s: disabling cache aware scheduling\n", __func__);
	}
}

/* used by debugfs */
શૂન્ય sched_cache_active_set(શૂન્ય)
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
શૂન્ય sched_update_llc_bytes(અચલ સંરચના cpumask *cpus)
{
	સંરચના sched_domain *sd, *sdp;
	unsigned પૂર્ણાંક i;

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
		જો (!sdp)
			ચાલુ;

		sd = rcu_dereference_sched_domain(cpu_rq(i)->sd);
		જો (sd)
			sd->llc_bytes = get_effective_llc_bytes(i, sdp);
	}

	sched_domains_mutex_unlock();
}

સ્થિર શૂન્ય sched_cache_set(bool has_multi_llcs)
{
	/*
	 * TBD: check before writing to it. sched domain rebuild
	 * is not in the critical path, leave as-is for now.
	 */
	જો (has_multi_llcs)
		static_branch_enable_cpuslocked(&sched_cache_present);
	નહીંતર
		static_branch_disable_cpuslocked(&sched_cache_present);

	_sched_cache_active_set();
}
#નહીંતર
સ્થિર bool alloc_sd_llc(અચલ સંરચના cpumask *cpu_map,
			 સંરચના s_data *d)
{
	પાછા false;
}
સ્થિર અંતઃરેખ શૂન્ય sched_cache_set(bool has_multi_llcs) { }
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
સ્થિર bool sd_in_multi_llcs(સંરચના sched_domain *sd)
{
	સંરચના sched_domain *sdp = sd->parent;

	/* it does not make sense to aggregate to 1 CPU */
	જો (sd->span_weight == 1)
		પાછા false;

	જ્યારે (sdp && sdp->span_weight == sd->span_weight)
		sdp = sdp->parent;

	પાછા !!sdp;
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
પૂર્ણાંક group_balance_cpu(સંરચના sched_group *sg)
{
	પાછા cpumask_first(group_balance_mask(sg));
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
સ્થિર શૂન્ય
build_balance_mask(સંરચના sched_domain *sd, સંરચના sched_group *sg, સંરચના cpumask *mask)
{
	અચલ સંરચના cpumask *sg_span = sched_group_span(sg);
	સંરચના sd_data *sdd = sd->ખાનગી;
	સંરચના sched_domain *sibling;
	પૂર્ણાંક i;

	cpumask_clear(mask);

	for_each_cpu(i, sg_span) {
		sibling = *per_cpu_ptr(sdd->sd, i);

		/*
		 * Can happen in the asymmetric case, where these siblings are
		 * unused. The mask will not be empty because those CPUs that
		 * do have the top domain _should_ span the domain.
		 */
		જો (!sibling->child)
			ચાલુ;

		/* If we would not end up here, we can't continue from here */
		જો (!cpumask_equal(sg_span, sched_domain_span(sibling->child)))
			ચાલુ;

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
સ્થિર સંરચના sched_group *
build_group_from_child_sched_domain(સંરચના sched_domain *sd, પૂર્ણાંક cpu)
{
	સંરચના sched_group *sg;
	સંરચના cpumask *sg_span;

	sg = kzalloc_ગાંઠ(કદ(સંરચના sched_group) + cpumask_size(),
			GFP_KERNEL, cpu_to_ગાંઠ(cpu));

	જો (!sg)
		પાછા ખાલી;

	sg_span = sched_group_span(sg);
	જો (sd->child) {
		cpumask_copy(sg_span, sched_domain_span(sd->child));
		sg->flags = sd->child->flags;
	} નહીંતર {
		cpumask_copy(sg_span, sched_domain_span(sd));
	}

	atomic_inc(&sg->ref);
	પાછા sg;
}

સ્થિર શૂન્ય init_overlap_sched_group(સંરચના sched_domain *sd,
				     સંરચના sched_group *sg)
{
	સંરચના cpumask *mask = sched_domains_tmpmask2;
	સંરચના sd_data *sdd = sd->ખાનગી;
	સંરચના cpumask *sg_span;
	પૂર્ણાંક cpu;

	build_balance_mask(sd, sg, mask);
	cpu = cpumask_first(mask);

	sg->sgc = *per_cpu_ptr(sdd->sgc, cpu);
	જો (atomic_inc_return(&sg->sgc->ref) == 1)
		cpumask_copy(group_balance_mask(sg), mask);
	નહીંતર
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

સ્થિર સંરચના sched_domain *
find_descended_sibling(સંરચના sched_domain *sd, સંરચના sched_domain *sibling)
{
	/*
	 * The proper descendant would be the one whose child won't span out
	 * of sd
	 */
	જ્યારે (sibling->child &&
	       !cpumask_subset(sched_domain_span(sibling->child),
			       sched_domain_span(sd)))
		sibling = sibling->child;

	/*
	 * As we are referencing sgc across different topology level, we need
	 * to go down to skip those sched_domains which don't contribute to
	 * scheduling because they will be degenerated in cpu_attach_domain
	 */
	જ્યારે (sibling->child &&
	       cpumask_equal(sched_domain_span(sibling->child),
			     sched_domain_span(sibling)))
		sibling = sibling->child;

	પાછા sibling;
}

સ્થિર પૂર્ણાંક
build_overlap_sched_groups(સંરચના sched_domain *sd, પૂર્ણાંક cpu)
{
	સંરચના sched_group *first = ખાલી, *last = ખાલી, *sg;
	અચલ સંરચના cpumask *span = sched_domain_span(sd);
	સંરચના cpumask *covered = sched_domains_tmpmask;
	સંરચના sd_data *sdd = sd->ખાનગી;
	સંરચના sched_domain *sibling;
	પૂર્ણાંક i;

	cpumask_clear(covered);

	for_each_cpu_wrap(i, span, cpu) {
		સંરચના cpumask *sg_span;

		જો (cpumask_test_cpu(i, covered))
			ચાલુ;

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
		જો (!cpumask_test_cpu(i, sched_domain_span(sibling)))
			ચાલુ;

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
		જો (sibling->child &&
		    !cpumask_subset(sched_domain_span(sibling->child), span))
			sibling = find_descended_sibling(sd, sibling);

		sg = build_group_from_child_sched_domain(sibling, cpu);
		જો (!sg)
			જાઓ fail;

		sg_span = sched_group_span(sg);
		cpumask_or(covered, covered, sg_span);

		init_overlap_sched_group(sibling, sg);

		જો (!first)
			first = sg;
		જો (last)
			last->next = sg;
		last = sg;
		last->next = first;
	}
	sd->groups = first;

	પાછા 0;

fail:
	free_sched_groups(first, 0);

	પાછા -ENOMEM;
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

સ્થિર સંરચના sched_group *get_group(પૂર્ણાંક cpu, સંરચના sd_data *sdd)
{
	સંરચના sched_domain *sd = *per_cpu_ptr(sdd->sd, cpu);
	સંરચના sched_domain *child = sd->child;
	સંરચના sched_group *sg;
	bool already_visited;

	જો (child)
		cpu = cpumask_first(sched_domain_span(child));

	sg = *per_cpu_ptr(sdd->sg, cpu);
	sg->sgc = *per_cpu_ptr(sdd->sgc, cpu);

	/* Increase refcounts for claim_allocations: */
	already_visited = atomic_inc_return(&sg->ref) > 1;
	/* sgc visits should follow a similar trend as sg */
	WARN_ON(already_visited != (atomic_inc_return(&sg->sgc->ref) > 1));

	/* If we have already visited that group, it's already initialized. */
	જો (already_visited)
		પાછા sg;

	જો (child) {
		cpumask_copy(sched_group_span(sg), sched_domain_span(child));
		cpumask_copy(group_balance_mask(sg), sched_group_span(sg));
		sg->flags = child->flags;
	} નહીંતર {
		cpumask_set_cpu(cpu, sched_group_span(sg));
		cpumask_set_cpu(cpu, group_balance_mask(sg));
	}

	sg->sgc->capacity = SCHED_CAPACITY_SCALE * cpumask_weight(sched_group_span(sg));
	sg->sgc->min_capacity = SCHED_CAPACITY_SCALE;
	sg->sgc->max_capacity = SCHED_CAPACITY_SCALE;

	પાછા sg;
}

/*
 * build_sched_groups will build a circular linked list of the groups
 * covered by the given span, will set each group's ->cpumask correctly,
 * and will initialize their ->sgc.
 *
 * Assumes the sched_domain tree is fully constructed
 */
સ્થિર પૂર્ણાંક
build_sched_groups(સંરચના sched_domain *sd, પૂર્ણાંક cpu)
{
	સંરચના sched_group *first = ખાલી, *last = ખાલી;
	સંરચના sd_data *sdd = sd->ખાનગી;
	અચલ સંરચના cpumask *span = sched_domain_span(sd);
	સંરચના cpumask *covered;
	પૂર્ણાંક i;

	lockdep_assert_held(&sched_domains_mutex);
	covered = sched_domains_tmpmask;

	cpumask_clear(covered);

	for_each_cpu_wrap(i, span, cpu) {
		સંરચના sched_group *sg;

		જો (cpumask_test_cpu(i, covered))
			ચાલુ;

		sg = get_group(i, sdd);

		cpumask_or(covered, covered, sched_group_span(sg));

		જો (!first)
			first = sg;
		જો (last)
			last->next = sg;
		last = sg;
	}
	last->next = first;
	sd->groups = first;

	પાછા 0;
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
સ્થિર શૂન્ય init_sched_groups_capacity(પૂર્ણાંક cpu, સંરચના sched_domain *sd)
{
	સંરચના sched_group *sg = sd->groups;
	સંરચના cpumask *mask = sched_domains_tmpmask2;

	WARN_ON(!sg);

	કરો {
		પૂર્ણાંક cpu, cores = 0, max_cpu = -1;

		sg->group_weight = cpumask_weight(sched_group_span(sg));

		cpumask_copy(mask, sched_group_span(sg));
		for_each_cpu(cpu, mask) {
			cores++;
			cpumask_andnot(mask, mask, cpu_smt_mask(cpu));
		}
		sg->cores = cores;

		જો (!(sd->flags & SD_ASYM_PACKING))
			જાઓ next;

		for_each_cpu(cpu, sched_group_span(sg)) {
			જો (max_cpu < 0)
				max_cpu = cpu;
			નહીંતર જો (sched_asym_prefer(cpu, max_cpu))
				max_cpu = cpu;
		}
		sg->asym_prefer_cpu = max_cpu;

next:
		sg = sg->next;
	} જ્યારે (sg != sd->groups);

	જો (cpu != group_balance_cpu(sg))
		પાછા;

	update_group_capacity(sd, cpu);
}

/* Update the "asym_prefer_cpu" when arch_asym_cpu_priority() changes. */
શૂન્ય sched_update_asym_prefer_cpu(પૂર્ણાંક cpu, પૂર્ણાંક old_prio, પૂર્ણાંક new_prio)
{
	પૂર્ણાંક asym_prefer_cpu = cpu;
	સંરચના sched_domain *sd;

	guard(rcu)();

	for_each_domain(cpu, sd) {
		સંરચના sched_group *sg;
		પૂર્ણાંક group_cpu;

		જો (!(sd->flags & SD_ASYM_PACKING))
			ચાલુ;

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
		જો (cpu != sg->asym_prefer_cpu) {
			/*
			 * Since the parent is a superset of the current group,
			 * if the cpu is not the "asym_prefer_cpu" at the
			 * current level, it cannot be the preferred CPU at a
			 * higher levels either.
			 */
			જો (!sched_asym_prefer(cpu, sg->asym_prefer_cpu))
				પાછા;

			WRITE_ONCE(sg->asym_prefer_cpu, cpu);
			ચાલુ;
		}

		/* Ranking has improved; CPU is still the preferred one. */
		જો (new_prio >= old_prio)
			ચાલુ;

		for_each_cpu(group_cpu, sched_group_span(sg)) {
			જો (sched_asym_prefer(group_cpu, asym_prefer_cpu))
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
સ્થિર અંતઃરેખ પૂર્ણાંક
asym_cpu_capacity_classify(અચલ સંરચના cpumask *sd_span,
			   અચલ સંરચના cpumask *cpu_map)
{
	સંરચના asym_cap_data *entry;
	પૂર્ણાંક ગણતરી = 0, miss = 0;

	/*
	 * Count how many unique CPU capacities this domain spans across
	 * (compare sched_domain CPUs mask with ones representing  available
	 * CPUs capacities). Take into account CPUs that might be offline:
	 * skip those.
	 */
	list_for_each_entry(entry, &asym_cap_list, link) {
		જો (cpumask_intersects(sd_span, cpu_capacity_span(entry)))
			++ગણતરી;
		નહીંતર જો (cpumask_intersects(cpu_map, cpu_capacity_span(entry)))
			++miss;
	}

	WARN_ON_ONCE(!ગણતરી && !list_empty(&asym_cap_list));

	/* No asymmetry detected */
	જો (ગણતરી < 2)
		પાછા 0;
	/* Some of the available CPU capacity values have not been detected */
	જો (miss)
		પાછા SD_ASYM_CPUCAPACITY;

	/* Full asymmetry */
	પાછા SD_ASYM_CPUCAPACITY | SD_ASYM_CPUCAPACITY_FULL;

}

સ્થિર શૂન્ય free_asym_cap_entry(સંરચના rcu_head *head)
{
	સંરચના asym_cap_data *entry = container_of(head, સંરચના asym_cap_data, rcu);
	kfree(entry);
}

સ્થિર અંતઃરેખ શૂન્ય asym_cpu_capacity_update_data(પૂર્ણાંક cpu)
{
	unsigned long capacity = arch_scale_cpu_capacity(cpu);
	સંરચના asym_cap_data *ઉમેરો_entry = ખાલી;
	સંરચના asym_cap_data *entry;

	/*
	 * Search if capacity already exits. If not, track which the entry
	 * where we should insert to keep the list ordered descending.
	 */
	list_for_each_entry(entry, &asym_cap_list, link) {
		જો (capacity == entry->capacity)
			જાઓ done;
		નહીંતર જો (!ઉમેરો_entry && capacity > entry->capacity)
			ઉમેરો_entry = list_prev_entry(entry, link);
	}

	entry = kzalloc(કદ(*entry) + cpumask_size(), GFP_KERNEL);
	જો (WARN_ONCE(!entry, "Failed to allocate memory for asymmetry data\n"))
		પાછા;
	entry->capacity = capacity;

	/* If NULL then the new capacity is the smallest, add last. */
	જો (!ઉમેરો_entry)
		list_add_tail_rcu(&entry->link, &asym_cap_list);
	નહીંતર
		list_add_rcu(&entry->link, &ઉમેરો_entry->link);
done:
	__cpumask_set_cpu(cpu, cpu_capacity_span(entry));
}

/*
 * Build-up/update list of CPUs grouped by their capacities
 * An update requires explicit request to rebuild sched domains
 * with state indicating CPU topology changes.
 */
સ્થિર શૂન્ય asym_cpu_capacity_scan(શૂન્ય)
{
	સંરચના asym_cap_data *entry, *next;
	પૂર્ણાંક cpu;

	list_for_each_entry(entry, &asym_cap_list, link)
		cpumask_clear(cpu_capacity_span(entry));

	for_each_cpu_and(cpu, cpu_possible_mask, housekeeping_cpumask(HK_TYPE_DOMAIN))
		asym_cpu_capacity_update_data(cpu);

	list_for_each_entry_safe(entry, next, &asym_cap_list, link) {
		જો (cpumask_empty(cpu_capacity_span(entry))) {
			list_del_rcu(&entry->link);
			call_rcu(&entry->rcu, free_asym_cap_entry);
		}
	}

	/*
	 * Only one capacity value has been detected i.e. this system is symmetric.
	 * No need to keep this data around.
	 */
	જો (list_is_singular(&asym_cap_list)) {
		entry = list_first_entry(&asym_cap_list, typeof(*entry), link);
		list_del_rcu(&entry->link);
		call_rcu(&entry->rcu, free_asym_cap_entry);
	}
}

/*
 * Initializers for schedule domains
 * Non-inlined to reduce accumulated stack pressure in build_sched_domains()
 */

સ્થિર પૂર્ણાંક default_relax_domain_level = -1;
પૂર્ણાંક sched_domain_level_max;

સ્થિર પૂર્ણાંક __init setup_relax_domain_level(અક્ષર *str)
{
	જો (kstrtoint(str, 0, &default_relax_domain_level))
		pr_warn("Unable to set relax_domain_level\n");

	પાછા 1;
}
__setup("relax_domain_level=", setup_relax_domain_level);

સ્થિર શૂન્ય set_domain_attribute(સંરચના sched_domain *sd,
				 સંરચના sched_domain_attr *attr)
{
	પૂર્ણાંક request;

	જો (!attr || attr->relax_domain_level < 0) {
		જો (default_relax_domain_level < 0)
			પાછા;
		request = default_relax_domain_level;
	} નહીંતર
		request = attr->relax_domain_level;

	જો (sd->level >= request) {
		/* Turn off idle balance on this domain: */
		sd->flags &= ~(SD_BALANCE_WAKE|SD_BALANCE_NEWIDLE);
	}
}

સ્થિર શૂન્ય __sdt_free(અચલ સંરચના cpumask *cpu_map);
સ્થિર પૂર્ણાંક __sdt_alloc(અચલ સંરચના cpumask *cpu_map);

સ્થિર શૂન્ય __sds_free(સંરચના s_data *d, અચલ સંરચના cpumask *cpu_map);
સ્થિર પૂર્ણાંક __sds_alloc(સંરચના s_data *d, અચલ સંરચના cpumask *cpu_map);

સ્થિર શૂન્ય __free_domain_allocs(સંરચના s_data *d, ગણના s_alloc what,
				 અચલ સંરચના cpumask *cpu_map)
{
	પસંદગી (what) {
	સ્થિતિ sa_rootdomain:
		જો (!atomic_read(&d->rd->refcount))
			free_rootdomain(&d->rd->rcu);
		fallthrough;
	સ્થિતિ sa_sd:
		free_percpu(d->sd);
		fallthrough;
	સ્થિતિ sa_sd_shared:
		__sds_free(d, cpu_map);
		fallthrough;
	સ્થિતિ sa_sd_storage:
		__sdt_free(cpu_map);
		fallthrough;
	સ્થિતિ sa_none:
		તોડો;
	}
}

સ્થિર ગણના s_alloc
__visit_domain_allocation_hell(સંરચના s_data *d, અચલ સંરચના cpumask *cpu_map)
{
	memset(d, 0, કદ(*d));

	જો (__sdt_alloc(cpu_map))
		પાછા sa_sd_storage;
	જો (__sds_alloc(d, cpu_map))
		પાછા sa_sd_shared;
	d->sd = alloc_percpu(સંરચના sched_domain *);
	જો (!d->sd)
		પાછા sa_sd_shared;
	d->rd = alloc_rootdomain();
	જો (!d->rd)
		પાછા sa_sd;

	પાછા sa_rootdomain;
}

/*
 * NULL the sd_data elements we've used to build the sched_domain and
 * sched_group structure so that the subsequent __free_domain_allocs()
 * will not free the data we're using.
 */
સ્થિર શૂન્ય claim_allocations(પૂર્ણાંક cpu, સંરચના s_data *d)
{
	સંરચના sched_domain *sd;

	જો (atomic_read(&(*per_cpu_ptr(d->sds, cpu))->ref))
		*per_cpu_ptr(d->sds, cpu) = ખાલી;

	માટે (sd = *per_cpu_ptr(d->sd, cpu); sd; sd = sd->parent) {
		સંરચના sd_data *sdd = sd->ખાનગી;

		WARN_ON_ONCE(*per_cpu_ptr(sdd->sd, cpu) != sd);
		*per_cpu_ptr(sdd->sd, cpu) = ખાલી;

		જો (atomic_read(&(*per_cpu_ptr(sdd->sg, cpu))->ref))
			*per_cpu_ptr(sdd->sg, cpu) = ખાલી;

		જો (atomic_read(&(*per_cpu_ptr(sdd->sgc, cpu))->ref))
			*per_cpu_ptr(sdd->sgc, cpu) = ખાલી;
	}
}

#ifdef CONFIG_NUMA
ગણના numa_topology_type sched_numa_topology_type;

/*
 * sched_domains_numa_distance is derived from sched_numa_node_distance
 * and provides a simplified view of NUMA distances used specifically
 * for building NUMA scheduling domains.
 */
સ્થિર પૂર્ણાંક			sched_domains_numa_levels;
સ્થિર પૂર્ણાંક			sched_numa_ગાંઠ_levels;

પૂર્ણાંક				sched_max_numa_distance;
સ્થિર પૂર્ણાંક			*sched_domains_numa_distance;
સ્થિર પૂર્ણાંક			*sched_numa_ગાંઠ_distance;
સ્થિર સંરચના cpumask		***sched_domains_numa_masks;
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

સ્થિર સંરચના sched_domain *
sd_init(સંરચના sched_domain_topology_level *tl,
	અચલ સંરચના cpumask *cpu_map,
	સંરચના sched_domain *child, પૂર્ણાંક cpu)
{
	સંરચના sd_data *sdd = &tl->data;
	સંરચના sched_domain *sd = *per_cpu_ptr(sdd->sd, cpu);
	પૂર્ણાંક sd_id, sd_weight, sd_flags = 0;
	સંરચના cpumask *sd_span;
	u64 now = sched_clock();

	sd_span = sched_domain_span(sd);
	cpumask_and(sd_span, cpu_map, tl->mask(tl, cpu));
	sd_weight = cpumask_weight(sd_span);
	sd_id = cpumask_first(sd_span);

	જો (tl->sd_flags)
		sd_flags = (*tl->sd_flags)();
	જો (WARN_ONCE(sd_flags & ~TOPOLOGY_SD_FLAGS,
		      "wrong sd_flags in topology description\n"))
		sd_flags &= TOPOLOGY_SD_FLAGS;
#ifdef CONFIG_SCHED_SMT
	જો (sched_smt_asym_packing && (sd_flags & SD_SHARE_CPUCAPACITY))
		sd_flags |= SD_ASYM_PACKING;
#endif
	sd_flags |= asym_cpu_capacity_classify(sd_span, cpu_map);

	*sd = (સંરચના sched_domain){
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
	જો (sd->flags & SD_SHARE_CPUCAPACITY) {
		sd->imbalance_pct = 110;

	} નહીંતર જો (sd->flags & SD_SHARE_LLC) {
		sd->imbalance_pct = 117;
		sd->cache_nice_tries = 1;

#ifdef CONFIG_NUMA
	} નહીંતર જો (sd->flags & SD_NUMA) {
		sd->cache_nice_tries = 2;

		sd->flags &= ~SD_PREFER_SIBLING;
		sd->flags |= SD_SERIALIZE;
		જો (sched_domains_numa_distance[tl->numa_level] > ગાંઠ_reclaim_distance) {
			sd->flags &= ~(SD_BALANCE_EXEC |
				       SD_BALANCE_FORK |
				       SD_WAKE_AFFINE);
		}

#endif /* CONFIG_NUMA */
	} નહીંતર {
		sd->cache_nice_tries = 1;
	}

	sd->ખાનગી = sdd;

	પાછા sd;
}

#ifdef CONFIG_SCHED_SMT
પૂર્ણાંક cpu_smt_flags(શૂન્ય)
{
	પાછા SD_SHARE_CPUCAPACITY | SD_SHARE_LLC;
}

અચલ સંરચના cpumask *tl_smt_mask(સંરચના sched_domain_topology_level *tl, પૂર્ણાંક cpu)
{
	પાછા cpu_smt_mask(cpu);
}
#endif

#ifdef CONFIG_SCHED_CLUSTER
પૂર્ણાંક cpu_cluster_flags(શૂન્ય)
{
	પાછા SD_CLUSTER | SD_SHARE_LLC;
}

અચલ સંરચના cpumask *tl_cls_mask(સંરચના sched_domain_topology_level *tl, પૂર્ણાંક cpu)
{
	પાછા cpu_clustergroup_mask(cpu);
}
#endif

#ifdef CONFIG_SCHED_MC
પૂર્ણાંક cpu_core_flags(શૂન્ય)
{
	પાછા SD_SHARE_LLC;
}

અચલ સંરચના cpumask *tl_mc_mask(સંરચના sched_domain_topology_level *tl, પૂર્ણાંક cpu)
{
	પાછા cpu_coregroup_mask(cpu);
}

/*
 * Majority of architectures have LLC at MC domain level with exception
 * such as powerpc. Provide a way for arch to specify where its LLC is
 * if it falls in exception category
 */
# ifndef arch_llc_mask
#define arch_llc_mask(cpu) cpu_coregroup_mask(cpu)
# endif

#નહીંતર
#define arch_llc_mask(cpu) cpumask_of(cpu)
#endif

#define llc_mask(cpu) arch_llc_mask(cpu)

અચલ સંરચના cpumask *tl_pkg_mask(સંરચના sched_domain_topology_level *tl, પૂર્ણાંક cpu)
{
	પાછા cpu_ગાંઠ_mask(cpu);
}

/*
 * Topology list, bottom-up.
 */
સ્થિર સંરચના sched_domain_topology_level default_topology[] = {
#ifdef CONFIG_SCHED_SMT
	SDTL_INIT(tl_smt_mask, cpu_smt_flags, SMT),
#endif

#ifdef CONFIG_SCHED_CLUSTER
	SDTL_INIT(tl_cls_mask, cpu_cluster_flags, CLS),
#endif

#ifdef CONFIG_SCHED_MC
	SDTL_INIT(tl_mc_mask, cpu_core_flags, MC),
#endif
	SDTL_INIT(tl_pkg_mask, ખાલી, PKG),
	{ ખાલી, },
};

સ્થિર સંરચના sched_domain_topology_level *sched_domain_topology =
	default_topology;
સ્થિર સંરચના sched_domain_topology_level *sched_domain_topology_saved;

#define for_each_sd_topology(tl)			\
	માટે (tl = sched_domain_topology; tl->mask; tl++)

શૂન્ય __init set_sched_topology(સંરચના sched_domain_topology_level *tl)
{
	જો (WARN_ON_ONCE(sched_smp_initialized))
		પાછા;

	sched_domain_topology = tl;
	sched_domain_topology_saved = ખાલી;
}

#ifdef CONFIG_NUMA
સ્થિર પૂર્ણાંક cpu_numa_flags(શૂન્ય)
{
	પાછા SD_NUMA;
}

સ્થિર અચલ સંરચના cpumask *sd_numa_mask(સંરચના sched_domain_topology_level *tl, પૂર્ણાંક cpu)
{
	પાછા sched_domains_numa_masks[tl->numa_level][cpu_to_ગાંઠ(cpu)];
}

સ્થિર શૂન્ય sched_numa_warn(અચલ અક્ષર *str)
{
	સ્થિર પૂર્ણાંક done = false;
	પૂર્ણાંક i,j;

	જો (done)
		પાછા;

	done = true;

	printk(KERN_WARNING "ERROR: %s\n\n", str);

	માટે (i = 0; i < nr_ગાંઠ_ids; i++) {
		printk(KERN_WARNING "  ");
		માટે (j = 0; j < nr_ગાંઠ_ids; j++) {
			જો (!ગાંઠ_state(i, N_CPU) || !ગાંઠ_state(j, N_CPU))
				printk(KERN_CONT "(%02d) ", ગાંઠ_distance(i,j));
			નહીંતર
				printk(KERN_CONT " %02d  ", ગાંઠ_distance(i,j));
		}
		printk(KERN_CONT "\n");
	}
	printk(KERN_WARNING "\n");
}

bool find_numa_distance(પૂર્ણાંક distance)
{
	bool found = false;
	પૂર્ણાંક i, *distances;

	જો (distance == ગાંઠ_distance(0, 0))
		પાછા true;

	rcu_read_lock();
	distances = rcu_dereference(sched_numa_ગાંઠ_distance);
	જો (!distances)
		જાઓ unlock;
	માટે (i = 0; i < sched_numa_ગાંઠ_levels; i++) {
		જો (distances[i] == distance) {
			found = true;
			તોડો;
		}
	}
unlock:
	rcu_read_unlock();

	પાછા found;
}

#define for_each_cpu_ગાંઠ_but(n, nbut)		\
	for_each_ગાંઠ_state(n, N_CPU)		\
		જો (n == nbut)			\
			ચાલુ;		\
		નહીંતર

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
સ્થિર શૂન્ય init_numa_topology_type(પૂર્ણાંક offline_ગાંઠ)
{
	પૂર્ણાંક a, b, c, n;

	n = sched_max_numa_distance;

	જો (sched_domains_numa_levels <= 2) {
		sched_numa_topology_type = NUMA_DIRECT;
		પાછા;
	}

	for_each_cpu_ગાંઠ_but(a, offline_ગાંઠ) {
		for_each_cpu_ગાંઠ_but(b, offline_ગાંઠ) {
			/* Find two nodes furthest removed from each other. */
			જો (ગાંઠ_distance(a, b) < n)
				ચાલુ;

			/* Is there an intermediary node between a and b? */
			for_each_cpu_ગાંઠ_but(c, offline_ગાંઠ) {
				જો (ગાંઠ_distance(a, c) < n &&
				    ગાંઠ_distance(b, c) < n) {
					sched_numa_topology_type =
							NUMA_GLUELESS_MESH;
					પાછા;
				}
			}

			sched_numa_topology_type = NUMA_BACKPLANE;
			પાછા;
		}
	}

	pr_err("Failed to find a NUMA topology type, defaulting to DIRECT\n");
	sched_numa_topology_type = NUMA_DIRECT;
}


#define NR_DISTANCE_કિંમતો (1 << DISTANCE_BITS)

/*
 * An architecture could modify its NUMA distance, to change
 * grouping of NUMA nodes and number of NUMA levels when creating
 * NUMA level sched domains.
 *
 * A NUMA level is created for each unique
 * arch_sched_node_distance.
 */
સ્થિર પૂર્ણાંક numa_ગાંઠ_dist(પૂર્ણાંક i, પૂર્ણાંક j)
{
	પાછા ગાંઠ_distance(i, j);
}

પૂર્ણાંક arch_sched_ગાંઠ_distance(પૂર્ણાંક from, પૂર્ણાંક to)
			     __weak __alias(numa_ગાંઠ_dist);

સ્થિર bool modified_sched_ગાંઠ_distance(શૂન્ય)
{
	પાછા numa_ગાંઠ_dist != arch_sched_ગાંઠ_distance;
}

સ્થિર પૂર્ણાંક sched_record_numa_dist(પૂર્ણાંક offline_ગાંઠ, પૂર્ણાંક (*n_dist)(પૂર્ણાંક, પૂર્ણાંક),
				  પૂર્ણાંક **dist, પૂર્ણાંક *levels)
{
	unsigned long *distance_map __free(bitmap) = ખાલી;
	પૂર્ણાંક nr_levels = 0;
	પૂર્ણાંક i, j;
	પૂર્ણાંક *distances;

	/*
	 * O(nr_nodes^2) de-duplicating selection sort -- in order to find the
	 * unique distances in the node_distance() table.
	 */
	distance_map = bitmap_alloc(NR_DISTANCE_કિંમતો, GFP_KERNEL);
	જો (!distance_map)
		પાછા -ENOMEM;

	bitmap_zero(distance_map, NR_DISTANCE_કિંમતો);
	for_each_cpu_ગાંઠ_but(i, offline_ગાંઠ) {
		for_each_cpu_ગાંઠ_but(j, offline_ગાંઠ) {
			પૂર્ણાંક distance = n_dist(i, j);

			જો (distance < LOCAL_DISTANCE || distance >= NR_DISTANCE_કિંમતો) {
				sched_numa_warn("Invalid distance value range");
				પાછા -EINVAL;
			}

			bitmap_set(distance_map, distance, 1);
		}
	}
	/*
	 * We can now figure out how many unique distance values there are and
	 * allocate memory accordingly.
	 */
	nr_levels = bitmap_weight(distance_map, NR_DISTANCE_કિંમતો);

	distances = kzalloc_objs(પૂર્ણાંક, nr_levels);
	જો (!distances)
		પાછા -ENOMEM;

	માટે (i = 0, j = 0; i < nr_levels; i++, j++) {
		j = find_next_bit(distance_map, NR_DISTANCE_કિંમતો, j);
		distances[i] = j;
	}
	*dist = distances;
	*levels = nr_levels;

	પાછા 0;
}

શૂન્ય sched_init_numa(પૂર્ણાંક offline_ગાંઠ)
{
	સંરચના sched_domain_topology_level *tl;
	પૂર્ણાંક nr_levels, nr_ગાંઠ_levels;
	પૂર્ણાંક i, j;
	પૂર્ણાંક *distances, *domain_distances;
	સંરચના cpumask ***masks;

	/* Record the NUMA distances from SLIT table */
	જો (sched_record_numa_dist(offline_ગાંઠ, numa_ગાંઠ_dist, &distances,
				   &nr_ગાંઠ_levels))
		પાછા;

	/* Record modified NUMA distances for building sched domains */
	જો (modified_sched_ગાંઠ_distance()) {
		જો (sched_record_numa_dist(offline_ગાંઠ, arch_sched_ગાંઠ_distance,
					   &domain_distances, &nr_levels)) {
			kfree(distances);
			પાછા;
		}
	} નહીંતર {
		domain_distances = distances;
		nr_levels = nr_ગાંઠ_levels;
	}
	rcu_assign_pointer(sched_numa_ગાંઠ_distance, distances);
	WRITE_ONCE(sched_max_numa_distance, distances[nr_ગાંઠ_levels - 1]);
	WRITE_ONCE(sched_numa_ગાંઠ_levels, nr_ગાંઠ_levels);

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

	masks = kzalloc(કદ(શૂન્ય *) * nr_levels, GFP_KERNEL);
	જો (!masks)
		પાછા;

	/*
	 * Now for each level, construct a mask per node which contains all
	 * CPUs of nodes that are that many hops away from us.
	 */
	માટે (i = 0; i < nr_levels; i++) {
		masks[i] = kzalloc(nr_ગાંઠ_ids * કદ(શૂન્ય *), GFP_KERNEL);
		જો (!masks[i])
			પાછા;

		for_each_cpu_ગાંઠ_but(j, offline_ગાંઠ) {
			સંરચના cpumask *mask = kzalloc(cpumask_size(), GFP_KERNEL);
			પૂર્ણાંક k;

			જો (!mask)
				પાછા;

			masks[i][j] = mask;

			for_each_cpu_ગાંઠ_but(k, offline_ગાંઠ) {
				જો (sched_debug() &&
				    (arch_sched_ગાંઠ_distance(j, k) !=
				     arch_sched_ગાંઠ_distance(k, j)))
					sched_numa_warn("Node-distance not symmetric");

				જો (arch_sched_ગાંઠ_distance(j, k) >
				    sched_domains_numa_distance[i])
					ચાલુ;

				cpumask_or(mask, mask, cpumask_of_ગાંઠ(k));
			}
		}
	}
	rcu_assign_pointer(sched_domains_numa_masks, masks);

	/* Compute default topology size */
	માટે (i = 0; sched_domain_topology[i].mask; i++);

	tl = kzalloc((i + nr_levels + 1) *
			કદ(સંરચના sched_domain_topology_level), GFP_KERNEL);
	જો (!tl)
		પાછા;

	/*
	 * Copy the default topology bits..
	 */
	માટે (i = 0; sched_domain_topology[i].mask; i++)
		tl[i] = sched_domain_topology[i];

	/*
	 * Add the NUMA identity distance, aka single NODE.
	 */
	tl[i++] = SDTL_INIT(sd_numa_mask, ખાલી, ગાંઠ_2);

	/*
	 * .. and append 'j' levels of NUMA goodness.
	 */
	માટે (j = 1; j < nr_levels; i++, j++) {
		tl[i] = SDTL_INIT(sd_numa_mask, cpu_numa_flags, NUMA);
		tl[i].numa_level = j;
	}

	sched_domain_topology_saved = sched_domain_topology;
	sched_domain_topology = tl;

	sched_domains_numa_levels = nr_levels;

	init_numa_topology_type(offline_ગાંઠ);
}


સ્થિર શૂન્ય sched_reset_numa(શૂન્ય)
{
	પૂર્ણાંક nr_levels, *distances, *dom_distances = ખાલી;
	સંરચના cpumask ***masks;

	nr_levels = sched_domains_numa_levels;
	sched_numa_ગાંઠ_levels = 0;
	sched_domains_numa_levels = 0;
	sched_max_numa_distance = 0;
	sched_numa_topology_type = NUMA_DIRECT;
	distances = sched_numa_ગાંઠ_distance;
	જો (sched_numa_ગાંઠ_distance != sched_domains_numa_distance)
		dom_distances = sched_domains_numa_distance;
	rcu_assign_pointer(sched_numa_ગાંઠ_distance, ખાલી);
	rcu_assign_pointer(sched_domains_numa_distance, ખાલી);
	masks = sched_domains_numa_masks;
	rcu_assign_pointer(sched_domains_numa_masks, ખાલી);
	જો (distances || masks) {
		પૂર્ણાંક i, j;

		synchronize_rcu();
		kfree(distances);
		kfree(dom_distances);
		માટે (i = 0; i < nr_levels && masks; i++) {
			જો (!masks[i])
				ચાલુ;
			for_each_ગાંઠ(j)
				kfree(masks[i][j]);
			kfree(masks[i]);
		}
		kfree(masks);
	}
	જો (sched_domain_topology_saved) {
		kfree(sched_domain_topology);
		sched_domain_topology = sched_domain_topology_saved;
		sched_domain_topology_saved = ખાલી;
	}
}

/*
 * Call with hotplug lock held
 */
શૂન્ય sched_update_numa(પૂર્ણાંક cpu, bool online)
{
	પૂર્ણાંક ગાંઠ;

	ગાંઠ = cpu_to_ગાંઠ(cpu);
	/*
	 * Scheduler NUMA topology is updated when the first CPU of a
	 * node is onlined or the last CPU of a node is offlined.
	 */
	જો (cpumask_weight(cpumask_of_ગાંઠ(ગાંઠ)) != 1)
		પાછા;

	sched_reset_numa();
	sched_init_numa(online ? NUMA_NO_ગાંઠ : ગાંઠ);
}

શૂન્ય sched_domains_numa_masks_set(unsigned પૂર્ણાંક cpu)
{
	પૂર્ણાંક ગાંઠ = cpu_to_ગાંઠ(cpu);
	પૂર્ણાંક i, j;

	માટે (i = 0; i < sched_domains_numa_levels; i++) {
		માટે (j = 0; j < nr_ગાંઠ_ids; j++) {
			જો (!ગાંઠ_state(j, N_CPU))
				ચાલુ;

			/* Set ourselves in the remote node's masks */
			જો (arch_sched_ગાંઠ_distance(j, ગાંઠ) <=
			    sched_domains_numa_distance[i])
				cpumask_set_cpu(cpu, sched_domains_numa_masks[i][j]);
		}
	}
}

શૂન્ય sched_domains_numa_masks_clear(unsigned પૂર્ણાંક cpu)
{
	પૂર્ણાંક i, j;

	માટે (i = 0; i < sched_domains_numa_levels; i++) {
		માટે (j = 0; j < nr_ગાંઠ_ids; j++) {
			જો (sched_domains_numa_masks[i][j])
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
પૂર્ણાંક sched_numa_find_closest(અચલ સંરચના cpumask *cpus, પૂર્ણાંક cpu)
{
	પૂર્ણાંક i, j = cpu_to_ગાંઠ(cpu), found = nr_cpu_ids;
	સંરચના cpumask ***masks;

	rcu_read_lock();
	masks = rcu_dereference(sched_domains_numa_masks);
	જો (!masks)
		જાઓ unlock;
	માટે (i = 0; i < sched_domains_numa_levels; i++) {
		જો (!masks[i][j])
			તોડો;
		cpu = cpumask_any_and_distribute(cpus, masks[i][j]);
		જો (cpu < nr_cpu_ids) {
			found = cpu;
			તોડો;
		}
	}
unlock:
	rcu_read_unlock();

	પાછા found;
}

સંરચના __cmp_key {
	અચલ સંરચના cpumask *cpus;
	સંરચના cpumask ***masks;
	પૂર્ણાંક ગાંઠ;
	પૂર્ણાંક cpu;
	પૂર્ણાંક w;
};

સ્થિર પૂર્ણાંક hop_cmp(અચલ શૂન્ય *a, અચલ શૂન્ય *b)
{
	સંરચના cpumask **prev_hop, **cur_hop = *(સંરચના cpumask ***)b;
	સંરચના __cmp_key *k = (સંરચના __cmp_key *)a;

	જો (cpumask_weight_and(k->cpus, cur_hop[k->ગાંઠ]) <= k->cpu)
		પાછા 1;

	જો (b == k->masks) {
		k->w = 0;
		પાછા 0;
	}

	prev_hop = *((સંરચના cpumask ***)b - 1);
	k->w = cpumask_weight_and(k->cpus, prev_hop[k->ગાંઠ]);
	જો (k->w <= k->cpu)
		પાછા 0;

	પાછા -1;
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
પૂર્ણાંક sched_numa_find_nth_cpu(અચલ સંરચના cpumask *cpus, પૂર્ણાંક cpu, પૂર્ણાંક ગાંઠ)
{
	સંરચના __cmp_key k = { .cpus = cpus, .cpu = cpu };
	સંરચના cpumask ***hop_masks;
	પૂર્ણાંક hop, ret = nr_cpu_ids;

	જો (ગાંઠ == NUMA_NO_ગાંઠ)
		પાછા cpumask_nth_and(cpu, cpus, cpu_online_mask);

	rcu_read_lock();

	/* CPU-less node entries are uninitialized in sched_domains_numa_masks */
	ગાંઠ = numa_nearest_ગાંઠ(ગાંઠ, N_CPU);
	k.ગાંઠ = ગાંઠ;

	k.masks = rcu_dereference(sched_domains_numa_masks);
	જો (!k.masks)
		જાઓ unlock;

	hop_masks = bsearch(&k, k.masks, sched_domains_numa_levels, કદ(k.masks[0]), hop_cmp);
	જો (!hop_masks)
		જાઓ unlock;
	hop = hop_masks	- k.masks;

	ret = hop ?
		cpumask_nth_and_andnot(cpu - k.w, cpus, k.masks[hop][ગાંઠ], k.masks[hop-1][ગાંઠ]) :
		cpumask_nth_and(cpu, cpus, k.masks[0][ગાંઠ]);
unlock:
	rcu_read_unlock();
	પાછા ret;
}
EXPORT_પ્રતીક_GPL(sched_numa_find_nth_cpu);

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
અચલ સંરચના cpumask *sched_numa_hop_mask(unsigned પૂર્ણાંક ગાંઠ, unsigned પૂર્ણાંક hops)
{
	સંરચના cpumask ***masks;

	જો (ગાંઠ >= nr_ગાંઠ_ids || hops >= sched_domains_numa_levels)
		પાછા ERR_PTR(-EINVAL);

	masks = rcu_dereference(sched_domains_numa_masks);
	જો (!masks)
		પાછા ERR_PTR(-EBUSY);

	પાછા masks[hops][ગાંઠ];
}
EXPORT_પ્રતીક_GPL(sched_numa_hop_mask);

#endif /* CONFIG_NUMA */

સ્થિર પૂર્ણાંક __sdt_alloc(અચલ સંરચના cpumask *cpu_map)
{
	સંરચના sched_domain_topology_level *tl;
	પૂર્ણાંક j;

	for_each_sd_topology(tl) {
		સંરચના sd_data *sdd = &tl->data;

		sdd->sd = alloc_percpu(સંરચના sched_domain *);
		જો (!sdd->sd)
			પાછા -ENOMEM;

		sdd->sg = alloc_percpu(સંરચના sched_group *);
		જો (!sdd->sg)
			પાછા -ENOMEM;

		sdd->sgc = alloc_percpu(સંરચના sched_group_capacity *);
		જો (!sdd->sgc)
			પાછા -ENOMEM;

		for_each_cpu(j, cpu_map) {
			સંરચના sched_domain *sd;
			સંરચના sched_group *sg;
			સંરચના sched_group_capacity *sgc;

			sd = kzalloc_ગાંઠ(કદ(સંરચના sched_domain) + cpumask_size(),
					GFP_KERNEL, cpu_to_ગાંઠ(j));
			જો (!sd)
				પાછા -ENOMEM;

			*per_cpu_ptr(sdd->sd, j) = sd;

			sg = kzalloc_ગાંઠ(કદ(સંરચના sched_group) + cpumask_size(),
					GFP_KERNEL, cpu_to_ગાંઠ(j));
			જો (!sg)
				પાછા -ENOMEM;

			sg->next = sg;

			*per_cpu_ptr(sdd->sg, j) = sg;

			sgc = kzalloc_ગાંઠ(કદ(સંરચના sched_group_capacity) + cpumask_size(),
					GFP_KERNEL, cpu_to_ગાંઠ(j));
			જો (!sgc)
				પાછા -ENOMEM;

			sgc->id = j;

			*per_cpu_ptr(sdd->sgc, j) = sgc;
		}
	}

	પાછા 0;
}

સ્થિર શૂન્ય __sdt_free(અચલ સંરચના cpumask *cpu_map)
{
	સંરચના sched_domain_topology_level *tl;
	પૂર્ણાંક j;

	for_each_sd_topology(tl) {
		સંરચના sd_data *sdd = &tl->data;

		for_each_cpu(j, cpu_map) {
			સંરચના sched_domain *sd;

			જો (sdd->sd) {
				sd = *per_cpu_ptr(sdd->sd, j);
				જો (sd && (sd->flags & SD_NUMA))
					free_sched_groups(sd->groups, 0);
				kfree(*per_cpu_ptr(sdd->sd, j));
			}

			જો (sdd->sg)
				kfree(*per_cpu_ptr(sdd->sg, j));
			જો (sdd->sgc)
				kfree(*per_cpu_ptr(sdd->sgc, j));
		}
		free_percpu(sdd->sd);
		sdd->sd = ખાલી;
		free_percpu(sdd->sg);
		sdd->sg = ખાલી;
		free_percpu(sdd->sgc);
		sdd->sgc = ખાલી;
	}
}

સ્થિર પૂર્ણાંક __sds_alloc(સંરચના s_data *d, અચલ સંરચના cpumask *cpu_map)
{
	પૂર્ણાંક j;

	d->sds = alloc_percpu(સંરચના sched_domain_shared *);
	જો (!d->sds)
		પાછા -ENOMEM;

	for_each_cpu(j, cpu_map) {
		સંરચના sched_domain_shared *sds;

		sds = kzalloc_ગાંઠ(કદ(સંરચના sched_domain_shared),
				GFP_KERNEL, cpu_to_ગાંઠ(j));
		જો (!sds)
			પાછા -ENOMEM;

		*per_cpu_ptr(d->sds, j) = sds;
	}

	પાછા 0;
}

સ્થિર શૂન્ય __sds_free(સંરચના s_data *d, અચલ સંરચના cpumask *cpu_map)
{
	પૂર્ણાંક j;

	જો (!d->sds)
		પાછા;

	for_each_cpu(j, cpu_map)
		kfree(*per_cpu_ptr(d->sds, j));

	free_percpu(d->sds);
	d->sds = ખાલી;
}

સ્થિર સંરચના sched_domain *build_sched_domain(સંરચના sched_domain_topology_level *tl,
		અચલ સંરચના cpumask *cpu_map, સંરચના sched_domain_attr *attr,
		સંરચના sched_domain *child, પૂર્ણાંક cpu)
{
	સંરચના sched_domain *sd = sd_init(tl, cpu_map, child, cpu);

	જો (child) {
		sd->level = child->level + 1;
		sched_domain_level_max = max(sched_domain_level_max, sd->level);
		child->parent = sd;

		જો (!cpumask_subset(sched_domain_span(child),
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

	પાછા sd;
}

/*
 * Ensure topology masks are sane, i.e. there are no conflicts (overlaps) for
 * any two given CPUs on non-NUMA topology levels.
 */
સ્થિર bool topology_span_sane(અચલ સંરચના cpumask *cpu_map)
{
	સંરચના sched_domain_topology_level *tl;
	સંરચના cpumask *covered, *id_seen;
	પૂર્ણાંક cpu;

	lockdep_assert_held(&sched_domains_mutex);
	covered = sched_domains_tmpmask;
	id_seen = sched_domains_tmpmask2;

	for_each_sd_topology(tl) {
		પૂર્ણાંક tl_common_flags = 0;

		જો (tl->sd_flags)
			tl_common_flags = (*tl->sd_flags)();

		/* NUMA levels are allowed to overlap */
		જો (tl_common_flags & SD_NUMA)
			ચાલુ;

		cpumask_clear(covered);
		cpumask_clear(id_seen);

		/*
		 * Non-NUMA levels cannot partially overlap - they must be either
		 * completely equal or completely disjoint. Otherwise we can end up
		 * breaking the sched_group lists - i.e. a later get_group() pass
		 * breaks the linking done for an earlier span.
		 */
		for_each_cpu(cpu, cpu_map) {
			અચલ સંરચના cpumask *tl_cpu_mask = tl->mask(tl, cpu);
			પૂર્ણાંક id;

			/* lowest bit set in this mask is used as a unique id */
			id = cpumask_first(tl_cpu_mask);

			જો (cpumask_test_cpu(id, id_seen)) {
				/* First CPU has already been seen, ensure identical spans */
				જો (!cpumask_equal(tl->mask(tl, id), tl_cpu_mask))
					પાછા false;
			} નહીંતર {
				/* First CPU hasn't been seen before, ensure it's a completely new span */
				જો (cpumask_intersects(tl_cpu_mask, covered))
					પાછા false;

				cpumask_or(covered, covered, tl_cpu_mask);
				cpumask_set_cpu(id, id_seen);
			}
		}
	}
	પાછા true;
}

/*
 * Calculate an allowed NUMA imbalance such that LLCs do not get
 * imbalanced.
 */
સ્થિર શૂન્ય adjust_numa_imbalance(સંરચના sched_domain *sd_llc)
{
	સંરચના sched_domain *parent;
	unsigned પૂર્ણાંક imb_span = 1;
	unsigned પૂર્ણાંક imb = 0;
	unsigned પૂર્ણાંક nr_llcs;

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
	જો (nr_llcs == 1)
		imb = sd_llc->parent->span_weight >> 3;
	નહીંતર
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
	જ્યારે (parent && !(parent->flags & SD_NUMA))
		parent = parent->parent;

	imb_span = parent ? parent->span_weight : sd_llc->parent->span_weight;

	/* Update the upper remainder of the topology */
	parent = sd_llc->parent;
	જ્યારે (parent) {
		પૂર્ણાંક factor = max(1U, (parent->span_weight / imb_span));

		parent->imb_numa_nr = imb * factor;
		parent = parent->parent;
	}
}

સ્થિર શૂન્ય
init_sched_domain_shared(સંરચના s_data *d, સંરચના sched_domain *sd, પૂર્ણાંક flags)
{
	સંરચના sched_domain_shared *sds = ખાલી;
	પૂર્ણાંક cpu;

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

		જો (!sds->alloc_flags ||
		    sd->span_weight == 1 ||
		    sds->alloc_flags == flags) {
			sds->alloc_flags = flags;
			sd->shared = sds;
			તોડો;
		}
	}

	/*
	 * Use the sd_shared corresponding to the last
	 * CPU in the span if none are avaialable.
	 */
	જો (WARN_ON_ONCE(!sd->shared))
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
સ્થિર bool claim_asym_sched_domain_shared(સંરચના s_data *d, પૂર્ણાંક cpu)
{
	સંરચના sched_domain *sd = *per_cpu_ptr(d->sd, cpu);
	સંરચના sched_domain *sd_asym;

	જો (!sd)
		પાછા false;

	sd_asym = sd;
	જ્યારે (sd_asym && !(sd_asym->flags & SD_ASYM_CPUCAPACITY_FULL))
		sd_asym = sd_asym->parent;

	જો (!sd_asym || (sd_asym->flags & SD_NUMA))
		પાછા false;

	init_sched_domain_shared(d, sd_asym, SD_ASYM_CPUCAPACITY);
	પાછા true;
}

સ્થિર પૂર્ણાંક __sched_domains_alloc_llc_id(શૂન્ય)
{
	પૂર્ણાંક lid, max;

	lockdep_assert_held(&sched_domains_mutex);

	lid = cpumask_first_zero(sched_domains_llc_id_allocmask);
	/*
	 * llc_id space should never grow larger than the
	 * possible number of CPUs in the system.
	 */
	જો (lid >= nr_cpu_ids)
		પાછા -1;

	__cpumask_set_cpu(lid, sched_domains_llc_id_allocmask);
	max = cpumask_last(sched_domains_llc_id_allocmask);
	જો (max > max_lid)
		max_lid = max;

	પાછા lid;
}

સ્થિર શૂન્ય __sched_domains_free_llc_id(પૂર્ણાંક cpu)
{
	પૂર્ણાંક i, lid, max;

	lockdep_assert_held(&sched_domains_mutex);

	lid = per_cpu(sd_llc_id, cpu);
	જો (lid == -1 || lid >= nr_cpu_ids)
		પાછા;

	per_cpu(sd_llc_id, cpu) = -1;

	for_each_cpu(i, llc_mask(cpu)) {
		/* An online CPU owns the llc_id. */
		જો (per_cpu(sd_llc_id, i) == lid)
			પાછા;
	}

	__cpumask_clear_cpu(lid, sched_domains_llc_id_allocmask);

	max = cpumask_last(sched_domains_llc_id_allocmask);
	/* shrink max lid to save memory */
	જો (max < max_lid)
		max_lid = max;
}

શૂન્ય sched_domains_free_llc_id(પૂર્ણાંક cpu)
{
	sched_domains_mutex_lock();
	__sched_domains_free_llc_id(cpu);
	sched_domains_mutex_unlock();
}

/*
 * Build sched domains for a given set of CPUs and attach the sched domains
 * to the individual CPUs
 */
સ્થિર પૂર્ણાંક
build_sched_domains(અચલ સંરચના cpumask *cpu_map, સંરચના sched_domain_attr *attr,
		    bool *multi_llcs)
{
	ગણના s_alloc alloc_state = sa_none;
	bool has_multi_llcs = false;
	સંરચના sched_domain *sd;
	સંરચના s_data d;
	સંરચના rq *rq = ખાલી;
	પૂર્ણાંક i, ret = -ENOMEM;
	bool has_asym = false;
	bool has_cluster = false;

	જો (WARN_ON(cpumask_empty(cpu_map)))
		જાઓ error;

	alloc_state = __visit_domain_allocation_hell(&d, cpu_map);
	જો (alloc_state != sa_rootdomain)
		જાઓ error;

	/* Set up domains for CPUs specified by the cpu_map: */
	for_each_cpu(i, cpu_map) {
		સંરચના sched_domain_topology_level *tl;
		પૂર્ણાંક lid;

		sd = ખાલી;
		for_each_sd_topology(tl) {

			sd = build_sched_domain(tl, cpu_map, attr, sd, i);

			has_asym |= sd->flags & SD_ASYM_CPUCAPACITY;

			જો (tl == sched_domain_topology)
				*per_cpu_ptr(d.sd, i) = sd;
			જો (cpumask_equal(cpu_map, sched_domain_span(sd)))
				તોડો;
		}

		lid = per_cpu(sd_llc_id, i);
		જો (lid == -1) {
			/* try to reuse the llc_id of its siblings */
			માટે (પૂર્ણાંક j = cpumask_first(llc_mask(i));
			     j < nr_cpu_ids;
			     j = cpumask_next(j, llc_mask(i))) {
				જો (i == j)
					ચાલુ;

				lid = per_cpu(sd_llc_id, j);

				જો (lid != -1) {
					per_cpu(sd_llc_id, i) = lid;

					તોડો;
				}
			}

			/* a new LLC is detected */
			જો (lid == -1)
				per_cpu(sd_llc_id, i) = __sched_domains_alloc_llc_id();
		}
	}

	જો (WARN_ON(!topology_span_sane(cpu_map)))
		જાઓ error;

	/* Build the groups for the domains */
	for_each_cpu(i, cpu_map) {
		માટે (sd = *per_cpu_ptr(d.sd, i); sd; sd = sd->parent) {
			sd->span_weight = cpumask_weight(sched_domain_span(sd));
			જો (sd->flags & SD_NUMA) {
				જો (build_overlap_sched_groups(sd, i))
					જાઓ error;
			} નહીંતર {
				જો (build_sched_groups(sd, i))
					જાઓ error;
			}
		}
	}

	for_each_cpu(i, cpu_map) {
		sd = *per_cpu_ptr(d.sd, i);
		જો (!sd)
			ચાલુ;

		જો (has_asym)
			claim_asym_sched_domain_shared(&d, i);

		/* First, find the topmost SD_SHARE_LLC domain */
		જ્યારે (sd->parent && (sd->parent->flags & SD_SHARE_LLC))
			sd = sd->parent;

		જો (sd->flags & SD_SHARE_LLC) {
			init_sched_domain_shared(&d, sd, SD_SHARE_LLC);

			/*
			 * In presence of higher domains, adjust the
			 * NUMA imbalance stats for the hierarchy.
			 */
			જો (sd->parent) {
				જો (IS_ENABLED(CONFIG_NUMA))
					adjust_numa_imbalance(sd);

				જો (sd_in_multi_llcs(sd))
					has_multi_llcs = true;
			}
		}
	}

	/* Calculate CPU capacity for physical packages and nodes */
	માટે (i = nr_cpumask_bits-1; i >= 0; i--) {
		જો (!cpumask_test_cpu(i, cpu_map))
			ચાલુ;

		claim_allocations(i, &d);

		માટે (sd = *per_cpu_ptr(d.sd, i); sd; sd = sd->parent)
			init_sched_groups_capacity(i, sd);
	}

	alloc_sd_llc(cpu_map, &d);

	/* Attach the domains */
	rcu_read_lock();
	for_each_cpu(i, cpu_map) {
		rq = cpu_rq(i);
		sd = *per_cpu_ptr(d.sd, i);

		cpu_attach_domain(sd, d.rd, i);

		જો (lowest_flag_domain(i, SD_CLUSTER))
			has_cluster = true;
	}
	rcu_read_unlock();

	જો (has_asym)
		static_branch_inc_cpuslocked(&sched_asym_cpucapacity);

	જો (has_cluster)
		static_branch_inc_cpuslocked(&sched_cluster_active);

	જો (rq && sched_debug_verbose)
		pr_info("root domain span: %*pbl\n", cpumask_pr_args(cpu_map));

	ret = 0;
error:
	*multi_llcs = has_multi_llcs;
	__free_domain_allocs(&d, alloc_state, cpu_map);

	પાછા ret;
}

/* Current sched domains: */
સ્થિર cpumask_var_t			*doms_cur;

/* Number of sched domains in 'doms_cur': */
સ્થિર પૂર્ણાંક				ndoms_cur;

/* Attributes of custom domains in 'doms_cur' */
સ્થિર સંરચના sched_domain_attr		*dattr_cur;

/*
 * Special case: If a kmalloc() of a doms_cur partition (array of
 * cpumask) fails, then fallback to a single sched domain,
 * as determined by the single cpumask fallback_doms.
 */
સ્થિર cpumask_var_t			fallback_doms;

/*
 * arch_update_cpu_topology lets virtualized architectures update the
 * CPU core maps. It is supposed to return 1 if the topology changed
 * or 0 if it stayed the same.
 */
પૂર્ણાંક __weak arch_update_cpu_topology(શૂન્ય)
{
	પાછા 0;
}

cpumask_var_t *alloc_sched_domains(unsigned પૂર્ણાંક ndoms)
{
	પૂર્ણાંક i;
	cpumask_var_t *doms;

	doms = kmalloc_objs(*doms, ndoms);
	જો (!doms)
		પાછા ખાલી;
	માટે (i = 0; i < ndoms; i++) {
		જો (!alloc_cpumask_var(&doms[i], GFP_KERNEL)) {
			free_sched_domains(doms, i);
			પાછા ખાલી;
		}
	}
	પાછા doms;
}

શૂન્ય free_sched_domains(cpumask_var_t doms[], unsigned પૂર્ણાંક ndoms)
{
	unsigned પૂર્ણાંક i;
	માટે (i = 0; i < ndoms; i++)
		free_cpumask_var(doms[i]);
	kfree(doms);
}

/*
 * Set up scheduler domains and groups.  For now this just excludes isolated
 * CPUs, but could be used to exclude other special cases in the future.
 */
પૂર્ણાંક __init sched_init_domains(અચલ સંરચના cpumask *cpu_map)
{
	bool multi_llcs;
	પૂર્ણાંક err;

	zalloc_cpumask_var(&sched_domains_llc_id_allocmask, GFP_KERNEL);
	zalloc_cpumask_var(&sched_domains_tmpmask, GFP_KERNEL);
	zalloc_cpumask_var(&sched_domains_tmpmask2, GFP_KERNEL);
	zalloc_cpumask_var(&fallback_doms, GFP_KERNEL);

	arch_update_cpu_topology();
	asym_cpu_capacity_scan();
	ndoms_cur = 1;
	doms_cur = alloc_sched_domains(ndoms_cur);
	જો (!doms_cur)
		doms_cur = &fallback_doms;
	cpumask_and(doms_cur[0], cpu_map, housekeeping_cpumask(HK_TYPE_DOMAIN));
	err = build_sched_domains(doms_cur[0], ખાલી, &multi_llcs);
	જો (!err)
		sched_cache_set(multi_llcs);

	પાછા err;
}

/*
 * Detach sched domains from a group of CPUs specified in cpu_map
 * These CPUs will now be attached to the NULL domain
 */
સ્થિર શૂન્ય detach_destroy_domains(અચલ સંરચના cpumask *cpu_map)
{
	unsigned પૂર્ણાંક cpu = cpumask_any(cpu_map);
	પૂર્ણાંક i;

	જો (rcu_access_pointer(per_cpu(sd_asym_cpucapacity, cpu)))
		static_branch_dec_cpuslocked(&sched_asym_cpucapacity);

	જો (static_branch_unlikely(&sched_cluster_active))
		static_branch_dec_cpuslocked(&sched_cluster_active);

	rcu_read_lock();
	for_each_cpu(i, cpu_map)
		cpu_attach_domain(ખાલી, &def_મૂળ_domain, i);
	rcu_read_unlock();
}

/* handle null as "default" */
સ્થિર પૂર્ણાંક dattrs_equal(સંરચના sched_domain_attr *cur, પૂર્ણાંક idx_cur,
			સંરચના sched_domain_attr *new, પૂર્ણાંક idx_new)
{
	સંરચના sched_domain_attr tmp;

	/* Fast path: */
	જો (!new && !cur)
		પાછા 1;

	tmp = SD_ATTR_INIT;

	પાછા !memcmp(cur ? (cur + idx_cur) : &tmp,
			new ? (new + idx_new) : &tmp,
			કદ(સંરચના sched_domain_attr));
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
સ્થિર શૂન્ય partition_sched_domains_locked(પૂર્ણાંક ndoms_new, cpumask_var_t doms_new[],
				    સંરચના sched_domain_attr *dattr_new)
{
	bool __maybe_unused has_eas = false;
	bool has_multi_llcs = false, multi_llcs;
	પૂર્ણાંક i, j, n;
	પૂર્ણાંક new_topology;

	lockdep_assert_held(&sched_domains_mutex);

	/* Let the architecture update CPU core mappings: */
	new_topology = arch_update_cpu_topology();
	/* Trigger rebuilding CPU capacity asymmetry data */
	જો (new_topology)
		asym_cpu_capacity_scan();

	જો (!doms_new) {
		WARN_ON_ONCE(dattr_new);
		n = 0;
		doms_new = alloc_sched_domains(1);
		જો (doms_new) {
			n = 1;
			cpumask_and(doms_new[0], cpu_active_mask,
				    housekeeping_cpumask(HK_TYPE_DOMAIN));
		}
	} નહીંતર {
		n = ndoms_new;
	}

	/* Destroy deleted domains: */
	માટે (i = 0; i < ndoms_cur; i++) {
		માટે (j = 0; j < n && !new_topology; j++) {
			જો (cpumask_equal(doms_cur[i], doms_new[j]) &&
			    dattrs_equal(dattr_cur, i, dattr_new, j))
				જાઓ match1;
		}
		/* No match - a current sched domain not in new doms_new[] */
		detach_destroy_domains(doms_cur[i]);
match1:
		;
	}

	n = ndoms_cur;
	જો (!doms_new) {
		n = 0;
		doms_new = &fallback_doms;
		cpumask_and(doms_new[0], cpu_active_mask,
			    housekeeping_cpumask(HK_TYPE_DOMAIN));
	}

	/* Build new domains: */
	માટે (i = 0; i < ndoms_new; i++) {
		માટે (j = 0; j < n && !new_topology; j++) {
			જો (cpumask_equal(doms_new[i], doms_cur[j]) &&
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
				સંરચના sched_domain *sd;
				પૂર્ણાંક cpu = cpumask_first(doms_cur[j]);

				guard(rcu)();
				sd = rcu_dereference(cpu_rq(cpu)->sd);
				જ્યારે (sd && sd->parent && (sd->parent->flags & SD_SHARE_LLC))
					sd = sd->parent;
				જો (sd && (sd->flags & SD_SHARE_LLC) && sd->parent &&
				    sd_in_multi_llcs(sd))
					has_multi_llcs = true;
				જાઓ match2;
			}
		}
		/* No match - add a new doms_new */
		build_sched_domains(doms_new[i], dattr_new ? dattr_new + i : ખાલી,
				    &multi_llcs);
		has_multi_llcs |= multi_llcs;
match2:
		;
	}
	sched_cache_set(has_multi_llcs);

#જો defined(CONFIG_ENERGY_MODEL) && defined(CONFIG_CPU_FREQ_GOV_SCHEDUTIL)
	/* Build perf domains: */
	માટે (i = 0; i < ndoms_new; i++) {
		માટે (j = 0; j < n && !sched_energy_update; j++) {
			જો (cpumask_equal(doms_new[i], doms_cur[j]) &&
			    cpu_rq(cpumask_first(doms_cur[j]))->rd->pd) {
				has_eas = true;
				જાઓ match3;
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
	જો (doms_cur != &fallback_doms)
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
શૂન્ય partition_sched_domains(પૂર્ણાંક ndoms_new, cpumask_var_t doms_new[],
			     સંરચના sched_domain_attr *dattr_new)
{
	sched_domains_mutex_lock();
	partition_sched_domains_locked(ndoms_new, doms_new, dattr_new);
	sched_domains_mutex_unlock();
}
