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

स्थिर पूर्णाङ्क
select_task_rq_stop(संरचना task_struct *p, पूर्णाङ्क cpu, पूर्णाङ्क flags)
{
	फर्काऊ task_cpu(p); /* stop tasks as never migrate */
}

स्थिर पूर्णाङ्क
balance_stop(संरचना rq *rq, संरचना rq_flags *rf)
{
	फर्काऊ sched_stop_runnable(rq);
}

स्थिर शून्य
wakeup_preempt_stop(संरचना rq *rq, संरचना task_struct *p, पूर्णाङ्क flags)
{
	/* we're never preempted */
}

स्थिर शून्य set_next_task_stop(संरचना rq *rq, संरचना task_struct *stop, गणना snt_e type)
{
	यदि (type == SNT_REPICK)
		फर्काऊ;

	stop->se.exec_start = rq_clock_task(rq);
}

स्थिर संरचना task_struct *pick_task_stop(संरचना rq *rq, संरचना rq_flags *rf)
{
	यदि (!sched_stop_runnable(rq))
		फर्काऊ रिक्त;

	फर्काऊ rq->stop;
}

स्थिर शून्य
enqueue_task_stop(संरचना rq *rq, संरचना task_struct *p, पूर्णाङ्क flags)
{
	add_nr_running(rq, 1);
}

स्थिर bool
dequeue_task_stop(संरचना rq *rq, संरचना task_struct *p, पूर्णाङ्क flags)
{
	sub_nr_running(rq, 1);
	फर्काऊ true;
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
स्थिर शून्य task_tick_stop(संरचना rq *rq, संरचना task_struct *curr, पूर्णाङ्क queued)
{
}

स्थिर शून्य switching_to_stop(संरचना rq *rq, संरचना task_struct *p)
{
	BUG(); /* its impossible to change to this class */
}

स्थिर शून्य
prio_changed_stop(संरचना rq *rq, संरचना task_struct *p, u64 oldprio)
{
	यदि (p->prio == oldprio)
		फर्काऊ;

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
