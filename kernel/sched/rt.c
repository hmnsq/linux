// SPDX-License-Identifier: GPL-2.0
/*
 * Real-Time Scheduling Class (mapped to the SCHED_FIFO and SCHED_RR
 * policies)
 */

#ಅನ್ತರ್ಭಾವ "sched.h"
#ಅನ್ತರ್ಭಾವ "pelt.h"

ಪೂರ್ಣಾಂಕ sched_rr_timeslice = RR_TIMESLICE;
/* More than 4 hours if BW_SHIFT equals 20. */
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ u64 max_rt_runtime = MAX_BW;

/*
 * period over which we measure -rt task CPU usage in us.
 * default: 1s
 */
ಪೂರ್ಣಾಂಕ sysctl_sched_rt_period = 1000000;

/*
 * part of the period that we allow rt tasks to run in us.
 * default: 1s
 */
ಪೂರ್ಣಾಂಕ sysctl_sched_rt_runtime = 1000000;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SYSCTL
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sysctl_sched_rr_timeslice = (MSEC_ಪ್ರತಿ_SEC * RR_TIMESLICE) / HZ;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sched_rt_handler(ಸ್ಥಿರಾಂಕ ರಚನೆ ctl_ಕೋಷ್ಟಕ *ಕೋಷ್ಟಕ, ಪೂರ್ಣಾಂಕ ಲಿಖ, ಶೂನ್ಯ *ಬಫರ್,
		ಗಾತ್ರ_t *lenp, loff_t *ppos);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sched_rr_handler(ಸ್ಥಿರಾಂಕ ರಚನೆ ctl_ಕೋಷ್ಟಕ *ಕೋಷ್ಟಕ, ಪೂರ್ಣಾಂಕ ಲಿಖ, ಶೂನ್ಯ *ಬಫರ್,
		ಗಾತ್ರ_t *lenp, loff_t *ppos);
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ ctl_ಕೋಷ್ಟಕ sched_rt_sysctls[] = {
	{
		.procname       = "sched_rt_period_us",
		.ದತ್ತಾಂಶ           = &sysctl_sched_rt_period,
		.maxlen         = ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ),
		.ಬಹುಲಕ           = 0644,
		.proc_handler   = sched_rt_handler,
		.extra1         = SYSCTL_ONE,
		.extra2         = SYSCTL_INT_MAX,
	},
	{
		.procname       = "sched_rt_runtime_us",
		.ದತ್ತಾಂಶ           = &sysctl_sched_rt_runtime,
		.maxlen         = ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ),
		.ಬಹುಲಕ           = 0644,
		.proc_handler   = sched_rt_handler,
		.extra1         = SYSCTL_NEG_ONE,
		.extra2         = (ಶೂನ್ಯ *)&sysctl_sched_rt_period,
	},
	{
		.procname       = "sched_rr_timeslice_ms",
		.ದತ್ತಾಂಶ           = &sysctl_sched_rr_timeslice,
		.maxlen         = ಗಾತ್ರ(ಪೂರ್ಣಾಂಕ),
		.ಬಹುಲಕ           = 0644,
		.proc_handler   = sched_rr_handler,
	},
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ __init sched_rt_sysctl_init(ಶೂನ್ಯ)
{
	ರಿಜಿಸ್ಟರ್_sysctl_init("kernel", sched_rt_sysctls);
	ಹಿಂತಿರುಗಿ 0;
}
late_initcall(sched_rt_sysctl_init);
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_SYSCTL */

