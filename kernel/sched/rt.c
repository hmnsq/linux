// SPDX-License-Identifier: GPL-2.0
/*
 * Real-Time Scheduling Class (mapped to the SCHED_FIFO and SCHED_RR
 * policies)
 */

#అన్తర్భావ "sched.h"
#అన్తర్భావ "pelt.h"

పూర్ణాంకం sched_rr_timeslice = RR_TIMESLICE;
/* More than 4 hours if BW_SHIFT equals 20. */
స్థిర స్థిరరాశి u64 max_rt_runtime = MAX_BW;

/*
 * period over which we measure -rt task CPU usage in us.
 * default: 1s
 */
పూర్ణాంకం sysctl_sched_rt_period = 1000000;

/*
 * part of the period that we allow rt tasks to run in us.
 * default: 1s
 */
పూర్ణాంకం sysctl_sched_rt_runtime = 1000000;

#నిర్వచితమైతే CONFIG_SYSCTL
స్థిర పూర్ణాంకం sysctl_sched_rr_timeslice = (MSEC_చొప్పున_SEC * RR_TIMESLICE) / HZ;
స్థిర పూర్ణాంకం sched_rt_handler(స్థిరరాశి నిర్మాణం ctl_పట్టిక *పట్టిక, పూర్ణాంకం లిఖ, శూన్య *బఫరు,
		కొలత_t *lenp, loff_t *ppos);
స్థిర పూర్ణాంకం sched_rr_handler(స్థిరరాశి నిర్మాణం ctl_పట్టిక *పట్టిక, పూర్ణాంకం లిఖ, శూన్య *బఫరు,
		కొలత_t *lenp, loff_t *ppos);
స్థిర స్థిరరాశి నిర్మాణం ctl_పట్టిక sched_rt_sysctls[] = {
	{
		.procname       = "sched_rt_period_us",
		.దత్తాంశం           = &sysctl_sched_rt_period,
		.maxlen         = పరిమాణం(పూర్ణాంకం),
		.బహుళకం           = 0644,
		.proc_handler   = sched_rt_handler,
		.extra1         = SYSCTL_ONE,
		.extra2         = SYSCTL_INT_MAX,
	},
	{
		.procname       = "sched_rt_runtime_us",
		.దత్తాంశం           = &sysctl_sched_rt_runtime,
		.maxlen         = పరిమాణం(పూర్ణాంకం),
		.బహుళకం           = 0644,
		.proc_handler   = sched_rt_handler,
		.extra1         = SYSCTL_NEG_ONE,
		.extra2         = (శూన్య *)&sysctl_sched_rt_period,
	},
	{
		.procname       = "sched_rr_timeslice_ms",
		.దత్తాంశం           = &sysctl_sched_rr_timeslice,
		.maxlen         = పరిమాణం(పూర్ణాంకం),
		.బహుళకం           = 0644,
		.proc_handler   = sched_rr_handler,
	},
};

స్థిర పూర్ణాంకం __init sched_rt_sysctl_init(శూన్య)
{
	రిజిస్టరు_sysctl_init("kernel", sched_rt_sysctls);
	తిరిగి 0;
}
late_initcall(sched_rt_sysctl_init);
#అయితే_ముగింపు /* CONFIG_SYSCTL */

శూన్య init_rt_rq(నిర్మాణం rt_rq *rt_rq)
{
	నిర్మాణం rt_prio_array *array;
	పూర్ణాంకం i;

	array = &rt_rq->చురుకైన;
	కొరకు (i = 0; i < MAX_RT_PRIO; i++) {
		INIT_LIST_తల(array->వరుస + i);
		__clear_bit(i, array->bitmap);
	}
	/* delimiter for bitsearch: */
	__set_bit(MAX_RT_PRIO, array->bitmap);

	rt_rq->highest_prio.curr = MAX_RT_PRIO-1;
	rt_rq->highest_prio.తదుపరి = MAX_RT_PRIO-1;
	rt_rq->overloaded = 0;
	plist_తల_init(&rt_rq->pushable_tasks);
	/* We start is dequeued state, because no RT tasks are queued */
	rt_rq->rt_queued = 0;

#నిర్వచితమైతే CONFIG_RT_సమూహం_SCHED
	rt_rq->rt_సమయం = 0;
	rt_rq->rt_throttled = 0;
	rt_rq->rt_runtime = 0;
	raw_spin_తాళం_init(&rt_rq->rt_runtime_తాళం);
	rt_rq->tg = &మూలం_task_సమూహం;
#అయితే_ముగింపు
}

#నిర్వచితమైతే CONFIG_RT_సమూహం_SCHED

స్థిర పూర్ణాంకం do_sched_rt_period_timer(నిర్మాణం rt_bandwidth *rt_b, పూర్ణాంకం overrun);

స్థిర గణన hrtimer_restart sched_rt_period_timer(నిర్మాణం hrtimer *timer)
{
	నిర్మాణం rt_bandwidth *rt_b =
		container_యొక్క(timer, నిర్మాణం rt_bandwidth, rt_period_timer);
	పూర్ణాంకం idle = 0;
	పూర్ణాంకం overrun;

	raw_spin_తాళం(&rt_b->rt_runtime_తాళం);
	కొరకు (;;) {
		overrun = hrtimer_forward_now(timer, rt_b->rt_period);
		ఒకవేళ (!overrun)
			ఆపు;

		raw_spin_unlock(&rt_b->rt_runtime_తాళం);
		idle = do_sched_rt_period_timer(rt_b, overrun);
		raw_spin_తాళం(&rt_b->rt_runtime_తాళం);
	}
	ఒకవేళ (idle)
		rt_b->rt_period_చురుకైన = 0;
	raw_spin_unlock(&rt_b->rt_runtime_తాళం);

	తిరిగి idle ? HRTIMER_NORESTART : HRTIMER_RESTART;
}

శూన్య init_rt_bandwidth(నిర్మాణం rt_bandwidth *rt_b, u64 period, u64 runtime)
{
	rt_b->rt_period = ns_గమ్యం_ktime(period);
	rt_b->rt_runtime = runtime;

	raw_spin_తాళం_init(&rt_b->rt_runtime_తాళం);

	hrtimer_అమరిక(&rt_b->rt_period_timer, sched_rt_period_timer, CLOCK_MONOTONIC,
		      HRTIMER_బహుళకం_REL_HARD);
}

స్థిర అంతఃవరుస శూన్య do_ప్రారంభం_rt_bandwidth(నిర్మాణం rt_bandwidth *rt_b)
{
	raw_spin_తాళం(&rt_b->rt_runtime_తాళం);
	ఒకవేళ (!rt_b->rt_period_చురుకైన) {
		rt_b->rt_period_చురుకైన = 1;
		/*
		 * SCHED_DEADLINE updates the bandwidth, as a run away
		 * RT task with a DL task could hog a CPU. But DL does
		 * not reset the period. If a deadline task was running
		 * without an RT task running, it can cause RT tasks to
		 * throttle when they start up. Kick the timer right away
		 * to update the period.
		 */
		hrtimer_forward_now(&rt_b->rt_period_timer, ns_గమ్యం_ktime(0));
		hrtimer_ప్రారంభం_expires(&rt_b->rt_period_timer,
				      HRTIMER_బహుళకం_ABS_PINNED_HARD);
	}
	raw_spin_unlock(&rt_b->rt_runtime_తాళం);
}

స్థిర శూన్య ప్రారంభం_rt_bandwidth(నిర్మాణం rt_bandwidth *rt_b)
{
	ఒకవేళ (!rt_bandwidth_enabled() || rt_b->rt_runtime == RUNTIME_INF)
		తిరిగి;

	do_ప్రారంభం_rt_bandwidth(rt_b);
}

స్థిర శూన్య destroy_rt_bandwidth(నిర్మాణం rt_bandwidth *rt_b)
{
	hrtimer_cancel(&rt_b->rt_period_timer);
}

#నిర్వచించు rt_entity_is_task(rt_se) (!(rt_se)->my_q)

స్థిర అంతఃవరుస నిర్మాణం task_struct *rt_task_యొక్క(నిర్మాణం sched_rt_entity *rt_se)
{
	WARN_ON_ONCE(!rt_entity_is_task(rt_se));

	తిరిగి container_యొక్క(rt_se, నిర్మాణం task_struct, rt);
}

స్థిర అంతఃవరుస నిర్మాణం rq *rq_యొక్క_rt_rq(నిర్మాణం rt_rq *rt_rq)
{
	/* Cannot fold with non-CONFIG_RT_GROUP_SCHED version, layout */
	WARN_ON(!rt_సమూహం_sched_enabled() && rt_rq->tg != &మూలం_task_సమూహం);
	తిరిగి rt_rq->rq;
}

స్థిర అంతఃవరుస నిర్మాణం rt_rq *rt_rq_యొక్క_se(నిర్మాణం sched_rt_entity *rt_se)
{
	WARN_ON(!rt_సమూహం_sched_enabled() && rt_se->rt_rq->tg != &మూలం_task_సమూహం);
	తిరిగి rt_se->rt_rq;
}

స్థిర అంతఃవరుస నిర్మాణం rq *rq_యొక్క_rt_se(నిర్మాణం sched_rt_entity *rt_se)
{
	నిర్మాణం rt_rq *rt_rq = rt_se->rt_rq;

	WARN_ON(!rt_సమూహం_sched_enabled() && rt_rq->tg != &మూలం_task_సమూహం);
	తిరిగి rt_rq->rq;
}

శూన్య unregister_rt_sched_సమూహం(నిర్మాణం task_సమూహం *tg)
{
	ఒకవేళ (!rt_సమూహం_sched_enabled())
		తిరిగి;

	ఒకవేళ (tg->rt_se)
		destroy_rt_bandwidth(&tg->rt_bandwidth);
}

శూన్య free_rt_sched_సమూహం(నిర్మాణం task_సమూహం *tg)
{
	పూర్ణాంకం i;

	ఒకవేళ (!rt_సమూహం_sched_enabled())
		తిరిగి;

	for_each_possible_cpu(i) {
		ఒకవేళ (tg->rt_rq)
			kfree(tg->rt_rq[i]);
		ఒకవేళ (tg->rt_se)
			kfree(tg->rt_se[i]);
	}

	kfree(tg->rt_rq);
	kfree(tg->rt_se);
}

శూన్య init_tg_rt_నమోదు(నిర్మాణం task_సమూహం *tg, నిర్మాణం rt_rq *rt_rq,
		నిర్మాణం sched_rt_entity *rt_se, పూర్ణాంకం cpu,
		నిర్మాణం sched_rt_entity *మాతృ)
{
	నిర్మాణం rq *rq = cpu_rq(cpu);

	rt_rq->highest_prio.curr = MAX_RT_PRIO-1;
	rt_rq->rt_nr_boosted = 0;
	rt_rq->rq = rq;
	rt_rq->tg = tg;

	tg->rt_rq[cpu] = rt_rq;
	tg->rt_se[cpu] = rt_se;

	ఒకవేళ (!rt_se)
		తిరిగి;

	ఒకవేళ (!మాతృ)
		rt_se->rt_rq = &rq->rt;
	లేకపోతే
		rt_se->rt_rq = మాతృ->my_q;

	rt_se->my_q = rt_rq;
	rt_se->మాతృ = మాతృ;
	INIT_LIST_తల(&rt_se->నిరంతరం_list);
}

పూర్ణాంకం alloc_rt_sched_సమూహం(నిర్మాణం task_సమూహం *tg, నిర్మాణం task_సమూహం *మాతృ)
{
	నిర్మాణం rt_rq *rt_rq;
	నిర్మాణం sched_rt_entity *rt_se;
	పూర్ణాంకం i;

	ఒకవేళ (!rt_సమూహం_sched_enabled())
		తిరిగి 1;

	tg->rt_rq = kzalloc_objs(rt_rq, nr_cpu_ids);
	ఒకవేళ (!tg->rt_rq)
		వెళ్ళు err;
	tg->rt_se = kzalloc_objs(rt_se, nr_cpu_ids);
	ఒకవేళ (!tg->rt_se)
		వెళ్ళు err;

	init_rt_bandwidth(&tg->rt_bandwidth, ktime_గమ్యం_ns(global_rt_period()), 0);

	for_each_possible_cpu(i) {
		rt_rq = kzalloc_కణుపు(పరిమాణం(నిర్మాణం rt_rq),
				     GFP_KERNEL, cpu_గమ్యం_కణుపు(i));
		ఒకవేళ (!rt_rq)
			వెళ్ళు err;

		rt_se = kzalloc_కణుపు(పరిమాణం(నిర్మాణం sched_rt_entity),
				     GFP_KERNEL, cpu_గమ్యం_కణుపు(i));
		ఒకవేళ (!rt_se)
			వెళ్ళు err_free_rq;

		init_rt_rq(rt_rq);
		rt_rq->rt_runtime = tg->rt_bandwidth.rt_runtime;
		init_tg_rt_నమోదు(tg, rt_rq, rt_se, i, మాతృ->rt_se[i]);
	}

	తిరిగి 1;

err_free_rq:
	kfree(rt_rq);
err:
	తిరిగి 0;
}

#లేకపోతే /* !CONFIG_RT_GROUP_SCHED: */

#నిర్వచించు rt_entity_is_task(rt_se) (1)

స్థిర అంతఃవరుస నిర్మాణం task_struct *rt_task_యొక్క(నిర్మాణం sched_rt_entity *rt_se)
{
	తిరిగి container_యొక్క(rt_se, నిర్మాణం task_struct, rt);
}

స్థిర అంతఃవరుస నిర్మాణం rq *rq_యొక్క_rt_rq(నిర్మాణం rt_rq *rt_rq)
{
	తిరిగి container_యొక్క(rt_rq, నిర్మాణం rq, rt);
}

స్థిర అంతఃవరుస నిర్మాణం rq *rq_యొక్క_rt_se(నిర్మాణం sched_rt_entity *rt_se)
{
	నిర్మాణం task_struct *p = rt_task_యొక్క(rt_se);

	తిరిగి task_rq(p);
}

స్థిర అంతఃవరుస నిర్మాణం rt_rq *rt_rq_యొక్క_se(నిర్మాణం sched_rt_entity *rt_se)
{
	నిర్మాణం rq *rq = rq_యొక్క_rt_se(rt_se);

	తిరిగి &rq->rt;
}

శూన్య unregister_rt_sched_సమూహం(నిర్మాణం task_సమూహం *tg) { }

శూన్య free_rt_sched_సమూహం(నిర్మాణం task_సమూహం *tg) { }

పూర్ణాంకం alloc_rt_sched_సమూహం(నిర్మాణం task_సమూహం *tg, నిర్మాణం task_సమూహం *మాతృ)
{
	తిరిగి 1;
}
#అయితే_ముగింపు /* !CONFIG_RT_GROUP_SCHED */

