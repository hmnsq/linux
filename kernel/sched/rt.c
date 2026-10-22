// SPDX-License-Identifier: GPL-2.0
/*
 * Real-Time Scheduling Class (mapped to the SCHED_FIFO and SCHED_RR
 * policies)
 */

#شامل "sched.h"
#شامل "pelt.h"

صحیح sched_rr_timeslice = RR_TIMESLICE;
/* More than 4 hours if BW_SHIFT equals 20. */
ایستا ثابت u64 max_rt_runtime = MAX_BW;

/*
 * period over which we measure -rt task CPU usage in us.
 * default: 1s
 */
صحیح sysctl_sched_rt_period = 1000000;

/*
 * part of the period that we allow rt tasks to run in us.
 * default: 1s
 */
صحیح sysctl_sched_rt_runtime = 1000000;

#ifdef CONFIG_SYSCTL
ایستا صحیح sysctl_sched_rr_timeslice = (MSEC_PER_SEC * RR_TIMESLICE) / HZ;
ایستا صحیح sched_rt_handler(ثابت ساختار ctl_table *table, صحیح اكتب, تهی *buffer,
		size_t *lenp, loff_t *ppos);
ایستا صحیح sched_rr_handler(ثابت ساختار ctl_table *table, صحیح اكتب, تهی *buffer,
		size_t *lenp, loff_t *ppos);
ایستا ثابت ساختار ctl_table sched_rt_sysctls[] = {
	{
		.procname       = "sched_rt_period_us",
		.data           = &sysctl_sched_rt_period,
		.maxlen         = اندازه(صحیح),
		.mode           = 0644,
		.proc_handler   = sched_rt_handler,
		.extra1         = SYSCTL_ONE,
		.extra2         = SYSCTL_INT_MAX,
	},
	{
		.procname       = "sched_rt_runtime_us",
		.data           = &sysctl_sched_rt_runtime,
		.maxlen         = اندازه(صحیح),
		.mode           = 0644,
		.proc_handler   = sched_rt_handler,
		.extra1         = SYSCTL_NEG_ONE,
		.extra2         = (تهی *)&sysctl_sched_rt_period,
	},
	{
		.procname       = "sched_rr_timeslice_ms",
		.data           = &sysctl_sched_rr_timeslice,
		.maxlen         = اندازه(صحیح),
		.mode           = 0644,
		.proc_handler   = sched_rr_handler,
	},
};

ایستا صحیح __init sched_rt_sysctl_init(تهی)
{
	register_sysctl_init("kernel", sched_rt_sysctls);
	بازگشت 0;
}
late_initcall(sched_rt_sysctl_init);
#endif /* CONFIG_SYSCTL */

