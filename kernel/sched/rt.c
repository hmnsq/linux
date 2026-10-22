// SPDX-License-Identifier: GPL-2.0
/*
 * Real-Time Scheduling Class (mapped to the SCHED_FIFO and SCHED_RR
 * policies)
 */

#شامل "sched.h"
#شامل "pelt.h"

صحیح sched_rr_timeslice = RR_TIMESLICE;
/* More than 4 hours if BW_SHIFT equals 20. */
ساکن ثابت u64 max_rt_runtime = MAX_BW;

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

#اگر_تعریف_شدہ CONFIG_SYSCTL
ساکن صحیح sysctl_sched_rr_timeslice = (MSEC_فی_SEC * RR_TIMESLICE) / HZ;
ساکن صحیح sched_rt_handler(ثابت ساخت ctl_جدول *جدول, صحیح اكتب, خالی *بفر,
		سائز_t *lenp, loff_t *ppos);
ساکن صحیح sched_rr_handler(ثابت ساخت ctl_جدول *جدول, صحیح اكتب, خالی *بفر,
		سائز_t *lenp, loff_t *ppos);
ساکن ثابت ساخت ctl_جدول sched_rt_sysctls[] = {
	{
		.procname       = "sched_rt_period_us",
		.ڈیٹا           = &sysctl_sched_rt_period,
		.maxlen         = حجم(صحیح),
		.منوال           = 0644,
		.proc_handler   = sched_rt_handler,
		.extra1         = SYSCTL_ONE,
		.extra2         = SYSCTL_INT_MAX,
	},
	{
		.procname       = "sched_rt_runtime_us",
		.ڈیٹا           = &sysctl_sched_rt_runtime,
		.maxlen         = حجم(صحیح),
		.منوال           = 0644,
		.proc_handler   = sched_rt_handler,
		.extra1         = SYSCTL_NEG_ONE,
		.extra2         = (خالی *)&sysctl_sched_rt_period,
	},
	{
		.procname       = "sched_rr_timeslice_ms",
		.ڈیٹا           = &sysctl_sched_rr_timeslice,
		.maxlen         = حجم(صحیح),
		.منوال           = 0644,
		.proc_handler   = sched_rr_handler,
	},
};

ساکن صحیح __init sched_rt_sysctl_init(خالی)
{
	رجسٹر_sysctl_init("kernel", sched_rt_sysctls);
	واپس 0;
}
late_initcall(sched_rt_sysctl_init);
#اگر_ختم /* CONFIG_SYSCTL */

خالی init_rt_rq(ساخت rt_rq *rt_rq)
{
	ساخت rt_prio_array *array;
	صحیح i;

	array = &rt_rq->فعال;
	برائے (i = 0; i < MAX_RT_PRIO; i++) {
		INIT_LIST_سر(array->قطار + i);
		__clear_bit(i, array->bitmap);
	}
	/* delimiter for bitsearch: */
	__set_bit(MAX_RT_PRIO, array->bitmap);

	rt_rq->highest_prio.curr = MAX_RT_PRIO-1;
	rt_rq->highest_prio.اگلا = MAX_RT_PRIO-1;
	rt_rq->overloaded = 0;
	plist_سر_init(&rt_rq->pushable_tasks);
	/* We start is dequeued state, because no RT tasks are queued */
	rt_rq->rt_queued = 0;

#اگر_تعریف_شدہ CONFIG_RT_گروہ_SCHED
	rt_rq->rt_وقت = 0;
	rt_rq->rt_throttled = 0;
	rt_rq->rt_runtime = 0;
	raw_spin_تالا_init(&rt_rq->rt_runtime_تالا);
	rt_rq->tg = &جڑ_task_گروہ;
#اگر_ختم
}

#اگر_تعریف_شدہ CONFIG_RT_گروہ_SCHED

ساکن صحیح do_sched_rt_period_timer(ساخت rt_bandwidth *rt_b, صحیح overrun);

ساکن شمار hrtimer_restart sched_rt_period_timer(ساخت hrtimer *timer)
{
	ساخت rt_bandwidth *rt_b =
		container_کا(timer, ساخت rt_bandwidth, rt_period_timer);
	صحیح idle = 0;
	صحیح overrun;

	raw_spin_تالا(&rt_b->rt_runtime_تالا);
	برائے (;;) {
		overrun = hrtimer_forward_now(timer, rt_b->rt_period);
		اگر (!overrun)
			توڑو;

		raw_spin_unlock(&rt_b->rt_runtime_تالا);
		idle = do_sched_rt_period_timer(rt_b, overrun);
		raw_spin_تالا(&rt_b->rt_runtime_تالا);
	}
	اگر (idle)
		rt_b->rt_period_فعال = 0;
	raw_spin_unlock(&rt_b->rt_runtime_تالا);

	واپس idle ? HRTIMER_NORESTART : HRTIMER_RESTART;
}

خالی init_rt_bandwidth(ساخت rt_bandwidth *rt_b, u64 period, u64 runtime)
{
	rt_b->rt_period = ns_مقصد_ktime(period);
	rt_b->rt_runtime = runtime;

	raw_spin_تالا_init(&rt_b->rt_runtime_تالا);

	hrtimer_تیاری(&rt_b->rt_period_timer, sched_rt_period_timer, CLOCK_MONOTONIC,
		      HRTIMER_منوال_REL_HARD);
}

ساکن اندرونی خالی do_آغاز_rt_bandwidth(ساخت rt_bandwidth *rt_b)
{
	raw_spin_تالا(&rt_b->rt_runtime_تالا);
	اگر (!rt_b->rt_period_فعال) {
		rt_b->rt_period_فعال = 1;
		/*
		 * SCHED_DEADLINE updates the bandwidth, as a run away
		 * RT task with a DL task could hog a CPU. But DL does
		 * not reset the period. If a deadline task was running
		 * without an RT task running, it can cause RT tasks to
		 * throttle when they start up. Kick the timer right away
		 * to update the period.
		 */
		hrtimer_forward_now(&rt_b->rt_period_timer, ns_مقصد_ktime(0));
		hrtimer_آغاز_expires(&rt_b->rt_period_timer,
				      HRTIMER_منوال_ABS_PINNED_HARD);
	}
	raw_spin_unlock(&rt_b->rt_runtime_تالا);
}

ساکن خالی آغاز_rt_bandwidth(ساخت rt_bandwidth *rt_b)
{
	اگر (!rt_bandwidth_enabled() || rt_b->rt_runtime == RUNTIME_INF)
		واپس;

	do_آغاز_rt_bandwidth(rt_b);
}

ساکن خالی destroy_rt_bandwidth(ساخت rt_bandwidth *rt_b)
{
	hrtimer_cancel(&rt_b->rt_period_timer);
}

#تعریف_کرو rt_entity_is_task(rt_se) (!(rt_se)->my_q)

ساکن اندرونی ساخت task_struct *rt_task_کا(ساخت sched_rt_entity *rt_se)
{
	WARN_ON_ONCE(!rt_entity_is_task(rt_se));

	واپس container_کا(rt_se, ساخت task_struct, rt);
}

ساکن اندرونی ساخت rq *rq_کا_rt_rq(ساخت rt_rq *rt_rq)
{
	/* Cannot fold with non-CONFIG_RT_GROUP_SCHED version, layout */
	WARN_ON(!rt_گروہ_sched_enabled() && rt_rq->tg != &جڑ_task_گروہ);
	واپس rt_rq->rq;
}

ساکن اندرونی ساخت rt_rq *rt_rq_کا_se(ساخت sched_rt_entity *rt_se)
{
	WARN_ON(!rt_گروہ_sched_enabled() && rt_se->rt_rq->tg != &جڑ_task_گروہ);
	واپس rt_se->rt_rq;
}

ساکن اندرونی ساخت rq *rq_کا_rt_se(ساخت sched_rt_entity *rt_se)
{
	ساخت rt_rq *rt_rq = rt_se->rt_rq;

	WARN_ON(!rt_گروہ_sched_enabled() && rt_rq->tg != &جڑ_task_گروہ);
	واپس rt_rq->rq;
}

خالی unregister_rt_sched_گروہ(ساخت task_گروہ *tg)
{
	اگر (!rt_گروہ_sched_enabled())
		واپس;

	اگر (tg->rt_se)
		destroy_rt_bandwidth(&tg->rt_bandwidth);
}

خالی free_rt_sched_گروہ(ساخت task_گروہ *tg)
{
	صحیح i;

	اگر (!rt_گروہ_sched_enabled())
		واپس;

	for_each_possible_cpu(i) {
		اگر (tg->rt_rq)
			kfree(tg->rt_rq[i]);
		اگر (tg->rt_se)
			kfree(tg->rt_se[i]);
	}

	kfree(tg->rt_rq);
	kfree(tg->rt_se);
}

خالی init_tg_rt_اندراج(ساخت task_گروہ *tg, ساخت rt_rq *rt_rq,
		ساخت sched_rt_entity *rt_se, صحیح cpu,
		ساخت sched_rt_entity *والد)
{
	ساخت rq *rq = cpu_rq(cpu);

	rt_rq->highest_prio.curr = MAX_RT_PRIO-1;
	rt_rq->rt_nr_boosted = 0;
	rt_rq->rq = rq;
	rt_rq->tg = tg;

	tg->rt_rq[cpu] = rt_rq;
	tg->rt_se[cpu] = rt_se;

	اگر (!rt_se)
		واپس;

	اگر (!والد)
		rt_se->rt_rq = &rq->rt;
	ورنہ
		rt_se->rt_rq = والد->my_q;

	rt_se->my_q = rt_rq;
	rt_se->والد = والد;
	INIT_LIST_سر(&rt_se->تسلسل_list);
}

صحیح alloc_rt_sched_گروہ(ساخت task_گروہ *tg, ساخت task_گروہ *والد)
{
	ساخت rt_rq *rt_rq;
	ساخت sched_rt_entity *rt_se;
	صحیح i;

	اگر (!rt_گروہ_sched_enabled())
		واپس 1;

	tg->rt_rq = kzalloc_objs(rt_rq, nr_cpu_ids);
	اگر (!tg->rt_rq)
		جاؤ err;
	tg->rt_se = kzalloc_objs(rt_se, nr_cpu_ids);
	اگر (!tg->rt_se)
		جاؤ err;

	init_rt_bandwidth(&tg->rt_bandwidth, ktime_مقصد_ns(global_rt_period()), 0);

	for_each_possible_cpu(i) {
		rt_rq = kzalloc_گرہ(حجم(ساخت rt_rq),
				     GFP_KERNEL, cpu_مقصد_گرہ(i));
		اگر (!rt_rq)
			جاؤ err;

		rt_se = kzalloc_گرہ(حجم(ساخت sched_rt_entity),
				     GFP_KERNEL, cpu_مقصد_گرہ(i));
		اگر (!rt_se)
			جاؤ err_free_rq;

		init_rt_rq(rt_rq);
		rt_rq->rt_runtime = tg->rt_bandwidth.rt_runtime;
		init_tg_rt_اندراج(tg, rt_rq, rt_se, i, والد->rt_se[i]);
	}

	واپس 1;

err_free_rq:
	kfree(rt_rq);
err:
	واپس 0;
}

#ورنہ /* !CONFIG_RT_GROUP_SCHED: */

#تعریف_کرو rt_entity_is_task(rt_se) (1)

ساکن اندرونی ساخت task_struct *rt_task_کا(ساخت sched_rt_entity *rt_se)
{
	واپس container_کا(rt_se, ساخت task_struct, rt);
}

ساکن اندرونی ساخت rq *rq_کا_rt_rq(ساخت rt_rq *rt_rq)
{
	واپس container_کا(rt_rq, ساخت rq, rt);
}

ساکن اندرونی ساخت rq *rq_کا_rt_se(ساخت sched_rt_entity *rt_se)
{
	ساخت task_struct *p = rt_task_کا(rt_se);

	واپس task_rq(p);
}

ساکن اندرونی ساخت rt_rq *rt_rq_کا_se(ساخت sched_rt_entity *rt_se)
{
	ساخت rq *rq = rq_کا_rt_se(rt_se);

	واپس &rq->rt;
}

