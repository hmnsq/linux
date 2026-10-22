// SPDX-License-Identifier: GPL-2.0
/*
 * Real-Time Scheduling Class (mapped to the SCHED_FIFO and SCHED_RR
 * policies)
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "sched.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "pelt.h"

ਪੂਰਨ sched_rr_timeslice = RR_TIMESLICE;
/* More than 4 hours if BW_SHIFT equals 20. */
ਸਥਿਰ ਅਚਲ u64 max_rt_runtime = MAX_BW;

/*
 * period over which we measure -rt task CPU usage in us.
 * default: 1s
 */
ਪੂਰਨ sysctl_sched_rt_period = 1000000;

/*
 * part of the period that we allow rt tasks to run in us.
 * default: 1s
 */
ਪੂਰਨ sysctl_sched_rt_runtime = 1000000;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SYSCTL
ਸਥਿਰ ਪੂਰਨ sysctl_sched_rr_timeslice = (MSEC_ਪ੍ਰਤੀ_SEC * RR_TIMESLICE) / HZ;
ਸਥਿਰ ਪੂਰਨ sched_rt_handler(ਅਚਲ ਬਣਤਰ ctl_ਸਾਰਣੀ *ਸਾਰਣੀ, ਪੂਰਨ ਲਿਖ, ਖਾਲੀ *ਬਫਰ,
		ਮਾਪ_t *lenp, loff_t *ppos);
ਸਥਿਰ ਪੂਰਨ sched_rr_handler(ਅਚਲ ਬਣਤਰ ctl_ਸਾਰਣੀ *ਸਾਰਣੀ, ਪੂਰਨ ਲਿਖ, ਖਾਲੀ *ਬਫਰ,
		ਮਾਪ_t *lenp, loff_t *ppos);
ਸਥਿਰ ਅਚਲ ਬਣਤਰ ctl_ਸਾਰਣੀ sched_rt_sysctls[] = {
	{
		.procname       = "sched_rt_period_us",
		.ਅੰਕੜੇ           = &sysctl_sched_rt_period,
		.maxlen         = ਆਕਾਰ(ਪੂਰਨ),
		.ਬਹੁਲਕ           = 0644,
		.proc_handler   = sched_rt_handler,
		.extra1         = SYSCTL_ONE,
		.extra2         = SYSCTL_INT_MAX,
	},
	{
		.procname       = "sched_rt_runtime_us",
		.ਅੰਕੜੇ           = &sysctl_sched_rt_runtime,
		.maxlen         = ਆਕਾਰ(ਪੂਰਨ),
		.ਬਹੁਲਕ           = 0644,
		.proc_handler   = sched_rt_handler,
		.extra1         = SYSCTL_NEG_ONE,
		.extra2         = (ਖਾਲੀ *)&sysctl_sched_rt_period,
	},
	{
		.procname       = "sched_rr_timeslice_ms",
		.ਅੰਕੜੇ           = &sysctl_sched_rr_timeslice,
		.maxlen         = ਆਕਾਰ(ਪੂਰਨ),
		.ਬਹੁਲਕ           = 0644,
		.proc_handler   = sched_rr_handler,
	},
};

ਸਥਿਰ ਪੂਰਨ __init sched_rt_sysctl_init(ਖਾਲੀ)
{
	ਰਜਿਸਟਰ_sysctl_init("kernel", sched_rt_sysctls);
	ਵਾਪਸ 0;
}
late_initcall(sched_rt_sysctl_init);
#ਜੇ_ਅੰਤ /* CONFIG_SYSCTL */

