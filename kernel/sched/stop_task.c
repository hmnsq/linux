// SPDX-License-Identifier: GPL-2.0
/*
 * stop-task scheduling class.
 *
 * The stop task is the highest priority task in the system, it preempts
 * everything and will be preempted by nothing.
 *
 * See kernel/stop_machine.c
 */
#అన్తర్భావ "sched.h"

స్థిర పూర్ణాంకం
select_task_rq_stop(నిర్మాణం task_struct *p, పూర్ణాంకం cpu, పూర్ణాంకం flags)
{
	తిరిగి task_cpu(p); /* stop tasks as never migrate */
}

స్థిర పూర్ణాంకం
balance_stop(నిర్మాణం rq *rq, నిర్మాణం rq_flags *rf)
{
	తిరిగి sched_stop_runnable(rq);
}

స్థిర శూన్య
wakeup_preempt_stop(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, పూర్ణాంకం flags)
{
	/* we're never preempted */
}

స్థిర శూన్య సమితి_తదుపరి_task_stop(నిర్మాణం rq *rq, నిర్మాణం task_struct *stop, గణన snt_e type)
{
	ఒకవేళ (type == SNT_REPICK)
		తిరిగి;

	stop->se.exec_ప్రారంభం = rq_clock_task(rq);
}

స్థిర నిర్మాణం task_struct *pick_task_stop(నిర్మాణం rq *rq, నిర్మాణం rq_flags *rf)
{
	ఒకవేళ (!sched_stop_runnable(rq))
		తిరిగి ఖాళీ;

	తిరిగి rq->stop;
}

స్థిర శూన్య
వరుసలో_చేర్చు_task_stop(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, పూర్ణాంకం flags)
{
	కూడు_nr_running(rq, 1);
}

స్థిర bool
వరుస_నుండి_తీయి_task_stop(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, పూర్ణాంకం flags)
{
	sub_nr_running(rq, 1);
	తిరిగి true;
}

స్థిర శూన్య yield_task_stop(నిర్మాణం rq *rq)
{
	BUG(); /* the stop task should never yield, its pointless. */
}

స్థిర శూన్య put_prev_task_stop(నిర్మాణం rq *rq, నిర్మాణం task_struct *prev, నిర్మాణం task_struct *తదుపరి)
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
స్థిర శూన్య task_tick_stop(నిర్మాణం rq *rq, నిర్మాణం task_struct *curr, పూర్ణాంకం queued)
{
}

స్థిర శూన్య switching_గమ్యం_stop(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
	BUG(); /* its impossible to change to this class */
}

స్థిర శూన్య
prio_మారింది_stop(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, u64 oldprio)
{
	ఒకవేళ (p->prio == oldprio)
		తిరిగి;

	BUG(); /* how!?, what priority? */
}

స్థిర శూన్య update_curr_stop(నిర్మాణం rq *rq)
{
}

/*
 * Simple, special scheduling class for the per-CPU stop tasks:
 */
DEFINE_SCHED_CLASS(stop) = {
	.వరుసలో_చేర్చు_task		= వరుసలో_చేర్చు_task_stop,
	.వరుస_నుండి_తీయి_task		= వరుస_నుండి_తీయి_task_stop,
	.yield_task		= yield_task_stop,

	.wakeup_preempt		= wakeup_preempt_stop,

	.pick_task		= pick_task_stop,
	.put_prev_task		= put_prev_task_stop,
	.సమితి_తదుపరి_task          = సమితి_తదుపరి_task_stop,

	.balance		= balance_stop,
	.select_task_rq		= select_task_rq_stop,
	.సమితి_cpus_allowed	= సమితి_cpus_allowed_common,

	.task_tick		= task_tick_stop,

	.prio_మారింది		= prio_మారింది_stop,
	.switching_గమ్యం		= switching_గమ్యం_stop,
	.update_curr		= update_curr_stop,
};