స్థిర అంతఃవరుస bool need_pull_rt_task(నిర్మాణం rq *rq, నిర్మాణం task_struct *prev)
{
	/* Try to pull RT tasks here if we lower this rq's prio */
	తిరిగి rq->online && rq->rt.highest_prio.curr > prev->prio;
}

స్థిర అంతఃవరుస పూర్ణాంకం rt_overloaded(నిర్మాణం rq *rq)
{
	తిరిగి atomic_read(&rq->rd->rto_లెక్క);
}

స్థిర అంతఃవరుస శూన్య rt_సమితి_overload(నిర్మాణం rq *rq)
{
	ఒకవేళ (!rq->online)
		తిరిగి;

	cpumask_సమితి_cpu(rq->cpu, rq->rd->rto_ముసుగు);
	/*
	 * Make sure the mask is visible before we set
	 * the overload count. That is checked to determine
	 * if we should look at the mask. It would be a shame
	 * if we looked at the mask, but the mask was not
	 * updated yet.
	 *
	 * Matched by the barrier in pull_rt_task().
	 */
	smp_wmb();
	atomic_inc(&rq->rd->rto_లెక్క);
}

స్థిర అంతఃవరుస శూన్య rt_clear_overload(నిర్మాణం rq *rq)
{
	ఒకవేళ (!rq->online)
		తిరిగి;

	/* the order here really doesn't matter */
	atomic_dec(&rq->rd->rto_లెక్క);
	cpumask_clear_cpu(rq->cpu, rq->rd->rto_ముసుగు);
}

స్థిర అంతఃవరుస పూర్ణాంకం has_pushable_tasks(నిర్మాణం rq *rq)
{
	తిరిగి !plist_తల_ఖాళీలు(&rq->rt.pushable_tasks);
}

స్థిర DEFINE_చొప్పున_CPU(నిర్మాణం balance_callback, rt_నెట్టు_తల);
స్థిర DEFINE_చొప్పున_CPU(నిర్మాణం balance_callback, rt_pull_తల);

స్థిర శూన్య నెట్టు_rt_tasks(నిర్మాణం rq *);
స్థిర శూన్య pull_rt_task(నిర్మాణం rq *);

స్థిర అంతఃవరుస శూన్య rt_వరుస_నెట్టు_tasks(నిర్మాణం rq *rq)
{
	ఒకవేళ (!has_pushable_tasks(rq))
		తిరిగి;

	వరుస_balance_callback(rq, &చొప్పున_cpu(rt_నెట్టు_తల, rq->cpu), నెట్టు_rt_tasks);
}

స్థిర అంతఃవరుస శూన్య rt_వరుస_pull_task(నిర్మాణం rq *rq)
{
	వరుస_balance_callback(rq, &చొప్పున_cpu(rt_pull_తల, rq->cpu), pull_rt_task);
}

స్థిర శూన్య వరుసలో_చేర్చు_pushable_task(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
	plist_del(&p->pushable_tasks, &rq->rt.pushable_tasks);
	plist_కణుపు_init(&p->pushable_tasks, p->prio);
	plist_కూడు(&p->pushable_tasks, &rq->rt.pushable_tasks);

	/* Update the highest prio pushable task */
	ఒకవేళ (p->prio < rq->rt.highest_prio.తదుపరి)
		rq->rt.highest_prio.తదుపరి = p->prio;

	ఒకవేళ (!rq->rt.overloaded) {
		rt_సమితి_overload(rq);
		rq->rt.overloaded = 1;
	}
}

స్థిర శూన్య వరుస_నుండి_తీయి_pushable_task(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
	plist_del(&p->pushable_tasks, &rq->rt.pushable_tasks);

	/* Update the new highest prio pushable task */
	ఒకవేళ (has_pushable_tasks(rq)) {
		p = plist_మొదటి_నమోదు(&rq->rt.pushable_tasks,
				      నిర్మాణం task_struct, pushable_tasks);
		rq->rt.highest_prio.తదుపరి = p->prio;
	} లేకపోతే {
		rq->rt.highest_prio.తదుపరి = MAX_RT_PRIO-1;

		ఒకవేళ (rq->rt.overloaded) {
			rt_clear_overload(rq);
			rq->rt.overloaded = 0;
		}
	}
}

స్థిర శూన్య వరుసలో_చేర్చు_శిఖరం_rt_rq(నిర్మాణం rt_rq *rt_rq);
స్థిర శూన్య వరుస_నుండి_తీయి_శిఖరం_rt_rq(నిర్మాణం rt_rq *rt_rq, గుర్తులేని పూర్ణాంకం లెక్క);

స్థిర అంతఃవరుస పూర్ణాంకం on_rt_rq(నిర్మాణం sched_rt_entity *rt_se)
{
	తిరిగి rt_se->on_rq;
}

#నిర్వచితమైతే CONFIG_UCLAMP_TASK
/*
 * Verify the fitness of task @p to run on @cpu taking into account the uclamp
 * settings.
 *
 * This check is only important for heterogeneous systems where uclamp_min value
 * is higher than the capacity of a @cpu. For non-heterogeneous system this
 * function will always return true.
 *
 * The function will return true if the capacity of the @cpu is >= the
 * uclamp_min and false otherwise.
 *
 * Note that uclamp_min will be clamped to uclamp_max if uclamp_min
 * > uclamp_max.
 */
స్థిర అంతఃవరుస bool rt_task_సరిపోతుంది_సామర్థ్యం(నిర్మాణం task_struct *p, పూర్ణాంకం cpu)
{
	గుర్తులేని పూర్ణాంకం min_cap;
	గుర్తులేని పూర్ణాంకం max_cap;
	గుర్తులేని పూర్ణాంకం cpu_cap;

	/* Only heterogeneous systems can benefit from this check */
	ఒకవేళ (!sched_asym_cpucap_చురుకైన())
		తిరిగి true;

	min_cap = uclamp_eff_విలువ(p, UCLAMP_MIN);
	max_cap = uclamp_eff_విలువ(p, UCLAMP_MAX);

	cpu_cap = arch_scale_cpu_సామర్థ్యం(cpu);

	తిరిగి cpu_cap >= min(min_cap, max_cap);
}
#లేకపోతే /* !CONFIG_UCLAMP_TASK: */
స్థిర అంతఃవరుస bool rt_task_సరిపోతుంది_సామర్థ్యం(నిర్మాణం task_struct *p, పూర్ణాంకం cpu)
{
	తిరిగి true;
}
#అయితే_ముగింపు /* !CONFIG_UCLAMP_TASK */

#నిర్వచితమైతే CONFIG_RT_సమూహం_SCHED

స్థిర అంతఃవరుస u64 sched_rt_runtime(నిర్మాణం rt_rq *rt_rq)
{
	తిరిగి rt_rq->rt_runtime;
}

స్థిర అంతఃవరుస u64 sched_rt_period(నిర్మాణం rt_rq *rt_rq)
{
	తిరిగి ktime_గమ్యం_ns(rt_rq->tg->rt_bandwidth.rt_period);
}

రకనిర్వచనం నిర్మాణం task_సమూహం *rt_rq_iter_t;

స్థిర అంతఃవరుస నిర్మాణం task_సమూహం *తదుపరి_task_సమూహం(నిర్మాణం task_సమూహం *tg)
{
	ఒకవేళ (!rt_సమూహం_sched_enabled()) {
		WARN_ON(tg != &మూలం_task_సమూహం);
		తిరిగి ఖాళీ;
	}

	చేయి {
		tg = list_నమోదు_rcu(tg->list.తదుపరి,
			typeof(నిర్మాణం task_సమూహం), list);
	} అయితే (&tg->list != &task_groups && task_సమూహం_is_autogroup(tg));

	ఒకవేళ (&tg->list == &task_groups)
		tg = ఖాళీ;

	తిరిగి tg;
}

#నిర్వచించు for_each_rt_rq(rt_rq, iter, rq)					\
	కొరకు (iter = &మూలం_task_సమూహం;					\
		iter && (rt_rq = iter->rt_rq[cpu_యొక్క(rq)]);		\
		iter = తదుపరి_task_సమూహం(iter))

#నిర్వచించు for_each_sched_rt_entity(rt_se) \
	కొరకు (; rt_se; rt_se = rt_se->మాతృ)

స్థిర అంతఃవరుస నిర్మాణం rt_rq *సమూహం_rt_rq(నిర్మాణం sched_rt_entity *rt_se)
{
	తిరిగి rt_se->my_q;
}

స్థిర శూన్య వరుసలో_చేర్చు_rt_entity(నిర్మాణం sched_rt_entity *rt_se, గుర్తులేని పూర్ణాంకం flags);
స్థిర శూన్య వరుస_నుండి_తీయి_rt_entity(నిర్మాణం sched_rt_entity *rt_se, గుర్తులేని పూర్ణాంకం flags);

స్థిర శూన్య sched_rt_rq_వరుసలో_చేర్చు(నిర్మాణం rt_rq *rt_rq)
{
	నిర్మాణం task_struct *donor = rq_యొక్క_rt_rq(rt_rq)->donor;
	నిర్మాణం rq *rq = rq_యొక్క_rt_rq(rt_rq);
	నిర్మాణం sched_rt_entity *rt_se;

	పూర్ణాంకం cpu = cpu_యొక్క(rq);

	rt_se = rt_rq->tg->rt_se[cpu];

	ఒకవేళ (rt_rq->rt_nr_running) {
		ఒకవేళ (!rt_se)
			వరుసలో_చేర్చు_శిఖరం_rt_rq(rt_rq);
		లేకపోతే ఒకవేళ (!on_rt_rq(rt_se))
			వరుసలో_చేర్చు_rt_entity(rt_se, 0);

		ఒకవేళ (rt_rq->highest_prio.curr < donor->prio)
			resched_curr(rq);
	}
}

స్థిర శూన్య sched_rt_rq_వరుస_నుండి_తీయి(నిర్మాణం rt_rq *rt_rq)
{
	నిర్మాణం sched_rt_entity *rt_se;
	పూర్ణాంకం cpu = cpu_యొక్క(rq_యొక్క_rt_rq(rt_rq));

	rt_se = rt_rq->tg->rt_se[cpu];

	ఒకవేళ (!rt_se) {
		వరుస_నుండి_తీయి_శిఖరం_rt_rq(rt_rq, rt_rq->rt_nr_running);
		/* Kick cpufreq (see the comment in kernel/sched/sched.h). */
		cpufreq_update_util(rq_యొక్క_rt_rq(rt_rq), 0);
	}
	లేకపోతే ఒకవేళ (on_rt_rq(rt_se))
		వరుస_నుండి_తీయి_rt_entity(rt_se, 0);
}

స్థిర అంతఃవరుస పూర్ణాంకం rt_rq_throttled(నిర్మాణం rt_rq *rt_rq)
{
	తిరిగి rt_rq->rt_throttled && !rt_rq->rt_nr_boosted;
}

స్థిర పూర్ణాంకం rt_se_boosted(నిర్మాణం sched_rt_entity *rt_se)
{
	నిర్మాణం rt_rq *rt_rq = సమూహం_rt_rq(rt_se);
	నిర్మాణం task_struct *p;

	ఒకవేళ (rt_rq)
		తిరిగి !!rt_rq->rt_nr_boosted;

	p = rt_task_యొక్క(rt_se);
	తిరిగి p->prio != p->సాధారణ_prio;
}

స్థిర అంతఃవరుస స్థిరరాశి నిర్మాణం cpumask *sched_rt_period_ముసుగు(శూన్య)
{
	తిరిగి this_rq()->rd->పరిధి;
}

స్థిర అంతఃవరుస
నిర్మాణం rt_rq *sched_rt_period_rt_rq(నిర్మాణం rt_bandwidth *rt_b, పూర్ణాంకం cpu)
{
	తిరిగి container_యొక్క(rt_b, నిర్మాణం task_సమూహం, rt_bandwidth)->rt_rq[cpu];
}

స్థిర అంతఃవరుస నిర్మాణం rt_bandwidth *sched_rt_bandwidth(నిర్మాణం rt_rq *rt_rq)
{
	తిరిగి &rt_rq->tg->rt_bandwidth;
}

bool sched_rt_bandwidth_account(నిర్మాణం rt_rq *rt_rq)
{
	నిర్మాణం rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);

	తిరిగి (hrtimer_చురుకైన(&rt_b->rt_period_timer) ||
		rt_rq->rt_సమయం < rt_b->rt_runtime);
}

/*
 * We ran out of runtime, see if we can borrow some from our neighbours.
 */
స్థిర శూన్య do_balance_runtime(నిర్మాణం rt_rq *rt_rq)
{
	నిర్మాణం rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);
	నిర్మాణం మూలం_domain *rd = rq_యొక్క_rt_rq(rt_rq)->rd;
	పూర్ణాంకం i, బరువు;
	u64 rt_period;

	బరువు = cpumask_బరువు(rd->పరిధి);

	raw_spin_తాళం(&rt_b->rt_runtime_తాళం);
	rt_period = ktime_గమ్యం_ns(rt_b->rt_period);
	for_each_cpu(i, rd->పరిధి) {
		నిర్మాణం rt_rq *iter = sched_rt_period_rt_rq(rt_b, i);
		s64 diff;

		ఒకవేళ (iter == rt_rq)
			కొనసాగు;

		raw_spin_తాళం(&iter->rt_runtime_తాళం);
		/*
		 * Either all rqs have inf runtime and there's nothing to steal
		 * or __disable_runtime() below sets a specific rq to inf to
		 * indicate its been disabled and disallow stealing.
		 */
		ఒకవేళ (iter->rt_runtime == RUNTIME_INF)
			వెళ్ళు తదుపరి;

		/*
		 * From runqueues with spare time, take 1/n part of their
		 * spare time, but no more than our period.
		 */
		diff = iter->rt_runtime - iter->rt_సమయం;
		ఒకవేళ (diff > 0) {
			diff = div_u64((u64)diff, బరువు);
			ఒకవేళ (rt_rq->rt_runtime + diff > rt_period)
				diff = rt_period - rt_rq->rt_runtime;
			iter->rt_runtime -= diff;
			rt_rq->rt_runtime += diff;
			ఒకవేళ (rt_rq->rt_runtime == rt_period) {
				raw_spin_unlock(&iter->rt_runtime_తాళం);
				ఆపు;
			}
		}
తదుపరి:
		raw_spin_unlock(&iter->rt_runtime_తాళం);
	}
	raw_spin_unlock(&rt_b->rt_runtime_తాళం);
}

/*
 * Ensure this RQ takes back all the runtime it lend to its neighbours.
 */
