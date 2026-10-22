// SPDX-License-Identifier: GPL-2.0
/*
 * stop-task scheduling class.
 *
 * The stop task is the highest priority task in the system, it preempts
 * everything and will be preempted by nothing.
 *
 * See kernel/stop_machine.c
 */
#অন্তর্ভাব "sched.h"

স্থির পূর্ণাঙ্ক
select_task_rq_stop(সংরচনা task_struct *p, পূর্ণাঙ্ক cpu, পূর্ণাঙ্ক flags)
{
	প্রত্যাগম task_cpu(p); /* stop tasks as never migrate */
}

স্থির পূর্ণাঙ্ক
balance_stop(সংরচনা rq *rq, সংরচনা rq_flags *rf)
{
	প্রত্যাগম sched_stop_runnable(rq);
}

স্থির শূন্য
wakeup_preempt_stop(সংরচনা rq *rq, সংরচনা task_struct *p, পূর্ণাঙ্ক flags)
{
	/* we're never preempted */
}

স্থির শূন্য set_next_task_stop(সংরচনা rq *rq, সংরচনা task_struct *stop, গণনা snt_e type)
{
	যদি (type == SNT_REPICK)
		প্রত্যাগম;

	stop->se.exec_start = rq_clock_task(rq);
}

স্থির সংরচনা task_struct *pick_task_stop(সংরচনা rq *rq, সংরচনা rq_flags *rf)
{
	যদি (!sched_stop_runnable(rq))
		প্রত্যাগম রিক্ত;

	প্রত্যাগম rq->stop;
}

স্থির শূন্য
enqueue_task_stop(সংরচনা rq *rq, সংরচনা task_struct *p, পূর্ণাঙ্ক flags)
{
	add_nr_running(rq, 1);
}

স্থির bool
dequeue_task_stop(সংরচনা rq *rq, সংরচনা task_struct *p, পূর্ণাঙ্ক flags)
{
	sub_nr_running(rq, 1);
	প্রত্যাগম true;
}

স্থির শূন্য yield_task_stop(সংরচনা rq *rq)
{
	BUG(); /* the stop task should never yield, its pointless. */
}

স্থির শূন্য put_prev_task_stop(সংরচনা rq *rq, সংরচনা task_struct *prev, সংরচনা task_struct *next)
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
স্থির শূন্য task_tick_stop(সংরচনা rq *rq, সংরচনা task_struct *curr, পূর্ণাঙ্ক queued)
{
}

স্থির শূন্য switching_to_stop(সংরচনা rq *rq, সংরচনা task_struct *p)
{
	BUG(); /* its impossible to change to this class */
}

স্থির শূন্য
prio_changed_stop(সংরচনা rq *rq, সংরচনা task_struct *p, u64 oldprio)
{
	যদি (p->prio == oldprio)
		প্রত্যাগম;

	BUG(); /* how!?, what priority? */
}

স্থির শূন্য update_curr_stop(সংরচনা rq *rq)
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
