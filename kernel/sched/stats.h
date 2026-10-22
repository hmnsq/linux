/* SPDX-License-Identifier: GPL-2.0 */
#अगर_अघोषित _KERNEL_STATS_H
#घोषणा _KERNEL_STATS_H

#अगर_घोषित CONFIG_SCHEDSTATS

बाह्य काष्ठा static_कुंजी_false sched_schedstats;

/*
 * Expects runqueue lock to be held for atomicity of update
 */
अटल अंतरभूत व्योम
rq_sched_info_arrive(काष्ठा rq *rq, अचिन्हित दीर्घ दीर्घ delta)
{
	अगर (rq) {
		rq->rq_sched_info.run_delay += delta;
		rq->rq_sched_info.pcount++;
	}
}

/*
 * Expects runqueue lock to be held for atomicity of update
 */
अटल अंतरभूत व्योम
rq_sched_info_depart(काष्ठा rq *rq, अचिन्हित दीर्घ दीर्घ delta)
{
	अगर (rq)
		rq->rq_cpu_time += delta;
}

अटल अंतरभूत व्योम
rq_sched_info_dequeue(काष्ठा rq *rq, अचिन्हित दीर्घ दीर्घ delta)
{
	अगर (rq)
		rq->rq_sched_info.run_delay += delta;
}
#घोषणा   schedstat_enabled()		static_branch_unlikely(&sched_schedstats)
#घोषणा __schedstat_inc(var)		करो { var++; } जबतक (0)
#घोषणा   schedstat_inc(var)		करो { अगर (schedstat_enabled()) { var++; } } जबतक (0)
#घोषणा __schedstat_add(var, amt)	करो { var += (amt); } जबतक (0)
#घोषणा   schedstat_add(var, amt)	करो { अगर (schedstat_enabled()) { var += (amt); } } जबतक (0)
#घोषणा __schedstat_set(var, val)	करो { var = (val); } जबतक (0)
#घोषणा   schedstat_set(var, val)	करो { अगर (schedstat_enabled()) { var = (val); } } जबतक (0)
#घोषणा   schedstat_val(var)		(var)
#घोषणा   schedstat_val_or_zero(var)	((schedstat_enabled()) ? (var) : 0)

व्योम __update_stats_wait_start(काष्ठा rq *rq, काष्ठा task_struct *p,
			       काष्ठा sched_statistics *stats);

व्योम __update_stats_wait_end(काष्ठा rq *rq, काष्ठा task_struct *p,
			     काष्ठा sched_statistics *stats);
व्योम __update_stats_enqueue_sleeper(काष्ठा rq *rq, काष्ठा task_struct *p,
				    काष्ठा sched_statistics *stats);

अटल अंतरभूत व्योम
check_schedstat_required(व्योम)
{
	अगर (schedstat_enabled())
		वापस;

	/* Force schedstat enabled if a dependent tracepoint is active */
	अगर (trace_sched_stat_wait_enabled()    ||
	    trace_sched_stat_sleep_enabled()   ||
	    trace_sched_stat_iowait_enabled()  ||
	    trace_sched_stat_blocked_enabled() ||
	    trace_sched_stat_runtime_enabled())
		printk_deferred_once("Scheduler tracepoints stat_sleep, stat_iowait, stat_blocked and stat_runtime require the kernel parameter schedstats=enable or kernel.sched_schedstats=1\n");
}

#अन्यथा /* !CONFIG_SCHEDSTATS: */

अटल अंतरभूत व्योम rq_sched_info_arrive  (काष्ठा rq *rq, अचिन्हित दीर्घ दीर्घ delta) { }
अटल अंतरभूत व्योम rq_sched_info_dequeue(काष्ठा rq *rq, अचिन्हित दीर्घ दीर्घ delta) { }
अटल अंतरभूत व्योम rq_sched_info_depart  (काष्ठा rq *rq, अचिन्हित दीर्घ दीर्घ delta) { }
# define   schedstat_enabled()		0
# define __schedstat_inc(var)		करो { } जबतक (0)
# define   schedstat_inc(var)		करो { } जबतक (0)
# define __schedstat_add(var, amt)	करो { } जबतक (0)
# define   schedstat_add(var, amt)	करो { } जबतक (0)
# define __schedstat_set(var, val)	करो { } जबतक (0)
# define   schedstat_set(var, val)	करो { } जबतक (0)
# define   schedstat_val(var)		0
# define   schedstat_val_or_zero(var)	0

# define __update_stats_wait_start(rq, p, stats)       करो { } जबतक (0)
# define __update_stats_wait_end(rq, p, stats)         करो { } जबतक (0)
# define __update_stats_enqueue_sleeper(rq, p, stats)  करो { } जबतक (0)
# define check_schedstat_required()                    करो { } जबतक (0)

