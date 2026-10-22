/* SPDX-License-Identifier: GPL-2.0 */
#جيڪڏهن_وصف_نه_ٿيل _KERNEL_STATS_H
#وصف_ڪريو _KERNEL_STATS_H

#جيڪڏهن_وصف_ٿيل CONFIG_SCHEDSTATS

خارجي بنية static_ڪنجي_false sched_schedstats;

/*
 * Expects runqueue lock to be held for atomicity of update
 */
ساكن مضمن فراغ
rq_sched_info_arrive(بنية rq *rq, بنانشان ڊگهو ڊگهو delta)
{
	إذا (rq) {
		rq->rq_sched_info.لاڳيتو_delay += delta;
		rq->rq_sched_info.pcount++;
	}
}

/*
 * Expects runqueue lock to be held for atomicity of update
 */
ساكن مضمن فراغ
rq_sched_info_depart(بنية rq *rq, بنانشان ڊگهو ڊگهو delta)
{
	إذا (rq)
		rq->rq_cpu_وقت += delta;
}

ساكن مضمن فراغ
rq_sched_info_قطار_مان_ڪڍو(بنية rq *rq, بنانشان ڊگهو ڊگهو delta)
{
	إذا (rq)
		rq->rq_sched_info.لاڳيتو_delay += delta;
}
#وصف_ڪريو   schedstat_enabled()		static_branch_unlikely(&sched_schedstats)
#وصف_ڪريو __schedstat_inc(var)		افعل { var++; } طالما (0)
#وصف_ڪريو   schedstat_inc(var)		افعل { إذا (schedstat_enabled()) { var++; } } طالما (0)
#وصف_ڪريو __schedstat_add(var, amt)	افعل { var += (amt); } طالما (0)
#وصف_ڪريو   schedstat_جوڙيو(var, amt)	افعل { إذا (schedstat_enabled()) { var += (amt); } } طالما (0)
#وصف_ڪريو __schedstat_set(var, val)	افعل { var = (val); } طالما (0)
#وصف_ڪريو   schedstat_سيٽ(var, val)	افعل { إذا (schedstat_enabled()) { var = (val); } } طالما (0)
#وصف_ڪريو   schedstat_val(var)		(var)
#وصف_ڪريو   schedstat_val_or_zero(var)	((schedstat_enabled()) ? (var) : 0)

فراغ __update_stats_wait_start(بنية rq *rq, بنية task_struct *p,
			       بنية sched_statistics *stats);

فراغ __update_stats_wait_end(بنية rq *rq, بنية task_struct *p,
			     بنية sched_statistics *stats);
فراغ __update_stats_enqueue_sleeper(بنية rq *rq, بنية task_struct *p,
				    بنية sched_statistics *stats);

ساكن مضمن فراغ
check_schedstat_required(فراغ)
{
	إذا (schedstat_enabled())
		أرجع;

	/* Force schedstat enabled if a dependent tracepoint is active */
	إذا (trace_sched_stat_wait_enabled()    ||
	    trace_sched_stat_sleep_enabled()   ||
	    trace_sched_stat_iowait_enabled()  ||
	    trace_sched_stat_blocked_enabled() ||
	    trace_sched_stat_runtime_enabled())
		printk_deferred_once("Scheduler tracepoints stat_sleep, stat_iowait, stat_blocked and stat_runtime require the kernel parameter schedstats=enable or kernel.sched_schedstats=1\n");
}

#وإلا /* !CONFIG_SCHEDSTATS: */

ساكن مضمن فراغ rq_sched_info_arrive  (بنية rq *rq, بنانشان ڊگهو ڊگهو delta) { }
ساكن مضمن فراغ rq_sched_info_قطار_مان_ڪڍو(بنية rq *rq, بنانشان ڊگهو ڊگهو delta) { }
ساكن مضمن فراغ rq_sched_info_depart  (بنية rq *rq, بنانشان ڊگهو ڊگهو delta) { }
# define   schedstat_enabled()		0
# define __schedstat_inc(var)		افعل { } طالما (0)
# define   schedstat_inc(var)		افعل { } طالما (0)
# define __schedstat_add(var, amt)	افعل { } طالما (0)
# define   schedstat_جوڙيو(var, amt)	افعل { } طالما (0)
# define __schedstat_set(var, val)	افعل { } طالما (0)
# define   schedstat_سيٽ(var, val)	افعل { } طالما (0)
# define   schedstat_val(var)		0
# define   schedstat_val_or_zero(var)	0

# define __update_stats_wait_start(rq, p, stats)       افعل { } طالما (0)
# define __update_stats_wait_end(rq, p, stats)         افعل { } طالما (0)
# define __update_stats_enqueue_sleeper(rq, p, stats)  افعل { } طالما (0)
# define check_schedstat_required()                    افعل { } طالما (0)

#جيڪڏهن_پورو /* CONFIG_SCHEDSTATS */

