// SPDX-License-Identifier: GPL-2.0
/*
 * stop-task scheduling class.
 *
 * The stop task is the highest priority task in the system, it preempts
 * everything and will be preempted by nothing.
 *
 * See kernel/stop_machine.c
 */
#अन्तर्भाव "sched.h"

स्थिर पूर्णाङ्क
select_task_rq_stop(संरचना task_struct *p, पूर्णाङ्क cpu, पूर्णाङ्क flags)
{
	प्रत्यागम task_cpu(p); /* stop tasks as never migrate */
}

स्थिर पूर्णाङ्क
balance_stop(संरचना rq *rq, संरचना rq_flags *rf)
{
	प्रत्यागम sched_stop_runnable(rq);
}

स्थिर शून्य
wakeup_preempt_stop(संरचना rq *rq, संरचना task_struct *p, पूर्णाङ्क flags)
{
	/* we're never preempted */
}

स्थिर शून्य समुच्चयः_अग्र_task_stop(संरचना rq *rq, संरचना task_struct *stop, गणना snt_e type)
{
	यदि (type == SNT_REPICK)
		प्रत्यागम;

	stop->se.exec_start = rq_clock_task(rq);
}

स्थिर संरचना task_struct *pick_task_stop(संरचना rq *rq, संरचना rq_flags *rf)
{
	यदि (!sched_stop_runnable(rq))
		प्रत्यागम रिक्त;

	प्रत्यागम rq->stop;
}

स्थिर शून्य
पङ्क्तौ_निक्षिप_task_stop(संरचना rq *rq, संरचना task_struct *p, पूर्णाङ्क flags)
{
	सङ्कलय_nr_running(rq, 1);
}

स्थिर bool
पङ्क्तेः_निष्कासय_task_stop(संरचना rq *rq, संरचना task_struct *p, पूर्णाङ्क flags)
{
	sub_nr_running(rq, 1);
	प्रत्यागम true;
}

स्थिर शून्य yield_task_stop(संरचना rq *rq)
{
	BUG(); /* the stop task should never yield, its pointless. */
}

स्थिर शून्य put_पूर्व_task_stop(संरचना rq *rq, संरचना task_struct *पूर्व, संरचना task_struct *अग्र)
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
स्थिर शून्य task_tick_stop(संरचना rq *rq, संरचना task_struct *curr, पूर्णाङ्क queued)
{
}

स्थिर शून्य switching_गन्तव्यम्_stop(संरचना rq *rq, संरचना task_struct *p)
{
	BUG(); /* its impossible to change to this class */
}

स्थिर शून्य
prio_परिवर्तितम्_stop(संरचना rq *rq, संरचना task_struct *p, u64 oldprio)
{
	यदि (p->prio == oldprio)
		प्रत्यागम;

	BUG(); /* how!?, what priority? */
}

स्थिर शून्य update_curr_stop(संरचना rq *rq)
{
}

/*
 * Simple, special scheduling class for the per-CPU stop tasks:
 */
DEFINE_SCHED_CLASS(stop) = {
	.पङ्क्तौ_निक्षिप_task		= पङ्क्तौ_निक्षिप_task_stop,
	.पङ्क्तेः_निष्कासय_task		= पङ्क्तेः_निष्कासय_task_stop,
	.yield_task		= yield_task_stop,

	.wakeup_preempt		= wakeup_preempt_stop,

	.pick_task		= pick_task_stop,
	.put_पूर्व_task		= put_पूर्व_task_stop,
	.समुच्चयः_अग्र_task          = समुच्चयः_अग्र_task_stop,

	.balance		= balance_stop,
	.select_task_rq		= select_task_rq_stop,
	.समुच्चयः_cpus_allowed	= समुच्चयः_cpus_allowed_common,

	.task_tick		= task_tick_stop,

	.prio_परिवर्तितम्		= prio_परिवर्तितम्_stop,
	.switching_गन्तव्यम्		= switching_गन्तव्यम्_stop,
	.update_curr		= update_curr_stop,
};
