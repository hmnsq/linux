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

স্থির পূর্ণসংখ্যা
select_task_rq_stop(কাঠামো task_struct *p, পূর্ণসংখ্যা cpu, পূর্ণসংখ্যা flags)
{
	ফেরত task_cpu(p); /* stop tasks as never migrate */
}

স্থির পূর্ণসংখ্যা
balance_stop(কাঠামো rq *rq, কাঠামো rq_flags *rf)
{
	ফেরত sched_stop_runnable(rq);
}

স্থির শূন্য
wakeup_preempt_stop(কাঠামো rq *rq, কাঠামো task_struct *p, পূর্ণসংখ্যা flags)
{
	/* we're never preempted */
}

স্থির শূন্য set_পরবর্তী_task_stop(কাঠামো rq *rq, কাঠামো task_struct *stop, গণনা snt_e type)
{
	যদি (type == SNT_REPICK)
		ফেরত;

	stop->se.exec_start = rq_clock_task(rq);
}

স্থির কাঠামো task_struct *pick_task_stop(কাঠামো rq *rq, কাঠামো rq_flags *rf)
{
	যদি (!sched_stop_runnable(rq))
		ফেরত ফাঁকা;

	ফেরত rq->stop;
}

স্থির শূন্য
enqueue_task_stop(কাঠামো rq *rq, কাঠামো task_struct *p, পূর্ণসংখ্যা flags)
{
	add_nr_running(rq, 1);
}

স্থির bool
dequeue_task_stop(কাঠামো rq *rq, কাঠামো task_struct *p, পূর্ণসংখ্যা flags)
{
	sub_nr_running(rq, 1);
	ফেরত true;
}

স্থির শূন্য yield_task_stop(কাঠামো rq *rq)
{
	BUG(); /* the stop task should never yield, its pointless. */
}

স্থির শূন্য put_পূর্ব_task_stop(কাঠামো rq *rq, কাঠামো task_struct *পূর্ব, কাঠামো task_struct *পরবর্তী)
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
স্থির শূন্য task_tick_stop(কাঠামো rq *rq, কাঠামো task_struct *curr, পূর্ণসংখ্যা queued)
{
}

স্থির শূন্য switching_to_stop(কাঠামো rq *rq, কাঠামো task_struct *p)
{
	BUG(); /* its impossible to change to this class */
}

স্থির শূন্য
prio_changed_stop(কাঠামো rq *rq, কাঠামো task_struct *p, u64 oldprio)
{
	যদি (p->prio == oldprio)
		ফেরত;

	BUG(); /* how!?, what priority? */
}

স্থির শূন্য update_curr_stop(কাঠামো rq *rq)
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
	.put_পূর্ব_task		= put_পূর্ব_task_stop,
	.set_পরবর্তী_task          = set_পরবর্তী_task_stop,

	.balance		= balance_stop,
	.select_task_rq		= select_task_rq_stop,
	.set_cpus_allowed	= set_cpus_allowed_common,

	.task_tick		= task_tick_stop,

	.prio_changed		= prio_changed_stop,
	.switching_to		= switching_to_stop,
	.update_curr		= update_curr_stop,
};
