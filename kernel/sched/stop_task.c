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

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
select_task_rq_stop(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ cpu, ಪೂರ್ಣಾಂಕ flags)
{
	ಹಿಂತಿರುಗಿ task_cpu(p); /* stop tasks as never migrate */
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
balance_stop(ರಚನೆ rq *rq, ರಚನೆ rq_flags *rf)
{
	ಹಿಂತಿರುಗಿ sched_stop_runnable(rq);
}

ಸ್ಥಿರ ಶೂನ್ಯ
wakeup_preempt_stop(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ flags)
{
	/* we're never preempted */
}

ಸ್ಥಿರ ಶೂನ್ಯ ಗಣ_ಮುಂದಿನ_task_stop(ರಚನೆ rq *rq, ರಚನೆ task_struct *stop, ಎಣಿಕೆ snt_e type)
{
	ಆದರೆ (type == SNT_REPICK)
		ಹಿಂತಿರುಗಿ;

	stop->se.exec_ಆರಂಭ = rq_clock_task(rq);
}

ಸ್ಥಿರ ರಚನೆ task_struct *pick_task_stop(ರಚನೆ rq *rq, ರಚನೆ rq_flags *rf)
{
	ಆದರೆ (!sched_stop_runnable(rq))
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	ಹಿಂತಿರುಗಿ rq->stop;
}

ಸ್ಥಿರ ಶೂನ್ಯ
ಸರತಿಗೆ_ಸೇರಿಸು_task_stop(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ flags)
{
	ಕೂಡಿಸು_nr_running(rq, 1);
}

ಸ್ಥಿರ bool
ಸರತಿಯಿಂದ_ತೆಗೆ_task_stop(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ flags)
{
	sub_nr_running(rq, 1);
	ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ ಶೂನ್ಯ yield_task_stop(ರಚನೆ rq *rq)
{
	BUG(); /* the stop task should never yield, its pointless. */
}

ಸ್ಥಿರ ಶೂನ್ಯ put_prev_task_stop(ರಚನೆ rq *rq, ರಚನೆ task_struct *prev, ರಚನೆ task_struct *ಮುಂದಿನ)
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
ಸ್ಥಿರ ಶೂನ್ಯ task_tick_stop(ರಚನೆ rq *rq, ರಚನೆ task_struct *curr, ಪೂರ್ಣಾಂಕ queued)
{
}

ಸ್ಥಿರ ಶೂನ್ಯ switching_ಗಮ್ಯ_stop(ರಚನೆ rq *rq, ರಚನೆ task_struct *p)
{
	BUG(); /* its impossible to change to this class */
}

ಸ್ಥಿರ ಶೂನ್ಯ
prio_ಬದಲಾಯಿತು_stop(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, u64 oldprio)
{
	ಆದರೆ (p->prio == oldprio)
		ಹಿಂತಿರುಗಿ;

	BUG(); /* how!?, what priority? */
}

ಸ್ಥಿರ ಶೂನ್ಯ update_curr_stop(ರಚನೆ rq *rq)
{
}

/*
 * Simple, special scheduling class for the per-CPU stop tasks:
 */
DEFINE_SCHED_CLASS(stop) = {
	.ಸರತಿಗೆ_ಸೇರಿಸು_task		= ಸರತಿಗೆ_ಸೇರಿಸು_task_stop,
	.ಸರತಿಯಿಂದ_ತೆಗೆ_task		= ಸರತಿಯಿಂದ_ತೆಗೆ_task_stop,
	.yield_task		= yield_task_stop,

	.wakeup_preempt		= wakeup_preempt_stop,

	.pick_task		= pick_task_stop,
	.put_prev_task		= put_prev_task_stop,
	.ಗಣ_ಮುಂದಿನ_task          = ಗಣ_ಮುಂದಿನ_task_stop,

	.balance		= balance_stop,
	.select_task_rq		= select_task_rq_stop,
	.ಗಣ_cpus_allowed	= ಗಣ_cpus_allowed_common,

	.task_tick		= task_tick_stop,

	.prio_ಬದಲಾಯಿತು		= prio_ಬದಲಾಯಿತು_stop,
	.switching_ಗಮ್ಯ		= switching_ಗಮ್ಯ_stop,
	.update_curr		= update_curr_stop,
};
