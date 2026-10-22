/* SPDX-License-Identifier: GPL-2.0 */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _KERNEL_STATS_H
#ವ್ಯಾಖ್ಯಾನಿಸು _KERNEL_STATS_H

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHEDSTATS

ಬಾಹ್ಯ ರಚನೆ static_ಕೀಲಿ_false sched_schedstats;

/*
 * Expects runqueue lock to be held for atomicity of update
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
rq_sched_info_arrive(ರಚನೆ rq *rq, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ delta)
{
	ಆದರೆ (rq) {
		rq->rq_sched_info.ಸತತ_delay += delta;
		rq->rq_sched_info.pcount++;
	}
}

/*
 * Expects runqueue lock to be held for atomicity of update
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
rq_sched_info_depart(ರಚನೆ rq *rq, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ delta)
{
	ಆದರೆ (rq)
		rq->rq_cpu_ಸಮಯ += delta;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
rq_sched_info_ಸರತಿಯಿಂದ_ತೆಗೆ(ರಚನೆ rq *rq, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ delta)
{
	ಆದರೆ (rq)
		rq->rq_sched_info.ಸತತ_delay += delta;
}
#ವ್ಯಾಖ್ಯಾನಿಸು   schedstat_enabled()		static_branch_unlikely(&sched_schedstats)
#ವ್ಯಾಖ್ಯಾನಿಸು __schedstat_inc(var)		ಮಾಡು { var++; } ಆಗ (0)
#ವ್ಯಾಖ್ಯಾನಿಸು   schedstat_inc(var)		ಮಾಡು { ಆದರೆ (schedstat_enabled()) { var++; } } ಆಗ (0)
#ವ್ಯಾಖ್ಯಾನಿಸು __schedstat_add(var, amt)	ಮಾಡು { var += (amt); } ಆಗ (0)
#ವ್ಯಾಖ್ಯಾನಿಸು   schedstat_ಕೂಡಿಸು(var, amt)	ಮಾಡು { ಆದರೆ (schedstat_enabled()) { var += (amt); } } ಆಗ (0)
#ವ್ಯಾಖ್ಯಾನಿಸು __schedstat_set(var, val)	ಮಾಡು { var = (val); } ಆಗ (0)
#ವ್ಯಾಖ್ಯಾನಿಸು   schedstat_ಗಣ(var, val)	ಮಾಡು { ಆದರೆ (schedstat_enabled()) { var = (val); } } ಆಗ (0)
#ವ್ಯಾಖ್ಯಾನಿಸು   schedstat_val(var)		(var)
#ವ್ಯಾಖ್ಯಾನಿಸು   schedstat_val_or_zero(var)	((schedstat_enabled()) ? (var) : 0)

ಶೂನ್ಯ __update_stats_wait_start(ರಚನೆ rq *rq, ರಚನೆ task_struct *p,
			       ರಚನೆ sched_statistics *stats);

ಶೂನ್ಯ __update_stats_wait_end(ರಚನೆ rq *rq, ರಚನೆ task_struct *p,
			     ರಚನೆ sched_statistics *stats);
ಶೂನ್ಯ __update_stats_enqueue_sleeper(ರಚನೆ rq *rq, ರಚನೆ task_struct *p,
				    ರಚನೆ sched_statistics *stats);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
check_schedstat_required(ಶೂನ್ಯ)
{
	ಆದರೆ (schedstat_enabled())
		ಹಿಂತಿರುಗಿ;

	/* Force schedstat enabled if a dependent tracepoint is active */
	ಆದರೆ (trace_sched_stat_wait_enabled()    ||
	    trace_sched_stat_sleep_enabled()   ||
	    trace_sched_stat_iowait_enabled()  ||
	    trace_sched_stat_blocked_enabled() ||
	    trace_sched_stat_runtime_enabled())
		printk_deferred_once("Scheduler tracepoints stat_sleep, stat_iowait, stat_blocked and stat_runtime require the kernel parameter schedstats=enable or kernel.sched_schedstats=1\n");
}

