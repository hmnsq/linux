// SPDX-License-Identifier: GPL-2.0
/*
 * Scheduler topology setup/handling methods
 */

#ଅନ୍ତର୍ଭାଵ <linux/sched/isolation.h>
#ଅନ୍ତର୍ଭାଵ <linux/sched/clock.h>
#ଅନ୍ତର୍ଭାଵ <linux/bsearch.h>
#ଅନ୍ତର୍ଭାଵ "sched.h"

DEFINE_MUTEX(sched_domains_mutex);
ଶୂନ୍ୟ sched_domains_mutex_lock(ଶୂନ୍ୟ)
{
	mutex_lock(&sched_domains_mutex);
}
ଶୂନ୍ୟ sched_domains_mutex_unlock(ଶୂନ୍ୟ)
{
	mutex_unlock(&sched_domains_mutex);
}

/* Protected by sched_domains_mutex: */
ସ୍ଥିର cpumask_var_t sched_domains_llc_id_allocmask;
ସ୍ଥିର cpumask_var_t sched_domains_tmpmask;
ସ୍ଥିର cpumask_var_t sched_domains_tmpmask2;
ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_lid;

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା __init sched_debug_setup(ଅକ୍ଷର *str)
{
	sched_debug_verbose = true;

	ଫେରନ୍ତୁ 0;
}
early_param("sched_verbose", sched_debug_setup);

#ifdef CONFIG_SCHED_SMT
ସ୍ଥିର bool sched_smt_asym_packing __read_mostly;

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା __init setup_sched_smt_asym_packing(ଅକ୍ଷର *str)
{
	ଯଦି (strcmp(str, "on"))
		ଫେରନ୍ତୁ 0;

	sched_smt_asym_packing = true;
	ଫେରନ୍ତୁ 1;
}
__setup("sched_smt_asym_packing=", setup_sched_smt_asym_packing);
#endif

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool sched_debug(ଶୂନ୍ୟ)
{
	ଫେରନ୍ତୁ sched_debug_verbose;
}

#define SD_FLAG(_name, mflags) [__##_name] = { .meta_flags = mflags, .name = #_name },
ଅଚଳ ସଂରଚନା sd_flag_debug sd_flag_debug[] = {
#ଅନ୍ତର୍ଭାଵ <linux/sched/sd_flags.h>
};
#undef SD_FLAG

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା sched_domain_debug_one(ସଂରଚନା sched_domain *sd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu, ପୂର୍ଣ୍ଣସଂଖ୍ୟା level,
				  ସଂରଚନା cpumask *groupmask)
{
	ସଂରଚନା sched_group *group = sd->groups;
	unsigned long flags = sd->flags;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା idx;

	cpumask_clear(groupmask);

	printk(KERN_DEBUG "%*s domain-%d: ", level, "", level);
	printk(KERN_CONT "span=%*pbl level=%s\n",
	       cpumask_pr_args(sched_domain_span(sd)), sd->name);

	ଯଦି (!cpumask_test_cpu(cpu, sched_domain_span(sd))) {
		printk(KERN_ERR "ERROR: domain->span does not contain CPU%d\n", cpu);
	}
	ଯଦି (group && !cpumask_test_cpu(cpu, sched_group_span(group))) {
		printk(KERN_ERR "ERROR: domain->groups does not contain CPU%d\n", cpu);
	}

	for_each_set_bit(idx, &flags, __SD_FLAG_CNT) {
		unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flag = BIT(idx);
		unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା meta_flags = sd_flag_debug[idx].meta_flags;

		ଯଦି ((meta_flags & SDF_SHARED_CHILD) && sd->child &&
		    !(sd->child->flags & flag))
			printk(KERN_ERR "ERROR: flag %s set here but not in child\n",
			       sd_flag_debug[idx].name);

		ଯଦି ((meta_flags & SDF_SHARED_PARENT) && sd->parent &&
		    !(sd->parent->flags & flag))
			printk(KERN_ERR "ERROR: flag %s set here but not in parent\n",
			       sd_flag_debug[idx].name);
	}

	printk(KERN_DEBUG "%*s groups:", level + 1, "");
	କର {
		ଯଦି (!group) {
			printk("\n");
			printk(KERN_ERR "ERROR: group is NULL\n");
			ଭାଙ୍ଗ;
		}

		ଯଦି (cpumask_empty(sched_group_span(group))) {
			printk(KERN_CONT "\n");
			printk(KERN_ERR "ERROR: empty group\n");
			ଭାଙ୍ଗ;
		}

		ଯଦି (!(sd->flags & SD_NUMA) &&
		    cpumask_intersects(groupmask, sched_group_span(group))) {
			printk(KERN_CONT "\n");
			printk(KERN_ERR "ERROR: repeated CPUs\n");
			ଭାଙ୍ଗ;
		}

		cpumask_or(groupmask, groupmask, sched_group_span(group));

		printk(KERN_CONT " %d:{ span=%*pbl",
				group->sgc->id,
				cpumask_pr_args(sched_group_span(group)));

		ଯଦି ((sd->flags & SD_NUMA) &&
		    !cpumask_equal(group_balance_mask(group), sched_group_span(group))) {
			printk(KERN_CONT " mask=%*pbl",
				cpumask_pr_args(group_balance_mask(group)));
		}

		ଯଦି (group->sgc->capacity != SCHED_CAPACITY_SCALE)
			printk(KERN_CONT " cap=%lu", group->sgc->capacity);

		ଯଦି (group == sd->groups && sd->child &&
		    !cpumask_equal(sched_domain_span(sd->child),
				   sched_group_span(group))) {
			printk(KERN_ERR "ERROR: domain->groups does not match domain->child\n");
		}

		printk(KERN_CONT " }");

		group = group->next;

		ଯଦି (group != sd->groups)
			printk(KERN_CONT ",");

	} ଯେତେବେଳେ (group != sd->groups);
	printk(KERN_CONT "\n");

	ଯଦି (!cpumask_equal(sched_domain_span(sd), groupmask))
		printk(KERN_ERR "ERROR: groups don't span domain->span\n");

	ଯଦି (sd->parent &&
	    !cpumask_subset(groupmask, sched_domain_span(sd->parent)))
		printk(KERN_ERR "ERROR: parent span is not a superset of domain->span\n");
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ sched_domain_debug(ସଂରଚନା sched_domain *sd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା level = 0;

	ଯଦି (!sched_debug_verbose)
		ଫେରନ୍ତୁ;

	ଯଦି (!sd) {
		printk(KERN_DEBUG "CPU%d attaching NULL sched-domain.\n", cpu);
		ଫେରନ୍ତୁ;
	}

	printk(KERN_DEBUG "CPU%d attaching sched-domain(s):\n", cpu);

	ପାଇଁ (;;) {
		ଯଦି (sched_domain_debug_one(sd, cpu, level, sched_domains_tmpmask))
			ଭାଙ୍ଗ;
		level++;
		sd = sd->parent;
		ଯଦି (!sd)
			ଭାଙ୍ଗ;
	}
}

/* Generate a mask of SD flags with the SDF_NEEDS_GROUPS metaflag */
#define SD_FLAG(name, mflags) (name * !!((mflags) & SDF_NEEDS_GROUPS)) |
ସ୍ଥିର ଅଚଳ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା SD_DEGENERATE_GROUPS_MASK =
#ଅନ୍ତର୍ଭାଵ <linux/sched/sd_flags.h>
0;
#undef SD_FLAG

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା sd_degenerate(ସଂରଚନା sched_domain *sd)
{
	ଯଦି (cpumask_weight(sched_domain_span(sd)) == 1)
		ଫେରନ୍ତୁ 1;

	/* Following flags need at least 2 groups */
	ଯଦି ((sd->flags & SD_DEGENERATE_GROUPS_MASK) &&
	    (sd->groups != sd->groups->next))
		ଫେରନ୍ତୁ 0;

	/* Following flags don't use groups */
	ଯଦି (sd->flags & (SD_WAKE_AFFINE))
		ଫେରନ୍ତୁ 0;

	ଫେରନ୍ତୁ 1;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
sd_parent_degenerate(ସଂରଚନା sched_domain *sd, ସଂରଚନା sched_domain *parent)
{
	unsigned long cflags = sd->flags, pflags = parent->flags;

	ଯଦି (sd_degenerate(parent))
		ଫେରନ୍ତୁ 1;

	ଯଦି (!cpumask_equal(sched_domain_span(sd), sched_domain_span(parent)))
		ଫେରନ୍ତୁ 0;

	/* Flags needing groups don't count if only 1 group in parent */
	ଯଦି (parent->groups == parent->groups->next)
		pflags &= ~SD_DEGENERATE_GROUPS_MASK;

	ଯଦି (~cflags & pflags)
		ଫେରନ୍ତୁ 0;

	ଫେରନ୍ତୁ 1;
}

#ଯଦି defined(CONFIG_ENERGY_MODEL) && defined(CONFIG_CPU_FREQ_GOV_SCHEDUTIL)
DEFINE_STATIC_ଚାବି_FALSE(sched_energy_present);
ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା sysctl_sched_energy_aware = 1;
ସ୍ଥିର DEFINE_MUTEX(sched_energy_mutex);
ସ୍ଥିର bool sched_energy_update;

ସ୍ଥିର bool sched_is_eas_possible(ଅଚଳ ସଂରଚନା cpumask *cpu_mask)
{
	bool any_asym_capacity = false;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	/* EAS is enabled for asymmetric CPU capacity topologies. */
	for_each_cpu(i, cpu_mask) {
		ଯଦି (rcu_access_pointer(per_cpu(sd_asym_cpucapacity, i))) {
			any_asym_capacity = true;
			ଭାଙ୍ଗ;
		}
	}
	ଯଦି (!any_asym_capacity) {
		ଯଦି (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS, CPUs do not have asymmetric capacities\n",
				cpumask_pr_args(cpu_mask));
		}
		ଫେରନ୍ତୁ false;
	}

	/* EAS definitely does *not* handle SMT */
	ଯଦି (sched_smt_active()) {
		ଯଦି (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS, SMT is not supported\n",
				cpumask_pr_args(cpu_mask));
		}
		ଫେରନ୍ତୁ false;
	}

	ଯଦି (!arch_scale_freq_invariant()) {
		ଯଦି (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS: frequency-invariant load tracking not yet supported",
				cpumask_pr_args(cpu_mask));
		}
		ଫେରନ୍ତୁ false;
	}

	ଯଦି (!cpufreq_ready_for_eas(cpu_mask)) {
		ଯଦି (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS: cpufreq is not ready\n",
				cpumask_pr_args(cpu_mask));
		}
		ଫେରନ୍ତୁ false;
	}

	ଫେରନ୍ତୁ true;
}

ଶୂନ୍ୟ rebuild_sched_domains_energy(ଶୂନ୍ୟ)
{
	mutex_lock(&sched_energy_mutex);
	sched_energy_update = true;
	rebuild_sched_domains();
	sched_energy_update = false;
	mutex_unlock(&sched_energy_mutex);
}