స్థిర శూన్య __disable_runtime(నిర్మాణం rq *rq)
{
	నిర్మాణం మూలం_domain *rd = rq->rd;
	rt_rq_iter_t iter;
	నిర్మాణం rt_rq *rt_rq;

	ఒకవేళ (unlikely(!scheduler_running))
		తిరిగి;

	for_each_rt_rq(rt_rq, iter, rq) {
		నిర్మాణం rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);
		s64 want;
		పూర్ణాంకం i;

		raw_spin_తాళం(&rt_b->rt_runtime_తాళం);
		raw_spin_తాళం(&rt_rq->rt_runtime_తాళం);
		/*
		 * Either we're all inf and nobody needs to borrow, or we're
		 * already disabled and thus have nothing to do, or we have
		 * exactly the right amount of runtime to take out.
		 */
		ఒకవేళ (rt_rq->rt_runtime == RUNTIME_INF ||
				rt_rq->rt_runtime == rt_b->rt_runtime)
			వెళ్ళు సమతుల్య;
		raw_spin_unlock(&rt_rq->rt_runtime_తాళం);

		/*
		 * Calculate the difference between what we started out with
		 * and what we current have, that's the amount of runtime
		 * we lend and now have to reclaim.
		 */
		want = rt_b->rt_runtime - rt_rq->rt_runtime;

		/*
		 * Greedy reclaim, take back as much as we can.
		 */
		for_each_cpu(i, rd->పరిధి) {
			నిర్మాణం rt_rq *iter = sched_rt_period_rt_rq(rt_b, i);
			s64 diff;

			/*
			 * Can't reclaim from ourselves or disabled runqueues.
			 */
			ఒకవేళ (iter == rt_rq || iter->rt_runtime == RUNTIME_INF)
				కొనసాగు;

			raw_spin_తాళం(&iter->rt_runtime_తాళం);
			ఒకవేళ (want > 0) {
				diff = min_t(s64, iter->rt_runtime, want);
				iter->rt_runtime -= diff;
				want -= diff;
			} లేకపోతే {
				iter->rt_runtime -= want;
				want -= want;
			}
			raw_spin_unlock(&iter->rt_runtime_తాళం);

			ఒకవేళ (!want)
				ఆపు;
		}

		raw_spin_తాళం(&rt_rq->rt_runtime_తాళం);
		/*
		 * We cannot be left wanting - that would mean some runtime
		 * leaked out of the system.
		 */
		WARN_ON_ONCE(want);
సమతుల్య:
		/*
		 * Disable all the borrow logic by pretending we have inf
		 * runtime - in which case borrowing doesn't make sense.
		 */
		rt_rq->rt_runtime = RUNTIME_INF;
		rt_rq->rt_throttled = 0;
		raw_spin_unlock(&rt_rq->rt_runtime_తాళం);
		raw_spin_unlock(&rt_b->rt_runtime_తాళం);

		/* Make rt_rq available for pick_next_task() */
		sched_rt_rq_వరుసలో_చేర్చు(rt_rq);
	}
}

స్థిర శూన్య __enable_runtime(నిర్మాణం rq *rq)
{
	rt_rq_iter_t iter;
	నిర్మాణం rt_rq *rt_rq;

	ఒకవేళ (unlikely(!scheduler_running))
		తిరిగి;

	/*
	 * Reset each runqueue's bandwidth settings
	 */
	for_each_rt_rq(rt_rq, iter, rq) {
		నిర్మాణం rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);

		raw_spin_తాళం(&rt_b->rt_runtime_తాళం);
		raw_spin_తాళం(&rt_rq->rt_runtime_తాళం);
		rt_rq->rt_runtime = rt_b->rt_runtime;
		rt_rq->rt_సమయం = 0;
		rt_rq->rt_throttled = 0;
		raw_spin_unlock(&rt_rq->rt_runtime_తాళం);
		raw_spin_unlock(&rt_b->rt_runtime_తాళం);
	}
}

స్థిర శూన్య balance_runtime(నిర్మాణం rt_rq *rt_rq)
{
	ఒకవేళ (!sched_feat(RT_RUNTIME_SHARE))
		తిరిగి;

	ఒకవేళ (rt_rq->rt_సమయం > rt_rq->rt_runtime) {
		raw_spin_unlock(&rt_rq->rt_runtime_తాళం);
		do_balance_runtime(rt_rq);
		raw_spin_తాళం(&rt_rq->rt_runtime_తాళం);
	}
}

స్థిర పూర్ణాంకం do_sched_rt_period_timer(నిర్మాణం rt_bandwidth *rt_b, పూర్ణాంకం overrun)
{
	పూర్ణాంకం i, idle = 1, throttled = 0;
	స్థిరరాశి నిర్మాణం cpumask *పరిధి;

	పరిధి = sched_rt_period_ముసుగు();

	/*
	 * FIXME: isolated CPUs should really leave the root task group,
	 * whether they are isolcpus or were isolated via cpusets, lest
	 * the timer run on a CPU which does not service all runqueues,
	 * potentially leaving other CPUs indefinitely throttled.  If
	 * isolation is really required, the user will turn the throttle
	 * off to kill the perturbations it causes anyway.  Meanwhile,
	 * this maintains functionality for boot and/or troubleshooting.
	 */
	ఒకవేళ (rt_b == &మూలం_task_సమూహం.rt_bandwidth)
		పరిధి = cpu_online_ముసుగు;

	for_each_cpu(i, పరిధి) {
		పూర్ణాంకం వరుసలో_చేర్చు = 0;
		నిర్మాణం rt_rq *rt_rq = sched_rt_period_rt_rq(rt_b, i);
		నిర్మాణం rq *rq = rq_యొక్క_rt_rq(rt_rq);
		నిర్మాణం rq_flags rf;
		పూర్ణాంకం skip;

		/*
		 * When span == cpu_online_mask, taking each rq->lock
		 * can be time-consuming. Try to avoid it when possible.
		 */
		raw_spin_తాళం(&rt_rq->rt_runtime_తాళం);
		ఒకవేళ (!sched_feat(RT_RUNTIME_SHARE) && rt_rq->rt_runtime != RUNTIME_INF)
			rt_rq->rt_runtime = rt_b->rt_runtime;
		skip = !rt_rq->rt_సమయం && !rt_rq->rt_nr_running;
		raw_spin_unlock(&rt_rq->rt_runtime_తాళం);
		ఒకవేళ (skip)
			కొనసాగు;

		rq_తాళం(rq, &rf);
		update_rq_clock(rq);

		ఒకవేళ (rt_rq->rt_సమయం) {
			u64 runtime;

			raw_spin_తాళం(&rt_rq->rt_runtime_తాళం);
			ఒకవేళ (rt_rq->rt_throttled)
				balance_runtime(rt_rq);
			runtime = rt_rq->rt_runtime;
			rt_rq->rt_సమయం -= min(rt_rq->rt_సమయం, overrun*runtime);
			ఒకవేళ (rt_rq->rt_throttled && rt_rq->rt_సమయం < runtime) {
				rt_rq->rt_throttled = 0;
				వరుసలో_చేర్చు = 1;

				/*
				 * When we're idle and a woken (rt) task is
				 * throttled wakeup_preempt() will set
				 * skip_update and the time between the wakeup
				 * and this unthrottle will get accounted as
				 * 'runtime'.
				 */
				ఒకవేళ (rt_rq->rt_nr_running && rq->curr == rq->idle)
					rq_clock_cancel_skipupdate(rq);
			}
			ఒకవేళ (rt_rq->rt_సమయం || rt_rq->rt_nr_running)
				idle = 0;
			raw_spin_unlock(&rt_rq->rt_runtime_తాళం);
		} లేకపోతే ఒకవేళ (rt_rq->rt_nr_running) {
			idle = 0;
			ఒకవేళ (!rt_rq_throttled(rt_rq))
				వరుసలో_చేర్చు = 1;
		}
		ఒకవేళ (rt_rq->rt_throttled)
			throttled = 1;

		ఒకవేళ (వరుసలో_చేర్చు)
			sched_rt_rq_వరుసలో_చేర్చు(rt_rq);
		rq_unlock(rq, &rf);
	}

	ఒకవేళ (!throttled && (!rt_bandwidth_enabled() || rt_b->rt_runtime == RUNTIME_INF))
		తిరిగి 1;

	తిరిగి idle;
}

స్థిర పూర్ణాంకం sched_rt_runtime_exceeded(నిర్మాణం rt_rq *rt_rq)
{
	u64 runtime = sched_rt_runtime(rt_rq);

	ఒకవేళ (rt_rq->rt_throttled)
		తిరిగి rt_rq_throttled(rt_rq);

	ఒకవేళ (runtime >= sched_rt_period(rt_rq))
		తిరిగి 0;

	balance_runtime(rt_rq);
	runtime = sched_rt_runtime(rt_rq);
	ఒకవేళ (runtime == RUNTIME_INF)
		తిరిగి 0;

	ఒకవేళ (rt_rq->rt_సమయం > runtime) {
		నిర్మాణం rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);

		/*
		 * Don't actually throttle groups that have no runtime assigned
		 * but accrue some time due to boosting.
		 */
		ఒకవేళ (likely(rt_b->rt_runtime)) {
			rt_rq->rt_throttled = 1;
			printk_deferred_once("sched: RT throttling activated\n");
		} లేకపోతే {
			/*
			 * In case we did anyway, make it go away,
			 * replenishment is a joke, since it will replenish us
			 * with exactly 0 ns.
			 */
			rt_rq->rt_సమయం = 0;
		}

		ఒకవేళ (rt_rq_throttled(rt_rq)) {
			sched_rt_rq_వరుస_నుండి_తీయి(rt_rq);
			తిరిగి 1;
		}
	}

	తిరిగి 0;
}

#లేకపోతే /* !CONFIG_RT_GROUP_SCHED: */

రకనిర్వచనం నిర్మాణం rt_rq *rt_rq_iter_t;

#నిర్వచించు for_each_rt_rq(rt_rq, iter, rq) \
	కొరకు ((శూన్య) iter, rt_rq = &rq->rt; rt_rq; rt_rq = ఖాళీ)

#నిర్వచించు for_each_sched_rt_entity(rt_se) \
	కొరకు (; rt_se; rt_se = ఖాళీ)

స్థిర అంతఃవరుస నిర్మాణం rt_rq *సమూహం_rt_rq(నిర్మాణం sched_rt_entity *rt_se)
{
	తిరిగి ఖాళీ;
}

స్థిర అంతఃవరుస శూన్య sched_rt_rq_వరుసలో_చేర్చు(నిర్మాణం rt_rq *rt_rq)
{
	నిర్మాణం rq *rq = rq_యొక్క_rt_rq(rt_rq);

	ఒకవేళ (!rt_rq->rt_nr_running)
		తిరిగి;

	వరుసలో_చేర్చు_శిఖరం_rt_rq(rt_rq);
	resched_curr(rq);
}

స్థిర అంతఃవరుస శూన్య sched_rt_rq_వరుస_నుండి_తీయి(నిర్మాణం rt_rq *rt_rq)
{
	వరుస_నుండి_తీయి_శిఖరం_rt_rq(rt_rq, rt_rq->rt_nr_running);
}

స్థిర అంతఃవరుస పూర్ణాంకం rt_rq_throttled(నిర్మాణం rt_rq *rt_rq)
{
	తిరిగి false;
}

స్థిర అంతఃవరుస స్థిరరాశి నిర్మాణం cpumask *sched_rt_period_ముసుగు(శూన్య)
{
	తిరిగి cpu_online_ముసుగు;
}

స్థిర అంతఃవరుస
నిర్మాణం rt_rq *sched_rt_period_rt_rq(నిర్మాణం rt_bandwidth *rt_b, పూర్ణాంకం cpu)
{
	తిరిగి &cpu_rq(cpu)->rt;
}

స్థిర శూన్య __enable_runtime(నిర్మాణం rq *rq) { }
స్థిర శూన్య __disable_runtime(నిర్మాణం rq *rq) { }

#అయితే_ముగింపు /* !CONFIG_RT_GROUP_SCHED */

స్థిర అంతఃవరుస పూర్ణాంకం rt_se_prio(నిర్మాణం sched_rt_entity *rt_se)
{
#నిర్వచితమైతే CONFIG_RT_సమూహం_SCHED
	నిర్మాణం rt_rq *rt_rq = సమూహం_rt_rq(rt_se);

	ఒకవేళ (rt_rq)
		తిరిగి rt_rq->highest_prio.curr;
#అయితే_ముగింపు

	తిరిగి rt_task_యొక్క(rt_se)->prio;
}

/*
 * Update the current task's runtime statistics. Skip current tasks that
 * are not in our scheduling class.
 */
స్థిర శూన్య update_curr_rt(నిర్మాణం rq *rq)
{
	నిర్మాణం task_struct *donor = rq->donor;
	s64 delta_exec;

	ఒకవేళ (donor->sched_class != &rt_sched_class)
		తిరిగి;

	delta_exec = update_curr_common(rq);
	ఒకవేళ (unlikely(delta_exec <= 0))
		తిరిగి;

#నిర్వచితమైతే CONFIG_RT_సమూహం_SCHED
	నిర్మాణం sched_rt_entity *rt_se = &donor->rt;

	ఒకవేళ (!rt_bandwidth_enabled())
		తిరిగి;

	for_each_sched_rt_entity(rt_se) {
		నిర్మాణం rt_rq *rt_rq = rt_rq_యొక్క_se(rt_se);
		పూర్ణాంకం exceeded;

		ఒకవేళ (sched_rt_runtime(rt_rq) != RUNTIME_INF) {
			raw_spin_తాళం(&rt_rq->rt_runtime_తాళం);
			rt_rq->rt_సమయం += delta_exec;
			exceeded = sched_rt_runtime_exceeded(rt_rq);
			ఒకవేళ (exceeded)
				resched_curr(rq);
			raw_spin_unlock(&rt_rq->rt_runtime_తాళం);
			ఒకవేళ (exceeded)
				do_ప్రారంభం_rt_bandwidth(sched_rt_bandwidth(rt_rq));
		}
	}
#అయితే_ముగింపు /* CONFIG_RT_GROUP_SCHED */
}

స్థిర శూన్య
వరుస_నుండి_తీయి_శిఖరం_rt_rq(నిర్మాణం rt_rq *rt_rq, గుర్తులేని పూర్ణాంకం లెక్క)
{
	నిర్మాణం rq *rq = rq_యొక్క_rt_rq(rt_rq);

	BUG_ON(&rq->rt != rt_rq);

	ఒకవేళ (!rt_rq->rt_queued)
		తిరిగి;

	BUG_ON(!rq->nr_running);

	sub_nr_running(rq, లెక్క);
	rt_rq->rt_queued = 0;

}

