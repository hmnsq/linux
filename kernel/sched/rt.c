// SPDX-License-Identifier: GPL-2.0
/*
 * Real-Time Scheduling Class (mapped to the SCHED_FIFO and SCHED_RR
 * policies)
 */

#अन्तर्भाव "sched.h"
#अन्तर्भाव "pelt.h"

पूर्णाङ्क sched_rr_timeslice = RR_TIMESLICE;
/* More than 4 hours if BW_SHIFT equals 20. */
स्थिर ध्रुव u64 max_rt_runtime = MAX_BW;

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

#यदि_परिभाषितम् CONFIG_SYSCTL
स्थिर पूर्णाङ्क sysctl_sched_rr_timeslice = (MSEC_प्रत्येकम्_SEC * RR_TIMESLICE) / HZ;
स्थिर पूर्णाङ्क sched_rt_handler(ध्रुव संरचना ctl_सारणी *सारणी, पूर्णाङ्क लिख, शून्य *बफर,
		आकार_t *lenp, loff_t *ppos);
स्थिर पूर्णाङ्क sched_rr_handler(ध्रुव संरचना ctl_सारणी *सारणी, पूर्णाङ्क लिख, शून्य *बफर,
		आकार_t *lenp, loff_t *ppos);
स्थिर ध्रुव संरचना ctl_सारणी sched_rt_sysctls[] = {
	{
		.procname       = "sched_rt_period_us",
		.दत्त           = &sysctl_sched_rt_period,
		.maxlen         = परिमाण(पूर्णाङ्क),
		.बहुलकम्           = 0644,
		.proc_handler   = sched_rt_handler,
		.extra1         = SYSCTL_ONE,
		.extra2         = SYSCTL_INT_MAX,
	},
	{
		.procname       = "sched_rt_runtime_us",
		.दत्त           = &sysctl_sched_rt_runtime,
		.maxlen         = परिमाण(पूर्णाङ्क),
		.बहुलकम्           = 0644,
		.proc_handler   = sched_rt_handler,
		.extra1         = SYSCTL_NEG_ONE,
		.extra2         = (शून्य *)&sysctl_sched_rt_period,
	},
	{
		.procname       = "sched_rr_timeslice_ms",
		.दत्त           = &sysctl_sched_rr_timeslice,
		.maxlen         = परिमाण(पूर्णाङ्क),
		.बहुलकम्           = 0644,
		.proc_handler   = sched_rr_handler,
	},
};

स्थिर पूर्णाङ्क __init sched_rt_sysctl_init(शून्य)
{
	पञ्जिकाकोशः_sysctl_init("kernel", sched_rt_sysctls);
	प्रत्यागम 0;
}
late_initcall(sched_rt_sysctl_init);
#यदि_अन्तः /* CONFIG_SYSCTL */