#ifdef CONFIG_SYSCTL
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା sched_energy_aware_handler(ଅଚଳ ସଂରଚନା ctl_table *table, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଲିଖ,
		ଶୂନ୍ୟ *buffer, size_t *lenp, loff_t *ppos)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	ଯଦି (ଲିଖ && !capable(CAP_SYS_ADMIN))
		ଫେରନ୍ତୁ -EPERM;

	ଯଦି (!sched_is_eas_possible(cpu_active_mask)) {
		ଯଦି (ଲିଖ) {
			ଫେରନ୍ତୁ -EOPNOTSUPP;
		} ଅନ୍ୟଥା {
			*lenp = 0;
			ଫେରନ୍ତୁ 0;
		}
	}

	ret = proc_dointvec_minmax(table, ଲିଖ, buffer, lenp, ppos);
	ଯଦି (!ret && ଲିଖ) {
		ଯଦି (sysctl_sched_energy_aware != sched_energy_enabled())
			rebuild_sched_domains_energy();
	}

	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ଅଚଳ ସଂରଚନା ctl_table sched_energy_aware_sysctls[] = {
	{
		.procname       = "sched_energy_aware",
		.data           = &sysctl_sched_energy_aware,
		.maxlen         = ଆକାର(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା),
		.mode           = 0644,
		.proc_handler   = sched_energy_aware_handler,
		.extra1         = SYSCTL_ZERO,
		.extra2         = SYSCTL_ONE,
	},
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା __init sched_energy_aware_sysctl_init(ଶୂନ୍ୟ)
{
	register_sysctl_init("kernel", sched_energy_aware_sysctls);
	ଫେରନ୍ତୁ 0;
}

late_initcall(sched_energy_aware_sysctl_init);
#endif /* CONFIG_SYSCTL */

ସ୍ଥିର ଶୂନ୍ୟ free_pd(ସଂରଚନା perf_domain *pd)
{
	ସଂରଚନା perf_domain *tmp;

	ଯେତେବେଳେ (pd) {
		tmp = pd->next;
		kfree(pd);
		pd = tmp;
	}
}

ସ୍ଥିର ସଂରଚନା perf_domain *find_pd(ସଂରଚନା perf_domain *pd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ଯେତେବେଳେ (pd) {
		ଯଦି (cpumask_test_cpu(cpu, perf_domain_span(pd)))
			ଫେରନ୍ତୁ pd;
		pd = pd->next;
	}

	ଫେରନ୍ତୁ ଖାଲି;
}

ସ୍ଥିର ସଂରଚନା perf_domain *pd_init(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ସଂରଚନା em_perf_domain *obj = em_cpu_get(cpu);
	ସଂରଚନା perf_domain *pd;

	ଯଦି (!obj) {
		ଯଦି (sched_debug())
			pr_info("%s: no EM found for CPU%d\n", __func__, cpu);
		ଫେରନ୍ତୁ ଖାଲି;
	}

	pd = kzalloc_obj(*pd);
	ଯଦି (!pd)
		ଫେରନ୍ତୁ ଖାଲି;
	pd->em_pd = obj;

	ଫେରନ୍ତୁ pd;
}

ସ୍ଥିର ଶୂନ୍ୟ perf_domain_debug(ଅଚଳ ସଂରଚନା cpumask *cpu_map,
						ସଂରଚନା perf_domain *pd)
{
	ଯଦି (!sched_debug() || !pd)
		ଫେରନ୍ତୁ;

	printk(KERN_DEBUG "root_domain %*pbl:", cpumask_pr_args(cpu_map));

	ଯେତେବେଳେ (pd) {
		printk(KERN_CONT " pd%d:{ cpus=%*pbl nr_pstate=%d }",
				cpumask_first(perf_domain_span(pd)),
				cpumask_pr_args(perf_domain_span(pd)),
				em_pd_nr_perf_states(pd->em_pd));
		pd = pd->next;
	}

	printk(KERN_CONT "\n");
}

ସ୍ଥିର ଶୂନ୍ୟ destroy_perf_domain_rcu(ସଂରଚନା rcu_head *rp)
{
	ସଂରଚନା perf_domain *pd;

	pd = container_of(rp, ସଂରଚନା perf_domain, rcu);
	free_pd(pd);
}

ସ୍ଥିର ଶୂନ୍ୟ sched_energy_set(bool has_eas)
{
	ଯଦି (!has_eas && sched_energy_enabled()) {
		ଯଦି (sched_debug())
			pr_info("%s: stopping EAS\n", __func__);
		static_branch_disable_cpuslocked(&sched_energy_present);
	} ଅନ୍ୟଥା ଯଦି (has_eas && !sched_energy_enabled()) {
		ଯଦି (sched_debug())
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
ସ୍ଥିର bool build_perf_domains(ଅଚଳ ସଂରଚନା cpumask *cpu_map)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ସଂରଚନା perf_domain *pd = ଖାଲି, *tmp;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu = cpumask_first(cpu_map);
	ସଂରଚନା ମୂଳ_domain *rd = cpu_rq(cpu)->rd;

	ଯଦି (!sysctl_sched_energy_aware)
		ଯାଅ ମୁକ୍ତ;

	ଯଦି (!sched_is_eas_possible(cpu_map))
		ଯାଅ ମୁକ୍ତ;

	for_each_cpu(i, cpu_map) {
		/* Skip already covered CPUs. */
		ଯଦି (find_pd(pd, i))
			ଜାରି;

		/* Create the new pd and add it to the local list. */
		tmp = pd_init(i);
		ଯଦି (!tmp)
			ଯାଅ ମୁକ୍ତ;
		tmp->next = pd;
		pd = tmp;
	}

	perf_domain_debug(cpu_map, pd);

	/* Attach the new list of performance domains to the root domain. */
	tmp = rd->pd;
	rcu_assign_pointer(rd->pd, pd);
	ଯଦି (tmp)
		call_rcu(&tmp->rcu, destroy_perf_domain_rcu);

	ଫେରନ୍ତୁ !!pd;

ମୁକ୍ତ:
	free_pd(pd);
	tmp = rd->pd;
	rcu_assign_pointer(rd->pd, ଖାଲି);
	ଯଦି (tmp)
		call_rcu(&tmp->rcu, destroy_perf_domain_rcu);

	ଫେରନ୍ତୁ false;
}
#ଅନ୍ୟଥା /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL): */
ସ୍ଥିର ଶୂନ୍ୟ free_pd(ସଂରଚନା perf_domain *pd) { }
#endif /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL) */

ସ୍ଥିର ଶୂନ୍ୟ free_rootdomain(ସଂରଚନା rcu_head *rcu)
{
	ସଂରଚନା ମୂଳ_domain *rd = container_of(rcu, ସଂରଚନା ମୂଳ_domain, rcu);

	cpupri_cleanup(&rd->cpupri);
	cpudl_cleanup(&rd->cpudl);
	free_cpumask_var(rd->dlo_mask);
	free_cpumask_var(rd->rto_mask);
	free_cpumask_var(rd->online);
	free_cpumask_var(rd->span);
	free_pd(rd->pd);
	kfree(rd);
}

ଶୂନ୍ୟ rq_attach_ମୂଳ(ସଂରଚନା rq *rq, ସଂରଚନା ମୂଳ_domain *rd)
{
	ସଂରଚନା ମୂଳ_domain *old_rd = ଖାଲି;
	ସଂରଚନା rq_flags rf;

	rq_lock_irqsave(rq, &rf);

	ଯଦି (rq->rd) {
		old_rd = rq->rd;

		ଯଦି (cpumask_test_cpu(rq->cpu, old_rd->online))
			set_rq_offline(rq);

		cpumask_clear_cpu(rq->cpu, old_rd->span);

		/*
		 * If we don't want to free the old_rd yet then
		 * set old_rd to NULL to skip the freeing later
		 * in this function:
		 */
		ଯଦି (!atomic_dec_and_test(&old_rd->refcount))
			old_rd = ଖାଲି;
	}

	atomic_inc(&rd->refcount);
	rq->rd = rd;

	cpumask_set_cpu(rq->cpu, rd->span);
	ଯଦି (cpumask_test_cpu(rq->cpu, cpu_active_mask))
		set_rq_online(rq);

	/*
	 * Because the rq is not a task, dl_add_task_root_domain() did not
	 * move the fair server bw to the rd if it already started.
	 * Add it now.
	 */
	ଯଦି (rq->fair_server.dl_server)
		__dl_server_attach_root(&rq->fair_server, rq);

#ifdef CONFIG_SCHED_CLASS_EXT
	ଯଦି (rq->ext_server.dl_server)
		__dl_server_attach_root(&rq->ext_server, rq);
#endif

	rq_unlock_irqrestore(rq, &rf);

	ଯଦି (old_rd)
		call_rcu(&old_rd->rcu, free_rootdomain);
}

ଶୂନ୍ୟ sched_get_rd(ସଂରଚନା ମୂଳ_domain *rd)
{
	atomic_inc(&rd->refcount);
}

ଶୂନ୍ୟ sched_put_rd(ସଂରଚନା ମୂଳ_domain *rd)
{
	ଯଦି (!atomic_dec_and_test(&rd->refcount))
		ଫେରନ୍ତୁ;

	call_rcu(&rd->rcu, free_rootdomain);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା init_rootdomain(ସଂରଚନା ମୂଳ_domain *rd)
{
	ଯଦି (!zalloc_cpumask_var(&rd->span, GFP_KERNEL))
		ଯାଅ out;
	ଯଦି (!zalloc_cpumask_var(&rd->online, GFP_KERNEL))
		ଯାଅ free_span;
	ଯଦି (!zalloc_cpumask_var(&rd->dlo_mask, GFP_KERNEL))
		ଯାଅ free_online;
	ଯଦି (!zalloc_cpumask_var(&rd->rto_mask, GFP_KERNEL))
		ଯାଅ free_dlo_mask;

#ifdef HAVE_RT_ଠେଲ_IPI
	rd->rto_cpu = -1;
	raw_spin_lock_init(&rd->rto_lock);
	rd->rto_ଠେଲ_work = IRQ_WORK_INIT_HARD(rto_ଠେଲ_irq_work_func);
#endif

	rd->visit_cookie = 0;
	init_dl_bw(&rd->dl_bw);
	ଯଦି (cpudl_init(&rd->cpudl) != 0)
		ଯାଅ free_rto_mask;

	ଯଦି (cpupri_init(&rd->cpupri) != 0)
		ଯାଅ free_cpudl;
	ଫେରନ୍ତୁ 0;

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
	ଫେରନ୍ତୁ -ENOMEM;
}

/*
 * By default the system creates a single root-domain with all CPUs as
 * members (mimicking the global state we have today).
 */
ସଂରଚନା ମୂଳ_domain def_ମୂଳ_domain;

ଶୂନ୍ୟ __init init_defrootdomain(ଶୂନ୍ୟ)
{
	init_rootdomain(&def_ମୂଳ_domain);

	atomic_set(&def_ମୂଳ_domain.refcount, 1);
}

ସ୍ଥିର ସଂରଚନା ମୂଳ_domain *alloc_rootdomain(ଶୂନ୍ୟ)
{
	ସଂରଚନା ମୂଳ_domain *rd;

	rd = kzalloc_obj(*rd);
	ଯଦି (!rd)
		ଫେରନ୍ତୁ ଖାଲି;

	ଯଦି (init_rootdomain(rd) != 0) {
		kfree(rd);
		ଫେରନ୍ତୁ ଖାଲି;
	}

	ଫେରନ୍ତୁ rd;
}

ସ୍ଥିର ଶୂନ୍ୟ free_sched_groups(ସଂରଚନା sched_group *sg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା free_sgc)
{
	ସଂରଚନା sched_group *tmp, *first;

	ଯଦି (!sg)
		ଫେରନ୍ତୁ;

	first = sg;
	କର {
		tmp = sg->next;

		ଯଦି (free_sgc && atomic_dec_and_test(&sg->sgc->ref))
			kfree(sg->sgc);

		ଯଦି (atomic_dec_and_test(&sg->ref))
			kfree(sg);
		sg = tmp;
	} ଯେତେବେଳେ (sg != first);
}

ସ୍ଥିର ଶୂନ୍ୟ free_sched_domain_shared(ସଂରଚନା sched_domain_shared *sds)
{
	ଯଦି (sds && atomic_dec_and_test(&sds->ref))
		kfree(sds);
}

ସ୍ଥିର ଶୂନ୍ୟ destroy_sched_domain(ସଂରଚନା sched_domain *sd)
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

ସ୍ଥିର ଶୂନ୍ୟ destroy_sched_domains_rcu(ସଂରଚନା rcu_head *rcu)
{
	ସଂରଚନା sched_domain *sd = container_of(rcu, ସଂରଚନା sched_domain, rcu);

	ଯେତେବେଳେ (sd) {
		ସଂରଚନା sched_domain *parent = sd->parent;
		destroy_sched_domain(sd);
		sd = parent;
	}
}

ସ୍ଥିର ଶୂନ୍ୟ destroy_sched_domains(ସଂରଚନା sched_domain *sd)
{
	ଯଦି (sd)
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
DEFINE_PER_CPU(ସଂରଚନା sched_domain __rcu *, sd_llc);
DEFINE_PER_CPU(ପୂର୍ଣ୍ଣସଂଖ୍ୟା, sd_llc_size);
DEFINE_PER_CPU(ପୂର୍ଣ୍ଣସଂଖ୍ୟା, sd_llc_id) = -1;
DEFINE_PER_CPU(ପୂର୍ଣ୍ଣସଂଖ୍ୟା, sd_share_id);
DEFINE_PER_CPU(ସଂରଚନା sched_domain_shared __rcu *, sd_llc_shared);
DEFINE_PER_CPU(ସଂରଚନା sched_domain_shared __rcu *, sd_balance_shared);
DEFINE_PER_CPU(ସଂରଚନା sched_domain __rcu *, sd_numa);
DEFINE_PER_CPU(ସଂରଚନା sched_domain __rcu *, sd_asym_packing);
DEFINE_PER_CPU(ସଂରଚନା sched_domain __rcu *, sd_asym_cpucapacity);

DEFINE_STATIC_ଚାବି_FALSE(sched_asym_cpucapacity);
DEFINE_STATIC_ଚାବି_FALSE(sched_cluster_active);

ସ୍ଥିର ଶୂନ୍ୟ update_ଶୀର୍ଷ_cache_domain(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ସଂରଚନା sched_domain_shared *sds = ଖାଲି;
	ସଂରଚନା sched_domain *sd;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା id = cpu;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା size = 1;

	sd = highest_flag_domain(cpu, SD_SHARE_LLC);
	ଯଦି (sd) {
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
	ଯଦି (sd)
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
	ଯଦି (sd && sd->shared)
		sds = sd->shared;

	rcu_assign_pointer(per_cpu(sd_asym_cpucapacity, cpu), sd);
	rcu_assign_pointer(per_cpu(sd_balance_shared, cpu), sds);
}

/*
 * Attach the domain 'sd' to 'cpu' as its base domain. Callers must
 * hold the hotplug lock.
 */
ସ୍ଥିର ଶୂନ୍ୟ
cpu_attach_domain(ସଂରଚନା sched_domain *sd, ସଂରଚନା ମୂଳ_domain *rd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ସଂରଚନା rq *rq = cpu_rq(cpu);
	ସଂରଚନା sched_domain *tmp;

	/* Remove the sched domains which do not contribute to scheduling. */
	ପାଇଁ (tmp = sd; tmp; ) {
		ସଂରଚନା sched_domain *parent = tmp->parent;
		ଯଦି (!parent)
			ଭାଙ୍ଗ;

		ଯଦି (sd_parent_degenerate(tmp, parent)) {
			tmp->parent = parent->parent;

			/* Pick reference to parent->shared. */
			ଯଦି (parent->shared) {
				/*
				 * It is safe to free a sd->shared that
				 * has not been published yet. If a
				 * sd->shared was published, the refcount
				 * will end up being non-zero and it will
				 * not be freed here.
				 */
				free_sched_domain_shared(tmp->shared);
				tmp->shared = parent->shared;
				parent->shared = ଖାଲି;
			}

			ଯଦି (parent->parent) {
				parent->parent->child = tmp;
				parent->parent->groups->flags = tmp->flags;
			}

			/*
			 * Transfer SD_PREFER_SIBLING down in case of a
			 * degenerate parent; the spans match for this
			 * so the property transfers.
			 */
			ଯଦି (parent->flags & SD_PREFER_SIBLING)
				tmp->flags |= SD_PREFER_SIBLING;
			destroy_sched_domain(parent);
		} ଅନ୍ୟଥା
			tmp = tmp->parent;
	}

	ଯଦି (sd && sd_degenerate(sd)) {
		tmp = sd;
		sd = sd->parent;

		ଯଦି (sd) {
			ସଂରଚନା sched_group *sg = sd->groups;

#ifdef CONFIG_SCHED_CACHE
			/* move buffer to parent as child is being destroyed */
			sd->llc_counts = tmp->llc_counts;
			sd->llc_max = tmp->llc_max;
			sd->llc_bytes = tmp->llc_bytes;
			/* make sure destroy_sched_domain() does not free it */
			tmp->llc_counts = ଖାଲି;
			tmp->llc_max = 0;
			tmp->llc_bytes = 0;
#endif
			/*
			 * sched groups hold the flags of the child sched
			 * domain for convenience. Clear such flags since
			 * the child is being destroyed.
			 */
			କର {
				sg->flags = 0;
			} ଯେତେବେଳେ (sg != sd->groups);

			sd->child = ଖାଲି;
		}

		destroy_sched_domain(tmp);
	}

	sched_domain_debug(sd, cpu);

	rq_attach_ମୂଳ(rq, rd);
	tmp = rq->sd;
	rcu_assign_pointer(rq->sd, sd);
	dirty_sched_domain_sysctl(cpu);
	destroy_sched_domains(tmp);

	update_ଶୀର୍ଷ_cache_domain(cpu);
}

ସଂରଚନା s_data {
	ସଂରଚନା sched_domain_shared * __percpu *sds;
	ସଂରଚନା sched_domain * __percpu *sd;
	ସଂରଚନା ମୂଳ_domain	*rd;
};

ଗଣନା s_alloc {
	sa_rootdomain,
	sa_sd,
	sa_sd_shared,
	sa_sd_storage,
	sa_none,
};

#ifdef CONFIG_SCHED_CACHE
/* hardware support for cache aware scheduling */
DEFINE_STATIC_ଚାବି_FALSE(sched_cache_present);
/*
 * Indicator of whether cache aware scheduling
 * is active, used by the scheduler.
 */
DEFINE_STATIC_ଚାବି_FALSE(sched_cache_active);
/* user wants cache aware scheduling [0 or 1] */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା sysctl_sched_cache_user = 1;

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
ସ୍ଥିର unsigned long get_effective_llc_bytes(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu,
					     ସଂରଚନା sched_domain *sd)
{
	ସଂରଚନା cacheinfo *ci;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା hw_weight;

	ci = get_cpu_cacheinfo_llc(cpu);
	ଯଦି (!ci)
		ଫେରନ୍ତୁ 0;

	hw_weight = cpumask_weight(&ci->shared_cpu_map);
	ଯଦି (!hw_weight)
		ଫେରନ୍ତୁ 0;

	ଫେରନ୍ତୁ div_u64((u64)ci->size * sd->span_weight, hw_weight);
}

ସ୍ଥିର bool alloc_sd_llc(ଅଚଳ ସଂରଚନା cpumask *cpu_map,
			 ସଂରଚନା s_data *d)
{
	ସଂରଚନା sched_domain *sd, *ଶୀର୍ଷ_llc, *parent;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *p;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	for_each_cpu(i, cpu_map) {
		sd = *per_cpu_ptr(d->sd, i);
		ଯଦି (!sd)
			ଯାଅ err;

		p = kcalloc_ଗଣ୍ଠି(max_lid + 1, ଆକାର(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା),
				 GFP_KERNEL, cpu_to_ଗଣ୍ଠି(i));
		ଯଦି (!p)
			ଯାଅ err;

		ଶୀର୍ଷ_llc = sd;
		/*
		 * Find the topmost SD_SHARE_LLC domain.
		 * Not yet attached to the CPU, so per_cpu(sd_llc, i)
		 * can not be used.
		 */
		ଯେତେବେଳେ ((parent = rcu_dereference_protected(ଶୀର୍ଷ_llc->parent, true)) &&
		       (parent->flags & SD_SHARE_LLC))
			ଶୀର୍ଷ_llc = parent;

		ଯଦି (ଶୀର୍ଷ_llc->flags & SD_SHARE_LLC) {
			sd->llc_max = max_lid + 1;
			sd->llc_counts = p;
			sd->llc_bytes = get_effective_llc_bytes(i, ଶୀର୍ଷ_llc);
		} ଅନ୍ୟଥା {
			/* avoid memory leak */
			kfree(p);
		}
	}

	ଫେରନ୍ତୁ true;
err:
	for_each_cpu(i, cpu_map) {
		sd = *per_cpu_ptr(d->sd, i);
		ଯଦି (sd) {
			kfree(sd->llc_counts);
			sd->llc_counts = ଖାଲି;
			sd->llc_max = 0;
			sd->llc_bytes = 0;
		}
	}

	ଫେରନ୍ତୁ false;
}

/*
 * Enable/disable cache aware scheduling according to
 * user input and the presence of hardware support.
 */
ସ୍ଥିର ଶୂନ୍ୟ _sched_cache_active_set(ଶୂନ୍ୟ)
{
	lockdep_assert_cpus_held();
	lockdep_assert_held(&sched_domains_mutex);

	/* hardware does not support */
	ଯଦି (!static_branch_likely(&sched_cache_present)) {
		static_branch_disable_cpuslocked(&sched_cache_active);
		ଯଦି (sched_debug())
			pr_info("%s: cache aware scheduling not supported on this platform\n", __func__);
		ଫେରନ୍ତୁ;
	}

	/*
	 * user wants it or not ?
	 * TBD: read before writing the static key.
	 * It is not in the critical path, leave as-is
	 * for now.
	 */
	ଯଦି (sysctl_sched_cache_user) {
		static_branch_enable_cpuslocked(&sched_cache_active);
		ଯଦି (sched_debug())
			pr_info("%s: enabling cache aware scheduling\n", __func__);
	} ଅନ୍ୟଥା {
		static_branch_disable_cpuslocked(&sched_cache_active);
		ଯଦି (sched_debug())
			pr_info("%s: disabling cache aware scheduling\n", __func__);
	}
}

/* used by debugfs */
ଶୂନ୍ୟ sched_cache_active_set(ଶୂନ୍ୟ)
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
ଶୂନ୍ୟ sched_update_llc_bytes(ଅଚଳ ସଂରଚନା cpumask *cpus)
{
	ସଂରଚନା sched_domain *sd, *sdp;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

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
		ଯଦି (!sdp)
			ଜାରି;

		sd = rcu_dereference_sched_domain(cpu_rq(i)->sd);
		ଯଦି (sd)
			sd->llc_bytes = get_effective_llc_bytes(i, sdp);
	}

	sched_domains_mutex_unlock();
}

ସ୍ଥିର ଶୂନ୍ୟ sched_cache_set(bool has_multi_llcs)
{
	/*
	 * TBD: check before writing to it. sched domain rebuild
	 * is not in the critical path, leave as-is for now.
	 */
	ଯଦି (has_multi_llcs)
		static_branch_enable_cpuslocked(&sched_cache_present);
	ଅନ୍ୟଥା
		static_branch_disable_cpuslocked(&sched_cache_present);

	_sched_cache_active_set();
}
#ଅନ୍ୟଥା
ସ୍ଥିର bool alloc_sd_llc(ଅଚଳ ସଂରଚନା cpumask *cpu_map,
			 ସଂରଚନା s_data *d)
{
	ଫେରନ୍ତୁ false;
}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ sched_cache_set(bool has_multi_llcs) { }
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
ସ୍ଥିର bool sd_in_multi_llcs(ସଂରଚନା sched_domain *sd)
{
	ସଂରଚନା sched_domain *sdp = sd->parent;

	/* it does not make sense to aggregate to 1 CPU */
	ଯଦି (sd->span_weight == 1)
		ଫେରନ୍ତୁ false;

	ଯେତେବେଳେ (sdp && sdp->span_weight == sd->span_weight)
		sdp = sdp->parent;

	ଫେରନ୍ତୁ !!sdp;
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
ପୂର୍ଣ୍ଣସଂଖ୍ୟା group_balance_cpu(ସଂରଚନା sched_group *sg)
{
	ଫେରନ୍ତୁ cpumask_first(group_balance_mask(sg));
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
ସ୍ଥିର ଶୂନ୍ୟ
build_balance_mask(ସଂରଚନା sched_domain *sd, ସଂରଚନା sched_group *sg, ସଂରଚନା cpumask *mask)
{
	ଅଚଳ ସଂରଚନା cpumask *sg_span = sched_group_span(sg);
	ସଂରଚନା sd_data *sdd = sd->ବ୍ୟକ୍ତିଗତ;
	ସଂରଚନା sched_domain *sibling;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	cpumask_clear(mask);

	for_each_cpu(i, sg_span) {
		sibling = *per_cpu_ptr(sdd->sd, i);

		/*
		 * Can happen in the asymmetric case, where these siblings are
		 * unused. The mask will not be empty because those CPUs that
		 * do have the top domain _should_ span the domain.
		 */
		ଯଦି (!sibling->child)
			ଜାରି;

		/* If we would not end up here, we can't continue from here */
		ଯଦି (!cpumask_equal(sg_span, sched_domain_span(sibling->child)))
			ଜାରି;

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
ସ୍ଥିର ସଂରଚନା sched_group *
build_group_from_child_sched_domain(ସଂରଚନା sched_domain *sd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ସଂରଚନା sched_group *sg;
	ସଂରଚନା cpumask *sg_span;

	sg = kzalloc_ଗଣ୍ଠି(ଆକାର(ସଂରଚନା sched_group) + cpumask_size(),
			GFP_KERNEL, cpu_to_ଗଣ୍ଠି(cpu));

	ଯଦି (!sg)
		ଫେରନ୍ତୁ ଖାଲି;

	sg_span = sched_group_span(sg);
	ଯଦି (sd->child) {
		cpumask_copy(sg_span, sched_domain_span(sd->child));
		sg->flags = sd->child->flags;
	} ଅନ୍ୟଥା {
		cpumask_copy(sg_span, sched_domain_span(sd));
	}

	atomic_inc(&sg->ref);
	ଫେରନ୍ତୁ sg;
}

ସ୍ଥିର ଶୂନ୍ୟ init_overlap_sched_group(ସଂରଚନା sched_domain *sd,
				     ସଂରଚନା sched_group *sg)
{
	ସଂରଚନା cpumask *mask = sched_domains_tmpmask2;
	ସଂରଚନା sd_data *sdd = sd->ବ୍ୟକ୍ତିଗତ;
	ସଂରଚନା cpumask *sg_span;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu;

	build_balance_mask(sd, sg, mask);
	cpu = cpumask_first(mask);

	sg->sgc = *per_cpu_ptr(sdd->sgc, cpu);
	ଯଦି (atomic_inc_return(&sg->sgc->ref) == 1)
		cpumask_copy(group_balance_mask(sg), mask);
	ଅନ୍ୟଥା
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

ସ୍ଥିର ସଂରଚନା sched_domain *
find_descended_sibling(ସଂରଚନା sched_domain *sd, ସଂରଚନା sched_domain *sibling)
{
	/*
	 * The proper descendant would be the one whose child won't span out
	 * of sd
	 */
	ଯେତେବେଳେ (sibling->child &&
	       !cpumask_subset(sched_domain_span(sibling->child),
			       sched_domain_span(sd)))
		sibling = sibling->child;

	/*
	 * As we are referencing sgc across different topology level, we need
	 * to go down to skip those sched_domains which don't contribute to
	 * scheduling because they will be degenerated in cpu_attach_domain
	 */
	ଯେତେବେଳେ (sibling->child &&
	       cpumask_equal(sched_domain_span(sibling->child),
			     sched_domain_span(sibling)))
		sibling = sibling->child;

	ଫେରନ୍ତୁ sibling;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
build_overlap_sched_groups(ସଂରଚନା sched_domain *sd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ସଂରଚନା sched_group *first = ଖାଲି, *last = ଖାଲି, *sg;
	ଅଚଳ ସଂରଚନା cpumask *span = sched_domain_span(sd);
	ସଂରଚନା cpumask *covered = sched_domains_tmpmask;
	ସଂରଚନା sd_data *sdd = sd->ବ୍ୟକ୍ତିଗତ;
	ସଂରଚନା sched_domain *sibling;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	cpumask_clear(covered);

	for_each_cpu_wrap(i, span, cpu) {
		ସଂରଚନା cpumask *sg_span;

		ଯଦି (cpumask_test_cpu(i, covered))
			ଜାରି;

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
		ଯଦି (!cpumask_test_cpu(i, sched_domain_span(sibling)))
			ଜାରି;

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
		ଯଦି (sibling->child &&
		    !cpumask_subset(sched_domain_span(sibling->child), span))
			sibling = find_descended_sibling(sd, sibling);

		sg = build_group_from_child_sched_domain(sibling, cpu);
		ଯଦି (!sg)
			ଯାଅ fail;

		sg_span = sched_group_span(sg);
		cpumask_or(covered, covered, sg_span);

		init_overlap_sched_group(sibling, sg);

		ଯଦି (!first)
			first = sg;
		ଯଦି (last)
			last->next = sg;
		last = sg;
		last->next = first;
	}
	sd->groups = first;

	ଫେରନ୍ତୁ 0;

fail:
	free_sched_groups(first, 0);

	ଫେରନ୍ତୁ -ENOMEM;
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

ସ୍ଥିର ସଂରଚନା sched_group *get_group(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu, ସଂରଚନା sd_data *sdd)
{
	ସଂରଚନା sched_domain *sd = *per_cpu_ptr(sdd->sd, cpu);
	ସଂରଚନା sched_domain *child = sd->child;
	ସଂରଚନା sched_group *sg;
	bool already_visited;

	ଯଦି (child)
		cpu = cpumask_first(sched_domain_span(child));

	sg = *per_cpu_ptr(sdd->sg, cpu);
	sg->sgc = *per_cpu_ptr(sdd->sgc, cpu);

	/* Increase refcounts for claim_allocations: */
	already_visited = atomic_inc_return(&sg->ref) > 1;
	/* sgc visits should follow a similar trend as sg */
	WARN_ON(already_visited != (atomic_inc_return(&sg->sgc->ref) > 1));

	/* If we have already visited that group, it's already initialized. */
	ଯଦି (already_visited)
		ଫେରନ୍ତୁ sg;

	ଯଦି (child) {
		cpumask_copy(sched_group_span(sg), sched_domain_span(child));
		cpumask_copy(group_balance_mask(sg), sched_group_span(sg));
		sg->flags = child->flags;
	} ଅନ୍ୟଥା {
		cpumask_set_cpu(cpu, sched_group_span(sg));
		cpumask_set_cpu(cpu, group_balance_mask(sg));
	}

	sg->sgc->capacity = SCHED_CAPACITY_SCALE * cpumask_weight(sched_group_span(sg));
	sg->sgc->min_capacity = SCHED_CAPACITY_SCALE;
	sg->sgc->max_capacity = SCHED_CAPACITY_SCALE;

	ଫେରନ୍ତୁ sg;
}

/*
 * build_sched_groups will build a circular linked list of the groups
 * covered by the given span, will set each group's ->cpumask correctly,
 * and will initialize their ->sgc.
 *
 * Assumes the sched_domain tree is fully constructed
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
build_sched_groups(ସଂରଚନା sched_domain *sd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ସଂରଚନା sched_group *first = ଖାଲି, *last = ଖାଲି;
	ସଂରଚନା sd_data *sdd = sd->ବ୍ୟକ୍ତିଗତ;
	ଅଚଳ ସଂରଚନା cpumask *span = sched_domain_span(sd);
	ସଂରଚନା cpumask *covered;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	lockdep_assert_held(&sched_domains_mutex);
	covered = sched_domains_tmpmask;

	cpumask_clear(covered);

	for_each_cpu_wrap(i, span, cpu) {
		ସଂରଚନା sched_group *sg;

		ଯଦି (cpumask_test_cpu(i, covered))
			ଜାରି;

		sg = get_group(i, sdd);

		cpumask_or(covered, covered, sched_group_span(sg));

		ଯଦି (!first)
			first = sg;
		ଯଦି (last)
			last->next = sg;
		last = sg;
	}
	last->next = first;
	sd->groups = first;

	ଫେରନ୍ତୁ 0;
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
ସ୍ଥିର ଶୂନ୍ୟ init_sched_groups_capacity(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu, ସଂରଚନା sched_domain *sd)
{
	ସଂରଚନା sched_group *sg = sd->groups;
	ସଂରଚନା cpumask *mask = sched_domains_tmpmask2;

	WARN_ON(!sg);

	କର {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu, cores = 0, max_cpu = -1;

		sg->group_weight = cpumask_weight(sched_group_span(sg));

		cpumask_copy(mask, sched_group_span(sg));
		for_each_cpu(cpu, mask) {
			cores++;
			cpumask_andnot(mask, mask, cpu_smt_mask(cpu));
		}
		sg->cores = cores;

		ଯଦି (!(sd->flags & SD_ASYM_PACKING))
			ଯାଅ next;

		for_each_cpu(cpu, sched_group_span(sg)) {
			ଯଦି (max_cpu < 0)
				max_cpu = cpu;
			ଅନ୍ୟଥା ଯଦି (sched_asym_prefer(cpu, max_cpu))
				max_cpu = cpu;
		}
		sg->asym_prefer_cpu = max_cpu;

next:
		sg = sg->next;
	} ଯେତେବେଳେ (sg != sd->groups);

	ଯଦି (cpu != group_balance_cpu(sg))
		ଫେରନ୍ତୁ;

	update_group_capacity(sd, cpu);
}

/* Update the "asym_prefer_cpu" when arch_asym_cpu_priority() changes. */
ଶୂନ୍ୟ sched_update_asym_prefer_cpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu, ପୂର୍ଣ୍ଣସଂଖ୍ୟା old_prio, ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_prio)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା asym_prefer_cpu = cpu;
	ସଂରଚନା sched_domain *sd;

	guard(rcu)();

	for_each_domain(cpu, sd) {
		ସଂରଚନା sched_group *sg;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା group_cpu;

		ଯଦି (!(sd->flags & SD_ASYM_PACKING))
			ଜାରି;

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
		ଯଦି (cpu != sg->asym_prefer_cpu) {
			/*
			 * Since the parent is a superset of the current group,
			 * if the cpu is not the "asym_prefer_cpu" at the
			 * current level, it cannot be the preferred CPU at a
			 * higher levels either.
			 */
			ଯଦି (!sched_asym_prefer(cpu, sg->asym_prefer_cpu))
				ଫେରନ୍ତୁ;

			WRITE_ONCE(sg->asym_prefer_cpu, cpu);
			ଜାରି;
		}

		/* Ranking has improved; CPU is still the preferred one. */
		ଯଦି (new_prio >= old_prio)
			ଜାରି;

		for_each_cpu(group_cpu, sched_group_span(sg)) {
			ଯଦି (sched_asym_prefer(group_cpu, asym_prefer_cpu))
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
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
asym_cpu_capacity_classify(ଅଚଳ ସଂରଚନା cpumask *sd_span,
			   ଅଚଳ ସଂରଚନା cpumask *cpu_map)
{
	ସଂରଚନା asym_cap_data *entry;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = 0, miss = 0;

	/*
	 * Count how many unique CPU capacities this domain spans across
	 * (compare sched_domain CPUs mask with ones representing  available
	 * CPUs capacities). Take into account CPUs that might be offline:
	 * skip those.
	 */
	list_for_each_entry(entry, &asym_cap_list, link) {
		ଯଦି (cpumask_intersects(sd_span, cpu_capacity_span(entry)))
			++ଗଣନା_2;
		ଅନ୍ୟଥା ଯଦି (cpumask_intersects(cpu_map, cpu_capacity_span(entry)))
			++miss;
	}

	WARN_ON_ONCE(!ଗଣନା_2 && !list_empty(&asym_cap_list));

	/* No asymmetry detected */
	ଯଦି (ଗଣନା_2 < 2)
		ଫେରନ୍ତୁ 0;
	/* Some of the available CPU capacity values have not been detected */
	ଯଦି (miss)
		ଫେରନ୍ତୁ SD_ASYM_CPUCAPACITY;

	/* Full asymmetry */
	ଫେରନ୍ତୁ SD_ASYM_CPUCAPACITY | SD_ASYM_CPUCAPACITY_FULL;

}

ସ୍ଥିର ଶୂନ୍ୟ free_asym_cap_entry(ସଂରଚନା rcu_head *head)
{
	ସଂରଚନା asym_cap_data *entry = container_of(head, ସଂରଚନା asym_cap_data, rcu);
	kfree(entry);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ asym_cpu_capacity_update_data(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	unsigned long capacity = arch_scale_cpu_capacity(cpu);
	ସଂରଚନା asym_cap_data *ଭର୍ତ୍ତି_entry = ଖାଲି;
	ସଂରଚନା asym_cap_data *entry;

	/*
	 * Search if capacity already exits. If not, track which the entry
	 * where we should insert to keep the list ordered descending.
	 */
	list_for_each_entry(entry, &asym_cap_list, link) {
		ଯଦି (capacity == entry->capacity)
			ଯାଅ done;
		ଅନ୍ୟଥା ଯଦି (!ଭର୍ତ୍ତି_entry && capacity > entry->capacity)
			ଭର୍ତ୍ତି_entry = list_prev_entry(entry, link);
	}

	entry = kzalloc(ଆକାର(*entry) + cpumask_size(), GFP_KERNEL);
	ଯଦି (WARN_ONCE(!entry, "Failed to allocate memory for asymmetry data\n"))
		ଫେରନ୍ତୁ;
	entry->capacity = capacity;

	/* If NULL then the new capacity is the smallest, add last. */
	ଯଦି (!ଭର୍ତ୍ତି_entry)
		list_add_tail_rcu(&entry->link, &asym_cap_list);
	ଅନ୍ୟଥା
		list_add_rcu(&entry->link, &ଭର୍ତ୍ତି_entry->link);
done:
	__cpumask_set_cpu(cpu, cpu_capacity_span(entry));
}

/*
 * Build-up/update list of CPUs grouped by their capacities
 * An update requires explicit request to rebuild sched domains
 * with state indicating CPU topology changes.
 */
ସ୍ଥିର ଶୂନ୍ୟ asym_cpu_capacity_scan(ଶୂନ୍ୟ)
{
	ସଂରଚନା asym_cap_data *entry, *next;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu;

	list_for_each_entry(entry, &asym_cap_list, link)
		cpumask_clear(cpu_capacity_span(entry));

	for_each_cpu_and(cpu, cpu_possible_mask, housekeeping_cpumask(HK_TYPE_DOMAIN))
		asym_cpu_capacity_update_data(cpu);

	list_for_each_entry_safe(entry, next, &asym_cap_list, link) {
		ଯଦି (cpumask_empty(cpu_capacity_span(entry))) {
			list_del_rcu(&entry->link);
			call_rcu(&entry->rcu, free_asym_cap_entry);
		}
	}

	/*
	 * Only one capacity value has been detected i.e. this system is symmetric.
	 * No need to keep this data around.
	 */
	ଯଦି (list_is_singular(&asym_cap_list)) {
		entry = list_first_entry(&asym_cap_list, typeof(*entry), link);
		list_del_rcu(&entry->link);
		call_rcu(&entry->rcu, free_asym_cap_entry);
	}
}

/*
 * Initializers for schedule domains
 * Non-inlined to reduce accumulated stack pressure in build_sched_domains()
 */

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା default_relax_domain_level = -1;
ପୂର୍ଣ୍ଣସଂଖ୍ୟା sched_domain_level_max;

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା __init setup_relax_domain_level(ଅକ୍ଷର *str)
{
	ଯଦି (kstrtoint(str, 0, &default_relax_domain_level))
		pr_warn("Unable to set relax_domain_level\n");

	ଫେରନ୍ତୁ 1;
}
__setup("relax_domain_level=", setup_relax_domain_level);

ସ୍ଥିର ଶୂନ୍ୟ set_domain_attribute(ସଂରଚନା sched_domain *sd,
				 ସଂରଚନା sched_domain_attr *attr)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା request;

	ଯଦି (!attr || attr->relax_domain_level < 0) {
		ଯଦି (default_relax_domain_level < 0)
			ଫେରନ୍ତୁ;
		request = default_relax_domain_level;
	} ଅନ୍ୟଥା
		request = attr->relax_domain_level;

	ଯଦି (sd->level >= request) {
		/* Turn off idle balance on this domain: */
		sd->flags &= ~(SD_BALANCE_WAKE|SD_BALANCE_NEWIDLE);
	}
}

ସ୍ଥିର ଶୂନ୍ୟ __sdt_free(ଅଚଳ ସଂରଚନା cpumask *cpu_map);
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା __sdt_alloc(ଅଚଳ ସଂରଚନା cpumask *cpu_map);

ସ୍ଥିର ଶୂନ୍ୟ __sds_free(ସଂରଚନା s_data *d, ଅଚଳ ସଂରଚନା cpumask *cpu_map);
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା __sds_alloc(ସଂରଚନା s_data *d, ଅଚଳ ସଂରଚନା cpumask *cpu_map);

ସ୍ଥିର ଶୂନ୍ୟ __free_domain_allocs(ସଂରଚନା s_data *d, ଗଣନା s_alloc what,
				 ଅଚଳ ସଂରଚନା cpumask *cpu_map)
{
	ବାଛ (what) {
	ସ୍ଥିତି sa_rootdomain:
		ଯଦି (!atomic_read(&d->rd->refcount))
			free_rootdomain(&d->rd->rcu);
		fallthrough;
	ସ୍ଥିତି sa_sd:
		free_percpu(d->sd);
		fallthrough;
	ସ୍ଥିତି sa_sd_shared:
		__sds_free(d, cpu_map);
		fallthrough;
	ସ୍ଥିତି sa_sd_storage:
		__sdt_free(cpu_map);
		fallthrough;
	ସ୍ଥିତି sa_none:
		ଭାଙ୍ଗ;
	}
}

ସ୍ଥିର ଗଣନା s_alloc
__visit_domain_allocation_hell(ସଂରଚନା s_data *d, ଅଚଳ ସଂରଚନା cpumask *cpu_map)
{
	memset(d, 0, ଆକାର(*d));

	ଯଦି (__sdt_alloc(cpu_map))
		ଫେରନ୍ତୁ sa_sd_storage;
	ଯଦି (__sds_alloc(d, cpu_map))
		ଫେରନ୍ତୁ sa_sd_shared;
	d->sd = alloc_percpu(ସଂରଚନା sched_domain *);
	ଯଦି (!d->sd)
		ଫେରନ୍ତୁ sa_sd_shared;
	d->rd = alloc_rootdomain();
	ଯଦି (!d->rd)
		ଫେରନ୍ତୁ sa_sd;

	ଫେରନ୍ତୁ sa_rootdomain;
}

/*
 * NULL the sd_data elements we've used to build the sched_domain and
 * sched_group structure so that the subsequent __free_domain_allocs()
 * will not free the data we're using.
 */
ସ୍ଥିର ଶୂନ୍ୟ claim_allocations(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu, ସଂରଚନା s_data *d)
{
	ସଂରଚନା sched_domain *sd;

	ଯଦି (atomic_read(&(*per_cpu_ptr(d->sds, cpu))->ref))
		*per_cpu_ptr(d->sds, cpu) = ଖାଲି;

	ପାଇଁ (sd = *per_cpu_ptr(d->sd, cpu); sd; sd = sd->parent) {
		ସଂରଚନା sd_data *sdd = sd->ବ୍ୟକ୍ତିଗତ;

		WARN_ON_ONCE(*per_cpu_ptr(sdd->sd, cpu) != sd);
		*per_cpu_ptr(sdd->sd, cpu) = ଖାଲି;

		ଯଦି (atomic_read(&(*per_cpu_ptr(sdd->sg, cpu))->ref))
			*per_cpu_ptr(sdd->sg, cpu) = ଖାଲି;

		ଯଦି (atomic_read(&(*per_cpu_ptr(sdd->sgc, cpu))->ref))
			*per_cpu_ptr(sdd->sgc, cpu) = ଖାଲି;
	}
}

#ifdef CONFIG_NUMA
ଗଣନା numa_topology_type sched_numa_topology_type;

/*
 * sched_domains_numa_distance is derived from sched_numa_node_distance
 * and provides a simplified view of NUMA distances used specifically
 * for building NUMA scheduling domains.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା			sched_domains_numa_levels;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା			sched_numa_ଗଣ୍ଠି_levels;

ପୂର୍ଣ୍ଣସଂଖ୍ୟା				sched_max_numa_distance;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା			*sched_domains_numa_distance;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା			*sched_numa_ଗଣ୍ଠି_distance;
ସ୍ଥିର ସଂରଚନା cpumask		***sched_domains_numa_masks;
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

ସ୍ଥିର ସଂରଚନା sched_domain *
sd_init(ସଂରଚନା sched_domain_topology_level *tl,
	ଅଚଳ ସଂରଚନା cpumask *cpu_map,
	ସଂରଚନା sched_domain *child, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ସଂରଚନା sd_data *sdd = &tl->data;
	ସଂରଚନା sched_domain *sd = *per_cpu_ptr(sdd->sd, cpu);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା sd_id, sd_weight, sd_flags = 0;
	ସଂରଚନା cpumask *sd_span;
	u64 now = sched_clock();

	sd_span = sched_domain_span(sd);
	cpumask_and(sd_span, cpu_map, tl->mask(tl, cpu));
	sd_weight = cpumask_weight(sd_span);
	sd_id = cpumask_first(sd_span);

	ଯଦି (tl->sd_flags)
		sd_flags = (*tl->sd_flags)();
	ଯଦି (WARN_ONCE(sd_flags & ~TOPOLOGY_SD_FLAGS,
		      "wrong sd_flags in topology description\n"))
		sd_flags &= TOPOLOGY_SD_FLAGS;
#ifdef CONFIG_SCHED_SMT
	ଯଦି (sched_smt_asym_packing && (sd_flags & SD_SHARE_CPUCAPACITY))
		sd_flags |= SD_ASYM_PACKING;
#endif
	sd_flags |= asym_cpu_capacity_classify(sd_span, cpu_map);

	*sd = (ସଂରଚନା sched_domain){
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
	ଯଦି (sd->flags & SD_SHARE_CPUCAPACITY) {
		sd->imbalance_pct = 110;

	} ଅନ୍ୟଥା ଯଦି (sd->flags & SD_SHARE_LLC) {
		sd->imbalance_pct = 117;
		sd->cache_nice_tries = 1;

#ifdef CONFIG_NUMA
	} ଅନ୍ୟଥା ଯଦି (sd->flags & SD_NUMA) {
		sd->cache_nice_tries = 2;

		sd->flags &= ~SD_PREFER_SIBLING;
		sd->flags |= SD_SERIALIZE;
		ଯଦି (sched_domains_numa_distance[tl->numa_level] > ଗଣ୍ଠି_reclaim_distance) {
			sd->flags &= ~(SD_BALANCE_EXEC |
				       SD_BALANCE_FORK |
				       SD_WAKE_AFFINE);
		}

#endif /* CONFIG_NUMA */
	} ଅନ୍ୟଥା {
		sd->cache_nice_tries = 1;
	}

	sd->ବ୍ୟକ୍ତିଗତ = sdd;

	ଫେରନ୍ତୁ sd;
}

#ifdef CONFIG_SCHED_SMT
ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu_smt_flags(ଶୂନ୍ୟ)
{
	ଫେରନ୍ତୁ SD_SHARE_CPUCAPACITY | SD_SHARE_LLC;
}

ଅଚଳ ସଂରଚନା cpumask *tl_smt_mask(ସଂରଚନା sched_domain_topology_level *tl, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ଫେରନ୍ତୁ cpu_smt_mask(cpu);
}
#endif

#ifdef CONFIG_SCHED_CLUSTER
ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu_cluster_flags(ଶୂନ୍ୟ)
{
	ଫେରନ୍ତୁ SD_CLUSTER | SD_SHARE_LLC;
}

ଅଚଳ ସଂରଚନା cpumask *tl_cls_mask(ସଂରଚନା sched_domain_topology_level *tl, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ଫେରନ୍ତୁ cpu_clustergroup_mask(cpu);
}
#endif

#ifdef CONFIG_SCHED_MC
ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu_core_flags(ଶୂନ୍ୟ)
{
	ଫେରନ୍ତୁ SD_SHARE_LLC;
}

ଅଚଳ ସଂରଚନା cpumask *tl_mc_mask(ସଂରଚନା sched_domain_topology_level *tl, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ଫେରନ୍ତୁ cpu_coregroup_mask(cpu);
}

/*
 * Majority of architectures have LLC at MC domain level with exception
 * such as powerpc. Provide a way for arch to specify where its LLC is
 * if it falls in exception category
 */
# ifndef arch_llc_mask
#define arch_llc_mask(cpu) cpu_coregroup_mask(cpu)
# endif

#ଅନ୍ୟଥା
#define arch_llc_mask(cpu) cpumask_of(cpu)
#endif

#define llc_mask(cpu) arch_llc_mask(cpu)

ଅଚଳ ସଂରଚନା cpumask *tl_pkg_mask(ସଂରଚନା sched_domain_topology_level *tl, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ଫେରନ୍ତୁ cpu_ଗଣ୍ଠି_mask(cpu);
}

/*
 * Topology list, bottom-up.
 */
ସ୍ଥିର ସଂରଚନା sched_domain_topology_level default_topology[] = {
#ifdef CONFIG_SCHED_SMT
	SDTL_INIT(tl_smt_mask, cpu_smt_flags, SMT),
#endif

#ifdef CONFIG_SCHED_CLUSTER
	SDTL_INIT(tl_cls_mask, cpu_cluster_flags, CLS),
#endif

#ifdef CONFIG_SCHED_MC
	SDTL_INIT(tl_mc_mask, cpu_core_flags, MC),
#endif
	SDTL_INIT(tl_pkg_mask, ଖାଲି, PKG),
	{ ଖାଲି, },
};

ସ୍ଥିର ସଂରଚନା sched_domain_topology_level *sched_domain_topology =
	default_topology;
ସ୍ଥିର ସଂରଚନା sched_domain_topology_level *sched_domain_topology_saved;

#define for_each_sd_topology(tl)			\
	ପାଇଁ (tl = sched_domain_topology; tl->mask; tl++)

ଶୂନ୍ୟ __init set_sched_topology(ସଂରଚନା sched_domain_topology_level *tl)
{
	ଯଦି (WARN_ON_ONCE(sched_smp_initialized))
		ଫେରନ୍ତୁ;

	sched_domain_topology = tl;
	sched_domain_topology_saved = ଖାଲି;
}

#ifdef CONFIG_NUMA
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu_numa_flags(ଶୂନ୍ୟ)
{
	ଫେରନ୍ତୁ SD_NUMA;
}

ସ୍ଥିର ଅଚଳ ସଂରଚନା cpumask *sd_numa_mask(ସଂରଚନା sched_domain_topology_level *tl, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ଫେରନ୍ତୁ sched_domains_numa_masks[tl->numa_level][cpu_to_ଗଣ୍ଠି(cpu)];
}

ସ୍ଥିର ଶୂନ୍ୟ sched_numa_warn(ଅଚଳ ଅକ୍ଷର *str)
{
	ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା done = false;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,j;

	ଯଦି (done)
		ଫେରନ୍ତୁ;

	done = true;

	printk(KERN_WARNING "ERROR: %s\n\n", str);

	ପାଇଁ (i = 0; i < nr_ଗଣ୍ଠି_ids; i++) {
		printk(KERN_WARNING "  ");
		ପାଇଁ (j = 0; j < nr_ଗଣ୍ଠି_ids; j++) {
			ଯଦି (!ଗଣ୍ଠି_state(i, N_CPU) || !ଗଣ୍ଠି_state(j, N_CPU))
				printk(KERN_CONT "(%02d) ", ଗଣ୍ଠି_distance(i,j));
			ଅନ୍ୟଥା
				printk(KERN_CONT " %02d  ", ଗଣ୍ଠି_distance(i,j));
		}
		printk(KERN_CONT "\n");
	}
	printk(KERN_WARNING "\n");
}

bool find_numa_distance(ପୂର୍ଣ୍ଣସଂଖ୍ୟା distance)
{
	bool found = false;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, *distances;

	ଯଦି (distance == ଗଣ୍ଠି_distance(0, 0))
		ଫେରନ୍ତୁ true;

	rcu_read_lock();
	distances = rcu_dereference(sched_numa_ଗଣ୍ଠି_distance);
	ଯଦି (!distances)
		ଯାଅ unlock;
	ପାଇଁ (i = 0; i < sched_numa_ଗଣ୍ଠି_levels; i++) {
		ଯଦି (distances[i] == distance) {
			found = true;
			ଭାଙ୍ଗ;
		}
	}
unlock:
	rcu_read_unlock();

	ଫେରନ୍ତୁ found;
}

#define for_each_cpu_ଗଣ୍ଠି_but(n, nbut)		\
	for_each_ଗଣ୍ଠି_state(n, N_CPU)		\
		ଯଦି (n == nbut)			\
			ଜାରି;		\
		ଅନ୍ୟଥା

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
ସ୍ଥିର ଶୂନ୍ୟ init_numa_topology_type(ପୂର୍ଣ୍ଣସଂଖ୍ୟା offline_ଗଣ୍ଠି)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା a, b, c, n;

	n = sched_max_numa_distance;

	ଯଦି (sched_domains_numa_levels <= 2) {
		sched_numa_topology_type = NUMA_DIRECT;
		ଫେରନ୍ତୁ;
	}

	for_each_cpu_ଗଣ୍ଠି_but(a, offline_ଗଣ୍ଠି) {
		for_each_cpu_ଗଣ୍ଠି_but(b, offline_ଗଣ୍ଠି) {
			/* Find two nodes furthest removed from each other. */
			ଯଦି (ଗଣ୍ଠି_distance(a, b) < n)
				ଜାରି;

			/* Is there an intermediary node between a and b? */
			for_each_cpu_ଗଣ୍ଠି_but(c, offline_ଗଣ୍ଠି) {
				ଯଦି (ଗଣ୍ଠି_distance(a, c) < n &&
				    ଗଣ୍ଠି_distance(b, c) < n) {
					sched_numa_topology_type =
							NUMA_GLUELESS_MESH;
					ଫେରନ୍ତୁ;
				}
			}

			sched_numa_topology_type = NUMA_BACKPLANE;
			ଫେରନ୍ତୁ;
		}
	}

	pr_err("Failed to find a NUMA topology type, defaulting to DIRECT\n");
	sched_numa_topology_type = NUMA_DIRECT;
}


#define NR_DISTANCE_ମୂଲ୍ୟଗୁଡ଼ିକ (1 << DISTANCE_BITS)

/*
 * An architecture could modify its NUMA distance, to change
 * grouping of NUMA nodes and number of NUMA levels when creating
 * NUMA level sched domains.
 *
 * A NUMA level is created for each unique
 * arch_sched_node_distance.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା numa_ଗଣ୍ଠି_dist(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, ପୂର୍ଣ୍ଣସଂଖ୍ୟା j)
{
	ଫେରନ୍ତୁ ଗଣ୍ଠି_distance(i, j);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା arch_sched_ଗଣ୍ଠି_distance(ପୂର୍ଣ୍ଣସଂଖ୍ୟା from, ପୂର୍ଣ୍ଣସଂଖ୍ୟା to)
			     __weak __alias(numa_ଗଣ୍ଠି_dist);

ସ୍ଥିର bool modified_sched_ଗଣ୍ଠି_distance(ଶୂନ୍ୟ)
{
	ଫେରନ୍ତୁ numa_ଗଣ୍ଠି_dist != arch_sched_ଗଣ୍ଠି_distance;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା sched_record_numa_dist(ପୂର୍ଣ୍ଣସଂଖ୍ୟା offline_ଗଣ୍ଠି, ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*n_dist)(ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ପୂର୍ଣ୍ଣସଂଖ୍ୟା),
				  ପୂର୍ଣ୍ଣସଂଖ୍ୟା **dist, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *levels)
{
	unsigned long *distance_map __free(bitmap) = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା nr_levels = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା *distances;

	/*
	 * O(nr_nodes^2) de-duplicating selection sort -- in order to find the
	 * unique distances in the node_distance() table.
	 */
	distance_map = bitmap_alloc(NR_DISTANCE_ମୂଲ୍ୟଗୁଡ଼ିକ, GFP_KERNEL);
	ଯଦି (!distance_map)
		ଫେରନ୍ତୁ -ENOMEM;

	bitmap_zero(distance_map, NR_DISTANCE_ମୂଲ୍ୟଗୁଡ଼ିକ);
	for_each_cpu_ଗଣ୍ଠି_but(i, offline_ଗଣ୍ଠି) {
		for_each_cpu_ଗଣ୍ଠି_but(j, offline_ଗଣ୍ଠି) {
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା distance = n_dist(i, j);

			ଯଦି (distance < LOCAL_DISTANCE || distance >= NR_DISTANCE_ମୂଲ୍ୟଗୁଡ଼ିକ) {
				sched_numa_warn("Invalid distance value range");
				ଫେରନ୍ତୁ -EINVAL;
			}

			bitmap_set(distance_map, distance, 1);
		}
	}
	/*
	 * We can now figure out how many unique distance values there are and
	 * allocate memory accordingly.
	 */
	nr_levels = bitmap_weight(distance_map, NR_DISTANCE_ମୂଲ୍ୟଗୁଡ଼ିକ);

	distances = kzalloc_objs(ପୂର୍ଣ୍ଣସଂଖ୍ୟା, nr_levels);
	ଯଦି (!distances)
		ଫେରନ୍ତୁ -ENOMEM;

	ପାଇଁ (i = 0, j = 0; i < nr_levels; i++, j++) {
		j = find_next_bit(distance_map, NR_DISTANCE_ମୂଲ୍ୟଗୁଡ଼ିକ, j);
		distances[i] = j;
	}
	*dist = distances;
	*levels = nr_levels;

	ଫେରନ୍ତୁ 0;
}

ଶୂନ୍ୟ sched_init_numa(ପୂର୍ଣ୍ଣସଂଖ୍ୟା offline_ଗଣ୍ଠି)
{
	ସଂରଚନା sched_domain_topology_level *tl;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା nr_levels, nr_ଗଣ୍ଠି_levels;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା *distances, *domain_distances;
	ସଂରଚନା cpumask ***masks;

	/* Record the NUMA distances from SLIT table */
	ଯଦି (sched_record_numa_dist(offline_ଗଣ୍ଠି, numa_ଗଣ୍ଠି_dist, &distances,
				   &nr_ଗଣ୍ଠି_levels))
		ଫେରନ୍ତୁ;

	/* Record modified NUMA distances for building sched domains */
	ଯଦି (modified_sched_ଗଣ୍ଠି_distance()) {
		ଯଦି (sched_record_numa_dist(offline_ଗଣ୍ଠି, arch_sched_ଗଣ୍ଠି_distance,
					   &domain_distances, &nr_levels)) {
			kfree(distances);
			ଫେରନ୍ତୁ;
		}
	} ଅନ୍ୟଥା {
		domain_distances = distances;
		nr_levels = nr_ଗଣ୍ଠି_levels;
	}
	rcu_assign_pointer(sched_numa_ଗଣ୍ଠି_distance, distances);
	WRITE_ONCE(sched_max_numa_distance, distances[nr_ଗଣ୍ଠି_levels - 1]);
	WRITE_ONCE(sched_numa_ଗଣ୍ଠି_levels, nr_ଗଣ୍ଠି_levels);

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

	masks = kzalloc(ଆକାର(ଶୂନ୍ୟ *) * nr_levels, GFP_KERNEL);
	ଯଦି (!masks)
		ଫେରନ୍ତୁ;

	/*
	 * Now for each level, construct a mask per node which contains all
	 * CPUs of nodes that are that many hops away from us.
	 */
	ପାଇଁ (i = 0; i < nr_levels; i++) {
		masks[i] = kzalloc(nr_ଗଣ୍ଠି_ids * ଆକାର(ଶୂନ୍ୟ *), GFP_KERNEL);
		ଯଦି (!masks[i])
			ଫେରନ୍ତୁ;

		for_each_cpu_ଗଣ୍ଠି_but(j, offline_ଗଣ୍ଠି) {
			ସଂରଚନା cpumask *mask = kzalloc(cpumask_size(), GFP_KERNEL);
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା k;

			ଯଦି (!mask)
				ଫେରନ୍ତୁ;

			masks[i][j] = mask;

			for_each_cpu_ଗଣ୍ଠି_but(k, offline_ଗଣ୍ଠି) {
				ଯଦି (sched_debug() &&
				    (arch_sched_ଗଣ୍ଠି_distance(j, k) !=
				     arch_sched_ଗଣ୍ଠି_distance(k, j)))
					sched_numa_warn("Node-distance not symmetric");

				ଯଦି (arch_sched_ଗଣ୍ଠି_distance(j, k) >
				    sched_domains_numa_distance[i])
					ଜାରି;

				cpumask_or(mask, mask, cpumask_of_ଗଣ୍ଠି(k));
			}
		}
	}
	rcu_assign_pointer(sched_domains_numa_masks, masks);

	/* Compute default topology size */
	ପାଇଁ (i = 0; sched_domain_topology[i].mask; i++);

	tl = kzalloc((i + nr_levels + 1) *
			ଆକାର(ସଂରଚନା sched_domain_topology_level), GFP_KERNEL);
	ଯଦି (!tl)
		ଫେରନ୍ତୁ;

	/*
	 * Copy the default topology bits..
	 */
	ପାଇଁ (i = 0; sched_domain_topology[i].mask; i++)
		tl[i] = sched_domain_topology[i];

	/*
	 * Add the NUMA identity distance, aka single NODE.
	 */
	tl[i++] = SDTL_INIT(sd_numa_mask, ଖାଲି, ଗଣ୍ଠି_2);

	/*
	 * .. and append 'j' levels of NUMA goodness.
	 */
	ପାଇଁ (j = 1; j < nr_levels; i++, j++) {
		tl[i] = SDTL_INIT(sd_numa_mask, cpu_numa_flags, NUMA);
		tl[i].numa_level = j;
	}

	sched_domain_topology_saved = sched_domain_topology;
	sched_domain_topology = tl;

	sched_domains_numa_levels = nr_levels;

	init_numa_topology_type(offline_ଗଣ୍ଠି);
}


ସ୍ଥିର ଶୂନ୍ୟ sched_reset_numa(ଶୂନ୍ୟ)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା nr_levels, *distances, *dom_distances = ଖାଲି;
	ସଂରଚନା cpumask ***masks;

	nr_levels = sched_domains_numa_levels;
	sched_numa_ଗଣ୍ଠି_levels = 0;
	sched_domains_numa_levels = 0;
	sched_max_numa_distance = 0;
	sched_numa_topology_type = NUMA_DIRECT;
	distances = sched_numa_ଗଣ୍ଠି_distance;
	ଯଦି (sched_numa_ଗଣ୍ଠି_distance != sched_domains_numa_distance)
		dom_distances = sched_domains_numa_distance;
	rcu_assign_pointer(sched_numa_ଗଣ୍ଠି_distance, ଖାଲି);
	rcu_assign_pointer(sched_domains_numa_distance, ଖାଲି);
	masks = sched_domains_numa_masks;
	rcu_assign_pointer(sched_domains_numa_masks, ଖାଲି);
	ଯଦି (distances || masks) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j;

		synchronize_rcu();
		kfree(distances);
		kfree(dom_distances);
		ପାଇଁ (i = 0; i < nr_levels && masks; i++) {
			ଯଦି (!masks[i])
				ଜାରି;
			for_each_ଗଣ୍ଠି(j)
				kfree(masks[i][j]);
			kfree(masks[i]);
		}
		kfree(masks);
	}
	ଯଦି (sched_domain_topology_saved) {
		kfree(sched_domain_topology);
		sched_domain_topology = sched_domain_topology_saved;
		sched_domain_topology_saved = ଖାଲି;
	}
}

/*
 * Call with hotplug lock held
 */
ଶୂନ୍ୟ sched_update_numa(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu, bool online)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣ୍ଠି;

	ଗଣ୍ଠି = cpu_to_ଗଣ୍ଠି(cpu);
	/*
	 * Scheduler NUMA topology is updated when the first CPU of a
	 * node is onlined or the last CPU of a node is offlined.
	 */
	ଯଦି (cpumask_weight(cpumask_of_ଗଣ୍ଠି(ଗଣ୍ଠି)) != 1)
		ଫେରନ୍ତୁ;

	sched_reset_numa();
	sched_init_numa(online ? NUMA_NO_ଗଣ୍ଠି : ଗଣ୍ଠି);
}

ଶୂନ୍ୟ sched_domains_numa_masks_set(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣ୍ଠି = cpu_to_ଗଣ୍ଠି(cpu);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j;

	ପାଇଁ (i = 0; i < sched_domains_numa_levels; i++) {
		ପାଇଁ (j = 0; j < nr_ଗଣ୍ଠି_ids; j++) {
			ଯଦି (!ଗଣ୍ଠି_state(j, N_CPU))
				ଜାରି;

			/* Set ourselves in the remote node's masks */
			ଯଦି (arch_sched_ଗଣ୍ଠି_distance(j, ଗଣ୍ଠି) <=
			    sched_domains_numa_distance[i])
				cpumask_set_cpu(cpu, sched_domains_numa_masks[i][j]);
		}
	}
}

ଶୂନ୍ୟ sched_domains_numa_masks_clear(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j;

	ପାଇଁ (i = 0; i < sched_domains_numa_levels; i++) {
		ପାଇଁ (j = 0; j < nr_ଗଣ୍ଠି_ids; j++) {
			ଯଦି (sched_domains_numa_masks[i][j])
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
ପୂର୍ଣ୍ଣସଂଖ୍ୟା sched_numa_find_closest(ଅଚଳ ସଂରଚନା cpumask *cpus, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j = cpu_to_ଗଣ୍ଠି(cpu), found = nr_cpu_ids;
	ସଂରଚନା cpumask ***masks;

	rcu_read_lock();
	masks = rcu_dereference(sched_domains_numa_masks);
	ଯଦି (!masks)
		ଯାଅ unlock;
	ପାଇଁ (i = 0; i < sched_domains_numa_levels; i++) {
		ଯଦି (!masks[i][j])
			ଭାଙ୍ଗ;
		cpu = cpumask_any_and_distribute(cpus, masks[i][j]);
		ଯଦି (cpu < nr_cpu_ids) {
			found = cpu;
			ଭାଙ୍ଗ;
		}
	}
unlock:
	rcu_read_unlock();

	ଫେରନ୍ତୁ found;
}

ସଂରଚନା __cmp_key {
	ଅଚଳ ସଂରଚନା cpumask *cpus;
	ସଂରଚନା cpumask ***masks;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣ୍ଠି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା w;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା hop_cmp(ଅଚଳ ଶୂନ୍ୟ *a, ଅଚଳ ଶୂନ୍ୟ *b)
{
	ସଂରଚନା cpumask **prev_hop, **cur_hop = *(ସଂରଚନା cpumask ***)b;
	ସଂରଚନା __cmp_key *k = (ସଂରଚନା __cmp_key *)a;

	ଯଦି (cpumask_weight_and(k->cpus, cur_hop[k->ଗଣ୍ଠି]) <= k->cpu)
		ଫେରନ୍ତୁ 1;

	ଯଦି (b == k->masks) {
		k->w = 0;
		ଫେରନ୍ତୁ 0;
	}

	prev_hop = *((ସଂରଚନା cpumask ***)b - 1);
	k->w = cpumask_weight_and(k->cpus, prev_hop[k->ଗଣ୍ଠି]);
	ଯଦି (k->w <= k->cpu)
		ଫେରନ୍ତୁ 0;

	ଫେରନ୍ତୁ -1;
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
ପୂର୍ଣ୍ଣସଂଖ୍ୟା sched_numa_find_nth_cpu(ଅଚଳ ସଂରଚନା cpumask *cpus, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣ୍ଠି)
{
	ସଂରଚନା __cmp_key k = { .cpus = cpus, .cpu = cpu };
	ସଂରଚନା cpumask ***hop_masks;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା hop, ret = nr_cpu_ids;

	ଯଦି (ଗଣ୍ଠି == NUMA_NO_ଗଣ୍ଠି)
		ଫେରନ୍ତୁ cpumask_nth_and(cpu, cpus, cpu_online_mask);

	rcu_read_lock();

	/* CPU-less node entries are uninitialized in sched_domains_numa_masks */
	ଗଣ୍ଠି = numa_nearest_ଗଣ୍ଠି(ଗଣ୍ଠି, N_CPU);
	k.ଗଣ୍ଠି = ଗଣ୍ଠି;

	k.masks = rcu_dereference(sched_domains_numa_masks);
	ଯଦି (!k.masks)
		ଯାଅ unlock;

	hop_masks = bsearch(&k, k.masks, sched_domains_numa_levels, ଆକାର(k.masks[0]), hop_cmp);
	ଯଦି (!hop_masks)
		ଯାଅ unlock;
	hop = hop_masks	- k.masks;

	ret = hop ?
		cpumask_nth_and_andnot(cpu - k.w, cpus, k.masks[hop][ଗଣ୍ଠି], k.masks[hop-1][ଗଣ୍ଠି]) :
		cpumask_nth_and(cpu, cpus, k.masks[0][ଗଣ୍ଠି]);
unlock:
	rcu_read_unlock();
	ଫେରନ୍ତୁ ret;
}
EXPORT_ପ୍ରତୀକ_GPL(sched_numa_find_nth_cpu);

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
ଅଚଳ ସଂରଚନା cpumask *sched_numa_hop_mask(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣ୍ଠି, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା hops)
{
	ସଂରଚନା cpumask ***masks;

	ଯଦି (ଗଣ୍ଠି >= nr_ଗଣ୍ଠି_ids || hops >= sched_domains_numa_levels)
		ଫେରନ୍ତୁ ERR_PTR(-EINVAL);

	masks = rcu_dereference(sched_domains_numa_masks);
	ଯଦି (!masks)
		ଫେରନ୍ତୁ ERR_PTR(-EBUSY);

	ଫେରନ୍ତୁ masks[hops][ଗଣ୍ଠି];
}
EXPORT_ପ୍ରତୀକ_GPL(sched_numa_hop_mask);

#endif /* CONFIG_NUMA */

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା __sdt_alloc(ଅଚଳ ସଂରଚନା cpumask *cpu_map)
{
	ସଂରଚନା sched_domain_topology_level *tl;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା j;

	for_each_sd_topology(tl) {
		ସଂରଚନା sd_data *sdd = &tl->data;

		sdd->sd = alloc_percpu(ସଂରଚନା sched_domain *);
		ଯଦି (!sdd->sd)
			ଫେରନ୍ତୁ -ENOMEM;

		sdd->sg = alloc_percpu(ସଂରଚନା sched_group *);
		ଯଦି (!sdd->sg)
			ଫେରନ୍ତୁ -ENOMEM;

		sdd->sgc = alloc_percpu(ସଂରଚନା sched_group_capacity *);
		ଯଦି (!sdd->sgc)
			ଫେରନ୍ତୁ -ENOMEM;

		for_each_cpu(j, cpu_map) {
			ସଂରଚନା sched_domain *sd;
			ସଂରଚନା sched_group *sg;
			ସଂରଚନା sched_group_capacity *sgc;

			sd = kzalloc_ଗଣ୍ଠି(ଆକାର(ସଂରଚନା sched_domain) + cpumask_size(),
					GFP_KERNEL, cpu_to_ଗଣ୍ଠି(j));
			ଯଦି (!sd)
				ଫେରନ୍ତୁ -ENOMEM;

			*per_cpu_ptr(sdd->sd, j) = sd;

			sg = kzalloc_ଗଣ୍ଠି(ଆକାର(ସଂରଚନା sched_group) + cpumask_size(),
					GFP_KERNEL, cpu_to_ଗଣ୍ଠି(j));
			ଯଦି (!sg)
				ଫେରନ୍ତୁ -ENOMEM;

			sg->next = sg;

			*per_cpu_ptr(sdd->sg, j) = sg;

			sgc = kzalloc_ଗଣ୍ଠି(ଆକାର(ସଂରଚନା sched_group_capacity) + cpumask_size(),
					GFP_KERNEL, cpu_to_ଗଣ୍ଠି(j));
			ଯଦି (!sgc)
				ଫେରନ୍ତୁ -ENOMEM;

			sgc->id = j;

			*per_cpu_ptr(sdd->sgc, j) = sgc;
		}
	}

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ __sdt_free(ଅଚଳ ସଂରଚନା cpumask *cpu_map)
{
	ସଂରଚନା sched_domain_topology_level *tl;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା j;

	for_each_sd_topology(tl) {
		ସଂରଚନା sd_data *sdd = &tl->data;

		for_each_cpu(j, cpu_map) {
			ସଂରଚନା sched_domain *sd;

			ଯଦି (sdd->sd) {
				sd = *per_cpu_ptr(sdd->sd, j);
				ଯଦି (sd && (sd->flags & SD_NUMA))
					free_sched_groups(sd->groups, 0);
				kfree(*per_cpu_ptr(sdd->sd, j));
			}

			ଯଦି (sdd->sg)
				kfree(*per_cpu_ptr(sdd->sg, j));
			ଯଦି (sdd->sgc)
				kfree(*per_cpu_ptr(sdd->sgc, j));
		}
		free_percpu(sdd->sd);
		sdd->sd = ଖାଲି;
		free_percpu(sdd->sg);
		sdd->sg = ଖାଲି;
		free_percpu(sdd->sgc);
		sdd->sgc = ଖାଲି;
	}
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା __sds_alloc(ସଂରଚନା s_data *d, ଅଚଳ ସଂରଚନା cpumask *cpu_map)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା j;

	d->sds = alloc_percpu(ସଂରଚନା sched_domain_shared *);
	ଯଦି (!d->sds)
		ଫେରନ୍ତୁ -ENOMEM;

	for_each_cpu(j, cpu_map) {
		ସଂରଚନା sched_domain_shared *sds;

		sds = kzalloc_ଗଣ୍ଠି(ଆକାର(ସଂରଚନା sched_domain_shared),
				GFP_KERNEL, cpu_to_ଗଣ୍ଠି(j));
		ଯଦି (!sds)
			ଫେରନ୍ତୁ -ENOMEM;

		*per_cpu_ptr(d->sds, j) = sds;
	}

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ __sds_free(ସଂରଚନା s_data *d, ଅଚଳ ସଂରଚନା cpumask *cpu_map)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା j;

	ଯଦି (!d->sds)
		ଫେରନ୍ତୁ;

	for_each_cpu(j, cpu_map)
		kfree(*per_cpu_ptr(d->sds, j));

	free_percpu(d->sds);
	d->sds = ଖାଲି;
}

ସ୍ଥିର ସଂରଚନା sched_domain *build_sched_domain(ସଂରଚନା sched_domain_topology_level *tl,
		ଅଚଳ ସଂରଚନା cpumask *cpu_map, ସଂରଚନା sched_domain_attr *attr,
		ସଂରଚନା sched_domain *child, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ସଂରଚନା sched_domain *sd = sd_init(tl, cpu_map, child, cpu);

	ଯଦି (child) {
		sd->level = child->level + 1;
		sched_domain_level_max = max(sched_domain_level_max, sd->level);
		child->parent = sd;

		ଯଦି (!cpumask_subset(sched_domain_span(child),
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

	ଫେରନ୍ତୁ sd;
}

/*
 * Ensure topology masks are sane, i.e. there are no conflicts (overlaps) for
 * any two given CPUs on non-NUMA topology levels.
 */
ସ୍ଥିର bool topology_span_sane(ଅଚଳ ସଂରଚନା cpumask *cpu_map)
{
	ସଂରଚନା sched_domain_topology_level *tl;
	ସଂରଚନା cpumask *covered, *id_seen;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu;

	lockdep_assert_held(&sched_domains_mutex);
	covered = sched_domains_tmpmask;
	id_seen = sched_domains_tmpmask2;

	for_each_sd_topology(tl) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା tl_common_flags = 0;

		ଯଦି (tl->sd_flags)
			tl_common_flags = (*tl->sd_flags)();

		/* NUMA levels are allowed to overlap */
		ଯଦି (tl_common_flags & SD_NUMA)
			ଜାରି;

		cpumask_clear(covered);
		cpumask_clear(id_seen);

		/*
		 * Non-NUMA levels cannot partially overlap - they must be either
		 * completely equal or completely disjoint. Otherwise we can end up
		 * breaking the sched_group lists - i.e. a later get_group() pass
		 * breaks the linking done for an earlier span.
		 */
		for_each_cpu(cpu, cpu_map) {
			ଅଚଳ ସଂରଚନା cpumask *tl_cpu_mask = tl->mask(tl, cpu);
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା id;

			/* lowest bit set in this mask is used as a unique id */
			id = cpumask_first(tl_cpu_mask);

			ଯଦି (cpumask_test_cpu(id, id_seen)) {
				/* First CPU has already been seen, ensure identical spans */
				ଯଦି (!cpumask_equal(tl->mask(tl, id), tl_cpu_mask))
					ଫେରନ୍ତୁ false;
			} ଅନ୍ୟଥା {
				/* First CPU hasn't been seen before, ensure it's a completely new span */
				ଯଦି (cpumask_intersects(tl_cpu_mask, covered))
					ଫେରନ୍ତୁ false;

				cpumask_or(covered, covered, tl_cpu_mask);
				cpumask_set_cpu(id, id_seen);
			}
		}
	}
	ଫେରନ୍ତୁ true;
}

/*
 * Calculate an allowed NUMA imbalance such that LLCs do not get
 * imbalanced.
 */
ସ୍ଥିର ଶୂନ୍ୟ adjust_numa_imbalance(ସଂରଚନା sched_domain *sd_llc)
{
	ସଂରଚନା sched_domain *parent;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା imb_span = 1;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା imb = 0;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା nr_llcs;

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
	ଯଦି (nr_llcs == 1)
		imb = sd_llc->parent->span_weight >> 3;
	ଅନ୍ୟଥା
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
	ଯେତେବେଳେ (parent && !(parent->flags & SD_NUMA))
		parent = parent->parent;

	imb_span = parent ? parent->span_weight : sd_llc->parent->span_weight;

	/* Update the upper remainder of the topology */
	parent = sd_llc->parent;
	ଯେତେବେଳେ (parent) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା factor = max(1U, (parent->span_weight / imb_span));

		parent->imb_numa_nr = imb * factor;
		parent = parent->parent;
	}
}

ସ୍ଥିର ଶୂନ୍ୟ
init_sched_domain_shared(ସଂରଚନା s_data *d, ସଂରଚନା sched_domain *sd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ସଂରଚନା sched_domain_shared *sds = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu;

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

		ଯଦି (!sds->alloc_flags ||
		    sd->span_weight == 1 ||
		    sds->alloc_flags == flags) {
			sds->alloc_flags = flags;
			sd->shared = sds;
			ଭାଙ୍ଗ;
		}
	}

	/*
	 * Use the sd_shared corresponding to the last
	 * CPU in the span if none are avaialable.
	 */
	ଯଦି (WARN_ON_ONCE(!sd->shared))
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
ସ୍ଥିର bool claim_asym_sched_domain_shared(ସଂରଚନା s_data *d, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ସଂରଚନା sched_domain *sd = *per_cpu_ptr(d->sd, cpu);
	ସଂରଚନା sched_domain *sd_asym;

	ଯଦି (!sd)
		ଫେରନ୍ତୁ false;

	sd_asym = sd;
	ଯେତେବେଳେ (sd_asym && !(sd_asym->flags & SD_ASYM_CPUCAPACITY_FULL))
		sd_asym = sd_asym->parent;

	ଯଦି (!sd_asym || (sd_asym->flags & SD_NUMA))
		ଫେରନ୍ତୁ false;

	init_sched_domain_shared(d, sd_asym, SD_ASYM_CPUCAPACITY);
	ଫେରନ୍ତୁ true;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା __sched_domains_alloc_llc_id(ଶୂନ୍ୟ)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା lid, max;

	lockdep_assert_held(&sched_domains_mutex);

	lid = cpumask_first_zero(sched_domains_llc_id_allocmask);
	/*
	 * llc_id space should never grow larger than the
	 * possible number of CPUs in the system.
	 */
	ଯଦି (lid >= nr_cpu_ids)
		ଫେରନ୍ତୁ -1;

	__cpumask_set_cpu(lid, sched_domains_llc_id_allocmask);
	max = cpumask_last(sched_domains_llc_id_allocmask);
	ଯଦି (max > max_lid)
		max_lid = max;

	ଫେରନ୍ତୁ lid;
}

ସ୍ଥିର ଶୂନ୍ୟ __sched_domains_free_llc_id(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, lid, max;

	lockdep_assert_held(&sched_domains_mutex);

	lid = per_cpu(sd_llc_id, cpu);
	ଯଦି (lid == -1 || lid >= nr_cpu_ids)
		ଫେରନ୍ତୁ;

	per_cpu(sd_llc_id, cpu) = -1;

	for_each_cpu(i, llc_mask(cpu)) {
		/* An online CPU owns the llc_id. */
		ଯଦି (per_cpu(sd_llc_id, i) == lid)
			ଫେରନ୍ତୁ;
	}

	__cpumask_clear_cpu(lid, sched_domains_llc_id_allocmask);

	max = cpumask_last(sched_domains_llc_id_allocmask);
	/* shrink max lid to save memory */
	ଯଦି (max < max_lid)
		max_lid = max;
}

ଶୂନ୍ୟ sched_domains_free_llc_id(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	sched_domains_mutex_lock();
	__sched_domains_free_llc_id(cpu);
	sched_domains_mutex_unlock();
}

/*
 * Build sched domains for a given set of CPUs and attach the sched domains
 * to the individual CPUs
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
build_sched_domains(ଅଚଳ ସଂରଚନା cpumask *cpu_map, ସଂରଚନା sched_domain_attr *attr,
		    bool *multi_llcs)
{
	ଗଣନା s_alloc alloc_state = sa_none;
	bool has_multi_llcs = false;
	ସଂରଚନା sched_domain *sd;
	ସଂରଚନା s_data d;
	ସଂରଚନା rq *rq = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, ret = -ENOMEM;
	bool has_asym = false;
	bool has_cluster = false;

	ଯଦି (WARN_ON(cpumask_empty(cpu_map)))
		ଯାଅ error;

	alloc_state = __visit_domain_allocation_hell(&d, cpu_map);
	ଯଦି (alloc_state != sa_rootdomain)
		ଯାଅ error;

	/* Set up domains for CPUs specified by the cpu_map: */
	for_each_cpu(i, cpu_map) {
		ସଂରଚନା sched_domain_topology_level *tl;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା lid;

		sd = ଖାଲି;
		for_each_sd_topology(tl) {

			sd = build_sched_domain(tl, cpu_map, attr, sd, i);

			has_asym |= sd->flags & SD_ASYM_CPUCAPACITY;

			ଯଦି (tl == sched_domain_topology)
				*per_cpu_ptr(d.sd, i) = sd;
			ଯଦି (cpumask_equal(cpu_map, sched_domain_span(sd)))
				ଭାଙ୍ଗ;
		}

		lid = per_cpu(sd_llc_id, i);
		ଯଦି (lid == -1) {
			/* try to reuse the llc_id of its siblings */
			ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = cpumask_first(llc_mask(i));
			     j < nr_cpu_ids;
			     j = cpumask_next(j, llc_mask(i))) {
				ଯଦି (i == j)
					ଜାରି;

				lid = per_cpu(sd_llc_id, j);

				ଯଦି (lid != -1) {
					per_cpu(sd_llc_id, i) = lid;

					ଭାଙ୍ଗ;
				}
			}

			/* a new LLC is detected */
			ଯଦି (lid == -1)
				per_cpu(sd_llc_id, i) = __sched_domains_alloc_llc_id();
		}
	}

	ଯଦି (WARN_ON(!topology_span_sane(cpu_map)))
		ଯାଅ error;

	/* Build the groups for the domains */
	for_each_cpu(i, cpu_map) {
		ପାଇଁ (sd = *per_cpu_ptr(d.sd, i); sd; sd = sd->parent) {
			sd->span_weight = cpumask_weight(sched_domain_span(sd));
			ଯଦି (sd->flags & SD_NUMA) {
				ଯଦି (build_overlap_sched_groups(sd, i))
					ଯାଅ error;
			} ଅନ୍ୟଥା {
				ଯଦି (build_sched_groups(sd, i))
					ଯାଅ error;
			}
		}
	}

	for_each_cpu(i, cpu_map) {
		sd = *per_cpu_ptr(d.sd, i);
		ଯଦି (!sd)
			ଜାରି;

		ଯଦି (has_asym)
			claim_asym_sched_domain_shared(&d, i);

		/* First, find the topmost SD_SHARE_LLC domain */
		ଯେତେବେଳେ (sd->parent && (sd->parent->flags & SD_SHARE_LLC))
			sd = sd->parent;

		ଯଦି (sd->flags & SD_SHARE_LLC) {
			init_sched_domain_shared(&d, sd, SD_SHARE_LLC);

			/*
			 * In presence of higher domains, adjust the
			 * NUMA imbalance stats for the hierarchy.
			 */
			ଯଦି (sd->parent) {
				ଯଦି (IS_ENABLED(CONFIG_NUMA))
					adjust_numa_imbalance(sd);

				ଯଦି (sd_in_multi_llcs(sd))
					has_multi_llcs = true;
			}
		}
	}

	/* Calculate CPU capacity for physical packages and nodes */
	ପାଇଁ (i = nr_cpumask_bits-1; i >= 0; i--) {
		ଯଦି (!cpumask_test_cpu(i, cpu_map))
			ଜାରି;

		claim_allocations(i, &d);

		ପାଇଁ (sd = *per_cpu_ptr(d.sd, i); sd; sd = sd->parent)
			init_sched_groups_capacity(i, sd);
	}

	alloc_sd_llc(cpu_map, &d);

	/* Attach the domains */
	rcu_read_lock();
	for_each_cpu(i, cpu_map) {
		rq = cpu_rq(i);
		sd = *per_cpu_ptr(d.sd, i);

		cpu_attach_domain(sd, d.rd, i);

		ଯଦି (lowest_flag_domain(i, SD_CLUSTER))
			has_cluster = true;
	}
	rcu_read_unlock();

	ଯଦି (has_asym)
		static_branch_inc_cpuslocked(&sched_asym_cpucapacity);

	ଯଦି (has_cluster)
		static_branch_inc_cpuslocked(&sched_cluster_active);

	ଯଦି (rq && sched_debug_verbose)
		pr_info("root domain span: %*pbl\n", cpumask_pr_args(cpu_map));

	ret = 0;
error:
	*multi_llcs = has_multi_llcs;
	__free_domain_allocs(&d, alloc_state, cpu_map);

	ଫେରନ୍ତୁ ret;
}

/* Current sched domains: */
ସ୍ଥିର cpumask_var_t			*doms_cur;

/* Number of sched domains in 'doms_cur': */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା				ndoms_cur;

/* Attributes of custom domains in 'doms_cur' */
ସ୍ଥିର ସଂରଚନା sched_domain_attr		*dattr_cur;

/*
 * Special case: If a kmalloc() of a doms_cur partition (array of
 * cpumask) fails, then fallback to a single sched domain,
 * as determined by the single cpumask fallback_doms.
 */
ସ୍ଥିର cpumask_var_t			fallback_doms;

/*
 * arch_update_cpu_topology lets virtualized architectures update the
 * CPU core maps. It is supposed to return 1 if the topology changed
 * or 0 if it stayed the same.
 */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା __weak arch_update_cpu_topology(ଶୂନ୍ୟ)
{
	ଫେରନ୍ତୁ 0;
}

cpumask_var_t *alloc_sched_domains(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ndoms)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	cpumask_var_t *doms;

	doms = kmalloc_objs(*doms, ndoms);
	ଯଦି (!doms)
		ଫେରନ୍ତୁ ଖାଲି;
	ପାଇଁ (i = 0; i < ndoms; i++) {
		ଯଦି (!alloc_cpumask_var(&doms[i], GFP_KERNEL)) {
			free_sched_domains(doms, i);
			ଫେରନ୍ତୁ ଖାଲି;
		}
	}
	ଫେରନ୍ତୁ doms;
}

ଶୂନ୍ୟ free_sched_domains(cpumask_var_t doms[], unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ndoms)
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ପାଇଁ (i = 0; i < ndoms; i++)
		free_cpumask_var(doms[i]);
	kfree(doms);
}

/*
 * Set up scheduler domains and groups.  For now this just excludes isolated
 * CPUs, but could be used to exclude other special cases in the future.
 */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା __init sched_init_domains(ଅଚଳ ସଂରଚନା cpumask *cpu_map)
{
	bool multi_llcs;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା err;

	zalloc_cpumask_var(&sched_domains_llc_id_allocmask, GFP_KERNEL);
	zalloc_cpumask_var(&sched_domains_tmpmask, GFP_KERNEL);
	zalloc_cpumask_var(&sched_domains_tmpmask2, GFP_KERNEL);
	zalloc_cpumask_var(&fallback_doms, GFP_KERNEL);

	arch_update_cpu_topology();
	asym_cpu_capacity_scan();
	ndoms_cur = 1;
	doms_cur = alloc_sched_domains(ndoms_cur);
	ଯଦି (!doms_cur)
		doms_cur = &fallback_doms;
	cpumask_and(doms_cur[0], cpu_map, housekeeping_cpumask(HK_TYPE_DOMAIN));
	err = build_sched_domains(doms_cur[0], ଖାଲି, &multi_llcs);
	ଯଦି (!err)
		sched_cache_set(multi_llcs);

	ଫେରନ୍ତୁ err;
}

/*
 * Detach sched domains from a group of CPUs specified in cpu_map
 * These CPUs will now be attached to the NULL domain
 */
ସ୍ଥିର ଶୂନ୍ୟ detach_destroy_domains(ଅଚଳ ସଂରଚନା cpumask *cpu_map)
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu = cpumask_any(cpu_map);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	ଯଦି (rcu_access_pointer(per_cpu(sd_asym_cpucapacity, cpu)))
		static_branch_dec_cpuslocked(&sched_asym_cpucapacity);

	ଯଦି (static_branch_unlikely(&sched_cluster_active))
		static_branch_dec_cpuslocked(&sched_cluster_active);

	rcu_read_lock();
	for_each_cpu(i, cpu_map)
		cpu_attach_domain(ଖାଲି, &def_ମୂଳ_domain, i);
	rcu_read_unlock();
}

/* handle null as "default" */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା dattrs_equal(ସଂରଚନା sched_domain_attr *cur, ପୂର୍ଣ୍ଣସଂଖ୍ୟା idx_cur,
			ସଂରଚନା sched_domain_attr *new, ପୂର୍ଣ୍ଣସଂଖ୍ୟା idx_new)
{
	ସଂରଚନା sched_domain_attr tmp;

	/* Fast path: */
	ଯଦି (!new && !cur)
		ଫେରନ୍ତୁ 1;

	tmp = SD_ATTR_INIT;

	ଫେରନ୍ତୁ !memcmp(cur ? (cur + idx_cur) : &tmp,
			new ? (new + idx_new) : &tmp,
			ଆକାର(ସଂରଚନା sched_domain_attr));
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
ସ୍ଥିର ଶୂନ୍ୟ partition_sched_domains_locked(ପୂର୍ଣ୍ଣସଂଖ୍ୟା ndoms_new, cpumask_var_t doms_new[],
				    ସଂରଚନା sched_domain_attr *dattr_new)
{
	bool __maybe_unused has_eas = false;
	bool has_multi_llcs = false, multi_llcs;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j, n;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_topology;

	lockdep_assert_held(&sched_domains_mutex);

	/* Let the architecture update CPU core mappings: */
	new_topology = arch_update_cpu_topology();
	/* Trigger rebuilding CPU capacity asymmetry data */
	ଯଦି (new_topology)
		asym_cpu_capacity_scan();

	ଯଦି (!doms_new) {
		WARN_ON_ONCE(dattr_new);
		n = 0;
		doms_new = alloc_sched_domains(1);
		ଯଦି (doms_new) {
			n = 1;
			cpumask_and(doms_new[0], cpu_active_mask,
				    housekeeping_cpumask(HK_TYPE_DOMAIN));
		}
	} ଅନ୍ୟଥା {
		n = ndoms_new;
	}

	/* Destroy deleted domains: */
	ପାଇଁ (i = 0; i < ndoms_cur; i++) {
		ପାଇଁ (j = 0; j < n && !new_topology; j++) {
			ଯଦି (cpumask_equal(doms_cur[i], doms_new[j]) &&
			    dattrs_equal(dattr_cur, i, dattr_new, j))
				ଯାଅ match1;
		}
		/* No match - a current sched domain not in new doms_new[] */
		detach_destroy_domains(doms_cur[i]);
match1:
		;
	}

	n = ndoms_cur;
	ଯଦି (!doms_new) {
		n = 0;
		doms_new = &fallback_doms;
		cpumask_and(doms_new[0], cpu_active_mask,
			    housekeeping_cpumask(HK_TYPE_DOMAIN));
	}

	/* Build new domains: */
	ପାଇଁ (i = 0; i < ndoms_new; i++) {
		ପାଇଁ (j = 0; j < n && !new_topology; j++) {
			ଯଦି (cpumask_equal(doms_new[i], doms_cur[j]) &&
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
				ସଂରଚନା sched_domain *sd;
				ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu = cpumask_first(doms_cur[j]);

				guard(rcu)();
				sd = rcu_dereference(cpu_rq(cpu)->sd);
				ଯେତେବେଳେ (sd && sd->parent && (sd->parent->flags & SD_SHARE_LLC))
					sd = sd->parent;
				ଯଦି (sd && (sd->flags & SD_SHARE_LLC) && sd->parent &&
				    sd_in_multi_llcs(sd))
					has_multi_llcs = true;
				ଯାଅ match2;
			}
		}
		/* No match - add a new doms_new */
		build_sched_domains(doms_new[i], dattr_new ? dattr_new + i : ଖାଲି,
				    &multi_llcs);
		has_multi_llcs |= multi_llcs;
match2:
		;
	}
	sched_cache_set(has_multi_llcs);

#ଯଦି defined(CONFIG_ENERGY_MODEL) && defined(CONFIG_CPU_FREQ_GOV_SCHEDUTIL)
	/* Build perf domains: */
	ପାଇଁ (i = 0; i < ndoms_new; i++) {
		ପାଇଁ (j = 0; j < n && !sched_energy_update; j++) {
			ଯଦି (cpumask_equal(doms_new[i], doms_cur[j]) &&
			    cpu_rq(cpumask_first(doms_cur[j]))->rd->pd) {
				has_eas = true;
				ଯାଅ match3;
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
	ଯଦି (doms_cur != &fallback_doms)
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
ଶୂନ୍ୟ partition_sched_domains(ପୂର୍ଣ୍ଣସଂଖ୍ୟା ndoms_new, cpumask_var_t doms_new[],
			     ସଂରଚନା sched_domain_attr *dattr_new)
{
	sched_domains_mutex_lock();
	partition_sched_domains_locked(ndoms_new, doms_new, dattr_new);
	sched_domains_mutex_unlock();
}
