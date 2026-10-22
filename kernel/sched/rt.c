// SPDX-License-Identifier: GPL-2.0
/*
 * Real-Time Scheduling Class (mapped to the SCHED_FIFO and SCHED_RR
 * policies)
 */

#ଅନ୍ତର୍ଭାଵ "sched.h"
#ଅନ୍ତର୍ଭାଵ "pelt.h"

ପୂର୍ଣ୍ଣସଂଖ୍ୟା sched_rr_timeslice = RR_TIMESLICE;
/* More than 4 hours if BW_SHIFT equals 20. */
ସ୍ଥିର ଅଚଳ u64 max_rt_runtime = MAX_BW;

/*
 * period over which we measure -rt task CPU usage in us.
 * default: 1s
 */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା sysctl_sched_rt_period = 1000000;

/*
 * part of the period that we allow rt tasks to run in us.
 * default: 1s
 */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା sysctl_sched_rt_runtime = 1000000;

#ifdef CONFIG_SYSCTL
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା sysctl_sched_rr_timeslice = (MSEC_PER_SEC * RR_TIMESLICE) / HZ;
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା sched_rt_handler(ଅଚଳ ସଂରଚନା ctl_table *table, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଲିଖ, ଶୂନ୍ୟ *buffer,
		size_t *lenp, loff_t *ppos);
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା sched_rr_handler(ଅଚଳ ସଂରଚନା ctl_table *table, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଲିଖ, ଶୂନ୍ୟ *buffer,
		size_t *lenp, loff_t *ppos);
ସ୍ଥିର ଅଚଳ ସଂରଚନା ctl_table sched_rt_sysctls[] = {
	{
		.procname       = "sched_rt_period_us",
		.data           = &sysctl_sched_rt_period,
		.maxlen         = ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା),
		.mode           = 0644,
		.proc_handler   = sched_rt_handler,
		.extra1         = SYSCTL_ONE,
		.extra2         = SYSCTL_INT_MAX,
	},
	{
		.procname       = "sched_rt_runtime_us",
		.data           = &sysctl_sched_rt_runtime,
		.maxlen         = ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା),
		.mode           = 0644,
		.proc_handler   = sched_rt_handler,
		.extra1         = SYSCTL_NEG_ONE,
		.extra2         = (ଶୂନ୍ୟ *)&sysctl_sched_rt_period,
	},
	{
		.procname       = "sched_rr_timeslice_ms",
		.data           = &sysctl_sched_rr_timeslice,
		.maxlen         = ଆକାର(ପୂର୍ଣ୍ଣସଂଖ୍ୟା),
		.mode           = 0644,
		.proc_handler   = sched_rr_handler,
	},
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା __init sched_rt_sysctl_init(ଶୂନ୍ୟ)
{
	register_sysctl_init("kernel", sched_rt_sysctls);
	ଫେରନ୍ତୁ 0;
}
late_initcall(sched_rt_sysctl_init);
#endif /* CONFIG_SYSCTL */

