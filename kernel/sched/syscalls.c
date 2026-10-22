// SPDX-License-Identifier: GPL-2.0-only
/*
 *  kernel/sched/syscalls.c
 *
 *  Core kernel scheduler syscalls related code
 *
 *  Copyright (C) 1991-2002  Linus Torvalds
 *  Copyright (C) 1998-2024  Ingo Molnar, Red Hat
 */
#అన్తర్భావ <linux/sched.h>
#అన్తర్భావ <linux/cpuset.h>
#అన్తర్భావ <linux/sched/debug.h>

#అన్తర్భావ <uapi/linux/sched/types.h>

#అన్తర్భావ "sched.h"
#అన్తర్భావ "autogroup.h"

స్థిర అంతఃవరుస పూర్ణాంకం __normal_prio(పూర్ణాంకం policy, పూర్ణాంకం rt_prio, పూర్ణాంకం nice)
{
	పూర్ణాంకం prio;

	ఒకవేళ (dl_policy(policy))
		prio = MAX_DL_PRIO - 1;
	లేకపోతే ఒకవేళ (rt_policy(policy))
		prio = MAX_RT_PRIO - 1 - rt_prio;
	లేకపోతే
		prio = NICE_గమ్యం_PRIO(nice);

	తిరిగి prio;
}

/*
 * Calculate the expected normal priority: i.e. priority
 * without taking RT-inheritance into account. Might be
 * boosted by interactivity modifiers. Changes upon fork,
 * setprio syscalls, and whenever the interactivity
 * estimator recalculates.
 */
స్థిర అంతఃవరుస పూర్ణాంకం సాధారణ_prio(నిర్మాణం task_struct *p)
{
	తిరిగి __normal_prio(p->policy, p->rt_ప్రాధాన్యత, PRIO_గమ్యం_NICE(p->static_prio));
}

/*
 * Calculate the current priority, i.e. the priority
 * taken into account by the scheduler. This value might
 * be boosted by RT tasks, or might be boosted by
 * interactivity modifiers. Will be RT if the task got
 * RT-boosted. If not then it returns p->normal_prio.
 */
స్థిర పూర్ణాంకం effective_prio(నిర్మాణం task_struct *p)
{
	p->సాధారణ_prio = సాధారణ_prio(p);
	/*
	 * If we are RT tasks or we were boosted to RT priority,
	 * keep the priority unchanged. Otherwise, update priority
	 * to the normal priority:
	 */
	ఒకవేళ (!rt_or_dl_prio(p->prio))
		తిరిగి p->సాధారణ_prio;
	తిరిగి p->prio;
}

శూన్య సమితి_user_nice(నిర్మాణం task_struct *p, దీర్ఘ nice)
{
	పూర్ణాంకం old_prio;

	ఒకవేళ (task_nice(p) == nice || nice < MIN_NICE || nice > MAX_NICE)
		తిరిగి;
	/*
	 * We have to be careful, if called from sys_setpriority(),
	 * the task might be in the middle of scheduling on another CPU.
	 */
	guard(task_rq_తాళం_2)(p);

	/*
	 * The RT priorities are set via sched_setscheduler(), but we still
	 * allow the 'normal' nice value to be set - but as expected
	 * it won't have any effect on scheduling until the task is
	 * SCHED_DEADLINE, SCHED_FIFO or SCHED_RR:
	 */
	ఒకవేళ (task_has_dl_policy(p) || task_has_rt_policy(p)) {
		p->static_prio = NICE_గమ్యం_PRIO(nice);
		తిరిగి;
	}

	scoped_guard (sched_change, p, వరుస_నుండి_తీయి_SAVE) {
		p->static_prio = NICE_గమ్యం_PRIO(nice);
		సమితి_ఎక్కించు_బరువు(p, true);
		old_prio = p->prio;
		p->prio = effective_prio(p);
	}
}
EXPORT_చిహ్నం(సమితి_user_nice);

/*
 * is_nice_reduction - check if nice value is an actual reduction
 *
 * Similar to can_nice() but does not perform a capability check.
 *
 * @p: task
 * @nice: nice value
 */
స్థిర bool is_nice_reduction(స్థిరరాశి నిర్మాణం task_struct *p, స్థిరరాశి పూర్ణాంకం nice)
{
	/* Convert nice value [19,-20] to rlimit style value [1,40]: */
	పూర్ణాంకం nice_rlim = nice_గమ్యం_rlimit(nice);

	తిరిగి (nice_rlim <= task_rlimit(p, RLIMIT_NICE));
}

/*
 * can_nice - check if a task can reduce its nice value
 * @p: task
 * @nice: nice value
 */
పూర్ణాంకం can_nice(స్థిరరాశి నిర్మాణం task_struct *p, స్థిరరాశి పూర్ణాంకం nice)
{
	తిరిగి is_nice_reduction(p, nice) || capable(CAP_SYS_NICE);
}

#నిర్వచితమైతే __ARCH_WANT_SYS_NICE

/*
 * sys_nice - change the priority of the current process.
 * @increment: priority increment
 *
 * sys_setpriority is a more generic, but much slower function that
 * does similar things.
 */
SYSCALL_DEFINE1(nice, పూర్ణాంకం, increment)
{
	దీర్ఘ nice, retval;

	/*
	 * Setpriority might change our priority at the same moment.
	 * We don't have to worry. Conceptually one call occurs first
	 * and we have a single winner.
	 */
	increment = clamp(increment, -NICE_వెడల్పు, NICE_వెడల్పు);
	nice = task_nice(current) + increment;

	nice = clamp_val(nice, MIN_NICE, MAX_NICE);
	ఒకవేళ (increment < 0 && !can_nice(current, nice))
		తిరిగి -EPERM;

	retval = security_task_setnice(current, nice);
	ఒకవేళ (retval)
		తిరిగి retval;

	సమితి_user_nice(current, nice);
	తిరిగి 0;
}

#అయితే_ముగింపు /* __ARCH_WANT_SYS_NICE */

/**
 * task_prio - return the priority value of a given task.
 * @p: the task in question.
 *
 * Return: The priority value as seen by users in /proc.
 *
 * sched policy         return value   kernel prio    user prio/nice
 *
 * normal, batch, idle     [0 ... 39]  [100 ... 139]          0/[-20 ... 19]
 * fifo, rr             [-2 ... -100]     [98 ... 0]  [1 ... 99]
 * deadline                     -101             -1           0
 */
పూర్ణాంకం task_prio(స్థిరరాశి నిర్మాణం task_struct *p)
{
	తిరిగి p->prio - MAX_RT_PRIO;
}

/**
 * idle_cpu - is a given CPU idle currently?
 * @cpu: the processor in question.
 *
 * Return: 1 if the CPU is currently idle. 0 otherwise.
 */
పూర్ణాంకం idle_cpu(పూర్ణాంకం cpu)
{
	తిరిగి idle_rq(cpu_rq(cpu));
}

/**
 * idle_task - return the idle task for a given CPU.
 * @cpu: the processor in question.
 *
 * Return: The idle task for the CPU @cpu.
 */
నిర్మాణం task_struct *idle_task(పూర్ణాంకం cpu)
{
	తిరిగి cpu_rq(cpu)->idle;
}

#నిర్వచితమైతే CONFIG_SCHED_CORE
పూర్ణాంకం sched_core_idle_cpu(పూర్ణాంకం cpu)
{
	నిర్మాణం rq *rq = cpu_rq(cpu);

	ఒకవేళ (sched_core_enabled(rq) && rq->curr == rq->idle)
		తిరిగి 1;

	తిరిగి idle_cpu(cpu);
}
#అయితే_ముగింపు /* CONFIG_SCHED_CORE */

