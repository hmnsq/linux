/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _KERNEL_STATS_H
#define _KERNEL_STATS_H

#ifdef CONFIG_SCHEDSTATS

ബാഹ്യ ഘടന static_താക്കോൽ_false sched_schedstats;

/*
 * Expects runqueue lock to be held for atomicity of update
 */
സ്ഥിതം വരിയിൽ ശൂന്യം
rq_sched_info_arrive(ഘടന rq *rq, unsigned long long delta)
{
	എങ്കിൽ (rq) {
		rq->rq_sched_info.run_delay += delta;
		rq->rq_sched_info.pcount++;
	}
}

/*
 * Expects runqueue lock to be held for atomicity of update
 */
സ്ഥിതം വരിയിൽ ശൂന്യം
rq_sched_info_depart(ഘടന rq *rq, unsigned long long delta)
{
	എങ്കിൽ (rq)
		rq->rq_cpu_time += delta;
}

സ്ഥിതം വരിയിൽ ശൂന്യം
rq_sched_info_dequeue(ഘടന rq *rq, unsigned long long delta)
{
	എങ്കിൽ (rq)
		rq->rq_sched_info.run_delay += delta;
}
#define   schedstat_enabled()		static_branch_unlikely(&sched_schedstats)
#define __schedstat_inc(var)		ചെയ്യുക { var++; } ആകുമ്പോൾ (0)
#define   schedstat_inc(var)		ചെയ്യുക { എങ്കിൽ (schedstat_enabled()) { var++; } } ആകുമ്പോൾ (0)
#define __schedstat_add(var, amt)	ചെയ്യുക { var += (amt); } ആകുമ്പോൾ (0)
#define   schedstat_add(var, amt)	ചെയ്യുക { എങ്കിൽ (schedstat_enabled()) { var += (amt); } } ആകുമ്പോൾ (0)
#define __schedstat_set(var, val)	ചെയ്യുക { var = (val); } ആകുമ്പോൾ (0)
#define   schedstat_set(var, val)	ചെയ്യുക { എങ്കിൽ (schedstat_enabled()) { var = (val); } } ആകുമ്പോൾ (0)
#define   schedstat_val(var)		(var)
#define   schedstat_val_or_zero(var)	((schedstat_enabled()) ? (var) : 0)

ശൂന്യം __update_stats_wait_start(ഘടന rq *rq, ഘടന task_struct *p,
			       ഘടന sched_statistics *stats);

ശൂന്യം __update_stats_wait_end(ഘടന rq *rq, ഘടന task_struct *p,
			     ഘടന sched_statistics *stats);
ശൂന്യം __update_stats_enqueue_sleeper(ഘടന rq *rq, ഘടന task_struct *p,
				    ഘടന sched_statistics *stats);

സ്ഥിതം വരിയിൽ ശൂന്യം
check_schedstat_required(ശൂന്യം)
{
	എങ്കിൽ (schedstat_enabled())
		തിരികെ;

	/* Force schedstat enabled if a dependent tracepoint is active */
	എങ്കിൽ (trace_sched_stat_wait_enabled()    ||
	    trace_sched_stat_sleep_enabled()   ||
	    trace_sched_stat_iowait_enabled()  ||
	    trace_sched_stat_blocked_enabled() ||
	    trace_sched_stat_runtime_enabled())
		printk_deferred_once("Scheduler tracepoints stat_sleep, stat_iowait, stat_blocked and stat_runtime require the kernel parameter schedstats=enable or kernel.sched_schedstats=1\n");
}

#അല്ലെങ്കിൽ /* !CONFIG_SCHEDSTATS: */

സ്ഥിതം വരിയിൽ ശൂന്യം rq_sched_info_arrive  (ഘടന rq *rq, unsigned long long delta) { }
സ്ഥിതം വരിയിൽ ശൂന്യം rq_sched_info_dequeue(ഘടന rq *rq, unsigned long long delta) { }
സ്ഥിതം വരിയിൽ ശൂന്യം rq_sched_info_depart  (ഘടന rq *rq, unsigned long long delta) { }
# define   schedstat_enabled()		0
# define __schedstat_inc(var)		ചെയ്യുക { } ആകുമ്പോൾ (0)
# define   schedstat_inc(var)		ചെയ്യുക { } ആകുമ്പോൾ (0)
# define __schedstat_add(var, amt)	ചെയ്യുക { } ആകുമ്പോൾ (0)
# define   schedstat_add(var, amt)	ചെയ്യുക { } ആകുമ്പോൾ (0)
# define __schedstat_set(var, val)	ചെയ്യുക { } ആകുമ്പോൾ (0)
# define   schedstat_set(var, val)	ചെയ്യുക { } ആകുമ്പോൾ (0)
# define   schedstat_val(var)		0
# define   schedstat_val_or_zero(var)	0

# define __update_stats_wait_start(rq, p, stats)       ചെയ്യുക { } ആകുമ്പോൾ (0)
# define __update_stats_wait_end(rq, p, stats)         ചെയ്യുക { } ആകുമ്പോൾ (0)
# define __update_stats_enqueue_sleeper(rq, p, stats)  ചെയ്യുക { } ആകുമ്പോൾ (0)
# define check_schedstat_required()                    ചെയ്യുക { } ആകുമ്പോൾ (0)

