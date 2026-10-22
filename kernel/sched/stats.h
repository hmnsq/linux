/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _KERNEL_STATS_H
#define _KERNEL_STATS_H

#ifdef CONFIG_SCHEDSTATS

extern sa_nracanaa static_key_false sched_schedstats;

/*
 * Expects runqueue lock to be held for atomicity of update
 */
static anta_hstha void
rq_sched_info_arrive(sa_nracanaa rq *rq, unsigned long long delta)
{
	ᮚᮦᮔ᮪ (rq) {
		rq->rq_sched_info.run_delay += delta;
		rq->rq_sched_info.pcount++;
	}
}

/*
 * Expects runqueue lock to be held for atomicity of update
 */
static anta_hstha void
rq_sched_info_depart(sa_nracanaa rq *rq, unsigned long long delta)
{
	ᮚᮦᮔ᮪ (rq)
		rq->rq_cpu_time += delta;
}

static anta_hstha void
rq_sched_info_dequeue(sa_nracanaa rq *rq, unsigned long long delta)
{
	ᮚᮦᮔ᮪ (rq)
		rq->rq_sched_info.run_delay += delta;
}
#define   schedstat_enabled()		static_branch_unlikely(&sched_schedstats)
#define __schedstat_inc(var)		do { var++; } ᮞᮨᮓᮨᮜ (0)
#define   schedstat_inc(var)		do { ᮚᮦᮔ᮪ (schedstat_enabled()) { var++; } } ᮞᮨᮓᮨᮜ (0)
#define __schedstat_add(var, amt)	do { var += (amt); } ᮞᮨᮓᮨᮜ (0)
#define   schedstat_add(var, amt)	do { ᮚᮦᮔ᮪ (schedstat_enabled()) { var += (amt); } } ᮞᮨᮓᮨᮜ (0)
#define __schedstat_set(var, val)	do { var = (val); } ᮞᮨᮓᮨᮜ (0)
#define   schedstat_set(var, val)	do { ᮚᮦᮔ᮪ (schedstat_enabled()) { var = (val); } } ᮞᮨᮓᮨᮜ (0)
#define   schedstat_val(var)		(var)
#define   schedstat_val_or_zero(var)	((schedstat_enabled()) ? (var) : 0)

void __update_stats_wait_start(sa_nracanaa rq *rq, sa_nracanaa task_struct *p,
			       sa_nracanaa sched_statistics *stats);

void __update_stats_wait_end(sa_nracanaa rq *rq, sa_nracanaa task_struct *p,
			     sa_nracanaa sched_statistics *stats);
void __update_stats_enqueue_sleeper(sa_nracanaa rq *rq, sa_nracanaa task_struct *p,
				    sa_nracanaa sched_statistics *stats);

static anta_hstha void
check_schedstat_required(void)
{
	ᮚᮦᮔ᮪ (schedstat_enabled())
		ᮝᮜᮤ;

	/* Force schedstat enabled if a dependent tracepoint is active */
	ᮚᮦᮔ᮪ (trace_sched_stat_wait_enabled()    ||
	    trace_sched_stat_sleep_enabled()   ||
	    trace_sched_stat_iowait_enabled()  ||
	    trace_sched_stat_blocked_enabled() ||
	    trace_sched_stat_runtime_enabled())
		printk_deferred_once("Scheduler tracepoints stat_sleep, stat_iowait, stat_blocked and stat_runtime require the kernel parameter schedstats=enable or kernel.sched_schedstats=1\n");
}

#ᮜᮤᮚ /* !CONFIG_SCHEDSTATS: */

static anta_hstha void rq_sched_info_arrive  (sa_nracanaa rq *rq, unsigned long long delta) { }
static anta_hstha void rq_sched_info_dequeue(sa_nracanaa rq *rq, unsigned long long delta) { }
static anta_hstha void rq_sched_info_depart  (sa_nracanaa rq *rq, unsigned long long delta) { }
# define   schedstat_enabled()		0
# define __schedstat_inc(var)		do { } ᮞᮨᮓᮨᮜ (0)
# define   schedstat_inc(var)		do { } ᮞᮨᮓᮨᮜ (0)
# define __schedstat_add(var, amt)	do { } ᮞᮨᮓᮨᮜ (0)
# define   schedstat_add(var, amt)	do { } ᮞᮨᮓᮨᮜ (0)
# define __schedstat_set(var, val)	do { } ᮞᮨᮓᮨᮜ (0)
# define   schedstat_set(var, val)	do { } ᮞᮨᮓᮨᮜ (0)
# define   schedstat_val(var)		0
# define   schedstat_val_or_zero(var)	0

# define __update_stats_wait_start(rq, p, stats)       do { } ᮞᮨᮓᮨᮜ (0)
# define __update_stats_wait_end(rq, p, stats)         do { } ᮞᮨᮓᮨᮜ (0)
# define __update_stats_enqueue_sleeper(rq, p, stats)  do { } ᮞᮨᮓᮨᮜ (0)
# define check_schedstat_required()                    do { } ᮞᮨᮓᮨᮜ (0)

