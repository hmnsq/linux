// SPDX-License-Identifier: GPL-2.0
/*
 * Real-Time Scheduling Class (mapped to the SCHED_FIFO and SCHED_RR
 * policies)
 */

#अन्तर्भाव "sched.h"
#अन्तर्भाव "pelt.h"

पूर्णाङ्क sched_rr_timeslice = RR_TIMESLICE;
/* More than 4 hours if BW_SHIFT equals 20. */
स्थिर अचर u64 max_rt_runtime = MAX_BW;

/*
 * period over which we measure -rt task CPU usage in us.
 * default: 1s
 */
पूर्णाङ्क sysctl_sched_rt_period = 1000000;

/*
 * part of the period that we allow rt tasks to run in us.
 * default: 1s
 */
पूर्णाङ्क sysctl_sched_rt_runtime = 1000000;

#ifdef CONFIG_SYSCTL
स्थिर पूर्णाङ्क sysctl_sched_rr_timeslice = (MSEC_PER_SEC * RR_TIMESLICE) / HZ;
स्थिर पूर्णाङ्क sched_rt_handler(अचर संरचना ctl_table *table, पूर्णाङ्क लिख, शून्य *buffer,
		माप_प्रकार *lenp, loff_t *ppos);
स्थिर पूर्णाङ्क sched_rr_handler(अचर संरचना ctl_table *table, पूर्णाङ्क लिख, शून्य *buffer,
		माप_प्रकार *lenp, loff_t *ppos);
स्थिर अचर संरचना ctl_table sched_rt_sysctls[] = {
	{
		.procname       = "sched_rt_period_us",
		.data           = &sysctl_sched_rt_period,
		.maxlen         = परिमाण(पूर्णाङ्क),
		.mode           = 0644,
		.proc_handler   = sched_rt_handler,
		.extra1         = SYSCTL_ONE,
		.extra2         = SYSCTL_INT_MAX,
	},
	{
		.procname       = "sched_rt_runtime_us",
		.data           = &sysctl_sched_rt_runtime,
		.maxlen         = परिमाण(पूर्णाङ्क),
		.mode           = 0644,
		.proc_handler   = sched_rt_handler,
		.extra1         = SYSCTL_NEG_ONE,
		.extra2         = (शून्य *)&sysctl_sched_rt_period,
	},
	{
		.procname       = "sched_rr_timeslice_ms",
		.data           = &sysctl_sched_rr_timeslice,
		.maxlen         = परिमाण(पूर्णाङ्क),
		.mode           = 0644,
		.proc_handler   = sched_rr_handler,
	},
};

स्थिर पूर्णाङ्क __init sched_rt_sysctl_init(शून्य)
{
	register_sysctl_init("kernel", sched_rt_sysctls);
	फर्काऊ 0;
}
late_initcall(sched_rt_sysctl_init);
#endif /* CONFIG_SYSCTL */