/**
 * find_process_by_pid - find a process with a matching PID value.
 * @pid: the pid in question.
 *
 * The task of @pid, if found. %NULL otherwise.
 */
స్థిర నిర్మాణం task_struct *కనుగొను_process_by_pid(pid_t pid)
{
	తిరిగి pid ? కనుగొను_task_by_vpid(pid) : current;
}

స్థిర నిర్మాణం task_struct *కనుగొను_get_task(pid_t pid)
{
	నిర్మాణం task_struct *p;
	guard(rcu)();

	p = కనుగొను_process_by_pid(pid);
	ఒకవేళ (likely(p))
		get_task_struct(p);

	తిరిగి p;
}

DEFINE_CLASS(కనుగొను_get_task, నిర్మాణం task_struct *, ఒకవేళ (_T) put_task_struct(_T),
	     కనుగొను_get_task(pid), pid_t pid)

/*
 * sched_setparam() passes in -1 for its policy, to let the functions
 * it calls know not to change it.
 */
#నిర్వచించు SETPARAM_POLICY	-1

స్థిర శూన్య __setscheduler_params(నిర్మాణం task_struct *p,
		స్థిరరాశి నిర్మాణం sched_attr *attr)
{
	పూర్ణాంకం policy = attr->sched_policy;

	ఒకవేళ (policy == SETPARAM_POLICY)
		policy = p->policy;

	p->policy = policy;

	ఒకవేళ (dl_policy(policy))
		__setparam_dl(p, attr);
	లేకపోతే ఒకవేళ (fair_policy(policy))
		__setparam_fair(p, attr);

	/* rt-policy tasks do not have a timerslack */
	ఒకవేళ (rt_or_dl_task_policy(p)) {
		p->timer_slack_ns = 0;
	} లేకపోతే ఒకవేళ (p->timer_slack_ns == 0) {
		/* when switching back to non-rt policy, restore timerslack */
		p->timer_slack_ns = p->default_timer_slack_ns;
	}

	/*
	 * __sched_setscheduler() ensures attr->sched_priority == 0 when
	 * !rt_policy. Always setting this ensures that things like
	 * getparam()/getattr() don't report silly values for !rt tasks.
	 */
	p->rt_ప్రాధాన్యత = attr->sched_ప్రాధాన్యత;
	p->సాధారణ_prio = సాధారణ_prio(p);
	సమితి_ఎక్కించు_బరువు(p, true);
}

/*
 * Check the target process has a UID that matches the current process's:
 */
స్థిర bool check_same_owner(నిర్మాణం task_struct *p)
{
	స్థిరరాశి నిర్మాణం cred *cred = current_cred(), *pcred;
	guard(rcu)();

	pcred = __task_cred(p);
	తిరిగి (uid_eq(cred->euid, pcred->euid) ||
		uid_eq(cred->euid, pcred->uid));
}

#నిర్వచితమైతే CONFIG_RT_MUTEXES
స్థిర అంతఃవరుస శూన్య __setscheduler_dl_pi(పూర్ణాంకం newprio, పూర్ణాంకం policy,
			      నిర్మాణం task_struct *p,
			      నిర్మాణం sched_change_ctx *scope)
{
	/*
	 * In case a DEADLINE task (either proper or boosted) gets
	 * setscheduled to a lower priority class, check if it neeeds to
	 * inherit parameters from a potential pi_task. In that case make
	 * sure replenishment happens with the next enqueue.
	 */

	ఒకవేళ (dl_prio(newprio) && !dl_policy(policy)) {
		నిర్మాణం task_struct *pi_task = rt_mutex_get_శిఖరం_task(p);

		ఒకవేళ (pi_task) {
			p->dl.pi_se = pi_task->dl.pi_se;
			scope->flags |= వరుసలో_చేర్చు_REPLENISH;
		}
	}
}
#లేకపోతే /* !CONFIG_RT_MUTEXES */
స్థిర అంతఃవరుస శూన్య __setscheduler_dl_pi(పూర్ణాంకం newprio, పూర్ణాంకం policy,
			      నిర్మాణం task_struct *p,
			      నిర్మాణం sched_change_ctx *scope)
{
}
#అయితే_ముగింపు /* !CONFIG_RT_MUTEXES */

#నిర్వచితమైతే CONFIG_UCLAMP_TASK

స్థిర పూర్ణాంకం uclamp_validate(నిర్మాణం task_struct *p,
			   స్థిరరాశి నిర్మాణం sched_attr *attr)
{
	పూర్ణాంకం util_min = p->uclamp_req[UCLAMP_MIN].విలువ;
	పూర్ణాంకం util_max = p->uclamp_req[UCLAMP_MAX].విలువ;

	ఒకవేళ (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MIN) {
		util_min = attr->sched_util_min;

		ఒకవేళ (util_min + 1 > SCHED_సామర్థ్యం_SCALE + 1)
			తిరిగి -EINVAL;
	}

	ఒకవేళ (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MAX) {
		util_max = attr->sched_util_max;

		ఒకవేళ (util_max + 1 > SCHED_సామర్థ్యం_SCALE + 1)
			తిరిగి -EINVAL;
	}

	ఒకవేళ (util_min != -1 && util_max != -1 && util_min > util_max)
		తిరిగి -EINVAL;

	/*
	 * We have valid uclamp attributes; make sure uclamp is enabled.
	 *
	 * We need to do that here, because enabling static branches is a
	 * blocking operation which obviously cannot be done while holding
	 * scheduler locks.
	 */
	sched_uclamp_enable();

	తిరిగి 0;
}

స్థిర bool uclamp_reset(స్థిరరాశి నిర్మాణం sched_attr *attr,
			 గణన uclamp_id clamp_id,
			 నిర్మాణం uclamp_se *uc_se)
{
	/* Reset on sched class change for a non user-defined clamp value. */
	ఒకవేళ (likely(!(attr->sched_flags & SCHED_FLAG_UTIL_CLAMP)) &&
	    !uc_se->user_defined)
		తిరిగి true;

	/* Reset on sched_util_{min,max} == -1. */
	ఒకవేళ (clamp_id == UCLAMP_MIN &&
	    attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MIN &&
	    attr->sched_util_min == -1) {
		తిరిగి true;
	}

	ఒకవేళ (clamp_id == UCLAMP_MAX &&
	    attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MAX &&
	    attr->sched_util_max == -1) {
		తిరిగి true;
	}

	తిరిగి false;
}

స్థిర శూన్య __setscheduler_uclamp(నిర్మాణం task_struct *p,
				  స్థిరరాశి నిర్మాణం sched_attr *attr)
{
	గణన uclamp_id clamp_id;

	for_each_clamp_id(clamp_id) {
		నిర్మాణం uclamp_se *uc_se = &p->uclamp_req[clamp_id];
		గుర్తులేని పూర్ణాంకం విలువ;

		ఒకవేళ (!uclamp_reset(attr, clamp_id, uc_se))
			కొనసాగు;

		/*
		 * RT by default have a 100% boost value that could be modified
		 * at runtime.
		 */
		ఒకవేళ (unlikely(rt_task(p) && clamp_id == UCLAMP_MIN))
			విలువ = sysctl_sched_uclamp_util_min_rt_default;
		లేకపోతే
			విలువ = uclamp_none(clamp_id);

		uclamp_se_సమితి(uc_se, విలువ, false);

	}

	ఒకవేళ (likely(!(attr->sched_flags & SCHED_FLAG_UTIL_CLAMP)))
		తిరిగి;

	ఒకవేళ (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MIN &&
	    attr->sched_util_min != -1) {
		uclamp_se_సమితి(&p->uclamp_req[UCLAMP_MIN],
			      attr->sched_util_min, true);
	}

	ఒకవేళ (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MAX &&
	    attr->sched_util_max != -1) {
		uclamp_se_సమితి(&p->uclamp_req[UCLAMP_MAX],
			      attr->sched_util_max, true);
	}
}

