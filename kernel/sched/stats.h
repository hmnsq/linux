/* SPDX-License-Identifier: GPL-2.0 */
#यदि_अपरिभाषितम् _KERNEL_STATS_H
#परिभाषय _KERNEL_STATS_H

#यदि_परिभाषितम् CONFIG_SCHEDSTATS

बाह्य संरचना static_कुञ्चिका_false sched_schedstats;

/*
 * Expects runqueue lock to be held for atomicity of update
 */
स्थिर अन्तःस्थ शून्य
rq_sched_info_arrive(संरचना rq *rq, अचिह्नित दीर्घ दीर्घ delta)
{
	यदि (rq) {
		rq->rq_sched_info.सततम्_delay += delta;
		rq->rq_sched_info.pcount++;
	}
}

/*
 * Expects runqueue lock to be held for atomicity of update
 */
स्थिर अन्तःस्थ शून्य
rq_sched_info_depart(संरचना rq *rq, अचिह्नित दीर्घ दीर्घ delta)
{
	यदि (rq)
		rq->rq_cpu_time += delta;
}

स्थिर अन्तःस्थ शून्य
rq_sched_info_पङ्क्तेः_निष्कासय(संरचना rq *rq, अचिह्नित दीर्घ दीर्घ delta)
{
	यदि (rq)
		rq->rq_sched_info.सततम्_delay += delta;
}
#परिभाषय   schedstat_enabled()		static_branch_unlikely(&sched_schedstats)
#परिभाषय __schedstat_inc(var)		कुरु { var++; } यावत् (0)
#परिभाषय   schedstat_inc(var)		कुरु { यदि (schedstat_enabled()) { var++; } } यावत् (0)
#परिभाषय __schedstat_add(var, amt)	कुरु { var += (amt); } यावत् (0)
#परिभाषय   schedstat_सङ्कलय(var, amt)	कुरु { यदि (schedstat_enabled()) { var += (amt); } } यावत् (0)
#परिभाषय __schedstat_set(var, val)	कुरु { var = (val); } यावत् (0)
#परिभाषय   schedstat_समुच्चयः(var, val)	कुरु { यदि (schedstat_enabled()) { var = (val); } } यावत् (0)
#परिभाषय   schedstat_val(var)		(var)
#परिभाषय   schedstat_val_or_zero(var)	((schedstat_enabled()) ? (var) : 0)

शून्य __update_stats_wait_start(संरचना rq *rq, संरचना task_struct *p,
			       संरचना sched_statistics *stats);

शून्य __update_stats_wait_end(संरचना rq *rq, संरचना task_struct *p,
			     संरचना sched_statistics *stats);
शून्य __update_stats_enqueue_sleeper(संरचना rq *rq, संरचना task_struct *p,
				    संरचना sched_statistics *stats);

स्थिर अन्तःस्थ शून्य
check_schedstat_required(शून्य)
{
	यदि (schedstat_enabled())
		प्रत्यागम;

	/* Force schedstat enabled if a dependent tracepoint is active */
	यदि (trace_sched_stat_wait_enabled()    ||
	    trace_sched_stat_sleep_enabled()   ||
	    trace_sched_stat_iowait_enabled()  ||
	    trace_sched_stat_blocked_enabled() ||
	    trace_sched_stat_runtime_enabled())
		printk_deferred_once("Scheduler tracepoints stat_sleep, stat_iowait, stat_blocked and stat_runtime require the kernel parameter schedstats=enable or kernel.sched_schedstats=1\n");
}

#अन्यथा /* !CONFIG_SCHEDSTATS: */

स्थिर अन्तःस्थ शून्य rq_sched_info_arrive  (संरचना rq *rq, अचिह्नित दीर्घ दीर्घ delta) { }
स्थिर अन्तःस्थ शून्य rq_sched_info_पङ्क्तेः_निष्कासय(संरचना rq *rq, अचिह्नित दीर्घ दीर्घ delta) { }
स्थिर अन्तःस्थ शून्य rq_sched_info_depart  (संरचना rq *rq, अचिह्नित दीर्घ दीर्घ delta) { }
# define   schedstat_enabled()		0
# define __schedstat_inc(var)		कुरु { } यावत् (0)
# define   schedstat_inc(var)		कुरु { } यावत् (0)
# define __schedstat_add(var, amt)	कुरु { } यावत् (0)
# define   schedstat_सङ्कलय(var, amt)	कुरु { } यावत् (0)
# define __schedstat_set(var, val)	कुरु { } यावत् (0)
# define   schedstat_समुच्चयः(var, val)	कुरु { } यावत् (0)
# define   schedstat_val(var)		0
# define   schedstat_val_or_zero(var)	0

# define __update_stats_wait_start(rq, p, stats)       कुरु { } यावत् (0)
# define __update_stats_wait_end(rq, p, stats)         कुरु { } यावत् (0)
# define __update_stats_enqueue_sleeper(rq, p, stats)  कुरु { } यावत् (0)
# define check_schedstat_required()                    कुरु { } यावत् (0)

#यदि_अन्तः /* CONFIG_SCHEDSTATS */

