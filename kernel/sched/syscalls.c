// SPDX-License-Identifier: GPL-2.0-only
/*
 *  kernel/sched/syscalls.c
 *
 *  Core kernel scheduler syscalls related code
 *
 *  Copyright (C) 1991-2002  Linus Torvalds
 *  Copyright (C) 1998-2024  Ingo Molnar, Red Hat
 */
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/cpuset.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/debug.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <uapi/linux/sched/types.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "sched.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "autogroup.h"

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ __normal_prio(ਪੂਰਨ policy, ਪੂਰਨ rt_prio, ਪੂਰਨ nice)
{
	ਪੂਰਨ prio;

	ਜੇ (dl_policy(policy))
		prio = MAX_DL_PRIO - 1;
	ਨਹੀਂਤਾਂ ਜੇ (rt_policy(policy))
		prio = MAX_RT_PRIO - 1 - rt_prio;
	ਨਹੀਂਤਾਂ
		prio = NICE_ਮੰਜ਼ਿਲ_PRIO(nice);

	ਵਾਪਸ prio;
}

/*
 * Calculate the expected normal priority: i.e. priority
 * without taking RT-inheritance into account. Might be
 * boosted by interactivity modifiers. Changes upon fork,
 * setprio syscalls, and whenever the interactivity
 * estimator recalculates.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ ਸਾਧਾਰਨ_prio(ਬਣਤਰ task_struct *p)
{
	ਵਾਪਸ __normal_prio(p->policy, p->rt_ਤਰਜੀਹ, PRIO_ਮੰਜ਼ਿਲ_NICE(p->static_prio));
}

/*
 * Calculate the current priority, i.e. the priority
 * taken into account by the scheduler. This value might
 * be boosted by RT tasks, or might be boosted by
 * interactivity modifiers. Will be RT if the task got
 * RT-boosted. If not then it returns p->normal_prio.
 */
ਸਥਿਰ ਪੂਰਨ effective_prio(ਬਣਤਰ task_struct *p)
{
	p->ਸਾਧਾਰਨ_prio = ਸਾਧਾਰਨ_prio(p);
	/*
	 * If we are RT tasks or we were boosted to RT priority,
	 * keep the priority unchanged. Otherwise, update priority
	 * to the normal priority:
	 */
	ਜੇ (!rt_or_dl_prio(p->prio))
		ਵਾਪਸ p->ਸਾਧਾਰਨ_prio;
	ਵਾਪਸ p->prio;
}

ਖਾਲੀ ਸੈੱਟ_user_nice(ਬਣਤਰ task_struct *p, ਲੰਮਾ nice)
{
	ਪੂਰਨ old_prio;

	ਜੇ (task_nice(p) == nice || nice < MIN_NICE || nice > MAX_NICE)
		ਵਾਪਸ;
	/*
	 * We have to be careful, if called from sys_setpriority(),
	 * the task might be in the middle of scheduling on another CPU.
	 */
	guard(task_rq_ਤਾਲਾ_2)(p);

	/*
	 * The RT priorities are set via sched_setscheduler(), but we still
	 * allow the 'normal' nice value to be set - but as expected
	 * it won't have any effect on scheduling until the task is
	 * SCHED_DEADLINE, SCHED_FIFO or SCHED_RR:
	 */
	ਜੇ (task_has_dl_policy(p) || task_has_rt_policy(p)) {
		p->static_prio = NICE_ਮੰਜ਼ਿਲ_PRIO(nice);
		ਵਾਪਸ;
	}

	scoped_guard (sched_change, p, ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_SAVE) {
		p->static_prio = NICE_ਮੰਜ਼ਿਲ_PRIO(nice);
		ਸੈੱਟ_ਲੱਦੋ_ਭਾਰ(p, true);
		old_prio = p->prio;
		p->prio = effective_prio(p);
	}
}
EXPORT_ਚਿੰਨ੍ਹ(ਸੈੱਟ_user_nice);

/*
 * is_nice_reduction - check if nice value is an actual reduction
 *
 * Similar to can_nice() but does not perform a capability check.
 *
 * @p: task
 * @nice: nice value
 */
ਸਥਿਰ bool is_nice_reduction(ਅਚਲ ਬਣਤਰ task_struct *p, ਅਚਲ ਪੂਰਨ nice)
{
	/* Convert nice value [19,-20] to rlimit style value [1,40]: */
	ਪੂਰਨ nice_rlim = nice_ਮੰਜ਼ਿਲ_rlimit(nice);

	ਵਾਪਸ (nice_rlim <= task_rlimit(p, RLIMIT_NICE));
}

/*
 * can_nice - check if a task can reduce its nice value
 * @p: task
 * @nice: nice value
 */
