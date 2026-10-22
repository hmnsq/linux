// SPDX-License-Identifier: GPL-2.0
/*
 * Real-Time Scheduling Class (mapped to the SCHED_FIFO and SCHED_RR
 * policies)
 */

#অন্তর্ভাৱ "sched.h"
#অন্তর্ভাৱ "pelt.h"

পূর্ণাঙ্ক sched_rr_timeslice = RR_TIMESLICE;
/* More than 4 hours if BW_SHIFT equals 20. */
স্থির ধ্রুৱ u64 max_rt_runtime = MAX_BW;

/*
 * period over which we measure -rt task CPU usage in us.
 * default: 1s
 */
পূর্ণাঙ্ক sysctl_sched_rt_period = 1000000;

/*
 * part of the period that we allow rt tasks to run in us.
 * default: 1s
 */
পূর্ণাঙ্ক sysctl_sched_rt_runtime = 1000000;

#ifdef CONFIG_SYSCTL
স্থির পূর্ণাঙ্ক sysctl_sched_rr_timeslice = (MSEC_PER_SEC * RR_TIMESLICE) / HZ;
স্থির পূর্ণাঙ্ক sched_rt_handler(ধ্রুৱ সংরচনা ctl_table *table, পূর্ণাঙ্ক লিখ, শূন্য *buffer,
		size_t *lenp, loff_t *ppos);
স্থির পূর্ণাঙ্ক sched_rr_handler(ধ্রুৱ সংরচনা ctl_table *table, পূর্ণাঙ্ক লিখ, শূন্য *buffer,
		size_t *lenp, loff_t *ppos);
স্থির ধ্রুৱ সংরচনা ctl_table sched_rt_sysctls[] = {
	{
		.procname       = "sched_rt_period_us",
		.data           = &sysctl_sched_rt_period,
		.maxlen         = পরিমাণ(পূর্ণাঙ্ক),
		.mode           = 0644,
		.proc_handler   = sched_rt_handler,
		.extra1         = SYSCTL_ONE,
		.extra2         = SYSCTL_INT_MAX,
	},
	{
		.procname       = "sched_rt_runtime_us",
		.data           = &sysctl_sched_rt_runtime,
		.maxlen         = পরিমাণ(পূর্ণাঙ্ক),
		.mode           = 0644,
		.proc_handler   = sched_rt_handler,
		.extra1         = SYSCTL_NEG_ONE,
		.extra2         = (শূন্য *)&sysctl_sched_rt_period,
	},
	{
		.procname       = "sched_rr_timeslice_ms",
		.data           = &sysctl_sched_rr_timeslice,
		.maxlen         = পরিমাণ(পূর্ণাঙ্ক),
		.mode           = 0644,
		.proc_handler   = sched_rr_handler,
	},
};

স্থির পূর্ণাঙ্ক __init sched_rt_sysctl_init(শূন্য)
{
	register_sysctl_init("kernel", sched_rt_sysctls);
	ঘূৰাই 0;
}
late_initcall(sched_rt_sysctl_init);
#endif /* CONFIG_SYSCTL */

