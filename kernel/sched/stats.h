/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _KERNEL_STATS_H
#define _KERNEL_STATS_H

#ifdef CONFIG_SCHEDSTATS

بیرونی ساختار static_کلید_false sched_schedstats;

/*
 * Expects runqueue lock to be held for atomicity of update
 */
ایستا درونخطی تهی
rq_sched_info_arrive(ساختار rq *rq, unsigned long long delta)
{
	اگر (rq) {
		rq->rq_sched_info.run_delay += delta;
		rq->rq_sched_info.pcount++;
	}
}

/*
 * Expects runqueue lock to be held for atomicity of update
 */
ایستا درونخطی تهی
rq_sched_info_depart(ساختار rq *rq, unsigned long long delta)
{
	اگر (rq)
		rq->rq_cpu_time += delta;
}

ایستا درونخطی تهی
rq_sched_info_dequeue(ساختار rq *rq, unsigned long long delta)
{
	اگر (rq)
		rq->rq_sched_info.run_delay += delta;
}
#define   schedstat_enabled()		static_branch_unlikely(&sched_schedstats)
#define __schedstat_inc(var)		انجامبده { var++; } وقتی (0)
#define   schedstat_inc(var)		انجامبده { اگر (schedstat_enabled()) { var++; } } وقتی (0)
#define __schedstat_add(var, amt)	انجامبده { var += (amt); } وقتی (0)
#define   schedstat_add(var, amt)	انجامبده { اگر (schedstat_enabled()) { var += (amt); } } وقتی (0)
#define __schedstat_set(var, val)	انجامبده { var = (val); } وقتی (0)
#define   schedstat_set(var, val)	انجامبده { اگر (schedstat_enabled()) { var = (val); } } وقتی (0)
#define   schedstat_val(var)		(var)
#define   schedstat_val_or_zero(var)	((schedstat_enabled()) ? (var) : 0)

تهی __update_stats_wait_start(ساختار rq *rq, ساختار task_struct *p,
			       ساختار sched_statistics *stats);

تهی __update_stats_wait_end(ساختار rq *rq, ساختار task_struct *p,
			     ساختار sched_statistics *stats);
تهی __update_stats_enqueue_sleeper(ساختار rq *rq, ساختار task_struct *p,
				    ساختار sched_statistics *stats);

ایستا درونخطی تهی
check_schedstat_required(تهی)
{
	اگر (schedstat_enabled())
		بازگشت;

	/* Force schedstat enabled if a dependent tracepoint is active */
	اگر (trace_sched_stat_wait_enabled()    ||
	    trace_sched_stat_sleep_enabled()   ||
	    trace_sched_stat_iowait_enabled()  ||
	    trace_sched_stat_blocked_enabled() ||
	    trace_sched_stat_runtime_enabled())
		printk_deferred_once("Scheduler tracepoints stat_sleep, stat_iowait, stat_blocked and stat_runtime require the kernel parameter schedstats=enable or kernel.sched_schedstats=1\n");
}

#وگرنه /* !CONFIG_SCHEDSTATS: */

ایستا درونخطی تهی rq_sched_info_arrive  (ساختار rq *rq, unsigned long long delta) { }
ایستا درونخطی تهی rq_sched_info_dequeue(ساختار rq *rq, unsigned long long delta) { }
ایستا درونخطی تهی rq_sched_info_depart  (ساختار rq *rq, unsigned long long delta) { }
# define   schedstat_enabled()		0
# define __schedstat_inc(var)		انجامبده { } وقتی (0)
# define   schedstat_inc(var)		انجامبده { } وقتی (0)
# define __schedstat_add(var, amt)	انجامبده { } وقتی (0)
# define   schedstat_add(var, amt)	انجامبده { } وقتی (0)
# define __schedstat_set(var, val)	انجامبده { } وقتی (0)
# define   schedstat_set(var, val)	انجامبده { } وقتی (0)
# define   schedstat_val(var)		0
# define   schedstat_val_or_zero(var)	0

# define __update_stats_wait_start(rq, p, stats)       انجامبده { } وقتی (0)
# define __update_stats_wait_end(rq, p, stats)         انجامبده { } وقتی (0)
# define __update_stats_enqueue_sleeper(rq, p, stats)  انجامبده { } وقتی (0)
# define check_schedstat_required()                    انجامبده { } وقتی (0)

