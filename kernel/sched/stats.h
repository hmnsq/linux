/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _KERNEL_STATS_H
#define _KERNEL_STATS_H

#ifdef CONFIG_SCHEDSTATS

બાહ્ય સંરચના static_ચાવી_false sched_schedstats;

/*
 * Expects runqueue lock to be held for atomicity of update
 */
સ્થિર અંતઃરેખ શૂન્ય
rq_sched_info_arrive(સંરચના rq *rq, unsigned long long delta)
{
	જો (rq) {
		rq->rq_sched_info.run_delay += delta;
		rq->rq_sched_info.pcount++;
	}
}

/*
 * Expects runqueue lock to be held for atomicity of update
 */
સ્થિર અંતઃરેખ શૂન્ય
rq_sched_info_depart(સંરચના rq *rq, unsigned long long delta)
{
	જો (rq)
		rq->rq_cpu_time += delta;
}

સ્થિર અંતઃરેખ શૂન્ય
rq_sched_info_dequeue(સંરચના rq *rq, unsigned long long delta)
{
	જો (rq)
		rq->rq_sched_info.run_delay += delta;
}
#define   schedstat_enabled()		static_branch_unlikely(&sched_schedstats)
#define __schedstat_inc(var)		કરો { var++; } જ્યારે (0)
#define   schedstat_inc(var)		કરો { જો (schedstat_enabled()) { var++; } } જ્યારે (0)
#define __schedstat_add(var, amt)	કરો { var += (amt); } જ્યારે (0)
#define   schedstat_add(var, amt)	કરો { જો (schedstat_enabled()) { var += (amt); } } જ્યારે (0)
#define __schedstat_set(var, val)	કરો { var = (val); } જ્યારે (0)
#define   schedstat_set(var, val)	કરો { જો (schedstat_enabled()) { var = (val); } } જ્યારે (0)
#define   schedstat_val(var)		(var)
#define   schedstat_val_or_zero(var)	((schedstat_enabled()) ? (var) : 0)

શૂન્ય __update_stats_wait_start(સંરચના rq *rq, સંરચના task_struct *p,
			       સંરચના sched_statistics *stats);

શૂન્ય __update_stats_wait_end(સંરચના rq *rq, સંરચના task_struct *p,
			     સંરચના sched_statistics *stats);
શૂન્ય __update_stats_enqueue_sleeper(સંરચના rq *rq, સંરચના task_struct *p,
				    સંરચના sched_statistics *stats);

સ્થિર અંતઃરેખ શૂન્ય
check_schedstat_required(શૂન્ય)
{
	જો (schedstat_enabled())
		પાછા;

	/* Force schedstat enabled if a dependent tracepoint is active */
	જો (trace_sched_stat_wait_enabled()    ||
	    trace_sched_stat_sleep_enabled()   ||
	    trace_sched_stat_iowait_enabled()  ||
	    trace_sched_stat_blocked_enabled() ||
	    trace_sched_stat_runtime_enabled())
		printk_deferred_once("Scheduler tracepoints stat_sleep, stat_iowait, stat_blocked and stat_runtime require the kernel parameter schedstats=enable or kernel.sched_schedstats=1\n");
}

#નહીંતર /* !CONFIG_SCHEDSTATS: */

સ્થિર અંતઃરેખ શૂન્ય rq_sched_info_arrive  (સંરચના rq *rq, unsigned long long delta) { }
સ્થિર અંતઃરેખ શૂન્ય rq_sched_info_dequeue(સંરચના rq *rq, unsigned long long delta) { }
સ્થિર અંતઃરેખ શૂન્ય rq_sched_info_depart  (સંરચના rq *rq, unsigned long long delta) { }
# define   schedstat_enabled()		0
# define __schedstat_inc(var)		કરો { } જ્યારે (0)
# define   schedstat_inc(var)		કરો { } જ્યારે (0)
# define __schedstat_add(var, amt)	કરો { } જ્યારે (0)
# define   schedstat_add(var, amt)	કરો { } જ્યારે (0)
# define __schedstat_set(var, val)	કરો { } જ્યારે (0)
# define   schedstat_set(var, val)	કરો { } જ્યારે (0)
# define   schedstat_val(var)		0
# define   schedstat_val_or_zero(var)	0

# define __update_stats_wait_start(rq, p, stats)       કરો { } જ્યારે (0)
# define __update_stats_wait_end(rq, p, stats)         કરો { } જ્યારે (0)
# define __update_stats_enqueue_sleeper(rq, p, stats)  કરો { } જ્યારે (0)
# define check_schedstat_required()                    કરો { } જ્યારે (0)

