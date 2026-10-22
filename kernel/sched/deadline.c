// SPDX-License-Identifier: GPL-2.0
/*
 * Deadline Scheduling Class (SCHED_DEADLINE)
 *
 * Earliest Deadline First (EDF) + Constant Bandwidth Server (CBS).
 *
 * Tasks that periodically executes their instances for less than their
 * runtime won't miss any of their deadlines.
 * Tasks that are not periodic or sporadic or that tries to execute more
 * than their reserved bandwidth will be slowed down (and may potentially
 * miss some of their deadlines), and won't affect any other task.
 *
 * Copyright (C) 2012 Dario Faggioli <raistlin@linux.it>,
 *                    Juri Lelli <juri.lelli@gmail.com>,
 *                    Michael Trimarchi <michael@amarulasolutions.com>,
 *                    Fabio Checconi <fchecconi@gmail.com>
 */

#అన్తర్భావ <linux/cpuset.h>
#అన్తర్భావ <linux/sched/clock.h>
#అన్తర్భావ <linux/sched/deadline.h>
#అన్తర్భావ <uapi/linux/sched/types.h>
#అన్తర్భావ "sched.h"
#అన్తర్భావ "pelt.h"

/*
 * Default limits for DL period; on the top end we guard against small util
 * tasks still getting ridiculously long effective runtimes, on the bottom end we
 * guard against timer DoS.
 */
స్థిర గుర్తులేని పూర్ణాంకం sysctl_sched_dl_period_max = 1 << 22; /* ~4 seconds */
స్థిర గుర్తులేని పూర్ణాంకం sysctl_sched_dl_period_min = 100;     /* 100 us */
#నిర్వచితమైతే CONFIG_SYSCTL
స్థిర స్థిరరాశి నిర్మాణం ctl_పట్టిక sched_dl_sysctls[] = {
	{
		.procname       = "sched_deadline_period_max_us",
		.దత్తాంశం           = &sysctl_sched_dl_period_max,
		.maxlen         = పరిమాణం(గుర్తులేని పూర్ణాంకం),
		.బహుళకం           = 0644,
		.proc_handler   = proc_douintvec_minmax,
		.extra1         = (శూన్య *)&sysctl_sched_dl_period_min,
	},
	{
		.procname       = "sched_deadline_period_min_us",
		.దత్తాంశం           = &sysctl_sched_dl_period_min,
		.maxlen         = పరిమాణం(గుర్తులేని పూర్ణాంకం),
		.బహుళకం           = 0644,
		.proc_handler   = proc_douintvec_minmax,
		.extra2         = (శూన్య *)&sysctl_sched_dl_period_max,
	},
};

స్థిర పూర్ణాంకం __init sched_dl_sysctl_init(శూన్య)
{
	రిజిస్టరు_sysctl_init("kernel", sched_dl_sysctls);
	తిరిగి 0;
}
late_initcall(sched_dl_sysctl_init);
#అయితే_ముగింపు /* CONFIG_SYSCTL */

స్థిర అంతఃవరుస నిర్మాణం rq *rq_యొక్క_dl_rq(నిర్మాణం dl_rq *dl_rq)
{
	తిరిగి container_యొక్క(dl_rq, నిర్మాణం rq, dl);
}

స్థిర అంతఃవరుస నిర్మాణం rq *rq_యొక్క_dl_se(నిర్మాణం sched_dl_entity *dl_se)
{
	నిర్మాణం rq *rq = dl_se->rq;

	ఒకవేళ (!dl_server(dl_se))
		rq = task_rq(dl_task_యొక్క(dl_se));

	తిరిగి rq;
}

స్థిర అంతఃవరుస నిర్మాణం dl_rq *dl_rq_యొక్క_se(నిర్మాణం sched_dl_entity *dl_se)
{
	తిరిగి &rq_యొక్క_dl_se(dl_se)->dl;
}

స్థిర అంతఃవరుస పూర్ణాంకం on_dl_rq(నిర్మాణం sched_dl_entity *dl_se)
{
	తిరిగి !RB_ఖాళీలు_కణుపు(&dl_se->rb_కణుపు);
}

#నిర్వచితమైతే CONFIG_RT_MUTEXES
స్థిర అంతఃవరుస నిర్మాణం sched_dl_entity *pi_యొక్క(నిర్మాణం sched_dl_entity *dl_se)
{
	తిరిగి dl_se->pi_se;
}

స్థిర అంతఃవరుస bool is_dl_boosted(నిర్మాణం sched_dl_entity *dl_se)
{
	తిరిగి pi_యొక్క(dl_se) != dl_se;
}
#లేకపోతే /* !CONFIG_RT_MUTEXES: */
స్థిర అంతఃవరుస నిర్మాణం sched_dl_entity *pi_యొక్క(నిర్మాణం sched_dl_entity *dl_se)
{
	తిరిగి dl_se;
}

స్థిర అంతఃవరుస bool is_dl_boosted(నిర్మాణం sched_dl_entity *dl_se)
{
	తిరిగి false;
}
#అయితే_ముగింపు /* !CONFIG_RT_MUTEXES */

స్థిర అంతఃవరుస u8 dl_get_type(నిర్మాణం sched_dl_entity *dl_se, నిర్మాణం rq *rq)
{
	ఒకవేళ (!dl_server(dl_se))
		తిరిగి DL_TASK;
	ఒకవేళ (dl_se == &rq->fair_server)
		తిరిగి DL_SERVER_FAIR;
#నిర్వచితమైతే CONFIG_SCHED_CLASS_EXT
	ఒకవేళ (dl_se == &rq->ext_server)
		తిరిగి DL_SERVER_EXT;
#అయితే_ముగింపు
	తిరిగి DL_OTHER;
}

స్థిర అంతఃవరుస నిర్మాణం dl_bw *dl_bw_యొక్క(పూర్ణాంకం i)
{
	RCU_LOCKDEP_WARN(!rcu_read_తాళం_sched_held(),
			 "sched RCU must be held");
	తిరిగి &cpu_rq(i)->rd->dl_bw;
}

స్థిర అంతఃవరుస పూర్ణాంకం dl_bw_cpus(పూర్ణాంకం i)
{
	నిర్మాణం మూలం_domain *rd = cpu_rq(i)->rd;

	RCU_LOCKDEP_WARN(!rcu_read_తాళం_sched_held(),
			 "sched RCU must be held");

	తిరిగి cpumask_బరువు_and(rd->పరిధి, cpu_చురుకైన_ముసుగు);
}

స్థిర అంతఃవరుస గుర్తులేని దీర్ఘ __dl_bw_capacity(స్థిరరాశి నిర్మాణం cpumask *ముసుగు)
{
	గుర్తులేని దీర్ఘ cap = 0;
	పూర్ణాంకం i;

	for_each_cpu_and(i, ముసుగు, cpu_చురుకైన_ముసుగు)
		cap += arch_scale_cpu_సామర్థ్యం(i);

	తిరిగి cap;
}

/*
 * XXX Fix: If 'rq->rd == def_root_domain' perform AC against capacity
 * of the CPU the task is running on rather rd's \Sum CPU capacity.
 */
స్థిర అంతఃవరుస గుర్తులేని దీర్ఘ dl_bw_సామర్థ్యం(పూర్ణాంకం i)
{
	ఒకవేళ (!sched_asym_cpucap_చురుకైన() &&
	    arch_scale_cpu_సామర్థ్యం(i) == SCHED_సామర్థ్యం_SCALE) {
		తిరిగి dl_bw_cpus(i) << SCHED_సామర్థ్యం_జరుపు;
	} లేకపోతే {
		RCU_LOCKDEP_WARN(!rcu_read_తాళం_sched_held(),
				 "sched RCU must be held");

		తిరిగి __dl_bw_capacity(cpu_rq(i)->rd->పరిధి);
	}
}

bool dl_bw_సందర్శించిన(పూర్ణాంకం cpu, u64 cookie)
{
	నిర్మాణం మూలం_domain *rd = cpu_rq(cpu)->rd;

	ఒకవేళ (rd->సందర్శించు_cookie == cookie)
		తిరిగి true;

	rd->సందర్శించు_cookie = cookie;
	తిరిగి false;
}

స్థిర అంతఃవరుస
శూన్య __dl_update(నిర్మాణం dl_bw *dl_b, s64 bw)
{
	నిర్మాణం మూలం_domain *rd = container_యొక్క(dl_b, నిర్మాణం మూలం_domain, dl_bw);
	పూర్ణాంకం i;

	RCU_LOCKDEP_WARN(!rcu_read_తాళం_sched_held(),
			 "sched RCU must be held");
	for_each_cpu_and(i, rd->పరిధి, cpu_చురుకైన_ముసుగు) {
		నిర్మాణం rq *rq = cpu_rq(i);

		rq->dl.extra_bw += bw;
	}
}

స్థిర అంతఃవరుస
శూన్య __dl_sub(నిర్మాణం dl_bw *dl_b, u64 tsk_bw, పూర్ణాంకం cpus)
{
	dl_b->మొత్తం_bw -= tsk_bw;
	__dl_update(dl_b, (s32)tsk_bw / cpus);
}

స్థిర అంతఃవరుస
శూన్య __dl_add(నిర్మాణం dl_bw *dl_b, u64 tsk_bw, పూర్ణాంకం cpus)
{
	dl_b->మొత్తం_bw += tsk_bw;
	__dl_update(dl_b, -((s32)tsk_bw / cpus));
}

స్థిర అంతఃవరుస bool
__dl_overflow(నిర్మాణం dl_bw *dl_b, గుర్తులేని దీర్ఘ cap, u64 old_bw, u64 new_bw)
{
	తిరిగి dl_b->bw != -1 &&
	       cap_scale(dl_b->bw, cap) < dl_b->మొత్తం_bw - old_bw + new_bw;
}

స్థిర అంతఃవరుస
శూన్య __add_running_bw(u64 dl_bw, నిర్మాణం dl_rq *dl_rq)
{
	u64 old = dl_rq->running_bw;

	lockdep_assert_rq_held(rq_యొక్క_dl_rq(dl_rq));
	dl_rq->running_bw += dl_bw;
	WARN_ON_ONCE(dl_rq->running_bw < old); /*అధికప్రవాహం*/
	WARN_ON_ONCE(dl_rq->running_bw > dl_rq->this_bw);
	/* kick cpufreq (see the comment in kernel/sched/sched.h). */
	cpufreq_update_util(rq_యొక్క_dl_rq(dl_rq), 0);
}

స్థిర అంతఃవరుస
శూన్య __sub_running_bw(u64 dl_bw, నిర్మాణం dl_rq *dl_rq)
{
	u64 old = dl_rq->running_bw;

	lockdep_assert_rq_held(rq_యొక్క_dl_rq(dl_rq));
	dl_rq->running_bw -= dl_bw;
	WARN_ON_ONCE(dl_rq->running_bw > old); /* underflow */
	ఒకవేళ (dl_rq->running_bw > old)
		dl_rq->running_bw = 0;
	/* kick cpufreq (see the comment in kernel/sched/sched.h). */
	cpufreq_update_util(rq_యొక్క_dl_rq(dl_rq), 0);
}

స్థిర అంతఃవరుస
శూన్య __add_rq_bw(u64 dl_bw, నిర్మాణం dl_rq *dl_rq)
{
	u64 old = dl_rq->this_bw;

	lockdep_assert_rq_held(rq_యొక్క_dl_rq(dl_rq));
	dl_rq->this_bw += dl_bw;
	WARN_ON_ONCE(dl_rq->this_bw < old); /*అధికప్రవాహం*/
}

స్థిర అంతఃవరుస
శూన్య __sub_rq_bw(u64 dl_bw, నిర్మాణం dl_rq *dl_rq)
{
	u64 old = dl_rq->this_bw;

	lockdep_assert_rq_held(rq_యొక్క_dl_rq(dl_rq));
	dl_rq->this_bw -= dl_bw;
	WARN_ON_ONCE(dl_rq->this_bw > old); /* underflow */
	ఒకవేళ (dl_rq->this_bw > old)
		dl_rq->this_bw = 0;
	WARN_ON_ONCE(dl_rq->running_bw > dl_rq->this_bw);
}

స్థిర అంతఃవరుస
శూన్య కూడు_rq_bw(నిర్మాణం sched_dl_entity *dl_se, నిర్మాణం dl_rq *dl_rq)
{
	ఒకవేళ (!dl_entity_is_special(dl_se))
		__add_rq_bw(dl_se->dl_bw, dl_rq);
}

స్థిర అంతఃవరుస
శూన్య sub_rq_bw(నిర్మాణం sched_dl_entity *dl_se, నిర్మాణం dl_rq *dl_rq)
{
	ఒకవేళ (!dl_entity_is_special(dl_se))
		__sub_rq_bw(dl_se->dl_bw, dl_rq);
}

స్థిర అంతఃవరుస
శూన్య కూడు_running_bw(నిర్మాణం sched_dl_entity *dl_se, నిర్మాణం dl_rq *dl_rq)
{
	ఒకవేళ (!dl_entity_is_special(dl_se))
		__add_running_bw(dl_se->dl_bw, dl_rq);
}

స్థిర అంతఃవరుస
శూన్య sub_running_bw(నిర్మాణం sched_dl_entity *dl_se, నిర్మాణం dl_rq *dl_rq)
{
	ఒకవేళ (!dl_entity_is_special(dl_se))
		__sub_running_bw(dl_se->dl_bw, dl_rq);
}

స్థిర శూన్య dl_rq_change_utilization(నిర్మాణం rq *rq, నిర్మాణం sched_dl_entity *dl_se, u64 new_bw)
{
	ఒకవేళ (dl_se->dl_non_contending) {
		sub_running_bw(dl_se, &rq->dl);
		dl_se->dl_non_contending = 0;

		/*
		 * If the timer handler is currently running and the
		 * timer cannot be canceled, inactive_task_timer()
		 * will see that dl_not_contending is not set, and
		 * will not touch the rq's active utilization,
		 * so we are still safe.
		 */
		ఒకవేళ (hrtimer_try_గమ్యం_cancel(&dl_se->inactive_timer) == 1) {
			ఒకవేళ (!dl_server(dl_se))
				put_task_struct(dl_task_యొక్క(dl_se));
		}
	}
	__sub_rq_bw(dl_se->dl_bw, &rq->dl);
	__add_rq_bw(new_bw, &rq->dl);
}

స్థిర __always_inline
శూన్య cancel_dl_timer(నిర్మాణం sched_dl_entity *dl_se, నిర్మాణం hrtimer *timer)
{
	/*
	 * If the timer callback was running (hrtimer_try_to_cancel == -1),
	 * it will eventually call put_task_struct().
	 */
	ఒకవేళ (hrtimer_try_గమ్యం_cancel(timer) == 1 && !dl_server(dl_se))
		put_task_struct(dl_task_యొక్క(dl_se));
}

స్థిర __always_inline
శూన్య cancel_replenish_timer(నిర్మాణం sched_dl_entity *dl_se)
{
	cancel_dl_timer(dl_se, &dl_se->dl_timer);
}

స్థిర __always_inline
శూన్య cancel_inactive_timer(నిర్మాణం sched_dl_entity *dl_se)
{
	cancel_dl_timer(dl_se, &dl_se->inactive_timer);
}

స్థిర శూన్య dl_change_utilization(నిర్మాణం task_struct *p, u64 new_bw)
{
	WARN_ON_ONCE(p->dl.flags & SCHED_FLAG_SUGOV);

	ఒకవేళ (task_on_rq_queued(p))
		తిరిగి;

	dl_rq_change_utilization(task_rq(p), &p->dl, new_bw);
}

స్థిర శూన్య __dl_clear_params(నిర్మాణం sched_dl_entity *dl_se);

/*
 * The utilization of a task cannot be immediately removed from
 * the rq active utilization (running_bw) when the task blocks.
 * Instead, we have to wait for the so called "0-lag time".
 *
 * If a task blocks before the "0-lag time", a timer (the inactive
 * timer) is armed, and running_bw is decreased when the timer
 * fires.
 *
 * If the task wakes up again before the inactive timer fires,
 * the timer is canceled, whereas if the task wakes up after the
 * inactive timer fired (and running_bw has been decreased) the
 * task's utilization has to be added to running_bw again.
 * A flag in the deadline scheduling entity (dl_non_contending)
 * is used to avoid race conditions between the inactive timer handler
 * and task wakeups.
 *
 * The following diagram shows how running_bw is updated. A task is
 * "ACTIVE" when its utilization contributes to running_bw; an
 * "ACTIVE contending" task is in the TASK_RUNNING state, while an
 * "ACTIVE non contending" task is a blocked task for which the "0-lag time"
 * has not passed yet. An "INACTIVE" task is a task for which the "0-lag"
 * time already passed, which does not contribute to running_bw anymore.
 *                              +------------------+
 *             wakeup           |    ACTIVE        |
 *          +------------------>+   contending     |
 *          | add_running_bw    |                  |
 *          |                   +----+------+------+
 *          |                        |      ^
 *          |                dequeue |      |
 * +--------+-------+                |      |
 * |                |   t >= 0-lag   |      | wakeup
 * |    INACTIVE    |<---------------+      |
 * |                | sub_running_bw |      |
 * +--------+-------+                |      |
 *          ^                        |      |
 *          |              t < 0-lag |      |
 *          |                        |      |
 *          |                        V      |
 *          |                   +----+------+------+
 *          | sub_running_bw    |    ACTIVE        |
 *          +-------------------+                  |
 *            inactive timer    |  non contending  |
 *            fired             +------------------+
 *
 * The task_non_contending() function is invoked when a task
 * blocks, and checks if the 0-lag time already passed or
 * not (in the first case, it directly updates running_bw;
 * in the second case, it arms the inactive timer).
 *
 * The task_contending() function is invoked when a task wakes
 * up, and checks if the task is still in the "ACTIVE non contending"
 * state or not (in the second case, it updates running_bw).
 */
స్థిర శూన్య task_non_contending(నిర్మాణం sched_dl_entity *dl_se, bool dl_task)
{
	నిర్మాణం hrtimer *timer = &dl_se->inactive_timer;
	నిర్మాణం rq *rq = rq_యొక్క_dl_se(dl_se);
	నిర్మాణం dl_rq *dl_rq = &rq->dl;
	s64 zerolag_సమయం;

	/*
	 * If this is a non-deadline task that has been boosted,
	 * do nothing
	 */
	ఒకవేళ (dl_se->dl_runtime == 0)
		తిరిగి;

	ఒకవేళ (dl_entity_is_special(dl_se))
		తిరిగి;

	WARN_ON(dl_se->dl_non_contending);

	zerolag_సమయం = dl_se->deadline -
		 div64_long((dl_se->runtime * dl_se->dl_period),
			dl_se->dl_runtime);

	/*
	 * Using relative times instead of the absolute "0-lag time"
	 * allows to simplify the code
	 */
	zerolag_సమయం -= rq_clock(rq);

	/*
	 * If the "0-lag time" already passed, decrease the active
	 * utilization now, instead of starting a timer
	 */
	ఒకవేళ ((zerolag_సమయం < 0) || hrtimer_చురుకైన(&dl_se->inactive_timer)) {
		ఒకవేళ (dl_server(dl_se)) {
			sub_running_bw(dl_se, dl_rq);
		} లేకపోతే {
			నిర్మాణం task_struct *p = dl_task_యొక్క(dl_se);

			ఒకవేళ (dl_task)
				sub_running_bw(dl_se, dl_rq);

			ఒకవేళ (!dl_task || READ_ONCE(p->__state) == TASK_DEAD) {
				నిర్మాణం dl_bw *dl_b = dl_bw_యొక్క(task_cpu(p));

				ఒకవేళ (READ_ONCE(p->__state) == TASK_DEAD)
					sub_rq_bw(dl_se, &rq->dl);
				raw_spin_తాళం(&dl_b->తాళం);
				__dl_sub(dl_b, dl_se->dl_bw, dl_bw_cpus(task_cpu(p)));
				raw_spin_unlock(&dl_b->తాళం);
				__dl_clear_params(dl_se);
			}
		}

		తిరిగి;
	}

	dl_se->dl_non_contending = 1;
	ఒకవేళ (!dl_server(dl_se))
		get_task_struct(dl_task_యొక్క(dl_se));

	hrtimer_ప్రారంభం(timer, ns_గమ్యం_ktime(zerolag_సమయం), HRTIMER_బహుళకం_REL_HARD);
}

