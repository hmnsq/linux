/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _KERNEL_STATS_H
#define _KERNEL_STATS_H

#ifdef CONFIG_SCHEDSTATS

ಬಾಹ್ಯ ಸಂರಚನಾ static_ಕೀಲಿ_false sched_schedstats;

/*
 * Expects runqueue lock to be held for atomicity of update
 */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
rq_sched_info_arrive(ಸಂರಚನಾ rq *rq, unsigned long long delta)
{
	ಯದಿ (rq) {
		rq->rq_sched_info.run_delay += delta;
		rq->rq_sched_info.pcount++;
	}
}

/*
 * Expects runqueue lock to be held for atomicity of update
 */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
rq_sched_info_depart(ಸಂರಚನಾ rq *rq, unsigned long long delta)
{
	ಯದಿ (rq)
		rq->rq_cpu_time += delta;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
rq_sched_info_dequeue(ಸಂರಚನಾ rq *rq, unsigned long long delta)
{
	ಯದಿ (rq)
		rq->rq_sched_info.run_delay += delta;
}
#define   schedstat_enabled()		static_branch_unlikely(&sched_schedstats)
#define __schedstat_inc(var)		ಕುರು { var++; } ಯಾವತ್ (0)
#define   schedstat_inc(var)		ಕುರು { ಯದಿ (schedstat_enabled()) { var++; } } ಯಾವತ್ (0)
#define __schedstat_add(var, amt)	ಕುರು { var += (amt); } ಯಾವತ್ (0)
#define   schedstat_add(var, amt)	ಕುರು { ಯದಿ (schedstat_enabled()) { var += (amt); } } ಯಾವತ್ (0)
#define __schedstat_set(var, val)	ಕುರು { var = (val); } ಯಾವತ್ (0)
#define   schedstat_set(var, val)	ಕುರು { ಯದಿ (schedstat_enabled()) { var = (val); } } ಯಾವತ್ (0)
#define   schedstat_val(var)		(var)
#define   schedstat_val_or_zero(var)	((schedstat_enabled()) ? (var) : 0)

ಶೂನ್ಯ __update_stats_wait_start(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p,
			       ಸಂರಚನಾ sched_statistics *stats);

ಶೂನ್ಯ __update_stats_wait_end(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p,
			     ಸಂರಚನಾ sched_statistics *stats);
ಶೂನ್ಯ __update_stats_enqueue_sleeper(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p,
				    ಸಂರಚನಾ sched_statistics *stats);

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
check_schedstat_required(ಶೂನ್ಯ)
{
	ಯದಿ (schedstat_enabled())
		ಪ್ರತ್ಯಾಗಮ;

	/* Force schedstat enabled if a dependent tracepoint is active */
	ಯದಿ (trace_sched_stat_wait_enabled()    ||
	    trace_sched_stat_sleep_enabled()   ||
	    trace_sched_stat_iowait_enabled()  ||
	    trace_sched_stat_blocked_enabled() ||
	    trace_sched_stat_runtime_enabled())
		printk_deferred_once("Scheduler tracepoints stat_sleep, stat_iowait, stat_blocked and stat_runtime require the kernel parameter schedstats=enable or kernel.sched_schedstats=1\n");
}

#ಅನ್ಯಥಾ /* !CONFIG_SCHEDSTATS: */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ rq_sched_info_arrive  (ಸಂರಚನಾ rq *rq, unsigned long long delta) { }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ rq_sched_info_dequeue(ಸಂರಚನಾ rq *rq, unsigned long long delta) { }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ rq_sched_info_depart  (ಸಂರಚನಾ rq *rq, unsigned long long delta) { }
# define   schedstat_enabled()		0
# define __schedstat_inc(var)		ಕುರು { } ಯಾವತ್ (0)
# define   schedstat_inc(var)		ಕುರು { } ಯಾವತ್ (0)
# define __schedstat_add(var, amt)	ಕುರು { } ಯಾವತ್ (0)
# define   schedstat_add(var, amt)	ಕುರು { } ಯಾವತ್ (0)
# define __schedstat_set(var, val)	ಕುರು { } ಯಾವತ್ (0)
# define   schedstat_set(var, val)	ಕುರು { } ಯಾವತ್ (0)
# define   schedstat_val(var)		0
# define   schedstat_val_or_zero(var)	0

# define __update_stats_wait_start(rq, p, stats)       ಕುರು { } ಯಾವತ್ (0)
# define __update_stats_wait_end(rq, p, stats)         ಕುರು { } ಯಾವತ್ (0)
# define __update_stats_enqueue_sleeper(rq, p, stats)  ಕುರು { } ಯಾವತ್ (0)
# define check_schedstat_required()                    ಕುರು { } ಯಾವತ್ (0)

#endif /* CONFIG_SCHEDSTATS */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ sched_statistics *
__schedstats_from_se(ಸಂರಚನಾ sched_entity *se)
{
#ifdef CONFIG_FAIR_GROUP_SCHED
	ಯದಿ (!entity_is_task(se))
		ಪ್ರತ್ಯಾಗಮ &container_of(se, ಸಂರಚನಾ cfs_tg_state, se)->stats;
#endif
	ಪ್ರತ್ಯಾಗಮ &task_of(se)->stats;
}

#ifdef CONFIG_PSI
ಶೂನ್ಯ psi_task_change(ಸಂರಚನಾ task_struct *task, ಪೂರ್ಣಾಙ್ಕ clear, ಪೂರ್ಣಾಙ್ಕ set);
ಶೂನ್ಯ psi_task_switch(ಸಂರಚನಾ task_struct *prev, ಸಂರಚನಾ task_struct *next,
		     bool sleep);
#ifdef CONFIG_IRQ_TIME_ACCOUNTING
ಶೂನ್ಯ psi_account_irqtime(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *curr, ಸಂರಚನಾ task_struct *prev);
#ಅನ್ಯಥಾ /* !CONFIG_IRQ_TIME_ACCOUNTING: */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ psi_account_irqtime(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *curr,
				       ಸಂರಚನಾ task_struct *prev) {}
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
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ psi_enqueue(ಸಂರಚನಾ task_struct *p, ಪೂರ್ಣಾಙ್ಕ flags)
{
	ಪೂರ್ಣಾಙ್ಕ clear = 0, set = 0;

	ಯದಿ (static_branch_likely(&psi_disabled))
		ಪ್ರತ್ಯಾಗಮ;

	/* Same runqueue, nothing changed for psi */
	ಯದಿ (flags & ENQUEUE_RESTORE)
		ಪ್ರತ್ಯಾಗಮ;

	/* psi_sched_switch() will handle the flags */
	ಯದಿ (task_on_cpu(task_rq(p), p))
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (p->se.sched_delayed) {
		/* CPU migration of "sleeping" task */
		WARN_ON_ONCE(!(flags & ENQUEUE_MIGRATED));
		ಯದಿ (p->in_memstall)
			set |= TSK_MEMSTALL;
		ಯದಿ (p->in_iowait)
			set |= TSK_IOWAIT;
	} ಅನ್ಯಥಾ ಯದಿ (flags & ENQUEUE_MIGRATED) {
		/* CPU migration of runnable task */
		set = TSK_RUNNING;
		ಯದಿ (p->in_memstall)
			set |= TSK_MEMSTALL | TSK_MEMSTALL_RUNNING;
	} ಅನ್ಯಥಾ {
		/* Wakeup of new or sleeping task */
		ಯದಿ (p->in_iowait)
			clear |= TSK_IOWAIT;
		set = TSK_RUNNING;
		ಯದಿ (p->in_memstall)
			set |= TSK_MEMSTALL_RUNNING;
	}

	psi_task_change(p, clear, set);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ psi_dequeue(ಸಂರಚನಾ task_struct *p, ಪೂರ್ಣಾಙ್ಕ flags)
{
	ಯದಿ (static_branch_likely(&psi_disabled))
		ಪ್ರತ್ಯಾಗಮ;

	/* Same runqueue, nothing changed for psi */
	ಯದಿ (flags & DEQUEUE_SAVE)
		ಪ್ರತ್ಯಾಗಮ;

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
	ಯದಿ ((flags & DEQUEUE_SLEEP) && (p->psi_flags & TSK_ONCPU))
		ಪ್ರತ್ಯಾಗಮ;

	/*
	 * When migrating a task to another CPU, clear all psi
	 * state. The enqueue callback above will work it out.
	 */
	psi_task_change(p, p->psi_flags, 0);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ psi_ttwu_dequeue(ಸಂರಚನಾ task_struct *p)
{
	ಯದಿ (static_branch_likely(&psi_disabled))
		ಪ್ರತ್ಯಾಗಮ;
	/*
	 * Is the task being migrated during a wakeup? Make sure to
	 * deregister its sleep-persistent psi states from the old
	 * queue, and let psi_enqueue() know it has to requeue.
	 */
	ಯದಿ (unlikely(p->psi_flags)) {
		ಸಂರಚನಾ rq_flags rf;
		ಸಂರಚನಾ rq *rq;

		rq = __task_rq_lock(p, &rf);
		psi_task_change(p, p->psi_flags, 0);
		__task_rq_unlock(rq, p, &rf);
	}
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ psi_sched_switch(ಸಂರಚನಾ task_struct *prev,
				    ಸಂರಚನಾ task_struct *next,
				    bool sleep)
{
	ಯದಿ (static_branch_likely(&psi_disabled))
		ಪ್ರತ್ಯಾಗಮ;

	psi_task_switch(prev, next, sleep);
}

#ಅನ್ಯಥಾ /* !CONFIG_PSI: */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ psi_enqueue(ಸಂರಚನಾ task_struct *p, bool migrate) {}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ psi_dequeue(ಸಂರಚನಾ task_struct *p, bool migrate) {}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ psi_ttwu_dequeue(ಸಂರಚನಾ task_struct *p) {}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ psi_sched_switch(ಸಂರಚನಾ task_struct *prev,
				    ಸಂರಚನಾ task_struct *next,
				    bool sleep) {}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ psi_account_irqtime(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *curr,
				       ಸಂರಚನಾ task_struct *prev) {}
#endif /* !CONFIG_PSI */

#ifdef CONFIG_SCHED_INFO
/*
 * We are interested in knowing how long it was from the *first* time a
 * task was queued to the time that it finally hit a CPU, we call this routine
 * from dequeue_task() to account for possible rq->clock skew across CPUs. The
 * delta taken on each CPU would annul the skew.
 */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ sched_info_dequeue(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *t)
{
	unsigned long long delta = 0;

	ಯದಿ (!t->sched_info.last_queued)
		ಪ್ರತ್ಯಾಗಮ;

	delta = rq_clock(rq) - t->sched_info.last_queued;
	t->sched_info.last_queued = 0;
	t->sched_info.run_delay += delta;
	ಯದಿ (delta > t->sched_info.max_run_delay) {
		t->sched_info.max_run_delay = delta;
		ktime_get_real_ts64(&t->sched_info.max_run_delay_ts);
	}
	ಯದಿ (delta && (!t->sched_info.min_run_delay || delta < t->sched_info.min_run_delay))
		t->sched_info.min_run_delay = delta;
	rq_sched_info_dequeue(rq, delta);
}

/*
 * Called when a task finally hits the CPU.  We can now calculate how
 * long it was waiting to run.  We also note when it began so that we
 * can keep stats on how long its time-slice is.
 */
ಸ್ಥಿರ ಶೂನ್ಯ sched_info_arrive(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *t)
{
	unsigned long long now, delta = 0;

	ಯದಿ (!t->sched_info.last_queued)
		ಪ್ರತ್ಯಾಗಮ;

	now = rq_clock(rq);
	delta = now - t->sched_info.last_queued;
	t->sched_info.last_queued = 0;
	t->sched_info.run_delay += delta;
	t->sched_info.last_arrival = now;
	t->sched_info.pcount++;
	ಯದಿ (delta > t->sched_info.max_run_delay) {
		t->sched_info.max_run_delay = delta;
		ktime_get_real_ts64(&t->sched_info.max_run_delay_ts);
	}
	ಯದಿ (delta && (!t->sched_info.min_run_delay || delta < t->sched_info.min_run_delay))
		t->sched_info.min_run_delay = delta;

	rq_sched_info_arrive(rq, delta);
}

/*
 * This function is only called from enqueue_task(), but also only updates
 * the timestamp if it is already not set.  It's assumed that
 * sched_info_dequeue() will clear that stamp when appropriate.
 */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ sched_info_enqueue(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *t)
{
	ಯದಿ (!t->sched_info.last_queued)
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
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ sched_info_depart(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *t)
{
	unsigned long long delta = rq_clock(rq) - t->sched_info.last_arrival;

	rq_sched_info_depart(rq, delta);

	ಯದಿ (task_is_running(t))
		sched_info_enqueue(rq, t);
}

/*
 * Called when tasks are switched involuntarily due, typically, to expiring
 * their time slice.  (This may also be called when switching to or from
 * the idle task.)  We are only called when prev != next.
 */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
sched_info_switch(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *prev, ಸಂರಚನಾ task_struct *next)
{
	/*
	 * prev now departs the CPU.  It's not interesting to record
	 * stats about how efficient we were at scheduling the idle
	 * process, however.
	 */
	ಯದಿ (prev != rq->idle)
		sched_info_depart(rq, prev);

	ಯದಿ (next != rq->idle)
		sched_info_arrive(rq, next);
}

#ಅನ್ಯಥಾ /* !CONFIG_SCHED_INFO: */
# define sched_info_enqueue(rq, t)	ಕುರು { } ಯಾವತ್ (0)
# define sched_info_dequeue(rq, t)	ಕುರು { } ಯಾವತ್ (0)
# define sched_info_switch(rq, t, next)	ಕುರು { } ಯಾವತ್ (0)
#endif /* !CONFIG_SCHED_INFO */

#endif /* _KERNEL_STATS_H */
