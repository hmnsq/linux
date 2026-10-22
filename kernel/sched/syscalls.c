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

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ __normal_prio(ಪೂರ್ಣಾಙ್ಕ policy, ಪೂರ್ಣಾಙ್ಕ rt_prio, ಪೂರ್ಣಾಙ್ಕ nice)
{
	ಪೂರ್ಣಾಙ್ಕ prio;

	ಯದಿ (dl_policy(policy))
		prio = MAX_DL_PRIO - 1;
	ಅನ್ಯಥಾ ಯದಿ (rt_policy(policy))
		prio = MAX_RT_PRIO - 1 - rt_prio;
	ಅನ್ಯಥಾ
		prio = NICE_TO_PRIO(nice);

	ಪ್ರತ್ಯಾಗಮ prio;
}

/*
 * Calculate the expected normal priority: i.e. priority
 * without taking RT-inheritance into account. Might be
 * boosted by interactivity modifiers. Changes upon fork,
 * setprio syscalls, and whenever the interactivity
 * estimator recalculates.
 */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ normal_prio(ಸಂರಚನಾ task_struct *p)
{
	ಪ್ರತ್ಯಾಗಮ __normal_prio(p->policy, p->rt_priority, PRIO_TO_NICE(p->static_prio));
}

/*
 * Calculate the current priority, i.e. the priority
 * taken into account by the scheduler. This value might
 * be boosted by RT tasks, or might be boosted by
 * interactivity modifiers. Will be RT if the task got
 * RT-boosted. If not then it returns p->normal_prio.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ effective_prio(ಸಂರಚನಾ task_struct *p)
{
	p->normal_prio = normal_prio(p);
	/*
	 * If we are RT tasks or we were boosted to RT priority,
	 * keep the priority unchanged. Otherwise, update priority
	 * to the normal priority:
	 */
	ಯದಿ (!rt_or_dl_prio(p->prio))
		ಪ್ರತ್ಯಾಗಮ p->normal_prio;
	ಪ್ರತ್ಯಾಗಮ p->prio;
}

ಶೂನ್ಯ set_user_nice(ಸಂರಚನಾ task_struct *p, long nice)
{
	ಪೂರ್ಣಾಙ್ಕ old_prio;

	ಯದಿ (task_nice(p) == nice || nice < MIN_NICE || nice > MAX_NICE)
		ಪ್ರತ್ಯಾಗಮ;
	/*
	 * We have to be careful, if called from sys_setpriority(),
	 * the task might be in the middle of scheduling on another CPU.
	 */
	guard(task_rq_lock)(p);

	/*
	 * The RT priorities are set via sched_setscheduler(), but we still
	 * allow the 'normal' nice value to be set - but as expected
	 * it won't have any effect on scheduling until the task is
	 * SCHED_DEADLINE, SCHED_FIFO or SCHED_RR:
	 */
	ಯದಿ (task_has_dl_policy(p) || task_has_rt_policy(p)) {
		p->static_prio = NICE_TO_PRIO(nice);
		ಪ್ರತ್ಯಾಗಮ;
	}

	scoped_guard (sched_change, p, DEQUEUE_SAVE) {
		p->static_prio = NICE_TO_PRIO(nice);
		set_load_weight(p, true);
		old_prio = p->prio;
		p->prio = effective_prio(p);
	}
}
EXPORT_ಚಿಹ್ನೆ(set_user_nice);

/*
 * is_nice_reduction - check if nice value is an actual reduction
 *
 * Similar to can_nice() but does not perform a capability check.
 *
 * @p: task
 * @nice: nice value
 */
ಸ್ಥಿರ bool is_nice_reduction(ಧ್ರುವ ಸಂರಚನಾ task_struct *p, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ nice)
{
	/* Convert nice value [19,-20] to rlimit style value [1,40]: */
	ಪೂರ್ಣಾಙ್ಕ nice_rlim = nice_to_rlimit(nice);

	ಪ್ರತ್ಯಾಗಮ (nice_rlim <= task_rlimit(p, RLIMIT_NICE));
}

/*
 * can_nice - check if a task can reduce its nice value
 * @p: task
 * @nice: nice value
 */
ಪೂರ್ಣಾಙ್ಕ can_nice(ಧ್ರುವ ಸಂರಚನಾ task_struct *p, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ nice)
{
	ಪ್ರತ್ಯಾಗಮ is_nice_reduction(p, nice) || capable(CAP_SYS_NICE);
}

#ifdef __ARCH_WANT_SYS_NICE

/*
 * sys_nice - change the priority of the current process.
 * @increment: priority increment
 *
 * sys_setpriority is a more generic, but much slower function that
 * does similar things.
 */
SYSCALL_DEFINE1(nice, ಪೂರ್ಣಾಙ್ಕ, increment)
{
	long nice, retval;

	/*
	 * Setpriority might change our priority at the same moment.
	 * We don't have to worry. Conceptually one call occurs first
	 * and we have a single winner.
	 */
	increment = clamp(increment, -NICE_WIDTH, NICE_WIDTH);
	nice = task_nice(current) + increment;

	nice = clamp_val(nice, MIN_NICE, MAX_NICE);
	ಯದಿ (increment < 0 && !can_nice(current, nice))
		ಪ್ರತ್ಯಾಗಮ -EPERM;

	retval = security_task_setnice(current, nice);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;

	set_user_nice(current, nice);
	ಪ್ರತ್ಯಾಗಮ 0;
}

#endif /* __ARCH_WANT_SYS_NICE */

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
ಪೂರ್ಣಾಙ್ಕ task_prio(ಧ್ರುವ ಸಂರಚನಾ task_struct *p)
{
	ಪ್ರತ್ಯಾಗಮ p->prio - MAX_RT_PRIO;
}

/**
 * idle_cpu - is a given CPU idle currently?
 * @cpu: the processor in question.
 *
 * Return: 1 if the CPU is currently idle. 0 otherwise.
 */
ಪೂರ್ಣಾಙ್ಕ idle_cpu(ಪೂರ್ಣಾಙ್ಕ cpu)
{
	ಪ್ರತ್ಯಾಗಮ idle_rq(cpu_rq(cpu));
}

/**
 * idle_task - return the idle task for a given CPU.
 * @cpu: the processor in question.
 *
 * Return: The idle task for the CPU @cpu.
 */
ಸಂರಚನಾ task_struct *idle_task(ಪೂರ್ಣಾಙ್ಕ cpu)
{
	ಪ್ರತ್ಯಾಗಮ cpu_rq(cpu)->idle;
}

#ifdef CONFIG_SCHED_CORE
ಪೂರ್ಣಾಙ್ಕ sched_core_idle_cpu(ಪೂರ್ಣಾಙ್ಕ cpu)
{
	ಸಂರಚನಾ rq *rq = cpu_rq(cpu);

	ಯದಿ (sched_core_enabled(rq) && rq->curr == rq->idle)
		ಪ್ರತ್ಯಾಗಮ 1;

	ಪ್ರತ್ಯಾಗಮ idle_cpu(cpu);
}
#endif /* CONFIG_SCHED_CORE */

/**
 * find_process_by_pid - find a process with a matching PID value.
 * @pid: the pid in question.
 *
 * The task of @pid, if found. %NULL otherwise.
 */
