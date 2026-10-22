// SPDX-License-Identifier: GPL-2.0
/*
 * Scheduler topology setup/handling methods
 */

#അന്തര്ഭാവ <linux/sched/isolation.h>
#അന്തര്ഭാവ <linux/sched/clock.h>
#അന്തര്ഭാവ <linux/bsearch.h>
#അന്തര്ഭാവ "sched.h"

DEFINE_MUTEX(sched_domains_mutex);
ശൂന്യം sched_domains_mutex_lock(ശൂന്യം)
{
	mutex_lock(&sched_domains_mutex);
}
ശൂന്യം sched_domains_mutex_unlock(ശൂന്യം)
{
	mutex_unlock(&sched_domains_mutex);
}

/* Protected by sched_domains_mutex: */
സ്ഥിതം cpumask_var_t sched_domains_llc_id_allocmask;
സ്ഥിതം cpumask_var_t sched_domains_tmpmask;
സ്ഥിതം cpumask_var_t sched_domains_tmpmask2;
പൂർണ്ണസംഖ്യ max_lid;

സ്ഥിതം പൂർണ്ണസംഖ്യ __init sched_debug_setup(അക്ഷരം *str)
{
	sched_debug_verbose = true;

	തിരികെ 0;
}
early_param("sched_verbose", sched_debug_setup);

#ifdef CONFIG_SCHED_SMT
സ്ഥിതം bool sched_smt_asym_packing __read_mostly;

സ്ഥിതം പൂർണ്ണസംഖ്യ __init setup_sched_smt_asym_packing(അക്ഷരം *str)
{
	എങ്കിൽ (strcmp(str, "on"))
		തിരികെ 0;

	sched_smt_asym_packing = true;
	തിരികെ 1;
}
__setup("sched_smt_asym_packing=", setup_sched_smt_asym_packing);
#endif

സ്ഥിതം വരിയിൽ bool sched_debug(ശൂന്യം)
{
	തിരികെ sched_debug_verbose;
}

#define SD_FLAG(_name, mflags) [__##_name] = { .meta_flags = mflags, .name = #_name },
സ്ഥിരം ഘടന sd_flag_debug sd_flag_debug[] = {
#അന്തര്ഭാവ <linux/sched/sd_flags.h>
};
#undef SD_FLAG

സ്ഥിതം പൂർണ്ണസംഖ്യ sched_domain_debug_one(ഘടന sched_domain *sd, പൂർണ്ണസംഖ്യ cpu, പൂർണ്ണസംഖ്യ level,
				  ഘടന cpumask *groupmask)
{
	ഘടന sched_group *group = sd->groups;
	unsigned long flags = sd->flags;
	unsigned പൂർണ്ണസംഖ്യ idx;

	cpumask_clear(groupmask);

	printk(KERN_DEBUG "%*s domain-%d: ", level, "", level);
	printk(KERN_CONT "span=%*pbl level=%s\n",
	       cpumask_pr_args(sched_domain_span(sd)), sd->name);

	എങ്കിൽ (!cpumask_test_cpu(cpu, sched_domain_span(sd))) {
		printk(KERN_ERR "ERROR: domain->span does not contain CPU%d\n", cpu);
	}
	എങ്കിൽ (group && !cpumask_test_cpu(cpu, sched_group_span(group))) {
		printk(KERN_ERR "ERROR: domain->groups does not contain CPU%d\n", cpu);
	}

	for_each_set_bit(idx, &flags, __SD_FLAG_CNT) {
		unsigned പൂർണ്ണസംഖ്യ flag = BIT(idx);
		unsigned പൂർണ്ണസംഖ്യ meta_flags = sd_flag_debug[idx].meta_flags;

		എങ്കിൽ ((meta_flags & SDF_SHARED_CHILD) && sd->child &&
		    !(sd->child->flags & flag))
			printk(KERN_ERR "ERROR: flag %s set here but not in child\n",
			       sd_flag_debug[idx].name);

		എങ്കിൽ ((meta_flags & SDF_SHARED_PARENT) && sd->parent &&
		    !(sd->parent->flags & flag))
			printk(KERN_ERR "ERROR: flag %s set here but not in parent\n",
			       sd_flag_debug[idx].name);
	}

	printk(KERN_DEBUG "%*s groups:", level + 1, "");
	ചെയ്യുക {
		എങ്കിൽ (!group) {
			printk("\n");
			printk(KERN_ERR "ERROR: group is NULL\n");
			നിർത്തുക;
		}

		എങ്കിൽ (cpumask_empty(sched_group_span(group))) {
			printk(KERN_CONT "\n");
			printk(KERN_ERR "ERROR: empty group\n");
			നിർത്തുക;
		}

		എങ്കിൽ (!(sd->flags & SD_NUMA) &&
		    cpumask_intersects(groupmask, sched_group_span(group))) {
			printk(KERN_CONT "\n");
			printk(KERN_ERR "ERROR: repeated CPUs\n");
			നിർത്തുക;
		}

		cpumask_or(groupmask, groupmask, sched_group_span(group));

		printk(KERN_CONT " %d:{ span=%*pbl",
				group->sgc->id,
				cpumask_pr_args(sched_group_span(group)));

		എങ്കിൽ ((sd->flags & SD_NUMA) &&
		    !cpumask_equal(group_balance_mask(group), sched_group_span(group))) {
			printk(KERN_CONT " mask=%*pbl",
				cpumask_pr_args(group_balance_mask(group)));
		}

		എങ്കിൽ (group->sgc->capacity != SCHED_CAPACITY_SCALE)
			printk(KERN_CONT " cap=%lu", group->sgc->capacity);

		എങ്കിൽ (group == sd->groups && sd->child &&
		    !cpumask_equal(sched_domain_span(sd->child),
				   sched_group_span(group))) {
			printk(KERN_ERR "ERROR: domain->groups does not match domain->child\n");
		}

		printk(KERN_CONT " }");

		group = group->next;

		എങ്കിൽ (group != sd->groups)
			printk(KERN_CONT ",");

	} ആകുമ്പോൾ (group != sd->groups);
	printk(KERN_CONT "\n");

	എങ്കിൽ (!cpumask_equal(sched_domain_span(sd), groupmask))
		printk(KERN_ERR "ERROR: groups don't span domain->span\n");

	എങ്കിൽ (sd->parent &&
	    !cpumask_subset(groupmask, sched_domain_span(sd->parent)))
		printk(KERN_ERR "ERROR: parent span is not a superset of domain->span\n");
	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം sched_domain_debug(ഘടന sched_domain *sd, പൂർണ്ണസംഖ്യ cpu)
{
	പൂർണ്ണസംഖ്യ level = 0;

	എങ്കിൽ (!sched_debug_verbose)
		തിരികെ;

	എങ്കിൽ (!sd) {
		printk(KERN_DEBUG "CPU%d attaching NULL sched-domain.\n", cpu);
		തിരികെ;
	}

	printk(KERN_DEBUG "CPU%d attaching sched-domain(s):\n", cpu);

	വേണ്ടി (;;) {
		എങ്കിൽ (sched_domain_debug_one(sd, cpu, level, sched_domains_tmpmask))
			നിർത്തുക;
		level++;
		sd = sd->parent;
		എങ്കിൽ (!sd)
			നിർത്തുക;
	}
}

/* Generate a mask of SD flags with the SDF_NEEDS_GROUPS metaflag */
#define SD_FLAG(name, mflags) (name * !!((mflags) & SDF_NEEDS_GROUPS)) |
സ്ഥിതം സ്ഥിരം unsigned പൂർണ്ണസംഖ്യ SD_DEGENERATE_GROUPS_MASK =
#അന്തര്ഭാവ <linux/sched/sd_flags.h>
0;
#undef SD_FLAG

സ്ഥിതം പൂർണ്ണസംഖ്യ sd_degenerate(ഘടന sched_domain *sd)
{
	എങ്കിൽ (cpumask_weight(sched_domain_span(sd)) == 1)
		തിരികെ 1;

	/* Following flags need at least 2 groups */
	എങ്കിൽ ((sd->flags & SD_DEGENERATE_GROUPS_MASK) &&
	    (sd->groups != sd->groups->next))
		തിരികെ 0;

	/* Following flags don't use groups */
	എങ്കിൽ (sd->flags & (SD_WAKE_AFFINE))
		തിരികെ 0;

	തിരികെ 1;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
sd_parent_degenerate(ഘടന sched_domain *sd, ഘടന sched_domain *parent)
{
	unsigned long cflags = sd->flags, pflags = parent->flags;

	എങ്കിൽ (sd_degenerate(parent))
		തിരികെ 1;

	എങ്കിൽ (!cpumask_equal(sched_domain_span(sd), sched_domain_span(parent)))
		തിരികെ 0;

	/* Flags needing groups don't count if only 1 group in parent */
	എങ്കിൽ (parent->groups == parent->groups->next)
		pflags &= ~SD_DEGENERATE_GROUPS_MASK;

	എങ്കിൽ (~cflags & pflags)
		തിരികെ 0;

	തിരികെ 1;
}

#എങ്കിൽ defined(CONFIG_ENERGY_MODEL) && defined(CONFIG_CPU_FREQ_GOV_SCHEDUTIL)
DEFINE_STATIC_താക്കോൽ_FALSE(sched_energy_present);
സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ sysctl_sched_energy_aware = 1;
സ്ഥിതം DEFINE_MUTEX(sched_energy_mutex);
സ്ഥിതം bool sched_energy_update;

സ്ഥിതം bool sched_is_eas_possible(സ്ഥിരം ഘടന cpumask *cpu_mask)
{
	bool any_asym_capacity = false;
	പൂർണ്ണസംഖ്യ i;

	/* EAS is enabled for asymmetric CPU capacity topologies. */
	for_each_cpu(i, cpu_mask) {
		എങ്കിൽ (rcu_access_pointer(per_cpu(sd_asym_cpucapacity, i))) {
			any_asym_capacity = true;
			നിർത്തുക;
		}
	}
	എങ്കിൽ (!any_asym_capacity) {
		എങ്കിൽ (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS, CPUs do not have asymmetric capacities\n",
				cpumask_pr_args(cpu_mask));
		}
		തിരികെ false;
	}

	/* EAS definitely does *not* handle SMT */
	എങ്കിൽ (sched_smt_active()) {
		എങ്കിൽ (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS, SMT is not supported\n",
				cpumask_pr_args(cpu_mask));
		}
		തിരികെ false;
	}

	എങ്കിൽ (!arch_scale_freq_invariant()) {
		എങ്കിൽ (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS: frequency-invariant load tracking not yet supported",
				cpumask_pr_args(cpu_mask));
		}
		തിരികെ false;
	}

	എങ്കിൽ (!cpufreq_ready_for_eas(cpu_mask)) {
		എങ്കിൽ (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS: cpufreq is not ready\n",
				cpumask_pr_args(cpu_mask));
		}
		തിരികെ false;
	}

	തിരികെ true;
}

ശൂന്യം rebuild_sched_domains_energy(ശൂന്യം)
{
	mutex_lock(&sched_energy_mutex);
	sched_energy_update = true;
	rebuild_sched_domains();
	sched_energy_update = false;
	mutex_unlock(&sched_energy_mutex);
}

#ifdef CONFIG_SYSCTL
സ്ഥിതം പൂർണ്ണസംഖ്യ sched_energy_aware_handler(സ്ഥിരം ഘടന ctl_table *table, പൂർണ്ണസംഖ്യ ലിഖ,
		ശൂന്യം *buffer, size_t *lenp, loff_t *ppos)
{
	പൂർണ്ണസംഖ്യ ret;

	എങ്കിൽ (ലിഖ && !capable(CAP_SYS_ADMIN))
		തിരികെ -EPERM;

	എങ്കിൽ (!sched_is_eas_possible(cpu_active_mask)) {
		എങ്കിൽ (ലിഖ) {
			തിരികെ -EOPNOTSUPP;
		} അല്ലെങ്കിൽ {
			*lenp = 0;
			തിരികെ 0;
		}
	}

	ret = proc_dointvec_minmax(table, ലിഖ, buffer, lenp, ppos);
	എങ്കിൽ (!ret && ലിഖ) {
		എങ്കിൽ (sysctl_sched_energy_aware != sched_energy_enabled())
			rebuild_sched_domains_energy();
	}

	തിരികെ ret;
}

സ്ഥിതം സ്ഥിരം ഘടന ctl_table sched_energy_aware_sysctls[] = {
	{
		.procname       = "sched_energy_aware",
		.data           = &sysctl_sched_energy_aware,
		.maxlen         = വലുപ്പം(unsigned പൂർണ്ണസംഖ്യ),
		.mode           = 0644,
		.proc_handler   = sched_energy_aware_handler,
		.extra1         = SYSCTL_ZERO,
		.extra2         = SYSCTL_ONE,
	},
};

സ്ഥിതം പൂർണ്ണസംഖ്യ __init sched_energy_aware_sysctl_init(ശൂന്യം)
{
	register_sysctl_init("kernel", sched_energy_aware_sysctls);
	തിരികെ 0;
}

late_initcall(sched_energy_aware_sysctl_init);
#endif /* CONFIG_SYSCTL */

സ്ഥിതം ശൂന്യം free_pd(ഘടന perf_domain *pd)
{
	ഘടന perf_domain *tmp;

	ആകുമ്പോൾ (pd) {
		tmp = pd->next;
		kfree(pd);
		pd = tmp;
	}
}

