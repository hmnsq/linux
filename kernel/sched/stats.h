/* SPDX-License-Identifier: GPL-2.0 */
#جے_بنا_تعریف _KERNEL_STATS_H
#تعریف_کرو _KERNEL_STATS_H

#جے_تعریف_شدہ CONFIG_SCHEDSTATS

خارجي بنية static_کلید_false sched_schedstats;

/*
 * Expects runqueue lock to be held for atomicity of update
 */
ساكن مضمن فراغ
rq_sched_info_arrive(بنية rq *rq, بنانشانی لما لما delta)
{
	إذا (rq) {
		rq->rq_sched_info.لگاتار_delay += delta;
		rq->rq_sched_info.pcount++;
	}
}

/*
 * Expects runqueue lock to be held for atomicity of update
 */
ساكن مضمن فراغ
rq_sched_info_depart(بنية rq *rq, بنانشانی لما لما delta)
{
	إذا (rq)
		rq->rq_cpu_ویلا += delta;
}

ساكن مضمن فراغ
rq_sched_info_قطار_توں_کڈھو(بنية rq *rq, بنانشانی لما لما delta)
{
	إذا (rq)
		rq->rq_sched_info.لگاتار_delay += delta;
}
#تعریف_کرو   schedstat_enabled()		static_branch_unlikely(&sched_schedstats)
#تعریف_کرو __schedstat_inc(var)		افعل { var++; } طالما (0)
#تعریف_کرو   schedstat_inc(var)		افعل { إذا (schedstat_enabled()) { var++; } } طالما (0)
#تعریف_کرو __schedstat_add(var, amt)	افعل { var += (amt); } طالما (0)
#تعریف_کرو   schedstat_جوڑو(var, amt)	افعل { إذا (schedstat_enabled()) { var += (amt); } } طالما (0)
#تعریف_کرو __schedstat_set(var, val)	افعل { var = (val); } طالما (0)
#تعریف_کرو   schedstat_سیٹ(var, val)	افعل { إذا (schedstat_enabled()) { var = (val); } } طالما (0)
#تعریف_کرو   schedstat_val(var)		(var)
#تعریف_کرو   schedstat_val_or_zero(var)	((schedstat_enabled()) ? (var) : 0)

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

ساكن مضمن فراغ rq_sched_info_arrive  (بنية rq *rq, بنانشانی لما لما delta) { }
ساكن مضمن فراغ rq_sched_info_قطار_توں_کڈھو(بنية rq *rq, بنانشانی لما لما delta) { }
ساكن مضمن فراغ rq_sched_info_depart  (بنية rq *rq, بنانشانی لما لما delta) { }
# define   schedstat_enabled()		0
# define __schedstat_inc(var)		افعل { } طالما (0)
# define   schedstat_inc(var)		افعل { } طالما (0)
# define __schedstat_add(var, amt)	افعل { } طالما (0)
# define   schedstat_جوڑو(var, amt)	افعل { } طالما (0)
# define __schedstat_set(var, val)	افعل { } طالما (0)
# define   schedstat_سیٹ(var, val)	افعل { } طالما (0)
# define   schedstat_val(var)		0
# define   schedstat_val_or_zero(var)	0

# define __update_stats_wait_start(rq, p, stats)       افعل { } طالما (0)
# define __update_stats_wait_end(rq, p, stats)         افعل { } طالما (0)
# define __update_stats_enqueue_sleeper(rq, p, stats)  افعل { } طالما (0)
# define check_schedstat_required()                    افعل { } طالما (0)

#جے_انت /* CONFIG_SCHEDSTATS */

ساكن مضمن بنية sched_statistics *
__schedstats_from_se(بنية sched_entity *se)
{
#جے_تعریف_شدہ CONFIG_FAIR_گروہ_SCHED
	إذا (!entity_is_task(se))
		أرجع &container_دا(se, بنية cfs_tg_state, se)->stats;
#جے_انت
	أرجع &task_دا(se)->stats;
}