#endif /* CONFIG_SCHEDSTATS */

സ്ഥിതം വരിയിൽ ഘടന sched_statistics *
__schedstats_from_se(ഘടന sched_entity *se)
{
#ifdef CONFIG_FAIR_GROUP_SCHED
	എങ്കിൽ (!entity_is_task(se))
		തിരികെ &container_of(se, ഘടന cfs_tg_state, se)->stats;
#endif
	തിരികെ &task_of(se)->stats;
}

#ifdef CONFIG_PSI
ശൂന്യം psi_task_change(ഘടന task_struct *task, പൂർണ്ണസംഖ്യ clear, പൂർണ്ണസംഖ്യ set);
ശൂന്യം psi_task_switch(ഘടന task_struct *prev, ഘടന task_struct *next,
		     bool sleep);
#ifdef CONFIG_IRQ_TIME_ACCOUNTING
ശൂന്യം psi_account_irqtime(ഘടന rq *rq, ഘടന task_struct *curr, ഘടന task_struct *prev);
#അല്ലെങ്കിൽ /* !CONFIG_IRQ_TIME_ACCOUNTING: */
സ്ഥിതം വരിയിൽ ശൂന്യം psi_account_irqtime(ഘടന rq *rq, ഘടന task_struct *curr,
				       ഘടന task_struct *prev) {}
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
സ്ഥിതം വരിയിൽ ശൂന്യം psi_enqueue(ഘടന task_struct *p, പൂർണ്ണസംഖ്യ flags)
{
	പൂർണ്ണസംഖ്യ clear = 0, set = 0;

	എങ്കിൽ (static_branch_likely(&psi_disabled))
		തിരികെ;

	/* Same runqueue, nothing changed for psi */
	എങ്കിൽ (flags & ENQUEUE_RESTORE)
		തിരികെ;

	/* psi_sched_switch() will handle the flags */
	എങ്കിൽ (task_on_cpu(task_rq(p), p))
		തിരികെ;

	എങ്കിൽ (p->se.sched_delayed) {
		/* CPU migration of "sleeping" task */
		WARN_ON_ONCE(!(flags & ENQUEUE_MIGRATED));
		എങ്കിൽ (p->in_memstall)
			set |= TSK_MEMSTALL;
		എങ്കിൽ (p->in_iowait)
			set |= TSK_IOWAIT;
	} അല്ലെങ്കിൽ എങ്കിൽ (flags & ENQUEUE_MIGRATED) {
		/* CPU migration of runnable task */
		set = TSK_RUNNING;
		എങ്കിൽ (p->in_memstall)
			set |= TSK_MEMSTALL | TSK_MEMSTALL_RUNNING;
	} അല്ലെങ്കിൽ {
		/* Wakeup of new or sleeping task */
		എങ്കിൽ (p->in_iowait)
			clear |= TSK_IOWAIT;
		set = TSK_RUNNING;
		എങ്കിൽ (p->in_memstall)
			set |= TSK_MEMSTALL_RUNNING;
	}

	psi_task_change(p, clear, set);
}

സ്ഥിതം വരിയിൽ ശൂന്യം psi_dequeue(ഘടന task_struct *p, പൂർണ്ണസംഖ്യ flags)
{
	എങ്കിൽ (static_branch_likely(&psi_disabled))
		തിരികെ;

	/* Same runqueue, nothing changed for psi */
	എങ്കിൽ (flags & DEQUEUE_SAVE)
		തിരികെ;

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
	എങ്കിൽ ((flags & DEQUEUE_SLEEP) && (p->psi_flags & TSK_ONCPU))
		തിരികെ;

	/*
	 * When migrating a task to another CPU, clear all psi
	 * state. The enqueue callback above will work it out.
	 */
	psi_task_change(p, p->psi_flags, 0);
}

സ്ഥിതം വരിയിൽ ശൂന്യം psi_ttwu_dequeue(ഘടന task_struct *p)
{
	എങ്കിൽ (static_branch_likely(&psi_disabled))
		തിരികെ;
	/*
	 * Is the task being migrated during a wakeup? Make sure to
	 * deregister its sleep-persistent psi states from the old
	 * queue, and let psi_enqueue() know it has to requeue.
	 */
	എങ്കിൽ (unlikely(p->psi_flags)) {
		ഘടന rq_flags rf;
		ഘടന rq *rq;

		rq = __task_rq_lock(p, &rf);
		psi_task_change(p, p->psi_flags, 0);
		__task_rq_unlock(rq, p, &rf);
	}
}

സ്ഥിതം വരിയിൽ ശൂന്യം psi_sched_switch(ഘടന task_struct *prev,
				    ഘടന task_struct *next,
				    bool sleep)
{
	എങ്കിൽ (static_branch_likely(&psi_disabled))
		തിരികെ;

	psi_task_switch(prev, next, sleep);
}