ಸ್ಥಿರ ಸಂರಚನಾ task_struct *find_process_by_pid(pid_t pid)
{
	ಪ್ರತ್ಯಾಗಮ pid ? find_task_by_vpid(pid) : current;
}

ಸ್ಥಿರ ಸಂರಚನಾ task_struct *find_get_task(pid_t pid)
{
	ಸಂರಚನಾ task_struct *p;
	guard(rcu)();

	p = find_process_by_pid(pid);
	ಯದಿ (likely(p))
		get_task_struct(p);

	ಪ್ರತ್ಯಾಗಮ p;
}

DEFINE_CLASS(find_get_task, ಸಂರಚನಾ task_struct *, ಯದಿ (_T) put_task_struct(_T),
	     find_get_task(pid), pid_t pid)

/*
 * sched_setparam() passes in -1 for its policy, to let the functions
 * it calls know not to change it.
 */
#define SETPARAM_POLICY	-1

ಸ್ಥಿರ ಶೂನ್ಯ __setscheduler_params(ಸಂರಚನಾ task_struct *p,
		ಧ್ರುವ ಸಂರಚನಾ sched_attr *attr)
{
	ಪೂರ್ಣಾಙ್ಕ policy = attr->sched_policy;

	ಯದಿ (policy == SETPARAM_POLICY)
		policy = p->policy;

	p->policy = policy;

	ಯದಿ (dl_policy(policy))
		__setparam_dl(p, attr);
	ಅನ್ಯಥಾ ಯದಿ (fair_policy(policy))
		__setparam_fair(p, attr);

	/* rt-policy tasks do not have a timerslack */
	ಯದಿ (rt_or_dl_task_policy(p)) {
		p->timer_slack_ns = 0;
	} ಅನ್ಯಥಾ ಯದಿ (p->timer_slack_ns == 0) {
		/* when switching back to non-rt policy, restore timerslack */
		p->timer_slack_ns = p->default_timer_slack_ns;
	}

	/*
	 * __sched_setscheduler() ensures attr->sched_priority == 0 when
	 * !rt_policy. Always setting this ensures that things like
	 * getparam()/getattr() don't report silly values for !rt tasks.
	 */
	p->rt_priority = attr->sched_priority;
	p->normal_prio = normal_prio(p);
	set_load_weight(p, true);
}

/*
 * Check the target process has a UID that matches the current process's:
 */
ಸ್ಥಿರ bool check_same_owner(ಸಂರಚನಾ task_struct *p)
{
	ಧ್ರುವ ಸಂರಚನಾ cred *cred = current_cred(), *pcred;
	guard(rcu)();

	pcred = __task_cred(p);
	ಪ್ರತ್ಯಾಗಮ (uid_eq(cred->euid, pcred->euid) ||
		uid_eq(cred->euid, pcred->uid));
}

#ifdef CONFIG_RT_MUTEXES
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ __setscheduler_dl_pi(ಪೂರ್ಣಾಙ್ಕ newprio, ಪೂರ್ಣಾಙ್ಕ policy,
			      ಸಂರಚನಾ task_struct *p,
			      ಸಂರಚನಾ sched_change_ctx *scope)
{
	/*
	 * In case a DEADLINE task (either proper or boosted) gets
	 * setscheduled to a lower priority class, check if it neeeds to
	 * inherit parameters from a potential pi_task. In that case make
	 * sure replenishment happens with the next enqueue.
	 */

	ಯದಿ (dl_prio(newprio) && !dl_policy(policy)) {
		ಸಂರಚನಾ task_struct *pi_task = rt_mutex_get_ತುದಿ_task(p);

		ಯದಿ (pi_task) {
			p->dl.pi_se = pi_task->dl.pi_se;
			scope->flags |= ENQUEUE_REPLENISH;
		}
	}
}
#ಅನ್ಯಥಾ /* !CONFIG_RT_MUTEXES */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ __setscheduler_dl_pi(ಪೂರ್ಣಾಙ್ಕ newprio, ಪೂರ್ಣಾಙ್ಕ policy,
			      ಸಂರಚನಾ task_struct *p,
			      ಸಂರಚನಾ sched_change_ctx *scope)
{
}
#endif /* !CONFIG_RT_MUTEXES */

#ifdef CONFIG_UCLAMP_TASK

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ uclamp_validate(ಸಂರಚನಾ task_struct *p,
			   ಧ್ರುವ ಸಂರಚನಾ sched_attr *attr)
{
	ಪೂರ್ಣಾಙ್ಕ util_min = p->uclamp_req[UCLAMP_MIN].ಮೌಲ್ಯ;
	ಪೂರ್ಣಾಙ್ಕ util_max = p->uclamp_req[UCLAMP_MAX].ಮೌಲ್ಯ;

	ಯದಿ (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MIN) {
		util_min = attr->sched_util_min;

		ಯದಿ (util_min + 1 > SCHED_CAPACITY_SCALE + 1)
			ಪ್ರತ್ಯಾಗಮ -EINVAL;
	}

	ಯದಿ (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MAX) {
		util_max = attr->sched_util_max;

		ಯದಿ (util_max + 1 > SCHED_CAPACITY_SCALE + 1)
			ಪ್ರತ್ಯಾಗಮ -EINVAL;
	}

	ಯದಿ (util_min != -1 && util_max != -1 && util_min > util_max)
		ಪ್ರತ್ಯಾಗಮ -EINVAL;

	/*
	 * We have valid uclamp attributes; make sure uclamp is enabled.
	 *
	 * We need to do that here, because enabling static branches is a
	 * blocking operation which obviously cannot be done while holding
	 * scheduler locks.
	 */
	sched_uclamp_enable();

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ bool uclamp_reset(ಧ್ರುವ ಸಂರಚನಾ sched_attr *attr,
			 ಗಣನಾ uclamp_id clamp_id,
			 ಸಂರಚನಾ uclamp_se *uc_se)
{
	/* Reset on sched class change for a non user-defined clamp value. */
	ಯದಿ (likely(!(attr->sched_flags & SCHED_FLAG_UTIL_CLAMP)) &&
	    !uc_se->user_defined)
		ಪ್ರತ್ಯಾಗಮ true;

	/* Reset on sched_util_{min,max} == -1. */
	ಯದಿ (clamp_id == UCLAMP_MIN &&
	    attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MIN &&
	    attr->sched_util_min == -1) {
		ಪ್ರತ್ಯಾಗಮ true;
	}

	ಯದಿ (clamp_id == UCLAMP_MAX &&
	    attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MAX &&
	    attr->sched_util_max == -1) {
		ಪ್ರತ್ಯಾಗಮ true;
	}

	ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ ಶೂನ್ಯ __setscheduler_uclamp(ಸಂರಚನಾ task_struct *p,
				  ಧ್ರುವ ಸಂರಚನಾ sched_attr *attr)
{
	ಗಣನಾ uclamp_id clamp_id;

	for_each_clamp_id(clamp_id) {
		ಸಂರಚನಾ uclamp_se *uc_se = &p->uclamp_req[clamp_id];
		unsigned ಪೂರ್ಣಾಙ್ಕ ಮೌಲ್ಯ;

		ಯದಿ (!uclamp_reset(attr, clamp_id, uc_se))
			ಅನುವರ್ತ;

		/*
		 * RT by default have a 100% boost value that could be modified
		 * at runtime.
		 */
		ಯದಿ (unlikely(rt_task(p) && clamp_id == UCLAMP_MIN))
			ಮೌಲ್ಯ = sysctl_sched_uclamp_util_min_rt_default;
		ಅನ್ಯಥಾ
			ಮೌಲ್ಯ = uclamp_none(clamp_id);

		uclamp_se_set(uc_se, ಮೌಲ್ಯ, false);

	}

	ಯದಿ (likely(!(attr->sched_flags & SCHED_FLAG_UTIL_CLAMP)))
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MIN &&
	    attr->sched_util_min != -1) {
		uclamp_se_set(&p->uclamp_req[UCLAMP_MIN],
			      attr->sched_util_min, true);
	}

	ಯದಿ (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MAX &&
	    attr->sched_util_max != -1) {
		uclamp_se_set(&p->uclamp_req[UCLAMP_MAX],
			      attr->sched_util_max, true);
	}
}

#ಅನ್ಯಥಾ /* !CONFIG_UCLAMP_TASK: */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ uclamp_validate(ಸಂರಚನಾ task_struct *p,
				  ಧ್ರುವ ಸಂರಚನಾ sched_attr *attr)
{
	ಪ್ರತ್ಯಾಗಮ -EOPNOTSUPP;
}
ಸ್ಥಿರ ಶೂನ್ಯ __setscheduler_uclamp(ಸಂರಚನಾ task_struct *p,
				  ಧ್ರುವ ಸಂರಚನಾ sched_attr *attr) { }
