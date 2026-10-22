/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _KERNEL_STATS_H
#define _KERNEL_STATS_H

#ifdef CONFIG_SCHEDSTATS

வெளி கட்டமைப்பு static_சாவி_false sched_schedstats;

/*
 * Expects runqueue lock to be held for atomicity of update
 */
நிலையான உள்வரி வெறுமை
rq_sched_info_arrive(கட்டமைப்பு rq *rq, unsigned long long delta)
{
	எனில் (rq) {
		rq->rq_sched_info.run_delay += delta;
		rq->rq_sched_info.pcount++;
	}
}

/*
 * Expects runqueue lock to be held for atomicity of update
 */
நிலையான உள்வரி வெறுமை
rq_sched_info_depart(கட்டமைப்பு rq *rq, unsigned long long delta)
{
	எனில் (rq)
		rq->rq_cpu_time += delta;
}

நிலையான உள்வரி வெறுமை
rq_sched_info_dequeue(கட்டமைப்பு rq *rq, unsigned long long delta)
{
	எனில் (rq)
		rq->rq_sched_info.run_delay += delta;
}
#define   schedstat_enabled()		static_branch_unlikely(&sched_schedstats)
#define __schedstat_inc(var)		செய் { var++; } போது (0)
#define   schedstat_inc(var)		செய் { எனில் (schedstat_enabled()) { var++; } } போது (0)
#define __schedstat_add(var, amt)	செய் { var += (amt); } போது (0)
#define   schedstat_add(var, amt)	செய் { எனில் (schedstat_enabled()) { var += (amt); } } போது (0)
#define __schedstat_set(var, val)	செய் { var = (val); } போது (0)
#define   schedstat_set(var, val)	செய் { எனில் (schedstat_enabled()) { var = (val); } } போது (0)
#define   schedstat_val(var)		(var)
#define   schedstat_val_or_zero(var)	((schedstat_enabled()) ? (var) : 0)

வெறுமை __update_stats_wait_start(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p,
			       கட்டமைப்பு sched_statistics *stats);

வெறுமை __update_stats_wait_end(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p,
			     கட்டமைப்பு sched_statistics *stats);
வெறுமை __update_stats_enqueue_sleeper(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p,
				    கட்டமைப்பு sched_statistics *stats);

நிலையான உள்வரி வெறுமை
check_schedstat_required(வெறுமை)
{
	எனில் (schedstat_enabled())
		திரும்பு;

	/* Force schedstat enabled if a dependent tracepoint is active */
	எனில் (trace_sched_stat_wait_enabled()    ||
	    trace_sched_stat_sleep_enabled()   ||
	    trace_sched_stat_iowait_enabled()  ||
	    trace_sched_stat_blocked_enabled() ||
	    trace_sched_stat_runtime_enabled())
		printk_deferred_once("Scheduler tracepoints stat_sleep, stat_iowait, stat_blocked and stat_runtime require the kernel parameter schedstats=enable or kernel.sched_schedstats=1\n");
}

#இல்லையெனில் /* !CONFIG_SCHEDSTATS: */

நிலையான உள்வரி வெறுமை rq_sched_info_arrive  (கட்டமைப்பு rq *rq, unsigned long long delta) { }
நிலையான உள்வரி வெறுமை rq_sched_info_dequeue(கட்டமைப்பு rq *rq, unsigned long long delta) { }
நிலையான உள்வரி வெறுமை rq_sched_info_depart  (கட்டமைப்பு rq *rq, unsigned long long delta) { }
# define   schedstat_enabled()		0
# define __schedstat_inc(var)		செய் { } போது (0)
# define   schedstat_inc(var)		செய் { } போது (0)
# define __schedstat_add(var, amt)	செய் { } போது (0)
# define   schedstat_add(var, amt)	செய் { } போது (0)
# define __schedstat_set(var, val)	செய் { } போது (0)
# define   schedstat_set(var, val)	செய் { } போது (0)
# define   schedstat_val(var)		0
# define   schedstat_val_or_zero(var)	0

# define __update_stats_wait_start(rq, p, stats)       செய் { } போது (0)
# define __update_stats_wait_end(rq, p, stats)         செய் { } போது (0)
# define __update_stats_enqueue_sleeper(rq, p, stats)  செய் { } போது (0)
# define check_schedstat_required()                    செய் { } போது (0)

#endif /* CONFIG_SCHEDSTATS */