ساكن مضمن بنية sched_statistics *
__schedstats_from_se(بنية sched_entity *se)
{
#جيڪڏهن_وصف_ٿيل CONFIG_FAIR_گروهه_SCHED
	إذا (!entity_is_task(se))
		أرجع &container_جو(se, بنية cfs_tg_state, se)->stats;
#جيڪڏهن_پورو
	أرجع &task_جو(se)->stats;
}

#جيڪڏهن_وصف_ٿيل CONFIG_PSI
فراغ psi_task_change(بنية task_struct *task, صحيح clear, صحيح سيٽ);
فراغ psi_task_switch(بنية task_struct *prev, بنية task_struct *ايندڙ,
		     bool sleep);
#جيڪڏهن_وصف_ٿيل CONFIG_IRQ_وقت_ACCOUNTING
فراغ psi_account_irqtime(بنية rq *rq, بنية task_struct *curr, بنية task_struct *prev);
#وإلا /* !CONFIG_IRQ_TIME_ACCOUNTING: */
ساكن مضمن فراغ psi_account_irqtime(بنية rq *rq, بنية task_struct *curr,
				       بنية task_struct *prev) {}
#جيڪڏهن_پورو /* !CONFIG_IRQ_TIME_ACCOUNTING */
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
ساكن مضمن فراغ psi_قطار_اندر_وجهو(بنية task_struct *p, صحيح flags)
{
	صحيح clear = 0, سيٽ = 0;

	إذا (static_branch_likely(&psi_disabled))
		أرجع;

	/* Same runqueue, nothing changed for psi */
	إذا (flags & قطار_اندر_وجهو_RESTORE)
		أرجع;

	/* psi_sched_switch() will handle the flags */
	إذا (task_on_cpu(task_rq(p), p))
		أرجع;

	إذا (p->se.sched_delayed) {
		/* CPU migration of "sleeping" task */
		WARN_ON_ONCE(!(flags & قطار_اندر_وجهو_MIGRATED));
		إذا (p->اندريون_memstall)
			سيٽ |= TSK_MEMSTALL;
		إذا (p->اندريون_iowait)
			سيٽ |= TSK_IOWAIT;
	} وإلا إذا (flags & قطار_اندر_وجهو_MIGRATED) {
		/* CPU migration of runnable task */
		سيٽ = TSK_RUNNING;
		إذا (p->اندريون_memstall)
			سيٽ |= TSK_MEMSTALL | TSK_MEMSTALL_RUNNING;
	} وإلا {
		/* Wakeup of new or sleeping task */
		إذا (p->اندريون_iowait)
			clear |= TSK_IOWAIT;
		سيٽ = TSK_RUNNING;
		إذا (p->اندريون_memstall)
			سيٽ |= TSK_MEMSTALL_RUNNING;
	}

	psi_task_change(p, clear, سيٽ);
}

ساكن مضمن فراغ psi_قطار_مان_ڪڍو(بنية task_struct *p, صحيح flags)
{
	إذا (static_branch_likely(&psi_disabled))
		أرجع;

	/* Same runqueue, nothing changed for psi */
	إذا (flags & قطار_مان_ڪڍو_SAVE)
		أرجع;

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
	إذا ((flags & قطار_مان_ڪڍو_SLEEP) && (p->psi_flags & TSK_ONCPU))
		أرجع;

	/*
	 * When migrating a task to another CPU, clear all psi
	 * state. The enqueue callback above will work it out.
	 */
	psi_task_change(p, p->psi_flags, 0);
}

ساكن مضمن فراغ psi_ttwu_قطار_مان_ڪڍو(بنية task_struct *p)
{
	إذا (static_branch_likely(&psi_disabled))
		أرجع;
	/*
	 * Is the task being migrated during a wakeup? Make sure to
	 * deregister its sleep-persistent psi states from the old
	 * queue, and let psi_enqueue() know it has to requeue.
	 */
	إذا (unlikely(p->psi_flags)) {
		بنية rq_flags rf;
		بنية rq *rq;

		rq = __task_rq_lock(p, &rf);
		psi_task_change(p, p->psi_flags, 0);
		__task_rq_unlock(rq, p, &rf);
	}
}

ساكن مضمن فراغ psi_sched_switch(بنية task_struct *prev,
				    بنية task_struct *ايندڙ,
				    bool sleep)
{
	إذا (static_branch_likely(&psi_disabled))
		أرجع;

	psi_task_switch(prev, ايندڙ, sleep);
}

#وإلا /* !CONFIG_PSI: */
ساكن مضمن فراغ psi_قطار_اندر_وجهو(بنية task_struct *p, bool migrate) {}
ساكن مضمن فراغ psi_قطار_مان_ڪڍو(بنية task_struct *p, bool migrate) {}
ساكن مضمن فراغ psi_ttwu_قطار_مان_ڪڍو(بنية task_struct *p) {}
ساكن مضمن فراغ psi_sched_switch(بنية task_struct *prev,
				    بنية task_struct *ايندڙ,
				    bool sleep) {}
