// SPDX-License-Identifier: GPL-2.0
/*
 * Scheduler topology setup/handling methods
 */

#అన్తర్భావ <linux/sched/isolation.h>
#అన్తర్భావ <linux/sched/clock.h>
#అన్తర్భావ <linux/bsearch.h>
#అన్తర్భావ "sched.h"

DEFINE_MUTEX(sched_domains_mutex);
శూన్య sched_domains_mutex_తాళం(శూన్య)
{
	mutex_తాళం(&sched_domains_mutex);
}
శూన్య sched_domains_mutex_unlock(శూన్య)
{
	mutex_unlock(&sched_domains_mutex);
}

/* Protected by sched_domains_mutex: */
స్థిర cpumask_var_t sched_domains_llc_id_allocmask;
స్థిర cpumask_var_t sched_domains_tmpmask;
స్థిర cpumask_var_t sched_domains_tmpmask2;
పూర్ణాంకం max_lid;

స్థిర పూర్ణాంకం __init sched_debug_అమరిక(అక్షరం *str)
{
	sched_debug_verbose = true;

	తిరిగి 0;
}
early_param("sched_verbose", sched_debug_అమరిక);

#నిర్వచితమైతే CONFIG_SCHED_SMT
స్థిర bool sched_smt_asym_packing __read_mostly;

స్థిర పూర్ణాంకం __init అమరిక_sched_smt_asym_packing(అక్షరం *str)
{
	ఒకవేళ (strcmp(str, "on"))
		తిరిగి 0;

	sched_smt_asym_packing = true;
	తిరిగి 1;
}
__setup("sched_smt_asym_packing=", అమరిక_sched_smt_asym_packing);
#అయితే_ముగింపు

స్థిర అంతఃవరుస bool sched_debug(శూన్య)
{
	తిరిగి sched_debug_verbose;
}

#నిర్వచించు SD_FLAG(_name, mflags) [__##_name] = { .meta_flags = mflags, .name = #_name },
స్థిరరాశి నిర్మాణం sd_flag_debug sd_flag_debug[] = {
#అన్తర్భావ <linux/sched/sd_flags.h>
};
#నిర్వచనం_రద్దు SD_FLAG

స్థిర పూర్ణాంకం sched_domain_debug_one(నిర్మాణం sched_domain *sd, పూర్ణాంకం cpu, పూర్ణాంకం level,
				  నిర్మాణం cpumask *groupmask)
{
	నిర్మాణం sched_సమూహం *సమూహం = sd->groups;
	గుర్తులేని దీర్ఘ flags = sd->flags;
	గుర్తులేని పూర్ణాంకం idx;

	cpumask_clear(groupmask);

	printk(KERN_DEBUG "%*s domain-%d: ", level, "", level);
	printk(KERN_CONT "span=%*pbl level=%s\n",
	       cpumask_pr_వాదనలు(sched_domain_పరిధి(sd)), sd->name);

	ఒకవేళ (!cpumask_test_cpu(cpu, sched_domain_పరిధి(sd))) {
		printk(KERN_ERR "ERROR: domain->span does not contain CPU%d\n", cpu);
	}
	ఒకవేళ (సమూహం && !cpumask_test_cpu(cpu, sched_సమూహం_పరిధి(సమూహం))) {
		printk(KERN_ERR "ERROR: domain->groups does not contain CPU%d\n", cpu);
	}

	for_each_సమితి_బిట్టు(idx, &flags, __SD_FLAG_CNT) {
		గుర్తులేని పూర్ణాంకం flag = బిట్టు(idx);
		గుర్తులేని పూర్ణాంకం meta_flags = sd_flag_debug[idx].meta_flags;

		ఒకవేళ ((meta_flags & SDF_SHARED_బిడ్డ) && sd->బిడ్డ &&
		    !(sd->బిడ్డ->flags & flag))
			printk(KERN_ERR "ERROR: flag %s set here but not in child\n",
			       sd_flag_debug[idx].name);

		ఒకవేళ ((meta_flags & SDF_SHARED_మాతృ) && sd->మాతృ &&
		    !(sd->మాతృ->flags & flag))
			printk(KERN_ERR "ERROR: flag %s set here but not in parent\n",
			       sd_flag_debug[idx].name);
	}

	printk(KERN_DEBUG "%*s groups:", level + 1, "");
	చేయి {
		ఒకవేళ (!సమూహం) {
			printk("\n");
			printk(KERN_ERR "ERROR: group is NULL\n");
			ఆపు;
		}

		ఒకవేళ (cpumask_ఖాళీలు(sched_సమూహం_పరిధి(సమూహం))) {
			printk(KERN_CONT "\n");
			printk(KERN_ERR "ERROR: empty group\n");
			ఆపు;
		}

		ఒకవేళ (!(sd->flags & SD_NUMA) &&
		    cpumask_intersects(groupmask, sched_సమూహం_పరిధి(సమూహం))) {
			printk(KERN_CONT "\n");
			printk(KERN_ERR "ERROR: repeated CPUs\n");
			ఆపు;
		}

		cpumask_or(groupmask, groupmask, sched_సమూహం_పరిధి(సమూహం));

		printk(KERN_CONT " %d:{ span=%*pbl",
				సమూహం->sgc->id,
				cpumask_pr_వాదనలు(sched_సమూహం_పరిధి(సమూహం)));

		ఒకవేళ ((sd->flags & SD_NUMA) &&
		    !cpumask_equal(సమూహం_balance_ముసుగు(సమూహం), sched_సమూహం_పరిధి(సమూహం))) {
			printk(KERN_CONT " mask=%*pbl",
				cpumask_pr_వాదనలు(సమూహం_balance_ముసుగు(సమూహం)));
		}

		ఒకవేళ (సమూహం->sgc->సామర్థ్యం != SCHED_సామర్థ్యం_SCALE)
			printk(KERN_CONT " cap=%lu", సమూహం->sgc->సామర్థ్యం);

		ఒకవేళ (సమూహం == sd->groups && sd->బిడ్డ &&
		    !cpumask_equal(sched_domain_పరిధి(sd->బిడ్డ),
				   sched_సమూహం_పరిధి(సమూహం))) {
			printk(KERN_ERR "ERROR: domain->groups does not match domain->child\n");
		}

		printk(KERN_CONT " }");

		సమూహం = సమూహం->తదుపరి;

		ఒకవేళ (సమూహం != sd->groups)
			printk(KERN_CONT ",");

	} అయితే (సమూహం != sd->groups);
	printk(KERN_CONT "\n");

	ఒకవేళ (!cpumask_equal(sched_domain_పరిధి(sd), groupmask))
		printk(KERN_ERR "ERROR: groups don't span domain->span\n");

	ఒకవేళ (sd->మాతృ &&
	    !cpumask_subset(groupmask, sched_domain_పరిధి(sd->మాతృ)))
		printk(KERN_ERR "ERROR: parent span is not a superset of domain->span\n");
	తిరిగి 0;
}

స్థిర శూన్య sched_domain_debug(నిర్మాణం sched_domain *sd, పూర్ణాంకం cpu)
{
	పూర్ణాంకం level = 0;

	ఒకవేళ (!sched_debug_verbose)
		తిరిగి;

	ఒకవేళ (!sd) {
		printk(KERN_DEBUG "CPU%d attaching NULL sched-domain.\n", cpu);
		తిరిగి;
	}

	printk(KERN_DEBUG "CPU%d attaching sched-domain(s):\n", cpu);

	కొరకు (;;) {
		ఒకవేళ (sched_domain_debug_one(sd, cpu, level, sched_domains_tmpmask))
			ఆపు;
		level++;
		sd = sd->మాతృ;
		ఒకవేళ (!sd)
			ఆపు;
	}
}

/* Generate a mask of SD flags with the SDF_NEEDS_GROUPS metaflag */
#నిర్వచించు SD_FLAG(name, mflags) (name * !!((mflags) & SDF_NEEDS_GROUPS)) |
స్థిర స్థిరరాశి గుర్తులేని పూర్ణాంకం SD_DEGENERATE_GROUPS_ముసుగు =
#అన్తర్భావ <linux/sched/sd_flags.h>
0;
#నిర్వచనం_రద్దు SD_FLAG

స్థిర పూర్ణాంకం sd_degenerate(నిర్మాణం sched_domain *sd)
{
	ఒకవేళ (cpumask_బరువు(sched_domain_పరిధి(sd)) == 1)
		తిరిగి 1;

	/* Following flags need at least 2 groups */
	ఒకవేళ ((sd->flags & SD_DEGENERATE_GROUPS_ముసుగు) &&
	    (sd->groups != sd->groups->తదుపరి))
		తిరిగి 0;

	/* Following flags don't use groups */
	ఒకవేళ (sd->flags & (SD_WAKE_AFFINE))
		తిరిగి 0;

	తిరిగి 1;
}

స్థిర పూర్ణాంకం
sd_మాతృ_degenerate(నిర్మాణం sched_domain *sd, నిర్మాణం sched_domain *మాతృ)
{
	గుర్తులేని దీర్ఘ cflags = sd->flags, pflags = మాతృ->flags;

	ఒకవేళ (sd_degenerate(మాతృ))
		తిరిగి 1;

	ఒకవేళ (!cpumask_equal(sched_domain_పరిధి(sd), sched_domain_పరిధి(మాతృ)))
		తిరిగి 0;

	/* Flags needing groups don't count if only 1 group in parent */
	ఒకవేళ (మాతృ->groups == మాతృ->groups->తదుపరి)
		pflags &= ~SD_DEGENERATE_GROUPS_ముసుగు;

	ఒకవేళ (~cflags & pflags)
		తిరిగి 0;

	తిరిగి 1;
}

#ఒకవేళ defined(CONFIG_ENERGY_MODEL) && defined(CONFIG_CPU_FREQ_GOV_SCHEDUTIL)
DEFINE_STATIC_కీలకం_FALSE(sched_energy_present);
స్థిర గుర్తులేని పూర్ణాంకం sysctl_sched_energy_aware = 1;
స్థిర DEFINE_MUTEX(sched_energy_mutex);
స్థిర bool sched_energy_update;

స్థిర bool sched_is_eas_possible(స్థిరరాశి నిర్మాణం cpumask *cpu_ముసుగు)
{
	bool any_asym_సామర్థ్యం = false;
	పూర్ణాంకం i;

	/* EAS is enabled for asymmetric CPU capacity topologies. */
	for_each_cpu(i, cpu_ముసుగు) {
		ఒకవేళ (rcu_access_సూచిక(చొప్పున_cpu(sd_asym_cpucapacity, i))) {
			any_asym_సామర్థ్యం = true;
			ఆపు;
		}
	}
	ఒకవేళ (!any_asym_సామర్థ్యం) {
		ఒకవేళ (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS, CPUs do not have asymmetric capacities\n",
				cpumask_pr_వాదనలు(cpu_ముసుగు));
		}
		తిరిగి false;
	}

	/* EAS definitely does *not* handle SMT */
	ఒకవేళ (sched_smt_చురుకైన()) {
		ఒకవేళ (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS, SMT is not supported\n",
				cpumask_pr_వాదనలు(cpu_ముసుగు));
		}
		తిరిగి false;
	}

	ఒకవేళ (!arch_scale_freq_invariant()) {
		ఒకవేళ (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS: frequency-invariant load tracking not yet supported",
				cpumask_pr_వాదనలు(cpu_ముసుగు));
		}
		తిరిగి false;
	}

	ఒకవేళ (!cpufreq_ready_for_eas(cpu_ముసుగు)) {
		ఒకవేళ (sched_debug()) {
			pr_info("rd %*pbl: Checking EAS: cpufreq is not ready\n",
				cpumask_pr_వాదనలు(cpu_ముసుగు));
		}
		తిరిగి false;
	}

	తిరిగి true;
}

శూన్య rebuild_sched_domains_energy(శూన్య)
{
	mutex_తాళం(&sched_energy_mutex);
	sched_energy_update = true;
	rebuild_sched_domains();
	sched_energy_update = false;
	mutex_unlock(&sched_energy_mutex);
}

#నిర్వచితమైతే CONFIG_SYSCTL
స్థిర పూర్ణాంకం sched_energy_aware_handler(స్థిరరాశి నిర్మాణం ctl_పట్టిక *పట్టిక, పూర్ణాంకం లిఖ,
		శూన్య *బఫరు, కొలత_t *lenp, loff_t *ppos)
{
	పూర్ణాంకం ret;

	ఒకవేళ (లిఖ && !capable(CAP_SYS_ADMIN))
		తిరిగి -EPERM;

	ఒకవేళ (!sched_is_eas_possible(cpu_చురుకైన_ముసుగు)) {
		ఒకవేళ (లిఖ) {
			తిరిగి -EOPNOTSUPP;
		} లేకపోతే {
			*lenp = 0;
			తిరిగి 0;
		}
	}

	ret = proc_dointvec_minmax(పట్టిక, లిఖ, బఫరు, lenp, ppos);
	ఒకవేళ (!ret && లిఖ) {
		ఒకవేళ (sysctl_sched_energy_aware != sched_energy_enabled())
			rebuild_sched_domains_energy();
	}

	తిరిగి ret;
}

స్థిర స్థిరరాశి నిర్మాణం ctl_పట్టిక sched_energy_aware_sysctls[] = {
	{
		.procname       = "sched_energy_aware",
		.దత్తాంశం           = &sysctl_sched_energy_aware,
		.maxlen         = పరిమాణం(గుర్తులేని పూర్ణాంకం),
		.బహుళకం           = 0644,
		.proc_handler   = sched_energy_aware_handler,
		.extra1         = SYSCTL_ZERO,
		.extra2         = SYSCTL_ONE,
	},
};

స్థిర పూర్ణాంకం __init sched_energy_aware_sysctl_init(శూన్య)
{
	రిజిస్టరు_sysctl_init("kernel", sched_energy_aware_sysctls);
	తిరిగి 0;
}

late_initcall(sched_energy_aware_sysctl_init);
#అయితే_ముగింపు /* CONFIG_SYSCTL */

స్థిర శూన్య free_pd(నిర్మాణం perf_domain *pd)
{
	నిర్మాణం perf_domain *tmp;

	అయితే (pd) {
		tmp = pd->తదుపరి;
		kfree(pd);
		pd = tmp;
	}
}

స్థిర నిర్మాణం perf_domain *కనుగొను_pd(నిర్మాణం perf_domain *pd, పూర్ణాంకం cpu)
{
	అయితే (pd) {
		ఒకవేళ (cpumask_test_cpu(cpu, perf_domain_పరిధి(pd)))
			తిరిగి pd;
		pd = pd->తదుపరి;
	}

	తిరిగి ఖాళీ;
}

స్థిర నిర్మాణం perf_domain *pd_init(పూర్ణాంకం cpu)
{
	నిర్మాణం em_perf_domain *obj = em_cpu_get(cpu);
	నిర్మాణం perf_domain *pd;

	ఒకవేళ (!obj) {
		ఒకవేళ (sched_debug())
			pr_info("%s: no EM found for CPU%d\n", __func__, cpu);
		తిరిగి ఖాళీ;
	}

	pd = kzalloc_obj(*pd);
	ఒకవేళ (!pd)
		తిరిగి ఖాళీ;
	pd->em_pd = obj;

	తిరిగి pd;
}

