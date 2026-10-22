// SPDX-License-Identifier: GPL-2.0
/*
 * stop-task scheduling class.
 *
 * The stop task is the highest priority task in the system, it preempts
 * everything and will be preempted by nothing.
 *
 * See kernel/stop_machine.c
 */
#ಅನ್ತರ್ಭಾವ "sched.h"

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
select_task_rq_stop(ಸಂರಚನಾ task_struct *p, ಪೂರ್ಣಾಙ್ಕ cpu, ಪೂರ್ಣಾಙ್ಕ flags)
{
	ಪ್ರತ್ಯಾಗಮ task_cpu(p); /* stop tasks as never migrate */
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
balance_stop(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ rq_flags *rf)
{
	ಪ್ರತ್ಯಾಗಮ sched_stop_runnable(rq);
}

ಸ್ಥಿರ ಶೂನ್ಯ
wakeup_preempt_stop(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p, ಪೂರ್ಣಾಙ್ಕ flags)
{
	/* we're never preempted */
}

ಸ್ಥಿರ ಶೂನ್ಯ set_next_task_stop(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *stop, ಗಣನಾ snt_e type)
{
	ಯದಿ (type == SNT_REPICK)
		ಪ್ರತ್ಯಾಗಮ;

	stop->se.exec_start = rq_clock_task(rq);
}

ಸ್ಥಿರ ಸಂರಚನಾ task_struct *pick_task_stop(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ rq_flags *rf)
{
	ಯದಿ (!sched_stop_runnable(rq))
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

	ಪ್ರತ್ಯಾಗಮ rq->stop;
}

ಸ್ಥಿರ ಶೂನ್ಯ
enqueue_task_stop(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p, ಪೂರ್ಣಾಙ್ಕ flags)
{
	add_nr_running(rq, 1);
}

ಸ್ಥಿರ bool
dequeue_task_stop(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p, ಪೂರ್ಣಾಙ್ಕ flags)
{
	sub_nr_running(rq, 1);
	ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ ಶೂನ್ಯ yield_task_stop(ಸಂರಚನಾ rq *rq)
{
	BUG(); /* the stop task should never yield, its pointless. */
}

ಸ್ಥಿರ ಶೂನ್ಯ put_prev_task_stop(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *prev, ಸಂರಚನಾ task_struct *next)
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
ಸ್ಥಿರ ಶೂನ್ಯ task_tick_stop(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *curr, ಪೂರ್ಣಾಙ್ಕ queued)
{
}

ಸ್ಥಿರ ಶೂನ್ಯ switching_to_stop(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p)
{
	BUG(); /* its impossible to change to this class */
}

ಸ್ಥಿರ ಶೂನ್ಯ
prio_changed_stop(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p, u64 oldprio)
{
	ಯದಿ (p->prio == oldprio)
		ಪ್ರತ್ಯಾಗಮ;

	BUG(); /* how!?, what priority? */
}

ಸ್ಥಿರ ಶೂನ್ಯ update_curr_stop(ಸಂರಚನಾ rq *rq)
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