#లేకపోతే /* !CONFIG_UCLAMP_TASK: */

స్థిర అంతఃవరుస పూర్ణాంకం uclamp_validate(నిర్మాణం task_struct *p,
				  స్థిరరాశి నిర్మాణం sched_attr *attr)
{
	తిరిగి -EOPNOTSUPP;
}
స్థిర శూన్య __setscheduler_uclamp(నిర్మాణం task_struct *p,
				  స్థిరరాశి నిర్మాణం sched_attr *attr) { }
#అయితే_ముగింపు /* !CONFIG_UCLAMP_TASK */

/*
 * Allow unprivileged RT tasks to decrease priority.
 * Only issue a capable test if needed and only once to avoid an audit
 * event on permitted non-privileged operations:
 */
స్థిర పూర్ణాంకం user_check_sched_setscheduler(నిర్మాణం task_struct *p,
					 స్థిరరాశి నిర్మాణం sched_attr *attr,
					 పూర్ణాంకం policy, పూర్ణాంకం reset_on_fork)
{
	ఒకవేళ (fair_policy(policy)) {
		ఒకవేళ (attr->sched_nice < task_nice(p) &&
		    !is_nice_reduction(p, attr->sched_nice))
			వెళ్ళు req_priv;
	}

	ఒకవేళ (rt_policy(policy)) {
		గుర్తులేని దీర్ఘ rlim_rtprio = task_rlimit(p, RLIMIT_RTPRIO);

		/* Can't set/change the rt policy: */
		ఒకవేళ (policy != p->policy && !rlim_rtprio)
			వెళ్ళు req_priv;

		/* Can't increase priority: */
		ఒకవేళ (attr->sched_ప్రాధాన్యత > p->rt_ప్రాధాన్యత &&
		    attr->sched_ప్రాధాన్యత > rlim_rtprio)
			వెళ్ళు req_priv;
	}

	/*
	 * Can't set/change SCHED_DEADLINE policy at all for now
	 * (safest behavior); in the future we would like to allow
	 * unprivileged DL tasks to increase their relative deadline
	 * or reduce their runtime (both ways reducing utilization)
	 */
	ఒకవేళ (dl_policy(policy))
		వెళ్ళు req_priv;

	/*
	 * Treat SCHED_IDLE as nice 20. Only allow a switch to
	 * SCHED_NORMAL if the RLIMIT_NICE would normally permit it.
	 */
	ఒకవేళ (task_has_idle_policy(p) && !idle_policy(policy)) {
		ఒకవేళ (!is_nice_reduction(p, task_nice(p)))
			వెళ్ళు req_priv;
	}

	/* Can't change other user's priorities: */
	ఒకవేళ (!check_same_owner(p))
		వెళ్ళు req_priv;

	/* Normal users shall not reset the sched_reset_on_fork flag: */
	ఒకవేళ (p->sched_reset_on_fork && !reset_on_fork)
		వెళ్ళు req_priv;

	తిరిగి 0;

req_priv:
	ఒకవేళ (!capable(CAP_SYS_NICE))
		తిరిగి -EPERM;

	తిరిగి 0;
}