ಶೂನ್ಯ init_rt_rq(ರಚನೆ rt_rq *rt_rq)
{
	ರಚನೆ rt_prio_array *array;
	ಪೂರ್ಣಾಂಕ i;

	array = &rt_rq->ಸಕ್ರಿಯ;
	ಗಾಗಿ (i = 0; i < MAX_RT_PRIO; i++) {
		INIT_LIST_ತಲೆ(array->ಸರತಿ + i);
		__clear_bit(i, array->bitmap);
	}
	/* delimiter for bitsearch: */
	__set_bit(MAX_RT_PRIO, array->bitmap);

	rt_rq->highest_prio.curr = MAX_RT_PRIO-1;
	rt_rq->highest_prio.ಮುಂದಿನ = MAX_RT_PRIO-1;
	rt_rq->overloaded = 0;
	plist_ತಲೆ_init(&rt_rq->pushable_tasks);
	/* We start is dequeued state, because no RT tasks are queued */
	rt_rq->rt_queued = 0;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_RT_ಗುಂಪು_SCHED
	rt_rq->rt_ಸಮಯ = 0;
	rt_rq->rt_throttled = 0;
	rt_rq->rt_runtime = 0;
	raw_spin_ಬೀಗ_init(&rt_rq->rt_runtime_ಬೀಗ);
	rt_rq->tg = &ಬೇರು_task_ಗುಂಪು;
#ಆದರೆ_ಅಂತ್ಯ
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_RT_ಗುಂಪು_SCHED

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ do_sched_rt_period_timer(ರಚನೆ rt_bandwidth *rt_b, ಪೂರ್ಣಾಂಕ overrun);

ಸ್ಥಿರ ಎಣಿಕೆ hrtimer_restart sched_rt_period_timer(ರಚನೆ hrtimer *timer)
{
	ರಚನೆ rt_bandwidth *rt_b =
		container_ದ(timer, ರಚನೆ rt_bandwidth, rt_period_timer);
	ಪೂರ್ಣಾಂಕ idle = 0;
	ಪೂರ್ಣಾಂಕ overrun;

	raw_spin_ಬೀಗ(&rt_b->rt_runtime_ಬೀಗ);
	ಗಾಗಿ (;;) {
		overrun = hrtimer_forward_now(timer, rt_b->rt_period);
		ಆದರೆ (!overrun)
			ನಿಲ್ಲಿಸು;

		raw_spin_unlock(&rt_b->rt_runtime_ಬೀಗ);
		idle = do_sched_rt_period_timer(rt_b, overrun);
		raw_spin_ಬೀಗ(&rt_b->rt_runtime_ಬೀಗ);
	}
	ಆದರೆ (idle)
		rt_b->rt_period_ಸಕ್ರಿಯ = 0;
	raw_spin_unlock(&rt_b->rt_runtime_ಬೀಗ);

	ಹಿಂತಿರುಗಿ idle ? HRTIMER_NORESTART : HRTIMER_RESTART;
}

ಶೂನ್ಯ init_rt_bandwidth(ರಚನೆ rt_bandwidth *rt_b, u64 period, u64 runtime)
{
	rt_b->rt_period = ns_ಗಮ್ಯ_ktime(period);
	rt_b->rt_runtime = runtime;

	raw_spin_ಬೀಗ_init(&rt_b->rt_runtime_ಬೀಗ);

	hrtimer_ಸಿದ್ಧತೆ(&rt_b->rt_period_timer, sched_rt_period_timer, CLOCK_MONOTONIC,
		      HRTIMER_ಬಹುಲಕ_REL_HARD);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ do_ಆರಂಭ_rt_bandwidth(ರಚನೆ rt_bandwidth *rt_b)
{
	raw_spin_ಬೀಗ(&rt_b->rt_runtime_ಬೀಗ);
	ಆದರೆ (!rt_b->rt_period_ಸಕ್ರಿಯ) {
		rt_b->rt_period_ಸಕ್ರಿಯ = 1;
		/*
		 * SCHED_DEADLINE updates the bandwidth, as a run away
		 * RT task with a DL task could hog a CPU. But DL does
		 * not reset the period. If a deadline task was running
		 * without an RT task running, it can cause RT tasks to
		 * throttle when they start up. Kick the timer right away
		 * to update the period.
		 */
		hrtimer_forward_now(&rt_b->rt_period_timer, ns_ಗಮ್ಯ_ktime(0));
		hrtimer_ಆರಂಭ_expires(&rt_b->rt_period_timer,
				      HRTIMER_ಬಹುಲಕ_ABS_PINNED_HARD);
	}
	raw_spin_unlock(&rt_b->rt_runtime_ಬೀಗ);
}

ಸ್ಥಿರ ಶೂನ್ಯ ಆರಂಭ_rt_bandwidth(ರಚನೆ rt_bandwidth *rt_b)
{
	ಆದರೆ (!rt_bandwidth_enabled() || rt_b->rt_runtime == RUNTIME_INF)
		ಹಿಂತಿರುಗಿ;

	do_ಆರಂಭ_rt_bandwidth(rt_b);
}

ಸ್ಥಿರ ಶೂನ್ಯ destroy_rt_bandwidth(ರಚನೆ rt_bandwidth *rt_b)
{
	hrtimer_cancel(&rt_b->rt_period_timer);
}

#ವ್ಯಾಖ್ಯಾನಿಸು rt_entity_is_task(rt_se) (!(rt_se)->my_q)

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ task_struct *rt_task_ದ(ರಚನೆ sched_rt_entity *rt_se)
{
	WARN_ON_ONCE(!rt_entity_is_task(rt_se));

	ಹಿಂತಿರುಗಿ container_ದ(rt_se, ರಚನೆ task_struct, rt);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ rq *rq_ದ_rt_rq(ರಚನೆ rt_rq *rt_rq)
{
	/* Cannot fold with non-CONFIG_RT_GROUP_SCHED version, layout */
	WARN_ON(!rt_ಗುಂಪು_sched_enabled() && rt_rq->tg != &ಬೇರು_task_ಗುಂಪು);
	ಹಿಂತಿರುಗಿ rt_rq->rq;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ rt_rq *rt_rq_ದ_se(ರಚನೆ sched_rt_entity *rt_se)
{
	WARN_ON(!rt_ಗುಂಪು_sched_enabled() && rt_se->rt_rq->tg != &ಬೇರು_task_ಗುಂಪು);
	ಹಿಂತಿರುಗಿ rt_se->rt_rq;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ rq *rq_ದ_rt_se(ರಚನೆ sched_rt_entity *rt_se)
{
	ರಚನೆ rt_rq *rt_rq = rt_se->rt_rq;

	WARN_ON(!rt_ಗುಂಪು_sched_enabled() && rt_rq->tg != &ಬೇರು_task_ಗುಂಪು);
	ಹಿಂತಿರುಗಿ rt_rq->rq;
}

ಶೂನ್ಯ unregister_rt_sched_ಗುಂಪು(ರಚನೆ task_ಗುಂಪು *tg)
{
	ಆದರೆ (!rt_ಗುಂಪು_sched_enabled())
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (tg->rt_se)
		destroy_rt_bandwidth(&tg->rt_bandwidth);
}

ಶೂನ್ಯ free_rt_sched_ಗುಂಪು(ರಚನೆ task_ಗುಂಪು *tg)
{
	ಪೂರ್ಣಾಂಕ i;

	ಆದರೆ (!rt_ಗುಂಪು_sched_enabled())
		ಹಿಂತಿರುಗಿ;

	for_each_possible_cpu(i) {
		ಆದರೆ (tg->rt_rq)
			kfree(tg->rt_rq[i]);
		ಆದರೆ (tg->rt_se)
			kfree(tg->rt_se[i]);
	}

	kfree(tg->rt_rq);
	kfree(tg->rt_se);
}

ಶೂನ್ಯ init_tg_rt_ನಮೂದು(ರಚನೆ task_ಗುಂಪು *tg, ರಚನೆ rt_rq *rt_rq,
		ರಚನೆ sched_rt_entity *rt_se, ಪೂರ್ಣಾಂಕ cpu,
		ರಚನೆ sched_rt_entity *ಪೋಷಕ)
{
	ರಚನೆ rq *rq = cpu_rq(cpu);

	rt_rq->highest_prio.curr = MAX_RT_PRIO-1;
	rt_rq->rt_nr_boosted = 0;
	rt_rq->rq = rq;
	rt_rq->tg = tg;

	tg->rt_rq[cpu] = rt_rq;
	tg->rt_se[cpu] = rt_se;

	ಆದರೆ (!rt_se)
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (!ಪೋಷಕ)
		rt_se->rt_rq = &rq->rt;
	ಇಲ್ಲದಿದ್ದರೆ
		rt_se->rt_rq = ಪೋಷಕ->my_q;

	rt_se->my_q = rt_rq;
	rt_se->ಪೋಷಕ = ಪೋಷಕ;
	INIT_LIST_ತಲೆ(&rt_se->ಸತತ_list);
}

ಪೂರ್ಣಾಂಕ alloc_rt_sched_ಗುಂಪು(ರಚನೆ task_ಗುಂಪು *tg, ರಚನೆ task_ಗುಂಪು *ಪೋಷಕ)
{
	ರಚನೆ rt_rq *rt_rq;
	ರಚನೆ sched_rt_entity *rt_se;
	ಪೂರ್ಣಾಂಕ i;

	ಆದರೆ (!rt_ಗುಂಪು_sched_enabled())
		ಹಿಂತಿರುಗಿ 1;

	tg->rt_rq = kzalloc_objs(rt_rq, nr_cpu_ids);
	ಆದರೆ (!tg->rt_rq)
		ಹೋಗು err;
	tg->rt_se = kzalloc_objs(rt_se, nr_cpu_ids);
	ಆದರೆ (!tg->rt_se)
		ಹೋಗು err;

	init_rt_bandwidth(&tg->rt_bandwidth, ktime_ಗಮ್ಯ_ns(global_rt_period()), 0);

	for_each_possible_cpu(i) {
		rt_rq = kzalloc_ಗಂಟು(ಗಾತ್ರ(ರಚನೆ rt_rq),
				     GFP_KERNEL, cpu_ಗಮ್ಯ_ಗಂಟು(i));
		ಆದರೆ (!rt_rq)
			ಹೋಗು err;

		rt_se = kzalloc_ಗಂಟು(ಗಾತ್ರ(ರಚನೆ sched_rt_entity),
				     GFP_KERNEL, cpu_ಗಮ್ಯ_ಗಂಟು(i));
		ಆದರೆ (!rt_se)
			ಹೋಗು err_free_rq;

		init_rt_rq(rt_rq);
		rt_rq->rt_runtime = tg->rt_bandwidth.rt_runtime;
		init_tg_rt_ನಮೂದು(tg, rt_rq, rt_se, i, ಪೋಷಕ->rt_se[i]);
	}

	ಹಿಂತಿರುಗಿ 1;

err_free_rq:
	kfree(rt_rq);
err:
	ಹಿಂತಿರುಗಿ 0;
}

#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_RT_GROUP_SCHED: */

#ವ್ಯಾಖ್ಯಾನಿಸು rt_entity_is_task(rt_se) (1)

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ task_struct *rt_task_ದ(ರಚನೆ sched_rt_entity *rt_se)
{
	ಹಿಂತಿರುಗಿ container_ದ(rt_se, ರಚನೆ task_struct, rt);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ rq *rq_ದ_rt_rq(ರಚನೆ rt_rq *rt_rq)
{
	ಹಿಂತಿರುಗಿ container_ದ(rt_rq, ರಚನೆ rq, rt);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ rq *rq_ದ_rt_se(ರಚನೆ sched_rt_entity *rt_se)
{
	ರಚನೆ task_struct *p = rt_task_ದ(rt_se);

	ಹಿಂತಿರುಗಿ task_rq(p);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ rt_rq *rt_rq_ದ_se(ರಚನೆ sched_rt_entity *rt_se)
{
	ರಚನೆ rq *rq = rq_ದ_rt_se(rt_se);

	ಹಿಂತಿರುಗಿ &rq->rt;
}

ಶೂನ್ಯ unregister_rt_sched_ಗುಂಪು(ರಚನೆ task_ಗುಂಪು *tg) { }

ಶೂನ್ಯ free_rt_sched_ಗುಂಪು(ರಚನೆ task_ಗುಂಪು *tg) { }

ಪೂರ್ಣಾಂಕ alloc_rt_sched_ಗುಂಪು(ರಚನೆ task_ಗುಂಪು *tg, ರಚನೆ task_ಗುಂಪು *ಪೋಷಕ)
{
	ಹಿಂತಿರುಗಿ 1;
}
#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_RT_GROUP_SCHED */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool need_pull_rt_task(ರಚನೆ rq *rq, ರಚನೆ task_struct *prev)
{
	/* Try to pull RT tasks here if we lower this rq's prio */
	ಹಿಂತಿರುಗಿ rq->online && rq->rt.highest_prio.curr > prev->prio;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ rt_overloaded(ರಚನೆ rq *rq)
{
	ಹಿಂತಿರುಗಿ atomic_read(&rq->rd->rto_ಎಣಿಕೆ);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ rt_ಗಣ_overload(ರಚನೆ rq *rq)
{
	ಆದರೆ (!rq->online)
		ಹಿಂತಿರುಗಿ;

	cpumask_ಗಣ_cpu(rq->cpu, rq->rd->rto_ಮುಖವಾಡ);
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
	atomic_inc(&rq->rd->rto_ಎಣಿಕೆ);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ rt_clear_overload(ರಚನೆ rq *rq)
{
	ಆದರೆ (!rq->online)
		ಹಿಂತಿರುಗಿ;

	/* the order here really doesn't matter */
	atomic_dec(&rq->rd->rto_ಎಣಿಕೆ);
	cpumask_clear_cpu(rq->cpu, rq->rd->rto_ಮುಖವಾಡ);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ has_pushable_tasks(ರಚನೆ rq *rq)
{
	ಹಿಂತಿರುಗಿ !plist_ತಲೆ_ಖಾಲಿಗಳು(&rq->rt.pushable_tasks);
}

ಸ್ಥಿರ DEFINE_ಪ್ರತಿ_CPU(ರಚನೆ balance_callback, rt_ತಳ್ಳು_ತಲೆ);
ಸ್ಥಿರ DEFINE_ಪ್ರತಿ_CPU(ರಚನೆ balance_callback, rt_pull_ತಲೆ);

ಸ್ಥಿರ ಶೂನ್ಯ ತಳ್ಳು_rt_tasks(ರಚನೆ rq *);
ಸ್ಥಿರ ಶೂನ್ಯ pull_rt_task(ರಚನೆ rq *);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ rt_ಸರತಿ_ತಳ್ಳು_tasks(ರಚನೆ rq *rq)
{
	ಆದರೆ (!has_pushable_tasks(rq))
		ಹಿಂತಿರುಗಿ;

	ಸರತಿ_balance_callback(rq, &ಪ್ರತಿ_cpu(rt_ತಳ್ಳು_ತಲೆ, rq->cpu), ತಳ್ಳು_rt_tasks);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ rt_ಸರತಿ_pull_task(ರಚನೆ rq *rq)
{
	ಸರತಿ_balance_callback(rq, &ಪ್ರತಿ_cpu(rt_pull_ತಲೆ, rq->cpu), pull_rt_task);
}

ಸ್ಥಿರ ಶೂನ್ಯ ಸರತಿಗೆ_ಸೇರಿಸು_pushable_task(ರಚನೆ rq *rq, ರಚನೆ task_struct *p)
{
	plist_del(&p->pushable_tasks, &rq->rt.pushable_tasks);
	plist_ಗಂಟು_init(&p->pushable_tasks, p->prio);
	plist_ಕೂಡಿಸು(&p->pushable_tasks, &rq->rt.pushable_tasks);

	/* Update the highest prio pushable task */
	ಆದರೆ (p->prio < rq->rt.highest_prio.ಮುಂದಿನ)
		rq->rt.highest_prio.ಮುಂದಿನ = p->prio;

	ಆದರೆ (!rq->rt.overloaded) {
		rt_ಗಣ_overload(rq);
		rq->rt.overloaded = 1;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ ಸರತಿಯಿಂದ_ತೆಗೆ_pushable_task(ರಚನೆ rq *rq, ರಚನೆ task_struct *p)
{
	plist_del(&p->pushable_tasks, &rq->rt.pushable_tasks);

	/* Update the new highest prio pushable task */
	ಆದರೆ (has_pushable_tasks(rq)) {
		p = plist_ಮೊದಲ_ನಮೂದು(&rq->rt.pushable_tasks,
				      ರಚನೆ task_struct, pushable_tasks);
		rq->rt.highest_prio.ಮುಂದಿನ = p->prio;
	} ಇಲ್ಲದಿದ್ದರೆ {
		rq->rt.highest_prio.ಮುಂದಿನ = MAX_RT_PRIO-1;

		ಆದರೆ (rq->rt.overloaded) {
			rt_clear_overload(rq);
			rq->rt.overloaded = 0;
		}
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ ಸರತಿಗೆ_ಸೇರಿಸು_ತುದಿ_rt_rq(ರಚನೆ rt_rq *rt_rq);
ಸ್ಥಿರ ಶೂನ್ಯ ಸರತಿಯಿಂದ_ತೆಗೆ_ತುದಿ_rt_rq(ರಚನೆ rt_rq *rt_rq, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ on_rt_rq(ರಚನೆ sched_rt_entity *rt_se)
{
	ಹಿಂತಿರುಗಿ rt_se->on_rq;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_UCLAMP_TASK
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
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool rt_task_ಹೊಂದುತ್ತದೆ_ಸಾಮರ್ಥ್ಯ(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ cpu)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ min_cap;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ max_cap;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cpu_cap;

	/* Only heterogeneous systems can benefit from this check */
	ಆದರೆ (!sched_asym_cpucap_ಸಕ್ರಿಯ())
		ಹಿಂತಿರುಗಿ true;

	min_cap = uclamp_eff_ಮೌಲ್ಯ(p, UCLAMP_MIN);
	max_cap = uclamp_eff_ಮೌಲ್ಯ(p, UCLAMP_MAX);

	cpu_cap = arch_scale_cpu_ಸಾಮರ್ಥ್ಯ(cpu);

	ಹಿಂತಿರುಗಿ cpu_cap >= min(min_cap, max_cap);
}
#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_UCLAMP_TASK: */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool rt_task_ಹೊಂದುತ್ತದೆ_ಸಾಮರ್ಥ್ಯ(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ cpu)
{
	ಹಿಂತಿರುಗಿ true;
}
#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_UCLAMP_TASK */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_RT_ಗುಂಪು_SCHED

ಸ್ಥಿರ ಸಾಲೊಳಗೆ u64 sched_rt_runtime(ರಚನೆ rt_rq *rt_rq)
{
	ಹಿಂತಿರುಗಿ rt_rq->rt_runtime;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ u64 sched_rt_period(ರಚನೆ rt_rq *rt_rq)
{
	ಹಿಂತಿರುಗಿ ktime_ಗಮ್ಯ_ns(rt_rq->tg->rt_bandwidth.rt_period);
}

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ task_ಗುಂಪು *rt_rq_iter_t;

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ task_ಗುಂಪು *ಮುಂದಿನ_task_ಗುಂಪು(ರಚನೆ task_ಗುಂಪು *tg)
{
	ಆದರೆ (!rt_ಗುಂಪು_sched_enabled()) {
		WARN_ON(tg != &ಬೇರು_task_ಗುಂಪು);
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;
	}

	ಮಾಡು {
		tg = list_ನಮೂದು_rcu(tg->list.ಮುಂದಿನ,
			typeof(ರಚನೆ task_ಗುಂಪು), list);
	} ಆಗ (&tg->list != &task_groups && task_ಗುಂಪು_is_autogroup(tg));

	ಆದರೆ (&tg->list == &task_groups)
		tg = ಖಾಲಿ;

	ಹಿಂತಿರುಗಿ tg;
}

#ವ್ಯಾಖ್ಯಾನಿಸು for_each_rt_rq(rt_rq, iter, rq)					\
	ಗಾಗಿ (iter = &ಬೇರು_task_ಗುಂಪು;					\
		iter && (rt_rq = iter->rt_rq[cpu_ದ(rq)]);		\
		iter = ಮುಂದಿನ_task_ಗುಂಪು(iter))

#ವ್ಯಾಖ್ಯಾನಿಸು for_each_sched_rt_entity(rt_se) \
	ಗಾಗಿ (; rt_se; rt_se = rt_se->ಪೋಷಕ)

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ rt_rq *ಗುಂಪು_rt_rq(ರಚನೆ sched_rt_entity *rt_se)
{
	ಹಿಂತಿರುಗಿ rt_se->my_q;
}

ಸ್ಥಿರ ಶೂನ್ಯ ಸರತಿಗೆ_ಸೇರಿಸು_rt_entity(ರಚನೆ sched_rt_entity *rt_se, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags);
ಸ್ಥಿರ ಶೂನ್ಯ ಸರತಿಯಿಂದ_ತೆಗೆ_rt_entity(ರಚನೆ sched_rt_entity *rt_se, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags);

ಸ್ಥಿರ ಶೂನ್ಯ sched_rt_rq_ಸರತಿಗೆ_ಸೇರಿಸು(ರಚನೆ rt_rq *rt_rq)
{
	ರಚನೆ task_struct *donor = rq_ದ_rt_rq(rt_rq)->donor;
	ರಚನೆ rq *rq = rq_ದ_rt_rq(rt_rq);
	ರಚನೆ sched_rt_entity *rt_se;

	ಪೂರ್ಣಾಂಕ cpu = cpu_ದ(rq);

	rt_se = rt_rq->tg->rt_se[cpu];

	ಆದರೆ (rt_rq->rt_nr_running) {
		ಆದರೆ (!rt_se)
			ಸರತಿಗೆ_ಸೇರಿಸು_ತುದಿ_rt_rq(rt_rq);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!on_rt_rq(rt_se))
			ಸರತಿಗೆ_ಸೇರಿಸು_rt_entity(rt_se, 0);

		ಆದರೆ (rt_rq->highest_prio.curr < donor->prio)
			resched_curr(rq);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ sched_rt_rq_ಸರತಿಯಿಂದ_ತೆಗೆ(ರಚನೆ rt_rq *rt_rq)
{
	ರಚನೆ sched_rt_entity *rt_se;
	ಪೂರ್ಣಾಂಕ cpu = cpu_ದ(rq_ದ_rt_rq(rt_rq));

	rt_se = rt_rq->tg->rt_se[cpu];

	ಆದರೆ (!rt_se) {
		ಸರತಿಯಿಂದ_ತೆಗೆ_ತುದಿ_rt_rq(rt_rq, rt_rq->rt_nr_running);
		/* Kick cpufreq (see the comment in kernel/sched/sched.h). */
		cpufreq_update_util(rq_ದ_rt_rq(rt_rq), 0);
	}
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (on_rt_rq(rt_se))
		ಸರತಿಯಿಂದ_ತೆಗೆ_rt_entity(rt_se, 0);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ rt_rq_throttled(ರಚನೆ rt_rq *rt_rq)
{
	ಹಿಂತಿರುಗಿ rt_rq->rt_throttled && !rt_rq->rt_nr_boosted;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ rt_se_boosted(ರಚನೆ sched_rt_entity *rt_se)
{
	ರಚನೆ rt_rq *rt_rq = ಗುಂಪು_rt_rq(rt_se);
	ರಚನೆ task_struct *p;

	ಆದರೆ (rt_rq)
		ಹಿಂತಿರುಗಿ !!rt_rq->rt_nr_boosted;

	p = rt_task_ದ(rt_se);
	ಹಿಂತಿರುಗಿ p->prio != p->ಸಾಮಾನ್ಯ_prio;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *sched_rt_period_ಮುಖವಾಡ(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ this_rq()->rd->ಹರವು;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ
ರಚನೆ rt_rq *sched_rt_period_rt_rq(ರಚನೆ rt_bandwidth *rt_b, ಪೂರ್ಣಾಂಕ cpu)
{
	ಹಿಂತಿರುಗಿ container_ದ(rt_b, ರಚನೆ task_ಗುಂಪು, rt_bandwidth)->rt_rq[cpu];
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ rt_bandwidth *sched_rt_bandwidth(ರಚನೆ rt_rq *rt_rq)
{
	ಹಿಂತಿರುಗಿ &rt_rq->tg->rt_bandwidth;
}

bool sched_rt_bandwidth_account(ರಚನೆ rt_rq *rt_rq)
{
	ರಚನೆ rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);

	ಹಿಂತಿರುಗಿ (hrtimer_ಸಕ್ರಿಯ(&rt_b->rt_period_timer) ||
		rt_rq->rt_ಸಮಯ < rt_b->rt_runtime);
}

/*
 * We ran out of runtime, see if we can borrow some from our neighbours.
 */
ಸ್ಥಿರ ಶೂನ್ಯ do_balance_runtime(ರಚನೆ rt_rq *rt_rq)
{
	ರಚನೆ rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);
	ರಚನೆ ಬೇರು_domain *rd = rq_ದ_rt_rq(rt_rq)->rd;
	ಪೂರ್ಣಾಂಕ i, ತೂಕ;
	u64 rt_period;

	ತೂಕ = cpumask_ತೂಕ(rd->ಹರವು);

	raw_spin_ಬೀಗ(&rt_b->rt_runtime_ಬೀಗ);
	rt_period = ktime_ಗಮ್ಯ_ns(rt_b->rt_period);
	for_each_cpu(i, rd->ಹರವು) {
		ರಚನೆ rt_rq *iter = sched_rt_period_rt_rq(rt_b, i);
		s64 diff;

		ಆದರೆ (iter == rt_rq)
			ಮುಂದುವರಿ;

		raw_spin_ಬೀಗ(&iter->rt_runtime_ಬೀಗ);
		/*
		 * Either all rqs have inf runtime and there's nothing to steal
		 * or __disable_runtime() below sets a specific rq to inf to
		 * indicate its been disabled and disallow stealing.
		 */
		ಆದರೆ (iter->rt_runtime == RUNTIME_INF)
			ಹೋಗು ಮುಂದಿನ;

		/*
		 * From runqueues with spare time, take 1/n part of their
		 * spare time, but no more than our period.
		 */
		diff = iter->rt_runtime - iter->rt_ಸಮಯ;
		ಆದರೆ (diff > 0) {
			diff = div_u64((u64)diff, ತೂಕ);
			ಆದರೆ (rt_rq->rt_runtime + diff > rt_period)
				diff = rt_period - rt_rq->rt_runtime;
			iter->rt_runtime -= diff;
			rt_rq->rt_runtime += diff;
			ಆದರೆ (rt_rq->rt_runtime == rt_period) {
				raw_spin_unlock(&iter->rt_runtime_ಬೀಗ);
				ನಿಲ್ಲಿಸು;
			}
		}
ಮುಂದಿನ:
		raw_spin_unlock(&iter->rt_runtime_ಬೀಗ);
	}
	raw_spin_unlock(&rt_b->rt_runtime_ಬೀಗ);
}

/*
 * Ensure this RQ takes back all the runtime it lend to its neighbours.
 */
ಸ್ಥಿರ ಶೂನ್ಯ __disable_runtime(ರಚನೆ rq *rq)
{
	ರಚನೆ ಬೇರು_domain *rd = rq->rd;
	rt_rq_iter_t iter;
	ರಚನೆ rt_rq *rt_rq;

	ಆದರೆ (unlikely(!scheduler_running))
		ಹಿಂತಿರುಗಿ;

	for_each_rt_rq(rt_rq, iter, rq) {
		ರಚನೆ rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);
		s64 want;
		ಪೂರ್ಣಾಂಕ i;

		raw_spin_ಬೀಗ(&rt_b->rt_runtime_ಬೀಗ);
		raw_spin_ಬೀಗ(&rt_rq->rt_runtime_ಬೀಗ);
		/*
		 * Either we're all inf and nobody needs to borrow, or we're
		 * already disabled and thus have nothing to do, or we have
		 * exactly the right amount of runtime to take out.
		 */
		ಆದರೆ (rt_rq->rt_runtime == RUNTIME_INF ||
				rt_rq->rt_runtime == rt_b->rt_runtime)
			ಹೋಗು ಸಮತೋಲಿತ;
		raw_spin_unlock(&rt_rq->rt_runtime_ಬೀಗ);

		/*
		 * Calculate the difference between what we started out with
		 * and what we current have, that's the amount of runtime
		 * we lend and now have to reclaim.
		 */
		want = rt_b->rt_runtime - rt_rq->rt_runtime;

		/*
		 * Greedy reclaim, take back as much as we can.
		 */
		for_each_cpu(i, rd->ಹರವು) {
			ರಚನೆ rt_rq *iter = sched_rt_period_rt_rq(rt_b, i);
			s64 diff;

			/*
			 * Can't reclaim from ourselves or disabled runqueues.
			 */
			ಆದರೆ (iter == rt_rq || iter->rt_runtime == RUNTIME_INF)
				ಮುಂದುವರಿ;

			raw_spin_ಬೀಗ(&iter->rt_runtime_ಬೀಗ);
			ಆದರೆ (want > 0) {
				diff = min_t(s64, iter->rt_runtime, want);
				iter->rt_runtime -= diff;
				want -= diff;
			} ಇಲ್ಲದಿದ್ದರೆ {
				iter->rt_runtime -= want;
				want -= want;
			}
			raw_spin_unlock(&iter->rt_runtime_ಬೀಗ);

			ಆದರೆ (!want)
				ನಿಲ್ಲಿಸು;
		}

		raw_spin_ಬೀಗ(&rt_rq->rt_runtime_ಬೀಗ);
		/*
		 * We cannot be left wanting - that would mean some runtime
		 * leaked out of the system.
		 */
		WARN_ON_ONCE(want);
ಸಮತೋಲಿತ:
		/*
		 * Disable all the borrow logic by pretending we have inf
		 * runtime - in which case borrowing doesn't make sense.
		 */
		rt_rq->rt_runtime = RUNTIME_INF;
		rt_rq->rt_throttled = 0;
		raw_spin_unlock(&rt_rq->rt_runtime_ಬೀಗ);
		raw_spin_unlock(&rt_b->rt_runtime_ಬೀಗ);

		/* Make rt_rq available for pick_next_task() */
		sched_rt_rq_ಸರತಿಗೆ_ಸೇರಿಸು(rt_rq);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ __enable_runtime(ರಚನೆ rq *rq)
{
	rt_rq_iter_t iter;
	ರಚನೆ rt_rq *rt_rq;

	ಆದರೆ (unlikely(!scheduler_running))
		ಹಿಂತಿರುಗಿ;

	/*
	 * Reset each runqueue's bandwidth settings
	 */
	for_each_rt_rq(rt_rq, iter, rq) {
		ರಚನೆ rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);

		raw_spin_ಬೀಗ(&rt_b->rt_runtime_ಬೀಗ);
		raw_spin_ಬೀಗ(&rt_rq->rt_runtime_ಬೀಗ);
		rt_rq->rt_runtime = rt_b->rt_runtime;
		rt_rq->rt_ಸಮಯ = 0;
		rt_rq->rt_throttled = 0;
		raw_spin_unlock(&rt_rq->rt_runtime_ಬೀಗ);
		raw_spin_unlock(&rt_b->rt_runtime_ಬೀಗ);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ balance_runtime(ರಚನೆ rt_rq *rt_rq)
{
	ಆದರೆ (!sched_feat(RT_RUNTIME_SHARE))
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (rt_rq->rt_ಸಮಯ > rt_rq->rt_runtime) {
		raw_spin_unlock(&rt_rq->rt_runtime_ಬೀಗ);
		do_balance_runtime(rt_rq);
		raw_spin_ಬೀಗ(&rt_rq->rt_runtime_ಬೀಗ);
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ do_sched_rt_period_timer(ರಚನೆ rt_bandwidth *rt_b, ಪೂರ್ಣಾಂಕ overrun)
{
	ಪೂರ್ಣಾಂಕ i, idle = 1, throttled = 0;
	ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *ಹರವು;

	ಹರವು = sched_rt_period_ಮುಖವಾಡ();

	/*
	 * FIXME: isolated CPUs should really leave the root task group,
	 * whether they are isolcpus or were isolated via cpusets, lest
	 * the timer run on a CPU which does not service all runqueues,
	 * potentially leaving other CPUs indefinitely throttled.  If
	 * isolation is really required, the user will turn the throttle
	 * off to kill the perturbations it causes anyway.  Meanwhile,
	 * this maintains functionality for boot and/or troubleshooting.
	 */
	ಆದರೆ (rt_b == &ಬೇರು_task_ಗುಂಪು.rt_bandwidth)
		ಹರವು = cpu_online_ಮುಖವಾಡ;

	for_each_cpu(i, ಹರವು) {
		ಪೂರ್ಣಾಂಕ ಸರತಿಗೆ_ಸೇರಿಸು = 0;
		ರಚನೆ rt_rq *rt_rq = sched_rt_period_rt_rq(rt_b, i);
		ರಚನೆ rq *rq = rq_ದ_rt_rq(rt_rq);
		ರಚನೆ rq_flags rf;
		ಪೂರ್ಣಾಂಕ skip;

		/*
		 * When span == cpu_online_mask, taking each rq->lock
		 * can be time-consuming. Try to avoid it when possible.
		 */
		raw_spin_ಬೀಗ(&rt_rq->rt_runtime_ಬೀಗ);
		ಆದರೆ (!sched_feat(RT_RUNTIME_SHARE) && rt_rq->rt_runtime != RUNTIME_INF)
			rt_rq->rt_runtime = rt_b->rt_runtime;
		skip = !rt_rq->rt_ಸಮಯ && !rt_rq->rt_nr_running;
		raw_spin_unlock(&rt_rq->rt_runtime_ಬೀಗ);
		ಆದರೆ (skip)
			ಮುಂದುವರಿ;

		rq_ಬೀಗ(rq, &rf);
		update_rq_clock(rq);

		ಆದರೆ (rt_rq->rt_ಸಮಯ) {
			u64 runtime;

			raw_spin_ಬೀಗ(&rt_rq->rt_runtime_ಬೀಗ);
			ಆದರೆ (rt_rq->rt_throttled)
				balance_runtime(rt_rq);
			runtime = rt_rq->rt_runtime;
			rt_rq->rt_ಸಮಯ -= min(rt_rq->rt_ಸಮಯ, overrun*runtime);
			ಆದರೆ (rt_rq->rt_throttled && rt_rq->rt_ಸಮಯ < runtime) {
				rt_rq->rt_throttled = 0;
				ಸರತಿಗೆ_ಸೇರಿಸು = 1;

				/*
				 * When we're idle and a woken (rt) task is
				 * throttled wakeup_preempt() will set
				 * skip_update and the time between the wakeup
				 * and this unthrottle will get accounted as
				 * 'runtime'.
				 */
				ಆದರೆ (rt_rq->rt_nr_running && rq->curr == rq->idle)
					rq_clock_cancel_skipupdate(rq);
			}
			ಆದರೆ (rt_rq->rt_ಸಮಯ || rt_rq->rt_nr_running)
				idle = 0;
			raw_spin_unlock(&rt_rq->rt_runtime_ಬೀಗ);
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (rt_rq->rt_nr_running) {
			idle = 0;
			ಆದರೆ (!rt_rq_throttled(rt_rq))
				ಸರತಿಗೆ_ಸೇರಿಸು = 1;
		}
		ಆದರೆ (rt_rq->rt_throttled)
			throttled = 1;

		ಆದರೆ (ಸರತಿಗೆ_ಸೇರಿಸು)
			sched_rt_rq_ಸರತಿಗೆ_ಸೇರಿಸು(rt_rq);
		rq_unlock(rq, &rf);
	}

	ಆದರೆ (!throttled && (!rt_bandwidth_enabled() || rt_b->rt_runtime == RUNTIME_INF))
		ಹಿಂತಿರುಗಿ 1;

	ಹಿಂತಿರುಗಿ idle;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sched_rt_runtime_exceeded(ರಚನೆ rt_rq *rt_rq)
{
	u64 runtime = sched_rt_runtime(rt_rq);

	ಆದರೆ (rt_rq->rt_throttled)
		ಹಿಂತಿರುಗಿ rt_rq_throttled(rt_rq);

	ಆದರೆ (runtime >= sched_rt_period(rt_rq))
		ಹಿಂತಿರುಗಿ 0;

	balance_runtime(rt_rq);
	runtime = sched_rt_runtime(rt_rq);
	ಆದರೆ (runtime == RUNTIME_INF)
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (rt_rq->rt_ಸಮಯ > runtime) {
		ರಚನೆ rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);

		/*
		 * Don't actually throttle groups that have no runtime assigned
		 * but accrue some time due to boosting.
		 */
		ಆದರೆ (likely(rt_b->rt_runtime)) {
			rt_rq->rt_throttled = 1;
			printk_deferred_once("sched: RT throttling activated\n");
		} ಇಲ್ಲದಿದ್ದರೆ {
			/*
			 * In case we did anyway, make it go away,
			 * replenishment is a joke, since it will replenish us
			 * with exactly 0 ns.
			 */
			rt_rq->rt_ಸಮಯ = 0;
		}

		ಆದರೆ (rt_rq_throttled(rt_rq)) {
			sched_rt_rq_ಸರತಿಯಿಂದ_ತೆಗೆ(rt_rq);
			ಹಿಂತಿರುಗಿ 1;
		}
	}

	ಹಿಂತಿರುಗಿ 0;
}

#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_RT_GROUP_SCHED: */

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ rt_rq *rt_rq_iter_t;

#ವ್ಯಾಖ್ಯಾನಿಸು for_each_rt_rq(rt_rq, iter, rq) \
	ಗಾಗಿ ((ಶೂನ್ಯ) iter, rt_rq = &rq->rt; rt_rq; rt_rq = ಖಾಲಿ)

#ವ್ಯಾಖ್ಯಾನಿಸು for_each_sched_rt_entity(rt_se) \
	ಗಾಗಿ (; rt_se; rt_se = ಖಾಲಿ)

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ rt_rq *ಗುಂಪು_rt_rq(ರಚನೆ sched_rt_entity *rt_se)
{
	ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ sched_rt_rq_ಸರತಿಗೆ_ಸೇರಿಸು(ರಚನೆ rt_rq *rt_rq)
{
	ರಚನೆ rq *rq = rq_ದ_rt_rq(rt_rq);

	ಆದರೆ (!rt_rq->rt_nr_running)
		ಹಿಂತಿರುಗಿ;

	ಸರತಿಗೆ_ಸೇರಿಸು_ತುದಿ_rt_rq(rt_rq);
	resched_curr(rq);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ sched_rt_rq_ಸರತಿಯಿಂದ_ತೆಗೆ(ರಚನೆ rt_rq *rt_rq)
{
	ಸರತಿಯಿಂದ_ತೆಗೆ_ತುದಿ_rt_rq(rt_rq, rt_rq->rt_nr_running);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ rt_rq_throttled(ರಚನೆ rt_rq *rt_rq)
{
	ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *sched_rt_period_ಮುಖವಾಡ(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ cpu_online_ಮುಖವಾಡ;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ
ರಚನೆ rt_rq *sched_rt_period_rt_rq(ರಚನೆ rt_bandwidth *rt_b, ಪೂರ್ಣಾಂಕ cpu)
{
	ಹಿಂತಿರುಗಿ &cpu_rq(cpu)->rt;
}

ಸ್ಥಿರ ಶೂನ್ಯ __enable_runtime(ರಚನೆ rq *rq) { }
ಸ್ಥಿರ ಶೂನ್ಯ __disable_runtime(ರಚನೆ rq *rq) { }

#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_RT_GROUP_SCHED */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ rt_se_prio(ರಚನೆ sched_rt_entity *rt_se)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_RT_ಗುಂಪು_SCHED
	ರಚನೆ rt_rq *rt_rq = ಗುಂಪು_rt_rq(rt_se);

	ಆದರೆ (rt_rq)
		ಹಿಂತಿರುಗಿ rt_rq->highest_prio.curr;
#ಆದರೆ_ಅಂತ್ಯ

	ಹಿಂತಿರುಗಿ rt_task_ದ(rt_se)->prio;
}

/*
 * Update the current task's runtime statistics. Skip current tasks that
 * are not in our scheduling class.
 */
ಸ್ಥಿರ ಶೂನ್ಯ update_curr_rt(ರಚನೆ rq *rq)
{
	ರಚನೆ task_struct *donor = rq->donor;
	s64 delta_exec;

	ಆದರೆ (donor->sched_class != &rt_sched_class)
		ಹಿಂತಿರುಗಿ;

	delta_exec = update_curr_common(rq);
	ಆದರೆ (unlikely(delta_exec <= 0))
		ಹಿಂತಿರುಗಿ;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_RT_ಗುಂಪು_SCHED
	ರಚನೆ sched_rt_entity *rt_se = &donor->rt;

	ಆದರೆ (!rt_bandwidth_enabled())
		ಹಿಂತಿರುಗಿ;

	for_each_sched_rt_entity(rt_se) {
		ರಚನೆ rt_rq *rt_rq = rt_rq_ದ_se(rt_se);
		ಪೂರ್ಣಾಂಕ exceeded;

		ಆದರೆ (sched_rt_runtime(rt_rq) != RUNTIME_INF) {
			raw_spin_ಬೀಗ(&rt_rq->rt_runtime_ಬೀಗ);
			rt_rq->rt_ಸಮಯ += delta_exec;
			exceeded = sched_rt_runtime_exceeded(rt_rq);
			ಆದರೆ (exceeded)
				resched_curr(rq);
			raw_spin_unlock(&rt_rq->rt_runtime_ಬೀಗ);
			ಆದರೆ (exceeded)
				do_ಆರಂಭ_rt_bandwidth(sched_rt_bandwidth(rt_rq));
		}
	}
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_RT_GROUP_SCHED */
}

ಸ್ಥಿರ ಶೂನ್ಯ
ಸರತಿಯಿಂದ_ತೆಗೆ_ತುದಿ_rt_rq(ರಚನೆ rt_rq *rt_rq, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2)
{
	ರಚನೆ rq *rq = rq_ದ_rt_rq(rt_rq);

	BUG_ON(&rq->rt != rt_rq);

	ಆದರೆ (!rt_rq->rt_queued)
		ಹಿಂತಿರುಗಿ;

	BUG_ON(!rq->nr_running);

	sub_nr_running(rq, ಎಣಿಕೆ_2);
	rt_rq->rt_queued = 0;

}

ಸ್ಥಿರ ಶೂನ್ಯ
ಸರತಿಗೆ_ಸೇರಿಸು_ತುದಿ_rt_rq(ರಚನೆ rt_rq *rt_rq)
{
	ರಚನೆ rq *rq = rq_ದ_rt_rq(rt_rq);

	BUG_ON(&rq->rt != rt_rq);

	ಆದರೆ (rt_rq->rt_queued)
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (rt_rq_throttled(rt_rq))
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (rt_rq->rt_nr_running) {
		ಕೂಡಿಸು_nr_running(rq, rt_rq->rt_nr_running);
		rt_rq->rt_queued = 1;
	}

	/* Kick cpufreq (see the comment in kernel/sched/sched.h). */
	cpufreq_update_util(rq, 0);
}

ಸ್ಥಿರ ಶೂನ್ಯ
inc_rt_prio_smp(ರಚನೆ rt_rq *rt_rq, ಪೂರ್ಣಾಂಕ prio, ಪೂರ್ಣಾಂಕ prev_prio)
{
	ರಚನೆ rq *rq = rq_ದ_rt_rq(rt_rq);

	/*
	 * Change rq's cpupri only if rt_rq is the top queue.
	 */
	ಆದರೆ (IS_ENABLED(CONFIG_RT_ಗುಂಪು_SCHED) && &rq->rt != rt_rq)
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (rq->online && prio < prev_prio)
		cpupri_ಗಣ(&rq->rd->cpupri, rq->cpu, prio);
}

ಸ್ಥಿರ ಶೂನ್ಯ
dec_rt_prio_smp(ರಚನೆ rt_rq *rt_rq, ಪೂರ್ಣಾಂಕ prio, ಪೂರ್ಣಾಂಕ prev_prio)
{
	ರಚನೆ rq *rq = rq_ದ_rt_rq(rt_rq);

	/*
	 * Change rq's cpupri only if rt_rq is the top queue.
	 */
	ಆದರೆ (IS_ENABLED(CONFIG_RT_ಗುಂಪು_SCHED) && &rq->rt != rt_rq)
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (rq->online && rt_rq->highest_prio.curr != prev_prio)
		cpupri_ಗಣ(&rq->rd->cpupri, rq->cpu, rt_rq->highest_prio.curr);
}

ಸ್ಥಿರ ಶೂನ್ಯ
inc_rt_prio(ರಚನೆ rt_rq *rt_rq, ಪೂರ್ಣಾಂಕ prio)
{
	ಪೂರ್ಣಾಂಕ prev_prio = rt_rq->highest_prio.curr;

	ಆದರೆ (prio < prev_prio)
		rt_rq->highest_prio.curr = prio;

	inc_rt_prio_smp(rt_rq, prio, prev_prio);
}

ಸ್ಥಿರ ಶೂನ್ಯ
dec_rt_prio(ರಚನೆ rt_rq *rt_rq, ಪೂರ್ಣಾಂಕ prio)
{
	ಪೂರ್ಣಾಂಕ prev_prio = rt_rq->highest_prio.curr;

	ಆದರೆ (rt_rq->rt_nr_running) {

		WARN_ON(prio < prev_prio);

		/*
		 * This may have been our highest task, and therefore
		 * we may have some re-computation to do
		 */
		ಆದರೆ (prio == prev_prio) {
			ರಚನೆ rt_prio_array *array = &rt_rq->ಸಕ್ರಿಯ;

			rt_rq->highest_prio.curr =
				sched_ಕಂಡುಹಿಡಿ_ಮೊದಲ_ಬಿಟ್(array->bitmap);
		}

	} ಇಲ್ಲದಿದ್ದರೆ {
		rt_rq->highest_prio.curr = MAX_RT_PRIO-1;
	}

	dec_rt_prio_smp(rt_rq, prio, prev_prio);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_RT_ಗುಂಪು_SCHED

ಸ್ಥಿರ ಶೂನ್ಯ
inc_rt_ಗುಂಪು(ರಚನೆ sched_rt_entity *rt_se, ರಚನೆ rt_rq *rt_rq)
{
	ಆದರೆ (rt_se_boosted(rt_se))
		rt_rq->rt_nr_boosted++;

	ಆರಂಭ_rt_bandwidth(&rt_rq->tg->rt_bandwidth);
}

ಸ್ಥಿರ ಶೂನ್ಯ
dec_rt_ಗುಂಪು(ರಚನೆ sched_rt_entity *rt_se, ರಚನೆ rt_rq *rt_rq)
{
	ಆದರೆ (rt_se_boosted(rt_se))
		rt_rq->rt_nr_boosted--;

	WARN_ON(!rt_rq->rt_nr_running && rt_rq->rt_nr_boosted);
}

#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_RT_GROUP_SCHED: */

ಸ್ಥಿರ ಶೂನ್ಯ
inc_rt_ಗುಂಪು(ರಚನೆ sched_rt_entity *rt_se, ರಚನೆ rt_rq *rt_rq)
{
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ
ಶೂನ್ಯ dec_rt_ಗುಂಪು(ರಚನೆ sched_rt_entity *rt_se, ರಚನೆ rt_rq *rt_rq) {}

#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_RT_GROUP_SCHED */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ
ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ rt_se_nr_running(ರಚನೆ sched_rt_entity *rt_se)
{
	ರಚನೆ rt_rq *ಗುಂಪು_rq = ಗುಂಪು_rt_rq(rt_se);

	ಆದರೆ (ಗುಂಪು_rq)
		ಹಿಂತಿರುಗಿ ಗುಂಪು_rq->rt_nr_running;
	ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ 1;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ
ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ rt_se_rr_nr_running(ರಚನೆ sched_rt_entity *rt_se)
{
	ರಚನೆ rt_rq *ಗುಂಪು_rq = ಗುಂಪು_rt_rq(rt_se);
	ರಚನೆ task_struct *tsk;

	ಆದರೆ (ಗುಂಪು_rq)
		ಹಿಂತಿರುಗಿ ಗುಂಪು_rq->rr_nr_running;

	tsk = rt_task_ದ(rt_se);

	ಹಿಂತಿರುಗಿ (tsk->policy == SCHED_RR) ? 1 : 0;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ
ಶೂನ್ಯ inc_rt_tasks(ರಚನೆ sched_rt_entity *rt_se, ರಚನೆ rt_rq *rt_rq)
{
	ಪೂರ್ಣಾಂಕ prio = rt_se_prio(rt_se);

	WARN_ON(!rt_prio(prio));
	rt_rq->rt_nr_running += rt_se_nr_running(rt_se);
	rt_rq->rr_nr_running += rt_se_rr_nr_running(rt_se);

	inc_rt_prio(rt_rq, prio);
	inc_rt_ಗುಂಪು(rt_se, rt_rq);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ
ಶೂನ್ಯ dec_rt_tasks(ರಚನೆ sched_rt_entity *rt_se, ರಚನೆ rt_rq *rt_rq)
{
	WARN_ON(!rt_prio(rt_se_prio(rt_se)));
	WARN_ON(!rt_rq->rt_nr_running);
	rt_rq->rt_nr_running -= rt_se_nr_running(rt_se);
	rt_rq->rr_nr_running -= rt_se_rr_nr_running(rt_se);

	dec_rt_prio(rt_rq, rt_se_prio(rt_se));
	dec_rt_ಗುಂಪು(rt_se, rt_rq);
}

/*
 * Change rt_se->run_list location unless SAVE && !MOVE
 *
 * assumes ENQUEUE/DEQUEUE flags match
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool move_entity(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags)
{
	ಆದರೆ ((flags & (ಸರತಿಯಿಂದ_ತೆಗೆ_SAVE | ಸರತಿಯಿಂದ_ತೆಗೆ_MOVE)) == ಸರತಿಯಿಂದ_ತೆಗೆ_SAVE)
		ಹಿಂತಿರುಗಿ false;

	ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ ಶೂನ್ಯ __delist_rt_entity(ರಚನೆ sched_rt_entity *rt_se, ರಚನೆ rt_prio_array *array)
{
	list_del_init(&rt_se->ಸತತ_list);

	ಆದರೆ (list_ಖಾಲಿಗಳು(array->ಸರತಿ + rt_se_prio(rt_se)))
		__clear_bit(rt_se_prio(rt_se), array->bitmap);

	rt_se->on_list = 0;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ sched_statistics *
__schedstats_from_rt_se(ರಚನೆ sched_rt_entity *rt_se)
{
	/* schedstats is not supported for rt group. */
	ಆದರೆ (!rt_entity_is_task(rt_se))
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	ಹಿಂತಿರುಗಿ &rt_task_ದ(rt_se)->stats;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
update_stats_wait_ಆರಂಭ_rt(ರಚನೆ rt_rq *rt_rq, ರಚನೆ sched_rt_entity *rt_se)
{
	ರಚನೆ sched_statistics *stats;
	ರಚನೆ task_struct *p = ಖಾಲಿ;

	ಆದರೆ (!schedstat_enabled())
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (rt_entity_is_task(rt_se))
		p = rt_task_ದ(rt_se);

	stats = __schedstats_from_rt_se(rt_se);
	ಆದರೆ (!stats)
		ಹಿಂತಿರುಗಿ;

	__update_stats_wait_start(rq_ದ_rt_rq(rt_rq), p, stats);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
update_stats_ಸರತಿಗೆ_ಸೇರಿಸು_sleeper_rt(ರಚನೆ rt_rq *rt_rq, ರಚನೆ sched_rt_entity *rt_se)
{
	ರಚನೆ sched_statistics *stats;
	ರಚನೆ task_struct *p = ಖಾಲಿ;

	ಆದರೆ (!schedstat_enabled())
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (rt_entity_is_task(rt_se))
		p = rt_task_ದ(rt_se);

	stats = __schedstats_from_rt_se(rt_se);
	ಆದರೆ (!stats)
		ಹಿಂತಿರುಗಿ;

	__update_stats_enqueue_sleeper(rq_ದ_rt_rq(rt_rq), p, stats);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
update_stats_ಸರತಿಗೆ_ಸೇರಿಸು_rt(ರಚನೆ rt_rq *rt_rq, ರಚನೆ sched_rt_entity *rt_se,
			ಪೂರ್ಣಾಂಕ flags)
{
	ಆದರೆ (!schedstat_enabled())
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (flags & ಸರತಿಗೆ_ಸೇರಿಸು_WAKEUP)
		update_stats_ಸರತಿಗೆ_ಸೇರಿಸು_sleeper_rt(rt_rq, rt_se);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
update_stats_wait_ಕೊನೆ_rt(ರಚನೆ rt_rq *rt_rq, ರಚನೆ sched_rt_entity *rt_se)
{
	ರಚನೆ sched_statistics *stats;
	ರಚನೆ task_struct *p = ಖಾಲಿ;

	ಆದರೆ (!schedstat_enabled())
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (rt_entity_is_task(rt_se))
		p = rt_task_ದ(rt_se);

	stats = __schedstats_from_rt_se(rt_se);
	ಆದರೆ (!stats)
		ಹಿಂತಿರುಗಿ;

	__update_stats_wait_end(rq_ದ_rt_rq(rt_rq), p, stats);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
update_stats_ಸರತಿಯಿಂದ_ತೆಗೆ_rt(ರಚನೆ rt_rq *rt_rq, ರಚನೆ sched_rt_entity *rt_se,
			ಪೂರ್ಣಾಂಕ flags)
{
	ರಚನೆ task_struct *p = ಖಾಲಿ;
	ರಚನೆ rq *rq = rq_ದ_rt_rq(rt_rq);

	ಆದರೆ (!schedstat_enabled())
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (rt_entity_is_task(rt_se)) {
		p = rt_task_ದ(rt_se);

		ಆದರೆ (p != rq->curr)
			update_stats_wait_ಕೊನೆ_rt(rt_rq, rt_se);
	}

	ಆದರೆ ((flags & ಸರತಿಯಿಂದ_ತೆಗೆ_SLEEP) && p) {
		ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ state;

		state = READ_ONCE(p->__state);
		ಆದರೆ (state & TASK_INTERRUPTIBLE)
			__schedstat_set(p->stats.sleep_ಆರಂಭ,
					rq_clock(rq_ದ_rt_rq(rt_rq)));

		ಆದರೆ (state & TASK_UNINTERRUPTIBLE)
			__schedstat_set(p->stats.ಖಂಡ_ಆರಂಭ,
					rq_clock(rq_ದ_rt_rq(rt_rq)));
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ __enqueue_rt_entity(ರಚನೆ sched_rt_entity *rt_se, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags)
{
	ರಚನೆ rt_rq *rt_rq = rt_rq_ದ_se(rt_se);
	ರಚನೆ rt_prio_array *array = &rt_rq->ಸಕ್ರಿಯ;
	ರಚನೆ rt_rq *ಗುಂಪು_rq = ಗುಂಪು_rt_rq(rt_se);
	ರಚನೆ list_ತಲೆ *ಸರತಿ = array->ಸರತಿ + rt_se_prio(rt_se);

	/*
	 * Don't enqueue the group if its throttled, or when empty.
	 * The latter is a consequence of the former when a child group
	 * get throttled and the current group doesn't have any other
	 * active members.
	 */
	ಆದರೆ (ಗುಂಪು_rq && (rt_rq_throttled(ಗುಂಪು_rq) || !ಗುಂಪು_rq->rt_nr_running)) {
		ಆದರೆ (rt_se->on_list)
			__delist_rt_entity(rt_se, array);
		ಹಿಂತಿರುಗಿ;
	}

	ಆದರೆ (move_entity(flags)) {
		WARN_ON_ONCE(rt_se->on_list);
		ಆದರೆ (flags & ಸರತಿಗೆ_ಸೇರಿಸು_ತಲೆ)
			list_ಕೂಡಿಸು(&rt_se->ಸತತ_list, ಸರತಿ);
		ಇಲ್ಲದಿದ್ದರೆ
			list_ಕೂಡಿಸು_ಬಾಲ(&rt_se->ಸತತ_list, ಸರತಿ);

		__set_bit(rt_se_prio(rt_se), array->bitmap);
		rt_se->on_list = 1;
	}
	rt_se->on_rq = 1;

	inc_rt_tasks(rt_se, rt_rq);
}

ಸ್ಥಿರ ಶೂನ್ಯ __dequeue_rt_entity(ರಚನೆ sched_rt_entity *rt_se, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags)
{
	ರಚನೆ rt_rq *rt_rq = rt_rq_ದ_se(rt_se);
	ರಚನೆ rt_prio_array *array = &rt_rq->ಸಕ್ರಿಯ;

	ಆದರೆ (move_entity(flags)) {
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
ಸ್ಥಿರ ಶೂನ್ಯ ಸರತಿಯಿಂದ_ತೆಗೆ_rt_ರಾಶಿ(ರಚನೆ sched_rt_entity *rt_se, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags)
{
	ರಚನೆ sched_rt_entity *ಹಿಂಭಾಗ = ಖಾಲಿ;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ rt_nr_running;

	for_each_sched_rt_entity(rt_se) {
		rt_se->ಹಿಂಭಾಗ = ಹಿಂಭಾಗ;
		ಹಿಂಭಾಗ = rt_se;
	}

	rt_nr_running = rt_rq_ದ_se(ಹಿಂಭಾಗ)->rt_nr_running;

	ಗಾಗಿ (rt_se = ಹಿಂಭಾಗ; rt_se; rt_se = rt_se->ಹಿಂಭಾಗ) {
		ಆದರೆ (on_rt_rq(rt_se))
			__dequeue_rt_entity(rt_se, flags);
	}

	ಸರತಿಯಿಂದ_ತೆಗೆ_ತುದಿ_rt_rq(rt_rq_ದ_se(ಹಿಂಭಾಗ), rt_nr_running);
}

ಸ್ಥಿರ ಶೂನ್ಯ ಸರತಿಗೆ_ಸೇರಿಸು_rt_entity(ರಚನೆ sched_rt_entity *rt_se, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags)
{
	ರಚನೆ rq *rq = rq_ದ_rt_se(rt_se);

	update_stats_ಸರತಿಗೆ_ಸೇರಿಸು_rt(rt_rq_ದ_se(rt_se), rt_se, flags);

	ಸರತಿಯಿಂದ_ತೆಗೆ_rt_ರಾಶಿ(rt_se, flags);
	for_each_sched_rt_entity(rt_se)
		__enqueue_rt_entity(rt_se, flags);
	ಸರತಿಗೆ_ಸೇರಿಸು_ತುದಿ_rt_rq(&rq->rt);
}

ಸ್ಥಿರ ಶೂನ್ಯ ಸರತಿಯಿಂದ_ತೆಗೆ_rt_entity(ರಚನೆ sched_rt_entity *rt_se, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags)
{
	ರಚನೆ rq *rq = rq_ದ_rt_se(rt_se);

	update_stats_ಸರತಿಯಿಂದ_ತೆಗೆ_rt(rt_rq_ದ_se(rt_se), rt_se, flags);

	ಸರತಿಯಿಂದ_ತೆಗೆ_rt_ರಾಶಿ(rt_se, flags);

	for_each_sched_rt_entity(rt_se) {
		ರಚನೆ rt_rq *rt_rq = ಗುಂಪು_rt_rq(rt_se);

		ಆದರೆ (rt_rq && rt_rq->rt_nr_running)
			__enqueue_rt_entity(rt_se, flags);
	}
	ಸರತಿಗೆ_ಸೇರಿಸು_ತುದಿ_rt_rq(&rq->rt);
}

/*
 * Adding/removing a task to/from a priority array:
 */
ಸ್ಥಿರ ಶೂನ್ಯ
ಸರತಿಗೆ_ಸೇರಿಸು_task_rt(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ flags)
{
	ರಚನೆ sched_rt_entity *rt_se = &p->rt;

	ಆದರೆ (flags & ಸರತಿಗೆ_ಸೇರಿಸು_WAKEUP)
		rt_se->timeout = 0;

	check_schedstat_required();
	update_stats_wait_ಆರಂಭ_rt(rt_rq_ದ_se(rt_se), rt_se);

	ಸರತಿಗೆ_ಸೇರಿಸು_rt_entity(rt_se, flags);

	ಆದರೆ (task_is_blocked(p))
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (!task_current(rq, p) && p->nr_cpus_allowed > 1)
		ಸರತಿಗೆ_ಸೇರಿಸು_pushable_task(rq, p);
}

ಸ್ಥಿರ bool ಸರತಿಯಿಂದ_ತೆಗೆ_task_rt(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ flags)
{
	ರಚನೆ sched_rt_entity *rt_se = &p->rt;

	update_curr_rt(rq);
	ಸರತಿಯಿಂದ_ತೆಗೆ_rt_entity(rt_se, flags);

	ಸರತಿಯಿಂದ_ತೆಗೆ_pushable_task(rq, p);

	ಹಿಂತಿರುಗಿ true;
}

/*
 * Put task to the head or the end of the run list without the overhead of
 * dequeue followed by enqueue.
 */
ಸ್ಥಿರ ಶೂನ್ಯ
requeue_rt_entity(ರಚನೆ rt_rq *rt_rq, ರಚನೆ sched_rt_entity *rt_se, ಪೂರ್ಣಾಂಕ ತಲೆ)
{
	ಆದರೆ (on_rt_rq(rt_se)) {
		ರಚನೆ rt_prio_array *array = &rt_rq->ಸಕ್ರಿಯ;
		ರಚನೆ list_ತಲೆ *ಸರತಿ = array->ಸರತಿ + rt_se_prio(rt_se);

		ಆದರೆ (ತಲೆ)
			list_move(&rt_se->ಸತತ_list, ಸರತಿ);
		ಇಲ್ಲದಿದ್ದರೆ
			list_move_ಬಾಲ(&rt_se->ಸತತ_list, ಸರತಿ);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ requeue_task_rt(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ ತಲೆ)
{
	ರಚನೆ sched_rt_entity *rt_se = &p->rt;
	ರಚನೆ rt_rq *rt_rq;

	for_each_sched_rt_entity(rt_se) {
		rt_rq = rt_rq_ದ_se(rt_se);
		requeue_rt_entity(rt_rq, rt_se, ತಲೆ);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ yield_task_rt(ರಚನೆ rq *rq)
{
	requeue_task_rt(rq, rq->donor, 0);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಕಂಡುಹಿಡಿ_ಅತಿಕಡಿಮೆ_rq(ರಚನೆ task_struct *task);

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
select_task_rq_rt(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ cpu, ಪೂರ್ಣಾಂಕ flags)
{
	ರಚನೆ task_struct *curr, *donor;
	ರಚನೆ rq *rq;
	bool test;

	/* For anything but wake ups, just return the task_cpu */
	ಆದರೆ (!(flags & (WF_TTWU | WF_FORK)))
		ಹೋಗು ಹೊರಹೋಗು;

	rq = cpu_rq(cpu);

	rcu_read_ಬೀಗ();
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

	ಆದರೆ (test || !rt_task_ಹೊಂದುತ್ತದೆ_ಸಾಮರ್ಥ್ಯ(p, cpu)) {
		ಪೂರ್ಣಾಂಕ ಗುರಿ = ಕಂಡುಹಿಡಿ_ಅತಿಕಡಿಮೆ_rq(p);

		/*
		 * Bail out if we were forcing a migration to find a better
		 * fitting CPU but our search failed.
		 */
		ಆದರೆ (!test && ಗುರಿ != -1 && !rt_task_ಹೊಂದುತ್ತದೆ_ಸಾಮರ್ಥ್ಯ(p, ಗುರಿ))
			ಹೋಗು ಹೊರಹೋಗು_unlock;

		/*
		 * Don't bother moving it if the destination CPU is
		 * not running a lower priority task.
		 */
		ಆದರೆ (ಗುರಿ != -1 &&
		    p->prio < cpu_rq(ಗುರಿ)->rt.highest_prio.curr)
			cpu = ಗುರಿ;
	}

ಹೊರಹೋಗು_unlock:
	rcu_read_unlock();

ಹೊರಹೋಗು:
	ಹಿಂತಿರುಗಿ cpu;
}

ಸ್ಥಿರ ಶೂನ್ಯ check_preempt_equal_prio(ರಚನೆ rq *rq, ರಚನೆ task_struct *p)
{
	ಆದರೆ (rq->curr->nr_cpus_allowed == 1 ||
	    !cpupri_ಕಂಡುಹಿಡಿ(&rq->rd->cpupri, rq->donor, ಖಾಲಿ))
		ಹಿಂತಿರುಗಿ;

	/*
	 * p is migratable, so let's not schedule it and
	 * see if it is pushed or pulled somewhere else.
	 */
	ಆದರೆ (p->nr_cpus_allowed != 1 &&
	    cpupri_ಕಂಡುಹಿಡಿ(&rq->rd->cpupri, p, ಖಾಲಿ))
		ಹಿಂತಿರುಗಿ;

	/*
	 * There appear to be other CPUs that can accept
	 * the current task but none can run 'p', so lets reschedule
	 * to try and push the current task away:
	 */
	requeue_task_rt(rq, p, 1);
	resched_curr(rq);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ balance_rt(ರಚನೆ rq *rq, ರಚನೆ rq_flags *rf)
{
	/*
	 * Note, rq->donor may change during rq lock drops,
	 * so don't re-use p across lock drops
	 */
	ರಚನೆ task_struct *p = rq->donor;

	ಆದರೆ (!on_rt_rq(&p->rt) && need_pull_rt_task(rq, p)) {
		/*
		 * This is OK, because current is on_cpu, which avoids it being
		 * picked for load-balance and preemption/IRQs are still
		 * disabled avoiding further scheduler activity on it and we've
		 * not yet started the picking loop.
		 */
		rq_unpin_ಬೀಗ(rq, rf);
		pull_rt_task(rq);
		rq_repin_ಬೀಗ(rq, rf);
	}

	ಹಿಂತಿರುಗಿ sched_stop_runnable(rq) || sched_dl_runnable(rq) || sched_rt_runnable(rq);
}

/*
 * Preempt the current task with a newly woken task if needed:
 */
ಸ್ಥಿರ ಶೂನ್ಯ wakeup_preempt_rt(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ flags)
{
	ರಚನೆ task_struct *donor = rq->donor;

	/*
	 * XXX If we're preempted by DL, queue a push?
	 */
	ಆದರೆ (p->sched_class != &rt_sched_class ||
	    donor->sched_class != &rt_sched_class)
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (p->prio < donor->prio) {
		resched_curr(rq);
		ಹಿಂತಿರುಗಿ;
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
	ಆದರೆ (p->prio == donor->prio && !test_tsk_need_resched(rq->curr))
		check_preempt_equal_prio(rq, p);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ ಗಣ_ಮುಂದಿನ_task_rt(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, ಎಣಿಕೆ snt_e type)
{
	ರಚನೆ sched_rt_entity *rt_se = &p->rt;
	ರಚನೆ rt_rq *rt_rq = &rq->rt;

	ಆದರೆ (type == SNT_REPICK)
		ಹಿಂತಿರುಗಿ;

	p->se.exec_ಆರಂಭ = rq_clock_task(rq);
	ಆದರೆ (on_rt_rq(&p->rt))
		update_stats_wait_ಕೊನೆ_rt(rt_rq, rt_se);

	/* The running task is never eligible for pushing */
	ಸರತಿಯಿಂದ_ತೆಗೆ_pushable_task(rq, p);

	ಆದರೆ (type != SNT_PICK)
		ಹಿಂತಿರುಗಿ;

	/*
	 * If prev task was rt, put_prev_task() has already updated the
	 * utilization. We only care of the case where we start to schedule a
	 * rt task
	 */
	ಆದರೆ (rq->donor->sched_class != &rt_sched_class)
		update_rt_rq_ತುಂಬು_avg(rq_clock_pelt(rq), rq, 0);

	rt_ಸರತಿ_ತಳ್ಳು_tasks(rq);
}

ಸ್ಥಿರ ರಚನೆ sched_rt_entity *pick_ಮುಂದಿನ_rt_entity(ರಚನೆ rt_rq *rt_rq)
{
	ರಚನೆ rt_prio_array *array = &rt_rq->ಸಕ್ರಿಯ;
	ರಚನೆ sched_rt_entity *ಮುಂದಿನ = ಖಾಲಿ;
	ರಚನೆ list_ತಲೆ *ಸರತಿ;
	ಪೂರ್ಣಾಂಕ idx;

	idx = sched_ಕಂಡುಹಿಡಿ_ಮೊದಲ_ಬಿಟ್(array->bitmap);
	BUG_ON(idx >= MAX_RT_PRIO);

	ಸರತಿ = array->ಸರತಿ + idx;
	ಆದರೆ (WARN_ON_ONCE(list_ಖಾಲಿಗಳು(ಸರತಿ)))
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;
	ಮುಂದಿನ = list_ನಮೂದು(ಸರತಿ->ಮುಂದಿನ, ರಚನೆ sched_rt_entity, ಸತತ_list);

	ಹಿಂತಿರುಗಿ ಮುಂದಿನ;
}

ಸ್ಥಿರ ರಚನೆ task_struct *pick_ಮುಂದಿನ_task_rt(ರಚನೆ rq *rq)
{
	ರಚನೆ sched_rt_entity *rt_se;
	ರಚನೆ rt_rq *rt_rq  = &rq->rt;

	ಮಾಡು {
		rt_se = pick_ಮುಂದಿನ_rt_entity(rt_rq);
		ಆದರೆ (unlikely(!rt_se))
			ಹಿಂತಿರುಗಿ ಖಾಲಿ;
		rt_rq = ಗುಂಪು_rt_rq(rt_se);
	} ಆಗ (rt_rq);

	ಹಿಂತಿರುಗಿ rt_task_ದ(rt_se);
}

ಸ್ಥಿರ ರಚನೆ task_struct *pick_task_rt(ರಚನೆ rq *rq, ರಚನೆ rq_flags *rf)
{
	ರಚನೆ task_struct *p;

	ಆದರೆ (!sched_rt_runnable(rq))
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	p = pick_ಮುಂದಿನ_task_rt(rq);

	ಹಿಂತಿರುಗಿ p;
}

ಸ್ಥಿರ ಶೂನ್ಯ put_prev_task_rt(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, ರಚನೆ task_struct *ಮುಂದಿನ)
{
	ರಚನೆ sched_rt_entity *rt_se = &p->rt;
	ರಚನೆ rt_rq *rt_rq = &rq->rt;

	ಆದರೆ (on_rt_rq(&p->rt))
		update_stats_wait_ಆರಂಭ_rt(rt_rq, rt_se);

	update_curr_rt(rq);

	update_rt_rq_ತುಂಬು_avg(rq_clock_pelt(rq), rq, 1);

	ಆದರೆ (task_is_blocked(p))
		ಹಿಂತಿರುಗಿ;
	/*
	 * The previous task needs to be made eligible for pushing
	 * if it is still active
	 */
	ಆದರೆ (on_rt_rq(&p->rt) && p->nr_cpus_allowed > 1)
		ಸರತಿಗೆ_ಸೇರಿಸು_pushable_task(rq, p);
}

/* Only try algorithms three times */
#ವ್ಯಾಖ್ಯಾನಿಸು RT_MAX_TRIES 3

/*
 * Return the highest pushable rq's task, which is suitable to be executed
 * on the CPU, NULL otherwise
 */
ಸ್ಥಿರ ರಚನೆ task_struct *pick_highest_pushable_task(ರಚನೆ rq *rq, ಪೂರ್ಣಾಂಕ cpu)
{
	ರಚನೆ plist_ತಲೆ *ತಲೆ = &rq->rt.pushable_tasks;
	ರಚನೆ task_struct *p;

	ಆದರೆ (!has_pushable_tasks(rq))
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	plist_for_each_ನಮೂದು(p, ತಲೆ, pushable_tasks) {
		ಆದರೆ (task_is_pushable(rq, p, cpu))
			ಹಿಂತಿರುಗಿ p;
	}

	ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಸ್ಥಿರ DEFINE_ಪ್ರತಿ_CPU(cpumask_var_t, local_cpu_ಮುಖವಾಡ);

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಕಂಡುಹಿಡಿ_ಅತಿಕಡಿಮೆ_rq(ರಚನೆ task_struct *task)
{
	ರಚನೆ sched_domain *sd;
	ರಚನೆ cpumask *ಅತಿಕಡಿಮೆ_ಮುಖವಾಡ = this_cpu_cpumask_var_ptr(local_cpu_ಮುಖವಾಡ);
	ಪೂರ್ಣಾಂಕ this_cpu = smp_processor_id();
	ಪೂರ್ಣಾಂಕ cpu      = task_cpu(task);
	ಪೂರ್ಣಾಂಕ ret;

	/* Make sure the mask is initialized first */
	ಆದರೆ (unlikely(!ಅತಿಕಡಿಮೆ_ಮುಖವಾಡ))
		ಹಿಂತಿರುಗಿ -1;

	ಆದರೆ (task->nr_cpus_allowed == 1)
		ಹಿಂತಿರುಗಿ -1; /* No other targets possible */

	/*
	 * If we're on asym system ensure we consider the different capacities
	 * of the CPUs when searching for the lowest_mask.
	 */
	ಆದರೆ (sched_asym_cpucap_ಸಕ್ರಿಯ()) {

		ret = cpupri_ಕಂಡುಹಿಡಿ_fitness(&task_rq(task)->rd->cpupri,
					  task, ಅತಿಕಡಿಮೆ_ಮುಖವಾಡ,
					  rt_task_ಹೊಂದುತ್ತದೆ_ಸಾಮರ್ಥ್ಯ);
	} ಇಲ್ಲದಿದ್ದರೆ {

		ret = cpupri_ಕಂಡುಹಿಡಿ(&task_rq(task)->rd->cpupri,
				  task, ಅತಿಕಡಿಮೆ_ಮುಖವಾಡ);
	}

	ಆದರೆ (!ret)
		ಹಿಂತಿರುಗಿ -1; /* No targets found */

	/*
	 * At this point we have built a mask of CPUs representing the
	 * lowest priority tasks in the system.  Now we want to elect
	 * the best one based on our affinity and topology.
	 *
	 * We prioritize the last CPU that the task executed on since
	 * it is most likely cache-hot in that location.
	 */
	ಆದರೆ (cpumask_test_cpu(cpu, ಅತಿಕಡಿಮೆ_ಮುಖವಾಡ))
		ಹಿಂತಿರುಗಿ cpu;

	/*
	 * Otherwise, we consult the sched_domains span maps to figure
	 * out which CPU is logically closest to our hot cache data.
	 */
	ಆದರೆ (!cpumask_test_cpu(this_cpu, ಅತಿಕಡಿಮೆ_ಮುಖವಾಡ))
		this_cpu = -1; /* Skip this_cpu opt if not among lowest */

	rcu_read_ಬೀಗ();
	for_each_domain(cpu, sd) {
		ಆದರೆ (sd->flags & SD_WAKE_AFFINE) {
			ಪೂರ್ಣಾಂಕ ಉತ್ತಮ_cpu;

			/*
			 * "this_cpu" is cheaper to preempt than a
			 * remote processor.
			 */
			ಆದರೆ (this_cpu != -1 &&
			    cpumask_test_cpu(this_cpu, sched_domain_ಹರವು(sd))) {
				rcu_read_unlock();
				ಹಿಂತಿರುಗಿ this_cpu;
			}

			ಉತ್ತಮ_cpu = cpumask_any_and_distribute(ಅತಿಕಡಿಮೆ_ಮುಖವಾಡ,
							      sched_domain_ಹರವು(sd));
			ಆದರೆ (ಉತ್ತಮ_cpu < nr_cpu_ids) {
				rcu_read_unlock();
				ಹಿಂತಿರುಗಿ ಉತ್ತಮ_cpu;
			}
		}
	}
	rcu_read_unlock();

	/*
	 * And finally, if there were no matches within the domains
	 * just give the caller *something* to work with from the compatible
	 * locations.
	 */
	ಆದರೆ (this_cpu != -1)
		ಹಿಂತಿರುಗಿ this_cpu;

	cpu = cpumask_any_distribute(ಅತಿಕಡಿಮೆ_ಮುಖವಾಡ);
	ಆದರೆ (cpu < nr_cpu_ids)
		ಹಿಂತಿರುಗಿ cpu;

	ಹಿಂತಿರುಗಿ -1;
}

ಸ್ಥಿರ ರಚನೆ task_struct *pick_ಮುಂದಿನ_pushable_task(ರಚನೆ rq *rq)
{
	ರಚನೆ plist_ತಲೆ *ತಲೆ = &rq->rt.pushable_tasks;
	ರಚನೆ task_struct *i, *p = ಖಾಲಿ;

	ಆದರೆ (!has_pushable_tasks(rq))
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	plist_for_each_ನಮೂದು(i, ತಲೆ, pushable_tasks) {
		/* skip tasks that cannot be migrated */
		ಆದರೆ (!task_on_cpu(rq, i) && !is_migration_disabled(i)) {
			p = i;
			ನಿಲ್ಲಿಸು;
		}
	}

	ಆದರೆ (!p)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	BUG_ON(rq->cpu != task_cpu(p));
	BUG_ON(task_current(rq, p));
	BUG_ON(task_current_donor(rq, p));
	BUG_ON(p->nr_cpus_allowed <= 1);

	BUG_ON(!task_on_rq_queued(p));
	BUG_ON(!rt_task(p));

	ಹಿಂತಿರುಗಿ p;
}

/* Will lock the rq it finds */
ಸ್ಥಿರ ರಚನೆ rq *ಕಂಡುಹಿಡಿ_ಬೀಗ_ಅತಿಕಡಿಮೆ_rq(ರಚನೆ task_struct *task, ರಚನೆ rq *rq)
{
	ರಚನೆ rq *ಅತಿಕಡಿಮೆ_rq = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ tries;
	ಪೂರ್ಣಾಂಕ cpu;

	ಗಾಗಿ (tries = 0; tries < RT_MAX_TRIES; tries++) {
		cpu = ಕಂಡುಹಿಡಿ_ಅತಿಕಡಿಮೆ_rq(task);

		ಆದರೆ ((cpu == -1) || (cpu == rq->cpu))
			ನಿಲ್ಲಿಸು;

		ಅತಿಕಡಿಮೆ_rq = cpu_rq(cpu);

		ಆದರೆ (ಅತಿಕಡಿಮೆ_rq->rt.highest_prio.curr <= task->prio) {
			/*
			 * Target rq has tasks of equal or higher priority,
			 * retrying does not release any lock and is unlikely
			 * to yield a different result.
			 */
			ಅತಿಕಡಿಮೆ_rq = ಖಾಲಿ;
			ನಿಲ್ಲಿಸು;
		}

		/* if the prio of this runqueue changed, try again */
		ಆದರೆ (double_ಬೀಗ_balance(rq, ಅತಿಕಡಿಮೆ_rq)) {
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
			ಆದರೆ (unlikely(is_migration_disabled(task) ||
				     !cpumask_test_cpu(ಅತಿಕಡಿಮೆ_rq->cpu, &task->cpus_ಮುಖವಾಡ) ||
				     task != pick_ಮುಂದಿನ_pushable_task(rq))) {

				double_unlock_balance(rq, ಅತಿಕಡಿಮೆ_rq);
				ಅತಿಕಡಿಮೆ_rq = ಖಾಲಿ;
				ನಿಲ್ಲಿಸು;
			}
		}

		/* If this rq is still suitable use it. */
		ಆದರೆ (ಅತಿಕಡಿಮೆ_rq->rt.highest_prio.curr > task->prio)
			ನಿಲ್ಲಿಸು;

		/* try again */
		double_unlock_balance(rq, ಅತಿಕಡಿಮೆ_rq);
		ಅತಿಕಡಿಮೆ_rq = ಖಾಲಿ;
	}

	ಹಿಂತಿರುಗಿ ಅತಿಕಡಿಮೆ_rq;
}

/*
 * If the current CPU has more than one RT task, see if the non
 * running task can migrate over to a CPU that is running a task
 * of lesser priority.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ತಳ್ಳು_rt_task(ರಚನೆ rq *rq, bool pull)
{
	ರಚನೆ task_struct *ಮುಂದಿನ_task;
	ರಚನೆ rq *ಅತಿಕಡಿಮೆ_rq;
	ಪೂರ್ಣಾಂಕ ret = 0;

	ಆದರೆ (!rq->rt.overloaded)
		ಹಿಂತಿರುಗಿ 0;

	ಮುಂದಿನ_task = pick_ಮುಂದಿನ_pushable_task(rq);
	ಆದರೆ (!ಮುಂದಿನ_task)
		ಹಿಂತಿರುಗಿ 0;

retry:
	/*
	 * It's possible that the next_task slipped in of
	 * higher priority than current. If that's the case
	 * just reschedule current.
	 */
	ಆದರೆ (unlikely(ಮುಂದಿನ_task->prio < rq->donor->prio)) {
		resched_curr(rq);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (is_migration_disabled(ಮುಂದಿನ_task)) {
		ರಚನೆ task_struct *ತಳ್ಳು_task = ಖಾಲಿ;
		ಪೂರ್ಣಾಂಕ cpu;

		ಆದರೆ (!pull || rq->ತಳ್ಳು_busy)
			ಹಿಂತಿರುಗಿ 0;

		/*
		 * Invoking find_lowest_rq() on anything but an RT task doesn't
		 * make sense. Per the above priority check, curr has to
		 * be of higher priority than next_task, so no need to
		 * reschedule when bailing out.
		 *
		 * Note that the stoppers are masqueraded as SCHED_FIFO
		 * (cf. sched_set_stop_task()), so we can't rely on rt_task().
		 */
		ಆದರೆ (rq->donor->sched_class != &rt_sched_class)
			ಹಿಂತಿರುಗಿ 0;

		cpu = ಕಂಡುಹಿಡಿ_ಅತಿಕಡಿಮೆ_rq(rq->curr);
		ಆದರೆ (cpu == -1 || cpu == rq->cpu)
			ಹಿಂತಿರುಗಿ 0;

		/*
		 * Given we found a CPU with lower priority than @next_task,
		 * therefore it should be running. However we cannot migrate it
		 * to this other CPU, instead attempt to push the current
		 * running task on this CPU away.
		 */
		ತಳ್ಳು_task = get_ತಳ್ಳು_task(rq);
		ಆದರೆ (ತಳ್ಳು_task) {
			preempt_disable();
			raw_spin_rq_unlock(rq);
			stop_one_cpu_nowait(rq->cpu, ತಳ್ಳು_cpu_stop,
					    ತಳ್ಳು_task, &rq->ತಳ್ಳು_work);
			preempt_enable();
			raw_spin_rq_ಬೀಗ(rq);
		}

		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (WARN_ON(ಮುಂದಿನ_task == rq->curr))
		ಹಿಂತಿರುಗಿ 0;

	/* We might release rq lock */
	get_task_struct(ಮುಂದಿನ_task);

	/* find_lock_lowest_rq locks the rq if found */
	ಅತಿಕಡಿಮೆ_rq = ಕಂಡುಹಿಡಿ_ಬೀಗ_ಅತಿಕಡಿಮೆ_rq(ಮುಂದಿನ_task, rq);
	ಆದರೆ (!ಅತಿಕಡಿಮೆ_rq) {
		ರಚನೆ task_struct *task;
		/*
		 * find_lock_lowest_rq releases rq->lock
		 * so it is possible that next_task has migrated.
		 *
		 * We need to make sure that the task is still on the same
		 * run-queue and is also still the next task eligible for
		 * pushing.
		 */
		task = pick_ಮುಂದಿನ_pushable_task(rq);
		ಆದರೆ (task == ಮುಂದಿನ_task) {
			/*
			 * The task hasn't migrated, and is still the next
			 * eligible task, but we failed to find a run-queue
			 * to push it to.  Do not retry in this case, since
			 * other CPUs will pull from us when ready.
			 */
			ಹೋಗು ಹೊರಹೋಗು;
		}

		ಆದರೆ (!task)
			/* No more tasks, just exit */
			ಹೋಗು ಹೊರಹೋಗು;

		/*
		 * Something has shifted, try again.
		 */
		put_task_struct(ಮುಂದಿನ_task);
		ಮುಂದಿನ_task = task;
		ಹೋಗು retry;
	}

	move_queued_task_locked(rq, ಅತಿಕಡಿಮೆ_rq, ಮುಂದಿನ_task);
	resched_curr(ಅತಿಕಡಿಮೆ_rq);
	ret = 1;

	double_unlock_balance(rq, ಅತಿಕಡಿಮೆ_rq);
ಹೊರಹೋಗು:
	put_task_struct(ಮುಂದಿನ_task);

	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ ತಳ್ಳು_rt_tasks(ರಚನೆ rq *rq)
{
	/* push_rt_task will return true if it moved an RT */
	ಆಗ (ತಳ್ಳು_rt_task(rq, false))
		;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_RT_ತಳ್ಳು_IPI

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
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ rto_ಮುಂದಿನ_cpu(ರಚನೆ ಬೇರು_domain *rd)
{
	ಪೂರ್ಣಾಂಕ this_cpu = smp_processor_id();
	ಪೂರ್ಣಾಂಕ ಮುಂದಿನ;
	ಪೂರ್ಣಾಂಕ cpu;

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
	ಗಾಗಿ (;;) {

		/* When rto_cpu is -1 this acts like cpumask_first() */
		cpu = cpumask_ಮುಂದಿನ(rd->rto_cpu, rd->rto_ಮುಖವಾಡ);

		rd->rto_cpu = cpu;

		/* Do not send IPI to self */
		ಆದರೆ (cpu == this_cpu)
			ಮುಂದುವರಿ;

		ಆದರೆ (cpu < nr_cpu_ids)
			ಹಿಂತಿರುಗಿ cpu;

		rd->rto_cpu = -1;

		/*
		 * ACQUIRE ensures we see the @rto_mask changes
		 * made prior to the @next value observed.
		 *
		 * Matches WMB in rt_set_overload().
		 */
		ಮುಂದಿನ = atomic_read_acquire(&rd->rto_loop_ಮುಂದಿನ);

		ಆದರೆ (rd->rto_loop == ಮುಂದಿನ)
			ನಿಲ್ಲಿಸು;

		rd->rto_loop = ಮುಂದಿನ;
	}

	ಹಿಂತಿರುಗಿ -1;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool rto_ಆರಂಭ_trylock(atomic_t *v)
{
	ಹಿಂತಿರುಗಿ !atomic_cmpxchg_acquire(v, 0, 1);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ rto_ಆರಂಭ_unlock(atomic_t *v)
{
	atomic_ಗಣ_release(v, 0);
}

ಸ್ಥಿರ ಶೂನ್ಯ tell_cpu_ಗಮ್ಯ_ತಳ್ಳು(ರಚನೆ rq *rq)
{
	ಪೂರ್ಣಾಂಕ cpu = -1;

	/* Keep the loop going if the IPI is currently active */
	atomic_inc(&rq->rd->rto_loop_ಮುಂದಿನ);

	/* Only one CPU can initiate a loop at a time */
	ಆದರೆ (!rto_ಆರಂಭ_trylock(&rq->rd->rto_loop_ಆರಂಭ))
		ಹಿಂತಿರುಗಿ;

	raw_spin_ಬೀಗ(&rq->rd->rto_ಬೀಗ);

	/*
	 * The rto_cpu is updated under the lock, if it has a valid CPU
	 * then the IPI is still running and will continue due to the
	 * update to loop_next, and nothing needs to be done here.
	 * Otherwise it is finishing up and an IPI needs to be sent.
	 */
	ಆದರೆ (rq->rd->rto_cpu < 0)
		cpu = rto_ಮುಂದಿನ_cpu(rq->rd);

	raw_spin_unlock(&rq->rd->rto_ಬೀಗ);

	rto_ಆರಂಭ_unlock(&rq->rd->rto_loop_ಆರಂಭ);

	ಆದರೆ (cpu >= 0) {
		/* Make sure the rd does not get freed while pushing */
		sched_get_rd(rq->rd);
		irq_work_ಸರತಿ_on(&rq->rd->rto_ತಳ್ಳು_work, cpu);
	}
}

/* Called from hardirq context */
ಶೂನ್ಯ rto_ತಳ್ಳು_irq_work_func(ರಚನೆ irq_work *work)
{
	ರಚನೆ ಬೇರು_domain *rd =
		container_ದ(work, ರಚನೆ ಬೇರು_domain, rto_ತಳ್ಳು_work);
	ರಚನೆ rq *rq;
	ಪೂರ್ಣಾಂಕ cpu;

	rq = this_rq();

	/*
	 * We do not need to grab the lock to check for has_pushable_tasks.
	 * When it gets updated, a check is made if a push is possible.
	 */
	ಆದರೆ (has_pushable_tasks(rq)) {
		raw_spin_rq_ಬೀಗ(rq);
		ಆಗ (ತಳ್ಳು_rt_task(rq, true))
			;
		raw_spin_rq_unlock(rq);
	}

	raw_spin_ಬೀಗ(&rd->rto_ಬೀಗ);

	/* Pass the IPI to the next rt overloaded queue */
	cpu = rto_ಮುಂದಿನ_cpu(rd);

	raw_spin_unlock(&rd->rto_ಬೀಗ);

	ಆದರೆ (cpu < 0) {
		sched_put_rd(rd);
		ಹಿಂತಿರುಗಿ;
	}

	/* Try the next RT overloaded CPU */
	irq_work_ಸರತಿ_on(&rd->rto_ತಳ್ಳು_work, cpu);
}
#ಆದರೆ_ಅಂತ್ಯ /* HAVE_RT_PUSH_IPI */

ಸ್ಥಿರ ಶೂನ್ಯ pull_rt_task(ರಚನೆ rq *this_rq)
{
	ಪೂರ್ಣಾಂಕ this_cpu = this_rq->cpu, cpu;
	bool resched = false;
	ರಚನೆ task_struct *p, *ತಳ್ಳು_task;
	ರಚನೆ rq *src_rq;
	ಪೂರ್ಣಾಂಕ rt_overload_ಎಣಿಕೆ = rt_overloaded(this_rq);

	ಆದರೆ (likely(!rt_overload_ಎಣಿಕೆ))
		ಹಿಂತಿರುಗಿ;

	/*
	 * Match the barrier from rt_set_overloaded; this guarantees that if we
	 * see overloaded we must also see the rto_mask bit.
	 */
	smp_rmb();

	/* If we are the only overloaded CPU do nothing */
	ಆದರೆ (rt_overload_ಎಣಿಕೆ == 1 &&
	    cpumask_test_cpu(this_rq->cpu, this_rq->rd->rto_ಮುಖವಾಡ))
		ಹಿಂತಿರುಗಿ;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_RT_ತಳ್ಳು_IPI
	ಆದರೆ (sched_feat(RT_ತಳ್ಳು_IPI)) {
		tell_cpu_ಗಮ್ಯ_ತಳ್ಳು(this_rq);
		ಹಿಂತಿರುಗಿ;
	}
#ಆದರೆ_ಅಂತ್ಯ

	for_each_cpu(cpu, this_rq->rd->rto_ಮುಖವಾಡ) {
		ಆದರೆ (this_cpu == cpu)
			ಮುಂದುವರಿ;

		src_rq = cpu_rq(cpu);

		/*
		 * Don't bother taking the src_rq->lock if the next highest
		 * task is known to be lower-priority than our current task.
		 * This may look racy, but if this value is about to go
		 * logically higher, the src_rq will push this task away.
		 * And if its going logically lower, we do not care
		 */
		ಆದರೆ (src_rq->rt.highest_prio.ಮುಂದಿನ >=
		    this_rq->rt.highest_prio.curr)
			ಮುಂದುವರಿ;

		/*
		 * We can potentially drop this_rq's lock in
		 * double_lock_balance, and another CPU could
		 * alter this_rq
		 */
		ತಳ್ಳು_task = ಖಾಲಿ;
		double_ಬೀಗ_balance(this_rq, src_rq);

		/*
		 * We can pull only a task, which is pushable
		 * on its rq, and no others.
		 */
		p = pick_highest_pushable_task(src_rq, this_cpu);

		/*
		 * Do we have an RT task that preempts
		 * the to-be-scheduled task?
		 */
		ಆದರೆ (p && (p->prio < this_rq->rt.highest_prio.curr)) {
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
			ಆದರೆ (p->prio < src_rq->donor->prio)
				ಹೋಗು skip;

			ಆದರೆ (is_migration_disabled(p)) {
				ತಳ್ಳು_task = get_ತಳ್ಳು_task(src_rq);
			} ಇಲ್ಲದಿದ್ದರೆ {
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

		ಆದರೆ (ತಳ್ಳು_task) {
			preempt_disable();
			raw_spin_rq_unlock(this_rq);
			stop_one_cpu_nowait(src_rq->cpu, ತಳ್ಳು_cpu_stop,
					    ತಳ್ಳು_task, &src_rq->ತಳ್ಳು_work);
			preempt_enable();
			raw_spin_rq_ಬೀಗ(this_rq);
		}
	}

	ಆದರೆ (resched)
		resched_curr(this_rq);
}

/*
 * If we are not running and we are not going to reschedule soon, we should
 * try to push tasks away now
 */
ಸ್ಥಿರ ಶೂನ್ಯ task_woken_rt(ರಚನೆ rq *rq, ರಚನೆ task_struct *p)
{
	bool need_ಗಮ್ಯ_ತಳ್ಳು = !task_on_cpu(rq, p) &&
			    !test_tsk_need_resched(rq->curr) &&
			    p->nr_cpus_allowed > 1 &&
			    (dl_task(rq->donor) || rt_task(rq->donor)) &&
			    (rq->curr->nr_cpus_allowed < 2 ||
			     rq->donor->prio <= p->prio);

	ಆದರೆ (need_ಗಮ್ಯ_ತಳ್ಳು)
		ತಳ್ಳು_rt_tasks(rq);
}

/* Assumes rq->lock is held */
ಸ್ಥಿರ ಶೂನ್ಯ rq_online_rt(ರಚನೆ rq *rq)
{
	ಆದರೆ (rq->rt.overloaded)
		rt_ಗಣ_overload(rq);

	__enable_runtime(rq);

	cpupri_ಗಣ(&rq->rd->cpupri, rq->cpu, rq->rt.highest_prio.curr);
}

/* Assumes rq->lock is held */
ಸ್ಥಿರ ಶೂನ್ಯ rq_offline_rt(ರಚನೆ rq *rq)
{
	ಆದರೆ (rq->rt.overloaded)
		rt_clear_overload(rq);

	__disable_runtime(rq);

	cpupri_ಗಣ(&rq->rd->cpupri, rq->cpu, CPUPRI_INVALID);
}

/*
 * When switch from the rt queue, we bring ourselves to a position
 * that we might want to pull RT tasks from other runqueues.
 */
ಸ್ಥಿರ ಶೂನ್ಯ switched_from_rt(ರಚನೆ rq *rq, ರಚನೆ task_struct *p)
{
	/*
	 * If there are other RT tasks then we will reschedule
	 * and the scheduling of the other RT tasks will handle
	 * the balancing. But if we are the last RT task
	 * we may need to handle the pulling of RT tasks
	 * now.
	 */
	ಆದರೆ (!task_on_rq_queued(p) || rq->rt.rt_nr_running)
		ಹಿಂತಿರುಗಿ;

	rt_ಸರತಿ_pull_task(rq);
}

ಶೂನ್ಯ __init init_sched_rt_class(ಶೂನ್ಯ)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ i;

	for_each_possible_cpu(i) {
		zalloc_cpumask_var_ಗಂಟು(&ಪ್ರತಿ_cpu(local_cpu_ಮುಖವಾಡ, i),
					GFP_KERNEL, cpu_ಗಮ್ಯ_ಗಂಟು(i));
	}
}

/*
 * When switching a task to RT, we may overload the runqueue
 * with RT tasks. In this case we try to push them off to
 * other runqueues.
 */
ಸ್ಥಿರ ಶೂನ್ಯ switched_ಗಮ್ಯ_rt(ರಚನೆ rq *rq, ರಚನೆ task_struct *p)
{
	/*
	 * If we are running, update the avg_rt tracking, as the running time
	 * will now on be accounted into the latter.
	 */
	ಆದರೆ (task_current(rq, p)) {
		update_rt_rq_ತುಂಬು_avg(rq_clock_pelt(rq), rq, 0);
		ಹಿಂತಿರುಗಿ;
	}

	/*
	 * If we are not running we may need to preempt the current
	 * running task. If that current running task is also an RT task
	 * then see if we can move to another run queue.
	 */
	ಆದರೆ (task_on_rq_queued(p)) {
		ಆದರೆ (p->nr_cpus_allowed > 1 && rq->rt.overloaded)
			rt_ಸರತಿ_ತಳ್ಳು_tasks(rq);
		ಆದರೆ (p->prio < rq->donor->prio && cpu_online(cpu_ದ(rq)))
			resched_curr(rq);
	}
}

/*
 * Priority of the task has changed. This may cause
 * us to initiate a push or pull.
 */
ಸ್ಥಿರ ಶೂನ್ಯ
prio_ಬದಲಾಯಿತು_rt(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, u64 oldprio)
{
	ಆದರೆ (!task_on_rq_queued(p))
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (p->prio == oldprio)
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (task_current_donor(rq, p)) {
		/*
		 * If our priority decreases while running, we
		 * may need to pull tasks to this runqueue.
		 */
		ಆದರೆ (oldprio < p->prio)
			rt_ಸರತಿ_pull_task(rq);

		/*
		 * If there's a higher priority task waiting to run
		 * then reschedule.
		 */
		ಆದರೆ (p->prio > rq->rt.highest_prio.curr)
			resched_curr(rq);
	} ಇಲ್ಲದಿದ್ದರೆ {
		/*
		 * This task is not running, but if it is
		 * greater than the current running task
		 * then reschedule.
		 */
		ಆದರೆ (p->prio < rq->donor->prio)
			resched_curr(rq);
	}
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_POSIX_TIMERS
ಸ್ಥಿರ ಶೂನ್ಯ watchdog(ರಚನೆ rq *rq, ರಚನೆ task_struct *p)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ soft, hard;

	/* max may change after cur was read, this will be fixed next tick */
	soft = task_rlimit(p, RLIMIT_RTTIME);
	hard = task_rlimit_max(p, RLIMIT_RTTIME);

	ಆದರೆ (soft != RLIM_INFINITY) {
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಮುಂದಿನ;

		ಆದರೆ (p->rt.watchdog_stamp != jiffies) {
			p->rt.timeout++;
			p->rt.watchdog_stamp = jiffies;
		}

		ಮುಂದಿನ = DIV_ಸುತ್ತು_ಮೇಲೆ(min(soft, hard), USEC_ಪ್ರತಿ_SEC/HZ);
		ಆದರೆ (p->rt.timeout > ಮುಂದಿನ) {
			posix_cputimers_rt_watchdog(&p->posix_cputimers,
						    p->se.ಮೊತ್ತ_exec_runtime);
		}
	}
}
#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_POSIX_TIMERS: */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ watchdog(ರಚನೆ rq *rq, ರಚನೆ task_struct *p) { }
#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_POSIX_TIMERS */

/*
 * scheduler tick hitting a task of our scheduling class.
 *
 * NOTE: This function can be called remotely by the tick offload that
 * goes along full dynticks. Therefore no local assumption can be made
 * and everything must be accessed through the @rq and @curr passed in
 * parameters.
 */
ಸ್ಥಿರ ಶೂನ್ಯ task_tick_rt(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ queued)
{
	ರಚನೆ sched_rt_entity *rt_se = &p->rt;

	update_curr_rt(rq);
	update_rt_rq_ತುಂಬು_avg(rq_clock_pelt(rq), rq, 1);

	watchdog(rq, p);

	/*
	 * RR tasks need a special form of time-slice management.
	 * FIFO tasks have no timeslices.
	 */
	ಆದರೆ (p->policy != SCHED_RR)
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (--p->rt.ಸಮಯ_ತುಂಡು)
		ಹಿಂತಿರುಗಿ;

	p->rt.ಸಮಯ_ತುಂಡು = sched_rr_timeslice;

	/*
	 * Requeue to the end of queue if we (and all of our ancestors) are not
	 * the only element on the queue
	 */
	for_each_sched_rt_entity(rt_se) {
		ಆದರೆ (rt_se->ಸತತ_list.prev != rt_se->ಸತತ_list.ಮುಂದಿನ) {
			requeue_task_rt(rq, p, 0);
			resched_curr(rq);
			ಹಿಂತಿರುಗಿ;
		}
	}
}

ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ get_rr_interval_rt(ರಚನೆ rq *rq, ರಚನೆ task_struct *task)
{
	/*
	 * Time slice is 0 for SCHED_FIFO tasks
	 */
	ಆದರೆ (task->policy == SCHED_RR)
		ಹಿಂತಿರುಗಿ sched_rr_timeslice;
	ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ 0;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CORE
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ task_is_throttled_rt(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ cpu)
{
	ರಚನೆ rt_rq *rt_rq;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_RT_ಗುಂಪು_SCHED // XXX maybe add task_rt_rq(), see also sched_rt_period_rt_rq
	rt_rq = task_ಗುಂಪು(p)->rt_rq[cpu];
	WARN_ON(!rt_ಗುಂಪು_sched_enabled() && rt_rq->tg != &ಬೇರು_task_ಗುಂಪು);
#ಇಲ್ಲದಿದ್ದರೆ
	rt_rq = &cpu_rq(cpu)->rt;
#ಆದರೆ_ಅಂತ್ಯ

	ಹಿಂತಿರುಗಿ rt_rq_throttled(rt_rq);
}
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_SCHED_CORE */

DEFINE_SCHED_CLASS(rt) = {
	.ಸರತಿಗೆ_ಸೇರಿಸು_task		= ಸರತಿಗೆ_ಸೇರಿಸು_task_rt,
	.ಸರತಿಯಿಂದ_ತೆಗೆ_task		= ಸರತಿಯಿಂದ_ತೆಗೆ_task_rt,
	.yield_task		= yield_task_rt,

	.wakeup_preempt		= wakeup_preempt_rt,

	.pick_task		= pick_task_rt,
	.put_prev_task		= put_prev_task_rt,
	.ಗಣ_ಮುಂದಿನ_task          = ಗಣ_ಮುಂದಿನ_task_rt,

	.balance		= balance_rt,
	.select_task_rq		= select_task_rq_rt,
	.ಗಣ_cpus_allowed       = ಗಣ_cpus_allowed_common,
	.rq_online              = rq_online_rt,
	.rq_offline             = rq_offline_rt,
	.task_woken		= task_woken_rt,
	.switched_from		= switched_from_rt,
	.ಕಂಡುಹಿಡಿ_ಬೀಗ_rq		= ಕಂಡುಹಿಡಿ_ಬೀಗ_ಅತಿಕಡಿಮೆ_rq,

	.task_tick		= task_tick_rt,

	.get_rr_interval	= get_rr_interval_rt,

	.switched_ಗಮ್ಯ		= switched_ಗಮ್ಯ_rt,
	.prio_ಬದಲಾಯಿತು		= prio_ಬದಲಾಯಿತು_rt,

	.update_curr		= update_curr_rt,

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CORE
	.task_is_throttled	= task_is_throttled_rt,
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_UCLAMP_TASK
	.uclamp_enabled		= 1,
#ಆದರೆ_ಅಂತ್ಯ
};

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_RT_ಗುಂಪು_SCHED
/*
 * Ensure that the real time constraints are schedulable.
 */
ಸ್ಥಿರ DEFINE_MUTEX(rt_constraints_mutex);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ tg_has_rt_tasks(ರಚನೆ task_ಗುಂಪು *tg)
{
	ರಚನೆ task_struct *task;
	ರಚನೆ css_task_iter it;
	ಪೂರ್ಣಾಂಕ ret = 0;

	/*
	 * Autogroups do not have RT tasks; see autogroup_create().
	 */
	ಆದರೆ (task_ಗುಂಪು_is_autogroup(tg))
		ಹಿಂತಿರುಗಿ 0;

	css_task_iter_ಆರಂಭ(&tg->css, 0, &it);
	ಆಗ (!ret && (task = css_task_iter_ಮುಂದಿನ(&it)))
		ret |= rt_task(task);
	css_task_iter_ಕೊನೆ(&it);

	ಹಿಂತಿರುಗಿ ret;
}

ರಚನೆ rt_schedulable_ದತ್ತಾಂಶ {
	ರಚನೆ task_ಗುಂಪು *tg;
	u64 rt_period;
	u64 rt_runtime;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ tg_rt_schedulable(ರಚನೆ task_ಗುಂಪು *tg, ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	ರಚನೆ rt_schedulable_ದತ್ತಾಂಶ *d = ದತ್ತಾಂಶ;
	ರಚನೆ task_ಗುಂಪು *ಮಗು;
	u64 ಒಟ್ಟು, ಮೊತ್ತ = 0;
	u64 period, runtime;

	period = ktime_ಗಮ್ಯ_ns(tg->rt_bandwidth.rt_period);
	runtime = tg->rt_bandwidth.rt_runtime;

	ಆದರೆ (tg == d->tg) {
		period = d->rt_period;
		runtime = d->rt_runtime;
	}

	/*
	 * Cannot have more runtime than the period.
	 */
	ಆದರೆ (runtime > period && runtime != RUNTIME_INF)
		ಹಿಂತಿರುಗಿ -EINVAL;

	/*
	 * Ensure we don't starve existing RT tasks if runtime turns zero.
	 */
	ಆದರೆ (rt_bandwidth_enabled() && !runtime &&
	    tg->rt_bandwidth.rt_runtime && tg_has_rt_tasks(tg))
		ಹಿಂತಿರುಗಿ -EBUSY;

	ಒಟ್ಟು = ಗಮ್ಯ_ಅನುಪಾತ(period, runtime);

	/*
	 * Nobody can have more than the global setting allows.
	 */
	ಆದರೆ (ಒಟ್ಟು > ಗಮ್ಯ_ಅನುಪಾತ(global_rt_period(), global_rt_runtime()))
		ಹಿಂತಿರುಗಿ -EINVAL;

	/*
	 * The sum of our children's runtime should not exceed our own.
	 */
	list_for_each_ನಮೂದು_rcu(ಮಗು, &tg->children, siblings) {
		period = ktime_ಗಮ್ಯ_ns(ಮಗು->rt_bandwidth.rt_period);
		runtime = ಮಗು->rt_bandwidth.rt_runtime;

		ಆದರೆ (ಮಗು == d->tg) {
			period = d->rt_period;
			runtime = d->rt_runtime;
		}

		ಮೊತ್ತ += ಗಮ್ಯ_ಅನುಪಾತ(period, runtime);
	}

	ಆದರೆ (ಮೊತ್ತ > ಒಟ್ಟು)
		ಹಿಂತಿರುಗಿ -EINVAL;

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ __rt_schedulable(ರಚನೆ task_ಗುಂಪು *tg, u64 period, u64 runtime)
{
	ಪೂರ್ಣಾಂಕ ret;

	ರಚನೆ rt_schedulable_ದತ್ತಾಂಶ ದತ್ತಾಂಶ = {
		.tg = tg,
		.rt_period = period,
		.rt_runtime = runtime,
	};

	rcu_read_ಬೀಗ();
	ret = walk_tg_ವೃಕ್ಷ(tg_rt_schedulable, tg_nop, &ದತ್ತಾಂಶ);
	rcu_read_unlock();

	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ tg_ಗಣ_rt_bandwidth(ರಚನೆ task_ಗುಂಪು *tg,
		u64 rt_period, u64 rt_runtime)
{
	ಪೂರ್ಣಾಂಕ i, err = 0;

	/*
	 * Disallowing the root group RT runtime is BAD, it would disallow the
	 * kernel creating (and or operating) RT threads.
	 */
	ಆದರೆ (tg == &ಬೇರು_task_ಗುಂಪು && rt_runtime == 0)
		ಹಿಂತಿರುಗಿ -EINVAL;

	/* No period doesn't make any sense. */
	ಆದರೆ (rt_period == 0)
		ಹಿಂತಿರುಗಿ -EINVAL;

	/*
	 * Bound quota to defend quota against overflow during bandwidth shift.
	 */
	ಆದರೆ (rt_runtime != RUNTIME_INF && rt_runtime > max_rt_runtime)
		ಹಿಂತಿರುಗಿ -EINVAL;

	mutex_ಬೀಗ(&rt_constraints_mutex);
	err = __rt_schedulable(tg, rt_period, rt_runtime);
	ಆದರೆ (err)
		ಹೋಗು unlock;

	raw_spin_ಬೀಗ_irq(&tg->rt_bandwidth.rt_runtime_ಬೀಗ);
	tg->rt_bandwidth.rt_period = ns_ಗಮ್ಯ_ktime(rt_period);
	tg->rt_bandwidth.rt_runtime = rt_runtime;

	for_each_possible_cpu(i) {
		ರಚನೆ rt_rq *rt_rq = tg->rt_rq[i];

		raw_spin_ಬೀಗ(&rt_rq->rt_runtime_ಬೀಗ);
		rt_rq->rt_runtime = rt_runtime;
		raw_spin_unlock(&rt_rq->rt_runtime_ಬೀಗ);
	}
	raw_spin_unlock_irq(&tg->rt_bandwidth.rt_runtime_ಬೀಗ);
unlock:
	mutex_unlock(&rt_constraints_mutex);

	ಹಿಂತಿರುಗಿ err;
}

ಪೂರ್ಣಾಂಕ sched_ಗುಂಪು_ಗಣ_rt_runtime(ರಚನೆ task_ಗುಂಪು *tg, ದೀರ್ಘ rt_runtime_us)
{
	u64 rt_runtime, rt_period;

	rt_period = ktime_ಗಮ್ಯ_ns(tg->rt_bandwidth.rt_period);
	rt_runtime = (u64)rt_runtime_us * NSEC_ಪ್ರತಿ_USEC;
	ಆದರೆ (rt_runtime_us < 0)
		rt_runtime = RUNTIME_INF;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ((u64)rt_runtime_us > U64_MAX / NSEC_ಪ್ರತಿ_USEC)
		ಹಿಂತಿರುಗಿ -EINVAL;

	ಹಿಂತಿರುಗಿ tg_ಗಣ_rt_bandwidth(tg, rt_period, rt_runtime);
}

ದೀರ್ಘ sched_ಗುಂಪು_rt_runtime(ರಚನೆ task_ಗುಂಪು *tg)
{
	u64 rt_runtime_us;

	ಆದರೆ (tg->rt_bandwidth.rt_runtime == RUNTIME_INF)
		ಹಿಂತಿರುಗಿ -1;

	rt_runtime_us = tg->rt_bandwidth.rt_runtime;
	do_div(rt_runtime_us, NSEC_ಪ್ರತಿ_USEC);
	ಹಿಂತಿರುಗಿ rt_runtime_us;
}

ಪೂರ್ಣಾಂಕ sched_ಗುಂಪು_ಗಣ_rt_period(ರಚನೆ task_ಗುಂಪು *tg, u64 rt_period_us)
{
	u64 rt_runtime, rt_period;

	ಆದರೆ (rt_period_us > U64_MAX / NSEC_ಪ್ರತಿ_USEC)
		ಹಿಂತಿರುಗಿ -EINVAL;

	rt_period = rt_period_us * NSEC_ಪ್ರತಿ_USEC;
	rt_runtime = tg->rt_bandwidth.rt_runtime;

	ಹಿಂತಿರುಗಿ tg_ಗಣ_rt_bandwidth(tg, rt_period, rt_runtime);
}

ದೀರ್ಘ sched_ಗುಂಪು_rt_period(ರಚನೆ task_ಗುಂಪು *tg)
{
	u64 rt_period_us;

	rt_period_us = ktime_ಗಮ್ಯ_ns(tg->rt_bandwidth.rt_period);
	do_div(rt_period_us, NSEC_ಪ್ರತಿ_USEC);
	ಹಿಂತಿರುಗಿ rt_period_us;
}

ಪೂರ್ಣಾಂಕ sched_rt_can_attach(ರಚನೆ task_ಗುಂಪು *tg, ರಚನೆ task_struct *tsk)
{
	/* Don't accept real-time tasks when there is no way for them to run */
	ಆದರೆ (rt_ಗುಂಪು_sched_enabled() && rt_task(tsk) && tg->rt_bandwidth.rt_runtime == 0)
		ಹಿಂತಿರುಗಿ 0;

	ಹಿಂತಿರುಗಿ 1;
}

#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_RT_GROUP_SCHED */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SYSCTL
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sched_rt_global_validate(ಶೂನ್ಯ)
{
	ಆದರೆ ((sysctl_sched_rt_runtime != RUNTIME_INF) &&
		((sysctl_sched_rt_runtime > sysctl_sched_rt_period) ||
		 ((u64)sysctl_sched_rt_runtime *
			NSEC_ಪ್ರತಿ_USEC > max_rt_runtime)))
		ಹಿಂತಿರುಗಿ -EINVAL;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_RT_ಗುಂಪು_SCHED
	ಆದರೆ (!rt_ಗುಂಪು_sched_enabled())
		ಹಿಂತಿರುಗಿ 0;

	scoped_guard(mutex, &rt_constraints_mutex)
		ಹಿಂತಿರುಗಿ __rt_schedulable(ಖಾಲಿ, 0, 0);
#ಆದರೆ_ಅಂತ್ಯ
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sched_rt_handler(ಸ್ಥಿರಾಂಕ ರಚನೆ ctl_ಕೋಷ್ಟಕ *ಕೋಷ್ಟಕ, ಪೂರ್ಣಾಂಕ ಲಿಖ, ಶೂನ್ಯ *ಬಫರ್,
		ಗಾತ್ರ_t *lenp, loff_t *ppos)
{
	ಪೂರ್ಣಾಂಕ old_period, old_runtime;
	ಸ್ಥಿರ DEFINE_MUTEX(mutex);
	ಪೂರ್ಣಾಂಕ ret;

	mutex_ಬೀಗ(&mutex);
	sched_domains_mutex_ಬೀಗ();
	old_period = sysctl_sched_rt_period;
	old_runtime = sysctl_sched_rt_runtime;

	ret = proc_dointvec_minmax(ಕೋಷ್ಟಕ, ಲಿಖ, ಬಫರ್, lenp, ppos);

	ಆದರೆ (!ret && ಲಿಖ) {
		ret = sched_rt_global_validate();
		ಆದರೆ (ret)
			ಹೋಗು undo;

		ret = sched_dl_global_validate();
		ಆದರೆ (ret)
			ಹೋಗು undo;

		sched_dl_do_global();
	}
	ಆದರೆ (0) {
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

	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sched_rr_handler(ಸ್ಥಿರಾಂಕ ರಚನೆ ctl_ಕೋಷ್ಟಕ *ಕೋಷ್ಟಕ, ಪೂರ್ಣಾಂಕ ಲಿಖ, ಶೂನ್ಯ *ಬಫರ್,
		ಗಾತ್ರ_t *lenp, loff_t *ppos)
{
	ಪೂರ್ಣಾಂಕ ret;
	ಸ್ಥಿರ DEFINE_MUTEX(mutex);

	mutex_ಬೀಗ(&mutex);
	ret = proc_dointvec(ಕೋಷ್ಟಕ, ಲಿಖ, ಬಫರ್, lenp, ppos);
	/*
	 * Make sure that internally we keep jiffies.
	 * Also, writing zero resets the time-slice to default:
	 */
	ಆದರೆ (!ret && ಲಿಖ) {
		sched_rr_timeslice =
			sysctl_sched_rr_timeslice <= 0 ? RR_TIMESLICE :
			msecs_ಗಮ್ಯ_jiffies(sysctl_sched_rr_timeslice);

		ಆದರೆ (sysctl_sched_rr_timeslice <= 0)
			sysctl_sched_rr_timeslice = jiffies_ಗಮ್ಯ_msecs(RR_TIMESLICE);
	}
	mutex_unlock(&mutex);

	ಹಿಂತಿರುಗಿ ret;
}
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_SYSCTL */

ಶೂನ್ಯ print_rt_stats(ರಚನೆ seq_file *m, ಪೂರ್ಣಾಂಕ cpu)
{
	rt_rq_iter_t iter;
	ರಚನೆ rt_rq *rt_rq;

	rcu_read_ಬೀಗ();
	for_each_rt_rq(rt_rq, iter, cpu_rq(cpu))
		print_rt_rq(m, cpu, rt_rq);
	rcu_read_unlock();
}