സ്ഥിതം ഘടന perf_domain *find_pd(ഘടന perf_domain *pd, പൂർണ്ണസംഖ്യ cpu)
{
	ആകുമ്പോൾ (pd) {
		എങ്കിൽ (cpumask_test_cpu(cpu, perf_domain_span(pd)))
			തിരികെ pd;
		pd = pd->next;
	}

	തിരികെ ശൂന്യ;
}

സ്ഥിതം ഘടന perf_domain *pd_init(പൂർണ്ണസംഖ്യ cpu)
{
	ഘടന em_perf_domain *obj = em_cpu_get(cpu);
	ഘടന perf_domain *pd;

	എങ്കിൽ (!obj) {
		എങ്കിൽ (sched_debug())
			pr_info("%s: no EM found for CPU%d\n", __func__, cpu);
		തിരികെ ശൂന്യ;
	}

	pd = kzalloc_obj(*pd);
	എങ്കിൽ (!pd)
		തിരികെ ശൂന്യ;
	pd->em_pd = obj;

	തിരികെ pd;
}

സ്ഥിതം ശൂന്യം perf_domain_debug(സ്ഥിരം ഘടന cpumask *cpu_map,
						ഘടന perf_domain *pd)
{
	എങ്കിൽ (!sched_debug() || !pd)
		തിരികെ;

	printk(KERN_DEBUG "root_domain %*pbl:", cpumask_pr_args(cpu_map));

	ആകുമ്പോൾ (pd) {
		printk(KERN_CONT " pd%d:{ cpus=%*pbl nr_pstate=%d }",
				cpumask_first(perf_domain_span(pd)),
				cpumask_pr_args(perf_domain_span(pd)),
				em_pd_nr_perf_states(pd->em_pd));
		pd = pd->next;
	}

	printk(KERN_CONT "\n");
}

സ്ഥിതം ശൂന്യം destroy_perf_domain_rcu(ഘടന rcu_head *rp)
{
	ഘടന perf_domain *pd;

	pd = container_of(rp, ഘടന perf_domain, rcu);
	free_pd(pd);
}

