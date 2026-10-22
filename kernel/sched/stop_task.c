// SPDX-License-Identifier: GPL-2.0
/*
 * stop-task scheduling class.
 *
 * The stop task is the highest priority task in the system, it preempts
 * everything and will be preempted by nothing.
 *
 * See kernel/stop_machine.c
 */
#അന്തര്ഭാവ "sched.h"

സ്ഥിതം പൂർണ്ണസംഖ്യ
select_task_rq_stop(ഘടന task_struct *p, പൂർണ്ണസംഖ്യ cpu, പൂർണ്ണസംഖ്യ flags)
{
	തിരികെ task_cpu(p); /* stop tasks as never migrate */
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
balance_stop(ഘടന rq *rq, ഘടന rq_flags *rf)
{
	തിരികെ sched_stop_runnable(rq);
}

സ്ഥിതം ശൂന്യം
wakeup_preempt_stop(ഘടന rq *rq, ഘടന task_struct *p, പൂർണ്ണസംഖ്യ flags)
{
	/* we're never preempted */
}

സ്ഥിതം ശൂന്യം set_next_task_stop(ഘടന rq *rq, ഘടന task_struct *stop, എണ്ണൽ snt_e type)
{
	എങ്കിൽ (type == SNT_REPICK)
		തിരികെ;

	stop->se.exec_start = rq_clock_task(rq);
}

സ്ഥിതം ഘടന task_struct *pick_task_stop(ഘടന rq *rq, ഘടന rq_flags *rf)
{
	എങ്കിൽ (!sched_stop_runnable(rq))
		തിരികെ ശൂന്യ;

	തിരികെ rq->stop;
}

സ്ഥിതം ശൂന്യം
enqueue_task_stop(ഘടന rq *rq, ഘടന task_struct *p, പൂർണ്ണസംഖ്യ flags)
{
	add_nr_running(rq, 1);
}

സ്ഥിതം bool
dequeue_task_stop(ഘടന rq *rq, ഘടന task_struct *p, പൂർണ്ണസംഖ്യ flags)
{
	sub_nr_running(rq, 1);
	തിരികെ true;
}

സ്ഥിതം ശൂന്യം yield_task_stop(ഘടന rq *rq)
{
	BUG(); /* the stop task should never yield, its pointless. */
}

സ്ഥിതം ശൂന്യം put_prev_task_stop(ഘടന rq *rq, ഘടന task_struct *prev, ഘടന task_struct *next)
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
സ്ഥിതം ശൂന്യം task_tick_stop(ഘടന rq *rq, ഘടന task_struct *curr, പൂർണ്ണസംഖ്യ queued)
{
}

സ്ഥിതം ശൂന്യം switching_to_stop(ഘടന rq *rq, ഘടന task_struct *p)
{
	BUG(); /* its impossible to change to this class */
}

സ്ഥിതം ശൂന്യം
prio_changed_stop(ഘടന rq *rq, ഘടന task_struct *p, u64 oldprio)
{
	എങ്കിൽ (p->prio == oldprio)
		തിരികെ;

	BUG(); /* how!?, what priority? */
}

സ്ഥിതം ശൂന്യം update_curr_stop(ഘടന rq *rq)
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
