// SPDX-License-Identifier: GPL-2.0
/*
 * stop-task scheduling class.
 *
 * The stop task is the highest priority task in the system, it preempts
 * everything and will be preempted by nothing.
 *
 * See kernel/stop_machine.c
 */
#ضمن "sched.h"

ساكن صحيح
select_task_rq_stop(بنية task_struct *p, صحيح cpu, صحيح flags)
{
	أرجع task_cpu(p); /* stop tasks as never migrate */
}

ساكن صحيح
balance_stop(بنية rq *rq, بنية rq_flags *rf)
{
	أرجع sched_stop_runnable(rq);
}

ساكن فراغ
wakeup_preempt_stop(بنية rq *rq, بنية task_struct *p, صحيح flags)
{
	/* we're never preempted */
}

ساكن فراغ سیٹ_اگلا_task_stop(بنية rq *rq, بنية task_struct *stop, تعداد snt_e type)
{
	إذا (type == SNT_REPICK)
		أرجع;

	stop->se.exec_شروع = rq_clock_task(rq);
}

ساكن بنية task_struct *pick_task_stop(بنية rq *rq, بنية rq_flags *rf)
{
	إذا (!sched_stop_runnable(rq))
		أرجع عدم;

	أرجع rq->stop;
}

ساكن فراغ
قطار_وچ_پاؤ_task_stop(بنية rq *rq, بنية task_struct *p, صحيح flags)
{
	جوڑو_nr_running(rq, 1);
}

ساكن bool
قطار_توں_کڈھو_task_stop(بنية rq *rq, بنية task_struct *p, صحيح flags)
{
	sub_nr_running(rq, 1);
	أرجع true;
}

ساكن فراغ yield_task_stop(بنية rq *rq)
{
	BUG(); /* the stop task should never yield, its pointless. */
}

ساكن فراغ put_prev_task_stop(بنية rq *rq, بنية task_struct *prev, بنية task_struct *اگلا)
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
ساكن فراغ task_tick_stop(بنية rq *rq, بنية task_struct *curr, صحيح queued)
{
}

ساكن فراغ switching_منزل_stop(بنية rq *rq, بنية task_struct *p)
{
	BUG(); /* its impossible to change to this class */
}

ساكن فراغ
prio_بدلیا_stop(بنية rq *rq, بنية task_struct *p, u64 oldprio)
{
	إذا (p->prio == oldprio)
		أرجع;

	BUG(); /* how!?, what priority? */
}

ساكن فراغ update_curr_stop(بنية rq *rq)
{
}

/*
 * Simple, special scheduling class for the per-CPU stop tasks:
 */
DEFINE_SCHED_CLASS(stop) = {
	.قطار_وچ_پاؤ_task		= قطار_وچ_پاؤ_task_stop,
	.قطار_توں_کڈھو_task		= قطار_توں_کڈھو_task_stop,
	.yield_task		= yield_task_stop,

	.wakeup_preempt		= wakeup_preempt_stop,

	.pick_task		= pick_task_stop,
	.put_prev_task		= put_prev_task_stop,
	.سیٹ_اگلا_task          = سیٹ_اگلا_task_stop,

	.balance		= balance_stop,
	.select_task_rq		= select_task_rq_stop,
	.سیٹ_cpus_allowed	= سیٹ_cpus_allowed_common,

	.task_tick		= task_tick_stop,

	.prio_بدلیا		= prio_بدلیا_stop,
	.switching_منزل		= switching_منزل_stop,
	.update_curr		= update_curr_stop,
};