#جے_تعریف_شدہ CONFIG_PSI
فراغ psi_task_change(بنية task_struct *task, صحيح clear, صحيح سیٹ);
فراغ psi_task_switch(بنية task_struct *prev, بنية task_struct *اگلا,
		     bool sleep);
#جے_تعریف_شدہ CONFIG_IRQ_ویلا_ACCOUNTING
فراغ psi_account_irqtime(بنية rq *rq, بنية task_struct *curr, بنية task_struct *prev);
#وإلا /* !CONFIG_IRQ_TIME_ACCOUNTING: */
ساكن مضمن فراغ psi_account_irqtime(بنية rq *rq, بنية task_struct *curr,
				       بنية task_struct *prev) {}
#جے_انت /* !CONFIG_IRQ_TIME_ACCOUNTING */
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
ساكن مضمن فراغ psi_قطار_وچ_پاؤ(بنية task_struct *p, صحيح flags)
{
	صحيح clear = 0, سیٹ = 0;

	إذا (static_branch_likely(&psi_disabled))
		أرجع;

	/* Same runqueue, nothing changed for psi */
	إذا (flags & قطار_وچ_پاؤ_RESTORE)
		أرجع;

	/* psi_sched_switch() will handle the flags */
	إذا (task_on_cpu(task_rq(p), p))
		أرجع;

	إذا (p->se.sched_delayed) {
		/* CPU migration of "sleeping" task */
		WARN_ON_ONCE(!(flags & قطار_وچ_پاؤ_MIGRATED));
		إذا (p->وچ_memstall)
			سیٹ |= TSK_MEMSTALL;
		إذا (p->وچ_iowait)
			سیٹ |= TSK_IOWAIT;
	} وإلا إذا (flags & قطار_وچ_پاؤ_MIGRATED) {
		/* CPU migration of runnable task */
		سیٹ = TSK_RUNNING;
		إذا (p->وچ_memstall)
			سیٹ |= TSK_MEMSTALL | TSK_MEMSTALL_RUNNING;
	} وإلا {
		/* Wakeup of new or sleeping task */
		إذا (p->وچ_iowait)
			clear |= TSK_IOWAIT;
		سیٹ = TSK_RUNNING;
		إذا (p->وچ_memstall)
			سیٹ |= TSK_MEMSTALL_RUNNING;
	}

	psi_task_change(p, clear, سیٹ);
}

ساكن مضمن فراغ psi_قطار_توں_کڈھو(بنية task_struct *p, صحيح flags)
{
	إذا (static_branch_likely(&psi_disabled))
		أرجع;

	/* Same runqueue, nothing changed for psi */
	إذا (flags & قطار_توں_کڈھو_SAVE)
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
	إذا ((flags & قطار_توں_کڈھو_SLEEP) && (p->psi_flags & TSK_ONCPU))
		أرجع;

	/*
	 * When migrating a task to another CPU, clear all psi
	 * state. The enqueue callback above will work it out.
	 */
	psi_task_change(p, p->psi_flags, 0);
}

ساكن مضمن فراغ psi_ttwu_قطار_توں_کڈھو(بنية task_struct *p)
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
				    بنية task_struct *اگلا,
				    bool sleep)
{
	إذا (static_branch_likely(&psi_disabled))
		أرجع;

	psi_task_switch(prev, اگلا, sleep);
}

#وإلا /* !CONFIG_PSI: */
ساكن مضمن فراغ psi_قطار_وچ_پاؤ(بنية task_struct *p, bool migrate) {}
ساكن مضمن فراغ psi_قطار_توں_کڈھو(بنية task_struct *p, bool migrate) {}
ساكن مضمن فراغ psi_ttwu_قطار_توں_کڈھو(بنية task_struct *p) {}
ساكن مضمن فراغ psi_sched_switch(بنية task_struct *prev,
				    بنية task_struct *اگلا,
				    bool sleep) {}
ساكن مضمن فراغ psi_account_irqtime(بنية rq *rq, بنية task_struct *curr,
				       بنية task_struct *prev) {}
#جے_انت /* !CONFIG_PSI */