ଶୂନ୍ୟ init_rt_rq(ସଂରଚନା rt_rq *rt_rq)
{
	ସଂରଚନା rt_prio_array *array;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	array = &rt_rq->active;
	ପାଇଁ (i = 0; i < MAX_RT_PRIO; i++) {
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
	rt_rq->tg = &ମୂଳ_task_group;
#endif
}

#ifdef CONFIG_RT_GROUP_SCHED

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା do_sched_rt_period_timer(ସଂରଚନା rt_bandwidth *rt_b, ପୂର୍ଣ୍ଣସଂଖ୍ୟା overrun);

ସ୍ଥିର ଗଣନା hrtimer_restart sched_rt_period_timer(ସଂରଚନା hrtimer *timer)
{
	ସଂରଚନା rt_bandwidth *rt_b =
		container_of(timer, ସଂରଚନା rt_bandwidth, rt_period_timer);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା idle = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା overrun;

	raw_spin_lock(&rt_b->rt_runtime_lock);
	ପାଇଁ (;;) {
		overrun = hrtimer_forward_now(timer, rt_b->rt_period);
		ଯଦି (!overrun)
			ଭାଙ୍ଗ;

		raw_spin_unlock(&rt_b->rt_runtime_lock);
		idle = do_sched_rt_period_timer(rt_b, overrun);
		raw_spin_lock(&rt_b->rt_runtime_lock);
	}
	ଯଦି (idle)
		rt_b->rt_period_active = 0;
	raw_spin_unlock(&rt_b->rt_runtime_lock);

	ଫେରନ୍ତୁ idle ? HRTIMER_NORESTART : HRTIMER_RESTART;
}

ଶୂନ୍ୟ init_rt_bandwidth(ସଂରଚନା rt_bandwidth *rt_b, u64 period, u64 runtime)
{
	rt_b->rt_period = ns_to_ktime(period);
	rt_b->rt_runtime = runtime;

	raw_spin_lock_init(&rt_b->rt_runtime_lock);

	hrtimer_setup(&rt_b->rt_period_timer, sched_rt_period_timer, CLOCK_MONOTONIC,
		      HRTIMER_MODE_REL_HARD);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ do_start_rt_bandwidth(ସଂରଚନା rt_bandwidth *rt_b)
{
	raw_spin_lock(&rt_b->rt_runtime_lock);
	ଯଦି (!rt_b->rt_period_active) {
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

ସ୍ଥିର ଶୂନ୍ୟ start_rt_bandwidth(ସଂରଚନା rt_bandwidth *rt_b)
{
	ଯଦି (!rt_bandwidth_enabled() || rt_b->rt_runtime == RUNTIME_INF)
		ଫେରନ୍ତୁ;

	do_start_rt_bandwidth(rt_b);
}

ସ୍ଥିର ଶୂନ୍ୟ destroy_rt_bandwidth(ସଂରଚନା rt_bandwidth *rt_b)
{
	hrtimer_cancel(&rt_b->rt_period_timer);
}

#define rt_entity_is_task(rt_se) (!(rt_se)->my_q)

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା task_struct *rt_task_of(ସଂରଚନା sched_rt_entity *rt_se)
{
	WARN_ON_ONCE(!rt_entity_is_task(rt_se));

	ଫେରନ୍ତୁ container_of(rt_se, ସଂରଚନା task_struct, rt);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା rq *rq_of_rt_rq(ସଂରଚନା rt_rq *rt_rq)
{
	/* Cannot fold with non-CONFIG_RT_GROUP_SCHED version, layout */
	WARN_ON(!rt_group_sched_enabled() && rt_rq->tg != &ମୂଳ_task_group);
	ଫେରନ୍ତୁ rt_rq->rq;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା rt_rq *rt_rq_of_se(ସଂରଚନା sched_rt_entity *rt_se)
{
	WARN_ON(!rt_group_sched_enabled() && rt_se->rt_rq->tg != &ମୂଳ_task_group);
	ଫେରନ୍ତୁ rt_se->rt_rq;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା rq *rq_of_rt_se(ସଂରଚନା sched_rt_entity *rt_se)
{
	ସଂରଚନା rt_rq *rt_rq = rt_se->rt_rq;

	WARN_ON(!rt_group_sched_enabled() && rt_rq->tg != &ମୂଳ_task_group);
	ଫେରନ୍ତୁ rt_rq->rq;
}

ଶୂନ୍ୟ unregister_rt_sched_group(ସଂରଚନା task_group *tg)
{
	ଯଦି (!rt_group_sched_enabled())
		ଫେରନ୍ତୁ;

	ଯଦି (tg->rt_se)
		destroy_rt_bandwidth(&tg->rt_bandwidth);
}

ଶୂନ୍ୟ free_rt_sched_group(ସଂରଚନା task_group *tg)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	ଯଦି (!rt_group_sched_enabled())
		ଫେରନ୍ତୁ;

	for_each_possible_cpu(i) {
		ଯଦି (tg->rt_rq)
			kfree(tg->rt_rq[i]);
		ଯଦି (tg->rt_se)
			kfree(tg->rt_se[i]);
	}

	kfree(tg->rt_rq);
	kfree(tg->rt_se);
}

ଶୂନ୍ୟ init_tg_rt_entry(ସଂରଚନା task_group *tg, ସଂରଚନା rt_rq *rt_rq,
		ସଂରଚନା sched_rt_entity *rt_se, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu,
		ସଂରଚନା sched_rt_entity *parent)
{
	ସଂରଚନା rq *rq = cpu_rq(cpu);

	rt_rq->highest_prio.curr = MAX_RT_PRIO-1;
	rt_rq->rt_nr_boosted = 0;
	rt_rq->rq = rq;
	rt_rq->tg = tg;

	tg->rt_rq[cpu] = rt_rq;
	tg->rt_se[cpu] = rt_se;

	ଯଦି (!rt_se)
		ଫେରନ୍ତୁ;

	ଯଦି (!parent)
		rt_se->rt_rq = &rq->rt;
	ଅନ୍ୟଥା
		rt_se->rt_rq = parent->my_q;

	rt_se->my_q = rt_rq;
	rt_se->parent = parent;
	INIT_LIST_HEAD(&rt_se->run_list);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା alloc_rt_sched_group(ସଂରଚନା task_group *tg, ସଂରଚନା task_group *parent)
{
	ସଂରଚନା rt_rq *rt_rq;
	ସଂରଚନା sched_rt_entity *rt_se;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	ଯଦି (!rt_group_sched_enabled())
		ଫେରନ୍ତୁ 1;

	tg->rt_rq = kzalloc_objs(rt_rq, nr_cpu_ids);
	ଯଦି (!tg->rt_rq)
		ଯାଅ err;
	tg->rt_se = kzalloc_objs(rt_se, nr_cpu_ids);
	ଯଦି (!tg->rt_se)
		ଯାଅ err;

	init_rt_bandwidth(&tg->rt_bandwidth, ktime_to_ns(global_rt_period()), 0);

	for_each_possible_cpu(i) {
		rt_rq = kzalloc_ଗଣ୍ଠି(ଆକାର(ସଂରଚନା rt_rq),
				     GFP_KERNEL, cpu_to_ଗଣ୍ଠି(i));
		ଯଦି (!rt_rq)
			ଯାଅ err;

		rt_se = kzalloc_ଗଣ୍ଠି(ଆକାର(ସଂରଚନା sched_rt_entity),
				     GFP_KERNEL, cpu_to_ଗଣ୍ଠି(i));
		ଯଦି (!rt_se)
			ଯାଅ err_free_rq;

		init_rt_rq(rt_rq);
		rt_rq->rt_runtime = tg->rt_bandwidth.rt_runtime;
		init_tg_rt_entry(tg, rt_rq, rt_se, i, parent->rt_se[i]);
	}

	ଫେରନ୍ତୁ 1;

err_free_rq:
	kfree(rt_rq);
err:
	ଫେରନ୍ତୁ 0;
}

#ଅନ୍ୟଥା /* !CONFIG_RT_GROUP_SCHED: */

#define rt_entity_is_task(rt_se) (1)

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା task_struct *rt_task_of(ସଂରଚନା sched_rt_entity *rt_se)
{
	ଫେରନ୍ତୁ container_of(rt_se, ସଂରଚନା task_struct, rt);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା rq *rq_of_rt_rq(ସଂରଚନା rt_rq *rt_rq)
{
	ଫେରନ୍ତୁ container_of(rt_rq, ସଂରଚନା rq, rt);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା rq *rq_of_rt_se(ସଂରଚନା sched_rt_entity *rt_se)
{
	ସଂରଚନା task_struct *p = rt_task_of(rt_se);

	ଫେରନ୍ତୁ task_rq(p);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା rt_rq *rt_rq_of_se(ସଂରଚନା sched_rt_entity *rt_se)
{
	ସଂରଚନା rq *rq = rq_of_rt_se(rt_se);

	ଫେରନ୍ତୁ &rq->rt;
}

ଶୂନ୍ୟ unregister_rt_sched_group(ସଂରଚନା task_group *tg) { }

ଶୂନ୍ୟ free_rt_sched_group(ସଂରଚନା task_group *tg) { }

ପୂର୍ଣ୍ଣସଂଖ୍ୟା alloc_rt_sched_group(ସଂରଚନା task_group *tg, ସଂରଚନା task_group *parent)
{
	ଫେରନ୍ତୁ 1;
}
#endif /* !CONFIG_RT_GROUP_SCHED */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool need_pull_rt_task(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *prev)
{
	/* Try to pull RT tasks here if we lower this rq's prio */
	ଫେରନ୍ତୁ rq->online && rq->rt.highest_prio.curr > prev->prio;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା rt_overloaded(ସଂରଚନା rq *rq)
{
	ଫେରନ୍ତୁ atomic_read(&rq->rd->rto_ଗଣନା);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ rt_set_overload(ସଂରଚନା rq *rq)
{
	ଯଦି (!rq->online)
		ଫେରନ୍ତୁ;

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
	atomic_inc(&rq->rd->rto_ଗଣନା);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ rt_clear_overload(ସଂରଚନା rq *rq)
{
	ଯଦି (!rq->online)
		ଫେରନ୍ତୁ;

	/* the order here really doesn't matter */
	atomic_dec(&rq->rd->rto_ଗଣନା);
	cpumask_clear_cpu(rq->cpu, rq->rd->rto_mask);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା has_pushable_tasks(ସଂରଚନା rq *rq)
{
	ଫେରନ୍ତୁ !plist_head_empty(&rq->rt.pushable_tasks);
}

ସ୍ଥିର DEFINE_PER_CPU(ସଂରଚନା balance_callback, rt_ଠେଲ_head);
ସ୍ଥିର DEFINE_PER_CPU(ସଂରଚନା balance_callback, rt_pull_head);

ସ୍ଥିର ଶୂନ୍ୟ ଠେଲ_rt_tasks(ସଂରଚନା rq *);
ସ୍ଥିର ଶୂନ୍ୟ pull_rt_task(ସଂରଚନା rq *);

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ rt_queue_ଠେଲ_tasks(ସଂରଚନା rq *rq)
{
	ଯଦି (!has_pushable_tasks(rq))
		ଫେରନ୍ତୁ;

	queue_balance_callback(rq, &per_cpu(rt_ଠେଲ_head, rq->cpu), ଠେଲ_rt_tasks);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ rt_queue_pull_task(ସଂରଚନା rq *rq)
{
	queue_balance_callback(rq, &per_cpu(rt_pull_head, rq->cpu), pull_rt_task);
}

ସ୍ଥିର ଶୂନ୍ୟ enqueue_pushable_task(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p)
{
	plist_del(&p->pushable_tasks, &rq->rt.pushable_tasks);
	plist_ଗଣ୍ଠି_init(&p->pushable_tasks, p->prio);
	plist_add(&p->pushable_tasks, &rq->rt.pushable_tasks);

	/* Update the highest prio pushable task */
	ଯଦି (p->prio < rq->rt.highest_prio.next)
		rq->rt.highest_prio.next = p->prio;

	ଯଦି (!rq->rt.overloaded) {
		rt_set_overload(rq);
		rq->rt.overloaded = 1;
	}
}

ସ୍ଥିର ଶୂନ୍ୟ dequeue_pushable_task(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p)
{
	plist_del(&p->pushable_tasks, &rq->rt.pushable_tasks);

	/* Update the new highest prio pushable task */
	ଯଦି (has_pushable_tasks(rq)) {
		p = plist_first_entry(&rq->rt.pushable_tasks,
				      ସଂରଚନା task_struct, pushable_tasks);
		rq->rt.highest_prio.next = p->prio;
	} ଅନ୍ୟଥା {
		rq->rt.highest_prio.next = MAX_RT_PRIO-1;

		ଯଦି (rq->rt.overloaded) {
			rt_clear_overload(rq);
			rq->rt.overloaded = 0;
		}
	}
}

ସ୍ଥିର ଶୂନ୍ୟ enqueue_ଶୀର୍ଷ_rt_rq(ସଂରଚନା rt_rq *rt_rq);
ସ୍ଥିର ଶୂନ୍ୟ dequeue_ଶୀର୍ଷ_rt_rq(ସଂରଚନା rt_rq *rt_rq, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2);

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା on_rt_rq(ସଂରଚନା sched_rt_entity *rt_se)
{
	ଫେରନ୍ତୁ rt_se->on_rq;
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
ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool rt_task_fits_capacity(ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା min_cap;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_cap;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu_cap;

	/* Only heterogeneous systems can benefit from this check */
	ଯଦି (!sched_asym_cpucap_active())
		ଫେରନ୍ତୁ true;

	min_cap = uclamp_eff_ମୂଲ୍ୟ(p, UCLAMP_MIN);
	max_cap = uclamp_eff_ମୂଲ୍ୟ(p, UCLAMP_MAX);

	cpu_cap = arch_scale_cpu_capacity(cpu);

	ଫେରନ୍ତୁ cpu_cap >= min(min_cap, max_cap);
}
#ଅନ୍ୟଥା /* !CONFIG_UCLAMP_TASK: */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool rt_task_fits_capacity(ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ଫେରନ୍ତୁ true;
}
#endif /* !CONFIG_UCLAMP_TASK */

#ifdef CONFIG_RT_GROUP_SCHED

ସ୍ଥିର ଧାଡ଼ିଭିତରେ u64 sched_rt_runtime(ସଂରଚନା rt_rq *rt_rq)
{
	ଫେରନ୍ତୁ rt_rq->rt_runtime;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ u64 sched_rt_period(ସଂରଚନା rt_rq *rt_rq)
{
	ଫେରନ୍ତୁ ktime_to_ns(rt_rq->tg->rt_bandwidth.rt_period);
}

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା task_group *rt_rq_iter_t;

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା task_group *next_task_group(ସଂରଚନା task_group *tg)
{
	ଯଦି (!rt_group_sched_enabled()) {
		WARN_ON(tg != &ମୂଳ_task_group);
		ଫେରନ୍ତୁ ଖାଲି;
	}

	କର {
		tg = list_entry_rcu(tg->list.next,
			typeof(ସଂରଚନା task_group), list);
	} ଯେତେବେଳେ (&tg->list != &task_groups && task_group_is_autogroup(tg));

	ଯଦି (&tg->list == &task_groups)
		tg = ଖାଲି;

	ଫେରନ୍ତୁ tg;
}

#define for_each_rt_rq(rt_rq, iter, rq)					\
	ପାଇଁ (iter = &ମୂଳ_task_group;					\
		iter && (rt_rq = iter->rt_rq[cpu_of(rq)]);		\
		iter = next_task_group(iter))

#define for_each_sched_rt_entity(rt_se) \
	ପାଇଁ (; rt_se; rt_se = rt_se->parent)

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା rt_rq *group_rt_rq(ସଂରଚନା sched_rt_entity *rt_se)
{
	ଫେରନ୍ତୁ rt_se->my_q;
}

ସ୍ଥିର ଶୂନ୍ୟ enqueue_rt_entity(ସଂରଚନା sched_rt_entity *rt_se, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags);
ସ୍ଥିର ଶୂନ୍ୟ dequeue_rt_entity(ସଂରଚନା sched_rt_entity *rt_se, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags);

ସ୍ଥିର ଶୂନ୍ୟ sched_rt_rq_enqueue(ସଂରଚନା rt_rq *rt_rq)
{
	ସଂରଚନା task_struct *donor = rq_of_rt_rq(rt_rq)->donor;
	ସଂରଚନା rq *rq = rq_of_rt_rq(rt_rq);
	ସଂରଚନା sched_rt_entity *rt_se;

	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu = cpu_of(rq);

	rt_se = rt_rq->tg->rt_se[cpu];

	ଯଦି (rt_rq->rt_nr_running) {
		ଯଦି (!rt_se)
			enqueue_ଶୀର୍ଷ_rt_rq(rt_rq);
		ଅନ୍ୟଥା ଯଦି (!on_rt_rq(rt_se))
			enqueue_rt_entity(rt_se, 0);

		ଯଦି (rt_rq->highest_prio.curr < donor->prio)
			resched_curr(rq);
	}
}

ସ୍ଥିର ଶୂନ୍ୟ sched_rt_rq_dequeue(ସଂରଚନା rt_rq *rt_rq)
{
	ସଂରଚନା sched_rt_entity *rt_se;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu = cpu_of(rq_of_rt_rq(rt_rq));

	rt_se = rt_rq->tg->rt_se[cpu];

	ଯଦି (!rt_se) {
		dequeue_ଶୀର୍ଷ_rt_rq(rt_rq, rt_rq->rt_nr_running);
		/* Kick cpufreq (see the comment in kernel/sched/sched.h). */
		cpufreq_update_util(rq_of_rt_rq(rt_rq), 0);
	}
	ଅନ୍ୟଥା ଯଦି (on_rt_rq(rt_se))
		dequeue_rt_entity(rt_se, 0);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା rt_rq_throttled(ସଂରଚନା rt_rq *rt_rq)
{
	ଫେରନ୍ତୁ rt_rq->rt_throttled && !rt_rq->rt_nr_boosted;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା rt_se_boosted(ସଂରଚନା sched_rt_entity *rt_se)
{
	ସଂରଚନା rt_rq *rt_rq = group_rt_rq(rt_se);
	ସଂରଚନା task_struct *p;

	ଯଦି (rt_rq)
		ଫେରନ୍ତୁ !!rt_rq->rt_nr_boosted;

	p = rt_task_of(rt_se);
	ଫେରନ୍ତୁ p->prio != p->normal_prio;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଅଚଳ ସଂରଚନା cpumask *sched_rt_period_mask(ଶୂନ୍ୟ)
{
	ଫେରନ୍ତୁ this_rq()->rd->span;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ
ସଂରଚନା rt_rq *sched_rt_period_rt_rq(ସଂରଚନା rt_bandwidth *rt_b, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ଫେରନ୍ତୁ container_of(rt_b, ସଂରଚନା task_group, rt_bandwidth)->rt_rq[cpu];
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା rt_bandwidth *sched_rt_bandwidth(ସଂରଚନା rt_rq *rt_rq)
{
	ଫେରନ୍ତୁ &rt_rq->tg->rt_bandwidth;
}

bool sched_rt_bandwidth_account(ସଂରଚନା rt_rq *rt_rq)
{
	ସଂରଚନା rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);

	ଫେରନ୍ତୁ (hrtimer_active(&rt_b->rt_period_timer) ||
		rt_rq->rt_time < rt_b->rt_runtime);
}

/*
 * We ran out of runtime, see if we can borrow some from our neighbours.
 */
ସ୍ଥିର ଶୂନ୍ୟ do_balance_runtime(ସଂରଚନା rt_rq *rt_rq)
{
	ସଂରଚନା rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);
	ସଂରଚନା ମୂଳ_domain *rd = rq_of_rt_rq(rt_rq)->rd;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, weight;
	u64 rt_period;

	weight = cpumask_weight(rd->span);

	raw_spin_lock(&rt_b->rt_runtime_lock);
	rt_period = ktime_to_ns(rt_b->rt_period);
	for_each_cpu(i, rd->span) {
		ସଂରଚନା rt_rq *iter = sched_rt_period_rt_rq(rt_b, i);
		s64 diff;

		ଯଦି (iter == rt_rq)
			ଜାରି;

		raw_spin_lock(&iter->rt_runtime_lock);
		/*
		 * Either all rqs have inf runtime and there's nothing to steal
		 * or __disable_runtime() below sets a specific rq to inf to
		 * indicate its been disabled and disallow stealing.
		 */
		ଯଦି (iter->rt_runtime == RUNTIME_INF)
			ଯାଅ next;

		/*
		 * From runqueues with spare time, take 1/n part of their
		 * spare time, but no more than our period.
		 */
		diff = iter->rt_runtime - iter->rt_time;
		ଯଦି (diff > 0) {
			diff = div_u64((u64)diff, weight);
			ଯଦି (rt_rq->rt_runtime + diff > rt_period)
				diff = rt_period - rt_rq->rt_runtime;
			iter->rt_runtime -= diff;
			rt_rq->rt_runtime += diff;
			ଯଦି (rt_rq->rt_runtime == rt_period) {
				raw_spin_unlock(&iter->rt_runtime_lock);
				ଭାଙ୍ଗ;
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
ସ୍ଥିର ଶୂନ୍ୟ __disable_runtime(ସଂରଚନା rq *rq)
{
	ସଂରଚନା ମୂଳ_domain *rd = rq->rd;
	rt_rq_iter_t iter;
	ସଂରଚନା rt_rq *rt_rq;

	ଯଦି (unlikely(!scheduler_running))
		ଫେରନ୍ତୁ;

	for_each_rt_rq(rt_rq, iter, rq) {
		ସଂରଚନା rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);
		s64 want;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

		raw_spin_lock(&rt_b->rt_runtime_lock);
		raw_spin_lock(&rt_rq->rt_runtime_lock);
		/*
		 * Either we're all inf and nobody needs to borrow, or we're
		 * already disabled and thus have nothing to do, or we have
		 * exactly the right amount of runtime to take out.
		 */
		ଯଦି (rt_rq->rt_runtime == RUNTIME_INF ||
				rt_rq->rt_runtime == rt_b->rt_runtime)
			ଯାଅ balanced;
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
			ସଂରଚନା rt_rq *iter = sched_rt_period_rt_rq(rt_b, i);
			s64 diff;

			/*
			 * Can't reclaim from ourselves or disabled runqueues.
			 */
			ଯଦି (iter == rt_rq || iter->rt_runtime == RUNTIME_INF)
				ଜାରି;

			raw_spin_lock(&iter->rt_runtime_lock);
			ଯଦି (want > 0) {
				diff = min_t(s64, iter->rt_runtime, want);
				iter->rt_runtime -= diff;
				want -= diff;
			} ଅନ୍ୟଥା {
				iter->rt_runtime -= want;
				want -= want;
			}
			raw_spin_unlock(&iter->rt_runtime_lock);

			ଯଦି (!want)
				ଭାଙ୍ଗ;
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

ସ୍ଥିର ଶୂନ୍ୟ __enable_runtime(ସଂରଚନା rq *rq)
{
	rt_rq_iter_t iter;
	ସଂରଚନା rt_rq *rt_rq;

	ଯଦି (unlikely(!scheduler_running))
		ଫେରନ୍ତୁ;

	/*
	 * Reset each runqueue's bandwidth settings
	 */
	for_each_rt_rq(rt_rq, iter, rq) {
		ସଂରଚନା rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);

		raw_spin_lock(&rt_b->rt_runtime_lock);
		raw_spin_lock(&rt_rq->rt_runtime_lock);
		rt_rq->rt_runtime = rt_b->rt_runtime;
		rt_rq->rt_time = 0;
		rt_rq->rt_throttled = 0;
		raw_spin_unlock(&rt_rq->rt_runtime_lock);
		raw_spin_unlock(&rt_b->rt_runtime_lock);
	}
}

ସ୍ଥିର ଶୂନ୍ୟ balance_runtime(ସଂରଚନା rt_rq *rt_rq)
{
	ଯଦି (!sched_feat(RT_RUNTIME_SHARE))
		ଫେରନ୍ତୁ;

	ଯଦି (rt_rq->rt_time > rt_rq->rt_runtime) {
		raw_spin_unlock(&rt_rq->rt_runtime_lock);
		do_balance_runtime(rt_rq);
		raw_spin_lock(&rt_rq->rt_runtime_lock);
	}
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା do_sched_rt_period_timer(ସଂରଚନା rt_bandwidth *rt_b, ପୂର୍ଣ୍ଣସଂଖ୍ୟା overrun)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, idle = 1, throttled = 0;
	ଅଚଳ ସଂରଚନା cpumask *span;

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
	ଯଦି (rt_b == &ମୂଳ_task_group.rt_bandwidth)
		span = cpu_online_mask;

	for_each_cpu(i, span) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା enqueue = 0;
		ସଂରଚନା rt_rq *rt_rq = sched_rt_period_rt_rq(rt_b, i);
		ସଂରଚନା rq *rq = rq_of_rt_rq(rt_rq);
		ସଂରଚନା rq_flags rf;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା skip;

		/*
		 * When span == cpu_online_mask, taking each rq->lock
		 * can be time-consuming. Try to avoid it when possible.
		 */
		raw_spin_lock(&rt_rq->rt_runtime_lock);
		ଯଦି (!sched_feat(RT_RUNTIME_SHARE) && rt_rq->rt_runtime != RUNTIME_INF)
			rt_rq->rt_runtime = rt_b->rt_runtime;
		skip = !rt_rq->rt_time && !rt_rq->rt_nr_running;
		raw_spin_unlock(&rt_rq->rt_runtime_lock);
		ଯଦି (skip)
			ଜାରି;

		rq_lock(rq, &rf);
		update_rq_clock(rq);

		ଯଦି (rt_rq->rt_time) {
			u64 runtime;

			raw_spin_lock(&rt_rq->rt_runtime_lock);
			ଯଦି (rt_rq->rt_throttled)
				balance_runtime(rt_rq);
			runtime = rt_rq->rt_runtime;
			rt_rq->rt_time -= min(rt_rq->rt_time, overrun*runtime);
			ଯଦି (rt_rq->rt_throttled && rt_rq->rt_time < runtime) {
				rt_rq->rt_throttled = 0;
				enqueue = 1;

				/*
				 * When we're idle and a woken (rt) task is
				 * throttled wakeup_preempt() will set
				 * skip_update and the time between the wakeup
				 * and this unthrottle will get accounted as
				 * 'runtime'.
				 */
				ଯଦି (rt_rq->rt_nr_running && rq->curr == rq->idle)
					rq_clock_cancel_skipupdate(rq);
			}
			ଯଦି (rt_rq->rt_time || rt_rq->rt_nr_running)
				idle = 0;
			raw_spin_unlock(&rt_rq->rt_runtime_lock);
		} ଅନ୍ୟଥା ଯଦି (rt_rq->rt_nr_running) {
			idle = 0;
			ଯଦି (!rt_rq_throttled(rt_rq))
				enqueue = 1;
		}
		ଯଦି (rt_rq->rt_throttled)
			throttled = 1;

		ଯଦି (enqueue)
			sched_rt_rq_enqueue(rt_rq);
		rq_unlock(rq, &rf);
	}

	ଯଦି (!throttled && (!rt_bandwidth_enabled() || rt_b->rt_runtime == RUNTIME_INF))
		ଫେରନ୍ତୁ 1;

	ଫେରନ୍ତୁ idle;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା sched_rt_runtime_exceeded(ସଂରଚନା rt_rq *rt_rq)
{
	u64 runtime = sched_rt_runtime(rt_rq);

	ଯଦି (rt_rq->rt_throttled)
		ଫେରନ୍ତୁ rt_rq_throttled(rt_rq);

	ଯଦି (runtime >= sched_rt_period(rt_rq))
		ଫେରନ୍ତୁ 0;

	balance_runtime(rt_rq);
	runtime = sched_rt_runtime(rt_rq);
	ଯଦି (runtime == RUNTIME_INF)
		ଫେରନ୍ତୁ 0;

	ଯଦି (rt_rq->rt_time > runtime) {
		ସଂରଚନା rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);

		/*
		 * Don't actually throttle groups that have no runtime assigned
		 * but accrue some time due to boosting.
		 */
		ଯଦି (likely(rt_b->rt_runtime)) {
			rt_rq->rt_throttled = 1;
			printk_deferred_once("sched: RT throttling activated\n");
		} ଅନ୍ୟଥା {
			/*
			 * In case we did anyway, make it go away,
			 * replenishment is a joke, since it will replenish us
			 * with exactly 0 ns.
			 */
			rt_rq->rt_time = 0;
		}

		ଯଦି (rt_rq_throttled(rt_rq)) {
			sched_rt_rq_dequeue(rt_rq);
			ଫେରନ୍ତୁ 1;
		}
	}

	ଫେରନ୍ତୁ 0;
}

#ଅନ୍ୟଥା /* !CONFIG_RT_GROUP_SCHED: */

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା rt_rq *rt_rq_iter_t;

#define for_each_rt_rq(rt_rq, iter, rq) \
	ପାଇଁ ((ଶୂନ୍ୟ) iter, rt_rq = &rq->rt; rt_rq; rt_rq = ଖାଲି)

#define for_each_sched_rt_entity(rt_se) \
	ପାଇଁ (; rt_se; rt_se = ଖାଲି)

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା rt_rq *group_rt_rq(ସଂରଚନା sched_rt_entity *rt_se)
{
	ଫେରନ୍ତୁ ଖାଲି;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ sched_rt_rq_enqueue(ସଂରଚନା rt_rq *rt_rq)
{
	ସଂରଚନା rq *rq = rq_of_rt_rq(rt_rq);

	ଯଦି (!rt_rq->rt_nr_running)
		ଫେରନ୍ତୁ;

	enqueue_ଶୀର୍ଷ_rt_rq(rt_rq);
	resched_curr(rq);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ sched_rt_rq_dequeue(ସଂରଚନା rt_rq *rt_rq)
{
	dequeue_ଶୀର୍ଷ_rt_rq(rt_rq, rt_rq->rt_nr_running);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା rt_rq_throttled(ସଂରଚନା rt_rq *rt_rq)
{
	ଫେରନ୍ତୁ false;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଅଚଳ ସଂରଚନା cpumask *sched_rt_period_mask(ଶୂନ୍ୟ)
{
	ଫେରନ୍ତୁ cpu_online_mask;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ
ସଂରଚନା rt_rq *sched_rt_period_rt_rq(ସଂରଚନା rt_bandwidth *rt_b, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ଫେରନ୍ତୁ &cpu_rq(cpu)->rt;
}

ସ୍ଥିର ଶୂନ୍ୟ __enable_runtime(ସଂରଚନା rq *rq) { }
ସ୍ଥିର ଶୂନ୍ୟ __disable_runtime(ସଂରଚନା rq *rq) { }

#endif /* !CONFIG_RT_GROUP_SCHED */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା rt_se_prio(ସଂରଚନା sched_rt_entity *rt_se)
{
#ifdef CONFIG_RT_GROUP_SCHED
	ସଂରଚନା rt_rq *rt_rq = group_rt_rq(rt_se);

	ଯଦି (rt_rq)
		ଫେରନ୍ତୁ rt_rq->highest_prio.curr;
#endif

	ଫେରନ୍ତୁ rt_task_of(rt_se)->prio;
}

/*
 * Update the current task's runtime statistics. Skip current tasks that
 * are not in our scheduling class.
 */
ସ୍ଥିର ଶୂନ୍ୟ update_curr_rt(ସଂରଚନା rq *rq)
{
	ସଂରଚନା task_struct *donor = rq->donor;
	s64 delta_exec;

	ଯଦି (donor->sched_class != &rt_sched_class)
		ଫେରନ୍ତୁ;

	delta_exec = update_curr_common(rq);
	ଯଦି (unlikely(delta_exec <= 0))
		ଫେରନ୍ତୁ;

#ifdef CONFIG_RT_GROUP_SCHED
	ସଂରଚନା sched_rt_entity *rt_se = &donor->rt;

	ଯଦି (!rt_bandwidth_enabled())
		ଫେରନ୍ତୁ;

	for_each_sched_rt_entity(rt_se) {
		ସଂରଚନା rt_rq *rt_rq = rt_rq_of_se(rt_se);
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା exceeded;

		ଯଦି (sched_rt_runtime(rt_rq) != RUNTIME_INF) {
			raw_spin_lock(&rt_rq->rt_runtime_lock);
			rt_rq->rt_time += delta_exec;
			exceeded = sched_rt_runtime_exceeded(rt_rq);
			ଯଦି (exceeded)
				resched_curr(rq);
			raw_spin_unlock(&rt_rq->rt_runtime_lock);
			ଯଦି (exceeded)
				do_start_rt_bandwidth(sched_rt_bandwidth(rt_rq));
		}
	}
#endif /* CONFIG_RT_GROUP_SCHED */
}

ସ୍ଥିର ଶୂନ୍ୟ
dequeue_ଶୀର୍ଷ_rt_rq(ସଂରଚନା rt_rq *rt_rq, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2)
{
	ସଂରଚନା rq *rq = rq_of_rt_rq(rt_rq);

	BUG_ON(&rq->rt != rt_rq);

	ଯଦି (!rt_rq->rt_queued)
		ଫେରନ୍ତୁ;

	BUG_ON(!rq->nr_running);

	sub_nr_running(rq, ଗଣନା_2);
	rt_rq->rt_queued = 0;

}

ସ୍ଥିର ଶୂନ୍ୟ
enqueue_ଶୀର୍ଷ_rt_rq(ସଂରଚନା rt_rq *rt_rq)
{
	ସଂରଚନା rq *rq = rq_of_rt_rq(rt_rq);

	BUG_ON(&rq->rt != rt_rq);

	ଯଦି (rt_rq->rt_queued)
		ଫେରନ୍ତୁ;

	ଯଦି (rt_rq_throttled(rt_rq))
		ଫେରନ୍ତୁ;

	ଯଦି (rt_rq->rt_nr_running) {
		add_nr_running(rq, rt_rq->rt_nr_running);
		rt_rq->rt_queued = 1;
	}

	/* Kick cpufreq (see the comment in kernel/sched/sched.h). */
	cpufreq_update_util(rq, 0);
}

ସ୍ଥିର ଶୂନ୍ୟ
inc_rt_prio_smp(ସଂରଚନା rt_rq *rt_rq, ପୂର୍ଣ୍ଣସଂଖ୍ୟା prio, ପୂର୍ଣ୍ଣସଂଖ୍ୟା prev_prio)
{
	ସଂରଚନା rq *rq = rq_of_rt_rq(rt_rq);

	/*
	 * Change rq's cpupri only if rt_rq is the top queue.
	 */
	ଯଦି (IS_ENABLED(CONFIG_RT_GROUP_SCHED) && &rq->rt != rt_rq)
		ଫେରନ୍ତୁ;

	ଯଦି (rq->online && prio < prev_prio)
		cpupri_set(&rq->rd->cpupri, rq->cpu, prio);
}

ସ୍ଥିର ଶୂନ୍ୟ
dec_rt_prio_smp(ସଂରଚନା rt_rq *rt_rq, ପୂର୍ଣ୍ଣସଂଖ୍ୟା prio, ପୂର୍ଣ୍ଣସଂଖ୍ୟା prev_prio)
{
	ସଂରଚନା rq *rq = rq_of_rt_rq(rt_rq);

	/*
	 * Change rq's cpupri only if rt_rq is the top queue.
	 */
	ଯଦି (IS_ENABLED(CONFIG_RT_GROUP_SCHED) && &rq->rt != rt_rq)
		ଫେରନ୍ତୁ;

	ଯଦି (rq->online && rt_rq->highest_prio.curr != prev_prio)
		cpupri_set(&rq->rd->cpupri, rq->cpu, rt_rq->highest_prio.curr);
}

ସ୍ଥିର ଶୂନ୍ୟ
inc_rt_prio(ସଂରଚନା rt_rq *rt_rq, ପୂର୍ଣ୍ଣସଂଖ୍ୟା prio)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା prev_prio = rt_rq->highest_prio.curr;

	ଯଦି (prio < prev_prio)
		rt_rq->highest_prio.curr = prio;

	inc_rt_prio_smp(rt_rq, prio, prev_prio);
}

ସ୍ଥିର ଶୂନ୍ୟ
dec_rt_prio(ସଂରଚନା rt_rq *rt_rq, ପୂର୍ଣ୍ଣସଂଖ୍ୟା prio)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା prev_prio = rt_rq->highest_prio.curr;

	ଯଦି (rt_rq->rt_nr_running) {

		WARN_ON(prio < prev_prio);

		/*
		 * This may have been our highest task, and therefore
		 * we may have some re-computation to do
		 */
		ଯଦି (prio == prev_prio) {
			ସଂରଚନା rt_prio_array *array = &rt_rq->active;

			rt_rq->highest_prio.curr =
				sched_find_first_bit(array->bitmap);
		}

	} ଅନ୍ୟଥା {
		rt_rq->highest_prio.curr = MAX_RT_PRIO-1;
	}

	dec_rt_prio_smp(rt_rq, prio, prev_prio);
}

#ifdef CONFIG_RT_GROUP_SCHED

ସ୍ଥିର ଶୂନ୍ୟ
inc_rt_group(ସଂରଚନା sched_rt_entity *rt_se, ସଂରଚନା rt_rq *rt_rq)
{
	ଯଦି (rt_se_boosted(rt_se))
		rt_rq->rt_nr_boosted++;

	start_rt_bandwidth(&rt_rq->tg->rt_bandwidth);
}

ସ୍ଥିର ଶୂନ୍ୟ
dec_rt_group(ସଂରଚନା sched_rt_entity *rt_se, ସଂରଚନା rt_rq *rt_rq)
{
	ଯଦି (rt_se_boosted(rt_se))
		rt_rq->rt_nr_boosted--;

	WARN_ON(!rt_rq->rt_nr_running && rt_rq->rt_nr_boosted);
}

#ଅନ୍ୟଥା /* !CONFIG_RT_GROUP_SCHED: */

ସ୍ଥିର ଶୂନ୍ୟ
inc_rt_group(ସଂରଚନା sched_rt_entity *rt_se, ସଂରଚନା rt_rq *rt_rq)
{
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ
ଶୂନ୍ୟ dec_rt_group(ସଂରଚନା sched_rt_entity *rt_se, ସଂରଚନା rt_rq *rt_rq) {}

#endif /* !CONFIG_RT_GROUP_SCHED */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ
unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା rt_se_nr_running(ସଂରଚନା sched_rt_entity *rt_se)
{
	ସଂରଚନା rt_rq *group_rq = group_rt_rq(rt_se);

	ଯଦି (group_rq)
		ଫେରନ୍ତୁ group_rq->rt_nr_running;
	ଅନ୍ୟଥା
		ଫେରନ୍ତୁ 1;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ
unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା rt_se_rr_nr_running(ସଂରଚନା sched_rt_entity *rt_se)
{
	ସଂରଚନା rt_rq *group_rq = group_rt_rq(rt_se);
	ସଂରଚନା task_struct *tsk;

	ଯଦି (group_rq)
		ଫେରନ୍ତୁ group_rq->rr_nr_running;

	tsk = rt_task_of(rt_se);

	ଫେରନ୍ତୁ (tsk->policy == SCHED_RR) ? 1 : 0;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ
ଶୂନ୍ୟ inc_rt_tasks(ସଂରଚନା sched_rt_entity *rt_se, ସଂରଚନା rt_rq *rt_rq)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା prio = rt_se_prio(rt_se);

	WARN_ON(!rt_prio(prio));
	rt_rq->rt_nr_running += rt_se_nr_running(rt_se);
	rt_rq->rr_nr_running += rt_se_rr_nr_running(rt_se);

	inc_rt_prio(rt_rq, prio);
	inc_rt_group(rt_se, rt_rq);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ
ଶୂନ୍ୟ dec_rt_tasks(ସଂରଚନା sched_rt_entity *rt_se, ସଂରଚନା rt_rq *rt_rq)
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
ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool move_entity(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ଯଦି ((flags & (DEQUEUE_SAVE | DEQUEUE_MOVE)) == DEQUEUE_SAVE)
		ଫେରନ୍ତୁ false;

	ଫେରନ୍ତୁ true;
}

ସ୍ଥିର ଶୂନ୍ୟ __delist_rt_entity(ସଂରଚନା sched_rt_entity *rt_se, ସଂରଚନା rt_prio_array *array)
{
	list_del_init(&rt_se->run_list);

	ଯଦି (list_empty(array->queue + rt_se_prio(rt_se)))
		__clear_bit(rt_se_prio(rt_se), array->bitmap);

	rt_se->on_list = 0;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା sched_statistics *
__schedstats_from_rt_se(ସଂରଚନା sched_rt_entity *rt_se)
{
	/* schedstats is not supported for rt group. */
	ଯଦି (!rt_entity_is_task(rt_se))
		ଫେରନ୍ତୁ ଖାଲି;

	ଫେରନ୍ତୁ &rt_task_of(rt_se)->stats;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
update_stats_wait_start_rt(ସଂରଚନା rt_rq *rt_rq, ସଂରଚନା sched_rt_entity *rt_se)
{
	ସଂରଚନା sched_statistics *stats;
	ସଂରଚନା task_struct *p = ଖାଲି;

	ଯଦି (!schedstat_enabled())
		ଫେରନ୍ତୁ;

	ଯଦି (rt_entity_is_task(rt_se))
		p = rt_task_of(rt_se);

	stats = __schedstats_from_rt_se(rt_se);
	ଯଦି (!stats)
		ଫେରନ୍ତୁ;

	__update_stats_wait_start(rq_of_rt_rq(rt_rq), p, stats);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
update_stats_enqueue_sleeper_rt(ସଂରଚନା rt_rq *rt_rq, ସଂରଚନା sched_rt_entity *rt_se)
{
	ସଂରଚନା sched_statistics *stats;
	ସଂରଚନା task_struct *p = ଖାଲି;

	ଯଦି (!schedstat_enabled())
		ଫେରନ୍ତୁ;

	ଯଦି (rt_entity_is_task(rt_se))
		p = rt_task_of(rt_se);

	stats = __schedstats_from_rt_se(rt_se);
	ଯଦି (!stats)
		ଫେରନ୍ତୁ;

	__update_stats_enqueue_sleeper(rq_of_rt_rq(rt_rq), p, stats);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
update_stats_enqueue_rt(ସଂରଚନା rt_rq *rt_rq, ସଂରଚନା sched_rt_entity *rt_se,
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ଯଦି (!schedstat_enabled())
		ଫେରନ୍ତୁ;

	ଯଦି (flags & ENQUEUE_WAKEUP)
		update_stats_enqueue_sleeper_rt(rt_rq, rt_se);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
update_stats_wait_end_rt(ସଂରଚନା rt_rq *rt_rq, ସଂରଚନା sched_rt_entity *rt_se)
{
	ସଂରଚନା sched_statistics *stats;
	ସଂରଚନା task_struct *p = ଖାଲି;

	ଯଦି (!schedstat_enabled())
		ଫେରନ୍ତୁ;

	ଯଦି (rt_entity_is_task(rt_se))
		p = rt_task_of(rt_se);

	stats = __schedstats_from_rt_se(rt_se);
	ଯଦି (!stats)
		ଫେରନ୍ତୁ;

	__update_stats_wait_end(rq_of_rt_rq(rt_rq), p, stats);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
update_stats_dequeue_rt(ସଂରଚନା rt_rq *rt_rq, ସଂରଚନା sched_rt_entity *rt_se,
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ସଂରଚନା task_struct *p = ଖାଲି;
	ସଂରଚନା rq *rq = rq_of_rt_rq(rt_rq);

	ଯଦି (!schedstat_enabled())
		ଫେରନ୍ତୁ;

	ଯଦି (rt_entity_is_task(rt_se)) {
		p = rt_task_of(rt_se);

		ଯଦି (p != rq->curr)
			update_stats_wait_end_rt(rt_rq, rt_se);
	}

	ଯଦି ((flags & DEQUEUE_SLEEP) && p) {
		unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା state;

		state = READ_ONCE(p->__state);
		ଯଦି (state & TASK_INTERRUPTIBLE)
			__schedstat_set(p->stats.sleep_start,
					rq_clock(rq_of_rt_rq(rt_rq)));

		ଯଦି (state & TASK_UNINTERRUPTIBLE)
			__schedstat_set(p->stats.block_start,
					rq_clock(rq_of_rt_rq(rt_rq)));
	}
}

ସ୍ଥିର ଶୂନ୍ୟ __enqueue_rt_entity(ସଂରଚନା sched_rt_entity *rt_se, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ସଂରଚନା rt_rq *rt_rq = rt_rq_of_se(rt_se);
	ସଂରଚନା rt_prio_array *array = &rt_rq->active;
	ସଂରଚନା rt_rq *group_rq = group_rt_rq(rt_se);
	ସଂରଚନା list_head *queue = array->queue + rt_se_prio(rt_se);

	/*
	 * Don't enqueue the group if its throttled, or when empty.
	 * The latter is a consequence of the former when a child group
	 * get throttled and the current group doesn't have any other
	 * active members.
	 */
	ଯଦି (group_rq && (rt_rq_throttled(group_rq) || !group_rq->rt_nr_running)) {
		ଯଦି (rt_se->on_list)
			__delist_rt_entity(rt_se, array);
		ଫେରନ୍ତୁ;
	}

	ଯଦି (move_entity(flags)) {
		WARN_ON_ONCE(rt_se->on_list);
		ଯଦି (flags & ENQUEUE_HEAD)
			list_add(&rt_se->run_list, queue);
		ଅନ୍ୟଥା
			list_add_tail(&rt_se->run_list, queue);

		__set_bit(rt_se_prio(rt_se), array->bitmap);
		rt_se->on_list = 1;
	}
	rt_se->on_rq = 1;

	inc_rt_tasks(rt_se, rt_rq);
}

ସ୍ଥିର ଶୂନ୍ୟ __dequeue_rt_entity(ସଂରଚନା sched_rt_entity *rt_se, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ସଂରଚନା rt_rq *rt_rq = rt_rq_of_se(rt_se);
	ସଂରଚନା rt_prio_array *array = &rt_rq->active;

	ଯଦି (move_entity(flags)) {
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
ସ୍ଥିର ଶୂନ୍ୟ dequeue_rt_ଗଦା(ସଂରଚନା sched_rt_entity *rt_se, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ସଂରଚନା sched_rt_entity *back = ଖାଲି;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା rt_nr_running;

	for_each_sched_rt_entity(rt_se) {
		rt_se->back = back;
		back = rt_se;
	}

	rt_nr_running = rt_rq_of_se(back)->rt_nr_running;

	ପାଇଁ (rt_se = back; rt_se; rt_se = rt_se->back) {
		ଯଦି (on_rt_rq(rt_se))
			__dequeue_rt_entity(rt_se, flags);
	}

	dequeue_ଶୀର୍ଷ_rt_rq(rt_rq_of_se(back), rt_nr_running);
}

ସ୍ଥିର ଶୂନ୍ୟ enqueue_rt_entity(ସଂରଚନା sched_rt_entity *rt_se, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ସଂରଚନା rq *rq = rq_of_rt_se(rt_se);

	update_stats_enqueue_rt(rt_rq_of_se(rt_se), rt_se, flags);

	dequeue_rt_ଗଦା(rt_se, flags);
	for_each_sched_rt_entity(rt_se)
		__enqueue_rt_entity(rt_se, flags);
	enqueue_ଶୀର୍ଷ_rt_rq(&rq->rt);
}

ସ୍ଥିର ଶୂନ୍ୟ dequeue_rt_entity(ସଂରଚନା sched_rt_entity *rt_se, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ସଂରଚନା rq *rq = rq_of_rt_se(rt_se);

	update_stats_dequeue_rt(rt_rq_of_se(rt_se), rt_se, flags);

	dequeue_rt_ଗଦା(rt_se, flags);

	for_each_sched_rt_entity(rt_se) {
		ସଂରଚନା rt_rq *rt_rq = group_rt_rq(rt_se);

		ଯଦି (rt_rq && rt_rq->rt_nr_running)
			__enqueue_rt_entity(rt_se, flags);
	}
	enqueue_ଶୀର୍ଷ_rt_rq(&rq->rt);
}

/*
 * Adding/removing a task to/from a priority array:
 */
ସ୍ଥିର ଶୂନ୍ୟ
enqueue_task_rt(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ସଂରଚନା sched_rt_entity *rt_se = &p->rt;

	ଯଦି (flags & ENQUEUE_WAKEUP)
		rt_se->timeout = 0;

	check_schedstat_required();
	update_stats_wait_start_rt(rt_rq_of_se(rt_se), rt_se);

	enqueue_rt_entity(rt_se, flags);

	ଯଦି (task_is_blocked(p))
		ଫେରନ୍ତୁ;

	ଯଦି (!task_current(rq, p) && p->nr_cpus_allowed > 1)
		enqueue_pushable_task(rq, p);
}

ସ୍ଥିର bool dequeue_task_rt(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ସଂରଚନା sched_rt_entity *rt_se = &p->rt;

	update_curr_rt(rq);
	dequeue_rt_entity(rt_se, flags);

	dequeue_pushable_task(rq, p);

	ଫେରନ୍ତୁ true;
}

/*
 * Put task to the head or the end of the run list without the overhead of
 * dequeue followed by enqueue.
 */
ସ୍ଥିର ଶୂନ୍ୟ
requeue_rt_entity(ସଂରଚନା rt_rq *rt_rq, ସଂରଚନା sched_rt_entity *rt_se, ପୂର୍ଣ୍ଣସଂଖ୍ୟା head)
{
	ଯଦି (on_rt_rq(rt_se)) {
		ସଂରଚନା rt_prio_array *array = &rt_rq->active;
		ସଂରଚନା list_head *queue = array->queue + rt_se_prio(rt_se);

		ଯଦି (head)
			list_move(&rt_se->run_list, queue);
		ଅନ୍ୟଥା
			list_move_tail(&rt_se->run_list, queue);
	}
}

ସ୍ଥିର ଶୂନ୍ୟ requeue_task_rt(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା head)
{
	ସଂରଚନା sched_rt_entity *rt_se = &p->rt;
	ସଂରଚନା rt_rq *rt_rq;

	for_each_sched_rt_entity(rt_se) {
		rt_rq = rt_rq_of_se(rt_se);
		requeue_rt_entity(rt_rq, rt_se, head);
	}
}

ସ୍ଥିର ଶୂନ୍ୟ yield_task_rt(ସଂରଚନା rq *rq)
{
	requeue_task_rt(rq, rq->donor, 0);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା find_lowest_rq(ସଂରଚନା task_struct *task);

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
select_task_rq_rt(ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ସଂରଚନା task_struct *curr, *donor;
	ସଂରଚନା rq *rq;
	bool test;

	/* For anything but wake ups, just return the task_cpu */
	ଯଦି (!(flags & (WF_TTWU | WF_FORK)))
		ଯାଅ out;

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

	ଯଦି (test || !rt_task_fits_capacity(p, cpu)) {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା target = find_lowest_rq(p);

		/*
		 * Bail out if we were forcing a migration to find a better
		 * fitting CPU but our search failed.
		 */
		ଯଦି (!test && target != -1 && !rt_task_fits_capacity(p, target))
			ଯାଅ out_unlock;

		/*
		 * Don't bother moving it if the destination CPU is
		 * not running a lower priority task.
		 */
		ଯଦି (target != -1 &&
		    p->prio < cpu_rq(target)->rt.highest_prio.curr)
			cpu = target;
	}

out_unlock:
	rcu_read_unlock();

out:
	ଫେରନ୍ତୁ cpu;
}

ସ୍ଥିର ଶୂନ୍ୟ check_preempt_equal_prio(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p)
{
	ଯଦି (rq->curr->nr_cpus_allowed == 1 ||
	    !cpupri_find(&rq->rd->cpupri, rq->donor, ଖାଲି))
		ଫେରନ୍ତୁ;

	/*
	 * p is migratable, so let's not schedule it and
	 * see if it is pushed or pulled somewhere else.
	 */
	ଯଦି (p->nr_cpus_allowed != 1 &&
	    cpupri_find(&rq->rd->cpupri, p, ଖାଲି))
		ଫେରନ୍ତୁ;

	/*
	 * There appear to be other CPUs that can accept
	 * the current task but none can run 'p', so lets reschedule
	 * to try and push the current task away:
	 */
	requeue_task_rt(rq, p, 1);
	resched_curr(rq);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା balance_rt(ସଂରଚନା rq *rq, ସଂରଚନା rq_flags *rf)
{
	/*
	 * Note, rq->donor may change during rq lock drops,
	 * so don't re-use p across lock drops
	 */
	ସଂରଚନା task_struct *p = rq->donor;

	ଯଦି (!on_rt_rq(&p->rt) && need_pull_rt_task(rq, p)) {
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

	ଫେରନ୍ତୁ sched_stop_runnable(rq) || sched_dl_runnable(rq) || sched_rt_runnable(rq);
}

/*
 * Preempt the current task with a newly woken task if needed:
 */
ସ୍ଥିର ଶୂନ୍ୟ wakeup_preempt_rt(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ସଂରଚନା task_struct *donor = rq->donor;

	/*
	 * XXX If we're preempted by DL, queue a push?
	 */
	ଯଦି (p->sched_class != &rt_sched_class ||
	    donor->sched_class != &rt_sched_class)
		ଫେରନ୍ତୁ;

	ଯଦି (p->prio < donor->prio) {
		resched_curr(rq);
		ଫେରନ୍ତୁ;
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
	ଯଦି (p->prio == donor->prio && !test_tsk_need_resched(rq->curr))
		check_preempt_equal_prio(rq, p);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ set_next_task_rt(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p, ଗଣନା snt_e type)
{
	ସଂରଚନା sched_rt_entity *rt_se = &p->rt;
	ସଂରଚନା rt_rq *rt_rq = &rq->rt;

	ଯଦି (type == SNT_REPICK)
		ଫେରନ୍ତୁ;

	p->se.exec_start = rq_clock_task(rq);
	ଯଦି (on_rt_rq(&p->rt))
		update_stats_wait_end_rt(rt_rq, rt_se);

	/* The running task is never eligible for pushing */
	dequeue_pushable_task(rq, p);

	ଯଦି (type != SNT_PICK)
		ଫେରନ୍ତୁ;

	/*
	 * If prev task was rt, put_prev_task() has already updated the
	 * utilization. We only care of the case where we start to schedule a
	 * rt task
	 */
	ଯଦି (rq->donor->sched_class != &rt_sched_class)
		update_rt_rq_load_avg(rq_clock_pelt(rq), rq, 0);

	rt_queue_ଠେଲ_tasks(rq);
}

ସ୍ଥିର ସଂରଚନା sched_rt_entity *pick_next_rt_entity(ସଂରଚନା rt_rq *rt_rq)
{
	ସଂରଚନା rt_prio_array *array = &rt_rq->active;
	ସଂରଚନା sched_rt_entity *next = ଖାଲି;
	ସଂରଚନା list_head *queue;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା idx;

	idx = sched_find_first_bit(array->bitmap);
	BUG_ON(idx >= MAX_RT_PRIO);

	queue = array->queue + idx;
	ଯଦି (WARN_ON_ONCE(list_empty(queue)))
		ଫେରନ୍ତୁ ଖାଲି;
	next = list_entry(queue->next, ସଂରଚନା sched_rt_entity, run_list);

	ଫେରନ୍ତୁ next;
}

ସ୍ଥିର ସଂରଚନା task_struct *_pick_next_task_rt(ସଂରଚନା rq *rq)
{
	ସଂରଚନା sched_rt_entity *rt_se;
	ସଂରଚନା rt_rq *rt_rq  = &rq->rt;

	କର {
		rt_se = pick_next_rt_entity(rt_rq);
		ଯଦି (unlikely(!rt_se))
			ଫେରନ୍ତୁ ଖାଲି;
		rt_rq = group_rt_rq(rt_se);
	} ଯେତେବେଳେ (rt_rq);

	ଫେରନ୍ତୁ rt_task_of(rt_se);
}

ସ୍ଥିର ସଂରଚନା task_struct *pick_task_rt(ସଂରଚନା rq *rq, ସଂରଚନା rq_flags *rf)
{
	ସଂରଚନା task_struct *p;

	ଯଦି (!sched_rt_runnable(rq))
		ଫେରନ୍ତୁ ଖାଲି;

	p = _pick_next_task_rt(rq);

	ଫେରନ୍ତୁ p;
}

ସ୍ଥିର ଶୂନ୍ୟ put_prev_task_rt(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p, ସଂରଚନା task_struct *next)
{
	ସଂରଚନା sched_rt_entity *rt_se = &p->rt;
	ସଂରଚନା rt_rq *rt_rq = &rq->rt;

	ଯଦି (on_rt_rq(&p->rt))
		update_stats_wait_start_rt(rt_rq, rt_se);

	update_curr_rt(rq);

	update_rt_rq_load_avg(rq_clock_pelt(rq), rq, 1);

	ଯଦି (task_is_blocked(p))
		ଫେରନ୍ତୁ;
	/*
	 * The previous task needs to be made eligible for pushing
	 * if it is still active
	 */
	ଯଦି (on_rt_rq(&p->rt) && p->nr_cpus_allowed > 1)
		enqueue_pushable_task(rq, p);
}

/* Only try algorithms three times */
#define RT_MAX_TRIES 3

/*
 * Return the highest pushable rq's task, which is suitable to be executed
 * on the CPU, NULL otherwise
 */
ସ୍ଥିର ସଂରଚନା task_struct *pick_highest_pushable_task(ସଂରଚନା rq *rq, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ସଂରଚନା plist_head *head = &rq->rt.pushable_tasks;
	ସଂରଚନା task_struct *p;

	ଯଦି (!has_pushable_tasks(rq))
		ଫେରନ୍ତୁ ଖାଲି;

	plist_for_each_entry(p, head, pushable_tasks) {
		ଯଦି (task_is_pushable(rq, p, cpu))
			ଫେରନ୍ତୁ p;
	}

	ଫେରନ୍ତୁ ଖାଲି;
}

ସ୍ଥିର DEFINE_PER_CPU(cpumask_var_t, local_cpu_mask);

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା find_lowest_rq(ସଂରଚନା task_struct *task)
{
	ସଂରଚନା sched_domain *sd;
	ସଂରଚନା cpumask *lowest_mask = this_cpu_cpumask_var_ptr(local_cpu_mask);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା this_cpu = smp_processor_id();
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu      = task_cpu(task);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	/* Make sure the mask is initialized first */
	ଯଦି (unlikely(!lowest_mask))
		ଫେରନ୍ତୁ -1;

	ଯଦି (task->nr_cpus_allowed == 1)
		ଫେରନ୍ତୁ -1; /* No other targets possible */

	/*
	 * If we're on asym system ensure we consider the different capacities
	 * of the CPUs when searching for the lowest_mask.
	 */
	ଯଦି (sched_asym_cpucap_active()) {

		ret = cpupri_find_fitness(&task_rq(task)->rd->cpupri,
					  task, lowest_mask,
					  rt_task_fits_capacity);
	} ଅନ୍ୟଥା {

		ret = cpupri_find(&task_rq(task)->rd->cpupri,
				  task, lowest_mask);
	}

	ଯଦି (!ret)
		ଫେରନ୍ତୁ -1; /* No targets found */

	/*
	 * At this point we have built a mask of CPUs representing the
	 * lowest priority tasks in the system.  Now we want to elect
	 * the best one based on our affinity and topology.
	 *
	 * We prioritize the last CPU that the task executed on since
	 * it is most likely cache-hot in that location.
	 */
	ଯଦି (cpumask_test_cpu(cpu, lowest_mask))
		ଫେରନ୍ତୁ cpu;

	/*
	 * Otherwise, we consult the sched_domains span maps to figure
	 * out which CPU is logically closest to our hot cache data.
	 */
	ଯଦି (!cpumask_test_cpu(this_cpu, lowest_mask))
		this_cpu = -1; /* Skip this_cpu opt if not among lowest */

	rcu_read_lock();
	for_each_domain(cpu, sd) {
		ଯଦି (sd->flags & SD_WAKE_AFFINE) {
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା best_cpu;

			/*
			 * "this_cpu" is cheaper to preempt than a
			 * remote processor.
			 */
			ଯଦି (this_cpu != -1 &&
			    cpumask_test_cpu(this_cpu, sched_domain_span(sd))) {
				rcu_read_unlock();
				ଫେରନ୍ତୁ this_cpu;
			}

			best_cpu = cpumask_any_and_distribute(lowest_mask,
							      sched_domain_span(sd));
			ଯଦି (best_cpu < nr_cpu_ids) {
				rcu_read_unlock();
				ଫେରନ୍ତୁ best_cpu;
			}
		}
	}
	rcu_read_unlock();

	/*
	 * And finally, if there were no matches within the domains
	 * just give the caller *something* to work with from the compatible
	 * locations.
	 */
	ଯଦି (this_cpu != -1)
		ଫେରନ୍ତୁ this_cpu;

	cpu = cpumask_any_distribute(lowest_mask);
	ଯଦି (cpu < nr_cpu_ids)
		ଫେରନ୍ତୁ cpu;

	ଫେରନ୍ତୁ -1;
}

ସ୍ଥିର ସଂରଚନା task_struct *pick_next_pushable_task(ସଂରଚନା rq *rq)
{
	ସଂରଚନା plist_head *head = &rq->rt.pushable_tasks;
	ସଂରଚନା task_struct *i, *p = ଖାଲି;

	ଯଦି (!has_pushable_tasks(rq))
		ଫେରନ୍ତୁ ଖାଲି;

	plist_for_each_entry(i, head, pushable_tasks) {
		/* skip tasks that cannot be migrated */
		ଯଦି (!task_on_cpu(rq, i) && !is_migration_disabled(i)) {
			p = i;
			ଭାଙ୍ଗ;
		}
	}

	ଯଦି (!p)
		ଫେରନ୍ତୁ ଖାଲି;

	BUG_ON(rq->cpu != task_cpu(p));
	BUG_ON(task_current(rq, p));
	BUG_ON(task_current_donor(rq, p));
	BUG_ON(p->nr_cpus_allowed <= 1);

	BUG_ON(!task_on_rq_queued(p));
	BUG_ON(!rt_task(p));

	ଫେରନ୍ତୁ p;
}

/* Will lock the rq it finds */
ସ୍ଥିର ସଂରଚନା rq *find_lock_lowest_rq(ସଂରଚନା task_struct *task, ସଂରଚନା rq *rq)
{
	ସଂରଚନା rq *lowest_rq = ଖାଲି;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା tries;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu;

	ପାଇଁ (tries = 0; tries < RT_MAX_TRIES; tries++) {
		cpu = find_lowest_rq(task);

		ଯଦି ((cpu == -1) || (cpu == rq->cpu))
			ଭାଙ୍ଗ;

		lowest_rq = cpu_rq(cpu);

		ଯଦି (lowest_rq->rt.highest_prio.curr <= task->prio) {
			/*
			 * Target rq has tasks of equal or higher priority,
			 * retrying does not release any lock and is unlikely
			 * to yield a different result.
			 */
			lowest_rq = ଖାଲି;
			ଭାଙ୍ଗ;
		}

		/* if the prio of this runqueue changed, try again */
		ଯଦି (double_lock_balance(rq, lowest_rq)) {
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
			ଯଦି (unlikely(is_migration_disabled(task) ||
				     !cpumask_test_cpu(lowest_rq->cpu, &task->cpus_mask) ||
				     task != pick_next_pushable_task(rq))) {

				double_unlock_balance(rq, lowest_rq);
				lowest_rq = ଖାଲି;
				ଭାଙ୍ଗ;
			}
		}

		/* If this rq is still suitable use it. */
		ଯଦି (lowest_rq->rt.highest_prio.curr > task->prio)
			ଭାଙ୍ଗ;

		/* try again */
		double_unlock_balance(rq, lowest_rq);
		lowest_rq = ଖାଲି;
	}

	ଫେରନ୍ତୁ lowest_rq;
}

/*
 * If the current CPU has more than one RT task, see if the non
 * running task can migrate over to a CPU that is running a task
 * of lesser priority.
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଠେଲ_rt_task(ସଂରଚନା rq *rq, bool pull)
{
	ସଂରଚନା task_struct *next_task;
	ସଂରଚନା rq *lowest_rq;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;

	ଯଦି (!rq->rt.overloaded)
		ଫେରନ୍ତୁ 0;

	next_task = pick_next_pushable_task(rq);
	ଯଦି (!next_task)
		ଫେରନ୍ତୁ 0;

retry:
	/*
	 * It's possible that the next_task slipped in of
	 * higher priority than current. If that's the case
	 * just reschedule current.
	 */
	ଯଦି (unlikely(next_task->prio < rq->donor->prio)) {
		resched_curr(rq);
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (is_migration_disabled(next_task)) {
		ସଂରଚନା task_struct *ଠେଲ_task = ଖାଲି;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu;

		ଯଦି (!pull || rq->ଠେଲ_busy)
			ଫେରନ୍ତୁ 0;

		/*
		 * Invoking find_lowest_rq() on anything but an RT task doesn't
		 * make sense. Per the above priority check, curr has to
		 * be of higher priority than next_task, so no need to
		 * reschedule when bailing out.
		 *
		 * Note that the stoppers are masqueraded as SCHED_FIFO
		 * (cf. sched_set_stop_task()), so we can't rely on rt_task().
		 */
		ଯଦି (rq->donor->sched_class != &rt_sched_class)
			ଫେରନ୍ତୁ 0;

		cpu = find_lowest_rq(rq->curr);
		ଯଦି (cpu == -1 || cpu == rq->cpu)
			ଫେରନ୍ତୁ 0;

		/*
		 * Given we found a CPU with lower priority than @next_task,
		 * therefore it should be running. However we cannot migrate it
		 * to this other CPU, instead attempt to push the current
		 * running task on this CPU away.
		 */
		ଠେଲ_task = get_ଠେଲ_task(rq);
		ଯଦି (ଠେଲ_task) {
			preempt_disable();
			raw_spin_rq_unlock(rq);
			stop_one_cpu_nowait(rq->cpu, ଠେଲ_cpu_stop,
					    ଠେଲ_task, &rq->ଠେଲ_work);
			preempt_enable();
			raw_spin_rq_lock(rq);
		}

		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (WARN_ON(next_task == rq->curr))
		ଫେରନ୍ତୁ 0;

	/* We might release rq lock */
	get_task_struct(next_task);

	/* find_lock_lowest_rq locks the rq if found */
	lowest_rq = find_lock_lowest_rq(next_task, rq);
	ଯଦି (!lowest_rq) {
		ସଂରଚନା task_struct *task;
		/*
		 * find_lock_lowest_rq releases rq->lock
		 * so it is possible that next_task has migrated.
		 *
		 * We need to make sure that the task is still on the same
		 * run-queue and is also still the next task eligible for
		 * pushing.
		 */
		task = pick_next_pushable_task(rq);
		ଯଦି (task == next_task) {
			/*
			 * The task hasn't migrated, and is still the next
			 * eligible task, but we failed to find a run-queue
			 * to push it to.  Do not retry in this case, since
			 * other CPUs will pull from us when ready.
			 */
			ଯାଅ out;
		}

		ଯଦି (!task)
			/* No more tasks, just exit */
			ଯାଅ out;

		/*
		 * Something has shifted, try again.
		 */
		put_task_struct(next_task);
		next_task = task;
		ଯାଅ retry;
	}

	move_queued_task_locked(rq, lowest_rq, next_task);
	resched_curr(lowest_rq);
	ret = 1;

	double_unlock_balance(rq, lowest_rq);
out:
	put_task_struct(next_task);

	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ଶୂନ୍ୟ ଠେଲ_rt_tasks(ସଂରଚନା rq *rq)
{
	/* push_rt_task will return true if it moved an RT */
	ଯେତେବେଳେ (ଠେଲ_rt_task(rq, false))
		;
}

#ifdef HAVE_RT_ଠେଲ_IPI

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
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା rto_next_cpu(ସଂରଚନା ମୂଳ_domain *rd)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା this_cpu = smp_processor_id();
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା next;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu;

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
	ପାଇଁ (;;) {

		/* When rto_cpu is -1 this acts like cpumask_first() */
		cpu = cpumask_next(rd->rto_cpu, rd->rto_mask);

		rd->rto_cpu = cpu;

		/* Do not send IPI to self */
		ଯଦି (cpu == this_cpu)
			ଜାରି;

		ଯଦି (cpu < nr_cpu_ids)
			ଫେରନ୍ତୁ cpu;

		rd->rto_cpu = -1;

		/*
		 * ACQUIRE ensures we see the @rto_mask changes
		 * made prior to the @next value observed.
		 *
		 * Matches WMB in rt_set_overload().
		 */
		next = atomic_read_acquire(&rd->rto_loop_next);

		ଯଦି (rd->rto_loop == next)
			ଭାଙ୍ଗ;

		rd->rto_loop = next;
	}

	ଫେରନ୍ତୁ -1;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool rto_start_trylock(atomic_t *v)
{
	ଫେରନ୍ତୁ !atomic_cmpxchg_acquire(v, 0, 1);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ rto_start_unlock(atomic_t *v)
{
	atomic_set_release(v, 0);
}

ସ୍ଥିର ଶୂନ୍ୟ tell_cpu_to_ଠେଲ(ସଂରଚନା rq *rq)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu = -1;

	/* Keep the loop going if the IPI is currently active */
	atomic_inc(&rq->rd->rto_loop_next);

	/* Only one CPU can initiate a loop at a time */
	ଯଦି (!rto_start_trylock(&rq->rd->rto_loop_start))
		ଫେରନ୍ତୁ;

	raw_spin_lock(&rq->rd->rto_lock);

	/*
	 * The rto_cpu is updated under the lock, if it has a valid CPU
	 * then the IPI is still running and will continue due to the
	 * update to loop_next, and nothing needs to be done here.
	 * Otherwise it is finishing up and an IPI needs to be sent.
	 */
	ଯଦି (rq->rd->rto_cpu < 0)
		cpu = rto_next_cpu(rq->rd);

	raw_spin_unlock(&rq->rd->rto_lock);

	rto_start_unlock(&rq->rd->rto_loop_start);

	ଯଦି (cpu >= 0) {
		/* Make sure the rd does not get freed while pushing */
		sched_get_rd(rq->rd);
		irq_work_queue_on(&rq->rd->rto_ଠେଲ_work, cpu);
	}
}

/* Called from hardirq context */
ଶୂନ୍ୟ rto_ଠେଲ_irq_work_func(ସଂରଚନା irq_work *work)
{
	ସଂରଚନା ମୂଳ_domain *rd =
		container_of(work, ସଂରଚନା ମୂଳ_domain, rto_ଠେଲ_work);
	ସଂରଚନା rq *rq;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu;

	rq = this_rq();

	/*
	 * We do not need to grab the lock to check for has_pushable_tasks.
	 * When it gets updated, a check is made if a push is possible.
	 */
	ଯଦି (has_pushable_tasks(rq)) {
		raw_spin_rq_lock(rq);
		ଯେତେବେଳେ (ଠେଲ_rt_task(rq, true))
			;
		raw_spin_rq_unlock(rq);
	}

	raw_spin_lock(&rd->rto_lock);

	/* Pass the IPI to the next rt overloaded queue */
	cpu = rto_next_cpu(rd);

	raw_spin_unlock(&rd->rto_lock);

	ଯଦି (cpu < 0) {
		sched_put_rd(rd);
		ଫେରନ୍ତୁ;
	}

	/* Try the next RT overloaded CPU */
	irq_work_queue_on(&rd->rto_ଠେଲ_work, cpu);
}
#endif /* HAVE_RT_PUSH_IPI */

ସ୍ଥିର ଶୂନ୍ୟ pull_rt_task(ସଂରଚନା rq *this_rq)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା this_cpu = this_rq->cpu, cpu;
	bool resched = false;
	ସଂରଚନା task_struct *p, *ଠେଲ_task;
	ସଂରଚନା rq *src_rq;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା rt_overload_ଗଣନା = rt_overloaded(this_rq);

	ଯଦି (likely(!rt_overload_ଗଣନା))
		ଫେରନ୍ତୁ;

	/*
	 * Match the barrier from rt_set_overloaded; this guarantees that if we
	 * see overloaded we must also see the rto_mask bit.
	 */
	smp_rmb();

	/* If we are the only overloaded CPU do nothing */
	ଯଦି (rt_overload_ଗଣନା == 1 &&
	    cpumask_test_cpu(this_rq->cpu, this_rq->rd->rto_mask))
		ଫେରନ୍ତୁ;

#ifdef HAVE_RT_ଠେଲ_IPI
	ଯଦି (sched_feat(RT_ଠେଲ_IPI)) {
		tell_cpu_to_ଠେଲ(this_rq);
		ଫେରନ୍ତୁ;
	}
#endif

	for_each_cpu(cpu, this_rq->rd->rto_mask) {
		ଯଦି (this_cpu == cpu)
			ଜାରି;

		src_rq = cpu_rq(cpu);

		/*
		 * Don't bother taking the src_rq->lock if the next highest
		 * task is known to be lower-priority than our current task.
		 * This may look racy, but if this value is about to go
		 * logically higher, the src_rq will push this task away.
		 * And if its going logically lower, we do not care
		 */
		ଯଦି (src_rq->rt.highest_prio.next >=
		    this_rq->rt.highest_prio.curr)
			ଜାରି;

		/*
		 * We can potentially drop this_rq's lock in
		 * double_lock_balance, and another CPU could
		 * alter this_rq
		 */
		ଠେଲ_task = ଖାଲି;
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
		ଯଦି (p && (p->prio < this_rq->rt.highest_prio.curr)) {
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
			ଯଦି (p->prio < src_rq->donor->prio)
				ଯାଅ skip;

			ଯଦି (is_migration_disabled(p)) {
				ଠେଲ_task = get_ଠେଲ_task(src_rq);
			} ଅନ୍ୟଥା {
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

		ଯଦି (ଠେଲ_task) {
			preempt_disable();
			raw_spin_rq_unlock(this_rq);
			stop_one_cpu_nowait(src_rq->cpu, ଠେଲ_cpu_stop,
					    ଠେଲ_task, &src_rq->ଠେଲ_work);
			preempt_enable();
			raw_spin_rq_lock(this_rq);
		}
	}

	ଯଦି (resched)
		resched_curr(this_rq);
}

/*
 * If we are not running and we are not going to reschedule soon, we should
 * try to push tasks away now
 */
ସ୍ଥିର ଶୂନ୍ୟ task_woken_rt(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p)
{
	bool need_to_ଠେଲ = !task_on_cpu(rq, p) &&
			    !test_tsk_need_resched(rq->curr) &&
			    p->nr_cpus_allowed > 1 &&
			    (dl_task(rq->donor) || rt_task(rq->donor)) &&
			    (rq->curr->nr_cpus_allowed < 2 ||
			     rq->donor->prio <= p->prio);

	ଯଦି (need_to_ଠେଲ)
		ଠେଲ_rt_tasks(rq);
}

/* Assumes rq->lock is held */
ସ୍ଥିର ଶୂନ୍ୟ rq_online_rt(ସଂରଚନା rq *rq)
{
	ଯଦି (rq->rt.overloaded)
		rt_set_overload(rq);

	__enable_runtime(rq);

	cpupri_set(&rq->rd->cpupri, rq->cpu, rq->rt.highest_prio.curr);
}

/* Assumes rq->lock is held */
ସ୍ଥିର ଶୂନ୍ୟ rq_offline_rt(ସଂରଚନା rq *rq)
{
	ଯଦି (rq->rt.overloaded)
		rt_clear_overload(rq);

	__disable_runtime(rq);

	cpupri_set(&rq->rd->cpupri, rq->cpu, CPUPRI_INVALID);
}

/*
 * When switch from the rt queue, we bring ourselves to a position
 * that we might want to pull RT tasks from other runqueues.
 */
ସ୍ଥିର ଶୂନ୍ୟ switched_from_rt(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p)
{
	/*
	 * If there are other RT tasks then we will reschedule
	 * and the scheduling of the other RT tasks will handle
	 * the balancing. But if we are the last RT task
	 * we may need to handle the pulling of RT tasks
	 * now.
	 */
	ଯଦି (!task_on_rq_queued(p) || rq->rt.rt_nr_running)
		ଫେରନ୍ତୁ;

	rt_queue_pull_task(rq);
}

ଶୂନ୍ୟ __init init_sched_rt_class(ଶୂନ୍ୟ)
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	for_each_possible_cpu(i) {
		zalloc_cpumask_var_ଗଣ୍ଠି(&per_cpu(local_cpu_mask, i),
					GFP_KERNEL, cpu_to_ଗଣ୍ଠି(i));
	}
}

/*
 * When switching a task to RT, we may overload the runqueue
 * with RT tasks. In this case we try to push them off to
 * other runqueues.
 */
ସ୍ଥିର ଶୂନ୍ୟ switched_to_rt(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p)
{
	/*
	 * If we are running, update the avg_rt tracking, as the running time
	 * will now on be accounted into the latter.
	 */
	ଯଦି (task_current(rq, p)) {
		update_rt_rq_load_avg(rq_clock_pelt(rq), rq, 0);
		ଫେରନ୍ତୁ;
	}

	/*
	 * If we are not running we may need to preempt the current
	 * running task. If that current running task is also an RT task
	 * then see if we can move to another run queue.
	 */
	ଯଦି (task_on_rq_queued(p)) {
		ଯଦି (p->nr_cpus_allowed > 1 && rq->rt.overloaded)
			rt_queue_ଠେଲ_tasks(rq);
		ଯଦି (p->prio < rq->donor->prio && cpu_online(cpu_of(rq)))
			resched_curr(rq);
	}
}

/*
 * Priority of the task has changed. This may cause
 * us to initiate a push or pull.
 */
ସ୍ଥିର ଶୂନ୍ୟ
prio_changed_rt(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p, u64 oldprio)
{
	ଯଦି (!task_on_rq_queued(p))
		ଫେରନ୍ତୁ;

	ଯଦି (p->prio == oldprio)
		ଫେରନ୍ତୁ;

	ଯଦି (task_current_donor(rq, p)) {
		/*
		 * If our priority decreases while running, we
		 * may need to pull tasks to this runqueue.
		 */
		ଯଦି (oldprio < p->prio)
			rt_queue_pull_task(rq);

		/*
		 * If there's a higher priority task waiting to run
		 * then reschedule.
		 */
		ଯଦି (p->prio > rq->rt.highest_prio.curr)
			resched_curr(rq);
	} ଅନ୍ୟଥା {
		/*
		 * This task is not running, but if it is
		 * greater than the current running task
		 * then reschedule.
		 */
		ଯଦି (p->prio < rq->donor->prio)
			resched_curr(rq);
	}
}

#ifdef CONFIG_POSIX_TIMERS
ସ୍ଥିର ଶୂନ୍ୟ watchdog(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p)
{
	unsigned long soft, hard;

	/* max may change after cur was read, this will be fixed next tick */
	soft = task_rlimit(p, RLIMIT_RTTIME);
	hard = task_rlimit_max(p, RLIMIT_RTTIME);

	ଯଦି (soft != RLIM_INFINITY) {
		unsigned long next;

		ଯଦି (p->rt.watchdog_stamp != jiffies) {
			p->rt.timeout++;
			p->rt.watchdog_stamp = jiffies;
		}

		next = DIV_ROUND_UP(min(soft, hard), USEC_PER_SEC/HZ);
		ଯଦି (p->rt.timeout > next) {
			posix_cputimers_rt_watchdog(&p->posix_cputimers,
						    p->se.sum_exec_runtime);
		}
	}
}
#ଅନ୍ୟଥା /* !CONFIG_POSIX_TIMERS: */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ watchdog(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p) { }
#endif /* !CONFIG_POSIX_TIMERS */

/*
 * scheduler tick hitting a task of our scheduling class.
 *
 * NOTE: This function can be called remotely by the tick offload that
 * goes along full dynticks. Therefore no local assumption can be made
 * and everything must be accessed through the @rq and @curr passed in
 * parameters.
 */
ସ୍ଥିର ଶୂନ୍ୟ task_tick_rt(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା queued)
{
	ସଂରଚନା sched_rt_entity *rt_se = &p->rt;

	update_curr_rt(rq);
	update_rt_rq_load_avg(rq_clock_pelt(rq), rq, 1);

	watchdog(rq, p);

	/*
	 * RR tasks need a special form of time-slice management.
	 * FIFO tasks have no timeslices.
	 */
	ଯଦି (p->policy != SCHED_RR)
		ଫେରନ୍ତୁ;

	ଯଦି (--p->rt.time_slice)
		ଫେରନ୍ତୁ;

	p->rt.time_slice = sched_rr_timeslice;

	/*
	 * Requeue to the end of queue if we (and all of our ancestors) are not
	 * the only element on the queue
	 */
	for_each_sched_rt_entity(rt_se) {
		ଯଦି (rt_se->run_list.prev != rt_se->run_list.next) {
			requeue_task_rt(rq, p, 0);
			resched_curr(rq);
			ଫେରନ୍ତୁ;
		}
	}
}

ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_rr_interval_rt(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *task)
{
	/*
	 * Time slice is 0 for SCHED_FIFO tasks
	 */
	ଯଦି (task->policy == SCHED_RR)
		ଫେରନ୍ତୁ sched_rr_timeslice;
	ଅନ୍ୟଥା
		ଫେରନ୍ତୁ 0;
}

#ifdef CONFIG_SCHED_CORE
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା task_is_throttled_rt(ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ସଂରଚନା rt_rq *rt_rq;

#ifdef CONFIG_RT_GROUP_SCHED // XXX maybe add task_rt_rq(), see also sched_rt_period_rt_rq
	rt_rq = task_group(p)->rt_rq[cpu];
	WARN_ON(!rt_group_sched_enabled() && rt_rq->tg != &ମୂଳ_task_group);
#ଅନ୍ୟଥା
	rt_rq = &cpu_rq(cpu)->rt;
#endif

	ଫେରନ୍ତୁ rt_rq_throttled(rt_rq);
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
ସ୍ଥିର DEFINE_MUTEX(rt_constraints_mutex);

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା tg_has_rt_tasks(ସଂରଚନା task_group *tg)
{
	ସଂରଚନା task_struct *task;
	ସଂରଚନା css_task_iter it;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;

	/*
	 * Autogroups do not have RT tasks; see autogroup_create().
	 */
	ଯଦି (task_group_is_autogroup(tg))
		ଫେରନ୍ତୁ 0;

	css_task_iter_start(&tg->css, 0, &it);
	ଯେତେବେଳେ (!ret && (task = css_task_iter_next(&it)))
		ret |= rt_task(task);
	css_task_iter_end(&it);

	ଫେରନ୍ତୁ ret;
}

ସଂରଚନା rt_schedulable_data {
	ସଂରଚନା task_group *tg;
	u64 rt_period;
	u64 rt_runtime;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା tg_rt_schedulable(ସଂରଚନା task_group *tg, ଶୂନ୍ୟ *data)
{
	ସଂରଚନା rt_schedulable_data *d = data;
	ସଂରଚନା task_group *child;
	u64 total, sum = 0;
	u64 period, runtime;

	period = ktime_to_ns(tg->rt_bandwidth.rt_period);
	runtime = tg->rt_bandwidth.rt_runtime;

	ଯଦି (tg == d->tg) {
		period = d->rt_period;
		runtime = d->rt_runtime;
	}

	/*
	 * Cannot have more runtime than the period.
	 */
	ଯଦି (runtime > period && runtime != RUNTIME_INF)
		ଫେରନ୍ତୁ -EINVAL;

	/*
	 * Ensure we don't starve existing RT tasks if runtime turns zero.
	 */
	ଯଦି (rt_bandwidth_enabled() && !runtime &&
	    tg->rt_bandwidth.rt_runtime && tg_has_rt_tasks(tg))
		ଫେରନ୍ତୁ -EBUSY;

	total = to_ratio(period, runtime);

	/*
	 * Nobody can have more than the global setting allows.
	 */
	ଯଦି (total > to_ratio(global_rt_period(), global_rt_runtime()))
		ଫେରନ୍ତୁ -EINVAL;

	/*
	 * The sum of our children's runtime should not exceed our own.
	 */
	list_for_each_entry_rcu(child, &tg->children, siblings) {
		period = ktime_to_ns(child->rt_bandwidth.rt_period);
		runtime = child->rt_bandwidth.rt_runtime;

		ଯଦି (child == d->tg) {
			period = d->rt_period;
			runtime = d->rt_runtime;
		}

		sum += to_ratio(period, runtime);
	}

	ଯଦି (sum > total)
		ଫେରନ୍ତୁ -EINVAL;

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା __rt_schedulable(ସଂରଚନା task_group *tg, u64 period, u64 runtime)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	ସଂରଚନା rt_schedulable_data data = {
		.tg = tg,
		.rt_period = period,
		.rt_runtime = runtime,
	};

	rcu_read_lock();
	ret = walk_tg_ବୃକ୍ଷ(tg_rt_schedulable, tg_nop, &data);
	rcu_read_unlock();

	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା tg_set_rt_bandwidth(ସଂରଚନା task_group *tg,
		u64 rt_period, u64 rt_runtime)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, err = 0;

	/*
	 * Disallowing the root group RT runtime is BAD, it would disallow the
	 * kernel creating (and or operating) RT threads.
	 */
	ଯଦି (tg == &ମୂଳ_task_group && rt_runtime == 0)
		ଫେରନ୍ତୁ -EINVAL;

	/* No period doesn't make any sense. */
	ଯଦି (rt_period == 0)
		ଫେରନ୍ତୁ -EINVAL;

	/*
	 * Bound quota to defend quota against overflow during bandwidth shift.
	 */
	ଯଦି (rt_runtime != RUNTIME_INF && rt_runtime > max_rt_runtime)
		ଫେରନ୍ତୁ -EINVAL;

	mutex_lock(&rt_constraints_mutex);
	err = __rt_schedulable(tg, rt_period, rt_runtime);
	ଯଦି (err)
		ଯାଅ unlock;

	raw_spin_lock_irq(&tg->rt_bandwidth.rt_runtime_lock);
	tg->rt_bandwidth.rt_period = ns_to_ktime(rt_period);
	tg->rt_bandwidth.rt_runtime = rt_runtime;

	for_each_possible_cpu(i) {
		ସଂରଚନା rt_rq *rt_rq = tg->rt_rq[i];

		raw_spin_lock(&rt_rq->rt_runtime_lock);
		rt_rq->rt_runtime = rt_runtime;
		raw_spin_unlock(&rt_rq->rt_runtime_lock);
	}
	raw_spin_unlock_irq(&tg->rt_bandwidth.rt_runtime_lock);
unlock:
	mutex_unlock(&rt_constraints_mutex);

	ଫେରନ୍ତୁ err;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା sched_group_set_rt_runtime(ସଂରଚନା task_group *tg, long rt_runtime_us)
{
	u64 rt_runtime, rt_period;

	rt_period = ktime_to_ns(tg->rt_bandwidth.rt_period);
	rt_runtime = (u64)rt_runtime_us * NSEC_PER_USEC;
	ଯଦି (rt_runtime_us < 0)
		rt_runtime = RUNTIME_INF;
	ଅନ୍ୟଥା ଯଦି ((u64)rt_runtime_us > U64_MAX / NSEC_PER_USEC)
		ଫେରନ୍ତୁ -EINVAL;

	ଫେରନ୍ତୁ tg_set_rt_bandwidth(tg, rt_period, rt_runtime);
}

long sched_group_rt_runtime(ସଂରଚନା task_group *tg)
{
	u64 rt_runtime_us;

	ଯଦି (tg->rt_bandwidth.rt_runtime == RUNTIME_INF)
		ଫେରନ୍ତୁ -1;

	rt_runtime_us = tg->rt_bandwidth.rt_runtime;
	do_div(rt_runtime_us, NSEC_PER_USEC);
	ଫେରନ୍ତୁ rt_runtime_us;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା sched_group_set_rt_period(ସଂରଚନା task_group *tg, u64 rt_period_us)
{
	u64 rt_runtime, rt_period;

	ଯଦି (rt_period_us > U64_MAX / NSEC_PER_USEC)
		ଫେରନ୍ତୁ -EINVAL;

	rt_period = rt_period_us * NSEC_PER_USEC;
	rt_runtime = tg->rt_bandwidth.rt_runtime;

	ଫେରନ୍ତୁ tg_set_rt_bandwidth(tg, rt_period, rt_runtime);
}

long sched_group_rt_period(ସଂରଚନା task_group *tg)
{
	u64 rt_period_us;

	rt_period_us = ktime_to_ns(tg->rt_bandwidth.rt_period);
	do_div(rt_period_us, NSEC_PER_USEC);
	ଫେରନ୍ତୁ rt_period_us;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା sched_rt_can_attach(ସଂରଚନା task_group *tg, ସଂରଚନା task_struct *tsk)
{
	/* Don't accept real-time tasks when there is no way for them to run */
	ଯଦି (rt_group_sched_enabled() && rt_task(tsk) && tg->rt_bandwidth.rt_runtime == 0)
		ଫେରନ୍ତୁ 0;

	ଫେରନ୍ତୁ 1;
}

#endif /* !CONFIG_RT_GROUP_SCHED */

#ifdef CONFIG_SYSCTL
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା sched_rt_global_validate(ଶୂନ୍ୟ)
{
	ଯଦି ((sysctl_sched_rt_runtime != RUNTIME_INF) &&
		((sysctl_sched_rt_runtime > sysctl_sched_rt_period) ||
		 ((u64)sysctl_sched_rt_runtime *
			NSEC_PER_USEC > max_rt_runtime)))
		ଫେରନ୍ତୁ -EINVAL;

#ifdef CONFIG_RT_GROUP_SCHED
	ଯଦି (!rt_group_sched_enabled())
		ଫେରନ୍ତୁ 0;

	scoped_guard(mutex, &rt_constraints_mutex)
		ଫେରନ୍ତୁ __rt_schedulable(ଖାଲି, 0, 0);
#endif
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା sched_rt_handler(ଅଚଳ ସଂରଚନା ctl_table *table, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଲିଖ, ଶୂନ୍ୟ *buffer,
		size_t *lenp, loff_t *ppos)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା old_period, old_runtime;
	ସ୍ଥିର DEFINE_MUTEX(mutex);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

	mutex_lock(&mutex);
	sched_domains_mutex_lock();
	old_period = sysctl_sched_rt_period;
	old_runtime = sysctl_sched_rt_runtime;

	ret = proc_dointvec_minmax(table, ଲିଖ, buffer, lenp, ppos);

	ଯଦି (!ret && ଲିଖ) {
		ret = sched_rt_global_validate();
		ଯଦି (ret)
			ଯାଅ undo;

		ret = sched_dl_global_validate();
		ଯଦି (ret)
			ଯାଅ undo;

		sched_dl_do_global();
	}
	ଯଦି (0) {
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

	ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା sched_rr_handler(ଅଚଳ ସଂରଚନା ctl_table *table, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଲିଖ, ଶୂନ୍ୟ *buffer,
		size_t *lenp, loff_t *ppos)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
	ସ୍ଥିର DEFINE_MUTEX(mutex);

	mutex_lock(&mutex);
	ret = proc_dointvec(table, ଲିଖ, buffer, lenp, ppos);
	/*
	 * Make sure that internally we keep jiffies.
	 * Also, writing zero resets the time-slice to default:
	 */
	ଯଦି (!ret && ଲିଖ) {
		sched_rr_timeslice =
			sysctl_sched_rr_timeslice <= 0 ? RR_TIMESLICE :
			msecs_to_jiffies(sysctl_sched_rr_timeslice);

		ଯଦି (sysctl_sched_rr_timeslice <= 0)
			sysctl_sched_rr_timeslice = jiffies_to_msecs(RR_TIMESLICE);
	}
	mutex_unlock(&mutex);

	ଫେରନ୍ତୁ ret;
}
#endif /* CONFIG_SYSCTL */

ଶୂନ୍ୟ print_rt_stats(ସଂରଚନା seq_file *m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	rt_rq_iter_t iter;
	ସଂରଚନା rt_rq *rt_rq;

	rcu_read_lock();
	for_each_rt_rq(rt_rq, iter, cpu_rq(cpu))
		print_rt_rq(m, cpu, rt_rq);
	rcu_read_unlock();
}
