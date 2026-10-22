// SPDX-License-Identifier: GPL-2.0
/*
 * Real-Time Scheduling Class (mapped to the SCHED_FIFO and SCHED_RR
 * policies)
 */

#અન્તર્ભાવ "sched.h"
#અન્તર્ભાવ "pelt.h"

પૂર્ણાંક sched_rr_timeslice = RR_TIMESLICE;
/* More than 4 hours if BW_SHIFT equals 20. */
સ્થિર અચલ u64 max_rt_runtime = MAX_BW;

/*
 * period over which we measure -rt task CPU usage in us.
 * default: 1s
 */
પૂર્ણાંક sysctl_sched_rt_period = 1000000;

/*
 * part of the period that we allow rt tasks to run in us.
 * default: 1s
 */
પૂર્ણાંક sysctl_sched_rt_runtime = 1000000;

#ifdef CONFIG_SYSCTL
સ્થિર પૂર્ણાંક sysctl_sched_rr_timeslice = (MSEC_PER_SEC * RR_TIMESLICE) / HZ;
સ્થિર પૂર્ણાંક sched_rt_handler(અચલ સંરચના ctl_table *table, પૂર્ણાંક લિખ, શૂન્ય *buffer,
		size_t *lenp, loff_t *ppos);
સ્થિર પૂર્ણાંક sched_rr_handler(અચલ સંરચના ctl_table *table, પૂર્ણાંક લિખ, શૂન્ય *buffer,
		size_t *lenp, loff_t *ppos);
સ્થિર અચલ સંરચના ctl_table sched_rt_sysctls[] = {
	{
		.procname       = "sched_rt_period_us",
		.data           = &sysctl_sched_rt_period,
		.maxlen         = કદ(પૂર્ણાંક),
		.mode           = 0644,
		.proc_handler   = sched_rt_handler,
		.extra1         = SYSCTL_ONE,
		.extra2         = SYSCTL_INT_MAX,
	},
	{
		.procname       = "sched_rt_runtime_us",
		.data           = &sysctl_sched_rt_runtime,
		.maxlen         = કદ(પૂર્ણાંક),
		.mode           = 0644,
		.proc_handler   = sched_rt_handler,
		.extra1         = SYSCTL_NEG_ONE,
		.extra2         = (શૂન્ય *)&sysctl_sched_rt_period,
	},
	{
		.procname       = "sched_rr_timeslice_ms",
		.data           = &sysctl_sched_rr_timeslice,
		.maxlen         = કદ(પૂર્ણાંક),
		.mode           = 0644,
		.proc_handler   = sched_rr_handler,
	},
};

સ્થિર પૂર્ણાંક __init sched_rt_sysctl_init(શૂન્ય)
{
	register_sysctl_init("kernel", sched_rt_sysctls);
	પાછા 0;
}
late_initcall(sched_rt_sysctl_init);
#endif /* CONFIG_SYSCTL */