స్థిర శూన్య task_contending(నిర్మాణం sched_dl_entity *dl_se, పూర్ణాంకం flags)
{
	నిర్మాణం dl_rq *dl_rq = dl_rq_యొక్క_se(dl_se);

	/*
	 * If this is a non-deadline task that has been boosted,
	 * do nothing
	 */
	ఒకవేళ (dl_se->dl_runtime == 0)
		తిరిగి;

	ఒకవేళ (flags & వరుసలో_చేర్చు_MIGRATED)
		కూడు_rq_bw(dl_se, dl_rq);

	ఒకవేళ (dl_se->dl_non_contending) {
		dl_se->dl_non_contending = 0;
		/*
		 * If the timer handler is currently running and the
		 * timer cannot be canceled, inactive_task_timer()
		 * will see that dl_not_contending is not set, and
		 * will not touch the rq's active utilization,
		 * so we are still safe.
		 */
		cancel_inactive_timer(dl_se);
	} లేకపోతే {
		/*
		 * Since "dl_non_contending" is not set, the
		 * task's utilization has already been removed from
		 * active utilization (either when the task blocked,
		 * when the "inactive timer" fired).
		 * So, add it back.
		 */
		కూడు_running_bw(dl_se, dl_rq);
	}
}

స్థిర అంతఃవరుస పూర్ణాంకం is_leftmost(నిర్మాణం sched_dl_entity *dl_se, నిర్మాణం dl_rq *dl_rq)
{
	తిరిగి rb_మొదటి_cached(&dl_rq->మూలం) == &dl_se->rb_కణుపు;
}

స్థిర శూన్య init_dl_rq_bw_నిష్పత్తి(నిర్మాణం dl_rq *dl_rq);

శూన్య init_dl_bw(నిర్మాణం dl_bw *dl_b)
{
	raw_spin_తాళం_init(&dl_b->తాళం);
	ఒకవేళ (global_rt_runtime() == RUNTIME_INF)
		dl_b->bw = -1;
	లేకపోతే
		dl_b->bw = గమ్యం_నిష్పత్తి(global_rt_period(), global_rt_runtime());
	dl_b->మొత్తం_bw = 0;
}

శూన్య init_dl_rq(నిర్మాణం dl_rq *dl_rq)
{
	dl_rq->మూలం = RB_మూలం_CACHED;

	/* zero means no -deadline tasks */
	dl_rq->earliest_dl.curr = dl_rq->earliest_dl.తదుపరి = 0;

	dl_rq->overloaded = 0;
	dl_rq->pushable_dl_tasks_మూలం = RB_మూలం_CACHED;

	dl_rq->running_bw = 0;
	dl_rq->this_bw = 0;
	init_dl_rq_bw_నిష్పత్తి(dl_rq);
}

స్థిర అంతఃవరుస పూర్ణాంకం dl_overloaded(నిర్మాణం rq *rq)
{
	తిరిగి atomic_read(&rq->rd->dlo_లెక్క);
}

స్థిర అంతఃవరుస శూన్య dl_సమితి_overload(నిర్మాణం rq *rq)
{
	ఒకవేళ (!rq->online)
		తిరిగి;

	cpumask_సమితి_cpu(rq->cpu, rq->rd->dlo_ముసుగు);
	/*
	 * Must be visible before the overload count is
	 * set (as in sched_rt.c).
	 *
	 * Matched by the barrier in pull_dl_task().
	 */
	smp_wmb();
	atomic_inc(&rq->rd->dlo_లెక్క);
}

స్థిర అంతఃవరుస శూన్య dl_clear_overload(నిర్మాణం rq *rq)
{
	ఒకవేళ (!rq->online)
		తిరిగి;

	atomic_dec(&rq->rd->dlo_లెక్క);
	cpumask_clear_cpu(rq->cpu, rq->rd->dlo_ముసుగు);
}

#నిర్వచించు __node_2_pdl(కణుపు) \
	rb_నమోదు((కణుపు), నిర్మాణం task_struct, pushable_dl_tasks)

స్థిర అంతఃవరుస bool __pushable_less(నిర్మాణం rb_కణుపు *a, స్థిరరాశి నిర్మాణం rb_కణుపు *b)
{
	తిరిగి dl_entity_preempt(&__node_2_pdl(a)->dl, &__node_2_pdl(b)->dl);
}

స్థిర అంతఃవరుస పూర్ణాంకం has_pushable_dl_tasks(నిర్మాణం rq *rq)
{
	తిరిగి !RB_ఖాళీలు_మూలం(&rq->dl.pushable_dl_tasks_మూలం.rb_మూలం);
}

/*
 * The list of pushable -deadline task is not a plist, like in
 * sched_rt.c, it is an rb-tree with tasks ordered by deadline.
 */
స్థిర శూన్య వరుసలో_చేర్చు_pushable_dl_task(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
	నిర్మాణం rb_కణుపు *leftmost;

	WARN_ON_ONCE(!RB_ఖాళీలు_కణుపు(&p->pushable_dl_tasks));

	leftmost = rb_కూడు_cached(&p->pushable_dl_tasks,
				 &rq->dl.pushable_dl_tasks_మూలం,
				 __pushable_less);
	ఒకవేళ (leftmost)
		rq->dl.earliest_dl.తదుపరి = p->dl.deadline;

	ఒకవేళ (!rq->dl.overloaded) {
		dl_సమితి_overload(rq);
		rq->dl.overloaded = 1;
	}
}

స్థిర శూన్య వరుస_నుండి_తీయి_pushable_dl_task(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
	నిర్మాణం dl_rq *dl_rq = &rq->dl;
	నిర్మాణం rb_మూలం_cached *మూలం = &dl_rq->pushable_dl_tasks_మూలం;
	నిర్మాణం rb_కణుపు *leftmost;

	ఒకవేళ (RB_ఖాళీలు_కణుపు(&p->pushable_dl_tasks))
		తిరిగి;

	leftmost = rb_erase_cached(&p->pushable_dl_tasks, మూలం);
	ఒకవేళ (leftmost)
		dl_rq->earliest_dl.తదుపరి = __node_2_pdl(leftmost)->dl.deadline;

	RB_CLEAR_కణుపు(&p->pushable_dl_tasks);

	ఒకవేళ (!has_pushable_dl_tasks(rq) && rq->dl.overloaded) {
		dl_clear_overload(rq);
		rq->dl.overloaded = 0;
	}
}

స్థిర పూర్ణాంకం నెట్టు_dl_task(నిర్మాణం rq *rq);

స్థిర అంతఃవరుస bool need_pull_dl_task(నిర్మాణం rq *rq, నిర్మాణం task_struct *prev)
{
	తిరిగి rq->online && dl_task(prev);
}

స్థిర DEFINE_చొప్పున_CPU(నిర్మాణం balance_callback, dl_నెట్టు_తల);
స్థిర DEFINE_చొప్పున_CPU(నిర్మాణం balance_callback, dl_pull_తల);

స్థిర శూన్య నెట్టు_dl_tasks(నిర్మాణం rq *);
స్థిర శూన్య pull_dl_task(నిర్మాణం rq *);

స్థిర అంతఃవరుస శూన్య deadline_వరుస_నెట్టు_tasks(నిర్మాణం rq *rq)
{
	ఒకవేళ (!has_pushable_dl_tasks(rq))
		తిరిగి;

	వరుస_balance_callback(rq, &చొప్పున_cpu(dl_నెట్టు_తల, rq->cpu), నెట్టు_dl_tasks);
}

స్థిర అంతఃవరుస శూన్య deadline_వరుస_pull_task(నిర్మాణం rq *rq)
{
	వరుస_balance_callback(rq, &చొప్పున_cpu(dl_pull_తల, rq->cpu), pull_dl_task);
}

స్థిర నిర్మాణం rq *కనుగొను_తాళం_later_rq(నిర్మాణం task_struct *task, నిర్మాణం rq *rq);

స్థిర నిర్మాణం rq *dl_task_offline_migration(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
	నిర్మాణం rq *later_rq = ఖాళీ;
	నిర్మాణం dl_bw *dl_b;

	later_rq = కనుగొను_తాళం_later_rq(p, rq);
	ఒకవేళ (!later_rq) {
		పూర్ణాంకం cpu;

		/*
		 * If we cannot preempt any rq, fall back to pick any
		 * online CPU:
		 */
		cpu = cpumask_any_and(cpu_చురుకైన_ముసుగు, p->cpus_ptr);
		ఒకవేళ (cpu >= nr_cpu_ids) {
			/*
			 * Failed to find any suitable CPU.
			 * The task will never come back!
			 */
			WARN_ON_ONCE(dl_bandwidth_enabled());

			/*
			 * If admission control is disabled we
			 * try a little harder to let the task
			 * run.
			 */
			cpu = cpumask_any(cpu_చురుకైన_ముసుగు);
		}
		later_rq = cpu_rq(cpu);
		double_తాళం_balance(rq, later_rq);
	}

	ఒకవేళ (p->dl.dl_non_contending || p->dl.dl_throttled) {
		/*
		 * Inactive timer is armed (or callback is running, but
		 * waiting for us to release rq locks). In any case, when it
		 * will fire (or continue), it will see running_bw of this
		 * task migrated to later_rq (and correctly handle it).
		 */
		sub_running_bw(&p->dl, &rq->dl);
		sub_rq_bw(&p->dl, &rq->dl);

		కూడు_rq_bw(&p->dl, &later_rq->dl);
		కూడు_running_bw(&p->dl, &later_rq->dl);
	} లేకపోతే {
		sub_rq_bw(&p->dl, &rq->dl);
		కూడు_rq_bw(&p->dl, &later_rq->dl);
	}

	/*
	 * And we finally need to fix up root_domain(s) bandwidth accounting,
	 * since p is still hanging out in the old (now moved to default) root
	 * domain.
	 */
	dl_b = &rq->rd->dl_bw;
	raw_spin_తాళం(&dl_b->తాళం);
	__dl_sub(dl_b, p->dl.dl_bw, cpumask_బరువు(rq->rd->పరిధి));
	raw_spin_unlock(&dl_b->తాళం);

	dl_b = &later_rq->rd->dl_bw;
	raw_spin_తాళం(&dl_b->తాళం);
	__dl_add(dl_b, p->dl.dl_bw, cpumask_బరువు(later_rq->rd->పరిధి));
	raw_spin_unlock(&dl_b->తాళం);

	సమితి_task_cpu(p, later_rq->cpu);
	double_unlock_balance(later_rq, rq);

	తిరిగి later_rq;
}

స్థిర శూన్య
వరుసలో_చేర్చు_dl_entity(నిర్మాణం sched_dl_entity *dl_se, పూర్ణాంకం flags);
స్థిర శూన్య వరుసలో_చేర్చు_task_dl(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, పూర్ణాంకం flags);
స్థిర శూన్య వరుస_నుండి_తీయి_dl_entity(నిర్మాణం sched_dl_entity *dl_se, పూర్ణాంకం flags);
స్థిర శూన్య wakeup_preempt_dl(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, పూర్ణాంకం flags);

స్థిర అంతఃవరుస శూన్య replenish_dl_new_period(నిర్మాణం sched_dl_entity *dl_se,
					    నిర్మాణం rq *rq)
{
	/* for non-boosted task, pi_of(dl_se) == dl_se */
	dl_se->deadline = rq_clock(rq) + pi_యొక్క(dl_se)->dl_deadline;
	dl_se->runtime = pi_యొక్క(dl_se)->dl_runtime;

	/*
	 * If it is a deferred reservation, and the server
	 * is not handling an starvation case, defer it.
	 */
	ఒకవేళ (dl_se->dl_defer && !dl_se->dl_defer_running) {
		dl_se->dl_throttled = 1;
		dl_se->dl_defer_armed = 1;
	}
	trace_sched_dl_replenish_tp(dl_se, cpu_యొక్క(rq), dl_get_type(dl_se, rq));
}

/*
 * We are being explicitly informed that a new instance is starting,
 * and this means that:
 *  - the absolute deadline of the entity has to be placed at
 *    current time + relative deadline;
 *  - the runtime of the entity has to be set to the maximum value.
 *
 * The capability of specifying such event is useful whenever a -deadline
 * entity wants to (try to!) synchronize its behaviour with the scheduler's
 * one, and to (try to!) reconcile itself with its own scheduling
 * parameters.
 */
స్థిర అంతఃవరుస శూన్య అమరిక_new_dl_entity(నిర్మాణం sched_dl_entity *dl_se)
{
	నిర్మాణం dl_rq *dl_rq = dl_rq_యొక్క_se(dl_se);
	నిర్మాణం rq *rq = rq_యొక్క_dl_rq(dl_rq);

	WARN_ON(is_dl_boosted(dl_se));
	WARN_ON(dl_సమయం_before(rq_clock(rq), dl_se->deadline));

	/*
	 * We are racing with the deadline timer. So, do nothing because
	 * the deadline timer handler will take care of properly recharging
	 * the runtime and postponing the deadline
	 */
	ఒకవేళ (dl_se->dl_throttled)
		తిరిగి;

	/*
	 * We use the regular wall clock time to set deadlines in the
	 * future; in fact, we must consider execution overheads (time
	 * spent on hardirq context, etc.).
	 */
	replenish_dl_new_period(dl_se, rq);
}

స్థిర పూర్ణాంకం ప్రారంభం_dl_timer(నిర్మాణం sched_dl_entity *dl_se);
స్థిర bool dl_entity_అధికప్రవాహం(నిర్మాణం sched_dl_entity *dl_se, u64 t);

/*
 * Pure Earliest Deadline First (EDF) scheduling does not deal with the
 * possibility of a entity lasting more than what it declared, and thus
 * exhausting its runtime.
 *
 * Here we are interested in making runtime overrun possible, but we do
 * not want a entity which is misbehaving to affect the scheduling of all
 * other entities.
 * Therefore, a budgeting strategy called Constant Bandwidth Server (CBS)
 * is used, in order to confine each entity within its own bandwidth.
 *
 * This function deals exactly with that, and ensures that when the runtime
 * of a entity is replenished, its deadline is also postponed. That ensures
 * the overrunning entity can't interfere with other entity in the system and
 * can't make them miss their deadlines. Reasons why this kind of overruns
 * could happen are, typically, a entity voluntarily trying to overcome its
 * runtime, or it just underestimated it during sched_setattr().
 */
స్థిర శూన్య replenish_dl_entity(నిర్మాణం sched_dl_entity *dl_se)
{
	నిర్మాణం dl_rq *dl_rq = dl_rq_యొక్క_se(dl_se);
	నిర్మాణం rq *rq = rq_యొక్క_dl_rq(dl_rq);

	WARN_ON_ONCE(pi_యొక్క(dl_se)->dl_runtime <= 0);

	/*
	 * This could be the case for a !-dl task that is boosted.
	 * Just go with full inherited parameters.
	 *
	 * Or, it could be the case of a deferred reservation that
	 * was not able to consume its runtime in background and
	 * reached this point with current u > U.
	 *
	 * In both cases, set a new period.
	 */
	ఒకవేళ (dl_se->dl_deadline == 0 ||
	    (dl_se->dl_defer_armed && dl_entity_అధికప్రవాహం(dl_se, rq_clock(rq)))) {
		dl_se->deadline = rq_clock(rq) + pi_యొక్క(dl_se)->dl_deadline;
		dl_se->runtime = pi_యొక్క(dl_se)->dl_runtime;
	}

	ఒకవేళ (dl_se->dl_yielded && dl_se->runtime > 0)
		dl_se->runtime = 0;

	/*
	 * We keep moving the deadline away until we get some
	 * available runtime for the entity. This ensures correct
	 * handling of situations where the runtime overrun is
	 * arbitrary large.
	 */
	అయితే (dl_se->runtime <= 0) {
		dl_se->deadline += pi_యొక్క(dl_se)->dl_period;
		dl_se->runtime += pi_యొక్క(dl_se)->dl_runtime;
	}

	/*
	 * At this point, the deadline really should be "in
	 * the future" with respect to rq->clock. If it's
	 * not, we are, for some reason, lagging too much!
	 * Anyway, after having warn userspace abut that,
	 * we still try to keep the things running by
	 * resetting the deadline and the budget of the
	 * entity.
	 */
	ఒకవేళ (dl_సమయం_before(dl_se->deadline, rq_clock(rq))) {
		printk_deferred_once("sched: DL replenish lagged too much\n");
		replenish_dl_new_period(dl_se, rq);
	}

	ఒకవేళ (dl_se->dl_yielded)
		dl_se->dl_yielded = 0;
	ఒకవేళ (dl_se->dl_throttled)
		dl_se->dl_throttled = 0;

	trace_sched_dl_replenish_tp(dl_se, cpu_యొక్క(rq), dl_get_type(dl_se, rq));

	/*
	 * If this is the replenishment of a deferred reservation,
	 * clear the flag and return.
	 */
	ఒకవేళ (dl_se->dl_defer_armed) {
		dl_se->dl_defer_armed = 0;
		తిరిగి;
	}

	/*
	 * A this point, if the deferred server is not armed, and the deadline
	 * is in the future, if it is not running already, throttle the server
	 * and arm the defer timer.
	 */
	ఒకవేళ (dl_se->dl_defer && !dl_se->dl_defer_running &&
	    dl_సమయం_before(rq_clock(dl_se->rq), dl_se->deadline - dl_se->runtime)) {
		ఒకవేళ (!is_dl_boosted(dl_se)) {

			/*
			 * Set dl_se->dl_defer_armed and dl_throttled variables to
			 * inform the start_dl_timer() that this is a deferred
			 * activation.
			 */
			dl_se->dl_defer_armed = 1;
			dl_se->dl_throttled = 1;
			ఒకవేళ (!ప్రారంభం_dl_timer(dl_se)) {
				/*
				 * If for whatever reason (delays), a previous timer was
				 * queued but not serviced, cancel it and clean the
				 * deferrable server variables intended for start_dl_timer().
				 */
				hrtimer_try_గమ్యం_cancel(&dl_se->dl_timer);
				dl_se->dl_defer_armed = 0;
				dl_se->dl_throttled = 0;
			}
		}
	}
}

/*
 * Here we check if --at time t-- an entity (which is probably being
 * [re]activated or, in general, enqueued) can use its remaining runtime
 * and its current deadline _without_ exceeding the bandwidth it is
 * assigned (function returns true if it can't). We are in fact applying
 * one of the CBS rules: when a task wakes up, if the residual runtime
 * over residual deadline fits within the allocated bandwidth, then we
 * can keep the current (absolute) deadline and residual budget without
 * disrupting the schedulability of the system. Otherwise, we should
 * refill the runtime and set the deadline a period in the future,
 * because keeping the current (absolute) deadline of the task would
 * result in breaking guarantees promised to other tasks (refer to
 * Documentation/scheduler/sched-deadline.rst for more information).
 *
 * This function returns true if:
 *
 *   runtime / (deadline - t) > dl_runtime / dl_deadline ,
 *
 * IOW we can't recycle current parameters.
 *
 * Notice that the bandwidth check is done against the deadline. For
 * task with deadline equal to period this is the same of using
 * dl_period instead of dl_deadline in the equation above.
 */
స్థిర bool dl_entity_అధికప్రవాహం(నిర్మాణం sched_dl_entity *dl_se, u64 t)
{
	u64 ఎడమ, కుడి;

	/*
	 * left and right are the two sides of the equation above,
	 * after a bit of shuffling to use multiplications instead
	 * of divisions.
	 *
	 * Note that none of the time values involved in the two
	 * multiplications are absolute: dl_deadline and dl_runtime
	 * are the relative deadline and the maximum runtime of each
	 * instance, runtime is the runtime left for the last instance
	 * and (deadline - t), since t is rq->clock, is the time left
	 * to the (absolute) deadline. Even if overflowing the u64 type
	 * is very unlikely to occur in both cases, here we scale down
	 * as we want to avoid that risk at all. Scaling down by 10
	 * means that we reduce granularity to 1us. We are fine with it,
	 * since this is only a true/false check and, anyway, thinking
	 * of anything below microseconds resolution is actually fiction
	 * (but still we want to give the user that illusion >;).
	 */
	ఎడమ = (pi_యొక్క(dl_se)->dl_deadline >> DL_SCALE) * (dl_se->runtime >> DL_SCALE);
	కుడి = ((dl_se->deadline - t) >> DL_SCALE) *
		(pi_యొక్క(dl_se)->dl_runtime >> DL_SCALE);

	తిరిగి dl_సమయం_before(కుడి, ఎడమ);
}

/*
 * Revised wakeup rule [1]: For self-suspending tasks, rather then
 * re-initializing task's runtime and deadline, the revised wakeup
 * rule adjusts the task's runtime to avoid the task to overrun its
 * density.
 *
 * Reasoning: a task may overrun the density if:
 *    runtime / (deadline - t) > dl_runtime / dl_deadline
 *
 * Therefore, runtime can be adjusted to:
 *     runtime = (dl_runtime / dl_deadline) * (deadline - t)
 *
 * In such way that runtime will be equal to the maximum density
 * the task can use without breaking any rule.
 *
 * [1] Luca Abeni, Giuseppe Lipari, and Juri Lelli. 2015. Constant
 * bandwidth server revisited. SIGBED Rev. 11, 4 (January 2015), 19-24.
 */