शून्य init_rt_rq(संरचना rt_rq *rt_rq)
{
	संरचना rt_prio_array *array;
	पूर्णाङ्क i;

	array = &rt_rq->सक्रिय;
	प्रति (i = 0; i < MAX_RT_PRIO; i++) {
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

#यदि_परिभाषितम् CONFIG_RT_गणः_SCHED
	rt_rq->rt_time = 0;
	rt_rq->rt_throttled = 0;
	rt_rq->rt_runtime = 0;
	raw_spin_तालकम्_init(&rt_rq->rt_runtime_तालकम्);
	rt_rq->tg = &मूलम्_task_गणः;
#यदि_अन्तः
}

#यदि_परिभाषितम् CONFIG_RT_गणः_SCHED

स्थिर पूर्णाङ्क do_sched_rt_period_timer(संरचना rt_bandwidth *rt_b, पूर्णाङ्क overrun);

स्थिर गणना hrtimer_restart sched_rt_period_timer(संरचना hrtimer *timer)
{
	संरचना rt_bandwidth *rt_b =
		container_सम्बन्धि(timer, संरचना rt_bandwidth, rt_period_timer);
	पूर्णाङ्क idle = 0;
	पूर्णाङ्क overrun;

	raw_spin_तालकम्(&rt_b->rt_runtime_तालकम्);
	प्रति (;;) {
		overrun = hrtimer_forward_now(timer, rt_b->rt_period);
		यदि (!overrun)
			भङ्ग;

		raw_spin_unlock(&rt_b->rt_runtime_तालकम्);
		idle = do_sched_rt_period_timer(rt_b, overrun);
		raw_spin_तालकम्(&rt_b->rt_runtime_तालकम्);
	}
	यदि (idle)
		rt_b->rt_period_सक्रिय = 0;
	raw_spin_unlock(&rt_b->rt_runtime_तालकम्);

	प्रत्यागम idle ? HRTIMER_NORESTART : HRTIMER_RESTART;
}

शून्य init_rt_bandwidth(संरचना rt_bandwidth *rt_b, u64 period, u64 runtime)
{
	rt_b->rt_period = ns_गन्तव्यम्_ktime(period);
	rt_b->rt_runtime = runtime;

	raw_spin_तालकम्_init(&rt_b->rt_runtime_तालकम्);

	hrtimer_सज्जता(&rt_b->rt_period_timer, sched_rt_period_timer, CLOCK_MONOTONIC,
		      HRTIMER_बहुलकम्_REL_HARD);
}

स्थिर अन्तःस्थ शून्य do_start_rt_bandwidth(संरचना rt_bandwidth *rt_b)
{
	raw_spin_तालकम्(&rt_b->rt_runtime_तालकम्);
	यदि (!rt_b->rt_period_सक्रिय) {
		rt_b->rt_period_सक्रिय = 1;
		/*
		 * SCHED_DEADLINE updates the bandwidth, as a run away
		 * RT task with a DL task could hog a CPU. But DL does
		 * not reset the period. If a deadline task was running
		 * without an RT task running, it can cause RT tasks to
		 * throttle when they start up. Kick the timer right away
		 * to update the period.
		 */
		hrtimer_forward_now(&rt_b->rt_period_timer, ns_गन्तव्यम्_ktime(0));
		hrtimer_start_expires(&rt_b->rt_period_timer,
				      HRTIMER_बहुलकम्_ABS_PINNED_HARD);
	}
	raw_spin_unlock(&rt_b->rt_runtime_तालकम्);
}

स्थिर शून्य start_rt_bandwidth(संरचना rt_bandwidth *rt_b)
{
	यदि (!rt_bandwidth_enabled() || rt_b->rt_runtime == RUNTIME_INF)
		प्रत्यागम;

	do_start_rt_bandwidth(rt_b);
}

स्थिर शून्य destroy_rt_bandwidth(संरचना rt_bandwidth *rt_b)
{
	hrtimer_cancel(&rt_b->rt_period_timer);
}

#परिभाषय rt_entity_is_task(rt_se) (!(rt_se)->my_q)

स्थिर अन्तःस्थ संरचना task_struct *rt_task_सम्बन्धि(संरचना sched_rt_entity *rt_se)
{
	WARN_ON_ONCE(!rt_entity_is_task(rt_se));

	प्रत्यागम container_सम्बन्धि(rt_se, संरचना task_struct, rt);
}

स्थिर अन्तःस्थ संरचना rq *rq_सम्बन्धि_rt_rq(संरचना rt_rq *rt_rq)
{
	/* Cannot fold with non-CONFIG_RT_GROUP_SCHED version, layout */
	WARN_ON(!rt_गणः_sched_enabled() && rt_rq->tg != &मूलम्_task_गणः);
	प्रत्यागम rt_rq->rq;
}

स्थिर अन्तःस्थ संरचना rt_rq *rt_rq_सम्बन्धि_se(संरचना sched_rt_entity *rt_se)
{
	WARN_ON(!rt_गणः_sched_enabled() && rt_se->rt_rq->tg != &मूलम्_task_गणः);
	प्रत्यागम rt_se->rt_rq;
}

स्थिर अन्तःस्थ संरचना rq *rq_सम्बन्धि_rt_se(संरचना sched_rt_entity *rt_se)
{
	संरचना rt_rq *rt_rq = rt_se->rt_rq;

	WARN_ON(!rt_गणः_sched_enabled() && rt_rq->tg != &मूलम्_task_गणः);
	प्रत्यागम rt_rq->rq;
}

शून्य unregister_rt_sched_गणः(संरचना task_गणः *tg)
{
	यदि (!rt_गणः_sched_enabled())
		प्रत्यागम;

	यदि (tg->rt_se)
		destroy_rt_bandwidth(&tg->rt_bandwidth);
}

शून्य free_rt_sched_गणः(संरचना task_गणः *tg)
{
	पूर्णाङ्क i;

	यदि (!rt_गणः_sched_enabled())
		प्रत्यागम;

	for_each_possible_cpu(i) {
		यदि (tg->rt_rq)
			kfree(tg->rt_rq[i]);
		यदि (tg->rt_se)
			kfree(tg->rt_se[i]);
	}

	kfree(tg->rt_rq);
	kfree(tg->rt_se);
}

शून्य init_tg_rt_प्रविष्टिः(संरचना task_गणः *tg, संरचना rt_rq *rt_rq,
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
		प्रत्यागम;

	यदि (!parent)
		rt_se->rt_rq = &rq->rt;
	अन्यथा
		rt_se->rt_rq = parent->my_q;

	rt_se->my_q = rt_rq;
	rt_se->parent = parent;
	INIT_LIST_शीर्ष(&rt_se->सततम्_list);
}

पूर्णाङ्क alloc_rt_sched_गणः(संरचना task_गणः *tg, संरचना task_गणः *parent)
{
	संरचना rt_rq *rt_rq;
	संरचना sched_rt_entity *rt_se;
	पूर्णाङ्क i;

	यदि (!rt_गणः_sched_enabled())
		प्रत्यागम 1;

	tg->rt_rq = kzalloc_objs(rt_rq, nr_cpu_ids);
	यदि (!tg->rt_rq)
		गच्छ err;
	tg->rt_se = kzalloc_objs(rt_se, nr_cpu_ids);
	यदि (!tg->rt_se)
		गच्छ err;

	init_rt_bandwidth(&tg->rt_bandwidth, ktime_गन्तव्यम्_ns(global_rt_period()), 0);

	for_each_possible_cpu(i) {
		rt_rq = kzalloc_ग्रन्थिः(परिमाण(संरचना rt_rq),
				     GFP_KERNEL, cpu_गन्तव्यम्_ग्रन्थिः(i));
		यदि (!rt_rq)
			गच्छ err;

		rt_se = kzalloc_ग्रन्थिः(परिमाण(संरचना sched_rt_entity),
				     GFP_KERNEL, cpu_गन्तव्यम्_ग्रन्थिः(i));
		यदि (!rt_se)
			गच्छ err_free_rq;

		init_rt_rq(rt_rq);
		rt_rq->rt_runtime = tg->rt_bandwidth.rt_runtime;
		init_tg_rt_प्रविष्टिः(tg, rt_rq, rt_se, i, parent->rt_se[i]);
	}

	प्रत्यागम 1;

err_free_rq:
	kfree(rt_rq);
err:
	प्रत्यागम 0;
}

#अन्यथा /* !CONFIG_RT_GROUP_SCHED: */

#परिभाषय rt_entity_is_task(rt_se) (1)

स्थिर अन्तःस्थ संरचना task_struct *rt_task_सम्बन्धि(संरचना sched_rt_entity *rt_se)
{
	प्रत्यागम container_सम्बन्धि(rt_se, संरचना task_struct, rt);
}

स्थिर अन्तःस्थ संरचना rq *rq_सम्बन्धि_rt_rq(संरचना rt_rq *rt_rq)
{
	प्रत्यागम container_सम्बन्धि(rt_rq, संरचना rq, rt);
}

स्थिर अन्तःस्थ संरचना rq *rq_सम्बन्धि_rt_se(संरचना sched_rt_entity *rt_se)
{
	संरचना task_struct *p = rt_task_सम्बन्धि(rt_se);

	प्रत्यागम task_rq(p);
}

स्थिर अन्तःस्थ संरचना rt_rq *rt_rq_सम्बन्धि_se(संरचना sched_rt_entity *rt_se)
{
	संरचना rq *rq = rq_सम्बन्धि_rt_se(rt_se);

	प्रत्यागम &rq->rt;
}

शून्य unregister_rt_sched_गणः(संरचना task_गणः *tg) { }

शून्य free_rt_sched_गणः(संरचना task_गणः *tg) { }

पूर्णाङ्क alloc_rt_sched_गणः(संरचना task_गणः *tg, संरचना task_गणः *parent)
{
	प्रत्यागम 1;
}
#यदि_अन्तः /* !CONFIG_RT_GROUP_SCHED */

स्थिर अन्तःस्थ bool need_pull_rt_task(संरचना rq *rq, संरचना task_struct *पूर्व)
{
	/* Try to pull RT tasks here if we lower this rq's prio */
	प्रत्यागम rq->online && rq->rt.highest_prio.curr > पूर्व->prio;
}

स्थिर अन्तःस्थ पूर्णाङ्क rt_overloaded(संरचना rq *rq)
{
	प्रत्यागम atomic_read(&rq->rd->rto_गणनम्);
}

स्थिर अन्तःस्थ शून्य rt_समुच्चयः_overload(संरचना rq *rq)
{
	यदि (!rq->online)
		प्रत्यागम;

	cpumask_समुच्चयः_cpu(rq->cpu, rq->rd->rto_आवरणम्);
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
	atomic_inc(&rq->rd->rto_गणनम्);
}

स्थिर अन्तःस्थ शून्य rt_clear_overload(संरचना rq *rq)
{
	यदि (!rq->online)
		प्रत्यागम;

	/* the order here really doesn't matter */
	atomic_dec(&rq->rd->rto_गणनम्);
	cpumask_clear_cpu(rq->cpu, rq->rd->rto_आवरणम्);
}

स्थिर अन्तःस्थ पूर्णाङ्क has_pushable_tasks(संरचना rq *rq)
{
	प्रत्यागम !plist_शीर्ष_रिक्तानि(&rq->rt.pushable_tasks);
}

स्थिर DEFINE_प्रत्येकम्_CPU(संरचना balance_callback, rt_प्रक्षिप_शीर्ष);
स्थिर DEFINE_प्रत्येकम्_CPU(संरचना balance_callback, rt_pull_शीर्ष);

स्थिर शून्य प्रक्षिप_rt_tasks(संरचना rq *);
स्थिर शून्य pull_rt_task(संरचना rq *);

स्थिर अन्तःस्थ शून्य rt_queue_प्रक्षिप_tasks(संरचना rq *rq)
{
	यदि (!has_pushable_tasks(rq))
		प्रत्यागम;

	queue_balance_callback(rq, &प्रत्येकम्_cpu(rt_प्रक्षिप_शीर्ष, rq->cpu), प्रक्षिप_rt_tasks);
}

स्थिर अन्तःस्थ शून्य rt_queue_pull_task(संरचना rq *rq)
{
	queue_balance_callback(rq, &प्रत्येकम्_cpu(rt_pull_शीर्ष, rq->cpu), pull_rt_task);
}

स्थिर शून्य पङ्क्तौ_निक्षिप_pushable_task(संरचना rq *rq, संरचना task_struct *p)
{
	plist_del(&p->pushable_tasks, &rq->rt.pushable_tasks);
	plist_ग्रन्थिः_init(&p->pushable_tasks, p->prio);
	plist_सङ्कलय(&p->pushable_tasks, &rq->rt.pushable_tasks);

	/* Update the highest prio pushable task */
	यदि (p->prio < rq->rt.highest_prio.अग्र)
		rq->rt.highest_prio.अग्र = p->prio;

	यदि (!rq->rt.overloaded) {
		rt_समुच्चयः_overload(rq);
		rq->rt.overloaded = 1;
	}
}

स्थिर शून्य पङ्क्तेः_निष्कासय_pushable_task(संरचना rq *rq, संरचना task_struct *p)
{
	plist_del(&p->pushable_tasks, &rq->rt.pushable_tasks);

	/* Update the new highest prio pushable task */
	यदि (has_pushable_tasks(rq)) {
		p = plist_first_प्रविष्टिः(&rq->rt.pushable_tasks,
				      संरचना task_struct, pushable_tasks);
		rq->rt.highest_prio.अग्र = p->prio;
	} अन्यथा {
		rq->rt.highest_prio.अग्र = MAX_RT_PRIO-1;

		यदि (rq->rt.overloaded) {
			rt_clear_overload(rq);
			rq->rt.overloaded = 0;
		}
	}
}

स्थिर शून्य पङ्क्तौ_निक्षिप_शिखरम्_rt_rq(संरचना rt_rq *rt_rq);
स्थिर शून्य पङ्क्तेः_निष्कासय_शिखरम्_rt_rq(संरचना rt_rq *rt_rq, अचिह्नित पूर्णाङ्क गणनम्);

स्थिर अन्तःस्थ पूर्णाङ्क on_rt_rq(संरचना sched_rt_entity *rt_se)
{
	प्रत्यागम rt_se->on_rq;
}

#यदि_परिभाषितम् CONFIG_UCLAMP_TASK
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
स्थिर अन्तःस्थ bool rt_task_माति_क्षमता(संरचना task_struct *p, पूर्णाङ्क cpu)
{
	अचिह्नित पूर्णाङ्क min_cap;
	अचिह्नित पूर्णाङ्क max_cap;
	अचिह्नित पूर्णाङ्क cpu_cap;

	/* Only heterogeneous systems can benefit from this check */
	यदि (!sched_asym_cpucap_सक्रिय())
		प्रत्यागम true;

	min_cap = uclamp_eff_मानम्(p, UCLAMP_MIN);
	max_cap = uclamp_eff_मानम्(p, UCLAMP_MAX);

	cpu_cap = arch_scale_cpu_क्षमता(cpu);

	प्रत्यागम cpu_cap >= min(min_cap, max_cap);
}
#अन्यथा /* !CONFIG_UCLAMP_TASK: */
स्थिर अन्तःस्थ bool rt_task_माति_क्षमता(संरचना task_struct *p, पूर्णाङ्क cpu)
{
	प्रत्यागम true;
}
#यदि_अन्तः /* !CONFIG_UCLAMP_TASK */

#यदि_परिभाषितम् CONFIG_RT_गणः_SCHED

स्थिर अन्तःस्थ u64 sched_rt_runtime(संरचना rt_rq *rt_rq)
{
	प्रत्यागम rt_rq->rt_runtime;
}

स्थिर अन्तःस्थ u64 sched_rt_period(संरचना rt_rq *rt_rq)
{
	प्रत्यागम ktime_गन्तव्यम्_ns(rt_rq->tg->rt_bandwidth.rt_period);
}

प्रकारनिर्देश संरचना task_गणः *rt_rq_iter_t;

स्थिर अन्तःस्थ संरचना task_गणः *अग्र_task_गणः(संरचना task_गणः *tg)
{
	यदि (!rt_गणः_sched_enabled()) {
		WARN_ON(tg != &मूलम्_task_गणः);
		प्रत्यागम रिक्त;
	}

	कुरु {
		tg = list_प्रविष्टिः_rcu(tg->list.अग्र,
			typeof(संरचना task_गणः), list);
	} यावत् (&tg->list != &task_groups && task_गणः_is_autogroup(tg));

	यदि (&tg->list == &task_groups)
		tg = रिक्त;

	प्रत्यागम tg;
}

#परिभाषय for_each_rt_rq(rt_rq, iter, rq)					\
	प्रति (iter = &मूलम्_task_गणः;					\
		iter && (rt_rq = iter->rt_rq[cpu_सम्बन्धि(rq)]);		\
		iter = अग्र_task_गणः(iter))

#परिभाषय for_each_sched_rt_entity(rt_se) \
	प्रति (; rt_se; rt_se = rt_se->parent)

स्थिर अन्तःस्थ संरचना rt_rq *गणः_rt_rq(संरचना sched_rt_entity *rt_se)
{
	प्रत्यागम rt_se->my_q;
}

स्थिर शून्य पङ्क्तौ_निक्षिप_rt_entity(संरचना sched_rt_entity *rt_se, अचिह्नित पूर्णाङ्क flags);
स्थिर शून्य पङ्क्तेः_निष्कासय_rt_entity(संरचना sched_rt_entity *rt_se, अचिह्नित पूर्णाङ्क flags);

स्थिर शून्य sched_rt_rq_पङ्क्तौ_निक्षिप(संरचना rt_rq *rt_rq)
{
	संरचना task_struct *donor = rq_सम्बन्धि_rt_rq(rt_rq)->donor;
	संरचना rq *rq = rq_सम्बन्धि_rt_rq(rt_rq);
	संरचना sched_rt_entity *rt_se;

	पूर्णाङ्क cpu = cpu_सम्बन्धि(rq);

	rt_se = rt_rq->tg->rt_se[cpu];

	यदि (rt_rq->rt_nr_running) {
		यदि (!rt_se)
			पङ्क्तौ_निक्षिप_शिखरम्_rt_rq(rt_rq);
		अन्यथा यदि (!on_rt_rq(rt_se))
			पङ्क्तौ_निक्षिप_rt_entity(rt_se, 0);

		यदि (rt_rq->highest_prio.curr < donor->prio)
			resched_curr(rq);
	}
}

स्थिर शून्य sched_rt_rq_पङ्क्तेः_निष्कासय(संरचना rt_rq *rt_rq)
{
	संरचना sched_rt_entity *rt_se;
	पूर्णाङ्क cpu = cpu_सम्बन्धि(rq_सम्बन्धि_rt_rq(rt_rq));

	rt_se = rt_rq->tg->rt_se[cpu];

	यदि (!rt_se) {
		पङ्क्तेः_निष्कासय_शिखरम्_rt_rq(rt_rq, rt_rq->rt_nr_running);
		/* Kick cpufreq (see the comment in kernel/sched/sched.h). */
		cpufreq_update_util(rq_सम्बन्धि_rt_rq(rt_rq), 0);
	}
	अन्यथा यदि (on_rt_rq(rt_se))
		पङ्क्तेः_निष्कासय_rt_entity(rt_se, 0);
}

स्थिर अन्तःस्थ पूर्णाङ्क rt_rq_throttled(संरचना rt_rq *rt_rq)
{
	प्रत्यागम rt_rq->rt_throttled && !rt_rq->rt_nr_boosted;
}

स्थिर पूर्णाङ्क rt_se_boosted(संरचना sched_rt_entity *rt_se)
{
	संरचना rt_rq *rt_rq = गणः_rt_rq(rt_se);
	संरचना task_struct *p;

	यदि (rt_rq)
		प्रत्यागम !!rt_rq->rt_nr_boosted;

	p = rt_task_सम्बन्धि(rt_se);
	प्रत्यागम p->prio != p->प्रसामान्यम्_prio;
}

स्थिर अन्तःस्थ ध्रुव संरचना cpumask *sched_rt_period_आवरणम्(शून्य)
{
	प्रत्यागम this_rq()->rd->विस्तृतिः;
}

स्थिर अन्तःस्थ
संरचना rt_rq *sched_rt_period_rt_rq(संरचना rt_bandwidth *rt_b, पूर्णाङ्क cpu)
{
	प्रत्यागम container_सम्बन्धि(rt_b, संरचना task_गणः, rt_bandwidth)->rt_rq[cpu];
}

स्थिर अन्तःस्थ संरचना rt_bandwidth *sched_rt_bandwidth(संरचना rt_rq *rt_rq)
{
	प्रत्यागम &rt_rq->tg->rt_bandwidth;
}

bool sched_rt_bandwidth_account(संरचना rt_rq *rt_rq)
{
	संरचना rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);

	प्रत्यागम (hrtimer_सक्रिय(&rt_b->rt_period_timer) ||
		rt_rq->rt_time < rt_b->rt_runtime);
}

/*
 * We ran out of runtime, see if we can borrow some from our neighbours.
 */
