// SPDX-License-Identifier: GPL-2.0
/*
 * stop-task scheduling class.
 *
 * The stop task is the highest priority task in the system, it preempts
 * everything and will be preempted by nothing.
 *
 * See kernel/stop_machine.c
 */
#شامل "sched.h"

ایستا صحیح
select_task_rq_stop(ساختار task_struct *p, صحیح cpu, صحیح flags)
{
	بازگشت task_cpu(p); /* stop tasks as never migrate */
}

ایستا صحیح
balance_stop(ساختار rq *rq, ساختار rq_flags *rf)
{
	بازگشت sched_stop_runnable(rq);
}

ایستا تهی
wakeup_preempt_stop(ساختار rq *rq, ساختار task_struct *p, صحیح flags)
{
	/* we're never preempted */
}

ایستا تهی set_next_task_stop(ساختار rq *rq, ساختار task_struct *stop, شمارش snt_e type)
{
	اگر (type == SNT_REPICK)
		بازگشت;

	stop->se.exec_start = rq_clock_task(rq);
}

ایستا ساختار task_struct *pick_task_stop(ساختار rq *rq, ساختار rq_flags *rf)
{
	اگر (!sched_stop_runnable(rq))
		بازگشت پوچ;

	بازگشت rq->stop;
}

ایستا تهی
enqueue_task_stop(ساختار rq *rq, ساختار task_struct *p, صحیح flags)
{
	add_nr_running(rq, 1);
}

ایستا bool
dequeue_task_stop(ساختار rq *rq, ساختار task_struct *p, صحیح flags)
{
	sub_nr_running(rq, 1);
	بازگشت true;
}

ایستا تهی yield_task_stop(ساختار rq *rq)
{
	BUG(); /* the stop task should never yield, its pointless. */
}

ایستا تهی put_prev_task_stop(ساختار rq *rq, ساختار task_struct *prev, ساختار task_struct *next)
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
ایستا تهی task_tick_stop(ساختار rq *rq, ساختار task_struct *curr, صحیح queued)
{
}

ایستا تهی switching_to_stop(ساختار rq *rq, ساختار task_struct *p)
{
	BUG(); /* its impossible to change to this class */
}

ایستا تهی
prio_changed_stop(ساختار rq *rq, ساختار task_struct *p, u64 oldprio)
{
	اگر (p->prio == oldprio)
		بازگشت;

	BUG(); /* how!?, what priority? */
}

ایستا تهی update_curr_stop(ساختار rq *rq)
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