స్థిర శూన్య
వరుసలో_చేర్చు_శిఖరం_rt_rq(నిర్మాణం rt_rq *rt_rq)
{
	నిర్మాణం rq *rq = rq_యొక్క_rt_rq(rt_rq);

	BUG_ON(&rq->rt != rt_rq);

	ఒకవేళ (rt_rq->rt_queued)
		తిరిగి;

	ఒకవేళ (rt_rq_throttled(rt_rq))
		తిరిగి;

	ఒకవేళ (rt_rq->rt_nr_running) {
		కూడు_nr_running(rq, rt_rq->rt_nr_running);
		rt_rq->rt_queued = 1;
	}

	/* Kick cpufreq (see the comment in kernel/sched/sched.h). */
	cpufreq_update_util(rq, 0);
}

స్థిర శూన్య
inc_rt_prio_smp(నిర్మాణం rt_rq *rt_rq, పూర్ణాంకం prio, పూర్ణాంకం prev_prio)
{
	నిర్మాణం rq *rq = rq_యొక్క_rt_rq(rt_rq);

	/*
	 * Change rq's cpupri only if rt_rq is the top queue.
	 */
	ఒకవేళ (IS_ENABLED(CONFIG_RT_సమూహం_SCHED) && &rq->rt != rt_rq)
		తిరిగి;

	ఒకవేళ (rq->online && prio < prev_prio)
		cpupri_సమితి(&rq->rd->cpupri, rq->cpu, prio);
}

స్థిర శూన్య
dec_rt_prio_smp(నిర్మాణం rt_rq *rt_rq, పూర్ణాంకం prio, పూర్ణాంకం prev_prio)
{
	నిర్మాణం rq *rq = rq_యొక్క_rt_rq(rt_rq);

	/*
	 * Change rq's cpupri only if rt_rq is the top queue.
	 */
	ఒకవేళ (IS_ENABLED(CONFIG_RT_సమూహం_SCHED) && &rq->rt != rt_rq)
		తిరిగి;

	ఒకవేళ (rq->online && rt_rq->highest_prio.curr != prev_prio)
		cpupri_సమితి(&rq->rd->cpupri, rq->cpu, rt_rq->highest_prio.curr);
}

స్థిర శూన్య
inc_rt_prio(నిర్మాణం rt_rq *rt_rq, పూర్ణాంకం prio)
{
	పూర్ణాంకం prev_prio = rt_rq->highest_prio.curr;

	ఒకవేళ (prio < prev_prio)
		rt_rq->highest_prio.curr = prio;

	inc_rt_prio_smp(rt_rq, prio, prev_prio);
}

స్థిర శూన్య
dec_rt_prio(నిర్మాణం rt_rq *rt_rq, పూర్ణాంకం prio)
{
	పూర్ణాంకం prev_prio = rt_rq->highest_prio.curr;

	ఒకవేళ (rt_rq->rt_nr_running) {

		WARN_ON(prio < prev_prio);

		/*
		 * This may have been our highest task, and therefore
		 * we may have some re-computation to do
		 */
		ఒకవేళ (prio == prev_prio) {
			నిర్మాణం rt_prio_array *array = &rt_rq->చురుకైన;

			rt_rq->highest_prio.curr =
				sched_కనుగొను_మొదటి_బిట్టు(array->bitmap);
		}

	} లేకపోతే {
		rt_rq->highest_prio.curr = MAX_RT_PRIO-1;
	}

	dec_rt_prio_smp(rt_rq, prio, prev_prio);
}

#నిర్వచితమైతే CONFIG_RT_సమూహం_SCHED

స్థిర శూన్య
inc_rt_సమూహం(నిర్మాణం sched_rt_entity *rt_se, నిర్మాణం rt_rq *rt_rq)
{
	ఒకవేళ (rt_se_boosted(rt_se))
		rt_rq->rt_nr_boosted++;

	ప్రారంభం_rt_bandwidth(&rt_rq->tg->rt_bandwidth);
}

స్థిర శూన్య
dec_rt_సమూహం(నిర్మాణం sched_rt_entity *rt_se, నిర్మాణం rt_rq *rt_rq)
{
	ఒకవేళ (rt_se_boosted(rt_se))
		rt_rq->rt_nr_boosted--;

	WARN_ON(!rt_rq->rt_nr_running && rt_rq->rt_nr_boosted);
}

#లేకపోతే /* !CONFIG_RT_GROUP_SCHED: */

స్థిర శూన్య
inc_rt_సమూహం(నిర్మాణం sched_rt_entity *rt_se, నిర్మాణం rt_rq *rt_rq)
{
}

స్థిర అంతఃవరుస
శూన్య dec_rt_సమూహం(నిర్మాణం sched_rt_entity *rt_se, నిర్మాణం rt_rq *rt_rq) {}

#అయితే_ముగింపు /* !CONFIG_RT_GROUP_SCHED */

స్థిర అంతఃవరుస
గుర్తులేని పూర్ణాంకం rt_se_nr_running(నిర్మాణం sched_rt_entity *rt_se)
{
	నిర్మాణం rt_rq *సమూహం_rq = సమూహం_rt_rq(rt_se);

	ఒకవేళ (సమూహం_rq)
		తిరిగి సమూహం_rq->rt_nr_running;
	లేకపోతే
		తిరిగి 1;
}

స్థిర అంతఃవరుస
గుర్తులేని పూర్ణాంకం rt_se_rr_nr_running(నిర్మాణం sched_rt_entity *rt_se)
{
	నిర్మాణం rt_rq *సమూహం_rq = సమూహం_rt_rq(rt_se);
	నిర్మాణం task_struct *tsk;

	ఒకవేళ (సమూహం_rq)
		తిరిగి సమూహం_rq->rr_nr_running;

	tsk = rt_task_యొక్క(rt_se);

	తిరిగి (tsk->policy == SCHED_RR) ? 1 : 0;
}

స్థిర అంతఃవరుస
శూన్య inc_rt_tasks(నిర్మాణం sched_rt_entity *rt_se, నిర్మాణం rt_rq *rt_rq)
{
	పూర్ణాంకం prio = rt_se_prio(rt_se);

	WARN_ON(!rt_prio(prio));
	rt_rq->rt_nr_running += rt_se_nr_running(rt_se);
	rt_rq->rr_nr_running += rt_se_rr_nr_running(rt_se);

	inc_rt_prio(rt_rq, prio);
	inc_rt_సమూహం(rt_se, rt_rq);
}

స్థిర అంతఃవరుస
శూన్య dec_rt_tasks(నిర్మాణం sched_rt_entity *rt_se, నిర్మాణం rt_rq *rt_rq)
{
	WARN_ON(!rt_prio(rt_se_prio(rt_se)));
	WARN_ON(!rt_rq->rt_nr_running);
	rt_rq->rt_nr_running -= rt_se_nr_running(rt_se);
	rt_rq->rr_nr_running -= rt_se_rr_nr_running(rt_se);

	dec_rt_prio(rt_rq, rt_se_prio(rt_se));
	dec_rt_సమూహం(rt_se, rt_rq);
}

/*
 * Change rt_se->run_list location unless SAVE && !MOVE
 *
 * assumes ENQUEUE/DEQUEUE flags match
 */
స్థిర అంతఃవరుస bool move_entity(గుర్తులేని పూర్ణాంకం flags)
{
	ఒకవేళ ((flags & (వరుస_నుండి_తీయి_SAVE | వరుస_నుండి_తీయి_MOVE)) == వరుస_నుండి_తీయి_SAVE)
		తిరిగి false;

	తిరిగి true;
}

స్థిర శూన్య __delist_rt_entity(నిర్మాణం sched_rt_entity *rt_se, నిర్మాణం rt_prio_array *array)
{
	list_del_init(&rt_se->నిరంతరం_list);

	ఒకవేళ (list_ఖాళీలు(array->వరుస + rt_se_prio(rt_se)))
		__clear_bit(rt_se_prio(rt_se), array->bitmap);

	rt_se->on_list = 0;
}

స్థిర అంతఃవరుస నిర్మాణం sched_statistics *
__schedstats_from_rt_se(నిర్మాణం sched_rt_entity *rt_se)
{
	/* schedstats is not supported for rt group. */
	ఒకవేళ (!rt_entity_is_task(rt_se))
		తిరిగి ఖాళీ;

	తిరిగి &rt_task_యొక్క(rt_se)->stats;
}

స్థిర అంతఃవరుస శూన్య
update_stats_wait_ప్రారంభం_rt(నిర్మాణం rt_rq *rt_rq, నిర్మాణం sched_rt_entity *rt_se)
{
	నిర్మాణం sched_statistics *stats;
	నిర్మాణం task_struct *p = ఖాళీ;

	ఒకవేళ (!schedstat_enabled())
		తిరిగి;

	ఒకవేళ (rt_entity_is_task(rt_se))
		p = rt_task_యొక్క(rt_se);

	stats = __schedstats_from_rt_se(rt_se);
	ఒకవేళ (!stats)
		తిరిగి;

	__update_stats_wait_start(rq_యొక్క_rt_rq(rt_rq), p, stats);
}

స్థిర అంతఃవరుస శూన్య
update_stats_వరుసలో_చేర్చు_sleeper_rt(నిర్మాణం rt_rq *rt_rq, నిర్మాణం sched_rt_entity *rt_se)
{
	నిర్మాణం sched_statistics *stats;
	నిర్మాణం task_struct *p = ఖాళీ;

	ఒకవేళ (!schedstat_enabled())
		తిరిగి;

	ఒకవేళ (rt_entity_is_task(rt_se))
		p = rt_task_యొక్క(rt_se);

	stats = __schedstats_from_rt_se(rt_se);
	ఒకవేళ (!stats)
		తిరిగి;

	__update_stats_enqueue_sleeper(rq_యొక్క_rt_rq(rt_rq), p, stats);
}

స్థిర అంతఃవరుస శూన్య
update_stats_వరుసలో_చేర్చు_rt(నిర్మాణం rt_rq *rt_rq, నిర్మాణం sched_rt_entity *rt_se,
			పూర్ణాంకం flags)
{
	ఒకవేళ (!schedstat_enabled())
		తిరిగి;

	ఒకవేళ (flags & వరుసలో_చేర్చు_WAKEUP)
		update_stats_వరుసలో_చేర్చు_sleeper_rt(rt_rq, rt_se);
}

స్థిర అంతఃవరుస శూన్య
update_stats_wait_చివర_rt(నిర్మాణం rt_rq *rt_rq, నిర్మాణం sched_rt_entity *rt_se)
{
	నిర్మాణం sched_statistics *stats;
	నిర్మాణం task_struct *p = ఖాళీ;

	ఒకవేళ (!schedstat_enabled())
		తిరిగి;

	ఒకవేళ (rt_entity_is_task(rt_se))
		p = rt_task_యొక్క(rt_se);

	stats = __schedstats_from_rt_se(rt_se);
	ఒకవేళ (!stats)
		తిరిగి;

	__update_stats_wait_end(rq_యొక్క_rt_rq(rt_rq), p, stats);
}

స్థిర అంతఃవరుస శూన్య
update_stats_వరుస_నుండి_తీయి_rt(నిర్మాణం rt_rq *rt_rq, నిర్మాణం sched_rt_entity *rt_se,
			పూర్ణాంకం flags)
{
	నిర్మాణం task_struct *p = ఖాళీ;
	నిర్మాణం rq *rq = rq_యొక్క_rt_rq(rt_rq);

	ఒకవేళ (!schedstat_enabled())
		తిరిగి;

	ఒకవేళ (rt_entity_is_task(rt_se)) {
		p = rt_task_యొక్క(rt_se);

		ఒకవేళ (p != rq->curr)
			update_stats_wait_చివర_rt(rt_rq, rt_se);
	}

	ఒకవేళ ((flags & వరుస_నుండి_తీయి_SLEEP) && p) {
		గుర్తులేని పూర్ణాంకం state;

		state = READ_ONCE(p->__state);
		ఒకవేళ (state & TASK_INTERRUPTIBLE)
			__schedstat_set(p->stats.sleep_ప్రారంభం,
					rq_clock(rq_యొక్క_rt_rq(rt_rq)));

		ఒకవేళ (state & TASK_UNINTERRUPTIBLE)
			__schedstat_set(p->stats.ఖండం_ప్రారంభం,
					rq_clock(rq_యొక్క_rt_rq(rt_rq)));
	}
}

స్థిర శూన్య __enqueue_rt_entity(నిర్మాణం sched_rt_entity *rt_se, గుర్తులేని పూర్ణాంకం flags)
{
	నిర్మాణం rt_rq *rt_rq = rt_rq_యొక్క_se(rt_se);
	నిర్మాణం rt_prio_array *array = &rt_rq->చురుకైన;
	నిర్మాణం rt_rq *సమూహం_rq = సమూహం_rt_rq(rt_se);
	నిర్మాణం list_తల *వరుస = array->వరుస + rt_se_prio(rt_se);

	/*
	 * Don't enqueue the group if its throttled, or when empty.
	 * The latter is a consequence of the former when a child group
	 * get throttled and the current group doesn't have any other
	 * active members.
	 */
	ఒకవేళ (సమూహం_rq && (rt_rq_throttled(సమూహం_rq) || !సమూహం_rq->rt_nr_running)) {
		ఒకవేళ (rt_se->on_list)
			__delist_rt_entity(rt_se, array);
		తిరిగి;
	}

	ఒకవేళ (move_entity(flags)) {
		WARN_ON_ONCE(rt_se->on_list);
		ఒకవేళ (flags & వరుసలో_చేర్చు_తల)
			list_కూడు(&rt_se->నిరంతరం_list, వరుస);
		లేకపోతే
			list_కూడు_తోక(&rt_se->నిరంతరం_list, వరుస);

		__set_bit(rt_se_prio(rt_se), array->bitmap);
		rt_se->on_list = 1;
	}
	rt_se->on_rq = 1;

	inc_rt_tasks(rt_se, rt_rq);
}

స్థిర శూన్య __dequeue_rt_entity(నిర్మాణం sched_rt_entity *rt_se, గుర్తులేని పూర్ణాంకం flags)
{
	నిర్మాణం rt_rq *rt_rq = rt_rq_యొక్క_se(rt_se);
	నిర్మాణం rt_prio_array *array = &rt_rq->చురుకైన;

	ఒకవేళ (move_entity(flags)) {
		WARN_ON_ONCE(!rt_se->on_list);
		__delist_rt_entity(rt_se, array);
	}
	rt_se->on_rq = 0;

	dec_rt_tasks(rt_se, rt_rq);
}

/*
 * Because the prio of an upper entry depends on the lower
 * entries, we must remove entries top - down.
 */