स्थिर शून्य do_balance_runtime(संरचना rt_rq *rt_rq)
{
	संरचना rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);
	संरचना मूलम्_domain *rd = rq_सम्बन्धि_rt_rq(rt_rq)->rd;
	पूर्णाङ्क i, weight;
	u64 rt_period;

	weight = cpumask_weight(rd->विस्तृतिः);

	raw_spin_तालकम्(&rt_b->rt_runtime_तालकम्);
	rt_period = ktime_गन्तव्यम्_ns(rt_b->rt_period);
	for_each_cpu(i, rd->विस्तृतिः) {
		संरचना rt_rq *iter = sched_rt_period_rt_rq(rt_b, i);
		s64 diff;

		यदि (iter == rt_rq)
			अनुवर्त;

		raw_spin_तालकम्(&iter->rt_runtime_तालकम्);
		/*
		 * Either all rqs have inf runtime and there's nothing to steal
		 * or __disable_runtime() below sets a specific rq to inf to
		 * indicate its been disabled and disallow stealing.
		 */
		यदि (iter->rt_runtime == RUNTIME_INF)
			गच्छ अग्र;

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
				raw_spin_unlock(&iter->rt_runtime_तालकम्);
				भङ्ग;
			}
		}
अग्र:
		raw_spin_unlock(&iter->rt_runtime_तालकम्);
	}
	raw_spin_unlock(&rt_b->rt_runtime_तालकम्);
}

/*
 * Ensure this RQ takes back all the runtime it lend to its neighbours.
 */
स्थिर शून्य __disable_runtime(संरचना rq *rq)
{
	संरचना मूलम्_domain *rd = rq->rd;
	rt_rq_iter_t iter;
	संरचना rt_rq *rt_rq;

	यदि (unlikely(!scheduler_running))
		प्रत्यागम;

	for_each_rt_rq(rt_rq, iter, rq) {
		संरचना rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);
		s64 want;
		पूर्णाङ्क i;

		raw_spin_तालकम्(&rt_b->rt_runtime_तालकम्);
		raw_spin_तालकम्(&rt_rq->rt_runtime_तालकम्);
		/*
		 * Either we're all inf and nobody needs to borrow, or we're
		 * already disabled and thus have nothing to do, or we have
		 * exactly the right amount of runtime to take out.
		 */
		यदि (rt_rq->rt_runtime == RUNTIME_INF ||
				rt_rq->rt_runtime == rt_b->rt_runtime)
			गच्छ balanced;
		raw_spin_unlock(&rt_rq->rt_runtime_तालकम्);

		/*
		 * Calculate the difference between what we started out with
		 * and what we current have, that's the amount of runtime
		 * we lend and now have to reclaim.
		 */
		want = rt_b->rt_runtime - rt_rq->rt_runtime;

		/*
		 * Greedy reclaim, take back as much as we can.
		 */
		for_each_cpu(i, rd->विस्तृतिः) {
			संरचना rt_rq *iter = sched_rt_period_rt_rq(rt_b, i);
			s64 diff;

			/*
			 * Can't reclaim from ourselves or disabled runqueues.
			 */
			यदि (iter == rt_rq || iter->rt_runtime == RUNTIME_INF)
				अनुवर्त;

			raw_spin_तालकम्(&iter->rt_runtime_तालकम्);
			यदि (want > 0) {
				diff = min_t(s64, iter->rt_runtime, want);
				iter->rt_runtime -= diff;
				want -= diff;
			} अन्यथा {
				iter->rt_runtime -= want;
				want -= want;
			}
			raw_spin_unlock(&iter->rt_runtime_तालकम्);

			यदि (!want)
				भङ्ग;
		}

		raw_spin_तालकम्(&rt_rq->rt_runtime_तालकम्);
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
		raw_spin_unlock(&rt_rq->rt_runtime_तालकम्);
		raw_spin_unlock(&rt_b->rt_runtime_तालकम्);

		/* Make rt_rq available for pick_next_task() */
		sched_rt_rq_पङ्क्तौ_निक्षिप(rt_rq);
	}
}

स्थिर शून्य __enable_runtime(संरचना rq *rq)
{
	rt_rq_iter_t iter;
	संरचना rt_rq *rt_rq;

	यदि (unlikely(!scheduler_running))
		प्रत्यागम;

	/*
	 * Reset each runqueue's bandwidth settings
	 */
	for_each_rt_rq(rt_rq, iter, rq) {
		संरचना rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);

		raw_spin_तालकम्(&rt_b->rt_runtime_तालकम्);
		raw_spin_तालकम्(&rt_rq->rt_runtime_तालकम्);
		rt_rq->rt_runtime = rt_b->rt_runtime;
		rt_rq->rt_time = 0;
		rt_rq->rt_throttled = 0;
		raw_spin_unlock(&rt_rq->rt_runtime_तालकम्);
		raw_spin_unlock(&rt_b->rt_runtime_तालकम्);
	}
}

स्थिर शून्य balance_runtime(संरचना rt_rq *rt_rq)
{
	यदि (!sched_feat(RT_RUNTIME_SHARE))
		प्रत्यागम;

	यदि (rt_rq->rt_time > rt_rq->rt_runtime) {
		raw_spin_unlock(&rt_rq->rt_runtime_तालकम्);
		do_balance_runtime(rt_rq);
		raw_spin_तालकम्(&rt_rq->rt_runtime_तालकम्);
	}
}

स्थिर पूर्णाङ्क do_sched_rt_period_timer(संरचना rt_bandwidth *rt_b, पूर्णाङ्क overrun)
{
	पूर्णाङ्क i, idle = 1, throttled = 0;
	ध्रुव संरचना cpumask *विस्तृतिः;

	विस्तृतिः = sched_rt_period_आवरणम्();

	/*
	 * FIXME: isolated CPUs should really leave the root task group,
	 * whether they are isolcpus or were isolated via cpusets, lest
	 * the timer run on a CPU which does not service all runqueues,
	 * potentially leaving other CPUs indefinitely throttled.  If
	 * isolation is really required, the user will turn the throttle
	 * off to kill the perturbations it causes anyway.  Meanwhile,
	 * this maintains functionality for boot and/or troubleshooting.
	 */
	यदि (rt_b == &मूलम्_task_गणः.rt_bandwidth)
		विस्तृतिः = cpu_online_आवरणम्;

	for_each_cpu(i, विस्तृतिः) {
		पूर्णाङ्क पङ्क्तौ_निक्षिप = 0;
		संरचना rt_rq *rt_rq = sched_rt_period_rt_rq(rt_b, i);
		संरचना rq *rq = rq_सम्बन्धि_rt_rq(rt_rq);
		संरचना rq_flags rf;
		पूर्णाङ्क skip;

		/*
		 * When span == cpu_online_mask, taking each rq->lock
		 * can be time-consuming. Try to avoid it when possible.
		 */
		raw_spin_तालकम्(&rt_rq->rt_runtime_तालकम्);
		यदि (!sched_feat(RT_RUNTIME_SHARE) && rt_rq->rt_runtime != RUNTIME_INF)
			rt_rq->rt_runtime = rt_b->rt_runtime;
		skip = !rt_rq->rt_time && !rt_rq->rt_nr_running;
		raw_spin_unlock(&rt_rq->rt_runtime_तालकम्);
		यदि (skip)
			अनुवर्त;

		rq_तालकम्(rq, &rf);
		update_rq_clock(rq);

		यदि (rt_rq->rt_time) {
			u64 runtime;

			raw_spin_तालकम्(&rt_rq->rt_runtime_तालकम्);
			यदि (rt_rq->rt_throttled)
				balance_runtime(rt_rq);
			runtime = rt_rq->rt_runtime;
			rt_rq->rt_time -= min(rt_rq->rt_time, overrun*runtime);
			यदि (rt_rq->rt_throttled && rt_rq->rt_time < runtime) {
				rt_rq->rt_throttled = 0;
				पङ्क्तौ_निक्षिप = 1;

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
			raw_spin_unlock(&rt_rq->rt_runtime_तालकम्);
		} अन्यथा यदि (rt_rq->rt_nr_running) {
			idle = 0;
			यदि (!rt_rq_throttled(rt_rq))
				पङ्क्तौ_निक्षिप = 1;
		}
		यदि (rt_rq->rt_throttled)
			throttled = 1;

		यदि (पङ्क्तौ_निक्षिप)
			sched_rt_rq_पङ्क्तौ_निक्षिप(rt_rq);
		rq_unlock(rq, &rf);
	}

	यदि (!throttled && (!rt_bandwidth_enabled() || rt_b->rt_runtime == RUNTIME_INF))
		प्रत्यागम 1;

	प्रत्यागम idle;
}

स्थिर पूर्णाङ्क sched_rt_runtime_exceeded(संरचना rt_rq *rt_rq)
{
	u64 runtime = sched_rt_runtime(rt_rq);

	यदि (rt_rq->rt_throttled)
		प्रत्यागम rt_rq_throttled(rt_rq);

	यदि (runtime >= sched_rt_period(rt_rq))
		प्रत्यागम 0;

	balance_runtime(rt_rq);
	runtime = sched_rt_runtime(rt_rq);
	यदि (runtime == RUNTIME_INF)
		प्रत्यागम 0;

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
			sched_rt_rq_पङ्क्तेः_निष्कासय(rt_rq);
			प्रत्यागम 1;
		}
	}

	प्रत्यागम 0;
}

#अन्यथा /* !CONFIG_RT_GROUP_SCHED: */

प्रकारनिर्देश संरचना rt_rq *rt_rq_iter_t;

#परिभाषय for_each_rt_rq(rt_rq, iter, rq) \
	प्रति ((शून्य) iter, rt_rq = &rq->rt; rt_rq; rt_rq = रिक्त)

#परिभाषय for_each_sched_rt_entity(rt_se) \
	प्रति (; rt_se; rt_se = रिक्त)

स्थिर अन्तःस्थ संरचना rt_rq *गणः_rt_rq(संरचना sched_rt_entity *rt_se)
{
	प्रत्यागम रिक्त;
}

स्थिर अन्तःस्थ शून्य sched_rt_rq_पङ्क्तौ_निक्षिप(संरचना rt_rq *rt_rq)
{
	संरचना rq *rq = rq_सम्बन्धि_rt_rq(rt_rq);

	यदि (!rt_rq->rt_nr_running)
		प्रत्यागम;

	पङ्क्तौ_निक्षिप_शिखरम्_rt_rq(rt_rq);
	resched_curr(rq);
}

स्थिर अन्तःस्थ शून्य sched_rt_rq_पङ्क्तेः_निष्कासय(संरचना rt_rq *rt_rq)
{
	पङ्क्तेः_निष्कासय_शिखरम्_rt_rq(rt_rq, rt_rq->rt_nr_running);
}

स्थिर अन्तःस्थ पूर्णाङ्क rt_rq_throttled(संरचना rt_rq *rt_rq)
{
	प्रत्यागम false;
}

स्थिर अन्तःस्थ ध्रुव संरचना cpumask *sched_rt_period_आवरणम्(शून्य)
{
	प्रत्यागम cpu_online_आवरणम्;
}

स्थिर अन्तःस्थ
संरचना rt_rq *sched_rt_period_rt_rq(संरचना rt_bandwidth *rt_b, पूर्णाङ्क cpu)
{
	प्रत्यागम &cpu_rq(cpu)->rt;
}

स्थिर शून्य __enable_runtime(संरचना rq *rq) { }
स्थिर शून्य __disable_runtime(संरचना rq *rq) { }

#यदि_अन्तः /* !CONFIG_RT_GROUP_SCHED */

स्थिर अन्तःस्थ पूर्णाङ्क rt_se_prio(संरचना sched_rt_entity *rt_se)
{
#यदि_परिभाषितम् CONFIG_RT_गणः_SCHED
	संरचना rt_rq *rt_rq = गणः_rt_rq(rt_se);

	यदि (rt_rq)
		प्रत्यागम rt_rq->highest_prio.curr;
#यदि_अन्तः

	प्रत्यागम rt_task_सम्बन्धि(rt_se)->prio;
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
		प्रत्यागम;

	delta_exec = update_curr_common(rq);
	यदि (unlikely(delta_exec <= 0))
		प्रत्यागम;

#यदि_परिभाषितम् CONFIG_RT_गणः_SCHED
	संरचना sched_rt_entity *rt_se = &donor->rt;

	यदि (!rt_bandwidth_enabled())
		प्रत्यागम;

	for_each_sched_rt_entity(rt_se) {
		संरचना rt_rq *rt_rq = rt_rq_सम्बन्धि_se(rt_se);
		पूर्णाङ्क exceeded;

		यदि (sched_rt_runtime(rt_rq) != RUNTIME_INF) {
			raw_spin_तालकम्(&rt_rq->rt_runtime_तालकम्);
			rt_rq->rt_time += delta_exec;
			exceeded = sched_rt_runtime_exceeded(rt_rq);
			यदि (exceeded)
				resched_curr(rq);
			raw_spin_unlock(&rt_rq->rt_runtime_तालकम्);
			यदि (exceeded)
				do_start_rt_bandwidth(sched_rt_bandwidth(rt_rq));
		}
	}
#यदि_अन्तः /* CONFIG_RT_GROUP_SCHED */
}