స్థిర శూన్య
update_dl_revised_wakeup(నిర్మాణం sched_dl_entity *dl_se, నిర్మాణం rq *rq)
{
	u64 laxity = dl_se->deadline - rq_clock(rq);

	/*
	 * If the task has deadline < period, and the deadline is in the past,
	 * it should already be throttled before this check.
	 *
	 * See update_dl_entity() comments for further details.
	 */
	WARN_ON(dl_సమయం_before(dl_se->deadline, rq_clock(rq)));

	dl_se->runtime = (dl_se->dl_density * laxity) >> BW_జరుపు;
}

/*
 * When a deadline entity is placed in the runqueue, its runtime and deadline
 * might need to be updated. This is done by a CBS wake up rule. There are two
 * different rules: 1) the original CBS; and 2) the Revisited CBS.
 *
 * When the task is starting a new period, the Original CBS is used. In this
 * case, the runtime is replenished and a new absolute deadline is set.
 *
 * When a task is queued before the begin of the next period, using the
 * remaining runtime and deadline could make the entity to overflow, see
 * dl_entity_overflow() to find more about runtime overflow. When such case
 * is detected, the runtime and deadline need to be updated.
 *
 * If the task has an implicit deadline, i.e., deadline == period, the Original
 * CBS is applied. The runtime is replenished and a new absolute deadline is
 * set, as in the previous cases.
 *
 * However, the Original CBS does not work properly for tasks with
 * deadline < period, which are said to have a constrained deadline. By
 * applying the Original CBS, a constrained deadline task would be able to run
 * runtime/deadline in a period. With deadline < period, the task would
 * overrun the runtime/period allowed bandwidth, breaking the admission test.
 *
 * In order to prevent this misbehave, the Revisited CBS is used for
 * constrained deadline tasks when a runtime overflow is detected. In the
 * Revisited CBS, rather than replenishing & setting a new absolute deadline,
 * the remaining runtime of the task is reduced to avoid runtime overflow.
 * Please refer to the comments update_dl_revised_wakeup() function to find
 * more about the Revised CBS rule.
 */
స్థిర శూన్య update_dl_entity(నిర్మాణం sched_dl_entity *dl_se)
{
	నిర్మాణం rq *rq = rq_యొక్క_dl_se(dl_se);

	ఒకవేళ (dl_సమయం_before(dl_se->deadline, rq_clock(rq)) ||
	    dl_entity_అధికప్రవాహం(dl_se, rq_clock(rq))) {

		ఒకవేళ (unlikely((!dl_is_implicit(dl_se) ||
			      (dl_se->dl_defer && dl_se->dl_defer_running)) &&
			     !dl_సమయం_before(dl_se->deadline, rq_clock(rq)) &&
			     !is_dl_boosted(dl_se))) {
			update_dl_revised_wakeup(dl_se, rq);
			తిరిగి;
		}

		/*
		 * When [4] D->A is followed by [1] A->B, dl_defer_running
		 * needs to be cleared, otherwise it will fail to properly
		 * start the zero-laxity timer.
		 */
		dl_se->dl_defer_running = 0;
		replenish_dl_new_period(dl_se, rq);
	} లేకపోతే ఒకవేళ (dl_server(dl_se) && dl_se->dl_defer) {
		/*
		 * The server can still use its previous deadline, so check if
		 * it left the dl_defer_running state.
		 */
		ఒకవేళ (!dl_se->dl_defer_running) {
			dl_se->dl_defer_armed = 1;
			dl_se->dl_throttled = 1;
		}
	}
}

స్థిర అంతఃవరుస u64 dl_తదుపరి_period(నిర్మాణం sched_dl_entity *dl_se)
{
	తిరిగి dl_se->deadline - dl_se->dl_deadline + dl_se->dl_period;
}

/*
 * If the entity depleted all its runtime, and if we want it to sleep
 * while waiting for some new execution time to become available, we
 * set the bandwidth replenishment timer to the replenishment instant
 * and try to activate it.
 *
 * Notice that it is important for the caller to know if the timer
 * actually started or not (i.e., the replenishment instant is in
 * the future or in the past).
 */
స్థిర పూర్ణాంకం ప్రారంభం_dl_timer(నిర్మాణం sched_dl_entity *dl_se)
{
	నిర్మాణం hrtimer *timer = &dl_se->dl_timer;
	నిర్మాణం dl_rq *dl_rq = dl_rq_యొక్క_se(dl_se);
	నిర్మాణం rq *rq = rq_యొక్క_dl_rq(dl_rq);
	ktime_t now, act;
	s64 delta;

	lockdep_assert_rq_held(rq);

	/*
	 * We want the timer to fire at the deadline, but considering
	 * that it is actually coming from rq->clock and not from
	 * hrtimer's time base reading.
	 *
	 * The deferred reservation will have its timer set to
	 * (deadline - runtime). At that point, the CBS rule will decide
	 * if the current deadline can be used, or if a replenishment is
	 * required to avoid add too much pressure on the system
	 * (current u > U).
	 */
	ఒకవేళ (dl_se->dl_defer_armed) {
		WARN_ON_ONCE(!dl_se->dl_throttled);
		act = ns_గమ్యం_ktime(dl_se->deadline - dl_se->runtime);
	} లేకపోతే {
		/* act = deadline - rel-deadline + period */
		act = ns_గమ్యం_ktime(dl_తదుపరి_period(dl_se));
	}

	now = ktime_get();
	delta = ktime_గమ్యం_ns(now) - rq_clock(rq);
	act = ktime_కూడు_ns(act, delta);

	/*
	 * If the expiry time already passed, e.g., because the value
	 * chosen as the deadline is too small, don't even try to
	 * start the timer in the past!
	 */
	ఒకవేళ (ktime_before(act, now))
		తిరిగి 0;

	/*
	 * !enqueued will guarantee another callback; even if one is already in
	 * progress. This ensures a balanced {get,put}_task_struct().
	 *
	 * The race against __run_timer() clearing the enqueued state is
	 * harmless because we're holding task_rq()->lock, therefore the timer
	 * expiring after we've done the check will wait on its task_rq_lock()
	 * and observe our state.
	 */
	ఒకవేళ (!hrtimer_is_queued(timer)) {
		ఒకవేళ (!dl_server(dl_se))
			get_task_struct(dl_task_యొక్క(dl_se));
		hrtimer_ప్రారంభం(timer, act, HRTIMER_బహుళకం_ABS_HARD);
	}

	తిరిగి 1;
}

స్థిర శూన్య __push_dl_task(నిర్మాణం rq *rq, నిర్మాణం rq_flags *rf)
{
	/*
	 * Queueing this task back might have overloaded rq, check if we need
	 * to kick someone away.
	 */
	ఒకవేళ (has_pushable_dl_tasks(rq)) {
		/*
		 * Nothing relies on rq->lock after this, so its safe to drop
		 * rq->lock.
		 */
		rq_unpin_తాళం(rq, rf);
		నెట్టు_dl_task(rq);
		rq_repin_తాళం(rq, rf);
	}
}

/* a defer timer will not be reset if the runtime consumed was < dl_server_min_res */
స్థిర స్థిరరాశి u64 dl_server_min_res = 1 * NSEC_చొప్పున_MSEC;

స్థిర గణన hrtimer_restart dl_server_timer(నిర్మాణం hrtimer *timer, నిర్మాణం sched_dl_entity *dl_se)
{
	నిర్మాణం rq *rq = rq_యొక్క_dl_se(dl_se);
	u64 fw;

	scoped_guard (rq_తాళం, rq) {
		నిర్మాణం rq_flags *rf = &scope.rf;

		ఒకవేళ (!dl_se->dl_throttled || !dl_se->dl_runtime)
			తిరిగి HRTIMER_NORESTART;

		sched_clock_tick();
		update_rq_clock(rq);

		/*
		 * Make sure current has propagated its pending runtime into
		 * any relevant server through calling dl_server_update() and
		 * friends.
		 */
		rq->donor->sched_class->update_curr(rq);

		ఒకవేళ (dl_se->dl_defer_idle) {
			dl_server_stop(dl_se);
			తిరిగి HRTIMER_NORESTART;
		}

		ఒకవేళ (dl_se->dl_defer_armed) {
			/*
			 * First check if the server could consume runtime in background.
			 * If so, it is possible to push the defer timer for this amount
			 * of time. The dl_server_min_res serves as a limit to avoid
			 * forwarding the timer for a too small amount of time.
			 */
			ఒకవేళ (dl_సమయం_before(rq_clock(dl_se->rq),
					   (dl_se->deadline - dl_se->runtime - dl_server_min_res))) {

				/* reset the defer timer */
				fw = dl_se->deadline - rq_clock(dl_se->rq) - dl_se->runtime;

				hrtimer_forward_now(timer, ns_గమ్యం_ktime(fw));
				తిరిగి HRTIMER_RESTART;
			}

			dl_se->dl_defer_running = 1;
		}

		వరుసలో_చేర్చు_dl_entity(dl_se, వరుసలో_చేర్చు_REPLENISH);

		ఒకవేళ (!dl_task(dl_se->rq->curr) || dl_entity_preempt(dl_se, &dl_se->rq->curr->dl))
			resched_curr(rq);

		__push_dl_task(rq, rf);
	}

	తిరిగి HRTIMER_NORESTART;
}

/*
 * This is the bandwidth enforcement timer callback. If here, we know
 * a task is not on its dl_rq, since the fact that the timer was running
 * means the task is throttled and needs a runtime replenishment.
 *
 * However, what we actually do depends on the fact the task is active,
 * (it is on its rq) or has been removed from there by a call to
 * dequeue_task_dl(). In the former case we must issue the runtime
 * replenishment and add the task back to the dl_rq; in the latter, we just
 * do nothing but clearing dl_throttled, so that runtime and deadline
 * updating (and the queueing back to dl_rq) will be done by the
 * next call to enqueue_task_dl().
 */
స్థిర గణన hrtimer_restart dl_task_timer(నిర్మాణం hrtimer *timer)
{
	నిర్మాణం sched_dl_entity *dl_se = container_యొక్క(timer,
						     నిర్మాణం sched_dl_entity,
						     dl_timer);
	నిర్మాణం task_struct *p;
	నిర్మాణం rq_flags rf;
	నిర్మాణం rq *rq;

	ఒకవేళ (dl_server(dl_se))
		తిరిగి dl_server_timer(timer, dl_se);

	p = dl_task_యొక్క(dl_se);
	rq = task_rq_తాళం_2(p, &rf);

	/*
	 * The task might have changed its scheduling policy to something
	 * different than SCHED_DEADLINE (through switched_from_dl()).
	 */
	ఒకవేళ (!dl_task(p))
		వెళ్ళు unlock;

	/*
	 * The task might have been boosted by someone else and might be in the
	 * boosting/deboosting path, its not throttled.
	 */
	ఒకవేళ (is_dl_boosted(dl_se))
		వెళ్ళు unlock;

	/*
	 * Spurious timer due to start_dl_timer() race; or we already received
	 * a replenishment from rt_mutex_setprio().
	 */
	ఒకవేళ (!dl_se->dl_throttled)
		వెళ్ళు unlock;

	sched_clock_tick();
	update_rq_clock(rq);

	/*
	 * If the throttle happened during sched-out; like:
	 *
	 *   schedule()
	 *     deactivate_task()
	 *       dequeue_task_dl()
	 *         update_curr_dl()
	 *           start_dl_timer()
	 *         __dequeue_task_dl()
	 *     prev->on_rq = 0;
	 *
	 * We can be both throttled and !queued. Replenish the counter
	 * but do not enqueue -- wait for our wakeup to do that.
	 */
	ఒకవేళ (!task_on_rq_queued(p)) {
		replenish_dl_entity(dl_se);
		వెళ్ళు unlock;
	}

	ఒకవేళ (unlikely(!rq->online)) {
		/*
		 * If the runqueue is no longer available, migrate the
		 * task elsewhere. This necessarily changes rq.
		 */
		lockdep_unpin_తాళం(__rq_lockp(rq), rf.cookie);
		rq = dl_task_offline_migration(rq, p);
		rf.cookie = lockdep_pin_తాళం(__rq_lockp(rq));
		update_rq_clock(rq);

		/*
		 * Now that the task has been migrated to the new RQ and we
		 * have that locked, proceed as normal and enqueue the task
		 * there.
		 */
	}

	వరుసలో_చేర్చు_task_dl(rq, p, వరుసలో_చేర్చు_REPLENISH);
	ఒకవేళ (dl_task(rq->donor))
		wakeup_preempt_dl(rq, p, 0);
	లేకపోతే
		resched_curr(rq);

	__push_dl_task(rq, &rf);

unlock:
	task_rq_unlock(rq, p, &rf);

	/*
	 * This can free the task_struct, including this hrtimer, do not touch
	 * anything related to that after this.
	 */
	put_task_struct(p);

	తిరిగి HRTIMER_NORESTART;
}

స్థిర శూన్య init_dl_task_timer(నిర్మాణం sched_dl_entity *dl_se)
{
	నిర్మాణం hrtimer *timer = &dl_se->dl_timer;

	hrtimer_అమరిక(timer, dl_task_timer, CLOCK_MONOTONIC, HRTIMER_బహుళకం_REL_HARD);
}

/*
 * During the activation, CBS checks if it can reuse the current task's
 * runtime and period. If the deadline of the task is in the past, CBS
 * cannot use the runtime, and so it replenishes the task. This rule
 * works fine for implicit deadline tasks (deadline == period), and the
 * CBS was designed for implicit deadline tasks. However, a task with
 * constrained deadline (deadline < period) might be awakened after the
 * deadline, but before the next period. In this case, replenishing the
 * task would allow it to run for runtime / deadline. As in this case
 * deadline < period, CBS enables a task to run for more than the
 * runtime / period. In a very loaded system, this can cause a domino
 * effect, making other tasks miss their deadlines.
 *
 * To avoid this problem, in the activation of a constrained deadline
 * task after the deadline but before the next period, throttle the
 * task and set the replenishing timer to the begin of the next period,
 * unless it is boosted.
 */
స్థిర అంతఃవరుస శూన్య dl_check_constrained_dl(నిర్మాణం sched_dl_entity *dl_se)
{
	నిర్మాణం rq *rq = rq_యొక్క_dl_se(dl_se);

	ఒకవేళ (dl_సమయం_before(dl_se->deadline, rq_clock(rq)) &&
	    dl_సమయం_before(rq_clock(rq), dl_తదుపరి_period(dl_se))) {
		ఒకవేళ (unlikely(is_dl_boosted(dl_se) || !ప్రారంభం_dl_timer(dl_se)))
			తిరిగి;
		trace_sched_dl_throttle_tp(dl_se, cpu_యొక్క(rq), dl_get_type(dl_se, rq));
		dl_se->dl_throttled = 1;
		ఒకవేళ (dl_se->runtime > 0)
			dl_se->runtime = 0;
	}
}

స్థిర
పూర్ణాంకం dl_runtime_exceeded(నిర్మాణం sched_dl_entity *dl_se)
{
	తిరిగి (dl_se->runtime <= 0);
}

/*
 * This function implements the GRUB accounting rule. According to the
 * GRUB reclaiming algorithm, the runtime is not decreased as "dq = -dt",
 * but as "dq = -(max{u, (Umax - Uinact - Uextra)} / Umax) dt",
 * where u is the utilization of the task, Umax is the maximum reclaimable
 * utilization, Uinact is the (per-runqueue) inactive utilization, computed
 * as the difference between the "total runqueue utilization" and the
 * "runqueue active utilization", and Uextra is the (per runqueue) extra
 * reclaimable utilization.
 * Since rq->dl.running_bw and rq->dl.this_bw contain utilizations multiplied
 * by 2^BW_SHIFT, the result has to be shifted right by BW_SHIFT.
 * Since rq->dl.bw_ratio contains 1 / Umax multiplied by 2^RATIO_SHIFT, dl_bw
 * is multiplied by rq->dl.bw_ratio and shifted right by RATIO_SHIFT.
 * Since delta is a 64 bit variable, to have an overflow its value should be
 * larger than 2^(64 - 20 - 8), which is more than 64 seconds. So, overflow is
 * not an issue here.
 */
స్థిర u64 grub_reclaim(u64 delta, నిర్మాణం rq *rq, నిర్మాణం sched_dl_entity *dl_se)
{
	u64 u_act;
	u64 u_inact = rq->dl.this_bw - rq->dl.running_bw; /* Utot - Uact */

	/*
	 * Instead of computing max{u, (u_max - u_inact - u_extra)}, we
	 * compare u_inact + u_extra with u_max - u, because u_inact + u_extra
	 * can be larger than u_max. So, u_max - u_inact - u_extra would be
	 * negative leading to wrong results.
	 */
	ఒకవేళ (u_inact + rq->dl.extra_bw > rq->dl.max_bw - dl_se->dl_bw)
		u_act = dl_se->dl_bw;
	లేకపోతే
		u_act = rq->dl.max_bw - u_inact - rq->dl.extra_bw;

	u_act = (u_act * rq->dl.bw_నిష్పత్తి) >> నిష్పత్తి_జరుపు;
	తిరిగి (delta * u_act) >> BW_జరుపు;
}

s64 dl_scaled_delta_exec(నిర్మాణం rq *rq, నిర్మాణం sched_dl_entity *dl_se, s64 delta_exec)
{
	s64 scaled_delta_exec;

	/*
	 * For tasks that participate in GRUB, we implement GRUB-PA: the
	 * spare reclaimed bandwidth is used to clock down frequency.
	 *
	 * For the others, we still need to scale reservation parameters
	 * according to current frequency and CPU maximum capacity.
	 */
	ఒకవేళ (unlikely(dl_se->flags & SCHED_FLAG_RECLAIM)) {
		scaled_delta_exec = grub_reclaim(delta_exec, rq, dl_se);
	} లేకపోతే {
		పూర్ణాంకం cpu = cpu_యొక్క(rq);
		గుర్తులేని దీర్ఘ scale_freq = arch_scale_freq_సామర్థ్యం(cpu);
		గుర్తులేని దీర్ఘ scale_cpu = arch_scale_cpu_సామర్థ్యం(cpu);

		scaled_delta_exec = cap_scale(delta_exec, scale_freq);
		scaled_delta_exec = cap_scale(scaled_delta_exec, scale_cpu);
	}

	తిరిగి scaled_delta_exec;
}

స్థిర అంతఃవరుస శూన్య
update_stats_వరుస_నుండి_తీయి_dl(నిర్మాణం dl_rq *dl_rq, నిర్మాణం sched_dl_entity *dl_se, పూర్ణాంకం flags);

