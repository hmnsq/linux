// SPDX-License-Identifier: GPL-2.0
/*
 * Scheduler topology setup/handling methods
 */

#समावेश <linux/sched/isolation.h>
#समावेश <linux/sched/clock.h>
#समावेश <linux/bsearch.h>
#समावेश "sched.h"

DEFINE_MUTEX(sched_domains_mutex);
व्योम sched_domains_mutex_lock(व्योम)
{
	mutex_lock(&sched_domains_mutex);
}
व्योम sched_domains_mutex_unlock(व्योम)
{
	mutex_unlock(&sched_domains_mutex);
}

/* Protected by sched_domains_mutex: */
अटल cpumask_var_t sched_domains_llc_id_allocmask;
अटल cpumask_var_t sched_domains_tmpmask;
अटल cpumask_var_t sched_domains_tmpmask2;
पूर्णांक max_lid;

अटल पूर्णांक __init sched_debug_setup(अक्षर *str)
{
	sched_debug_verbose = true;

	वापस 0;
}
early_param("sched_verbose", sched_debug_setup);

#अगर_घोषित CONFIG_SCHED_SMT
अटल bool sched_smt_asym_packing __read_mostly;

अटल पूर्णांक __init setup_sched_smt_asym_packing(अक्षर *str)
{
	अगर (म_भेद(str, "on"))
		वापस 0;

	sched_smt_asym_packing = true;
	वापस 1;
}
__setup("sched_smt_asym_packing=", setup_sched_smt_asym_packing);
#पूर्ण_अगर

अटल अंतरभूत bool sched_debug(व्योम)
{
	वापस sched_debug_verbose;
}

#घोषणा SD_FLAG(_name, mflags) [__##_name] = { .meta_flags = mflags, .name = #_name },
स्थिर काष्ठा sd_flag_debug sd_flag_debug[] = {
#समावेश <linux/sched/sd_flags.h>
};
#अघोषित SD_FLAG

अटल पूर्णांक sched_domain_debug_one(काष्ठा sched_domain *sd, पूर्णांक cpu, पूर्णांक level,
				  काष्ठा cpumask *groupmask)
{
	काष्ठा sched_group *group = sd->groups;
	अचिन्हित दीर्घ flags = sd->flags;
	अचिन्हित पूर्णांक idx;

	cpumask_clear(groupmask);

	printk(KERN_DEBUG "%*s domain-%d: ", level, "", level);
	printk(KERN_CONT "span=%*pbl level=%s\n",
	       cpumask_pr_args(sched_domain_span(sd)), sd->name);

	अगर (!cpumask_test_cpu(cpu, sched_domain_span(sd))) {
		printk(KERN_ERR "ERROR: domain->span does not contain CPU%d\n", cpu);
	}
	अगर (group && !cpumask_test_cpu(cpu, sched_group_span(group))) {
		printk(KERN_ERR "ERROR: domain->groups does not contain CPU%d\n", cpu);
	}

	for_each_set_bit(idx, &flags, __SD_FLAG_CNT) {
		अचिन्हित पूर्णांक flag = BIT(idx);
		अचिन्हित पूर्णांक meta_flags = sd_flag_debug[idx].meta_flags;

		अगर ((meta_flags & SDF_SHARED_CHILD) && sd->child &&
		    !(sd->child->flags & flag))
			printk(KERN_ERR "ERROR: flag %s set here but not in child\n",
			       sd_flag_debug[idx].name);

		अगर ((meta_flags & SDF_SHARED_PARENT) && sd->parent &&
		    !(sd->parent->flags & flag))
			printk(KERN_ERR "ERROR: flag %s set here but not in parent\n",
			       sd_flag_debug[idx].name);
	}

	printk(KERN_DEBUG "%*s groups:", level + 1, "");
	करो {
		अगर (!group) {
			printk("\n");
			printk(KERN_ERR "ERROR: group is NULL\n");
			अवरोध;
		}

		अगर (cpumask_empty(sched_group_span(group))) {
			printk(KERN_CONT "\n");
			printk(KERN_ERR "ERROR: empty group\n");
			अवरोध;
		}

		अगर (!(sd->flags & SD_NUMA) &&
		    cpumask_intersects(groupmask, sched_group_span(group))) {
			printk(KERN_CONT "\n");
			printk(KERN_ERR "ERROR: repeated CPUs\n");
			अवरोध;
		}

		cpumask_or(groupmask, groupmask, sched_group_span(group));

		printk(KERN_CONT " %d:{ span=%*pbl",
				group->sgc->id,
				cpumask_pr_args(sched_group_span(group)));

		अगर ((sd->flags & SD_NUMA) &&
		    !cpumask_equal(group_balance_mask(group), sched_group_span(group))) {
			printk(KERN_CONT " mask=%*pbl",
				cpumask_pr_args(group_balance_mask(group)));
		}

		अगर (group->sgc->capacity != SCHED_CAPACITY_SCALE)
			printk(KERN_CONT " cap=%lu", group->sgc->capacity);

		अगर (group == sd->groups && sd->child &&
		    !cpumask_equal(sched_domain_span(sd->child),
				   sched_group_span(group))) {
			printk(KERN_ERR "ERROR: domain->groups does not match domain->child\n");
		}

		printk(KERN_CONT " }");

		group = group->अग्र;

		अगर (group != sd->groups)
			printk(KERN_CONT ",");

	} जबतक (group != sd->groups);
	printk(KERN_CONT "\n");

	अगर (!cpumask_equal(sched_domain_span(sd), groupmask))
		printk(KERN_ERR "ERROR: groups don't span domain->span\n");

	अगर (sd->parent &&
	    !cpumask_subset(groupmask, sched_domain_span(sd->parent)))
		printk(KERN_ERR "ERROR: parent span is not a superset of domain->span\n");
	वापस 0;
}

अटल व्योम sched_domain_debug(काष्ठा sched_domain *sd, पूर्णांक cpu)
{
	पूर्णांक level = 0;

	अगर (!sched_debug_verbose)
		वापस;

	अगर (!sd) {
		printk(KERN_DEBUG "CPU%d attaching NULL sched-domain.\n", cpu);
		वापस;
	}

	printk(KERN_DEBUG "CPU%d attaching sched-domain(s):\n", cpu);

	क्रम (;;) {
		अगर (sched_domain_debug_one(sd, cpu, level, sched_domains_tmpmask))
			अवरोध;
		level++;
		sd = sd->parent;
		अगर (!sd)
			अवरोध;
	}
}

/* Generate a mask of SD flags with the SDF_NEEDS_GROUPS metaflag */
#घोषणा SD_FLAG(name, mflags) (name * !!((mflags) & SDF_NEEDS_GROUPS)) |
अटल स्थिर अचिन्हित पूर्णांक SD_DEGENERATE_GROUPS_MASK =
#समावेश <linux/sched/sd_flags.h>
0;
#अघोषित SD_FLAG

अटल पूर्णांक sd_degenerate(काष्ठा sched_domain *sd)
{
	अगर (cpumask_weight(sched_domain_span(sd)) == 1)
		वापस 1;

	/* Following flags need at least 2 groups */
	अगर ((sd->flags & SD_DEGENERATE_GROUPS_MASK) &&
	    (sd->groups != sd->groups->अग्र))
		वापस 0;

	/* Following flags don't use groups */
	अगर (sd->flags & (SD_WAKE_AFFINE))
		वापस 0;

	वापस 1;
}

अटल पूर्णांक
sd_parent_degenerate(काष्ठा sched_domain *sd, काष्ठा sched_domain *parent)
{
	अचिन्हित दीर्घ cflags = sd->flags, pflags = parent->flags;

	अगर (sd_degenerate(parent))
		वापस 1;

	अगर (!cpumask_equal(sched_domain_span(sd), sched_domain_span(parent)))
		वापस 0;

	/* Flags needing groups don't count if only 1 group in parent */
	अगर (parent->groups == parent->groups->अग्र)
		pflags &= ~SD_DEGENERATE_GROUPS_MASK;

	अगर (~cflags & pflags)
		वापस 0;

	वापस 1;
}

#अगर defined(CONFIG_ENERGY_MODEL) && defined(CONFIG_CPU_FREQ_GOV_SCHEDUTIL)
DEFINE_STATIC_कुंजी_FALSE(sched_energy_present);
अटल अचिन्हित पूर्णांक sysctl_sched_energy_aware = 1;
अटल DEFINE_MUTEX(sched_energy_mutex);
अटल bool sched_energy_update;

अटल bool sched_is_eas_possible(स्थिर काष्ठा cpumask *cpu_mask)
{
	bool any_asym_capacity = false;
	पूर्णांक i;

	/* EAS is enabled for asymmetric CPU capacity topologies. */
	for_each_cpu(i, cpu_mask) {
		अगर (rcu_access_pointer(per_cpu(sd_asym_cpucapacity, i))) {
			any_asym_capacity = true;
			अवरोध;
		}
	}
	अगर (!any_asym_capacity) {
		अगर (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS, CPUs do not have asymmetric capacities\n",
				cpumask_pr_args(cpu_mask));
		}
		वापस false;
	}

	/* EAS definitely does *not* handle SMT */
	अगर (sched_smt_active()) {
		अगर (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS, SMT is not supported\n",
				cpumask_pr_args(cpu_mask));
		}
		वापस false;
	}

	अगर (!arch_scale_freq_invariant()) {
		अगर (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS: frequency-invariant load tracking not yet supported",
				cpumask_pr_args(cpu_mask));
		}
		वापस false;
	}

	अगर (!cpufreq_ready_for_eas(cpu_mask)) {
		अगर (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS: cpufreq is not ready\n",
				cpumask_pr_args(cpu_mask));
		}
		वापस false;
	}

	वापस true;
}

व्योम rebuild_sched_domains_energy(व्योम)
{
	mutex_lock(&sched_energy_mutex);
	sched_energy_update = true;
	rebuild_sched_domains();
	sched_energy_update = false;
	mutex_unlock(&sched_energy_mutex);
}

#अगर_घोषित CONFIG_SYSCTL
अटल पूर्णांक sched_energy_aware_handler(स्थिर काष्ठा ctl_table *table, पूर्णांक ग_लिखो,
		व्योम *बफर, माप_प्रकार *lenp, loff_t *ppos)
{
	पूर्णांक ret;

	अगर (ग_लिखो && !capable(CAP_SYS_ADMIN))
		वापस -EPERM;

	अगर (!sched_is_eas_possible(cpu_active_mask)) {
		अगर (ग_लिखो) {
			वापस -EOPNOTSUPP;
		} अन्यथा {
			*lenp = 0;
			वापस 0;
		}
	}

	ret = proc_dointvec_minmax(table, ग_लिखो, बफर, lenp, ppos);
	अगर (!ret && ग_लिखो) {
		अगर (sysctl_sched_energy_aware != sched_energy_enabled())
			rebuild_sched_domains_energy();
	}

	वापस ret;
}

अटल स्थिर काष्ठा ctl_table sched_energy_aware_sysctls[] = {
	{
		.procname       = "sched_energy_aware",
		.तथ्य           = &sysctl_sched_energy_aware,
		.maxlen         = माप(अचिन्हित पूर्णांक),
		.mode           = 0644,
		.proc_handler   = sched_energy_aware_handler,
		.extra1         = SYSCTL_ZERO,
		.extra2         = SYSCTL_ONE,
	},
};

अटल पूर्णांक __init sched_energy_aware_sysctl_init(व्योम)
{
	register_sysctl_init("kernel", sched_energy_aware_sysctls);
	वापस 0;
}

late_initcall(sched_energy_aware_sysctl_init);
#पूर्ण_अगर /* CONFIG_SYSCTL */

अटल व्योम free_pd(काष्ठा perf_domain *pd)
{
	काष्ठा perf_domain *tmp;

	जबतक (pd) {
		tmp = pd->अग्र;
		kfree(pd);
		pd = tmp;
	}
}

अटल काष्ठा perf_domain *find_pd(काष्ठा perf_domain *pd, पूर्णांक cpu)
{
	जबतक (pd) {
		अगर (cpumask_test_cpu(cpu, perf_domain_span(pd)))
			वापस pd;
		pd = pd->अग्र;
	}

	वापस शून्य;
}

