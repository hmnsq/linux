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

#अन्तर्भाव <linux/cpuset.h>
#अन्तर्भाव <linux/sched/clock.h>
#अन्तर्भाव <linux/sched/deadline.h>
#अन्तर्भाव <uapi/linux/sched/types.h>
#अन्तर्भाव "sched.h"
#अन्तर्भाव "pelt.h"

/*
 * Default limits for DL period; on the top end we guard against small util
 * tasks still getting ridiculously long effective runtimes, on the bottom end we
 * guard against timer DoS.
 */
स्थिर unsigned पूर्णाङ्क sysctl_sched_dl_period_max = 1 << 22; /* ~4 seconds */
स्थिर unsigned पूर्णाङ्क sysctl_sched_dl_period_min = 100;     /* 100 us */
#ifdef CONFIG_SYSCTL
स्थिर ध्रुव संरचना ctl_table sched_dl_sysctls[] = {
	{
		.procname       = "sched_deadline_period_max_us",
		.data           = &sysctl_sched_dl_period_max,
		.maxlen         = परिमाण(unsigned पूर्णाङ्क),
		.mode           = 0644,
		.proc_handler   = proc_douintvec_minmax,
		.extra1         = (शून्य *)&sysctl_sched_dl_period_min,
	},
	{
		.procname       = "sched_deadline_period_min_us",
		.data           = &sysctl_sched_dl_period_min,
		.maxlen         = परिमाण(unsigned पूर्णाङ्क),
		.mode           = 0644,
		.proc_handler   = proc_douintvec_minmax,
		.extra2         = (शून्य *)&sysctl_sched_dl_period_max,
	},
};

स्थिर पूर्णाङ्क __init sched_dl_sysctl_init(शून्य)
{
	register_sysctl_init("kernel", sched_dl_sysctls);
	प्रत्यागम 0;
}
late_initcall(sched_dl_sysctl_init);
#endif /* CONFIG_SYSCTL */

स्थिर अन्तःस्थ संरचना rq *rq_of_dl_rq(संरचना dl_rq *dl_rq)
{
	प्रत्यागम container_of(dl_rq, संरचना rq, dl);
}

स्थिर अन्तःस्थ संरचना rq *rq_of_dl_se(संरचना sched_dl_entity *dl_se)
{
	संरचना rq *rq = dl_se->rq;

	यदि (!dl_server(dl_se))
		rq = task_rq(dl_task_of(dl_se));

	प्रत्यागम rq;
}

स्थिर अन्तःस्थ संरचना dl_rq *dl_rq_of_se(संरचना sched_dl_entity *dl_se)
{
	प्रत्यागम &rq_of_dl_se(dl_se)->dl;
}

स्थिर अन्तःस्थ पूर्णाङ्क on_dl_rq(संरचना sched_dl_entity *dl_se)
{
	प्रत्यागम !RB_EMPTY_गाठ(&dl_se->rb_गाठ);
}

#ifdef CONFIG_RT_MUTEXES
स्थिर अन्तःस्थ संरचना sched_dl_entity *pi_of(संरचना sched_dl_entity *dl_se)
{
	प्रत्यागम dl_se->pi_se;
}

स्थिर अन्तःस्थ bool is_dl_boosted(संरचना sched_dl_entity *dl_se)
{
	प्रत्यागम pi_of(dl_se) != dl_se;
}
#अन्यथा /* !CONFIG_RT_MUTEXES: */
स्थिर अन्तःस्थ संरचना sched_dl_entity *pi_of(संरचना sched_dl_entity *dl_se)
{
	प्रत्यागम dl_se;
}

स्थिर अन्तःस्थ bool is_dl_boosted(संरचना sched_dl_entity *dl_se)
{
	प्रत्यागम false;
}
#endif /* !CONFIG_RT_MUTEXES */

स्थिर अन्तःस्थ u8 dl_get_type(संरचना sched_dl_entity *dl_se, संरचना rq *rq)
{
	यदि (!dl_server(dl_se))
		प्रत्यागम DL_TASK;
	यदि (dl_se == &rq->fair_server)
		प्रत्यागम DL_SERVER_FAIR;
#ifdef CONFIG_SCHED_CLASS_EXT
	यदि (dl_se == &rq->ext_server)
		प्रत्यागम DL_SERVER_EXT;
#endif
	प्रत्यागम DL_OTHER;
}

स्थिर अन्तःस्थ संरचना dl_bw *dl_bw_of(पूर्णाङ्क i)
{
	RCU_LOCKDEP_WARN(!rcu_read_lock_sched_held(),
			 "sched RCU must be held");
	प्रत्यागम &cpu_rq(i)->rd->dl_bw;
}

स्थिर अन्तःस्थ पूर्णाङ्क dl_bw_cpus(पूर्णाङ्क i)
{
	संरचना मूळ_domain *rd = cpu_rq(i)->rd;

	RCU_LOCKDEP_WARN(!rcu_read_lock_sched_held(),
			 "sched RCU must be held");

	प्रत्यागम cpumask_weight_and(rd->span, cpu_active_mask);
}

स्थिर अन्तःस्थ unsigned long __dl_bw_capacity(ध्रुव संरचना cpumask *mask)
{
	unsigned long cap = 0;
	पूर्णाङ्क i;

	for_each_cpu_and(i, mask, cpu_active_mask)
		cap += arch_scale_cpu_capacity(i);

	प्रत्यागम cap;
}

/*
 * XXX Fix: If 'rq->rd == def_root_domain' perform AC against capacity
 * of the CPU the task is running on rather rd's \Sum CPU capacity.
 */
स्थिर अन्तःस्थ unsigned long dl_bw_capacity(पूर्णाङ्क i)
{
	यदि (!sched_asym_cpucap_active() &&
	    arch_scale_cpu_capacity(i) == SCHED_CAPACITY_SCALE) {
		प्रत्यागम dl_bw_cpus(i) << SCHED_CAPACITY_SHIFT;
	} अन्यथा {
		RCU_LOCKDEP_WARN(!rcu_read_lock_sched_held(),
				 "sched RCU must be held");

		प्रत्यागम __dl_bw_capacity(cpu_rq(i)->rd->span);
	}
}

bool dl_bw_visited(पूर्णाङ्क cpu, u64 cookie)
{
	संरचना मूळ_domain *rd = cpu_rq(cpu)->rd;

	यदि (rd->visit_cookie == cookie)
		प्रत्यागम true;

	rd->visit_cookie = cookie;
	प्रत्यागम false;
}

स्थिर अन्तःस्थ
शून्य __dl_update(संरचना dl_bw *dl_b, s64 bw)
{
	संरचना मूळ_domain *rd = container_of(dl_b, संरचना मूळ_domain, dl_bw);
	पूर्णाङ्क i;

	RCU_LOCKDEP_WARN(!rcu_read_lock_sched_held(),
			 "sched RCU must be held");
	for_each_cpu_and(i, rd->span, cpu_active_mask) {
		संरचना rq *rq = cpu_rq(i);

		rq->dl.extra_bw += bw;
	}
}

स्थिर अन्तःस्थ
शून्य __dl_sub(संरचना dl_bw *dl_b, u64 tsk_bw, पूर्णाङ्क cpus)
{
	dl_b->total_bw -= tsk_bw;
	__dl_update(dl_b, (s32)tsk_bw / cpus);
}

स्थिर अन्तःस्थ
शून्य __dl_add(संरचना dl_bw *dl_b, u64 tsk_bw, पूर्णाङ्क cpus)
{
	dl_b->total_bw += tsk_bw;
	__dl_update(dl_b, -((s32)tsk_bw / cpus));
}

स्थिर अन्तःस्थ bool
__dl_overflow(संरचना dl_bw *dl_b, unsigned long cap, u64 old_bw, u64 new_bw)
{
	प्रत्यागम dl_b->bw != -1 &&
	       cap_scale(dl_b->bw, cap) < dl_b->total_bw - old_bw + new_bw;
}

स्थिर अन्तःस्थ
शून्य __add_running_bw(u64 dl_bw, संरचना dl_rq *dl_rq)
{
	u64 old = dl_rq->running_bw;

	lockdep_assert_rq_held(rq_of_dl_rq(dl_rq));
	dl_rq->running_bw += dl_bw;
	WARN_ON_ONCE(dl_rq->running_bw < old); /* overflow */
	WARN_ON_ONCE(dl_rq->running_bw > dl_rq->this_bw);
	/* kick cpufreq (see the comment in kernel/sched/sched.h). */
	cpufreq_update_util(rq_of_dl_rq(dl_rq), 0);
}

स्थिर अन्तःस्थ
शून्य __sub_running_bw(u64 dl_bw, संरचना dl_rq *dl_rq)
{
	u64 old = dl_rq->running_bw;

	lockdep_assert_rq_held(rq_of_dl_rq(dl_rq));
	dl_rq->running_bw -= dl_bw;
	WARN_ON_ONCE(dl_rq->running_bw > old); /* underflow */
	यदि (dl_rq->running_bw > old)
		dl_rq->running_bw = 0;
	/* kick cpufreq (see the comment in kernel/sched/sched.h). */
	cpufreq_update_util(rq_of_dl_rq(dl_rq), 0);
}

स्थिर अन्तःस्थ
शून्य __add_rq_bw(u64 dl_bw, संरचना dl_rq *dl_rq)
{
	u64 old = dl_rq->this_bw;

	lockdep_assert_rq_held(rq_of_dl_rq(dl_rq));
	dl_rq->this_bw += dl_bw;
	WARN_ON_ONCE(dl_rq->this_bw < old); /* overflow */
}

स्थिर अन्तःस्थ
शून्य __sub_rq_bw(u64 dl_bw, संरचना dl_rq *dl_rq)
{
	u64 old = dl_rq->this_bw;

	lockdep_assert_rq_held(rq_of_dl_rq(dl_rq));
	dl_rq->this_bw -= dl_bw;
	WARN_ON_ONCE(dl_rq->this_bw > old); /* underflow */
	यदि (dl_rq->this_bw > old)
		dl_rq->this_bw = 0;
	WARN_ON_ONCE(dl_rq->running_bw > dl_rq->this_bw);
}

स्थिर अन्तःस्थ
शून्य add_rq_bw(संरचना sched_dl_entity *dl_se, संरचना dl_rq *dl_rq)
{
	यदि (!dl_entity_is_special(dl_se))
		__add_rq_bw(dl_se->dl_bw, dl_rq);
}

स्थिर अन्तःस्थ
शून्य sub_rq_bw(संरचना sched_dl_entity *dl_se, संरचना dl_rq *dl_rq)
{
	यदि (!dl_entity_is_special(dl_se))
		__sub_rq_bw(dl_se->dl_bw, dl_rq);
}

स्थिर अन्तःस्थ
शून्य add_running_bw(संरचना sched_dl_entity *dl_se, संरचना dl_rq *dl_rq)
{
	यदि (!dl_entity_is_special(dl_se))
		__add_running_bw(dl_se->dl_bw, dl_rq);
}

स्थिर अन्तःस्थ
शून्य sub_running_bw(संरचना sched_dl_entity *dl_se, संरचना dl_rq *dl_rq)
{
	यदि (!dl_entity_is_special(dl_se))
		__sub_running_bw(dl_se->dl_bw, dl_rq);
}

स्थिर शून्य dl_rq_change_utilization(संरचना rq *rq, संरचना sched_dl_entity *dl_se, u64 new_bw)
{
	यदि (dl_se->dl_non_contending) {
		sub_running_bw(dl_se, &rq->dl);
		dl_se->dl_non_contending = 0;

		/*
		 * If the timer handler is currently running and the
		 * timer cannot be canceled, inactive_task_timer()
		 * will see that dl_not_contending is not set, and
		 * will not touch the rq's active utilization,
		 * so we are still safe.
		 */
		यदि (hrtimer_try_to_cancel(&dl_se->inactive_timer) == 1) {
			यदि (!dl_server(dl_se))
				put_task_struct(dl_task_of(dl_se));
		}
	}
	__sub_rq_bw(dl_se->dl_bw, &rq->dl);
	__add_rq_bw(new_bw, &rq->dl);
}

स्थिर __always_inline
शून्य cancel_dl_timer(संरचना sched_dl_entity *dl_se, संरचना hrtimer *timer)
{
	/*
	 * If the timer callback was running (hrtimer_try_to_cancel == -1),
	 * it will eventually call put_task_struct().
	 */
	यदि (hrtimer_try_to_cancel(timer) == 1 && !dl_server(dl_se))
		put_task_struct(dl_task_of(dl_se));
}

स्थिर __always_inline
शून्य cancel_replenish_timer(संरचना sched_dl_entity *dl_se)
{
	cancel_dl_timer(dl_se, &dl_se->dl_timer);
}

स्थिर __always_inline
शून्य cancel_inactive_timer(संरचना sched_dl_entity *dl_se)
{
	cancel_dl_timer(dl_se, &dl_se->inactive_timer);
}

स्थिर शून्य dl_change_utilization(संरचना task_struct *p, u64 new_bw)
{
	WARN_ON_ONCE(p->dl.flags & SCHED_FLAG_SUGOV);

	यदि (task_on_rq_queued(p))
		प्रत्यागम;

	dl_rq_change_utilization(task_rq(p), &p->dl, new_bw);
}

स्थिर शून्य __dl_clear_params(संरचना sched_dl_entity *dl_se);

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
स्थिर शून्य task_non_contending(संरचना sched_dl_entity *dl_se, bool dl_task)
{
	संरचना hrtimer *timer = &dl_se->inactive_timer;
	संरचना rq *rq = rq_of_dl_se(dl_se);
	संरचना dl_rq *dl_rq = &rq->dl;
	s64 zerolag_time;

	/*
	 * If this is a non-deadline task that has been boosted,
	 * do nothing
	 */
	यदि (dl_se->dl_runtime == 0)
		प्रत्यागम;

	यदि (dl_entity_is_special(dl_se))
		प्रत्यागम;

	WARN_ON(dl_se->dl_non_contending);

	zerolag_time = dl_se->deadline -
		 div64_long((dl_se->runtime * dl_se->dl_period),
			dl_se->dl_runtime);

	/*
	 * Using relative times instead of the absolute "0-lag time"
	 * allows to simplify the code
	 */
	zerolag_time -= rq_clock(rq);

	/*
	 * If the "0-lag time" already passed, decrease the active
	 * utilization now, instead of starting a timer
	 */
	यदि ((zerolag_time < 0) || hrtimer_active(&dl_se->inactive_timer)) {
		यदि (dl_server(dl_se)) {
			sub_running_bw(dl_se, dl_rq);
		} अन्यथा {
			संरचना task_struct *p = dl_task_of(dl_se);

			यदि (dl_task)
				sub_running_bw(dl_se, dl_rq);

			यदि (!dl_task || READ_ONCE(p->__state) == TASK_DEAD) {
				संरचना dl_bw *dl_b = dl_bw_of(task_cpu(p));

				यदि (READ_ONCE(p->__state) == TASK_DEAD)
					sub_rq_bw(dl_se, &rq->dl);
				raw_spin_lock(&dl_b->lock);
				__dl_sub(dl_b, dl_se->dl_bw, dl_bw_cpus(task_cpu(p)));
				raw_spin_unlock(&dl_b->lock);
				__dl_clear_params(dl_se);
			}
		}

		प्रत्यागम;
	}

	dl_se->dl_non_contending = 1;
	यदि (!dl_server(dl_se))
		get_task_struct(dl_task_of(dl_se));

	hrtimer_start(timer, ns_to_ktime(zerolag_time), HRTIMER_MODE_REL_HARD);
}