تهی init_rt_rq(ساختار rt_rq *rt_rq)
{
	ساختار rt_prio_array *array;
	صحیح i;

	array = &rt_rq->active;
	برای (i = 0; i < MAX_RT_PRIO; i++) {
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
	rt_rq->tg = &ریشه_task_group;
#endif
}

#ifdef CONFIG_RT_GROUP_SCHED

ایستا صحیح do_sched_rt_period_timer(ساختار rt_bandwidth *rt_b, صحیح overrun);

ایستا شمارش hrtimer_restart sched_rt_period_timer(ساختار hrtimer *timer)
{
	ساختار rt_bandwidth *rt_b =
		container_of(timer, ساختار rt_bandwidth, rt_period_timer);
	صحیح idle = 0;
	صحیح overrun;

	raw_spin_lock(&rt_b->rt_runtime_lock);
	برای (;;) {
		overrun = hrtimer_forward_now(timer, rt_b->rt_period);
		اگر (!overrun)
			بشکن;

		raw_spin_unlock(&rt_b->rt_runtime_lock);
		idle = do_sched_rt_period_timer(rt_b, overrun);
		raw_spin_lock(&rt_b->rt_runtime_lock);
	}
	اگر (idle)
		rt_b->rt_period_active = 0;
	raw_spin_unlock(&rt_b->rt_runtime_lock);

	بازگشت idle ? HRTIMER_NORESTART : HRTIMER_RESTART;
}

تهی init_rt_bandwidth(ساختار rt_bandwidth *rt_b, u64 period, u64 runtime)
{
	rt_b->rt_period = ns_to_ktime(period);
	rt_b->rt_runtime = runtime;

	raw_spin_lock_init(&rt_b->rt_runtime_lock);

	hrtimer_setup(&rt_b->rt_period_timer, sched_rt_period_timer, CLOCK_MONOTONIC,
		      HRTIMER_MODE_REL_HARD);
}

ایستا درونخطی تهی do_start_rt_bandwidth(ساختار rt_bandwidth *rt_b)
{
	raw_spin_lock(&rt_b->rt_runtime_lock);
	اگر (!rt_b->rt_period_active) {
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

ایستا تهی start_rt_bandwidth(ساختار rt_bandwidth *rt_b)
{
	اگر (!rt_bandwidth_enabled() || rt_b->rt_runtime == RUNTIME_INF)
		بازگشت;

	do_start_rt_bandwidth(rt_b);
}

ایستا تهی destroy_rt_bandwidth(ساختار rt_bandwidth *rt_b)
{
	hrtimer_cancel(&rt_b->rt_period_timer);
}

#define rt_entity_is_task(rt_se) (!(rt_se)->my_q)

ایستا درونخطی ساختار task_struct *rt_task_of(ساختار sched_rt_entity *rt_se)
{
	WARN_ON_ONCE(!rt_entity_is_task(rt_se));

	بازگشت container_of(rt_se, ساختار task_struct, rt);
}

ایستا درونخطی ساختار rq *rq_of_rt_rq(ساختار rt_rq *rt_rq)
{
	/* Cannot fold with non-CONFIG_RT_GROUP_SCHED version, layout */
	WARN_ON(!rt_group_sched_enabled() && rt_rq->tg != &ریشه_task_group);
	بازگشت rt_rq->rq;
}

ایستا درونخطی ساختار rt_rq *rt_rq_of_se(ساختار sched_rt_entity *rt_se)
{
	WARN_ON(!rt_group_sched_enabled() && rt_se->rt_rq->tg != &ریشه_task_group);
	بازگشت rt_se->rt_rq;
}

ایستا درونخطی ساختار rq *rq_of_rt_se(ساختار sched_rt_entity *rt_se)
{
	ساختار rt_rq *rt_rq = rt_se->rt_rq;

	WARN_ON(!rt_group_sched_enabled() && rt_rq->tg != &ریشه_task_group);
	بازگشت rt_rq->rq;
}

تهی unregister_rt_sched_group(ساختار task_group *tg)
{
	اگر (!rt_group_sched_enabled())
		بازگشت;

	اگر (tg->rt_se)
		destroy_rt_bandwidth(&tg->rt_bandwidth);
}

تهی free_rt_sched_group(ساختار task_group *tg)
{
	صحیح i;

	اگر (!rt_group_sched_enabled())
		بازگشت;

	for_each_possible_cpu(i) {
		اگر (tg->rt_rq)
			kfree(tg->rt_rq[i]);
		اگر (tg->rt_se)
			kfree(tg->rt_se[i]);
	}

	kfree(tg->rt_rq);
	kfree(tg->rt_se);
}

تهی init_tg_rt_entry(ساختار task_group *tg, ساختار rt_rq *rt_rq,
		ساختار sched_rt_entity *rt_se, صحیح cpu,
		ساختار sched_rt_entity *parent)
{
	ساختار rq *rq = cpu_rq(cpu);

	rt_rq->highest_prio.curr = MAX_RT_PRIO-1;
	rt_rq->rt_nr_boosted = 0;
	rt_rq->rq = rq;
	rt_rq->tg = tg;

	tg->rt_rq[cpu] = rt_rq;
	tg->rt_se[cpu] = rt_se;

	اگر (!rt_se)
		بازگشت;

	اگر (!parent)
		rt_se->rt_rq = &rq->rt;
	وگرنه
		rt_se->rt_rq = parent->my_q;

	rt_se->my_q = rt_rq;
	rt_se->parent = parent;
	INIT_LIST_HEAD(&rt_se->run_list);
}

صحیح alloc_rt_sched_group(ساختار task_group *tg, ساختار task_group *parent)
{
	ساختار rt_rq *rt_rq;
	ساختار sched_rt_entity *rt_se;
	صحیح i;

	اگر (!rt_group_sched_enabled())
		بازگشت 1;

	tg->rt_rq = kzalloc_objs(rt_rq, nr_cpu_ids);
	اگر (!tg->rt_rq)
		برو err;
	tg->rt_se = kzalloc_objs(rt_se, nr_cpu_ids);
	اگر (!tg->rt_se)
		برو err;

	init_rt_bandwidth(&tg->rt_bandwidth, ktime_to_ns(global_rt_period()), 0);

	for_each_possible_cpu(i) {
		rt_rq = kzalloc_گره(اندازه(ساختار rt_rq),
				     GFP_KERNEL, cpu_to_گره(i));
		اگر (!rt_rq)
			برو err;

		rt_se = kzalloc_گره(اندازه(ساختار sched_rt_entity),
				     GFP_KERNEL, cpu_to_گره(i));
		اگر (!rt_se)
			برو err_free_rq;

		init_rt_rq(rt_rq);
		rt_rq->rt_runtime = tg->rt_bandwidth.rt_runtime;
		init_tg_rt_entry(tg, rt_rq, rt_se, i, parent->rt_se[i]);
	}

	بازگشت 1;

err_free_rq:
	kfree(rt_rq);
err:
	بازگشت 0;
}

#وگرنه /* !CONFIG_RT_GROUP_SCHED: */

#define rt_entity_is_task(rt_se) (1)

ایستا درونخطی ساختار task_struct *rt_task_of(ساختار sched_rt_entity *rt_se)
{
	بازگشت container_of(rt_se, ساختار task_struct, rt);
}

ایستا درونخطی ساختار rq *rq_of_rt_rq(ساختار rt_rq *rt_rq)
{
	بازگشت container_of(rt_rq, ساختار rq, rt);
}

ایستا درونخطی ساختار rq *rq_of_rt_se(ساختار sched_rt_entity *rt_se)
{
	ساختار task_struct *p = rt_task_of(rt_se);

	بازگشت task_rq(p);
}

ایستا درونخطی ساختار rt_rq *rt_rq_of_se(ساختار sched_rt_entity *rt_se)
{
	ساختار rq *rq = rq_of_rt_se(rt_se);

	بازگشت &rq->rt;
}

تهی unregister_rt_sched_group(ساختار task_group *tg) { }

تهی free_rt_sched_group(ساختار task_group *tg) { }

صحیح alloc_rt_sched_group(ساختار task_group *tg, ساختار task_group *parent)
{
	بازگشت 1;
}
#endif /* !CONFIG_RT_GROUP_SCHED */

ایستا درونخطی bool need_pull_rt_task(ساختار rq *rq, ساختار task_struct *prev)
{
	/* Try to pull RT tasks here if we lower this rq's prio */
	بازگشت rq->online && rq->rt.highest_prio.curr > prev->prio;
}

ایستا درونخطی صحیح rt_overloaded(ساختار rq *rq)
{
	بازگشت atomic_read(&rq->rd->rto_شمار);
}

ایستا درونخطی تهی rt_set_overload(ساختار rq *rq)
{
	اگر (!rq->online)
		بازگشت;

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
	atomic_inc(&rq->rd->rto_شمار);
}

ایستا درونخطی تهی rt_clear_overload(ساختار rq *rq)
{
	اگر (!rq->online)
		بازگشت;

	/* the order here really doesn't matter */
	atomic_dec(&rq->rd->rto_شمار);
	cpumask_clear_cpu(rq->cpu, rq->rd->rto_mask);
}

ایستا درونخطی صحیح has_pushable_tasks(ساختار rq *rq)
{
	بازگشت !plist_head_empty(&rq->rt.pushable_tasks);
}

ایستا DEFINE_PER_CPU(ساختار balance_callback, rt_افزودن_head);
ایستا DEFINE_PER_CPU(ساختار balance_callback, rt_pull_head);

ایستا تهی افزودن_rt_tasks(ساختار rq *);
ایستا تهی pull_rt_task(ساختار rq *);

ایستا درونخطی تهی rt_queue_افزودن_tasks(ساختار rq *rq)
{
	اگر (!has_pushable_tasks(rq))
		بازگشت;

	queue_balance_callback(rq, &per_cpu(rt_افزودن_head, rq->cpu), افزودن_rt_tasks);
}

ایستا درونخطی تهی rt_queue_pull_task(ساختار rq *rq)
{
	queue_balance_callback(rq, &per_cpu(rt_pull_head, rq->cpu), pull_rt_task);
}

ایستا تهی enqueue_pushable_task(ساختار rq *rq, ساختار task_struct *p)
{
	plist_del(&p->pushable_tasks, &rq->rt.pushable_tasks);
	plist_گره_init(&p->pushable_tasks, p->prio);
	plist_add(&p->pushable_tasks, &rq->rt.pushable_tasks);

	/* Update the highest prio pushable task */
	اگر (p->prio < rq->rt.highest_prio.next)
		rq->rt.highest_prio.next = p->prio;

	اگر (!rq->rt.overloaded) {
		rt_set_overload(rq);
		rq->rt.overloaded = 1;
	}
}

ایستا تهی dequeue_pushable_task(ساختار rq *rq, ساختار task_struct *p)
{
	plist_del(&p->pushable_tasks, &rq->rt.pushable_tasks);

	/* Update the new highest prio pushable task */
	اگر (has_pushable_tasks(rq)) {
		p = plist_first_entry(&rq->rt.pushable_tasks,
				      ساختار task_struct, pushable_tasks);
		rq->rt.highest_prio.next = p->prio;
	} وگرنه {
		rq->rt.highest_prio.next = MAX_RT_PRIO-1;

		اگر (rq->rt.overloaded) {
			rt_clear_overload(rq);
			rq->rt.overloaded = 0;
		}
	}
}

ایستا تهی enqueue_بالاترین_rt_rq(ساختار rt_rq *rt_rq);
ایستا تهی dequeue_بالاترین_rt_rq(ساختار rt_rq *rt_rq, unsigned صحیح شمار);

ایستا درونخطی صحیح on_rt_rq(ساختار sched_rt_entity *rt_se)
{
	بازگشت rt_se->on_rq;
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
ایستا درونخطی bool rt_task_fits_capacity(ساختار task_struct *p, صحیح cpu)
{
	unsigned صحیح min_cap;
	unsigned صحیح max_cap;
	unsigned صحیح cpu_cap;

	/* Only heterogeneous systems can benefit from this check */
	اگر (!sched_asym_cpucap_active())
		بازگشت true;

	min_cap = uclamp_eff_مقدار(p, UCLAMP_MIN);
	max_cap = uclamp_eff_مقدار(p, UCLAMP_MAX);

	cpu_cap = arch_scale_cpu_capacity(cpu);

	بازگشت cpu_cap >= min(min_cap, max_cap);
}
#وگرنه /* !CONFIG_UCLAMP_TASK: */
ایستا درونخطی bool rt_task_fits_capacity(ساختار task_struct *p, صحیح cpu)
{
	بازگشت true;
}
#endif /* !CONFIG_UCLAMP_TASK */

#ifdef CONFIG_RT_GROUP_SCHED

ایستا درونخطی u64 sched_rt_runtime(ساختار rt_rq *rt_rq)
{
	بازگشت rt_rq->rt_runtime;
}

ایستا درونخطی u64 sched_rt_period(ساختار rt_rq *rt_rq)
{
	بازگشت ktime_to_ns(rt_rq->tg->rt_bandwidth.rt_period);
}

تعریفنوع ساختار task_group *rt_rq_iter_t;

ایستا درونخطی ساختار task_group *next_task_group(ساختار task_group *tg)
{
	اگر (!rt_group_sched_enabled()) {
		WARN_ON(tg != &ریشه_task_group);
		بازگشت پوچ;
	}

	انجامبده {
		tg = list_entry_rcu(tg->list.next,
			typeof(ساختار task_group), list);
	} وقتی (&tg->list != &task_groups && task_group_is_autogroup(tg));

	اگر (&tg->list == &task_groups)
		tg = پوچ;

	بازگشت tg;
}

#define for_each_rt_rq(rt_rq, iter, rq)					\
	برای (iter = &ریشه_task_group;					\
		iter && (rt_rq = iter->rt_rq[cpu_of(rq)]);		\
		iter = next_task_group(iter))

#define for_each_sched_rt_entity(rt_se) \
	برای (; rt_se; rt_se = rt_se->parent)

ایستا درونخطی ساختار rt_rq *group_rt_rq(ساختار sched_rt_entity *rt_se)
{
	بازگشت rt_se->my_q;
}

ایستا تهی enqueue_rt_entity(ساختار sched_rt_entity *rt_se, unsigned صحیح flags);
ایستا تهی dequeue_rt_entity(ساختار sched_rt_entity *rt_se, unsigned صحیح flags);

ایستا تهی sched_rt_rq_enqueue(ساختار rt_rq *rt_rq)
{
	ساختار task_struct *donor = rq_of_rt_rq(rt_rq)->donor;
	ساختار rq *rq = rq_of_rt_rq(rt_rq);
	ساختار sched_rt_entity *rt_se;

	صحیح cpu = cpu_of(rq);

	rt_se = rt_rq->tg->rt_se[cpu];

	اگر (rt_rq->rt_nr_running) {
		اگر (!rt_se)
			enqueue_بالاترین_rt_rq(rt_rq);
		وگرنه اگر (!on_rt_rq(rt_se))
			enqueue_rt_entity(rt_se, 0);

		اگر (rt_rq->highest_prio.curr < donor->prio)
			resched_curr(rq);
	}
}

ایستا تهی sched_rt_rq_dequeue(ساختار rt_rq *rt_rq)
{
	ساختار sched_rt_entity *rt_se;
	صحیح cpu = cpu_of(rq_of_rt_rq(rt_rq));

	rt_se = rt_rq->tg->rt_se[cpu];

	اگر (!rt_se) {
		dequeue_بالاترین_rt_rq(rt_rq, rt_rq->rt_nr_running);
		/* Kick cpufreq (see the comment in kernel/sched/sched.h). */
		cpufreq_update_util(rq_of_rt_rq(rt_rq), 0);
	}
	وگرنه اگر (on_rt_rq(rt_se))
		dequeue_rt_entity(rt_se, 0);
}

ایستا درونخطی صحیح rt_rq_throttled(ساختار rt_rq *rt_rq)
{
	بازگشت rt_rq->rt_throttled && !rt_rq->rt_nr_boosted;
}

ایستا صحیح rt_se_boosted(ساختار sched_rt_entity *rt_se)
{
	ساختار rt_rq *rt_rq = group_rt_rq(rt_se);
	ساختار task_struct *p;

	اگر (rt_rq)
		بازگشت !!rt_rq->rt_nr_boosted;

	p = rt_task_of(rt_se);
	بازگشت p->prio != p->normal_prio;
}

ایستا درونخطی ثابت ساختار cpumask *sched_rt_period_mask(تهی)
{
	بازگشت this_rq()->rd->span;
}

ایستا درونخطی
ساختار rt_rq *sched_rt_period_rt_rq(ساختار rt_bandwidth *rt_b, صحیح cpu)
{
	بازگشت container_of(rt_b, ساختار task_group, rt_bandwidth)->rt_rq[cpu];
}

ایستا درونخطی ساختار rt_bandwidth *sched_rt_bandwidth(ساختار rt_rq *rt_rq)
{
	بازگشت &rt_rq->tg->rt_bandwidth;
}

bool sched_rt_bandwidth_account(ساختار rt_rq *rt_rq)
{
	ساختار rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);

	بازگشت (hrtimer_active(&rt_b->rt_period_timer) ||
		rt_rq->rt_time < rt_b->rt_runtime);
}

/*
 * We ran out of runtime, see if we can borrow some from our neighbours.
 */
ایستا تهی do_balance_runtime(ساختار rt_rq *rt_rq)
{
	ساختار rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);
	ساختار ریشه_domain *rd = rq_of_rt_rq(rt_rq)->rd;
	صحیح i, weight;
	u64 rt_period;

	weight = cpumask_weight(rd->span);

	raw_spin_lock(&rt_b->rt_runtime_lock);
	rt_period = ktime_to_ns(rt_b->rt_period);
	for_each_cpu(i, rd->span) {
		ساختار rt_rq *iter = sched_rt_period_rt_rq(rt_b, i);
		s64 diff;

		اگر (iter == rt_rq)
			ادامه;

		raw_spin_lock(&iter->rt_runtime_lock);
		/*
		 * Either all rqs have inf runtime and there's nothing to steal
		 * or __disable_runtime() below sets a specific rq to inf to
		 * indicate its been disabled and disallow stealing.
		 */
		اگر (iter->rt_runtime == RUNTIME_INF)
			برو next;

		/*
		 * From runqueues with spare time, take 1/n part of their
		 * spare time, but no more than our period.
		 */
		diff = iter->rt_runtime - iter->rt_time;
		اگر (diff > 0) {
			diff = div_u64((u64)diff, weight);
			اگر (rt_rq->rt_runtime + diff > rt_period)
				diff = rt_period - rt_rq->rt_runtime;
			iter->rt_runtime -= diff;
			rt_rq->rt_runtime += diff;
			اگر (rt_rq->rt_runtime == rt_period) {
				raw_spin_unlock(&iter->rt_runtime_lock);
				بشکن;
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
ایستا تهی __disable_runtime(ساختار rq *rq)
{
	ساختار ریشه_domain *rd = rq->rd;
	rt_rq_iter_t iter;
	ساختار rt_rq *rt_rq;

	اگر (unlikely(!scheduler_running))
		بازگشت;

	for_each_rt_rq(rt_rq, iter, rq) {
		ساختار rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);
		s64 want;
		صحیح i;

		raw_spin_lock(&rt_b->rt_runtime_lock);
		raw_spin_lock(&rt_rq->rt_runtime_lock);
		/*
		 * Either we're all inf and nobody needs to borrow, or we're
		 * already disabled and thus have nothing to do, or we have
		 * exactly the right amount of runtime to take out.
		 */
		اگر (rt_rq->rt_runtime == RUNTIME_INF ||
				rt_rq->rt_runtime == rt_b->rt_runtime)
			برو balanced;
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
			ساختار rt_rq *iter = sched_rt_period_rt_rq(rt_b, i);
			s64 diff;

			/*
			 * Can't reclaim from ourselves or disabled runqueues.
			 */
			اگر (iter == rt_rq || iter->rt_runtime == RUNTIME_INF)
				ادامه;

			raw_spin_lock(&iter->rt_runtime_lock);
			اگر (want > 0) {
				diff = min_t(s64, iter->rt_runtime, want);
				iter->rt_runtime -= diff;
				want -= diff;
			} وگرنه {
				iter->rt_runtime -= want;
				want -= want;
			}
			raw_spin_unlock(&iter->rt_runtime_lock);

			اگر (!want)
				بشکن;
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

ایستا تهی __enable_runtime(ساختار rq *rq)
{
	rt_rq_iter_t iter;
	ساختار rt_rq *rt_rq;

	اگر (unlikely(!scheduler_running))
		بازگشت;

	/*
	 * Reset each runqueue's bandwidth settings
	 */
	for_each_rt_rq(rt_rq, iter, rq) {
		ساختار rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);

		raw_spin_lock(&rt_b->rt_runtime_lock);
		raw_spin_lock(&rt_rq->rt_runtime_lock);
		rt_rq->rt_runtime = rt_b->rt_runtime;
		rt_rq->rt_time = 0;
		rt_rq->rt_throttled = 0;
		raw_spin_unlock(&rt_rq->rt_runtime_lock);
		raw_spin_unlock(&rt_b->rt_runtime_lock);
	}
}

ایستا تهی balance_runtime(ساختار rt_rq *rt_rq)
{
	اگر (!sched_feat(RT_RUNTIME_SHARE))
		بازگشت;

	اگر (rt_rq->rt_time > rt_rq->rt_runtime) {
		raw_spin_unlock(&rt_rq->rt_runtime_lock);
		do_balance_runtime(rt_rq);
		raw_spin_lock(&rt_rq->rt_runtime_lock);
	}
}

ایستا صحیح do_sched_rt_period_timer(ساختار rt_bandwidth *rt_b, صحیح overrun)
{
	صحیح i, idle = 1, throttled = 0;
	ثابت ساختار cpumask *span;

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
	اگر (rt_b == &ریشه_task_group.rt_bandwidth)
		span = cpu_online_mask;

	for_each_cpu(i, span) {
		صحیح enqueue = 0;
		ساختار rt_rq *rt_rq = sched_rt_period_rt_rq(rt_b, i);
		ساختار rq *rq = rq_of_rt_rq(rt_rq);
		ساختار rq_flags rf;
		صحیح skip;

		/*
		 * When span == cpu_online_mask, taking each rq->lock
		 * can be time-consuming. Try to avoid it when possible.
		 */
		raw_spin_lock(&rt_rq->rt_runtime_lock);
		اگر (!sched_feat(RT_RUNTIME_SHARE) && rt_rq->rt_runtime != RUNTIME_INF)
			rt_rq->rt_runtime = rt_b->rt_runtime;
		skip = !rt_rq->rt_time && !rt_rq->rt_nr_running;
		raw_spin_unlock(&rt_rq->rt_runtime_lock);
		اگر (skip)
			ادامه;

		rq_lock(rq, &rf);
		update_rq_clock(rq);

		اگر (rt_rq->rt_time) {
			u64 runtime;

			raw_spin_lock(&rt_rq->rt_runtime_lock);
			اگر (rt_rq->rt_throttled)
				balance_runtime(rt_rq);
			runtime = rt_rq->rt_runtime;
			rt_rq->rt_time -= min(rt_rq->rt_time, overrun*runtime);
			اگر (rt_rq->rt_throttled && rt_rq->rt_time < runtime) {
				rt_rq->rt_throttled = 0;
				enqueue = 1;

				/*
				 * When we're idle and a woken (rt) task is
				 * throttled wakeup_preempt() will set
				 * skip_update and the time between the wakeup
				 * and this unthrottle will get accounted as
				 * 'runtime'.
				 */
				اگر (rt_rq->rt_nr_running && rq->curr == rq->idle)
					rq_clock_cancel_skipupdate(rq);
			}
			اگر (rt_rq->rt_time || rt_rq->rt_nr_running)
				idle = 0;
			raw_spin_unlock(&rt_rq->rt_runtime_lock);
		} وگرنه اگر (rt_rq->rt_nr_running) {
			idle = 0;
			اگر (!rt_rq_throttled(rt_rq))
				enqueue = 1;
		}
		اگر (rt_rq->rt_throttled)
			throttled = 1;

		اگر (enqueue)
			sched_rt_rq_enqueue(rt_rq);
		rq_unlock(rq, &rf);
	}

	اگر (!throttled && (!rt_bandwidth_enabled() || rt_b->rt_runtime == RUNTIME_INF))
		بازگشت 1;

	بازگشت idle;
}

ایستا صحیح sched_rt_runtime_exceeded(ساختار rt_rq *rt_rq)
{
	u64 runtime = sched_rt_runtime(rt_rq);

	اگر (rt_rq->rt_throttled)
		بازگشت rt_rq_throttled(rt_rq);

	اگر (runtime >= sched_rt_period(rt_rq))
		بازگشت 0;

	balance_runtime(rt_rq);
	runtime = sched_rt_runtime(rt_rq);
	اگر (runtime == RUNTIME_INF)
		بازگشت 0;

	اگر (rt_rq->rt_time > runtime) {
		ساختار rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);

		/*
		 * Don't actually throttle groups that have no runtime assigned
		 * but accrue some time due to boosting.
		 */
		اگر (likely(rt_b->rt_runtime)) {
			rt_rq->rt_throttled = 1;
			printk_deferred_once("sched: RT throttling activated\n");
		} وگرنه {
			/*
			 * In case we did anyway, make it go away,
			 * replenishment is a joke, since it will replenish us
			 * with exactly 0 ns.
			 */
			rt_rq->rt_time = 0;
		}

		اگر (rt_rq_throttled(rt_rq)) {
			sched_rt_rq_dequeue(rt_rq);
			بازگشت 1;
		}
	}

	بازگشت 0;
}

#وگرنه /* !CONFIG_RT_GROUP_SCHED: */

تعریفنوع ساختار rt_rq *rt_rq_iter_t;

#define for_each_rt_rq(rt_rq, iter, rq) \
	برای ((تهی) iter, rt_rq = &rq->rt; rt_rq; rt_rq = پوچ)

#define for_each_sched_rt_entity(rt_se) \
	برای (; rt_se; rt_se = پوچ)

ایستا درونخطی ساختار rt_rq *group_rt_rq(ساختار sched_rt_entity *rt_se)
{
	بازگشت پوچ;
}

ایستا درونخطی تهی sched_rt_rq_enqueue(ساختار rt_rq *rt_rq)
{
	ساختار rq *rq = rq_of_rt_rq(rt_rq);

	اگر (!rt_rq->rt_nr_running)
		بازگشت;

	enqueue_بالاترین_rt_rq(rt_rq);
	resched_curr(rq);
}

ایستا درونخطی تهی sched_rt_rq_dequeue(ساختار rt_rq *rt_rq)
{
	dequeue_بالاترین_rt_rq(rt_rq, rt_rq->rt_nr_running);
}

ایستا درونخطی صحیح rt_rq_throttled(ساختار rt_rq *rt_rq)
{
	بازگشت false;
}

ایستا درونخطی ثابت ساختار cpumask *sched_rt_period_mask(تهی)
{
	بازگشت cpu_online_mask;
}

ایستا درونخطی
ساختار rt_rq *sched_rt_period_rt_rq(ساختار rt_bandwidth *rt_b, صحیح cpu)
{
	بازگشت &cpu_rq(cpu)->rt;
}

ایستا تهی __enable_runtime(ساختار rq *rq) { }
ایستا تهی __disable_runtime(ساختار rq *rq) { }

#endif /* !CONFIG_RT_GROUP_SCHED */

ایستا درونخطی صحیح rt_se_prio(ساختار sched_rt_entity *rt_se)
{
#ifdef CONFIG_RT_GROUP_SCHED
	ساختار rt_rq *rt_rq = group_rt_rq(rt_se);

	اگر (rt_rq)
		بازگشت rt_rq->highest_prio.curr;
#endif

	بازگشت rt_task_of(rt_se)->prio;
}

/*
 * Update the current task's runtime statistics. Skip current tasks that
 * are not in our scheduling class.
 */
ایستا تهی update_curr_rt(ساختار rq *rq)
{
	ساختار task_struct *donor = rq->donor;
	s64 delta_exec;

	اگر (donor->sched_class != &rt_sched_class)
		بازگشت;

	delta_exec = update_curr_common(rq);
	اگر (unlikely(delta_exec <= 0))
		بازگشت;

#ifdef CONFIG_RT_GROUP_SCHED
	ساختار sched_rt_entity *rt_se = &donor->rt;

	اگر (!rt_bandwidth_enabled())
		بازگشت;

	for_each_sched_rt_entity(rt_se) {
		ساختار rt_rq *rt_rq = rt_rq_of_se(rt_se);
		صحیح exceeded;

		اگر (sched_rt_runtime(rt_rq) != RUNTIME_INF) {
			raw_spin_lock(&rt_rq->rt_runtime_lock);
			rt_rq->rt_time += delta_exec;
			exceeded = sched_rt_runtime_exceeded(rt_rq);
			اگر (exceeded)
				resched_curr(rq);
			raw_spin_unlock(&rt_rq->rt_runtime_lock);
			اگر (exceeded)
				do_start_rt_bandwidth(sched_rt_bandwidth(rt_rq));
		}
	}
#endif /* CONFIG_RT_GROUP_SCHED */
}

ایستا تهی
dequeue_بالاترین_rt_rq(ساختار rt_rq *rt_rq, unsigned صحیح شمار)
{
	ساختار rq *rq = rq_of_rt_rq(rt_rq);

	BUG_ON(&rq->rt != rt_rq);

	اگر (!rt_rq->rt_queued)
		بازگشت;

	BUG_ON(!rq->nr_running);

	sub_nr_running(rq, شمار);
	rt_rq->rt_queued = 0;

}

ایستا تهی
enqueue_بالاترین_rt_rq(ساختار rt_rq *rt_rq)
{
	ساختار rq *rq = rq_of_rt_rq(rt_rq);

	BUG_ON(&rq->rt != rt_rq);

	اگر (rt_rq->rt_queued)
		بازگشت;

	اگر (rt_rq_throttled(rt_rq))
		بازگشت;

	اگر (rt_rq->rt_nr_running) {
		add_nr_running(rq, rt_rq->rt_nr_running);
		rt_rq->rt_queued = 1;
	}

	/* Kick cpufreq (see the comment in kernel/sched/sched.h). */
	cpufreq_update_util(rq, 0);
}

ایستا تهی
inc_rt_prio_smp(ساختار rt_rq *rt_rq, صحیح prio, صحیح prev_prio)
{
	ساختار rq *rq = rq_of_rt_rq(rt_rq);

	/*
	 * Change rq's cpupri only if rt_rq is the top queue.
	 */
	اگر (IS_ENABLED(CONFIG_RT_GROUP_SCHED) && &rq->rt != rt_rq)
		بازگشت;

	اگر (rq->online && prio < prev_prio)
		cpupri_set(&rq->rd->cpupri, rq->cpu, prio);
}

ایستا تهی
dec_rt_prio_smp(ساختار rt_rq *rt_rq, صحیح prio, صحیح prev_prio)
{
	ساختار rq *rq = rq_of_rt_rq(rt_rq);

	/*
	 * Change rq's cpupri only if rt_rq is the top queue.
	 */
	اگر (IS_ENABLED(CONFIG_RT_GROUP_SCHED) && &rq->rt != rt_rq)
		بازگشت;

	اگر (rq->online && rt_rq->highest_prio.curr != prev_prio)
		cpupri_set(&rq->rd->cpupri, rq->cpu, rt_rq->highest_prio.curr);
}

ایستا تهی
inc_rt_prio(ساختار rt_rq *rt_rq, صحیح prio)
{
	صحیح prev_prio = rt_rq->highest_prio.curr;

	اگر (prio < prev_prio)
		rt_rq->highest_prio.curr = prio;

	inc_rt_prio_smp(rt_rq, prio, prev_prio);
}

ایستا تهی
dec_rt_prio(ساختار rt_rq *rt_rq, صحیح prio)
{
	صحیح prev_prio = rt_rq->highest_prio.curr;

	اگر (rt_rq->rt_nr_running) {

		WARN_ON(prio < prev_prio);

		/*
		 * This may have been our highest task, and therefore
		 * we may have some re-computation to do
		 */
		اگر (prio == prev_prio) {
			ساختار rt_prio_array *array = &rt_rq->active;

			rt_rq->highest_prio.curr =
				sched_find_first_bit(array->bitmap);
		}

	} وگرنه {
		rt_rq->highest_prio.curr = MAX_RT_PRIO-1;
	}

	dec_rt_prio_smp(rt_rq, prio, prev_prio);
}

#ifdef CONFIG_RT_GROUP_SCHED

ایستا تهی
inc_rt_group(ساختار sched_rt_entity *rt_se, ساختار rt_rq *rt_rq)
{
	اگر (rt_se_boosted(rt_se))
		rt_rq->rt_nr_boosted++;

	start_rt_bandwidth(&rt_rq->tg->rt_bandwidth);
}

ایستا تهی
dec_rt_group(ساختار sched_rt_entity *rt_se, ساختار rt_rq *rt_rq)
{
	اگر (rt_se_boosted(rt_se))
		rt_rq->rt_nr_boosted--;

	WARN_ON(!rt_rq->rt_nr_running && rt_rq->rt_nr_boosted);
}

#وگرنه /* !CONFIG_RT_GROUP_SCHED: */

ایستا تهی
inc_rt_group(ساختار sched_rt_entity *rt_se, ساختار rt_rq *rt_rq)
{
}

ایستا درونخطی
تهی dec_rt_group(ساختار sched_rt_entity *rt_se, ساختار rt_rq *rt_rq) {}

#endif /* !CONFIG_RT_GROUP_SCHED */

ایستا درونخطی
unsigned صحیح rt_se_nr_running(ساختار sched_rt_entity *rt_se)
{
	ساختار rt_rq *group_rq = group_rt_rq(rt_se);

	اگر (group_rq)
		بازگشت group_rq->rt_nr_running;
	وگرنه
		بازگشت 1;
}

ایستا درونخطی
unsigned صحیح rt_se_rr_nr_running(ساختار sched_rt_entity *rt_se)
{
	ساختار rt_rq *group_rq = group_rt_rq(rt_se);
	ساختار task_struct *tsk;

	اگر (group_rq)
		بازگشت group_rq->rr_nr_running;

	tsk = rt_task_of(rt_se);

	بازگشت (tsk->policy == SCHED_RR) ? 1 : 0;
}

ایستا درونخطی
تهی inc_rt_tasks(ساختار sched_rt_entity *rt_se, ساختار rt_rq *rt_rq)
{
	صحیح prio = rt_se_prio(rt_se);

	WARN_ON(!rt_prio(prio));
	rt_rq->rt_nr_running += rt_se_nr_running(rt_se);
	rt_rq->rr_nr_running += rt_se_rr_nr_running(rt_se);

	inc_rt_prio(rt_rq, prio);
	inc_rt_group(rt_se, rt_rq);
}

ایستا درونخطی
تهی dec_rt_tasks(ساختار sched_rt_entity *rt_se, ساختار rt_rq *rt_rq)
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
ایستا درونخطی bool move_entity(unsigned صحیح flags)
{
	اگر ((flags & (DEQUEUE_SAVE | DEQUEUE_MOVE)) == DEQUEUE_SAVE)
		بازگشت false;

	بازگشت true;
}

ایستا تهی __delist_rt_entity(ساختار sched_rt_entity *rt_se, ساختار rt_prio_array *array)
{
	list_del_init(&rt_se->run_list);

	اگر (list_empty(array->queue + rt_se_prio(rt_se)))
		__clear_bit(rt_se_prio(rt_se), array->bitmap);

	rt_se->on_list = 0;
}

ایستا درونخطی ساختار sched_statistics *
__schedstats_from_rt_se(ساختار sched_rt_entity *rt_se)
{
	/* schedstats is not supported for rt group. */
	اگر (!rt_entity_is_task(rt_se))
		بازگشت پوچ;

	بازگشت &rt_task_of(rt_se)->stats;
}

ایستا درونخطی تهی
update_stats_wait_start_rt(ساختار rt_rq *rt_rq, ساختار sched_rt_entity *rt_se)
{
	ساختار sched_statistics *stats;
	ساختار task_struct *p = پوچ;

	اگر (!schedstat_enabled())
		بازگشت;

	اگر (rt_entity_is_task(rt_se))
		p = rt_task_of(rt_se);

	stats = __schedstats_from_rt_se(rt_se);
	اگر (!stats)
		بازگشت;

	__update_stats_wait_start(rq_of_rt_rq(rt_rq), p, stats);
}

ایستا درونخطی تهی
update_stats_enqueue_sleeper_rt(ساختار rt_rq *rt_rq, ساختار sched_rt_entity *rt_se)
{
	ساختار sched_statistics *stats;
	ساختار task_struct *p = پوچ;

	اگر (!schedstat_enabled())
		بازگشت;

	اگر (rt_entity_is_task(rt_se))
		p = rt_task_of(rt_se);

	stats = __schedstats_from_rt_se(rt_se);
	اگر (!stats)
		بازگشت;

	__update_stats_enqueue_sleeper(rq_of_rt_rq(rt_rq), p, stats);
}

ایستا درونخطی تهی
update_stats_enqueue_rt(ساختار rt_rq *rt_rq, ساختار sched_rt_entity *rt_se,
			صحیح flags)
{
	اگر (!schedstat_enabled())
		بازگشت;

	اگر (flags & ENQUEUE_WAKEUP)
		update_stats_enqueue_sleeper_rt(rt_rq, rt_se);
}

ایستا درونخطی تهی
update_stats_wait_end_rt(ساختار rt_rq *rt_rq, ساختار sched_rt_entity *rt_se)
{
	ساختار sched_statistics *stats;
	ساختار task_struct *p = پوچ;

	اگر (!schedstat_enabled())
		بازگشت;

	اگر (rt_entity_is_task(rt_se))
		p = rt_task_of(rt_se);

	stats = __schedstats_from_rt_se(rt_se);
	اگر (!stats)
		بازگشت;

	__update_stats_wait_end(rq_of_rt_rq(rt_rq), p, stats);
}

ایستا درونخطی تهی
update_stats_dequeue_rt(ساختار rt_rq *rt_rq, ساختار sched_rt_entity *rt_se,
			صحیح flags)
{
	ساختار task_struct *p = پوچ;
	ساختار rq *rq = rq_of_rt_rq(rt_rq);

	اگر (!schedstat_enabled())
		بازگشت;

	اگر (rt_entity_is_task(rt_se)) {
		p = rt_task_of(rt_se);

		اگر (p != rq->curr)
			update_stats_wait_end_rt(rt_rq, rt_se);
	}

	اگر ((flags & DEQUEUE_SLEEP) && p) {
		unsigned صحیح state;

		state = READ_ONCE(p->__state);
		اگر (state & TASK_INTERRUPTIBLE)
			__schedstat_set(p->stats.sleep_start,
					rq_clock(rq_of_rt_rq(rt_rq)));

		اگر (state & TASK_UNINTERRUPTIBLE)
			__schedstat_set(p->stats.block_start,
					rq_clock(rq_of_rt_rq(rt_rq)));
	}
}

ایستا تهی __enqueue_rt_entity(ساختار sched_rt_entity *rt_se, unsigned صحیح flags)
{
	ساختار rt_rq *rt_rq = rt_rq_of_se(rt_se);
	ساختار rt_prio_array *array = &rt_rq->active;
	ساختار rt_rq *group_rq = group_rt_rq(rt_se);
	ساختار list_head *queue = array->queue + rt_se_prio(rt_se);

	/*
	 * Don't enqueue the group if its throttled, or when empty.
	 * The latter is a consequence of the former when a child group
	 * get throttled and the current group doesn't have any other
	 * active members.
	 */
	اگر (group_rq && (rt_rq_throttled(group_rq) || !group_rq->rt_nr_running)) {
		اگر (rt_se->on_list)
			__delist_rt_entity(rt_se, array);
		بازگشت;
	}

	اگر (move_entity(flags)) {
		WARN_ON_ONCE(rt_se->on_list);
		اگر (flags & ENQUEUE_HEAD)
			list_add(&rt_se->run_list, queue);
		وگرنه
			list_add_tail(&rt_se->run_list, queue);

		__set_bit(rt_se_prio(rt_se), array->bitmap);
		rt_se->on_list = 1;
	}
	rt_se->on_rq = 1;

	inc_rt_tasks(rt_se, rt_rq);
}

ایستا تهی __dequeue_rt_entity(ساختار sched_rt_entity *rt_se, unsigned صحیح flags)
{
	ساختار rt_rq *rt_rq = rt_rq_of_se(rt_se);
	ساختار rt_prio_array *array = &rt_rq->active;

	اگر (move_entity(flags)) {
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
ایستا تهی dequeue_rt_پشته(ساختار sched_rt_entity *rt_se, unsigned صحیح flags)
{
	ساختار sched_rt_entity *back = پوچ;
	unsigned صحیح rt_nr_running;

	for_each_sched_rt_entity(rt_se) {
		rt_se->back = back;
		back = rt_se;
	}

	rt_nr_running = rt_rq_of_se(back)->rt_nr_running;

	برای (rt_se = back; rt_se; rt_se = rt_se->back) {
		اگر (on_rt_rq(rt_se))
			__dequeue_rt_entity(rt_se, flags);
	}

	dequeue_بالاترین_rt_rq(rt_rq_of_se(back), rt_nr_running);
}

ایستا تهی enqueue_rt_entity(ساختار sched_rt_entity *rt_se, unsigned صحیح flags)
{
	ساختار rq *rq = rq_of_rt_se(rt_se);

	update_stats_enqueue_rt(rt_rq_of_se(rt_se), rt_se, flags);

	dequeue_rt_پشته(rt_se, flags);
	for_each_sched_rt_entity(rt_se)
		__enqueue_rt_entity(rt_se, flags);
	enqueue_بالاترین_rt_rq(&rq->rt);
}

ایستا تهی dequeue_rt_entity(ساختار sched_rt_entity *rt_se, unsigned صحیح flags)
{
	ساختار rq *rq = rq_of_rt_se(rt_se);

	update_stats_dequeue_rt(rt_rq_of_se(rt_se), rt_se, flags);

	dequeue_rt_پشته(rt_se, flags);

	for_each_sched_rt_entity(rt_se) {
		ساختار rt_rq *rt_rq = group_rt_rq(rt_se);

		اگر (rt_rq && rt_rq->rt_nr_running)
			__enqueue_rt_entity(rt_se, flags);
	}
	enqueue_بالاترین_rt_rq(&rq->rt);
}

/*
 * Adding/removing a task to/from a priority array:
 */
ایستا تهی
enqueue_task_rt(ساختار rq *rq, ساختار task_struct *p, صحیح flags)
{
	ساختار sched_rt_entity *rt_se = &p->rt;

	اگر (flags & ENQUEUE_WAKEUP)
		rt_se->timeout = 0;

	check_schedstat_required();
	update_stats_wait_start_rt(rt_rq_of_se(rt_se), rt_se);

	enqueue_rt_entity(rt_se, flags);

	اگر (task_is_blocked(p))
		بازگشت;

	اگر (!task_current(rq, p) && p->nr_cpus_allowed > 1)
		enqueue_pushable_task(rq, p);
}

ایستا bool dequeue_task_rt(ساختار rq *rq, ساختار task_struct *p, صحیح flags)
{
	ساختار sched_rt_entity *rt_se = &p->rt;

	update_curr_rt(rq);
	dequeue_rt_entity(rt_se, flags);

	dequeue_pushable_task(rq, p);

	بازگشت true;
}

/*
 * Put task to the head or the end of the run list without the overhead of
 * dequeue followed by enqueue.
 */
ایستا تهی
requeue_rt_entity(ساختار rt_rq *rt_rq, ساختار sched_rt_entity *rt_se, صحیح head)
{
	اگر (on_rt_rq(rt_se)) {
		ساختار rt_prio_array *array = &rt_rq->active;
		ساختار list_head *queue = array->queue + rt_se_prio(rt_se);

		اگر (head)
			list_move(&rt_se->run_list, queue);
		وگرنه
			list_move_tail(&rt_se->run_list, queue);
	}
}

ایستا تهی requeue_task_rt(ساختار rq *rq, ساختار task_struct *p, صحیح head)
{
	ساختار sched_rt_entity *rt_se = &p->rt;
	ساختار rt_rq *rt_rq;

	for_each_sched_rt_entity(rt_se) {
		rt_rq = rt_rq_of_se(rt_se);
		requeue_rt_entity(rt_rq, rt_se, head);
	}
}

ایستا تهی yield_task_rt(ساختار rq *rq)
{
	requeue_task_rt(rq, rq->donor, 0);
}

ایستا صحیح find_lowest_rq(ساختار task_struct *task);

ایستا صحیح
select_task_rq_rt(ساختار task_struct *p, صحیح cpu, صحیح flags)
{
	ساختار task_struct *curr, *donor;
	ساختار rq *rq;
	bool test;

	/* For anything but wake ups, just return the task_cpu */
	اگر (!(flags & (WF_TTWU | WF_FORK)))
		برو out;

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

	اگر (test || !rt_task_fits_capacity(p, cpu)) {
		صحیح target = find_lowest_rq(p);

		/*
		 * Bail out if we were forcing a migration to find a better
		 * fitting CPU but our search failed.
		 */
		اگر (!test && target != -1 && !rt_task_fits_capacity(p, target))
			برو out_unlock;

		/*
		 * Don't bother moving it if the destination CPU is
		 * not running a lower priority task.
		 */
		اگر (target != -1 &&
		    p->prio < cpu_rq(target)->rt.highest_prio.curr)
			cpu = target;
	}

out_unlock:
	rcu_read_unlock();

out:
	بازگشت cpu;
}

ایستا تهی check_preempt_equal_prio(ساختار rq *rq, ساختار task_struct *p)
{
	اگر (rq->curr->nr_cpus_allowed == 1 ||
	    !cpupri_find(&rq->rd->cpupri, rq->donor, پوچ))
		بازگشت;

	/*
	 * p is migratable, so let's not schedule it and
	 * see if it is pushed or pulled somewhere else.
	 */
	اگر (p->nr_cpus_allowed != 1 &&
	    cpupri_find(&rq->rd->cpupri, p, پوچ))
		بازگشت;

	/*
	 * There appear to be other CPUs that can accept
	 * the current task but none can run 'p', so lets reschedule
	 * to try and push the current task away:
	 */
	requeue_task_rt(rq, p, 1);
	resched_curr(rq);
}

ایستا صحیح balance_rt(ساختار rq *rq, ساختار rq_flags *rf)
{
	/*
	 * Note, rq->donor may change during rq lock drops,
	 * so don't re-use p across lock drops
	 */
	ساختار task_struct *p = rq->donor;

	اگر (!on_rt_rq(&p->rt) && need_pull_rt_task(rq, p)) {
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

	بازگشت sched_stop_runnable(rq) || sched_dl_runnable(rq) || sched_rt_runnable(rq);
}

/*
 * Preempt the current task with a newly woken task if needed:
 */
ایستا تهی wakeup_preempt_rt(ساختار rq *rq, ساختار task_struct *p, صحیح flags)
{
	ساختار task_struct *donor = rq->donor;

	/*
	 * XXX If we're preempted by DL, queue a push?
	 */
	اگر (p->sched_class != &rt_sched_class ||
	    donor->sched_class != &rt_sched_class)
		بازگشت;

	اگر (p->prio < donor->prio) {
		resched_curr(rq);
		بازگشت;
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
	اگر (p->prio == donor->prio && !test_tsk_need_resched(rq->curr))
		check_preempt_equal_prio(rq, p);
}

ایستا درونخطی تهی set_next_task_rt(ساختار rq *rq, ساختار task_struct *p, شمارش snt_e type)
{
	ساختار sched_rt_entity *rt_se = &p->rt;
	ساختار rt_rq *rt_rq = &rq->rt;

	اگر (type == SNT_REPICK)
		بازگشت;

	p->se.exec_start = rq_clock_task(rq);
	اگر (on_rt_rq(&p->rt))
		update_stats_wait_end_rt(rt_rq, rt_se);

	/* The running task is never eligible for pushing */
	dequeue_pushable_task(rq, p);

	اگر (type != SNT_PICK)
		بازگشت;

	/*
	 * If prev task was rt, put_prev_task() has already updated the
	 * utilization. We only care of the case where we start to schedule a
	 * rt task
	 */
	اگر (rq->donor->sched_class != &rt_sched_class)
		update_rt_rq_load_avg(rq_clock_pelt(rq), rq, 0);

	rt_queue_افزودن_tasks(rq);
}

ایستا ساختار sched_rt_entity *pick_next_rt_entity(ساختار rt_rq *rt_rq)
{
	ساختار rt_prio_array *array = &rt_rq->active;
	ساختار sched_rt_entity *next = پوچ;
	ساختار list_head *queue;
	صحیح idx;

	idx = sched_find_first_bit(array->bitmap);
	BUG_ON(idx >= MAX_RT_PRIO);

	queue = array->queue + idx;
	اگر (WARN_ON_ONCE(list_empty(queue)))
		بازگشت پوچ;
	next = list_entry(queue->next, ساختار sched_rt_entity, run_list);

	بازگشت next;
}

ایستا ساختار task_struct *_pick_next_task_rt(ساختار rq *rq)
{
	ساختار sched_rt_entity *rt_se;
	ساختار rt_rq *rt_rq  = &rq->rt;

	انجامبده {
		rt_se = pick_next_rt_entity(rt_rq);
		اگر (unlikely(!rt_se))
			بازگشت پوچ;
		rt_rq = group_rt_rq(rt_se);
	} وقتی (rt_rq);

	بازگشت rt_task_of(rt_se);
}

ایستا ساختار task_struct *pick_task_rt(ساختار rq *rq, ساختار rq_flags *rf)
{
	ساختار task_struct *p;

	اگر (!sched_rt_runnable(rq))
		بازگشت پوچ;

	p = _pick_next_task_rt(rq);

	بازگشت p;
}

ایستا تهی put_prev_task_rt(ساختار rq *rq, ساختار task_struct *p, ساختار task_struct *next)
{
	ساختار sched_rt_entity *rt_se = &p->rt;
	ساختار rt_rq *rt_rq = &rq->rt;

	اگر (on_rt_rq(&p->rt))
		update_stats_wait_start_rt(rt_rq, rt_se);

	update_curr_rt(rq);

	update_rt_rq_load_avg(rq_clock_pelt(rq), rq, 1);

	اگر (task_is_blocked(p))
		بازگشت;
	/*
	 * The previous task needs to be made eligible for pushing
	 * if it is still active
	 */
	اگر (on_rt_rq(&p->rt) && p->nr_cpus_allowed > 1)
		enqueue_pushable_task(rq, p);
}

/* Only try algorithms three times */
#define RT_MAX_TRIES 3

/*
 * Return the highest pushable rq's task, which is suitable to be executed
 * on the CPU, NULL otherwise
 */
ایستا ساختار task_struct *pick_highest_pushable_task(ساختار rq *rq, صحیح cpu)
{
	ساختار plist_head *head = &rq->rt.pushable_tasks;
	ساختار task_struct *p;

	اگر (!has_pushable_tasks(rq))
		بازگشت پوچ;

	plist_for_each_entry(p, head, pushable_tasks) {
		اگر (task_is_pushable(rq, p, cpu))
			بازگشت p;
	}

	بازگشت پوچ;
}

ایستا DEFINE_PER_CPU(cpumask_var_t, local_cpu_mask);

ایستا صحیح find_lowest_rq(ساختار task_struct *task)
{
	ساختار sched_domain *sd;
	ساختار cpumask *lowest_mask = this_cpu_cpumask_var_ptr(local_cpu_mask);
	صحیح this_cpu = smp_processor_id();
	صحیح cpu      = task_cpu(task);
	صحیح ret;

	/* Make sure the mask is initialized first */
	اگر (unlikely(!lowest_mask))
		بازگشت -1;

	اگر (task->nr_cpus_allowed == 1)
		بازگشت -1; /* No other targets possible */

	/*
	 * If we're on asym system ensure we consider the different capacities
	 * of the CPUs when searching for the lowest_mask.
	 */
	اگر (sched_asym_cpucap_active()) {

		ret = cpupri_find_fitness(&task_rq(task)->rd->cpupri,
					  task, lowest_mask,
					  rt_task_fits_capacity);
	} وگرنه {

		ret = cpupri_find(&task_rq(task)->rd->cpupri,
				  task, lowest_mask);
	}

	اگر (!ret)
		بازگشت -1; /* No targets found */

	/*
	 * At this point we have built a mask of CPUs representing the
	 * lowest priority tasks in the system.  Now we want to elect
	 * the best one based on our affinity and topology.
	 *
	 * We prioritize the last CPU that the task executed on since
	 * it is most likely cache-hot in that location.
	 */
	اگر (cpumask_test_cpu(cpu, lowest_mask))
		بازگشت cpu;

	/*
	 * Otherwise, we consult the sched_domains span maps to figure
	 * out which CPU is logically closest to our hot cache data.
	 */
	اگر (!cpumask_test_cpu(this_cpu, lowest_mask))
		this_cpu = -1; /* Skip this_cpu opt if not among lowest */

	rcu_read_lock();
	for_each_domain(cpu, sd) {
		اگر (sd->flags & SD_WAKE_AFFINE) {
			صحیح best_cpu;

			/*
			 * "this_cpu" is cheaper to preempt than a
			 * remote processor.
			 */
			اگر (this_cpu != -1 &&
			    cpumask_test_cpu(this_cpu, sched_domain_span(sd))) {
				rcu_read_unlock();
				بازگشت this_cpu;
			}

			best_cpu = cpumask_any_and_distribute(lowest_mask,
							      sched_domain_span(sd));
			اگر (best_cpu < nr_cpu_ids) {
				rcu_read_unlock();
				بازگشت best_cpu;
			}
		}
	}
	rcu_read_unlock();

	/*
	 * And finally, if there were no matches within the domains
	 * just give the caller *something* to work with from the compatible
	 * locations.
	 */
	اگر (this_cpu != -1)
		بازگشت this_cpu;

	cpu = cpumask_any_distribute(lowest_mask);
	اگر (cpu < nr_cpu_ids)
		بازگشت cpu;

	بازگشت -1;
}

ایستا ساختار task_struct *pick_next_pushable_task(ساختار rq *rq)
{
	ساختار plist_head *head = &rq->rt.pushable_tasks;
	ساختار task_struct *i, *p = پوچ;

	اگر (!has_pushable_tasks(rq))
		بازگشت پوچ;

	plist_for_each_entry(i, head, pushable_tasks) {
		/* skip tasks that cannot be migrated */
		اگر (!task_on_cpu(rq, i) && !is_migration_disabled(i)) {
			p = i;
			بشکن;
		}
	}

	اگر (!p)
		بازگشت پوچ;

	BUG_ON(rq->cpu != task_cpu(p));
	BUG_ON(task_current(rq, p));
	BUG_ON(task_current_donor(rq, p));
	BUG_ON(p->nr_cpus_allowed <= 1);

	BUG_ON(!task_on_rq_queued(p));
	BUG_ON(!rt_task(p));

	بازگشت p;
}

/* Will lock the rq it finds */
ایستا ساختار rq *find_lock_lowest_rq(ساختار task_struct *task, ساختار rq *rq)
{
	ساختار rq *lowest_rq = پوچ;
	صحیح tries;
	صحیح cpu;

	برای (tries = 0; tries < RT_MAX_TRIES; tries++) {
		cpu = find_lowest_rq(task);

		اگر ((cpu == -1) || (cpu == rq->cpu))
			بشکن;

		lowest_rq = cpu_rq(cpu);

		اگر (lowest_rq->rt.highest_prio.curr <= task->prio) {
			/*
			 * Target rq has tasks of equal or higher priority,
			 * retrying does not release any lock and is unlikely
			 * to yield a different result.
			 */
			lowest_rq = پوچ;
			بشکن;
		}

		/* if the prio of this runqueue changed, try again */
		اگر (double_lock_balance(rq, lowest_rq)) {
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
			اگر (unlikely(is_migration_disabled(task) ||
				     !cpumask_test_cpu(lowest_rq->cpu, &task->cpus_mask) ||
				     task != pick_next_pushable_task(rq))) {

				double_unlock_balance(rq, lowest_rq);
				lowest_rq = پوچ;
				بشکن;
			}
		}

		/* If this rq is still suitable use it. */
		اگر (lowest_rq->rt.highest_prio.curr > task->prio)
			بشکن;

		/* try again */
		double_unlock_balance(rq, lowest_rq);
		lowest_rq = پوچ;
	}

	بازگشت lowest_rq;
}

/*
 * If the current CPU has more than one RT task, see if the non
 * running task can migrate over to a CPU that is running a task
 * of lesser priority.
 */
ایستا صحیح افزودن_rt_task(ساختار rq *rq, bool pull)
{
	ساختار task_struct *next_task;
	ساختار rq *lowest_rq;
	صحیح ret = 0;

	اگر (!rq->rt.overloaded)
		بازگشت 0;

	next_task = pick_next_pushable_task(rq);
	اگر (!next_task)
		بازگشت 0;

retry:
	/*
	 * It's possible that the next_task slipped in of
	 * higher priority than current. If that's the case
	 * just reschedule current.
	 */
	اگر (unlikely(next_task->prio < rq->donor->prio)) {
		resched_curr(rq);
		بازگشت 0;
	}

	اگر (is_migration_disabled(next_task)) {
		ساختار task_struct *افزودن_task = پوچ;
		صحیح cpu;

		اگر (!pull || rq->افزودن_busy)
			بازگشت 0;

		/*
		 * Invoking find_lowest_rq() on anything but an RT task doesn't
		 * make sense. Per the above priority check, curr has to
		 * be of higher priority than next_task, so no need to
		 * reschedule when bailing out.
		 *
		 * Note that the stoppers are masqueraded as SCHED_FIFO
		 * (cf. sched_set_stop_task()), so we can't rely on rt_task().
		 */
		اگر (rq->donor->sched_class != &rt_sched_class)
			بازگشت 0;

		cpu = find_lowest_rq(rq->curr);
		اگر (cpu == -1 || cpu == rq->cpu)
			بازگشت 0;

		/*
		 * Given we found a CPU with lower priority than @next_task,
		 * therefore it should be running. However we cannot migrate it
		 * to this other CPU, instead attempt to push the current
		 * running task on this CPU away.
		 */
		افزودن_task = get_افزودن_task(rq);
		اگر (افزودن_task) {
			preempt_disable();
			raw_spin_rq_unlock(rq);
			stop_one_cpu_nowait(rq->cpu, افزودن_cpu_stop,
					    افزودن_task, &rq->افزودن_work);
			preempt_enable();
			raw_spin_rq_lock(rq);
		}

		بازگشت 0;
	}

	اگر (WARN_ON(next_task == rq->curr))
		بازگشت 0;

	/* We might release rq lock */
	get_task_struct(next_task);

	/* find_lock_lowest_rq locks the rq if found */
	lowest_rq = find_lock_lowest_rq(next_task, rq);
	اگر (!lowest_rq) {
		ساختار task_struct *task;
		/*
		 * find_lock_lowest_rq releases rq->lock
		 * so it is possible that next_task has migrated.
		 *
		 * We need to make sure that the task is still on the same
		 * run-queue and is also still the next task eligible for
		 * pushing.
		 */
		task = pick_next_pushable_task(rq);
		اگر (task == next_task) {
			/*
			 * The task hasn't migrated, and is still the next
			 * eligible task, but we failed to find a run-queue
			 * to push it to.  Do not retry in this case, since
			 * other CPUs will pull from us when ready.
			 */
			برو out;
		}

		اگر (!task)
			/* No more tasks, just exit */
			برو out;

		/*
		 * Something has shifted, try again.
		 */
		put_task_struct(next_task);
		next_task = task;
		برو retry;
	}

	move_queued_task_locked(rq, lowest_rq, next_task);
	resched_curr(lowest_rq);
	ret = 1;

	double_unlock_balance(rq, lowest_rq);
out:
	put_task_struct(next_task);

	بازگشت ret;
}

ایستا تهی افزودن_rt_tasks(ساختار rq *rq)
{
	/* push_rt_task will return true if it moved an RT */
	وقتی (افزودن_rt_task(rq, false))
		;
}

#ifdef HAVE_RT_افزودن_IPI

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
ایستا صحیح rto_next_cpu(ساختار ریشه_domain *rd)
{
	صحیح this_cpu = smp_processor_id();
	صحیح next;
	صحیح cpu;

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
	برای (;;) {

		/* When rto_cpu is -1 this acts like cpumask_first() */
		cpu = cpumask_next(rd->rto_cpu, rd->rto_mask);

		rd->rto_cpu = cpu;

		/* Do not send IPI to self */
		اگر (cpu == this_cpu)
			ادامه;

		اگر (cpu < nr_cpu_ids)
			بازگشت cpu;

		rd->rto_cpu = -1;

		/*
		 * ACQUIRE ensures we see the @rto_mask changes
		 * made prior to the @next value observed.
		 *
		 * Matches WMB in rt_set_overload().
		 */
		next = atomic_read_acquire(&rd->rto_loop_next);

		اگر (rd->rto_loop == next)
			بشکن;

		rd->rto_loop = next;
	}

	بازگشت -1;
}

ایستا درونخطی bool rto_start_trylock(atomic_t *v)
{
	بازگشت !atomic_cmpxchg_acquire(v, 0, 1);
}

ایستا درونخطی تهی rto_start_unlock(atomic_t *v)
{
	atomic_set_release(v, 0);
}

ایستا تهی tell_cpu_to_افزودن(ساختار rq *rq)
{
	صحیح cpu = -1;

	/* Keep the loop going if the IPI is currently active */
	atomic_inc(&rq->rd->rto_loop_next);

	/* Only one CPU can initiate a loop at a time */
	اگر (!rto_start_trylock(&rq->rd->rto_loop_start))
		بازگشت;

	raw_spin_lock(&rq->rd->rto_lock);

	/*
	 * The rto_cpu is updated under the lock, if it has a valid CPU
	 * then the IPI is still running and will continue due to the
	 * update to loop_next, and nothing needs to be done here.
	 * Otherwise it is finishing up and an IPI needs to be sent.
	 */
	اگر (rq->rd->rto_cpu < 0)
		cpu = rto_next_cpu(rq->rd);

	raw_spin_unlock(&rq->rd->rto_lock);

	rto_start_unlock(&rq->rd->rto_loop_start);

	اگر (cpu >= 0) {
		/* Make sure the rd does not get freed while pushing */
		sched_get_rd(rq->rd);
		irq_work_queue_on(&rq->rd->rto_افزودن_work, cpu);
	}
}

/* Called from hardirq context */
تهی rto_افزودن_irq_work_func(ساختار irq_work *work)
{
	ساختار ریشه_domain *rd =
		container_of(work, ساختار ریشه_domain, rto_افزودن_work);
	ساختار rq *rq;
	صحیح cpu;

	rq = this_rq();

	/*
	 * We do not need to grab the lock to check for has_pushable_tasks.
	 * When it gets updated, a check is made if a push is possible.
	 */
	اگر (has_pushable_tasks(rq)) {
		raw_spin_rq_lock(rq);
		وقتی (افزودن_rt_task(rq, true))
			;
		raw_spin_rq_unlock(rq);
	}

	raw_spin_lock(&rd->rto_lock);

	/* Pass the IPI to the next rt overloaded queue */
	cpu = rto_next_cpu(rd);

	raw_spin_unlock(&rd->rto_lock);

	اگر (cpu < 0) {
		sched_put_rd(rd);
		بازگشت;
	}

	/* Try the next RT overloaded CPU */
	irq_work_queue_on(&rd->rto_افزودن_work, cpu);
}
#endif /* HAVE_RT_PUSH_IPI */

ایستا تهی pull_rt_task(ساختار rq *this_rq)
{
	صحیح this_cpu = this_rq->cpu, cpu;
	bool resched = false;
	ساختار task_struct *p, *افزودن_task;
	ساختار rq *src_rq;
	صحیح rt_overload_شمار = rt_overloaded(this_rq);

	اگر (likely(!rt_overload_شمار))
		بازگشت;

	/*
	 * Match the barrier from rt_set_overloaded; this guarantees that if we
	 * see overloaded we must also see the rto_mask bit.
	 */
	smp_rmb();

	/* If we are the only overloaded CPU do nothing */
	اگر (rt_overload_شمار == 1 &&
	    cpumask_test_cpu(this_rq->cpu, this_rq->rd->rto_mask))
		بازگشت;

#ifdef HAVE_RT_افزودن_IPI
	اگر (sched_feat(RT_افزودن_IPI)) {
		tell_cpu_to_افزودن(this_rq);
		بازگشت;
	}
#endif

	for_each_cpu(cpu, this_rq->rd->rto_mask) {
		اگر (this_cpu == cpu)
			ادامه;

		src_rq = cpu_rq(cpu);

		/*
		 * Don't bother taking the src_rq->lock if the next highest
		 * task is known to be lower-priority than our current task.
		 * This may look racy, but if this value is about to go
		 * logically higher, the src_rq will push this task away.
		 * And if its going logically lower, we do not care
		 */
		اگر (src_rq->rt.highest_prio.next >=
		    this_rq->rt.highest_prio.curr)
			ادامه;

		/*
		 * We can potentially drop this_rq's lock in
		 * double_lock_balance, and another CPU could
		 * alter this_rq
		 */
		افزودن_task = پوچ;
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
		اگر (p && (p->prio < this_rq->rt.highest_prio.curr)) {
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
			اگر (p->prio < src_rq->donor->prio)
				برو skip;

			اگر (is_migration_disabled(p)) {
				افزودن_task = get_افزودن_task(src_rq);
			} وگرنه {
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

		اگر (افزودن_task) {
			preempt_disable();
			raw_spin_rq_unlock(this_rq);
			stop_one_cpu_nowait(src_rq->cpu, افزودن_cpu_stop,
					    افزودن_task, &src_rq->افزودن_work);
			preempt_enable();
			raw_spin_rq_lock(this_rq);
		}
	}

	اگر (resched)
		resched_curr(this_rq);
}

/*
 * If we are not running and we are not going to reschedule soon, we should
 * try to push tasks away now
 */
ایستا تهی task_woken_rt(ساختار rq *rq, ساختار task_struct *p)
{
	bool need_to_افزودن = !task_on_cpu(rq, p) &&
			    !test_tsk_need_resched(rq->curr) &&
			    p->nr_cpus_allowed > 1 &&
			    (dl_task(rq->donor) || rt_task(rq->donor)) &&
			    (rq->curr->nr_cpus_allowed < 2 ||
			     rq->donor->prio <= p->prio);

	اگر (need_to_افزودن)
		افزودن_rt_tasks(rq);
}

/* Assumes rq->lock is held */
ایستا تهی rq_online_rt(ساختار rq *rq)
{
	اگر (rq->rt.overloaded)
		rt_set_overload(rq);

	__enable_runtime(rq);

	cpupri_set(&rq->rd->cpupri, rq->cpu, rq->rt.highest_prio.curr);
}

/* Assumes rq->lock is held */
ایستا تهی rq_offline_rt(ساختار rq *rq)
{
	اگر (rq->rt.overloaded)
		rt_clear_overload(rq);

	__disable_runtime(rq);

	cpupri_set(&rq->rd->cpupri, rq->cpu, CPUPRI_INVALID);
}

/*
 * When switch from the rt queue, we bring ourselves to a position
 * that we might want to pull RT tasks from other runqueues.
 */
ایستا تهی switched_from_rt(ساختار rq *rq, ساختار task_struct *p)
{
	/*
	 * If there are other RT tasks then we will reschedule
	 * and the scheduling of the other RT tasks will handle
	 * the balancing. But if we are the last RT task
	 * we may need to handle the pulling of RT tasks
	 * now.
	 */
	اگر (!task_on_rq_queued(p) || rq->rt.rt_nr_running)
		بازگشت;

	rt_queue_pull_task(rq);
}

تهی __init init_sched_rt_class(تهی)
{
	unsigned صحیح i;

	for_each_possible_cpu(i) {
		zalloc_cpumask_var_گره(&per_cpu(local_cpu_mask, i),
					GFP_KERNEL, cpu_to_گره(i));
	}
}

/*
 * When switching a task to RT, we may overload the runqueue
 * with RT tasks. In this case we try to push them off to
 * other runqueues.
 */
ایستا تهی switched_to_rt(ساختار rq *rq, ساختار task_struct *p)
{
	/*
	 * If we are running, update the avg_rt tracking, as the running time
	 * will now on be accounted into the latter.
	 */
	اگر (task_current(rq, p)) {
		update_rt_rq_load_avg(rq_clock_pelt(rq), rq, 0);
		بازگشت;
	}

	/*
	 * If we are not running we may need to preempt the current
	 * running task. If that current running task is also an RT task
	 * then see if we can move to another run queue.
	 */
	اگر (task_on_rq_queued(p)) {
		اگر (p->nr_cpus_allowed > 1 && rq->rt.overloaded)
			rt_queue_افزودن_tasks(rq);
		اگر (p->prio < rq->donor->prio && cpu_online(cpu_of(rq)))
			resched_curr(rq);
	}
}

/*
 * Priority of the task has changed. This may cause
 * us to initiate a push or pull.
 */
ایستا تهی
prio_changed_rt(ساختار rq *rq, ساختار task_struct *p, u64 oldprio)
{
	اگر (!task_on_rq_queued(p))
		بازگشت;

	اگر (p->prio == oldprio)
		بازگشت;

	اگر (task_current_donor(rq, p)) {
		/*
		 * If our priority decreases while running, we
		 * may need to pull tasks to this runqueue.
		 */
		اگر (oldprio < p->prio)
			rt_queue_pull_task(rq);

		/*
		 * If there's a higher priority task waiting to run
		 * then reschedule.
		 */
		اگر (p->prio > rq->rt.highest_prio.curr)
			resched_curr(rq);
	} وگرنه {
		/*
		 * This task is not running, but if it is
		 * greater than the current running task
		 * then reschedule.
		 */
		اگر (p->prio < rq->donor->prio)
			resched_curr(rq);
	}
}

#ifdef CONFIG_POSIX_TIMERS
ایستا تهی watchdog(ساختار rq *rq, ساختار task_struct *p)
{
	unsigned long soft, hard;

	/* max may change after cur was read, this will be fixed next tick */
	soft = task_rlimit(p, RLIMIT_RTTIME);
	hard = task_rlimit_max(p, RLIMIT_RTTIME);

	اگر (soft != RLIM_INFINITY) {
		unsigned long next;

		اگر (p->rt.watchdog_stamp != jiffies) {
			p->rt.timeout++;
			p->rt.watchdog_stamp = jiffies;
		}

		next = DIV_ROUND_UP(min(soft, hard), USEC_PER_SEC/HZ);
		اگر (p->rt.timeout > next) {
			posix_cputimers_rt_watchdog(&p->posix_cputimers,
						    p->se.sum_exec_runtime);
		}
	}
}
#وگرنه /* !CONFIG_POSIX_TIMERS: */
ایستا درونخطی تهی watchdog(ساختار rq *rq, ساختار task_struct *p) { }
#endif /* !CONFIG_POSIX_TIMERS */

/*
 * scheduler tick hitting a task of our scheduling class.
 *
 * NOTE: This function can be called remotely by the tick offload that
 * goes along full dynticks. Therefore no local assumption can be made
 * and everything must be accessed through the @rq and @curr passed in
 * parameters.
 */
ایستا تهی task_tick_rt(ساختار rq *rq, ساختار task_struct *p, صحیح queued)
{
	ساختار sched_rt_entity *rt_se = &p->rt;

	update_curr_rt(rq);
	update_rt_rq_load_avg(rq_clock_pelt(rq), rq, 1);

	watchdog(rq, p);

	/*
	 * RR tasks need a special form of time-slice management.
	 * FIFO tasks have no timeslices.
	 */
	اگر (p->policy != SCHED_RR)
		بازگشت;

	اگر (--p->rt.time_slice)
		بازگشت;

	p->rt.time_slice = sched_rr_timeslice;

	/*
	 * Requeue to the end of queue if we (and all of our ancestors) are not
	 * the only element on the queue
	 */
	for_each_sched_rt_entity(rt_se) {
		اگر (rt_se->run_list.prev != rt_se->run_list.next) {
			requeue_task_rt(rq, p, 0);
			resched_curr(rq);
			بازگشت;
		}
	}
}

ایستا unsigned صحیح get_rr_interval_rt(ساختار rq *rq, ساختار task_struct *task)
{
	/*
	 * Time slice is 0 for SCHED_FIFO tasks
	 */
	اگر (task->policy == SCHED_RR)
		بازگشت sched_rr_timeslice;
	وگرنه
		بازگشت 0;
}

#ifdef CONFIG_SCHED_CORE
ایستا صحیح task_is_throttled_rt(ساختار task_struct *p, صحیح cpu)
{
	ساختار rt_rq *rt_rq;

#ifdef CONFIG_RT_GROUP_SCHED // XXX maybe add task_rt_rq(), see also sched_rt_period_rt_rq
	rt_rq = task_group(p)->rt_rq[cpu];
	WARN_ON(!rt_group_sched_enabled() && rt_rq->tg != &ریشه_task_group);
#وگرنه
	rt_rq = &cpu_rq(cpu)->rt;
#endif

	بازگشت rt_rq_throttled(rt_rq);
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
ایستا DEFINE_MUTEX(rt_constraints_mutex);

ایستا درونخطی صحیح tg_has_rt_tasks(ساختار task_group *tg)
{
	ساختار task_struct *task;
	ساختار css_task_iter it;
	صحیح ret = 0;

	/*
	 * Autogroups do not have RT tasks; see autogroup_create().
	 */
	اگر (task_group_is_autogroup(tg))
		بازگشت 0;

	css_task_iter_start(&tg->css, 0, &it);
	وقتی (!ret && (task = css_task_iter_next(&it)))
		ret |= rt_task(task);
	css_task_iter_end(&it);

	بازگشت ret;
}

ساختار rt_schedulable_data {
	ساختار task_group *tg;
	u64 rt_period;
	u64 rt_runtime;
};

ایستا صحیح tg_rt_schedulable(ساختار task_group *tg, تهی *data)
{
	ساختار rt_schedulable_data *d = data;
	ساختار task_group *child;
	u64 total, sum = 0;
	u64 period, runtime;

	period = ktime_to_ns(tg->rt_bandwidth.rt_period);
	runtime = tg->rt_bandwidth.rt_runtime;

	اگر (tg == d->tg) {
		period = d->rt_period;
		runtime = d->rt_runtime;
	}

	/*
	 * Cannot have more runtime than the period.
	 */
	اگر (runtime > period && runtime != RUNTIME_INF)
		بازگشت -EINVAL;

	/*
	 * Ensure we don't starve existing RT tasks if runtime turns zero.
	 */
	اگر (rt_bandwidth_enabled() && !runtime &&
	    tg->rt_bandwidth.rt_runtime && tg_has_rt_tasks(tg))
		بازگشت -EBUSY;

	total = to_ratio(period, runtime);

	/*
	 * Nobody can have more than the global setting allows.
	 */
	اگر (total > to_ratio(global_rt_period(), global_rt_runtime()))
		بازگشت -EINVAL;

	/*
	 * The sum of our children's runtime should not exceed our own.
	 */
	list_for_each_entry_rcu(child, &tg->children, siblings) {
		period = ktime_to_ns(child->rt_bandwidth.rt_period);
		runtime = child->rt_bandwidth.rt_runtime;

		اگر (child == d->tg) {
			period = d->rt_period;
			runtime = d->rt_runtime;
		}

		sum += to_ratio(period, runtime);
	}

	اگر (sum > total)
		بازگشت -EINVAL;

	بازگشت 0;
}

ایستا صحیح __rt_schedulable(ساختار task_group *tg, u64 period, u64 runtime)
{
	صحیح ret;

	ساختار rt_schedulable_data data = {
		.tg = tg,
		.rt_period = period,
		.rt_runtime = runtime,
	};

	rcu_read_lock();
	ret = walk_tg_درخت(tg_rt_schedulable, tg_nop, &data);
	rcu_read_unlock();

	بازگشت ret;
}

ایستا صحیح tg_set_rt_bandwidth(ساختار task_group *tg,
		u64 rt_period, u64 rt_runtime)
{
	صحیح i, err = 0;

	/*
	 * Disallowing the root group RT runtime is BAD, it would disallow the
	 * kernel creating (and or operating) RT threads.
	 */
	اگر (tg == &ریشه_task_group && rt_runtime == 0)
		بازگشت -EINVAL;

	/* No period doesn't make any sense. */
	اگر (rt_period == 0)
		بازگشت -EINVAL;

	/*
	 * Bound quota to defend quota against overflow during bandwidth shift.
	 */
	اگر (rt_runtime != RUNTIME_INF && rt_runtime > max_rt_runtime)
		بازگشت -EINVAL;

	mutex_lock(&rt_constraints_mutex);
	err = __rt_schedulable(tg, rt_period, rt_runtime);
	اگر (err)
		برو unlock;

	raw_spin_lock_irq(&tg->rt_bandwidth.rt_runtime_lock);
	tg->rt_bandwidth.rt_period = ns_to_ktime(rt_period);
	tg->rt_bandwidth.rt_runtime = rt_runtime;

	for_each_possible_cpu(i) {
		ساختار rt_rq *rt_rq = tg->rt_rq[i];

		raw_spin_lock(&rt_rq->rt_runtime_lock);
		rt_rq->rt_runtime = rt_runtime;
		raw_spin_unlock(&rt_rq->rt_runtime_lock);
	}
	raw_spin_unlock_irq(&tg->rt_bandwidth.rt_runtime_lock);
unlock:
	mutex_unlock(&rt_constraints_mutex);

	بازگشت err;
}

صحیح sched_group_set_rt_runtime(ساختار task_group *tg, long rt_runtime_us)
{
	u64 rt_runtime, rt_period;

	rt_period = ktime_to_ns(tg->rt_bandwidth.rt_period);
	rt_runtime = (u64)rt_runtime_us * NSEC_PER_USEC;
	اگر (rt_runtime_us < 0)
		rt_runtime = RUNTIME_INF;
	وگرنه اگر ((u64)rt_runtime_us > U64_MAX / NSEC_PER_USEC)
		بازگشت -EINVAL;

	بازگشت tg_set_rt_bandwidth(tg, rt_period, rt_runtime);
}

long sched_group_rt_runtime(ساختار task_group *tg)
{
	u64 rt_runtime_us;

	اگر (tg->rt_bandwidth.rt_runtime == RUNTIME_INF)
		بازگشت -1;

	rt_runtime_us = tg->rt_bandwidth.rt_runtime;
	do_div(rt_runtime_us, NSEC_PER_USEC);
	بازگشت rt_runtime_us;
}

صحیح sched_group_set_rt_period(ساختار task_group *tg, u64 rt_period_us)
{
	u64 rt_runtime, rt_period;

	اگر (rt_period_us > U64_MAX / NSEC_PER_USEC)
		بازگشت -EINVAL;

	rt_period = rt_period_us * NSEC_PER_USEC;
	rt_runtime = tg->rt_bandwidth.rt_runtime;

	بازگشت tg_set_rt_bandwidth(tg, rt_period, rt_runtime);
}

long sched_group_rt_period(ساختار task_group *tg)
{
	u64 rt_period_us;

	rt_period_us = ktime_to_ns(tg->rt_bandwidth.rt_period);
	do_div(rt_period_us, NSEC_PER_USEC);
	بازگشت rt_period_us;
}

صحیح sched_rt_can_attach(ساختار task_group *tg, ساختار task_struct *tsk)
{
	/* Don't accept real-time tasks when there is no way for them to run */
	اگر (rt_group_sched_enabled() && rt_task(tsk) && tg->rt_bandwidth.rt_runtime == 0)
		بازگشت 0;

	بازگشت 1;
}

#endif /* !CONFIG_RT_GROUP_SCHED */

#ifdef CONFIG_SYSCTL
ایستا صحیح sched_rt_global_validate(تهی)
{
	اگر ((sysctl_sched_rt_runtime != RUNTIME_INF) &&
		((sysctl_sched_rt_runtime > sysctl_sched_rt_period) ||
		 ((u64)sysctl_sched_rt_runtime *
			NSEC_PER_USEC > max_rt_runtime)))
		بازگشت -EINVAL;

#ifdef CONFIG_RT_GROUP_SCHED
	اگر (!rt_group_sched_enabled())
		بازگشت 0;

	scoped_guard(mutex, &rt_constraints_mutex)
		بازگشت __rt_schedulable(پوچ, 0, 0);
#endif
	بازگشت 0;
}

ایستا صحیح sched_rt_handler(ثابت ساختار ctl_table *table, صحیح اكتب, تهی *buffer,
		size_t *lenp, loff_t *ppos)
{
	صحیح old_period, old_runtime;
	ایستا DEFINE_MUTEX(mutex);
	صحیح ret;

	mutex_lock(&mutex);
	sched_domains_mutex_lock();
	old_period = sysctl_sched_rt_period;
	old_runtime = sysctl_sched_rt_runtime;

	ret = proc_dointvec_minmax(table, اكتب, buffer, lenp, ppos);

	اگر (!ret && اكتب) {
		ret = sched_rt_global_validate();
		اگر (ret)
			برو undo;

		ret = sched_dl_global_validate();
		اگر (ret)
			برو undo;

		sched_dl_do_global();
	}
	اگر (0) {
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

	بازگشت ret;
}

ایستا صحیح sched_rr_handler(ثابت ساختار ctl_table *table, صحیح اكتب, تهی *buffer,
		size_t *lenp, loff_t *ppos)
{
	صحیح ret;
	ایستا DEFINE_MUTEX(mutex);

	mutex_lock(&mutex);
	ret = proc_dointvec(table, اكتب, buffer, lenp, ppos);
	/*
	 * Make sure that internally we keep jiffies.
	 * Also, writing zero resets the time-slice to default:
	 */
	اگر (!ret && اكتب) {
		sched_rr_timeslice =
			sysctl_sched_rr_timeslice <= 0 ? RR_TIMESLICE :
			msecs_to_jiffies(sysctl_sched_rr_timeslice);

		اگر (sysctl_sched_rr_timeslice <= 0)
			sysctl_sched_rr_timeslice = jiffies_to_msecs(RR_TIMESLICE);
	}
	mutex_unlock(&mutex);

	بازگشت ret;
}
#endif /* CONFIG_SYSCTL */

تهی print_rt_stats(ساختار seq_file *m, صحیح cpu)
{
	rt_rq_iter_t iter;
	ساختار rt_rq *rt_rq;

	rcu_read_lock();
	for_each_rt_rq(rt_rq, iter, cpu_rq(cpu))
		print_rt_rq(m, cpu, rt_rq);
	rcu_read_unlock();
}
