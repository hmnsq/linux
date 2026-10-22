// SPDX-License-Identifier: GPL-2.0
/*
 * stop-task scheduling class.
 *
 * The stop task is the highest priority task in the system, it preempts
 * everything and will be preempted by nothing.
 *
 * See kernel/stop_machine.c
 */
#ਅਨ੍ਤਰ੍ਭਾਵ "sched.h"

ਸਥਿਰ ਪੂਰਨ
select_task_rq_stop(ਬਣਤਰ task_struct *p, ਪੂਰਨ cpu, ਪੂਰਨ flags)
{
	ਵਾਪਸ task_cpu(p); /* stop tasks as never migrate */
}

ਸਥਿਰ ਪੂਰਨ
balance_stop(ਬਣਤਰ rq *rq, ਬਣਤਰ rq_flags *rf)
{
	ਵਾਪਸ sched_stop_runnable(rq);
}

ਸਥਿਰ ਖਾਲੀ
wakeup_preempt_stop(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, ਪੂਰਨ flags)
{
	/* we're never preempted */
}

ਸਥਿਰ ਖਾਲੀ ਸੈੱਟ_ਅਗਲਾ_task_stop(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *stop, ਗਿਣਤੀ snt_e type)
{
	ਜੇ (type == SNT_REPICK)
		ਵਾਪਸ;

	stop->se.exec_ਸ਼ੁਰੂ = rq_clock_task(rq);
}

ਸਥਿਰ ਬਣਤਰ task_struct *pick_task_stop(ਬਣਤਰ rq *rq, ਬਣਤਰ rq_flags *rf)
{
	ਜੇ (!sched_stop_runnable(rq))
		ਵਾਪਸ ਨਿਰਾ;

	ਵਾਪਸ rq->stop;
}

ਸਥਿਰ ਖਾਲੀ
ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_task_stop(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, ਪੂਰਨ flags)
{
	ਜੋੜੋ_nr_running(rq, 1);
}

ਸਥਿਰ bool
ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_task_stop(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, ਪੂਰਨ flags)
{
	sub_nr_running(rq, 1);
	ਵਾਪਸ true;
}

ਸਥਿਰ ਖਾਲੀ yield_task_stop(ਬਣਤਰ rq *rq)
{
	BUG(); /* the stop task should never yield, its pointless. */
}

ਸਥਿਰ ਖਾਲੀ put_prev_task_stop(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *prev, ਬਣਤਰ task_struct *ਅਗਲਾ)
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
ਸਥਿਰ ਖਾਲੀ task_tick_stop(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *curr, ਪੂਰਨ queued)
{
}

ਸਥਿਰ ਖਾਲੀ switching_ਮੰਜ਼ਿਲ_stop(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p)
{
	BUG(); /* its impossible to change to this class */
}

ਸਥਿਰ ਖਾਲੀ
prio_ਬਦਲਿਆ_stop(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, u64 oldprio)
{
	ਜੇ (p->prio == oldprio)
		ਵਾਪਸ;

	BUG(); /* how!?, what priority? */
}

ਸਥਿਰ ਖਾਲੀ update_curr_stop(ਬਣਤਰ rq *rq)
{
}

/*
 * Simple, special scheduling class for the per-CPU stop tasks:
 */
DEFINE_SCHED_CLASS(stop) = {
	.ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_task		= ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_task_stop,
	.ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_task		= ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_task_stop,
	.yield_task		= yield_task_stop,

	.wakeup_preempt		= wakeup_preempt_stop,

	.pick_task		= pick_task_stop,
	.put_prev_task		= put_prev_task_stop,
	.ਸੈੱਟ_ਅਗਲਾ_task          = ਸੈੱਟ_ਅਗਲਾ_task_stop,

	.balance		= balance_stop,
	.select_task_rq		= select_task_rq_stop,
	.ਸੈੱਟ_cpus_allowed	= ਸੈੱਟ_cpus_allowed_common,

	.task_tick		= task_tick_stop,

	.prio_ਬਦਲਿਆ		= prio_ਬਦਲਿਆ_stop,
	.switching_ਮੰਜ਼ਿਲ		= switching_ਮੰਜ਼ਿਲ_stop,
	.update_curr		= update_curr_stop,
};