#endif /* CONFIG_SCHEDSTATS */

સ્થિર અંતઃરેખ સંરચના sched_statistics *
__schedstats_from_se(સંરચના sched_entity *se)
{
#ifdef CONFIG_FAIR_GROUP_SCHED
	જો (!entity_is_task(se))
		પાછા &container_of(se, સંરચના cfs_tg_state, se)->stats;
#endif
	પાછા &task_of(se)->stats;
}

#ifdef CONFIG_PSI
શૂન્ય psi_task_change(સંરચના task_struct *task, પૂર્ણાંક clear, પૂર્ણાંક set);
શૂન્ય psi_task_switch(સંરચના task_struct *prev, સંરચના task_struct *next,
		     bool sleep);
#ifdef CONFIG_IRQ_TIME_ACCOUNTING
શૂન્ય psi_account_irqtime(સંરચના rq *rq, સંરચના task_struct *curr, સંરચના task_struct *prev);
#નહીંતર /* !CONFIG_IRQ_TIME_ACCOUNTING: */
સ્થિર અંતઃરેખ શૂન્ય psi_account_irqtime(સંરચના rq *rq, સંરચના task_struct *curr,
				       સંરચના task_struct *prev) {}
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
સ્થિર અંતઃરેખ શૂન્ય psi_enqueue(સંરચના task_struct *p, પૂર્ણાંક flags)
{
	પૂર્ણાંક clear = 0, set = 0;

	જો (static_branch_likely(&psi_disabled))
		પાછા;

	/* Same runqueue, nothing changed for psi */
	જો (flags & ENQUEUE_RESTORE)
		પાછા;

	/* psi_sched_switch() will handle the flags */
	જો (task_on_cpu(task_rq(p), p))
		પાછા;

	જો (p->se.sched_delayed) {
		/* CPU migration of "sleeping" task */
		WARN_ON_ONCE(!(flags & ENQUEUE_MIGRATED));
		જો (p->in_memstall)
			set |= TSK_MEMSTALL;
		જો (p->in_iowait)
			set |= TSK_IOWAIT;
	} નહીંતર જો (flags & ENQUEUE_MIGRATED) {
		/* CPU migration of runnable task */
		set = TSK_RUNNING;
		જો (p->in_memstall)
			set |= TSK_MEMSTALL | TSK_MEMSTALL_RUNNING;
	} નહીંતર {
		/* Wakeup of new or sleeping task */
		જો (p->in_iowait)
			clear |= TSK_IOWAIT;
		set = TSK_RUNNING;
		જો (p->in_memstall)
			set |= TSK_MEMSTALL_RUNNING;
	}

	psi_task_change(p, clear, set);
}

સ્થિર અંતઃરેખ શૂન્ય psi_dequeue(સંરચના task_struct *p, પૂર્ણાંક flags)
{
	જો (static_branch_likely(&psi_disabled))
		પાછા;

	/* Same runqueue, nothing changed for psi */
	જો (flags & DEQUEUE_SAVE)
		પાછા;

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
	જો ((flags & DEQUEUE_SLEEP) && (p->psi_flags & TSK_ONCPU))
		પાછા;

	/*
	 * When migrating a task to another CPU, clear all psi
	 * state. The enqueue callback above will work it out.
	 */
	psi_task_change(p, p->psi_flags, 0);
}

સ્થિર અંતઃરેખ શૂન્ય psi_ttwu_dequeue(સંરચના task_struct *p)
{
	જો (static_branch_likely(&psi_disabled))
		પાછા;
	/*
	 * Is the task being migrated during a wakeup? Make sure to
	 * deregister its sleep-persistent psi states from the old
	 * queue, and let psi_enqueue() know it has to requeue.
	 */
	જો (unlikely(p->psi_flags)) {
		સંરચના rq_flags rf;
		સંરચના rq *rq;

		rq = __task_rq_lock(p, &rf);
		psi_task_change(p, p->psi_flags, 0);
		__task_rq_unlock(rq, p, &rf);
	}
}

સ્થિર અંતઃરેખ શૂન્ય psi_sched_switch(સંરચના task_struct *prev,
				    સંરચના task_struct *next,
				    bool sleep)
{
	જો (static_branch_likely(&psi_disabled))
		પાછા;

	psi_task_switch(prev, next, sleep);
}