స్థిర శూన్య update_curr_dl_se(నిర్మాణం rq *rq, నిర్మాణం sched_dl_entity *dl_se, s64 delta_exec)
{
	bool idle = idle_rq(rq);
	s64 scaled_delta_exec;

	ఒకవేళ (unlikely(delta_exec <= 0)) {
		ఒకవేళ (unlikely(dl_se->dl_yielded))
			వెళ్ళు throttle;
		తిరిగి;
	}

	ఒకవేళ (dl_server(dl_se) && dl_se->dl_throttled && !dl_se->dl_defer)
		తిరిగి;

	ఒకవేళ (dl_entity_is_special(dl_se))
		తిరిగి;

	scaled_delta_exec = delta_exec;
	ఒకవేళ (!dl_server(dl_se))
		scaled_delta_exec = dl_scaled_delta_exec(rq, dl_se, delta_exec);

	dl_se->runtime -= scaled_delta_exec;

	ఒకవేళ (dl_se->dl_defer_idle && !idle)
		dl_se->dl_defer_idle = 0;

	/*
	 * The DL server can consume its runtime while throttled (not
	 * queued / running as regular CFS).
	 *
	 * If the server consumes its entire runtime in this state. The server
	 * is not required for the current period. Thus, reset the server by
	 * starting a new period, pushing the activation.
	 */
	ఒకవేళ (dl_se->dl_defer && dl_se->dl_throttled && dl_runtime_exceeded(dl_se)) {
		/*
		 * Non-servers would never get time accounted while throttled.
		 */
		WARN_ON_ONCE(!dl_server(dl_se));

		/*
		 * While the server is marked idle, do not push out the
		 * activation further, instead wait for the period timer
		 * to lapse and stop the server.
		 */
		ఒకవేళ (dl_se->dl_defer_idle && idle) {
			/*
			 * The timer is at the zero-laxity point, this means
			 * dl_server_stop() / dl_server_start() can happen
			 * while now < deadline. This means update_dl_entity()
			 * will not replenish. Additionally start_dl_timer()
			 * will be set for 'deadline - runtime'. Negative
			 * runtime will not do.
			 */
			dl_se->runtime = 0;
			తిరిగి;
		}

		/*
		 * If the server was previously activated - the starving condition
		 * took place, it this point it went away because the fair scheduler
		 * was able to get runtime in background. So return to the initial
		 * state.
		 */
		dl_se->dl_defer_running = 0;

		hrtimer_try_గమ్యం_cancel(&dl_se->dl_timer);

		replenish_dl_new_period(dl_se, dl_se->rq);

		ఒకవేళ (idle)
			dl_se->dl_defer_idle = 1;

		/*
		 * Not being able to start the timer seems problematic. If it could not
		 * be started for whatever reason, we need to "unthrottle" the DL server
		 * and queue right away. Otherwise nothing might queue it. That's similar
		 * to what enqueue_dl_entity() does on start_dl_timer==0. For now, just warn.
		 */
		WARN_ON_ONCE(!ప్రారంభం_dl_timer(dl_se));

		తిరిగి;
	}

throttle:
	ఒకవేళ (dl_runtime_exceeded(dl_se) || dl_se->dl_yielded) {
		trace_sched_dl_throttle_tp(dl_se, cpu_యొక్క(rq), dl_get_type(dl_se, rq));
		dl_se->dl_throttled = 1;

		/* If requested, inform the user about runtime overruns. */
		ఒకవేళ (dl_runtime_exceeded(dl_se) &&
		    (dl_se->flags & SCHED_FLAG_DL_OVERRUN))
			dl_se->dl_overrun = 1;

		వరుస_నుండి_తీయి_dl_entity(dl_se, 0);
		ఒకవేళ (!dl_server(dl_se)) {
			update_stats_వరుస_నుండి_తీయి_dl(&rq->dl, dl_se, 0);
			వరుస_నుండి_తీయి_pushable_dl_task(rq, dl_task_యొక్క(dl_se));
		}

		ఒకవేళ (unlikely(is_dl_boosted(dl_se) || !ప్రారంభం_dl_timer(dl_se))) {
			ఒకవేళ (dl_server(dl_se)) {
				ఒకవేళ (dl_se->dl_defer) {
					replenish_dl_new_period(dl_se, rq);
					ప్రారంభం_dl_timer(dl_se);
				} లేకపోతే {
					వరుసలో_చేర్చు_dl_entity(dl_se, వరుసలో_చేర్చు_REPLENISH);
				}
			} లేకపోతే {
				వరుసలో_చేర్చు_task_dl(rq, dl_task_యొక్క(dl_se), వరుసలో_చేర్చు_REPLENISH);
			}
		}

		ఒకవేళ (!is_leftmost(dl_se, &rq->dl))
			resched_curr(rq);
	} లేకపోతే {
		trace_sched_dl_update_tp(dl_se, cpu_యొక్క(rq), dl_get_type(dl_se, rq));
	}

	/*
	 * The dl_server does not account for real-time workload because it
	 * is running fair work.
	 */
	ఒకవేళ (dl_se->dl_server)
		తిరిగి;

#నిర్వచితమైతే CONFIG_RT_సమూహం_SCHED
	/*
	 * Because -- for now -- we share the rt bandwidth, we need to
	 * account our runtime there too, otherwise actual rt tasks
	 * would be able to exceed the shared quota.
	 *
	 * Account to the root rt group for now.
	 *
	 * The solution we're working towards is having the RT groups scheduled
	 * using deadline servers -- however there's a few nasties to figure
	 * out before that can happen.
	 */
	ఒకవేళ (rt_bandwidth_enabled()) {
		నిర్మాణం rt_rq *rt_rq = &rq->rt;

		raw_spin_తాళం(&rt_rq->rt_runtime_తాళం);
		/*
		 * We'll let actual RT tasks worry about the overflow here, we
		 * have our own CBS to keep us inline; only account when RT
		 * bandwidth is relevant.
		 */
		ఒకవేళ (sched_rt_bandwidth_account(rt_rq))
			rt_rq->rt_సమయం += delta_exec;
		raw_spin_unlock(&rt_rq->rt_runtime_తాళం);
	}
#అయితే_ముగింపు /* CONFIG_RT_GROUP_SCHED */
}

/*
 * In the non-defer mode, the idle time is not accounted, as the
 * server provides a guarantee.
 *
 * If the dl_server is in defer mode, the idle time is also considered as
 * time available for the dl_server, avoiding a penalty for the rt
 * scheduler that did not consumed that time.
 */
శూన్య dl_server_update_idle(నిర్మాణం sched_dl_entity *dl_se, s64 delta_exec)
{
	ఒకవేళ (dl_se->dl_server_చురుకైన && dl_se->dl_runtime && dl_se->dl_defer)
		update_curr_dl_se(dl_se->rq, dl_se, delta_exec);
}

శూన్య dl_server_update(నిర్మాణం sched_dl_entity *dl_se, s64 delta_exec)
{
	/* 0 runtime = fair server disabled */
	ఒకవేళ (dl_se->dl_server_చురుకైన && dl_se->dl_runtime)
		update_curr_dl_se(dl_se->rq, dl_se, delta_exec);
}

/*
 * dl_server && dl_defer:
 *
 *                                        6
 *                            +--------------------+
 *                            v                    |
 *     +-------------+  4   +-----------+  5     +------------------+
 * +-> |   A:init    | <--- | D:running | -----> | E:replenish-wait |
 * |   +-------------+      +-----------+        +------------------+
 * |     |         |    1     ^    ^               |
 * |     | 1       +----------+    | 3             |
 * |     v                         |               |
 * |   +--------------------------------+   2      |
 * |   |                                | ----+    |
 * | 8 |       B:zero_laxity-wait       |     |    |
 * |   |                                | <---+    |
 * |   +--------------------------------+          |
 * |     |              ^         ^       2        |
 * |     | 7            | 2, 1    +----------------+
 * |     v              |
 * |   +-------------+  |
 * +-- | C:idle-wait | -+
 *     +-------------+
 *       ^ 7       |
 *       +---------+
 *
 *
 * [A] - init
 *   dl_server_active = 0
 *   dl_throttled = 0
 *   dl_defer_armed = 0
 *   dl_defer_running = 0/1
 *   dl_defer_idle = 0
 *
 * [B] - zero_laxity-wait
 *   dl_server_active = 1
 *   dl_throttled = 1
 *   dl_defer_armed = 1
 *   dl_defer_running = 0
 *   dl_defer_idle = 0
 *
 * [C] - idle-wait
 *   dl_server_active = 1
 *   dl_throttled = 1
 *   dl_defer_armed = 1
 *   dl_defer_running = 0
 *   dl_defer_idle = 1
 *
 * [D] - running
 *   dl_server_active = 1
 *   dl_throttled = 0
 *   dl_defer_armed = 0
 *   dl_defer_running = 1
 *   dl_defer_idle = 0
 *
 * [E] - replenish-wait
 *   dl_server_active = 1
 *   dl_throttled = 1
 *   dl_defer_armed = 0
 *   dl_defer_running = 1
 *   dl_defer_idle = 0
 *
 *
 * [1] A->B, A->D, C->B
 * dl_server_start()
 *   dl_defer_idle = 0;
 *   if (dl_server_active)
 *     return; // [B]
 *   dl_server_active = 1;
 *   enqueue_dl_entity()
 *     update_dl_entity(WAKEUP)
 *       if (dl_time_before() || dl_entity_overflow())
 *         dl_defer_running = 0;
 *         replenish_dl_new_period();
 *           // fwd period
 *           dl_throttled = 1;
 *           dl_defer_armed = 1;
 *       if (!dl_defer_running)
 *         dl_defer_armed = 1;
 *         dl_throttled = 1;
 *     if (dl_throttled && start_dl_timer())
 *       return; // [B]
 *     __enqueue_dl_entity();
 *     // [D]
 *
 * // deplete server runtime from client-class
 * [2] B->B, C->B, E->B
 * dl_server_update()
 *   update_curr_dl_se() // idle = false
 *     if (dl_defer_idle)
 *       dl_defer_idle = 0;
 *     if (dl_defer && dl_throttled && dl_runtime_exceeded())
 *       dl_defer_running = 0;
 *       hrtimer_try_to_cancel();   // stop timer
 *       replenish_dl_new_period()
 *         // fwd period
 *         dl_throttled = 1;
 *         dl_defer_armed = 1;
 *       start_dl_timer();        // restart timer
 *       // [B]
 *
 * // timer actually fires means we have runtime
 * [3] B->D
 * dl_server_timer()
 *   if (dl_defer_armed)
 *     dl_defer_running = 1;
 *   enqueue_dl_entity(REPLENISH)
 *     replenish_dl_entity()
 *       // fwd period
 *       if (dl_throttled)
 *         dl_throttled = 0;
 *       if (dl_defer_armed)
 *         dl_defer_armed = 0;
 *     __enqueue_dl_entity();
 *     // [D]
 *
 * // schedule server
 * [4] D->A
 * pick_task_dl()
 *   p = server_pick_task();
 *   if (!p)
 *     dl_server_stop()
 *       dequeue_dl_entity();
 *       hrtimer_try_to_cancel();
 *       dl_defer_armed = 0;
 *       dl_throttled = 0;
 *       dl_server_active = 0;
 *       // [A]
 *   return p;
 *
 * // server running
 * [5] D->E
 * update_curr_dl_se()
 *   if (dl_runtime_exceeded())
 *     dl_throttled = 1;
 *     dequeue_dl_entity();
 *     start_dl_timer();
 *     // [E]
 *
 * // server replenished
 * [6] E->D
 * dl_server_timer()
 *   enqueue_dl_entity(REPLENISH)
 *     replenish_dl_entity()
 *       fwd-period
 *       if (dl_throttled)
 *         dl_throttled = 0;
 *     __enqueue_dl_entity();
 *     // [D]
 *
 * // deplete server runtime from idle
 * [7] B->C, C->C
 * dl_server_update_idle()
 *   update_curr_dl_se() // idle = true
 *     if (dl_defer && dl_throttled && dl_runtime_exceeded())
 *       if (dl_defer_idle)
 *         return;
 *       dl_defer_running = 0;
 *       hrtimer_try_to_cancel();
 *       replenish_dl_new_period()
 *         // fwd period
 *         dl_throttled = 1;
 *         dl_defer_armed = 1;
 *       dl_defer_idle = 1;
 *       start_dl_timer();        // restart timer
 *       // [C]
 *
 * // stop idle server
 * [8] C->A
 * dl_server_timer()
 *   if (dl_defer_idle)
 *     dl_server_stop();
 *     // [A]
 *
 *
 * digraph dl_server {
 *   "A:init" -> "B:zero_laxity-wait"             [label="1:dl_server_start"]
 *   "A:init" -> "D:running"                      [label="1:dl_server_start"]
 *   "B:zero_laxity-wait" -> "B:zero_laxity-wait" [label="2:dl_server_update"]
 *   "B:zero_laxity-wait" -> "C:idle-wait"        [label="7:dl_server_update_idle"]
 *   "B:zero_laxity-wait" -> "D:running"          [label="3:dl_server_timer"]
 *   "C:idle-wait" -> "A:init"                    [label="8:dl_server_timer"]
 *   "C:idle-wait" -> "B:zero_laxity-wait"        [label="1:dl_server_start"]
 *   "C:idle-wait" -> "B:zero_laxity-wait"        [label="2:dl_server_update"]
 *   "C:idle-wait" -> "C:idle-wait"               [label="7:dl_server_update_idle"]
 *   "D:running" -> "A:init"                      [label="4:pick_task_dl"]
 *   "D:running" -> "E:replenish-wait"            [label="5:update_curr_dl_se"]
 *   "E:replenish-wait" -> "B:zero_laxity-wait"   [label="2:dl_server_update"]
 *   "E:replenish-wait" -> "D:running"            [label="6:dl_server_timer"]
 * }
 *
 *
 * Notes:
 *
 *  - When there are fair tasks running the most likely loop is [2]->[2].
 *    the dl_server never actually runs, the timer never fires.
 *
 *  - When there is actual fair starvation; the timer fires and starts the
 *    dl_server. This will then throttle and replenish like a normal DL
 *    task. Notably it will not 'defer' again.
 *
 *  - When idle it will push the actication forward once, and then wait
 *    for the timer to hit or a non-idle update to restart things.
 */
శూన్య dl_server_ప్రారంభం(నిర్మాణం sched_dl_entity *dl_se)
{
	నిర్మాణం rq *rq = dl_se->rq;

	dl_se->dl_defer_idle = 0;
	ఒకవేళ (!dl_server(dl_se) || dl_se->dl_server_చురుకైన || !dl_se->dl_runtime ||
	    !dl_se->dl_bw_attached)
		తిరిగి;

	/*
	 * Update the current task to 'now'.
	 */
	rq->donor->sched_class->update_curr(rq);

	ఒకవేళ (WARN_ON_ONCE(!cpu_online(cpu_యొక్క(rq))))
		తిరిగి;

	trace_sched_dl_server_ప్రారంభం_tp(dl_se, cpu_యొక్క(rq), dl_get_type(dl_se, rq));
	dl_se->dl_server_చురుకైన = 1;
	వరుసలో_చేర్చు_dl_entity(dl_se, వరుసలో_చేర్చు_WAKEUP);
	ఒకవేళ (!dl_task(dl_se->rq->curr) || dl_entity_preempt(dl_se, &rq->curr->dl))
		resched_curr(dl_se->rq);
}

శూన్య dl_server_stop(నిర్మాణం sched_dl_entity *dl_se)
{
	ఒకవేళ (!dl_server(dl_se) || !dl_server_చురుకైన(dl_se))
		తిరిగి;

	trace_sched_dl_server_stop_tp(dl_se, cpu_యొక్క(dl_se->rq),
				      dl_get_type(dl_se, dl_se->rq));
	వరుస_నుండి_తీయి_dl_entity(dl_se, వరుస_నుండి_తీయి_SLEEP);
	hrtimer_try_గమ్యం_cancel(&dl_se->dl_timer);
	dl_se->dl_defer_armed = 0;
	dl_se->dl_throttled = 0;
	dl_se->dl_defer_idle = 0;
	dl_se->dl_server_చురుకైన = 0;
}

శూన్య dl_server_init(నిర్మాణం sched_dl_entity *dl_se, నిర్మాణం rq *rq,
		    dl_server_pick_f pick_task)
{
	dl_se->rq = rq;
	dl_se->server_pick_task = pick_task;
}

శూన్య sched_init_dl_servers(శూన్య)
{
	పూర్ణాంకం cpu;
	నిర్మాణం rq *rq;
	నిర్మాణం sched_dl_entity *dl_se;

	for_each_online_cpu(cpu) {
		u64 runtime =  50 * NSEC_చొప్పున_MSEC;
		u64 period = 1000 * NSEC_చొప్పున_MSEC;

		rq = cpu_rq(cpu);

		guard(rq_తాళం_irq)(rq);
		update_rq_clock(rq);

		dl_se = &rq->fair_server;

		WARN_ON(dl_server(dl_se));

		dl_server_apply_params(dl_se, runtime, period, 1);

		dl_se->dl_server = 1;
		dl_se->dl_defer = 1;
		అమరిక_new_dl_entity(dl_se);

#నిర్వచితమైతే CONFIG_SCHED_CLASS_EXT
		dl_se = &rq->ext_server;

		WARN_ON(dl_server(dl_se));

		dl_server_apply_params(dl_se, runtime, period, 1);

		dl_se->dl_server = 1;
		dl_se->dl_defer = 1;
		అమరిక_new_dl_entity(dl_se);

		/*
		 * No BPF scheduler is loaded at boot, so the ext_server has no
		 * tasks to protect. Detach its bandwidth reservation, it will
		 * be attached when a BPF scheduler is loaded.
		 */
		dl_server_detach_bw(dl_se);
#అయితే_ముగింపు
	}
}

శూన్య __dl_server_attach_root(నిర్మాణం sched_dl_entity *dl_se, నిర్మాణం rq *rq)
{
	u64 new_bw = dl_se->dl_bw;
	పూర్ణాంకం cpu = cpu_యొక్క(rq);
	నిర్మాణం dl_bw *dl_b;

	ఒకవేళ (!dl_se->dl_bw_attached)
		తిరిగి;

	dl_b = dl_bw_యొక్క(cpu_యొక్క(rq));
	guard(raw_spinlock)(&dl_b->తాళం);

	ఒకవేళ (!dl_bw_cpus(cpu))
		తిరిగి;

	__dl_add(dl_b, new_bw, dl_bw_cpus(cpu));
}

పూర్ణాంకం dl_server_apply_params(నిర్మాణం sched_dl_entity *dl_se, u64 runtime, u64 period, bool init)
{
	u64 old_bw = (init || !dl_se->dl_bw_attached) ? 0 :
		     గమ్యం_నిష్పత్తి(dl_se->dl_period, dl_se->dl_runtime);
	u64 new_bw = గమ్యం_నిష్పత్తి(period, runtime);
	నిర్మాణం rq *rq = dl_se->rq;
	పూర్ణాంకం cpu = cpu_యొక్క(rq);
	నిర్మాణం dl_bw *dl_b;
	గుర్తులేని దీర్ఘ cap;
	పూర్ణాంకం cpus;

	dl_b = dl_bw_యొక్క(cpu);
	guard(raw_spinlock)(&dl_b->తాళం);

	cpus = dl_bw_cpus(cpu);
	cap = dl_bw_సామర్థ్యం(cpu);

	ఒకవేళ (__dl_overflow(dl_b, cap, old_bw, new_bw))
		తిరిగి -EBUSY;

	ఒకవేళ (init) {
		__add_rq_bw(new_bw, &rq->dl);
		__dl_add(dl_b, new_bw, cpus);
		dl_se->dl_bw_attached = 1;
	} లేకపోతే ఒకవేళ (dl_se->dl_bw_attached) {
		__dl_sub(dl_b, dl_se->dl_bw, cpus);
		__dl_add(dl_b, new_bw, cpus);

		dl_rq_change_utilization(rq, dl_se, new_bw);
	}

	dl_se->dl_runtime = runtime;
	dl_se->dl_deadline = period;
	dl_se->dl_period = period;

	dl_se->runtime = 0;
	dl_se->deadline = 0;

	dl_se->dl_bw = గమ్యం_నిష్పత్తి(dl_se->dl_period, dl_se->dl_runtime);
	dl_se->dl_density = గమ్యం_నిష్పత్తి(dl_se->dl_deadline, dl_se->dl_runtime);

	తిరిగి 0;
}

/*
 * Add @dl_se's bw to the root-domain accounting.
 *
 * Return -EBUSY if attaching would overflow root domain capacity.
 */
స్థిర పూర్ణాంకం __dl_server_attach_bw_locked(నిర్మాణం sched_dl_entity *dl_se,
					నిర్మాణం dl_bw *dl_b, పూర్ణాంకం cpus)
{
	నిర్మాణం rq *rq = dl_se->rq;
	గుర్తులేని దీర్ఘ cap;

	/*
	 * Always update @rq->dl.this_bw, but only update @dl_b->total_bw
	 * (and run the overflow check it gates) while this CPU is active.
	 *
	 * This mirrors dl_server_add_bw() during root-domain rebuilds, which
	 * only publishes bandwidth from active CPUs into @dl_b.
	 */
	ఒకవేళ (cpu_చురుకైన(cpu_యొక్క(rq))) {
		cap = dl_bw_సామర్థ్యం(cpu_యొక్క(rq));
		ఒకవేళ (__dl_overflow(dl_b, cap, 0, dl_se->dl_bw))
			తిరిగి -EBUSY;
		__dl_add(dl_b, dl_se->dl_bw, cpus);
	}
	__add_rq_bw(dl_se->dl_bw, &rq->dl);
	dl_se->dl_bw_attached = 1;

	తిరిగి 0;
}

/*
 * Drain @dl_se and remove its bw from the root-domain accounting.
 */
