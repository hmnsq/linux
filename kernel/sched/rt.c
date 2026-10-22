// SPDX-License-Identifier: GPL-2.0
/*
 * Real-Time Scheduling Class (mapped to the SCHED_FIFO and SCHED_RR
 * policies)
 */

#ಅನ್ತರ್ಭಾವ "sched.h"
#ಅನ್ತರ್ಭಾವ "pelt.h"

ಪೂರ್ಣಾಙ್ಕ sched_rr_timeslice = RR_TIMESLICE;
/* More than 4 hours if BW_SHIFT equals 20. */
ಸ್ಥಿರ ಧ್ರುವ u64 max_rt_runtime = MAX_BW;

/*
 * period over which we measure -rt task CPU usage in us.
 * default: 1s
 */
ಪೂರ್ಣಾಙ್ಕ sysctl_sched_rt_period = 1000000;

/*
 * part of the period that we allow rt tasks to run in us.
 * default: 1s
 */
ಪೂರ್ಣಾಙ್ಕ sysctl_sched_rt_runtime = 1000000;

#ifdef CONFIG_SYSCTL
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sysctl_sched_rr_timeslice = (MSEC_PER_SEC * RR_TIMESLICE) / HZ;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sched_rt_handler(ಧ್ರುವ ಸಂರಚನಾ ctl_table *table, ಪೂರ್ಣಾಙ್ಕ ಲಿಖ, ಶೂನ್ಯ *buffer,
		size_t *lenp, loff_t *ppos);
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sched_rr_handler(ಧ್ರುವ ಸಂರಚನಾ ctl_table *table, ಪೂರ್ಣಾಙ್ಕ ಲಿಖ, ಶೂನ್ಯ *buffer,
		size_t *lenp, loff_t *ppos);
ಸ್ಥಿರ ಧ್ರುವ ಸಂರಚನಾ ctl_table sched_rt_sysctls[] = {
	{
		.procname       = "sched_rt_period_us",
		.data           = &sysctl_sched_rt_period,
		.maxlen         = ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ),
		.mode           = 0644,
		.proc_handler   = sched_rt_handler,
		.extra1         = SYSCTL_ONE,
		.extra2         = SYSCTL_INT_MAX,
	},
	{
		.procname       = "sched_rt_runtime_us",
		.data           = &sysctl_sched_rt_runtime,
		.maxlen         = ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ),
		.mode           = 0644,
		.proc_handler   = sched_rt_handler,
		.extra1         = SYSCTL_NEG_ONE,
		.extra2         = (ಶೂನ್ಯ *)&sysctl_sched_rt_period,
	},
	{
		.procname       = "sched_rr_timeslice_ms",
		.data           = &sysctl_sched_rr_timeslice,
		.maxlen         = ಪರಿಮಾಣ(ಪೂರ್ಣಾಙ್ಕ),
		.mode           = 0644,
		.proc_handler   = sched_rr_handler,
	},
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ __init sched_rt_sysctl_init(ಶೂನ್ಯ)
{
	register_sysctl_init("kernel", sched_rt_sysctls);
	ಪ್ರತ್ಯಾಗಮ 0;
}
late_initcall(sched_rt_sysctl_init);
#endif /* CONFIG_SYSCTL */

ಶೂನ್ಯ init_rt_rq(ಸಂರಚನಾ rt_rq *rt_rq)
{
	ಸಂರಚನಾ rt_prio_array *array;
	ಪೂರ್ಣಾಙ್ಕ i;

	array = &rt_rq->active;
	ಪ್ರತಿ (i = 0; i < MAX_RT_PRIO; i++) {
		INIT_LIST_HEAD(array->queue + i);
		__clear_bit(i, array->bitmap);
	}
	/* delimiter for bitsearch: */
	__set_bit(MAX_RT_PRIO, array->bitmap);

	rt_rq->highest_prio.curr = MAX_RT_PRIO-1;
	rt_rq->highest_prio.next = MAX_RT_PRIO-1;
	rt_rq->overloaded = 0;
	plist_head_init(&rt_rq->pushable_tasks);
	/* We start is dequeued state, because no RT tasks are queued */
	rt_rq->rt_queued = 0;

#ifdef CONFIG_RT_GROUP_SCHED
	rt_rq->rt_time = 0;
	rt_rq->rt_throttled = 0;
	rt_rq->rt_runtime = 0;
	raw_spin_lock_init(&rt_rq->rt_runtime_lock);
	rt_rq->tg = &ಬೇರು_task_group;
#endif
}

#ifdef CONFIG_RT_GROUP_SCHED

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ do_sched_rt_period_timer(ಸಂರಚನಾ rt_bandwidth *rt_b, ಪೂರ್ಣಾಙ್ಕ overrun);

ಸ್ಥಿರ ಗಣನಾ hrtimer_restart sched_rt_period_timer(ಸಂರಚನಾ hrtimer *timer)
{
	ಸಂರಚನಾ rt_bandwidth *rt_b =
		container_of(timer, ಸಂರಚನಾ rt_bandwidth, rt_period_timer);
	ಪೂರ್ಣಾಙ್ಕ idle = 0;
	ಪೂರ್ಣಾಙ್ಕ overrun;

	raw_spin_lock(&rt_b->rt_runtime_lock);
	ಪ್ರತಿ (;;) {
		overrun = hrtimer_forward_now(timer, rt_b->rt_period);
		ಯದಿ (!overrun)
			ಭಙ್ಗ;

		raw_spin_unlock(&rt_b->rt_runtime_lock);
		idle = do_sched_rt_period_timer(rt_b, overrun);
		raw_spin_lock(&rt_b->rt_runtime_lock);
	}
	ಯದಿ (idle)
		rt_b->rt_period_active = 0;
	raw_spin_unlock(&rt_b->rt_runtime_lock);

	ಪ್ರತ್ಯಾಗಮ idle ? HRTIMER_NORESTART : HRTIMER_RESTART;
}