పూర్ణాంకం __sched_setscheduler(నిర్మాణం task_struct *p,
			 స్థిరరాశి నిర్మాణం sched_attr *attr,
			 bool user, bool pi)
{
	పూర్ణాంకం oldpolicy = -1, policy = attr->sched_policy;
	పూర్ణాంకం retval, oldprio, newprio;
	స్థిరరాశి నిర్మాణం sched_class *prev_class, *తదుపరి_class;
	నిర్మాణం balance_callback *తల;
	నిర్మాణం rq_flags rf;
	పూర్ణాంకం reset_on_fork;
	పూర్ణాంకం వరుస_flags = వరుస_నుండి_తీయి_SAVE | వరుస_నుండి_తీయి_MOVE | వరుస_నుండి_తీయి_NOCLOCK;
	నిర్మాణం rq *rq;
	bool cpuset_locked = false;

	/* The pi code expects interrupts enabled */
	BUG_ON(pi && లోపల_అంతరాయం());
recheck:
	/* Double check policy once rq lock held: */
	ఒకవేళ (policy < 0) {
		reset_on_fork = p->sched_reset_on_fork;
		policy = oldpolicy = p->policy;
	} లేకపోతే {
		reset_on_fork = !!(attr->sched_flags & SCHED_FLAG_RESET_ON_FORK);

		ఒకవేళ (!valid_policy(policy))
			తిరిగి -EINVAL;
	}

	ఒకవేళ (attr->sched_flags & ~(SCHED_FLAG_ALL | SCHED_FLAG_SUGOV))
		తిరిగి -EINVAL;

	/*
	 * Valid priorities for SCHED_FIFO and SCHED_RR are
	 * 1..MAX_RT_PRIO-1, valid priority for SCHED_NORMAL,
	 * SCHED_BATCH and SCHED_IDLE is 0.
	 */
	ఒకవేళ (attr->sched_ప్రాధాన్యత > MAX_RT_PRIO-1)
		తిరిగి -EINVAL;
	ఒకవేళ ((dl_policy(policy) && !__checkparam_dl(attr)) ||
	    (rt_policy(policy) != (attr->sched_ప్రాధాన్యత != 0)))
		తిరిగి -EINVAL;

	ఒకవేళ (user) {
		retval = user_check_sched_setscheduler(p, attr, policy, reset_on_fork);
		ఒకవేళ (retval)
			తిరిగి retval;

		ఒకవేళ (attr->sched_flags & SCHED_FLAG_SUGOV)
			తిరిగి -EINVAL;

		retval = security_task_setscheduler(p);
		ఒకవేళ (retval)
			తిరిగి retval;
	}

	/* Update task specific "requested" clamps */
	ఒకవేళ (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP) {
		retval = uclamp_validate(p, attr);
		ఒకవేళ (retval)
			తిరిగి retval;
	}

	/*
	 * SCHED_DEADLINE bandwidth accounting relies on stable cpusets
	 * information.
	 */
	ఒకవేళ (dl_policy(policy) || dl_policy(p->policy)) {
		cpuset_locked = true;
		cpuset_తాళం();
	}

	/*
	 * Make sure no PI-waiters arrive (or leave) while we are
	 * changing the priority of the task:
	 *
	 * To be able to change p->policy safely, the appropriate
	 * runqueue lock must be held.
	 */
	rq = task_rq_తాళం_2(p, &rf);
	update_rq_clock(rq);

	/*
	 * Changing the policy of the stop threads its a very bad idea:
	 */
	ఒకవేళ (p == rq->stop) {
		retval = -EINVAL;
		వెళ్ళు unlock;
	}

	retval = scx_check_setscheduler(p, policy);
	ఒకవేళ (retval)
		వెళ్ళు unlock;

	/*
	 * If not changing anything there's no need to proceed further,
	 * but store a possible modification of reset_on_fork.
	 */
	ఒకవేళ (unlikely(policy == p->policy)) {
		ఒకవేళ (fair_policy(policy) &&
		    (attr->sched_nice != task_nice(p) ||
		     (attr->sched_runtime != p->se.ముక్క)))
			వెళ్ళు change;
		ఒకవేళ (rt_policy(policy) && attr->sched_ప్రాధాన్యత != p->rt_ప్రాధాన్యత)
			వెళ్ళు change;
		ఒకవేళ (dl_policy(policy) && dl_param_మారింది(p, attr))
			వెళ్ళు change;
		ఒకవేళ (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP)
			వెళ్ళు change;

		p->sched_reset_on_fork = reset_on_fork;
		retval = 0;
		వెళ్ళు unlock;
	}
change:

	ఒకవేళ (user) {
#నిర్వచితమైతే CONFIG_RT_సమూహం_SCHED
		/*
		 * Do not allow real-time tasks into groups that have no runtime
		 * assigned.
		 */
		ఒకవేళ (rt_సమూహం_sched_enabled() &&
				rt_bandwidth_enabled() && rt_policy(policy) &&
				task_సమూహం(p)->rt_bandwidth.rt_runtime == 0 &&
				!task_సమూహం_is_autogroup(task_సమూహం(p))) {
			retval = -EPERM;
			వెళ్ళు unlock;
		}
#అయితే_ముగింపు /* CONFIG_RT_GROUP_SCHED */
		ఒకవేళ (dl_bandwidth_enabled() && dl_policy(policy) &&
				!(attr->sched_flags & SCHED_FLAG_SUGOV)) {
			cpumask_t *పరిధి = rq->rd->పరిధి;

			/*
			 * Don't allow tasks with an affinity mask smaller than
			 * the entire root_domain to become SCHED_DEADLINE. We
			 * will also fail if there's no bandwidth available.
			 */
			ఒకవేళ (!cpumask_subset(పరిధి, p->cpus_ptr) ||
			    rq->rd->dl_bw.bw == 0) {
				retval = -EPERM;
				వెళ్ళు unlock;
			}
		}
	}

	/* Re-check policy now with rq lock held: */
	ఒకవేళ (unlikely(oldpolicy != -1 && oldpolicy != p->policy)) {
		policy = oldpolicy = -1;
		task_rq_unlock(rq, p, &rf);
		ఒకవేళ (cpuset_locked)
			cpuset_unlock();
		వెళ్ళు recheck;
	}

	/*
	 * If setscheduling to SCHED_DEADLINE (or changing the parameters
	 * of a SCHED_DEADLINE task) we need to check if enough bandwidth
	 * is available.
	 */
	ఒకవేళ ((dl_policy(policy) || dl_task(p)) && sched_dl_అధికప్రవాహం(p, policy, attr)) {
		retval = -EBUSY;
		వెళ్ళు unlock;
	}

	p->sched_reset_on_fork = reset_on_fork;
	oldprio = p->prio;

	newprio = __normal_prio(policy, attr->sched_ప్రాధాన్యత, attr->sched_nice);
	ఒకవేళ (pi) {
		/*
		 * Take priority boosted tasks into account. If the new
		 * effective priority is unchanged, we just store the new
		 * normal parameters and do not touch the scheduler class and
		 * the runqueue. This will be done when the task deboost
		 * itself.
		 */
		newprio = rt_effective_prio(p, newprio);
		ఒకవేళ (newprio == oldprio && !dl_prio(newprio))
			వరుస_flags &= ~వరుస_నుండి_తీయి_MOVE;
	}

	prev_class = p->sched_class;
	తదుపరి_class = __setscheduler_class(policy, newprio);

	ఒకవేళ (prev_class != తదుపరి_class)
		వరుస_flags |= వరుస_నుండి_తీయి_CLASS;

	scoped_guard (sched_change, p, వరుస_flags) {

		ఒకవేళ (!(attr->sched_flags & SCHED_FLAG_KEEP_PARAMS)) {
			__setscheduler_params(p, attr);
			p->sched_class = తదుపరి_class;
			p->prio = newprio;
			__setscheduler_dl_pi(newprio, policy, p, scope);
		}
		__setscheduler_uclamp(p, attr);

		ఒకవేళ (scope->queued) {
			/*
			 * We enqueue to tail when the priority of a task is
			 * increased (user space view).
			 */
			ఒకవేళ (oldprio < p->prio)
				scope->flags |= వరుసలో_చేర్చు_తల;
		}
	}

	/* Avoid rq from going away on us: */
	preempt_disable();
	తల = splice_balance_callbacks(rq);
	task_rq_unlock(rq, p, &rf);

	ఒకవేళ (pi) {
		ఒకవేళ (cpuset_locked)
			cpuset_unlock();
		rt_mutex_adjust_pi(p);
	}

	/* Run balance callbacks after we've adjusted the PI chain: */
	balance_callbacks(rq, తల);
	preempt_enable();

	తిరిగి 0;

unlock:
	task_rq_unlock(rq, p, &rf);
	ఒకవేళ (cpuset_locked)
		cpuset_unlock();
	తిరిగి retval;
}

స్థిర పూర్ణాంకం _sched_setscheduler(నిర్మాణం task_struct *p, పూర్ణాంకం policy,
			       స్థిరరాశి నిర్మాణం sched_param *param, bool check)
{
	నిర్మాణం sched_attr attr = {
		.sched_policy   = policy,
		.sched_ప్రాధాన్యత = param->sched_ప్రాధాన్యత,
		.sched_nice	= PRIO_గమ్యం_NICE(p->static_prio),
	};

	ఒకవేళ (p->se.custom_ముక్క)
		attr.sched_runtime = p->se.ముక్క;

	/* Fixup the legacy SCHED_RESET_ON_FORK hack. */
	ఒకవేళ ((policy != SETPARAM_POLICY) && (policy & SCHED_RESET_ON_FORK)) {
		attr.sched_flags |= SCHED_FLAG_RESET_ON_FORK;
		policy &= ~SCHED_RESET_ON_FORK;
		attr.sched_policy = policy;
	}

	తిరిగి __sched_setscheduler(p, &attr, check, true);
}
/**
 * sched_setscheduler - change the scheduling policy and/or RT priority of a thread.
 * @p: the task in question.
 * @policy: new policy.
 * @param: structure containing the new RT priority.
 *
 * Use sched_set_fifo(), read its comment.
 *
 * Return: 0 on success. An error code otherwise.
 *
 * NOTE that the task may be already dead.
 */
పూర్ణాంకం sched_setscheduler(నిర్మాణం task_struct *p, పూర్ణాంకం policy,
		       స్థిరరాశి నిర్మాణం sched_param *param)
{
	తిరిగి _sched_setscheduler(p, policy, param, true);
}

పూర్ణాంకం sched_setattr(నిర్మాణం task_struct *p, స్థిరరాశి నిర్మాణం sched_attr *attr)
{
	తిరిగి __sched_setscheduler(p, attr, true, true);
}

పూర్ణాంకం sched_setattr_nocheck(నిర్మాణం task_struct *p, స్థిరరాశి నిర్మాణం sched_attr *attr)
{
	తిరిగి __sched_setscheduler(p, attr, false, true);
}
EXPORT_చిహ్నం_GPL(sched_setattr_nocheck);

/**
 * sched_setscheduler_nocheck - change the scheduling policy and/or RT priority of a thread from kernel-space.
 * @p: the task in question.
 * @policy: new policy.
 * @param: structure containing the new RT priority.
 *
 * Just like sched_setscheduler, only don't bother checking if the
 * current context has permission.  For example, this is needed in
 * stop_machine(): we create temporary high priority worker threads,
 * but our caller might not have that capability.
 *
 * Return: 0 on success. An error code otherwise.
 */
