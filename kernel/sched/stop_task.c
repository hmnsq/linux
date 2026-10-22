// SPDX-License-Identifier: GPL-2.0
/*
 * stop-task scheduling class.
 *
 * The stop task is the highest priority task in the system, it preempts
 * everything and will be preempted by nothing.
 *
 * See kernel/stop_machine.c
 */
#અન્તર્ભાવ "sched.h"

સ્થિર પૂર્ણાંક
select_task_rq_stop(સંરચના task_struct *p, પૂર્ણાંક cpu, પૂર્ણાંક flags)
{
	પાછા task_cpu(p); /* stop tasks as never migrate */
}

સ્થિર પૂર્ણાંક
balance_stop(સંરચના rq *rq, સંરચના rq_flags *rf)
{
	પાછા sched_stop_runnable(rq);
}

સ્થિર શૂન્ય
wakeup_preempt_stop(સંરચના rq *rq, સંરચના task_struct *p, પૂર્ણાંક flags)
{
	/* we're never preempted */
}

સ્થિર શૂન્ય set_next_task_stop(સંરચના rq *rq, સંરચના task_struct *stop, ગણના snt_e type)
{
	જો (type == SNT_REPICK)
		પાછા;

	stop->se.exec_start = rq_clock_task(rq);
}

સ્થિર સંરચના task_struct *pick_task_stop(સંરચના rq *rq, સંરચના rq_flags *rf)
{
	જો (!sched_stop_runnable(rq))
		પાછા ખાલી;

	પાછા rq->stop;
}

સ્થિર શૂન્ય
enqueue_task_stop(સંરચના rq *rq, સંરચના task_struct *p, પૂર્ણાંક flags)
{
	add_nr_running(rq, 1);
}

સ્થિર bool
dequeue_task_stop(સંરચના rq *rq, સંરચના task_struct *p, પૂર્ણાંક flags)
{
	sub_nr_running(rq, 1);
	પાછા true;
}

સ્થિર શૂન્ય yield_task_stop(સંરચના rq *rq)
{
	BUG(); /* the stop task should never yield, its pointless. */
}

સ્થિર શૂન્ય put_prev_task_stop(સંરચના rq *rq, સંરચના task_struct *prev, સંરચના task_struct *next)
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
સ્થિર શૂન્ય task_tick_stop(સંરચના rq *rq, સંરચના task_struct *curr, પૂર્ણાંક queued)
{
}

સ્થિર શૂન્ય switching_to_stop(સંરચના rq *rq, સંરચના task_struct *p)
{
	BUG(); /* its impossible to change to this class */
}

સ્થિર શૂન્ય
prio_changed_stop(સંરચના rq *rq, સંરચના task_struct *p, u64 oldprio)
{
	જો (p->prio == oldprio)
		પાછા;

	BUG(); /* how!?, what priority? */
}

સ્થિર શૂન્ય update_curr_stop(સંરચના rq *rq)
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