शून्य init_rt_rq(संरचना rt_rq *rt_rq)
{
	संरचना rt_prio_array *array;
	पूर्णाङ्क i;

	array = &rt_rq->active;
	लागि (i = 0; i < MAX_RT_PRIO; i++) {
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
	rt_rq->tg = &जरा_task_group;
#endif
}

#ifdef CONFIG_RT_GROUP_SCHED

स्थिर पूर्णाङ्क do_sched_rt_period_timer(संरचना rt_bandwidth *rt_b, पूर्णाङ्क overrun);

स्थिर गणना hrtimer_restart sched_rt_period_timer(संरचना hrtimer *timer)
{
	संरचना rt_bandwidth *rt_b =
		container_of(timer, संरचना rt_bandwidth, rt_period_timer);
	पूर्णाङ्क idle = 0;
	पूर्णाङ्क overrun;

	raw_spin_lock(&rt_b->rt_runtime_lock);
	लागि (;;) {
		overrun = hrtimer_forward_now(timer, rt_b->rt_period);
		यदि (!overrun)
			रोक;

		raw_spin_unlock(&rt_b->rt_runtime_lock);
		idle = do_sched_rt_period_timer(rt_b, overrun);
		raw_spin_lock(&rt_b->rt_runtime_lock);
	}
	यदि (idle)
		rt_b->rt_period_active = 0;
	raw_spin_unlock(&rt_b->rt_runtime_lock);

	फर्काऊ idle ? HRTIMER_NORESTART : HRTIMER_RESTART;
}

शून्य init_rt_bandwidth(संरचना rt_bandwidth *rt_b, u64 period, u64 runtime)
{
	rt_b->rt_period = ns_to_ktime(period);
	rt_b->rt_runtime = runtime;

	raw_spin_lock_init(&rt_b->rt_runtime_lock);

	hrtimer_setup(&rt_b->rt_period_timer, sched_rt_period_timer, CLOCK_MONOTONIC,
		      HRTIMER_MODE_REL_HARD);
}

स्थिर अन्तःस्थ शून्य do_start_rt_bandwidth(संरचना rt_bandwidth *rt_b)
{
	raw_spin_lock(&rt_b->rt_runtime_lock);
	यदि (!rt_b->rt_period_active) {
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

स्थिर शून्य start_rt_bandwidth(संरचना rt_bandwidth *rt_b)
{
	यदि (!rt_bandwidth_enabled() || rt_b->rt_runtime == RUNTIME_INF)
		फर्काऊ;

	do_start_rt_bandwidth(rt_b);
}

स्थिर शून्य destroy_rt_bandwidth(संरचना rt_bandwidth *rt_b)
{
	hrtimer_cancel(&rt_b->rt_period_timer);
}

#define rt_entity_is_task(rt_se) (!(rt_se)->my_q)

स्थिर अन्तःस्थ संरचना task_struct *rt_task_of(संरचना sched_rt_entity *rt_se)
{
	WARN_ON_ONCE(!rt_entity_is_task(rt_se));

	फर्काऊ container_of(rt_se, संरचना task_struct, rt);
}

स्थिर अन्तःस्थ संरचना rq *rq_of_rt_rq(संरचना rt_rq *rt_rq)
{
	/* Cannot fold with non-CONFIG_RT_GROUP_SCHED version, layout */
	WARN_ON(!rt_group_sched_enabled() && rt_rq->tg != &जरा_task_group);
	फर्काऊ rt_rq->rq;
}

स्थिर अन्तःस्थ संरचना rt_rq *rt_rq_of_se(संरचना sched_rt_entity *rt_se)
{
	WARN_ON(!rt_group_sched_enabled() && rt_se->rt_rq->tg != &जरा_task_group);
	फर्काऊ rt_se->rt_rq;
}

स्थिर अन्तःस्थ संरचना rq *rq_of_rt_se(संरचना sched_rt_entity *rt_se)
{
	संरचना rt_rq *rt_rq = rt_se->rt_rq;

	WARN_ON(!rt_group_sched_enabled() && rt_rq->tg != &जरा_task_group);
	फर्काऊ rt_rq->rq;
}

शून्य unregister_rt_sched_group(संरचना task_group *tg)
{
	यदि (!rt_group_sched_enabled())
		फर्काऊ;

	यदि (tg->rt_se)
		destroy_rt_bandwidth(&tg->rt_bandwidth);
}

शून्य free_rt_sched_group(संरचना task_group *tg)
{
	पूर्णाङ्क i;

	यदि (!rt_group_sched_enabled())
		फर्काऊ;

	for_each_possible_cpu(i) {
		यदि (tg->rt_rq)
			kfree(tg->rt_rq[i]);
		यदि (tg->rt_se)
			kfree(tg->rt_se[i]);
	}

	kfree(tg->rt_rq);
	kfree(tg->rt_se);
}

शून्य init_tg_rt_entry(संरचना task_group *tg, संरचना rt_rq *rt_rq,
		संरचना sched_rt_entity *rt_se, पूर्णाङ्क cpu,
		संरचना sched_rt_entity *parent)
{
	संरचना rq *rq = cpu_rq(cpu);

	rt_rq->highest_prio.curr = MAX_RT_PRIO-1;
	rt_rq->rt_nr_boosted = 0;
	rt_rq->rq = rq;
	rt_rq->tg = tg;

	tg->rt_rq[cpu] = rt_rq;
	tg->rt_se[cpu] = rt_se;

	यदि (!rt_se)
		फर्काऊ;

	यदि (!parent)
		rt_se->rt_rq = &rq->rt;
	अन्यथा
		rt_se->rt_rq = parent->my_q;

	rt_se->my_q = rt_rq;
	rt_se->parent = parent;
	INIT_LIST_HEAD(&rt_se->run_list);
}

पूर्णाङ्क alloc_rt_sched_group(संरचना task_group *tg, संरचना task_group *parent)
{
	संरचना rt_rq *rt_rq;
	संरचना sched_rt_entity *rt_se;
	पूर्णाङ्क i;

	यदि (!rt_group_sched_enabled())
		फर्काऊ 1;

	tg->rt_rq = kzalloc_objs(rt_rq, nr_cpu_ids);
	यदि (!tg->rt_rq)
		गच्छ err;
	tg->rt_se = kzalloc_objs(rt_se, nr_cpu_ids);
	यदि (!tg->rt_se)
		गच्छ err;

	init_rt_bandwidth(&tg->rt_bandwidth, ktime_to_ns(global_rt_period()), 0);

	for_each_possible_cpu(i) {
		rt_rq = kzalloc_गाँठो(परिमाण(संरचना rt_rq),
				     GFP_KERNEL, cpu_to_गाँठो(i));
		यदि (!rt_rq)
			गच्छ err;

		rt_se = kzalloc_गाँठो(परिमाण(संरचना sched_rt_entity),
				     GFP_KERNEL, cpu_to_गाँठो(i));
		यदि (!rt_se)
			गच्छ err_free_rq;

		init_rt_rq(rt_rq);
		rt_rq->rt_runtime = tg->rt_bandwidth.rt_runtime;
		init_tg_rt_entry(tg, rt_rq, rt_se, i, parent->rt_se[i]);
	}

	फर्काऊ 1;

err_free_rq:
	kfree(rt_rq);
err:
	फर्काऊ 0;
}

#अन्यथा /* !CONFIG_RT_GROUP_SCHED: */

#define rt_entity_is_task(rt_se) (1)

स्थिर अन्तःस्थ संरचना task_struct *rt_task_of(संरचना sched_rt_entity *rt_se)
{
	फर्काऊ container_of(rt_se, संरचना task_struct, rt);
}

स्थिर अन्तःस्थ संरचना rq *rq_of_rt_rq(संरचना rt_rq *rt_rq)
{
	फर्काऊ container_of(rt_rq, संरचना rq, rt);
}

स्थिर अन्तःस्थ संरचना rq *rq_of_rt_se(संरचना sched_rt_entity *rt_se)
{
	संरचना task_struct *p = rt_task_of(rt_se);

	फर्काऊ task_rq(p);
}

स्थिर अन्तःस्थ संरचना rt_rq *rt_rq_of_se(संरचना sched_rt_entity *rt_se)
{
	संरचना rq *rq = rq_of_rt_se(rt_se);

	फर्काऊ &rq->rt;
}

शून्य unregister_rt_sched_group(संरचना task_group *tg) { }

शून्य free_rt_sched_group(संरचना task_group *tg) { }

पूर्णाङ्क alloc_rt_sched_group(संरचना task_group *tg, संरचना task_group *parent)
{
	फर्काऊ 1;
}
#endif /* !CONFIG_RT_GROUP_SCHED */

स्थिर अन्तःस्थ bool need_pull_rt_task(संरचना rq *rq, संरचना task_struct *prev)
{
	/* Try to pull RT tasks here if we lower this rq's prio */
	फर्काऊ rq->online && rq->rt.highest_prio.curr > prev->prio;
}

स्थिर अन्तःस्थ पूर्णाङ्क rt_overloaded(संरचना rq *rq)
{
	फर्काऊ atomic_read(&rq->rd->rto_गन्ती);
}

स्थिर अन्तःस्थ शून्य rt_set_overload(संरचना rq *rq)
{
	यदि (!rq->online)
		फर्काऊ;

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
	atomic_inc(&rq->rd->rto_गन्ती);
}

स्थिर अन्तःस्थ शून्य rt_clear_overload(संरचना rq *rq)
{
	यदि (!rq->online)
		फर्काऊ;

	/* the order here really doesn't matter */
	atomic_dec(&rq->rd->rto_गन्ती);
	cpumask_clear_cpu(rq->cpu, rq->rd->rto_mask);
}

स्थिर अन्तःस्थ पूर्णाङ्क has_pushable_tasks(संरचना rq *rq)
{
	फर्काऊ !plist_head_empty(&rq->rt.pushable_tasks);
}

स्थिर DEFINE_PER_CPU(संरचना balance_callback, rt_थप_head);
स्थिर DEFINE_PER_CPU(संरचना balance_callback, rt_pull_head);

स्थिर शून्य थप_rt_tasks(संरचना rq *);
स्थिर शून्य pull_rt_task(संरचना rq *);

स्थिर अन्तःस्थ शून्य rt_queue_थप_tasks(संरचना rq *rq)
{
	यदि (!has_pushable_tasks(rq))
		फर्काऊ;

	queue_balance_callback(rq, &per_cpu(rt_थप_head, rq->cpu), थप_rt_tasks);
}

स्थिर अन्तःस्थ शून्य rt_queue_pull_task(संरचना rq *rq)
{
	queue_balance_callback(rq, &per_cpu(rt_pull_head, rq->cpu), pull_rt_task);
}

स्थिर शून्य enqueue_pushable_task(संरचना rq *rq, संरचना task_struct *p)
{
	plist_del(&p->pushable_tasks, &rq->rt.pushable_tasks);
	plist_गाँठो_init(&p->pushable_tasks, p->prio);
	plist_add(&p->pushable_tasks, &rq->rt.pushable_tasks);

	/* Update the highest prio pushable task */
	यदि (p->prio < rq->rt.highest_prio.next)
		rq->rt.highest_prio.next = p->prio;

	यदि (!rq->rt.overloaded) {
		rt_set_overload(rq);
		rq->rt.overloaded = 1;
	}
}

स्थिर शून्य dequeue_pushable_task(संरचना rq *rq, संरचना task_struct *p)
{
	plist_del(&p->pushable_tasks, &rq->rt.pushable_tasks);

	/* Update the new highest prio pushable task */
	यदि (has_pushable_tasks(rq)) {
		p = plist_first_entry(&rq->rt.pushable_tasks,
				      संरचना task_struct, pushable_tasks);
		rq->rt.highest_prio.next = p->prio;
	} अन्यथा {
		rq->rt.highest_prio.next = MAX_RT_PRIO-1;

		यदि (rq->rt.overloaded) {
			rt_clear_overload(rq);
			rq->rt.overloaded = 0;
		}
	}
}

स्थिर शून्य enqueue_शीर्ष_rt_rq(संरचना rt_rq *rt_rq);
स्थिर शून्य dequeue_शीर्ष_rt_rq(संरचना rt_rq *rt_rq, अचिन्हित पूर्णाङ्क गन्ती);

स्थिर अन्तःस्थ पूर्णाङ्क on_rt_rq(संरचना sched_rt_entity *rt_se)
{
	फर्काऊ rt_se->on_rq;
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
स्थिर अन्तःस्थ bool rt_task_fits_capacity(संरचना task_struct *p, पूर्णाङ्क cpu)
{
	अचिन्हित पूर्णाङ्क min_cap;
	अचिन्हित पूर्णाङ्क max_cap;
	अचिन्हित पूर्णाङ्क cpu_cap;

	/* Only heterogeneous systems can benefit from this check */
	यदि (!sched_asym_cpucap_active())
		फर्काऊ true;

	min_cap = uclamp_eff_मान(p, UCLAMP_MIN);
	max_cap = uclamp_eff_मान(p, UCLAMP_MAX);

	cpu_cap = arch_scale_cpu_capacity(cpu);

	फर्काऊ cpu_cap >= min(min_cap, max_cap);
}
#अन्यथा /* !CONFIG_UCLAMP_TASK: */
स्थिर अन्तःस्थ bool rt_task_fits_capacity(संरचना task_struct *p, पूर्णाङ्क cpu)
{
	फर्काऊ true;
}
#endif /* !CONFIG_UCLAMP_TASK */

#ifdef CONFIG_RT_GROUP_SCHED

स्थिर अन्तःस्थ u64 sched_rt_runtime(संरचना rt_rq *rt_rq)
{
	फर्काऊ rt_rq->rt_runtime;
}

स्थिर अन्तःस्थ u64 sched_rt_period(संरचना rt_rq *rt_rq)
{
	फर्काऊ ktime_to_ns(rt_rq->tg->rt_bandwidth.rt_period);
}

प्रकारनिर्देश संरचना task_group *rt_rq_iter_t;

स्थिर अन्तःस्थ संरचना task_group *next_task_group(संरचना task_group *tg)
{
	यदि (!rt_group_sched_enabled()) {
		WARN_ON(tg != &जरा_task_group);
		फर्काऊ रिक्त;
	}

	गर {
		tg = list_entry_rcu(tg->list.next,
			typeof(संरचना task_group), list);
	} जबसम्म (&tg->list != &task_groups && task_group_is_autogroup(tg));

	यदि (&tg->list == &task_groups)
		tg = रिक्त;

	फर्काऊ tg;
}

#define for_each_rt_rq(rt_rq, iter, rq)					\
	लागि (iter = &जरा_task_group;					\
		iter && (rt_rq = iter->rt_rq[cpu_of(rq)]);		\
		iter = next_task_group(iter))

#define for_each_sched_rt_entity(rt_se) \
	लागि (; rt_se; rt_se = rt_se->parent)

स्थिर अन्तःस्थ संरचना rt_rq *group_rt_rq(संरचना sched_rt_entity *rt_se)
{
	फर्काऊ rt_se->my_q;
}

स्थिर शून्य enqueue_rt_entity(संरचना sched_rt_entity *rt_se, अचिन्हित पूर्णाङ्क flags);
स्थिर शून्य dequeue_rt_entity(संरचना sched_rt_entity *rt_se, अचिन्हित पूर्णाङ्क flags);

स्थिर शून्य sched_rt_rq_enqueue(संरचना rt_rq *rt_rq)
{
	संरचना task_struct *donor = rq_of_rt_rq(rt_rq)->donor;
	संरचना rq *rq = rq_of_rt_rq(rt_rq);
	संरचना sched_rt_entity *rt_se;

	पूर्णाङ्क cpu = cpu_of(rq);

	rt_se = rt_rq->tg->rt_se[cpu];

	यदि (rt_rq->rt_nr_running) {
		यदि (!rt_se)
			enqueue_शीर्ष_rt_rq(rt_rq);
		अन्यथा यदि (!on_rt_rq(rt_se))
			enqueue_rt_entity(rt_se, 0);

		यदि (rt_rq->highest_prio.curr < donor->prio)
			resched_curr(rq);
	}
}

स्थिर शून्य sched_rt_rq_dequeue(संरचना rt_rq *rt_rq)
{
	संरचना sched_rt_entity *rt_se;
	पूर्णाङ्क cpu = cpu_of(rq_of_rt_rq(rt_rq));

	rt_se = rt_rq->tg->rt_se[cpu];

	यदि (!rt_se) {
		dequeue_शीर्ष_rt_rq(rt_rq, rt_rq->rt_nr_running);
		/* Kick cpufreq (see the comment in kernel/sched/sched.h). */
		cpufreq_update_util(rq_of_rt_rq(rt_rq), 0);
	}
	अन्यथा यदि (on_rt_rq(rt_se))
		dequeue_rt_entity(rt_se, 0);
}

स्थिर अन्तःस्थ पूर्णाङ्क rt_rq_throttled(संरचना rt_rq *rt_rq)
{
	फर्काऊ rt_rq->rt_throttled && !rt_rq->rt_nr_boosted;
}

स्थिर पूर्णाङ्क rt_se_boosted(संरचना sched_rt_entity *rt_se)
{
	संरचना rt_rq *rt_rq = group_rt_rq(rt_se);
	संरचना task_struct *p;

	यदि (rt_rq)
		फर्काऊ !!rt_rq->rt_nr_boosted;

	p = rt_task_of(rt_se);
	फर्काऊ p->prio != p->normal_prio;
}

स्थिर अन्तःस्थ अचर संरचना cpumask *sched_rt_period_mask(शून्य)
{
	फर्काऊ this_rq()->rd->span;
}

स्थिर अन्तःस्थ
संरचना rt_rq *sched_rt_period_rt_rq(संरचना rt_bandwidth *rt_b, पूर्णाङ्क cpu)
{
	फर्काऊ container_of(rt_b, संरचना task_group, rt_bandwidth)->rt_rq[cpu];
}

स्थिर अन्तःस्थ संरचना rt_bandwidth *sched_rt_bandwidth(संरचना rt_rq *rt_rq)
{
	फर्काऊ &rt_rq->tg->rt_bandwidth;
}

bool sched_rt_bandwidth_account(संरचना rt_rq *rt_rq)
{
	संरचना rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);

	फर्काऊ (hrtimer_active(&rt_b->rt_period_timer) ||
		rt_rq->rt_time < rt_b->rt_runtime);
}

/*
 * We ran out of runtime, see if we can borrow some from our neighbours.
 */
स्थिर शून्य do_balance_runtime(संरचना rt_rq *rt_rq)
{
	संरचना rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);
	संरचना जरा_domain *rd = rq_of_rt_rq(rt_rq)->rd;
	पूर्णाङ्क i, weight;
	u64 rt_period;

	weight = cpumask_weight(rd->span);

	raw_spin_lock(&rt_b->rt_runtime_lock);
	rt_period = ktime_to_ns(rt_b->rt_period);
	for_each_cpu(i, rd->span) {
		संरचना rt_rq *iter = sched_rt_period_rt_rq(rt_b, i);
		s64 diff;

		यदि (iter == rt_rq)
			जारी;

		raw_spin_lock(&iter->rt_runtime_lock);
		/*
		 * Either all rqs have inf runtime and there's nothing to steal
		 * or __disable_runtime() below sets a specific rq to inf to
		 * indicate its been disabled and disallow stealing.
		 */
		यदि (iter->rt_runtime == RUNTIME_INF)
			गच्छ next;

		/*
		 * From runqueues with spare time, take 1/n part of their
		 * spare time, but no more than our period.
		 */
		diff = iter->rt_runtime - iter->rt_time;
		यदि (diff > 0) {
			diff = div_u64((u64)diff, weight);
			यदि (rt_rq->rt_runtime + diff > rt_period)
				diff = rt_period - rt_rq->rt_runtime;
			iter->rt_runtime -= diff;
			rt_rq->rt_runtime += diff;
			यदि (rt_rq->rt_runtime == rt_period) {
				raw_spin_unlock(&iter->rt_runtime_lock);
				रोक;
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
स्थिर शून्य __disable_runtime(संरचना rq *rq)
{
	संरचना जरा_domain *rd = rq->rd;
	rt_rq_iter_t iter;
	संरचना rt_rq *rt_rq;

	यदि (unlikely(!scheduler_running))
		फर्काऊ;

	for_each_rt_rq(rt_rq, iter, rq) {
		संरचना rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);
		s64 want;
		पूर्णाङ्क i;

		raw_spin_lock(&rt_b->rt_runtime_lock);
		raw_spin_lock(&rt_rq->rt_runtime_lock);
		/*
		 * Either we're all inf and nobody needs to borrow, or we're
		 * already disabled and thus have nothing to do, or we have
		 * exactly the right amount of runtime to take out.
		 */
		यदि (rt_rq->rt_runtime == RUNTIME_INF ||
				rt_rq->rt_runtime == rt_b->rt_runtime)
			गच्छ balanced;
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
			संरचना rt_rq *iter = sched_rt_period_rt_rq(rt_b, i);
			s64 diff;

			/*
			 * Can't reclaim from ourselves or disabled runqueues.
			 */
			यदि (iter == rt_rq || iter->rt_runtime == RUNTIME_INF)
				जारी;

			raw_spin_lock(&iter->rt_runtime_lock);
			यदि (want > 0) {
				diff = min_t(s64, iter->rt_runtime, want);
				iter->rt_runtime -= diff;
				want -= diff;
			} अन्यथा {
				iter->rt_runtime -= want;
				want -= want;
			}
			raw_spin_unlock(&iter->rt_runtime_lock);

			यदि (!want)
				रोक;
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

स्थिर शून्य __enable_runtime(संरचना rq *rq)
{
	rt_rq_iter_t iter;
	संरचना rt_rq *rt_rq;

	यदि (unlikely(!scheduler_running))
		फर्काऊ;

	/*
	 * Reset each runqueue's bandwidth settings
	 */
	for_each_rt_rq(rt_rq, iter, rq) {
		संरचना rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);

		raw_spin_lock(&rt_b->rt_runtime_lock);
		raw_spin_lock(&rt_rq->rt_runtime_lock);
		rt_rq->rt_runtime = rt_b->rt_runtime;
		rt_rq->rt_time = 0;
		rt_rq->rt_throttled = 0;
		raw_spin_unlock(&rt_rq->rt_runtime_lock);
		raw_spin_unlock(&rt_b->rt_runtime_lock);
	}
}

स्थिर शून्य balance_runtime(संरचना rt_rq *rt_rq)
{
	यदि (!sched_feat(RT_RUNTIME_SHARE))
		फर्काऊ;

	यदि (rt_rq->rt_time > rt_rq->rt_runtime) {
		raw_spin_unlock(&rt_rq->rt_runtime_lock);
		do_balance_runtime(rt_rq);
		raw_spin_lock(&rt_rq->rt_runtime_lock);
	}
}

स्थिर पूर्णाङ्क do_sched_rt_period_timer(संरचना rt_bandwidth *rt_b, पूर्णाङ्क overrun)
{
	पूर्णाङ्क i, idle = 1, throttled = 0;
	अचर संरचना cpumask *span;

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
	यदि (rt_b == &जरा_task_group.rt_bandwidth)
		span = cpu_online_mask;

	for_each_cpu(i, span) {
		पूर्णाङ्क enqueue = 0;
		संरचना rt_rq *rt_rq = sched_rt_period_rt_rq(rt_b, i);
		संरचना rq *rq = rq_of_rt_rq(rt_rq);
		संरचना rq_flags rf;
		पूर्णाङ्क skip;

		/*
		 * When span == cpu_online_mask, taking each rq->lock
		 * can be time-consuming. Try to avoid it when possible.
		 */
		raw_spin_lock(&rt_rq->rt_runtime_lock);
		यदि (!sched_feat(RT_RUNTIME_SHARE) && rt_rq->rt_runtime != RUNTIME_INF)
			rt_rq->rt_runtime = rt_b->rt_runtime;
		skip = !rt_rq->rt_time && !rt_rq->rt_nr_running;
		raw_spin_unlock(&rt_rq->rt_runtime_lock);
		यदि (skip)
			जारी;

		rq_lock(rq, &rf);
		update_rq_clock(rq);

		यदि (rt_rq->rt_time) {
			u64 runtime;

			raw_spin_lock(&rt_rq->rt_runtime_lock);
			यदि (rt_rq->rt_throttled)
				balance_runtime(rt_rq);
			runtime = rt_rq->rt_runtime;
			rt_rq->rt_time -= min(rt_rq->rt_time, overrun*runtime);
			यदि (rt_rq->rt_throttled && rt_rq->rt_time < runtime) {
				rt_rq->rt_throttled = 0;
				enqueue = 1;

				/*
				 * When we're idle and a woken (rt) task is
				 * throttled wakeup_preempt() will set
				 * skip_update and the time between the wakeup
				 * and this unthrottle will get accounted as
				 * 'runtime'.
				 */
				यदि (rt_rq->rt_nr_running && rq->curr == rq->idle)
					rq_clock_cancel_skipupdate(rq);
			}
			यदि (rt_rq->rt_time || rt_rq->rt_nr_running)
				idle = 0;
			raw_spin_unlock(&rt_rq->rt_runtime_lock);
		} अन्यथा यदि (rt_rq->rt_nr_running) {
			idle = 0;
			यदि (!rt_rq_throttled(rt_rq))
				enqueue = 1;
		}
		यदि (rt_rq->rt_throttled)
			throttled = 1;

		यदि (enqueue)
			sched_rt_rq_enqueue(rt_rq);
		rq_unlock(rq, &rf);
	}

	यदि (!throttled && (!rt_bandwidth_enabled() || rt_b->rt_runtime == RUNTIME_INF))
		फर्काऊ 1;

	फर्काऊ idle;
}

स्थिर पूर्णाङ्क sched_rt_runtime_exceeded(संरचना rt_rq *rt_rq)
{
	u64 runtime = sched_rt_runtime(rt_rq);

	यदि (rt_rq->rt_throttled)
		फर्काऊ rt_rq_throttled(rt_rq);

	यदि (runtime >= sched_rt_period(rt_rq))
		फर्काऊ 0;

	balance_runtime(rt_rq);
	runtime = sched_rt_runtime(rt_rq);
	यदि (runtime == RUNTIME_INF)
		फर्काऊ 0;

	यदि (rt_rq->rt_time > runtime) {
		संरचना rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);

		/*
		 * Don't actually throttle groups that have no runtime assigned
		 * but accrue some time due to boosting.
		 */
		यदि (likely(rt_b->rt_runtime)) {
			rt_rq->rt_throttled = 1;
			printk_deferred_once("sched: RT throttling activated\n");
		} अन्यथा {
			/*
			 * In case we did anyway, make it go away,
			 * replenishment is a joke, since it will replenish us
			 * with exactly 0 ns.
			 */
			rt_rq->rt_time = 0;
		}

		यदि (rt_rq_throttled(rt_rq)) {
			sched_rt_rq_dequeue(rt_rq);
			फर्काऊ 1;
		}
	}

	फर्काऊ 0;
}

#अन्यथा /* !CONFIG_RT_GROUP_SCHED: */

प्रकारनिर्देश संरचना rt_rq *rt_rq_iter_t;

#define for_each_rt_rq(rt_rq, iter, rq) \
	लागि ((शून्य) iter, rt_rq = &rq->rt; rt_rq; rt_rq = रिक्त)

#define for_each_sched_rt_entity(rt_se) \
	लागि (; rt_se; rt_se = रिक्त)

स्थिर अन्तःस्थ संरचना rt_rq *group_rt_rq(संरचना sched_rt_entity *rt_se)
{
	फर्काऊ रिक्त;
}

स्थिर अन्तःस्थ शून्य sched_rt_rq_enqueue(संरचना rt_rq *rt_rq)
{
	संरचना rq *rq = rq_of_rt_rq(rt_rq);

	यदि (!rt_rq->rt_nr_running)
		फर्काऊ;

	enqueue_शीर्ष_rt_rq(rt_rq);
	resched_curr(rq);
}

स्थिर अन्तःस्थ शून्य sched_rt_rq_dequeue(संरचना rt_rq *rt_rq)
{
	dequeue_शीर्ष_rt_rq(rt_rq, rt_rq->rt_nr_running);
}

स्थिर अन्तःस्थ पूर्णाङ्क rt_rq_throttled(संरचना rt_rq *rt_rq)
{
	फर्काऊ false;
}

स्थिर अन्तःस्थ अचर संरचना cpumask *sched_rt_period_mask(शून्य)
{
	फर्काऊ cpu_online_mask;
}

स्थिर अन्तःस्थ
संरचना rt_rq *sched_rt_period_rt_rq(संरचना rt_bandwidth *rt_b, पूर्णाङ्क cpu)
{
	फर्काऊ &cpu_rq(cpu)->rt;
}

स्थिर शून्य __enable_runtime(संरचना rq *rq) { }
स्थिर शून्य __disable_runtime(संरचना rq *rq) { }

#endif /* !CONFIG_RT_GROUP_SCHED */

स्थिर अन्तःस्थ पूर्णाङ्क rt_se_prio(संरचना sched_rt_entity *rt_se)
{
#ifdef CONFIG_RT_GROUP_SCHED
	संरचना rt_rq *rt_rq = group_rt_rq(rt_se);

	यदि (rt_rq)
		फर्काऊ rt_rq->highest_prio.curr;
#endif

	फर्काऊ rt_task_of(rt_se)->prio;
}

/*
 * Update the current task's runtime statistics. Skip current tasks that
 * are not in our scheduling class.
 */
स्थिर शून्य update_curr_rt(संरचना rq *rq)
{
	संरचना task_struct *donor = rq->donor;
	s64 delta_exec;

	यदि (donor->sched_class != &rt_sched_class)
		फर्काऊ;

	delta_exec = update_curr_common(rq);
	यदि (unlikely(delta_exec <= 0))
		फर्काऊ;

#ifdef CONFIG_RT_GROUP_SCHED
	संरचना sched_rt_entity *rt_se = &donor->rt;

	यदि (!rt_bandwidth_enabled())
		फर्काऊ;

	for_each_sched_rt_entity(rt_se) {
		संरचना rt_rq *rt_rq = rt_rq_of_se(rt_se);
		पूर्णाङ्क exceeded;

		यदि (sched_rt_runtime(rt_rq) != RUNTIME_INF) {
			raw_spin_lock(&rt_rq->rt_runtime_lock);
			rt_rq->rt_time += delta_exec;
			exceeded = sched_rt_runtime_exceeded(rt_rq);
			यदि (exceeded)
				resched_curr(rq);
			raw_spin_unlock(&rt_rq->rt_runtime_lock);
			यदि (exceeded)
				do_start_rt_bandwidth(sched_rt_bandwidth(rt_rq));
		}
	}
#endif /* CONFIG_RT_GROUP_SCHED */
}

स्थिर शून्य
dequeue_शीर्ष_rt_rq(संरचना rt_rq *rt_rq, अचिन्हित पूर्णाङ्क गन्ती)
{
	संरचना rq *rq = rq_of_rt_rq(rt_rq);

	BUG_ON(&rq->rt != rt_rq);

	यदि (!rt_rq->rt_queued)
		फर्काऊ;

	BUG_ON(!rq->nr_running);

	sub_nr_running(rq, गन्ती);
	rt_rq->rt_queued = 0;

}

स्थिर शून्य
enqueue_शीर्ष_rt_rq(संरचना rt_rq *rt_rq)
{
	संरचना rq *rq = rq_of_rt_rq(rt_rq);

	BUG_ON(&rq->rt != rt_rq);

	यदि (rt_rq->rt_queued)
		फर्काऊ;

	यदि (rt_rq_throttled(rt_rq))
		फर्काऊ;

	यदि (rt_rq->rt_nr_running) {
		add_nr_running(rq, rt_rq->rt_nr_running);
		rt_rq->rt_queued = 1;
	}

	/* Kick cpufreq (see the comment in kernel/sched/sched.h). */
	cpufreq_update_util(rq, 0);
}

स्थिर शून्य
inc_rt_prio_smp(संरचना rt_rq *rt_rq, पूर्णाङ्क prio, पूर्णाङ्क prev_prio)
{
	संरचना rq *rq = rq_of_rt_rq(rt_rq);

	/*
	 * Change rq's cpupri only if rt_rq is the top queue.
	 */
	यदि (IS_ENABLED(CONFIG_RT_GROUP_SCHED) && &rq->rt != rt_rq)
		फर्काऊ;

	यदि (rq->online && prio < prev_prio)
		cpupri_set(&rq->rd->cpupri, rq->cpu, prio);
}

स्थिर शून्य
dec_rt_prio_smp(संरचना rt_rq *rt_rq, पूर्णाङ्क prio, पूर्णाङ्क prev_prio)
{
	संरचना rq *rq = rq_of_rt_rq(rt_rq);

	/*
	 * Change rq's cpupri only if rt_rq is the top queue.
	 */
	यदि (IS_ENABLED(CONFIG_RT_GROUP_SCHED) && &rq->rt != rt_rq)
		फर्काऊ;

	यदि (rq->online && rt_rq->highest_prio.curr != prev_prio)
		cpupri_set(&rq->rd->cpupri, rq->cpu, rt_rq->highest_prio.curr);
}

स्थिर शून्य
inc_rt_prio(संरचना rt_rq *rt_rq, पूर्णाङ्क prio)
{
	पूर्णाङ्क prev_prio = rt_rq->highest_prio.curr;

	यदि (prio < prev_prio)
		rt_rq->highest_prio.curr = prio;

	inc_rt_prio_smp(rt_rq, prio, prev_prio);
}

स्थिर शून्य
dec_rt_prio(संरचना rt_rq *rt_rq, पूर्णाङ्क prio)
{
	पूर्णाङ्क prev_prio = rt_rq->highest_prio.curr;

	यदि (rt_rq->rt_nr_running) {

		WARN_ON(prio < prev_prio);

		/*
		 * This may have been our highest task, and therefore
		 * we may have some re-computation to do
		 */
		यदि (prio == prev_prio) {
			संरचना rt_prio_array *array = &rt_rq->active;

			rt_rq->highest_prio.curr =
				sched_find_first_bit(array->bitmap);
		}

	} अन्यथा {
		rt_rq->highest_prio.curr = MAX_RT_PRIO-1;
	}

	dec_rt_prio_smp(rt_rq, prio, prev_prio);
}

#ifdef CONFIG_RT_GROUP_SCHED

स्थिर शून्य
inc_rt_group(संरचना sched_rt_entity *rt_se, संरचना rt_rq *rt_rq)
{
	यदि (rt_se_boosted(rt_se))
		rt_rq->rt_nr_boosted++;

	start_rt_bandwidth(&rt_rq->tg->rt_bandwidth);
}

स्थिर शून्य
dec_rt_group(संरचना sched_rt_entity *rt_se, संरचना rt_rq *rt_rq)
{
	यदि (rt_se_boosted(rt_se))
		rt_rq->rt_nr_boosted--;

	WARN_ON(!rt_rq->rt_nr_running && rt_rq->rt_nr_boosted);
}

#अन्यथा /* !CONFIG_RT_GROUP_SCHED: */

स्थिर शून्य
inc_rt_group(संरचना sched_rt_entity *rt_se, संरचना rt_rq *rt_rq)
{
}

स्थिर अन्तःस्थ
शून्य dec_rt_group(संरचना sched_rt_entity *rt_se, संरचना rt_rq *rt_rq) {}

#endif /* !CONFIG_RT_GROUP_SCHED */

स्थिर अन्तःस्थ
अचिन्हित पूर्णाङ्क rt_se_nr_running(संरचना sched_rt_entity *rt_se)
{
	संरचना rt_rq *group_rq = group_rt_rq(rt_se);

	यदि (group_rq)
		फर्काऊ group_rq->rt_nr_running;
	अन्यथा
		फर्काऊ 1;
}

स्थिर अन्तःस्थ
अचिन्हित पूर्णाङ्क rt_se_rr_nr_running(संरचना sched_rt_entity *rt_se)
{
	संरचना rt_rq *group_rq = group_rt_rq(rt_se);
	संरचना task_struct *tsk;

	यदि (group_rq)
		फर्काऊ group_rq->rr_nr_running;

	tsk = rt_task_of(rt_se);

	फर्काऊ (tsk->policy == SCHED_RR) ? 1 : 0;
}

स्थिर अन्तःस्थ
शून्य inc_rt_tasks(संरचना sched_rt_entity *rt_se, संरचना rt_rq *rt_rq)
{
	पूर्णाङ्क prio = rt_se_prio(rt_se);

	WARN_ON(!rt_prio(prio));
	rt_rq->rt_nr_running += rt_se_nr_running(rt_se);
	rt_rq->rr_nr_running += rt_se_rr_nr_running(rt_se);

	inc_rt_prio(rt_rq, prio);
	inc_rt_group(rt_se, rt_rq);
}

स्थिर अन्तःस्थ
शून्य dec_rt_tasks(संरचना sched_rt_entity *rt_se, संरचना rt_rq *rt_rq)
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
स्थिर अन्तःस्थ bool move_entity(अचिन्हित पूर्णाङ्क flags)
{
	यदि ((flags & (DEQUEUE_SAVE | DEQUEUE_MOVE)) == DEQUEUE_SAVE)
		फर्काऊ false;

	फर्काऊ true;
}

स्थिर शून्य __delist_rt_entity(संरचना sched_rt_entity *rt_se, संरचना rt_prio_array *array)
{
	list_del_init(&rt_se->run_list);

	यदि (list_empty(array->queue + rt_se_prio(rt_se)))
		__clear_bit(rt_se_prio(rt_se), array->bitmap);

	rt_se->on_list = 0;
}

स्थिर अन्तःस्थ संरचना sched_statistics *
__schedstats_from_rt_se(संरचना sched_rt_entity *rt_se)
{
	/* schedstats is not supported for rt group. */
	यदि (!rt_entity_is_task(rt_se))
		फर्काऊ रिक्त;

	फर्काऊ &rt_task_of(rt_se)->stats;
}

स्थिर अन्तःस्थ शून्य
update_stats_wait_start_rt(संरचना rt_rq *rt_rq, संरचना sched_rt_entity *rt_se)
{
	संरचना sched_statistics *stats;
	संरचना task_struct *p = रिक्त;

	यदि (!schedstat_enabled())
		फर्काऊ;

	यदि (rt_entity_is_task(rt_se))
		p = rt_task_of(rt_se);

	stats = __schedstats_from_rt_se(rt_se);
	यदि (!stats)
		फर्काऊ;

	__update_stats_wait_start(rq_of_rt_rq(rt_rq), p, stats);
}

स्थिर अन्तःस्थ शून्य
update_stats_enqueue_sleeper_rt(संरचना rt_rq *rt_rq, संरचना sched_rt_entity *rt_se)
{
	संरचना sched_statistics *stats;
	संरचना task_struct *p = रिक्त;

	यदि (!schedstat_enabled())
		फर्काऊ;

	यदि (rt_entity_is_task(rt_se))
		p = rt_task_of(rt_se);

	stats = __schedstats_from_rt_se(rt_se);
	यदि (!stats)
		फर्काऊ;

	__update_stats_enqueue_sleeper(rq_of_rt_rq(rt_rq), p, stats);
}

स्थिर अन्तःस्थ शून्य
update_stats_enqueue_rt(संरचना rt_rq *rt_rq, संरचना sched_rt_entity *rt_se,
			पूर्णाङ्क flags)
{
	यदि (!schedstat_enabled())
		फर्काऊ;

	यदि (flags & ENQUEUE_WAKEUP)
		update_stats_enqueue_sleeper_rt(rt_rq, rt_se);
}

स्थिर अन्तःस्थ शून्य
update_stats_wait_end_rt(संरचना rt_rq *rt_rq, संरचना sched_rt_entity *rt_se)
{
	संरचना sched_statistics *stats;
	संरचना task_struct *p = रिक्त;

	यदि (!schedstat_enabled())
		फर्काऊ;

	यदि (rt_entity_is_task(rt_se))
		p = rt_task_of(rt_se);

	stats = __schedstats_from_rt_se(rt_se);
	यदि (!stats)
		फर्काऊ;

	__update_stats_wait_end(rq_of_rt_rq(rt_rq), p, stats);
}

स्थिर अन्तःस्थ शून्य
update_stats_dequeue_rt(संरचना rt_rq *rt_rq, संरचना sched_rt_entity *rt_se,
			पूर्णाङ्क flags)
{
	संरचना task_struct *p = रिक्त;
	संरचना rq *rq = rq_of_rt_rq(rt_rq);

	यदि (!schedstat_enabled())
		फर्काऊ;

	यदि (rt_entity_is_task(rt_se)) {
		p = rt_task_of(rt_se);

		यदि (p != rq->curr)
			update_stats_wait_end_rt(rt_rq, rt_se);
	}

	यदि ((flags & DEQUEUE_SLEEP) && p) {
		अचिन्हित पूर्णाङ्क state;

		state = READ_ONCE(p->__state);
		यदि (state & TASK_INTERRUPTIBLE)
			__schedstat_set(p->stats.sleep_start,
					rq_clock(rq_of_rt_rq(rt_rq)));

		यदि (state & TASK_UNINTERRUPTIBLE)
			__schedstat_set(p->stats.block_start,
					rq_clock(rq_of_rt_rq(rt_rq)));
	}
}

स्थिर शून्य __enqueue_rt_entity(संरचना sched_rt_entity *rt_se, अचिन्हित पूर्णाङ्क flags)
{
	संरचना rt_rq *rt_rq = rt_rq_of_se(rt_se);
	संरचना rt_prio_array *array = &rt_rq->active;
	संरचना rt_rq *group_rq = group_rt_rq(rt_se);
	संरचना list_head *queue = array->queue + rt_se_prio(rt_se);

	/*
	 * Don't enqueue the group if its throttled, or when empty.
	 * The latter is a consequence of the former when a child group
	 * get throttled and the current group doesn't have any other
	 * active members.
	 */
	यदि (group_rq && (rt_rq_throttled(group_rq) || !group_rq->rt_nr_running)) {
		यदि (rt_se->on_list)
			__delist_rt_entity(rt_se, array);
		फर्काऊ;
	}

	यदि (move_entity(flags)) {
		WARN_ON_ONCE(rt_se->on_list);
		यदि (flags & ENQUEUE_HEAD)
			list_add(&rt_se->run_list, queue);
		अन्यथा
			list_add_tail(&rt_se->run_list, queue);

		__set_bit(rt_se_prio(rt_se), array->bitmap);
		rt_se->on_list = 1;
	}
	rt_se->on_rq = 1;

	inc_rt_tasks(rt_se, rt_rq);
}

स्थिर शून्य __dequeue_rt_entity(संरचना sched_rt_entity *rt_se, अचिन्हित पूर्णाङ्क flags)
{
	संरचना rt_rq *rt_rq = rt_rq_of_se(rt_se);
	संरचना rt_prio_array *array = &rt_rq->active;

	यदि (move_entity(flags)) {
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
स्थिर शून्य dequeue_rt_थाक(संरचना sched_rt_entity *rt_se, अचिन्हित पूर्णाङ्क flags)
{
	संरचना sched_rt_entity *back = रिक्त;
	अचिन्हित पूर्णाङ्क rt_nr_running;

	for_each_sched_rt_entity(rt_se) {
		rt_se->back = back;
		back = rt_se;
	}

	rt_nr_running = rt_rq_of_se(back)->rt_nr_running;

	लागि (rt_se = back; rt_se; rt_se = rt_se->back) {
		यदि (on_rt_rq(rt_se))
			__dequeue_rt_entity(rt_se, flags);
	}

	dequeue_शीर्ष_rt_rq(rt_rq_of_se(back), rt_nr_running);
}

स्थिर शून्य enqueue_rt_entity(संरचना sched_rt_entity *rt_se, अचिन्हित पूर्णाङ्क flags)
{
	संरचना rq *rq = rq_of_rt_se(rt_se);

	update_stats_enqueue_rt(rt_rq_of_se(rt_se), rt_se, flags);

	dequeue_rt_थाक(rt_se, flags);
	for_each_sched_rt_entity(rt_se)
		__enqueue_rt_entity(rt_se, flags);
	enqueue_शीर्ष_rt_rq(&rq->rt);
}

स्थिर शून्य dequeue_rt_entity(संरचना sched_rt_entity *rt_se, अचिन्हित पूर्णाङ्क flags)
{
	संरचना rq *rq = rq_of_rt_se(rt_se);

	update_stats_dequeue_rt(rt_rq_of_se(rt_se), rt_se, flags);

	dequeue_rt_थाक(rt_se, flags);

	for_each_sched_rt_entity(rt_se) {
		संरचना rt_rq *rt_rq = group_rt_rq(rt_se);

		यदि (rt_rq && rt_rq->rt_nr_running)
			__enqueue_rt_entity(rt_se, flags);
	}
	enqueue_शीर्ष_rt_rq(&rq->rt);
}

/*
 * Adding/removing a task to/from a priority array:
 */
स्थिर शून्य
enqueue_task_rt(संरचना rq *rq, संरचना task_struct *p, पूर्णाङ्क flags)
{
	संरचना sched_rt_entity *rt_se = &p->rt;

	यदि (flags & ENQUEUE_WAKEUP)
		rt_se->timeout = 0;

	check_schedstat_required();
	update_stats_wait_start_rt(rt_rq_of_se(rt_se), rt_se);

	enqueue_rt_entity(rt_se, flags);

	यदि (task_is_blocked(p))
		फर्काऊ;

	यदि (!task_current(rq, p) && p->nr_cpus_allowed > 1)
		enqueue_pushable_task(rq, p);
}

स्थिर bool dequeue_task_rt(संरचना rq *rq, संरचना task_struct *p, पूर्णाङ्क flags)
{
	संरचना sched_rt_entity *rt_se = &p->rt;

	update_curr_rt(rq);
	dequeue_rt_entity(rt_se, flags);

	dequeue_pushable_task(rq, p);

	फर्काऊ true;
}

/*
 * Put task to the head or the end of the run list without the overhead of
 * dequeue followed by enqueue.
 */
स्थिर शून्य
requeue_rt_entity(संरचना rt_rq *rt_rq, संरचना sched_rt_entity *rt_se, पूर्णाङ्क head)
{
	यदि (on_rt_rq(rt_se)) {
		संरचना rt_prio_array *array = &rt_rq->active;
		संरचना list_head *queue = array->queue + rt_se_prio(rt_se);

		यदि (head)
			list_move(&rt_se->run_list, queue);
		अन्यथा
			list_move_tail(&rt_se->run_list, queue);
	}
}

स्थिर शून्य requeue_task_rt(संरचना rq *rq, संरचना task_struct *p, पूर्णाङ्क head)
{
	संरचना sched_rt_entity *rt_se = &p->rt;
	संरचना rt_rq *rt_rq;

	for_each_sched_rt_entity(rt_se) {
		rt_rq = rt_rq_of_se(rt_se);
		requeue_rt_entity(rt_rq, rt_se, head);
	}
}

स्थिर शून्य yield_task_rt(संरचना rq *rq)
{
	requeue_task_rt(rq, rq->donor, 0);
}

स्थिर पूर्णाङ्क find_lowest_rq(संरचना task_struct *task);

स्थिर पूर्णाङ्क
select_task_rq_rt(संरचना task_struct *p, पूर्णाङ्क cpu, पूर्णाङ्क flags)
{
	संरचना task_struct *curr, *donor;
	संरचना rq *rq;
	bool test;

	/* For anything but wake ups, just return the task_cpu */
	यदि (!(flags & (WF_TTWU | WF_FORK)))
		गच्छ out;

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

	यदि (test || !rt_task_fits_capacity(p, cpu)) {
		पूर्णाङ्क target = find_lowest_rq(p);

		/*
		 * Bail out if we were forcing a migration to find a better
		 * fitting CPU but our search failed.
		 */
		यदि (!test && target != -1 && !rt_task_fits_capacity(p, target))
			गच्छ out_unlock;

		/*
		 * Don't bother moving it if the destination CPU is
		 * not running a lower priority task.
		 */
		यदि (target != -1 &&
		    p->prio < cpu_rq(target)->rt.highest_prio.curr)
			cpu = target;
	}

out_unlock:
	rcu_read_unlock();

out:
	फर्काऊ cpu;
}

स्थिर शून्य check_preempt_equal_prio(संरचना rq *rq, संरचना task_struct *p)
{
	यदि (rq->curr->nr_cpus_allowed == 1 ||
	    !cpupri_find(&rq->rd->cpupri, rq->donor, रिक्त))
		फर्काऊ;

	/*
	 * p is migratable, so let's not schedule it and
	 * see if it is pushed or pulled somewhere else.
	 */
	यदि (p->nr_cpus_allowed != 1 &&
	    cpupri_find(&rq->rd->cpupri, p, रिक्त))
		फर्काऊ;

	/*
	 * There appear to be other CPUs that can accept
	 * the current task but none can run 'p', so lets reschedule
	 * to try and push the current task away:
	 */
	requeue_task_rt(rq, p, 1);
	resched_curr(rq);
}

स्थिर पूर्णाङ्क balance_rt(संरचना rq *rq, संरचना rq_flags *rf)
{
	/*
	 * Note, rq->donor may change during rq lock drops,
	 * so don't re-use p across lock drops
	 */
	संरचना task_struct *p = rq->donor;

	यदि (!on_rt_rq(&p->rt) && need_pull_rt_task(rq, p)) {
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

	फर्काऊ sched_stop_runnable(rq) || sched_dl_runnable(rq) || sched_rt_runnable(rq);
}

/*
 * Preempt the current task with a newly woken task if needed:
 */
स्थिर शून्य wakeup_preempt_rt(संरचना rq *rq, संरचना task_struct *p, पूर्णाङ्क flags)
{
	संरचना task_struct *donor = rq->donor;

	/*
	 * XXX If we're preempted by DL, queue a push?
	 */
	यदि (p->sched_class != &rt_sched_class ||
	    donor->sched_class != &rt_sched_class)
		फर्काऊ;

	यदि (p->prio < donor->prio) {
		resched_curr(rq);
		फर्काऊ;
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
	यदि (p->prio == donor->prio && !test_tsk_need_resched(rq->curr))
		check_preempt_equal_prio(rq, p);
}

स्थिर अन्तःस्थ शून्य set_next_task_rt(संरचना rq *rq, संरचना task_struct *p, गणना snt_e type)
{
	संरचना sched_rt_entity *rt_se = &p->rt;
	संरचना rt_rq *rt_rq = &rq->rt;

	यदि (type == SNT_REPICK)
		फर्काऊ;

	p->se.exec_start = rq_clock_task(rq);
	यदि (on_rt_rq(&p->rt))
		update_stats_wait_end_rt(rt_rq, rt_se);

	/* The running task is never eligible for pushing */
	dequeue_pushable_task(rq, p);

	यदि (type != SNT_PICK)
		फर्काऊ;

	/*
	 * If prev task was rt, put_prev_task() has already updated the
	 * utilization. We only care of the case where we start to schedule a
	 * rt task
	 */
	यदि (rq->donor->sched_class != &rt_sched_class)
		update_rt_rq_load_avg(rq_clock_pelt(rq), rq, 0);

	rt_queue_थप_tasks(rq);
}

स्थिर संरचना sched_rt_entity *pick_next_rt_entity(संरचना rt_rq *rt_rq)
{
	संरचना rt_prio_array *array = &rt_rq->active;
	संरचना sched_rt_entity *next = रिक्त;
	संरचना list_head *queue;
	पूर्णाङ्क idx;

	idx = sched_find_first_bit(array->bitmap);
	BUG_ON(idx >= MAX_RT_PRIO);

	queue = array->queue + idx;
	यदि (WARN_ON_ONCE(list_empty(queue)))
		फर्काऊ रिक्त;
	next = list_entry(queue->next, संरचना sched_rt_entity, run_list);

	फर्काऊ next;
}

स्थिर संरचना task_struct *_pick_next_task_rt(संरचना rq *rq)
{
	संरचना sched_rt_entity *rt_se;
	संरचना rt_rq *rt_rq  = &rq->rt;

	गर {
		rt_se = pick_next_rt_entity(rt_rq);
		यदि (unlikely(!rt_se))
			फर्काऊ रिक्त;
		rt_rq = group_rt_rq(rt_se);
	} जबसम्म (rt_rq);

	फर्काऊ rt_task_of(rt_se);
}

स्थिर संरचना task_struct *pick_task_rt(संरचना rq *rq, संरचना rq_flags *rf)
{
	संरचना task_struct *p;

	यदि (!sched_rt_runnable(rq))
		फर्काऊ रिक्त;

	p = _pick_next_task_rt(rq);

	फर्काऊ p;
}

स्थिर शून्य put_prev_task_rt(संरचना rq *rq, संरचना task_struct *p, संरचना task_struct *next)
{
	संरचना sched_rt_entity *rt_se = &p->rt;
	संरचना rt_rq *rt_rq = &rq->rt;

	यदि (on_rt_rq(&p->rt))
		update_stats_wait_start_rt(rt_rq, rt_se);

	update_curr_rt(rq);

	update_rt_rq_load_avg(rq_clock_pelt(rq), rq, 1);

	यदि (task_is_blocked(p))
		फर्काऊ;
	/*
	 * The previous task needs to be made eligible for pushing
	 * if it is still active
	 */
	यदि (on_rt_rq(&p->rt) && p->nr_cpus_allowed > 1)
		enqueue_pushable_task(rq, p);
}

/* Only try algorithms three times */
#define RT_MAX_TRIES 3

/*
 * Return the highest pushable rq's task, which is suitable to be executed
 * on the CPU, NULL otherwise
 */
स्थिर संरचना task_struct *pick_highest_pushable_task(संरचना rq *rq, पूर्णाङ्क cpu)
{
	संरचना plist_head *head = &rq->rt.pushable_tasks;
	संरचना task_struct *p;

	यदि (!has_pushable_tasks(rq))
		फर्काऊ रिक्त;

	plist_for_each_entry(p, head, pushable_tasks) {
		यदि (task_is_pushable(rq, p, cpu))
			फर्काऊ p;
	}

	फर्काऊ रिक्त;
}

स्थिर DEFINE_PER_CPU(cpumask_var_t, local_cpu_mask);

स्थिर पूर्णाङ्क find_lowest_rq(संरचना task_struct *task)
{
	संरचना sched_domain *sd;
	संरचना cpumask *lowest_mask = this_cpu_cpumask_var_ptr(local_cpu_mask);
	पूर्णाङ्क this_cpu = smp_processor_id();
	पूर्णाङ्क cpu      = task_cpu(task);
	पूर्णाङ्क ret;

	/* Make sure the mask is initialized first */
	यदि (unlikely(!lowest_mask))
		फर्काऊ -1;

	यदि (task->nr_cpus_allowed == 1)
		फर्काऊ -1; /* No other targets possible */

	/*
	 * If we're on asym system ensure we consider the different capacities
	 * of the CPUs when searching for the lowest_mask.
	 */
	यदि (sched_asym_cpucap_active()) {

		ret = cpupri_find_fitness(&task_rq(task)->rd->cpupri,
					  task, lowest_mask,
					  rt_task_fits_capacity);
	} अन्यथा {

		ret = cpupri_find(&task_rq(task)->rd->cpupri,
				  task, lowest_mask);
	}

	यदि (!ret)
		फर्काऊ -1; /* No targets found */

	/*
	 * At this point we have built a mask of CPUs representing the
	 * lowest priority tasks in the system.  Now we want to elect
	 * the best one based on our affinity and topology.
	 *
	 * We prioritize the last CPU that the task executed on since
	 * it is most likely cache-hot in that location.
	 */
	यदि (cpumask_test_cpu(cpu, lowest_mask))
		फर्काऊ cpu;

	/*
	 * Otherwise, we consult the sched_domains span maps to figure
	 * out which CPU is logically closest to our hot cache data.
	 */
	यदि (!cpumask_test_cpu(this_cpu, lowest_mask))
		this_cpu = -1; /* Skip this_cpu opt if not among lowest */

	rcu_read_lock();
	for_each_domain(cpu, sd) {
		यदि (sd->flags & SD_WAKE_AFFINE) {
			पूर्णाङ्क best_cpu;

			/*
			 * "this_cpu" is cheaper to preempt than a
			 * remote processor.
			 */
			यदि (this_cpu != -1 &&
			    cpumask_test_cpu(this_cpu, sched_domain_span(sd))) {
				rcu_read_unlock();
				फर्काऊ this_cpu;
			}

			best_cpu = cpumask_any_and_distribute(lowest_mask,
							      sched_domain_span(sd));
			यदि (best_cpu < nr_cpu_ids) {
				rcu_read_unlock();
				फर्काऊ best_cpu;
			}
		}
	}
	rcu_read_unlock();

	/*
	 * And finally, if there were no matches within the domains
	 * just give the caller *something* to work with from the compatible
	 * locations.
	 */
	यदि (this_cpu != -1)
		फर्काऊ this_cpu;

	cpu = cpumask_any_distribute(lowest_mask);
	यदि (cpu < nr_cpu_ids)
		फर्काऊ cpu;

	फर्काऊ -1;
}

स्थिर संरचना task_struct *pick_next_pushable_task(संरचना rq *rq)
{
	संरचना plist_head *head = &rq->rt.pushable_tasks;
	संरचना task_struct *i, *p = रिक्त;

	यदि (!has_pushable_tasks(rq))
		फर्काऊ रिक्त;

	plist_for_each_entry(i, head, pushable_tasks) {
		/* skip tasks that cannot be migrated */
		यदि (!task_on_cpu(rq, i) && !is_migration_disabled(i)) {
			p = i;
			रोक;
		}
	}

	यदि (!p)
		फर्काऊ रिक्त;

	BUG_ON(rq->cpu != task_cpu(p));
	BUG_ON(task_current(rq, p));
	BUG_ON(task_current_donor(rq, p));
	BUG_ON(p->nr_cpus_allowed <= 1);

	BUG_ON(!task_on_rq_queued(p));
	BUG_ON(!rt_task(p));

	फर्काऊ p;
}

/* Will lock the rq it finds */
स्थिर संरचना rq *find_lock_lowest_rq(संरचना task_struct *task, संरचना rq *rq)
{
	संरचना rq *lowest_rq = रिक्त;
	पूर्णाङ्क tries;
	पूर्णाङ्क cpu;

	लागि (tries = 0; tries < RT_MAX_TRIES; tries++) {
		cpu = find_lowest_rq(task);

		यदि ((cpu == -1) || (cpu == rq->cpu))
			रोक;

		lowest_rq = cpu_rq(cpu);

		यदि (lowest_rq->rt.highest_prio.curr <= task->prio) {
			/*
			 * Target rq has tasks of equal or higher priority,
			 * retrying does not release any lock and is unlikely
			 * to yield a different result.
			 */
			lowest_rq = रिक्त;
			रोक;
		}

		/* if the prio of this runqueue changed, try again */
		यदि (double_lock_balance(rq, lowest_rq)) {
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
			यदि (unlikely(is_migration_disabled(task) ||
				     !cpumask_test_cpu(lowest_rq->cpu, &task->cpus_mask) ||
				     task != pick_next_pushable_task(rq))) {

				double_unlock_balance(rq, lowest_rq);
				lowest_rq = रिक्त;
				रोक;
			}
		}

		/* If this rq is still suitable use it. */
		यदि (lowest_rq->rt.highest_prio.curr > task->prio)
			रोक;

		/* try again */
		double_unlock_balance(rq, lowest_rq);
		lowest_rq = रिक्त;
	}

	फर्काऊ lowest_rq;
}

/*
 * If the current CPU has more than one RT task, see if the non
 * running task can migrate over to a CPU that is running a task
 * of lesser priority.
 */
स्थिर पूर्णाङ्क थप_rt_task(संरचना rq *rq, bool pull)
{
	संरचना task_struct *next_task;
	संरचना rq *lowest_rq;
	पूर्णाङ्क ret = 0;

	यदि (!rq->rt.overloaded)
		फर्काऊ 0;

	next_task = pick_next_pushable_task(rq);
	यदि (!next_task)
		फर्काऊ 0;

retry:
	/*
	 * It's possible that the next_task slipped in of
	 * higher priority than current. If that's the case
	 * just reschedule current.
	 */
	यदि (unlikely(next_task->prio < rq->donor->prio)) {
		resched_curr(rq);
		फर्काऊ 0;
	}

	यदि (is_migration_disabled(next_task)) {
		संरचना task_struct *थप_task = रिक्त;
		पूर्णाङ्क cpu;

		यदि (!pull || rq->थप_busy)
			फर्काऊ 0;

		/*
		 * Invoking find_lowest_rq() on anything but an RT task doesn't
		 * make sense. Per the above priority check, curr has to
		 * be of higher priority than next_task, so no need to
		 * reschedule when bailing out.
		 *
		 * Note that the stoppers are masqueraded as SCHED_FIFO
		 * (cf. sched_set_stop_task()), so we can't rely on rt_task().
		 */
		यदि (rq->donor->sched_class != &rt_sched_class)
			फर्काऊ 0;

		cpu = find_lowest_rq(rq->curr);
		यदि (cpu == -1 || cpu == rq->cpu)
			फर्काऊ 0;

		/*
		 * Given we found a CPU with lower priority than @next_task,
		 * therefore it should be running. However we cannot migrate it
		 * to this other CPU, instead attempt to push the current
		 * running task on this CPU away.
		 */
		थप_task = get_थप_task(rq);
		यदि (थप_task) {
			preempt_disable();
			raw_spin_rq_unlock(rq);
			stop_one_cpu_nowait(rq->cpu, थप_cpu_stop,
					    थप_task, &rq->थप_work);
			preempt_enable();
			raw_spin_rq_lock(rq);
		}

		फर्काऊ 0;
	}

	यदि (WARN_ON(next_task == rq->curr))
		फर्काऊ 0;

	/* We might release rq lock */
	get_task_struct(next_task);

	/* find_lock_lowest_rq locks the rq if found */
	lowest_rq = find_lock_lowest_rq(next_task, rq);
	यदि (!lowest_rq) {
		संरचना task_struct *task;
		/*
		 * find_lock_lowest_rq releases rq->lock
		 * so it is possible that next_task has migrated.
		 *
		 * We need to make sure that the task is still on the same
		 * run-queue and is also still the next task eligible for
		 * pushing.
		 */
		task = pick_next_pushable_task(rq);
		यदि (task == next_task) {
			/*
			 * The task hasn't migrated, and is still the next
			 * eligible task, but we failed to find a run-queue
			 * to push it to.  Do not retry in this case, since
			 * other CPUs will pull from us when ready.
			 */
			गच्छ out;
		}

		यदि (!task)
			/* No more tasks, just exit */
			गच्छ out;

		/*
		 * Something has shifted, try again.
		 */
		put_task_struct(next_task);
		next_task = task;
		गच्छ retry;
	}

	move_queued_task_locked(rq, lowest_rq, next_task);
	resched_curr(lowest_rq);
	ret = 1;

	double_unlock_balance(rq, lowest_rq);
out:
	put_task_struct(next_task);

	फर्काऊ ret;
}

स्थिर शून्य थप_rt_tasks(संरचना rq *rq)
{
	/* push_rt_task will return true if it moved an RT */
	जबसम्म (थप_rt_task(rq, false))
		;
}

#ifdef HAVE_RT_थप_IPI

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
स्थिर पूर्णाङ्क rto_next_cpu(संरचना जरा_domain *rd)
{
	पूर्णाङ्क this_cpu = smp_processor_id();
	पूर्णाङ्क next;
	पूर्णाङ्क cpu;

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
	लागि (;;) {

		/* When rto_cpu is -1 this acts like cpumask_first() */
		cpu = cpumask_next(rd->rto_cpu, rd->rto_mask);

		rd->rto_cpu = cpu;

		/* Do not send IPI to self */
		यदि (cpu == this_cpu)
			जारी;

		यदि (cpu < nr_cpu_ids)
			फर्काऊ cpu;

		rd->rto_cpu = -1;

		/*
		 * ACQUIRE ensures we see the @rto_mask changes
		 * made prior to the @next value observed.
		 *
		 * Matches WMB in rt_set_overload().
		 */
		next = atomic_read_acquire(&rd->rto_loop_next);

		यदि (rd->rto_loop == next)
			रोक;

		rd->rto_loop = next;
	}

	फर्काऊ -1;
}

स्थिर अन्तःस्थ bool rto_start_trylock(atomic_t *v)
{
	फर्काऊ !atomic_cmpxchg_acquire(v, 0, 1);
}

स्थिर अन्तःस्थ शून्य rto_start_unlock(atomic_t *v)
{
	atomic_set_release(v, 0);
}

स्थिर शून्य tell_cpu_to_थप(संरचना rq *rq)
{
	पूर्णाङ्क cpu = -1;

	/* Keep the loop going if the IPI is currently active */
	atomic_inc(&rq->rd->rto_loop_next);

	/* Only one CPU can initiate a loop at a time */
	यदि (!rto_start_trylock(&rq->rd->rto_loop_start))
		फर्काऊ;

	raw_spin_lock(&rq->rd->rto_lock);

	/*
	 * The rto_cpu is updated under the lock, if it has a valid CPU
	 * then the IPI is still running and will continue due to the
	 * update to loop_next, and nothing needs to be done here.
	 * Otherwise it is finishing up and an IPI needs to be sent.
	 */
	यदि (rq->rd->rto_cpu < 0)
		cpu = rto_next_cpu(rq->rd);

	raw_spin_unlock(&rq->rd->rto_lock);

	rto_start_unlock(&rq->rd->rto_loop_start);

	यदि (cpu >= 0) {
		/* Make sure the rd does not get freed while pushing */
		sched_get_rd(rq->rd);
		irq_work_queue_on(&rq->rd->rto_थप_work, cpu);
	}
}

/* Called from hardirq context */
शून्य rto_थप_irq_work_func(संरचना irq_work *work)
{
	संरचना जरा_domain *rd =
		container_of(work, संरचना जरा_domain, rto_थप_work);
	संरचना rq *rq;
	पूर्णाङ्क cpu;

	rq = this_rq();

	/*
	 * We do not need to grab the lock to check for has_pushable_tasks.
	 * When it gets updated, a check is made if a push is possible.
	 */
	यदि (has_pushable_tasks(rq)) {
		raw_spin_rq_lock(rq);
		जबसम्म (थप_rt_task(rq, true))
			;
		raw_spin_rq_unlock(rq);
	}

	raw_spin_lock(&rd->rto_lock);

	/* Pass the IPI to the next rt overloaded queue */
	cpu = rto_next_cpu(rd);

	raw_spin_unlock(&rd->rto_lock);

	यदि (cpu < 0) {
		sched_put_rd(rd);
		फर्काऊ;
	}

	/* Try the next RT overloaded CPU */
	irq_work_queue_on(&rd->rto_थप_work, cpu);
}
#endif /* HAVE_RT_PUSH_IPI */

स्थिर शून्य pull_rt_task(संरचना rq *this_rq)
{
	पूर्णाङ्क this_cpu = this_rq->cpu, cpu;
	bool resched = false;
	संरचना task_struct *p, *थप_task;
	संरचना rq *src_rq;
	पूर्णाङ्क rt_overload_गन्ती = rt_overloaded(this_rq);

	यदि (likely(!rt_overload_गन्ती))
		फर्काऊ;

	/*
	 * Match the barrier from rt_set_overloaded; this guarantees that if we
	 * see overloaded we must also see the rto_mask bit.
	 */
	smp_rmb();

	/* If we are the only overloaded CPU do nothing */
	यदि (rt_overload_गन्ती == 1 &&
	    cpumask_test_cpu(this_rq->cpu, this_rq->rd->rto_mask))
		फर्काऊ;

#ifdef HAVE_RT_थप_IPI
	यदि (sched_feat(RT_थप_IPI)) {
		tell_cpu_to_थप(this_rq);
		फर्काऊ;
	}
#endif

	for_each_cpu(cpu, this_rq->rd->rto_mask) {
		यदि (this_cpu == cpu)
			जारी;

		src_rq = cpu_rq(cpu);

		/*
		 * Don't bother taking the src_rq->lock if the next highest
		 * task is known to be lower-priority than our current task.
		 * This may look racy, but if this value is about to go
		 * logically higher, the src_rq will push this task away.
		 * And if its going logically lower, we do not care
		 */
		यदि (src_rq->rt.highest_prio.next >=
		    this_rq->rt.highest_prio.curr)
			जारी;

		/*
		 * We can potentially drop this_rq's lock in
		 * double_lock_balance, and another CPU could
		 * alter this_rq
		 */
		थप_task = रिक्त;
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
		यदि (p && (p->prio < this_rq->rt.highest_prio.curr)) {
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
			यदि (p->prio < src_rq->donor->prio)
				गच्छ skip;

			यदि (is_migration_disabled(p)) {
				थप_task = get_थप_task(src_rq);
			} अन्यथा {
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

		यदि (थप_task) {
			preempt_disable();
			raw_spin_rq_unlock(this_rq);
			stop_one_cpu_nowait(src_rq->cpu, थप_cpu_stop,
					    थप_task, &src_rq->थप_work);
			preempt_enable();
			raw_spin_rq_lock(this_rq);
		}
	}

	यदि (resched)
		resched_curr(this_rq);
}

/*
 * If we are not running and we are not going to reschedule soon, we should
 * try to push tasks away now
 */
स्थिर शून्य task_woken_rt(संरचना rq *rq, संरचना task_struct *p)
{
	bool need_to_थप = !task_on_cpu(rq, p) &&
			    !test_tsk_need_resched(rq->curr) &&
			    p->nr_cpus_allowed > 1 &&
			    (dl_task(rq->donor) || rt_task(rq->donor)) &&
			    (rq->curr->nr_cpus_allowed < 2 ||
			     rq->donor->prio <= p->prio);

	यदि (need_to_थप)
		थप_rt_tasks(rq);
}

/* Assumes rq->lock is held */
स्थिर शून्य rq_online_rt(संरचना rq *rq)
{
	यदि (rq->rt.overloaded)
		rt_set_overload(rq);

	__enable_runtime(rq);

	cpupri_set(&rq->rd->cpupri, rq->cpu, rq->rt.highest_prio.curr);
}

/* Assumes rq->lock is held */
स्थिर शून्य rq_offline_rt(संरचना rq *rq)
{
	यदि (rq->rt.overloaded)
		rt_clear_overload(rq);

	__disable_runtime(rq);

	cpupri_set(&rq->rd->cpupri, rq->cpu, CPUPRI_INVALID);
}

/*
 * When switch from the rt queue, we bring ourselves to a position
 * that we might want to pull RT tasks from other runqueues.
 */
स्थिर शून्य switched_from_rt(संरचना rq *rq, संरचना task_struct *p)
{
	/*
	 * If there are other RT tasks then we will reschedule
	 * and the scheduling of the other RT tasks will handle
	 * the balancing. But if we are the last RT task
	 * we may need to handle the pulling of RT tasks
	 * now.
	 */
	यदि (!task_on_rq_queued(p) || rq->rt.rt_nr_running)
		फर्काऊ;

	rt_queue_pull_task(rq);
}

शून्य __init init_sched_rt_class(शून्य)
{
	अचिन्हित पूर्णाङ्क i;

	for_each_possible_cpu(i) {
		zalloc_cpumask_var_गाँठो(&per_cpu(local_cpu_mask, i),
					GFP_KERNEL, cpu_to_गाँठो(i));
	}
}

/*
 * When switching a task to RT, we may overload the runqueue
 * with RT tasks. In this case we try to push them off to
 * other runqueues.
 */
स्थिर शून्य switched_to_rt(संरचना rq *rq, संरचना task_struct *p)
{
	/*
	 * If we are running, update the avg_rt tracking, as the running time
	 * will now on be accounted into the latter.
	 */
	यदि (task_current(rq, p)) {
		update_rt_rq_load_avg(rq_clock_pelt(rq), rq, 0);
		फर्काऊ;
	}

	/*
	 * If we are not running we may need to preempt the current
	 * running task. If that current running task is also an RT task
	 * then see if we can move to another run queue.
	 */
	यदि (task_on_rq_queued(p)) {
		यदि (p->nr_cpus_allowed > 1 && rq->rt.overloaded)
			rt_queue_थप_tasks(rq);
		यदि (p->prio < rq->donor->prio && cpu_online(cpu_of(rq)))
			resched_curr(rq);
	}
}

/*
 * Priority of the task has changed. This may cause
 * us to initiate a push or pull.
 */
स्थिर शून्य
prio_changed_rt(संरचना rq *rq, संरचना task_struct *p, u64 oldprio)
{
	यदि (!task_on_rq_queued(p))
		फर्काऊ;

	यदि (p->prio == oldprio)
		फर्काऊ;

	यदि (task_current_donor(rq, p)) {
		/*
		 * If our priority decreases while running, we
		 * may need to pull tasks to this runqueue.
		 */
		यदि (oldprio < p->prio)
			rt_queue_pull_task(rq);

		/*
		 * If there's a higher priority task waiting to run
		 * then reschedule.
		 */
		यदि (p->prio > rq->rt.highest_prio.curr)
			resched_curr(rq);
	} अन्यथा {
		/*
		 * This task is not running, but if it is
		 * greater than the current running task
		 * then reschedule.
		 */
		यदि (p->prio < rq->donor->prio)
			resched_curr(rq);
	}
}

#ifdef CONFIG_POSIX_TIMERS
स्थिर शून्य watchdog(संरचना rq *rq, संरचना task_struct *p)
{
	अचिन्हित दीर्घ soft, hard;

	/* max may change after cur was read, this will be fixed next tick */
	soft = task_rlimit(p, RLIMIT_RTTIME);
	hard = task_rlimit_max(p, RLIMIT_RTTIME);

	यदि (soft != RLIM_INFINITY) {
		अचिन्हित दीर्घ next;

		यदि (p->rt.watchdog_stamp != jiffies) {
			p->rt.timeout++;
			p->rt.watchdog_stamp = jiffies;
		}

		next = DIV_ROUND_UP(min(soft, hard), USEC_PER_SEC/HZ);
		यदि (p->rt.timeout > next) {
			posix_cputimers_rt_watchdog(&p->posix_cputimers,
						    p->se.sum_exec_runtime);
		}
	}
}
#अन्यथा /* !CONFIG_POSIX_TIMERS: */
स्थिर अन्तःस्थ शून्य watchdog(संरचना rq *rq, संरचना task_struct *p) { }
#endif /* !CONFIG_POSIX_TIMERS */

/*
 * scheduler tick hitting a task of our scheduling class.
 *
 * NOTE: This function can be called remotely by the tick offload that
 * goes along full dynticks. Therefore no local assumption can be made
 * and everything must be accessed through the @rq and @curr passed in
 * parameters.
 */
स्थिर शून्य task_tick_rt(संरचना rq *rq, संरचना task_struct *p, पूर्णाङ्क queued)
{
	संरचना sched_rt_entity *rt_se = &p->rt;

	update_curr_rt(rq);
	update_rt_rq_load_avg(rq_clock_pelt(rq), rq, 1);

	watchdog(rq, p);

	/*
	 * RR tasks need a special form of time-slice management.
	 * FIFO tasks have no timeslices.
	 */
	यदि (p->policy != SCHED_RR)
		फर्काऊ;

	यदि (--p->rt.time_slice)
		फर्काऊ;

	p->rt.time_slice = sched_rr_timeslice;

	/*
	 * Requeue to the end of queue if we (and all of our ancestors) are not
	 * the only element on the queue
	 */
	for_each_sched_rt_entity(rt_se) {
		यदि (rt_se->run_list.prev != rt_se->run_list.next) {
			requeue_task_rt(rq, p, 0);
			resched_curr(rq);
			फर्काऊ;
		}
	}
}

स्थिर अचिन्हित पूर्णाङ्क get_rr_interval_rt(संरचना rq *rq, संरचना task_struct *task)
{
	/*
	 * Time slice is 0 for SCHED_FIFO tasks
	 */
	यदि (task->policy == SCHED_RR)
		फर्काऊ sched_rr_timeslice;
	अन्यथा
		फर्काऊ 0;
}

#ifdef CONFIG_SCHED_CORE
स्थिर पूर्णाङ्क task_is_throttled_rt(संरचना task_struct *p, पूर्णाङ्क cpu)
{
	संरचना rt_rq *rt_rq;

#ifdef CONFIG_RT_GROUP_SCHED // XXX maybe add task_rt_rq(), see also sched_rt_period_rt_rq
	rt_rq = task_group(p)->rt_rq[cpu];
	WARN_ON(!rt_group_sched_enabled() && rt_rq->tg != &जरा_task_group);
#अन्यथा
	rt_rq = &cpu_rq(cpu)->rt;
#endif

	फर्काऊ rt_rq_throttled(rt_rq);
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
स्थिर DEFINE_MUTEX(rt_constraints_mutex);

स्थिर अन्तःस्थ पूर्णाङ्क tg_has_rt_tasks(संरचना task_group *tg)
{
	संरचना task_struct *task;
	संरचना css_task_iter it;
	पूर्णाङ्क ret = 0;

	/*
	 * Autogroups do not have RT tasks; see autogroup_create().
	 */
	यदि (task_group_is_autogroup(tg))
		फर्काऊ 0;

	css_task_iter_start(&tg->css, 0, &it);
	जबसम्म (!ret && (task = css_task_iter_next(&it)))
		ret |= rt_task(task);
	css_task_iter_end(&it);

	फर्काऊ ret;
}

संरचना rt_schedulable_data {
	संरचना task_group *tg;
	u64 rt_period;
	u64 rt_runtime;
};

स्थिर पूर्णाङ्क tg_rt_schedulable(संरचना task_group *tg, शून्य *data)
{
	संरचना rt_schedulable_data *d = data;
	संरचना task_group *child;
	u64 total, sum = 0;
	u64 period, runtime;

	period = ktime_to_ns(tg->rt_bandwidth.rt_period);
	runtime = tg->rt_bandwidth.rt_runtime;

	यदि (tg == d->tg) {
		period = d->rt_period;
		runtime = d->rt_runtime;
	}

	/*
	 * Cannot have more runtime than the period.
	 */
	यदि (runtime > period && runtime != RUNTIME_INF)
		फर्काऊ -EINVAL;

	/*
	 * Ensure we don't starve existing RT tasks if runtime turns zero.
	 */
	यदि (rt_bandwidth_enabled() && !runtime &&
	    tg->rt_bandwidth.rt_runtime && tg_has_rt_tasks(tg))
		फर्काऊ -EBUSY;

	total = to_ratio(period, runtime);

	/*
	 * Nobody can have more than the global setting allows.
	 */
	यदि (total > to_ratio(global_rt_period(), global_rt_runtime()))
		फर्काऊ -EINVAL;

	/*
	 * The sum of our children's runtime should not exceed our own.
	 */
	list_for_each_entry_rcu(child, &tg->children, siblings) {
		period = ktime_to_ns(child->rt_bandwidth.rt_period);
		runtime = child->rt_bandwidth.rt_runtime;

		यदि (child == d->tg) {
			period = d->rt_period;
			runtime = d->rt_runtime;
		}

		sum += to_ratio(period, runtime);
	}

	यदि (sum > total)
		फर्काऊ -EINVAL;

	फर्काऊ 0;
}

स्थिर पूर्णाङ्क __rt_schedulable(संरचना task_group *tg, u64 period, u64 runtime)
{
	पूर्णाङ्क ret;

	संरचना rt_schedulable_data data = {
		.tg = tg,
		.rt_period = period,
		.rt_runtime = runtime,
	};

	rcu_read_lock();
	ret = walk_tg_रूख(tg_rt_schedulable, tg_nop, &data);
	rcu_read_unlock();

	फर्काऊ ret;
}

स्थिर पूर्णाङ्क tg_set_rt_bandwidth(संरचना task_group *tg,
		u64 rt_period, u64 rt_runtime)
{
	पूर्णाङ्क i, err = 0;

	/*
	 * Disallowing the root group RT runtime is BAD, it would disallow the
	 * kernel creating (and or operating) RT threads.
	 */
	यदि (tg == &जरा_task_group && rt_runtime == 0)
		फर्काऊ -EINVAL;

	/* No period doesn't make any sense. */
	यदि (rt_period == 0)
		फर्काऊ -EINVAL;

	/*
	 * Bound quota to defend quota against overflow during bandwidth shift.
	 */
	यदि (rt_runtime != RUNTIME_INF && rt_runtime > max_rt_runtime)
		फर्काऊ -EINVAL;

	mutex_lock(&rt_constraints_mutex);
	err = __rt_schedulable(tg, rt_period, rt_runtime);
	यदि (err)
		गच्छ unlock;

	raw_spin_lock_irq(&tg->rt_bandwidth.rt_runtime_lock);
	tg->rt_bandwidth.rt_period = ns_to_ktime(rt_period);
	tg->rt_bandwidth.rt_runtime = rt_runtime;

	for_each_possible_cpu(i) {
		संरचना rt_rq *rt_rq = tg->rt_rq[i];

		raw_spin_lock(&rt_rq->rt_runtime_lock);
		rt_rq->rt_runtime = rt_runtime;
		raw_spin_unlock(&rt_rq->rt_runtime_lock);
	}
	raw_spin_unlock_irq(&tg->rt_bandwidth.rt_runtime_lock);
unlock:
	mutex_unlock(&rt_constraints_mutex);

	फर्काऊ err;
}

पूर्णाङ्क sched_group_set_rt_runtime(संरचना task_group *tg, दीर्घ rt_runtime_us)
{
	u64 rt_runtime, rt_period;

	rt_period = ktime_to_ns(tg->rt_bandwidth.rt_period);
	rt_runtime = (u64)rt_runtime_us * NSEC_PER_USEC;
	यदि (rt_runtime_us < 0)
		rt_runtime = RUNTIME_INF;
	अन्यथा यदि ((u64)rt_runtime_us > U64_MAX / NSEC_PER_USEC)
		फर्काऊ -EINVAL;

	फर्काऊ tg_set_rt_bandwidth(tg, rt_period, rt_runtime);
}

दीर्घ sched_group_rt_runtime(संरचना task_group *tg)
{
	u64 rt_runtime_us;

	यदि (tg->rt_bandwidth.rt_runtime == RUNTIME_INF)
		फर्काऊ -1;

	rt_runtime_us = tg->rt_bandwidth.rt_runtime;
	do_div(rt_runtime_us, NSEC_PER_USEC);
	फर्काऊ rt_runtime_us;
}

पूर्णाङ्क sched_group_set_rt_period(संरचना task_group *tg, u64 rt_period_us)
{
	u64 rt_runtime, rt_period;

	यदि (rt_period_us > U64_MAX / NSEC_PER_USEC)
		फर्काऊ -EINVAL;

	rt_period = rt_period_us * NSEC_PER_USEC;
	rt_runtime = tg->rt_bandwidth.rt_runtime;

	फर्काऊ tg_set_rt_bandwidth(tg, rt_period, rt_runtime);
}

दीर्घ sched_group_rt_period(संरचना task_group *tg)
{
	u64 rt_period_us;

	rt_period_us = ktime_to_ns(tg->rt_bandwidth.rt_period);
	do_div(rt_period_us, NSEC_PER_USEC);
	फर्काऊ rt_period_us;
}

पूर्णाङ्क sched_rt_can_attach(संरचना task_group *tg, संरचना task_struct *tsk)
{
	/* Don't accept real-time tasks when there is no way for them to run */
	यदि (rt_group_sched_enabled() && rt_task(tsk) && tg->rt_bandwidth.rt_runtime == 0)
		फर्काऊ 0;

	फर्काऊ 1;
}

#endif /* !CONFIG_RT_GROUP_SCHED */

#ifdef CONFIG_SYSCTL
स्थिर पूर्णाङ्क sched_rt_global_validate(शून्य)
{
	यदि ((sysctl_sched_rt_runtime != RUNTIME_INF) &&
		((sysctl_sched_rt_runtime > sysctl_sched_rt_period) ||
		 ((u64)sysctl_sched_rt_runtime *
			NSEC_PER_USEC > max_rt_runtime)))
		फर्काऊ -EINVAL;

#ifdef CONFIG_RT_GROUP_SCHED
	यदि (!rt_group_sched_enabled())
		फर्काऊ 0;

	scoped_guard(mutex, &rt_constraints_mutex)
		फर्काऊ __rt_schedulable(रिक्त, 0, 0);
#endif
	फर्काऊ 0;
}

स्थिर पूर्णाङ्क sched_rt_handler(अचर संरचना ctl_table *table, पूर्णाङ्क लिख, शून्य *buffer,
		माप_प्रकार *lenp, loff_t *ppos)
{
	पूर्णाङ्क old_period, old_runtime;
	स्थिर DEFINE_MUTEX(mutex);
	पूर्णाङ्क ret;

	mutex_lock(&mutex);
	sched_domains_mutex_lock();
	old_period = sysctl_sched_rt_period;
	old_runtime = sysctl_sched_rt_runtime;

	ret = proc_dointvec_minmax(table, लिख, buffer, lenp, ppos);

	यदि (!ret && लिख) {
		ret = sched_rt_global_validate();
		यदि (ret)
			गच्छ undo;

		ret = sched_dl_global_validate();
		यदि (ret)
			गच्छ undo;

		sched_dl_do_global();
	}
	यदि (0) {
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

	फर्काऊ ret;
}

स्थिर पूर्णाङ्क sched_rr_handler(अचर संरचना ctl_table *table, पूर्णाङ्क लिख, शून्य *buffer,
		माप_प्रकार *lenp, loff_t *ppos)
{
	पूर्णाङ्क ret;
	स्थिर DEFINE_MUTEX(mutex);

	mutex_lock(&mutex);
	ret = proc_dointvec(table, लिख, buffer, lenp, ppos);
	/*
	 * Make sure that internally we keep jiffies.
	 * Also, writing zero resets the time-slice to default:
	 */
	यदि (!ret && लिख) {
		sched_rr_timeslice =
			sysctl_sched_rr_timeslice <= 0 ? RR_TIMESLICE :
			msecs_to_jiffies(sysctl_sched_rr_timeslice);

		यदि (sysctl_sched_rr_timeslice <= 0)
			sysctl_sched_rr_timeslice = jiffies_to_msecs(RR_TIMESLICE);
	}
	mutex_unlock(&mutex);

	फर्काऊ ret;
}
#endif /* CONFIG_SYSCTL */

शून्य print_rt_stats(संरचना seq_file *m, पूर्णाङ्क cpu)
{
	rt_rq_iter_t iter;
	संरचना rt_rq *rt_rq;

	rcu_read_lock();
	for_each_rt_rq(rt_rq, iter, cpu_rq(cpu))
		print_rt_rq(m, cpu, rt_rq);
	rcu_read_unlock();
}
