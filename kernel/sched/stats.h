/* SPDX-License-Identifier: GPL-2.0 */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _KERNEL_STATS_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _KERNEL_STATS_H

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHEDSTATS

ਬਾਹਰੀ ਬਣਤਰ static_ਕੁੰਜੀ_false sched_schedstats;

/*
 * Expects runqueue lock to be held for atomicity of update
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
rq_sched_info_arrive(ਬਣਤਰ rq *rq, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ delta)
{
	ਜੇ (rq) {
		rq->rq_sched_info.ਲੜੀਵਾਰ_delay += delta;
		rq->rq_sched_info.pcount++;
	}
}

/*
 * Expects runqueue lock to be held for atomicity of update
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
rq_sched_info_depart(ਬਣਤਰ rq *rq, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ delta)
{
	ਜੇ (rq)
		rq->rq_cpu_ਸਮਾਂ += delta;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
rq_sched_info_ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ(ਬਣਤਰ rq *rq, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ delta)
{
	ਜੇ (rq)
		rq->rq_sched_info.ਲੜੀਵਾਰ_delay += delta;
}
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ   schedstat_enabled()		static_branch_unlikely(&sched_schedstats)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __schedstat_inc(var)		ਕਰੋ { var++; } ਜਦੋਂ (0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ   schedstat_inc(var)		ਕਰੋ { ਜੇ (schedstat_enabled()) { var++; } } ਜਦੋਂ (0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __schedstat_add(var, amt)	ਕਰੋ { var += (amt); } ਜਦੋਂ (0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ   schedstat_ਜੋੜੋ(var, amt)	ਕਰੋ { ਜੇ (schedstat_enabled()) { var += (amt); } } ਜਦੋਂ (0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __schedstat_set(var, val)	ਕਰੋ { var = (val); } ਜਦੋਂ (0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ   schedstat_ਸੈੱਟ(var, val)	ਕਰੋ { ਜੇ (schedstat_enabled()) { var = (val); } } ਜਦੋਂ (0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ   schedstat_val(var)		(var)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ   schedstat_val_or_zero(var)	((schedstat_enabled()) ? (var) : 0)

ਖਾਲੀ __update_stats_wait_start(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p,
			       ਬਣਤਰ sched_statistics *stats);

ਖਾਲੀ __update_stats_wait_end(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p,
			     ਬਣਤਰ sched_statistics *stats);
ਖਾਲੀ __update_stats_enqueue_sleeper(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p,
				    ਬਣਤਰ sched_statistics *stats);

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
check_schedstat_required(ਖਾਲੀ)
{
	ਜੇ (schedstat_enabled())
		ਵਾਪਸ;

	/* Force schedstat enabled if a dependent tracepoint is active */
	ਜੇ (trace_sched_stat_wait_enabled()    ||
	    trace_sched_stat_sleep_enabled()   ||
	    trace_sched_stat_iowait_enabled()  ||
	    trace_sched_stat_blocked_enabled() ||
	    trace_sched_stat_runtime_enabled())
		printk_deferred_once("Scheduler tracepoints stat_sleep, stat_iowait, stat_blocked and stat_runtime require the kernel parameter schedstats=enable or kernel.sched_schedstats=1\n");
}

#ਨਹੀਂਤਾਂ /* !CONFIG_SCHEDSTATS: */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ rq_sched_info_arrive  (ਬਣਤਰ rq *rq, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ delta) { }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ rq_sched_info_ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ(ਬਣਤਰ rq *rq, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ delta) { }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ rq_sched_info_depart  (ਬਣਤਰ rq *rq, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ delta) { }
# define   schedstat_enabled()		0
# define __schedstat_inc(var)		ਕਰੋ { } ਜਦੋਂ (0)
# define   schedstat_inc(var)		ਕਰੋ { } ਜਦੋਂ (0)
# define __schedstat_add(var, amt)	ਕਰੋ { } ਜਦੋਂ (0)
# define   schedstat_ਜੋੜੋ(var, amt)	ਕਰੋ { } ਜਦੋਂ (0)
# define __schedstat_set(var, val)	ਕਰੋ { } ਜਦੋਂ (0)
# define   schedstat_ਸੈੱਟ(var, val)	ਕਰੋ { } ਜਦੋਂ (0)
# define   schedstat_val(var)		0
# define   schedstat_val_or_zero(var)	0

# define __update_stats_wait_start(rq, p, stats)       ਕਰੋ { } ਜਦੋਂ (0)
# define __update_stats_wait_end(rq, p, stats)         ਕਰੋ { } ਜਦੋਂ (0)
# define __update_stats_enqueue_sleeper(rq, p, stats)  ਕਰੋ { } ਜਦੋਂ (0)
# define check_schedstat_required()                    ਕਰੋ { } ਜਦੋਂ (0)

#ਜੇ_ਅੰਤ /* CONFIG_SCHEDSTATS */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ sched_statistics *
__schedstats_from_se(ਬਣਤਰ sched_entity *se)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_FAIR_ਸਮੂਹ_SCHED
	ਜੇ (!entity_is_task(se))
		ਵਾਪਸ &container_ਦਾ(se, ਬਣਤਰ cfs_tg_state, se)->stats;
