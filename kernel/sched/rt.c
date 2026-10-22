// SPDX-License-Identifier: GPL-2.0
/*
 * Real-Time Scheduling Class (mapped to the SCHED_FIFO and SCHED_RR
 * policies)
 */

#समावेश "sched.h"
#समावेश "pelt.h"

पूर्णांक sched_rr_timeslice = RR_TIMESLICE;
/* More than 4 hours if BW_SHIFT equals 20. */
अटल स्थिर u64 max_rt_runtime = MAX_BW;

/*
 * period over which we measure -rt task CPU usage in us.
 * default: 1s
 */
पूर्णांक sysctl_sched_rt_period = 1000000;

/*
 * part of the period that we allow rt tasks to run in us.
 * default: 1s
 */
पूर्णांक sysctl_sched_rt_runtime = 1000000;

#अगर_घोषित CONFIG_SYSCTL
अटल पूर्णांक sysctl_sched_rr_timeslice = (MSEC_PER_SEC * RR_TIMESLICE) / HZ;
अटल पूर्णांक sched_rt_handler(स्थिर काष्ठा ctl_table *table, पूर्णांक ग_लिखो, व्योम *बफर,
		माप_प्रकार *lenp, loff_t *ppos);
अटल पूर्णांक sched_rr_handler(स्थिर काष्ठा ctl_table *table, पूर्णांक ग_लिखो, व्योम *बफर,
		माप_प्रकार *lenp, loff_t *ppos);
अटल स्थिर काष्ठा ctl_table sched_rt_sysctls[] = {
	{
		.procname       = "sched_rt_period_us",
		.तथ्य           = &sysctl_sched_rt_period,
		.maxlen         = माप(पूर्णांक),
		.mode           = 0644,
		.proc_handler   = sched_rt_handler,
		.extra1         = SYSCTL_ONE,
		.extra2         = SYSCTL_INT_MAX,
	},
	{
		.procname       = "sched_rt_runtime_us",
		.तथ्य           = &sysctl_sched_rt_runtime,
		.maxlen         = माप(पूर्णांक),
		.mode           = 0644,
		.proc_handler   = sched_rt_handler,
		.extra1         = SYSCTL_NEG_ONE,
		.extra2         = (व्योम *)&sysctl_sched_rt_period,
	},
	{
		.procname       = "sched_rr_timeslice_ms",
		.तथ्य           = &sysctl_sched_rr_timeslice,
		.maxlen         = माप(पूर्णांक),
		.mode           = 0644,
		.proc_handler   = sched_rr_handler,
	},
};

अटल पूर्णांक __init sched_rt_sysctl_init(व्योम)
{
	register_sysctl_init("kernel", sched_rt_sysctls);
	वापस 0;
}
late_initcall(sched_rt_sysctl_init);
#पूर्ण_अगर /* CONFIG_SYSCTL */

व्योम init_rt_rq(काष्ठा rt_rq *rt_rq)
{
	काष्ठा rt_prio_array *array;
	पूर्णांक i;

	array = &rt_rq->active;
	क्रम (i = 0; i < MAX_RT_PRIO; i++) {
		INIT_LIST_शीर्ष(array->queue + i);
		__clear_bit(i, array->bitmap);
	}
	/* delimiter for bitsearch: */
	__set_bit(MAX_RT_PRIO, array->bitmap);

	rt_rq->highest_prio.curr = MAX_RT_PRIO-1;
	rt_rq->highest_prio.अग्र = MAX_RT_PRIO-1;
	rt_rq->overloaded = 0;
	plist_शीर्ष_init(&rt_rq->pushable_tasks);
	/* We start is dequeued state, because no RT tasks are queued */
	rt_rq->rt_queued = 0;

#अगर_घोषित CONFIG_RT_GROUP_SCHED
	rt_rq->rt_time = 0;
	rt_rq->rt_throttled = 0;
	rt_rq->rt_runtime = 0;
	raw_spin_lock_init(&rt_rq->rt_runtime_lock);
	rt_rq->tg = &मूल_task_group;
#पूर्ण_अगर
}

#अगर_घोषित CONFIG_RT_GROUP_SCHED

अटल पूर्णांक do_sched_rt_period_timer(काष्ठा rt_bandwidth *rt_b, पूर्णांक overrun);

अटल क्रमागत hrtimer_restart sched_rt_period_timer(काष्ठा hrtimer *timer)
{
	काष्ठा rt_bandwidth *rt_b =
		container_of(timer, काष्ठा rt_bandwidth, rt_period_timer);
	पूर्णांक idle = 0;
	पूर्णांक overrun;

	raw_spin_lock(&rt_b->rt_runtime_lock);
	क्रम (;;) {
		overrun = hrtimer_forward_now(timer, rt_b->rt_period);
		अगर (!overrun)
			अवरोध;

		raw_spin_unlock(&rt_b->rt_runtime_lock);
		idle = do_sched_rt_period_timer(rt_b, overrun);
		raw_spin_lock(&rt_b->rt_runtime_lock);
	}
	अगर (idle)
		rt_b->rt_period_active = 0;
	raw_spin_unlock(&rt_b->rt_runtime_lock);

	वापस idle ? HRTIMER_NORESTART : HRTIMER_RESTART;
}

व्योम init_rt_bandwidth(काष्ठा rt_bandwidth *rt_b, u64 period, u64 runtime)
{
	rt_b->rt_period = ns_to_ktime(period);
	rt_b->rt_runtime = runtime;

	raw_spin_lock_init(&rt_b->rt_runtime_lock);

	hrtimer_setup(&rt_b->rt_period_timer, sched_rt_period_timer, CLOCK_MONOTONIC,
		      HRTIMER_MODE_REL_HARD);
}

