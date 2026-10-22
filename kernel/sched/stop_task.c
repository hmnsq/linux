// SPDX-License-Identifier: GPL-2.0
/*
 * stop-task scheduling class.
 *
 * The stop task is the highest priority task in the system, it preempts
 * everything and will be preempted by nothing.
 *
 * See kernel/stop_machine.c
 */
#अन्तर्भाव "sched.h"

स्थिर पूर्णांक
select_task_rq_stop(संरचना task_struct *p, पूर्णांक cpu, पूर्णांक flags)
{
	परत task_cpu(p); /* stop tasks as never migrate */
}

स्थिर पूर्णांक
balance_stop(संरचना rq *rq, संरचना rq_flags *rf)
{
	परत sched_stop_runnable(rq);
}

स्थिर शून्य
wakeup_preempt_stop(संरचना rq *rq, संरचना task_struct *p, पूर्णांक flags)
{
	/* we're never preempted */
}

स्थिर शून्य set_next_task_stop(संरचना rq *rq, संरचना task_struct *stop, गणना snt_e type)
{
	जर (type == SNT_REPICK)
		परत;

	stop->se.exec_start = rq_clock_task(rq);
}

स्थिर संरचना task_struct *pick_task_stop(संरचना rq *rq, संरचना rq_flags *rf)
{
	जर (!sched_stop_runnable(rq))
		परत रिक्त;

	परत rq->stop;
}

स्थिर शून्य
enqueue_task_stop(संरचना rq *rq, संरचना task_struct *p, पूर्णांक flags)
{
	add_nr_running(rq, 1);
}

स्थिर bool
dequeue_task_stop(संरचना rq *rq, संरचना task_struct *p, पूर्णांक flags)
{
	sub_nr_running(rq, 1);
	परत true;
}

स्थिर शून्य yield_task_stop(संरचना rq *rq)
{
	BUG(); /* the stop task should never yield, its pointless. */
}

स्थिर शून्य put_prev_task_stop(संरचना rq *rq, संरचना task_struct *prev, संरचना task_struct *next)
{
	update_curr_common(rq);
}

/*
 * scheduler tick hitting a task of our scheduling class.
 *
 * NOTE: This function can be called remotely by the tick offload that
 * goes along full dynticks. Therefore no local assumption can be made
 * and everything must be accessed through the @rq and @curr passed in
 * parameters.
 */
स्थिर शून्य task_tick_stop(संरचना rq *rq, संरचना task_struct *curr, पूर्णांक queued)
{
}

स्थिर शून्य switching_to_stop(संरचना rq *rq, संरचना task_struct *p)
{
	BUG(); /* its impossible to change to this class */
}

स्थिर शून्य
prio_changed_stop(संरचना rq *rq, संरचना task_struct *p, u64 oldprio)
{
	जर (p->prio == oldprio)
		परत;

	BUG(); /* how!?, what priority? */
}

स्थिर शून्य update_curr_stop(संरचना rq *rq)
{
}

/*
 * Simple, special scheduling class for the per-CPU stop tasks:
 */
DEFINE_SCHED_CLASS(stop) = {
	.enqueue_task		= enqueue_task_stop,
	.dequeue_task		= dequeue_task_stop,
	.yield_task		= yield_task_stop,

	.wakeup_preempt		= wakeup_preempt_stop,

	.pick_task		= pick_task_stop,
	.put_prev_task		= put_prev_task_stop,
	.set_next_task          = set_next_task_stop,

	.balance		= balance_stop,
	.select_task_rq		= select_task_rq_stop,
	.set_cpus_allowed	= set_cpus_allowed_common,

	.task_tick		= task_tick_stop,

	.prio_changed		= prio_changed_stop,
	.switching_to		= switching_to_stop,
	.update_curr		= update_curr_stop,
};
