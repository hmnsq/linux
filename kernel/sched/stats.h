/* SPDX-License-Identifier: GPL-2.0 */
#నిర్వచితం_కాకపోతే _KERNEL_STATS_H
#నిర్వచించు _KERNEL_STATS_H

#నిర్వచితమైతే CONFIG_SCHEDSTATS

బాహ్య నిర్మాణం static_కీలకం_false sched_schedstats;

/*
 * Expects runqueue lock to be held for atomicity of update
 */
స్థిర అంతఃవరుస శూన్య
rq_sched_info_arrive(నిర్మాణం rq *rq, గుర్తులేని దీర్ఘ దీర్ఘ delta)
{
	ఒకవేళ (rq) {
		rq->rq_sched_info.నిరంతరం_delay += delta;
		rq->rq_sched_info.pcount++;
	}
}

/*
 * Expects runqueue lock to be held for atomicity of update
 */
స్థిర అంతఃవరుస శూన్య
rq_sched_info_depart(నిర్మాణం rq *rq, గుర్తులేని దీర్ఘ దీర్ఘ delta)
{
	ఒకవేళ (rq)
		rq->rq_cpu_సమయం += delta;
}

స్థిర అంతఃవరుస శూన్య
rq_sched_info_వరుస_నుండి_తీయి(నిర్మాణం rq *rq, గుర్తులేని దీర్ఘ దీర్ఘ delta)
{
	ఒకవేళ (rq)
		rq->rq_sched_info.నిరంతరం_delay += delta;
}
#నిర్వచించు   schedstat_enabled()		static_branch_unlikely(&sched_schedstats)
#నిర్వచించు __schedstat_inc(var)		చేయి { var++; } అయితే (0)
#నిర్వచించు   schedstat_inc(var)		చేయి { ఒకవేళ (schedstat_enabled()) { var++; } } అయితే (0)
#నిర్వచించు __schedstat_add(var, amt)	చేయి { var += (amt); } అయితే (0)
#నిర్వచించు   schedstat_కూడు(var, amt)	చేయి { ఒకవేళ (schedstat_enabled()) { var += (amt); } } అయితే (0)
#నిర్వచించు __schedstat_set(var, val)	చేయి { var = (val); } అయితే (0)
#నిర్వచించు   schedstat_సమితి(var, val)	చేయి { ఒకవేళ (schedstat_enabled()) { var = (val); } } అయితే (0)
#నిర్వచించు   schedstat_val(var)		(var)
#నిర్వచించు   schedstat_val_or_zero(var)	((schedstat_enabled()) ? (var) : 0)

శూన్య __update_stats_wait_start(నిర్మాణం rq *rq, నిర్మాణం task_struct *p,
			       నిర్మాణం sched_statistics *stats);

శూన్య __update_stats_wait_end(నిర్మాణం rq *rq, నిర్మాణం task_struct *p,
			     నిర్మాణం sched_statistics *stats);
శూన్య __update_stats_enqueue_sleeper(నిర్మాణం rq *rq, నిర్మాణం task_struct *p,
				    నిర్మాణం sched_statistics *stats);

స్థిర అంతఃవరుస శూన్య
check_schedstat_required(శూన్య)
{
	ఒకవేళ (schedstat_enabled())
		తిరిగి;

	/* Force schedstat enabled if a dependent tracepoint is active */
	ఒకవేళ (trace_sched_stat_wait_enabled()    ||
	    trace_sched_stat_sleep_enabled()   ||
	    trace_sched_stat_iowait_enabled()  ||
	    trace_sched_stat_blocked_enabled() ||
	    trace_sched_stat_runtime_enabled())
		printk_deferred_once("Scheduler tracepoints stat_sleep, stat_iowait, stat_blocked and stat_runtime require the kernel parameter schedstats=enable or kernel.sched_schedstats=1\n");
}

#లేకపోతే /* !CONFIG_SCHEDSTATS: */