స్థిర శూన్య __dl_server_detach_bw_locked(నిర్మాణం sched_dl_entity *dl_se,
					 నిర్మాణం dl_bw *dl_b, పూర్ణాంకం cpus)
{
	నిర్మాణం rq *rq = dl_se->rq;

	/*
	 * If the server is still active (on_rq), dequeue it via
	 * dl_server_stop(); task_non_contending() will either subtract
	 * @dl_bw from running_bw immediately (0-lag passed) or set
	 * dl_non_contending and arm the inactive_timer.
	 */
	ఒకవేళ (dl_se->dl_server_చురుకైన)
		dl_server_stop(dl_se);

	/*
	 * Drop @dl_se's contribution from this rq's bandwidth accounting,
	 * mirroring the __add_rq_bw() done at attach time.
	 */
	dl_rq_change_utilization(rq, dl_se, 0);

	/*
	 * Update @dl_b only while this CPU is active, matching
	 * dl_server_add_bw() during root-domain rebuilds.
	 *
	 * If this CPU is inactive, its bandwidth is not currently accounted in
	 * @dl_b->total_bw: either attach skipped adding it, or a rebuild
	 * already dropped it while re-publishing active CPUs only.
	 *
	 * In that case there is nothing to subtract from @dl_b. Just clear
	 * @dl_se->dl_bw_attached; if the CPU becomes active again, the next
	 * rebuild will re-publish its bandwidth.
	 */
	ఒకవేళ (cpu_చురుకైన(cpu_యొక్క(rq)))
		__dl_sub(dl_b, dl_se->dl_bw, cpus);
	dl_se->dl_bw_attached = 0;
}

/*
 * Attach @dl_se's bandwidth to the root domain's total_bw accounting.
 *
 * Use to dynamically register a dl_server's bandwidth reservation while
 * preserving its configured @dl_runtime / @dl_period. No-op if @dl_se is
 * already attached.
 *
 * Returns -EBUSY if attaching would overflow the root domain capacity.
 */
పూర్ణాంకం dl_server_attach_bw(నిర్మాణం sched_dl_entity *dl_se)
{
	నిర్మాణం rq *rq = dl_se->rq;
	పూర్ణాంకం cpu = cpu_యొక్క(rq);
	నిర్మాణం dl_bw *dl_b;
	పూర్ణాంకం cpus, ret;

	ఒకవేళ (dl_se->dl_bw_attached)
		తిరిగి 0;

	scoped_guard (raw_spinlock, &dl_bw_యొక్క(cpu)->తాళం) {
		dl_b = dl_bw_యొక్క(cpu);
		cpus = dl_bw_cpus(cpu);
		ret = __dl_server_attach_bw_locked(dl_se, dl_b, cpus);
	}
	ఒకవేళ (ret)
		తిరిగి ret;

	/*
	 * The natural 0->nr_running transition that triggers dl_server_start()
	 * may have happened while @dl_se was still detached (e.g., between
	 * scx_bypass(false) and the scx_enable() re-balance loop), so kick a
	 * start here.
	 *
	 * dl_server_start() bails out cleanly if there's nothing to schedule or
	 * it's already active. Skip if @cpu is offline; the server will be
	 * started naturally on the first enqueue once @cpu comes back.
	 */
	ఒకవేళ (cpu_online(cpu))
		dl_server_ప్రారంభం(dl_se);

	తిరిగి 0;
}

/*
 * Detach @dl_se's bandwidth from the root domain's total_bw accounting.
 *
 * Use to dynamically unregister a dl_server's bandwidth reservation while
 * preserving its configured @dl_runtime / @dl_period. No-op if @dl_se is
 * not currently attached.
 */
శూన్య dl_server_detach_bw(నిర్మాణం sched_dl_entity *dl_se)
{
	పూర్ణాంకం cpu = cpu_యొక్క(dl_se->rq);
	నిర్మాణం dl_bw *dl_b;
	పూర్ణాంకం cpus;

	ఒకవేళ (!dl_se->dl_bw_attached)
		తిరిగి;

	dl_b = dl_bw_యొక్క(cpu);
	guard(raw_spinlock)(&dl_b->తాళం);
	cpus = dl_bw_cpus(cpu);
	__dl_server_detach_bw_locked(dl_se, dl_b, cpus);
}

/*
 * Atomically detach @detach_se and attach @attach_se on the same rq, holding
 * @dl_b->lock across both operations so a concurrent sched_setattr() cannot
 * steal the bandwidth freed by the detach before the attach can claim it.
 *
 * Both entities must live on the same rq (same root domain). Returns the
 * result of the attach: -EBUSY if attaching @attach_se would overflow root
 * domain capacity (in which case both servers end up detached).
 */
పూర్ణాంకం dl_server_మార్పిడి_bw(నిర్మాణం sched_dl_entity *detach_se,
		      నిర్మాణం sched_dl_entity *attach_se)
{
	నిర్మాణం rq *rq = detach_se->rq;
	పూర్ణాంకం cpu = cpu_యొక్క(rq);
	నిర్మాణం dl_bw *dl_b;
	పూర్ణాంకం cpus, ret;

	WARN_ON_ONCE(attach_se->rq != rq);

	scoped_guard (raw_spinlock, &dl_bw_యొక్క(cpu)->తాళం) {
		dl_b = dl_bw_యొక్క(cpu);
		cpus = dl_bw_cpus(cpu);

		ఒకవేళ (detach_se->dl_bw_attached)
			__dl_server_detach_bw_locked(detach_se, dl_b, cpus);

		ఒకవేళ (attach_se->dl_bw_attached)
			ret = 0;
		లేకపోతే
			ret = __dl_server_attach_bw_locked(attach_se, dl_b, cpus);
	}
	ఒకవేళ (ret)
		తిరిగి ret;

	ఒకవేళ (cpu_online(cpu))
		dl_server_ప్రారంభం(attach_se);

	తిరిగి 0;
}

/*
 * Update the current task's runtime statistics (provided it is still
 * a -deadline task and has not been removed from the dl_rq).
 */
స్థిర శూన్య update_curr_dl(నిర్మాణం rq *rq)
{
	నిర్మాణం task_struct *donor = rq->donor;
	నిర్మాణం sched_dl_entity *dl_se = &donor->dl;
	s64 delta_exec;

	ఒకవేళ (!dl_task(donor) || !on_dl_rq(dl_se))
		తిరిగి;

	/*
	 * Consumed budget is computed considering the time as
	 * observed by schedulable tasks (excluding time spent
	 * in hardirq context, etc.). Deadlines are instead
	 * computed using hard walltime. This seems to be the more
	 * natural solution, but the full ramifications of this
	 * approach need further study.
	 */
	delta_exec = update_curr_common(rq);
	update_curr_dl_se(rq, dl_se, delta_exec);
}

స్థిర గణన hrtimer_restart inactive_task_timer(నిర్మాణం hrtimer *timer)
{
	నిర్మాణం sched_dl_entity *dl_se = container_యొక్క(timer,
						     నిర్మాణం sched_dl_entity,
						     inactive_timer);
	నిర్మాణం task_struct *p = ఖాళీ;
	నిర్మాణం rq_flags rf;
	నిర్మాణం rq *rq;

	ఒకవేళ (!dl_server(dl_se)) {
		p = dl_task_యొక్క(dl_se);
		rq = task_rq_తాళం_2(p, &rf);
	} లేకపోతే {
		rq = dl_se->rq;
		rq_తాళం(rq, &rf);
	}

	sched_clock_tick();
	update_rq_clock(rq);

	ఒకవేళ (dl_server(dl_se))
		వెళ్ళు no_task;

	ఒకవేళ (!dl_task(p) || READ_ONCE(p->__state) == TASK_DEAD) {
		నిర్మాణం dl_bw *dl_b = dl_bw_యొక్క(task_cpu(p));

		ఒకవేళ (READ_ONCE(p->__state) == TASK_DEAD && dl_se->dl_non_contending) {
			sub_running_bw(&p->dl, dl_rq_యొక్క_se(&p->dl));
			sub_rq_bw(&p->dl, dl_rq_యొక్క_se(&p->dl));
			dl_se->dl_non_contending = 0;
		}

		raw_spin_తాళం(&dl_b->తాళం);
		__dl_sub(dl_b, p->dl.dl_bw, dl_bw_cpus(task_cpu(p)));
		raw_spin_unlock(&dl_b->తాళం);
		__dl_clear_params(dl_se);

		వెళ్ళు unlock;
	}

no_task:
	ఒకవేళ (dl_se->dl_non_contending == 0)
		వెళ్ళు unlock;

	sub_running_bw(dl_se, &rq->dl);
	dl_se->dl_non_contending = 0;
unlock:

	ఒకవేళ (!dl_server(dl_se)) {
		task_rq_unlock(rq, p, &rf);
		put_task_struct(p);
	} లేకపోతే {
		rq_unlock(rq, &rf);
	}

	తిరిగి HRTIMER_NORESTART;
}

స్థిర శూన్య init_dl_inactive_task_timer(నిర్మాణం sched_dl_entity *dl_se)
{
	నిర్మాణం hrtimer *timer = &dl_se->inactive_timer;

	hrtimer_అమరిక(timer, inactive_task_timer, CLOCK_MONOTONIC, HRTIMER_బహుళకం_REL_HARD);
}

#నిర్వచించు __node_2_dle(కణుపు) \
	rb_నమోదు((కణుపు), నిర్మాణం sched_dl_entity, rb_కణుపు)

స్థిర శూన్య inc_dl_deadline(నిర్మాణం dl_rq *dl_rq, u64 deadline)
{
	నిర్మాణం rq *rq = rq_యొక్క_dl_rq(dl_rq);

	ఒకవేళ (dl_rq->earliest_dl.curr == 0 ||
	    dl_సమయం_before(deadline, dl_rq->earliest_dl.curr)) {
		ఒకవేళ (dl_rq->earliest_dl.curr == 0)
			cpupri_సమితి(&rq->rd->cpupri, rq->cpu, CPUPRI_HIGHER);
		dl_rq->earliest_dl.curr = deadline;
		cpudl_సమితి(&rq->rd->cpudl, rq->cpu, deadline);
	}
}

స్థిర శూన్య dec_dl_deadline(నిర్మాణం dl_rq *dl_rq, u64 deadline)
{
	నిర్మాణం rq *rq = rq_యొక్క_dl_rq(dl_rq);

	/*
	 * Since we may have removed our earliest (and/or next earliest)
	 * task we must recompute them.
	 */
	ఒకవేళ (!dl_rq->dl_nr_running) {
		dl_rq->earliest_dl.curr = 0;
		dl_rq->earliest_dl.తదుపరి = 0;
		cpudl_clear(&rq->rd->cpudl, rq->cpu, rq->online);
		cpupri_సమితి(&rq->rd->cpupri, rq->cpu, rq->rt.highest_prio.curr);
	} లేకపోతే {
		నిర్మాణం rb_కణుపు *leftmost = rb_మొదటి_cached(&dl_rq->మూలం);
		నిర్మాణం sched_dl_entity *నమోదు_2 = __node_2_dle(leftmost);

		dl_rq->earliest_dl.curr = నమోదు_2->deadline;
		cpudl_సమితి(&rq->rd->cpudl, rq->cpu, నమోదు_2->deadline);
	}
}

స్థిర అంతఃవరుస
శూన్య inc_dl_tasks(నిర్మాణం sched_dl_entity *dl_se, నిర్మాణం dl_rq *dl_rq)
{
	u64 deadline = dl_se->deadline;

	dl_rq->dl_nr_running++;

	ఒకవేళ (!dl_server(dl_se))
		కూడు_nr_running(rq_యొక్క_dl_rq(dl_rq), 1);

	inc_dl_deadline(dl_rq, deadline);
}

స్థిర అంతఃవరుస
శూన్య dec_dl_tasks(నిర్మాణం sched_dl_entity *dl_se, నిర్మాణం dl_rq *dl_rq)
{
	WARN_ON(!dl_rq->dl_nr_running);
	dl_rq->dl_nr_running--;

	ఒకవేళ (!dl_server(dl_se))
		sub_nr_running(rq_యొక్క_dl_rq(dl_rq), 1);

	dec_dl_deadline(dl_rq, dl_se->deadline);
}

స్థిర అంతఃవరుస bool __dl_less(నిర్మాణం rb_కణుపు *a, స్థిరరాశి నిర్మాణం rb_కణుపు *b)
{
	తిరిగి dl_సమయం_before(__node_2_dle(a)->deadline, __node_2_dle(b)->deadline);
}

స్థిర __always_inline నిర్మాణం sched_statistics *
__schedstats_from_dl_se(నిర్మాణం sched_dl_entity *dl_se)
{
	ఒకవేళ (!schedstat_enabled())
		తిరిగి ఖాళీ;

	ఒకవేళ (dl_server(dl_se))
		తిరిగి ఖాళీ;

	తిరిగి &dl_task_యొక్క(dl_se)->stats;
}

స్థిర అంతఃవరుస శూన్య
update_stats_wait_ప్రారంభం_dl(నిర్మాణం dl_rq *dl_rq, నిర్మాణం sched_dl_entity *dl_se)
{
	నిర్మాణం sched_statistics *stats = __schedstats_from_dl_se(dl_se);
	ఒకవేళ (stats)
		__update_stats_wait_start(rq_యొక్క_dl_rq(dl_rq), dl_task_యొక్క(dl_se), stats);
}

స్థిర అంతఃవరుస శూన్య
update_stats_wait_చివర_dl(నిర్మాణం dl_rq *dl_rq, నిర్మాణం sched_dl_entity *dl_se)
{
	నిర్మాణం sched_statistics *stats = __schedstats_from_dl_se(dl_se);
	ఒకవేళ (stats)
		__update_stats_wait_end(rq_యొక్క_dl_rq(dl_rq), dl_task_యొక్క(dl_se), stats);
}

స్థిర అంతఃవరుస శూన్య
update_stats_వరుసలో_చేర్చు_sleeper_dl(నిర్మాణం dl_rq *dl_rq, నిర్మాణం sched_dl_entity *dl_se)
{
	నిర్మాణం sched_statistics *stats = __schedstats_from_dl_se(dl_se);
	ఒకవేళ (stats)
		__update_stats_enqueue_sleeper(rq_యొక్క_dl_rq(dl_rq), dl_task_యొక్క(dl_se), stats);
}

స్థిర అంతఃవరుస శూన్య
update_stats_వరుసలో_చేర్చు_dl(నిర్మాణం dl_rq *dl_rq, నిర్మాణం sched_dl_entity *dl_se,
			పూర్ణాంకం flags)
{
	ఒకవేళ (!schedstat_enabled())
		తిరిగి;

	ఒకవేళ (flags & వరుసలో_చేర్చు_WAKEUP)
		update_stats_వరుసలో_చేర్చు_sleeper_dl(dl_rq, dl_se);
}

స్థిర అంతఃవరుస శూన్య
update_stats_వరుస_నుండి_తీయి_dl(నిర్మాణం dl_rq *dl_rq, నిర్మాణం sched_dl_entity *dl_se,
			పూర్ణాంకం flags)
{
	నిర్మాణం task_struct *p = dl_task_యొక్క(dl_se);
	నిర్మాణం rq *rq = rq_యొక్క_dl_rq(dl_rq);

	ఒకవేళ (!schedstat_enabled())
		తిరిగి;

	ఒకవేళ (p != rq->curr)
		update_stats_wait_చివర_dl(dl_rq, dl_se);

	ఒకవేళ ((flags & వరుస_నుండి_తీయి_SLEEP)) {
		గుర్తులేని పూర్ణాంకం state;

		state = READ_ONCE(p->__state);
		ఒకవేళ (state & TASK_INTERRUPTIBLE)
			__schedstat_set(p->stats.sleep_ప్రారంభం,
					rq_clock(rq_యొక్క_dl_rq(dl_rq)));

		ఒకవేళ (state & TASK_UNINTERRUPTIBLE)
			__schedstat_set(p->stats.ఖండం_ప్రారంభం,
					rq_clock(rq_యొక్క_dl_rq(dl_rq)));
	}
}

స్థిర శూన్య __enqueue_dl_entity(నిర్మాణం sched_dl_entity *dl_se)
{
	నిర్మాణం dl_rq *dl_rq = dl_rq_యొక్క_se(dl_se);

	WARN_ON_ONCE(!RB_ఖాళీలు_కణుపు(&dl_se->rb_కణుపు));

	rb_కూడు_cached(&dl_se->rb_కణుపు, &dl_rq->మూలం, __dl_less);

	inc_dl_tasks(dl_se, dl_rq);
}

స్థిర శూన్య __dequeue_dl_entity(నిర్మాణం sched_dl_entity *dl_se)
{
	నిర్మాణం dl_rq *dl_rq = dl_rq_యొక్క_se(dl_se);

	ఒకవేళ (RB_ఖాళీలు_కణుపు(&dl_se->rb_కణుపు))
		తిరిగి;

	rb_erase_cached(&dl_se->rb_కణుపు, &dl_rq->మూలం);

	RB_CLEAR_కణుపు(&dl_se->rb_కణుపు);

	dec_dl_tasks(dl_se, dl_rq);
}

స్థిర శూన్య
వరుసలో_చేర్చు_dl_entity(నిర్మాణం sched_dl_entity *dl_se, పూర్ణాంకం flags)
{
	WARN_ON_ONCE(on_dl_rq(dl_se));

	update_stats_వరుసలో_చేర్చు_dl(dl_rq_యొక్క_se(dl_se), dl_se, flags);

	/*
	 * Check if a constrained deadline task was activated
	 * after the deadline but before the next period.
	 * If that is the case, the task will be throttled and
	 * the replenishment timer will be set to the next period.
	 */
	ఒకవేళ (!dl_se->dl_throttled && !dl_is_implicit(dl_se))
		dl_check_constrained_dl(dl_se);

	ఒకవేళ (flags & (వరుసలో_చేర్చు_RESTORE|వరుసలో_చేర్చు_MIGRATING)) {
		నిర్మాణం dl_rq *dl_rq = dl_rq_యొక్క_se(dl_se);

		కూడు_rq_bw(dl_se, dl_rq);
		కూడు_running_bw(dl_se, dl_rq);
	}

	/*
	 * If p is throttled, we do not enqueue it. In fact, if it exhausted
	 * its budget it needs a replenishment and, since it now is on
	 * its rq, the bandwidth timer callback (which clearly has not
	 * run yet) will take care of this.
	 * However, the active utilization does not depend on the fact
	 * that the task is on the runqueue or not (but depends on the
	 * task's state - in GRUB parlance, "inactive" vs "active contending").
	 * In other words, even if a task is throttled its utilization must
	 * be counted in the active utilization; hence, we need to call
	 * add_running_bw().
	 */
	ఒకవేళ (!dl_se->dl_defer && dl_se->dl_throttled && !(flags & వరుసలో_చేర్చు_REPLENISH)) {
		ఒకవేళ (flags & వరుసలో_చేర్చు_WAKEUP)
			task_contending(dl_se, flags);

		తిరిగి;
	}

	/*
	 * If this is a wakeup or a new instance, the scheduling
	 * parameters of the task might need updating. Otherwise,
	 * we want a replenishment of its runtime.
	 */
	ఒకవేళ (flags & వరుసలో_చేర్చు_WAKEUP) {
		task_contending(dl_se, flags);
		update_dl_entity(dl_se);
	} లేకపోతే ఒకవేళ (flags & వరుసలో_చేర్చు_REPLENISH) {
		replenish_dl_entity(dl_se);
	} లేకపోతే ఒకవేళ ((flags & వరుసలో_చేర్చు_MOVE) &&
		   !is_dl_boosted(dl_se) &&
		   dl_సమయం_before(dl_se->deadline, rq_clock(rq_యొక్క_dl_se(dl_se)))) {
		అమరిక_new_dl_entity(dl_se);
	}

	/*
	 * If the reservation is still throttled, e.g., it got replenished but is a
	 * deferred task and still got to wait, don't enqueue.
	 */
	ఒకవేళ (dl_se->dl_throttled && ప్రారంభం_dl_timer(dl_se))
		తిరిగి;

	/*
	 * We're about to enqueue, make sure we're not ->dl_throttled!
	 * In case the timer was not started, say because the defer time
	 * has passed, mark as not throttled and mark unarmed.
	 * Also cancel earlier timers, since letting those run is pointless.
	 */
	ఒకవేళ (dl_se->dl_throttled) {
		hrtimer_try_గమ్యం_cancel(&dl_se->dl_timer);
		dl_se->dl_defer_armed = 0;
		dl_se->dl_throttled = 0;
	}

	__enqueue_dl_entity(dl_se);
}

