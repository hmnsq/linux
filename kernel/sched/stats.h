/* SPDX-License-Identifier: GPL-2.0 */
#اگر_غیر_تعریف_شدہ _KERNEL_STATS_H
#تعریف_کرو _KERNEL_STATS_H

#اگر_تعریف_شدہ CONFIG_SCHEDSTATS

خارجی ساخت static_کلید_false sched_schedstats;

/*
 * Expects runqueue lock to be held for atomicity of update
 */
ساکن اندرونی خالی
rq_sched_info_arrive(ساخت rq *rq, غیرعلامتی طویل طویل delta)
{
	اگر (rq) {
		rq->rq_sched_info.تسلسل_delay += delta;
		rq->rq_sched_info.pcount++;
	}
}

/*
 * Expects runqueue lock to be held for atomicity of update
 */
ساکن اندرونی خالی
rq_sched_info_depart(ساخت rq *rq, غیرعلامتی طویل طویل delta)
{
	اگر (rq)
		rq->rq_cpu_وقت += delta;
}

ساکن اندرونی خالی
rq_sched_info_قطار_سے_نکالو(ساخت rq *rq, غیرعلامتی طویل طویل delta)
{
	اگر (rq)
		rq->rq_sched_info.تسلسل_delay += delta;
}
#تعریف_کرو   schedstat_enabled()		static_branch_unlikely(&sched_schedstats)
#تعریف_کرو __schedstat_inc(var)		کرو { var++; } جب (0)
#تعریف_کرو   schedstat_inc(var)		کرو { اگر (schedstat_enabled()) { var++; } } جب (0)
#تعریف_کرو __schedstat_add(var, amt)	کرو { var += (amt); } جب (0)
#تعریف_کرو   schedstat_جوڑو(var, amt)	کرو { اگر (schedstat_enabled()) { var += (amt); } } جب (0)
#تعریف_کرو __schedstat_set(var, val)	کرو { var = (val); } جب (0)
#تعریف_کرو   schedstat_سیٹ(var, val)	کرو { اگر (schedstat_enabled()) { var = (val); } } جب (0)
#تعریف_کرو   schedstat_val(var)		(var)
#تعریف_کرو   schedstat_val_or_zero(var)	((schedstat_enabled()) ? (var) : 0)

خالی __update_stats_wait_start(ساخت rq *rq, ساخت task_struct *p,
			       ساخت sched_statistics *stats);

خالی __update_stats_wait_end(ساخت rq *rq, ساخت task_struct *p,
			     ساخت sched_statistics *stats);
خالی __update_stats_enqueue_sleeper(ساخت rq *rq, ساخت task_struct *p,
				    ساخت sched_statistics *stats);

ساکن اندرونی خالی
check_schedstat_required(خالی)
{
	اگر (schedstat_enabled())
		واپس;

	/* Force schedstat enabled if a dependent tracepoint is active */
	اگر (trace_sched_stat_wait_enabled()    ||
	    trace_sched_stat_sleep_enabled()   ||
	    trace_sched_stat_iowait_enabled()  ||
	    trace_sched_stat_blocked_enabled() ||
	    trace_sched_stat_runtime_enabled())
		printk_deferred_once("Scheduler tracepoints stat_sleep, stat_iowait, stat_blocked and stat_runtime require the kernel parameter schedstats=enable or kernel.sched_schedstats=1\n");
}

#ورنہ /* !CONFIG_SCHEDSTATS: */

ساکن اندرونی خالی rq_sched_info_arrive  (ساخت rq *rq, غیرعلامتی طویل طویل delta) { }
ساکن اندرونی خالی rq_sched_info_قطار_سے_نکالو(ساخت rq *rq, غیرعلامتی طویل طویل delta) { }
ساکن اندرونی خالی rq_sched_info_depart  (ساخت rq *rq, غیرعلامتی طویل طویل delta) { }
# define   schedstat_enabled()		0
# define __schedstat_inc(var)		کرو { } جب (0)
# define   schedstat_inc(var)		کرو { } جب (0)
# define __schedstat_add(var, amt)	کرو { } جب (0)
# define   schedstat_جوڑو(var, amt)	کرو { } جب (0)
# define __schedstat_set(var, val)	کرو { } جب (0)
# define   schedstat_سیٹ(var, val)	کرو { } جب (0)
# define   schedstat_val(var)		0
# define   schedstat_val_or_zero(var)	0

# define __update_stats_wait_start(rq, p, stats)       کرو { } جب (0)
# define __update_stats_wait_end(rq, p, stats)         کرو { } جب (0)
# define __update_stats_enqueue_sleeper(rq, p, stats)  کرو { } جب (0)
# define check_schedstat_required()                    کرو { } جب (0)

#اگر_ختم /* CONFIG_SCHEDSTATS */