स्थिर अन्तःस्थ संरचना sched_statistics *
__schedstats_from_se(संरचना sched_entity *se)
{
#यदि_परिभाषितम् CONFIG_FAIR_गणः_SCHED
	यदि (!entity_is_task(se))
		प्रत्यागम &container_सम्बन्धि(se, संरचना cfs_tg_state, se)->stats;
#यदि_अन्तः
	प्रत्यागम &task_सम्बन्धि(se)->stats;
}

#यदि_परिभाषितम् CONFIG_PSI
शून्य psi_task_change(संरचना task_struct *task, पूर्णाङ्क clear, पूर्णाङ्क समुच्चयः);
शून्य psi_task_switch(संरचना task_struct *पूर्व, संरचना task_struct *अग्र,
		     bool sleep);
#यदि_परिभाषितम् CONFIG_IRQ_TIME_ACCOUNTING
शून्य psi_account_irqtime(संरचना rq *rq, संरचना task_struct *curr, संरचना task_struct *पूर्व);
#अन्यथा /* !CONFIG_IRQ_TIME_ACCOUNTING: */
स्थिर अन्तःस्थ शून्य psi_account_irqtime(संरचना rq *rq, संरचना task_struct *curr,
				       संरचना task_struct *पूर्व) {}
#यदि_अन्तः /* !CONFIG_IRQ_TIME_ACCOUNTING */
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
स्थिर अन्तःस्थ शून्य psi_पङ्क्तौ_निक्षिप(संरचना task_struct *p, पूर्णाङ्क flags)
{
	पूर्णाङ्क clear = 0, समुच्चयः = 0;

	यदि (static_branch_likely(&psi_disabled))
		प्रत्यागम;

	/* Same runqueue, nothing changed for psi */
	यदि (flags & पङ्क्तौ_निक्षिप_RESTORE)
		प्रत्यागम;

	/* psi_sched_switch() will handle the flags */
	यदि (task_on_cpu(task_rq(p), p))
		प्रत्यागम;

	यदि (p->se.sched_delayed) {
		/* CPU migration of "sleeping" task */
		WARN_ON_ONCE(!(flags & पङ्क्तौ_निक्षिप_MIGRATED));
		यदि (p->अन्तर्_memstall)
			समुच्चयः |= TSK_MEMSTALL;
		यदि (p->अन्तर्_iowait)
			समुच्चयः |= TSK_IOWAIT;
	} अन्यथा यदि (flags & पङ्क्तौ_निक्षिप_MIGRATED) {
		/* CPU migration of runnable task */
		समुच्चयः = TSK_RUNNING;
		यदि (p->अन्तर्_memstall)
			समुच्चयः |= TSK_MEMSTALL | TSK_MEMSTALL_RUNNING;
	} अन्यथा {
		/* Wakeup of new or sleeping task */
		यदि (p->अन्तर्_iowait)
			clear |= TSK_IOWAIT;
		समुच्चयः = TSK_RUNNING;
		यदि (p->अन्तर्_memstall)
			समुच्चयः |= TSK_MEMSTALL_RUNNING;
	}

	psi_task_change(p, clear, समुच्चयः);
}

स्थिर अन्तःस्थ शून्य psi_पङ्क्तेः_निष्कासय(संरचना task_struct *p, पूर्णाङ्क flags)
{
	यदि (static_branch_likely(&psi_disabled))
		प्रत्यागम;

	/* Same runqueue, nothing changed for psi */
	यदि (flags & पङ्क्तेः_निष्कासय_SAVE)
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
	यदि ((flags & पङ्क्तेः_निष्कासय_SLEEP) && (p->psi_flags & TSK_ONCPU))
		प्रत्यागम;

	/*
	 * When migrating a task to another CPU, clear all psi
	 * state. The enqueue callback above will work it out.
	 */
	psi_task_change(p, p->psi_flags, 0);
}

स्थिर अन्तःस्थ शून्य psi_ttwu_पङ्क्तेः_निष्कासय(संरचना task_struct *p)
{
	यदि (static_branch_likely(&psi_disabled))
		प्रत्यागम;
	/*
	 * Is the task being migrated during a wakeup? Make sure to
	 * deregister its sleep-persistent psi states from the old
	 * queue, and let psi_enqueue() know it has to requeue.
	 */
	यदि (unlikely(p->psi_flags)) {
		संरचना rq_flags rf;
		संरचना rq *rq;

		rq = __task_rq_lock(p, &rf);
		psi_task_change(p, p->psi_flags, 0);
		__task_rq_unlock(rq, p, &rf);
	}
}

स्थिर अन्तःस्थ शून्य psi_sched_switch(संरचना task_struct *पूर्व,
				    संरचना task_struct *अग्र,
				    bool sleep)
{
	यदि (static_branch_likely(&psi_disabled))
		प्रत्यागम;

	psi_task_switch(पूर्व, अग्र, sleep);
}

