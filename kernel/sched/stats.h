/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _KERNEL_STATS_H
#define _KERNEL_STATS_H

#ifdef CONFIG_SCHEDSTATS

extern struct static_key_false sched_schedstats;

/*
 * Expects runqueue lock to be held for atomicity of update
 */
static inline void
rq_sched_info_arrive(struct rq *rq, unsigned long long delta)
{
	якщо (rq) {
		rq->rq_sched_info.run_delay += delta;
		rq->rq_sched_info.pcount++;
	}
}

/*
 * Expects runqueue lock to be held for atomicity of update
 */
static inline void
rq_sched_info_depart(struct rq *rq, unsigned long long delta)
{
	якщо (rq)
		rq->rq_cpu_time += delta;
}

static inline void
rq_sched_info_dequeue(struct rq *rq, unsigned long long delta)
{
	якщо (rq)
		rq->rq_sched_info.run_delay += delta;
}
#define   schedstat_enabled()		static_branch_unlikely(&sched_schedstats)
#define __schedstat_inc(var)		do { var++; } поки (0)
#define   schedstat_inc(var)		do { якщо (schedstat_enabled()) { var++; } } поки (0)
#define __schedstat_add(var, amt)	do { var += (amt); } поки (0)
#define   schedstat_add(var, amt)	do { якщо (schedstat_enabled()) { var += (amt); } } поки (0)
#define __schedstat_set(var, val)	do { var = (val); } поки (0)
#define   schedstat_set(var, val)	do { якщо (schedstat_enabled()) { var = (val); } } поки (0)
#define   schedstat_val(var)		(var)
#define   schedstat_val_or_zero(var)	((schedstat_enabled()) ? (var) : 0)

void __update_stats_wait_start(struct rq *rq, struct task_struct *p,
			       struct sched_statistics *stats);

void __update_stats_wait_end(struct rq *rq, struct task_struct *p,
			     struct sched_statistics *stats);
void __update_stats_enqueue_sleeper(struct rq *rq, struct task_struct *p,
				    struct sched_statistics *stats);

static inline void
check_schedstat_required(void)
{
	якщо (schedstat_enabled())
		повернути;

	/* Force schedstat enabled if a dependent tracepoint is active */
	якщо (trace_sched_stat_wait_enabled()    ||
	    trace_sched_stat_sleep_enabled()   ||
	    trace_sched_stat_iowait_enabled()  ||
	    trace_sched_stat_blocked_enabled() ||
	    trace_sched_stat_runtime_enabled())
		printk_deferred_once("Scheduler tracepoints stat_sleep, stat_iowait, stat_blocked and stat_runtime require the kernel parameter schedstats=enable or kernel.sched_schedstats=1\n");
}

#інакше /* !CONFIG_SCHEDSTATS: */

static inline void rq_sched_info_arrive  (struct rq *rq, unsigned long long delta) { }
static inline void rq_sched_info_dequeue(struct rq *rq, unsigned long long delta) { }
static inline void rq_sched_info_depart  (struct rq *rq, unsigned long long delta) { }
# define   schedstat_enabled()		0
# define __schedstat_inc(var)		do { } поки (0)
# define   schedstat_inc(var)		do { } поки (0)
# define __schedstat_add(var, amt)	do { } поки (0)
# define   schedstat_add(var, amt)	do { } поки (0)
# define __schedstat_set(var, val)	do { } поки (0)
# define   schedstat_set(var, val)	do { } поки (0)
# define   schedstat_val(var)		0
# define   schedstat_val_or_zero(var)	0

# define __update_stats_wait_start(rq, p, stats)       do { } поки (0)
# define __update_stats_wait_end(rq, p, stats)         do { } поки (0)
# define __update_stats_enqueue_sleeper(rq, p, stats)  do { } поки (0)
# define check_schedstat_required()                    do { } поки (0)

#endif /* CONFIG_SCHEDSTATS */