अटल अंतरभूत व्योम do_start_rt_bandwidth(काष्ठा rt_bandwidth *rt_b)
{
	raw_spin_lock(&rt_b->rt_runtime_lock);
	अगर (!rt_b->rt_period_active) {
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

अटल व्योम start_rt_bandwidth(काष्ठा rt_bandwidth *rt_b)
{
	अगर (!rt_bandwidth_enabled() || rt_b->rt_runtime == RUNTIME_INF)
		वापस;

	do_start_rt_bandwidth(rt_b);
}

अटल व्योम destroy_rt_bandwidth(काष्ठा rt_bandwidth *rt_b)
{
	hrtimer_cancel(&rt_b->rt_period_timer);
}

#घोषणा rt_entity_is_task(rt_se) (!(rt_se)->my_q)

अटल अंतरभूत काष्ठा task_struct *rt_task_of(काष्ठा sched_rt_entity *rt_se)
{
	WARN_ON_ONCE(!rt_entity_is_task(rt_se));

	वापस container_of(rt_se, काष्ठा task_struct, rt);
}

अटल अंतरभूत काष्ठा rq *rq_of_rt_rq(काष्ठा rt_rq *rt_rq)
{
	/* Cannot fold with non-CONFIG_RT_GROUP_SCHED version, layout */
	WARN_ON(!rt_group_sched_enabled() && rt_rq->tg != &मूल_task_group);
	वापस rt_rq->rq;
}

अटल अंतरभूत काष्ठा rt_rq *rt_rq_of_se(काष्ठा sched_rt_entity *rt_se)
{
	WARN_ON(!rt_group_sched_enabled() && rt_se->rt_rq->tg != &मूल_task_group);
	वापस rt_se->rt_rq;
}

अटल अंतरभूत काष्ठा rq *rq_of_rt_se(काष्ठा sched_rt_entity *rt_se)
{
	काष्ठा rt_rq *rt_rq = rt_se->rt_rq;

	WARN_ON(!rt_group_sched_enabled() && rt_rq->tg != &मूल_task_group);
	वापस rt_rq->rq;
}

व्योम unregister_rt_sched_group(काष्ठा task_group *tg)
{
	अगर (!rt_group_sched_enabled())
		वापस;

	अगर (tg->rt_se)
		destroy_rt_bandwidth(&tg->rt_bandwidth);
}

व्योम free_rt_sched_group(काष्ठा task_group *tg)
{
	पूर्णांक i;

	अगर (!rt_group_sched_enabled())
		वापस;

	for_each_possible_cpu(i) {
		अगर (tg->rt_rq)
			kfree(tg->rt_rq[i]);
		अगर (tg->rt_se)
			kfree(tg->rt_se[i]);
	}

	kfree(tg->rt_rq);
	kfree(tg->rt_se);
}

व्योम init_tg_rt_entry(काष्ठा task_group *tg, काष्ठा rt_rq *rt_rq,
		काष्ठा sched_rt_entity *rt_se, पूर्णांक cpu,
		काष्ठा sched_rt_entity *parent)
{
	काष्ठा rq *rq = cpu_rq(cpu);

	rt_rq->highest_prio.curr = MAX_RT_PRIO-1;
	rt_rq->rt_nr_boosted = 0;
	rt_rq->rq = rq;
	rt_rq->tg = tg;

	tg->rt_rq[cpu] = rt_rq;
	tg->rt_se[cpu] = rt_se;

	अगर (!rt_se)
		वापस;

	अगर (!parent)
		rt_se->rt_rq = &rq->rt;
	अन्यथा
		rt_se->rt_rq = parent->my_q;

	rt_se->my_q = rt_rq;
	rt_se->parent = parent;
	INIT_LIST_शीर्ष(&rt_se->run_list);
}

पूर्णांक alloc_rt_sched_group(काष्ठा task_group *tg, काष्ठा task_group *parent)
{
	काष्ठा rt_rq *rt_rq;
	काष्ठा sched_rt_entity *rt_se;
	पूर्णांक i;

	अगर (!rt_group_sched_enabled())
		वापस 1;

	tg->rt_rq = kzalloc_objs(rt_rq, nr_cpu_ids);
	अगर (!tg->rt_rq)
		जाओ err;
	tg->rt_se = kzalloc_objs(rt_se, nr_cpu_ids);
	अगर (!tg->rt_se)
		जाओ err;

	init_rt_bandwidth(&tg->rt_bandwidth, ktime_to_ns(global_rt_period()), 0);

	for_each_possible_cpu(i) {
		rt_rq = kzalloc_गाँठ(माप(काष्ठा rt_rq),
				     GFP_KERNEL, cpu_to_गाँठ(i));
		अगर (!rt_rq)
			जाओ err;

		rt_se = kzalloc_गाँठ(माप(काष्ठा sched_rt_entity),
				     GFP_KERNEL, cpu_to_गाँठ(i));
		अगर (!rt_se)
			जाओ err_free_rq;

		init_rt_rq(rt_rq);
		rt_rq->rt_runtime = tg->rt_bandwidth.rt_runtime;
		init_tg_rt_entry(tg, rt_rq, rt_se, i, parent->rt_se[i]);
	}

	वापस 1;

err_free_rq:
	kfree(rt_rq);
err:
	वापस 0;
}

#अन्यथा /* !CONFIG_RT_GROUP_SCHED: */

#घोषणा rt_entity_is_task(rt_se) (1)

अटल अंतरभूत काष्ठा task_struct *rt_task_of(काष्ठा sched_rt_entity *rt_se)
{
	वापस container_of(rt_se, काष्ठा task_struct, rt);
}

अटल अंतरभूत काष्ठा rq *rq_of_rt_rq(काष्ठा rt_rq *rt_rq)
{
	वापस container_of(rt_rq, काष्ठा rq, rt);
}

अटल अंतरभूत काष्ठा rq *rq_of_rt_se(काष्ठा sched_rt_entity *rt_se)
{
	काष्ठा task_struct *p = rt_task_of(rt_se);

	वापस task_rq(p);
}

अटल अंतरभूत काष्ठा rt_rq *rt_rq_of_se(काष्ठा sched_rt_entity *rt_se)
{
	काष्ठा rq *rq = rq_of_rt_se(rt_se);

	वापस &rq->rt;
}

व्योम unregister_rt_sched_group(काष्ठा task_group *tg) { }

व्योम free_rt_sched_group(काष्ठा task_group *tg) { }

पूर्णांक alloc_rt_sched_group(काष्ठा task_group *tg, काष्ठा task_group *parent)
{
	वापस 1;
}
#पूर्ण_अगर /* !CONFIG_RT_GROUP_SCHED */

अटल अंतरभूत bool need_pull_rt_task(काष्ठा rq *rq, काष्ठा task_struct *पूर्व)
{
	/* Try to pull RT tasks here if we lower this rq's prio */
	वापस rq->online && rq->rt.highest_prio.curr > पूर्व->prio;
}

अटल अंतरभूत पूर्णांक rt_overloaded(काष्ठा rq *rq)
{
	वापस atomic_read(&rq->rd->rto_गिनती);
}

अटल अंतरभूत व्योम rt_set_overload(काष्ठा rq *rq)
{
	अगर (!rq->online)
		वापस;

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
	atomic_inc(&rq->rd->rto_गिनती);
}

अटल अंतरभूत व्योम rt_clear_overload(काष्ठा rq *rq)
{
	अगर (!rq->online)
		वापस;

	/* the order here really doesn't matter */
	atomic_dec(&rq->rd->rto_गिनती);
	cpumask_clear_cpu(rq->cpu, rq->rd->rto_mask);
}

अटल अंतरभूत पूर्णांक has_pushable_tasks(काष्ठा rq *rq)
{
	वापस !plist_शीर्ष_empty(&rq->rt.pushable_tasks);
}

अटल DEFINE_PER_CPU(काष्ठा balance_callback, rt_धकेलो_शीर्ष);
अटल DEFINE_PER_CPU(काष्ठा balance_callback, rt_pull_शीर्ष);

अटल व्योम धकेलो_rt_tasks(काष्ठा rq *);
अटल व्योम pull_rt_task(काष्ठा rq *);

अटल अंतरभूत व्योम rt_queue_धकेलो_tasks(काष्ठा rq *rq)
{
	अगर (!has_pushable_tasks(rq))
		वापस;

	queue_balance_callback(rq, &per_cpu(rt_धकेलो_शीर्ष, rq->cpu), धकेलो_rt_tasks);
}

अटल अंतरभूत व्योम rt_queue_pull_task(काष्ठा rq *rq)
{
	queue_balance_callback(rq, &per_cpu(rt_pull_शीर्ष, rq->cpu), pull_rt_task);
}

अटल व्योम enqueue_pushable_task(काष्ठा rq *rq, काष्ठा task_struct *p)
{
	plist_del(&p->pushable_tasks, &rq->rt.pushable_tasks);
	plist_गाँठ_init(&p->pushable_tasks, p->prio);
	plist_add(&p->pushable_tasks, &rq->rt.pushable_tasks);

	/* Update the highest prio pushable task */
	अगर (p->prio < rq->rt.highest_prio.अग्र)
		rq->rt.highest_prio.अग्र = p->prio;

	अगर (!rq->rt.overloaded) {
		rt_set_overload(rq);
		rq->rt.overloaded = 1;
	}
}

अटल व्योम dequeue_pushable_task(काष्ठा rq *rq, काष्ठा task_struct *p)
{
	plist_del(&p->pushable_tasks, &rq->rt.pushable_tasks);

	/* Update the new highest prio pushable task */
	अगर (has_pushable_tasks(rq)) {
		p = plist_first_entry(&rq->rt.pushable_tasks,
				      काष्ठा task_struct, pushable_tasks);
		rq->rt.highest_prio.अग्र = p->prio;
	} अन्यथा {
		rq->rt.highest_prio.अग्र = MAX_RT_PRIO-1;

		अगर (rq->rt.overloaded) {
			rt_clear_overload(rq);
			rq->rt.overloaded = 0;
		}
	}
}

अटल व्योम enqueue_शीर्ष_rt_rq(काष्ठा rt_rq *rt_rq);
अटल व्योम dequeue_शीर्ष_rt_rq(काष्ठा rt_rq *rt_rq, अचिन्हित पूर्णांक गिनती);

अटल अंतरभूत पूर्णांक on_rt_rq(काष्ठा sched_rt_entity *rt_se)
{
	वापस rt_se->on_rq;
}

#अगर_घोषित CONFIG_UCLAMP_TASK
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
अटल अंतरभूत bool rt_task_fits_capacity(काष्ठा task_struct *p, पूर्णांक cpu)
{
	अचिन्हित पूर्णांक min_cap;
	अचिन्हित पूर्णांक max_cap;
	अचिन्हित पूर्णांक cpu_cap;

	/* Only heterogeneous systems can benefit from this check */
	अगर (!sched_asym_cpucap_active())
		वापस true;

	min_cap = uclamp_eff_मूल्य(p, UCLAMP_MIN);
	max_cap = uclamp_eff_मूल्य(p, UCLAMP_MAX);

	cpu_cap = arch_scale_cpu_capacity(cpu);

	वापस cpu_cap >= min(min_cap, max_cap);
}
#अन्यथा /* !CONFIG_UCLAMP_TASK: */
अटल अंतरभूत bool rt_task_fits_capacity(काष्ठा task_struct *p, पूर्णांक cpu)
{
	वापस true;
}
#पूर्ण_अगर /* !CONFIG_UCLAMP_TASK */

#अगर_घोषित CONFIG_RT_GROUP_SCHED

अटल अंतरभूत u64 sched_rt_runtime(काष्ठा rt_rq *rt_rq)
{
	वापस rt_rq->rt_runtime;
}

अटल अंतरभूत u64 sched_rt_period(काष्ठा rt_rq *rt_rq)
{
	वापस ktime_to_ns(rt_rq->tg->rt_bandwidth.rt_period);
}

प्रकार काष्ठा task_group *rt_rq_iter_t;

अटल अंतरभूत काष्ठा task_group *अग्र_task_group(काष्ठा task_group *tg)
{
	अगर (!rt_group_sched_enabled()) {
		WARN_ON(tg != &मूल_task_group);
		वापस शून्य;
	}

	करो {
		tg = list_entry_rcu(tg->list.अग्र,
			typeof(काष्ठा task_group), list);
	} जबतक (&tg->list != &task_groups && task_group_is_autogroup(tg));

	अगर (&tg->list == &task_groups)
		tg = शून्य;

	वापस tg;
}

#घोषणा for_each_rt_rq(rt_rq, iter, rq)					\
	क्रम (iter = &मूल_task_group;					\
		iter && (rt_rq = iter->rt_rq[cpu_of(rq)]);		\
		iter = अग्र_task_group(iter))

#घोषणा for_each_sched_rt_entity(rt_se) \
	क्रम (; rt_se; rt_se = rt_se->parent)

अटल अंतरभूत काष्ठा rt_rq *group_rt_rq(काष्ठा sched_rt_entity *rt_se)
{
	वापस rt_se->my_q;
}

अटल व्योम enqueue_rt_entity(काष्ठा sched_rt_entity *rt_se, अचिन्हित पूर्णांक flags);
अटल व्योम dequeue_rt_entity(काष्ठा sched_rt_entity *rt_se, अचिन्हित पूर्णांक flags);

अटल व्योम sched_rt_rq_enqueue(काष्ठा rt_rq *rt_rq)
{
	काष्ठा task_struct *donor = rq_of_rt_rq(rt_rq)->donor;
	काष्ठा rq *rq = rq_of_rt_rq(rt_rq);
	काष्ठा sched_rt_entity *rt_se;

	पूर्णांक cpu = cpu_of(rq);

	rt_se = rt_rq->tg->rt_se[cpu];

	अगर (rt_rq->rt_nr_running) {
		अगर (!rt_se)
			enqueue_शीर्ष_rt_rq(rt_rq);
		अन्यथा अगर (!on_rt_rq(rt_se))
			enqueue_rt_entity(rt_se, 0);

		अगर (rt_rq->highest_prio.curr < donor->prio)
			resched_curr(rq);
	}
}

अटल व्योम sched_rt_rq_dequeue(काष्ठा rt_rq *rt_rq)
{
	काष्ठा sched_rt_entity *rt_se;
	पूर्णांक cpu = cpu_of(rq_of_rt_rq(rt_rq));

	rt_se = rt_rq->tg->rt_se[cpu];

	अगर (!rt_se) {
		dequeue_शीर्ष_rt_rq(rt_rq, rt_rq->rt_nr_running);
		/* Kick cpufreq (see the comment in kernel/sched/sched.h). */
		cpufreq_update_util(rq_of_rt_rq(rt_rq), 0);
	}
	अन्यथा अगर (on_rt_rq(rt_se))
		dequeue_rt_entity(rt_se, 0);
}

अटल अंतरभूत पूर्णांक rt_rq_throttled(काष्ठा rt_rq *rt_rq)
{
	वापस rt_rq->rt_throttled && !rt_rq->rt_nr_boosted;
}

अटल पूर्णांक rt_se_boosted(काष्ठा sched_rt_entity *rt_se)
{
	काष्ठा rt_rq *rt_rq = group_rt_rq(rt_se);
	काष्ठा task_struct *p;

	अगर (rt_rq)
		वापस !!rt_rq->rt_nr_boosted;

	p = rt_task_of(rt_se);
	वापस p->prio != p->normal_prio;
}

अटल अंतरभूत स्थिर काष्ठा cpumask *sched_rt_period_mask(व्योम)
{
	वापस this_rq()->rd->span;
}

अटल अंतरभूत
काष्ठा rt_rq *sched_rt_period_rt_rq(काष्ठा rt_bandwidth *rt_b, पूर्णांक cpu)
{
	वापस container_of(rt_b, काष्ठा task_group, rt_bandwidth)->rt_rq[cpu];
}

अटल अंतरभूत काष्ठा rt_bandwidth *sched_rt_bandwidth(काष्ठा rt_rq *rt_rq)
{
	वापस &rt_rq->tg->rt_bandwidth;
}

bool sched_rt_bandwidth_account(काष्ठा rt_rq *rt_rq)
{
	काष्ठा rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);

	वापस (hrtimer_active(&rt_b->rt_period_timer) ||
		rt_rq->rt_time < rt_b->rt_runtime);
}

/*
 * We ran out of runtime, see if we can borrow some from our neighbours.
 */
अटल व्योम do_balance_runtime(काष्ठा rt_rq *rt_rq)
{
	काष्ठा rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);
	काष्ठा मूल_domain *rd = rq_of_rt_rq(rt_rq)->rd;
	पूर्णांक i, weight;
	u64 rt_period;

	weight = cpumask_weight(rd->span);

	raw_spin_lock(&rt_b->rt_runtime_lock);
	rt_period = ktime_to_ns(rt_b->rt_period);
	for_each_cpu(i, rd->span) {
		काष्ठा rt_rq *iter = sched_rt_period_rt_rq(rt_b, i);
		s64 diff;

		अगर (iter == rt_rq)
			जारी;

		raw_spin_lock(&iter->rt_runtime_lock);
		/*
		 * Either all rqs have inf runtime and there's nothing to steal
		 * or __disable_runtime() below sets a specific rq to inf to
		 * indicate its been disabled and disallow stealing.
		 */
		अगर (iter->rt_runtime == RUNTIME_INF)
			जाओ अग्र;

		/*
		 * From runqueues with spare time, take 1/n part of their
		 * spare time, but no more than our period.
		 */
		diff = iter->rt_runtime - iter->rt_time;
		अगर (diff > 0) {
			diff = div_u64((u64)diff, weight);
			अगर (rt_rq->rt_runtime + diff > rt_period)
				diff = rt_period - rt_rq->rt_runtime;
			iter->rt_runtime -= diff;
			rt_rq->rt_runtime += diff;
			अगर (rt_rq->rt_runtime == rt_period) {
				raw_spin_unlock(&iter->rt_runtime_lock);
				अवरोध;
			}
		}
अग्र:
		raw_spin_unlock(&iter->rt_runtime_lock);
	}
	raw_spin_unlock(&rt_b->rt_runtime_lock);
}