పూర్ణాంకం sched_setscheduler_nocheck(నిర్మాణం task_struct *p, పూర్ణాంకం policy,
			       స్థిరరాశి నిర్మాణం sched_param *param)
{
	తిరిగి _sched_setscheduler(p, policy, param, false);
}

/*
 * SCHED_FIFO is a broken scheduler model; that is, it is fundamentally
 * incapable of resource management, which is the one thing an OS really should
 * be doing.
 *
 * This is of course the reason it is limited to privileged users only.
 *
 * Worse still; it is fundamentally impossible to compose static priority
 * workloads. You cannot take two correctly working static prio workloads
 * and smash them together and still expect them to work.
 *
 * For this reason 'all' FIFO tasks the kernel creates are basically at:
 *
 *   MAX_RT_PRIO / 2
 *
 * The administrator _MUST_ configure the system, the kernel simply doesn't
 * know enough information to make a sensible choice.
 */
శూన్య sched_సమితి_fifo(నిర్మాణం task_struct *p)
{
	నిర్మాణం sched_param sp = { .sched_ప్రాధాన్యత = MAX_RT_PRIO / 2 };
	WARN_ON_ONCE(sched_setscheduler_nocheck(p, SCHED_FIFO, &sp) != 0);
}
EXPORT_చిహ్నం_GPL(sched_సమితి_fifo);

/*
 * For when you don't much care about FIFO, but want to be above SCHED_NORMAL.
 */
శూన్య sched_సమితి_fifo_దిగువ(నిర్మాణం task_struct *p)
{
	నిర్మాణం sched_param sp = { .sched_ప్రాధాన్యత = 1 };
	WARN_ON_ONCE(sched_setscheduler_nocheck(p, SCHED_FIFO, &sp) != 0);
}
EXPORT_చిహ్నం_GPL(sched_సమితి_fifo_దిగువ);

/*
 * Used when the primary interrupt handler is forced into a thread, in addition
 * to the (always threaded) secondary handler.  The secondary handler gets a
 * slightly lower priority so that the primary handler can preempt it, thereby
 * emulating the behavior of a non-PREEMPT_RT system where the primary handler
 * runs in hard interrupt context.
 */
శూన్య sched_సమితి_fifo_secondary(నిర్మాణం task_struct *p)
{
	నిర్మాణం sched_param sp = { .sched_ప్రాధాన్యత = MAX_RT_PRIO / 2 - 1 };
	WARN_ON_ONCE(sched_setscheduler_nocheck(p, SCHED_FIFO, &sp) != 0);
}

శూన్య sched_సమితి_సాధారణ(నిర్మాణం task_struct *p, పూర్ణాంకం nice)
{
	నిర్మాణం sched_attr attr = {
		.sched_policy = SCHED_సాధారణ,
		.sched_nice = nice,
	};
	WARN_ON_ONCE(sched_setattr_nocheck(p, &attr) != 0);
}
EXPORT_చిహ్నం_GPL(sched_సమితి_సాధారణ);

స్థిర పూర్ణాంకం
do_sched_setscheduler(pid_t pid, పూర్ణాంకం policy, నిర్మాణం sched_param __user *param)
{
	నిర్మాణం sched_param lparam;

	ఒకవేళ (unlikely(!param || pid < 0))
		తిరిగి -EINVAL;
	ఒకవేళ (copy_from_user(&lparam, param, పరిమాణం(నిర్మాణం sched_param)))
		తిరిగి -EFAULT;

	CLASS(కనుగొను_get_task, p)(pid);
	ఒకవేళ (!p)
		తిరిగి -ESRCH;

	తిరిగి sched_setscheduler(p, policy, &lparam);
}

/*
 * Mimics kernel/events/core.c perf_copy_attr().
 */
స్థిర పూర్ణాంకం sched_copy_attr(నిర్మాణం sched_attr __user *uattr, నిర్మాణం sched_attr *attr)
{
	u32 కొలత;
	పూర్ణాంకం ret;

	/* Zero the full structure, so that a short copy will be nice: */
	memset(attr, 0, పరిమాణం(*attr));

	ret = get_user(కొలత, &uattr->కొలత);
	ఒకవేళ (ret)
		తిరిగి ret;

	/* ABI compatibility quirk: */
	ఒకవేళ (!కొలత)
		కొలత = SCHED_ATTR_కొలత_VER0;
	ఒకవేళ (కొలత < SCHED_ATTR_కొలత_VER0 || కొలత > పుట_కొలత)
		వెళ్ళు err_కొలత;

	ret = copy_struct_from_user(attr, పరిమాణం(*attr), uattr, కొలత);
	ఒకవేళ (ret) {
		ఒకవేళ (ret == -E2BIG)
			వెళ్ళు err_కొలత;
		తిరిగి ret;
	}

	ఒకవేళ ((attr->sched_flags & SCHED_FLAG_UTIL_CLAMP) &&
	    కొలత < SCHED_ATTR_కొలత_VER1)
		తిరిగి -EINVAL;

	/*
	 * XXX: Do we want to be lenient like existing syscalls; or do we want
	 * to be strict and return an error on out-of-bounds values?
	 */
	attr->sched_nice = clamp(attr->sched_nice, MIN_NICE, MAX_NICE);

	తిరిగి 0;

err_కొలత:
	put_user(పరిమాణం(*attr), &uattr->కొలత);
	తిరిగి -E2BIG;
}

స్థిర శూన్య get_params(నిర్మాణం task_struct *p, నిర్మాణం sched_attr *attr, గుర్తులేని పూర్ణాంకం flags)
{
	ఒకవేళ (task_has_dl_policy(p)) {
		__getparam_dl(p, attr, flags);
	} లేకపోతే ఒకవేళ (task_has_rt_policy(p)) {
		attr->sched_ప్రాధాన్యత = p->rt_ప్రాధాన్యత;
	} లేకపోతే {
		attr->sched_nice = task_nice(p);
		attr->sched_runtime = p->se.ముక్క;
	}
}

/**
 * sys_sched_setscheduler - set/change the scheduler policy and RT priority
 * @pid: the pid in question.
 * @policy: new policy.
 * @param: structure containing the new RT priority.
 *
 * Return: 0 on success. An error code otherwise.
 */
SYSCALL_DEFINE3(sched_setscheduler, pid_t, pid, పూర్ణాంకం, policy, నిర్మాణం sched_param __user *, param)
{
	ఒకవేళ (policy < 0)
		తిరిగి -EINVAL;

	తిరిగి do_sched_setscheduler(pid, policy, param);
}

/**
 * sys_sched_setparam - set/change the RT priority of a thread
 * @pid: the pid in question.
 * @param: structure containing the new RT priority.
 *
 * Return: 0 on success. An error code otherwise.
 */
SYSCALL_DEFINE2(sched_setparam, pid_t, pid, నిర్మాణం sched_param __user *, param)
{
	తిరిగి do_sched_setscheduler(pid, SETPARAM_POLICY, param);
}

/**
 * sys_sched_setattr - same as above, but with extended sched_attr
 * @pid: the pid in question.
 * @uattr: structure containing the extended parameters.
 * @flags: for future extension.
 */