#अन्यथा /* !CONFIG_PSI: */
स्थिर अन्तःस्थ शून्य psi_पङ्क्तौ_निक्षिप(संरचना task_struct *p, bool migrate) {}
स्थिर अन्तःस्थ शून्य psi_पङ्क्तेः_निष्कासय(संरचना task_struct *p, bool migrate) {}
स्थिर अन्तःस्थ शून्य psi_ttwu_पङ्क्तेः_निष्कासय(संरचना task_struct *p) {}
स्थिर अन्तःस्थ शून्य psi_sched_switch(संरचना task_struct *पूर्व,
				    संरचना task_struct *अग्र,
				    bool sleep) {}
स्थिर अन्तःस्थ शून्य psi_account_irqtime(संरचना rq *rq, संरचना task_struct *curr,
				       संरचना task_struct *पूर्व) {}
#यदि_अन्तः /* !CONFIG_PSI */

#यदि_परिभाषितम् CONFIG_SCHED_INFO
/*
 * We are interested in knowing how long it was from the *first* time a
 * task was queued to the time that it finally hit a CPU, we call this routine
 * from dequeue_task() to account for possible rq->clock skew across CPUs. The
 * delta taken on each CPU would annul the skew.
 */
स्थिर अन्तःस्थ शून्य sched_info_पङ्क्तेः_निष्कासय(संरचना rq *rq, संरचना task_struct *t)
{
	अचिह्नित दीर्घ दीर्घ delta = 0;

	यदि (!t->sched_info.चरमः_queued)
		प्रत्यागम;

	delta = rq_clock(rq) - t->sched_info.चरमः_queued;
	t->sched_info.चरमः_queued = 0;
	t->sched_info.सततम्_delay += delta;
	यदि (delta > t->sched_info.max_सततम्_delay) {
		t->sched_info.max_सततम्_delay = delta;
		ktime_get_real_ts64(&t->sched_info.max_सततम्_delay_ts);
	}
	यदि (delta && (!t->sched_info.min_सततम्_delay || delta < t->sched_info.min_सततम्_delay))
		t->sched_info.min_सततम्_delay = delta;
	rq_sched_info_पङ्क्तेः_निष्कासय(rq, delta);
}

/*
 * Called when a task finally hits the CPU.  We can now calculate how
 * long it was waiting to run.  We also note when it began so that we
 * can keep stats on how long its time-slice is.
 */
स्थिर शून्य sched_info_arrive(संरचना rq *rq, संरचना task_struct *t)
{
	अचिह्नित दीर्घ दीर्घ now, delta = 0;

	यदि (!t->sched_info.चरमः_queued)
		प्रत्यागम;

	now = rq_clock(rq);
	delta = now - t->sched_info.चरमः_queued;
	t->sched_info.चरमः_queued = 0;
	t->sched_info.सततम्_delay += delta;
	t->sched_info.चरमः_आगमनम् = now;
	t->sched_info.pcount++;
	यदि (delta > t->sched_info.max_सततम्_delay) {
		t->sched_info.max_सततम्_delay = delta;
		ktime_get_real_ts64(&t->sched_info.max_सततम्_delay_ts);
	}
	यदि (delta && (!t->sched_info.min_सततम्_delay || delta < t->sched_info.min_सततम्_delay))
		t->sched_info.min_सततम्_delay = delta;

	rq_sched_info_arrive(rq, delta);
}

/*
 * This function is only called from enqueue_task(), but also only updates
 * the timestamp if it is already not set.  It's assumed that
 * sched_info_dequeue() will clear that stamp when appropriate.
 */
स्थिर अन्तःस्थ शून्य sched_info_पङ्क्तौ_निक्षिप(संरचना rq *rq, संरचना task_struct *t)
{
	यदि (!t->sched_info.चरमः_queued)
		t->sched_info.चरमः_queued = rq_clock(rq);
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
	अचिह्नित दीर्घ दीर्घ delta = rq_clock(rq) - t->sched_info.चरमः_आगमनम्;

	rq_sched_info_depart(rq, delta);

	यदि (task_is_running(t))
		sched_info_पङ्क्तौ_निक्षिप(rq, t);
}

/*
 * Called when tasks are switched involuntarily due, typically, to expiring
 * their time slice.  (This may also be called when switching to or from
 * the idle task.)  We are only called when prev != next.
 */
स्थिर अन्तःस्थ शून्य
sched_info_switch(संरचना rq *rq, संरचना task_struct *पूर्व, संरचना task_struct *अग्र)
{
	/*
	 * prev now departs the CPU.  It's not interesting to record
	 * stats about how efficient we were at scheduling the idle
	 * process, however.
	 */
	यदि (पूर्व != rq->idle)
		sched_info_depart(rq, पूर्व);

	यदि (अग्र != rq->idle)
		sched_info_arrive(rq, अग्र);
}

#अन्यथा /* !CONFIG_SCHED_INFO: */
# define sched_info_पङ्क्तौ_निक्षिप(rq, t)	कुरु { } यावत् (0)
# define sched_info_पङ्क्तेः_निष्कासय(rq, t)	कुरु { } यावत् (0)
# define sched_info_switch(rq, t, अग्र)	कुरु { } यावत् (0)
#यदि_अन्तः /* !CONFIG_SCHED_INFO */

#यदि_अन्तः /* _KERNEL_STATS_H */