స్థిర శూన్య వరుస_నుండి_తీయి_dl_entity(నిర్మాణం sched_dl_entity *dl_se, పూర్ణాంకం flags)
{
	__dequeue_dl_entity(dl_se);

	ఒకవేళ (flags & (వరుస_నుండి_తీయి_SAVE|వరుస_నుండి_తీయి_MIGRATING)) {
		నిర్మాణం dl_rq *dl_rq = dl_rq_యొక్క_se(dl_se);

		sub_running_bw(dl_se, dl_rq);
		sub_rq_bw(dl_se, dl_rq);
	}

	/*
	 * This check allows to start the inactive timer (or to immediately
	 * decrease the active utilization, if needed) in two cases:
	 * when the task blocks and when it is terminating
	 * (p->state == TASK_DEAD). We can handle the two cases in the same
	 * way, because from GRUB's point of view the same thing is happening
	 * (the task moves from "active contending" to "active non contending"
	 * or "inactive")
	 */
	ఒకవేళ (flags & వరుస_నుండి_తీయి_SLEEP)
		task_non_contending(dl_se, true);
}

స్థిర శూన్య వరుసలో_చేర్చు_task_dl(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, పూర్ణాంకం flags)
{
	నిర్మాణం sched_dl_entity *dl_se = &p->dl;
	నిర్మాణం dl_rq *dl_rq = &rq->dl;

	ఒకవేళ (is_dl_boosted(dl_se)) {
		/*
		 * Because of delays in the detection of the overrun of a
		 * thread's runtime, it might be the case that a thread
		 * goes to sleep in a rt mutex with negative runtime. As
		 * a consequence, the thread will be throttled.
		 *
		 * While waiting for the mutex, this thread can also be
		 * boosted via PI, resulting in a thread that is throttled
		 * and boosted at the same time.
		 *
		 * In this case, the boost overrides the throttle.
		 */
		ఒకవేళ (dl_se->dl_throttled) {
			/*
			 * The replenish timer needs to be canceled. No
			 * problem if it fires concurrently: boosted threads
			 * are ignored in dl_task_timer().
			 */
			cancel_replenish_timer(dl_se);
			dl_se->dl_throttled = 0;
		}
	} లేకపోతే ఒకవేళ (!dl_prio(p->సాధారణ_prio)) {
		/*
		 * Special case in which we have a !SCHED_DEADLINE task that is going
		 * to be deboosted, but exceeds its runtime while doing so. No point in
		 * replenishing it, as it's going to return back to its original
		 * scheduling class after this. If it has been throttled, we need to
		 * clear the flag, otherwise the task may wake up as throttled after
		 * being boosted again with no means to replenish the runtime and clear
		 * the throttle.
		 */
		dl_se->dl_throttled = 0;
		ఒకవేళ (!(flags & వరుసలో_చేర్చు_REPLENISH))
			printk_deferred_once("sched: DL de-boosted task PID %d: REPLENISH flag missing\n",
					     task_pid_nr(p));

		తిరిగి;
	}

	check_schedstat_required();
	update_stats_wait_ప్రారంభం_dl(dl_rq, dl_se);

	ఒకవేళ (task_on_rq_migrating(p))
		flags |= వరుసలో_చేర్చు_MIGRATING;

	వరుసలో_చేర్చు_dl_entity(dl_se, flags);

	ఒకవేళ (dl_server(dl_se))
		తిరిగి;

	ఒకవేళ (task_is_blocked(p))
		తిరిగి;

	ఒకవేళ (dl_rq->curr == dl_se)
		తిరిగి;

	ఒకవేళ (!task_current(rq, p) && !dl_se->dl_throttled && p->nr_cpus_allowed > 1)
		వరుసలో_చేర్చు_pushable_dl_task(rq, p);
}

స్థిర bool వరుస_నుండి_తీయి_task_dl(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, పూర్ణాంకం flags)
{
	update_curr_dl(rq);

	ఒకవేళ (task_on_rq_migrating(p))
		flags |= వరుస_నుండి_తీయి_MIGRATING;

	వరుస_నుండి_తీయి_dl_entity(&p->dl, flags);
	ఒకవేళ (!p->dl.dl_throttled && !dl_server(&p->dl))
		వరుస_నుండి_తీయి_pushable_dl_task(rq, p);

	తిరిగి true;
}

/*
 * Yield task semantic for -deadline tasks is:
 *
 *   get off from the CPU until our next instance, with
 *   a new runtime. This is of little use now, since we
 *   don't have a bandwidth reclaiming mechanism. Anyway,
 *   bandwidth reclaiming is planned for the future, and
 *   yield_task_dl will indicate that some spare budget
 *   is available for other task instances to use it.
 */
స్థిర శూన్య yield_task_dl(నిర్మాణం rq *rq)
{
	/*
	 * We make the task go to sleep until its current deadline by
	 * forcing its runtime to zero. This way, update_curr_dl() stops
	 * it and the bandwidth timer will wake it up and will give it
	 * new scheduling parameters (thanks to dl_yielded=1).
	 */
	rq->donor->dl.dl_yielded = 1;

	update_rq_clock(rq);
	update_curr_dl(rq);
	/*
	 * Tell update_rq_clock() that we've just updated,
	 * so we don't do microscopic update in schedule()
	 * and double the fastpath cost.
	 */
	rq_clock_skip_update(rq);
}

స్థిర అంతఃవరుస bool dl_task_is_earliest_deadline(నిర్మాణం task_struct *p,
						 నిర్మాణం rq *rq)
{
	తిరిగి (!rq->dl.dl_nr_running ||
		dl_సమయం_before(p->dl.deadline,
			       rq->dl.earliest_dl.curr));
}

స్థిర పూర్ణాంకం కనుగొను_later_rq(నిర్మాణం task_struct *task);

స్థిర పూర్ణాంకం
select_task_rq_dl(నిర్మాణం task_struct *p, పూర్ణాంకం cpu, పూర్ణాంకం flags)
{
	నిర్మాణం task_struct *curr, *donor;
	bool select_rq;
	నిర్మాణం rq *rq;

	ఒకవేళ (!(flags & WF_TTWU))
		తిరిగి cpu;

	rq = cpu_rq(cpu);

	rcu_read_తాళం();
	curr = READ_ONCE(rq->curr); /* unlocked access */
	donor = READ_ONCE(rq->donor);

	/*
	 * If we are dealing with a -deadline task, we must
	 * decide where to wake it up.
	 * If it has a later deadline and the current task
	 * on this rq can't move (provided the waking task
	 * can!) we prefer to send it somewhere else. On the
	 * other hand, if it has a shorter deadline, we
	 * try to make it stay here, it might be important.
	 */
	select_rq = unlikely(dl_task(donor)) &&
		    (curr->nr_cpus_allowed < 2 ||
		     !dl_entity_preempt(&p->dl, &donor->dl)) &&
		    p->nr_cpus_allowed > 1;

	/*
	 * Take the capacity of the CPU into account to
	 * ensure it fits the requirement of the task.
	 */
	ఒకవేళ (sched_asym_cpucap_చురుకైన())
		select_rq |= !dl_task_సరిపోతుంది_సామర్థ్యం(p, cpu);

	ఒకవేళ (select_rq) {
		పూర్ణాంకం లక్ష్యం = కనుగొను_later_rq(p);

		ఒకవేళ (లక్ష్యం != -1 &&
		    dl_task_is_earliest_deadline(p, cpu_rq(లక్ష్యం)))
			cpu = లక్ష్యం;
	}
	rcu_read_unlock();

	తిరిగి cpu;
}

స్థిర శూన్య migrate_task_rq_dl(నిర్మాణం task_struct *p, పూర్ణాంకం new_cpu __maybe_unused)
{
	నిర్మాణం rq_flags rf;
	నిర్మాణం rq *rq;

	ఒకవేళ (READ_ONCE(p->__state) != TASK_WAKING)
		తిరిగి;

	rq = task_rq(p);
	/*
	 * Since p->state == TASK_WAKING, set_task_cpu() has been called
	 * from try_to_wake_up(). Hence, p->pi_lock is locked, but
	 * rq->lock is not... So, lock it
	 */
	rq_తాళం(rq, &rf);
	ఒకవేళ (p->dl.dl_non_contending) {
		update_rq_clock(rq);
		sub_running_bw(&p->dl, &rq->dl);
		p->dl.dl_non_contending = 0;
		/*
		 * If the timer handler is currently running and the
		 * timer cannot be canceled, inactive_task_timer()
		 * will see that dl_not_contending is not set, and
		 * will not touch the rq's active utilization,
		 * so we are still safe.
		 */
		cancel_inactive_timer(&p->dl);
	}
	sub_rq_bw(&p->dl, &rq->dl);
	rq_unlock(rq, &rf);
}

స్థిర శూన్య check_preempt_equal_dl(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
	/*
	 * Current can't be migrated, useless to reschedule,
	 * let's hope p can move out.
	 */
	ఒకవేళ (rq->curr->nr_cpus_allowed == 1 ||
	    !cpudl_కనుగొను(&rq->rd->cpudl, rq->donor, ఖాళీ))
		తిరిగి;

	/*
	 * p is migratable, so let's not schedule it and
	 * see if it is pushed or pulled somewhere else.
	 */
	ఒకవేళ (p->nr_cpus_allowed != 1 &&
	    cpudl_కనుగొను(&rq->rd->cpudl, p, ఖాళీ))
		తిరిగి;

	resched_curr(rq);
}

స్థిర పూర్ణాంకం balance_dl(నిర్మాణం rq *rq, నిర్మాణం rq_flags *rf)
{
	/*
	 * Note, rq->donor may change during rq lock drops,
	 * so don't re-use prev across lock drops
	 */
	నిర్మాణం task_struct *p = rq->donor;

	ఒకవేళ (!on_dl_rq(&p->dl) && need_pull_dl_task(rq, p)) {
		/*
		 * This is OK, because current is on_cpu, which avoids it being
		 * picked for load-balance and preemption/IRQs are still
		 * disabled avoiding further scheduler activity on it and we've
		 * not yet started the picking loop.
		 */
		rq_unpin_తాళం(rq, rf);
		pull_dl_task(rq);
		rq_repin_తాళం(rq, rf);
	}

	తిరిగి sched_stop_runnable(rq) || sched_dl_runnable(rq);
}

/*
 * Only called when both the current and waking task are -deadline
 * tasks.
 */
స్థిర శూన్య wakeup_preempt_dl(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, పూర్ణాంకం flags)
{
	నిర్మాణం task_struct *donor = rq->donor;
	/*
	 * Can only get preempted by stop-class, and those should be
	 * few and short lived, doesn't really make sense to push
	 * anything away for that.
	 */
	ఒకవేళ (p->sched_class != &dl_sched_class ||
	    donor->sched_class != &dl_sched_class)
		తిరిగి;

	ఒకవేళ (dl_entity_preempt(&p->dl, &donor->dl)) {
		resched_curr(rq);
		తిరిగి;
	}

	/*
	 * In the unlikely case current and p have the same deadline
	 * let us try to decide what's the best thing to do...
	 */
	ఒకవేళ ((p->dl.deadline == rq->donor->dl.deadline) &&
	    !test_tsk_need_resched(rq->curr))
		check_preempt_equal_dl(rq, p);
}

#నిర్వచితమైతే CONFIG_SCHED_HRTICK
స్థిర శూన్య ప్రారంభం_hrtick_dl(నిర్మాణం rq *rq, నిర్మాణం sched_dl_entity *dl_se)
{
	hrtick_ప్రారంభం(rq, dl_se->runtime);
}
#లేకపోతే /* !CONFIG_SCHED_HRTICK: */
స్థిర శూన్య ప్రారంభం_hrtick_dl(నిర్మాణం rq *rq, నిర్మాణం sched_dl_entity *dl_se)
{
}
#అయితే_ముగింపు /* !CONFIG_SCHED_HRTICK */

/*
 * DL keeps current in tree, because ->deadline is not typically changed while
 * a task is runnable.
 */
స్థిర శూన్య సమితి_తదుపరి_task_dl(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, గణన snt_e type)
{
	నిర్మాణం sched_dl_entity *dl_se = &p->dl;
	నిర్మాణం dl_rq *dl_rq = &rq->dl;

	ఒకవేళ (type == SNT_REPICK)
		తిరిగి;

	p->se.exec_ప్రారంభం = rq_clock_task(rq);
	ఒకవేళ (on_dl_rq(&p->dl))
		update_stats_wait_చివర_dl(dl_rq, dl_se);

	/* You can't push away the running task */
	వరుస_నుండి_తీయి_pushable_dl_task(rq, p);

	WARN_ON_ONCE(dl_rq->curr);
	dl_rq->curr = dl_se;

	ఒకవేళ (type != SNT_PICK)
		తిరిగి;

	ఒకవేళ (rq->donor->sched_class != &dl_sched_class)
		update_dl_rq_ఎక్కించు_avg(rq_clock_pelt(rq), rq, 0);

	deadline_వరుస_నెట్టు_tasks(rq);

	ఒకవేళ (hrtick_enabled_dl(rq))
		ప్రారంభం_hrtick_dl(rq, &p->dl);
}

స్థిర నిర్మాణం sched_dl_entity *pick_తదుపరి_dl_entity(నిర్మాణం dl_rq *dl_rq)
{
	నిర్మాణం rb_కణుపు *ఎడమ = rb_మొదటి_cached(&dl_rq->మూలం);

	ఒకవేళ (!ఎడమ)
		తిరిగి ఖాళీ;

	తిరిగి __node_2_dle(ఎడమ);
}

/*
 * __pick_next_task_dl - Helper to pick the next -deadline task to run.
 * @rq: The runqueue to pick the next task from.
 */
స్థిర నిర్మాణం task_struct *__pick_task_dl(నిర్మాణం rq *rq, నిర్మాణం rq_flags *rf)
{
	నిర్మాణం sched_dl_entity *dl_se;
	నిర్మాణం dl_rq *dl_rq = &rq->dl;
	నిర్మాణం task_struct *p;

again:
	ఒకవేళ (!sched_dl_runnable(rq))
		తిరిగి ఖాళీ;

	dl_se = pick_తదుపరి_dl_entity(dl_rq);
	WARN_ON_ONCE(!dl_se);

	ఒకవేళ (dl_server(dl_se)) {
		p = dl_se->server_pick_task(dl_se, rf);
		ఒకవేళ (!p) {
			dl_server_stop(dl_se);
			వెళ్ళు again;
		}
		rq->dl_server = dl_se;
	} లేకపోతే {
		p = dl_task_యొక్క(dl_se);
	}

	తిరిగి p;
}

స్థిర నిర్మాణం task_struct *pick_task_dl(నిర్మాణం rq *rq, నిర్మాణం rq_flags *rf)
{
	తిరిగి __pick_task_dl(rq, rf);
}

స్థిర శూన్య put_prev_task_dl(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, నిర్మాణం task_struct *తదుపరి)
{
	నిర్మాణం sched_dl_entity *dl_se = &p->dl;
	నిర్మాణం dl_rq *dl_rq = &rq->dl;

	ఒకవేళ (on_dl_rq(dl_se))
		update_stats_wait_ప్రారంభం_dl(dl_rq, dl_se);

	update_curr_dl(rq);

	update_dl_rq_ఎక్కించు_avg(rq_clock_pelt(rq), rq, 1);

	WARN_ON_ONCE(dl_rq->curr != dl_se);
	dl_rq->curr = ఖాళీ;

	ఒకవేళ (task_is_blocked(p))
		తిరిగి;

	ఒకవేళ (on_dl_rq(dl_se) && p->nr_cpus_allowed > 1)
		వరుసలో_చేర్చు_pushable_dl_task(rq, p);
}

/*
 * scheduler tick hitting a task of our scheduling class.
 *
 * NOTE: This function can be called remotely by the tick offload that
 * goes along full dynticks. Therefore no local assumption can be made
 * and everything must be accessed through the @rq and @curr passed in
 * parameters.
 */
స్థిర శూన్య task_tick_dl(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, పూర్ణాంకం queued)
{
	update_curr_dl(rq);

	update_dl_rq_ఎక్కించు_avg(rq_clock_pelt(rq), rq, 1);
	/*
	 * Even when we have runtime, update_curr_dl() might have resulted in us
	 * not being the leftmost task anymore. In that case NEED_RESCHED will
	 * be set and schedule() will start a new hrtick for the next task.
	 */
	ఒకవేళ (hrtick_enabled_dl(rq) && queued && p->dl.runtime > 0 &&
	    is_leftmost(&p->dl, &rq->dl))
		ప్రారంభం_hrtick_dl(rq, &p->dl);
}

స్థిర శూన్య task_fork_dl(నిర్మాణం task_struct *p)
{
	/*
	 * SCHED_DEADLINE tasks cannot fork and this is achieved through
	 * sched_fork()
	 */
}

/* Only try algorithms three times */
#నిర్వచించు DL_MAX_TRIES 3

/*
 * Return the earliest pushable rq's task, which is suitable to be executed
 * on the CPU, NULL otherwise:
 */
స్థిర నిర్మాణం task_struct *pick_earliest_pushable_dl_task(నిర్మాణం rq *rq, పూర్ణాంకం cpu)
{
	నిర్మాణం task_struct *p = ఖాళీ;
	నిర్మాణం rb_కణుపు *తదుపరి_కణుపు;

	ఒకవేళ (!has_pushable_dl_tasks(rq))
		తిరిగి ఖాళీ;

	తదుపరి_కణుపు = rb_మొదటి_cached(&rq->dl.pushable_dl_tasks_మూలం);
	అయితే (తదుపరి_కణుపు) {
		p = __node_2_pdl(తదుపరి_కణుపు);

		ఒకవేళ (task_is_pushable(rq, p, cpu))
			తిరిగి p;

		తదుపరి_కణుపు = rb_తదుపరి(తదుపరి_కణుపు);
	}

	తిరిగి ఖాళీ;
}

/* Access rule: must be called on local CPU with preemption disabled */
స్థిర DEFINE_చొప్పున_CPU(cpumask_var_t, local_cpu_ముసుగు_dl);

స్థిర పూర్ణాంకం కనుగొను_later_rq(నిర్మాణం task_struct *task)
{
	నిర్మాణం sched_domain *sd;
	నిర్మాణం cpumask *later_ముసుగు = this_cpu_cpumask_var_ptr(local_cpu_ముసుగు_dl);
	పూర్ణాంకం this_cpu = smp_processor_id();
	పూర్ణాంకం cpu = task_cpu(task);

	/* Make sure the mask is initialized first */
	ఒకవేళ (unlikely(!later_ముసుగు))
		తిరిగి -1;

	ఒకవేళ (task->nr_cpus_allowed == 1)
		తిరిగి -1;

	/*
	 * We have to consider system topology and task affinity
	 * first, then we can look for a suitable CPU.
	 */
	ఒకవేళ (!cpudl_కనుగొను(&task_rq(task)->rd->cpudl, task, later_ముసుగు))
		తిరిగి -1;

	/*
	 * If we are here, some targets have been found, including
	 * the most suitable which is, among the runqueues where the
	 * current tasks have later deadlines than the task's one, the
	 * rq with the latest possible one.
	 *
	 * Now we check how well this matches with task's
	 * affinity and system topology.
	 *
	 * The last CPU where the task run is our first
	 * guess, since it is most likely cache-hot there.
	 */
	ఒకవేళ (cpumask_test_cpu(cpu, later_ముసుగు))
		తిరిగి cpu;
	/*
	 * Check if this_cpu is to be skipped (i.e., it is
	 * not in the mask) or not.
	 */
	ఒకవేళ (!cpumask_test_cpu(this_cpu, later_ముసుగు))
		this_cpu = -1;

	rcu_read_తాళం();
	for_each_domain(cpu, sd) {
		ఒకవేళ (sd->flags & SD_WAKE_AFFINE) {
			పూర్ణాంకం ఉత్తమ_cpu;

			/*
			 * If possible, preempting this_cpu is
			 * cheaper than migrating.
			 */
			ఒకవేళ (this_cpu != -1 &&
			    cpumask_test_cpu(this_cpu, sched_domain_పరిధి(sd))) {
				rcu_read_unlock();
				తిరిగి this_cpu;
			}

			ఉత్తమ_cpu = cpumask_any_and_distribute(later_ముసుగు,
							      sched_domain_పరిధి(sd));
			/*
			 * Last chance: if a CPU being in both later_mask
			 * and current sd span is valid, that becomes our
			 * choice. Of course, the latest possible CPU is
			 * already under consideration through later_mask.
			 */
			ఒకవేళ (ఉత్తమ_cpu < nr_cpu_ids) {
				rcu_read_unlock();
				తిరిగి ఉత్తమ_cpu;
			}
		}
	}
	rcu_read_unlock();

	/*
	 * At this point, all our guesses failed, we just return
	 * 'something', and let the caller sort the things out.
	 */
	ఒకవేళ (this_cpu != -1)
		తిరిగి this_cpu;

	cpu = cpumask_any_distribute(later_ముసుగు);
	ఒకవేళ (cpu < nr_cpu_ids)
		తిరిగి cpu;

	తిరిగి -1;
}

