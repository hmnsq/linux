// SPDX-License-Identifier: GPL-2.0
/*
 * Scheduler topology setup/handling methods
 */

#अन्तर्भाव <linux/sched/isolation.h>
#अन्तर्भाव <linux/sched/clock.h>
#अन्तर्भाव <linux/bsearch.h>
#अन्तर्भाव "sched.h"

DEFINE_MUTEX(sched_domains_mutex);
शून्य sched_domains_mutex_lock(शून्य)
{
	mutex_lock(&sched_domains_mutex);
}
शून्य sched_domains_mutex_unlock(शून्य)
{
	mutex_unlock(&sched_domains_mutex);
}

/* Protected by sched_domains_mutex: */
स्थिर cpumask_var_t sched_domains_llc_id_allocmask;
स्थिर cpumask_var_t sched_domains_tmpmask;
स्थिर cpumask_var_t sched_domains_tmpmask2;
पूर्णाङ्क max_lid;

स्थिर पूर्णाङ्क __init sched_debug_setup(वर्ण *str)
{
	sched_debug_verbose = true;

	प्रत्यागम 0;
}
early_param("sched_verbose", sched_debug_setup);

#ifdef CONFIG_SCHED_SMT
स्थिर bool sched_smt_asym_packing __read_mostly;

स्थिर पूर्णाङ्क __init setup_sched_smt_asym_packing(वर्ण *str)
{
	जइ (strcmp(str, "on"))
		प्रत्यागम 0;

	sched_smt_asym_packing = true;
	प्रत्यागम 1;
}
__setup("sched_smt_asym_packing=", setup_sched_smt_asym_packing);
#endif

स्थिर अन्तःस्थ bool sched_debug(शून्य)
{
	प्रत्यागम sched_debug_verbose;
}

#define SD_FLAG(_name, mflags) [__##_name] = { .meta_flags = mflags, .name = #_name },
ध्रुव संरचना sd_flag_debug sd_flag_debug[] = {
#अन्तर्भाव <linux/sched/sd_flags.h>
};
#undef SD_FLAG

स्थिर पूर्णाङ्क sched_domain_debug_one(संरचना sched_domain *sd, पूर्णाङ्क cpu, पूर्णाङ्क level,
				  संरचना cpumask *groupmask)
{
	संरचना sched_group *group = sd->groups;
	unsigned long flags = sd->flags;
	unsigned पूर्णाङ्क idx;

	cpumask_clear(groupmask);

	printk(KERN_DEBUG "%*s domain-%d: ", level, "", level);
	printk(KERN_CONT "span=%*pbl level=%s\n",
	       cpumask_pr_args(sched_domain_span(sd)), sd->name);

	जइ (!cpumask_test_cpu(cpu, sched_domain_span(sd))) {
		printk(KERN_ERR "ERROR: domain->span does not contain CPU%d\n", cpu);
	}
	जइ (group && !cpumask_test_cpu(cpu, sched_group_span(group))) {
		printk(KERN_ERR "ERROR: domain->groups does not contain CPU%d\n", cpu);
	}

	for_each_set_bit(idx, &flags, __SD_FLAG_CNT) {
		unsigned पूर्णाङ्क flag = BIT(idx);
		unsigned पूर्णाङ्क meta_flags = sd_flag_debug[idx].meta_flags;

		जइ ((meta_flags & SDF_SHARED_CHILD) && sd->child &&
		    !(sd->child->flags & flag))
			printk(KERN_ERR "ERROR: flag %s set here but not in child\n",
			       sd_flag_debug[idx].name);

		जइ ((meta_flags & SDF_SHARED_PARENT) && sd->parent &&
		    !(sd->parent->flags & flag))
			printk(KERN_ERR "ERROR: flag %s set here but not in parent\n",
			       sd_flag_debug[idx].name);
	}

	printk(KERN_DEBUG "%*s groups:", level + 1, "");
	करेहि {
		जइ (!group) {
			printk("\n");
			printk(KERN_ERR "ERROR: group is NULL\n");
			भङ्ग;
		}

		जइ (cpumask_empty(sched_group_span(group))) {
			printk(KERN_CONT "\n");
			printk(KERN_ERR "ERROR: empty group\n");
			भङ्ग;
		}

		जइ (!(sd->flags & SD_NUMA) &&
		    cpumask_intersects(groupmask, sched_group_span(group))) {
			printk(KERN_CONT "\n");
			printk(KERN_ERR "ERROR: repeated CPUs\n");
			भङ्ग;
		}

		cpumask_or(groupmask, groupmask, sched_group_span(group));

		printk(KERN_CONT " %d:{ span=%*pbl",
				group->sgc->id,
				cpumask_pr_args(sched_group_span(group)));

		जइ ((sd->flags & SD_NUMA) &&
		    !cpumask_equal(group_balance_mask(group), sched_group_span(group))) {
			printk(KERN_CONT " mask=%*pbl",
				cpumask_pr_args(group_balance_mask(group)));
		}

		जइ (group->sgc->capacity != SCHED_CAPACITY_SCALE)
			printk(KERN_CONT " cap=%lu", group->sgc->capacity);

		जइ (group == sd->groups && sd->child &&
		    !cpumask_equal(sched_domain_span(sd->child),
				   sched_group_span(group))) {
			printk(KERN_ERR "ERROR: domain->groups does not match domain->child\n");
		}

		printk(KERN_CONT " }");

		group = group->next;

		जइ (group != sd->groups)
			printk(KERN_CONT ",");

	} जाव (group != sd->groups);
	printk(KERN_CONT "\n");

	जइ (!cpumask_equal(sched_domain_span(sd), groupmask))
		printk(KERN_ERR "ERROR: groups don't span domain->span\n");

	जइ (sd->parent &&
	    !cpumask_subset(groupmask, sched_domain_span(sd->parent)))
		printk(KERN_ERR "ERROR: parent span is not a superset of domain->span\n");
	प्रत्यागम 0;
}

स्थिर शून्य sched_domain_debug(संरचना sched_domain *sd, पूर्णाङ्क cpu)
{
	पूर्णाङ्क level = 0;

	जइ (!sched_debug_verbose)
		प्रत्यागम;

	जइ (!sd) {
		printk(KERN_DEBUG "CPU%d attaching NULL sched-domain.\n", cpu);
		प्रत्यागम;
	}

	printk(KERN_DEBUG "CPU%d attaching sched-domain(s):\n", cpu);

	प्रति (;;) {
		जइ (sched_domain_debug_one(sd, cpu, level, sched_domains_tmpmask))
			भङ्ग;
		level++;
		sd = sd->parent;
		जइ (!sd)
			भङ्ग;
	}
}

/* Generate a mask of SD flags with the SDF_NEEDS_GROUPS metaflag */
#define SD_FLAG(name, mflags) (name * !!((mflags) & SDF_NEEDS_GROUPS)) |
स्थिर ध्रुव unsigned पूर्णाङ्क SD_DEGENERATE_GROUPS_MASK =
#अन्तर्भाव <linux/sched/sd_flags.h>
0;
#undef SD_FLAG

स्थिर पूर्णाङ्क sd_degenerate(संरचना sched_domain *sd)
{
	जइ (cpumask_weight(sched_domain_span(sd)) == 1)
		प्रत्यागम 1;

	/* Following flags need at least 2 groups */
	जइ ((sd->flags & SD_DEGENERATE_GROUPS_MASK) &&
	    (sd->groups != sd->groups->next))
		प्रत्यागम 0;

	/* Following flags don't use groups */
	जइ (sd->flags & (SD_WAKE_AFFINE))
		प्रत्यागम 0;

	प्रत्यागम 1;
}

स्थिर पूर्णाङ्क
sd_parent_degenerate(संरचना sched_domain *sd, संरचना sched_domain *parent)
{
	unsigned long cflags = sd->flags, pflags = parent->flags;

	जइ (sd_degenerate(parent))
		प्रत्यागम 1;

	जइ (!cpumask_equal(sched_domain_span(sd), sched_domain_span(parent)))
		प्रत्यागम 0;

	/* Flags needing groups don't count if only 1 group in parent */
	जइ (parent->groups == parent->groups->next)
		pflags &= ~SD_DEGENERATE_GROUPS_MASK;

	जइ (~cflags & pflags)
		प्रत्यागम 0;

	प्रत्यागम 1;
}

#जइ defined(CONFIG_ENERGY_MODEL) && defined(CONFIG_CPU_FREQ_GOV_SCHEDUTIL)
DEFINE_STATIC_कुञ्चिका_FALSE(sched_energy_present);
स्थिर unsigned पूर्णाङ्क sysctl_sched_energy_aware = 1;
स्थिर DEFINE_MUTEX(sched_energy_mutex);
स्थिर bool sched_energy_update;

स्थिर bool sched_is_eas_possible(ध्रुव संरचना cpumask *cpu_mask)
{
	bool any_asym_capacity = false;
	पूर्णाङ्क i;

	/* EAS is enabled for asymmetric CPU capacity topologies. */
	for_each_cpu(i, cpu_mask) {
		जइ (rcu_access_pointer(per_cpu(sd_asym_cpucapacity, i))) {
			any_asym_capacity = true;
			भङ्ग;
		}
	}
	जइ (!any_asym_capacity) {
		जइ (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS, CPUs do not have asymmetric capacities\n",
				cpumask_pr_args(cpu_mask));
		}
		प्रत्यागम false;
	}

	/* EAS definitely does *not* handle SMT */
	जइ (sched_smt_active()) {
		जइ (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS, SMT is not supported\n",
				cpumask_pr_args(cpu_mask));
		}
		प्रत्यागम false;
	}

	जइ (!arch_scale_freq_invariant()) {
		जइ (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS: frequency-invariant load tracking not yet supported",
				cpumask_pr_args(cpu_mask));
		}
		प्रत्यागम false;
	}

	जइ (!cpufreq_ready_for_eas(cpu_mask)) {
		जइ (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS: cpufreq is not ready\n",
				cpumask_pr_args(cpu_mask));
		}
		प्रत्यागम false;
	}

	प्रत्यागम true;
}

शून्य rebuild_sched_domains_energy(शून्य)
{
	mutex_lock(&sched_energy_mutex);
	sched_energy_update = true;
	rebuild_sched_domains();
	sched_energy_update = false;
	mutex_unlock(&sched_energy_mutex);
}

#ifdef CONFIG_SYSCTL
स्थिर पूर्णाङ्क sched_energy_aware_handler(ध्रुव संरचना ctl_table *table, पूर्णाङ्क लिख,
		शून्य *buffer, size_t *lenp, loff_t *ppos)
{
	पूर्णाङ्क ret;

	जइ (लिख && !capable(CAP_SYS_ADMIN))
		प्रत्यागम -EPERM;

	जइ (!sched_is_eas_possible(cpu_active_mask)) {
		जइ (लिख) {
			प्रत्यागम -EOPNOTSUPP;
		} अण्णहा {
			*lenp = 0;
			प्रत्यागम 0;
		}
	}

	ret = proc_dointvec_minmax(table, लिख, buffer, lenp, ppos);
	जइ (!ret && लिख) {
		जइ (sysctl_sched_energy_aware != sched_energy_enabled())
			rebuild_sched_domains_energy();
	}

	प्रत्यागम ret;
}

स्थिर ध्रुव संरचना ctl_table sched_energy_aware_sysctls[] = {
	{
		.procname       = "sched_energy_aware",
		.data           = &sysctl_sched_energy_aware,
		.maxlen         = परिमाण(unsigned पूर्णाङ्क),
		.mode           = 0644,
		.proc_handler   = sched_energy_aware_handler,
		.extra1         = SYSCTL_ZERO,
		.extra2         = SYSCTL_ONE,
	},
};

स्थिर पूर्णाङ्क __init sched_energy_aware_sysctl_init(शून्य)
{
	register_sysctl_init("kernel", sched_energy_aware_sysctls);
	प्रत्यागम 0;
}

late_initcall(sched_energy_aware_sysctl_init);
#endif /* CONFIG_SYSCTL */

स्थिर शून्य free_pd(संरचना perf_domain *pd)
{
	संरचना perf_domain *tmp;

	जाव (pd) {
		tmp = pd->next;
		kfree(pd);
		pd = tmp;
	}
}

स्थिर संरचना perf_domain *find_pd(संरचना perf_domain *pd, पूर्णाङ्क cpu)
{
	जाव (pd) {
		जइ (cpumask_test_cpu(cpu, perf_domain_span(pd)))
			प्रत्यागम pd;
		pd = pd->next;
	}

	प्रत्यागम रिक्त;
}