ساکن اندرونی ساخت sched_statistics *
__schedstats_from_se(ساخت sched_entity *se)
{
#اگر_تعریف_شدہ CONFIG_FAIR_گروہ_SCHED
	اگر (!entity_is_task(se))
		واپس &container_کا(se, ساخت cfs_tg_state, se)->stats;
#اگر_ختم
	واپس &task_کا(se)->stats;
}

#اگر_تعریف_شدہ CONFIG_PSI
خالی psi_task_change(ساخت task_struct *task, صحیح clear, صحیح سیٹ);
خالی psi_task_switch(ساخت task_struct *پچھلا, ساخت task_struct *اگلا,
		     bool sleep);
#اگر_تعریف_شدہ CONFIG_IRQ_وقت_ACCOUNTING
خالی psi_account_irqtime(ساخت rq *rq, ساخت task_struct *curr, ساخت task_struct *پچھلا);
#ورنہ /* !CONFIG_IRQ_TIME_ACCOUNTING: */
ساکن اندرونی خالی psi_account_irqtime(ساخت rq *rq, ساخت task_struct *curr,
				       ساخت task_struct *پچھلا) {}
#اگر_ختم /* !CONFIG_IRQ_TIME_ACCOUNTING */
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
ساکن اندرونی خالی psi_قطار_میں_ڈالو(ساخت task_struct *p, صحیح flags)
{
	صحیح clear = 0, سیٹ = 0;

	اگر (static_branch_likely(&psi_disabled))
		واپس;

	/* Same runqueue, nothing changed for psi */
	اگر (flags & قطار_میں_ڈالو_RESTORE)
		واپس;

	/* psi_sched_switch() will handle the flags */
	اگر (task_on_cpu(task_rq(p), p))
		واپس;

	اگر (p->se.sched_delayed) {
		/* CPU migration of "sleeping" task */
		WARN_ON_ONCE(!(flags & قطار_میں_ڈالو_MIGRATED));
		اگر (p->اندر_memstall)
			سیٹ |= TSK_MEMSTALL;
		اگر (p->اندر_iowait)
			سیٹ |= TSK_IOWAIT;
	} ورنہ اگر (flags & قطار_میں_ڈالو_MIGRATED) {
		/* CPU migration of runnable task */
		سیٹ = TSK_RUNNING;
		اگر (p->اندر_memstall)
			سیٹ |= TSK_MEMSTALL | TSK_MEMSTALL_RUNNING;
	} ورنہ {
		/* Wakeup of new or sleeping task */
		اگر (p->اندر_iowait)
			clear |= TSK_IOWAIT;
		سیٹ = TSK_RUNNING;
		اگر (p->اندر_memstall)
			سیٹ |= TSK_MEMSTALL_RUNNING;
	}

	psi_task_change(p, clear, سیٹ);
}

ساکن اندرونی خالی psi_قطار_سے_نکالو(ساخت task_struct *p, صحیح flags)
{
	اگر (static_branch_likely(&psi_disabled))
		واپس;

	/* Same runqueue, nothing changed for psi */
	اگر (flags & قطار_سے_نکالو_SAVE)
		واپس;

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
	اگر ((flags & قطار_سے_نکالو_SLEEP) && (p->psi_flags & TSK_ONCPU))
		واپس;

	/*
	 * When migrating a task to another CPU, clear all psi
	 * state. The enqueue callback above will work it out.
	 */
	psi_task_change(p, p->psi_flags, 0);
}

ساکن اندرونی خالی psi_ttwu_قطار_سے_نکالو(ساخت task_struct *p)
{
	اگر (static_branch_likely(&psi_disabled))
		واپس;
	/*
	 * Is the task being migrated during a wakeup? Make sure to
	 * deregister its sleep-persistent psi states from the old
	 * queue, and let psi_enqueue() know it has to requeue.
	 */
	اگر (unlikely(p->psi_flags)) {
		ساخت rq_flags rf;
		ساخت rq *rq;

		rq = __task_rq_lock(p, &rf);
		psi_task_change(p, p->psi_flags, 0);
		__task_rq_unlock(rq, p, &rf);
	}
}

ساکن اندرونی خالی psi_sched_switch(ساخت task_struct *پچھلا,
				    ساخت task_struct *اگلا,
				    bool sleep)
{
	اگر (static_branch_likely(&psi_disabled))
		واپس;

	psi_task_switch(پچھلا, اگلا, sleep);
}

#ورنہ /* !CONFIG_PSI: */
ساکن اندرونی خالی psi_قطار_میں_ڈالو(ساخت task_struct *p, bool migrate) {}
ساکن اندرونی خالی psi_قطار_سے_نکالو(ساخت task_struct *p, bool migrate) {}
ساکن اندرونی خالی psi_ttwu_قطار_سے_نکالو(ساخت task_struct *p) {}
ساکن اندرونی خالی psi_sched_switch(ساخت task_struct *پچھلا,
				    ساخت task_struct *اگلا,
				    bool sleep) {}