শূন্য init_rt_rq(সংরচনা rt_rq *rt_rq)
{
	সংরচনা rt_prio_array *array;
	পূর্ণাঙ্ক i;

	array = &rt_rq->active;
	বাবে (i = 0; i < MAX_RT_PRIO; i++) {
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
	rt_rq->tg = &মূল_task_group;
#endif
}

#ifdef CONFIG_RT_GROUP_SCHED

স্থির পূর্ণাঙ্ক do_sched_rt_period_timer(সংরচনা rt_bandwidth *rt_b, পূর্ণাঙ্ক overrun);

স্থির গণনা hrtimer_restart sched_rt_period_timer(সংরচনা hrtimer *timer)
{
	সংরচনা rt_bandwidth *rt_b =
		container_of(timer, সংরচনা rt_bandwidth, rt_period_timer);
	পূর্ণাঙ্ক idle = 0;
	পূর্ণাঙ্ক overrun;

	raw_spin_lock(&rt_b->rt_runtime_lock);
	বাবে (;;) {
		overrun = hrtimer_forward_now(timer, rt_b->rt_period);
		যদি (!overrun)
			ভঙ্গ;

		raw_spin_unlock(&rt_b->rt_runtime_lock);
		idle = do_sched_rt_period_timer(rt_b, overrun);
		raw_spin_lock(&rt_b->rt_runtime_lock);
	}
	যদি (idle)
		rt_b->rt_period_active = 0;
	raw_spin_unlock(&rt_b->rt_runtime_lock);

	ঘূৰাই idle ? HRTIMER_NORESTART : HRTIMER_RESTART;
}

শূন্য init_rt_bandwidth(সংরচনা rt_bandwidth *rt_b, u64 period, u64 runtime)
{
	rt_b->rt_period = ns_to_ktime(period);
	rt_b->rt_runtime = runtime;

	raw_spin_lock_init(&rt_b->rt_runtime_lock);

	hrtimer_setup(&rt_b->rt_period_timer, sched_rt_period_timer, CLOCK_MONOTONIC,
		      HRTIMER_MODE_REL_HARD);
}

স্থির অন্তঃস্থ শূন্য do_start_rt_bandwidth(সংরচনা rt_bandwidth *rt_b)
{
	raw_spin_lock(&rt_b->rt_runtime_lock);
	যদি (!rt_b->rt_period_active) {
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

স্থির শূন্য start_rt_bandwidth(সংরচনা rt_bandwidth *rt_b)
{
	যদি (!rt_bandwidth_enabled() || rt_b->rt_runtime == RUNTIME_INF)
		ঘূৰাই;

	do_start_rt_bandwidth(rt_b);
}

স্থির শূন্য destroy_rt_bandwidth(সংরচনা rt_bandwidth *rt_b)
{
	hrtimer_cancel(&rt_b->rt_period_timer);
}

#define rt_entity_is_task(rt_se) (!(rt_se)->my_q)

স্থির অন্তঃস্থ সংরচনা task_struct *rt_task_of(সংরচনা sched_rt_entity *rt_se)
{
	WARN_ON_ONCE(!rt_entity_is_task(rt_se));

	ঘূৰাই container_of(rt_se, সংরচনা task_struct, rt);
}

স্থির অন্তঃস্থ সংরচনা rq *rq_of_rt_rq(সংরচনা rt_rq *rt_rq)
{
	/* Cannot fold with non-CONFIG_RT_GROUP_SCHED version, layout */
	WARN_ON(!rt_group_sched_enabled() && rt_rq->tg != &মূল_task_group);
	ঘূৰাই rt_rq->rq;
}

স্থির অন্তঃস্থ সংরচনা rt_rq *rt_rq_of_se(সংরচনা sched_rt_entity *rt_se)
{
	WARN_ON(!rt_group_sched_enabled() && rt_se->rt_rq->tg != &মূল_task_group);
	ঘূৰাই rt_se->rt_rq;
}

স্থির অন্তঃস্থ সংরচনা rq *rq_of_rt_se(সংরচনা sched_rt_entity *rt_se)
{
	সংরচনা rt_rq *rt_rq = rt_se->rt_rq;

	WARN_ON(!rt_group_sched_enabled() && rt_rq->tg != &মূল_task_group);
	ঘূৰাই rt_rq->rq;
}

শূন্য unregister_rt_sched_group(সংরচনা task_group *tg)
{
	যদি (!rt_group_sched_enabled())
		ঘূৰাই;

	যদি (tg->rt_se)
		destroy_rt_bandwidth(&tg->rt_bandwidth);
}

শূন্য free_rt_sched_group(সংরচনা task_group *tg)
{
	পূর্ণাঙ্ক i;

	যদি (!rt_group_sched_enabled())
		ঘূৰাই;

	for_each_possible_cpu(i) {
		যদি (tg->rt_rq)
			kfree(tg->rt_rq[i]);
		যদি (tg->rt_se)
			kfree(tg->rt_se[i]);
	}

	kfree(tg->rt_rq);
	kfree(tg->rt_se);
}

শূন্য init_tg_rt_entry(সংরচনা task_group *tg, সংরচনা rt_rq *rt_rq,
		সংরচনা sched_rt_entity *rt_se, পূর্ণাঙ্ক cpu,
		সংরচনা sched_rt_entity *parent)
{
	সংরচনা rq *rq = cpu_rq(cpu);

	rt_rq->highest_prio.curr = MAX_RT_PRIO-1;
	rt_rq->rt_nr_boosted = 0;
	rt_rq->rq = rq;
	rt_rq->tg = tg;

	tg->rt_rq[cpu] = rt_rq;
	tg->rt_se[cpu] = rt_se;

	যদি (!rt_se)
		ঘূৰাই;

	যদি (!parent)
		rt_se->rt_rq = &rq->rt;
	অন্যথা
		rt_se->rt_rq = parent->my_q;

	rt_se->my_q = rt_rq;
	rt_se->parent = parent;
	INIT_LIST_HEAD(&rt_se->run_list);
}

পূর্ণাঙ্ক alloc_rt_sched_group(সংরচনা task_group *tg, সংরচনা task_group *parent)
{
	সংরচনা rt_rq *rt_rq;
	সংরচনা sched_rt_entity *rt_se;
	পূর্ণাঙ্ক i;

	যদি (!rt_group_sched_enabled())
		ঘূৰাই 1;

	tg->rt_rq = kzalloc_objs(rt_rq, nr_cpu_ids);
	যদি (!tg->rt_rq)
		গচ্ছ err;
	tg->rt_se = kzalloc_objs(rt_se, nr_cpu_ids);
	যদি (!tg->rt_se)
		গচ্ছ err;

	init_rt_bandwidth(&tg->rt_bandwidth, ktime_to_ns(global_rt_period()), 0);

	for_each_possible_cpu(i) {
		rt_rq = kzalloc_গ্ৰন্থি(পরিমাণ(সংরচনা rt_rq),
				     GFP_KERNEL, cpu_to_গ্ৰন্থি(i));
		যদি (!rt_rq)
			গচ্ছ err;

		rt_se = kzalloc_গ্ৰন্থি(পরিমাণ(সংরচনা sched_rt_entity),
				     GFP_KERNEL, cpu_to_গ্ৰন্থি(i));
		যদি (!rt_se)
			গচ্ছ err_free_rq;

		init_rt_rq(rt_rq);
		rt_rq->rt_runtime = tg->rt_bandwidth.rt_runtime;
		init_tg_rt_entry(tg, rt_rq, rt_se, i, parent->rt_se[i]);
	}

	ঘূৰাই 1;

err_free_rq:
	kfree(rt_rq);
err:
	ঘূৰাই 0;
}

#অন্যথা /* !CONFIG_RT_GROUP_SCHED: */

#define rt_entity_is_task(rt_se) (1)

স্থির অন্তঃস্থ সংরচনা task_struct *rt_task_of(সংরচনা sched_rt_entity *rt_se)
{
	ঘূৰাই container_of(rt_se, সংরচনা task_struct, rt);
}

স্থির অন্তঃস্থ সংরচনা rq *rq_of_rt_rq(সংরচনা rt_rq *rt_rq)
{
	ঘূৰাই container_of(rt_rq, সংরচনা rq, rt);
}

স্থির অন্তঃস্থ সংরচনা rq *rq_of_rt_se(সংরচনা sched_rt_entity *rt_se)
{
	সংরচনা task_struct *p = rt_task_of(rt_se);

	ঘূৰাই task_rq(p);
}

স্থির অন্তঃস্থ সংরচনা rt_rq *rt_rq_of_se(সংরচনা sched_rt_entity *rt_se)
{
	সংরচনা rq *rq = rq_of_rt_se(rt_se);

	ঘূৰাই &rq->rt;
}

শূন্য unregister_rt_sched_group(সংরচনা task_group *tg) { }

শূন্য free_rt_sched_group(সংরচনা task_group *tg) { }

পূর্ণাঙ্ক alloc_rt_sched_group(সংরচনা task_group *tg, সংরচনা task_group *parent)
{
	ঘূৰাই 1;
}
#endif /* !CONFIG_RT_GROUP_SCHED */

স্থির অন্তঃস্থ bool need_pull_rt_task(সংরচনা rq *rq, সংরচনা task_struct *prev)
{
	/* Try to pull RT tasks here if we lower this rq's prio */
	ঘূৰাই rq->online && rq->rt.highest_prio.curr > prev->prio;
}

স্থির অন্তঃস্থ পূর্ণাঙ্ক rt_overloaded(সংরচনা rq *rq)
{
	ঘূৰাই atomic_read(&rq->rd->rto_গুনতি);
}

স্থির অন্তঃস্থ শূন্য rt_set_overload(সংরচনা rq *rq)
{
	যদি (!rq->online)
		ঘূৰাই;

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
	atomic_inc(&rq->rd->rto_গুনতি);
}

স্থির অন্তঃস্থ শূন্য rt_clear_overload(সংরচনা rq *rq)
{
	যদি (!rq->online)
		ঘূৰাই;

	/* the order here really doesn't matter */
	atomic_dec(&rq->rd->rto_গুনতি);
	cpumask_clear_cpu(rq->cpu, rq->rd->rto_mask);
}

স্থির অন্তঃস্থ পূর্ণাঙ্ক has_pushable_tasks(সংরচনা rq *rq)
{
	ঘূৰাই !plist_head_empty(&rq->rt.pushable_tasks);
}

স্থির DEFINE_PER_CPU(সংরচনা balance_callback, rt_ঠেলো_head);
স্থির DEFINE_PER_CPU(সংরচনা balance_callback, rt_pull_head);

স্থির শূন্য ঠেলো_rt_tasks(সংরচনা rq *);
স্থির শূন্য pull_rt_task(সংরচনা rq *);

স্থির অন্তঃস্থ শূন্য rt_queue_ঠেলো_tasks(সংরচনা rq *rq)
{
	যদি (!has_pushable_tasks(rq))
		ঘূৰাই;

	queue_balance_callback(rq, &per_cpu(rt_ঠেলো_head, rq->cpu), ঠেলো_rt_tasks);
}

স্থির অন্তঃস্থ শূন্য rt_queue_pull_task(সংরচনা rq *rq)
{
	queue_balance_callback(rq, &per_cpu(rt_pull_head, rq->cpu), pull_rt_task);
}

স্থির শূন্য enqueue_pushable_task(সংরচনা rq *rq, সংরচনা task_struct *p)
{
	plist_del(&p->pushable_tasks, &rq->rt.pushable_tasks);
	plist_গ্ৰন্থি_init(&p->pushable_tasks, p->prio);
	plist_add(&p->pushable_tasks, &rq->rt.pushable_tasks);

	/* Update the highest prio pushable task */
	যদি (p->prio < rq->rt.highest_prio.next)
		rq->rt.highest_prio.next = p->prio;

	যদি (!rq->rt.overloaded) {
		rt_set_overload(rq);
		rq->rt.overloaded = 1;
	}
}

স্থির শূন্য dequeue_pushable_task(সংরচনা rq *rq, সংরচনা task_struct *p)
{
	plist_del(&p->pushable_tasks, &rq->rt.pushable_tasks);

	/* Update the new highest prio pushable task */
	যদি (has_pushable_tasks(rq)) {
		p = plist_first_entry(&rq->rt.pushable_tasks,
				      সংরচনা task_struct, pushable_tasks);
		rq->rt.highest_prio.next = p->prio;
	} অন্যথা {
		rq->rt.highest_prio.next = MAX_RT_PRIO-1;

		যদি (rq->rt.overloaded) {
			rt_clear_overload(rq);
			rq->rt.overloaded = 0;
		}
	}
}

স্থির শূন্য enqueue_শীৰ্ষ_rt_rq(সংরচনা rt_rq *rt_rq);
স্থির শূন্য dequeue_শীৰ্ষ_rt_rq(সংরচনা rt_rq *rt_rq, unsigned পূর্ণাঙ্ক গুনতি);

স্থির অন্তঃস্থ পূর্ণাঙ্ক on_rt_rq(সংরচনা sched_rt_entity *rt_se)
{
	ঘূৰাই rt_se->on_rq;
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
স্থির অন্তঃস্থ bool rt_task_fits_capacity(সংরচনা task_struct *p, পূর্ণাঙ্ক cpu)
{
	unsigned পূর্ণাঙ্ক min_cap;
	unsigned পূর্ণাঙ্ক max_cap;
	unsigned পূর্ণাঙ্ক cpu_cap;

	/* Only heterogeneous systems can benefit from this check */
	যদি (!sched_asym_cpucap_active())
		ঘূৰাই true;

	min_cap = uclamp_eff_মান(p, UCLAMP_MIN);
	max_cap = uclamp_eff_মান(p, UCLAMP_MAX);

	cpu_cap = arch_scale_cpu_capacity(cpu);

	ঘূৰাই cpu_cap >= min(min_cap, max_cap);
}
#অন্যথা /* !CONFIG_UCLAMP_TASK: */
স্থির অন্তঃস্থ bool rt_task_fits_capacity(সংরচনা task_struct *p, পূর্ণাঙ্ক cpu)
{
	ঘূৰাই true;
}
#endif /* !CONFIG_UCLAMP_TASK */

#ifdef CONFIG_RT_GROUP_SCHED

স্থির অন্তঃস্থ u64 sched_rt_runtime(সংরচনা rt_rq *rt_rq)
{
	ঘূৰাই rt_rq->rt_runtime;
}

স্থির অন্তঃস্থ u64 sched_rt_period(সংরচনা rt_rq *rt_rq)
{
	ঘূৰাই ktime_to_ns(rt_rq->tg->rt_bandwidth.rt_period);
}

প্রকারনির্দেশ সংরচনা task_group *rt_rq_iter_t;

স্থির অন্তঃস্থ সংরচনা task_group *next_task_group(সংরচনা task_group *tg)
{
	যদি (!rt_group_sched_enabled()) {
		WARN_ON(tg != &মূল_task_group);
		ঘূৰাই রিক্ত;
	}

	কৰা {
		tg = list_entry_rcu(tg->list.next,
			typeof(সংরচনা task_group), list);
	} যেতিয়ালৈ (&tg->list != &task_groups && task_group_is_autogroup(tg));

	যদি (&tg->list == &task_groups)
		tg = রিক্ত;

	ঘূৰাই tg;
}

#define for_each_rt_rq(rt_rq, iter, rq)					\
	বাবে (iter = &মূল_task_group;					\
		iter && (rt_rq = iter->rt_rq[cpu_of(rq)]);		\
		iter = next_task_group(iter))

#define for_each_sched_rt_entity(rt_se) \
	বাবে (; rt_se; rt_se = rt_se->parent)

স্থির অন্তঃস্থ সংরচনা rt_rq *group_rt_rq(সংরচনা sched_rt_entity *rt_se)
{
	ঘূৰাই rt_se->my_q;
}

স্থির শূন্য enqueue_rt_entity(সংরচনা sched_rt_entity *rt_se, unsigned পূর্ণাঙ্ক flags);
স্থির শূন্য dequeue_rt_entity(সংরচনা sched_rt_entity *rt_se, unsigned পূর্ণাঙ্ক flags);

স্থির শূন্য sched_rt_rq_enqueue(সংরচনা rt_rq *rt_rq)
{
	সংরচনা task_struct *donor = rq_of_rt_rq(rt_rq)->donor;
	সংরচনা rq *rq = rq_of_rt_rq(rt_rq);
	সংরচনা sched_rt_entity *rt_se;

	পূর্ণাঙ্ক cpu = cpu_of(rq);

	rt_se = rt_rq->tg->rt_se[cpu];

	যদি (rt_rq->rt_nr_running) {
		যদি (!rt_se)
			enqueue_শীৰ্ষ_rt_rq(rt_rq);
		অন্যথা যদি (!on_rt_rq(rt_se))
			enqueue_rt_entity(rt_se, 0);

		যদি (rt_rq->highest_prio.curr < donor->prio)
			resched_curr(rq);
	}
}

স্থির শূন্য sched_rt_rq_dequeue(সংরচনা rt_rq *rt_rq)
{
	সংরচনা sched_rt_entity *rt_se;
	পূর্ণাঙ্ক cpu = cpu_of(rq_of_rt_rq(rt_rq));

	rt_se = rt_rq->tg->rt_se[cpu];

	যদি (!rt_se) {
		dequeue_শীৰ্ষ_rt_rq(rt_rq, rt_rq->rt_nr_running);
		/* Kick cpufreq (see the comment in kernel/sched/sched.h). */
		cpufreq_update_util(rq_of_rt_rq(rt_rq), 0);
	}
	অন্যথা যদি (on_rt_rq(rt_se))
		dequeue_rt_entity(rt_se, 0);
}

স্থির অন্তঃস্থ পূর্ণাঙ্ক rt_rq_throttled(সংরচনা rt_rq *rt_rq)
{
	ঘূৰাই rt_rq->rt_throttled && !rt_rq->rt_nr_boosted;
}

স্থির পূর্ণাঙ্ক rt_se_boosted(সংরচনা sched_rt_entity *rt_se)
{
	সংরচনা rt_rq *rt_rq = group_rt_rq(rt_se);
	সংরচনা task_struct *p;

	যদি (rt_rq)
		ঘূৰাই !!rt_rq->rt_nr_boosted;

	p = rt_task_of(rt_se);
	ঘূৰাই p->prio != p->normal_prio;
}

স্থির অন্তঃস্থ ধ্রুৱ সংরচনা cpumask *sched_rt_period_mask(শূন্য)
{
	ঘূৰাই this_rq()->rd->span;
}

স্থির অন্তঃস্থ
সংরচনা rt_rq *sched_rt_period_rt_rq(সংরচনা rt_bandwidth *rt_b, পূর্ণাঙ্ক cpu)
{
	ঘূৰাই container_of(rt_b, সংরচনা task_group, rt_bandwidth)->rt_rq[cpu];
}

স্থির অন্তঃস্থ সংরচনা rt_bandwidth *sched_rt_bandwidth(সংরচনা rt_rq *rt_rq)
{
	ঘূৰাই &rt_rq->tg->rt_bandwidth;
}

bool sched_rt_bandwidth_account(সংরচনা rt_rq *rt_rq)
{
	সংরচনা rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);

	ঘূৰাই (hrtimer_active(&rt_b->rt_period_timer) ||
		rt_rq->rt_time < rt_b->rt_runtime);
}

/*
 * We ran out of runtime, see if we can borrow some from our neighbours.
 */
স্থির শূন্য do_balance_runtime(সংরচনা rt_rq *rt_rq)
{
	সংরচনা rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);
	সংরচনা মূল_domain *rd = rq_of_rt_rq(rt_rq)->rd;
	পূর্ণাঙ্ক i, weight;
	u64 rt_period;

	weight = cpumask_weight(rd->span);

	raw_spin_lock(&rt_b->rt_runtime_lock);
	rt_period = ktime_to_ns(rt_b->rt_period);
	for_each_cpu(i, rd->span) {
		সংরচনা rt_rq *iter = sched_rt_period_rt_rq(rt_b, i);
		s64 diff;

		যদি (iter == rt_rq)
			অনুৱর্ত;

		raw_spin_lock(&iter->rt_runtime_lock);
		/*
		 * Either all rqs have inf runtime and there's nothing to steal
		 * or __disable_runtime() below sets a specific rq to inf to
		 * indicate its been disabled and disallow stealing.
		 */
		যদি (iter->rt_runtime == RUNTIME_INF)
			গচ্ছ next;

		/*
		 * From runqueues with spare time, take 1/n part of their
		 * spare time, but no more than our period.
		 */
		diff = iter->rt_runtime - iter->rt_time;
		যদি (diff > 0) {
			diff = div_u64((u64)diff, weight);
			যদি (rt_rq->rt_runtime + diff > rt_period)
				diff = rt_period - rt_rq->rt_runtime;
			iter->rt_runtime -= diff;
			rt_rq->rt_runtime += diff;
			যদি (rt_rq->rt_runtime == rt_period) {
				raw_spin_unlock(&iter->rt_runtime_lock);
				ভঙ্গ;
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
স্থির শূন্য __disable_runtime(সংরচনা rq *rq)
{
	সংরচনা মূল_domain *rd = rq->rd;
	rt_rq_iter_t iter;
	সংরচনা rt_rq *rt_rq;

	যদি (unlikely(!scheduler_running))
		ঘূৰাই;

	for_each_rt_rq(rt_rq, iter, rq) {
		সংরচনা rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);
		s64 want;
		পূর্ণাঙ্ক i;

		raw_spin_lock(&rt_b->rt_runtime_lock);
		raw_spin_lock(&rt_rq->rt_runtime_lock);
		/*
		 * Either we're all inf and nobody needs to borrow, or we're
		 * already disabled and thus have nothing to do, or we have
		 * exactly the right amount of runtime to take out.
		 */
		যদি (rt_rq->rt_runtime == RUNTIME_INF ||
				rt_rq->rt_runtime == rt_b->rt_runtime)
			গচ্ছ balanced;
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
			সংরচনা rt_rq *iter = sched_rt_period_rt_rq(rt_b, i);
			s64 diff;

			/*
			 * Can't reclaim from ourselves or disabled runqueues.
			 */
			যদি (iter == rt_rq || iter->rt_runtime == RUNTIME_INF)
				অনুৱর্ত;

			raw_spin_lock(&iter->rt_runtime_lock);
			যদি (want > 0) {
				diff = min_t(s64, iter->rt_runtime, want);
				iter->rt_runtime -= diff;
				want -= diff;
			} অন্যথা {
				iter->rt_runtime -= want;
				want -= want;
			}
			raw_spin_unlock(&iter->rt_runtime_lock);

			যদি (!want)
				ভঙ্গ;
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

স্থির শূন্য __enable_runtime(সংরচনা rq *rq)
{
	rt_rq_iter_t iter;
	সংরচনা rt_rq *rt_rq;

	যদি (unlikely(!scheduler_running))
		ঘূৰাই;

	/*
	 * Reset each runqueue's bandwidth settings
	 */
	for_each_rt_rq(rt_rq, iter, rq) {
		সংরচনা rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);

		raw_spin_lock(&rt_b->rt_runtime_lock);
		raw_spin_lock(&rt_rq->rt_runtime_lock);
		rt_rq->rt_runtime = rt_b->rt_runtime;
		rt_rq->rt_time = 0;
		rt_rq->rt_throttled = 0;
		raw_spin_unlock(&rt_rq->rt_runtime_lock);
		raw_spin_unlock(&rt_b->rt_runtime_lock);
	}
}

স্থির শূন্য balance_runtime(সংরচনা rt_rq *rt_rq)
{
	যদি (!sched_feat(RT_RUNTIME_SHARE))
		ঘূৰাই;

	যদি (rt_rq->rt_time > rt_rq->rt_runtime) {
		raw_spin_unlock(&rt_rq->rt_runtime_lock);
		do_balance_runtime(rt_rq);
		raw_spin_lock(&rt_rq->rt_runtime_lock);
	}
}

স্থির পূর্ণাঙ্ক do_sched_rt_period_timer(সংরচনা rt_bandwidth *rt_b, পূর্ণাঙ্ক overrun)
{
	পূর্ণাঙ্ক i, idle = 1, throttled = 0;
	ধ্রুৱ সংরচনা cpumask *span;

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
	যদি (rt_b == &মূল_task_group.rt_bandwidth)
		span = cpu_online_mask;

	for_each_cpu(i, span) {
		পূর্ণাঙ্ক enqueue = 0;
		সংরচনা rt_rq *rt_rq = sched_rt_period_rt_rq(rt_b, i);
		সংরচনা rq *rq = rq_of_rt_rq(rt_rq);
		সংরচনা rq_flags rf;
		পূর্ণাঙ্ক skip;

		/*
		 * When span == cpu_online_mask, taking each rq->lock
		 * can be time-consuming. Try to avoid it when possible.
		 */
		raw_spin_lock(&rt_rq->rt_runtime_lock);
		যদি (!sched_feat(RT_RUNTIME_SHARE) && rt_rq->rt_runtime != RUNTIME_INF)
			rt_rq->rt_runtime = rt_b->rt_runtime;
		skip = !rt_rq->rt_time && !rt_rq->rt_nr_running;
		raw_spin_unlock(&rt_rq->rt_runtime_lock);
		যদি (skip)
			অনুৱর্ত;

		rq_lock(rq, &rf);
		update_rq_clock(rq);

		যদি (rt_rq->rt_time) {
			u64 runtime;

			raw_spin_lock(&rt_rq->rt_runtime_lock);
			যদি (rt_rq->rt_throttled)
				balance_runtime(rt_rq);
			runtime = rt_rq->rt_runtime;
			rt_rq->rt_time -= min(rt_rq->rt_time, overrun*runtime);
			যদি (rt_rq->rt_throttled && rt_rq->rt_time < runtime) {
				rt_rq->rt_throttled = 0;
				enqueue = 1;

				/*
				 * When we're idle and a woken (rt) task is
				 * throttled wakeup_preempt() will set
				 * skip_update and the time between the wakeup
				 * and this unthrottle will get accounted as
				 * 'runtime'.
				 */
				যদি (rt_rq->rt_nr_running && rq->curr == rq->idle)
					rq_clock_cancel_skipupdate(rq);
			}
			যদি (rt_rq->rt_time || rt_rq->rt_nr_running)
				idle = 0;
			raw_spin_unlock(&rt_rq->rt_runtime_lock);
		} অন্যথা যদি (rt_rq->rt_nr_running) {
			idle = 0;
			যদি (!rt_rq_throttled(rt_rq))
				enqueue = 1;
		}
		যদি (rt_rq->rt_throttled)
			throttled = 1;

		যদি (enqueue)
			sched_rt_rq_enqueue(rt_rq);
		rq_unlock(rq, &rf);
	}

	যদি (!throttled && (!rt_bandwidth_enabled() || rt_b->rt_runtime == RUNTIME_INF))
		ঘূৰাই 1;

	ঘূৰাই idle;
}

স্থির পূর্ণাঙ্ক sched_rt_runtime_exceeded(সংরচনা rt_rq *rt_rq)
{
	u64 runtime = sched_rt_runtime(rt_rq);

	যদি (rt_rq->rt_throttled)
		ঘূৰাই rt_rq_throttled(rt_rq);

	যদি (runtime >= sched_rt_period(rt_rq))
		ঘূৰাই 0;

	balance_runtime(rt_rq);
	runtime = sched_rt_runtime(rt_rq);
	যদি (runtime == RUNTIME_INF)
		ঘূৰাই 0;

	যদি (rt_rq->rt_time > runtime) {
		সংরচনা rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);

		/*
		 * Don't actually throttle groups that have no runtime assigned
		 * but accrue some time due to boosting.
		 */
		যদি (likely(rt_b->rt_runtime)) {
			rt_rq->rt_throttled = 1;
			printk_deferred_once("sched: RT throttling activated\n");
		} অন্যথা {
			/*
			 * In case we did anyway, make it go away,
			 * replenishment is a joke, since it will replenish us
			 * with exactly 0 ns.
			 */
			rt_rq->rt_time = 0;
		}

		যদি (rt_rq_throttled(rt_rq)) {
			sched_rt_rq_dequeue(rt_rq);
			ঘূৰাই 1;
		}
	}

	ঘূৰাই 0;
}

#অন্যথা /* !CONFIG_RT_GROUP_SCHED: */

প্রকারনির্দেশ সংরচনা rt_rq *rt_rq_iter_t;

#define for_each_rt_rq(rt_rq, iter, rq) \
	বাবে ((শূন্য) iter, rt_rq = &rq->rt; rt_rq; rt_rq = রিক্ত)

#define for_each_sched_rt_entity(rt_se) \
	বাবে (; rt_se; rt_se = রিক্ত)

স্থির অন্তঃস্থ সংরচনা rt_rq *group_rt_rq(সংরচনা sched_rt_entity *rt_se)
{
	ঘূৰাই রিক্ত;
}

স্থির অন্তঃস্থ শূন্য sched_rt_rq_enqueue(সংরচনা rt_rq *rt_rq)
{
	সংরচনা rq *rq = rq_of_rt_rq(rt_rq);

	যদি (!rt_rq->rt_nr_running)
		ঘূৰাই;

	enqueue_শীৰ্ষ_rt_rq(rt_rq);
	resched_curr(rq);
}

স্থির অন্তঃস্থ শূন্য sched_rt_rq_dequeue(সংরচনা rt_rq *rt_rq)
{
	dequeue_শীৰ্ষ_rt_rq(rt_rq, rt_rq->rt_nr_running);
}

স্থির অন্তঃস্থ পূর্ণাঙ্ক rt_rq_throttled(সংরচনা rt_rq *rt_rq)
{
	ঘূৰাই false;
}

স্থির অন্তঃস্থ ধ্রুৱ সংরচনা cpumask *sched_rt_period_mask(শূন্য)
{
	ঘূৰাই cpu_online_mask;
}

স্থির অন্তঃস্থ
সংরচনা rt_rq *sched_rt_period_rt_rq(সংরচনা rt_bandwidth *rt_b, পূর্ণাঙ্ক cpu)
{
	ঘূৰাই &cpu_rq(cpu)->rt;
}

স্থির শূন্য __enable_runtime(সংরচনা rq *rq) { }
স্থির শূন্য __disable_runtime(সংরচনা rq *rq) { }

#endif /* !CONFIG_RT_GROUP_SCHED */

স্থির অন্তঃস্থ পূর্ণাঙ্ক rt_se_prio(সংরচনা sched_rt_entity *rt_se)
{
#ifdef CONFIG_RT_GROUP_SCHED
	সংরচনা rt_rq *rt_rq = group_rt_rq(rt_se);

	যদি (rt_rq)
		ঘূৰাই rt_rq->highest_prio.curr;
#endif

	ঘূৰাই rt_task_of(rt_se)->prio;
}

/*
 * Update the current task's runtime statistics. Skip current tasks that
 * are not in our scheduling class.
 */
স্থির শূন্য update_curr_rt(সংরচনা rq *rq)
{
	সংরচনা task_struct *donor = rq->donor;
	s64 delta_exec;

	যদি (donor->sched_class != &rt_sched_class)
		ঘূৰাই;

	delta_exec = update_curr_common(rq);
	যদি (unlikely(delta_exec <= 0))
		ঘূৰাই;

#ifdef CONFIG_RT_GROUP_SCHED
	সংরচনা sched_rt_entity *rt_se = &donor->rt;

	যদি (!rt_bandwidth_enabled())
		ঘূৰাই;

	for_each_sched_rt_entity(rt_se) {
		সংরচনা rt_rq *rt_rq = rt_rq_of_se(rt_se);
		পূর্ণাঙ্ক exceeded;

		যদি (sched_rt_runtime(rt_rq) != RUNTIME_INF) {
			raw_spin_lock(&rt_rq->rt_runtime_lock);
			rt_rq->rt_time += delta_exec;
			exceeded = sched_rt_runtime_exceeded(rt_rq);
			যদি (exceeded)
				resched_curr(rq);
			raw_spin_unlock(&rt_rq->rt_runtime_lock);
			যদি (exceeded)
				do_start_rt_bandwidth(sched_rt_bandwidth(rt_rq));
		}
	}
#endif /* CONFIG_RT_GROUP_SCHED */
}

স্থির শূন্য
dequeue_শীৰ্ষ_rt_rq(সংরচনা rt_rq *rt_rq, unsigned পূর্ণাঙ্ক গুনতি)
{
	সংরচনা rq *rq = rq_of_rt_rq(rt_rq);

	BUG_ON(&rq->rt != rt_rq);

	যদি (!rt_rq->rt_queued)
		ঘূৰাই;

	BUG_ON(!rq->nr_running);

	sub_nr_running(rq, গুনতি);
	rt_rq->rt_queued = 0;

}

স্থির শূন্য
enqueue_শীৰ্ষ_rt_rq(সংরচনা rt_rq *rt_rq)
{
	সংরচনা rq *rq = rq_of_rt_rq(rt_rq);

	BUG_ON(&rq->rt != rt_rq);

	যদি (rt_rq->rt_queued)
		ঘূৰাই;

	যদি (rt_rq_throttled(rt_rq))
		ঘূৰাই;

	যদি (rt_rq->rt_nr_running) {
		add_nr_running(rq, rt_rq->rt_nr_running);
		rt_rq->rt_queued = 1;
	}

	/* Kick cpufreq (see the comment in kernel/sched/sched.h). */
	cpufreq_update_util(rq, 0);
}

স্থির শূন্য
inc_rt_prio_smp(সংরচনা rt_rq *rt_rq, পূর্ণাঙ্ক prio, পূর্ণাঙ্ক prev_prio)
{
	সংরচনা rq *rq = rq_of_rt_rq(rt_rq);

	/*
	 * Change rq's cpupri only if rt_rq is the top queue.
	 */
	যদি (IS_ENABLED(CONFIG_RT_GROUP_SCHED) && &rq->rt != rt_rq)
		ঘূৰাই;

	যদি (rq->online && prio < prev_prio)
		cpupri_set(&rq->rd->cpupri, rq->cpu, prio);
}

স্থির শূন্য
dec_rt_prio_smp(সংরচনা rt_rq *rt_rq, পূর্ণাঙ্ক prio, পূর্ণাঙ্ক prev_prio)
{
	সংরচনা rq *rq = rq_of_rt_rq(rt_rq);

	/*
	 * Change rq's cpupri only if rt_rq is the top queue.
	 */
	যদি (IS_ENABLED(CONFIG_RT_GROUP_SCHED) && &rq->rt != rt_rq)
		ঘূৰাই;

	যদি (rq->online && rt_rq->highest_prio.curr != prev_prio)
		cpupri_set(&rq->rd->cpupri, rq->cpu, rt_rq->highest_prio.curr);
}

স্থির শূন্য
inc_rt_prio(সংরচনা rt_rq *rt_rq, পূর্ণাঙ্ক prio)
{
	পূর্ণাঙ্ক prev_prio = rt_rq->highest_prio.curr;

	যদি (prio < prev_prio)
		rt_rq->highest_prio.curr = prio;

	inc_rt_prio_smp(rt_rq, prio, prev_prio);
}

স্থির শূন্য
dec_rt_prio(সংরচনা rt_rq *rt_rq, পূর্ণাঙ্ক prio)
{
	পূর্ণাঙ্ক prev_prio = rt_rq->highest_prio.curr;

	যদি (rt_rq->rt_nr_running) {

		WARN_ON(prio < prev_prio);

		/*
		 * This may have been our highest task, and therefore
		 * we may have some re-computation to do
		 */
		যদি (prio == prev_prio) {
			সংরচনা rt_prio_array *array = &rt_rq->active;

			rt_rq->highest_prio.curr =
				sched_find_first_bit(array->bitmap);
		}

	} অন্যথা {
		rt_rq->highest_prio.curr = MAX_RT_PRIO-1;
	}

	dec_rt_prio_smp(rt_rq, prio, prev_prio);
}

#ifdef CONFIG_RT_GROUP_SCHED

স্থির শূন্য
inc_rt_group(সংরচনা sched_rt_entity *rt_se, সংরচনা rt_rq *rt_rq)
{
	যদি (rt_se_boosted(rt_se))
		rt_rq->rt_nr_boosted++;

	start_rt_bandwidth(&rt_rq->tg->rt_bandwidth);
}

স্থির শূন্য
dec_rt_group(সংরচনা sched_rt_entity *rt_se, সংরচনা rt_rq *rt_rq)
{
	যদি (rt_se_boosted(rt_se))
		rt_rq->rt_nr_boosted--;

	WARN_ON(!rt_rq->rt_nr_running && rt_rq->rt_nr_boosted);
}

#অন্যথা /* !CONFIG_RT_GROUP_SCHED: */

স্থির শূন্য
inc_rt_group(সংরচনা sched_rt_entity *rt_se, সংরচনা rt_rq *rt_rq)
{
}

স্থির অন্তঃস্থ
শূন্য dec_rt_group(সংরচনা sched_rt_entity *rt_se, সংরচনা rt_rq *rt_rq) {}

#endif /* !CONFIG_RT_GROUP_SCHED */

স্থির অন্তঃস্থ
unsigned পূর্ণাঙ্ক rt_se_nr_running(সংরচনা sched_rt_entity *rt_se)
{
	সংরচনা rt_rq *group_rq = group_rt_rq(rt_se);

	যদি (group_rq)
		ঘূৰাই group_rq->rt_nr_running;
	অন্যথা
		ঘূৰাই 1;
}

স্থির অন্তঃস্থ
unsigned পূর্ণাঙ্ক rt_se_rr_nr_running(সংরচনা sched_rt_entity *rt_se)
{
	সংরচনা rt_rq *group_rq = group_rt_rq(rt_se);
	সংরচনা task_struct *tsk;

	যদি (group_rq)
		ঘূৰাই group_rq->rr_nr_running;

	tsk = rt_task_of(rt_se);

	ঘূৰাই (tsk->policy == SCHED_RR) ? 1 : 0;
}

স্থির অন্তঃস্থ
শূন্য inc_rt_tasks(সংরচনা sched_rt_entity *rt_se, সংরচনা rt_rq *rt_rq)
{
	পূর্ণাঙ্ক prio = rt_se_prio(rt_se);

	WARN_ON(!rt_prio(prio));
	rt_rq->rt_nr_running += rt_se_nr_running(rt_se);
	rt_rq->rr_nr_running += rt_se_rr_nr_running(rt_se);

	inc_rt_prio(rt_rq, prio);
	inc_rt_group(rt_se, rt_rq);
}

স্থির অন্তঃস্থ
শূন্য dec_rt_tasks(সংরচনা sched_rt_entity *rt_se, সংরচনা rt_rq *rt_rq)
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
স্থির অন্তঃস্থ bool move_entity(unsigned পূর্ণাঙ্ক flags)
{
	যদি ((flags & (DEQUEUE_SAVE | DEQUEUE_MOVE)) == DEQUEUE_SAVE)
		ঘূৰাই false;

	ঘূৰাই true;
}

স্থির শূন্য __delist_rt_entity(সংরচনা sched_rt_entity *rt_se, সংরচনা rt_prio_array *array)
{
	list_del_init(&rt_se->run_list);

	যদি (list_empty(array->queue + rt_se_prio(rt_se)))
		__clear_bit(rt_se_prio(rt_se), array->bitmap);

	rt_se->on_list = 0;
}

স্থির অন্তঃস্থ সংরচনা sched_statistics *
__schedstats_from_rt_se(সংরচনা sched_rt_entity *rt_se)
{
	/* schedstats is not supported for rt group. */
	যদি (!rt_entity_is_task(rt_se))
		ঘূৰাই রিক্ত;

	ঘূৰাই &rt_task_of(rt_se)->stats;
}

স্থির অন্তঃস্থ শূন্য
update_stats_wait_start_rt(সংরচনা rt_rq *rt_rq, সংরচনা sched_rt_entity *rt_se)
{
	সংরচনা sched_statistics *stats;
	সংরচনা task_struct *p = রিক্ত;

	যদি (!schedstat_enabled())
		ঘূৰাই;

	যদি (rt_entity_is_task(rt_se))
		p = rt_task_of(rt_se);

	stats = __schedstats_from_rt_se(rt_se);
	যদি (!stats)
		ঘূৰাই;

	__update_stats_wait_start(rq_of_rt_rq(rt_rq), p, stats);
}

স্থির অন্তঃস্থ শূন্য
update_stats_enqueue_sleeper_rt(সংরচনা rt_rq *rt_rq, সংরচনা sched_rt_entity *rt_se)
{
	সংরচনা sched_statistics *stats;
	সংরচনা task_struct *p = রিক্ত;

	যদি (!schedstat_enabled())
		ঘূৰাই;

	যদি (rt_entity_is_task(rt_se))
		p = rt_task_of(rt_se);

	stats = __schedstats_from_rt_se(rt_se);
	যদি (!stats)
		ঘূৰাই;

	__update_stats_enqueue_sleeper(rq_of_rt_rq(rt_rq), p, stats);
}

স্থির অন্তঃস্থ শূন্য
update_stats_enqueue_rt(সংরচনা rt_rq *rt_rq, সংরচনা sched_rt_entity *rt_se,
			পূর্ণাঙ্ক flags)
{
	যদি (!schedstat_enabled())
		ঘূৰাই;

	যদি (flags & ENQUEUE_WAKEUP)
		update_stats_enqueue_sleeper_rt(rt_rq, rt_se);
}

স্থির অন্তঃস্থ শূন্য
update_stats_wait_end_rt(সংরচনা rt_rq *rt_rq, সংরচনা sched_rt_entity *rt_se)
{
	সংরচনা sched_statistics *stats;
	সংরচনা task_struct *p = রিক্ত;

	যদি (!schedstat_enabled())
		ঘূৰাই;

	যদি (rt_entity_is_task(rt_se))
		p = rt_task_of(rt_se);

	stats = __schedstats_from_rt_se(rt_se);
	যদি (!stats)
		ঘূৰাই;

	__update_stats_wait_end(rq_of_rt_rq(rt_rq), p, stats);
}

স্থির অন্তঃস্থ শূন্য
update_stats_dequeue_rt(সংরচনা rt_rq *rt_rq, সংরচনা sched_rt_entity *rt_se,
			পূর্ণাঙ্ক flags)
{
	সংরচনা task_struct *p = রিক্ত;
	সংরচনা rq *rq = rq_of_rt_rq(rt_rq);

	যদি (!schedstat_enabled())
		ঘূৰাই;

	যদি (rt_entity_is_task(rt_se)) {
		p = rt_task_of(rt_se);

		যদি (p != rq->curr)
			update_stats_wait_end_rt(rt_rq, rt_se);
	}

	যদি ((flags & DEQUEUE_SLEEP) && p) {
		unsigned পূর্ণাঙ্ক state;

		state = READ_ONCE(p->__state);
		যদি (state & TASK_INTERRUPTIBLE)
			__schedstat_set(p->stats.sleep_start,
					rq_clock(rq_of_rt_rq(rt_rq)));

		যদি (state & TASK_UNINTERRUPTIBLE)
			__schedstat_set(p->stats.block_start,
					rq_clock(rq_of_rt_rq(rt_rq)));
	}
}

স্থির শূন্য __enqueue_rt_entity(সংরচনা sched_rt_entity *rt_se, unsigned পূর্ণাঙ্ক flags)
{
	সংরচনা rt_rq *rt_rq = rt_rq_of_se(rt_se);
	সংরচনা rt_prio_array *array = &rt_rq->active;
	সংরচনা rt_rq *group_rq = group_rt_rq(rt_se);
	সংরচনা list_head *queue = array->queue + rt_se_prio(rt_se);

	/*
	 * Don't enqueue the group if its throttled, or when empty.
	 * The latter is a consequence of the former when a child group
	 * get throttled and the current group doesn't have any other
	 * active members.
	 */
	যদি (group_rq && (rt_rq_throttled(group_rq) || !group_rq->rt_nr_running)) {
		যদি (rt_se->on_list)
			__delist_rt_entity(rt_se, array);
		ঘূৰাই;
	}

	যদি (move_entity(flags)) {
		WARN_ON_ONCE(rt_se->on_list);
		যদি (flags & ENQUEUE_HEAD)
			list_add(&rt_se->run_list, queue);
		অন্যথা
			list_add_tail(&rt_se->run_list, queue);

		__set_bit(rt_se_prio(rt_se), array->bitmap);
		rt_se->on_list = 1;
	}
	rt_se->on_rq = 1;

	inc_rt_tasks(rt_se, rt_rq);
}

স্থির শূন্য __dequeue_rt_entity(সংরচনা sched_rt_entity *rt_se, unsigned পূর্ণাঙ্ক flags)
{
	সংরচনা rt_rq *rt_rq = rt_rq_of_se(rt_se);
	সংরচনা rt_prio_array *array = &rt_rq->active;

	যদি (move_entity(flags)) {
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
স্থির শূন্য dequeue_rt_স্তূপ(সংরচনা sched_rt_entity *rt_se, unsigned পূর্ণাঙ্ক flags)
{
	সংরচনা sched_rt_entity *back = রিক্ত;
	unsigned পূর্ণাঙ্ক rt_nr_running;

	for_each_sched_rt_entity(rt_se) {
		rt_se->back = back;
		back = rt_se;
	}

	rt_nr_running = rt_rq_of_se(back)->rt_nr_running;

	বাবে (rt_se = back; rt_se; rt_se = rt_se->back) {
		যদি (on_rt_rq(rt_se))
			__dequeue_rt_entity(rt_se, flags);
	}

	dequeue_শীৰ্ষ_rt_rq(rt_rq_of_se(back), rt_nr_running);
}

স্থির শূন্য enqueue_rt_entity(সংরচনা sched_rt_entity *rt_se, unsigned পূর্ণাঙ্ক flags)
{
	সংরচনা rq *rq = rq_of_rt_se(rt_se);

	update_stats_enqueue_rt(rt_rq_of_se(rt_se), rt_se, flags);

	dequeue_rt_স্তূপ(rt_se, flags);
	for_each_sched_rt_entity(rt_se)
		__enqueue_rt_entity(rt_se, flags);
	enqueue_শীৰ্ষ_rt_rq(&rq->rt);
}

স্থির শূন্য dequeue_rt_entity(সংরচনা sched_rt_entity *rt_se, unsigned পূর্ণাঙ্ক flags)
{
	সংরচনা rq *rq = rq_of_rt_se(rt_se);

	update_stats_dequeue_rt(rt_rq_of_se(rt_se), rt_se, flags);

	dequeue_rt_স্তূপ(rt_se, flags);

	for_each_sched_rt_entity(rt_se) {
		সংরচনা rt_rq *rt_rq = group_rt_rq(rt_se);

		যদি (rt_rq && rt_rq->rt_nr_running)
			__enqueue_rt_entity(rt_se, flags);
	}
	enqueue_শীৰ্ষ_rt_rq(&rq->rt);
}

/*
 * Adding/removing a task to/from a priority array:
 */
স্থির শূন্য
enqueue_task_rt(সংরচনা rq *rq, সংরচনা task_struct *p, পূর্ণাঙ্ক flags)
{
	সংরচনা sched_rt_entity *rt_se = &p->rt;

	যদি (flags & ENQUEUE_WAKEUP)
		rt_se->timeout = 0;

	check_schedstat_required();
	update_stats_wait_start_rt(rt_rq_of_se(rt_se), rt_se);

	enqueue_rt_entity(rt_se, flags);

	যদি (task_is_blocked(p))
		ঘূৰাই;

	যদি (!task_current(rq, p) && p->nr_cpus_allowed > 1)
		enqueue_pushable_task(rq, p);
}

স্থির bool dequeue_task_rt(সংরচনা rq *rq, সংরচনা task_struct *p, পূর্ণাঙ্ক flags)
{
	সংরচনা sched_rt_entity *rt_se = &p->rt;

	update_curr_rt(rq);
	dequeue_rt_entity(rt_se, flags);

	dequeue_pushable_task(rq, p);

	ঘূৰাই true;
}

/*
 * Put task to the head or the end of the run list without the overhead of
 * dequeue followed by enqueue.
 */
স্থির শূন্য
requeue_rt_entity(সংরচনা rt_rq *rt_rq, সংরচনা sched_rt_entity *rt_se, পূর্ণাঙ্ক head)
{
	যদি (on_rt_rq(rt_se)) {
		সংরচনা rt_prio_array *array = &rt_rq->active;
		সংরচনা list_head *queue = array->queue + rt_se_prio(rt_se);

		যদি (head)
			list_move(&rt_se->run_list, queue);
		অন্যথা
			list_move_tail(&rt_se->run_list, queue);
	}
}

স্থির শূন্য requeue_task_rt(সংরচনা rq *rq, সংরচনা task_struct *p, পূর্ণাঙ্ক head)
{
	সংরচনা sched_rt_entity *rt_se = &p->rt;
	সংরচনা rt_rq *rt_rq;

	for_each_sched_rt_entity(rt_se) {
		rt_rq = rt_rq_of_se(rt_se);
		requeue_rt_entity(rt_rq, rt_se, head);
	}
}

স্থির শূন্য yield_task_rt(সংরচনা rq *rq)
{
	requeue_task_rt(rq, rq->donor, 0);
}

স্থির পূর্ণাঙ্ক find_lowest_rq(সংরচনা task_struct *task);

স্থির পূর্ণাঙ্ক
select_task_rq_rt(সংরচনা task_struct *p, পূর্ণাঙ্ক cpu, পূর্ণাঙ্ক flags)
{
	সংরচনা task_struct *curr, *donor;
	সংরচনা rq *rq;
	bool test;

	/* For anything but wake ups, just return the task_cpu */
	যদি (!(flags & (WF_TTWU | WF_FORK)))
		গচ্ছ out;

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

	যদি (test || !rt_task_fits_capacity(p, cpu)) {
		পূর্ণাঙ্ক target = find_lowest_rq(p);

		/*
		 * Bail out if we were forcing a migration to find a better
		 * fitting CPU but our search failed.
		 */
		যদি (!test && target != -1 && !rt_task_fits_capacity(p, target))
			গচ্ছ out_unlock;

		/*
		 * Don't bother moving it if the destination CPU is
		 * not running a lower priority task.
		 */
		যদি (target != -1 &&
		    p->prio < cpu_rq(target)->rt.highest_prio.curr)
			cpu = target;
	}

out_unlock:
	rcu_read_unlock();

out:
	ঘূৰাই cpu;
}

স্থির শূন্য check_preempt_equal_prio(সংরচনা rq *rq, সংরচনা task_struct *p)
{
	যদি (rq->curr->nr_cpus_allowed == 1 ||
	    !cpupri_find(&rq->rd->cpupri, rq->donor, রিক্ত))
		ঘূৰাই;

	/*
	 * p is migratable, so let's not schedule it and
	 * see if it is pushed or pulled somewhere else.
	 */
	যদি (p->nr_cpus_allowed != 1 &&
	    cpupri_find(&rq->rd->cpupri, p, রিক্ত))
		ঘূৰাই;

	/*
	 * There appear to be other CPUs that can accept
	 * the current task but none can run 'p', so lets reschedule
	 * to try and push the current task away:
	 */
	requeue_task_rt(rq, p, 1);
	resched_curr(rq);
}

স্থির পূর্ণাঙ্ক balance_rt(সংরচনা rq *rq, সংরচনা rq_flags *rf)
{
	/*
	 * Note, rq->donor may change during rq lock drops,
	 * so don't re-use p across lock drops
	 */
	সংরচনা task_struct *p = rq->donor;

	যদি (!on_rt_rq(&p->rt) && need_pull_rt_task(rq, p)) {
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

	ঘূৰাই sched_stop_runnable(rq) || sched_dl_runnable(rq) || sched_rt_runnable(rq);
}

/*
 * Preempt the current task with a newly woken task if needed:
 */
স্থির শূন্য wakeup_preempt_rt(সংরচনা rq *rq, সংরচনা task_struct *p, পূর্ণাঙ্ক flags)
{
	সংরচনা task_struct *donor = rq->donor;

	/*
	 * XXX If we're preempted by DL, queue a push?
	 */
	যদি (p->sched_class != &rt_sched_class ||
	    donor->sched_class != &rt_sched_class)
		ঘূৰাই;

	যদি (p->prio < donor->prio) {
		resched_curr(rq);
		ঘূৰাই;
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
	যদি (p->prio == donor->prio && !test_tsk_need_resched(rq->curr))
		check_preempt_equal_prio(rq, p);
}

স্থির অন্তঃস্থ শূন্য set_next_task_rt(সংরচনা rq *rq, সংরচনা task_struct *p, গণনা snt_e type)
{
	সংরচনা sched_rt_entity *rt_se = &p->rt;
	সংরচনা rt_rq *rt_rq = &rq->rt;

	যদি (type == SNT_REPICK)
		ঘূৰাই;

	p->se.exec_start = rq_clock_task(rq);
	যদি (on_rt_rq(&p->rt))
		update_stats_wait_end_rt(rt_rq, rt_se);

	/* The running task is never eligible for pushing */
	dequeue_pushable_task(rq, p);

	যদি (type != SNT_PICK)
		ঘূৰাই;

	/*
	 * If prev task was rt, put_prev_task() has already updated the
	 * utilization. We only care of the case where we start to schedule a
	 * rt task
	 */
	যদি (rq->donor->sched_class != &rt_sched_class)
		update_rt_rq_load_avg(rq_clock_pelt(rq), rq, 0);

	rt_queue_ঠেলো_tasks(rq);
}

স্থির সংরচনা sched_rt_entity *pick_next_rt_entity(সংরচনা rt_rq *rt_rq)
{
	সংরচনা rt_prio_array *array = &rt_rq->active;
	সংরচনা sched_rt_entity *next = রিক্ত;
	সংরচনা list_head *queue;
	পূর্ণাঙ্ক idx;

	idx = sched_find_first_bit(array->bitmap);
	BUG_ON(idx >= MAX_RT_PRIO);

	queue = array->queue + idx;
	যদি (WARN_ON_ONCE(list_empty(queue)))
		ঘূৰাই রিক্ত;
	next = list_entry(queue->next, সংরচনা sched_rt_entity, run_list);

	ঘূৰাই next;
}

স্থির সংরচনা task_struct *_pick_next_task_rt(সংরচনা rq *rq)
{
	সংরচনা sched_rt_entity *rt_se;
	সংরচনা rt_rq *rt_rq  = &rq->rt;

	কৰা {
		rt_se = pick_next_rt_entity(rt_rq);
		যদি (unlikely(!rt_se))
			ঘূৰাই রিক্ত;
		rt_rq = group_rt_rq(rt_se);
	} যেতিয়ালৈ (rt_rq);

	ঘূৰাই rt_task_of(rt_se);
}

স্থির সংরচনা task_struct *pick_task_rt(সংরচনা rq *rq, সংরচনা rq_flags *rf)
{
	সংরচনা task_struct *p;

	যদি (!sched_rt_runnable(rq))
		ঘূৰাই রিক্ত;

	p = _pick_next_task_rt(rq);

	ঘূৰাই p;
}

স্থির শূন্য put_prev_task_rt(সংরচনা rq *rq, সংরচনা task_struct *p, সংরচনা task_struct *next)
{
	সংরচনা sched_rt_entity *rt_se = &p->rt;
	সংরচনা rt_rq *rt_rq = &rq->rt;

	যদি (on_rt_rq(&p->rt))
		update_stats_wait_start_rt(rt_rq, rt_se);

	update_curr_rt(rq);

	update_rt_rq_load_avg(rq_clock_pelt(rq), rq, 1);

	যদি (task_is_blocked(p))
		ঘূৰাই;
	/*
	 * The previous task needs to be made eligible for pushing
	 * if it is still active
	 */
	যদি (on_rt_rq(&p->rt) && p->nr_cpus_allowed > 1)
		enqueue_pushable_task(rq, p);
}

/* Only try algorithms three times */
#define RT_MAX_TRIES 3

/*
 * Return the highest pushable rq's task, which is suitable to be executed
 * on the CPU, NULL otherwise
 */
স্থির সংরচনা task_struct *pick_highest_pushable_task(সংরচনা rq *rq, পূর্ণাঙ্ক cpu)
{
	সংরচনা plist_head *head = &rq->rt.pushable_tasks;
	সংরচনা task_struct *p;

	যদি (!has_pushable_tasks(rq))
		ঘূৰাই রিক্ত;

	plist_for_each_entry(p, head, pushable_tasks) {
		যদি (task_is_pushable(rq, p, cpu))
			ঘূৰাই p;
	}

	ঘূৰাই রিক্ত;
}

স্থির DEFINE_PER_CPU(cpumask_var_t, local_cpu_mask);

স্থির পূর্ণাঙ্ক find_lowest_rq(সংরচনা task_struct *task)
{
	সংরচনা sched_domain *sd;
	সংরচনা cpumask *lowest_mask = this_cpu_cpumask_var_ptr(local_cpu_mask);
	পূর্ণাঙ্ক this_cpu = smp_processor_id();
	পূর্ণাঙ্ক cpu      = task_cpu(task);
	পূর্ণাঙ্ক ret;

	/* Make sure the mask is initialized first */
	যদি (unlikely(!lowest_mask))
		ঘূৰাই -1;

	যদি (task->nr_cpus_allowed == 1)
		ঘূৰাই -1; /* No other targets possible */

	/*
	 * If we're on asym system ensure we consider the different capacities
	 * of the CPUs when searching for the lowest_mask.
	 */
	যদি (sched_asym_cpucap_active()) {

		ret = cpupri_find_fitness(&task_rq(task)->rd->cpupri,
					  task, lowest_mask,
					  rt_task_fits_capacity);
	} অন্যথা {

		ret = cpupri_find(&task_rq(task)->rd->cpupri,
				  task, lowest_mask);
	}

	যদি (!ret)
		ঘূৰাই -1; /* No targets found */

	/*
	 * At this point we have built a mask of CPUs representing the
	 * lowest priority tasks in the system.  Now we want to elect
	 * the best one based on our affinity and topology.
	 *
	 * We prioritize the last CPU that the task executed on since
	 * it is most likely cache-hot in that location.
	 */
	যদি (cpumask_test_cpu(cpu, lowest_mask))
		ঘূৰাই cpu;

	/*
	 * Otherwise, we consult the sched_domains span maps to figure
	 * out which CPU is logically closest to our hot cache data.
	 */
	যদি (!cpumask_test_cpu(this_cpu, lowest_mask))
		this_cpu = -1; /* Skip this_cpu opt if not among lowest */

	rcu_read_lock();
	for_each_domain(cpu, sd) {
		যদি (sd->flags & SD_WAKE_AFFINE) {
			পূর্ণাঙ্ক best_cpu;

			/*
			 * "this_cpu" is cheaper to preempt than a
			 * remote processor.
			 */
			যদি (this_cpu != -1 &&
			    cpumask_test_cpu(this_cpu, sched_domain_span(sd))) {
				rcu_read_unlock();
				ঘূৰাই this_cpu;
			}

			best_cpu = cpumask_any_and_distribute(lowest_mask,
							      sched_domain_span(sd));
			যদি (best_cpu < nr_cpu_ids) {
				rcu_read_unlock();
				ঘূৰাই best_cpu;
			}
		}
	}
	rcu_read_unlock();

	/*
	 * And finally, if there were no matches within the domains
	 * just give the caller *something* to work with from the compatible
	 * locations.
	 */
	যদি (this_cpu != -1)
		ঘূৰাই this_cpu;

	cpu = cpumask_any_distribute(lowest_mask);
	যদি (cpu < nr_cpu_ids)
		ঘূৰাই cpu;

	ঘূৰাই -1;
}

স্থির সংরচনা task_struct *pick_next_pushable_task(সংরচনা rq *rq)
{
	সংরচনা plist_head *head = &rq->rt.pushable_tasks;
	সংরচনা task_struct *i, *p = রিক্ত;

	যদি (!has_pushable_tasks(rq))
		ঘূৰাই রিক্ত;

	plist_for_each_entry(i, head, pushable_tasks) {
		/* skip tasks that cannot be migrated */
		যদি (!task_on_cpu(rq, i) && !is_migration_disabled(i)) {
			p = i;
			ভঙ্গ;
		}
	}

	যদি (!p)
		ঘূৰাই রিক্ত;

	BUG_ON(rq->cpu != task_cpu(p));
	BUG_ON(task_current(rq, p));
	BUG_ON(task_current_donor(rq, p));
	BUG_ON(p->nr_cpus_allowed <= 1);

	BUG_ON(!task_on_rq_queued(p));
	BUG_ON(!rt_task(p));

	ঘূৰাই p;
}

/* Will lock the rq it finds */
স্থির সংরচনা rq *find_lock_lowest_rq(সংরচনা task_struct *task, সংরচনা rq *rq)
{
	সংরচনা rq *lowest_rq = রিক্ত;
	পূর্ণাঙ্ক tries;
	পূর্ণাঙ্ক cpu;

	বাবে (tries = 0; tries < RT_MAX_TRIES; tries++) {
		cpu = find_lowest_rq(task);

		যদি ((cpu == -1) || (cpu == rq->cpu))
			ভঙ্গ;

		lowest_rq = cpu_rq(cpu);

		যদি (lowest_rq->rt.highest_prio.curr <= task->prio) {
			/*
			 * Target rq has tasks of equal or higher priority,
			 * retrying does not release any lock and is unlikely
			 * to yield a different result.
			 */
			lowest_rq = রিক্ত;
			ভঙ্গ;
		}

		/* if the prio of this runqueue changed, try again */
		যদি (double_lock_balance(rq, lowest_rq)) {
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
			যদি (unlikely(is_migration_disabled(task) ||
				     !cpumask_test_cpu(lowest_rq->cpu, &task->cpus_mask) ||
				     task != pick_next_pushable_task(rq))) {

				double_unlock_balance(rq, lowest_rq);
				lowest_rq = রিক্ত;
				ভঙ্গ;
			}
		}

		/* If this rq is still suitable use it. */
		যদি (lowest_rq->rt.highest_prio.curr > task->prio)
			ভঙ্গ;

		/* try again */
		double_unlock_balance(rq, lowest_rq);
		lowest_rq = রিক্ত;
	}

	ঘূৰাই lowest_rq;
}

/*
 * If the current CPU has more than one RT task, see if the non
 * running task can migrate over to a CPU that is running a task
 * of lesser priority.
 */
স্থির পূর্ণাঙ্ক ঠেলো_rt_task(সংরচনা rq *rq, bool pull)
{
	সংরচনা task_struct *next_task;
	সংরচনা rq *lowest_rq;
	পূর্ণাঙ্ক ret = 0;

	যদি (!rq->rt.overloaded)
		ঘূৰাই 0;

	next_task = pick_next_pushable_task(rq);
	যদি (!next_task)
		ঘূৰাই 0;

retry:
	/*
	 * It's possible that the next_task slipped in of
	 * higher priority than current. If that's the case
	 * just reschedule current.
	 */
	যদি (unlikely(next_task->prio < rq->donor->prio)) {
		resched_curr(rq);
		ঘূৰাই 0;
	}

	যদি (is_migration_disabled(next_task)) {
		সংরচনা task_struct *ঠেলো_task = রিক্ত;
		পূর্ণাঙ্ক cpu;

		যদি (!pull || rq->ঠেলো_busy)
			ঘূৰাই 0;

		/*
		 * Invoking find_lowest_rq() on anything but an RT task doesn't
		 * make sense. Per the above priority check, curr has to
		 * be of higher priority than next_task, so no need to
		 * reschedule when bailing out.
		 *
		 * Note that the stoppers are masqueraded as SCHED_FIFO
		 * (cf. sched_set_stop_task()), so we can't rely on rt_task().
		 */
		যদি (rq->donor->sched_class != &rt_sched_class)
			ঘূৰাই 0;

		cpu = find_lowest_rq(rq->curr);
		যদি (cpu == -1 || cpu == rq->cpu)
			ঘূৰাই 0;

		/*
		 * Given we found a CPU with lower priority than @next_task,
		 * therefore it should be running. However we cannot migrate it
		 * to this other CPU, instead attempt to push the current
		 * running task on this CPU away.
		 */
		ঠেলো_task = get_ঠেলো_task(rq);
		যদি (ঠেলো_task) {
			preempt_disable();
			raw_spin_rq_unlock(rq);
			stop_one_cpu_nowait(rq->cpu, ঠেলো_cpu_stop,
					    ঠেলো_task, &rq->ঠেলো_work);
			preempt_enable();
			raw_spin_rq_lock(rq);
		}

		ঘূৰাই 0;
	}

	যদি (WARN_ON(next_task == rq->curr))
		ঘূৰাই 0;

	/* We might release rq lock */
	get_task_struct(next_task);

	/* find_lock_lowest_rq locks the rq if found */
	lowest_rq = find_lock_lowest_rq(next_task, rq);
	যদি (!lowest_rq) {
		সংরচনা task_struct *task;
		/*
		 * find_lock_lowest_rq releases rq->lock
		 * so it is possible that next_task has migrated.
		 *
		 * We need to make sure that the task is still on the same
		 * run-queue and is also still the next task eligible for
		 * pushing.
		 */
		task = pick_next_pushable_task(rq);
		যদি (task == next_task) {
			/*
			 * The task hasn't migrated, and is still the next
			 * eligible task, but we failed to find a run-queue
			 * to push it to.  Do not retry in this case, since
			 * other CPUs will pull from us when ready.
			 */
			গচ্ছ out;
		}

		যদি (!task)
			/* No more tasks, just exit */
			গচ্ছ out;

		/*
		 * Something has shifted, try again.
		 */
		put_task_struct(next_task);
		next_task = task;
		গচ্ছ retry;
	}

	move_queued_task_locked(rq, lowest_rq, next_task);
	resched_curr(lowest_rq);
	ret = 1;

	double_unlock_balance(rq, lowest_rq);
out:
	put_task_struct(next_task);

	ঘূৰাই ret;
}

স্থির শূন্য ঠেলো_rt_tasks(সংরচনা rq *rq)
{
	/* push_rt_task will return true if it moved an RT */
	যেতিয়ালৈ (ঠেলো_rt_task(rq, false))
		;
}

#ifdef HAVE_RT_ঠেলো_IPI

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
স্থির পূর্ণাঙ্ক rto_next_cpu(সংরচনা মূল_domain *rd)
{
	পূর্ণাঙ্ক this_cpu = smp_processor_id();
	পূর্ণাঙ্ক next;
	পূর্ণাঙ্ক cpu;

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
	বাবে (;;) {

		/* When rto_cpu is -1 this acts like cpumask_first() */
		cpu = cpumask_next(rd->rto_cpu, rd->rto_mask);

		rd->rto_cpu = cpu;

		/* Do not send IPI to self */
		যদি (cpu == this_cpu)
			অনুৱর্ত;

		যদি (cpu < nr_cpu_ids)
			ঘূৰাই cpu;

		rd->rto_cpu = -1;

		/*
		 * ACQUIRE ensures we see the @rto_mask changes
		 * made prior to the @next value observed.
		 *
		 * Matches WMB in rt_set_overload().
		 */
		next = atomic_read_acquire(&rd->rto_loop_next);

		যদি (rd->rto_loop == next)
			ভঙ্গ;

		rd->rto_loop = next;
	}

	ঘূৰাই -1;
}

স্থির অন্তঃস্থ bool rto_start_trylock(atomic_t *v)
{
	ঘূৰাই !atomic_cmpxchg_acquire(v, 0, 1);
}

স্থির অন্তঃস্থ শূন্য rto_start_unlock(atomic_t *v)
{
	atomic_set_release(v, 0);
}

স্থির শূন্য tell_cpu_to_ঠেলো(সংরচনা rq *rq)
{
	পূর্ণাঙ্ক cpu = -1;

	/* Keep the loop going if the IPI is currently active */
	atomic_inc(&rq->rd->rto_loop_next);

	/* Only one CPU can initiate a loop at a time */
	যদি (!rto_start_trylock(&rq->rd->rto_loop_start))
		ঘূৰাই;

	raw_spin_lock(&rq->rd->rto_lock);

	/*
	 * The rto_cpu is updated under the lock, if it has a valid CPU
	 * then the IPI is still running and will continue due to the
	 * update to loop_next, and nothing needs to be done here.
	 * Otherwise it is finishing up and an IPI needs to be sent.
	 */
	যদি (rq->rd->rto_cpu < 0)
		cpu = rto_next_cpu(rq->rd);

	raw_spin_unlock(&rq->rd->rto_lock);

	rto_start_unlock(&rq->rd->rto_loop_start);

	যদি (cpu >= 0) {
		/* Make sure the rd does not get freed while pushing */
		sched_get_rd(rq->rd);
		irq_work_queue_on(&rq->rd->rto_ঠেলো_work, cpu);
	}
}

/* Called from hardirq context */
শূন্য rto_ঠেলো_irq_work_func(সংরচনা irq_work *work)
{
	সংরচনা মূল_domain *rd =
		container_of(work, সংরচনা মূল_domain, rto_ঠেলো_work);
	সংরচনা rq *rq;
	পূর্ণাঙ্ক cpu;

	rq = this_rq();

	/*
	 * We do not need to grab the lock to check for has_pushable_tasks.
	 * When it gets updated, a check is made if a push is possible.
	 */
	যদি (has_pushable_tasks(rq)) {
		raw_spin_rq_lock(rq);
		যেতিয়ালৈ (ঠেলো_rt_task(rq, true))
			;
		raw_spin_rq_unlock(rq);
	}

	raw_spin_lock(&rd->rto_lock);

	/* Pass the IPI to the next rt overloaded queue */
	cpu = rto_next_cpu(rd);

	raw_spin_unlock(&rd->rto_lock);

	যদি (cpu < 0) {
		sched_put_rd(rd);
		ঘূৰাই;
	}

	/* Try the next RT overloaded CPU */
	irq_work_queue_on(&rd->rto_ঠেলো_work, cpu);
}
#endif /* HAVE_RT_PUSH_IPI */

স্থির শূন্য pull_rt_task(সংরচনা rq *this_rq)
{
	পূর্ণাঙ্ক this_cpu = this_rq->cpu, cpu;
	bool resched = false;
	সংরচনা task_struct *p, *ঠেলো_task;
	সংরচনা rq *src_rq;
	পূর্ণাঙ্ক rt_overload_গুনতি = rt_overloaded(this_rq);

	যদি (likely(!rt_overload_গুনতি))
		ঘূৰাই;

	/*
	 * Match the barrier from rt_set_overloaded; this guarantees that if we
	 * see overloaded we must also see the rto_mask bit.
	 */
	smp_rmb();

	/* If we are the only overloaded CPU do nothing */
	যদি (rt_overload_গুনতি == 1 &&
	    cpumask_test_cpu(this_rq->cpu, this_rq->rd->rto_mask))
		ঘূৰাই;

#ifdef HAVE_RT_ঠেলো_IPI
	যদি (sched_feat(RT_ঠেলো_IPI)) {
		tell_cpu_to_ঠেলো(this_rq);
		ঘূৰাই;
	}
#endif

	for_each_cpu(cpu, this_rq->rd->rto_mask) {
		যদি (this_cpu == cpu)
			অনুৱর্ত;

		src_rq = cpu_rq(cpu);

		/*
		 * Don't bother taking the src_rq->lock if the next highest
		 * task is known to be lower-priority than our current task.
		 * This may look racy, but if this value is about to go
		 * logically higher, the src_rq will push this task away.
		 * And if its going logically lower, we do not care
		 */
		যদি (src_rq->rt.highest_prio.next >=
		    this_rq->rt.highest_prio.curr)
			অনুৱর্ত;

		/*
		 * We can potentially drop this_rq's lock in
		 * double_lock_balance, and another CPU could
		 * alter this_rq
		 */
		ঠেলো_task = রিক্ত;
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
		যদি (p && (p->prio < this_rq->rt.highest_prio.curr)) {
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
			যদি (p->prio < src_rq->donor->prio)
				গচ্ছ skip;

			যদি (is_migration_disabled(p)) {
				ঠেলো_task = get_ঠেলো_task(src_rq);
			} অন্যথা {
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

		যদি (ঠেলো_task) {
			preempt_disable();
			raw_spin_rq_unlock(this_rq);
			stop_one_cpu_nowait(src_rq->cpu, ঠেলো_cpu_stop,
					    ঠেলো_task, &src_rq->ঠেলো_work);
			preempt_enable();
			raw_spin_rq_lock(this_rq);
		}
	}

	যদি (resched)
		resched_curr(this_rq);
}

/*
 * If we are not running and we are not going to reschedule soon, we should
 * try to push tasks away now
 */
স্থির শূন্য task_woken_rt(সংরচনা rq *rq, সংরচনা task_struct *p)
{
	bool need_to_ঠেলো = !task_on_cpu(rq, p) &&
			    !test_tsk_need_resched(rq->curr) &&
			    p->nr_cpus_allowed > 1 &&
			    (dl_task(rq->donor) || rt_task(rq->donor)) &&
			    (rq->curr->nr_cpus_allowed < 2 ||
			     rq->donor->prio <= p->prio);

	যদি (need_to_ঠেলো)
		ঠেলো_rt_tasks(rq);
}

/* Assumes rq->lock is held */
স্থির শূন্য rq_online_rt(সংরচনা rq *rq)
{
	যদি (rq->rt.overloaded)
		rt_set_overload(rq);

	__enable_runtime(rq);

	cpupri_set(&rq->rd->cpupri, rq->cpu, rq->rt.highest_prio.curr);
}

/* Assumes rq->lock is held */
স্থির শূন্য rq_offline_rt(সংরচনা rq *rq)
{
	যদি (rq->rt.overloaded)
		rt_clear_overload(rq);

	__disable_runtime(rq);

	cpupri_set(&rq->rd->cpupri, rq->cpu, CPUPRI_INVALID);
}

/*
 * When switch from the rt queue, we bring ourselves to a position
 * that we might want to pull RT tasks from other runqueues.
 */
স্থির শূন্য switched_from_rt(সংরচনা rq *rq, সংরচনা task_struct *p)
{
	/*
	 * If there are other RT tasks then we will reschedule
	 * and the scheduling of the other RT tasks will handle
	 * the balancing. But if we are the last RT task
	 * we may need to handle the pulling of RT tasks
	 * now.
	 */
	যদি (!task_on_rq_queued(p) || rq->rt.rt_nr_running)
		ঘূৰাই;

	rt_queue_pull_task(rq);
}

শূন্য __init init_sched_rt_class(শূন্য)
{
	unsigned পূর্ণাঙ্ক i;

	for_each_possible_cpu(i) {
		zalloc_cpumask_var_গ্ৰন্থি(&per_cpu(local_cpu_mask, i),
					GFP_KERNEL, cpu_to_গ্ৰন্থি(i));
	}
}

/*
 * When switching a task to RT, we may overload the runqueue
 * with RT tasks. In this case we try to push them off to
 * other runqueues.
 */
স্থির শূন্য switched_to_rt(সংরচনা rq *rq, সংরচনা task_struct *p)
{
	/*
	 * If we are running, update the avg_rt tracking, as the running time
	 * will now on be accounted into the latter.
	 */
	যদি (task_current(rq, p)) {
		update_rt_rq_load_avg(rq_clock_pelt(rq), rq, 0);
		ঘূৰাই;
	}

	/*
	 * If we are not running we may need to preempt the current
	 * running task. If that current running task is also an RT task
	 * then see if we can move to another run queue.
	 */
	যদি (task_on_rq_queued(p)) {
		যদি (p->nr_cpus_allowed > 1 && rq->rt.overloaded)
			rt_queue_ঠেলো_tasks(rq);
		যদি (p->prio < rq->donor->prio && cpu_online(cpu_of(rq)))
			resched_curr(rq);
	}
}

/*
 * Priority of the task has changed. This may cause
 * us to initiate a push or pull.
 */
স্থির শূন্য
prio_changed_rt(সংরচনা rq *rq, সংরচনা task_struct *p, u64 oldprio)
{
	যদি (!task_on_rq_queued(p))
		ঘূৰাই;

	যদি (p->prio == oldprio)
		ঘূৰাই;

	যদি (task_current_donor(rq, p)) {
		/*
		 * If our priority decreases while running, we
		 * may need to pull tasks to this runqueue.
		 */
		যদি (oldprio < p->prio)
			rt_queue_pull_task(rq);

		/*
		 * If there's a higher priority task waiting to run
		 * then reschedule.
		 */
		যদি (p->prio > rq->rt.highest_prio.curr)
			resched_curr(rq);
	} অন্যথা {
		/*
		 * This task is not running, but if it is
		 * greater than the current running task
		 * then reschedule.
		 */
		যদি (p->prio < rq->donor->prio)
			resched_curr(rq);
	}
}

#ifdef CONFIG_POSIX_TIMERS
স্থির শূন্য watchdog(সংরচনা rq *rq, সংরচনা task_struct *p)
{
	unsigned long soft, hard;

	/* max may change after cur was read, this will be fixed next tick */
	soft = task_rlimit(p, RLIMIT_RTTIME);
	hard = task_rlimit_max(p, RLIMIT_RTTIME);

	যদি (soft != RLIM_INFINITY) {
		unsigned long next;

		যদি (p->rt.watchdog_stamp != jiffies) {
			p->rt.timeout++;
			p->rt.watchdog_stamp = jiffies;
		}

		next = DIV_ROUND_UP(min(soft, hard), USEC_PER_SEC/HZ);
		যদি (p->rt.timeout > next) {
			posix_cputimers_rt_watchdog(&p->posix_cputimers,
						    p->se.sum_exec_runtime);
		}
	}
}
#অন্যথা /* !CONFIG_POSIX_TIMERS: */
স্থির অন্তঃস্থ শূন্য watchdog(সংরচনা rq *rq, সংরচনা task_struct *p) { }
#endif /* !CONFIG_POSIX_TIMERS */

/*
 * scheduler tick hitting a task of our scheduling class.
 *
 * NOTE: This function can be called remotely by the tick offload that
 * goes along full dynticks. Therefore no local assumption can be made
 * and everything must be accessed through the @rq and @curr passed in
 * parameters.
 */
স্থির শূন্য task_tick_rt(সংরচনা rq *rq, সংরচনা task_struct *p, পূর্ণাঙ্ক queued)
{
	সংরচনা sched_rt_entity *rt_se = &p->rt;

	update_curr_rt(rq);
	update_rt_rq_load_avg(rq_clock_pelt(rq), rq, 1);

	watchdog(rq, p);

	/*
	 * RR tasks need a special form of time-slice management.
	 * FIFO tasks have no timeslices.
	 */
	যদি (p->policy != SCHED_RR)
		ঘূৰাই;

	যদি (--p->rt.time_slice)
		ঘূৰাই;

	p->rt.time_slice = sched_rr_timeslice;

	/*
	 * Requeue to the end of queue if we (and all of our ancestors) are not
	 * the only element on the queue
	 */
	for_each_sched_rt_entity(rt_se) {
		যদি (rt_se->run_list.prev != rt_se->run_list.next) {
			requeue_task_rt(rq, p, 0);
			resched_curr(rq);
			ঘূৰাই;
		}
	}
}

স্থির unsigned পূর্ণাঙ্ক get_rr_interval_rt(সংরচনা rq *rq, সংরচনা task_struct *task)
{
	/*
	 * Time slice is 0 for SCHED_FIFO tasks
	 */
	যদি (task->policy == SCHED_RR)
		ঘূৰাই sched_rr_timeslice;
	অন্যথা
		ঘূৰাই 0;
}

#ifdef CONFIG_SCHED_CORE
স্থির পূর্ণাঙ্ক task_is_throttled_rt(সংরচনা task_struct *p, পূর্ণাঙ্ক cpu)
{
	সংরচনা rt_rq *rt_rq;

#ifdef CONFIG_RT_GROUP_SCHED // XXX maybe add task_rt_rq(), see also sched_rt_period_rt_rq
	rt_rq = task_group(p)->rt_rq[cpu];
	WARN_ON(!rt_group_sched_enabled() && rt_rq->tg != &মূল_task_group);
#অন্যথা
	rt_rq = &cpu_rq(cpu)->rt;
#endif

	ঘূৰাই rt_rq_throttled(rt_rq);
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
স্থির DEFINE_MUTEX(rt_constraints_mutex);

স্থির অন্তঃস্থ পূর্ণাঙ্ক tg_has_rt_tasks(সংরচনা task_group *tg)
{
	সংরচনা task_struct *task;
	সংরচনা css_task_iter it;
	পূর্ণাঙ্ক ret = 0;

	/*
	 * Autogroups do not have RT tasks; see autogroup_create().
	 */
	যদি (task_group_is_autogroup(tg))
		ঘূৰাই 0;

	css_task_iter_start(&tg->css, 0, &it);
	যেতিয়ালৈ (!ret && (task = css_task_iter_next(&it)))
		ret |= rt_task(task);
	css_task_iter_end(&it);

	ঘূৰাই ret;
}

সংরচনা rt_schedulable_data {
	সংরচনা task_group *tg;
	u64 rt_period;
	u64 rt_runtime;
};

স্থির পূর্ণাঙ্ক tg_rt_schedulable(সংরচনা task_group *tg, শূন্য *data)
{
	সংরচনা rt_schedulable_data *d = data;
	সংরচনা task_group *child;
	u64 total, sum = 0;
	u64 period, runtime;

	period = ktime_to_ns(tg->rt_bandwidth.rt_period);
	runtime = tg->rt_bandwidth.rt_runtime;

	যদি (tg == d->tg) {
		period = d->rt_period;
		runtime = d->rt_runtime;
	}

	/*
	 * Cannot have more runtime than the period.
	 */
	যদি (runtime > period && runtime != RUNTIME_INF)
		ঘূৰাই -EINVAL;

	/*
	 * Ensure we don't starve existing RT tasks if runtime turns zero.
	 */
	যদি (rt_bandwidth_enabled() && !runtime &&
	    tg->rt_bandwidth.rt_runtime && tg_has_rt_tasks(tg))
		ঘূৰাই -EBUSY;

	total = to_ratio(period, runtime);

	/*
	 * Nobody can have more than the global setting allows.
	 */
	যদি (total > to_ratio(global_rt_period(), global_rt_runtime()))
		ঘূৰাই -EINVAL;

	/*
	 * The sum of our children's runtime should not exceed our own.
	 */
	list_for_each_entry_rcu(child, &tg->children, siblings) {
		period = ktime_to_ns(child->rt_bandwidth.rt_period);
		runtime = child->rt_bandwidth.rt_runtime;

		যদি (child == d->tg) {
			period = d->rt_period;
			runtime = d->rt_runtime;
		}

		sum += to_ratio(period, runtime);
	}

	যদি (sum > total)
		ঘূৰাই -EINVAL;

	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক __rt_schedulable(সংরচনা task_group *tg, u64 period, u64 runtime)
{
	পূর্ণাঙ্ক ret;

	সংরচনা rt_schedulable_data data = {
		.tg = tg,
		.rt_period = period,
		.rt_runtime = runtime,
	};

	rcu_read_lock();
	ret = walk_tg_বৃক্ষ(tg_rt_schedulable, tg_nop, &data);
	rcu_read_unlock();

	ঘূৰাই ret;
}

স্থির পূর্ণাঙ্ক tg_set_rt_bandwidth(সংরচনা task_group *tg,
		u64 rt_period, u64 rt_runtime)
{
	পূর্ণাঙ্ক i, err = 0;

	/*
	 * Disallowing the root group RT runtime is BAD, it would disallow the
	 * kernel creating (and or operating) RT threads.
	 */
	যদি (tg == &মূল_task_group && rt_runtime == 0)
		ঘূৰাই -EINVAL;

	/* No period doesn't make any sense. */
	যদি (rt_period == 0)
		ঘূৰাই -EINVAL;

	/*
	 * Bound quota to defend quota against overflow during bandwidth shift.
	 */
	যদি (rt_runtime != RUNTIME_INF && rt_runtime > max_rt_runtime)
		ঘূৰাই -EINVAL;

	mutex_lock(&rt_constraints_mutex);
	err = __rt_schedulable(tg, rt_period, rt_runtime);
	যদি (err)
		গচ্ছ unlock;

	raw_spin_lock_irq(&tg->rt_bandwidth.rt_runtime_lock);
	tg->rt_bandwidth.rt_period = ns_to_ktime(rt_period);
	tg->rt_bandwidth.rt_runtime = rt_runtime;

	for_each_possible_cpu(i) {
		সংরচনা rt_rq *rt_rq = tg->rt_rq[i];

		raw_spin_lock(&rt_rq->rt_runtime_lock);
		rt_rq->rt_runtime = rt_runtime;
		raw_spin_unlock(&rt_rq->rt_runtime_lock);
	}
	raw_spin_unlock_irq(&tg->rt_bandwidth.rt_runtime_lock);
unlock:
	mutex_unlock(&rt_constraints_mutex);

	ঘূৰাই err;
}

পূর্ণাঙ্ক sched_group_set_rt_runtime(সংরচনা task_group *tg, long rt_runtime_us)
{
	u64 rt_runtime, rt_period;

	rt_period = ktime_to_ns(tg->rt_bandwidth.rt_period);
	rt_runtime = (u64)rt_runtime_us * NSEC_PER_USEC;
	যদি (rt_runtime_us < 0)
		rt_runtime = RUNTIME_INF;
	অন্যথা যদি ((u64)rt_runtime_us > U64_MAX / NSEC_PER_USEC)
		ঘূৰাই -EINVAL;

	ঘূৰাই tg_set_rt_bandwidth(tg, rt_period, rt_runtime);
}

long sched_group_rt_runtime(সংরচনা task_group *tg)
{
	u64 rt_runtime_us;

	যদি (tg->rt_bandwidth.rt_runtime == RUNTIME_INF)
		ঘূৰাই -1;

	rt_runtime_us = tg->rt_bandwidth.rt_runtime;
	do_div(rt_runtime_us, NSEC_PER_USEC);
	ঘূৰাই rt_runtime_us;
}

পূর্ণাঙ্ক sched_group_set_rt_period(সংরচনা task_group *tg, u64 rt_period_us)
{
	u64 rt_runtime, rt_period;

	যদি (rt_period_us > U64_MAX / NSEC_PER_USEC)
		ঘূৰাই -EINVAL;

	rt_period = rt_period_us * NSEC_PER_USEC;
	rt_runtime = tg->rt_bandwidth.rt_runtime;

	ঘূৰাই tg_set_rt_bandwidth(tg, rt_period, rt_runtime);
}

long sched_group_rt_period(সংরচনা task_group *tg)
{
	u64 rt_period_us;

	rt_period_us = ktime_to_ns(tg->rt_bandwidth.rt_period);
	do_div(rt_period_us, NSEC_PER_USEC);
	ঘূৰাই rt_period_us;
}

পূর্ণাঙ্ক sched_rt_can_attach(সংরচনা task_group *tg, সংরচনা task_struct *tsk)
{
	/* Don't accept real-time tasks when there is no way for them to run */
	যদি (rt_group_sched_enabled() && rt_task(tsk) && tg->rt_bandwidth.rt_runtime == 0)
		ঘূৰাই 0;

	ঘূৰাই 1;
}

#endif /* !CONFIG_RT_GROUP_SCHED */

#ifdef CONFIG_SYSCTL
স্থির পূর্ণাঙ্ক sched_rt_global_validate(শূন্য)
{
	যদি ((sysctl_sched_rt_runtime != RUNTIME_INF) &&
		((sysctl_sched_rt_runtime > sysctl_sched_rt_period) ||
		 ((u64)sysctl_sched_rt_runtime *
			NSEC_PER_USEC > max_rt_runtime)))
		ঘূৰাই -EINVAL;

#ifdef CONFIG_RT_GROUP_SCHED
	যদি (!rt_group_sched_enabled())
		ঘূৰাই 0;

	scoped_guard(mutex, &rt_constraints_mutex)
		ঘূৰাই __rt_schedulable(রিক্ত, 0, 0);
#endif
	ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক sched_rt_handler(ধ্রুৱ সংরচনা ctl_table *table, পূর্ণাঙ্ক লিখ, শূন্য *buffer,
		size_t *lenp, loff_t *ppos)
{
	পূর্ণাঙ্ক old_period, old_runtime;
	স্থির DEFINE_MUTEX(mutex);
	পূর্ণাঙ্ক ret;

	mutex_lock(&mutex);
	sched_domains_mutex_lock();
	old_period = sysctl_sched_rt_period;
	old_runtime = sysctl_sched_rt_runtime;

	ret = proc_dointvec_minmax(table, লিখ, buffer, lenp, ppos);

	যদি (!ret && লিখ) {
		ret = sched_rt_global_validate();
		যদি (ret)
			গচ্ছ undo;

		ret = sched_dl_global_validate();
		যদি (ret)
			গচ্ছ undo;

		sched_dl_do_global();
	}
	যদি (0) {
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

	ঘূৰাই ret;
}

স্থির পূর্ণাঙ্ক sched_rr_handler(ধ্রুৱ সংরচনা ctl_table *table, পূর্ণাঙ্ক লিখ, শূন্য *buffer,
		size_t *lenp, loff_t *ppos)
{
	পূর্ণাঙ্ক ret;
	স্থির DEFINE_MUTEX(mutex);

	mutex_lock(&mutex);
	ret = proc_dointvec(table, লিখ, buffer, lenp, ppos);
	/*
	 * Make sure that internally we keep jiffies.
	 * Also, writing zero resets the time-slice to default:
	 */
	যদি (!ret && লিখ) {
		sched_rr_timeslice =
			sysctl_sched_rr_timeslice <= 0 ? RR_TIMESLICE :
			msecs_to_jiffies(sysctl_sched_rr_timeslice);

		যদি (sysctl_sched_rr_timeslice <= 0)
			sysctl_sched_rr_timeslice = jiffies_to_msecs(RR_TIMESLICE);
	}
	mutex_unlock(&mutex);

	ঘূৰাই ret;
}
#endif /* CONFIG_SYSCTL */

শূন্য print_rt_stats(সংরচনা seq_file *m, পূর্ণাঙ্ক cpu)
{
	rt_rq_iter_t iter;
	সংরচনা rt_rq *rt_rq;

	rcu_read_lock();
	for_each_rt_rq(rt_rq, iter, cpu_rq(cpu))
		print_rt_rq(m, cpu, rt_rq);
	rcu_read_unlock();
}