#નહીંતર /* !CONFIG_PSI: */
સ્થિર અંતઃરેખ શૂન્ય psi_enqueue(સંરચના task_struct *p, bool migrate) {}
સ્થિર અંતઃરેખ શૂન્ય psi_dequeue(સંરચના task_struct *p, bool migrate) {}
સ્થિર અંતઃરેખ શૂન્ય psi_ttwu_dequeue(સંરચના task_struct *p) {}
સ્થિર અંતઃરેખ શૂન્ય psi_sched_switch(સંરચના task_struct *prev,
				    સંરચના task_struct *next,
				    bool sleep) {}
સ્થિર અંતઃરેખ શૂન્ય psi_account_irqtime(સંરચના rq *rq, સંરચના task_struct *curr,
				       સંરચના task_struct *prev) {}
#endif /* !CONFIG_PSI */

#ifdef CONFIG_SCHED_INFO
/*
 * We are interested in knowing how long it was from the *first* time a
 * task was queued to the time that it finally hit a CPU, we call this routine
 * from dequeue_task() to account for possible rq->clock skew across CPUs. The
 * delta taken on each CPU would annul the skew.
 */
સ્થિર અંતઃરેખ શૂન્ય sched_info_dequeue(સંરચના rq *rq, સંરચના task_struct *t)
{
	unsigned long long delta = 0;

	જો (!t->sched_info.last_queued)
		પાછા;

	delta = rq_clock(rq) - t->sched_info.last_queued;
	t->sched_info.last_queued = 0;
	t->sched_info.run_delay += delta;
	જો (delta > t->sched_info.max_run_delay) {
		t->sched_info.max_run_delay = delta;
		ktime_get_real_ts64(&t->sched_info.max_run_delay_ts);
	}
	જો (delta && (!t->sched_info.min_run_delay || delta < t->sched_info.min_run_delay))
		t->sched_info.min_run_delay = delta;
	rq_sched_info_dequeue(rq, delta);
}

/*
 * Called when a task finally hits the CPU.  We can now calculate how
 * long it was waiting to run.  We also note when it began so that we
 * can keep stats on how long its time-slice is.
 */
સ્થિર શૂન્ય sched_info_arrive(સંરચના rq *rq, સંરચના task_struct *t)
{
	unsigned long long now, delta = 0;

	જો (!t->sched_info.last_queued)
		પાછા;

	now = rq_clock(rq);
	delta = now - t->sched_info.last_queued;
	t->sched_info.last_queued = 0;
	t->sched_info.run_delay += delta;
	t->sched_info.last_arrival = now;
	t->sched_info.pcount++;
	જો (delta > t->sched_info.max_run_delay) {
		t->sched_info.max_run_delay = delta;
		ktime_get_real_ts64(&t->sched_info.max_run_delay_ts);
	}
	જો (delta && (!t->sched_info.min_run_delay || delta < t->sched_info.min_run_delay))
		t->sched_info.min_run_delay = delta;

	rq_sched_info_arrive(rq, delta);
}

/*
 * This function is only called from enqueue_task(), but also only updates
 * the timestamp if it is already not set.  It's assumed that
 * sched_info_dequeue() will clear that stamp when appropriate.
 */
સ્થિર અંતઃરેખ શૂન્ય sched_info_enqueue(સંરચના rq *rq, સંરચના task_struct *t)
{
	જો (!t->sched_info.last_queued)
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
સ્થિર અંતઃરેખ શૂન્ય sched_info_depart(સંરચના rq *rq, સંરચના task_struct *t)
{
	unsigned long long delta = rq_clock(rq) - t->sched_info.last_arrival;

	rq_sched_info_depart(rq, delta);

	જો (task_is_running(t))
		sched_info_enqueue(rq, t);
}

/*
 * Called when tasks are switched involuntarily due, typically, to expiring
 * their time slice.  (This may also be called when switching to or from
 * the idle task.)  We are only called when prev != next.
 */
સ્થિર અંતઃરેખ શૂન્ય
sched_info_switch(સંરચના rq *rq, સંરચના task_struct *prev, સંરચના task_struct *next)
{
	/*
	 * prev now departs the CPU.  It's not interesting to record
	 * stats about how efficient we were at scheduling the idle
	 * process, however.
	 */
	જો (prev != rq->idle)
		sched_info_depart(rq, prev);

	જો (next != rq->idle)
		sched_info_arrive(rq, next);
}

#નહીંતર /* !CONFIG_SCHED_INFO: */
# define sched_info_enqueue(rq, t)	કરો { } જ્યારે (0)
# define sched_info_dequeue(rq, t)	કરો { } જ્યારે (0)
# define sched_info_switch(rq, t, next)	કરો { } જ્યારે (0)
#endif /* !CONFIG_SCHED_INFO */

#endif /* _KERNEL_STATS_H */