స్థిర శూన్య వరుస_నుండి_తీయి_rt_దొంతర(నిర్మాణం sched_rt_entity *rt_se, గుర్తులేని పూర్ణాంకం flags)
{
	నిర్మాణం sched_rt_entity *వెనుక = ఖాళీ;
	గుర్తులేని పూర్ణాంకం rt_nr_running;

	for_each_sched_rt_entity(rt_se) {
		rt_se->వెనుక = వెనుక;
		వెనుక = rt_se;
	}

	rt_nr_running = rt_rq_యొక్క_se(వెనుక)->rt_nr_running;

	కొరకు (rt_se = వెనుక; rt_se; rt_se = rt_se->వెనుక) {
		ఒకవేళ (on_rt_rq(rt_se))
			__dequeue_rt_entity(rt_se, flags);
	}

	వరుస_నుండి_తీయి_శిఖరం_rt_rq(rt_rq_యొక్క_se(వెనుక), rt_nr_running);
}

స్థిర శూన్య వరుసలో_చేర్చు_rt_entity(నిర్మాణం sched_rt_entity *rt_se, గుర్తులేని పూర్ణాంకం flags)
{
	నిర్మాణం rq *rq = rq_యొక్క_rt_se(rt_se);

	update_stats_వరుసలో_చేర్చు_rt(rt_rq_యొక్క_se(rt_se), rt_se, flags);

	వరుస_నుండి_తీయి_rt_దొంతర(rt_se, flags);
	for_each_sched_rt_entity(rt_se)
		__enqueue_rt_entity(rt_se, flags);
	వరుసలో_చేర్చు_శిఖరం_rt_rq(&rq->rt);
}

స్థిర శూన్య వరుస_నుండి_తీయి_rt_entity(నిర్మాణం sched_rt_entity *rt_se, గుర్తులేని పూర్ణాంకం flags)
{
	నిర్మాణం rq *rq = rq_యొక్క_rt_se(rt_se);

	update_stats_వరుస_నుండి_తీయి_rt(rt_rq_యొక్క_se(rt_se), rt_se, flags);

	వరుస_నుండి_తీయి_rt_దొంతర(rt_se, flags);

	for_each_sched_rt_entity(rt_se) {
		నిర్మాణం rt_rq *rt_rq = సమూహం_rt_rq(rt_se);

		ఒకవేళ (rt_rq && rt_rq->rt_nr_running)
			__enqueue_rt_entity(rt_se, flags);
	}
	వరుసలో_చేర్చు_శిఖరం_rt_rq(&rq->rt);
}

/*
 * Adding/removing a task to/from a priority array:
 */
స్థిర శూన్య
వరుసలో_చేర్చు_task_rt(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, పూర్ణాంకం flags)
{
	నిర్మాణం sched_rt_entity *rt_se = &p->rt;

	ఒకవేళ (flags & వరుసలో_చేర్చు_WAKEUP)
		rt_se->timeout = 0;

	check_schedstat_required();
	update_stats_wait_ప్రారంభం_rt(rt_rq_యొక్క_se(rt_se), rt_se);

	వరుసలో_చేర్చు_rt_entity(rt_se, flags);

	ఒకవేళ (task_is_blocked(p))
		తిరిగి;

	ఒకవేళ (!task_current(rq, p) && p->nr_cpus_allowed > 1)
		వరుసలో_చేర్చు_pushable_task(rq, p);
}

స్థిర bool వరుస_నుండి_తీయి_task_rt(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, పూర్ణాంకం flags)
{
	నిర్మాణం sched_rt_entity *rt_se = &p->rt;

	update_curr_rt(rq);
	వరుస_నుండి_తీయి_rt_entity(rt_se, flags);

	వరుస_నుండి_తీయి_pushable_task(rq, p);

	తిరిగి true;
}

/*
 * Put task to the head or the end of the run list without the overhead of
 * dequeue followed by enqueue.
 */
స్థిర శూన్య
requeue_rt_entity(నిర్మాణం rt_rq *rt_rq, నిర్మాణం sched_rt_entity *rt_se, పూర్ణాంకం తల)
{
	ఒకవేళ (on_rt_rq(rt_se)) {
		నిర్మాణం rt_prio_array *array = &rt_rq->చురుకైన;
		నిర్మాణం list_తల *వరుస = array->వరుస + rt_se_prio(rt_se);

		ఒకవేళ (తల)
			list_move(&rt_se->నిరంతరం_list, వరుస);
		లేకపోతే
			list_move_తోక(&rt_se->నిరంతరం_list, వరుస);
	}
}

స్థిర శూన్య requeue_task_rt(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, పూర్ణాంకం తల)
{
	నిర్మాణం sched_rt_entity *rt_se = &p->rt;
	నిర్మాణం rt_rq *rt_rq;

	for_each_sched_rt_entity(rt_se) {
		rt_rq = rt_rq_యొక్క_se(rt_se);
		requeue_rt_entity(rt_rq, rt_se, తల);
	}
}

స్థిర శూన్య yield_task_rt(నిర్మాణం rq *rq)
{
	requeue_task_rt(rq, rq->donor, 0);
}

స్థిర పూర్ణాంకం కనుగొను_అత్యల్ప_rq(నిర్మాణం task_struct *task);

స్థిర పూర్ణాంకం
select_task_rq_rt(నిర్మాణం task_struct *p, పూర్ణాంకం cpu, పూర్ణాంకం flags)
{
	నిర్మాణం task_struct *curr, *donor;
	నిర్మాణం rq *rq;
	bool test;

	/* For anything but wake ups, just return the task_cpu */
	ఒకవేళ (!(flags & (WF_TTWU | WF_FORK)))
		వెళ్ళు బహిర్గతం;

	rq = cpu_rq(cpu);

	rcu_read_తాళం();
	curr = READ_ONCE(rq->curr); /* unlocked access */
	donor = READ_ONCE(rq->donor);

	/*
	 * If the current task on @p's runqueue is an RT task, then
	 * try to see if we can wake this RT task up on another
	 * runqueue. Otherwise simply start this RT task
	 * on its current runqueue.
	 *
	 * We want to avoid overloading runqueues. If the woken
	 * task is a higher priority, then it will stay on this CPU
	 * and the lower prio task should be moved to another CPU.
	 * Even though this will probably make the lower prio task
	 * lose its cache, we do not want to bounce a higher task
	 * around just because it gave up its CPU, perhaps for a
	 * lock?
	 *
	 * For equal prio tasks, we just let the scheduler sort it out.
	 *
	 * Otherwise, just let it ride on the affine RQ and the
	 * post-schedule router will push the preempted task away
	 *
	 * This test is optimistic, if we get it wrong the load-balancer
	 * will have to sort it out.
	 *
	 * We take into account the capacity of the CPU to ensure it fits the
	 * requirement of the task - which is only important on heterogeneous
	 * systems like big.LITTLE.
	 */
	test = curr &&
	       unlikely(rt_task(donor)) &&
	       (curr->nr_cpus_allowed < 2 || donor->prio <= p->prio);

	ఒకవేళ (test || !rt_task_సరిపోతుంది_సామర్థ్యం(p, cpu)) {
		పూర్ణాంకం లక్ష్యం = కనుగొను_అత్యల్ప_rq(p);

		/*
		 * Bail out if we were forcing a migration to find a better
		 * fitting CPU but our search failed.
		 */
		ఒకవేళ (!test && లక్ష్యం != -1 && !rt_task_సరిపోతుంది_సామర్థ్యం(p, లక్ష్యం))
			వెళ్ళు బహిర్గతం_unlock;

		/*
		 * Don't bother moving it if the destination CPU is
		 * not running a lower priority task.
		 */
		ఒకవేళ (లక్ష్యం != -1 &&
		    p->prio < cpu_rq(లక్ష్యం)->rt.highest_prio.curr)
			cpu = లక్ష్యం;
	}

బహిర్గతం_unlock:
	rcu_read_unlock();

బహిర్గతం:
	తిరిగి cpu;
}

స్థిర శూన్య check_preempt_equal_prio(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
	ఒకవేళ (rq->curr->nr_cpus_allowed == 1 ||
	    !cpupri_కనుగొను(&rq->rd->cpupri, rq->donor, ఖాళీ))
		తిరిగి;

	/*
	 * p is migratable, so let's not schedule it and
	 * see if it is pushed or pulled somewhere else.
	 */
	ఒకవేళ (p->nr_cpus_allowed != 1 &&
	    cpupri_కనుగొను(&rq->rd->cpupri, p, ఖాళీ))
		తిరిగి;

	/*
	 * There appear to be other CPUs that can accept
	 * the current task but none can run 'p', so lets reschedule
	 * to try and push the current task away:
	 */
	requeue_task_rt(rq, p, 1);
	resched_curr(rq);
}

స్థిర పూర్ణాంకం balance_rt(నిర్మాణం rq *rq, నిర్మాణం rq_flags *rf)
{
	/*
	 * Note, rq->donor may change during rq lock drops,
	 * so don't re-use p across lock drops
	 */
	నిర్మాణం task_struct *p = rq->donor;

	ఒకవేళ (!on_rt_rq(&p->rt) && need_pull_rt_task(rq, p)) {
		/*
		 * This is OK, because current is on_cpu, which avoids it being
		 * picked for load-balance and preemption/IRQs are still
		 * disabled avoiding further scheduler activity on it and we've
		 * not yet started the picking loop.
		 */
		rq_unpin_తాళం(rq, rf);
		pull_rt_task(rq);
		rq_repin_తాళం(rq, rf);
	}

	తిరిగి sched_stop_runnable(rq) || sched_dl_runnable(rq) || sched_rt_runnable(rq);
}

/*
 * Preempt the current task with a newly woken task if needed:
 */
స్థిర శూన్య wakeup_preempt_rt(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, పూర్ణాంకం flags)
{
	నిర్మాణం task_struct *donor = rq->donor;

	/*
	 * XXX If we're preempted by DL, queue a push?
	 */
	ఒకవేళ (p->sched_class != &rt_sched_class ||
	    donor->sched_class != &rt_sched_class)
		తిరిగి;

	ఒకవేళ (p->prio < donor->prio) {
		resched_curr(rq);
		తిరిగి;
	}

	/*
	 * If:
	 *
	 * - the newly woken task is of equal priority to the current task
	 * - the newly woken task is non-migratable while current is migratable
	 * - current will be preempted on the next reschedule
	 *
	 * we should check to see if current can readily move to a different
	 * cpu.  If so, we will reschedule to allow the push logic to try
	 * to move current somewhere else, making room for our non-migratable
	 * task.
	 */
	ఒకవేళ (p->prio == donor->prio && !test_tsk_need_resched(rq->curr))
		check_preempt_equal_prio(rq, p);
}

స్థిర అంతఃవరుస శూన్య సమితి_తదుపరి_task_rt(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, గణన snt_e type)
{
	నిర్మాణం sched_rt_entity *rt_se = &p->rt;
	నిర్మాణం rt_rq *rt_rq = &rq->rt;

	ఒకవేళ (type == SNT_REPICK)
		తిరిగి;

	p->se.exec_ప్రారంభం = rq_clock_task(rq);
	ఒకవేళ (on_rt_rq(&p->rt))
		update_stats_wait_చివర_rt(rt_rq, rt_se);

	/* The running task is never eligible for pushing */
	వరుస_నుండి_తీయి_pushable_task(rq, p);

	ఒకవేళ (type != SNT_PICK)
		తిరిగి;

	/*
	 * If prev task was rt, put_prev_task() has already updated the
	 * utilization. We only care of the case where we start to schedule a
	 * rt task
	 */
	ఒకవేళ (rq->donor->sched_class != &rt_sched_class)
		update_rt_rq_ఎక్కించు_avg(rq_clock_pelt(rq), rq, 0);

	rt_వరుస_నెట్టు_tasks(rq);
}

స్థిర నిర్మాణం sched_rt_entity *pick_తదుపరి_rt_entity(నిర్మాణం rt_rq *rt_rq)
{
	నిర్మాణం rt_prio_array *array = &rt_rq->చురుకైన;
	నిర్మాణం sched_rt_entity *తదుపరి = ఖాళీ;
	నిర్మాణం list_తల *వరుస;
	పూర్ణాంకం idx;

	idx = sched_కనుగొను_మొదటి_బిట్టు(array->bitmap);
	BUG_ON(idx >= MAX_RT_PRIO);

	వరుస = array->వరుస + idx;
	ఒకవేళ (WARN_ON_ONCE(list_ఖాళీలు(వరుస)))
		తిరిగి ఖాళీ;
	తదుపరి = list_నమోదు(వరుస->తదుపరి, నిర్మాణం sched_rt_entity, నిరంతరం_list);

	తిరిగి తదుపరి;
}

స్థిర నిర్మాణం task_struct *pick_తదుపరి_task_rt(నిర్మాణం rq *rq)
{
	నిర్మాణం sched_rt_entity *rt_se;
	నిర్మాణం rt_rq *rt_rq  = &rq->rt;

	చేయి {
		rt_se = pick_తదుపరి_rt_entity(rt_rq);
		ఒకవేళ (unlikely(!rt_se))
			తిరిగి ఖాళీ;
		rt_rq = సమూహం_rt_rq(rt_se);
	} అయితే (rt_rq);

	తిరిగి rt_task_యొక్క(rt_se);
}

స్థిర నిర్మాణం task_struct *pick_task_rt(నిర్మాణం rq *rq, నిర్మాణం rq_flags *rf)
{
	నిర్మాణం task_struct *p;

	ఒకవేళ (!sched_rt_runnable(rq))
		తిరిగి ఖాళీ;

	p = pick_తదుపరి_task_rt(rq);

	తిరిగి p;
}

స్థిర శూన్య put_prev_task_rt(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, నిర్మాణం task_struct *తదుపరి)
{
	నిర్మాణం sched_rt_entity *rt_se = &p->rt;
	నిర్మాణం rt_rq *rt_rq = &rq->rt;

	ఒకవేళ (on_rt_rq(&p->rt))
		update_stats_wait_ప్రారంభం_rt(rt_rq, rt_se);

	update_curr_rt(rq);

	update_rt_rq_ఎక్కించు_avg(rq_clock_pelt(rq), rq, 1);

	ఒకవేళ (task_is_blocked(p))
		తిరిగి;
	/*
	 * The previous task needs to be made eligible for pushing
	 * if it is still active
	 */
	ఒకవేళ (on_rt_rq(&p->rt) && p->nr_cpus_allowed > 1)
		వరుసలో_చేర్చు_pushable_task(rq, p);
}

/* Only try algorithms three times */
#నిర్వచించు RT_MAX_TRIES 3

/*
 * Return the highest pushable rq's task, which is suitable to be executed
 * on the CPU, NULL otherwise
 */