SYSCALL_DEFINE3(sched_setattr, pid_t, pid, నిర్మాణం sched_attr __user *, uattr,
			       గుర్తులేని పూర్ణాంకం, flags)
{
	నిర్మాణం sched_attr attr;
	పూర్ణాంకం retval;

	ఒకవేళ (unlikely(!uattr || pid < 0 || flags))
		తిరిగి -EINVAL;

	retval = sched_copy_attr(uattr, &attr);
	ఒకవేళ (retval)
		తిరిగి retval;

	ఒకవేళ ((పూర్ణాంకం)attr.sched_policy < 0)
		తిరిగి -EINVAL;
	ఒకవేళ (attr.sched_flags & SCHED_FLAG_KEEP_POLICY)
		attr.sched_policy = SETPARAM_POLICY;

	CLASS(కనుగొను_get_task, p)(pid);
	ఒకవేళ (!p)
		తిరిగి -ESRCH;

	ఒకవేళ (attr.sched_flags & SCHED_FLAG_KEEP_PARAMS)
		get_params(p, &attr, 0);

	తిరిగి sched_setattr(p, &attr);
}

/**
 * sys_sched_getscheduler - get the policy (scheduling class) of a thread
 * @pid: the pid in question.
 *
 * Return: On success, the policy of the thread. Otherwise, a negative error
 * code.
 */
SYSCALL_DEFINE1(sched_getscheduler, pid_t, pid)
{
	నిర్మాణం task_struct *p;
	పూర్ణాంకం retval;

	ఒకవేళ (pid < 0)
		తిరిగి -EINVAL;

	guard(rcu)();
	p = కనుగొను_process_by_pid(pid);
	ఒకవేళ (!p)
		తిరిగి -ESRCH;

	retval = security_task_getscheduler(p);
	ఒకవేళ (!retval) {
		retval = p->policy;
		ఒకవేళ (p->sched_reset_on_fork)
			retval |= SCHED_RESET_ON_FORK;
	}
	తిరిగి retval;
}

/**
 * sys_sched_getparam - get the RT priority of a thread
 * @pid: the pid in question.
 * @param: structure containing the RT priority.
 *
 * Return: On success, 0 and the RT priority is in @param. Otherwise, an error
 * code.
 */
SYSCALL_DEFINE2(sched_getparam, pid_t, pid, నిర్మాణం sched_param __user *, param)
{
	నిర్మాణం sched_param lp = { .sched_ప్రాధాన్యత = 0 };
	నిర్మాణం task_struct *p;
	పూర్ణాంకం retval;

	ఒకవేళ (unlikely(!param || pid < 0))
		తిరిగి -EINVAL;

	scoped_guard (rcu) {
		p = కనుగొను_process_by_pid(pid);
		ఒకవేళ (!p)
			తిరిగి -ESRCH;

		retval = security_task_getscheduler(p);
		ఒకవేళ (retval)
			తిరిగి retval;

		ఒకవేళ (task_has_rt_policy(p))
			lp.sched_ప్రాధాన్యత = p->rt_ప్రాధాన్యత;
	}

	/*
	 * This one might sleep, we cannot do it with a spinlock held ...
	 */
	తిరిగి copy_గమ్యం_user(param, &lp, పరిమాణం(*param)) ? -EFAULT : 0;
}

/**
 * sys_sched_getattr - similar to sched_getparam, but with sched_attr
 * @pid: the pid in question.
 * @uattr: structure containing the extended parameters.
 * @usize: sizeof(attr) for fwd/bwd comp.
 * @flags: for future extension.
 */
SYSCALL_DEFINE4(sched_getattr, pid_t, pid, నిర్మాణం sched_attr __user *, uattr,
		గుర్తులేని పూర్ణాంకం, usize, గుర్తులేని పూర్ణాంకం, flags)
{
	నిర్మాణం sched_attr kattr = { };
	నిర్మాణం task_struct *p;
	పూర్ణాంకం retval;

	ఒకవేళ (unlikely(!uattr || pid < 0 || usize > పుట_కొలత ||
		     usize < SCHED_ATTR_కొలత_VER0))
		తిరిగి -EINVAL;

	scoped_guard (rcu) {
		p = కనుగొను_process_by_pid(pid);
		ఒకవేళ (!p)
			తిరిగి -ESRCH;

		ఒకవేళ (flags) {
			ఒకవేళ (!task_has_dl_policy(p) ||
			    flags != SCHED_GETATTR_FLAG_DL_DYNAMIC)
				తిరిగి -EINVAL;
		}

		retval = security_task_getscheduler(p);
		ఒకవేళ (retval)
			తిరిగి retval;

		kattr.sched_policy = p->policy;
		ఒకవేళ (p->sched_reset_on_fork)
			kattr.sched_flags |= SCHED_FLAG_RESET_ON_FORK;
		get_params(p, &kattr, flags);
		kattr.sched_flags &= SCHED_FLAG_ALL;

#నిర్వచితమైతే CONFIG_UCLAMP_TASK
		/*
		 * This could race with another potential updater, but this is fine
		 * because it'll correctly read the old or the new value. We don't need
		 * to guarantee who wins the race as long as it doesn't return garbage.
		 */
		kattr.sched_util_min = p->uclamp_req[UCLAMP_MIN].విలువ;
		kattr.sched_util_max = p->uclamp_req[UCLAMP_MAX].విలువ;
#అయితే_ముగింపు
	}

	kattr.కొలత = min(usize, పరిమాణం(kattr));
	తిరిగి copy_struct_గమ్యం_user(uattr, usize, &kattr, పరిమాణం(kattr), ఖాళీ);
}

పూర్ణాంకం dl_task_check_affinity(నిర్మాణం task_struct *p, స్థిరరాశి నిర్మాణం cpumask *ముసుగు)
{
	/*
	 * If the task isn't a deadline task or admission control is
	 * disabled then we don't care about affinity changes.
	 */
	ఒకవేళ (!task_has_dl_policy(p) || !dl_bandwidth_enabled())
		తిరిగి 0;

	/*
	 * The special/sugov task isn't part of regular bandwidth/admission
	 * control so let userspace change affinities.
	 */
	ఒకవేళ (dl_entity_is_special(&p->dl))
		తిరిగి 0;

	/*
	 * Since bandwidth control happens on root_domain basis,
	 * if admission test is enabled, we only admit -deadline
	 * tasks allowed to run on all the CPUs in the task's
	 * root_domain.
	 */
	guard(rcu)();
	ఒకవేళ (!cpumask_subset(task_rq(p)->rd->పరిధి, ముసుగు))
		తిరిగి -EBUSY;

	తిరిగి 0;
}