స్థిర శూన్య perf_domain_debug(స్థిరరాశి నిర్మాణం cpumask *cpu_map,
						నిర్మాణం perf_domain *pd)
{
	ఒకవేళ (!sched_debug() || !pd)
		తిరిగి;

	printk(KERN_DEBUG "root_domain %*pbl:", cpumask_pr_వాదనలు(cpu_map));

	అయితే (pd) {
		printk(KERN_CONT " pd%d:{ cpus=%*pbl nr_pstate=%d }",
				cpumask_మొదటి(perf_domain_పరిధి(pd)),
				cpumask_pr_వాదనలు(perf_domain_పరిధి(pd)),
				em_pd_nr_perf_స్థితులు(pd->em_pd));
		pd = pd->తదుపరి;
	}

	printk(KERN_CONT "\n");
}

స్థిర శూన్య destroy_perf_domain_rcu(నిర్మాణం rcu_తల *rp)
{
	నిర్మాణం perf_domain *pd;

	pd = container_యొక్క(rp, నిర్మాణం perf_domain, rcu);
	free_pd(pd);
}

స్థిర శూన్య sched_energy_సమితి(bool has_eas)
{
	ఒకవేళ (!has_eas && sched_energy_enabled()) {
		ఒకవేళ (sched_debug())
			pr_info("%s: stopping EAS\n", __func__);
		static_branch_disable_cpuslocked(&sched_energy_present);
	} లేకపోతే ఒకవేళ (has_eas && !sched_energy_enabled()) {
		ఒకవేళ (sched_debug())
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
స్థిర bool build_perf_domains(స్థిరరాశి నిర్మాణం cpumask *cpu_map)
{
	పూర్ణాంకం i;
	నిర్మాణం perf_domain *pd = ఖాళీ, *tmp;
	పూర్ణాంకం cpu = cpumask_మొదటి(cpu_map);
	నిర్మాణం మూలం_domain *rd = cpu_rq(cpu)->rd;

	ఒకవేళ (!sysctl_sched_energy_aware)
		వెళ్ళు విడుదల;

	ఒకవేళ (!sched_is_eas_possible(cpu_map))
		వెళ్ళు విడుదల;

	for_each_cpu(i, cpu_map) {
		/* Skip already covered CPUs. */
		ఒకవేళ (కనుగొను_pd(pd, i))
			కొనసాగు;

		/* Create the new pd and add it to the local list. */
		tmp = pd_init(i);
		ఒకవేళ (!tmp)
			వెళ్ళు విడుదల;
		tmp->తదుపరి = pd;
		pd = tmp;
	}

	perf_domain_debug(cpu_map, pd);

	/* Attach the new list of performance domains to the root domain. */
	tmp = rd->pd;
	rcu_కేటాయించు_సూచిక(rd->pd, pd);
	ఒకవేళ (tmp)
		call_rcu(&tmp->rcu, destroy_perf_domain_rcu);

	తిరిగి !!pd;

విడుదల:
	free_pd(pd);
	tmp = rd->pd;
	rcu_కేటాయించు_సూచిక(rd->pd, ఖాళీ);
	ఒకవేళ (tmp)
		call_rcu(&tmp->rcu, destroy_perf_domain_rcu);

	తిరిగి false;
}
#లేకపోతే /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL): */
స్థిర శూన్య free_pd(నిర్మాణం perf_domain *pd) { }
#అయితే_ముగింపు /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL) */

స్థిర శూన్య free_rootdomain(నిర్మాణం rcu_తల *rcu)
{
	నిర్మాణం మూలం_domain *rd = container_యొక్క(rcu, నిర్మాణం మూలం_domain, rcu);

	cpupri_cleanup(&rd->cpupri);
	cpudl_cleanup(&rd->cpudl);
	free_cpumask_var(rd->dlo_ముసుగు);
	free_cpumask_var(rd->rto_ముసుగు);
	free_cpumask_var(rd->online);
	free_cpumask_var(rd->పరిధి);
	free_pd(rd->pd);
	kfree(rd);
}

శూన్య rq_attach_మూలం(నిర్మాణం rq *rq, నిర్మాణం మూలం_domain *rd)
{
	నిర్మాణం మూలం_domain *old_rd = ఖాళీ;
	నిర్మాణం rq_flags rf;

	rq_తాళం_irqsave(rq, &rf);

	ఒకవేళ (rq->rd) {
		old_rd = rq->rd;

		ఒకవేళ (cpumask_test_cpu(rq->cpu, old_rd->online))
			సమితి_rq_offline(rq);

		cpumask_clear_cpu(rq->cpu, old_rd->పరిధి);

		/*
		 * If we don't want to free the old_rd yet then
		 * set old_rd to NULL to skip the freeing later
		 * in this function:
		 */
		ఒకవేళ (!atomic_dec_and_test(&old_rd->refcount))
			old_rd = ఖాళీ;
	}

	atomic_inc(&rd->refcount);
	rq->rd = rd;

	cpumask_సమితి_cpu(rq->cpu, rd->పరిధి);
	ఒకవేళ (cpumask_test_cpu(rq->cpu, cpu_చురుకైన_ముసుగు))
		సమితి_rq_online(rq);

	/*
	 * Because the rq is not a task, dl_add_task_root_domain() did not
	 * move the fair server bw to the rd if it already started.
	 * Add it now.
	 */
	ఒకవేళ (rq->fair_server.dl_server)
		__dl_server_attach_root(&rq->fair_server, rq);

#నిర్వచితమైతే CONFIG_SCHED_CLASS_EXT
	ఒకవేళ (rq->ext_server.dl_server)
		__dl_server_attach_root(&rq->ext_server, rq);
#అయితే_ముగింపు

	rq_unlock_irqrestore(rq, &rf);

	ఒకవేళ (old_rd)
		call_rcu(&old_rd->rcu, free_rootdomain);
}

శూన్య sched_get_rd(నిర్మాణం మూలం_domain *rd)
{
	atomic_inc(&rd->refcount);
}

శూన్య sched_put_rd(నిర్మాణం మూలం_domain *rd)
{
	ఒకవేళ (!atomic_dec_and_test(&rd->refcount))
		తిరిగి;

	call_rcu(&rd->rcu, free_rootdomain);
}

స్థిర పూర్ణాంకం init_rootdomain(నిర్మాణం మూలం_domain *rd)
{
	ఒకవేళ (!zalloc_cpumask_var(&rd->పరిధి, GFP_KERNEL))
		వెళ్ళు బహిర్గతం;
	ఒకవేళ (!zalloc_cpumask_var(&rd->online, GFP_KERNEL))
		వెళ్ళు free_పరిధి;
	ఒకవేళ (!zalloc_cpumask_var(&rd->dlo_ముసుగు, GFP_KERNEL))
		వెళ్ళు free_online;
	ఒకవేళ (!zalloc_cpumask_var(&rd->rto_ముసుగు, GFP_KERNEL))
		వెళ్ళు free_dlo_ముసుగు;

#నిర్వచితమైతే HAVE_RT_నెట్టు_IPI
	rd->rto_cpu = -1;
	raw_spin_తాళం_init(&rd->rto_తాళం);
	rd->rto_నెట్టు_work = IRQ_WORK_INIT_HARD(rto_నెట్టు_irq_work_func);
#అయితే_ముగింపు

	rd->సందర్శించు_cookie = 0;
	init_dl_bw(&rd->dl_bw);
	ఒకవేళ (cpudl_init(&rd->cpudl) != 0)
		వెళ్ళు free_rto_ముసుగు;

	ఒకవేళ (cpupri_init(&rd->cpupri) != 0)
		వెళ్ళు free_cpudl;
	తిరిగి 0;

free_cpudl:
	cpudl_cleanup(&rd->cpudl);
free_rto_ముసుగు:
	free_cpumask_var(rd->rto_ముసుగు);
free_dlo_ముసుగు:
	free_cpumask_var(rd->dlo_ముసుగు);
free_online:
	free_cpumask_var(rd->online);
free_పరిధి:
	free_cpumask_var(rd->పరిధి);
బహిర్గతం:
	తిరిగి -ENOMEM;
}

/*
 * By default the system creates a single root-domain with all CPUs as
 * members (mimicking the global state we have today).
 */
నిర్మాణం మూలం_domain def_మూలం_domain;

శూన్య __init init_defrootdomain(శూన్య)
{
	init_rootdomain(&def_మూలం_domain);

	atomic_సమితి(&def_మూలం_domain.refcount, 1);
}

స్థిర నిర్మాణం మూలం_domain *alloc_rootdomain(శూన్య)
{
	నిర్మాణం మూలం_domain *rd;

	rd = kzalloc_obj(*rd);
	ఒకవేళ (!rd)
		తిరిగి ఖాళీ;

	ఒకవేళ (init_rootdomain(rd) != 0) {
		kfree(rd);
		తిరిగి ఖాళీ;
	}

	తిరిగి rd;
}

స్థిర శూన్య free_sched_groups(నిర్మాణం sched_సమూహం *sg, పూర్ణాంకం free_sgc)
{
	నిర్మాణం sched_సమూహం *tmp, *మొదటి;

	ఒకవేళ (!sg)
		తిరిగి;

	మొదటి = sg;
	చేయి {
		tmp = sg->తదుపరి;

		ఒకవేళ (free_sgc && atomic_dec_and_test(&sg->sgc->ref))
			kfree(sg->sgc);

		ఒకవేళ (atomic_dec_and_test(&sg->ref))
			kfree(sg);
		sg = tmp;
	} అయితే (sg != మొదటి);
}

స్థిర శూన్య free_sched_domain_shared(నిర్మాణం sched_domain_shared *sds)
{
	ఒకవేళ (sds && atomic_dec_and_test(&sds->ref))
		kfree(sds);
}

స్థిర శూన్య destroy_sched_domain(నిర్మాణం sched_domain *sd)
{
	/*
	 * A normal sched domain may have multiple group references, an
	 * overlapping domain, having private groups, only one.  Iterate,
	 * dropping group/capacity references, freeing where none remain.
	 */
	free_sched_groups(sd->groups, 1);
	free_sched_domain_shared(sd->shared);

#నిర్వచితమైతే CONFIG_SCHED_CACHE
	/* only the bottom sd has llc_counts array */
	kfree(sd->llc_counts);
#అయితే_ముగింపు
	kfree(sd);
}

స్థిర శూన్య destroy_sched_domains_rcu(నిర్మాణం rcu_తల *rcu)
{
	నిర్మాణం sched_domain *sd = container_యొక్క(rcu, నిర్మాణం sched_domain, rcu);

	అయితే (sd) {
		నిర్మాణం sched_domain *మాతృ = sd->మాతృ;
		destroy_sched_domain(sd);
		sd = మాతృ;
	}
}

స్థిర శూన్య destroy_sched_domains(నిర్మాణం sched_domain *sd)
{
	ఒకవేళ (sd)
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
DEFINE_చొప్పున_CPU(నిర్మాణం sched_domain __rcu *, sd_llc);
DEFINE_చొప్పున_CPU(పూర్ణాంకం, sd_llc_కొలత);
DEFINE_చొప్పున_CPU(పూర్ణాంకం, sd_llc_id) = -1;
DEFINE_చొప్పున_CPU(పూర్ణాంకం, sd_share_id);
DEFINE_చొప్పున_CPU(నిర్మాణం sched_domain_shared __rcu *, sd_llc_shared);
DEFINE_చొప్పున_CPU(నిర్మాణం sched_domain_shared __rcu *, sd_balance_shared);
DEFINE_చొప్పున_CPU(నిర్మాణం sched_domain __rcu *, sd_numa);
DEFINE_చొప్పున_CPU(నిర్మాణం sched_domain __rcu *, sd_asym_packing);
DEFINE_చొప్పున_CPU(నిర్మాణం sched_domain __rcu *, sd_asym_cpucapacity);

DEFINE_STATIC_కీలకం_FALSE(sched_asym_cpucapacity);
DEFINE_STATIC_కీలకం_FALSE(sched_cluster_చురుకైన);

స్థిర శూన్య update_శిఖరం_cache_domain(పూర్ణాంకం cpu)
{
	నిర్మాణం sched_domain_shared *sds = ఖాళీ;
	నిర్మాణం sched_domain *sd;
	పూర్ణాంకం id = cpu;
	పూర్ణాంకం కొలత = 1;

	sd = highest_flag_domain(cpu, SD_SHARE_LLC);
	ఒకవేళ (sd) {
		id = cpumask_మొదటి(sched_domain_పరిధి(sd));
		కొలత = cpumask_బరువు(sched_domain_పరిధి(sd));

		/* If sd_llc exists, sd_llc_shared should exist too. */
		WARN_ON_ONCE(!sd->shared);
		sds = sd->shared;
	}

	rcu_కేటాయించు_సూచిక(చొప్పున_cpu(sd_llc, cpu), sd);
	చొప్పున_cpu(sd_llc_కొలత, cpu) = కొలత;
	rcu_కేటాయించు_సూచిక(చొప్పున_cpu(sd_llc_shared, cpu), sds);

	sd = అత్యల్ప_flag_domain(cpu, SD_CLUSTER);
	ఒకవేళ (sd)
		id = cpumask_మొదటి(sched_domain_పరిధి(sd));

	/*
	 * This assignment should be placed after the sd_llc_id as
	 * we want this id equals to cluster id on cluster machines
	 * but equals to LLC id on non-Cluster machines.
	 */
	చొప్పున_cpu(sd_share_id, cpu) = id;

	sd = అత్యల్ప_flag_domain(cpu, SD_NUMA);
	rcu_కేటాయించు_సూచిక(చొప్పున_cpu(sd_numa, cpu), sd);

	sd = highest_flag_domain(cpu, SD_ASYM_PACKING);
	rcu_కేటాయించు_సూచిక(చొప్పున_cpu(sd_asym_packing, cpu), sd);

	sd = అత్యల్ప_flag_domain(cpu, SD_ASYM_CPUCAPACITY_నిండిన);
	/*
	 * The shared object is attached to sd_asym_cpucapacity only when the
	 * asym domain is non-overlapping (i.e., not built from SD_NUMA).
	 * On overlapping (NUMA) asym domains we fall back to letting the
	 * SD_SHARE_LLC path own the shared object, so sd->shared may be NULL
	 * here.
	 */
	ఒకవేళ (sd && sd->shared)
		sds = sd->shared;

	rcu_కేటాయించు_సూచిక(చొప్పున_cpu(sd_asym_cpucapacity, cpu), sd);
	rcu_కేటాయించు_సూచిక(చొప్పున_cpu(sd_balance_shared, cpu), sds);
}

/*
 * Attach the domain 'sd' to 'cpu' as its base domain. Callers must
 * hold the hotplug lock.
 */
స్థిర శూన్య
cpu_attach_domain(నిర్మాణం sched_domain *sd, నిర్మాణం మూలం_domain *rd, పూర్ణాంకం cpu)
{
	నిర్మాణం rq *rq = cpu_rq(cpu);
	నిర్మాణం sched_domain *tmp;

	/* Remove the sched domains which do not contribute to scheduling. */
	కొరకు (tmp = sd; tmp; ) {
		నిర్మాణం sched_domain *మాతృ = tmp->మాతృ;
		ఒకవేళ (!మాతృ)
			ఆపు;

		ఒకవేళ (sd_మాతృ_degenerate(tmp, మాతృ)) {
			tmp->మాతృ = మాతృ->మాతృ;

			/* Pick reference to parent->shared. */
			ఒకవేళ (మాతృ->shared) {
				/*
				 * It is safe to free a sd->shared that
				 * has not been published yet. If a
				 * sd->shared was published, the refcount
				 * will end up being non-zero and it will
				 * not be freed here.
				 */
				free_sched_domain_shared(tmp->shared);
				tmp->shared = మాతృ->shared;
				మాతృ->shared = ఖాళీ;
			}

			ఒకవేళ (మాతృ->మాతృ) {
				మాతృ->మాతృ->బిడ్డ = tmp;
				మాతృ->మాతృ->groups->flags = tmp->flags;
			}

			/*
			 * Transfer SD_PREFER_SIBLING down in case of a
			 * degenerate parent; the spans match for this
			 * so the property transfers.
			 */
			ఒకవేళ (మాతృ->flags & SD_PREFER_SIBLING)
				tmp->flags |= SD_PREFER_SIBLING;
			destroy_sched_domain(మాతృ);
		} లేకపోతే
			tmp = tmp->మాతృ;
	}

	ఒకవేళ (sd && sd_degenerate(sd)) {
		tmp = sd;
		sd = sd->మాతృ;

		ఒకవేళ (sd) {
			నిర్మాణం sched_సమూహం *sg = sd->groups;

#నిర్వచితమైతే CONFIG_SCHED_CACHE
			/* move buffer to parent as child is being destroyed */
			sd->llc_counts = tmp->llc_counts;
			sd->llc_max = tmp->llc_max;
			sd->llc_బైట్లు = tmp->llc_బైట్లు;
			/* make sure destroy_sched_domain() does not free it */
			tmp->llc_counts = ఖాళీ;
			tmp->llc_max = 0;
			tmp->llc_బైట్లు = 0;
#అయితే_ముగింపు
			/*
			 * sched groups hold the flags of the child sched
			 * domain for convenience. Clear such flags since
			 * the child is being destroyed.
			 */
			చేయి {
				sg->flags = 0;
			} అయితే (sg != sd->groups);

			sd->బిడ్డ = ఖాళీ;
		}

		destroy_sched_domain(tmp);
	}

	sched_domain_debug(sd, cpu);

	rq_attach_మూలం(rq, rd);
	tmp = rq->sd;
	rcu_కేటాయించు_సూచిక(rq->sd, sd);
	dirty_sched_domain_sysctl(cpu);
	destroy_sched_domains(tmp);

	update_శిఖరం_cache_domain(cpu);
}

నిర్మాణం s_దత్తాంశం {
	నిర్మాణం sched_domain_shared * __percpu *sds;
	నిర్మాణం sched_domain * __percpu *sd;
	నిర్మాణం మూలం_domain	*rd;
};

గణన s_alloc {
	sa_rootdomain,
	sa_sd,
	sa_sd_shared,
	sa_sd_storage,
	sa_none,
};

#నిర్వచితమైతే CONFIG_SCHED_CACHE
/* hardware support for cache aware scheduling */
DEFINE_STATIC_కీలకం_FALSE(sched_cache_present);
/*
 * Indicator of whether cache aware scheduling
 * is active, used by the scheduler.
 */
DEFINE_STATIC_కీలకం_FALSE(sched_cache_చురుకైన);
/* user wants cache aware scheduling [0 or 1] */
పూర్ణాంకం sysctl_sched_cache_user = 1;

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
స్థిర గుర్తులేని దీర్ఘ get_effective_llc_బైట్లు(పూర్ణాంకం cpu,
					     నిర్మాణం sched_domain *sd)
{
	నిర్మాణం cacheinfo *ci;
	గుర్తులేని పూర్ణాంకం hw_బరువు;

	ci = get_cpu_cacheinfo_llc(cpu);
	ఒకవేళ (!ci)
		తిరిగి 0;

	hw_బరువు = cpumask_బరువు(&ci->shared_cpu_map);
	ఒకవేళ (!hw_బరువు)
		తిరిగి 0;

	తిరిగి div_u64((u64)ci->కొలత * sd->పరిధి_బరువు, hw_బరువు);
}

స్థిర bool alloc_sd_llc(స్థిరరాశి నిర్మాణం cpumask *cpu_map,
			 నిర్మాణం s_దత్తాంశం *d)
{
	నిర్మాణం sched_domain *sd, *శిఖరం_llc, *మాతృ;
	గుర్తులేని పూర్ణాంకం *p;
	పూర్ణాంకం i;

	for_each_cpu(i, cpu_map) {
		sd = *చొప్పున_cpu_ptr(d->sd, i);
		ఒకవేళ (!sd)
			వెళ్ళు err;

		p = kcalloc_కణుపు(max_lid + 1, పరిమాణం(గుర్తులేని పూర్ణాంకం),
				 GFP_KERNEL, cpu_గమ్యం_కణుపు(i));
		ఒకవేళ (!p)
			వెళ్ళు err;

		శిఖరం_llc = sd;
		/*
		 * Find the topmost SD_SHARE_LLC domain.
		 * Not yet attached to the CPU, so per_cpu(sd_llc, i)
		 * can not be used.
		 */
		అయితే ((మాతృ = rcu_dereference_protected(శిఖరం_llc->మాతృ, true)) &&
		       (మాతృ->flags & SD_SHARE_LLC))
			శిఖరం_llc = మాతృ;

		ఒకవేళ (శిఖరం_llc->flags & SD_SHARE_LLC) {
			sd->llc_max = max_lid + 1;
			sd->llc_counts = p;
			sd->llc_బైట్లు = get_effective_llc_బైట్లు(i, శిఖరం_llc);
		} లేకపోతే {
			/* avoid memory leak */
			kfree(p);
		}
	}

	తిరిగి true;
err:
	for_each_cpu(i, cpu_map) {
		sd = *చొప్పున_cpu_ptr(d->sd, i);
		ఒకవేళ (sd) {
			kfree(sd->llc_counts);
			sd->llc_counts = ఖాళీ;
			sd->llc_max = 0;
			sd->llc_బైట్లు = 0;
		}
	}

	తిరిగి false;
}

/*
 * Enable/disable cache aware scheduling according to
 * user input and the presence of hardware support.
 */
స్థిర శూన్య sched_cache_చురుకైన_సమితి_2(శూన్య)
{
	lockdep_assert_cpus_held();
	lockdep_assert_held(&sched_domains_mutex);

	/* hardware does not support */
	ఒకవేళ (!static_branch_likely(&sched_cache_present)) {
		static_branch_disable_cpuslocked(&sched_cache_చురుకైన);
		ఒకవేళ (sched_debug())
			pr_info("%s: cache aware scheduling not supported on this platform\n", __func__);
		తిరిగి;
	}

	/*
	 * user wants it or not ?
	 * TBD: read before writing the static key.
	 * It is not in the critical path, leave as-is
	 * for now.
	 */
	ఒకవేళ (sysctl_sched_cache_user) {
		static_branch_enable_cpuslocked(&sched_cache_చురుకైన);
		ఒకవేళ (sched_debug())
			pr_info("%s: enabling cache aware scheduling\n", __func__);
	} లేకపోతే {
		static_branch_disable_cpuslocked(&sched_cache_చురుకైన);
		ఒకవేళ (sched_debug())
			pr_info("%s: disabling cache aware scheduling\n", __func__);
	}
}

/* used by debugfs */
శూన్య sched_cache_చురుకైన_సమితి(శూన్య)
{
	cpus_read_తాళం();
	sched_domains_mutex_తాళం();
	sched_cache_చురుకైన_సమితి_2();
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
శూన్య sched_update_llc_బైట్లు(స్థిరరాశి నిర్మాణం cpumask *cpus)
{
	నిర్మాణం sched_domain *sd, *sdp;
	గుర్తులేని పూర్ణాంకం i;

	sched_domains_mutex_తాళం();

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
		sdp = rcu_dereference_sched_domain(చొప్పున_cpu(sd_llc, i));
		ఒకవేళ (!sdp)
			కొనసాగు;

		sd = rcu_dereference_sched_domain(cpu_rq(i)->sd);
		ఒకవేళ (sd)
			sd->llc_బైట్లు = get_effective_llc_బైట్లు(i, sdp);
	}

	sched_domains_mutex_unlock();
}

స్థిర శూన్య sched_cache_సమితి(bool has_multi_llcs)
{
	/*
	 * TBD: check before writing to it. sched domain rebuild
	 * is not in the critical path, leave as-is for now.
	 */
	ఒకవేళ (has_multi_llcs)
		static_branch_enable_cpuslocked(&sched_cache_present);
	లేకపోతే
		static_branch_disable_cpuslocked(&sched_cache_present);

	sched_cache_చురుకైన_సమితి_2();
}
#లేకపోతే
స్థిర bool alloc_sd_llc(స్థిరరాశి నిర్మాణం cpumask *cpu_map,
			 నిర్మాణం s_దత్తాంశం *d)
{
	తిరిగి false;
}
స్థిర అంతఃవరుస శూన్య sched_cache_సమితి(bool has_multi_llcs) { }
#అయితే_ముగింపు

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
స్థిర bool sd_లోపల_multi_llcs(నిర్మాణం sched_domain *sd)
{
	నిర్మాణం sched_domain *sdp = sd->మాతృ;

	/* it does not make sense to aggregate to 1 CPU */
	ఒకవేళ (sd->పరిధి_బరువు == 1)
		తిరిగి false;

	అయితే (sdp && sdp->పరిధి_బరువు == sd->పరిధి_బరువు)
		sdp = sdp->మాతృ;

	తిరిగి !!sdp;
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
పూర్ణాంకం సమూహం_balance_cpu(నిర్మాణం sched_సమూహం *sg)
{
	తిరిగి cpumask_మొదటి(సమూహం_balance_ముసుగు(sg));
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
స్థిర శూన్య
build_balance_ముసుగు(నిర్మాణం sched_domain *sd, నిర్మాణం sched_సమూహం *sg, నిర్మాణం cpumask *ముసుగు)
{
	స్థిరరాశి నిర్మాణం cpumask *sg_పరిధి = sched_సమూహం_పరిధి(sg);
	నిర్మాణం sd_దత్తాంశం *sdd = sd->అంతరంగ;
	నిర్మాణం sched_domain *sibling;
	పూర్ణాంకం i;

	cpumask_clear(ముసుగు);

	for_each_cpu(i, sg_పరిధి) {
		sibling = *చొప్పున_cpu_ptr(sdd->sd, i);

		/*
		 * Can happen in the asymmetric case, where these siblings are
		 * unused. The mask will not be empty because those CPUs that
		 * do have the top domain _should_ span the domain.
		 */
		ఒకవేళ (!sibling->బిడ్డ)
			కొనసాగు;

		/* If we would not end up here, we can't continue from here */
		ఒకవేళ (!cpumask_equal(sg_పరిధి, sched_domain_పరిధి(sibling->బిడ్డ)))
			కొనసాగు;

		cpumask_సమితి_cpu(i, ముసుగు);
	}

	/* We must not have empty masks here */
	WARN_ON_ONCE(cpumask_ఖాళీలు(ముసుగు));
}

/*
 * XXX: This creates per-node group entries; since the load-balancer will
 * immediately access remote memory to construct this group's load-balance
 * statistics having the groups node local is of dubious benefit.
 */
స్థిర నిర్మాణం sched_సమూహం *
build_సమూహం_from_బిడ్డ_sched_domain(నిర్మాణం sched_domain *sd, పూర్ణాంకం cpu)
{
	నిర్మాణం sched_సమూహం *sg;
	నిర్మాణం cpumask *sg_పరిధి;

	sg = kzalloc_కణుపు(పరిమాణం(నిర్మాణం sched_సమూహం) + cpumask_కొలత(),
			GFP_KERNEL, cpu_గమ్యం_కణుపు(cpu));

	ఒకవేళ (!sg)
		తిరిగి ఖాళీ;

	sg_పరిధి = sched_సమూహం_పరిధి(sg);
	ఒకవేళ (sd->బిడ్డ) {
		cpumask_copy(sg_పరిధి, sched_domain_పరిధి(sd->బిడ్డ));
		sg->flags = sd->బిడ్డ->flags;
	} లేకపోతే {
		cpumask_copy(sg_పరిధి, sched_domain_పరిధి(sd));
	}

	atomic_inc(&sg->ref);
	తిరిగి sg;
}

స్థిర శూన్య init_overlap_sched_సమూహం(నిర్మాణం sched_domain *sd,
				     నిర్మాణం sched_సమూహం *sg)
{
	నిర్మాణం cpumask *ముసుగు = sched_domains_tmpmask2;
	నిర్మాణం sd_దత్తాంశం *sdd = sd->అంతరంగ;
	నిర్మాణం cpumask *sg_పరిధి;
	పూర్ణాంకం cpu;

	build_balance_ముసుగు(sd, sg, ముసుగు);
	cpu = cpumask_మొదటి(ముసుగు);

	sg->sgc = *చొప్పున_cpu_ptr(sdd->sgc, cpu);
	ఒకవేళ (atomic_inc_return(&sg->sgc->ref) == 1)
		cpumask_copy(సమూహం_balance_ముసుగు(sg), ముసుగు);
	లేకపోతే
		WARN_ON_ONCE(!cpumask_equal(సమూహం_balance_ముసుగు(sg), ముసుగు));

	/*
	 * Initialize sgc->capacity such that even if we mess up the
	 * domains and no possible iteration will get us here, we won't
	 * die on a /0 trap.
	 */
	sg_పరిధి = sched_సమూహం_పరిధి(sg);
	sg->sgc->సామర్థ్యం = SCHED_సామర్థ్యం_SCALE * cpumask_బరువు(sg_పరిధి);
	sg->sgc->min_సామర్థ్యం = SCHED_సామర్థ్యం_SCALE;
	sg->sgc->max_సామర్థ్యం = SCHED_సామర్థ్యం_SCALE;
}

స్థిర నిర్మాణం sched_domain *
కనుగొను_descended_sibling(నిర్మాణం sched_domain *sd, నిర్మాణం sched_domain *sibling)
{
	/*
	 * The proper descendant would be the one whose child won't span out
	 * of sd
	 */
	అయితే (sibling->బిడ్డ &&
	       !cpumask_subset(sched_domain_పరిధి(sibling->బిడ్డ),
			       sched_domain_పరిధి(sd)))
		sibling = sibling->బిడ్డ;

	/*
	 * As we are referencing sgc across different topology level, we need
	 * to go down to skip those sched_domains which don't contribute to
	 * scheduling because they will be degenerated in cpu_attach_domain
	 */
	అయితే (sibling->బిడ్డ &&
	       cpumask_equal(sched_domain_పరిధి(sibling->బిడ్డ),
			     sched_domain_పరిధి(sibling)))
		sibling = sibling->బిడ్డ;

	తిరిగి sibling;
}

స్థిర పూర్ణాంకం
build_overlap_sched_groups(నిర్మాణం sched_domain *sd, పూర్ణాంకం cpu)
{
	నిర్మాణం sched_సమూహం *మొదటి = ఖాళీ, *చివరి = ఖాళీ, *sg;
	స్థిరరాశి నిర్మాణం cpumask *పరిధి = sched_domain_పరిధి(sd);
	నిర్మాణం cpumask *covered = sched_domains_tmpmask;
	నిర్మాణం sd_దత్తాంశం *sdd = sd->అంతరంగ;
	నిర్మాణం sched_domain *sibling;
	పూర్ణాంకం i;

	cpumask_clear(covered);

	for_each_cpu_wrap(i, పరిధి, cpu) {
		నిర్మాణం cpumask *sg_పరిధి;

		ఒకవేళ (cpumask_test_cpu(i, covered))
			కొనసాగు;

		sibling = *చొప్పున_cpu_ptr(sdd->sd, i);

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
		ఒకవేళ (!cpumask_test_cpu(i, sched_domain_పరిధి(sibling)))
			కొనసాగు;

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
		ఒకవేళ (sibling->బిడ్డ &&
		    !cpumask_subset(sched_domain_పరిధి(sibling->బిడ్డ), పరిధి))
			sibling = కనుగొను_descended_sibling(sd, sibling);

		sg = build_సమూహం_from_బిడ్డ_sched_domain(sibling, cpu);
		ఒకవేళ (!sg)
			వెళ్ళు fail;

		sg_పరిధి = sched_సమూహం_పరిధి(sg);
		cpumask_or(covered, covered, sg_పరిధి);

		init_overlap_sched_సమూహం(sibling, sg);

		ఒకవేళ (!మొదటి)
			మొదటి = sg;
		ఒకవేళ (చివరి)
			చివరి->తదుపరి = sg;
		చివరి = sg;
		చివరి->తదుపరి = మొదటి;
	}
	sd->groups = మొదటి;

	తిరిగి 0;

fail:
	free_sched_groups(మొదటి, 0);

	తిరిగి -ENOMEM;
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

స్థిర నిర్మాణం sched_సమూహం *get_సమూహం(పూర్ణాంకం cpu, నిర్మాణం sd_దత్తాంశం *sdd)
{
	నిర్మాణం sched_domain *sd = *చొప్పున_cpu_ptr(sdd->sd, cpu);
	నిర్మాణం sched_domain *బిడ్డ = sd->బిడ్డ;
	నిర్మాణం sched_సమూహం *sg;
	bool already_సందర్శించిన;

	ఒకవేళ (బిడ్డ)
		cpu = cpumask_మొదటి(sched_domain_పరిధి(బిడ్డ));

	sg = *చొప్పున_cpu_ptr(sdd->sg, cpu);
	sg->sgc = *చొప్పున_cpu_ptr(sdd->sgc, cpu);

	/* Increase refcounts for claim_allocations: */
	already_సందర్శించిన = atomic_inc_return(&sg->ref) > 1;
	/* sgc visits should follow a similar trend as sg */
	WARN_ON(already_సందర్శించిన != (atomic_inc_return(&sg->sgc->ref) > 1));

	/* If we have already visited that group, it's already initialized. */
	ఒకవేళ (already_సందర్శించిన)
		తిరిగి sg;

	ఒకవేళ (బిడ్డ) {
		cpumask_copy(sched_సమూహం_పరిధి(sg), sched_domain_పరిధి(బిడ్డ));
		cpumask_copy(సమూహం_balance_ముసుగు(sg), sched_సమూహం_పరిధి(sg));
		sg->flags = బిడ్డ->flags;
	} లేకపోతే {
		cpumask_సమితి_cpu(cpu, sched_సమూహం_పరిధి(sg));
		cpumask_సమితి_cpu(cpu, సమూహం_balance_ముసుగు(sg));
	}

	sg->sgc->సామర్థ్యం = SCHED_సామర్థ్యం_SCALE * cpumask_బరువు(sched_సమూహం_పరిధి(sg));
	sg->sgc->min_సామర్థ్యం = SCHED_సామర్థ్యం_SCALE;
	sg->sgc->max_సామర్థ్యం = SCHED_సామర్థ్యం_SCALE;

	తిరిగి sg;
}

/*
 * build_sched_groups will build a circular linked list of the groups
 * covered by the given span, will set each group's ->cpumask correctly,
 * and will initialize their ->sgc.
 *
 * Assumes the sched_domain tree is fully constructed
 */
స్థిర పూర్ణాంకం
build_sched_groups(నిర్మాణం sched_domain *sd, పూర్ణాంకం cpu)
{
	నిర్మాణం sched_సమూహం *మొదటి = ఖాళీ, *చివరి = ఖాళీ;
	నిర్మాణం sd_దత్తాంశం *sdd = sd->అంతరంగ;
	స్థిరరాశి నిర్మాణం cpumask *పరిధి = sched_domain_పరిధి(sd);
	నిర్మాణం cpumask *covered;
	పూర్ణాంకం i;

	lockdep_assert_held(&sched_domains_mutex);
	covered = sched_domains_tmpmask;

	cpumask_clear(covered);

	for_each_cpu_wrap(i, పరిధి, cpu) {
		నిర్మాణం sched_సమూహం *sg;

		ఒకవేళ (cpumask_test_cpu(i, covered))
			కొనసాగు;

		sg = get_సమూహం(i, sdd);

		cpumask_or(covered, covered, sched_సమూహం_పరిధి(sg));

		ఒకవేళ (!మొదటి)
			మొదటి = sg;
		ఒకవేళ (చివరి)
			చివరి->తదుపరి = sg;
		చివరి = sg;
	}
	చివరి->తదుపరి = మొదటి;
	sd->groups = మొదటి;

	తిరిగి 0;
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
స్థిర శూన్య init_sched_groups_సామర్థ్యం(పూర్ణాంకం cpu, నిర్మాణం sched_domain *sd)
{
	నిర్మాణం sched_సమూహం *sg = sd->groups;
	నిర్మాణం cpumask *ముసుగు = sched_domains_tmpmask2;

	WARN_ON(!sg);

	చేయి {
		పూర్ణాంకం cpu, cores = 0, max_cpu = -1;

		sg->సమూహం_బరువు = cpumask_బరువు(sched_సమూహం_పరిధి(sg));

		cpumask_copy(ముసుగు, sched_సమూహం_పరిధి(sg));
		for_each_cpu(cpu, ముసుగు) {
			cores++;
			cpumask_andnot(ముసుగు, ముసుగు, cpu_smt_ముసుగు(cpu));
		}
		sg->cores = cores;

		ఒకవేళ (!(sd->flags & SD_ASYM_PACKING))
			వెళ్ళు తదుపరి;

		for_each_cpu(cpu, sched_సమూహం_పరిధి(sg)) {
			ఒకవేళ (max_cpu < 0)
				max_cpu = cpu;
			లేకపోతే ఒకవేళ (sched_asym_prefer(cpu, max_cpu))
				max_cpu = cpu;
		}
		sg->asym_prefer_cpu = max_cpu;

తదుపరి:
		sg = sg->తదుపరి;
	} అయితే (sg != sd->groups);

	ఒకవేళ (cpu != సమూహం_balance_cpu(sg))
		తిరిగి;

	update_సమూహం_సామర్థ్యం(sd, cpu);
}

/* Update the "asym_prefer_cpu" when arch_asym_cpu_priority() changes. */
శూన్య sched_update_asym_prefer_cpu(పూర్ణాంకం cpu, పూర్ణాంకం old_prio, పూర్ణాంకం new_prio)
{
	పూర్ణాంకం asym_prefer_cpu = cpu;
	నిర్మాణం sched_domain *sd;

	guard(rcu)();

	for_each_domain(cpu, sd) {
		నిర్మాణం sched_సమూహం *sg;
		పూర్ణాంకం సమూహం_cpu;

		ఒకవేళ (!(sd->flags & SD_ASYM_PACKING))
			కొనసాగు;

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
		ఒకవేళ (cpu != sg->asym_prefer_cpu) {
			/*
			 * Since the parent is a superset of the current group,
			 * if the cpu is not the "asym_prefer_cpu" at the
			 * current level, it cannot be the preferred CPU at a
			 * higher levels either.
			 */
			ఒకవేళ (!sched_asym_prefer(cpu, sg->asym_prefer_cpu))
				తిరిగి;

			WRITE_ONCE(sg->asym_prefer_cpu, cpu);
			కొనసాగు;
		}

		/* Ranking has improved; CPU is still the preferred one. */
		ఒకవేళ (new_prio >= old_prio)
			కొనసాగు;

		for_each_cpu(సమూహం_cpu, sched_సమూహం_పరిధి(sg)) {
			ఒకవేళ (sched_asym_prefer(సమూహం_cpu, asym_prefer_cpu))
				asym_prefer_cpu = సమూహం_cpu;
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
LIST_తల(asym_cap_list);

/*
 * Verify whether there is any CPU capacity asymmetry in a given sched domain.
 * Provides sd_flags reflecting the asymmetry scope.
 */
స్థిర అంతఃవరుస పూర్ణాంకం
asym_cpu_సామర్థ్యం_classify(స్థిరరాశి నిర్మాణం cpumask *sd_పరిధి,
			   స్థిరరాశి నిర్మాణం cpumask *cpu_map)
{
	నిర్మాణం asym_cap_దత్తాంశం *నమోదు_2;
	పూర్ణాంకం లెక్క = 0, తప్పిపోవు = 0;

	/*
	 * Count how many unique CPU capacities this domain spans across
	 * (compare sched_domain CPUs mask with ones representing  available
	 * CPUs capacities). Take into account CPUs that might be offline:
	 * skip those.
	 */
	list_for_each_నమోదు(నమోదు_2, &asym_cap_list, link) {
		ఒకవేళ (cpumask_intersects(sd_పరిధి, cpu_సామర్థ్యం_పరిధి(నమోదు_2)))
			++లెక్క;
		లేకపోతే ఒకవేళ (cpumask_intersects(cpu_map, cpu_సామర్థ్యం_పరిధి(నమోదు_2)))
			++తప్పిపోవు;
	}

	WARN_ON_ONCE(!లెక్క && !list_ఖాళీలు(&asym_cap_list));

	/* No asymmetry detected */
	ఒకవేళ (లెక్క < 2)
		తిరిగి 0;
	/* Some of the available CPU capacity values have not been detected */
	ఒకవేళ (తప్పిపోవు)
		తిరిగి SD_ASYM_CPUCAPACITY;

	/* Full asymmetry */
	తిరిగి SD_ASYM_CPUCAPACITY | SD_ASYM_CPUCAPACITY_నిండిన;

}

స్థిర శూన్య free_asym_cap_నమోదు(నిర్మాణం rcu_తల *తల)
{
	నిర్మాణం asym_cap_దత్తాంశం *నమోదు_2 = container_యొక్క(తల, నిర్మాణం asym_cap_దత్తాంశం, rcu);
	kfree(నమోదు_2);
}

స్థిర అంతఃవరుస శూన్య asym_cpu_సామర్థ్యం_update_దత్తాంశం(పూర్ణాంకం cpu)
{
	గుర్తులేని దీర్ఘ సామర్థ్యం = arch_scale_cpu_సామర్థ్యం(cpu);
	నిర్మాణం asym_cap_దత్తాంశం *చొప్పించు_నమోదు = ఖాళీ;
	నిర్మాణం asym_cap_దత్తాంశం *నమోదు_2;

	/*
	 * Search if capacity already exits. If not, track which the entry
	 * where we should insert to keep the list ordered descending.
	 */
	list_for_each_నమోదు(నమోదు_2, &asym_cap_list, link) {
		ఒకవేళ (సామర్థ్యం == నమోదు_2->సామర్థ్యం)
			వెళ్ళు పూర్తి;
		లేకపోతే ఒకవేళ (!చొప్పించు_నమోదు && సామర్థ్యం > నమోదు_2->సామర్థ్యం)
			చొప్పించు_నమోదు = list_prev_నమోదు(నమోదు_2, link);
	}

	నమోదు_2 = kzalloc(పరిమాణం(*నమోదు_2) + cpumask_కొలత(), GFP_KERNEL);
	ఒకవేళ (WARN_ONCE(!నమోదు_2, "Failed to allocate memory for asymmetry data\n"))
		తిరిగి;
	నమోదు_2->సామర్థ్యం = సామర్థ్యం;

	/* If NULL then the new capacity is the smallest, add last. */
	ఒకవేళ (!చొప్పించు_నమోదు)
		list_కూడు_తోక_rcu(&నమోదు_2->link, &asym_cap_list);
	లేకపోతే
		list_కూడు_rcu(&నమోదు_2->link, &చొప్పించు_నమోదు->link);
పూర్తి:
	__cpumask_set_cpu(cpu, cpu_సామర్థ్యం_పరిధి(నమోదు_2));
}

/*
 * Build-up/update list of CPUs grouped by their capacities
 * An update requires explicit request to rebuild sched domains
 * with state indicating CPU topology changes.
 */
స్థిర శూన్య asym_cpu_సామర్థ్యం_పరిశీలన(శూన్య)
{
	నిర్మాణం asym_cap_దత్తాంశం *నమోదు_2, *తదుపరి;
	పూర్ణాంకం cpu;

	list_for_each_నమోదు(నమోదు_2, &asym_cap_list, link)
		cpumask_clear(cpu_సామర్థ్యం_పరిధి(నమోదు_2));

	for_each_cpu_and(cpu, cpu_possible_ముసుగు, housekeeping_cpumask(HK_TYPE_DOMAIN))
		asym_cpu_సామర్థ్యం_update_దత్తాంశం(cpu);

	list_for_each_నమోదు_సురక్షిత(నమోదు_2, తదుపరి, &asym_cap_list, link) {
		ఒకవేళ (cpumask_ఖాళీలు(cpu_సామర్థ్యం_పరిధి(నమోదు_2))) {
			list_del_rcu(&నమోదు_2->link);
			call_rcu(&నమోదు_2->rcu, free_asym_cap_నమోదు);
		}
	}

	/*
	 * Only one capacity value has been detected i.e. this system is symmetric.
	 * No need to keep this data around.
	 */
	ఒకవేళ (list_is_singular(&asym_cap_list)) {
		నమోదు_2 = list_మొదటి_నమోదు(&asym_cap_list, typeof(*నమోదు_2), link);
		list_del_rcu(&నమోదు_2->link);
		call_rcu(&నమోదు_2->rcu, free_asym_cap_నమోదు);
	}
}

/*
 * Initializers for schedule domains
 * Non-inlined to reduce accumulated stack pressure in build_sched_domains()
 */

స్థిర పూర్ణాంకం default_relax_domain_level = -1;
పూర్ణాంకం sched_domain_level_max;

స్థిర పూర్ణాంకం __init అమరిక_relax_domain_level(అక్షరం *str)
{
	ఒకవేళ (kstrtoint(str, 0, &default_relax_domain_level))
		pr_warn("Unable to set relax_domain_level\n");

	తిరిగి 1;
}
__setup("relax_domain_level=", అమరిక_relax_domain_level);

స్థిర శూన్య సమితి_domain_attribute(నిర్మాణం sched_domain *sd,
				 నిర్మాణం sched_domain_attr *attr)
{
	పూర్ణాంకం request;

	ఒకవేళ (!attr || attr->relax_domain_level < 0) {
		ఒకవేళ (default_relax_domain_level < 0)
			తిరిగి;
		request = default_relax_domain_level;
	} లేకపోతే
		request = attr->relax_domain_level;

	ఒకవేళ (sd->level >= request) {
		/* Turn off idle balance on this domain: */
		sd->flags &= ~(SD_BALANCE_WAKE|SD_BALANCE_NEWIDLE);
	}
}

స్థిర శూన్య __sdt_free(స్థిరరాశి నిర్మాణం cpumask *cpu_map);
స్థిర పూర్ణాంకం __sdt_alloc(స్థిరరాశి నిర్మాణం cpumask *cpu_map);

స్థిర శూన్య __sds_free(నిర్మాణం s_దత్తాంశం *d, స్థిరరాశి నిర్మాణం cpumask *cpu_map);
స్థిర పూర్ణాంకం __sds_alloc(నిర్మాణం s_దత్తాంశం *d, స్థిరరాశి నిర్మాణం cpumask *cpu_map);

స్థిర శూన్య __free_domain_allocs(నిర్మాణం s_దత్తాంశం *d, గణన s_alloc what,
				 స్థిరరాశి నిర్మాణం cpumask *cpu_map)
{
	ఎంపిక (what) {
	స్థితి sa_rootdomain:
		ఒకవేళ (!atomic_read(&d->rd->refcount))
			free_rootdomain(&d->rd->rcu);
		fallthrough;
	స్థితి sa_sd:
		free_percpu(d->sd);
		fallthrough;
	స్థితి sa_sd_shared:
		__sds_free(d, cpu_map);
		fallthrough;
	స్థితి sa_sd_storage:
		__sdt_free(cpu_map);
		fallthrough;
	స్థితి sa_none:
		ఆపు;
	}
}

స్థిర గణన s_alloc
__visit_domain_allocation_hell(నిర్మాణం s_దత్తాంశం *d, స్థిరరాశి నిర్మాణం cpumask *cpu_map)
{
	memset(d, 0, పరిమాణం(*d));

	ఒకవేళ (__sdt_alloc(cpu_map))
		తిరిగి sa_sd_storage;
	ఒకవేళ (__sds_alloc(d, cpu_map))
		తిరిగి sa_sd_shared;
	d->sd = alloc_percpu(నిర్మాణం sched_domain *);
	ఒకవేళ (!d->sd)
		తిరిగి sa_sd_shared;
	d->rd = alloc_rootdomain();
	ఒకవేళ (!d->rd)
		తిరిగి sa_sd;

	తిరిగి sa_rootdomain;
}

/*
 * NULL the sd_data elements we've used to build the sched_domain and
 * sched_group structure so that the subsequent __free_domain_allocs()
 * will not free the data we're using.
 */
స్థిర శూన్య claim_allocations(పూర్ణాంకం cpu, నిర్మాణం s_దత్తాంశం *d)
{
	నిర్మాణం sched_domain *sd;

	ఒకవేళ (atomic_read(&(*చొప్పున_cpu_ptr(d->sds, cpu))->ref))
		*చొప్పున_cpu_ptr(d->sds, cpu) = ఖాళీ;

	కొరకు (sd = *చొప్పున_cpu_ptr(d->sd, cpu); sd; sd = sd->మాతృ) {
		నిర్మాణం sd_దత్తాంశం *sdd = sd->అంతరంగ;

		WARN_ON_ONCE(*చొప్పున_cpu_ptr(sdd->sd, cpu) != sd);
		*చొప్పున_cpu_ptr(sdd->sd, cpu) = ఖాళీ;

		ఒకవేళ (atomic_read(&(*చొప్పున_cpu_ptr(sdd->sg, cpu))->ref))
			*చొప్పున_cpu_ptr(sdd->sg, cpu) = ఖాళీ;

		ఒకవేళ (atomic_read(&(*చొప్పున_cpu_ptr(sdd->sgc, cpu))->ref))
			*చొప్పున_cpu_ptr(sdd->sgc, cpu) = ఖాళీ;
	}
}

#నిర్వచితమైతే CONFIG_NUMA
గణన numa_topology_type sched_numa_topology_type;

/*
 * sched_domains_numa_distance is derived from sched_numa_node_distance
 * and provides a simplified view of NUMA distances used specifically
 * for building NUMA scheduling domains.
 */
స్థిర పూర్ణాంకం			sched_domains_numa_స్థాయిలు;
స్థిర పూర్ణాంకం			sched_numa_కణుపు_స్థాయిలు;

పూర్ణాంకం				sched_max_numa_దూరం;
స్థిర పూర్ణాంకం			*sched_domains_numa_దూరం;
స్థిర పూర్ణాంకం			*sched_numa_కణుపు_దూరం;
స్థిర నిర్మాణం cpumask		***sched_domains_numa_masks;
#అయితే_ముగింపు /* CONFIG_NUMA */

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
#నిర్వచించు TOPOLOGY_SD_FLAGS		\
	(SD_SHARE_CPUCAPACITY	|	\
	 SD_CLUSTER		|	\
	 SD_SHARE_LLC		|	\
	 SD_NUMA		|	\
	 SD_ASYM_PACKING)

స్థిర నిర్మాణం sched_domain *
sd_init(నిర్మాణం sched_domain_topology_level *tl,
	స్థిరరాశి నిర్మాణం cpumask *cpu_map,
	నిర్మాణం sched_domain *బిడ్డ, పూర్ణాంకం cpu)
{
	నిర్మాణం sd_దత్తాంశం *sdd = &tl->దత్తాంశం;
	నిర్మాణం sched_domain *sd = *చొప్పున_cpu_ptr(sdd->sd, cpu);
	పూర్ణాంకం sd_id, sd_బరువు, sd_flags = 0;
	నిర్మాణం cpumask *sd_పరిధి;
	u64 now = sched_clock();

	sd_పరిధి = sched_domain_పరిధి(sd);
	cpumask_and(sd_పరిధి, cpu_map, tl->ముసుగు(tl, cpu));
	sd_బరువు = cpumask_బరువు(sd_పరిధి);
	sd_id = cpumask_మొదటి(sd_పరిధి);

	ఒకవేళ (tl->sd_flags)
		sd_flags = (*tl->sd_flags)();
	ఒకవేళ (WARN_ONCE(sd_flags & ~TOPOLOGY_SD_FLAGS,
		      "wrong sd_flags in topology description\n"))
		sd_flags &= TOPOLOGY_SD_FLAGS;
#నిర్వచితమైతే CONFIG_SCHED_SMT
	ఒకవేళ (sched_smt_asym_packing && (sd_flags & SD_SHARE_CPUCAPACITY))
		sd_flags |= SD_ASYM_PACKING;
#అయితే_ముగింపు
	sd_flags |= asym_cpu_సామర్థ్యం_classify(sd_పరిధి, cpu_map);

	*sd = (నిర్మాణం sched_domain){
		.min_interval		= sd_బరువు,
		.max_interval		= 2*sd_బరువు,
		.busy_గుణకం		= 16,
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

		.చివరి_balance		= jiffies,
		.balance_interval	= sd_బరువు,

		/* 50% success rate */
		.newidle_call		= 512,
		.newidle_success	= 256,
		.newidle_నిష్పత్తి		= 512,
		.newidle_stamp		= now,

		.max_newidle_lb_ఖర్చు	= 0,
		.చివరి_decay_max_lb_ఖర్చు	= jiffies,
		.బిడ్డ			= బిడ్డ,
		.name			= tl->name,
	};

	WARN_ONCE((sd->flags & (SD_SHARE_CPUCAPACITY | SD_ASYM_CPUCAPACITY)) ==
		  (SD_SHARE_CPUCAPACITY | SD_ASYM_CPUCAPACITY),
		  "CPU capacity asymmetry not supported on SMT\n");

	/*
	 * Convert topological properties into behaviour.
	 */
	ఒకవేళ (sd->flags & SD_SHARE_CPUCAPACITY) {
		sd->imbalance_pct = 110;

	} లేకపోతే ఒకవేళ (sd->flags & SD_SHARE_LLC) {
		sd->imbalance_pct = 117;
		sd->cache_nice_tries = 1;

#నిర్వచితమైతే CONFIG_NUMA
	} లేకపోతే ఒకవేళ (sd->flags & SD_NUMA) {
		sd->cache_nice_tries = 2;

		sd->flags &= ~SD_PREFER_SIBLING;
		sd->flags |= SD_SERIALIZE;
		ఒకవేళ (sched_domains_numa_దూరం[tl->numa_level] > కణుపు_reclaim_దూరం) {
			sd->flags &= ~(SD_BALANCE_EXEC |
				       SD_BALANCE_FORK |
				       SD_WAKE_AFFINE);
		}

#అయితే_ముగింపు /* CONFIG_NUMA */
	} లేకపోతే {
		sd->cache_nice_tries = 1;
	}

	sd->అంతరంగ = sdd;

	తిరిగి sd;
}

#నిర్వచితమైతే CONFIG_SCHED_SMT
పూర్ణాంకం cpu_smt_flags(శూన్య)
{
	తిరిగి SD_SHARE_CPUCAPACITY | SD_SHARE_LLC;
}

స్థిరరాశి నిర్మాణం cpumask *tl_smt_ముసుగు(నిర్మాణం sched_domain_topology_level *tl, పూర్ణాంకం cpu)
{
	తిరిగి cpu_smt_ముసుగు(cpu);
}
#అయితే_ముగింపు

#నిర్వచితమైతే CONFIG_SCHED_CLUSTER
పూర్ణాంకం cpu_cluster_flags(శూన్య)
{
	తిరిగి SD_CLUSTER | SD_SHARE_LLC;
}

స్థిరరాశి నిర్మాణం cpumask *tl_cls_ముసుగు(నిర్మాణం sched_domain_topology_level *tl, పూర్ణాంకం cpu)
{
	తిరిగి cpu_clustergroup_ముసుగు(cpu);
}
#అయితే_ముగింపు

#నిర్వచితమైతే CONFIG_SCHED_MC
పూర్ణాంకం cpu_core_flags(శూన్య)
{
	తిరిగి SD_SHARE_LLC;
}

స్థిరరాశి నిర్మాణం cpumask *tl_mc_ముసుగు(నిర్మాణం sched_domain_topology_level *tl, పూర్ణాంకం cpu)
{
	తిరిగి cpu_coregroup_ముసుగు(cpu);
}

/*
 * Majority of architectures have LLC at MC domain level with exception
 * such as powerpc. Provide a way for arch to specify where its LLC is
 * if it falls in exception category
 */
# ifndef arch_llc_ముసుగు
#నిర్వచించు arch_llc_ముసుగు(cpu) cpu_coregroup_ముసుగు(cpu)
# endif

#లేకపోతే
#నిర్వచించు arch_llc_ముసుగు(cpu) cpumask_యొక్క(cpu)
#అయితే_ముగింపు

#నిర్వచించు llc_ముసుగు(cpu) arch_llc_ముసుగు(cpu)

స్థిరరాశి నిర్మాణం cpumask *tl_pkg_ముసుగు(నిర్మాణం sched_domain_topology_level *tl, పూర్ణాంకం cpu)
{
	తిరిగి cpu_కణుపు_ముసుగు(cpu);
}

/*
 * Topology list, bottom-up.
 */
స్థిర నిర్మాణం sched_domain_topology_level default_topology[] = {
#నిర్వచితమైతే CONFIG_SCHED_SMT
	SDTL_INIT(tl_smt_ముసుగు, cpu_smt_flags, SMT),
#అయితే_ముగింపు

#నిర్వచితమైతే CONFIG_SCHED_CLUSTER
	SDTL_INIT(tl_cls_ముసుగు, cpu_cluster_flags, CLS),
#అయితే_ముగింపు

#నిర్వచితమైతే CONFIG_SCHED_MC
	SDTL_INIT(tl_mc_ముసుగు, cpu_core_flags, MC),
#అయితే_ముగింపు
	SDTL_INIT(tl_pkg_ముసుగు, ఖాళీ, PKG),
	{ ఖాళీ, },
};

స్థిర నిర్మాణం sched_domain_topology_level *sched_domain_topology =
	default_topology;
స్థిర నిర్మాణం sched_domain_topology_level *sched_domain_topology_saved;

#నిర్వచించు for_each_sd_topology(tl)			\
	కొరకు (tl = sched_domain_topology; tl->ముసుగు; tl++)

శూన్య __init సమితి_sched_topology(నిర్మాణం sched_domain_topology_level *tl)
{
	ఒకవేళ (WARN_ON_ONCE(sched_smp_initialized))
		తిరిగి;

	sched_domain_topology = tl;
	sched_domain_topology_saved = ఖాళీ;
}

#నిర్వచితమైతే CONFIG_NUMA
స్థిర పూర్ణాంకం cpu_numa_flags(శూన్య)
{
	తిరిగి SD_NUMA;
}

స్థిర స్థిరరాశి నిర్మాణం cpumask *sd_numa_ముసుగు(నిర్మాణం sched_domain_topology_level *tl, పూర్ణాంకం cpu)
{
	తిరిగి sched_domains_numa_masks[tl->numa_level][cpu_గమ్యం_కణుపు(cpu)];
}

స్థిర శూన్య sched_numa_warn(స్థిరరాశి అక్షరం *str)
{
	స్థిర పూర్ణాంకం పూర్తి = false;
	పూర్ణాంకం i,j;

	ఒకవేళ (పూర్తి)
		తిరిగి;

	పూర్తి = true;

	printk(KERN_WARNING "ERROR: %s\n\n", str);

	కొరకు (i = 0; i < nr_కణుపు_ids; i++) {
		printk(KERN_WARNING "  ");
		కొరకు (j = 0; j < nr_కణుపు_ids; j++) {
			ఒకవేళ (!కణుపు_state(i, N_CPU) || !కణుపు_state(j, N_CPU))
				printk(KERN_CONT "(%02d) ", కణుపు_దూరం(i,j));
			లేకపోతే
				printk(KERN_CONT " %02d  ", కణుపు_దూరం(i,j));
		}
		printk(KERN_CONT "\n");
	}
	printk(KERN_WARNING "\n");
}

bool కనుగొను_numa_దూరం(పూర్ణాంకం దూరం)
{
	bool దొరికింది = false;
	పూర్ణాంకం i, *distances;

	ఒకవేళ (దూరం == కణుపు_దూరం(0, 0))
		తిరిగి true;

	rcu_read_తాళం();
	distances = rcu_dereference(sched_numa_కణుపు_దూరం);
	ఒకవేళ (!distances)
		వెళ్ళు unlock;
	కొరకు (i = 0; i < sched_numa_కణుపు_స్థాయిలు; i++) {
		ఒకవేళ (distances[i] == దూరం) {
			దొరికింది = true;
			ఆపు;
		}
	}
unlock:
	rcu_read_unlock();

	తిరిగి దొరికింది;
}

#నిర్వచించు for_each_cpu_కణుపు_but(n, nbut)		\
	for_each_కణుపు_state(n, N_CPU)		\
		ఒకవేళ (n == nbut)			\
			కొనసాగు;		\
		లేకపోతే

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
స్థిర శూన్య init_numa_topology_type(పూర్ణాంకం offline_కణుపు)
{
	పూర్ణాంకం a, b, c, n;

	n = sched_max_numa_దూరం;

	ఒకవేళ (sched_domains_numa_స్థాయిలు <= 2) {
		sched_numa_topology_type = NUMA_ప్రత్యక్ష;
		తిరిగి;
	}

	for_each_cpu_కణుపు_but(a, offline_కణుపు) {
		for_each_cpu_కణుపు_but(b, offline_కణుపు) {
			/* Find two nodes furthest removed from each other. */
			ఒకవేళ (కణుపు_దూరం(a, b) < n)
				కొనసాగు;

			/* Is there an intermediary node between a and b? */
			for_each_cpu_కణుపు_but(c, offline_కణుపు) {
				ఒకవేళ (కణుపు_దూరం(a, c) < n &&
				    కణుపు_దూరం(b, c) < n) {
					sched_numa_topology_type =
							NUMA_GLUELESS_MESH;
					తిరిగి;
				}
			}

			sched_numa_topology_type = NUMA_BACKPLANE;
			తిరిగి;
		}
	}

	pr_err("Failed to find a NUMA topology type, defaulting to DIRECT\n");
	sched_numa_topology_type = NUMA_ప్రత్యక్ష;
}


#నిర్వచించు NR_దూరం_విలువలు (1 << దూరం_బిట్లు)

/*
 * An architecture could modify its NUMA distance, to change
 * grouping of NUMA nodes and number of NUMA levels when creating
 * NUMA level sched domains.
 *
 * A NUMA level is created for each unique
 * arch_sched_node_distance.
 */
స్థిర పూర్ణాంకం numa_కణుపు_dist(పూర్ణాంకం i, పూర్ణాంకం j)
{
	తిరిగి కణుపు_దూరం(i, j);
}

పూర్ణాంకం arch_sched_కణుపు_దూరం(పూర్ణాంకం from, పూర్ణాంకం గమ్యం)
			     __weak __alias(numa_కణుపు_dist);

స్థిర bool modified_sched_కణుపు_దూరం(శూన్య)
{
	తిరిగి numa_కణుపు_dist != arch_sched_కణుపు_దూరం;
}

స్థిర పూర్ణాంకం sched_రికార్డు_numa_dist(పూర్ణాంకం offline_కణుపు, పూర్ణాంకం (*n_dist)(పూర్ణాంకం, పూర్ణాంకం),
				  పూర్ణాంకం **dist, పూర్ణాంకం *స్థాయిలు)
{
	గుర్తులేని దీర్ఘ *దూరం_map __free(bitmap) = ఖాళీ;
	పూర్ణాంకం nr_స్థాయిలు = 0;
	పూర్ణాంకం i, j;
	పూర్ణాంకం *distances;

	/*
	 * O(nr_nodes^2) de-duplicating selection sort -- in order to find the
	 * unique distances in the node_distance() table.
	 */
	దూరం_map = bitmap_alloc(NR_దూరం_విలువలు, GFP_KERNEL);
	ఒకవేళ (!దూరం_map)
		తిరిగి -ENOMEM;

	bitmap_zero(దూరం_map, NR_దూరం_విలువలు);
	for_each_cpu_కణుపు_but(i, offline_కణుపు) {
		for_each_cpu_కణుపు_but(j, offline_కణుపు) {
			పూర్ణాంకం దూరం = n_dist(i, j);

			ఒకవేళ (దూరం < LOCAL_దూరం || దూరం >= NR_దూరం_విలువలు) {
				sched_numa_warn("Invalid distance value range");
				తిరిగి -EINVAL;
			}

			bitmap_సమితి(దూరం_map, దూరం, 1);
		}
	}
	/*
	 * We can now figure out how many unique distance values there are and
	 * allocate memory accordingly.
	 */
	nr_స్థాయిలు = bitmap_బరువు(దూరం_map, NR_దూరం_విలువలు);

	distances = kzalloc_objs(పూర్ణాంకం, nr_స్థాయిలు);
	ఒకవేళ (!distances)
		తిరిగి -ENOMEM;

	కొరకు (i = 0, j = 0; i < nr_స్థాయిలు; i++, j++) {
		j = కనుగొను_తదుపరి_బిట్టు(దూరం_map, NR_దూరం_విలువలు, j);
		distances[i] = j;
	}
	*dist = distances;
	*స్థాయిలు = nr_స్థాయిలు;

	తిరిగి 0;
}

శూన్య sched_init_numa(పూర్ణాంకం offline_కణుపు)
{
	నిర్మాణం sched_domain_topology_level *tl;
	పూర్ణాంకం nr_స్థాయిలు, nr_కణుపు_స్థాయిలు;
	పూర్ణాంకం i, j;
	పూర్ణాంకం *distances, *domain_distances;
	నిర్మాణం cpumask ***masks;

	/* Record the NUMA distances from SLIT table */
	ఒకవేళ (sched_రికార్డు_numa_dist(offline_కణుపు, numa_కణుపు_dist, &distances,
				   &nr_కణుపు_స్థాయిలు))
		తిరిగి;

	/* Record modified NUMA distances for building sched domains */
	ఒకవేళ (modified_sched_కణుపు_దూరం()) {
		ఒకవేళ (sched_రికార్డు_numa_dist(offline_కణుపు, arch_sched_కణుపు_దూరం,
					   &domain_distances, &nr_స్థాయిలు)) {
			kfree(distances);
			తిరిగి;
		}
	} లేకపోతే {
		domain_distances = distances;
		nr_స్థాయిలు = nr_కణుపు_స్థాయిలు;
	}
	rcu_కేటాయించు_సూచిక(sched_numa_కణుపు_దూరం, distances);
	WRITE_ONCE(sched_max_numa_దూరం, distances[nr_కణుపు_స్థాయిలు - 1]);
	WRITE_ONCE(sched_numa_కణుపు_స్థాయిలు, nr_కణుపు_స్థాయిలు);

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
	rcu_కేటాయించు_సూచిక(sched_domains_numa_దూరం, domain_distances);

	sched_domains_numa_స్థాయిలు = 0;

	masks = kzalloc(పరిమాణం(శూన్య *) * nr_స్థాయిలు, GFP_KERNEL);
	ఒకవేళ (!masks)
		తిరిగి;

	/*
	 * Now for each level, construct a mask per node which contains all
	 * CPUs of nodes that are that many hops away from us.
	 */
	కొరకు (i = 0; i < nr_స్థాయిలు; i++) {
		masks[i] = kzalloc(nr_కణుపు_ids * పరిమాణం(శూన్య *), GFP_KERNEL);
		ఒకవేళ (!masks[i])
			తిరిగి;

		for_each_cpu_కణుపు_but(j, offline_కణుపు) {
			నిర్మాణం cpumask *ముసుగు = kzalloc(cpumask_కొలత(), GFP_KERNEL);
			పూర్ణాంకం k;

			ఒకవేళ (!ముసుగు)
				తిరిగి;

			masks[i][j] = ముసుగు;

			for_each_cpu_కణుపు_but(k, offline_కణుపు) {
				ఒకవేళ (sched_debug() &&
				    (arch_sched_కణుపు_దూరం(j, k) !=
				     arch_sched_కణుపు_దూరం(k, j)))
					sched_numa_warn("Node-distance not symmetric");

				ఒకవేళ (arch_sched_కణుపు_దూరం(j, k) >
				    sched_domains_numa_దూరం[i])
					కొనసాగు;

				cpumask_or(ముసుగు, ముసుగు, cpumask_యొక్క_కణుపు(k));
			}
		}
	}
	rcu_కేటాయించు_సూచిక(sched_domains_numa_masks, masks);

	/* Compute default topology size */
	కొరకు (i = 0; sched_domain_topology[i].ముసుగు; i++);

	tl = kzalloc((i + nr_స్థాయిలు + 1) *
			పరిమాణం(నిర్మాణం sched_domain_topology_level), GFP_KERNEL);
	ఒకవేళ (!tl)
		తిరిగి;

	/*
	 * Copy the default topology bits..
	 */
	కొరకు (i = 0; sched_domain_topology[i].ముసుగు; i++)
		tl[i] = sched_domain_topology[i];

	/*
	 * Add the NUMA identity distance, aka single NODE.
	 */
	tl[i++] = SDTL_INIT(sd_numa_ముసుగు, ఖాళీ, కణుపు_2);

	/*
	 * .. and append 'j' levels of NUMA goodness.
	 */
	కొరకు (j = 1; j < nr_స్థాయిలు; i++, j++) {
		tl[i] = SDTL_INIT(sd_numa_ముసుగు, cpu_numa_flags, NUMA);
		tl[i].numa_level = j;
	}

	sched_domain_topology_saved = sched_domain_topology;
	sched_domain_topology = tl;

	sched_domains_numa_స్థాయిలు = nr_స్థాయిలు;

	init_numa_topology_type(offline_కణుపు);
}


స్థిర శూన్య sched_reset_numa(శూన్య)
{
	పూర్ణాంకం nr_స్థాయిలు, *distances, *dom_distances = ఖాళీ;
	నిర్మాణం cpumask ***masks;

	nr_స్థాయిలు = sched_domains_numa_స్థాయిలు;
	sched_numa_కణుపు_స్థాయిలు = 0;
	sched_domains_numa_స్థాయిలు = 0;
	sched_max_numa_దూరం = 0;
	sched_numa_topology_type = NUMA_ప్రత్యక్ష;
	distances = sched_numa_కణుపు_దూరం;
	ఒకవేళ (sched_numa_కణుపు_దూరం != sched_domains_numa_దూరం)
		dom_distances = sched_domains_numa_దూరం;
	rcu_కేటాయించు_సూచిక(sched_numa_కణుపు_దూరం, ఖాళీ);
	rcu_కేటాయించు_సూచిక(sched_domains_numa_దూరం, ఖాళీ);
	masks = sched_domains_numa_masks;
	rcu_కేటాయించు_సూచిక(sched_domains_numa_masks, ఖాళీ);
	ఒకవేళ (distances || masks) {
		పూర్ణాంకం i, j;

		synchronize_rcu();
		kfree(distances);
		kfree(dom_distances);
		కొరకు (i = 0; i < nr_స్థాయిలు && masks; i++) {
			ఒకవేళ (!masks[i])
				కొనసాగు;
			for_each_కణుపు(j)
				kfree(masks[i][j]);
			kfree(masks[i]);
		}
		kfree(masks);
	}
	ఒకవేళ (sched_domain_topology_saved) {
		kfree(sched_domain_topology);
		sched_domain_topology = sched_domain_topology_saved;
		sched_domain_topology_saved = ఖాళీ;
	}
}

/*
 * Call with hotplug lock held
 */
శూన్య sched_update_numa(పూర్ణాంకం cpu, bool online)
{
	పూర్ణాంకం కణుపు;

	కణుపు = cpu_గమ్యం_కణుపు(cpu);
	/*
	 * Scheduler NUMA topology is updated when the first CPU of a
	 * node is onlined or the last CPU of a node is offlined.
	 */
	ఒకవేళ (cpumask_బరువు(cpumask_యొక్క_కణుపు(కణుపు)) != 1)
		తిరిగి;

	sched_reset_numa();
	sched_init_numa(online ? NUMA_NO_కణుపు : కణుపు);
}

శూన్య sched_domains_numa_masks_సమితి(గుర్తులేని పూర్ణాంకం cpu)
{
	పూర్ణాంకం కణుపు = cpu_గమ్యం_కణుపు(cpu);
	పూర్ణాంకం i, j;

	కొరకు (i = 0; i < sched_domains_numa_స్థాయిలు; i++) {
		కొరకు (j = 0; j < nr_కణుపు_ids; j++) {
			ఒకవేళ (!కణుపు_state(j, N_CPU))
				కొనసాగు;

			/* Set ourselves in the remote node's masks */
			ఒకవేళ (arch_sched_కణుపు_దూరం(j, కణుపు) <=
			    sched_domains_numa_దూరం[i])
				cpumask_సమితి_cpu(cpu, sched_domains_numa_masks[i][j]);
		}
	}
}

శూన్య sched_domains_numa_masks_clear(గుర్తులేని పూర్ణాంకం cpu)
{
	పూర్ణాంకం i, j;

	కొరకు (i = 0; i < sched_domains_numa_స్థాయిలు; i++) {
		కొరకు (j = 0; j < nr_కణుపు_ids; j++) {
			ఒకవేళ (sched_domains_numa_masks[i][j])
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
పూర్ణాంకం sched_numa_కనుగొను_closest(స్థిరరాశి నిర్మాణం cpumask *cpus, పూర్ణాంకం cpu)
{
	పూర్ణాంకం i, j = cpu_గమ్యం_కణుపు(cpu), దొరికింది = nr_cpu_ids;
	నిర్మాణం cpumask ***masks;

	rcu_read_తాళం();
	masks = rcu_dereference(sched_domains_numa_masks);
	ఒకవేళ (!masks)
		వెళ్ళు unlock;
	కొరకు (i = 0; i < sched_domains_numa_స్థాయిలు; i++) {
		ఒకవేళ (!masks[i][j])
			ఆపు;
		cpu = cpumask_any_and_distribute(cpus, masks[i][j]);
		ఒకవేళ (cpu < nr_cpu_ids) {
			దొరికింది = cpu;
			ఆపు;
		}
	}
unlock:
	rcu_read_unlock();

	తిరిగి దొరికింది;
}

నిర్మాణం __cmp_key {
	స్థిరరాశి నిర్మాణం cpumask *cpus;
	నిర్మాణం cpumask ***masks;
	పూర్ణాంకం కణుపు;
	పూర్ణాంకం cpu;
	పూర్ణాంకం w;
};

స్థిర పూర్ణాంకం hop_cmp(స్థిరరాశి శూన్య *a, స్థిరరాశి శూన్య *b)
{
	నిర్మాణం cpumask **prev_hop, **cur_hop = *(నిర్మాణం cpumask ***)b;
	నిర్మాణం __cmp_key *k = (నిర్మాణం __cmp_key *)a;

	ఒకవేళ (cpumask_బరువు_and(k->cpus, cur_hop[k->కణుపు]) <= k->cpu)
		తిరిగి 1;

	ఒకవేళ (b == k->masks) {
		k->w = 0;
		తిరిగి 0;
	}

	prev_hop = *((నిర్మాణం cpumask ***)b - 1);
	k->w = cpumask_బరువు_and(k->cpus, prev_hop[k->కణుపు]);
	ఒకవేళ (k->w <= k->cpu)
		తిరిగి 0;

	తిరిగి -1;
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
పూర్ణాంకం sched_numa_కనుగొను_nth_cpu(స్థిరరాశి నిర్మాణం cpumask *cpus, పూర్ణాంకం cpu, పూర్ణాంకం కణుపు)
{
	నిర్మాణం __cmp_key k = { .cpus = cpus, .cpu = cpu };
	నిర్మాణం cpumask ***hop_masks;
	పూర్ణాంకం hop, ret = nr_cpu_ids;

	ఒకవేళ (కణుపు == NUMA_NO_కణుపు)
		తిరిగి cpumask_nth_and(cpu, cpus, cpu_online_ముసుగు);

	rcu_read_తాళం();

	/* CPU-less node entries are uninitialized in sched_domains_numa_masks */
	కణుపు = numa_సమీప_కణుపు(కణుపు, N_CPU);
	k.కణుపు = కణుపు;

	k.masks = rcu_dereference(sched_domains_numa_masks);
	ఒకవేళ (!k.masks)
		వెళ్ళు unlock;

	hop_masks = bsearch(&k, k.masks, sched_domains_numa_స్థాయిలు, పరిమాణం(k.masks[0]), hop_cmp);
	ఒకవేళ (!hop_masks)
		వెళ్ళు unlock;
	hop = hop_masks	- k.masks;

	ret = hop ?
		cpumask_nth_and_andnot(cpu - k.w, cpus, k.masks[hop][కణుపు], k.masks[hop-1][కణుపు]) :
		cpumask_nth_and(cpu, cpus, k.masks[0][కణుపు]);
unlock:
	rcu_read_unlock();
	తిరిగి ret;
}
EXPORT_చిహ్నం_GPL(sched_numa_కనుగొను_nth_cpu);

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
స్థిరరాశి నిర్మాణం cpumask *sched_numa_hop_ముసుగు(గుర్తులేని పూర్ణాంకం కణుపు, గుర్తులేని పూర్ణాంకం hops)
{
	నిర్మాణం cpumask ***masks;

	ఒకవేళ (కణుపు >= nr_కణుపు_ids || hops >= sched_domains_numa_స్థాయిలు)
		తిరిగి ERR_PTR(-EINVAL);

	masks = rcu_dereference(sched_domains_numa_masks);
	ఒకవేళ (!masks)
		తిరిగి ERR_PTR(-EBUSY);

	తిరిగి masks[hops][కణుపు];
}
EXPORT_చిహ్నం_GPL(sched_numa_hop_ముసుగు);

#అయితే_ముగింపు /* CONFIG_NUMA */

స్థిర పూర్ణాంకం __sdt_alloc(స్థిరరాశి నిర్మాణం cpumask *cpu_map)
{
	నిర్మాణం sched_domain_topology_level *tl;
	పూర్ణాంకం j;

	for_each_sd_topology(tl) {
		నిర్మాణం sd_దత్తాంశం *sdd = &tl->దత్తాంశం;

		sdd->sd = alloc_percpu(నిర్మాణం sched_domain *);
		ఒకవేళ (!sdd->sd)
			తిరిగి -ENOMEM;

		sdd->sg = alloc_percpu(నిర్మాణం sched_సమూహం *);
		ఒకవేళ (!sdd->sg)
			తిరిగి -ENOMEM;

		sdd->sgc = alloc_percpu(నిర్మాణం sched_సమూహం_సామర్థ్యం *);
		ఒకవేళ (!sdd->sgc)
			తిరిగి -ENOMEM;

		for_each_cpu(j, cpu_map) {
			నిర్మాణం sched_domain *sd;
			నిర్మాణం sched_సమూహం *sg;
			నిర్మాణం sched_సమూహం_సామర్థ్యం *sgc;

			sd = kzalloc_కణుపు(పరిమాణం(నిర్మాణం sched_domain) + cpumask_కొలత(),
					GFP_KERNEL, cpu_గమ్యం_కణుపు(j));
			ఒకవేళ (!sd)
				తిరిగి -ENOMEM;

			*చొప్పున_cpu_ptr(sdd->sd, j) = sd;

			sg = kzalloc_కణుపు(పరిమాణం(నిర్మాణం sched_సమూహం) + cpumask_కొలత(),
					GFP_KERNEL, cpu_గమ్యం_కణుపు(j));
			ఒకవేళ (!sg)
				తిరిగి -ENOMEM;

			sg->తదుపరి = sg;

			*చొప్పున_cpu_ptr(sdd->sg, j) = sg;

			sgc = kzalloc_కణుపు(పరిమాణం(నిర్మాణం sched_సమూహం_సామర్థ్యం) + cpumask_కొలత(),
					GFP_KERNEL, cpu_గమ్యం_కణుపు(j));
			ఒకవేళ (!sgc)
				తిరిగి -ENOMEM;

			sgc->id = j;

			*చొప్పున_cpu_ptr(sdd->sgc, j) = sgc;
		}
	}

	తిరిగి 0;
}

స్థిర శూన్య __sdt_free(స్థిరరాశి నిర్మాణం cpumask *cpu_map)
{
	నిర్మాణం sched_domain_topology_level *tl;
	పూర్ణాంకం j;

	for_each_sd_topology(tl) {
		నిర్మాణం sd_దత్తాంశం *sdd = &tl->దత్తాంశం;

		for_each_cpu(j, cpu_map) {
			నిర్మాణం sched_domain *sd;

			ఒకవేళ (sdd->sd) {
				sd = *చొప్పున_cpu_ptr(sdd->sd, j);
				ఒకవేళ (sd && (sd->flags & SD_NUMA))
					free_sched_groups(sd->groups, 0);
				kfree(*చొప్పున_cpu_ptr(sdd->sd, j));
			}

			ఒకవేళ (sdd->sg)
				kfree(*చొప్పున_cpu_ptr(sdd->sg, j));
			ఒకవేళ (sdd->sgc)
				kfree(*చొప్పున_cpu_ptr(sdd->sgc, j));
		}
		free_percpu(sdd->sd);
		sdd->sd = ఖాళీ;
		free_percpu(sdd->sg);
		sdd->sg = ఖాళీ;
		free_percpu(sdd->sgc);
		sdd->sgc = ఖాళీ;
	}
}

స్థిర పూర్ణాంకం __sds_alloc(నిర్మాణం s_దత్తాంశం *d, స్థిరరాశి నిర్మాణం cpumask *cpu_map)
{
	పూర్ణాంకం j;

	d->sds = alloc_percpu(నిర్మాణం sched_domain_shared *);
	ఒకవేళ (!d->sds)
		తిరిగి -ENOMEM;

	for_each_cpu(j, cpu_map) {
		నిర్మాణం sched_domain_shared *sds;

		sds = kzalloc_కణుపు(పరిమాణం(నిర్మాణం sched_domain_shared),
				GFP_KERNEL, cpu_గమ్యం_కణుపు(j));
		ఒకవేళ (!sds)
			తిరిగి -ENOMEM;

		*చొప్పున_cpu_ptr(d->sds, j) = sds;
	}

	తిరిగి 0;
}

స్థిర శూన్య __sds_free(నిర్మాణం s_దత్తాంశం *d, స్థిరరాశి నిర్మాణం cpumask *cpu_map)
{
	పూర్ణాంకం j;

	ఒకవేళ (!d->sds)
		తిరిగి;

	for_each_cpu(j, cpu_map)
		kfree(*చొప్పున_cpu_ptr(d->sds, j));

	free_percpu(d->sds);
	d->sds = ఖాళీ;
}

స్థిర నిర్మాణం sched_domain *build_sched_domain(నిర్మాణం sched_domain_topology_level *tl,
		స్థిరరాశి నిర్మాణం cpumask *cpu_map, నిర్మాణం sched_domain_attr *attr,
		నిర్మాణం sched_domain *బిడ్డ, పూర్ణాంకం cpu)
{
	నిర్మాణం sched_domain *sd = sd_init(tl, cpu_map, బిడ్డ, cpu);

	ఒకవేళ (బిడ్డ) {
		sd->level = బిడ్డ->level + 1;
		sched_domain_level_max = max(sched_domain_level_max, sd->level);
		బిడ్డ->మాతృ = sd;

		ఒకవేళ (!cpumask_subset(sched_domain_పరిధి(బిడ్డ),
				    sched_domain_పరిధి(sd))) {
			pr_err("BUG: arch topology borken\n");
			pr_err("     the %s domain not a subset of the %s domain\n",
					బిడ్డ->name, sd->name);
			/* Fixup, ensure @sd has at least @child CPUs. */
			cpumask_or(sched_domain_పరిధి(sd),
				   sched_domain_పరిధి(sd),
				   sched_domain_పరిధి(బిడ్డ));
		}

	}
	సమితి_domain_attribute(sd, attr);

	తిరిగి sd;
}

/*
 * Ensure topology masks are sane, i.e. there are no conflicts (overlaps) for
 * any two given CPUs on non-NUMA topology levels.
 */
స్థిర bool topology_పరిధి_sane(స్థిరరాశి నిర్మాణం cpumask *cpu_map)
{
	నిర్మాణం sched_domain_topology_level *tl;
	నిర్మాణం cpumask *covered, *id_చూసిన;
	పూర్ణాంకం cpu;

	lockdep_assert_held(&sched_domains_mutex);
	covered = sched_domains_tmpmask;
	id_చూసిన = sched_domains_tmpmask2;

	for_each_sd_topology(tl) {
		పూర్ణాంకం tl_common_flags = 0;

		ఒకవేళ (tl->sd_flags)
			tl_common_flags = (*tl->sd_flags)();

		/* NUMA levels are allowed to overlap */
		ఒకవేళ (tl_common_flags & SD_NUMA)
			కొనసాగు;

		cpumask_clear(covered);
		cpumask_clear(id_చూసిన);

		/*
		 * Non-NUMA levels cannot partially overlap - they must be either
		 * completely equal or completely disjoint. Otherwise we can end up
		 * breaking the sched_group lists - i.e. a later get_group() pass
		 * breaks the linking done for an earlier span.
		 */
		for_each_cpu(cpu, cpu_map) {
			స్థిరరాశి నిర్మాణం cpumask *tl_cpu_ముసుగు = tl->ముసుగు(tl, cpu);
			పూర్ణాంకం id;

			/* lowest bit set in this mask is used as a unique id */
			id = cpumask_మొదటి(tl_cpu_ముసుగు);

			ఒకవేళ (cpumask_test_cpu(id, id_చూసిన)) {
				/* First CPU has already been seen, ensure identical spans */
				ఒకవేళ (!cpumask_equal(tl->ముసుగు(tl, id), tl_cpu_ముసుగు))
					తిరిగి false;
			} లేకపోతే {
				/* First CPU hasn't been seen before, ensure it's a completely new span */
				ఒకవేళ (cpumask_intersects(tl_cpu_ముసుగు, covered))
					తిరిగి false;

				cpumask_or(covered, covered, tl_cpu_ముసుగు);
				cpumask_సమితి_cpu(id, id_చూసిన);
			}
		}
	}
	తిరిగి true;
}

/*
 * Calculate an allowed NUMA imbalance such that LLCs do not get
 * imbalanced.
 */
స్థిర శూన్య adjust_numa_imbalance(నిర్మాణం sched_domain *sd_llc)
{
	నిర్మాణం sched_domain *మాతృ;
	గుర్తులేని పూర్ణాంకం imb_పరిధి = 1;
	గుర్తులేని పూర్ణాంకం imb = 0;
	గుర్తులేని పూర్ణాంకం nr_llcs;

	WARN_ON(!(sd_llc->flags & SD_SHARE_LLC));
	WARN_ON(!sd_llc->మాతృ);

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
	nr_llcs = sd_llc->మాతృ->పరిధి_బరువు / sd_llc->పరిధి_బరువు;
	ఒకవేళ (nr_llcs == 1)
		imb = sd_llc->మాతృ->పరిధి_బరువు >> 3;
	లేకపోతే
		imb = nr_llcs;

	imb = max(1U, imb);
	sd_llc->మాతృ->imb_numa_nr = imb;

	/*
	 * Set span based on the first NUMA domain.
	 *
	 * NUMA systems always add a NODE domain before
	 * iterating the NUMA domains. Since this is before
	 * degeneration, start from sd_llc's parent's
	 * parent which is the lowest an SD_NUMA domain can
	 * be relative to sd_llc.
	 */
	మాతృ = sd_llc->మాతృ->మాతృ;
	అయితే (మాతృ && !(మాతృ->flags & SD_NUMA))
		మాతృ = మాతృ->మాతృ;

	imb_పరిధి = మాతృ ? మాతృ->పరిధి_బరువు : sd_llc->మాతృ->పరిధి_బరువు;

	/* Update the upper remainder of the topology */
	మాతృ = sd_llc->మాతృ;
	అయితే (మాతృ) {
		పూర్ణాంకం గుణకం = max(1U, (మాతృ->పరిధి_బరువు / imb_పరిధి));

		మాతృ->imb_numa_nr = imb * గుణకం;
		మాతృ = మాతృ->మాతృ;
	}
}

స్థిర శూన్య
init_sched_domain_shared(నిర్మాణం s_దత్తాంశం *d, నిర్మాణం sched_domain *sd, పూర్ణాంకం flags)
{
	నిర్మాణం sched_domain_shared *sds = ఖాళీ;
	పూర్ణాంకం cpu;

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
	for_each_cpu(cpu, sched_domain_పరిధి(sd)) {
		sds = *చొప్పున_cpu_ptr(d->sds, cpu);

		ఒకవేళ (!sds->alloc_flags ||
		    sd->పరిధి_బరువు == 1 ||
		    sds->alloc_flags == flags) {
			sds->alloc_flags = flags;
			sd->shared = sds;
			ఆపు;
		}
	}

	/*
	 * Use the sd_shared corresponding to the last
	 * CPU in the span if none are avaialable.
	 */
	ఒకవేళ (WARN_ON_ONCE(!sd->shared))
		sd->shared = sds;

	/*
	 * nr_busy_cpus is consumed only by the NOHZ kick path via
	 * sd_balance_shared; on the asym-capacity path it is initialized but
	 * never read.
	 */
	atomic_సమితి(&sd->shared->nr_busy_cpus, sd->పరిధి_బరువు);
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
స్థిర bool claim_asym_sched_domain_shared(నిర్మాణం s_దత్తాంశం *d, పూర్ణాంకం cpu)
{
	నిర్మాణం sched_domain *sd = *చొప్పున_cpu_ptr(d->sd, cpu);
	నిర్మాణం sched_domain *sd_asym;

	ఒకవేళ (!sd)
		తిరిగి false;

	sd_asym = sd;
	అయితే (sd_asym && !(sd_asym->flags & SD_ASYM_CPUCAPACITY_నిండిన))
		sd_asym = sd_asym->మాతృ;

	ఒకవేళ (!sd_asym || (sd_asym->flags & SD_NUMA))
		తిరిగి false;

	init_sched_domain_shared(d, sd_asym, SD_ASYM_CPUCAPACITY);
	తిరిగి true;
}

స్థిర పూర్ణాంకం __sched_domains_alloc_llc_id(శూన్య)
{
	పూర్ణాంకం lid, max;

	lockdep_assert_held(&sched_domains_mutex);

	lid = cpumask_మొదటి_zero(sched_domains_llc_id_allocmask);
	/*
	 * llc_id space should never grow larger than the
	 * possible number of CPUs in the system.
	 */
	ఒకవేళ (lid >= nr_cpu_ids)
		తిరిగి -1;

	__cpumask_set_cpu(lid, sched_domains_llc_id_allocmask);
	max = cpumask_చివరి(sched_domains_llc_id_allocmask);
	ఒకవేళ (max > max_lid)
		max_lid = max;

	తిరిగి lid;
}

స్థిర శూన్య __sched_domains_free_llc_id(పూర్ణాంకం cpu)
{
	పూర్ణాంకం i, lid, max;

	lockdep_assert_held(&sched_domains_mutex);

	lid = చొప్పున_cpu(sd_llc_id, cpu);
	ఒకవేళ (lid == -1 || lid >= nr_cpu_ids)
		తిరిగి;

	చొప్పున_cpu(sd_llc_id, cpu) = -1;

	for_each_cpu(i, llc_ముసుగు(cpu)) {
		/* An online CPU owns the llc_id. */
		ఒకవేళ (చొప్పున_cpu(sd_llc_id, i) == lid)
			తిరిగి;
	}

	__cpumask_clear_cpu(lid, sched_domains_llc_id_allocmask);

	max = cpumask_చివరి(sched_domains_llc_id_allocmask);
	/* shrink max lid to save memory */
	ఒకవేళ (max < max_lid)
		max_lid = max;
}

శూన్య sched_domains_free_llc_id(పూర్ణాంకం cpu)
{
	sched_domains_mutex_తాళం();
	__sched_domains_free_llc_id(cpu);
	sched_domains_mutex_unlock();
}

/*
 * Build sched domains for a given set of CPUs and attach the sched domains
 * to the individual CPUs
 */
స్థిర పూర్ణాంకం
build_sched_domains(స్థిరరాశి నిర్మాణం cpumask *cpu_map, నిర్మాణం sched_domain_attr *attr,
		    bool *multi_llcs)
{
	గణన s_alloc alloc_state = sa_none;
	bool has_multi_llcs = false;
	నిర్మాణం sched_domain *sd;
	నిర్మాణం s_దత్తాంశం d;
	నిర్మాణం rq *rq = ఖాళీ;
	పూర్ణాంకం i, ret = -ENOMEM;
	bool has_asym = false;
	bool has_cluster = false;

	ఒకవేళ (WARN_ON(cpumask_ఖాళీలు(cpu_map)))
		వెళ్ళు error;

	alloc_state = __visit_domain_allocation_hell(&d, cpu_map);
	ఒకవేళ (alloc_state != sa_rootdomain)
		వెళ్ళు error;

	/* Set up domains for CPUs specified by the cpu_map: */
	for_each_cpu(i, cpu_map) {
		నిర్మాణం sched_domain_topology_level *tl;
		పూర్ణాంకం lid;

		sd = ఖాళీ;
		for_each_sd_topology(tl) {

			sd = build_sched_domain(tl, cpu_map, attr, sd, i);

			has_asym |= sd->flags & SD_ASYM_CPUCAPACITY;

			ఒకవేళ (tl == sched_domain_topology)
				*చొప్పున_cpu_ptr(d.sd, i) = sd;
			ఒకవేళ (cpumask_equal(cpu_map, sched_domain_పరిధి(sd)))
				ఆపు;
		}

		lid = చొప్పున_cpu(sd_llc_id, i);
		ఒకవేళ (lid == -1) {
			/* try to reuse the llc_id of its siblings */
			కొరకు (పూర్ణాంకం j = cpumask_మొదటి(llc_ముసుగు(i));
			     j < nr_cpu_ids;
			     j = cpumask_తదుపరి(j, llc_ముసుగు(i))) {
				ఒకవేళ (i == j)
					కొనసాగు;

				lid = చొప్పున_cpu(sd_llc_id, j);

				ఒకవేళ (lid != -1) {
					చొప్పున_cpu(sd_llc_id, i) = lid;

					ఆపు;
				}
			}

			/* a new LLC is detected */
			ఒకవేళ (lid == -1)
				చొప్పున_cpu(sd_llc_id, i) = __sched_domains_alloc_llc_id();
		}
	}

	ఒకవేళ (WARN_ON(!topology_పరిధి_sane(cpu_map)))
		వెళ్ళు error;

	/* Build the groups for the domains */
	for_each_cpu(i, cpu_map) {
		కొరకు (sd = *చొప్పున_cpu_ptr(d.sd, i); sd; sd = sd->మాతృ) {
			sd->పరిధి_బరువు = cpumask_బరువు(sched_domain_పరిధి(sd));
			ఒకవేళ (sd->flags & SD_NUMA) {
				ఒకవేళ (build_overlap_sched_groups(sd, i))
					వెళ్ళు error;
			} లేకపోతే {
				ఒకవేళ (build_sched_groups(sd, i))
					వెళ్ళు error;
			}
		}
	}

	for_each_cpu(i, cpu_map) {
		sd = *చొప్పున_cpu_ptr(d.sd, i);
		ఒకవేళ (!sd)
			కొనసాగు;

		ఒకవేళ (has_asym)
			claim_asym_sched_domain_shared(&d, i);

		/* First, find the topmost SD_SHARE_LLC domain */
		అయితే (sd->మాతృ && (sd->మాతృ->flags & SD_SHARE_LLC))
			sd = sd->మాతృ;

		ఒకవేళ (sd->flags & SD_SHARE_LLC) {
			init_sched_domain_shared(&d, sd, SD_SHARE_LLC);

			/*
			 * In presence of higher domains, adjust the
			 * NUMA imbalance stats for the hierarchy.
			 */
			ఒకవేళ (sd->మాతృ) {
				ఒకవేళ (IS_ENABLED(CONFIG_NUMA))
					adjust_numa_imbalance(sd);

				ఒకవేళ (sd_లోపల_multi_llcs(sd))
					has_multi_llcs = true;
			}
		}
	}

	/* Calculate CPU capacity for physical packages and nodes */
	కొరకు (i = nr_cpumask_బిట్లు-1; i >= 0; i--) {
		ఒకవేళ (!cpumask_test_cpu(i, cpu_map))
			కొనసాగు;

		claim_allocations(i, &d);

		కొరకు (sd = *చొప్పున_cpu_ptr(d.sd, i); sd; sd = sd->మాతృ)
			init_sched_groups_సామర్థ్యం(i, sd);
	}

	alloc_sd_llc(cpu_map, &d);

	/* Attach the domains */
	rcu_read_తాళం();
	for_each_cpu(i, cpu_map) {
		rq = cpu_rq(i);
		sd = *చొప్పున_cpu_ptr(d.sd, i);

		cpu_attach_domain(sd, d.rd, i);

		ఒకవేళ (అత్యల్ప_flag_domain(i, SD_CLUSTER))
			has_cluster = true;
	}
	rcu_read_unlock();

	ఒకవేళ (has_asym)
		static_branch_inc_cpuslocked(&sched_asym_cpucapacity);

	ఒకవేళ (has_cluster)
		static_branch_inc_cpuslocked(&sched_cluster_చురుకైన);

	ఒకవేళ (rq && sched_debug_verbose)
		pr_info("root domain span: %*pbl\n", cpumask_pr_వాదనలు(cpu_map));

	ret = 0;
error:
	*multi_llcs = has_multi_llcs;
	__free_domain_allocs(&d, alloc_state, cpu_map);

	తిరిగి ret;
}

/* Current sched domains: */
స్థిర cpumask_var_t			*doms_cur;

/* Number of sched domains in 'doms_cur': */
స్థిర పూర్ణాంకం				ndoms_cur;

/* Attributes of custom domains in 'doms_cur' */
స్థిర నిర్మాణం sched_domain_attr		*dattr_cur;

/*
 * Special case: If a kmalloc() of a doms_cur partition (array of
 * cpumask) fails, then fallback to a single sched domain,
 * as determined by the single cpumask fallback_doms.
 */
స్థిర cpumask_var_t			fallback_doms;

/*
 * arch_update_cpu_topology lets virtualized architectures update the
 * CPU core maps. It is supposed to return 1 if the topology changed
 * or 0 if it stayed the same.
 */
పూర్ణాంకం __weak arch_update_cpu_topology(శూన్య)
{
	తిరిగి 0;
}

cpumask_var_t *alloc_sched_domains(గుర్తులేని పూర్ణాంకం ndoms)
{
	పూర్ణాంకం i;
	cpumask_var_t *doms;

	doms = kmalloc_objs(*doms, ndoms);
	ఒకవేళ (!doms)
		తిరిగి ఖాళీ;
	కొరకు (i = 0; i < ndoms; i++) {
		ఒకవేళ (!alloc_cpumask_var(&doms[i], GFP_KERNEL)) {
			free_sched_domains(doms, i);
			తిరిగి ఖాళీ;
		}
	}
	తిరిగి doms;
}

శూన్య free_sched_domains(cpumask_var_t doms[], గుర్తులేని పూర్ణాంకం ndoms)
{
	గుర్తులేని పూర్ణాంకం i;
	కొరకు (i = 0; i < ndoms; i++)
		free_cpumask_var(doms[i]);
	kfree(doms);
}

/*
 * Set up scheduler domains and groups.  For now this just excludes isolated
 * CPUs, but could be used to exclude other special cases in the future.
 */
పూర్ణాంకం __init sched_init_domains(స్థిరరాశి నిర్మాణం cpumask *cpu_map)
{
	bool multi_llcs;
	పూర్ణాంకం err;

	zalloc_cpumask_var(&sched_domains_llc_id_allocmask, GFP_KERNEL);
	zalloc_cpumask_var(&sched_domains_tmpmask, GFP_KERNEL);
	zalloc_cpumask_var(&sched_domains_tmpmask2, GFP_KERNEL);
	zalloc_cpumask_var(&fallback_doms, GFP_KERNEL);

	arch_update_cpu_topology();
	asym_cpu_సామర్థ్యం_పరిశీలన();
	ndoms_cur = 1;
	doms_cur = alloc_sched_domains(ndoms_cur);
	ఒకవేళ (!doms_cur)
		doms_cur = &fallback_doms;
	cpumask_and(doms_cur[0], cpu_map, housekeeping_cpumask(HK_TYPE_DOMAIN));
	err = build_sched_domains(doms_cur[0], ఖాళీ, &multi_llcs);
	ఒకవేళ (!err)
		sched_cache_సమితి(multi_llcs);

	తిరిగి err;
}

/*
 * Detach sched domains from a group of CPUs specified in cpu_map
 * These CPUs will now be attached to the NULL domain
 */
స్థిర శూన్య detach_destroy_domains(స్థిరరాశి నిర్మాణం cpumask *cpu_map)
{
	గుర్తులేని పూర్ణాంకం cpu = cpumask_any(cpu_map);
	పూర్ణాంకం i;

	ఒకవేళ (rcu_access_సూచిక(చొప్పున_cpu(sd_asym_cpucapacity, cpu)))
		static_branch_dec_cpuslocked(&sched_asym_cpucapacity);

	ఒకవేళ (static_branch_unlikely(&sched_cluster_చురుకైన))
		static_branch_dec_cpuslocked(&sched_cluster_చురుకైన);

	rcu_read_తాళం();
	for_each_cpu(i, cpu_map)
		cpu_attach_domain(ఖాళీ, &def_మూలం_domain, i);
	rcu_read_unlock();
}

/* handle null as "default" */
స్థిర పూర్ణాంకం dattrs_equal(నిర్మాణం sched_domain_attr *cur, పూర్ణాంకం idx_cur,
			నిర్మాణం sched_domain_attr *new, పూర్ణాంకం idx_new)
{
	నిర్మాణం sched_domain_attr tmp;

	/* Fast path: */
	ఒకవేళ (!new && !cur)
		తిరిగి 1;

	tmp = SD_ATTR_INIT;

	తిరిగి !memcmp(cur ? (cur + idx_cur) : &tmp,
			new ? (new + idx_new) : &tmp,
			పరిమాణం(నిర్మాణం sched_domain_attr));
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
స్థిర శూన్య విభజన_sched_domains_locked(పూర్ణాంకం ndoms_new, cpumask_var_t doms_new[],
				    నిర్మాణం sched_domain_attr *dattr_new)
{
	bool __maybe_unused has_eas = false;
	bool has_multi_llcs = false, multi_llcs;
	పూర్ణాంకం i, j, n;
	పూర్ణాంకం new_topology;

	lockdep_assert_held(&sched_domains_mutex);

	/* Let the architecture update CPU core mappings: */
	new_topology = arch_update_cpu_topology();
	/* Trigger rebuilding CPU capacity asymmetry data */
	ఒకవేళ (new_topology)
		asym_cpu_సామర్థ్యం_పరిశీలన();

	ఒకవేళ (!doms_new) {
		WARN_ON_ONCE(dattr_new);
		n = 0;
		doms_new = alloc_sched_domains(1);
		ఒకవేళ (doms_new) {
			n = 1;
			cpumask_and(doms_new[0], cpu_చురుకైన_ముసుగు,
				    housekeeping_cpumask(HK_TYPE_DOMAIN));
		}
	} లేకపోతే {
		n = ndoms_new;
	}

	/* Destroy deleted domains: */
	కొరకు (i = 0; i < ndoms_cur; i++) {
		కొరకు (j = 0; j < n && !new_topology; j++) {
			ఒకవేళ (cpumask_equal(doms_cur[i], doms_new[j]) &&
			    dattrs_equal(dattr_cur, i, dattr_new, j))
				వెళ్ళు match1;
		}
		/* No match - a current sched domain not in new doms_new[] */
		detach_destroy_domains(doms_cur[i]);
match1:
		;
	}

	n = ndoms_cur;
	ఒకవేళ (!doms_new) {
		n = 0;
		doms_new = &fallback_doms;
		cpumask_and(doms_new[0], cpu_చురుకైన_ముసుగు,
			    housekeeping_cpumask(HK_TYPE_DOMAIN));
	}

	/* Build new domains: */
	కొరకు (i = 0; i < ndoms_new; i++) {
		కొరకు (j = 0; j < n && !new_topology; j++) {
			ఒకవేళ (cpumask_equal(doms_new[i], doms_cur[j]) &&
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
				నిర్మాణం sched_domain *sd;
				పూర్ణాంకం cpu = cpumask_మొదటి(doms_cur[j]);

				guard(rcu)();
				sd = rcu_dereference(cpu_rq(cpu)->sd);
				అయితే (sd && sd->మాతృ && (sd->మాతృ->flags & SD_SHARE_LLC))
					sd = sd->మాతృ;
				ఒకవేళ (sd && (sd->flags & SD_SHARE_LLC) && sd->మాతృ &&
				    sd_లోపల_multi_llcs(sd))
					has_multi_llcs = true;
				వెళ్ళు match2;
			}
		}
		/* No match - add a new doms_new */
		build_sched_domains(doms_new[i], dattr_new ? dattr_new + i : ఖాళీ,
				    &multi_llcs);
		has_multi_llcs |= multi_llcs;
match2:
		;
	}
	sched_cache_సమితి(has_multi_llcs);

#ఒకవేళ defined(CONFIG_ENERGY_MODEL) && defined(CONFIG_CPU_FREQ_GOV_SCHEDUTIL)
	/* Build perf domains: */
	కొరకు (i = 0; i < ndoms_new; i++) {
		కొరకు (j = 0; j < n && !sched_energy_update; j++) {
			ఒకవేళ (cpumask_equal(doms_new[i], doms_cur[j]) &&
			    cpu_rq(cpumask_మొదటి(doms_cur[j]))->rd->pd) {
				has_eas = true;
				వెళ్ళు match3;
			}
		}
		/* No match - add perf domains for a new rd */
		has_eas |= build_perf_domains(doms_new[i]);
match3:
		;
	}
	sched_energy_సమితి(has_eas);
#అయితే_ముగింపు

	/* Remember the new sched domains: */
	ఒకవేళ (doms_cur != &fallback_doms)
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
శూన్య విభజన_sched_domains(పూర్ణాంకం ndoms_new, cpumask_var_t doms_new[],
			     నిర్మాణం sched_domain_attr *dattr_new)
{
	sched_domains_mutex_తాళం();
	విభజన_sched_domains_locked(ndoms_new, doms_new, dattr_new);
	sched_domains_mutex_unlock();
}
