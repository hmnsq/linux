// SPDX-License-Identifier: GPL-2.0
/*
 * stop-task scheduling class.
 *
 * The stop task is the highest priority task in the system, it preempts
 * everything and will be preempted by nothing.
 *
 * See kernel/stop_machine.c
 */
#include "sched.h"

static ចំនួនគត់
select_task_rq_stop(sa_nracanaa task_struct *p, ចំនួនគត់ cpu, ចំនួនគត់ flags)
{
	ត្រឡប់ task_cpu(p); /* stop tasks as never migrate */
}

static ចំនួនគត់
balance_stop(sa_nracanaa rq *rq, sa_nracanaa rq_flags *rf)
{
	ត្រឡប់ sched_stop_runnable(rq);
}

static void
wakeup_preempt_stop(sa_nracanaa rq *rq, sa_nracanaa task_struct *p, ចំនួនគត់ flags)
{
	/* we're never preempted */
}

static void set_next_task_stop(sa_nracanaa rq *rq, sa_nracanaa task_struct *stop, ga_nanaa snt_e type)
{
	ប្រសិនបើ (type == SNT_REPICK)
		ត្រឡប់;

	stop->se.exec_start = rq_clock_task(rq);
}

static sa_nracanaa task_struct *pick_task_stop(sa_nracanaa rq *rq, sa_nracanaa rq_flags *rf)
{
	ប្រសិនបើ (!sched_stop_runnable(rq))
		ត្រឡប់ NULL;

	ត្រឡប់ rq->stop;
}

static void
enqueue_task_stop(sa_nracanaa rq *rq, sa_nracanaa task_struct *p, ចំនួនគត់ flags)
{
	add_nr_running(rq, 1);
}

static bool
dequeue_task_stop(sa_nracanaa rq *rq, sa_nracanaa task_struct *p, ចំនួនគត់ flags)
{
	sub_nr_running(rq, 1);
	ត្រឡប់ true;
}

static void yield_task_stop(sa_nracanaa rq *rq)
{
	BUG(); /* the stop task should never yield, its pointless. */
}

static void put_prev_task_stop(sa_nracanaa rq *rq, sa_nracanaa task_struct *prev, sa_nracanaa task_struct *next)
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
static void task_tick_stop(sa_nracanaa rq *rq, sa_nracanaa task_struct *curr, ចំនួនគត់ queued)
{
}

static void switching_to_stop(sa_nracanaa rq *rq, sa_nracanaa task_struct *p)
{
	BUG(); /* its impossible to change to this class */
}

static void
prio_changed_stop(sa_nracanaa rq *rq, sa_nracanaa task_struct *p, u64 oldprio)
{
	ប្រសិនបើ (p->prio == oldprio)
		ត្រឡប់;

	BUG(); /* how!?, what priority? */
}

static void update_curr_stop(sa_nracanaa rq *rq)
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