#ਜੇ_ਅੰਤ
	ਵਾਪਸ &task_ਦਾ(se)->stats;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_PSI
ਖਾਲੀ psi_task_change(ਬਣਤਰ task_struct *task, ਪੂਰਨ clear, ਪੂਰਨ ਸੈੱਟ);
ਖਾਲੀ psi_task_switch(ਬਣਤਰ task_struct *prev, ਬਣਤਰ task_struct *ਅਗਲਾ,
		     bool sleep);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_IRQ_ਸਮਾਂ_ACCOUNTING
ਖਾਲੀ psi_account_irqtime(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *curr, ਬਣਤਰ task_struct *prev);
#ਨਹੀਂਤਾਂ /* !CONFIG_IRQ_TIME_ACCOUNTING: */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ psi_account_irqtime(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *curr,
				       ਬਣਤਰ task_struct *prev) {}
#ਜੇ_ਅੰਤ /* !CONFIG_IRQ_TIME_ACCOUNTING */
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
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ psi_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ(ਬਣਤਰ task_struct *p, ਪੂਰਨ flags)
{
	ਪੂਰਨ clear = 0, ਸੈੱਟ = 0;

	ਜੇ (static_branch_likely(&psi_disabled))
		ਵਾਪਸ;

	/* Same runqueue, nothing changed for psi */
	ਜੇ (flags & ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_RESTORE)
		ਵਾਪਸ;

	/* psi_sched_switch() will handle the flags */
	ਜੇ (task_on_cpu(task_rq(p), p))
		ਵਾਪਸ;

	ਜੇ (p->se.sched_delayed) {
		/* CPU migration of "sleeping" task */
		WARN_ON_ONCE(!(flags & ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_MIGRATED));
		ਜੇ (p->ਵਿੱਚ_memstall)
			ਸੈੱਟ |= TSK_MEMSTALL;
		ਜੇ (p->ਵਿੱਚ_iowait)
			ਸੈੱਟ |= TSK_IOWAIT;
	} ਨਹੀਂਤਾਂ ਜੇ (flags & ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_MIGRATED) {
		/* CPU migration of runnable task */
		ਸੈੱਟ = TSK_RUNNING;
		ਜੇ (p->ਵਿੱਚ_memstall)
			ਸੈੱਟ |= TSK_MEMSTALL | TSK_MEMSTALL_RUNNING;
	} ਨਹੀਂਤਾਂ {
		/* Wakeup of new or sleeping task */
		ਜੇ (p->ਵਿੱਚ_iowait)
			clear |= TSK_IOWAIT;
		ਸੈੱਟ = TSK_RUNNING;
		ਜੇ (p->ਵਿੱਚ_memstall)
			ਸੈੱਟ |= TSK_MEMSTALL_RUNNING;
	}

	psi_task_change(p, clear, ਸੈੱਟ);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ psi_ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ(ਬਣਤਰ task_struct *p, ਪੂਰਨ flags)
{
	ਜੇ (static_branch_likely(&psi_disabled))
		ਵਾਪਸ;

	/* Same runqueue, nothing changed for psi */
	ਜੇ (flags & ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_SAVE)
		ਵਾਪਸ;

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
	ਜੇ ((flags & ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_SLEEP) && (p->psi_flags & TSK_ONCPU))
		ਵਾਪਸ;

	/*
	 * When migrating a task to another CPU, clear all psi
	 * state. The enqueue callback above will work it out.
	 */
	psi_task_change(p, p->psi_flags, 0);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ psi_ttwu_ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ(ਬਣਤਰ task_struct *p)
{
	ਜੇ (static_branch_likely(&psi_disabled))
		ਵਾਪਸ;
	/*
	 * Is the task being migrated during a wakeup? Make sure to
	 * deregister its sleep-persistent psi states from the old
	 * queue, and let psi_enqueue() know it has to requeue.
	 */
	ਜੇ (unlikely(p->psi_flags)) {
		ਬਣਤਰ rq_flags rf;
		ਬਣਤਰ rq *rq;

		rq = __task_rq_lock(p, &rf);
		psi_task_change(p, p->psi_flags, 0);
		__task_rq_unlock(rq, p, &rf);
	}
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ psi_sched_switch(ਬਣਤਰ task_struct *prev,
				    ਬਣਤਰ task_struct *ਅਗਲਾ,
				    bool sleep)
{
	ਜੇ (static_branch_likely(&psi_disabled))
		ਵਾਪਸ;

	psi_task_switch(prev, ਅਗਲਾ, sleep);
}

#ਨਹੀਂਤਾਂ /* !CONFIG_PSI: */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ psi_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ(ਬਣਤਰ task_struct *p, bool migrate) {}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ psi_ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ(ਬਣਤਰ task_struct *p, bool migrate) {}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ psi_ttwu_ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ(ਬਣਤਰ task_struct *p) {}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ psi_sched_switch(ਬਣਤਰ task_struct *prev,
				    ਬਣਤਰ task_struct *ਅਗਲਾ,
				    bool sleep) {}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ psi_account_irqtime(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *curr,
				       ਬਣਤਰ task_struct *prev) {}
#ਜੇ_ਅੰਤ /* !CONFIG_PSI */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_INFO
/*
 * We are interested in knowing how long it was from the *first* time a
 * task was queued to the time that it finally hit a CPU, we call this routine
 * from dequeue_task() to account for possible rq->clock skew across CPUs. The
 * delta taken on each CPU would annul the skew.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ sched_info_ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *t)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ delta = 0;

	ਜੇ (!t->sched_info.ਆਖਰੀ_queued)
		ਵਾਪਸ;

	delta = rq_clock(rq) - t->sched_info.ਆਖਰੀ_queued;
	t->sched_info.ਆਖਰੀ_queued = 0;
	t->sched_info.ਲੜੀਵਾਰ_delay += delta;
	ਜੇ (delta > t->sched_info.max_ਲੜੀਵਾਰ_delay) {
		t->sched_info.max_ਲੜੀਵਾਰ_delay = delta;
		ktime_get_real_ts64(&t->sched_info.max_ਲੜੀਵਾਰ_delay_ts);
	}
	ਜੇ (delta && (!t->sched_info.min_ਲੜੀਵਾਰ_delay || delta < t->sched_info.min_ਲੜੀਵਾਰ_delay))
		t->sched_info.min_ਲੜੀਵਾਰ_delay = delta;
	rq_sched_info_ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ(rq, delta);
}

/*
 * Called when a task finally hits the CPU.  We can now calculate how
 * long it was waiting to run.  We also note when it began so that we
 * can keep stats on how long its time-slice is.
 */
ਸਥਿਰ ਖਾਲੀ sched_info_arrive(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *t)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ now, delta = 0;

	ਜੇ (!t->sched_info.ਆਖਰੀ_queued)
		ਵਾਪਸ;

	now = rq_clock(rq);
	delta = now - t->sched_info.ਆਖਰੀ_queued;
	t->sched_info.ਆਖਰੀ_queued = 0;
	t->sched_info.ਲੜੀਵਾਰ_delay += delta;
	t->sched_info.ਆਖਰੀ_ਆਮਦ = now;
	t->sched_info.pcount++;
	ਜੇ (delta > t->sched_info.max_ਲੜੀਵਾਰ_delay) {
		t->sched_info.max_ਲੜੀਵਾਰ_delay = delta;
		ktime_get_real_ts64(&t->sched_info.max_ਲੜੀਵਾਰ_delay_ts);
	}
	ਜੇ (delta && (!t->sched_info.min_ਲੜੀਵਾਰ_delay || delta < t->sched_info.min_ਲੜੀਵਾਰ_delay))
		t->sched_info.min_ਲੜੀਵਾਰ_delay = delta;

	rq_sched_info_arrive(rq, delta);
}

/*
 * This function is only called from enqueue_task(), but also only updates
 * the timestamp if it is already not set.  It's assumed that
 * sched_info_dequeue() will clear that stamp when appropriate.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ sched_info_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *t)
{
	ਜੇ (!t->sched_info.ਆਖਰੀ_queued)
		t->sched_info.ਆਖਰੀ_queued = rq_clock(rq);
}

/*
 * Called when a process ceases being the active-running process involuntarily
 * due, typically, to expiring its time slice (this may also be called when
 * switching to the idle task).  Now we can calculate how long we ran.
 * Also, if the process is still in the TASK_RUNNING state, call
 * sched_info_enqueue() to mark that it has now again started waiting on
 * the runqueue.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ sched_info_depart(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *t)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ delta = rq_clock(rq) - t->sched_info.ਆਖਰੀ_ਆਮਦ;

	rq_sched_info_depart(rq, delta);

	ਜੇ (task_is_running(t))
		sched_info_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ(rq, t);
}

/*
 * Called when tasks are switched involuntarily due, typically, to expiring
 * their time slice.  (This may also be called when switching to or from
 * the idle task.)  We are only called when prev != next.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
sched_info_switch(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *prev, ਬਣਤਰ task_struct *ਅਗਲਾ)
{
	/*
	 * prev now departs the CPU.  It's not interesting to record
	 * stats about how efficient we were at scheduling the idle
	 * process, however.
	 */
	ਜੇ (prev != rq->idle)
		sched_info_depart(rq, prev);

	ਜੇ (ਅਗਲਾ != rq->idle)
		sched_info_arrive(rq, ਅਗਲਾ);
}

#ਨਹੀਂਤਾਂ /* !CONFIG_SCHED_INFO: */
# define sched_info_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ(rq, t)	ਕਰੋ { } ਜਦੋਂ (0)
# define sched_info_ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ(rq, t)	ਕਰੋ { } ਜਦੋਂ (0)
# define sched_info_switch(rq, t, ਅਗਲਾ)	ਕਰੋ { } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ /* !CONFIG_SCHED_INFO */

#ਜੇ_ਅੰਤ /* _KERNEL_STATS_H */