స్థిర నిర్మాణం task_struct *pick_తదుపరి_pushable_dl_task(నిర్మాణం rq *rq)
{
	నిర్మాణం task_struct *i, *p = ఖాళీ;
	నిర్మాణం rb_కణుపు *తదుపరి_కణుపు;

	ఒకవేళ (!has_pushable_dl_tasks(rq))
		తిరిగి ఖాళీ;

	తదుపరి_కణుపు = rb_మొదటి_cached(&rq->dl.pushable_dl_tasks_మూలం);
	అయితే (తదుపరి_కణుపు) {
		i = __node_2_pdl(తదుపరి_కణుపు);
		/* skip tasks that cannot be migrated */
		ఒకవేళ (!task_on_cpu(rq, i) && !is_migration_disabled(i)) {
			p = i;
			ఆపు;
		}

		తదుపరి_కణుపు = rb_తదుపరి(తదుపరి_కణుపు);
	}

	ఒకవేళ (!p)
		తిరిగి ఖాళీ;

	WARN_ON_ONCE(rq->cpu != task_cpu(p));
	WARN_ON_ONCE(task_current(rq, p));
	WARN_ON_ONCE(p->nr_cpus_allowed <= 1);

	WARN_ON_ONCE(!task_on_rq_queued(p));
	WARN_ON_ONCE(!dl_task(p));

	తిరిగి p;
}

/* Locks the rq it finds */
స్థిర నిర్మాణం rq *కనుగొను_తాళం_later_rq(నిర్మాణం task_struct *task, నిర్మాణం rq *rq)
{
	నిర్మాణం rq *later_rq = ఖాళీ;
	పూర్ణాంకం tries;
	పూర్ణాంకం cpu;

	కొరకు (tries = 0; tries < DL_MAX_TRIES; tries++) {
		cpu = కనుగొను_later_rq(task);

		ఒకవేళ ((cpu == -1) || (cpu == rq->cpu))
			ఆపు;

		later_rq = cpu_rq(cpu);

		ఒకవేళ (!dl_task_is_earliest_deadline(task, later_rq)) {
			/*
			 * Target rq has tasks of equal or earlier deadline,
			 * retrying does not release any lock and is unlikely
			 * to yield a different result.
			 */
			later_rq = ఖాళీ;
			ఆపు;
		}

		/* Retry if something changed. */
		ఒకవేళ (double_తాళం_balance(rq, later_rq)) {
			/*
			 * double_lock_balance had to release rq->lock, in the
			 * meantime, task may no longer be fit to be migrated.
			 * Check the following to ensure that the task is
			 * still suitable for migration:
			 * 1. It is possible the task was scheduled,
			 *    migrate_disabled was set and then got preempted,
			 *    so we must check the task migration disable
			 *    flag.
			 * 2. The CPU picked is in the task's affinity.
			 * 3. For throttled task (dl_task_offline_migration),
			 *    check the following:
			 *    - the task is not on the rq anymore (it was
			 *      migrated)
			 *    - the task is not on CPU anymore
			 *    - the task is still a dl task
			 *    - the task is not queued on the rq anymore
			 * 4. For the non-throttled task (push_dl_task), the
			 *    check to ensure that this task is still at the
			 *    head of the pushable tasks list is enough.
			 */
			ఒకవేళ (unlikely(is_migration_disabled(task) ||
				     !cpumask_test_cpu(later_rq->cpu, &task->cpus_ముసుగు) ||
				     (task->dl.dl_throttled &&
				      (task_rq(task) != rq ||
				       task_on_cpu(rq, task) ||
				       !dl_task(task) ||
				       !task_on_rq_queued(task))) ||
				     (!task->dl.dl_throttled &&
				      task != pick_తదుపరి_pushable_dl_task(rq)))) {

				double_unlock_balance(rq, later_rq);
				later_rq = ఖాళీ;
				ఆపు;
			}
		}

		/*
		 * If the rq we found has no -deadline task, or
		 * its earliest one has a later deadline than our
		 * task, the rq is a good one.
		 */
		ఒకవేళ (dl_task_is_earliest_deadline(task, later_rq))
			ఆపు;

		/* Otherwise we try again. */
		double_unlock_balance(rq, later_rq);
		later_rq = ఖాళీ;
	}

	తిరిగి later_rq;
}

/*
 * See if the non running -deadline tasks on this rq
 * can be sent to some other CPU where they can preempt
 * and start executing.
 */
స్థిర పూర్ణాంకం నెట్టు_dl_task(నిర్మాణం rq *rq)
{
	నిర్మాణం task_struct *తదుపరి_task;
	నిర్మాణం rq *later_rq;
	పూర్ణాంకం ret = 0;

	తదుపరి_task = pick_తదుపరి_pushable_dl_task(rq);
	ఒకవేళ (!తదుపరి_task)
		తిరిగి 0;

retry:
	/*
	 * If next_task preempts rq->curr, and rq->curr
	 * can move away, it makes sense to just reschedule
	 * without going further in pushing next_task.
	 */
	ఒకవేళ (dl_task(rq->donor) &&
	    dl_సమయం_before(తదుపరి_task->dl.deadline, rq->donor->dl.deadline) &&
	    rq->curr->nr_cpus_allowed > 1) {
		resched_curr(rq);
		తిరిగి 0;
	}

	ఒకవేళ (is_migration_disabled(తదుపరి_task))
		తిరిగి 0;

	ఒకవేళ (WARN_ON(తదుపరి_task == rq->curr))
		తిరిగి 0;

	/* We might release rq lock */
	get_task_struct(తదుపరి_task);

	/* Will lock the rq it'll find */
	later_rq = కనుగొను_తాళం_later_rq(తదుపరి_task, rq);
	ఒకవేళ (!later_rq) {
		నిర్మాణం task_struct *task;

		/*
		 * We must check all this again, since
		 * find_lock_later_rq releases rq->lock and it is
		 * then possible that next_task has migrated.
		 */
		task = pick_తదుపరి_pushable_dl_task(rq);
		ఒకవేళ (task == తదుపరి_task) {
			/*
			 * The task is still there. We don't try
			 * again, some other CPU will pull it when ready.
			 */
			వెళ్ళు బహిర్గతం;
		}

		ఒకవేళ (!task)
			/* No more tasks */
			వెళ్ళు బహిర్గతం;

		put_task_struct(తదుపరి_task);
		తదుపరి_task = task;
		వెళ్ళు retry;
	}

	move_queued_task_locked(rq, later_rq, తదుపరి_task);
	ret = 1;

	resched_curr(later_rq);

	double_unlock_balance(rq, later_rq);

బహిర్గతం:
	put_task_struct(తదుపరి_task);

	తిరిగి ret;
}

స్థిర శూన్య నెట్టు_dl_tasks(నిర్మాణం rq *rq)
{
	/* push_dl_task() will return true if it moved a -deadline task */
	అయితే (నెట్టు_dl_task(rq))
		;
}

స్థిర శూన్య pull_dl_task(నిర్మాణం rq *this_rq)
{
	పూర్ణాంకం this_cpu = this_rq->cpu, cpu;
	నిర్మాణం task_struct *p, *నెట్టు_task;
	bool resched = false;
	నిర్మాణం rq *src_rq;
	u64 dmin = LONG_MAX;

	ఒకవేళ (likely(!dl_overloaded(this_rq)))
		తిరిగి;

	/*
	 * Match the barrier from dl_set_overloaded; this guarantees that if we
	 * see overloaded we must also see the dlo_mask bit.
	 */
	smp_rmb();

	for_each_cpu(cpu, this_rq->rd->dlo_ముసుగు) {
		ఒకవేళ (this_cpu == cpu)
			కొనసాగు;

		src_rq = cpu_rq(cpu);

		/*
		 * It looks racy, and it is! However, as in sched_rt.c,
		 * we are fine with this.
		 */
		ఒకవేళ (this_rq->dl.dl_nr_running &&
		    dl_సమయం_before(this_rq->dl.earliest_dl.curr,
				   src_rq->dl.earliest_dl.తదుపరి))
			కొనసాగు;

		/* Might drop this_rq->lock */
		నెట్టు_task = ఖాళీ;
		double_తాళం_balance(this_rq, src_rq);

		/*
		 * If there are no more pullable tasks on the
		 * rq, we're done with it.
		 */
		ఒకవేళ (src_rq->dl.dl_nr_running <= 1)
			వెళ్ళు skip;

		p = pick_earliest_pushable_dl_task(src_rq, this_cpu);

		/*
		 * We found a task to be pulled if:
		 *  - it preempts our current (if there's one),
		 *  - it will preempt the last one we pulled (if any).
		 */
		ఒకవేళ (p && dl_సమయం_before(p->dl.deadline, dmin) &&
		    dl_task_is_earliest_deadline(p, this_rq)) {
			WARN_ON(p == src_rq->curr);
			WARN_ON(!task_on_rq_queued(p));

			/*
			 * Then we pull iff p has actually an earlier
			 * deadline than the current task of its runqueue.
			 */
			ఒకవేళ (dl_సమయం_before(p->dl.deadline,
					   src_rq->donor->dl.deadline))
				వెళ్ళు skip;

			ఒకవేళ (is_migration_disabled(p)) {
				నెట్టు_task = get_నెట్టు_task(src_rq);
			} లేకపోతే {
				move_queued_task_locked(src_rq, this_rq, p);
				dmin = p->dl.deadline;
				resched = true;
			}

			/* Is there any other task even earlier? */
		}
skip:
		double_unlock_balance(this_rq, src_rq);

		ఒకవేళ (నెట్టు_task) {
			preempt_disable();
			raw_spin_rq_unlock(this_rq);
			stop_one_cpu_nowait(src_rq->cpu, నెట్టు_cpu_stop,
					    నెట్టు_task, &src_rq->నెట్టు_work);
			preempt_enable();
			raw_spin_rq_తాళం(this_rq);
		}
	}

	ఒకవేళ (resched)
		resched_curr(this_rq);
}

/*
 * Since the task is not running and a reschedule is not going to happen
 * anytime soon on its runqueue, we try pushing it away now.
 */
స్థిర శూన్య task_woken_dl(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
	ఒకవేళ (!task_on_cpu(rq, p) &&
	    !test_tsk_need_resched(rq->curr) &&
	    p->nr_cpus_allowed > 1 &&
	    dl_task(rq->donor) &&
	    (rq->curr->nr_cpus_allowed < 2 ||
	     !dl_entity_preempt(&p->dl, &rq->donor->dl))) {
		నెట్టు_dl_tasks(rq);
	}
}

స్థిర శూన్య సమితి_cpus_allowed_dl(నిర్మాణం task_struct *p,
				నిర్మాణం affinity_context *ctx)
{
	నిర్మాణం rq *rq;

	WARN_ON_ONCE(!dl_task(p));

	rq = task_rq(p);
	/*
	 * Migrating a SCHED_DEADLINE task between exclusive
	 * cpusets (different root_domains) entails a bandwidth
	 * update. We already made space for us in the destination
	 * domain (see cpuset_can_attach()).
	 */
	ఒకవేళ (dl_task_needs_bw_move(p, ctx->new_ముసుగు)) {
		నిర్మాణం dl_bw *src_dl_b;

		src_dl_b = dl_bw_యొక్క(cpu_యొక్క(rq));
		/*
		 * We now free resources of the root_domain we are migrating
		 * off. In the worst case, sched_setattr() may temporary fail
		 * until we complete the update.
		 */
		raw_spin_తాళం(&src_dl_b->తాళం);
		__dl_sub(src_dl_b, p->dl.dl_bw, dl_bw_cpus(task_cpu(p)));
		raw_spin_unlock(&src_dl_b->తాళం);
	}

	సమితి_cpus_allowed_common(p, ctx);
}

bool dl_task_needs_bw_move(నిర్మాణం task_struct *p,
			   స్థిరరాశి నిర్మాణం cpumask *new_ముసుగు)
{
	ఒకవేళ (!dl_task(p))
		తిరిగి false;

	తిరిగి !cpumask_intersects(task_rq(p)->rd->పరిధి, new_ముసుగు);
}

/* Assumes rq->lock is held */
స్థిర శూన్య rq_online_dl(నిర్మాణం rq *rq)
{
	ఒకవేళ (rq->dl.overloaded)
		dl_సమితి_overload(rq);

	ఒకవేళ (rq->dl.dl_nr_running > 0)
		cpudl_సమితి(&rq->rd->cpudl, rq->cpu, rq->dl.earliest_dl.curr);
	లేకపోతే
		cpudl_clear(&rq->rd->cpudl, rq->cpu, true);
}

/* Assumes rq->lock is held */
స్థిర శూన్య rq_offline_dl(నిర్మాణం rq *rq)
{
	ఒకవేళ (rq->dl.overloaded)
		dl_clear_overload(rq);

	cpudl_clear(&rq->rd->cpudl, rq->cpu, false);
}

శూన్య __init init_sched_dl_class(శూన్య)
{
	గుర్తులేని పూర్ణాంకం i;

	for_each_possible_cpu(i)
		zalloc_cpumask_var_కణుపు(&చొప్పున_cpu(local_cpu_ముసుగు_dl, i),
					GFP_KERNEL, cpu_గమ్యం_కణుపు(i));
}

/*
 * This function always returns a non-empty bitmap in @cpus. This is because
 * if a root domain has reserved bandwidth for DL tasks, the DL bandwidth
 * check will prevent CPU hotplug from deactivating all CPUs in that domain.
 */
స్థిర శూన్య dl_get_task_effective_cpus(నిర్మాణం task_struct *p, నిర్మాణం cpumask *cpus)
{
	స్థిరరాశి నిర్మాణం cpumask *hk_msk;

	hk_msk = housekeeping_cpumask(HK_TYPE_DOMAIN);
	ఒకవేళ (housekeeping_enabled(HK_TYPE_DOMAIN)) {
		ఒకవేళ (!cpumask_intersects(p->cpus_ptr, hk_msk)) {
			/*
			 * CPUs isolated by isolcpu="domain" always belong to
			 * def_root_domain.
			 */
			cpumask_andnot(cpus, cpu_చురుకైన_ముసుగు, hk_msk);
			తిరిగి;
		}
	}

	/*
	 * If a root domain holds a DL task, it must have active CPUs. So
	 * active CPUs can always be found by walking up the task's cpuset
	 * hierarchy up to the partition root.
	 */
	cpuset_cpus_allowed_locked(p, cpus);
}

/* The caller should hold cpuset_mutex */
శూన్య dl_కూడు_task_మూలం_domain(నిర్మాణం task_struct *p)
{
	నిర్మాణం rq_flags rf;
	నిర్మాణం rq *rq;
	నిర్మాణం dl_bw *dl_b;
	గుర్తులేని పూర్ణాంకం cpu;
	నిర్మాణం cpumask *msk;

	raw_spin_తాళం_irqsave(&p->pi_తాళం, rf.flags);
	ఒకవేళ (!dl_task(p) || dl_entity_is_special(&p->dl)) {
		raw_spin_unlock_irqrestore(&p->pi_తాళం, rf.flags);
		తిరిగి;
	}

	msk = this_cpu_cpumask_var_ptr(local_cpu_ముసుగు_dl);
	dl_get_task_effective_cpus(p, msk);
	cpu = cpumask_మొదటి_and(cpu_చురుకైన_ముసుగు, msk);
	BUG_ON(cpu >= nr_cpu_ids);
	rq = cpu_rq(cpu);
	dl_b = &rq->rd->dl_bw;

	raw_spin_తాళం(&dl_b->తాళం);
	__dl_add(dl_b, p->dl.dl_bw, cpumask_బరువు(rq->rd->పరిధి));
	raw_spin_unlock(&dl_b->తాళం);
	raw_spin_unlock_irqrestore(&p->pi_తాళం, rf.flags);
}

స్థిర శూన్య dl_server_కూడు_bw(నిర్మాణం మూలం_domain *rd, పూర్ణాంకం cpu)
{
	నిర్మాణం sched_dl_entity *dl_se;

	dl_se = &cpu_rq(cpu)->fair_server;
	ఒకవేళ (dl_server(dl_se) && dl_se->dl_bw_attached && cpu_చురుకైన(cpu))
		__dl_add(&rd->dl_bw, dl_se->dl_bw, dl_bw_cpus(cpu));

#నిర్వచితమైతే CONFIG_SCHED_CLASS_EXT
	dl_se = &cpu_rq(cpu)->ext_server;
	ఒకవేళ (dl_server(dl_se) && dl_se->dl_bw_attached && cpu_చురుకైన(cpu))
		__dl_add(&rd->dl_bw, dl_se->dl_bw, dl_bw_cpus(cpu));
#అయితే_ముగింపు
}

స్థిర u64 dl_server_read_bw(పూర్ణాంకం cpu)
{
	u64 dl_bw = 0;

	ఒకవేళ (cpu_rq(cpu)->fair_server.dl_server &&
	    cpu_rq(cpu)->fair_server.dl_bw_attached)
		dl_bw += cpu_rq(cpu)->fair_server.dl_bw;

#నిర్వచితమైతే CONFIG_SCHED_CLASS_EXT
	ఒకవేళ (cpu_rq(cpu)->ext_server.dl_server &&
	    cpu_rq(cpu)->ext_server.dl_bw_attached)
		dl_bw += cpu_rq(cpu)->ext_server.dl_bw;
#అయితే_ముగింపు

	తిరిగి dl_bw;
}

శూన్య dl_clear_మూలం_domain(నిర్మాణం మూలం_domain *rd)
{
	పూర్ణాంకం i;

	guard(raw_spinlock_irqsave)(&rd->dl_bw.తాళం);

	/*
	 * Reset total_bw to zero and extra_bw to max_bw so that next
	 * loop will add dl-servers contributions back properly,
	 */
	rd->dl_bw.మొత్తం_bw = 0;
	for_each_cpu(i, rd->పరిధి)
		cpu_rq(i)->dl.extra_bw = cpu_rq(i)->dl.max_bw;

	/*
	 * dl_servers are not tasks. Since dl_add_task_root_domain ignores
	 * them, we need to account for them here explicitly.
	 */
	for_each_cpu(i, rd->పరిధి)
		dl_server_కూడు_bw(rd, i);
}

శూన్య dl_clear_మూలం_domain_cpu(పూర్ణాంకం cpu)
{
	dl_clear_మూలం_domain(cpu_rq(cpu)->rd);
}

స్థిర శూన్య switched_from_dl(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
	/*
	 * task_non_contending() can start the "inactive timer" (if the 0-lag
	 * time is in the future). If the task switches back to dl before
	 * the "inactive timer" fires, it can continue to consume its current
	 * runtime using its current deadline. If it stays outside of
	 * SCHED_DEADLINE until the 0-lag time passes, inactive_task_timer()
	 * will reset the task parameters.
	 */
	ఒకవేళ (task_on_rq_queued(p) && p->dl.dl_runtime)
		task_non_contending(&p->dl, false);

	/*
	 * In case a task is setscheduled out from SCHED_DEADLINE we need to
	 * keep track of that on its cpuset (for correct bandwidth tracking).
	 */
	dec_dl_tasks_cs(p);

	ఒకవేళ (!task_on_rq_queued(p)) {
		/*
		 * Inactive timer is armed. However, p is leaving DEADLINE and
		 * might migrate away from this rq while continuing to run on
		 * some other class. We need to remove its contribution from
		 * this rq running_bw now, or sub_rq_bw (below) will complain.
		 */
		ఒకవేళ (p->dl.dl_non_contending)
			sub_running_bw(&p->dl, &rq->dl);
		sub_rq_bw(&p->dl, &rq->dl);
	}

	/*
	 * We cannot use inactive_task_timer() to invoke sub_running_bw()
	 * at the 0-lag time, because the task could have been migrated
	 * while SCHED_OTHER in the meanwhile.
	 */
	ఒకవేళ (p->dl.dl_non_contending)
		p->dl.dl_non_contending = 0;

	/*
	 * Since this might be the only -deadline task on the rq,
	 * this is the right place to try to pull some other one
	 * from an overloaded CPU, if any.
	 */
	ఒకవేళ (!task_on_rq_queued(p) || rq->dl.dl_nr_running)
		తిరిగి;

	deadline_వరుస_pull_task(rq);
}

/*
 * When switching to -deadline, we may overload the rq, then
 * we try to push someone off, if possible.
 */
