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

ساکن صحیح
select_task_rq_stop(ساخت task_struct *p, صحیح cpu, صحیح flags)
{
	واپس task_cpu(p); /* stop tasks as never migrate */
}

ساکن صحیح
balance_stop(ساخت rq *rq, ساخت rq_flags *rf)
{
	واپس sched_stop_runnable(rq);
}

ساکن خالی
wakeup_preempt_stop(ساخت rq *rq, ساخت task_struct *p, صحیح flags)
{
	/* we're never preempted */
}

ساکن خالی سیٹ_اگلا_task_stop(ساخت rq *rq, ساخت task_struct *stop, شمار snt_e type)
{
	اگر (type == SNT_REPICK)
		واپس;

	stop->se.exec_آغاز = rq_clock_task(rq);
}

ساکن ساخت task_struct *pick_task_stop(ساخت rq *rq, ساخت rq_flags *rf)
{
	اگر (!sched_stop_runnable(rq))
		واپس تہی;

	واپس rq->stop;
}

ساکن خالی
قطار_میں_ڈالو_task_stop(ساخت rq *rq, ساخت task_struct *p, صحیح flags)
{
	جوڑو_nr_running(rq, 1);
}

ساکن bool
قطار_سے_نکالو_task_stop(ساخت rq *rq, ساخت task_struct *p, صحیح flags)
{
	sub_nr_running(rq, 1);
	واپس true;
}

ساکن خالی yield_task_stop(ساخت rq *rq)
{
	BUG(); /* the stop task should never yield, its pointless. */
}

ساکن خالی put_پچھلا_task_stop(ساخت rq *rq, ساخت task_struct *پچھلا, ساخت task_struct *اگلا)
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
ساکن خالی task_tick_stop(ساخت rq *rq, ساخت task_struct *curr, صحیح queued)
{
}

ساکن خالی switching_مقصد_stop(ساخت rq *rq, ساخت task_struct *p)
{
	BUG(); /* its impossible to change to this class */
}

ساکن خالی
prio_بدلا_stop(ساخت rq *rq, ساخت task_struct *p, u64 oldprio)
{
	اگر (p->prio == oldprio)
		واپس;

	BUG(); /* how!?, what priority? */
}

ساکن خالی update_curr_stop(ساخت rq *rq)
{
}

/*
 * Simple, special scheduling class for the per-CPU stop tasks:
 */
DEFINE_SCHED_CLASS(stop) = {
	.قطار_میں_ڈالو_task		= قطار_میں_ڈالو_task_stop,
	.قطار_سے_نکالو_task		= قطار_سے_نکالو_task_stop,
	.yield_task		= yield_task_stop,

	.wakeup_preempt		= wakeup_preempt_stop,

	.pick_task		= pick_task_stop,
	.put_پچھلا_task		= put_پچھلا_task_stop,
	.سیٹ_اگلا_task          = سیٹ_اگلا_task_stop,

	.balance		= balance_stop,
	.select_task_rq		= select_task_rq_stop,
	.سیٹ_cpus_allowed	= سیٹ_cpus_allowed_common,

	.task_tick		= task_tick_stop,

	.prio_بدلا		= prio_بدلا_stop,
	.switching_مقصد		= switching_مقصد_stop,
	.update_curr		= update_curr_stop,
};
