/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _KERNEL_STATS_H
#define _KERNEL_STATS_H

#ifdef CONFIG_SCHEDSTATS

خارجي بنية static_key_false sched_schedstats;

/*
 * Expects runqueue lock to be held for atomicity of update
 */
ساكن مضمن فراغ
rq_sched_info_arrive(بنية rq *rq, unsigned long long delta)
{
	אם (rq) {
		rq->rq_sched_info.run_delay += delta;
		rq->rq_sched_info.pcount++;
	}
}

/*
 * Expects runqueue lock to be held for atomicity of update
 */
ساكن مضمن فراغ
rq_sched_info_depart(بنية rq *rq, unsigned long long delta)
{
	אם (rq)
		rq->rq_cpu_time += delta;
}

ساكن مضمن فراغ
rq_sched_info_dequeue(بنية rq *rq, unsigned long long delta)
{
	אם (rq)
		rq->rq_sched_info.run_delay += delta;
}
#define   schedstat_enabled()		static_branch_unlikely(&sched_schedstats)
#define __schedstat_inc(var)		עשה { var++; } כלעוד (0)
#define   schedstat_inc(var)		עשה { אם (schedstat_enabled()) { var++; } } כלעוד (0)
#define __schedstat_add(var, amt)	עשה { var += (amt); } כלעוד (0)
#define   schedstat_add(var, amt)	עשה { אם (schedstat_enabled()) { var += (amt); } } כלעוד (0)
#define __schedstat_set(var, val)	עשה { var = (val); } כלעוד (0)
#define   schedstat_set(var, val)	עשה { אם (schedstat_enabled()) { var = (val); } } כלעוד (0)
#define   schedstat_val(var)		(var)
#define   schedstat_val_or_zero(var)	((schedstat_enabled()) ? (var) : 0)

فراغ __update_stats_wait_start(بنية rq *rq, بنية task_struct *p,
			       بنية sched_statistics *stats);

فراغ __update_stats_wait_end(بنية rq *rq, بنية task_struct *p,
			     بنية sched_statistics *stats);
فراغ __update_stats_enqueue_sleeper(بنية rq *rq, بنية task_struct *p,
				    بنية sched_statistics *stats);

ساكن مضمن فراغ
check_schedstat_required(فراغ)
{
	אם (schedstat_enabled())
		החזר;

	/* Force schedstat enabled if a dependent tracepoint is active */
	אם (trace_sched_stat_wait_enabled()    ||
	    trace_sched_stat_sleep_enabled()   ||
	    trace_sched_stat_iowait_enabled()  ||
	    trace_sched_stat_blocked_enabled() ||
	    trace_sched_stat_runtime_enabled())
		printk_deferred_once("Scheduler tracepoints stat_sleep, stat_iowait, stat_blocked and stat_runtime require the kernel parameter schedstats=enable or kernel.sched_schedstats=1\n");
}

#אחרת /* !CONFIG_SCHEDSTATS: */

ساكن مضمن فراغ rq_sched_info_arrive  (بنية rq *rq, unsigned long long delta) { }
ساكن مضمن فراغ rq_sched_info_dequeue(بنية rq *rq, unsigned long long delta) { }
ساكن مضمن فراغ rq_sched_info_depart  (بنية rq *rq, unsigned long long delta) { }
# define   schedstat_enabled()		0
# define __schedstat_inc(var)		עשה { } כלעוד (0)
# define   schedstat_inc(var)		עשה { } כלעוד (0)
# define __schedstat_add(var, amt)	עשה { } כלעוד (0)
# define   schedstat_add(var, amt)	עשה { } כלעוד (0)
# define __schedstat_set(var, val)	עשה { } כלעוד (0)
# define   schedstat_set(var, val)	עשה { } כלעוד (0)
# define   schedstat_val(var)		0
# define   schedstat_val_or_zero(var)	0

# define __update_stats_wait_start(rq, p, stats)       עשה { } כלעוד (0)
# define __update_stats_wait_end(rq, p, stats)         עשה { } כלעוד (0)
# define __update_stats_enqueue_sleeper(rq, p, stats)  עשה { } כלעוד (0)
# define check_schedstat_required()                    עשה { } כלעוד (0)

#endif /* CONFIG_SCHEDSTATS */

ساكن مضمن بنية sched_statistics *
__schedstats_from_se(بنية sched_entity *se)
{
#ifdef CONFIG_FAIR_GROUP_SCHED
	אם (!entity_is_task(se))
		החזר &container_of(se, بنية cfs_tg_state, se)->stats;
#endif
	החזר &task_of(se)->stats;
}