ਪੂਰਨ can_nice(ਅਚਲ ਬਣਤਰ task_struct *p, ਅਚਲ ਪੂਰਨ nice)
{
	ਵਾਪਸ is_nice_reduction(p, nice) || capable(CAP_SYS_NICE);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __ARCH_WANT_SYS_NICE

/*
 * sys_nice - change the priority of the current process.
 * @increment: priority increment
 *
 * sys_setpriority is a more generic, but much slower function that
 * does similar things.
 */
SYSCALL_DEFINE1(nice, ਪੂਰਨ, increment)
{
	ਲੰਮਾ nice, retval;

	/*
	 * Setpriority might change our priority at the same moment.
	 * We don't have to worry. Conceptually one call occurs first
	 * and we have a single winner.
	 */
	increment = clamp(increment, -NICE_ਚੌੜਾਈ, NICE_ਚੌੜਾਈ);
	nice = task_nice(current) + increment;

	nice = clamp_val(nice, MIN_NICE, MAX_NICE);
	ਜੇ (increment < 0 && !can_nice(current, nice))
		ਵਾਪਸ -EPERM;

	retval = security_task_setnice(current, nice);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	ਸੈੱਟ_user_nice(current, nice);
	ਵਾਪਸ 0;
}

#ਜੇ_ਅੰਤ /* __ARCH_WANT_SYS_NICE */

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
ਪੂਰਨ task_prio(ਅਚਲ ਬਣਤਰ task_struct *p)
{
	ਵਾਪਸ p->prio - MAX_RT_PRIO;
}

/**
 * idle_cpu - is a given CPU idle currently?
 * @cpu: the processor in question.
 *
 * Return: 1 if the CPU is currently idle. 0 otherwise.
 */
ਪੂਰਨ idle_cpu(ਪੂਰਨ cpu)
{
	ਵਾਪਸ idle_rq(cpu_rq(cpu));
}

/**
 * idle_task - return the idle task for a given CPU.
 * @cpu: the processor in question.
 *
 * Return: The idle task for the CPU @cpu.
 */
ਬਣਤਰ task_struct *idle_task(ਪੂਰਨ cpu)
{
	ਵਾਪਸ cpu_rq(cpu)->idle;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CORE
ਪੂਰਨ sched_core_idle_cpu(ਪੂਰਨ cpu)
{
	ਬਣਤਰ rq *rq = cpu_rq(cpu);

	ਜੇ (sched_core_enabled(rq) && rq->curr == rq->idle)
		ਵਾਪਸ 1;

	ਵਾਪਸ idle_cpu(cpu);
}
#ਜੇ_ਅੰਤ /* CONFIG_SCHED_CORE */

/**
 * find_process_by_pid - find a process with a matching PID value.
 * @pid: the pid in question.
 *
 * The task of @pid, if found. %NULL otherwise.
 */
ਸਥਿਰ ਬਣਤਰ task_struct *ਲੱਭੋ_process_by_pid(pid_t pid)
{
	ਵਾਪਸ pid ? ਲੱਭੋ_task_by_vpid(pid) : current;
}

ਸਥਿਰ ਬਣਤਰ task_struct *ਲੱਭੋ_get_task(pid_t pid)
{
	ਬਣਤਰ task_struct *p;
	guard(rcu)();

	p = ਲੱਭੋ_process_by_pid(pid);
	ਜੇ (likely(p))
		get_task_struct(p);

	ਵਾਪਸ p;
}

DEFINE_CLASS(ਲੱਭੋ_get_task, ਬਣਤਰ task_struct *, ਜੇ (_T) put_task_struct(_T),
	     ਲੱਭੋ_get_task(pid), pid_t pid)

/*
 * sched_setparam() passes in -1 for its policy, to let the functions
 * it calls know not to change it.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SETPARAM_POLICY	-1

ਸਥਿਰ ਖਾਲੀ __setscheduler_params(ਬਣਤਰ task_struct *p,
		ਅਚਲ ਬਣਤਰ sched_attr *attr)
{
	ਪੂਰਨ policy = attr->sched_policy;

	ਜੇ (policy == SETPARAM_POLICY)
		policy = p->policy;

	p->policy = policy;

	ਜੇ (dl_policy(policy))
		__setparam_dl(p, attr);
	ਨਹੀਂਤਾਂ ਜੇ (fair_policy(policy))
		__setparam_fair(p, attr);

	/* rt-policy tasks do not have a timerslack */
	ਜੇ (rt_or_dl_task_policy(p)) {
		p->timer_slack_ns = 0;
	} ਨਹੀਂਤਾਂ ਜੇ (p->timer_slack_ns == 0) {
		/* when switching back to non-rt policy, restore timerslack */
		p->timer_slack_ns = p->default_timer_slack_ns;
	}

	/*
	 * __sched_setscheduler() ensures attr->sched_priority == 0 when
	 * !rt_policy. Always setting this ensures that things like
	 * getparam()/getattr() don't report silly values for !rt tasks.
	 */
	p->rt_ਤਰਜੀਹ = attr->sched_ਤਰਜੀਹ;
	p->ਸਾਧਾਰਨ_prio = ਸਾਧਾਰਨ_prio(p);
	ਸੈੱਟ_ਲੱਦੋ_ਭਾਰ(p, true);
}

/*
 * Check the target process has a UID that matches the current process's:
 */
ਸਥਿਰ bool check_same_owner(ਬਣਤਰ task_struct *p)
{
	ਅਚਲ ਬਣਤਰ cred *cred = current_cred(), *pcred;
	guard(rcu)();

	pcred = __task_cred(p);
	ਵਾਪਸ (uid_eq(cred->euid, pcred->euid) ||
		uid_eq(cred->euid, pcred->uid));
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_RT_MUTEXES
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ __setscheduler_dl_pi(ਪੂਰਨ newprio, ਪੂਰਨ policy,
			      ਬਣਤਰ task_struct *p,
			      ਬਣਤਰ sched_change_ctx *scope)
{
	/*
	 * In case a DEADLINE task (either proper or boosted) gets
	 * setscheduled to a lower priority class, check if it neeeds to
	 * inherit parameters from a potential pi_task. In that case make
	 * sure replenishment happens with the next enqueue.
	 */

	ਜੇ (dl_prio(newprio) && !dl_policy(policy)) {
		ਬਣਤਰ task_struct *pi_task = rt_mutex_get_ਸਿਖਰ_task(p);

		ਜੇ (pi_task) {
			p->dl.pi_se = pi_task->dl.pi_se;
			scope->flags |= ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_REPLENISH;
		}
	}
}
#ਨਹੀਂਤਾਂ /* !CONFIG_RT_MUTEXES */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ __setscheduler_dl_pi(ਪੂਰਨ newprio, ਪੂਰਨ policy,
			      ਬਣਤਰ task_struct *p,
			      ਬਣਤਰ sched_change_ctx *scope)
{
}
#ਜੇ_ਅੰਤ /* !CONFIG_RT_MUTEXES */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_UCLAMP_TASK

ਸਥਿਰ ਪੂਰਨ uclamp_validate(ਬਣਤਰ task_struct *p,
			   ਅਚਲ ਬਣਤਰ sched_attr *attr)
{
	ਪੂਰਨ util_min = p->uclamp_req[UCLAMP_MIN].ਮੁੱਲ;
	ਪੂਰਨ util_max = p->uclamp_req[UCLAMP_MAX].ਮੁੱਲ;

	ਜੇ (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MIN) {
		util_min = attr->sched_util_min;

		ਜੇ (util_min + 1 > SCHED_ਸਮਰੱਥਾ_SCALE + 1)
			ਵਾਪਸ -EINVAL;
	}

	ਜੇ (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MAX) {
		util_max = attr->sched_util_max;

		ਜੇ (util_max + 1 > SCHED_ਸਮਰੱਥਾ_SCALE + 1)
			ਵਾਪਸ -EINVAL;
	}

	ਜੇ (util_min != -1 && util_max != -1 && util_min > util_max)
		ਵਾਪਸ -EINVAL;

	/*
	 * We have valid uclamp attributes; make sure uclamp is enabled.
	 *
	 * We need to do that here, because enabling static branches is a
	 * blocking operation which obviously cannot be done while holding
	 * scheduler locks.
	 */
	sched_uclamp_enable();

	ਵਾਪਸ 0;
}

ਸਥਿਰ bool uclamp_reset(ਅਚਲ ਬਣਤਰ sched_attr *attr,
			 ਗਿਣਤੀ uclamp_id clamp_id,
			 ਬਣਤਰ uclamp_se *uc_se)
{
	/* Reset on sched class change for a non user-defined clamp value. */
	ਜੇ (likely(!(attr->sched_flags & SCHED_FLAG_UTIL_CLAMP)) &&
	    !uc_se->user_defined)
		ਵਾਪਸ true;

	/* Reset on sched_util_{min,max} == -1. */
	ਜੇ (clamp_id == UCLAMP_MIN &&
	    attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MIN &&
	    attr->sched_util_min == -1) {
		ਵਾਪਸ true;
	}

	ਜੇ (clamp_id == UCLAMP_MAX &&
	    attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MAX &&
	    attr->sched_util_max == -1) {
		ਵਾਪਸ true;
	}

	ਵਾਪਸ false;
}

ਸਥਿਰ ਖਾਲੀ __setscheduler_uclamp(ਬਣਤਰ task_struct *p,
				  ਅਚਲ ਬਣਤਰ sched_attr *attr)
{
	ਗਿਣਤੀ uclamp_id clamp_id;

	for_each_clamp_id(clamp_id) {
		ਬਣਤਰ uclamp_se *uc_se = &p->uclamp_req[clamp_id];
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮੁੱਲ;

		ਜੇ (!uclamp_reset(attr, clamp_id, uc_se))
			ਜਾਰੀ;

		/*
		 * RT by default have a 100% boost value that could be modified
		 * at runtime.
		 */
		ਜੇ (unlikely(rt_task(p) && clamp_id == UCLAMP_MIN))
			ਮੁੱਲ = sysctl_sched_uclamp_util_min_rt_default;
		ਨਹੀਂਤਾਂ
			ਮੁੱਲ = uclamp_none(clamp_id);

		uclamp_se_ਸੈੱਟ(uc_se, ਮੁੱਲ, false);

	}

	ਜੇ (likely(!(attr->sched_flags & SCHED_FLAG_UTIL_CLAMP)))
		ਵਾਪਸ;

	ਜੇ (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MIN &&
	    attr->sched_util_min != -1) {
		uclamp_se_ਸੈੱਟ(&p->uclamp_req[UCLAMP_MIN],
			      attr->sched_util_min, true);
	}

	ਜੇ (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MAX &&
	    attr->sched_util_max != -1) {
		uclamp_se_ਸੈੱਟ(&p->uclamp_req[UCLAMP_MAX],
			      attr->sched_util_max, true);
	}
}

#ਨਹੀਂਤਾਂ /* !CONFIG_UCLAMP_TASK: */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ uclamp_validate(ਬਣਤਰ task_struct *p,
				  ਅਚਲ ਬਣਤਰ sched_attr *attr)
{
	ਵਾਪਸ -EOPNOTSUPP;
}
ਸਥਿਰ ਖਾਲੀ __setscheduler_uclamp(ਬਣਤਰ task_struct *p,
				  ਅਚਲ ਬਣਤਰ sched_attr *attr) { }
