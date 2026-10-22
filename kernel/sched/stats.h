/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _KERNEL_STATS_H
#define _KERNEL_STATS_H

#ifdef CONFIG_SCHEDSTATS

বাহ্য সংরচনা static_চাবি_false sched_schedstats;

/*
 * Expects runqueue lock to be held for atomicity of update
 */
স্থির অন্তঃস্থ শূন্য
rq_sched_info_arrive(সংরচনা rq *rq, unsigned long long delta)
{
	যদি (rq) {
		rq->rq_sched_info.run_delay += delta;
		rq->rq_sched_info.pcount++;
	}
}

/*
 * Expects runqueue lock to be held for atomicity of update
 */
স্থির অন্তঃস্থ শূন্য
rq_sched_info_depart(সংরচনা rq *rq, unsigned long long delta)
{
	যদি (rq)
		rq->rq_cpu_time += delta;
}

স্থির অন্তঃস্থ শূন্য
rq_sched_info_dequeue(সংরচনা rq *rq, unsigned long long delta)
{
	যদি (rq)
		rq->rq_sched_info.run_delay += delta;
}
#define   schedstat_enabled()		static_branch_unlikely(&sched_schedstats)
#define __schedstat_inc(var)		কৰা { var++; } যেতিয়ালৈ (0)
#define   schedstat_inc(var)		কৰা { যদি (schedstat_enabled()) { var++; } } যেতিয়ালৈ (0)
#define __schedstat_add(var, amt)	কৰা { var += (amt); } যেতিয়ালৈ (0)
#define   schedstat_add(var, amt)	কৰা { যদি (schedstat_enabled()) { var += (amt); } } যেতিয়ালৈ (0)
#define __schedstat_set(var, val)	কৰা { var = (val); } যেতিয়ালৈ (0)
#define   schedstat_set(var, val)	কৰা { যদি (schedstat_enabled()) { var = (val); } } যেতিয়ালৈ (0)
#define   schedstat_val(var)		(var)
#define   schedstat_val_or_zero(var)	((schedstat_enabled()) ? (var) : 0)

শূন্য __update_stats_wait_start(সংরচনা rq *rq, সংরচনা task_struct *p,
			       সংরচনা sched_statistics *stats);

শূন্য __update_stats_wait_end(সংরচনা rq *rq, সংরচনা task_struct *p,
			     সংরচনা sched_statistics *stats);
শূন্য __update_stats_enqueue_sleeper(সংরচনা rq *rq, সংরচনা task_struct *p,
				    সংরচনা sched_statistics *stats);

স্থির অন্তঃস্থ শূন্য
check_schedstat_required(শূন্য)
{
	যদি (schedstat_enabled())
		ঘূৰাই;

	/* Force schedstat enabled if a dependent tracepoint is active */
	যদি (trace_sched_stat_wait_enabled()    ||
	    trace_sched_stat_sleep_enabled()   ||
	    trace_sched_stat_iowait_enabled()  ||
	    trace_sched_stat_blocked_enabled() ||
	    trace_sched_stat_runtime_enabled())
		printk_deferred_once("Scheduler tracepoints stat_sleep, stat_iowait, stat_blocked and stat_runtime require the kernel parameter schedstats=enable or kernel.sched_schedstats=1\n");
}

#অন্যথা /* !CONFIG_SCHEDSTATS: */

স্থির অন্তঃস্থ শূন্য rq_sched_info_arrive  (সংরচনা rq *rq, unsigned long long delta) { }
স্থির অন্তঃস্থ শূন্য rq_sched_info_dequeue(সংরচনা rq *rq, unsigned long long delta) { }
স্থির অন্তঃস্থ শূন্য rq_sched_info_depart  (সংরচনা rq *rq, unsigned long long delta) { }
# define   schedstat_enabled()		0
# define __schedstat_inc(var)		কৰা { } যেতিয়ালৈ (0)
# define   schedstat_inc(var)		কৰা { } যেতিয়ালৈ (0)
# define __schedstat_add(var, amt)	কৰা { } যেতিয়ালৈ (0)
# define   schedstat_add(var, amt)	কৰা { } যেতিয়ালৈ (0)
# define __schedstat_set(var, val)	কৰা { } যেতিয়ালৈ (0)
# define   schedstat_set(var, val)	কৰা { } যেতিয়ালৈ (0)
# define   schedstat_val(var)		0
# define   schedstat_val_or_zero(var)	0

# define __update_stats_wait_start(rq, p, stats)       কৰা { } যেতিয়ালৈ (0)
# define __update_stats_wait_end(rq, p, stats)         কৰা { } যেতিয়ালৈ (0)
# define __update_stats_enqueue_sleeper(rq, p, stats)  কৰা { } যেতিয়ালৈ (0)
# define check_schedstat_required()                    কৰা { } যেতিয়ালৈ (0)