static inline struct sched_statistics *
__schedstats_from_se(struct sched_entity *se)
{
#ifdef CONFIG_FAIR_GROUP_SCHED
	якщо (!entity_is_task(se))
		повернути &container_of(se, struct cfs_tg_state, se)->stats;
#endif
	повернути &task_of(se)->stats;
}

#ifdef CONFIG_PSI
void psi_task_change(struct task_struct *task, ціле clear, ціле set);
void psi_task_switch(struct task_struct *prev, struct task_struct *next,
		     bool sleep);
#ifdef CONFIG_IRQ_TIME_ACCOUNTING
void psi_account_irqtime(struct rq *rq, struct task_struct *curr, struct task_struct *prev);
#інакше /* !CONFIG_IRQ_TIME_ACCOUNTING: */
static inline void psi_account_irqtime(struct rq *rq, struct task_struct *curr,
				       struct task_struct *prev) {}
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
static inline void psi_enqueue(struct task_struct *p, ціле flags)
{
	ціле clear = 0, set = 0;

	якщо (static_branch_likely(&psi_disabled))
		повернути;

	/* Same runqueue, nothing changed for psi */
	якщо (flags & ENQUEUE_RESTORE)
		повернути;

	/* psi_sched_switch() will handle the flags */
	якщо (task_on_cpu(task_rq(p), p))
		повернути;

	якщо (p->se.sched_delayed) {
		/* CPU migration of "sleeping" task */
		WARN_ON_ONCE(!(flags & ENQUEUE_MIGRATED));
		якщо (p->in_memstall)
			set |= TSK_MEMSTALL;
		якщо (p->in_iowait)
			set |= TSK_IOWAIT;
	} інакше якщо (flags & ENQUEUE_MIGRATED) {
		/* CPU migration of runnable task */
		set = TSK_RUNNING;
		якщо (p->in_memstall)
			set |= TSK_MEMSTALL | TSK_MEMSTALL_RUNNING;
	} інакше {
		/* Wakeup of new or sleeping task */
		якщо (p->in_iowait)
			clear |= TSK_IOWAIT;
		set = TSK_RUNNING;
		якщо (p->in_memstall)
			set |= TSK_MEMSTALL_RUNNING;
	}

	psi_task_change(p, clear, set);
}

static inline void psi_dequeue(struct task_struct *p, ціле flags)
{
	якщо (static_branch_likely(&psi_disabled))
		повернути;

	/* Same runqueue, nothing changed for psi */
	якщо (flags & DEQUEUE_SAVE)
		повернути;

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
	якщо ((flags & DEQUEUE_SLEEP) && (p->psi_flags & TSK_ONCPU))
		повернути;

	/*
	 * When migrating a task to another CPU, clear all psi
	 * state. The enqueue callback above will work it out.
	 */
	psi_task_change(p, p->psi_flags, 0);
}

static inline void psi_ttwu_dequeue(struct task_struct *p)
{
	якщо (static_branch_likely(&psi_disabled))
		повернути;
	/*
	 * Is the task being migrated during a wakeup? Make sure to
	 * deregister its sleep-persistent psi states from the old
	 * queue, and let psi_enqueue() know it has to requeue.
	 */
	якщо (unlikely(p->psi_flags)) {
		struct rq_flags rf;
		struct rq *rq;

		rq = __task_rq_lock(p, &rf);
		psi_task_change(p, p->psi_flags, 0);
		__task_rq_unlock(rq, p, &rf);
	}
}

static inline void psi_sched_switch(struct task_struct *prev,
				    struct task_struct *next,
				    bool sleep)
{
	якщо (static_branch_likely(&psi_disabled))
		повернути;

	psi_task_switch(prev, next, sleep);
}

#інакше /* !CONFIG_PSI: */
static inline void psi_enqueue(struct task_struct *p, bool migrate) {}
static inline void psi_dequeue(struct task_struct *p, bool migrate) {}
static inline void psi_ttwu_dequeue(struct task_struct *p) {}
static inline void psi_sched_switch(struct task_struct *prev,
				    struct task_struct *next,
				    bool sleep) {}