స్థిర అంతఃవరుస శూన్య rq_sched_info_arrive  (నిర్మాణం rq *rq, గుర్తులేని దీర్ఘ దీర్ఘ delta) { }
స్థిర అంతఃవరుస శూన్య rq_sched_info_వరుస_నుండి_తీయి(నిర్మాణం rq *rq, గుర్తులేని దీర్ఘ దీర్ఘ delta) { }
స్థిర అంతఃవరుస శూన్య rq_sched_info_depart  (నిర్మాణం rq *rq, గుర్తులేని దీర్ఘ దీర్ఘ delta) { }
# define   schedstat_enabled()		0
# define __schedstat_inc(var)		చేయి { } అయితే (0)
# define   schedstat_inc(var)		చేయి { } అయితే (0)
# define __schedstat_add(var, amt)	చేయి { } అయితే (0)
# define   schedstat_కూడు(var, amt)	చేయి { } అయితే (0)
# define __schedstat_set(var, val)	చేయి { } అయితే (0)
# define   schedstat_సమితి(var, val)	చేయి { } అయితే (0)
# define   schedstat_val(var)		0
# define   schedstat_val_or_zero(var)	0

# define __update_stats_wait_start(rq, p, stats)       చేయి { } అయితే (0)
# define __update_stats_wait_end(rq, p, stats)         చేయి { } అయితే (0)
# define __update_stats_enqueue_sleeper(rq, p, stats)  చేయి { } అయితే (0)
# define check_schedstat_required()                    చేయి { } అయితే (0)

#అయితే_ముగింపు /* CONFIG_SCHEDSTATS */

స్థిర అంతఃవరుస నిర్మాణం sched_statistics *
__schedstats_from_se(నిర్మాణం sched_entity *se)
{
#నిర్వచితమైతే CONFIG_FAIR_సమూహం_SCHED
	ఒకవేళ (!entity_is_task(se))
		తిరిగి &container_యొక్క(se, నిర్మాణం cfs_tg_state, se)->stats;
#అయితే_ముగింపు
	తిరిగి &task_యొక్క(se)->stats;
}

#నిర్వచితమైతే CONFIG_PSI
శూన్య psi_task_change(నిర్మాణం task_struct *task, పూర్ణాంకం clear, పూర్ణాంకం సమితి);
శూన్య psi_task_switch(నిర్మాణం task_struct *prev, నిర్మాణం task_struct *తదుపరి,
		     bool sleep);
#నిర్వచితమైతే CONFIG_IRQ_సమయం_ACCOUNTING
శూన్య psi_account_irqtime(నిర్మాణం rq *rq, నిర్మాణం task_struct *curr, నిర్మాణం task_struct *prev);
#లేకపోతే /* !CONFIG_IRQ_TIME_ACCOUNTING: */
స్థిర అంతఃవరుస శూన్య psi_account_irqtime(నిర్మాణం rq *rq, నిర్మాణం task_struct *curr,
				       నిర్మాణం task_struct *prev) {}
#అయితే_ముగింపు /* !CONFIG_IRQ_TIME_ACCOUNTING */
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
స్థిర అంతఃవరుస శూన్య psi_వరుసలో_చేర్చు(నిర్మాణం task_struct *p, పూర్ణాంకం flags)
{
	పూర్ణాంకం clear = 0, సమితి = 0;

	ఒకవేళ (static_branch_likely(&psi_disabled))
		తిరిగి;

	/* Same runqueue, nothing changed for psi */
	ఒకవేళ (flags & వరుసలో_చేర్చు_RESTORE)
		తిరిగి;

	/* psi_sched_switch() will handle the flags */
	ఒకవేళ (task_on_cpu(task_rq(p), p))
		తిరిగి;

	ఒకవేళ (p->se.sched_delayed) {
		/* CPU migration of "sleeping" task */
		WARN_ON_ONCE(!(flags & వరుసలో_చేర్చు_MIGRATED));
		ఒకవేళ (p->లోపల_memstall)
			సమితి |= TSK_MEMSTALL;
		ఒకవేళ (p->లోపల_iowait)
			సమితి |= TSK_IOWAIT;
	} లేకపోతే ఒకవేళ (flags & వరుసలో_చేర్చు_MIGRATED) {
		/* CPU migration of runnable task */
		సమితి = TSK_RUNNING;
		ఒకవేళ (p->లోపల_memstall)
			సమితి |= TSK_MEMSTALL | TSK_MEMSTALL_RUNNING;
	} లేకపోతే {
		/* Wakeup of new or sleeping task */
		ఒకవేళ (p->లోపల_iowait)
			clear |= TSK_IOWAIT;
		సమితి = TSK_RUNNING;
		ఒకవేళ (p->లోపల_memstall)
			సమితి |= TSK_MEMSTALL_RUNNING;
	}

	psi_task_change(p, clear, సమితి);
}