స్థిర నిర్మాణం task_struct *pick_highest_pushable_task(నిర్మాణం rq *rq, పూర్ణాంకం cpu)
{
	నిర్మాణం plist_తల *తల = &rq->rt.pushable_tasks;
	నిర్మాణం task_struct *p;

	ఒకవేళ (!has_pushable_tasks(rq))
		తిరిగి ఖాళీ;

	plist_for_each_నమోదు(p, తల, pushable_tasks) {
		ఒకవేళ (task_is_pushable(rq, p, cpu))
			తిరిగి p;
	}

	తిరిగి ఖాళీ;
}

స్థిర DEFINE_చొప్పున_CPU(cpumask_var_t, local_cpu_ముసుగు);

స్థిర పూర్ణాంకం కనుగొను_అత్యల్ప_rq(నిర్మాణం task_struct *task)
{
	నిర్మాణం sched_domain *sd;
	నిర్మాణం cpumask *అత్యల్ప_ముసుగు = this_cpu_cpumask_var_ptr(local_cpu_ముసుగు);
	పూర్ణాంకం this_cpu = smp_processor_id();
	పూర్ణాంకం cpu      = task_cpu(task);
	పూర్ణాంకం ret;

	/* Make sure the mask is initialized first */
	ఒకవేళ (unlikely(!అత్యల్ప_ముసుగు))
		తిరిగి -1;

	ఒకవేళ (task->nr_cpus_allowed == 1)
		తిరిగి -1; /* No other targets possible */

	/*
	 * If we're on asym system ensure we consider the different capacities
	 * of the CPUs when searching for the lowest_mask.
	 */
	ఒకవేళ (sched_asym_cpucap_చురుకైన()) {

		ret = cpupri_కనుగొను_fitness(&task_rq(task)->rd->cpupri,
					  task, అత్యల్ప_ముసుగు,
					  rt_task_సరిపోతుంది_సామర్థ్యం);
	} లేకపోతే {

		ret = cpupri_కనుగొను(&task_rq(task)->rd->cpupri,
				  task, అత్యల్ప_ముసుగు);
	}

	ఒకవేళ (!ret)
		తిరిగి -1; /* No targets found */

	/*
	 * At this point we have built a mask of CPUs representing the
	 * lowest priority tasks in the system.  Now we want to elect
	 * the best one based on our affinity and topology.
	 *
	 * We prioritize the last CPU that the task executed on since
	 * it is most likely cache-hot in that location.
	 */
	ఒకవేళ (cpumask_test_cpu(cpu, అత్యల్ప_ముసుగు))
		తిరిగి cpu;

	/*
	 * Otherwise, we consult the sched_domains span maps to figure
	 * out which CPU is logically closest to our hot cache data.
	 */
	ఒకవేళ (!cpumask_test_cpu(this_cpu, అత్యల్ప_ముసుగు))
		this_cpu = -1; /* Skip this_cpu opt if not among lowest */

	rcu_read_తాళం();
	for_each_domain(cpu, sd) {
		ఒకవేళ (sd->flags & SD_WAKE_AFFINE) {
			పూర్ణాంకం ఉత్తమ_cpu;

			/*
			 * "this_cpu" is cheaper to preempt than a
			 * remote processor.
			 */
			ఒకవేళ (this_cpu != -1 &&
			    cpumask_test_cpu(this_cpu, sched_domain_పరిధి(sd))) {
				rcu_read_unlock();
				తిరిగి this_cpu;
			}

			ఉత్తమ_cpu = cpumask_any_and_distribute(అత్యల్ప_ముసుగు,
							      sched_domain_పరిధి(sd));
			ఒకవేళ (ఉత్తమ_cpu < nr_cpu_ids) {
				rcu_read_unlock();
				తిరిగి ఉత్తమ_cpu;
			}
		}
	}
	rcu_read_unlock();

	/*
	 * And finally, if there were no matches within the domains
	 * just give the caller *something* to work with from the compatible
	 * locations.
	 */
	ఒకవేళ (this_cpu != -1)
		తిరిగి this_cpu;

	cpu = cpumask_any_distribute(అత్యల్ప_ముసుగు);
	ఒకవేళ (cpu < nr_cpu_ids)
		తిరిగి cpu;

	తిరిగి -1;
}

స్థిర నిర్మాణం task_struct *pick_తదుపరి_pushable_task(నిర్మాణం rq *rq)
{
	నిర్మాణం plist_తల *తల = &rq->rt.pushable_tasks;
	నిర్మాణం task_struct *i, *p = ఖాళీ;

	ఒకవేళ (!has_pushable_tasks(rq))
		తిరిగి ఖాళీ;

	plist_for_each_నమోదు(i, తల, pushable_tasks) {
		/* skip tasks that cannot be migrated */
		ఒకవేళ (!task_on_cpu(rq, i) && !is_migration_disabled(i)) {
			p = i;
			ఆపు;
		}
	}

	ఒకవేళ (!p)
		తిరిగి ఖాళీ;

	BUG_ON(rq->cpu != task_cpu(p));
	BUG_ON(task_current(rq, p));
	BUG_ON(task_current_donor(rq, p));
	BUG_ON(p->nr_cpus_allowed <= 1);

	BUG_ON(!task_on_rq_queued(p));
	BUG_ON(!rt_task(p));

	తిరిగి p;
}

/* Will lock the rq it finds */
స్థిర నిర్మాణం rq *కనుగొను_తాళం_అత్యల్ప_rq(నిర్మాణం task_struct *task, నిర్మాణం rq *rq)
{
	నిర్మాణం rq *అత్యల్ప_rq = ఖాళీ;
	పూర్ణాంకం tries;
	పూర్ణాంకం cpu;

	కొరకు (tries = 0; tries < RT_MAX_TRIES; tries++) {
		cpu = కనుగొను_అత్యల్ప_rq(task);

		ఒకవేళ ((cpu == -1) || (cpu == rq->cpu))
			ఆపు;

		అత్యల్ప_rq = cpu_rq(cpu);

		ఒకవేళ (అత్యల్ప_rq->rt.highest_prio.curr <= task->prio) {
			/*
			 * Target rq has tasks of equal or higher priority,
			 * retrying does not release any lock and is unlikely
			 * to yield a different result.
			 */
			అత్యల్ప_rq = ఖాళీ;
			ఆపు;
		}

		/* if the prio of this runqueue changed, try again */
		ఒకవేళ (double_తాళం_balance(rq, అత్యల్ప_rq)) {
			/*
			 * We had to unlock the run queue. In
			 * the mean time, task could have
			 * migrated already or had its affinity changed,
			 * therefore check if the task is still at the
			 * head of the pushable tasks list.
			 * It is possible the task was scheduled, set
			 * "migrate_disabled" and then got preempted, so we must
			 * check the task migration disable flag here too.
			 */
			ఒకవేళ (unlikely(is_migration_disabled(task) ||
				     !cpumask_test_cpu(అత్యల్ప_rq->cpu, &task->cpus_ముసుగు) ||
				     task != pick_తదుపరి_pushable_task(rq))) {

				double_unlock_balance(rq, అత్యల్ప_rq);
				అత్యల్ప_rq = ఖాళీ;
				ఆపు;
			}
		}

		/* If this rq is still suitable use it. */
		ఒకవేళ (అత్యల్ప_rq->rt.highest_prio.curr > task->prio)
			ఆపు;

		/* try again */
		double_unlock_balance(rq, అత్యల్ప_rq);
		అత్యల్ప_rq = ఖాళీ;
	}

	తిరిగి అత్యల్ప_rq;
}

/*
 * If the current CPU has more than one RT task, see if the non
 * running task can migrate over to a CPU that is running a task
 * of lesser priority.
 */
స్థిర పూర్ణాంకం నెట్టు_rt_task(నిర్మాణం rq *rq, bool pull)
{
	నిర్మాణం task_struct *తదుపరి_task;
	నిర్మాణం rq *అత్యల్ప_rq;
	పూర్ణాంకం ret = 0;

	ఒకవేళ (!rq->rt.overloaded)
		తిరిగి 0;

	తదుపరి_task = pick_తదుపరి_pushable_task(rq);
	ఒకవేళ (!తదుపరి_task)
		తిరిగి 0;

retry:
	/*
	 * It's possible that the next_task slipped in of
	 * higher priority than current. If that's the case
	 * just reschedule current.
	 */
	ఒకవేళ (unlikely(తదుపరి_task->prio < rq->donor->prio)) {
		resched_curr(rq);
		తిరిగి 0;
	}

	ఒకవేళ (is_migration_disabled(తదుపరి_task)) {
		నిర్మాణం task_struct *నెట్టు_task = ఖాళీ;
		పూర్ణాంకం cpu;

		ఒకవేళ (!pull || rq->నెట్టు_busy)
			తిరిగి 0;

		/*
		 * Invoking find_lowest_rq() on anything but an RT task doesn't
		 * make sense. Per the above priority check, curr has to
		 * be of higher priority than next_task, so no need to
		 * reschedule when bailing out.
		 *
		 * Note that the stoppers are masqueraded as SCHED_FIFO
		 * (cf. sched_set_stop_task()), so we can't rely on rt_task().
		 */
		ఒకవేళ (rq->donor->sched_class != &rt_sched_class)
			తిరిగి 0;

		cpu = కనుగొను_అత్యల్ప_rq(rq->curr);
		ఒకవేళ (cpu == -1 || cpu == rq->cpu)
			తిరిగి 0;

		/*
		 * Given we found a CPU with lower priority than @next_task,
		 * therefore it should be running. However we cannot migrate it
		 * to this other CPU, instead attempt to push the current
		 * running task on this CPU away.
		 */
		నెట్టు_task = get_నెట్టు_task(rq);
		ఒకవేళ (నెట్టు_task) {
			preempt_disable();
			raw_spin_rq_unlock(rq);
			stop_one_cpu_nowait(rq->cpu, నెట్టు_cpu_stop,
					    నెట్టు_task, &rq->నెట్టు_work);
			preempt_enable();
			raw_spin_rq_తాళం(rq);
		}

		తిరిగి 0;
	}

	ఒకవేళ (WARN_ON(తదుపరి_task == rq->curr))
		తిరిగి 0;

	/* We might release rq lock */
	get_task_struct(తదుపరి_task);

	/* find_lock_lowest_rq locks the rq if found */
	అత్యల్ప_rq = కనుగొను_తాళం_అత్యల్ప_rq(తదుపరి_task, rq);
	ఒకవేళ (!అత్యల్ప_rq) {
		నిర్మాణం task_struct *task;
		/*
		 * find_lock_lowest_rq releases rq->lock
		 * so it is possible that next_task has migrated.
		 *
		 * We need to make sure that the task is still on the same
		 * run-queue and is also still the next task eligible for
		 * pushing.
		 */
		task = pick_తదుపరి_pushable_task(rq);
		ఒకవేళ (task == తదుపరి_task) {
			/*
			 * The task hasn't migrated, and is still the next
			 * eligible task, but we failed to find a run-queue
			 * to push it to.  Do not retry in this case, since
			 * other CPUs will pull from us when ready.
			 */
			వెళ్ళు బహిర్గతం;
		}

		ఒకవేళ (!task)
			/* No more tasks, just exit */
			వెళ్ళు బహిర్గతం;

		/*
		 * Something has shifted, try again.
		 */
		put_task_struct(తదుపరి_task);
		తదుపరి_task = task;
		వెళ్ళు retry;
	}

	move_queued_task_locked(rq, అత్యల్ప_rq, తదుపరి_task);
	resched_curr(అత్యల్ప_rq);
	ret = 1;

	double_unlock_balance(rq, అత్యల్ప_rq);
బహిర్గతం:
	put_task_struct(తదుపరి_task);

	తిరిగి ret;
}

స్థిర శూన్య నెట్టు_rt_tasks(నిర్మాణం rq *rq)
{
	/* push_rt_task will return true if it moved an RT */
	అయితే (నెట్టు_rt_task(rq, false))
		;
}

#నిర్వచితమైతే HAVE_RT_నెట్టు_IPI

/*
 * When a high priority task schedules out from a CPU and a lower priority
 * task is scheduled in, a check is made to see if there's any RT tasks
 * on other CPUs that are waiting to run because a higher priority RT task
 * is currently running on its CPU. In this case, the CPU with multiple RT
 * tasks queued on it (overloaded) needs to be notified that a CPU has opened
 * up that may be able to run one of its non-running queued RT tasks.
 *
 * All CPUs with overloaded RT tasks need to be notified as there is currently
 * no way to know which of these CPUs have the highest priority task waiting
 * to run. Instead of trying to take a spinlock on each of these CPUs,
 * which has shown to cause large latency when done on machines with many
 * CPUs, sending an IPI to the CPUs to have them push off the overloaded
 * RT tasks waiting to run.
 *
 * Just sending an IPI to each of the CPUs is also an issue, as on large
 * count CPU machines, this can cause an IPI storm on a CPU, especially
 * if its the only CPU with multiple RT tasks queued, and a large number
 * of CPUs scheduling a lower priority task at the same time.
 *
 * Each root domain has its own IRQ work function that can iterate over
 * all CPUs with RT overloaded tasks. Since all CPUs with overloaded RT
 * task must be checked if there's one or many CPUs that are lowering
 * their priority, there's a single IRQ work iterator that will try to
 * push off RT tasks that are waiting to run.
 *
 * When a CPU schedules a lower priority task, it will kick off the
 * IRQ work iterator that will jump to each CPU with overloaded RT tasks.
 * As it only takes the first CPU that schedules a lower priority task
 * to start the process, the rto_start variable is incremented and if
 * the atomic result is one, then that CPU will try to take the rto_lock.
 * This prevents high contention on the lock as the process handles all
 * CPUs scheduling lower priority tasks.
 *
 * All CPUs that are scheduling a lower priority task will increment the
 * rt_loop_next variable. This will make sure that the IRQ work iterator
 * checks all RT overloaded CPUs whenever a CPU schedules a new lower
 * priority task, even if the iterator is in the middle of a scan. Incrementing
 * the rt_loop_next will cause the iterator to perform another scan.
 *
 */
స్థిర పూర్ణాంకం rto_తదుపరి_cpu(నిర్మాణం మూలం_domain *rd)
{
	పూర్ణాంకం this_cpu = smp_processor_id();
	పూర్ణాంకం తదుపరి;
	పూర్ణాంకం cpu;

	/*
	 * When starting the IPI RT pushing, the rto_cpu is set to -1,
	 * rt_next_cpu() will simply return the first CPU found in
	 * the rto_mask.
	 *
	 * If rto_next_cpu() is called with rto_cpu is a valid CPU, it
	 * will return the next CPU found in the rto_mask.
	 *
	 * If there are no more CPUs left in the rto_mask, then a check is made
	 * against rto_loop and rto_loop_next. rto_loop is only updated with
	 * the rto_lock held, but any CPU may increment the rto_loop_next
	 * without any locking.
	 */
	కొరకు (;;) {

		/* When rto_cpu is -1 this acts like cpumask_first() */
		cpu = cpumask_తదుపరి(rd->rto_cpu, rd->rto_ముసుగు);

		rd->rto_cpu = cpu;

		/* Do not send IPI to self */
		ఒకవేళ (cpu == this_cpu)
			కొనసాగు;

		ఒకవేళ (cpu < nr_cpu_ids)
			తిరిగి cpu;

		rd->rto_cpu = -1;

		/*
		 * ACQUIRE ensures we see the @rto_mask changes
		 * made prior to the @next value observed.
		 *
		 * Matches WMB in rt_set_overload().
		 */
		తదుపరి = atomic_read_acquire(&rd->rto_loop_తదుపరి);

		ఒకవేళ (rd->rto_loop == తదుపరి)
			ఆపు;

		rd->rto_loop = తదుపరి;
	}

	తిరిగి -1;
}

