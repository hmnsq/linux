// SPDX-License-Identifier: GPL-2.0
/*
 * Real-Time Scheduling Class (mapped to the SCHED_FIFO and SCHED_RR
 * policies)
 */

#അന്തര്ഭാവ "sched.h"
#അന്തര്ഭാവ "pelt.h"

പൂർണ്ണസംഖ്യ sched_rr_timeslice = RR_TIMESLICE;
/* More than 4 hours if BW_SHIFT equals 20. */
സ്ഥിതം സ്ഥിരം u64 max_rt_runtime = MAX_BW;

/*
 * period over which we measure -rt task CPU usage in us.
 * default: 1s
 */
പൂർണ്ണസംഖ്യ sysctl_sched_rt_period = 1000000;

/*
 * part of the period that we allow rt tasks to run in us.
 * default: 1s
 */
പൂർണ്ണസംഖ്യ sysctl_sched_rt_runtime = 1000000;

#ifdef CONFIG_SYSCTL
സ്ഥിതം പൂർണ്ണസംഖ്യ sysctl_sched_rr_timeslice = (MSEC_PER_SEC * RR_TIMESLICE) / HZ;
സ്ഥിതം പൂർണ്ണസംഖ്യ sched_rt_handler(സ്ഥിരം ഘടന ctl_table *table, പൂർണ്ണസംഖ്യ ലിഖ, ശൂന്യം *buffer,
		size_t *lenp, loff_t *ppos);
സ്ഥിതം പൂർണ്ണസംഖ്യ sched_rr_handler(സ്ഥിരം ഘടന ctl_table *table, പൂർണ്ണസംഖ്യ ലിഖ, ശൂന്യം *buffer,
		size_t *lenp, loff_t *ppos);
സ്ഥിതം സ്ഥിരം ഘടന ctl_table sched_rt_sysctls[] = {
	{
		.procname       = "sched_rt_period_us",
		.data           = &sysctl_sched_rt_period,
		.maxlen         = വലുപ്പം(പൂർണ്ണസംഖ്യ),
		.mode           = 0644,
		.proc_handler   = sched_rt_handler,
		.extra1         = SYSCTL_ONE,
		.extra2         = SYSCTL_INT_MAX,
	},
	{
		.procname       = "sched_rt_runtime_us",
		.data           = &sysctl_sched_rt_runtime,
		.maxlen         = വലുപ്പം(പൂർണ്ണസംഖ്യ),
		.mode           = 0644,
		.proc_handler   = sched_rt_handler,
		.extra1         = SYSCTL_NEG_ONE,
		.extra2         = (ശൂന്യം *)&sysctl_sched_rt_period,
	},
	{
		.procname       = "sched_rr_timeslice_ms",
		.data           = &sysctl_sched_rr_timeslice,
		.maxlen         = വലുപ്പം(പൂർണ്ണസംഖ്യ),
		.mode           = 0644,
		.proc_handler   = sched_rr_handler,
	},
};

സ്ഥിതം പൂർണ്ണസംഖ്യ __init sched_rt_sysctl_init(ശൂന്യം)
{
	register_sysctl_init("kernel", sched_rt_sysctls);
	തിരികെ 0;
}
late_initcall(sched_rt_sysctl_init);
#endif /* CONFIG_SYSCTL */