स्थिर शून्य
पङ्क्तेः_निष्कासय_शिखरम्_rt_rq(संरचना rt_rq *rt_rq, अचिह्नित पूर्णाङ्क गणनम्)
{
	संरचना rq *rq = rq_सम्बन्धि_rt_rq(rt_rq);

	BUG_ON(&rq->rt != rt_rq);

	यदि (!rt_rq->rt_queued)
		प्रत्यागम;

	BUG_ON(!rq->nr_running);

	sub_nr_running(rq, गणनम्);
	rt_rq->rt_queued = 0;

}

स्थिर शून्य
पङ्क्तौ_निक्षिप_शिखरम्_rt_rq(संरचना rt_rq *rt_rq)
{
	संरचना rq *rq = rq_सम्बन्धि_rt_rq(rt_rq);

	BUG_ON(&rq->rt != rt_rq);

	यदि (rt_rq->rt_queued)
		प्रत्यागम;

	यदि (rt_rq_throttled(rt_rq))
		प्रत्यागम;

	यदि (rt_rq->rt_nr_running) {
		सङ्कलय_nr_running(rq, rt_rq->rt_nr_running);
		rt_rq->rt_queued = 1;
	}

	/* Kick cpufreq (see the comment in kernel/sched/sched.h). */
	cpufreq_update_util(rq, 0);
}

स्थिर शून्य
inc_rt_prio_smp(संरचना rt_rq *rt_rq, पूर्णाङ्क prio, पूर्णाङ्क पूर्व_prio)
{
	संरचना rq *rq = rq_सम्बन्धि_rt_rq(rt_rq);

	/*
	 * Change rq's cpupri only if rt_rq is the top queue.
	 */
	यदि (IS_ENABLED(CONFIG_RT_गणः_SCHED) && &rq->rt != rt_rq)
		प्रत्यागम;

	यदि (rq->online && prio < पूर्व_prio)
		cpupri_समुच्चयः(&rq->rd->cpupri, rq->cpu, prio);
}

स्थिर शून्य
dec_rt_prio_smp(संरचना rt_rq *rt_rq, पूर्णाङ्क prio, पूर्णाङ्क पूर्व_prio)
{
	संरचना rq *rq = rq_सम्बन्धि_rt_rq(rt_rq);

	/*
	 * Change rq's cpupri only if rt_rq is the top queue.
	 */
	यदि (IS_ENABLED(CONFIG_RT_गणः_SCHED) && &rq->rt != rt_rq)
		प्रत्यागम;

	यदि (rq->online && rt_rq->highest_prio.curr != पूर्व_prio)
		cpupri_समुच्चयः(&rq->rd->cpupri, rq->cpu, rt_rq->highest_prio.curr);
}

स्थिर शून्य
inc_rt_prio(संरचना rt_rq *rt_rq, पूर्णाङ्क prio)
{
	पूर्णाङ्क पूर्व_prio = rt_rq->highest_prio.curr;

	यदि (prio < पूर्व_prio)
		rt_rq->highest_prio.curr = prio;

	inc_rt_prio_smp(rt_rq, prio, पूर्व_prio);
}

स्थिर शून्य
dec_rt_prio(संरचना rt_rq *rt_rq, पूर्णाङ्क prio)
{
	पूर्णाङ्क पूर्व_prio = rt_rq->highest_prio.curr;

	यदि (rt_rq->rt_nr_running) {

		WARN_ON(prio < पूर्व_prio);

		/*
		 * This may have been our highest task, and therefore
		 * we may have some re-computation to do
		 */
		यदि (prio == पूर्व_prio) {
			संरचना rt_prio_array *array = &rt_rq->सक्रिय;

			rt_rq->highest_prio.curr =
				sched_find_first_bit(array->bitmap);
		}

	} अन्यथा {
		rt_rq->highest_prio.curr = MAX_RT_PRIO-1;
	}

	dec_rt_prio_smp(rt_rq, prio, पूर्व_prio);
}

#यदि_परिभाषितम् CONFIG_RT_गणः_SCHED

स्थिर शून्य
inc_rt_गणः(संरचना sched_rt_entity *rt_se, संरचना rt_rq *rt_rq)
{
	यदि (rt_se_boosted(rt_se))
		rt_rq->rt_nr_boosted++;

	start_rt_bandwidth(&rt_rq->tg->rt_bandwidth);
}

स्थिर शून्य
dec_rt_गणः(संरचना sched_rt_entity *rt_se, संरचना rt_rq *rt_rq)
{
	यदि (rt_se_boosted(rt_se))
		rt_rq->rt_nr_boosted--;

	WARN_ON(!rt_rq->rt_nr_running && rt_rq->rt_nr_boosted);
}

#अन्यथा /* !CONFIG_RT_GROUP_SCHED: */

स्थिर शून्य
inc_rt_गणः(संरचना sched_rt_entity *rt_se, संरचना rt_rq *rt_rq)
{
}

स्थिर अन्तःस्थ
शून्य dec_rt_गणः(संरचना sched_rt_entity *rt_se, संरचना rt_rq *rt_rq) {}

#यदि_अन्तः /* !CONFIG_RT_GROUP_SCHED */

स्थिर अन्तःस्थ
अचिह्नित पूर्णाङ्क rt_se_nr_running(संरचना sched_rt_entity *rt_se)
{
	संरचना rt_rq *गणः_rq = गणः_rt_rq(rt_se);

	यदि (गणः_rq)
		प्रत्यागम गणः_rq->rt_nr_running;
	अन्यथा
		प्रत्यागम 1;
}

स्थिर अन्तःस्थ
अचिह्नित पूर्णाङ्क rt_se_rr_nr_running(संरचना sched_rt_entity *rt_se)
{
	संरचना rt_rq *गणः_rq = गणः_rt_rq(rt_se);
	संरचना task_struct *tsk;

	यदि (गणः_rq)
		प्रत्यागम गणः_rq->rr_nr_running;

	tsk = rt_task_सम्बन्धि(rt_se);

	प्रत्यागम (tsk->policy == SCHED_RR) ? 1 : 0;
}

स्थिर अन्तःस्थ
शून्य inc_rt_tasks(संरचना sched_rt_entity *rt_se, संरचना rt_rq *rt_rq)
{
	पूर्णाङ्क prio = rt_se_prio(rt_se);

	WARN_ON(!rt_prio(prio));
	rt_rq->rt_nr_running += rt_se_nr_running(rt_se);
	rt_rq->rr_nr_running += rt_se_rr_nr_running(rt_se);

	inc_rt_prio(rt_rq, prio);
	inc_rt_गणः(rt_se, rt_rq);
}

स्थिर अन्तःस्थ
शून्य dec_rt_tasks(संरचना sched_rt_entity *rt_se, संरचना rt_rq *rt_rq)
{
	WARN_ON(!rt_prio(rt_se_prio(rt_se)));
	WARN_ON(!rt_rq->rt_nr_running);
	rt_rq->rt_nr_running -= rt_se_nr_running(rt_se);
	rt_rq->rr_nr_running -= rt_se_rr_nr_running(rt_se);

	dec_rt_prio(rt_rq, rt_se_prio(rt_se));
	dec_rt_गणः(rt_se, rt_rq);
}

/*
 * Change rt_se->run_list location unless SAVE && !MOVE
 *
 * assumes ENQUEUE/DEQUEUE flags match
 */
स्थिर अन्तःस्थ bool move_entity(अचिह्नित पूर्णाङ्क flags)
{
	यदि ((flags & (पङ्क्तेः_निष्कासय_SAVE | पङ्क्तेः_निष्कासय_MOVE)) == पङ्क्तेः_निष्कासय_SAVE)
		प्रत्यागम false;

	प्रत्यागम true;
}

स्थिर शून्य __delist_rt_entity(संरचना sched_rt_entity *rt_se, संरचना rt_prio_array *array)
{
	list_del_init(&rt_se->सततम्_list);

	यदि (list_रिक्तानि(array->queue + rt_se_prio(rt_se)))
		__clear_bit(rt_se_prio(rt_se), array->bitmap);

	rt_se->on_list = 0;
}

स्थिर अन्तःस्थ संरचना sched_statistics *
__schedstats_from_rt_se(संरचना sched_rt_entity *rt_se)
{
	/* schedstats is not supported for rt group. */
	यदि (!rt_entity_is_task(rt_se))
		प्रत्यागम रिक्त;

	प्रत्यागम &rt_task_सम्बन्धि(rt_se)->stats;
}

स्थिर अन्तःस्थ शून्य
update_stats_wait_start_rt(संरचना rt_rq *rt_rq, संरचना sched_rt_entity *rt_se)
{
	संरचना sched_statistics *stats;
	संरचना task_struct *p = रिक्त;

	यदि (!schedstat_enabled())
		प्रत्यागम;

	यदि (rt_entity_is_task(rt_se))
		p = rt_task_सम्बन्धि(rt_se);

	stats = __schedstats_from_rt_se(rt_se);
	यदि (!stats)
		प्रत्यागम;

	__update_stats_wait_start(rq_सम्बन्धि_rt_rq(rt_rq), p, stats);
}

स्थिर अन्तःस्थ शून्य
update_stats_पङ्क्तौ_निक्षिप_sleeper_rt(संरचना rt_rq *rt_rq, संरचना sched_rt_entity *rt_se)
{
	संरचना sched_statistics *stats;
	संरचना task_struct *p = रिक्त;

	यदि (!schedstat_enabled())
		प्रत्यागम;

	यदि (rt_entity_is_task(rt_se))
		p = rt_task_सम्बन्धि(rt_se);

	stats = __schedstats_from_rt_se(rt_se);
	यदि (!stats)
		प्रत्यागम;

	__update_stats_enqueue_sleeper(rq_सम्बन्धि_rt_rq(rt_rq), p, stats);
}

स्थिर अन्तःस्थ शून्य
update_stats_पङ्क्तौ_निक्षिप_rt(संरचना rt_rq *rt_rq, संरचना sched_rt_entity *rt_se,
			पूर्णाङ्क flags)
{
	यदि (!schedstat_enabled())
		प्रत्यागम;

	यदि (flags & पङ्क्तौ_निक्षिप_WAKEUP)
		update_stats_पङ्क्तौ_निक्षिप_sleeper_rt(rt_rq, rt_se);
}

स्थिर अन्तःस्थ शून्य
update_stats_wait_अन्तः_rt(संरचना rt_rq *rt_rq, संरचना sched_rt_entity *rt_se)
{
	संरचना sched_statistics *stats;
	संरचना task_struct *p = रिक्त;

	यदि (!schedstat_enabled())
		प्रत्यागम;

	यदि (rt_entity_is_task(rt_se))
		p = rt_task_सम्बन्धि(rt_se);

	stats = __schedstats_from_rt_se(rt_se);
	यदि (!stats)
		प्रत्यागम;

	__update_stats_wait_end(rq_सम्बन्धि_rt_rq(rt_rq), p, stats);
}

स्थिर अन्तःस्थ शून्य
update_stats_पङ्क्तेः_निष्कासय_rt(संरचना rt_rq *rt_rq, संरचना sched_rt_entity *rt_se,
			पूर्णाङ्क flags)
{
	संरचना task_struct *p = रिक्त;
	संरचना rq *rq = rq_सम्बन्धि_rt_rq(rt_rq);

	यदि (!schedstat_enabled())
		प्रत्यागम;

	यदि (rt_entity_is_task(rt_se)) {
		p = rt_task_सम्बन्धि(rt_se);

		यदि (p != rq->curr)
			update_stats_wait_अन्तः_rt(rt_rq, rt_se);
	}

	यदि ((flags & पङ्क्तेः_निष्कासय_SLEEP) && p) {
		अचिह्नित पूर्णाङ्क state;

		state = READ_ONCE(p->__state);
		यदि (state & TASK_INTERRUPTIBLE)
			__schedstat_set(p->stats.sleep_start,
					rq_clock(rq_सम्बन्धि_rt_rq(rt_rq)));

		यदि (state & TASK_UNINTERRUPTIBLE)
			__schedstat_set(p->stats.खण्डम्_start,
					rq_clock(rq_सम्बन्धि_rt_rq(rt_rq)));
	}
}

