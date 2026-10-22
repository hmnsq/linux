// SPDX-License-Identifier: GPL-2.0
/*
 * Real-Time Scheduling Class (mapped to the SCHED_FIFO and SCHED_RR
 * policies)
 */

#ضمن "sched.h"
#ضمن "pelt.h"

صحيح sched_rr_timeslice = RR_TIMESLICE;
/* More than 4 hours if BW_SHIFT equals 20. */
ساكن ثابت u64 max_rt_runtime = MAX_BW;

/*
 * period over which we measure -rt task CPU usage in us.
 * default: 1s
 */
صحيح sysctl_sched_rt_period = 1000000;

/*
 * part of the period that we allow rt tasks to run in us.
 * default: 1s
 */
صحيح sysctl_sched_rt_runtime = 1000000;

#ifdef CONFIG_SYSCTL
ساكن صحيح sysctl_sched_rr_timeslice = (MSEC_PER_SEC * RR_TIMESLICE) / HZ;
ساكن صحيح sched_rt_handler(ثابت بنية ctl_table *table, صحيح اكتب, فراغ *buffer,
		size_t *lenp, loff_t *ppos);
ساكن صحيح sched_rr_handler(ثابت بنية ctl_table *table, صحيح اكتب, فراغ *buffer,
		size_t *lenp, loff_t *ppos);
ساكن ثابت بنية ctl_table sched_rt_sysctls[] = {
	{
		.procname       = "sched_rt_period_us",
		.data           = &sysctl_sched_rt_period,
		.maxlen         = حجم(صحيح),
		.mode           = 0644,
		.proc_handler   = sched_rt_handler,
		.extra1         = SYSCTL_ONE,
		.extra2         = SYSCTL_INT_MAX,
	},
	{
		.procname       = "sched_rt_runtime_us",
		.data           = &sysctl_sched_rt_runtime,
		.maxlen         = حجم(صحيح),
		.mode           = 0644,
		.proc_handler   = sched_rt_handler,
		.extra1         = SYSCTL_NEG_ONE,
		.extra2         = (فراغ *)&sysctl_sched_rt_period,
	},
	{
		.procname       = "sched_rr_timeslice_ms",
		.data           = &sysctl_sched_rr_timeslice,
		.maxlen         = حجم(صحيح),
		.mode           = 0644,
		.proc_handler   = sched_rr_handler,
	},
};

ساكن صحيح __init sched_rt_sysctl_init(فراغ)
{
	register_sysctl_init("kernel", sched_rt_sysctls);
	أرجع 0;
}
late_initcall(sched_rt_sysctl_init);
#endif /* CONFIG_SYSCTL */