#ifdef CONFIG_PSI
فراغ psi_task_change(بنية task_struct *task, صحيح clear, صحيح set);
فراغ psi_task_switch(بنية task_struct *prev, بنية task_struct *next,
		     bool sleep);
#ifdef CONFIG_IRQ_TIME_ACCOUNTING
فراغ psi_account_irqtime(بنية rq *rq, بنية task_struct *curr, بنية task_struct *prev);
#אחרת /* !CONFIG_IRQ_TIME_ACCOUNTING: */
ساكن مضمن فراغ psi_account_irqtime(بنية rq *rq, بنية task_struct *curr,
				       بنية task_struct *prev) {}
#endif /* !CONFIG_IRQ_TIME_ACCOUNTING */
/*
 * PSI tracks state that persists across sleeps, such as iowaits and
 * memory stalls. As a result, it has to distinguish between sleeps,
 * where a task's runnable state changes, and migrations, where a task
 * and its runnable state are being moved between CPUs and runqueues.
 *
 * A notable case is a task whose dequeue is delayed. PSI considers
 * those sleeping, but because they are still on the runqueue they can
 * go through migration requeues. In this case, *sleeping* states need
 * to be transferred.
 */
ساكن مضمن فراغ psi_enqueue(بنية task_struct *p, صحيح flags)
{
	صحيح clear = 0, set = 0;

	אם (static_branch_likely(&psi_disabled))
		החזר;

	/* Same runqueue, nothing changed for psi */
	אם (flags & ENQUEUE_RESTORE)
		החזר;

	/* psi_sched_switch() will handle the flags */
	אם (task_on_cpu(task_rq(p), p))
		החזר;

	אם (p->se.sched_delayed) {
		/* CPU migration of "sleeping" task */
		WARN_ON_ONCE(!(flags & ENQUEUE_MIGRATED));
		אם (p->in_memstall)
			set |= TSK_MEMSTALL;
		אם (p->in_iowait)
			set |= TSK_IOWAIT;
	} אחרת אם (flags & ENQUEUE_MIGRATED) {
		/* CPU migration of runnable task */
		set = TSK_RUNNING;
		אם (p->in_memstall)
			set |= TSK_MEMSTALL | TSK_MEMSTALL_RUNNING;
	} אחרת {
		/* Wakeup of new or sleeping task */
		אם (p->in_iowait)
			clear |= TSK_IOWAIT;
		set = TSK_RUNNING;
		אם (p->in_memstall)
			set |= TSK_MEMSTALL_RUNNING;
	}

	psi_task_change(p, clear, set);
}

ساكن مضمن فراغ psi_dequeue(بنية task_struct *p, صحيح flags)
{
	אם (static_branch_likely(&psi_disabled))
		החזר;

	/* Same runqueue, nothing changed for psi */
	אם (flags & DEQUEUE_SAVE)
		החזר;

	/*
	 * A voluntary sleep is a dequeue followed by a task switch. To
	 * avoid walking all ancestors twice, psi_task_switch() handles
	 * TSK_RUNNING and TSK_IOWAIT for us when it moves TSK_ONCPU.
	 * Do nothing here.
	 *
	 * In the SCHED_PROXY_EXECUTION case we may do sleeping
	 * dequeues that are not followed by a task switch, so check
	 * TSK_ONCPU is set to ensure the task switch is imminent.
	 * Otherwise clear the flags as usual.
	 */
	אם ((flags & DEQUEUE_SLEEP) && (p->psi_flags & TSK_ONCPU))
		החזר;

	/*
	 * When migrating a task to another CPU, clear all psi
	 * state. The enqueue callback above will work it out.
	 */
	psi_task_change(p, p->psi_flags, 0);
}

ساكن مضمن فراغ psi_ttwu_dequeue(بنية task_struct *p)
{
	אם (static_branch_likely(&psi_disabled))
		החזר;
	/*
	 * Is the task being migrated during a wakeup? Make sure to
	 * deregister its sleep-persistent psi states from the old
	 * queue, and let psi_enqueue() know it has to requeue.
	 */
	אם (unlikely(p->psi_flags)) {
		بنية rq_flags rf;
		بنية rq *rq;

		rq = __task_rq_lock(p, &rf);
		psi_task_change(p, p->psi_flags, 0);
		__task_rq_unlock(rq, p, &rf);
	}
}

ساكن مضمن فراغ psi_sched_switch(بنية task_struct *prev,
				    بنية task_struct *next,
				    bool sleep)
{
	אם (static_branch_likely(&psi_disabled))
		החזר;

	psi_task_switch(prev, next, sleep);
}