స్థిర అంతఃవరుస bool rto_ప్రారంభం_trylock(atomic_t *v)
{
	తిరిగి !atomic_cmpxchg_acquire(v, 0, 1);
}

స్థిర అంతఃవరుస శూన్య rto_ప్రారంభం_unlock(atomic_t *v)
{
	atomic_సమితి_release(v, 0);
}

స్థిర శూన్య tell_cpu_గమ్యం_నెట్టు(నిర్మాణం rq *rq)
{
	పూర్ణాంకం cpu = -1;

	/* Keep the loop going if the IPI is currently active */
	atomic_inc(&rq->rd->rto_loop_తదుపరి);

	/* Only one CPU can initiate a loop at a time */
	ఒకవేళ (!rto_ప్రారంభం_trylock(&rq->rd->rto_loop_ప్రారంభం))
		తిరిగి;

	raw_spin_తాళం(&rq->rd->rto_తాళం);

	/*
	 * The rto_cpu is updated under the lock, if it has a valid CPU
	 * then the IPI is still running and will continue due to the
	 * update to loop_next, and nothing needs to be done here.
	 * Otherwise it is finishing up and an IPI needs to be sent.
	 */
	ఒకవేళ (rq->rd->rto_cpu < 0)
		cpu = rto_తదుపరి_cpu(rq->rd);

	raw_spin_unlock(&rq->rd->rto_తాళం);

	rto_ప్రారంభం_unlock(&rq->rd->rto_loop_ప్రారంభం);

	ఒకవేళ (cpu >= 0) {
		/* Make sure the rd does not get freed while pushing */
		sched_get_rd(rq->rd);
		irq_work_వరుస_on(&rq->rd->rto_నెట్టు_work, cpu);
	}
}

/* Called from hardirq context */
శూన్య rto_నెట్టు_irq_work_func(నిర్మాణం irq_work *work)
{
	నిర్మాణం మూలం_domain *rd =
		container_యొక్క(work, నిర్మాణం మూలం_domain, rto_నెట్టు_work);
	నిర్మాణం rq *rq;
	పూర్ణాంకం cpu;

	rq = this_rq();

	/*
	 * We do not need to grab the lock to check for has_pushable_tasks.
	 * When it gets updated, a check is made if a push is possible.
	 */
	ఒకవేళ (has_pushable_tasks(rq)) {
		raw_spin_rq_తాళం(rq);
		అయితే (నెట్టు_rt_task(rq, true))
			;
		raw_spin_rq_unlock(rq);
	}

	raw_spin_తాళం(&rd->rto_తాళం);

	/* Pass the IPI to the next rt overloaded queue */
	cpu = rto_తదుపరి_cpu(rd);

	raw_spin_unlock(&rd->rto_తాళం);

	ఒకవేళ (cpu < 0) {
		sched_put_rd(rd);
		తిరిగి;
	}

	/* Try the next RT overloaded CPU */
	irq_work_వరుస_on(&rd->rto_నెట్టు_work, cpu);
}
#అయితే_ముగింపు /* HAVE_RT_PUSH_IPI */

స్థిర శూన్య pull_rt_task(నిర్మాణం rq *this_rq)
{
	పూర్ణాంకం this_cpu = this_rq->cpu, cpu;
	bool resched = false;
	నిర్మాణం task_struct *p, *నెట్టు_task;
	నిర్మాణం rq *src_rq;
	పూర్ణాంకం rt_overload_లెక్క = rt_overloaded(this_rq);

	ఒకవేళ (likely(!rt_overload_లెక్క))
		తిరిగి;

	/*
	 * Match the barrier from rt_set_overloaded; this guarantees that if we
	 * see overloaded we must also see the rto_mask bit.
	 */
	smp_rmb();

	/* If we are the only overloaded CPU do nothing */
	ఒకవేళ (rt_overload_లెక్క == 1 &&
	    cpumask_test_cpu(this_rq->cpu, this_rq->rd->rto_ముసుగు))
		తిరిగి;

#నిర్వచితమైతే HAVE_RT_నెట్టు_IPI
	ఒకవేళ (sched_feat(RT_నెట్టు_IPI)) {
		tell_cpu_గమ్యం_నెట్టు(this_rq);
		తిరిగి;
	}
#అయితే_ముగింపు

	for_each_cpu(cpu, this_rq->rd->rto_ముసుగు) {
		ఒకవేళ (this_cpu == cpu)
			కొనసాగు;

		src_rq = cpu_rq(cpu);

		/*
		 * Don't bother taking the src_rq->lock if the next highest
		 * task is known to be lower-priority than our current task.
		 * This may look racy, but if this value is about to go
		 * logically higher, the src_rq will push this task away.
		 * And if its going logically lower, we do not care
		 */
		ఒకవేళ (src_rq->rt.highest_prio.తదుపరి >=
		    this_rq->rt.highest_prio.curr)
			కొనసాగు;

		/*
		 * We can potentially drop this_rq's lock in
		 * double_lock_balance, and another CPU could
		 * alter this_rq
		 */
		నెట్టు_task = ఖాళీ;
		double_తాళం_balance(this_rq, src_rq);

		/*
		 * We can pull only a task, which is pushable
		 * on its rq, and no others.
		 */
		p = pick_highest_pushable_task(src_rq, this_cpu);

		/*
		 * Do we have an RT task that preempts
		 * the to-be-scheduled task?
		 */
		ఒకవేళ (p && (p->prio < this_rq->rt.highest_prio.curr)) {
			WARN_ON(p == src_rq->curr);
			WARN_ON(!task_on_rq_queued(p));

			/*
			 * There's a chance that p is higher in priority
			 * than what's currently running on its CPU.
			 * This is just that p is waking up and hasn't
			 * had a chance to schedule. We only pull
			 * p if it is lower in priority than the
			 * current task on the run queue
			 */
			ఒకవేళ (p->prio < src_rq->donor->prio)
				వెళ్ళు skip;

			ఒకవేళ (is_migration_disabled(p)) {
				నెట్టు_task = get_నెట్టు_task(src_rq);
			} లేకపోతే {
				move_queued_task_locked(src_rq, this_rq, p);
				resched = true;
			}
			/*
			 * We continue with the search, just in
			 * case there's an even higher prio task
			 * in another runqueue. (low likelihood
			 * but possible)
			 */
		}
skip:
		double_unlock_balance(this_rq, src_rq);

		ఒకవేళ (నెట్టు_task) {
			preempt_disable();
			raw_spin_rq_unlock(this_rq);
			stop_one_cpu_nowait(src_rq->cpu, నెట్టు_cpu_stop,
					    నెట్టు_task, &src_rq->నెట్టు_work);
			preempt_enable();
			raw_spin_rq_తాళం(this_rq);
		}
	}

	ఒకవేళ (resched)
		resched_curr(this_rq);
}

/*
 * If we are not running and we are not going to reschedule soon, we should
 * try to push tasks away now
 */
స్థిర శూన్య task_woken_rt(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
	bool need_గమ్యం_నెట్టు = !task_on_cpu(rq, p) &&
			    !test_tsk_need_resched(rq->curr) &&
			    p->nr_cpus_allowed > 1 &&
			    (dl_task(rq->donor) || rt_task(rq->donor)) &&
			    (rq->curr->nr_cpus_allowed < 2 ||
			     rq->donor->prio <= p->prio);

	ఒకవేళ (need_గమ్యం_నెట్టు)
		నెట్టు_rt_tasks(rq);
}

/* Assumes rq->lock is held */
స్థిర శూన్య rq_online_rt(నిర్మాణం rq *rq)
{
	ఒకవేళ (rq->rt.overloaded)
		rt_సమితి_overload(rq);

	__enable_runtime(rq);

	cpupri_సమితి(&rq->rd->cpupri, rq->cpu, rq->rt.highest_prio.curr);
}

/* Assumes rq->lock is held */
స్థిర శూన్య rq_offline_rt(నిర్మాణం rq *rq)
{
	ఒకవేళ (rq->rt.overloaded)
		rt_clear_overload(rq);

	__disable_runtime(rq);

	cpupri_సమితి(&rq->rd->cpupri, rq->cpu, CPUPRI_INVALID);
}

/*
 * When switch from the rt queue, we bring ourselves to a position
 * that we might want to pull RT tasks from other runqueues.
 */
స్థిర శూన్య switched_from_rt(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
	/*
	 * If there are other RT tasks then we will reschedule
	 * and the scheduling of the other RT tasks will handle
	 * the balancing. But if we are the last RT task
	 * we may need to handle the pulling of RT tasks
	 * now.
	 */
	ఒకవేళ (!task_on_rq_queued(p) || rq->rt.rt_nr_running)
		తిరిగి;

	rt_వరుస_pull_task(rq);
}

శూన్య __init init_sched_rt_class(శూన్య)
{
	గుర్తులేని పూర్ణాంకం i;

	for_each_possible_cpu(i) {
		zalloc_cpumask_var_కణుపు(&చొప్పున_cpu(local_cpu_ముసుగు, i),
					GFP_KERNEL, cpu_గమ్యం_కణుపు(i));
	}
}

/*
 * When switching a task to RT, we may overload the runqueue
 * with RT tasks. In this case we try to push them off to
 * other runqueues.
 */
స్థిర శూన్య switched_గమ్యం_rt(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
	/*
	 * If we are running, update the avg_rt tracking, as the running time
	 * will now on be accounted into the latter.
	 */
	ఒకవేళ (task_current(rq, p)) {
		update_rt_rq_ఎక్కించు_avg(rq_clock_pelt(rq), rq, 0);
		తిరిగి;
	}

	/*
	 * If we are not running we may need to preempt the current
	 * running task. If that current running task is also an RT task
	 * then see if we can move to another run queue.
	 */
	ఒకవేళ (task_on_rq_queued(p)) {
		ఒకవేళ (p->nr_cpus_allowed > 1 && rq->rt.overloaded)
			rt_వరుస_నెట్టు_tasks(rq);
		ఒకవేళ (p->prio < rq->donor->prio && cpu_online(cpu_యొక్క(rq)))
			resched_curr(rq);
	}
}

/*
 * Priority of the task has changed. This may cause
 * us to initiate a push or pull.
 */
స్థిర శూన్య
prio_మారింది_rt(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, u64 oldprio)
{
	ఒకవేళ (!task_on_rq_queued(p))
		తిరిగి;

	ఒకవేళ (p->prio == oldprio)
		తిరిగి;

	ఒకవేళ (task_current_donor(rq, p)) {
		/*
		 * If our priority decreases while running, we
		 * may need to pull tasks to this runqueue.
		 */
		ఒకవేళ (oldprio < p->prio)
			rt_వరుస_pull_task(rq);

		/*
		 * If there's a higher priority task waiting to run
		 * then reschedule.
		 */
		ఒకవేళ (p->prio > rq->rt.highest_prio.curr)
			resched_curr(rq);
	} లేకపోతే {
		/*
		 * This task is not running, but if it is
		 * greater than the current running task
		 * then reschedule.
		 */
		ఒకవేళ (p->prio < rq->donor->prio)
			resched_curr(rq);
	}
}

#నిర్వచితమైతే CONFIG_POSIX_TIMERS
స్థిర శూన్య watchdog(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
	గుర్తులేని దీర్ఘ soft, hard;

	/* max may change after cur was read, this will be fixed next tick */
	soft = task_rlimit(p, RLIMIT_RTTIME);
	hard = task_rlimit_max(p, RLIMIT_RTTIME);

	ఒకవేళ (soft != RLIM_INFINITY) {
		గుర్తులేని దీర్ఘ తదుపరి;

		ఒకవేళ (p->rt.watchdog_stamp != jiffies) {
			p->rt.timeout++;
			p->rt.watchdog_stamp = jiffies;
		}

		తదుపరి = DIV_విడత_పైకి(min(soft, hard), USEC_చొప్పున_SEC/HZ);
		ఒకవేళ (p->rt.timeout > తదుపరి) {
			posix_cputimers_rt_watchdog(&p->posix_cputimers,
						    p->se.కూడిక_exec_runtime);
		}
	}
}
#లేకపోతే /* !CONFIG_POSIX_TIMERS: */
స్థిర అంతఃవరుస శూన్య watchdog(నిర్మాణం rq *rq, నిర్మాణం task_struct *p) { }
#అయితే_ముగింపు /* !CONFIG_POSIX_TIMERS */

/*
 * scheduler tick hitting a task of our scheduling class.
 *
 * NOTE: This function can be called remotely by the tick offload that
 * goes along full dynticks. Therefore no local assumption can be made
 * and everything must be accessed through the @rq and @curr passed in
 * parameters.
 */
స్థిర శూన్య task_tick_rt(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, పూర్ణాంకం queued)
{
	నిర్మాణం sched_rt_entity *rt_se = &p->rt;

	update_curr_rt(rq);
	update_rt_rq_ఎక్కించు_avg(rq_clock_pelt(rq), rq, 1);

	watchdog(rq, p);

	/*
	 * RR tasks need a special form of time-slice management.
	 * FIFO tasks have no timeslices.
	 */
	ఒకవేళ (p->policy != SCHED_RR)
		తిరిగి;

	ఒకవేళ (--p->rt.సమయం_ముక్క)
		తిరిగి;

	p->rt.సమయం_ముక్క = sched_rr_timeslice;

	/*
	 * Requeue to the end of queue if we (and all of our ancestors) are not
	 * the only element on the queue
	 */
	for_each_sched_rt_entity(rt_se) {
		ఒకవేళ (rt_se->నిరంతరం_list.prev != rt_se->నిరంతరం_list.తదుపరి) {
			requeue_task_rt(rq, p, 0);
			resched_curr(rq);
			తిరిగి;
		}
	}
}

స్థిర గుర్తులేని పూర్ణాంకం get_rr_interval_rt(నిర్మాణం rq *rq, నిర్మాణం task_struct *task)
{
	/*
	 * Time slice is 0 for SCHED_FIFO tasks
	 */
	ఒకవేళ (task->policy == SCHED_RR)
		తిరిగి sched_rr_timeslice;
	లేకపోతే
		తిరిగి 0;
}