#endif /* !CONFIG_UCLAMP_TASK */

/*
 * Allow unprivileged RT tasks to decrease priority.
 * Only issue a capable test if needed and only once to avoid an audit
 * event on permitted non-privileged operations:
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ user_check_sched_setscheduler(ಸಂರಚನಾ task_struct *p,
					 ಧ್ರುವ ಸಂರಚನಾ sched_attr *attr,
					 ಪೂರ್ಣಾಙ್ಕ policy, ಪೂರ್ಣಾಙ್ಕ reset_on_fork)
{
	ಯದಿ (fair_policy(policy)) {
		ಯದಿ (attr->sched_nice < task_nice(p) &&
		    !is_nice_reduction(p, attr->sched_nice))
			ಗಚ್ಛ req_priv;
	}

	ಯದಿ (rt_policy(policy)) {
		unsigned long rlim_rtprio = task_rlimit(p, RLIMIT_RTPRIO);

		/* Can't set/change the rt policy: */
		ಯದಿ (policy != p->policy && !rlim_rtprio)
			ಗಚ್ಛ req_priv;

		/* Can't increase priority: */
		ಯದಿ (attr->sched_priority > p->rt_priority &&
		    attr->sched_priority > rlim_rtprio)
			ಗಚ್ಛ req_priv;
	}

	/*
	 * Can't set/change SCHED_DEADLINE policy at all for now
	 * (safest behavior); in the future we would like to allow
	 * unprivileged DL tasks to increase their relative deadline
	 * or reduce their runtime (both ways reducing utilization)
	 */
	ಯದಿ (dl_policy(policy))
		ಗಚ್ಛ req_priv;

	/*
	 * Treat SCHED_IDLE as nice 20. Only allow a switch to
	 * SCHED_NORMAL if the RLIMIT_NICE would normally permit it.
	 */
	ಯದಿ (task_has_idle_policy(p) && !idle_policy(policy)) {
		ಯದಿ (!is_nice_reduction(p, task_nice(p)))
			ಗಚ್ಛ req_priv;
	}

	/* Can't change other user's priorities: */
	ಯದಿ (!check_same_owner(p))
		ಗಚ್ಛ req_priv;

	/* Normal users shall not reset the sched_reset_on_fork flag: */
	ಯದಿ (p->sched_reset_on_fork && !reset_on_fork)
		ಗಚ್ಛ req_priv;

	ಪ್ರತ್ಯಾಗಮ 0;

