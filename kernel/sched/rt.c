// SPDX-License-Identifier: GPL-2.0
/*
 * Real-Time Scheduling Class (mapped to the SCHED_FIFO and SCHED_RR
 * policies)
 */

#அந்தர்பாவ "sched.h"
#அந்தர்பாவ "pelt.h"

முழு sched_rr_timeslice = RR_TIMESLICE;
/* More than 4 hours if BW_SHIFT equals 20. */
நிலையான மாறிலி u64 max_rt_runtime = MAX_BW;

/*
 * period over which we measure -rt task CPU usage in us.
 * default: 1s
 */
முழு sysctl_sched_rt_period = 1000000;

/*
 * part of the period that we allow rt tasks to run in us.
 * default: 1s
 */
முழு sysctl_sched_rt_runtime = 1000000;

#ifdef CONFIG_SYSCTL
நிலையான முழு sysctl_sched_rr_timeslice = (MSEC_PER_SEC * RR_TIMESLICE) / HZ;
நிலையான முழு sched_rt_handler(மாறிலி கட்டமைப்பு ctl_table *table, முழு லிக, வெறுமை *buffer,
		size_t *lenp, loff_t *ppos);
நிலையான முழு sched_rr_handler(மாறிலி கட்டமைப்பு ctl_table *table, முழு லிக, வெறுமை *buffer,
		size_t *lenp, loff_t *ppos);
நிலையான மாறிலி கட்டமைப்பு ctl_table sched_rt_sysctls[] = {
	{
		.procname       = "sched_rt_period_us",
		.data           = &sysctl_sched_rt_period,
		.maxlen         = அளவு(முழு),
		.mode           = 0644,
		.proc_handler   = sched_rt_handler,
		.extra1         = SYSCTL_ONE,
		.extra2         = SYSCTL_INT_MAX,
	},
	{
		.procname       = "sched_rt_runtime_us",
		.data           = &sysctl_sched_rt_runtime,
		.maxlen         = அளவு(முழு),
		.mode           = 0644,
		.proc_handler   = sched_rt_handler,
		.extra1         = SYSCTL_NEG_ONE,
		.extra2         = (வெறுமை *)&sysctl_sched_rt_period,
	},
	{
		.procname       = "sched_rr_timeslice_ms",
		.data           = &sysctl_sched_rr_timeslice,
		.maxlen         = அளவு(முழு),
		.mode           = 0644,
		.proc_handler   = sched_rr_handler,
	},
};

நிலையான முழு __init sched_rt_sysctl_init(வெறுமை)
{
	register_sysctl_init("kernel", sched_rt_sysctls);
	திரும்பு 0;
}
late_initcall(sched_rt_sysctl_init);
#endif /* CONFIG_SYSCTL */