#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_SCHEDSTATS: */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ rq_sched_info_arrive  (ರಚನೆ rq *rq, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ delta) { }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ rq_sched_info_ಸರತಿಯಿಂದ_ತೆಗೆ(ರಚನೆ rq *rq, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ delta) { }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ rq_sched_info_depart  (ರಚನೆ rq *rq, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ delta) { }
# define   schedstat_enabled()		0
# define __schedstat_inc(var)		ಮಾಡು { } ಆಗ (0)
# define   schedstat_inc(var)		ಮಾಡು { } ಆಗ (0)
# define __schedstat_add(var, amt)	ಮಾಡು { } ಆಗ (0)
# define   schedstat_ಕೂಡಿಸು(var, amt)	ಮಾಡು { } ಆಗ (0)
# define __schedstat_set(var, val)	ಮಾಡು { } ಆಗ (0)
# define   schedstat_ಗಣ(var, val)	ಮಾಡು { } ಆಗ (0)
# define   schedstat_val(var)		0
# define   schedstat_val_or_zero(var)	0

# define __update_stats_wait_start(rq, p, stats)       ಮಾಡು { } ಆಗ (0)
# define __update_stats_wait_end(rq, p, stats)         ಮಾಡು { } ಆಗ (0)
# define __update_stats_enqueue_sleeper(rq, p, stats)  ಮಾಡು { } ಆಗ (0)
# define check_schedstat_required()                    ಮಾಡು { } ಆಗ (0)

#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_SCHEDSTATS */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ sched_statistics *
__schedstats_from_se(ರಚನೆ sched_entity *se)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_FAIR_ಗುಂಪು_SCHED
	ಆದರೆ (!entity_is_task(se))
		ಹಿಂತಿರುಗಿ &container_ದ(se, ರಚನೆ cfs_tg_state, se)->stats;
#ಆದರೆ_ಅಂತ್ಯ
	ಹಿಂತಿರುಗಿ &task_ದ(se)->stats;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_PSI
ಶೂನ್ಯ psi_task_change(ರಚನೆ task_struct *task, ಪೂರ್ಣಾಂಕ clear, ಪೂರ್ಣಾಂಕ ಗಣ);
ಶೂನ್ಯ psi_task_switch(ರಚನೆ task_struct *prev, ರಚನೆ task_struct *ಮುಂದಿನ,
		     bool sleep);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_IRQ_ಸಮಯ_ACCOUNTING
ಶೂನ್ಯ psi_account_irqtime(ರಚನೆ rq *rq, ರಚನೆ task_struct *curr, ರಚನೆ task_struct *prev);
#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_IRQ_TIME_ACCOUNTING: */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ psi_account_irqtime(ರಚನೆ rq *rq, ರಚನೆ task_struct *curr,
				       ರಚನೆ task_struct *prev) {}