स्थिर शून्य __enqueue_rt_entity(संरचना sched_rt_entity *rt_se, अचिह्नित पूर्णाङ्क flags)
{
	संरचना rt_rq *rt_rq = rt_rq_सम्बन्धि_se(rt_se);
	संरचना rt_prio_array *array = &rt_rq->सक्रिय;
	संरचना rt_rq *गणः_rq = गणः_rt_rq(rt_se);
	संरचना list_शीर्ष *queue = array->queue + rt_se_prio(rt_se);

	/*
	 * Don't enqueue the group if its throttled, or when empty.
	 * The latter is a consequence of the former when a child group
	 * get throttled and the current group doesn't have any other
	 * active members.
	 */
	यदि (गणः_rq && (rt_rq_throttled(गणः_rq) || !गणः_rq->rt_nr_running)) {
		यदि (rt_se->on_list)
			__delist_rt_entity(rt_se, array);
		प्रत्यागम;
	}

	यदि (move_entity(flags)) {
		WARN_ON_ONCE(rt_se->on_list);
		यदि (flags & पङ्क्तौ_निक्षिप_शीर्ष)
			list_सङ्कलय(&rt_se->सततम्_list, queue);
		अन्यथा
			list_सङ्कलय_पुच्छ(&rt_se->सततम्_list, queue);

		__set_bit(rt_se_prio(rt_se), array->bitmap);
		rt_se->on_list = 1;
	}
	rt_se->on_rq = 1;

	inc_rt_tasks(rt_se, rt_rq);
}