ساكن مضمن فراغ psi_account_irqtime(بنية rq *rq, بنية task_struct *curr,
				       بنية task_struct *prev) {}
#جيڪڏهن_پورو /* !CONFIG_PSI */

#جيڪڏهن_وصف_ٿيل CONFIG_SCHED_INFO
/*
 * We are interested in knowing how long it was from the *first* time a
 * task was queued to the time that it finally hit a CPU, we call this routine
 * from dequeue_task() to account for possible rq->clock skew across CPUs. The
 * delta taken on each CPU would annul the skew.
 */
ساكن مضمن فراغ sched_info_قطار_مان_ڪڍو(بنية rq *rq, بنية task_struct *t)
{
	بنانشان ڊگهو ڊگهو delta = 0;

	إذا (!t->sched_info.آخري_queued)
		أرجع;

	delta = rq_clock(rq) - t->sched_info.آخري_queued;
	t->sched_info.آخري_queued = 0;
	t->sched_info.لاڳيتو_delay += delta;
	إذا (delta > t->sched_info.max_لاڳيتو_delay) {
		t->sched_info.max_لاڳيتو_delay = delta;
		ktime_get_real_ts64(&t->sched_info.max_لاڳيتو_delay_ts);
	}
	إذا (delta && (!t->sched_info.min_لاڳيتو_delay || delta < t->sched_info.min_لاڳيتو_delay))
		t->sched_info.min_لاڳيتو_delay = delta;
	rq_sched_info_قطار_مان_ڪڍو(rq, delta);
}

/*
 * Called when a task finally hits the CPU.  We can now calculate how
 * long it was waiting to run.  We also note when it began so that we
 * can keep stats on how long its time-slice is.
 */
ساكن فراغ sched_info_arrive(بنية rq *rq, بنية task_struct *t)
{
	بنانشان ڊگهو ڊگهو now, delta = 0;

	إذا (!t->sched_info.آخري_queued)
		أرجع;

	now = rq_clock(rq);
	delta = now - t->sched_info.آخري_queued;
	t->sched_info.آخري_queued = 0;
	t->sched_info.لاڳيتو_delay += delta;
	t->sched_info.آخري_آمد = now;
	t->sched_info.pcount++;
	إذا (delta > t->sched_info.max_لاڳيتو_delay) {
		t->sched_info.max_لاڳيتو_delay = delta;
		ktime_get_real_ts64(&t->sched_info.max_لاڳيتو_delay_ts);
	}
	إذا (delta && (!t->sched_info.min_لاڳيتو_delay || delta < t->sched_info.min_لاڳيتو_delay))
		t->sched_info.min_لاڳيتو_delay = delta;

	rq_sched_info_arrive(rq, delta);
}

/*
 * This function is only called from enqueue_task(), but also only updates
 * the timestamp if it is already not set.  It's assumed that
 * sched_info_dequeue() will clear that stamp when appropriate.
 */
ساكن مضمن فراغ sched_info_قطار_اندر_وجهو(بنية rq *rq, بنية task_struct *t)
{
	إذا (!t->sched_info.آخري_queued)
		t->sched_info.آخري_queued = rq_clock(rq);
}

/*
 * Called when a process ceases being the active-running process involuntarily
 * due, typically, to expiring its time slice (this may also be called when
 * switching to the idle task).  Now we can calculate how long we ran.
 * Also, if the process is still in the TASK_RUNNING state, call
 * sched_info_enqueue() to mark that it has now again started waiting on
 * the runqueue.
 */
ساكن مضمن فراغ sched_info_depart(بنية rq *rq, بنية task_struct *t)
{
	بنانشان ڊگهو ڊگهو delta = rq_clock(rq) - t->sched_info.آخري_آمد;

	rq_sched_info_depart(rq, delta);

	إذا (task_is_running(t))
		sched_info_قطار_اندر_وجهو(rq, t);
}

/*
 * Called when tasks are switched involuntarily due, typically, to expiring
 * their time slice.  (This may also be called when switching to or from
 * the idle task.)  We are only called when prev != next.
 */
ساكن مضمن فراغ
sched_info_switch(بنية rq *rq, بنية task_struct *prev, بنية task_struct *ايندڙ)
{
	/*
	 * prev now departs the CPU.  It's not interesting to record
	 * stats about how efficient we were at scheduling the idle
	 * process, however.
	 */
	إذا (prev != rq->idle)
		sched_info_depart(rq, prev);

	إذا (ايندڙ != rq->idle)
		sched_info_arrive(rq, ايندڙ);
}

#وإلا /* !CONFIG_SCHED_INFO: */
# define sched_info_قطار_اندر_وجهو(rq, t)	افعل { } طالما (0)
# define sched_info_قطار_مان_ڪڍو(rq, t)	افعل { } طالما (0)
# define sched_info_switch(rq, t, ايندڙ)	افعل { } طالما (0)
#جيڪڏهن_پورو /* !CONFIG_SCHED_INFO */

#جيڪڏهن_پورو /* _KERNEL_STATS_H */