நிலையான உள்வரி கட்டமைப்பு sched_statistics *
__schedstats_from_se(கட்டமைப்பு sched_entity *se)
{
#ifdef CONFIG_FAIR_GROUP_SCHED
	எனில் (!entity_is_task(se))
		திரும்பு &container_of(se, கட்டமைப்பு cfs_tg_state, se)->stats;
#endif
	திரும்பு &task_of(se)->stats;
}

#ifdef CONFIG_PSI
வெறுமை psi_task_change(கட்டமைப்பு task_struct *task, முழு clear, முழு set);
வெறுமை psi_task_switch(கட்டமைப்பு task_struct *prev, கட்டமைப்பு task_struct *next,
		     bool sleep);
#ifdef CONFIG_IRQ_TIME_ACCOUNTING
வெறுமை psi_account_irqtime(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *curr, கட்டமைப்பு task_struct *prev);
#இல்லையெனில் /* !CONFIG_IRQ_TIME_ACCOUNTING: */
நிலையான உள்வரி வெறுமை psi_account_irqtime(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *curr,
				       கட்டமைப்பு task_struct *prev) {}
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
நிலையான உள்வரி வெறுமை psi_enqueue(கட்டமைப்பு task_struct *p, முழு flags)
{
	முழு clear = 0, set = 0;

	எனில் (static_branch_likely(&psi_disabled))
		திரும்பு;

	/* Same runqueue, nothing changed for psi */
	எனில் (flags & ENQUEUE_RESTORE)
		திரும்பு;

	/* psi_sched_switch() will handle the flags */
	எனில் (task_on_cpu(task_rq(p), p))
		திரும்பு;

	எனில் (p->se.sched_delayed) {
		/* CPU migration of "sleeping" task */
		WARN_ON_ONCE(!(flags & ENQUEUE_MIGRATED));
		எனில் (p->in_memstall)
			set |= TSK_MEMSTALL;
		எனில் (p->in_iowait)
			set |= TSK_IOWAIT;
	} இல்லையெனில் எனில் (flags & ENQUEUE_MIGRATED) {
		/* CPU migration of runnable task */
		set = TSK_RUNNING;
		எனில் (p->in_memstall)
			set |= TSK_MEMSTALL | TSK_MEMSTALL_RUNNING;
	} இல்லையெனில் {
		/* Wakeup of new or sleeping task */
		எனில் (p->in_iowait)
			clear |= TSK_IOWAIT;
		set = TSK_RUNNING;
		எனில் (p->in_memstall)
			set |= TSK_MEMSTALL_RUNNING;
	}

	psi_task_change(p, clear, set);
}

நிலையான உள்வரி வெறுமை psi_dequeue(கட்டமைப்பு task_struct *p, முழு flags)
{
	எனில் (static_branch_likely(&psi_disabled))
		திரும்பு;

	/* Same runqueue, nothing changed for psi */
	எனில் (flags & DEQUEUE_SAVE)
		திரும்பு;

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
	எனில் ((flags & DEQUEUE_SLEEP) && (p->psi_flags & TSK_ONCPU))
		திரும்பு;

	/*
	 * When migrating a task to another CPU, clear all psi
	 * state. The enqueue callback above will work it out.
	 */
	psi_task_change(p, p->psi_flags, 0);
}

நிலையான உள்வரி வெறுமை psi_ttwu_dequeue(கட்டமைப்பு task_struct *p)
{
	எனில் (static_branch_likely(&psi_disabled))
		திரும்பு;
	/*
	 * Is the task being migrated during a wakeup? Make sure to
	 * deregister its sleep-persistent psi states from the old
	 * queue, and let psi_enqueue() know it has to requeue.
	 */
	எனில் (unlikely(p->psi_flags)) {
		கட்டமைப்பு rq_flags rf;
		கட்டமைப்பு rq *rq;

		rq = __task_rq_lock(p, &rf);
		psi_task_change(p, p->psi_flags, 0);
		__task_rq_unlock(rq, p, &rf);
	}
}

நிலையான உள்வரி வெறுமை psi_sched_switch(கட்டமைப்பு task_struct *prev,
				    கட்டமைப்பு task_struct *next,
				    bool sleep)
{
	எனில் (static_branch_likely(&psi_disabled))
		திரும்பு;

	psi_task_switch(prev, next, sleep);
}

