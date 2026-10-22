/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _KERNEL_STATS_H
#define _KERNEL_STATS_H

#ifdef CONFIG_SCHEDSTATS

ବାହ୍ୟ ସଂରଚନା static_ଚାବି_false sched_schedstats;

/*
 * Expects runqueue lock to be held for atomicity of update
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
rq_sched_info_arrive(ସଂରଚନା rq *rq, unsigned long long delta)
{
	ଯଦି (rq) {
		rq->rq_sched_info.run_delay += delta;
		rq->rq_sched_info.pcount++;
	}
}

/*
 * Expects runqueue lock to be held for atomicity of update
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
rq_sched_info_depart(ସଂରଚନା rq *rq, unsigned long long delta)
{
	ଯଦି (rq)
		rq->rq_cpu_time += delta;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
rq_sched_info_dequeue(ସଂରଚନା rq *rq, unsigned long long delta)
{
	ଯଦି (rq)
		rq->rq_sched_info.run_delay += delta;
}
#define   schedstat_enabled()		static_branch_unlikely(&sched_schedstats)
#define __schedstat_inc(var)		କର { var++; } ଯେତେବେଳେ (0)
#define   schedstat_inc(var)		କର { ଯଦି (schedstat_enabled()) { var++; } } ଯେତେବେଳେ (0)
#define __schedstat_add(var, amt)	କର { var += (amt); } ଯେତେବେଳେ (0)
#define   schedstat_add(var, amt)	କର { ଯଦି (schedstat_enabled()) { var += (amt); } } ଯେତେବେଳେ (0)
#define __schedstat_set(var, val)	କର { var = (val); } ଯେତେବେଳେ (0)
#define   schedstat_set(var, val)	କର { ଯଦି (schedstat_enabled()) { var = (val); } } ଯେତେବେଳେ (0)
#define   schedstat_val(var)		(var)
#define   schedstat_val_or_zero(var)	((schedstat_enabled()) ? (var) : 0)

ଶୂନ୍ୟ __update_stats_wait_start(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p,
			       ସଂରଚନା sched_statistics *stats);

ଶୂନ୍ୟ __update_stats_wait_end(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p,
			     ସଂରଚନା sched_statistics *stats);
ଶୂନ୍ୟ __update_stats_enqueue_sleeper(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p,
				    ସଂରଚନା sched_statistics *stats);

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
check_schedstat_required(ଶୂନ୍ୟ)
{
	ଯଦି (schedstat_enabled())
		ଫେରନ୍ତୁ;

	/* Force schedstat enabled if a dependent tracepoint is active */
	ଯଦି (trace_sched_stat_wait_enabled()    ||
	    trace_sched_stat_sleep_enabled()   ||
	    trace_sched_stat_iowait_enabled()  ||
	    trace_sched_stat_blocked_enabled() ||
	    trace_sched_stat_runtime_enabled())
		printk_deferred_once("Scheduler tracepoints stat_sleep, stat_iowait, stat_blocked and stat_runtime require the kernel parameter schedstats=enable or kernel.sched_schedstats=1\n");
}

#ଅନ୍ୟଥା /* !CONFIG_SCHEDSTATS: */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ rq_sched_info_arrive  (ସଂରଚନା rq *rq, unsigned long long delta) { }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ rq_sched_info_dequeue(ସଂରଚନା rq *rq, unsigned long long delta) { }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ rq_sched_info_depart  (ସଂରଚନା rq *rq, unsigned long long delta) { }
# define   schedstat_enabled()		0
# define __schedstat_inc(var)		କର { } ଯେତେବେଳେ (0)
# define   schedstat_inc(var)		କର { } ଯେତେବେଳେ (0)
# define __schedstat_add(var, amt)	କର { } ଯେତେବେଳେ (0)
# define   schedstat_add(var, amt)	କର { } ଯେତେବେଳେ (0)
# define __schedstat_set(var, val)	କର { } ଯେତେବେଳେ (0)
# define   schedstat_set(var, val)	କର { } ଯେତେବେଳେ (0)
# define   schedstat_val(var)		0
# define   schedstat_val_or_zero(var)	0

# define __update_stats_wait_start(rq, p, stats)       କର { } ଯେତେବେଳେ (0)
# define __update_stats_wait_end(rq, p, stats)         କର { } ଯେତେବେଳେ (0)
# define __update_stats_enqueue_sleeper(rq, p, stats)  କର { } ଯେତେବେଳେ (0)
# define check_schedstat_required()                    କର { } ଯେତେବେଳେ (0)