ಶೂನ್ಯ init_rt_bandwidth(ಸಂರಚನಾ rt_bandwidth *rt_b, u64 period, u64 runtime)
{
	rt_b->rt_period = ns_to_ktime(period);
	rt_b->rt_runtime = runtime;

	raw_spin_lock_init(&rt_b->rt_runtime_lock);

	hrtimer_setup(&rt_b->rt_period_timer, sched_rt_period_timer, CLOCK_MONOTONIC,
		      HRTIMER_MODE_REL_HARD);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ do_start_rt_bandwidth(ಸಂರಚನಾ rt_bandwidth *rt_b)
{
	raw_spin_lock(&rt_b->rt_runtime_lock);
	ಯದಿ (!rt_b->rt_period_active) {
		rt_b->rt_period_active = 1;
		/*
		 * SCHED_DEADLINE updates the bandwidth, as a run away
		 * RT task with a DL task could hog a CPU. But DL does
		 * not reset the period. If a deadline task was running
		 * without an RT task running, it can cause RT tasks to
		 * throttle when they start up. Kick the timer right away
		 * to update the period.
		 */
		hrtimer_forward_now(&rt_b->rt_period_timer, ns_to_ktime(0));
		hrtimer_start_expires(&rt_b->rt_period_timer,
				      HRTIMER_MODE_ABS_PINNED_HARD);
	}
	raw_spin_unlock(&rt_b->rt_runtime_lock);
}

ಸ್ಥಿರ ಶೂನ್ಯ start_rt_bandwidth(ಸಂರಚನಾ rt_bandwidth *rt_b)
{
	ಯದಿ (!rt_bandwidth_enabled() || rt_b->rt_runtime == RUNTIME_INF)
		ಪ್ರತ್ಯಾಗಮ;

	do_start_rt_bandwidth(rt_b);
}

ಸ್ಥಿರ ಶೂನ್ಯ destroy_rt_bandwidth(ಸಂರಚನಾ rt_bandwidth *rt_b)
{
	hrtimer_cancel(&rt_b->rt_period_timer);
}

#define rt_entity_is_task(rt_se) (!(rt_se)->my_q)

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ task_struct *rt_task_of(ಸಂರಚನಾ sched_rt_entity *rt_se)
{
	WARN_ON_ONCE(!rt_entity_is_task(rt_se));

	ಪ್ರತ್ಯಾಗಮ container_of(rt_se, ಸಂರಚನಾ task_struct, rt);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ rq *rq_of_rt_rq(ಸಂರಚನಾ rt_rq *rt_rq)
{
	/* Cannot fold with non-CONFIG_RT_GROUP_SCHED version, layout */
	WARN_ON(!rt_group_sched_enabled() && rt_rq->tg != &ಬೇರು_task_group);
	ಪ್ರತ್ಯಾಗಮ rt_rq->rq;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ rt_rq *rt_rq_of_se(ಸಂರಚನಾ sched_rt_entity *rt_se)
{
	WARN_ON(!rt_group_sched_enabled() && rt_se->rt_rq->tg != &ಬೇರು_task_group);
	ಪ್ರತ್ಯಾಗಮ rt_se->rt_rq;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ rq *rq_of_rt_se(ಸಂರಚನಾ sched_rt_entity *rt_se)
{
	ಸಂರಚನಾ rt_rq *rt_rq = rt_se->rt_rq;

	WARN_ON(!rt_group_sched_enabled() && rt_rq->tg != &ಬೇರು_task_group);
	ಪ್ರತ್ಯಾಗಮ rt_rq->rq;
}

ಶೂನ್ಯ unregister_rt_sched_group(ಸಂರಚನಾ task_group *tg)
{
	ಯದಿ (!rt_group_sched_enabled())
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (tg->rt_se)
		destroy_rt_bandwidth(&tg->rt_bandwidth);
}

ಶೂನ್ಯ free_rt_sched_group(ಸಂರಚನಾ task_group *tg)
{
	ಪೂರ್ಣಾಙ್ಕ i;

	ಯದಿ (!rt_group_sched_enabled())
		ಪ್ರತ್ಯಾಗಮ;

	for_each_possible_cpu(i) {
		ಯದಿ (tg->rt_rq)
			kfree(tg->rt_rq[i]);
		ಯದಿ (tg->rt_se)
			kfree(tg->rt_se[i]);
	}

	kfree(tg->rt_rq);
	kfree(tg->rt_se);
}

ಶೂನ್ಯ init_tg_rt_entry(ಸಂರಚನಾ task_group *tg, ಸಂರಚನಾ rt_rq *rt_rq,
		ಸಂರಚನಾ sched_rt_entity *rt_se, ಪೂರ್ಣಾಙ್ಕ cpu,
		ಸಂರಚನಾ sched_rt_entity *parent)
{
	ಸಂರಚನಾ rq *rq = cpu_rq(cpu);

	rt_rq->highest_prio.curr = MAX_RT_PRIO-1;
	rt_rq->rt_nr_boosted = 0;
	rt_rq->rq = rq;
	rt_rq->tg = tg;

	tg->rt_rq[cpu] = rt_rq;
	tg->rt_se[cpu] = rt_se;

	ಯದಿ (!rt_se)
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (!parent)
		rt_se->rt_rq = &rq->rt;
	ಅನ್ಯಥಾ
		rt_se->rt_rq = parent->my_q;

	rt_se->my_q = rt_rq;
	rt_se->parent = parent;
	INIT_LIST_HEAD(&rt_se->run_list);
}

ಪೂರ್ಣಾಙ್ಕ alloc_rt_sched_group(ಸಂರಚನಾ task_group *tg, ಸಂರಚನಾ task_group *parent)
{
	ಸಂರಚನಾ rt_rq *rt_rq;
	ಸಂರಚನಾ sched_rt_entity *rt_se;
	ಪೂರ್ಣಾಙ್ಕ i;

	ಯದಿ (!rt_group_sched_enabled())
		ಪ್ರತ್ಯಾಗಮ 1;

	tg->rt_rq = kzalloc_objs(rt_rq, nr_cpu_ids);
	ಯದಿ (!tg->rt_rq)
		ಗಚ್ಛ err;
	tg->rt_se = kzalloc_objs(rt_se, nr_cpu_ids);
	ಯದಿ (!tg->rt_se)
		ಗಚ್ಛ err;

	init_rt_bandwidth(&tg->rt_bandwidth, ktime_to_ns(global_rt_period()), 0);

	for_each_possible_cpu(i) {
		rt_rq = kzalloc_ಗಂಟು(ಪರಿಮಾಣ(ಸಂರಚನಾ rt_rq),
				     GFP_KERNEL, cpu_to_ಗಂಟು(i));
		ಯದಿ (!rt_rq)
			ಗಚ್ಛ err;

		rt_se = kzalloc_ಗಂಟು(ಪರಿಮಾಣ(ಸಂರಚನಾ sched_rt_entity),
				     GFP_KERNEL, cpu_to_ಗಂಟು(i));
		ಯದಿ (!rt_se)
			ಗಚ್ಛ err_free_rq;

		init_rt_rq(rt_rq);
		rt_rq->rt_runtime = tg->rt_bandwidth.rt_runtime;
		init_tg_rt_entry(tg, rt_rq, rt_se, i, parent->rt_se[i]);
	}

	ಪ್ರತ್ಯಾಗಮ 1;

err_free_rq:
	kfree(rt_rq);
err:
	ಪ್ರತ್ಯಾಗಮ 0;
}

#ಅನ್ಯಥಾ /* !CONFIG_RT_GROUP_SCHED: */

#define rt_entity_is_task(rt_se) (1)

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ task_struct *rt_task_of(ಸಂರಚನಾ sched_rt_entity *rt_se)
{
	ಪ್ರತ್ಯಾಗಮ container_of(rt_se, ಸಂರಚನಾ task_struct, rt);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ rq *rq_of_rt_rq(ಸಂರಚನಾ rt_rq *rt_rq)
{
	ಪ್ರತ್ಯಾಗಮ container_of(rt_rq, ಸಂರಚನಾ rq, rt);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ rq *rq_of_rt_se(ಸಂರಚನಾ sched_rt_entity *rt_se)
{
	ಸಂರಚನಾ task_struct *p = rt_task_of(rt_se);

	ಪ್ರತ್ಯಾಗಮ task_rq(p);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ rt_rq *rt_rq_of_se(ಸಂರಚನಾ sched_rt_entity *rt_se)
{
	ಸಂರಚನಾ rq *rq = rq_of_rt_se(rt_se);

	ಪ್ರತ್ಯಾಗಮ &rq->rt;
}

ಶೂನ್ಯ unregister_rt_sched_group(ಸಂರಚನಾ task_group *tg) { }

ಶೂನ್ಯ free_rt_sched_group(ಸಂರಚನಾ task_group *tg) { }

ಪೂರ್ಣಾಙ್ಕ alloc_rt_sched_group(ಸಂರಚನಾ task_group *tg, ಸಂರಚನಾ task_group *parent)
{
	ಪ್ರತ್ಯಾಗಮ 1;
}
#endif /* !CONFIG_RT_GROUP_SCHED */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool need_pull_rt_task(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *prev)
{
	/* Try to pull RT tasks here if we lower this rq's prio */
	ಪ್ರತ್ಯಾಗಮ rq->online && rq->rt.highest_prio.curr > prev->prio;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ rt_overloaded(ಸಂರಚನಾ rq *rq)
{
	ಪ್ರತ್ಯಾಗಮ atomic_read(&rq->rd->rto_ಎಣಿಕೆ);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ rt_set_overload(ಸಂರಚನಾ rq *rq)
{
	ಯದಿ (!rq->online)
		ಪ್ರತ್ಯಾಗಮ;

	cpumask_set_cpu(rq->cpu, rq->rd->rto_mask);
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

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ rt_clear_overload(ಸಂರಚನಾ rq *rq)
{
	ಯದಿ (!rq->online)
		ಪ್ರತ್ಯಾಗಮ;

	/* the order here really doesn't matter */
	atomic_dec(&rq->rd->rto_ಎಣಿಕೆ);
	cpumask_clear_cpu(rq->cpu, rq->rd->rto_mask);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ has_pushable_tasks(ಸಂರಚನಾ rq *rq)
{
	ಪ್ರತ್ಯಾಗಮ !plist_head_empty(&rq->rt.pushable_tasks);
}

ಸ್ಥಿರ DEFINE_PER_CPU(ಸಂರಚನಾ balance_callback, rt_ತಳ್ಳು_head);
ಸ್ಥಿರ DEFINE_PER_CPU(ಸಂರಚನಾ balance_callback, rt_pull_head);

ಸ್ಥಿರ ಶೂನ್ಯ ತಳ್ಳು_rt_tasks(ಸಂರಚನಾ rq *);
ಸ್ಥಿರ ಶೂನ್ಯ pull_rt_task(ಸಂರಚನಾ rq *);

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ rt_queue_ತಳ್ಳು_tasks(ಸಂರಚನಾ rq *rq)
{
	ಯದಿ (!has_pushable_tasks(rq))
		ಪ್ರತ್ಯಾಗಮ;

	queue_balance_callback(rq, &per_cpu(rt_ತಳ್ಳು_head, rq->cpu), ತಳ್ಳು_rt_tasks);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ rt_queue_pull_task(ಸಂರಚನಾ rq *rq)
{
	queue_balance_callback(rq, &per_cpu(rt_pull_head, rq->cpu), pull_rt_task);
}

ಸ್ಥಿರ ಶೂನ್ಯ enqueue_pushable_task(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p)
{
	plist_del(&p->pushable_tasks, &rq->rt.pushable_tasks);
	plist_ಗಂಟು_init(&p->pushable_tasks, p->prio);
	plist_add(&p->pushable_tasks, &rq->rt.pushable_tasks);

	/* Update the highest prio pushable task */
	ಯದಿ (p->prio < rq->rt.highest_prio.next)
		rq->rt.highest_prio.next = p->prio;

	ಯದಿ (!rq->rt.overloaded) {
		rt_set_overload(rq);
		rq->rt.overloaded = 1;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ dequeue_pushable_task(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p)
{
	plist_del(&p->pushable_tasks, &rq->rt.pushable_tasks);

	/* Update the new highest prio pushable task */
	ಯದಿ (has_pushable_tasks(rq)) {
		p = plist_first_entry(&rq->rt.pushable_tasks,
				      ಸಂರಚನಾ task_struct, pushable_tasks);
		rq->rt.highest_prio.next = p->prio;
	} ಅನ್ಯಥಾ {
		rq->rt.highest_prio.next = MAX_RT_PRIO-1;

		ಯದಿ (rq->rt.overloaded) {
			rt_clear_overload(rq);
			rq->rt.overloaded = 0;
		}
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ enqueue_ತುದಿ_rt_rq(ಸಂರಚನಾ rt_rq *rt_rq);
ಸ್ಥಿರ ಶೂನ್ಯ dequeue_ತುದಿ_rt_rq(ಸಂರಚನಾ rt_rq *rt_rq, unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ);

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ on_rt_rq(ಸಂರಚನಾ sched_rt_entity *rt_se)
{
	ಪ್ರತ್ಯಾಗಮ rt_se->on_rq;
}

#ifdef CONFIG_UCLAMP_TASK
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
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool rt_task_fits_capacity(ಸಂರಚನಾ task_struct *p, ಪೂರ್ಣಾಙ್ಕ cpu)
{
	unsigned ಪೂರ್ಣಾಙ್ಕ min_cap;
	unsigned ಪೂರ್ಣಾಙ್ಕ max_cap;
	unsigned ಪೂರ್ಣಾಙ್ಕ cpu_cap;

	/* Only heterogeneous systems can benefit from this check */
	ಯದಿ (!sched_asym_cpucap_active())
		ಪ್ರತ್ಯಾಗಮ true;

	min_cap = uclamp_eff_ಮೌಲ್ಯ(p, UCLAMP_MIN);
	max_cap = uclamp_eff_ಮೌಲ್ಯ(p, UCLAMP_MAX);

	cpu_cap = arch_scale_cpu_capacity(cpu);

	ಪ್ರತ್ಯಾಗಮ cpu_cap >= min(min_cap, max_cap);
}
#ಅನ್ಯಥಾ /* !CONFIG_UCLAMP_TASK: */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool rt_task_fits_capacity(ಸಂರಚನಾ task_struct *p, ಪೂರ್ಣಾಙ್ಕ cpu)
{
	ಪ್ರತ್ಯಾಗಮ true;
}
#endif /* !CONFIG_UCLAMP_TASK */

#ifdef CONFIG_RT_GROUP_SCHED

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ u64 sched_rt_runtime(ಸಂರಚನಾ rt_rq *rt_rq)
{
	ಪ್ರತ್ಯಾಗಮ rt_rq->rt_runtime;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ u64 sched_rt_period(ಸಂರಚನಾ rt_rq *rt_rq)
{
	ಪ್ರತ್ಯಾಗಮ ktime_to_ns(rt_rq->tg->rt_bandwidth.rt_period);
}

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ task_group *rt_rq_iter_t;

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ task_group *next_task_group(ಸಂರಚನಾ task_group *tg)
{
	ಯದಿ (!rt_group_sched_enabled()) {
		WARN_ON(tg != &ಬೇರು_task_group);
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
	}

	ಕುರು {
		tg = list_entry_rcu(tg->list.next,
			typeof(ಸಂರಚನಾ task_group), list);
	} ಯಾವತ್ (&tg->list != &task_groups && task_group_is_autogroup(tg));

	ಯದಿ (&tg->list == &task_groups)
		tg = ರಿಕ್ತ;

	ಪ್ರತ್ಯಾಗಮ tg;
}

#define for_each_rt_rq(rt_rq, iter, rq)					\
	ಪ್ರತಿ (iter = &ಬೇರು_task_group;					\
		iter && (rt_rq = iter->rt_rq[cpu_of(rq)]);		\
		iter = next_task_group(iter))

#define for_each_sched_rt_entity(rt_se) \
	ಪ್ರತಿ (; rt_se; rt_se = rt_se->parent)

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ rt_rq *group_rt_rq(ಸಂರಚನಾ sched_rt_entity *rt_se)
{
	ಪ್ರತ್ಯಾಗಮ rt_se->my_q;
}

ಸ್ಥಿರ ಶೂನ್ಯ enqueue_rt_entity(ಸಂರಚನಾ sched_rt_entity *rt_se, unsigned ಪೂರ್ಣಾಙ್ಕ flags);
ಸ್ಥಿರ ಶೂನ್ಯ dequeue_rt_entity(ಸಂರಚನಾ sched_rt_entity *rt_se, unsigned ಪೂರ್ಣಾಙ್ಕ flags);

ಸ್ಥಿರ ಶೂನ್ಯ sched_rt_rq_enqueue(ಸಂರಚನಾ rt_rq *rt_rq)
{
	ಸಂರಚನಾ task_struct *donor = rq_of_rt_rq(rt_rq)->donor;
	ಸಂರಚನಾ rq *rq = rq_of_rt_rq(rt_rq);
	ಸಂರಚನಾ sched_rt_entity *rt_se;

	ಪೂರ್ಣಾಙ್ಕ cpu = cpu_of(rq);

	rt_se = rt_rq->tg->rt_se[cpu];

	ಯದಿ (rt_rq->rt_nr_running) {
		ಯದಿ (!rt_se)
			enqueue_ತುದಿ_rt_rq(rt_rq);
		ಅನ್ಯಥಾ ಯದಿ (!on_rt_rq(rt_se))
			enqueue_rt_entity(rt_se, 0);

		ಯದಿ (rt_rq->highest_prio.curr < donor->prio)
			resched_curr(rq);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ sched_rt_rq_dequeue(ಸಂರಚನಾ rt_rq *rt_rq)
{
	ಸಂರಚನಾ sched_rt_entity *rt_se;
	ಪೂರ್ಣಾಙ್ಕ cpu = cpu_of(rq_of_rt_rq(rt_rq));

	rt_se = rt_rq->tg->rt_se[cpu];

	ಯದಿ (!rt_se) {
		dequeue_ತುದಿ_rt_rq(rt_rq, rt_rq->rt_nr_running);
		/* Kick cpufreq (see the comment in kernel/sched/sched.h). */
		cpufreq_update_util(rq_of_rt_rq(rt_rq), 0);
	}
	ಅನ್ಯಥಾ ಯದಿ (on_rt_rq(rt_se))
		dequeue_rt_entity(rt_se, 0);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ rt_rq_throttled(ಸಂರಚನಾ rt_rq *rt_rq)
{
	ಪ್ರತ್ಯಾಗಮ rt_rq->rt_throttled && !rt_rq->rt_nr_boosted;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ rt_se_boosted(ಸಂರಚನಾ sched_rt_entity *rt_se)
{
	ಸಂರಚನಾ rt_rq *rt_rq = group_rt_rq(rt_se);
	ಸಂರಚನಾ task_struct *p;

	ಯದಿ (rt_rq)
		ಪ್ರತ್ಯಾಗಮ !!rt_rq->rt_nr_boosted;

	p = rt_task_of(rt_se);
	ಪ್ರತ್ಯಾಗಮ p->prio != p->normal_prio;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಧ್ರುವ ಸಂರಚನಾ cpumask *sched_rt_period_mask(ಶೂನ್ಯ)
{
	ಪ್ರತ್ಯಾಗಮ this_rq()->rd->span;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ
ಸಂರಚನಾ rt_rq *sched_rt_period_rt_rq(ಸಂರಚನಾ rt_bandwidth *rt_b, ಪೂರ್ಣಾಙ್ಕ cpu)
{
	ಪ್ರತ್ಯಾಗಮ container_of(rt_b, ಸಂರಚನಾ task_group, rt_bandwidth)->rt_rq[cpu];
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ rt_bandwidth *sched_rt_bandwidth(ಸಂರಚನಾ rt_rq *rt_rq)
{
	ಪ್ರತ್ಯಾಗಮ &rt_rq->tg->rt_bandwidth;
}

bool sched_rt_bandwidth_account(ಸಂರಚನಾ rt_rq *rt_rq)
{
	ಸಂರಚನಾ rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);

	ಪ್ರತ್ಯಾಗಮ (hrtimer_active(&rt_b->rt_period_timer) ||
		rt_rq->rt_time < rt_b->rt_runtime);
}

/*
 * We ran out of runtime, see if we can borrow some from our neighbours.
 */
ಸ್ಥಿರ ಶೂನ್ಯ do_balance_runtime(ಸಂರಚನಾ rt_rq *rt_rq)
{
	ಸಂರಚನಾ rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);
	ಸಂರಚನಾ ಬೇರು_domain *rd = rq_of_rt_rq(rt_rq)->rd;
	ಪೂರ್ಣಾಙ್ಕ i, weight;
	u64 rt_period;

	weight = cpumask_weight(rd->span);

	raw_spin_lock(&rt_b->rt_runtime_lock);
	rt_period = ktime_to_ns(rt_b->rt_period);
	for_each_cpu(i, rd->span) {
		ಸಂರಚನಾ rt_rq *iter = sched_rt_period_rt_rq(rt_b, i);
		s64 diff;

		ಯದಿ (iter == rt_rq)
			ಅನುವರ್ತ;

		raw_spin_lock(&iter->rt_runtime_lock);
		/*
		 * Either all rqs have inf runtime and there's nothing to steal
		 * or __disable_runtime() below sets a specific rq to inf to
		 * indicate its been disabled and disallow stealing.
		 */
		ಯದಿ (iter->rt_runtime == RUNTIME_INF)
			ಗಚ್ಛ next;

		/*
		 * From runqueues with spare time, take 1/n part of their
		 * spare time, but no more than our period.
		 */
		diff = iter->rt_runtime - iter->rt_time;
		ಯದಿ (diff > 0) {
			diff = div_u64((u64)diff, weight);
			ಯದಿ (rt_rq->rt_runtime + diff > rt_period)
				diff = rt_period - rt_rq->rt_runtime;
			iter->rt_runtime -= diff;
			rt_rq->rt_runtime += diff;
			ಯದಿ (rt_rq->rt_runtime == rt_period) {
				raw_spin_unlock(&iter->rt_runtime_lock);
				ಭಙ್ಗ;
			}
		}
next:
		raw_spin_unlock(&iter->rt_runtime_lock);
	}
	raw_spin_unlock(&rt_b->rt_runtime_lock);
}

/*
 * Ensure this RQ takes back all the runtime it lend to its neighbours.
 */
ಸ್ಥಿರ ಶೂನ್ಯ __disable_runtime(ಸಂರಚನಾ rq *rq)
{
	ಸಂರಚನಾ ಬೇರು_domain *rd = rq->rd;
	rt_rq_iter_t iter;
	ಸಂರಚನಾ rt_rq *rt_rq;

	ಯದಿ (unlikely(!scheduler_running))
		ಪ್ರತ್ಯಾಗಮ;

	for_each_rt_rq(rt_rq, iter, rq) {
		ಸಂರಚನಾ rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);
		s64 want;
		ಪೂರ್ಣಾಙ್ಕ i;

		raw_spin_lock(&rt_b->rt_runtime_lock);
		raw_spin_lock(&rt_rq->rt_runtime_lock);
		/*
		 * Either we're all inf and nobody needs to borrow, or we're
		 * already disabled and thus have nothing to do, or we have
		 * exactly the right amount of runtime to take out.
		 */
		ಯದಿ (rt_rq->rt_runtime == RUNTIME_INF ||
				rt_rq->rt_runtime == rt_b->rt_runtime)
			ಗಚ್ಛ balanced;
		raw_spin_unlock(&rt_rq->rt_runtime_lock);

		/*
		 * Calculate the difference between what we started out with
		 * and what we current have, that's the amount of runtime
		 * we lend and now have to reclaim.
		 */
		want = rt_b->rt_runtime - rt_rq->rt_runtime;

		/*
		 * Greedy reclaim, take back as much as we can.
		 */
		for_each_cpu(i, rd->span) {
			ಸಂರಚನಾ rt_rq *iter = sched_rt_period_rt_rq(rt_b, i);
			s64 diff;

			/*
			 * Can't reclaim from ourselves or disabled runqueues.
			 */
			ಯದಿ (iter == rt_rq || iter->rt_runtime == RUNTIME_INF)
				ಅನುವರ್ತ;

			raw_spin_lock(&iter->rt_runtime_lock);
			ಯದಿ (want > 0) {
				diff = min_t(s64, iter->rt_runtime, want);
				iter->rt_runtime -= diff;
				want -= diff;
			} ಅನ್ಯಥಾ {
				iter->rt_runtime -= want;
				want -= want;
			}
			raw_spin_unlock(&iter->rt_runtime_lock);

			ಯದಿ (!want)
				ಭಙ್ಗ;
		}

		raw_spin_lock(&rt_rq->rt_runtime_lock);
		/*
		 * We cannot be left wanting - that would mean some runtime
		 * leaked out of the system.
		 */
		WARN_ON_ONCE(want);
balanced:
		/*
		 * Disable all the borrow logic by pretending we have inf
		 * runtime - in which case borrowing doesn't make sense.
		 */
		rt_rq->rt_runtime = RUNTIME_INF;
		rt_rq->rt_throttled = 0;
		raw_spin_unlock(&rt_rq->rt_runtime_lock);
		raw_spin_unlock(&rt_b->rt_runtime_lock);

		/* Make rt_rq available for pick_next_task() */
		sched_rt_rq_enqueue(rt_rq);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ __enable_runtime(ಸಂರಚನಾ rq *rq)
{
	rt_rq_iter_t iter;
	ಸಂರಚನಾ rt_rq *rt_rq;

	ಯದಿ (unlikely(!scheduler_running))
		ಪ್ರತ್ಯಾಗಮ;

	/*
	 * Reset each runqueue's bandwidth settings
	 */
	for_each_rt_rq(rt_rq, iter, rq) {
		ಸಂರಚನಾ rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);

		raw_spin_lock(&rt_b->rt_runtime_lock);
		raw_spin_lock(&rt_rq->rt_runtime_lock);
		rt_rq->rt_runtime = rt_b->rt_runtime;
		rt_rq->rt_time = 0;
		rt_rq->rt_throttled = 0;
		raw_spin_unlock(&rt_rq->rt_runtime_lock);
		raw_spin_unlock(&rt_b->rt_runtime_lock);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ balance_runtime(ಸಂರಚನಾ rt_rq *rt_rq)
{
	ಯದಿ (!sched_feat(RT_RUNTIME_SHARE))
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (rt_rq->rt_time > rt_rq->rt_runtime) {
		raw_spin_unlock(&rt_rq->rt_runtime_lock);
		do_balance_runtime(rt_rq);
		raw_spin_lock(&rt_rq->rt_runtime_lock);
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ do_sched_rt_period_timer(ಸಂರಚನಾ rt_bandwidth *rt_b, ಪೂರ್ಣಾಙ್ಕ overrun)
{
	ಪೂರ್ಣಾಙ್ಕ i, idle = 1, throttled = 0;
	ಧ್ರುವ ಸಂರಚನಾ cpumask *span;

	span = sched_rt_period_mask();

	/*
	 * FIXME: isolated CPUs should really leave the root task group,
	 * whether they are isolcpus or were isolated via cpusets, lest
	 * the timer run on a CPU which does not service all runqueues,
	 * potentially leaving other CPUs indefinitely throttled.  If
	 * isolation is really required, the user will turn the throttle
	 * off to kill the perturbations it causes anyway.  Meanwhile,
	 * this maintains functionality for boot and/or troubleshooting.
	 */
	ಯದಿ (rt_b == &ಬೇರು_task_group.rt_bandwidth)
		span = cpu_online_mask;

	for_each_cpu(i, span) {
		ಪೂರ್ಣಾಙ್ಕ enqueue = 0;
		ಸಂರಚನಾ rt_rq *rt_rq = sched_rt_period_rt_rq(rt_b, i);
		ಸಂರಚನಾ rq *rq = rq_of_rt_rq(rt_rq);
		ಸಂರಚನಾ rq_flags rf;
		ಪೂರ್ಣಾಙ್ಕ skip;

		/*
		 * When span == cpu_online_mask, taking each rq->lock
		 * can be time-consuming. Try to avoid it when possible.
		 */
		raw_spin_lock(&rt_rq->rt_runtime_lock);
		ಯದಿ (!sched_feat(RT_RUNTIME_SHARE) && rt_rq->rt_runtime != RUNTIME_INF)
			rt_rq->rt_runtime = rt_b->rt_runtime;
		skip = !rt_rq->rt_time && !rt_rq->rt_nr_running;
		raw_spin_unlock(&rt_rq->rt_runtime_lock);
		ಯದಿ (skip)
			ಅನುವರ್ತ;

		rq_lock(rq, &rf);
		update_rq_clock(rq);

		ಯದಿ (rt_rq->rt_time) {
			u64 runtime;

			raw_spin_lock(&rt_rq->rt_runtime_lock);
			ಯದಿ (rt_rq->rt_throttled)
				balance_runtime(rt_rq);
			runtime = rt_rq->rt_runtime;
			rt_rq->rt_time -= min(rt_rq->rt_time, overrun*runtime);
			ಯದಿ (rt_rq->rt_throttled && rt_rq->rt_time < runtime) {
				rt_rq->rt_throttled = 0;
				enqueue = 1;

				/*
				 * When we're idle and a woken (rt) task is
				 * throttled wakeup_preempt() will set
				 * skip_update and the time between the wakeup
				 * and this unthrottle will get accounted as
				 * 'runtime'.
				 */
				ಯದಿ (rt_rq->rt_nr_running && rq->curr == rq->idle)
					rq_clock_cancel_skipupdate(rq);
			}
			ಯದಿ (rt_rq->rt_time || rt_rq->rt_nr_running)
				idle = 0;
			raw_spin_unlock(&rt_rq->rt_runtime_lock);
		} ಅನ್ಯಥಾ ಯದಿ (rt_rq->rt_nr_running) {
			idle = 0;
			ಯದಿ (!rt_rq_throttled(rt_rq))
				enqueue = 1;
		}
		ಯದಿ (rt_rq->rt_throttled)
			throttled = 1;

		ಯದಿ (enqueue)
			sched_rt_rq_enqueue(rt_rq);
		rq_unlock(rq, &rf);
	}

	ಯದಿ (!throttled && (!rt_bandwidth_enabled() || rt_b->rt_runtime == RUNTIME_INF))
		ಪ್ರತ್ಯಾಗಮ 1;

	ಪ್ರತ್ಯಾಗಮ idle;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sched_rt_runtime_exceeded(ಸಂರಚನಾ rt_rq *rt_rq)
{
	u64 runtime = sched_rt_runtime(rt_rq);

	ಯದಿ (rt_rq->rt_throttled)
		ಪ್ರತ್ಯಾಗಮ rt_rq_throttled(rt_rq);

	ಯದಿ (runtime >= sched_rt_period(rt_rq))
		ಪ್ರತ್ಯಾಗಮ 0;

	balance_runtime(rt_rq);
	runtime = sched_rt_runtime(rt_rq);
	ಯದಿ (runtime == RUNTIME_INF)
		ಪ್ರತ್ಯಾಗಮ 0;

	ಯದಿ (rt_rq->rt_time > runtime) {
		ಸಂರಚನಾ rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);

		/*
		 * Don't actually throttle groups that have no runtime assigned
		 * but accrue some time due to boosting.
		 */
		ಯದಿ (likely(rt_b->rt_runtime)) {
			rt_rq->rt_throttled = 1;
			printk_deferred_once("sched: RT throttling activated\n");
		} ಅನ್ಯಥಾ {
			/*
			 * In case we did anyway, make it go away,
			 * replenishment is a joke, since it will replenish us
			 * with exactly 0 ns.
			 */
			rt_rq->rt_time = 0;
		}

		ಯದಿ (rt_rq_throttled(rt_rq)) {
			sched_rt_rq_dequeue(rt_rq);
			ಪ್ರತ್ಯಾಗಮ 1;
		}
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}

#ಅನ್ಯಥಾ /* !CONFIG_RT_GROUP_SCHED: */

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ rt_rq *rt_rq_iter_t;

#define for_each_rt_rq(rt_rq, iter, rq) \
	ಪ್ರತಿ ((ಶೂನ್ಯ) iter, rt_rq = &rq->rt; rt_rq; rt_rq = ರಿಕ್ತ)

#define for_each_sched_rt_entity(rt_se) \
	ಪ್ರತಿ (; rt_se; rt_se = ರಿಕ್ತ)

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ rt_rq *group_rt_rq(ಸಂರಚನಾ sched_rt_entity *rt_se)
{
	ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ sched_rt_rq_enqueue(ಸಂರಚನಾ rt_rq *rt_rq)
{
	ಸಂರಚನಾ rq *rq = rq_of_rt_rq(rt_rq);

	ಯದಿ (!rt_rq->rt_nr_running)
		ಪ್ರತ್ಯಾಗಮ;

	enqueue_ತುದಿ_rt_rq(rt_rq);
	resched_curr(rq);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ sched_rt_rq_dequeue(ಸಂರಚನಾ rt_rq *rt_rq)
{
	dequeue_ತುದಿ_rt_rq(rt_rq, rt_rq->rt_nr_running);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ rt_rq_throttled(ಸಂರಚನಾ rt_rq *rt_rq)
{
	ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಧ್ರುವ ಸಂರಚನಾ cpumask *sched_rt_period_mask(ಶೂನ್ಯ)
{
	ಪ್ರತ್ಯಾಗಮ cpu_online_mask;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ
ಸಂರಚನಾ rt_rq *sched_rt_period_rt_rq(ಸಂರಚನಾ rt_bandwidth *rt_b, ಪೂರ್ಣಾಙ್ಕ cpu)
{
	ಪ್ರತ್ಯಾಗಮ &cpu_rq(cpu)->rt;
}

ಸ್ಥಿರ ಶೂನ್ಯ __enable_runtime(ಸಂರಚನಾ rq *rq) { }
ಸ್ಥಿರ ಶೂನ್ಯ __disable_runtime(ಸಂರಚನಾ rq *rq) { }

#endif /* !CONFIG_RT_GROUP_SCHED */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ rt_se_prio(ಸಂರಚನಾ sched_rt_entity *rt_se)
{
#ifdef CONFIG_RT_GROUP_SCHED
	ಸಂರಚನಾ rt_rq *rt_rq = group_rt_rq(rt_se);

	ಯದಿ (rt_rq)
		ಪ್ರತ್ಯಾಗಮ rt_rq->highest_prio.curr;
#endif

	ಪ್ರತ್ಯಾಗಮ rt_task_of(rt_se)->prio;
}

/*
 * Update the current task's runtime statistics. Skip current tasks that
 * are not in our scheduling class.
 */
ಸ್ಥಿರ ಶೂನ್ಯ update_curr_rt(ಸಂರಚನಾ rq *rq)
{
	ಸಂರಚನಾ task_struct *donor = rq->donor;
	s64 delta_exec;

	ಯದಿ (donor->sched_class != &rt_sched_class)
		ಪ್ರತ್ಯಾಗಮ;

	delta_exec = update_curr_common(rq);
	ಯದಿ (unlikely(delta_exec <= 0))
		ಪ್ರತ್ಯಾಗಮ;

#ifdef CONFIG_RT_GROUP_SCHED
	ಸಂರಚನಾ sched_rt_entity *rt_se = &donor->rt;

	ಯದಿ (!rt_bandwidth_enabled())
		ಪ್ರತ್ಯಾಗಮ;

	for_each_sched_rt_entity(rt_se) {
		ಸಂರಚನಾ rt_rq *rt_rq = rt_rq_of_se(rt_se);
		ಪೂರ್ಣಾಙ್ಕ exceeded;

		ಯದಿ (sched_rt_runtime(rt_rq) != RUNTIME_INF) {
			raw_spin_lock(&rt_rq->rt_runtime_lock);
			rt_rq->rt_time += delta_exec;
			exceeded = sched_rt_runtime_exceeded(rt_rq);
			ಯದಿ (exceeded)
				resched_curr(rq);
			raw_spin_unlock(&rt_rq->rt_runtime_lock);
			ಯದಿ (exceeded)
				do_start_rt_bandwidth(sched_rt_bandwidth(rt_rq));
		}
	}
#endif /* CONFIG_RT_GROUP_SCHED */
}

ಸ್ಥಿರ ಶೂನ್ಯ
dequeue_ತುದಿ_rt_rq(ಸಂರಚನಾ rt_rq *rt_rq, unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ)
{
	ಸಂರಚನಾ rq *rq = rq_of_rt_rq(rt_rq);

	BUG_ON(&rq->rt != rt_rq);

	ಯದಿ (!rt_rq->rt_queued)
		ಪ್ರತ್ಯಾಗಮ;

	BUG_ON(!rq->nr_running);

	sub_nr_running(rq, ಎಣಿಕೆ);
	rt_rq->rt_queued = 0;

}

ಸ್ಥಿರ ಶೂನ್ಯ
enqueue_ತುದಿ_rt_rq(ಸಂರಚನಾ rt_rq *rt_rq)
{
	ಸಂರಚನಾ rq *rq = rq_of_rt_rq(rt_rq);

	BUG_ON(&rq->rt != rt_rq);

	ಯದಿ (rt_rq->rt_queued)
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (rt_rq_throttled(rt_rq))
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (rt_rq->rt_nr_running) {
		add_nr_running(rq, rt_rq->rt_nr_running);
		rt_rq->rt_queued = 1;
	}

	/* Kick cpufreq (see the comment in kernel/sched/sched.h). */
	cpufreq_update_util(rq, 0);
}

ಸ್ಥಿರ ಶೂನ್ಯ
inc_rt_prio_smp(ಸಂರಚನಾ rt_rq *rt_rq, ಪೂರ್ಣಾಙ್ಕ prio, ಪೂರ್ಣಾಙ್ಕ prev_prio)
{
	ಸಂರಚನಾ rq *rq = rq_of_rt_rq(rt_rq);

	/*
	 * Change rq's cpupri only if rt_rq is the top queue.
	 */
	ಯದಿ (IS_ENABLED(CONFIG_RT_GROUP_SCHED) && &rq->rt != rt_rq)
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (rq->online && prio < prev_prio)
		cpupri_set(&rq->rd->cpupri, rq->cpu, prio);
}

ಸ್ಥಿರ ಶೂನ್ಯ
dec_rt_prio_smp(ಸಂರಚನಾ rt_rq *rt_rq, ಪೂರ್ಣಾಙ್ಕ prio, ಪೂರ್ಣಾಙ್ಕ prev_prio)
{
	ಸಂರಚನಾ rq *rq = rq_of_rt_rq(rt_rq);

	/*
	 * Change rq's cpupri only if rt_rq is the top queue.
	 */
	ಯದಿ (IS_ENABLED(CONFIG_RT_GROUP_SCHED) && &rq->rt != rt_rq)
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (rq->online && rt_rq->highest_prio.curr != prev_prio)
		cpupri_set(&rq->rd->cpupri, rq->cpu, rt_rq->highest_prio.curr);
}

ಸ್ಥಿರ ಶೂನ್ಯ
inc_rt_prio(ಸಂರಚನಾ rt_rq *rt_rq, ಪೂರ್ಣಾಙ್ಕ prio)
{
	ಪೂರ್ಣಾಙ್ಕ prev_prio = rt_rq->highest_prio.curr;

	ಯದಿ (prio < prev_prio)
		rt_rq->highest_prio.curr = prio;

	inc_rt_prio_smp(rt_rq, prio, prev_prio);
}

ಸ್ಥಿರ ಶೂನ್ಯ
dec_rt_prio(ಸಂರಚನಾ rt_rq *rt_rq, ಪೂರ್ಣಾಙ್ಕ prio)
{
	ಪೂರ್ಣಾಙ್ಕ prev_prio = rt_rq->highest_prio.curr;

	ಯದಿ (rt_rq->rt_nr_running) {

		WARN_ON(prio < prev_prio);

		/*
		 * This may have been our highest task, and therefore
		 * we may have some re-computation to do
		 */
		ಯದಿ (prio == prev_prio) {
			ಸಂರಚನಾ rt_prio_array *array = &rt_rq->active;

			rt_rq->highest_prio.curr =
				sched_find_first_bit(array->bitmap);
		}

	} ಅನ್ಯಥಾ {
		rt_rq->highest_prio.curr = MAX_RT_PRIO-1;
	}

	dec_rt_prio_smp(rt_rq, prio, prev_prio);
}

#ifdef CONFIG_RT_GROUP_SCHED

ಸ್ಥಿರ ಶೂನ್ಯ
inc_rt_group(ಸಂರಚನಾ sched_rt_entity *rt_se, ಸಂರಚನಾ rt_rq *rt_rq)
{
	ಯದಿ (rt_se_boosted(rt_se))
		rt_rq->rt_nr_boosted++;

	start_rt_bandwidth(&rt_rq->tg->rt_bandwidth);
}

ಸ್ಥಿರ ಶೂನ್ಯ
dec_rt_group(ಸಂರಚನಾ sched_rt_entity *rt_se, ಸಂರಚನಾ rt_rq *rt_rq)
{
	ಯದಿ (rt_se_boosted(rt_se))
		rt_rq->rt_nr_boosted--;

	WARN_ON(!rt_rq->rt_nr_running && rt_rq->rt_nr_boosted);
}

#ಅನ್ಯಥಾ /* !CONFIG_RT_GROUP_SCHED: */

ಸ್ಥಿರ ಶೂನ್ಯ
inc_rt_group(ಸಂರಚನಾ sched_rt_entity *rt_se, ಸಂರಚನಾ rt_rq *rt_rq)
{
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ
ಶೂನ್ಯ dec_rt_group(ಸಂರಚನಾ sched_rt_entity *rt_se, ಸಂರಚನಾ rt_rq *rt_rq) {}

#endif /* !CONFIG_RT_GROUP_SCHED */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ
unsigned ಪೂರ್ಣಾಙ್ಕ rt_se_nr_running(ಸಂರಚನಾ sched_rt_entity *rt_se)
{
	ಸಂರಚನಾ rt_rq *group_rq = group_rt_rq(rt_se);

	ಯದಿ (group_rq)
		ಪ್ರತ್ಯಾಗಮ group_rq->rt_nr_running;
	ಅನ್ಯಥಾ
		ಪ್ರತ್ಯಾಗಮ 1;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ
unsigned ಪೂರ್ಣಾಙ್ಕ rt_se_rr_nr_running(ಸಂರಚನಾ sched_rt_entity *rt_se)
{
	ಸಂರಚನಾ rt_rq *group_rq = group_rt_rq(rt_se);
	ಸಂರಚನಾ task_struct *tsk;

	ಯದಿ (group_rq)
		ಪ್ರತ್ಯಾಗಮ group_rq->rr_nr_running;

	tsk = rt_task_of(rt_se);

	ಪ್ರತ್ಯಾಗಮ (tsk->policy == SCHED_RR) ? 1 : 0;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ
ಶೂನ್ಯ inc_rt_tasks(ಸಂರಚನಾ sched_rt_entity *rt_se, ಸಂರಚನಾ rt_rq *rt_rq)
{
	ಪೂರ್ಣಾಙ್ಕ prio = rt_se_prio(rt_se);

	WARN_ON(!rt_prio(prio));
	rt_rq->rt_nr_running += rt_se_nr_running(rt_se);
	rt_rq->rr_nr_running += rt_se_rr_nr_running(rt_se);

	inc_rt_prio(rt_rq, prio);
	inc_rt_group(rt_se, rt_rq);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ
ಶೂನ್ಯ dec_rt_tasks(ಸಂರಚನಾ sched_rt_entity *rt_se, ಸಂರಚನಾ rt_rq *rt_rq)
{
	WARN_ON(!rt_prio(rt_se_prio(rt_se)));
	WARN_ON(!rt_rq->rt_nr_running);
	rt_rq->rt_nr_running -= rt_se_nr_running(rt_se);
	rt_rq->rr_nr_running -= rt_se_rr_nr_running(rt_se);

	dec_rt_prio(rt_rq, rt_se_prio(rt_se));
	dec_rt_group(rt_se, rt_rq);
}

/*
 * Change rt_se->run_list location unless SAVE && !MOVE
 *
 * assumes ENQUEUE/DEQUEUE flags match
 */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool move_entity(unsigned ಪೂರ್ಣಾಙ್ಕ flags)
{
	ಯದಿ ((flags & (DEQUEUE_SAVE | DEQUEUE_MOVE)) == DEQUEUE_SAVE)
		ಪ್ರತ್ಯಾಗಮ false;

	ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ ಶೂನ್ಯ __delist_rt_entity(ಸಂರಚನಾ sched_rt_entity *rt_se, ಸಂರಚನಾ rt_prio_array *array)
{
	list_del_init(&rt_se->run_list);

	ಯದಿ (list_empty(array->queue + rt_se_prio(rt_se)))
		__clear_bit(rt_se_prio(rt_se), array->bitmap);

	rt_se->on_list = 0;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ sched_statistics *
__schedstats_from_rt_se(ಸಂರಚನಾ sched_rt_entity *rt_se)
{
	/* schedstats is not supported for rt group. */
	ಯದಿ (!rt_entity_is_task(rt_se))
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

	ಪ್ರತ್ಯಾಗಮ &rt_task_of(rt_se)->stats;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
update_stats_wait_start_rt(ಸಂರಚನಾ rt_rq *rt_rq, ಸಂರಚನಾ sched_rt_entity *rt_se)
{
	ಸಂರಚನಾ sched_statistics *stats;
	ಸಂರಚನಾ task_struct *p = ರಿಕ್ತ;

	ಯದಿ (!schedstat_enabled())
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (rt_entity_is_task(rt_se))
		p = rt_task_of(rt_se);

	stats = __schedstats_from_rt_se(rt_se);
	ಯದಿ (!stats)
		ಪ್ರತ್ಯಾಗಮ;

	__update_stats_wait_start(rq_of_rt_rq(rt_rq), p, stats);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
update_stats_enqueue_sleeper_rt(ಸಂರಚನಾ rt_rq *rt_rq, ಸಂರಚನಾ sched_rt_entity *rt_se)
{
	ಸಂರಚನಾ sched_statistics *stats;
	ಸಂರಚನಾ task_struct *p = ರಿಕ್ತ;

	ಯದಿ (!schedstat_enabled())
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (rt_entity_is_task(rt_se))
		p = rt_task_of(rt_se);

	stats = __schedstats_from_rt_se(rt_se);
	ಯದಿ (!stats)
		ಪ್ರತ್ಯಾಗಮ;

	__update_stats_enqueue_sleeper(rq_of_rt_rq(rt_rq), p, stats);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
update_stats_enqueue_rt(ಸಂರಚನಾ rt_rq *rt_rq, ಸಂರಚನಾ sched_rt_entity *rt_se,
			ಪೂರ್ಣಾಙ್ಕ flags)
{
	ಯದಿ (!schedstat_enabled())
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (flags & ENQUEUE_WAKEUP)
		update_stats_enqueue_sleeper_rt(rt_rq, rt_se);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
update_stats_wait_end_rt(ಸಂರಚನಾ rt_rq *rt_rq, ಸಂರಚನಾ sched_rt_entity *rt_se)
{
	ಸಂರಚನಾ sched_statistics *stats;
	ಸಂರಚನಾ task_struct *p = ರಿಕ್ತ;

	ಯದಿ (!schedstat_enabled())
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (rt_entity_is_task(rt_se))
		p = rt_task_of(rt_se);

	stats = __schedstats_from_rt_se(rt_se);
	ಯದಿ (!stats)
		ಪ್ರತ್ಯಾಗಮ;

	__update_stats_wait_end(rq_of_rt_rq(rt_rq), p, stats);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
update_stats_dequeue_rt(ಸಂರಚನಾ rt_rq *rt_rq, ಸಂರಚನಾ sched_rt_entity *rt_se,
			ಪೂರ್ಣಾಙ್ಕ flags)
{
	ಸಂರಚನಾ task_struct *p = ರಿಕ್ತ;
	ಸಂರಚನಾ rq *rq = rq_of_rt_rq(rt_rq);

	ಯದಿ (!schedstat_enabled())
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (rt_entity_is_task(rt_se)) {
		p = rt_task_of(rt_se);

		ಯದಿ (p != rq->curr)
			update_stats_wait_end_rt(rt_rq, rt_se);
	}

	ಯದಿ ((flags & DEQUEUE_SLEEP) && p) {
		unsigned ಪೂರ್ಣಾಙ್ಕ state;

		state = READ_ONCE(p->__state);
		ಯದಿ (state & TASK_INTERRUPTIBLE)
			__schedstat_set(p->stats.sleep_start,
					rq_clock(rq_of_rt_rq(rt_rq)));

		ಯದಿ (state & TASK_UNINTERRUPTIBLE)
			__schedstat_set(p->stats.block_start,
					rq_clock(rq_of_rt_rq(rt_rq)));
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ __enqueue_rt_entity(ಸಂರಚನಾ sched_rt_entity *rt_se, unsigned ಪೂರ್ಣಾಙ್ಕ flags)
{
	ಸಂರಚನಾ rt_rq *rt_rq = rt_rq_of_se(rt_se);
	ಸಂರಚನಾ rt_prio_array *array = &rt_rq->active;
	ಸಂರಚನಾ rt_rq *group_rq = group_rt_rq(rt_se);
	ಸಂರಚನಾ list_head *queue = array->queue + rt_se_prio(rt_se);

	/*
	 * Don't enqueue the group if its throttled, or when empty.
	 * The latter is a consequence of the former when a child group
	 * get throttled and the current group doesn't have any other
	 * active members.
	 */
	ಯದಿ (group_rq && (rt_rq_throttled(group_rq) || !group_rq->rt_nr_running)) {
		ಯದಿ (rt_se->on_list)
			__delist_rt_entity(rt_se, array);
		ಪ್ರತ್ಯಾಗಮ;
	}

	ಯದಿ (move_entity(flags)) {
		WARN_ON_ONCE(rt_se->on_list);
		ಯದಿ (flags & ENQUEUE_HEAD)
			list_add(&rt_se->run_list, queue);
		ಅನ್ಯಥಾ
			list_add_tail(&rt_se->run_list, queue);

		__set_bit(rt_se_prio(rt_se), array->bitmap);
		rt_se->on_list = 1;
	}
	rt_se->on_rq = 1;

	inc_rt_tasks(rt_se, rt_rq);
}

ಸ್ಥಿರ ಶೂನ್ಯ __dequeue_rt_entity(ಸಂರಚನಾ sched_rt_entity *rt_se, unsigned ಪೂರ್ಣಾಙ್ಕ flags)
{
	ಸಂರಚನಾ rt_rq *rt_rq = rt_rq_of_se(rt_se);
	ಸಂರಚನಾ rt_prio_array *array = &rt_rq->active;

	ಯದಿ (move_entity(flags)) {
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
ಸ್ಥಿರ ಶೂನ್ಯ dequeue_rt_ರಾಶಿ(ಸಂರಚನಾ sched_rt_entity *rt_se, unsigned ಪೂರ್ಣಾಙ್ಕ flags)
{
	ಸಂರಚನಾ sched_rt_entity *back = ರಿಕ್ತ;
	unsigned ಪೂರ್ಣಾಙ್ಕ rt_nr_running;

	for_each_sched_rt_entity(rt_se) {
		rt_se->back = back;
		back = rt_se;
	}

	rt_nr_running = rt_rq_of_se(back)->rt_nr_running;

	ಪ್ರತಿ (rt_se = back; rt_se; rt_se = rt_se->back) {
		ಯದಿ (on_rt_rq(rt_se))
			__dequeue_rt_entity(rt_se, flags);
	}

	dequeue_ತುದಿ_rt_rq(rt_rq_of_se(back), rt_nr_running);
}

ಸ್ಥಿರ ಶೂನ್ಯ enqueue_rt_entity(ಸಂರಚನಾ sched_rt_entity *rt_se, unsigned ಪೂರ್ಣಾಙ್ಕ flags)
{
	ಸಂರಚನಾ rq *rq = rq_of_rt_se(rt_se);

	update_stats_enqueue_rt(rt_rq_of_se(rt_se), rt_se, flags);

	dequeue_rt_ರಾಶಿ(rt_se, flags);
	for_each_sched_rt_entity(rt_se)
		__enqueue_rt_entity(rt_se, flags);
	enqueue_ತುದಿ_rt_rq(&rq->rt);
}

ಸ್ಥಿರ ಶೂನ್ಯ dequeue_rt_entity(ಸಂರಚನಾ sched_rt_entity *rt_se, unsigned ಪೂರ್ಣಾಙ್ಕ flags)
{
	ಸಂರಚನಾ rq *rq = rq_of_rt_se(rt_se);

	update_stats_dequeue_rt(rt_rq_of_se(rt_se), rt_se, flags);

	dequeue_rt_ರಾಶಿ(rt_se, flags);

	for_each_sched_rt_entity(rt_se) {
		ಸಂರಚನಾ rt_rq *rt_rq = group_rt_rq(rt_se);

		ಯದಿ (rt_rq && rt_rq->rt_nr_running)
			__enqueue_rt_entity(rt_se, flags);
	}
	enqueue_ತುದಿ_rt_rq(&rq->rt);
}

/*
 * Adding/removing a task to/from a priority array:
 */
ಸ್ಥಿರ ಶೂನ್ಯ
enqueue_task_rt(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p, ಪೂರ್ಣಾಙ್ಕ flags)
{
	ಸಂರಚನಾ sched_rt_entity *rt_se = &p->rt;

	ಯದಿ (flags & ENQUEUE_WAKEUP)
		rt_se->timeout = 0;

	check_schedstat_required();
	update_stats_wait_start_rt(rt_rq_of_se(rt_se), rt_se);

	enqueue_rt_entity(rt_se, flags);

	ಯದಿ (task_is_blocked(p))
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (!task_current(rq, p) && p->nr_cpus_allowed > 1)
		enqueue_pushable_task(rq, p);
}

ಸ್ಥಿರ bool dequeue_task_rt(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p, ಪೂರ್ಣಾಙ್ಕ flags)
{
	ಸಂರಚನಾ sched_rt_entity *rt_se = &p->rt;

	update_curr_rt(rq);
	dequeue_rt_entity(rt_se, flags);

	dequeue_pushable_task(rq, p);

	ಪ್ರತ್ಯಾಗಮ true;
}

/*
 * Put task to the head or the end of the run list without the overhead of
 * dequeue followed by enqueue.
 */
ಸ್ಥಿರ ಶೂನ್ಯ
requeue_rt_entity(ಸಂರಚನಾ rt_rq *rt_rq, ಸಂರಚನಾ sched_rt_entity *rt_se, ಪೂರ್ಣಾಙ್ಕ head)
{
	ಯದಿ (on_rt_rq(rt_se)) {
		ಸಂರಚನಾ rt_prio_array *array = &rt_rq->active;
		ಸಂರಚನಾ list_head *queue = array->queue + rt_se_prio(rt_se);

		ಯದಿ (head)
			list_move(&rt_se->run_list, queue);
		ಅನ್ಯಥಾ
			list_move_tail(&rt_se->run_list, queue);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ requeue_task_rt(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p, ಪೂರ್ಣಾಙ್ಕ head)
{
	ಸಂರಚನಾ sched_rt_entity *rt_se = &p->rt;
	ಸಂರಚನಾ rt_rq *rt_rq;

	for_each_sched_rt_entity(rt_se) {
		rt_rq = rt_rq_of_se(rt_se);
		requeue_rt_entity(rt_rq, rt_se, head);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ yield_task_rt(ಸಂರಚನಾ rq *rq)
{
	requeue_task_rt(rq, rq->donor, 0);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ find_lowest_rq(ಸಂರಚನಾ task_struct *task);

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
select_task_rq_rt(ಸಂರಚನಾ task_struct *p, ಪೂರ್ಣಾಙ್ಕ cpu, ಪೂರ್ಣಾಙ್ಕ flags)
{
	ಸಂರಚನಾ task_struct *curr, *donor;
	ಸಂರಚನಾ rq *rq;
	bool test;

	/* For anything but wake ups, just return the task_cpu */
	ಯದಿ (!(flags & (WF_TTWU | WF_FORK)))
		ಗಚ್ಛ out;

	rq = cpu_rq(cpu);

	rcu_read_lock();
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

	ಯದಿ (test || !rt_task_fits_capacity(p, cpu)) {
		ಪೂರ್ಣಾಙ್ಕ target = find_lowest_rq(p);

		/*
		 * Bail out if we were forcing a migration to find a better
		 * fitting CPU but our search failed.
		 */
		ಯದಿ (!test && target != -1 && !rt_task_fits_capacity(p, target))
			ಗಚ್ಛ out_unlock;

		/*
		 * Don't bother moving it if the destination CPU is
		 * not running a lower priority task.
		 */
		ಯದಿ (target != -1 &&
		    p->prio < cpu_rq(target)->rt.highest_prio.curr)
			cpu = target;
	}

out_unlock:
	rcu_read_unlock();

out:
	ಪ್ರತ್ಯಾಗಮ cpu;
}

ಸ್ಥಿರ ಶೂನ್ಯ check_preempt_equal_prio(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p)
{
	ಯದಿ (rq->curr->nr_cpus_allowed == 1 ||
	    !cpupri_find(&rq->rd->cpupri, rq->donor, ರಿಕ್ತ))
		ಪ್ರತ್ಯಾಗಮ;

	/*
	 * p is migratable, so let's not schedule it and
	 * see if it is pushed or pulled somewhere else.
	 */
	ಯದಿ (p->nr_cpus_allowed != 1 &&
	    cpupri_find(&rq->rd->cpupri, p, ರಿಕ್ತ))
		ಪ್ರತ್ಯಾಗಮ;

	/*
	 * There appear to be other CPUs that can accept
	 * the current task but none can run 'p', so lets reschedule
	 * to try and push the current task away:
	 */
	requeue_task_rt(rq, p, 1);
	resched_curr(rq);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ balance_rt(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ rq_flags *rf)
{
	/*
	 * Note, rq->donor may change during rq lock drops,
	 * so don't re-use p across lock drops
	 */
	ಸಂರಚನಾ task_struct *p = rq->donor;

	ಯದಿ (!on_rt_rq(&p->rt) && need_pull_rt_task(rq, p)) {
		/*
		 * This is OK, because current is on_cpu, which avoids it being
		 * picked for load-balance and preemption/IRQs are still
		 * disabled avoiding further scheduler activity on it and we've
		 * not yet started the picking loop.
		 */
		rq_unpin_lock(rq, rf);
		pull_rt_task(rq);
		rq_repin_lock(rq, rf);
	}

	ಪ್ರತ್ಯಾಗಮ sched_stop_runnable(rq) || sched_dl_runnable(rq) || sched_rt_runnable(rq);
}

/*
 * Preempt the current task with a newly woken task if needed:
 */
ಸ್ಥಿರ ಶೂನ್ಯ wakeup_preempt_rt(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p, ಪೂರ್ಣಾಙ್ಕ flags)
{
	ಸಂರಚನಾ task_struct *donor = rq->donor;

	/*
	 * XXX If we're preempted by DL, queue a push?
	 */
	ಯದಿ (p->sched_class != &rt_sched_class ||
	    donor->sched_class != &rt_sched_class)
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (p->prio < donor->prio) {
		resched_curr(rq);
		ಪ್ರತ್ಯಾಗಮ;
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
	ಯದಿ (p->prio == donor->prio && !test_tsk_need_resched(rq->curr))
		check_preempt_equal_prio(rq, p);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ set_next_task_rt(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p, ಗಣನಾ snt_e type)
{
	ಸಂರಚನಾ sched_rt_entity *rt_se = &p->rt;
	ಸಂರಚನಾ rt_rq *rt_rq = &rq->rt;

	ಯದಿ (type == SNT_REPICK)
		ಪ್ರತ್ಯಾಗಮ;

	p->se.exec_start = rq_clock_task(rq);
	ಯದಿ (on_rt_rq(&p->rt))
		update_stats_wait_end_rt(rt_rq, rt_se);

	/* The running task is never eligible for pushing */
	dequeue_pushable_task(rq, p);

	ಯದಿ (type != SNT_PICK)
		ಪ್ರತ್ಯಾಗಮ;

	/*
	 * If prev task was rt, put_prev_task() has already updated the
	 * utilization. We only care of the case where we start to schedule a
	 * rt task
	 */
	ಯದಿ (rq->donor->sched_class != &rt_sched_class)
		update_rt_rq_load_avg(rq_clock_pelt(rq), rq, 0);

	rt_queue_ತಳ್ಳು_tasks(rq);
}

ಸ್ಥಿರ ಸಂರಚನಾ sched_rt_entity *pick_next_rt_entity(ಸಂರಚನಾ rt_rq *rt_rq)
{
	ಸಂರಚನಾ rt_prio_array *array = &rt_rq->active;
	ಸಂರಚನಾ sched_rt_entity *next = ರಿಕ್ತ;
	ಸಂರಚನಾ list_head *queue;
	ಪೂರ್ಣಾಙ್ಕ idx;

	idx = sched_find_first_bit(array->bitmap);
	BUG_ON(idx >= MAX_RT_PRIO);

	queue = array->queue + idx;
	ಯದಿ (WARN_ON_ONCE(list_empty(queue)))
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
	next = list_entry(queue->next, ಸಂರಚನಾ sched_rt_entity, run_list);

	ಪ್ರತ್ಯಾಗಮ next;
}

ಸ್ಥಿರ ಸಂರಚನಾ task_struct *_pick_next_task_rt(ಸಂರಚನಾ rq *rq)
{
	ಸಂರಚನಾ sched_rt_entity *rt_se;
	ಸಂರಚನಾ rt_rq *rt_rq  = &rq->rt;

	ಕುರು {
		rt_se = pick_next_rt_entity(rt_rq);
		ಯದಿ (unlikely(!rt_se))
			ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
		rt_rq = group_rt_rq(rt_se);
	} ಯಾವತ್ (rt_rq);

	ಪ್ರತ್ಯಾಗಮ rt_task_of(rt_se);
}

ಸ್ಥಿರ ಸಂರಚನಾ task_struct *pick_task_rt(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ rq_flags *rf)
{
	ಸಂರಚನಾ task_struct *p;

	ಯದಿ (!sched_rt_runnable(rq))
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

	p = _pick_next_task_rt(rq);

	ಪ್ರತ್ಯಾಗಮ p;
}

ಸ್ಥಿರ ಶೂನ್ಯ put_prev_task_rt(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p, ಸಂರಚನಾ task_struct *next)
{
	ಸಂರಚನಾ sched_rt_entity *rt_se = &p->rt;
	ಸಂರಚನಾ rt_rq *rt_rq = &rq->rt;

	ಯದಿ (on_rt_rq(&p->rt))
		update_stats_wait_start_rt(rt_rq, rt_se);

	update_curr_rt(rq);

	update_rt_rq_load_avg(rq_clock_pelt(rq), rq, 1);

	ಯದಿ (task_is_blocked(p))
		ಪ್ರತ್ಯಾಗಮ;
	/*
	 * The previous task needs to be made eligible for pushing
	 * if it is still active
	 */
	ಯದಿ (on_rt_rq(&p->rt) && p->nr_cpus_allowed > 1)
		enqueue_pushable_task(rq, p);
}

/* Only try algorithms three times */
#define RT_MAX_TRIES 3

/*
 * Return the highest pushable rq's task, which is suitable to be executed
 * on the CPU, NULL otherwise
 */
ಸ್ಥಿರ ಸಂರಚನಾ task_struct *pick_highest_pushable_task(ಸಂರಚನಾ rq *rq, ಪೂರ್ಣಾಙ್ಕ cpu)
{
	ಸಂರಚನಾ plist_head *head = &rq->rt.pushable_tasks;
	ಸಂರಚನಾ task_struct *p;

	ಯದಿ (!has_pushable_tasks(rq))
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

	plist_for_each_entry(p, head, pushable_tasks) {
		ಯದಿ (task_is_pushable(rq, p, cpu))
			ಪ್ರತ್ಯಾಗಮ p;
	}

	ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

ಸ್ಥಿರ DEFINE_PER_CPU(cpumask_var_t, local_cpu_mask);

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ find_lowest_rq(ಸಂರಚನಾ task_struct *task)
{
	ಸಂರಚನಾ sched_domain *sd;
	ಸಂರಚನಾ cpumask *lowest_mask = this_cpu_cpumask_var_ptr(local_cpu_mask);
	ಪೂರ್ಣಾಙ್ಕ this_cpu = smp_processor_id();
	ಪೂರ್ಣಾಙ್ಕ cpu      = task_cpu(task);
	ಪೂರ್ಣಾಙ್ಕ ret;

	/* Make sure the mask is initialized first */
	ಯದಿ (unlikely(!lowest_mask))
		ಪ್ರತ್ಯಾಗಮ -1;

	ಯದಿ (task->nr_cpus_allowed == 1)
		ಪ್ರತ್ಯಾಗಮ -1; /* No other targets possible */

	/*
	 * If we're on asym system ensure we consider the different capacities
	 * of the CPUs when searching for the lowest_mask.
	 */
	ಯದಿ (sched_asym_cpucap_active()) {

		ret = cpupri_find_fitness(&task_rq(task)->rd->cpupri,
					  task, lowest_mask,
					  rt_task_fits_capacity);
	} ಅನ್ಯಥಾ {

		ret = cpupri_find(&task_rq(task)->rd->cpupri,
				  task, lowest_mask);
	}

	ಯದಿ (!ret)
		ಪ್ರತ್ಯಾಗಮ -1; /* No targets found */

	/*
	 * At this point we have built a mask of CPUs representing the
	 * lowest priority tasks in the system.  Now we want to elect
	 * the best one based on our affinity and topology.
	 *
	 * We prioritize the last CPU that the task executed on since
	 * it is most likely cache-hot in that location.
	 */
	ಯದಿ (cpumask_test_cpu(cpu, lowest_mask))
		ಪ್ರತ್ಯಾಗಮ cpu;

	/*
	 * Otherwise, we consult the sched_domains span maps to figure
	 * out which CPU is logically closest to our hot cache data.
	 */
	ಯದಿ (!cpumask_test_cpu(this_cpu, lowest_mask))
		this_cpu = -1; /* Skip this_cpu opt if not among lowest */

	rcu_read_lock();
	for_each_domain(cpu, sd) {
		ಯದಿ (sd->flags & SD_WAKE_AFFINE) {
			ಪೂರ್ಣಾಙ್ಕ best_cpu;

			/*
			 * "this_cpu" is cheaper to preempt than a
			 * remote processor.
			 */
			ಯದಿ (this_cpu != -1 &&
			    cpumask_test_cpu(this_cpu, sched_domain_span(sd))) {
				rcu_read_unlock();
				ಪ್ರತ್ಯಾಗಮ this_cpu;
			}

			best_cpu = cpumask_any_and_distribute(lowest_mask,
							      sched_domain_span(sd));
			ಯದಿ (best_cpu < nr_cpu_ids) {
				rcu_read_unlock();
				ಪ್ರತ್ಯಾಗಮ best_cpu;
			}
		}
	}
	rcu_read_unlock();

	/*
	 * And finally, if there were no matches within the domains
	 * just give the caller *something* to work with from the compatible
	 * locations.
	 */
	ಯದಿ (this_cpu != -1)
		ಪ್ರತ್ಯಾಗಮ this_cpu;

	cpu = cpumask_any_distribute(lowest_mask);
	ಯದಿ (cpu < nr_cpu_ids)
		ಪ್ರತ್ಯಾಗಮ cpu;

	ಪ್ರತ್ಯಾಗಮ -1;
}

ಸ್ಥಿರ ಸಂರಚನಾ task_struct *pick_next_pushable_task(ಸಂರಚನಾ rq *rq)
{
	ಸಂರಚನಾ plist_head *head = &rq->rt.pushable_tasks;
	ಸಂರಚನಾ task_struct *i, *p = ರಿಕ್ತ;

	ಯದಿ (!has_pushable_tasks(rq))
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

	plist_for_each_entry(i, head, pushable_tasks) {
		/* skip tasks that cannot be migrated */
		ಯದಿ (!task_on_cpu(rq, i) && !is_migration_disabled(i)) {
			p = i;
			ಭಙ್ಗ;
		}
	}

	ಯದಿ (!p)
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

	BUG_ON(rq->cpu != task_cpu(p));
	BUG_ON(task_current(rq, p));
	BUG_ON(task_current_donor(rq, p));
	BUG_ON(p->nr_cpus_allowed <= 1);

	BUG_ON(!task_on_rq_queued(p));
	BUG_ON(!rt_task(p));

	ಪ್ರತ್ಯಾಗಮ p;
}

/* Will lock the rq it finds */
ಸ್ಥಿರ ಸಂರಚನಾ rq *find_lock_lowest_rq(ಸಂರಚನಾ task_struct *task, ಸಂರಚನಾ rq *rq)
{
	ಸಂರಚನಾ rq *lowest_rq = ರಿಕ್ತ;
	ಪೂರ್ಣಾಙ್ಕ tries;
	ಪೂರ್ಣಾಙ್ಕ cpu;

	ಪ್ರತಿ (tries = 0; tries < RT_MAX_TRIES; tries++) {
		cpu = find_lowest_rq(task);

		ಯದಿ ((cpu == -1) || (cpu == rq->cpu))
			ಭಙ್ಗ;

		lowest_rq = cpu_rq(cpu);

		ಯದಿ (lowest_rq->rt.highest_prio.curr <= task->prio) {
			/*
			 * Target rq has tasks of equal or higher priority,
			 * retrying does not release any lock and is unlikely
			 * to yield a different result.
			 */
			lowest_rq = ರಿಕ್ತ;
			ಭಙ್ಗ;
		}

		/* if the prio of this runqueue changed, try again */
		ಯದಿ (double_lock_balance(rq, lowest_rq)) {
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
			ಯದಿ (unlikely(is_migration_disabled(task) ||
				     !cpumask_test_cpu(lowest_rq->cpu, &task->cpus_mask) ||
				     task != pick_next_pushable_task(rq))) {

				double_unlock_balance(rq, lowest_rq);
				lowest_rq = ರಿಕ್ತ;
				ಭಙ್ಗ;
			}
		}

		/* If this rq is still suitable use it. */
		ಯದಿ (lowest_rq->rt.highest_prio.curr > task->prio)
			ಭಙ್ಗ;

		/* try again */
		double_unlock_balance(rq, lowest_rq);
		lowest_rq = ರಿಕ್ತ;
	}

	ಪ್ರತ್ಯಾಗಮ lowest_rq;
}

/*
 * If the current CPU has more than one RT task, see if the non
 * running task can migrate over to a CPU that is running a task
 * of lesser priority.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ ತಳ್ಳು_rt_task(ಸಂರಚನಾ rq *rq, bool pull)
{
	ಸಂರಚನಾ task_struct *next_task;
	ಸಂರಚನಾ rq *lowest_rq;
	ಪೂರ್ಣಾಙ್ಕ ret = 0;

	ಯದಿ (!rq->rt.overloaded)
		ಪ್ರತ್ಯಾಗಮ 0;

	next_task = pick_next_pushable_task(rq);
	ಯದಿ (!next_task)
		ಪ್ರತ್ಯಾಗಮ 0;

retry:
	/*
	 * It's possible that the next_task slipped in of
	 * higher priority than current. If that's the case
	 * just reschedule current.
	 */
	ಯದಿ (unlikely(next_task->prio < rq->donor->prio)) {
		resched_curr(rq);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (is_migration_disabled(next_task)) {
		ಸಂರಚನಾ task_struct *ತಳ್ಳು_task = ರಿಕ್ತ;
		ಪೂರ್ಣಾಙ್ಕ cpu;

		ಯದಿ (!pull || rq->ತಳ್ಳು_busy)
			ಪ್ರತ್ಯಾಗಮ 0;

		/*
		 * Invoking find_lowest_rq() on anything but an RT task doesn't
		 * make sense. Per the above priority check, curr has to
		 * be of higher priority than next_task, so no need to
		 * reschedule when bailing out.
		 *
		 * Note that the stoppers are masqueraded as SCHED_FIFO
		 * (cf. sched_set_stop_task()), so we can't rely on rt_task().
		 */
		ಯದಿ (rq->donor->sched_class != &rt_sched_class)
			ಪ್ರತ್ಯಾಗಮ 0;

		cpu = find_lowest_rq(rq->curr);
		ಯದಿ (cpu == -1 || cpu == rq->cpu)
			ಪ್ರತ್ಯಾಗಮ 0;

		/*
		 * Given we found a CPU with lower priority than @next_task,
		 * therefore it should be running. However we cannot migrate it
		 * to this other CPU, instead attempt to push the current
		 * running task on this CPU away.
		 */
		ತಳ್ಳು_task = get_ತಳ್ಳು_task(rq);
		ಯದಿ (ತಳ್ಳು_task) {
			preempt_disable();
			raw_spin_rq_unlock(rq);
			stop_one_cpu_nowait(rq->cpu, ತಳ್ಳು_cpu_stop,
					    ತಳ್ಳು_task, &rq->ತಳ್ಳು_work);
			preempt_enable();
			raw_spin_rq_lock(rq);
		}

		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (WARN_ON(next_task == rq->curr))
		ಪ್ರತ್ಯಾಗಮ 0;

	/* We might release rq lock */
	get_task_struct(next_task);

	/* find_lock_lowest_rq locks the rq if found */
	lowest_rq = find_lock_lowest_rq(next_task, rq);
	ಯದಿ (!lowest_rq) {
		ಸಂರಚನಾ task_struct *task;
		/*
		 * find_lock_lowest_rq releases rq->lock
		 * so it is possible that next_task has migrated.
		 *
		 * We need to make sure that the task is still on the same
		 * run-queue and is also still the next task eligible for
		 * pushing.
		 */
		task = pick_next_pushable_task(rq);
		ಯದಿ (task == next_task) {
			/*
			 * The task hasn't migrated, and is still the next
			 * eligible task, but we failed to find a run-queue
			 * to push it to.  Do not retry in this case, since
			 * other CPUs will pull from us when ready.
			 */
			ಗಚ್ಛ out;
		}

		ಯದಿ (!task)
			/* No more tasks, just exit */
			ಗಚ್ಛ out;

		/*
		 * Something has shifted, try again.
		 */
		put_task_struct(next_task);
		next_task = task;
		ಗಚ್ಛ retry;
	}

	move_queued_task_locked(rq, lowest_rq, next_task);
	resched_curr(lowest_rq);
	ret = 1;

	double_unlock_balance(rq, lowest_rq);
out:
	put_task_struct(next_task);

	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ ತಳ್ಳು_rt_tasks(ಸಂರಚನಾ rq *rq)
{
	/* push_rt_task will return true if it moved an RT */
	ಯಾವತ್ (ತಳ್ಳು_rt_task(rq, false))
		;
}

#ifdef HAVE_RT_ತಳ್ಳು_IPI

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
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ rto_next_cpu(ಸಂರಚನಾ ಬೇರು_domain *rd)
{
	ಪೂರ್ಣಾಙ್ಕ this_cpu = smp_processor_id();
	ಪೂರ್ಣಾಙ್ಕ next;
	ಪೂರ್ಣಾಙ್ಕ cpu;

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
	ಪ್ರತಿ (;;) {

		/* When rto_cpu is -1 this acts like cpumask_first() */
		cpu = cpumask_next(rd->rto_cpu, rd->rto_mask);

		rd->rto_cpu = cpu;

		/* Do not send IPI to self */
		ಯದಿ (cpu == this_cpu)
			ಅನುವರ್ತ;

		ಯದಿ (cpu < nr_cpu_ids)
			ಪ್ರತ್ಯಾಗಮ cpu;

		rd->rto_cpu = -1;

		/*
		 * ACQUIRE ensures we see the @rto_mask changes
		 * made prior to the @next value observed.
		 *
		 * Matches WMB in rt_set_overload().
		 */
		next = atomic_read_acquire(&rd->rto_loop_next);

		ಯದಿ (rd->rto_loop == next)
			ಭಙ್ಗ;

		rd->rto_loop = next;
	}

	ಪ್ರತ್ಯಾಗಮ -1;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool rto_start_trylock(atomic_t *v)
{
	ಪ್ರತ್ಯಾಗಮ !atomic_cmpxchg_acquire(v, 0, 1);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ rto_start_unlock(atomic_t *v)
{
	atomic_set_release(v, 0);
}

ಸ್ಥಿರ ಶೂನ್ಯ tell_cpu_to_ತಳ್ಳು(ಸಂರಚನಾ rq *rq)
{
	ಪೂರ್ಣಾಙ್ಕ cpu = -1;

	/* Keep the loop going if the IPI is currently active */
	atomic_inc(&rq->rd->rto_loop_next);

	/* Only one CPU can initiate a loop at a time */
	ಯದಿ (!rto_start_trylock(&rq->rd->rto_loop_start))
		ಪ್ರತ್ಯಾಗಮ;

	raw_spin_lock(&rq->rd->rto_lock);

	/*
	 * The rto_cpu is updated under the lock, if it has a valid CPU
	 * then the IPI is still running and will continue due to the
	 * update to loop_next, and nothing needs to be done here.
	 * Otherwise it is finishing up and an IPI needs to be sent.
	 */
	ಯದಿ (rq->rd->rto_cpu < 0)
		cpu = rto_next_cpu(rq->rd);

	raw_spin_unlock(&rq->rd->rto_lock);

	rto_start_unlock(&rq->rd->rto_loop_start);

	ಯದಿ (cpu >= 0) {
		/* Make sure the rd does not get freed while pushing */
		sched_get_rd(rq->rd);
		irq_work_queue_on(&rq->rd->rto_ತಳ್ಳು_work, cpu);
	}
}

/* Called from hardirq context */
ಶೂನ್ಯ rto_ತಳ್ಳು_irq_work_func(ಸಂರಚನಾ irq_work *work)
{
	ಸಂರಚನಾ ಬೇರು_domain *rd =
		container_of(work, ಸಂರಚನಾ ಬೇರು_domain, rto_ತಳ್ಳು_work);
	ಸಂರಚನಾ rq *rq;
	ಪೂರ್ಣಾಙ್ಕ cpu;

	rq = this_rq();

	/*
	 * We do not need to grab the lock to check for has_pushable_tasks.
	 * When it gets updated, a check is made if a push is possible.
	 */
	ಯದಿ (has_pushable_tasks(rq)) {
		raw_spin_rq_lock(rq);
		ಯಾವತ್ (ತಳ್ಳು_rt_task(rq, true))
			;
		raw_spin_rq_unlock(rq);
	}

	raw_spin_lock(&rd->rto_lock);

	/* Pass the IPI to the next rt overloaded queue */
	cpu = rto_next_cpu(rd);

	raw_spin_unlock(&rd->rto_lock);

	ಯದಿ (cpu < 0) {
		sched_put_rd(rd);
		ಪ್ರತ್ಯಾಗಮ;
	}

	/* Try the next RT overloaded CPU */
	irq_work_queue_on(&rd->rto_ತಳ್ಳು_work, cpu);
}
#endif /* HAVE_RT_PUSH_IPI */

ಸ್ಥಿರ ಶೂನ್ಯ pull_rt_task(ಸಂರಚನಾ rq *this_rq)
{
	ಪೂರ್ಣಾಙ್ಕ this_cpu = this_rq->cpu, cpu;
	bool resched = false;
	ಸಂರಚನಾ task_struct *p, *ತಳ್ಳು_task;
	ಸಂರಚನಾ rq *src_rq;
	ಪೂರ್ಣಾಙ್ಕ rt_overload_ಎಣಿಕೆ = rt_overloaded(this_rq);

	ಯದಿ (likely(!rt_overload_ಎಣಿಕೆ))
		ಪ್ರತ್ಯಾಗಮ;

	/*
	 * Match the barrier from rt_set_overloaded; this guarantees that if we
	 * see overloaded we must also see the rto_mask bit.
	 */
	smp_rmb();

	/* If we are the only overloaded CPU do nothing */
	ಯದಿ (rt_overload_ಎಣಿಕೆ == 1 &&
	    cpumask_test_cpu(this_rq->cpu, this_rq->rd->rto_mask))
		ಪ್ರತ್ಯಾಗಮ;

#ifdef HAVE_RT_ತಳ್ಳು_IPI
	ಯದಿ (sched_feat(RT_ತಳ್ಳು_IPI)) {
		tell_cpu_to_ತಳ್ಳು(this_rq);
		ಪ್ರತ್ಯಾಗಮ;
	}
#endif

	for_each_cpu(cpu, this_rq->rd->rto_mask) {
		ಯದಿ (this_cpu == cpu)
			ಅನುವರ್ತ;

		src_rq = cpu_rq(cpu);

		/*
		 * Don't bother taking the src_rq->lock if the next highest
		 * task is known to be lower-priority than our current task.
		 * This may look racy, but if this value is about to go
		 * logically higher, the src_rq will push this task away.
		 * And if its going logically lower, we do not care
		 */
		ಯದಿ (src_rq->rt.highest_prio.next >=
		    this_rq->rt.highest_prio.curr)
			ಅನುವರ್ತ;

		/*
		 * We can potentially drop this_rq's lock in
		 * double_lock_balance, and another CPU could
		 * alter this_rq
		 */
		ತಳ್ಳು_task = ರಿಕ್ತ;
		double_lock_balance(this_rq, src_rq);

		/*
		 * We can pull only a task, which is pushable
		 * on its rq, and no others.
		 */
		p = pick_highest_pushable_task(src_rq, this_cpu);

		/*
		 * Do we have an RT task that preempts
		 * the to-be-scheduled task?
		 */
		ಯದಿ (p && (p->prio < this_rq->rt.highest_prio.curr)) {
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
			ಯದಿ (p->prio < src_rq->donor->prio)
				ಗಚ್ಛ skip;

			ಯದಿ (is_migration_disabled(p)) {
				ತಳ್ಳು_task = get_ತಳ್ಳು_task(src_rq);
			} ಅನ್ಯಥಾ {
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

		ಯದಿ (ತಳ್ಳು_task) {
			preempt_disable();
			raw_spin_rq_unlock(this_rq);
			stop_one_cpu_nowait(src_rq->cpu, ತಳ್ಳು_cpu_stop,
					    ತಳ್ಳು_task, &src_rq->ತಳ್ಳು_work);
			preempt_enable();
			raw_spin_rq_lock(this_rq);
		}
	}

	ಯದಿ (resched)
		resched_curr(this_rq);
}

/*
 * If we are not running and we are not going to reschedule soon, we should
 * try to push tasks away now
 */
ಸ್ಥಿರ ಶೂನ್ಯ task_woken_rt(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p)
{
	bool need_to_ತಳ್ಳು = !task_on_cpu(rq, p) &&
			    !test_tsk_need_resched(rq->curr) &&
			    p->nr_cpus_allowed > 1 &&
			    (dl_task(rq->donor) || rt_task(rq->donor)) &&
			    (rq->curr->nr_cpus_allowed < 2 ||
			     rq->donor->prio <= p->prio);

	ಯದಿ (need_to_ತಳ್ಳು)
		ತಳ್ಳು_rt_tasks(rq);
}

/* Assumes rq->lock is held */
ಸ್ಥಿರ ಶೂನ್ಯ rq_online_rt(ಸಂರಚನಾ rq *rq)
{
	ಯದಿ (rq->rt.overloaded)
		rt_set_overload(rq);

	__enable_runtime(rq);

	cpupri_set(&rq->rd->cpupri, rq->cpu, rq->rt.highest_prio.curr);
}

/* Assumes rq->lock is held */
ಸ್ಥಿರ ಶೂನ್ಯ rq_offline_rt(ಸಂರಚನಾ rq *rq)
{
	ಯದಿ (rq->rt.overloaded)
		rt_clear_overload(rq);

	__disable_runtime(rq);

	cpupri_set(&rq->rd->cpupri, rq->cpu, CPUPRI_INVALID);
}

/*
 * When switch from the rt queue, we bring ourselves to a position
 * that we might want to pull RT tasks from other runqueues.
 */
ಸ್ಥಿರ ಶೂನ್ಯ switched_from_rt(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p)
{
	/*
	 * If there are other RT tasks then we will reschedule
	 * and the scheduling of the other RT tasks will handle
	 * the balancing. But if we are the last RT task
	 * we may need to handle the pulling of RT tasks
	 * now.
	 */
	ಯದಿ (!task_on_rq_queued(p) || rq->rt.rt_nr_running)
		ಪ್ರತ್ಯಾಗಮ;

	rt_queue_pull_task(rq);
}

ಶೂನ್ಯ __init init_sched_rt_class(ಶೂನ್ಯ)
{
	unsigned ಪೂರ್ಣಾಙ್ಕ i;

	for_each_possible_cpu(i) {
		zalloc_cpumask_var_ಗಂಟು(&per_cpu(local_cpu_mask, i),
					GFP_KERNEL, cpu_to_ಗಂಟು(i));
	}
}

/*
 * When switching a task to RT, we may overload the runqueue
 * with RT tasks. In this case we try to push them off to
 * other runqueues.
 */
ಸ್ಥಿರ ಶೂನ್ಯ switched_to_rt(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p)
{
	/*
	 * If we are running, update the avg_rt tracking, as the running time
	 * will now on be accounted into the latter.
	 */
	ಯದಿ (task_current(rq, p)) {
		update_rt_rq_load_avg(rq_clock_pelt(rq), rq, 0);
		ಪ್ರತ್ಯಾಗಮ;
	}

	/*
	 * If we are not running we may need to preempt the current
	 * running task. If that current running task is also an RT task
	 * then see if we can move to another run queue.
	 */
	ಯದಿ (task_on_rq_queued(p)) {
		ಯದಿ (p->nr_cpus_allowed > 1 && rq->rt.overloaded)
			rt_queue_ತಳ್ಳು_tasks(rq);
		ಯದಿ (p->prio < rq->donor->prio && cpu_online(cpu_of(rq)))
			resched_curr(rq);
	}
}

/*
 * Priority of the task has changed. This may cause
 * us to initiate a push or pull.
 */
ಸ್ಥಿರ ಶೂನ್ಯ
prio_changed_rt(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p, u64 oldprio)
{
	ಯದಿ (!task_on_rq_queued(p))
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (p->prio == oldprio)
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (task_current_donor(rq, p)) {
		/*
		 * If our priority decreases while running, we
		 * may need to pull tasks to this runqueue.
		 */
		ಯದಿ (oldprio < p->prio)
			rt_queue_pull_task(rq);

		/*
		 * If there's a higher priority task waiting to run
		 * then reschedule.
		 */
		ಯದಿ (p->prio > rq->rt.highest_prio.curr)
			resched_curr(rq);
	} ಅನ್ಯಥಾ {
		/*
		 * This task is not running, but if it is
		 * greater than the current running task
		 * then reschedule.
		 */
		ಯದಿ (p->prio < rq->donor->prio)
			resched_curr(rq);
	}
}

#ifdef CONFIG_POSIX_TIMERS
ಸ್ಥಿರ ಶೂನ್ಯ watchdog(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p)
{
	unsigned long soft, hard;

	/* max may change after cur was read, this will be fixed next tick */
	soft = task_rlimit(p, RLIMIT_RTTIME);
	hard = task_rlimit_max(p, RLIMIT_RTTIME);

	ಯದಿ (soft != RLIM_INFINITY) {
		unsigned long next;

		ಯದಿ (p->rt.watchdog_stamp != jiffies) {
			p->rt.timeout++;
			p->rt.watchdog_stamp = jiffies;
		}

		next = DIV_ROUND_UP(min(soft, hard), USEC_PER_SEC/HZ);
		ಯದಿ (p->rt.timeout > next) {
			posix_cputimers_rt_watchdog(&p->posix_cputimers,
						    p->se.sum_exec_runtime);
		}
	}
}
#ಅನ್ಯಥಾ /* !CONFIG_POSIX_TIMERS: */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ watchdog(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p) { }
#endif /* !CONFIG_POSIX_TIMERS */

/*
 * scheduler tick hitting a task of our scheduling class.
 *
 * NOTE: This function can be called remotely by the tick offload that
 * goes along full dynticks. Therefore no local assumption can be made
 * and everything must be accessed through the @rq and @curr passed in
 * parameters.
 */
ಸ್ಥಿರ ಶೂನ್ಯ task_tick_rt(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p, ಪೂರ್ಣಾಙ್ಕ queued)
{
	ಸಂರಚನಾ sched_rt_entity *rt_se = &p->rt;

	update_curr_rt(rq);
	update_rt_rq_load_avg(rq_clock_pelt(rq), rq, 1);

	watchdog(rq, p);

	/*
	 * RR tasks need a special form of time-slice management.
	 * FIFO tasks have no timeslices.
	 */
	ಯದಿ (p->policy != SCHED_RR)
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (--p->rt.time_slice)
		ಪ್ರತ್ಯಾಗಮ;

	p->rt.time_slice = sched_rr_timeslice;

	/*
	 * Requeue to the end of queue if we (and all of our ancestors) are not
	 * the only element on the queue
	 */
	for_each_sched_rt_entity(rt_se) {
		ಯದಿ (rt_se->run_list.prev != rt_se->run_list.next) {
			requeue_task_rt(rq, p, 0);
			resched_curr(rq);
			ಪ್ರತ್ಯಾಗಮ;
		}
	}
}

ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ get_rr_interval_rt(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *task)
{
	/*
	 * Time slice is 0 for SCHED_FIFO tasks
	 */
	ಯದಿ (task->policy == SCHED_RR)
		ಪ್ರತ್ಯಾಗಮ sched_rr_timeslice;
	ಅನ್ಯಥಾ
		ಪ್ರತ್ಯಾಗಮ 0;
}

#ifdef CONFIG_SCHED_CORE
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ task_is_throttled_rt(ಸಂರಚನಾ task_struct *p, ಪೂರ್ಣಾಙ್ಕ cpu)
{
	ಸಂರಚನಾ rt_rq *rt_rq;

#ifdef CONFIG_RT_GROUP_SCHED // XXX maybe add task_rt_rq(), see also sched_rt_period_rt_rq
	rt_rq = task_group(p)->rt_rq[cpu];
	WARN_ON(!rt_group_sched_enabled() && rt_rq->tg != &ಬೇರು_task_group);
#ಅನ್ಯಥಾ
	rt_rq = &cpu_rq(cpu)->rt;
#endif

	ಪ್ರತ್ಯಾಗಮ rt_rq_throttled(rt_rq);
}
#endif /* CONFIG_SCHED_CORE */

DEFINE_SCHED_CLASS(rt) = {
	.enqueue_task		= enqueue_task_rt,
	.dequeue_task		= dequeue_task_rt,
	.yield_task		= yield_task_rt,

	.wakeup_preempt		= wakeup_preempt_rt,

	.pick_task		= pick_task_rt,
	.put_prev_task		= put_prev_task_rt,
	.set_next_task          = set_next_task_rt,

	.balance		= balance_rt,
	.select_task_rq		= select_task_rq_rt,
	.set_cpus_allowed       = set_cpus_allowed_common,
	.rq_online              = rq_online_rt,
	.rq_offline             = rq_offline_rt,
	.task_woken		= task_woken_rt,
	.switched_from		= switched_from_rt,
	.find_lock_rq		= find_lock_lowest_rq,

	.task_tick		= task_tick_rt,

	.get_rr_interval	= get_rr_interval_rt,

	.switched_to		= switched_to_rt,
	.prio_changed		= prio_changed_rt,

	.update_curr		= update_curr_rt,

#ifdef CONFIG_SCHED_CORE
	.task_is_throttled	= task_is_throttled_rt,
#endif

#ifdef CONFIG_UCLAMP_TASK
	.uclamp_enabled		= 1,
#endif
};

#ifdef CONFIG_RT_GROUP_SCHED
/*
 * Ensure that the real time constraints are schedulable.
 */
ಸ್ಥಿರ DEFINE_MUTEX(rt_constraints_mutex);

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ tg_has_rt_tasks(ಸಂರಚನಾ task_group *tg)
{
	ಸಂರಚನಾ task_struct *task;
	ಸಂರಚನಾ css_task_iter it;
	ಪೂರ್ಣಾಙ್ಕ ret = 0;

	/*
	 * Autogroups do not have RT tasks; see autogroup_create().
	 */
	ಯದಿ (task_group_is_autogroup(tg))
		ಪ್ರತ್ಯಾಗಮ 0;

	css_task_iter_start(&tg->css, 0, &it);
	ಯಾವತ್ (!ret && (task = css_task_iter_next(&it)))
		ret |= rt_task(task);
	css_task_iter_end(&it);

	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸಂರಚನಾ rt_schedulable_data {
	ಸಂರಚನಾ task_group *tg;
	u64 rt_period;
	u64 rt_runtime;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ tg_rt_schedulable(ಸಂರಚನಾ task_group *tg, ಶೂನ್ಯ *data)
{
	ಸಂರಚನಾ rt_schedulable_data *d = data;
	ಸಂರಚನಾ task_group *child;
	u64 total, sum = 0;
	u64 period, runtime;

	period = ktime_to_ns(tg->rt_bandwidth.rt_period);
	runtime = tg->rt_bandwidth.rt_runtime;

	ಯದಿ (tg == d->tg) {
		period = d->rt_period;
		runtime = d->rt_runtime;
	}

	/*
	 * Cannot have more runtime than the period.
	 */
	ಯದಿ (runtime > period && runtime != RUNTIME_INF)
		ಪ್ರತ್ಯಾಗಮ -EINVAL;

	/*
	 * Ensure we don't starve existing RT tasks if runtime turns zero.
	 */
	ಯದಿ (rt_bandwidth_enabled() && !runtime &&
	    tg->rt_bandwidth.rt_runtime && tg_has_rt_tasks(tg))
		ಪ್ರತ್ಯಾಗಮ -EBUSY;

	total = to_ratio(period, runtime);

	/*
	 * Nobody can have more than the global setting allows.
	 */
	ಯದಿ (total > to_ratio(global_rt_period(), global_rt_runtime()))
		ಪ್ರತ್ಯಾಗಮ -EINVAL;

	/*
	 * The sum of our children's runtime should not exceed our own.
	 */
	list_for_each_entry_rcu(child, &tg->children, siblings) {
		period = ktime_to_ns(child->rt_bandwidth.rt_period);
		runtime = child->rt_bandwidth.rt_runtime;

		ಯದಿ (child == d->tg) {
			period = d->rt_period;
			runtime = d->rt_runtime;
		}

		sum += to_ratio(period, runtime);
	}

	ಯದಿ (sum > total)
		ಪ್ರತ್ಯಾಗಮ -EINVAL;

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ __rt_schedulable(ಸಂರಚನಾ task_group *tg, u64 period, u64 runtime)
{
	ಪೂರ್ಣಾಙ್ಕ ret;

	ಸಂರಚನಾ rt_schedulable_data data = {
		.tg = tg,
		.rt_period = period,
		.rt_runtime = runtime,
	};

	rcu_read_lock();
	ret = walk_tg_ವೃಕ್ಷ(tg_rt_schedulable, tg_nop, &data);
	rcu_read_unlock();

	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ tg_set_rt_bandwidth(ಸಂರಚನಾ task_group *tg,
		u64 rt_period, u64 rt_runtime)
{
	ಪೂರ್ಣಾಙ್ಕ i, err = 0;

	/*
	 * Disallowing the root group RT runtime is BAD, it would disallow the
	 * kernel creating (and or operating) RT threads.
	 */
	ಯದಿ (tg == &ಬೇರು_task_group && rt_runtime == 0)
		ಪ್ರತ್ಯಾಗಮ -EINVAL;

	/* No period doesn't make any sense. */
	ಯದಿ (rt_period == 0)
		ಪ್ರತ್ಯಾಗಮ -EINVAL;

	/*
	 * Bound quota to defend quota against overflow during bandwidth shift.
	 */
	ಯದಿ (rt_runtime != RUNTIME_INF && rt_runtime > max_rt_runtime)
		ಪ್ರತ್ಯಾಗಮ -EINVAL;

	mutex_lock(&rt_constraints_mutex);
	err = __rt_schedulable(tg, rt_period, rt_runtime);
	ಯದಿ (err)
		ಗಚ್ಛ unlock;

	raw_spin_lock_irq(&tg->rt_bandwidth.rt_runtime_lock);
	tg->rt_bandwidth.rt_period = ns_to_ktime(rt_period);
	tg->rt_bandwidth.rt_runtime = rt_runtime;

	for_each_possible_cpu(i) {
		ಸಂರಚನಾ rt_rq *rt_rq = tg->rt_rq[i];

		raw_spin_lock(&rt_rq->rt_runtime_lock);
		rt_rq->rt_runtime = rt_runtime;
		raw_spin_unlock(&rt_rq->rt_runtime_lock);
	}
	raw_spin_unlock_irq(&tg->rt_bandwidth.rt_runtime_lock);
unlock:
	mutex_unlock(&rt_constraints_mutex);

	ಪ್ರತ್ಯಾಗಮ err;
}

ಪೂರ್ಣಾಙ್ಕ sched_group_set_rt_runtime(ಸಂರಚನಾ task_group *tg, long rt_runtime_us)
{
	u64 rt_runtime, rt_period;

	rt_period = ktime_to_ns(tg->rt_bandwidth.rt_period);
	rt_runtime = (u64)rt_runtime_us * NSEC_PER_USEC;
	ಯದಿ (rt_runtime_us < 0)
		rt_runtime = RUNTIME_INF;
	ಅನ್ಯಥಾ ಯದಿ ((u64)rt_runtime_us > U64_MAX / NSEC_PER_USEC)
		ಪ್ರತ್ಯಾಗಮ -EINVAL;

	ಪ್ರತ್ಯಾಗಮ tg_set_rt_bandwidth(tg, rt_period, rt_runtime);
}

long sched_group_rt_runtime(ಸಂರಚನಾ task_group *tg)
{
	u64 rt_runtime_us;

	ಯದಿ (tg->rt_bandwidth.rt_runtime == RUNTIME_INF)
		ಪ್ರತ್ಯಾಗಮ -1;

	rt_runtime_us = tg->rt_bandwidth.rt_runtime;
	do_div(rt_runtime_us, NSEC_PER_USEC);
	ಪ್ರತ್ಯಾಗಮ rt_runtime_us;
}

ಪೂರ್ಣಾಙ್ಕ sched_group_set_rt_period(ಸಂರಚನಾ task_group *tg, u64 rt_period_us)
{
	u64 rt_runtime, rt_period;

	ಯದಿ (rt_period_us > U64_MAX / NSEC_PER_USEC)
		ಪ್ರತ್ಯಾಗಮ -EINVAL;

	rt_period = rt_period_us * NSEC_PER_USEC;
	rt_runtime = tg->rt_bandwidth.rt_runtime;

	ಪ್ರತ್ಯಾಗಮ tg_set_rt_bandwidth(tg, rt_period, rt_runtime);
}

long sched_group_rt_period(ಸಂರಚನಾ task_group *tg)
{
	u64 rt_period_us;

	rt_period_us = ktime_to_ns(tg->rt_bandwidth.rt_period);
	do_div(rt_period_us, NSEC_PER_USEC);
	ಪ್ರತ್ಯಾಗಮ rt_period_us;
}

ಪೂರ್ಣಾಙ್ಕ sched_rt_can_attach(ಸಂರಚನಾ task_group *tg, ಸಂರಚನಾ task_struct *tsk)
{
	/* Don't accept real-time tasks when there is no way for them to run */
	ಯದಿ (rt_group_sched_enabled() && rt_task(tsk) && tg->rt_bandwidth.rt_runtime == 0)
		ಪ್ರತ್ಯಾಗಮ 0;

	ಪ್ರತ್ಯಾಗಮ 1;
}

#endif /* !CONFIG_RT_GROUP_SCHED */

#ifdef CONFIG_SYSCTL
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sched_rt_global_validate(ಶೂನ್ಯ)
{
	ಯದಿ ((sysctl_sched_rt_runtime != RUNTIME_INF) &&
		((sysctl_sched_rt_runtime > sysctl_sched_rt_period) ||
		 ((u64)sysctl_sched_rt_runtime *
			NSEC_PER_USEC > max_rt_runtime)))
		ಪ್ರತ್ಯಾಗಮ -EINVAL;

#ifdef CONFIG_RT_GROUP_SCHED
	ಯದಿ (!rt_group_sched_enabled())
		ಪ್ರತ್ಯಾಗಮ 0;

	scoped_guard(mutex, &rt_constraints_mutex)
		ಪ್ರತ್ಯಾಗಮ __rt_schedulable(ರಿಕ್ತ, 0, 0);
#endif
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sched_rt_handler(ಧ್ರುವ ಸಂರಚನಾ ctl_table *table, ಪೂರ್ಣಾಙ್ಕ ಲಿಖ, ಶೂನ್ಯ *buffer,
		size_t *lenp, loff_t *ppos)
{
	ಪೂರ್ಣಾಙ್ಕ old_period, old_runtime;
	ಸ್ಥಿರ DEFINE_MUTEX(mutex);
	ಪೂರ್ಣಾಙ್ಕ ret;

	mutex_lock(&mutex);
	sched_domains_mutex_lock();
	old_period = sysctl_sched_rt_period;
	old_runtime = sysctl_sched_rt_runtime;

	ret = proc_dointvec_minmax(table, ಲಿಖ, buffer, lenp, ppos);

	ಯದಿ (!ret && ಲಿಖ) {
		ret = sched_rt_global_validate();
		ಯದಿ (ret)
			ಗಚ್ಛ undo;

		ret = sched_dl_global_validate();
		ಯದಿ (ret)
			ಗಚ್ಛ undo;

		sched_dl_do_global();
	}
	ಯದಿ (0) {
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

	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sched_rr_handler(ಧ್ರುವ ಸಂರಚನಾ ctl_table *table, ಪೂರ್ಣಾಙ್ಕ ಲಿಖ, ಶೂನ್ಯ *buffer,
		size_t *lenp, loff_t *ppos)
{
	ಪೂರ್ಣಾಙ್ಕ ret;
	ಸ್ಥಿರ DEFINE_MUTEX(mutex);

	mutex_lock(&mutex);
	ret = proc_dointvec(table, ಲಿಖ, buffer, lenp, ppos);
	/*
	 * Make sure that internally we keep jiffies.
	 * Also, writing zero resets the time-slice to default:
	 */
	ಯದಿ (!ret && ಲಿಖ) {
		sched_rr_timeslice =
			sysctl_sched_rr_timeslice <= 0 ? RR_TIMESLICE :
			msecs_to_jiffies(sysctl_sched_rr_timeslice);

		ಯದಿ (sysctl_sched_rr_timeslice <= 0)
			sysctl_sched_rr_timeslice = jiffies_to_msecs(RR_TIMESLICE);
	}
	mutex_unlock(&mutex);

	ಪ್ರತ್ಯಾಗಮ ret;
}
#endif /* CONFIG_SYSCTL */

ಶೂನ್ಯ print_rt_stats(ಸಂರಚನಾ seq_file *m, ಪೂರ್ಣಾಙ್ಕ cpu)
{
	rt_rq_iter_t iter;
	ಸಂರಚನಾ rt_rq *rt_rq;

	rcu_read_lock();
	for_each_rt_rq(rt_rq, iter, cpu_rq(cpu))
		print_rt_rq(m, cpu, rt_rq);
	rcu_read_unlock();
}
