// SPDX-License-Identifier: GPL-2.0-only
/*
 *  kernel/sched/syscalls.c
 *
 *  Core kernel scheduler syscalls related code
 *
 *  Copyright (C) 1991-2002  Linus Torvalds
 *  Copyright (C) 1998-2024  Ingo Molnar, Red Hat
 */
#ಅನ್ತರ್ಭಾವ <linux/sched.h>
#ಅನ್ತರ್ಭಾವ <linux/cpuset.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/debug.h>

#ಅನ್ತರ್ಭಾವ <uapi/linux/sched/types.h>

#ಅನ್ತರ್ಭಾವ "sched.h"
#ಅನ್ತರ್ಭಾವ "autogroup.h"

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ __normal_prio(ಪೂರ್ಣಾಂಕ policy, ಪೂರ್ಣಾಂಕ rt_prio, ಪೂರ್ಣಾಂಕ nice)
{
	ಪೂರ್ಣಾಂಕ prio;

	ಆದರೆ (dl_policy(policy))
		prio = MAX_DL_PRIO - 1;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (rt_policy(policy))
		prio = MAX_RT_PRIO - 1 - rt_prio;
	ಇಲ್ಲದಿದ್ದರೆ
		prio = NICE_ಗಮ್ಯ_PRIO(nice);

	ಹಿಂತಿರುಗಿ prio;
}

/*
 * Calculate the expected normal priority: i.e. priority
 * without taking RT-inheritance into account. Might be
 * boosted by interactivity modifiers. Changes upon fork,
 * setprio syscalls, and whenever the interactivity
 * estimator recalculates.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ ಸಾಮಾನ್ಯ_prio(ರಚನೆ task_struct *p)
{
	ಹಿಂತಿರುಗಿ __normal_prio(p->policy, p->rt_ಆದ್ಯತೆ, PRIO_ಗಮ್ಯ_NICE(p->static_prio));
}

/*
 * Calculate the current priority, i.e. the priority
 * taken into account by the scheduler. This value might
 * be boosted by RT tasks, or might be boosted by
 * interactivity modifiers. Will be RT if the task got
 * RT-boosted. If not then it returns p->normal_prio.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ effective_prio(ರಚನೆ task_struct *p)
{
	p->ಸಾಮಾನ್ಯ_prio = ಸಾಮಾನ್ಯ_prio(p);
	/*
	 * If we are RT tasks or we were boosted to RT priority,
	 * keep the priority unchanged. Otherwise, update priority
	 * to the normal priority:
	 */
	ಆದರೆ (!rt_or_dl_prio(p->prio))
		ಹಿಂತಿರುಗಿ p->ಸಾಮಾನ್ಯ_prio;
	ಹಿಂತಿರುಗಿ p->prio;
}

ಶೂನ್ಯ ಗಣ_user_nice(ರಚನೆ task_struct *p, ದೀರ್ಘ nice)
{
	ಪೂರ್ಣಾಂಕ old_prio;

	ಆದರೆ (task_nice(p) == nice || nice < MIN_NICE || nice > MAX_NICE)
		ಹಿಂತಿರುಗಿ;
	/*
	 * We have to be careful, if called from sys_setpriority(),
	 * the task might be in the middle of scheduling on another CPU.
	 */
	guard(task_rq_ಬೀಗ_2)(p);

	/*
	 * The RT priorities are set via sched_setscheduler(), but we still
	 * allow the 'normal' nice value to be set - but as expected
	 * it won't have any effect on scheduling until the task is
	 * SCHED_DEADLINE, SCHED_FIFO or SCHED_RR:
	 */
	ಆದರೆ (task_has_dl_policy(p) || task_has_rt_policy(p)) {
		p->static_prio = NICE_ಗಮ್ಯ_PRIO(nice);
		ಹಿಂತಿರುಗಿ;
	}

	scoped_guard (sched_change, p, ಸರತಿಯಿಂದ_ತೆಗೆ_SAVE) {
		p->static_prio = NICE_ಗಮ್ಯ_PRIO(nice);
		ಗಣ_ತುಂಬು_ತೂಕ(p, true);
		old_prio = p->prio;
		p->prio = effective_prio(p);
	}
}
EXPORT_ಚಿಹ್ನೆ(ಗಣ_user_nice);

/*
 * is_nice_reduction - check if nice value is an actual reduction
 *
 * Similar to can_nice() but does not perform a capability check.
 *
 * @p: task
 * @nice: nice value
 */
ಸ್ಥಿರ bool is_nice_reduction(ಸ್ಥಿರಾಂಕ ರಚನೆ task_struct *p, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ nice)
{
	/* Convert nice value [19,-20] to rlimit style value [1,40]: */
	ಪೂರ್ಣಾಂಕ nice_rlim = nice_ಗಮ್ಯ_rlimit(nice);

	ಹಿಂತಿರುಗಿ (nice_rlim <= task_rlimit(p, RLIMIT_NICE));
}

/*
 * can_nice - check if a task can reduce its nice value
 * @p: task
 * @nice: nice value
 */