#endif /* CONFIG_SCHEDSTATS */

ایستا درونخطی ساختار sched_statistics *
__schedstats_from_se(ساختار sched_entity *se)
{
#ifdef CONFIG_FAIR_GROUP_SCHED
	اگر (!entity_is_task(se))
		بازگشت &container_of(se, ساختار cfs_tg_state, se)->stats;
#endif
	بازگشت &task_of(se)->stats;
}

#ifdef CONFIG_PSI
تهی psi_task_change(ساختار task_struct *task, صحیح clear, صحیح set);
تهی psi_task_switch(ساختار task_struct *prev, ساختار task_struct *next,
		     bool sleep);
#ifdef CONFIG_IRQ_TIME_ACCOUNTING
تهی psi_account_irqtime(ساختار rq *rq, ساختار task_struct *curr, ساختار task_struct *prev);
#وگرنه /* !CONFIG_IRQ_TIME_ACCOUNTING: */
ایستا درونخطی تهی psi_account_irqtime(ساختار rq *rq, ساختار task_struct *curr,
				       ساختار task_struct *prev) {}
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
ایستا درونخطی تهی psi_enqueue(ساختار task_struct *p, صحیح flags)
{
	صحیح clear = 0, set = 0;

	اگر (static_branch_likely(&psi_disabled))
		بازگشت;

	/* Same runqueue, nothing changed for psi */
	اگر (flags & ENQUEUE_RESTORE)
		بازگشت;

	/* psi_sched_switch() will handle the flags */
	اگر (task_on_cpu(task_rq(p), p))
		بازگشت;

	اگر (p->se.sched_delayed) {
		/* CPU migration of "sleeping" task */
		WARN_ON_ONCE(!(flags & ENQUEUE_MIGRATED));
		اگر (p->in_memstall)
			set |= TSK_MEMSTALL;
		اگر (p->in_iowait)
			set |= TSK_IOWAIT;
	} وگرنه اگر (flags & ENQUEUE_MIGRATED) {
		/* CPU migration of runnable task */
		set = TSK_RUNNING;
		اگر (p->in_memstall)
			set |= TSK_MEMSTALL | TSK_MEMSTALL_RUNNING;
	} وگرنه {
		/* Wakeup of new or sleeping task */
		اگر (p->in_iowait)
			clear |= TSK_IOWAIT;
		set = TSK_RUNNING;
		اگر (p->in_memstall)
			set |= TSK_MEMSTALL_RUNNING;
	}

	psi_task_change(p, clear, set);
}

ایستا درونخطی تهی psi_dequeue(ساختار task_struct *p, صحیح flags)
{
	اگر (static_branch_likely(&psi_disabled))
		بازگشت;

	/* Same runqueue, nothing changed for psi */
	اگر (flags & DEQUEUE_SAVE)
		بازگشت;

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
	اگر ((flags & DEQUEUE_SLEEP) && (p->psi_flags & TSK_ONCPU))
		بازگشت;

	/*
	 * When migrating a task to another CPU, clear all psi
	 * state. The enqueue callback above will work it out.
	 */
	psi_task_change(p, p->psi_flags, 0);
}

ایستا درونخطی تهی psi_ttwu_dequeue(ساختار task_struct *p)
{
	اگر (static_branch_likely(&psi_disabled))
		بازگشت;
	/*
	 * Is the task being migrated during a wakeup? Make sure to
	 * deregister its sleep-persistent psi states from the old
	 * queue, and let psi_enqueue() know it has to requeue.
	 */
	اگر (unlikely(p->psi_flags)) {
		ساختار rq_flags rf;
		ساختار rq *rq;

		rq = __task_rq_lock(p, &rf);
		psi_task_change(p, p->psi_flags, 0);
		__task_rq_unlock(rq, p, &rf);
	}
}

ایستا درونخطی تهی psi_sched_switch(ساختار task_struct *prev,
				    ساختار task_struct *next,
				    bool sleep)
{
	اگر (static_branch_likely(&psi_disabled))
		بازگشت;

	psi_task_switch(prev, next, sleep);
}