/*
 * Ensure this RQ takes back all the runtime it lend to its neighbours.
 */
अटल व्योम __disable_runtime(काष्ठा rq *rq)
{
	काष्ठा मूल_domain *rd = rq->rd;
	rt_rq_iter_t iter;
	काष्ठा rt_rq *rt_rq;

	अगर (unlikely(!scheduler_running))
		वापस;

	for_each_rt_rq(rt_rq, iter, rq) {
		काष्ठा rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);
		s64 want;
		पूर्णांक i;

		raw_spin_lock(&rt_b->rt_runtime_lock);
		raw_spin_lock(&rt_rq->rt_runtime_lock);
		/*
		 * Either we're all inf and nobody needs to borrow, or we're
		 * already disabled and thus have nothing to do, or we have
		 * exactly the right amount of runtime to take out.
		 */
		अगर (rt_rq->rt_runtime == RUNTIME_INF ||
				rt_rq->rt_runtime == rt_b->rt_runtime)
			जाओ balanced;
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
			काष्ठा rt_rq *iter = sched_rt_period_rt_rq(rt_b, i);
			s64 diff;

			/*
			 * Can't reclaim from ourselves or disabled runqueues.
			 */
			अगर (iter == rt_rq || iter->rt_runtime == RUNTIME_INF)
				जारी;

			raw_spin_lock(&iter->rt_runtime_lock);
			अगर (want > 0) {
				diff = min_t(s64, iter->rt_runtime, want);
				iter->rt_runtime -= diff;
				want -= diff;
			} अन्यथा {
				iter->rt_runtime -= want;
				want -= want;
			}
			raw_spin_unlock(&iter->rt_runtime_lock);

			अगर (!want)
				अवरोध;
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

अटल व्योम __enable_runtime(काष्ठा rq *rq)
{
	rt_rq_iter_t iter;
	काष्ठा rt_rq *rt_rq;

	अगर (unlikely(!scheduler_running))
		वापस;

	/*
	 * Reset each runqueue's bandwidth settings
	 */
	for_each_rt_rq(rt_rq, iter, rq) {
		काष्ठा rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);

		raw_spin_lock(&rt_b->rt_runtime_lock);
		raw_spin_lock(&rt_rq->rt_runtime_lock);
		rt_rq->rt_runtime = rt_b->rt_runtime;
		rt_rq->rt_time = 0;
		rt_rq->rt_throttled = 0;
		raw_spin_unlock(&rt_rq->rt_runtime_lock);
		raw_spin_unlock(&rt_b->rt_runtime_lock);
	}
}

अटल व्योम balance_runtime(काष्ठा rt_rq *rt_rq)
{
	अगर (!sched_feat(RT_RUNTIME_SHARE))
		वापस;

	अगर (rt_rq->rt_time > rt_rq->rt_runtime) {
		raw_spin_unlock(&rt_rq->rt_runtime_lock);
		do_balance_runtime(rt_rq);
		raw_spin_lock(&rt_rq->rt_runtime_lock);
	}
}

अटल पूर्णांक do_sched_rt_period_timer(काष्ठा rt_bandwidth *rt_b, पूर्णांक overrun)
{
	पूर्णांक i, idle = 1, throttled = 0;
	स्थिर काष्ठा cpumask *span;

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
	अगर (rt_b == &मूल_task_group.rt_bandwidth)
		span = cpu_online_mask;

	for_each_cpu(i, span) {
		पूर्णांक enqueue = 0;
		काष्ठा rt_rq *rt_rq = sched_rt_period_rt_rq(rt_b, i);
		काष्ठा rq *rq = rq_of_rt_rq(rt_rq);
		काष्ठा rq_flags rf;
		पूर्णांक skip;

		/*
		 * When span == cpu_online_mask, taking each rq->lock
		 * can be time-consuming. Try to avoid it when possible.
		 */
		raw_spin_lock(&rt_rq->rt_runtime_lock);
		अगर (!sched_feat(RT_RUNTIME_SHARE) && rt_rq->rt_runtime != RUNTIME_INF)
			rt_rq->rt_runtime = rt_b->rt_runtime;
		skip = !rt_rq->rt_time && !rt_rq->rt_nr_running;
		raw_spin_unlock(&rt_rq->rt_runtime_lock);
		अगर (skip)
			जारी;

		rq_lock(rq, &rf);
		update_rq_clock(rq);

		अगर (rt_rq->rt_time) {
			u64 runtime;

			raw_spin_lock(&rt_rq->rt_runtime_lock);
			अगर (rt_rq->rt_throttled)
				balance_runtime(rt_rq);
			runtime = rt_rq->rt_runtime;
			rt_rq->rt_time -= min(rt_rq->rt_time, overrun*runtime);
			अगर (rt_rq->rt_throttled && rt_rq->rt_time < runtime) {
				rt_rq->rt_throttled = 0;
				enqueue = 1;

				/*
				 * When we're idle and a woken (rt) task is
				 * throttled wakeup_preempt() will set
				 * skip_update and the time between the wakeup
				 * and this unthrottle will get accounted as
				 * 'runtime'.
				 */
				अगर (rt_rq->rt_nr_running && rq->curr == rq->idle)
					rq_clock_cancel_skipupdate(rq);
			}
			अगर (rt_rq->rt_time || rt_rq->rt_nr_running)
				idle = 0;
			raw_spin_unlock(&rt_rq->rt_runtime_lock);
		} अन्यथा अगर (rt_rq->rt_nr_running) {
			idle = 0;
			अगर (!rt_rq_throttled(rt_rq))
				enqueue = 1;
		}
		अगर (rt_rq->rt_throttled)
			throttled = 1;

		अगर (enqueue)
			sched_rt_rq_enqueue(rt_rq);
		rq_unlock(rq, &rf);
	}

	अगर (!throttled && (!rt_bandwidth_enabled() || rt_b->rt_runtime == RUNTIME_INF))
		वापस 1;

	वापस idle;
}

अटल पूर्णांक sched_rt_runtime_exceeded(काष्ठा rt_rq *rt_rq)
{
	u64 runtime = sched_rt_runtime(rt_rq);

	अगर (rt_rq->rt_throttled)
		वापस rt_rq_throttled(rt_rq);

	अगर (runtime >= sched_rt_period(rt_rq))
		वापस 0;

	balance_runtime(rt_rq);
	runtime = sched_rt_runtime(rt_rq);
	अगर (runtime == RUNTIME_INF)
		वापस 0;

	अगर (rt_rq->rt_time > runtime) {
		काष्ठा rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);

		/*
		 * Don't actually throttle groups that have no runtime assigned
		 * but accrue some time due to boosting.
		 */
		अगर (likely(rt_b->rt_runtime)) {
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

		अगर (rt_rq_throttled(rt_rq)) {
			sched_rt_rq_dequeue(rt_rq);
			वापस 1;
		}
	}

	वापस 0;
}

#अन्यथा /* !CONFIG_RT_GROUP_SCHED: */

प्रकार काष्ठा rt_rq *rt_rq_iter_t;

#घोषणा for_each_rt_rq(rt_rq, iter, rq) \
	क्रम ((व्योम) iter, rt_rq = &rq->rt; rt_rq; rt_rq = शून्य)

#घोषणा for_each_sched_rt_entity(rt_se) \
	क्रम (; rt_se; rt_se = शून्य)

अटल अंतरभूत काष्ठा rt_rq *group_rt_rq(काष्ठा sched_rt_entity *rt_se)
{
	वापस शून्य;
}

अटल अंतरभूत व्योम sched_rt_rq_enqueue(काष्ठा rt_rq *rt_rq)
{
	काष्ठा rq *rq = rq_of_rt_rq(rt_rq);

	अगर (!rt_rq->rt_nr_running)
		वापस;

	enqueue_शीर्ष_rt_rq(rt_rq);
	resched_curr(rq);
}

अटल अंतरभूत व्योम sched_rt_rq_dequeue(काष्ठा rt_rq *rt_rq)
{
	dequeue_शीर्ष_rt_rq(rt_rq, rt_rq->rt_nr_running);
}

अटल अंतरभूत पूर्णांक rt_rq_throttled(काष्ठा rt_rq *rt_rq)
{
	वापस false;
}

अटल अंतरभूत स्थिर काष्ठा cpumask *sched_rt_period_mask(व्योम)
{
	वापस cpu_online_mask;
}

अटल अंतरभूत
काष्ठा rt_rq *sched_rt_period_rt_rq(काष्ठा rt_bandwidth *rt_b, पूर्णांक cpu)
{
	वापस &cpu_rq(cpu)->rt;
}

अटल व्योम __enable_runtime(काष्ठा rq *rq) { }
अटल व्योम __disable_runtime(काष्ठा rq *rq) { }

#पूर्ण_अगर /* !CONFIG_RT_GROUP_SCHED */

अटल अंतरभूत पूर्णांक rt_se_prio(काष्ठा sched_rt_entity *rt_se)
{
#अगर_घोषित CONFIG_RT_GROUP_SCHED
	काष्ठा rt_rq *rt_rq = group_rt_rq(rt_se);

	अगर (rt_rq)
		वापस rt_rq->highest_prio.curr;
#पूर्ण_अगर

	वापस rt_task_of(rt_se)->prio;
}

/*
 * Update the current task's runtime statistics. Skip current tasks that
 * are not in our scheduling class.
 */
अटल व्योम update_curr_rt(काष्ठा rq *rq)
{
	काष्ठा task_struct *donor = rq->donor;
	s64 delta_exec;

	अगर (donor->sched_class != &rt_sched_class)
		वापस;

	delta_exec = update_curr_common(rq);
	अगर (unlikely(delta_exec <= 0))
		वापस;

#अगर_घोषित CONFIG_RT_GROUP_SCHED
	काष्ठा sched_rt_entity *rt_se = &donor->rt;

	अगर (!rt_bandwidth_enabled())
		वापस;

	for_each_sched_rt_entity(rt_se) {
		काष्ठा rt_rq *rt_rq = rt_rq_of_se(rt_se);
		पूर्णांक exceeded;

		अगर (sched_rt_runtime(rt_rq) != RUNTIME_INF) {
			raw_spin_lock(&rt_rq->rt_runtime_lock);
			rt_rq->rt_time += delta_exec;
			exceeded = sched_rt_runtime_exceeded(rt_rq);
			अगर (exceeded)
				resched_curr(rq);
			raw_spin_unlock(&rt_rq->rt_runtime_lock);
			अगर (exceeded)
				do_start_rt_bandwidth(sched_rt_bandwidth(rt_rq));
		}
	}
#पूर्ण_अगर /* CONFIG_RT_GROUP_SCHED */
}