#endif /* CONFIG_SCHEDSTATS */

স্থির অন্তঃস্থ সংরচনা sched_statistics *
__schedstats_from_se(সংরচনা sched_entity *se)
{
#ifdef CONFIG_FAIR_GROUP_SCHED
	যদি (!entity_is_task(se))
		ঘূৰাই &container_of(se, সংরচনা cfs_tg_state, se)->stats;
#endif
	ঘূৰাই &task_of(se)->stats;
}

#ifdef CONFIG_PSI
শূন্য psi_task_change(সংরচনা task_struct *task, পূর্ণাঙ্ক clear, পূর্ণাঙ্ক set);
শূন্য psi_task_switch(সংরচনা task_struct *prev, সংরচনা task_struct *next,
		     bool sleep);
#ifdef CONFIG_IRQ_TIME_ACCOUNTING
শূন্য psi_account_irqtime(সংরচনা rq *rq, সংরচনা task_struct *curr, সংরচনা task_struct *prev);
#অন্যথা /* !CONFIG_IRQ_TIME_ACCOUNTING: */
স্থির অন্তঃস্থ শূন্য psi_account_irqtime(সংরচনা rq *rq, সংরচনা task_struct *curr,
				       সংরচনা task_struct *prev) {}
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
স্থির অন্তঃস্থ শূন্য psi_enqueue(সংরচনা task_struct *p, পূর্ণাঙ্ক flags)
{
	পূর্ণাঙ্ক clear = 0, set = 0;

	যদি (static_branch_likely(&psi_disabled))
		ঘূৰাই;

	/* Same runqueue, nothing changed for psi */
	যদি (flags & ENQUEUE_RESTORE)
		ঘূৰাই;

	/* psi_sched_switch() will handle the flags */
	যদি (task_on_cpu(task_rq(p), p))
		ঘূৰাই;

	যদি (p->se.sched_delayed) {
		/* CPU migration of "sleeping" task */
		WARN_ON_ONCE(!(flags & ENQUEUE_MIGRATED));
		যদি (p->in_memstall)
			set |= TSK_MEMSTALL;
		যদি (p->in_iowait)
			set |= TSK_IOWAIT;
	} অন্যথা যদি (flags & ENQUEUE_MIGRATED) {
		/* CPU migration of runnable task */
		set = TSK_RUNNING;
		যদি (p->in_memstall)
			set |= TSK_MEMSTALL | TSK_MEMSTALL_RUNNING;
	} অন্যথা {
		/* Wakeup of new or sleeping task */
		যদি (p->in_iowait)
			clear |= TSK_IOWAIT;
		set = TSK_RUNNING;
		যদি (p->in_memstall)
			set |= TSK_MEMSTALL_RUNNING;
	}

	psi_task_change(p, clear, set);
}

স্থির অন্তঃস্থ শূন্য psi_dequeue(সংরচনা task_struct *p, পূর্ণাঙ্ক flags)
{
	যদি (static_branch_likely(&psi_disabled))
		ঘূৰাই;

	/* Same runqueue, nothing changed for psi */
	যদি (flags & DEQUEUE_SAVE)
		ঘূৰাই;

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
	যদি ((flags & DEQUEUE_SLEEP) && (p->psi_flags & TSK_ONCPU))
		ঘূৰাই;

	/*
	 * When migrating a task to another CPU, clear all psi
	 * state. The enqueue callback above will work it out.
	 */
	psi_task_change(p, p->psi_flags, 0);
}

স্থির অন্তঃস্থ শূন্য psi_ttwu_dequeue(সংরচনা task_struct *p)
{
	যদি (static_branch_likely(&psi_disabled))
		ঘূৰাই;
	/*
	 * Is the task being migrated during a wakeup? Make sure to
	 * deregister its sleep-persistent psi states from the old
	 * queue, and let psi_enqueue() know it has to requeue.
	 */
	যদি (unlikely(p->psi_flags)) {
		সংরচনা rq_flags rf;
		সংরচনা rq *rq;

		rq = __task_rq_lock(p, &rf);
		psi_task_change(p, p->psi_flags, 0);
		__task_rq_unlock(rq, p, &rf);
	}
}

স্থির অন্তঃস্থ শূন্য psi_sched_switch(সংরচনা task_struct *prev,
				    সংরচনা task_struct *next,
				    bool sleep)
{
	যদি (static_branch_likely(&psi_disabled))
		ঘূৰাই;

	psi_task_switch(prev, next, sleep);
}