#وگرنه /* !CONFIG_PSI: */
ایستا درونخطی تهی psi_enqueue(ساختار task_struct *p, bool migrate) {}
ایستا درونخطی تهی psi_dequeue(ساختار task_struct *p, bool migrate) {}
ایستا درونخطی تهی psi_ttwu_dequeue(ساختار task_struct *p) {}
ایستا درونخطی تهی psi_sched_switch(ساختار task_struct *prev,
				    ساختار task_struct *next,
				    bool sleep) {}
ایستا درونخطی تهی psi_account_irqtime(ساختار rq *rq, ساختار task_struct *curr,
				       ساختار task_struct *prev) {}
#endif /* !CONFIG_PSI */

#ifdef CONFIG_SCHED_INFO
/*
 * We are interested in knowing how long it was from the *first* time a
 * task was queued to the time that it finally hit a CPU, we call this routine
 * from dequeue_task() to account for possible rq->clock skew across CPUs. The
 * delta taken on each CPU would annul the skew.
 */
ایستا درونخطی تهی sched_info_dequeue(ساختار rq *rq, ساختار task_struct *t)
{
	unsigned long long delta = 0;

	اگر (!t->sched_info.last_queued)
		بازگشت;

	delta = rq_clock(rq) - t->sched_info.last_queued;
	t->sched_info.last_queued = 0;
	t->sched_info.run_delay += delta;
	اگر (delta > t->sched_info.max_run_delay) {
		t->sched_info.max_run_delay = delta;
		ktime_get_real_ts64(&t->sched_info.max_run_delay_ts);
	}
	اگر (delta && (!t->sched_info.min_run_delay || delta < t->sched_info.min_run_delay))
		t->sched_info.min_run_delay = delta;
	rq_sched_info_dequeue(rq, delta);
}

/*
 * Called when a task finally hits the CPU.  We can now calculate how
 * long it was waiting to run.  We also note when it began so that we
 * can keep stats on how long its time-slice is.
 */
ایستا تهی sched_info_arrive(ساختار rq *rq, ساختار task_struct *t)
{
	unsigned long long now, delta = 0;

	اگر (!t->sched_info.last_queued)
		بازگشت;

	now = rq_clock(rq);
	delta = now - t->sched_info.last_queued;
	t->sched_info.last_queued = 0;
	t->sched_info.run_delay += delta;
	t->sched_info.last_arrival = now;
	t->sched_info.pcount++;
	اگر (delta > t->sched_info.max_run_delay) {
		t->sched_info.max_run_delay = delta;
		ktime_get_real_ts64(&t->sched_info.max_run_delay_ts);
	}
	اگر (delta && (!t->sched_info.min_run_delay || delta < t->sched_info.min_run_delay))
		t->sched_info.min_run_delay = delta;

	rq_sched_info_arrive(rq, delta);
}

/*
 * This function is only called from enqueue_task(), but also only updates
 * the timestamp if it is already not set.  It's assumed that
 * sched_info_dequeue() will clear that stamp when appropriate.
 */
ایستا درونخطی تهی sched_info_enqueue(ساختار rq *rq, ساختار task_struct *t)
{
	اگر (!t->sched_info.last_queued)
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
ایستا درونخطی تهی sched_info_depart(ساختار rq *rq, ساختار task_struct *t)
{
	unsigned long long delta = rq_clock(rq) - t->sched_info.last_arrival;

	rq_sched_info_depart(rq, delta);

	اگر (task_is_running(t))
		sched_info_enqueue(rq, t);
}

/*
 * Called when tasks are switched involuntarily due, typically, to expiring
 * their time slice.  (This may also be called when switching to or from
 * the idle task.)  We are only called when prev != next.
 */
ایستا درونخطی تهی
sched_info_switch(ساختار rq *rq, ساختار task_struct *prev, ساختار task_struct *next)
{
	/*
	 * prev now departs the CPU.  It's not interesting to record
	 * stats about how efficient we were at scheduling the idle
	 * process, however.
	 */
	اگر (prev != rq->idle)
		sched_info_depart(rq, prev);

	اگر (next != rq->idle)
		sched_info_arrive(rq, next);
}

#وگرنه /* !CONFIG_SCHED_INFO: */
# define sched_info_enqueue(rq, t)	انجامبده { } وقتی (0)
# define sched_info_dequeue(rq, t)	انجامبده { } وقتی (0)
# define sched_info_switch(rq, t, next)	انجامبده { } وقتی (0)
#endif /* !CONFIG_SCHED_INFO */

#endif /* _KERNEL_STATS_H */