अटल व्योम
dequeue_शीर्ष_rt_rq(काष्ठा rt_rq *rt_rq, अचिन्हित पूर्णांक गिनती)
{
	काष्ठा rq *rq = rq_of_rt_rq(rt_rq);

	BUG_ON(&rq->rt != rt_rq);

	अगर (!rt_rq->rt_queued)
		वापस;

	BUG_ON(!rq->nr_running);

	sub_nr_running(rq, गिनती);
	rt_rq->rt_queued = 0;

}

अटल व्योम
enqueue_शीर्ष_rt_rq(काष्ठा rt_rq *rt_rq)
{
	काष्ठा rq *rq = rq_of_rt_rq(rt_rq);

	BUG_ON(&rq->rt != rt_rq);

	अगर (rt_rq->rt_queued)
		वापस;

	अगर (rt_rq_throttled(rt_rq))
		वापस;

	अगर (rt_rq->rt_nr_running) {
		add_nr_running(rq, rt_rq->rt_nr_running);
		rt_rq->rt_queued = 1;
	}

	/* Kick cpufreq (see the comment in kernel/sched/sched.h). */
	cpufreq_update_util(rq, 0);
}

अटल व्योम
inc_rt_prio_smp(काष्ठा rt_rq *rt_rq, पूर्णांक prio, पूर्णांक पूर्व_prio)
{
	काष्ठा rq *rq = rq_of_rt_rq(rt_rq);

	/*
	 * Change rq's cpupri only if rt_rq is the top queue.
	 */
	अगर (IS_ENABLED(CONFIG_RT_GROUP_SCHED) && &rq->rt != rt_rq)
		वापस;

	अगर (rq->online && prio < पूर्व_prio)
		cpupri_set(&rq->rd->cpupri, rq->cpu, prio);
}

अटल व्योम
dec_rt_prio_smp(काष्ठा rt_rq *rt_rq, पूर्णांक prio, पूर्णांक पूर्व_prio)
{
	काष्ठा rq *rq = rq_of_rt_rq(rt_rq);

	/*
	 * Change rq's cpupri only if rt_rq is the top queue.
	 */
	अगर (IS_ENABLED(CONFIG_RT_GROUP_SCHED) && &rq->rt != rt_rq)
		वापस;

	अगर (rq->online && rt_rq->highest_prio.curr != पूर्व_prio)
		cpupri_set(&rq->rd->cpupri, rq->cpu, rt_rq->highest_prio.curr);
}

अटल व्योम
inc_rt_prio(काष्ठा rt_rq *rt_rq, पूर्णांक prio)
{
	पूर्णांक पूर्व_prio = rt_rq->highest_prio.curr;

	अगर (prio < पूर्व_prio)
		rt_rq->highest_prio.curr = prio;

	inc_rt_prio_smp(rt_rq, prio, पूर्व_prio);
}

अटल व्योम
dec_rt_prio(काष्ठा rt_rq *rt_rq, पूर्णांक prio)
{
	पूर्णांक पूर्व_prio = rt_rq->highest_prio.curr;

	अगर (rt_rq->rt_nr_running) {

		WARN_ON(prio < पूर्व_prio);

		/*
		 * This may have been our highest task, and therefore
		 * we may have some re-computation to do
		 */
		अगर (prio == पूर्व_prio) {
			काष्ठा rt_prio_array *array = &rt_rq->active;

			rt_rq->highest_prio.curr =
				sched_find_first_bit(array->bitmap);
		}

	} अन्यथा {
		rt_rq->highest_prio.curr = MAX_RT_PRIO-1;
	}

	dec_rt_prio_smp(rt_rq, prio, पूर्व_prio);
}

#अगर_घोषित CONFIG_RT_GROUP_SCHED

अटल व्योम
inc_rt_group(काष्ठा sched_rt_entity *rt_se, काष्ठा rt_rq *rt_rq)
{
	अगर (rt_se_boosted(rt_se))
		rt_rq->rt_nr_boosted++;

	start_rt_bandwidth(&rt_rq->tg->rt_bandwidth);
}

अटल व्योम
dec_rt_group(काष्ठा sched_rt_entity *rt_se, काष्ठा rt_rq *rt_rq)
{
	अगर (rt_se_boosted(rt_se))
		rt_rq->rt_nr_boosted--;

	WARN_ON(!rt_rq->rt_nr_running && rt_rq->rt_nr_boosted);
}

#अन्यथा /* !CONFIG_RT_GROUP_SCHED: */

अटल व्योम
inc_rt_group(काष्ठा sched_rt_entity *rt_se, काष्ठा rt_rq *rt_rq)
{
}

अटल अंतरभूत
व्योम dec_rt_group(काष्ठा sched_rt_entity *rt_se, काष्ठा rt_rq *rt_rq) {}

#पूर्ण_अगर /* !CONFIG_RT_GROUP_SCHED */

अटल अंतरभूत
अचिन्हित पूर्णांक rt_se_nr_running(काष्ठा sched_rt_entity *rt_se)
{
	काष्ठा rt_rq *group_rq = group_rt_rq(rt_se);

	अगर (group_rq)
		वापस group_rq->rt_nr_running;
	अन्यथा
		वापस 1;
}

अटल अंतरभूत
अचिन्हित पूर्णांक rt_se_rr_nr_running(काष्ठा sched_rt_entity *rt_se)
{
	काष्ठा rt_rq *group_rq = group_rt_rq(rt_se);
	काष्ठा task_struct *tsk;

	अगर (group_rq)
		वापस group_rq->rr_nr_running;

	tsk = rt_task_of(rt_se);

	वापस (tsk->policy == SCHED_RR) ? 1 : 0;
}

अटल अंतरभूत
व्योम inc_rt_tasks(काष्ठा sched_rt_entity *rt_se, काष्ठा rt_rq *rt_rq)
{
	पूर्णांक prio = rt_se_prio(rt_se);

	WARN_ON(!rt_prio(prio));
	rt_rq->rt_nr_running += rt_se_nr_running(rt_se);
	rt_rq->rr_nr_running += rt_se_rr_nr_running(rt_se);

	inc_rt_prio(rt_rq, prio);
	inc_rt_group(rt_se, rt_rq);
}

अटल अंतरभूत
व्योम dec_rt_tasks(काष्ठा sched_rt_entity *rt_se, काष्ठा rt_rq *rt_rq)
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
अटल अंतरभूत bool move_entity(अचिन्हित पूर्णांक flags)
{
	अगर ((flags & (DEQUEUE_SAVE | DEQUEUE_MOVE)) == DEQUEUE_SAVE)
		वापस false;

	वापस true;
}

अटल व्योम __delist_rt_entity(काष्ठा sched_rt_entity *rt_se, काष्ठा rt_prio_array *array)
{
	list_del_init(&rt_se->run_list);

	अगर (list_empty(array->queue + rt_se_prio(rt_se)))
		__clear_bit(rt_se_prio(rt_se), array->bitmap);

	rt_se->on_list = 0;
}

अटल अंतरभूत काष्ठा sched_statistics *
__schedstats_from_rt_se(काष्ठा sched_rt_entity *rt_se)
{
	/* schedstats is not supported for rt group. */
	अगर (!rt_entity_is_task(rt_se))
		वापस शून्य;

	वापस &rt_task_of(rt_se)->stats;
}

अटल अंतरभूत व्योम
update_stats_wait_start_rt(काष्ठा rt_rq *rt_rq, काष्ठा sched_rt_entity *rt_se)
{
	काष्ठा sched_statistics *stats;
	काष्ठा task_struct *p = शून्य;

	अगर (!schedstat_enabled())
		वापस;

	अगर (rt_entity_is_task(rt_se))
		p = rt_task_of(rt_se);

	stats = __schedstats_from_rt_se(rt_se);
	अगर (!stats)
		वापस;

	__update_stats_wait_start(rq_of_rt_rq(rt_rq), p, stats);
}

अटल अंतरभूत व्योम
update_stats_enqueue_sleeper_rt(काष्ठा rt_rq *rt_rq, काष्ठा sched_rt_entity *rt_se)
{
	काष्ठा sched_statistics *stats;
	काष्ठा task_struct *p = शून्य;

	अगर (!schedstat_enabled())
		वापस;

	अगर (rt_entity_is_task(rt_se))
		p = rt_task_of(rt_se);

	stats = __schedstats_from_rt_se(rt_se);
	अगर (!stats)
		वापस;

	__update_stats_enqueue_sleeper(rq_of_rt_rq(rt_rq), p, stats);
}

अटल अंतरभूत व्योम
update_stats_enqueue_rt(काष्ठा rt_rq *rt_rq, काष्ठा sched_rt_entity *rt_se,
			पूर्णांक flags)
{
	अगर (!schedstat_enabled())
		वापस;

	अगर (flags & ENQUEUE_WAKEUP)
		update_stats_enqueue_sleeper_rt(rt_rq, rt_se);
}

अटल अंतरभूत व्योम
update_stats_wait_end_rt(काष्ठा rt_rq *rt_rq, काष्ठा sched_rt_entity *rt_se)
{
	काष्ठा sched_statistics *stats;
	काष्ठा task_struct *p = शून्य;

	अगर (!schedstat_enabled())
		वापस;

	अगर (rt_entity_is_task(rt_se))
		p = rt_task_of(rt_se);

	stats = __schedstats_from_rt_se(rt_se);
	अगर (!stats)
		वापस;

	__update_stats_wait_end(rq_of_rt_rq(rt_rq), p, stats);
}

अटल अंतरभूत व्योम
update_stats_dequeue_rt(काष्ठा rt_rq *rt_rq, काष्ठा sched_rt_entity *rt_se,
			पूर्णांक flags)
{
	काष्ठा task_struct *p = शून्य;
	काष्ठा rq *rq = rq_of_rt_rq(rt_rq);

	अगर (!schedstat_enabled())
		वापस;

	अगर (rt_entity_is_task(rt_se)) {
		p = rt_task_of(rt_se);

		अगर (p != rq->curr)
			update_stats_wait_end_rt(rt_rq, rt_se);
	}

	अगर ((flags & DEQUEUE_SLEEP) && p) {
		अचिन्हित पूर्णांक state;

		state = READ_ONCE(p->__state);
		अगर (state & TASK_INTERRUPTIBLE)
			__schedstat_set(p->stats.sleep_start,
					rq_clock(rq_of_rt_rq(rt_rq)));

		अगर (state & TASK_UNINTERRUPTIBLE)
			__schedstat_set(p->stats.block_start,
					rq_clock(rq_of_rt_rq(rt_rq)));
	}
}

अटल व्योम __enqueue_rt_entity(काष्ठा sched_rt_entity *rt_se, अचिन्हित पूर्णांक flags)
{
	काष्ठा rt_rq *rt_rq = rt_rq_of_se(rt_se);
	काष्ठा rt_prio_array *array = &rt_rq->active;
	काष्ठा rt_rq *group_rq = group_rt_rq(rt_se);
	काष्ठा list_शीर्ष *queue = array->queue + rt_se_prio(rt_se);

	/*
	 * Don't enqueue the group if its throttled, or when empty.
	 * The latter is a consequence of the former when a child group
	 * get throttled and the current group doesn't have any other
	 * active members.
	 */
	अगर (group_rq && (rt_rq_throttled(group_rq) || !group_rq->rt_nr_running)) {
		अगर (rt_se->on_list)
			__delist_rt_entity(rt_se, array);
		वापस;
	}

	अगर (move_entity(flags)) {
		WARN_ON_ONCE(rt_se->on_list);
		अगर (flags & ENQUEUE_शीर्ष)
			list_add(&rt_se->run_list, queue);
		अन्यथा
			list_add_पुच्छ(&rt_se->run_list, queue);

		__set_bit(rt_se_prio(rt_se), array->bitmap);
		rt_se->on_list = 1;
	}
	rt_se->on_rq = 1;

	inc_rt_tasks(rt_se, rt_rq);
}