#ਜੇ_ਅੰਤ /* !CONFIG_UCLAMP_TASK */

/*
 * Allow unprivileged RT tasks to decrease priority.
 * Only issue a capable test if needed and only once to avoid an audit
 * event on permitted non-privileged operations:
 */
ਸਥਿਰ ਪੂਰਨ user_check_sched_setscheduler(ਬਣਤਰ task_struct *p,
					 ਅਚਲ ਬਣਤਰ sched_attr *attr,
					 ਪੂਰਨ policy, ਪੂਰਨ reset_on_fork)
{
	ਜੇ (fair_policy(policy)) {
		ਜੇ (attr->sched_nice < task_nice(p) &&
		    !is_nice_reduction(p, attr->sched_nice))
			ਜਾਓ req_priv;
	}

	ਜੇ (rt_policy(policy)) {
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ rlim_rtprio = task_rlimit(p, RLIMIT_RTPRIO);

		/* Can't set/change the rt policy: */
		ਜੇ (policy != p->policy && !rlim_rtprio)
			ਜਾਓ req_priv;

		/* Can't increase priority: */
		ਜੇ (attr->sched_ਤਰਜੀਹ > p->rt_ਤਰਜੀਹ &&
		    attr->sched_ਤਰਜੀਹ > rlim_rtprio)
			ਜਾਓ req_priv;
	}

	/*
	 * Can't set/change SCHED_DEADLINE policy at all for now
	 * (safest behavior); in the future we would like to allow
	 * unprivileged DL tasks to increase their relative deadline
	 * or reduce their runtime (both ways reducing utilization)
	 */
	ਜੇ (dl_policy(policy))
		ਜਾਓ req_priv;

	/*
	 * Treat SCHED_IDLE as nice 20. Only allow a switch to
	 * SCHED_NORMAL if the RLIMIT_NICE would normally permit it.
	 */
	ਜੇ (task_has_idle_policy(p) && !idle_policy(policy)) {
		ਜੇ (!is_nice_reduction(p, task_nice(p)))
			ਜਾਓ req_priv;
	}

	/* Can't change other user's priorities: */
	ਜੇ (!check_same_owner(p))
		ਜਾਓ req_priv;

	/* Normal users shall not reset the sched_reset_on_fork flag: */
	ਜੇ (p->sched_reset_on_fork && !reset_on_fork)
		ਜਾਓ req_priv;

	ਵਾਪਸ 0;

req_priv:
	ਜੇ (!capable(CAP_SYS_NICE))
		ਵਾਪਸ -EPERM;

	ਵਾਪਸ 0;
}