अटल काष्ठा perf_domain *pd_init(पूर्णांक cpu)
{
	काष्ठा em_perf_domain *obj = em_cpu_get(cpu);
	काष्ठा perf_domain *pd;

	अगर (!obj) {
		अगर (sched_debug())
			pr_info("%s: no EM found for CPU%d\n", __func__, cpu);
		वापस शून्य;
	}

	pd = kzalloc_obj(*pd);
	अगर (!pd)
		वापस शून्य;
	pd->em_pd = obj;

	वापस pd;
}

अटल व्योम perf_domain_debug(स्थिर काष्ठा cpumask *cpu_map,
						काष्ठा perf_domain *pd)
{
	अगर (!sched_debug() || !pd)
		वापस;

	printk(KERN_DEBUG "root_domain %*pbl:", cpumask_pr_args(cpu_map));

	जबतक (pd) {
		printk(KERN_CONT " pd%d:{ cpus=%*pbl nr_pstate=%d }",
				cpumask_first(perf_domain_span(pd)),
				cpumask_pr_args(perf_domain_span(pd)),
				em_pd_nr_perf_states(pd->em_pd));
		pd = pd->अग्र;
	}

	printk(KERN_CONT "\n");
}

अटल व्योम destroy_perf_domain_rcu(काष्ठा rcu_शीर्ष *rp)
{
	काष्ठा perf_domain *pd;

	pd = container_of(rp, काष्ठा perf_domain, rcu);
	free_pd(pd);
}