పూర్ణాంకం __sched_setaffinity(నిర్మాణం task_struct *p, నిర్మాణం affinity_context *ctx)
{
	పూర్ణాంకం retval;
	cpumask_var_t cpus_allowed, new_ముసుగు;

	ఒకవేళ (!alloc_cpumask_var(&cpus_allowed, GFP_KERNEL))
		తిరిగి -ENOMEM;

	ఒకవేళ (!alloc_cpumask_var(&new_ముసుగు, GFP_KERNEL)) {
		retval = -ENOMEM;
		వెళ్ళు బహిర్గతం_free_cpus_allowed;
	}

	cpuset_cpus_allowed(p, cpus_allowed);
	cpumask_and(new_ముసుగు, ctx->new_ముసుగు, cpus_allowed);

	ctx->new_ముసుగు = new_ముసుగు;
	ctx->flags |= SCA_CHECK;

	retval = dl_task_check_affinity(p, new_ముసుగు);
	ఒకవేళ (retval)
		వెళ్ళు బహిర్గతం_free_new_ముసుగు;

	retval = __set_cpus_allowed_ptr(p, ctx);
	ఒకవేళ (retval)
		వెళ్ళు బహిర్గతం_free_new_ముసుగు;

	cpuset_cpus_allowed(p, cpus_allowed);
	ఒకవేళ (!cpumask_subset(new_ముసుగు, cpus_allowed)) {
		/*
		 * We must have raced with a concurrent cpuset update.
		 * Just reset the cpumask to the cpuset's cpus_allowed.
		 */
		cpumask_copy(new_ముసుగు, cpus_allowed);

		/*
		 * If SCA_USER is set, a 2nd call to __set_cpus_allowed_ptr()
		 * will restore the previous user_cpus_ptr value.
		 *
		 * In the unlikely event a previous user_cpus_ptr exists,
		 * we need to further restrict the mask to what is allowed
		 * by that old user_cpus_ptr.
		 */
		ఒకవేళ (unlikely((ctx->flags & SCA_USER) && ctx->user_ముసుగు)) {
			bool ఖాళీలు = !cpumask_and(new_ముసుగు, new_ముసుగు,
						  ctx->user_ముసుగు);

			ఒకవేళ (ఖాళీలు)
				cpumask_copy(new_ముసుగు, cpus_allowed);
		}
		__set_cpus_allowed_ptr(p, ctx);
		retval = -EINVAL;
	}

బహిర్గతం_free_new_ముసుగు:
	free_cpumask_var(new_ముసుగు);
బహిర్గతం_free_cpus_allowed:
	free_cpumask_var(cpus_allowed);
	తిరిగి retval;
}

దీర్ఘ sched_setaffinity(pid_t pid, స్థిరరాశి నిర్మాణం cpumask *లోపల_ముసుగు)
{
	నిర్మాణం affinity_context ac;
	నిర్మాణం cpumask *user_ముసుగు;
	పూర్ణాంకం retval;

	CLASS(కనుగొను_get_task, p)(pid);
	ఒకవేళ (!p)
		తిరిగి -ESRCH;

	ఒకవేళ (p->flags & PF_NO_SETAFFINITY)
		తిరిగి -EINVAL;

	ఒకవేళ (!check_same_owner(p)) {
		guard(rcu)();
		ఒకవేళ (!ns_capable(__task_cred(p)->user_ns, CAP_SYS_NICE))
			తిరిగి -EPERM;
	}

	retval = security_task_setscheduler(p);
	ఒకవేళ (retval)
		తిరిగి retval;

	/*
	 * With non-SMP configs, user_cpus_ptr/user_mask isn't used and
	 * alloc_user_cpus_ptr() returns NULL.
	 */
	user_ముసుగు = alloc_user_cpus_ptr(NUMA_NO_కణుపు);
	ఒకవేళ (user_ముసుగు) {
		cpumask_copy(user_ముసుగు, లోపల_ముసుగు);
	} లేకపోతే {
		తిరిగి -ENOMEM;
	}

	ac = (నిర్మాణం affinity_context){
		.new_ముసుగు  = లోపల_ముసుగు,
		.user_ముసుగు = user_ముసుగు,
		.flags     = SCA_USER,
	};

	retval = __sched_setaffinity(p, &ac);
	kfree(ac.user_ముసుగు);

	తిరిగి retval;
}

స్థిర పూర్ణాంకం get_user_cpu_ముసుగు(గుర్తులేని దీర్ఘ __user *user_ముసుగు_ptr, గుర్తులేని len,
			     నిర్మాణం cpumask *new_ముసుగు)
{
	ఒకవేళ (len < cpumask_కొలత())
		cpumask_clear(new_ముసుగు);
	లేకపోతే ఒకవేళ (len > cpumask_కొలత())
		len = cpumask_కొలత();

	తిరిగి copy_from_user(new_ముసుగు, user_ముసుగు_ptr, len) ? -EFAULT : 0;
}

/**
 * sys_sched_setaffinity - set the CPU affinity of a process
 * @pid: pid of the process
 * @len: length in bytes of the bitmask pointed to by user_mask_ptr
 * @user_mask_ptr: user-space pointer to the new CPU mask
 *
 * Return: 0 on success. An error code otherwise.
 */
SYSCALL_DEFINE3(sched_setaffinity, pid_t, pid, గుర్తులేని పూర్ణాంకం, len,
		గుర్తులేని దీర్ఘ __user *, user_ముసుగు_ptr)
{
	cpumask_var_t new_ముసుగు;
	పూర్ణాంకం retval;

	ఒకవేళ (!alloc_cpumask_var(&new_ముసుగు, GFP_KERNEL))
		తిరిగి -ENOMEM;

	retval = get_user_cpu_ముసుగు(user_ముసుగు_ptr, len, new_ముసుగు);
	ఒకవేళ (retval == 0)
		retval = sched_setaffinity(pid, new_ముసుగు);
	free_cpumask_var(new_ముసుగు);
	తిరిగి retval;
}

దీర్ఘ sched_getaffinity(pid_t pid, నిర్మాణం cpumask *ముసుగు)
{
	నిర్మాణం task_struct *p;
	పూర్ణాంకం retval;

	guard(rcu)();
	p = కనుగొను_process_by_pid(pid);
	ఒకవేళ (!p)
		తిరిగి -ESRCH;

	retval = security_task_getscheduler(p);
	ఒకవేళ (retval)
		తిరిగి retval;

	guard(raw_spinlock_irqsave)(&p->pi_తాళం);
	cpumask_and(ముసుగు, &p->cpus_ముసుగు, cpu_చురుకైన_ముసుగు);

	తిరిగి 0;
}

/**
 * sys_sched_getaffinity - get the CPU affinity of a process
 * @pid: pid of the process
 * @len: length in bytes of the bitmask pointed to by user_mask_ptr
 * @user_mask_ptr: user-space pointer to hold the current CPU mask
 *
 * Return: size of CPU mask copied to user_mask_ptr on success. An
 * error code otherwise.
 */
SYSCALL_DEFINE3(sched_getaffinity, pid_t, pid, గుర్తులేని పూర్ణాంకం, len,
		గుర్తులేని దీర్ఘ __user *, user_ముసుగు_ptr)
{
	పూర్ణాంకం ret;
	cpumask_var_t ముసుగు;

	ఒకవేళ ((len * బిట్లు_చొప్పున_BYTE) < nr_cpu_ids)
		తిరిగి -EINVAL;
	ఒకవేళ (len & (పరిమాణం(గుర్తులేని దీర్ఘ)-1))
		తిరిగి -EINVAL;

	ఒకవేళ (!zalloc_cpumask_var(&ముసుగు, GFP_KERNEL))
		తిరిగి -ENOMEM;

	ret = sched_getaffinity(pid, ముసుగు);
	ఒకవేళ (ret == 0) {
		గుర్తులేని పూర్ణాంకం retlen = min(len, cpumask_కొలత());

		ఒకవేళ (copy_గమ్యం_user(user_ముసుగు_ptr, cpumask_బిట్లు(ముసుగు), retlen))
			ret = -EFAULT;
		లేకపోతే
			ret = retlen;
	}
	free_cpumask_var(ముసుగు);

	తిరిగి ret;
}

స్థిర శూన్య do_sched_yield(శూన్య)
{
	నిర్మాణం rq_flags rf;
	నిర్మాణం rq *rq;

	rq = this_rq_తాళం_irq(&rf);

	schedstat_inc(rq->yld_లెక్క);
	rq->donor->sched_class->yield_task(rq);

	preempt_disable();
	rq_unlock_irq(rq, &rf);
	sched_preempt_enable_no_resched();

	కాలపట్టిక();
}