#অন্যথা /* !CONFIG_PSI: */
স্থির অন্তঃস্থ শূন্য psi_enqueue(সংরচনা task_struct *p, bool migrate) {}
স্থির অন্তঃস্থ শূন্য psi_dequeue(সংরচনা task_struct *p, bool migrate) {}
স্থির অন্তঃস্থ শূন্য psi_ttwu_dequeue(সংরচনা task_struct *p) {}
স্থির অন্তঃস্থ শূন্য psi_sched_switch(সংরচনা task_struct *prev,
				    সংরচনা task_struct *next,
				    bool sleep) {}
স্থির অন্তঃস্থ শূন্য psi_account_irqtime(সংরচনা rq *rq, সংরচনা task_struct *curr,
				       সংরচনা task_struct *prev) {}
#endif /* !CONFIG_PSI */

#ifdef CONFIG_SCHED_INFO
/*
 * We are interested in knowing how long it was from the *first* time a
 * task was queued to the time that it finally hit a CPU, we call this routine
 * from dequeue_task() to account for possible rq->clock skew across CPUs. The
 * delta taken on each CPU would annul the skew.
 */
স্থির অন্তঃস্থ শূন্য sched_info_dequeue(সংরচনা rq *rq, সংরচনা task_struct *t)
{
	unsigned long long delta = 0;

	যদি (!t->sched_info.last_queued)
		ঘূৰাই;

	delta = rq_clock(rq) - t->sched_info.last_queued;
	t->sched_info.last_queued = 0;
	t->sched_info.run_delay += delta;
	যদি (delta > t->sched_info.max_run_delay) {
		t->sched_info.max_run_delay = delta;
		ktime_get_real_ts64(&t->sched_info.max_run_delay_ts);
	}
	যদি (delta && (!t->sched_info.min_run_delay || delta < t->sched_info.min_run_delay))
		t->sched_info.min_run_delay = delta;
	rq_sched_info_dequeue(rq, delta);
}

/*
 * Called when a task finally hits the CPU.  We can now calculate how
 * long it was waiting to run.  We also note when it began so that we
 * can keep stats on how long its time-slice is.
 */
স্থির শূন্য sched_info_arrive(সংরচনা rq *rq, সংরচনা task_struct *t)
{
	unsigned long long now, delta = 0;

	যদি (!t->sched_info.last_queued)
		ঘূৰাই;

	now = rq_clock(rq);
	delta = now - t->sched_info.last_queued;
	t->sched_info.last_queued = 0;
	t->sched_info.run_delay += delta;
	t->sched_info.last_arrival = now;
	t->sched_info.pcount++;
	যদি (delta > t->sched_info.max_run_delay) {
		t->sched_info.max_run_delay = delta;
		ktime_get_real_ts64(&t->sched_info.max_run_delay_ts);
	}
	যদি (delta && (!t->sched_info.min_run_delay || delta < t->sched_info.min_run_delay))
		t->sched_info.min_run_delay = delta;

	rq_sched_info_arrive(rq, delta);
}

/*
 * This function is only called from enqueue_task(), but also only updates
 * the timestamp if it is already not set.  It's assumed that
 * sched_info_dequeue() will clear that stamp when appropriate.
 */
স্থির অন্তঃস্থ শূন্য sched_info_enqueue(সংরচনা rq *rq, সংরচনা task_struct *t)
{
	যদি (!t->sched_info.last_queued)
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
স্থির অন্তঃস্থ শূন্য sched_info_depart(সংরচনা rq *rq, সংরচনা task_struct *t)
{
	unsigned long long delta = rq_clock(rq) - t->sched_info.last_arrival;

	rq_sched_info_depart(rq, delta);

	যদি (task_is_running(t))
		sched_info_enqueue(rq, t);
}

/*
 * Called when tasks are switched involuntarily due, typically, to expiring
 * their time slice.  (This may also be called when switching to or from
 * the idle task.)  We are only called when prev != next.
 */
স্থির অন্তঃস্থ শূন্য
sched_info_switch(সংরচনা rq *rq, সংরচনা task_struct *prev, সংরচনা task_struct *next)
{
	/*
	 * prev now departs the CPU.  It's not interesting to record
	 * stats about how efficient we were at scheduling the idle
	 * process, however.
	 */
	যদি (prev != rq->idle)
		sched_info_depart(rq, prev);

	যদি (next != rq->idle)
		sched_info_arrive(rq, next);
}

#অন্যথা /* !CONFIG_SCHED_INFO: */
# define sched_info_enqueue(rq, t)	কৰা { } যেতিয়ালৈ (0)
# define sched_info_dequeue(rq, t)	কৰা { } যেতিয়ালৈ (0)
# define sched_info_switch(rq, t, next)	কৰা { } যেতিয়ালৈ (0)
#endif /* !CONFIG_SCHED_INFO */

#endif /* _KERNEL_STATS_H */