अटल व्योम sched_energy_set(bool has_eas)
{
	अगर (!has_eas && sched_energy_enabled()) {
		अगर (sched_debug())
			pr_info("%s: stopping EAS\n", __func__);
		static_branch_disable_cpuslocked(&sched_energy_present);
	} अन्यथा अगर (has_eas && !sched_energy_enabled()) {
		अगर (sched_debug())
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
अटल bool build_perf_domains(स्थिर काष्ठा cpumask *cpu_map)
{
	पूर्णांक i;
	काष्ठा perf_domain *pd = शून्य, *tmp;
	पूर्णांक cpu = cpumask_first(cpu_map);
	काष्ठा मूल_domain *rd = cpu_rq(cpu)->rd;

	अगर (!sysctl_sched_energy_aware)
		जाओ मुक्त;

	अगर (!sched_is_eas_possible(cpu_map))
		जाओ मुक्त;

	for_each_cpu(i, cpu_map) {
		/* Skip already covered CPUs. */
		अगर (find_pd(pd, i))
			जारी;

		/* Create the new pd and add it to the local list. */
		tmp = pd_init(i);
		अगर (!tmp)
			जाओ मुक्त;
		tmp->अग्र = pd;
		pd = tmp;
	}

	perf_domain_debug(cpu_map, pd);

	/* Attach the new list of performance domains to the root domain. */
	tmp = rd->pd;
	rcu_assign_pointer(rd->pd, pd);
	अगर (tmp)
		call_rcu(&tmp->rcu, destroy_perf_domain_rcu);

	वापस !!pd;

मुक्त:
	free_pd(pd);
	tmp = rd->pd;
	rcu_assign_pointer(rd->pd, शून्य);
	अगर (tmp)
		call_rcu(&tmp->rcu, destroy_perf_domain_rcu);

	वापस false;
}
#अन्यथा /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL): */
अटल व्योम free_pd(काष्ठा perf_domain *pd) { }
#पूर्ण_अगर /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL) */

अटल व्योम free_rootdomain(काष्ठा rcu_शीर्ष *rcu)
{
	काष्ठा मूल_domain *rd = container_of(rcu, काष्ठा मूल_domain, rcu);

	cpupri_cleanup(&rd->cpupri);
	cpudl_cleanup(&rd->cpudl);
	free_cpumask_var(rd->dlo_mask);
	free_cpumask_var(rd->rto_mask);
	free_cpumask_var(rd->online);
	free_cpumask_var(rd->span);
	free_pd(rd->pd);
	kfree(rd);
}

व्योम rq_attach_मूल(काष्ठा rq *rq, काष्ठा मूल_domain *rd)
{
	काष्ठा मूल_domain *old_rd = शून्य;
	काष्ठा rq_flags rf;

	rq_lock_irqsave(rq, &rf);

	अगर (rq->rd) {
		old_rd = rq->rd;

		अगर (cpumask_test_cpu(rq->cpu, old_rd->online))
			set_rq_offline(rq);

		cpumask_clear_cpu(rq->cpu, old_rd->span);

		/*
		 * If we don't want to free the old_rd yet then
		 * set old_rd to NULL to skip the freeing later
		 * in this function:
		 */
		अगर (!atomic_dec_and_test(&old_rd->refcount))
			old_rd = शून्य;
	}

	atomic_inc(&rd->refcount);
	rq->rd = rd;

	cpumask_set_cpu(rq->cpu, rd->span);
	अगर (cpumask_test_cpu(rq->cpu, cpu_active_mask))
		set_rq_online(rq);

	/*
	 * Because the rq is not a task, dl_add_task_root_domain() did not
	 * move the fair server bw to the rd if it already started.
	 * Add it now.
	 */
	अगर (rq->fair_server.dl_server)
		__dl_server_attach_root(&rq->fair_server, rq);

#अगर_घोषित CONFIG_SCHED_CLASS_EXT
	अगर (rq->ext_server.dl_server)
		__dl_server_attach_root(&rq->ext_server, rq);
#पूर्ण_अगर

	rq_unlock_irqrestore(rq, &rf);

	अगर (old_rd)
		call_rcu(&old_rd->rcu, free_rootdomain);
}

व्योम sched_get_rd(काष्ठा मूल_domain *rd)
{
	atomic_inc(&rd->refcount);
}

व्योम sched_put_rd(काष्ठा मूल_domain *rd)
{
	अगर (!atomic_dec_and_test(&rd->refcount))
		वापस;

	call_rcu(&rd->rcu, free_rootdomain);
}

अटल पूर्णांक init_rootdomain(काष्ठा मूल_domain *rd)
{
	अगर (!zalloc_cpumask_var(&rd->span, GFP_KERNEL))
		जाओ out;
	अगर (!zalloc_cpumask_var(&rd->online, GFP_KERNEL))
		जाओ free_span;
	अगर (!zalloc_cpumask_var(&rd->dlo_mask, GFP_KERNEL))
		जाओ free_online;
	अगर (!zalloc_cpumask_var(&rd->rto_mask, GFP_KERNEL))
		जाओ free_dlo_mask;

#अगर_घोषित HAVE_RT_धकेलो_IPI
	rd->rto_cpu = -1;
	raw_spin_lock_init(&rd->rto_lock);
	rd->rto_धकेलो_work = IRQ_WORK_INIT_HARD(rto_धकेलो_irq_work_func);
#पूर्ण_अगर

	rd->visit_cookie = 0;
	init_dl_bw(&rd->dl_bw);
	अगर (cpudl_init(&rd->cpudl) != 0)
		जाओ free_rto_mask;

	अगर (cpupri_init(&rd->cpupri) != 0)
		जाओ free_cpudl;
	वापस 0;

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
	वापस -ENOMEM;
}

/*
 * By default the system creates a single root-domain with all CPUs as
 * members (mimicking the global state we have today).
 */
काष्ठा मूल_domain def_मूल_domain;

व्योम __init init_defrootdomain(व्योम)
{
	init_rootdomain(&def_मूल_domain);

	atomic_set(&def_मूल_domain.refcount, 1);
}

अटल काष्ठा मूल_domain *alloc_rootdomain(व्योम)
{
	काष्ठा मूल_domain *rd;

	rd = kzalloc_obj(*rd);
	अगर (!rd)
		वापस शून्य;

	अगर (init_rootdomain(rd) != 0) {
		kfree(rd);
		वापस शून्य;
	}

	वापस rd;
}

अटल व्योम free_sched_groups(काष्ठा sched_group *sg, पूर्णांक free_sgc)
{
	काष्ठा sched_group *tmp, *first;

	अगर (!sg)
		वापस;

	first = sg;
	करो {
		tmp = sg->अग्र;

		अगर (free_sgc && atomic_dec_and_test(&sg->sgc->ref))
			kfree(sg->sgc);

		अगर (atomic_dec_and_test(&sg->ref))
			kfree(sg);
		sg = tmp;
	} जबतक (sg != first);
}

अटल व्योम free_sched_domain_shared(काष्ठा sched_domain_shared *sds)
{
	अगर (sds && atomic_dec_and_test(&sds->ref))
		kfree(sds);
}

अटल व्योम destroy_sched_domain(काष्ठा sched_domain *sd)
{
	/*
	 * A normal sched domain may have multiple group references, an
	 * overlapping domain, having private groups, only one.  Iterate,
	 * dropping group/capacity references, freeing where none remain.
	 */
	free_sched_groups(sd->groups, 1);
	free_sched_domain_shared(sd->shared);

#अगर_घोषित CONFIG_SCHED_CACHE
	/* only the bottom sd has llc_counts array */
	kfree(sd->llc_counts);
#पूर्ण_अगर
	kfree(sd);
}

अटल व्योम destroy_sched_domains_rcu(काष्ठा rcu_शीर्ष *rcu)
{
	काष्ठा sched_domain *sd = container_of(rcu, काष्ठा sched_domain, rcu);

	जबतक (sd) {
		काष्ठा sched_domain *parent = sd->parent;
		destroy_sched_domain(sd);
		sd = parent;
	}
}

अटल व्योम destroy_sched_domains(काष्ठा sched_domain *sd)
{
	अगर (sd)
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
DEFINE_PER_CPU(काष्ठा sched_domain __rcu *, sd_llc);
DEFINE_PER_CPU(पूर्णांक, sd_llc_आकार);
DEFINE_PER_CPU(पूर्णांक, sd_llc_id) = -1;
DEFINE_PER_CPU(पूर्णांक, sd_share_id);
DEFINE_PER_CPU(काष्ठा sched_domain_shared __rcu *, sd_llc_shared);
DEFINE_PER_CPU(काष्ठा sched_domain_shared __rcu *, sd_balance_shared);
DEFINE_PER_CPU(काष्ठा sched_domain __rcu *, sd_numa);
DEFINE_PER_CPU(काष्ठा sched_domain __rcu *, sd_asym_packing);
DEFINE_PER_CPU(काष्ठा sched_domain __rcu *, sd_asym_cpucapacity);

DEFINE_STATIC_कुंजी_FALSE(sched_asym_cpucapacity);
DEFINE_STATIC_कुंजी_FALSE(sched_cluster_active);

अटल व्योम update_शीर्ष_cache_domain(पूर्णांक cpu)
{
	काष्ठा sched_domain_shared *sds = शून्य;
	काष्ठा sched_domain *sd;
	पूर्णांक id = cpu;
	पूर्णांक आकार = 1;

	sd = highest_flag_domain(cpu, SD_SHARE_LLC);
	अगर (sd) {
		id = cpumask_first(sched_domain_span(sd));
		आकार = cpumask_weight(sched_domain_span(sd));

		/* If sd_llc exists, sd_llc_shared should exist too. */
		WARN_ON_ONCE(!sd->shared);
		sds = sd->shared;
	}

	rcu_assign_pointer(per_cpu(sd_llc, cpu), sd);
	per_cpu(sd_llc_आकार, cpu) = आकार;
	rcu_assign_pointer(per_cpu(sd_llc_shared, cpu), sds);

	sd = lowest_flag_domain(cpu, SD_CLUSTER);
	अगर (sd)
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
	अगर (sd && sd->shared)
		sds = sd->shared;

	rcu_assign_pointer(per_cpu(sd_asym_cpucapacity, cpu), sd);
	rcu_assign_pointer(per_cpu(sd_balance_shared, cpu), sds);
}

/*
 * Attach the domain 'sd' to 'cpu' as its base domain. Callers must
 * hold the hotplug lock.
 */
अटल व्योम
cpu_attach_domain(काष्ठा sched_domain *sd, काष्ठा मूल_domain *rd, पूर्णांक cpu)
{
	काष्ठा rq *rq = cpu_rq(cpu);
	काष्ठा sched_domain *tmp;

	/* Remove the sched domains which do not contribute to scheduling. */
	क्रम (tmp = sd; tmp; ) {
		काष्ठा sched_domain *parent = tmp->parent;
		अगर (!parent)
			अवरोध;

		अगर (sd_parent_degenerate(tmp, parent)) {
			tmp->parent = parent->parent;

			/* Pick reference to parent->shared. */
			अगर (parent->shared) {
				/*
				 * It is safe to free a sd->shared that
				 * has not been published yet. If a
				 * sd->shared was published, the refcount
				 * will end up being non-zero and it will
				 * not be freed here.
				 */
				free_sched_domain_shared(tmp->shared);
				tmp->shared = parent->shared;
				parent->shared = शून्य;
			}

			अगर (parent->parent) {
				parent->parent->child = tmp;
				parent->parent->groups->flags = tmp->flags;
			}

			/*
			 * Transfer SD_PREFER_SIBLING down in case of a
			 * degenerate parent; the spans match for this
			 * so the property transfers.
			 */
			अगर (parent->flags & SD_PREFER_SIBLING)
				tmp->flags |= SD_PREFER_SIBLING;
			destroy_sched_domain(parent);
		} अन्यथा
			tmp = tmp->parent;
	}

	अगर (sd && sd_degenerate(sd)) {
		tmp = sd;
		sd = sd->parent;

		अगर (sd) {
			काष्ठा sched_group *sg = sd->groups;

#अगर_घोषित CONFIG_SCHED_CACHE
			/* move buffer to parent as child is being destroyed */
			sd->llc_counts = tmp->llc_counts;
			sd->llc_max = tmp->llc_max;
			sd->llc_bytes = tmp->llc_bytes;
			/* make sure destroy_sched_domain() does not free it */
			tmp->llc_counts = शून्य;
			tmp->llc_max = 0;
			tmp->llc_bytes = 0;
#पूर्ण_अगर
			/*
			 * sched groups hold the flags of the child sched
			 * domain for convenience. Clear such flags since
			 * the child is being destroyed.
			 */
			करो {
				sg->flags = 0;
			} जबतक (sg != sd->groups);

			sd->child = शून्य;
		}

		destroy_sched_domain(tmp);
	}

	sched_domain_debug(sd, cpu);

	rq_attach_मूल(rq, rd);
	tmp = rq->sd;
	rcu_assign_pointer(rq->sd, sd);
	dirty_sched_domain_sysctl(cpu);
	destroy_sched_domains(tmp);

	update_शीर्ष_cache_domain(cpu);
}

काष्ठा s_तथ्य {
	काष्ठा sched_domain_shared * __percpu *sds;
	काष्ठा sched_domain * __percpu *sd;
	काष्ठा मूल_domain	*rd;
};

क्रमागत s_alloc {
	sa_rootdomain,
	sa_sd,
	sa_sd_shared,
	sa_sd_storage,
	sa_none,
};

#अगर_घोषित CONFIG_SCHED_CACHE
/* hardware support for cache aware scheduling */
DEFINE_STATIC_कुंजी_FALSE(sched_cache_present);
/*
 * Indicator of whether cache aware scheduling
 * is active, used by the scheduler.
 */
DEFINE_STATIC_कुंजी_FALSE(sched_cache_active);
/* user wants cache aware scheduling [0 or 1] */
पूर्णांक sysctl_sched_cache_user = 1;

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
अटल अचिन्हित दीर्घ get_effective_llc_bytes(पूर्णांक cpu,
					     काष्ठा sched_domain *sd)
{
	काष्ठा cacheinfo *ci;
	अचिन्हित पूर्णांक hw_weight;

	ci = get_cpu_cacheinfo_llc(cpu);
	अगर (!ci)
		वापस 0;

	hw_weight = cpumask_weight(&ci->shared_cpu_map);
	अगर (!hw_weight)
		वापस 0;

	वापस div_u64((u64)ci->आकार * sd->span_weight, hw_weight);
}

अटल bool alloc_sd_llc(स्थिर काष्ठा cpumask *cpu_map,
			 काष्ठा s_तथ्य *d)
{
	काष्ठा sched_domain *sd, *शीर्ष_llc, *parent;
	अचिन्हित पूर्णांक *p;
	पूर्णांक i;

	for_each_cpu(i, cpu_map) {
		sd = *per_cpu_ptr(d->sd, i);
		अगर (!sd)
			जाओ err;

		p = kcalloc_गाँठ(max_lid + 1, माप(अचिन्हित पूर्णांक),
				 GFP_KERNEL, cpu_to_गाँठ(i));
		अगर (!p)
			जाओ err;

		शीर्ष_llc = sd;
		/*
		 * Find the topmost SD_SHARE_LLC domain.
		 * Not yet attached to the CPU, so per_cpu(sd_llc, i)
		 * can not be used.
		 */
		जबतक ((parent = rcu_dereference_protected(शीर्ष_llc->parent, true)) &&
		       (parent->flags & SD_SHARE_LLC))
			शीर्ष_llc = parent;

		अगर (शीर्ष_llc->flags & SD_SHARE_LLC) {
			sd->llc_max = max_lid + 1;
			sd->llc_counts = p;
			sd->llc_bytes = get_effective_llc_bytes(i, शीर्ष_llc);
		} अन्यथा {
			/* avoid memory leak */
			kfree(p);
		}
	}

	वापस true;
err:
	for_each_cpu(i, cpu_map) {
		sd = *per_cpu_ptr(d->sd, i);
		अगर (sd) {
			kfree(sd->llc_counts);
			sd->llc_counts = शून्य;
			sd->llc_max = 0;
			sd->llc_bytes = 0;
		}
	}

	वापस false;
}

/*
 * Enable/disable cache aware scheduling according to
 * user input and the presence of hardware support.
 */
अटल व्योम _sched_cache_active_set(व्योम)
{
	lockdep_assert_cpus_held();
	lockdep_assert_held(&sched_domains_mutex);

	/* hardware does not support */
	अगर (!static_branch_likely(&sched_cache_present)) {
		static_branch_disable_cpuslocked(&sched_cache_active);
		अगर (sched_debug())
			pr_info("%s: cache aware scheduling not supported on this platform\n", __func__);
		वापस;
	}

	/*
	 * user wants it or not ?
	 * TBD: read before writing the static key.
	 * It is not in the critical path, leave as-is
	 * for now.
	 */
	अगर (sysctl_sched_cache_user) {
		static_branch_enable_cpuslocked(&sched_cache_active);
		अगर (sched_debug())
			pr_info("%s: enabling cache aware scheduling\n", __func__);
	} अन्यथा {
		static_branch_disable_cpuslocked(&sched_cache_active);
		अगर (sched_debug())
			pr_info("%s: disabling cache aware scheduling\n", __func__);
	}
}

/* used by debugfs */
व्योम sched_cache_active_set(व्योम)
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
व्योम sched_update_llc_bytes(स्थिर काष्ठा cpumask *cpus)
{
	काष्ठा sched_domain *sd, *sdp;
	अचिन्हित पूर्णांक i;

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
		अगर (!sdp)
			जारी;

		sd = rcu_dereference_sched_domain(cpu_rq(i)->sd);
		अगर (sd)
			sd->llc_bytes = get_effective_llc_bytes(i, sdp);
	}

	sched_domains_mutex_unlock();
}

अटल व्योम sched_cache_set(bool has_multi_llcs)
{
	/*
	 * TBD: check before writing to it. sched domain rebuild
	 * is not in the critical path, leave as-is for now.
	 */
	अगर (has_multi_llcs)
		static_branch_enable_cpuslocked(&sched_cache_present);
	अन्यथा
		static_branch_disable_cpuslocked(&sched_cache_present);

	_sched_cache_active_set();
}
#अन्यथा
अटल bool alloc_sd_llc(स्थिर काष्ठा cpumask *cpu_map,
			 काष्ठा s_तथ्य *d)
{
	वापस false;
}
अटल अंतरभूत व्योम sched_cache_set(bool has_multi_llcs) { }
#पूर्ण_अगर

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
अटल bool sd_in_multi_llcs(काष्ठा sched_domain *sd)
{
	काष्ठा sched_domain *sdp = sd->parent;

	/* it does not make sense to aggregate to 1 CPU */
	अगर (sd->span_weight == 1)
		वापस false;

	जबतक (sdp && sdp->span_weight == sd->span_weight)
		sdp = sdp->parent;

	वापस !!sdp;
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
पूर्णांक group_balance_cpu(काष्ठा sched_group *sg)
{
	वापस cpumask_first(group_balance_mask(sg));
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
अटल व्योम
build_balance_mask(काष्ठा sched_domain *sd, काष्ठा sched_group *sg, काष्ठा cpumask *mask)
{
	स्थिर काष्ठा cpumask *sg_span = sched_group_span(sg);
	काष्ठा sd_तथ्य *sdd = sd->निजी;
	काष्ठा sched_domain *sibling;
	पूर्णांक i;

	cpumask_clear(mask);

	for_each_cpu(i, sg_span) {
		sibling = *per_cpu_ptr(sdd->sd, i);

		/*
		 * Can happen in the asymmetric case, where these siblings are
		 * unused. The mask will not be empty because those CPUs that
		 * do have the top domain _should_ span the domain.
		 */
		अगर (!sibling->child)
			जारी;

		/* If we would not end up here, we can't continue from here */
		अगर (!cpumask_equal(sg_span, sched_domain_span(sibling->child)))
			जारी;

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
अटल काष्ठा sched_group *
build_group_from_child_sched_domain(काष्ठा sched_domain *sd, पूर्णांक cpu)
{
	काष्ठा sched_group *sg;
	काष्ठा cpumask *sg_span;

	sg = kzalloc_गाँठ(माप(काष्ठा sched_group) + cpumask_आकार(),
			GFP_KERNEL, cpu_to_गाँठ(cpu));

	अगर (!sg)
		वापस शून्य;

	sg_span = sched_group_span(sg);
	अगर (sd->child) {
		cpumask_copy(sg_span, sched_domain_span(sd->child));
		sg->flags = sd->child->flags;
	} अन्यथा {
		cpumask_copy(sg_span, sched_domain_span(sd));
	}

	atomic_inc(&sg->ref);
	वापस sg;
}

अटल व्योम init_overlap_sched_group(काष्ठा sched_domain *sd,
				     काष्ठा sched_group *sg)
{
	काष्ठा cpumask *mask = sched_domains_tmpmask2;
	काष्ठा sd_तथ्य *sdd = sd->निजी;
	काष्ठा cpumask *sg_span;
	पूर्णांक cpu;

	build_balance_mask(sd, sg, mask);
	cpu = cpumask_first(mask);

	sg->sgc = *per_cpu_ptr(sdd->sgc, cpu);
	अगर (atomic_inc_return(&sg->sgc->ref) == 1)
		cpumask_copy(group_balance_mask(sg), mask);
	अन्यथा
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

अटल काष्ठा sched_domain *
find_descended_sibling(काष्ठा sched_domain *sd, काष्ठा sched_domain *sibling)
{
	/*
	 * The proper descendant would be the one whose child won't span out
	 * of sd
	 */
	जबतक (sibling->child &&
	       !cpumask_subset(sched_domain_span(sibling->child),
			       sched_domain_span(sd)))
		sibling = sibling->child;

	/*
	 * As we are referencing sgc across different topology level, we need
	 * to go down to skip those sched_domains which don't contribute to
	 * scheduling because they will be degenerated in cpu_attach_domain
	 */
	जबतक (sibling->child &&
	       cpumask_equal(sched_domain_span(sibling->child),
			     sched_domain_span(sibling)))
		sibling = sibling->child;

	वापस sibling;
}

अटल पूर्णांक
build_overlap_sched_groups(काष्ठा sched_domain *sd, पूर्णांक cpu)
{
	काष्ठा sched_group *first = शून्य, *last = शून्य, *sg;
	स्थिर काष्ठा cpumask *span = sched_domain_span(sd);
	काष्ठा cpumask *covered = sched_domains_tmpmask;
	काष्ठा sd_तथ्य *sdd = sd->निजी;
	काष्ठा sched_domain *sibling;
	पूर्णांक i;

	cpumask_clear(covered);

	for_each_cpu_wrap(i, span, cpu) {
		काष्ठा cpumask *sg_span;

		अगर (cpumask_test_cpu(i, covered))
			जारी;

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
		अगर (!cpumask_test_cpu(i, sched_domain_span(sibling)))
			जारी;

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
		अगर (sibling->child &&
		    !cpumask_subset(sched_domain_span(sibling->child), span))
			sibling = find_descended_sibling(sd, sibling);

		sg = build_group_from_child_sched_domain(sibling, cpu);
		अगर (!sg)
			जाओ fail;

		sg_span = sched_group_span(sg);
		cpumask_or(covered, covered, sg_span);

		init_overlap_sched_group(sibling, sg);

		अगर (!first)
			first = sg;
		अगर (last)
			last->अग्र = sg;
		last = sg;
		last->अग्र = first;
	}
	sd->groups = first;

	वापस 0;

fail:
	free_sched_groups(first, 0);

	वापस -ENOMEM;
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

अटल काष्ठा sched_group *get_group(पूर्णांक cpu, काष्ठा sd_तथ्य *sdd)
{
	काष्ठा sched_domain *sd = *per_cpu_ptr(sdd->sd, cpu);
	काष्ठा sched_domain *child = sd->child;
	काष्ठा sched_group *sg;
	bool already_visited;

	अगर (child)
		cpu = cpumask_first(sched_domain_span(child));

	sg = *per_cpu_ptr(sdd->sg, cpu);
	sg->sgc = *per_cpu_ptr(sdd->sgc, cpu);

	/* Increase refcounts for claim_allocations: */
	already_visited = atomic_inc_return(&sg->ref) > 1;
	/* sgc visits should follow a similar trend as sg */
	WARN_ON(already_visited != (atomic_inc_return(&sg->sgc->ref) > 1));

	/* If we have already visited that group, it's already initialized. */
	अगर (already_visited)
		वापस sg;

	अगर (child) {
		cpumask_copy(sched_group_span(sg), sched_domain_span(child));
		cpumask_copy(group_balance_mask(sg), sched_group_span(sg));
		sg->flags = child->flags;
	} अन्यथा {
		cpumask_set_cpu(cpu, sched_group_span(sg));
		cpumask_set_cpu(cpu, group_balance_mask(sg));
	}

	sg->sgc->capacity = SCHED_CAPACITY_SCALE * cpumask_weight(sched_group_span(sg));
	sg->sgc->min_capacity = SCHED_CAPACITY_SCALE;
	sg->sgc->max_capacity = SCHED_CAPACITY_SCALE;

	वापस sg;
}

/*
 * build_sched_groups will build a circular linked list of the groups
 * covered by the given span, will set each group's ->cpumask correctly,
 * and will initialize their ->sgc.
 *
 * Assumes the sched_domain tree is fully constructed
 */
अटल पूर्णांक
build_sched_groups(काष्ठा sched_domain *sd, पूर्णांक cpu)
{
	काष्ठा sched_group *first = शून्य, *last = शून्य;
	काष्ठा sd_तथ्य *sdd = sd->निजी;
	स्थिर काष्ठा cpumask *span = sched_domain_span(sd);
	काष्ठा cpumask *covered;
	पूर्णांक i;

	lockdep_assert_held(&sched_domains_mutex);
	covered = sched_domains_tmpmask;

	cpumask_clear(covered);

	for_each_cpu_wrap(i, span, cpu) {
		काष्ठा sched_group *sg;

		अगर (cpumask_test_cpu(i, covered))
			जारी;

		sg = get_group(i, sdd);

		cpumask_or(covered, covered, sched_group_span(sg));

		अगर (!first)
			first = sg;
		अगर (last)
			last->अग्र = sg;
		last = sg;
	}
	last->अग्र = first;
	sd->groups = first;

	वापस 0;
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
अटल व्योम init_sched_groups_capacity(पूर्णांक cpu, काष्ठा sched_domain *sd)
{
	काष्ठा sched_group *sg = sd->groups;
	काष्ठा cpumask *mask = sched_domains_tmpmask2;

	WARN_ON(!sg);

	करो {
		पूर्णांक cpu, cores = 0, max_cpu = -1;

		sg->group_weight = cpumask_weight(sched_group_span(sg));

		cpumask_copy(mask, sched_group_span(sg));
		for_each_cpu(cpu, mask) {
			cores++;
			cpumask_andnot(mask, mask, cpu_smt_mask(cpu));
		}
		sg->cores = cores;

		अगर (!(sd->flags & SD_ASYM_PACKING))
			जाओ अग्र;

		for_each_cpu(cpu, sched_group_span(sg)) {
			अगर (max_cpu < 0)
				max_cpu = cpu;
			अन्यथा अगर (sched_asym_prefer(cpu, max_cpu))
				max_cpu = cpu;
		}
		sg->asym_prefer_cpu = max_cpu;

अग्र:
		sg = sg->अग्र;
	} जबतक (sg != sd->groups);

	अगर (cpu != group_balance_cpu(sg))
		वापस;

	update_group_capacity(sd, cpu);
}

/* Update the "asym_prefer_cpu" when arch_asym_cpu_priority() changes. */
व्योम sched_update_asym_prefer_cpu(पूर्णांक cpu, पूर्णांक old_prio, पूर्णांक new_prio)
{
	पूर्णांक asym_prefer_cpu = cpu;
	काष्ठा sched_domain *sd;

	guard(rcu)();

	for_each_domain(cpu, sd) {
		काष्ठा sched_group *sg;
		पूर्णांक group_cpu;

		अगर (!(sd->flags & SD_ASYM_PACKING))
			जारी;

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
		अगर (cpu != sg->asym_prefer_cpu) {
			/*
			 * Since the parent is a superset of the current group,
			 * if the cpu is not the "asym_prefer_cpu" at the
			 * current level, it cannot be the preferred CPU at a
			 * higher levels either.
			 */
			अगर (!sched_asym_prefer(cpu, sg->asym_prefer_cpu))
				वापस;

			WRITE_ONCE(sg->asym_prefer_cpu, cpu);
			जारी;
		}

		/* Ranking has improved; CPU is still the preferred one. */
		अगर (new_prio >= old_prio)
			जारी;

		for_each_cpu(group_cpu, sched_group_span(sg)) {
			अगर (sched_asym_prefer(group_cpu, asym_prefer_cpu))
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
LIST_शीर्ष(asym_cap_list);

/*
 * Verify whether there is any CPU capacity asymmetry in a given sched domain.
 * Provides sd_flags reflecting the asymmetry scope.
 */
अटल अंतरभूत पूर्णांक
asym_cpu_capacity_classify(स्थिर काष्ठा cpumask *sd_span,
			   स्थिर काष्ठा cpumask *cpu_map)
{
	काष्ठा asym_cap_तथ्य *entry;
	पूर्णांक गिनती = 0, miss = 0;

	/*
	 * Count how many unique CPU capacities this domain spans across
	 * (compare sched_domain CPUs mask with ones representing  available
	 * CPUs capacities). Take into account CPUs that might be offline:
	 * skip those.
	 */
	list_for_each_entry(entry, &asym_cap_list, link) {
		अगर (cpumask_intersects(sd_span, cpu_capacity_span(entry)))
			++गिनती;
		अन्यथा अगर (cpumask_intersects(cpu_map, cpu_capacity_span(entry)))
			++miss;
	}

	WARN_ON_ONCE(!गिनती && !list_empty(&asym_cap_list));

	/* No asymmetry detected */
	अगर (गिनती < 2)
		वापस 0;
	/* Some of the available CPU capacity values have not been detected */
	अगर (miss)
		वापस SD_ASYM_CPUCAPACITY;

	/* Full asymmetry */
	वापस SD_ASYM_CPUCAPACITY | SD_ASYM_CPUCAPACITY_FULL;

}

अटल व्योम free_asym_cap_entry(काष्ठा rcu_शीर्ष *शीर्ष)
{
	काष्ठा asym_cap_तथ्य *entry = container_of(शीर्ष, काष्ठा asym_cap_तथ्य, rcu);
	kfree(entry);
}

अटल अंतरभूत व्योम asym_cpu_capacity_update_तथ्य(पूर्णांक cpu)
{
	अचिन्हित दीर्घ capacity = arch_scale_cpu_capacity(cpu);
	काष्ठा asym_cap_तथ्य *जोड़ो_entry = शून्य;
	काष्ठा asym_cap_तथ्य *entry;

	/*
	 * Search if capacity already exits. If not, track which the entry
	 * where we should insert to keep the list ordered descending.
	 */
	list_for_each_entry(entry, &asym_cap_list, link) {
		अगर (capacity == entry->capacity)
			जाओ done;
		अन्यथा अगर (!जोड़ो_entry && capacity > entry->capacity)
			जोड़ो_entry = list_पूर्व_entry(entry, link);
	}

	entry = kzalloc(माप(*entry) + cpumask_आकार(), GFP_KERNEL);
	अगर (WARN_ONCE(!entry, "Failed to allocate memory for asymmetry data\n"))
		वापस;
	entry->capacity = capacity;

	/* If NULL then the new capacity is the smallest, add last. */
	अगर (!जोड़ो_entry)
		list_add_पुच्छ_rcu(&entry->link, &asym_cap_list);
	अन्यथा
		list_add_rcu(&entry->link, &जोड़ो_entry->link);
done:
	__cpumask_set_cpu(cpu, cpu_capacity_span(entry));
}

/*
 * Build-up/update list of CPUs grouped by their capacities
 * An update requires explicit request to rebuild sched domains
 * with state indicating CPU topology changes.
 */
अटल व्योम asym_cpu_capacity_scan(व्योम)
{
	काष्ठा asym_cap_तथ्य *entry, *अग्र;
	पूर्णांक cpu;

	list_for_each_entry(entry, &asym_cap_list, link)
		cpumask_clear(cpu_capacity_span(entry));

	for_each_cpu_and(cpu, cpu_possible_mask, housekeeping_cpumask(HK_TYPE_DOMAIN))
		asym_cpu_capacity_update_तथ्य(cpu);

	list_for_each_entry_safe(entry, अग्र, &asym_cap_list, link) {
		अगर (cpumask_empty(cpu_capacity_span(entry))) {
			list_del_rcu(&entry->link);
			call_rcu(&entry->rcu, free_asym_cap_entry);
		}
	}

	/*
	 * Only one capacity value has been detected i.e. this system is symmetric.
	 * No need to keep this data around.
	 */
	अगर (list_is_singular(&asym_cap_list)) {
		entry = list_first_entry(&asym_cap_list, typeof(*entry), link);
		list_del_rcu(&entry->link);
		call_rcu(&entry->rcu, free_asym_cap_entry);
	}
}

/*
 * Initializers for schedule domains
 * Non-inlined to reduce accumulated stack pressure in build_sched_domains()
 */

अटल पूर्णांक default_relax_domain_level = -1;
पूर्णांक sched_domain_level_max;

अटल पूर्णांक __init setup_relax_domain_level(अक्षर *str)
{
	अगर (kstrtoint(str, 0, &default_relax_domain_level))
		pr_warn("Unable to set relax_domain_level\n");

	वापस 1;
}
__setup("relax_domain_level=", setup_relax_domain_level);

अटल व्योम set_domain_attribute(काष्ठा sched_domain *sd,
				 काष्ठा sched_domain_attr *attr)
{
	पूर्णांक request;

	अगर (!attr || attr->relax_domain_level < 0) {
		अगर (default_relax_domain_level < 0)
			वापस;
		request = default_relax_domain_level;
	} अन्यथा
		request = attr->relax_domain_level;

	अगर (sd->level >= request) {
		/* Turn off idle balance on this domain: */
		sd->flags &= ~(SD_BALANCE_WAKE|SD_BALANCE_NEWIDLE);
	}
}

अटल व्योम __sdt_free(स्थिर काष्ठा cpumask *cpu_map);
अटल पूर्णांक __sdt_alloc(स्थिर काष्ठा cpumask *cpu_map);

अटल व्योम __sds_free(काष्ठा s_तथ्य *d, स्थिर काष्ठा cpumask *cpu_map);
अटल पूर्णांक __sds_alloc(काष्ठा s_तथ्य *d, स्थिर काष्ठा cpumask *cpu_map);

अटल व्योम __free_domain_allocs(काष्ठा s_तथ्य *d, क्रमागत s_alloc what,
				 स्थिर काष्ठा cpumask *cpu_map)
{
	चयन (what) {
	हाल sa_rootdomain:
		अगर (!atomic_read(&d->rd->refcount))
			free_rootdomain(&d->rd->rcu);
		fallthrough;
	हाल sa_sd:
		free_percpu(d->sd);
		fallthrough;
	हाल sa_sd_shared:
		__sds_free(d, cpu_map);
		fallthrough;
	हाल sa_sd_storage:
		__sdt_free(cpu_map);
		fallthrough;
	हाल sa_none:
		अवरोध;
	}
}

अटल क्रमागत s_alloc
__visit_domain_allocation_hell(काष्ठा s_तथ्य *d, स्थिर काष्ठा cpumask *cpu_map)
{
	स_रखो(d, 0, माप(*d));

	अगर (__sdt_alloc(cpu_map))
		वापस sa_sd_storage;
	अगर (__sds_alloc(d, cpu_map))
		वापस sa_sd_shared;
	d->sd = alloc_percpu(काष्ठा sched_domain *);
	अगर (!d->sd)
		वापस sa_sd_shared;
	d->rd = alloc_rootdomain();
	अगर (!d->rd)
		वापस sa_sd;

	वापस sa_rootdomain;
}

/*
 * NULL the sd_data elements we've used to build the sched_domain and
 * sched_group structure so that the subsequent __free_domain_allocs()
 * will not free the data we're using.
 */
अटल व्योम claim_allocations(पूर्णांक cpu, काष्ठा s_तथ्य *d)
{
	काष्ठा sched_domain *sd;

	अगर (atomic_read(&(*per_cpu_ptr(d->sds, cpu))->ref))
		*per_cpu_ptr(d->sds, cpu) = शून्य;

	क्रम (sd = *per_cpu_ptr(d->sd, cpu); sd; sd = sd->parent) {
		काष्ठा sd_तथ्य *sdd = sd->निजी;

		WARN_ON_ONCE(*per_cpu_ptr(sdd->sd, cpu) != sd);
		*per_cpu_ptr(sdd->sd, cpu) = शून्य;

		अगर (atomic_read(&(*per_cpu_ptr(sdd->sg, cpu))->ref))
			*per_cpu_ptr(sdd->sg, cpu) = शून्य;

		अगर (atomic_read(&(*per_cpu_ptr(sdd->sgc, cpu))->ref))
			*per_cpu_ptr(sdd->sgc, cpu) = शून्य;
	}
}

#अगर_घोषित CONFIG_NUMA
क्रमागत numa_topology_type sched_numa_topology_type;

/*
 * sched_domains_numa_distance is derived from sched_numa_node_distance
 * and provides a simplified view of NUMA distances used specifically
 * for building NUMA scheduling domains.
 */
अटल पूर्णांक			sched_domains_numa_levels;
अटल पूर्णांक			sched_numa_गाँठ_levels;

पूर्णांक				sched_max_numa_distance;
अटल पूर्णांक			*sched_domains_numa_distance;
अटल पूर्णांक			*sched_numa_गाँठ_distance;
अटल काष्ठा cpumask		***sched_domains_numa_masks;
#पूर्ण_अगर /* CONFIG_NUMA */

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
#घोषणा TOPOLOGY_SD_FLAGS		\
	(SD_SHARE_CPUCAPACITY	|	\
	 SD_CLUSTER		|	\
	 SD_SHARE_LLC		|	\
	 SD_NUMA		|	\
	 SD_ASYM_PACKING)

अटल काष्ठा sched_domain *
sd_init(काष्ठा sched_domain_topology_level *tl,
	स्थिर काष्ठा cpumask *cpu_map,
	काष्ठा sched_domain *child, पूर्णांक cpu)
{
	काष्ठा sd_तथ्य *sdd = &tl->तथ्य;
	काष्ठा sched_domain *sd = *per_cpu_ptr(sdd->sd, cpu);
	पूर्णांक sd_id, sd_weight, sd_flags = 0;
	काष्ठा cpumask *sd_span;
	u64 now = sched_clock();

	sd_span = sched_domain_span(sd);
	cpumask_and(sd_span, cpu_map, tl->mask(tl, cpu));
	sd_weight = cpumask_weight(sd_span);
	sd_id = cpumask_first(sd_span);

	अगर (tl->sd_flags)
		sd_flags = (*tl->sd_flags)();
	अगर (WARN_ONCE(sd_flags & ~TOPOLOGY_SD_FLAGS,
		      "wrong sd_flags in topology description\n"))
		sd_flags &= TOPOLOGY_SD_FLAGS;
#अगर_घोषित CONFIG_SCHED_SMT
	अगर (sched_smt_asym_packing && (sd_flags & SD_SHARE_CPUCAPACITY))
		sd_flags |= SD_ASYM_PACKING;
#पूर्ण_अगर
	sd_flags |= asym_cpu_capacity_classify(sd_span, cpu_map);

	*sd = (काष्ठा sched_domain){
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
	अगर (sd->flags & SD_SHARE_CPUCAPACITY) {
		sd->imbalance_pct = 110;

	} अन्यथा अगर (sd->flags & SD_SHARE_LLC) {
		sd->imbalance_pct = 117;
		sd->cache_nice_tries = 1;

#अगर_घोषित CONFIG_NUMA
	} अन्यथा अगर (sd->flags & SD_NUMA) {
		sd->cache_nice_tries = 2;

		sd->flags &= ~SD_PREFER_SIBLING;
		sd->flags |= SD_SERIALIZE;
		अगर (sched_domains_numa_distance[tl->numa_level] > गाँठ_reclaim_distance) {
			sd->flags &= ~(SD_BALANCE_EXEC |
				       SD_BALANCE_FORK |
				       SD_WAKE_AFFINE);
		}

#पूर्ण_अगर /* CONFIG_NUMA */
	} अन्यथा {
		sd->cache_nice_tries = 1;
	}

	sd->निजी = sdd;

	वापस sd;
}

#अगर_घोषित CONFIG_SCHED_SMT
पूर्णांक cpu_smt_flags(व्योम)
{
	वापस SD_SHARE_CPUCAPACITY | SD_SHARE_LLC;
}

स्थिर काष्ठा cpumask *tl_smt_mask(काष्ठा sched_domain_topology_level *tl, पूर्णांक cpu)
{
	वापस cpu_smt_mask(cpu);
}
#पूर्ण_अगर

#अगर_घोषित CONFIG_SCHED_CLUSTER
पूर्णांक cpu_cluster_flags(व्योम)
{
	वापस SD_CLUSTER | SD_SHARE_LLC;
}

स्थिर काष्ठा cpumask *tl_cls_mask(काष्ठा sched_domain_topology_level *tl, पूर्णांक cpu)
{
	वापस cpu_clustergroup_mask(cpu);
}
#पूर्ण_अगर

#अगर_घोषित CONFIG_SCHED_MC
पूर्णांक cpu_core_flags(व्योम)
{
	वापस SD_SHARE_LLC;
}

स्थिर काष्ठा cpumask *tl_mc_mask(काष्ठा sched_domain_topology_level *tl, पूर्णांक cpu)
{
	वापस cpu_coregroup_mask(cpu);
}

/*
 * Majority of architectures have LLC at MC domain level with exception
 * such as powerpc. Provide a way for arch to specify where its LLC is
 * if it falls in exception category
 */
# ifndef arch_llc_mask
#घोषणा arch_llc_mask(cpu) cpu_coregroup_mask(cpu)
# endif

#अन्यथा
#घोषणा arch_llc_mask(cpu) cpumask_of(cpu)
#पूर्ण_अगर

#घोषणा llc_mask(cpu) arch_llc_mask(cpu)

स्थिर काष्ठा cpumask *tl_pkg_mask(काष्ठा sched_domain_topology_level *tl, पूर्णांक cpu)
{
	वापस cpu_गाँठ_mask(cpu);
}

/*
 * Topology list, bottom-up.
 */
अटल काष्ठा sched_domain_topology_level default_topology[] = {
#अगर_घोषित CONFIG_SCHED_SMT
	SDTL_INIT(tl_smt_mask, cpu_smt_flags, SMT),
#पूर्ण_अगर

#अगर_घोषित CONFIG_SCHED_CLUSTER
	SDTL_INIT(tl_cls_mask, cpu_cluster_flags, CLS),
#पूर्ण_अगर

#अगर_घोषित CONFIG_SCHED_MC
	SDTL_INIT(tl_mc_mask, cpu_core_flags, MC),
#पूर्ण_अगर
	SDTL_INIT(tl_pkg_mask, शून्य, PKG),
	{ शून्य, },
};

अटल काष्ठा sched_domain_topology_level *sched_domain_topology =
	default_topology;
अटल काष्ठा sched_domain_topology_level *sched_domain_topology_saved;

#घोषणा for_each_sd_topology(tl)			\
	क्रम (tl = sched_domain_topology; tl->mask; tl++)

व्योम __init set_sched_topology(काष्ठा sched_domain_topology_level *tl)
{
	अगर (WARN_ON_ONCE(sched_smp_initialized))
		वापस;

	sched_domain_topology = tl;
	sched_domain_topology_saved = शून्य;
}

#अगर_घोषित CONFIG_NUMA
अटल पूर्णांक cpu_numa_flags(व्योम)
{
	वापस SD_NUMA;
}

अटल स्थिर काष्ठा cpumask *sd_numa_mask(काष्ठा sched_domain_topology_level *tl, पूर्णांक cpu)
{
	वापस sched_domains_numa_masks[tl->numa_level][cpu_to_गाँठ(cpu)];
}

अटल व्योम sched_numa_warn(स्थिर अक्षर *str)
{
	अटल पूर्णांक done = false;
	पूर्णांक i,j;

	अगर (done)
		वापस;

	done = true;

	printk(KERN_WARNING "ERROR: %s\n\n", str);

	क्रम (i = 0; i < nr_गाँठ_ids; i++) {
		printk(KERN_WARNING "  ");
		क्रम (j = 0; j < nr_गाँठ_ids; j++) {
			अगर (!गाँठ_state(i, N_CPU) || !गाँठ_state(j, N_CPU))
				printk(KERN_CONT "(%02d) ", गाँठ_distance(i,j));
			अन्यथा
				printk(KERN_CONT " %02d  ", गाँठ_distance(i,j));
		}
		printk(KERN_CONT "\n");
	}
	printk(KERN_WARNING "\n");
}

bool find_numa_distance(पूर्णांक distance)
{
	bool found = false;
	पूर्णांक i, *distances;

	अगर (distance == गाँठ_distance(0, 0))
		वापस true;

	rcu_read_lock();
	distances = rcu_dereference(sched_numa_गाँठ_distance);
	अगर (!distances)
		जाओ unlock;
	क्रम (i = 0; i < sched_numa_गाँठ_levels; i++) {
		अगर (distances[i] == distance) {
			found = true;
			अवरोध;
		}
	}
unlock:
	rcu_read_unlock();

	वापस found;
}

#घोषणा for_each_cpu_गाँठ_but(n, nbut)		\
	for_each_गाँठ_state(n, N_CPU)		\
		अगर (n == nbut)			\
			जारी;		\
		अन्यथा

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
अटल व्योम init_numa_topology_type(पूर्णांक offline_गाँठ)
{
	पूर्णांक a, b, c, n;

	n = sched_max_numa_distance;

	अगर (sched_domains_numa_levels <= 2) {
		sched_numa_topology_type = NUMA_DIRECT;
		वापस;
	}

	for_each_cpu_गाँठ_but(a, offline_गाँठ) {
		for_each_cpu_गाँठ_but(b, offline_गाँठ) {
			/* Find two nodes furthest removed from each other. */
			अगर (गाँठ_distance(a, b) < n)
				जारी;

			/* Is there an intermediary node between a and b? */
			for_each_cpu_गाँठ_but(c, offline_गाँठ) {
				अगर (गाँठ_distance(a, c) < n &&
				    गाँठ_distance(b, c) < n) {
					sched_numa_topology_type =
							NUMA_GLUELESS_MESH;
					वापस;
				}
			}

			sched_numa_topology_type = NUMA_BACKPLANE;
			वापस;
		}
	}

	pr_err("Failed to find a NUMA topology type, defaulting to DIRECT\n");
	sched_numa_topology_type = NUMA_DIRECT;
}


#घोषणा NR_DISTANCE_मूल्यावली (1 << DISTANCE_BITS)

/*
 * An architecture could modify its NUMA distance, to change
 * grouping of NUMA nodes and number of NUMA levels when creating
 * NUMA level sched domains.
 *
 * A NUMA level is created for each unique
 * arch_sched_node_distance.
 */
अटल पूर्णांक numa_गाँठ_dist(पूर्णांक i, पूर्णांक j)
{
	वापस गाँठ_distance(i, j);
}

पूर्णांक arch_sched_गाँठ_distance(पूर्णांक from, पूर्णांक to)
			     __weak __alias(numa_गाँठ_dist);

अटल bool modified_sched_गाँठ_distance(व्योम)
{
	वापस numa_गाँठ_dist != arch_sched_गाँठ_distance;
}

अटल पूर्णांक sched_record_numa_dist(पूर्णांक offline_गाँठ, पूर्णांक (*n_dist)(पूर्णांक, पूर्णांक),
				  पूर्णांक **dist, पूर्णांक *levels)
{
	अचिन्हित दीर्घ *distance_map __free(bitmap) = शून्य;
	पूर्णांक nr_levels = 0;
	पूर्णांक i, j;
	पूर्णांक *distances;

	/*
	 * O(nr_nodes^2) de-duplicating selection sort -- in order to find the
	 * unique distances in the node_distance() table.
	 */
	distance_map = bitmap_alloc(NR_DISTANCE_मूल्यावली, GFP_KERNEL);
	अगर (!distance_map)
		वापस -ENOMEM;

	bitmap_zero(distance_map, NR_DISTANCE_मूल्यावली);
	for_each_cpu_गाँठ_but(i, offline_गाँठ) {
		for_each_cpu_गाँठ_but(j, offline_गाँठ) {
			पूर्णांक distance = n_dist(i, j);

			अगर (distance < LOCAL_DISTANCE || distance >= NR_DISTANCE_मूल्यावली) {
				sched_numa_warn("Invalid distance value range");
				वापस -EINVAL;
			}

			bitmap_set(distance_map, distance, 1);
		}
	}
	/*
	 * We can now figure out how many unique distance values there are and
	 * allocate memory accordingly.
	 */
	nr_levels = bitmap_weight(distance_map, NR_DISTANCE_मूल्यावली);

	distances = kzalloc_objs(पूर्णांक, nr_levels);
	अगर (!distances)
		वापस -ENOMEM;

	क्रम (i = 0, j = 0; i < nr_levels; i++, j++) {
		j = find_अग्र_bit(distance_map, NR_DISTANCE_मूल्यावली, j);
		distances[i] = j;
	}
	*dist = distances;
	*levels = nr_levels;

	वापस 0;
}

व्योम sched_init_numa(पूर्णांक offline_गाँठ)
{
	काष्ठा sched_domain_topology_level *tl;
	पूर्णांक nr_levels, nr_गाँठ_levels;
	पूर्णांक i, j;
	पूर्णांक *distances, *domain_distances;
	काष्ठा cpumask ***masks;

	/* Record the NUMA distances from SLIT table */
	अगर (sched_record_numa_dist(offline_गाँठ, numa_गाँठ_dist, &distances,
				   &nr_गाँठ_levels))
		वापस;

	/* Record modified NUMA distances for building sched domains */
	अगर (modified_sched_गाँठ_distance()) {
		अगर (sched_record_numa_dist(offline_गाँठ, arch_sched_गाँठ_distance,
					   &domain_distances, &nr_levels)) {
			kfree(distances);
			वापस;
		}
	} अन्यथा {
		domain_distances = distances;
		nr_levels = nr_गाँठ_levels;
	}
	rcu_assign_pointer(sched_numa_गाँठ_distance, distances);
	WRITE_ONCE(sched_max_numa_distance, distances[nr_गाँठ_levels - 1]);
	WRITE_ONCE(sched_numa_गाँठ_levels, nr_गाँठ_levels);

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

	masks = kzalloc(माप(व्योम *) * nr_levels, GFP_KERNEL);
	अगर (!masks)
		वापस;

	/*
	 * Now for each level, construct a mask per node which contains all
	 * CPUs of nodes that are that many hops away from us.
	 */
	क्रम (i = 0; i < nr_levels; i++) {
		masks[i] = kzalloc(nr_गाँठ_ids * माप(व्योम *), GFP_KERNEL);
		अगर (!masks[i])
			वापस;

		for_each_cpu_गाँठ_but(j, offline_गाँठ) {
			काष्ठा cpumask *mask = kzalloc(cpumask_आकार(), GFP_KERNEL);
			पूर्णांक k;

			अगर (!mask)
				वापस;

			masks[i][j] = mask;

			for_each_cpu_गाँठ_but(k, offline_गाँठ) {
				अगर (sched_debug() &&
				    (arch_sched_गाँठ_distance(j, k) !=
				     arch_sched_गाँठ_distance(k, j)))
					sched_numa_warn("Node-distance not symmetric");

				अगर (arch_sched_गाँठ_distance(j, k) >
				    sched_domains_numa_distance[i])
					जारी;

				cpumask_or(mask, mask, cpumask_of_गाँठ(k));
			}
		}
	}
	rcu_assign_pointer(sched_domains_numa_masks, masks);

	/* Compute default topology size */
	क्रम (i = 0; sched_domain_topology[i].mask; i++);

	tl = kzalloc((i + nr_levels + 1) *
			माप(काष्ठा sched_domain_topology_level), GFP_KERNEL);
	अगर (!tl)
		वापस;

	/*
	 * Copy the default topology bits..
	 */
	क्रम (i = 0; sched_domain_topology[i].mask; i++)
		tl[i] = sched_domain_topology[i];

	/*
	 * Add the NUMA identity distance, aka single NODE.
	 */
	tl[i++] = SDTL_INIT(sd_numa_mask, शून्य, गाँठ_2);

	/*
	 * .. and append 'j' levels of NUMA goodness.
	 */
	क्रम (j = 1; j < nr_levels; i++, j++) {
		tl[i] = SDTL_INIT(sd_numa_mask, cpu_numa_flags, NUMA);
		tl[i].numa_level = j;
	}

	sched_domain_topology_saved = sched_domain_topology;
	sched_domain_topology = tl;

	sched_domains_numa_levels = nr_levels;

	init_numa_topology_type(offline_गाँठ);
}


अटल व्योम sched_reset_numa(व्योम)
{
	पूर्णांक nr_levels, *distances, *dom_distances = शून्य;
	काष्ठा cpumask ***masks;

	nr_levels = sched_domains_numa_levels;
	sched_numa_गाँठ_levels = 0;
	sched_domains_numa_levels = 0;
	sched_max_numa_distance = 0;
	sched_numa_topology_type = NUMA_DIRECT;
	distances = sched_numa_गाँठ_distance;
	अगर (sched_numa_गाँठ_distance != sched_domains_numa_distance)
		dom_distances = sched_domains_numa_distance;
	rcu_assign_pointer(sched_numa_गाँठ_distance, शून्य);
	rcu_assign_pointer(sched_domains_numa_distance, शून्य);
	masks = sched_domains_numa_masks;
	rcu_assign_pointer(sched_domains_numa_masks, शून्य);
	अगर (distances || masks) {
		पूर्णांक i, j;

		synchronize_rcu();
		kfree(distances);
		kfree(dom_distances);
		क्रम (i = 0; i < nr_levels && masks; i++) {
			अगर (!masks[i])
				जारी;
			for_each_गाँठ(j)
				kfree(masks[i][j]);
			kfree(masks[i]);
		}
		kfree(masks);
	}
	अगर (sched_domain_topology_saved) {
		kfree(sched_domain_topology);
		sched_domain_topology = sched_domain_topology_saved;
		sched_domain_topology_saved = शून्य;
	}
}

/*
 * Call with hotplug lock held
 */
व्योम sched_update_numa(पूर्णांक cpu, bool online)
{
	पूर्णांक गाँठ;

	गाँठ = cpu_to_गाँठ(cpu);
	/*
	 * Scheduler NUMA topology is updated when the first CPU of a
	 * node is onlined or the last CPU of a node is offlined.
	 */
	अगर (cpumask_weight(cpumask_of_गाँठ(गाँठ)) != 1)
		वापस;

	sched_reset_numa();
	sched_init_numa(online ? NUMA_NO_गाँठ : गाँठ);
}

व्योम sched_domains_numa_masks_set(अचिन्हित पूर्णांक cpu)
{
	पूर्णांक गाँठ = cpu_to_गाँठ(cpu);
	पूर्णांक i, j;

	क्रम (i = 0; i < sched_domains_numa_levels; i++) {
		क्रम (j = 0; j < nr_गाँठ_ids; j++) {
			अगर (!गाँठ_state(j, N_CPU))
				जारी;

			/* Set ourselves in the remote node's masks */
			अगर (arch_sched_गाँठ_distance(j, गाँठ) <=
			    sched_domains_numa_distance[i])
				cpumask_set_cpu(cpu, sched_domains_numa_masks[i][j]);
		}
	}
}

व्योम sched_domains_numa_masks_clear(अचिन्हित पूर्णांक cpu)
{
	पूर्णांक i, j;

	क्रम (i = 0; i < sched_domains_numa_levels; i++) {
		क्रम (j = 0; j < nr_गाँठ_ids; j++) {
			अगर (sched_domains_numa_masks[i][j])
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
पूर्णांक sched_numa_find_closest(स्थिर काष्ठा cpumask *cpus, पूर्णांक cpu)
{
	पूर्णांक i, j = cpu_to_गाँठ(cpu), found = nr_cpu_ids;
	काष्ठा cpumask ***masks;

	rcu_read_lock();
	masks = rcu_dereference(sched_domains_numa_masks);
	अगर (!masks)
		जाओ unlock;
	क्रम (i = 0; i < sched_domains_numa_levels; i++) {
		अगर (!masks[i][j])
			अवरोध;
		cpu = cpumask_any_and_distribute(cpus, masks[i][j]);
		अगर (cpu < nr_cpu_ids) {
			found = cpu;
			अवरोध;
		}
	}
unlock:
	rcu_read_unlock();

	वापस found;
}

काष्ठा __cmp_key {
	स्थिर काष्ठा cpumask *cpus;
	काष्ठा cpumask ***masks;
	पूर्णांक गाँठ;
	पूर्णांक cpu;
	पूर्णांक w;
};

अटल पूर्णांक hop_cmp(स्थिर व्योम *a, स्थिर व्योम *b)
{
	काष्ठा cpumask **पूर्व_hop, **cur_hop = *(काष्ठा cpumask ***)b;
	काष्ठा __cmp_key *k = (काष्ठा __cmp_key *)a;

	अगर (cpumask_weight_and(k->cpus, cur_hop[k->गाँठ]) <= k->cpu)
		वापस 1;

	अगर (b == k->masks) {
		k->w = 0;
		वापस 0;
	}

	पूर्व_hop = *((काष्ठा cpumask ***)b - 1);
	k->w = cpumask_weight_and(k->cpus, पूर्व_hop[k->गाँठ]);
	अगर (k->w <= k->cpu)
		वापस 0;

	वापस -1;
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
पूर्णांक sched_numa_find_nth_cpu(स्थिर काष्ठा cpumask *cpus, पूर्णांक cpu, पूर्णांक गाँठ)
{
	काष्ठा __cmp_key k = { .cpus = cpus, .cpu = cpu };
	काष्ठा cpumask ***hop_masks;
	पूर्णांक hop, ret = nr_cpu_ids;

	अगर (गाँठ == NUMA_NO_गाँठ)
		वापस cpumask_nth_and(cpu, cpus, cpu_online_mask);

	rcu_read_lock();

	/* CPU-less node entries are uninitialized in sched_domains_numa_masks */
	गाँठ = numa_nearest_गाँठ(गाँठ, N_CPU);
	k.गाँठ = गाँठ;

	k.masks = rcu_dereference(sched_domains_numa_masks);
	अगर (!k.masks)
		जाओ unlock;

	hop_masks = द्वा_खोज(&k, k.masks, sched_domains_numa_levels, माप(k.masks[0]), hop_cmp);
	अगर (!hop_masks)
		जाओ unlock;
	hop = hop_masks	- k.masks;

	ret = hop ?
		cpumask_nth_and_andnot(cpu - k.w, cpus, k.masks[hop][गाँठ], k.masks[hop-1][गाँठ]) :
		cpumask_nth_and(cpu, cpus, k.masks[0][गाँठ]);
unlock:
	rcu_read_unlock();
	वापस ret;
}
EXPORT_प्रतीक_GPL(sched_numa_find_nth_cpu);

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
स्थिर काष्ठा cpumask *sched_numa_hop_mask(अचिन्हित पूर्णांक गाँठ, अचिन्हित पूर्णांक hops)
{
	काष्ठा cpumask ***masks;

	अगर (गाँठ >= nr_गाँठ_ids || hops >= sched_domains_numa_levels)
		वापस ERR_PTR(-EINVAL);

	masks = rcu_dereference(sched_domains_numa_masks);
	अगर (!masks)
		वापस ERR_PTR(-EBUSY);

	वापस masks[hops][गाँठ];
}
EXPORT_प्रतीक_GPL(sched_numa_hop_mask);

#पूर्ण_अगर /* CONFIG_NUMA */

अटल पूर्णांक __sdt_alloc(स्थिर काष्ठा cpumask *cpu_map)
{
	काष्ठा sched_domain_topology_level *tl;
	पूर्णांक j;

	for_each_sd_topology(tl) {
		काष्ठा sd_तथ्य *sdd = &tl->तथ्य;

		sdd->sd = alloc_percpu(काष्ठा sched_domain *);
		अगर (!sdd->sd)
			वापस -ENOMEM;

		sdd->sg = alloc_percpu(काष्ठा sched_group *);
		अगर (!sdd->sg)
			वापस -ENOMEM;

		sdd->sgc = alloc_percpu(काष्ठा sched_group_capacity *);
		अगर (!sdd->sgc)
			वापस -ENOMEM;

		for_each_cpu(j, cpu_map) {
			काष्ठा sched_domain *sd;
			काष्ठा sched_group *sg;
			काष्ठा sched_group_capacity *sgc;

			sd = kzalloc_गाँठ(माप(काष्ठा sched_domain) + cpumask_आकार(),
					GFP_KERNEL, cpu_to_गाँठ(j));
			अगर (!sd)
				वापस -ENOMEM;

			*per_cpu_ptr(sdd->sd, j) = sd;

			sg = kzalloc_गाँठ(माप(काष्ठा sched_group) + cpumask_आकार(),
					GFP_KERNEL, cpu_to_गाँठ(j));
			अगर (!sg)
				वापस -ENOMEM;

			sg->अग्र = sg;

			*per_cpu_ptr(sdd->sg, j) = sg;

			sgc = kzalloc_गाँठ(माप(काष्ठा sched_group_capacity) + cpumask_आकार(),
					GFP_KERNEL, cpu_to_गाँठ(j));
			अगर (!sgc)
				वापस -ENOMEM;

			sgc->id = j;

			*per_cpu_ptr(sdd->sgc, j) = sgc;
		}
	}

	वापस 0;
}

अटल व्योम __sdt_free(स्थिर काष्ठा cpumask *cpu_map)
{
	काष्ठा sched_domain_topology_level *tl;
	पूर्णांक j;

	for_each_sd_topology(tl) {
		काष्ठा sd_तथ्य *sdd = &tl->तथ्य;

		for_each_cpu(j, cpu_map) {
			काष्ठा sched_domain *sd;

			अगर (sdd->sd) {
				sd = *per_cpu_ptr(sdd->sd, j);
				अगर (sd && (sd->flags & SD_NUMA))
					free_sched_groups(sd->groups, 0);
				kfree(*per_cpu_ptr(sdd->sd, j));
			}

			अगर (sdd->sg)
				kfree(*per_cpu_ptr(sdd->sg, j));
			अगर (sdd->sgc)
				kfree(*per_cpu_ptr(sdd->sgc, j));
		}
		free_percpu(sdd->sd);
		sdd->sd = शून्य;
		free_percpu(sdd->sg);
		sdd->sg = शून्य;
		free_percpu(sdd->sgc);
		sdd->sgc = शून्य;
	}
}

अटल पूर्णांक __sds_alloc(काष्ठा s_तथ्य *d, स्थिर काष्ठा cpumask *cpu_map)
{
	पूर्णांक j;

	d->sds = alloc_percpu(काष्ठा sched_domain_shared *);
	अगर (!d->sds)
		वापस -ENOMEM;

	for_each_cpu(j, cpu_map) {
		काष्ठा sched_domain_shared *sds;

		sds = kzalloc_गाँठ(माप(काष्ठा sched_domain_shared),
				GFP_KERNEL, cpu_to_गाँठ(j));
		अगर (!sds)
			वापस -ENOMEM;

		*per_cpu_ptr(d->sds, j) = sds;
	}

	वापस 0;
}

अटल व्योम __sds_free(काष्ठा s_तथ्य *d, स्थिर काष्ठा cpumask *cpu_map)
{
	पूर्णांक j;

	अगर (!d->sds)
		वापस;

	for_each_cpu(j, cpu_map)
		kfree(*per_cpu_ptr(d->sds, j));

	free_percpu(d->sds);
	d->sds = शून्य;
}

अटल काष्ठा sched_domain *build_sched_domain(काष्ठा sched_domain_topology_level *tl,
		स्थिर काष्ठा cpumask *cpu_map, काष्ठा sched_domain_attr *attr,
		काष्ठा sched_domain *child, पूर्णांक cpu)
{
	काष्ठा sched_domain *sd = sd_init(tl, cpu_map, child, cpu);

	अगर (child) {
		sd->level = child->level + 1;
		sched_domain_level_max = max(sched_domain_level_max, sd->level);
		child->parent = sd;

		अगर (!cpumask_subset(sched_domain_span(child),
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

	वापस sd;
}

/*
 * Ensure topology masks are sane, i.e. there are no conflicts (overlaps) for
 * any two given CPUs on non-NUMA topology levels.
 */
अटल bool topology_span_sane(स्थिर काष्ठा cpumask *cpu_map)
{
	काष्ठा sched_domain_topology_level *tl;
	काष्ठा cpumask *covered, *id_seen;
	पूर्णांक cpu;

	lockdep_assert_held(&sched_domains_mutex);
	covered = sched_domains_tmpmask;
	id_seen = sched_domains_tmpmask2;

	for_each_sd_topology(tl) {
		पूर्णांक tl_common_flags = 0;

		अगर (tl->sd_flags)
			tl_common_flags = (*tl->sd_flags)();

		/* NUMA levels are allowed to overlap */
		अगर (tl_common_flags & SD_NUMA)
			जारी;

		cpumask_clear(covered);
		cpumask_clear(id_seen);

		/*
		 * Non-NUMA levels cannot partially overlap - they must be either
		 * completely equal or completely disjoint. Otherwise we can end up
		 * breaking the sched_group lists - i.e. a later get_group() pass
		 * breaks the linking done for an earlier span.
		 */
		for_each_cpu(cpu, cpu_map) {
			स्थिर काष्ठा cpumask *tl_cpu_mask = tl->mask(tl, cpu);
			पूर्णांक id;

			/* lowest bit set in this mask is used as a unique id */
			id = cpumask_first(tl_cpu_mask);

			अगर (cpumask_test_cpu(id, id_seen)) {
				/* First CPU has already been seen, ensure identical spans */
				अगर (!cpumask_equal(tl->mask(tl, id), tl_cpu_mask))
					वापस false;
			} अन्यथा {
				/* First CPU hasn't been seen before, ensure it's a completely new span */
				अगर (cpumask_intersects(tl_cpu_mask, covered))
					वापस false;

				cpumask_or(covered, covered, tl_cpu_mask);
				cpumask_set_cpu(id, id_seen);
			}
		}
	}
	वापस true;
}

/*
 * Calculate an allowed NUMA imbalance such that LLCs do not get
 * imbalanced.
 */
अटल व्योम adjust_numa_imbalance(काष्ठा sched_domain *sd_llc)
{
	काष्ठा sched_domain *parent;
	अचिन्हित पूर्णांक imb_span = 1;
	अचिन्हित पूर्णांक imb = 0;
	अचिन्हित पूर्णांक nr_llcs;

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
	अगर (nr_llcs == 1)
		imb = sd_llc->parent->span_weight >> 3;
	अन्यथा
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
	जबतक (parent && !(parent->flags & SD_NUMA))
		parent = parent->parent;

	imb_span = parent ? parent->span_weight : sd_llc->parent->span_weight;

	/* Update the upper remainder of the topology */
	parent = sd_llc->parent;
	जबतक (parent) {
		पूर्णांक factor = max(1U, (parent->span_weight / imb_span));

		parent->imb_numa_nr = imb * factor;
		parent = parent->parent;
	}
}

अटल व्योम
init_sched_domain_shared(काष्ठा s_तथ्य *d, काष्ठा sched_domain *sd, पूर्णांक flags)
{
	काष्ठा sched_domain_shared *sds = शून्य;
	पूर्णांक cpu;

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

		अगर (!sds->alloc_flags ||
		    sd->span_weight == 1 ||
		    sds->alloc_flags == flags) {
			sds->alloc_flags = flags;
			sd->shared = sds;
			अवरोध;
		}
	}

	/*
	 * Use the sd_shared corresponding to the last
	 * CPU in the span if none are avaialable.
	 */
	अगर (WARN_ON_ONCE(!sd->shared))
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
अटल bool claim_asym_sched_domain_shared(काष्ठा s_तथ्य *d, पूर्णांक cpu)
{
	काष्ठा sched_domain *sd = *per_cpu_ptr(d->sd, cpu);
	काष्ठा sched_domain *sd_asym;

	अगर (!sd)
		वापस false;

	sd_asym = sd;
	जबतक (sd_asym && !(sd_asym->flags & SD_ASYM_CPUCAPACITY_FULL))
		sd_asym = sd_asym->parent;

	अगर (!sd_asym || (sd_asym->flags & SD_NUMA))
		वापस false;

	init_sched_domain_shared(d, sd_asym, SD_ASYM_CPUCAPACITY);
	वापस true;
}

अटल पूर्णांक __sched_domains_alloc_llc_id(व्योम)
{
	पूर्णांक lid, max;

	lockdep_assert_held(&sched_domains_mutex);

	lid = cpumask_first_zero(sched_domains_llc_id_allocmask);
	/*
	 * llc_id space should never grow larger than the
	 * possible number of CPUs in the system.
	 */
	अगर (lid >= nr_cpu_ids)
		वापस -1;

	__cpumask_set_cpu(lid, sched_domains_llc_id_allocmask);
	max = cpumask_last(sched_domains_llc_id_allocmask);
	अगर (max > max_lid)
		max_lid = max;

	वापस lid;
}

अटल व्योम __sched_domains_free_llc_id(पूर्णांक cpu)
{
	पूर्णांक i, lid, max;

	lockdep_assert_held(&sched_domains_mutex);

	lid = per_cpu(sd_llc_id, cpu);
	अगर (lid == -1 || lid >= nr_cpu_ids)
		वापस;

	per_cpu(sd_llc_id, cpu) = -1;

	for_each_cpu(i, llc_mask(cpu)) {
		/* An online CPU owns the llc_id. */
		अगर (per_cpu(sd_llc_id, i) == lid)
			वापस;
	}

	__cpumask_clear_cpu(lid, sched_domains_llc_id_allocmask);

	max = cpumask_last(sched_domains_llc_id_allocmask);
	/* shrink max lid to save memory */
	अगर (max < max_lid)
		max_lid = max;
}

व्योम sched_domains_free_llc_id(पूर्णांक cpu)
{
	sched_domains_mutex_lock();
	__sched_domains_free_llc_id(cpu);
	sched_domains_mutex_unlock();
}

/*
 * Build sched domains for a given set of CPUs and attach the sched domains
 * to the individual CPUs
 */
अटल पूर्णांक
build_sched_domains(स्थिर काष्ठा cpumask *cpu_map, काष्ठा sched_domain_attr *attr,
		    bool *multi_llcs)
{
	क्रमागत s_alloc alloc_state = sa_none;
	bool has_multi_llcs = false;
	काष्ठा sched_domain *sd;
	काष्ठा s_तथ्य d;
	काष्ठा rq *rq = शून्य;
	पूर्णांक i, ret = -ENOMEM;
	bool has_asym = false;
	bool has_cluster = false;

	अगर (WARN_ON(cpumask_empty(cpu_map)))
		जाओ error;

	alloc_state = __visit_domain_allocation_hell(&d, cpu_map);
	अगर (alloc_state != sa_rootdomain)
		जाओ error;

	/* Set up domains for CPUs specified by the cpu_map: */
	for_each_cpu(i, cpu_map) {
		काष्ठा sched_domain_topology_level *tl;
		पूर्णांक lid;

		sd = शून्य;
		for_each_sd_topology(tl) {

			sd = build_sched_domain(tl, cpu_map, attr, sd, i);

			has_asym |= sd->flags & SD_ASYM_CPUCAPACITY;

			अगर (tl == sched_domain_topology)
				*per_cpu_ptr(d.sd, i) = sd;
			अगर (cpumask_equal(cpu_map, sched_domain_span(sd)))
				अवरोध;
		}

		lid = per_cpu(sd_llc_id, i);
		अगर (lid == -1) {
			/* try to reuse the llc_id of its siblings */
			क्रम (पूर्णांक j = cpumask_first(llc_mask(i));
			     j < nr_cpu_ids;
			     j = cpumask_अग्र(j, llc_mask(i))) {
				अगर (i == j)
					जारी;

				lid = per_cpu(sd_llc_id, j);

				अगर (lid != -1) {
					per_cpu(sd_llc_id, i) = lid;

					अवरोध;
				}
			}

			/* a new LLC is detected */
			अगर (lid == -1)
				per_cpu(sd_llc_id, i) = __sched_domains_alloc_llc_id();
		}
	}

	अगर (WARN_ON(!topology_span_sane(cpu_map)))
		जाओ error;

	/* Build the groups for the domains */
	for_each_cpu(i, cpu_map) {
		क्रम (sd = *per_cpu_ptr(d.sd, i); sd; sd = sd->parent) {
			sd->span_weight = cpumask_weight(sched_domain_span(sd));
			अगर (sd->flags & SD_NUMA) {
				अगर (build_overlap_sched_groups(sd, i))
					जाओ error;
			} अन्यथा {
				अगर (build_sched_groups(sd, i))
					जाओ error;
			}
		}
	}

	for_each_cpu(i, cpu_map) {
		sd = *per_cpu_ptr(d.sd, i);
		अगर (!sd)
			जारी;

		अगर (has_asym)
			claim_asym_sched_domain_shared(&d, i);

		/* First, find the topmost SD_SHARE_LLC domain */
		जबतक (sd->parent && (sd->parent->flags & SD_SHARE_LLC))
			sd = sd->parent;

		अगर (sd->flags & SD_SHARE_LLC) {
			init_sched_domain_shared(&d, sd, SD_SHARE_LLC);

			/*
			 * In presence of higher domains, adjust the
			 * NUMA imbalance stats for the hierarchy.
			 */
			अगर (sd->parent) {
				अगर (IS_ENABLED(CONFIG_NUMA))
					adjust_numa_imbalance(sd);

				अगर (sd_in_multi_llcs(sd))
					has_multi_llcs = true;
			}
		}
	}

	/* Calculate CPU capacity for physical packages and nodes */
	क्रम (i = nr_cpumask_bits-1; i >= 0; i--) {
		अगर (!cpumask_test_cpu(i, cpu_map))
			जारी;

		claim_allocations(i, &d);

		क्रम (sd = *per_cpu_ptr(d.sd, i); sd; sd = sd->parent)
			init_sched_groups_capacity(i, sd);
	}

	alloc_sd_llc(cpu_map, &d);

	/* Attach the domains */
	rcu_read_lock();
	for_each_cpu(i, cpu_map) {
		rq = cpu_rq(i);
		sd = *per_cpu_ptr(d.sd, i);

		cpu_attach_domain(sd, d.rd, i);

		अगर (lowest_flag_domain(i, SD_CLUSTER))
			has_cluster = true;
	}
	rcu_read_unlock();

	अगर (has_asym)
		static_branch_inc_cpuslocked(&sched_asym_cpucapacity);

	अगर (has_cluster)
		static_branch_inc_cpuslocked(&sched_cluster_active);

	अगर (rq && sched_debug_verbose)
		pr_info("root domain span: %*pbl\n", cpumask_pr_args(cpu_map));

	ret = 0;
error:
	*multi_llcs = has_multi_llcs;
	__free_domain_allocs(&d, alloc_state, cpu_map);

	वापस ret;
}

/* Current sched domains: */
अटल cpumask_var_t			*doms_cur;

/* Number of sched domains in 'doms_cur': */
अटल पूर्णांक				ndoms_cur;

/* Attributes of custom domains in 'doms_cur' */
अटल काष्ठा sched_domain_attr		*dattr_cur;

/*
 * Special case: If a kmalloc() of a doms_cur partition (array of
 * cpumask) fails, then fallback to a single sched domain,
 * as determined by the single cpumask fallback_doms.
 */
अटल cpumask_var_t			fallback_doms;

/*
 * arch_update_cpu_topology lets virtualized architectures update the
 * CPU core maps. It is supposed to return 1 if the topology changed
 * or 0 if it stayed the same.
 */
पूर्णांक __weak arch_update_cpu_topology(व्योम)
{
	वापस 0;
}

cpumask_var_t *alloc_sched_domains(अचिन्हित पूर्णांक ndoms)
{
	पूर्णांक i;
	cpumask_var_t *doms;

	doms = kmalloc_objs(*doms, ndoms);
	अगर (!doms)
		वापस शून्य;
	क्रम (i = 0; i < ndoms; i++) {
		अगर (!alloc_cpumask_var(&doms[i], GFP_KERNEL)) {
			free_sched_domains(doms, i);
			वापस शून्य;
		}
	}
	वापस doms;
}

व्योम free_sched_domains(cpumask_var_t doms[], अचिन्हित पूर्णांक ndoms)
{
	अचिन्हित पूर्णांक i;
	क्रम (i = 0; i < ndoms; i++)
		free_cpumask_var(doms[i]);
	kfree(doms);
}

/*
 * Set up scheduler domains and groups.  For now this just excludes isolated
 * CPUs, but could be used to exclude other special cases in the future.
 */
पूर्णांक __init sched_init_domains(स्थिर काष्ठा cpumask *cpu_map)
{
	bool multi_llcs;
	पूर्णांक err;

	zalloc_cpumask_var(&sched_domains_llc_id_allocmask, GFP_KERNEL);
	zalloc_cpumask_var(&sched_domains_tmpmask, GFP_KERNEL);
	zalloc_cpumask_var(&sched_domains_tmpmask2, GFP_KERNEL);
	zalloc_cpumask_var(&fallback_doms, GFP_KERNEL);

	arch_update_cpu_topology();
	asym_cpu_capacity_scan();
	ndoms_cur = 1;
	doms_cur = alloc_sched_domains(ndoms_cur);
	अगर (!doms_cur)
		doms_cur = &fallback_doms;
	cpumask_and(doms_cur[0], cpu_map, housekeeping_cpumask(HK_TYPE_DOMAIN));
	err = build_sched_domains(doms_cur[0], शून्य, &multi_llcs);
	अगर (!err)
		sched_cache_set(multi_llcs);

	वापस err;
}

/*
 * Detach sched domains from a group of CPUs specified in cpu_map
 * These CPUs will now be attached to the NULL domain
 */
अटल व्योम detach_destroy_domains(स्थिर काष्ठा cpumask *cpu_map)
{
	अचिन्हित पूर्णांक cpu = cpumask_any(cpu_map);
	पूर्णांक i;

	अगर (rcu_access_pointer(per_cpu(sd_asym_cpucapacity, cpu)))
		static_branch_dec_cpuslocked(&sched_asym_cpucapacity);

	अगर (static_branch_unlikely(&sched_cluster_active))
		static_branch_dec_cpuslocked(&sched_cluster_active);

	rcu_read_lock();
	for_each_cpu(i, cpu_map)
		cpu_attach_domain(शून्य, &def_मूल_domain, i);
	rcu_read_unlock();
}

/* handle null as "default" */
अटल पूर्णांक dattrs_equal(काष्ठा sched_domain_attr *cur, पूर्णांक idx_cur,
			काष्ठा sched_domain_attr *new, पूर्णांक idx_new)
{
	काष्ठा sched_domain_attr tmp;

	/* Fast path: */
	अगर (!new && !cur)
		वापस 1;

	tmp = SD_ATTR_INIT;

	वापस !स_भेद(cur ? (cur + idx_cur) : &tmp,
			new ? (new + idx_new) : &tmp,
			माप(काष्ठा sched_domain_attr));
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
अटल व्योम partition_sched_domains_locked(पूर्णांक ndoms_new, cpumask_var_t doms_new[],
				    काष्ठा sched_domain_attr *dattr_new)
{
	bool __maybe_unused has_eas = false;
	bool has_multi_llcs = false, multi_llcs;
	पूर्णांक i, j, n;
	पूर्णांक new_topology;

	lockdep_assert_held(&sched_domains_mutex);

	/* Let the architecture update CPU core mappings: */
	new_topology = arch_update_cpu_topology();
	/* Trigger rebuilding CPU capacity asymmetry data */
	अगर (new_topology)
		asym_cpu_capacity_scan();

	अगर (!doms_new) {
		WARN_ON_ONCE(dattr_new);
		n = 0;
		doms_new = alloc_sched_domains(1);
		अगर (doms_new) {
			n = 1;
			cpumask_and(doms_new[0], cpu_active_mask,
				    housekeeping_cpumask(HK_TYPE_DOMAIN));
		}
	} अन्यथा {
		n = ndoms_new;
	}

	/* Destroy deleted domains: */
	क्रम (i = 0; i < ndoms_cur; i++) {
		क्रम (j = 0; j < n && !new_topology; j++) {
			अगर (cpumask_equal(doms_cur[i], doms_new[j]) &&
			    dattrs_equal(dattr_cur, i, dattr_new, j))
				जाओ match1;
		}
		/* No match - a current sched domain not in new doms_new[] */
		detach_destroy_domains(doms_cur[i]);
match1:
		;
	}

	n = ndoms_cur;
	अगर (!doms_new) {
		n = 0;
		doms_new = &fallback_doms;
		cpumask_and(doms_new[0], cpu_active_mask,
			    housekeeping_cpumask(HK_TYPE_DOMAIN));
	}

	/* Build new domains: */
	क्रम (i = 0; i < ndoms_new; i++) {
		क्रम (j = 0; j < n && !new_topology; j++) {
			अगर (cpumask_equal(doms_new[i], doms_cur[j]) &&
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
				काष्ठा sched_domain *sd;
				पूर्णांक cpu = cpumask_first(doms_cur[j]);

				guard(rcu)();
				sd = rcu_dereference(cpu_rq(cpu)->sd);
				जबतक (sd && sd->parent && (sd->parent->flags & SD_SHARE_LLC))
					sd = sd->parent;
				अगर (sd && (sd->flags & SD_SHARE_LLC) && sd->parent &&
				    sd_in_multi_llcs(sd))
					has_multi_llcs = true;
				जाओ match2;
			}
		}
		/* No match - add a new doms_new */
		build_sched_domains(doms_new[i], dattr_new ? dattr_new + i : शून्य,
				    &multi_llcs);
		has_multi_llcs |= multi_llcs;
match2:
		;
	}
	sched_cache_set(has_multi_llcs);

#अगर defined(CONFIG_ENERGY_MODEL) && defined(CONFIG_CPU_FREQ_GOV_SCHEDUTIL)
	/* Build perf domains: */
	क्रम (i = 0; i < ndoms_new; i++) {
		क्रम (j = 0; j < n && !sched_energy_update; j++) {
			अगर (cpumask_equal(doms_new[i], doms_cur[j]) &&
			    cpu_rq(cpumask_first(doms_cur[j]))->rd->pd) {
				has_eas = true;
				जाओ match3;
			}
		}
		/* No match - add perf domains for a new rd */
		has_eas |= build_perf_domains(doms_new[i]);
match3:
		;
	}
	sched_energy_set(has_eas);
#पूर्ण_अगर

	/* Remember the new sched domains: */
	अगर (doms_cur != &fallback_doms)
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
व्योम partition_sched_domains(पूर्णांक ndoms_new, cpumask_var_t doms_new[],
			     काष्ठा sched_domain_attr *dattr_new)
{
	sched_domains_mutex_lock();
	partition_sched_domains_locked(ndoms_new, doms_new, dattr_new);
	sched_domains_mutex_unlock();
}