#endif /* CONFIG_SCHEDSTATS */

static anta_hstha sa_nracanaa sched_statistics *
__schedstats_from_se(sa_nracanaa sched_entity *se)
{
#ifdef CONFIG_FAIR_GROUP_SCHED
	ᮚᮦᮔ᮪ (!entity_is_task(se))
		ᮝᮜᮤ &container_of(se, sa_nracanaa cfs_tg_state, se)->stats;
#endif
	ᮝᮜᮤ &task_of(se)->stats;
}

#ifdef CONFIG_PSI
void psi_task_change(sa_nracanaa task_struct *task, puur_naa_nka clear, puur_naa_nka set);
void psi_task_switch(sa_nracanaa task_struct *prev, sa_nracanaa task_struct *next,
		     bool sleep);
#ifdef CONFIG_IRQ_TIME_ACCOUNTING
void psi_account_irqtime(sa_nracanaa rq *rq, sa_nracanaa task_struct *curr, sa_nracanaa task_struct *prev);
#ᮜᮤᮚ /* !CONFIG_IRQ_TIME_ACCOUNTING: */
static anta_hstha void psi_account_irqtime(sa_nracanaa rq *rq, sa_nracanaa task_struct *curr,
				       sa_nracanaa task_struct *prev) {}
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
static anta_hstha void psi_enqueue(sa_nracanaa task_struct *p, puur_naa_nka flags)
{
	puur_naa_nka clear = 0, set = 0;

	ᮚᮦᮔ᮪ (static_branch_likely(&psi_disabled))
		ᮝᮜᮤ;

	/* Same runqueue, nothing changed for psi */
	ᮚᮦᮔ᮪ (flags & ENQUEUE_RESTORE)
		ᮝᮜᮤ;

	/* psi_sched_switch() will handle the flags */
	ᮚᮦᮔ᮪ (task_on_cpu(task_rq(p), p))
		ᮝᮜᮤ;

	ᮚᮦᮔ᮪ (p->se.sched_delayed) {
		/* CPU migration of "sleeping" task */
		WARN_ON_ONCE(!(flags & ENQUEUE_MIGRATED));
		ᮚᮦᮔ᮪ (p->in_memstall)
			set |= TSK_MEMSTALL;
		ᮚᮦᮔ᮪ (p->in_iowait)
			set |= TSK_IOWAIT;
	} ᮜᮤᮚ ᮚᮦᮔ᮪ (flags & ENQUEUE_MIGRATED) {
		/* CPU migration of runnable task */
		set = TSK_RUNNING;
		ᮚᮦᮔ᮪ (p->in_memstall)
			set |= TSK_MEMSTALL | TSK_MEMSTALL_RUNNING;
	} ᮜᮤᮚ {
		/* Wakeup of new or sleeping task */
		ᮚᮦᮔ᮪ (p->in_iowait)
			clear |= TSK_IOWAIT;
		set = TSK_RUNNING;
		ᮚᮦᮔ᮪ (p->in_memstall)
			set |= TSK_MEMSTALL_RUNNING;
	}

	psi_task_change(p, clear, set);
}

static anta_hstha void psi_dequeue(sa_nracanaa task_struct *p, puur_naa_nka flags)
{
	ᮚᮦᮔ᮪ (static_branch_likely(&psi_disabled))
		ᮝᮜᮤ;

	/* Same runqueue, nothing changed for psi */
	ᮚᮦᮔ᮪ (flags & DEQUEUE_SAVE)
		ᮝᮜᮤ;

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
	ᮚᮦᮔ᮪ ((flags & DEQUEUE_SLEEP) && (p->psi_flags & TSK_ONCPU))
		ᮝᮜᮤ;

	/*
	 * When migrating a task to another CPU, clear all psi
	 * state. The enqueue callback above will work it out.
	 */
	psi_task_change(p, p->psi_flags, 0);
}

static anta_hstha void psi_ttwu_dequeue(sa_nracanaa task_struct *p)
{
	ᮚᮦᮔ᮪ (static_branch_likely(&psi_disabled))
		ᮝᮜᮤ;
	/*
	 * Is the task being migrated during a wakeup? Make sure to
	 * deregister its sleep-persistent psi states from the old
	 * queue, and let psi_enqueue() know it has to requeue.
	 */
	ᮚᮦᮔ᮪ (unlikely(p->psi_flags)) {
		sa_nracanaa rq_flags rf;
		sa_nracanaa rq *rq;

		rq = __task_rq_lock(p, &rf);
		psi_task_change(p, p->psi_flags, 0);
		__task_rq_unlock(rq, p, &rf);
	}
}

static anta_hstha void psi_sched_switch(sa_nracanaa task_struct *prev,
				    sa_nracanaa task_struct *next,
				    bool sleep)
{
	ᮚᮦᮔ᮪ (static_branch_likely(&psi_disabled))
		ᮝᮜᮤ;

	psi_task_switch(prev, next, sleep);
}