स्थिर शून्य task_contending(संरचना sched_dl_entity *dl_se, पूर्णाङ्क flags)
{
	संरचना dl_rq *dl_rq = dl_rq_of_se(dl_se);

	/*
	 * If this is a non-deadline task that has been boosted,
	 * do nothing
	 */
	यदि (dl_se->dl_runtime == 0)
		प्रत्यागम;

	यदि (flags & ENQUEUE_MIGRATED)
		add_rq_bw(dl_se, dl_rq);

	यदि (dl_se->dl_non_contending) {
		dl_se->dl_non_contending = 0;
		/*
		 * If the timer handler is currently running and the
		 * timer cannot be canceled, inactive_task_timer()
		 * will see that dl_not_contending is not set, and
		 * will not touch the rq's active utilization,
		 * so we are still safe.
		 */
		cancel_inactive_timer(dl_se);
	} अन्यथा {
		/*
		 * Since "dl_non_contending" is not set, the
		 * task's utilization has already been removed from
		 * active utilization (either when the task blocked,
		 * when the "inactive timer" fired).
		 * So, add it back.
		 */
		add_running_bw(dl_se, dl_rq);
	}
}

स्थिर अन्तःस्थ पूर्णाङ्क is_leftmost(संरचना sched_dl_entity *dl_se, संरचना dl_rq *dl_rq)
{
	प्रत्यागम rb_first_cached(&dl_rq->मूळ) == &dl_se->rb_गाठ;
}

स्थिर शून्य init_dl_rq_bw_ratio(संरचना dl_rq *dl_rq);

शून्य init_dl_bw(संरचना dl_bw *dl_b)
{
	raw_spin_lock_init(&dl_b->lock);
	यदि (global_rt_runtime() == RUNTIME_INF)
		dl_b->bw = -1;
	अन्यथा
		dl_b->bw = to_ratio(global_rt_period(), global_rt_runtime());
	dl_b->total_bw = 0;
}

शून्य init_dl_rq(संरचना dl_rq *dl_rq)
{
	dl_rq->मूळ = RB_मूळ_CACHED;

	/* zero means no -deadline tasks */
	dl_rq->earliest_dl.curr = dl_rq->earliest_dl.next = 0;

	dl_rq->overloaded = 0;
	dl_rq->pushable_dl_tasks_मूळ = RB_मूळ_CACHED;

	dl_rq->running_bw = 0;
	dl_rq->this_bw = 0;
	init_dl_rq_bw_ratio(dl_rq);
}

स्थिर अन्तःस्थ पूर्णाङ्क dl_overloaded(संरचना rq *rq)
{
	प्रत्यागम atomic_read(&rq->rd->dlo_मोजणी);
}

स्थिर अन्तःस्थ शून्य dl_set_overload(संरचना rq *rq)
{
	यदि (!rq->online)
		प्रत्यागम;

	cpumask_set_cpu(rq->cpu, rq->rd->dlo_mask);
	/*
	 * Must be visible before the overload count is
	 * set (as in sched_rt.c).
	 *
	 * Matched by the barrier in pull_dl_task().
	 */
	smp_wmb();
	atomic_inc(&rq->rd->dlo_मोजणी);
}

स्थिर अन्तःस्थ शून्य dl_clear_overload(संरचना rq *rq)
{
	यदि (!rq->online)
		प्रत्यागम;

	atomic_dec(&rq->rd->dlo_मोजणी);
	cpumask_clear_cpu(rq->cpu, rq->rd->dlo_mask);
}

#define __node_2_pdl(गाठ) \
	rb_entry((गाठ), संरचना task_struct, pushable_dl_tasks)

स्थिर अन्तःस्थ bool __pushable_less(संरचना rb_गाठ *a, ध्रुव संरचना rb_गाठ *b)
{
	प्रत्यागम dl_entity_preempt(&__node_2_pdl(a)->dl, &__node_2_pdl(b)->dl);
}

स्थिर अन्तःस्थ पूर्णाङ्क has_pushable_dl_tasks(संरचना rq *rq)
{
	प्रत्यागम !RB_EMPTY_मूळ(&rq->dl.pushable_dl_tasks_मूळ.rb_मूळ);
}

/*
 * The list of pushable -deadline task is not a plist, like in
 * sched_rt.c, it is an rb-tree with tasks ordered by deadline.
 */
स्थिर शून्य enqueue_pushable_dl_task(संरचना rq *rq, संरचना task_struct *p)
{
	संरचना rb_गाठ *leftmost;

	WARN_ON_ONCE(!RB_EMPTY_गाठ(&p->pushable_dl_tasks));

	leftmost = rb_add_cached(&p->pushable_dl_tasks,
				 &rq->dl.pushable_dl_tasks_मूळ,
				 __pushable_less);
	यदि (leftmost)
		rq->dl.earliest_dl.next = p->dl.deadline;

	यदि (!rq->dl.overloaded) {
		dl_set_overload(rq);
		rq->dl.overloaded = 1;
	}
}

स्थिर शून्य dequeue_pushable_dl_task(संरचना rq *rq, संरचना task_struct *p)
{
	संरचना dl_rq *dl_rq = &rq->dl;
	संरचना rb_मूळ_cached *मूळ = &dl_rq->pushable_dl_tasks_मूळ;
	संरचना rb_गाठ *leftmost;

	यदि (RB_EMPTY_गाठ(&p->pushable_dl_tasks))
		प्रत्यागम;

	leftmost = rb_erase_cached(&p->pushable_dl_tasks, मूळ);
	यदि (leftmost)
		dl_rq->earliest_dl.next = __node_2_pdl(leftmost)->dl.deadline;

	RB_CLEAR_गाठ(&p->pushable_dl_tasks);

	यदि (!has_pushable_dl_tasks(rq) && rq->dl.overloaded) {
		dl_clear_overload(rq);
		rq->dl.overloaded = 0;
	}
}

स्थिर पूर्णाङ्क ढकला_dl_task(संरचना rq *rq);

स्थिर अन्तःस्थ bool need_pull_dl_task(संरचना rq *rq, संरचना task_struct *prev)
{
	प्रत्यागम rq->online && dl_task(prev);
}

स्थिर DEFINE_PER_CPU(संरचना balance_callback, dl_ढकला_head);
स्थिर DEFINE_PER_CPU(संरचना balance_callback, dl_pull_head);

स्थिर शून्य ढकला_dl_tasks(संरचना rq *);
स्थिर शून्य pull_dl_task(संरचना rq *);

स्थिर अन्तःस्थ शून्य deadline_queue_ढकला_tasks(संरचना rq *rq)
{
	यदि (!has_pushable_dl_tasks(rq))
		प्रत्यागम;

	queue_balance_callback(rq, &per_cpu(dl_ढकला_head, rq->cpu), ढकला_dl_tasks);
}

स्थिर अन्तःस्थ शून्य deadline_queue_pull_task(संरचना rq *rq)
{
	queue_balance_callback(rq, &per_cpu(dl_pull_head, rq->cpu), pull_dl_task);
}

स्थिर संरचना rq *find_lock_later_rq(संरचना task_struct *task, संरचना rq *rq);

स्थिर संरचना rq *dl_task_offline_migration(संरचना rq *rq, संरचना task_struct *p)
{
	संरचना rq *later_rq = रिक्त;
	संरचना dl_bw *dl_b;

	later_rq = find_lock_later_rq(p, rq);
	यदि (!later_rq) {
		पूर्णाङ्क cpu;

		/*
		 * If we cannot preempt any rq, fall back to pick any
		 * online CPU:
		 */
		cpu = cpumask_any_and(cpu_active_mask, p->cpus_ptr);
		यदि (cpu >= nr_cpu_ids) {
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
			cpu = cpumask_any(cpu_active_mask);
		}
		later_rq = cpu_rq(cpu);
		double_lock_balance(rq, later_rq);
	}

	यदि (p->dl.dl_non_contending || p->dl.dl_throttled) {
		/*
		 * Inactive timer is armed (or callback is running, but
		 * waiting for us to release rq locks). In any case, when it
		 * will fire (or continue), it will see running_bw of this
		 * task migrated to later_rq (and correctly handle it).
		 */
		sub_running_bw(&p->dl, &rq->dl);
		sub_rq_bw(&p->dl, &rq->dl);

		add_rq_bw(&p->dl, &later_rq->dl);
		add_running_bw(&p->dl, &later_rq->dl);
	} अन्यथा {
		sub_rq_bw(&p->dl, &rq->dl);
		add_rq_bw(&p->dl, &later_rq->dl);
	}

	/*
	 * And we finally need to fix up root_domain(s) bandwidth accounting,
	 * since p is still hanging out in the old (now moved to default) root
	 * domain.
	 */
	dl_b = &rq->rd->dl_bw;
	raw_spin_lock(&dl_b->lock);
	__dl_sub(dl_b, p->dl.dl_bw, cpumask_weight(rq->rd->span));
	raw_spin_unlock(&dl_b->lock);

	dl_b = &later_rq->rd->dl_bw;
	raw_spin_lock(&dl_b->lock);
	__dl_add(dl_b, p->dl.dl_bw, cpumask_weight(later_rq->rd->span));
	raw_spin_unlock(&dl_b->lock);

	set_task_cpu(p, later_rq->cpu);
	double_unlock_balance(later_rq, rq);

	प्रत्यागम later_rq;
}

स्थिर शून्य
enqueue_dl_entity(संरचना sched_dl_entity *dl_se, पूर्णाङ्क flags);
स्थिर शून्य enqueue_task_dl(संरचना rq *rq, संरचना task_struct *p, पूर्णाङ्क flags);
स्थिर शून्य dequeue_dl_entity(संरचना sched_dl_entity *dl_se, पूर्णाङ्क flags);
स्थिर शून्य wakeup_preempt_dl(संरचना rq *rq, संरचना task_struct *p, पूर्णाङ्क flags);