స్థిర అంతఃవరుస శూన్య psi_వరుస_నుండి_తీయి(నిర్మాణం task_struct *p, పూర్ణాంకం flags)
{
	ఒకవేళ (static_branch_likely(&psi_disabled))
		తిరిగి;

	/* Same runqueue, nothing changed for psi */
	ఒకవేళ (flags & వరుస_నుండి_తీయి_SAVE)
		తిరిగి;

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
	ఒకవేళ ((flags & వరుస_నుండి_తీయి_SLEEP) && (p->psi_flags & TSK_ONCPU))
		తిరిగి;

	/*
	 * When migrating a task to another CPU, clear all psi
	 * state. The enqueue callback above will work it out.
	 */
	psi_task_change(p, p->psi_flags, 0);
}

స్థిర అంతఃవరుస శూన్య psi_ttwu_వరుస_నుండి_తీయి(నిర్మాణం task_struct *p)
{
	ఒకవేళ (static_branch_likely(&psi_disabled))
		తిరిగి;
	/*
	 * Is the task being migrated during a wakeup? Make sure to
	 * deregister its sleep-persistent psi states from the old
	 * queue, and let psi_enqueue() know it has to requeue.
	 */
	ఒకవేళ (unlikely(p->psi_flags)) {
		నిర్మాణం rq_flags rf;
		నిర్మాణం rq *rq;

		rq = __task_rq_lock(p, &rf);
		psi_task_change(p, p->psi_flags, 0);
		__task_rq_unlock(rq, p, &rf);
	}
}

స్థిర అంతఃవరుస శూన్య psi_sched_switch(నిర్మాణం task_struct *prev,
				    నిర్మాణం task_struct *తదుపరి,
				    bool sleep)
{
	ఒకవేళ (static_branch_likely(&psi_disabled))
		తిరిగి;

	psi_task_switch(prev, తదుపరి, sleep);
}

#లేకపోతే /* !CONFIG_PSI: */
స్థిర అంతఃవరుస శూన్య psi_వరుసలో_చేర్చు(నిర్మాణం task_struct *p, bool migrate) {}
స్థిర అంతఃవరుస శూన్య psi_వరుస_నుండి_తీయి(నిర్మాణం task_struct *p, bool migrate) {}
స్థిర అంతఃవరుస శూన్య psi_ttwu_వరుస_నుండి_తీయి(నిర్మాణం task_struct *p) {}
స్థిర అంతఃవరుస శూన్య psi_sched_switch(నిర్మాణం task_struct *prev,
				    నిర్మాణం task_struct *తదుపరి,
				    bool sleep) {}
స్థిర అంతఃవరుస శూన్య psi_account_irqtime(నిర్మాణం rq *rq, నిర్మాణం task_struct *curr,
				       నిర్మాణం task_struct *prev) {}
#అయితే_ముగింపు /* !CONFIG_PSI */

#నిర్వచితమైతే CONFIG_SCHED_INFO
/*
 * We are interested in knowing how long it was from the *first* time a
 * task was queued to the time that it finally hit a CPU, we call this routine
 * from dequeue_task() to account for possible rq->clock skew across CPUs. The
 * delta taken on each CPU would annul the skew.
 */
స్థిర అంతఃవరుస శూన్య sched_info_వరుస_నుండి_తీయి(నిర్మాణం rq *rq, నిర్మాణం task_struct *t)
{
	గుర్తులేని దీర్ఘ దీర్ఘ delta = 0;

	ఒకవేళ (!t->sched_info.చివరి_queued)
		తిరిగి;

	delta = rq_clock(rq) - t->sched_info.చివరి_queued;
	t->sched_info.చివరి_queued = 0;
	t->sched_info.నిరంతరం_delay += delta;
	ఒకవేళ (delta > t->sched_info.max_నిరంతరం_delay) {
		t->sched_info.max_నిరంతరం_delay = delta;
		ktime_get_real_ts64(&t->sched_info.max_నిరంతరం_delay_ts);
	}
	ఒకవేళ (delta && (!t->sched_info.min_నిరంతరం_delay || delta < t->sched_info.min_నిరంతరం_delay))
		t->sched_info.min_నిరంతరం_delay = delta;
	rq_sched_info_వరుస_నుండి_తీయి(rq, delta);
}

