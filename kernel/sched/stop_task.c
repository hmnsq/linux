// SPDX-License-Identifier: GPL-2.0
/*
 * stop-task scheduling class.
 *
 * The stop task is the highest priority task in the system, it preempts
 * everything and will be preempted by nothing.
 *
 * See kernel/stop_machine.c
 */
#ଅନ୍ତର୍ଭାଵ "sched.h"

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
select_task_rq_stop(ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ଫେରନ୍ତୁ task_cpu(p); /* stop tasks as never migrate */
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
balance_stop(ସଂରଚନା rq *rq, ସଂରଚନା rq_flags *rf)
{
	ଫେରନ୍ତୁ sched_stop_runnable(rq);
}

ସ୍ଥିର ଶୂନ୍ୟ
wakeup_preempt_stop(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	/* we're never preempted */
}

ସ୍ଥିର ଶୂନ୍ୟ set_next_task_stop(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *stop, ଗଣନା snt_e type)
{
	ଯଦି (type == SNT_REPICK)
		ଫେରନ୍ତୁ;

	stop->se.exec_start = rq_clock_task(rq);
}

ସ୍ଥିର ସଂରଚନା task_struct *pick_task_stop(ସଂରଚନା rq *rq, ସଂରଚନା rq_flags *rf)
{
	ଯଦି (!sched_stop_runnable(rq))
		ଫେରନ୍ତୁ ଖାଲି;

	ଫେରନ୍ତୁ rq->stop;
}

ସ୍ଥିର ଶୂନ୍ୟ
enqueue_task_stop(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	add_nr_running(rq, 1);
}

ସ୍ଥିର bool
dequeue_task_stop(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	sub_nr_running(rq, 1);
	ଫେରନ୍ତୁ true;
}

ସ୍ଥିର ଶୂନ୍ୟ yield_task_stop(ସଂରଚନା rq *rq)
{
	BUG(); /* the stop task should never yield, its pointless. */
}

ସ୍ଥିର ଶୂନ୍ୟ put_prev_task_stop(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *prev, ସଂରଚନା task_struct *next)
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
ସ୍ଥିର ଶୂନ୍ୟ task_tick_stop(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *curr, ପୂର୍ଣ୍ଣସଂଖ୍ୟା queued)
{
}

ସ୍ଥିର ଶୂନ୍ୟ switching_to_stop(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p)
{
	BUG(); /* its impossible to change to this class */
}

ସ୍ଥିର ଶୂନ୍ୟ
prio_changed_stop(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p, u64 oldprio)
{
	ଯଦି (p->prio == oldprio)
		ଫେରନ୍ତୁ;

	BUG(); /* how!?, what priority? */
}

ସ୍ଥିର ଶୂନ୍ୟ update_curr_stop(ସଂରଚନା rq *rq)
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
