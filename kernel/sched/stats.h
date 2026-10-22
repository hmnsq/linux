/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _KERNEL_STATS_H
#define _KERNEL_STATS_H

#ifdef CONFIG_SCHEDSTATS

बाह्य संरचना static_कुञ्चिका_false sched_schedstats;

/*
 * Expects runqueue lock to be held for atomicity of update
 */
स्थिर अन्तःस्थ शून्य
rq_sched_info_arrive(संरचना rq *rq, unsigned long long delta)
{
	जइ (rq) {
		rq->rq_sched_info.run_delay += delta;
		rq->rq_sched_info.pcount++;
	}
}

/*
 * Expects runqueue lock to be held for atomicity of update
 */
स्थिर अन्तःस्थ शून्य
rq_sched_info_depart(संरचना rq *rq, unsigned long long delta)
{
	जइ (rq)
		rq->rq_cpu_time += delta;
}

स्थिर अन्तःस्थ शून्य
rq_sched_info_dequeue(संरचना rq *rq, unsigned long long delta)
{
	जइ (rq)
		rq->rq_sched_info.run_delay += delta;
}
#define   schedstat_enabled()		static_branch_unlikely(&sched_schedstats)
#define __schedstat_inc(var)		करेहि { var++; } जाव (0)
#define   schedstat_inc(var)		करेहि { जइ (schedstat_enabled()) { var++; } } जाव (0)
#define __schedstat_add(var, amt)	करेहि { var += (amt); } जाव (0)
#define   schedstat_add(var, amt)	करेहि { जइ (schedstat_enabled()) { var += (amt); } } जाव (0)
#define __schedstat_set(var, val)	करेहि { var = (val); } जाव (0)
#define   schedstat_set(var, val)	करेहि { जइ (schedstat_enabled()) { var = (val); } } जाव (0)
#define   schedstat_val(var)		(var)
#define   schedstat_val_or_zero(var)	((schedstat_enabled()) ? (var) : 0)

शून्य __update_stats_wait_start(संरचना rq *rq, संरचना task_struct *p,
			       संरचना sched_statistics *stats);

शून्य __update_stats_wait_end(संरचना rq *rq, संरचना task_struct *p,
			     संरचना sched_statistics *stats);
शून्य __update_stats_enqueue_sleeper(संरचना rq *rq, संरचना task_struct *p,
				    संरचना sched_statistics *stats);

स्थिर अन्तःस्थ शून्य
check_schedstat_required(शून्य)
{
	जइ (schedstat_enabled())
		प्रत्यागम;

	/* Force schedstat enabled if a dependent tracepoint is active */
	जइ (trace_sched_stat_wait_enabled()    ||
	    trace_sched_stat_sleep_enabled()   ||
	    trace_sched_stat_iowait_enabled()  ||
	    trace_sched_stat_blocked_enabled() ||
	    trace_sched_stat_runtime_enabled())
		printk_deferred_once("Scheduler tracepoints stat_sleep, stat_iowait, stat_blocked and stat_runtime require the kernel parameter schedstats=enable or kernel.sched_schedstats=1\n");
}

#अण्णहा /* !CONFIG_SCHEDSTATS: */

स्थिर अन्तःस्थ शून्य rq_sched_info_arrive  (संरचना rq *rq, unsigned long long delta) { }
स्थिर अन्तःस्थ शून्य rq_sched_info_dequeue(संरचना rq *rq, unsigned long long delta) { }
स्थिर अन्तःस्थ शून्य rq_sched_info_depart  (संरचना rq *rq, unsigned long long delta) { }
# define   schedstat_enabled()		0
# define __schedstat_inc(var)		करेहि { } जाव (0)
# define   schedstat_inc(var)		करेहि { } जाव (0)
# define __schedstat_add(var, amt)	करेहि { } जाव (0)
# define   schedstat_add(var, amt)	करेहि { } जाव (0)
# define __schedstat_set(var, val)	करेहि { } जाव (0)
# define   schedstat_set(var, val)	करेहि { } जाव (0)
# define   schedstat_val(var)		0
# define   schedstat_val_or_zero(var)	0

# define __update_stats_wait_start(rq, p, stats)       करेहि { } जाव (0)
# define __update_stats_wait_end(rq, p, stats)         करेहि { } जाव (0)
# define __update_stats_enqueue_sleeper(rq, p, stats)  करेहि { } जाव (0)
# define check_schedstat_required()                    करेहि { } जाव (0)

#endif /* CONFIG_SCHEDSTATS */