ਖਾਲੀ init_rt_rq(ਬਣਤਰ rt_rq *rt_rq)
{
	ਬਣਤਰ rt_prio_array *array;
	ਪੂਰਨ i;

	array = &rt_rq->ਸਰਗਰਮ;
	ਲਈ (i = 0; i < MAX_RT_PRIO; i++) {
		INIT_LIST_ਸਿਰ(array->ਕਤਾਰ + i);
		__clear_bit(i, array->bitmap);
	}
	/* delimiter for bitsearch: */
	__set_bit(MAX_RT_PRIO, array->bitmap);

	rt_rq->highest_prio.curr = MAX_RT_PRIO-1;
	rt_rq->highest_prio.ਅਗਲਾ = MAX_RT_PRIO-1;
	rt_rq->overloaded = 0;
	plist_ਸਿਰ_init(&rt_rq->pushable_tasks);
	/* We start is dequeued state, because no RT tasks are queued */
	rt_rq->rt_queued = 0;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_RT_ਸਮੂਹ_SCHED
	rt_rq->rt_ਸਮਾਂ = 0;
	rt_rq->rt_throttled = 0;
	rt_rq->rt_runtime = 0;
	raw_spin_ਤਾਲਾ_init(&rt_rq->rt_runtime_ਤਾਲਾ);
	rt_rq->tg = &ਜੜ੍ਹ_task_ਸਮੂਹ;
#ਜੇ_ਅੰਤ
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_RT_ਸਮੂਹ_SCHED

ਸਥਿਰ ਪੂਰਨ do_sched_rt_period_timer(ਬਣਤਰ rt_bandwidth *rt_b, ਪੂਰਨ overrun);

ਸਥਿਰ ਗਿਣਤੀ hrtimer_restart sched_rt_period_timer(ਬਣਤਰ hrtimer *timer)
{
	ਬਣਤਰ rt_bandwidth *rt_b =
		container_ਦਾ(timer, ਬਣਤਰ rt_bandwidth, rt_period_timer);
	ਪੂਰਨ idle = 0;
	ਪੂਰਨ overrun;

	raw_spin_ਤਾਲਾ(&rt_b->rt_runtime_ਤਾਲਾ);
	ਲਈ (;;) {
		overrun = hrtimer_forward_now(timer, rt_b->rt_period);
		ਜੇ (!overrun)
			ਰੋਕੋ;

		raw_spin_unlock(&rt_b->rt_runtime_ਤਾਲਾ);
		idle = do_sched_rt_period_timer(rt_b, overrun);
		raw_spin_ਤਾਲਾ(&rt_b->rt_runtime_ਤਾਲਾ);
	}
	ਜੇ (idle)
		rt_b->rt_period_ਸਰਗਰਮ = 0;
	raw_spin_unlock(&rt_b->rt_runtime_ਤਾਲਾ);

	ਵਾਪਸ idle ? HRTIMER_NORESTART : HRTIMER_RESTART;
}

ਖਾਲੀ init_rt_bandwidth(ਬਣਤਰ rt_bandwidth *rt_b, u64 period, u64 runtime)
{
	rt_b->rt_period = ns_ਮੰਜ਼ਿਲ_ktime(period);
	rt_b->rt_runtime = runtime;

	raw_spin_ਤਾਲਾ_init(&rt_b->rt_runtime_ਤਾਲਾ);

	hrtimer_ਤਿਆਰੀ(&rt_b->rt_period_timer, sched_rt_period_timer, CLOCK_MONOTONIC,
		      HRTIMER_ਬਹੁਲਕ_REL_HARD);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ do_ਸ਼ੁਰੂ_rt_bandwidth(ਬਣਤਰ rt_bandwidth *rt_b)
{
	raw_spin_ਤਾਲਾ(&rt_b->rt_runtime_ਤਾਲਾ);
	ਜੇ (!rt_b->rt_period_ਸਰਗਰਮ) {
		rt_b->rt_period_ਸਰਗਰਮ = 1;
		/*
		 * SCHED_DEADLINE updates the bandwidth, as a run away
		 * RT task with a DL task could hog a CPU. But DL does
		 * not reset the period. If a deadline task was running
		 * without an RT task running, it can cause RT tasks to
		 * throttle when they start up. Kick the timer right away
		 * to update the period.
		 */
		hrtimer_forward_now(&rt_b->rt_period_timer, ns_ਮੰਜ਼ਿਲ_ktime(0));
		hrtimer_ਸ਼ੁਰੂ_expires(&rt_b->rt_period_timer,
				      HRTIMER_ਬਹੁਲਕ_ABS_PINNED_HARD);
	}
	raw_spin_unlock(&rt_b->rt_runtime_ਤਾਲਾ);
}

ਸਥਿਰ ਖਾਲੀ ਸ਼ੁਰੂ_rt_bandwidth(ਬਣਤਰ rt_bandwidth *rt_b)
{
	ਜੇ (!rt_bandwidth_enabled() || rt_b->rt_runtime == RUNTIME_INF)
		ਵਾਪਸ;

	do_ਸ਼ੁਰੂ_rt_bandwidth(rt_b);
}

ਸਥਿਰ ਖਾਲੀ destroy_rt_bandwidth(ਬਣਤਰ rt_bandwidth *rt_b)
{
	hrtimer_cancel(&rt_b->rt_period_timer);
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ rt_entity_is_task(rt_se) (!(rt_se)->my_q)

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ task_struct *rt_task_ਦਾ(ਬਣਤਰ sched_rt_entity *rt_se)
{
	WARN_ON_ONCE(!rt_entity_is_task(rt_se));

	ਵਾਪਸ container_ਦਾ(rt_se, ਬਣਤਰ task_struct, rt);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ rq *rq_ਦਾ_rt_rq(ਬਣਤਰ rt_rq *rt_rq)
{
	/* Cannot fold with non-CONFIG_RT_GROUP_SCHED version, layout */
	WARN_ON(!rt_ਸਮੂਹ_sched_enabled() && rt_rq->tg != &ਜੜ੍ਹ_task_ਸਮੂਹ);
	ਵਾਪਸ rt_rq->rq;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ rt_rq *rt_rq_ਦਾ_se(ਬਣਤਰ sched_rt_entity *rt_se)
{
	WARN_ON(!rt_ਸਮੂਹ_sched_enabled() && rt_se->rt_rq->tg != &ਜੜ੍ਹ_task_ਸਮੂਹ);
	ਵਾਪਸ rt_se->rt_rq;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ rq *rq_ਦਾ_rt_se(ਬਣਤਰ sched_rt_entity *rt_se)
{
	ਬਣਤਰ rt_rq *rt_rq = rt_se->rt_rq;

	WARN_ON(!rt_ਸਮੂਹ_sched_enabled() && rt_rq->tg != &ਜੜ੍ਹ_task_ਸਮੂਹ);
	ਵਾਪਸ rt_rq->rq;
}

ਖਾਲੀ unregister_rt_sched_ਸਮੂਹ(ਬਣਤਰ task_ਸਮੂਹ *tg)
{
	ਜੇ (!rt_ਸਮੂਹ_sched_enabled())
		ਵਾਪਸ;

	ਜੇ (tg->rt_se)
		destroy_rt_bandwidth(&tg->rt_bandwidth);
}

ਖਾਲੀ free_rt_sched_ਸਮੂਹ(ਬਣਤਰ task_ਸਮੂਹ *tg)
{
	ਪੂਰਨ i;

	ਜੇ (!rt_ਸਮੂਹ_sched_enabled())
		ਵਾਪਸ;

	for_each_possible_cpu(i) {
		ਜੇ (tg->rt_rq)
			kfree(tg->rt_rq[i]);
		ਜੇ (tg->rt_se)
			kfree(tg->rt_se[i]);
	}

	kfree(tg->rt_rq);
	kfree(tg->rt_se);
}

ਖਾਲੀ init_tg_rt_ਇੰਦਰਾਜ(ਬਣਤਰ task_ਸਮੂਹ *tg, ਬਣਤਰ rt_rq *rt_rq,
		ਬਣਤਰ sched_rt_entity *rt_se, ਪੂਰਨ cpu,
		ਬਣਤਰ sched_rt_entity *ਮਾਪਾ)
{
	ਬਣਤਰ rq *rq = cpu_rq(cpu);

	rt_rq->highest_prio.curr = MAX_RT_PRIO-1;
	rt_rq->rt_nr_boosted = 0;
	rt_rq->rq = rq;
	rt_rq->tg = tg;

	tg->rt_rq[cpu] = rt_rq;
	tg->rt_se[cpu] = rt_se;

	ਜੇ (!rt_se)
		ਵਾਪਸ;

	ਜੇ (!ਮਾਪਾ)
		rt_se->rt_rq = &rq->rt;
	ਨਹੀਂਤਾਂ
		rt_se->rt_rq = ਮਾਪਾ->my_q;

	rt_se->my_q = rt_rq;
	rt_se->ਮਾਪਾ = ਮਾਪਾ;
	INIT_LIST_ਸਿਰ(&rt_se->ਲੜੀਵਾਰ_list);
}

ਪੂਰਨ alloc_rt_sched_ਸਮੂਹ(ਬਣਤਰ task_ਸਮੂਹ *tg, ਬਣਤਰ task_ਸਮੂਹ *ਮਾਪਾ)
{
	ਬਣਤਰ rt_rq *rt_rq;
	ਬਣਤਰ sched_rt_entity *rt_se;
	ਪੂਰਨ i;

	ਜੇ (!rt_ਸਮੂਹ_sched_enabled())
		ਵਾਪਸ 1;

	tg->rt_rq = kzalloc_objs(rt_rq, nr_cpu_ids);
	ਜੇ (!tg->rt_rq)
		ਜਾਓ err;
	tg->rt_se = kzalloc_objs(rt_se, nr_cpu_ids);
	ਜੇ (!tg->rt_se)
		ਜਾਓ err;

	init_rt_bandwidth(&tg->rt_bandwidth, ktime_ਮੰਜ਼ਿਲ_ns(global_rt_period()), 0);

	for_each_possible_cpu(i) {
		rt_rq = kzalloc_ਗੰਢ(ਆਕਾਰ(ਬਣਤਰ rt_rq),
				     GFP_KERNEL, cpu_ਮੰਜ਼ਿਲ_ਗੰਢ(i));
		ਜੇ (!rt_rq)
			ਜਾਓ err;

		rt_se = kzalloc_ਗੰਢ(ਆਕਾਰ(ਬਣਤਰ sched_rt_entity),
				     GFP_KERNEL, cpu_ਮੰਜ਼ਿਲ_ਗੰਢ(i));
		ਜੇ (!rt_se)
			ਜਾਓ err_free_rq;

		init_rt_rq(rt_rq);
		rt_rq->rt_runtime = tg->rt_bandwidth.rt_runtime;
		init_tg_rt_ਇੰਦਰਾਜ(tg, rt_rq, rt_se, i, ਮਾਪਾ->rt_se[i]);
	}

	ਵਾਪਸ 1;

err_free_rq:
	kfree(rt_rq);
err:
	ਵਾਪਸ 0;
}

#ਨਹੀਂਤਾਂ /* !CONFIG_RT_GROUP_SCHED: */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ rt_entity_is_task(rt_se) (1)

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ task_struct *rt_task_ਦਾ(ਬਣਤਰ sched_rt_entity *rt_se)
{
	ਵਾਪਸ container_ਦਾ(rt_se, ਬਣਤਰ task_struct, rt);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ rq *rq_ਦਾ_rt_rq(ਬਣਤਰ rt_rq *rt_rq)
{
	ਵਾਪਸ container_ਦਾ(rt_rq, ਬਣਤਰ rq, rt);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ rq *rq_ਦਾ_rt_se(ਬਣਤਰ sched_rt_entity *rt_se)
{
	ਬਣਤਰ task_struct *p = rt_task_ਦਾ(rt_se);

	ਵਾਪਸ task_rq(p);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ rt_rq *rt_rq_ਦਾ_se(ਬਣਤਰ sched_rt_entity *rt_se)
{
	ਬਣਤਰ rq *rq = rq_ਦਾ_rt_se(rt_se);

	ਵਾਪਸ &rq->rt;
}

ਖਾਲੀ unregister_rt_sched_ਸਮੂਹ(ਬਣਤਰ task_ਸਮੂਹ *tg) { }

ਖਾਲੀ free_rt_sched_ਸਮੂਹ(ਬਣਤਰ task_ਸਮੂਹ *tg) { }

ਪੂਰਨ alloc_rt_sched_ਸਮੂਹ(ਬਣਤਰ task_ਸਮੂਹ *tg, ਬਣਤਰ task_ਸਮੂਹ *ਮਾਪਾ)
{
	ਵਾਪਸ 1;
}
#ਜੇ_ਅੰਤ /* !CONFIG_RT_GROUP_SCHED */

ਸਥਿਰ ਸਤਰਅੰਦਰ bool need_pull_rt_task(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *prev)
{
	/* Try to pull RT tasks here if we lower this rq's prio */
	ਵਾਪਸ rq->online && rq->rt.highest_prio.curr > prev->prio;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ rt_overloaded(ਬਣਤਰ rq *rq)
{
	ਵਾਪਸ atomic_read(&rq->rd->rto_ਗਿਣਤੀ);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ rt_ਸੈੱਟ_overload(ਬਣਤਰ rq *rq)
{
	ਜੇ (!rq->online)
		ਵਾਪਸ;

	cpumask_ਸੈੱਟ_cpu(rq->cpu, rq->rd->rto_ਨਕਾਬ);
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
	atomic_inc(&rq->rd->rto_ਗਿਣਤੀ);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ rt_clear_overload(ਬਣਤਰ rq *rq)
{
	ਜੇ (!rq->online)
		ਵਾਪਸ;

	/* the order here really doesn't matter */
	atomic_dec(&rq->rd->rto_ਗਿਣਤੀ);
	cpumask_clear_cpu(rq->cpu, rq->rd->rto_ਨਕਾਬ);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ has_pushable_tasks(ਬਣਤਰ rq *rq)
{
	ਵਾਪਸ !plist_ਸਿਰ_ਖਾਲੀ_ਥਾਵਾਂ(&rq->rt.pushable_tasks);
}

ਸਥਿਰ DEFINE_ਪ੍ਰਤੀ_CPU(ਬਣਤਰ balance_callback, rt_ਧੱਕੋ_ਸਿਰ);
ਸਥਿਰ DEFINE_ਪ੍ਰਤੀ_CPU(ਬਣਤਰ balance_callback, rt_pull_ਸਿਰ);

ਸਥਿਰ ਖਾਲੀ ਧੱਕੋ_rt_tasks(ਬਣਤਰ rq *);
ਸਥਿਰ ਖਾਲੀ pull_rt_task(ਬਣਤਰ rq *);

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ rt_ਕਤਾਰ_ਧੱਕੋ_tasks(ਬਣਤਰ rq *rq)
{
	ਜੇ (!has_pushable_tasks(rq))
		ਵਾਪਸ;

	ਕਤਾਰ_balance_callback(rq, &ਪ੍ਰਤੀ_cpu(rt_ਧੱਕੋ_ਸਿਰ, rq->cpu), ਧੱਕੋ_rt_tasks);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ rt_ਕਤਾਰ_pull_task(ਬਣਤਰ rq *rq)
{
	ਕਤਾਰ_balance_callback(rq, &ਪ੍ਰਤੀ_cpu(rt_pull_ਸਿਰ, rq->cpu), pull_rt_task);
}

ਸਥਿਰ ਖਾਲੀ ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_pushable_task(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p)
{
	plist_del(&p->pushable_tasks, &rq->rt.pushable_tasks);
	plist_ਗੰਢ_init(&p->pushable_tasks, p->prio);
	plist_ਜੋੜੋ(&p->pushable_tasks, &rq->rt.pushable_tasks);

	/* Update the highest prio pushable task */
	ਜੇ (p->prio < rq->rt.highest_prio.ਅਗਲਾ)
		rq->rt.highest_prio.ਅਗਲਾ = p->prio;

	ਜੇ (!rq->rt.overloaded) {
		rt_ਸੈੱਟ_overload(rq);
		rq->rt.overloaded = 1;
	}
}

ਸਥਿਰ ਖਾਲੀ ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_pushable_task(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p)
{
	plist_del(&p->pushable_tasks, &rq->rt.pushable_tasks);

	/* Update the new highest prio pushable task */
	ਜੇ (has_pushable_tasks(rq)) {
		p = plist_ਪਹਿਲਾ_ਇੰਦਰਾਜ(&rq->rt.pushable_tasks,
				      ਬਣਤਰ task_struct, pushable_tasks);
		rq->rt.highest_prio.ਅਗਲਾ = p->prio;
	} ਨਹੀਂਤਾਂ {
		rq->rt.highest_prio.ਅਗਲਾ = MAX_RT_PRIO-1;

		ਜੇ (rq->rt.overloaded) {
			rt_clear_overload(rq);
			rq->rt.overloaded = 0;
		}
	}
}

ਸਥਿਰ ਖਾਲੀ ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_ਸਿਖਰ_rt_rq(ਬਣਤਰ rt_rq *rt_rq);
ਸਥਿਰ ਖਾਲੀ ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_ਸਿਖਰ_rt_rq(ਬਣਤਰ rt_rq *rt_rq, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2);

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ on_rt_rq(ਬਣਤਰ sched_rt_entity *rt_se)
{
	ਵਾਪਸ rt_se->on_rq;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_UCLAMP_TASK
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
ਸਥਿਰ ਸਤਰਅੰਦਰ bool rt_task_ਸਮਾਉਂਦਾ_ਸਮਰੱਥਾ(ਬਣਤਰ task_struct *p, ਪੂਰਨ cpu)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ min_cap;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ max_cap;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cpu_cap;

	/* Only heterogeneous systems can benefit from this check */
	ਜੇ (!sched_asym_cpucap_ਸਰਗਰਮ())
		ਵਾਪਸ true;

	min_cap = uclamp_eff_ਮੁੱਲ(p, UCLAMP_MIN);
	max_cap = uclamp_eff_ਮੁੱਲ(p, UCLAMP_MAX);

	cpu_cap = arch_scale_cpu_ਸਮਰੱਥਾ(cpu);

	ਵਾਪਸ cpu_cap >= min(min_cap, max_cap);
}
#ਨਹੀਂਤਾਂ /* !CONFIG_UCLAMP_TASK: */
ਸਥਿਰ ਸਤਰਅੰਦਰ bool rt_task_ਸਮਾਉਂਦਾ_ਸਮਰੱਥਾ(ਬਣਤਰ task_struct *p, ਪੂਰਨ cpu)
{
	ਵਾਪਸ true;
}
#ਜੇ_ਅੰਤ /* !CONFIG_UCLAMP_TASK */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_RT_ਸਮੂਹ_SCHED

ਸਥਿਰ ਸਤਰਅੰਦਰ u64 sched_rt_runtime(ਬਣਤਰ rt_rq *rt_rq)
{
	ਵਾਪਸ rt_rq->rt_runtime;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ u64 sched_rt_period(ਬਣਤਰ rt_rq *rt_rq)
{
	ਵਾਪਸ ktime_ਮੰਜ਼ਿਲ_ns(rt_rq->tg->rt_bandwidth.rt_period);
}

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ task_ਸਮੂਹ *rt_rq_iter_t;

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ task_ਸਮੂਹ *ਅਗਲਾ_task_ਸਮੂਹ(ਬਣਤਰ task_ਸਮੂਹ *tg)
{
	ਜੇ (!rt_ਸਮੂਹ_sched_enabled()) {
		WARN_ON(tg != &ਜੜ੍ਹ_task_ਸਮੂਹ);
		ਵਾਪਸ ਨਿਰਾ;
	}

	ਕਰੋ {
		tg = list_ਇੰਦਰਾਜ_rcu(tg->list.ਅਗਲਾ,
			typeof(ਬਣਤਰ task_ਸਮੂਹ), list);
	} ਜਦੋਂ (&tg->list != &task_groups && task_ਸਮੂਹ_is_autogroup(tg));

	ਜੇ (&tg->list == &task_groups)
		tg = ਨਿਰਾ;

	ਵਾਪਸ tg;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ for_each_rt_rq(rt_rq, iter, rq)					\
	ਲਈ (iter = &ਜੜ੍ਹ_task_ਸਮੂਹ;					\
		iter && (rt_rq = iter->rt_rq[cpu_ਦਾ(rq)]);		\
		iter = ਅਗਲਾ_task_ਸਮੂਹ(iter))

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ for_each_sched_rt_entity(rt_se) \
	ਲਈ (; rt_se; rt_se = rt_se->ਮਾਪਾ)

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ rt_rq *ਸਮੂਹ_rt_rq(ਬਣਤਰ sched_rt_entity *rt_se)
{
	ਵਾਪਸ rt_se->my_q;
}

ਸਥਿਰ ਖਾਲੀ ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_rt_entity(ਬਣਤਰ sched_rt_entity *rt_se, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags);
ਸਥਿਰ ਖਾਲੀ ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_rt_entity(ਬਣਤਰ sched_rt_entity *rt_se, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags);

ਸਥਿਰ ਖਾਲੀ sched_rt_rq_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ(ਬਣਤਰ rt_rq *rt_rq)
{
	ਬਣਤਰ task_struct *donor = rq_ਦਾ_rt_rq(rt_rq)->donor;
	ਬਣਤਰ rq *rq = rq_ਦਾ_rt_rq(rt_rq);
	ਬਣਤਰ sched_rt_entity *rt_se;

	ਪੂਰਨ cpu = cpu_ਦਾ(rq);

	rt_se = rt_rq->tg->rt_se[cpu];

	ਜੇ (rt_rq->rt_nr_running) {
		ਜੇ (!rt_se)
			ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_ਸਿਖਰ_rt_rq(rt_rq);
		ਨਹੀਂਤਾਂ ਜੇ (!on_rt_rq(rt_se))
			ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_rt_entity(rt_se, 0);

		ਜੇ (rt_rq->highest_prio.curr < donor->prio)
			resched_curr(rq);
	}
}

ਸਥਿਰ ਖਾਲੀ sched_rt_rq_ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ(ਬਣਤਰ rt_rq *rt_rq)
{
	ਬਣਤਰ sched_rt_entity *rt_se;
	ਪੂਰਨ cpu = cpu_ਦਾ(rq_ਦਾ_rt_rq(rt_rq));

	rt_se = rt_rq->tg->rt_se[cpu];

	ਜੇ (!rt_se) {
		ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_ਸਿਖਰ_rt_rq(rt_rq, rt_rq->rt_nr_running);
		/* Kick cpufreq (see the comment in kernel/sched/sched.h). */
		cpufreq_update_util(rq_ਦਾ_rt_rq(rt_rq), 0);
	}
	ਨਹੀਂਤਾਂ ਜੇ (on_rt_rq(rt_se))
		ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_rt_entity(rt_se, 0);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ rt_rq_throttled(ਬਣਤਰ rt_rq *rt_rq)
{
	ਵਾਪਸ rt_rq->rt_throttled && !rt_rq->rt_nr_boosted;
}

ਸਥਿਰ ਪੂਰਨ rt_se_boosted(ਬਣਤਰ sched_rt_entity *rt_se)
{
	ਬਣਤਰ rt_rq *rt_rq = ਸਮੂਹ_rt_rq(rt_se);
	ਬਣਤਰ task_struct *p;

	ਜੇ (rt_rq)
		ਵਾਪਸ !!rt_rq->rt_nr_boosted;

	p = rt_task_ਦਾ(rt_se);
	ਵਾਪਸ p->prio != p->ਸਾਧਾਰਨ_prio;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਅਚਲ ਬਣਤਰ cpumask *sched_rt_period_ਨਕਾਬ(ਖਾਲੀ)
{
	ਵਾਪਸ this_rq()->rd->ਵਿਸਤਾਰ;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ
ਬਣਤਰ rt_rq *sched_rt_period_rt_rq(ਬਣਤਰ rt_bandwidth *rt_b, ਪੂਰਨ cpu)
{
	ਵਾਪਸ container_ਦਾ(rt_b, ਬਣਤਰ task_ਸਮੂਹ, rt_bandwidth)->rt_rq[cpu];
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ rt_bandwidth *sched_rt_bandwidth(ਬਣਤਰ rt_rq *rt_rq)
{
	ਵਾਪਸ &rt_rq->tg->rt_bandwidth;
}

bool sched_rt_bandwidth_account(ਬਣਤਰ rt_rq *rt_rq)
{
	ਬਣਤਰ rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);

	ਵਾਪਸ (hrtimer_ਸਰਗਰਮ(&rt_b->rt_period_timer) ||
		rt_rq->rt_ਸਮਾਂ < rt_b->rt_runtime);
}

/*
 * We ran out of runtime, see if we can borrow some from our neighbours.
 */
ਸਥਿਰ ਖਾਲੀ do_balance_runtime(ਬਣਤਰ rt_rq *rt_rq)
{
	ਬਣਤਰ rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);
	ਬਣਤਰ ਜੜ੍ਹ_domain *rd = rq_ਦਾ_rt_rq(rt_rq)->rd;
	ਪੂਰਨ i, ਭਾਰ;
	u64 rt_period;

	ਭਾਰ = cpumask_ਭਾਰ(rd->ਵਿਸਤਾਰ);

	raw_spin_ਤਾਲਾ(&rt_b->rt_runtime_ਤਾਲਾ);
	rt_period = ktime_ਮੰਜ਼ਿਲ_ns(rt_b->rt_period);
	for_each_cpu(i, rd->ਵਿਸਤਾਰ) {
		ਬਣਤਰ rt_rq *iter = sched_rt_period_rt_rq(rt_b, i);
		s64 diff;

		ਜੇ (iter == rt_rq)
			ਜਾਰੀ;

		raw_spin_ਤਾਲਾ(&iter->rt_runtime_ਤਾਲਾ);
		/*
		 * Either all rqs have inf runtime and there's nothing to steal
		 * or __disable_runtime() below sets a specific rq to inf to
		 * indicate its been disabled and disallow stealing.
		 */
		ਜੇ (iter->rt_runtime == RUNTIME_INF)
			ਜਾਓ ਅਗਲਾ;

		/*
		 * From runqueues with spare time, take 1/n part of their
		 * spare time, but no more than our period.
		 */
		diff = iter->rt_runtime - iter->rt_ਸਮਾਂ;
		ਜੇ (diff > 0) {
			diff = div_u64((u64)diff, ਭਾਰ);
			ਜੇ (rt_rq->rt_runtime + diff > rt_period)
				diff = rt_period - rt_rq->rt_runtime;
			iter->rt_runtime -= diff;
			rt_rq->rt_runtime += diff;
			ਜੇ (rt_rq->rt_runtime == rt_period) {
				raw_spin_unlock(&iter->rt_runtime_ਤਾਲਾ);
				ਰੋਕੋ;
			}
		}
ਅਗਲਾ:
		raw_spin_unlock(&iter->rt_runtime_ਤਾਲਾ);
	}
	raw_spin_unlock(&rt_b->rt_runtime_ਤਾਲਾ);
}

/*
 * Ensure this RQ takes back all the runtime it lend to its neighbours.
 */
ਸਥਿਰ ਖਾਲੀ __disable_runtime(ਬਣਤਰ rq *rq)
{
	ਬਣਤਰ ਜੜ੍ਹ_domain *rd = rq->rd;
	rt_rq_iter_t iter;
	ਬਣਤਰ rt_rq *rt_rq;

	ਜੇ (unlikely(!scheduler_running))
		ਵਾਪਸ;

	for_each_rt_rq(rt_rq, iter, rq) {
		ਬਣਤਰ rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);
		s64 want;
		ਪੂਰਨ i;

		raw_spin_ਤਾਲਾ(&rt_b->rt_runtime_ਤਾਲਾ);
		raw_spin_ਤਾਲਾ(&rt_rq->rt_runtime_ਤਾਲਾ);
		/*
		 * Either we're all inf and nobody needs to borrow, or we're
		 * already disabled and thus have nothing to do, or we have
		 * exactly the right amount of runtime to take out.
		 */
		ਜੇ (rt_rq->rt_runtime == RUNTIME_INF ||
				rt_rq->rt_runtime == rt_b->rt_runtime)
			ਜਾਓ ਸੰਤੁਲਿਤ;
		raw_spin_unlock(&rt_rq->rt_runtime_ਤਾਲਾ);

		/*
		 * Calculate the difference between what we started out with
		 * and what we current have, that's the amount of runtime
		 * we lend and now have to reclaim.
		 */
		want = rt_b->rt_runtime - rt_rq->rt_runtime;

		/*
		 * Greedy reclaim, take back as much as we can.
		 */
		for_each_cpu(i, rd->ਵਿਸਤਾਰ) {
			ਬਣਤਰ rt_rq *iter = sched_rt_period_rt_rq(rt_b, i);
			s64 diff;

			/*
			 * Can't reclaim from ourselves or disabled runqueues.
			 */
			ਜੇ (iter == rt_rq || iter->rt_runtime == RUNTIME_INF)
				ਜਾਰੀ;

			raw_spin_ਤਾਲਾ(&iter->rt_runtime_ਤਾਲਾ);
			ਜੇ (want > 0) {
				diff = min_t(s64, iter->rt_runtime, want);
				iter->rt_runtime -= diff;
				want -= diff;
			} ਨਹੀਂਤਾਂ {
				iter->rt_runtime -= want;
				want -= want;
			}
			raw_spin_unlock(&iter->rt_runtime_ਤਾਲਾ);

			ਜੇ (!want)
				ਰੋਕੋ;
		}

		raw_spin_ਤਾਲਾ(&rt_rq->rt_runtime_ਤਾਲਾ);
		/*
		 * We cannot be left wanting - that would mean some runtime
		 * leaked out of the system.
		 */
		WARN_ON_ONCE(want);
ਸੰਤੁਲਿਤ:
		/*
		 * Disable all the borrow logic by pretending we have inf
		 * runtime - in which case borrowing doesn't make sense.
		 */
		rt_rq->rt_runtime = RUNTIME_INF;
		rt_rq->rt_throttled = 0;
		raw_spin_unlock(&rt_rq->rt_runtime_ਤਾਲਾ);
		raw_spin_unlock(&rt_b->rt_runtime_ਤਾਲਾ);

		/* Make rt_rq available for pick_next_task() */
		sched_rt_rq_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ(rt_rq);
	}
}

ਸਥਿਰ ਖਾਲੀ __enable_runtime(ਬਣਤਰ rq *rq)
{
	rt_rq_iter_t iter;
	ਬਣਤਰ rt_rq *rt_rq;

	ਜੇ (unlikely(!scheduler_running))
		ਵਾਪਸ;

	/*
	 * Reset each runqueue's bandwidth settings
	 */
	for_each_rt_rq(rt_rq, iter, rq) {
		ਬਣਤਰ rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);

		raw_spin_ਤਾਲਾ(&rt_b->rt_runtime_ਤਾਲਾ);
		raw_spin_ਤਾਲਾ(&rt_rq->rt_runtime_ਤਾਲਾ);
		rt_rq->rt_runtime = rt_b->rt_runtime;
		rt_rq->rt_ਸਮਾਂ = 0;
		rt_rq->rt_throttled = 0;
		raw_spin_unlock(&rt_rq->rt_runtime_ਤਾਲਾ);
		raw_spin_unlock(&rt_b->rt_runtime_ਤਾਲਾ);
	}
}

ਸਥਿਰ ਖਾਲੀ balance_runtime(ਬਣਤਰ rt_rq *rt_rq)
{
	ਜੇ (!sched_feat(RT_RUNTIME_SHARE))
		ਵਾਪਸ;

	ਜੇ (rt_rq->rt_ਸਮਾਂ > rt_rq->rt_runtime) {
		raw_spin_unlock(&rt_rq->rt_runtime_ਤਾਲਾ);
		do_balance_runtime(rt_rq);
		raw_spin_ਤਾਲਾ(&rt_rq->rt_runtime_ਤਾਲਾ);
	}
}

ਸਥਿਰ ਪੂਰਨ do_sched_rt_period_timer(ਬਣਤਰ rt_bandwidth *rt_b, ਪੂਰਨ overrun)
{
	ਪੂਰਨ i, idle = 1, throttled = 0;
	ਅਚਲ ਬਣਤਰ cpumask *ਵਿਸਤਾਰ;

	ਵਿਸਤਾਰ = sched_rt_period_ਨਕਾਬ();

	/*
	 * FIXME: isolated CPUs should really leave the root task group,
	 * whether they are isolcpus or were isolated via cpusets, lest
	 * the timer run on a CPU which does not service all runqueues,
	 * potentially leaving other CPUs indefinitely throttled.  If
	 * isolation is really required, the user will turn the throttle
	 * off to kill the perturbations it causes anyway.  Meanwhile,
	 * this maintains functionality for boot and/or troubleshooting.
	 */
	ਜੇ (rt_b == &ਜੜ੍ਹ_task_ਸਮੂਹ.rt_bandwidth)
		ਵਿਸਤਾਰ = cpu_online_ਨਕਾਬ;

	for_each_cpu(i, ਵਿਸਤਾਰ) {
		ਪੂਰਨ ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ = 0;
		ਬਣਤਰ rt_rq *rt_rq = sched_rt_period_rt_rq(rt_b, i);
		ਬਣਤਰ rq *rq = rq_ਦਾ_rt_rq(rt_rq);
		ਬਣਤਰ rq_flags rf;
		ਪੂਰਨ skip;

		/*
		 * When span == cpu_online_mask, taking each rq->lock
		 * can be time-consuming. Try to avoid it when possible.
		 */
		raw_spin_ਤਾਲਾ(&rt_rq->rt_runtime_ਤਾਲਾ);
		ਜੇ (!sched_feat(RT_RUNTIME_SHARE) && rt_rq->rt_runtime != RUNTIME_INF)
			rt_rq->rt_runtime = rt_b->rt_runtime;
		skip = !rt_rq->rt_ਸਮਾਂ && !rt_rq->rt_nr_running;
		raw_spin_unlock(&rt_rq->rt_runtime_ਤਾਲਾ);
		ਜੇ (skip)
			ਜਾਰੀ;

		rq_ਤਾਲਾ(rq, &rf);
		update_rq_clock(rq);

		ਜੇ (rt_rq->rt_ਸਮਾਂ) {
			u64 runtime;

			raw_spin_ਤਾਲਾ(&rt_rq->rt_runtime_ਤਾਲਾ);
			ਜੇ (rt_rq->rt_throttled)
				balance_runtime(rt_rq);
			runtime = rt_rq->rt_runtime;
			rt_rq->rt_ਸਮਾਂ -= min(rt_rq->rt_ਸਮਾਂ, overrun*runtime);
			ਜੇ (rt_rq->rt_throttled && rt_rq->rt_ਸਮਾਂ < runtime) {
				rt_rq->rt_throttled = 0;
				ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ = 1;

				/*
				 * When we're idle and a woken (rt) task is
				 * throttled wakeup_preempt() will set
				 * skip_update and the time between the wakeup
				 * and this unthrottle will get accounted as
				 * 'runtime'.
				 */
				ਜੇ (rt_rq->rt_nr_running && rq->curr == rq->idle)
					rq_clock_cancel_skipupdate(rq);
			}
			ਜੇ (rt_rq->rt_ਸਮਾਂ || rt_rq->rt_nr_running)
				idle = 0;
			raw_spin_unlock(&rt_rq->rt_runtime_ਤਾਲਾ);
		} ਨਹੀਂਤਾਂ ਜੇ (rt_rq->rt_nr_running) {
			idle = 0;
			ਜੇ (!rt_rq_throttled(rt_rq))
				ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ = 1;
		}
		ਜੇ (rt_rq->rt_throttled)
			throttled = 1;

		ਜੇ (ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ)
			sched_rt_rq_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ(rt_rq);
		rq_unlock(rq, &rf);
	}

	ਜੇ (!throttled && (!rt_bandwidth_enabled() || rt_b->rt_runtime == RUNTIME_INF))
		ਵਾਪਸ 1;

	ਵਾਪਸ idle;
}

ਸਥਿਰ ਪੂਰਨ sched_rt_runtime_exceeded(ਬਣਤਰ rt_rq *rt_rq)
{
	u64 runtime = sched_rt_runtime(rt_rq);

	ਜੇ (rt_rq->rt_throttled)
		ਵਾਪਸ rt_rq_throttled(rt_rq);

	ਜੇ (runtime >= sched_rt_period(rt_rq))
		ਵਾਪਸ 0;

	balance_runtime(rt_rq);
	runtime = sched_rt_runtime(rt_rq);
	ਜੇ (runtime == RUNTIME_INF)
		ਵਾਪਸ 0;

	ਜੇ (rt_rq->rt_ਸਮਾਂ > runtime) {
		ਬਣਤਰ rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);

		/*
		 * Don't actually throttle groups that have no runtime assigned
		 * but accrue some time due to boosting.
		 */
		ਜੇ (likely(rt_b->rt_runtime)) {
			rt_rq->rt_throttled = 1;
			printk_deferred_once("sched: RT throttling activated\n");
		} ਨਹੀਂਤਾਂ {
			/*
			 * In case we did anyway, make it go away,
			 * replenishment is a joke, since it will replenish us
			 * with exactly 0 ns.
			 */
			rt_rq->rt_ਸਮਾਂ = 0;
		}

		ਜੇ (rt_rq_throttled(rt_rq)) {
			sched_rt_rq_ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ(rt_rq);
			ਵਾਪਸ 1;
		}
	}

	ਵਾਪਸ 0;
}

#ਨਹੀਂਤਾਂ /* !CONFIG_RT_GROUP_SCHED: */

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ rt_rq *rt_rq_iter_t;

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ for_each_rt_rq(rt_rq, iter, rq) \
	ਲਈ ((ਖਾਲੀ) iter, rt_rq = &rq->rt; rt_rq; rt_rq = ਨਿਰਾ)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ for_each_sched_rt_entity(rt_se) \
	ਲਈ (; rt_se; rt_se = ਨਿਰਾ)

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ rt_rq *ਸਮੂਹ_rt_rq(ਬਣਤਰ sched_rt_entity *rt_se)
{
	ਵਾਪਸ ਨਿਰਾ;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ sched_rt_rq_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ(ਬਣਤਰ rt_rq *rt_rq)
{
	ਬਣਤਰ rq *rq = rq_ਦਾ_rt_rq(rt_rq);

	ਜੇ (!rt_rq->rt_nr_running)
		ਵਾਪਸ;

	ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_ਸਿਖਰ_rt_rq(rt_rq);
	resched_curr(rq);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ sched_rt_rq_ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ(ਬਣਤਰ rt_rq *rt_rq)
{
	ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_ਸਿਖਰ_rt_rq(rt_rq, rt_rq->rt_nr_running);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ rt_rq_throttled(ਬਣਤਰ rt_rq *rt_rq)
{
	ਵਾਪਸ false;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਅਚਲ ਬਣਤਰ cpumask *sched_rt_period_ਨਕਾਬ(ਖਾਲੀ)
{
	ਵਾਪਸ cpu_online_ਨਕਾਬ;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ
ਬਣਤਰ rt_rq *sched_rt_period_rt_rq(ਬਣਤਰ rt_bandwidth *rt_b, ਪੂਰਨ cpu)
{
	ਵਾਪਸ &cpu_rq(cpu)->rt;
}

ਸਥਿਰ ਖਾਲੀ __enable_runtime(ਬਣਤਰ rq *rq) { }
ਸਥਿਰ ਖਾਲੀ __disable_runtime(ਬਣਤਰ rq *rq) { }

#ਜੇ_ਅੰਤ /* !CONFIG_RT_GROUP_SCHED */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ rt_se_prio(ਬਣਤਰ sched_rt_entity *rt_se)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_RT_ਸਮੂਹ_SCHED
	ਬਣਤਰ rt_rq *rt_rq = ਸਮੂਹ_rt_rq(rt_se);

	ਜੇ (rt_rq)
		ਵਾਪਸ rt_rq->highest_prio.curr;
#ਜੇ_ਅੰਤ

	ਵਾਪਸ rt_task_ਦਾ(rt_se)->prio;
}

/*
 * Update the current task's runtime statistics. Skip current tasks that
 * are not in our scheduling class.
 */
ਸਥਿਰ ਖਾਲੀ update_curr_rt(ਬਣਤਰ rq *rq)
{
	ਬਣਤਰ task_struct *donor = rq->donor;
	s64 delta_exec;

	ਜੇ (donor->sched_class != &rt_sched_class)
		ਵਾਪਸ;

	delta_exec = update_curr_common(rq);
	ਜੇ (unlikely(delta_exec <= 0))
		ਵਾਪਸ;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_RT_ਸਮੂਹ_SCHED
	ਬਣਤਰ sched_rt_entity *rt_se = &donor->rt;

	ਜੇ (!rt_bandwidth_enabled())
		ਵਾਪਸ;

	for_each_sched_rt_entity(rt_se) {
		ਬਣਤਰ rt_rq *rt_rq = rt_rq_ਦਾ_se(rt_se);
		ਪੂਰਨ exceeded;

		ਜੇ (sched_rt_runtime(rt_rq) != RUNTIME_INF) {
			raw_spin_ਤਾਲਾ(&rt_rq->rt_runtime_ਤਾਲਾ);
			rt_rq->rt_ਸਮਾਂ += delta_exec;
			exceeded = sched_rt_runtime_exceeded(rt_rq);
			ਜੇ (exceeded)
				resched_curr(rq);
			raw_spin_unlock(&rt_rq->rt_runtime_ਤਾਲਾ);
			ਜੇ (exceeded)
				do_ਸ਼ੁਰੂ_rt_bandwidth(sched_rt_bandwidth(rt_rq));
		}
	}
#ਜੇ_ਅੰਤ /* CONFIG_RT_GROUP_SCHED */
}

ਸਥਿਰ ਖਾਲੀ
ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_ਸਿਖਰ_rt_rq(ਬਣਤਰ rt_rq *rt_rq, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2)
{
	ਬਣਤਰ rq *rq = rq_ਦਾ_rt_rq(rt_rq);

	BUG_ON(&rq->rt != rt_rq);

	ਜੇ (!rt_rq->rt_queued)
		ਵਾਪਸ;

	BUG_ON(!rq->nr_running);

	sub_nr_running(rq, ਗਿਣਤੀ_2);
	rt_rq->rt_queued = 0;

}

ਸਥਿਰ ਖਾਲੀ
ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_ਸਿਖਰ_rt_rq(ਬਣਤਰ rt_rq *rt_rq)
{
	ਬਣਤਰ rq *rq = rq_ਦਾ_rt_rq(rt_rq);

	BUG_ON(&rq->rt != rt_rq);

	ਜੇ (rt_rq->rt_queued)
		ਵਾਪਸ;

	ਜੇ (rt_rq_throttled(rt_rq))
		ਵਾਪਸ;

	ਜੇ (rt_rq->rt_nr_running) {
		ਜੋੜੋ_nr_running(rq, rt_rq->rt_nr_running);
		rt_rq->rt_queued = 1;
	}

	/* Kick cpufreq (see the comment in kernel/sched/sched.h). */
	cpufreq_update_util(rq, 0);
}

ਸਥਿਰ ਖਾਲੀ
inc_rt_prio_smp(ਬਣਤਰ rt_rq *rt_rq, ਪੂਰਨ prio, ਪੂਰਨ prev_prio)
{
	ਬਣਤਰ rq *rq = rq_ਦਾ_rt_rq(rt_rq);

	/*
	 * Change rq's cpupri only if rt_rq is the top queue.
	 */
	ਜੇ (IS_ENABLED(CONFIG_RT_ਸਮੂਹ_SCHED) && &rq->rt != rt_rq)
		ਵਾਪਸ;

	ਜੇ (rq->online && prio < prev_prio)
		cpupri_ਸੈੱਟ(&rq->rd->cpupri, rq->cpu, prio);
}

ਸਥਿਰ ਖਾਲੀ
dec_rt_prio_smp(ਬਣਤਰ rt_rq *rt_rq, ਪੂਰਨ prio, ਪੂਰਨ prev_prio)
{
	ਬਣਤਰ rq *rq = rq_ਦਾ_rt_rq(rt_rq);

	/*
	 * Change rq's cpupri only if rt_rq is the top queue.
	 */
	ਜੇ (IS_ENABLED(CONFIG_RT_ਸਮੂਹ_SCHED) && &rq->rt != rt_rq)
		ਵਾਪਸ;

	ਜੇ (rq->online && rt_rq->highest_prio.curr != prev_prio)
		cpupri_ਸੈੱਟ(&rq->rd->cpupri, rq->cpu, rt_rq->highest_prio.curr);
}

ਸਥਿਰ ਖਾਲੀ
inc_rt_prio(ਬਣਤਰ rt_rq *rt_rq, ਪੂਰਨ prio)
{
	ਪੂਰਨ prev_prio = rt_rq->highest_prio.curr;

	ਜੇ (prio < prev_prio)
		rt_rq->highest_prio.curr = prio;

	inc_rt_prio_smp(rt_rq, prio, prev_prio);
}

ਸਥਿਰ ਖਾਲੀ
dec_rt_prio(ਬਣਤਰ rt_rq *rt_rq, ਪੂਰਨ prio)
{
	ਪੂਰਨ prev_prio = rt_rq->highest_prio.curr;

	ਜੇ (rt_rq->rt_nr_running) {

		WARN_ON(prio < prev_prio);

		/*
		 * This may have been our highest task, and therefore
		 * we may have some re-computation to do
		 */
		ਜੇ (prio == prev_prio) {
			ਬਣਤਰ rt_prio_array *array = &rt_rq->ਸਰਗਰਮ;

			rt_rq->highest_prio.curr =
				sched_ਲੱਭੋ_ਪਹਿਲਾ_ਦੋਆਧਾਰੀ_ਅੰਕ(array->bitmap);
		}

	} ਨਹੀਂਤਾਂ {
		rt_rq->highest_prio.curr = MAX_RT_PRIO-1;
	}

	dec_rt_prio_smp(rt_rq, prio, prev_prio);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_RT_ਸਮੂਹ_SCHED

ਸਥਿਰ ਖਾਲੀ
inc_rt_ਸਮੂਹ(ਬਣਤਰ sched_rt_entity *rt_se, ਬਣਤਰ rt_rq *rt_rq)
{
	ਜੇ (rt_se_boosted(rt_se))
		rt_rq->rt_nr_boosted++;

	ਸ਼ੁਰੂ_rt_bandwidth(&rt_rq->tg->rt_bandwidth);
}

ਸਥਿਰ ਖਾਲੀ
dec_rt_ਸਮੂਹ(ਬਣਤਰ sched_rt_entity *rt_se, ਬਣਤਰ rt_rq *rt_rq)
{
	ਜੇ (rt_se_boosted(rt_se))
		rt_rq->rt_nr_boosted--;

	WARN_ON(!rt_rq->rt_nr_running && rt_rq->rt_nr_boosted);
}

#ਨਹੀਂਤਾਂ /* !CONFIG_RT_GROUP_SCHED: */

ਸਥਿਰ ਖਾਲੀ
inc_rt_ਸਮੂਹ(ਬਣਤਰ sched_rt_entity *rt_se, ਬਣਤਰ rt_rq *rt_rq)
{
}

ਸਥਿਰ ਸਤਰਅੰਦਰ
ਖਾਲੀ dec_rt_ਸਮੂਹ(ਬਣਤਰ sched_rt_entity *rt_se, ਬਣਤਰ rt_rq *rt_rq) {}

#ਜੇ_ਅੰਤ /* !CONFIG_RT_GROUP_SCHED */

ਸਥਿਰ ਸਤਰਅੰਦਰ
ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ rt_se_nr_running(ਬਣਤਰ sched_rt_entity *rt_se)
{
	ਬਣਤਰ rt_rq *ਸਮੂਹ_rq = ਸਮੂਹ_rt_rq(rt_se);

	ਜੇ (ਸਮੂਹ_rq)
		ਵਾਪਸ ਸਮੂਹ_rq->rt_nr_running;
	ਨਹੀਂਤਾਂ
		ਵਾਪਸ 1;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ
ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ rt_se_rr_nr_running(ਬਣਤਰ sched_rt_entity *rt_se)
{
	ਬਣਤਰ rt_rq *ਸਮੂਹ_rq = ਸਮੂਹ_rt_rq(rt_se);
	ਬਣਤਰ task_struct *tsk;

	ਜੇ (ਸਮੂਹ_rq)
		ਵਾਪਸ ਸਮੂਹ_rq->rr_nr_running;

	tsk = rt_task_ਦਾ(rt_se);

	ਵਾਪਸ (tsk->policy == SCHED_RR) ? 1 : 0;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ
ਖਾਲੀ inc_rt_tasks(ਬਣਤਰ sched_rt_entity *rt_se, ਬਣਤਰ rt_rq *rt_rq)
{
	ਪੂਰਨ prio = rt_se_prio(rt_se);

	WARN_ON(!rt_prio(prio));
	rt_rq->rt_nr_running += rt_se_nr_running(rt_se);
	rt_rq->rr_nr_running += rt_se_rr_nr_running(rt_se);

	inc_rt_prio(rt_rq, prio);
	inc_rt_ਸਮੂਹ(rt_se, rt_rq);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ
ਖਾਲੀ dec_rt_tasks(ਬਣਤਰ sched_rt_entity *rt_se, ਬਣਤਰ rt_rq *rt_rq)
{
	WARN_ON(!rt_prio(rt_se_prio(rt_se)));
	WARN_ON(!rt_rq->rt_nr_running);
	rt_rq->rt_nr_running -= rt_se_nr_running(rt_se);
	rt_rq->rr_nr_running -= rt_se_rr_nr_running(rt_se);

	dec_rt_prio(rt_rq, rt_se_prio(rt_se));
	dec_rt_ਸਮੂਹ(rt_se, rt_rq);
}

/*
 * Change rt_se->run_list location unless SAVE && !MOVE
 *
 * assumes ENQUEUE/DEQUEUE flags match
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ bool move_entity(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags)
{
	ਜੇ ((flags & (ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_SAVE | ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_MOVE)) == ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_SAVE)
		ਵਾਪਸ false;

	ਵਾਪਸ true;
}

ਸਥਿਰ ਖਾਲੀ __delist_rt_entity(ਬਣਤਰ sched_rt_entity *rt_se, ਬਣਤਰ rt_prio_array *array)
{
	list_del_init(&rt_se->ਲੜੀਵਾਰ_list);

	ਜੇ (list_ਖਾਲੀ_ਥਾਵਾਂ(array->ਕਤਾਰ + rt_se_prio(rt_se)))
		__clear_bit(rt_se_prio(rt_se), array->bitmap);

	rt_se->on_list = 0;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ sched_statistics *
__schedstats_from_rt_se(ਬਣਤਰ sched_rt_entity *rt_se)
{
	/* schedstats is not supported for rt group. */
	ਜੇ (!rt_entity_is_task(rt_se))
		ਵਾਪਸ ਨਿਰਾ;

	ਵਾਪਸ &rt_task_ਦਾ(rt_se)->stats;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
update_stats_wait_ਸ਼ੁਰੂ_rt(ਬਣਤਰ rt_rq *rt_rq, ਬਣਤਰ sched_rt_entity *rt_se)
{
	ਬਣਤਰ sched_statistics *stats;
	ਬਣਤਰ task_struct *p = ਨਿਰਾ;

	ਜੇ (!schedstat_enabled())
		ਵਾਪਸ;

	ਜੇ (rt_entity_is_task(rt_se))
		p = rt_task_ਦਾ(rt_se);

	stats = __schedstats_from_rt_se(rt_se);
	ਜੇ (!stats)
		ਵਾਪਸ;

	__update_stats_wait_start(rq_ਦਾ_rt_rq(rt_rq), p, stats);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
update_stats_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_sleeper_rt(ਬਣਤਰ rt_rq *rt_rq, ਬਣਤਰ sched_rt_entity *rt_se)
{
	ਬਣਤਰ sched_statistics *stats;
	ਬਣਤਰ task_struct *p = ਨਿਰਾ;

	ਜੇ (!schedstat_enabled())
		ਵਾਪਸ;

	ਜੇ (rt_entity_is_task(rt_se))
		p = rt_task_ਦਾ(rt_se);

	stats = __schedstats_from_rt_se(rt_se);
	ਜੇ (!stats)
		ਵਾਪਸ;

	__update_stats_enqueue_sleeper(rq_ਦਾ_rt_rq(rt_rq), p, stats);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
update_stats_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_rt(ਬਣਤਰ rt_rq *rt_rq, ਬਣਤਰ sched_rt_entity *rt_se,
			ਪੂਰਨ flags)
{
	ਜੇ (!schedstat_enabled())
		ਵਾਪਸ;

	ਜੇ (flags & ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_WAKEUP)
		update_stats_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_sleeper_rt(rt_rq, rt_se);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
update_stats_wait_ਸਿਰਾ_rt(ਬਣਤਰ rt_rq *rt_rq, ਬਣਤਰ sched_rt_entity *rt_se)
{
	ਬਣਤਰ sched_statistics *stats;
	ਬਣਤਰ task_struct *p = ਨਿਰਾ;

	ਜੇ (!schedstat_enabled())
		ਵਾਪਸ;

	ਜੇ (rt_entity_is_task(rt_se))
		p = rt_task_ਦਾ(rt_se);

	stats = __schedstats_from_rt_se(rt_se);
	ਜੇ (!stats)
		ਵਾਪਸ;

	__update_stats_wait_end(rq_ਦਾ_rt_rq(rt_rq), p, stats);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
update_stats_ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_rt(ਬਣਤਰ rt_rq *rt_rq, ਬਣਤਰ sched_rt_entity *rt_se,
			ਪੂਰਨ flags)
{
	ਬਣਤਰ task_struct *p = ਨਿਰਾ;
	ਬਣਤਰ rq *rq = rq_ਦਾ_rt_rq(rt_rq);

	ਜੇ (!schedstat_enabled())
		ਵਾਪਸ;

	ਜੇ (rt_entity_is_task(rt_se)) {
		p = rt_task_ਦਾ(rt_se);

		ਜੇ (p != rq->curr)
			update_stats_wait_ਸਿਰਾ_rt(rt_rq, rt_se);
	}

	ਜੇ ((flags & ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_SLEEP) && p) {
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ state;

		state = READ_ONCE(p->__state);
		ਜੇ (state & TASK_INTERRUPTIBLE)
			__schedstat_set(p->stats.sleep_ਸ਼ੁਰੂ,
					rq_clock(rq_ਦਾ_rt_rq(rt_rq)));

		ਜੇ (state & TASK_UNINTERRUPTIBLE)
			__schedstat_set(p->stats.ਖੰਡ_ਸ਼ੁਰੂ,
					rq_clock(rq_ਦਾ_rt_rq(rt_rq)));
	}
}

ਸਥਿਰ ਖਾਲੀ __enqueue_rt_entity(ਬਣਤਰ sched_rt_entity *rt_se, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags)
{
	ਬਣਤਰ rt_rq *rt_rq = rt_rq_ਦਾ_se(rt_se);
	ਬਣਤਰ rt_prio_array *array = &rt_rq->ਸਰਗਰਮ;
	ਬਣਤਰ rt_rq *ਸਮੂਹ_rq = ਸਮੂਹ_rt_rq(rt_se);
	ਬਣਤਰ list_ਸਿਰ *ਕਤਾਰ = array->ਕਤਾਰ + rt_se_prio(rt_se);

	/*
	 * Don't enqueue the group if its throttled, or when empty.
	 * The latter is a consequence of the former when a child group
	 * get throttled and the current group doesn't have any other
	 * active members.
	 */
	ਜੇ (ਸਮੂਹ_rq && (rt_rq_throttled(ਸਮੂਹ_rq) || !ਸਮੂਹ_rq->rt_nr_running)) {
		ਜੇ (rt_se->on_list)
			__delist_rt_entity(rt_se, array);
		ਵਾਪਸ;
	}

	ਜੇ (move_entity(flags)) {
		WARN_ON_ONCE(rt_se->on_list);
		ਜੇ (flags & ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_ਸਿਰ)
			list_ਜੋੜੋ(&rt_se->ਲੜੀਵਾਰ_list, ਕਤਾਰ);
		ਨਹੀਂਤਾਂ
			list_ਜੋੜੋ_ਪੂਛ(&rt_se->ਲੜੀਵਾਰ_list, ਕਤਾਰ);

		__set_bit(rt_se_prio(rt_se), array->bitmap);
		rt_se->on_list = 1;
	}
	rt_se->on_rq = 1;

	inc_rt_tasks(rt_se, rt_rq);
}

ਸਥਿਰ ਖਾਲੀ __dequeue_rt_entity(ਬਣਤਰ sched_rt_entity *rt_se, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags)
{
	ਬਣਤਰ rt_rq *rt_rq = rt_rq_ਦਾ_se(rt_se);
	ਬਣਤਰ rt_prio_array *array = &rt_rq->ਸਰਗਰਮ;

	ਜੇ (move_entity(flags)) {
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
ਸਥਿਰ ਖਾਲੀ ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_rt_ਢੇਰ(ਬਣਤਰ sched_rt_entity *rt_se, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags)
{
	ਬਣਤਰ sched_rt_entity *ਪਿੱਠ = ਨਿਰਾ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ rt_nr_running;

	for_each_sched_rt_entity(rt_se) {
		rt_se->ਪਿੱਠ = ਪਿੱਠ;
		ਪਿੱਠ = rt_se;
	}

	rt_nr_running = rt_rq_ਦਾ_se(ਪਿੱਠ)->rt_nr_running;

	ਲਈ (rt_se = ਪਿੱਠ; rt_se; rt_se = rt_se->ਪਿੱਠ) {
		ਜੇ (on_rt_rq(rt_se))
			__dequeue_rt_entity(rt_se, flags);
	}

	ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_ਸਿਖਰ_rt_rq(rt_rq_ਦਾ_se(ਪਿੱਠ), rt_nr_running);
}

ਸਥਿਰ ਖਾਲੀ ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_rt_entity(ਬਣਤਰ sched_rt_entity *rt_se, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags)
{
	ਬਣਤਰ rq *rq = rq_ਦਾ_rt_se(rt_se);

	update_stats_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_rt(rt_rq_ਦਾ_se(rt_se), rt_se, flags);

	ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_rt_ਢੇਰ(rt_se, flags);
	for_each_sched_rt_entity(rt_se)
		__enqueue_rt_entity(rt_se, flags);
	ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_ਸਿਖਰ_rt_rq(&rq->rt);
}

ਸਥਿਰ ਖਾਲੀ ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_rt_entity(ਬਣਤਰ sched_rt_entity *rt_se, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags)
{
	ਬਣਤਰ rq *rq = rq_ਦਾ_rt_se(rt_se);

	update_stats_ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_rt(rt_rq_ਦਾ_se(rt_se), rt_se, flags);

	ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_rt_ਢੇਰ(rt_se, flags);

	for_each_sched_rt_entity(rt_se) {
		ਬਣਤਰ rt_rq *rt_rq = ਸਮੂਹ_rt_rq(rt_se);

		ਜੇ (rt_rq && rt_rq->rt_nr_running)
			__enqueue_rt_entity(rt_se, flags);
	}
	ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_ਸਿਖਰ_rt_rq(&rq->rt);
}

/*
 * Adding/removing a task to/from a priority array:
 */
ਸਥਿਰ ਖਾਲੀ
ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_task_rt(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, ਪੂਰਨ flags)
{
	ਬਣਤਰ sched_rt_entity *rt_se = &p->rt;

	ਜੇ (flags & ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_WAKEUP)
		rt_se->timeout = 0;

	check_schedstat_required();
	update_stats_wait_ਸ਼ੁਰੂ_rt(rt_rq_ਦਾ_se(rt_se), rt_se);

	ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_rt_entity(rt_se, flags);

	ਜੇ (task_is_blocked(p))
		ਵਾਪਸ;

	ਜੇ (!task_current(rq, p) && p->nr_cpus_allowed > 1)
		ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_pushable_task(rq, p);
}

ਸਥਿਰ bool ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_task_rt(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, ਪੂਰਨ flags)
{
	ਬਣਤਰ sched_rt_entity *rt_se = &p->rt;

	update_curr_rt(rq);
	ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_rt_entity(rt_se, flags);

	ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_pushable_task(rq, p);

	ਵਾਪਸ true;
}

/*
 * Put task to the head or the end of the run list without the overhead of
 * dequeue followed by enqueue.
 */
ਸਥਿਰ ਖਾਲੀ
requeue_rt_entity(ਬਣਤਰ rt_rq *rt_rq, ਬਣਤਰ sched_rt_entity *rt_se, ਪੂਰਨ ਸਿਰ)
{
	ਜੇ (on_rt_rq(rt_se)) {
		ਬਣਤਰ rt_prio_array *array = &rt_rq->ਸਰਗਰਮ;
		ਬਣਤਰ list_ਸਿਰ *ਕਤਾਰ = array->ਕਤਾਰ + rt_se_prio(rt_se);

		ਜੇ (ਸਿਰ)
			list_move(&rt_se->ਲੜੀਵਾਰ_list, ਕਤਾਰ);
		ਨਹੀਂਤਾਂ
			list_move_ਪੂਛ(&rt_se->ਲੜੀਵਾਰ_list, ਕਤਾਰ);
	}
}

ਸਥਿਰ ਖਾਲੀ requeue_task_rt(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, ਪੂਰਨ ਸਿਰ)
{
	ਬਣਤਰ sched_rt_entity *rt_se = &p->rt;
	ਬਣਤਰ rt_rq *rt_rq;

	for_each_sched_rt_entity(rt_se) {
		rt_rq = rt_rq_ਦਾ_se(rt_se);
		requeue_rt_entity(rt_rq, rt_se, ਸਿਰ);
	}
}

ਸਥਿਰ ਖਾਲੀ yield_task_rt(ਬਣਤਰ rq *rq)
{
	requeue_task_rt(rq, rq->donor, 0);
}

ਸਥਿਰ ਪੂਰਨ ਲੱਭੋ_ਸਭ_ਤੋਂ_ਨੀਵਾਂ_rq(ਬਣਤਰ task_struct *task);

ਸਥਿਰ ਪੂਰਨ
select_task_rq_rt(ਬਣਤਰ task_struct *p, ਪੂਰਨ cpu, ਪੂਰਨ flags)
{
	ਬਣਤਰ task_struct *curr, *donor;
	ਬਣਤਰ rq *rq;
	bool test;

	/* For anything but wake ups, just return the task_cpu */
	ਜੇ (!(flags & (WF_TTWU | WF_FORK)))
		ਜਾਓ ਬਾਹਰਲਾ;

	rq = cpu_rq(cpu);

	rcu_read_ਤਾਲਾ();
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

	ਜੇ (test || !rt_task_ਸਮਾਉਂਦਾ_ਸਮਰੱਥਾ(p, cpu)) {
		ਪੂਰਨ ਨਿਸ਼ਾਨਾ = ਲੱਭੋ_ਸਭ_ਤੋਂ_ਨੀਵਾਂ_rq(p);

		/*
		 * Bail out if we were forcing a migration to find a better
		 * fitting CPU but our search failed.
		 */
		ਜੇ (!test && ਨਿਸ਼ਾਨਾ != -1 && !rt_task_ਸਮਾਉਂਦਾ_ਸਮਰੱਥਾ(p, ਨਿਸ਼ਾਨਾ))
			ਜਾਓ ਬਾਹਰਲਾ_unlock;

		/*
		 * Don't bother moving it if the destination CPU is
		 * not running a lower priority task.
		 */
		ਜੇ (ਨਿਸ਼ਾਨਾ != -1 &&
		    p->prio < cpu_rq(ਨਿਸ਼ਾਨਾ)->rt.highest_prio.curr)
			cpu = ਨਿਸ਼ਾਨਾ;
	}

ਬਾਹਰਲਾ_unlock:
	rcu_read_unlock();

ਬਾਹਰਲਾ:
	ਵਾਪਸ cpu;
}

ਸਥਿਰ ਖਾਲੀ check_preempt_equal_prio(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p)
{
	ਜੇ (rq->curr->nr_cpus_allowed == 1 ||
	    !cpupri_ਲੱਭੋ(&rq->rd->cpupri, rq->donor, ਨਿਰਾ))
		ਵਾਪਸ;

	/*
	 * p is migratable, so let's not schedule it and
	 * see if it is pushed or pulled somewhere else.
	 */
	ਜੇ (p->nr_cpus_allowed != 1 &&
	    cpupri_ਲੱਭੋ(&rq->rd->cpupri, p, ਨਿਰਾ))
		ਵਾਪਸ;

	/*
	 * There appear to be other CPUs that can accept
	 * the current task but none can run 'p', so lets reschedule
	 * to try and push the current task away:
	 */
	requeue_task_rt(rq, p, 1);
	resched_curr(rq);
}

ਸਥਿਰ ਪੂਰਨ balance_rt(ਬਣਤਰ rq *rq, ਬਣਤਰ rq_flags *rf)
{
	/*
	 * Note, rq->donor may change during rq lock drops,
	 * so don't re-use p across lock drops
	 */
	ਬਣਤਰ task_struct *p = rq->donor;

	ਜੇ (!on_rt_rq(&p->rt) && need_pull_rt_task(rq, p)) {
		/*
		 * This is OK, because current is on_cpu, which avoids it being
		 * picked for load-balance and preemption/IRQs are still
		 * disabled avoiding further scheduler activity on it and we've
		 * not yet started the picking loop.
		 */
		rq_unpin_ਤਾਲਾ(rq, rf);
		pull_rt_task(rq);
		rq_repin_ਤਾਲਾ(rq, rf);
	}

	ਵਾਪਸ sched_stop_runnable(rq) || sched_dl_runnable(rq) || sched_rt_runnable(rq);
}

/*
 * Preempt the current task with a newly woken task if needed:
 */
ਸਥਿਰ ਖਾਲੀ wakeup_preempt_rt(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, ਪੂਰਨ flags)
{
	ਬਣਤਰ task_struct *donor = rq->donor;

	/*
	 * XXX If we're preempted by DL, queue a push?
	 */
	ਜੇ (p->sched_class != &rt_sched_class ||
	    donor->sched_class != &rt_sched_class)
		ਵਾਪਸ;

	ਜੇ (p->prio < donor->prio) {
		resched_curr(rq);
		ਵਾਪਸ;
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
	ਜੇ (p->prio == donor->prio && !test_tsk_need_resched(rq->curr))
		check_preempt_equal_prio(rq, p);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ ਸੈੱਟ_ਅਗਲਾ_task_rt(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, ਗਿਣਤੀ snt_e type)
{
	ਬਣਤਰ sched_rt_entity *rt_se = &p->rt;
	ਬਣਤਰ rt_rq *rt_rq = &rq->rt;

	ਜੇ (type == SNT_REPICK)
		ਵਾਪਸ;

	p->se.exec_ਸ਼ੁਰੂ = rq_clock_task(rq);
	ਜੇ (on_rt_rq(&p->rt))
		update_stats_wait_ਸਿਰਾ_rt(rt_rq, rt_se);

	/* The running task is never eligible for pushing */
	ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_pushable_task(rq, p);

	ਜੇ (type != SNT_PICK)
		ਵਾਪਸ;

	/*
	 * If prev task was rt, put_prev_task() has already updated the
	 * utilization. We only care of the case where we start to schedule a
	 * rt task
	 */
	ਜੇ (rq->donor->sched_class != &rt_sched_class)
		update_rt_rq_ਲੱਦੋ_avg(rq_clock_pelt(rq), rq, 0);

	rt_ਕਤਾਰ_ਧੱਕੋ_tasks(rq);
}

ਸਥਿਰ ਬਣਤਰ sched_rt_entity *pick_ਅਗਲਾ_rt_entity(ਬਣਤਰ rt_rq *rt_rq)
{
	ਬਣਤਰ rt_prio_array *array = &rt_rq->ਸਰਗਰਮ;
	ਬਣਤਰ sched_rt_entity *ਅਗਲਾ = ਨਿਰਾ;
	ਬਣਤਰ list_ਸਿਰ *ਕਤਾਰ;
	ਪੂਰਨ idx;

	idx = sched_ਲੱਭੋ_ਪਹਿਲਾ_ਦੋਆਧਾਰੀ_ਅੰਕ(array->bitmap);
	BUG_ON(idx >= MAX_RT_PRIO);

	ਕਤਾਰ = array->ਕਤਾਰ + idx;
	ਜੇ (WARN_ON_ONCE(list_ਖਾਲੀ_ਥਾਵਾਂ(ਕਤਾਰ)))
		ਵਾਪਸ ਨਿਰਾ;
	ਅਗਲਾ = list_ਇੰਦਰਾਜ(ਕਤਾਰ->ਅਗਲਾ, ਬਣਤਰ sched_rt_entity, ਲੜੀਵਾਰ_list);

	ਵਾਪਸ ਅਗਲਾ;
}

ਸਥਿਰ ਬਣਤਰ task_struct *pick_ਅਗਲਾ_task_rt(ਬਣਤਰ rq *rq)
{
	ਬਣਤਰ sched_rt_entity *rt_se;
	ਬਣਤਰ rt_rq *rt_rq  = &rq->rt;

	ਕਰੋ {
		rt_se = pick_ਅਗਲਾ_rt_entity(rt_rq);
		ਜੇ (unlikely(!rt_se))
			ਵਾਪਸ ਨਿਰਾ;
		rt_rq = ਸਮੂਹ_rt_rq(rt_se);
	} ਜਦੋਂ (rt_rq);

	ਵਾਪਸ rt_task_ਦਾ(rt_se);
}

ਸਥਿਰ ਬਣਤਰ task_struct *pick_task_rt(ਬਣਤਰ rq *rq, ਬਣਤਰ rq_flags *rf)
{
	ਬਣਤਰ task_struct *p;

	ਜੇ (!sched_rt_runnable(rq))
		ਵਾਪਸ ਨਿਰਾ;

	p = pick_ਅਗਲਾ_task_rt(rq);

	ਵਾਪਸ p;
}

ਸਥਿਰ ਖਾਲੀ put_prev_task_rt(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, ਬਣਤਰ task_struct *ਅਗਲਾ)
{
	ਬਣਤਰ sched_rt_entity *rt_se = &p->rt;
	ਬਣਤਰ rt_rq *rt_rq = &rq->rt;

	ਜੇ (on_rt_rq(&p->rt))
		update_stats_wait_ਸ਼ੁਰੂ_rt(rt_rq, rt_se);

	update_curr_rt(rq);

	update_rt_rq_ਲੱਦੋ_avg(rq_clock_pelt(rq), rq, 1);

	ਜੇ (task_is_blocked(p))
		ਵਾਪਸ;
	/*
	 * The previous task needs to be made eligible for pushing
	 * if it is still active
	 */
	ਜੇ (on_rt_rq(&p->rt) && p->nr_cpus_allowed > 1)
		ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_pushable_task(rq, p);
}

/* Only try algorithms three times */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ RT_MAX_TRIES 3

/*
 * Return the highest pushable rq's task, which is suitable to be executed
 * on the CPU, NULL otherwise
 */
ਸਥਿਰ ਬਣਤਰ task_struct *pick_highest_pushable_task(ਬਣਤਰ rq *rq, ਪੂਰਨ cpu)
{
	ਬਣਤਰ plist_ਸਿਰ *ਸਿਰ = &rq->rt.pushable_tasks;
	ਬਣਤਰ task_struct *p;

	ਜੇ (!has_pushable_tasks(rq))
		ਵਾਪਸ ਨਿਰਾ;

	plist_for_each_ਇੰਦਰਾਜ(p, ਸਿਰ, pushable_tasks) {
		ਜੇ (task_is_pushable(rq, p, cpu))
			ਵਾਪਸ p;
	}

	ਵਾਪਸ ਨਿਰਾ;
}

ਸਥਿਰ DEFINE_ਪ੍ਰਤੀ_CPU(cpumask_var_t, local_cpu_ਨਕਾਬ);

ਸਥਿਰ ਪੂਰਨ ਲੱਭੋ_ਸਭ_ਤੋਂ_ਨੀਵਾਂ_rq(ਬਣਤਰ task_struct *task)
{
	ਬਣਤਰ sched_domain *sd;
	ਬਣਤਰ cpumask *ਸਭ_ਤੋਂ_ਨੀਵਾਂ_ਨਕਾਬ = this_cpu_cpumask_var_ptr(local_cpu_ਨਕਾਬ);
	ਪੂਰਨ this_cpu = smp_processor_id();
	ਪੂਰਨ cpu      = task_cpu(task);
	ਪੂਰਨ ret;

	/* Make sure the mask is initialized first */
	ਜੇ (unlikely(!ਸਭ_ਤੋਂ_ਨੀਵਾਂ_ਨਕਾਬ))
		ਵਾਪਸ -1;

	ਜੇ (task->nr_cpus_allowed == 1)
		ਵਾਪਸ -1; /* No other targets possible */

	/*
	 * If we're on asym system ensure we consider the different capacities
	 * of the CPUs when searching for the lowest_mask.
	 */
	ਜੇ (sched_asym_cpucap_ਸਰਗਰਮ()) {

		ret = cpupri_ਲੱਭੋ_fitness(&task_rq(task)->rd->cpupri,
					  task, ਸਭ_ਤੋਂ_ਨੀਵਾਂ_ਨਕਾਬ,
					  rt_task_ਸਮਾਉਂਦਾ_ਸਮਰੱਥਾ);
	} ਨਹੀਂਤਾਂ {

		ret = cpupri_ਲੱਭੋ(&task_rq(task)->rd->cpupri,
				  task, ਸਭ_ਤੋਂ_ਨੀਵਾਂ_ਨਕਾਬ);
	}

	ਜੇ (!ret)
		ਵਾਪਸ -1; /* No targets found */

	/*
	 * At this point we have built a mask of CPUs representing the
	 * lowest priority tasks in the system.  Now we want to elect
	 * the best one based on our affinity and topology.
	 *
	 * We prioritize the last CPU that the task executed on since
	 * it is most likely cache-hot in that location.
	 */
	ਜੇ (cpumask_test_cpu(cpu, ਸਭ_ਤੋਂ_ਨੀਵਾਂ_ਨਕਾਬ))
		ਵਾਪਸ cpu;

	/*
	 * Otherwise, we consult the sched_domains span maps to figure
	 * out which CPU is logically closest to our hot cache data.
	 */
	ਜੇ (!cpumask_test_cpu(this_cpu, ਸਭ_ਤੋਂ_ਨੀਵਾਂ_ਨਕਾਬ))
		this_cpu = -1; /* Skip this_cpu opt if not among lowest */

	rcu_read_ਤਾਲਾ();
	for_each_domain(cpu, sd) {
		ਜੇ (sd->flags & SD_WAKE_AFFINE) {
			ਪੂਰਨ ਉੱਤਮ_cpu;

			/*
			 * "this_cpu" is cheaper to preempt than a
			 * remote processor.
			 */
			ਜੇ (this_cpu != -1 &&
			    cpumask_test_cpu(this_cpu, sched_domain_ਵਿਸਤਾਰ(sd))) {
				rcu_read_unlock();
				ਵਾਪਸ this_cpu;
			}

			ਉੱਤਮ_cpu = cpumask_any_and_distribute(ਸਭ_ਤੋਂ_ਨੀਵਾਂ_ਨਕਾਬ,
							      sched_domain_ਵਿਸਤਾਰ(sd));
			ਜੇ (ਉੱਤਮ_cpu < nr_cpu_ids) {
				rcu_read_unlock();
				ਵਾਪਸ ਉੱਤਮ_cpu;
			}
		}
	}
	rcu_read_unlock();

	/*
	 * And finally, if there were no matches within the domains
	 * just give the caller *something* to work with from the compatible
	 * locations.
	 */
	ਜੇ (this_cpu != -1)
		ਵਾਪਸ this_cpu;

	cpu = cpumask_any_distribute(ਸਭ_ਤੋਂ_ਨੀਵਾਂ_ਨਕਾਬ);
	ਜੇ (cpu < nr_cpu_ids)
		ਵਾਪਸ cpu;

	ਵਾਪਸ -1;
}

ਸਥਿਰ ਬਣਤਰ task_struct *pick_ਅਗਲਾ_pushable_task(ਬਣਤਰ rq *rq)
{
	ਬਣਤਰ plist_ਸਿਰ *ਸਿਰ = &rq->rt.pushable_tasks;
	ਬਣਤਰ task_struct *i, *p = ਨਿਰਾ;

	ਜੇ (!has_pushable_tasks(rq))
		ਵਾਪਸ ਨਿਰਾ;

	plist_for_each_ਇੰਦਰਾਜ(i, ਸਿਰ, pushable_tasks) {
		/* skip tasks that cannot be migrated */
		ਜੇ (!task_on_cpu(rq, i) && !is_migration_disabled(i)) {
			p = i;
			ਰੋਕੋ;
		}
	}

	ਜੇ (!p)
		ਵਾਪਸ ਨਿਰਾ;

	BUG_ON(rq->cpu != task_cpu(p));
	BUG_ON(task_current(rq, p));
	BUG_ON(task_current_donor(rq, p));
	BUG_ON(p->nr_cpus_allowed <= 1);

	BUG_ON(!task_on_rq_queued(p));
	BUG_ON(!rt_task(p));

	ਵਾਪਸ p;
}

/* Will lock the rq it finds */
ਸਥਿਰ ਬਣਤਰ rq *ਲੱਭੋ_ਤਾਲਾ_ਸਭ_ਤੋਂ_ਨੀਵਾਂ_rq(ਬਣਤਰ task_struct *task, ਬਣਤਰ rq *rq)
{
	ਬਣਤਰ rq *ਸਭ_ਤੋਂ_ਨੀਵਾਂ_rq = ਨਿਰਾ;
	ਪੂਰਨ tries;
	ਪੂਰਨ cpu;

	ਲਈ (tries = 0; tries < RT_MAX_TRIES; tries++) {
		cpu = ਲੱਭੋ_ਸਭ_ਤੋਂ_ਨੀਵਾਂ_rq(task);

		ਜੇ ((cpu == -1) || (cpu == rq->cpu))
			ਰੋਕੋ;

		ਸਭ_ਤੋਂ_ਨੀਵਾਂ_rq = cpu_rq(cpu);

		ਜੇ (ਸਭ_ਤੋਂ_ਨੀਵਾਂ_rq->rt.highest_prio.curr <= task->prio) {
			/*
			 * Target rq has tasks of equal or higher priority,
			 * retrying does not release any lock and is unlikely
			 * to yield a different result.
			 */
			ਸਭ_ਤੋਂ_ਨੀਵਾਂ_rq = ਨਿਰਾ;
			ਰੋਕੋ;
		}

		/* if the prio of this runqueue changed, try again */
		ਜੇ (double_ਤਾਲਾ_balance(rq, ਸਭ_ਤੋਂ_ਨੀਵਾਂ_rq)) {
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
			ਜੇ (unlikely(is_migration_disabled(task) ||
				     !cpumask_test_cpu(ਸਭ_ਤੋਂ_ਨੀਵਾਂ_rq->cpu, &task->cpus_ਨਕਾਬ) ||
				     task != pick_ਅਗਲਾ_pushable_task(rq))) {

				double_unlock_balance(rq, ਸਭ_ਤੋਂ_ਨੀਵਾਂ_rq);
				ਸਭ_ਤੋਂ_ਨੀਵਾਂ_rq = ਨਿਰਾ;
				ਰੋਕੋ;
			}
		}

		/* If this rq is still suitable use it. */
		ਜੇ (ਸਭ_ਤੋਂ_ਨੀਵਾਂ_rq->rt.highest_prio.curr > task->prio)
			ਰੋਕੋ;

		/* try again */
		double_unlock_balance(rq, ਸਭ_ਤੋਂ_ਨੀਵਾਂ_rq);
		ਸਭ_ਤੋਂ_ਨੀਵਾਂ_rq = ਨਿਰਾ;
	}

	ਵਾਪਸ ਸਭ_ਤੋਂ_ਨੀਵਾਂ_rq;
}

/*
 * If the current CPU has more than one RT task, see if the non
 * running task can migrate over to a CPU that is running a task
 * of lesser priority.
 */
ਸਥਿਰ ਪੂਰਨ ਧੱਕੋ_rt_task(ਬਣਤਰ rq *rq, bool pull)
{
	ਬਣਤਰ task_struct *ਅਗਲਾ_task;
	ਬਣਤਰ rq *ਸਭ_ਤੋਂ_ਨੀਵਾਂ_rq;
	ਪੂਰਨ ret = 0;

	ਜੇ (!rq->rt.overloaded)
		ਵਾਪਸ 0;

	ਅਗਲਾ_task = pick_ਅਗਲਾ_pushable_task(rq);
	ਜੇ (!ਅਗਲਾ_task)
		ਵਾਪਸ 0;

retry:
	/*
	 * It's possible that the next_task slipped in of
	 * higher priority than current. If that's the case
	 * just reschedule current.
	 */
	ਜੇ (unlikely(ਅਗਲਾ_task->prio < rq->donor->prio)) {
		resched_curr(rq);
		ਵਾਪਸ 0;
	}

	ਜੇ (is_migration_disabled(ਅਗਲਾ_task)) {
		ਬਣਤਰ task_struct *ਧੱਕੋ_task = ਨਿਰਾ;
		ਪੂਰਨ cpu;

		ਜੇ (!pull || rq->ਧੱਕੋ_busy)
			ਵਾਪਸ 0;

		/*
		 * Invoking find_lowest_rq() on anything but an RT task doesn't
		 * make sense. Per the above priority check, curr has to
		 * be of higher priority than next_task, so no need to
		 * reschedule when bailing out.
		 *
		 * Note that the stoppers are masqueraded as SCHED_FIFO
		 * (cf. sched_set_stop_task()), so we can't rely on rt_task().
		 */
		ਜੇ (rq->donor->sched_class != &rt_sched_class)
			ਵਾਪਸ 0;

		cpu = ਲੱਭੋ_ਸਭ_ਤੋਂ_ਨੀਵਾਂ_rq(rq->curr);
		ਜੇ (cpu == -1 || cpu == rq->cpu)
			ਵਾਪਸ 0;

		/*
		 * Given we found a CPU with lower priority than @next_task,
		 * therefore it should be running. However we cannot migrate it
		 * to this other CPU, instead attempt to push the current
		 * running task on this CPU away.
		 */
		ਧੱਕੋ_task = get_ਧੱਕੋ_task(rq);
		ਜੇ (ਧੱਕੋ_task) {
			preempt_disable();
			raw_spin_rq_unlock(rq);
			stop_one_cpu_nowait(rq->cpu, ਧੱਕੋ_cpu_stop,
					    ਧੱਕੋ_task, &rq->ਧੱਕੋ_work);
			preempt_enable();
			raw_spin_rq_ਤਾਲਾ(rq);
		}

		ਵਾਪਸ 0;
	}

	ਜੇ (WARN_ON(ਅਗਲਾ_task == rq->curr))
		ਵਾਪਸ 0;

	/* We might release rq lock */
	get_task_struct(ਅਗਲਾ_task);

	/* find_lock_lowest_rq locks the rq if found */
	ਸਭ_ਤੋਂ_ਨੀਵਾਂ_rq = ਲੱਭੋ_ਤਾਲਾ_ਸਭ_ਤੋਂ_ਨੀਵਾਂ_rq(ਅਗਲਾ_task, rq);
	ਜੇ (!ਸਭ_ਤੋਂ_ਨੀਵਾਂ_rq) {
		ਬਣਤਰ task_struct *task;
		/*
		 * find_lock_lowest_rq releases rq->lock
		 * so it is possible that next_task has migrated.
		 *
		 * We need to make sure that the task is still on the same
		 * run-queue and is also still the next task eligible for
		 * pushing.
		 */
		task = pick_ਅਗਲਾ_pushable_task(rq);
		ਜੇ (task == ਅਗਲਾ_task) {
			/*
			 * The task hasn't migrated, and is still the next
			 * eligible task, but we failed to find a run-queue
			 * to push it to.  Do not retry in this case, since
			 * other CPUs will pull from us when ready.
			 */
			ਜਾਓ ਬਾਹਰਲਾ;
		}

		ਜੇ (!task)
			/* No more tasks, just exit */
			ਜਾਓ ਬਾਹਰਲਾ;

		/*
		 * Something has shifted, try again.
		 */
		put_task_struct(ਅਗਲਾ_task);
		ਅਗਲਾ_task = task;
		ਜਾਓ retry;
	}

	move_queued_task_locked(rq, ਸਭ_ਤੋਂ_ਨੀਵਾਂ_rq, ਅਗਲਾ_task);
	resched_curr(ਸਭ_ਤੋਂ_ਨੀਵਾਂ_rq);
	ret = 1;

	double_unlock_balance(rq, ਸਭ_ਤੋਂ_ਨੀਵਾਂ_rq);
ਬਾਹਰਲਾ:
	put_task_struct(ਅਗਲਾ_task);

	ਵਾਪਸ ret;
}

ਸਥਿਰ ਖਾਲੀ ਧੱਕੋ_rt_tasks(ਬਣਤਰ rq *rq)
{
	/* push_rt_task will return true if it moved an RT */
	ਜਦੋਂ (ਧੱਕੋ_rt_task(rq, false))
		;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_RT_ਧੱਕੋ_IPI

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
ਸਥਿਰ ਪੂਰਨ rto_ਅਗਲਾ_cpu(ਬਣਤਰ ਜੜ੍ਹ_domain *rd)
{
	ਪੂਰਨ this_cpu = smp_processor_id();
	ਪੂਰਨ ਅਗਲਾ;
	ਪੂਰਨ cpu;

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
	ਲਈ (;;) {

		/* When rto_cpu is -1 this acts like cpumask_first() */
		cpu = cpumask_ਅਗਲਾ(rd->rto_cpu, rd->rto_ਨਕਾਬ);

		rd->rto_cpu = cpu;

		/* Do not send IPI to self */
		ਜੇ (cpu == this_cpu)
			ਜਾਰੀ;

		ਜੇ (cpu < nr_cpu_ids)
			ਵਾਪਸ cpu;

		rd->rto_cpu = -1;

		/*
		 * ACQUIRE ensures we see the @rto_mask changes
		 * made prior to the @next value observed.
		 *
		 * Matches WMB in rt_set_overload().
		 */
		ਅਗਲਾ = atomic_read_acquire(&rd->rto_loop_ਅਗਲਾ);

		ਜੇ (rd->rto_loop == ਅਗਲਾ)
			ਰੋਕੋ;

		rd->rto_loop = ਅਗਲਾ;
	}

	ਵਾਪਸ -1;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool rto_ਸ਼ੁਰੂ_trylock(atomic_t *v)
{
	ਵਾਪਸ !atomic_cmpxchg_acquire(v, 0, 1);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ rto_ਸ਼ੁਰੂ_unlock(atomic_t *v)
{
	atomic_ਸੈੱਟ_release(v, 0);
}

ਸਥਿਰ ਖਾਲੀ tell_cpu_ਮੰਜ਼ਿਲ_ਧੱਕੋ(ਬਣਤਰ rq *rq)
{
	ਪੂਰਨ cpu = -1;

	/* Keep the loop going if the IPI is currently active */
	atomic_inc(&rq->rd->rto_loop_ਅਗਲਾ);

	/* Only one CPU can initiate a loop at a time */
	ਜੇ (!rto_ਸ਼ੁਰੂ_trylock(&rq->rd->rto_loop_ਸ਼ੁਰੂ))
		ਵਾਪਸ;

	raw_spin_ਤਾਲਾ(&rq->rd->rto_ਤਾਲਾ);

	/*
	 * The rto_cpu is updated under the lock, if it has a valid CPU
	 * then the IPI is still running and will continue due to the
	 * update to loop_next, and nothing needs to be done here.
	 * Otherwise it is finishing up and an IPI needs to be sent.
	 */
	ਜੇ (rq->rd->rto_cpu < 0)
		cpu = rto_ਅਗਲਾ_cpu(rq->rd);

	raw_spin_unlock(&rq->rd->rto_ਤਾਲਾ);

	rto_ਸ਼ੁਰੂ_unlock(&rq->rd->rto_loop_ਸ਼ੁਰੂ);

	ਜੇ (cpu >= 0) {
		/* Make sure the rd does not get freed while pushing */
		sched_get_rd(rq->rd);
		irq_work_ਕਤਾਰ_on(&rq->rd->rto_ਧੱਕੋ_work, cpu);
	}
}

/* Called from hardirq context */
ਖਾਲੀ rto_ਧੱਕੋ_irq_work_func(ਬਣਤਰ irq_work *work)
{
	ਬਣਤਰ ਜੜ੍ਹ_domain *rd =
		container_ਦਾ(work, ਬਣਤਰ ਜੜ੍ਹ_domain, rto_ਧੱਕੋ_work);
	ਬਣਤਰ rq *rq;
	ਪੂਰਨ cpu;

	rq = this_rq();

	/*
	 * We do not need to grab the lock to check for has_pushable_tasks.
	 * When it gets updated, a check is made if a push is possible.
	 */
	ਜੇ (has_pushable_tasks(rq)) {
		raw_spin_rq_ਤਾਲਾ(rq);
		ਜਦੋਂ (ਧੱਕੋ_rt_task(rq, true))
			;
		raw_spin_rq_unlock(rq);
	}

	raw_spin_ਤਾਲਾ(&rd->rto_ਤਾਲਾ);

	/* Pass the IPI to the next rt overloaded queue */
	cpu = rto_ਅਗਲਾ_cpu(rd);

	raw_spin_unlock(&rd->rto_ਤਾਲਾ);

	ਜੇ (cpu < 0) {
		sched_put_rd(rd);
		ਵਾਪਸ;
	}

	/* Try the next RT overloaded CPU */
	irq_work_ਕਤਾਰ_on(&rd->rto_ਧੱਕੋ_work, cpu);
}
#ਜੇ_ਅੰਤ /* HAVE_RT_PUSH_IPI */

ਸਥਿਰ ਖਾਲੀ pull_rt_task(ਬਣਤਰ rq *this_rq)
{
	ਪੂਰਨ this_cpu = this_rq->cpu, cpu;
	bool resched = false;
	ਬਣਤਰ task_struct *p, *ਧੱਕੋ_task;
	ਬਣਤਰ rq *src_rq;
	ਪੂਰਨ rt_overload_ਗਿਣਤੀ = rt_overloaded(this_rq);

	ਜੇ (likely(!rt_overload_ਗਿਣਤੀ))
		ਵਾਪਸ;

	/*
	 * Match the barrier from rt_set_overloaded; this guarantees that if we
	 * see overloaded we must also see the rto_mask bit.
	 */
	smp_rmb();

	/* If we are the only overloaded CPU do nothing */
	ਜੇ (rt_overload_ਗਿਣਤੀ == 1 &&
	    cpumask_test_cpu(this_rq->cpu, this_rq->rd->rto_ਨਕਾਬ))
		ਵਾਪਸ;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_RT_ਧੱਕੋ_IPI
	ਜੇ (sched_feat(RT_ਧੱਕੋ_IPI)) {
		tell_cpu_ਮੰਜ਼ਿਲ_ਧੱਕੋ(this_rq);
		ਵਾਪਸ;
	}
#ਜੇ_ਅੰਤ

	for_each_cpu(cpu, this_rq->rd->rto_ਨਕਾਬ) {
		ਜੇ (this_cpu == cpu)
			ਜਾਰੀ;

		src_rq = cpu_rq(cpu);

		/*
		 * Don't bother taking the src_rq->lock if the next highest
		 * task is known to be lower-priority than our current task.
		 * This may look racy, but if this value is about to go
		 * logically higher, the src_rq will push this task away.
		 * And if its going logically lower, we do not care
		 */
		ਜੇ (src_rq->rt.highest_prio.ਅਗਲਾ >=
		    this_rq->rt.highest_prio.curr)
			ਜਾਰੀ;

		/*
		 * We can potentially drop this_rq's lock in
		 * double_lock_balance, and another CPU could
		 * alter this_rq
		 */
		ਧੱਕੋ_task = ਨਿਰਾ;
		double_ਤਾਲਾ_balance(this_rq, src_rq);

		/*
		 * We can pull only a task, which is pushable
		 * on its rq, and no others.
		 */
		p = pick_highest_pushable_task(src_rq, this_cpu);

		/*
		 * Do we have an RT task that preempts
		 * the to-be-scheduled task?
		 */
		ਜੇ (p && (p->prio < this_rq->rt.highest_prio.curr)) {
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
			ਜੇ (p->prio < src_rq->donor->prio)
				ਜਾਓ skip;

			ਜੇ (is_migration_disabled(p)) {
				ਧੱਕੋ_task = get_ਧੱਕੋ_task(src_rq);
			} ਨਹੀਂਤਾਂ {
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

		ਜੇ (ਧੱਕੋ_task) {
			preempt_disable();
			raw_spin_rq_unlock(this_rq);
			stop_one_cpu_nowait(src_rq->cpu, ਧੱਕੋ_cpu_stop,
					    ਧੱਕੋ_task, &src_rq->ਧੱਕੋ_work);
			preempt_enable();
			raw_spin_rq_ਤਾਲਾ(this_rq);
		}
	}

	ਜੇ (resched)
		resched_curr(this_rq);
}

/*
 * If we are not running and we are not going to reschedule soon, we should
 * try to push tasks away now
 */
ਸਥਿਰ ਖਾਲੀ task_woken_rt(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p)
{
	bool need_ਮੰਜ਼ਿਲ_ਧੱਕੋ = !task_on_cpu(rq, p) &&
			    !test_tsk_need_resched(rq->curr) &&
			    p->nr_cpus_allowed > 1 &&
			    (dl_task(rq->donor) || rt_task(rq->donor)) &&
			    (rq->curr->nr_cpus_allowed < 2 ||
			     rq->donor->prio <= p->prio);

	ਜੇ (need_ਮੰਜ਼ਿਲ_ਧੱਕੋ)
		ਧੱਕੋ_rt_tasks(rq);
}

/* Assumes rq->lock is held */
ਸਥਿਰ ਖਾਲੀ rq_online_rt(ਬਣਤਰ rq *rq)
{
	ਜੇ (rq->rt.overloaded)
		rt_ਸੈੱਟ_overload(rq);

	__enable_runtime(rq);

	cpupri_ਸੈੱਟ(&rq->rd->cpupri, rq->cpu, rq->rt.highest_prio.curr);
}

/* Assumes rq->lock is held */
ਸਥਿਰ ਖਾਲੀ rq_offline_rt(ਬਣਤਰ rq *rq)
{
	ਜੇ (rq->rt.overloaded)
		rt_clear_overload(rq);

	__disable_runtime(rq);

	cpupri_ਸੈੱਟ(&rq->rd->cpupri, rq->cpu, CPUPRI_INVALID);
}

/*
 * When switch from the rt queue, we bring ourselves to a position
 * that we might want to pull RT tasks from other runqueues.
 */
ਸਥਿਰ ਖਾਲੀ switched_from_rt(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p)
{
	/*
	 * If there are other RT tasks then we will reschedule
	 * and the scheduling of the other RT tasks will handle
	 * the balancing. But if we are the last RT task
	 * we may need to handle the pulling of RT tasks
	 * now.
	 */
	ਜੇ (!task_on_rq_queued(p) || rq->rt.rt_nr_running)
		ਵਾਪਸ;

	rt_ਕਤਾਰ_pull_task(rq);
}

ਖਾਲੀ __init init_sched_rt_class(ਖਾਲੀ)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i;

	for_each_possible_cpu(i) {
		zalloc_cpumask_var_ਗੰਢ(&ਪ੍ਰਤੀ_cpu(local_cpu_ਨਕਾਬ, i),
					GFP_KERNEL, cpu_ਮੰਜ਼ਿਲ_ਗੰਢ(i));
	}
}

/*
 * When switching a task to RT, we may overload the runqueue
 * with RT tasks. In this case we try to push them off to
 * other runqueues.
 */
ਸਥਿਰ ਖਾਲੀ switched_ਮੰਜ਼ਿਲ_rt(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p)
{
	/*
	 * If we are running, update the avg_rt tracking, as the running time
	 * will now on be accounted into the latter.
	 */
	ਜੇ (task_current(rq, p)) {
		update_rt_rq_ਲੱਦੋ_avg(rq_clock_pelt(rq), rq, 0);
		ਵਾਪਸ;
	}

	/*
	 * If we are not running we may need to preempt the current
	 * running task. If that current running task is also an RT task
	 * then see if we can move to another run queue.
	 */
	ਜੇ (task_on_rq_queued(p)) {
		ਜੇ (p->nr_cpus_allowed > 1 && rq->rt.overloaded)
			rt_ਕਤਾਰ_ਧੱਕੋ_tasks(rq);
		ਜੇ (p->prio < rq->donor->prio && cpu_online(cpu_ਦਾ(rq)))
			resched_curr(rq);
	}
}

/*
 * Priority of the task has changed. This may cause
 * us to initiate a push or pull.
 */
ਸਥਿਰ ਖਾਲੀ
prio_ਬਦਲਿਆ_rt(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, u64 oldprio)
{
	ਜੇ (!task_on_rq_queued(p))
		ਵਾਪਸ;

	ਜੇ (p->prio == oldprio)
		ਵਾਪਸ;

	ਜੇ (task_current_donor(rq, p)) {
		/*
		 * If our priority decreases while running, we
		 * may need to pull tasks to this runqueue.
		 */
		ਜੇ (oldprio < p->prio)
			rt_ਕਤਾਰ_pull_task(rq);

		/*
		 * If there's a higher priority task waiting to run
		 * then reschedule.
		 */
		ਜੇ (p->prio > rq->rt.highest_prio.curr)
			resched_curr(rq);
	} ਨਹੀਂਤਾਂ {
		/*
		 * This task is not running, but if it is
		 * greater than the current running task
		 * then reschedule.
		 */
		ਜੇ (p->prio < rq->donor->prio)
			resched_curr(rq);
	}
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_POSIX_TIMERS
ਸਥਿਰ ਖਾਲੀ watchdog(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ soft, hard;

	/* max may change after cur was read, this will be fixed next tick */
	soft = task_rlimit(p, RLIMIT_RTTIME);
	hard = task_rlimit_max(p, RLIMIT_RTTIME);

	ਜੇ (soft != RLIM_INFINITY) {
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਅਗਲਾ;

		ਜੇ (p->rt.watchdog_stamp != jiffies) {
			p->rt.timeout++;
			p->rt.watchdog_stamp = jiffies;
		}

		ਅਗਲਾ = DIV_ਗੇੜਾ_ਉੱਪਰ(min(soft, hard), USEC_ਪ੍ਰਤੀ_SEC/HZ);
		ਜੇ (p->rt.timeout > ਅਗਲਾ) {
			posix_cputimers_rt_watchdog(&p->posix_cputimers,
						    p->se.ਜੋੜ_exec_runtime);
		}
	}
}
#ਨਹੀਂਤਾਂ /* !CONFIG_POSIX_TIMERS: */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ watchdog(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p) { }
#ਜੇ_ਅੰਤ /* !CONFIG_POSIX_TIMERS */

/*
 * scheduler tick hitting a task of our scheduling class.
 *
 * NOTE: This function can be called remotely by the tick offload that
 * goes along full dynticks. Therefore no local assumption can be made
 * and everything must be accessed through the @rq and @curr passed in
 * parameters.
 */
ਸਥਿਰ ਖਾਲੀ task_tick_rt(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, ਪੂਰਨ queued)
{
	ਬਣਤਰ sched_rt_entity *rt_se = &p->rt;

	update_curr_rt(rq);
	update_rt_rq_ਲੱਦੋ_avg(rq_clock_pelt(rq), rq, 1);

	watchdog(rq, p);

	/*
	 * RR tasks need a special form of time-slice management.
	 * FIFO tasks have no timeslices.
	 */
	ਜੇ (p->policy != SCHED_RR)
		ਵਾਪਸ;

	ਜੇ (--p->rt.ਸਮਾਂ_ਟੁਕੜਾ)
		ਵਾਪਸ;

	p->rt.ਸਮਾਂ_ਟੁਕੜਾ = sched_rr_timeslice;

	/*
	 * Requeue to the end of queue if we (and all of our ancestors) are not
	 * the only element on the queue
	 */
	for_each_sched_rt_entity(rt_se) {
		ਜੇ (rt_se->ਲੜੀਵਾਰ_list.prev != rt_se->ਲੜੀਵਾਰ_list.ਅਗਲਾ) {
			requeue_task_rt(rq, p, 0);
			resched_curr(rq);
			ਵਾਪਸ;
		}
	}
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_rr_interval_rt(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *task)
{
	/*
	 * Time slice is 0 for SCHED_FIFO tasks
	 */
	ਜੇ (task->policy == SCHED_RR)
		ਵਾਪਸ sched_rr_timeslice;
	ਨਹੀਂਤਾਂ
		ਵਾਪਸ 0;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CORE
ਸਥਿਰ ਪੂਰਨ task_is_throttled_rt(ਬਣਤਰ task_struct *p, ਪੂਰਨ cpu)
{
	ਬਣਤਰ rt_rq *rt_rq;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_RT_ਸਮੂਹ_SCHED // XXX maybe add task_rt_rq(), see also sched_rt_period_rt_rq
	rt_rq = task_ਸਮੂਹ(p)->rt_rq[cpu];
	WARN_ON(!rt_ਸਮੂਹ_sched_enabled() && rt_rq->tg != &ਜੜ੍ਹ_task_ਸਮੂਹ);
#ਨਹੀਂਤਾਂ
	rt_rq = &cpu_rq(cpu)->rt;
#ਜੇ_ਅੰਤ

	ਵਾਪਸ rt_rq_throttled(rt_rq);
}
#ਜੇ_ਅੰਤ /* CONFIG_SCHED_CORE */

DEFINE_SCHED_CLASS(rt) = {
	.ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_task		= ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_task_rt,
	.ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_task		= ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_task_rt,
	.yield_task		= yield_task_rt,

	.wakeup_preempt		= wakeup_preempt_rt,

	.pick_task		= pick_task_rt,
	.put_prev_task		= put_prev_task_rt,
	.ਸੈੱਟ_ਅਗਲਾ_task          = ਸੈੱਟ_ਅਗਲਾ_task_rt,

	.balance		= balance_rt,
	.select_task_rq		= select_task_rq_rt,
	.ਸੈੱਟ_cpus_allowed       = ਸੈੱਟ_cpus_allowed_common,
	.rq_online              = rq_online_rt,
	.rq_offline             = rq_offline_rt,
	.task_woken		= task_woken_rt,
	.switched_from		= switched_from_rt,
	.ਲੱਭੋ_ਤਾਲਾ_rq		= ਲੱਭੋ_ਤਾਲਾ_ਸਭ_ਤੋਂ_ਨੀਵਾਂ_rq,

	.task_tick		= task_tick_rt,

	.get_rr_interval	= get_rr_interval_rt,

	.switched_ਮੰਜ਼ਿਲ		= switched_ਮੰਜ਼ਿਲ_rt,
	.prio_ਬਦਲਿਆ		= prio_ਬਦਲਿਆ_rt,

	.update_curr		= update_curr_rt,

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CORE
	.task_is_throttled	= task_is_throttled_rt,
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_UCLAMP_TASK
	.uclamp_enabled		= 1,
#ਜੇ_ਅੰਤ
};

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_RT_ਸਮੂਹ_SCHED
/*
 * Ensure that the real time constraints are schedulable.
 */
ਸਥਿਰ DEFINE_MUTEX(rt_constraints_mutex);

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ tg_has_rt_tasks(ਬਣਤਰ task_ਸਮੂਹ *tg)
{
	ਬਣਤਰ task_struct *task;
	ਬਣਤਰ css_task_iter it;
	ਪੂਰਨ ret = 0;

	/*
	 * Autogroups do not have RT tasks; see autogroup_create().
	 */
	ਜੇ (task_ਸਮੂਹ_is_autogroup(tg))
		ਵਾਪਸ 0;

	css_task_iter_ਸ਼ੁਰੂ(&tg->css, 0, &it);
	ਜਦੋਂ (!ret && (task = css_task_iter_ਅਗਲਾ(&it)))
		ret |= rt_task(task);
	css_task_iter_ਸਿਰਾ(&it);

	ਵਾਪਸ ret;
}

ਬਣਤਰ rt_schedulable_ਅੰਕੜੇ {
	ਬਣਤਰ task_ਸਮੂਹ *tg;
	u64 rt_period;
	u64 rt_runtime;
};

ਸਥਿਰ ਪੂਰਨ tg_rt_schedulable(ਬਣਤਰ task_ਸਮੂਹ *tg, ਖਾਲੀ *ਅੰਕੜੇ)
{
	ਬਣਤਰ rt_schedulable_ਅੰਕੜੇ *d = ਅੰਕੜੇ;
	ਬਣਤਰ task_ਸਮੂਹ *ਬੱਚਾ;
	u64 ਕੁੱਲ, ਜੋੜ = 0;
	u64 period, runtime;

	period = ktime_ਮੰਜ਼ਿਲ_ns(tg->rt_bandwidth.rt_period);
	runtime = tg->rt_bandwidth.rt_runtime;

	ਜੇ (tg == d->tg) {
		period = d->rt_period;
		runtime = d->rt_runtime;
	}

	/*
	 * Cannot have more runtime than the period.
	 */
	ਜੇ (runtime > period && runtime != RUNTIME_INF)
		ਵਾਪਸ -EINVAL;

	/*
	 * Ensure we don't starve existing RT tasks if runtime turns zero.
	 */
	ਜੇ (rt_bandwidth_enabled() && !runtime &&
	    tg->rt_bandwidth.rt_runtime && tg_has_rt_tasks(tg))
		ਵਾਪਸ -EBUSY;

	ਕੁੱਲ = ਮੰਜ਼ਿਲ_ਅਨੁਪਾਤ(period, runtime);

	/*
	 * Nobody can have more than the global setting allows.
	 */
	ਜੇ (ਕੁੱਲ > ਮੰਜ਼ਿਲ_ਅਨੁਪਾਤ(global_rt_period(), global_rt_runtime()))
		ਵਾਪਸ -EINVAL;

	/*
	 * The sum of our children's runtime should not exceed our own.
	 */
	list_for_each_ਇੰਦਰਾਜ_rcu(ਬੱਚਾ, &tg->children, siblings) {
		period = ktime_ਮੰਜ਼ਿਲ_ns(ਬੱਚਾ->rt_bandwidth.rt_period);
		runtime = ਬੱਚਾ->rt_bandwidth.rt_runtime;

		ਜੇ (ਬੱਚਾ == d->tg) {
			period = d->rt_period;
			runtime = d->rt_runtime;
		}

		ਜੋੜ += ਮੰਜ਼ਿਲ_ਅਨੁਪਾਤ(period, runtime);
	}

	ਜੇ (ਜੋੜ > ਕੁੱਲ)
		ਵਾਪਸ -EINVAL;

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ __rt_schedulable(ਬਣਤਰ task_ਸਮੂਹ *tg, u64 period, u64 runtime)
{
	ਪੂਰਨ ret;

	ਬਣਤਰ rt_schedulable_ਅੰਕੜੇ ਅੰਕੜੇ = {
		.tg = tg,
		.rt_period = period,
		.rt_runtime = runtime,
	};

	rcu_read_ਤਾਲਾ();
	ret = walk_tg_ਰੁੱਖ(tg_rt_schedulable, tg_nop, &ਅੰਕੜੇ);
	rcu_read_unlock();

	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ tg_ਸੈੱਟ_rt_bandwidth(ਬਣਤਰ task_ਸਮੂਹ *tg,
		u64 rt_period, u64 rt_runtime)
{
	ਪੂਰਨ i, err = 0;

	/*
	 * Disallowing the root group RT runtime is BAD, it would disallow the
	 * kernel creating (and or operating) RT threads.
	 */
	ਜੇ (tg == &ਜੜ੍ਹ_task_ਸਮੂਹ && rt_runtime == 0)
		ਵਾਪਸ -EINVAL;

	/* No period doesn't make any sense. */
	ਜੇ (rt_period == 0)
		ਵਾਪਸ -EINVAL;

	/*
	 * Bound quota to defend quota against overflow during bandwidth shift.
	 */
	ਜੇ (rt_runtime != RUNTIME_INF && rt_runtime > max_rt_runtime)
		ਵਾਪਸ -EINVAL;

	mutex_ਤਾਲਾ(&rt_constraints_mutex);
	err = __rt_schedulable(tg, rt_period, rt_runtime);
	ਜੇ (err)
		ਜਾਓ unlock;

	raw_spin_ਤਾਲਾ_irq(&tg->rt_bandwidth.rt_runtime_ਤਾਲਾ);
	tg->rt_bandwidth.rt_period = ns_ਮੰਜ਼ਿਲ_ktime(rt_period);
	tg->rt_bandwidth.rt_runtime = rt_runtime;

	for_each_possible_cpu(i) {
		ਬਣਤਰ rt_rq *rt_rq = tg->rt_rq[i];

		raw_spin_ਤਾਲਾ(&rt_rq->rt_runtime_ਤਾਲਾ);
		rt_rq->rt_runtime = rt_runtime;
		raw_spin_unlock(&rt_rq->rt_runtime_ਤਾਲਾ);
	}
	raw_spin_unlock_irq(&tg->rt_bandwidth.rt_runtime_ਤਾਲਾ);
unlock:
	mutex_unlock(&rt_constraints_mutex);

	ਵਾਪਸ err;
}

ਪੂਰਨ sched_ਸਮੂਹ_ਸੈੱਟ_rt_runtime(ਬਣਤਰ task_ਸਮੂਹ *tg, ਲੰਮਾ rt_runtime_us)
{
	u64 rt_runtime, rt_period;

	rt_period = ktime_ਮੰਜ਼ਿਲ_ns(tg->rt_bandwidth.rt_period);
	rt_runtime = (u64)rt_runtime_us * NSEC_ਪ੍ਰਤੀ_USEC;
	ਜੇ (rt_runtime_us < 0)
		rt_runtime = RUNTIME_INF;
	ਨਹੀਂਤਾਂ ਜੇ ((u64)rt_runtime_us > U64_MAX / NSEC_ਪ੍ਰਤੀ_USEC)
		ਵਾਪਸ -EINVAL;

	ਵਾਪਸ tg_ਸੈੱਟ_rt_bandwidth(tg, rt_period, rt_runtime);
}

ਲੰਮਾ sched_ਸਮੂਹ_rt_runtime(ਬਣਤਰ task_ਸਮੂਹ *tg)
{
	u64 rt_runtime_us;

	ਜੇ (tg->rt_bandwidth.rt_runtime == RUNTIME_INF)
		ਵਾਪਸ -1;

	rt_runtime_us = tg->rt_bandwidth.rt_runtime;
	do_div(rt_runtime_us, NSEC_ਪ੍ਰਤੀ_USEC);
	ਵਾਪਸ rt_runtime_us;
}

ਪੂਰਨ sched_ਸਮੂਹ_ਸੈੱਟ_rt_period(ਬਣਤਰ task_ਸਮੂਹ *tg, u64 rt_period_us)
{
	u64 rt_runtime, rt_period;

	ਜੇ (rt_period_us > U64_MAX / NSEC_ਪ੍ਰਤੀ_USEC)
		ਵਾਪਸ -EINVAL;

	rt_period = rt_period_us * NSEC_ਪ੍ਰਤੀ_USEC;
	rt_runtime = tg->rt_bandwidth.rt_runtime;

	ਵਾਪਸ tg_ਸੈੱਟ_rt_bandwidth(tg, rt_period, rt_runtime);
}

ਲੰਮਾ sched_ਸਮੂਹ_rt_period(ਬਣਤਰ task_ਸਮੂਹ *tg)
{
	u64 rt_period_us;

	rt_period_us = ktime_ਮੰਜ਼ਿਲ_ns(tg->rt_bandwidth.rt_period);
	do_div(rt_period_us, NSEC_ਪ੍ਰਤੀ_USEC);
	ਵਾਪਸ rt_period_us;
}

ਪੂਰਨ sched_rt_can_attach(ਬਣਤਰ task_ਸਮੂਹ *tg, ਬਣਤਰ task_struct *tsk)
{
	/* Don't accept real-time tasks when there is no way for them to run */
	ਜੇ (rt_ਸਮੂਹ_sched_enabled() && rt_task(tsk) && tg->rt_bandwidth.rt_runtime == 0)
		ਵਾਪਸ 0;

	ਵਾਪਸ 1;
}

#ਜੇ_ਅੰਤ /* !CONFIG_RT_GROUP_SCHED */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SYSCTL
ਸਥਿਰ ਪੂਰਨ sched_rt_global_validate(ਖਾਲੀ)
{
	ਜੇ ((sysctl_sched_rt_runtime != RUNTIME_INF) &&
		((sysctl_sched_rt_runtime > sysctl_sched_rt_period) ||
		 ((u64)sysctl_sched_rt_runtime *
			NSEC_ਪ੍ਰਤੀ_USEC > max_rt_runtime)))
		ਵਾਪਸ -EINVAL;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_RT_ਸਮੂਹ_SCHED
	ਜੇ (!rt_ਸਮੂਹ_sched_enabled())
		ਵਾਪਸ 0;

	scoped_guard(mutex, &rt_constraints_mutex)
		ਵਾਪਸ __rt_schedulable(ਨਿਰਾ, 0, 0);
#ਜੇ_ਅੰਤ
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ sched_rt_handler(ਅਚਲ ਬਣਤਰ ctl_ਸਾਰਣੀ *ਸਾਰਣੀ, ਪੂਰਨ ਲਿਖ, ਖਾਲੀ *ਬਫਰ,
		ਮਾਪ_t *lenp, loff_t *ppos)
{
	ਪੂਰਨ old_period, old_runtime;
	ਸਥਿਰ DEFINE_MUTEX(mutex);
	ਪੂਰਨ ret;

	mutex_ਤਾਲਾ(&mutex);
	sched_domains_mutex_ਤਾਲਾ();
	old_period = sysctl_sched_rt_period;
	old_runtime = sysctl_sched_rt_runtime;

	ret = proc_dointvec_minmax(ਸਾਰਣੀ, ਲਿਖ, ਬਫਰ, lenp, ppos);

	ਜੇ (!ret && ਲਿਖ) {
		ret = sched_rt_global_validate();
		ਜੇ (ret)
			ਜਾਓ undo;

		ret = sched_dl_global_validate();
		ਜੇ (ret)
			ਜਾਓ undo;

		sched_dl_do_global();
	}
	ਜੇ (0) {
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

	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ sched_rr_handler(ਅਚਲ ਬਣਤਰ ctl_ਸਾਰਣੀ *ਸਾਰਣੀ, ਪੂਰਨ ਲਿਖ, ਖਾਲੀ *ਬਫਰ,
		ਮਾਪ_t *lenp, loff_t *ppos)
{
	ਪੂਰਨ ret;
	ਸਥਿਰ DEFINE_MUTEX(mutex);

	mutex_ਤਾਲਾ(&mutex);
	ret = proc_dointvec(ਸਾਰਣੀ, ਲਿਖ, ਬਫਰ, lenp, ppos);
	/*
	 * Make sure that internally we keep jiffies.
	 * Also, writing zero resets the time-slice to default:
	 */
	ਜੇ (!ret && ਲਿਖ) {
		sched_rr_timeslice =
			sysctl_sched_rr_timeslice <= 0 ? RR_TIMESLICE :
			msecs_ਮੰਜ਼ਿਲ_jiffies(sysctl_sched_rr_timeslice);

		ਜੇ (sysctl_sched_rr_timeslice <= 0)
			sysctl_sched_rr_timeslice = jiffies_ਮੰਜ਼ਿਲ_msecs(RR_TIMESLICE);
	}
	mutex_unlock(&mutex);

	ਵਾਪਸ ret;
}
#ਜੇ_ਅੰਤ /* CONFIG_SYSCTL */

ਖਾਲੀ print_rt_stats(ਬਣਤਰ seq_file *m, ਪੂਰਨ cpu)
{
	rt_rq_iter_t iter;
	ਬਣਤਰ rt_rq *rt_rq;

	rcu_read_ਤਾਲਾ();
	for_each_rt_rq(rt_rq, iter, cpu_rq(cpu))
		print_rt_rq(m, cpu, rt_rq);
	rcu_read_unlock();
}