#இல்லையெனில் /* !CONFIG_PSI: */
நிலையான உள்வரி வெறுமை psi_enqueue(கட்டமைப்பு task_struct *p, bool migrate) {}
நிலையான உள்வரி வெறுமை psi_dequeue(கட்டமைப்பு task_struct *p, bool migrate) {}
நிலையான உள்வரி வெறுமை psi_ttwu_dequeue(கட்டமைப்பு task_struct *p) {}
நிலையான உள்வரி வெறுமை psi_sched_switch(கட்டமைப்பு task_struct *prev,
				    கட்டமைப்பு task_struct *next,
				    bool sleep) {}
நிலையான உள்வரி வெறுமை psi_account_irqtime(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *curr,
				       கட்டமைப்பு task_struct *prev) {}
#endif /* !CONFIG_PSI */

#ifdef CONFIG_SCHED_INFO
/*
 * We are interested in knowing how long it was from the *first* time a
 * task was queued to the time that it finally hit a CPU, we call this routine
 * from dequeue_task() to account for possible rq->clock skew across CPUs. The
 * delta taken on each CPU would annul the skew.
 */
நிலையான உள்வரி வெறுமை sched_info_dequeue(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *t)
{
	unsigned long long delta = 0;

	எனில் (!t->sched_info.last_queued)
		திரும்பு;

	delta = rq_clock(rq) - t->sched_info.last_queued;
	t->sched_info.last_queued = 0;
	t->sched_info.run_delay += delta;
	எனில் (delta > t->sched_info.max_run_delay) {
		t->sched_info.max_run_delay = delta;
		ktime_get_real_ts64(&t->sched_info.max_run_delay_ts);
	}
	எனில் (delta && (!t->sched_info.min_run_delay || delta < t->sched_info.min_run_delay))
		t->sched_info.min_run_delay = delta;
	rq_sched_info_dequeue(rq, delta);
}

/*
 * Called when a task finally hits the CPU.  We can now calculate how
 * long it was waiting to run.  We also note when it began so that we
 * can keep stats on how long its time-slice is.
 */
நிலையான வெறுமை sched_info_arrive(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *t)
{
	unsigned long long now, delta = 0;

	எனில் (!t->sched_info.last_queued)
		திரும்பு;

	now = rq_clock(rq);
	delta = now - t->sched_info.last_queued;
	t->sched_info.last_queued = 0;
	t->sched_info.run_delay += delta;
	t->sched_info.last_arrival = now;
	t->sched_info.pcount++;
	எனில் (delta > t->sched_info.max_run_delay) {
		t->sched_info.max_run_delay = delta;
		ktime_get_real_ts64(&t->sched_info.max_run_delay_ts);
	}
	எனில் (delta && (!t->sched_info.min_run_delay || delta < t->sched_info.min_run_delay))
		t->sched_info.min_run_delay = delta;

	rq_sched_info_arrive(rq, delta);
}

/*
 * This function is only called from enqueue_task(), but also only updates
 * the timestamp if it is already not set.  It's assumed that
 * sched_info_dequeue() will clear that stamp when appropriate.
 */
நிலையான உள்வரி வெறுமை sched_info_enqueue(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *t)
{
	எனில் (!t->sched_info.last_queued)
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
நிலையான உள்வரி வெறுமை sched_info_depart(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *t)
{
	unsigned long long delta = rq_clock(rq) - t->sched_info.last_arrival;

	rq_sched_info_depart(rq, delta);

	எனில் (task_is_running(t))
		sched_info_enqueue(rq, t);
}

/*
 * Called when tasks are switched involuntarily due, typically, to expiring
 * their time slice.  (This may also be called when switching to or from
 * the idle task.)  We are only called when prev != next.
 */
நிலையான உள்வரி வெறுமை
sched_info_switch(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *prev, கட்டமைப்பு task_struct *next)
{
	/*
	 * prev now departs the CPU.  It's not interesting to record
	 * stats about how efficient we were at scheduling the idle
	 * process, however.
	 */
	எனில் (prev != rq->idle)
		sched_info_depart(rq, prev);

	எனில் (next != rq->idle)
		sched_info_arrive(rq, next);
}

#இல்லையெனில் /* !CONFIG_SCHED_INFO: */
# define sched_info_enqueue(rq, t)	செய் { } போது (0)
# define sched_info_dequeue(rq, t)	செய் { } போது (0)
# define sched_info_switch(rq, t, next)	செய் { } போது (0)
#endif /* !CONFIG_SCHED_INFO */

#endif /* _KERNEL_STATS_H */