ಪೂರ್ಣಾಂಕ can_nice(ಸ್ಥಿರಾಂಕ ರಚನೆ task_struct *p, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ nice)
{
	ಹಿಂತಿರುಗಿ is_nice_reduction(p, nice) || capable(CAP_SYS_NICE);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __ARCH_WANT_SYS_NICE

/*
 * sys_nice - change the priority of the current process.
 * @increment: priority increment
 *
 * sys_setpriority is a more generic, but much slower function that
 * does similar things.
 */
SYSCALL_DEFINE1(nice, ಪೂರ್ಣಾಂಕ, increment)
{
	ದೀರ್ಘ nice, retval;

	/*
	 * Setpriority might change our priority at the same moment.
	 * We don't have to worry. Conceptually one call occurs first
	 * and we have a single winner.
	 */
	increment = clamp(increment, -NICE_ಅಗಲ, NICE_ಅಗಲ);
	nice = task_nice(current) + increment;

	nice = clamp_val(nice, MIN_NICE, MAX_NICE);
	ಆದರೆ (increment < 0 && !can_nice(current, nice))
		ಹಿಂತಿರುಗಿ -EPERM;

	retval = security_task_setnice(current, nice);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	ಗಣ_user_nice(current, nice);
	ಹಿಂತಿರುಗಿ 0;
}

#ಆದರೆ_ಅಂತ್ಯ /* __ARCH_WANT_SYS_NICE */

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
ಪೂರ್ಣಾಂಕ task_prio(ಸ್ಥಿರಾಂಕ ರಚನೆ task_struct *p)
{
	ಹಿಂತಿರುಗಿ p->prio - MAX_RT_PRIO;
}

/**
 * idle_cpu - is a given CPU idle currently?
 * @cpu: the processor in question.
 *
 * Return: 1 if the CPU is currently idle. 0 otherwise.
 */
ಪೂರ್ಣಾಂಕ idle_cpu(ಪೂರ್ಣಾಂಕ cpu)
{
	ಹಿಂತಿರುಗಿ idle_rq(cpu_rq(cpu));
}

/**
 * idle_task - return the idle task for a given CPU.
 * @cpu: the processor in question.
 *
 * Return: The idle task for the CPU @cpu.
 */
ರಚನೆ task_struct *idle_task(ಪೂರ್ಣಾಂಕ cpu)
{
	ಹಿಂತಿರುಗಿ cpu_rq(cpu)->idle;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CORE
ಪೂರ್ಣಾಂಕ sched_core_idle_cpu(ಪೂರ್ಣಾಂಕ cpu)
{
	ರಚನೆ rq *rq = cpu_rq(cpu);

	ಆದರೆ (sched_core_enabled(rq) && rq->curr == rq->idle)
		ಹಿಂತಿರುಗಿ 1;

	ಹಿಂತಿರುಗಿ idle_cpu(cpu);
}
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_SCHED_CORE */

/**
 * find_process_by_pid - find a process with a matching PID value.
 * @pid: the pid in question.
 *
 * The task of @pid, if found. %NULL otherwise.
 */
ಸ್ಥಿರ ರಚನೆ task_struct *ಕಂಡುಹಿಡಿ_process_by_pid(pid_t pid)
{
	ಹಿಂತಿರುಗಿ pid ? ಕಂಡುಹಿಡಿ_task_by_vpid(pid) : current;
}

ಸ್ಥಿರ ರಚನೆ task_struct *ಕಂಡುಹಿಡಿ_get_task(pid_t pid)
{
	ರಚನೆ task_struct *p;
	guard(rcu)();

	p = ಕಂಡುಹಿಡಿ_process_by_pid(pid);
	ಆದರೆ (likely(p))
		get_task_struct(p);

	ಹಿಂತಿರುಗಿ p;
}

DEFINE_CLASS(ಕಂಡುಹಿಡಿ_get_task, ರಚನೆ task_struct *, ಆದರೆ (_T) put_task_struct(_T),
	     ಕಂಡುಹಿಡಿ_get_task(pid), pid_t pid)

/*
 * sched_setparam() passes in -1 for its policy, to let the functions
 * it calls know not to change it.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು SETPARAM_POLICY	-1

ಸ್ಥಿರ ಶೂನ್ಯ __setscheduler_params(ರಚನೆ task_struct *p,
		ಸ್ಥಿರಾಂಕ ರಚನೆ sched_attr *attr)
{
	ಪೂರ್ಣಾಂಕ policy = attr->sched_policy;

	ಆದರೆ (policy == SETPARAM_POLICY)
		policy = p->policy;

	p->policy = policy;

	ಆದರೆ (dl_policy(policy))
		__setparam_dl(p, attr);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (fair_policy(policy))
		__setparam_fair(p, attr);

	/* rt-policy tasks do not have a timerslack */
	ಆದರೆ (rt_or_dl_task_policy(p)) {
		p->timer_slack_ns = 0;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (p->timer_slack_ns == 0) {
		/* when switching back to non-rt policy, restore timerslack */
		p->timer_slack_ns = p->default_timer_slack_ns;
	}

	/*
	 * __sched_setscheduler() ensures attr->sched_priority == 0 when
	 * !rt_policy. Always setting this ensures that things like
	 * getparam()/getattr() don't report silly values for !rt tasks.
	 */
	p->rt_ಆದ್ಯತೆ = attr->sched_ಆದ್ಯತೆ;
	p->ಸಾಮಾನ್ಯ_prio = ಸಾಮಾನ್ಯ_prio(p);
	ಗಣ_ತುಂಬು_ತೂಕ(p, true);
}

/*
 * Check the target process has a UID that matches the current process's:
 */
ಸ್ಥಿರ bool check_same_owner(ರಚನೆ task_struct *p)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ cred *cred = current_cred(), *pcred;
	guard(rcu)();

	pcred = __task_cred(p);
	ಹಿಂತಿರುಗಿ (uid_eq(cred->euid, pcred->euid) ||
		uid_eq(cred->euid, pcred->uid));
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_RT_MUTEXES
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ __setscheduler_dl_pi(ಪೂರ್ಣಾಂಕ newprio, ಪೂರ್ಣಾಂಕ policy,
			      ರಚನೆ task_struct *p,
			      ರಚನೆ sched_change_ctx *scope)
{
	/*
	 * In case a DEADLINE task (either proper or boosted) gets
	 * setscheduled to a lower priority class, check if it neeeds to
	 * inherit parameters from a potential pi_task. In that case make
	 * sure replenishment happens with the next enqueue.
	 */

	ಆದರೆ (dl_prio(newprio) && !dl_policy(policy)) {
		ರಚನೆ task_struct *pi_task = rt_mutex_get_ತುದಿ_task(p);

		ಆದರೆ (pi_task) {
			p->dl.pi_se = pi_task->dl.pi_se;
			scope->flags |= ಸರತಿಗೆ_ಸೇರಿಸು_REPLENISH;
		}
	}
}
#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_RT_MUTEXES */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ __setscheduler_dl_pi(ಪೂರ್ಣಾಂಕ newprio, ಪೂರ್ಣಾಂಕ policy,
			      ರಚನೆ task_struct *p,
			      ರಚನೆ sched_change_ctx *scope)
{
}
#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_RT_MUTEXES */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_UCLAMP_TASK

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ uclamp_validate(ರಚನೆ task_struct *p,
			   ಸ್ಥಿರಾಂಕ ರಚನೆ sched_attr *attr)
{
	ಪೂರ್ಣಾಂಕ util_min = p->uclamp_req[UCLAMP_MIN].ಮೌಲ್ಯ;
	ಪೂರ್ಣಾಂಕ util_max = p->uclamp_req[UCLAMP_MAX].ಮೌಲ್ಯ;

	ಆದರೆ (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MIN) {
		util_min = attr->sched_util_min;

		ಆದರೆ (util_min + 1 > SCHED_ಸಾಮರ್ಥ್ಯ_SCALE + 1)
			ಹಿಂತಿರುಗಿ -EINVAL;
	}

	ಆದರೆ (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MAX) {
		util_max = attr->sched_util_max;

		ಆದರೆ (util_max + 1 > SCHED_ಸಾಮರ್ಥ್ಯ_SCALE + 1)
			ಹಿಂತಿರುಗಿ -EINVAL;
	}

	ಆದರೆ (util_min != -1 && util_max != -1 && util_min > util_max)
		ಹಿಂತಿರುಗಿ -EINVAL;

	/*
	 * We have valid uclamp attributes; make sure uclamp is enabled.
	 *
	 * We need to do that here, because enabling static branches is a
	 * blocking operation which obviously cannot be done while holding
	 * scheduler locks.
	 */
	sched_uclamp_enable();

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ bool uclamp_reset(ಸ್ಥಿರಾಂಕ ರಚನೆ sched_attr *attr,
			 ಎಣಿಕೆ uclamp_id clamp_id,
			 ರಚನೆ uclamp_se *uc_se)
{
	/* Reset on sched class change for a non user-defined clamp value. */
	ಆದರೆ (likely(!(attr->sched_flags & SCHED_FLAG_UTIL_CLAMP)) &&
	    !uc_se->user_defined)
		ಹಿಂತಿರುಗಿ true;

	/* Reset on sched_util_{min,max} == -1. */
	ಆದರೆ (clamp_id == UCLAMP_MIN &&
	    attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MIN &&
	    attr->sched_util_min == -1) {
		ಹಿಂತಿರುಗಿ true;
	}

	ಆದರೆ (clamp_id == UCLAMP_MAX &&
	    attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MAX &&
	    attr->sched_util_max == -1) {
		ಹಿಂತಿರುಗಿ true;
	}

	ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ ಶೂನ್ಯ __setscheduler_uclamp(ರಚನೆ task_struct *p,
				  ಸ್ಥಿರಾಂಕ ರಚನೆ sched_attr *attr)
{
	ಎಣಿಕೆ uclamp_id clamp_id;

	for_each_clamp_id(clamp_id) {
		ರಚನೆ uclamp_se *uc_se = &p->uclamp_req[clamp_id];
		ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಮೌಲ್ಯ;

		ಆದರೆ (!uclamp_reset(attr, clamp_id, uc_se))
			ಮುಂದುವರಿ;

		/*
		 * RT by default have a 100% boost value that could be modified
		 * at runtime.
		 */
		ಆದರೆ (unlikely(rt_task(p) && clamp_id == UCLAMP_MIN))
			ಮೌಲ್ಯ = sysctl_sched_uclamp_util_min_rt_default;
		ಇಲ್ಲದಿದ್ದರೆ
			ಮೌಲ್ಯ = uclamp_none(clamp_id);

		uclamp_se_ಗಣ(uc_se, ಮೌಲ್ಯ, false);

	}

	ಆದರೆ (likely(!(attr->sched_flags & SCHED_FLAG_UTIL_CLAMP)))
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MIN &&
	    attr->sched_util_min != -1) {
		uclamp_se_ಗಣ(&p->uclamp_req[UCLAMP_MIN],
			      attr->sched_util_min, true);
	}

	ಆದರೆ (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MAX &&
	    attr->sched_util_max != -1) {
		uclamp_se_ಗಣ(&p->uclamp_req[UCLAMP_MAX],
			      attr->sched_util_max, true);
	}
}

#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_UCLAMP_TASK: */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ uclamp_validate(ರಚನೆ task_struct *p,
				  ಸ್ಥಿರಾಂಕ ರಚನೆ sched_attr *attr)
{
	ಹಿಂತಿರುಗಿ -EOPNOTSUPP;
}
ಸ್ಥಿರ ಶೂನ್ಯ __setscheduler_uclamp(ರಚನೆ task_struct *p,
				  ಸ್ಥಿರಾಂಕ ರಚನೆ sched_attr *attr) { }
#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_UCLAMP_TASK */

/*
 * Allow unprivileged RT tasks to decrease priority.
 * Only issue a capable test if needed and only once to avoid an audit
 * event on permitted non-privileged operations:
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ user_check_sched_setscheduler(ರಚನೆ task_struct *p,
					 ಸ್ಥಿರಾಂಕ ರಚನೆ sched_attr *attr,
					 ಪೂರ್ಣಾಂಕ policy, ಪೂರ್ಣಾಂಕ reset_on_fork)
{
	ಆದರೆ (fair_policy(policy)) {
		ಆದರೆ (attr->sched_nice < task_nice(p) &&
		    !is_nice_reduction(p, attr->sched_nice))
			ಹೋಗು req_priv;
	}

	ಆದರೆ (rt_policy(policy)) {
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ rlim_rtprio = task_rlimit(p, RLIMIT_RTPRIO);

		/* Can't set/change the rt policy: */
		ಆದರೆ (policy != p->policy && !rlim_rtprio)
			ಹೋಗು req_priv;

		/* Can't increase priority: */
		ಆದರೆ (attr->sched_ಆದ್ಯತೆ > p->rt_ಆದ್ಯತೆ &&
		    attr->sched_ಆದ್ಯತೆ > rlim_rtprio)
			ಹೋಗು req_priv;
	}

	/*
	 * Can't set/change SCHED_DEADLINE policy at all for now
	 * (safest behavior); in the future we would like to allow
	 * unprivileged DL tasks to increase their relative deadline
	 * or reduce their runtime (both ways reducing utilization)
	 */
	ಆದರೆ (dl_policy(policy))
		ಹೋಗು req_priv;

	/*
	 * Treat SCHED_IDLE as nice 20. Only allow a switch to
	 * SCHED_NORMAL if the RLIMIT_NICE would normally permit it.
	 */
	ಆದರೆ (task_has_idle_policy(p) && !idle_policy(policy)) {
		ಆದರೆ (!is_nice_reduction(p, task_nice(p)))
			ಹೋಗು req_priv;
	}

	/* Can't change other user's priorities: */
	ಆದರೆ (!check_same_owner(p))
		ಹೋಗು req_priv;

	/* Normal users shall not reset the sched_reset_on_fork flag: */
	ಆದರೆ (p->sched_reset_on_fork && !reset_on_fork)
		ಹೋಗು req_priv;

	ಹಿಂತಿರುಗಿ 0;