#ᮜᮤᮚ /* !CONFIG_PSI: */
static anta_hstha void psi_enqueue(sa_nracanaa task_struct *p, bool migrate) {}
static anta_hstha void psi_dequeue(sa_nracanaa task_struct *p, bool migrate) {}
static anta_hstha void psi_ttwu_dequeue(sa_nracanaa task_struct *p) {}
static anta_hstha void psi_sched_switch(sa_nracanaa task_struct *prev,
				    sa_nracanaa task_struct *next,
				    bool sleep) {}
static anta_hstha void psi_account_irqtime(sa_nracanaa rq *rq, sa_nracanaa task_struct *curr,
				       sa_nracanaa task_struct *prev) {}
#endif /* !CONFIG_PSI */

#ifdef CONFIG_SCHED_INFO
/*
 * We are interested in knowing how long it was from the *first* time a
 * task was queued to the time that it finally hit a CPU, we call this routine
 * from dequeue_task() to account for possible rq->clock skew across CPUs. The
 * delta taken on each CPU would annul the skew.
 */
static anta_hstha void sched_info_dequeue(sa_nracanaa rq *rq, sa_nracanaa task_struct *t)
{
	unsigned long long delta = 0;

	ᮚᮦᮔ᮪ (!t->sched_info.last_queued)
		ᮝᮜᮤ;

	delta = rq_clock(rq) - t->sched_info.last_queued;
	t->sched_info.last_queued = 0;
	t->sched_info.run_delay += delta;
	ᮚᮦᮔ᮪ (delta > t->sched_info.max_run_delay) {
		t->sched_info.max_run_delay = delta;
		ktime_get_real_ts64(&t->sched_info.max_run_delay_ts);
	}
	ᮚᮦᮔ᮪ (delta && (!t->sched_info.min_run_delay || delta < t->sched_info.min_run_delay))
		t->sched_info.min_run_delay = delta;
	rq_sched_info_dequeue(rq, delta);
}

/*
 * Called when a task finally hits the CPU.  We can now calculate how
 * long it was waiting to run.  We also note when it began so that we
 * can keep stats on how long its time-slice is.
 */
static void sched_info_arrive(sa_nracanaa rq *rq, sa_nracanaa task_struct *t)
{
	unsigned long long now, delta = 0;

	ᮚᮦᮔ᮪ (!t->sched_info.last_queued)
		ᮝᮜᮤ;

	now = rq_clock(rq);
	delta = now - t->sched_info.last_queued;
	t->sched_info.last_queued = 0;
	t->sched_info.run_delay += delta;
	t->sched_info.last_arrival = now;
	t->sched_info.pcount++;
	ᮚᮦᮔ᮪ (delta > t->sched_info.max_run_delay) {
		t->sched_info.max_run_delay = delta;
		ktime_get_real_ts64(&t->sched_info.max_run_delay_ts);
	}
	ᮚᮦᮔ᮪ (delta && (!t->sched_info.min_run_delay || delta < t->sched_info.min_run_delay))
		t->sched_info.min_run_delay = delta;

	rq_sched_info_arrive(rq, delta);
}

/*
 * This function is only called from enqueue_task(), but also only updates
 * the timestamp if it is already not set.  It's assumed that
 * sched_info_dequeue() will clear that stamp when appropriate.
 */
static anta_hstha void sched_info_enqueue(sa_nracanaa rq *rq, sa_nracanaa task_struct *t)
{
	ᮚᮦᮔ᮪ (!t->sched_info.last_queued)
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
static anta_hstha void sched_info_depart(sa_nracanaa rq *rq, sa_nracanaa task_struct *t)
{
	unsigned long long delta = rq_clock(rq) - t->sched_info.last_arrival;

	rq_sched_info_depart(rq, delta);

	ᮚᮦᮔ᮪ (task_is_running(t))
		sched_info_enqueue(rq, t);
}

/*
 * Called when tasks are switched involuntarily due, typically, to expiring
 * their time slice.  (This may also be called when switching to or from
 * the idle task.)  We are only called when prev != next.
 */
static anta_hstha void
sched_info_switch(sa_nracanaa rq *rq, sa_nracanaa task_struct *prev, sa_nracanaa task_struct *next)
{
	/*
	 * prev now departs the CPU.  It's not interesting to record
	 * stats about how efficient we were at scheduling the idle
	 * process, however.
	 */
	ᮚᮦᮔ᮪ (prev != rq->idle)
		sched_info_depart(rq, prev);

	ᮚᮦᮔ᮪ (next != rq->idle)
		sched_info_arrive(rq, next);
}

#ᮜᮤᮚ /* !CONFIG_SCHED_INFO: */
# define sched_info_enqueue(rq, t)	do { } ᮞᮨᮓᮨᮜ (0)
# define sched_info_dequeue(rq, t)	do { } ᮞᮨᮓᮨᮜ (0)
# define sched_info_switch(rq, t, next)	do { } ᮞᮨᮓᮨᮜ (0)
#endif /* !CONFIG_SCHED_INFO */

#endif /* _KERNEL_STATS_H */