#אחרת /* !CONFIG_PSI: */
ساكن مضمن فراغ psi_enqueue(بنية task_struct *p, bool migrate) {}
ساكن مضمن فراغ psi_dequeue(بنية task_struct *p, bool migrate) {}
ساكن مضمن فراغ psi_ttwu_dequeue(بنية task_struct *p) {}
ساكن مضمن فراغ psi_sched_switch(بنية task_struct *prev,
				    بنية task_struct *next,
				    bool sleep) {}
ساكن مضمن فراغ psi_account_irqtime(بنية rq *rq, بنية task_struct *curr,
				       بنية task_struct *prev) {}
#endif /* !CONFIG_PSI */

#ifdef CONFIG_SCHED_INFO
/*
 * We are interested in knowing how long it was from the *first* time a
 * task was queued to the time that it finally hit a CPU, we call this routine
 * from dequeue_task() to account for possible rq->clock skew across CPUs. The
 * delta taken on each CPU would annul the skew.
 */
ساكن مضمن فراغ sched_info_dequeue(بنية rq *rq, بنية task_struct *t)
{
	unsigned long long delta = 0;

	אם (!t->sched_info.last_queued)
		החזר;

	delta = rq_clock(rq) - t->sched_info.last_queued;
	t->sched_info.last_queued = 0;
	t->sched_info.run_delay += delta;
	אם (delta > t->sched_info.max_run_delay) {
		t->sched_info.max_run_delay = delta;
		ktime_get_real_ts64(&t->sched_info.max_run_delay_ts);
	}
	אם (delta && (!t->sched_info.min_run_delay || delta < t->sched_info.min_run_delay))
		t->sched_info.min_run_delay = delta;
	rq_sched_info_dequeue(rq, delta);
}

/*
 * Called when a task finally hits the CPU.  We can now calculate how
 * long it was waiting to run.  We also note when it began so that we
 * can keep stats on how long its time-slice is.
 */
ساكن فراغ sched_info_arrive(بنية rq *rq, بنية task_struct *t)
{
	unsigned long long now, delta = 0;

	אם (!t->sched_info.last_queued)
		החזר;

	now = rq_clock(rq);
	delta = now - t->sched_info.last_queued;
	t->sched_info.last_queued = 0;
	t->sched_info.run_delay += delta;
	t->sched_info.last_arrival = now;
	t->sched_info.pcount++;
	אם (delta > t->sched_info.max_run_delay) {
		t->sched_info.max_run_delay = delta;
		ktime_get_real_ts64(&t->sched_info.max_run_delay_ts);
	}
	אם (delta && (!t->sched_info.min_run_delay || delta < t->sched_info.min_run_delay))
		t->sched_info.min_run_delay = delta;

	rq_sched_info_arrive(rq, delta);
}

/*
 * This function is only called from enqueue_task(), but also only updates
 * the timestamp if it is already not set.  It's assumed that
 * sched_info_dequeue() will clear that stamp when appropriate.
 */
ساكن مضمن فراغ sched_info_enqueue(بنية rq *rq, بنية task_struct *t)
{
	אם (!t->sched_info.last_queued)
		t->sched_info.last_queued = rq_clock(rq);
}

/*
 * Called when a process ceases being the active-running process involuntarily
 * due, typically, to expiring its time slice (this may also be called when
 * switching to the idle task).  Now we can calculate how long we ran.
 * Also, if the process is still in the TASK_RUNNING state, call
 * sched_info_enqueue() to mark that it has now again started waiting on
 * the runqueue.
 */
ساكن مضمن فراغ sched_info_depart(بنية rq *rq, بنية task_struct *t)
{
	unsigned long long delta = rq_clock(rq) - t->sched_info.last_arrival;

	rq_sched_info_depart(rq, delta);

	אם (task_is_running(t))
		sched_info_enqueue(rq, t);
}

/*
 * Called when tasks are switched involuntarily due, typically, to expiring
 * their time slice.  (This may also be called when switching to or from
 * the idle task.)  We are only called when prev != next.
 */
ساكن مضمن فراغ
sched_info_switch(بنية rq *rq, بنية task_struct *prev, بنية task_struct *next)
{
	/*
	 * prev now departs the CPU.  It's not interesting to record
	 * stats about how efficient we were at scheduling the idle
	 * process, however.
	 */
	אם (prev != rq->idle)
		sched_info_depart(rq, prev);

	אם (next != rq->idle)
		sched_info_arrive(rq, next);
}

#אחרת /* !CONFIG_SCHED_INFO: */
# define sched_info_enqueue(rq, t)	עשה { } כלעוד (0)
# define sched_info_dequeue(rq, t)	עשה { } כלעוד (0)
# define sched_info_switch(rq, t, next)	עשה { } כלעוד (0)
#endif /* !CONFIG_SCHED_INFO */

#endif /* _KERNEL_STATS_H */