स्थिर शून्य __dequeue_rt_entity(संरचना sched_rt_entity *rt_se, अचिह्नित पूर्णाङ्क flags)
{
	संरचना rt_rq *rt_rq = rt_rq_सम्बन्धि_se(rt_se);
	संरचना rt_prio_array *array = &rt_rq->सक्रिय;

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
स्थिर शून्य पङ्क्तेः_निष्कासय_rt_राशिः(संरचना sched_rt_entity *rt_se, अचिह्नित पूर्णाङ्क flags)
{
	संरचना sched_rt_entity *पृष्ठभागः = रिक्त;
	अचिह्नित पूर्णाङ्क rt_nr_running;

	for_each_sched_rt_entity(rt_se) {
		rt_se->पृष्ठभागः = पृष्ठभागः;
		पृष्ठभागः = rt_se;
	}

	rt_nr_running = rt_rq_सम्बन्धि_se(पृष्ठभागः)->rt_nr_running;

	प्रति (rt_se = पृष्ठभागः; rt_se; rt_se = rt_se->पृष्ठभागः) {
		यदि (on_rt_rq(rt_se))
			__dequeue_rt_entity(rt_se, flags);
	}

	पङ्क्तेः_निष्कासय_शिखरम्_rt_rq(rt_rq_सम्बन्धि_se(पृष्ठभागः), rt_nr_running);
}

स्थिर शून्य पङ्क्तौ_निक्षिप_rt_entity(संरचना sched_rt_entity *rt_se, अचिह्नित पूर्णाङ्क flags)
{
	संरचना rq *rq = rq_सम्बन्धि_rt_se(rt_se);

	update_stats_पङ्क्तौ_निक्षिप_rt(rt_rq_सम्बन्धि_se(rt_se), rt_se, flags);

	पङ्क्तेः_निष्कासय_rt_राशिः(rt_se, flags);
	for_each_sched_rt_entity(rt_se)
		__enqueue_rt_entity(rt_se, flags);
	पङ्क्तौ_निक्षिप_शिखरम्_rt_rq(&rq->rt);
}

स्थिर शून्य पङ्क्तेः_निष्कासय_rt_entity(संरचना sched_rt_entity *rt_se, अचिह्नित पूर्णाङ्क flags)
{
	संरचना rq *rq = rq_सम्बन्धि_rt_se(rt_se);

	update_stats_पङ्क्तेः_निष्कासय_rt(rt_rq_सम्बन्धि_se(rt_se), rt_se, flags);

	पङ्क्तेः_निष्कासय_rt_राशिः(rt_se, flags);

	for_each_sched_rt_entity(rt_se) {
		संरचना rt_rq *rt_rq = गणः_rt_rq(rt_se);

		यदि (rt_rq && rt_rq->rt_nr_running)
			__enqueue_rt_entity(rt_se, flags);
	}
	पङ्क्तौ_निक्षिप_शिखरम्_rt_rq(&rq->rt);
}

/*
 * Adding/removing a task to/from a priority array:
 */
स्थिर शून्य
पङ्क्तौ_निक्षिप_task_rt(संरचना rq *rq, संरचना task_struct *p, पूर्णाङ्क flags)
{
	संरचना sched_rt_entity *rt_se = &p->rt;

	यदि (flags & पङ्क्तौ_निक्षिप_WAKEUP)
		rt_se->timeout = 0;

	check_schedstat_required();
	update_stats_wait_start_rt(rt_rq_सम्बन्धि_se(rt_se), rt_se);

	पङ्क्तौ_निक्षिप_rt_entity(rt_se, flags);

	यदि (task_is_blocked(p))
		प्रत्यागम;

	यदि (!task_current(rq, p) && p->nr_cpus_allowed > 1)
		पङ्क्तौ_निक्षिप_pushable_task(rq, p);
}

स्थिर bool पङ्क्तेः_निष्कासय_task_rt(संरचना rq *rq, संरचना task_struct *p, पूर्णाङ्क flags)
{
	संरचना sched_rt_entity *rt_se = &p->rt;

	update_curr_rt(rq);
	पङ्क्तेः_निष्कासय_rt_entity(rt_se, flags);

	पङ्क्तेः_निष्कासय_pushable_task(rq, p);

	प्रत्यागम true;
}

/*
 * Put task to the head or the end of the run list without the overhead of
 * dequeue followed by enqueue.
 */
स्थिर शून्य
requeue_rt_entity(संरचना rt_rq *rt_rq, संरचना sched_rt_entity *rt_se, पूर्णाङ्क शीर्ष)
{
	यदि (on_rt_rq(rt_se)) {
		संरचना rt_prio_array *array = &rt_rq->सक्रिय;
		संरचना list_शीर्ष *queue = array->queue + rt_se_prio(rt_se);

		यदि (शीर्ष)
			list_move(&rt_se->सततम्_list, queue);
		अन्यथा
			list_move_पुच्छ(&rt_se->सततम्_list, queue);
	}
}

स्थिर शून्य requeue_task_rt(संरचना rq *rq, संरचना task_struct *p, पूर्णाङ्क शीर्ष)
{
	संरचना sched_rt_entity *rt_se = &p->rt;
	संरचना rt_rq *rt_rq;

	for_each_sched_rt_entity(rt_se) {
		rt_rq = rt_rq_सम्बन्धि_se(rt_se);
		requeue_rt_entity(rt_rq, rt_se, शीर्ष);
	}
}

स्थिर शून्य yield_task_rt(संरचना rq *rq)
{
	requeue_task_rt(rq, rq->donor, 0);
}

स्थिर पूर्णाङ्क find_निम्नतमम्_rq(संरचना task_struct *task);

स्थिर पूर्णाङ्क
select_task_rq_rt(संरचना task_struct *p, पूर्णाङ्क cpu, पूर्णाङ्क flags)
{
	संरचना task_struct *curr, *donor;
	संरचना rq *rq;
	bool test;

	/* For anything but wake ups, just return the task_cpu */
	यदि (!(flags & (WF_TTWU | WF_FORK)))
		गच्छ बहिर्गमनम्;

	rq = cpu_rq(cpu);

	rcu_read_तालकम्();
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

	यदि (test || !rt_task_माति_क्षमता(p, cpu)) {
		पूर्णाङ्क target = find_निम्नतमम्_rq(p);

		/*
		 * Bail out if we were forcing a migration to find a better
		 * fitting CPU but our search failed.
		 */
		यदि (!test && target != -1 && !rt_task_माति_क्षमता(p, target))
			गच्छ बहिर्गमनम्_unlock;

		/*
		 * Don't bother moving it if the destination CPU is
		 * not running a lower priority task.
		 */
		यदि (target != -1 &&
		    p->prio < cpu_rq(target)->rt.highest_prio.curr)
			cpu = target;
	}

बहिर्गमनम्_unlock:
	rcu_read_unlock();

बहिर्गमनम्:
	प्रत्यागम cpu;
}

स्थिर शून्य check_preempt_equal_prio(संरचना rq *rq, संरचना task_struct *p)
{
	यदि (rq->curr->nr_cpus_allowed == 1 ||
	    !cpupri_find(&rq->rd->cpupri, rq->donor, रिक्त))
		प्रत्यागम;

	/*
	 * p is migratable, so let's not schedule it and
	 * see if it is pushed or pulled somewhere else.
	 */
	यदि (p->nr_cpus_allowed != 1 &&
	    cpupri_find(&rq->rd->cpupri, p, रिक्त))
		प्रत्यागम;

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
		rq_unpin_तालकम्(rq, rf);
		pull_rt_task(rq);
		rq_repin_तालकम्(rq, rf);
	}

	प्रत्यागम sched_stop_runnable(rq) || sched_dl_runnable(rq) || sched_rt_runnable(rq);
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
		प्रत्यागम;

	यदि (p->prio < donor->prio) {
		resched_curr(rq);
		प्रत्यागम;
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

स्थिर अन्तःस्थ शून्य समुच्चयः_अग्र_task_rt(संरचना rq *rq, संरचना task_struct *p, गणना snt_e type)
{
	संरचना sched_rt_entity *rt_se = &p->rt;
	संरचना rt_rq *rt_rq = &rq->rt;

	यदि (type == SNT_REPICK)
		प्रत्यागम;

	p->se.exec_start = rq_clock_task(rq);
	यदि (on_rt_rq(&p->rt))
		update_stats_wait_अन्तः_rt(rt_rq, rt_se);

	/* The running task is never eligible for pushing */
	पङ्क्तेः_निष्कासय_pushable_task(rq, p);

	यदि (type != SNT_PICK)
		प्रत्यागम;

	/*
	 * If prev task was rt, put_prev_task() has already updated the
	 * utilization. We only care of the case where we start to schedule a
	 * rt task
	 */
	यदि (rq->donor->sched_class != &rt_sched_class)
		update_rt_rq_आरोपय_avg(rq_clock_pelt(rq), rq, 0);

	rt_queue_प्रक्षिप_tasks(rq);
}

स्थिर संरचना sched_rt_entity *pick_अग्र_rt_entity(संरचना rt_rq *rt_rq)
{
	संरचना rt_prio_array *array = &rt_rq->सक्रिय;
	संरचना sched_rt_entity *अग्र = रिक्त;
	संरचना list_शीर्ष *queue;
	पूर्णाङ्क idx;

	idx = sched_find_first_bit(array->bitmap);
	BUG_ON(idx >= MAX_RT_PRIO);

	queue = array->queue + idx;
	यदि (WARN_ON_ONCE(list_रिक्तानि(queue)))
		प्रत्यागम रिक्त;
	अग्र = list_प्रविष्टिः(queue->अग्र, संरचना sched_rt_entity, सततम्_list);

	प्रत्यागम अग्र;
}

स्थिर संरचना task_struct *pick_अग्र_task_rt(संरचना rq *rq)
{
	संरचना sched_rt_entity *rt_se;
	संरचना rt_rq *rt_rq  = &rq->rt;

	कुरु {
		rt_se = pick_अग्र_rt_entity(rt_rq);
		यदि (unlikely(!rt_se))
			प्रत्यागम रिक्त;
		rt_rq = गणः_rt_rq(rt_se);
	} यावत् (rt_rq);

	प्रत्यागम rt_task_सम्बन्धि(rt_se);
}

स्थिर संरचना task_struct *pick_task_rt(संरचना rq *rq, संरचना rq_flags *rf)
{
	संरचना task_struct *p;

	यदि (!sched_rt_runnable(rq))
		प्रत्यागम रिक्त;

	p = pick_अग्र_task_rt(rq);

	प्रत्यागम p;
}

स्थिर शून्य put_पूर्व_task_rt(संरचना rq *rq, संरचना task_struct *p, संरचना task_struct *अग्र)
{
	संरचना sched_rt_entity *rt_se = &p->rt;
	संरचना rt_rq *rt_rq = &rq->rt;

	यदि (on_rt_rq(&p->rt))
		update_stats_wait_start_rt(rt_rq, rt_se);

	update_curr_rt(rq);

	update_rt_rq_आरोपय_avg(rq_clock_pelt(rq), rq, 1);

	यदि (task_is_blocked(p))
		प्रत्यागम;
	/*
	 * The previous task needs to be made eligible for pushing
	 * if it is still active
	 */
	यदि (on_rt_rq(&p->rt) && p->nr_cpus_allowed > 1)
		पङ्क्तौ_निक्षिप_pushable_task(rq, p);
}

/* Only try algorithms three times */
#परिभाषय RT_MAX_TRIES 3

/*
 * Return the highest pushable rq's task, which is suitable to be executed
 * on the CPU, NULL otherwise
 */
स्थिर संरचना task_struct *pick_highest_pushable_task(संरचना rq *rq, पूर्णाङ्क cpu)
{
	संरचना plist_शीर्ष *शीर्ष = &rq->rt.pushable_tasks;
	संरचना task_struct *p;

	यदि (!has_pushable_tasks(rq))
		प्रत्यागम रिक्त;

	plist_for_each_प्रविष्टिः(p, शीर्ष, pushable_tasks) {
		यदि (task_is_pushable(rq, p, cpu))
			प्रत्यागम p;
	}

	प्रत्यागम रिक्त;
}

स्थिर DEFINE_प्रत्येकम्_CPU(cpumask_var_t, local_cpu_आवरणम्);

स्थिर पूर्णाङ्क find_निम्नतमम्_rq(संरचना task_struct *task)
{
	संरचना sched_domain *sd;
	संरचना cpumask *निम्नतमम्_आवरणम् = this_cpu_cpumask_var_ptr(local_cpu_आवरणम्);
	पूर्णाङ्क this_cpu = smp_processor_id();
	पूर्णाङ्क cpu      = task_cpu(task);
	पूर्णाङ्क ret;

	/* Make sure the mask is initialized first */
	यदि (unlikely(!निम्नतमम्_आवरणम्))
		प्रत्यागम -1;

	यदि (task->nr_cpus_allowed == 1)
		प्रत्यागम -1; /* No other targets possible */

	/*
	 * If we're on asym system ensure we consider the different capacities
	 * of the CPUs when searching for the lowest_mask.
	 */
	यदि (sched_asym_cpucap_सक्रिय()) {

		ret = cpupri_find_fitness(&task_rq(task)->rd->cpupri,
					  task, निम्नतमम्_आवरणम्,
					  rt_task_माति_क्षमता);
	} अन्यथा {

		ret = cpupri_find(&task_rq(task)->rd->cpupri,
				  task, निम्नतमम्_आवरणम्);
	}

	यदि (!ret)
		प्रत्यागम -1; /* No targets found */

	/*
	 * At this point we have built a mask of CPUs representing the
	 * lowest priority tasks in the system.  Now we want to elect
	 * the best one based on our affinity and topology.
	 *
	 * We prioritize the last CPU that the task executed on since
	 * it is most likely cache-hot in that location.
	 */
	यदि (cpumask_test_cpu(cpu, निम्नतमम्_आवरणम्))
		प्रत्यागम cpu;

	/*
	 * Otherwise, we consult the sched_domains span maps to figure
	 * out which CPU is logically closest to our hot cache data.
	 */
	यदि (!cpumask_test_cpu(this_cpu, निम्नतमम्_आवरणम्))
		this_cpu = -1; /* Skip this_cpu opt if not among lowest */

	rcu_read_तालकम्();
	for_each_domain(cpu, sd) {
		यदि (sd->flags & SD_WAKE_AFFINE) {
			पूर्णाङ्क best_cpu;

			/*
			 * "this_cpu" is cheaper to preempt than a
			 * remote processor.
			 */
			यदि (this_cpu != -1 &&
			    cpumask_test_cpu(this_cpu, sched_domain_विस्तृतिः(sd))) {
				rcu_read_unlock();
				प्रत्यागम this_cpu;
			}

			best_cpu = cpumask_any_and_distribute(निम्नतमम्_आवरणम्,
							      sched_domain_विस्तृतिः(sd));
			यदि (best_cpu < nr_cpu_ids) {
				rcu_read_unlock();
				प्रत्यागम best_cpu;
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
		प्रत्यागम this_cpu;

	cpu = cpumask_any_distribute(निम्नतमम्_आवरणम्);
	यदि (cpu < nr_cpu_ids)
		प्रत्यागम cpu;

	प्रत्यागम -1;
}

स्थिर संरचना task_struct *pick_अग्र_pushable_task(संरचना rq *rq)
{
	संरचना plist_शीर्ष *शीर्ष = &rq->rt.pushable_tasks;
	संरचना task_struct *i, *p = रिक्त;

	यदि (!has_pushable_tasks(rq))
		प्रत्यागम रिक्त;

	plist_for_each_प्रविष्टिः(i, शीर्ष, pushable_tasks) {
		/* skip tasks that cannot be migrated */
		यदि (!task_on_cpu(rq, i) && !is_migration_disabled(i)) {
			p = i;
			भङ्ग;
		}
	}

	यदि (!p)
		प्रत्यागम रिक्त;

	BUG_ON(rq->cpu != task_cpu(p));
	BUG_ON(task_current(rq, p));
	BUG_ON(task_current_donor(rq, p));
	BUG_ON(p->nr_cpus_allowed <= 1);

	BUG_ON(!task_on_rq_queued(p));
	BUG_ON(!rt_task(p));

	प्रत्यागम p;
}

/* Will lock the rq it finds */
स्थिर संरचना rq *find_तालकम्_निम्नतमम्_rq(संरचना task_struct *task, संरचना rq *rq)
{
	संरचना rq *निम्नतमम्_rq = रिक्त;
	पूर्णाङ्क tries;
	पूर्णाङ्क cpu;

	प्रति (tries = 0; tries < RT_MAX_TRIES; tries++) {
		cpu = find_निम्नतमम्_rq(task);

		यदि ((cpu == -1) || (cpu == rq->cpu))
			भङ्ग;

		निम्नतमम्_rq = cpu_rq(cpu);

		यदि (निम्नतमम्_rq->rt.highest_prio.curr <= task->prio) {
			/*
			 * Target rq has tasks of equal or higher priority,
			 * retrying does not release any lock and is unlikely
			 * to yield a different result.
			 */
			निम्नतमम्_rq = रिक्त;
			भङ्ग;
		}

		/* if the prio of this runqueue changed, try again */
		यदि (double_तालकम्_balance(rq, निम्नतमम्_rq)) {
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
				     !cpumask_test_cpu(निम्नतमम्_rq->cpu, &task->cpus_आवरणम्) ||
				     task != pick_अग्र_pushable_task(rq))) {

				double_unlock_balance(rq, निम्नतमम्_rq);
				निम्नतमम्_rq = रिक्त;
				भङ्ग;
			}
		}

		/* If this rq is still suitable use it. */
		यदि (निम्नतमम्_rq->rt.highest_prio.curr > task->prio)
			भङ्ग;

		/* try again */
		double_unlock_balance(rq, निम्नतमम्_rq);
		निम्नतमम्_rq = रिक्त;
	}

	प्रत्यागम निम्नतमम्_rq;
}

/*
 * If the current CPU has more than one RT task, see if the non
 * running task can migrate over to a CPU that is running a task
 * of lesser priority.
 */
स्थिर पूर्णाङ्क प्रक्षिप_rt_task(संरचना rq *rq, bool pull)
{
	संरचना task_struct *अग्र_task;
	संरचना rq *निम्नतमम्_rq;
	पूर्णाङ्क ret = 0;

	यदि (!rq->rt.overloaded)
		प्रत्यागम 0;

	अग्र_task = pick_अग्र_pushable_task(rq);
	यदि (!अग्र_task)
		प्रत्यागम 0;

retry:
	/*
	 * It's possible that the next_task slipped in of
	 * higher priority than current. If that's the case
	 * just reschedule current.
	 */
	यदि (unlikely(अग्र_task->prio < rq->donor->prio)) {
		resched_curr(rq);
		प्रत्यागम 0;
	}

	यदि (is_migration_disabled(अग्र_task)) {
		संरचना task_struct *प्रक्षिप_task = रिक्त;
		पूर्णाङ्क cpu;

		यदि (!pull || rq->प्रक्षिप_busy)
			प्रत्यागम 0;

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
			प्रत्यागम 0;

		cpu = find_निम्नतमम्_rq(rq->curr);
		यदि (cpu == -1 || cpu == rq->cpu)
			प्रत्यागम 0;

		/*
		 * Given we found a CPU with lower priority than @next_task,
		 * therefore it should be running. However we cannot migrate it
		 * to this other CPU, instead attempt to push the current
		 * running task on this CPU away.
		 */
		प्रक्षिप_task = get_प्रक्षिप_task(rq);
		यदि (प्रक्षिप_task) {
			preempt_disable();
			raw_spin_rq_unlock(rq);
			stop_one_cpu_nowait(rq->cpu, प्रक्षिप_cpu_stop,
					    प्रक्षिप_task, &rq->प्रक्षिप_work);
			preempt_enable();
			raw_spin_rq_तालकम्(rq);
		}

		प्रत्यागम 0;
	}

	यदि (WARN_ON(अग्र_task == rq->curr))
		प्रत्यागम 0;

	/* We might release rq lock */
	get_task_struct(अग्र_task);

	/* find_lock_lowest_rq locks the rq if found */
	निम्नतमम्_rq = find_तालकम्_निम्नतमम्_rq(अग्र_task, rq);
	यदि (!निम्नतमम्_rq) {
		संरचना task_struct *task;
		/*
		 * find_lock_lowest_rq releases rq->lock
		 * so it is possible that next_task has migrated.
		 *
		 * We need to make sure that the task is still on the same
		 * run-queue and is also still the next task eligible for
		 * pushing.
		 */
		task = pick_अग्र_pushable_task(rq);
		यदि (task == अग्र_task) {
			/*
			 * The task hasn't migrated, and is still the next
			 * eligible task, but we failed to find a run-queue
			 * to push it to.  Do not retry in this case, since
			 * other CPUs will pull from us when ready.
			 */
			गच्छ बहिर्गमनम्;
		}

		यदि (!task)
			/* No more tasks, just exit */
			गच्छ बहिर्गमनम्;

		/*
		 * Something has shifted, try again.
		 */
		put_task_struct(अग्र_task);
		अग्र_task = task;
		गच्छ retry;
	}

	move_queued_task_locked(rq, निम्नतमम्_rq, अग्र_task);
	resched_curr(निम्नतमम्_rq);
	ret = 1;

	double_unlock_balance(rq, निम्नतमम्_rq);
बहिर्गमनम्:
	put_task_struct(अग्र_task);

	प्रत्यागम ret;
}

स्थिर शून्य प्रक्षिप_rt_tasks(संरचना rq *rq)
{
	/* push_rt_task will return true if it moved an RT */
	यावत् (प्रक्षिप_rt_task(rq, false))
		;
}

#यदि_परिभाषितम् HAVE_RT_प्रक्षिप_IPI

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
स्थिर पूर्णाङ्क rto_अग्र_cpu(संरचना मूलम्_domain *rd)
{
	पूर्णाङ्क this_cpu = smp_processor_id();
	पूर्णाङ्क अग्र;
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
	प्रति (;;) {

		/* When rto_cpu is -1 this acts like cpumask_first() */
		cpu = cpumask_अग्र(rd->rto_cpu, rd->rto_आवरणम्);

		rd->rto_cpu = cpu;

		/* Do not send IPI to self */
		यदि (cpu == this_cpu)
			अनुवर्त;

		यदि (cpu < nr_cpu_ids)
			प्रत्यागम cpu;

		rd->rto_cpu = -1;

		/*
		 * ACQUIRE ensures we see the @rto_mask changes
		 * made prior to the @next value observed.
		 *
		 * Matches WMB in rt_set_overload().
		 */
		अग्र = atomic_read_acquire(&rd->rto_loop_अग्र);

		यदि (rd->rto_loop == अग्र)
			भङ्ग;

		rd->rto_loop = अग्र;
	}

	प्रत्यागम -1;
}

स्थिर अन्तःस्थ bool rto_start_trylock(atomic_t *v)
{
	प्रत्यागम !atomic_cmpxchg_acquire(v, 0, 1);
}

स्थिर अन्तःस्थ शून्य rto_start_unlock(atomic_t *v)
{
	atomic_समुच्चयः_release(v, 0);
}

स्थिर शून्य tell_cpu_गन्तव्यम्_प्रक्षिप(संरचना rq *rq)
{
	पूर्णाङ्क cpu = -1;

	/* Keep the loop going if the IPI is currently active */
	atomic_inc(&rq->rd->rto_loop_अग्र);

	/* Only one CPU can initiate a loop at a time */
	यदि (!rto_start_trylock(&rq->rd->rto_loop_start))
		प्रत्यागम;

	raw_spin_तालकम्(&rq->rd->rto_तालकम्);

	/*
	 * The rto_cpu is updated under the lock, if it has a valid CPU
	 * then the IPI is still running and will continue due to the
	 * update to loop_next, and nothing needs to be done here.
	 * Otherwise it is finishing up and an IPI needs to be sent.
	 */
	यदि (rq->rd->rto_cpu < 0)
		cpu = rto_अग्र_cpu(rq->rd);

	raw_spin_unlock(&rq->rd->rto_तालकम्);

	rto_start_unlock(&rq->rd->rto_loop_start);

	यदि (cpu >= 0) {
		/* Make sure the rd does not get freed while pushing */
		sched_get_rd(rq->rd);
		irq_work_queue_on(&rq->rd->rto_प्रक्षिप_work, cpu);
	}
}

/* Called from hardirq context */
शून्य rto_प्रक्षिप_irq_work_func(संरचना irq_work *work)
{
	संरचना मूलम्_domain *rd =
		container_सम्बन्धि(work, संरचना मूलम्_domain, rto_प्रक्षिप_work);
	संरचना rq *rq;
	पूर्णाङ्क cpu;

	rq = this_rq();

	/*
	 * We do not need to grab the lock to check for has_pushable_tasks.
	 * When it gets updated, a check is made if a push is possible.
	 */
	यदि (has_pushable_tasks(rq)) {
		raw_spin_rq_तालकम्(rq);
		यावत् (प्रक्षिप_rt_task(rq, true))
			;
		raw_spin_rq_unlock(rq);
	}

	raw_spin_तालकम्(&rd->rto_तालकम्);

	/* Pass the IPI to the next rt overloaded queue */
	cpu = rto_अग्र_cpu(rd);

	raw_spin_unlock(&rd->rto_तालकम्);

	यदि (cpu < 0) {
		sched_put_rd(rd);
		प्रत्यागम;
	}

	/* Try the next RT overloaded CPU */
	irq_work_queue_on(&rd->rto_प्रक्षिप_work, cpu);
}
#यदि_अन्तः /* HAVE_RT_PUSH_IPI */

स्थिर शून्य pull_rt_task(संरचना rq *this_rq)
{
	पूर्णाङ्क this_cpu = this_rq->cpu, cpu;
	bool resched = false;
	संरचना task_struct *p, *प्रक्षिप_task;
	संरचना rq *src_rq;
	पूर्णाङ्क rt_overload_गणनम् = rt_overloaded(this_rq);

	यदि (likely(!rt_overload_गणनम्))
		प्रत्यागम;

	/*
	 * Match the barrier from rt_set_overloaded; this guarantees that if we
	 * see overloaded we must also see the rto_mask bit.
	 */
	smp_rmb();

	/* If we are the only overloaded CPU do nothing */
	यदि (rt_overload_गणनम् == 1 &&
	    cpumask_test_cpu(this_rq->cpu, this_rq->rd->rto_आवरणम्))
		प्रत्यागम;

#यदि_परिभाषितम् HAVE_RT_प्रक्षिप_IPI
	यदि (sched_feat(RT_प्रक्षिप_IPI)) {
		tell_cpu_गन्तव्यम्_प्रक्षिप(this_rq);
		प्रत्यागम;
	}
#यदि_अन्तः

	for_each_cpu(cpu, this_rq->rd->rto_आवरणम्) {
		यदि (this_cpu == cpu)
			अनुवर्त;

		src_rq = cpu_rq(cpu);

		/*
		 * Don't bother taking the src_rq->lock if the next highest
		 * task is known to be lower-priority than our current task.
		 * This may look racy, but if this value is about to go
		 * logically higher, the src_rq will push this task away.
		 * And if its going logically lower, we do not care
		 */
		यदि (src_rq->rt.highest_prio.अग्र >=
		    this_rq->rt.highest_prio.curr)
			अनुवर्त;

		/*
		 * We can potentially drop this_rq's lock in
		 * double_lock_balance, and another CPU could
		 * alter this_rq
		 */
		प्रक्षिप_task = रिक्त;
		double_तालकम्_balance(this_rq, src_rq);

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
				प्रक्षिप_task = get_प्रक्षिप_task(src_rq);
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

		यदि (प्रक्षिप_task) {
			preempt_disable();
			raw_spin_rq_unlock(this_rq);
			stop_one_cpu_nowait(src_rq->cpu, प्रक्षिप_cpu_stop,
					    प्रक्षिप_task, &src_rq->प्रक्षिप_work);
			preempt_enable();
			raw_spin_rq_तालकम्(this_rq);
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
	bool need_गन्तव्यम्_प्रक्षिप = !task_on_cpu(rq, p) &&
			    !test_tsk_need_resched(rq->curr) &&
			    p->nr_cpus_allowed > 1 &&
			    (dl_task(rq->donor) || rt_task(rq->donor)) &&
			    (rq->curr->nr_cpus_allowed < 2 ||
			     rq->donor->prio <= p->prio);

	यदि (need_गन्तव्यम्_प्रक्षिप)
		प्रक्षिप_rt_tasks(rq);
}

/* Assumes rq->lock is held */
स्थिर शून्य rq_online_rt(संरचना rq *rq)
{
	यदि (rq->rt.overloaded)
		rt_समुच्चयः_overload(rq);

	__enable_runtime(rq);

	cpupri_समुच्चयः(&rq->rd->cpupri, rq->cpu, rq->rt.highest_prio.curr);
}

/* Assumes rq->lock is held */
स्थिर शून्य rq_offline_rt(संरचना rq *rq)
{
	यदि (rq->rt.overloaded)
		rt_clear_overload(rq);

	__disable_runtime(rq);

	cpupri_समुच्चयः(&rq->rd->cpupri, rq->cpu, CPUPRI_INVALID);
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
		प्रत्यागम;

	rt_queue_pull_task(rq);
}

शून्य __init init_sched_rt_class(शून्य)
{
	अचिह्नित पूर्णाङ्क i;

	for_each_possible_cpu(i) {
		zalloc_cpumask_var_ग्रन्थिः(&प्रत्येकम्_cpu(local_cpu_आवरणम्, i),
					GFP_KERNEL, cpu_गन्तव्यम्_ग्रन्थिः(i));
	}
}

/*
 * When switching a task to RT, we may overload the runqueue
 * with RT tasks. In this case we try to push them off to
 * other runqueues.
 */
स्थिर शून्य switched_गन्तव्यम्_rt(संरचना rq *rq, संरचना task_struct *p)
{
	/*
	 * If we are running, update the avg_rt tracking, as the running time
	 * will now on be accounted into the latter.
	 */
	यदि (task_current(rq, p)) {
		update_rt_rq_आरोपय_avg(rq_clock_pelt(rq), rq, 0);
		प्रत्यागम;
	}

	/*
	 * If we are not running we may need to preempt the current
	 * running task. If that current running task is also an RT task
	 * then see if we can move to another run queue.
	 */
	यदि (task_on_rq_queued(p)) {
		यदि (p->nr_cpus_allowed > 1 && rq->rt.overloaded)
			rt_queue_प्रक्षिप_tasks(rq);
		यदि (p->prio < rq->donor->prio && cpu_online(cpu_सम्बन्धि(rq)))
			resched_curr(rq);
	}
}

/*
 * Priority of the task has changed. This may cause
 * us to initiate a push or pull.
 */
स्थिर शून्य
prio_परिवर्तितम्_rt(संरचना rq *rq, संरचना task_struct *p, u64 oldprio)
{
	यदि (!task_on_rq_queued(p))
		प्रत्यागम;

	यदि (p->prio == oldprio)
		प्रत्यागम;

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

#यदि_परिभाषितम् CONFIG_POSIX_TIMERS
स्थिर शून्य watchdog(संरचना rq *rq, संरचना task_struct *p)
{
	अचिह्नित दीर्घ soft, hard;

	/* max may change after cur was read, this will be fixed next tick */
	soft = task_rlimit(p, RLIMIT_RTTIME);
	hard = task_rlimit_max(p, RLIMIT_RTTIME);

	यदि (soft != RLIM_INFINITY) {
		अचिह्नित दीर्घ अग्र;

		यदि (p->rt.watchdog_stamp != jiffies) {
			p->rt.timeout++;
			p->rt.watchdog_stamp = jiffies;
		}

		अग्र = DIV_आवर्तनम्_ऊर्ध्वम्(min(soft, hard), USEC_प्रत्येकम्_SEC/HZ);
		यदि (p->rt.timeout > अग्र) {
			posix_cputimers_rt_watchdog(&p->posix_cputimers,
						    p->se.योग_exec_runtime);
		}
	}
}
#अन्यथा /* !CONFIG_POSIX_TIMERS: */
स्थिर अन्तःस्थ शून्य watchdog(संरचना rq *rq, संरचना task_struct *p) { }
#यदि_अन्तः /* !CONFIG_POSIX_TIMERS */

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
	update_rt_rq_आरोपय_avg(rq_clock_pelt(rq), rq, 1);

	watchdog(rq, p);

	/*
	 * RR tasks need a special form of time-slice management.
	 * FIFO tasks have no timeslices.
	 */
	यदि (p->policy != SCHED_RR)
		प्रत्यागम;

	यदि (--p->rt.time_खण्डिका)
		प्रत्यागम;

	p->rt.time_खण्डिका = sched_rr_timeslice;

	/*
	 * Requeue to the end of queue if we (and all of our ancestors) are not
	 * the only element on the queue
	 */
	for_each_sched_rt_entity(rt_se) {
		यदि (rt_se->सततम्_list.पूर्व != rt_se->सततम्_list.अग्र) {
			requeue_task_rt(rq, p, 0);
			resched_curr(rq);
			प्रत्यागम;
		}
	}
}

स्थिर अचिह्नित पूर्णाङ्क get_rr_interval_rt(संरचना rq *rq, संरचना task_struct *task)
{
	/*
	 * Time slice is 0 for SCHED_FIFO tasks
	 */
	यदि (task->policy == SCHED_RR)
		प्रत्यागम sched_rr_timeslice;
	अन्यथा
		प्रत्यागम 0;
}

#यदि_परिभाषितम् CONFIG_SCHED_CORE
स्थिर पूर्णाङ्क task_is_throttled_rt(संरचना task_struct *p, पूर्णाङ्क cpu)
{
	संरचना rt_rq *rt_rq;

#यदि_परिभाषितम् CONFIG_RT_गणः_SCHED // XXX maybe add task_rt_rq(), see also sched_rt_period_rt_rq
	rt_rq = task_गणः(p)->rt_rq[cpu];
	WARN_ON(!rt_गणः_sched_enabled() && rt_rq->tg != &मूलम्_task_गणः);
#अन्यथा
	rt_rq = &cpu_rq(cpu)->rt;
#यदि_अन्तः

	प्रत्यागम rt_rq_throttled(rt_rq);
}
#यदि_अन्तः /* CONFIG_SCHED_CORE */

DEFINE_SCHED_CLASS(rt) = {
	.पङ्क्तौ_निक्षिप_task		= पङ्क्तौ_निक्षिप_task_rt,
	.पङ्क्तेः_निष्कासय_task		= पङ्क्तेः_निष्कासय_task_rt,
	.yield_task		= yield_task_rt,

	.wakeup_preempt		= wakeup_preempt_rt,

	.pick_task		= pick_task_rt,
	.put_पूर्व_task		= put_पूर्व_task_rt,
	.समुच्चयः_अग्र_task          = समुच्चयः_अग्र_task_rt,

	.balance		= balance_rt,
	.select_task_rq		= select_task_rq_rt,
	.समुच्चयः_cpus_allowed       = समुच्चयः_cpus_allowed_common,
	.rq_online              = rq_online_rt,
	.rq_offline             = rq_offline_rt,
	.task_woken		= task_woken_rt,
	.switched_from		= switched_from_rt,
	.find_तालकम्_rq		= find_तालकम्_निम्नतमम्_rq,

	.task_tick		= task_tick_rt,

	.get_rr_interval	= get_rr_interval_rt,

	.switched_गन्तव्यम्		= switched_गन्तव्यम्_rt,
	.prio_परिवर्तितम्		= prio_परिवर्तितम्_rt,

	.update_curr		= update_curr_rt,

#यदि_परिभाषितम् CONFIG_SCHED_CORE
	.task_is_throttled	= task_is_throttled_rt,
#यदि_अन्तः

#यदि_परिभाषितम् CONFIG_UCLAMP_TASK
	.uclamp_enabled		= 1,
#यदि_अन्तः
};

#यदि_परिभाषितम् CONFIG_RT_गणः_SCHED
/*
 * Ensure that the real time constraints are schedulable.
 */
स्थिर DEFINE_MUTEX(rt_constraints_mutex);

स्थिर अन्तःस्थ पूर्णाङ्क tg_has_rt_tasks(संरचना task_गणः *tg)
{
	संरचना task_struct *task;
	संरचना css_task_iter it;
	पूर्णाङ्क ret = 0;

	/*
	 * Autogroups do not have RT tasks; see autogroup_create().
	 */
	यदि (task_गणः_is_autogroup(tg))
		प्रत्यागम 0;

	css_task_iter_start(&tg->css, 0, &it);
	यावत् (!ret && (task = css_task_iter_अग्र(&it)))
		ret |= rt_task(task);
	css_task_iter_अन्तः(&it);

	प्रत्यागम ret;
}

संरचना rt_schedulable_दत्त {
	संरचना task_गणः *tg;
	u64 rt_period;
	u64 rt_runtime;
};

स्थिर पूर्णाङ्क tg_rt_schedulable(संरचना task_गणः *tg, शून्य *दत्त)
{
	संरचना rt_schedulable_दत्त *d = दत्त;
	संरचना task_गणः *child;
	u64 योग_2, योग = 0;
	u64 period, runtime;

	period = ktime_गन्तव्यम्_ns(tg->rt_bandwidth.rt_period);
	runtime = tg->rt_bandwidth.rt_runtime;

	यदि (tg == d->tg) {
		period = d->rt_period;
		runtime = d->rt_runtime;
	}

	/*
	 * Cannot have more runtime than the period.
	 */
	यदि (runtime > period && runtime != RUNTIME_INF)
		प्रत्यागम -EINVAL;

	/*
	 * Ensure we don't starve existing RT tasks if runtime turns zero.
	 */
	यदि (rt_bandwidth_enabled() && !runtime &&
	    tg->rt_bandwidth.rt_runtime && tg_has_rt_tasks(tg))
		प्रत्यागम -EBUSY;

	योग_2 = गन्तव्यम्_अनुपातः(period, runtime);

	/*
	 * Nobody can have more than the global setting allows.
	 */
	यदि (योग_2 > गन्तव्यम्_अनुपातः(global_rt_period(), global_rt_runtime()))
		प्रत्यागम -EINVAL;

	/*
	 * The sum of our children's runtime should not exceed our own.
	 */
	list_for_each_प्रविष्टिः_rcu(child, &tg->children, siblings) {
		period = ktime_गन्तव्यम्_ns(child->rt_bandwidth.rt_period);
		runtime = child->rt_bandwidth.rt_runtime;

		यदि (child == d->tg) {
			period = d->rt_period;
			runtime = d->rt_runtime;
		}

		योग += गन्तव्यम्_अनुपातः(period, runtime);
	}

	यदि (योग > योग_2)
		प्रत्यागम -EINVAL;

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क __rt_schedulable(संरचना task_गणः *tg, u64 period, u64 runtime)
{
	पूर्णाङ्क ret;

	संरचना rt_schedulable_दत्त दत्त = {
		.tg = tg,
		.rt_period = period,
		.rt_runtime = runtime,
	};

	rcu_read_तालकम्();
	ret = walk_tg_वृक्षः(tg_rt_schedulable, tg_nop, &दत्त);
	rcu_read_unlock();

	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क tg_समुच्चयः_rt_bandwidth(संरचना task_गणः *tg,
		u64 rt_period, u64 rt_runtime)
{
	पूर्णाङ्क i, err = 0;

	/*
	 * Disallowing the root group RT runtime is BAD, it would disallow the
	 * kernel creating (and or operating) RT threads.
	 */
	यदि (tg == &मूलम्_task_गणः && rt_runtime == 0)
		प्रत्यागम -EINVAL;

	/* No period doesn't make any sense. */
	यदि (rt_period == 0)
		प्रत्यागम -EINVAL;

	/*
	 * Bound quota to defend quota against overflow during bandwidth shift.
	 */
	यदि (rt_runtime != RUNTIME_INF && rt_runtime > max_rt_runtime)
		प्रत्यागम -EINVAL;

	mutex_तालकम्(&rt_constraints_mutex);
	err = __rt_schedulable(tg, rt_period, rt_runtime);
	यदि (err)
		गच्छ unlock;

	raw_spin_तालकम्_irq(&tg->rt_bandwidth.rt_runtime_तालकम्);
	tg->rt_bandwidth.rt_period = ns_गन्तव्यम्_ktime(rt_period);
	tg->rt_bandwidth.rt_runtime = rt_runtime;

	for_each_possible_cpu(i) {
		संरचना rt_rq *rt_rq = tg->rt_rq[i];

		raw_spin_तालकम्(&rt_rq->rt_runtime_तालकम्);
		rt_rq->rt_runtime = rt_runtime;
		raw_spin_unlock(&rt_rq->rt_runtime_तालकम्);
	}
	raw_spin_unlock_irq(&tg->rt_bandwidth.rt_runtime_तालकम्);
unlock:
	mutex_unlock(&rt_constraints_mutex);

	प्रत्यागम err;
}

पूर्णाङ्क sched_गणः_समुच्चयः_rt_runtime(संरचना task_गणः *tg, दीर्घ rt_runtime_us)
{
	u64 rt_runtime, rt_period;

	rt_period = ktime_गन्तव्यम्_ns(tg->rt_bandwidth.rt_period);
	rt_runtime = (u64)rt_runtime_us * NSEC_प्रत्येकम्_USEC;
	यदि (rt_runtime_us < 0)
		rt_runtime = RUNTIME_INF;
	अन्यथा यदि ((u64)rt_runtime_us > U64_MAX / NSEC_प्रत्येकम्_USEC)
		प्रत्यागम -EINVAL;

	प्रत्यागम tg_समुच्चयः_rt_bandwidth(tg, rt_period, rt_runtime);
}

दीर्घ sched_गणः_rt_runtime(संरचना task_गणः *tg)
{
	u64 rt_runtime_us;

	यदि (tg->rt_bandwidth.rt_runtime == RUNTIME_INF)
		प्रत्यागम -1;

	rt_runtime_us = tg->rt_bandwidth.rt_runtime;
	do_div(rt_runtime_us, NSEC_प्रत्येकम्_USEC);
	प्रत्यागम rt_runtime_us;
}

पूर्णाङ्क sched_गणः_समुच्चयः_rt_period(संरचना task_गणः *tg, u64 rt_period_us)
{
	u64 rt_runtime, rt_period;

	यदि (rt_period_us > U64_MAX / NSEC_प्रत्येकम्_USEC)
		प्रत्यागम -EINVAL;

	rt_period = rt_period_us * NSEC_प्रत्येकम्_USEC;
	rt_runtime = tg->rt_bandwidth.rt_runtime;

	प्रत्यागम tg_समुच्चयः_rt_bandwidth(tg, rt_period, rt_runtime);
}

दीर्घ sched_गणः_rt_period(संरचना task_गणः *tg)
{
	u64 rt_period_us;

	rt_period_us = ktime_गन्तव्यम्_ns(tg->rt_bandwidth.rt_period);
	do_div(rt_period_us, NSEC_प्रत्येकम्_USEC);
	प्रत्यागम rt_period_us;
}

पूर्णाङ्क sched_rt_can_attach(संरचना task_गणः *tg, संरचना task_struct *tsk)
{
	/* Don't accept real-time tasks when there is no way for them to run */
	यदि (rt_गणः_sched_enabled() && rt_task(tsk) && tg->rt_bandwidth.rt_runtime == 0)
		प्रत्यागम 0;

	प्रत्यागम 1;
}

#यदि_अन्तः /* !CONFIG_RT_GROUP_SCHED */

#यदि_परिभाषितम् CONFIG_SYSCTL
स्थिर पूर्णाङ्क sched_rt_global_validate(शून्य)
{
	यदि ((sysctl_sched_rt_runtime != RUNTIME_INF) &&
		((sysctl_sched_rt_runtime > sysctl_sched_rt_period) ||
		 ((u64)sysctl_sched_rt_runtime *
			NSEC_प्रत्येकम्_USEC > max_rt_runtime)))
		प्रत्यागम -EINVAL;

#यदि_परिभाषितम् CONFIG_RT_गणः_SCHED
	यदि (!rt_गणः_sched_enabled())
		प्रत्यागम 0;

	scoped_guard(mutex, &rt_constraints_mutex)
		प्रत्यागम __rt_schedulable(रिक्त, 0, 0);
#यदि_अन्तः
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क sched_rt_handler(ध्रुव संरचना ctl_सारणी *सारणी, पूर्णाङ्क लिख, शून्य *बफर,
		आकार_t *lenp, loff_t *ppos)
{
	पूर्णाङ्क old_period, old_runtime;
	स्थिर DEFINE_MUTEX(mutex);
	पूर्णाङ्क ret;

	mutex_तालकम्(&mutex);
	sched_domains_mutex_तालकम्();
	old_period = sysctl_sched_rt_period;
	old_runtime = sysctl_sched_rt_runtime;

	ret = proc_dointvec_minmax(सारणी, लिख, बफर, lenp, ppos);

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

	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क sched_rr_handler(ध्रुव संरचना ctl_सारणी *सारणी, पूर्णाङ्क लिख, शून्य *बफर,
		आकार_t *lenp, loff_t *ppos)
{
	पूर्णाङ्क ret;
	स्थिर DEFINE_MUTEX(mutex);

	mutex_तालकम्(&mutex);
	ret = proc_dointvec(सारणी, लिख, बफर, lenp, ppos);
	/*
	 * Make sure that internally we keep jiffies.
	 * Also, writing zero resets the time-slice to default:
	 */
	यदि (!ret && लिख) {
		sched_rr_timeslice =
			sysctl_sched_rr_timeslice <= 0 ? RR_TIMESLICE :
			msecs_गन्तव्यम्_jiffies(sysctl_sched_rr_timeslice);

		यदि (sysctl_sched_rr_timeslice <= 0)
			sysctl_sched_rr_timeslice = jiffies_गन्तव्यम्_msecs(RR_TIMESLICE);
	}
	mutex_unlock(&mutex);

	प्रत्यागम ret;
}
#यदि_अन्तः /* CONFIG_SYSCTL */

शून्य print_rt_stats(संरचना seq_file *m, पूर्णाङ्क cpu)
{
	rt_rq_iter_t iter;
	संरचना rt_rq *rt_rq;

	rcu_read_तालकम्();
	for_each_rt_rq(rt_rq, iter, cpu_rq(cpu))
		print_rt_rq(m, cpu, rt_rq);
	rcu_read_unlock();
}