स्थिर अन्तःस्थ संरचना sched_statistics *
__schedstats_from_se(संरचना sched_entity *se)
{
#ifdef CONFIG_FAIR_GROUP_SCHED
	जइ (!entity_is_task(se))
		प्रत्यागम &container_of(se, संरचना cfs_tg_state, se)->stats;
#endif
	प्रत्यागम &task_of(se)->stats;
}

#ifdef CONFIG_PSI
शून्य psi_task_change(संरचना task_struct *task, पूर्णाङ्क clear, पूर्णाङ्क set);
शून्य psi_task_switch(संरचना task_struct *prev, संरचना task_struct *next,
		     bool sleep);
#ifdef CONFIG_IRQ_TIME_ACCOUNTING
शून्य psi_account_irqtime(संरचना rq *rq, संरचना task_struct *curr, संरचना task_struct *prev);
#अण्णहा /* !CONFIG_IRQ_TIME_ACCOUNTING: */
स्थिर अन्तःस्थ शून्य psi_account_irqtime(संरचना rq *rq, संरचना task_struct *curr,
				       संरचना task_struct *prev) {}
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
स्थिर अन्तःस्थ शून्य psi_enqueue(संरचना task_struct *p, पूर्णाङ्क flags)
{
	पूर्णाङ्क clear = 0, set = 0;

	जइ (static_branch_likely(&psi_disabled))
		प्रत्यागम;

	/* Same runqueue, nothing changed for psi */
	जइ (flags & ENQUEUE_RESTORE)
		प्रत्यागम;

	/* psi_sched_switch() will handle the flags */
	जइ (task_on_cpu(task_rq(p), p))
		प्रत्यागम;

	जइ (p->se.sched_delayed) {
		/* CPU migration of "sleeping" task */
		WARN_ON_ONCE(!(flags & ENQUEUE_MIGRATED));
		जइ (p->in_memstall)
			set |= TSK_MEMSTALL;
		जइ (p->in_iowait)
			set |= TSK_IOWAIT;
	} अण्णहा जइ (flags & ENQUEUE_MIGRATED) {
		/* CPU migration of runnable task */
		set = TSK_RUNNING;
		जइ (p->in_memstall)
			set |= TSK_MEMSTALL | TSK_MEMSTALL_RUNNING;
	} अण्णहा {
		/* Wakeup of new or sleeping task */
		जइ (p->in_iowait)
			clear |= TSK_IOWAIT;
		set = TSK_RUNNING;
		जइ (p->in_memstall)
			set |= TSK_MEMSTALL_RUNNING;
	}

	psi_task_change(p, clear, set);
}

स्थिर अन्तःस्थ शून्य psi_dequeue(संरचना task_struct *p, पूर्णाङ्क flags)
{
	जइ (static_branch_likely(&psi_disabled))
		प्रत्यागम;

	/* Same runqueue, nothing changed for psi */
	जइ (flags & DEQUEUE_SAVE)
		प्रत्यागम;

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
	जइ ((flags & DEQUEUE_SLEEP) && (p->psi_flags & TSK_ONCPU))
		प्रत्यागम;

	/*
	 * When migrating a task to another CPU, clear all psi
	 * state. The enqueue callback above will work it out.
	 */
	psi_task_change(p, p->psi_flags, 0);
}

स्थिर अन्तःस्थ शून्य psi_ttwu_dequeue(संरचना task_struct *p)
{
	जइ (static_branch_likely(&psi_disabled))
		प्रत्यागम;
	/*
	 * Is the task being migrated during a wakeup? Make sure to
	 * deregister its sleep-persistent psi states from the old
	 * queue, and let psi_enqueue() know it has to requeue.
	 */
	जइ (unlikely(p->psi_flags)) {
		संरचना rq_flags rf;
		संरचना rq *rq;

		rq = __task_rq_lock(p, &rf);
		psi_task_change(p, p->psi_flags, 0);
		__task_rq_unlock(rq, p, &rf);
	}
}

स्थिर अन्तःस्थ शून्य psi_sched_switch(संरचना task_struct *prev,
				    संरचना task_struct *next,
				    bool sleep)
{
	जइ (static_branch_likely(&psi_disabled))
		प्रत्यागम;

	psi_task_switch(prev, next, sleep);
}