#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_IRQ_TIME_ACCOUNTING */
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
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ psi_ಸರತಿಗೆ_ಸೇರಿಸು(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ flags)
{
	ಪೂರ್ಣಾಂಕ clear = 0, ಗಣ = 0;

	ಆದರೆ (static_branch_likely(&psi_disabled))
		ಹಿಂತಿರುಗಿ;

	/* Same runqueue, nothing changed for psi */
	ಆದರೆ (flags & ಸರತಿಗೆ_ಸೇರಿಸು_RESTORE)
		ಹಿಂತಿರುಗಿ;

	/* psi_sched_switch() will handle the flags */
	ಆದರೆ (task_on_cpu(task_rq(p), p))
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (p->se.sched_delayed) {
		/* CPU migration of "sleeping" task */
		WARN_ON_ONCE(!(flags & ಸರತಿಗೆ_ಸೇರಿಸು_MIGRATED));
		ಆದರೆ (p->ಒಳ_memstall)
			ಗಣ |= TSK_MEMSTALL;
		ಆದರೆ (p->ಒಳ_iowait)
			ಗಣ |= TSK_IOWAIT;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (flags & ಸರತಿಗೆ_ಸೇರಿಸು_MIGRATED) {
		/* CPU migration of runnable task */
		ಗಣ = TSK_RUNNING;
		ಆದರೆ (p->ಒಳ_memstall)
			ಗಣ |= TSK_MEMSTALL | TSK_MEMSTALL_RUNNING;
	} ಇಲ್ಲದಿದ್ದರೆ {
		/* Wakeup of new or sleeping task */
		ಆದರೆ (p->ಒಳ_iowait)
			clear |= TSK_IOWAIT;
		ಗಣ = TSK_RUNNING;
		ಆದರೆ (p->ಒಳ_memstall)
			ಗಣ |= TSK_MEMSTALL_RUNNING;
	}

	psi_task_change(p, clear, ಗಣ);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ psi_ಸರತಿಯಿಂದ_ತೆಗೆ(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ flags)
{
	ಆದರೆ (static_branch_likely(&psi_disabled))
		ಹಿಂತಿರುಗಿ;

	/* Same runqueue, nothing changed for psi */
	ಆದರೆ (flags & ಸರತಿಯಿಂದ_ತೆಗೆ_SAVE)
		ಹಿಂತಿರುಗಿ;

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
	ಆದರೆ ((flags & ಸರತಿಯಿಂದ_ತೆಗೆ_SLEEP) && (p->psi_flags & TSK_ONCPU))
		ಹಿಂತಿರುಗಿ;

	/*
	 * When migrating a task to another CPU, clear all psi
	 * state. The enqueue callback above will work it out.
	 */
	psi_task_change(p, p->psi_flags, 0);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ psi_ttwu_ಸರತಿಯಿಂದ_ತೆಗೆ(ರಚನೆ task_struct *p)
{
	ಆದರೆ (static_branch_likely(&psi_disabled))
		ಹಿಂತಿರುಗಿ;
	/*
	 * Is the task being migrated during a wakeup? Make sure to
	 * deregister its sleep-persistent psi states from the old
	 * queue, and let psi_enqueue() know it has to requeue.
	 */
	ಆದರೆ (unlikely(p->psi_flags)) {
		ರಚನೆ rq_flags rf;
		ರಚನೆ rq *rq;

		rq = __task_rq_lock(p, &rf);
		psi_task_change(p, p->psi_flags, 0);
		__task_rq_unlock(rq, p, &rf);
	}
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ psi_sched_switch(ರಚನೆ task_struct *prev,
				    ರಚನೆ task_struct *ಮುಂದಿನ,
				    bool sleep)
{
	ಆದರೆ (static_branch_likely(&psi_disabled))
		ಹಿಂತಿರುಗಿ;

	psi_task_switch(prev, ಮುಂದಿನ, sleep);
}

#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_PSI: */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ psi_ಸರತಿಗೆ_ಸೇರಿಸು(ರಚನೆ task_struct *p, bool migrate) {}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ psi_ಸರತಿಯಿಂದ_ತೆಗೆ(ರಚನೆ task_struct *p, bool migrate) {}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ psi_ttwu_ಸರತಿಯಿಂದ_ತೆಗೆ(ರಚನೆ task_struct *p) {}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ psi_sched_switch(ರಚನೆ task_struct *prev,
				    ರಚನೆ task_struct *ಮುಂದಿನ,
				    bool sleep) {}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ psi_account_irqtime(ರಚನೆ rq *rq, ರಚನೆ task_struct *curr,
				       ರಚನೆ task_struct *prev) {}
#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_PSI */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_INFO
/*
 * We are interested in knowing how long it was from the *first* time a
 * task was queued to the time that it finally hit a CPU, we call this routine
 * from dequeue_task() to account for possible rq->clock skew across CPUs. The
 * delta taken on each CPU would annul the skew.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ sched_info_ಸರತಿಯಿಂದ_ತೆಗೆ(ರಚನೆ rq *rq, ರಚನೆ task_struct *t)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ delta = 0;

	ಆದರೆ (!t->sched_info.ಕೊನೆಯ_queued)
		ಹಿಂತಿರುಗಿ;

	delta = rq_clock(rq) - t->sched_info.ಕೊನೆಯ_queued;
	t->sched_info.ಕೊನೆಯ_queued = 0;
	t->sched_info.ಸತತ_delay += delta;
	ಆದರೆ (delta > t->sched_info.max_ಸತತ_delay) {
		t->sched_info.max_ಸತತ_delay = delta;
		ktime_get_real_ts64(&t->sched_info.max_ಸತತ_delay_ts);
	}
	ಆದರೆ (delta && (!t->sched_info.min_ಸತತ_delay || delta < t->sched_info.min_ಸತತ_delay))
		t->sched_info.min_ಸತತ_delay = delta;
	rq_sched_info_ಸರತಿಯಿಂದ_ತೆಗೆ(rq, delta);
}

/*
 * Called when a task finally hits the CPU.  We can now calculate how
 * long it was waiting to run.  We also note when it began so that we
 * can keep stats on how long its time-slice is.
 */
ಸ್ಥಿರ ಶೂನ್ಯ sched_info_arrive(ರಚನೆ rq *rq, ರಚನೆ task_struct *t)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ now, delta = 0;

	ಆದರೆ (!t->sched_info.ಕೊನೆಯ_queued)
		ಹಿಂತಿರುಗಿ;

	now = rq_clock(rq);
	delta = now - t->sched_info.ಕೊನೆಯ_queued;
	t->sched_info.ಕೊನೆಯ_queued = 0;
	t->sched_info.ಸತತ_delay += delta;
	t->sched_info.ಕೊನೆಯ_ಆಗಮನ = now;
	t->sched_info.pcount++;
	ಆದರೆ (delta > t->sched_info.max_ಸತತ_delay) {
		t->sched_info.max_ಸತತ_delay = delta;
		ktime_get_real_ts64(&t->sched_info.max_ಸತತ_delay_ts);
	}
	ಆದರೆ (delta && (!t->sched_info.min_ಸತತ_delay || delta < t->sched_info.min_ಸತತ_delay))
		t->sched_info.min_ಸತತ_delay = delta;

	rq_sched_info_arrive(rq, delta);
}

/*
 * This function is only called from enqueue_task(), but also only updates
 * the timestamp if it is already not set.  It's assumed that
 * sched_info_dequeue() will clear that stamp when appropriate.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ sched_info_ಸರತಿಗೆ_ಸೇರಿಸು(ರಚನೆ rq *rq, ರಚನೆ task_struct *t)
{
	ಆದರೆ (!t->sched_info.ಕೊನೆಯ_queued)
		t->sched_info.ಕೊನೆಯ_queued = rq_clock(rq);
}

/*
 * Called when a process ceases being the active-running process involuntarily
 * due, typically, to expiring its time slice (this may also be called when
 * switching to the idle task).  Now we can calculate how long we ran.
 * Also, if the process is still in the TASK_RUNNING state, call
 * sched_info_enqueue() to mark that it has now again started waiting on
 * the runqueue.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ sched_info_depart(ರಚನೆ rq *rq, ರಚನೆ task_struct *t)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ delta = rq_clock(rq) - t->sched_info.ಕೊನೆಯ_ಆಗಮನ;

	rq_sched_info_depart(rq, delta);

	ಆದರೆ (task_is_running(t))
		sched_info_ಸರತಿಗೆ_ಸೇರಿಸು(rq, t);
}

/*
 * Called when tasks are switched involuntarily due, typically, to expiring
 * their time slice.  (This may also be called when switching to or from
 * the idle task.)  We are only called when prev != next.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
sched_info_switch(ರಚನೆ rq *rq, ರಚನೆ task_struct *prev, ರಚನೆ task_struct *ಮುಂದಿನ)
{
	/*
	 * prev now departs the CPU.  It's not interesting to record
	 * stats about how efficient we were at scheduling the idle
	 * process, however.
	 */
	ಆದರೆ (prev != rq->idle)
		sched_info_depart(rq, prev);

	ಆದರೆ (ಮುಂದಿನ != rq->idle)
		sched_info_arrive(rq, ಮುಂದಿನ);
}

#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_SCHED_INFO: */
# define sched_info_ಸರತಿಗೆ_ಸೇರಿಸು(rq, t)	ಮಾಡು { } ಆಗ (0)
# define sched_info_ಸರತಿಯಿಂದ_ತೆಗೆ(rq, t)	ಮಾಡು { } ಆಗ (0)
# define sched_info_switch(rq, t, ಮುಂದಿನ)	ಮಾಡು { } ಆಗ (0)
#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_SCHED_INFO */

#ಆದರೆ_ಅಂತ್ಯ /* _KERNEL_STATS_H */