req_priv:
	ಯದಿ (!capable(CAP_SYS_NICE))
		ಪ್ರತ್ಯಾಗಮ -EPERM;

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ __sched_setscheduler(ಸಂರಚನಾ task_struct *p,
			 ಧ್ರುವ ಸಂರಚನಾ sched_attr *attr,
			 bool user, bool pi)
{
	ಪೂರ್ಣಾಙ್ಕ oldpolicy = -1, policy = attr->sched_policy;
	ಪೂರ್ಣಾಙ್ಕ retval, oldprio, newprio;
	ಧ್ರುವ ಸಂರಚನಾ sched_class *prev_class, *next_class;
	ಸಂರಚನಾ balance_callback *head;
	ಸಂರಚನಾ rq_flags rf;
	ಪೂರ್ಣಾಙ್ಕ reset_on_fork;
	ಪೂರ್ಣಾಙ್ಕ queue_flags = DEQUEUE_SAVE | DEQUEUE_MOVE | DEQUEUE_NOCLOCK;
	ಸಂರಚನಾ rq *rq;
	bool cpuset_locked = false;

	/* The pi code expects interrupts enabled */
	BUG_ON(pi && in_interrupt());
recheck:
	/* Double check policy once rq lock held: */
	ಯದಿ (policy < 0) {
		reset_on_fork = p->sched_reset_on_fork;
		policy = oldpolicy = p->policy;
	} ಅನ್ಯಥಾ {
		reset_on_fork = !!(attr->sched_flags & SCHED_FLAG_RESET_ON_FORK);

		ಯದಿ (!valid_policy(policy))
			ಪ್ರತ್ಯಾಗಮ -EINVAL;
	}

	ಯದಿ (attr->sched_flags & ~(SCHED_FLAG_ALL | SCHED_FLAG_SUGOV))
		ಪ್ರತ್ಯಾಗಮ -EINVAL;

	/*
	 * Valid priorities for SCHED_FIFO and SCHED_RR are
	 * 1..MAX_RT_PRIO-1, valid priority for SCHED_NORMAL,
	 * SCHED_BATCH and SCHED_IDLE is 0.
	 */
	ಯದಿ (attr->sched_priority > MAX_RT_PRIO-1)
		ಪ್ರತ್ಯಾಗಮ -EINVAL;
	ಯದಿ ((dl_policy(policy) && !__checkparam_dl(attr)) ||
	    (rt_policy(policy) != (attr->sched_priority != 0)))
		ಪ್ರತ್ಯಾಗಮ -EINVAL;

	ಯದಿ (user) {
		retval = user_check_sched_setscheduler(p, attr, policy, reset_on_fork);
		ಯದಿ (retval)
			ಪ್ರತ್ಯಾಗಮ retval;

		ಯದಿ (attr->sched_flags & SCHED_FLAG_SUGOV)
			ಪ್ರತ್ಯಾಗಮ -EINVAL;

		retval = security_task_setscheduler(p);
		ಯದಿ (retval)
			ಪ್ರತ್ಯಾಗಮ retval;
	}

	/* Update task specific "requested" clamps */
	ಯದಿ (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP) {
		retval = uclamp_validate(p, attr);
		ಯದಿ (retval)
			ಪ್ರತ್ಯಾಗಮ retval;
	}

	/*
	 * SCHED_DEADLINE bandwidth accounting relies on stable cpusets
	 * information.
	 */
	ಯದಿ (dl_policy(policy) || dl_policy(p->policy)) {
		cpuset_locked = true;
		cpuset_lock();
	}

	/*
	 * Make sure no PI-waiters arrive (or leave) while we are
	 * changing the priority of the task:
	 *
	 * To be able to change p->policy safely, the appropriate
	 * runqueue lock must be held.
	 */
	rq = task_rq_lock(p, &rf);
	update_rq_clock(rq);

	/*
	 * Changing the policy of the stop threads its a very bad idea:
	 */
	ಯದಿ (p == rq->stop) {
		retval = -EINVAL;
		ಗಚ್ಛ unlock;
	}

	retval = scx_check_setscheduler(p, policy);
	ಯದಿ (retval)
		ಗಚ್ಛ unlock;

	/*
	 * If not changing anything there's no need to proceed further,
	 * but store a possible modification of reset_on_fork.
	 */
	ಯದಿ (unlikely(policy == p->policy)) {
		ಯದಿ (fair_policy(policy) &&
		    (attr->sched_nice != task_nice(p) ||
		     (attr->sched_runtime != p->se.slice)))
			ಗಚ್ಛ change;
		ಯದಿ (rt_policy(policy) && attr->sched_priority != p->rt_priority)
			ಗಚ್ಛ change;
		ಯದಿ (dl_policy(policy) && dl_param_changed(p, attr))
			ಗಚ್ಛ change;
		ಯದಿ (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP)
			ಗಚ್ಛ change;

		p->sched_reset_on_fork = reset_on_fork;
		retval = 0;
		ಗಚ್ಛ unlock;
	}
change:

	ಯದಿ (user) {
#ifdef CONFIG_RT_GROUP_SCHED
		/*
		 * Do not allow real-time tasks into groups that have no runtime
		 * assigned.
		 */
		ಯದಿ (rt_group_sched_enabled() &&
				rt_bandwidth_enabled() && rt_policy(policy) &&
				task_group(p)->rt_bandwidth.rt_runtime == 0 &&
				!task_group_is_autogroup(task_group(p))) {
			retval = -EPERM;
			ಗಚ್ಛ unlock;
		}
#endif /* CONFIG_RT_GROUP_SCHED */
		ಯದಿ (dl_bandwidth_enabled() && dl_policy(policy) &&
				!(attr->sched_flags & SCHED_FLAG_SUGOV)) {
			cpumask_t *span = rq->rd->span;

			/*
			 * Don't allow tasks with an affinity mask smaller than
			 * the entire root_domain to become SCHED_DEADLINE. We
			 * will also fail if there's no bandwidth available.
			 */
			ಯದಿ (!cpumask_subset(span, p->cpus_ptr) ||
			    rq->rd->dl_bw.bw == 0) {
				retval = -EPERM;
				ಗಚ್ಛ unlock;
			}
		}
	}

	/* Re-check policy now with rq lock held: */
	ಯದಿ (unlikely(oldpolicy != -1 && oldpolicy != p->policy)) {
		policy = oldpolicy = -1;
		task_rq_unlock(rq, p, &rf);
		ಯದಿ (cpuset_locked)
			cpuset_unlock();
		ಗಚ್ಛ recheck;
	}

	/*
	 * If setscheduling to SCHED_DEADLINE (or changing the parameters
	 * of a SCHED_DEADLINE task) we need to check if enough bandwidth
	 * is available.
	 */
	ಯದಿ ((dl_policy(policy) || dl_task(p)) && sched_dl_overflow(p, policy, attr)) {
		retval = -EBUSY;
		ಗಚ್ಛ unlock;
	}

	p->sched_reset_on_fork = reset_on_fork;
	oldprio = p->prio;

	newprio = __normal_prio(policy, attr->sched_priority, attr->sched_nice);
	ಯದಿ (pi) {
		/*
		 * Take priority boosted tasks into account. If the new
		 * effective priority is unchanged, we just store the new
		 * normal parameters and do not touch the scheduler class and
		 * the runqueue. This will be done when the task deboost
		 * itself.
		 */
		newprio = rt_effective_prio(p, newprio);
		ಯದಿ (newprio == oldprio && !dl_prio(newprio))
			queue_flags &= ~DEQUEUE_MOVE;
	}

	prev_class = p->sched_class;
	next_class = __setscheduler_class(policy, newprio);

	ಯದಿ (prev_class != next_class)
		queue_flags |= DEQUEUE_CLASS;

	scoped_guard (sched_change, p, queue_flags) {

		ಯದಿ (!(attr->sched_flags & SCHED_FLAG_KEEP_PARAMS)) {
			__setscheduler_params(p, attr);
			p->sched_class = next_class;
			p->prio = newprio;
			__setscheduler_dl_pi(newprio, policy, p, scope);
		}
		__setscheduler_uclamp(p, attr);

		ಯದಿ (scope->queued) {
			/*
			 * We enqueue to tail when the priority of a task is
			 * increased (user space view).
			 */
			ಯದಿ (oldprio < p->prio)
				scope->flags |= ENQUEUE_HEAD;
		}
	}

	/* Avoid rq from going away on us: */
	preempt_disable();
	head = splice_balance_callbacks(rq);
	task_rq_unlock(rq, p, &rf);

	ಯದಿ (pi) {
		ಯದಿ (cpuset_locked)
			cpuset_unlock();
		rt_mutex_adjust_pi(p);
	}

	/* Run balance callbacks after we've adjusted the PI chain: */
	balance_callbacks(rq, head);
	preempt_enable();

	ಪ್ರತ್ಯಾಗಮ 0;

unlock:
	task_rq_unlock(rq, p, &rf);
	ಯದಿ (cpuset_locked)
		cpuset_unlock();
	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ _sched_setscheduler(ಸಂರಚನಾ task_struct *p, ಪೂರ್ಣಾಙ್ಕ policy,
			       ಧ್ರುವ ಸಂರಚನಾ sched_param *param, bool check)
{
	ಸಂರಚನಾ sched_attr attr = {
		.sched_policy   = policy,
		.sched_priority = param->sched_priority,
		.sched_nice	= PRIO_TO_NICE(p->static_prio),
	};

	ಯದಿ (p->se.custom_slice)
		attr.sched_runtime = p->se.slice;

	/* Fixup the legacy SCHED_RESET_ON_FORK hack. */
	ಯದಿ ((policy != SETPARAM_POLICY) && (policy & SCHED_RESET_ON_FORK)) {
		attr.sched_flags |= SCHED_FLAG_RESET_ON_FORK;
		policy &= ~SCHED_RESET_ON_FORK;
		attr.sched_policy = policy;
	}

	ಪ್ರತ್ಯಾಗಮ __sched_setscheduler(p, &attr, check, true);
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
ಪೂರ್ಣಾಙ್ಕ sched_setscheduler(ಸಂರಚನಾ task_struct *p, ಪೂರ್ಣಾಙ್ಕ policy,
		       ಧ್ರುವ ಸಂರಚನಾ sched_param *param)
{
	ಪ್ರತ್ಯಾಗಮ _sched_setscheduler(p, policy, param, true);
}

ಪೂರ್ಣಾಙ್ಕ sched_setattr(ಸಂರಚನಾ task_struct *p, ಧ್ರುವ ಸಂರಚನಾ sched_attr *attr)
{
	ಪ್ರತ್ಯಾಗಮ __sched_setscheduler(p, attr, true, true);
}

ಪೂರ್ಣಾಙ್ಕ sched_setattr_nocheck(ಸಂರಚನಾ task_struct *p, ಧ್ರುವ ಸಂರಚನಾ sched_attr *attr)
{
	ಪ್ರತ್ಯಾಗಮ __sched_setscheduler(p, attr, false, true);
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
ಪೂರ್ಣಾಙ್ಕ sched_setscheduler_nocheck(ಸಂರಚನಾ task_struct *p, ಪೂರ್ಣಾಙ್ಕ policy,
			       ಧ್ರುವ ಸಂರಚನಾ sched_param *param)
{
	ಪ್ರತ್ಯಾಗಮ _sched_setscheduler(p, policy, param, false);
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
ಶೂನ್ಯ sched_set_fifo(ಸಂರಚನಾ task_struct *p)
{
	ಸಂರಚನಾ sched_param sp = { .sched_priority = MAX_RT_PRIO / 2 };
	WARN_ON_ONCE(sched_setscheduler_nocheck(p, SCHED_FIFO, &sp) != 0);
}
EXPORT_ಚಿಹ್ನೆ_GPL(sched_set_fifo);

/*
 * For when you don't much care about FIFO, but want to be above SCHED_NORMAL.
 */
ಶೂನ್ಯ sched_set_fifo_ಕೆಳಗಿನ(ಸಂರಚನಾ task_struct *p)
{
	ಸಂರಚನಾ sched_param sp = { .sched_priority = 1 };
	WARN_ON_ONCE(sched_setscheduler_nocheck(p, SCHED_FIFO, &sp) != 0);
}
EXPORT_ಚಿಹ್ನೆ_GPL(sched_set_fifo_ಕೆಳಗಿನ);

/*
 * Used when the primary interrupt handler is forced into a thread, in addition
 * to the (always threaded) secondary handler.  The secondary handler gets a
 * slightly lower priority so that the primary handler can preempt it, thereby
 * emulating the behavior of a non-PREEMPT_RT system where the primary handler
 * runs in hard interrupt context.
 */
ಶೂನ್ಯ sched_set_fifo_secondary(ಸಂರಚನಾ task_struct *p)
{
	ಸಂರಚನಾ sched_param sp = { .sched_priority = MAX_RT_PRIO / 2 - 1 };
	WARN_ON_ONCE(sched_setscheduler_nocheck(p, SCHED_FIFO, &sp) != 0);
}

ಶೂನ್ಯ sched_set_normal(ಸಂರಚನಾ task_struct *p, ಪೂರ್ಣಾಙ್ಕ nice)
{
	ಸಂರಚನಾ sched_attr attr = {
		.sched_policy = SCHED_NORMAL,
		.sched_nice = nice,
	};
	WARN_ON_ONCE(sched_setattr_nocheck(p, &attr) != 0);
}
EXPORT_ಚಿಹ್ನೆ_GPL(sched_set_normal);

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
do_sched_setscheduler(pid_t pid, ಪೂರ್ಣಾಙ್ಕ policy, ಸಂರಚನಾ sched_param __user *param)
{
	ಸಂರಚನಾ sched_param lparam;

	ಯದಿ (unlikely(!param || pid < 0))
		ಪ್ರತ್ಯಾಗಮ -EINVAL;
	ಯದಿ (copy_from_user(&lparam, param, ಪರಿಮಾಣ(ಸಂರಚನಾ sched_param)))
		ಪ್ರತ್ಯಾಗಮ -EFAULT;

	CLASS(find_get_task, p)(pid);
	ಯದಿ (!p)
		ಪ್ರತ್ಯಾಗಮ -ESRCH;

	ಪ್ರತ್ಯಾಗಮ sched_setscheduler(p, policy, &lparam);
}

/*
 * Mimics kernel/events/core.c perf_copy_attr().
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sched_copy_attr(ಸಂರಚನಾ sched_attr __user *uattr, ಸಂರಚನಾ sched_attr *attr)
{
	u32 size;
	ಪೂರ್ಣಾಙ್ಕ ret;

	/* Zero the full structure, so that a short copy will be nice: */
	memset(attr, 0, ಪರಿಮಾಣ(*attr));

	ret = get_user(size, &uattr->size);
	ಯದಿ (ret)
		ಪ್ರತ್ಯಾಗಮ ret;

	/* ABI compatibility quirk: */
	ಯದಿ (!size)
		size = SCHED_ATTR_SIZE_VER0;
	ಯದಿ (size < SCHED_ATTR_SIZE_VER0 || size > PAGE_SIZE)
		ಗಚ್ಛ err_size;

	ret = copy_struct_from_user(attr, ಪರಿಮಾಣ(*attr), uattr, size);
	ಯದಿ (ret) {
		ಯದಿ (ret == -E2BIG)
			ಗಚ್ಛ err_size;
		ಪ್ರತ್ಯಾಗಮ ret;
	}

	ಯದಿ ((attr->sched_flags & SCHED_FLAG_UTIL_CLAMP) &&
	    size < SCHED_ATTR_SIZE_VER1)
		ಪ್ರತ್ಯಾಗಮ -EINVAL;

	/*
	 * XXX: Do we want to be lenient like existing syscalls; or do we want
	 * to be strict and return an error on out-of-bounds values?
	 */
	attr->sched_nice = clamp(attr->sched_nice, MIN_NICE, MAX_NICE);

	ಪ್ರತ್ಯಾಗಮ 0;

err_size:
	put_user(ಪರಿಮಾಣ(*attr), &uattr->size);
	ಪ್ರತ್ಯಾಗಮ -E2BIG;
}

ಸ್ಥಿರ ಶೂನ್ಯ get_params(ಸಂರಚನಾ task_struct *p, ಸಂರಚನಾ sched_attr *attr, unsigned ಪೂರ್ಣಾಙ್ಕ flags)
{
	ಯದಿ (task_has_dl_policy(p)) {
		__getparam_dl(p, attr, flags);
	} ಅನ್ಯಥಾ ಯದಿ (task_has_rt_policy(p)) {
		attr->sched_priority = p->rt_priority;
	} ಅನ್ಯಥಾ {
		attr->sched_nice = task_nice(p);
		attr->sched_runtime = p->se.slice;
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
SYSCALL_DEFINE3(sched_setscheduler, pid_t, pid, ಪೂರ್ಣಾಙ್ಕ, policy, ಸಂರಚನಾ sched_param __user *, param)
{
	ಯದಿ (policy < 0)
		ಪ್ರತ್ಯಾಗಮ -EINVAL;

	ಪ್ರತ್ಯಾಗಮ do_sched_setscheduler(pid, policy, param);
}

/**
 * sys_sched_setparam - set/change the RT priority of a thread
 * @pid: the pid in question.
 * @param: structure containing the new RT priority.
 *
 * Return: 0 on success. An error code otherwise.
 */
SYSCALL_DEFINE2(sched_setparam, pid_t, pid, ಸಂರಚನಾ sched_param __user *, param)
{
	ಪ್ರತ್ಯಾಗಮ do_sched_setscheduler(pid, SETPARAM_POLICY, param);
}

/**
 * sys_sched_setattr - same as above, but with extended sched_attr
 * @pid: the pid in question.
 * @uattr: structure containing the extended parameters.
 * @flags: for future extension.
 */
SYSCALL_DEFINE3(sched_setattr, pid_t, pid, ಸಂರಚನಾ sched_attr __user *, uattr,
			       unsigned ಪೂರ್ಣಾಙ್ಕ, flags)
{
	ಸಂರಚನಾ sched_attr attr;
	ಪೂರ್ಣಾಙ್ಕ retval;

	ಯದಿ (unlikely(!uattr || pid < 0 || flags))
		ಪ್ರತ್ಯಾಗಮ -EINVAL;

	retval = sched_copy_attr(uattr, &attr);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;

	ಯದಿ ((ಪೂರ್ಣಾಙ್ಕ)attr.sched_policy < 0)
		ಪ್ರತ್ಯಾಗಮ -EINVAL;
	ಯದಿ (attr.sched_flags & SCHED_FLAG_KEEP_POLICY)
		attr.sched_policy = SETPARAM_POLICY;

	CLASS(find_get_task, p)(pid);
	ಯದಿ (!p)
		ಪ್ರತ್ಯಾಗಮ -ESRCH;

	ಯದಿ (attr.sched_flags & SCHED_FLAG_KEEP_PARAMS)
		get_params(p, &attr, 0);

	ಪ್ರತ್ಯಾಗಮ sched_setattr(p, &attr);
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
	ಸಂರಚನಾ task_struct *p;
	ಪೂರ್ಣಾಙ್ಕ retval;

	ಯದಿ (pid < 0)
		ಪ್ರತ್ಯಾಗಮ -EINVAL;

	guard(rcu)();
	p = find_process_by_pid(pid);
	ಯದಿ (!p)
		ಪ್ರತ್ಯಾಗಮ -ESRCH;

	retval = security_task_getscheduler(p);
	ಯದಿ (!retval) {
		retval = p->policy;
		ಯದಿ (p->sched_reset_on_fork)
			retval |= SCHED_RESET_ON_FORK;
	}
	ಪ್ರತ್ಯಾಗಮ retval;
}

/**
 * sys_sched_getparam - get the RT priority of a thread
 * @pid: the pid in question.
 * @param: structure containing the RT priority.
 *
 * Return: On success, 0 and the RT priority is in @param. Otherwise, an error
 * code.
 */
SYSCALL_DEFINE2(sched_getparam, pid_t, pid, ಸಂರಚನಾ sched_param __user *, param)
{
	ಸಂರಚನಾ sched_param lp = { .sched_priority = 0 };
	ಸಂರಚನಾ task_struct *p;
	ಪೂರ್ಣಾಙ್ಕ retval;

	ಯದಿ (unlikely(!param || pid < 0))
		ಪ್ರತ್ಯಾಗಮ -EINVAL;

	scoped_guard (rcu) {
		p = find_process_by_pid(pid);
		ಯದಿ (!p)
			ಪ್ರತ್ಯಾಗಮ -ESRCH;

		retval = security_task_getscheduler(p);
		ಯದಿ (retval)
			ಪ್ರತ್ಯಾಗಮ retval;

		ಯದಿ (task_has_rt_policy(p))
			lp.sched_priority = p->rt_priority;
	}

	/*
	 * This one might sleep, we cannot do it with a spinlock held ...
	 */
	ಪ್ರತ್ಯಾಗಮ copy_to_user(param, &lp, ಪರಿಮಾಣ(*param)) ? -EFAULT : 0;
}

/**
 * sys_sched_getattr - similar to sched_getparam, but with sched_attr
 * @pid: the pid in question.
 * @uattr: structure containing the extended parameters.
 * @usize: sizeof(attr) for fwd/bwd comp.
 * @flags: for future extension.
 */
SYSCALL_DEFINE4(sched_getattr, pid_t, pid, ಸಂರಚನಾ sched_attr __user *, uattr,
		unsigned ಪೂರ್ಣಾಙ್ಕ, usize, unsigned ಪೂರ್ಣಾಙ್ಕ, flags)
{
	ಸಂರಚನಾ sched_attr kattr = { };
	ಸಂರಚನಾ task_struct *p;
	ಪೂರ್ಣಾಙ್ಕ retval;

	ಯದಿ (unlikely(!uattr || pid < 0 || usize > PAGE_SIZE ||
		     usize < SCHED_ATTR_SIZE_VER0))
		ಪ್ರತ್ಯಾಗಮ -EINVAL;

	scoped_guard (rcu) {
		p = find_process_by_pid(pid);
		ಯದಿ (!p)
			ಪ್ರತ್ಯಾಗಮ -ESRCH;

		ಯದಿ (flags) {
			ಯದಿ (!task_has_dl_policy(p) ||
			    flags != SCHED_GETATTR_FLAG_DL_DYNAMIC)
				ಪ್ರತ್ಯಾಗಮ -EINVAL;
		}

		retval = security_task_getscheduler(p);
		ಯದಿ (retval)
			ಪ್ರತ್ಯಾಗಮ retval;

		kattr.sched_policy = p->policy;
		ಯದಿ (p->sched_reset_on_fork)
			kattr.sched_flags |= SCHED_FLAG_RESET_ON_FORK;
		get_params(p, &kattr, flags);
		kattr.sched_flags &= SCHED_FLAG_ALL;

#ifdef CONFIG_UCLAMP_TASK
		/*
		 * This could race with another potential updater, but this is fine
		 * because it'll correctly read the old or the new value. We don't need
		 * to guarantee who wins the race as long as it doesn't return garbage.
		 */
		kattr.sched_util_min = p->uclamp_req[UCLAMP_MIN].ಮೌಲ್ಯ;
		kattr.sched_util_max = p->uclamp_req[UCLAMP_MAX].ಮೌಲ್ಯ;
#endif
	}

	kattr.size = min(usize, ಪರಿಮಾಣ(kattr));
	ಪ್ರತ್ಯಾಗಮ copy_struct_to_user(uattr, usize, &kattr, ಪರಿಮಾಣ(kattr), ರಿಕ್ತ);
}

ಪೂರ್ಣಾಙ್ಕ dl_task_check_affinity(ಸಂರಚನಾ task_struct *p, ಧ್ರುವ ಸಂರಚನಾ cpumask *mask)
{
	/*
	 * If the task isn't a deadline task or admission control is
	 * disabled then we don't care about affinity changes.
	 */
	ಯದಿ (!task_has_dl_policy(p) || !dl_bandwidth_enabled())
		ಪ್ರತ್ಯಾಗಮ 0;

	/*
	 * The special/sugov task isn't part of regular bandwidth/admission
	 * control so let userspace change affinities.
	 */
	ಯದಿ (dl_entity_is_special(&p->dl))
		ಪ್ರತ್ಯಾಗಮ 0;

	/*
	 * Since bandwidth control happens on root_domain basis,
	 * if admission test is enabled, we only admit -deadline
	 * tasks allowed to run on all the CPUs in the task's
	 * root_domain.
	 */
	guard(rcu)();
	ಯದಿ (!cpumask_subset(task_rq(p)->rd->span, mask))
		ಪ್ರತ್ಯಾಗಮ -EBUSY;

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ __sched_setaffinity(ಸಂರಚನಾ task_struct *p, ಸಂರಚನಾ affinity_context *ctx)
{
	ಪೂರ್ಣಾಙ್ಕ retval;
	cpumask_var_t cpus_allowed, new_mask;

	ಯದಿ (!alloc_cpumask_var(&cpus_allowed, GFP_KERNEL))
		ಪ್ರತ್ಯಾಗಮ -ENOMEM;

	ಯದಿ (!alloc_cpumask_var(&new_mask, GFP_KERNEL)) {
		retval = -ENOMEM;
		ಗಚ್ಛ out_free_cpus_allowed;
	}

	cpuset_cpus_allowed(p, cpus_allowed);
	cpumask_and(new_mask, ctx->new_mask, cpus_allowed);

	ctx->new_mask = new_mask;
	ctx->flags |= SCA_CHECK;

	retval = dl_task_check_affinity(p, new_mask);
	ಯದಿ (retval)
		ಗಚ್ಛ out_free_new_mask;

	retval = __set_cpus_allowed_ptr(p, ctx);
	ಯದಿ (retval)
		ಗಚ್ಛ out_free_new_mask;

	cpuset_cpus_allowed(p, cpus_allowed);
	ಯದಿ (!cpumask_subset(new_mask, cpus_allowed)) {
		/*
		 * We must have raced with a concurrent cpuset update.
		 * Just reset the cpumask to the cpuset's cpus_allowed.
		 */
		cpumask_copy(new_mask, cpus_allowed);

		/*
		 * If SCA_USER is set, a 2nd call to __set_cpus_allowed_ptr()
		 * will restore the previous user_cpus_ptr value.
		 *
		 * In the unlikely event a previous user_cpus_ptr exists,
		 * we need to further restrict the mask to what is allowed
		 * by that old user_cpus_ptr.
		 */
		ಯದಿ (unlikely((ctx->flags & SCA_USER) && ctx->user_mask)) {
			bool empty = !cpumask_and(new_mask, new_mask,
						  ctx->user_mask);

			ಯದಿ (empty)
				cpumask_copy(new_mask, cpus_allowed);
		}
		__set_cpus_allowed_ptr(p, ctx);
		retval = -EINVAL;
	}

out_free_new_mask:
	free_cpumask_var(new_mask);
out_free_cpus_allowed:
	free_cpumask_var(cpus_allowed);
	ಪ್ರತ್ಯಾಗಮ retval;
}

long sched_setaffinity(pid_t pid, ಧ್ರುವ ಸಂರಚನಾ cpumask *in_mask)
{
	ಸಂರಚನಾ affinity_context ac;
	ಸಂರಚನಾ cpumask *user_mask;
	ಪೂರ್ಣಾಙ್ಕ retval;

	CLASS(find_get_task, p)(pid);
	ಯದಿ (!p)
		ಪ್ರತ್ಯಾಗಮ -ESRCH;

	ಯದಿ (p->flags & PF_NO_SETAFFINITY)
		ಪ್ರತ್ಯಾಗಮ -EINVAL;

	ಯದಿ (!check_same_owner(p)) {
		guard(rcu)();
		ಯದಿ (!ns_capable(__task_cred(p)->user_ns, CAP_SYS_NICE))
			ಪ್ರತ್ಯಾಗಮ -EPERM;
	}

	retval = security_task_setscheduler(p);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;

	/*
	 * With non-SMP configs, user_cpus_ptr/user_mask isn't used and
	 * alloc_user_cpus_ptr() returns NULL.
	 */
	user_mask = alloc_user_cpus_ptr(NUMA_NO_ಗಂಟು);
	ಯದಿ (user_mask) {
		cpumask_copy(user_mask, in_mask);
	} ಅನ್ಯಥಾ {
		ಪ್ರತ್ಯಾಗಮ -ENOMEM;
	}

	ac = (ಸಂರಚನಾ affinity_context){
		.new_mask  = in_mask,
		.user_mask = user_mask,
		.flags     = SCA_USER,
	};

	retval = __sched_setaffinity(p, &ac);
	kfree(ac.user_mask);

	ಪ್ರತ್ಯಾಗಮ retval;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ get_user_cpu_mask(unsigned long __user *user_mask_ptr, unsigned len,
			     ಸಂರಚನಾ cpumask *new_mask)
{
	ಯದಿ (len < cpumask_size())
		cpumask_clear(new_mask);
	ಅನ್ಯಥಾ ಯದಿ (len > cpumask_size())
		len = cpumask_size();

	ಪ್ರತ್ಯಾಗಮ copy_from_user(new_mask, user_mask_ptr, len) ? -EFAULT : 0;
}

/**
 * sys_sched_setaffinity - set the CPU affinity of a process
 * @pid: pid of the process
 * @len: length in bytes of the bitmask pointed to by user_mask_ptr
 * @user_mask_ptr: user-space pointer to the new CPU mask
 *
 * Return: 0 on success. An error code otherwise.
 */
SYSCALL_DEFINE3(sched_setaffinity, pid_t, pid, unsigned ಪೂರ್ಣಾಙ್ಕ, len,
		unsigned long __user *, user_mask_ptr)
{
	cpumask_var_t new_mask;
	ಪೂರ್ಣಾಙ್ಕ retval;

	ಯದಿ (!alloc_cpumask_var(&new_mask, GFP_KERNEL))
		ಪ್ರತ್ಯಾಗಮ -ENOMEM;

	retval = get_user_cpu_mask(user_mask_ptr, len, new_mask);
	ಯದಿ (retval == 0)
		retval = sched_setaffinity(pid, new_mask);
	free_cpumask_var(new_mask);
	ಪ್ರತ್ಯಾಗಮ retval;
}

long sched_getaffinity(pid_t pid, ಸಂರಚನಾ cpumask *mask)
{
	ಸಂರಚನಾ task_struct *p;
	ಪೂರ್ಣಾಙ್ಕ retval;

	guard(rcu)();
	p = find_process_by_pid(pid);
	ಯದಿ (!p)
		ಪ್ರತ್ಯಾಗಮ -ESRCH;

	retval = security_task_getscheduler(p);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;

	guard(raw_spinlock_irqsave)(&p->pi_lock);
	cpumask_and(mask, &p->cpus_mask, cpu_active_mask);

	ಪ್ರತ್ಯಾಗಮ 0;
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
SYSCALL_DEFINE3(sched_getaffinity, pid_t, pid, unsigned ಪೂರ್ಣಾಙ್ಕ, len,
		unsigned long __user *, user_mask_ptr)
{
	ಪೂರ್ಣಾಙ್ಕ ret;
	cpumask_var_t mask;

	ಯದಿ ((len * BITS_PER_BYTE) < nr_cpu_ids)
		ಪ್ರತ್ಯಾಗಮ -EINVAL;
	ಯದಿ (len & (ಪರಿಮಾಣ(unsigned long)-1))
		ಪ್ರತ್ಯಾಗಮ -EINVAL;

	ಯದಿ (!zalloc_cpumask_var(&mask, GFP_KERNEL))
		ಪ್ರತ್ಯಾಗಮ -ENOMEM;

	ret = sched_getaffinity(pid, mask);
	ಯದಿ (ret == 0) {
		unsigned ಪೂರ್ಣಾಙ್ಕ retlen = min(len, cpumask_size());

		ಯದಿ (copy_to_user(user_mask_ptr, cpumask_bits(mask), retlen))
			ret = -EFAULT;
		ಅನ್ಯಥಾ
			ret = retlen;
	}
	free_cpumask_var(mask);

	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಶೂನ್ಯ do_sched_yield(ಶೂನ್ಯ)
{
	ಸಂರಚನಾ rq_flags rf;
	ಸಂರಚನಾ rq *rq;

	rq = this_rq_lock_irq(&rf);

	schedstat_inc(rq->yld_ಎಣಿಕೆ);
	rq->donor->sched_class->yield_task(rq);

	preempt_disable();
	rq_unlock_irq(rq, &rf);
	sched_preempt_enable_no_resched();

	schedule();
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
	ಪ್ರತ್ಯಾಗಮ 0;
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
	set_current_state(TASK_RUNNING);
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
ಪೂರ್ಣಾಙ್ಕ __sched yield_to(ಸಂರಚನಾ task_struct *p, bool preempt)
{
	ಸಂರಚನಾ task_struct *curr;
	ಸಂರಚನಾ rq *rq, *p_rq;
	ಪೂರ್ಣಾಙ್ಕ yielded = 0;

	scoped_guard (raw_spinlock_irqsave, &p->pi_lock) {
		rq = this_rq();
		curr = rq->donor;

again:
		p_rq = task_rq(p);
		/*
		 * If we're the only runnable task on the rq and target rq also
		 * has only one task, there's absolutely no point in yielding.
		 */
		ಯದಿ (rq->nr_running == 1 && p_rq->nr_running == 1)
			ಪ್ರತ್ಯಾಗಮ -ESRCH;

		guard(double_rq_lock)(rq, p_rq);
		ಯದಿ (task_rq(p) != p_rq)
			ಗಚ್ಛ again;

		ಯದಿ (!curr->sched_class->yield_to_task)
			ಪ್ರತ್ಯಾಗಮ 0;

		ಯದಿ (curr->sched_class != p->sched_class)
			ಪ್ರತ್ಯಾಗಮ 0;

		ಯದಿ (task_on_cpu(p_rq, p) || !task_is_running(p))
			ಪ್ರತ್ಯಾಗಮ 0;

		yielded = curr->sched_class->yield_to_task(rq, p);
		ಯದಿ (yielded) {
			schedstat_inc(rq->yld_ಎಣಿಕೆ);
			/*
			 * Make p's CPU reschedule; pick_next_entity
			 * takes care of fairness.
			 */
			ಯದಿ (preempt && rq != p_rq)
				resched_curr(p_rq);
		}
	}

	ಯದಿ (yielded)
		schedule();

	ಪ್ರತ್ಯಾಗಮ yielded;
}
EXPORT_ಚಿಹ್ನೆ_GPL(yield_to);

/**
 * sys_sched_get_priority_max - return maximum RT priority.
 * @policy: scheduling class.
 *
 * Return: On success, this syscall returns the maximum
 * rt_priority that can be used by a given scheduling class.
 * On failure, a negative error code is returned.
 */
SYSCALL_DEFINE1(sched_get_priority_max, ಪೂರ್ಣಾಙ್ಕ, policy)
{
	ಪೂರ್ಣಾಙ್ಕ ret = -EINVAL;

	ವಿಕಲ್ಪ (policy) {
	ಪಕ್ಷ SCHED_FIFO:
	ಪಕ್ಷ SCHED_RR:
		ret = MAX_RT_PRIO-1;
		ಭಙ್ಗ;
	ಪಕ್ಷ SCHED_DEADLINE:
	ಪಕ್ಷ SCHED_NORMAL:
	ಪಕ್ಷ SCHED_BATCH:
	ಪಕ್ಷ SCHED_IDLE:
	ಪಕ್ಷ SCHED_EXT:
		ret = 0;
		ಭಙ್ಗ;
	}
	ಪ್ರತ್ಯಾಗಮ ret;
}

/**
 * sys_sched_get_priority_min - return minimum RT priority.
 * @policy: scheduling class.
 *
 * Return: On success, this syscall returns the minimum
 * rt_priority that can be used by a given scheduling class.
 * On failure, a negative error code is returned.
 */
SYSCALL_DEFINE1(sched_get_priority_min, ಪೂರ್ಣಾಙ್ಕ, policy)
{
	ಪೂರ್ಣಾಙ್ಕ ret = -EINVAL;

	ವಿಕಲ್ಪ (policy) {
	ಪಕ್ಷ SCHED_FIFO:
	ಪಕ್ಷ SCHED_RR:
		ret = 1;
		ಭಙ್ಗ;
	ಪಕ್ಷ SCHED_DEADLINE:
	ಪಕ್ಷ SCHED_NORMAL:
	ಪಕ್ಷ SCHED_BATCH:
	ಪಕ್ಷ SCHED_IDLE:
	ಪಕ್ಷ SCHED_EXT:
		ret = 0;
	}
	ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ sched_rr_get_interval(pid_t pid, ಸಂರಚನಾ timespec64 *t)
{
	unsigned ಪೂರ್ಣಾಙ್ಕ time_slice = 0;
	ಪೂರ್ಣಾಙ್ಕ retval;

	ಯದಿ (pid < 0)
		ಪ್ರತ್ಯಾಗಮ -EINVAL;

	scoped_guard (rcu) {
		ಸಂರಚನಾ task_struct *p = find_process_by_pid(pid);
		ಯದಿ (!p)
			ಪ್ರತ್ಯಾಗಮ -ESRCH;

		retval = security_task_getscheduler(p);
		ಯದಿ (retval)
			ಪ್ರತ್ಯಾಗಮ retval;

		scoped_guard (task_rq_lock, p) {
			ಸಂರಚನಾ rq *rq = scope.rq;
			ಯದಿ (p->sched_class->get_rr_interval)
				time_slice = p->sched_class->get_rr_interval(rq, p);
		}
	}

	jiffies_to_timespec64(time_slice, t);
	ಪ್ರತ್ಯಾಗಮ 0;
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
		ಸಂರಚನಾ __kernel_timespec __user *, interval)
{
	ಸಂರಚನಾ timespec64 t;
	ಪೂರ್ಣಾಙ್ಕ retval = sched_rr_get_interval(pid, &t);

	ಯದಿ (retval == 0)
		retval = put_timespec64(&t, interval);

	ಪ್ರತ್ಯಾಗಮ retval;
}

#ifdef CONFIG_COMPAT_32BIT_TIME
SYSCALL_DEFINE2(sched_rr_get_interval_time32, pid_t, pid,
		ಸಂರಚನಾ old_timespec32 __user *, interval)
{
	ಸಂರಚನಾ timespec64 t;
	ಪೂರ್ಣಾಙ್ಕ retval = sched_rr_get_interval(pid, &t);

	ಯದಿ (retval == 0)
		retval = put_old_timespec32(&t, interval);
	ಪ್ರತ್ಯಾಗಮ retval;
}
#endif