स्थिर संरचना perf_domain *pd_init(पूर्णाङ्क cpu)
{
	संरचना em_perf_domain *obj = em_cpu_get(cpu);
	संरचना perf_domain *pd;

	जइ (!obj) {
		जइ (sched_debug())
			pr_info("%s: no EM found for CPU%d\n", __func__, cpu);
		प्रत्यागम रिक्त;
	}

	pd = kzalloc_obj(*pd);
	जइ (!pd)
		प्रत्यागम रिक्त;
	pd->em_pd = obj;

	प्रत्यागम pd;
}

स्थिर शून्य perf_domain_debug(ध्रुव संरचना cpumask *cpu_map,
						संरचना perf_domain *pd)
{
	जइ (!sched_debug() || !pd)
		प्रत्यागम;

	printk(KERN_DEBUG "root_domain %*pbl:", cpumask_pr_args(cpu_map));

	जाव (pd) {
		printk(KERN_CONT " pd%d:{ cpus=%*pbl nr_pstate=%d }",
				cpumask_first(perf_domain_span(pd)),
				cpumask_pr_args(perf_domain_span(pd)),
				em_pd_nr_perf_states(pd->em_pd));
		pd = pd->next;
	}

	printk(KERN_CONT "\n");
}

स्थिर शून्य destroy_perf_domain_rcu(संरचना rcu_head *rp)
{
	संरचना perf_domain *pd;

	pd = container_of(rp, संरचना perf_domain, rcu);
	free_pd(pd);
}