#endif /* CONFIG_SCHEDSTATS */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା sched_statistics *
__schedstats_from_se(ସଂରଚନା sched_entity *se)
{
#ifdef CONFIG_FAIR_GROUP_SCHED
	ଯଦି (!entity_is_task(se))
		ଫେରନ୍ତୁ &container_of(se, ସଂରଚନା cfs_tg_state, se)->stats;
#endif
	ଫେରନ୍ତୁ &task_of(se)->stats;
}

#ifdef CONFIG_PSI
ଶୂନ୍ୟ psi_task_change(ସଂରଚନା task_struct *task, ପୂର୍ଣ୍ଣସଂଖ୍ୟା clear, ପୂର୍ଣ୍ଣସଂଖ୍ୟା set);
ଶୂନ୍ୟ psi_task_switch(ସଂରଚନା task_struct *prev, ସଂରଚନା task_struct *next,
		     bool sleep);
#ifdef CONFIG_IRQ_TIME_ACCOUNTING
ଶୂନ୍ୟ psi_account_irqtime(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *curr, ସଂରଚନା task_struct *prev);
#ଅନ୍ୟଥା /* !CONFIG_IRQ_TIME_ACCOUNTING: */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ psi_account_irqtime(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *curr,
				       ସଂରଚନା task_struct *prev) {}
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
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ psi_enqueue(ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା clear = 0, set = 0;

	ଯଦି (static_branch_likely(&psi_disabled))
		ଫେରନ୍ତୁ;

	/* Same runqueue, nothing changed for psi */
	ଯଦି (flags & ENQUEUE_RESTORE)
		ଫେରନ୍ତୁ;

	/* psi_sched_switch() will handle the flags */
	ଯଦି (task_on_cpu(task_rq(p), p))
		ଫେରନ୍ତୁ;

	ଯଦି (p->se.sched_delayed) {
		/* CPU migration of "sleeping" task */
		WARN_ON_ONCE(!(flags & ENQUEUE_MIGRATED));
		ଯଦି (p->in_memstall)
			set |= TSK_MEMSTALL;
		ଯଦି (p->in_iowait)
			set |= TSK_IOWAIT;
	} ଅନ୍ୟଥା ଯଦି (flags & ENQUEUE_MIGRATED) {
		/* CPU migration of runnable task */
		set = TSK_RUNNING;
		ଯଦି (p->in_memstall)
			set |= TSK_MEMSTALL | TSK_MEMSTALL_RUNNING;
	} ଅନ୍ୟଥା {
		/* Wakeup of new or sleeping task */
		ଯଦି (p->in_iowait)
			clear |= TSK_IOWAIT;
		set = TSK_RUNNING;
		ଯଦି (p->in_memstall)
			set |= TSK_MEMSTALL_RUNNING;
	}

	psi_task_change(p, clear, set);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ psi_dequeue(ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ଯଦି (static_branch_likely(&psi_disabled))
		ଫେରନ୍ତୁ;

	/* Same runqueue, nothing changed for psi */
	ଯଦି (flags & DEQUEUE_SAVE)
		ଫେରନ୍ତୁ;

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
	ଯଦି ((flags & DEQUEUE_SLEEP) && (p->psi_flags & TSK_ONCPU))
		ଫେରନ୍ତୁ;

	/*
	 * When migrating a task to another CPU, clear all psi
	 * state. The enqueue callback above will work it out.
	 */
	psi_task_change(p, p->psi_flags, 0);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ psi_ttwu_dequeue(ସଂରଚନା task_struct *p)
{
	ଯଦି (static_branch_likely(&psi_disabled))
		ଫେରନ୍ତୁ;
	/*
	 * Is the task being migrated during a wakeup? Make sure to
	 * deregister its sleep-persistent psi states from the old
	 * queue, and let psi_enqueue() know it has to requeue.
	 */
	ଯଦି (unlikely(p->psi_flags)) {
		ସଂରଚନା rq_flags rf;
		ସଂରଚନା rq *rq;

		rq = __task_rq_lock(p, &rf);
		psi_task_change(p, p->psi_flags, 0);
		__task_rq_unlock(rq, p, &rf);
	}
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ psi_sched_switch(ସଂରଚନା task_struct *prev,
				    ସଂରଚନା task_struct *next,
				    bool sleep)
{
	ଯଦି (static_branch_likely(&psi_disabled))
		ଫେରନ୍ତୁ;

	psi_task_switch(prev, next, sleep);
}

#ଅନ୍ୟଥା /* !CONFIG_PSI: */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ psi_enqueue(ସଂରଚନା task_struct *p, bool migrate) {}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ psi_dequeue(ସଂରଚନା task_struct *p, bool migrate) {}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ psi_ttwu_dequeue(ସଂରଚନା task_struct *p) {}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ psi_sched_switch(ସଂରଚନା task_struct *prev,
				    ସଂରଚନା task_struct *next,
				    bool sleep) {}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ psi_account_irqtime(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *curr,
				       ସଂରଚନା task_struct *prev) {}
#endif /* !CONFIG_PSI */

#ifdef CONFIG_SCHED_INFO
/*
 * We are interested in knowing how long it was from the *first* time a
 * task was queued to the time that it finally hit a CPU, we call this routine
 * from dequeue_task() to account for possible rq->clock skew across CPUs. The
 * delta taken on each CPU would annul the skew.
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ sched_info_dequeue(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *t)
{
	unsigned long long delta = 0;

	ଯଦି (!t->sched_info.last_queued)
		ଫେରନ୍ତୁ;

	delta = rq_clock(rq) - t->sched_info.last_queued;
	t->sched_info.last_queued = 0;
	t->sched_info.run_delay += delta;
	ଯଦି (delta > t->sched_info.max_run_delay) {
		t->sched_info.max_run_delay = delta;
		ktime_get_real_ts64(&t->sched_info.max_run_delay_ts);
	}
	ଯଦି (delta && (!t->sched_info.min_run_delay || delta < t->sched_info.min_run_delay))
		t->sched_info.min_run_delay = delta;
	rq_sched_info_dequeue(rq, delta);
}

/*
 * Called when a task finally hits the CPU.  We can now calculate how
 * long it was waiting to run.  We also note when it began so that we
 * can keep stats on how long its time-slice is.
 */
ସ୍ଥିର ଶୂନ୍ୟ sched_info_arrive(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *t)
{
	unsigned long long now, delta = 0;

	ଯଦି (!t->sched_info.last_queued)
		ଫେରନ୍ତୁ;

	now = rq_clock(rq);
	delta = now - t->sched_info.last_queued;
	t->sched_info.last_queued = 0;
	t->sched_info.run_delay += delta;
	t->sched_info.last_arrival = now;
	t->sched_info.pcount++;
	ଯଦି (delta > t->sched_info.max_run_delay) {
		t->sched_info.max_run_delay = delta;
		ktime_get_real_ts64(&t->sched_info.max_run_delay_ts);
	}
	ଯଦି (delta && (!t->sched_info.min_run_delay || delta < t->sched_info.min_run_delay))
		t->sched_info.min_run_delay = delta;

	rq_sched_info_arrive(rq, delta);
}

/*
 * This function is only called from enqueue_task(), but also only updates
 * the timestamp if it is already not set.  It's assumed that
 * sched_info_dequeue() will clear that stamp when appropriate.
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ sched_info_enqueue(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *t)
{
	ଯଦି (!t->sched_info.last_queued)
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
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ sched_info_depart(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *t)
{
	unsigned long long delta = rq_clock(rq) - t->sched_info.last_arrival;

	rq_sched_info_depart(rq, delta);

	ଯଦି (task_is_running(t))
		sched_info_enqueue(rq, t);
}

/*
 * Called when tasks are switched involuntarily due, typically, to expiring
 * their time slice.  (This may also be called when switching to or from
 * the idle task.)  We are only called when prev != next.
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
sched_info_switch(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *prev, ସଂରଚନା task_struct *next)
{
	/*
	 * prev now departs the CPU.  It's not interesting to record
	 * stats about how efficient we were at scheduling the idle
	 * process, however.
	 */
	ଯଦି (prev != rq->idle)
		sched_info_depart(rq, prev);

	ଯଦି (next != rq->idle)
		sched_info_arrive(rq, next);
}

#ଅନ୍ୟଥା /* !CONFIG_SCHED_INFO: */
# define sched_info_enqueue(rq, t)	କର { } ଯେତେବେଳେ (0)
# define sched_info_dequeue(rq, t)	କର { } ଯେତେବେଳେ (0)
# define sched_info_switch(rq, t, next)	କର { } ଯେତେବେଳେ (0)
#endif /* !CONFIG_SCHED_INFO */

#endif /* _KERNEL_STATS_H */