خالی unregister_rt_sched_گروہ(ساخت task_گروہ *tg) { }

خالی free_rt_sched_گروہ(ساخت task_گروہ *tg) { }

صحیح alloc_rt_sched_گروہ(ساخت task_گروہ *tg, ساخت task_گروہ *والد)
{
	واپس 1;
}
#اگر_ختم /* !CONFIG_RT_GROUP_SCHED */

ساکن اندرونی bool need_pull_rt_task(ساخت rq *rq, ساخت task_struct *پچھلا)
{
	/* Try to pull RT tasks here if we lower this rq's prio */
	واپس rq->online && rq->rt.highest_prio.curr > پچھلا->prio;
}

ساکن اندرونی صحیح rt_overloaded(ساخت rq *rq)
{
	واپس atomic_read(&rq->rd->rto_گنتی);
}

ساکن اندرونی خالی rt_سیٹ_overload(ساخت rq *rq)
{
	اگر (!rq->online)
		واپس;

	cpumask_سیٹ_cpu(rq->cpu, rq->rd->rto_نقاب);
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
	atomic_inc(&rq->rd->rto_گنتی);
}

ساکن اندرونی خالی rt_clear_overload(ساخت rq *rq)
{
	اگر (!rq->online)
		واپس;

	/* the order here really doesn't matter */
	atomic_dec(&rq->rd->rto_گنتی);
	cpumask_clear_cpu(rq->cpu, rq->rd->rto_نقاب);
}

ساکن اندرونی صحیح has_pushable_tasks(ساخت rq *rq)
{
	واپس !plist_سر_خالی_خانے(&rq->rt.pushable_tasks);
}

ساکن DEFINE_فی_CPU(ساخت balance_callback, rt_ڈالو_سر);
ساکن DEFINE_فی_CPU(ساخت balance_callback, rt_pull_سر);

ساکن خالی ڈالو_rt_tasks(ساخت rq *);
ساکن خالی pull_rt_task(ساخت rq *);

ساکن اندرونی خالی rt_قطار_ڈالو_tasks(ساخت rq *rq)
{
	اگر (!has_pushable_tasks(rq))
		واپس;

	قطار_balance_callback(rq, &فی_cpu(rt_ڈالو_سر, rq->cpu), ڈالو_rt_tasks);
}

ساکن اندرونی خالی rt_قطار_pull_task(ساخت rq *rq)
{
	قطار_balance_callback(rq, &فی_cpu(rt_pull_سر, rq->cpu), pull_rt_task);
}

ساکن خالی قطار_میں_ڈالو_pushable_task(ساخت rq *rq, ساخت task_struct *p)
{
	plist_del(&p->pushable_tasks, &rq->rt.pushable_tasks);
	plist_گرہ_init(&p->pushable_tasks, p->prio);
	plist_جوڑو(&p->pushable_tasks, &rq->rt.pushable_tasks);

	/* Update the highest prio pushable task */
	اگر (p->prio < rq->rt.highest_prio.اگلا)
		rq->rt.highest_prio.اگلا = p->prio;

	اگر (!rq->rt.overloaded) {
		rt_سیٹ_overload(rq);
		rq->rt.overloaded = 1;
	}
}

ساکن خالی قطار_سے_نکالو_pushable_task(ساخت rq *rq, ساخت task_struct *p)
{
	plist_del(&p->pushable_tasks, &rq->rt.pushable_tasks);

	/* Update the new highest prio pushable task */
	اگر (has_pushable_tasks(rq)) {
		p = plist_پہلا_اندراج(&rq->rt.pushable_tasks,
				      ساخت task_struct, pushable_tasks);
		rq->rt.highest_prio.اگلا = p->prio;
	} ورنہ {
		rq->rt.highest_prio.اگلا = MAX_RT_PRIO-1;

		اگر (rq->rt.overloaded) {
			rt_clear_overload(rq);
			rq->rt.overloaded = 0;
		}
	}
}

ساکن خالی قطار_میں_ڈالو_چوٹی_rt_rq(ساخت rt_rq *rt_rq);
ساکن خالی قطار_سے_نکالو_چوٹی_rt_rq(ساخت rt_rq *rt_rq, غیرعلامتی صحیح گنتی);

ساکن اندرونی صحیح on_rt_rq(ساخت sched_rt_entity *rt_se)
{
	واپس rt_se->on_rq;
}

#اگر_تعریف_شدہ CONFIG_UCLAMP_TASK
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
ساکن اندرونی bool rt_task_سماتا_استعداد(ساخت task_struct *p, صحیح cpu)
{
	غیرعلامتی صحیح min_cap;
	غیرعلامتی صحیح max_cap;
	غیرعلامتی صحیح cpu_cap;

	/* Only heterogeneous systems can benefit from this check */
	اگر (!sched_asym_cpucap_فعال())
		واپس true;

	min_cap = uclamp_eff_قدر(p, UCLAMP_MIN);
	max_cap = uclamp_eff_قدر(p, UCLAMP_MAX);

	cpu_cap = arch_scale_cpu_استعداد(cpu);

	واپس cpu_cap >= min(min_cap, max_cap);
}
#ورنہ /* !CONFIG_UCLAMP_TASK: */
ساکن اندرونی bool rt_task_سماتا_استعداد(ساخت task_struct *p, صحیح cpu)
{
	واپس true;
}
#اگر_ختم /* !CONFIG_UCLAMP_TASK */

#اگر_تعریف_شدہ CONFIG_RT_گروہ_SCHED

ساکن اندرونی u64 sched_rt_runtime(ساخت rt_rq *rt_rq)
{
	واپس rt_rq->rt_runtime;
}

ساکن اندرونی u64 sched_rt_period(ساخت rt_rq *rt_rq)
{
	واپس ktime_مقصد_ns(rt_rq->tg->rt_bandwidth.rt_period);
}

قسمتعریف ساخت task_گروہ *rt_rq_iter_t;

ساکن اندرونی ساخت task_گروہ *اگلا_task_گروہ(ساخت task_گروہ *tg)
{
	اگر (!rt_گروہ_sched_enabled()) {
		WARN_ON(tg != &جڑ_task_گروہ);
		واپس تہی;
	}

	کرو {
		tg = list_اندراج_rcu(tg->list.اگلا,
			typeof(ساخت task_گروہ), list);
	} جب (&tg->list != &task_groups && task_گروہ_is_autogroup(tg));

	اگر (&tg->list == &task_groups)
		tg = تہی;

	واپس tg;
}

#تعریف_کرو for_each_rt_rq(rt_rq, iter, rq)					\
	برائے (iter = &جڑ_task_گروہ;					\
		iter && (rt_rq = iter->rt_rq[cpu_کا(rq)]);		\
		iter = اگلا_task_گروہ(iter))

#تعریف_کرو for_each_sched_rt_entity(rt_se) \
	برائے (; rt_se; rt_se = rt_se->والد)

ساکن اندرونی ساخت rt_rq *گروہ_rt_rq(ساخت sched_rt_entity *rt_se)
{
	واپس rt_se->my_q;
}

ساکن خالی قطار_میں_ڈالو_rt_entity(ساخت sched_rt_entity *rt_se, غیرعلامتی صحیح flags);
ساکن خالی قطار_سے_نکالو_rt_entity(ساخت sched_rt_entity *rt_se, غیرعلامتی صحیح flags);

ساکن خالی sched_rt_rq_قطار_میں_ڈالو(ساخت rt_rq *rt_rq)
{
	ساخت task_struct *donor = rq_کا_rt_rq(rt_rq)->donor;
	ساخت rq *rq = rq_کا_rt_rq(rt_rq);
	ساخت sched_rt_entity *rt_se;

	صحیح cpu = cpu_کا(rq);

	rt_se = rt_rq->tg->rt_se[cpu];

	اگر (rt_rq->rt_nr_running) {
		اگر (!rt_se)
			قطار_میں_ڈالو_چوٹی_rt_rq(rt_rq);
		ورنہ اگر (!on_rt_rq(rt_se))
			قطار_میں_ڈالو_rt_entity(rt_se, 0);

		اگر (rt_rq->highest_prio.curr < donor->prio)
			resched_curr(rq);
	}
}

ساکن خالی sched_rt_rq_قطار_سے_نکالو(ساخت rt_rq *rt_rq)
{
	ساخت sched_rt_entity *rt_se;
	صحیح cpu = cpu_کا(rq_کا_rt_rq(rt_rq));

	rt_se = rt_rq->tg->rt_se[cpu];

	اگر (!rt_se) {
		قطار_سے_نکالو_چوٹی_rt_rq(rt_rq, rt_rq->rt_nr_running);
		/* Kick cpufreq (see the comment in kernel/sched/sched.h). */
		cpufreq_update_util(rq_کا_rt_rq(rt_rq), 0);
	}
	ورنہ اگر (on_rt_rq(rt_se))
		قطار_سے_نکالو_rt_entity(rt_se, 0);
}

ساکن اندرونی صحیح rt_rq_throttled(ساخت rt_rq *rt_rq)
{
	واپس rt_rq->rt_throttled && !rt_rq->rt_nr_boosted;
}

ساکن صحیح rt_se_boosted(ساخت sched_rt_entity *rt_se)
{
	ساخت rt_rq *rt_rq = گروہ_rt_rq(rt_se);
	ساخت task_struct *p;

	اگر (rt_rq)
		واپس !!rt_rq->rt_nr_boosted;

	p = rt_task_کا(rt_se);
	واپس p->prio != p->معمول_prio;
}

ساکن اندرونی ثابت ساخت cpumask *sched_rt_period_نقاب(خالی)
{
	واپس this_rq()->rd->وسعت;
}

ساکن اندرونی
ساخت rt_rq *sched_rt_period_rt_rq(ساخت rt_bandwidth *rt_b, صحیح cpu)
{
	واپس container_کا(rt_b, ساخت task_گروہ, rt_bandwidth)->rt_rq[cpu];
}

ساکن اندرونی ساخت rt_bandwidth *sched_rt_bandwidth(ساخت rt_rq *rt_rq)
{
	واپس &rt_rq->tg->rt_bandwidth;
}

bool sched_rt_bandwidth_account(ساخت rt_rq *rt_rq)
{
	ساخت rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);

	واپس (hrtimer_فعال(&rt_b->rt_period_timer) ||
		rt_rq->rt_وقت < rt_b->rt_runtime);
}

/*
 * We ran out of runtime, see if we can borrow some from our neighbours.
 */
ساکن خالی do_balance_runtime(ساخت rt_rq *rt_rq)
{
	ساخت rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);
	ساخت جڑ_domain *rd = rq_کا_rt_rq(rt_rq)->rd;
	صحیح i, وزن;
	u64 rt_period;

	وزن = cpumask_وزن(rd->وسعت);

	raw_spin_تالا(&rt_b->rt_runtime_تالا);
	rt_period = ktime_مقصد_ns(rt_b->rt_period);
	for_each_cpu(i, rd->وسعت) {
		ساخت rt_rq *iter = sched_rt_period_rt_rq(rt_b, i);
		s64 diff;

		اگر (iter == rt_rq)
			جاری;

		raw_spin_تالا(&iter->rt_runtime_تالا);
		/*
		 * Either all rqs have inf runtime and there's nothing to steal
		 * or __disable_runtime() below sets a specific rq to inf to
		 * indicate its been disabled and disallow stealing.
		 */
		اگر (iter->rt_runtime == RUNTIME_INF)
			جاؤ اگلا;

		/*
		 * From runqueues with spare time, take 1/n part of their
		 * spare time, but no more than our period.
		 */
		diff = iter->rt_runtime - iter->rt_وقت;
		اگر (diff > 0) {
			diff = div_u64((u64)diff, وزن);
			اگر (rt_rq->rt_runtime + diff > rt_period)
				diff = rt_period - rt_rq->rt_runtime;
			iter->rt_runtime -= diff;
			rt_rq->rt_runtime += diff;
			اگر (rt_rq->rt_runtime == rt_period) {
				raw_spin_unlock(&iter->rt_runtime_تالا);
				توڑو;
			}
		}
اگلا:
		raw_spin_unlock(&iter->rt_runtime_تالا);
	}
	raw_spin_unlock(&rt_b->rt_runtime_تالا);
}