స్థిర శూన్య switched_గమ్యం_dl(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
	cancel_inactive_timer(&p->dl);

	/*
	 * In case a task is setscheduled to SCHED_DEADLINE we need to keep
	 * track of that on its cpuset (for correct bandwidth tracking).
	 */
	inc_dl_tasks_cs(p);

	/* If p is not queued we will update its parameters at next wakeup. */
	ఒకవేళ (!task_on_rq_queued(p)) {
		కూడు_rq_bw(&p->dl, &rq->dl);

		తిరిగి;
	}

	ఒకవేళ (rq->donor != p) {
		ఒకవేళ (p->nr_cpus_allowed > 1 && rq->dl.overloaded)
			deadline_వరుస_నెట్టు_tasks(rq);
		ఒకవేళ (dl_task(rq->donor))
			wakeup_preempt_dl(rq, p, 0);
		లేకపోతే
			resched_curr(rq);
	} లేకపోతే {
		update_dl_rq_ఎక్కించు_avg(rq_clock_pelt(rq), rq, 0);
	}
}

స్థిర u64 get_prio_dl(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
	/*
	 * Make sure to update current so we don't return a stale value.
	 */
	ఒకవేళ (task_current_donor(rq, p))
		update_curr_dl(rq);

	తిరిగి p->dl.deadline;
}

/*
 * If the scheduling parameters of a -deadline task changed,
 * a push or pull operation might be needed.
 */
స్థిర శూన్య prio_మారింది_dl(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, u64 old_deadline)
{
	ఒకవేళ (!task_on_rq_queued(p))
		తిరిగి;

	ఒకవేళ (p->dl.deadline == old_deadline)
		తిరిగి;

	ఒకవేళ (dl_సమయం_before(old_deadline, p->dl.deadline))
		deadline_వరుస_pull_task(rq);

	ఒకవేళ (task_current_donor(rq, p)) {
		/*
		 * If we now have a earlier deadline task than p,
		 * then reschedule, provided p is still on this
		 * runqueue.
		 */
		ఒకవేళ (dl_సమయం_before(rq->dl.earliest_dl.curr, p->dl.deadline))
			resched_curr(rq);
	} లేకపోతే {
		/*
		 * Current may not be deadline in case p was throttled but we
		 * have just replenished it (e.g. rt_mutex_setprio()).
		 *
		 * Otherwise, if p was given an earlier deadline, reschedule.
		 */
		ఒకవేళ (!dl_task(rq->curr) ||
		    dl_సమయం_before(p->dl.deadline, rq->curr->dl.deadline))
			resched_curr(rq);
	}
}

#నిర్వచితమైతే CONFIG_SCHED_CORE
స్థిర పూర్ణాంకం task_is_throttled_dl(నిర్మాణం task_struct *p, పూర్ణాంకం cpu)
{
	తిరిగి p->dl.dl_throttled;
}
#అయితే_ముగింపు

DEFINE_SCHED_CLASS(dl) = {
	.వరుసలో_చేర్చు_task		= వరుసలో_చేర్చు_task_dl,
	.వరుస_నుండి_తీయి_task		= వరుస_నుండి_తీయి_task_dl,
	.yield_task		= yield_task_dl,

	.wakeup_preempt		= wakeup_preempt_dl,

	.pick_task		= pick_task_dl,
	.put_prev_task		= put_prev_task_dl,
	.సమితి_తదుపరి_task		= సమితి_తదుపరి_task_dl,

	.balance		= balance_dl,
	.select_task_rq		= select_task_rq_dl,
	.migrate_task_rq	= migrate_task_rq_dl,
	.సమితి_cpus_allowed       = సమితి_cpus_allowed_dl,
	.rq_online              = rq_online_dl,
	.rq_offline             = rq_offline_dl,
	.task_woken		= task_woken_dl,
	.కనుగొను_తాళం_rq		= కనుగొను_తాళం_later_rq,

	.task_tick		= task_tick_dl,
	.task_fork              = task_fork_dl,

	.get_prio		= get_prio_dl,
	.prio_మారింది           = prio_మారింది_dl,
	.switched_from		= switched_from_dl,
	.switched_గమ్యం		= switched_గమ్యం_dl,

	.update_curr		= update_curr_dl,
#నిర్వచితమైతే CONFIG_SCHED_CORE
	.task_is_throttled	= task_is_throttled_dl,
#అయితే_ముగింపు
};

/*
 * Used for dl_bw check and update, used under sched_rt_handler()::mutex and
 * sched_domains_mutex.
 */
u64 dl_cookie;

పూర్ణాంకం sched_dl_global_validate(శూన్య)
{
	u64 runtime = global_rt_runtime();
	u64 period = global_rt_period();
	u64 new_bw = గమ్యం_నిష్పత్తి(period, runtime);
	u64 cookie = ++dl_cookie;
	నిర్మాణం dl_bw *dl_b;
	పూర్ణాంకం cpu, cpus, ret = 0;
	గుర్తులేని దీర్ఘ flags;

	/*
	 * Here we want to check the bandwidth not being set to some
	 * value smaller than the currently allocated bandwidth in
	 * any of the root_domains.
	 */
	for_each_online_cpu(cpu) {
		rcu_read_తాళం_sched();

		ఒకవేళ (dl_bw_సందర్శించిన(cpu, cookie))
			వెళ్ళు తదుపరి;

		dl_b = dl_bw_యొక్క(cpu);
		cpus = dl_bw_cpus(cpu);

		raw_spin_తాళం_irqsave(&dl_b->తాళం, flags);
		ఒకవేళ (new_bw * cpus < dl_b->మొత్తం_bw)
			ret = -EBUSY;
		raw_spin_unlock_irqrestore(&dl_b->తాళం, flags);

తదుపరి:
		rcu_read_unlock_sched();

		ఒకవేళ (ret)
			ఆపు;
	}

	తిరిగి ret;
}

స్థిర శూన్య init_dl_rq_bw_నిష్పత్తి(నిర్మాణం dl_rq *dl_rq)
{
	ఒకవేళ (global_rt_runtime() == RUNTIME_INF) {
		dl_rq->bw_నిష్పత్తి = 1 << నిష్పత్తి_జరుపు;
		dl_rq->max_bw = dl_rq->extra_bw = 1 << BW_జరుపు;
	} లేకపోతే {
		dl_rq->bw_నిష్పత్తి = గమ్యం_నిష్పత్తి(global_rt_runtime(),
			  global_rt_period()) >> (BW_జరుపు - నిష్పత్తి_జరుపు);
		dl_rq->max_bw = dl_rq->extra_bw =
			గమ్యం_నిష్పత్తి(global_rt_period(), global_rt_runtime());
	}
}

శూన్య sched_dl_do_global(శూన్య)
{
	u64 new_bw = -1;
	u64 cookie = ++dl_cookie;
	నిర్మాణం dl_bw *dl_b;
	పూర్ణాంకం cpu;
	గుర్తులేని దీర్ఘ flags;

	ఒకవేళ (global_rt_runtime() != RUNTIME_INF)
		new_bw = గమ్యం_నిష్పత్తి(global_rt_period(), global_rt_runtime());

	for_each_possible_cpu(cpu)
		init_dl_rq_bw_నిష్పత్తి(&cpu_rq(cpu)->dl);

	for_each_possible_cpu(cpu) {
		rcu_read_తాళం_sched();

		ఒకవేళ (dl_bw_సందర్శించిన(cpu, cookie)) {
			rcu_read_unlock_sched();
			కొనసాగు;
		}

		dl_b = dl_bw_యొక్క(cpu);

		raw_spin_తాళం_irqsave(&dl_b->తాళం, flags);
		dl_b->bw = new_bw;
		raw_spin_unlock_irqrestore(&dl_b->తాళం, flags);

		rcu_read_unlock_sched();
	}
}

/*
 * We must be sure that accepting a new task (or allowing changing the
 * parameters of an existing one) is consistent with the bandwidth
 * constraints. If yes, this function also accordingly updates the currently
 * allocated bandwidth to reflect the new situation.
 *
 * This function is called while holding p's rq->lock.
 */
పూర్ణాంకం sched_dl_అధికప్రవాహం(నిర్మాణం task_struct *p, పూర్ణాంకం policy,
		      స్థిరరాశి నిర్మాణం sched_attr *attr)
{
	u64 period = attr->sched_period ?: attr->sched_deadline;
	u64 runtime = attr->sched_runtime;
	u64 new_bw = dl_policy(policy) ? గమ్యం_నిష్పత్తి(period, runtime) : 0;
	పూర్ణాంకం cpus, err = -1, cpu = task_cpu(p);
	నిర్మాణం dl_bw *dl_b = dl_bw_యొక్క(cpu);
	గుర్తులేని దీర్ఘ cap;

	ఒకవేళ (attr->sched_flags & SCHED_FLAG_SUGOV)
		తిరిగి 0;

	/* !deadline task may carry old deadline bandwidth */
	ఒకవేళ (new_bw == p->dl.dl_bw && task_has_dl_policy(p))
		తిరిగి 0;

	/*
	 * Either if a task, enters, leave, or stays -deadline but changes
	 * its parameters, we may need to update accordingly the total
	 * allocated bandwidth of the container.
	 */
	raw_spin_తాళం(&dl_b->తాళం);
	cpus = dl_bw_cpus(cpu);
	cap = dl_bw_సామర్థ్యం(cpu);

	ఒకవేళ (dl_policy(policy) && !task_has_dl_policy(p) &&
	    !__dl_overflow(dl_b, cap, 0, new_bw)) {
		ఒకవేళ (hrtimer_చురుకైన(&p->dl.inactive_timer))
			__dl_sub(dl_b, p->dl.dl_bw, cpus);
		__dl_add(dl_b, new_bw, cpus);
		err = 0;
	} లేకపోతే ఒకవేళ (dl_policy(policy) && task_has_dl_policy(p) &&
		   !__dl_overflow(dl_b, cap, p->dl.dl_bw, new_bw)) {
		/*
		 * XXX this is slightly incorrect: when the task
		 * utilization decreases, we should delay the total
		 * utilization change until the task's 0-lag point.
		 * But this would require to set the task's "inactive
		 * timer" when the task is not inactive.
		 */
		__dl_sub(dl_b, p->dl.dl_bw, cpus);
		__dl_add(dl_b, new_bw, cpus);
		dl_change_utilization(p, new_bw);
		err = 0;
	} లేకపోతే ఒకవేళ (!dl_policy(policy) && task_has_dl_policy(p)) {
		/*
		 * Do not decrease the total deadline utilization here,
		 * switched_from_dl() will take care to do it at the correct
		 * (0-lag) time.
		 */
		err = 0;
	}
	raw_spin_unlock(&dl_b->తాళం);

	తిరిగి err;
}

/*
 * This function initializes the sched_dl_entity of a newly becoming
 * SCHED_DEADLINE task.
 *
 * Only the static values are considered here, the actual runtime and the
 * absolute deadline will be properly calculated when the task is enqueued
 * for the first time with its new policy.
 */
శూన్య __setparam_dl(నిర్మాణం task_struct *p, స్థిరరాశి నిర్మాణం sched_attr *attr)
{
	నిర్మాణం sched_dl_entity *dl_se = &p->dl;

	dl_se->dl_runtime = attr->sched_runtime;
	dl_se->dl_deadline = attr->sched_deadline;
	dl_se->dl_period = attr->sched_period ?: dl_se->dl_deadline;
	dl_se->flags = attr->sched_flags & SCHED_DL_FLAGS;
	dl_se->dl_bw = గమ్యం_నిష్పత్తి(dl_se->dl_period, dl_se->dl_runtime);
	dl_se->dl_density = గమ్యం_నిష్పత్తి(dl_se->dl_deadline, dl_se->dl_runtime);
}

శూన్య __getparam_dl(నిర్మాణం task_struct *p, నిర్మాణం sched_attr *attr, గుర్తులేని పూర్ణాంకం flags)
{
	నిర్మాణం sched_dl_entity *dl_se = &p->dl;
	నిర్మాణం rq *rq = task_rq(p);
	u64 adj_deadline;

	attr->sched_ప్రాధాన్యత = p->rt_ప్రాధాన్యత;
	ఒకవేళ (flags & SCHED_GETATTR_FLAG_DL_DYNAMIC) {
		guard(raw_spinlock_irq)(&rq->__lock);
		update_rq_clock(rq);
		ఒకవేళ (task_current(rq, p))
			update_curr_dl(rq);

		attr->sched_runtime = dl_se->runtime;
		adj_deadline = dl_se->deadline - rq_clock(rq) + ktime_get_ns();
		attr->sched_deadline = adj_deadline;
	} లేకపోతే {
		attr->sched_runtime = dl_se->dl_runtime;
		attr->sched_deadline = dl_se->dl_deadline;
	}
	attr->sched_period = dl_se->dl_period;
	attr->sched_flags &= ~SCHED_DL_FLAGS;
	attr->sched_flags |= dl_se->flags;
}

/*
 * This function validates the new parameters of a -deadline task.
 * We ask for the deadline not being zero, and greater or equal
 * than the runtime, as well as the period of being zero or
 * greater than deadline. Furthermore, we have to be sure that
 * user parameters are above the internal resolution of 1us (we
 * check sched_runtime only since it is always the smaller one) and
 * below 2^63 ns (we have to check both sched_deadline and
 * sched_period, as the latter can be zero).
 */
bool __checkparam_dl(స్థిరరాశి నిర్మాణం sched_attr *attr)
{
	u64 period, max, min;

	/* special dl tasks don't actually use any parameter */
	ఒకవేళ (attr->sched_flags & SCHED_FLAG_SUGOV)
		తిరిగి true;

	/* deadline != 0 */
	ఒకవేళ (attr->sched_deadline == 0)
		తిరిగి false;

	/*
	 * Since we truncate DL_SCALE bits, make sure we're at least
	 * that big.
	 */
	ఒకవేళ (attr->sched_runtime < (1ULL << DL_SCALE))
		తిరిగి false;

	/*
	 * Since we use the MSB for wrap-around and sign issues, make
	 * sure it's not set (mind that period can be equal to zero).
	 */
	ఒకవేళ (attr->sched_deadline & (1ULL << 63) ||
	    attr->sched_period & (1ULL << 63))
		తిరిగి false;

	period = attr->sched_period;
	ఒకవేళ (!period)
		period = attr->sched_deadline;

	/* runtime <= deadline <= period (if period != 0) */
	ఒకవేళ (period < attr->sched_deadline ||
	    attr->sched_deadline < attr->sched_runtime)
		తిరిగి false;

	max = (u64)READ_ONCE(sysctl_sched_dl_period_max) * NSEC_చొప్పున_USEC;
	min = (u64)READ_ONCE(sysctl_sched_dl_period_min) * NSEC_చొప్పున_USEC;

	ఒకవేళ (period < min || period > max)
		తిరిగి false;

	తిరిగి true;
}

/*
 * This function clears the sched_dl_entity static params.
 */
స్థిర శూన్య __dl_clear_params(నిర్మాణం sched_dl_entity *dl_se)
{
	dl_se->dl_runtime		= 0;
	dl_se->dl_deadline		= 0;
	dl_se->dl_period		= 0;
	dl_se->flags			= 0;
	dl_se->dl_bw			= 0;
	dl_se->dl_density		= 0;

	dl_se->dl_throttled		= 0;
	dl_se->dl_yielded		= 0;
	dl_se->dl_non_contending	= 0;
	dl_se->dl_overrun		= 0;
	dl_se->dl_server		= 0;
	dl_se->dl_defer			= 0;
	dl_se->dl_defer_running		= 0;
	dl_se->dl_defer_armed		= 0;

#నిర్వచితమైతే CONFIG_RT_MUTEXES
	dl_se->pi_se			= dl_se;
#అయితే_ముగింపు
}

శూన్య init_dl_entity(నిర్మాణం sched_dl_entity *dl_se)
{
	RB_CLEAR_కణుపు(&dl_se->rb_కణుపు);
	init_dl_task_timer(dl_se);
	init_dl_inactive_task_timer(dl_se);
	__dl_clear_params(dl_se);
}

bool dl_param_మారింది(నిర్మాణం task_struct *p, స్థిరరాశి నిర్మాణం sched_attr *attr)
{
	నిర్మాణం sched_dl_entity *dl_se = &p->dl;

	ఒకవేళ (dl_se->dl_runtime != attr->sched_runtime ||
	    dl_se->dl_deadline != attr->sched_deadline ||
	    dl_se->dl_period != attr->sched_period ||
	    dl_se->flags != (attr->sched_flags & SCHED_DL_FLAGS))
		తిరిగి true;

	తిరిగి false;
}

పూర్ణాంకం dl_cpuset_cpumask_can_shrink(స్థిరరాశి నిర్మాణం cpumask *cur,
				 స్థిరరాశి నిర్మాణం cpumask *trial)
{
	గుర్తులేని దీర్ఘ flags, cap;
	నిర్మాణం dl_bw *cur_dl_b;
	పూర్ణాంకం ret = 1;

	rcu_read_తాళం_sched();
	cur_dl_b = dl_bw_యొక్క(cpumask_any(cur));
	cap = __dl_bw_capacity(trial);
	raw_spin_తాళం_irqsave(&cur_dl_b->తాళం, flags);
	ఒకవేళ (__dl_overflow(cur_dl_b, cap, 0, 0))
		ret = 0;
	raw_spin_unlock_irqrestore(&cur_dl_b->తాళం, flags);
	rcu_read_unlock_sched();

	తిరిగి ret;
}

గణన dl_bw_request {
	dl_bw_req_deactivate = 0,
	dl_bw_req_alloc,
	dl_bw_req_free
};

స్థిర పూర్ణాంకం dl_bw_manage(గణన dl_bw_request req, పూర్ణాంకం cpu, u64 dl_bw)
{
	గుర్తులేని దీర్ఘ flags, cap;
	నిర్మాణం dl_bw *dl_b;
	bool అధికప్రవాహం = 0;
	u64 dl_server_bw = 0;

	rcu_read_తాళం_sched();
	dl_b = dl_bw_యొక్క(cpu);
	raw_spin_తాళం_irqsave(&dl_b->తాళం, flags);

	cap = dl_bw_సామర్థ్యం(cpu);
	ఎంపిక (req) {
	స్థితి dl_bw_req_free:
		__dl_sub(dl_b, dl_bw, dl_bw_cpus(cpu));
		ఆపు;
	స్థితి dl_bw_req_alloc:
		అధికప్రవాహం = __dl_overflow(dl_b, cap, 0, dl_bw);

		ఒకవేళ (!అధికప్రవాహం) {
			/*
			 * We reserve space in the destination
			 * root_domain, as we can't fail after this point.
			 * We will free resources in the source root_domain
			 * later on (see set_cpus_allowed_dl()).
			 */
			__dl_add(dl_b, dl_bw, dl_bw_cpus(cpu));
		}
		ఆపు;
	స్థితి dl_bw_req_deactivate:
		/*
		 * cpu is not off yet, but we need to do the math by
		 * considering it off already (i.e., what would happen if we
		 * turn cpu off?).
		 */
		cap -= arch_scale_cpu_సామర్థ్యం(cpu);

		/*
		 * cpu is going offline and NORMAL and EXT tasks will be
		 * moved away from it. We can thus discount dl_server
		 * bandwidth contribution as it won't need to be servicing
		 * tasks after the cpu is off.
		 */
		dl_server_bw = dl_server_read_bw(cpu);

		/*
		 * Not much to check if no DEADLINE bandwidth is present.
		 * dl_servers we can discount, as tasks will be moved out the
		 * offlined CPUs anyway.
		 */
		ఒకవేళ (dl_b->మొత్తం_bw - dl_server_bw > 0) {
			/*
			 * Leaving at least one CPU for DEADLINE tasks seems a
			 * wise thing to do. As said above, cpu is not offline
			 * yet, so account for that.
			 */
			ఒకవేళ (dl_bw_cpus(cpu) - 1)
				అధికప్రవాహం = __dl_overflow(dl_b, cap, dl_server_bw, 0);
			లేకపోతే
				అధికప్రవాహం = 1;
		}

		ఆపు;
	}

	raw_spin_unlock_irqrestore(&dl_b->తాళం, flags);
	rcu_read_unlock_sched();

	తిరిగి అధికప్రవాహం ? -EBUSY : 0;
}

పూర్ణాంకం dl_bw_deactivate(పూర్ణాంకం cpu)
{
	తిరిగి dl_bw_manage(dl_bw_req_deactivate, cpu, 0);
}

పూర్ణాంకం dl_bw_alloc(పూర్ణాంకం cpu, u64 dl_bw)
{
	తిరిగి dl_bw_manage(dl_bw_req_alloc, cpu, dl_bw);
}

శూన్య dl_bw_free(పూర్ణాంకం cpu, u64 dl_bw)
{
	dl_bw_manage(dl_bw_req_free, cpu, dl_bw);
}

శూన్య print_dl_stats(నిర్మాణం seq_file *m, పూర్ణాంకం cpu)
{
	print_dl_rq(m, cpu, &cpu_rq(cpu)->dl);
}