فراغ init_rt_rq(بنية rt_rq *rt_rq)
{
	بنية rt_prio_array *array;
	صحيح i;

	array = &rt_rq->active;
	لكل (i = 0; i < MAX_RT_PRIO; i++) {
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
	rt_rq->tg = &جذر_task_group;
#endif
}

#ifdef CONFIG_RT_GROUP_SCHED

ساكن صحيح do_sched_rt_period_timer(بنية rt_bandwidth *rt_b, صحيح overrun);

ساكن تعداد hrtimer_restart sched_rt_period_timer(بنية hrtimer *timer)
{
	بنية rt_bandwidth *rt_b =
		container_of(timer, بنية rt_bandwidth, rt_period_timer);
	صحيح idle = 0;
	صحيح overrun;

	raw_spin_lock(&rt_b->rt_runtime_lock);
	لكل (;;) {
		overrun = hrtimer_forward_now(timer, rt_b->rt_period);
		إذا (!overrun)
			اكسر;

		raw_spin_unlock(&rt_b->rt_runtime_lock);
		idle = do_sched_rt_period_timer(rt_b, overrun);
		raw_spin_lock(&rt_b->rt_runtime_lock);
	}
	إذا (idle)
		rt_b->rt_period_active = 0;
	raw_spin_unlock(&rt_b->rt_runtime_lock);

	أرجع idle ? HRTIMER_NORESTART : HRTIMER_RESTART;
}

فراغ init_rt_bandwidth(بنية rt_bandwidth *rt_b, u64 period, u64 runtime)
{
	rt_b->rt_period = ns_to_ktime(period);
	rt_b->rt_runtime = runtime;

	raw_spin_lock_init(&rt_b->rt_runtime_lock);

	hrtimer_setup(&rt_b->rt_period_timer, sched_rt_period_timer, CLOCK_MONOTONIC,
		      HRTIMER_MODE_REL_HARD);
}

ساكن مضمن فراغ do_start_rt_bandwidth(بنية rt_bandwidth *rt_b)
{
	raw_spin_lock(&rt_b->rt_runtime_lock);
	إذا (!rt_b->rt_period_active) {
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

ساكن فراغ start_rt_bandwidth(بنية rt_bandwidth *rt_b)
{
	إذا (!rt_bandwidth_enabled() || rt_b->rt_runtime == RUNTIME_INF)
		أرجع;

	do_start_rt_bandwidth(rt_b);
}

ساكن فراغ destroy_rt_bandwidth(بنية rt_bandwidth *rt_b)
{
	hrtimer_cancel(&rt_b->rt_period_timer);
}

#define rt_entity_is_task(rt_se) (!(rt_se)->my_q)

ساكن مضمن بنية task_struct *rt_task_of(بنية sched_rt_entity *rt_se)
{
	WARN_ON_ONCE(!rt_entity_is_task(rt_se));

	أرجع container_of(rt_se, بنية task_struct, rt);
}

ساكن مضمن بنية rq *rq_of_rt_rq(بنية rt_rq *rt_rq)
{
	/* Cannot fold with non-CONFIG_RT_GROUP_SCHED version, layout */
	WARN_ON(!rt_group_sched_enabled() && rt_rq->tg != &جذر_task_group);
	أرجع rt_rq->rq;
}

ساكن مضمن بنية rt_rq *rt_rq_of_se(بنية sched_rt_entity *rt_se)
{
	WARN_ON(!rt_group_sched_enabled() && rt_se->rt_rq->tg != &جذر_task_group);
	أرجع rt_se->rt_rq;
}

ساكن مضمن بنية rq *rq_of_rt_se(بنية sched_rt_entity *rt_se)
{
	بنية rt_rq *rt_rq = rt_se->rt_rq;

	WARN_ON(!rt_group_sched_enabled() && rt_rq->tg != &جذر_task_group);
	أرجع rt_rq->rq;
}

فراغ unregister_rt_sched_group(بنية task_group *tg)
{
	إذا (!rt_group_sched_enabled())
		أرجع;

	إذا (tg->rt_se)
		destroy_rt_bandwidth(&tg->rt_bandwidth);
}

فراغ free_rt_sched_group(بنية task_group *tg)
{
	صحيح i;

	إذا (!rt_group_sched_enabled())
		أرجع;

	for_each_possible_cpu(i) {
		إذا (tg->rt_rq)
			kfree(tg->rt_rq[i]);
		إذا (tg->rt_se)
			kfree(tg->rt_se[i]);
	}

	kfree(tg->rt_rq);
	kfree(tg->rt_se);
}

فراغ init_tg_rt_entry(بنية task_group *tg, بنية rt_rq *rt_rq,
		بنية sched_rt_entity *rt_se, صحيح cpu,
		بنية sched_rt_entity *parent)
{
	بنية rq *rq = cpu_rq(cpu);

	rt_rq->highest_prio.curr = MAX_RT_PRIO-1;
	rt_rq->rt_nr_boosted = 0;
	rt_rq->rq = rq;
	rt_rq->tg = tg;

	tg->rt_rq[cpu] = rt_rq;
	tg->rt_se[cpu] = rt_se;

	إذا (!rt_se)
		أرجع;

	إذا (!parent)
		rt_se->rt_rq = &rq->rt;
	وإلا
		rt_se->rt_rq = parent->my_q;

	rt_se->my_q = rt_rq;
	rt_se->parent = parent;
	INIT_LIST_HEAD(&rt_se->run_list);
}

صحيح alloc_rt_sched_group(بنية task_group *tg, بنية task_group *parent)
{
	بنية rt_rq *rt_rq;
	بنية sched_rt_entity *rt_se;
	صحيح i;

	إذا (!rt_group_sched_enabled())
		أرجع 1;

	tg->rt_rq = kzalloc_objs(rt_rq, nr_cpu_ids);
	إذا (!tg->rt_rq)
		اذهب err;
	tg->rt_se = kzalloc_objs(rt_se, nr_cpu_ids);
	إذا (!tg->rt_se)
		اذهب err;

	init_rt_bandwidth(&tg->rt_bandwidth, ktime_to_ns(global_rt_period()), 0);

	for_each_possible_cpu(i) {
		rt_rq = kzalloc_عقدة(حجم(بنية rt_rq),
				     GFP_KERNEL, cpu_to_عقدة(i));
		إذا (!rt_rq)
			اذهب err;

		rt_se = kzalloc_عقدة(حجم(بنية sched_rt_entity),
				     GFP_KERNEL, cpu_to_عقدة(i));
		إذا (!rt_se)
			اذهب err_free_rq;

		init_rt_rq(rt_rq);
		rt_rq->rt_runtime = tg->rt_bandwidth.rt_runtime;
		init_tg_rt_entry(tg, rt_rq, rt_se, i, parent->rt_se[i]);
	}

	أرجع 1;

err_free_rq:
	kfree(rt_rq);
err:
	أرجع 0;
}

#وإلا /* !CONFIG_RT_GROUP_SCHED: */

#define rt_entity_is_task(rt_se) (1)

ساكن مضمن بنية task_struct *rt_task_of(بنية sched_rt_entity *rt_se)
{
	أرجع container_of(rt_se, بنية task_struct, rt);
}

ساكن مضمن بنية rq *rq_of_rt_rq(بنية rt_rq *rt_rq)
{
	أرجع container_of(rt_rq, بنية rq, rt);
}

ساكن مضمن بنية rq *rq_of_rt_se(بنية sched_rt_entity *rt_se)
{
	بنية task_struct *p = rt_task_of(rt_se);

	أرجع task_rq(p);
}

ساكن مضمن بنية rt_rq *rt_rq_of_se(بنية sched_rt_entity *rt_se)
{
	بنية rq *rq = rq_of_rt_se(rt_se);

	أرجع &rq->rt;
}

فراغ unregister_rt_sched_group(بنية task_group *tg) { }

فراغ free_rt_sched_group(بنية task_group *tg) { }

صحيح alloc_rt_sched_group(بنية task_group *tg, بنية task_group *parent)
{
	أرجع 1;
}
#endif /* !CONFIG_RT_GROUP_SCHED */

ساكن مضمن bool need_pull_rt_task(بنية rq *rq, بنية task_struct *prev)
{
	/* Try to pull RT tasks here if we lower this rq's prio */
	أرجع rq->online && rq->rt.highest_prio.curr > prev->prio;
}

ساكن مضمن صحيح rt_overloaded(بنية rq *rq)
{
	أرجع atomic_read(&rq->rd->rto_عدد);
}

ساكن مضمن فراغ rt_set_overload(بنية rq *rq)
{
	إذا (!rq->online)
		أرجع;

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
	atomic_inc(&rq->rd->rto_عدد);
}

ساكن مضمن فراغ rt_clear_overload(بنية rq *rq)
{
	إذا (!rq->online)
		أرجع;

	/* the order here really doesn't matter */
	atomic_dec(&rq->rd->rto_عدد);
	cpumask_clear_cpu(rq->cpu, rq->rd->rto_mask);
}

ساكن مضمن صحيح has_pushable_tasks(بنية rq *rq)
{
	أرجع !plist_head_empty(&rq->rt.pushable_tasks);
}

ساكن DEFINE_PER_CPU(بنية balance_callback, rt_ادفع_head);
ساكن DEFINE_PER_CPU(بنية balance_callback, rt_pull_head);

ساكن فراغ ادفع_rt_tasks(بنية rq *);
ساكن فراغ pull_rt_task(بنية rq *);

ساكن مضمن فراغ rt_queue_ادفع_tasks(بنية rq *rq)
{
	إذا (!has_pushable_tasks(rq))
		أرجع;

	queue_balance_callback(rq, &per_cpu(rt_ادفع_head, rq->cpu), ادفع_rt_tasks);
}

ساكن مضمن فراغ rt_queue_pull_task(بنية rq *rq)
{
	queue_balance_callback(rq, &per_cpu(rt_pull_head, rq->cpu), pull_rt_task);
}

ساكن فراغ enqueue_pushable_task(بنية rq *rq, بنية task_struct *p)
{
	plist_del(&p->pushable_tasks, &rq->rt.pushable_tasks);
	plist_عقدة_init(&p->pushable_tasks, p->prio);
	plist_add(&p->pushable_tasks, &rq->rt.pushable_tasks);

	/* Update the highest prio pushable task */
	إذا (p->prio < rq->rt.highest_prio.next)
		rq->rt.highest_prio.next = p->prio;

	إذا (!rq->rt.overloaded) {
		rt_set_overload(rq);
		rq->rt.overloaded = 1;
	}
}

ساكن فراغ dequeue_pushable_task(بنية rq *rq, بنية task_struct *p)
{
	plist_del(&p->pushable_tasks, &rq->rt.pushable_tasks);

	/* Update the new highest prio pushable task */
	إذا (has_pushable_tasks(rq)) {
		p = plist_first_entry(&rq->rt.pushable_tasks,
				      بنية task_struct, pushable_tasks);
		rq->rt.highest_prio.next = p->prio;
	} وإلا {
		rq->rt.highest_prio.next = MAX_RT_PRIO-1;

		إذا (rq->rt.overloaded) {
			rt_clear_overload(rq);
			rq->rt.overloaded = 0;
		}
	}
}

ساكن فراغ enqueue_قمة_rt_rq(بنية rt_rq *rt_rq);
ساكن فراغ dequeue_قمة_rt_rq(بنية rt_rq *rt_rq, unsigned صحيح عدد);

ساكن مضمن صحيح on_rt_rq(بنية sched_rt_entity *rt_se)
{
	أرجع rt_se->on_rq;
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
ساكن مضمن bool rt_task_fits_capacity(بنية task_struct *p, صحيح cpu)
{
	unsigned صحيح min_cap;
	unsigned صحيح max_cap;
	unsigned صحيح cpu_cap;

	/* Only heterogeneous systems can benefit from this check */
	إذا (!sched_asym_cpucap_active())
		أرجع true;

	min_cap = uclamp_eff_قيمة(p, UCLAMP_MIN);
	max_cap = uclamp_eff_قيمة(p, UCLAMP_MAX);

	cpu_cap = arch_scale_cpu_capacity(cpu);

	أرجع cpu_cap >= min(min_cap, max_cap);
}
#وإلا /* !CONFIG_UCLAMP_TASK: */
ساكن مضمن bool rt_task_fits_capacity(بنية task_struct *p, صحيح cpu)
{
	أرجع true;
}
#endif /* !CONFIG_UCLAMP_TASK */

#ifdef CONFIG_RT_GROUP_SCHED

ساكن مضمن u64 sched_rt_runtime(بنية rt_rq *rt_rq)
{
	أرجع rt_rq->rt_runtime;
}

ساكن مضمن u64 sched_rt_period(بنية rt_rq *rt_rq)
{
	أرجع ktime_to_ns(rt_rq->tg->rt_bandwidth.rt_period);
}

تعريفنوع بنية task_group *rt_rq_iter_t;

ساكن مضمن بنية task_group *next_task_group(بنية task_group *tg)
{
	إذا (!rt_group_sched_enabled()) {
		WARN_ON(tg != &جذر_task_group);
		أرجع عدم;
	}

	افعل {
		tg = list_entry_rcu(tg->list.next,
			typeof(بنية task_group), list);
	} طالما (&tg->list != &task_groups && task_group_is_autogroup(tg));

	إذا (&tg->list == &task_groups)
		tg = عدم;

	أرجع tg;
}

#define for_each_rt_rq(rt_rq, iter, rq)					\
	لكل (iter = &جذر_task_group;					\
		iter && (rt_rq = iter->rt_rq[cpu_of(rq)]);		\
		iter = next_task_group(iter))

#define for_each_sched_rt_entity(rt_se) \
	لكل (; rt_se; rt_se = rt_se->parent)

ساكن مضمن بنية rt_rq *group_rt_rq(بنية sched_rt_entity *rt_se)
{
	أرجع rt_se->my_q;
}

ساكن فراغ enqueue_rt_entity(بنية sched_rt_entity *rt_se, unsigned صحيح flags);
ساكن فراغ dequeue_rt_entity(بنية sched_rt_entity *rt_se, unsigned صحيح flags);

ساكن فراغ sched_rt_rq_enqueue(بنية rt_rq *rt_rq)
{
	بنية task_struct *donor = rq_of_rt_rq(rt_rq)->donor;
	بنية rq *rq = rq_of_rt_rq(rt_rq);
	بنية sched_rt_entity *rt_se;

	صحيح cpu = cpu_of(rq);

	rt_se = rt_rq->tg->rt_se[cpu];

	إذا (rt_rq->rt_nr_running) {
		إذا (!rt_se)
			enqueue_قمة_rt_rq(rt_rq);
		وإلا إذا (!on_rt_rq(rt_se))
			enqueue_rt_entity(rt_se, 0);

		إذا (rt_rq->highest_prio.curr < donor->prio)
			resched_curr(rq);
	}
}

ساكن فراغ sched_rt_rq_dequeue(بنية rt_rq *rt_rq)
{
	بنية sched_rt_entity *rt_se;
	صحيح cpu = cpu_of(rq_of_rt_rq(rt_rq));

	rt_se = rt_rq->tg->rt_se[cpu];

	إذا (!rt_se) {
		dequeue_قمة_rt_rq(rt_rq, rt_rq->rt_nr_running);
		/* Kick cpufreq (see the comment in kernel/sched/sched.h). */
		cpufreq_update_util(rq_of_rt_rq(rt_rq), 0);
	}
	وإلا إذا (on_rt_rq(rt_se))
		dequeue_rt_entity(rt_se, 0);
}

ساكن مضمن صحيح rt_rq_throttled(بنية rt_rq *rt_rq)
{
	أرجع rt_rq->rt_throttled && !rt_rq->rt_nr_boosted;
}

ساكن صحيح rt_se_boosted(بنية sched_rt_entity *rt_se)
{
	بنية rt_rq *rt_rq = group_rt_rq(rt_se);
	بنية task_struct *p;

	إذا (rt_rq)
		أرجع !!rt_rq->rt_nr_boosted;

	p = rt_task_of(rt_se);
	أرجع p->prio != p->normal_prio;
}

ساكن مضمن ثابت بنية cpumask *sched_rt_period_mask(فراغ)
{
	أرجع this_rq()->rd->span;
}

ساكن مضمن
بنية rt_rq *sched_rt_period_rt_rq(بنية rt_bandwidth *rt_b, صحيح cpu)
{
	أرجع container_of(rt_b, بنية task_group, rt_bandwidth)->rt_rq[cpu];
}

ساكن مضمن بنية rt_bandwidth *sched_rt_bandwidth(بنية rt_rq *rt_rq)
{
	أرجع &rt_rq->tg->rt_bandwidth;
}

bool sched_rt_bandwidth_account(بنية rt_rq *rt_rq)
{
	بنية rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);

	أرجع (hrtimer_active(&rt_b->rt_period_timer) ||
		rt_rq->rt_time < rt_b->rt_runtime);
}

/*
 * We ran out of runtime, see if we can borrow some from our neighbours.
 */
ساكن فراغ do_balance_runtime(بنية rt_rq *rt_rq)
{
	بنية rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);
	بنية جذر_domain *rd = rq_of_rt_rq(rt_rq)->rd;
	صحيح i, weight;
	u64 rt_period;

	weight = cpumask_weight(rd->span);

	raw_spin_lock(&rt_b->rt_runtime_lock);
	rt_period = ktime_to_ns(rt_b->rt_period);
	for_each_cpu(i, rd->span) {
		بنية rt_rq *iter = sched_rt_period_rt_rq(rt_b, i);
		s64 diff;

		إذا (iter == rt_rq)
			استمر;

		raw_spin_lock(&iter->rt_runtime_lock);
		/*
		 * Either all rqs have inf runtime and there's nothing to steal
		 * or __disable_runtime() below sets a specific rq to inf to
		 * indicate its been disabled and disallow stealing.
		 */
		إذا (iter->rt_runtime == RUNTIME_INF)
			اذهب next;

		/*
		 * From runqueues with spare time, take 1/n part of their
		 * spare time, but no more than our period.
		 */
		diff = iter->rt_runtime - iter->rt_time;
		إذا (diff > 0) {
			diff = div_u64((u64)diff, weight);
			إذا (rt_rq->rt_runtime + diff > rt_period)
				diff = rt_period - rt_rq->rt_runtime;
			iter->rt_runtime -= diff;
			rt_rq->rt_runtime += diff;
			إذا (rt_rq->rt_runtime == rt_period) {
				raw_spin_unlock(&iter->rt_runtime_lock);
				اكسر;
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
ساكن فراغ __disable_runtime(بنية rq *rq)
{
	بنية جذر_domain *rd = rq->rd;
	rt_rq_iter_t iter;
	بنية rt_rq *rt_rq;

	إذا (unlikely(!scheduler_running))
		أرجع;

	for_each_rt_rq(rt_rq, iter, rq) {
		بنية rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);
		s64 want;
		صحيح i;

		raw_spin_lock(&rt_b->rt_runtime_lock);
		raw_spin_lock(&rt_rq->rt_runtime_lock);
		/*
		 * Either we're all inf and nobody needs to borrow, or we're
		 * already disabled and thus have nothing to do, or we have
		 * exactly the right amount of runtime to take out.
		 */
		إذا (rt_rq->rt_runtime == RUNTIME_INF ||
				rt_rq->rt_runtime == rt_b->rt_runtime)
			اذهب balanced;
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
			بنية rt_rq *iter = sched_rt_period_rt_rq(rt_b, i);
			s64 diff;

			/*
			 * Can't reclaim from ourselves or disabled runqueues.
			 */
			إذا (iter == rt_rq || iter->rt_runtime == RUNTIME_INF)
				استمر;

			raw_spin_lock(&iter->rt_runtime_lock);
			إذا (want > 0) {
				diff = min_t(s64, iter->rt_runtime, want);
				iter->rt_runtime -= diff;
				want -= diff;
			} وإلا {
				iter->rt_runtime -= want;
				want -= want;
			}
			raw_spin_unlock(&iter->rt_runtime_lock);

			إذا (!want)
				اكسر;
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

ساكن فراغ __enable_runtime(بنية rq *rq)
{
	rt_rq_iter_t iter;
	بنية rt_rq *rt_rq;

	إذا (unlikely(!scheduler_running))
		أرجع;

	/*
	 * Reset each runqueue's bandwidth settings
	 */
	for_each_rt_rq(rt_rq, iter, rq) {
		بنية rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);

		raw_spin_lock(&rt_b->rt_runtime_lock);
		raw_spin_lock(&rt_rq->rt_runtime_lock);
		rt_rq->rt_runtime = rt_b->rt_runtime;
		rt_rq->rt_time = 0;
		rt_rq->rt_throttled = 0;
		raw_spin_unlock(&rt_rq->rt_runtime_lock);
		raw_spin_unlock(&rt_b->rt_runtime_lock);
	}
}

ساكن فراغ balance_runtime(بنية rt_rq *rt_rq)
{
	إذا (!sched_feat(RT_RUNTIME_SHARE))
		أرجع;

	إذا (rt_rq->rt_time > rt_rq->rt_runtime) {
		raw_spin_unlock(&rt_rq->rt_runtime_lock);
		do_balance_runtime(rt_rq);
		raw_spin_lock(&rt_rq->rt_runtime_lock);
	}
}

ساكن صحيح do_sched_rt_period_timer(بنية rt_bandwidth *rt_b, صحيح overrun)
{
	صحيح i, idle = 1, throttled = 0;
	ثابت بنية cpumask *span;

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
	إذا (rt_b == &جذر_task_group.rt_bandwidth)
		span = cpu_online_mask;

	for_each_cpu(i, span) {
		صحيح enqueue = 0;
		بنية rt_rq *rt_rq = sched_rt_period_rt_rq(rt_b, i);
		بنية rq *rq = rq_of_rt_rq(rt_rq);
		بنية rq_flags rf;
		صحيح skip;

		/*
		 * When span == cpu_online_mask, taking each rq->lock
		 * can be time-consuming. Try to avoid it when possible.
		 */
		raw_spin_lock(&rt_rq->rt_runtime_lock);
		إذا (!sched_feat(RT_RUNTIME_SHARE) && rt_rq->rt_runtime != RUNTIME_INF)
			rt_rq->rt_runtime = rt_b->rt_runtime;
		skip = !rt_rq->rt_time && !rt_rq->rt_nr_running;
		raw_spin_unlock(&rt_rq->rt_runtime_lock);
		إذا (skip)
			استمر;

		rq_lock(rq, &rf);
		update_rq_clock(rq);

		إذا (rt_rq->rt_time) {
			u64 runtime;

			raw_spin_lock(&rt_rq->rt_runtime_lock);
			إذا (rt_rq->rt_throttled)
				balance_runtime(rt_rq);
			runtime = rt_rq->rt_runtime;
			rt_rq->rt_time -= min(rt_rq->rt_time, overrun*runtime);
			إذا (rt_rq->rt_throttled && rt_rq->rt_time < runtime) {
				rt_rq->rt_throttled = 0;
				enqueue = 1;

				/*
				 * When we're idle and a woken (rt) task is
				 * throttled wakeup_preempt() will set
				 * skip_update and the time between the wakeup
				 * and this unthrottle will get accounted as
				 * 'runtime'.
				 */
				إذا (rt_rq->rt_nr_running && rq->curr == rq->idle)
					rq_clock_cancel_skipupdate(rq);
			}
			إذا (rt_rq->rt_time || rt_rq->rt_nr_running)
				idle = 0;
			raw_spin_unlock(&rt_rq->rt_runtime_lock);
		} وإلا إذا (rt_rq->rt_nr_running) {
			idle = 0;
			إذا (!rt_rq_throttled(rt_rq))
				enqueue = 1;
		}
		إذا (rt_rq->rt_throttled)
			throttled = 1;

		إذا (enqueue)
			sched_rt_rq_enqueue(rt_rq);
		rq_unlock(rq, &rf);
	}

	إذا (!throttled && (!rt_bandwidth_enabled() || rt_b->rt_runtime == RUNTIME_INF))
		أرجع 1;

	أرجع idle;
}

ساكن صحيح sched_rt_runtime_exceeded(بنية rt_rq *rt_rq)
{
	u64 runtime = sched_rt_runtime(rt_rq);

	إذا (rt_rq->rt_throttled)
		أرجع rt_rq_throttled(rt_rq);

	إذا (runtime >= sched_rt_period(rt_rq))
		أرجع 0;

	balance_runtime(rt_rq);
	runtime = sched_rt_runtime(rt_rq);
	إذا (runtime == RUNTIME_INF)
		أرجع 0;

	إذا (rt_rq->rt_time > runtime) {
		بنية rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);

		/*
		 * Don't actually throttle groups that have no runtime assigned
		 * but accrue some time due to boosting.
		 */
		إذا (likely(rt_b->rt_runtime)) {
			rt_rq->rt_throttled = 1;
			printk_deferred_once("sched: RT throttling activated\n");
		} وإلا {
			/*
			 * In case we did anyway, make it go away,
			 * replenishment is a joke, since it will replenish us
			 * with exactly 0 ns.
			 */
			rt_rq->rt_time = 0;
		}

		إذا (rt_rq_throttled(rt_rq)) {
			sched_rt_rq_dequeue(rt_rq);
			أرجع 1;
		}
	}

	أرجع 0;
}

#وإلا /* !CONFIG_RT_GROUP_SCHED: */

تعريفنوع بنية rt_rq *rt_rq_iter_t;

#define for_each_rt_rq(rt_rq, iter, rq) \
	لكل ((فراغ) iter, rt_rq = &rq->rt; rt_rq; rt_rq = عدم)

#define for_each_sched_rt_entity(rt_se) \
	لكل (; rt_se; rt_se = عدم)

ساكن مضمن بنية rt_rq *group_rt_rq(بنية sched_rt_entity *rt_se)
{
	أرجع عدم;
}

ساكن مضمن فراغ sched_rt_rq_enqueue(بنية rt_rq *rt_rq)
{
	بنية rq *rq = rq_of_rt_rq(rt_rq);

	إذا (!rt_rq->rt_nr_running)
		أرجع;

	enqueue_قمة_rt_rq(rt_rq);
	resched_curr(rq);
}

ساكن مضمن فراغ sched_rt_rq_dequeue(بنية rt_rq *rt_rq)
{
	dequeue_قمة_rt_rq(rt_rq, rt_rq->rt_nr_running);
}

ساكن مضمن صحيح rt_rq_throttled(بنية rt_rq *rt_rq)
{
	أرجع false;
}

ساكن مضمن ثابت بنية cpumask *sched_rt_period_mask(فراغ)
{
	أرجع cpu_online_mask;
}

ساكن مضمن
بنية rt_rq *sched_rt_period_rt_rq(بنية rt_bandwidth *rt_b, صحيح cpu)
{
	أرجع &cpu_rq(cpu)->rt;
}

ساكن فراغ __enable_runtime(بنية rq *rq) { }
ساكن فراغ __disable_runtime(بنية rq *rq) { }

#endif /* !CONFIG_RT_GROUP_SCHED */

ساكن مضمن صحيح rt_se_prio(بنية sched_rt_entity *rt_se)
{
#ifdef CONFIG_RT_GROUP_SCHED
	بنية rt_rq *rt_rq = group_rt_rq(rt_se);

	إذا (rt_rq)
		أرجع rt_rq->highest_prio.curr;
#endif

	أرجع rt_task_of(rt_se)->prio;
}

/*
 * Update the current task's runtime statistics. Skip current tasks that
 * are not in our scheduling class.
 */
ساكن فراغ update_curr_rt(بنية rq *rq)
{
	بنية task_struct *donor = rq->donor;
	s64 delta_exec;

	إذا (donor->sched_class != &rt_sched_class)
		أرجع;

	delta_exec = update_curr_common(rq);
	إذا (unlikely(delta_exec <= 0))
		أرجع;

#ifdef CONFIG_RT_GROUP_SCHED
	بنية sched_rt_entity *rt_se = &donor->rt;

	إذا (!rt_bandwidth_enabled())
		أرجع;

	for_each_sched_rt_entity(rt_se) {
		بنية rt_rq *rt_rq = rt_rq_of_se(rt_se);
		صحيح exceeded;

		إذا (sched_rt_runtime(rt_rq) != RUNTIME_INF) {
			raw_spin_lock(&rt_rq->rt_runtime_lock);
			rt_rq->rt_time += delta_exec;
			exceeded = sched_rt_runtime_exceeded(rt_rq);
			إذا (exceeded)
				resched_curr(rq);
			raw_spin_unlock(&rt_rq->rt_runtime_lock);
			إذا (exceeded)
				do_start_rt_bandwidth(sched_rt_bandwidth(rt_rq));
		}
	}
#endif /* CONFIG_RT_GROUP_SCHED */
}

ساكن فراغ
dequeue_قمة_rt_rq(بنية rt_rq *rt_rq, unsigned صحيح عدد)
{
	بنية rq *rq = rq_of_rt_rq(rt_rq);

	BUG_ON(&rq->rt != rt_rq);

	إذا (!rt_rq->rt_queued)
		أرجع;

	BUG_ON(!rq->nr_running);

	sub_nr_running(rq, عدد);
	rt_rq->rt_queued = 0;

}

ساكن فراغ
enqueue_قمة_rt_rq(بنية rt_rq *rt_rq)
{
	بنية rq *rq = rq_of_rt_rq(rt_rq);

	BUG_ON(&rq->rt != rt_rq);

	إذا (rt_rq->rt_queued)
		أرجع;

	إذا (rt_rq_throttled(rt_rq))
		أرجع;

	إذا (rt_rq->rt_nr_running) {
		add_nr_running(rq, rt_rq->rt_nr_running);
		rt_rq->rt_queued = 1;
	}

	/* Kick cpufreq (see the comment in kernel/sched/sched.h). */
	cpufreq_update_util(rq, 0);
}

ساكن فراغ
inc_rt_prio_smp(بنية rt_rq *rt_rq, صحيح prio, صحيح prev_prio)
{
	بنية rq *rq = rq_of_rt_rq(rt_rq);

	/*
	 * Change rq's cpupri only if rt_rq is the top queue.
	 */
	إذا (IS_ENABLED(CONFIG_RT_GROUP_SCHED) && &rq->rt != rt_rq)
		أرجع;

	إذا (rq->online && prio < prev_prio)
		cpupri_set(&rq->rd->cpupri, rq->cpu, prio);
}

ساكن فراغ
dec_rt_prio_smp(بنية rt_rq *rt_rq, صحيح prio, صحيح prev_prio)
{
	بنية rq *rq = rq_of_rt_rq(rt_rq);

	/*
	 * Change rq's cpupri only if rt_rq is the top queue.
	 */
	إذا (IS_ENABLED(CONFIG_RT_GROUP_SCHED) && &rq->rt != rt_rq)
		أرجع;

	إذا (rq->online && rt_rq->highest_prio.curr != prev_prio)
		cpupri_set(&rq->rd->cpupri, rq->cpu, rt_rq->highest_prio.curr);
}

ساكن فراغ
inc_rt_prio(بنية rt_rq *rt_rq, صحيح prio)
{
	صحيح prev_prio = rt_rq->highest_prio.curr;

	إذا (prio < prev_prio)
		rt_rq->highest_prio.curr = prio;

	inc_rt_prio_smp(rt_rq, prio, prev_prio);
}

ساكن فراغ
dec_rt_prio(بنية rt_rq *rt_rq, صحيح prio)
{
	صحيح prev_prio = rt_rq->highest_prio.curr;

	إذا (rt_rq->rt_nr_running) {

		WARN_ON(prio < prev_prio);

		/*
		 * This may have been our highest task, and therefore
		 * we may have some re-computation to do
		 */
		إذا (prio == prev_prio) {
			بنية rt_prio_array *array = &rt_rq->active;

			rt_rq->highest_prio.curr =
				sched_find_first_bit(array->bitmap);
		}

	} وإلا {
		rt_rq->highest_prio.curr = MAX_RT_PRIO-1;
	}

	dec_rt_prio_smp(rt_rq, prio, prev_prio);
}

#ifdef CONFIG_RT_GROUP_SCHED

ساكن فراغ
inc_rt_group(بنية sched_rt_entity *rt_se, بنية rt_rq *rt_rq)
{
	إذا (rt_se_boosted(rt_se))
		rt_rq->rt_nr_boosted++;

	start_rt_bandwidth(&rt_rq->tg->rt_bandwidth);
}

ساكن فراغ
dec_rt_group(بنية sched_rt_entity *rt_se, بنية rt_rq *rt_rq)
{
	إذا (rt_se_boosted(rt_se))
		rt_rq->rt_nr_boosted--;

	WARN_ON(!rt_rq->rt_nr_running && rt_rq->rt_nr_boosted);
}

#وإلا /* !CONFIG_RT_GROUP_SCHED: */

ساكن فراغ
inc_rt_group(بنية sched_rt_entity *rt_se, بنية rt_rq *rt_rq)
{
}

ساكن مضمن
فراغ dec_rt_group(بنية sched_rt_entity *rt_se, بنية rt_rq *rt_rq) {}

#endif /* !CONFIG_RT_GROUP_SCHED */

ساكن مضمن
unsigned صحيح rt_se_nr_running(بنية sched_rt_entity *rt_se)
{
	بنية rt_rq *group_rq = group_rt_rq(rt_se);

	إذا (group_rq)
		أرجع group_rq->rt_nr_running;
	وإلا
		أرجع 1;
}

ساكن مضمن
unsigned صحيح rt_se_rr_nr_running(بنية sched_rt_entity *rt_se)
{
	بنية rt_rq *group_rq = group_rt_rq(rt_se);
	بنية task_struct *tsk;

	إذا (group_rq)
		أرجع group_rq->rr_nr_running;

	tsk = rt_task_of(rt_se);

	أرجع (tsk->policy == SCHED_RR) ? 1 : 0;
}

ساكن مضمن
فراغ inc_rt_tasks(بنية sched_rt_entity *rt_se, بنية rt_rq *rt_rq)
{
	صحيح prio = rt_se_prio(rt_se);

	WARN_ON(!rt_prio(prio));
	rt_rq->rt_nr_running += rt_se_nr_running(rt_se);
	rt_rq->rr_nr_running += rt_se_rr_nr_running(rt_se);

	inc_rt_prio(rt_rq, prio);
	inc_rt_group(rt_se, rt_rq);
}

ساكن مضمن
فراغ dec_rt_tasks(بنية sched_rt_entity *rt_se, بنية rt_rq *rt_rq)
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
ساكن مضمن bool move_entity(unsigned صحيح flags)
{
	إذا ((flags & (DEQUEUE_SAVE | DEQUEUE_MOVE)) == DEQUEUE_SAVE)
		أرجع false;

	أرجع true;
}

ساكن فراغ __delist_rt_entity(بنية sched_rt_entity *rt_se, بنية rt_prio_array *array)
{
	list_del_init(&rt_se->run_list);

	إذا (list_empty(array->queue + rt_se_prio(rt_se)))
		__clear_bit(rt_se_prio(rt_se), array->bitmap);

	rt_se->on_list = 0;
}

ساكن مضمن بنية sched_statistics *
__schedstats_from_rt_se(بنية sched_rt_entity *rt_se)
{
	/* schedstats is not supported for rt group. */
	إذا (!rt_entity_is_task(rt_se))
		أرجع عدم;

	أرجع &rt_task_of(rt_se)->stats;
}

ساكن مضمن فراغ
update_stats_wait_start_rt(بنية rt_rq *rt_rq, بنية sched_rt_entity *rt_se)
{
	بنية sched_statistics *stats;
	بنية task_struct *p = عدم;

	إذا (!schedstat_enabled())
		أرجع;

	إذا (rt_entity_is_task(rt_se))
		p = rt_task_of(rt_se);

	stats = __schedstats_from_rt_se(rt_se);
	إذا (!stats)
		أرجع;

	__update_stats_wait_start(rq_of_rt_rq(rt_rq), p, stats);
}

ساكن مضمن فراغ
update_stats_enqueue_sleeper_rt(بنية rt_rq *rt_rq, بنية sched_rt_entity *rt_se)
{
	بنية sched_statistics *stats;
	بنية task_struct *p = عدم;

	إذا (!schedstat_enabled())
		أرجع;

	إذا (rt_entity_is_task(rt_se))
		p = rt_task_of(rt_se);

	stats = __schedstats_from_rt_se(rt_se);
	إذا (!stats)
		أرجع;

	__update_stats_enqueue_sleeper(rq_of_rt_rq(rt_rq), p, stats);
}

ساكن مضمن فراغ
update_stats_enqueue_rt(بنية rt_rq *rt_rq, بنية sched_rt_entity *rt_se,
			صحيح flags)
{
	إذا (!schedstat_enabled())
		أرجع;

	إذا (flags & ENQUEUE_WAKEUP)
		update_stats_enqueue_sleeper_rt(rt_rq, rt_se);
}

ساكن مضمن فراغ
update_stats_wait_end_rt(بنية rt_rq *rt_rq, بنية sched_rt_entity *rt_se)
{
	بنية sched_statistics *stats;
	بنية task_struct *p = عدم;

	إذا (!schedstat_enabled())
		أرجع;

	إذا (rt_entity_is_task(rt_se))
		p = rt_task_of(rt_se);

	stats = __schedstats_from_rt_se(rt_se);
	إذا (!stats)
		أرجع;

	__update_stats_wait_end(rq_of_rt_rq(rt_rq), p, stats);
}

ساكن مضمن فراغ
update_stats_dequeue_rt(بنية rt_rq *rt_rq, بنية sched_rt_entity *rt_se,
			صحيح flags)
{
	بنية task_struct *p = عدم;
	بنية rq *rq = rq_of_rt_rq(rt_rq);

	إذا (!schedstat_enabled())
		أرجع;

	إذا (rt_entity_is_task(rt_se)) {
		p = rt_task_of(rt_se);

		إذا (p != rq->curr)
			update_stats_wait_end_rt(rt_rq, rt_se);
	}

	إذا ((flags & DEQUEUE_SLEEP) && p) {
		unsigned صحيح state;

		state = READ_ONCE(p->__state);
		إذا (state & TASK_INTERRUPTIBLE)
			__schedstat_set(p->stats.sleep_start,
					rq_clock(rq_of_rt_rq(rt_rq)));

		إذا (state & TASK_UNINTERRUPTIBLE)
			__schedstat_set(p->stats.block_start,
					rq_clock(rq_of_rt_rq(rt_rq)));
	}
}

ساكن فراغ __enqueue_rt_entity(بنية sched_rt_entity *rt_se, unsigned صحيح flags)
{
	بنية rt_rq *rt_rq = rt_rq_of_se(rt_se);
	بنية rt_prio_array *array = &rt_rq->active;
	بنية rt_rq *group_rq = group_rt_rq(rt_se);
	بنية list_head *queue = array->queue + rt_se_prio(rt_se);

	/*
	 * Don't enqueue the group if its throttled, or when empty.
	 * The latter is a consequence of the former when a child group
	 * get throttled and the current group doesn't have any other
	 * active members.
	 */
	إذا (group_rq && (rt_rq_throttled(group_rq) || !group_rq->rt_nr_running)) {
		إذا (rt_se->on_list)
			__delist_rt_entity(rt_se, array);
		أرجع;
	}

	إذا (move_entity(flags)) {
		WARN_ON_ONCE(rt_se->on_list);
		إذا (flags & ENQUEUE_HEAD)
			list_add(&rt_se->run_list, queue);
		وإلا
			list_add_tail(&rt_se->run_list, queue);

		__set_bit(rt_se_prio(rt_se), array->bitmap);
		rt_se->on_list = 1;
	}
	rt_se->on_rq = 1;

	inc_rt_tasks(rt_se, rt_rq);
}

ساكن فراغ __dequeue_rt_entity(بنية sched_rt_entity *rt_se, unsigned صحيح flags)
{
	بنية rt_rq *rt_rq = rt_rq_of_se(rt_se);
	بنية rt_prio_array *array = &rt_rq->active;

	إذا (move_entity(flags)) {
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
ساكن فراغ dequeue_rt_مكدس(بنية sched_rt_entity *rt_se, unsigned صحيح flags)
{
	بنية sched_rt_entity *back = عدم;
	unsigned صحيح rt_nr_running;

	for_each_sched_rt_entity(rt_se) {
		rt_se->back = back;
		back = rt_se;
	}

	rt_nr_running = rt_rq_of_se(back)->rt_nr_running;

	لكل (rt_se = back; rt_se; rt_se = rt_se->back) {
		إذا (on_rt_rq(rt_se))
			__dequeue_rt_entity(rt_se, flags);
	}

	dequeue_قمة_rt_rq(rt_rq_of_se(back), rt_nr_running);
}

ساكن فراغ enqueue_rt_entity(بنية sched_rt_entity *rt_se, unsigned صحيح flags)
{
	بنية rq *rq = rq_of_rt_se(rt_se);

	update_stats_enqueue_rt(rt_rq_of_se(rt_se), rt_se, flags);

	dequeue_rt_مكدس(rt_se, flags);
	for_each_sched_rt_entity(rt_se)
		__enqueue_rt_entity(rt_se, flags);
	enqueue_قمة_rt_rq(&rq->rt);
}

ساكن فراغ dequeue_rt_entity(بنية sched_rt_entity *rt_se, unsigned صحيح flags)
{
	بنية rq *rq = rq_of_rt_se(rt_se);

	update_stats_dequeue_rt(rt_rq_of_se(rt_se), rt_se, flags);

	dequeue_rt_مكدس(rt_se, flags);

	for_each_sched_rt_entity(rt_se) {
		بنية rt_rq *rt_rq = group_rt_rq(rt_se);

		إذا (rt_rq && rt_rq->rt_nr_running)
			__enqueue_rt_entity(rt_se, flags);
	}
	enqueue_قمة_rt_rq(&rq->rt);
}

/*
 * Adding/removing a task to/from a priority array:
 */
ساكن فراغ
enqueue_task_rt(بنية rq *rq, بنية task_struct *p, صحيح flags)
{
	بنية sched_rt_entity *rt_se = &p->rt;

	إذا (flags & ENQUEUE_WAKEUP)
		rt_se->timeout = 0;

	check_schedstat_required();
	update_stats_wait_start_rt(rt_rq_of_se(rt_se), rt_se);

	enqueue_rt_entity(rt_se, flags);

	إذا (task_is_blocked(p))
		أرجع;

	إذا (!task_current(rq, p) && p->nr_cpus_allowed > 1)
		enqueue_pushable_task(rq, p);
}

ساكن bool dequeue_task_rt(بنية rq *rq, بنية task_struct *p, صحيح flags)
{
	بنية sched_rt_entity *rt_se = &p->rt;

	update_curr_rt(rq);
	dequeue_rt_entity(rt_se, flags);

	dequeue_pushable_task(rq, p);

	أرجع true;
}

/*
 * Put task to the head or the end of the run list without the overhead of
 * dequeue followed by enqueue.
 */
ساكن فراغ
requeue_rt_entity(بنية rt_rq *rt_rq, بنية sched_rt_entity *rt_se, صحيح head)
{
	إذا (on_rt_rq(rt_se)) {
		بنية rt_prio_array *array = &rt_rq->active;
		بنية list_head *queue = array->queue + rt_se_prio(rt_se);

		إذا (head)
			list_move(&rt_se->run_list, queue);
		وإلا
			list_move_tail(&rt_se->run_list, queue);
	}
}

ساكن فراغ requeue_task_rt(بنية rq *rq, بنية task_struct *p, صحيح head)
{
	بنية sched_rt_entity *rt_se = &p->rt;
	بنية rt_rq *rt_rq;

	for_each_sched_rt_entity(rt_se) {
		rt_rq = rt_rq_of_se(rt_se);
		requeue_rt_entity(rt_rq, rt_se, head);
	}
}

ساكن فراغ yield_task_rt(بنية rq *rq)
{
	requeue_task_rt(rq, rq->donor, 0);
}

ساكن صحيح find_lowest_rq(بنية task_struct *task);

ساكن صحيح
select_task_rq_rt(بنية task_struct *p, صحيح cpu, صحيح flags)
{
	بنية task_struct *curr, *donor;
	بنية rq *rq;
	bool test;

	/* For anything but wake ups, just return the task_cpu */
	إذا (!(flags & (WF_TTWU | WF_FORK)))
		اذهب out;

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

	إذا (test || !rt_task_fits_capacity(p, cpu)) {
		صحيح target = find_lowest_rq(p);

		/*
		 * Bail out if we were forcing a migration to find a better
		 * fitting CPU but our search failed.
		 */
		إذا (!test && target != -1 && !rt_task_fits_capacity(p, target))
			اذهب out_unlock;

		/*
		 * Don't bother moving it if the destination CPU is
		 * not running a lower priority task.
		 */
		إذا (target != -1 &&
		    p->prio < cpu_rq(target)->rt.highest_prio.curr)
			cpu = target;
	}

out_unlock:
	rcu_read_unlock();

out:
	أرجع cpu;
}

ساكن فراغ check_preempt_equal_prio(بنية rq *rq, بنية task_struct *p)
{
	إذا (rq->curr->nr_cpus_allowed == 1 ||
	    !cpupri_find(&rq->rd->cpupri, rq->donor, عدم))
		أرجع;

	/*
	 * p is migratable, so let's not schedule it and
	 * see if it is pushed or pulled somewhere else.
	 */
	إذا (p->nr_cpus_allowed != 1 &&
	    cpupri_find(&rq->rd->cpupri, p, عدم))
		أرجع;

	/*
	 * There appear to be other CPUs that can accept
	 * the current task but none can run 'p', so lets reschedule
	 * to try and push the current task away:
	 */
	requeue_task_rt(rq, p, 1);
	resched_curr(rq);
}

ساكن صحيح balance_rt(بنية rq *rq, بنية rq_flags *rf)
{
	/*
	 * Note, rq->donor may change during rq lock drops,
	 * so don't re-use p across lock drops
	 */
	بنية task_struct *p = rq->donor;

	إذا (!on_rt_rq(&p->rt) && need_pull_rt_task(rq, p)) {
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

	أرجع sched_stop_runnable(rq) || sched_dl_runnable(rq) || sched_rt_runnable(rq);
}

/*
 * Preempt the current task with a newly woken task if needed:
 */
ساكن فراغ wakeup_preempt_rt(بنية rq *rq, بنية task_struct *p, صحيح flags)
{
	بنية task_struct *donor = rq->donor;

	/*
	 * XXX If we're preempted by DL, queue a push?
	 */
	إذا (p->sched_class != &rt_sched_class ||
	    donor->sched_class != &rt_sched_class)
		أرجع;

	إذا (p->prio < donor->prio) {
		resched_curr(rq);
		أرجع;
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
	إذا (p->prio == donor->prio && !test_tsk_need_resched(rq->curr))
		check_preempt_equal_prio(rq, p);
}

ساكن مضمن فراغ set_next_task_rt(بنية rq *rq, بنية task_struct *p, تعداد snt_e type)
{
	بنية sched_rt_entity *rt_se = &p->rt;
	بنية rt_rq *rt_rq = &rq->rt;

	إذا (type == SNT_REPICK)
		أرجع;

	p->se.exec_start = rq_clock_task(rq);
	إذا (on_rt_rq(&p->rt))
		update_stats_wait_end_rt(rt_rq, rt_se);

	/* The running task is never eligible for pushing */
	dequeue_pushable_task(rq, p);

	إذا (type != SNT_PICK)
		أرجع;

	/*
	 * If prev task was rt, put_prev_task() has already updated the
	 * utilization. We only care of the case where we start to schedule a
	 * rt task
	 */
	إذا (rq->donor->sched_class != &rt_sched_class)
		update_rt_rq_load_avg(rq_clock_pelt(rq), rq, 0);

	rt_queue_ادفع_tasks(rq);
}

ساكن بنية sched_rt_entity *pick_next_rt_entity(بنية rt_rq *rt_rq)
{
	بنية rt_prio_array *array = &rt_rq->active;
	بنية sched_rt_entity *next = عدم;
	بنية list_head *queue;
	صحيح idx;

	idx = sched_find_first_bit(array->bitmap);
	BUG_ON(idx >= MAX_RT_PRIO);

	queue = array->queue + idx;
	إذا (WARN_ON_ONCE(list_empty(queue)))
		أرجع عدم;
	next = list_entry(queue->next, بنية sched_rt_entity, run_list);

	أرجع next;
}

ساكن بنية task_struct *_pick_next_task_rt(بنية rq *rq)
{
	بنية sched_rt_entity *rt_se;
	بنية rt_rq *rt_rq  = &rq->rt;

	افعل {
		rt_se = pick_next_rt_entity(rt_rq);
		إذا (unlikely(!rt_se))
			أرجع عدم;
		rt_rq = group_rt_rq(rt_se);
	} طالما (rt_rq);

	أرجع rt_task_of(rt_se);
}

ساكن بنية task_struct *pick_task_rt(بنية rq *rq, بنية rq_flags *rf)
{
	بنية task_struct *p;

	إذا (!sched_rt_runnable(rq))
		أرجع عدم;

	p = _pick_next_task_rt(rq);

	أرجع p;
}

ساكن فراغ put_prev_task_rt(بنية rq *rq, بنية task_struct *p, بنية task_struct *next)
{
	بنية sched_rt_entity *rt_se = &p->rt;
	بنية rt_rq *rt_rq = &rq->rt;

	إذا (on_rt_rq(&p->rt))
		update_stats_wait_start_rt(rt_rq, rt_se);

	update_curr_rt(rq);

	update_rt_rq_load_avg(rq_clock_pelt(rq), rq, 1);

	إذا (task_is_blocked(p))
		أرجع;
	/*
	 * The previous task needs to be made eligible for pushing
	 * if it is still active
	 */
	إذا (on_rt_rq(&p->rt) && p->nr_cpus_allowed > 1)
		enqueue_pushable_task(rq, p);
}

/* Only try algorithms three times */
#define RT_MAX_TRIES 3

/*
 * Return the highest pushable rq's task, which is suitable to be executed
 * on the CPU, NULL otherwise
 */
ساكن بنية task_struct *pick_highest_pushable_task(بنية rq *rq, صحيح cpu)
{
	بنية plist_head *head = &rq->rt.pushable_tasks;
	بنية task_struct *p;

	إذا (!has_pushable_tasks(rq))
		أرجع عدم;

	plist_for_each_entry(p, head, pushable_tasks) {
		إذا (task_is_pushable(rq, p, cpu))
			أرجع p;
	}

	أرجع عدم;
}

ساكن DEFINE_PER_CPU(cpumask_var_t, local_cpu_mask);

ساكن صحيح find_lowest_rq(بنية task_struct *task)
{
	بنية sched_domain *sd;
	بنية cpumask *lowest_mask = this_cpu_cpumask_var_ptr(local_cpu_mask);
	صحيح this_cpu = smp_processor_id();
	صحيح cpu      = task_cpu(task);
	صحيح ret;

	/* Make sure the mask is initialized first */
	إذا (unlikely(!lowest_mask))
		أرجع -1;

	إذا (task->nr_cpus_allowed == 1)
		أرجع -1; /* No other targets possible */

	/*
	 * If we're on asym system ensure we consider the different capacities
	 * of the CPUs when searching for the lowest_mask.
	 */
	إذا (sched_asym_cpucap_active()) {

		ret = cpupri_find_fitness(&task_rq(task)->rd->cpupri,
					  task, lowest_mask,
					  rt_task_fits_capacity);
	} وإلا {

		ret = cpupri_find(&task_rq(task)->rd->cpupri,
				  task, lowest_mask);
	}

	إذا (!ret)
		أرجع -1; /* No targets found */

	/*
	 * At this point we have built a mask of CPUs representing the
	 * lowest priority tasks in the system.  Now we want to elect
	 * the best one based on our affinity and topology.
	 *
	 * We prioritize the last CPU that the task executed on since
	 * it is most likely cache-hot in that location.
	 */
	إذا (cpumask_test_cpu(cpu, lowest_mask))
		أرجع cpu;

	/*
	 * Otherwise, we consult the sched_domains span maps to figure
	 * out which CPU is logically closest to our hot cache data.
	 */
	إذا (!cpumask_test_cpu(this_cpu, lowest_mask))
		this_cpu = -1; /* Skip this_cpu opt if not among lowest */

	rcu_read_lock();
	for_each_domain(cpu, sd) {
		إذا (sd->flags & SD_WAKE_AFFINE) {
			صحيح best_cpu;

			/*
			 * "this_cpu" is cheaper to preempt than a
			 * remote processor.
			 */
			إذا (this_cpu != -1 &&
			    cpumask_test_cpu(this_cpu, sched_domain_span(sd))) {
				rcu_read_unlock();
				أرجع this_cpu;
			}

			best_cpu = cpumask_any_and_distribute(lowest_mask,
							      sched_domain_span(sd));
			إذا (best_cpu < nr_cpu_ids) {
				rcu_read_unlock();
				أرجع best_cpu;
			}
		}
	}
	rcu_read_unlock();

	/*
	 * And finally, if there were no matches within the domains
	 * just give the caller *something* to work with from the compatible
	 * locations.
	 */
	إذا (this_cpu != -1)
		أرجع this_cpu;

	cpu = cpumask_any_distribute(lowest_mask);
	إذا (cpu < nr_cpu_ids)
		أرجع cpu;

	أرجع -1;
}

ساكن بنية task_struct *pick_next_pushable_task(بنية rq *rq)
{
	بنية plist_head *head = &rq->rt.pushable_tasks;
	بنية task_struct *i, *p = عدم;

	إذا (!has_pushable_tasks(rq))
		أرجع عدم;

	plist_for_each_entry(i, head, pushable_tasks) {
		/* skip tasks that cannot be migrated */
		إذا (!task_on_cpu(rq, i) && !is_migration_disabled(i)) {
			p = i;
			اكسر;
		}
	}

	إذا (!p)
		أرجع عدم;

	BUG_ON(rq->cpu != task_cpu(p));
	BUG_ON(task_current(rq, p));
	BUG_ON(task_current_donor(rq, p));
	BUG_ON(p->nr_cpus_allowed <= 1);

	BUG_ON(!task_on_rq_queued(p));
	BUG_ON(!rt_task(p));

	أرجع p;
}

/* Will lock the rq it finds */
ساكن بنية rq *find_lock_lowest_rq(بنية task_struct *task, بنية rq *rq)
{
	بنية rq *lowest_rq = عدم;
	صحيح tries;
	صحيح cpu;

	لكل (tries = 0; tries < RT_MAX_TRIES; tries++) {
		cpu = find_lowest_rq(task);

		إذا ((cpu == -1) || (cpu == rq->cpu))
			اكسر;

		lowest_rq = cpu_rq(cpu);

		إذا (lowest_rq->rt.highest_prio.curr <= task->prio) {
			/*
			 * Target rq has tasks of equal or higher priority,
			 * retrying does not release any lock and is unlikely
			 * to yield a different result.
			 */
			lowest_rq = عدم;
			اكسر;
		}

		/* if the prio of this runqueue changed, try again */
		إذا (double_lock_balance(rq, lowest_rq)) {
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
			إذا (unlikely(is_migration_disabled(task) ||
				     !cpumask_test_cpu(lowest_rq->cpu, &task->cpus_mask) ||
				     task != pick_next_pushable_task(rq))) {

				double_unlock_balance(rq, lowest_rq);
				lowest_rq = عدم;
				اكسر;
			}
		}

		/* If this rq is still suitable use it. */
		إذا (lowest_rq->rt.highest_prio.curr > task->prio)
			اكسر;

		/* try again */
		double_unlock_balance(rq, lowest_rq);
		lowest_rq = عدم;
	}

	أرجع lowest_rq;
}

/*
 * If the current CPU has more than one RT task, see if the non
 * running task can migrate over to a CPU that is running a task
 * of lesser priority.
 */
ساكن صحيح ادفع_rt_task(بنية rq *rq, bool pull)
{
	بنية task_struct *next_task;
	بنية rq *lowest_rq;
	صحيح ret = 0;

	إذا (!rq->rt.overloaded)
		أرجع 0;

	next_task = pick_next_pushable_task(rq);
	إذا (!next_task)
		أرجع 0;

retry:
	/*
	 * It's possible that the next_task slipped in of
	 * higher priority than current. If that's the case
	 * just reschedule current.
	 */
	إذا (unlikely(next_task->prio < rq->donor->prio)) {
		resched_curr(rq);
		أرجع 0;
	}

	إذا (is_migration_disabled(next_task)) {
		بنية task_struct *ادفع_task = عدم;
		صحيح cpu;

		إذا (!pull || rq->ادفع_busy)
			أرجع 0;

		/*
		 * Invoking find_lowest_rq() on anything but an RT task doesn't
		 * make sense. Per the above priority check, curr has to
		 * be of higher priority than next_task, so no need to
		 * reschedule when bailing out.
		 *
		 * Note that the stoppers are masqueraded as SCHED_FIFO
		 * (cf. sched_set_stop_task()), so we can't rely on rt_task().
		 */
		إذا (rq->donor->sched_class != &rt_sched_class)
			أرجع 0;

		cpu = find_lowest_rq(rq->curr);
		إذا (cpu == -1 || cpu == rq->cpu)
			أرجع 0;

		/*
		 * Given we found a CPU with lower priority than @next_task,
		 * therefore it should be running. However we cannot migrate it
		 * to this other CPU, instead attempt to push the current
		 * running task on this CPU away.
		 */
		ادفع_task = get_ادفع_task(rq);
		إذا (ادفع_task) {
			preempt_disable();
			raw_spin_rq_unlock(rq);
			stop_one_cpu_nowait(rq->cpu, ادفع_cpu_stop,
					    ادفع_task, &rq->ادفع_work);
			preempt_enable();
			raw_spin_rq_lock(rq);
		}

		أرجع 0;
	}

	إذا (WARN_ON(next_task == rq->curr))
		أرجع 0;

	/* We might release rq lock */
	get_task_struct(next_task);

	/* find_lock_lowest_rq locks the rq if found */
	lowest_rq = find_lock_lowest_rq(next_task, rq);
	إذا (!lowest_rq) {
		بنية task_struct *task;
		/*
		 * find_lock_lowest_rq releases rq->lock
		 * so it is possible that next_task has migrated.
		 *
		 * We need to make sure that the task is still on the same
		 * run-queue and is also still the next task eligible for
		 * pushing.
		 */
		task = pick_next_pushable_task(rq);
		إذا (task == next_task) {
			/*
			 * The task hasn't migrated, and is still the next
			 * eligible task, but we failed to find a run-queue
			 * to push it to.  Do not retry in this case, since
			 * other CPUs will pull from us when ready.
			 */
			اذهب out;
		}

		إذا (!task)
			/* No more tasks, just exit */
			اذهب out;

		/*
		 * Something has shifted, try again.
		 */
		put_task_struct(next_task);
		next_task = task;
		اذهب retry;
	}

	move_queued_task_locked(rq, lowest_rq, next_task);
	resched_curr(lowest_rq);
	ret = 1;

	double_unlock_balance(rq, lowest_rq);
out:
	put_task_struct(next_task);

	أرجع ret;
}

ساكن فراغ ادفع_rt_tasks(بنية rq *rq)
{
	/* push_rt_task will return true if it moved an RT */
	طالما (ادفع_rt_task(rq, false))
		;
}

#ifdef HAVE_RT_ادفع_IPI

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
ساكن صحيح rto_next_cpu(بنية جذر_domain *rd)
{
	صحيح this_cpu = smp_processor_id();
	صحيح next;
	صحيح cpu;

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
	لكل (;;) {

		/* When rto_cpu is -1 this acts like cpumask_first() */
		cpu = cpumask_next(rd->rto_cpu, rd->rto_mask);

		rd->rto_cpu = cpu;

		/* Do not send IPI to self */
		إذا (cpu == this_cpu)
			استمر;

		إذا (cpu < nr_cpu_ids)
			أرجع cpu;

		rd->rto_cpu = -1;

		/*
		 * ACQUIRE ensures we see the @rto_mask changes
		 * made prior to the @next value observed.
		 *
		 * Matches WMB in rt_set_overload().
		 */
		next = atomic_read_acquire(&rd->rto_loop_next);

		إذا (rd->rto_loop == next)
			اكسر;

		rd->rto_loop = next;
	}

	أرجع -1;
}

ساكن مضمن bool rto_start_trylock(atomic_t *v)
{
	أرجع !atomic_cmpxchg_acquire(v, 0, 1);
}

ساكن مضمن فراغ rto_start_unlock(atomic_t *v)
{
	atomic_set_release(v, 0);
}

ساكن فراغ tell_cpu_to_ادفع(بنية rq *rq)
{
	صحيح cpu = -1;

	/* Keep the loop going if the IPI is currently active */
	atomic_inc(&rq->rd->rto_loop_next);

	/* Only one CPU can initiate a loop at a time */
	إذا (!rto_start_trylock(&rq->rd->rto_loop_start))
		أرجع;

	raw_spin_lock(&rq->rd->rto_lock);

	/*
	 * The rto_cpu is updated under the lock, if it has a valid CPU
	 * then the IPI is still running and will continue due to the
	 * update to loop_next, and nothing needs to be done here.
	 * Otherwise it is finishing up and an IPI needs to be sent.
	 */
	إذا (rq->rd->rto_cpu < 0)
		cpu = rto_next_cpu(rq->rd);

	raw_spin_unlock(&rq->rd->rto_lock);

	rto_start_unlock(&rq->rd->rto_loop_start);

	إذا (cpu >= 0) {
		/* Make sure the rd does not get freed while pushing */
		sched_get_rd(rq->rd);
		irq_work_queue_on(&rq->rd->rto_ادفع_work, cpu);
	}
}

/* Called from hardirq context */
فراغ rto_ادفع_irq_work_func(بنية irq_work *work)
{
	بنية جذر_domain *rd =
		container_of(work, بنية جذر_domain, rto_ادفع_work);
	بنية rq *rq;
	صحيح cpu;

	rq = this_rq();

	/*
	 * We do not need to grab the lock to check for has_pushable_tasks.
	 * When it gets updated, a check is made if a push is possible.
	 */
	إذا (has_pushable_tasks(rq)) {
		raw_spin_rq_lock(rq);
		طالما (ادفع_rt_task(rq, true))
			;
		raw_spin_rq_unlock(rq);
	}

	raw_spin_lock(&rd->rto_lock);

	/* Pass the IPI to the next rt overloaded queue */
	cpu = rto_next_cpu(rd);

	raw_spin_unlock(&rd->rto_lock);

	إذا (cpu < 0) {
		sched_put_rd(rd);
		أرجع;
	}

	/* Try the next RT overloaded CPU */
	irq_work_queue_on(&rd->rto_ادفع_work, cpu);
}
#endif /* HAVE_RT_PUSH_IPI */

ساكن فراغ pull_rt_task(بنية rq *this_rq)
{
	صحيح this_cpu = this_rq->cpu, cpu;
	bool resched = false;
	بنية task_struct *p, *ادفع_task;
	بنية rq *src_rq;
	صحيح rt_overload_عدد = rt_overloaded(this_rq);

	إذا (likely(!rt_overload_عدد))
		أرجع;

	/*
	 * Match the barrier from rt_set_overloaded; this guarantees that if we
	 * see overloaded we must also see the rto_mask bit.
	 */
	smp_rmb();

	/* If we are the only overloaded CPU do nothing */
	إذا (rt_overload_عدد == 1 &&
	    cpumask_test_cpu(this_rq->cpu, this_rq->rd->rto_mask))
		أرجع;

#ifdef HAVE_RT_ادفع_IPI
	إذا (sched_feat(RT_ادفع_IPI)) {
		tell_cpu_to_ادفع(this_rq);
		أرجع;
	}
#endif

	for_each_cpu(cpu, this_rq->rd->rto_mask) {
		إذا (this_cpu == cpu)
			استمر;

		src_rq = cpu_rq(cpu);

		/*
		 * Don't bother taking the src_rq->lock if the next highest
		 * task is known to be lower-priority than our current task.
		 * This may look racy, but if this value is about to go
		 * logically higher, the src_rq will push this task away.
		 * And if its going logically lower, we do not care
		 */
		إذا (src_rq->rt.highest_prio.next >=
		    this_rq->rt.highest_prio.curr)
			استمر;

		/*
		 * We can potentially drop this_rq's lock in
		 * double_lock_balance, and another CPU could
		 * alter this_rq
		 */
		ادفع_task = عدم;
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
		إذا (p && (p->prio < this_rq->rt.highest_prio.curr)) {
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
			إذا (p->prio < src_rq->donor->prio)
				اذهب skip;

			إذا (is_migration_disabled(p)) {
				ادفع_task = get_ادفع_task(src_rq);
			} وإلا {
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

		إذا (ادفع_task) {
			preempt_disable();
			raw_spin_rq_unlock(this_rq);
			stop_one_cpu_nowait(src_rq->cpu, ادفع_cpu_stop,
					    ادفع_task, &src_rq->ادفع_work);
			preempt_enable();
			raw_spin_rq_lock(this_rq);
		}
	}

	إذا (resched)
		resched_curr(this_rq);
}

/*
 * If we are not running and we are not going to reschedule soon, we should
 * try to push tasks away now
 */
ساكن فراغ task_woken_rt(بنية rq *rq, بنية task_struct *p)
{
	bool need_to_ادفع = !task_on_cpu(rq, p) &&
			    !test_tsk_need_resched(rq->curr) &&
			    p->nr_cpus_allowed > 1 &&
			    (dl_task(rq->donor) || rt_task(rq->donor)) &&
			    (rq->curr->nr_cpus_allowed < 2 ||
			     rq->donor->prio <= p->prio);

	إذا (need_to_ادفع)
		ادفع_rt_tasks(rq);
}

/* Assumes rq->lock is held */
ساكن فراغ rq_online_rt(بنية rq *rq)
{
	إذا (rq->rt.overloaded)
		rt_set_overload(rq);

	__enable_runtime(rq);

	cpupri_set(&rq->rd->cpupri, rq->cpu, rq->rt.highest_prio.curr);
}

/* Assumes rq->lock is held */
ساكن فراغ rq_offline_rt(بنية rq *rq)
{
	إذا (rq->rt.overloaded)
		rt_clear_overload(rq);

	__disable_runtime(rq);

	cpupri_set(&rq->rd->cpupri, rq->cpu, CPUPRI_INVALID);
}

/*
 * When switch from the rt queue, we bring ourselves to a position
 * that we might want to pull RT tasks from other runqueues.
 */
ساكن فراغ switched_from_rt(بنية rq *rq, بنية task_struct *p)
{
	/*
	 * If there are other RT tasks then we will reschedule
	 * and the scheduling of the other RT tasks will handle
	 * the balancing. But if we are the last RT task
	 * we may need to handle the pulling of RT tasks
	 * now.
	 */
	إذا (!task_on_rq_queued(p) || rq->rt.rt_nr_running)
		أرجع;

	rt_queue_pull_task(rq);
}

فراغ __init init_sched_rt_class(فراغ)
{
	unsigned صحيح i;

	for_each_possible_cpu(i) {
		zalloc_cpumask_var_عقدة(&per_cpu(local_cpu_mask, i),
					GFP_KERNEL, cpu_to_عقدة(i));
	}
}

/*
 * When switching a task to RT, we may overload the runqueue
 * with RT tasks. In this case we try to push them off to
 * other runqueues.
 */
ساكن فراغ switched_to_rt(بنية rq *rq, بنية task_struct *p)
{
	/*
	 * If we are running, update the avg_rt tracking, as the running time
	 * will now on be accounted into the latter.
	 */
	إذا (task_current(rq, p)) {
		update_rt_rq_load_avg(rq_clock_pelt(rq), rq, 0);
		أرجع;
	}

	/*
	 * If we are not running we may need to preempt the current
	 * running task. If that current running task is also an RT task
	 * then see if we can move to another run queue.
	 */
	إذا (task_on_rq_queued(p)) {
		إذا (p->nr_cpus_allowed > 1 && rq->rt.overloaded)
			rt_queue_ادفع_tasks(rq);
		إذا (p->prio < rq->donor->prio && cpu_online(cpu_of(rq)))
			resched_curr(rq);
	}
}

/*
 * Priority of the task has changed. This may cause
 * us to initiate a push or pull.
 */
ساكن فراغ
prio_changed_rt(بنية rq *rq, بنية task_struct *p, u64 oldprio)
{
	إذا (!task_on_rq_queued(p))
		أرجع;

	إذا (p->prio == oldprio)
		أرجع;

	إذا (task_current_donor(rq, p)) {
		/*
		 * If our priority decreases while running, we
		 * may need to pull tasks to this runqueue.
		 */
		إذا (oldprio < p->prio)
			rt_queue_pull_task(rq);

		/*
		 * If there's a higher priority task waiting to run
		 * then reschedule.
		 */
		إذا (p->prio > rq->rt.highest_prio.curr)
			resched_curr(rq);
	} وإلا {
		/*
		 * This task is not running, but if it is
		 * greater than the current running task
		 * then reschedule.
		 */
		إذا (p->prio < rq->donor->prio)
			resched_curr(rq);
	}
}

#ifdef CONFIG_POSIX_TIMERS
ساكن فراغ watchdog(بنية rq *rq, بنية task_struct *p)
{
	unsigned long soft, hard;

	/* max may change after cur was read, this will be fixed next tick */
	soft = task_rlimit(p, RLIMIT_RTTIME);
	hard = task_rlimit_max(p, RLIMIT_RTTIME);

	إذا (soft != RLIM_INFINITY) {
		unsigned long next;

		إذا (p->rt.watchdog_stamp != jiffies) {
			p->rt.timeout++;
			p->rt.watchdog_stamp = jiffies;
		}

		next = DIV_ROUND_UP(min(soft, hard), USEC_PER_SEC/HZ);
		إذا (p->rt.timeout > next) {
			posix_cputimers_rt_watchdog(&p->posix_cputimers,
						    p->se.sum_exec_runtime);
		}
	}
}
#وإلا /* !CONFIG_POSIX_TIMERS: */
ساكن مضمن فراغ watchdog(بنية rq *rq, بنية task_struct *p) { }
#endif /* !CONFIG_POSIX_TIMERS */

/*
 * scheduler tick hitting a task of our scheduling class.
 *
 * NOTE: This function can be called remotely by the tick offload that
 * goes along full dynticks. Therefore no local assumption can be made
 * and everything must be accessed through the @rq and @curr passed in
 * parameters.
 */
ساكن فراغ task_tick_rt(بنية rq *rq, بنية task_struct *p, صحيح queued)
{
	بنية sched_rt_entity *rt_se = &p->rt;

	update_curr_rt(rq);
	update_rt_rq_load_avg(rq_clock_pelt(rq), rq, 1);

	watchdog(rq, p);

	/*
	 * RR tasks need a special form of time-slice management.
	 * FIFO tasks have no timeslices.
	 */
	إذا (p->policy != SCHED_RR)
		أرجع;

	إذا (--p->rt.time_slice)
		أرجع;

	p->rt.time_slice = sched_rr_timeslice;

	/*
	 * Requeue to the end of queue if we (and all of our ancestors) are not
	 * the only element on the queue
	 */
	for_each_sched_rt_entity(rt_se) {
		إذا (rt_se->run_list.prev != rt_se->run_list.next) {
			requeue_task_rt(rq, p, 0);
			resched_curr(rq);
			أرجع;
		}
	}
}

ساكن unsigned صحيح get_rr_interval_rt(بنية rq *rq, بنية task_struct *task)
{
	/*
	 * Time slice is 0 for SCHED_FIFO tasks
	 */
	إذا (task->policy == SCHED_RR)
		أرجع sched_rr_timeslice;
	وإلا
		أرجع 0;
}

#ifdef CONFIG_SCHED_CORE
ساكن صحيح task_is_throttled_rt(بنية task_struct *p, صحيح cpu)
{
	بنية rt_rq *rt_rq;

#ifdef CONFIG_RT_GROUP_SCHED // XXX maybe add task_rt_rq(), see also sched_rt_period_rt_rq
	rt_rq = task_group(p)->rt_rq[cpu];
	WARN_ON(!rt_group_sched_enabled() && rt_rq->tg != &جذر_task_group);
#وإلا
	rt_rq = &cpu_rq(cpu)->rt;
#endif

	أرجع rt_rq_throttled(rt_rq);
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
ساكن DEFINE_MUTEX(rt_constraints_mutex);

ساكن مضمن صحيح tg_has_rt_tasks(بنية task_group *tg)
{
	بنية task_struct *task;
	بنية css_task_iter it;
	صحيح ret = 0;

	/*
	 * Autogroups do not have RT tasks; see autogroup_create().
	 */
	إذا (task_group_is_autogroup(tg))
		أرجع 0;

	css_task_iter_start(&tg->css, 0, &it);
	طالما (!ret && (task = css_task_iter_next(&it)))
		ret |= rt_task(task);
	css_task_iter_end(&it);

	أرجع ret;
}

بنية rt_schedulable_data {
	بنية task_group *tg;
	u64 rt_period;
	u64 rt_runtime;
};

ساكن صحيح tg_rt_schedulable(بنية task_group *tg, فراغ *data)
{
	بنية rt_schedulable_data *d = data;
	بنية task_group *child;
	u64 total, sum = 0;
	u64 period, runtime;

	period = ktime_to_ns(tg->rt_bandwidth.rt_period);
	runtime = tg->rt_bandwidth.rt_runtime;

	إذا (tg == d->tg) {
		period = d->rt_period;
		runtime = d->rt_runtime;
	}

	/*
	 * Cannot have more runtime than the period.
	 */
	إذا (runtime > period && runtime != RUNTIME_INF)
		أرجع -EINVAL;

	/*
	 * Ensure we don't starve existing RT tasks if runtime turns zero.
	 */
	إذا (rt_bandwidth_enabled() && !runtime &&
	    tg->rt_bandwidth.rt_runtime && tg_has_rt_tasks(tg))
		أرجع -EBUSY;

	total = to_ratio(period, runtime);

	/*
	 * Nobody can have more than the global setting allows.
	 */
	إذا (total > to_ratio(global_rt_period(), global_rt_runtime()))
		أرجع -EINVAL;

	/*
	 * The sum of our children's runtime should not exceed our own.
	 */
	list_for_each_entry_rcu(child, &tg->children, siblings) {
		period = ktime_to_ns(child->rt_bandwidth.rt_period);
		runtime = child->rt_bandwidth.rt_runtime;

		إذا (child == d->tg) {
			period = d->rt_period;
			runtime = d->rt_runtime;
		}

		sum += to_ratio(period, runtime);
	}

	إذا (sum > total)
		أرجع -EINVAL;

	أرجع 0;
}

ساكن صحيح __rt_schedulable(بنية task_group *tg, u64 period, u64 runtime)
{
	صحيح ret;

	بنية rt_schedulable_data data = {
		.tg = tg,
		.rt_period = period,
		.rt_runtime = runtime,
	};

	rcu_read_lock();
	ret = walk_tg_شجرة(tg_rt_schedulable, tg_nop, &data);
	rcu_read_unlock();

	أرجع ret;
}

ساكن صحيح tg_set_rt_bandwidth(بنية task_group *tg,
		u64 rt_period, u64 rt_runtime)
{
	صحيح i, err = 0;

	/*
	 * Disallowing the root group RT runtime is BAD, it would disallow the
	 * kernel creating (and or operating) RT threads.
	 */
	إذا (tg == &جذر_task_group && rt_runtime == 0)
		أرجع -EINVAL;

	/* No period doesn't make any sense. */
	إذا (rt_period == 0)
		أرجع -EINVAL;

	/*
	 * Bound quota to defend quota against overflow during bandwidth shift.
	 */
	إذا (rt_runtime != RUNTIME_INF && rt_runtime > max_rt_runtime)
		أرجع -EINVAL;

	mutex_lock(&rt_constraints_mutex);
	err = __rt_schedulable(tg, rt_period, rt_runtime);
	إذا (err)
		اذهب unlock;

	raw_spin_lock_irq(&tg->rt_bandwidth.rt_runtime_lock);
	tg->rt_bandwidth.rt_period = ns_to_ktime(rt_period);
	tg->rt_bandwidth.rt_runtime = rt_runtime;

	for_each_possible_cpu(i) {
		بنية rt_rq *rt_rq = tg->rt_rq[i];

		raw_spin_lock(&rt_rq->rt_runtime_lock);
		rt_rq->rt_runtime = rt_runtime;
		raw_spin_unlock(&rt_rq->rt_runtime_lock);
	}
	raw_spin_unlock_irq(&tg->rt_bandwidth.rt_runtime_lock);
unlock:
	mutex_unlock(&rt_constraints_mutex);

	أرجع err;
}

صحيح sched_group_set_rt_runtime(بنية task_group *tg, long rt_runtime_us)
{
	u64 rt_runtime, rt_period;

	rt_period = ktime_to_ns(tg->rt_bandwidth.rt_period);
	rt_runtime = (u64)rt_runtime_us * NSEC_PER_USEC;
	إذا (rt_runtime_us < 0)
		rt_runtime = RUNTIME_INF;
	وإلا إذا ((u64)rt_runtime_us > U64_MAX / NSEC_PER_USEC)
		أرجع -EINVAL;

	أرجع tg_set_rt_bandwidth(tg, rt_period, rt_runtime);
}

long sched_group_rt_runtime(بنية task_group *tg)
{
	u64 rt_runtime_us;

	إذا (tg->rt_bandwidth.rt_runtime == RUNTIME_INF)
		أرجع -1;

	rt_runtime_us = tg->rt_bandwidth.rt_runtime;
	do_div(rt_runtime_us, NSEC_PER_USEC);
	أرجع rt_runtime_us;
}

صحيح sched_group_set_rt_period(بنية task_group *tg, u64 rt_period_us)
{
	u64 rt_runtime, rt_period;

	إذا (rt_period_us > U64_MAX / NSEC_PER_USEC)
		أرجع -EINVAL;

	rt_period = rt_period_us * NSEC_PER_USEC;
	rt_runtime = tg->rt_bandwidth.rt_runtime;

	أرجع tg_set_rt_bandwidth(tg, rt_period, rt_runtime);
}

long sched_group_rt_period(بنية task_group *tg)
{
	u64 rt_period_us;

	rt_period_us = ktime_to_ns(tg->rt_bandwidth.rt_period);
	do_div(rt_period_us, NSEC_PER_USEC);
	أرجع rt_period_us;
}

صحيح sched_rt_can_attach(بنية task_group *tg, بنية task_struct *tsk)
{
	/* Don't accept real-time tasks when there is no way for them to run */
	إذا (rt_group_sched_enabled() && rt_task(tsk) && tg->rt_bandwidth.rt_runtime == 0)
		أرجع 0;

	أرجع 1;
}

#endif /* !CONFIG_RT_GROUP_SCHED */

#ifdef CONFIG_SYSCTL
ساكن صحيح sched_rt_global_validate(فراغ)
{
	إذا ((sysctl_sched_rt_runtime != RUNTIME_INF) &&
		((sysctl_sched_rt_runtime > sysctl_sched_rt_period) ||
		 ((u64)sysctl_sched_rt_runtime *
			NSEC_PER_USEC > max_rt_runtime)))
		أرجع -EINVAL;

#ifdef CONFIG_RT_GROUP_SCHED
	إذا (!rt_group_sched_enabled())
		أرجع 0;

	scoped_guard(mutex, &rt_constraints_mutex)
		أرجع __rt_schedulable(عدم, 0, 0);
#endif
	أرجع 0;
}

ساكن صحيح sched_rt_handler(ثابت بنية ctl_table *table, صحيح اكتب, فراغ *buffer,
		size_t *lenp, loff_t *ppos)
{
	صحيح old_period, old_runtime;
	ساكن DEFINE_MUTEX(mutex);
	صحيح ret;

	mutex_lock(&mutex);
	sched_domains_mutex_lock();
	old_period = sysctl_sched_rt_period;
	old_runtime = sysctl_sched_rt_runtime;

	ret = proc_dointvec_minmax(table, اكتب, buffer, lenp, ppos);

	إذا (!ret && اكتب) {
		ret = sched_rt_global_validate();
		إذا (ret)
			اذهب undo;

		ret = sched_dl_global_validate();
		إذا (ret)
			اذهب undo;

		sched_dl_do_global();
	}
	إذا (0) {
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

	أرجع ret;
}

ساكن صحيح sched_rr_handler(ثابت بنية ctl_table *table, صحيح اكتب, فراغ *buffer,
		size_t *lenp, loff_t *ppos)
{
	صحيح ret;
	ساكن DEFINE_MUTEX(mutex);

	mutex_lock(&mutex);
	ret = proc_dointvec(table, اكتب, buffer, lenp, ppos);
	/*
	 * Make sure that internally we keep jiffies.
	 * Also, writing zero resets the time-slice to default:
	 */
	إذا (!ret && اكتب) {
		sched_rr_timeslice =
			sysctl_sched_rr_timeslice <= 0 ? RR_TIMESLICE :
			msecs_to_jiffies(sysctl_sched_rr_timeslice);

		إذا (sysctl_sched_rr_timeslice <= 0)
			sysctl_sched_rr_timeslice = jiffies_to_msecs(RR_TIMESLICE);
	}
	mutex_unlock(&mutex);

	أرجع ret;
}
#endif /* CONFIG_SYSCTL */

فراغ اطبع_rt_stats(بنية seq_file *m, صحيح cpu)
{
	rt_rq_iter_t iter;
	بنية rt_rq *rt_rq;

	rcu_read_lock();
	for_each_rt_rq(rt_rq, iter, cpu_rq(cpu))
		اطبع_rt_rq(m, cpu, rt_rq);
	rcu_read_unlock();
}