अटल व्योम __dequeue_rt_entity(काष्ठा sched_rt_entity *rt_se, अचिन्हित पूर्णांक flags)
{
	काष्ठा rt_rq *rt_rq = rt_rq_of_se(rt_se);
	काष्ठा rt_prio_array *array = &rt_rq->active;

	अगर (move_entity(flags)) {
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
अटल व्योम dequeue_rt_ढेर(काष्ठा sched_rt_entity *rt_se, अचिन्हित पूर्णांक flags)
{
	काष्ठा sched_rt_entity *back = शून्य;
	अचिन्हित पूर्णांक rt_nr_running;

	for_each_sched_rt_entity(rt_se) {
		rt_se->back = back;
		back = rt_se;
	}

	rt_nr_running = rt_rq_of_se(back)->rt_nr_running;

	क्रम (rt_se = back; rt_se; rt_se = rt_se->back) {
		अगर (on_rt_rq(rt_se))
			__dequeue_rt_entity(rt_se, flags);
	}

	dequeue_शीर्ष_rt_rq(rt_rq_of_se(back), rt_nr_running);
}

अटल व्योम enqueue_rt_entity(काष्ठा sched_rt_entity *rt_se, अचिन्हित पूर्णांक flags)
{
	काष्ठा rq *rq = rq_of_rt_se(rt_se);

	update_stats_enqueue_rt(rt_rq_of_se(rt_se), rt_se, flags);

	dequeue_rt_ढेर(rt_se, flags);
	for_each_sched_rt_entity(rt_se)
		__enqueue_rt_entity(rt_se, flags);
	enqueue_शीर्ष_rt_rq(&rq->rt);
}

अटल व्योम dequeue_rt_entity(काष्ठा sched_rt_entity *rt_se, अचिन्हित पूर्णांक flags)
{
	काष्ठा rq *rq = rq_of_rt_se(rt_se);

	update_stats_dequeue_rt(rt_rq_of_se(rt_se), rt_se, flags);

	dequeue_rt_ढेर(rt_se, flags);

	for_each_sched_rt_entity(rt_se) {
		काष्ठा rt_rq *rt_rq = group_rt_rq(rt_se);

		अगर (rt_rq && rt_rq->rt_nr_running)
			__enqueue_rt_entity(rt_se, flags);
	}
	enqueue_शीर्ष_rt_rq(&rq->rt);
}

/*
 * Adding/removing a task to/from a priority array:
 */
अटल व्योम
enqueue_task_rt(काष्ठा rq *rq, काष्ठा task_struct *p, पूर्णांक flags)
{
	काष्ठा sched_rt_entity *rt_se = &p->rt;

	अगर (flags & ENQUEUE_WAKEUP)
		rt_se->timeout = 0;

	check_schedstat_required();
	update_stats_wait_start_rt(rt_rq_of_se(rt_se), rt_se);

	enqueue_rt_entity(rt_se, flags);

	अगर (task_is_blocked(p))
		वापस;

	अगर (!task_current(rq, p) && p->nr_cpus_allowed > 1)
		enqueue_pushable_task(rq, p);
}

अटल bool dequeue_task_rt(काष्ठा rq *rq, काष्ठा task_struct *p, पूर्णांक flags)
{
	काष्ठा sched_rt_entity *rt_se = &p->rt;

	update_curr_rt(rq);
	dequeue_rt_entity(rt_se, flags);

	dequeue_pushable_task(rq, p);

	वापस true;
}

/*
 * Put task to the head or the end of the run list without the overhead of
 * dequeue followed by enqueue.
 */
अटल व्योम
requeue_rt_entity(काष्ठा rt_rq *rt_rq, काष्ठा sched_rt_entity *rt_se, पूर्णांक शीर्ष)
{
	अगर (on_rt_rq(rt_se)) {
		काष्ठा rt_prio_array *array = &rt_rq->active;
		काष्ठा list_शीर्ष *queue = array->queue + rt_se_prio(rt_se);

		अगर (शीर्ष)
			list_move(&rt_se->run_list, queue);
		अन्यथा
			list_move_पुच्छ(&rt_se->run_list, queue);
	}
}

अटल व्योम requeue_task_rt(काष्ठा rq *rq, काष्ठा task_struct *p, पूर्णांक शीर्ष)
{
	काष्ठा sched_rt_entity *rt_se = &p->rt;
	काष्ठा rt_rq *rt_rq;

	for_each_sched_rt_entity(rt_se) {
		rt_rq = rt_rq_of_se(rt_se);
		requeue_rt_entity(rt_rq, rt_se, शीर्ष);
	}
}

अटल व्योम yield_task_rt(काष्ठा rq *rq)
{
	requeue_task_rt(rq, rq->donor, 0);
}

अटल पूर्णांक find_lowest_rq(काष्ठा task_struct *task);

अटल पूर्णांक
select_task_rq_rt(काष्ठा task_struct *p, पूर्णांक cpu, पूर्णांक flags)
{
	काष्ठा task_struct *curr, *donor;
	काष्ठा rq *rq;
	bool test;

	/* For anything but wake ups, just return the task_cpu */
	अगर (!(flags & (WF_TTWU | WF_FORK)))
		जाओ out;

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

	अगर (test || !rt_task_fits_capacity(p, cpu)) {
		पूर्णांक target = find_lowest_rq(p);

		/*
		 * Bail out if we were forcing a migration to find a better
		 * fitting CPU but our search failed.
		 */
		अगर (!test && target != -1 && !rt_task_fits_capacity(p, target))
			जाओ out_unlock;

		/*
		 * Don't bother moving it if the destination CPU is
		 * not running a lower priority task.
		 */
		अगर (target != -1 &&
		    p->prio < cpu_rq(target)->rt.highest_prio.curr)
			cpu = target;
	}

out_unlock:
	rcu_read_unlock();

out:
	वापस cpu;
}

अटल व्योम check_preempt_equal_prio(काष्ठा rq *rq, काष्ठा task_struct *p)
{
	अगर (rq->curr->nr_cpus_allowed == 1 ||
	    !cpupri_find(&rq->rd->cpupri, rq->donor, शून्य))
		वापस;

	/*
	 * p is migratable, so let's not schedule it and
	 * see if it is pushed or pulled somewhere else.
	 */
	अगर (p->nr_cpus_allowed != 1 &&
	    cpupri_find(&rq->rd->cpupri, p, शून्य))
		वापस;

	/*
	 * There appear to be other CPUs that can accept
	 * the current task but none can run 'p', so lets reschedule
	 * to try and push the current task away:
	 */
	requeue_task_rt(rq, p, 1);
	resched_curr(rq);
}

अटल पूर्णांक balance_rt(काष्ठा rq *rq, काष्ठा rq_flags *rf)
{
	/*
	 * Note, rq->donor may change during rq lock drops,
	 * so don't re-use p across lock drops
	 */
	काष्ठा task_struct *p = rq->donor;

	अगर (!on_rt_rq(&p->rt) && need_pull_rt_task(rq, p)) {
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

	वापस sched_stop_runnable(rq) || sched_dl_runnable(rq) || sched_rt_runnable(rq);
}

/*
 * Preempt the current task with a newly woken task if needed:
 */
अटल व्योम wakeup_preempt_rt(काष्ठा rq *rq, काष्ठा task_struct *p, पूर्णांक flags)
{
	काष्ठा task_struct *donor = rq->donor;

	/*
	 * XXX If we're preempted by DL, queue a push?
	 */
	अगर (p->sched_class != &rt_sched_class ||
	    donor->sched_class != &rt_sched_class)
		वापस;

	अगर (p->prio < donor->prio) {
		resched_curr(rq);
		वापस;
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
	अगर (p->prio == donor->prio && !test_tsk_need_resched(rq->curr))
		check_preempt_equal_prio(rq, p);
}

अटल अंतरभूत व्योम set_अग्र_task_rt(काष्ठा rq *rq, काष्ठा task_struct *p, क्रमागत snt_e type)
{
	काष्ठा sched_rt_entity *rt_se = &p->rt;
	काष्ठा rt_rq *rt_rq = &rq->rt;

	अगर (type == SNT_REPICK)
		वापस;

	p->se.exec_start = rq_clock_task(rq);
	अगर (on_rt_rq(&p->rt))
		update_stats_wait_end_rt(rt_rq, rt_se);

	/* The running task is never eligible for pushing */
	dequeue_pushable_task(rq, p);

	अगर (type != SNT_PICK)
		वापस;

	/*
	 * If prev task was rt, put_prev_task() has already updated the
	 * utilization. We only care of the case where we start to schedule a
	 * rt task
	 */
	अगर (rq->donor->sched_class != &rt_sched_class)
		update_rt_rq_load_avg(rq_clock_pelt(rq), rq, 0);

	rt_queue_धकेलो_tasks(rq);
}

अटल काष्ठा sched_rt_entity *pick_अग्र_rt_entity(काष्ठा rt_rq *rt_rq)
{
	काष्ठा rt_prio_array *array = &rt_rq->active;
	काष्ठा sched_rt_entity *अग्र = शून्य;
	काष्ठा list_शीर्ष *queue;
	पूर्णांक idx;

	idx = sched_find_first_bit(array->bitmap);
	BUG_ON(idx >= MAX_RT_PRIO);

	queue = array->queue + idx;
	अगर (WARN_ON_ONCE(list_empty(queue)))
		वापस शून्य;
	अग्र = list_entry(queue->अग्र, काष्ठा sched_rt_entity, run_list);

	वापस अग्र;
}

अटल काष्ठा task_struct *pick_अग्र_task_rt(काष्ठा rq *rq)
{
	काष्ठा sched_rt_entity *rt_se;
	काष्ठा rt_rq *rt_rq  = &rq->rt;

	करो {
		rt_se = pick_अग्र_rt_entity(rt_rq);
		अगर (unlikely(!rt_se))
			वापस शून्य;
		rt_rq = group_rt_rq(rt_se);
	} जबतक (rt_rq);

	वापस rt_task_of(rt_se);
}

अटल काष्ठा task_struct *pick_task_rt(काष्ठा rq *rq, काष्ठा rq_flags *rf)
{
	काष्ठा task_struct *p;

	अगर (!sched_rt_runnable(rq))
		वापस शून्य;

	p = pick_अग्र_task_rt(rq);

	वापस p;
}

अटल व्योम put_पूर्व_task_rt(काष्ठा rq *rq, काष्ठा task_struct *p, काष्ठा task_struct *अग्र)
{
	काष्ठा sched_rt_entity *rt_se = &p->rt;
	काष्ठा rt_rq *rt_rq = &rq->rt;

	अगर (on_rt_rq(&p->rt))
		update_stats_wait_start_rt(rt_rq, rt_se);

	update_curr_rt(rq);

	update_rt_rq_load_avg(rq_clock_pelt(rq), rq, 1);

	अगर (task_is_blocked(p))
		वापस;
	/*
	 * The previous task needs to be made eligible for pushing
	 * if it is still active
	 */
	अगर (on_rt_rq(&p->rt) && p->nr_cpus_allowed > 1)
		enqueue_pushable_task(rq, p);
}

/* Only try algorithms three times */
#घोषणा RT_MAX_TRIES 3

/*
 * Return the highest pushable rq's task, which is suitable to be executed
 * on the CPU, NULL otherwise
 */
अटल काष्ठा task_struct *pick_highest_pushable_task(काष्ठा rq *rq, पूर्णांक cpu)
{
	काष्ठा plist_शीर्ष *शीर्ष = &rq->rt.pushable_tasks;
	काष्ठा task_struct *p;

	अगर (!has_pushable_tasks(rq))
		वापस शून्य;

	plist_for_each_entry(p, शीर्ष, pushable_tasks) {
		अगर (task_is_pushable(rq, p, cpu))
			वापस p;
	}

	वापस शून्य;
}

अटल DEFINE_PER_CPU(cpumask_var_t, local_cpu_mask);

अटल पूर्णांक find_lowest_rq(काष्ठा task_struct *task)
{
	काष्ठा sched_domain *sd;
	काष्ठा cpumask *lowest_mask = this_cpu_cpumask_var_ptr(local_cpu_mask);
	पूर्णांक this_cpu = smp_processor_id();
	पूर्णांक cpu      = task_cpu(task);
	पूर्णांक ret;

	/* Make sure the mask is initialized first */
	अगर (unlikely(!lowest_mask))
		वापस -1;

	अगर (task->nr_cpus_allowed == 1)
		वापस -1; /* No other targets possible */

	/*
	 * If we're on asym system ensure we consider the different capacities
	 * of the CPUs when searching for the lowest_mask.
	 */
	अगर (sched_asym_cpucap_active()) {

		ret = cpupri_find_fitness(&task_rq(task)->rd->cpupri,
					  task, lowest_mask,
					  rt_task_fits_capacity);
	} अन्यथा {

		ret = cpupri_find(&task_rq(task)->rd->cpupri,
				  task, lowest_mask);
	}

	अगर (!ret)
		वापस -1; /* No targets found */

	/*
	 * At this point we have built a mask of CPUs representing the
	 * lowest priority tasks in the system.  Now we want to elect
	 * the best one based on our affinity and topology.
	 *
	 * We prioritize the last CPU that the task executed on since
	 * it is most likely cache-hot in that location.
	 */
	अगर (cpumask_test_cpu(cpu, lowest_mask))
		वापस cpu;

	/*
	 * Otherwise, we consult the sched_domains span maps to figure
	 * out which CPU is logically closest to our hot cache data.
	 */
	अगर (!cpumask_test_cpu(this_cpu, lowest_mask))
		this_cpu = -1; /* Skip this_cpu opt if not among lowest */

	rcu_read_lock();
	for_each_domain(cpu, sd) {
		अगर (sd->flags & SD_WAKE_AFFINE) {
			पूर्णांक best_cpu;

			/*
			 * "this_cpu" is cheaper to preempt than a
			 * remote processor.
			 */
			अगर (this_cpu != -1 &&
			    cpumask_test_cpu(this_cpu, sched_domain_span(sd))) {
				rcu_read_unlock();
				वापस this_cpu;
			}

			best_cpu = cpumask_any_and_distribute(lowest_mask,
							      sched_domain_span(sd));
			अगर (best_cpu < nr_cpu_ids) {
				rcu_read_unlock();
				वापस best_cpu;
			}
		}
	}
	rcu_read_unlock();

	/*
	 * And finally, if there were no matches within the domains
	 * just give the caller *something* to work with from the compatible
	 * locations.
	 */
	अगर (this_cpu != -1)
		वापस this_cpu;

	cpu = cpumask_any_distribute(lowest_mask);
	अगर (cpu < nr_cpu_ids)
		वापस cpu;

	वापस -1;
}

अटल काष्ठा task_struct *pick_अग्र_pushable_task(काष्ठा rq *rq)
{
	काष्ठा plist_शीर्ष *शीर्ष = &rq->rt.pushable_tasks;
	काष्ठा task_struct *i, *p = शून्य;

	अगर (!has_pushable_tasks(rq))
		वापस शून्य;

	plist_for_each_entry(i, शीर्ष, pushable_tasks) {
		/* skip tasks that cannot be migrated */
		अगर (!task_on_cpu(rq, i) && !is_migration_disabled(i)) {
			p = i;
			अवरोध;
		}
	}

	अगर (!p)
		वापस शून्य;

	BUG_ON(rq->cpu != task_cpu(p));
	BUG_ON(task_current(rq, p));
	BUG_ON(task_current_donor(rq, p));
	BUG_ON(p->nr_cpus_allowed <= 1);

	BUG_ON(!task_on_rq_queued(p));
	BUG_ON(!rt_task(p));

	वापस p;
}

/* Will lock the rq it finds */
अटल काष्ठा rq *find_lock_lowest_rq(काष्ठा task_struct *task, काष्ठा rq *rq)
{
	काष्ठा rq *lowest_rq = शून्य;
	पूर्णांक tries;
	पूर्णांक cpu;

	क्रम (tries = 0; tries < RT_MAX_TRIES; tries++) {
		cpu = find_lowest_rq(task);

		अगर ((cpu == -1) || (cpu == rq->cpu))
			अवरोध;

		lowest_rq = cpu_rq(cpu);

		अगर (lowest_rq->rt.highest_prio.curr <= task->prio) {
			/*
			 * Target rq has tasks of equal or higher priority,
			 * retrying does not release any lock and is unlikely
			 * to yield a different result.
			 */
			lowest_rq = शून्य;
			अवरोध;
		}

		/* if the prio of this runqueue changed, try again */
		अगर (double_lock_balance(rq, lowest_rq)) {
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
			अगर (unlikely(is_migration_disabled(task) ||
				     !cpumask_test_cpu(lowest_rq->cpu, &task->cpus_mask) ||
				     task != pick_अग्र_pushable_task(rq))) {

				double_unlock_balance(rq, lowest_rq);
				lowest_rq = शून्य;
				अवरोध;
			}
		}

		/* If this rq is still suitable use it. */
		अगर (lowest_rq->rt.highest_prio.curr > task->prio)
			अवरोध;

		/* try again */
		double_unlock_balance(rq, lowest_rq);
		lowest_rq = शून्य;
	}

	वापस lowest_rq;
}

/*
 * If the current CPU has more than one RT task, see if the non
 * running task can migrate over to a CPU that is running a task
 * of lesser priority.
 */
अटल पूर्णांक धकेलो_rt_task(काष्ठा rq *rq, bool pull)
{
	काष्ठा task_struct *अग्र_task;
	काष्ठा rq *lowest_rq;
	पूर्णांक ret = 0;

	अगर (!rq->rt.overloaded)
		वापस 0;

	अग्र_task = pick_अग्र_pushable_task(rq);
	अगर (!अग्र_task)
		वापस 0;

retry:
	/*
	 * It's possible that the next_task slipped in of
	 * higher priority than current. If that's the case
	 * just reschedule current.
	 */
	अगर (unlikely(अग्र_task->prio < rq->donor->prio)) {
		resched_curr(rq);
		वापस 0;
	}

	अगर (is_migration_disabled(अग्र_task)) {
		काष्ठा task_struct *धकेलो_task = शून्य;
		पूर्णांक cpu;

		अगर (!pull || rq->धकेलो_busy)
			वापस 0;

		/*
		 * Invoking find_lowest_rq() on anything but an RT task doesn't
		 * make sense. Per the above priority check, curr has to
		 * be of higher priority than next_task, so no need to
		 * reschedule when bailing out.
		 *
		 * Note that the stoppers are masqueraded as SCHED_FIFO
		 * (cf. sched_set_stop_task()), so we can't rely on rt_task().
		 */
		अगर (rq->donor->sched_class != &rt_sched_class)
			वापस 0;

		cpu = find_lowest_rq(rq->curr);
		अगर (cpu == -1 || cpu == rq->cpu)
			वापस 0;

		/*
		 * Given we found a CPU with lower priority than @next_task,
		 * therefore it should be running. However we cannot migrate it
		 * to this other CPU, instead attempt to push the current
		 * running task on this CPU away.
		 */
		धकेलो_task = get_धकेलो_task(rq);
		अगर (धकेलो_task) {
			preempt_disable();
			raw_spin_rq_unlock(rq);
			stop_one_cpu_nowait(rq->cpu, धकेलो_cpu_stop,
					    धकेलो_task, &rq->धकेलो_work);
			preempt_enable();
			raw_spin_rq_lock(rq);
		}

		वापस 0;
	}

	अगर (WARN_ON(अग्र_task == rq->curr))
		वापस 0;

	/* We might release rq lock */
	get_task_struct(अग्र_task);

	/* find_lock_lowest_rq locks the rq if found */
	lowest_rq = find_lock_lowest_rq(अग्र_task, rq);
	अगर (!lowest_rq) {
		काष्ठा task_struct *task;
		/*
		 * find_lock_lowest_rq releases rq->lock
		 * so it is possible that next_task has migrated.
		 *
		 * We need to make sure that the task is still on the same
		 * run-queue and is also still the next task eligible for
		 * pushing.
		 */
		task = pick_अग्र_pushable_task(rq);
		अगर (task == अग्र_task) {
			/*
			 * The task hasn't migrated, and is still the next
			 * eligible task, but we failed to find a run-queue
			 * to push it to.  Do not retry in this case, since
			 * other CPUs will pull from us when ready.
			 */
			जाओ out;
		}

		अगर (!task)
			/* No more tasks, just exit */
			जाओ out;

		/*
		 * Something has shifted, try again.
		 */
		put_task_struct(अग्र_task);
		अग्र_task = task;
		जाओ retry;
	}

	move_queued_task_locked(rq, lowest_rq, अग्र_task);
	resched_curr(lowest_rq);
	ret = 1;

	double_unlock_balance(rq, lowest_rq);
out:
	put_task_struct(अग्र_task);

	वापस ret;
}

अटल व्योम धकेलो_rt_tasks(काष्ठा rq *rq)
{
	/* push_rt_task will return true if it moved an RT */
	जबतक (धकेलो_rt_task(rq, false))
		;
}

#अगर_घोषित HAVE_RT_धकेलो_IPI

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
अटल पूर्णांक rto_अग्र_cpu(काष्ठा मूल_domain *rd)
{
	पूर्णांक this_cpu = smp_processor_id();
	पूर्णांक अग्र;
	पूर्णांक cpu;

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
	क्रम (;;) {

		/* When rto_cpu is -1 this acts like cpumask_first() */
		cpu = cpumask_अग्र(rd->rto_cpu, rd->rto_mask);

		rd->rto_cpu = cpu;

		/* Do not send IPI to self */
		अगर (cpu == this_cpu)
			जारी;

		अगर (cpu < nr_cpu_ids)
			वापस cpu;

		rd->rto_cpu = -1;

		/*
		 * ACQUIRE ensures we see the @rto_mask changes
		 * made prior to the @next value observed.
		 *
		 * Matches WMB in rt_set_overload().
		 */
		अग्र = atomic_read_acquire(&rd->rto_loop_अग्र);

		अगर (rd->rto_loop == अग्र)
			अवरोध;

		rd->rto_loop = अग्र;
	}

	वापस -1;
}

अटल अंतरभूत bool rto_start_trylock(atomic_t *v)
{
	वापस !atomic_cmpxchg_acquire(v, 0, 1);
}

अटल अंतरभूत व्योम rto_start_unlock(atomic_t *v)
{
	atomic_set_release(v, 0);
}

अटल व्योम tell_cpu_to_धकेलो(काष्ठा rq *rq)
{
	पूर्णांक cpu = -1;

	/* Keep the loop going if the IPI is currently active */
	atomic_inc(&rq->rd->rto_loop_अग्र);

	/* Only one CPU can initiate a loop at a time */
	अगर (!rto_start_trylock(&rq->rd->rto_loop_start))
		वापस;

	raw_spin_lock(&rq->rd->rto_lock);

	/*
	 * The rto_cpu is updated under the lock, if it has a valid CPU
	 * then the IPI is still running and will continue due to the
	 * update to loop_next, and nothing needs to be done here.
	 * Otherwise it is finishing up and an IPI needs to be sent.
	 */
	अगर (rq->rd->rto_cpu < 0)
		cpu = rto_अग्र_cpu(rq->rd);

	raw_spin_unlock(&rq->rd->rto_lock);

	rto_start_unlock(&rq->rd->rto_loop_start);

	अगर (cpu >= 0) {
		/* Make sure the rd does not get freed while pushing */
		sched_get_rd(rq->rd);
		irq_work_queue_on(&rq->rd->rto_धकेलो_work, cpu);
	}
}

/* Called from hardirq context */
व्योम rto_धकेलो_irq_work_func(काष्ठा irq_work *work)
{
	काष्ठा मूल_domain *rd =
		container_of(work, काष्ठा मूल_domain, rto_धकेलो_work);
	काष्ठा rq *rq;
	पूर्णांक cpu;

	rq = this_rq();

	/*
	 * We do not need to grab the lock to check for has_pushable_tasks.
	 * When it gets updated, a check is made if a push is possible.
	 */
	अगर (has_pushable_tasks(rq)) {
		raw_spin_rq_lock(rq);
		जबतक (धकेलो_rt_task(rq, true))
			;
		raw_spin_rq_unlock(rq);
	}

	raw_spin_lock(&rd->rto_lock);

	/* Pass the IPI to the next rt overloaded queue */
	cpu = rto_अग्र_cpu(rd);

	raw_spin_unlock(&rd->rto_lock);

	अगर (cpu < 0) {
		sched_put_rd(rd);
		वापस;
	}

	/* Try the next RT overloaded CPU */
	irq_work_queue_on(&rd->rto_धकेलो_work, cpu);
}
#पूर्ण_अगर /* HAVE_RT_PUSH_IPI */

अटल व्योम pull_rt_task(काष्ठा rq *this_rq)
{
	पूर्णांक this_cpu = this_rq->cpu, cpu;
	bool resched = false;
	काष्ठा task_struct *p, *धकेलो_task;
	काष्ठा rq *src_rq;
	पूर्णांक rt_overload_गिनती = rt_overloaded(this_rq);

	अगर (likely(!rt_overload_गिनती))
		वापस;

	/*
	 * Match the barrier from rt_set_overloaded; this guarantees that if we
	 * see overloaded we must also see the rto_mask bit.
	 */
	smp_rmb();

	/* If we are the only overloaded CPU do nothing */
	अगर (rt_overload_गिनती == 1 &&
	    cpumask_test_cpu(this_rq->cpu, this_rq->rd->rto_mask))
		वापस;

#अगर_घोषित HAVE_RT_धकेलो_IPI
	अगर (sched_feat(RT_धकेलो_IPI)) {
		tell_cpu_to_धकेलो(this_rq);
		वापस;
	}
#पूर्ण_अगर

	for_each_cpu(cpu, this_rq->rd->rto_mask) {
		अगर (this_cpu == cpu)
			जारी;

		src_rq = cpu_rq(cpu);

		/*
		 * Don't bother taking the src_rq->lock if the next highest
		 * task is known to be lower-priority than our current task.
		 * This may look racy, but if this value is about to go
		 * logically higher, the src_rq will push this task away.
		 * And if its going logically lower, we do not care
		 */
		अगर (src_rq->rt.highest_prio.अग्र >=
		    this_rq->rt.highest_prio.curr)
			जारी;

		/*
		 * We can potentially drop this_rq's lock in
		 * double_lock_balance, and another CPU could
		 * alter this_rq
		 */
		धकेलो_task = शून्य;
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
		अगर (p && (p->prio < this_rq->rt.highest_prio.curr)) {
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
			अगर (p->prio < src_rq->donor->prio)
				जाओ skip;

			अगर (is_migration_disabled(p)) {
				धकेलो_task = get_धकेलो_task(src_rq);
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

		अगर (धकेलो_task) {
			preempt_disable();
			raw_spin_rq_unlock(this_rq);
			stop_one_cpu_nowait(src_rq->cpu, धकेलो_cpu_stop,
					    धकेलो_task, &src_rq->धकेलो_work);
			preempt_enable();
			raw_spin_rq_lock(this_rq);
		}
	}

	अगर (resched)
		resched_curr(this_rq);
}

/*
 * If we are not running and we are not going to reschedule soon, we should
 * try to push tasks away now
 */
अटल व्योम task_woken_rt(काष्ठा rq *rq, काष्ठा task_struct *p)
{
	bool need_to_धकेलो = !task_on_cpu(rq, p) &&
			    !test_tsk_need_resched(rq->curr) &&
			    p->nr_cpus_allowed > 1 &&
			    (dl_task(rq->donor) || rt_task(rq->donor)) &&
			    (rq->curr->nr_cpus_allowed < 2 ||
			     rq->donor->prio <= p->prio);

	अगर (need_to_धकेलो)
		धकेलो_rt_tasks(rq);
}

/* Assumes rq->lock is held */
अटल व्योम rq_online_rt(काष्ठा rq *rq)
{
	अगर (rq->rt.overloaded)
		rt_set_overload(rq);

	__enable_runtime(rq);

	cpupri_set(&rq->rd->cpupri, rq->cpu, rq->rt.highest_prio.curr);
}

/* Assumes rq->lock is held */
अटल व्योम rq_offline_rt(काष्ठा rq *rq)
{
	अगर (rq->rt.overloaded)
		rt_clear_overload(rq);

	__disable_runtime(rq);

	cpupri_set(&rq->rd->cpupri, rq->cpu, CPUPRI_INVALID);
}

/*
 * When switch from the rt queue, we bring ourselves to a position
 * that we might want to pull RT tasks from other runqueues.
 */
अटल व्योम switched_from_rt(काष्ठा rq *rq, काष्ठा task_struct *p)
{
	/*
	 * If there are other RT tasks then we will reschedule
	 * and the scheduling of the other RT tasks will handle
	 * the balancing. But if we are the last RT task
	 * we may need to handle the pulling of RT tasks
	 * now.
	 */
	अगर (!task_on_rq_queued(p) || rq->rt.rt_nr_running)
		वापस;

	rt_queue_pull_task(rq);
}

व्योम __init init_sched_rt_class(व्योम)
{
	अचिन्हित पूर्णांक i;

	for_each_possible_cpu(i) {
		zalloc_cpumask_var_गाँठ(&per_cpu(local_cpu_mask, i),
					GFP_KERNEL, cpu_to_गाँठ(i));
	}
}

/*
 * When switching a task to RT, we may overload the runqueue
 * with RT tasks. In this case we try to push them off to
 * other runqueues.
 */
अटल व्योम switched_to_rt(काष्ठा rq *rq, काष्ठा task_struct *p)
{
	/*
	 * If we are running, update the avg_rt tracking, as the running time
	 * will now on be accounted into the latter.
	 */
	अगर (task_current(rq, p)) {
		update_rt_rq_load_avg(rq_clock_pelt(rq), rq, 0);
		वापस;
	}

	/*
	 * If we are not running we may need to preempt the current
	 * running task. If that current running task is also an RT task
	 * then see if we can move to another run queue.
	 */
	अगर (task_on_rq_queued(p)) {
		अगर (p->nr_cpus_allowed > 1 && rq->rt.overloaded)
			rt_queue_धकेलो_tasks(rq);
		अगर (p->prio < rq->donor->prio && cpu_online(cpu_of(rq)))
			resched_curr(rq);
	}
}

/*
 * Priority of the task has changed. This may cause
 * us to initiate a push or pull.
 */
अटल व्योम
prio_changed_rt(काष्ठा rq *rq, काष्ठा task_struct *p, u64 oldprio)
{
	अगर (!task_on_rq_queued(p))
		वापस;

	अगर (p->prio == oldprio)
		वापस;

	अगर (task_current_donor(rq, p)) {
		/*
		 * If our priority decreases while running, we
		 * may need to pull tasks to this runqueue.
		 */
		अगर (oldprio < p->prio)
			rt_queue_pull_task(rq);

		/*
		 * If there's a higher priority task waiting to run
		 * then reschedule.
		 */
		अगर (p->prio > rq->rt.highest_prio.curr)
			resched_curr(rq);
	} अन्यथा {
		/*
		 * This task is not running, but if it is
		 * greater than the current running task
		 * then reschedule.
		 */
		अगर (p->prio < rq->donor->prio)
			resched_curr(rq);
	}
}

#अगर_घोषित CONFIG_POSIX_TIMERS
अटल व्योम watchdog(काष्ठा rq *rq, काष्ठा task_struct *p)
{
	अचिन्हित दीर्घ soft, hard;

	/* max may change after cur was read, this will be fixed next tick */
	soft = task_rlimit(p, RLIMIT_RTTIME);
	hard = task_rlimit_max(p, RLIMIT_RTTIME);

	अगर (soft != RLIM_INFINITY) {
		अचिन्हित दीर्घ अग्र;

		अगर (p->rt.watchdog_stamp != jiffies) {
			p->rt.timeout++;
			p->rt.watchdog_stamp = jiffies;
		}

		अग्र = DIV_ROUND_UP(min(soft, hard), USEC_PER_SEC/HZ);
		अगर (p->rt.timeout > अग्र) {
			posix_cputimers_rt_watchdog(&p->posix_cputimers,
						    p->se.योग_exec_runtime);
		}
	}
}
#अन्यथा /* !CONFIG_POSIX_TIMERS: */
अटल अंतरभूत व्योम watchdog(काष्ठा rq *rq, काष्ठा task_struct *p) { }
#पूर्ण_अगर /* !CONFIG_POSIX_TIMERS */

/*
 * scheduler tick hitting a task of our scheduling class.
 *
 * NOTE: This function can be called remotely by the tick offload that
 * goes along full dynticks. Therefore no local assumption can be made
 * and everything must be accessed through the @rq and @curr passed in
 * parameters.
 */
अटल व्योम task_tick_rt(काष्ठा rq *rq, काष्ठा task_struct *p, पूर्णांक queued)
{
	काष्ठा sched_rt_entity *rt_se = &p->rt;

	update_curr_rt(rq);
	update_rt_rq_load_avg(rq_clock_pelt(rq), rq, 1);

	watchdog(rq, p);

	/*
	 * RR tasks need a special form of time-slice management.
	 * FIFO tasks have no timeslices.
	 */
	अगर (p->policy != SCHED_RR)
		वापस;

	अगर (--p->rt.time_slice)
		वापस;

	p->rt.time_slice = sched_rr_timeslice;

	/*
	 * Requeue to the end of queue if we (and all of our ancestors) are not
	 * the only element on the queue
	 */
	for_each_sched_rt_entity(rt_se) {
		अगर (rt_se->run_list.पूर्व != rt_se->run_list.अग्र) {
			requeue_task_rt(rq, p, 0);
			resched_curr(rq);
			वापस;
		}
	}
}

अटल अचिन्हित पूर्णांक get_rr_interval_rt(काष्ठा rq *rq, काष्ठा task_struct *task)
{
	/*
	 * Time slice is 0 for SCHED_FIFO tasks
	 */
	अगर (task->policy == SCHED_RR)
		वापस sched_rr_timeslice;
	अन्यथा
		वापस 0;
}

#अगर_घोषित CONFIG_SCHED_CORE
अटल पूर्णांक task_is_throttled_rt(काष्ठा task_struct *p, पूर्णांक cpu)
{
	काष्ठा rt_rq *rt_rq;

#अगर_घोषित CONFIG_RT_GROUP_SCHED // XXX maybe add task_rt_rq(), see also sched_rt_period_rt_rq
	rt_rq = task_group(p)->rt_rq[cpu];
	WARN_ON(!rt_group_sched_enabled() && rt_rq->tg != &मूल_task_group);
#अन्यथा
	rt_rq = &cpu_rq(cpu)->rt;
#पूर्ण_अगर

	वापस rt_rq_throttled(rt_rq);
}
#पूर्ण_अगर /* CONFIG_SCHED_CORE */

DEFINE_SCHED_CLASS(rt) = {
	.enqueue_task		= enqueue_task_rt,
	.dequeue_task		= dequeue_task_rt,
	.yield_task		= yield_task_rt,

	.wakeup_preempt		= wakeup_preempt_rt,

	.pick_task		= pick_task_rt,
	.put_पूर्व_task		= put_पूर्व_task_rt,
	.set_अग्र_task          = set_अग्र_task_rt,

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

#अगर_घोषित CONFIG_SCHED_CORE
	.task_is_throttled	= task_is_throttled_rt,
#पूर्ण_अगर

#अगर_घोषित CONFIG_UCLAMP_TASK
	.uclamp_enabled		= 1,
#पूर्ण_अगर
};

#अगर_घोषित CONFIG_RT_GROUP_SCHED
/*
 * Ensure that the real time constraints are schedulable.
 */
अटल DEFINE_MUTEX(rt_constraints_mutex);

अटल अंतरभूत पूर्णांक tg_has_rt_tasks(काष्ठा task_group *tg)
{
	काष्ठा task_struct *task;
	काष्ठा css_task_iter it;
	पूर्णांक ret = 0;

	/*
	 * Autogroups do not have RT tasks; see autogroup_create().
	 */
	अगर (task_group_is_autogroup(tg))
		वापस 0;

	css_task_iter_start(&tg->css, 0, &it);
	जबतक (!ret && (task = css_task_iter_अग्र(&it)))
		ret |= rt_task(task);
	css_task_iter_end(&it);

	वापस ret;
}

काष्ठा rt_schedulable_तथ्य {
	काष्ठा task_group *tg;
	u64 rt_period;
	u64 rt_runtime;
};

अटल पूर्णांक tg_rt_schedulable(काष्ठा task_group *tg, व्योम *तथ्य)
{
	काष्ठा rt_schedulable_तथ्य *d = तथ्य;
	काष्ठा task_group *child;
	u64 योग_2, योग = 0;
	u64 period, runtime;

	period = ktime_to_ns(tg->rt_bandwidth.rt_period);
	runtime = tg->rt_bandwidth.rt_runtime;

	अगर (tg == d->tg) {
		period = d->rt_period;
		runtime = d->rt_runtime;
	}

	/*
	 * Cannot have more runtime than the period.
	 */
	अगर (runtime > period && runtime != RUNTIME_INF)
		वापस -EINVAL;

	/*
	 * Ensure we don't starve existing RT tasks if runtime turns zero.
	 */
	अगर (rt_bandwidth_enabled() && !runtime &&
	    tg->rt_bandwidth.rt_runtime && tg_has_rt_tasks(tg))
		वापस -EBUSY;

	योग_2 = to_ratio(period, runtime);

	/*
	 * Nobody can have more than the global setting allows.
	 */
	अगर (योग_2 > to_ratio(global_rt_period(), global_rt_runtime()))
		वापस -EINVAL;

	/*
	 * The sum of our children's runtime should not exceed our own.
	 */
	list_for_each_entry_rcu(child, &tg->children, siblings) {
		period = ktime_to_ns(child->rt_bandwidth.rt_period);
		runtime = child->rt_bandwidth.rt_runtime;

		अगर (child == d->tg) {
			period = d->rt_period;
			runtime = d->rt_runtime;
		}

		योग += to_ratio(period, runtime);
	}

	अगर (योग > योग_2)
		वापस -EINVAL;

	वापस 0;
}

अटल पूर्णांक __rt_schedulable(काष्ठा task_group *tg, u64 period, u64 runtime)
{
	पूर्णांक ret;

	काष्ठा rt_schedulable_तथ्य तथ्य = {
		.tg = tg,
		.rt_period = period,
		.rt_runtime = runtime,
	};

	rcu_read_lock();
	ret = walk_tg_वृक्ष(tg_rt_schedulable, tg_nop, &तथ्य);
	rcu_read_unlock();

	वापस ret;
}

अटल पूर्णांक tg_set_rt_bandwidth(काष्ठा task_group *tg,
		u64 rt_period, u64 rt_runtime)
{
	पूर्णांक i, err = 0;

	/*
	 * Disallowing the root group RT runtime is BAD, it would disallow the
	 * kernel creating (and or operating) RT threads.
	 */
	अगर (tg == &मूल_task_group && rt_runtime == 0)
		वापस -EINVAL;

	/* No period doesn't make any sense. */
	अगर (rt_period == 0)
		वापस -EINVAL;

	/*
	 * Bound quota to defend quota against overflow during bandwidth shift.
	 */
	अगर (rt_runtime != RUNTIME_INF && rt_runtime > max_rt_runtime)
		वापस -EINVAL;

	mutex_lock(&rt_constraints_mutex);
	err = __rt_schedulable(tg, rt_period, rt_runtime);
	अगर (err)
		जाओ unlock;

	raw_spin_lock_irq(&tg->rt_bandwidth.rt_runtime_lock);
	tg->rt_bandwidth.rt_period = ns_to_ktime(rt_period);
	tg->rt_bandwidth.rt_runtime = rt_runtime;

	for_each_possible_cpu(i) {
		काष्ठा rt_rq *rt_rq = tg->rt_rq[i];

		raw_spin_lock(&rt_rq->rt_runtime_lock);
		rt_rq->rt_runtime = rt_runtime;
		raw_spin_unlock(&rt_rq->rt_runtime_lock);
	}
	raw_spin_unlock_irq(&tg->rt_bandwidth.rt_runtime_lock);
unlock:
	mutex_unlock(&rt_constraints_mutex);

	वापस err;
}

पूर्णांक sched_group_set_rt_runtime(काष्ठा task_group *tg, दीर्घ rt_runtime_us)
{
	u64 rt_runtime, rt_period;

	rt_period = ktime_to_ns(tg->rt_bandwidth.rt_period);
	rt_runtime = (u64)rt_runtime_us * NSEC_PER_USEC;
	अगर (rt_runtime_us < 0)
		rt_runtime = RUNTIME_INF;
	अन्यथा अगर ((u64)rt_runtime_us > U64_MAX / NSEC_PER_USEC)
		वापस -EINVAL;

	वापस tg_set_rt_bandwidth(tg, rt_period, rt_runtime);
}

दीर्घ sched_group_rt_runtime(काष्ठा task_group *tg)
{
	u64 rt_runtime_us;

	अगर (tg->rt_bandwidth.rt_runtime == RUNTIME_INF)
		वापस -1;

	rt_runtime_us = tg->rt_bandwidth.rt_runtime;
	do_div(rt_runtime_us, NSEC_PER_USEC);
	वापस rt_runtime_us;
}

पूर्णांक sched_group_set_rt_period(काष्ठा task_group *tg, u64 rt_period_us)
{
	u64 rt_runtime, rt_period;

	अगर (rt_period_us > U64_MAX / NSEC_PER_USEC)
		वापस -EINVAL;

	rt_period = rt_period_us * NSEC_PER_USEC;
	rt_runtime = tg->rt_bandwidth.rt_runtime;

	वापस tg_set_rt_bandwidth(tg, rt_period, rt_runtime);
}

दीर्घ sched_group_rt_period(काष्ठा task_group *tg)
{
	u64 rt_period_us;

	rt_period_us = ktime_to_ns(tg->rt_bandwidth.rt_period);
	do_div(rt_period_us, NSEC_PER_USEC);
	वापस rt_period_us;
}

पूर्णांक sched_rt_can_attach(काष्ठा task_group *tg, काष्ठा task_struct *tsk)
{
	/* Don't accept real-time tasks when there is no way for them to run */
	अगर (rt_group_sched_enabled() && rt_task(tsk) && tg->rt_bandwidth.rt_runtime == 0)
		वापस 0;

	वापस 1;
}

#पूर्ण_अगर /* !CONFIG_RT_GROUP_SCHED */

#अगर_घोषित CONFIG_SYSCTL
अटल पूर्णांक sched_rt_global_validate(व्योम)
{
	अगर ((sysctl_sched_rt_runtime != RUNTIME_INF) &&
		((sysctl_sched_rt_runtime > sysctl_sched_rt_period) ||
		 ((u64)sysctl_sched_rt_runtime *
			NSEC_PER_USEC > max_rt_runtime)))
		वापस -EINVAL;

#अगर_घोषित CONFIG_RT_GROUP_SCHED
	अगर (!rt_group_sched_enabled())
		वापस 0;

	scoped_guard(mutex, &rt_constraints_mutex)
		वापस __rt_schedulable(शून्य, 0, 0);
#पूर्ण_अगर
	वापस 0;
}

अटल पूर्णांक sched_rt_handler(स्थिर काष्ठा ctl_table *table, पूर्णांक ग_लिखो, व्योम *बफर,
		माप_प्रकार *lenp, loff_t *ppos)
{
	पूर्णांक old_period, old_runtime;
	अटल DEFINE_MUTEX(mutex);
	पूर्णांक ret;

	mutex_lock(&mutex);
	sched_domains_mutex_lock();
	old_period = sysctl_sched_rt_period;
	old_runtime = sysctl_sched_rt_runtime;

	ret = proc_dointvec_minmax(table, ग_लिखो, बफर, lenp, ppos);

	अगर (!ret && ग_लिखो) {
		ret = sched_rt_global_validate();
		अगर (ret)
			जाओ undo;

		ret = sched_dl_global_validate();
		अगर (ret)
			जाओ undo;

		sched_dl_do_global();
	}
	अगर (0) {
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

	वापस ret;
}

अटल पूर्णांक sched_rr_handler(स्थिर काष्ठा ctl_table *table, पूर्णांक ग_लिखो, व्योम *बफर,
		माप_प्रकार *lenp, loff_t *ppos)
{
	पूर्णांक ret;
	अटल DEFINE_MUTEX(mutex);

	mutex_lock(&mutex);
	ret = proc_dointvec(table, ग_लिखो, बफर, lenp, ppos);
	/*
	 * Make sure that internally we keep jiffies.
	 * Also, writing zero resets the time-slice to default:
	 */
	अगर (!ret && ग_लिखो) {
		sched_rr_timeslice =
			sysctl_sched_rr_timeslice <= 0 ? RR_TIMESLICE :
			msecs_to_jiffies(sysctl_sched_rr_timeslice);

		अगर (sysctl_sched_rr_timeslice <= 0)
			sysctl_sched_rr_timeslice = jiffies_to_msecs(RR_TIMESLICE);
	}
	mutex_unlock(&mutex);

	वापस ret;
}
#पूर्ण_अगर /* CONFIG_SYSCTL */

व्योम print_rt_stats(काष्ठा seq_file *m, पूर्णांक cpu)
{
	rt_rq_iter_t iter;
	काष्ठा rt_rq *rt_rq;

	rcu_read_lock();
	for_each_rt_rq(rt_rq, iter, cpu_rq(cpu))
		print_rt_rq(m, cpu, rt_rq);
	rcu_read_unlock();
}