#पूर्ण_अगर /* CONFIG_SCHEDSTATS */

अटल अंतरभूत काष्ठा sched_statistics *
__schedstats_from_se(काष्ठा sched_entity *se)
{
#अगर_घोषित CONFIG_FAIR_GROUP_SCHED
	अगर (!entity_is_task(se))
		वापस &container_of(se, काष्ठा cfs_tg_state, se)->stats;
#पूर्ण_अगर
	वापस &task_of(se)->stats;
}

#अगर_घोषित CONFIG_PSI
व्योम psi_task_change(काष्ठा task_struct *task, पूर्णांक clear, पूर्णांक set);
व्योम psi_task_switch(काष्ठा task_struct *पूर्व, काष्ठा task_struct *अग्र,
		     bool sleep);
#अगर_घोषित CONFIG_IRQ_TIME_ACCOUNTING
व्योम psi_account_irqtime(काष्ठा rq *rq, काष्ठा task_struct *curr, काष्ठा task_struct *पूर्व);
#अन्यथा /* !CONFIG_IRQ_TIME_ACCOUNTING: */
अटल अंतरभूत व्योम psi_account_irqtime(काष्ठा rq *rq, काष्ठा task_struct *curr,
				       काष्ठा task_struct *पूर्व) {}
#पूर्ण_अगर /* !CONFIG_IRQ_TIME_ACCOUNTING */
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
अटल अंतरभूत व्योम psi_enqueue(काष्ठा task_struct *p, पूर्णांक flags)
{
	पूर्णांक clear = 0, set = 0;

	अगर (static_branch_likely(&psi_disabled))
		वापस;

	/* Same runqueue, nothing changed for psi */
	अगर (flags & ENQUEUE_RESTORE)
		वापस;

	/* psi_sched_switch() will handle the flags */
	अगर (task_on_cpu(task_rq(p), p))
		वापस;

	अगर (p->se.sched_delayed) {
		/* CPU migration of "sleeping" task */
		WARN_ON_ONCE(!(flags & ENQUEUE_MIGRATED));
		अगर (p->in_memstall)
			set |= TSK_MEMSTALL;
		अगर (p->in_iowait)
			set |= TSK_IOWAIT;
	} अन्यथा अगर (flags & ENQUEUE_MIGRATED) {
		/* CPU migration of runnable task */
		set = TSK_RUNNING;
		अगर (p->in_memstall)
			set |= TSK_MEMSTALL | TSK_MEMSTALL_RUNNING;
	} अन्यथा {
		/* Wakeup of new or sleeping task */
		अगर (p->in_iowait)
			clear |= TSK_IOWAIT;
		set = TSK_RUNNING;
		अगर (p->in_memstall)
			set |= TSK_MEMSTALL_RUNNING;
	}

	psi_task_change(p, clear, set);
}

अटल अंतरभूत व्योम psi_dequeue(काष्ठा task_struct *p, पूर्णांक flags)
{
	अगर (static_branch_likely(&psi_disabled))
		वापस;

	/* Same runqueue, nothing changed for psi */
	अगर (flags & DEQUEUE_SAVE)
		वापस;

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
	अगर ((flags & DEQUEUE_SLEEP) && (p->psi_flags & TSK_ONCPU))
		वापस;

	/*
	 * When migrating a task to another CPU, clear all psi
	 * state. The enqueue callback above will work it out.
	 */
	psi_task_change(p, p->psi_flags, 0);
}

अटल अंतरभूत व्योम psi_ttwu_dequeue(काष्ठा task_struct *p)
{
	अगर (static_branch_likely(&psi_disabled))
		वापस;
	/*
	 * Is the task being migrated during a wakeup? Make sure to
	 * deregister its sleep-persistent psi states from the old
	 * queue, and let psi_enqueue() know it has to requeue.
	 */
	अगर (unlikely(p->psi_flags)) {
		काष्ठा rq_flags rf;
		काष्ठा rq *rq;

		rq = __task_rq_lock(p, &rf);
		psi_task_change(p, p->psi_flags, 0);
		__task_rq_unlock(rq, p, &rf);
	}
}

अटल अंतरभूत व्योम psi_sched_switch(काष्ठा task_struct *पूर्व,
				    काष्ठा task_struct *अग्र,
				    bool sleep)
{
	अगर (static_branch_likely(&psi_disabled))
		वापस;

	psi_task_switch(पूर्व, अग्र, sleep);
}