/*
 * Called when a task finally hits the CPU.  We can now calculate how
 * long it was waiting to run.  We also note when it began so that we
 * can keep stats on how long its time-slice is.
 */
స్థిర శూన్య sched_info_arrive(నిర్మాణం rq *rq, నిర్మాణం task_struct *t)
{
	గుర్తులేని దీర్ఘ దీర్ఘ now, delta = 0;

	ఒకవేళ (!t->sched_info.చివరి_queued)
		తిరిగి;

	now = rq_clock(rq);
	delta = now - t->sched_info.చివరి_queued;
	t->sched_info.చివరి_queued = 0;
	t->sched_info.నిరంతరం_delay += delta;
	t->sched_info.చివరి_రాక = now;
	t->sched_info.pcount++;
	ఒకవేళ (delta > t->sched_info.max_నిరంతరం_delay) {
		t->sched_info.max_నిరంతరం_delay = delta;
		ktime_get_real_ts64(&t->sched_info.max_నిరంతరం_delay_ts);
	}
	ఒకవేళ (delta && (!t->sched_info.min_నిరంతరం_delay || delta < t->sched_info.min_నిరంతరం_delay))
		t->sched_info.min_నిరంతరం_delay = delta;

	rq_sched_info_arrive(rq, delta);
}

/*
 * This function is only called from enqueue_task(), but also only updates
 * the timestamp if it is already not set.  It's assumed that
 * sched_info_dequeue() will clear that stamp when appropriate.
 */
స్థిర అంతఃవరుస శూన్య sched_info_వరుసలో_చేర్చు(నిర్మాణం rq *rq, నిర్మాణం task_struct *t)
{
	ఒకవేళ (!t->sched_info.చివరి_queued)
		t->sched_info.చివరి_queued = rq_clock(rq);
}

/*
 * Called when a process ceases being the active-running process involuntarily
 * due, typically, to expiring its time slice (this may also be called when
 * switching to the idle task).  Now we can calculate how long we ran.
 * Also, if the process is still in the TASK_RUNNING state, call
 * sched_info_enqueue() to mark that it has now again started waiting on
 * the runqueue.
 */
స్థిర అంతఃవరుస శూన్య sched_info_depart(నిర్మాణం rq *rq, నిర్మాణం task_struct *t)
{
	గుర్తులేని దీర్ఘ దీర్ఘ delta = rq_clock(rq) - t->sched_info.చివరి_రాక;

	rq_sched_info_depart(rq, delta);

	ఒకవేళ (task_is_running(t))
		sched_info_వరుసలో_చేర్చు(rq, t);
}

/*
 * Called when tasks are switched involuntarily due, typically, to expiring
 * their time slice.  (This may also be called when switching to or from
 * the idle task.)  We are only called when prev != next.
 */
స్థిర అంతఃవరుస శూన్య
sched_info_switch(నిర్మాణం rq *rq, నిర్మాణం task_struct *prev, నిర్మాణం task_struct *తదుపరి)
{
	/*
	 * prev now departs the CPU.  It's not interesting to record
	 * stats about how efficient we were at scheduling the idle
	 * process, however.
	 */
	ఒకవేళ (prev != rq->idle)
		sched_info_depart(rq, prev);

	ఒకవేళ (తదుపరి != rq->idle)
		sched_info_arrive(rq, తదుపరి);
}

#లేకపోతే /* !CONFIG_SCHED_INFO: */
# define sched_info_వరుసలో_చేర్చు(rq, t)	చేయి { } అయితే (0)
# define sched_info_వరుస_నుండి_తీయి(rq, t)	చేయి { } అయితే (0)
# define sched_info_switch(rq, t, తదుపరి)	చేయి { } అయితే (0)
#అయితే_ముగింపు /* !CONFIG_SCHED_INFO */

#అయితే_ముగింపు /* _KERNEL_STATS_H */