ਪੂਰਨ __sched_setscheduler(ਬਣਤਰ task_struct *p,
			 ਅਚਲ ਬਣਤਰ sched_attr *attr,
			 bool user, bool pi)
{
	ਪੂਰਨ oldpolicy = -1, policy = attr->sched_policy;
	ਪੂਰਨ retval, oldprio, newprio;
	ਅਚਲ ਬਣਤਰ sched_class *prev_class, *ਅਗਲਾ_class;
	ਬਣਤਰ balance_callback *ਸਿਰ;
	ਬਣਤਰ rq_flags rf;
	ਪੂਰਨ reset_on_fork;
	ਪੂਰਨ ਕਤਾਰ_flags = ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_SAVE | ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_MOVE | ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_NOCLOCK;
	ਬਣਤਰ rq *rq;
	bool cpuset_locked = false;

	/* The pi code expects interrupts enabled */
	BUG_ON(pi && ਵਿੱਚ_ਰੁਕਾਵਟ());
recheck:
	/* Double check policy once rq lock held: */
	ਜੇ (policy < 0) {
		reset_on_fork = p->sched_reset_on_fork;
		policy = oldpolicy = p->policy;
	} ਨਹੀਂਤਾਂ {
		reset_on_fork = !!(attr->sched_flags & SCHED_FLAG_RESET_ON_FORK);

		ਜੇ (!valid_policy(policy))
			ਵਾਪਸ -EINVAL;
	}

	ਜੇ (attr->sched_flags & ~(SCHED_FLAG_ALL | SCHED_FLAG_SUGOV))
		ਵਾਪਸ -EINVAL;

	/*
	 * Valid priorities for SCHED_FIFO and SCHED_RR are
	 * 1..MAX_RT_PRIO-1, valid priority for SCHED_NORMAL,
	 * SCHED_BATCH and SCHED_IDLE is 0.
	 */
	ਜੇ (attr->sched_ਤਰਜੀਹ > MAX_RT_PRIO-1)
		ਵਾਪਸ -EINVAL;
	ਜੇ ((dl_policy(policy) && !__checkparam_dl(attr)) ||
	    (rt_policy(policy) != (attr->sched_ਤਰਜੀਹ != 0)))
		ਵਾਪਸ -EINVAL;

	ਜੇ (user) {
		retval = user_check_sched_setscheduler(p, attr, policy, reset_on_fork);
		ਜੇ (retval)
			ਵਾਪਸ retval;

		ਜੇ (attr->sched_flags & SCHED_FLAG_SUGOV)
			ਵਾਪਸ -EINVAL;

		retval = security_task_setscheduler(p);
		ਜੇ (retval)
			ਵਾਪਸ retval;
	}

	/* Update task specific "requested" clamps */
	ਜੇ (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP) {
		retval = uclamp_validate(p, attr);
		ਜੇ (retval)
			ਵਾਪਸ retval;
	}

	/*
	 * SCHED_DEADLINE bandwidth accounting relies on stable cpusets
	 * information.
	 */
	ਜੇ (dl_policy(policy) || dl_policy(p->policy)) {
		cpuset_locked = true;
		cpuset_ਤਾਲਾ();
	}

	/*
	 * Make sure no PI-waiters arrive (or leave) while we are
	 * changing the priority of the task:
	 *
	 * To be able to change p->policy safely, the appropriate
	 * runqueue lock must be held.
	 */
	rq = task_rq_ਤਾਲਾ_2(p, &rf);
	update_rq_clock(rq);

	/*
	 * Changing the policy of the stop threads its a very bad idea:
	 */
	ਜੇ (p == rq->stop) {
		retval = -EINVAL;
		ਜਾਓ unlock;
	}

	retval = scx_check_setscheduler(p, policy);
	ਜੇ (retval)
		ਜਾਓ unlock;

	/*
	 * If not changing anything there's no need to proceed further,
	 * but store a possible modification of reset_on_fork.
	 */
	ਜੇ (unlikely(policy == p->policy)) {
		ਜੇ (fair_policy(policy) &&
		    (attr->sched_nice != task_nice(p) ||
		     (attr->sched_runtime != p->se.ਟੁਕੜਾ)))
			ਜਾਓ change;
		ਜੇ (rt_policy(policy) && attr->sched_ਤਰਜੀਹ != p->rt_ਤਰਜੀਹ)
			ਜਾਓ change;
		ਜੇ (dl_policy(policy) && dl_param_ਬਦਲਿਆ(p, attr))
			ਜਾਓ change;
		ਜੇ (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP)
			ਜਾਓ change;

		p->sched_reset_on_fork = reset_on_fork;
		retval = 0;
		ਜਾਓ unlock;
	}
change:

	ਜੇ (user) {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_RT_ਸਮੂਹ_SCHED
		/*
		 * Do not allow real-time tasks into groups that have no runtime
		 * assigned.
		 */
		ਜੇ (rt_ਸਮੂਹ_sched_enabled() &&
				rt_bandwidth_enabled() && rt_policy(policy) &&
				task_ਸਮੂਹ(p)->rt_bandwidth.rt_runtime == 0 &&
				!task_ਸਮੂਹ_is_autogroup(task_ਸਮੂਹ(p))) {
			retval = -EPERM;
			ਜਾਓ unlock;
		}
#ਜੇ_ਅੰਤ /* CONFIG_RT_GROUP_SCHED */
		ਜੇ (dl_bandwidth_enabled() && dl_policy(policy) &&
				!(attr->sched_flags & SCHED_FLAG_SUGOV)) {
			cpumask_t *ਵਿਸਤਾਰ = rq->rd->ਵਿਸਤਾਰ;

			/*
			 * Don't allow tasks with an affinity mask smaller than
			 * the entire root_domain to become SCHED_DEADLINE. We
			 * will also fail if there's no bandwidth available.
			 */
			ਜੇ (!cpumask_subset(ਵਿਸਤਾਰ, p->cpus_ptr) ||
			    rq->rd->dl_bw.bw == 0) {
				retval = -EPERM;
				ਜਾਓ unlock;
			}
		}
	}

	/* Re-check policy now with rq lock held: */
	ਜੇ (unlikely(oldpolicy != -1 && oldpolicy != p->policy)) {
		policy = oldpolicy = -1;
		task_rq_unlock(rq, p, &rf);
		ਜੇ (cpuset_locked)
			cpuset_unlock();
		ਜਾਓ recheck;
	}

	/*
	 * If setscheduling to SCHED_DEADLINE (or changing the parameters
	 * of a SCHED_DEADLINE task) we need to check if enough bandwidth
	 * is available.
	 */
	ਜੇ ((dl_policy(policy) || dl_task(p)) && sched_dl_ਉਛਾਲ(p, policy, attr)) {
		retval = -EBUSY;
		ਜਾਓ unlock;
	}

	p->sched_reset_on_fork = reset_on_fork;
	oldprio = p->prio;

	newprio = __normal_prio(policy, attr->sched_ਤਰਜੀਹ, attr->sched_nice);
	ਜੇ (pi) {
		/*
		 * Take priority boosted tasks into account. If the new
		 * effective priority is unchanged, we just store the new
		 * normal parameters and do not touch the scheduler class and
		 * the runqueue. This will be done when the task deboost
		 * itself.
		 */
		newprio = rt_effective_prio(p, newprio);
		ਜੇ (newprio == oldprio && !dl_prio(newprio))
			ਕਤਾਰ_flags &= ~ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_MOVE;
	}

	prev_class = p->sched_class;
	ਅਗਲਾ_class = __setscheduler_class(policy, newprio);

	ਜੇ (prev_class != ਅਗਲਾ_class)
		ਕਤਾਰ_flags |= ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_CLASS;

	scoped_guard (sched_change, p, ਕਤਾਰ_flags) {

		ਜੇ (!(attr->sched_flags & SCHED_FLAG_KEEP_PARAMS)) {
			__setscheduler_params(p, attr);
			p->sched_class = ਅਗਲਾ_class;
			p->prio = newprio;
			__setscheduler_dl_pi(newprio, policy, p, scope);
		}
		__setscheduler_uclamp(p, attr);

		ਜੇ (scope->queued) {
			/*
			 * We enqueue to tail when the priority of a task is
			 * increased (user space view).
			 */
			ਜੇ (oldprio < p->prio)
				scope->flags |= ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_ਸਿਰ;
		}
	}

	/* Avoid rq from going away on us: */
	preempt_disable();
	ਸਿਰ = splice_balance_callbacks(rq);
	task_rq_unlock(rq, p, &rf);

	ਜੇ (pi) {
		ਜੇ (cpuset_locked)
			cpuset_unlock();
		rt_mutex_adjust_pi(p);
	}

	/* Run balance callbacks after we've adjusted the PI chain: */
	balance_callbacks(rq, ਸਿਰ);
	preempt_enable();

	ਵਾਪਸ 0;

unlock:
	task_rq_unlock(rq, p, &rf);
	ਜੇ (cpuset_locked)
		cpuset_unlock();
	ਵਾਪਸ retval;
}