#నిర్వచితమైతే CONFIG_SCHED_CORE
స్థిర పూర్ణాంకం task_is_throttled_rt(నిర్మాణం task_struct *p, పూర్ణాంకం cpu)
{
	నిర్మాణం rt_rq *rt_rq;

#నిర్వచితమైతే CONFIG_RT_సమూహం_SCHED // XXX maybe add task_rt_rq(), see also sched_rt_period_rt_rq
	rt_rq = task_సమూహం(p)->rt_rq[cpu];
	WARN_ON(!rt_సమూహం_sched_enabled() && rt_rq->tg != &మూలం_task_సమూహం);
#లేకపోతే
	rt_rq = &cpu_rq(cpu)->rt;
#అయితే_ముగింపు

	తిరిగి rt_rq_throttled(rt_rq);
}
#అయితే_ముగింపు /* CONFIG_SCHED_CORE */

DEFINE_SCHED_CLASS(rt) = {
	.వరుసలో_చేర్చు_task		= వరుసలో_చేర్చు_task_rt,
	.వరుస_నుండి_తీయి_task		= వరుస_నుండి_తీయి_task_rt,
	.yield_task		= yield_task_rt,

	.wakeup_preempt		= wakeup_preempt_rt,

	.pick_task		= pick_task_rt,
	.put_prev_task		= put_prev_task_rt,
	.సమితి_తదుపరి_task          = సమితి_తదుపరి_task_rt,

	.balance		= balance_rt,
	.select_task_rq		= select_task_rq_rt,
	.సమితి_cpus_allowed       = సమితి_cpus_allowed_common,
	.rq_online              = rq_online_rt,
	.rq_offline             = rq_offline_rt,
	.task_woken		= task_woken_rt,
	.switched_from		= switched_from_rt,
	.కనుగొను_తాళం_rq		= కనుగొను_తాళం_అత్యల్ప_rq,

	.task_tick		= task_tick_rt,

	.get_rr_interval	= get_rr_interval_rt,

	.switched_గమ్యం		= switched_గమ్యం_rt,
	.prio_మారింది		= prio_మారింది_rt,

	.update_curr		= update_curr_rt,

#నిర్వచితమైతే CONFIG_SCHED_CORE
	.task_is_throttled	= task_is_throttled_rt,
#అయితే_ముగింపు

#నిర్వచితమైతే CONFIG_UCLAMP_TASK
	.uclamp_enabled		= 1,
#అయితే_ముగింపు
};

#నిర్వచితమైతే CONFIG_RT_సమూహం_SCHED
/*
 * Ensure that the real time constraints are schedulable.
 */
స్థిర DEFINE_MUTEX(rt_constraints_mutex);

స్థిర అంతఃవరుస పూర్ణాంకం tg_has_rt_tasks(నిర్మాణం task_సమూహం *tg)
{
	నిర్మాణం task_struct *task;
	నిర్మాణం css_task_iter it;
	పూర్ణాంకం ret = 0;

	/*
	 * Autogroups do not have RT tasks; see autogroup_create().
	 */
	ఒకవేళ (task_సమూహం_is_autogroup(tg))
		తిరిగి 0;

	css_task_iter_ప్రారంభం(&tg->css, 0, &it);
	అయితే (!ret && (task = css_task_iter_తదుపరి(&it)))
		ret |= rt_task(task);
	css_task_iter_చివర(&it);

	తిరిగి ret;
}

నిర్మాణం rt_schedulable_దత్తాంశం {
	నిర్మాణం task_సమూహం *tg;
	u64 rt_period;
	u64 rt_runtime;
};

స్థిర పూర్ణాంకం tg_rt_schedulable(నిర్మాణం task_సమూహం *tg, శూన్య *దత్తాంశం)
{
	నిర్మాణం rt_schedulable_దత్తాంశం *d = దత్తాంశం;
	నిర్మాణం task_సమూహం *బిడ్డ;
	u64 మొత్తం, కూడిక = 0;
	u64 period, runtime;

	period = ktime_గమ్యం_ns(tg->rt_bandwidth.rt_period);
	runtime = tg->rt_bandwidth.rt_runtime;

	ఒకవేళ (tg == d->tg) {
		period = d->rt_period;
		runtime = d->rt_runtime;
	}

	/*
	 * Cannot have more runtime than the period.
	 */
	ఒకవేళ (runtime > period && runtime != RUNTIME_INF)
		తిరిగి -EINVAL;

	/*
	 * Ensure we don't starve existing RT tasks if runtime turns zero.
	 */
	ఒకవేళ (rt_bandwidth_enabled() && !runtime &&
	    tg->rt_bandwidth.rt_runtime && tg_has_rt_tasks(tg))
		తిరిగి -EBUSY;

	మొత్తం = గమ్యం_నిష్పత్తి(period, runtime);

	/*
	 * Nobody can have more than the global setting allows.
	 */
	ఒకవేళ (మొత్తం > గమ్యం_నిష్పత్తి(global_rt_period(), global_rt_runtime()))
		తిరిగి -EINVAL;

	/*
	 * The sum of our children's runtime should not exceed our own.
	 */
	list_for_each_నమోదు_rcu(బిడ్డ, &tg->children, siblings) {
		period = ktime_గమ్యం_ns(బిడ్డ->rt_bandwidth.rt_period);
		runtime = బిడ్డ->rt_bandwidth.rt_runtime;

		ఒకవేళ (బిడ్డ == d->tg) {
			period = d->rt_period;
			runtime = d->rt_runtime;
		}

		కూడిక += గమ్యం_నిష్పత్తి(period, runtime);
	}

	ఒకవేళ (కూడిక > మొత్తం)
		తిరిగి -EINVAL;

	తిరిగి 0;
}

స్థిర పూర్ణాంకం __rt_schedulable(నిర్మాణం task_సమూహం *tg, u64 period, u64 runtime)
{
	పూర్ణాంకం ret;

	నిర్మాణం rt_schedulable_దత్తాంశం దత్తాంశం = {
		.tg = tg,
		.rt_period = period,
		.rt_runtime = runtime,
	};

	rcu_read_తాళం();
	ret = walk_tg_వృక్షం(tg_rt_schedulable, tg_nop, &దత్తాంశం);
	rcu_read_unlock();

	తిరిగి ret;
}

స్థిర పూర్ణాంకం tg_సమితి_rt_bandwidth(నిర్మాణం task_సమూహం *tg,
		u64 rt_period, u64 rt_runtime)
{
	పూర్ణాంకం i, err = 0;

	/*
	 * Disallowing the root group RT runtime is BAD, it would disallow the
	 * kernel creating (and or operating) RT threads.
	 */
	ఒకవేళ (tg == &మూలం_task_సమూహం && rt_runtime == 0)
		తిరిగి -EINVAL;

	/* No period doesn't make any sense. */
	ఒకవేళ (rt_period == 0)
		తిరిగి -EINVAL;

	/*
	 * Bound quota to defend quota against overflow during bandwidth shift.
	 */
	ఒకవేళ (rt_runtime != RUNTIME_INF && rt_runtime > max_rt_runtime)
		తిరిగి -EINVAL;

	mutex_తాళం(&rt_constraints_mutex);
	err = __rt_schedulable(tg, rt_period, rt_runtime);
	ఒకవేళ (err)
		వెళ్ళు unlock;

	raw_spin_తాళం_irq(&tg->rt_bandwidth.rt_runtime_తాళం);
	tg->rt_bandwidth.rt_period = ns_గమ్యం_ktime(rt_period);
	tg->rt_bandwidth.rt_runtime = rt_runtime;

	for_each_possible_cpu(i) {
		నిర్మాణం rt_rq *rt_rq = tg->rt_rq[i];

		raw_spin_తాళం(&rt_rq->rt_runtime_తాళం);
		rt_rq->rt_runtime = rt_runtime;
		raw_spin_unlock(&rt_rq->rt_runtime_తాళం);
	}
	raw_spin_unlock_irq(&tg->rt_bandwidth.rt_runtime_తాళం);
unlock:
	mutex_unlock(&rt_constraints_mutex);

	తిరిగి err;
}

పూర్ణాంకం sched_సమూహం_సమితి_rt_runtime(నిర్మాణం task_సమూహం *tg, దీర్ఘ rt_runtime_us)
{
	u64 rt_runtime, rt_period;

	rt_period = ktime_గమ్యం_ns(tg->rt_bandwidth.rt_period);
	rt_runtime = (u64)rt_runtime_us * NSEC_చొప్పున_USEC;
	ఒకవేళ (rt_runtime_us < 0)
		rt_runtime = RUNTIME_INF;
	లేకపోతే ఒకవేళ ((u64)rt_runtime_us > U64_MAX / NSEC_చొప్పున_USEC)
		తిరిగి -EINVAL;

	తిరిగి tg_సమితి_rt_bandwidth(tg, rt_period, rt_runtime);
}

దీర్ఘ sched_సమూహం_rt_runtime(నిర్మాణం task_సమూహం *tg)
{
	u64 rt_runtime_us;

	ఒకవేళ (tg->rt_bandwidth.rt_runtime == RUNTIME_INF)
		తిరిగి -1;

	rt_runtime_us = tg->rt_bandwidth.rt_runtime;
	do_div(rt_runtime_us, NSEC_చొప్పున_USEC);
	తిరిగి rt_runtime_us;
}

పూర్ణాంకం sched_సమూహం_సమితి_rt_period(నిర్మాణం task_సమూహం *tg, u64 rt_period_us)
{
	u64 rt_runtime, rt_period;

	ఒకవేళ (rt_period_us > U64_MAX / NSEC_చొప్పున_USEC)
		తిరిగి -EINVAL;

	rt_period = rt_period_us * NSEC_చొప్పున_USEC;
	rt_runtime = tg->rt_bandwidth.rt_runtime;

	తిరిగి tg_సమితి_rt_bandwidth(tg, rt_period, rt_runtime);
}

దీర్ఘ sched_సమూహం_rt_period(నిర్మాణం task_సమూహం *tg)
{
	u64 rt_period_us;

	rt_period_us = ktime_గమ్యం_ns(tg->rt_bandwidth.rt_period);
	do_div(rt_period_us, NSEC_చొప్పున_USEC);
	తిరిగి rt_period_us;
}

పూర్ణాంకం sched_rt_can_attach(నిర్మాణం task_సమూహం *tg, నిర్మాణం task_struct *tsk)
{
	/* Don't accept real-time tasks when there is no way for them to run */
	ఒకవేళ (rt_సమూహం_sched_enabled() && rt_task(tsk) && tg->rt_bandwidth.rt_runtime == 0)
		తిరిగి 0;

	తిరిగి 1;
}

#అయితే_ముగింపు /* !CONFIG_RT_GROUP_SCHED */

#నిర్వచితమైతే CONFIG_SYSCTL
స్థిర పూర్ణాంకం sched_rt_global_validate(శూన్య)
{
	ఒకవేళ ((sysctl_sched_rt_runtime != RUNTIME_INF) &&
		((sysctl_sched_rt_runtime > sysctl_sched_rt_period) ||
		 ((u64)sysctl_sched_rt_runtime *
			NSEC_చొప్పున_USEC > max_rt_runtime)))
		తిరిగి -EINVAL;

#నిర్వచితమైతే CONFIG_RT_సమూహం_SCHED
	ఒకవేళ (!rt_సమూహం_sched_enabled())
		తిరిగి 0;

	scoped_guard(mutex, &rt_constraints_mutex)
		తిరిగి __rt_schedulable(ఖాళీ, 0, 0);
#అయితే_ముగింపు
	తిరిగి 0;
}

స్థిర పూర్ణాంకం sched_rt_handler(స్థిరరాశి నిర్మాణం ctl_పట్టిక *పట్టిక, పూర్ణాంకం లిఖ, శూన్య *బఫరు,
		కొలత_t *lenp, loff_t *ppos)
{
	పూర్ణాంకం old_period, old_runtime;
	స్థిర DEFINE_MUTEX(mutex);
	పూర్ణాంకం ret;

	mutex_తాళం(&mutex);
	sched_domains_mutex_తాళం();
	old_period = sysctl_sched_rt_period;
	old_runtime = sysctl_sched_rt_runtime;

	ret = proc_dointvec_minmax(పట్టిక, లిఖ, బఫరు, lenp, ppos);

	ఒకవేళ (!ret && లిఖ) {
		ret = sched_rt_global_validate();
		ఒకవేళ (ret)
			వెళ్ళు undo;

		ret = sched_dl_global_validate();
		ఒకవేళ (ret)
			వెళ్ళు undo;

		sched_dl_do_global();
	}
	ఒకవేళ (0) {
undo:
		sysctl_sched_rt_period = old_period;
		sysctl_sched_rt_runtime = old_runtime;
	}
	sched_domains_mutex_unlock();
	mutex_unlock(&mutex);

	/*
	 * After changing maximum available bandwidth for DEADLINE, we need to
	 * recompute per root domain and per cpus variables accordingly.
	 */
	rebuild_sched_domains();

	తిరిగి ret;
}

స్థిర పూర్ణాంకం sched_rr_handler(స్థిరరాశి నిర్మాణం ctl_పట్టిక *పట్టిక, పూర్ణాంకం లిఖ, శూన్య *బఫరు,
		కొలత_t *lenp, loff_t *ppos)
{
	పూర్ణాంకం ret;
	స్థిర DEFINE_MUTEX(mutex);

	mutex_తాళం(&mutex);
	ret = proc_dointvec(పట్టిక, లిఖ, బఫరు, lenp, ppos);
	/*
	 * Make sure that internally we keep jiffies.
	 * Also, writing zero resets the time-slice to default:
	 */
	ఒకవేళ (!ret && లిఖ) {
		sched_rr_timeslice =
			sysctl_sched_rr_timeslice <= 0 ? RR_TIMESLICE :
			msecs_గమ్యం_jiffies(sysctl_sched_rr_timeslice);

		ఒకవేళ (sysctl_sched_rr_timeslice <= 0)
			sysctl_sched_rr_timeslice = jiffies_గమ్యం_msecs(RR_TIMESLICE);
	}
	mutex_unlock(&mutex);

	తిరిగి ret;
}
#అయితే_ముగింపు /* CONFIG_SYSCTL */

శూన్య print_rt_stats(నిర్మాణం seq_file *m, పూర్ణాంకం cpu)
{
	rt_rq_iter_t iter;
	నిర్మాణం rt_rq *rt_rq;

	rcu_read_తాళం();
	for_each_rt_rq(rt_rq, iter, cpu_rq(cpu))
		print_rt_rq(m, cpu, rt_rq);
	rcu_read_unlock();
}