/*
 * Ensure this RQ takes back all the runtime it lend to its neighbours.
 */
ساکن خالی __disable_runtime(ساخت rq *rq)
{
	ساخت جڑ_domain *rd = rq->rd;
	rt_rq_iter_t iter;
	ساخت rt_rq *rt_rq;

	اگر (unlikely(!scheduler_running))
		واپس;

	for_each_rt_rq(rt_rq, iter, rq) {
		ساخت rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);
		s64 want;
		صحیح i;

		raw_spin_تالا(&rt_b->rt_runtime_تالا);
		raw_spin_تالا(&rt_rq->rt_runtime_تالا);
		/*
		 * Either we're all inf and nobody needs to borrow, or we're
		 * already disabled and thus have nothing to do, or we have
		 * exactly the right amount of runtime to take out.
		 */
		اگر (rt_rq->rt_runtime == RUNTIME_INF ||
				rt_rq->rt_runtime == rt_b->rt_runtime)
			جاؤ متوازن;
		raw_spin_unlock(&rt_rq->rt_runtime_تالا);

		/*
		 * Calculate the difference between what we started out with
		 * and what we current have, that's the amount of runtime
		 * we lend and now have to reclaim.
		 */
		want = rt_b->rt_runtime - rt_rq->rt_runtime;

		/*
		 * Greedy reclaim, take back as much as we can.
		 */
		for_each_cpu(i, rd->وسعت) {
			ساخت rt_rq *iter = sched_rt_period_rt_rq(rt_b, i);
			s64 diff;

			/*
			 * Can't reclaim from ourselves or disabled runqueues.
			 */
			اگر (iter == rt_rq || iter->rt_runtime == RUNTIME_INF)
				جاری;

			raw_spin_تالا(&iter->rt_runtime_تالا);
			اگر (want > 0) {
				diff = min_t(s64, iter->rt_runtime, want);
				iter->rt_runtime -= diff;
				want -= diff;
			} ورنہ {
				iter->rt_runtime -= want;
				want -= want;
			}
			raw_spin_unlock(&iter->rt_runtime_تالا);

			اگر (!want)
				توڑو;
		}

		raw_spin_تالا(&rt_rq->rt_runtime_تالا);
		/*
		 * We cannot be left wanting - that would mean some runtime
		 * leaked out of the system.
		 */
		WARN_ON_ONCE(want);
متوازن:
		/*
		 * Disable all the borrow logic by pretending we have inf
		 * runtime - in which case borrowing doesn't make sense.
		 */
		rt_rq->rt_runtime = RUNTIME_INF;
		rt_rq->rt_throttled = 0;
		raw_spin_unlock(&rt_rq->rt_runtime_تالا);
		raw_spin_unlock(&rt_b->rt_runtime_تالا);

		/* Make rt_rq available for pick_next_task() */
		sched_rt_rq_قطار_میں_ڈالو(rt_rq);
	}
}

ساکن خالی __enable_runtime(ساخت rq *rq)
{
	rt_rq_iter_t iter;
	ساخت rt_rq *rt_rq;

	اگر (unlikely(!scheduler_running))
		واپس;

	/*
	 * Reset each runqueue's bandwidth settings
	 */
	for_each_rt_rq(rt_rq, iter, rq) {
		ساخت rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);

		raw_spin_تالا(&rt_b->rt_runtime_تالا);
		raw_spin_تالا(&rt_rq->rt_runtime_تالا);
		rt_rq->rt_runtime = rt_b->rt_runtime;
		rt_rq->rt_وقت = 0;
		rt_rq->rt_throttled = 0;
		raw_spin_unlock(&rt_rq->rt_runtime_تالا);
		raw_spin_unlock(&rt_b->rt_runtime_تالا);
	}
}

ساکن خالی balance_runtime(ساخت rt_rq *rt_rq)
{
	اگر (!sched_feat(RT_RUNTIME_SHARE))
		واپس;

	اگر (rt_rq->rt_وقت > rt_rq->rt_runtime) {
		raw_spin_unlock(&rt_rq->rt_runtime_تالا);
		do_balance_runtime(rt_rq);
		raw_spin_تالا(&rt_rq->rt_runtime_تالا);
	}
}