/**
 * sys_sched_yield - yield the current processor to other threads.
 *
 * This function yields the current CPU to other tasks. If there are no
 * other threads running on this CPU then this function will return.
 *
 * Return: 0.
 */
SYSCALL_DEFINE0(sched_yield)
{
	do_sched_yield();
	తిరిగి 0;
}

/**
 * yield - yield the current processor to other threads.
 *
 * Do not ever use this function, there's a 99% chance you're doing it wrong.
 *
 * The scheduler is at all times free to pick the calling task as the most
 * eligible task to run, if removing the yield() call from your code breaks
 * it, it's already broken.
 *
 * Typical broken usage is:
 *
 * while (!event)
 *	yield();
 *
 * where one assumes that yield() will let 'the other' process run that will
 * make event true. If the current task is a SCHED_FIFO task that will never
 * happen. Never use yield() as a progress guarantee!!
 *
 * If you want to use yield() to wait for something, use wait_event().
 * If you want to use yield() to be 'nice' for others, use cond_resched().
 * If you still want to use yield(), do not!
 */
శూన్య __sched yield(శూన్య)
{
	సమితి_current_state(TASK_RUNNING);
	do_sched_yield();
}
EXPORT_చిహ్నం(yield);

/**
 * yield_to - yield the current processor to another thread in
 * your thread group, or accelerate that thread toward the
 * processor it's on.
 * @p: target task
 * @preempt: whether task preemption is allowed or not
 *
 * It's the caller's job to ensure that the target task struct
 * can't go away on us before we can do any checks.
 *
 * Return:
 *	true (>0) if we indeed boosted the target task.
 *	false (0) if we failed to boost the target.
 *	-ESRCH if there's no task to yield to.
 */
పూర్ణాంకం __sched yield_గమ్యం(నిర్మాణం task_struct *p, bool preempt)
{
	నిర్మాణం task_struct *curr;
	నిర్మాణం rq *rq, *p_rq;
	పూర్ణాంకం yielded = 0;

	scoped_guard (raw_spinlock_irqsave, &p->pi_తాళం) {
		rq = this_rq();
		curr = rq->donor;

again:
		p_rq = task_rq(p);
		/*
		 * If we're the only runnable task on the rq and target rq also
		 * has only one task, there's absolutely no point in yielding.
		 */
		ఒకవేళ (rq->nr_running == 1 && p_rq->nr_running == 1)
			తిరిగి -ESRCH;

		guard(double_rq_తాళం)(rq, p_rq);
		ఒకవేళ (task_rq(p) != p_rq)
			వెళ్ళు again;

		ఒకవేళ (!curr->sched_class->yield_గమ్యం_task)
			తిరిగి 0;

		ఒకవేళ (curr->sched_class != p->sched_class)
			తిరిగి 0;

		ఒకవేళ (task_on_cpu(p_rq, p) || !task_is_running(p))
			తిరిగి 0;

		yielded = curr->sched_class->yield_గమ్యం_task(rq, p);
		ఒకవేళ (yielded) {
			schedstat_inc(rq->yld_లెక్క);
			/*
			 * Make p's CPU reschedule; pick_next_entity
			 * takes care of fairness.
			 */
			ఒకవేళ (preempt && rq != p_rq)
				resched_curr(p_rq);
		}
	}

	ఒకవేళ (yielded)
		కాలపట్టిక();

	తిరిగి yielded;
}
EXPORT_చిహ్నం_GPL(yield_గమ్యం);

/**
 * sys_sched_get_priority_max - return maximum RT priority.
 * @policy: scheduling class.
 *
 * Return: On success, this syscall returns the maximum
 * rt_priority that can be used by a given scheduling class.
 * On failure, a negative error code is returned.
 */
SYSCALL_DEFINE1(sched_get_ప్రాధాన్యత_max, పూర్ణాంకం, policy)
{
	పూర్ణాంకం ret = -EINVAL;

	ఎంపిక (policy) {
	స్థితి SCHED_FIFO:
	స్థితి SCHED_RR:
		ret = MAX_RT_PRIO-1;
		ఆపు;
	స్థితి SCHED_DEADLINE:
	స్థితి SCHED_సాధారణ:
	స్థితి SCHED_BATCH:
	స్థితి SCHED_IDLE:
	స్థితి SCHED_EXT:
		ret = 0;
		ఆపు;
	}
	తిరిగి ret;
}

/**
 * sys_sched_get_priority_min - return minimum RT priority.
 * @policy: scheduling class.
 *
 * Return: On success, this syscall returns the minimum
 * rt_priority that can be used by a given scheduling class.
 * On failure, a negative error code is returned.
 */
SYSCALL_DEFINE1(sched_get_ప్రాధాన్యత_min, పూర్ణాంకం, policy)
{
	పూర్ణాంకం ret = -EINVAL;

	ఎంపిక (policy) {
	స్థితి SCHED_FIFO:
	స్థితి SCHED_RR:
		ret = 1;
		ఆపు;
	స్థితి SCHED_DEADLINE:
	స్థితి SCHED_సాధారణ:
	స్థితి SCHED_BATCH:
	స్థితి SCHED_IDLE:
	స్థితి SCHED_EXT:
		ret = 0;
	}
	తిరిగి ret;
}

స్థిర పూర్ణాంకం sched_rr_get_interval(pid_t pid, నిర్మాణం timespec64 *t)
{
	గుర్తులేని పూర్ణాంకం సమయం_ముక్క = 0;
	పూర్ణాంకం retval;

	ఒకవేళ (pid < 0)
		తిరిగి -EINVAL;

	scoped_guard (rcu) {
		నిర్మాణం task_struct *p = కనుగొను_process_by_pid(pid);
		ఒకవేళ (!p)
			తిరిగి -ESRCH;

		retval = security_task_getscheduler(p);
		ఒకవేళ (retval)
			తిరిగి retval;

		scoped_guard (task_rq_తాళం_2, p) {
			నిర్మాణం rq *rq = scope.rq;
			ఒకవేళ (p->sched_class->get_rr_interval)
				సమయం_ముక్క = p->sched_class->get_rr_interval(rq, p);
		}
	}

	jiffies_గమ్యం_timespec64(సమయం_ముక్క, t);
	తిరిగి 0;
}

/**
 * sys_sched_rr_get_interval - return the default time-slice of a process.
 * @pid: pid of the process.
 * @interval: userspace pointer to the time-slice value.
 *
 * this syscall writes the default time-slice value of a given process
 * into the user-space timespec buffer. A value of '0' means infinity.
 *
 * Return: On success, 0 and the time-slice is in @interval. Otherwise,
 * an error code.
 */
SYSCALL_DEFINE2(sched_rr_get_interval, pid_t, pid,
		నిర్మాణం __kernel_timespec __user *, interval)
{
	నిర్మాణం timespec64 t;
	పూర్ణాంకం retval = sched_rr_get_interval(pid, &t);

	ఒకవేళ (retval == 0)
		retval = put_timespec64(&t, interval);

	తిరిగి retval;
}

#నిర్వచితమైతే CONFIG_COMPAT_32_బిట్టు_సమయం
SYSCALL_DEFINE2(sched_rr_get_interval_time32, pid_t, pid,
		నిర్మాణం old_timespec32 __user *, interval)
{
	నిర్మాణం timespec64 t;
	పూర్ణాంకం retval = sched_rr_get_interval(pid, &t);

	ఒకవేళ (retval == 0)
		retval = put_old_timespec32(&t, interval);
	తిరిగి retval;
}
#అయితే_ముగింపు