શૂન્ય init_rt_rq(સંરચના rt_rq *rt_rq)
{
	સંરચના rt_prio_array *array;
	પૂર્ણાંક i;

	array = &rt_rq->active;
	માટે (i = 0; i < MAX_RT_PRIO; i++) {
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
	rt_rq->tg = &મૂળ_task_group;
#endif
}

#ifdef CONFIG_RT_GROUP_SCHED

સ્થિર પૂર્ણાંક do_sched_rt_period_timer(સંરચના rt_bandwidth *rt_b, પૂર્ણાંક overrun);

સ્થિર ગણના hrtimer_restart sched_rt_period_timer(સંરચના hrtimer *timer)
{
	સંરચના rt_bandwidth *rt_b =
		container_of(timer, સંરચના rt_bandwidth, rt_period_timer);
	પૂર્ણાંક idle = 0;
	પૂર્ણાંક overrun;

	raw_spin_lock(&rt_b->rt_runtime_lock);
	માટે (;;) {
		overrun = hrtimer_forward_now(timer, rt_b->rt_period);
		જો (!overrun)
			તોડો;

		raw_spin_unlock(&rt_b->rt_runtime_lock);
		idle = do_sched_rt_period_timer(rt_b, overrun);
		raw_spin_lock(&rt_b->rt_runtime_lock);
	}
	જો (idle)
		rt_b->rt_period_active = 0;
	raw_spin_unlock(&rt_b->rt_runtime_lock);

	પાછા idle ? HRTIMER_NORESTART : HRTIMER_RESTART;
}

શૂન્ય init_rt_bandwidth(સંરચના rt_bandwidth *rt_b, u64 period, u64 runtime)
{
	rt_b->rt_period = ns_to_ktime(period);
	rt_b->rt_runtime = runtime;

	raw_spin_lock_init(&rt_b->rt_runtime_lock);

	hrtimer_setup(&rt_b->rt_period_timer, sched_rt_period_timer, CLOCK_MONOTONIC,
		      HRTIMER_MODE_REL_HARD);
}

સ્થિર અંતઃરેખ શૂન્ય do_start_rt_bandwidth(સંરચના rt_bandwidth *rt_b)
{
	raw_spin_lock(&rt_b->rt_runtime_lock);
	જો (!rt_b->rt_period_active) {
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

સ્થિર શૂન્ય start_rt_bandwidth(સંરચના rt_bandwidth *rt_b)
{
	જો (!rt_bandwidth_enabled() || rt_b->rt_runtime == RUNTIME_INF)
		પાછા;

	do_start_rt_bandwidth(rt_b);
}

સ્થિર શૂન્ય destroy_rt_bandwidth(સંરચના rt_bandwidth *rt_b)
{
	hrtimer_cancel(&rt_b->rt_period_timer);
}

#define rt_entity_is_task(rt_se) (!(rt_se)->my_q)

સ્થિર અંતઃરેખ સંરચના task_struct *rt_task_of(સંરચના sched_rt_entity *rt_se)
{
	WARN_ON_ONCE(!rt_entity_is_task(rt_se));

	પાછા container_of(rt_se, સંરચના task_struct, rt);
}

સ્થિર અંતઃરેખ સંરચના rq *rq_of_rt_rq(સંરચના rt_rq *rt_rq)
{
	/* Cannot fold with non-CONFIG_RT_GROUP_SCHED version, layout */
	WARN_ON(!rt_group_sched_enabled() && rt_rq->tg != &મૂળ_task_group);
	પાછા rt_rq->rq;
}

સ્થિર અંતઃરેખ સંરચના rt_rq *rt_rq_of_se(સંરચના sched_rt_entity *rt_se)
{
	WARN_ON(!rt_group_sched_enabled() && rt_se->rt_rq->tg != &મૂળ_task_group);
	પાછા rt_se->rt_rq;
}

સ્થિર અંતઃરેખ સંરચના rq *rq_of_rt_se(સંરચના sched_rt_entity *rt_se)
{
	સંરચના rt_rq *rt_rq = rt_se->rt_rq;

	WARN_ON(!rt_group_sched_enabled() && rt_rq->tg != &મૂળ_task_group);
	પાછા rt_rq->rq;
}

શૂન્ય unregister_rt_sched_group(સંરચના task_group *tg)
{
	જો (!rt_group_sched_enabled())
		પાછા;

	જો (tg->rt_se)
		destroy_rt_bandwidth(&tg->rt_bandwidth);
}

શૂન્ય free_rt_sched_group(સંરચના task_group *tg)
{
	પૂર્ણાંક i;

	જો (!rt_group_sched_enabled())
		પાછા;

	for_each_possible_cpu(i) {
		જો (tg->rt_rq)
			kfree(tg->rt_rq[i]);
		જો (tg->rt_se)
			kfree(tg->rt_se[i]);
	}

	kfree(tg->rt_rq);
	kfree(tg->rt_se);
}

શૂન્ય init_tg_rt_entry(સંરચના task_group *tg, સંરચના rt_rq *rt_rq,
		સંરચના sched_rt_entity *rt_se, પૂર્ણાંક cpu,
		સંરચના sched_rt_entity *parent)
{
	સંરચના rq *rq = cpu_rq(cpu);

	rt_rq->highest_prio.curr = MAX_RT_PRIO-1;
	rt_rq->rt_nr_boosted = 0;
	rt_rq->rq = rq;
	rt_rq->tg = tg;

	tg->rt_rq[cpu] = rt_rq;
	tg->rt_se[cpu] = rt_se;

	જો (!rt_se)
		પાછા;

	જો (!parent)
		rt_se->rt_rq = &rq->rt;
	નહીંતર
		rt_se->rt_rq = parent->my_q;

	rt_se->my_q = rt_rq;
	rt_se->parent = parent;
	INIT_LIST_HEAD(&rt_se->run_list);
}

પૂર્ણાંક alloc_rt_sched_group(સંરચના task_group *tg, સંરચના task_group *parent)
{
	સંરચના rt_rq *rt_rq;
	સંરચના sched_rt_entity *rt_se;
	પૂર્ણાંક i;

	જો (!rt_group_sched_enabled())
		પાછા 1;

	tg->rt_rq = kzalloc_objs(rt_rq, nr_cpu_ids);
	જો (!tg->rt_rq)
		જાઓ err;
	tg->rt_se = kzalloc_objs(rt_se, nr_cpu_ids);
	જો (!tg->rt_se)
		જાઓ err;

	init_rt_bandwidth(&tg->rt_bandwidth, ktime_to_ns(global_rt_period()), 0);

	for_each_possible_cpu(i) {
		rt_rq = kzalloc_ગાંઠ(કદ(સંરચના rt_rq),
				     GFP_KERNEL, cpu_to_ગાંઠ(i));
		જો (!rt_rq)
			જાઓ err;

		rt_se = kzalloc_ગાંઠ(કદ(સંરચના sched_rt_entity),
				     GFP_KERNEL, cpu_to_ગાંઠ(i));
		જો (!rt_se)
			જાઓ err_free_rq;

		init_rt_rq(rt_rq);
		rt_rq->rt_runtime = tg->rt_bandwidth.rt_runtime;
		init_tg_rt_entry(tg, rt_rq, rt_se, i, parent->rt_se[i]);
	}

	પાછા 1;

err_free_rq:
	kfree(rt_rq);
err:
	પાછા 0;
}

#નહીંતર /* !CONFIG_RT_GROUP_SCHED: */

#define rt_entity_is_task(rt_se) (1)

સ્થિર અંતઃરેખ સંરચના task_struct *rt_task_of(સંરચના sched_rt_entity *rt_se)
{
	પાછા container_of(rt_se, સંરચના task_struct, rt);
}

સ્થિર અંતઃરેખ સંરચના rq *rq_of_rt_rq(સંરચના rt_rq *rt_rq)
{
	પાછા container_of(rt_rq, સંરચના rq, rt);
}

સ્થિર અંતઃરેખ સંરચના rq *rq_of_rt_se(સંરચના sched_rt_entity *rt_se)
{
	સંરચના task_struct *p = rt_task_of(rt_se);

	પાછા task_rq(p);
}

સ્થિર અંતઃરેખ સંરચના rt_rq *rt_rq_of_se(સંરચના sched_rt_entity *rt_se)
{
	સંરચના rq *rq = rq_of_rt_se(rt_se);

	પાછા &rq->rt;
}

શૂન્ય unregister_rt_sched_group(સંરચના task_group *tg) { }

શૂન્ય free_rt_sched_group(સંરચના task_group *tg) { }

પૂર્ણાંક alloc_rt_sched_group(સંરચના task_group *tg, સંરચના task_group *parent)
{
	પાછા 1;
}
#endif /* !CONFIG_RT_GROUP_SCHED */

સ્થિર અંતઃરેખ bool need_pull_rt_task(સંરચના rq *rq, સંરચના task_struct *prev)
{
	/* Try to pull RT tasks here if we lower this rq's prio */
	પાછા rq->online && rq->rt.highest_prio.curr > prev->prio;
}

સ્થિર અંતઃરેખ પૂર્ણાંક rt_overloaded(સંરચના rq *rq)
{
	પાછા atomic_read(&rq->rd->rto_ગણતરી);
}

સ્થિર અંતઃરેખ શૂન્ય rt_set_overload(સંરચના rq *rq)
{
	જો (!rq->online)
		પાછા;

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
	atomic_inc(&rq->rd->rto_ગણતરી);
}

સ્થિર અંતઃરેખ શૂન્ય rt_clear_overload(સંરચના rq *rq)
{
	જો (!rq->online)
		પાછા;

	/* the order here really doesn't matter */
	atomic_dec(&rq->rd->rto_ગણતરી);
	cpumask_clear_cpu(rq->cpu, rq->rd->rto_mask);
}

સ્થિર અંતઃરેખ પૂર્ણાંક has_pushable_tasks(સંરચના rq *rq)
{
	પાછા !plist_head_empty(&rq->rt.pushable_tasks);
}

સ્થિર DEFINE_PER_CPU(સંરચના balance_callback, rt_ધકેલો_head);
સ્થિર DEFINE_PER_CPU(સંરચના balance_callback, rt_pull_head);

સ્થિર શૂન્ય ધકેલો_rt_tasks(સંરચના rq *);
સ્થિર શૂન્ય pull_rt_task(સંરચના rq *);

સ્થિર અંતઃરેખ શૂન્ય rt_queue_ધકેલો_tasks(સંરચના rq *rq)
{
	જો (!has_pushable_tasks(rq))
		પાછા;

	queue_balance_callback(rq, &per_cpu(rt_ધકેલો_head, rq->cpu), ધકેલો_rt_tasks);
}

સ્થિર અંતઃરેખ શૂન્ય rt_queue_pull_task(સંરચના rq *rq)
{
	queue_balance_callback(rq, &per_cpu(rt_pull_head, rq->cpu), pull_rt_task);
}

સ્થિર શૂન્ય enqueue_pushable_task(સંરચના rq *rq, સંરચના task_struct *p)
{
	plist_del(&p->pushable_tasks, &rq->rt.pushable_tasks);
	plist_ગાંઠ_init(&p->pushable_tasks, p->prio);
	plist_add(&p->pushable_tasks, &rq->rt.pushable_tasks);

	/* Update the highest prio pushable task */
	જો (p->prio < rq->rt.highest_prio.next)
		rq->rt.highest_prio.next = p->prio;

	જો (!rq->rt.overloaded) {
		rt_set_overload(rq);
		rq->rt.overloaded = 1;
	}
}

સ્થિર શૂન્ય dequeue_pushable_task(સંરચના rq *rq, સંરચના task_struct *p)
{
	plist_del(&p->pushable_tasks, &rq->rt.pushable_tasks);

	/* Update the new highest prio pushable task */
	જો (has_pushable_tasks(rq)) {
		p = plist_first_entry(&rq->rt.pushable_tasks,
				      સંરચના task_struct, pushable_tasks);
		rq->rt.highest_prio.next = p->prio;
	} નહીંતર {
		rq->rt.highest_prio.next = MAX_RT_PRIO-1;

		જો (rq->rt.overloaded) {
			rt_clear_overload(rq);
			rq->rt.overloaded = 0;
		}
	}
}

સ્થિર શૂન્ય enqueue_ટોચ_rt_rq(સંરચના rt_rq *rt_rq);
સ્થિર શૂન્ય dequeue_ટોચ_rt_rq(સંરચના rt_rq *rt_rq, unsigned પૂર્ણાંક ગણતરી);

સ્થિર અંતઃરેખ પૂર્ણાંક on_rt_rq(સંરચના sched_rt_entity *rt_se)
{
	પાછા rt_se->on_rq;
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
સ્થિર અંતઃરેખ bool rt_task_fits_capacity(સંરચના task_struct *p, પૂર્ણાંક cpu)
{
	unsigned પૂર્ણાંક min_cap;
	unsigned પૂર્ણાંક max_cap;
	unsigned પૂર્ણાંક cpu_cap;

	/* Only heterogeneous systems can benefit from this check */
	જો (!sched_asym_cpucap_active())
		પાછા true;

	min_cap = uclamp_eff_કિંમત(p, UCLAMP_MIN);
	max_cap = uclamp_eff_કિંમત(p, UCLAMP_MAX);

	cpu_cap = arch_scale_cpu_capacity(cpu);

	પાછા cpu_cap >= min(min_cap, max_cap);
}
#નહીંતર /* !CONFIG_UCLAMP_TASK: */
સ્થિર અંતઃરેખ bool rt_task_fits_capacity(સંરચના task_struct *p, પૂર્ણાંક cpu)
{
	પાછા true;
}
#endif /* !CONFIG_UCLAMP_TASK */

#ifdef CONFIG_RT_GROUP_SCHED

સ્થિર અંતઃરેખ u64 sched_rt_runtime(સંરચના rt_rq *rt_rq)
{
	પાછા rt_rq->rt_runtime;
}

સ્થિર અંતઃરેખ u64 sched_rt_period(સંરચના rt_rq *rt_rq)
{
	પાછા ktime_to_ns(rt_rq->tg->rt_bandwidth.rt_period);
}

પ્રકારવ્યાખ્યા સંરચના task_group *rt_rq_iter_t;

સ્થિર અંતઃરેખ સંરચના task_group *next_task_group(સંરચના task_group *tg)
{
	જો (!rt_group_sched_enabled()) {
		WARN_ON(tg != &મૂળ_task_group);
		પાછા ખાલી;
	}

	કરો {
		tg = list_entry_rcu(tg->list.next,
			typeof(સંરચના task_group), list);
	} જ્યારે (&tg->list != &task_groups && task_group_is_autogroup(tg));

	જો (&tg->list == &task_groups)
		tg = ખાલી;

	પાછા tg;
}

#define for_each_rt_rq(rt_rq, iter, rq)					\
	માટે (iter = &મૂળ_task_group;					\
		iter && (rt_rq = iter->rt_rq[cpu_of(rq)]);		\
		iter = next_task_group(iter))

#define for_each_sched_rt_entity(rt_se) \
	માટે (; rt_se; rt_se = rt_se->parent)

સ્થિર અંતઃરેખ સંરચના rt_rq *group_rt_rq(સંરચના sched_rt_entity *rt_se)
{
	પાછા rt_se->my_q;
}

સ્થિર શૂન્ય enqueue_rt_entity(સંરચના sched_rt_entity *rt_se, unsigned પૂર્ણાંક flags);
સ્થિર શૂન્ય dequeue_rt_entity(સંરચના sched_rt_entity *rt_se, unsigned પૂર્ણાંક flags);

સ્થિર શૂન્ય sched_rt_rq_enqueue(સંરચના rt_rq *rt_rq)
{
	સંરચના task_struct *donor = rq_of_rt_rq(rt_rq)->donor;
	સંરચના rq *rq = rq_of_rt_rq(rt_rq);
	સંરચના sched_rt_entity *rt_se;

	પૂર્ણાંક cpu = cpu_of(rq);

	rt_se = rt_rq->tg->rt_se[cpu];

	જો (rt_rq->rt_nr_running) {
		જો (!rt_se)
			enqueue_ટોચ_rt_rq(rt_rq);
		નહીંતર જો (!on_rt_rq(rt_se))
			enqueue_rt_entity(rt_se, 0);

		જો (rt_rq->highest_prio.curr < donor->prio)
			resched_curr(rq);
	}
}

સ્થિર શૂન્ય sched_rt_rq_dequeue(સંરચના rt_rq *rt_rq)
{
	સંરચના sched_rt_entity *rt_se;
	પૂર્ણાંક cpu = cpu_of(rq_of_rt_rq(rt_rq));

	rt_se = rt_rq->tg->rt_se[cpu];

	જો (!rt_se) {
		dequeue_ટોચ_rt_rq(rt_rq, rt_rq->rt_nr_running);
		/* Kick cpufreq (see the comment in kernel/sched/sched.h). */
		cpufreq_update_util(rq_of_rt_rq(rt_rq), 0);
	}
	નહીંતર જો (on_rt_rq(rt_se))
		dequeue_rt_entity(rt_se, 0);
}

સ્થિર અંતઃરેખ પૂર્ણાંક rt_rq_throttled(સંરચના rt_rq *rt_rq)
{
	પાછા rt_rq->rt_throttled && !rt_rq->rt_nr_boosted;
}

સ્થિર પૂર્ણાંક rt_se_boosted(સંરચના sched_rt_entity *rt_se)
{
	સંરચના rt_rq *rt_rq = group_rt_rq(rt_se);
	સંરચના task_struct *p;

	જો (rt_rq)
		પાછા !!rt_rq->rt_nr_boosted;

	p = rt_task_of(rt_se);
	પાછા p->prio != p->normal_prio;
}

સ્થિર અંતઃરેખ અચલ સંરચના cpumask *sched_rt_period_mask(શૂન્ય)
{
	પાછા this_rq()->rd->span;
}

સ્થિર અંતઃરેખ
સંરચના rt_rq *sched_rt_period_rt_rq(સંરચના rt_bandwidth *rt_b, પૂર્ણાંક cpu)
{
	પાછા container_of(rt_b, સંરચના task_group, rt_bandwidth)->rt_rq[cpu];
}

સ્થિર અંતઃરેખ સંરચના rt_bandwidth *sched_rt_bandwidth(સંરચના rt_rq *rt_rq)
{
	પાછા &rt_rq->tg->rt_bandwidth;
}

bool sched_rt_bandwidth_account(સંરચના rt_rq *rt_rq)
{
	સંરચના rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);

	પાછા (hrtimer_active(&rt_b->rt_period_timer) ||
		rt_rq->rt_time < rt_b->rt_runtime);
}

/*
 * We ran out of runtime, see if we can borrow some from our neighbours.
 */
સ્થિર શૂન્ય do_balance_runtime(સંરચના rt_rq *rt_rq)
{
	સંરચના rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);
	સંરચના મૂળ_domain *rd = rq_of_rt_rq(rt_rq)->rd;
	પૂર્ણાંક i, weight;
	u64 rt_period;

	weight = cpumask_weight(rd->span);

	raw_spin_lock(&rt_b->rt_runtime_lock);
	rt_period = ktime_to_ns(rt_b->rt_period);
	for_each_cpu(i, rd->span) {
		સંરચના rt_rq *iter = sched_rt_period_rt_rq(rt_b, i);
		s64 diff;

		જો (iter == rt_rq)
			ચાલુ;

		raw_spin_lock(&iter->rt_runtime_lock);
		/*
		 * Either all rqs have inf runtime and there's nothing to steal
		 * or __disable_runtime() below sets a specific rq to inf to
		 * indicate its been disabled and disallow stealing.
		 */
		જો (iter->rt_runtime == RUNTIME_INF)
			જાઓ next;

		/*
		 * From runqueues with spare time, take 1/n part of their
		 * spare time, but no more than our period.
		 */
		diff = iter->rt_runtime - iter->rt_time;
		જો (diff > 0) {
			diff = div_u64((u64)diff, weight);
			જો (rt_rq->rt_runtime + diff > rt_period)
				diff = rt_period - rt_rq->rt_runtime;
			iter->rt_runtime -= diff;
			rt_rq->rt_runtime += diff;
			જો (rt_rq->rt_runtime == rt_period) {
				raw_spin_unlock(&iter->rt_runtime_lock);
				તોડો;
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
સ્થિર શૂન્ય __disable_runtime(સંરચના rq *rq)
{
	સંરચના મૂળ_domain *rd = rq->rd;
	rt_rq_iter_t iter;
	સંરચના rt_rq *rt_rq;

	જો (unlikely(!scheduler_running))
		પાછા;

	for_each_rt_rq(rt_rq, iter, rq) {
		સંરચના rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);
		s64 want;
		પૂર્ણાંક i;

		raw_spin_lock(&rt_b->rt_runtime_lock);
		raw_spin_lock(&rt_rq->rt_runtime_lock);
		/*
		 * Either we're all inf and nobody needs to borrow, or we're
		 * already disabled and thus have nothing to do, or we have
		 * exactly the right amount of runtime to take out.
		 */
		જો (rt_rq->rt_runtime == RUNTIME_INF ||
				rt_rq->rt_runtime == rt_b->rt_runtime)
			જાઓ balanced;
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
			સંરચના rt_rq *iter = sched_rt_period_rt_rq(rt_b, i);
			s64 diff;

			/*
			 * Can't reclaim from ourselves or disabled runqueues.
			 */
			જો (iter == rt_rq || iter->rt_runtime == RUNTIME_INF)
				ચાલુ;

			raw_spin_lock(&iter->rt_runtime_lock);
			જો (want > 0) {
				diff = min_t(s64, iter->rt_runtime, want);
				iter->rt_runtime -= diff;
				want -= diff;
			} નહીંતર {
				iter->rt_runtime -= want;
				want -= want;
			}
			raw_spin_unlock(&iter->rt_runtime_lock);

			જો (!want)
				તોડો;
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

સ્થિર શૂન્ય __enable_runtime(સંરચના rq *rq)
{
	rt_rq_iter_t iter;
	સંરચના rt_rq *rt_rq;

	જો (unlikely(!scheduler_running))
		પાછા;

	/*
	 * Reset each runqueue's bandwidth settings
	 */
	for_each_rt_rq(rt_rq, iter, rq) {
		સંરચના rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);

		raw_spin_lock(&rt_b->rt_runtime_lock);
		raw_spin_lock(&rt_rq->rt_runtime_lock);
		rt_rq->rt_runtime = rt_b->rt_runtime;
		rt_rq->rt_time = 0;
		rt_rq->rt_throttled = 0;
		raw_spin_unlock(&rt_rq->rt_runtime_lock);
		raw_spin_unlock(&rt_b->rt_runtime_lock);
	}
}

સ્થિર શૂન્ય balance_runtime(સંરચના rt_rq *rt_rq)
{
	જો (!sched_feat(RT_RUNTIME_SHARE))
		પાછા;

	જો (rt_rq->rt_time > rt_rq->rt_runtime) {
		raw_spin_unlock(&rt_rq->rt_runtime_lock);
		do_balance_runtime(rt_rq);
		raw_spin_lock(&rt_rq->rt_runtime_lock);
	}
}

સ્થિર પૂર્ણાંક do_sched_rt_period_timer(સંરચના rt_bandwidth *rt_b, પૂર્ણાંક overrun)
{
	પૂર્ણાંક i, idle = 1, throttled = 0;
	અચલ સંરચના cpumask *span;

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
	જો (rt_b == &મૂળ_task_group.rt_bandwidth)
		span = cpu_online_mask;

	for_each_cpu(i, span) {
		પૂર્ણાંક enqueue = 0;
		સંરચના rt_rq *rt_rq = sched_rt_period_rt_rq(rt_b, i);
		સંરચના rq *rq = rq_of_rt_rq(rt_rq);
		સંરચના rq_flags rf;
		પૂર્ણાંક skip;

		/*
		 * When span == cpu_online_mask, taking each rq->lock
		 * can be time-consuming. Try to avoid it when possible.
		 */
		raw_spin_lock(&rt_rq->rt_runtime_lock);
		જો (!sched_feat(RT_RUNTIME_SHARE) && rt_rq->rt_runtime != RUNTIME_INF)
			rt_rq->rt_runtime = rt_b->rt_runtime;
		skip = !rt_rq->rt_time && !rt_rq->rt_nr_running;
		raw_spin_unlock(&rt_rq->rt_runtime_lock);
		જો (skip)
			ચાલુ;

		rq_lock(rq, &rf);
		update_rq_clock(rq);

		જો (rt_rq->rt_time) {
			u64 runtime;

			raw_spin_lock(&rt_rq->rt_runtime_lock);
			જો (rt_rq->rt_throttled)
				balance_runtime(rt_rq);
			runtime = rt_rq->rt_runtime;
			rt_rq->rt_time -= min(rt_rq->rt_time, overrun*runtime);
			જો (rt_rq->rt_throttled && rt_rq->rt_time < runtime) {
				rt_rq->rt_throttled = 0;
				enqueue = 1;

				/*
				 * When we're idle and a woken (rt) task is
				 * throttled wakeup_preempt() will set
				 * skip_update and the time between the wakeup
				 * and this unthrottle will get accounted as
				 * 'runtime'.
				 */
				જો (rt_rq->rt_nr_running && rq->curr == rq->idle)
					rq_clock_cancel_skipupdate(rq);
			}
			જો (rt_rq->rt_time || rt_rq->rt_nr_running)
				idle = 0;
			raw_spin_unlock(&rt_rq->rt_runtime_lock);
		} નહીંતર જો (rt_rq->rt_nr_running) {
			idle = 0;
			જો (!rt_rq_throttled(rt_rq))
				enqueue = 1;
		}
		જો (rt_rq->rt_throttled)
			throttled = 1;

		જો (enqueue)
			sched_rt_rq_enqueue(rt_rq);
		rq_unlock(rq, &rf);
	}

	જો (!throttled && (!rt_bandwidth_enabled() || rt_b->rt_runtime == RUNTIME_INF))
		પાછા 1;

	પાછા idle;
}

સ્થિર પૂર્ણાંક sched_rt_runtime_exceeded(સંરચના rt_rq *rt_rq)
{
	u64 runtime = sched_rt_runtime(rt_rq);

	જો (rt_rq->rt_throttled)
		પાછા rt_rq_throttled(rt_rq);

	જો (runtime >= sched_rt_period(rt_rq))
		પાછા 0;

	balance_runtime(rt_rq);
	runtime = sched_rt_runtime(rt_rq);
	જો (runtime == RUNTIME_INF)
		પાછા 0;

	જો (rt_rq->rt_time > runtime) {
		સંરચના rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);

		/*
		 * Don't actually throttle groups that have no runtime assigned
		 * but accrue some time due to boosting.
		 */
		જો (likely(rt_b->rt_runtime)) {
			rt_rq->rt_throttled = 1;
			printk_deferred_once("sched: RT throttling activated\n");
		} નહીંતર {
			/*
			 * In case we did anyway, make it go away,
			 * replenishment is a joke, since it will replenish us
			 * with exactly 0 ns.
			 */
			rt_rq->rt_time = 0;
		}

		જો (rt_rq_throttled(rt_rq)) {
			sched_rt_rq_dequeue(rt_rq);
			પાછા 1;
		}
	}

	પાછા 0;
}

#નહીંતર /* !CONFIG_RT_GROUP_SCHED: */

પ્રકારવ્યાખ્યા સંરચના rt_rq *rt_rq_iter_t;

#define for_each_rt_rq(rt_rq, iter, rq) \
	માટે ((શૂન્ય) iter, rt_rq = &rq->rt; rt_rq; rt_rq = ખાલી)

#define for_each_sched_rt_entity(rt_se) \
	માટે (; rt_se; rt_se = ખાલી)

સ્થિર અંતઃરેખ સંરચના rt_rq *group_rt_rq(સંરચના sched_rt_entity *rt_se)
{
	પાછા ખાલી;
}

સ્થિર અંતઃરેખ શૂન્ય sched_rt_rq_enqueue(સંરચના rt_rq *rt_rq)
{
	સંરચના rq *rq = rq_of_rt_rq(rt_rq);

	જો (!rt_rq->rt_nr_running)
		પાછા;

	enqueue_ટોચ_rt_rq(rt_rq);
	resched_curr(rq);
}

સ્થિર અંતઃરેખ શૂન્ય sched_rt_rq_dequeue(સંરચના rt_rq *rt_rq)
{
	dequeue_ટોચ_rt_rq(rt_rq, rt_rq->rt_nr_running);
}

સ્થિર અંતઃરેખ પૂર્ણાંક rt_rq_throttled(સંરચના rt_rq *rt_rq)
{
	પાછા false;
}

સ્થિર અંતઃરેખ અચલ સંરચના cpumask *sched_rt_period_mask(શૂન્ય)
{
	પાછા cpu_online_mask;
}

સ્થિર અંતઃરેખ
સંરચના rt_rq *sched_rt_period_rt_rq(સંરચના rt_bandwidth *rt_b, પૂર્ણાંક cpu)
{
	પાછા &cpu_rq(cpu)->rt;
}

સ્થિર શૂન્ય __enable_runtime(સંરચના rq *rq) { }
સ્થિર શૂન્ય __disable_runtime(સંરચના rq *rq) { }

#endif /* !CONFIG_RT_GROUP_SCHED */

સ્થિર અંતઃરેખ પૂર્ણાંક rt_se_prio(સંરચના sched_rt_entity *rt_se)
{
#ifdef CONFIG_RT_GROUP_SCHED
	સંરચના rt_rq *rt_rq = group_rt_rq(rt_se);

	જો (rt_rq)
		પાછા rt_rq->highest_prio.curr;
#endif

	પાછા rt_task_of(rt_se)->prio;
}

/*
 * Update the current task's runtime statistics. Skip current tasks that
 * are not in our scheduling class.
 */
સ્થિર શૂન્ય update_curr_rt(સંરચના rq *rq)
{
	સંરચના task_struct *donor = rq->donor;
	s64 delta_exec;

	જો (donor->sched_class != &rt_sched_class)
		પાછા;

	delta_exec = update_curr_common(rq);
	જો (unlikely(delta_exec <= 0))
		પાછા;

#ifdef CONFIG_RT_GROUP_SCHED
	સંરચના sched_rt_entity *rt_se = &donor->rt;

	જો (!rt_bandwidth_enabled())
		પાછા;

	for_each_sched_rt_entity(rt_se) {
		સંરચના rt_rq *rt_rq = rt_rq_of_se(rt_se);
		પૂર્ણાંક exceeded;

		જો (sched_rt_runtime(rt_rq) != RUNTIME_INF) {
			raw_spin_lock(&rt_rq->rt_runtime_lock);
			rt_rq->rt_time += delta_exec;
			exceeded = sched_rt_runtime_exceeded(rt_rq);
			જો (exceeded)
				resched_curr(rq);
			raw_spin_unlock(&rt_rq->rt_runtime_lock);
			જો (exceeded)
				do_start_rt_bandwidth(sched_rt_bandwidth(rt_rq));
		}
	}
#endif /* CONFIG_RT_GROUP_SCHED */
}

સ્થિર શૂન્ય
dequeue_ટોચ_rt_rq(સંરચના rt_rq *rt_rq, unsigned પૂર્ણાંક ગણતરી)
{
	સંરચના rq *rq = rq_of_rt_rq(rt_rq);

	BUG_ON(&rq->rt != rt_rq);

	જો (!rt_rq->rt_queued)
		પાછા;

	BUG_ON(!rq->nr_running);

	sub_nr_running(rq, ગણતરી);
	rt_rq->rt_queued = 0;

}

સ્થિર શૂન્ય
enqueue_ટોચ_rt_rq(સંરચના rt_rq *rt_rq)
{
	સંરચના rq *rq = rq_of_rt_rq(rt_rq);

	BUG_ON(&rq->rt != rt_rq);

	જો (rt_rq->rt_queued)
		પાછા;

	જો (rt_rq_throttled(rt_rq))
		પાછા;

	જો (rt_rq->rt_nr_running) {
		add_nr_running(rq, rt_rq->rt_nr_running);
		rt_rq->rt_queued = 1;
	}

	/* Kick cpufreq (see the comment in kernel/sched/sched.h). */
	cpufreq_update_util(rq, 0);
}

સ્થિર શૂન્ય
inc_rt_prio_smp(સંરચના rt_rq *rt_rq, પૂર્ણાંક prio, પૂર્ણાંક prev_prio)
{
	સંરચના rq *rq = rq_of_rt_rq(rt_rq);

	/*
	 * Change rq's cpupri only if rt_rq is the top queue.
	 */
	જો (IS_ENABLED(CONFIG_RT_GROUP_SCHED) && &rq->rt != rt_rq)
		પાછા;

	જો (rq->online && prio < prev_prio)
		cpupri_set(&rq->rd->cpupri, rq->cpu, prio);
}

સ્થિર શૂન્ય
dec_rt_prio_smp(સંરચના rt_rq *rt_rq, પૂર્ણાંક prio, પૂર્ણાંક prev_prio)
{
	સંરચના rq *rq = rq_of_rt_rq(rt_rq);

	/*
	 * Change rq's cpupri only if rt_rq is the top queue.
	 */
	જો (IS_ENABLED(CONFIG_RT_GROUP_SCHED) && &rq->rt != rt_rq)
		પાછા;

	જો (rq->online && rt_rq->highest_prio.curr != prev_prio)
		cpupri_set(&rq->rd->cpupri, rq->cpu, rt_rq->highest_prio.curr);
}

સ્થિર શૂન્ય
inc_rt_prio(સંરચના rt_rq *rt_rq, પૂર્ણાંક prio)
{
	પૂર્ણાંક prev_prio = rt_rq->highest_prio.curr;

	જો (prio < prev_prio)
		rt_rq->highest_prio.curr = prio;

	inc_rt_prio_smp(rt_rq, prio, prev_prio);
}

સ્થિર શૂન્ય
dec_rt_prio(સંરચના rt_rq *rt_rq, પૂર્ણાંક prio)
{
	પૂર્ણાંક prev_prio = rt_rq->highest_prio.curr;

	જો (rt_rq->rt_nr_running) {

		WARN_ON(prio < prev_prio);

		/*
		 * This may have been our highest task, and therefore
		 * we may have some re-computation to do
		 */
		જો (prio == prev_prio) {
			સંરચના rt_prio_array *array = &rt_rq->active;

			rt_rq->highest_prio.curr =
				sched_find_first_bit(array->bitmap);
		}

	} નહીંતર {
		rt_rq->highest_prio.curr = MAX_RT_PRIO-1;
	}

	dec_rt_prio_smp(rt_rq, prio, prev_prio);
}

#ifdef CONFIG_RT_GROUP_SCHED

સ્થિર શૂન્ય
inc_rt_group(સંરચના sched_rt_entity *rt_se, સંરચના rt_rq *rt_rq)
{
	જો (rt_se_boosted(rt_se))
		rt_rq->rt_nr_boosted++;

	start_rt_bandwidth(&rt_rq->tg->rt_bandwidth);
}

સ્થિર શૂન્ય
dec_rt_group(સંરચના sched_rt_entity *rt_se, સંરચના rt_rq *rt_rq)
{
	જો (rt_se_boosted(rt_se))
		rt_rq->rt_nr_boosted--;

	WARN_ON(!rt_rq->rt_nr_running && rt_rq->rt_nr_boosted);
}

#નહીંતર /* !CONFIG_RT_GROUP_SCHED: */

સ્થિર શૂન્ય
inc_rt_group(સંરચના sched_rt_entity *rt_se, સંરચના rt_rq *rt_rq)
{
}

સ્થિર અંતઃરેખ
શૂન્ય dec_rt_group(સંરચના sched_rt_entity *rt_se, સંરચના rt_rq *rt_rq) {}

#endif /* !CONFIG_RT_GROUP_SCHED */

સ્થિર અંતઃરેખ
unsigned પૂર્ણાંક rt_se_nr_running(સંરચના sched_rt_entity *rt_se)
{
	સંરચના rt_rq *group_rq = group_rt_rq(rt_se);

	જો (group_rq)
		પાછા group_rq->rt_nr_running;
	નહીંતર
		પાછા 1;
}

સ્થિર અંતઃરેખ
unsigned પૂર્ણાંક rt_se_rr_nr_running(સંરચના sched_rt_entity *rt_se)
{
	સંરચના rt_rq *group_rq = group_rt_rq(rt_se);
	સંરચના task_struct *tsk;

	જો (group_rq)
		પાછા group_rq->rr_nr_running;

	tsk = rt_task_of(rt_se);

	પાછા (tsk->policy == SCHED_RR) ? 1 : 0;
}

સ્થિર અંતઃરેખ
શૂન્ય inc_rt_tasks(સંરચના sched_rt_entity *rt_se, સંરચના rt_rq *rt_rq)
{
	પૂર્ણાંક prio = rt_se_prio(rt_se);

	WARN_ON(!rt_prio(prio));
	rt_rq->rt_nr_running += rt_se_nr_running(rt_se);
	rt_rq->rr_nr_running += rt_se_rr_nr_running(rt_se);

	inc_rt_prio(rt_rq, prio);
	inc_rt_group(rt_se, rt_rq);
}

સ્થિર અંતઃરેખ
શૂન્ય dec_rt_tasks(સંરચના sched_rt_entity *rt_se, સંરચના rt_rq *rt_rq)
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
સ્થિર અંતઃરેખ bool move_entity(unsigned પૂર્ણાંક flags)
{
	જો ((flags & (DEQUEUE_SAVE | DEQUEUE_MOVE)) == DEQUEUE_SAVE)
		પાછા false;

	પાછા true;
}

સ્થિર શૂન્ય __delist_rt_entity(સંરચના sched_rt_entity *rt_se, સંરચના rt_prio_array *array)
{
	list_del_init(&rt_se->run_list);

	જો (list_empty(array->queue + rt_se_prio(rt_se)))
		__clear_bit(rt_se_prio(rt_se), array->bitmap);

	rt_se->on_list = 0;
}

સ્થિર અંતઃરેખ સંરચના sched_statistics *
__schedstats_from_rt_se(સંરચના sched_rt_entity *rt_se)
{
	/* schedstats is not supported for rt group. */
	જો (!rt_entity_is_task(rt_se))
		પાછા ખાલી;

	પાછા &rt_task_of(rt_se)->stats;
}

સ્થિર અંતઃરેખ શૂન્ય
update_stats_wait_start_rt(સંરચના rt_rq *rt_rq, સંરચના sched_rt_entity *rt_se)
{
	સંરચના sched_statistics *stats;
	સંરચના task_struct *p = ખાલી;

	જો (!schedstat_enabled())
		પાછા;

	જો (rt_entity_is_task(rt_se))
		p = rt_task_of(rt_se);

	stats = __schedstats_from_rt_se(rt_se);
	જો (!stats)
		પાછા;

	__update_stats_wait_start(rq_of_rt_rq(rt_rq), p, stats);
}

સ્થિર અંતઃરેખ શૂન્ય
update_stats_enqueue_sleeper_rt(સંરચના rt_rq *rt_rq, સંરચના sched_rt_entity *rt_se)
{
	સંરચના sched_statistics *stats;
	સંરચના task_struct *p = ખાલી;

	જો (!schedstat_enabled())
		પાછા;

	જો (rt_entity_is_task(rt_se))
		p = rt_task_of(rt_se);

	stats = __schedstats_from_rt_se(rt_se);
	જો (!stats)
		પાછા;

	__update_stats_enqueue_sleeper(rq_of_rt_rq(rt_rq), p, stats);
}

સ્થિર અંતઃરેખ શૂન્ય
update_stats_enqueue_rt(સંરચના rt_rq *rt_rq, સંરચના sched_rt_entity *rt_se,
			પૂર્ણાંક flags)
{
	જો (!schedstat_enabled())
		પાછા;

	જો (flags & ENQUEUE_WAKEUP)
		update_stats_enqueue_sleeper_rt(rt_rq, rt_se);
}

સ્થિર અંતઃરેખ શૂન્ય
update_stats_wait_end_rt(સંરચના rt_rq *rt_rq, સંરચના sched_rt_entity *rt_se)
{
	સંરચના sched_statistics *stats;
	સંરચના task_struct *p = ખાલી;

	જો (!schedstat_enabled())
		પાછા;

	જો (rt_entity_is_task(rt_se))
		p = rt_task_of(rt_se);

	stats = __schedstats_from_rt_se(rt_se);
	જો (!stats)
		પાછા;

	__update_stats_wait_end(rq_of_rt_rq(rt_rq), p, stats);
}

સ્થિર અંતઃરેખ શૂન્ય
update_stats_dequeue_rt(સંરચના rt_rq *rt_rq, સંરચના sched_rt_entity *rt_se,
			પૂર્ણાંક flags)
{
	સંરચના task_struct *p = ખાલી;
	સંરચના rq *rq = rq_of_rt_rq(rt_rq);

	જો (!schedstat_enabled())
		પાછા;

	જો (rt_entity_is_task(rt_se)) {
		p = rt_task_of(rt_se);

		જો (p != rq->curr)
			update_stats_wait_end_rt(rt_rq, rt_se);
	}

	જો ((flags & DEQUEUE_SLEEP) && p) {
		unsigned પૂર્ણાંક state;

		state = READ_ONCE(p->__state);
		જો (state & TASK_INTERRUPTIBLE)
			__schedstat_set(p->stats.sleep_start,
					rq_clock(rq_of_rt_rq(rt_rq)));

		જો (state & TASK_UNINTERRUPTIBLE)
			__schedstat_set(p->stats.block_start,
					rq_clock(rq_of_rt_rq(rt_rq)));
	}
}

સ્થિર શૂન્ય __enqueue_rt_entity(સંરચના sched_rt_entity *rt_se, unsigned પૂર્ણાંક flags)
{
	સંરચના rt_rq *rt_rq = rt_rq_of_se(rt_se);
	સંરચના rt_prio_array *array = &rt_rq->active;
	સંરચના rt_rq *group_rq = group_rt_rq(rt_se);
	સંરચના list_head *queue = array->queue + rt_se_prio(rt_se);

	/*
	 * Don't enqueue the group if its throttled, or when empty.
	 * The latter is a consequence of the former when a child group
	 * get throttled and the current group doesn't have any other
	 * active members.
	 */
	જો (group_rq && (rt_rq_throttled(group_rq) || !group_rq->rt_nr_running)) {
		જો (rt_se->on_list)
			__delist_rt_entity(rt_se, array);
		પાછા;
	}

	જો (move_entity(flags)) {
		WARN_ON_ONCE(rt_se->on_list);
		જો (flags & ENQUEUE_HEAD)
			list_add(&rt_se->run_list, queue);
		નહીંતર
			list_add_tail(&rt_se->run_list, queue);

		__set_bit(rt_se_prio(rt_se), array->bitmap);
		rt_se->on_list = 1;
	}
	rt_se->on_rq = 1;

	inc_rt_tasks(rt_se, rt_rq);
}

સ્થિર શૂન્ય __dequeue_rt_entity(સંરચના sched_rt_entity *rt_se, unsigned પૂર્ણાંક flags)
{
	સંરચના rt_rq *rt_rq = rt_rq_of_se(rt_se);
	સંરચના rt_prio_array *array = &rt_rq->active;

	જો (move_entity(flags)) {
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
સ્થિર શૂન્ય dequeue_rt_ઢગલો(સંરચના sched_rt_entity *rt_se, unsigned પૂર્ણાંક flags)
{
	સંરચના sched_rt_entity *back = ખાલી;
	unsigned પૂર્ણાંક rt_nr_running;

	for_each_sched_rt_entity(rt_se) {
		rt_se->back = back;
		back = rt_se;
	}

	rt_nr_running = rt_rq_of_se(back)->rt_nr_running;

	માટે (rt_se = back; rt_se; rt_se = rt_se->back) {
		જો (on_rt_rq(rt_se))
			__dequeue_rt_entity(rt_se, flags);
	}

	dequeue_ટોચ_rt_rq(rt_rq_of_se(back), rt_nr_running);
}

સ્થિર શૂન્ય enqueue_rt_entity(સંરચના sched_rt_entity *rt_se, unsigned પૂર્ણાંક flags)
{
	સંરચના rq *rq = rq_of_rt_se(rt_se);

	update_stats_enqueue_rt(rt_rq_of_se(rt_se), rt_se, flags);

	dequeue_rt_ઢગલો(rt_se, flags);
	for_each_sched_rt_entity(rt_se)
		__enqueue_rt_entity(rt_se, flags);
	enqueue_ટોચ_rt_rq(&rq->rt);
}

સ્થિર શૂન્ય dequeue_rt_entity(સંરચના sched_rt_entity *rt_se, unsigned પૂર્ણાંક flags)
{
	સંરચના rq *rq = rq_of_rt_se(rt_se);

	update_stats_dequeue_rt(rt_rq_of_se(rt_se), rt_se, flags);

	dequeue_rt_ઢગલો(rt_se, flags);

	for_each_sched_rt_entity(rt_se) {
		સંરચના rt_rq *rt_rq = group_rt_rq(rt_se);

		જો (rt_rq && rt_rq->rt_nr_running)
			__enqueue_rt_entity(rt_se, flags);
	}
	enqueue_ટોચ_rt_rq(&rq->rt);
}

/*
 * Adding/removing a task to/from a priority array:
 */
સ્થિર શૂન્ય
enqueue_task_rt(સંરચના rq *rq, સંરચના task_struct *p, પૂર્ણાંક flags)
{
	સંરચના sched_rt_entity *rt_se = &p->rt;

	જો (flags & ENQUEUE_WAKEUP)
		rt_se->timeout = 0;

	check_schedstat_required();
	update_stats_wait_start_rt(rt_rq_of_se(rt_se), rt_se);

	enqueue_rt_entity(rt_se, flags);

	જો (task_is_blocked(p))
		પાછા;

	જો (!task_current(rq, p) && p->nr_cpus_allowed > 1)
		enqueue_pushable_task(rq, p);
}

સ્થિર bool dequeue_task_rt(સંરચના rq *rq, સંરચના task_struct *p, પૂર્ણાંક flags)
{
	સંરચના sched_rt_entity *rt_se = &p->rt;

	update_curr_rt(rq);
	dequeue_rt_entity(rt_se, flags);

	dequeue_pushable_task(rq, p);

	પાછા true;
}

/*
 * Put task to the head or the end of the run list without the overhead of
 * dequeue followed by enqueue.
 */
સ્થિર શૂન્ય
requeue_rt_entity(સંરચના rt_rq *rt_rq, સંરચના sched_rt_entity *rt_se, પૂર્ણાંક head)
{
	જો (on_rt_rq(rt_se)) {
		સંરચના rt_prio_array *array = &rt_rq->active;
		સંરચના list_head *queue = array->queue + rt_se_prio(rt_se);

		જો (head)
			list_move(&rt_se->run_list, queue);
		નહીંતર
			list_move_tail(&rt_se->run_list, queue);
	}
}

સ્થિર શૂન્ય requeue_task_rt(સંરચના rq *rq, સંરચના task_struct *p, પૂર્ણાંક head)
{
	સંરચના sched_rt_entity *rt_se = &p->rt;
	સંરચના rt_rq *rt_rq;

	for_each_sched_rt_entity(rt_se) {
		rt_rq = rt_rq_of_se(rt_se);
		requeue_rt_entity(rt_rq, rt_se, head);
	}
}

સ્થિર શૂન્ય yield_task_rt(સંરચના rq *rq)
{
	requeue_task_rt(rq, rq->donor, 0);
}

સ્થિર પૂર્ણાંક find_lowest_rq(સંરચના task_struct *task);

સ્થિર પૂર્ણાંક
select_task_rq_rt(સંરચના task_struct *p, પૂર્ણાંક cpu, પૂર્ણાંક flags)
{
	સંરચના task_struct *curr, *donor;
	સંરચના rq *rq;
	bool test;

	/* For anything but wake ups, just return the task_cpu */
	જો (!(flags & (WF_TTWU | WF_FORK)))
		જાઓ out;

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

	જો (test || !rt_task_fits_capacity(p, cpu)) {
		પૂર્ણાંક target = find_lowest_rq(p);

		/*
		 * Bail out if we were forcing a migration to find a better
		 * fitting CPU but our search failed.
		 */
		જો (!test && target != -1 && !rt_task_fits_capacity(p, target))
			જાઓ out_unlock;

		/*
		 * Don't bother moving it if the destination CPU is
		 * not running a lower priority task.
		 */
		જો (target != -1 &&
		    p->prio < cpu_rq(target)->rt.highest_prio.curr)
			cpu = target;
	}

out_unlock:
	rcu_read_unlock();

out:
	પાછા cpu;
}

સ્થિર શૂન્ય check_preempt_equal_prio(સંરચના rq *rq, સંરચના task_struct *p)
{
	જો (rq->curr->nr_cpus_allowed == 1 ||
	    !cpupri_find(&rq->rd->cpupri, rq->donor, ખાલી))
		પાછા;

	/*
	 * p is migratable, so let's not schedule it and
	 * see if it is pushed or pulled somewhere else.
	 */
	જો (p->nr_cpus_allowed != 1 &&
	    cpupri_find(&rq->rd->cpupri, p, ખાલી))
		પાછા;

	/*
	 * There appear to be other CPUs that can accept
	 * the current task but none can run 'p', so lets reschedule
	 * to try and push the current task away:
	 */
	requeue_task_rt(rq, p, 1);
	resched_curr(rq);
}

સ્થિર પૂર્ણાંક balance_rt(સંરચના rq *rq, સંરચના rq_flags *rf)
{
	/*
	 * Note, rq->donor may change during rq lock drops,
	 * so don't re-use p across lock drops
	 */
	સંરચના task_struct *p = rq->donor;

	જો (!on_rt_rq(&p->rt) && need_pull_rt_task(rq, p)) {
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

	પાછા sched_stop_runnable(rq) || sched_dl_runnable(rq) || sched_rt_runnable(rq);
}

/*
 * Preempt the current task with a newly woken task if needed:
 */
સ્થિર શૂન્ય wakeup_preempt_rt(સંરચના rq *rq, સંરચના task_struct *p, પૂર્ણાંક flags)
{
	સંરચના task_struct *donor = rq->donor;

	/*
	 * XXX If we're preempted by DL, queue a push?
	 */
	જો (p->sched_class != &rt_sched_class ||
	    donor->sched_class != &rt_sched_class)
		પાછા;

	જો (p->prio < donor->prio) {
		resched_curr(rq);
		પાછા;
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
	જો (p->prio == donor->prio && !test_tsk_need_resched(rq->curr))
		check_preempt_equal_prio(rq, p);
}

સ્થિર અંતઃરેખ શૂન્ય set_next_task_rt(સંરચના rq *rq, સંરચના task_struct *p, ગણના snt_e type)
{
	સંરચના sched_rt_entity *rt_se = &p->rt;
	સંરચના rt_rq *rt_rq = &rq->rt;

	જો (type == SNT_REPICK)
		પાછા;

	p->se.exec_start = rq_clock_task(rq);
	જો (on_rt_rq(&p->rt))
		update_stats_wait_end_rt(rt_rq, rt_se);

	/* The running task is never eligible for pushing */
	dequeue_pushable_task(rq, p);

	જો (type != SNT_PICK)
		પાછા;

	/*
	 * If prev task was rt, put_prev_task() has already updated the
	 * utilization. We only care of the case where we start to schedule a
	 * rt task
	 */
	જો (rq->donor->sched_class != &rt_sched_class)
		update_rt_rq_load_avg(rq_clock_pelt(rq), rq, 0);

	rt_queue_ધકેલો_tasks(rq);
}

સ્થિર સંરચના sched_rt_entity *pick_next_rt_entity(સંરચના rt_rq *rt_rq)
{
	સંરચના rt_prio_array *array = &rt_rq->active;
	સંરચના sched_rt_entity *next = ખાલી;
	સંરચના list_head *queue;
	પૂર્ણાંક idx;

	idx = sched_find_first_bit(array->bitmap);
	BUG_ON(idx >= MAX_RT_PRIO);

	queue = array->queue + idx;
	જો (WARN_ON_ONCE(list_empty(queue)))
		પાછા ખાલી;
	next = list_entry(queue->next, સંરચના sched_rt_entity, run_list);

	પાછા next;
}

સ્થિર સંરચના task_struct *_pick_next_task_rt(સંરચના rq *rq)
{
	સંરચના sched_rt_entity *rt_se;
	સંરચના rt_rq *rt_rq  = &rq->rt;

	કરો {
		rt_se = pick_next_rt_entity(rt_rq);
		જો (unlikely(!rt_se))
			પાછા ખાલી;
		rt_rq = group_rt_rq(rt_se);
	} જ્યારે (rt_rq);

	પાછા rt_task_of(rt_se);
}

સ્થિર સંરચના task_struct *pick_task_rt(સંરચના rq *rq, સંરચના rq_flags *rf)
{
	સંરચના task_struct *p;

	જો (!sched_rt_runnable(rq))
		પાછા ખાલી;

	p = _pick_next_task_rt(rq);

	પાછા p;
}

સ્થિર શૂન્ય put_prev_task_rt(સંરચના rq *rq, સંરચના task_struct *p, સંરચના task_struct *next)
{
	સંરચના sched_rt_entity *rt_se = &p->rt;
	સંરચના rt_rq *rt_rq = &rq->rt;

	જો (on_rt_rq(&p->rt))
		update_stats_wait_start_rt(rt_rq, rt_se);

	update_curr_rt(rq);

	update_rt_rq_load_avg(rq_clock_pelt(rq), rq, 1);

	જો (task_is_blocked(p))
		પાછા;
	/*
	 * The previous task needs to be made eligible for pushing
	 * if it is still active
	 */
	જો (on_rt_rq(&p->rt) && p->nr_cpus_allowed > 1)
		enqueue_pushable_task(rq, p);
}

/* Only try algorithms three times */
#define RT_MAX_TRIES 3

/*
 * Return the highest pushable rq's task, which is suitable to be executed
 * on the CPU, NULL otherwise
 */
સ્થિર સંરચના task_struct *pick_highest_pushable_task(સંરચના rq *rq, પૂર્ણાંક cpu)
{
	સંરચના plist_head *head = &rq->rt.pushable_tasks;
	સંરચના task_struct *p;

	જો (!has_pushable_tasks(rq))
		પાછા ખાલી;

	plist_for_each_entry(p, head, pushable_tasks) {
		જો (task_is_pushable(rq, p, cpu))
			પાછા p;
	}

	પાછા ખાલી;
}

સ્થિર DEFINE_PER_CPU(cpumask_var_t, local_cpu_mask);

સ્થિર પૂર્ણાંક find_lowest_rq(સંરચના task_struct *task)
{
	સંરચના sched_domain *sd;
	સંરચના cpumask *lowest_mask = this_cpu_cpumask_var_ptr(local_cpu_mask);
	પૂર્ણાંક this_cpu = smp_processor_id();
	પૂર્ણાંક cpu      = task_cpu(task);
	પૂર્ણાંક ret;

	/* Make sure the mask is initialized first */
	જો (unlikely(!lowest_mask))
		પાછા -1;

	જો (task->nr_cpus_allowed == 1)
		પાછા -1; /* No other targets possible */

	/*
	 * If we're on asym system ensure we consider the different capacities
	 * of the CPUs when searching for the lowest_mask.
	 */
	જો (sched_asym_cpucap_active()) {

		ret = cpupri_find_fitness(&task_rq(task)->rd->cpupri,
					  task, lowest_mask,
					  rt_task_fits_capacity);
	} નહીંતર {

		ret = cpupri_find(&task_rq(task)->rd->cpupri,
				  task, lowest_mask);
	}

	જો (!ret)
		પાછા -1; /* No targets found */

	/*
	 * At this point we have built a mask of CPUs representing the
	 * lowest priority tasks in the system.  Now we want to elect
	 * the best one based on our affinity and topology.
	 *
	 * We prioritize the last CPU that the task executed on since
	 * it is most likely cache-hot in that location.
	 */
	જો (cpumask_test_cpu(cpu, lowest_mask))
		પાછા cpu;

	/*
	 * Otherwise, we consult the sched_domains span maps to figure
	 * out which CPU is logically closest to our hot cache data.
	 */
	જો (!cpumask_test_cpu(this_cpu, lowest_mask))
		this_cpu = -1; /* Skip this_cpu opt if not among lowest */

	rcu_read_lock();
	for_each_domain(cpu, sd) {
		જો (sd->flags & SD_WAKE_AFFINE) {
			પૂર્ણાંક best_cpu;

			/*
			 * "this_cpu" is cheaper to preempt than a
			 * remote processor.
			 */
			જો (this_cpu != -1 &&
			    cpumask_test_cpu(this_cpu, sched_domain_span(sd))) {
				rcu_read_unlock();
				પાછા this_cpu;
			}

			best_cpu = cpumask_any_and_distribute(lowest_mask,
							      sched_domain_span(sd));
			જો (best_cpu < nr_cpu_ids) {
				rcu_read_unlock();
				પાછા best_cpu;
			}
		}
	}
	rcu_read_unlock();

	/*
	 * And finally, if there were no matches within the domains
	 * just give the caller *something* to work with from the compatible
	 * locations.
	 */
	જો (this_cpu != -1)
		પાછા this_cpu;

	cpu = cpumask_any_distribute(lowest_mask);
	જો (cpu < nr_cpu_ids)
		પાછા cpu;

	પાછા -1;
}

સ્થિર સંરચના task_struct *pick_next_pushable_task(સંરચના rq *rq)
{
	સંરચના plist_head *head = &rq->rt.pushable_tasks;
	સંરચના task_struct *i, *p = ખાલી;

	જો (!has_pushable_tasks(rq))
		પાછા ખાલી;

	plist_for_each_entry(i, head, pushable_tasks) {
		/* skip tasks that cannot be migrated */
		જો (!task_on_cpu(rq, i) && !is_migration_disabled(i)) {
			p = i;
			તોડો;
		}
	}

	જો (!p)
		પાછા ખાલી;

	BUG_ON(rq->cpu != task_cpu(p));
	BUG_ON(task_current(rq, p));
	BUG_ON(task_current_donor(rq, p));
	BUG_ON(p->nr_cpus_allowed <= 1);

	BUG_ON(!task_on_rq_queued(p));
	BUG_ON(!rt_task(p));

	પાછા p;
}

/* Will lock the rq it finds */
સ્થિર સંરચના rq *find_lock_lowest_rq(સંરચના task_struct *task, સંરચના rq *rq)
{
	સંરચના rq *lowest_rq = ખાલી;
	પૂર્ણાંક tries;
	પૂર્ણાંક cpu;

	માટે (tries = 0; tries < RT_MAX_TRIES; tries++) {
		cpu = find_lowest_rq(task);

		જો ((cpu == -1) || (cpu == rq->cpu))
			તોડો;

		lowest_rq = cpu_rq(cpu);

		જો (lowest_rq->rt.highest_prio.curr <= task->prio) {
			/*
			 * Target rq has tasks of equal or higher priority,
			 * retrying does not release any lock and is unlikely
			 * to yield a different result.
			 */
			lowest_rq = ખાલી;
			તોડો;
		}

		/* if the prio of this runqueue changed, try again */
		જો (double_lock_balance(rq, lowest_rq)) {
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
			જો (unlikely(is_migration_disabled(task) ||
				     !cpumask_test_cpu(lowest_rq->cpu, &task->cpus_mask) ||
				     task != pick_next_pushable_task(rq))) {

				double_unlock_balance(rq, lowest_rq);
				lowest_rq = ખાલી;
				તોડો;
			}
		}

		/* If this rq is still suitable use it. */
		જો (lowest_rq->rt.highest_prio.curr > task->prio)
			તોડો;

		/* try again */
		double_unlock_balance(rq, lowest_rq);
		lowest_rq = ખાલી;
	}

	પાછા lowest_rq;
}

/*
 * If the current CPU has more than one RT task, see if the non
 * running task can migrate over to a CPU that is running a task
 * of lesser priority.
 */
સ્થિર પૂર્ણાંક ધકેલો_rt_task(સંરચના rq *rq, bool pull)
{
	સંરચના task_struct *next_task;
	સંરચના rq *lowest_rq;
	પૂર્ણાંક ret = 0;

	જો (!rq->rt.overloaded)
		પાછા 0;

	next_task = pick_next_pushable_task(rq);
	જો (!next_task)
		પાછા 0;

retry:
	/*
	 * It's possible that the next_task slipped in of
	 * higher priority than current. If that's the case
	 * just reschedule current.
	 */
	જો (unlikely(next_task->prio < rq->donor->prio)) {
		resched_curr(rq);
		પાછા 0;
	}

	જો (is_migration_disabled(next_task)) {
		સંરચના task_struct *ધકેલો_task = ખાલી;
		પૂર્ણાંક cpu;

		જો (!pull || rq->ધકેલો_busy)
			પાછા 0;

		/*
		 * Invoking find_lowest_rq() on anything but an RT task doesn't
		 * make sense. Per the above priority check, curr has to
		 * be of higher priority than next_task, so no need to
		 * reschedule when bailing out.
		 *
		 * Note that the stoppers are masqueraded as SCHED_FIFO
		 * (cf. sched_set_stop_task()), so we can't rely on rt_task().
		 */
		જો (rq->donor->sched_class != &rt_sched_class)
			પાછા 0;

		cpu = find_lowest_rq(rq->curr);
		જો (cpu == -1 || cpu == rq->cpu)
			પાછા 0;

		/*
		 * Given we found a CPU with lower priority than @next_task,
		 * therefore it should be running. However we cannot migrate it
		 * to this other CPU, instead attempt to push the current
		 * running task on this CPU away.
		 */
		ધકેલો_task = get_ધકેલો_task(rq);
		જો (ધકેલો_task) {
			preempt_disable();
			raw_spin_rq_unlock(rq);
			stop_one_cpu_nowait(rq->cpu, ધકેલો_cpu_stop,
					    ધકેલો_task, &rq->ધકેલો_work);
			preempt_enable();
			raw_spin_rq_lock(rq);
		}

		પાછા 0;
	}

	જો (WARN_ON(next_task == rq->curr))
		પાછા 0;

	/* We might release rq lock */
	get_task_struct(next_task);

	/* find_lock_lowest_rq locks the rq if found */
	lowest_rq = find_lock_lowest_rq(next_task, rq);
	જો (!lowest_rq) {
		સંરચના task_struct *task;
		/*
		 * find_lock_lowest_rq releases rq->lock
		 * so it is possible that next_task has migrated.
		 *
		 * We need to make sure that the task is still on the same
		 * run-queue and is also still the next task eligible for
		 * pushing.
		 */
		task = pick_next_pushable_task(rq);
		જો (task == next_task) {
			/*
			 * The task hasn't migrated, and is still the next
			 * eligible task, but we failed to find a run-queue
			 * to push it to.  Do not retry in this case, since
			 * other CPUs will pull from us when ready.
			 */
			જાઓ out;
		}

		જો (!task)
			/* No more tasks, just exit */
			જાઓ out;

		/*
		 * Something has shifted, try again.
		 */
		put_task_struct(next_task);
		next_task = task;
		જાઓ retry;
	}

	move_queued_task_locked(rq, lowest_rq, next_task);
	resched_curr(lowest_rq);
	ret = 1;

	double_unlock_balance(rq, lowest_rq);
out:
	put_task_struct(next_task);

	પાછા ret;
}

સ્થિર શૂન્ય ધકેલો_rt_tasks(સંરચના rq *rq)
{
	/* push_rt_task will return true if it moved an RT */
	જ્યારે (ધકેલો_rt_task(rq, false))
		;
}

#ifdef HAVE_RT_ધકેલો_IPI

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
સ્થિર પૂર્ણાંક rto_next_cpu(સંરચના મૂળ_domain *rd)
{
	પૂર્ણાંક this_cpu = smp_processor_id();
	પૂર્ણાંક next;
	પૂર્ણાંક cpu;

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
	માટે (;;) {

		/* When rto_cpu is -1 this acts like cpumask_first() */
		cpu = cpumask_next(rd->rto_cpu, rd->rto_mask);

		rd->rto_cpu = cpu;

		/* Do not send IPI to self */
		જો (cpu == this_cpu)
			ચાલુ;

		જો (cpu < nr_cpu_ids)
			પાછા cpu;

		rd->rto_cpu = -1;

		/*
		 * ACQUIRE ensures we see the @rto_mask changes
		 * made prior to the @next value observed.
		 *
		 * Matches WMB in rt_set_overload().
		 */
		next = atomic_read_acquire(&rd->rto_loop_next);

		જો (rd->rto_loop == next)
			તોડો;

		rd->rto_loop = next;
	}

	પાછા -1;
}

સ્થિર અંતઃરેખ bool rto_start_trylock(atomic_t *v)
{
	પાછા !atomic_cmpxchg_acquire(v, 0, 1);
}

સ્થિર અંતઃરેખ શૂન્ય rto_start_unlock(atomic_t *v)
{
	atomic_set_release(v, 0);
}

સ્થિર શૂન્ય tell_cpu_to_ધકેલો(સંરચના rq *rq)
{
	પૂર્ણાંક cpu = -1;

	/* Keep the loop going if the IPI is currently active */
	atomic_inc(&rq->rd->rto_loop_next);

	/* Only one CPU can initiate a loop at a time */
	જો (!rto_start_trylock(&rq->rd->rto_loop_start))
		પાછા;

	raw_spin_lock(&rq->rd->rto_lock);

	/*
	 * The rto_cpu is updated under the lock, if it has a valid CPU
	 * then the IPI is still running and will continue due to the
	 * update to loop_next, and nothing needs to be done here.
	 * Otherwise it is finishing up and an IPI needs to be sent.
	 */
	જો (rq->rd->rto_cpu < 0)
		cpu = rto_next_cpu(rq->rd);

	raw_spin_unlock(&rq->rd->rto_lock);

	rto_start_unlock(&rq->rd->rto_loop_start);

	જો (cpu >= 0) {
		/* Make sure the rd does not get freed while pushing */
		sched_get_rd(rq->rd);
		irq_work_queue_on(&rq->rd->rto_ધકેલો_work, cpu);
	}
}

/* Called from hardirq context */
શૂન્ય rto_ધકેલો_irq_work_func(સંરચના irq_work *work)
{
	સંરચના મૂળ_domain *rd =
		container_of(work, સંરચના મૂળ_domain, rto_ધકેલો_work);
	સંરચના rq *rq;
	પૂર્ણાંક cpu;

	rq = this_rq();

	/*
	 * We do not need to grab the lock to check for has_pushable_tasks.
	 * When it gets updated, a check is made if a push is possible.
	 */
	જો (has_pushable_tasks(rq)) {
		raw_spin_rq_lock(rq);
		જ્યારે (ધકેલો_rt_task(rq, true))
			;
		raw_spin_rq_unlock(rq);
	}

	raw_spin_lock(&rd->rto_lock);

	/* Pass the IPI to the next rt overloaded queue */
	cpu = rto_next_cpu(rd);

	raw_spin_unlock(&rd->rto_lock);

	જો (cpu < 0) {
		sched_put_rd(rd);
		પાછા;
	}

	/* Try the next RT overloaded CPU */
	irq_work_queue_on(&rd->rto_ધકેલો_work, cpu);
}
#endif /* HAVE_RT_PUSH_IPI */

સ્થિર શૂન્ય pull_rt_task(સંરચના rq *this_rq)
{
	પૂર્ણાંક this_cpu = this_rq->cpu, cpu;
	bool resched = false;
	સંરચના task_struct *p, *ધકેલો_task;
	સંરચના rq *src_rq;
	પૂર્ણાંક rt_overload_ગણતરી = rt_overloaded(this_rq);

	જો (likely(!rt_overload_ગણતરી))
		પાછા;

	/*
	 * Match the barrier from rt_set_overloaded; this guarantees that if we
	 * see overloaded we must also see the rto_mask bit.
	 */
	smp_rmb();

	/* If we are the only overloaded CPU do nothing */
	જો (rt_overload_ગણતરી == 1 &&
	    cpumask_test_cpu(this_rq->cpu, this_rq->rd->rto_mask))
		પાછા;

#ifdef HAVE_RT_ધકેલો_IPI
	જો (sched_feat(RT_ધકેલો_IPI)) {
		tell_cpu_to_ધકેલો(this_rq);
		પાછા;
	}
#endif

	for_each_cpu(cpu, this_rq->rd->rto_mask) {
		જો (this_cpu == cpu)
			ચાલુ;

		src_rq = cpu_rq(cpu);

		/*
		 * Don't bother taking the src_rq->lock if the next highest
		 * task is known to be lower-priority than our current task.
		 * This may look racy, but if this value is about to go
		 * logically higher, the src_rq will push this task away.
		 * And if its going logically lower, we do not care
		 */
		જો (src_rq->rt.highest_prio.next >=
		    this_rq->rt.highest_prio.curr)
			ચાલુ;

		/*
		 * We can potentially drop this_rq's lock in
		 * double_lock_balance, and another CPU could
		 * alter this_rq
		 */
		ધકેલો_task = ખાલી;
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
		જો (p && (p->prio < this_rq->rt.highest_prio.curr)) {
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
			જો (p->prio < src_rq->donor->prio)
				જાઓ skip;

			જો (is_migration_disabled(p)) {
				ધકેલો_task = get_ધકેલો_task(src_rq);
			} નહીંતર {
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

		જો (ધકેલો_task) {
			preempt_disable();
			raw_spin_rq_unlock(this_rq);
			stop_one_cpu_nowait(src_rq->cpu, ધકેલો_cpu_stop,
					    ધકેલો_task, &src_rq->ધકેલો_work);
			preempt_enable();
			raw_spin_rq_lock(this_rq);
		}
	}

	જો (resched)
		resched_curr(this_rq);
}

/*
 * If we are not running and we are not going to reschedule soon, we should
 * try to push tasks away now
 */
સ્થિર શૂન્ય task_woken_rt(સંરચના rq *rq, સંરચના task_struct *p)
{
	bool need_to_ધકેલો = !task_on_cpu(rq, p) &&
			    !test_tsk_need_resched(rq->curr) &&
			    p->nr_cpus_allowed > 1 &&
			    (dl_task(rq->donor) || rt_task(rq->donor)) &&
			    (rq->curr->nr_cpus_allowed < 2 ||
			     rq->donor->prio <= p->prio);

	જો (need_to_ધકેલો)
		ધકેલો_rt_tasks(rq);
}

/* Assumes rq->lock is held */
સ્થિર શૂન્ય rq_online_rt(સંરચના rq *rq)
{
	જો (rq->rt.overloaded)
		rt_set_overload(rq);

	__enable_runtime(rq);

	cpupri_set(&rq->rd->cpupri, rq->cpu, rq->rt.highest_prio.curr);
}

/* Assumes rq->lock is held */
સ્થિર શૂન્ય rq_offline_rt(સંરચના rq *rq)
{
	જો (rq->rt.overloaded)
		rt_clear_overload(rq);

	__disable_runtime(rq);

	cpupri_set(&rq->rd->cpupri, rq->cpu, CPUPRI_INVALID);
}

/*
 * When switch from the rt queue, we bring ourselves to a position
 * that we might want to pull RT tasks from other runqueues.
 */
સ્થિર શૂન્ય switched_from_rt(સંરચના rq *rq, સંરચના task_struct *p)
{
	/*
	 * If there are other RT tasks then we will reschedule
	 * and the scheduling of the other RT tasks will handle
	 * the balancing. But if we are the last RT task
	 * we may need to handle the pulling of RT tasks
	 * now.
	 */
	જો (!task_on_rq_queued(p) || rq->rt.rt_nr_running)
		પાછા;

	rt_queue_pull_task(rq);
}

શૂન્ય __init init_sched_rt_class(શૂન્ય)
{
	unsigned પૂર્ણાંક i;

	for_each_possible_cpu(i) {
		zalloc_cpumask_var_ગાંઠ(&per_cpu(local_cpu_mask, i),
					GFP_KERNEL, cpu_to_ગાંઠ(i));
	}
}

/*
 * When switching a task to RT, we may overload the runqueue
 * with RT tasks. In this case we try to push them off to
 * other runqueues.
 */
સ્થિર શૂન્ય switched_to_rt(સંરચના rq *rq, સંરચના task_struct *p)
{
	/*
	 * If we are running, update the avg_rt tracking, as the running time
	 * will now on be accounted into the latter.
	 */
	જો (task_current(rq, p)) {
		update_rt_rq_load_avg(rq_clock_pelt(rq), rq, 0);
		પાછા;
	}

	/*
	 * If we are not running we may need to preempt the current
	 * running task. If that current running task is also an RT task
	 * then see if we can move to another run queue.
	 */
	જો (task_on_rq_queued(p)) {
		જો (p->nr_cpus_allowed > 1 && rq->rt.overloaded)
			rt_queue_ધકેલો_tasks(rq);
		જો (p->prio < rq->donor->prio && cpu_online(cpu_of(rq)))
			resched_curr(rq);
	}
}

/*
 * Priority of the task has changed. This may cause
 * us to initiate a push or pull.
 */
સ્થિર શૂન્ય
prio_changed_rt(સંરચના rq *rq, સંરચના task_struct *p, u64 oldprio)
{
	જો (!task_on_rq_queued(p))
		પાછા;

	જો (p->prio == oldprio)
		પાછા;

	જો (task_current_donor(rq, p)) {
		/*
		 * If our priority decreases while running, we
		 * may need to pull tasks to this runqueue.
		 */
		જો (oldprio < p->prio)
			rt_queue_pull_task(rq);

		/*
		 * If there's a higher priority task waiting to run
		 * then reschedule.
		 */
		જો (p->prio > rq->rt.highest_prio.curr)
			resched_curr(rq);
	} નહીંતર {
		/*
		 * This task is not running, but if it is
		 * greater than the current running task
		 * then reschedule.
		 */
		જો (p->prio < rq->donor->prio)
			resched_curr(rq);
	}
}

#ifdef CONFIG_POSIX_TIMERS
સ્થિર શૂન્ય watchdog(સંરચના rq *rq, સંરચના task_struct *p)
{
	unsigned long soft, hard;

	/* max may change after cur was read, this will be fixed next tick */
	soft = task_rlimit(p, RLIMIT_RTTIME);
	hard = task_rlimit_max(p, RLIMIT_RTTIME);

	જો (soft != RLIM_INFINITY) {
		unsigned long next;

		જો (p->rt.watchdog_stamp != jiffies) {
			p->rt.timeout++;
			p->rt.watchdog_stamp = jiffies;
		}

		next = DIV_ROUND_UP(min(soft, hard), USEC_PER_SEC/HZ);
		જો (p->rt.timeout > next) {
			posix_cputimers_rt_watchdog(&p->posix_cputimers,
						    p->se.sum_exec_runtime);
		}
	}
}
#નહીંતર /* !CONFIG_POSIX_TIMERS: */
સ્થિર અંતઃરેખ શૂન્ય watchdog(સંરચના rq *rq, સંરચના task_struct *p) { }
#endif /* !CONFIG_POSIX_TIMERS */

/*
 * scheduler tick hitting a task of our scheduling class.
 *
 * NOTE: This function can be called remotely by the tick offload that
 * goes along full dynticks. Therefore no local assumption can be made
 * and everything must be accessed through the @rq and @curr passed in
 * parameters.
 */
સ્થિર શૂન્ય task_tick_rt(સંરચના rq *rq, સંરચના task_struct *p, પૂર્ણાંક queued)
{
	સંરચના sched_rt_entity *rt_se = &p->rt;

	update_curr_rt(rq);
	update_rt_rq_load_avg(rq_clock_pelt(rq), rq, 1);

	watchdog(rq, p);

	/*
	 * RR tasks need a special form of time-slice management.
	 * FIFO tasks have no timeslices.
	 */
	જો (p->policy != SCHED_RR)
		પાછા;

	જો (--p->rt.time_slice)
		પાછા;

	p->rt.time_slice = sched_rr_timeslice;

	/*
	 * Requeue to the end of queue if we (and all of our ancestors) are not
	 * the only element on the queue
	 */
	for_each_sched_rt_entity(rt_se) {
		જો (rt_se->run_list.prev != rt_se->run_list.next) {
			requeue_task_rt(rq, p, 0);
			resched_curr(rq);
			પાછા;
		}
	}
}

સ્થિર unsigned પૂર્ણાંક get_rr_interval_rt(સંરચના rq *rq, સંરચના task_struct *task)
{
	/*
	 * Time slice is 0 for SCHED_FIFO tasks
	 */
	જો (task->policy == SCHED_RR)
		પાછા sched_rr_timeslice;
	નહીંતર
		પાછા 0;
}

#ifdef CONFIG_SCHED_CORE
સ્થિર પૂર્ણાંક task_is_throttled_rt(સંરચના task_struct *p, પૂર્ણાંક cpu)
{
	સંરચના rt_rq *rt_rq;

#ifdef CONFIG_RT_GROUP_SCHED // XXX maybe add task_rt_rq(), see also sched_rt_period_rt_rq
	rt_rq = task_group(p)->rt_rq[cpu];
	WARN_ON(!rt_group_sched_enabled() && rt_rq->tg != &મૂળ_task_group);
#નહીંતર
	rt_rq = &cpu_rq(cpu)->rt;
#endif

	પાછા rt_rq_throttled(rt_rq);
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
સ્થિર DEFINE_MUTEX(rt_constraints_mutex);

સ્થિર અંતઃરેખ પૂર્ણાંક tg_has_rt_tasks(સંરચના task_group *tg)
{
	સંરચના task_struct *task;
	સંરચના css_task_iter it;
	પૂર્ણાંક ret = 0;

	/*
	 * Autogroups do not have RT tasks; see autogroup_create().
	 */
	જો (task_group_is_autogroup(tg))
		પાછા 0;

	css_task_iter_start(&tg->css, 0, &it);
	જ્યારે (!ret && (task = css_task_iter_next(&it)))
		ret |= rt_task(task);
	css_task_iter_end(&it);

	પાછા ret;
}

સંરચના rt_schedulable_data {
	સંરચના task_group *tg;
	u64 rt_period;
	u64 rt_runtime;
};

સ્થિર પૂર્ણાંક tg_rt_schedulable(સંરચના task_group *tg, શૂન્ય *data)
{
	સંરચના rt_schedulable_data *d = data;
	સંરચના task_group *child;
	u64 total, sum = 0;
	u64 period, runtime;

	period = ktime_to_ns(tg->rt_bandwidth.rt_period);
	runtime = tg->rt_bandwidth.rt_runtime;

	જો (tg == d->tg) {
		period = d->rt_period;
		runtime = d->rt_runtime;
	}

	/*
	 * Cannot have more runtime than the period.
	 */
	જો (runtime > period && runtime != RUNTIME_INF)
		પાછા -EINVAL;

	/*
	 * Ensure we don't starve existing RT tasks if runtime turns zero.
	 */
	જો (rt_bandwidth_enabled() && !runtime &&
	    tg->rt_bandwidth.rt_runtime && tg_has_rt_tasks(tg))
		પાછા -EBUSY;

	total = to_ratio(period, runtime);

	/*
	 * Nobody can have more than the global setting allows.
	 */
	જો (total > to_ratio(global_rt_period(), global_rt_runtime()))
		પાછા -EINVAL;

	/*
	 * The sum of our children's runtime should not exceed our own.
	 */
	list_for_each_entry_rcu(child, &tg->children, siblings) {
		period = ktime_to_ns(child->rt_bandwidth.rt_period);
		runtime = child->rt_bandwidth.rt_runtime;

		જો (child == d->tg) {
			period = d->rt_period;
			runtime = d->rt_runtime;
		}

		sum += to_ratio(period, runtime);
	}

	જો (sum > total)
		પાછા -EINVAL;

	પાછા 0;
}

સ્થિર પૂર્ણાંક __rt_schedulable(સંરચના task_group *tg, u64 period, u64 runtime)
{
	પૂર્ણાંક ret;

	સંરચના rt_schedulable_data data = {
		.tg = tg,
		.rt_period = period,
		.rt_runtime = runtime,
	};

	rcu_read_lock();
	ret = walk_tg_વૃક્ષ(tg_rt_schedulable, tg_nop, &data);
	rcu_read_unlock();

	પાછા ret;
}

સ્થિર પૂર્ણાંક tg_set_rt_bandwidth(સંરચના task_group *tg,
		u64 rt_period, u64 rt_runtime)
{
	પૂર્ણાંક i, err = 0;

	/*
	 * Disallowing the root group RT runtime is BAD, it would disallow the
	 * kernel creating (and or operating) RT threads.
	 */
	જો (tg == &મૂળ_task_group && rt_runtime == 0)
		પાછા -EINVAL;

	/* No period doesn't make any sense. */
	જો (rt_period == 0)
		પાછા -EINVAL;

	/*
	 * Bound quota to defend quota against overflow during bandwidth shift.
	 */
	જો (rt_runtime != RUNTIME_INF && rt_runtime > max_rt_runtime)
		પાછા -EINVAL;

	mutex_lock(&rt_constraints_mutex);
	err = __rt_schedulable(tg, rt_period, rt_runtime);
	જો (err)
		જાઓ unlock;

	raw_spin_lock_irq(&tg->rt_bandwidth.rt_runtime_lock);
	tg->rt_bandwidth.rt_period = ns_to_ktime(rt_period);
	tg->rt_bandwidth.rt_runtime = rt_runtime;

	for_each_possible_cpu(i) {
		સંરચના rt_rq *rt_rq = tg->rt_rq[i];

		raw_spin_lock(&rt_rq->rt_runtime_lock);
		rt_rq->rt_runtime = rt_runtime;
		raw_spin_unlock(&rt_rq->rt_runtime_lock);
	}
	raw_spin_unlock_irq(&tg->rt_bandwidth.rt_runtime_lock);
unlock:
	mutex_unlock(&rt_constraints_mutex);

	પાછા err;
}

પૂર્ણાંક sched_group_set_rt_runtime(સંરચના task_group *tg, long rt_runtime_us)
{
	u64 rt_runtime, rt_period;

	rt_period = ktime_to_ns(tg->rt_bandwidth.rt_period);
	rt_runtime = (u64)rt_runtime_us * NSEC_PER_USEC;
	જો (rt_runtime_us < 0)
		rt_runtime = RUNTIME_INF;
	નહીંતર જો ((u64)rt_runtime_us > U64_MAX / NSEC_PER_USEC)
		પાછા -EINVAL;

	પાછા tg_set_rt_bandwidth(tg, rt_period, rt_runtime);
}

long sched_group_rt_runtime(સંરચના task_group *tg)
{
	u64 rt_runtime_us;

	જો (tg->rt_bandwidth.rt_runtime == RUNTIME_INF)
		પાછા -1;

	rt_runtime_us = tg->rt_bandwidth.rt_runtime;
	do_div(rt_runtime_us, NSEC_PER_USEC);
	પાછા rt_runtime_us;
}

પૂર્ણાંક sched_group_set_rt_period(સંરચના task_group *tg, u64 rt_period_us)
{
	u64 rt_runtime, rt_period;

	જો (rt_period_us > U64_MAX / NSEC_PER_USEC)
		પાછા -EINVAL;

	rt_period = rt_period_us * NSEC_PER_USEC;
	rt_runtime = tg->rt_bandwidth.rt_runtime;

	પાછા tg_set_rt_bandwidth(tg, rt_period, rt_runtime);
}

long sched_group_rt_period(સંરચના task_group *tg)
{
	u64 rt_period_us;

	rt_period_us = ktime_to_ns(tg->rt_bandwidth.rt_period);
	do_div(rt_period_us, NSEC_PER_USEC);
	પાછા rt_period_us;
}

પૂર્ણાંક sched_rt_can_attach(સંરચના task_group *tg, સંરચના task_struct *tsk)
{
	/* Don't accept real-time tasks when there is no way for them to run */
	જો (rt_group_sched_enabled() && rt_task(tsk) && tg->rt_bandwidth.rt_runtime == 0)
		પાછા 0;

	પાછા 1;
}

#endif /* !CONFIG_RT_GROUP_SCHED */

#ifdef CONFIG_SYSCTL
સ્થિર પૂર્ણાંક sched_rt_global_validate(શૂન્ય)
{
	જો ((sysctl_sched_rt_runtime != RUNTIME_INF) &&
		((sysctl_sched_rt_runtime > sysctl_sched_rt_period) ||
		 ((u64)sysctl_sched_rt_runtime *
			NSEC_PER_USEC > max_rt_runtime)))
		પાછા -EINVAL;

#ifdef CONFIG_RT_GROUP_SCHED
	જો (!rt_group_sched_enabled())
		પાછા 0;

	scoped_guard(mutex, &rt_constraints_mutex)
		પાછા __rt_schedulable(ખાલી, 0, 0);
#endif
	પાછા 0;
}

સ્થિર પૂર્ણાંક sched_rt_handler(અચલ સંરચના ctl_table *table, પૂર્ણાંક લિખ, શૂન્ય *buffer,
		size_t *lenp, loff_t *ppos)
{
	પૂર્ણાંક old_period, old_runtime;
	સ્થિર DEFINE_MUTEX(mutex);
	પૂર્ણાંક ret;

	mutex_lock(&mutex);
	sched_domains_mutex_lock();
	old_period = sysctl_sched_rt_period;
	old_runtime = sysctl_sched_rt_runtime;

	ret = proc_dointvec_minmax(table, લિખ, buffer, lenp, ppos);

	જો (!ret && લિખ) {
		ret = sched_rt_global_validate();
		જો (ret)
			જાઓ undo;

		ret = sched_dl_global_validate();
		જો (ret)
			જાઓ undo;

		sched_dl_do_global();
	}
	જો (0) {
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

	પાછા ret;
}

સ્થિર પૂર્ણાંક sched_rr_handler(અચલ સંરચના ctl_table *table, પૂર્ણાંક લિખ, શૂન્ય *buffer,
		size_t *lenp, loff_t *ppos)
{
	પૂર્ણાંક ret;
	સ્થિર DEFINE_MUTEX(mutex);

	mutex_lock(&mutex);
	ret = proc_dointvec(table, લિખ, buffer, lenp, ppos);
	/*
	 * Make sure that internally we keep jiffies.
	 * Also, writing zero resets the time-slice to default:
	 */
	જો (!ret && લિખ) {
		sched_rr_timeslice =
			sysctl_sched_rr_timeslice <= 0 ? RR_TIMESLICE :
			msecs_to_jiffies(sysctl_sched_rr_timeslice);

		જો (sysctl_sched_rr_timeslice <= 0)
			sysctl_sched_rr_timeslice = jiffies_to_msecs(RR_TIMESLICE);
	}
	mutex_unlock(&mutex);

	પાછા ret;
}
#endif /* CONFIG_SYSCTL */

શૂન્ય print_rt_stats(સંરચના seq_file *m, પૂર્ણાંક cpu)
{
	rt_rq_iter_t iter;
	સંરચના rt_rq *rt_rq;

	rcu_read_lock();
	for_each_rt_rq(rt_rq, iter, cpu_rq(cpu))
		print_rt_rq(m, cpu, rt_rq);
	rcu_read_unlock();
}