#अण्णहा /* !CONFIG_PSI: */
स्थिर अन्तःस्थ शून्य psi_enqueue(संरचना task_struct *p, bool migrate) {}
स्थिर अन्तःस्थ शून्य psi_dequeue(संरचना task_struct *p, bool migrate) {}
स्थिर अन्तःस्थ शून्य psi_ttwu_dequeue(संरचना task_struct *p) {}
स्थिर अन्तःस्थ शून्य psi_sched_switch(संरचना task_struct *prev,
				    संरचना task_struct *next,
				    bool sleep) {}
स्थिर अन्तःस्थ शून्य psi_account_irqtime(संरचना rq *rq, संरचना task_struct *curr,
				       संरचना task_struct *prev) {}
#endif /* !CONFIG_PSI */

#ifdef CONFIG_SCHED_INFO
/*
 * We are interested in knowing how long it was from the *first* time a
 * task was queued to the time that it finally hit a CPU, we call this routine
 * from dequeue_task() to account for possible rq->clock skew across CPUs. The
 * delta taken on each CPU would annul the skew.
 */
स्थिर अन्तःस्थ शून्य sched_info_dequeue(संरचना rq *rq, संरचना task_struct *t)
{
	unsigned long long delta = 0;

	जइ (!t->sched_info.last_queued)
		प्रत्यागम;

	delta = rq_clock(rq) - t->sched_info.last_queued;
	t->sched_info.last_queued = 0;
	t->sched_info.run_delay += delta;
	जइ (delta > t->sched_info.max_run_delay) {
		t->sched_info.max_run_delay = delta;
		ktime_get_real_ts64(&t->sched_info.max_run_delay_ts);
	}
	जइ (delta && (!t->sched_info.min_run_delay || delta < t->sched_info.min_run_delay))
		t->sched_info.min_run_delay = delta;
	rq_sched_info_dequeue(rq, delta);
}

/*
 * Called when a task finally hits the CPU.  We can now calculate how
 * long it was waiting to run.  We also note when it began so that we
 * can keep stats on how long its time-slice is.
 */
स्थिर शून्य sched_info_arrive(संरचना rq *rq, संरचना task_struct *t)
{
	unsigned long long now, delta = 0;

	जइ (!t->sched_info.last_queued)
		प्रत्यागम;

	now = rq_clock(rq);
	delta = now - t->sched_info.last_queued;
	t->sched_info.last_queued = 0;
	t->sched_info.run_delay += delta;
	t->sched_info.last_arrival = now;
	t->sched_info.pcount++;
	जइ (delta > t->sched_info.max_run_delay) {
		t->sched_info.max_run_delay = delta;
		ktime_get_real_ts64(&t->sched_info.max_run_delay_ts);
	}
	जइ (delta && (!t->sched_info.min_run_delay || delta < t->sched_info.min_run_delay))
		t->sched_info.min_run_delay = delta;

	rq_sched_info_arrive(rq, delta);
}

/*
 * This function is only called from enqueue_task(), but also only updates
 * the timestamp if it is already not set.  It's assumed that
 * sched_info_dequeue() will clear that stamp when appropriate.
 */
स्थिर अन्तःस्थ शून्य sched_info_enqueue(संरचना rq *rq, संरचना task_struct *t)
{
	जइ (!t->sched_info.last_queued)
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
स्थिर अन्तःस्थ शून्य sched_info_depart(संरचना rq *rq, संरचना task_struct *t)
{
	unsigned long long delta = rq_clock(rq) - t->sched_info.last_arrival;

	rq_sched_info_depart(rq, delta);

	जइ (task_is_running(t))
		sched_info_enqueue(rq, t);
}

/*
 * Called when tasks are switched involuntarily due, typically, to expiring
 * their time slice.  (This may also be called when switching to or from
 * the idle task.)  We are only called when prev != next.
 */
स्थिर अन्तःस्थ शून्य
sched_info_switch(संरचना rq *rq, संरचना task_struct *prev, संरचना task_struct *next)
{
	/*
	 * prev now departs the CPU.  It's not interesting to record
	 * stats about how efficient we were at scheduling the idle
	 * process, however.
	 */
	जइ (prev != rq->idle)
		sched_info_depart(rq, prev);

	जइ (next != rq->idle)
		sched_info_arrive(rq, next);
}

#अण्णहा /* !CONFIG_SCHED_INFO: */
# define sched_info_enqueue(rq, t)	करेहि { } जाव (0)
# define sched_info_dequeue(rq, t)	करेहि { } जाव (0)
# define sched_info_switch(rq, t, next)	करेहि { } जाव (0)
#endif /* !CONFIG_SCHED_INFO */

#endif /* _KERNEL_STATS_H */