static inline void psi_account_irqtime(struct rq *rq, struct task_struct *curr,
				       struct task_struct *prev) {}
#endif /* !CONFIG_PSI */

#ifdef CONFIG_SCHED_INFO
/*
 * We are interested in knowing how long it was from the *first* time a
 * task was queued to the time that it finally hit a CPU, we call this routine
 * from dequeue_task() to account for possible rq->clock skew across CPUs. The
 * delta taken on each CPU would annul the skew.
 */
static inline void sched_info_dequeue(struct rq *rq, struct task_struct *t)
{
	unsigned long long delta = 0;

	якщо (!t->sched_info.last_queued)
		повернути;

	delta = rq_clock(rq) - t->sched_info.last_queued;
	t->sched_info.last_queued = 0;
	t->sched_info.run_delay += delta;
	якщо (delta > t->sched_info.max_run_delay) {
		t->sched_info.max_run_delay = delta;
		ktime_get_real_ts64(&t->sched_info.max_run_delay_ts);
	}
	якщо (delta && (!t->sched_info.min_run_delay || delta < t->sched_info.min_run_delay))
		t->sched_info.min_run_delay = delta;
	rq_sched_info_dequeue(rq, delta);
}

/*
 * Called when a task finally hits the CPU.  We can now calculate how
 * long it was waiting to run.  We also note when it began so that we
 * can keep stats on how long its time-slice is.
 */
static void sched_info_arrive(struct rq *rq, struct task_struct *t)
{
	unsigned long long now, delta = 0;

	якщо (!t->sched_info.last_queued)
		повернути;

	now = rq_clock(rq);
	delta = now - t->sched_info.last_queued;
	t->sched_info.last_queued = 0;
	t->sched_info.run_delay += delta;
	t->sched_info.last_arrival = now;
	t->sched_info.pcount++;
	якщо (delta > t->sched_info.max_run_delay) {
		t->sched_info.max_run_delay = delta;
		ktime_get_real_ts64(&t->sched_info.max_run_delay_ts);
	}
	якщо (delta && (!t->sched_info.min_run_delay || delta < t->sched_info.min_run_delay))
		t->sched_info.min_run_delay = delta;

	rq_sched_info_arrive(rq, delta);
}

/*
 * This function is only called from enqueue_task(), but also only updates
 * the timestamp if it is already not set.  It's assumed that
 * sched_info_dequeue() will clear that stamp when appropriate.
 */
static inline void sched_info_enqueue(struct rq *rq, struct task_struct *t)
{
	якщо (!t->sched_info.last_queued)
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
static inline void sched_info_depart(struct rq *rq, struct task_struct *t)
{
	unsigned long long delta = rq_clock(rq) - t->sched_info.last_arrival;

	rq_sched_info_depart(rq, delta);

	якщо (task_is_running(t))
		sched_info_enqueue(rq, t);
}

/*
 * Called when tasks are switched involuntarily due, typically, to expiring
 * their time slice.  (This may also be called when switching to or from
 * the idle task.)  We are only called when prev != next.
 */
static inline void
sched_info_switch(struct rq *rq, struct task_struct *prev, struct task_struct *next)
{
	/*
	 * prev now departs the CPU.  It's not interesting to record
	 * stats about how efficient we were at scheduling the idle
	 * process, however.
	 */
	якщо (prev != rq->idle)
		sched_info_depart(rq, prev);

	якщо (next != rq->idle)
		sched_info_arrive(rq, next);
}

#інакше /* !CONFIG_SCHED_INFO: */
# define sched_info_enqueue(rq, t)	do { } поки (0)
# define sched_info_dequeue(rq, t)	do { } поки (0)
# define sched_info_switch(rq, t, next)	do { } поки (0)
#endif /* !CONFIG_SCHED_INFO */

#endif /* _KERNEL_STATS_H */