வெறுமை init_rt_rq(கட்டமைப்பு rt_rq *rt_rq)
{
	கட்டமைப்பு rt_prio_array *array;
	முழு i;

	array = &rt_rq->active;
	க்கு (i = 0; i < MAX_RT_PRIO; i++) {
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
	rt_rq->tg = &வேர்_task_group;
#endif
}

#ifdef CONFIG_RT_GROUP_SCHED

நிலையான முழு do_sched_rt_period_timer(கட்டமைப்பு rt_bandwidth *rt_b, முழு overrun);

நிலையான எண்ணிக்கை hrtimer_restart sched_rt_period_timer(கட்டமைப்பு hrtimer *timer)
{
	கட்டமைப்பு rt_bandwidth *rt_b =
		container_of(timer, கட்டமைப்பு rt_bandwidth, rt_period_timer);
	முழு idle = 0;
	முழு overrun;

	raw_spin_lock(&rt_b->rt_runtime_lock);
	க்கு (;;) {
		overrun = hrtimer_forward_now(timer, rt_b->rt_period);
		எனில் (!overrun)
			நிறுத்து;

		raw_spin_unlock(&rt_b->rt_runtime_lock);
		idle = do_sched_rt_period_timer(rt_b, overrun);
		raw_spin_lock(&rt_b->rt_runtime_lock);
	}
	எனில் (idle)
		rt_b->rt_period_active = 0;
	raw_spin_unlock(&rt_b->rt_runtime_lock);

	திரும்பு idle ? HRTIMER_NORESTART : HRTIMER_RESTART;
}

வெறுமை init_rt_bandwidth(கட்டமைப்பு rt_bandwidth *rt_b, u64 period, u64 runtime)
{
	rt_b->rt_period = ns_to_ktime(period);
	rt_b->rt_runtime = runtime;

	raw_spin_lock_init(&rt_b->rt_runtime_lock);

	hrtimer_setup(&rt_b->rt_period_timer, sched_rt_period_timer, CLOCK_MONOTONIC,
		      HRTIMER_MODE_REL_HARD);
}

நிலையான உள்வரி வெறுமை do_start_rt_bandwidth(கட்டமைப்பு rt_bandwidth *rt_b)
{
	raw_spin_lock(&rt_b->rt_runtime_lock);
	எனில் (!rt_b->rt_period_active) {
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

நிலையான வெறுமை start_rt_bandwidth(கட்டமைப்பு rt_bandwidth *rt_b)
{
	எனில் (!rt_bandwidth_enabled() || rt_b->rt_runtime == RUNTIME_INF)
		திரும்பு;

	do_start_rt_bandwidth(rt_b);
}

நிலையான வெறுமை destroy_rt_bandwidth(கட்டமைப்பு rt_bandwidth *rt_b)
{
	hrtimer_cancel(&rt_b->rt_period_timer);
}

#define rt_entity_is_task(rt_se) (!(rt_se)->my_q)

நிலையான உள்வரி கட்டமைப்பு task_struct *rt_task_of(கட்டமைப்பு sched_rt_entity *rt_se)
{
	WARN_ON_ONCE(!rt_entity_is_task(rt_se));

	திரும்பு container_of(rt_se, கட்டமைப்பு task_struct, rt);
}

நிலையான உள்வரி கட்டமைப்பு rq *rq_of_rt_rq(கட்டமைப்பு rt_rq *rt_rq)
{
	/* Cannot fold with non-CONFIG_RT_GROUP_SCHED version, layout */
	WARN_ON(!rt_group_sched_enabled() && rt_rq->tg != &வேர்_task_group);
	திரும்பு rt_rq->rq;
}

நிலையான உள்வரி கட்டமைப்பு rt_rq *rt_rq_of_se(கட்டமைப்பு sched_rt_entity *rt_se)
{
	WARN_ON(!rt_group_sched_enabled() && rt_se->rt_rq->tg != &வேர்_task_group);
	திரும்பு rt_se->rt_rq;
}

நிலையான உள்வரி கட்டமைப்பு rq *rq_of_rt_se(கட்டமைப்பு sched_rt_entity *rt_se)
{
	கட்டமைப்பு rt_rq *rt_rq = rt_se->rt_rq;

	WARN_ON(!rt_group_sched_enabled() && rt_rq->tg != &வேர்_task_group);
	திரும்பு rt_rq->rq;
}

வெறுமை unregister_rt_sched_group(கட்டமைப்பு task_group *tg)
{
	எனில் (!rt_group_sched_enabled())
		திரும்பு;

	எனில் (tg->rt_se)
		destroy_rt_bandwidth(&tg->rt_bandwidth);
}

வெறுமை free_rt_sched_group(கட்டமைப்பு task_group *tg)
{
	முழு i;

	எனில் (!rt_group_sched_enabled())
		திரும்பு;

	for_each_possible_cpu(i) {
		எனில் (tg->rt_rq)
			kfree(tg->rt_rq[i]);
		எனில் (tg->rt_se)
			kfree(tg->rt_se[i]);
	}

	kfree(tg->rt_rq);
	kfree(tg->rt_se);
}

வெறுமை init_tg_rt_entry(கட்டமைப்பு task_group *tg, கட்டமைப்பு rt_rq *rt_rq,
		கட்டமைப்பு sched_rt_entity *rt_se, முழு cpu,
		கட்டமைப்பு sched_rt_entity *parent)
{
	கட்டமைப்பு rq *rq = cpu_rq(cpu);

	rt_rq->highest_prio.curr = MAX_RT_PRIO-1;
	rt_rq->rt_nr_boosted = 0;
	rt_rq->rq = rq;
	rt_rq->tg = tg;

	tg->rt_rq[cpu] = rt_rq;
	tg->rt_se[cpu] = rt_se;

	எனில் (!rt_se)
		திரும்பு;

	எனில் (!parent)
		rt_se->rt_rq = &rq->rt;
	இல்லையெனில்
		rt_se->rt_rq = parent->my_q;

	rt_se->my_q = rt_rq;
	rt_se->parent = parent;
	INIT_LIST_HEAD(&rt_se->run_list);
}

முழு alloc_rt_sched_group(கட்டமைப்பு task_group *tg, கட்டமைப்பு task_group *parent)
{
	கட்டமைப்பு rt_rq *rt_rq;
	கட்டமைப்பு sched_rt_entity *rt_se;
	முழு i;

	எனில் (!rt_group_sched_enabled())
		திரும்பு 1;

	tg->rt_rq = kzalloc_objs(rt_rq, nr_cpu_ids);
	எனில் (!tg->rt_rq)
		செல் err;
	tg->rt_se = kzalloc_objs(rt_se, nr_cpu_ids);
	எனில் (!tg->rt_se)
		செல் err;

	init_rt_bandwidth(&tg->rt_bandwidth, ktime_to_ns(global_rt_period()), 0);

	for_each_possible_cpu(i) {
		rt_rq = kzalloc_முனை(அளவு(கட்டமைப்பு rt_rq),
				     GFP_KERNEL, cpu_to_முனை(i));
		எனில் (!rt_rq)
			செல் err;

		rt_se = kzalloc_முனை(அளவு(கட்டமைப்பு sched_rt_entity),
				     GFP_KERNEL, cpu_to_முனை(i));
		எனில் (!rt_se)
			செல் err_free_rq;

		init_rt_rq(rt_rq);
		rt_rq->rt_runtime = tg->rt_bandwidth.rt_runtime;
		init_tg_rt_entry(tg, rt_rq, rt_se, i, parent->rt_se[i]);
	}

	திரும்பு 1;

err_free_rq:
	kfree(rt_rq);
err:
	திரும்பு 0;
}

#இல்லையெனில் /* !CONFIG_RT_GROUP_SCHED: */

#define rt_entity_is_task(rt_se) (1)

நிலையான உள்வரி கட்டமைப்பு task_struct *rt_task_of(கட்டமைப்பு sched_rt_entity *rt_se)
{
	திரும்பு container_of(rt_se, கட்டமைப்பு task_struct, rt);
}

நிலையான உள்வரி கட்டமைப்பு rq *rq_of_rt_rq(கட்டமைப்பு rt_rq *rt_rq)
{
	திரும்பு container_of(rt_rq, கட்டமைப்பு rq, rt);
}

நிலையான உள்வரி கட்டமைப்பு rq *rq_of_rt_se(கட்டமைப்பு sched_rt_entity *rt_se)
{
	கட்டமைப்பு task_struct *p = rt_task_of(rt_se);

	திரும்பு task_rq(p);
}

நிலையான உள்வரி கட்டமைப்பு rt_rq *rt_rq_of_se(கட்டமைப்பு sched_rt_entity *rt_se)
{
	கட்டமைப்பு rq *rq = rq_of_rt_se(rt_se);

	திரும்பு &rq->rt;
}

வெறுமை unregister_rt_sched_group(கட்டமைப்பு task_group *tg) { }

வெறுமை free_rt_sched_group(கட்டமைப்பு task_group *tg) { }

முழு alloc_rt_sched_group(கட்டமைப்பு task_group *tg, கட்டமைப்பு task_group *parent)
{
	திரும்பு 1;
}
#endif /* !CONFIG_RT_GROUP_SCHED */

நிலையான உள்வரி bool need_pull_rt_task(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *prev)
{
	/* Try to pull RT tasks here if we lower this rq's prio */
	திரும்பு rq->online && rq->rt.highest_prio.curr > prev->prio;
}

நிலையான உள்வரி முழு rt_overloaded(கட்டமைப்பு rq *rq)
{
	திரும்பு atomic_read(&rq->rd->rto_மொத்தம்);
}

நிலையான உள்வரி வெறுமை rt_set_overload(கட்டமைப்பு rq *rq)
{
	எனில் (!rq->online)
		திரும்பு;

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
	atomic_inc(&rq->rd->rto_மொத்தம்);
}

நிலையான உள்வரி வெறுமை rt_clear_overload(கட்டமைப்பு rq *rq)
{
	எனில் (!rq->online)
		திரும்பு;

	/* the order here really doesn't matter */
	atomic_dec(&rq->rd->rto_மொத்தம்);
	cpumask_clear_cpu(rq->cpu, rq->rd->rto_mask);
}

நிலையான உள்வரி முழு has_pushable_tasks(கட்டமைப்பு rq *rq)
{
	திரும்பு !plist_head_empty(&rq->rt.pushable_tasks);
}

நிலையான DEFINE_PER_CPU(கட்டமைப்பு balance_callback, rt_தள்ளு_head);
நிலையான DEFINE_PER_CPU(கட்டமைப்பு balance_callback, rt_pull_head);

நிலையான வெறுமை தள்ளு_rt_tasks(கட்டமைப்பு rq *);
நிலையான வெறுமை pull_rt_task(கட்டமைப்பு rq *);

நிலையான உள்வரி வெறுமை rt_queue_தள்ளு_tasks(கட்டமைப்பு rq *rq)
{
	எனில் (!has_pushable_tasks(rq))
		திரும்பு;

	queue_balance_callback(rq, &per_cpu(rt_தள்ளு_head, rq->cpu), தள்ளு_rt_tasks);
}

நிலையான உள்வரி வெறுமை rt_queue_pull_task(கட்டமைப்பு rq *rq)
{
	queue_balance_callback(rq, &per_cpu(rt_pull_head, rq->cpu), pull_rt_task);
}

நிலையான வெறுமை enqueue_pushable_task(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p)
{
	plist_del(&p->pushable_tasks, &rq->rt.pushable_tasks);
	plist_முனை_init(&p->pushable_tasks, p->prio);
	plist_add(&p->pushable_tasks, &rq->rt.pushable_tasks);

	/* Update the highest prio pushable task */
	எனில் (p->prio < rq->rt.highest_prio.next)
		rq->rt.highest_prio.next = p->prio;

	எனில் (!rq->rt.overloaded) {
		rt_set_overload(rq);
		rq->rt.overloaded = 1;
	}
}

நிலையான வெறுமை dequeue_pushable_task(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p)
{
	plist_del(&p->pushable_tasks, &rq->rt.pushable_tasks);

	/* Update the new highest prio pushable task */
	எனில் (has_pushable_tasks(rq)) {
		p = plist_first_entry(&rq->rt.pushable_tasks,
				      கட்டமைப்பு task_struct, pushable_tasks);
		rq->rt.highest_prio.next = p->prio;
	} இல்லையெனில் {
		rq->rt.highest_prio.next = MAX_RT_PRIO-1;

		எனில் (rq->rt.overloaded) {
			rt_clear_overload(rq);
			rq->rt.overloaded = 0;
		}
	}
}

நிலையான வெறுமை enqueue_உச்சி_rt_rq(கட்டமைப்பு rt_rq *rt_rq);
நிலையான வெறுமை dequeue_உச்சி_rt_rq(கட்டமைப்பு rt_rq *rt_rq, unsigned முழு மொத்தம்);

நிலையான உள்வரி முழு on_rt_rq(கட்டமைப்பு sched_rt_entity *rt_se)
{
	திரும்பு rt_se->on_rq;
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
நிலையான உள்வரி bool rt_task_fits_capacity(கட்டமைப்பு task_struct *p, முழு cpu)
{
	unsigned முழு min_cap;
	unsigned முழு max_cap;
	unsigned முழு cpu_cap;

	/* Only heterogeneous systems can benefit from this check */
	எனில் (!sched_asym_cpucap_active())
		திரும்பு true;

	min_cap = uclamp_eff_மதிப்பு(p, UCLAMP_MIN);
	max_cap = uclamp_eff_மதிப்பு(p, UCLAMP_MAX);

	cpu_cap = arch_scale_cpu_capacity(cpu);

	திரும்பு cpu_cap >= min(min_cap, max_cap);
}
#இல்லையெனில் /* !CONFIG_UCLAMP_TASK: */
நிலையான உள்வரி bool rt_task_fits_capacity(கட்டமைப்பு task_struct *p, முழு cpu)
{
	திரும்பு true;
}
#endif /* !CONFIG_UCLAMP_TASK */

#ifdef CONFIG_RT_GROUP_SCHED

நிலையான உள்வரி u64 sched_rt_runtime(கட்டமைப்பு rt_rq *rt_rq)
{
	திரும்பு rt_rq->rt_runtime;
}

நிலையான உள்வரி u64 sched_rt_period(கட்டமைப்பு rt_rq *rt_rq)
{
	திரும்பு ktime_to_ns(rt_rq->tg->rt_bandwidth.rt_period);
}

வகைவரை கட்டமைப்பு task_group *rt_rq_iter_t;

நிலையான உள்வரி கட்டமைப்பு task_group *next_task_group(கட்டமைப்பு task_group *tg)
{
	எனில் (!rt_group_sched_enabled()) {
		WARN_ON(tg != &வேர்_task_group);
		திரும்பு வெற்று;
	}

	செய் {
		tg = list_entry_rcu(tg->list.next,
			typeof(கட்டமைப்பு task_group), list);
	} போது (&tg->list != &task_groups && task_group_is_autogroup(tg));

	எனில் (&tg->list == &task_groups)
		tg = வெற்று;

	திரும்பு tg;
}

#define for_each_rt_rq(rt_rq, iter, rq)					\
	க்கு (iter = &வேர்_task_group;					\
		iter && (rt_rq = iter->rt_rq[cpu_of(rq)]);		\
		iter = next_task_group(iter))

#define for_each_sched_rt_entity(rt_se) \
	க்கு (; rt_se; rt_se = rt_se->parent)

நிலையான உள்வரி கட்டமைப்பு rt_rq *group_rt_rq(கட்டமைப்பு sched_rt_entity *rt_se)
{
	திரும்பு rt_se->my_q;
}

நிலையான வெறுமை enqueue_rt_entity(கட்டமைப்பு sched_rt_entity *rt_se, unsigned முழு flags);
நிலையான வெறுமை dequeue_rt_entity(கட்டமைப்பு sched_rt_entity *rt_se, unsigned முழு flags);

நிலையான வெறுமை sched_rt_rq_enqueue(கட்டமைப்பு rt_rq *rt_rq)
{
	கட்டமைப்பு task_struct *donor = rq_of_rt_rq(rt_rq)->donor;
	கட்டமைப்பு rq *rq = rq_of_rt_rq(rt_rq);
	கட்டமைப்பு sched_rt_entity *rt_se;

	முழு cpu = cpu_of(rq);

	rt_se = rt_rq->tg->rt_se[cpu];

	எனில் (rt_rq->rt_nr_running) {
		எனில் (!rt_se)
			enqueue_உச்சி_rt_rq(rt_rq);
		இல்லையெனில் எனில் (!on_rt_rq(rt_se))
			enqueue_rt_entity(rt_se, 0);

		எனில் (rt_rq->highest_prio.curr < donor->prio)
			resched_curr(rq);
	}
}

நிலையான வெறுமை sched_rt_rq_dequeue(கட்டமைப்பு rt_rq *rt_rq)
{
	கட்டமைப்பு sched_rt_entity *rt_se;
	முழு cpu = cpu_of(rq_of_rt_rq(rt_rq));

	rt_se = rt_rq->tg->rt_se[cpu];

	எனில் (!rt_se) {
		dequeue_உச்சி_rt_rq(rt_rq, rt_rq->rt_nr_running);
		/* Kick cpufreq (see the comment in kernel/sched/sched.h). */
		cpufreq_update_util(rq_of_rt_rq(rt_rq), 0);
	}
	இல்லையெனில் எனில் (on_rt_rq(rt_se))
		dequeue_rt_entity(rt_se, 0);
}

நிலையான உள்வரி முழு rt_rq_throttled(கட்டமைப்பு rt_rq *rt_rq)
{
	திரும்பு rt_rq->rt_throttled && !rt_rq->rt_nr_boosted;
}

நிலையான முழு rt_se_boosted(கட்டமைப்பு sched_rt_entity *rt_se)
{
	கட்டமைப்பு rt_rq *rt_rq = group_rt_rq(rt_se);
	கட்டமைப்பு task_struct *p;

	எனில் (rt_rq)
		திரும்பு !!rt_rq->rt_nr_boosted;

	p = rt_task_of(rt_se);
	திரும்பு p->prio != p->normal_prio;
}

நிலையான உள்வரி மாறிலி கட்டமைப்பு cpumask *sched_rt_period_mask(வெறுமை)
{
	திரும்பு this_rq()->rd->span;
}

நிலையான உள்வரி
கட்டமைப்பு rt_rq *sched_rt_period_rt_rq(கட்டமைப்பு rt_bandwidth *rt_b, முழு cpu)
{
	திரும்பு container_of(rt_b, கட்டமைப்பு task_group, rt_bandwidth)->rt_rq[cpu];
}

நிலையான உள்வரி கட்டமைப்பு rt_bandwidth *sched_rt_bandwidth(கட்டமைப்பு rt_rq *rt_rq)
{
	திரும்பு &rt_rq->tg->rt_bandwidth;
}

bool sched_rt_bandwidth_account(கட்டமைப்பு rt_rq *rt_rq)
{
	கட்டமைப்பு rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);

	திரும்பு (hrtimer_active(&rt_b->rt_period_timer) ||
		rt_rq->rt_time < rt_b->rt_runtime);
}

/*
 * We ran out of runtime, see if we can borrow some from our neighbours.
 */
நிலையான வெறுமை do_balance_runtime(கட்டமைப்பு rt_rq *rt_rq)
{
	கட்டமைப்பு rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);
	கட்டமைப்பு வேர்_domain *rd = rq_of_rt_rq(rt_rq)->rd;
	முழு i, weight;
	u64 rt_period;

	weight = cpumask_weight(rd->span);

	raw_spin_lock(&rt_b->rt_runtime_lock);
	rt_period = ktime_to_ns(rt_b->rt_period);
	for_each_cpu(i, rd->span) {
		கட்டமைப்பு rt_rq *iter = sched_rt_period_rt_rq(rt_b, i);
		s64 diff;

		எனில் (iter == rt_rq)
			தொடர்;

		raw_spin_lock(&iter->rt_runtime_lock);
		/*
		 * Either all rqs have inf runtime and there's nothing to steal
		 * or __disable_runtime() below sets a specific rq to inf to
		 * indicate its been disabled and disallow stealing.
		 */
		எனில் (iter->rt_runtime == RUNTIME_INF)
			செல் next;

		/*
		 * From runqueues with spare time, take 1/n part of their
		 * spare time, but no more than our period.
		 */
		diff = iter->rt_runtime - iter->rt_time;
		எனில் (diff > 0) {
			diff = div_u64((u64)diff, weight);
			எனில் (rt_rq->rt_runtime + diff > rt_period)
				diff = rt_period - rt_rq->rt_runtime;
			iter->rt_runtime -= diff;
			rt_rq->rt_runtime += diff;
			எனில் (rt_rq->rt_runtime == rt_period) {
				raw_spin_unlock(&iter->rt_runtime_lock);
				நிறுத்து;
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
நிலையான வெறுமை __disable_runtime(கட்டமைப்பு rq *rq)
{
	கட்டமைப்பு வேர்_domain *rd = rq->rd;
	rt_rq_iter_t iter;
	கட்டமைப்பு rt_rq *rt_rq;

	எனில் (unlikely(!scheduler_running))
		திரும்பு;

	for_each_rt_rq(rt_rq, iter, rq) {
		கட்டமைப்பு rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);
		s64 want;
		முழு i;

		raw_spin_lock(&rt_b->rt_runtime_lock);
		raw_spin_lock(&rt_rq->rt_runtime_lock);
		/*
		 * Either we're all inf and nobody needs to borrow, or we're
		 * already disabled and thus have nothing to do, or we have
		 * exactly the right amount of runtime to take out.
		 */
		எனில் (rt_rq->rt_runtime == RUNTIME_INF ||
				rt_rq->rt_runtime == rt_b->rt_runtime)
			செல் balanced;
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
			கட்டமைப்பு rt_rq *iter = sched_rt_period_rt_rq(rt_b, i);
			s64 diff;

			/*
			 * Can't reclaim from ourselves or disabled runqueues.
			 */
			எனில் (iter == rt_rq || iter->rt_runtime == RUNTIME_INF)
				தொடர்;

			raw_spin_lock(&iter->rt_runtime_lock);
			எனில் (want > 0) {
				diff = min_t(s64, iter->rt_runtime, want);
				iter->rt_runtime -= diff;
				want -= diff;
			} இல்லையெனில் {
				iter->rt_runtime -= want;
				want -= want;
			}
			raw_spin_unlock(&iter->rt_runtime_lock);

			எனில் (!want)
				நிறுத்து;
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

நிலையான வெறுமை __enable_runtime(கட்டமைப்பு rq *rq)
{
	rt_rq_iter_t iter;
	கட்டமைப்பு rt_rq *rt_rq;

	எனில் (unlikely(!scheduler_running))
		திரும்பு;

	/*
	 * Reset each runqueue's bandwidth settings
	 */
	for_each_rt_rq(rt_rq, iter, rq) {
		கட்டமைப்பு rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);

		raw_spin_lock(&rt_b->rt_runtime_lock);
		raw_spin_lock(&rt_rq->rt_runtime_lock);
		rt_rq->rt_runtime = rt_b->rt_runtime;
		rt_rq->rt_time = 0;
		rt_rq->rt_throttled = 0;
		raw_spin_unlock(&rt_rq->rt_runtime_lock);
		raw_spin_unlock(&rt_b->rt_runtime_lock);
	}
}

நிலையான வெறுமை balance_runtime(கட்டமைப்பு rt_rq *rt_rq)
{
	எனில் (!sched_feat(RT_RUNTIME_SHARE))
		திரும்பு;

	எனில் (rt_rq->rt_time > rt_rq->rt_runtime) {
		raw_spin_unlock(&rt_rq->rt_runtime_lock);
		do_balance_runtime(rt_rq);
		raw_spin_lock(&rt_rq->rt_runtime_lock);
	}
}

நிலையான முழு do_sched_rt_period_timer(கட்டமைப்பு rt_bandwidth *rt_b, முழு overrun)
{
	முழு i, idle = 1, throttled = 0;
	மாறிலி கட்டமைப்பு cpumask *span;

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
	எனில் (rt_b == &வேர்_task_group.rt_bandwidth)
		span = cpu_online_mask;

	for_each_cpu(i, span) {
		முழு enqueue = 0;
		கட்டமைப்பு rt_rq *rt_rq = sched_rt_period_rt_rq(rt_b, i);
		கட்டமைப்பு rq *rq = rq_of_rt_rq(rt_rq);
		கட்டமைப்பு rq_flags rf;
		முழு skip;

		/*
		 * When span == cpu_online_mask, taking each rq->lock
		 * can be time-consuming. Try to avoid it when possible.
		 */
		raw_spin_lock(&rt_rq->rt_runtime_lock);
		எனில் (!sched_feat(RT_RUNTIME_SHARE) && rt_rq->rt_runtime != RUNTIME_INF)
			rt_rq->rt_runtime = rt_b->rt_runtime;
		skip = !rt_rq->rt_time && !rt_rq->rt_nr_running;
		raw_spin_unlock(&rt_rq->rt_runtime_lock);
		எனில் (skip)
			தொடர்;

		rq_lock(rq, &rf);
		update_rq_clock(rq);

		எனில் (rt_rq->rt_time) {
			u64 runtime;

			raw_spin_lock(&rt_rq->rt_runtime_lock);
			எனில் (rt_rq->rt_throttled)
				balance_runtime(rt_rq);
			runtime = rt_rq->rt_runtime;
			rt_rq->rt_time -= min(rt_rq->rt_time, overrun*runtime);
			எனில் (rt_rq->rt_throttled && rt_rq->rt_time < runtime) {
				rt_rq->rt_throttled = 0;
				enqueue = 1;

				/*
				 * When we're idle and a woken (rt) task is
				 * throttled wakeup_preempt() will set
				 * skip_update and the time between the wakeup
				 * and this unthrottle will get accounted as
				 * 'runtime'.
				 */
				எனில் (rt_rq->rt_nr_running && rq->curr == rq->idle)
					rq_clock_cancel_skipupdate(rq);
			}
			எனில் (rt_rq->rt_time || rt_rq->rt_nr_running)
				idle = 0;
			raw_spin_unlock(&rt_rq->rt_runtime_lock);
		} இல்லையெனில் எனில் (rt_rq->rt_nr_running) {
			idle = 0;
			எனில் (!rt_rq_throttled(rt_rq))
				enqueue = 1;
		}
		எனில் (rt_rq->rt_throttled)
			throttled = 1;

		எனில் (enqueue)
			sched_rt_rq_enqueue(rt_rq);
		rq_unlock(rq, &rf);
	}

	எனில் (!throttled && (!rt_bandwidth_enabled() || rt_b->rt_runtime == RUNTIME_INF))
		திரும்பு 1;

	திரும்பு idle;
}

நிலையான முழு sched_rt_runtime_exceeded(கட்டமைப்பு rt_rq *rt_rq)
{
	u64 runtime = sched_rt_runtime(rt_rq);

	எனில் (rt_rq->rt_throttled)
		திரும்பு rt_rq_throttled(rt_rq);

	எனில் (runtime >= sched_rt_period(rt_rq))
		திரும்பு 0;

	balance_runtime(rt_rq);
	runtime = sched_rt_runtime(rt_rq);
	எனில் (runtime == RUNTIME_INF)
		திரும்பு 0;

	எனில் (rt_rq->rt_time > runtime) {
		கட்டமைப்பு rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);

		/*
		 * Don't actually throttle groups that have no runtime assigned
		 * but accrue some time due to boosting.
		 */
		எனில் (likely(rt_b->rt_runtime)) {
			rt_rq->rt_throttled = 1;
			printk_deferred_once("sched: RT throttling activated\n");
		} இல்லையெனில் {
			/*
			 * In case we did anyway, make it go away,
			 * replenishment is a joke, since it will replenish us
			 * with exactly 0 ns.
			 */
			rt_rq->rt_time = 0;
		}

		எனில் (rt_rq_throttled(rt_rq)) {
			sched_rt_rq_dequeue(rt_rq);
			திரும்பு 1;
		}
	}

	திரும்பு 0;
}

#இல்லையெனில் /* !CONFIG_RT_GROUP_SCHED: */

வகைவரை கட்டமைப்பு rt_rq *rt_rq_iter_t;

#define for_each_rt_rq(rt_rq, iter, rq) \
	க்கு ((வெறுமை) iter, rt_rq = &rq->rt; rt_rq; rt_rq = வெற்று)

#define for_each_sched_rt_entity(rt_se) \
	க்கு (; rt_se; rt_se = வெற்று)

நிலையான உள்வரி கட்டமைப்பு rt_rq *group_rt_rq(கட்டமைப்பு sched_rt_entity *rt_se)
{
	திரும்பு வெற்று;
}

நிலையான உள்வரி வெறுமை sched_rt_rq_enqueue(கட்டமைப்பு rt_rq *rt_rq)
{
	கட்டமைப்பு rq *rq = rq_of_rt_rq(rt_rq);

	எனில் (!rt_rq->rt_nr_running)
		திரும்பு;

	enqueue_உச்சி_rt_rq(rt_rq);
	resched_curr(rq);
}

நிலையான உள்வரி வெறுமை sched_rt_rq_dequeue(கட்டமைப்பு rt_rq *rt_rq)
{
	dequeue_உச்சி_rt_rq(rt_rq, rt_rq->rt_nr_running);
}

நிலையான உள்வரி முழு rt_rq_throttled(கட்டமைப்பு rt_rq *rt_rq)
{
	திரும்பு false;
}

நிலையான உள்வரி மாறிலி கட்டமைப்பு cpumask *sched_rt_period_mask(வெறுமை)
{
	திரும்பு cpu_online_mask;
}

நிலையான உள்வரி
கட்டமைப்பு rt_rq *sched_rt_period_rt_rq(கட்டமைப்பு rt_bandwidth *rt_b, முழு cpu)
{
	திரும்பு &cpu_rq(cpu)->rt;
}

நிலையான வெறுமை __enable_runtime(கட்டமைப்பு rq *rq) { }
நிலையான வெறுமை __disable_runtime(கட்டமைப்பு rq *rq) { }

#endif /* !CONFIG_RT_GROUP_SCHED */

நிலையான உள்வரி முழு rt_se_prio(கட்டமைப்பு sched_rt_entity *rt_se)
{
#ifdef CONFIG_RT_GROUP_SCHED
	கட்டமைப்பு rt_rq *rt_rq = group_rt_rq(rt_se);

	எனில் (rt_rq)
		திரும்பு rt_rq->highest_prio.curr;
#endif

	திரும்பு rt_task_of(rt_se)->prio;
}

/*
 * Update the current task's runtime statistics. Skip current tasks that
 * are not in our scheduling class.
 */
நிலையான வெறுமை update_curr_rt(கட்டமைப்பு rq *rq)
{
	கட்டமைப்பு task_struct *donor = rq->donor;
	s64 delta_exec;

	எனில் (donor->sched_class != &rt_sched_class)
		திரும்பு;

	delta_exec = update_curr_common(rq);
	எனில் (unlikely(delta_exec <= 0))
		திரும்பு;

#ifdef CONFIG_RT_GROUP_SCHED
	கட்டமைப்பு sched_rt_entity *rt_se = &donor->rt;

	எனில் (!rt_bandwidth_enabled())
		திரும்பு;

	for_each_sched_rt_entity(rt_se) {
		கட்டமைப்பு rt_rq *rt_rq = rt_rq_of_se(rt_se);
		முழு exceeded;

		எனில் (sched_rt_runtime(rt_rq) != RUNTIME_INF) {
			raw_spin_lock(&rt_rq->rt_runtime_lock);
			rt_rq->rt_time += delta_exec;
			exceeded = sched_rt_runtime_exceeded(rt_rq);
			எனில் (exceeded)
				resched_curr(rq);
			raw_spin_unlock(&rt_rq->rt_runtime_lock);
			எனில் (exceeded)
				do_start_rt_bandwidth(sched_rt_bandwidth(rt_rq));
		}
	}
#endif /* CONFIG_RT_GROUP_SCHED */
}

நிலையான வெறுமை
dequeue_உச்சி_rt_rq(கட்டமைப்பு rt_rq *rt_rq, unsigned முழு மொத்தம்)
{
	கட்டமைப்பு rq *rq = rq_of_rt_rq(rt_rq);

	BUG_ON(&rq->rt != rt_rq);

	எனில் (!rt_rq->rt_queued)
		திரும்பு;

	BUG_ON(!rq->nr_running);

	sub_nr_running(rq, மொத்தம்);
	rt_rq->rt_queued = 0;

}

நிலையான வெறுமை
enqueue_உச்சி_rt_rq(கட்டமைப்பு rt_rq *rt_rq)
{
	கட்டமைப்பு rq *rq = rq_of_rt_rq(rt_rq);

	BUG_ON(&rq->rt != rt_rq);

	எனில் (rt_rq->rt_queued)
		திரும்பு;

	எனில் (rt_rq_throttled(rt_rq))
		திரும்பு;

	எனில் (rt_rq->rt_nr_running) {
		add_nr_running(rq, rt_rq->rt_nr_running);
		rt_rq->rt_queued = 1;
	}

	/* Kick cpufreq (see the comment in kernel/sched/sched.h). */
	cpufreq_update_util(rq, 0);
}

நிலையான வெறுமை
inc_rt_prio_smp(கட்டமைப்பு rt_rq *rt_rq, முழு prio, முழு prev_prio)
{
	கட்டமைப்பு rq *rq = rq_of_rt_rq(rt_rq);

	/*
	 * Change rq's cpupri only if rt_rq is the top queue.
	 */
	எனில் (IS_ENABLED(CONFIG_RT_GROUP_SCHED) && &rq->rt != rt_rq)
		திரும்பு;

	எனில் (rq->online && prio < prev_prio)
		cpupri_set(&rq->rd->cpupri, rq->cpu, prio);
}

நிலையான வெறுமை
dec_rt_prio_smp(கட்டமைப்பு rt_rq *rt_rq, முழு prio, முழு prev_prio)
{
	கட்டமைப்பு rq *rq = rq_of_rt_rq(rt_rq);

	/*
	 * Change rq's cpupri only if rt_rq is the top queue.
	 */
	எனில் (IS_ENABLED(CONFIG_RT_GROUP_SCHED) && &rq->rt != rt_rq)
		திரும்பு;

	எனில் (rq->online && rt_rq->highest_prio.curr != prev_prio)
		cpupri_set(&rq->rd->cpupri, rq->cpu, rt_rq->highest_prio.curr);
}

நிலையான வெறுமை
inc_rt_prio(கட்டமைப்பு rt_rq *rt_rq, முழு prio)
{
	முழு prev_prio = rt_rq->highest_prio.curr;

	எனில் (prio < prev_prio)
		rt_rq->highest_prio.curr = prio;

	inc_rt_prio_smp(rt_rq, prio, prev_prio);
}

நிலையான வெறுமை
dec_rt_prio(கட்டமைப்பு rt_rq *rt_rq, முழு prio)
{
	முழு prev_prio = rt_rq->highest_prio.curr;

	எனில் (rt_rq->rt_nr_running) {

		WARN_ON(prio < prev_prio);

		/*
		 * This may have been our highest task, and therefore
		 * we may have some re-computation to do
		 */
		எனில் (prio == prev_prio) {
			கட்டமைப்பு rt_prio_array *array = &rt_rq->active;

			rt_rq->highest_prio.curr =
				sched_find_first_bit(array->bitmap);
		}

	} இல்லையெனில் {
		rt_rq->highest_prio.curr = MAX_RT_PRIO-1;
	}

	dec_rt_prio_smp(rt_rq, prio, prev_prio);
}

#ifdef CONFIG_RT_GROUP_SCHED

நிலையான வெறுமை
inc_rt_group(கட்டமைப்பு sched_rt_entity *rt_se, கட்டமைப்பு rt_rq *rt_rq)
{
	எனில் (rt_se_boosted(rt_se))
		rt_rq->rt_nr_boosted++;

	start_rt_bandwidth(&rt_rq->tg->rt_bandwidth);
}

நிலையான வெறுமை
dec_rt_group(கட்டமைப்பு sched_rt_entity *rt_se, கட்டமைப்பு rt_rq *rt_rq)
{
	எனில் (rt_se_boosted(rt_se))
		rt_rq->rt_nr_boosted--;

	WARN_ON(!rt_rq->rt_nr_running && rt_rq->rt_nr_boosted);
}

#இல்லையெனில் /* !CONFIG_RT_GROUP_SCHED: */

நிலையான வெறுமை
inc_rt_group(கட்டமைப்பு sched_rt_entity *rt_se, கட்டமைப்பு rt_rq *rt_rq)
{
}

நிலையான உள்வரி
வெறுமை dec_rt_group(கட்டமைப்பு sched_rt_entity *rt_se, கட்டமைப்பு rt_rq *rt_rq) {}

#endif /* !CONFIG_RT_GROUP_SCHED */

நிலையான உள்வரி
unsigned முழு rt_se_nr_running(கட்டமைப்பு sched_rt_entity *rt_se)
{
	கட்டமைப்பு rt_rq *group_rq = group_rt_rq(rt_se);

	எனில் (group_rq)
		திரும்பு group_rq->rt_nr_running;
	இல்லையெனில்
		திரும்பு 1;
}

நிலையான உள்வரி
unsigned முழு rt_se_rr_nr_running(கட்டமைப்பு sched_rt_entity *rt_se)
{
	கட்டமைப்பு rt_rq *group_rq = group_rt_rq(rt_se);
	கட்டமைப்பு task_struct *tsk;

	எனில் (group_rq)
		திரும்பு group_rq->rr_nr_running;

	tsk = rt_task_of(rt_se);

	திரும்பு (tsk->policy == SCHED_RR) ? 1 : 0;
}

நிலையான உள்வரி
வெறுமை inc_rt_tasks(கட்டமைப்பு sched_rt_entity *rt_se, கட்டமைப்பு rt_rq *rt_rq)
{
	முழு prio = rt_se_prio(rt_se);

	WARN_ON(!rt_prio(prio));
	rt_rq->rt_nr_running += rt_se_nr_running(rt_se);
	rt_rq->rr_nr_running += rt_se_rr_nr_running(rt_se);

	inc_rt_prio(rt_rq, prio);
	inc_rt_group(rt_se, rt_rq);
}

நிலையான உள்வரி
வெறுமை dec_rt_tasks(கட்டமைப்பு sched_rt_entity *rt_se, கட்டமைப்பு rt_rq *rt_rq)
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
நிலையான உள்வரி bool move_entity(unsigned முழு flags)
{
	எனில் ((flags & (DEQUEUE_SAVE | DEQUEUE_MOVE)) == DEQUEUE_SAVE)
		திரும்பு false;

	திரும்பு true;
}

நிலையான வெறுமை __delist_rt_entity(கட்டமைப்பு sched_rt_entity *rt_se, கட்டமைப்பு rt_prio_array *array)
{
	list_del_init(&rt_se->run_list);

	எனில் (list_empty(array->queue + rt_se_prio(rt_se)))
		__clear_bit(rt_se_prio(rt_se), array->bitmap);

	rt_se->on_list = 0;
}

நிலையான உள்வரி கட்டமைப்பு sched_statistics *
__schedstats_from_rt_se(கட்டமைப்பு sched_rt_entity *rt_se)
{
	/* schedstats is not supported for rt group. */
	எனில் (!rt_entity_is_task(rt_se))
		திரும்பு வெற்று;

	திரும்பு &rt_task_of(rt_se)->stats;
}

நிலையான உள்வரி வெறுமை
update_stats_wait_start_rt(கட்டமைப்பு rt_rq *rt_rq, கட்டமைப்பு sched_rt_entity *rt_se)
{
	கட்டமைப்பு sched_statistics *stats;
	கட்டமைப்பு task_struct *p = வெற்று;

	எனில் (!schedstat_enabled())
		திரும்பு;

	எனில் (rt_entity_is_task(rt_se))
		p = rt_task_of(rt_se);

	stats = __schedstats_from_rt_se(rt_se);
	எனில் (!stats)
		திரும்பு;

	__update_stats_wait_start(rq_of_rt_rq(rt_rq), p, stats);
}

நிலையான உள்வரி வெறுமை
update_stats_enqueue_sleeper_rt(கட்டமைப்பு rt_rq *rt_rq, கட்டமைப்பு sched_rt_entity *rt_se)
{
	கட்டமைப்பு sched_statistics *stats;
	கட்டமைப்பு task_struct *p = வெற்று;

	எனில் (!schedstat_enabled())
		திரும்பு;

	எனில் (rt_entity_is_task(rt_se))
		p = rt_task_of(rt_se);

	stats = __schedstats_from_rt_se(rt_se);
	எனில் (!stats)
		திரும்பு;

	__update_stats_enqueue_sleeper(rq_of_rt_rq(rt_rq), p, stats);
}

நிலையான உள்வரி வெறுமை
update_stats_enqueue_rt(கட்டமைப்பு rt_rq *rt_rq, கட்டமைப்பு sched_rt_entity *rt_se,
			முழு flags)
{
	எனில் (!schedstat_enabled())
		திரும்பு;

	எனில் (flags & ENQUEUE_WAKEUP)
		update_stats_enqueue_sleeper_rt(rt_rq, rt_se);
}

நிலையான உள்வரி வெறுமை
update_stats_wait_end_rt(கட்டமைப்பு rt_rq *rt_rq, கட்டமைப்பு sched_rt_entity *rt_se)
{
	கட்டமைப்பு sched_statistics *stats;
	கட்டமைப்பு task_struct *p = வெற்று;

	எனில் (!schedstat_enabled())
		திரும்பு;

	எனில் (rt_entity_is_task(rt_se))
		p = rt_task_of(rt_se);

	stats = __schedstats_from_rt_se(rt_se);
	எனில் (!stats)
		திரும்பு;

	__update_stats_wait_end(rq_of_rt_rq(rt_rq), p, stats);
}

நிலையான உள்வரி வெறுமை
update_stats_dequeue_rt(கட்டமைப்பு rt_rq *rt_rq, கட்டமைப்பு sched_rt_entity *rt_se,
			முழு flags)
{
	கட்டமைப்பு task_struct *p = வெற்று;
	கட்டமைப்பு rq *rq = rq_of_rt_rq(rt_rq);

	எனில் (!schedstat_enabled())
		திரும்பு;

	எனில் (rt_entity_is_task(rt_se)) {
		p = rt_task_of(rt_se);

		எனில் (p != rq->curr)
			update_stats_wait_end_rt(rt_rq, rt_se);
	}

	எனில் ((flags & DEQUEUE_SLEEP) && p) {
		unsigned முழு state;

		state = READ_ONCE(p->__state);
		எனில் (state & TASK_INTERRUPTIBLE)
			__schedstat_set(p->stats.sleep_start,
					rq_clock(rq_of_rt_rq(rt_rq)));

		எனில் (state & TASK_UNINTERRUPTIBLE)
			__schedstat_set(p->stats.block_start,
					rq_clock(rq_of_rt_rq(rt_rq)));
	}
}

நிலையான வெறுமை __enqueue_rt_entity(கட்டமைப்பு sched_rt_entity *rt_se, unsigned முழு flags)
{
	கட்டமைப்பு rt_rq *rt_rq = rt_rq_of_se(rt_se);
	கட்டமைப்பு rt_prio_array *array = &rt_rq->active;
	கட்டமைப்பு rt_rq *group_rq = group_rt_rq(rt_se);
	கட்டமைப்பு list_head *queue = array->queue + rt_se_prio(rt_se);

	/*
	 * Don't enqueue the group if its throttled, or when empty.
	 * The latter is a consequence of the former when a child group
	 * get throttled and the current group doesn't have any other
	 * active members.
	 */
	எனில் (group_rq && (rt_rq_throttled(group_rq) || !group_rq->rt_nr_running)) {
		எனில் (rt_se->on_list)
			__delist_rt_entity(rt_se, array);
		திரும்பு;
	}

	எனில் (move_entity(flags)) {
		WARN_ON_ONCE(rt_se->on_list);
		எனில் (flags & ENQUEUE_HEAD)
			list_add(&rt_se->run_list, queue);
		இல்லையெனில்
			list_add_tail(&rt_se->run_list, queue);

		__set_bit(rt_se_prio(rt_se), array->bitmap);
		rt_se->on_list = 1;
	}
	rt_se->on_rq = 1;

	inc_rt_tasks(rt_se, rt_rq);
}

நிலையான வெறுமை __dequeue_rt_entity(கட்டமைப்பு sched_rt_entity *rt_se, unsigned முழு flags)
{
	கட்டமைப்பு rt_rq *rt_rq = rt_rq_of_se(rt_se);
	கட்டமைப்பு rt_prio_array *array = &rt_rq->active;

	எனில் (move_entity(flags)) {
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
நிலையான வெறுமை dequeue_rt_அடுக்கு(கட்டமைப்பு sched_rt_entity *rt_se, unsigned முழு flags)
{
	கட்டமைப்பு sched_rt_entity *back = வெற்று;
	unsigned முழு rt_nr_running;

	for_each_sched_rt_entity(rt_se) {
		rt_se->back = back;
		back = rt_se;
	}

	rt_nr_running = rt_rq_of_se(back)->rt_nr_running;

	க்கு (rt_se = back; rt_se; rt_se = rt_se->back) {
		எனில் (on_rt_rq(rt_se))
			__dequeue_rt_entity(rt_se, flags);
	}

	dequeue_உச்சி_rt_rq(rt_rq_of_se(back), rt_nr_running);
}

நிலையான வெறுமை enqueue_rt_entity(கட்டமைப்பு sched_rt_entity *rt_se, unsigned முழு flags)
{
	கட்டமைப்பு rq *rq = rq_of_rt_se(rt_se);

	update_stats_enqueue_rt(rt_rq_of_se(rt_se), rt_se, flags);

	dequeue_rt_அடுக்கு(rt_se, flags);
	for_each_sched_rt_entity(rt_se)
		__enqueue_rt_entity(rt_se, flags);
	enqueue_உச்சி_rt_rq(&rq->rt);
}

நிலையான வெறுமை dequeue_rt_entity(கட்டமைப்பு sched_rt_entity *rt_se, unsigned முழு flags)
{
	கட்டமைப்பு rq *rq = rq_of_rt_se(rt_se);

	update_stats_dequeue_rt(rt_rq_of_se(rt_se), rt_se, flags);

	dequeue_rt_அடுக்கு(rt_se, flags);

	for_each_sched_rt_entity(rt_se) {
		கட்டமைப்பு rt_rq *rt_rq = group_rt_rq(rt_se);

		எனில் (rt_rq && rt_rq->rt_nr_running)
			__enqueue_rt_entity(rt_se, flags);
	}
	enqueue_உச்சி_rt_rq(&rq->rt);
}

/*
 * Adding/removing a task to/from a priority array:
 */
நிலையான வெறுமை
enqueue_task_rt(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p, முழு flags)
{
	கட்டமைப்பு sched_rt_entity *rt_se = &p->rt;

	எனில் (flags & ENQUEUE_WAKEUP)
		rt_se->timeout = 0;

	check_schedstat_required();
	update_stats_wait_start_rt(rt_rq_of_se(rt_se), rt_se);

	enqueue_rt_entity(rt_se, flags);

	எனில் (task_is_blocked(p))
		திரும்பு;

	எனில் (!task_current(rq, p) && p->nr_cpus_allowed > 1)
		enqueue_pushable_task(rq, p);
}

நிலையான bool dequeue_task_rt(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p, முழு flags)
{
	கட்டமைப்பு sched_rt_entity *rt_se = &p->rt;

	update_curr_rt(rq);
	dequeue_rt_entity(rt_se, flags);

	dequeue_pushable_task(rq, p);

	திரும்பு true;
}

/*
 * Put task to the head or the end of the run list without the overhead of
 * dequeue followed by enqueue.
 */
நிலையான வெறுமை
requeue_rt_entity(கட்டமைப்பு rt_rq *rt_rq, கட்டமைப்பு sched_rt_entity *rt_se, முழு head)
{
	எனில் (on_rt_rq(rt_se)) {
		கட்டமைப்பு rt_prio_array *array = &rt_rq->active;
		கட்டமைப்பு list_head *queue = array->queue + rt_se_prio(rt_se);

		எனில் (head)
			list_move(&rt_se->run_list, queue);
		இல்லையெனில்
			list_move_tail(&rt_se->run_list, queue);
	}
}

நிலையான வெறுமை requeue_task_rt(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p, முழு head)
{
	கட்டமைப்பு sched_rt_entity *rt_se = &p->rt;
	கட்டமைப்பு rt_rq *rt_rq;

	for_each_sched_rt_entity(rt_se) {
		rt_rq = rt_rq_of_se(rt_se);
		requeue_rt_entity(rt_rq, rt_se, head);
	}
}

நிலையான வெறுமை yield_task_rt(கட்டமைப்பு rq *rq)
{
	requeue_task_rt(rq, rq->donor, 0);
}

நிலையான முழு find_lowest_rq(கட்டமைப்பு task_struct *task);

நிலையான முழு
select_task_rq_rt(கட்டமைப்பு task_struct *p, முழு cpu, முழு flags)
{
	கட்டமைப்பு task_struct *curr, *donor;
	கட்டமைப்பு rq *rq;
	bool test;

	/* For anything but wake ups, just return the task_cpu */
	எனில் (!(flags & (WF_TTWU | WF_FORK)))
		செல் out;

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

	எனில் (test || !rt_task_fits_capacity(p, cpu)) {
		முழு target = find_lowest_rq(p);

		/*
		 * Bail out if we were forcing a migration to find a better
		 * fitting CPU but our search failed.
		 */
		எனில் (!test && target != -1 && !rt_task_fits_capacity(p, target))
			செல் out_unlock;

		/*
		 * Don't bother moving it if the destination CPU is
		 * not running a lower priority task.
		 */
		எனில் (target != -1 &&
		    p->prio < cpu_rq(target)->rt.highest_prio.curr)
			cpu = target;
	}

out_unlock:
	rcu_read_unlock();

out:
	திரும்பு cpu;
}

நிலையான வெறுமை check_preempt_equal_prio(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p)
{
	எனில் (rq->curr->nr_cpus_allowed == 1 ||
	    !cpupri_find(&rq->rd->cpupri, rq->donor, வெற்று))
		திரும்பு;

	/*
	 * p is migratable, so let's not schedule it and
	 * see if it is pushed or pulled somewhere else.
	 */
	எனில் (p->nr_cpus_allowed != 1 &&
	    cpupri_find(&rq->rd->cpupri, p, வெற்று))
		திரும்பு;

	/*
	 * There appear to be other CPUs that can accept
	 * the current task but none can run 'p', so lets reschedule
	 * to try and push the current task away:
	 */
	requeue_task_rt(rq, p, 1);
	resched_curr(rq);
}

நிலையான முழு balance_rt(கட்டமைப்பு rq *rq, கட்டமைப்பு rq_flags *rf)
{
	/*
	 * Note, rq->donor may change during rq lock drops,
	 * so don't re-use p across lock drops
	 */
	கட்டமைப்பு task_struct *p = rq->donor;

	எனில் (!on_rt_rq(&p->rt) && need_pull_rt_task(rq, p)) {
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

	திரும்பு sched_stop_runnable(rq) || sched_dl_runnable(rq) || sched_rt_runnable(rq);
}

/*
 * Preempt the current task with a newly woken task if needed:
 */
நிலையான வெறுமை wakeup_preempt_rt(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p, முழு flags)
{
	கட்டமைப்பு task_struct *donor = rq->donor;

	/*
	 * XXX If we're preempted by DL, queue a push?
	 */
	எனில் (p->sched_class != &rt_sched_class ||
	    donor->sched_class != &rt_sched_class)
		திரும்பு;

	எனில் (p->prio < donor->prio) {
		resched_curr(rq);
		திரும்பு;
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
	எனில் (p->prio == donor->prio && !test_tsk_need_resched(rq->curr))
		check_preempt_equal_prio(rq, p);
}

நிலையான உள்வரி வெறுமை set_next_task_rt(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p, எண்ணிக்கை snt_e type)
{
	கட்டமைப்பு sched_rt_entity *rt_se = &p->rt;
	கட்டமைப்பு rt_rq *rt_rq = &rq->rt;

	எனில் (type == SNT_REPICK)
		திரும்பு;

	p->se.exec_start = rq_clock_task(rq);
	எனில் (on_rt_rq(&p->rt))
		update_stats_wait_end_rt(rt_rq, rt_se);

	/* The running task is never eligible for pushing */
	dequeue_pushable_task(rq, p);

	எனில் (type != SNT_PICK)
		திரும்பு;

	/*
	 * If prev task was rt, put_prev_task() has already updated the
	 * utilization. We only care of the case where we start to schedule a
	 * rt task
	 */
	எனில் (rq->donor->sched_class != &rt_sched_class)
		update_rt_rq_load_avg(rq_clock_pelt(rq), rq, 0);

	rt_queue_தள்ளு_tasks(rq);
}

நிலையான கட்டமைப்பு sched_rt_entity *pick_next_rt_entity(கட்டமைப்பு rt_rq *rt_rq)
{
	கட்டமைப்பு rt_prio_array *array = &rt_rq->active;
	கட்டமைப்பு sched_rt_entity *next = வெற்று;
	கட்டமைப்பு list_head *queue;
	முழு idx;

	idx = sched_find_first_bit(array->bitmap);
	BUG_ON(idx >= MAX_RT_PRIO);

	queue = array->queue + idx;
	எனில் (WARN_ON_ONCE(list_empty(queue)))
		திரும்பு வெற்று;
	next = list_entry(queue->next, கட்டமைப்பு sched_rt_entity, run_list);

	திரும்பு next;
}

நிலையான கட்டமைப்பு task_struct *_pick_next_task_rt(கட்டமைப்பு rq *rq)
{
	கட்டமைப்பு sched_rt_entity *rt_se;
	கட்டமைப்பு rt_rq *rt_rq  = &rq->rt;

	செய் {
		rt_se = pick_next_rt_entity(rt_rq);
		எனில் (unlikely(!rt_se))
			திரும்பு வெற்று;
		rt_rq = group_rt_rq(rt_se);
	} போது (rt_rq);

	திரும்பு rt_task_of(rt_se);
}

நிலையான கட்டமைப்பு task_struct *pick_task_rt(கட்டமைப்பு rq *rq, கட்டமைப்பு rq_flags *rf)
{
	கட்டமைப்பு task_struct *p;

	எனில் (!sched_rt_runnable(rq))
		திரும்பு வெற்று;

	p = _pick_next_task_rt(rq);

	திரும்பு p;
}

நிலையான வெறுமை put_prev_task_rt(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p, கட்டமைப்பு task_struct *next)
{
	கட்டமைப்பு sched_rt_entity *rt_se = &p->rt;
	கட்டமைப்பு rt_rq *rt_rq = &rq->rt;

	எனில் (on_rt_rq(&p->rt))
		update_stats_wait_start_rt(rt_rq, rt_se);

	update_curr_rt(rq);

	update_rt_rq_load_avg(rq_clock_pelt(rq), rq, 1);

	எனில் (task_is_blocked(p))
		திரும்பு;
	/*
	 * The previous task needs to be made eligible for pushing
	 * if it is still active
	 */
	எனில் (on_rt_rq(&p->rt) && p->nr_cpus_allowed > 1)
		enqueue_pushable_task(rq, p);
}

/* Only try algorithms three times */
#define RT_MAX_TRIES 3

/*
 * Return the highest pushable rq's task, which is suitable to be executed
 * on the CPU, NULL otherwise
 */
நிலையான கட்டமைப்பு task_struct *pick_highest_pushable_task(கட்டமைப்பு rq *rq, முழு cpu)
{
	கட்டமைப்பு plist_head *head = &rq->rt.pushable_tasks;
	கட்டமைப்பு task_struct *p;

	எனில் (!has_pushable_tasks(rq))
		திரும்பு வெற்று;

	plist_for_each_entry(p, head, pushable_tasks) {
		எனில் (task_is_pushable(rq, p, cpu))
			திரும்பு p;
	}

	திரும்பு வெற்று;
}

நிலையான DEFINE_PER_CPU(cpumask_var_t, local_cpu_mask);

நிலையான முழு find_lowest_rq(கட்டமைப்பு task_struct *task)
{
	கட்டமைப்பு sched_domain *sd;
	கட்டமைப்பு cpumask *lowest_mask = this_cpu_cpumask_var_ptr(local_cpu_mask);
	முழு this_cpu = smp_processor_id();
	முழு cpu      = task_cpu(task);
	முழு ret;

	/* Make sure the mask is initialized first */
	எனில் (unlikely(!lowest_mask))
		திரும்பு -1;

	எனில் (task->nr_cpus_allowed == 1)
		திரும்பு -1; /* No other targets possible */

	/*
	 * If we're on asym system ensure we consider the different capacities
	 * of the CPUs when searching for the lowest_mask.
	 */
	எனில் (sched_asym_cpucap_active()) {

		ret = cpupri_find_fitness(&task_rq(task)->rd->cpupri,
					  task, lowest_mask,
					  rt_task_fits_capacity);
	} இல்லையெனில் {

		ret = cpupri_find(&task_rq(task)->rd->cpupri,
				  task, lowest_mask);
	}

	எனில் (!ret)
		திரும்பு -1; /* No targets found */

	/*
	 * At this point we have built a mask of CPUs representing the
	 * lowest priority tasks in the system.  Now we want to elect
	 * the best one based on our affinity and topology.
	 *
	 * We prioritize the last CPU that the task executed on since
	 * it is most likely cache-hot in that location.
	 */
	எனில் (cpumask_test_cpu(cpu, lowest_mask))
		திரும்பு cpu;

	/*
	 * Otherwise, we consult the sched_domains span maps to figure
	 * out which CPU is logically closest to our hot cache data.
	 */
	எனில் (!cpumask_test_cpu(this_cpu, lowest_mask))
		this_cpu = -1; /* Skip this_cpu opt if not among lowest */

	rcu_read_lock();
	for_each_domain(cpu, sd) {
		எனில் (sd->flags & SD_WAKE_AFFINE) {
			முழு best_cpu;

			/*
			 * "this_cpu" is cheaper to preempt than a
			 * remote processor.
			 */
			எனில் (this_cpu != -1 &&
			    cpumask_test_cpu(this_cpu, sched_domain_span(sd))) {
				rcu_read_unlock();
				திரும்பு this_cpu;
			}

			best_cpu = cpumask_any_and_distribute(lowest_mask,
							      sched_domain_span(sd));
			எனில் (best_cpu < nr_cpu_ids) {
				rcu_read_unlock();
				திரும்பு best_cpu;
			}
		}
	}
	rcu_read_unlock();

	/*
	 * And finally, if there were no matches within the domains
	 * just give the caller *something* to work with from the compatible
	 * locations.
	 */
	எனில் (this_cpu != -1)
		திரும்பு this_cpu;

	cpu = cpumask_any_distribute(lowest_mask);
	எனில் (cpu < nr_cpu_ids)
		திரும்பு cpu;

	திரும்பு -1;
}

நிலையான கட்டமைப்பு task_struct *pick_next_pushable_task(கட்டமைப்பு rq *rq)
{
	கட்டமைப்பு plist_head *head = &rq->rt.pushable_tasks;
	கட்டமைப்பு task_struct *i, *p = வெற்று;

	எனில் (!has_pushable_tasks(rq))
		திரும்பு வெற்று;

	plist_for_each_entry(i, head, pushable_tasks) {
		/* skip tasks that cannot be migrated */
		எனில் (!task_on_cpu(rq, i) && !is_migration_disabled(i)) {
			p = i;
			நிறுத்து;
		}
	}

	எனில் (!p)
		திரும்பு வெற்று;

	BUG_ON(rq->cpu != task_cpu(p));
	BUG_ON(task_current(rq, p));
	BUG_ON(task_current_donor(rq, p));
	BUG_ON(p->nr_cpus_allowed <= 1);

	BUG_ON(!task_on_rq_queued(p));
	BUG_ON(!rt_task(p));

	திரும்பு p;
}

/* Will lock the rq it finds */
நிலையான கட்டமைப்பு rq *find_lock_lowest_rq(கட்டமைப்பு task_struct *task, கட்டமைப்பு rq *rq)
{
	கட்டமைப்பு rq *lowest_rq = வெற்று;
	முழு tries;
	முழு cpu;

	க்கு (tries = 0; tries < RT_MAX_TRIES; tries++) {
		cpu = find_lowest_rq(task);

		எனில் ((cpu == -1) || (cpu == rq->cpu))
			நிறுத்து;

		lowest_rq = cpu_rq(cpu);

		எனில் (lowest_rq->rt.highest_prio.curr <= task->prio) {
			/*
			 * Target rq has tasks of equal or higher priority,
			 * retrying does not release any lock and is unlikely
			 * to yield a different result.
			 */
			lowest_rq = வெற்று;
			நிறுத்து;
		}

		/* if the prio of this runqueue changed, try again */
		எனில் (double_lock_balance(rq, lowest_rq)) {
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
			எனில் (unlikely(is_migration_disabled(task) ||
				     !cpumask_test_cpu(lowest_rq->cpu, &task->cpus_mask) ||
				     task != pick_next_pushable_task(rq))) {

				double_unlock_balance(rq, lowest_rq);
				lowest_rq = வெற்று;
				நிறுத்து;
			}
		}

		/* If this rq is still suitable use it. */
		எனில் (lowest_rq->rt.highest_prio.curr > task->prio)
			நிறுத்து;

		/* try again */
		double_unlock_balance(rq, lowest_rq);
		lowest_rq = வெற்று;
	}

	திரும்பு lowest_rq;
}

/*
 * If the current CPU has more than one RT task, see if the non
 * running task can migrate over to a CPU that is running a task
 * of lesser priority.
 */
நிலையான முழு தள்ளு_rt_task(கட்டமைப்பு rq *rq, bool pull)
{
	கட்டமைப்பு task_struct *next_task;
	கட்டமைப்பு rq *lowest_rq;
	முழு ret = 0;

	எனில் (!rq->rt.overloaded)
		திரும்பு 0;

	next_task = pick_next_pushable_task(rq);
	எனில் (!next_task)
		திரும்பு 0;

retry:
	/*
	 * It's possible that the next_task slipped in of
	 * higher priority than current. If that's the case
	 * just reschedule current.
	 */
	எனில் (unlikely(next_task->prio < rq->donor->prio)) {
		resched_curr(rq);
		திரும்பு 0;
	}

	எனில் (is_migration_disabled(next_task)) {
		கட்டமைப்பு task_struct *தள்ளு_task = வெற்று;
		முழு cpu;

		எனில் (!pull || rq->தள்ளு_busy)
			திரும்பு 0;

		/*
		 * Invoking find_lowest_rq() on anything but an RT task doesn't
		 * make sense. Per the above priority check, curr has to
		 * be of higher priority than next_task, so no need to
		 * reschedule when bailing out.
		 *
		 * Note that the stoppers are masqueraded as SCHED_FIFO
		 * (cf. sched_set_stop_task()), so we can't rely on rt_task().
		 */
		எனில் (rq->donor->sched_class != &rt_sched_class)
			திரும்பு 0;

		cpu = find_lowest_rq(rq->curr);
		எனில் (cpu == -1 || cpu == rq->cpu)
			திரும்பு 0;

		/*
		 * Given we found a CPU with lower priority than @next_task,
		 * therefore it should be running. However we cannot migrate it
		 * to this other CPU, instead attempt to push the current
		 * running task on this CPU away.
		 */
		தள்ளு_task = get_தள்ளு_task(rq);
		எனில் (தள்ளு_task) {
			preempt_disable();
			raw_spin_rq_unlock(rq);
			stop_one_cpu_nowait(rq->cpu, தள்ளு_cpu_stop,
					    தள்ளு_task, &rq->தள்ளு_work);
			preempt_enable();
			raw_spin_rq_lock(rq);
		}

		திரும்பு 0;
	}

	எனில் (WARN_ON(next_task == rq->curr))
		திரும்பு 0;

	/* We might release rq lock */
	get_task_struct(next_task);

	/* find_lock_lowest_rq locks the rq if found */
	lowest_rq = find_lock_lowest_rq(next_task, rq);
	எனில் (!lowest_rq) {
		கட்டமைப்பு task_struct *task;
		/*
		 * find_lock_lowest_rq releases rq->lock
		 * so it is possible that next_task has migrated.
		 *
		 * We need to make sure that the task is still on the same
		 * run-queue and is also still the next task eligible for
		 * pushing.
		 */
		task = pick_next_pushable_task(rq);
		எனில் (task == next_task) {
			/*
			 * The task hasn't migrated, and is still the next
			 * eligible task, but we failed to find a run-queue
			 * to push it to.  Do not retry in this case, since
			 * other CPUs will pull from us when ready.
			 */
			செல் out;
		}

		எனில் (!task)
			/* No more tasks, just exit */
			செல் out;

		/*
		 * Something has shifted, try again.
		 */
		put_task_struct(next_task);
		next_task = task;
		செல் retry;
	}

	move_queued_task_locked(rq, lowest_rq, next_task);
	resched_curr(lowest_rq);
	ret = 1;

	double_unlock_balance(rq, lowest_rq);
out:
	put_task_struct(next_task);

	திரும்பு ret;
}

நிலையான வெறுமை தள்ளு_rt_tasks(கட்டமைப்பு rq *rq)
{
	/* push_rt_task will return true if it moved an RT */
	போது (தள்ளு_rt_task(rq, false))
		;
}

#ifdef HAVE_RT_தள்ளு_IPI

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
நிலையான முழு rto_next_cpu(கட்டமைப்பு வேர்_domain *rd)
{
	முழு this_cpu = smp_processor_id();
	முழு next;
	முழு cpu;

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
	க்கு (;;) {

		/* When rto_cpu is -1 this acts like cpumask_first() */
		cpu = cpumask_next(rd->rto_cpu, rd->rto_mask);

		rd->rto_cpu = cpu;

		/* Do not send IPI to self */
		எனில் (cpu == this_cpu)
			தொடர்;

		எனில் (cpu < nr_cpu_ids)
			திரும்பு cpu;

		rd->rto_cpu = -1;

		/*
		 * ACQUIRE ensures we see the @rto_mask changes
		 * made prior to the @next value observed.
		 *
		 * Matches WMB in rt_set_overload().
		 */
		next = atomic_read_acquire(&rd->rto_loop_next);

		எனில் (rd->rto_loop == next)
			நிறுத்து;

		rd->rto_loop = next;
	}

	திரும்பு -1;
}

நிலையான உள்வரி bool rto_start_trylock(atomic_t *v)
{
	திரும்பு !atomic_cmpxchg_acquire(v, 0, 1);
}

நிலையான உள்வரி வெறுமை rto_start_unlock(atomic_t *v)
{
	atomic_set_release(v, 0);
}

நிலையான வெறுமை tell_cpu_to_தள்ளு(கட்டமைப்பு rq *rq)
{
	முழு cpu = -1;

	/* Keep the loop going if the IPI is currently active */
	atomic_inc(&rq->rd->rto_loop_next);

	/* Only one CPU can initiate a loop at a time */
	எனில் (!rto_start_trylock(&rq->rd->rto_loop_start))
		திரும்பு;

	raw_spin_lock(&rq->rd->rto_lock);

	/*
	 * The rto_cpu is updated under the lock, if it has a valid CPU
	 * then the IPI is still running and will continue due to the
	 * update to loop_next, and nothing needs to be done here.
	 * Otherwise it is finishing up and an IPI needs to be sent.
	 */
	எனில் (rq->rd->rto_cpu < 0)
		cpu = rto_next_cpu(rq->rd);

	raw_spin_unlock(&rq->rd->rto_lock);

	rto_start_unlock(&rq->rd->rto_loop_start);

	எனில் (cpu >= 0) {
		/* Make sure the rd does not get freed while pushing */
		sched_get_rd(rq->rd);
		irq_work_queue_on(&rq->rd->rto_தள்ளு_work, cpu);
	}
}

/* Called from hardirq context */
வெறுமை rto_தள்ளு_irq_work_func(கட்டமைப்பு irq_work *work)
{
	கட்டமைப்பு வேர்_domain *rd =
		container_of(work, கட்டமைப்பு வேர்_domain, rto_தள்ளு_work);
	கட்டமைப்பு rq *rq;
	முழு cpu;

	rq = this_rq();

	/*
	 * We do not need to grab the lock to check for has_pushable_tasks.
	 * When it gets updated, a check is made if a push is possible.
	 */
	எனில் (has_pushable_tasks(rq)) {
		raw_spin_rq_lock(rq);
		போது (தள்ளு_rt_task(rq, true))
			;
		raw_spin_rq_unlock(rq);
	}

	raw_spin_lock(&rd->rto_lock);

	/* Pass the IPI to the next rt overloaded queue */
	cpu = rto_next_cpu(rd);

	raw_spin_unlock(&rd->rto_lock);

	எனில் (cpu < 0) {
		sched_put_rd(rd);
		திரும்பு;
	}

	/* Try the next RT overloaded CPU */
	irq_work_queue_on(&rd->rto_தள்ளு_work, cpu);
}
#endif /* HAVE_RT_PUSH_IPI */

நிலையான வெறுமை pull_rt_task(கட்டமைப்பு rq *this_rq)
{
	முழு this_cpu = this_rq->cpu, cpu;
	bool resched = false;
	கட்டமைப்பு task_struct *p, *தள்ளு_task;
	கட்டமைப்பு rq *src_rq;
	முழு rt_overload_மொத்தம் = rt_overloaded(this_rq);

	எனில் (likely(!rt_overload_மொத்தம்))
		திரும்பு;

	/*
	 * Match the barrier from rt_set_overloaded; this guarantees that if we
	 * see overloaded we must also see the rto_mask bit.
	 */
	smp_rmb();

	/* If we are the only overloaded CPU do nothing */
	எனில் (rt_overload_மொத்தம் == 1 &&
	    cpumask_test_cpu(this_rq->cpu, this_rq->rd->rto_mask))
		திரும்பு;

#ifdef HAVE_RT_தள்ளு_IPI
	எனில் (sched_feat(RT_தள்ளு_IPI)) {
		tell_cpu_to_தள்ளு(this_rq);
		திரும்பு;
	}
#endif

	for_each_cpu(cpu, this_rq->rd->rto_mask) {
		எனில் (this_cpu == cpu)
			தொடர்;

		src_rq = cpu_rq(cpu);

		/*
		 * Don't bother taking the src_rq->lock if the next highest
		 * task is known to be lower-priority than our current task.
		 * This may look racy, but if this value is about to go
		 * logically higher, the src_rq will push this task away.
		 * And if its going logically lower, we do not care
		 */
		எனில் (src_rq->rt.highest_prio.next >=
		    this_rq->rt.highest_prio.curr)
			தொடர்;

		/*
		 * We can potentially drop this_rq's lock in
		 * double_lock_balance, and another CPU could
		 * alter this_rq
		 */
		தள்ளு_task = வெற்று;
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
		எனில் (p && (p->prio < this_rq->rt.highest_prio.curr)) {
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
			எனில் (p->prio < src_rq->donor->prio)
				செல் skip;

			எனில் (is_migration_disabled(p)) {
				தள்ளு_task = get_தள்ளு_task(src_rq);
			} இல்லையெனில் {
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

		எனில் (தள்ளு_task) {
			preempt_disable();
			raw_spin_rq_unlock(this_rq);
			stop_one_cpu_nowait(src_rq->cpu, தள்ளு_cpu_stop,
					    தள்ளு_task, &src_rq->தள்ளு_work);
			preempt_enable();
			raw_spin_rq_lock(this_rq);
		}
	}

	எனில் (resched)
		resched_curr(this_rq);
}

/*
 * If we are not running and we are not going to reschedule soon, we should
 * try to push tasks away now
 */
நிலையான வெறுமை task_woken_rt(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p)
{
	bool need_to_தள்ளு = !task_on_cpu(rq, p) &&
			    !test_tsk_need_resched(rq->curr) &&
			    p->nr_cpus_allowed > 1 &&
			    (dl_task(rq->donor) || rt_task(rq->donor)) &&
			    (rq->curr->nr_cpus_allowed < 2 ||
			     rq->donor->prio <= p->prio);

	எனில் (need_to_தள்ளு)
		தள்ளு_rt_tasks(rq);
}

/* Assumes rq->lock is held */
நிலையான வெறுமை rq_online_rt(கட்டமைப்பு rq *rq)
{
	எனில் (rq->rt.overloaded)
		rt_set_overload(rq);

	__enable_runtime(rq);

	cpupri_set(&rq->rd->cpupri, rq->cpu, rq->rt.highest_prio.curr);
}

/* Assumes rq->lock is held */
நிலையான வெறுமை rq_offline_rt(கட்டமைப்பு rq *rq)
{
	எனில் (rq->rt.overloaded)
		rt_clear_overload(rq);

	__disable_runtime(rq);

	cpupri_set(&rq->rd->cpupri, rq->cpu, CPUPRI_INVALID);
}

/*
 * When switch from the rt queue, we bring ourselves to a position
 * that we might want to pull RT tasks from other runqueues.
 */
நிலையான வெறுமை switched_from_rt(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p)
{
	/*
	 * If there are other RT tasks then we will reschedule
	 * and the scheduling of the other RT tasks will handle
	 * the balancing. But if we are the last RT task
	 * we may need to handle the pulling of RT tasks
	 * now.
	 */
	எனில் (!task_on_rq_queued(p) || rq->rt.rt_nr_running)
		திரும்பு;

	rt_queue_pull_task(rq);
}

வெறுமை __init init_sched_rt_class(வெறுமை)
{
	unsigned முழு i;

	for_each_possible_cpu(i) {
		zalloc_cpumask_var_முனை(&per_cpu(local_cpu_mask, i),
					GFP_KERNEL, cpu_to_முனை(i));
	}
}

/*
 * When switching a task to RT, we may overload the runqueue
 * with RT tasks. In this case we try to push them off to
 * other runqueues.
 */
நிலையான வெறுமை switched_to_rt(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p)
{
	/*
	 * If we are running, update the avg_rt tracking, as the running time
	 * will now on be accounted into the latter.
	 */
	எனில் (task_current(rq, p)) {
		update_rt_rq_load_avg(rq_clock_pelt(rq), rq, 0);
		திரும்பு;
	}

	/*
	 * If we are not running we may need to preempt the current
	 * running task. If that current running task is also an RT task
	 * then see if we can move to another run queue.
	 */
	எனில் (task_on_rq_queued(p)) {
		எனில் (p->nr_cpus_allowed > 1 && rq->rt.overloaded)
			rt_queue_தள்ளு_tasks(rq);
		எனில் (p->prio < rq->donor->prio && cpu_online(cpu_of(rq)))
			resched_curr(rq);
	}
}

/*
 * Priority of the task has changed. This may cause
 * us to initiate a push or pull.
 */
நிலையான வெறுமை
prio_changed_rt(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p, u64 oldprio)
{
	எனில் (!task_on_rq_queued(p))
		திரும்பு;

	எனில் (p->prio == oldprio)
		திரும்பு;

	எனில் (task_current_donor(rq, p)) {
		/*
		 * If our priority decreases while running, we
		 * may need to pull tasks to this runqueue.
		 */
		எனில் (oldprio < p->prio)
			rt_queue_pull_task(rq);

		/*
		 * If there's a higher priority task waiting to run
		 * then reschedule.
		 */
		எனில் (p->prio > rq->rt.highest_prio.curr)
			resched_curr(rq);
	} இல்லையெனில் {
		/*
		 * This task is not running, but if it is
		 * greater than the current running task
		 * then reschedule.
		 */
		எனில் (p->prio < rq->donor->prio)
			resched_curr(rq);
	}
}

#ifdef CONFIG_POSIX_TIMERS
நிலையான வெறுமை watchdog(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p)
{
	unsigned long soft, hard;

	/* max may change after cur was read, this will be fixed next tick */
	soft = task_rlimit(p, RLIMIT_RTTIME);
	hard = task_rlimit_max(p, RLIMIT_RTTIME);

	எனில் (soft != RLIM_INFINITY) {
		unsigned long next;

		எனில் (p->rt.watchdog_stamp != jiffies) {
			p->rt.timeout++;
			p->rt.watchdog_stamp = jiffies;
		}

		next = DIV_ROUND_UP(min(soft, hard), USEC_PER_SEC/HZ);
		எனில் (p->rt.timeout > next) {
			posix_cputimers_rt_watchdog(&p->posix_cputimers,
						    p->se.sum_exec_runtime);
		}
	}
}
#இல்லையெனில் /* !CONFIG_POSIX_TIMERS: */
நிலையான உள்வரி வெறுமை watchdog(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p) { }
#endif /* !CONFIG_POSIX_TIMERS */

/*
 * scheduler tick hitting a task of our scheduling class.
 *
 * NOTE: This function can be called remotely by the tick offload that
 * goes along full dynticks. Therefore no local assumption can be made
 * and everything must be accessed through the @rq and @curr passed in
 * parameters.
 */
நிலையான வெறுமை task_tick_rt(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p, முழு queued)
{
	கட்டமைப்பு sched_rt_entity *rt_se = &p->rt;

	update_curr_rt(rq);
	update_rt_rq_load_avg(rq_clock_pelt(rq), rq, 1);

	watchdog(rq, p);

	/*
	 * RR tasks need a special form of time-slice management.
	 * FIFO tasks have no timeslices.
	 */
	எனில் (p->policy != SCHED_RR)
		திரும்பு;

	எனில் (--p->rt.time_slice)
		திரும்பு;

	p->rt.time_slice = sched_rr_timeslice;

	/*
	 * Requeue to the end of queue if we (and all of our ancestors) are not
	 * the only element on the queue
	 */
	for_each_sched_rt_entity(rt_se) {
		எனில் (rt_se->run_list.prev != rt_se->run_list.next) {
			requeue_task_rt(rq, p, 0);
			resched_curr(rq);
			திரும்பு;
		}
	}
}

நிலையான unsigned முழு get_rr_interval_rt(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *task)
{
	/*
	 * Time slice is 0 for SCHED_FIFO tasks
	 */
	எனில் (task->policy == SCHED_RR)
		திரும்பு sched_rr_timeslice;
	இல்லையெனில்
		திரும்பு 0;
}

#ifdef CONFIG_SCHED_CORE
நிலையான முழு task_is_throttled_rt(கட்டமைப்பு task_struct *p, முழு cpu)
{
	கட்டமைப்பு rt_rq *rt_rq;

#ifdef CONFIG_RT_GROUP_SCHED // XXX maybe add task_rt_rq(), see also sched_rt_period_rt_rq
	rt_rq = task_group(p)->rt_rq[cpu];
	WARN_ON(!rt_group_sched_enabled() && rt_rq->tg != &வேர்_task_group);
#இல்லையெனில்
	rt_rq = &cpu_rq(cpu)->rt;
#endif

	திரும்பு rt_rq_throttled(rt_rq);
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
நிலையான DEFINE_MUTEX(rt_constraints_mutex);

நிலையான உள்வரி முழு tg_has_rt_tasks(கட்டமைப்பு task_group *tg)
{
	கட்டமைப்பு task_struct *task;
	கட்டமைப்பு css_task_iter it;
	முழு ret = 0;

	/*
	 * Autogroups do not have RT tasks; see autogroup_create().
	 */
	எனில் (task_group_is_autogroup(tg))
		திரும்பு 0;

	css_task_iter_start(&tg->css, 0, &it);
	போது (!ret && (task = css_task_iter_next(&it)))
		ret |= rt_task(task);
	css_task_iter_end(&it);

	திரும்பு ret;
}

கட்டமைப்பு rt_schedulable_data {
	கட்டமைப்பு task_group *tg;
	u64 rt_period;
	u64 rt_runtime;
};

நிலையான முழு tg_rt_schedulable(கட்டமைப்பு task_group *tg, வெறுமை *data)
{
	கட்டமைப்பு rt_schedulable_data *d = data;
	கட்டமைப்பு task_group *child;
	u64 total, sum = 0;
	u64 period, runtime;

	period = ktime_to_ns(tg->rt_bandwidth.rt_period);
	runtime = tg->rt_bandwidth.rt_runtime;

	எனில் (tg == d->tg) {
		period = d->rt_period;
		runtime = d->rt_runtime;
	}

	/*
	 * Cannot have more runtime than the period.
	 */
	எனில் (runtime > period && runtime != RUNTIME_INF)
		திரும்பு -EINVAL;

	/*
	 * Ensure we don't starve existing RT tasks if runtime turns zero.
	 */
	எனில் (rt_bandwidth_enabled() && !runtime &&
	    tg->rt_bandwidth.rt_runtime && tg_has_rt_tasks(tg))
		திரும்பு -EBUSY;

	total = to_ratio(period, runtime);

	/*
	 * Nobody can have more than the global setting allows.
	 */
	எனில் (total > to_ratio(global_rt_period(), global_rt_runtime()))
		திரும்பு -EINVAL;

	/*
	 * The sum of our children's runtime should not exceed our own.
	 */
	list_for_each_entry_rcu(child, &tg->children, siblings) {
		period = ktime_to_ns(child->rt_bandwidth.rt_period);
		runtime = child->rt_bandwidth.rt_runtime;

		எனில் (child == d->tg) {
			period = d->rt_period;
			runtime = d->rt_runtime;
		}

		sum += to_ratio(period, runtime);
	}

	எனில் (sum > total)
		திரும்பு -EINVAL;

	திரும்பு 0;
}

நிலையான முழு __rt_schedulable(கட்டமைப்பு task_group *tg, u64 period, u64 runtime)
{
	முழு ret;

	கட்டமைப்பு rt_schedulable_data data = {
		.tg = tg,
		.rt_period = period,
		.rt_runtime = runtime,
	};

	rcu_read_lock();
	ret = walk_tg_மரம்(tg_rt_schedulable, tg_nop, &data);
	rcu_read_unlock();

	திரும்பு ret;
}

நிலையான முழு tg_set_rt_bandwidth(கட்டமைப்பு task_group *tg,
		u64 rt_period, u64 rt_runtime)
{
	முழு i, err = 0;

	/*
	 * Disallowing the root group RT runtime is BAD, it would disallow the
	 * kernel creating (and or operating) RT threads.
	 */
	எனில் (tg == &வேர்_task_group && rt_runtime == 0)
		திரும்பு -EINVAL;

	/* No period doesn't make any sense. */
	எனில் (rt_period == 0)
		திரும்பு -EINVAL;

	/*
	 * Bound quota to defend quota against overflow during bandwidth shift.
	 */
	எனில் (rt_runtime != RUNTIME_INF && rt_runtime > max_rt_runtime)
		திரும்பு -EINVAL;

	mutex_lock(&rt_constraints_mutex);
	err = __rt_schedulable(tg, rt_period, rt_runtime);
	எனில் (err)
		செல் unlock;

	raw_spin_lock_irq(&tg->rt_bandwidth.rt_runtime_lock);
	tg->rt_bandwidth.rt_period = ns_to_ktime(rt_period);
	tg->rt_bandwidth.rt_runtime = rt_runtime;

	for_each_possible_cpu(i) {
		கட்டமைப்பு rt_rq *rt_rq = tg->rt_rq[i];

		raw_spin_lock(&rt_rq->rt_runtime_lock);
		rt_rq->rt_runtime = rt_runtime;
		raw_spin_unlock(&rt_rq->rt_runtime_lock);
	}
	raw_spin_unlock_irq(&tg->rt_bandwidth.rt_runtime_lock);
unlock:
	mutex_unlock(&rt_constraints_mutex);

	திரும்பு err;
}

முழு sched_group_set_rt_runtime(கட்டமைப்பு task_group *tg, long rt_runtime_us)
{
	u64 rt_runtime, rt_period;

	rt_period = ktime_to_ns(tg->rt_bandwidth.rt_period);
	rt_runtime = (u64)rt_runtime_us * NSEC_PER_USEC;
	எனில் (rt_runtime_us < 0)
		rt_runtime = RUNTIME_INF;
	இல்லையெனில் எனில் ((u64)rt_runtime_us > U64_MAX / NSEC_PER_USEC)
		திரும்பு -EINVAL;

	திரும்பு tg_set_rt_bandwidth(tg, rt_period, rt_runtime);
}

long sched_group_rt_runtime(கட்டமைப்பு task_group *tg)
{
	u64 rt_runtime_us;

	எனில் (tg->rt_bandwidth.rt_runtime == RUNTIME_INF)
		திரும்பு -1;

	rt_runtime_us = tg->rt_bandwidth.rt_runtime;
	do_div(rt_runtime_us, NSEC_PER_USEC);
	திரும்பு rt_runtime_us;
}

முழு sched_group_set_rt_period(கட்டமைப்பு task_group *tg, u64 rt_period_us)
{
	u64 rt_runtime, rt_period;

	எனில் (rt_period_us > U64_MAX / NSEC_PER_USEC)
		திரும்பு -EINVAL;

	rt_period = rt_period_us * NSEC_PER_USEC;
	rt_runtime = tg->rt_bandwidth.rt_runtime;

	திரும்பு tg_set_rt_bandwidth(tg, rt_period, rt_runtime);
}

long sched_group_rt_period(கட்டமைப்பு task_group *tg)
{
	u64 rt_period_us;

	rt_period_us = ktime_to_ns(tg->rt_bandwidth.rt_period);
	do_div(rt_period_us, NSEC_PER_USEC);
	திரும்பு rt_period_us;
}

முழு sched_rt_can_attach(கட்டமைப்பு task_group *tg, கட்டமைப்பு task_struct *tsk)
{
	/* Don't accept real-time tasks when there is no way for them to run */
	எனில் (rt_group_sched_enabled() && rt_task(tsk) && tg->rt_bandwidth.rt_runtime == 0)
		திரும்பு 0;

	திரும்பு 1;
}

#endif /* !CONFIG_RT_GROUP_SCHED */

#ifdef CONFIG_SYSCTL
நிலையான முழு sched_rt_global_validate(வெறுமை)
{
	எனில் ((sysctl_sched_rt_runtime != RUNTIME_INF) &&
		((sysctl_sched_rt_runtime > sysctl_sched_rt_period) ||
		 ((u64)sysctl_sched_rt_runtime *
			NSEC_PER_USEC > max_rt_runtime)))
		திரும்பு -EINVAL;

#ifdef CONFIG_RT_GROUP_SCHED
	எனில் (!rt_group_sched_enabled())
		திரும்பு 0;

	scoped_guard(mutex, &rt_constraints_mutex)
		திரும்பு __rt_schedulable(வெற்று, 0, 0);
#endif
	திரும்பு 0;
}

நிலையான முழு sched_rt_handler(மாறிலி கட்டமைப்பு ctl_table *table, முழு லிக, வெறுமை *buffer,
		size_t *lenp, loff_t *ppos)
{
	முழு old_period, old_runtime;
	நிலையான DEFINE_MUTEX(mutex);
	முழு ret;

	mutex_lock(&mutex);
	sched_domains_mutex_lock();
	old_period = sysctl_sched_rt_period;
	old_runtime = sysctl_sched_rt_runtime;

	ret = proc_dointvec_minmax(table, லிக, buffer, lenp, ppos);

	எனில் (!ret && லிக) {
		ret = sched_rt_global_validate();
		எனில் (ret)
			செல் undo;

		ret = sched_dl_global_validate();
		எனில் (ret)
			செல் undo;

		sched_dl_do_global();
	}
	எனில் (0) {
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

	திரும்பு ret;
}

நிலையான முழு sched_rr_handler(மாறிலி கட்டமைப்பு ctl_table *table, முழு லிக, வெறுமை *buffer,
		size_t *lenp, loff_t *ppos)
{
	முழு ret;
	நிலையான DEFINE_MUTEX(mutex);

	mutex_lock(&mutex);
	ret = proc_dointvec(table, லிக, buffer, lenp, ppos);
	/*
	 * Make sure that internally we keep jiffies.
	 * Also, writing zero resets the time-slice to default:
	 */
	எனில் (!ret && லிக) {
		sched_rr_timeslice =
			sysctl_sched_rr_timeslice <= 0 ? RR_TIMESLICE :
			msecs_to_jiffies(sysctl_sched_rr_timeslice);

		எனில் (sysctl_sched_rr_timeslice <= 0)
			sysctl_sched_rr_timeslice = jiffies_to_msecs(RR_TIMESLICE);
	}
	mutex_unlock(&mutex);

	திரும்பு ret;
}
#endif /* CONFIG_SYSCTL */

வெறுமை print_rt_stats(கட்டமைப்பு seq_file *m, முழு cpu)
{
	rt_rq_iter_t iter;
	கட்டமைப்பு rt_rq *rt_rq;

	rcu_read_lock();
	for_each_rt_rq(rt_rq, iter, cpu_rq(cpu))
		print_rt_rq(m, cpu, rt_rq);
	rcu_read_unlock();
}