സ്ഥിതം ശൂന്യം sched_energy_set(bool has_eas)
{
	എങ്കിൽ (!has_eas && sched_energy_enabled()) {
		എങ്കിൽ (sched_debug())
			pr_info("%s: stopping EAS\n", __func__);
		static_branch_disable_cpuslocked(&sched_energy_present);
	} അല്ലെങ്കിൽ എങ്കിൽ (has_eas && !sched_energy_enabled()) {
		എങ്കിൽ (sched_debug())
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
സ്ഥിതം bool build_perf_domains(സ്ഥിരം ഘടന cpumask *cpu_map)
{
	പൂർണ്ണസംഖ്യ i;
	ഘടന perf_domain *pd = ശൂന്യ, *tmp;
	പൂർണ്ണസംഖ്യ cpu = cpumask_first(cpu_map);
	ഘടന വേര്_domain *rd = cpu_rq(cpu)->rd;

	എങ്കിൽ (!sysctl_sched_energy_aware)
		പോകുക സ്വതന്ത്രം;

	എങ്കിൽ (!sched_is_eas_possible(cpu_map))
		പോകുക സ്വതന്ത്രം;

	for_each_cpu(i, cpu_map) {
		/* Skip already covered CPUs. */
		എങ്കിൽ (find_pd(pd, i))
			തുടരുക;

		/* Create the new pd and add it to the local list. */
		tmp = pd_init(i);
		എങ്കിൽ (!tmp)
			പോകുക സ്വതന്ത്രം;
		tmp->next = pd;
		pd = tmp;
	}

	perf_domain_debug(cpu_map, pd);

	/* Attach the new list of performance domains to the root domain. */
	tmp = rd->pd;
	rcu_assign_pointer(rd->pd, pd);
	എങ്കിൽ (tmp)
		call_rcu(&tmp->rcu, destroy_perf_domain_rcu);

	തിരികെ !!pd;

സ്വതന്ത്രം:
	free_pd(pd);
	tmp = rd->pd;
	rcu_assign_pointer(rd->pd, ശൂന്യ);
	എങ്കിൽ (tmp)
		call_rcu(&tmp->rcu, destroy_perf_domain_rcu);

	തിരികെ false;
}
#അല്ലെങ്കിൽ /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL): */
സ്ഥിതം ശൂന്യം free_pd(ഘടന perf_domain *pd) { }
#endif /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL) */

സ്ഥിതം ശൂന്യം free_rootdomain(ഘടന rcu_head *rcu)
{
	ഘടന വേര്_domain *rd = container_of(rcu, ഘടന വേര്_domain, rcu);

	cpupri_cleanup(&rd->cpupri);
	cpudl_cleanup(&rd->cpudl);
	free_cpumask_var(rd->dlo_mask);
	free_cpumask_var(rd->rto_mask);
	free_cpumask_var(rd->online);
	free_cpumask_var(rd->span);
	free_pd(rd->pd);
	kfree(rd);
}

ശൂന്യം rq_attach_വേര്(ഘടന rq *rq, ഘടന വേര്_domain *rd)
{
	ഘടന വേര്_domain *old_rd = ശൂന്യ;
	ഘടന rq_flags rf;

	rq_lock_irqsave(rq, &rf);

	എങ്കിൽ (rq->rd) {
		old_rd = rq->rd;

		എങ്കിൽ (cpumask_test_cpu(rq->cpu, old_rd->online))
			set_rq_offline(rq);

		cpumask_clear_cpu(rq->cpu, old_rd->span);

		/*
		 * If we don't want to free the old_rd yet then
		 * set old_rd to NULL to skip the freeing later
		 * in this function:
		 */
		എങ്കിൽ (!atomic_dec_and_test(&old_rd->refcount))
			old_rd = ശൂന്യ;
	}

	atomic_inc(&rd->refcount);
	rq->rd = rd;

	cpumask_set_cpu(rq->cpu, rd->span);
	എങ്കിൽ (cpumask_test_cpu(rq->cpu, cpu_active_mask))
		set_rq_online(rq);

	/*
	 * Because the rq is not a task, dl_add_task_root_domain() did not
	 * move the fair server bw to the rd if it already started.
	 * Add it now.
	 */
	എങ്കിൽ (rq->fair_server.dl_server)
		__dl_server_attach_root(&rq->fair_server, rq);

#ifdef CONFIG_SCHED_CLASS_EXT
	എങ്കിൽ (rq->ext_server.dl_server)
		__dl_server_attach_root(&rq->ext_server, rq);
#endif

	rq_unlock_irqrestore(rq, &rf);

	എങ്കിൽ (old_rd)
		call_rcu(&old_rd->rcu, free_rootdomain);
}

ശൂന്യം sched_get_rd(ഘടന വേര്_domain *rd)
{
	atomic_inc(&rd->refcount);
}

ശൂന്യം sched_put_rd(ഘടന വേര്_domain *rd)
{
	എങ്കിൽ (!atomic_dec_and_test(&rd->refcount))
		തിരികെ;

	call_rcu(&rd->rcu, free_rootdomain);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ init_rootdomain(ഘടന വേര്_domain *rd)
{
	എങ്കിൽ (!zalloc_cpumask_var(&rd->span, GFP_KERNEL))
		പോകുക out;
	എങ്കിൽ (!zalloc_cpumask_var(&rd->online, GFP_KERNEL))
		പോകുക free_span;
	എങ്കിൽ (!zalloc_cpumask_var(&rd->dlo_mask, GFP_KERNEL))
		പോകുക free_online;
	എങ്കിൽ (!zalloc_cpumask_var(&rd->rto_mask, GFP_KERNEL))
		പോകുക free_dlo_mask;

#ifdef HAVE_RT_തള്ളുക_IPI
	rd->rto_cpu = -1;
	raw_spin_lock_init(&rd->rto_lock);
	rd->rto_തള്ളുക_work = IRQ_WORK_INIT_HARD(rto_തള്ളുക_irq_work_func);
#endif

	rd->visit_cookie = 0;
	init_dl_bw(&rd->dl_bw);
	എങ്കിൽ (cpudl_init(&rd->cpudl) != 0)
		പോകുക free_rto_mask;

	എങ്കിൽ (cpupri_init(&rd->cpupri) != 0)
		പോകുക free_cpudl;
	തിരികെ 0;

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
	തിരികെ -ENOMEM;
}

/*
 * By default the system creates a single root-domain with all CPUs as
 * members (mimicking the global state we have today).
 */
ഘടന വേര്_domain def_വേര്_domain;

ശൂന്യം __init init_defrootdomain(ശൂന്യം)
{
	init_rootdomain(&def_വേര്_domain);

	atomic_set(&def_വേര്_domain.refcount, 1);
}

സ്ഥിതം ഘടന വേര്_domain *alloc_rootdomain(ശൂന്യം)
{
	ഘടന വേര്_domain *rd;

	rd = kzalloc_obj(*rd);
	എങ്കിൽ (!rd)
		തിരികെ ശൂന്യ;

	എങ്കിൽ (init_rootdomain(rd) != 0) {
		kfree(rd);
		തിരികെ ശൂന്യ;
	}

	തിരികെ rd;
}

സ്ഥിതം ശൂന്യം free_sched_groups(ഘടന sched_group *sg, പൂർണ്ണസംഖ്യ free_sgc)
{
	ഘടന sched_group *tmp, *first;

	എങ്കിൽ (!sg)
		തിരികെ;

	first = sg;
	ചെയ്യുക {
		tmp = sg->next;

		എങ്കിൽ (free_sgc && atomic_dec_and_test(&sg->sgc->ref))
			kfree(sg->sgc);

		എങ്കിൽ (atomic_dec_and_test(&sg->ref))
			kfree(sg);
		sg = tmp;
	} ആകുമ്പോൾ (sg != first);
}

സ്ഥിതം ശൂന്യം free_sched_domain_shared(ഘടന sched_domain_shared *sds)
{
	എങ്കിൽ (sds && atomic_dec_and_test(&sds->ref))
		kfree(sds);
}

സ്ഥിതം ശൂന്യം destroy_sched_domain(ഘടന sched_domain *sd)
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

സ്ഥിതം ശൂന്യം destroy_sched_domains_rcu(ഘടന rcu_head *rcu)
{
	ഘടന sched_domain *sd = container_of(rcu, ഘടന sched_domain, rcu);

	ആകുമ്പോൾ (sd) {
		ഘടന sched_domain *parent = sd->parent;
		destroy_sched_domain(sd);
		sd = parent;
	}
}

സ്ഥിതം ശൂന്യം destroy_sched_domains(ഘടന sched_domain *sd)
{
	എങ്കിൽ (sd)
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
DEFINE_PER_CPU(ഘടന sched_domain __rcu *, sd_llc);
DEFINE_PER_CPU(പൂർണ്ണസംഖ്യ, sd_llc_size);
DEFINE_PER_CPU(പൂർണ്ണസംഖ്യ, sd_llc_id) = -1;
DEFINE_PER_CPU(പൂർണ്ണസംഖ്യ, sd_share_id);
DEFINE_PER_CPU(ഘടന sched_domain_shared __rcu *, sd_llc_shared);
DEFINE_PER_CPU(ഘടന sched_domain_shared __rcu *, sd_balance_shared);
DEFINE_PER_CPU(ഘടന sched_domain __rcu *, sd_numa);
DEFINE_PER_CPU(ഘടന sched_domain __rcu *, sd_asym_packing);
DEFINE_PER_CPU(ഘടന sched_domain __rcu *, sd_asym_cpucapacity);

DEFINE_STATIC_താക്കോൽ_FALSE(sched_asym_cpucapacity);
DEFINE_STATIC_താക്കോൽ_FALSE(sched_cluster_active);

സ്ഥിതം ശൂന്യം update_മുകൾ_cache_domain(പൂർണ്ണസംഖ്യ cpu)
{
	ഘടന sched_domain_shared *sds = ശൂന്യ;
	ഘടന sched_domain *sd;
	പൂർണ്ണസംഖ്യ id = cpu;
	പൂർണ്ണസംഖ്യ size = 1;

	sd = highest_flag_domain(cpu, SD_SHARE_LLC);
	എങ്കിൽ (sd) {
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
	എങ്കിൽ (sd)
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
	എങ്കിൽ (sd && sd->shared)
		sds = sd->shared;

	rcu_assign_pointer(per_cpu(sd_asym_cpucapacity, cpu), sd);
	rcu_assign_pointer(per_cpu(sd_balance_shared, cpu), sds);
}

/*
 * Attach the domain 'sd' to 'cpu' as its base domain. Callers must
 * hold the hotplug lock.
 */
സ്ഥിതം ശൂന്യം
cpu_attach_domain(ഘടന sched_domain *sd, ഘടന വേര്_domain *rd, പൂർണ്ണസംഖ്യ cpu)
{
	ഘടന rq *rq = cpu_rq(cpu);
	ഘടന sched_domain *tmp;

	/* Remove the sched domains which do not contribute to scheduling. */
	വേണ്ടി (tmp = sd; tmp; ) {
		ഘടന sched_domain *parent = tmp->parent;
		എങ്കിൽ (!parent)
			നിർത്തുക;

		എങ്കിൽ (sd_parent_degenerate(tmp, parent)) {
			tmp->parent = parent->parent;

			/* Pick reference to parent->shared. */
			എങ്കിൽ (parent->shared) {
				/*
				 * It is safe to free a sd->shared that
				 * has not been published yet. If a
				 * sd->shared was published, the refcount
				 * will end up being non-zero and it will
				 * not be freed here.
				 */
				free_sched_domain_shared(tmp->shared);
				tmp->shared = parent->shared;
				parent->shared = ശൂന്യ;
			}

			എങ്കിൽ (parent->parent) {
				parent->parent->child = tmp;
				parent->parent->groups->flags = tmp->flags;
			}

			/*
			 * Transfer SD_PREFER_SIBLING down in case of a
			 * degenerate parent; the spans match for this
			 * so the property transfers.
			 */
			എങ്കിൽ (parent->flags & SD_PREFER_SIBLING)
				tmp->flags |= SD_PREFER_SIBLING;
			destroy_sched_domain(parent);
		} അല്ലെങ്കിൽ
			tmp = tmp->parent;
	}

	എങ്കിൽ (sd && sd_degenerate(sd)) {
		tmp = sd;
		sd = sd->parent;

		എങ്കിൽ (sd) {
			ഘടന sched_group *sg = sd->groups;

#ifdef CONFIG_SCHED_CACHE
			/* move buffer to parent as child is being destroyed */
			sd->llc_counts = tmp->llc_counts;
			sd->llc_max = tmp->llc_max;
			sd->llc_bytes = tmp->llc_bytes;
			/* make sure destroy_sched_domain() does not free it */
			tmp->llc_counts = ശൂന്യ;
			tmp->llc_max = 0;
			tmp->llc_bytes = 0;
#endif
			/*
			 * sched groups hold the flags of the child sched
			 * domain for convenience. Clear such flags since
			 * the child is being destroyed.
			 */
			ചെയ്യുക {
				sg->flags = 0;
			} ആകുമ്പോൾ (sg != sd->groups);

			sd->child = ശൂന്യ;
		}

		destroy_sched_domain(tmp);
	}

	sched_domain_debug(sd, cpu);

	rq_attach_വേര്(rq, rd);
	tmp = rq->sd;
	rcu_assign_pointer(rq->sd, sd);
	dirty_sched_domain_sysctl(cpu);
	destroy_sched_domains(tmp);

	update_മുകൾ_cache_domain(cpu);
}

ഘടന s_data {
	ഘടന sched_domain_shared * __percpu *sds;
	ഘടന sched_domain * __percpu *sd;
	ഘടന വേര്_domain	*rd;
};

എണ്ണൽ s_alloc {
	sa_rootdomain,
	sa_sd,
	sa_sd_shared,
	sa_sd_storage,
	sa_none,
};

#ifdef CONFIG_SCHED_CACHE
/* hardware support for cache aware scheduling */
DEFINE_STATIC_താക്കോൽ_FALSE(sched_cache_present);
/*
 * Indicator of whether cache aware scheduling
 * is active, used by the scheduler.
 */
DEFINE_STATIC_താക്കോൽ_FALSE(sched_cache_active);
/* user wants cache aware scheduling [0 or 1] */
പൂർണ്ണസംഖ്യ sysctl_sched_cache_user = 1;

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
സ്ഥിതം unsigned long get_effective_llc_bytes(പൂർണ്ണസംഖ്യ cpu,
					     ഘടന sched_domain *sd)
{
	ഘടന cacheinfo *ci;
	unsigned പൂർണ്ണസംഖ്യ hw_weight;

	ci = get_cpu_cacheinfo_llc(cpu);
	എങ്കിൽ (!ci)
		തിരികെ 0;

	hw_weight = cpumask_weight(&ci->shared_cpu_map);
	എങ്കിൽ (!hw_weight)
		തിരികെ 0;

	തിരികെ div_u64((u64)ci->size * sd->span_weight, hw_weight);
}

സ്ഥിതം bool alloc_sd_llc(സ്ഥിരം ഘടന cpumask *cpu_map,
			 ഘടന s_data *d)
{
	ഘടന sched_domain *sd, *മുകൾ_llc, *parent;
	unsigned പൂർണ്ണസംഖ്യ *p;
	പൂർണ്ണസംഖ്യ i;

	for_each_cpu(i, cpu_map) {
		sd = *per_cpu_ptr(d->sd, i);
		എങ്കിൽ (!sd)
			പോകുക err;

		p = kcalloc_കണ്ണി(max_lid + 1, വലുപ്പം(unsigned പൂർണ്ണസംഖ്യ),
				 GFP_KERNEL, cpu_to_കണ്ണി(i));
		എങ്കിൽ (!p)
			പോകുക err;

		മുകൾ_llc = sd;
		/*
		 * Find the topmost SD_SHARE_LLC domain.
		 * Not yet attached to the CPU, so per_cpu(sd_llc, i)
		 * can not be used.
		 */
		ആകുമ്പോൾ ((parent = rcu_dereference_protected(മുകൾ_llc->parent, true)) &&
		       (parent->flags & SD_SHARE_LLC))
			മുകൾ_llc = parent;

		എങ്കിൽ (മുകൾ_llc->flags & SD_SHARE_LLC) {
			sd->llc_max = max_lid + 1;
			sd->llc_counts = p;
			sd->llc_bytes = get_effective_llc_bytes(i, മുകൾ_llc);
		} അല്ലെങ്കിൽ {
			/* avoid memory leak */
			kfree(p);
		}
	}

	തിരികെ true;
err:
	for_each_cpu(i, cpu_map) {
		sd = *per_cpu_ptr(d->sd, i);
		എങ്കിൽ (sd) {
			kfree(sd->llc_counts);
			sd->llc_counts = ശൂന്യ;
			sd->llc_max = 0;
			sd->llc_bytes = 0;
		}
	}

	തിരികെ false;
}

/*
 * Enable/disable cache aware scheduling according to
 * user input and the presence of hardware support.
 */
സ്ഥിതം ശൂന്യം _sched_cache_active_set(ശൂന്യം)
{
	lockdep_assert_cpus_held();
	lockdep_assert_held(&sched_domains_mutex);

	/* hardware does not support */
	എങ്കിൽ (!static_branch_likely(&sched_cache_present)) {
		static_branch_disable_cpuslocked(&sched_cache_active);
		എങ്കിൽ (sched_debug())
			pr_info("%s: cache aware scheduling not supported on this platform\n", __func__);
		തിരികെ;
	}

	/*
	 * user wants it or not ?
	 * TBD: read before writing the static key.
	 * It is not in the critical path, leave as-is
	 * for now.
	 */
	എങ്കിൽ (sysctl_sched_cache_user) {
		static_branch_enable_cpuslocked(&sched_cache_active);
		എങ്കിൽ (sched_debug())
			pr_info("%s: enabling cache aware scheduling\n", __func__);
	} അല്ലെങ്കിൽ {
		static_branch_disable_cpuslocked(&sched_cache_active);
		എങ്കിൽ (sched_debug())
			pr_info("%s: disabling cache aware scheduling\n", __func__);
	}
}

/* used by debugfs */
ശൂന്യം sched_cache_active_set(ശൂന്യം)
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
ശൂന്യം sched_update_llc_bytes(സ്ഥിരം ഘടന cpumask *cpus)
{
	ഘടന sched_domain *sd, *sdp;
	unsigned പൂർണ്ണസംഖ്യ i;

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
		എങ്കിൽ (!sdp)
			തുടരുക;

		sd = rcu_dereference_sched_domain(cpu_rq(i)->sd);
		എങ്കിൽ (sd)
			sd->llc_bytes = get_effective_llc_bytes(i, sdp);
	}

	sched_domains_mutex_unlock();
}

സ്ഥിതം ശൂന്യം sched_cache_set(bool has_multi_llcs)
{
	/*
	 * TBD: check before writing to it. sched domain rebuild
	 * is not in the critical path, leave as-is for now.
	 */
	എങ്കിൽ (has_multi_llcs)
		static_branch_enable_cpuslocked(&sched_cache_present);
	അല്ലെങ്കിൽ
		static_branch_disable_cpuslocked(&sched_cache_present);

	_sched_cache_active_set();
}
#അല്ലെങ്കിൽ
സ്ഥിതം bool alloc_sd_llc(സ്ഥിരം ഘടന cpumask *cpu_map,
			 ഘടന s_data *d)
{
	തിരികെ false;
}
സ്ഥിതം വരിയിൽ ശൂന്യം sched_cache_set(bool has_multi_llcs) { }
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
സ്ഥിതം bool sd_in_multi_llcs(ഘടന sched_domain *sd)
{
	ഘടന sched_domain *sdp = sd->parent;

	/* it does not make sense to aggregate to 1 CPU */
	എങ്കിൽ (sd->span_weight == 1)
		തിരികെ false;

	ആകുമ്പോൾ (sdp && sdp->span_weight == sd->span_weight)
		sdp = sdp->parent;

	തിരികെ !!sdp;
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
പൂർണ്ണസംഖ്യ group_balance_cpu(ഘടന sched_group *sg)
{
	തിരികെ cpumask_first(group_balance_mask(sg));
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
സ്ഥിതം ശൂന്യം
build_balance_mask(ഘടന sched_domain *sd, ഘടന sched_group *sg, ഘടന cpumask *mask)
{
	സ്ഥിരം ഘടന cpumask *sg_span = sched_group_span(sg);
	ഘടന sd_data *sdd = sd->സ്വകാര്യ;
	ഘടന sched_domain *sibling;
	പൂർണ്ണസംഖ്യ i;

	cpumask_clear(mask);

	for_each_cpu(i, sg_span) {
		sibling = *per_cpu_ptr(sdd->sd, i);

		/*
		 * Can happen in the asymmetric case, where these siblings are
		 * unused. The mask will not be empty because those CPUs that
		 * do have the top domain _should_ span the domain.
		 */
		എങ്കിൽ (!sibling->child)
			തുടരുക;

		/* If we would not end up here, we can't continue from here */
		എങ്കിൽ (!cpumask_equal(sg_span, sched_domain_span(sibling->child)))
			തുടരുക;

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
സ്ഥിതം ഘടന sched_group *
build_group_from_child_sched_domain(ഘടന sched_domain *sd, പൂർണ്ണസംഖ്യ cpu)
{
	ഘടന sched_group *sg;
	ഘടന cpumask *sg_span;

	sg = kzalloc_കണ്ണി(വലുപ്പം(ഘടന sched_group) + cpumask_size(),
			GFP_KERNEL, cpu_to_കണ്ണി(cpu));

	എങ്കിൽ (!sg)
		തിരികെ ശൂന്യ;

	sg_span = sched_group_span(sg);
	എങ്കിൽ (sd->child) {
		cpumask_copy(sg_span, sched_domain_span(sd->child));
		sg->flags = sd->child->flags;
	} അല്ലെങ്കിൽ {
		cpumask_copy(sg_span, sched_domain_span(sd));
	}

	atomic_inc(&sg->ref);
	തിരികെ sg;
}

സ്ഥിതം ശൂന്യം init_overlap_sched_group(ഘടന sched_domain *sd,
				     ഘടന sched_group *sg)
{
	ഘടന cpumask *mask = sched_domains_tmpmask2;
	ഘടന sd_data *sdd = sd->സ്വകാര്യ;
	ഘടന cpumask *sg_span;
	പൂർണ്ണസംഖ്യ cpu;

	build_balance_mask(sd, sg, mask);
	cpu = cpumask_first(mask);

	sg->sgc = *per_cpu_ptr(sdd->sgc, cpu);
	എങ്കിൽ (atomic_inc_return(&sg->sgc->ref) == 1)
		cpumask_copy(group_balance_mask(sg), mask);
	അല്ലെങ്കിൽ
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

സ്ഥിതം ഘടന sched_domain *
find_descended_sibling(ഘടന sched_domain *sd, ഘടന sched_domain *sibling)
{
	/*
	 * The proper descendant would be the one whose child won't span out
	 * of sd
	 */
	ആകുമ്പോൾ (sibling->child &&
	       !cpumask_subset(sched_domain_span(sibling->child),
			       sched_domain_span(sd)))
		sibling = sibling->child;

	/*
	 * As we are referencing sgc across different topology level, we need
	 * to go down to skip those sched_domains which don't contribute to
	 * scheduling because they will be degenerated in cpu_attach_domain
	 */
	ആകുമ്പോൾ (sibling->child &&
	       cpumask_equal(sched_domain_span(sibling->child),
			     sched_domain_span(sibling)))
		sibling = sibling->child;

	തിരികെ sibling;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
build_overlap_sched_groups(ഘടന sched_domain *sd, പൂർണ്ണസംഖ്യ cpu)
{
	ഘടന sched_group *first = ശൂന്യ, *last = ശൂന്യ, *sg;
	സ്ഥിരം ഘടന cpumask *span = sched_domain_span(sd);
	ഘടന cpumask *covered = sched_domains_tmpmask;
	ഘടന sd_data *sdd = sd->സ്വകാര്യ;
	ഘടന sched_domain *sibling;
	പൂർണ്ണസംഖ്യ i;

	cpumask_clear(covered);

	for_each_cpu_wrap(i, span, cpu) {
		ഘടന cpumask *sg_span;

		എങ്കിൽ (cpumask_test_cpu(i, covered))
			തുടരുക;

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
		എങ്കിൽ (!cpumask_test_cpu(i, sched_domain_span(sibling)))
			തുടരുക;

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
		എങ്കിൽ (sibling->child &&
		    !cpumask_subset(sched_domain_span(sibling->child), span))
			sibling = find_descended_sibling(sd, sibling);

		sg = build_group_from_child_sched_domain(sibling, cpu);
		എങ്കിൽ (!sg)
			പോകുക fail;

		sg_span = sched_group_span(sg);
		cpumask_or(covered, covered, sg_span);

		init_overlap_sched_group(sibling, sg);

		എങ്കിൽ (!first)
			first = sg;
		എങ്കിൽ (last)
			last->next = sg;
		last = sg;
		last->next = first;
	}
	sd->groups = first;

	തിരികെ 0;

fail:
	free_sched_groups(first, 0);

	തിരികെ -ENOMEM;
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

സ്ഥിതം ഘടന sched_group *get_group(പൂർണ്ണസംഖ്യ cpu, ഘടന sd_data *sdd)
{
	ഘടന sched_domain *sd = *per_cpu_ptr(sdd->sd, cpu);
	ഘടന sched_domain *child = sd->child;
	ഘടന sched_group *sg;
	bool already_visited;

	എങ്കിൽ (child)
		cpu = cpumask_first(sched_domain_span(child));

	sg = *per_cpu_ptr(sdd->sg, cpu);
	sg->sgc = *per_cpu_ptr(sdd->sgc, cpu);

	/* Increase refcounts for claim_allocations: */
	already_visited = atomic_inc_return(&sg->ref) > 1;
	/* sgc visits should follow a similar trend as sg */
	WARN_ON(already_visited != (atomic_inc_return(&sg->sgc->ref) > 1));

	/* If we have already visited that group, it's already initialized. */
	എങ്കിൽ (already_visited)
		തിരികെ sg;

	എങ്കിൽ (child) {
		cpumask_copy(sched_group_span(sg), sched_domain_span(child));
		cpumask_copy(group_balance_mask(sg), sched_group_span(sg));
		sg->flags = child->flags;
	} അല്ലെങ്കിൽ {
		cpumask_set_cpu(cpu, sched_group_span(sg));
		cpumask_set_cpu(cpu, group_balance_mask(sg));
	}

	sg->sgc->capacity = SCHED_CAPACITY_SCALE * cpumask_weight(sched_group_span(sg));
	sg->sgc->min_capacity = SCHED_CAPACITY_SCALE;
	sg->sgc->max_capacity = SCHED_CAPACITY_SCALE;

	തിരികെ sg;
}

/*
 * build_sched_groups will build a circular linked list of the groups
 * covered by the given span, will set each group's ->cpumask correctly,
 * and will initialize their ->sgc.
 *
 * Assumes the sched_domain tree is fully constructed
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ
build_sched_groups(ഘടന sched_domain *sd, പൂർണ്ണസംഖ്യ cpu)
{
	ഘടന sched_group *first = ശൂന്യ, *last = ശൂന്യ;
	ഘടന sd_data *sdd = sd->സ്വകാര്യ;
	സ്ഥിരം ഘടന cpumask *span = sched_domain_span(sd);
	ഘടന cpumask *covered;
	പൂർണ്ണസംഖ്യ i;

	lockdep_assert_held(&sched_domains_mutex);
	covered = sched_domains_tmpmask;

	cpumask_clear(covered);

	for_each_cpu_wrap(i, span, cpu) {
		ഘടന sched_group *sg;

		എങ്കിൽ (cpumask_test_cpu(i, covered))
			തുടരുക;

		sg = get_group(i, sdd);

		cpumask_or(covered, covered, sched_group_span(sg));

		എങ്കിൽ (!first)
			first = sg;
		എങ്കിൽ (last)
			last->next = sg;
		last = sg;
	}
	last->next = first;
	sd->groups = first;

	തിരികെ 0;
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
സ്ഥിതം ശൂന്യം init_sched_groups_capacity(പൂർണ്ണസംഖ്യ cpu, ഘടന sched_domain *sd)
{
	ഘടന sched_group *sg = sd->groups;
	ഘടന cpumask *mask = sched_domains_tmpmask2;

	WARN_ON(!sg);

	ചെയ്യുക {
		പൂർണ്ണസംഖ്യ cpu, cores = 0, max_cpu = -1;

		sg->group_weight = cpumask_weight(sched_group_span(sg));

		cpumask_copy(mask, sched_group_span(sg));
		for_each_cpu(cpu, mask) {
			cores++;
			cpumask_andnot(mask, mask, cpu_smt_mask(cpu));
		}
		sg->cores = cores;

		എങ്കിൽ (!(sd->flags & SD_ASYM_PACKING))
			പോകുക next;

		for_each_cpu(cpu, sched_group_span(sg)) {
			എങ്കിൽ (max_cpu < 0)
				max_cpu = cpu;
			അല്ലെങ്കിൽ എങ്കിൽ (sched_asym_prefer(cpu, max_cpu))
				max_cpu = cpu;
		}
		sg->asym_prefer_cpu = max_cpu;

next:
		sg = sg->next;
	} ആകുമ്പോൾ (sg != sd->groups);

	എങ്കിൽ (cpu != group_balance_cpu(sg))
		തിരികെ;

	update_group_capacity(sd, cpu);
}

/* Update the "asym_prefer_cpu" when arch_asym_cpu_priority() changes. */
ശൂന്യം sched_update_asym_prefer_cpu(പൂർണ്ണസംഖ്യ cpu, പൂർണ്ണസംഖ്യ old_prio, പൂർണ്ണസംഖ്യ new_prio)
{
	പൂർണ്ണസംഖ്യ asym_prefer_cpu = cpu;
	ഘടന sched_domain *sd;

	guard(rcu)();

	for_each_domain(cpu, sd) {
		ഘടന sched_group *sg;
		പൂർണ്ണസംഖ്യ group_cpu;

		എങ്കിൽ (!(sd->flags & SD_ASYM_PACKING))
			തുടരുക;

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
		എങ്കിൽ (cpu != sg->asym_prefer_cpu) {
			/*
			 * Since the parent is a superset of the current group,
			 * if the cpu is not the "asym_prefer_cpu" at the
			 * current level, it cannot be the preferred CPU at a
			 * higher levels either.
			 */
			എങ്കിൽ (!sched_asym_prefer(cpu, sg->asym_prefer_cpu))
				തിരികെ;

			WRITE_ONCE(sg->asym_prefer_cpu, cpu);
			തുടരുക;
		}

		/* Ranking has improved; CPU is still the preferred one. */
		എങ്കിൽ (new_prio >= old_prio)
			തുടരുക;

		for_each_cpu(group_cpu, sched_group_span(sg)) {
			എങ്കിൽ (sched_asym_prefer(group_cpu, asym_prefer_cpu))
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
സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
asym_cpu_capacity_classify(സ്ഥിരം ഘടന cpumask *sd_span,
			   സ്ഥിരം ഘടന cpumask *cpu_map)
{
	ഘടന asym_cap_data *entry;
	പൂർണ്ണസംഖ്യ എണ്ണം = 0, miss = 0;

	/*
	 * Count how many unique CPU capacities this domain spans across
	 * (compare sched_domain CPUs mask with ones representing  available
	 * CPUs capacities). Take into account CPUs that might be offline:
	 * skip those.
	 */
	list_for_each_entry(entry, &asym_cap_list, link) {
		എങ്കിൽ (cpumask_intersects(sd_span, cpu_capacity_span(entry)))
			++എണ്ണം;
		അല്ലെങ്കിൽ എങ്കിൽ (cpumask_intersects(cpu_map, cpu_capacity_span(entry)))
			++miss;
	}

	WARN_ON_ONCE(!എണ്ണം && !list_empty(&asym_cap_list));

	/* No asymmetry detected */
	എങ്കിൽ (എണ്ണം < 2)
		തിരികെ 0;
	/* Some of the available CPU capacity values have not been detected */
	എങ്കിൽ (miss)
		തിരികെ SD_ASYM_CPUCAPACITY;

	/* Full asymmetry */
	തിരികെ SD_ASYM_CPUCAPACITY | SD_ASYM_CPUCAPACITY_FULL;

}

സ്ഥിതം ശൂന്യം free_asym_cap_entry(ഘടന rcu_head *head)
{
	ഘടന asym_cap_data *entry = container_of(head, ഘടന asym_cap_data, rcu);
	kfree(entry);
}

സ്ഥിതം വരിയിൽ ശൂന്യം asym_cpu_capacity_update_data(പൂർണ്ണസംഖ്യ cpu)
{
	unsigned long capacity = arch_scale_cpu_capacity(cpu);
	ഘടന asym_cap_data *തിരുകുക_entry = ശൂന്യ;
	ഘടന asym_cap_data *entry;

	/*
	 * Search if capacity already exits. If not, track which the entry
	 * where we should insert to keep the list ordered descending.
	 */
	list_for_each_entry(entry, &asym_cap_list, link) {
		എങ്കിൽ (capacity == entry->capacity)
			പോകുക done;
		അല്ലെങ്കിൽ എങ്കിൽ (!തിരുകുക_entry && capacity > entry->capacity)
			തിരുകുക_entry = list_prev_entry(entry, link);
	}

	entry = kzalloc(വലുപ്പം(*entry) + cpumask_size(), GFP_KERNEL);
	എങ്കിൽ (WARN_ONCE(!entry, "Failed to allocate memory for asymmetry data\n"))
		തിരികെ;
	entry->capacity = capacity;

	/* If NULL then the new capacity is the smallest, add last. */
	എങ്കിൽ (!തിരുകുക_entry)
		list_add_tail_rcu(&entry->link, &asym_cap_list);
	അല്ലെങ്കിൽ
		list_add_rcu(&entry->link, &തിരുകുക_entry->link);
done:
	__cpumask_set_cpu(cpu, cpu_capacity_span(entry));
}

/*
 * Build-up/update list of CPUs grouped by their capacities
 * An update requires explicit request to rebuild sched domains
 * with state indicating CPU topology changes.
 */
സ്ഥിതം ശൂന്യം asym_cpu_capacity_scan(ശൂന്യം)
{
	ഘടന asym_cap_data *entry, *next;
	പൂർണ്ണസംഖ്യ cpu;

	list_for_each_entry(entry, &asym_cap_list, link)
		cpumask_clear(cpu_capacity_span(entry));

	for_each_cpu_and(cpu, cpu_possible_mask, housekeeping_cpumask(HK_TYPE_DOMAIN))
		asym_cpu_capacity_update_data(cpu);

	list_for_each_entry_safe(entry, next, &asym_cap_list, link) {
		എങ്കിൽ (cpumask_empty(cpu_capacity_span(entry))) {
			list_del_rcu(&entry->link);
			call_rcu(&entry->rcu, free_asym_cap_entry);
		}
	}

	/*
	 * Only one capacity value has been detected i.e. this system is symmetric.
	 * No need to keep this data around.
	 */
	എങ്കിൽ (list_is_singular(&asym_cap_list)) {
		entry = list_first_entry(&asym_cap_list, typeof(*entry), link);
		list_del_rcu(&entry->link);
		call_rcu(&entry->rcu, free_asym_cap_entry);
	}
}

/*
 * Initializers for schedule domains
 * Non-inlined to reduce accumulated stack pressure in build_sched_domains()
 */

സ്ഥിതം പൂർണ്ണസംഖ്യ default_relax_domain_level = -1;
പൂർണ്ണസംഖ്യ sched_domain_level_max;

സ്ഥിതം പൂർണ്ണസംഖ്യ __init setup_relax_domain_level(അക്ഷരം *str)
{
	എങ്കിൽ (kstrtoint(str, 0, &default_relax_domain_level))
		pr_warn("Unable to set relax_domain_level\n");

	തിരികെ 1;
}
__setup("relax_domain_level=", setup_relax_domain_level);

സ്ഥിതം ശൂന്യം set_domain_attribute(ഘടന sched_domain *sd,
				 ഘടന sched_domain_attr *attr)
{
	പൂർണ്ണസംഖ്യ request;

	എങ്കിൽ (!attr || attr->relax_domain_level < 0) {
		എങ്കിൽ (default_relax_domain_level < 0)
			തിരികെ;
		request = default_relax_domain_level;
	} അല്ലെങ്കിൽ
		request = attr->relax_domain_level;

	എങ്കിൽ (sd->level >= request) {
		/* Turn off idle balance on this domain: */
		sd->flags &= ~(SD_BALANCE_WAKE|SD_BALANCE_NEWIDLE);
	}
}

സ്ഥിതം ശൂന്യം __sdt_free(സ്ഥിരം ഘടന cpumask *cpu_map);
സ്ഥിതം പൂർണ്ണസംഖ്യ __sdt_alloc(സ്ഥിരം ഘടന cpumask *cpu_map);

സ്ഥിതം ശൂന്യം __sds_free(ഘടന s_data *d, സ്ഥിരം ഘടന cpumask *cpu_map);
സ്ഥിതം പൂർണ്ണസംഖ്യ __sds_alloc(ഘടന s_data *d, സ്ഥിരം ഘടന cpumask *cpu_map);

സ്ഥിതം ശൂന്യം __free_domain_allocs(ഘടന s_data *d, എണ്ണൽ s_alloc what,
				 സ്ഥിരം ഘടന cpumask *cpu_map)
{
	തിരഞ്ഞെടുപ്പ് (what) {
	അവസ്ഥ sa_rootdomain:
		എങ്കിൽ (!atomic_read(&d->rd->refcount))
			free_rootdomain(&d->rd->rcu);
		fallthrough;
	അവസ്ഥ sa_sd:
		free_percpu(d->sd);
		fallthrough;
	അവസ്ഥ sa_sd_shared:
		__sds_free(d, cpu_map);
		fallthrough;
	അവസ്ഥ sa_sd_storage:
		__sdt_free(cpu_map);
		fallthrough;
	അവസ്ഥ sa_none:
		നിർത്തുക;
	}
}

സ്ഥിതം എണ്ണൽ s_alloc
__visit_domain_allocation_hell(ഘടന s_data *d, സ്ഥിരം ഘടന cpumask *cpu_map)
{
	memset(d, 0, വലുപ്പം(*d));

	എങ്കിൽ (__sdt_alloc(cpu_map))
		തിരികെ sa_sd_storage;
	എങ്കിൽ (__sds_alloc(d, cpu_map))
		തിരികെ sa_sd_shared;
	d->sd = alloc_percpu(ഘടന sched_domain *);
	എങ്കിൽ (!d->sd)
		തിരികെ sa_sd_shared;
	d->rd = alloc_rootdomain();
	എങ്കിൽ (!d->rd)
		തിരികെ sa_sd;

	തിരികെ sa_rootdomain;
}

/*
 * NULL the sd_data elements we've used to build the sched_domain and
 * sched_group structure so that the subsequent __free_domain_allocs()
 * will not free the data we're using.
 */
സ്ഥിതം ശൂന്യം claim_allocations(പൂർണ്ണസംഖ്യ cpu, ഘടന s_data *d)
{
	ഘടന sched_domain *sd;

	എങ്കിൽ (atomic_read(&(*per_cpu_ptr(d->sds, cpu))->ref))
		*per_cpu_ptr(d->sds, cpu) = ശൂന്യ;

	വേണ്ടി (sd = *per_cpu_ptr(d->sd, cpu); sd; sd = sd->parent) {
		ഘടന sd_data *sdd = sd->സ്വകാര്യ;

		WARN_ON_ONCE(*per_cpu_ptr(sdd->sd, cpu) != sd);
		*per_cpu_ptr(sdd->sd, cpu) = ശൂന്യ;

		എങ്കിൽ (atomic_read(&(*per_cpu_ptr(sdd->sg, cpu))->ref))
			*per_cpu_ptr(sdd->sg, cpu) = ശൂന്യ;

		എങ്കിൽ (atomic_read(&(*per_cpu_ptr(sdd->sgc, cpu))->ref))
			*per_cpu_ptr(sdd->sgc, cpu) = ശൂന്യ;
	}
}

#ifdef CONFIG_NUMA
എണ്ണൽ numa_topology_type sched_numa_topology_type;

/*
 * sched_domains_numa_distance is derived from sched_numa_node_distance
 * and provides a simplified view of NUMA distances used specifically
 * for building NUMA scheduling domains.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ			sched_domains_numa_levels;
സ്ഥിതം പൂർണ്ണസംഖ്യ			sched_numa_കണ്ണി_levels;

പൂർണ്ണസംഖ്യ				sched_max_numa_distance;
സ്ഥിതം പൂർണ്ണസംഖ്യ			*sched_domains_numa_distance;
സ്ഥിതം പൂർണ്ണസംഖ്യ			*sched_numa_കണ്ണി_distance;
സ്ഥിതം ഘടന cpumask		***sched_domains_numa_masks;
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

സ്ഥിതം ഘടന sched_domain *
sd_init(ഘടന sched_domain_topology_level *tl,
	സ്ഥിരം ഘടന cpumask *cpu_map,
	ഘടന sched_domain *child, പൂർണ്ണസംഖ്യ cpu)
{
	ഘടന sd_data *sdd = &tl->data;
	ഘടന sched_domain *sd = *per_cpu_ptr(sdd->sd, cpu);
	പൂർണ്ണസംഖ്യ sd_id, sd_weight, sd_flags = 0;
	ഘടന cpumask *sd_span;
	u64 now = sched_clock();

	sd_span = sched_domain_span(sd);
	cpumask_and(sd_span, cpu_map, tl->mask(tl, cpu));
	sd_weight = cpumask_weight(sd_span);
	sd_id = cpumask_first(sd_span);

	എങ്കിൽ (tl->sd_flags)
		sd_flags = (*tl->sd_flags)();
	എങ്കിൽ (WARN_ONCE(sd_flags & ~TOPOLOGY_SD_FLAGS,
		      "wrong sd_flags in topology description\n"))
		sd_flags &= TOPOLOGY_SD_FLAGS;
#ifdef CONFIG_SCHED_SMT
	എങ്കിൽ (sched_smt_asym_packing && (sd_flags & SD_SHARE_CPUCAPACITY))
		sd_flags |= SD_ASYM_PACKING;
#endif
	sd_flags |= asym_cpu_capacity_classify(sd_span, cpu_map);

	*sd = (ഘടന sched_domain){
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
	എങ്കിൽ (sd->flags & SD_SHARE_CPUCAPACITY) {
		sd->imbalance_pct = 110;

	} അല്ലെങ്കിൽ എങ്കിൽ (sd->flags & SD_SHARE_LLC) {
		sd->imbalance_pct = 117;
		sd->cache_nice_tries = 1;

#ifdef CONFIG_NUMA
	} അല്ലെങ്കിൽ എങ്കിൽ (sd->flags & SD_NUMA) {
		sd->cache_nice_tries = 2;

		sd->flags &= ~SD_PREFER_SIBLING;
		sd->flags |= SD_SERIALIZE;
		എങ്കിൽ (sched_domains_numa_distance[tl->numa_level] > കണ്ണി_reclaim_distance) {
			sd->flags &= ~(SD_BALANCE_EXEC |
				       SD_BALANCE_FORK |
				       SD_WAKE_AFFINE);
		}

#endif /* CONFIG_NUMA */
	} അല്ലെങ്കിൽ {
		sd->cache_nice_tries = 1;
	}

	sd->സ്വകാര്യ = sdd;

	തിരികെ sd;
}

#ifdef CONFIG_SCHED_SMT
പൂർണ്ണസംഖ്യ cpu_smt_flags(ശൂന്യം)
{
	തിരികെ SD_SHARE_CPUCAPACITY | SD_SHARE_LLC;
}

സ്ഥിരം ഘടന cpumask *tl_smt_mask(ഘടന sched_domain_topology_level *tl, പൂർണ്ണസംഖ്യ cpu)
{
	തിരികെ cpu_smt_mask(cpu);
}
#endif

#ifdef CONFIG_SCHED_CLUSTER
പൂർണ്ണസംഖ്യ cpu_cluster_flags(ശൂന്യം)
{
	തിരികെ SD_CLUSTER | SD_SHARE_LLC;
}

സ്ഥിരം ഘടന cpumask *tl_cls_mask(ഘടന sched_domain_topology_level *tl, പൂർണ്ണസംഖ്യ cpu)
{
	തിരികെ cpu_clustergroup_mask(cpu);
}
#endif

#ifdef CONFIG_SCHED_MC
പൂർണ്ണസംഖ്യ cpu_core_flags(ശൂന്യം)
{
	തിരികെ SD_SHARE_LLC;
}

സ്ഥിരം ഘടന cpumask *tl_mc_mask(ഘടന sched_domain_topology_level *tl, പൂർണ്ണസംഖ്യ cpu)
{
	തിരികെ cpu_coregroup_mask(cpu);
}

/*
 * Majority of architectures have LLC at MC domain level with exception
 * such as powerpc. Provide a way for arch to specify where its LLC is
 * if it falls in exception category
 */
# ifndef arch_llc_mask
#define arch_llc_mask(cpu) cpu_coregroup_mask(cpu)
# endif

#അല്ലെങ്കിൽ
#define arch_llc_mask(cpu) cpumask_of(cpu)
#endif

#define llc_mask(cpu) arch_llc_mask(cpu)

സ്ഥിരം ഘടന cpumask *tl_pkg_mask(ഘടന sched_domain_topology_level *tl, പൂർണ്ണസംഖ്യ cpu)
{
	തിരികെ cpu_കണ്ണി_mask(cpu);
}

/*
 * Topology list, bottom-up.
 */
സ്ഥിതം ഘടന sched_domain_topology_level default_topology[] = {
#ifdef CONFIG_SCHED_SMT
	SDTL_INIT(tl_smt_mask, cpu_smt_flags, SMT),
#endif

#ifdef CONFIG_SCHED_CLUSTER
	SDTL_INIT(tl_cls_mask, cpu_cluster_flags, CLS),
#endif

#ifdef CONFIG_SCHED_MC
	SDTL_INIT(tl_mc_mask, cpu_core_flags, MC),
#endif
	SDTL_INIT(tl_pkg_mask, ശൂന്യ, PKG),
	{ ശൂന്യ, },
};

സ്ഥിതം ഘടന sched_domain_topology_level *sched_domain_topology =
	default_topology;
സ്ഥിതം ഘടന sched_domain_topology_level *sched_domain_topology_saved;

#define for_each_sd_topology(tl)			\
	വേണ്ടി (tl = sched_domain_topology; tl->mask; tl++)

ശൂന്യം __init set_sched_topology(ഘടന sched_domain_topology_level *tl)
{
	എങ്കിൽ (WARN_ON_ONCE(sched_smp_initialized))
		തിരികെ;

	sched_domain_topology = tl;
	sched_domain_topology_saved = ശൂന്യ;
}

#ifdef CONFIG_NUMA
സ്ഥിതം പൂർണ്ണസംഖ്യ cpu_numa_flags(ശൂന്യം)
{
	തിരികെ SD_NUMA;
}

സ്ഥിതം സ്ഥിരം ഘടന cpumask *sd_numa_mask(ഘടന sched_domain_topology_level *tl, പൂർണ്ണസംഖ്യ cpu)
{
	തിരികെ sched_domains_numa_masks[tl->numa_level][cpu_to_കണ്ണി(cpu)];
}

സ്ഥിതം ശൂന്യം sched_numa_warn(സ്ഥിരം അക്ഷരം *str)
{
	സ്ഥിതം പൂർണ്ണസംഖ്യ done = false;
	പൂർണ്ണസംഖ്യ i,j;

	എങ്കിൽ (done)
		തിരികെ;

	done = true;

	printk(KERN_WARNING "ERROR: %s\n\n", str);

	വേണ്ടി (i = 0; i < nr_കണ്ണി_ids; i++) {
		printk(KERN_WARNING "  ");
		വേണ്ടി (j = 0; j < nr_കണ്ണി_ids; j++) {
			എങ്കിൽ (!കണ്ണി_state(i, N_CPU) || !കണ്ണി_state(j, N_CPU))
				printk(KERN_CONT "(%02d) ", കണ്ണി_distance(i,j));
			അല്ലെങ്കിൽ
				printk(KERN_CONT " %02d  ", കണ്ണി_distance(i,j));
		}
		printk(KERN_CONT "\n");
	}
	printk(KERN_WARNING "\n");
}

bool find_numa_distance(പൂർണ്ണസംഖ്യ distance)
{
	bool found = false;
	പൂർണ്ണസംഖ്യ i, *distances;

	എങ്കിൽ (distance == കണ്ണി_distance(0, 0))
		തിരികെ true;

	rcu_read_lock();
	distances = rcu_dereference(sched_numa_കണ്ണി_distance);
	എങ്കിൽ (!distances)
		പോകുക unlock;
	വേണ്ടി (i = 0; i < sched_numa_കണ്ണി_levels; i++) {
		എങ്കിൽ (distances[i] == distance) {
			found = true;
			നിർത്തുക;
		}
	}
unlock:
	rcu_read_unlock();

	തിരികെ found;
}

#define for_each_cpu_കണ്ണി_but(n, nbut)		\
	for_each_കണ്ണി_state(n, N_CPU)		\
		എങ്കിൽ (n == nbut)			\
			തുടരുക;		\
		അല്ലെങ്കിൽ

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
സ്ഥിതം ശൂന്യം init_numa_topology_type(പൂർണ്ണസംഖ്യ offline_കണ്ണി)
{
	പൂർണ്ണസംഖ്യ a, b, c, n;

	n = sched_max_numa_distance;

	എങ്കിൽ (sched_domains_numa_levels <= 2) {
		sched_numa_topology_type = NUMA_DIRECT;
		തിരികെ;
	}

	for_each_cpu_കണ്ണി_but(a, offline_കണ്ണി) {
		for_each_cpu_കണ്ണി_but(b, offline_കണ്ണി) {
			/* Find two nodes furthest removed from each other. */
			എങ്കിൽ (കണ്ണി_distance(a, b) < n)
				തുടരുക;

			/* Is there an intermediary node between a and b? */
			for_each_cpu_കണ്ണി_but(c, offline_കണ്ണി) {
				എങ്കിൽ (കണ്ണി_distance(a, c) < n &&
				    കണ്ണി_distance(b, c) < n) {
					sched_numa_topology_type =
							NUMA_GLUELESS_MESH;
					തിരികെ;
				}
			}

			sched_numa_topology_type = NUMA_BACKPLANE;
			തിരികെ;
		}
	}

	pr_err("Failed to find a NUMA topology type, defaulting to DIRECT\n");
	sched_numa_topology_type = NUMA_DIRECT;
}


#define NR_DISTANCE_മൂല്യങ്ങൾ (1 << DISTANCE_BITS)

/*
 * An architecture could modify its NUMA distance, to change
 * grouping of NUMA nodes and number of NUMA levels when creating
 * NUMA level sched domains.
 *
 * A NUMA level is created for each unique
 * arch_sched_node_distance.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ numa_കണ്ണി_dist(പൂർണ്ണസംഖ്യ i, പൂർണ്ണസംഖ്യ j)
{
	തിരികെ കണ്ണി_distance(i, j);
}

പൂർണ്ണസംഖ്യ arch_sched_കണ്ണി_distance(പൂർണ്ണസംഖ്യ from, പൂർണ്ണസംഖ്യ to)
			     __weak __alias(numa_കണ്ണി_dist);

സ്ഥിതം bool modified_sched_കണ്ണി_distance(ശൂന്യം)
{
	തിരികെ numa_കണ്ണി_dist != arch_sched_കണ്ണി_distance;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ sched_record_numa_dist(പൂർണ്ണസംഖ്യ offline_കണ്ണി, പൂർണ്ണസംഖ്യ (*n_dist)(പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ),
				  പൂർണ്ണസംഖ്യ **dist, പൂർണ്ണസംഖ്യ *levels)
{
	unsigned long *distance_map __free(bitmap) = ശൂന്യ;
	പൂർണ്ണസംഖ്യ nr_levels = 0;
	പൂർണ്ണസംഖ്യ i, j;
	പൂർണ്ണസംഖ്യ *distances;

	/*
	 * O(nr_nodes^2) de-duplicating selection sort -- in order to find the
	 * unique distances in the node_distance() table.
	 */
	distance_map = bitmap_alloc(NR_DISTANCE_മൂല്യങ്ങൾ, GFP_KERNEL);
	എങ്കിൽ (!distance_map)
		തിരികെ -ENOMEM;

	bitmap_zero(distance_map, NR_DISTANCE_മൂല്യങ്ങൾ);
	for_each_cpu_കണ്ണി_but(i, offline_കണ്ണി) {
		for_each_cpu_കണ്ണി_but(j, offline_കണ്ണി) {
			പൂർണ്ണസംഖ്യ distance = n_dist(i, j);

			എങ്കിൽ (distance < LOCAL_DISTANCE || distance >= NR_DISTANCE_മൂല്യങ്ങൾ) {
				sched_numa_warn("Invalid distance value range");
				തിരികെ -EINVAL;
			}

			bitmap_set(distance_map, distance, 1);
		}
	}
	/*
	 * We can now figure out how many unique distance values there are and
	 * allocate memory accordingly.
	 */
	nr_levels = bitmap_weight(distance_map, NR_DISTANCE_മൂല്യങ്ങൾ);

	distances = kzalloc_objs(പൂർണ്ണസംഖ്യ, nr_levels);
	എങ്കിൽ (!distances)
		തിരികെ -ENOMEM;

	വേണ്ടി (i = 0, j = 0; i < nr_levels; i++, j++) {
		j = find_next_bit(distance_map, NR_DISTANCE_മൂല്യങ്ങൾ, j);
		distances[i] = j;
	}
	*dist = distances;
	*levels = nr_levels;

	തിരികെ 0;
}

ശൂന്യം sched_init_numa(പൂർണ്ണസംഖ്യ offline_കണ്ണി)
{
	ഘടന sched_domain_topology_level *tl;
	പൂർണ്ണസംഖ്യ nr_levels, nr_കണ്ണി_levels;
	പൂർണ്ണസംഖ്യ i, j;
	പൂർണ്ണസംഖ്യ *distances, *domain_distances;
	ഘടന cpumask ***masks;

	/* Record the NUMA distances from SLIT table */
	എങ്കിൽ (sched_record_numa_dist(offline_കണ്ണി, numa_കണ്ണി_dist, &distances,
				   &nr_കണ്ണി_levels))
		തിരികെ;

	/* Record modified NUMA distances for building sched domains */
	എങ്കിൽ (modified_sched_കണ്ണി_distance()) {
		എങ്കിൽ (sched_record_numa_dist(offline_കണ്ണി, arch_sched_കണ്ണി_distance,
					   &domain_distances, &nr_levels)) {
			kfree(distances);
			തിരികെ;
		}
	} അല്ലെങ്കിൽ {
		domain_distances = distances;
		nr_levels = nr_കണ്ണി_levels;
	}
	rcu_assign_pointer(sched_numa_കണ്ണി_distance, distances);
	WRITE_ONCE(sched_max_numa_distance, distances[nr_കണ്ണി_levels - 1]);
	WRITE_ONCE(sched_numa_കണ്ണി_levels, nr_കണ്ണി_levels);

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

	masks = kzalloc(വലുപ്പം(ശൂന്യം *) * nr_levels, GFP_KERNEL);
	എങ്കിൽ (!masks)
		തിരികെ;

	/*
	 * Now for each level, construct a mask per node which contains all
	 * CPUs of nodes that are that many hops away from us.
	 */
	വേണ്ടി (i = 0; i < nr_levels; i++) {
		masks[i] = kzalloc(nr_കണ്ണി_ids * വലുപ്പം(ശൂന്യം *), GFP_KERNEL);
		എങ്കിൽ (!masks[i])
			തിരികെ;

		for_each_cpu_കണ്ണി_but(j, offline_കണ്ണി) {
			ഘടന cpumask *mask = kzalloc(cpumask_size(), GFP_KERNEL);
			പൂർണ്ണസംഖ്യ k;

			എങ്കിൽ (!mask)
				തിരികെ;

			masks[i][j] = mask;

			for_each_cpu_കണ്ണി_but(k, offline_കണ്ണി) {
				എങ്കിൽ (sched_debug() &&
				    (arch_sched_കണ്ണി_distance(j, k) !=
				     arch_sched_കണ്ണി_distance(k, j)))
					sched_numa_warn("Node-distance not symmetric");

				എങ്കിൽ (arch_sched_കണ്ണി_distance(j, k) >
				    sched_domains_numa_distance[i])
					തുടരുക;

				cpumask_or(mask, mask, cpumask_of_കണ്ണി(k));
			}
		}
	}
	rcu_assign_pointer(sched_domains_numa_masks, masks);

	/* Compute default topology size */
	വേണ്ടി (i = 0; sched_domain_topology[i].mask; i++);

	tl = kzalloc((i + nr_levels + 1) *
			വലുപ്പം(ഘടന sched_domain_topology_level), GFP_KERNEL);
	എങ്കിൽ (!tl)
		തിരികെ;

	/*
	 * Copy the default topology bits..
	 */
	വേണ്ടി (i = 0; sched_domain_topology[i].mask; i++)
		tl[i] = sched_domain_topology[i];

	/*
	 * Add the NUMA identity distance, aka single NODE.
	 */
	tl[i++] = SDTL_INIT(sd_numa_mask, ശൂന്യ, കണ്ണി_2);

	/*
	 * .. and append 'j' levels of NUMA goodness.
	 */
	വേണ്ടി (j = 1; j < nr_levels; i++, j++) {
		tl[i] = SDTL_INIT(sd_numa_mask, cpu_numa_flags, NUMA);
		tl[i].numa_level = j;
	}

	sched_domain_topology_saved = sched_domain_topology;
	sched_domain_topology = tl;

	sched_domains_numa_levels = nr_levels;

	init_numa_topology_type(offline_കണ്ണി);
}


സ്ഥിതം ശൂന്യം sched_reset_numa(ശൂന്യം)
{
	പൂർണ്ണസംഖ്യ nr_levels, *distances, *dom_distances = ശൂന്യ;
	ഘടന cpumask ***masks;

	nr_levels = sched_domains_numa_levels;
	sched_numa_കണ്ണി_levels = 0;
	sched_domains_numa_levels = 0;
	sched_max_numa_distance = 0;
	sched_numa_topology_type = NUMA_DIRECT;
	distances = sched_numa_കണ്ണി_distance;
	എങ്കിൽ (sched_numa_കണ്ണി_distance != sched_domains_numa_distance)
		dom_distances = sched_domains_numa_distance;
	rcu_assign_pointer(sched_numa_കണ്ണി_distance, ശൂന്യ);
	rcu_assign_pointer(sched_domains_numa_distance, ശൂന്യ);
	masks = sched_domains_numa_masks;
	rcu_assign_pointer(sched_domains_numa_masks, ശൂന്യ);
	എങ്കിൽ (distances || masks) {
		പൂർണ്ണസംഖ്യ i, j;

		synchronize_rcu();
		kfree(distances);
		kfree(dom_distances);
		വേണ്ടി (i = 0; i < nr_levels && masks; i++) {
			എങ്കിൽ (!masks[i])
				തുടരുക;
			for_each_കണ്ണി(j)
				kfree(masks[i][j]);
			kfree(masks[i]);
		}
		kfree(masks);
	}
	എങ്കിൽ (sched_domain_topology_saved) {
		kfree(sched_domain_topology);
		sched_domain_topology = sched_domain_topology_saved;
		sched_domain_topology_saved = ശൂന്യ;
	}
}

/*
 * Call with hotplug lock held
 */
ശൂന്യം sched_update_numa(പൂർണ്ണസംഖ്യ cpu, bool online)
{
	പൂർണ്ണസംഖ്യ കണ്ണി;

	കണ്ണി = cpu_to_കണ്ണി(cpu);
	/*
	 * Scheduler NUMA topology is updated when the first CPU of a
	 * node is onlined or the last CPU of a node is offlined.
	 */
	എങ്കിൽ (cpumask_weight(cpumask_of_കണ്ണി(കണ്ണി)) != 1)
		തിരികെ;

	sched_reset_numa();
	sched_init_numa(online ? NUMA_NO_കണ്ണി : കണ്ണി);
}

ശൂന്യം sched_domains_numa_masks_set(unsigned പൂർണ്ണസംഖ്യ cpu)
{
	പൂർണ്ണസംഖ്യ കണ്ണി = cpu_to_കണ്ണി(cpu);
	പൂർണ്ണസംഖ്യ i, j;

	വേണ്ടി (i = 0; i < sched_domains_numa_levels; i++) {
		വേണ്ടി (j = 0; j < nr_കണ്ണി_ids; j++) {
			എങ്കിൽ (!കണ്ണി_state(j, N_CPU))
				തുടരുക;

			/* Set ourselves in the remote node's masks */
			എങ്കിൽ (arch_sched_കണ്ണി_distance(j, കണ്ണി) <=
			    sched_domains_numa_distance[i])
				cpumask_set_cpu(cpu, sched_domains_numa_masks[i][j]);
		}
	}
}

ശൂന്യം sched_domains_numa_masks_clear(unsigned പൂർണ്ണസംഖ്യ cpu)
{
	പൂർണ്ണസംഖ്യ i, j;

	വേണ്ടി (i = 0; i < sched_domains_numa_levels; i++) {
		വേണ്ടി (j = 0; j < nr_കണ്ണി_ids; j++) {
			എങ്കിൽ (sched_domains_numa_masks[i][j])
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
പൂർണ്ണസംഖ്യ sched_numa_find_closest(സ്ഥിരം ഘടന cpumask *cpus, പൂർണ്ണസംഖ്യ cpu)
{
	പൂർണ്ണസംഖ്യ i, j = cpu_to_കണ്ണി(cpu), found = nr_cpu_ids;
	ഘടന cpumask ***masks;

	rcu_read_lock();
	masks = rcu_dereference(sched_domains_numa_masks);
	എങ്കിൽ (!masks)
		പോകുക unlock;
	വേണ്ടി (i = 0; i < sched_domains_numa_levels; i++) {
		എങ്കിൽ (!masks[i][j])
			നിർത്തുക;
		cpu = cpumask_any_and_distribute(cpus, masks[i][j]);
		എങ്കിൽ (cpu < nr_cpu_ids) {
			found = cpu;
			നിർത്തുക;
		}
	}
unlock:
	rcu_read_unlock();

	തിരികെ found;
}

ഘടന __cmp_key {
	സ്ഥിരം ഘടന cpumask *cpus;
	ഘടന cpumask ***masks;
	പൂർണ്ണസംഖ്യ കണ്ണി;
	പൂർണ്ണസംഖ്യ cpu;
	പൂർണ്ണസംഖ്യ w;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ hop_cmp(സ്ഥിരം ശൂന്യം *a, സ്ഥിരം ശൂന്യം *b)
{
	ഘടന cpumask **prev_hop, **cur_hop = *(ഘടന cpumask ***)b;
	ഘടന __cmp_key *k = (ഘടന __cmp_key *)a;

	എങ്കിൽ (cpumask_weight_and(k->cpus, cur_hop[k->കണ്ണി]) <= k->cpu)
		തിരികെ 1;

	എങ്കിൽ (b == k->masks) {
		k->w = 0;
		തിരികെ 0;
	}

	prev_hop = *((ഘടന cpumask ***)b - 1);
	k->w = cpumask_weight_and(k->cpus, prev_hop[k->കണ്ണി]);
	എങ്കിൽ (k->w <= k->cpu)
		തിരികെ 0;

	തിരികെ -1;
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
പൂർണ്ണസംഖ്യ sched_numa_find_nth_cpu(സ്ഥിരം ഘടന cpumask *cpus, പൂർണ്ണസംഖ്യ cpu, പൂർണ്ണസംഖ്യ കണ്ണി)
{
	ഘടന __cmp_key k = { .cpus = cpus, .cpu = cpu };
	ഘടന cpumask ***hop_masks;
	പൂർണ്ണസംഖ്യ hop, ret = nr_cpu_ids;

	എങ്കിൽ (കണ്ണി == NUMA_NO_കണ്ണി)
		തിരികെ cpumask_nth_and(cpu, cpus, cpu_online_mask);

	rcu_read_lock();

	/* CPU-less node entries are uninitialized in sched_domains_numa_masks */
	കണ്ണി = numa_nearest_കണ്ണി(കണ്ണി, N_CPU);
	k.കണ്ണി = കണ്ണി;

	k.masks = rcu_dereference(sched_domains_numa_masks);
	എങ്കിൽ (!k.masks)
		പോകുക unlock;

	hop_masks = bsearch(&k, k.masks, sched_domains_numa_levels, വലുപ്പം(k.masks[0]), hop_cmp);
	എങ്കിൽ (!hop_masks)
		പോകുക unlock;
	hop = hop_masks	- k.masks;

	ret = hop ?
		cpumask_nth_and_andnot(cpu - k.w, cpus, k.masks[hop][കണ്ണി], k.masks[hop-1][കണ്ണി]) :
		cpumask_nth_and(cpu, cpus, k.masks[0][കണ്ണി]);
unlock:
	rcu_read_unlock();
	തിരികെ ret;
}
EXPORT_ചിഹ്നം_GPL(sched_numa_find_nth_cpu);

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
സ്ഥിരം ഘടന cpumask *sched_numa_hop_mask(unsigned പൂർണ്ണസംഖ്യ കണ്ണി, unsigned പൂർണ്ണസംഖ്യ hops)
{
	ഘടന cpumask ***masks;

	എങ്കിൽ (കണ്ണി >= nr_കണ്ണി_ids || hops >= sched_domains_numa_levels)
		തിരികെ ERR_PTR(-EINVAL);

	masks = rcu_dereference(sched_domains_numa_masks);
	എങ്കിൽ (!masks)
		തിരികെ ERR_PTR(-EBUSY);

	തിരികെ masks[hops][കണ്ണി];
}
EXPORT_ചിഹ്നം_GPL(sched_numa_hop_mask);

#endif /* CONFIG_NUMA */

സ്ഥിതം പൂർണ്ണസംഖ്യ __sdt_alloc(സ്ഥിരം ഘടന cpumask *cpu_map)
{
	ഘടന sched_domain_topology_level *tl;
	പൂർണ്ണസംഖ്യ j;

	for_each_sd_topology(tl) {
		ഘടന sd_data *sdd = &tl->data;

		sdd->sd = alloc_percpu(ഘടന sched_domain *);
		എങ്കിൽ (!sdd->sd)
			തിരികെ -ENOMEM;

		sdd->sg = alloc_percpu(ഘടന sched_group *);
		എങ്കിൽ (!sdd->sg)
			തിരികെ -ENOMEM;

		sdd->sgc = alloc_percpu(ഘടന sched_group_capacity *);
		എങ്കിൽ (!sdd->sgc)
			തിരികെ -ENOMEM;

		for_each_cpu(j, cpu_map) {
			ഘടന sched_domain *sd;
			ഘടന sched_group *sg;
			ഘടന sched_group_capacity *sgc;

			sd = kzalloc_കണ്ണി(വലുപ്പം(ഘടന sched_domain) + cpumask_size(),
					GFP_KERNEL, cpu_to_കണ്ണി(j));
			എങ്കിൽ (!sd)
				തിരികെ -ENOMEM;

			*per_cpu_ptr(sdd->sd, j) = sd;

			sg = kzalloc_കണ്ണി(വലുപ്പം(ഘടന sched_group) + cpumask_size(),
					GFP_KERNEL, cpu_to_കണ്ണി(j));
			എങ്കിൽ (!sg)
				തിരികെ -ENOMEM;

			sg->next = sg;

			*per_cpu_ptr(sdd->sg, j) = sg;

			sgc = kzalloc_കണ്ണി(വലുപ്പം(ഘടന sched_group_capacity) + cpumask_size(),
					GFP_KERNEL, cpu_to_കണ്ണി(j));
			എങ്കിൽ (!sgc)
				തിരികെ -ENOMEM;

			sgc->id = j;

			*per_cpu_ptr(sdd->sgc, j) = sgc;
		}
	}

	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം __sdt_free(സ്ഥിരം ഘടന cpumask *cpu_map)
{
	ഘടന sched_domain_topology_level *tl;
	പൂർണ്ണസംഖ്യ j;

	for_each_sd_topology(tl) {
		ഘടന sd_data *sdd = &tl->data;

		for_each_cpu(j, cpu_map) {
			ഘടന sched_domain *sd;

			എങ്കിൽ (sdd->sd) {
				sd = *per_cpu_ptr(sdd->sd, j);
				എങ്കിൽ (sd && (sd->flags & SD_NUMA))
					free_sched_groups(sd->groups, 0);
				kfree(*per_cpu_ptr(sdd->sd, j));
			}

			എങ്കിൽ (sdd->sg)
				kfree(*per_cpu_ptr(sdd->sg, j));
			എങ്കിൽ (sdd->sgc)
				kfree(*per_cpu_ptr(sdd->sgc, j));
		}
		free_percpu(sdd->sd);
		sdd->sd = ശൂന്യ;
		free_percpu(sdd->sg);
		sdd->sg = ശൂന്യ;
		free_percpu(sdd->sgc);
		sdd->sgc = ശൂന്യ;
	}
}

സ്ഥിതം പൂർണ്ണസംഖ്യ __sds_alloc(ഘടന s_data *d, സ്ഥിരം ഘടന cpumask *cpu_map)
{
	പൂർണ്ണസംഖ്യ j;

	d->sds = alloc_percpu(ഘടന sched_domain_shared *);
	എങ്കിൽ (!d->sds)
		തിരികെ -ENOMEM;

	for_each_cpu(j, cpu_map) {
		ഘടന sched_domain_shared *sds;

		sds = kzalloc_കണ്ണി(വലുപ്പം(ഘടന sched_domain_shared),
				GFP_KERNEL, cpu_to_കണ്ണി(j));
		എങ്കിൽ (!sds)
			തിരികെ -ENOMEM;

		*per_cpu_ptr(d->sds, j) = sds;
	}

	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം __sds_free(ഘടന s_data *d, സ്ഥിരം ഘടന cpumask *cpu_map)
{
	പൂർണ്ണസംഖ്യ j;

	എങ്കിൽ (!d->sds)
		തിരികെ;

	for_each_cpu(j, cpu_map)
		kfree(*per_cpu_ptr(d->sds, j));

	free_percpu(d->sds);
	d->sds = ശൂന്യ;
}

സ്ഥിതം ഘടന sched_domain *build_sched_domain(ഘടന sched_domain_topology_level *tl,
		സ്ഥിരം ഘടന cpumask *cpu_map, ഘടന sched_domain_attr *attr,
		ഘടന sched_domain *child, പൂർണ്ണസംഖ്യ cpu)
{
	ഘടന sched_domain *sd = sd_init(tl, cpu_map, child, cpu);

	എങ്കിൽ (child) {
		sd->level = child->level + 1;
		sched_domain_level_max = max(sched_domain_level_max, sd->level);
		child->parent = sd;

		എങ്കിൽ (!cpumask_subset(sched_domain_span(child),
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

	തിരികെ sd;
}

/*
 * Ensure topology masks are sane, i.e. there are no conflicts (overlaps) for
 * any two given CPUs on non-NUMA topology levels.
 */
സ്ഥിതം bool topology_span_sane(സ്ഥിരം ഘടന cpumask *cpu_map)
{
	ഘടന sched_domain_topology_level *tl;
	ഘടന cpumask *covered, *id_seen;
	പൂർണ്ണസംഖ്യ cpu;

	lockdep_assert_held(&sched_domains_mutex);
	covered = sched_domains_tmpmask;
	id_seen = sched_domains_tmpmask2;

	for_each_sd_topology(tl) {
		പൂർണ്ണസംഖ്യ tl_common_flags = 0;

		എങ്കിൽ (tl->sd_flags)
			tl_common_flags = (*tl->sd_flags)();

		/* NUMA levels are allowed to overlap */
		എങ്കിൽ (tl_common_flags & SD_NUMA)
			തുടരുക;

		cpumask_clear(covered);
		cpumask_clear(id_seen);

		/*
		 * Non-NUMA levels cannot partially overlap - they must be either
		 * completely equal or completely disjoint. Otherwise we can end up
		 * breaking the sched_group lists - i.e. a later get_group() pass
		 * breaks the linking done for an earlier span.
		 */
		for_each_cpu(cpu, cpu_map) {
			സ്ഥിരം ഘടന cpumask *tl_cpu_mask = tl->mask(tl, cpu);
			പൂർണ്ണസംഖ്യ id;

			/* lowest bit set in this mask is used as a unique id */
			id = cpumask_first(tl_cpu_mask);

			എങ്കിൽ (cpumask_test_cpu(id, id_seen)) {
				/* First CPU has already been seen, ensure identical spans */
				എങ്കിൽ (!cpumask_equal(tl->mask(tl, id), tl_cpu_mask))
					തിരികെ false;
			} അല്ലെങ്കിൽ {
				/* First CPU hasn't been seen before, ensure it's a completely new span */
				എങ്കിൽ (cpumask_intersects(tl_cpu_mask, covered))
					തിരികെ false;

				cpumask_or(covered, covered, tl_cpu_mask);
				cpumask_set_cpu(id, id_seen);
			}
		}
	}
	തിരികെ true;
}

/*
 * Calculate an allowed NUMA imbalance such that LLCs do not get
 * imbalanced.
 */
സ്ഥിതം ശൂന്യം adjust_numa_imbalance(ഘടന sched_domain *sd_llc)
{
	ഘടന sched_domain *parent;
	unsigned പൂർണ്ണസംഖ്യ imb_span = 1;
	unsigned പൂർണ്ണസംഖ്യ imb = 0;
	unsigned പൂർണ്ണസംഖ്യ nr_llcs;

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
	എങ്കിൽ (nr_llcs == 1)
		imb = sd_llc->parent->span_weight >> 3;
	അല്ലെങ്കിൽ
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
	ആകുമ്പോൾ (parent && !(parent->flags & SD_NUMA))
		parent = parent->parent;

	imb_span = parent ? parent->span_weight : sd_llc->parent->span_weight;

	/* Update the upper remainder of the topology */
	parent = sd_llc->parent;
	ആകുമ്പോൾ (parent) {
		പൂർണ്ണസംഖ്യ factor = max(1U, (parent->span_weight / imb_span));

		parent->imb_numa_nr = imb * factor;
		parent = parent->parent;
	}
}

സ്ഥിതം ശൂന്യം
init_sched_domain_shared(ഘടന s_data *d, ഘടന sched_domain *sd, പൂർണ്ണസംഖ്യ flags)
{
	ഘടന sched_domain_shared *sds = ശൂന്യ;
	പൂർണ്ണസംഖ്യ cpu;

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

		എങ്കിൽ (!sds->alloc_flags ||
		    sd->span_weight == 1 ||
		    sds->alloc_flags == flags) {
			sds->alloc_flags = flags;
			sd->shared = sds;
			നിർത്തുക;
		}
	}

	/*
	 * Use the sd_shared corresponding to the last
	 * CPU in the span if none are avaialable.
	 */
	എങ്കിൽ (WARN_ON_ONCE(!sd->shared))
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
സ്ഥിതം bool claim_asym_sched_domain_shared(ഘടന s_data *d, പൂർണ്ണസംഖ്യ cpu)
{
	ഘടന sched_domain *sd = *per_cpu_ptr(d->sd, cpu);
	ഘടന sched_domain *sd_asym;

	എങ്കിൽ (!sd)
		തിരികെ false;

	sd_asym = sd;
	ആകുമ്പോൾ (sd_asym && !(sd_asym->flags & SD_ASYM_CPUCAPACITY_FULL))
		sd_asym = sd_asym->parent;

	എങ്കിൽ (!sd_asym || (sd_asym->flags & SD_NUMA))
		തിരികെ false;

	init_sched_domain_shared(d, sd_asym, SD_ASYM_CPUCAPACITY);
	തിരികെ true;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ __sched_domains_alloc_llc_id(ശൂന്യം)
{
	പൂർണ്ണസംഖ്യ lid, max;

	lockdep_assert_held(&sched_domains_mutex);

	lid = cpumask_first_zero(sched_domains_llc_id_allocmask);
	/*
	 * llc_id space should never grow larger than the
	 * possible number of CPUs in the system.
	 */
	എങ്കിൽ (lid >= nr_cpu_ids)
		തിരികെ -1;

	__cpumask_set_cpu(lid, sched_domains_llc_id_allocmask);
	max = cpumask_last(sched_domains_llc_id_allocmask);
	എങ്കിൽ (max > max_lid)
		max_lid = max;

	തിരികെ lid;
}

സ്ഥിതം ശൂന്യം __sched_domains_free_llc_id(പൂർണ്ണസംഖ്യ cpu)
{
	പൂർണ്ണസംഖ്യ i, lid, max;

	lockdep_assert_held(&sched_domains_mutex);

	lid = per_cpu(sd_llc_id, cpu);
	എങ്കിൽ (lid == -1 || lid >= nr_cpu_ids)
		തിരികെ;

	per_cpu(sd_llc_id, cpu) = -1;

	for_each_cpu(i, llc_mask(cpu)) {
		/* An online CPU owns the llc_id. */
		എങ്കിൽ (per_cpu(sd_llc_id, i) == lid)
			തിരികെ;
	}

	__cpumask_clear_cpu(lid, sched_domains_llc_id_allocmask);

	max = cpumask_last(sched_domains_llc_id_allocmask);
	/* shrink max lid to save memory */
	എങ്കിൽ (max < max_lid)
		max_lid = max;
}

ശൂന്യം sched_domains_free_llc_id(പൂർണ്ണസംഖ്യ cpu)
{
	sched_domains_mutex_lock();
	__sched_domains_free_llc_id(cpu);
	sched_domains_mutex_unlock();
}

/*
 * Build sched domains for a given set of CPUs and attach the sched domains
 * to the individual CPUs
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ
build_sched_domains(സ്ഥിരം ഘടന cpumask *cpu_map, ഘടന sched_domain_attr *attr,
		    bool *multi_llcs)
{
	എണ്ണൽ s_alloc alloc_state = sa_none;
	bool has_multi_llcs = false;
	ഘടന sched_domain *sd;
	ഘടന s_data d;
	ഘടന rq *rq = ശൂന്യ;
	പൂർണ്ണസംഖ്യ i, ret = -ENOMEM;
	bool has_asym = false;
	bool has_cluster = false;

	എങ്കിൽ (WARN_ON(cpumask_empty(cpu_map)))
		പോകുക error;

	alloc_state = __visit_domain_allocation_hell(&d, cpu_map);
	എങ്കിൽ (alloc_state != sa_rootdomain)
		പോകുക error;

	/* Set up domains for CPUs specified by the cpu_map: */
	for_each_cpu(i, cpu_map) {
		ഘടന sched_domain_topology_level *tl;
		പൂർണ്ണസംഖ്യ lid;

		sd = ശൂന്യ;
		for_each_sd_topology(tl) {

			sd = build_sched_domain(tl, cpu_map, attr, sd, i);

			has_asym |= sd->flags & SD_ASYM_CPUCAPACITY;

			എങ്കിൽ (tl == sched_domain_topology)
				*per_cpu_ptr(d.sd, i) = sd;
			എങ്കിൽ (cpumask_equal(cpu_map, sched_domain_span(sd)))
				നിർത്തുക;
		}

		lid = per_cpu(sd_llc_id, i);
		എങ്കിൽ (lid == -1) {
			/* try to reuse the llc_id of its siblings */
			വേണ്ടി (പൂർണ്ണസംഖ്യ j = cpumask_first(llc_mask(i));
			     j < nr_cpu_ids;
			     j = cpumask_next(j, llc_mask(i))) {
				എങ്കിൽ (i == j)
					തുടരുക;

				lid = per_cpu(sd_llc_id, j);

				എങ്കിൽ (lid != -1) {
					per_cpu(sd_llc_id, i) = lid;

					നിർത്തുക;
				}
			}

			/* a new LLC is detected */
			എങ്കിൽ (lid == -1)
				per_cpu(sd_llc_id, i) = __sched_domains_alloc_llc_id();
		}
	}

	എങ്കിൽ (WARN_ON(!topology_span_sane(cpu_map)))
		പോകുക error;

	/* Build the groups for the domains */
	for_each_cpu(i, cpu_map) {
		വേണ്ടി (sd = *per_cpu_ptr(d.sd, i); sd; sd = sd->parent) {
			sd->span_weight = cpumask_weight(sched_domain_span(sd));
			എങ്കിൽ (sd->flags & SD_NUMA) {
				എങ്കിൽ (build_overlap_sched_groups(sd, i))
					പോകുക error;
			} അല്ലെങ്കിൽ {
				എങ്കിൽ (build_sched_groups(sd, i))
					പോകുക error;
			}
		}
	}

	for_each_cpu(i, cpu_map) {
		sd = *per_cpu_ptr(d.sd, i);
		എങ്കിൽ (!sd)
			തുടരുക;

		എങ്കിൽ (has_asym)
			claim_asym_sched_domain_shared(&d, i);

		/* First, find the topmost SD_SHARE_LLC domain */
		ആകുമ്പോൾ (sd->parent && (sd->parent->flags & SD_SHARE_LLC))
			sd = sd->parent;

		എങ്കിൽ (sd->flags & SD_SHARE_LLC) {
			init_sched_domain_shared(&d, sd, SD_SHARE_LLC);

			/*
			 * In presence of higher domains, adjust the
			 * NUMA imbalance stats for the hierarchy.
			 */
			എങ്കിൽ (sd->parent) {
				എങ്കിൽ (IS_ENABLED(CONFIG_NUMA))
					adjust_numa_imbalance(sd);

				എങ്കിൽ (sd_in_multi_llcs(sd))
					has_multi_llcs = true;
			}
		}
	}

	/* Calculate CPU capacity for physical packages and nodes */
	വേണ്ടി (i = nr_cpumask_bits-1; i >= 0; i--) {
		എങ്കിൽ (!cpumask_test_cpu(i, cpu_map))
			തുടരുക;

		claim_allocations(i, &d);

		വേണ്ടി (sd = *per_cpu_ptr(d.sd, i); sd; sd = sd->parent)
			init_sched_groups_capacity(i, sd);
	}

	alloc_sd_llc(cpu_map, &d);

	/* Attach the domains */
	rcu_read_lock();
	for_each_cpu(i, cpu_map) {
		rq = cpu_rq(i);
		sd = *per_cpu_ptr(d.sd, i);

		cpu_attach_domain(sd, d.rd, i);

		എങ്കിൽ (lowest_flag_domain(i, SD_CLUSTER))
			has_cluster = true;
	}
	rcu_read_unlock();

	എങ്കിൽ (has_asym)
		static_branch_inc_cpuslocked(&sched_asym_cpucapacity);

	എങ്കിൽ (has_cluster)
		static_branch_inc_cpuslocked(&sched_cluster_active);

	എങ്കിൽ (rq && sched_debug_verbose)
		pr_info("root domain span: %*pbl\n", cpumask_pr_args(cpu_map));

	ret = 0;
error:
	*multi_llcs = has_multi_llcs;
	__free_domain_allocs(&d, alloc_state, cpu_map);

	തിരികെ ret;
}

/* Current sched domains: */
സ്ഥിതം cpumask_var_t			*doms_cur;

/* Number of sched domains in 'doms_cur': */
സ്ഥിതം പൂർണ്ണസംഖ്യ				ndoms_cur;

/* Attributes of custom domains in 'doms_cur' */
സ്ഥിതം ഘടന sched_domain_attr		*dattr_cur;

/*
 * Special case: If a kmalloc() of a doms_cur partition (array of
 * cpumask) fails, then fallback to a single sched domain,
 * as determined by the single cpumask fallback_doms.
 */
സ്ഥിതം cpumask_var_t			fallback_doms;

/*
 * arch_update_cpu_topology lets virtualized architectures update the
 * CPU core maps. It is supposed to return 1 if the topology changed
 * or 0 if it stayed the same.
 */
പൂർണ്ണസംഖ്യ __weak arch_update_cpu_topology(ശൂന്യം)
{
	തിരികെ 0;
}

cpumask_var_t *alloc_sched_domains(unsigned പൂർണ്ണസംഖ്യ ndoms)
{
	പൂർണ്ണസംഖ്യ i;
	cpumask_var_t *doms;

	doms = kmalloc_objs(*doms, ndoms);
	എങ്കിൽ (!doms)
		തിരികെ ശൂന്യ;
	വേണ്ടി (i = 0; i < ndoms; i++) {
		എങ്കിൽ (!alloc_cpumask_var(&doms[i], GFP_KERNEL)) {
			free_sched_domains(doms, i);
			തിരികെ ശൂന്യ;
		}
	}
	തിരികെ doms;
}

ശൂന്യം free_sched_domains(cpumask_var_t doms[], unsigned പൂർണ്ണസംഖ്യ ndoms)
{
	unsigned പൂർണ്ണസംഖ്യ i;
	വേണ്ടി (i = 0; i < ndoms; i++)
		free_cpumask_var(doms[i]);
	kfree(doms);
}

/*
 * Set up scheduler domains and groups.  For now this just excludes isolated
 * CPUs, but could be used to exclude other special cases in the future.
 */
പൂർണ്ണസംഖ്യ __init sched_init_domains(സ്ഥിരം ഘടന cpumask *cpu_map)
{
	bool multi_llcs;
	പൂർണ്ണസംഖ്യ err;

	zalloc_cpumask_var(&sched_domains_llc_id_allocmask, GFP_KERNEL);
	zalloc_cpumask_var(&sched_domains_tmpmask, GFP_KERNEL);
	zalloc_cpumask_var(&sched_domains_tmpmask2, GFP_KERNEL);
	zalloc_cpumask_var(&fallback_doms, GFP_KERNEL);

	arch_update_cpu_topology();
	asym_cpu_capacity_scan();
	ndoms_cur = 1;
	doms_cur = alloc_sched_domains(ndoms_cur);
	എങ്കിൽ (!doms_cur)
		doms_cur = &fallback_doms;
	cpumask_and(doms_cur[0], cpu_map, housekeeping_cpumask(HK_TYPE_DOMAIN));
	err = build_sched_domains(doms_cur[0], ശൂന്യ, &multi_llcs);
	എങ്കിൽ (!err)
		sched_cache_set(multi_llcs);

	തിരികെ err;
}

/*
 * Detach sched domains from a group of CPUs specified in cpu_map
 * These CPUs will now be attached to the NULL domain
 */
സ്ഥിതം ശൂന്യം detach_destroy_domains(സ്ഥിരം ഘടന cpumask *cpu_map)
{
	unsigned പൂർണ്ണസംഖ്യ cpu = cpumask_any(cpu_map);
	പൂർണ്ണസംഖ്യ i;

	എങ്കിൽ (rcu_access_pointer(per_cpu(sd_asym_cpucapacity, cpu)))
		static_branch_dec_cpuslocked(&sched_asym_cpucapacity);

	എങ്കിൽ (static_branch_unlikely(&sched_cluster_active))
		static_branch_dec_cpuslocked(&sched_cluster_active);

	rcu_read_lock();
	for_each_cpu(i, cpu_map)
		cpu_attach_domain(ശൂന്യ, &def_വേര്_domain, i);
	rcu_read_unlock();
}

/* handle null as "default" */
സ്ഥിതം പൂർണ്ണസംഖ്യ dattrs_equal(ഘടന sched_domain_attr *cur, പൂർണ്ണസംഖ്യ idx_cur,
			ഘടന sched_domain_attr *new, പൂർണ്ണസംഖ്യ idx_new)
{
	ഘടന sched_domain_attr tmp;

	/* Fast path: */
	എങ്കിൽ (!new && !cur)
		തിരികെ 1;

	tmp = SD_ATTR_INIT;

	തിരികെ !memcmp(cur ? (cur + idx_cur) : &tmp,
			new ? (new + idx_new) : &tmp,
			വലുപ്പം(ഘടന sched_domain_attr));
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
സ്ഥിതം ശൂന്യം partition_sched_domains_locked(പൂർണ്ണസംഖ്യ ndoms_new, cpumask_var_t doms_new[],
				    ഘടന sched_domain_attr *dattr_new)
{
	bool __maybe_unused has_eas = false;
	bool has_multi_llcs = false, multi_llcs;
	പൂർണ്ണസംഖ്യ i, j, n;
	പൂർണ്ണസംഖ്യ new_topology;

	lockdep_assert_held(&sched_domains_mutex);

	/* Let the architecture update CPU core mappings: */
	new_topology = arch_update_cpu_topology();
	/* Trigger rebuilding CPU capacity asymmetry data */
	എങ്കിൽ (new_topology)
		asym_cpu_capacity_scan();

	എങ്കിൽ (!doms_new) {
		WARN_ON_ONCE(dattr_new);
		n = 0;
		doms_new = alloc_sched_domains(1);
		എങ്കിൽ (doms_new) {
			n = 1;
			cpumask_and(doms_new[0], cpu_active_mask,
				    housekeeping_cpumask(HK_TYPE_DOMAIN));
		}
	} അല്ലെങ്കിൽ {
		n = ndoms_new;
	}

	/* Destroy deleted domains: */
	വേണ്ടി (i = 0; i < ndoms_cur; i++) {
		വേണ്ടി (j = 0; j < n && !new_topology; j++) {
			എങ്കിൽ (cpumask_equal(doms_cur[i], doms_new[j]) &&
			    dattrs_equal(dattr_cur, i, dattr_new, j))
				പോകുക match1;
		}
		/* No match - a current sched domain not in new doms_new[] */
		detach_destroy_domains(doms_cur[i]);
match1:
		;
	}

	n = ndoms_cur;
	എങ്കിൽ (!doms_new) {
		n = 0;
		doms_new = &fallback_doms;
		cpumask_and(doms_new[0], cpu_active_mask,
			    housekeeping_cpumask(HK_TYPE_DOMAIN));
	}

	/* Build new domains: */
	വേണ്ടി (i = 0; i < ndoms_new; i++) {
		വേണ്ടി (j = 0; j < n && !new_topology; j++) {
			എങ്കിൽ (cpumask_equal(doms_new[i], doms_cur[j]) &&
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
				ഘടന sched_domain *sd;
				പൂർണ്ണസംഖ്യ cpu = cpumask_first(doms_cur[j]);

				guard(rcu)();
				sd = rcu_dereference(cpu_rq(cpu)->sd);
				ആകുമ്പോൾ (sd && sd->parent && (sd->parent->flags & SD_SHARE_LLC))
					sd = sd->parent;
				എങ്കിൽ (sd && (sd->flags & SD_SHARE_LLC) && sd->parent &&
				    sd_in_multi_llcs(sd))
					has_multi_llcs = true;
				പോകുക match2;
			}
		}
		/* No match - add a new doms_new */
		build_sched_domains(doms_new[i], dattr_new ? dattr_new + i : ശൂന്യ,
				    &multi_llcs);
		has_multi_llcs |= multi_llcs;
match2:
		;
	}
	sched_cache_set(has_multi_llcs);

#എങ്കിൽ defined(CONFIG_ENERGY_MODEL) && defined(CONFIG_CPU_FREQ_GOV_SCHEDUTIL)
	/* Build perf domains: */
	വേണ്ടി (i = 0; i < ndoms_new; i++) {
		വേണ്ടി (j = 0; j < n && !sched_energy_update; j++) {
			എങ്കിൽ (cpumask_equal(doms_new[i], doms_cur[j]) &&
			    cpu_rq(cpumask_first(doms_cur[j]))->rd->pd) {
				has_eas = true;
				പോകുക match3;
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
	എങ്കിൽ (doms_cur != &fallback_doms)
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
ശൂന്യം partition_sched_domains(പൂർണ്ണസംഖ്യ ndoms_new, cpumask_var_t doms_new[],
			     ഘടന sched_domain_attr *dattr_new)
{
	sched_domains_mutex_lock();
	partition_sched_domains_locked(ndoms_new, doms_new, dattr_new);
	sched_domains_mutex_unlock();
}