req_priv:
	ಆದರೆ (!capable(CAP_SYS_NICE))
		ಹಿಂತಿರುಗಿ -EPERM;

	ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ __sched_setscheduler(ರಚನೆ task_struct *p,
			 ಸ್ಥಿರಾಂಕ ರಚನೆ sched_attr *attr,
			 bool user, bool pi)
{
	ಪೂರ್ಣಾಂಕ oldpolicy = -1, policy = attr->sched_policy;
	ಪೂರ್ಣಾಂಕ retval, oldprio, newprio;
	ಸ್ಥಿರಾಂಕ ರಚನೆ sched_class *prev_class, *ಮುಂದಿನ_class;
	ರಚನೆ balance_callback *ತಲೆ;
	ರಚನೆ rq_flags rf;
	ಪೂರ್ಣಾಂಕ reset_on_fork;
	ಪೂರ್ಣಾಂಕ ಸರತಿ_flags = ಸರತಿಯಿಂದ_ತೆಗೆ_SAVE | ಸರತಿಯಿಂದ_ತೆಗೆ_MOVE | ಸರತಿಯಿಂದ_ತೆಗೆ_NOCLOCK;
	ರಚನೆ rq *rq;
	bool cpuset_locked = false;

	/* The pi code expects interrupts enabled */
	BUG_ON(pi && ಒಳ_ಅಡಚಣೆ());
recheck:
	/* Double check policy once rq lock held: */
	ಆದರೆ (policy < 0) {
		reset_on_fork = p->sched_reset_on_fork;
		policy = oldpolicy = p->policy;
	} ಇಲ್ಲದಿದ್ದರೆ {
		reset_on_fork = !!(attr->sched_flags & SCHED_FLAG_RESET_ON_FORK);

		ಆದರೆ (!valid_policy(policy))
			ಹಿಂತಿರುಗಿ -EINVAL;
	}

	ಆದರೆ (attr->sched_flags & ~(SCHED_FLAG_ALL | SCHED_FLAG_SUGOV))
		ಹಿಂತಿರುಗಿ -EINVAL;

	/*
	 * Valid priorities for SCHED_FIFO and SCHED_RR are
	 * 1..MAX_RT_PRIO-1, valid priority for SCHED_NORMAL,
	 * SCHED_BATCH and SCHED_IDLE is 0.
	 */
	ಆದರೆ (attr->sched_ಆದ್ಯತೆ > MAX_RT_PRIO-1)
		ಹಿಂತಿರುಗಿ -EINVAL;
	ಆದರೆ ((dl_policy(policy) && !__checkparam_dl(attr)) ||
	    (rt_policy(policy) != (attr->sched_ಆದ್ಯತೆ != 0)))
		ಹಿಂತಿರುಗಿ -EINVAL;

	ಆದರೆ (user) {
		retval = user_check_sched_setscheduler(p, attr, policy, reset_on_fork);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;

		ಆದರೆ (attr->sched_flags & SCHED_FLAG_SUGOV)
			ಹಿಂತಿರುಗಿ -EINVAL;

		retval = security_task_setscheduler(p);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
	}

	/* Update task specific "requested" clamps */
	ಆದರೆ (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP) {
		retval = uclamp_validate(p, attr);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
	}

	/*
	 * SCHED_DEADLINE bandwidth accounting relies on stable cpusets
	 * information.
	 */
	ಆದರೆ (dl_policy(policy) || dl_policy(p->policy)) {
		cpuset_locked = true;
		cpuset_ಬೀಗ();
	}

	/*
	 * Make sure no PI-waiters arrive (or leave) while we are
	 * changing the priority of the task:
	 *
	 * To be able to change p->policy safely, the appropriate
	 * runqueue lock must be held.
	 */
	rq = task_rq_ಬೀಗ_2(p, &rf);
	update_rq_clock(rq);

	/*
	 * Changing the policy of the stop threads its a very bad idea:
	 */
	ಆದರೆ (p == rq->stop) {
		retval = -EINVAL;
		ಹೋಗು unlock;
	}

	retval = scx_check_setscheduler(p, policy);
	ಆದರೆ (retval)
		ಹೋಗು unlock;

	/*
	 * If not changing anything there's no need to proceed further,
	 * but store a possible modification of reset_on_fork.
	 */
	ಆದರೆ (unlikely(policy == p->policy)) {
		ಆದರೆ (fair_policy(policy) &&
		    (attr->sched_nice != task_nice(p) ||
		     (attr->sched_runtime != p->se.ತುಂಡು)))
			ಹೋಗು change;
		ಆದರೆ (rt_policy(policy) && attr->sched_ಆದ್ಯತೆ != p->rt_ಆದ್ಯತೆ)
			ಹೋಗು change;
		ಆದರೆ (dl_policy(policy) && dl_param_ಬದಲಾಯಿತು(p, attr))
			ಹೋಗು change;
		ಆದರೆ (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP)
			ಹೋಗು change;

		p->sched_reset_on_fork = reset_on_fork;
		retval = 0;
		ಹೋಗು unlock;
	}
change:

	ಆದರೆ (user) {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_RT_ಗುಂಪು_SCHED
		/*
		 * Do not allow real-time tasks into groups that have no runtime
		 * assigned.
		 */
		ಆದರೆ (rt_ಗುಂಪು_sched_enabled() &&
				rt_bandwidth_enabled() && rt_policy(policy) &&
				task_ಗುಂಪು(p)->rt_bandwidth.rt_runtime == 0 &&
				!task_ಗುಂಪು_is_autogroup(task_ಗುಂಪು(p))) {
			retval = -EPERM;
			ಹೋಗು unlock;
		}
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_RT_GROUP_SCHED */
		ಆದರೆ (dl_bandwidth_enabled() && dl_policy(policy) &&
				!(attr->sched_flags & SCHED_FLAG_SUGOV)) {
			cpumask_t *ಹರವು = rq->rd->ಹರವು;

			/*
			 * Don't allow tasks with an affinity mask smaller than
			 * the entire root_domain to become SCHED_DEADLINE. We
			 * will also fail if there's no bandwidth available.
			 */
			ಆದರೆ (!cpumask_subset(ಹರವು, p->cpus_ptr) ||
			    rq->rd->dl_bw.bw == 0) {
				retval = -EPERM;
				ಹೋಗು unlock;
			}
		}
	}

	/* Re-check policy now with rq lock held: */
	ಆದರೆ (unlikely(oldpolicy != -1 && oldpolicy != p->policy)) {
		policy = oldpolicy = -1;
		task_rq_unlock(rq, p, &rf);
		ಆದರೆ (cpuset_locked)
			cpuset_unlock();
		ಹೋಗು recheck;
	}

	/*
	 * If setscheduling to SCHED_DEADLINE (or changing the parameters
	 * of a SCHED_DEADLINE task) we need to check if enough bandwidth
	 * is available.
	 */
	ಆದರೆ ((dl_policy(policy) || dl_task(p)) && sched_dl_ಅತಿಪ್ರವಾಹ(p, policy, attr)) {
		retval = -EBUSY;
		ಹೋಗು unlock;
	}

	p->sched_reset_on_fork = reset_on_fork;
	oldprio = p->prio;

	newprio = __normal_prio(policy, attr->sched_ಆದ್ಯತೆ, attr->sched_nice);
	ಆದರೆ (pi) {
		/*
		 * Take priority boosted tasks into account. If the new
		 * effective priority is unchanged, we just store the new
		 * normal parameters and do not touch the scheduler class and
		 * the runqueue. This will be done when the task deboost
		 * itself.
		 */
		newprio = rt_effective_prio(p, newprio);
		ಆದರೆ (newprio == oldprio && !dl_prio(newprio))
			ಸರತಿ_flags &= ~ಸರತಿಯಿಂದ_ತೆಗೆ_MOVE;
	}

	prev_class = p->sched_class;
	ಮುಂದಿನ_class = __setscheduler_class(policy, newprio);

	ಆದರೆ (prev_class != ಮುಂದಿನ_class)
		ಸರತಿ_flags |= ಸರತಿಯಿಂದ_ತೆಗೆ_CLASS;

	scoped_guard (sched_change, p, ಸರತಿ_flags) {

		ಆದರೆ (!(attr->sched_flags & SCHED_FLAG_KEEP_PARAMS)) {
			__setscheduler_params(p, attr);
			p->sched_class = ಮುಂದಿನ_class;
			p->prio = newprio;
			__setscheduler_dl_pi(newprio, policy, p, scope);
		}
		__setscheduler_uclamp(p, attr);

		ಆದರೆ (scope->queued) {
			/*
			 * We enqueue to tail when the priority of a task is
			 * increased (user space view).
			 */
			ಆದರೆ (oldprio < p->prio)
				scope->flags |= ಸರತಿಗೆ_ಸೇರಿಸು_ತಲೆ;
		}
	}

	/* Avoid rq from going away on us: */
	preempt_disable();
	ತಲೆ = splice_balance_callbacks(rq);
	task_rq_unlock(rq, p, &rf);

	ಆದರೆ (pi) {
		ಆದರೆ (cpuset_locked)
			cpuset_unlock();
		rt_mutex_adjust_pi(p);
	}

	/* Run balance callbacks after we've adjusted the PI chain: */
	balance_callbacks(rq, ತಲೆ);
	preempt_enable();

	ಹಿಂತಿರುಗಿ 0;

unlock:
	task_rq_unlock(rq, p, &rf);
	ಆದರೆ (cpuset_locked)
		cpuset_unlock();
	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ _sched_setscheduler(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ policy,
			       ಸ್ಥಿರಾಂಕ ರಚನೆ sched_param *param, bool check)
{
	ರಚನೆ sched_attr attr = {
		.sched_policy   = policy,
		.sched_ಆದ್ಯತೆ = param->sched_ಆದ್ಯತೆ,
		.sched_nice	= PRIO_ಗಮ್ಯ_NICE(p->static_prio),
	};

	ಆದರೆ (p->se.custom_ತುಂಡು)
		attr.sched_runtime = p->se.ತುಂಡು;

	/* Fixup the legacy SCHED_RESET_ON_FORK hack. */
	ಆದರೆ ((policy != SETPARAM_POLICY) && (policy & SCHED_RESET_ON_FORK)) {
		attr.sched_flags |= SCHED_FLAG_RESET_ON_FORK;
		policy &= ~SCHED_RESET_ON_FORK;
		attr.sched_policy = policy;
	}

	ಹಿಂತಿರುಗಿ __sched_setscheduler(p, &attr, check, true);
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
ಪೂರ್ಣಾಂಕ sched_setscheduler(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ policy,
		       ಸ್ಥಿರಾಂಕ ರಚನೆ sched_param *param)
{
	ಹಿಂತಿರುಗಿ _sched_setscheduler(p, policy, param, true);
}

ಪೂರ್ಣಾಂಕ sched_setattr(ರಚನೆ task_struct *p, ಸ್ಥಿರಾಂಕ ರಚನೆ sched_attr *attr)
{
	ಹಿಂತಿರುಗಿ __sched_setscheduler(p, attr, true, true);
}

ಪೂರ್ಣಾಂಕ sched_setattr_nocheck(ರಚನೆ task_struct *p, ಸ್ಥಿರಾಂಕ ರಚನೆ sched_attr *attr)
{
	ಹಿಂತಿರುಗಿ __sched_setscheduler(p, attr, false, true);
}
EXPORT_ಚಿಹ್ನೆ_GPL(sched_setattr_nocheck);

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
ಪೂರ್ಣಾಂಕ sched_setscheduler_nocheck(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ policy,
			       ಸ್ಥಿರಾಂಕ ರಚನೆ sched_param *param)
{
	ಹಿಂತಿರುಗಿ _sched_setscheduler(p, policy, param, false);
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
ಶೂನ್ಯ sched_ಗಣ_fifo(ರಚನೆ task_struct *p)
{
	ರಚನೆ sched_param sp = { .sched_ಆದ್ಯತೆ = MAX_RT_PRIO / 2 };
	WARN_ON_ONCE(sched_setscheduler_nocheck(p, SCHED_FIFO, &sp) != 0);
}
EXPORT_ಚಿಹ್ನೆ_GPL(sched_ಗಣ_fifo);

/*
 * For when you don't much care about FIFO, but want to be above SCHED_NORMAL.
 */
ಶೂನ್ಯ sched_ಗಣ_fifo_ಕೆಳಗಿನ(ರಚನೆ task_struct *p)
{
	ರಚನೆ sched_param sp = { .sched_ಆದ್ಯತೆ = 1 };
	WARN_ON_ONCE(sched_setscheduler_nocheck(p, SCHED_FIFO, &sp) != 0);
}
EXPORT_ಚಿಹ್ನೆ_GPL(sched_ಗಣ_fifo_ಕೆಳಗಿನ);

/*
 * Used when the primary interrupt handler is forced into a thread, in addition
 * to the (always threaded) secondary handler.  The secondary handler gets a
 * slightly lower priority so that the primary handler can preempt it, thereby
 * emulating the behavior of a non-PREEMPT_RT system where the primary handler
 * runs in hard interrupt context.
 */
ಶೂನ್ಯ sched_ಗಣ_fifo_secondary(ರಚನೆ task_struct *p)
{
	ರಚನೆ sched_param sp = { .sched_ಆದ್ಯತೆ = MAX_RT_PRIO / 2 - 1 };
	WARN_ON_ONCE(sched_setscheduler_nocheck(p, SCHED_FIFO, &sp) != 0);
}

ಶೂನ್ಯ sched_ಗಣ_ಸಾಮಾನ್ಯ(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ nice)
{
	ರಚನೆ sched_attr attr = {
		.sched_policy = SCHED_ಸಾಮಾನ್ಯ,
		.sched_nice = nice,
	};
	WARN_ON_ONCE(sched_setattr_nocheck(p, &attr) != 0);
}
EXPORT_ಚಿಹ್ನೆ_GPL(sched_ಗಣ_ಸಾಮಾನ್ಯ);

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
do_sched_setscheduler(pid_t pid, ಪೂರ್ಣಾಂಕ policy, ರಚನೆ sched_param __user *param)
{
	ರಚನೆ sched_param lparam;

	ಆದರೆ (unlikely(!param || pid < 0))
		ಹಿಂತಿರುಗಿ -EINVAL;
	ಆದರೆ (copy_from_user(&lparam, param, ಗಾತ್ರ(ರಚನೆ sched_param)))
		ಹಿಂತಿರುಗಿ -EFAULT;

	CLASS(ಕಂಡುಹಿಡಿ_get_task, p)(pid);
	ಆದರೆ (!p)
		ಹಿಂತಿರುಗಿ -ESRCH;

	ಹಿಂತಿರುಗಿ sched_setscheduler(p, policy, &lparam);
}

/*
 * Mimics kernel/events/core.c perf_copy_attr().
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sched_copy_attr(ರಚನೆ sched_attr __user *uattr, ರಚನೆ sched_attr *attr)
{
	u32 ಗಾತ್ರ_2;
	ಪೂರ್ಣಾಂಕ ret;

	/* Zero the full structure, so that a short copy will be nice: */
	memset(attr, 0, ಗಾತ್ರ(*attr));

	ret = get_user(ಗಾತ್ರ_2, &uattr->ಗಾತ್ರ_2);
	ಆದರೆ (ret)
		ಹಿಂತಿರುಗಿ ret;

	/* ABI compatibility quirk: */
	ಆದರೆ (!ಗಾತ್ರ_2)
		ಗಾತ್ರ_2 = SCHED_ATTR_ಗಾತ್ರ_VER0;
	ಆದರೆ (ಗಾತ್ರ_2 < SCHED_ATTR_ಗಾತ್ರ_VER0 || ಗಾತ್ರ_2 > ಪುಟ_ಗಾತ್ರ)
		ಹೋಗು err_ಗಾತ್ರ;

	ret = copy_struct_from_user(attr, ಗಾತ್ರ(*attr), uattr, ಗಾತ್ರ_2);
	ಆದರೆ (ret) {
		ಆದರೆ (ret == -E2BIG)
			ಹೋಗು err_ಗಾತ್ರ;
		ಹಿಂತಿರುಗಿ ret;
	}

	ಆದರೆ ((attr->sched_flags & SCHED_FLAG_UTIL_CLAMP) &&
	    ಗಾತ್ರ_2 < SCHED_ATTR_ಗಾತ್ರ_VER1)
		ಹಿಂತಿರುಗಿ -EINVAL;

	/*
	 * XXX: Do we want to be lenient like existing syscalls; or do we want
	 * to be strict and return an error on out-of-bounds values?
	 */
	attr->sched_nice = clamp(attr->sched_nice, MIN_NICE, MAX_NICE);

	ಹಿಂತಿರುಗಿ 0;

err_ಗಾತ್ರ:
	put_user(ಗಾತ್ರ(*attr), &uattr->ಗಾತ್ರ_2);
	ಹಿಂತಿರುಗಿ -E2BIG;
}

ಸ್ಥಿರ ಶೂನ್ಯ get_params(ರಚನೆ task_struct *p, ರಚನೆ sched_attr *attr, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags)
{
	ಆದರೆ (task_has_dl_policy(p)) {
		__getparam_dl(p, attr, flags);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (task_has_rt_policy(p)) {
		attr->sched_ಆದ್ಯತೆ = p->rt_ಆದ್ಯತೆ;
	} ಇಲ್ಲದಿದ್ದರೆ {
		attr->sched_nice = task_nice(p);
		attr->sched_runtime = p->se.ತುಂಡು;
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
SYSCALL_DEFINE3(sched_setscheduler, pid_t, pid, ಪೂರ್ಣಾಂಕ, policy, ರಚನೆ sched_param __user *, param)
{
	ಆದರೆ (policy < 0)
		ಹಿಂತಿರುಗಿ -EINVAL;

	ಹಿಂತಿರುಗಿ do_sched_setscheduler(pid, policy, param);
}

/**
 * sys_sched_setparam - set/change the RT priority of a thread
 * @pid: the pid in question.
 * @param: structure containing the new RT priority.
 *
 * Return: 0 on success. An error code otherwise.
 */
SYSCALL_DEFINE2(sched_setparam, pid_t, pid, ರಚನೆ sched_param __user *, param)
{
	ಹಿಂತಿರುಗಿ do_sched_setscheduler(pid, SETPARAM_POLICY, param);
}

/**
 * sys_sched_setattr - same as above, but with extended sched_attr
 * @pid: the pid in question.
 * @uattr: structure containing the extended parameters.
 * @flags: for future extension.
 */
SYSCALL_DEFINE3(sched_setattr, pid_t, pid, ರಚನೆ sched_attr __user *, uattr,
			       ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ, flags)
{
	ರಚನೆ sched_attr attr;
	ಪೂರ್ಣಾಂಕ retval;

	ಆದರೆ (unlikely(!uattr || pid < 0 || flags))
		ಹಿಂತಿರುಗಿ -EINVAL;

	retval = sched_copy_attr(uattr, &attr);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	ಆದರೆ ((ಪೂರ್ಣಾಂಕ)attr.sched_policy < 0)
		ಹಿಂತಿರುಗಿ -EINVAL;
	ಆದರೆ (attr.sched_flags & SCHED_FLAG_KEEP_POLICY)
		attr.sched_policy = SETPARAM_POLICY;

	CLASS(ಕಂಡುಹಿಡಿ_get_task, p)(pid);
	ಆದರೆ (!p)
		ಹಿಂತಿರುಗಿ -ESRCH;

	ಆದರೆ (attr.sched_flags & SCHED_FLAG_KEEP_PARAMS)
		get_params(p, &attr, 0);

	ಹಿಂತಿರುಗಿ sched_setattr(p, &attr);
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
	ರಚನೆ task_struct *p;
	ಪೂರ್ಣಾಂಕ retval;

	ಆದರೆ (pid < 0)
		ಹಿಂತಿರುಗಿ -EINVAL;

	guard(rcu)();
	p = ಕಂಡುಹಿಡಿ_process_by_pid(pid);
	ಆದರೆ (!p)
		ಹಿಂತಿರುಗಿ -ESRCH;

	retval = security_task_getscheduler(p);
	ಆದರೆ (!retval) {
		retval = p->policy;
		ಆದರೆ (p->sched_reset_on_fork)
			retval |= SCHED_RESET_ON_FORK;
	}
	ಹಿಂತಿರುಗಿ retval;
}

/**
 * sys_sched_getparam - get the RT priority of a thread
 * @pid: the pid in question.
 * @param: structure containing the RT priority.
 *
 * Return: On success, 0 and the RT priority is in @param. Otherwise, an error
 * code.
 */
SYSCALL_DEFINE2(sched_getparam, pid_t, pid, ರಚನೆ sched_param __user *, param)
{
	ರಚನೆ sched_param lp = { .sched_ಆದ್ಯತೆ = 0 };
	ರಚನೆ task_struct *p;
	ಪೂರ್ಣಾಂಕ retval;

	ಆದರೆ (unlikely(!param || pid < 0))
		ಹಿಂತಿರುಗಿ -EINVAL;

	scoped_guard (rcu) {
		p = ಕಂಡುಹಿಡಿ_process_by_pid(pid);
		ಆದರೆ (!p)
			ಹಿಂತಿರುಗಿ -ESRCH;

		retval = security_task_getscheduler(p);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;

		ಆದರೆ (task_has_rt_policy(p))
			lp.sched_ಆದ್ಯತೆ = p->rt_ಆದ್ಯತೆ;
	}

	/*
	 * This one might sleep, we cannot do it with a spinlock held ...
	 */
	ಹಿಂತಿರುಗಿ copy_ಗಮ್ಯ_user(param, &lp, ಗಾತ್ರ(*param)) ? -EFAULT : 0;
}

/**
 * sys_sched_getattr - similar to sched_getparam, but with sched_attr
 * @pid: the pid in question.
 * @uattr: structure containing the extended parameters.
 * @usize: sizeof(attr) for fwd/bwd comp.
 * @flags: for future extension.
 */
SYSCALL_DEFINE4(sched_getattr, pid_t, pid, ರಚನೆ sched_attr __user *, uattr,
		ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ, usize, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ, flags)
{
	ರಚನೆ sched_attr kattr = { };
	ರಚನೆ task_struct *p;
	ಪೂರ್ಣಾಂಕ retval;

	ಆದರೆ (unlikely(!uattr || pid < 0 || usize > ಪುಟ_ಗಾತ್ರ ||
		     usize < SCHED_ATTR_ಗಾತ್ರ_VER0))
		ಹಿಂತಿರುಗಿ -EINVAL;

	scoped_guard (rcu) {
		p = ಕಂಡುಹಿಡಿ_process_by_pid(pid);
		ಆದರೆ (!p)
			ಹಿಂತಿರುಗಿ -ESRCH;

		ಆದರೆ (flags) {
			ಆದರೆ (!task_has_dl_policy(p) ||
			    flags != SCHED_GETATTR_FLAG_DL_DYNAMIC)
				ಹಿಂತಿರುಗಿ -EINVAL;
		}

		retval = security_task_getscheduler(p);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;

		kattr.sched_policy = p->policy;
		ಆದರೆ (p->sched_reset_on_fork)
			kattr.sched_flags |= SCHED_FLAG_RESET_ON_FORK;
		get_params(p, &kattr, flags);
		kattr.sched_flags &= SCHED_FLAG_ALL;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_UCLAMP_TASK
		/*
		 * This could race with another potential updater, but this is fine
		 * because it'll correctly read the old or the new value. We don't need
		 * to guarantee who wins the race as long as it doesn't return garbage.
		 */
		kattr.sched_util_min = p->uclamp_req[UCLAMP_MIN].ಮೌಲ್ಯ;
		kattr.sched_util_max = p->uclamp_req[UCLAMP_MAX].ಮೌಲ್ಯ;
#ಆದರೆ_ಅಂತ್ಯ
	}

	kattr.ಗಾತ್ರ_2 = min(usize, ಗಾತ್ರ(kattr));
	ಹಿಂತಿರುಗಿ copy_struct_ಗಮ್ಯ_user(uattr, usize, &kattr, ಗಾತ್ರ(kattr), ಖಾಲಿ);
}

ಪೂರ್ಣಾಂಕ dl_task_check_affinity(ರಚನೆ task_struct *p, ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *ಮುಖವಾಡ)
{
	/*
	 * If the task isn't a deadline task or admission control is
	 * disabled then we don't care about affinity changes.
	 */
	ಆದರೆ (!task_has_dl_policy(p) || !dl_bandwidth_enabled())
		ಹಿಂತಿರುಗಿ 0;

	/*
	 * The special/sugov task isn't part of regular bandwidth/admission
	 * control so let userspace change affinities.
	 */
	ಆದರೆ (dl_entity_is_special(&p->dl))
		ಹಿಂತಿರುಗಿ 0;

	/*
	 * Since bandwidth control happens on root_domain basis,
	 * if admission test is enabled, we only admit -deadline
	 * tasks allowed to run on all the CPUs in the task's
	 * root_domain.
	 */
	guard(rcu)();
	ಆದರೆ (!cpumask_subset(task_rq(p)->rd->ಹರವು, ಮುಖವಾಡ))
		ಹಿಂತಿರುಗಿ -EBUSY;

	ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ __sched_setaffinity(ರಚನೆ task_struct *p, ರಚನೆ affinity_context *ctx)
{
	ಪೂರ್ಣಾಂಕ retval;
	cpumask_var_t cpus_allowed, new_ಮುಖವಾಡ;

	ಆದರೆ (!alloc_cpumask_var(&cpus_allowed, GFP_KERNEL))
		ಹಿಂತಿರುಗಿ -ENOMEM;

	ಆದರೆ (!alloc_cpumask_var(&new_ಮುಖವಾಡ, GFP_KERNEL)) {
		retval = -ENOMEM;
		ಹೋಗು ಹೊರಹೋಗು_free_cpus_allowed;
	}

	cpuset_cpus_allowed(p, cpus_allowed);
	cpumask_and(new_ಮುಖವಾಡ, ctx->new_ಮುಖವಾಡ, cpus_allowed);

	ctx->new_ಮುಖವಾಡ = new_ಮುಖವಾಡ;
	ctx->flags |= SCA_CHECK;

	retval = dl_task_check_affinity(p, new_ಮುಖವಾಡ);
	ಆದರೆ (retval)
		ಹೋಗು ಹೊರಹೋಗು_free_new_ಮುಖವಾಡ;

	retval = __set_cpus_allowed_ptr(p, ctx);
	ಆದರೆ (retval)
		ಹೋಗು ಹೊರಹೋಗು_free_new_ಮುಖವಾಡ;

	cpuset_cpus_allowed(p, cpus_allowed);
	ಆದರೆ (!cpumask_subset(new_ಮುಖವಾಡ, cpus_allowed)) {
		/*
		 * We must have raced with a concurrent cpuset update.
		 * Just reset the cpumask to the cpuset's cpus_allowed.
		 */
		cpumask_copy(new_ಮುಖವಾಡ, cpus_allowed);

		/*
		 * If SCA_USER is set, a 2nd call to __set_cpus_allowed_ptr()
		 * will restore the previous user_cpus_ptr value.
		 *
		 * In the unlikely event a previous user_cpus_ptr exists,
		 * we need to further restrict the mask to what is allowed
		 * by that old user_cpus_ptr.
		 */
		ಆದರೆ (unlikely((ctx->flags & SCA_USER) && ctx->user_ಮುಖವಾಡ)) {
			bool ಖಾಲಿಗಳು = !cpumask_and(new_ಮುಖವಾಡ, new_ಮುಖವಾಡ,
						  ctx->user_ಮುಖವಾಡ);

			ಆದರೆ (ಖಾಲಿಗಳು)
				cpumask_copy(new_ಮುಖವಾಡ, cpus_allowed);
		}
		__set_cpus_allowed_ptr(p, ctx);
		retval = -EINVAL;
	}

ಹೊರಹೋಗು_free_new_ಮುಖವಾಡ:
	free_cpumask_var(new_ಮುಖವಾಡ);
ಹೊರಹೋಗು_free_cpus_allowed:
	free_cpumask_var(cpus_allowed);
	ಹಿಂತಿರುಗಿ retval;
}

ದೀರ್ಘ sched_setaffinity(pid_t pid, ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *ಒಳ_ಮುಖವಾಡ)
{
	ರಚನೆ affinity_context ac;
	ರಚನೆ cpumask *user_ಮುಖವಾಡ;
	ಪೂರ್ಣಾಂಕ retval;

	CLASS(ಕಂಡುಹಿಡಿ_get_task, p)(pid);
	ಆದರೆ (!p)
		ಹಿಂತಿರುಗಿ -ESRCH;

	ಆದರೆ (p->flags & PF_NO_SETAFFINITY)
		ಹಿಂತಿರುಗಿ -EINVAL;

	ಆದರೆ (!check_same_owner(p)) {
		guard(rcu)();
		ಆದರೆ (!ns_capable(__task_cred(p)->user_ns, CAP_SYS_NICE))
			ಹಿಂತಿರುಗಿ -EPERM;
	}

	retval = security_task_setscheduler(p);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	/*
	 * With non-SMP configs, user_cpus_ptr/user_mask isn't used and
	 * alloc_user_cpus_ptr() returns NULL.
	 */
	user_ಮುಖವಾಡ = alloc_user_cpus_ptr(NUMA_NO_ಗಂಟು);
	ಆದರೆ (user_ಮುಖವಾಡ) {
		cpumask_copy(user_ಮುಖವಾಡ, ಒಳ_ಮುಖವಾಡ);
	} ಇಲ್ಲದಿದ್ದರೆ {
		ಹಿಂತಿರುಗಿ -ENOMEM;
	}

	ac = (ರಚನೆ affinity_context){
		.new_ಮುಖವಾಡ  = ಒಳ_ಮುಖವಾಡ,
		.user_ಮುಖವಾಡ = user_ಮುಖವಾಡ,
		.flags     = SCA_USER,
	};

	retval = __sched_setaffinity(p, &ac);
	kfree(ac.user_ಮುಖವಾಡ);

	ಹಿಂತಿರುಗಿ retval;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ get_user_cpu_ಮುಖವಾಡ(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ __user *user_ಮುಖವಾಡ_ptr, ಚಿಹ್ನೆರಹಿತ len,
			     ರಚನೆ cpumask *new_ಮುಖವಾಡ)
{
	ಆದರೆ (len < cpumask_ಗಾತ್ರ())
		cpumask_clear(new_ಮುಖವಾಡ);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (len > cpumask_ಗಾತ್ರ())
		len = cpumask_ಗಾತ್ರ();

	ಹಿಂತಿರುಗಿ copy_from_user(new_ಮುಖವಾಡ, user_ಮುಖವಾಡ_ptr, len) ? -EFAULT : 0;
}

/**
 * sys_sched_setaffinity - set the CPU affinity of a process
 * @pid: pid of the process
 * @len: length in bytes of the bitmask pointed to by user_mask_ptr
 * @user_mask_ptr: user-space pointer to the new CPU mask
 *
 * Return: 0 on success. An error code otherwise.
 */
SYSCALL_DEFINE3(sched_setaffinity, pid_t, pid, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ, len,
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ __user *, user_ಮುಖವಾಡ_ptr)
{
	cpumask_var_t new_ಮುಖವಾಡ;
	ಪೂರ್ಣಾಂಕ retval;

	ಆದರೆ (!alloc_cpumask_var(&new_ಮುಖವಾಡ, GFP_KERNEL))
		ಹಿಂತಿರುಗಿ -ENOMEM;

	retval = get_user_cpu_ಮುಖವಾಡ(user_ಮುಖವಾಡ_ptr, len, new_ಮುಖವಾಡ);
	ಆದರೆ (retval == 0)
		retval = sched_setaffinity(pid, new_ಮುಖವಾಡ);
	free_cpumask_var(new_ಮುಖವಾಡ);
	ಹಿಂತಿರುಗಿ retval;
}

ದೀರ್ಘ sched_getaffinity(pid_t pid, ರಚನೆ cpumask *ಮುಖವಾಡ)
{
	ರಚನೆ task_struct *p;
	ಪೂರ್ಣಾಂಕ retval;

	guard(rcu)();
	p = ಕಂಡುಹಿಡಿ_process_by_pid(pid);
	ಆದರೆ (!p)
		ಹಿಂತಿರುಗಿ -ESRCH;

	retval = security_task_getscheduler(p);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	guard(raw_spinlock_irqsave)(&p->pi_ಬೀಗ);
	cpumask_and(ಮುಖವಾಡ, &p->cpus_ಮುಖವಾಡ, cpu_ಸಕ್ರಿಯ_ಮುಖವಾಡ);

	ಹಿಂತಿರುಗಿ 0;
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
SYSCALL_DEFINE3(sched_getaffinity, pid_t, pid, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ, len,
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ __user *, user_ಮುಖವಾಡ_ptr)
{
	ಪೂರ್ಣಾಂಕ ret;
	cpumask_var_t ಮುಖವಾಡ;

	ಆದರೆ ((len * ಬಿಟ್ಟುಗಳು_ಪ್ರತಿ_BYTE) < nr_cpu_ids)
		ಹಿಂತಿರುಗಿ -EINVAL;
	ಆದರೆ (len & (ಗಾತ್ರ(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ)-1))
		ಹಿಂತಿರುಗಿ -EINVAL;

	ಆದರೆ (!zalloc_cpumask_var(&ಮುಖವಾಡ, GFP_KERNEL))
		ಹಿಂತಿರುಗಿ -ENOMEM;

	ret = sched_getaffinity(pid, ಮುಖವಾಡ);
	ಆದರೆ (ret == 0) {
		ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ retlen = min(len, cpumask_ಗಾತ್ರ());

		ಆದರೆ (copy_ಗಮ್ಯ_user(user_ಮುಖವಾಡ_ptr, cpumask_ಬಿಟ್ಟುಗಳು(ಮುಖವಾಡ), retlen))
			ret = -EFAULT;
		ಇಲ್ಲದಿದ್ದರೆ
			ret = retlen;
	}
	free_cpumask_var(ಮುಖವಾಡ);

	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ do_sched_yield(ಶೂನ್ಯ)
{
	ರಚನೆ rq_flags rf;
	ರಚನೆ rq *rq;

	rq = this_rq_ಬೀಗ_irq(&rf);

	schedstat_inc(rq->yld_ಎಣಿಕೆ);
	rq->donor->sched_class->yield_task(rq);

	preempt_disable();
	rq_unlock_irq(rq, &rf);
	sched_preempt_enable_no_resched();

	ವೇಳಾಪಟ್ಟಿ();
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
	ಹಿಂತಿರುಗಿ 0;
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
ಶೂನ್ಯ __sched yield(ಶೂನ್ಯ)
{
	ಗಣ_current_state(TASK_RUNNING);
	do_sched_yield();
}
EXPORT_ಚಿಹ್ನೆ(yield);

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
ಪೂರ್ಣಾಂಕ __sched yield_ಗಮ್ಯ(ರಚನೆ task_struct *p, bool preempt)
{
	ರಚನೆ task_struct *curr;
	ರಚನೆ rq *rq, *p_rq;
	ಪೂರ್ಣಾಂಕ yielded = 0;

	scoped_guard (raw_spinlock_irqsave, &p->pi_ಬೀಗ) {
		rq = this_rq();
		curr = rq->donor;

again:
		p_rq = task_rq(p);
		/*
		 * If we're the only runnable task on the rq and target rq also
		 * has only one task, there's absolutely no point in yielding.
		 */
		ಆದರೆ (rq->nr_running == 1 && p_rq->nr_running == 1)
			ಹಿಂತಿರುಗಿ -ESRCH;

		guard(double_rq_ಬೀಗ)(rq, p_rq);
		ಆದರೆ (task_rq(p) != p_rq)
			ಹೋಗು again;

		ಆದರೆ (!curr->sched_class->yield_ಗಮ್ಯ_task)
			ಹಿಂತಿರುಗಿ 0;

		ಆದರೆ (curr->sched_class != p->sched_class)
			ಹಿಂತಿರುಗಿ 0;

		ಆದರೆ (task_on_cpu(p_rq, p) || !task_is_running(p))
			ಹಿಂತಿರುಗಿ 0;

		yielded = curr->sched_class->yield_ಗಮ್ಯ_task(rq, p);
		ಆದರೆ (yielded) {
			schedstat_inc(rq->yld_ಎಣಿಕೆ);
			/*
			 * Make p's CPU reschedule; pick_next_entity
			 * takes care of fairness.
			 */
			ಆದರೆ (preempt && rq != p_rq)
				resched_curr(p_rq);
		}
	}

	ಆದರೆ (yielded)
		ವೇಳಾಪಟ್ಟಿ();

	ಹಿಂತಿರುಗಿ yielded;
}
EXPORT_ಚಿಹ್ನೆ_GPL(yield_ಗಮ್ಯ);

/**
 * sys_sched_get_priority_max - return maximum RT priority.
 * @policy: scheduling class.
 *
 * Return: On success, this syscall returns the maximum
 * rt_priority that can be used by a given scheduling class.
 * On failure, a negative error code is returned.
 */
SYSCALL_DEFINE1(sched_get_ಆದ್ಯತೆ_max, ಪೂರ್ಣಾಂಕ, policy)
{
	ಪೂರ್ಣಾಂಕ ret = -EINVAL;

	ಆಯ್ಕೆ (policy) {
	ಸ್ಥಿತಿ SCHED_FIFO:
	ಸ್ಥಿತಿ SCHED_RR:
		ret = MAX_RT_PRIO-1;
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ SCHED_DEADLINE:
	ಸ್ಥಿತಿ SCHED_ಸಾಮಾನ್ಯ:
	ಸ್ಥಿತಿ SCHED_BATCH:
	ಸ್ಥಿತಿ SCHED_IDLE:
	ಸ್ಥಿತಿ SCHED_EXT:
		ret = 0;
		ನಿಲ್ಲಿಸು;
	}
	ಹಿಂತಿರುಗಿ ret;
}

/**
 * sys_sched_get_priority_min - return minimum RT priority.
 * @policy: scheduling class.
 *
 * Return: On success, this syscall returns the minimum
 * rt_priority that can be used by a given scheduling class.
 * On failure, a negative error code is returned.
 */
SYSCALL_DEFINE1(sched_get_ಆದ್ಯತೆ_min, ಪೂರ್ಣಾಂಕ, policy)
{
	ಪೂರ್ಣಾಂಕ ret = -EINVAL;

	ಆಯ್ಕೆ (policy) {
	ಸ್ಥಿತಿ SCHED_FIFO:
	ಸ್ಥಿತಿ SCHED_RR:
		ret = 1;
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ SCHED_DEADLINE:
	ಸ್ಥಿತಿ SCHED_ಸಾಮಾನ್ಯ:
	ಸ್ಥಿತಿ SCHED_BATCH:
	ಸ್ಥಿತಿ SCHED_IDLE:
	ಸ್ಥಿತಿ SCHED_EXT:
		ret = 0;
	}
	ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sched_rr_get_interval(pid_t pid, ರಚನೆ timespec64 *t)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಸಮಯ_ತುಂಡು = 0;
	ಪೂರ್ಣಾಂಕ retval;

	ಆದರೆ (pid < 0)
		ಹಿಂತಿರುಗಿ -EINVAL;

	scoped_guard (rcu) {
		ರಚನೆ task_struct *p = ಕಂಡುಹಿಡಿ_process_by_pid(pid);
		ಆದರೆ (!p)
			ಹಿಂತಿರುಗಿ -ESRCH;

		retval = security_task_getscheduler(p);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;

		scoped_guard (task_rq_ಬೀಗ_2, p) {
			ರಚನೆ rq *rq = scope.rq;
			ಆದರೆ (p->sched_class->get_rr_interval)
				ಸಮಯ_ತುಂಡು = p->sched_class->get_rr_interval(rq, p);
		}
	}

	jiffies_ಗಮ್ಯ_timespec64(ಸಮಯ_ತುಂಡು, t);
	ಹಿಂತಿರುಗಿ 0;
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
		ರಚನೆ __kernel_timespec __user *, interval)
{
	ರಚನೆ timespec64 t;
	ಪೂರ್ಣಾಂಕ retval = sched_rr_get_interval(pid, &t);

	ಆದರೆ (retval == 0)
		retval = put_timespec64(&t, interval);

	ಹಿಂತಿರುಗಿ retval;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_COMPAT_32_ಬಿಟ್_ಸಮಯ
SYSCALL_DEFINE2(sched_rr_get_interval_time32, pid_t, pid,
		ರಚನೆ old_timespec32 __user *, interval)
{
	ರಚನೆ timespec64 t;
	ಪೂರ್ಣಾಂಕ retval = sched_rr_get_interval(pid, &t);

	ಆದರೆ (retval == 0)
		retval = put_old_timespec32(&t, interval);
	ಹಿಂತಿರುಗಿ retval;
}
#ಆದರೆ_ಅಂತ್ಯ