ਸਥਿਰ ਪੂਰਨ _sched_setscheduler(ਬਣਤਰ task_struct *p, ਪੂਰਨ policy,
			       ਅਚਲ ਬਣਤਰ sched_param *param, bool check)
{
	ਬਣਤਰ sched_attr attr = {
		.sched_policy   = policy,
		.sched_ਤਰਜੀਹ = param->sched_ਤਰਜੀਹ,
		.sched_nice	= PRIO_ਮੰਜ਼ਿਲ_NICE(p->static_prio),
	};

	ਜੇ (p->se.custom_ਟੁਕੜਾ)
		attr.sched_runtime = p->se.ਟੁਕੜਾ;

	/* Fixup the legacy SCHED_RESET_ON_FORK hack. */
	ਜੇ ((policy != SETPARAM_POLICY) && (policy & SCHED_RESET_ON_FORK)) {
		attr.sched_flags |= SCHED_FLAG_RESET_ON_FORK;
		policy &= ~SCHED_RESET_ON_FORK;
		attr.sched_policy = policy;
	}

	ਵਾਪਸ __sched_setscheduler(p, &attr, check, true);
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
ਪੂਰਨ sched_setscheduler(ਬਣਤਰ task_struct *p, ਪੂਰਨ policy,
		       ਅਚਲ ਬਣਤਰ sched_param *param)
{
	ਵਾਪਸ _sched_setscheduler(p, policy, param, true);
}

ਪੂਰਨ sched_setattr(ਬਣਤਰ task_struct *p, ਅਚਲ ਬਣਤਰ sched_attr *attr)
{
	ਵਾਪਸ __sched_setscheduler(p, attr, true, true);
}

ਪੂਰਨ sched_setattr_nocheck(ਬਣਤਰ task_struct *p, ਅਚਲ ਬਣਤਰ sched_attr *attr)
{
	ਵਾਪਸ __sched_setscheduler(p, attr, false, true);
}
EXPORT_ਚਿੰਨ੍ਹ_GPL(sched_setattr_nocheck);

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
ਪੂਰਨ sched_setscheduler_nocheck(ਬਣਤਰ task_struct *p, ਪੂਰਨ policy,
			       ਅਚਲ ਬਣਤਰ sched_param *param)
{
	ਵਾਪਸ _sched_setscheduler(p, policy, param, false);
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
ਖਾਲੀ sched_ਸੈੱਟ_fifo(ਬਣਤਰ task_struct *p)
{
	ਬਣਤਰ sched_param sp = { .sched_ਤਰਜੀਹ = MAX_RT_PRIO / 2 };
	WARN_ON_ONCE(sched_setscheduler_nocheck(p, SCHED_FIFO, &sp) != 0);
}
EXPORT_ਚਿੰਨ੍ਹ_GPL(sched_ਸੈੱਟ_fifo);

/*
 * For when you don't much care about FIFO, but want to be above SCHED_NORMAL.
 */
ਖਾਲੀ sched_ਸੈੱਟ_fifo_ਨੀਵਾਂ(ਬਣਤਰ task_struct *p)
{
	ਬਣਤਰ sched_param sp = { .sched_ਤਰਜੀਹ = 1 };
	WARN_ON_ONCE(sched_setscheduler_nocheck(p, SCHED_FIFO, &sp) != 0);
}
EXPORT_ਚਿੰਨ੍ਹ_GPL(sched_ਸੈੱਟ_fifo_ਨੀਵਾਂ);

/*
 * Used when the primary interrupt handler is forced into a thread, in addition
 * to the (always threaded) secondary handler.  The secondary handler gets a
 * slightly lower priority so that the primary handler can preempt it, thereby
 * emulating the behavior of a non-PREEMPT_RT system where the primary handler
 * runs in hard interrupt context.
 */
ਖਾਲੀ sched_ਸੈੱਟ_fifo_secondary(ਬਣਤਰ task_struct *p)
{
	ਬਣਤਰ sched_param sp = { .sched_ਤਰਜੀਹ = MAX_RT_PRIO / 2 - 1 };
	WARN_ON_ONCE(sched_setscheduler_nocheck(p, SCHED_FIFO, &sp) != 0);
}

ਖਾਲੀ sched_ਸੈੱਟ_ਸਾਧਾਰਨ(ਬਣਤਰ task_struct *p, ਪੂਰਨ nice)
{
	ਬਣਤਰ sched_attr attr = {
		.sched_policy = SCHED_ਸਾਧਾਰਨ,
		.sched_nice = nice,
	};
	WARN_ON_ONCE(sched_setattr_nocheck(p, &attr) != 0);
}
EXPORT_ਚਿੰਨ੍ਹ_GPL(sched_ਸੈੱਟ_ਸਾਧਾਰਨ);

ਸਥਿਰ ਪੂਰਨ
do_sched_setscheduler(pid_t pid, ਪੂਰਨ policy, ਬਣਤਰ sched_param __user *param)
{
	ਬਣਤਰ sched_param lparam;

	ਜੇ (unlikely(!param || pid < 0))
		ਵਾਪਸ -EINVAL;
	ਜੇ (copy_from_user(&lparam, param, ਆਕਾਰ(ਬਣਤਰ sched_param)))
		ਵਾਪਸ -EFAULT;

	CLASS(ਲੱਭੋ_get_task, p)(pid);
	ਜੇ (!p)
		ਵਾਪਸ -ESRCH;

	ਵਾਪਸ sched_setscheduler(p, policy, &lparam);
}

/*
 * Mimics kernel/events/core.c perf_copy_attr().
 */
ਸਥਿਰ ਪੂਰਨ sched_copy_attr(ਬਣਤਰ sched_attr __user *uattr, ਬਣਤਰ sched_attr *attr)
{
	u32 ਮਾਪ;
	ਪੂਰਨ ret;

	/* Zero the full structure, so that a short copy will be nice: */
	memset(attr, 0, ਆਕਾਰ(*attr));

	ret = get_user(ਮਾਪ, &uattr->ਮਾਪ);
	ਜੇ (ret)
		ਵਾਪਸ ret;

	/* ABI compatibility quirk: */
	ਜੇ (!ਮਾਪ)
		ਮਾਪ = SCHED_ATTR_ਮਾਪ_VER0;
	ਜੇ (ਮਾਪ < SCHED_ATTR_ਮਾਪ_VER0 || ਮਾਪ > ਪੰਨਾ_ਮਾਪ)
		ਜਾਓ err_ਮਾਪ;

	ret = copy_struct_from_user(attr, ਆਕਾਰ(*attr), uattr, ਮਾਪ);
	ਜੇ (ret) {
		ਜੇ (ret == -E2BIG)
			ਜਾਓ err_ਮਾਪ;
		ਵਾਪਸ ret;
	}

	ਜੇ ((attr->sched_flags & SCHED_FLAG_UTIL_CLAMP) &&
	    ਮਾਪ < SCHED_ATTR_ਮਾਪ_VER1)
		ਵਾਪਸ -EINVAL;

	/*
	 * XXX: Do we want to be lenient like existing syscalls; or do we want
	 * to be strict and return an error on out-of-bounds values?
	 */
	attr->sched_nice = clamp(attr->sched_nice, MIN_NICE, MAX_NICE);

	ਵਾਪਸ 0;

err_ਮਾਪ:
	put_user(ਆਕਾਰ(*attr), &uattr->ਮਾਪ);
	ਵਾਪਸ -E2BIG;
}

ਸਥਿਰ ਖਾਲੀ get_params(ਬਣਤਰ task_struct *p, ਬਣਤਰ sched_attr *attr, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags)
{
	ਜੇ (task_has_dl_policy(p)) {
		__getparam_dl(p, attr, flags);
	} ਨਹੀਂਤਾਂ ਜੇ (task_has_rt_policy(p)) {
		attr->sched_ਤਰਜੀਹ = p->rt_ਤਰਜੀਹ;
	} ਨਹੀਂਤਾਂ {
		attr->sched_nice = task_nice(p);
		attr->sched_runtime = p->se.ਟੁਕੜਾ;
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
SYSCALL_DEFINE3(sched_setscheduler, pid_t, pid, ਪੂਰਨ, policy, ਬਣਤਰ sched_param __user *, param)
{
	ਜੇ (policy < 0)
		ਵਾਪਸ -EINVAL;

	ਵਾਪਸ do_sched_setscheduler(pid, policy, param);
}

/**
 * sys_sched_setparam - set/change the RT priority of a thread
 * @pid: the pid in question.
 * @param: structure containing the new RT priority.
 *
 * Return: 0 on success. An error code otherwise.
 */
SYSCALL_DEFINE2(sched_setparam, pid_t, pid, ਬਣਤਰ sched_param __user *, param)
{
	ਵਾਪਸ do_sched_setscheduler(pid, SETPARAM_POLICY, param);
}

/**
 * sys_sched_setattr - same as above, but with extended sched_attr
 * @pid: the pid in question.
 * @uattr: structure containing the extended parameters.
 * @flags: for future extension.
 */
SYSCALL_DEFINE3(sched_setattr, pid_t, pid, ਬਣਤਰ sched_attr __user *, uattr,
			       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ, flags)
{
	ਬਣਤਰ sched_attr attr;
	ਪੂਰਨ retval;

	ਜੇ (unlikely(!uattr || pid < 0 || flags))
		ਵਾਪਸ -EINVAL;

	retval = sched_copy_attr(uattr, &attr);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	ਜੇ ((ਪੂਰਨ)attr.sched_policy < 0)
		ਵਾਪਸ -EINVAL;
	ਜੇ (attr.sched_flags & SCHED_FLAG_KEEP_POLICY)
		attr.sched_policy = SETPARAM_POLICY;

	CLASS(ਲੱਭੋ_get_task, p)(pid);
	ਜੇ (!p)
		ਵਾਪਸ -ESRCH;

	ਜੇ (attr.sched_flags & SCHED_FLAG_KEEP_PARAMS)
		get_params(p, &attr, 0);

	ਵਾਪਸ sched_setattr(p, &attr);
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
	ਬਣਤਰ task_struct *p;
	ਪੂਰਨ retval;

	ਜੇ (pid < 0)
		ਵਾਪਸ -EINVAL;

	guard(rcu)();
	p = ਲੱਭੋ_process_by_pid(pid);
	ਜੇ (!p)
		ਵਾਪਸ -ESRCH;

	retval = security_task_getscheduler(p);
	ਜੇ (!retval) {
		retval = p->policy;
		ਜੇ (p->sched_reset_on_fork)
			retval |= SCHED_RESET_ON_FORK;
	}
	ਵਾਪਸ retval;
}

/**
 * sys_sched_getparam - get the RT priority of a thread
 * @pid: the pid in question.
 * @param: structure containing the RT priority.
 *
 * Return: On success, 0 and the RT priority is in @param. Otherwise, an error
 * code.
 */
SYSCALL_DEFINE2(sched_getparam, pid_t, pid, ਬਣਤਰ sched_param __user *, param)
{
	ਬਣਤਰ sched_param lp = { .sched_ਤਰਜੀਹ = 0 };
	ਬਣਤਰ task_struct *p;
	ਪੂਰਨ retval;

	ਜੇ (unlikely(!param || pid < 0))
		ਵਾਪਸ -EINVAL;

	scoped_guard (rcu) {
		p = ਲੱਭੋ_process_by_pid(pid);
		ਜੇ (!p)
			ਵਾਪਸ -ESRCH;

		retval = security_task_getscheduler(p);
		ਜੇ (retval)
			ਵਾਪਸ retval;

		ਜੇ (task_has_rt_policy(p))
			lp.sched_ਤਰਜੀਹ = p->rt_ਤਰਜੀਹ;
	}

	/*
	 * This one might sleep, we cannot do it with a spinlock held ...
	 */
	ਵਾਪਸ copy_ਮੰਜ਼ਿਲ_user(param, &lp, ਆਕਾਰ(*param)) ? -EFAULT : 0;
}

/**
 * sys_sched_getattr - similar to sched_getparam, but with sched_attr
 * @pid: the pid in question.
 * @uattr: structure containing the extended parameters.
 * @usize: sizeof(attr) for fwd/bwd comp.
 * @flags: for future extension.
 */
SYSCALL_DEFINE4(sched_getattr, pid_t, pid, ਬਣਤਰ sched_attr __user *, uattr,
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ, usize, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ, flags)
{
	ਬਣਤਰ sched_attr kattr = { };
	ਬਣਤਰ task_struct *p;
	ਪੂਰਨ retval;

	ਜੇ (unlikely(!uattr || pid < 0 || usize > ਪੰਨਾ_ਮਾਪ ||
		     usize < SCHED_ATTR_ਮਾਪ_VER0))
		ਵਾਪਸ -EINVAL;

	scoped_guard (rcu) {
		p = ਲੱਭੋ_process_by_pid(pid);
		ਜੇ (!p)
			ਵਾਪਸ -ESRCH;

		ਜੇ (flags) {
			ਜੇ (!task_has_dl_policy(p) ||
			    flags != SCHED_GETATTR_FLAG_DL_DYNAMIC)
				ਵਾਪਸ -EINVAL;
		}

		retval = security_task_getscheduler(p);
		ਜੇ (retval)
			ਵਾਪਸ retval;

		kattr.sched_policy = p->policy;
		ਜੇ (p->sched_reset_on_fork)
			kattr.sched_flags |= SCHED_FLAG_RESET_ON_FORK;
		get_params(p, &kattr, flags);
		kattr.sched_flags &= SCHED_FLAG_ALL;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_UCLAMP_TASK
		/*
		 * This could race with another potential updater, but this is fine
		 * because it'll correctly read the old or the new value. We don't need
		 * to guarantee who wins the race as long as it doesn't return garbage.
		 */
		kattr.sched_util_min = p->uclamp_req[UCLAMP_MIN].ਮੁੱਲ;
		kattr.sched_util_max = p->uclamp_req[UCLAMP_MAX].ਮੁੱਲ;
#ਜੇ_ਅੰਤ
	}

	kattr.ਮਾਪ = min(usize, ਆਕਾਰ(kattr));
	ਵਾਪਸ copy_struct_ਮੰਜ਼ਿਲ_user(uattr, usize, &kattr, ਆਕਾਰ(kattr), ਨਿਰਾ);
}

ਪੂਰਨ dl_task_check_affinity(ਬਣਤਰ task_struct *p, ਅਚਲ ਬਣਤਰ cpumask *ਨਕਾਬ)
{
	/*
	 * If the task isn't a deadline task or admission control is
	 * disabled then we don't care about affinity changes.
	 */
	ਜੇ (!task_has_dl_policy(p) || !dl_bandwidth_enabled())
		ਵਾਪਸ 0;

	/*
	 * The special/sugov task isn't part of regular bandwidth/admission
	 * control so let userspace change affinities.
	 */
	ਜੇ (dl_entity_is_special(&p->dl))
		ਵਾਪਸ 0;

	/*
	 * Since bandwidth control happens on root_domain basis,
	 * if admission test is enabled, we only admit -deadline
	 * tasks allowed to run on all the CPUs in the task's
	 * root_domain.
	 */
	guard(rcu)();
	ਜੇ (!cpumask_subset(task_rq(p)->rd->ਵਿਸਤਾਰ, ਨਕਾਬ))
		ਵਾਪਸ -EBUSY;

	ਵਾਪਸ 0;
}

ਪੂਰਨ __sched_setaffinity(ਬਣਤਰ task_struct *p, ਬਣਤਰ affinity_context *ctx)
{
	ਪੂਰਨ retval;
	cpumask_var_t cpus_allowed, new_ਨਕਾਬ;

	ਜੇ (!alloc_cpumask_var(&cpus_allowed, GFP_KERNEL))
		ਵਾਪਸ -ENOMEM;

	ਜੇ (!alloc_cpumask_var(&new_ਨਕਾਬ, GFP_KERNEL)) {
		retval = -ENOMEM;
		ਜਾਓ ਬਾਹਰਲਾ_free_cpus_allowed;
	}

	cpuset_cpus_allowed(p, cpus_allowed);
	cpumask_and(new_ਨਕਾਬ, ctx->new_ਨਕਾਬ, cpus_allowed);

	ctx->new_ਨਕਾਬ = new_ਨਕਾਬ;
	ctx->flags |= SCA_CHECK;

	retval = dl_task_check_affinity(p, new_ਨਕਾਬ);
	ਜੇ (retval)
		ਜਾਓ ਬਾਹਰਲਾ_free_new_ਨਕਾਬ;

	retval = __set_cpus_allowed_ptr(p, ctx);
	ਜੇ (retval)
		ਜਾਓ ਬਾਹਰਲਾ_free_new_ਨਕਾਬ;

	cpuset_cpus_allowed(p, cpus_allowed);
	ਜੇ (!cpumask_subset(new_ਨਕਾਬ, cpus_allowed)) {
		/*
		 * We must have raced with a concurrent cpuset update.
		 * Just reset the cpumask to the cpuset's cpus_allowed.
		 */
		cpumask_copy(new_ਨਕਾਬ, cpus_allowed);

		/*
		 * If SCA_USER is set, a 2nd call to __set_cpus_allowed_ptr()
		 * will restore the previous user_cpus_ptr value.
		 *
		 * In the unlikely event a previous user_cpus_ptr exists,
		 * we need to further restrict the mask to what is allowed
		 * by that old user_cpus_ptr.
		 */
		ਜੇ (unlikely((ctx->flags & SCA_USER) && ctx->user_ਨਕਾਬ)) {
			bool ਖਾਲੀ_ਥਾਵਾਂ = !cpumask_and(new_ਨਕਾਬ, new_ਨਕਾਬ,
						  ctx->user_ਨਕਾਬ);

			ਜੇ (ਖਾਲੀ_ਥਾਵਾਂ)
				cpumask_copy(new_ਨਕਾਬ, cpus_allowed);
		}
		__set_cpus_allowed_ptr(p, ctx);
		retval = -EINVAL;
	}

ਬਾਹਰਲਾ_free_new_ਨਕਾਬ:
	free_cpumask_var(new_ਨਕਾਬ);
ਬਾਹਰਲਾ_free_cpus_allowed:
	free_cpumask_var(cpus_allowed);
	ਵਾਪਸ retval;
}

ਲੰਮਾ sched_setaffinity(pid_t pid, ਅਚਲ ਬਣਤਰ cpumask *ਵਿੱਚ_ਨਕਾਬ)
{
	ਬਣਤਰ affinity_context ac;
	ਬਣਤਰ cpumask *user_ਨਕਾਬ;
	ਪੂਰਨ retval;

	CLASS(ਲੱਭੋ_get_task, p)(pid);
	ਜੇ (!p)
		ਵਾਪਸ -ESRCH;

	ਜੇ (p->flags & PF_NO_SETAFFINITY)
		ਵਾਪਸ -EINVAL;

	ਜੇ (!check_same_owner(p)) {
		guard(rcu)();
		ਜੇ (!ns_capable(__task_cred(p)->user_ns, CAP_SYS_NICE))
			ਵਾਪਸ -EPERM;
	}

	retval = security_task_setscheduler(p);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	/*
	 * With non-SMP configs, user_cpus_ptr/user_mask isn't used and
	 * alloc_user_cpus_ptr() returns NULL.
	 */
	user_ਨਕਾਬ = alloc_user_cpus_ptr(NUMA_NO_ਗੰਢ);
	ਜੇ (user_ਨਕਾਬ) {
		cpumask_copy(user_ਨਕਾਬ, ਵਿੱਚ_ਨਕਾਬ);
	} ਨਹੀਂਤਾਂ {
		ਵਾਪਸ -ENOMEM;
	}

	ac = (ਬਣਤਰ affinity_context){
		.new_ਨਕਾਬ  = ਵਿੱਚ_ਨਕਾਬ,
		.user_ਨਕਾਬ = user_ਨਕਾਬ,
		.flags     = SCA_USER,
	};

	retval = __sched_setaffinity(p, &ac);
	kfree(ac.user_ਨਕਾਬ);

	ਵਾਪਸ retval;
}

ਸਥਿਰ ਪੂਰਨ get_user_cpu_ਨਕਾਬ(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ __user *user_ਨਕਾਬ_ptr, ਬਿਨਾਨਿਸ਼ਾਨੀ len,
			     ਬਣਤਰ cpumask *new_ਨਕਾਬ)
{
	ਜੇ (len < cpumask_ਮਾਪ())
		cpumask_clear(new_ਨਕਾਬ);
	ਨਹੀਂਤਾਂ ਜੇ (len > cpumask_ਮਾਪ())
		len = cpumask_ਮਾਪ();

	ਵਾਪਸ copy_from_user(new_ਨਕਾਬ, user_ਨਕਾਬ_ptr, len) ? -EFAULT : 0;
}

/**
 * sys_sched_setaffinity - set the CPU affinity of a process
 * @pid: pid of the process
 * @len: length in bytes of the bitmask pointed to by user_mask_ptr
 * @user_mask_ptr: user-space pointer to the new CPU mask
 *
 * Return: 0 on success. An error code otherwise.
 */
SYSCALL_DEFINE3(sched_setaffinity, pid_t, pid, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ, len,
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ __user *, user_ਨਕਾਬ_ptr)
{
	cpumask_var_t new_ਨਕਾਬ;
	ਪੂਰਨ retval;

	ਜੇ (!alloc_cpumask_var(&new_ਨਕਾਬ, GFP_KERNEL))
		ਵਾਪਸ -ENOMEM;

	retval = get_user_cpu_ਨਕਾਬ(user_ਨਕਾਬ_ptr, len, new_ਨਕਾਬ);
	ਜੇ (retval == 0)
		retval = sched_setaffinity(pid, new_ਨਕਾਬ);
	free_cpumask_var(new_ਨਕਾਬ);
	ਵਾਪਸ retval;
}

ਲੰਮਾ sched_getaffinity(pid_t pid, ਬਣਤਰ cpumask *ਨਕਾਬ)
{
	ਬਣਤਰ task_struct *p;
	ਪੂਰਨ retval;

	guard(rcu)();
	p = ਲੱਭੋ_process_by_pid(pid);
	ਜੇ (!p)
		ਵਾਪਸ -ESRCH;

	retval = security_task_getscheduler(p);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	guard(raw_spinlock_irqsave)(&p->pi_ਤਾਲਾ);
	cpumask_and(ਨਕਾਬ, &p->cpus_ਨਕਾਬ, cpu_ਸਰਗਰਮ_ਨਕਾਬ);

	ਵਾਪਸ 0;
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
SYSCALL_DEFINE3(sched_getaffinity, pid_t, pid, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ, len,
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ __user *, user_ਨਕਾਬ_ptr)
{
	ਪੂਰਨ ret;
	cpumask_var_t ਨਕਾਬ;

	ਜੇ ((len * ਬਿੱਟ_ਪ੍ਰਤੀ_BYTE) < nr_cpu_ids)
		ਵਾਪਸ -EINVAL;
	ਜੇ (len & (ਆਕਾਰ(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ)-1))
		ਵਾਪਸ -EINVAL;

	ਜੇ (!zalloc_cpumask_var(&ਨਕਾਬ, GFP_KERNEL))
		ਵਾਪਸ -ENOMEM;

	ret = sched_getaffinity(pid, ਨਕਾਬ);
	ਜੇ (ret == 0) {
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ retlen = min(len, cpumask_ਮਾਪ());

		ਜੇ (copy_ਮੰਜ਼ਿਲ_user(user_ਨਕਾਬ_ptr, cpumask_ਬਿੱਟ(ਨਕਾਬ), retlen))
			ret = -EFAULT;
		ਨਹੀਂਤਾਂ
			ret = retlen;
	}
	free_cpumask_var(ਨਕਾਬ);

	ਵਾਪਸ ret;
}

ਸਥਿਰ ਖਾਲੀ do_sched_yield(ਖਾਲੀ)
{
	ਬਣਤਰ rq_flags rf;
	ਬਣਤਰ rq *rq;

	rq = this_rq_ਤਾਲਾ_irq(&rf);

	schedstat_inc(rq->yld_ਗਿਣਤੀ);
	rq->donor->sched_class->yield_task(rq);

	preempt_disable();
	rq_unlock_irq(rq, &rf);
	sched_preempt_enable_no_resched();

	ਸਮਾਂ_ਸਾਰਣੀ();
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
	ਵਾਪਸ 0;
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
ਖਾਲੀ __sched yield(ਖਾਲੀ)
{
	ਸੈੱਟ_current_state(TASK_RUNNING);
	do_sched_yield();
}
EXPORT_ਚਿੰਨ੍ਹ(yield);

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
ਪੂਰਨ __sched yield_ਮੰਜ਼ਿਲ(ਬਣਤਰ task_struct *p, bool preempt)
{
	ਬਣਤਰ task_struct *curr;
	ਬਣਤਰ rq *rq, *p_rq;
	ਪੂਰਨ yielded = 0;

	scoped_guard (raw_spinlock_irqsave, &p->pi_ਤਾਲਾ) {
		rq = this_rq();
		curr = rq->donor;

again:
		p_rq = task_rq(p);
		/*
		 * If we're the only runnable task on the rq and target rq also
		 * has only one task, there's absolutely no point in yielding.
		 */
		ਜੇ (rq->nr_running == 1 && p_rq->nr_running == 1)
			ਵਾਪਸ -ESRCH;

		guard(double_rq_ਤਾਲਾ)(rq, p_rq);
		ਜੇ (task_rq(p) != p_rq)
			ਜਾਓ again;

		ਜੇ (!curr->sched_class->yield_ਮੰਜ਼ਿਲ_task)
			ਵਾਪਸ 0;

		ਜੇ (curr->sched_class != p->sched_class)
			ਵਾਪਸ 0;

		ਜੇ (task_on_cpu(p_rq, p) || !task_is_running(p))
			ਵਾਪਸ 0;

		yielded = curr->sched_class->yield_ਮੰਜ਼ਿਲ_task(rq, p);
		ਜੇ (yielded) {
			schedstat_inc(rq->yld_ਗਿਣਤੀ);
			/*
			 * Make p's CPU reschedule; pick_next_entity
			 * takes care of fairness.
			 */
			ਜੇ (preempt && rq != p_rq)
				resched_curr(p_rq);
		}
	}

	ਜੇ (yielded)
		ਸਮਾਂ_ਸਾਰਣੀ();

	ਵਾਪਸ yielded;
}
EXPORT_ਚਿੰਨ੍ਹ_GPL(yield_ਮੰਜ਼ਿਲ);

/**
 * sys_sched_get_priority_max - return maximum RT priority.
 * @policy: scheduling class.
 *
 * Return: On success, this syscall returns the maximum
 * rt_priority that can be used by a given scheduling class.
 * On failure, a negative error code is returned.
 */
SYSCALL_DEFINE1(sched_get_ਤਰਜੀਹ_max, ਪੂਰਨ, policy)
{
	ਪੂਰਨ ret = -EINVAL;

	ਚੋਣ (policy) {
	ਹਾਲਤ SCHED_FIFO:
	ਹਾਲਤ SCHED_RR:
		ret = MAX_RT_PRIO-1;
		ਰੋਕੋ;
	ਹਾਲਤ SCHED_DEADLINE:
	ਹਾਲਤ SCHED_ਸਾਧਾਰਨ:
	ਹਾਲਤ SCHED_BATCH:
	ਹਾਲਤ SCHED_IDLE:
	ਹਾਲਤ SCHED_EXT:
		ret = 0;
		ਰੋਕੋ;
	}
	ਵਾਪਸ ret;
}

/**
 * sys_sched_get_priority_min - return minimum RT priority.
 * @policy: scheduling class.
 *
 * Return: On success, this syscall returns the minimum
 * rt_priority that can be used by a given scheduling class.
 * On failure, a negative error code is returned.
 */
SYSCALL_DEFINE1(sched_get_ਤਰਜੀਹ_min, ਪੂਰਨ, policy)
{
	ਪੂਰਨ ret = -EINVAL;

	ਚੋਣ (policy) {
	ਹਾਲਤ SCHED_FIFO:
	ਹਾਲਤ SCHED_RR:
		ret = 1;
		ਰੋਕੋ;
	ਹਾਲਤ SCHED_DEADLINE:
	ਹਾਲਤ SCHED_ਸਾਧਾਰਨ:
	ਹਾਲਤ SCHED_BATCH:
	ਹਾਲਤ SCHED_IDLE:
	ਹਾਲਤ SCHED_EXT:
		ret = 0;
	}
	ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ sched_rr_get_interval(pid_t pid, ਬਣਤਰ timespec64 *t)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਮਾਂ_ਟੁਕੜਾ = 0;
	ਪੂਰਨ retval;

	ਜੇ (pid < 0)
		ਵਾਪਸ -EINVAL;

	scoped_guard (rcu) {
		ਬਣਤਰ task_struct *p = ਲੱਭੋ_process_by_pid(pid);
		ਜੇ (!p)
			ਵਾਪਸ -ESRCH;

		retval = security_task_getscheduler(p);
		ਜੇ (retval)
			ਵਾਪਸ retval;

		scoped_guard (task_rq_ਤਾਲਾ_2, p) {
			ਬਣਤਰ rq *rq = scope.rq;
			ਜੇ (p->sched_class->get_rr_interval)
				ਸਮਾਂ_ਟੁਕੜਾ = p->sched_class->get_rr_interval(rq, p);
		}
	}

	jiffies_ਮੰਜ਼ਿਲ_timespec64(ਸਮਾਂ_ਟੁਕੜਾ, t);
	ਵਾਪਸ 0;
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
		ਬਣਤਰ __kernel_timespec __user *, interval)
{
	ਬਣਤਰ timespec64 t;
	ਪੂਰਨ retval = sched_rr_get_interval(pid, &t);

	ਜੇ (retval == 0)
		retval = put_timespec64(&t, interval);

	ਵਾਪਸ retval;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_COMPAT_32_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸਮਾਂ
SYSCALL_DEFINE2(sched_rr_get_interval_time32, pid_t, pid,
		ਬਣਤਰ old_timespec32 __user *, interval)
{
	ਬਣਤਰ timespec64 t;
	ਪੂਰਨ retval = sched_rr_get_interval(pid, &t);

	ਜੇ (retval == 0)
		retval = put_old_timespec32(&t, interval);
	ਵਾਪਸ retval;
}
#ਜੇ_ਅੰਤ