ساکن صحیح do_sched_rt_period_timer(ساخت rt_bandwidth *rt_b, صحیح overrun)
{
	صحیح i, idle = 1, throttled = 0;
	ثابت ساخت cpumask *وسعت;

	وسعت = sched_rt_period_نقاب();

	/*
	 * FIXME: isolated CPUs should really leave the root task group,
	 * whether they are isolcpus or were isolated via cpusets, lest
	 * the timer run on a CPU which does not service all runqueues,
	 * potentially leaving other CPUs indefinitely throttled.  If
	 * isolation is really required, the user will turn the throttle
	 * off to kill the perturbations it causes anyway.  Meanwhile,
	 * this maintains functionality for boot and/or troubleshooting.
	 */
	اگر (rt_b == &جڑ_task_گروہ.rt_bandwidth)
		وسعت = cpu_online_نقاب;

	for_each_cpu(i, وسعت) {
		صحیح قطار_میں_ڈالو = 0;
		ساخت rt_rq *rt_rq = sched_rt_period_rt_rq(rt_b, i);
		ساخت rq *rq = rq_کا_rt_rq(rt_rq);
		ساخت rq_flags rf;
		صحیح skip;

		/*
		 * When span == cpu_online_mask, taking each rq->lock
		 * can be time-consuming. Try to avoid it when possible.
		 */
		raw_spin_تالا(&rt_rq->rt_runtime_تالا);
		اگر (!sched_feat(RT_RUNTIME_SHARE) && rt_rq->rt_runtime != RUNTIME_INF)
			rt_rq->rt_runtime = rt_b->rt_runtime;
		skip = !rt_rq->rt_وقت && !rt_rq->rt_nr_running;
		raw_spin_unlock(&rt_rq->rt_runtime_تالا);
		اگر (skip)
			جاری;

		rq_تالا(rq, &rf);
		update_rq_clock(rq);

		اگر (rt_rq->rt_وقت) {
			u64 runtime;

			raw_spin_تالا(&rt_rq->rt_runtime_تالا);
			اگر (rt_rq->rt_throttled)
				balance_runtime(rt_rq);
			runtime = rt_rq->rt_runtime;
			rt_rq->rt_وقت -= min(rt_rq->rt_وقت, overrun*runtime);
			اگر (rt_rq->rt_throttled && rt_rq->rt_وقت < runtime) {
				rt_rq->rt_throttled = 0;
				قطار_میں_ڈالو = 1;

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
			اگر (rt_rq->rt_وقت || rt_rq->rt_nr_running)
				idle = 0;
			raw_spin_unlock(&rt_rq->rt_runtime_تالا);
		} ورنہ اگر (rt_rq->rt_nr_running) {
			idle = 0;
			اگر (!rt_rq_throttled(rt_rq))
				قطار_میں_ڈالو = 1;
		}
		اگر (rt_rq->rt_throttled)
			throttled = 1;

		اگر (قطار_میں_ڈالو)
			sched_rt_rq_قطار_میں_ڈالو(rt_rq);
		rq_unlock(rq, &rf);
	}

	اگر (!throttled && (!rt_bandwidth_enabled() || rt_b->rt_runtime == RUNTIME_INF))
		واپس 1;

	واپس idle;
}

ساکن صحیح sched_rt_runtime_exceeded(ساخت rt_rq *rt_rq)
{
	u64 runtime = sched_rt_runtime(rt_rq);

	اگر (rt_rq->rt_throttled)
		واپس rt_rq_throttled(rt_rq);

	اگر (runtime >= sched_rt_period(rt_rq))
		واپس 0;

	balance_runtime(rt_rq);
	runtime = sched_rt_runtime(rt_rq);
	اگر (runtime == RUNTIME_INF)
		واپس 0;

	اگر (rt_rq->rt_وقت > runtime) {
		ساخت rt_bandwidth *rt_b = sched_rt_bandwidth(rt_rq);

		/*
		 * Don't actually throttle groups that have no runtime assigned
		 * but accrue some time due to boosting.
		 */
		اگر (likely(rt_b->rt_runtime)) {
			rt_rq->rt_throttled = 1;
			printk_deferred_once("sched: RT throttling activated\n");
		} ورنہ {
			/*
			 * In case we did anyway, make it go away,
			 * replenishment is a joke, since it will replenish us
			 * with exactly 0 ns.
			 */
			rt_rq->rt_وقت = 0;
		}

		اگر (rt_rq_throttled(rt_rq)) {
			sched_rt_rq_قطار_سے_نکالو(rt_rq);
			واپس 1;
		}
	}

	واپس 0;
}

#ورنہ /* !CONFIG_RT_GROUP_SCHED: */

قسمتعریف ساخت rt_rq *rt_rq_iter_t;

#تعریف_کرو for_each_rt_rq(rt_rq, iter, rq) \
	برائے ((خالی) iter, rt_rq = &rq->rt; rt_rq; rt_rq = تہی)

#تعریف_کرو for_each_sched_rt_entity(rt_se) \
	برائے (; rt_se; rt_se = تہی)

ساکن اندرونی ساخت rt_rq *گروہ_rt_rq(ساخت sched_rt_entity *rt_se)
{
	واپس تہی;
}

ساکن اندرونی خالی sched_rt_rq_قطار_میں_ڈالو(ساخت rt_rq *rt_rq)
{
	ساخت rq *rq = rq_کا_rt_rq(rt_rq);

	اگر (!rt_rq->rt_nr_running)
		واپس;

	قطار_میں_ڈالو_چوٹی_rt_rq(rt_rq);
	resched_curr(rq);
}

ساکن اندرونی خالی sched_rt_rq_قطار_سے_نکالو(ساخت rt_rq *rt_rq)
{
	قطار_سے_نکالو_چوٹی_rt_rq(rt_rq, rt_rq->rt_nr_running);
}

ساکن اندرونی صحیح rt_rq_throttled(ساخت rt_rq *rt_rq)
{
	واپس false;
}

ساکن اندرونی ثابت ساخت cpumask *sched_rt_period_نقاب(خالی)
{
	واپس cpu_online_نقاب;
}

ساکن اندرونی
ساخت rt_rq *sched_rt_period_rt_rq(ساخت rt_bandwidth *rt_b, صحیح cpu)
{
	واپس &cpu_rq(cpu)->rt;
}

ساکن خالی __enable_runtime(ساخت rq *rq) { }
ساکن خالی __disable_runtime(ساخت rq *rq) { }

#اگر_ختم /* !CONFIG_RT_GROUP_SCHED */

ساکن اندرونی صحیح rt_se_prio(ساخت sched_rt_entity *rt_se)
{
#اگر_تعریف_شدہ CONFIG_RT_گروہ_SCHED
	ساخت rt_rq *rt_rq = گروہ_rt_rq(rt_se);

	اگر (rt_rq)
		واپس rt_rq->highest_prio.curr;
#اگر_ختم

	واپس rt_task_کا(rt_se)->prio;
}

/*
 * Update the current task's runtime statistics. Skip current tasks that
 * are not in our scheduling class.
 */
ساکن خالی update_curr_rt(ساخت rq *rq)
{
	ساخت task_struct *donor = rq->donor;
	s64 delta_exec;

	اگر (donor->sched_class != &rt_sched_class)
		واپس;

	delta_exec = update_curr_common(rq);
	اگر (unlikely(delta_exec <= 0))
		واپس;

#اگر_تعریف_شدہ CONFIG_RT_گروہ_SCHED
	ساخت sched_rt_entity *rt_se = &donor->rt;

	اگر (!rt_bandwidth_enabled())
		واپس;

	for_each_sched_rt_entity(rt_se) {
		ساخت rt_rq *rt_rq = rt_rq_کا_se(rt_se);
		صحیح exceeded;

		اگر (sched_rt_runtime(rt_rq) != RUNTIME_INF) {
			raw_spin_تالا(&rt_rq->rt_runtime_تالا);
			rt_rq->rt_وقت += delta_exec;
			exceeded = sched_rt_runtime_exceeded(rt_rq);
			اگر (exceeded)
				resched_curr(rq);
			raw_spin_unlock(&rt_rq->rt_runtime_تالا);
			اگر (exceeded)
				do_آغاز_rt_bandwidth(sched_rt_bandwidth(rt_rq));
		}
	}
#اگر_ختم /* CONFIG_RT_GROUP_SCHED */
}

ساکن خالی
قطار_سے_نکالو_چوٹی_rt_rq(ساخت rt_rq *rt_rq, غیرعلامتی صحیح گنتی)
{
	ساخت rq *rq = rq_کا_rt_rq(rt_rq);

	BUG_ON(&rq->rt != rt_rq);

	اگر (!rt_rq->rt_queued)
		واپس;

	BUG_ON(!rq->nr_running);

	sub_nr_running(rq, گنتی);
	rt_rq->rt_queued = 0;

}

ساکن خالی
قطار_میں_ڈالو_چوٹی_rt_rq(ساخت rt_rq *rt_rq)
{
	ساخت rq *rq = rq_کا_rt_rq(rt_rq);

	BUG_ON(&rq->rt != rt_rq);

	اگر (rt_rq->rt_queued)
		واپس;

	اگر (rt_rq_throttled(rt_rq))
		واپس;

	اگر (rt_rq->rt_nr_running) {
		جوڑو_nr_running(rq, rt_rq->rt_nr_running);
		rt_rq->rt_queued = 1;
	}

	/* Kick cpufreq (see the comment in kernel/sched/sched.h). */
	cpufreq_update_util(rq, 0);
}

ساکن خالی
inc_rt_prio_smp(ساخت rt_rq *rt_rq, صحیح prio, صحیح پچھلا_prio)
{
	ساخت rq *rq = rq_کا_rt_rq(rt_rq);

	/*
	 * Change rq's cpupri only if rt_rq is the top queue.
	 */
	اگر (IS_ENABLED(CONFIG_RT_گروہ_SCHED) && &rq->rt != rt_rq)
		واپس;

	اگر (rq->online && prio < پچھلا_prio)
		cpupri_سیٹ(&rq->rd->cpupri, rq->cpu, prio);
}

ساکن خالی
dec_rt_prio_smp(ساخت rt_rq *rt_rq, صحیح prio, صحیح پچھلا_prio)
{
	ساخت rq *rq = rq_کا_rt_rq(rt_rq);

	/*
	 * Change rq's cpupri only if rt_rq is the top queue.
	 */
	اگر (IS_ENABLED(CONFIG_RT_گروہ_SCHED) && &rq->rt != rt_rq)
		واپس;

	اگر (rq->online && rt_rq->highest_prio.curr != پچھلا_prio)
		cpupri_سیٹ(&rq->rd->cpupri, rq->cpu, rt_rq->highest_prio.curr);
}

ساکن خالی
inc_rt_prio(ساخت rt_rq *rt_rq, صحیح prio)
{
	صحیح پچھلا_prio = rt_rq->highest_prio.curr;

	اگر (prio < پچھلا_prio)
		rt_rq->highest_prio.curr = prio;

	inc_rt_prio_smp(rt_rq, prio, پچھلا_prio);
}

ساکن خالی
dec_rt_prio(ساخت rt_rq *rt_rq, صحیح prio)
{
	صحیح پچھلا_prio = rt_rq->highest_prio.curr;

	اگر (rt_rq->rt_nr_running) {

		WARN_ON(prio < پچھلا_prio);

		/*
		 * This may have been our highest task, and therefore
		 * we may have some re-computation to do
		 */
		اگر (prio == پچھلا_prio) {
			ساخت rt_prio_array *array = &rt_rq->فعال;

			rt_rq->highest_prio.curr =
				sched_ڈھونڈو_پہلا_ثنائی_ہندسہ(array->bitmap);
		}

	} ورنہ {
		rt_rq->highest_prio.curr = MAX_RT_PRIO-1;
	}

	dec_rt_prio_smp(rt_rq, prio, پچھلا_prio);
}

#اگر_تعریف_شدہ CONFIG_RT_گروہ_SCHED

ساکن خالی
inc_rt_گروہ(ساخت sched_rt_entity *rt_se, ساخت rt_rq *rt_rq)
{
	اگر (rt_se_boosted(rt_se))
		rt_rq->rt_nr_boosted++;

	آغاز_rt_bandwidth(&rt_rq->tg->rt_bandwidth);
}

ساکن خالی
dec_rt_گروہ(ساخت sched_rt_entity *rt_se, ساخت rt_rq *rt_rq)
{
	اگر (rt_se_boosted(rt_se))
		rt_rq->rt_nr_boosted--;

	WARN_ON(!rt_rq->rt_nr_running && rt_rq->rt_nr_boosted);
}

#ورنہ /* !CONFIG_RT_GROUP_SCHED: */

ساکن خالی
inc_rt_گروہ(ساخت sched_rt_entity *rt_se, ساخت rt_rq *rt_rq)
{
}

ساکن اندرونی
خالی dec_rt_گروہ(ساخت sched_rt_entity *rt_se, ساخت rt_rq *rt_rq) {}

#اگر_ختم /* !CONFIG_RT_GROUP_SCHED */

ساکن اندرونی
غیرعلامتی صحیح rt_se_nr_running(ساخت sched_rt_entity *rt_se)
{
	ساخت rt_rq *گروہ_rq = گروہ_rt_rq(rt_se);

	اگر (گروہ_rq)
		واپس گروہ_rq->rt_nr_running;
	ورنہ
		واپس 1;
}

ساکن اندرونی
غیرعلامتی صحیح rt_se_rr_nr_running(ساخت sched_rt_entity *rt_se)
{
	ساخت rt_rq *گروہ_rq = گروہ_rt_rq(rt_se);
	ساخت task_struct *tsk;

	اگر (گروہ_rq)
		واپس گروہ_rq->rr_nr_running;

	tsk = rt_task_کا(rt_se);

	واپس (tsk->policy == SCHED_RR) ? 1 : 0;
}

ساکن اندرونی
خالی inc_rt_tasks(ساخت sched_rt_entity *rt_se, ساخت rt_rq *rt_rq)
{
	صحیح prio = rt_se_prio(rt_se);

	WARN_ON(!rt_prio(prio));
	rt_rq->rt_nr_running += rt_se_nr_running(rt_se);
	rt_rq->rr_nr_running += rt_se_rr_nr_running(rt_se);

	inc_rt_prio(rt_rq, prio);
	inc_rt_گروہ(rt_se, rt_rq);
}

ساکن اندرونی
خالی dec_rt_tasks(ساخت sched_rt_entity *rt_se, ساخت rt_rq *rt_rq)
{
	WARN_ON(!rt_prio(rt_se_prio(rt_se)));
	WARN_ON(!rt_rq->rt_nr_running);
	rt_rq->rt_nr_running -= rt_se_nr_running(rt_se);
	rt_rq->rr_nr_running -= rt_se_rr_nr_running(rt_se);

	dec_rt_prio(rt_rq, rt_se_prio(rt_se));
	dec_rt_گروہ(rt_se, rt_rq);
}

/*
 * Change rt_se->run_list location unless SAVE && !MOVE
 *
 * assumes ENQUEUE/DEQUEUE flags match
 */
ساکن اندرونی bool move_entity(غیرعلامتی صحیح flags)
{
	اگر ((flags & (قطار_سے_نکالو_SAVE | قطار_سے_نکالو_MOVE)) == قطار_سے_نکالو_SAVE)
		واپس false;

	واپس true;
}

ساکن خالی __delist_rt_entity(ساخت sched_rt_entity *rt_se, ساخت rt_prio_array *array)
{
	list_del_init(&rt_se->تسلسل_list);

	اگر (list_خالی_خانے(array->قطار + rt_se_prio(rt_se)))
		__clear_bit(rt_se_prio(rt_se), array->bitmap);

	rt_se->on_list = 0;
}

ساکن اندرونی ساخت sched_statistics *
__schedstats_from_rt_se(ساخت sched_rt_entity *rt_se)
{
	/* schedstats is not supported for rt group. */
	اگر (!rt_entity_is_task(rt_se))
		واپس تہی;

	واپس &rt_task_کا(rt_se)->stats;
}

ساکن اندرونی خالی
update_stats_wait_آغاز_rt(ساخت rt_rq *rt_rq, ساخت sched_rt_entity *rt_se)
{
	ساخت sched_statistics *stats;
	ساخت task_struct *p = تہی;

	اگر (!schedstat_enabled())
		واپس;

	اگر (rt_entity_is_task(rt_se))
		p = rt_task_کا(rt_se);

	stats = __schedstats_from_rt_se(rt_se);
	اگر (!stats)
		واپس;

	__update_stats_wait_start(rq_کا_rt_rq(rt_rq), p, stats);
}

ساکن اندرونی خالی
update_stats_قطار_میں_ڈالو_sleeper_rt(ساخت rt_rq *rt_rq, ساخت sched_rt_entity *rt_se)
{
	ساخت sched_statistics *stats;
	ساخت task_struct *p = تہی;

	اگر (!schedstat_enabled())
		واپس;

	اگر (rt_entity_is_task(rt_se))
		p = rt_task_کا(rt_se);

	stats = __schedstats_from_rt_se(rt_se);
	اگر (!stats)
		واپس;

	__update_stats_enqueue_sleeper(rq_کا_rt_rq(rt_rq), p, stats);
}

ساکن اندرونی خالی
update_stats_قطار_میں_ڈالو_rt(ساخت rt_rq *rt_rq, ساخت sched_rt_entity *rt_se,
			صحیح flags)
{
	اگر (!schedstat_enabled())
		واپس;

	اگر (flags & قطار_میں_ڈالو_WAKEUP)
		update_stats_قطار_میں_ڈالو_sleeper_rt(rt_rq, rt_se);
}

ساکن اندرونی خالی
update_stats_wait_انتہا_rt(ساخت rt_rq *rt_rq, ساخت sched_rt_entity *rt_se)
{
	ساخت sched_statistics *stats;
	ساخت task_struct *p = تہی;

	اگر (!schedstat_enabled())
		واپس;

	اگر (rt_entity_is_task(rt_se))
		p = rt_task_کا(rt_se);

	stats = __schedstats_from_rt_se(rt_se);
	اگر (!stats)
		واپس;

	__update_stats_wait_end(rq_کا_rt_rq(rt_rq), p, stats);
}

ساکن اندرونی خالی
update_stats_قطار_سے_نکالو_rt(ساخت rt_rq *rt_rq, ساخت sched_rt_entity *rt_se,
			صحیح flags)
{
	ساخت task_struct *p = تہی;
	ساخت rq *rq = rq_کا_rt_rq(rt_rq);

	اگر (!schedstat_enabled())
		واپس;

	اگر (rt_entity_is_task(rt_se)) {
		p = rt_task_کا(rt_se);

		اگر (p != rq->curr)
			update_stats_wait_انتہا_rt(rt_rq, rt_se);
	}

	اگر ((flags & قطار_سے_نکالو_SLEEP) && p) {
		غیرعلامتی صحیح state;

		state = READ_ONCE(p->__state);
		اگر (state & TASK_INTERRUPTIBLE)
			__schedstat_set(p->stats.sleep_آغاز,
					rq_clock(rq_کا_rt_rq(rt_rq)));

		اگر (state & TASK_UNINTERRUPTIBLE)
			__schedstat_set(p->stats.بلاک_آغاز,
					rq_clock(rq_کا_rt_rq(rt_rq)));
	}
}

ساکن خالی __enqueue_rt_entity(ساخت sched_rt_entity *rt_se, غیرعلامتی صحیح flags)
{
	ساخت rt_rq *rt_rq = rt_rq_کا_se(rt_se);
	ساخت rt_prio_array *array = &rt_rq->فعال;
	ساخت rt_rq *گروہ_rq = گروہ_rt_rq(rt_se);
	ساخت list_سر *قطار = array->قطار + rt_se_prio(rt_se);

	/*
	 * Don't enqueue the group if its throttled, or when empty.
	 * The latter is a consequence of the former when a child group
	 * get throttled and the current group doesn't have any other
	 * active members.
	 */
	اگر (گروہ_rq && (rt_rq_throttled(گروہ_rq) || !گروہ_rq->rt_nr_running)) {
		اگر (rt_se->on_list)
			__delist_rt_entity(rt_se, array);
		واپس;
	}

	اگر (move_entity(flags)) {
		WARN_ON_ONCE(rt_se->on_list);
		اگر (flags & قطار_میں_ڈالو_سر)
			list_جوڑو(&rt_se->تسلسل_list, قطار);
		ورنہ
			list_جوڑو_دم(&rt_se->تسلسل_list, قطار);

		__set_bit(rt_se_prio(rt_se), array->bitmap);
		rt_se->on_list = 1;
	}
	rt_se->on_rq = 1;

	inc_rt_tasks(rt_se, rt_rq);
}

ساکن خالی __dequeue_rt_entity(ساخت sched_rt_entity *rt_se, غیرعلامتی صحیح flags)
{
	ساخت rt_rq *rt_rq = rt_rq_کا_se(rt_se);
	ساخت rt_prio_array *array = &rt_rq->فعال;

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
ساکن خالی قطار_سے_نکالو_rt_ڈھیر(ساخت sched_rt_entity *rt_se, غیرعلامتی صحیح flags)
{
	ساخت sched_rt_entity *عقب = تہی;
	غیرعلامتی صحیح rt_nr_running;

	for_each_sched_rt_entity(rt_se) {
		rt_se->عقب = عقب;
		عقب = rt_se;
	}

	rt_nr_running = rt_rq_کا_se(عقب)->rt_nr_running;

	برائے (rt_se = عقب; rt_se; rt_se = rt_se->عقب) {
		اگر (on_rt_rq(rt_se))
			__dequeue_rt_entity(rt_se, flags);
	}

	قطار_سے_نکالو_چوٹی_rt_rq(rt_rq_کا_se(عقب), rt_nr_running);
}

ساکن خالی قطار_میں_ڈالو_rt_entity(ساخت sched_rt_entity *rt_se, غیرعلامتی صحیح flags)
{
	ساخت rq *rq = rq_کا_rt_se(rt_se);

	update_stats_قطار_میں_ڈالو_rt(rt_rq_کا_se(rt_se), rt_se, flags);

	قطار_سے_نکالو_rt_ڈھیر(rt_se, flags);
	for_each_sched_rt_entity(rt_se)
		__enqueue_rt_entity(rt_se, flags);
	قطار_میں_ڈالو_چوٹی_rt_rq(&rq->rt);
}

ساکن خالی قطار_سے_نکالو_rt_entity(ساخت sched_rt_entity *rt_se, غیرعلامتی صحیح flags)
{
	ساخت rq *rq = rq_کا_rt_se(rt_se);

	update_stats_قطار_سے_نکالو_rt(rt_rq_کا_se(rt_se), rt_se, flags);

	قطار_سے_نکالو_rt_ڈھیر(rt_se, flags);

	for_each_sched_rt_entity(rt_se) {
		ساخت rt_rq *rt_rq = گروہ_rt_rq(rt_se);

		اگر (rt_rq && rt_rq->rt_nr_running)
			__enqueue_rt_entity(rt_se, flags);
	}
	قطار_میں_ڈالو_چوٹی_rt_rq(&rq->rt);
}

/*
 * Adding/removing a task to/from a priority array:
 */
ساکن خالی
قطار_میں_ڈالو_task_rt(ساخت rq *rq, ساخت task_struct *p, صحیح flags)
{
	ساخت sched_rt_entity *rt_se = &p->rt;

	اگر (flags & قطار_میں_ڈالو_WAKEUP)
		rt_se->timeout = 0;

	check_schedstat_required();
	update_stats_wait_آغاز_rt(rt_rq_کا_se(rt_se), rt_se);

	قطار_میں_ڈالو_rt_entity(rt_se, flags);

	اگر (task_is_blocked(p))
		واپس;

	اگر (!task_current(rq, p) && p->nr_cpus_allowed > 1)
		قطار_میں_ڈالو_pushable_task(rq, p);
}

ساکن bool قطار_سے_نکالو_task_rt(ساخت rq *rq, ساخت task_struct *p, صحیح flags)
{
	ساخت sched_rt_entity *rt_se = &p->rt;

	update_curr_rt(rq);
	قطار_سے_نکالو_rt_entity(rt_se, flags);

	قطار_سے_نکالو_pushable_task(rq, p);

	واپس true;
}

/*
 * Put task to the head or the end of the run list without the overhead of
 * dequeue followed by enqueue.
 */
ساکن خالی
requeue_rt_entity(ساخت rt_rq *rt_rq, ساخت sched_rt_entity *rt_se, صحیح سر)
{
	اگر (on_rt_rq(rt_se)) {
		ساخت rt_prio_array *array = &rt_rq->فعال;
		ساخت list_سر *قطار = array->قطار + rt_se_prio(rt_se);

		اگر (سر)
			list_move(&rt_se->تسلسل_list, قطار);
		ورنہ
			list_move_دم(&rt_se->تسلسل_list, قطار);
	}
}

ساکن خالی requeue_task_rt(ساخت rq *rq, ساخت task_struct *p, صحیح سر)
{
	ساخت sched_rt_entity *rt_se = &p->rt;
	ساخت rt_rq *rt_rq;

	for_each_sched_rt_entity(rt_se) {
		rt_rq = rt_rq_کا_se(rt_se);
		requeue_rt_entity(rt_rq, rt_se, سر);
	}
}

ساکن خالی yield_task_rt(ساخت rq *rq)
{
	requeue_task_rt(rq, rq->donor, 0);
}

ساکن صحیح ڈھونڈو_سب_سے_نیچا_rq(ساخت task_struct *task);

ساکن صحیح
select_task_rq_rt(ساخت task_struct *p, صحیح cpu, صحیح flags)
{
	ساخت task_struct *curr, *donor;
	ساخت rq *rq;
	bool test;

	/* For anything but wake ups, just return the task_cpu */
	اگر (!(flags & (WF_TTWU | WF_FORK)))
		جاؤ برآمد;

	rq = cpu_rq(cpu);

	rcu_read_تالا();
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

	اگر (test || !rt_task_سماتا_استعداد(p, cpu)) {
		صحیح ہدف = ڈھونڈو_سب_سے_نیچا_rq(p);

		/*
		 * Bail out if we were forcing a migration to find a better
		 * fitting CPU but our search failed.
		 */
		اگر (!test && ہدف != -1 && !rt_task_سماتا_استعداد(p, ہدف))
			جاؤ برآمد_unlock;

		/*
		 * Don't bother moving it if the destination CPU is
		 * not running a lower priority task.
		 */
		اگر (ہدف != -1 &&
		    p->prio < cpu_rq(ہدف)->rt.highest_prio.curr)
			cpu = ہدف;
	}

برآمد_unlock:
	rcu_read_unlock();

برآمد:
	واپس cpu;
}

ساکن خالی check_preempt_equal_prio(ساخت rq *rq, ساخت task_struct *p)
{
	اگر (rq->curr->nr_cpus_allowed == 1 ||
	    !cpupri_ڈھونڈو(&rq->rd->cpupri, rq->donor, تہی))
		واپس;

	/*
	 * p is migratable, so let's not schedule it and
	 * see if it is pushed or pulled somewhere else.
	 */
	اگر (p->nr_cpus_allowed != 1 &&
	    cpupri_ڈھونڈو(&rq->rd->cpupri, p, تہی))
		واپس;

	/*
	 * There appear to be other CPUs that can accept
	 * the current task but none can run 'p', so lets reschedule
	 * to try and push the current task away:
	 */
	requeue_task_rt(rq, p, 1);
	resched_curr(rq);
}

ساکن صحیح balance_rt(ساخت rq *rq, ساخت rq_flags *rf)
{
	/*
	 * Note, rq->donor may change during rq lock drops,
	 * so don't re-use p across lock drops
	 */
	ساخت task_struct *p = rq->donor;

	اگر (!on_rt_rq(&p->rt) && need_pull_rt_task(rq, p)) {
		/*
		 * This is OK, because current is on_cpu, which avoids it being
		 * picked for load-balance and preemption/IRQs are still
		 * disabled avoiding further scheduler activity on it and we've
		 * not yet started the picking loop.
		 */
		rq_unpin_تالا(rq, rf);
		pull_rt_task(rq);
		rq_repin_تالا(rq, rf);
	}

	واپس sched_stop_runnable(rq) || sched_dl_runnable(rq) || sched_rt_runnable(rq);
}

/*
 * Preempt the current task with a newly woken task if needed:
 */
ساکن خالی wakeup_preempt_rt(ساخت rq *rq, ساخت task_struct *p, صحیح flags)
{
	ساخت task_struct *donor = rq->donor;

	/*
	 * XXX If we're preempted by DL, queue a push?
	 */
	اگر (p->sched_class != &rt_sched_class ||
	    donor->sched_class != &rt_sched_class)
		واپس;

	اگر (p->prio < donor->prio) {
		resched_curr(rq);
		واپس;
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

ساکن اندرونی خالی سیٹ_اگلا_task_rt(ساخت rq *rq, ساخت task_struct *p, شمار snt_e type)
{
	ساخت sched_rt_entity *rt_se = &p->rt;
	ساخت rt_rq *rt_rq = &rq->rt;

	اگر (type == SNT_REPICK)
		واپس;

	p->se.exec_آغاز = rq_clock_task(rq);
	اگر (on_rt_rq(&p->rt))
		update_stats_wait_انتہا_rt(rt_rq, rt_se);

	/* The running task is never eligible for pushing */
	قطار_سے_نکالو_pushable_task(rq, p);

	اگر (type != SNT_PICK)
		واپس;

	/*
	 * If prev task was rt, put_prev_task() has already updated the
	 * utilization. We only care of the case where we start to schedule a
	 * rt task
	 */
	اگر (rq->donor->sched_class != &rt_sched_class)
		update_rt_rq_بھرو_avg(rq_clock_pelt(rq), rq, 0);

	rt_قطار_ڈالو_tasks(rq);
}

ساکن ساخت sched_rt_entity *pick_اگلا_rt_entity(ساخت rt_rq *rt_rq)
{
	ساخت rt_prio_array *array = &rt_rq->فعال;
	ساخت sched_rt_entity *اگلا = تہی;
	ساخت list_سر *قطار;
	صحیح idx;

	idx = sched_ڈھونڈو_پہلا_ثنائی_ہندسہ(array->bitmap);
	BUG_ON(idx >= MAX_RT_PRIO);

	قطار = array->قطار + idx;
	اگر (WARN_ON_ONCE(list_خالی_خانے(قطار)))
		واپس تہی;
	اگلا = list_اندراج(قطار->اگلا, ساخت sched_rt_entity, تسلسل_list);

	واپس اگلا;
}

ساکن ساخت task_struct *pick_اگلا_task_rt(ساخت rq *rq)
{
	ساخت sched_rt_entity *rt_se;
	ساخت rt_rq *rt_rq  = &rq->rt;

	کرو {
		rt_se = pick_اگلا_rt_entity(rt_rq);
		اگر (unlikely(!rt_se))
			واپس تہی;
		rt_rq = گروہ_rt_rq(rt_se);
	} جب (rt_rq);

	واپس rt_task_کا(rt_se);
}

ساکن ساخت task_struct *pick_task_rt(ساخت rq *rq, ساخت rq_flags *rf)
{
	ساخت task_struct *p;

	اگر (!sched_rt_runnable(rq))
		واپس تہی;

	p = pick_اگلا_task_rt(rq);

	واپس p;
}

ساکن خالی put_پچھلا_task_rt(ساخت rq *rq, ساخت task_struct *p, ساخت task_struct *اگلا)
{
	ساخت sched_rt_entity *rt_se = &p->rt;
	ساخت rt_rq *rt_rq = &rq->rt;

	اگر (on_rt_rq(&p->rt))
		update_stats_wait_آغاز_rt(rt_rq, rt_se);

	update_curr_rt(rq);

	update_rt_rq_بھرو_avg(rq_clock_pelt(rq), rq, 1);

	اگر (task_is_blocked(p))
		واپس;
	/*
	 * The previous task needs to be made eligible for pushing
	 * if it is still active
	 */
	اگر (on_rt_rq(&p->rt) && p->nr_cpus_allowed > 1)
		قطار_میں_ڈالو_pushable_task(rq, p);
}

/* Only try algorithms three times */
#تعریف_کرو RT_MAX_TRIES 3

/*
 * Return the highest pushable rq's task, which is suitable to be executed
 * on the CPU, NULL otherwise
 */
ساکن ساخت task_struct *pick_highest_pushable_task(ساخت rq *rq, صحیح cpu)
{
	ساخت plist_سر *سر = &rq->rt.pushable_tasks;
	ساخت task_struct *p;

	اگر (!has_pushable_tasks(rq))
		واپس تہی;

	plist_for_each_اندراج(p, سر, pushable_tasks) {
		اگر (task_is_pushable(rq, p, cpu))
			واپس p;
	}

	واپس تہی;
}

ساکن DEFINE_فی_CPU(cpumask_var_t, local_cpu_نقاب);

ساکن صحیح ڈھونڈو_سب_سے_نیچا_rq(ساخت task_struct *task)
{
	ساخت sched_domain *sd;
	ساخت cpumask *سب_سے_نیچا_نقاب = this_cpu_cpumask_var_ptr(local_cpu_نقاب);
	صحیح this_cpu = smp_processor_id();
	صحیح cpu      = task_cpu(task);
	صحیح ret;

	/* Make sure the mask is initialized first */
	اگر (unlikely(!سب_سے_نیچا_نقاب))
		واپس -1;

	اگر (task->nr_cpus_allowed == 1)
		واپس -1; /* No other targets possible */

	/*
	 * If we're on asym system ensure we consider the different capacities
	 * of the CPUs when searching for the lowest_mask.
	 */
	اگر (sched_asym_cpucap_فعال()) {

		ret = cpupri_ڈھونڈو_fitness(&task_rq(task)->rd->cpupri,
					  task, سب_سے_نیچا_نقاب,
					  rt_task_سماتا_استعداد);
	} ورنہ {

		ret = cpupri_ڈھونڈو(&task_rq(task)->rd->cpupri,
				  task, سب_سے_نیچا_نقاب);
	}

	اگر (!ret)
		واپس -1; /* No targets found */

	/*
	 * At this point we have built a mask of CPUs representing the
	 * lowest priority tasks in the system.  Now we want to elect
	 * the best one based on our affinity and topology.
	 *
	 * We prioritize the last CPU that the task executed on since
	 * it is most likely cache-hot in that location.
	 */
	اگر (cpumask_test_cpu(cpu, سب_سے_نیچا_نقاب))
		واپس cpu;

	/*
	 * Otherwise, we consult the sched_domains span maps to figure
	 * out which CPU is logically closest to our hot cache data.
	 */
	اگر (!cpumask_test_cpu(this_cpu, سب_سے_نیچا_نقاب))
		this_cpu = -1; /* Skip this_cpu opt if not among lowest */

	rcu_read_تالا();
	for_each_domain(cpu, sd) {
		اگر (sd->flags & SD_WAKE_AFFINE) {
			صحیح بہترین_cpu;

			/*
			 * "this_cpu" is cheaper to preempt than a
			 * remote processor.
			 */
			اگر (this_cpu != -1 &&
			    cpumask_test_cpu(this_cpu, sched_domain_وسعت(sd))) {
				rcu_read_unlock();
				واپس this_cpu;
			}

			بہترین_cpu = cpumask_any_and_distribute(سب_سے_نیچا_نقاب,
							      sched_domain_وسعت(sd));
			اگر (بہترین_cpu < nr_cpu_ids) {
				rcu_read_unlock();
				واپس بہترین_cpu;
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
		واپس this_cpu;

	cpu = cpumask_any_distribute(سب_سے_نیچا_نقاب);
	اگر (cpu < nr_cpu_ids)
		واپس cpu;

	واپس -1;
}

ساکن ساخت task_struct *pick_اگلا_pushable_task(ساخت rq *rq)
{
	ساخت plist_سر *سر = &rq->rt.pushable_tasks;
	ساخت task_struct *i, *p = تہی;

	اگر (!has_pushable_tasks(rq))
		واپس تہی;

	plist_for_each_اندراج(i, سر, pushable_tasks) {
		/* skip tasks that cannot be migrated */
		اگر (!task_on_cpu(rq, i) && !is_migration_disabled(i)) {
			p = i;
			توڑو;
		}
	}

	اگر (!p)
		واپس تہی;

	BUG_ON(rq->cpu != task_cpu(p));
	BUG_ON(task_current(rq, p));
	BUG_ON(task_current_donor(rq, p));
	BUG_ON(p->nr_cpus_allowed <= 1);

	BUG_ON(!task_on_rq_queued(p));
	BUG_ON(!rt_task(p));

	واپس p;
}

/* Will lock the rq it finds */
ساکن ساخت rq *ڈھونڈو_تالا_سب_سے_نیچا_rq(ساخت task_struct *task, ساخت rq *rq)
{
	ساخت rq *سب_سے_نیچا_rq = تہی;
	صحیح tries;
	صحیح cpu;

	برائے (tries = 0; tries < RT_MAX_TRIES; tries++) {
		cpu = ڈھونڈو_سب_سے_نیچا_rq(task);

		اگر ((cpu == -1) || (cpu == rq->cpu))
			توڑو;

		سب_سے_نیچا_rq = cpu_rq(cpu);

		اگر (سب_سے_نیچا_rq->rt.highest_prio.curr <= task->prio) {
			/*
			 * Target rq has tasks of equal or higher priority,
			 * retrying does not release any lock and is unlikely
			 * to yield a different result.
			 */
			سب_سے_نیچا_rq = تہی;
			توڑو;
		}

		/* if the prio of this runqueue changed, try again */
		اگر (double_تالا_balance(rq, سب_سے_نیچا_rq)) {
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
				     !cpumask_test_cpu(سب_سے_نیچا_rq->cpu, &task->cpus_نقاب) ||
				     task != pick_اگلا_pushable_task(rq))) {

				double_unlock_balance(rq, سب_سے_نیچا_rq);
				سب_سے_نیچا_rq = تہی;
				توڑو;
			}
		}

		/* If this rq is still suitable use it. */
		اگر (سب_سے_نیچا_rq->rt.highest_prio.curr > task->prio)
			توڑو;

		/* try again */
		double_unlock_balance(rq, سب_سے_نیچا_rq);
		سب_سے_نیچا_rq = تہی;
	}

	واپس سب_سے_نیچا_rq;
}

/*
 * If the current CPU has more than one RT task, see if the non
 * running task can migrate over to a CPU that is running a task
 * of lesser priority.
 */
ساکن صحیح ڈالو_rt_task(ساخت rq *rq, bool pull)
{
	ساخت task_struct *اگلا_task;
	ساخت rq *سب_سے_نیچا_rq;
	صحیح ret = 0;

	اگر (!rq->rt.overloaded)
		واپس 0;

	اگلا_task = pick_اگلا_pushable_task(rq);
	اگر (!اگلا_task)
		واپس 0;

retry:
	/*
	 * It's possible that the next_task slipped in of
	 * higher priority than current. If that's the case
	 * just reschedule current.
	 */
	اگر (unlikely(اگلا_task->prio < rq->donor->prio)) {
		resched_curr(rq);
		واپس 0;
	}

	اگر (is_migration_disabled(اگلا_task)) {
		ساخت task_struct *ڈالو_task = تہی;
		صحیح cpu;

		اگر (!pull || rq->ڈالو_busy)
			واپس 0;

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
			واپس 0;

		cpu = ڈھونڈو_سب_سے_نیچا_rq(rq->curr);
		اگر (cpu == -1 || cpu == rq->cpu)
			واپس 0;

		/*
		 * Given we found a CPU with lower priority than @next_task,
		 * therefore it should be running. However we cannot migrate it
		 * to this other CPU, instead attempt to push the current
		 * running task on this CPU away.
		 */
		ڈالو_task = get_ڈالو_task(rq);
		اگر (ڈالو_task) {
			preempt_disable();
			raw_spin_rq_unlock(rq);
			stop_one_cpu_nowait(rq->cpu, ڈالو_cpu_stop,
					    ڈالو_task, &rq->ڈالو_work);
			preempt_enable();
			raw_spin_rq_تالا(rq);
		}

		واپس 0;
	}

	اگر (WARN_ON(اگلا_task == rq->curr))
		واپس 0;

	/* We might release rq lock */
	get_task_struct(اگلا_task);

	/* find_lock_lowest_rq locks the rq if found */
	سب_سے_نیچا_rq = ڈھونڈو_تالا_سب_سے_نیچا_rq(اگلا_task, rq);
	اگر (!سب_سے_نیچا_rq) {
		ساخت task_struct *task;
		/*
		 * find_lock_lowest_rq releases rq->lock
		 * so it is possible that next_task has migrated.
		 *
		 * We need to make sure that the task is still on the same
		 * run-queue and is also still the next task eligible for
		 * pushing.
		 */
		task = pick_اگلا_pushable_task(rq);
		اگر (task == اگلا_task) {
			/*
			 * The task hasn't migrated, and is still the next
			 * eligible task, but we failed to find a run-queue
			 * to push it to.  Do not retry in this case, since
			 * other CPUs will pull from us when ready.
			 */
			جاؤ برآمد;
		}

		اگر (!task)
			/* No more tasks, just exit */
			جاؤ برآمد;

		/*
		 * Something has shifted, try again.
		 */
		put_task_struct(اگلا_task);
		اگلا_task = task;
		جاؤ retry;
	}

	move_queued_task_locked(rq, سب_سے_نیچا_rq, اگلا_task);
	resched_curr(سب_سے_نیچا_rq);
	ret = 1;

	double_unlock_balance(rq, سب_سے_نیچا_rq);
برآمد:
	put_task_struct(اگلا_task);

	واپس ret;
}

ساکن خالی ڈالو_rt_tasks(ساخت rq *rq)
{
	/* push_rt_task will return true if it moved an RT */
	جب (ڈالو_rt_task(rq, false))
		;
}

#اگر_تعریف_شدہ HAVE_RT_ڈالو_IPI

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
ساکن صحیح rto_اگلا_cpu(ساخت جڑ_domain *rd)
{
	صحیح this_cpu = smp_processor_id();
	صحیح اگلا;
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
	برائے (;;) {

		/* When rto_cpu is -1 this acts like cpumask_first() */
		cpu = cpumask_اگلا(rd->rto_cpu, rd->rto_نقاب);

		rd->rto_cpu = cpu;

		/* Do not send IPI to self */
		اگر (cpu == this_cpu)
			جاری;

		اگر (cpu < nr_cpu_ids)
			واپس cpu;

		rd->rto_cpu = -1;

		/*
		 * ACQUIRE ensures we see the @rto_mask changes
		 * made prior to the @next value observed.
		 *
		 * Matches WMB in rt_set_overload().
		 */
		اگلا = atomic_read_acquire(&rd->rto_loop_اگلا);

		اگر (rd->rto_loop == اگلا)
			توڑو;

		rd->rto_loop = اگلا;
	}

	واپس -1;
}

ساکن اندرونی bool rto_آغاز_trylock(atomic_t *v)
{
	واپس !atomic_cmpxchg_acquire(v, 0, 1);
}

ساکن اندرونی خالی rto_آغاز_unlock(atomic_t *v)
{
	atomic_سیٹ_release(v, 0);
}

ساکن خالی tell_cpu_مقصد_ڈالو(ساخت rq *rq)
{
	صحیح cpu = -1;

	/* Keep the loop going if the IPI is currently active */
	atomic_inc(&rq->rd->rto_loop_اگلا);

	/* Only one CPU can initiate a loop at a time */
	اگر (!rto_آغاز_trylock(&rq->rd->rto_loop_آغاز))
		واپس;

	raw_spin_تالا(&rq->rd->rto_تالا);

	/*
	 * The rto_cpu is updated under the lock, if it has a valid CPU
	 * then the IPI is still running and will continue due to the
	 * update to loop_next, and nothing needs to be done here.
	 * Otherwise it is finishing up and an IPI needs to be sent.
	 */
	اگر (rq->rd->rto_cpu < 0)
		cpu = rto_اگلا_cpu(rq->rd);

	raw_spin_unlock(&rq->rd->rto_تالا);

	rto_آغاز_unlock(&rq->rd->rto_loop_آغاز);

	اگر (cpu >= 0) {
		/* Make sure the rd does not get freed while pushing */
		sched_get_rd(rq->rd);
		irq_work_قطار_on(&rq->rd->rto_ڈالو_work, cpu);
	}
}

/* Called from hardirq context */
خالی rto_ڈالو_irq_work_func(ساخت irq_work *work)
{
	ساخت جڑ_domain *rd =
		container_کا(work, ساخت جڑ_domain, rto_ڈالو_work);
	ساخت rq *rq;
	صحیح cpu;

	rq = this_rq();

	/*
	 * We do not need to grab the lock to check for has_pushable_tasks.
	 * When it gets updated, a check is made if a push is possible.
	 */
	اگر (has_pushable_tasks(rq)) {
		raw_spin_rq_تالا(rq);
		جب (ڈالو_rt_task(rq, true))
			;
		raw_spin_rq_unlock(rq);
	}

	raw_spin_تالا(&rd->rto_تالا);

	/* Pass the IPI to the next rt overloaded queue */
	cpu = rto_اگلا_cpu(rd);

	raw_spin_unlock(&rd->rto_تالا);

	اگر (cpu < 0) {
		sched_put_rd(rd);
		واپس;
	}

	/* Try the next RT overloaded CPU */
	irq_work_قطار_on(&rd->rto_ڈالو_work, cpu);
}
#اگر_ختم /* HAVE_RT_PUSH_IPI */

ساکن خالی pull_rt_task(ساخت rq *this_rq)
{
	صحیح this_cpu = this_rq->cpu, cpu;
	bool resched = false;
	ساخت task_struct *p, *ڈالو_task;
	ساخت rq *src_rq;
	صحیح rt_overload_گنتی = rt_overloaded(this_rq);

	اگر (likely(!rt_overload_گنتی))
		واپس;

	/*
	 * Match the barrier from rt_set_overloaded; this guarantees that if we
	 * see overloaded we must also see the rto_mask bit.
	 */
	smp_rmb();

	/* If we are the only overloaded CPU do nothing */
	اگر (rt_overload_گنتی == 1 &&
	    cpumask_test_cpu(this_rq->cpu, this_rq->rd->rto_نقاب))
		واپس;

#اگر_تعریف_شدہ HAVE_RT_ڈالو_IPI
	اگر (sched_feat(RT_ڈالو_IPI)) {
		tell_cpu_مقصد_ڈالو(this_rq);
		واپس;
	}
#اگر_ختم

	for_each_cpu(cpu, this_rq->rd->rto_نقاب) {
		اگر (this_cpu == cpu)
			جاری;

		src_rq = cpu_rq(cpu);

		/*
		 * Don't bother taking the src_rq->lock if the next highest
		 * task is known to be lower-priority than our current task.
		 * This may look racy, but if this value is about to go
		 * logically higher, the src_rq will push this task away.
		 * And if its going logically lower, we do not care
		 */
		اگر (src_rq->rt.highest_prio.اگلا >=
		    this_rq->rt.highest_prio.curr)
			جاری;

		/*
		 * We can potentially drop this_rq's lock in
		 * double_lock_balance, and another CPU could
		 * alter this_rq
		 */
		ڈالو_task = تہی;
		double_تالا_balance(this_rq, src_rq);

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
				جاؤ skip;

			اگر (is_migration_disabled(p)) {
				ڈالو_task = get_ڈالو_task(src_rq);
			} ورنہ {
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

		اگر (ڈالو_task) {
			preempt_disable();
			raw_spin_rq_unlock(this_rq);
			stop_one_cpu_nowait(src_rq->cpu, ڈالو_cpu_stop,
					    ڈالو_task, &src_rq->ڈالو_work);
			preempt_enable();
			raw_spin_rq_تالا(this_rq);
		}
	}

	اگر (resched)
		resched_curr(this_rq);
}

/*
 * If we are not running and we are not going to reschedule soon, we should
 * try to push tasks away now
 */
ساکن خالی task_woken_rt(ساخت rq *rq, ساخت task_struct *p)
{
	bool need_مقصد_ڈالو = !task_on_cpu(rq, p) &&
			    !test_tsk_need_resched(rq->curr) &&
			    p->nr_cpus_allowed > 1 &&
			    (dl_task(rq->donor) || rt_task(rq->donor)) &&
			    (rq->curr->nr_cpus_allowed < 2 ||
			     rq->donor->prio <= p->prio);

	اگر (need_مقصد_ڈالو)
		ڈالو_rt_tasks(rq);
}

/* Assumes rq->lock is held */
ساکن خالی rq_online_rt(ساخت rq *rq)
{
	اگر (rq->rt.overloaded)
		rt_سیٹ_overload(rq);

	__enable_runtime(rq);

	cpupri_سیٹ(&rq->rd->cpupri, rq->cpu, rq->rt.highest_prio.curr);
}

/* Assumes rq->lock is held */
ساکن خالی rq_offline_rt(ساخت rq *rq)
{
	اگر (rq->rt.overloaded)
		rt_clear_overload(rq);

	__disable_runtime(rq);

	cpupri_سیٹ(&rq->rd->cpupri, rq->cpu, CPUPRI_INVALID);
}

/*
 * When switch from the rt queue, we bring ourselves to a position
 * that we might want to pull RT tasks from other runqueues.
 */
ساکن خالی switched_from_rt(ساخت rq *rq, ساخت task_struct *p)
{
	/*
	 * If there are other RT tasks then we will reschedule
	 * and the scheduling of the other RT tasks will handle
	 * the balancing. But if we are the last RT task
	 * we may need to handle the pulling of RT tasks
	 * now.
	 */
	اگر (!task_on_rq_queued(p) || rq->rt.rt_nr_running)
		واپس;

	rt_قطار_pull_task(rq);
}

خالی __init init_sched_rt_class(خالی)
{
	غیرعلامتی صحیح i;

	for_each_possible_cpu(i) {
		zalloc_cpumask_var_گرہ(&فی_cpu(local_cpu_نقاب, i),
					GFP_KERNEL, cpu_مقصد_گرہ(i));
	}
}

/*
 * When switching a task to RT, we may overload the runqueue
 * with RT tasks. In this case we try to push them off to
 * other runqueues.
 */
ساکن خالی switched_مقصد_rt(ساخت rq *rq, ساخت task_struct *p)
{
	/*
	 * If we are running, update the avg_rt tracking, as the running time
	 * will now on be accounted into the latter.
	 */
	اگر (task_current(rq, p)) {
		update_rt_rq_بھرو_avg(rq_clock_pelt(rq), rq, 0);
		واپس;
	}

	/*
	 * If we are not running we may need to preempt the current
	 * running task. If that current running task is also an RT task
	 * then see if we can move to another run queue.
	 */
	اگر (task_on_rq_queued(p)) {
		اگر (p->nr_cpus_allowed > 1 && rq->rt.overloaded)
			rt_قطار_ڈالو_tasks(rq);
		اگر (p->prio < rq->donor->prio && cpu_online(cpu_کا(rq)))
			resched_curr(rq);
	}
}

/*
 * Priority of the task has changed. This may cause
 * us to initiate a push or pull.
 */
ساکن خالی
prio_بدلا_rt(ساخت rq *rq, ساخت task_struct *p, u64 oldprio)
{
	اگر (!task_on_rq_queued(p))
		واپس;

	اگر (p->prio == oldprio)
		واپس;

	اگر (task_current_donor(rq, p)) {
		/*
		 * If our priority decreases while running, we
		 * may need to pull tasks to this runqueue.
		 */
		اگر (oldprio < p->prio)
			rt_قطار_pull_task(rq);

		/*
		 * If there's a higher priority task waiting to run
		 * then reschedule.
		 */
		اگر (p->prio > rq->rt.highest_prio.curr)
			resched_curr(rq);
	} ورنہ {
		/*
		 * This task is not running, but if it is
		 * greater than the current running task
		 * then reschedule.
		 */
		اگر (p->prio < rq->donor->prio)
			resched_curr(rq);
	}
}

#اگر_تعریف_شدہ CONFIG_POSIX_TIMERS
ساکن خالی watchdog(ساخت rq *rq, ساخت task_struct *p)
{
	غیرعلامتی طویل soft, hard;

	/* max may change after cur was read, this will be fixed next tick */
	soft = task_rlimit(p, RLIMIT_RTTIME);
	hard = task_rlimit_max(p, RLIMIT_RTTIME);

	اگر (soft != RLIM_INFINITY) {
		غیرعلامتی طویل اگلا;

		اگر (p->rt.watchdog_stamp != jiffies) {
			p->rt.timeout++;
			p->rt.watchdog_stamp = jiffies;
		}

		اگلا = DIV_باری_اوپر(min(soft, hard), USEC_فی_SEC/HZ);
		اگر (p->rt.timeout > اگلا) {
			posix_cputimers_rt_watchdog(&p->posix_cputimers,
						    p->se.مجموع_exec_runtime);
		}
	}
}
#ورنہ /* !CONFIG_POSIX_TIMERS: */
ساکن اندرونی خالی watchdog(ساخت rq *rq, ساخت task_struct *p) { }
#اگر_ختم /* !CONFIG_POSIX_TIMERS */

/*
 * scheduler tick hitting a task of our scheduling class.
 *
 * NOTE: This function can be called remotely by the tick offload that
 * goes along full dynticks. Therefore no local assumption can be made
 * and everything must be accessed through the @rq and @curr passed in
 * parameters.
 */
ساکن خالی task_tick_rt(ساخت rq *rq, ساخت task_struct *p, صحیح queued)
{
	ساخت sched_rt_entity *rt_se = &p->rt;

	update_curr_rt(rq);
	update_rt_rq_بھرو_avg(rq_clock_pelt(rq), rq, 1);

	watchdog(rq, p);

	/*
	 * RR tasks need a special form of time-slice management.
	 * FIFO tasks have no timeslices.
	 */
	اگر (p->policy != SCHED_RR)
		واپس;

	اگر (--p->rt.وقت_قاش)
		واپس;

	p->rt.وقت_قاش = sched_rr_timeslice;

	/*
	 * Requeue to the end of queue if we (and all of our ancestors) are not
	 * the only element on the queue
	 */
	for_each_sched_rt_entity(rt_se) {
		اگر (rt_se->تسلسل_list.پچھلا != rt_se->تسلسل_list.اگلا) {
			requeue_task_rt(rq, p, 0);
			resched_curr(rq);
			واپس;
		}
	}
}

ساکن غیرعلامتی صحیح get_rr_interval_rt(ساخت rq *rq, ساخت task_struct *task)
{
	/*
	 * Time slice is 0 for SCHED_FIFO tasks
	 */
	اگر (task->policy == SCHED_RR)
		واپس sched_rr_timeslice;
	ورنہ
		واپس 0;
}

#اگر_تعریف_شدہ CONFIG_SCHED_CORE
ساکن صحیح task_is_throttled_rt(ساخت task_struct *p, صحیح cpu)
{
	ساخت rt_rq *rt_rq;

#اگر_تعریف_شدہ CONFIG_RT_گروہ_SCHED // XXX maybe add task_rt_rq(), see also sched_rt_period_rt_rq
	rt_rq = task_گروہ(p)->rt_rq[cpu];
	WARN_ON(!rt_گروہ_sched_enabled() && rt_rq->tg != &جڑ_task_گروہ);
#ورنہ
	rt_rq = &cpu_rq(cpu)->rt;
#اگر_ختم

	واپس rt_rq_throttled(rt_rq);
}
#اگر_ختم /* CONFIG_SCHED_CORE */

DEFINE_SCHED_CLASS(rt) = {
	.قطار_میں_ڈالو_task		= قطار_میں_ڈالو_task_rt,
	.قطار_سے_نکالو_task		= قطار_سے_نکالو_task_rt,
	.yield_task		= yield_task_rt,

	.wakeup_preempt		= wakeup_preempt_rt,

	.pick_task		= pick_task_rt,
	.put_پچھلا_task		= put_پچھلا_task_rt,
	.سیٹ_اگلا_task          = سیٹ_اگلا_task_rt,

	.balance		= balance_rt,
	.select_task_rq		= select_task_rq_rt,
	.سیٹ_cpus_allowed       = سیٹ_cpus_allowed_common,
	.rq_online              = rq_online_rt,
	.rq_offline             = rq_offline_rt,
	.task_woken		= task_woken_rt,
	.switched_from		= switched_from_rt,
	.ڈھونڈو_تالا_rq		= ڈھونڈو_تالا_سب_سے_نیچا_rq,

	.task_tick		= task_tick_rt,

	.get_rr_interval	= get_rr_interval_rt,

	.switched_مقصد		= switched_مقصد_rt,
	.prio_بدلا		= prio_بدلا_rt,

	.update_curr		= update_curr_rt,

#اگر_تعریف_شدہ CONFIG_SCHED_CORE
	.task_is_throttled	= task_is_throttled_rt,
#اگر_ختم

#اگر_تعریف_شدہ CONFIG_UCLAMP_TASK
	.uclamp_enabled		= 1,
#اگر_ختم
};

#اگر_تعریف_شدہ CONFIG_RT_گروہ_SCHED
/*
 * Ensure that the real time constraints are schedulable.
 */
ساکن DEFINE_MUTEX(rt_constraints_mutex);

ساکن اندرونی صحیح tg_has_rt_tasks(ساخت task_گروہ *tg)
{
	ساخت task_struct *task;
	ساخت css_task_iter it;
	صحیح ret = 0;

	/*
	 * Autogroups do not have RT tasks; see autogroup_create().
	 */
	اگر (task_گروہ_is_autogroup(tg))
		واپس 0;

	css_task_iter_آغاز(&tg->css, 0, &it);
	جب (!ret && (task = css_task_iter_اگلا(&it)))
		ret |= rt_task(task);
	css_task_iter_انتہا(&it);

	واپس ret;
}

ساخت rt_schedulable_ڈیٹا {
	ساخت task_گروہ *tg;
	u64 rt_period;
	u64 rt_runtime;
};

ساکن صحیح tg_rt_schedulable(ساخت task_گروہ *tg, خالی *ڈیٹا)
{
	ساخت rt_schedulable_ڈیٹا *d = ڈیٹا;
	ساخت task_گروہ *بچہ;
	u64 کل, مجموع = 0;
	u64 period, runtime;

	period = ktime_مقصد_ns(tg->rt_bandwidth.rt_period);
	runtime = tg->rt_bandwidth.rt_runtime;

	اگر (tg == d->tg) {
		period = d->rt_period;
		runtime = d->rt_runtime;
	}

	/*
	 * Cannot have more runtime than the period.
	 */
	اگر (runtime > period && runtime != RUNTIME_INF)
		واپس -EINVAL;

	/*
	 * Ensure we don't starve existing RT tasks if runtime turns zero.
	 */
	اگر (rt_bandwidth_enabled() && !runtime &&
	    tg->rt_bandwidth.rt_runtime && tg_has_rt_tasks(tg))
		واپس -EBUSY;

	کل = مقصد_تناسب(period, runtime);

	/*
	 * Nobody can have more than the global setting allows.
	 */
	اگر (کل > مقصد_تناسب(global_rt_period(), global_rt_runtime()))
		واپس -EINVAL;

	/*
	 * The sum of our children's runtime should not exceed our own.
	 */
	list_for_each_اندراج_rcu(بچہ, &tg->children, siblings) {
		period = ktime_مقصد_ns(بچہ->rt_bandwidth.rt_period);
		runtime = بچہ->rt_bandwidth.rt_runtime;

		اگر (بچہ == d->tg) {
			period = d->rt_period;
			runtime = d->rt_runtime;
		}

		مجموع += مقصد_تناسب(period, runtime);
	}

	اگر (مجموع > کل)
		واپس -EINVAL;

	واپس 0;
}

ساکن صحیح __rt_schedulable(ساخت task_گروہ *tg, u64 period, u64 runtime)
{
	صحیح ret;

	ساخت rt_schedulable_ڈیٹا ڈیٹا = {
		.tg = tg,
		.rt_period = period,
		.rt_runtime = runtime,
	};

	rcu_read_تالا();
	ret = walk_tg_درخت(tg_rt_schedulable, tg_nop, &ڈیٹا);
	rcu_read_unlock();

	واپس ret;
}

ساکن صحیح tg_سیٹ_rt_bandwidth(ساخت task_گروہ *tg,
		u64 rt_period, u64 rt_runtime)
{
	صحیح i, err = 0;

	/*
	 * Disallowing the root group RT runtime is BAD, it would disallow the
	 * kernel creating (and or operating) RT threads.
	 */
	اگر (tg == &جڑ_task_گروہ && rt_runtime == 0)
		واپس -EINVAL;

	/* No period doesn't make any sense. */
	اگر (rt_period == 0)
		واپس -EINVAL;

	/*
	 * Bound quota to defend quota against overflow during bandwidth shift.
	 */
	اگر (rt_runtime != RUNTIME_INF && rt_runtime > max_rt_runtime)
		واپس -EINVAL;

	mutex_تالا(&rt_constraints_mutex);
	err = __rt_schedulable(tg, rt_period, rt_runtime);
	اگر (err)
		جاؤ unlock;

	raw_spin_تالا_irq(&tg->rt_bandwidth.rt_runtime_تالا);
	tg->rt_bandwidth.rt_period = ns_مقصد_ktime(rt_period);
	tg->rt_bandwidth.rt_runtime = rt_runtime;

	for_each_possible_cpu(i) {
		ساخت rt_rq *rt_rq = tg->rt_rq[i];

		raw_spin_تالا(&rt_rq->rt_runtime_تالا);
		rt_rq->rt_runtime = rt_runtime;
		raw_spin_unlock(&rt_rq->rt_runtime_تالا);
	}
	raw_spin_unlock_irq(&tg->rt_bandwidth.rt_runtime_تالا);
unlock:
	mutex_unlock(&rt_constraints_mutex);

	واپس err;
}

صحیح sched_گروہ_سیٹ_rt_runtime(ساخت task_گروہ *tg, طویل rt_runtime_us)
{
	u64 rt_runtime, rt_period;

	rt_period = ktime_مقصد_ns(tg->rt_bandwidth.rt_period);
	rt_runtime = (u64)rt_runtime_us * NSEC_فی_USEC;
	اگر (rt_runtime_us < 0)
		rt_runtime = RUNTIME_INF;
	ورنہ اگر ((u64)rt_runtime_us > U64_MAX / NSEC_فی_USEC)
		واپس -EINVAL;

	واپس tg_سیٹ_rt_bandwidth(tg, rt_period, rt_runtime);
}

طویل sched_گروہ_rt_runtime(ساخت task_گروہ *tg)
{
	u64 rt_runtime_us;

	اگر (tg->rt_bandwidth.rt_runtime == RUNTIME_INF)
		واپس -1;

	rt_runtime_us = tg->rt_bandwidth.rt_runtime;
	do_div(rt_runtime_us, NSEC_فی_USEC);
	واپس rt_runtime_us;
}

صحیح sched_گروہ_سیٹ_rt_period(ساخت task_گروہ *tg, u64 rt_period_us)
{
	u64 rt_runtime, rt_period;

	اگر (rt_period_us > U64_MAX / NSEC_فی_USEC)
		واپس -EINVAL;

	rt_period = rt_period_us * NSEC_فی_USEC;
	rt_runtime = tg->rt_bandwidth.rt_runtime;

	واپس tg_سیٹ_rt_bandwidth(tg, rt_period, rt_runtime);
}

طویل sched_گروہ_rt_period(ساخت task_گروہ *tg)
{
	u64 rt_period_us;

	rt_period_us = ktime_مقصد_ns(tg->rt_bandwidth.rt_period);
	do_div(rt_period_us, NSEC_فی_USEC);
	واپس rt_period_us;
}

صحیح sched_rt_can_attach(ساخت task_گروہ *tg, ساخت task_struct *tsk)
{
	/* Don't accept real-time tasks when there is no way for them to run */
	اگر (rt_گروہ_sched_enabled() && rt_task(tsk) && tg->rt_bandwidth.rt_runtime == 0)
		واپس 0;

	واپس 1;
}

#اگر_ختم /* !CONFIG_RT_GROUP_SCHED */

#اگر_تعریف_شدہ CONFIG_SYSCTL
ساکن صحیح sched_rt_global_validate(خالی)
{
	اگر ((sysctl_sched_rt_runtime != RUNTIME_INF) &&
		((sysctl_sched_rt_runtime > sysctl_sched_rt_period) ||
		 ((u64)sysctl_sched_rt_runtime *
			NSEC_فی_USEC > max_rt_runtime)))
		واپس -EINVAL;

#اگر_تعریف_شدہ CONFIG_RT_گروہ_SCHED
	اگر (!rt_گروہ_sched_enabled())
		واپس 0;

	scoped_guard(mutex, &rt_constraints_mutex)
		واپس __rt_schedulable(تہی, 0, 0);
#اگر_ختم
	واپس 0;
}

ساکن صحیح sched_rt_handler(ثابت ساخت ctl_جدول *جدول, صحیح اكتب, خالی *بفر,
		سائز_t *lenp, loff_t *ppos)
{
	صحیح old_period, old_runtime;
	ساکن DEFINE_MUTEX(mutex);
	صحیح ret;

	mutex_تالا(&mutex);
	sched_domains_mutex_تالا();
	old_period = sysctl_sched_rt_period;
	old_runtime = sysctl_sched_rt_runtime;

	ret = proc_dointvec_minmax(جدول, اكتب, بفر, lenp, ppos);

	اگر (!ret && اكتب) {
		ret = sched_rt_global_validate();
		اگر (ret)
			جاؤ undo;

		ret = sched_dl_global_validate();
		اگر (ret)
			جاؤ undo;

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

	واپس ret;
}

ساکن صحیح sched_rr_handler(ثابت ساخت ctl_جدول *جدول, صحیح اكتب, خالی *بفر,
		سائز_t *lenp, loff_t *ppos)
{
	صحیح ret;
	ساکن DEFINE_MUTEX(mutex);

	mutex_تالا(&mutex);
	ret = proc_dointvec(جدول, اكتب, بفر, lenp, ppos);
	/*
	 * Make sure that internally we keep jiffies.
	 * Also, writing zero resets the time-slice to default:
	 */
	اگر (!ret && اكتب) {
		sched_rr_timeslice =
			sysctl_sched_rr_timeslice <= 0 ? RR_TIMESLICE :
			msecs_مقصد_jiffies(sysctl_sched_rr_timeslice);

		اگر (sysctl_sched_rr_timeslice <= 0)
			sysctl_sched_rr_timeslice = jiffies_مقصد_msecs(RR_TIMESLICE);
	}
	mutex_unlock(&mutex);

	واپس ret;
}
#اگر_ختم /* CONFIG_SYSCTL */

خالی print_rt_stats(ساخت seq_file *m, صحیح cpu)
{
	rt_rq_iter_t iter;
	ساخت rt_rq *rt_rq;

	rcu_read_تالا();
	for_each_rt_rq(rt_rq, iter, cpu_rq(cpu))
		print_rt_rq(m, cpu, rt_rq);
	rcu_read_unlock();
}