#അല്ലെങ്കിൽ /* !CONFIG_PSI: */
സ്ഥിതം വരിയിൽ ശൂന്യം psi_enqueue(ഘടന task_struct *p, bool migrate) {}
സ്ഥിതം വരിയിൽ ശൂന്യം psi_dequeue(ഘടന task_struct *p, bool migrate) {}
സ്ഥിതം വരിയിൽ ശൂന്യം psi_ttwu_dequeue(ഘടന task_struct *p) {}
സ്ഥിതം വരിയിൽ ശൂന്യം psi_sched_switch(ഘടന task_struct *prev,
				    ഘടന task_struct *next,
				    bool sleep) {}
സ്ഥിതം വരിയിൽ ശൂന്യം psi_account_irqtime(ഘടന rq *rq, ഘടന task_struct *curr,
				       ഘടന task_struct *prev) {}
#endif /* !CONFIG_PSI */

#ifdef CONFIG_SCHED_INFO
/*
 * We are interested in knowing how long it was from the *first* time a
 * task was queued to the time that it finally hit a CPU, we call this routine
 * from dequeue_task() to account for possible rq->clock skew across CPUs. The
 * delta taken on each CPU would annul the skew.
 */
സ്ഥിതം വരിയിൽ ശൂന്യം sched_info_dequeue(ഘടന rq *rq, ഘടന task_struct *t)
{
	unsigned long long delta = 0;

	എങ്കിൽ (!t->sched_info.last_queued)
		തിരികെ;

	delta = rq_clock(rq) - t->sched_info.last_queued;
	t->sched_info.last_queued = 0;
	t->sched_info.run_delay += delta;
	എങ്കിൽ (delta > t->sched_info.max_run_delay) {
		t->sched_info.max_run_delay = delta;
		ktime_get_real_ts64(&t->sched_info.max_run_delay_ts);
	}
	എങ്കിൽ (delta && (!t->sched_info.min_run_delay || delta < t->sched_info.min_run_delay))
		t->sched_info.min_run_delay = delta;
	rq_sched_info_dequeue(rq, delta);
}

/*
 * Called when a task finally hits the CPU.  We can now calculate how
 * long it was waiting to run.  We also note when it began so that we
 * can keep stats on how long its time-slice is.
 */
സ്ഥിതം ശൂന്യം sched_info_arrive(ഘടന rq *rq, ഘടന task_struct *t)
{
	unsigned long long now, delta = 0;

	എങ്കിൽ (!t->sched_info.last_queued)
		തിരികെ;

	now = rq_clock(rq);
	delta = now - t->sched_info.last_queued;
	t->sched_info.last_queued = 0;
	t->sched_info.run_delay += delta;
	t->sched_info.last_arrival = now;
	t->sched_info.pcount++;
	എങ്കിൽ (delta > t->sched_info.max_run_delay) {
		t->sched_info.max_run_delay = delta;
		ktime_get_real_ts64(&t->sched_info.max_run_delay_ts);
	}
	എങ്കിൽ (delta && (!t->sched_info.min_run_delay || delta < t->sched_info.min_run_delay))
		t->sched_info.min_run_delay = delta;

	rq_sched_info_arrive(rq, delta);
}

/*
 * This function is only called from enqueue_task(), but also only updates
 * the timestamp if it is already not set.  It's assumed that
 * sched_info_dequeue() will clear that stamp when appropriate.
 */
സ്ഥിതം വരിയിൽ ശൂന്യം sched_info_enqueue(ഘടന rq *rq, ഘടന task_struct *t)
{
	എങ്കിൽ (!t->sched_info.last_queued)
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
സ്ഥിതം വരിയിൽ ശൂന്യം sched_info_depart(ഘടന rq *rq, ഘടന task_struct *t)
{
	unsigned long long delta = rq_clock(rq) - t->sched_info.last_arrival;

	rq_sched_info_depart(rq, delta);

	എങ്കിൽ (task_is_running(t))
		sched_info_enqueue(rq, t);
}

/*
 * Called when tasks are switched involuntarily due, typically, to expiring
 * their time slice.  (This may also be called when switching to or from
 * the idle task.)  We are only called when prev != next.
 */
സ്ഥിതം വരിയിൽ ശൂന്യം
sched_info_switch(ഘടന rq *rq, ഘടന task_struct *prev, ഘടന task_struct *next)
{
	/*
	 * prev now departs the CPU.  It's not interesting to record
	 * stats about how efficient we were at scheduling the idle
	 * process, however.
	 */
	എങ്കിൽ (prev != rq->idle)
		sched_info_depart(rq, prev);

	എങ്കിൽ (next != rq->idle)
		sched_info_arrive(rq, next);
}

#അല്ലെങ്കിൽ /* !CONFIG_SCHED_INFO: */
# define sched_info_enqueue(rq, t)	ചെയ്യുക { } ആകുമ്പോൾ (0)
# define sched_info_dequeue(rq, t)	ചെയ്യുക { } ആകുമ്പോൾ (0)
# define sched_info_switch(rq, t, next)	ചെയ്യുക { } ആകുമ്പോൾ (0)
#endif /* !CONFIG_SCHED_INFO */

#endif /* _KERNEL_STATS_H */