#جے_تعریف_شدہ CONFIG_SCHED_INFO
/*
 * We are interested in knowing how long it was from the *first* time a
 * task was queued to the time that it finally hit a CPU, we call this routine
 * from dequeue_task() to account for possible rq->clock skew across CPUs. The
 * delta taken on each CPU would annul the skew.
 */
ساكن مضمن فراغ sched_info_قطار_توں_کڈھو(بنية rq *rq, بنية task_struct *t)
{
	بنانشانی لما لما delta = 0;

	إذا (!t->sched_info.آخری_queued)
		أرجع;

	delta = rq_clock(rq) - t->sched_info.آخری_queued;
	t->sched_info.آخری_queued = 0;
	t->sched_info.لگاتار_delay += delta;
	إذا (delta > t->sched_info.max_لگاتار_delay) {
		t->sched_info.max_لگاتار_delay = delta;
		ktime_get_real_ts64(&t->sched_info.max_لگاتار_delay_ts);
	}
	إذا (delta && (!t->sched_info.min_لگاتار_delay || delta < t->sched_info.min_لگاتار_delay))
		t->sched_info.min_لگاتار_delay = delta;
	rq_sched_info_قطار_توں_کڈھو(rq, delta);
}

/*
 * Called when a task finally hits the CPU.  We can now calculate how
 * long it was waiting to run.  We also note when it began so that we
 * can keep stats on how long its time-slice is.
 */
ساكن فراغ sched_info_arrive(بنية rq *rq, بنية task_struct *t)
{
	بنانشانی لما لما now, delta = 0;

	إذا (!t->sched_info.آخری_queued)
		أرجع;

	now = rq_clock(rq);
	delta = now - t->sched_info.آخری_queued;
	t->sched_info.آخری_queued = 0;
	t->sched_info.لگاتار_delay += delta;
	t->sched_info.آخری_آمد = now;
	t->sched_info.pcount++;
	إذا (delta > t->sched_info.max_لگاتار_delay) {
		t->sched_info.max_لگاتار_delay = delta;
		ktime_get_real_ts64(&t->sched_info.max_لگاتار_delay_ts);
	}
	إذا (delta && (!t->sched_info.min_لگاتار_delay || delta < t->sched_info.min_لگاتار_delay))
		t->sched_info.min_لگاتار_delay = delta;

	rq_sched_info_arrive(rq, delta);
}

/*
 * This function is only called from enqueue_task(), but also only updates
 * the timestamp if it is already not set.  It's assumed that
 * sched_info_dequeue() will clear that stamp when appropriate.
 */
ساكن مضمن فراغ sched_info_قطار_وچ_پاؤ(بنية rq *rq, بنية task_struct *t)
{
	إذا (!t->sched_info.آخری_queued)
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
ساكن مضمن فراغ sched_info_depart(بنية rq *rq, بنية task_struct *t)
{
	بنانشانی لما لما delta = rq_clock(rq) - t->sched_info.آخری_آمد;

	rq_sched_info_depart(rq, delta);

	إذا (task_is_running(t))
		sched_info_قطار_وچ_پاؤ(rq, t);
}

/*
 * Called when tasks are switched involuntarily due, typically, to expiring
 * their time slice.  (This may also be called when switching to or from
 * the idle task.)  We are only called when prev != next.
 */
ساكن مضمن فراغ
sched_info_switch(بنية rq *rq, بنية task_struct *prev, بنية task_struct *اگلا)
{
	/*
	 * prev now departs the CPU.  It's not interesting to record
	 * stats about how efficient we were at scheduling the idle
	 * process, however.
	 */
	إذا (prev != rq->idle)
		sched_info_depart(rq, prev);

	إذا (اگلا != rq->idle)
		sched_info_arrive(rq, اگلا);
}

#وإلا /* !CONFIG_SCHED_INFO: */
# define sched_info_قطار_وچ_پاؤ(rq, t)	افعل { } طالما (0)
# define sched_info_قطار_توں_کڈھو(rq, t)	افعل { } طالما (0)
# define sched_info_switch(rq, t, اگلا)	افعل { } طالما (0)
#جے_انت /* !CONFIG_SCHED_INFO */

#جے_انت /* _KERNEL_STATS_H */