ശൂന്യം init_rt_rq(ഘടന rt_rq *rt_rq)
{
	ഘടന rt_prio_array *array;
	പൂർണ്ണസംഖ്യ i;

	array = &rt_rq->active;
	വേണ്ടി (i = 0; i < MAX_RT_PRIO; i++) {
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
	rt_rq->tg = &വേര്_task_group;
#endif
}

#ifdef CONFIG_RT_GROUP_SCHED

സ്ഥിതം പൂർണ്ണസംഖ്യ do_sched_rt_period_timer(ഘടന rt_bandwidth *rt_b, പൂർണ്ണസംഖ്യ overrun);

സ്ഥിതം എണ്ണൽ hrtimer_restart sched_rt_period_timer(ഘടന hrtimer *timer)
{
	ഘടന rt_bandwidth *rt_b =
		container_of(timer, ഘടന rt_bandwidth, rt_period_timer);
	പൂർണ്ണസംഖ്യ idle = 0;
	പൂർണ്ണസംഖ്യ overrun;

	raw_spin_lock(&rt_b->rt_runtime_lock);
	വേണ്ടി (;;) {
		overrun = hrtimer_forward_now(timer, rt_b->rt_period);
		എങ്കിൽ (!overrun)
			നിർത്തുക;

		raw_spin_unlock(&rt_b->rt_runtime_lock);
		idle = do_sched_rt_period_timer(rt_b, overrun);
		raw_spin_lock(&rt_b->rt_runtime_lock);
	}
	എങ്കിൽ (idle)
		rt_b->rt_period_active = 0;
	raw_spin_unlock(&rt_b->rt_runtime_lock);

	തിരികെ idle ? HRTIMER_NORESTART : HRTIMER_RESTART;
}

ശൂന്യം init_rt_bandwidth(ഘടന rt_bandwidth *rt_b, u64 period, u64 runtime)
{
	rt_b->rt_period = ns_to_ktime(period);
	rt_b->rt_runtime = runtime;

	raw_spin_lock_init(&rt_b->rt_runtime_lock);

	hrtimer_setup(&rt_b->rt_period_timer, sched_rt_period_timer, CLOCK_MONOTONIC,
		      HRTIMER_MODE_REL_HARD);
}

സ്ഥിതം വരിയിൽ ശൂന്യം do_start_rt_bandwidth(ഘടന rt_bandwidth *rt_b)
{
	raw_spin_lock(&rt_b->rt_runtime_lock);
	എങ്കിൽ (!rt_b->rt_period_active) {
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

സ്ഥിതം ശൂന്യം start_rt_bandwidth(ഘടന rt_bandwidth *rt_b)
{
	എങ്കിൽ (!rt_bandwidth_enabled() || rt_b->rt_runtime == RUNTIME_INF)
		തിരികെ;

	do_start_rt_bandwidth(rt_b);
}

സ്ഥിതം ശൂന്യം destroy_rt_bandwidth(ഘടന rt_bandwidth *rt_b)
{
	hrtimer_cancel(&rt_b->rt_period_timer);
}

#define rt_entity_is_task(rt_se) (!(rt_se)->my_q)

സ്ഥിതം വരിയിൽ ഘടന task_struct *rt_task_of(ഘടന sched_rt_entity *rt_se)
{
	WARN_ON_ONCE(!rt_entity_is_task(rt_se));

	തിരികെ container_of(rt_se, ഘടന task_struct, rt);
}

സ്ഥിതം വരിയിൽ ഘടന rq *rq_of_rt_rq(ഘടന rt_rq *rt_rq)
{
	/* Cannot fold with non-CONFIG_RT_GROUP_SCHED version, layout */
	WARN_ON(!rt_group_sched_enabled() && rt_rq->tg != &വേര്_task_group);
	തിരികെ rt_rq->rq;
}

സ്ഥിതം വരിയിൽ ഘടന rt_rq *rt_rq_of_se(ഘടന sched_rt_entity *rt_se)
{
	WARN_ON(!rt_group_sched_enabled() && rt_se->rt_rq->tg != &വേര്_task_group);
	തിരികെ rt_se->rt_rq;
}

സ്ഥിതം വരിയിൽ ഘടന rq *rq_of_rt_se(ഘടന sched_rt_entity *rt_se)
{
	ഘടന rt_rq *rt_rq = rt_se->rt_rq;

	WARN_ON(!rt_group_sched_enabled() && rt_rq->tg != &വേര്_task_group);
	തിരികെ rt_rq->rq;
}

ശൂന്യം unregister_rt_sched_group(ഘടന task_group *tg)
{
	എങ്കിൽ (!rt_group_sched_enabled())
		തിരികെ;

	എങ്കിൽ (tg->rt_se)
		destroy_rt_bandwidth(&tg->rt_bandwidth);
}

ശൂന്യം free_rt_sched_group(ഘടന task_group *tg)
{
	പൂർണ്ണസംഖ്യ i;

	എങ്കിൽ (!rt_group_sched_enabled())
		തിരികെ;

	for_each_possible_cpu(i) {
		എങ്കിൽ (tg->rt_rq)
			kfree(tg->rt_rq[i]);
		എങ്കിൽ (tg->rt_se)
			kfree(tg->rt_se[i]);
	}

	kfree(tg->rt_rq);
	kfree(tg->rt_se);
}

ശൂന്യം init_tg_rt_entry(ഘടന task_group *tg, ഘടന rt_rq *rt_rq,
		ഘടന sched_rt_entity *rt_se, പൂർണ്ണസംഖ്യ cpu,
		ഘടന sched_rt_entity *parent)
{
	ഘടന rq *rq = cpu_rq(cpu);

	rt_rq->highest_prio.curr = MAX_RT_PRIO-1;
	rt_rq->rt_nr_boosted = 0;
	rt_rq->rq = rq;
	rt_rq->tg = tg;

	tg->rt_rq[cpu] = rt_rq;
	tg->rt_se[cpu] = rt_se;

	എങ്കിൽ (!rt_se)
		തിരികെ;

	എങ്കിൽ (!parent)
		rt_se->rt_rq = &rq->rt;
	അല്ലെങ്കിൽ
		rt_se->rt_rq = parent->my_q;

	rt_se->my_q = rt_rq;
	rt_se->parent = parent;
	INIT_LIST_HEAD(&rt_se->run_list);
}

പൂർണ്ണസംഖ്യ alloc_rt_sched_group(ഘടന task_group *tg, ഘടന task_group *parent)
{
	ഘടന rt_rq *rt_rq;
	ഘടന sched_rt_entity *rt_se;
	പൂർണ്ണസംഖ്യ i;

	എങ്കിൽ (!rt_group_sched_enabled())
		തിരികെ 1;

	tg->rt_rq = kzalloc_objs(rt_rq, nr_cpu_ids);
	എങ്കിൽ (!tg->rt_rq)
		പോകുക err;
	tg->rt_se = kzalloc_objs(rt_se, nr_cpu_ids);
	എങ്കിൽ (!tg->rt_se)
		പോകുക err;

	init_rt_bandwidth(&tg->rt_bandwidth, ktime_to_ns(global_rt_period()), 0);

	for_each_possible_cpu(i) {
		rt_rq = kzalloc_കണ്ണി(വലുപ്പം(ഘടന rt_rq),
				     GFP_KERNEL, cpu_to_കണ്ണി(i));
		എങ്കിൽ (!rt_rq)
			പോകുക err;

		rt_se = kzalloc_കണ്ണി(വലുപ്പം(ഘടന sched_rt_entity),
				     GFP_KERNEL, cpu_to_കണ്ണി(i));
		എങ്കിൽ (!rt_se)
			പോകുക err_free_rq;

		init_rt_rq(rt_rq);
		rt_rq->rt_runtime = tg->rt_bandwidth.rt_runtime;
		init_tg_rt_entry(tg, rt_rq, rt_se, i, parent->rt_se[i]);
	}

	തിരികെ 1;

err_free_rq:
	kfree(rt_rq);
err:
	തിരികെ 0;
}

#അല്ലെങ്കിൽ /* !CONFIG_RT_GROUP_SCHED: */

#define rt_entity_is_task(rt_se) (1)

സ്ഥിതം വരിയിൽ ഘടന task_struct *rt_task_of(ഘടന sched_rt_entity *rt_se)
{
	തിരികെ container_of(rt_se, ഘടന task_struct, rt);
}

സ്ഥിതം വരിയിൽ ഘടന rq *rq_of_rt_rq(ഘടന rt_rq *rt_rq)
{
	തിരികെ container_of(rt_rq, ഘടന rq, rt);
}

സ്ഥിതം വരിയിൽ ഘടന rq *rq_of_rt_se(ഘടന sched_rt_entity *rt_se)
{
	ഘടന task_struct *p = rt_task_of(rt_se);

	തിരികെ task_rq(p);
}

സ്ഥിതം വരിയിൽ ഘടന rt_rq *rt_rq_of_se(ഘടന sched_rt_entity *rt_se)
{
	ഘടന rq *rq = rq_of_rt_se(rt_se);

	തിരികെ &rq->rt;
}

ശൂന്യം unregister_rt_sched_group(ഘടന task_group *tg) { }

ശൂന്യം free_rt_sched_group(ഘടന task_group *tg) { }

പൂർണ്ണസംഖ്യ alloc_rt_sched_group(ഘടന task_group *tg, ഘടന task_group *parent)
{
	തിരികെ 1;
}
#endif /* !CONFIG_RT_GROUP_SCHED */

സ്ഥിതം വരിയിൽ bool need_pull_rt_task(ഘടന rq *rq, ഘടന task_struct *prev)
{
	/* Try to pull RT tasks here if we lower this rq's prio */
	തിരികെ rq->online && rq->rt.highest_prio.curr > prev->prio;
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ rt_overloaded(ഘടന rq *rq)
{
	തിരികെ atomic_read(&rq->rd->rto_എണ്ണം);
}

സ്ഥിതം വരിയിൽ ശൂന്യം rt_set_overload(ഘടന rq *rq)
{
	എങ്കിൽ (!rq->online)
		തിരികെ;

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
	atomic_inc(&rq->rd->rto_എണ്ണം);
}

സ്ഥിതം വരിയിൽ ശൂന്യം rt_clear_overload(ഘടന rq *rq)
{
	എങ്കിൽ (!rq->online)
		തിരികെ;

	/* the order here really doesn't matter */
	atomic_dec(&rq->rd->rto_എണ്ണം);
	cpumask_clear_cpu(rq->cpu, rq->rd->rto_mask);
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ has_pushable_tasks(ഘടന rq *rq)
{
	തിരികെ !plist_head_empty(&rq->rt.pushable_tasks);
}

സ്ഥിതം DEFINE_PER_CPU(ഘടന balance_callback, rt_തള്ളുക_head);
സ്ഥിതം DEFINE_PER_CPU(ഘടന balance_callback, rt_pull_head);

സ്ഥിതം ശൂന്യം തള്ളുക_rt_tasks(ഘടന rq *);
സ്ഥിതം ശൂന്യം pull_rt_task(ഘടന rq *);

സ്ഥിതം വരിയിൽ ശൂന്യം rt_queue_തള്ളുക_tasks(ഘടന rq *rq)
{
	എങ്കിൽ (!has_pushable_tasks(rq))
		തിരികെ;

	queue_balance_callback(rq, &per_cpu(rt_തള്ളുക_head, rq->cpu), തള്ളുക_rt_tasks);
}

സ്ഥിതം വരിയിൽ ശൂന്യം rt_queue_pull_task(ഘടന rq *rq)
{
	queue_balance_callback(rq, &per_cpu(rt_pull_head, rq->cpu), pull_rt_task);
}

സ്ഥിതം ശൂന്യം enqueue_pushable_task(ഘടന rq *rq, ഘടന task_struct *p)
{
	plist_del(&p->pushable_tasks, &rq->rt.pushable_tasks);
	plist_കണ്ണി_init(&p->pushable_tasks, p->prio);
	plist_add(&p->pushable_tasks, &rq->rt.pushable_tasks);

	/* Update the highest prio pushable task */
	എങ്കിൽ (p->prio < rq->rt.highest_prio.next)
		rq->rt.highest_prio.next = p->prio;

	എങ്കിൽ (!rq->rt.overloaded) {
		rt_set_overload(rq);
		rq->rt.overloaded = 1;
	}
}

സ്ഥിതം ശൂന്യം dequeue_pushable_task(ഘടന rq *rq, ഘടന task_struct *p)
{
	plist_del(&p->pushable_tasks, &rq->rt.pushable_tasks);

	/* Update the new highest prio pushable task */
	എങ്കിൽ (has_pushable_tasks(rq)) {
		p = plist_first_entry(&rq->rt.pushable_tasks,
				      ഘടന task_struct, pushable_tasks);
		rq->rt.highest_prio.next = p->prio;
	} അല്ലെങ്കിൽ {
		rq->rt.highest_prio.next = MAX_RT_PRIO-1;

		എങ്കിൽ (rq->rt.overloaded) {
			rt_clear_overload(rq);
			rq->rt.overloaded = 0;
		}
	}
}

സ്ഥിതം ശൂന്യം enqueue_മുകൾ_rt_rq(ഘടന rt_rq *rt_rq);
സ്ഥിതം ശൂന്യം dequeue_മുകൾ_rt_rq(ഘടന rt_rq *rt_rq, unsigned പൂർണ്ണസംഖ്യ എണ്ണം);

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ on_rt_rq(ഘടന sched_rt_entity *rt_se)
{
	തിരികെ rt_se->on_rq;
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
സ്ഥിതം വരിയിൽ bool rt_task_fits_capacity(ഘടന task_struct *p, പൂർണ്ണസംഖ്യ cpu)
{
	unsigned പൂർണ്ണസംഖ്യ min_cap;
	unsigned പൂർണ്ണസംഖ്യ max_cap;
	unsigned പൂർണ്ണസംഖ്യ cpu_cap;

	/* Only heterogeneous systems can benefit from this check */
	എങ്കിൽ (!sched_asym_cpucap_active())
		തിരികെ true;

	min_cap = uclamp_eff_മൂല്യം(p, UCLAMP_MIN);
	max_cap = uclamp_eff_മൂല്യം(p, UCLAMP_MAX);

	cpu_cap = arch_scale_cpu_capacity(cpu);

	തിരികെ cpu_cap >= min(min_cap, max_cap);
}
#അല്ലെങ്കിൽ /* !CONFIG_UCLAMP_TASK: */
സ്ഥിതം വരിയിൽ bool rt_task_fits_capacity(ഘടന task_struct *p, പൂർണ്ണസംഖ്യ cpu)
{
	തിരികെ true;
}
#endif /* !CONFIG_UCLAMP_TASK */

#ifdef CONFIG_RT_GROUP_SCHED

സ്ഥിതം വരിയിൽ u64 sched_rt_runtime(ഘടന rt_rq *rt_rq)
{
	തിരികെ rt_rq->rt_runtime;
}

സ്ഥിതം വരിയിൽ u64 sched_rt_period(ഘടന rt_rq *rt_rq)
{
	തിരികെ ktime_to_ns(rt_rq->tg->rt_bandwidth.rt_period);
}

തരനിർവചനം ഘടന task_group *rt_rq_iter_t;

സ്ഥിതം വരിയിൽ ഘടന task_group *next_task_group(ഘടന task_group *tg)
{
	എങ്കിൽ (!rt_group_sched_enabled()) {
		WARN_ON(tg != &വേര്_task_group);
		തിരികെ ശൂന്യ;
	}

	ചെയ്യുക {
		tg = list_entry_rcu(tg->list.next,
			typeof(ഘടന task_group), list);
	} ആകുമ്പോൾ (&tg->list != &task_groups && task_group_is_autogroup(tg));

	എങ്കിൽ (&tg->list == &task_groups)
		tg = ശൂന്യ;

	തിരികെ tg;
}

#define for_each_rt_rq(rt_rq, iter, rq)					\
	വേണ്ടി (iter = &വേര്_task_group;					\
		iter && (rt_rq = iter->rt_rq[cpu_of(rq)]);		\
		iter = next_task_group(iter))

#define for_each_sched_rt_entity(rt_se) \
	വേണ്ടി (; rt_se; rt_se = rt_se->parent)

സ്ഥിതം വരിയിൽ ഘടന rt_rq *group_rt_rq(ഘടന sched_rt_entity *rt_se)
{
	തിരികെ rt_se->my_q;
}

സ്ഥിതം ശൂന്യം enqueue_rt_entity(ഘടന sched_rt_entity *rt_se, unsigned പൂർണ്ണസംഖ്യ flags);
സ്ഥിതം ശൂന്യം dequeue_rt_entity(ഘടന sched_rt_entity *rt_se, unsigned പൂർണ്ണസംഖ്യ flags);

സ്ഥിതം ശൂന്യം sched_rt_rq_enqueue(ഘടന rt_rq *rt_rq)
{
	ഘടന task_struct *donor = rq_of_rt_rq(rt_rq)->donor;
	ഘടന rq *rq = rq_of_rt_rq(rt_rq);
	ഘടന sched_rt_entity *rt_se;

	പൂർണ്ണസംഖ്യ cpu = cpu_of(rq);

	rt_se = rt_rq->tg->rt_se[cpu];

	എങ്കിൽ (rt_rq->rt_nr_running) {
		എങ്കിൽ (!rt_se)
			enqueue_മുകൾ_rt_rq(rt_rq);
		അല്ലെങ്കിൽ എങ്കിൽ (!on_rt_rq(rt_se))
			enqueue_rt_entity(rt_se, 0);

		എങ്കിൽ (rt_rq->highest_prio.curr < donor->prio)
			resched_curr(rq);
	}
}

സ്ഥിതം ശൂന്യം sched_rt_rq_dequeue(ഘടന rt_rq *rt_rq)
{
	ഘടന sched_rt_entity *rt_se;
	പൂർണ്ണസംഖ്യ cpu = cpu_of(rq_of_rt_rq(rt_rq));

	rt_se = rt_rq->tg->rt_se[cpu];

	എങ്കിൽ (!rt_se) {
		dequeue_മുകൾ_rt_rq(rt_rq, rt_rq->rt_nr_running);
		/* Kick cpufreq (see the comment in kernel/sched/sched.h). */
		cpufreq_update_util(rq_of_rt_rq(rt_rq), 0);
	}
	അല്ലെങ്കിൽ എങ്കിൽ (on_rt_rq(rt_se))
		dequeue_rt_entity(rt_se, 0);
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ rt_rq_throttled(ഘടന rt_rq *rt_rq)
{
	തിരികെ rt_rq->rt_throttled && !rt_rq->rt_nr_boosted;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ rt_se_boosted(ഘടന sched_rt_entity *rt_se)
{
	ഘടന rt_rq *rt_rq = group_rt_rq(rt_se);
	ഘടന task_struct *p;

	എങ്കിൽ (rt_rq)
		തിരികെ !!rt_rq->rt_nr_boosted;

	p = rt_task_of(rt_se);
	തിരികെ p->prio != p->normal_prio;
}

സ്ഥിതം വരിയിൽ സ്ഥിരം ഘടന cpumask *sched_rt_period_mask(ശൂന്യം)
{
	തിരികെ this_rq()->rd->span;
}

സ്ഥിതം വരിയിൽ
ഘടന rt_rq *sched_rt_period_rt_rq(ഘടന rt_bandwidth *rt_b, പൂർണ്ണസംഖ്യ cpu)
{
	തിരികെ container_of(rt_b, ഘടന task_group, rt_bandwidth)->rt_rq[cpu];
}

സ്ഥിതം വരിയിൽ ഘടന rt_bandwidth *sched_rt_bandwidth(ഘടന rt_rq *rt_rq)
{
	തിരികെ &rt_rq->tg->rt_bandwidth;
}

bool sched_rt_bandwidth_account(ഘടന rt_rq *rt_rq)
{
	ഘടന rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);

	തിരികെ (hrtimer_active(&rt_b->rt_period_timer) ||
		rt_rq->rt_time < rt_b->rt_runtime);
}

/*
 * We ran out of runtime, see if we can borrow some from our neighbours.
 */
സ്ഥിതം ശൂന്യം do_balance_runtime(ഘടന rt_rq *rt_rq)
{
	ഘടന rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);
	ഘടന വേര്_domain *rd = rq_of_rt_rq(rt_rq)->rd;
	പൂർണ്ണസംഖ്യ i, weight;
	u64 rt_period;

	weight = cpumask_weight(rd->span);

	raw_spin_lock(&rt_b->rt_runtime_lock);
	rt_period = ktime_to_ns(rt_b->rt_period);
	for_each_cpu(i, rd->span) {
		ഘടന rt_rq *iter = sched_rt_period_rt_rq(rt_b, i);
		s64 diff;

		എങ്കിൽ (iter == rt_rq)
			തുടരുക;

		raw_spin_lock(&iter->rt_runtime_lock);
		/*
		 * Either all rqs have inf runtime and there's nothing to steal
		 * or __disable_runtime() below sets a specific rq to inf to
		 * indicate its been disabled and disallow stealing.
		 */
		എങ്കിൽ (iter->rt_runtime == RUNTIME_INF)
			പോകുക next;

		/*
		 * From runqueues with spare time, take 1/n part of their
		 * spare time, but no more than our period.
		 */
		diff = iter->rt_runtime - iter->rt_time;
		എങ്കിൽ (diff > 0) {
			diff = div_u64((u64)diff, weight);
			എങ്കിൽ (rt_rq->rt_runtime + diff > rt_period)
				diff = rt_period - rt_rq->rt_runtime;
			iter->rt_runtime -= diff;
			rt_rq->rt_runtime += diff;
			എങ്കിൽ (rt_rq->rt_runtime == rt_period) {
				raw_spin_unlock(&iter->rt_runtime_lock);
				നിർത്തുക;
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
സ്ഥിതം ശൂന്യം __disable_runtime(ഘടന rq *rq)
{
	ഘടന വേര്_domain *rd = rq->rd;
	rt_rq_iter_t iter;
	ഘടന rt_rq *rt_rq;

	എങ്കിൽ (unlikely(!scheduler_running))
		തിരികെ;

	for_each_rt_rq(rt_rq, iter, rq) {
		ഘടന rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);
		s64 want;
		പൂർണ്ണസംഖ്യ i;

		raw_spin_lock(&rt_b->rt_runtime_lock);
		raw_spin_lock(&rt_rq->rt_runtime_lock);
		/*
		 * Either we're all inf and nobody needs to borrow, or we're
		 * already disabled and thus have nothing to do, or we have
		 * exactly the right amount of runtime to take out.
		 */
		എങ്കിൽ (rt_rq->rt_runtime == RUNTIME_INF ||
				rt_rq->rt_runtime == rt_b->rt_runtime)
			പോകുക balanced;
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
			ഘടന rt_rq *iter = sched_rt_period_rt_rq(rt_b, i);
			s64 diff;

			/*
			 * Can't reclaim from ourselves or disabled runqueues.
			 */
			എങ്കിൽ (iter == rt_rq || iter->rt_runtime == RUNTIME_INF)
				തുടരുക;

			raw_spin_lock(&iter->rt_runtime_lock);
			എങ്കിൽ (want > 0) {
				diff = min_t(s64, iter->rt_runtime, want);
				iter->rt_runtime -= diff;
				want -= diff;
			} അല്ലെങ്കിൽ {
				iter->rt_runtime -= want;
				want -= want;
			}
			raw_spin_unlock(&iter->rt_runtime_lock);

			എങ്കിൽ (!want)
				നിർത്തുക;
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

സ്ഥിതം ശൂന്യം __enable_runtime(ഘടന rq *rq)
{
	rt_rq_iter_t iter;
	ഘടന rt_rq *rt_rq;

	എങ്കിൽ (unlikely(!scheduler_running))
		തിരികെ;

	/*
	 * Reset each runqueue's bandwidth settings
	 */
	for_each_rt_rq(rt_rq, iter, rq) {
		ഘടന rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);

		raw_spin_lock(&rt_b->rt_runtime_lock);
		raw_spin_lock(&rt_rq->rt_runtime_lock);
		rt_rq->rt_runtime = rt_b->rt_runtime;
		rt_rq->rt_time = 0;
		rt_rq->rt_throttled = 0;
		raw_spin_unlock(&rt_rq->rt_runtime_lock);
		raw_spin_unlock(&rt_b->rt_runtime_lock);
	}
}

സ്ഥിതം ശൂന്യം balance_runtime(ഘടന rt_rq *rt_rq)
{
	എങ്കിൽ (!sched_feat(RT_RUNTIME_SHARE))
		തിരികെ;

	എങ്കിൽ (rt_rq->rt_time > rt_rq->rt_runtime) {
		raw_spin_unlock(&rt_rq->rt_runtime_lock);
		do_balance_runtime(rt_rq);
		raw_spin_lock(&rt_rq->rt_runtime_lock);
	}
}

സ്ഥിതം പൂർണ്ണസംഖ്യ do_sched_rt_period_timer(ഘടന rt_bandwidth *rt_b, പൂർണ്ണസംഖ്യ overrun)
{
	പൂർണ്ണസംഖ്യ i, idle = 1, throttled = 0;
	സ്ഥിരം ഘടന cpumask *span;

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
	എങ്കിൽ (rt_b == &വേര്_task_group.rt_bandwidth)
		span = cpu_online_mask;

	for_each_cpu(i, span) {
		പൂർണ്ണസംഖ്യ enqueue = 0;
		ഘടന rt_rq *rt_rq = sched_rt_period_rt_rq(rt_b, i);
		ഘടന rq *rq = rq_of_rt_rq(rt_rq);
		ഘടന rq_flags rf;
		പൂർണ്ണസംഖ്യ skip;

		/*
		 * When span == cpu_online_mask, taking each rq->lock
		 * can be time-consuming. Try to avoid it when possible.
		 */
		raw_spin_lock(&rt_rq->rt_runtime_lock);
		എങ്കിൽ (!sched_feat(RT_RUNTIME_SHARE) && rt_rq->rt_runtime != RUNTIME_INF)
			rt_rq->rt_runtime = rt_b->rt_runtime;
		skip = !rt_rq->rt_time && !rt_rq->rt_nr_running;
		raw_spin_unlock(&rt_rq->rt_runtime_lock);
		എങ്കിൽ (skip)
			തുടരുക;

		rq_lock(rq, &rf);
		update_rq_clock(rq);

		എങ്കിൽ (rt_rq->rt_time) {
			u64 runtime;

			raw_spin_lock(&rt_rq->rt_runtime_lock);
			എങ്കിൽ (rt_rq->rt_throttled)
				balance_runtime(rt_rq);
			runtime = rt_rq->rt_runtime;
			rt_rq->rt_time -= min(rt_rq->rt_time, overrun*runtime);
			എങ്കിൽ (rt_rq->rt_throttled && rt_rq->rt_time < runtime) {
				rt_rq->rt_throttled = 0;
				enqueue = 1;

				/*
				 * When we're idle and a woken (rt) task is
				 * throttled wakeup_preempt() will set
				 * skip_update and the time between the wakeup
				 * and this unthrottle will get accounted as
				 * 'runtime'.
				 */
				എങ്കിൽ (rt_rq->rt_nr_running && rq->curr == rq->idle)
					rq_clock_cancel_skipupdate(rq);
			}
			എങ്കിൽ (rt_rq->rt_time || rt_rq->rt_nr_running)
				idle = 0;
			raw_spin_unlock(&rt_rq->rt_runtime_lock);
		} അല്ലെങ്കിൽ എങ്കിൽ (rt_rq->rt_nr_running) {
			idle = 0;
			എങ്കിൽ (!rt_rq_throttled(rt_rq))
				enqueue = 1;
		}
		എങ്കിൽ (rt_rq->rt_throttled)
			throttled = 1;

		എങ്കിൽ (enqueue)
			sched_rt_rq_enqueue(rt_rq);
		rq_unlock(rq, &rf);
	}

	എങ്കിൽ (!throttled && (!rt_bandwidth_enabled() || rt_b->rt_runtime == RUNTIME_INF))
		തിരികെ 1;

	തിരികെ idle;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ sched_rt_runtime_exceeded(ഘടന rt_rq *rt_rq)
{
	u64 runtime = sched_rt_runtime(rt_rq);

	എങ്കിൽ (rt_rq->rt_throttled)
		തിരികെ rt_rq_throttled(rt_rq);

	എങ്കിൽ (runtime >= sched_rt_period(rt_rq))
		തിരികെ 0;

	balance_runtime(rt_rq);
	runtime = sched_rt_runtime(rt_rq);
	എങ്കിൽ (runtime == RUNTIME_INF)
		തിരികെ 0;

	എങ്കിൽ (rt_rq->rt_time > runtime) {
		ഘടന rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);

		/*
		 * Don't actually throttle groups that have no runtime assigned
		 * but accrue some time due to boosting.
		 */
		എങ്കിൽ (likely(rt_b->rt_runtime)) {
			rt_rq->rt_throttled = 1;
			printk_deferred_once("sched: RT throttling activated\n");
		} അല്ലെങ്കിൽ {
			/*
			 * In case we did anyway, make it go away,
			 * replenishment is a joke, since it will replenish us
			 * with exactly 0 ns.
			 */
			rt_rq->rt_time = 0;
		}

		എങ്കിൽ (rt_rq_throttled(rt_rq)) {
			sched_rt_rq_dequeue(rt_rq);
			തിരികെ 1;
		}
	}

	തിരികെ 0;
}

#അല്ലെങ്കിൽ /* !CONFIG_RT_GROUP_SCHED: */

തരനിർവചനം ഘടന rt_rq *rt_rq_iter_t;

#define for_each_rt_rq(rt_rq, iter, rq) \
	വേണ്ടി ((ശൂന്യം) iter, rt_rq = &rq->rt; rt_rq; rt_rq = ശൂന്യ)

#define for_each_sched_rt_entity(rt_se) \
	വേണ്ടി (; rt_se; rt_se = ശൂന്യ)

സ്ഥിതം വരിയിൽ ഘടന rt_rq *group_rt_rq(ഘടന sched_rt_entity *rt_se)
{
	തിരികെ ശൂന്യ;
}

സ്ഥിതം വരിയിൽ ശൂന്യം sched_rt_rq_enqueue(ഘടന rt_rq *rt_rq)
{
	ഘടന rq *rq = rq_of_rt_rq(rt_rq);

	എങ്കിൽ (!rt_rq->rt_nr_running)
		തിരികെ;

	enqueue_മുകൾ_rt_rq(rt_rq);
	resched_curr(rq);
}

സ്ഥിതം വരിയിൽ ശൂന്യം sched_rt_rq_dequeue(ഘടന rt_rq *rt_rq)
{
	dequeue_മുകൾ_rt_rq(rt_rq, rt_rq->rt_nr_running);
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ rt_rq_throttled(ഘടന rt_rq *rt_rq)
{
	തിരികെ false;
}

സ്ഥിതം വരിയിൽ സ്ഥിരം ഘടന cpumask *sched_rt_period_mask(ശൂന്യം)
{
	തിരികെ cpu_online_mask;
}

സ്ഥിതം വരിയിൽ
ഘടന rt_rq *sched_rt_period_rt_rq(ഘടന rt_bandwidth *rt_b, പൂർണ്ണസംഖ്യ cpu)
{
	തിരികെ &cpu_rq(cpu)->rt;
}

സ്ഥിതം ശൂന്യം __enable_runtime(ഘടന rq *rq) { }
സ്ഥിതം ശൂന്യം __disable_runtime(ഘടന rq *rq) { }

#endif /* !CONFIG_RT_GROUP_SCHED */

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ rt_se_prio(ഘടന sched_rt_entity *rt_se)
{
#ifdef CONFIG_RT_GROUP_SCHED
	ഘടന rt_rq *rt_rq = group_rt_rq(rt_se);

	എങ്കിൽ (rt_rq)
		തിരികെ rt_rq->highest_prio.curr;
#endif

	തിരികെ rt_task_of(rt_se)->prio;
}

/*
 * Update the current task's runtime statistics. Skip current tasks that
 * are not in our scheduling class.
 */
സ്ഥിതം ശൂന്യം update_curr_rt(ഘടന rq *rq)
{
	ഘടന task_struct *donor = rq->donor;
	s64 delta_exec;

	എങ്കിൽ (donor->sched_class != &rt_sched_class)
		തിരികെ;

	delta_exec = update_curr_common(rq);
	എങ്കിൽ (unlikely(delta_exec <= 0))
		തിരികെ;

#ifdef CONFIG_RT_GROUP_SCHED
	ഘടന sched_rt_entity *rt_se = &donor->rt;

	എങ്കിൽ (!rt_bandwidth_enabled())
		തിരികെ;

	for_each_sched_rt_entity(rt_se) {
		ഘടന rt_rq *rt_rq = rt_rq_of_se(rt_se);
		പൂർണ്ണസംഖ്യ exceeded;

		എങ്കിൽ (sched_rt_runtime(rt_rq) != RUNTIME_INF) {
			raw_spin_lock(&rt_rq->rt_runtime_lock);
			rt_rq->rt_time += delta_exec;
			exceeded = sched_rt_runtime_exceeded(rt_rq);
			എങ്കിൽ (exceeded)
				resched_curr(rq);
			raw_spin_unlock(&rt_rq->rt_runtime_lock);
			എങ്കിൽ (exceeded)
				do_start_rt_bandwidth(sched_rt_bandwidth(rt_rq));
		}
	}
#endif /* CONFIG_RT_GROUP_SCHED */
}

സ്ഥിതം ശൂന്യം
dequeue_മുകൾ_rt_rq(ഘടന rt_rq *rt_rq, unsigned പൂർണ്ണസംഖ്യ എണ്ണം)
{
	ഘടന rq *rq = rq_of_rt_rq(rt_rq);

	BUG_ON(&rq->rt != rt_rq);

	എങ്കിൽ (!rt_rq->rt_queued)
		തിരികെ;

	BUG_ON(!rq->nr_running);

	sub_nr_running(rq, എണ്ണം);
	rt_rq->rt_queued = 0;

}

സ്ഥിതം ശൂന്യം
enqueue_മുകൾ_rt_rq(ഘടന rt_rq *rt_rq)
{
	ഘടന rq *rq = rq_of_rt_rq(rt_rq);

	BUG_ON(&rq->rt != rt_rq);

	എങ്കിൽ (rt_rq->rt_queued)
		തിരികെ;

	എങ്കിൽ (rt_rq_throttled(rt_rq))
		തിരികെ;

	എങ്കിൽ (rt_rq->rt_nr_running) {
		add_nr_running(rq, rt_rq->rt_nr_running);
		rt_rq->rt_queued = 1;
	}

	/* Kick cpufreq (see the comment in kernel/sched/sched.h). */
	cpufreq_update_util(rq, 0);
}

സ്ഥിതം ശൂന്യം
inc_rt_prio_smp(ഘടന rt_rq *rt_rq, പൂർണ്ണസംഖ്യ prio, പൂർണ്ണസംഖ്യ prev_prio)
{
	ഘടന rq *rq = rq_of_rt_rq(rt_rq);

	/*
	 * Change rq's cpupri only if rt_rq is the top queue.
	 */
	എങ്കിൽ (IS_ENABLED(CONFIG_RT_GROUP_SCHED) && &rq->rt != rt_rq)
		തിരികെ;

	എങ്കിൽ (rq->online && prio < prev_prio)
		cpupri_set(&rq->rd->cpupri, rq->cpu, prio);
}

സ്ഥിതം ശൂന്യം
dec_rt_prio_smp(ഘടന rt_rq *rt_rq, പൂർണ്ണസംഖ്യ prio, പൂർണ്ണസംഖ്യ prev_prio)
{
	ഘടന rq *rq = rq_of_rt_rq(rt_rq);

	/*
	 * Change rq's cpupri only if rt_rq is the top queue.
	 */
	എങ്കിൽ (IS_ENABLED(CONFIG_RT_GROUP_SCHED) && &rq->rt != rt_rq)
		തിരികെ;

	എങ്കിൽ (rq->online && rt_rq->highest_prio.curr != prev_prio)
		cpupri_set(&rq->rd->cpupri, rq->cpu, rt_rq->highest_prio.curr);
}

സ്ഥിതം ശൂന്യം
inc_rt_prio(ഘടന rt_rq *rt_rq, പൂർണ്ണസംഖ്യ prio)
{
	പൂർണ്ണസംഖ്യ prev_prio = rt_rq->highest_prio.curr;

	എങ്കിൽ (prio < prev_prio)
		rt_rq->highest_prio.curr = prio;

	inc_rt_prio_smp(rt_rq, prio, prev_prio);
}

സ്ഥിതം ശൂന്യം
dec_rt_prio(ഘടന rt_rq *rt_rq, പൂർണ്ണസംഖ്യ prio)
{
	പൂർണ്ണസംഖ്യ prev_prio = rt_rq->highest_prio.curr;

	എങ്കിൽ (rt_rq->rt_nr_running) {

		WARN_ON(prio < prev_prio);

		/*
		 * This may have been our highest task, and therefore
		 * we may have some re-computation to do
		 */
		എങ്കിൽ (prio == prev_prio) {
			ഘടന rt_prio_array *array = &rt_rq->active;

			rt_rq->highest_prio.curr =
				sched_find_first_bit(array->bitmap);
		}

	} അല്ലെങ്കിൽ {
		rt_rq->highest_prio.curr = MAX_RT_PRIO-1;
	}

	dec_rt_prio_smp(rt_rq, prio, prev_prio);
}

#ifdef CONFIG_RT_GROUP_SCHED

സ്ഥിതം ശൂന്യം
inc_rt_group(ഘടന sched_rt_entity *rt_se, ഘടന rt_rq *rt_rq)
{
	എങ്കിൽ (rt_se_boosted(rt_se))
		rt_rq->rt_nr_boosted++;

	start_rt_bandwidth(&rt_rq->tg->rt_bandwidth);
}

സ്ഥിതം ശൂന്യം
dec_rt_group(ഘടന sched_rt_entity *rt_se, ഘടന rt_rq *rt_rq)
{
	എങ്കിൽ (rt_se_boosted(rt_se))
		rt_rq->rt_nr_boosted--;

	WARN_ON(!rt_rq->rt_nr_running && rt_rq->rt_nr_boosted);
}

#അല്ലെങ്കിൽ /* !CONFIG_RT_GROUP_SCHED: */

സ്ഥിതം ശൂന്യം
inc_rt_group(ഘടന sched_rt_entity *rt_se, ഘടന rt_rq *rt_rq)
{
}

സ്ഥിതം വരിയിൽ
ശൂന്യം dec_rt_group(ഘടന sched_rt_entity *rt_se, ഘടന rt_rq *rt_rq) {}

#endif /* !CONFIG_RT_GROUP_SCHED */

സ്ഥിതം വരിയിൽ
unsigned പൂർണ്ണസംഖ്യ rt_se_nr_running(ഘടന sched_rt_entity *rt_se)
{
	ഘടന rt_rq *group_rq = group_rt_rq(rt_se);

	എങ്കിൽ (group_rq)
		തിരികെ group_rq->rt_nr_running;
	അല്ലെങ്കിൽ
		തിരികെ 1;
}

സ്ഥിതം വരിയിൽ
unsigned പൂർണ്ണസംഖ്യ rt_se_rr_nr_running(ഘടന sched_rt_entity *rt_se)
{
	ഘടന rt_rq *group_rq = group_rt_rq(rt_se);
	ഘടന task_struct *tsk;

	എങ്കിൽ (group_rq)
		തിരികെ group_rq->rr_nr_running;

	tsk = rt_task_of(rt_se);

	തിരികെ (tsk->policy == SCHED_RR) ? 1 : 0;
}

സ്ഥിതം വരിയിൽ
ശൂന്യം inc_rt_tasks(ഘടന sched_rt_entity *rt_se, ഘടന rt_rq *rt_rq)
{
	പൂർണ്ണസംഖ്യ prio = rt_se_prio(rt_se);

	WARN_ON(!rt_prio(prio));
	rt_rq->rt_nr_running += rt_se_nr_running(rt_se);
	rt_rq->rr_nr_running += rt_se_rr_nr_running(rt_se);

	inc_rt_prio(rt_rq, prio);
	inc_rt_group(rt_se, rt_rq);
}

സ്ഥിതം വരിയിൽ
ശൂന്യം dec_rt_tasks(ഘടന sched_rt_entity *rt_se, ഘടന rt_rq *rt_rq)
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
സ്ഥിതം വരിയിൽ bool move_entity(unsigned പൂർണ്ണസംഖ്യ flags)
{
	എങ്കിൽ ((flags & (DEQUEUE_SAVE | DEQUEUE_MOVE)) == DEQUEUE_SAVE)
		തിരികെ false;

	തിരികെ true;
}

സ്ഥിതം ശൂന്യം __delist_rt_entity(ഘടന sched_rt_entity *rt_se, ഘടന rt_prio_array *array)
{
	list_del_init(&rt_se->run_list);

	എങ്കിൽ (list_empty(array->queue + rt_se_prio(rt_se)))
		__clear_bit(rt_se_prio(rt_se), array->bitmap);

	rt_se->on_list = 0;
}

സ്ഥിതം വരിയിൽ ഘടന sched_statistics *
__schedstats_from_rt_se(ഘടന sched_rt_entity *rt_se)
{
	/* schedstats is not supported for rt group. */
	എങ്കിൽ (!rt_entity_is_task(rt_se))
		തിരികെ ശൂന്യ;

	തിരികെ &rt_task_of(rt_se)->stats;
}

സ്ഥിതം വരിയിൽ ശൂന്യം
update_stats_wait_start_rt(ഘടന rt_rq *rt_rq, ഘടന sched_rt_entity *rt_se)
{
	ഘടന sched_statistics *stats;
	ഘടന task_struct *p = ശൂന്യ;

	എങ്കിൽ (!schedstat_enabled())
		തിരികെ;

	എങ്കിൽ (rt_entity_is_task(rt_se))
		p = rt_task_of(rt_se);

	stats = __schedstats_from_rt_se(rt_se);
	എങ്കിൽ (!stats)
		തിരികെ;

	__update_stats_wait_start(rq_of_rt_rq(rt_rq), p, stats);
}

സ്ഥിതം വരിയിൽ ശൂന്യം
update_stats_enqueue_sleeper_rt(ഘടന rt_rq *rt_rq, ഘടന sched_rt_entity *rt_se)
{
	ഘടന sched_statistics *stats;
	ഘടന task_struct *p = ശൂന്യ;

	എങ്കിൽ (!schedstat_enabled())
		തിരികെ;

	എങ്കിൽ (rt_entity_is_task(rt_se))
		p = rt_task_of(rt_se);

	stats = __schedstats_from_rt_se(rt_se);
	എങ്കിൽ (!stats)
		തിരികെ;

	__update_stats_enqueue_sleeper(rq_of_rt_rq(rt_rq), p, stats);
}

സ്ഥിതം വരിയിൽ ശൂന്യം
update_stats_enqueue_rt(ഘടന rt_rq *rt_rq, ഘടന sched_rt_entity *rt_se,
			പൂർണ്ണസംഖ്യ flags)
{
	എങ്കിൽ (!schedstat_enabled())
		തിരികെ;

	എങ്കിൽ (flags & ENQUEUE_WAKEUP)
		update_stats_enqueue_sleeper_rt(rt_rq, rt_se);
}

സ്ഥിതം വരിയിൽ ശൂന്യം
update_stats_wait_end_rt(ഘടന rt_rq *rt_rq, ഘടന sched_rt_entity *rt_se)
{
	ഘടന sched_statistics *stats;
	ഘടന task_struct *p = ശൂന്യ;

	എങ്കിൽ (!schedstat_enabled())
		തിരികെ;

	എങ്കിൽ (rt_entity_is_task(rt_se))
		p = rt_task_of(rt_se);

	stats = __schedstats_from_rt_se(rt_se);
	എങ്കിൽ (!stats)
		തിരികെ;

	__update_stats_wait_end(rq_of_rt_rq(rt_rq), p, stats);
}

സ്ഥിതം വരിയിൽ ശൂന്യം
update_stats_dequeue_rt(ഘടന rt_rq *rt_rq, ഘടന sched_rt_entity *rt_se,
			പൂർണ്ണസംഖ്യ flags)
{
	ഘടന task_struct *p = ശൂന്യ;
	ഘടന rq *rq = rq_of_rt_rq(rt_rq);

	എങ്കിൽ (!schedstat_enabled())
		തിരികെ;

	എങ്കിൽ (rt_entity_is_task(rt_se)) {
		p = rt_task_of(rt_se);

		എങ്കിൽ (p != rq->curr)
			update_stats_wait_end_rt(rt_rq, rt_se);
	}

	എങ്കിൽ ((flags & DEQUEUE_SLEEP) && p) {
		unsigned പൂർണ്ണസംഖ്യ state;

		state = READ_ONCE(p->__state);
		എങ്കിൽ (state & TASK_INTERRUPTIBLE)
			__schedstat_set(p->stats.sleep_start,
					rq_clock(rq_of_rt_rq(rt_rq)));

		എങ്കിൽ (state & TASK_UNINTERRUPTIBLE)
			__schedstat_set(p->stats.block_start,
					rq_clock(rq_of_rt_rq(rt_rq)));
	}
}

സ്ഥിതം ശൂന്യം __enqueue_rt_entity(ഘടന sched_rt_entity *rt_se, unsigned പൂർണ്ണസംഖ്യ flags)
{
	ഘടന rt_rq *rt_rq = rt_rq_of_se(rt_se);
	ഘടന rt_prio_array *array = &rt_rq->active;
	ഘടന rt_rq *group_rq = group_rt_rq(rt_se);
	ഘടന list_head *queue = array->queue + rt_se_prio(rt_se);

	/*
	 * Don't enqueue the group if its throttled, or when empty.
	 * The latter is a consequence of the former when a child group
	 * get throttled and the current group doesn't have any other
	 * active members.
	 */
	എങ്കിൽ (group_rq && (rt_rq_throttled(group_rq) || !group_rq->rt_nr_running)) {
		എങ്കിൽ (rt_se->on_list)
			__delist_rt_entity(rt_se, array);
		തിരികെ;
	}

	എങ്കിൽ (move_entity(flags)) {
		WARN_ON_ONCE(rt_se->on_list);
		എങ്കിൽ (flags & ENQUEUE_HEAD)
			list_add(&rt_se->run_list, queue);
		അല്ലെങ്കിൽ
			list_add_tail(&rt_se->run_list, queue);

		__set_bit(rt_se_prio(rt_se), array->bitmap);
		rt_se->on_list = 1;
	}
	rt_se->on_rq = 1;

	inc_rt_tasks(rt_se, rt_rq);
}

സ്ഥിതം ശൂന്യം __dequeue_rt_entity(ഘടന sched_rt_entity *rt_se, unsigned പൂർണ്ണസംഖ്യ flags)
{
	ഘടന rt_rq *rt_rq = rt_rq_of_se(rt_se);
	ഘടന rt_prio_array *array = &rt_rq->active;

	എങ്കിൽ (move_entity(flags)) {
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
സ്ഥിതം ശൂന്യം dequeue_rt_അട്ടി(ഘടന sched_rt_entity *rt_se, unsigned പൂർണ്ണസംഖ്യ flags)
{
	ഘടന sched_rt_entity *back = ശൂന്യ;
	unsigned പൂർണ്ണസംഖ്യ rt_nr_running;

	for_each_sched_rt_entity(rt_se) {
		rt_se->back = back;
		back = rt_se;
	}

	rt_nr_running = rt_rq_of_se(back)->rt_nr_running;

	വേണ്ടി (rt_se = back; rt_se; rt_se = rt_se->back) {
		എങ്കിൽ (on_rt_rq(rt_se))
			__dequeue_rt_entity(rt_se, flags);
	}

	dequeue_മുകൾ_rt_rq(rt_rq_of_se(back), rt_nr_running);
}

സ്ഥിതം ശൂന്യം enqueue_rt_entity(ഘടന sched_rt_entity *rt_se, unsigned പൂർണ്ണസംഖ്യ flags)
{
	ഘടന rq *rq = rq_of_rt_se(rt_se);

	update_stats_enqueue_rt(rt_rq_of_se(rt_se), rt_se, flags);

	dequeue_rt_അട്ടി(rt_se, flags);
	for_each_sched_rt_entity(rt_se)
		__enqueue_rt_entity(rt_se, flags);
	enqueue_മുകൾ_rt_rq(&rq->rt);
}

സ്ഥിതം ശൂന്യം dequeue_rt_entity(ഘടന sched_rt_entity *rt_se, unsigned പൂർണ്ണസംഖ്യ flags)
{
	ഘടന rq *rq = rq_of_rt_se(rt_se);

	update_stats_dequeue_rt(rt_rq_of_se(rt_se), rt_se, flags);

	dequeue_rt_അട്ടി(rt_se, flags);

	for_each_sched_rt_entity(rt_se) {
		ഘടന rt_rq *rt_rq = group_rt_rq(rt_se);

		എങ്കിൽ (rt_rq && rt_rq->rt_nr_running)
			__enqueue_rt_entity(rt_se, flags);
	}
	enqueue_മുകൾ_rt_rq(&rq->rt);
}

/*
 * Adding/removing a task to/from a priority array:
 */
സ്ഥിതം ശൂന്യം
enqueue_task_rt(ഘടന rq *rq, ഘടന task_struct *p, പൂർണ്ണസംഖ്യ flags)
{
	ഘടന sched_rt_entity *rt_se = &p->rt;

	എങ്കിൽ (flags & ENQUEUE_WAKEUP)
		rt_se->timeout = 0;

	check_schedstat_required();
	update_stats_wait_start_rt(rt_rq_of_se(rt_se), rt_se);

	enqueue_rt_entity(rt_se, flags);

	എങ്കിൽ (task_is_blocked(p))
		തിരികെ;

	എങ്കിൽ (!task_current(rq, p) && p->nr_cpus_allowed > 1)
		enqueue_pushable_task(rq, p);
}

സ്ഥിതം bool dequeue_task_rt(ഘടന rq *rq, ഘടന task_struct *p, പൂർണ്ണസംഖ്യ flags)
{
	ഘടന sched_rt_entity *rt_se = &p->rt;

	update_curr_rt(rq);
	dequeue_rt_entity(rt_se, flags);

	dequeue_pushable_task(rq, p);

	തിരികെ true;
}

/*
 * Put task to the head or the end of the run list without the overhead of
 * dequeue followed by enqueue.
 */
സ്ഥിതം ശൂന്യം
requeue_rt_entity(ഘടന rt_rq *rt_rq, ഘടന sched_rt_entity *rt_se, പൂർണ്ണസംഖ്യ head)
{
	എങ്കിൽ (on_rt_rq(rt_se)) {
		ഘടന rt_prio_array *array = &rt_rq->active;
		ഘടന list_head *queue = array->queue + rt_se_prio(rt_se);

		എങ്കിൽ (head)
			list_move(&rt_se->run_list, queue);
		അല്ലെങ്കിൽ
			list_move_tail(&rt_se->run_list, queue);
	}
}

സ്ഥിതം ശൂന്യം requeue_task_rt(ഘടന rq *rq, ഘടന task_struct *p, പൂർണ്ണസംഖ്യ head)
{
	ഘടന sched_rt_entity *rt_se = &p->rt;
	ഘടന rt_rq *rt_rq;

	for_each_sched_rt_entity(rt_se) {
		rt_rq = rt_rq_of_se(rt_se);
		requeue_rt_entity(rt_rq, rt_se, head);
	}
}

സ്ഥിതം ശൂന്യം yield_task_rt(ഘടന rq *rq)
{
	requeue_task_rt(rq, rq->donor, 0);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ find_lowest_rq(ഘടന task_struct *task);

സ്ഥിതം പൂർണ്ണസംഖ്യ
select_task_rq_rt(ഘടന task_struct *p, പൂർണ്ണസംഖ്യ cpu, പൂർണ്ണസംഖ്യ flags)
{
	ഘടന task_struct *curr, *donor;
	ഘടന rq *rq;
	bool test;

	/* For anything but wake ups, just return the task_cpu */
	എങ്കിൽ (!(flags & (WF_TTWU | WF_FORK)))
		പോകുക out;

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

	എങ്കിൽ (test || !rt_task_fits_capacity(p, cpu)) {
		പൂർണ്ണസംഖ്യ target = find_lowest_rq(p);

		/*
		 * Bail out if we were forcing a migration to find a better
		 * fitting CPU but our search failed.
		 */
		എങ്കിൽ (!test && target != -1 && !rt_task_fits_capacity(p, target))
			പോകുക out_unlock;

		/*
		 * Don't bother moving it if the destination CPU is
		 * not running a lower priority task.
		 */
		എങ്കിൽ (target != -1 &&
		    p->prio < cpu_rq(target)->rt.highest_prio.curr)
			cpu = target;
	}

out_unlock:
	rcu_read_unlock();

out:
	തിരികെ cpu;
}

സ്ഥിതം ശൂന്യം check_preempt_equal_prio(ഘടന rq *rq, ഘടന task_struct *p)
{
	എങ്കിൽ (rq->curr->nr_cpus_allowed == 1 ||
	    !cpupri_find(&rq->rd->cpupri, rq->donor, ശൂന്യ))
		തിരികെ;

	/*
	 * p is migratable, so let's not schedule it and
	 * see if it is pushed or pulled somewhere else.
	 */
	എങ്കിൽ (p->nr_cpus_allowed != 1 &&
	    cpupri_find(&rq->rd->cpupri, p, ശൂന്യ))
		തിരികെ;

	/*
	 * There appear to be other CPUs that can accept
	 * the current task but none can run 'p', so lets reschedule
	 * to try and push the current task away:
	 */
	requeue_task_rt(rq, p, 1);
	resched_curr(rq);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ balance_rt(ഘടന rq *rq, ഘടന rq_flags *rf)
{
	/*
	 * Note, rq->donor may change during rq lock drops,
	 * so don't re-use p across lock drops
	 */
	ഘടന task_struct *p = rq->donor;

	എങ്കിൽ (!on_rt_rq(&p->rt) && need_pull_rt_task(rq, p)) {
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

	തിരികെ sched_stop_runnable(rq) || sched_dl_runnable(rq) || sched_rt_runnable(rq);
}

/*
 * Preempt the current task with a newly woken task if needed:
 */
സ്ഥിതം ശൂന്യം wakeup_preempt_rt(ഘടന rq *rq, ഘടന task_struct *p, പൂർണ്ണസംഖ്യ flags)
{
	ഘടന task_struct *donor = rq->donor;

	/*
	 * XXX If we're preempted by DL, queue a push?
	 */
	എങ്കിൽ (p->sched_class != &rt_sched_class ||
	    donor->sched_class != &rt_sched_class)
		തിരികെ;

	എങ്കിൽ (p->prio < donor->prio) {
		resched_curr(rq);
		തിരികെ;
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
	എങ്കിൽ (p->prio == donor->prio && !test_tsk_need_resched(rq->curr))
		check_preempt_equal_prio(rq, p);
}

സ്ഥിതം വരിയിൽ ശൂന്യം set_next_task_rt(ഘടന rq *rq, ഘടന task_struct *p, എണ്ണൽ snt_e type)
{
	ഘടന sched_rt_entity *rt_se = &p->rt;
	ഘടന rt_rq *rt_rq = &rq->rt;

	എങ്കിൽ (type == SNT_REPICK)
		തിരികെ;

	p->se.exec_start = rq_clock_task(rq);
	എങ്കിൽ (on_rt_rq(&p->rt))
		update_stats_wait_end_rt(rt_rq, rt_se);

	/* The running task is never eligible for pushing */
	dequeue_pushable_task(rq, p);

	എങ്കിൽ (type != SNT_PICK)
		തിരികെ;

	/*
	 * If prev task was rt, put_prev_task() has already updated the
	 * utilization. We only care of the case where we start to schedule a
	 * rt task
	 */
	എങ്കിൽ (rq->donor->sched_class != &rt_sched_class)
		update_rt_rq_load_avg(rq_clock_pelt(rq), rq, 0);

	rt_queue_തള്ളുക_tasks(rq);
}

സ്ഥിതം ഘടന sched_rt_entity *pick_next_rt_entity(ഘടന rt_rq *rt_rq)
{
	ഘടന rt_prio_array *array = &rt_rq->active;
	ഘടന sched_rt_entity *next = ശൂന്യ;
	ഘടന list_head *queue;
	പൂർണ്ണസംഖ്യ idx;

	idx = sched_find_first_bit(array->bitmap);
	BUG_ON(idx >= MAX_RT_PRIO);

	queue = array->queue + idx;
	എങ്കിൽ (WARN_ON_ONCE(list_empty(queue)))
		തിരികെ ശൂന്യ;
	next = list_entry(queue->next, ഘടന sched_rt_entity, run_list);

	തിരികെ next;
}

സ്ഥിതം ഘടന task_struct *_pick_next_task_rt(ഘടന rq *rq)
{
	ഘടന sched_rt_entity *rt_se;
	ഘടന rt_rq *rt_rq  = &rq->rt;

	ചെയ്യുക {
		rt_se = pick_next_rt_entity(rt_rq);
		എങ്കിൽ (unlikely(!rt_se))
			തിരികെ ശൂന്യ;
		rt_rq = group_rt_rq(rt_se);
	} ആകുമ്പോൾ (rt_rq);

	തിരികെ rt_task_of(rt_se);
}

സ്ഥിതം ഘടന task_struct *pick_task_rt(ഘടന rq *rq, ഘടന rq_flags *rf)
{
	ഘടന task_struct *p;

	എങ്കിൽ (!sched_rt_runnable(rq))
		തിരികെ ശൂന്യ;

	p = _pick_next_task_rt(rq);

	തിരികെ p;
}

സ്ഥിതം ശൂന്യം put_prev_task_rt(ഘടന rq *rq, ഘടന task_struct *p, ഘടന task_struct *next)
{
	ഘടന sched_rt_entity *rt_se = &p->rt;
	ഘടന rt_rq *rt_rq = &rq->rt;

	എങ്കിൽ (on_rt_rq(&p->rt))
		update_stats_wait_start_rt(rt_rq, rt_se);

	update_curr_rt(rq);

	update_rt_rq_load_avg(rq_clock_pelt(rq), rq, 1);

	എങ്കിൽ (task_is_blocked(p))
		തിരികെ;
	/*
	 * The previous task needs to be made eligible for pushing
	 * if it is still active
	 */
	എങ്കിൽ (on_rt_rq(&p->rt) && p->nr_cpus_allowed > 1)
		enqueue_pushable_task(rq, p);
}

/* Only try algorithms three times */
#define RT_MAX_TRIES 3

/*
 * Return the highest pushable rq's task, which is suitable to be executed
 * on the CPU, NULL otherwise
 */
സ്ഥിതം ഘടന task_struct *pick_highest_pushable_task(ഘടന rq *rq, പൂർണ്ണസംഖ്യ cpu)
{
	ഘടന plist_head *head = &rq->rt.pushable_tasks;
	ഘടന task_struct *p;

	എങ്കിൽ (!has_pushable_tasks(rq))
		തിരികെ ശൂന്യ;

	plist_for_each_entry(p, head, pushable_tasks) {
		എങ്കിൽ (task_is_pushable(rq, p, cpu))
			തിരികെ p;
	}

	തിരികെ ശൂന്യ;
}

സ്ഥിതം DEFINE_PER_CPU(cpumask_var_t, local_cpu_mask);

സ്ഥിതം പൂർണ്ണസംഖ്യ find_lowest_rq(ഘടന task_struct *task)
{
	ഘടന sched_domain *sd;
	ഘടന cpumask *lowest_mask = this_cpu_cpumask_var_ptr(local_cpu_mask);
	പൂർണ്ണസംഖ്യ this_cpu = smp_processor_id();
	പൂർണ്ണസംഖ്യ cpu      = task_cpu(task);
	പൂർണ്ണസംഖ്യ ret;

	/* Make sure the mask is initialized first */
	എങ്കിൽ (unlikely(!lowest_mask))
		തിരികെ -1;

	എങ്കിൽ (task->nr_cpus_allowed == 1)
		തിരികെ -1; /* No other targets possible */

	/*
	 * If we're on asym system ensure we consider the different capacities
	 * of the CPUs when searching for the lowest_mask.
	 */
	എങ്കിൽ (sched_asym_cpucap_active()) {

		ret = cpupri_find_fitness(&task_rq(task)->rd->cpupri,
					  task, lowest_mask,
					  rt_task_fits_capacity);
	} അല്ലെങ്കിൽ {

		ret = cpupri_find(&task_rq(task)->rd->cpupri,
				  task, lowest_mask);
	}

	എങ്കിൽ (!ret)
		തിരികെ -1; /* No targets found */

	/*
	 * At this point we have built a mask of CPUs representing the
	 * lowest priority tasks in the system.  Now we want to elect
	 * the best one based on our affinity and topology.
	 *
	 * We prioritize the last CPU that the task executed on since
	 * it is most likely cache-hot in that location.
	 */
	എങ്കിൽ (cpumask_test_cpu(cpu, lowest_mask))
		തിരികെ cpu;

	/*
	 * Otherwise, we consult the sched_domains span maps to figure
	 * out which CPU is logically closest to our hot cache data.
	 */
	എങ്കിൽ (!cpumask_test_cpu(this_cpu, lowest_mask))
		this_cpu = -1; /* Skip this_cpu opt if not among lowest */

	rcu_read_lock();
	for_each_domain(cpu, sd) {
		എങ്കിൽ (sd->flags & SD_WAKE_AFFINE) {
			പൂർണ്ണസംഖ്യ best_cpu;

			/*
			 * "this_cpu" is cheaper to preempt than a
			 * remote processor.
			 */
			എങ്കിൽ (this_cpu != -1 &&
			    cpumask_test_cpu(this_cpu, sched_domain_span(sd))) {
				rcu_read_unlock();
				തിരികെ this_cpu;
			}

			best_cpu = cpumask_any_and_distribute(lowest_mask,
							      sched_domain_span(sd));
			എങ്കിൽ (best_cpu < nr_cpu_ids) {
				rcu_read_unlock();
				തിരികെ best_cpu;
			}
		}
	}
	rcu_read_unlock();

	/*
	 * And finally, if there were no matches within the domains
	 * just give the caller *something* to work with from the compatible
	 * locations.
	 */
	എങ്കിൽ (this_cpu != -1)
		തിരികെ this_cpu;

	cpu = cpumask_any_distribute(lowest_mask);
	എങ്കിൽ (cpu < nr_cpu_ids)
		തിരികെ cpu;

	തിരികെ -1;
}

സ്ഥിതം ഘടന task_struct *pick_next_pushable_task(ഘടന rq *rq)
{
	ഘടന plist_head *head = &rq->rt.pushable_tasks;
	ഘടന task_struct *i, *p = ശൂന്യ;

	എങ്കിൽ (!has_pushable_tasks(rq))
		തിരികെ ശൂന്യ;

	plist_for_each_entry(i, head, pushable_tasks) {
		/* skip tasks that cannot be migrated */
		എങ്കിൽ (!task_on_cpu(rq, i) && !is_migration_disabled(i)) {
			p = i;
			നിർത്തുക;
		}
	}

	എങ്കിൽ (!p)
		തിരികെ ശൂന്യ;

	BUG_ON(rq->cpu != task_cpu(p));
	BUG_ON(task_current(rq, p));
	BUG_ON(task_current_donor(rq, p));
	BUG_ON(p->nr_cpus_allowed <= 1);

	BUG_ON(!task_on_rq_queued(p));
	BUG_ON(!rt_task(p));

	തിരികെ p;
}

/* Will lock the rq it finds */
സ്ഥിതം ഘടന rq *find_lock_lowest_rq(ഘടന task_struct *task, ഘടന rq *rq)
{
	ഘടന rq *lowest_rq = ശൂന്യ;
	പൂർണ്ണസംഖ്യ tries;
	പൂർണ്ണസംഖ്യ cpu;

	വേണ്ടി (tries = 0; tries < RT_MAX_TRIES; tries++) {
		cpu = find_lowest_rq(task);

		എങ്കിൽ ((cpu == -1) || (cpu == rq->cpu))
			നിർത്തുക;

		lowest_rq = cpu_rq(cpu);

		എങ്കിൽ (lowest_rq->rt.highest_prio.curr <= task->prio) {
			/*
			 * Target rq has tasks of equal or higher priority,
			 * retrying does not release any lock and is unlikely
			 * to yield a different result.
			 */
			lowest_rq = ശൂന്യ;
			നിർത്തുക;
		}

		/* if the prio of this runqueue changed, try again */
		എങ്കിൽ (double_lock_balance(rq, lowest_rq)) {
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
			എങ്കിൽ (unlikely(is_migration_disabled(task) ||
				     !cpumask_test_cpu(lowest_rq->cpu, &task->cpus_mask) ||
				     task != pick_next_pushable_task(rq))) {

				double_unlock_balance(rq, lowest_rq);
				lowest_rq = ശൂന്യ;
				നിർത്തുക;
			}
		}

		/* If this rq is still suitable use it. */
		എങ്കിൽ (lowest_rq->rt.highest_prio.curr > task->prio)
			നിർത്തുക;

		/* try again */
		double_unlock_balance(rq, lowest_rq);
		lowest_rq = ശൂന്യ;
	}

	തിരികെ lowest_rq;
}

/*
 * If the current CPU has more than one RT task, see if the non
 * running task can migrate over to a CPU that is running a task
 * of lesser priority.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ തള്ളുക_rt_task(ഘടന rq *rq, bool pull)
{
	ഘടന task_struct *next_task;
	ഘടന rq *lowest_rq;
	പൂർണ്ണസംഖ്യ ret = 0;

	എങ്കിൽ (!rq->rt.overloaded)
		തിരികെ 0;

	next_task = pick_next_pushable_task(rq);
	എങ്കിൽ (!next_task)
		തിരികെ 0;

retry:
	/*
	 * It's possible that the next_task slipped in of
	 * higher priority than current. If that's the case
	 * just reschedule current.
	 */
	എങ്കിൽ (unlikely(next_task->prio < rq->donor->prio)) {
		resched_curr(rq);
		തിരികെ 0;
	}

	എങ്കിൽ (is_migration_disabled(next_task)) {
		ഘടന task_struct *തള്ളുക_task = ശൂന്യ;
		പൂർണ്ണസംഖ്യ cpu;

		എങ്കിൽ (!pull || rq->തള്ളുക_busy)
			തിരികെ 0;

		/*
		 * Invoking find_lowest_rq() on anything but an RT task doesn't
		 * make sense. Per the above priority check, curr has to
		 * be of higher priority than next_task, so no need to
		 * reschedule when bailing out.
		 *
		 * Note that the stoppers are masqueraded as SCHED_FIFO
		 * (cf. sched_set_stop_task()), so we can't rely on rt_task().
		 */
		എങ്കിൽ (rq->donor->sched_class != &rt_sched_class)
			തിരികെ 0;

		cpu = find_lowest_rq(rq->curr);
		എങ്കിൽ (cpu == -1 || cpu == rq->cpu)
			തിരികെ 0;

		/*
		 * Given we found a CPU with lower priority than @next_task,
		 * therefore it should be running. However we cannot migrate it
		 * to this other CPU, instead attempt to push the current
		 * running task on this CPU away.
		 */
		തള്ളുക_task = get_തള്ളുക_task(rq);
		എങ്കിൽ (തള്ളുക_task) {
			preempt_disable();
			raw_spin_rq_unlock(rq);
			stop_one_cpu_nowait(rq->cpu, തള്ളുക_cpu_stop,
					    തള്ളുക_task, &rq->തള്ളുക_work);
			preempt_enable();
			raw_spin_rq_lock(rq);
		}

		തിരികെ 0;
	}

	എങ്കിൽ (WARN_ON(next_task == rq->curr))
		തിരികെ 0;

	/* We might release rq lock */
	get_task_struct(next_task);

	/* find_lock_lowest_rq locks the rq if found */
	lowest_rq = find_lock_lowest_rq(next_task, rq);
	എങ്കിൽ (!lowest_rq) {
		ഘടന task_struct *task;
		/*
		 * find_lock_lowest_rq releases rq->lock
		 * so it is possible that next_task has migrated.
		 *
		 * We need to make sure that the task is still on the same
		 * run-queue and is also still the next task eligible for
		 * pushing.
		 */
		task = pick_next_pushable_task(rq);
		എങ്കിൽ (task == next_task) {
			/*
			 * The task hasn't migrated, and is still the next
			 * eligible task, but we failed to find a run-queue
			 * to push it to.  Do not retry in this case, since
			 * other CPUs will pull from us when ready.
			 */
			പോകുക out;
		}

		എങ്കിൽ (!task)
			/* No more tasks, just exit */
			പോകുക out;

		/*
		 * Something has shifted, try again.
		 */
		put_task_struct(next_task);
		next_task = task;
		പോകുക retry;
	}

	move_queued_task_locked(rq, lowest_rq, next_task);
	resched_curr(lowest_rq);
	ret = 1;

	double_unlock_balance(rq, lowest_rq);
out:
	put_task_struct(next_task);

	തിരികെ ret;
}

സ്ഥിതം ശൂന്യം തള്ളുക_rt_tasks(ഘടന rq *rq)
{
	/* push_rt_task will return true if it moved an RT */
	ആകുമ്പോൾ (തള്ളുക_rt_task(rq, false))
		;
}

#ifdef HAVE_RT_തള്ളുക_IPI

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
സ്ഥിതം പൂർണ്ണസംഖ്യ rto_next_cpu(ഘടന വേര്_domain *rd)
{
	പൂർണ്ണസംഖ്യ this_cpu = smp_processor_id();
	പൂർണ്ണസംഖ്യ next;
	പൂർണ്ണസംഖ്യ cpu;

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
	വേണ്ടി (;;) {

		/* When rto_cpu is -1 this acts like cpumask_first() */
		cpu = cpumask_next(rd->rto_cpu, rd->rto_mask);

		rd->rto_cpu = cpu;

		/* Do not send IPI to self */
		എങ്കിൽ (cpu == this_cpu)
			തുടരുക;

		എങ്കിൽ (cpu < nr_cpu_ids)
			തിരികെ cpu;

		rd->rto_cpu = -1;

		/*
		 * ACQUIRE ensures we see the @rto_mask changes
		 * made prior to the @next value observed.
		 *
		 * Matches WMB in rt_set_overload().
		 */
		next = atomic_read_acquire(&rd->rto_loop_next);

		എങ്കിൽ (rd->rto_loop == next)
			നിർത്തുക;

		rd->rto_loop = next;
	}

	തിരികെ -1;
}

സ്ഥിതം വരിയിൽ bool rto_start_trylock(atomic_t *v)
{
	തിരികെ !atomic_cmpxchg_acquire(v, 0, 1);
}

സ്ഥിതം വരിയിൽ ശൂന്യം rto_start_unlock(atomic_t *v)
{
	atomic_set_release(v, 0);
}

സ്ഥിതം ശൂന്യം tell_cpu_to_തള്ളുക(ഘടന rq *rq)
{
	പൂർണ്ണസംഖ്യ cpu = -1;

	/* Keep the loop going if the IPI is currently active */
	atomic_inc(&rq->rd->rto_loop_next);

	/* Only one CPU can initiate a loop at a time */
	എങ്കിൽ (!rto_start_trylock(&rq->rd->rto_loop_start))
		തിരികെ;

	raw_spin_lock(&rq->rd->rto_lock);

	/*
	 * The rto_cpu is updated under the lock, if it has a valid CPU
	 * then the IPI is still running and will continue due to the
	 * update to loop_next, and nothing needs to be done here.
	 * Otherwise it is finishing up and an IPI needs to be sent.
	 */
	എങ്കിൽ (rq->rd->rto_cpu < 0)
		cpu = rto_next_cpu(rq->rd);

	raw_spin_unlock(&rq->rd->rto_lock);

	rto_start_unlock(&rq->rd->rto_loop_start);

	എങ്കിൽ (cpu >= 0) {
		/* Make sure the rd does not get freed while pushing */
		sched_get_rd(rq->rd);
		irq_work_queue_on(&rq->rd->rto_തള്ളുക_work, cpu);
	}
}

/* Called from hardirq context */
ശൂന്യം rto_തള്ളുക_irq_work_func(ഘടന irq_work *work)
{
	ഘടന വേര്_domain *rd =
		container_of(work, ഘടന വേര്_domain, rto_തള്ളുക_work);
	ഘടന rq *rq;
	പൂർണ്ണസംഖ്യ cpu;

	rq = this_rq();

	/*
	 * We do not need to grab the lock to check for has_pushable_tasks.
	 * When it gets updated, a check is made if a push is possible.
	 */
	എങ്കിൽ (has_pushable_tasks(rq)) {
		raw_spin_rq_lock(rq);
		ആകുമ്പോൾ (തള്ളുക_rt_task(rq, true))
			;
		raw_spin_rq_unlock(rq);
	}

	raw_spin_lock(&rd->rto_lock);

	/* Pass the IPI to the next rt overloaded queue */
	cpu = rto_next_cpu(rd);

	raw_spin_unlock(&rd->rto_lock);

	എങ്കിൽ (cpu < 0) {
		sched_put_rd(rd);
		തിരികെ;
	}

	/* Try the next RT overloaded CPU */
	irq_work_queue_on(&rd->rto_തള്ളുക_work, cpu);
}
#endif /* HAVE_RT_PUSH_IPI */

സ്ഥിതം ശൂന്യം pull_rt_task(ഘടന rq *this_rq)
{
	പൂർണ്ണസംഖ്യ this_cpu = this_rq->cpu, cpu;
	bool resched = false;
	ഘടന task_struct *p, *തള്ളുക_task;
	ഘടന rq *src_rq;
	പൂർണ്ണസംഖ്യ rt_overload_എണ്ണം = rt_overloaded(this_rq);

	എങ്കിൽ (likely(!rt_overload_എണ്ണം))
		തിരികെ;

	/*
	 * Match the barrier from rt_set_overloaded; this guarantees that if we
	 * see overloaded we must also see the rto_mask bit.
	 */
	smp_rmb();

	/* If we are the only overloaded CPU do nothing */
	എങ്കിൽ (rt_overload_എണ്ണം == 1 &&
	    cpumask_test_cpu(this_rq->cpu, this_rq->rd->rto_mask))
		തിരികെ;

#ifdef HAVE_RT_തള്ളുക_IPI
	എങ്കിൽ (sched_feat(RT_തള്ളുക_IPI)) {
		tell_cpu_to_തള്ളുക(this_rq);
		തിരികെ;
	}
#endif

	for_each_cpu(cpu, this_rq->rd->rto_mask) {
		എങ്കിൽ (this_cpu == cpu)
			തുടരുക;

		src_rq = cpu_rq(cpu);

		/*
		 * Don't bother taking the src_rq->lock if the next highest
		 * task is known to be lower-priority than our current task.
		 * This may look racy, but if this value is about to go
		 * logically higher, the src_rq will push this task away.
		 * And if its going logically lower, we do not care
		 */
		എങ്കിൽ (src_rq->rt.highest_prio.next >=
		    this_rq->rt.highest_prio.curr)
			തുടരുക;

		/*
		 * We can potentially drop this_rq's lock in
		 * double_lock_balance, and another CPU could
		 * alter this_rq
		 */
		തള്ളുക_task = ശൂന്യ;
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
		എങ്കിൽ (p && (p->prio < this_rq->rt.highest_prio.curr)) {
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
			എങ്കിൽ (p->prio < src_rq->donor->prio)
				പോകുക skip;

			എങ്കിൽ (is_migration_disabled(p)) {
				തള്ളുക_task = get_തള്ളുക_task(src_rq);
			} അല്ലെങ്കിൽ {
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

		എങ്കിൽ (തള്ളുക_task) {
			preempt_disable();
			raw_spin_rq_unlock(this_rq);
			stop_one_cpu_nowait(src_rq->cpu, തള്ളുക_cpu_stop,
					    തള്ളുക_task, &src_rq->തള്ളുക_work);
			preempt_enable();
			raw_spin_rq_lock(this_rq);
		}
	}

	എങ്കിൽ (resched)
		resched_curr(this_rq);
}

/*
 * If we are not running and we are not going to reschedule soon, we should
 * try to push tasks away now
 */
സ്ഥിതം ശൂന്യം task_woken_rt(ഘടന rq *rq, ഘടന task_struct *p)
{
	bool need_to_തള്ളുക = !task_on_cpu(rq, p) &&
			    !test_tsk_need_resched(rq->curr) &&
			    p->nr_cpus_allowed > 1 &&
			    (dl_task(rq->donor) || rt_task(rq->donor)) &&
			    (rq->curr->nr_cpus_allowed < 2 ||
			     rq->donor->prio <= p->prio);

	എങ്കിൽ (need_to_തള്ളുക)
		തള്ളുക_rt_tasks(rq);
}

/* Assumes rq->lock is held */
സ്ഥിതം ശൂന്യം rq_online_rt(ഘടന rq *rq)
{
	എങ്കിൽ (rq->rt.overloaded)
		rt_set_overload(rq);

	__enable_runtime(rq);

	cpupri_set(&rq->rd->cpupri, rq->cpu, rq->rt.highest_prio.curr);
}

/* Assumes rq->lock is held */
സ്ഥിതം ശൂന്യം rq_offline_rt(ഘടന rq *rq)
{
	എങ്കിൽ (rq->rt.overloaded)
		rt_clear_overload(rq);

	__disable_runtime(rq);

	cpupri_set(&rq->rd->cpupri, rq->cpu, CPUPRI_INVALID);
}

/*
 * When switch from the rt queue, we bring ourselves to a position
 * that we might want to pull RT tasks from other runqueues.
 */
സ്ഥിതം ശൂന്യം switched_from_rt(ഘടന rq *rq, ഘടന task_struct *p)
{
	/*
	 * If there are other RT tasks then we will reschedule
	 * and the scheduling of the other RT tasks will handle
	 * the balancing. But if we are the last RT task
	 * we may need to handle the pulling of RT tasks
	 * now.
	 */
	എങ്കിൽ (!task_on_rq_queued(p) || rq->rt.rt_nr_running)
		തിരികെ;

	rt_queue_pull_task(rq);
}

ശൂന്യം __init init_sched_rt_class(ശൂന്യം)
{
	unsigned പൂർണ്ണസംഖ്യ i;

	for_each_possible_cpu(i) {
		zalloc_cpumask_var_കണ്ണി(&per_cpu(local_cpu_mask, i),
					GFP_KERNEL, cpu_to_കണ്ണി(i));
	}
}

/*
 * When switching a task to RT, we may overload the runqueue
 * with RT tasks. In this case we try to push them off to
 * other runqueues.
 */
സ്ഥിതം ശൂന്യം switched_to_rt(ഘടന rq *rq, ഘടന task_struct *p)
{
	/*
	 * If we are running, update the avg_rt tracking, as the running time
	 * will now on be accounted into the latter.
	 */
	എങ്കിൽ (task_current(rq, p)) {
		update_rt_rq_load_avg(rq_clock_pelt(rq), rq, 0);
		തിരികെ;
	}

	/*
	 * If we are not running we may need to preempt the current
	 * running task. If that current running task is also an RT task
	 * then see if we can move to another run queue.
	 */
	എങ്കിൽ (task_on_rq_queued(p)) {
		എങ്കിൽ (p->nr_cpus_allowed > 1 && rq->rt.overloaded)
			rt_queue_തള്ളുക_tasks(rq);
		എങ്കിൽ (p->prio < rq->donor->prio && cpu_online(cpu_of(rq)))
			resched_curr(rq);
	}
}

/*
 * Priority of the task has changed. This may cause
 * us to initiate a push or pull.
 */
സ്ഥിതം ശൂന്യം
prio_changed_rt(ഘടന rq *rq, ഘടന task_struct *p, u64 oldprio)
{
	എങ്കിൽ (!task_on_rq_queued(p))
		തിരികെ;

	എങ്കിൽ (p->prio == oldprio)
		തിരികെ;

	എങ്കിൽ (task_current_donor(rq, p)) {
		/*
		 * If our priority decreases while running, we
		 * may need to pull tasks to this runqueue.
		 */
		എങ്കിൽ (oldprio < p->prio)
			rt_queue_pull_task(rq);

		/*
		 * If there's a higher priority task waiting to run
		 * then reschedule.
		 */
		എങ്കിൽ (p->prio > rq->rt.highest_prio.curr)
			resched_curr(rq);
	} അല്ലെങ്കിൽ {
		/*
		 * This task is not running, but if it is
		 * greater than the current running task
		 * then reschedule.
		 */
		എങ്കിൽ (p->prio < rq->donor->prio)
			resched_curr(rq);
	}
}

#ifdef CONFIG_POSIX_TIMERS
സ്ഥിതം ശൂന്യം watchdog(ഘടന rq *rq, ഘടന task_struct *p)
{
	unsigned long soft, hard;

	/* max may change after cur was read, this will be fixed next tick */
	soft = task_rlimit(p, RLIMIT_RTTIME);
	hard = task_rlimit_max(p, RLIMIT_RTTIME);

	എങ്കിൽ (soft != RLIM_INFINITY) {
		unsigned long next;

		എങ്കിൽ (p->rt.watchdog_stamp != jiffies) {
			p->rt.timeout++;
			p->rt.watchdog_stamp = jiffies;
		}

		next = DIV_ROUND_UP(min(soft, hard), USEC_PER_SEC/HZ);
		എങ്കിൽ (p->rt.timeout > next) {
			posix_cputimers_rt_watchdog(&p->posix_cputimers,
						    p->se.sum_exec_runtime);
		}
	}
}
#അല്ലെങ്കിൽ /* !CONFIG_POSIX_TIMERS: */
സ്ഥിതം വരിയിൽ ശൂന്യം watchdog(ഘടന rq *rq, ഘടന task_struct *p) { }
#endif /* !CONFIG_POSIX_TIMERS */

/*
 * scheduler tick hitting a task of our scheduling class.
 *
 * NOTE: This function can be called remotely by the tick offload that
 * goes along full dynticks. Therefore no local assumption can be made
 * and everything must be accessed through the @rq and @curr passed in
 * parameters.
 */
സ്ഥിതം ശൂന്യം task_tick_rt(ഘടന rq *rq, ഘടന task_struct *p, പൂർണ്ണസംഖ്യ queued)
{
	ഘടന sched_rt_entity *rt_se = &p->rt;

	update_curr_rt(rq);
	update_rt_rq_load_avg(rq_clock_pelt(rq), rq, 1);

	watchdog(rq, p);

	/*
	 * RR tasks need a special form of time-slice management.
	 * FIFO tasks have no timeslices.
	 */
	എങ്കിൽ (p->policy != SCHED_RR)
		തിരികെ;

	എങ്കിൽ (--p->rt.time_slice)
		തിരികെ;

	p->rt.time_slice = sched_rr_timeslice;

	/*
	 * Requeue to the end of queue if we (and all of our ancestors) are not
	 * the only element on the queue
	 */
	for_each_sched_rt_entity(rt_se) {
		എങ്കിൽ (rt_se->run_list.prev != rt_se->run_list.next) {
			requeue_task_rt(rq, p, 0);
			resched_curr(rq);
			തിരികെ;
		}
	}
}

സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ get_rr_interval_rt(ഘടന rq *rq, ഘടന task_struct *task)
{
	/*
	 * Time slice is 0 for SCHED_FIFO tasks
	 */
	എങ്കിൽ (task->policy == SCHED_RR)
		തിരികെ sched_rr_timeslice;
	അല്ലെങ്കിൽ
		തിരികെ 0;
}

#ifdef CONFIG_SCHED_CORE
സ്ഥിതം പൂർണ്ണസംഖ്യ task_is_throttled_rt(ഘടന task_struct *p, പൂർണ്ണസംഖ്യ cpu)
{
	ഘടന rt_rq *rt_rq;

#ifdef CONFIG_RT_GROUP_SCHED // XXX maybe add task_rt_rq(), see also sched_rt_period_rt_rq
	rt_rq = task_group(p)->rt_rq[cpu];
	WARN_ON(!rt_group_sched_enabled() && rt_rq->tg != &വേര്_task_group);
#അല്ലെങ്കിൽ
	rt_rq = &cpu_rq(cpu)->rt;
#endif

	തിരികെ rt_rq_throttled(rt_rq);
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
സ്ഥിതം DEFINE_MUTEX(rt_constraints_mutex);

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ tg_has_rt_tasks(ഘടന task_group *tg)
{
	ഘടന task_struct *task;
	ഘടന css_task_iter it;
	പൂർണ്ണസംഖ്യ ret = 0;

	/*
	 * Autogroups do not have RT tasks; see autogroup_create().
	 */
	എങ്കിൽ (task_group_is_autogroup(tg))
		തിരികെ 0;

	css_task_iter_start(&tg->css, 0, &it);
	ആകുമ്പോൾ (!ret && (task = css_task_iter_next(&it)))
		ret |= rt_task(task);
	css_task_iter_end(&it);

	തിരികെ ret;
}

ഘടന rt_schedulable_data {
	ഘടന task_group *tg;
	u64 rt_period;
	u64 rt_runtime;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ tg_rt_schedulable(ഘടന task_group *tg, ശൂന്യം *data)
{
	ഘടന rt_schedulable_data *d = data;
	ഘടന task_group *child;
	u64 total, sum = 0;
	u64 period, runtime;

	period = ktime_to_ns(tg->rt_bandwidth.rt_period);
	runtime = tg->rt_bandwidth.rt_runtime;

	എങ്കിൽ (tg == d->tg) {
		period = d->rt_period;
		runtime = d->rt_runtime;
	}

	/*
	 * Cannot have more runtime than the period.
	 */
	എങ്കിൽ (runtime > period && runtime != RUNTIME_INF)
		തിരികെ -EINVAL;

	/*
	 * Ensure we don't starve existing RT tasks if runtime turns zero.
	 */
	എങ്കിൽ (rt_bandwidth_enabled() && !runtime &&
	    tg->rt_bandwidth.rt_runtime && tg_has_rt_tasks(tg))
		തിരികെ -EBUSY;

	total = to_ratio(period, runtime);

	/*
	 * Nobody can have more than the global setting allows.
	 */
	എങ്കിൽ (total > to_ratio(global_rt_period(), global_rt_runtime()))
		തിരികെ -EINVAL;

	/*
	 * The sum of our children's runtime should not exceed our own.
	 */
	list_for_each_entry_rcu(child, &tg->children, siblings) {
		period = ktime_to_ns(child->rt_bandwidth.rt_period);
		runtime = child->rt_bandwidth.rt_runtime;

		എങ്കിൽ (child == d->tg) {
			period = d->rt_period;
			runtime = d->rt_runtime;
		}

		sum += to_ratio(period, runtime);
	}

	എങ്കിൽ (sum > total)
		തിരികെ -EINVAL;

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ __rt_schedulable(ഘടന task_group *tg, u64 period, u64 runtime)
{
	പൂർണ്ണസംഖ്യ ret;

	ഘടന rt_schedulable_data data = {
		.tg = tg,
		.rt_period = period,
		.rt_runtime = runtime,
	};

	rcu_read_lock();
	ret = walk_tg_വൃക്ഷം(tg_rt_schedulable, tg_nop, &data);
	rcu_read_unlock();

	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ tg_set_rt_bandwidth(ഘടന task_group *tg,
		u64 rt_period, u64 rt_runtime)
{
	പൂർണ്ണസംഖ്യ i, err = 0;

	/*
	 * Disallowing the root group RT runtime is BAD, it would disallow the
	 * kernel creating (and or operating) RT threads.
	 */
	എങ്കിൽ (tg == &വേര്_task_group && rt_runtime == 0)
		തിരികെ -EINVAL;

	/* No period doesn't make any sense. */
	എങ്കിൽ (rt_period == 0)
		തിരികെ -EINVAL;

	/*
	 * Bound quota to defend quota against overflow during bandwidth shift.
	 */
	എങ്കിൽ (rt_runtime != RUNTIME_INF && rt_runtime > max_rt_runtime)
		തിരികെ -EINVAL;

	mutex_lock(&rt_constraints_mutex);
	err = __rt_schedulable(tg, rt_period, rt_runtime);
	എങ്കിൽ (err)
		പോകുക unlock;

	raw_spin_lock_irq(&tg->rt_bandwidth.rt_runtime_lock);
	tg->rt_bandwidth.rt_period = ns_to_ktime(rt_period);
	tg->rt_bandwidth.rt_runtime = rt_runtime;

	for_each_possible_cpu(i) {
		ഘടന rt_rq *rt_rq = tg->rt_rq[i];

		raw_spin_lock(&rt_rq->rt_runtime_lock);
		rt_rq->rt_runtime = rt_runtime;
		raw_spin_unlock(&rt_rq->rt_runtime_lock);
	}
	raw_spin_unlock_irq(&tg->rt_bandwidth.rt_runtime_lock);
unlock:
	mutex_unlock(&rt_constraints_mutex);

	തിരികെ err;
}

പൂർണ്ണസംഖ്യ sched_group_set_rt_runtime(ഘടന task_group *tg, long rt_runtime_us)
{
	u64 rt_runtime, rt_period;

	rt_period = ktime_to_ns(tg->rt_bandwidth.rt_period);
	rt_runtime = (u64)rt_runtime_us * NSEC_PER_USEC;
	എങ്കിൽ (rt_runtime_us < 0)
		rt_runtime = RUNTIME_INF;
	അല്ലെങ്കിൽ എങ്കിൽ ((u64)rt_runtime_us > U64_MAX / NSEC_PER_USEC)
		തിരികെ -EINVAL;

	തിരികെ tg_set_rt_bandwidth(tg, rt_period, rt_runtime);
}

long sched_group_rt_runtime(ഘടന task_group *tg)
{
	u64 rt_runtime_us;

	എങ്കിൽ (tg->rt_bandwidth.rt_runtime == RUNTIME_INF)
		തിരികെ -1;

	rt_runtime_us = tg->rt_bandwidth.rt_runtime;
	do_div(rt_runtime_us, NSEC_PER_USEC);
	തിരികെ rt_runtime_us;
}

പൂർണ്ണസംഖ്യ sched_group_set_rt_period(ഘടന task_group *tg, u64 rt_period_us)
{
	u64 rt_runtime, rt_period;

	എങ്കിൽ (rt_period_us > U64_MAX / NSEC_PER_USEC)
		തിരികെ -EINVAL;

	rt_period = rt_period_us * NSEC_PER_USEC;
	rt_runtime = tg->rt_bandwidth.rt_runtime;

	തിരികെ tg_set_rt_bandwidth(tg, rt_period, rt_runtime);
}

long sched_group_rt_period(ഘടന task_group *tg)
{
	u64 rt_period_us;

	rt_period_us = ktime_to_ns(tg->rt_bandwidth.rt_period);
	do_div(rt_period_us, NSEC_PER_USEC);
	തിരികെ rt_period_us;
}

പൂർണ്ണസംഖ്യ sched_rt_can_attach(ഘടന task_group *tg, ഘടന task_struct *tsk)
{
	/* Don't accept real-time tasks when there is no way for them to run */
	എങ്കിൽ (rt_group_sched_enabled() && rt_task(tsk) && tg->rt_bandwidth.rt_runtime == 0)
		തിരികെ 0;

	തിരികെ 1;
}

#endif /* !CONFIG_RT_GROUP_SCHED */

#ifdef CONFIG_SYSCTL
സ്ഥിതം പൂർണ്ണസംഖ്യ sched_rt_global_validate(ശൂന്യം)
{
	എങ്കിൽ ((sysctl_sched_rt_runtime != RUNTIME_INF) &&
		((sysctl_sched_rt_runtime > sysctl_sched_rt_period) ||
		 ((u64)sysctl_sched_rt_runtime *
			NSEC_PER_USEC > max_rt_runtime)))
		തിരികെ -EINVAL;

#ifdef CONFIG_RT_GROUP_SCHED
	എങ്കിൽ (!rt_group_sched_enabled())
		തിരികെ 0;

	scoped_guard(mutex, &rt_constraints_mutex)
		തിരികെ __rt_schedulable(ശൂന്യ, 0, 0);
#endif
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ sched_rt_handler(സ്ഥിരം ഘടന ctl_table *table, പൂർണ്ണസംഖ്യ ലിഖ, ശൂന്യം *buffer,
		size_t *lenp, loff_t *ppos)
{
	പൂർണ്ണസംഖ്യ old_period, old_runtime;
	സ്ഥിതം DEFINE_MUTEX(mutex);
	പൂർണ്ണസംഖ്യ ret;

	mutex_lock(&mutex);
	sched_domains_mutex_lock();
	old_period = sysctl_sched_rt_period;
	old_runtime = sysctl_sched_rt_runtime;

	ret = proc_dointvec_minmax(table, ലിഖ, buffer, lenp, ppos);

	എങ്കിൽ (!ret && ലിഖ) {
		ret = sched_rt_global_validate();
		എങ്കിൽ (ret)
			പോകുക undo;

		ret = sched_dl_global_validate();
		എങ്കിൽ (ret)
			പോകുക undo;

		sched_dl_do_global();
	}
	എങ്കിൽ (0) {
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

	തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ sched_rr_handler(സ്ഥിരം ഘടന ctl_table *table, പൂർണ്ണസംഖ്യ ലിഖ, ശൂന്യം *buffer,
		size_t *lenp, loff_t *ppos)
{
	പൂർണ്ണസംഖ്യ ret;
	സ്ഥിതം DEFINE_MUTEX(mutex);

	mutex_lock(&mutex);
	ret = proc_dointvec(table, ലിഖ, buffer, lenp, ppos);
	/*
	 * Make sure that internally we keep jiffies.
	 * Also, writing zero resets the time-slice to default:
	 */
	എങ്കിൽ (!ret && ലിഖ) {
		sched_rr_timeslice =
			sysctl_sched_rr_timeslice <= 0 ? RR_TIMESLICE :
			msecs_to_jiffies(sysctl_sched_rr_timeslice);

		എങ്കിൽ (sysctl_sched_rr_timeslice <= 0)
			sysctl_sched_rr_timeslice = jiffies_to_msecs(RR_TIMESLICE);
	}
	mutex_unlock(&mutex);

	തിരികെ ret;
}
#endif /* CONFIG_SYSCTL */

ശൂന്യം print_rt_stats(ഘടന seq_file *m, പൂർണ്ണസംഖ്യ cpu)
{
	rt_rq_iter_t iter;
	ഘടന rt_rq *rt_rq;

	rcu_read_lock();
	for_each_rt_rq(rt_rq, iter, cpu_rq(cpu))
		print_rt_rq(m, cpu, rt_rq);
	rcu_read_unlock();
}