ساکن اندرونی خالی psi_account_irqtime(ساخت rq *rq, ساخت task_struct *curr,
				       ساخت task_struct *پچھلا) {}
#اگر_ختم /* !CONFIG_PSI */

#اگر_تعریف_شدہ CONFIG_SCHED_INFO
/*
 * We are interested in knowing how long it was from the *first* time a
 * task was queued to the time that it finally hit a CPU, we call this routine
 * from dequeue_task() to account for possible rq->clock skew across CPUs. The
 * delta taken on each CPU would annul the skew.
 */
ساکن اندرونی خالی sched_info_قطار_سے_نکالو(ساخت rq *rq, ساخت task_struct *t)
{
	غیرعلامتی طویل طویل delta = 0;

	اگر (!t->sched_info.آخری_queued)
		واپس;

	delta = rq_clock(rq) - t->sched_info.آخری_queued;
	t->sched_info.آخری_queued = 0;
	t->sched_info.تسلسل_delay += delta;
	اگر (delta > t->sched_info.max_تسلسل_delay) {
		t->sched_info.max_تسلسل_delay = delta;
		ktime_get_real_ts64(&t->sched_info.max_تسلسل_delay_ts);
	}
	اگر (delta && (!t->sched_info.min_تسلسل_delay || delta < t->sched_info.min_تسلسل_delay))
		t->sched_info.min_تسلسل_delay = delta;
	rq_sched_info_قطار_سے_نکالو(rq, delta);
}

/*
 * Called when a task finally hits the CPU.  We can now calculate how
 * long it was waiting to run.  We also note when it began so that we
 * can keep stats on how long its time-slice is.
 */
ساکن خالی sched_info_arrive(ساخت rq *rq, ساخت task_struct *t)
{
	غیرعلامتی طویل طویل now, delta = 0;

	اگر (!t->sched_info.آخری_queued)
		واپس;

	now = rq_clock(rq);
	delta = now - t->sched_info.آخری_queued;
	t->sched_info.آخری_queued = 0;
	t->sched_info.تسلسل_delay += delta;
	t->sched_info.آخری_آمد = now;
	t->sched_info.pcount++;
	اگر (delta > t->sched_info.max_تسلسل_delay) {
		t->sched_info.max_تسلسل_delay = delta;
		ktime_get_real_ts64(&t->sched_info.max_تسلسل_delay_ts);
	}
	اگر (delta && (!t->sched_info.min_تسلسل_delay || delta < t->sched_info.min_تسلسل_delay))
		t->sched_info.min_تسلسل_delay = delta;

	rq_sched_info_arrive(rq, delta);
}

/*
 * This function is only called from enqueue_task(), but also only updates
 * the timestamp if it is already not set.  It's assumed that
 * sched_info_dequeue() will clear that stamp when appropriate.
 */
ساکن اندرونی خالی sched_info_قطار_میں_ڈالو(ساخت rq *rq, ساخت task_struct *t)
{
	اگر (!t->sched_info.آخری_queued)
		t->sched_info.آخری_queued = rq_clock(rq);
}

/*
 * Called when a process ceases being the active-running process involuntarily
 * due, typically, to expiring its time slice (this may also be called when
 * switching to the idle task).  Now we can calculate how long we ran.
 * Also, if the process is still in the TASK_RUNNING state, call
 * sched_info_enqueue() to mark that it has now again started waiting on
 * the runqueue.
 */
ساکن اندرونی خالی sched_info_depart(ساخت rq *rq, ساخت task_struct *t)
{
	غیرعلامتی طویل طویل delta = rq_clock(rq) - t->sched_info.آخری_آمد;

	rq_sched_info_depart(rq, delta);

	اگر (task_is_running(t))
		sched_info_قطار_میں_ڈالو(rq, t);
}

/*
 * Called when tasks are switched involuntarily due, typically, to expiring
 * their time slice.  (This may also be called when switching to or from
 * the idle task.)  We are only called when prev != next.
 */
ساکن اندرونی خالی
sched_info_switch(ساخت rq *rq, ساخت task_struct *پچھلا, ساخت task_struct *اگلا)
{
	/*
	 * prev now departs the CPU.  It's not interesting to record
	 * stats about how efficient we were at scheduling the idle
	 * process, however.
	 */
	اگر (پچھلا != rq->idle)
		sched_info_depart(rq, پچھلا);

	اگر (اگلا != rq->idle)
		sched_info_arrive(rq, اگلا);
}

#ورنہ /* !CONFIG_SCHED_INFO: */
# define sched_info_قطار_میں_ڈالو(rq, t)	کرو { } جب (0)
# define sched_info_قطار_سے_نکالو(rq, t)	کرو { } جب (0)
# define sched_info_switch(rq, t, اگلا)	کرو { } جب (0)
#اگر_ختم /* !CONFIG_SCHED_INFO */

#اگر_ختم /* _KERNEL_STATS_H */