स्थिर शून्य sched_energy_set(bool has_eas)
{
	जइ (!has_eas && sched_energy_enabled()) {
		जइ (sched_debug())
			pr_info("%s: stopping EAS\n", __func__);
		static_branch_disable_cpuslocked(&sched_energy_present);
	} अण्णहा जइ (has_eas && !sched_energy_enabled()) {
		जइ (sched_debug())
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
स्थिर bool build_perf_domains(ध्रुव संरचना cpumask *cpu_map)
{
	पूर्णाङ्क i;
	संरचना perf_domain *pd = रिक्त, *tmp;
	पूर्णाङ्क cpu = cpumask_first(cpu_map);
	संरचना मूलम्_domain *rd = cpu_rq(cpu)->rd;

	जइ (!sysctl_sched_energy_aware)
		गच्छ मुक्त;

	जइ (!sched_is_eas_possible(cpu_map))
		गच्छ मुक्त;

	for_each_cpu(i, cpu_map) {
		/* Skip already covered CPUs. */
		जइ (find_pd(pd, i))
			अनुवर्त;

		/* Create the new pd and add it to the local list. */
		tmp = pd_init(i);
		जइ (!tmp)
			गच्छ मुक्त;
		tmp->next = pd;
		pd = tmp;
	}

	perf_domain_debug(cpu_map, pd);

	/* Attach the new list of performance domains to the root domain. */
	tmp = rd->pd;
	rcu_assign_pointer(rd->pd, pd);
	जइ (tmp)
		call_rcu(&tmp->rcu, destroy_perf_domain_rcu);

	प्रत्यागम !!pd;

मुक्त:
	free_pd(pd);
	tmp = rd->pd;
	rcu_assign_pointer(rd->pd, रिक्त);
	जइ (tmp)
		call_rcu(&tmp->rcu, destroy_perf_domain_rcu);

	प्रत्यागम false;
}
#अण्णहा /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL): */
स्थिर शून्य free_pd(संरचना perf_domain *pd) { }
#endif /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL) */

स्थिर शून्य free_rootdomain(संरचना rcu_head *rcu)
{
	संरचना मूलम्_domain *rd = container_of(rcu, संरचना मूलम्_domain, rcu);

	cpupri_cleanup(&rd->cpupri);
	cpudl_cleanup(&rd->cpudl);
	free_cpumask_var(rd->dlo_mask);
	free_cpumask_var(rd->rto_mask);
	free_cpumask_var(rd->online);
	free_cpumask_var(rd->span);
	free_pd(rd->pd);
	kfree(rd);
}

शून्य rq_attach_मूलम्(संरचना rq *rq, संरचना मूलम्_domain *rd)
{
	संरचना मूलम्_domain *old_rd = रिक्त;
	संरचना rq_flags rf;

	rq_lock_irqsave(rq, &rf);

	जइ (rq->rd) {
		old_rd = rq->rd;

		जइ (cpumask_test_cpu(rq->cpu, old_rd->online))
			set_rq_offline(rq);

		cpumask_clear_cpu(rq->cpu, old_rd->span);

		/*
		 * If we don't want to free the old_rd yet then
		 * set old_rd to NULL to skip the freeing later
		 * in this function:
		 */
		जइ (!atomic_dec_and_test(&old_rd->refcount))
			old_rd = रिक्त;
	}

	atomic_inc(&rd->refcount);
	rq->rd = rd;

	cpumask_set_cpu(rq->cpu, rd->span);
	जइ (cpumask_test_cpu(rq->cpu, cpu_active_mask))
		set_rq_online(rq);

	/*
	 * Because the rq is not a task, dl_add_task_root_domain() did not
	 * move the fair server bw to the rd if it already started.
	 * Add it now.
	 */
	जइ (rq->fair_server.dl_server)
		__dl_server_attach_root(&rq->fair_server, rq);

#ifdef CONFIG_SCHED_CLASS_EXT
	जइ (rq->ext_server.dl_server)
		__dl_server_attach_root(&rq->ext_server, rq);
#endif

	rq_unlock_irqrestore(rq, &rf);

	जइ (old_rd)
		call_rcu(&old_rd->rcu, free_rootdomain);
}

शून्य sched_get_rd(संरचना मूलम्_domain *rd)
{
	atomic_inc(&rd->refcount);
}

शून्य sched_put_rd(संरचना मूलम्_domain *rd)
{
	जइ (!atomic_dec_and_test(&rd->refcount))
		प्रत्यागम;

	call_rcu(&rd->rcu, free_rootdomain);
}

स्थिर पूर्णाङ्क init_rootdomain(संरचना मूलम्_domain *rd)
{
	जइ (!zalloc_cpumask_var(&rd->span, GFP_KERNEL))
		गच्छ out;
	जइ (!zalloc_cpumask_var(&rd->online, GFP_KERNEL))
		गच्छ free_span;
	जइ (!zalloc_cpumask_var(&rd->dlo_mask, GFP_KERNEL))
		गच्छ free_online;
	जइ (!zalloc_cpumask_var(&rd->rto_mask, GFP_KERNEL))
		गच्छ free_dlo_mask;

#ifdef HAVE_RT_प्रक्षिप_IPI
	rd->rto_cpu = -1;
	raw_spin_lock_init(&rd->rto_lock);
	rd->rto_प्रक्षिप_work = IRQ_WORK_INIT_HARD(rto_प्रक्षिप_irq_work_func);
#endif

	rd->visit_cookie = 0;
	init_dl_bw(&rd->dl_bw);
	जइ (cpudl_init(&rd->cpudl) != 0)
		गच्छ free_rto_mask;

	जइ (cpupri_init(&rd->cpupri) != 0)
		गच्छ free_cpudl;
	प्रत्यागम 0;

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
	प्रत्यागम -ENOMEM;
}

/*
 * By default the system creates a single root-domain with all CPUs as
 * members (mimicking the global state we have today).
 */
संरचना मूलम्_domain def_मूलम्_domain;

शून्य __init init_defrootdomain(शून्य)
{
	init_rootdomain(&def_मूलम्_domain);

	atomic_set(&def_मूलम्_domain.refcount, 1);
}

स्थिर संरचना मूलम्_domain *alloc_rootdomain(शून्य)
{
	संरचना मूलम्_domain *rd;

	rd = kzalloc_obj(*rd);
	जइ (!rd)
		प्रत्यागम रिक्त;

	जइ (init_rootdomain(rd) != 0) {
		kfree(rd);
		प्रत्यागम रिक्त;
	}

	प्रत्यागम rd;
}

स्थिर शून्य free_sched_groups(संरचना sched_group *sg, पूर्णाङ्क free_sgc)
{
	संरचना sched_group *tmp, *first;

	जइ (!sg)
		प्रत्यागम;

	first = sg;
	करेहि {
		tmp = sg->next;

		जइ (free_sgc && atomic_dec_and_test(&sg->sgc->ref))
			kfree(sg->sgc);

		जइ (atomic_dec_and_test(&sg->ref))
			kfree(sg);
		sg = tmp;
	} जाव (sg != first);
}

स्थिर शून्य free_sched_domain_shared(संरचना sched_domain_shared *sds)
{
	जइ (sds && atomic_dec_and_test(&sds->ref))
		kfree(sds);
}

स्थिर शून्य destroy_sched_domain(संरचना sched_domain *sd)
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

स्थिर शून्य destroy_sched_domains_rcu(संरचना rcu_head *rcu)
{
	संरचना sched_domain *sd = container_of(rcu, संरचना sched_domain, rcu);

	जाव (sd) {
		संरचना sched_domain *parent = sd->parent;
		destroy_sched_domain(sd);
		sd = parent;
	}
}

स्थिर शून्य destroy_sched_domains(संरचना sched_domain *sd)
{
	जइ (sd)
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
DEFINE_PER_CPU(संरचना sched_domain __rcu *, sd_llc);
DEFINE_PER_CPU(पूर्णाङ्क, sd_llc_size);
DEFINE_PER_CPU(पूर्णाङ्क, sd_llc_id) = -1;
DEFINE_PER_CPU(पूर्णाङ्क, sd_share_id);
DEFINE_PER_CPU(संरचना sched_domain_shared __rcu *, sd_llc_shared);
DEFINE_PER_CPU(संरचना sched_domain_shared __rcu *, sd_balance_shared);
DEFINE_PER_CPU(संरचना sched_domain __rcu *, sd_numa);
DEFINE_PER_CPU(संरचना sched_domain __rcu *, sd_asym_packing);
DEFINE_PER_CPU(संरचना sched_domain __rcu *, sd_asym_cpucapacity);

DEFINE_STATIC_कुञ्चिका_FALSE(sched_asym_cpucapacity);
DEFINE_STATIC_कुञ्चिका_FALSE(sched_cluster_active);

स्थिर शून्य update_शिखरम्_cache_domain(पूर्णाङ्क cpu)
{
	संरचना sched_domain_shared *sds = रिक्त;
	संरचना sched_domain *sd;
	पूर्णाङ्क id = cpu;
	पूर्णाङ्क size = 1;

	sd = highest_flag_domain(cpu, SD_SHARE_LLC);
	जइ (sd) {
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
	जइ (sd)
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
	जइ (sd && sd->shared)
		sds = sd->shared;

	rcu_assign_pointer(per_cpu(sd_asym_cpucapacity, cpu), sd);
	rcu_assign_pointer(per_cpu(sd_balance_shared, cpu), sds);
}

/*
 * Attach the domain 'sd' to 'cpu' as its base domain. Callers must
 * hold the hotplug lock.
 */
स्थिर शून्य
cpu_attach_domain(संरचना sched_domain *sd, संरचना मूलम्_domain *rd, पूर्णाङ्क cpu)
{
	संरचना rq *rq = cpu_rq(cpu);
	संरचना sched_domain *tmp;

	/* Remove the sched domains which do not contribute to scheduling. */
	प्रति (tmp = sd; tmp; ) {
		संरचना sched_domain *parent = tmp->parent;
		जइ (!parent)
			भङ्ग;

		जइ (sd_parent_degenerate(tmp, parent)) {
			tmp->parent = parent->parent;

			/* Pick reference to parent->shared. */
			जइ (parent->shared) {
				/*
				 * It is safe to free a sd->shared that
				 * has not been published yet. If a
				 * sd->shared was published, the refcount
				 * will end up being non-zero and it will
				 * not be freed here.
				 */
				free_sched_domain_shared(tmp->shared);
				tmp->shared = parent->shared;
				parent->shared = रिक्त;
			}

			जइ (parent->parent) {
				parent->parent->child = tmp;
				parent->parent->groups->flags = tmp->flags;
			}

			/*
			 * Transfer SD_PREFER_SIBLING down in case of a
			 * degenerate parent; the spans match for this
			 * so the property transfers.
			 */
			जइ (parent->flags & SD_PREFER_SIBLING)
				tmp->flags |= SD_PREFER_SIBLING;
			destroy_sched_domain(parent);
		} अण्णहा
			tmp = tmp->parent;
	}

	जइ (sd && sd_degenerate(sd)) {
		tmp = sd;
		sd = sd->parent;

		जइ (sd) {
			संरचना sched_group *sg = sd->groups;

#ifdef CONFIG_SCHED_CACHE
			/* move buffer to parent as child is being destroyed */
			sd->llc_counts = tmp->llc_counts;
			sd->llc_max = tmp->llc_max;
			sd->llc_bytes = tmp->llc_bytes;
			/* make sure destroy_sched_domain() does not free it */
			tmp->llc_counts = रिक्त;
			tmp->llc_max = 0;
			tmp->llc_bytes = 0;
#endif
			/*
			 * sched groups hold the flags of the child sched
			 * domain for convenience. Clear such flags since
			 * the child is being destroyed.
			 */
			करेहि {
				sg->flags = 0;
			} जाव (sg != sd->groups);

			sd->child = रिक्त;
		}

		destroy_sched_domain(tmp);
	}

	sched_domain_debug(sd, cpu);

	rq_attach_मूलम्(rq, rd);
	tmp = rq->sd;
	rcu_assign_pointer(rq->sd, sd);
	dirty_sched_domain_sysctl(cpu);
	destroy_sched_domains(tmp);

	update_शिखरम्_cache_domain(cpu);
}

संरचना s_data {
	संरचना sched_domain_shared * __percpu *sds;
	संरचना sched_domain * __percpu *sd;
	संरचना मूलम्_domain	*rd;
};

गणना s_alloc {
	sa_rootdomain,
	sa_sd,
	sa_sd_shared,
	sa_sd_storage,
	sa_none,
};

#ifdef CONFIG_SCHED_CACHE
/* hardware support for cache aware scheduling */
DEFINE_STATIC_कुञ्चिका_FALSE(sched_cache_present);
/*
 * Indicator of whether cache aware scheduling
 * is active, used by the scheduler.
 */
DEFINE_STATIC_कुञ्चिका_FALSE(sched_cache_active);
/* user wants cache aware scheduling [0 or 1] */
पूर्णाङ्क sysctl_sched_cache_user = 1;

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
स्थिर unsigned long get_effective_llc_bytes(पूर्णाङ्क cpu,
					     संरचना sched_domain *sd)
{
	संरचना cacheinfo *ci;
	unsigned पूर्णाङ्क hw_weight;

	ci = get_cpu_cacheinfo_llc(cpu);
	जइ (!ci)
		प्रत्यागम 0;

	hw_weight = cpumask_weight(&ci->shared_cpu_map);
	जइ (!hw_weight)
		प्रत्यागम 0;

	प्रत्यागम div_u64((u64)ci->size * sd->span_weight, hw_weight);
}

स्थिर bool alloc_sd_llc(ध्रुव संरचना cpumask *cpu_map,
			 संरचना s_data *d)
{
	संरचना sched_domain *sd, *शिखरम्_llc, *parent;
	unsigned पूर्णाङ्क *p;
	पूर्णाङ्क i;

	for_each_cpu(i, cpu_map) {
		sd = *per_cpu_ptr(d->sd, i);
		जइ (!sd)
			गच्छ err;

		p = kcalloc_ग्रन्थिः(max_lid + 1, परिमाण(unsigned पूर्णाङ्क),
				 GFP_KERNEL, cpu_to_ग्रन्थिः(i));
		जइ (!p)
			गच्छ err;

		शिखरम्_llc = sd;
		/*
		 * Find the topmost SD_SHARE_LLC domain.
		 * Not yet attached to the CPU, so per_cpu(sd_llc, i)
		 * can not be used.
		 */
		जाव ((parent = rcu_dereference_protected(शिखरम्_llc->parent, true)) &&
		       (parent->flags & SD_SHARE_LLC))
			शिखरम्_llc = parent;

		जइ (शिखरम्_llc->flags & SD_SHARE_LLC) {
			sd->llc_max = max_lid + 1;
			sd->llc_counts = p;
			sd->llc_bytes = get_effective_llc_bytes(i, शिखरम्_llc);
		} अण्णहा {
			/* avoid memory leak */
			kfree(p);
		}
	}

	प्रत्यागम true;
err:
	for_each_cpu(i, cpu_map) {
		sd = *per_cpu_ptr(d->sd, i);
		जइ (sd) {
			kfree(sd->llc_counts);
			sd->llc_counts = रिक्त;
			sd->llc_max = 0;
			sd->llc_bytes = 0;
		}
	}

	प्रत्यागम false;
}

/*
 * Enable/disable cache aware scheduling according to
 * user input and the presence of hardware support.
 */
स्थिर शून्य _sched_cache_active_set(शून्य)
{
	lockdep_assert_cpus_held();
	lockdep_assert_held(&sched_domains_mutex);

	/* hardware does not support */
	जइ (!static_branch_likely(&sched_cache_present)) {
		static_branch_disable_cpuslocked(&sched_cache_active);
		जइ (sched_debug())
			pr_info("%s: cache aware scheduling not supported on this platform\n", __func__);
		प्रत्यागम;
	}

	/*
	 * user wants it or not ?
	 * TBD: read before writing the static key.
	 * It is not in the critical path, leave as-is
	 * for now.
	 */
	जइ (sysctl_sched_cache_user) {
		static_branch_enable_cpuslocked(&sched_cache_active);
		जइ (sched_debug())
			pr_info("%s: enabling cache aware scheduling\n", __func__);
	} अण्णहा {
		static_branch_disable_cpuslocked(&sched_cache_active);
		जइ (sched_debug())
			pr_info("%s: disabling cache aware scheduling\n", __func__);
	}
}

/* used by debugfs */
शून्य sched_cache_active_set(शून्य)
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
शून्य sched_update_llc_bytes(ध्रुव संरचना cpumask *cpus)
{
	संरचना sched_domain *sd, *sdp;
	unsigned पूर्णाङ्क i;

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
		जइ (!sdp)
			अनुवर्त;

		sd = rcu_dereference_sched_domain(cpu_rq(i)->sd);
		जइ (sd)
			sd->llc_bytes = get_effective_llc_bytes(i, sdp);
	}

	sched_domains_mutex_unlock();
}

स्थिर शून्य sched_cache_set(bool has_multi_llcs)
{
	/*
	 * TBD: check before writing to it. sched domain rebuild
	 * is not in the critical path, leave as-is for now.
	 */
	जइ (has_multi_llcs)
		static_branch_enable_cpuslocked(&sched_cache_present);
	अण्णहा
		static_branch_disable_cpuslocked(&sched_cache_present);

	_sched_cache_active_set();
}
#अण्णहा
स्थिर bool alloc_sd_llc(ध्रुव संरचना cpumask *cpu_map,
			 संरचना s_data *d)
{
	प्रत्यागम false;
}
स्थिर अन्तःस्थ शून्य sched_cache_set(bool has_multi_llcs) { }
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
स्थिर bool sd_in_multi_llcs(संरचना sched_domain *sd)
{
	संरचना sched_domain *sdp = sd->parent;

	/* it does not make sense to aggregate to 1 CPU */
	जइ (sd->span_weight == 1)
		प्रत्यागम false;

	जाव (sdp && sdp->span_weight == sd->span_weight)
		sdp = sdp->parent;

	प्रत्यागम !!sdp;
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
पूर्णाङ्क group_balance_cpu(संरचना sched_group *sg)
{
	प्रत्यागम cpumask_first(group_balance_mask(sg));
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
स्थिर शून्य
build_balance_mask(संरचना sched_domain *sd, संरचना sched_group *sg, संरचना cpumask *mask)
{
	ध्रुव संरचना cpumask *sg_span = sched_group_span(sg);
	संरचना sd_data *sdd = sd->गुप्त;
	संरचना sched_domain *sibling;
	पूर्णाङ्क i;

	cpumask_clear(mask);

	for_each_cpu(i, sg_span) {
		sibling = *per_cpu_ptr(sdd->sd, i);

		/*
		 * Can happen in the asymmetric case, where these siblings are
		 * unused. The mask will not be empty because those CPUs that
		 * do have the top domain _should_ span the domain.
		 */
		जइ (!sibling->child)
			अनुवर्त;

		/* If we would not end up here, we can't continue from here */
		जइ (!cpumask_equal(sg_span, sched_domain_span(sibling->child)))
			अनुवर्त;

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
स्थिर संरचना sched_group *
build_group_from_child_sched_domain(संरचना sched_domain *sd, पूर्णाङ्क cpu)
{
	संरचना sched_group *sg;
	संरचना cpumask *sg_span;

	sg = kzalloc_ग्रन्थिः(परिमाण(संरचना sched_group) + cpumask_size(),
			GFP_KERNEL, cpu_to_ग्रन्थिः(cpu));

	जइ (!sg)
		प्रत्यागम रिक्त;

	sg_span = sched_group_span(sg);
	जइ (sd->child) {
		cpumask_copy(sg_span, sched_domain_span(sd->child));
		sg->flags = sd->child->flags;
	} अण्णहा {
		cpumask_copy(sg_span, sched_domain_span(sd));
	}

	atomic_inc(&sg->ref);
	प्रत्यागम sg;
}

स्थिर शून्य init_overlap_sched_group(संरचना sched_domain *sd,
				     संरचना sched_group *sg)
{
	संरचना cpumask *mask = sched_domains_tmpmask2;
	संरचना sd_data *sdd = sd->गुप्त;
	संरचना cpumask *sg_span;
	पूर्णाङ्क cpu;

	build_balance_mask(sd, sg, mask);
	cpu = cpumask_first(mask);

	sg->sgc = *per_cpu_ptr(sdd->sgc, cpu);
	जइ (atomic_inc_return(&sg->sgc->ref) == 1)
		cpumask_copy(group_balance_mask(sg), mask);
	अण्णहा
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

स्थिर संरचना sched_domain *
find_descended_sibling(संरचना sched_domain *sd, संरचना sched_domain *sibling)
{
	/*
	 * The proper descendant would be the one whose child won't span out
	 * of sd
	 */
	जाव (sibling->child &&
	       !cpumask_subset(sched_domain_span(sibling->child),
			       sched_domain_span(sd)))
		sibling = sibling->child;

	/*
	 * As we are referencing sgc across different topology level, we need
	 * to go down to skip those sched_domains which don't contribute to
	 * scheduling because they will be degenerated in cpu_attach_domain
	 */
	जाव (sibling->child &&
	       cpumask_equal(sched_domain_span(sibling->child),
			     sched_domain_span(sibling)))
		sibling = sibling->child;

	प्रत्यागम sibling;
}

स्थिर पूर्णाङ्क
build_overlap_sched_groups(संरचना sched_domain *sd, पूर्णाङ्क cpu)
{
	संरचना sched_group *first = रिक्त, *last = रिक्त, *sg;
	ध्रुव संरचना cpumask *span = sched_domain_span(sd);
	संरचना cpumask *covered = sched_domains_tmpmask;
	संरचना sd_data *sdd = sd->गुप्त;
	संरचना sched_domain *sibling;
	पूर्णाङ्क i;

	cpumask_clear(covered);

	for_each_cpu_wrap(i, span, cpu) {
		संरचना cpumask *sg_span;

		जइ (cpumask_test_cpu(i, covered))
			अनुवर्त;

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
		जइ (!cpumask_test_cpu(i, sched_domain_span(sibling)))
			अनुवर्त;

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
		जइ (sibling->child &&
		    !cpumask_subset(sched_domain_span(sibling->child), span))
			sibling = find_descended_sibling(sd, sibling);

		sg = build_group_from_child_sched_domain(sibling, cpu);
		जइ (!sg)
			गच्छ fail;

		sg_span = sched_group_span(sg);
		cpumask_or(covered, covered, sg_span);

		init_overlap_sched_group(sibling, sg);

		जइ (!first)
			first = sg;
		जइ (last)
			last->next = sg;
		last = sg;
		last->next = first;
	}
	sd->groups = first;

	प्रत्यागम 0;

fail:
	free_sched_groups(first, 0);

	प्रत्यागम -ENOMEM;
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

स्थिर संरचना sched_group *get_group(पूर्णाङ्क cpu, संरचना sd_data *sdd)
{
	संरचना sched_domain *sd = *per_cpu_ptr(sdd->sd, cpu);
	संरचना sched_domain *child = sd->child;
	संरचना sched_group *sg;
	bool already_visited;

	जइ (child)
		cpu = cpumask_first(sched_domain_span(child));

	sg = *per_cpu_ptr(sdd->sg, cpu);
	sg->sgc = *per_cpu_ptr(sdd->sgc, cpu);

	/* Increase refcounts for claim_allocations: */
	already_visited = atomic_inc_return(&sg->ref) > 1;
	/* sgc visits should follow a similar trend as sg */
	WARN_ON(already_visited != (atomic_inc_return(&sg->sgc->ref) > 1));

	/* If we have already visited that group, it's already initialized. */
	जइ (already_visited)
		प्रत्यागम sg;

	जइ (child) {
		cpumask_copy(sched_group_span(sg), sched_domain_span(child));
		cpumask_copy(group_balance_mask(sg), sched_group_span(sg));
		sg->flags = child->flags;
	} अण्णहा {
		cpumask_set_cpu(cpu, sched_group_span(sg));
		cpumask_set_cpu(cpu, group_balance_mask(sg));
	}

	sg->sgc->capacity = SCHED_CAPACITY_SCALE * cpumask_weight(sched_group_span(sg));
	sg->sgc->min_capacity = SCHED_CAPACITY_SCALE;
	sg->sgc->max_capacity = SCHED_CAPACITY_SCALE;

	प्रत्यागम sg;
}

/*
 * build_sched_groups will build a circular linked list of the groups
 * covered by the given span, will set each group's ->cpumask correctly,
 * and will initialize their ->sgc.
 *
 * Assumes the sched_domain tree is fully constructed
 */
स्थिर पूर्णाङ्क
build_sched_groups(संरचना sched_domain *sd, पूर्णाङ्क cpu)
{
	संरचना sched_group *first = रिक्त, *last = रिक्त;
	संरचना sd_data *sdd = sd->गुप्त;
	ध्रुव संरचना cpumask *span = sched_domain_span(sd);
	संरचना cpumask *covered;
	पूर्णाङ्क i;

	lockdep_assert_held(&sched_domains_mutex);
	covered = sched_domains_tmpmask;

	cpumask_clear(covered);

	for_each_cpu_wrap(i, span, cpu) {
		संरचना sched_group *sg;

		जइ (cpumask_test_cpu(i, covered))
			अनुवर्त;

		sg = get_group(i, sdd);

		cpumask_or(covered, covered, sched_group_span(sg));

		जइ (!first)
			first = sg;
		जइ (last)
			last->next = sg;
		last = sg;
	}
	last->next = first;
	sd->groups = first;

	प्रत्यागम 0;
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
स्थिर शून्य init_sched_groups_capacity(पूर्णाङ्क cpu, संरचना sched_domain *sd)
{
	संरचना sched_group *sg = sd->groups;
	संरचना cpumask *mask = sched_domains_tmpmask2;

	WARN_ON(!sg);

	करेहि {
		पूर्णाङ्क cpu, cores = 0, max_cpu = -1;

		sg->group_weight = cpumask_weight(sched_group_span(sg));

		cpumask_copy(mask, sched_group_span(sg));
		for_each_cpu(cpu, mask) {
			cores++;
			cpumask_andnot(mask, mask, cpu_smt_mask(cpu));
		}
		sg->cores = cores;

		जइ (!(sd->flags & SD_ASYM_PACKING))
			गच्छ next;

		for_each_cpu(cpu, sched_group_span(sg)) {
			जइ (max_cpu < 0)
				max_cpu = cpu;
			अण्णहा जइ (sched_asym_prefer(cpu, max_cpu))
				max_cpu = cpu;
		}
		sg->asym_prefer_cpu = max_cpu;

next:
		sg = sg->next;
	} जाव (sg != sd->groups);

	जइ (cpu != group_balance_cpu(sg))
		प्रत्यागम;

	update_group_capacity(sd, cpu);
}

/* Update the "asym_prefer_cpu" when arch_asym_cpu_priority() changes. */
शून्य sched_update_asym_prefer_cpu(पूर्णाङ्क cpu, पूर्णाङ्क old_prio, पूर्णाङ्क new_prio)
{
	पूर्णाङ्क asym_prefer_cpu = cpu;
	संरचना sched_domain *sd;

	guard(rcu)();

	for_each_domain(cpu, sd) {
		संरचना sched_group *sg;
		पूर्णाङ्क group_cpu;

		जइ (!(sd->flags & SD_ASYM_PACKING))
			अनुवर्त;

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
		जइ (cpu != sg->asym_prefer_cpu) {
			/*
			 * Since the parent is a superset of the current group,
			 * if the cpu is not the "asym_prefer_cpu" at the
			 * current level, it cannot be the preferred CPU at a
			 * higher levels either.
			 */
			जइ (!sched_asym_prefer(cpu, sg->asym_prefer_cpu))
				प्रत्यागम;

			WRITE_ONCE(sg->asym_prefer_cpu, cpu);
			अनुवर्त;
		}

		/* Ranking has improved; CPU is still the preferred one. */
		जइ (new_prio >= old_prio)
			अनुवर्त;

		for_each_cpu(group_cpu, sched_group_span(sg)) {
			जइ (sched_asym_prefer(group_cpu, asym_prefer_cpu))
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
स्थिर अन्तःस्थ पूर्णाङ्क
asym_cpu_capacity_classify(ध्रुव संरचना cpumask *sd_span,
			   ध्रुव संरचना cpumask *cpu_map)
{
	संरचना asym_cap_data *entry;
	पूर्णाङ्क गणनम् = 0, miss = 0;

	/*
	 * Count how many unique CPU capacities this domain spans across
	 * (compare sched_domain CPUs mask with ones representing  available
	 * CPUs capacities). Take into account CPUs that might be offline:
	 * skip those.
	 */
	list_for_each_entry(entry, &asym_cap_list, link) {
		जइ (cpumask_intersects(sd_span, cpu_capacity_span(entry)))
			++गणनम्;
		अण्णहा जइ (cpumask_intersects(cpu_map, cpu_capacity_span(entry)))
			++miss;
	}

	WARN_ON_ONCE(!गणनम् && !list_empty(&asym_cap_list));

	/* No asymmetry detected */
	जइ (गणनम् < 2)
		प्रत्यागम 0;
	/* Some of the available CPU capacity values have not been detected */
	जइ (miss)
		प्रत्यागम SD_ASYM_CPUCAPACITY;

	/* Full asymmetry */
	प्रत्यागम SD_ASYM_CPUCAPACITY | SD_ASYM_CPUCAPACITY_FULL;

}

स्थिर शून्य free_asym_cap_entry(संरचना rcu_head *head)
{
	संरचना asym_cap_data *entry = container_of(head, संरचना asym_cap_data, rcu);
	kfree(entry);
}

स्थिर अन्तःस्थ शून्य asym_cpu_capacity_update_data(पूर्णाङ्क cpu)
{
	unsigned long capacity = arch_scale_cpu_capacity(cpu);
	संरचना asym_cap_data *निवेशय_entry = रिक्त;
	संरचना asym_cap_data *entry;

	/*
	 * Search if capacity already exits. If not, track which the entry
	 * where we should insert to keep the list ordered descending.
	 */
	list_for_each_entry(entry, &asym_cap_list, link) {
		जइ (capacity == entry->capacity)
			गच्छ done;
		अण्णहा जइ (!निवेशय_entry && capacity > entry->capacity)
			निवेशय_entry = list_prev_entry(entry, link);
	}

	entry = kzalloc(परिमाण(*entry) + cpumask_size(), GFP_KERNEL);
	जइ (WARN_ONCE(!entry, "Failed to allocate memory for asymmetry data\n"))
		प्रत्यागम;
	entry->capacity = capacity;

	/* If NULL then the new capacity is the smallest, add last. */
	जइ (!निवेशय_entry)
		list_add_tail_rcu(&entry->link, &asym_cap_list);
	अण्णहा
		list_add_rcu(&entry->link, &निवेशय_entry->link);
done:
	__cpumask_set_cpu(cpu, cpu_capacity_span(entry));
}

/*
 * Build-up/update list of CPUs grouped by their capacities
 * An update requires explicit request to rebuild sched domains
 * with state indicating CPU topology changes.
 */
स्थिर शून्य asym_cpu_capacity_scan(शून्य)
{
	संरचना asym_cap_data *entry, *next;
	पूर्णाङ्क cpu;

	list_for_each_entry(entry, &asym_cap_list, link)
		cpumask_clear(cpu_capacity_span(entry));

	for_each_cpu_and(cpu, cpu_possible_mask, housekeeping_cpumask(HK_TYPE_DOMAIN))
		asym_cpu_capacity_update_data(cpu);

	list_for_each_entry_safe(entry, next, &asym_cap_list, link) {
		जइ (cpumask_empty(cpu_capacity_span(entry))) {
			list_del_rcu(&entry->link);
			call_rcu(&entry->rcu, free_asym_cap_entry);
		}
	}

	/*
	 * Only one capacity value has been detected i.e. this system is symmetric.
	 * No need to keep this data around.
	 */
	जइ (list_is_singular(&asym_cap_list)) {
		entry = list_first_entry(&asym_cap_list, typeof(*entry), link);
		list_del_rcu(&entry->link);
		call_rcu(&entry->rcu, free_asym_cap_entry);
	}
}

/*
 * Initializers for schedule domains
 * Non-inlined to reduce accumulated stack pressure in build_sched_domains()
 */

स्थिर पूर्णाङ्क default_relax_domain_level = -1;
पूर्णाङ्क sched_domain_level_max;

स्थिर पूर्णाङ्क __init setup_relax_domain_level(वर्ण *str)
{
	जइ (kstrtoint(str, 0, &default_relax_domain_level))
		pr_warn("Unable to set relax_domain_level\n");

	प्रत्यागम 1;
}
__setup("relax_domain_level=", setup_relax_domain_level);

स्थिर शून्य set_domain_attribute(संरचना sched_domain *sd,
				 संरचना sched_domain_attr *attr)
{
	पूर्णाङ्क request;

	जइ (!attr || attr->relax_domain_level < 0) {
		जइ (default_relax_domain_level < 0)
			प्रत्यागम;
		request = default_relax_domain_level;
	} अण्णहा
		request = attr->relax_domain_level;

	जइ (sd->level >= request) {
		/* Turn off idle balance on this domain: */
		sd->flags &= ~(SD_BALANCE_WAKE|SD_BALANCE_NEWIDLE);
	}
}

स्थिर शून्य __sdt_free(ध्रुव संरचना cpumask *cpu_map);
स्थिर पूर्णाङ्क __sdt_alloc(ध्रुव संरचना cpumask *cpu_map);

स्थिर शून्य __sds_free(संरचना s_data *d, ध्रुव संरचना cpumask *cpu_map);
स्थिर पूर्णाङ्क __sds_alloc(संरचना s_data *d, ध्रुव संरचना cpumask *cpu_map);

स्थिर शून्य __free_domain_allocs(संरचना s_data *d, गणना s_alloc what,
				 ध्रुव संरचना cpumask *cpu_map)
{
	विकल्प (what) {
	पक्ष sa_rootdomain:
		जइ (!atomic_read(&d->rd->refcount))
			free_rootdomain(&d->rd->rcu);
		fallthrough;
	पक्ष sa_sd:
		free_percpu(d->sd);
		fallthrough;
	पक्ष sa_sd_shared:
		__sds_free(d, cpu_map);
		fallthrough;
	पक्ष sa_sd_storage:
		__sdt_free(cpu_map);
		fallthrough;
	पक्ष sa_none:
		भङ्ग;
	}
}

स्थिर गणना s_alloc
__visit_domain_allocation_hell(संरचना s_data *d, ध्रुव संरचना cpumask *cpu_map)
{
	memset(d, 0, परिमाण(*d));

	जइ (__sdt_alloc(cpu_map))
		प्रत्यागम sa_sd_storage;
	जइ (__sds_alloc(d, cpu_map))
		प्रत्यागम sa_sd_shared;
	d->sd = alloc_percpu(संरचना sched_domain *);
	जइ (!d->sd)
		प्रत्यागम sa_sd_shared;
	d->rd = alloc_rootdomain();
	जइ (!d->rd)
		प्रत्यागम sa_sd;

	प्रत्यागम sa_rootdomain;
}

/*
 * NULL the sd_data elements we've used to build the sched_domain and
 * sched_group structure so that the subsequent __free_domain_allocs()
 * will not free the data we're using.
 */
स्थिर शून्य claim_allocations(पूर्णाङ्क cpu, संरचना s_data *d)
{
	संरचना sched_domain *sd;

	जइ (atomic_read(&(*per_cpu_ptr(d->sds, cpu))->ref))
		*per_cpu_ptr(d->sds, cpu) = रिक्त;

	प्रति (sd = *per_cpu_ptr(d->sd, cpu); sd; sd = sd->parent) {
		संरचना sd_data *sdd = sd->गुप्त;

		WARN_ON_ONCE(*per_cpu_ptr(sdd->sd, cpu) != sd);
		*per_cpu_ptr(sdd->sd, cpu) = रिक्त;

		जइ (atomic_read(&(*per_cpu_ptr(sdd->sg, cpu))->ref))
			*per_cpu_ptr(sdd->sg, cpu) = रिक्त;

		जइ (atomic_read(&(*per_cpu_ptr(sdd->sgc, cpu))->ref))
			*per_cpu_ptr(sdd->sgc, cpu) = रिक्त;
	}
}

#ifdef CONFIG_NUMA
गणना numa_topology_type sched_numa_topology_type;

/*
 * sched_domains_numa_distance is derived from sched_numa_node_distance
 * and provides a simplified view of NUMA distances used specifically
 * for building NUMA scheduling domains.
 */
स्थिर पूर्णाङ्क			sched_domains_numa_levels;
स्थिर पूर्णाङ्क			sched_numa_ग्रन्थिः_levels;

पूर्णाङ्क				sched_max_numa_distance;
स्थिर पूर्णाङ्क			*sched_domains_numa_distance;
स्थिर पूर्णाङ्क			*sched_numa_ग्रन्थिः_distance;
स्थिर संरचना cpumask		***sched_domains_numa_masks;
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

स्थिर संरचना sched_domain *
sd_init(संरचना sched_domain_topology_level *tl,
	ध्रुव संरचना cpumask *cpu_map,
	संरचना sched_domain *child, पूर्णाङ्क cpu)
{
	संरचना sd_data *sdd = &tl->data;
	संरचना sched_domain *sd = *per_cpu_ptr(sdd->sd, cpu);
	पूर्णाङ्क sd_id, sd_weight, sd_flags = 0;
	संरचना cpumask *sd_span;
	u64 now = sched_clock();

	sd_span = sched_domain_span(sd);
	cpumask_and(sd_span, cpu_map, tl->mask(tl, cpu));
	sd_weight = cpumask_weight(sd_span);
	sd_id = cpumask_first(sd_span);

	जइ (tl->sd_flags)
		sd_flags = (*tl->sd_flags)();
	जइ (WARN_ONCE(sd_flags & ~TOPOLOGY_SD_FLAGS,
		      "wrong sd_flags in topology description\n"))
		sd_flags &= TOPOLOGY_SD_FLAGS;
#ifdef CONFIG_SCHED_SMT
	जइ (sched_smt_asym_packing && (sd_flags & SD_SHARE_CPUCAPACITY))
		sd_flags |= SD_ASYM_PACKING;
#endif
	sd_flags |= asym_cpu_capacity_classify(sd_span, cpu_map);

	*sd = (संरचना sched_domain){
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
	जइ (sd->flags & SD_SHARE_CPUCAPACITY) {
		sd->imbalance_pct = 110;

	} अण्णहा जइ (sd->flags & SD_SHARE_LLC) {
		sd->imbalance_pct = 117;
		sd->cache_nice_tries = 1;

#ifdef CONFIG_NUMA
	} अण्णहा जइ (sd->flags & SD_NUMA) {
		sd->cache_nice_tries = 2;

		sd->flags &= ~SD_PREFER_SIBLING;
		sd->flags |= SD_SERIALIZE;
		जइ (sched_domains_numa_distance[tl->numa_level] > ग्रन्थिः_reclaim_distance) {
			sd->flags &= ~(SD_BALANCE_EXEC |
				       SD_BALANCE_FORK |
				       SD_WAKE_AFFINE);
		}

#endif /* CONFIG_NUMA */
	} अण्णहा {
		sd->cache_nice_tries = 1;
	}

	sd->गुप्त = sdd;

	प्रत्यागम sd;
}

#ifdef CONFIG_SCHED_SMT
पूर्णाङ्क cpu_smt_flags(शून्य)
{
	प्रत्यागम SD_SHARE_CPUCAPACITY | SD_SHARE_LLC;
}

ध्रुव संरचना cpumask *tl_smt_mask(संरचना sched_domain_topology_level *tl, पूर्णाङ्क cpu)
{
	प्रत्यागम cpu_smt_mask(cpu);
}
#endif

#ifdef CONFIG_SCHED_CLUSTER
पूर्णाङ्क cpu_cluster_flags(शून्य)
{
	प्रत्यागम SD_CLUSTER | SD_SHARE_LLC;
}

ध्रुव संरचना cpumask *tl_cls_mask(संरचना sched_domain_topology_level *tl, पूर्णाङ्क cpu)
{
	प्रत्यागम cpu_clustergroup_mask(cpu);
}
#endif

#ifdef CONFIG_SCHED_MC
पूर्णाङ्क cpu_core_flags(शून्य)
{
	प्रत्यागम SD_SHARE_LLC;
}

ध्रुव संरचना cpumask *tl_mc_mask(संरचना sched_domain_topology_level *tl, पूर्णाङ्क cpu)
{
	प्रत्यागम cpu_coregroup_mask(cpu);
}

/*
 * Majority of architectures have LLC at MC domain level with exception
 * such as powerpc. Provide a way for arch to specify where its LLC is
 * if it falls in exception category
 */
# ifndef arch_llc_mask
#define arch_llc_mask(cpu) cpu_coregroup_mask(cpu)
# endif

#अण्णहा
#define arch_llc_mask(cpu) cpumask_of(cpu)
#endif

#define llc_mask(cpu) arch_llc_mask(cpu)

ध्रुव संरचना cpumask *tl_pkg_mask(संरचना sched_domain_topology_level *tl, पूर्णाङ्क cpu)
{
	प्रत्यागम cpu_ग्रन्थिः_mask(cpu);
}

/*
 * Topology list, bottom-up.
 */
स्थिर संरचना sched_domain_topology_level default_topology[] = {
#ifdef CONFIG_SCHED_SMT
	SDTL_INIT(tl_smt_mask, cpu_smt_flags, SMT),
#endif

#ifdef CONFIG_SCHED_CLUSTER
	SDTL_INIT(tl_cls_mask, cpu_cluster_flags, CLS),
#endif

#ifdef CONFIG_SCHED_MC
	SDTL_INIT(tl_mc_mask, cpu_core_flags, MC),
#endif
	SDTL_INIT(tl_pkg_mask, रिक्त, PKG),
	{ रिक्त, },
};

स्थिर संरचना sched_domain_topology_level *sched_domain_topology =
	default_topology;
स्थिर संरचना sched_domain_topology_level *sched_domain_topology_saved;

#define for_each_sd_topology(tl)			\
	प्रति (tl = sched_domain_topology; tl->mask; tl++)

शून्य __init set_sched_topology(संरचना sched_domain_topology_level *tl)
{
	जइ (WARN_ON_ONCE(sched_smp_initialized))
		प्रत्यागम;

	sched_domain_topology = tl;
	sched_domain_topology_saved = रिक्त;
}

#ifdef CONFIG_NUMA
स्थिर पूर्णाङ्क cpu_numa_flags(शून्य)
{
	प्रत्यागम SD_NUMA;
}

स्थिर ध्रुव संरचना cpumask *sd_numa_mask(संरचना sched_domain_topology_level *tl, पूर्णाङ्क cpu)
{
	प्रत्यागम sched_domains_numa_masks[tl->numa_level][cpu_to_ग्रन्थिः(cpu)];
}

स्थिर शून्य sched_numa_warn(ध्रुव वर्ण *str)
{
	स्थिर पूर्णाङ्क done = false;
	पूर्णाङ्क i,j;

	जइ (done)
		प्रत्यागम;

	done = true;

	printk(KERN_WARNING "ERROR: %s\n\n", str);

	प्रति (i = 0; i < nr_ग्रन्थिः_ids; i++) {
		printk(KERN_WARNING "  ");
		प्रति (j = 0; j < nr_ग्रन्थिः_ids; j++) {
			जइ (!ग्रन्थिः_state(i, N_CPU) || !ग्रन्थिः_state(j, N_CPU))
				printk(KERN_CONT "(%02d) ", ग्रन्थिः_distance(i,j));
			अण्णहा
				printk(KERN_CONT " %02d  ", ग्रन्थिः_distance(i,j));
		}
		printk(KERN_CONT "\n");
	}
	printk(KERN_WARNING "\n");
}

bool find_numa_distance(पूर्णाङ्क distance)
{
	bool found = false;
	पूर्णाङ्क i, *distances;

	जइ (distance == ग्रन्थिः_distance(0, 0))
		प्रत्यागम true;

	rcu_read_lock();
	distances = rcu_dereference(sched_numa_ग्रन्थिः_distance);
	जइ (!distances)
		गच्छ unlock;
	प्रति (i = 0; i < sched_numa_ग्रन्थिः_levels; i++) {
		जइ (distances[i] == distance) {
			found = true;
			भङ्ग;
		}
	}
unlock:
	rcu_read_unlock();

	प्रत्यागम found;
}

#define for_each_cpu_ग्रन्थिः_but(n, nbut)		\
	for_each_ग्रन्थिः_state(n, N_CPU)		\
		जइ (n == nbut)			\
			अनुवर्त;		\
		अण्णहा

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
स्थिर शून्य init_numa_topology_type(पूर्णाङ्क offline_ग्रन्थिः)
{
	पूर्णाङ्क a, b, c, n;

	n = sched_max_numa_distance;

	जइ (sched_domains_numa_levels <= 2) {
		sched_numa_topology_type = NUMA_DIRECT;
		प्रत्यागम;
	}

	for_each_cpu_ग्रन्थिः_but(a, offline_ग्रन्थिः) {
		for_each_cpu_ग्रन्थिः_but(b, offline_ग्रन्थिः) {
			/* Find two nodes furthest removed from each other. */
			जइ (ग्रन्थिः_distance(a, b) < n)
				अनुवर्त;

			/* Is there an intermediary node between a and b? */
			for_each_cpu_ग्रन्थिः_but(c, offline_ग्रन्थिः) {
				जइ (ग्रन्थिः_distance(a, c) < n &&
				    ग्रन्थिः_distance(b, c) < n) {
					sched_numa_topology_type =
							NUMA_GLUELESS_MESH;
					प्रत्यागम;
				}
			}

			sched_numa_topology_type = NUMA_BACKPLANE;
			प्रत्यागम;
		}
	}

	pr_err("Failed to find a NUMA topology type, defaulting to DIRECT\n");
	sched_numa_topology_type = NUMA_DIRECT;
}


#define NR_DISTANCE_मानानि (1 << DISTANCE_BITS)

/*
 * An architecture could modify its NUMA distance, to change
 * grouping of NUMA nodes and number of NUMA levels when creating
 * NUMA level sched domains.
 *
 * A NUMA level is created for each unique
 * arch_sched_node_distance.
 */
स्थिर पूर्णाङ्क numa_ग्रन्थिः_dist(पूर्णाङ्क i, पूर्णाङ्क j)
{
	प्रत्यागम ग्रन्थिः_distance(i, j);
}

पूर्णाङ्क arch_sched_ग्रन्थिः_distance(पूर्णाङ्क from, पूर्णाङ्क to)
			     __weak __alias(numa_ग्रन्थिः_dist);

स्थिर bool modified_sched_ग्रन्थिः_distance(शून्य)
{
	प्रत्यागम numa_ग्रन्थिः_dist != arch_sched_ग्रन्थिः_distance;
}

स्थिर पूर्णाङ्क sched_record_numa_dist(पूर्णाङ्क offline_ग्रन्थिः, पूर्णाङ्क (*n_dist)(पूर्णाङ्क, पूर्णाङ्क),
				  पूर्णाङ्क **dist, पूर्णाङ्क *levels)
{
	unsigned long *distance_map __free(bitmap) = रिक्त;
	पूर्णाङ्क nr_levels = 0;
	पूर्णाङ्क i, j;
	पूर्णाङ्क *distances;

	/*
	 * O(nr_nodes^2) de-duplicating selection sort -- in order to find the
	 * unique distances in the node_distance() table.
	 */
	distance_map = bitmap_alloc(NR_DISTANCE_मानानि, GFP_KERNEL);
	जइ (!distance_map)
		प्रत्यागम -ENOMEM;

	bitmap_zero(distance_map, NR_DISTANCE_मानानि);
	for_each_cpu_ग्रन्थिः_but(i, offline_ग्रन्थिः) {
		for_each_cpu_ग्रन्थिः_but(j, offline_ग्रन्थिः) {
			पूर्णाङ्क distance = n_dist(i, j);

			जइ (distance < LOCAL_DISTANCE || distance >= NR_DISTANCE_मानानि) {
				sched_numa_warn("Invalid distance value range");
				प्रत्यागम -EINVAL;
			}

			bitmap_set(distance_map, distance, 1);
		}
	}
	/*
	 * We can now figure out how many unique distance values there are and
	 * allocate memory accordingly.
	 */
	nr_levels = bitmap_weight(distance_map, NR_DISTANCE_मानानि);

	distances = kzalloc_objs(पूर्णाङ्क, nr_levels);
	जइ (!distances)
		प्रत्यागम -ENOMEM;

	प्रति (i = 0, j = 0; i < nr_levels; i++, j++) {
		j = find_next_bit(distance_map, NR_DISTANCE_मानानि, j);
		distances[i] = j;
	}
	*dist = distances;
	*levels = nr_levels;

	प्रत्यागम 0;
}

शून्य sched_init_numa(पूर्णाङ्क offline_ग्रन्थिः)
{
	संरचना sched_domain_topology_level *tl;
	पूर्णाङ्क nr_levels, nr_ग्रन्थिः_levels;
	पूर्णाङ्क i, j;
	पूर्णाङ्क *distances, *domain_distances;
	संरचना cpumask ***masks;

	/* Record the NUMA distances from SLIT table */
	जइ (sched_record_numa_dist(offline_ग्रन्थिः, numa_ग्रन्थिः_dist, &distances,
				   &nr_ग्रन्थिः_levels))
		प्रत्यागम;

	/* Record modified NUMA distances for building sched domains */
	जइ (modified_sched_ग्रन्थिः_distance()) {
		जइ (sched_record_numa_dist(offline_ग्रन्थिः, arch_sched_ग्रन्थिः_distance,
					   &domain_distances, &nr_levels)) {
			kfree(distances);
			प्रत्यागम;
		}
	} अण्णहा {
		domain_distances = distances;
		nr_levels = nr_ग्रन्थिः_levels;
	}
	rcu_assign_pointer(sched_numa_ग्रन्थिः_distance, distances);
	WRITE_ONCE(sched_max_numa_distance, distances[nr_ग्रन्थिः_levels - 1]);
	WRITE_ONCE(sched_numa_ग्रन्थिः_levels, nr_ग्रन्थिः_levels);

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

	masks = kzalloc(परिमाण(शून्य *) * nr_levels, GFP_KERNEL);
	जइ (!masks)
		प्रत्यागम;

	/*
	 * Now for each level, construct a mask per node which contains all
	 * CPUs of nodes that are that many hops away from us.
	 */
	प्रति (i = 0; i < nr_levels; i++) {
		masks[i] = kzalloc(nr_ग्रन्थिः_ids * परिमाण(शून्य *), GFP_KERNEL);
		जइ (!masks[i])
			प्रत्यागम;

		for_each_cpu_ग्रन्थिः_but(j, offline_ग्रन्थिः) {
			संरचना cpumask *mask = kzalloc(cpumask_size(), GFP_KERNEL);
			पूर्णाङ्क k;

			जइ (!mask)
				प्रत्यागम;

			masks[i][j] = mask;

			for_each_cpu_ग्रन्थिः_but(k, offline_ग्रन्थिः) {
				जइ (sched_debug() &&
				    (arch_sched_ग्रन्थिः_distance(j, k) !=
				     arch_sched_ग्रन्थिः_distance(k, j)))
					sched_numa_warn("Node-distance not symmetric");

				जइ (arch_sched_ग्रन्थिः_distance(j, k) >
				    sched_domains_numa_distance[i])
					अनुवर्त;

				cpumask_or(mask, mask, cpumask_of_ग्रन्थिः(k));
			}
		}
	}
	rcu_assign_pointer(sched_domains_numa_masks, masks);

	/* Compute default topology size */
	प्रति (i = 0; sched_domain_topology[i].mask; i++);

	tl = kzalloc((i + nr_levels + 1) *
			परिमाण(संरचना sched_domain_topology_level), GFP_KERNEL);
	जइ (!tl)
		प्रत्यागम;

	/*
	 * Copy the default topology bits..
	 */
	प्रति (i = 0; sched_domain_topology[i].mask; i++)
		tl[i] = sched_domain_topology[i];

	/*
	 * Add the NUMA identity distance, aka single NODE.
	 */
	tl[i++] = SDTL_INIT(sd_numa_mask, रिक्त, ग्रन्थिः_2);

	/*
	 * .. and append 'j' levels of NUMA goodness.
	 */
	प्रति (j = 1; j < nr_levels; i++, j++) {
		tl[i] = SDTL_INIT(sd_numa_mask, cpu_numa_flags, NUMA);
		tl[i].numa_level = j;
	}

	sched_domain_topology_saved = sched_domain_topology;
	sched_domain_topology = tl;

	sched_domains_numa_levels = nr_levels;

	init_numa_topology_type(offline_ग्रन्थिः);
}


स्थिर शून्य sched_reset_numa(शून्य)
{
	पूर्णाङ्क nr_levels, *distances, *dom_distances = रिक्त;
	संरचना cpumask ***masks;

	nr_levels = sched_domains_numa_levels;
	sched_numa_ग्रन्थिः_levels = 0;
	sched_domains_numa_levels = 0;
	sched_max_numa_distance = 0;
	sched_numa_topology_type = NUMA_DIRECT;
	distances = sched_numa_ग्रन्थिः_distance;
	जइ (sched_numa_ग्रन्थिः_distance != sched_domains_numa_distance)
		dom_distances = sched_domains_numa_distance;
	rcu_assign_pointer(sched_numa_ग्रन्थिः_distance, रिक्त);
	rcu_assign_pointer(sched_domains_numa_distance, रिक्त);
	masks = sched_domains_numa_masks;
	rcu_assign_pointer(sched_domains_numa_masks, रिक्त);
	जइ (distances || masks) {
		पूर्णाङ्क i, j;

		synchronize_rcu();
		kfree(distances);
		kfree(dom_distances);
		प्रति (i = 0; i < nr_levels && masks; i++) {
			जइ (!masks[i])
				अनुवर्त;
			for_each_ग्रन्थिः(j)
				kfree(masks[i][j]);
			kfree(masks[i]);
		}
		kfree(masks);
	}
	जइ (sched_domain_topology_saved) {
		kfree(sched_domain_topology);
		sched_domain_topology = sched_domain_topology_saved;
		sched_domain_topology_saved = रिक्त;
	}
}

/*
 * Call with hotplug lock held
 */
शून्य sched_update_numa(पूर्णाङ्क cpu, bool online)
{
	पूर्णाङ्क ग्रन्थिः;

	ग्रन्थिः = cpu_to_ग्रन्थिः(cpu);
	/*
	 * Scheduler NUMA topology is updated when the first CPU of a
	 * node is onlined or the last CPU of a node is offlined.
	 */
	जइ (cpumask_weight(cpumask_of_ग्रन्थिः(ग्रन्थिः)) != 1)
		प्रत्यागम;

	sched_reset_numa();
	sched_init_numa(online ? NUMA_NO_ग्रन्थिः : ग्रन्थिः);
}

शून्य sched_domains_numa_masks_set(unsigned पूर्णाङ्क cpu)
{
	पूर्णाङ्क ग्रन्थिः = cpu_to_ग्रन्थिः(cpu);
	पूर्णाङ्क i, j;

	प्रति (i = 0; i < sched_domains_numa_levels; i++) {
		प्रति (j = 0; j < nr_ग्रन्थिः_ids; j++) {
			जइ (!ग्रन्थिः_state(j, N_CPU))
				अनुवर्त;

			/* Set ourselves in the remote node's masks */
			जइ (arch_sched_ग्रन्थिः_distance(j, ग्रन्थिः) <=
			    sched_domains_numa_distance[i])
				cpumask_set_cpu(cpu, sched_domains_numa_masks[i][j]);
		}
	}
}

शून्य sched_domains_numa_masks_clear(unsigned पूर्णाङ्क cpu)
{
	पूर्णाङ्क i, j;

	प्रति (i = 0; i < sched_domains_numa_levels; i++) {
		प्रति (j = 0; j < nr_ग्रन्थिः_ids; j++) {
			जइ (sched_domains_numa_masks[i][j])
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
पूर्णाङ्क sched_numa_find_closest(ध्रुव संरचना cpumask *cpus, पूर्णाङ्क cpu)
{
	पूर्णाङ्क i, j = cpu_to_ग्रन्थिः(cpu), found = nr_cpu_ids;
	संरचना cpumask ***masks;

	rcu_read_lock();
	masks = rcu_dereference(sched_domains_numa_masks);
	जइ (!masks)
		गच्छ unlock;
	प्रति (i = 0; i < sched_domains_numa_levels; i++) {
		जइ (!masks[i][j])
			भङ्ग;
		cpu = cpumask_any_and_distribute(cpus, masks[i][j]);
		जइ (cpu < nr_cpu_ids) {
			found = cpu;
			भङ्ग;
		}
	}
unlock:
	rcu_read_unlock();

	प्रत्यागम found;
}

संरचना __cmp_key {
	ध्रुव संरचना cpumask *cpus;
	संरचना cpumask ***masks;
	पूर्णाङ्क ग्रन्थिः;
	पूर्णाङ्क cpu;
	पूर्णाङ्क w;
};

स्थिर पूर्णाङ्क hop_cmp(ध्रुव शून्य *a, ध्रुव शून्य *b)
{
	संरचना cpumask **prev_hop, **cur_hop = *(संरचना cpumask ***)b;
	संरचना __cmp_key *k = (संरचना __cmp_key *)a;

	जइ (cpumask_weight_and(k->cpus, cur_hop[k->ग्रन्थिः]) <= k->cpu)
		प्रत्यागम 1;

	जइ (b == k->masks) {
		k->w = 0;
		प्रत्यागम 0;
	}

	prev_hop = *((संरचना cpumask ***)b - 1);
	k->w = cpumask_weight_and(k->cpus, prev_hop[k->ग्रन्थिः]);
	जइ (k->w <= k->cpu)
		प्रत्यागम 0;

	प्रत्यागम -1;
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
पूर्णाङ्क sched_numa_find_nth_cpu(ध्रुव संरचना cpumask *cpus, पूर्णाङ्क cpu, पूर्णाङ्क ग्रन्थिः)
{
	संरचना __cmp_key k = { .cpus = cpus, .cpu = cpu };
	संरचना cpumask ***hop_masks;
	पूर्णाङ्क hop, ret = nr_cpu_ids;

	जइ (ग्रन्थिः == NUMA_NO_ग्रन्थिः)
		प्रत्यागम cpumask_nth_and(cpu, cpus, cpu_online_mask);

	rcu_read_lock();

	/* CPU-less node entries are uninitialized in sched_domains_numa_masks */
	ग्रन्थिः = numa_nearest_ग्रन्थिः(ग्रन्थिः, N_CPU);
	k.ग्रन्थिः = ग्रन्थिः;

	k.masks = rcu_dereference(sched_domains_numa_masks);
	जइ (!k.masks)
		गच्छ unlock;

	hop_masks = bsearch(&k, k.masks, sched_domains_numa_levels, परिमाण(k.masks[0]), hop_cmp);
	जइ (!hop_masks)
		गच्छ unlock;
	hop = hop_masks	- k.masks;

	ret = hop ?
		cpumask_nth_and_andnot(cpu - k.w, cpus, k.masks[hop][ग्रन्थिः], k.masks[hop-1][ग्रन्थिः]) :
		cpumask_nth_and(cpu, cpus, k.masks[0][ग्रन्थिः]);
unlock:
	rcu_read_unlock();
	प्रत्यागम ret;
}
EXPORT_चिह्नम्_GPL(sched_numa_find_nth_cpu);

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
ध्रुव संरचना cpumask *sched_numa_hop_mask(unsigned पूर्णाङ्क ग्रन्थिः, unsigned पूर्णाङ्क hops)
{
	संरचना cpumask ***masks;

	जइ (ग्रन्थिः >= nr_ग्रन्थिः_ids || hops >= sched_domains_numa_levels)
		प्रत्यागम ERR_PTR(-EINVAL);

	masks = rcu_dereference(sched_domains_numa_masks);
	जइ (!masks)
		प्रत्यागम ERR_PTR(-EBUSY);

	प्रत्यागम masks[hops][ग्रन्थिः];
}
EXPORT_चिह्नम्_GPL(sched_numa_hop_mask);

#endif /* CONFIG_NUMA */

स्थिर पूर्णाङ्क __sdt_alloc(ध्रुव संरचना cpumask *cpu_map)
{
	संरचना sched_domain_topology_level *tl;
	पूर्णाङ्क j;

	for_each_sd_topology(tl) {
		संरचना sd_data *sdd = &tl->data;

		sdd->sd = alloc_percpu(संरचना sched_domain *);
		जइ (!sdd->sd)
			प्रत्यागम -ENOMEM;

		sdd->sg = alloc_percpu(संरचना sched_group *);
		जइ (!sdd->sg)
			प्रत्यागम -ENOMEM;

		sdd->sgc = alloc_percpu(संरचना sched_group_capacity *);
		जइ (!sdd->sgc)
			प्रत्यागम -ENOMEM;

		for_each_cpu(j, cpu_map) {
			संरचना sched_domain *sd;
			संरचना sched_group *sg;
			संरचना sched_group_capacity *sgc;

			sd = kzalloc_ग्रन्थिः(परिमाण(संरचना sched_domain) + cpumask_size(),
					GFP_KERNEL, cpu_to_ग्रन्थिः(j));
			जइ (!sd)
				प्रत्यागम -ENOMEM;

			*per_cpu_ptr(sdd->sd, j) = sd;

			sg = kzalloc_ग्रन्थिः(परिमाण(संरचना sched_group) + cpumask_size(),
					GFP_KERNEL, cpu_to_ग्रन्थिः(j));
			जइ (!sg)
				प्रत्यागम -ENOMEM;

			sg->next = sg;

			*per_cpu_ptr(sdd->sg, j) = sg;

			sgc = kzalloc_ग्रन्थिः(परिमाण(संरचना sched_group_capacity) + cpumask_size(),
					GFP_KERNEL, cpu_to_ग्रन्थिः(j));
			जइ (!sgc)
				प्रत्यागम -ENOMEM;

			sgc->id = j;

			*per_cpu_ptr(sdd->sgc, j) = sgc;
		}
	}

	प्रत्यागम 0;
}

स्थिर शून्य __sdt_free(ध्रुव संरचना cpumask *cpu_map)
{
	संरचना sched_domain_topology_level *tl;
	पूर्णाङ्क j;

	for_each_sd_topology(tl) {
		संरचना sd_data *sdd = &tl->data;

		for_each_cpu(j, cpu_map) {
			संरचना sched_domain *sd;

			जइ (sdd->sd) {
				sd = *per_cpu_ptr(sdd->sd, j);
				जइ (sd && (sd->flags & SD_NUMA))
					free_sched_groups(sd->groups, 0);
				kfree(*per_cpu_ptr(sdd->sd, j));
			}

			जइ (sdd->sg)
				kfree(*per_cpu_ptr(sdd->sg, j));
			जइ (sdd->sgc)
				kfree(*per_cpu_ptr(sdd->sgc, j));
		}
		free_percpu(sdd->sd);
		sdd->sd = रिक्त;
		free_percpu(sdd->sg);
		sdd->sg = रिक्त;
		free_percpu(sdd->sgc);
		sdd->sgc = रिक्त;
	}
}

स्थिर पूर्णाङ्क __sds_alloc(संरचना s_data *d, ध्रुव संरचना cpumask *cpu_map)
{
	पूर्णाङ्क j;

	d->sds = alloc_percpu(संरचना sched_domain_shared *);
	जइ (!d->sds)
		प्रत्यागम -ENOMEM;

	for_each_cpu(j, cpu_map) {
		संरचना sched_domain_shared *sds;

		sds = kzalloc_ग्रन्थिः(परिमाण(संरचना sched_domain_shared),
				GFP_KERNEL, cpu_to_ग्रन्थिः(j));
		जइ (!sds)
			प्रत्यागम -ENOMEM;

		*per_cpu_ptr(d->sds, j) = sds;
	}

	प्रत्यागम 0;
}

स्थिर शून्य __sds_free(संरचना s_data *d, ध्रुव संरचना cpumask *cpu_map)
{
	पूर्णाङ्क j;

	जइ (!d->sds)
		प्रत्यागम;

	for_each_cpu(j, cpu_map)
		kfree(*per_cpu_ptr(d->sds, j));

	free_percpu(d->sds);
	d->sds = रिक्त;
}

स्थिर संरचना sched_domain *build_sched_domain(संरचना sched_domain_topology_level *tl,
		ध्रुव संरचना cpumask *cpu_map, संरचना sched_domain_attr *attr,
		संरचना sched_domain *child, पूर्णाङ्क cpu)
{
	संरचना sched_domain *sd = sd_init(tl, cpu_map, child, cpu);

	जइ (child) {
		sd->level = child->level + 1;
		sched_domain_level_max = max(sched_domain_level_max, sd->level);
		child->parent = sd;

		जइ (!cpumask_subset(sched_domain_span(child),
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

	प्रत्यागम sd;
}

/*
 * Ensure topology masks are sane, i.e. there are no conflicts (overlaps) for
 * any two given CPUs on non-NUMA topology levels.
 */
स्थिर bool topology_span_sane(ध्रुव संरचना cpumask *cpu_map)
{
	संरचना sched_domain_topology_level *tl;
	संरचना cpumask *covered, *id_seen;
	पूर्णाङ्क cpu;

	lockdep_assert_held(&sched_domains_mutex);
	covered = sched_domains_tmpmask;
	id_seen = sched_domains_tmpmask2;

	for_each_sd_topology(tl) {
		पूर्णाङ्क tl_common_flags = 0;

		जइ (tl->sd_flags)
			tl_common_flags = (*tl->sd_flags)();

		/* NUMA levels are allowed to overlap */
		जइ (tl_common_flags & SD_NUMA)
			अनुवर्त;

		cpumask_clear(covered);
		cpumask_clear(id_seen);

		/*
		 * Non-NUMA levels cannot partially overlap - they must be either
		 * completely equal or completely disjoint. Otherwise we can end up
		 * breaking the sched_group lists - i.e. a later get_group() pass
		 * breaks the linking done for an earlier span.
		 */
		for_each_cpu(cpu, cpu_map) {
			ध्रुव संरचना cpumask *tl_cpu_mask = tl->mask(tl, cpu);
			पूर्णाङ्क id;

			/* lowest bit set in this mask is used as a unique id */
			id = cpumask_first(tl_cpu_mask);

			जइ (cpumask_test_cpu(id, id_seen)) {
				/* First CPU has already been seen, ensure identical spans */
				जइ (!cpumask_equal(tl->mask(tl, id), tl_cpu_mask))
					प्रत्यागम false;
			} अण्णहा {
				/* First CPU hasn't been seen before, ensure it's a completely new span */
				जइ (cpumask_intersects(tl_cpu_mask, covered))
					प्रत्यागम false;

				cpumask_or(covered, covered, tl_cpu_mask);
				cpumask_set_cpu(id, id_seen);
			}
		}
	}
	प्रत्यागम true;
}

/*
 * Calculate an allowed NUMA imbalance such that LLCs do not get
 * imbalanced.
 */
स्थिर शून्य adjust_numa_imbalance(संरचना sched_domain *sd_llc)
{
	संरचना sched_domain *parent;
	unsigned पूर्णाङ्क imb_span = 1;
	unsigned पूर्णाङ्क imb = 0;
	unsigned पूर्णाङ्क nr_llcs;

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
	जइ (nr_llcs == 1)
		imb = sd_llc->parent->span_weight >> 3;
	अण्णहा
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
	जाव (parent && !(parent->flags & SD_NUMA))
		parent = parent->parent;

	imb_span = parent ? parent->span_weight : sd_llc->parent->span_weight;

	/* Update the upper remainder of the topology */
	parent = sd_llc->parent;
	जाव (parent) {
		पूर्णाङ्क factor = max(1U, (parent->span_weight / imb_span));

		parent->imb_numa_nr = imb * factor;
		parent = parent->parent;
	}
}

स्थिर शून्य
init_sched_domain_shared(संरचना s_data *d, संरचना sched_domain *sd, पूर्णाङ्क flags)
{
	संरचना sched_domain_shared *sds = रिक्त;
	पूर्णाङ्क cpu;

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

		जइ (!sds->alloc_flags ||
		    sd->span_weight == 1 ||
		    sds->alloc_flags == flags) {
			sds->alloc_flags = flags;
			sd->shared = sds;
			भङ्ग;
		}
	}

	/*
	 * Use the sd_shared corresponding to the last
	 * CPU in the span if none are avaialable.
	 */
	जइ (WARN_ON_ONCE(!sd->shared))
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
स्थिर bool claim_asym_sched_domain_shared(संरचना s_data *d, पूर्णाङ्क cpu)
{
	संरचना sched_domain *sd = *per_cpu_ptr(d->sd, cpu);
	संरचना sched_domain *sd_asym;

	जइ (!sd)
		प्रत्यागम false;

	sd_asym = sd;
	जाव (sd_asym && !(sd_asym->flags & SD_ASYM_CPUCAPACITY_FULL))
		sd_asym = sd_asym->parent;

	जइ (!sd_asym || (sd_asym->flags & SD_NUMA))
		प्रत्यागम false;

	init_sched_domain_shared(d, sd_asym, SD_ASYM_CPUCAPACITY);
	प्रत्यागम true;
}

स्थिर पूर्णाङ्क __sched_domains_alloc_llc_id(शून्य)
{
	पूर्णाङ्क lid, max;

	lockdep_assert_held(&sched_domains_mutex);

	lid = cpumask_first_zero(sched_domains_llc_id_allocmask);
	/*
	 * llc_id space should never grow larger than the
	 * possible number of CPUs in the system.
	 */
	जइ (lid >= nr_cpu_ids)
		प्रत्यागम -1;

	__cpumask_set_cpu(lid, sched_domains_llc_id_allocmask);
	max = cpumask_last(sched_domains_llc_id_allocmask);
	जइ (max > max_lid)
		max_lid = max;

	प्रत्यागम lid;
}

स्थिर शून्य __sched_domains_free_llc_id(पूर्णाङ्क cpu)
{
	पूर्णाङ्क i, lid, max;

	lockdep_assert_held(&sched_domains_mutex);

	lid = per_cpu(sd_llc_id, cpu);
	जइ (lid == -1 || lid >= nr_cpu_ids)
		प्रत्यागम;

	per_cpu(sd_llc_id, cpu) = -1;

	for_each_cpu(i, llc_mask(cpu)) {
		/* An online CPU owns the llc_id. */
		जइ (per_cpu(sd_llc_id, i) == lid)
			प्रत्यागम;
	}

	__cpumask_clear_cpu(lid, sched_domains_llc_id_allocmask);

	max = cpumask_last(sched_domains_llc_id_allocmask);
	/* shrink max lid to save memory */
	जइ (max < max_lid)
		max_lid = max;
}

शून्य sched_domains_free_llc_id(पूर्णाङ्क cpu)
{
	sched_domains_mutex_lock();
	__sched_domains_free_llc_id(cpu);
	sched_domains_mutex_unlock();
}

/*
 * Build sched domains for a given set of CPUs and attach the sched domains
 * to the individual CPUs
 */
स्थिर पूर्णाङ्क
build_sched_domains(ध्रुव संरचना cpumask *cpu_map, संरचना sched_domain_attr *attr,
		    bool *multi_llcs)
{
	गणना s_alloc alloc_state = sa_none;
	bool has_multi_llcs = false;
	संरचना sched_domain *sd;
	संरचना s_data d;
	संरचना rq *rq = रिक्त;
	पूर्णाङ्क i, ret = -ENOMEM;
	bool has_asym = false;
	bool has_cluster = false;

	जइ (WARN_ON(cpumask_empty(cpu_map)))
		गच्छ error;

	alloc_state = __visit_domain_allocation_hell(&d, cpu_map);
	जइ (alloc_state != sa_rootdomain)
		गच्छ error;

	/* Set up domains for CPUs specified by the cpu_map: */
	for_each_cpu(i, cpu_map) {
		संरचना sched_domain_topology_level *tl;
		पूर्णाङ्क lid;

		sd = रिक्त;
		for_each_sd_topology(tl) {

			sd = build_sched_domain(tl, cpu_map, attr, sd, i);

			has_asym |= sd->flags & SD_ASYM_CPUCAPACITY;

			जइ (tl == sched_domain_topology)
				*per_cpu_ptr(d.sd, i) = sd;
			जइ (cpumask_equal(cpu_map, sched_domain_span(sd)))
				भङ्ग;
		}

		lid = per_cpu(sd_llc_id, i);
		जइ (lid == -1) {
			/* try to reuse the llc_id of its siblings */
			प्रति (पूर्णाङ्क j = cpumask_first(llc_mask(i));
			     j < nr_cpu_ids;
			     j = cpumask_next(j, llc_mask(i))) {
				जइ (i == j)
					अनुवर्त;

				lid = per_cpu(sd_llc_id, j);

				जइ (lid != -1) {
					per_cpu(sd_llc_id, i) = lid;

					भङ्ग;
				}
			}

			/* a new LLC is detected */
			जइ (lid == -1)
				per_cpu(sd_llc_id, i) = __sched_domains_alloc_llc_id();
		}
	}

	जइ (WARN_ON(!topology_span_sane(cpu_map)))
		गच्छ error;

	/* Build the groups for the domains */
	for_each_cpu(i, cpu_map) {
		प्रति (sd = *per_cpu_ptr(d.sd, i); sd; sd = sd->parent) {
			sd->span_weight = cpumask_weight(sched_domain_span(sd));
			जइ (sd->flags & SD_NUMA) {
				जइ (build_overlap_sched_groups(sd, i))
					गच्छ error;
			} अण्णहा {
				जइ (build_sched_groups(sd, i))
					गच्छ error;
			}
		}
	}

	for_each_cpu(i, cpu_map) {
		sd = *per_cpu_ptr(d.sd, i);
		जइ (!sd)
			अनुवर्त;

		जइ (has_asym)
			claim_asym_sched_domain_shared(&d, i);

		/* First, find the topmost SD_SHARE_LLC domain */
		जाव (sd->parent && (sd->parent->flags & SD_SHARE_LLC))
			sd = sd->parent;

		जइ (sd->flags & SD_SHARE_LLC) {
			init_sched_domain_shared(&d, sd, SD_SHARE_LLC);

			/*
			 * In presence of higher domains, adjust the
			 * NUMA imbalance stats for the hierarchy.
			 */
			जइ (sd->parent) {
				जइ (IS_ENABLED(CONFIG_NUMA))
					adjust_numa_imbalance(sd);

				जइ (sd_in_multi_llcs(sd))
					has_multi_llcs = true;
			}
		}
	}

	/* Calculate CPU capacity for physical packages and nodes */
	प्रति (i = nr_cpumask_bits-1; i >= 0; i--) {
		जइ (!cpumask_test_cpu(i, cpu_map))
			अनुवर्त;

		claim_allocations(i, &d);

		प्रति (sd = *per_cpu_ptr(d.sd, i); sd; sd = sd->parent)
			init_sched_groups_capacity(i, sd);
	}

	alloc_sd_llc(cpu_map, &d);

	/* Attach the domains */
	rcu_read_lock();
	for_each_cpu(i, cpu_map) {
		rq = cpu_rq(i);
		sd = *per_cpu_ptr(d.sd, i);

		cpu_attach_domain(sd, d.rd, i);

		जइ (lowest_flag_domain(i, SD_CLUSTER))
			has_cluster = true;
	}
	rcu_read_unlock();

	जइ (has_asym)
		static_branch_inc_cpuslocked(&sched_asym_cpucapacity);

	जइ (has_cluster)
		static_branch_inc_cpuslocked(&sched_cluster_active);

	जइ (rq && sched_debug_verbose)
		pr_info("root domain span: %*pbl\n", cpumask_pr_args(cpu_map));

	ret = 0;
error:
	*multi_llcs = has_multi_llcs;
	__free_domain_allocs(&d, alloc_state, cpu_map);

	प्रत्यागम ret;
}

/* Current sched domains: */
स्थिर cpumask_var_t			*doms_cur;

/* Number of sched domains in 'doms_cur': */
स्थिर पूर्णाङ्क				ndoms_cur;

/* Attributes of custom domains in 'doms_cur' */
स्थिर संरचना sched_domain_attr		*dattr_cur;

/*
 * Special case: If a kmalloc() of a doms_cur partition (array of
 * cpumask) fails, then fallback to a single sched domain,
 * as determined by the single cpumask fallback_doms.
 */
स्थिर cpumask_var_t			fallback_doms;

/*
 * arch_update_cpu_topology lets virtualized architectures update the
 * CPU core maps. It is supposed to return 1 if the topology changed
 * or 0 if it stayed the same.
 */
पूर्णाङ्क __weak arch_update_cpu_topology(शून्य)
{
	प्रत्यागम 0;
}

cpumask_var_t *alloc_sched_domains(unsigned पूर्णाङ्क ndoms)
{
	पूर्णाङ्क i;
	cpumask_var_t *doms;

	doms = kmalloc_objs(*doms, ndoms);
	जइ (!doms)
		प्रत्यागम रिक्त;
	प्रति (i = 0; i < ndoms; i++) {
		जइ (!alloc_cpumask_var(&doms[i], GFP_KERNEL)) {
			free_sched_domains(doms, i);
			प्रत्यागम रिक्त;
		}
	}
	प्रत्यागम doms;
}

शून्य free_sched_domains(cpumask_var_t doms[], unsigned पूर्णाङ्क ndoms)
{
	unsigned पूर्णाङ्क i;
	प्रति (i = 0; i < ndoms; i++)
		free_cpumask_var(doms[i]);
	kfree(doms);
}

/*
 * Set up scheduler domains and groups.  For now this just excludes isolated
 * CPUs, but could be used to exclude other special cases in the future.
 */
पूर्णाङ्क __init sched_init_domains(ध्रुव संरचना cpumask *cpu_map)
{
	bool multi_llcs;
	पूर्णाङ्क err;

	zalloc_cpumask_var(&sched_domains_llc_id_allocmask, GFP_KERNEL);
	zalloc_cpumask_var(&sched_domains_tmpmask, GFP_KERNEL);
	zalloc_cpumask_var(&sched_domains_tmpmask2, GFP_KERNEL);
	zalloc_cpumask_var(&fallback_doms, GFP_KERNEL);

	arch_update_cpu_topology();
	asym_cpu_capacity_scan();
	ndoms_cur = 1;
	doms_cur = alloc_sched_domains(ndoms_cur);
	जइ (!doms_cur)
		doms_cur = &fallback_doms;
	cpumask_and(doms_cur[0], cpu_map, housekeeping_cpumask(HK_TYPE_DOMAIN));
	err = build_sched_domains(doms_cur[0], रिक्त, &multi_llcs);
	जइ (!err)
		sched_cache_set(multi_llcs);

	प्रत्यागम err;
}

/*
 * Detach sched domains from a group of CPUs specified in cpu_map
 * These CPUs will now be attached to the NULL domain
 */
स्थिर शून्य detach_destroy_domains(ध्रुव संरचना cpumask *cpu_map)
{
	unsigned पूर्णाङ्क cpu = cpumask_any(cpu_map);
	पूर्णाङ्क i;

	जइ (rcu_access_pointer(per_cpu(sd_asym_cpucapacity, cpu)))
		static_branch_dec_cpuslocked(&sched_asym_cpucapacity);

	जइ (static_branch_unlikely(&sched_cluster_active))
		static_branch_dec_cpuslocked(&sched_cluster_active);

	rcu_read_lock();
	for_each_cpu(i, cpu_map)
		cpu_attach_domain(रिक्त, &def_मूलम्_domain, i);
	rcu_read_unlock();
}

/* handle null as "default" */
स्थिर पूर्णाङ्क dattrs_equal(संरचना sched_domain_attr *cur, पूर्णाङ्क idx_cur,
			संरचना sched_domain_attr *new, पूर्णाङ्क idx_new)
{
	संरचना sched_domain_attr tmp;

	/* Fast path: */
	जइ (!new && !cur)
		प्रत्यागम 1;

	tmp = SD_ATTR_INIT;

	प्रत्यागम !memcmp(cur ? (cur + idx_cur) : &tmp,
			new ? (new + idx_new) : &tmp,
			परिमाण(संरचना sched_domain_attr));
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
स्थिर शून्य partition_sched_domains_locked(पूर्णाङ्क ndoms_new, cpumask_var_t doms_new[],
				    संरचना sched_domain_attr *dattr_new)
{
	bool __maybe_unused has_eas = false;
	bool has_multi_llcs = false, multi_llcs;
	पूर्णाङ्क i, j, n;
	पूर्णाङ्क new_topology;

	lockdep_assert_held(&sched_domains_mutex);

	/* Let the architecture update CPU core mappings: */
	new_topology = arch_update_cpu_topology();
	/* Trigger rebuilding CPU capacity asymmetry data */
	जइ (new_topology)
		asym_cpu_capacity_scan();

	जइ (!doms_new) {
		WARN_ON_ONCE(dattr_new);
		n = 0;
		doms_new = alloc_sched_domains(1);
		जइ (doms_new) {
			n = 1;
			cpumask_and(doms_new[0], cpu_active_mask,
				    housekeeping_cpumask(HK_TYPE_DOMAIN));
		}
	} अण्णहा {
		n = ndoms_new;
	}

	/* Destroy deleted domains: */
	प्रति (i = 0; i < ndoms_cur; i++) {
		प्रति (j = 0; j < n && !new_topology; j++) {
			जइ (cpumask_equal(doms_cur[i], doms_new[j]) &&
			    dattrs_equal(dattr_cur, i, dattr_new, j))
				गच्छ match1;
		}
		/* No match - a current sched domain not in new doms_new[] */
		detach_destroy_domains(doms_cur[i]);
match1:
		;
	}

	n = ndoms_cur;
	जइ (!doms_new) {
		n = 0;
		doms_new = &fallback_doms;
		cpumask_and(doms_new[0], cpu_active_mask,
			    housekeeping_cpumask(HK_TYPE_DOMAIN));
	}

	/* Build new domains: */
	प्रति (i = 0; i < ndoms_new; i++) {
		प्रति (j = 0; j < n && !new_topology; j++) {
			जइ (cpumask_equal(doms_new[i], doms_cur[j]) &&
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
				संरचना sched_domain *sd;
				पूर्णाङ्क cpu = cpumask_first(doms_cur[j]);

				guard(rcu)();
				sd = rcu_dereference(cpu_rq(cpu)->sd);
				जाव (sd && sd->parent && (sd->parent->flags & SD_SHARE_LLC))
					sd = sd->parent;
				जइ (sd && (sd->flags & SD_SHARE_LLC) && sd->parent &&
				    sd_in_multi_llcs(sd))
					has_multi_llcs = true;
				गच्छ match2;
			}
		}
		/* No match - add a new doms_new */
		build_sched_domains(doms_new[i], dattr_new ? dattr_new + i : रिक्त,
				    &multi_llcs);
		has_multi_llcs |= multi_llcs;
match2:
		;
	}
	sched_cache_set(has_multi_llcs);

#जइ defined(CONFIG_ENERGY_MODEL) && defined(CONFIG_CPU_FREQ_GOV_SCHEDUTIL)
	/* Build perf domains: */
	प्रति (i = 0; i < ndoms_new; i++) {
		प्रति (j = 0; j < n && !sched_energy_update; j++) {
			जइ (cpumask_equal(doms_new[i], doms_cur[j]) &&
			    cpu_rq(cpumask_first(doms_cur[j]))->rd->pd) {
				has_eas = true;
				गच्छ match3;
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
	जइ (doms_cur != &fallback_doms)
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
शून्य partition_sched_domains(पूर्णाङ्क ndoms_new, cpumask_var_t doms_new[],
			     संरचना sched_domain_attr *dattr_new)
{
	sched_domains_mutex_lock();
	partition_sched_domains_locked(ndoms_new, doms_new, dattr_new);
	sched_domains_mutex_unlock();
}
