// SPDX-License-Identifier: GPL-2.0
/*
 * stop-task scheduling class.
 *
 * The stop task is the highest priority task in the system, it preempts
 * everything and will be preempted by nothing.
 *
 * See kernel/stop_machine.c
 */
#समावेश "sched.h"

अटल पूर्णांक
select_task_rq_stop(काष्ठा task_struct *p, पूर्णांक cpu, पूर्णांक flags)
{
	वापस task_cpu(p); /* stop tasks as never migrate */
}

अटल पूर्णांक
balance_stop(काष्ठा rq *rq, काष्ठा rq_flags *rf)
{
	वापस sched_stop_runnable(rq);
}

अटल व्योम
wakeup_preempt_stop(काष्ठा rq *rq, काष्ठा task_struct *p, पूर्णांक flags)
{
	/* we're never preempted */
}

अटल व्योम set_अग्र_task_stop(काष्ठा rq *rq, काष्ठा task_struct *stop, क्रमागत snt_e type)
{
	अगर (type == SNT_REPICK)
		वापस;

	stop->se.exec_start = rq_clock_task(rq);
}

अटल काष्ठा task_struct *pick_task_stop(काष्ठा rq *rq, काष्ठा rq_flags *rf)
{
	अगर (!sched_stop_runnable(rq))
		वापस शून्य;

	वापस rq->stop;
}

अटल व्योम
enqueue_task_stop(काष्ठा rq *rq, काष्ठा task_struct *p, पूर्णांक flags)
{
	add_nr_running(rq, 1);
}

अटल bool
dequeue_task_stop(काष्ठा rq *rq, काष्ठा task_struct *p, पूर्णांक flags)
{
	sub_nr_running(rq, 1);
	वापस true;
}

अटल व्योम yield_task_stop(काष्ठा rq *rq)
{
	BUG(); /* the stop task should never yield, its pointless. */
}

अटल व्योम put_पूर्व_task_stop(काष्ठा rq *rq, काष्ठा task_struct *पूर्व, काष्ठा task_struct *अग्र)
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
अटल व्योम task_tick_stop(काष्ठा rq *rq, काष्ठा task_struct *curr, पूर्णांक queued)
{
}

अटल व्योम switching_to_stop(काष्ठा rq *rq, काष्ठा task_struct *p)
{
	BUG(); /* its impossible to change to this class */
}

अटल व्योम
prio_changed_stop(काष्ठा rq *rq, काष्ठा task_struct *p, u64 oldprio)
{
	अगर (p->prio == oldprio)
		वापस;

	BUG(); /* how!?, what priority? */
}

अटल व्योम update_curr_stop(काष्ठा rq *rq)
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
	.put_पूर्व_task		= put_पूर्व_task_stop,
	.set_अग्र_task          = set_अग्र_task_stop,

	.balance		= balance_stop,
	.select_task_rq		= select_task_rq_stop,
	.set_cpus_allowed	= set_cpus_allowed_common,

	.task_tick		= task_tick_stop,

	.prio_changed		= prio_changed_stop,
	.switching_to		= switching_to_stop,
	.update_curr		= update_curr_stop,
};