स्थिर अन्तःस्थ शून्य replenish_dl_new_period(संरचना sched_dl_entity *dl_se,
					    संरचना rq *rq)
{
	/* for non-boosted task, pi_of(dl_se) == dl_se */
	dl_se->deadline = rq_clock(rq) + pi_of(dl_se)->dl_deadline;
	dl_se->runtime = pi_of(dl_se)->dl_runtime;

	/*
	 * If it is a deferred reservation, and the server
	 * is not handling an starvation case, defer it.
	 */
	यदि (dl_se->dl_defer && !dl_se->dl_defer_running) {
		dl_se->dl_throttled = 1;
		dl_se->dl_defer_armed = 1;
	}
	trace_sched_dl_replenish_tp(dl_se, cpu_of(rq), dl_get_type(dl_se, rq));
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
स्थिर अन्तःस्थ शून्य setup_new_dl_entity(संरचना sched_dl_entity *dl_se)
{
	संरचना dl_rq *dl_rq = dl_rq_of_se(dl_se);
	संरचना rq *rq = rq_of_dl_rq(dl_rq);

	WARN_ON(is_dl_boosted(dl_se));
	WARN_ON(dl_time_before(rq_clock(rq), dl_se->deadline));

	/*
	 * We are racing with the deadline timer. So, do nothing because
	 * the deadline timer handler will take care of properly recharging
	 * the runtime and postponing the deadline
	 */
	यदि (dl_se->dl_throttled)
		प्रत्यागम;

	/*
	 * We use the regular wall clock time to set deadlines in the
	 * future; in fact, we must consider execution overheads (time
	 * spent on hardirq context, etc.).
	 */
	replenish_dl_new_period(dl_se, rq);
}

स्थिर पूर्णाङ्क start_dl_timer(संरचना sched_dl_entity *dl_se);
स्थिर bool dl_entity_overflow(संरचना sched_dl_entity *dl_se, u64 t);

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
स्थिर शून्य replenish_dl_entity(संरचना sched_dl_entity *dl_se)
{
	संरचना dl_rq *dl_rq = dl_rq_of_se(dl_se);
	संरचना rq *rq = rq_of_dl_rq(dl_rq);

	WARN_ON_ONCE(pi_of(dl_se)->dl_runtime <= 0);

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
	यदि (dl_se->dl_deadline == 0 ||
	    (dl_se->dl_defer_armed && dl_entity_overflow(dl_se, rq_clock(rq)))) {
		dl_se->deadline = rq_clock(rq) + pi_of(dl_se)->dl_deadline;
		dl_se->runtime = pi_of(dl_se)->dl_runtime;
	}

	यदि (dl_se->dl_yielded && dl_se->runtime > 0)
		dl_se->runtime = 0;

	/*
	 * We keep moving the deadline away until we get some
	 * available runtime for the entity. This ensures correct
	 * handling of situations where the runtime overrun is
	 * arbitrary large.
	 */
	यावत् (dl_se->runtime <= 0) {
		dl_se->deadline += pi_of(dl_se)->dl_period;
		dl_se->runtime += pi_of(dl_se)->dl_runtime;
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
	यदि (dl_time_before(dl_se->deadline, rq_clock(rq))) {
		printk_deferred_once("sched: DL replenish lagged too much\n");
		replenish_dl_new_period(dl_se, rq);
	}

	यदि (dl_se->dl_yielded)
		dl_se->dl_yielded = 0;
	यदि (dl_se->dl_throttled)
		dl_se->dl_throttled = 0;

	trace_sched_dl_replenish_tp(dl_se, cpu_of(rq), dl_get_type(dl_se, rq));

	/*
	 * If this is the replenishment of a deferred reservation,
	 * clear the flag and return.
	 */
	यदि (dl_se->dl_defer_armed) {
		dl_se->dl_defer_armed = 0;
		प्रत्यागम;
	}

	/*
	 * A this point, if the deferred server is not armed, and the deadline
	 * is in the future, if it is not running already, throttle the server
	 * and arm the defer timer.
	 */
	यदि (dl_se->dl_defer && !dl_se->dl_defer_running &&
	    dl_time_before(rq_clock(dl_se->rq), dl_se->deadline - dl_se->runtime)) {
		यदि (!is_dl_boosted(dl_se)) {

			/*
			 * Set dl_se->dl_defer_armed and dl_throttled variables to
			 * inform the start_dl_timer() that this is a deferred
			 * activation.
			 */
			dl_se->dl_defer_armed = 1;
			dl_se->dl_throttled = 1;
			यदि (!start_dl_timer(dl_se)) {
				/*
				 * If for whatever reason (delays), a previous timer was
				 * queued but not serviced, cancel it and clean the
				 * deferrable server variables intended for start_dl_timer().
				 */
				hrtimer_try_to_cancel(&dl_se->dl_timer);
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
स्थिर bool dl_entity_overflow(संरचना sched_dl_entity *dl_se, u64 t)
{
	u64 डावा, उजवा;

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
	डावा = (pi_of(dl_se)->dl_deadline >> DL_SCALE) * (dl_se->runtime >> DL_SCALE);
	उजवा = ((dl_se->deadline - t) >> DL_SCALE) *
		(pi_of(dl_se)->dl_runtime >> DL_SCALE);

	प्रत्यागम dl_time_before(उजवा, डावा);
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
स्थिर शून्य
update_dl_revised_wakeup(संरचना sched_dl_entity *dl_se, संरचना rq *rq)
{
	u64 laxity = dl_se->deadline - rq_clock(rq);

	/*
	 * If the task has deadline < period, and the deadline is in the past,
	 * it should already be throttled before this check.
	 *
	 * See update_dl_entity() comments for further details.
	 */
	WARN_ON(dl_time_before(dl_se->deadline, rq_clock(rq)));

	dl_se->runtime = (dl_se->dl_density * laxity) >> BW_SHIFT;
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
स्थिर शून्य update_dl_entity(संरचना sched_dl_entity *dl_se)
{
	संरचना rq *rq = rq_of_dl_se(dl_se);

	यदि (dl_time_before(dl_se->deadline, rq_clock(rq)) ||
	    dl_entity_overflow(dl_se, rq_clock(rq))) {

		यदि (unlikely((!dl_is_implicit(dl_se) ||
			      (dl_se->dl_defer && dl_se->dl_defer_running)) &&
			     !dl_time_before(dl_se->deadline, rq_clock(rq)) &&
			     !is_dl_boosted(dl_se))) {
			update_dl_revised_wakeup(dl_se, rq);
			प्रत्यागम;
		}

		/*
		 * When [4] D->A is followed by [1] A->B, dl_defer_running
		 * needs to be cleared, otherwise it will fail to properly
		 * start the zero-laxity timer.
		 */
		dl_se->dl_defer_running = 0;
		replenish_dl_new_period(dl_se, rq);
	} अन्यथा यदि (dl_server(dl_se) && dl_se->dl_defer) {
		/*
		 * The server can still use its previous deadline, so check if
		 * it left the dl_defer_running state.
		 */
		यदि (!dl_se->dl_defer_running) {
			dl_se->dl_defer_armed = 1;
			dl_se->dl_throttled = 1;
		}
	}
}

स्थिर अन्तःस्थ u64 dl_next_period(संरचना sched_dl_entity *dl_se)
{
	प्रत्यागम dl_se->deadline - dl_se->dl_deadline + dl_se->dl_period;
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
स्थिर पूर्णाङ्क start_dl_timer(संरचना sched_dl_entity *dl_se)
{
	संरचना hrtimer *timer = &dl_se->dl_timer;
	संरचना dl_rq *dl_rq = dl_rq_of_se(dl_se);
	संरचना rq *rq = rq_of_dl_rq(dl_rq);
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
	यदि (dl_se->dl_defer_armed) {
		WARN_ON_ONCE(!dl_se->dl_throttled);
		act = ns_to_ktime(dl_se->deadline - dl_se->runtime);
	} अन्यथा {
		/* act = deadline - rel-deadline + period */
		act = ns_to_ktime(dl_next_period(dl_se));
	}

	now = ktime_get();
	delta = ktime_to_ns(now) - rq_clock(rq);
	act = ktime_add_ns(act, delta);

	/*
	 * If the expiry time already passed, e.g., because the value
	 * chosen as the deadline is too small, don't even try to
	 * start the timer in the past!
	 */
	यदि (ktime_before(act, now))
		प्रत्यागम 0;

	/*
	 * !enqueued will guarantee another callback; even if one is already in
	 * progress. This ensures a balanced {get,put}_task_struct().
	 *
	 * The race against __run_timer() clearing the enqueued state is
	 * harmless because we're holding task_rq()->lock, therefore the timer
	 * expiring after we've done the check will wait on its task_rq_lock()
	 * and observe our state.
	 */
	यदि (!hrtimer_is_queued(timer)) {
		यदि (!dl_server(dl_se))
			get_task_struct(dl_task_of(dl_se));
		hrtimer_start(timer, act, HRTIMER_MODE_ABS_HARD);
	}

	प्रत्यागम 1;
}

स्थिर शून्य __push_dl_task(संरचना rq *rq, संरचना rq_flags *rf)
{
	/*
	 * Queueing this task back might have overloaded rq, check if we need
	 * to kick someone away.
	 */
	यदि (has_pushable_dl_tasks(rq)) {
		/*
		 * Nothing relies on rq->lock after this, so its safe to drop
		 * rq->lock.
		 */
		rq_unpin_lock(rq, rf);
		ढकला_dl_task(rq);
		rq_repin_lock(rq, rf);
	}
}

/* a defer timer will not be reset if the runtime consumed was < dl_server_min_res */
स्थिर ध्रुव u64 dl_server_min_res = 1 * NSEC_PER_MSEC;

स्थिर गणना hrtimer_restart dl_server_timer(संरचना hrtimer *timer, संरचना sched_dl_entity *dl_se)
{
	संरचना rq *rq = rq_of_dl_se(dl_se);
	u64 fw;

	scoped_guard (rq_lock, rq) {
		संरचना rq_flags *rf = &scope.rf;

		यदि (!dl_se->dl_throttled || !dl_se->dl_runtime)
			प्रत्यागम HRTIMER_NORESTART;

		sched_clock_tick();
		update_rq_clock(rq);

		/*
		 * Make sure current has propagated its pending runtime into
		 * any relevant server through calling dl_server_update() and
		 * friends.
		 */
		rq->donor->sched_class->update_curr(rq);

		यदि (dl_se->dl_defer_idle) {
			dl_server_stop(dl_se);
			प्रत्यागम HRTIMER_NORESTART;
		}

		यदि (dl_se->dl_defer_armed) {
			/*
			 * First check if the server could consume runtime in background.
			 * If so, it is possible to push the defer timer for this amount
			 * of time. The dl_server_min_res serves as a limit to avoid
			 * forwarding the timer for a too small amount of time.
			 */
			यदि (dl_time_before(rq_clock(dl_se->rq),
					   (dl_se->deadline - dl_se->runtime - dl_server_min_res))) {

				/* reset the defer timer */
				fw = dl_se->deadline - rq_clock(dl_se->rq) - dl_se->runtime;

				hrtimer_forward_now(timer, ns_to_ktime(fw));
				प्रत्यागम HRTIMER_RESTART;
			}

			dl_se->dl_defer_running = 1;
		}

		enqueue_dl_entity(dl_se, ENQUEUE_REPLENISH);

		यदि (!dl_task(dl_se->rq->curr) || dl_entity_preempt(dl_se, &dl_se->rq->curr->dl))
			resched_curr(rq);

		__push_dl_task(rq, rf);
	}

	प्रत्यागम HRTIMER_NORESTART;
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
स्थिर गणना hrtimer_restart dl_task_timer(संरचना hrtimer *timer)
{
	संरचना sched_dl_entity *dl_se = container_of(timer,
						     संरचना sched_dl_entity,
						     dl_timer);
	संरचना task_struct *p;
	संरचना rq_flags rf;
	संरचना rq *rq;

	यदि (dl_server(dl_se))
		प्रत्यागम dl_server_timer(timer, dl_se);

	p = dl_task_of(dl_se);
	rq = task_rq_lock(p, &rf);

	/*
	 * The task might have changed its scheduling policy to something
	 * different than SCHED_DEADLINE (through switched_from_dl()).
	 */
	यदि (!dl_task(p))
		गच्छ unlock;

	/*
	 * The task might have been boosted by someone else and might be in the
	 * boosting/deboosting path, its not throttled.
	 */
	यदि (is_dl_boosted(dl_se))
		गच्छ unlock;

	/*
	 * Spurious timer due to start_dl_timer() race; or we already received
	 * a replenishment from rt_mutex_setprio().
	 */
	यदि (!dl_se->dl_throttled)
		गच्छ unlock;

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
	यदि (!task_on_rq_queued(p)) {
		replenish_dl_entity(dl_se);
		गच्छ unlock;
	}

	यदि (unlikely(!rq->online)) {
		/*
		 * If the runqueue is no longer available, migrate the
		 * task elsewhere. This necessarily changes rq.
		 */
		lockdep_unpin_lock(__rq_lockp(rq), rf.cookie);
		rq = dl_task_offline_migration(rq, p);
		rf.cookie = lockdep_pin_lock(__rq_lockp(rq));
		update_rq_clock(rq);

		/*
		 * Now that the task has been migrated to the new RQ and we
		 * have that locked, proceed as normal and enqueue the task
		 * there.
		 */
	}

	enqueue_task_dl(rq, p, ENQUEUE_REPLENISH);
	यदि (dl_task(rq->donor))
		wakeup_preempt_dl(rq, p, 0);
	अन्यथा
		resched_curr(rq);

	__push_dl_task(rq, &rf);

unlock:
	task_rq_unlock(rq, p, &rf);

	/*
	 * This can free the task_struct, including this hrtimer, do not touch
	 * anything related to that after this.
	 */
	put_task_struct(p);

	प्रत्यागम HRTIMER_NORESTART;
}

स्थिर शून्य init_dl_task_timer(संरचना sched_dl_entity *dl_se)
{
	संरचना hrtimer *timer = &dl_se->dl_timer;

	hrtimer_setup(timer, dl_task_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL_HARD);
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
स्थिर अन्तःस्थ शून्य dl_check_constrained_dl(संरचना sched_dl_entity *dl_se)
{
	संरचना rq *rq = rq_of_dl_se(dl_se);

	यदि (dl_time_before(dl_se->deadline, rq_clock(rq)) &&
	    dl_time_before(rq_clock(rq), dl_next_period(dl_se))) {
		यदि (unlikely(is_dl_boosted(dl_se) || !start_dl_timer(dl_se)))
			प्रत्यागम;
		trace_sched_dl_throttle_tp(dl_se, cpu_of(rq), dl_get_type(dl_se, rq));
		dl_se->dl_throttled = 1;
		यदि (dl_se->runtime > 0)
			dl_se->runtime = 0;
	}
}

स्थिर
पूर्णाङ्क dl_runtime_exceeded(संरचना sched_dl_entity *dl_se)
{
	प्रत्यागम (dl_se->runtime <= 0);
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
स्थिर u64 grub_reclaim(u64 delta, संरचना rq *rq, संरचना sched_dl_entity *dl_se)
{
	u64 u_act;
	u64 u_inact = rq->dl.this_bw - rq->dl.running_bw; /* Utot - Uact */

	/*
	 * Instead of computing max{u, (u_max - u_inact - u_extra)}, we
	 * compare u_inact + u_extra with u_max - u, because u_inact + u_extra
	 * can be larger than u_max. So, u_max - u_inact - u_extra would be
	 * negative leading to wrong results.
	 */
	यदि (u_inact + rq->dl.extra_bw > rq->dl.max_bw - dl_se->dl_bw)
		u_act = dl_se->dl_bw;
	अन्यथा
		u_act = rq->dl.max_bw - u_inact - rq->dl.extra_bw;

	u_act = (u_act * rq->dl.bw_ratio) >> RATIO_SHIFT;
	प्रत्यागम (delta * u_act) >> BW_SHIFT;
}

s64 dl_scaled_delta_exec(संरचना rq *rq, संरचना sched_dl_entity *dl_se, s64 delta_exec)
{
	s64 scaled_delta_exec;

	/*
	 * For tasks that participate in GRUB, we implement GRUB-PA: the
	 * spare reclaimed bandwidth is used to clock down frequency.
	 *
	 * For the others, we still need to scale reservation parameters
	 * according to current frequency and CPU maximum capacity.
	 */
	यदि (unlikely(dl_se->flags & SCHED_FLAG_RECLAIM)) {
		scaled_delta_exec = grub_reclaim(delta_exec, rq, dl_se);
	} अन्यथा {
		पूर्णाङ्क cpu = cpu_of(rq);
		unsigned long scale_freq = arch_scale_freq_capacity(cpu);
		unsigned long scale_cpu = arch_scale_cpu_capacity(cpu);

		scaled_delta_exec = cap_scale(delta_exec, scale_freq);
		scaled_delta_exec = cap_scale(scaled_delta_exec, scale_cpu);
	}

	प्रत्यागम scaled_delta_exec;
}

स्थिर अन्तःस्थ शून्य
update_stats_dequeue_dl(संरचना dl_rq *dl_rq, संरचना sched_dl_entity *dl_se, पूर्णाङ्क flags);

स्थिर शून्य update_curr_dl_se(संरचना rq *rq, संरचना sched_dl_entity *dl_se, s64 delta_exec)
{
	bool idle = idle_rq(rq);
	s64 scaled_delta_exec;

	यदि (unlikely(delta_exec <= 0)) {
		यदि (unlikely(dl_se->dl_yielded))
			गच्छ throttle;
		प्रत्यागम;
	}

	यदि (dl_server(dl_se) && dl_se->dl_throttled && !dl_se->dl_defer)
		प्रत्यागम;

	यदि (dl_entity_is_special(dl_se))
		प्रत्यागम;

	scaled_delta_exec = delta_exec;
	यदि (!dl_server(dl_se))
		scaled_delta_exec = dl_scaled_delta_exec(rq, dl_se, delta_exec);

	dl_se->runtime -= scaled_delta_exec;

	यदि (dl_se->dl_defer_idle && !idle)
		dl_se->dl_defer_idle = 0;

	/*
	 * The DL server can consume its runtime while throttled (not
	 * queued / running as regular CFS).
	 *
	 * If the server consumes its entire runtime in this state. The server
	 * is not required for the current period. Thus, reset the server by
	 * starting a new period, pushing the activation.
	 */
	यदि (dl_se->dl_defer && dl_se->dl_throttled && dl_runtime_exceeded(dl_se)) {
		/*
		 * Non-servers would never get time accounted while throttled.
		 */
		WARN_ON_ONCE(!dl_server(dl_se));

		/*
		 * While the server is marked idle, do not push out the
		 * activation further, instead wait for the period timer
		 * to lapse and stop the server.
		 */
		यदि (dl_se->dl_defer_idle && idle) {
			/*
			 * The timer is at the zero-laxity point, this means
			 * dl_server_stop() / dl_server_start() can happen
			 * while now < deadline. This means update_dl_entity()
			 * will not replenish. Additionally start_dl_timer()
			 * will be set for 'deadline - runtime'. Negative
			 * runtime will not do.
			 */
			dl_se->runtime = 0;
			प्रत्यागम;
		}

		/*
		 * If the server was previously activated - the starving condition
		 * took place, it this point it went away because the fair scheduler
		 * was able to get runtime in background. So return to the initial
		 * state.
		 */
		dl_se->dl_defer_running = 0;

		hrtimer_try_to_cancel(&dl_se->dl_timer);

		replenish_dl_new_period(dl_se, dl_se->rq);

		यदि (idle)
			dl_se->dl_defer_idle = 1;

		/*
		 * Not being able to start the timer seems problematic. If it could not
		 * be started for whatever reason, we need to "unthrottle" the DL server
		 * and queue right away. Otherwise nothing might queue it. That's similar
		 * to what enqueue_dl_entity() does on start_dl_timer==0. For now, just warn.
		 */
		WARN_ON_ONCE(!start_dl_timer(dl_se));

		प्रत्यागम;
	}

throttle:
	यदि (dl_runtime_exceeded(dl_se) || dl_se->dl_yielded) {
		trace_sched_dl_throttle_tp(dl_se, cpu_of(rq), dl_get_type(dl_se, rq));
		dl_se->dl_throttled = 1;

		/* If requested, inform the user about runtime overruns. */
		यदि (dl_runtime_exceeded(dl_se) &&
		    (dl_se->flags & SCHED_FLAG_DL_OVERRUN))
			dl_se->dl_overrun = 1;

		dequeue_dl_entity(dl_se, 0);
		यदि (!dl_server(dl_se)) {
			update_stats_dequeue_dl(&rq->dl, dl_se, 0);
			dequeue_pushable_dl_task(rq, dl_task_of(dl_se));
		}

		यदि (unlikely(is_dl_boosted(dl_se) || !start_dl_timer(dl_se))) {
			यदि (dl_server(dl_se)) {
				यदि (dl_se->dl_defer) {
					replenish_dl_new_period(dl_se, rq);
					start_dl_timer(dl_se);
				} अन्यथा {
					enqueue_dl_entity(dl_se, ENQUEUE_REPLENISH);
				}
			} अन्यथा {
				enqueue_task_dl(rq, dl_task_of(dl_se), ENQUEUE_REPLENISH);
			}
		}

		यदि (!is_leftmost(dl_se, &rq->dl))
			resched_curr(rq);
	} अन्यथा {
		trace_sched_dl_update_tp(dl_se, cpu_of(rq), dl_get_type(dl_se, rq));
	}

	/*
	 * The dl_server does not account for real-time workload because it
	 * is running fair work.
	 */
	यदि (dl_se->dl_server)
		प्रत्यागम;

#ifdef CONFIG_RT_GROUP_SCHED
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
	यदि (rt_bandwidth_enabled()) {
		संरचना rt_rq *rt_rq = &rq->rt;

		raw_spin_lock(&rt_rq->rt_runtime_lock);
		/*
		 * We'll let actual RT tasks worry about the overflow here, we
		 * have our own CBS to keep us inline; only account when RT
		 * bandwidth is relevant.
		 */
		यदि (sched_rt_bandwidth_account(rt_rq))
			rt_rq->rt_time += delta_exec;
		raw_spin_unlock(&rt_rq->rt_runtime_lock);
	}
#endif /* CONFIG_RT_GROUP_SCHED */
}

/*
 * In the non-defer mode, the idle time is not accounted, as the
 * server provides a guarantee.
 *
 * If the dl_server is in defer mode, the idle time is also considered as
 * time available for the dl_server, avoiding a penalty for the rt
 * scheduler that did not consumed that time.
 */
शून्य dl_server_update_idle(संरचना sched_dl_entity *dl_se, s64 delta_exec)
{
	यदि (dl_se->dl_server_active && dl_se->dl_runtime && dl_se->dl_defer)
		update_curr_dl_se(dl_se->rq, dl_se, delta_exec);
}

शून्य dl_server_update(संरचना sched_dl_entity *dl_se, s64 delta_exec)
{
	/* 0 runtime = fair server disabled */
	यदि (dl_se->dl_server_active && dl_se->dl_runtime)
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
शून्य dl_server_start(संरचना sched_dl_entity *dl_se)
{
	संरचना rq *rq = dl_se->rq;

	dl_se->dl_defer_idle = 0;
	यदि (!dl_server(dl_se) || dl_se->dl_server_active || !dl_se->dl_runtime ||
	    !dl_se->dl_bw_attached)
		प्रत्यागम;

	/*
	 * Update the current task to 'now'.
	 */
	rq->donor->sched_class->update_curr(rq);

	यदि (WARN_ON_ONCE(!cpu_online(cpu_of(rq))))
		प्रत्यागम;

	trace_sched_dl_server_start_tp(dl_se, cpu_of(rq), dl_get_type(dl_se, rq));
	dl_se->dl_server_active = 1;
	enqueue_dl_entity(dl_se, ENQUEUE_WAKEUP);
	यदि (!dl_task(dl_se->rq->curr) || dl_entity_preempt(dl_se, &rq->curr->dl))
		resched_curr(dl_se->rq);
}

शून्य dl_server_stop(संरचना sched_dl_entity *dl_se)
{
	यदि (!dl_server(dl_se) || !dl_server_active(dl_se))
		प्रत्यागम;

	trace_sched_dl_server_stop_tp(dl_se, cpu_of(dl_se->rq),
				      dl_get_type(dl_se, dl_se->rq));
	dequeue_dl_entity(dl_se, DEQUEUE_SLEEP);
	hrtimer_try_to_cancel(&dl_se->dl_timer);
	dl_se->dl_defer_armed = 0;
	dl_se->dl_throttled = 0;
	dl_se->dl_defer_idle = 0;
	dl_se->dl_server_active = 0;
}

शून्य dl_server_init(संरचना sched_dl_entity *dl_se, संरचना rq *rq,
		    dl_server_pick_f pick_task)
{
	dl_se->rq = rq;
	dl_se->server_pick_task = pick_task;
}

शून्य sched_init_dl_servers(शून्य)
{
	पूर्णाङ्क cpu;
	संरचना rq *rq;
	संरचना sched_dl_entity *dl_se;

	for_each_online_cpu(cpu) {
		u64 runtime =  50 * NSEC_PER_MSEC;
		u64 period = 1000 * NSEC_PER_MSEC;

		rq = cpu_rq(cpu);

		guard(rq_lock_irq)(rq);
		update_rq_clock(rq);

		dl_se = &rq->fair_server;

		WARN_ON(dl_server(dl_se));

		dl_server_apply_params(dl_se, runtime, period, 1);

		dl_se->dl_server = 1;
		dl_se->dl_defer = 1;
		setup_new_dl_entity(dl_se);

#ifdef CONFIG_SCHED_CLASS_EXT
		dl_se = &rq->ext_server;

		WARN_ON(dl_server(dl_se));

		dl_server_apply_params(dl_se, runtime, period, 1);

		dl_se->dl_server = 1;
		dl_se->dl_defer = 1;
		setup_new_dl_entity(dl_se);

		/*
		 * No BPF scheduler is loaded at boot, so the ext_server has no
		 * tasks to protect. Detach its bandwidth reservation, it will
		 * be attached when a BPF scheduler is loaded.
		 */
		dl_server_detach_bw(dl_se);
#endif
	}
}

शून्य __dl_server_attach_root(संरचना sched_dl_entity *dl_se, संरचना rq *rq)
{
	u64 new_bw = dl_se->dl_bw;
	पूर्णाङ्क cpu = cpu_of(rq);
	संरचना dl_bw *dl_b;

	यदि (!dl_se->dl_bw_attached)
		प्रत्यागम;

	dl_b = dl_bw_of(cpu_of(rq));
	guard(raw_spinlock)(&dl_b->lock);

	यदि (!dl_bw_cpus(cpu))
		प्रत्यागम;

	__dl_add(dl_b, new_bw, dl_bw_cpus(cpu));
}

पूर्णाङ्क dl_server_apply_params(संरचना sched_dl_entity *dl_se, u64 runtime, u64 period, bool init)
{
	u64 old_bw = (init || !dl_se->dl_bw_attached) ? 0 :
		     to_ratio(dl_se->dl_period, dl_se->dl_runtime);
	u64 new_bw = to_ratio(period, runtime);
	संरचना rq *rq = dl_se->rq;
	पूर्णाङ्क cpu = cpu_of(rq);
	संरचना dl_bw *dl_b;
	unsigned long cap;
	पूर्णाङ्क cpus;

	dl_b = dl_bw_of(cpu);
	guard(raw_spinlock)(&dl_b->lock);

	cpus = dl_bw_cpus(cpu);
	cap = dl_bw_capacity(cpu);

	यदि (__dl_overflow(dl_b, cap, old_bw, new_bw))
		प्रत्यागम -EBUSY;

	यदि (init) {
		__add_rq_bw(new_bw, &rq->dl);
		__dl_add(dl_b, new_bw, cpus);
		dl_se->dl_bw_attached = 1;
	} अन्यथा यदि (dl_se->dl_bw_attached) {
		__dl_sub(dl_b, dl_se->dl_bw, cpus);
		__dl_add(dl_b, new_bw, cpus);

		dl_rq_change_utilization(rq, dl_se, new_bw);
	}

	dl_se->dl_runtime = runtime;
	dl_se->dl_deadline = period;
	dl_se->dl_period = period;

	dl_se->runtime = 0;
	dl_se->deadline = 0;

	dl_se->dl_bw = to_ratio(dl_se->dl_period, dl_se->dl_runtime);
	dl_se->dl_density = to_ratio(dl_se->dl_deadline, dl_se->dl_runtime);

	प्रत्यागम 0;
}

/*
 * Add @dl_se's bw to the root-domain accounting.
 *
 * Return -EBUSY if attaching would overflow root domain capacity.
 */
स्थिर पूर्णाङ्क __dl_server_attach_bw_locked(संरचना sched_dl_entity *dl_se,
					संरचना dl_bw *dl_b, पूर्णाङ्क cpus)
{
	संरचना rq *rq = dl_se->rq;
	unsigned long cap;

	/*
	 * Always update @rq->dl.this_bw, but only update @dl_b->total_bw
	 * (and run the overflow check it gates) while this CPU is active.
	 *
	 * This mirrors dl_server_add_bw() during root-domain rebuilds, which
	 * only publishes bandwidth from active CPUs into @dl_b.
	 */
	यदि (cpu_active(cpu_of(rq))) {
		cap = dl_bw_capacity(cpu_of(rq));
		यदि (__dl_overflow(dl_b, cap, 0, dl_se->dl_bw))
			प्रत्यागम -EBUSY;
		__dl_add(dl_b, dl_se->dl_bw, cpus);
	}
	__add_rq_bw(dl_se->dl_bw, &rq->dl);
	dl_se->dl_bw_attached = 1;

	प्रत्यागम 0;
}

/*
 * Drain @dl_se and remove its bw from the root-domain accounting.
 */
स्थिर शून्य __dl_server_detach_bw_locked(संरचना sched_dl_entity *dl_se,
					 संरचना dl_bw *dl_b, पूर्णाङ्क cpus)
{
	संरचना rq *rq = dl_se->rq;

	/*
	 * If the server is still active (on_rq), dequeue it via
	 * dl_server_stop(); task_non_contending() will either subtract
	 * @dl_bw from running_bw immediately (0-lag passed) or set
	 * dl_non_contending and arm the inactive_timer.
	 */
	यदि (dl_se->dl_server_active)
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
	यदि (cpu_active(cpu_of(rq)))
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
पूर्णाङ्क dl_server_attach_bw(संरचना sched_dl_entity *dl_se)
{
	संरचना rq *rq = dl_se->rq;
	पूर्णाङ्क cpu = cpu_of(rq);
	संरचना dl_bw *dl_b;
	पूर्णाङ्क cpus, ret;

	यदि (dl_se->dl_bw_attached)
		प्रत्यागम 0;

	scoped_guard (raw_spinlock, &dl_bw_of(cpu)->lock) {
		dl_b = dl_bw_of(cpu);
		cpus = dl_bw_cpus(cpu);
		ret = __dl_server_attach_bw_locked(dl_se, dl_b, cpus);
	}
	यदि (ret)
		प्रत्यागम ret;

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
	यदि (cpu_online(cpu))
		dl_server_start(dl_se);

	प्रत्यागम 0;
}

/*
 * Detach @dl_se's bandwidth from the root domain's total_bw accounting.
 *
 * Use to dynamically unregister a dl_server's bandwidth reservation while
 * preserving its configured @dl_runtime / @dl_period. No-op if @dl_se is
 * not currently attached.
 */
शून्य dl_server_detach_bw(संरचना sched_dl_entity *dl_se)
{
	पूर्णाङ्क cpu = cpu_of(dl_se->rq);
	संरचना dl_bw *dl_b;
	पूर्णाङ्क cpus;

	यदि (!dl_se->dl_bw_attached)
		प्रत्यागम;

	dl_b = dl_bw_of(cpu);
	guard(raw_spinlock)(&dl_b->lock);
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
पूर्णाङ्क dl_server_swap_bw(संरचना sched_dl_entity *detach_se,
		      संरचना sched_dl_entity *attach_se)
{
	संरचना rq *rq = detach_se->rq;
	पूर्णाङ्क cpu = cpu_of(rq);
	संरचना dl_bw *dl_b;
	पूर्णाङ्क cpus, ret;

	WARN_ON_ONCE(attach_se->rq != rq);

	scoped_guard (raw_spinlock, &dl_bw_of(cpu)->lock) {
		dl_b = dl_bw_of(cpu);
		cpus = dl_bw_cpus(cpu);

		यदि (detach_se->dl_bw_attached)
			__dl_server_detach_bw_locked(detach_se, dl_b, cpus);

		यदि (attach_se->dl_bw_attached)
			ret = 0;
		अन्यथा
			ret = __dl_server_attach_bw_locked(attach_se, dl_b, cpus);
	}
	यदि (ret)
		प्रत्यागम ret;

	यदि (cpu_online(cpu))
		dl_server_start(attach_se);

	प्रत्यागम 0;
}

/*
 * Update the current task's runtime statistics (provided it is still
 * a -deadline task and has not been removed from the dl_rq).
 */
स्थिर शून्य update_curr_dl(संरचना rq *rq)
{
	संरचना task_struct *donor = rq->donor;
	संरचना sched_dl_entity *dl_se = &donor->dl;
	s64 delta_exec;

	यदि (!dl_task(donor) || !on_dl_rq(dl_se))
		प्रत्यागम;

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

स्थिर गणना hrtimer_restart inactive_task_timer(संरचना hrtimer *timer)
{
	संरचना sched_dl_entity *dl_se = container_of(timer,
						     संरचना sched_dl_entity,
						     inactive_timer);
	संरचना task_struct *p = रिक्त;
	संरचना rq_flags rf;
	संरचना rq *rq;

	यदि (!dl_server(dl_se)) {
		p = dl_task_of(dl_se);
		rq = task_rq_lock(p, &rf);
	} अन्यथा {
		rq = dl_se->rq;
		rq_lock(rq, &rf);
	}

	sched_clock_tick();
	update_rq_clock(rq);

	यदि (dl_server(dl_se))
		गच्छ no_task;

	यदि (!dl_task(p) || READ_ONCE(p->__state) == TASK_DEAD) {
		संरचना dl_bw *dl_b = dl_bw_of(task_cpu(p));

		यदि (READ_ONCE(p->__state) == TASK_DEAD && dl_se->dl_non_contending) {
			sub_running_bw(&p->dl, dl_rq_of_se(&p->dl));
			sub_rq_bw(&p->dl, dl_rq_of_se(&p->dl));
			dl_se->dl_non_contending = 0;
		}

		raw_spin_lock(&dl_b->lock);
		__dl_sub(dl_b, p->dl.dl_bw, dl_bw_cpus(task_cpu(p)));
		raw_spin_unlock(&dl_b->lock);
		__dl_clear_params(dl_se);

		गच्छ unlock;
	}

no_task:
	यदि (dl_se->dl_non_contending == 0)
		गच्छ unlock;

	sub_running_bw(dl_se, &rq->dl);
	dl_se->dl_non_contending = 0;
unlock:

	यदि (!dl_server(dl_se)) {
		task_rq_unlock(rq, p, &rf);
		put_task_struct(p);
	} अन्यथा {
		rq_unlock(rq, &rf);
	}

	प्रत्यागम HRTIMER_NORESTART;
}

स्थिर शून्य init_dl_inactive_task_timer(संरचना sched_dl_entity *dl_se)
{
	संरचना hrtimer *timer = &dl_se->inactive_timer;

	hrtimer_setup(timer, inactive_task_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL_HARD);
}

#define __node_2_dle(गाठ) \
	rb_entry((गाठ), संरचना sched_dl_entity, rb_गाठ)

स्थिर शून्य inc_dl_deadline(संरचना dl_rq *dl_rq, u64 deadline)
{
	संरचना rq *rq = rq_of_dl_rq(dl_rq);

	यदि (dl_rq->earliest_dl.curr == 0 ||
	    dl_time_before(deadline, dl_rq->earliest_dl.curr)) {
		यदि (dl_rq->earliest_dl.curr == 0)
			cpupri_set(&rq->rd->cpupri, rq->cpu, CPUPRI_HIGHER);
		dl_rq->earliest_dl.curr = deadline;
		cpudl_set(&rq->rd->cpudl, rq->cpu, deadline);
	}
}

स्थिर शून्य dec_dl_deadline(संरचना dl_rq *dl_rq, u64 deadline)
{
	संरचना rq *rq = rq_of_dl_rq(dl_rq);

	/*
	 * Since we may have removed our earliest (and/or next earliest)
	 * task we must recompute them.
	 */
	यदि (!dl_rq->dl_nr_running) {
		dl_rq->earliest_dl.curr = 0;
		dl_rq->earliest_dl.next = 0;
		cpudl_clear(&rq->rd->cpudl, rq->cpu, rq->online);
		cpupri_set(&rq->rd->cpupri, rq->cpu, rq->rt.highest_prio.curr);
	} अन्यथा {
		संरचना rb_गाठ *leftmost = rb_first_cached(&dl_rq->मूळ);
		संरचना sched_dl_entity *entry = __node_2_dle(leftmost);

		dl_rq->earliest_dl.curr = entry->deadline;
		cpudl_set(&rq->rd->cpudl, rq->cpu, entry->deadline);
	}
}

स्थिर अन्तःस्थ
शून्य inc_dl_tasks(संरचना sched_dl_entity *dl_se, संरचना dl_rq *dl_rq)
{
	u64 deadline = dl_se->deadline;

	dl_rq->dl_nr_running++;

	यदि (!dl_server(dl_se))
		add_nr_running(rq_of_dl_rq(dl_rq), 1);

	inc_dl_deadline(dl_rq, deadline);
}

स्थिर अन्तःस्थ
शून्य dec_dl_tasks(संरचना sched_dl_entity *dl_se, संरचना dl_rq *dl_rq)
{
	WARN_ON(!dl_rq->dl_nr_running);
	dl_rq->dl_nr_running--;

	यदि (!dl_server(dl_se))
		sub_nr_running(rq_of_dl_rq(dl_rq), 1);

	dec_dl_deadline(dl_rq, dl_se->deadline);
}

स्थिर अन्तःस्थ bool __dl_less(संरचना rb_गाठ *a, ध्रुव संरचना rb_गाठ *b)
{
	प्रत्यागम dl_time_before(__node_2_dle(a)->deadline, __node_2_dle(b)->deadline);
}

स्थिर __always_inline संरचना sched_statistics *
__schedstats_from_dl_se(संरचना sched_dl_entity *dl_se)
{
	यदि (!schedstat_enabled())
		प्रत्यागम रिक्त;

	यदि (dl_server(dl_se))
		प्रत्यागम रिक्त;

	प्रत्यागम &dl_task_of(dl_se)->stats;
}

स्थिर अन्तःस्थ शून्य
update_stats_wait_start_dl(संरचना dl_rq *dl_rq, संरचना sched_dl_entity *dl_se)
{
	संरचना sched_statistics *stats = __schedstats_from_dl_se(dl_se);
	यदि (stats)
		__update_stats_wait_start(rq_of_dl_rq(dl_rq), dl_task_of(dl_se), stats);
}

स्थिर अन्तःस्थ शून्य
update_stats_wait_end_dl(संरचना dl_rq *dl_rq, संरचना sched_dl_entity *dl_se)
{
	संरचना sched_statistics *stats = __schedstats_from_dl_se(dl_se);
	यदि (stats)
		__update_stats_wait_end(rq_of_dl_rq(dl_rq), dl_task_of(dl_se), stats);
}

स्थिर अन्तःस्थ शून्य
update_stats_enqueue_sleeper_dl(संरचना dl_rq *dl_rq, संरचना sched_dl_entity *dl_se)
{
	संरचना sched_statistics *stats = __schedstats_from_dl_se(dl_se);
	यदि (stats)
		__update_stats_enqueue_sleeper(rq_of_dl_rq(dl_rq), dl_task_of(dl_se), stats);
}

स्थिर अन्तःस्थ शून्य
update_stats_enqueue_dl(संरचना dl_rq *dl_rq, संरचना sched_dl_entity *dl_se,
			पूर्णाङ्क flags)
{
	यदि (!schedstat_enabled())
		प्रत्यागम;

	यदि (flags & ENQUEUE_WAKEUP)
		update_stats_enqueue_sleeper_dl(dl_rq, dl_se);
}

स्थिर अन्तःस्थ शून्य
update_stats_dequeue_dl(संरचना dl_rq *dl_rq, संरचना sched_dl_entity *dl_se,
			पूर्णाङ्क flags)
{
	संरचना task_struct *p = dl_task_of(dl_se);
	संरचना rq *rq = rq_of_dl_rq(dl_rq);

	यदि (!schedstat_enabled())
		प्रत्यागम;

	यदि (p != rq->curr)
		update_stats_wait_end_dl(dl_rq, dl_se);

	यदि ((flags & DEQUEUE_SLEEP)) {
		unsigned पूर्णाङ्क state;

		state = READ_ONCE(p->__state);
		यदि (state & TASK_INTERRUPTIBLE)
			__schedstat_set(p->stats.sleep_start,
					rq_clock(rq_of_dl_rq(dl_rq)));

		यदि (state & TASK_UNINTERRUPTIBLE)
			__schedstat_set(p->stats.block_start,
					rq_clock(rq_of_dl_rq(dl_rq)));
	}
}

स्थिर शून्य __enqueue_dl_entity(संरचना sched_dl_entity *dl_se)
{
	संरचना dl_rq *dl_rq = dl_rq_of_se(dl_se);

	WARN_ON_ONCE(!RB_EMPTY_गाठ(&dl_se->rb_गाठ));

	rb_add_cached(&dl_se->rb_गाठ, &dl_rq->मूळ, __dl_less);

	inc_dl_tasks(dl_se, dl_rq);
}

स्थिर शून्य __dequeue_dl_entity(संरचना sched_dl_entity *dl_se)
{
	संरचना dl_rq *dl_rq = dl_rq_of_se(dl_se);

	यदि (RB_EMPTY_गाठ(&dl_se->rb_गाठ))
		प्रत्यागम;

	rb_erase_cached(&dl_se->rb_गाठ, &dl_rq->मूळ);

	RB_CLEAR_गाठ(&dl_se->rb_गाठ);

	dec_dl_tasks(dl_se, dl_rq);
}

स्थिर शून्य
enqueue_dl_entity(संरचना sched_dl_entity *dl_se, पूर्णाङ्क flags)
{
	WARN_ON_ONCE(on_dl_rq(dl_se));

	update_stats_enqueue_dl(dl_rq_of_se(dl_se), dl_se, flags);

	/*
	 * Check if a constrained deadline task was activated
	 * after the deadline but before the next period.
	 * If that is the case, the task will be throttled and
	 * the replenishment timer will be set to the next period.
	 */
	यदि (!dl_se->dl_throttled && !dl_is_implicit(dl_se))
		dl_check_constrained_dl(dl_se);

	यदि (flags & (ENQUEUE_RESTORE|ENQUEUE_MIGRATING)) {
		संरचना dl_rq *dl_rq = dl_rq_of_se(dl_se);

		add_rq_bw(dl_se, dl_rq);
		add_running_bw(dl_se, dl_rq);
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
	यदि (!dl_se->dl_defer && dl_se->dl_throttled && !(flags & ENQUEUE_REPLENISH)) {
		यदि (flags & ENQUEUE_WAKEUP)
			task_contending(dl_se, flags);

		प्रत्यागम;
	}

	/*
	 * If this is a wakeup or a new instance, the scheduling
	 * parameters of the task might need updating. Otherwise,
	 * we want a replenishment of its runtime.
	 */
	यदि (flags & ENQUEUE_WAKEUP) {
		task_contending(dl_se, flags);
		update_dl_entity(dl_se);
	} अन्यथा यदि (flags & ENQUEUE_REPLENISH) {
		replenish_dl_entity(dl_se);
	} अन्यथा यदि ((flags & ENQUEUE_MOVE) &&
		   !is_dl_boosted(dl_se) &&
		   dl_time_before(dl_se->deadline, rq_clock(rq_of_dl_se(dl_se)))) {
		setup_new_dl_entity(dl_se);
	}

	/*
	 * If the reservation is still throttled, e.g., it got replenished but is a
	 * deferred task and still got to wait, don't enqueue.
	 */
	यदि (dl_se->dl_throttled && start_dl_timer(dl_se))
		प्रत्यागम;

	/*
	 * We're about to enqueue, make sure we're not ->dl_throttled!
	 * In case the timer was not started, say because the defer time
	 * has passed, mark as not throttled and mark unarmed.
	 * Also cancel earlier timers, since letting those run is pointless.
	 */
	यदि (dl_se->dl_throttled) {
		hrtimer_try_to_cancel(&dl_se->dl_timer);
		dl_se->dl_defer_armed = 0;
		dl_se->dl_throttled = 0;
	}

	__enqueue_dl_entity(dl_se);
}

स्थिर शून्य dequeue_dl_entity(संरचना sched_dl_entity *dl_se, पूर्णाङ्क flags)
{
	__dequeue_dl_entity(dl_se);

	यदि (flags & (DEQUEUE_SAVE|DEQUEUE_MIGRATING)) {
		संरचना dl_rq *dl_rq = dl_rq_of_se(dl_se);

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
	यदि (flags & DEQUEUE_SLEEP)
		task_non_contending(dl_se, true);
}

स्थिर शून्य enqueue_task_dl(संरचना rq *rq, संरचना task_struct *p, पूर्णाङ्क flags)
{
	संरचना sched_dl_entity *dl_se = &p->dl;
	संरचना dl_rq *dl_rq = &rq->dl;

	यदि (is_dl_boosted(dl_se)) {
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
		यदि (dl_se->dl_throttled) {
			/*
			 * The replenish timer needs to be canceled. No
			 * problem if it fires concurrently: boosted threads
			 * are ignored in dl_task_timer().
			 */
			cancel_replenish_timer(dl_se);
			dl_se->dl_throttled = 0;
		}
	} अन्यथा यदि (!dl_prio(p->normal_prio)) {
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
		यदि (!(flags & ENQUEUE_REPLENISH))
			printk_deferred_once("sched: DL de-boosted task PID %d: REPLENISH flag missing\n",
					     task_pid_nr(p));

		प्रत्यागम;
	}

	check_schedstat_required();
	update_stats_wait_start_dl(dl_rq, dl_se);

	यदि (task_on_rq_migrating(p))
		flags |= ENQUEUE_MIGRATING;

	enqueue_dl_entity(dl_se, flags);

	यदि (dl_server(dl_se))
		प्रत्यागम;

	यदि (task_is_blocked(p))
		प्रत्यागम;

	यदि (dl_rq->curr == dl_se)
		प्रत्यागम;

	यदि (!task_current(rq, p) && !dl_se->dl_throttled && p->nr_cpus_allowed > 1)
		enqueue_pushable_dl_task(rq, p);
}

स्थिर bool dequeue_task_dl(संरचना rq *rq, संरचना task_struct *p, पूर्णाङ्क flags)
{
	update_curr_dl(rq);

	यदि (task_on_rq_migrating(p))
		flags |= DEQUEUE_MIGRATING;

	dequeue_dl_entity(&p->dl, flags);
	यदि (!p->dl.dl_throttled && !dl_server(&p->dl))
		dequeue_pushable_dl_task(rq, p);

	प्रत्यागम true;
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
स्थिर शून्य yield_task_dl(संरचना rq *rq)
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

स्थिर अन्तःस्थ bool dl_task_is_earliest_deadline(संरचना task_struct *p,
						 संरचना rq *rq)
{
	प्रत्यागम (!rq->dl.dl_nr_running ||
		dl_time_before(p->dl.deadline,
			       rq->dl.earliest_dl.curr));
}

स्थिर पूर्णाङ्क find_later_rq(संरचना task_struct *task);

स्थिर पूर्णाङ्क
select_task_rq_dl(संरचना task_struct *p, पूर्णाङ्क cpu, पूर्णाङ्क flags)
{
	संरचना task_struct *curr, *donor;
	bool select_rq;
	संरचना rq *rq;

	यदि (!(flags & WF_TTWU))
		प्रत्यागम cpu;

	rq = cpu_rq(cpu);

	rcu_read_lock();
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
	यदि (sched_asym_cpucap_active())
		select_rq |= !dl_task_fits_capacity(p, cpu);

	यदि (select_rq) {
		पूर्णाङ्क target = find_later_rq(p);

		यदि (target != -1 &&
		    dl_task_is_earliest_deadline(p, cpu_rq(target)))
			cpu = target;
	}
	rcu_read_unlock();

	प्रत्यागम cpu;
}

स्थिर शून्य migrate_task_rq_dl(संरचना task_struct *p, पूर्णाङ्क new_cpu __maybe_unused)
{
	संरचना rq_flags rf;
	संरचना rq *rq;

	यदि (READ_ONCE(p->__state) != TASK_WAKING)
		प्रत्यागम;

	rq = task_rq(p);
	/*
	 * Since p->state == TASK_WAKING, set_task_cpu() has been called
	 * from try_to_wake_up(). Hence, p->pi_lock is locked, but
	 * rq->lock is not... So, lock it
	 */
	rq_lock(rq, &rf);
	यदि (p->dl.dl_non_contending) {
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

स्थिर शून्य check_preempt_equal_dl(संरचना rq *rq, संरचना task_struct *p)
{
	/*
	 * Current can't be migrated, useless to reschedule,
	 * let's hope p can move out.
	 */
	यदि (rq->curr->nr_cpus_allowed == 1 ||
	    !cpudl_find(&rq->rd->cpudl, rq->donor, रिक्त))
		प्रत्यागम;

	/*
	 * p is migratable, so let's not schedule it and
	 * see if it is pushed or pulled somewhere else.
	 */
	यदि (p->nr_cpus_allowed != 1 &&
	    cpudl_find(&rq->rd->cpudl, p, रिक्त))
		प्रत्यागम;

	resched_curr(rq);
}

स्थिर पूर्णाङ्क balance_dl(संरचना rq *rq, संरचना rq_flags *rf)
{
	/*
	 * Note, rq->donor may change during rq lock drops,
	 * so don't re-use prev across lock drops
	 */
	संरचना task_struct *p = rq->donor;

	यदि (!on_dl_rq(&p->dl) && need_pull_dl_task(rq, p)) {
		/*
		 * This is OK, because current is on_cpu, which avoids it being
		 * picked for load-balance and preemption/IRQs are still
		 * disabled avoiding further scheduler activity on it and we've
		 * not yet started the picking loop.
		 */
		rq_unpin_lock(rq, rf);
		pull_dl_task(rq);
		rq_repin_lock(rq, rf);
	}

	प्रत्यागम sched_stop_runnable(rq) || sched_dl_runnable(rq);
}

/*
 * Only called when both the current and waking task are -deadline
 * tasks.
 */
स्थिर शून्य wakeup_preempt_dl(संरचना rq *rq, संरचना task_struct *p, पूर्णाङ्क flags)
{
	संरचना task_struct *donor = rq->donor;
	/*
	 * Can only get preempted by stop-class, and those should be
	 * few and short lived, doesn't really make sense to push
	 * anything away for that.
	 */
	यदि (p->sched_class != &dl_sched_class ||
	    donor->sched_class != &dl_sched_class)
		प्रत्यागम;

	यदि (dl_entity_preempt(&p->dl, &donor->dl)) {
		resched_curr(rq);
		प्रत्यागम;
	}

	/*
	 * In the unlikely case current and p have the same deadline
	 * let us try to decide what's the best thing to do...
	 */
	यदि ((p->dl.deadline == rq->donor->dl.deadline) &&
	    !test_tsk_need_resched(rq->curr))
		check_preempt_equal_dl(rq, p);
}

#ifdef CONFIG_SCHED_HRTICK
स्थिर शून्य start_hrtick_dl(संरचना rq *rq, संरचना sched_dl_entity *dl_se)
{
	hrtick_start(rq, dl_se->runtime);
}
#अन्यथा /* !CONFIG_SCHED_HRTICK: */
स्थिर शून्य start_hrtick_dl(संरचना rq *rq, संरचना sched_dl_entity *dl_se)
{
}
#endif /* !CONFIG_SCHED_HRTICK */

/*
 * DL keeps current in tree, because ->deadline is not typically changed while
 * a task is runnable.
 */
स्थिर शून्य set_next_task_dl(संरचना rq *rq, संरचना task_struct *p, गणना snt_e type)
{
	संरचना sched_dl_entity *dl_se = &p->dl;
	संरचना dl_rq *dl_rq = &rq->dl;

	यदि (type == SNT_REPICK)
		प्रत्यागम;

	p->se.exec_start = rq_clock_task(rq);
	यदि (on_dl_rq(&p->dl))
		update_stats_wait_end_dl(dl_rq, dl_se);

	/* You can't push away the running task */
	dequeue_pushable_dl_task(rq, p);

	WARN_ON_ONCE(dl_rq->curr);
	dl_rq->curr = dl_se;

	यदि (type != SNT_PICK)
		प्रत्यागम;

	यदि (rq->donor->sched_class != &dl_sched_class)
		update_dl_rq_load_avg(rq_clock_pelt(rq), rq, 0);

	deadline_queue_ढकला_tasks(rq);

	यदि (hrtick_enabled_dl(rq))
		start_hrtick_dl(rq, &p->dl);
}

स्थिर संरचना sched_dl_entity *pick_next_dl_entity(संरचना dl_rq *dl_rq)
{
	संरचना rb_गाठ *डावा = rb_first_cached(&dl_rq->मूळ);

	यदि (!डावा)
		प्रत्यागम रिक्त;

	प्रत्यागम __node_2_dle(डावा);
}

/*
 * __pick_next_task_dl - Helper to pick the next -deadline task to run.
 * @rq: The runqueue to pick the next task from.
 */
स्थिर संरचना task_struct *__pick_task_dl(संरचना rq *rq, संरचना rq_flags *rf)
{
	संरचना sched_dl_entity *dl_se;
	संरचना dl_rq *dl_rq = &rq->dl;
	संरचना task_struct *p;

again:
	यदि (!sched_dl_runnable(rq))
		प्रत्यागम रिक्त;

	dl_se = pick_next_dl_entity(dl_rq);
	WARN_ON_ONCE(!dl_se);

	यदि (dl_server(dl_se)) {
		p = dl_se->server_pick_task(dl_se, rf);
		यदि (!p) {
			dl_server_stop(dl_se);
			गच्छ again;
		}
		rq->dl_server = dl_se;
	} अन्यथा {
		p = dl_task_of(dl_se);
	}

	प्रत्यागम p;
}

स्थिर संरचना task_struct *pick_task_dl(संरचना rq *rq, संरचना rq_flags *rf)
{
	प्रत्यागम __pick_task_dl(rq, rf);
}

स्थिर शून्य put_prev_task_dl(संरचना rq *rq, संरचना task_struct *p, संरचना task_struct *next)
{
	संरचना sched_dl_entity *dl_se = &p->dl;
	संरचना dl_rq *dl_rq = &rq->dl;

	यदि (on_dl_rq(dl_se))
		update_stats_wait_start_dl(dl_rq, dl_se);

	update_curr_dl(rq);

	update_dl_rq_load_avg(rq_clock_pelt(rq), rq, 1);

	WARN_ON_ONCE(dl_rq->curr != dl_se);
	dl_rq->curr = रिक्त;

	यदि (task_is_blocked(p))
		प्रत्यागम;

	यदि (on_dl_rq(dl_se) && p->nr_cpus_allowed > 1)
		enqueue_pushable_dl_task(rq, p);
}

/*
 * scheduler tick hitting a task of our scheduling class.
 *
 * NOTE: This function can be called remotely by the tick offload that
 * goes along full dynticks. Therefore no local assumption can be made
 * and everything must be accessed through the @rq and @curr passed in
 * parameters.
 */
स्थिर शून्य task_tick_dl(संरचना rq *rq, संरचना task_struct *p, पूर्णाङ्क queued)
{
	update_curr_dl(rq);

	update_dl_rq_load_avg(rq_clock_pelt(rq), rq, 1);
	/*
	 * Even when we have runtime, update_curr_dl() might have resulted in us
	 * not being the leftmost task anymore. In that case NEED_RESCHED will
	 * be set and schedule() will start a new hrtick for the next task.
	 */
	यदि (hrtick_enabled_dl(rq) && queued && p->dl.runtime > 0 &&
	    is_leftmost(&p->dl, &rq->dl))
		start_hrtick_dl(rq, &p->dl);
}

स्थिर शून्य task_fork_dl(संरचना task_struct *p)
{
	/*
	 * SCHED_DEADLINE tasks cannot fork and this is achieved through
	 * sched_fork()
	 */
}

/* Only try algorithms three times */
#define DL_MAX_TRIES 3

/*
 * Return the earliest pushable rq's task, which is suitable to be executed
 * on the CPU, NULL otherwise:
 */
स्थिर संरचना task_struct *pick_earliest_pushable_dl_task(संरचना rq *rq, पूर्णाङ्क cpu)
{
	संरचना task_struct *p = रिक्त;
	संरचना rb_गाठ *next_गाठ;

	यदि (!has_pushable_dl_tasks(rq))
		प्रत्यागम रिक्त;

	next_गाठ = rb_first_cached(&rq->dl.pushable_dl_tasks_मूळ);
	यावत् (next_गाठ) {
		p = __node_2_pdl(next_गाठ);

		यदि (task_is_pushable(rq, p, cpu))
			प्रत्यागम p;

		next_गाठ = rb_next(next_गाठ);
	}

	प्रत्यागम रिक्त;
}

/* Access rule: must be called on local CPU with preemption disabled */
स्थिर DEFINE_PER_CPU(cpumask_var_t, local_cpu_mask_dl);

स्थिर पूर्णाङ्क find_later_rq(संरचना task_struct *task)
{
	संरचना sched_domain *sd;
	संरचना cpumask *later_mask = this_cpu_cpumask_var_ptr(local_cpu_mask_dl);
	पूर्णाङ्क this_cpu = smp_processor_id();
	पूर्णाङ्क cpu = task_cpu(task);

	/* Make sure the mask is initialized first */
	यदि (unlikely(!later_mask))
		प्रत्यागम -1;

	यदि (task->nr_cpus_allowed == 1)
		प्रत्यागम -1;

	/*
	 * We have to consider system topology and task affinity
	 * first, then we can look for a suitable CPU.
	 */
	यदि (!cpudl_find(&task_rq(task)->rd->cpudl, task, later_mask))
		प्रत्यागम -1;

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
	यदि (cpumask_test_cpu(cpu, later_mask))
		प्रत्यागम cpu;
	/*
	 * Check if this_cpu is to be skipped (i.e., it is
	 * not in the mask) or not.
	 */
	यदि (!cpumask_test_cpu(this_cpu, later_mask))
		this_cpu = -1;

	rcu_read_lock();
	for_each_domain(cpu, sd) {
		यदि (sd->flags & SD_WAKE_AFFINE) {
			पूर्णाङ्क best_cpu;

			/*
			 * If possible, preempting this_cpu is
			 * cheaper than migrating.
			 */
			यदि (this_cpu != -1 &&
			    cpumask_test_cpu(this_cpu, sched_domain_span(sd))) {
				rcu_read_unlock();
				प्रत्यागम this_cpu;
			}

			best_cpu = cpumask_any_and_distribute(later_mask,
							      sched_domain_span(sd));
			/*
			 * Last chance: if a CPU being in both later_mask
			 * and current sd span is valid, that becomes our
			 * choice. Of course, the latest possible CPU is
			 * already under consideration through later_mask.
			 */
			यदि (best_cpu < nr_cpu_ids) {
				rcu_read_unlock();
				प्रत्यागम best_cpu;
			}
		}
	}
	rcu_read_unlock();

	/*
	 * At this point, all our guesses failed, we just return
	 * 'something', and let the caller sort the things out.
	 */
	यदि (this_cpu != -1)
		प्रत्यागम this_cpu;

	cpu = cpumask_any_distribute(later_mask);
	यदि (cpu < nr_cpu_ids)
		प्रत्यागम cpu;

	प्रत्यागम -1;
}

स्थिर संरचना task_struct *pick_next_pushable_dl_task(संरचना rq *rq)
{
	संरचना task_struct *i, *p = रिक्त;
	संरचना rb_गाठ *next_गाठ;

	यदि (!has_pushable_dl_tasks(rq))
		प्रत्यागम रिक्त;

	next_गाठ = rb_first_cached(&rq->dl.pushable_dl_tasks_मूळ);
	यावत् (next_गाठ) {
		i = __node_2_pdl(next_गाठ);
		/* skip tasks that cannot be migrated */
		यदि (!task_on_cpu(rq, i) && !is_migration_disabled(i)) {
			p = i;
			भङ्ग;
		}

		next_गाठ = rb_next(next_गाठ);
	}

	यदि (!p)
		प्रत्यागम रिक्त;

	WARN_ON_ONCE(rq->cpu != task_cpu(p));
	WARN_ON_ONCE(task_current(rq, p));
	WARN_ON_ONCE(p->nr_cpus_allowed <= 1);

	WARN_ON_ONCE(!task_on_rq_queued(p));
	WARN_ON_ONCE(!dl_task(p));

	प्रत्यागम p;
}

/* Locks the rq it finds */
स्थिर संरचना rq *find_lock_later_rq(संरचना task_struct *task, संरचना rq *rq)
{
	संरचना rq *later_rq = रिक्त;
	पूर्णाङ्क tries;
	पूर्णाङ्क cpu;

	प्रति (tries = 0; tries < DL_MAX_TRIES; tries++) {
		cpu = find_later_rq(task);

		यदि ((cpu == -1) || (cpu == rq->cpu))
			भङ्ग;

		later_rq = cpu_rq(cpu);

		यदि (!dl_task_is_earliest_deadline(task, later_rq)) {
			/*
			 * Target rq has tasks of equal or earlier deadline,
			 * retrying does not release any lock and is unlikely
			 * to yield a different result.
			 */
			later_rq = रिक्त;
			भङ्ग;
		}

		/* Retry if something changed. */
		यदि (double_lock_balance(rq, later_rq)) {
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
			यदि (unlikely(is_migration_disabled(task) ||
				     !cpumask_test_cpu(later_rq->cpu, &task->cpus_mask) ||
				     (task->dl.dl_throttled &&
				      (task_rq(task) != rq ||
				       task_on_cpu(rq, task) ||
				       !dl_task(task) ||
				       !task_on_rq_queued(task))) ||
				     (!task->dl.dl_throttled &&
				      task != pick_next_pushable_dl_task(rq)))) {

				double_unlock_balance(rq, later_rq);
				later_rq = रिक्त;
				भङ्ग;
			}
		}

		/*
		 * If the rq we found has no -deadline task, or
		 * its earliest one has a later deadline than our
		 * task, the rq is a good one.
		 */
		यदि (dl_task_is_earliest_deadline(task, later_rq))
			भङ्ग;

		/* Otherwise we try again. */
		double_unlock_balance(rq, later_rq);
		later_rq = रिक्त;
	}

	प्रत्यागम later_rq;
}

/*
 * See if the non running -deadline tasks on this rq
 * can be sent to some other CPU where they can preempt
 * and start executing.
 */
स्थिर पूर्णाङ्क ढकला_dl_task(संरचना rq *rq)
{
	संरचना task_struct *next_task;
	संरचना rq *later_rq;
	पूर्णाङ्क ret = 0;

	next_task = pick_next_pushable_dl_task(rq);
	यदि (!next_task)
		प्रत्यागम 0;

retry:
	/*
	 * If next_task preempts rq->curr, and rq->curr
	 * can move away, it makes sense to just reschedule
	 * without going further in pushing next_task.
	 */
	यदि (dl_task(rq->donor) &&
	    dl_time_before(next_task->dl.deadline, rq->donor->dl.deadline) &&
	    rq->curr->nr_cpus_allowed > 1) {
		resched_curr(rq);
		प्रत्यागम 0;
	}

	यदि (is_migration_disabled(next_task))
		प्रत्यागम 0;

	यदि (WARN_ON(next_task == rq->curr))
		प्रत्यागम 0;

	/* We might release rq lock */
	get_task_struct(next_task);

	/* Will lock the rq it'll find */
	later_rq = find_lock_later_rq(next_task, rq);
	यदि (!later_rq) {
		संरचना task_struct *task;

		/*
		 * We must check all this again, since
		 * find_lock_later_rq releases rq->lock and it is
		 * then possible that next_task has migrated.
		 */
		task = pick_next_pushable_dl_task(rq);
		यदि (task == next_task) {
			/*
			 * The task is still there. We don't try
			 * again, some other CPU will pull it when ready.
			 */
			गच्छ out;
		}

		यदि (!task)
			/* No more tasks */
			गच्छ out;

		put_task_struct(next_task);
		next_task = task;
		गच्छ retry;
	}

	move_queued_task_locked(rq, later_rq, next_task);
	ret = 1;

	resched_curr(later_rq);

	double_unlock_balance(rq, later_rq);

out:
	put_task_struct(next_task);

	प्रत्यागम ret;
}

स्थिर शून्य ढकला_dl_tasks(संरचना rq *rq)
{
	/* push_dl_task() will return true if it moved a -deadline task */
	यावत् (ढकला_dl_task(rq))
		;
}

स्थिर शून्य pull_dl_task(संरचना rq *this_rq)
{
	पूर्णाङ्क this_cpu = this_rq->cpu, cpu;
	संरचना task_struct *p, *ढकला_task;
	bool resched = false;
	संरचना rq *src_rq;
	u64 dmin = LONG_MAX;

	यदि (likely(!dl_overloaded(this_rq)))
		प्रत्यागम;

	/*
	 * Match the barrier from dl_set_overloaded; this guarantees that if we
	 * see overloaded we must also see the dlo_mask bit.
	 */
	smp_rmb();

	for_each_cpu(cpu, this_rq->rd->dlo_mask) {
		यदि (this_cpu == cpu)
			अनुवर्त;

		src_rq = cpu_rq(cpu);

		/*
		 * It looks racy, and it is! However, as in sched_rt.c,
		 * we are fine with this.
		 */
		यदि (this_rq->dl.dl_nr_running &&
		    dl_time_before(this_rq->dl.earliest_dl.curr,
				   src_rq->dl.earliest_dl.next))
			अनुवर्त;

		/* Might drop this_rq->lock */
		ढकला_task = रिक्त;
		double_lock_balance(this_rq, src_rq);

		/*
		 * If there are no more pullable tasks on the
		 * rq, we're done with it.
		 */
		यदि (src_rq->dl.dl_nr_running <= 1)
			गच्छ skip;

		p = pick_earliest_pushable_dl_task(src_rq, this_cpu);

		/*
		 * We found a task to be pulled if:
		 *  - it preempts our current (if there's one),
		 *  - it will preempt the last one we pulled (if any).
		 */
		यदि (p && dl_time_before(p->dl.deadline, dmin) &&
		    dl_task_is_earliest_deadline(p, this_rq)) {
			WARN_ON(p == src_rq->curr);
			WARN_ON(!task_on_rq_queued(p));

			/*
			 * Then we pull iff p has actually an earlier
			 * deadline than the current task of its runqueue.
			 */
			यदि (dl_time_before(p->dl.deadline,
					   src_rq->donor->dl.deadline))
				गच्छ skip;

			यदि (is_migration_disabled(p)) {
				ढकला_task = get_ढकला_task(src_rq);
			} अन्यथा {
				move_queued_task_locked(src_rq, this_rq, p);
				dmin = p->dl.deadline;
				resched = true;
			}

			/* Is there any other task even earlier? */
		}
skip:
		double_unlock_balance(this_rq, src_rq);

		यदि (ढकला_task) {
			preempt_disable();
			raw_spin_rq_unlock(this_rq);
			stop_one_cpu_nowait(src_rq->cpu, ढकला_cpu_stop,
					    ढकला_task, &src_rq->ढकला_work);
			preempt_enable();
			raw_spin_rq_lock(this_rq);
		}
	}

	यदि (resched)
		resched_curr(this_rq);
}

/*
 * Since the task is not running and a reschedule is not going to happen
 * anytime soon on its runqueue, we try pushing it away now.
 */
स्थिर शून्य task_woken_dl(संरचना rq *rq, संरचना task_struct *p)
{
	यदि (!task_on_cpu(rq, p) &&
	    !test_tsk_need_resched(rq->curr) &&
	    p->nr_cpus_allowed > 1 &&
	    dl_task(rq->donor) &&
	    (rq->curr->nr_cpus_allowed < 2 ||
	     !dl_entity_preempt(&p->dl, &rq->donor->dl))) {
		ढकला_dl_tasks(rq);
	}
}

स्थिर शून्य set_cpus_allowed_dl(संरचना task_struct *p,
				संरचना affinity_context *ctx)
{
	संरचना rq *rq;

	WARN_ON_ONCE(!dl_task(p));

	rq = task_rq(p);
	/*
	 * Migrating a SCHED_DEADLINE task between exclusive
	 * cpusets (different root_domains) entails a bandwidth
	 * update. We already made space for us in the destination
	 * domain (see cpuset_can_attach()).
	 */
	यदि (dl_task_needs_bw_move(p, ctx->new_mask)) {
		संरचना dl_bw *src_dl_b;

		src_dl_b = dl_bw_of(cpu_of(rq));
		/*
		 * We now free resources of the root_domain we are migrating
		 * off. In the worst case, sched_setattr() may temporary fail
		 * until we complete the update.
		 */
		raw_spin_lock(&src_dl_b->lock);
		__dl_sub(src_dl_b, p->dl.dl_bw, dl_bw_cpus(task_cpu(p)));
		raw_spin_unlock(&src_dl_b->lock);
	}

	set_cpus_allowed_common(p, ctx);
}

bool dl_task_needs_bw_move(संरचना task_struct *p,
			   ध्रुव संरचना cpumask *new_mask)
{
	यदि (!dl_task(p))
		प्रत्यागम false;

	प्रत्यागम !cpumask_intersects(task_rq(p)->rd->span, new_mask);
}

/* Assumes rq->lock is held */
स्थिर शून्य rq_online_dl(संरचना rq *rq)
{
	यदि (rq->dl.overloaded)
		dl_set_overload(rq);

	यदि (rq->dl.dl_nr_running > 0)
		cpudl_set(&rq->rd->cpudl, rq->cpu, rq->dl.earliest_dl.curr);
	अन्यथा
		cpudl_clear(&rq->rd->cpudl, rq->cpu, true);
}

/* Assumes rq->lock is held */
स्थिर शून्य rq_offline_dl(संरचना rq *rq)
{
	यदि (rq->dl.overloaded)
		dl_clear_overload(rq);

	cpudl_clear(&rq->rd->cpudl, rq->cpu, false);
}

शून्य __init init_sched_dl_class(शून्य)
{
	unsigned पूर्णाङ्क i;

	for_each_possible_cpu(i)
		zalloc_cpumask_var_गाठ(&per_cpu(local_cpu_mask_dl, i),
					GFP_KERNEL, cpu_to_गाठ(i));
}

/*
 * This function always returns a non-empty bitmap in @cpus. This is because
 * if a root domain has reserved bandwidth for DL tasks, the DL bandwidth
 * check will prevent CPU hotplug from deactivating all CPUs in that domain.
 */
स्थिर शून्य dl_get_task_effective_cpus(संरचना task_struct *p, संरचना cpumask *cpus)
{
	ध्रुव संरचना cpumask *hk_msk;

	hk_msk = housekeeping_cpumask(HK_TYPE_DOMAIN);
	यदि (housekeeping_enabled(HK_TYPE_DOMAIN)) {
		यदि (!cpumask_intersects(p->cpus_ptr, hk_msk)) {
			/*
			 * CPUs isolated by isolcpu="domain" always belong to
			 * def_root_domain.
			 */
			cpumask_andnot(cpus, cpu_active_mask, hk_msk);
			प्रत्यागम;
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
शून्य dl_add_task_मूळ_domain(संरचना task_struct *p)
{
	संरचना rq_flags rf;
	संरचना rq *rq;
	संरचना dl_bw *dl_b;
	unsigned पूर्णाङ्क cpu;
	संरचना cpumask *msk;

	raw_spin_lock_irqsave(&p->pi_lock, rf.flags);
	यदि (!dl_task(p) || dl_entity_is_special(&p->dl)) {
		raw_spin_unlock_irqrestore(&p->pi_lock, rf.flags);
		प्रत्यागम;
	}

	msk = this_cpu_cpumask_var_ptr(local_cpu_mask_dl);
	dl_get_task_effective_cpus(p, msk);
	cpu = cpumask_first_and(cpu_active_mask, msk);
	BUG_ON(cpu >= nr_cpu_ids);
	rq = cpu_rq(cpu);
	dl_b = &rq->rd->dl_bw;

	raw_spin_lock(&dl_b->lock);
	__dl_add(dl_b, p->dl.dl_bw, cpumask_weight(rq->rd->span));
	raw_spin_unlock(&dl_b->lock);
	raw_spin_unlock_irqrestore(&p->pi_lock, rf.flags);
}

स्थिर शून्य dl_server_add_bw(संरचना मूळ_domain *rd, पूर्णाङ्क cpu)
{
	संरचना sched_dl_entity *dl_se;

	dl_se = &cpu_rq(cpu)->fair_server;
	यदि (dl_server(dl_se) && dl_se->dl_bw_attached && cpu_active(cpu))
		__dl_add(&rd->dl_bw, dl_se->dl_bw, dl_bw_cpus(cpu));

#ifdef CONFIG_SCHED_CLASS_EXT
	dl_se = &cpu_rq(cpu)->ext_server;
	यदि (dl_server(dl_se) && dl_se->dl_bw_attached && cpu_active(cpu))
		__dl_add(&rd->dl_bw, dl_se->dl_bw, dl_bw_cpus(cpu));
#endif
}

स्थिर u64 dl_server_read_bw(पूर्णाङ्क cpu)
{
	u64 dl_bw = 0;

	यदि (cpu_rq(cpu)->fair_server.dl_server &&
	    cpu_rq(cpu)->fair_server.dl_bw_attached)
		dl_bw += cpu_rq(cpu)->fair_server.dl_bw;

#ifdef CONFIG_SCHED_CLASS_EXT
	यदि (cpu_rq(cpu)->ext_server.dl_server &&
	    cpu_rq(cpu)->ext_server.dl_bw_attached)
		dl_bw += cpu_rq(cpu)->ext_server.dl_bw;
#endif

	प्रत्यागम dl_bw;
}

शून्य dl_clear_मूळ_domain(संरचना मूळ_domain *rd)
{
	पूर्णाङ्क i;

	guard(raw_spinlock_irqsave)(&rd->dl_bw.lock);

	/*
	 * Reset total_bw to zero and extra_bw to max_bw so that next
	 * loop will add dl-servers contributions back properly,
	 */
	rd->dl_bw.total_bw = 0;
	for_each_cpu(i, rd->span)
		cpu_rq(i)->dl.extra_bw = cpu_rq(i)->dl.max_bw;

	/*
	 * dl_servers are not tasks. Since dl_add_task_root_domain ignores
	 * them, we need to account for them here explicitly.
	 */
	for_each_cpu(i, rd->span)
		dl_server_add_bw(rd, i);
}

शून्य dl_clear_मूळ_domain_cpu(पूर्णाङ्क cpu)
{
	dl_clear_मूळ_domain(cpu_rq(cpu)->rd);
}

स्थिर शून्य switched_from_dl(संरचना rq *rq, संरचना task_struct *p)
{
	/*
	 * task_non_contending() can start the "inactive timer" (if the 0-lag
	 * time is in the future). If the task switches back to dl before
	 * the "inactive timer" fires, it can continue to consume its current
	 * runtime using its current deadline. If it stays outside of
	 * SCHED_DEADLINE until the 0-lag time passes, inactive_task_timer()
	 * will reset the task parameters.
	 */
	यदि (task_on_rq_queued(p) && p->dl.dl_runtime)
		task_non_contending(&p->dl, false);

	/*
	 * In case a task is setscheduled out from SCHED_DEADLINE we need to
	 * keep track of that on its cpuset (for correct bandwidth tracking).
	 */
	dec_dl_tasks_cs(p);

	यदि (!task_on_rq_queued(p)) {
		/*
		 * Inactive timer is armed. However, p is leaving DEADLINE and
		 * might migrate away from this rq while continuing to run on
		 * some other class. We need to remove its contribution from
		 * this rq running_bw now, or sub_rq_bw (below) will complain.
		 */
		यदि (p->dl.dl_non_contending)
			sub_running_bw(&p->dl, &rq->dl);
		sub_rq_bw(&p->dl, &rq->dl);
	}

	/*
	 * We cannot use inactive_task_timer() to invoke sub_running_bw()
	 * at the 0-lag time, because the task could have been migrated
	 * while SCHED_OTHER in the meanwhile.
	 */
	यदि (p->dl.dl_non_contending)
		p->dl.dl_non_contending = 0;

	/*
	 * Since this might be the only -deadline task on the rq,
	 * this is the right place to try to pull some other one
	 * from an overloaded CPU, if any.
	 */
	यदि (!task_on_rq_queued(p) || rq->dl.dl_nr_running)
		प्रत्यागम;

	deadline_queue_pull_task(rq);
}

/*
 * When switching to -deadline, we may overload the rq, then
 * we try to push someone off, if possible.
 */
स्थिर शून्य switched_to_dl(संरचना rq *rq, संरचना task_struct *p)
{
	cancel_inactive_timer(&p->dl);

	/*
	 * In case a task is setscheduled to SCHED_DEADLINE we need to keep
	 * track of that on its cpuset (for correct bandwidth tracking).
	 */
	inc_dl_tasks_cs(p);

	/* If p is not queued we will update its parameters at next wakeup. */
	यदि (!task_on_rq_queued(p)) {
		add_rq_bw(&p->dl, &rq->dl);

		प्रत्यागम;
	}

	यदि (rq->donor != p) {
		यदि (p->nr_cpus_allowed > 1 && rq->dl.overloaded)
			deadline_queue_ढकला_tasks(rq);
		यदि (dl_task(rq->donor))
			wakeup_preempt_dl(rq, p, 0);
		अन्यथा
			resched_curr(rq);
	} अन्यथा {
		update_dl_rq_load_avg(rq_clock_pelt(rq), rq, 0);
	}
}

स्थिर u64 get_prio_dl(संरचना rq *rq, संरचना task_struct *p)
{
	/*
	 * Make sure to update current so we don't return a stale value.
	 */
	यदि (task_current_donor(rq, p))
		update_curr_dl(rq);

	प्रत्यागम p->dl.deadline;
}

/*
 * If the scheduling parameters of a -deadline task changed,
 * a push or pull operation might be needed.
 */
स्थिर शून्य prio_changed_dl(संरचना rq *rq, संरचना task_struct *p, u64 old_deadline)
{
	यदि (!task_on_rq_queued(p))
		प्रत्यागम;

	यदि (p->dl.deadline == old_deadline)
		प्रत्यागम;

	यदि (dl_time_before(old_deadline, p->dl.deadline))
		deadline_queue_pull_task(rq);

	यदि (task_current_donor(rq, p)) {
		/*
		 * If we now have a earlier deadline task than p,
		 * then reschedule, provided p is still on this
		 * runqueue.
		 */
		यदि (dl_time_before(rq->dl.earliest_dl.curr, p->dl.deadline))
			resched_curr(rq);
	} अन्यथा {
		/*
		 * Current may not be deadline in case p was throttled but we
		 * have just replenished it (e.g. rt_mutex_setprio()).
		 *
		 * Otherwise, if p was given an earlier deadline, reschedule.
		 */
		यदि (!dl_task(rq->curr) ||
		    dl_time_before(p->dl.deadline, rq->curr->dl.deadline))
			resched_curr(rq);
	}
}

#ifdef CONFIG_SCHED_CORE
स्थिर पूर्णाङ्क task_is_throttled_dl(संरचना task_struct *p, पूर्णाङ्क cpu)
{
	प्रत्यागम p->dl.dl_throttled;
}
#endif

DEFINE_SCHED_CLASS(dl) = {
	.enqueue_task		= enqueue_task_dl,
	.dequeue_task		= dequeue_task_dl,
	.yield_task		= yield_task_dl,

	.wakeup_preempt		= wakeup_preempt_dl,

	.pick_task		= pick_task_dl,
	.put_prev_task		= put_prev_task_dl,
	.set_next_task		= set_next_task_dl,

	.balance		= balance_dl,
	.select_task_rq		= select_task_rq_dl,
	.migrate_task_rq	= migrate_task_rq_dl,
	.set_cpus_allowed       = set_cpus_allowed_dl,
	.rq_online              = rq_online_dl,
	.rq_offline             = rq_offline_dl,
	.task_woken		= task_woken_dl,
	.find_lock_rq		= find_lock_later_rq,

	.task_tick		= task_tick_dl,
	.task_fork              = task_fork_dl,

	.get_prio		= get_prio_dl,
	.prio_changed           = prio_changed_dl,
	.switched_from		= switched_from_dl,
	.switched_to		= switched_to_dl,

	.update_curr		= update_curr_dl,
#ifdef CONFIG_SCHED_CORE
	.task_is_throttled	= task_is_throttled_dl,
#endif
};

/*
 * Used for dl_bw check and update, used under sched_rt_handler()::mutex and
 * sched_domains_mutex.
 */
u64 dl_cookie;

पूर्णाङ्क sched_dl_global_validate(शून्य)
{
	u64 runtime = global_rt_runtime();
	u64 period = global_rt_period();
	u64 new_bw = to_ratio(period, runtime);
	u64 cookie = ++dl_cookie;
	संरचना dl_bw *dl_b;
	पूर्णाङ्क cpu, cpus, ret = 0;
	unsigned long flags;

	/*
	 * Here we want to check the bandwidth not being set to some
	 * value smaller than the currently allocated bandwidth in
	 * any of the root_domains.
	 */
	for_each_online_cpu(cpu) {
		rcu_read_lock_sched();

		यदि (dl_bw_visited(cpu, cookie))
			गच्छ next;

		dl_b = dl_bw_of(cpu);
		cpus = dl_bw_cpus(cpu);

		raw_spin_lock_irqsave(&dl_b->lock, flags);
		यदि (new_bw * cpus < dl_b->total_bw)
			ret = -EBUSY;
		raw_spin_unlock_irqrestore(&dl_b->lock, flags);

next:
		rcu_read_unlock_sched();

		यदि (ret)
			भङ्ग;
	}

	प्रत्यागम ret;
}

स्थिर शून्य init_dl_rq_bw_ratio(संरचना dl_rq *dl_rq)
{
	यदि (global_rt_runtime() == RUNTIME_INF) {
		dl_rq->bw_ratio = 1 << RATIO_SHIFT;
		dl_rq->max_bw = dl_rq->extra_bw = 1 << BW_SHIFT;
	} अन्यथा {
		dl_rq->bw_ratio = to_ratio(global_rt_runtime(),
			  global_rt_period()) >> (BW_SHIFT - RATIO_SHIFT);
		dl_rq->max_bw = dl_rq->extra_bw =
			to_ratio(global_rt_period(), global_rt_runtime());
	}
}

शून्य sched_dl_do_global(शून्य)
{
	u64 new_bw = -1;
	u64 cookie = ++dl_cookie;
	संरचना dl_bw *dl_b;
	पूर्णाङ्क cpu;
	unsigned long flags;

	यदि (global_rt_runtime() != RUNTIME_INF)
		new_bw = to_ratio(global_rt_period(), global_rt_runtime());

	for_each_possible_cpu(cpu)
		init_dl_rq_bw_ratio(&cpu_rq(cpu)->dl);

	for_each_possible_cpu(cpu) {
		rcu_read_lock_sched();

		यदि (dl_bw_visited(cpu, cookie)) {
			rcu_read_unlock_sched();
			अनुवर्त;
		}

		dl_b = dl_bw_of(cpu);

		raw_spin_lock_irqsave(&dl_b->lock, flags);
		dl_b->bw = new_bw;
		raw_spin_unlock_irqrestore(&dl_b->lock, flags);

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
पूर्णाङ्क sched_dl_overflow(संरचना task_struct *p, पूर्णाङ्क policy,
		      ध्रुव संरचना sched_attr *attr)
{
	u64 period = attr->sched_period ?: attr->sched_deadline;
	u64 runtime = attr->sched_runtime;
	u64 new_bw = dl_policy(policy) ? to_ratio(period, runtime) : 0;
	पूर्णाङ्क cpus, err = -1, cpu = task_cpu(p);
	संरचना dl_bw *dl_b = dl_bw_of(cpu);
	unsigned long cap;

	यदि (attr->sched_flags & SCHED_FLAG_SUGOV)
		प्रत्यागम 0;

	/* !deadline task may carry old deadline bandwidth */
	यदि (new_bw == p->dl.dl_bw && task_has_dl_policy(p))
		प्रत्यागम 0;

	/*
	 * Either if a task, enters, leave, or stays -deadline but changes
	 * its parameters, we may need to update accordingly the total
	 * allocated bandwidth of the container.
	 */
	raw_spin_lock(&dl_b->lock);
	cpus = dl_bw_cpus(cpu);
	cap = dl_bw_capacity(cpu);

	यदि (dl_policy(policy) && !task_has_dl_policy(p) &&
	    !__dl_overflow(dl_b, cap, 0, new_bw)) {
		यदि (hrtimer_active(&p->dl.inactive_timer))
			__dl_sub(dl_b, p->dl.dl_bw, cpus);
		__dl_add(dl_b, new_bw, cpus);
		err = 0;
	} अन्यथा यदि (dl_policy(policy) && task_has_dl_policy(p) &&
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
	} अन्यथा यदि (!dl_policy(policy) && task_has_dl_policy(p)) {
		/*
		 * Do not decrease the total deadline utilization here,
		 * switched_from_dl() will take care to do it at the correct
		 * (0-lag) time.
		 */
		err = 0;
	}
	raw_spin_unlock(&dl_b->lock);

	प्रत्यागम err;
}

/*
 * This function initializes the sched_dl_entity of a newly becoming
 * SCHED_DEADLINE task.
 *
 * Only the static values are considered here, the actual runtime and the
 * absolute deadline will be properly calculated when the task is enqueued
 * for the first time with its new policy.
 */
शून्य __setparam_dl(संरचना task_struct *p, ध्रुव संरचना sched_attr *attr)
{
	संरचना sched_dl_entity *dl_se = &p->dl;

	dl_se->dl_runtime = attr->sched_runtime;
	dl_se->dl_deadline = attr->sched_deadline;
	dl_se->dl_period = attr->sched_period ?: dl_se->dl_deadline;
	dl_se->flags = attr->sched_flags & SCHED_DL_FLAGS;
	dl_se->dl_bw = to_ratio(dl_se->dl_period, dl_se->dl_runtime);
	dl_se->dl_density = to_ratio(dl_se->dl_deadline, dl_se->dl_runtime);
}

शून्य __getparam_dl(संरचना task_struct *p, संरचना sched_attr *attr, unsigned पूर्णाङ्क flags)
{
	संरचना sched_dl_entity *dl_se = &p->dl;
	संरचना rq *rq = task_rq(p);
	u64 adj_deadline;

	attr->sched_priority = p->rt_priority;
	यदि (flags & SCHED_GETATTR_FLAG_DL_DYNAMIC) {
		guard(raw_spinlock_irq)(&rq->__lock);
		update_rq_clock(rq);
		यदि (task_current(rq, p))
			update_curr_dl(rq);

		attr->sched_runtime = dl_se->runtime;
		adj_deadline = dl_se->deadline - rq_clock(rq) + ktime_get_ns();
		attr->sched_deadline = adj_deadline;
	} अन्यथा {
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
bool __checkparam_dl(ध्रुव संरचना sched_attr *attr)
{
	u64 period, max, min;

	/* special dl tasks don't actually use any parameter */
	यदि (attr->sched_flags & SCHED_FLAG_SUGOV)
		प्रत्यागम true;

	/* deadline != 0 */
	यदि (attr->sched_deadline == 0)
		प्रत्यागम false;

	/*
	 * Since we truncate DL_SCALE bits, make sure we're at least
	 * that big.
	 */
	यदि (attr->sched_runtime < (1ULL << DL_SCALE))
		प्रत्यागम false;

	/*
	 * Since we use the MSB for wrap-around and sign issues, make
	 * sure it's not set (mind that period can be equal to zero).
	 */
	यदि (attr->sched_deadline & (1ULL << 63) ||
	    attr->sched_period & (1ULL << 63))
		प्रत्यागम false;

	period = attr->sched_period;
	यदि (!period)
		period = attr->sched_deadline;

	/* runtime <= deadline <= period (if period != 0) */
	यदि (period < attr->sched_deadline ||
	    attr->sched_deadline < attr->sched_runtime)
		प्रत्यागम false;

	max = (u64)READ_ONCE(sysctl_sched_dl_period_max) * NSEC_PER_USEC;
	min = (u64)READ_ONCE(sysctl_sched_dl_period_min) * NSEC_PER_USEC;

	यदि (period < min || period > max)
		प्रत्यागम false;

	प्रत्यागम true;
}

/*
 * This function clears the sched_dl_entity static params.
 */
स्थिर शून्य __dl_clear_params(संरचना sched_dl_entity *dl_se)
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

#ifdef CONFIG_RT_MUTEXES
	dl_se->pi_se			= dl_se;
#endif
}

शून्य init_dl_entity(संरचना sched_dl_entity *dl_se)
{
	RB_CLEAR_गाठ(&dl_se->rb_गाठ);
	init_dl_task_timer(dl_se);
	init_dl_inactive_task_timer(dl_se);
	__dl_clear_params(dl_se);
}

bool dl_param_changed(संरचना task_struct *p, ध्रुव संरचना sched_attr *attr)
{
	संरचना sched_dl_entity *dl_se = &p->dl;

	यदि (dl_se->dl_runtime != attr->sched_runtime ||
	    dl_se->dl_deadline != attr->sched_deadline ||
	    dl_se->dl_period != attr->sched_period ||
	    dl_se->flags != (attr->sched_flags & SCHED_DL_FLAGS))
		प्रत्यागम true;

	प्रत्यागम false;
}

पूर्णाङ्क dl_cpuset_cpumask_can_shrink(ध्रुव संरचना cpumask *cur,
				 ध्रुव संरचना cpumask *trial)
{
	unsigned long flags, cap;
	संरचना dl_bw *cur_dl_b;
	पूर्णाङ्क ret = 1;

	rcu_read_lock_sched();
	cur_dl_b = dl_bw_of(cpumask_any(cur));
	cap = __dl_bw_capacity(trial);
	raw_spin_lock_irqsave(&cur_dl_b->lock, flags);
	यदि (__dl_overflow(cur_dl_b, cap, 0, 0))
		ret = 0;
	raw_spin_unlock_irqrestore(&cur_dl_b->lock, flags);
	rcu_read_unlock_sched();

	प्रत्यागम ret;
}

गणना dl_bw_request {
	dl_bw_req_deactivate = 0,
	dl_bw_req_alloc,
	dl_bw_req_free
};

स्थिर पूर्णाङ्क dl_bw_manage(गणना dl_bw_request req, पूर्णाङ्क cpu, u64 dl_bw)
{
	unsigned long flags, cap;
	संरचना dl_bw *dl_b;
	bool overflow = 0;
	u64 dl_server_bw = 0;

	rcu_read_lock_sched();
	dl_b = dl_bw_of(cpu);
	raw_spin_lock_irqsave(&dl_b->lock, flags);

	cap = dl_bw_capacity(cpu);
	विकल्प (req) {
	पक्ष dl_bw_req_free:
		__dl_sub(dl_b, dl_bw, dl_bw_cpus(cpu));
		भङ्ग;
	पक्ष dl_bw_req_alloc:
		overflow = __dl_overflow(dl_b, cap, 0, dl_bw);

		यदि (!overflow) {
			/*
			 * We reserve space in the destination
			 * root_domain, as we can't fail after this point.
			 * We will free resources in the source root_domain
			 * later on (see set_cpus_allowed_dl()).
			 */
			__dl_add(dl_b, dl_bw, dl_bw_cpus(cpu));
		}
		भङ्ग;
	पक्ष dl_bw_req_deactivate:
		/*
		 * cpu is not off yet, but we need to do the math by
		 * considering it off already (i.e., what would happen if we
		 * turn cpu off?).
		 */
		cap -= arch_scale_cpu_capacity(cpu);

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
		यदि (dl_b->total_bw - dl_server_bw > 0) {
			/*
			 * Leaving at least one CPU for DEADLINE tasks seems a
			 * wise thing to do. As said above, cpu is not offline
			 * yet, so account for that.
			 */
			यदि (dl_bw_cpus(cpu) - 1)
				overflow = __dl_overflow(dl_b, cap, dl_server_bw, 0);
			अन्यथा
				overflow = 1;
		}

		भङ्ग;
	}

	raw_spin_unlock_irqrestore(&dl_b->lock, flags);
	rcu_read_unlock_sched();

	प्रत्यागम overflow ? -EBUSY : 0;
}

पूर्णाङ्क dl_bw_deactivate(पूर्णाङ्क cpu)
{
	प्रत्यागम dl_bw_manage(dl_bw_req_deactivate, cpu, 0);
}

पूर्णाङ्क dl_bw_alloc(पूर्णाङ्क cpu, u64 dl_bw)
{
	प्रत्यागम dl_bw_manage(dl_bw_req_alloc, cpu, dl_bw);
}

शून्य dl_bw_free(पूर्णाङ्क cpu, u64 dl_bw)
{
	dl_bw_manage(dl_bw_req_free, cpu, dl_bw);
}

शून्य print_dl_stats(संरचना seq_file *m, पूर्णाङ्क cpu)
{
	print_dl_rq(m, cpu, &cpu_rq(cpu)->dl);
}