#अन्यथा /* !CONFIG_PSI: */
अटल अंतरभूत व्योम psi_enqueue(काष्ठा task_struct *p, bool migrate) {}
अटल अंतरभूत व्योम psi_dequeue(काष्ठा task_struct *p, bool migrate) {}
अटल अंतरभूत व्योम psi_ttwu_dequeue(काष्ठा task_struct *p) {}
अटल अंतरभूत व्योम psi_sched_switch(काष्ठा task_struct *पूर्व,
				    काष्ठा task_struct *अग्र,
				    bool sleep) {}
अटल अंतरभूत व्योम psi_account_irqtime(काष्ठा rq *rq, काष्ठा task_struct *curr,
				       काष्ठा task_struct *पूर्व) {}
#पूर्ण_अगर /* !CONFIG_PSI */

#अगर_घोषित CONFIG_SCHED_INFO
/*
 * We are interested in knowing how long it was from the *first* time a
 * task was queued to the time that it finally hit a CPU, we call this routine
 * from dequeue_task() to account for possible rq->clock skew across CPUs. The
 * delta taken on each CPU would annul the skew.
 */
अटल अंतरभूत व्योम sched_info_dequeue(काष्ठा rq *rq, काष्ठा task_struct *t)
{
	अचिन्हित दीर्घ दीर्घ delta = 0;

	अगर (!t->sched_info.last_queued)
		वापस;

	delta = rq_clock(rq) - t->sched_info.last_queued;
	t->sched_info.last_queued = 0;
	t->sched_info.run_delay += delta;
	अगर (delta > t->sched_info.max_run_delay) {
		t->sched_info.max_run_delay = delta;
		ktime_get_real_ts64(&t->sched_info.max_run_delay_ts);
	}
	अगर (delta && (!t->sched_info.min_run_delay || delta < t->sched_info.min_run_delay))
		t->sched_info.min_run_delay = delta;
	rq_sched_info_dequeue(rq, delta);
}

/*
 * Called when a task finally hits the CPU.  We can now calculate how
 * long it was waiting to run.  We also note when it began so that we
 * can keep stats on how long its time-slice is.
 */
अटल व्योम sched_info_arrive(काष्ठा rq *rq, काष्ठा task_struct *t)
{
	अचिन्हित दीर्घ दीर्घ now, delta = 0;

	अगर (!t->sched_info.last_queued)
		वापस;

	now = rq_clock(rq);
	delta = now - t->sched_info.last_queued;
	t->sched_info.last_queued = 0;
	t->sched_info.run_delay += delta;
	t->sched_info.last_arrival = now;
	t->sched_info.pcount++;
	अगर (delta > t->sched_info.max_run_delay) {
		t->sched_info.max_run_delay = delta;
		ktime_get_real_ts64(&t->sched_info.max_run_delay_ts);
	}
	अगर (delta && (!t->sched_info.min_run_delay || delta < t->sched_info.min_run_delay))
		t->sched_info.min_run_delay = delta;

	rq_sched_info_arrive(rq, delta);
}

/*
 * This function is only called from enqueue_task(), but also only updates
 * the timestamp if it is already not set.  It's assumed that
 * sched_info_dequeue() will clear that stamp when appropriate.
 */
अटल अंतरभूत व्योम sched_info_enqueue(काष्ठा rq *rq, काष्ठा task_struct *t)
{
	अगर (!t->sched_info.last_queued)
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
अटल अंतरभूत व्योम sched_info_depart(काष्ठा rq *rq, काष्ठा task_struct *t)
{
	अचिन्हित दीर्घ दीर्घ delta = rq_clock(rq) - t->sched_info.last_arrival;

	rq_sched_info_depart(rq, delta);

	अगर (task_is_running(t))
		sched_info_enqueue(rq, t);
}

/*
 * Called when tasks are switched involuntarily due, typically, to expiring
 * their time slice.  (This may also be called when switching to or from
 * the idle task.)  We are only called when prev != next.
 */
अटल अंतरभूत व्योम
sched_info_switch(काष्ठा rq *rq, काष्ठा task_struct *पूर्व, काष्ठा task_struct *अग्र)
{
	/*
	 * prev now departs the CPU.  It's not interesting to record
	 * stats about how efficient we were at scheduling the idle
	 * process, however.
	 */
	अगर (पूर्व != rq->idle)
		sched_info_depart(rq, पूर्व);

	अगर (अग्र != rq->idle)
		sched_info_arrive(rq, अग्र);
}

#अन्यथा /* !CONFIG_SCHED_INFO: */
# define sched_info_enqueue(rq, t)	करो { } जबतक (0)
# define sched_info_dequeue(rq, t)	करो { } जबतक (0)
# define sched_info_switch(rq, t, अग्र)	करो { } जबतक (0)
#पूर्ण_अगर /* !CONFIG_SCHED_INFO */

#पूर्ण_अगर /* _KERNEL_STATS_H */
