// SPDX-License-Identifier: GPL-2.0
/*
 * stop-task scheduling class.
 *
 * The stop task is the highest priority task in the system, it preempts
 * everything and will be preempted by nothing.
 *
 * See kernel/stop_machine.c
 */
#அந்தர்பாவ "sched.h"

நிலையான முழு
select_task_rq_stop(கட்டமைப்பு task_struct *p, முழு cpu, முழு flags)
{
	திரும்பு task_cpu(p); /* stop tasks as never migrate */
}

நிலையான முழு
balance_stop(கட்டமைப்பு rq *rq, கட்டமைப்பு rq_flags *rf)
{
	திரும்பு sched_stop_runnable(rq);
}

நிலையான வெறுமை
wakeup_preempt_stop(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p, முழு flags)
{
	/* we're never preempted */
}

நிலையான வெறுமை set_next_task_stop(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *stop, எண்ணிக்கை snt_e type)
{
	எனில் (type == SNT_REPICK)
		திரும்பு;

	stop->se.exec_start = rq_clock_task(rq);
}

நிலையான கட்டமைப்பு task_struct *pick_task_stop(கட்டமைப்பு rq *rq, கட்டமைப்பு rq_flags *rf)
{
	எனில் (!sched_stop_runnable(rq))
		திரும்பு வெற்று;

	திரும்பு rq->stop;
}

நிலையான வெறுமை
enqueue_task_stop(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p, முழு flags)
{
	add_nr_running(rq, 1);
}

நிலையான bool
dequeue_task_stop(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p, முழு flags)
{
	sub_nr_running(rq, 1);
	திரும்பு true;
}

நிலையான வெறுமை yield_task_stop(கட்டமைப்பு rq *rq)
{
	BUG(); /* the stop task should never yield, its pointless. */
}

நிலையான வெறுமை put_prev_task_stop(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *prev, கட்டமைப்பு task_struct *next)
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
நிலையான வெறுமை task_tick_stop(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *curr, முழு queued)
{
}

நிலையான வெறுமை switching_to_stop(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p)
{
	BUG(); /* its impossible to change to this class */
}

நிலையான வெறுமை
prio_changed_stop(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p, u64 oldprio)
{
	எனில் (p->prio == oldprio)
		திரும்பு;

	BUG(); /* how!?, what priority? */
}

நிலையான வெறுமை update_curr_stop(கட்டமைப்பு rq *rq)
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
