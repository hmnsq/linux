// SPDX-License-Identifier: GPL-2.0-only
/*
 *  kernel/sched/syscalls.c
 *
 *  Core kernel scheduler syscalls related code
 *
 *  Copyright (C) 1991-2002  Linus Torvalds
 *  Copyright (C) 1998-2024  Ingo Molnar, Red Hat
 */
#अन्तर्भाव <linux/sched.h>
#अन्तर्भाव <linux/cpuset.h>
#अन्तर्भाव <linux/sched/debug.h>

#अन्तर्भाव <uapi/linux/sched/types.h>

#अन्तर्भाव "sched.h"
#अन्तर्भाव "autogroup.h"

स्थिर अन्तःस्थ पूर्णाङ्क __normal_prio(पूर्णाङ्क policy, पूर्णाङ्क rt_prio, पूर्णाङ्क nice)
{
	पूर्णाङ्क prio;

	जइ (dl_policy(policy))
		prio = MAX_DL_PRIO - 1;
	अण्णहा जइ (rt_policy(policy))
		prio = MAX_RT_PRIO - 1 - rt_prio;
	अण्णहा
		prio = NICE_TO_PRIO(nice);

	प्रत्यागम prio;
}

/*
 * Calculate the expected normal priority: i.e. priority
 * without taking RT-inheritance into account. Might be
 * boosted by interactivity modifiers. Changes upon fork,
 * setprio syscalls, and whenever the interactivity
 * estimator recalculates.
 */
स्थिर अन्तःस्थ पूर्णाङ्क normal_prio(संरचना task_struct *p)
{
	प्रत्यागम __normal_prio(p->policy, p->rt_priority, PRIO_TO_NICE(p->static_prio));
}

/*
 * Calculate the current priority, i.e. the priority
 * taken into account by the scheduler. This value might
 * be boosted by RT tasks, or might be boosted by
 * interactivity modifiers. Will be RT if the task got
 * RT-boosted. If not then it returns p->normal_prio.
 */
स्थिर पूर्णाङ्क effective_prio(संरचना task_struct *p)
{
	p->normal_prio = normal_prio(p);
	/*
	 * If we are RT tasks or we were boosted to RT priority,
	 * keep the priority unchanged. Otherwise, update priority
	 * to the normal priority:
	 */
	जइ (!rt_or_dl_prio(p->prio))
		प्रत्यागम p->normal_prio;
	प्रत्यागम p->prio;
}

शून्य set_user_nice(संरचना task_struct *p, long nice)
{
	पूर्णाङ्क old_prio;

	जइ (task_nice(p) == nice || nice < MIN_NICE || nice > MAX_NICE)
		प्रत्यागम;
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
	जइ (task_has_dl_policy(p) || task_has_rt_policy(p)) {
		p->static_prio = NICE_TO_PRIO(nice);
		प्रत्यागम;
	}

	scoped_guard (sched_change, p, DEQUEUE_SAVE) {
		p->static_prio = NICE_TO_PRIO(nice);
		set_load_weight(p, true);
		old_prio = p->prio;
		p->prio = effective_prio(p);
	}
}
EXPORT_चिह्नम्(set_user_nice);

/*
 * is_nice_reduction - check if nice value is an actual reduction
 *
 * Similar to can_nice() but does not perform a capability check.
 *
 * @p: task
 * @nice: nice value
 */
स्थिर bool is_nice_reduction(ध्रुव संरचना task_struct *p, ध्रुव पूर्णाङ्क nice)
{
	/* Convert nice value [19,-20] to rlimit style value [1,40]: */
	पूर्णाङ्क nice_rlim = nice_to_rlimit(nice);

	प्रत्यागम (nice_rlim <= task_rlimit(p, RLIMIT_NICE));
}

/*
 * can_nice - check if a task can reduce its nice value
 * @p: task
 * @nice: nice value
 */
पूर्णाङ्क can_nice(ध्रुव संरचना task_struct *p, ध्रुव पूर्णाङ्क nice)
{
	प्रत्यागम is_nice_reduction(p, nice) || capable(CAP_SYS_NICE);
}

#ifdef __ARCH_WANT_SYS_NICE

/*
 * sys_nice - change the priority of the current process.
 * @increment: priority increment
 *
 * sys_setpriority is a more generic, but much slower function that
 * does similar things.
 */
SYSCALL_DEFINE1(nice, पूर्णाङ्क, increment)
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
	जइ (increment < 0 && !can_nice(current, nice))
		प्रत्यागम -EPERM;

	retval = security_task_setnice(current, nice);
	जइ (retval)
		प्रत्यागम retval;

	set_user_nice(current, nice);
	प्रत्यागम 0;
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
पूर्णाङ्क task_prio(ध्रुव संरचना task_struct *p)
{
	प्रत्यागम p->prio - MAX_RT_PRIO;
}

/**
 * idle_cpu - is a given CPU idle currently?
 * @cpu: the processor in question.
 *
 * Return: 1 if the CPU is currently idle. 0 otherwise.
 */
पूर्णाङ्क idle_cpu(पूर्णाङ्क cpu)
{
	प्रत्यागम idle_rq(cpu_rq(cpu));
}

/**
 * idle_task - return the idle task for a given CPU.
 * @cpu: the processor in question.
 *
 * Return: The idle task for the CPU @cpu.
 */
संरचना task_struct *idle_task(पूर्णाङ्क cpu)
{
	प्रत्यागम cpu_rq(cpu)->idle;
}

#ifdef CONFIG_SCHED_CORE
पूर्णाङ्क sched_core_idle_cpu(पूर्णाङ्क cpu)
{
	संरचना rq *rq = cpu_rq(cpu);

	जइ (sched_core_enabled(rq) && rq->curr == rq->idle)
		प्रत्यागम 1;

	प्रत्यागम idle_cpu(cpu);
}
#endif /* CONFIG_SCHED_CORE */

/**
 * find_process_by_pid - find a process with a matching PID value.
 * @pid: the pid in question.
 *
 * The task of @pid, if found. %NULL otherwise.
 */
स्थिर संरचना task_struct *find_process_by_pid(pid_t pid)
{
	प्रत्यागम pid ? find_task_by_vpid(pid) : current;
}

स्थिर संरचना task_struct *find_get_task(pid_t pid)
{
	संरचना task_struct *p;
	guard(rcu)();

	p = find_process_by_pid(pid);
	जइ (likely(p))
		get_task_struct(p);

	प्रत्यागम p;
}

DEFINE_CLASS(find_get_task, संरचना task_struct *, जइ (_T) put_task_struct(_T),
	     find_get_task(pid), pid_t pid)

/*
 * sched_setparam() passes in -1 for its policy, to let the functions
 * it calls know not to change it.
 */
#define SETPARAM_POLICY	-1

स्थिर शून्य __setscheduler_params(संरचना task_struct *p,
		ध्रुव संरचना sched_attr *attr)
{
	पूर्णाङ्क policy = attr->sched_policy;

	जइ (policy == SETPARAM_POLICY)
		policy = p->policy;

	p->policy = policy;

	जइ (dl_policy(policy))
		__setparam_dl(p, attr);
	अण्णहा जइ (fair_policy(policy))
		__setparam_fair(p, attr);

	/* rt-policy tasks do not have a timerslack */
	जइ (rt_or_dl_task_policy(p)) {
		p->timer_slack_ns = 0;
	} अण्णहा जइ (p->timer_slack_ns == 0) {
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
स्थिर bool check_same_owner(संरचना task_struct *p)
{
	ध्रुव संरचना cred *cred = current_cred(), *pcred;
	guard(rcu)();

	pcred = __task_cred(p);
	प्रत्यागम (uid_eq(cred->euid, pcred->euid) ||
		uid_eq(cred->euid, pcred->uid));
}

#ifdef CONFIG_RT_MUTEXES
स्थिर अन्तःस्थ शून्य __setscheduler_dl_pi(पूर्णाङ्क newprio, पूर्णाङ्क policy,
			      संरचना task_struct *p,
			      संरचना sched_change_ctx *scope)
{
	/*
	 * In case a DEADLINE task (either proper or boosted) gets
	 * setscheduled to a lower priority class, check if it neeeds to
	 * inherit parameters from a potential pi_task. In that case make
	 * sure replenishment happens with the next enqueue.
	 */

	जइ (dl_prio(newprio) && !dl_policy(policy)) {
		संरचना task_struct *pi_task = rt_mutex_get_शिखरम्_task(p);

		जइ (pi_task) {
			p->dl.pi_se = pi_task->dl.pi_se;
			scope->flags |= ENQUEUE_REPLENISH;
		}
	}
}
#अण्णहा /* !CONFIG_RT_MUTEXES */
स्थिर अन्तःस्थ शून्य __setscheduler_dl_pi(पूर्णाङ्क newprio, पूर्णाङ्क policy,
			      संरचना task_struct *p,
			      संरचना sched_change_ctx *scope)
{
}
#endif /* !CONFIG_RT_MUTEXES */

#ifdef CONFIG_UCLAMP_TASK

स्थिर पूर्णाङ्क uclamp_validate(संरचना task_struct *p,
			   ध्रुव संरचना sched_attr *attr)
{
	पूर्णाङ्क util_min = p->uclamp_req[UCLAMP_MIN].मानम्;
	पूर्णाङ्क util_max = p->uclamp_req[UCLAMP_MAX].मानम्;

	जइ (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MIN) {
		util_min = attr->sched_util_min;

		जइ (util_min + 1 > SCHED_CAPACITY_SCALE + 1)
			प्रत्यागम -EINVAL;
	}

	जइ (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MAX) {
		util_max = attr->sched_util_max;

		जइ (util_max + 1 > SCHED_CAPACITY_SCALE + 1)
			प्रत्यागम -EINVAL;
	}

	जइ (util_min != -1 && util_max != -1 && util_min > util_max)
		प्रत्यागम -EINVAL;

	/*
	 * We have valid uclamp attributes; make sure uclamp is enabled.
	 *
	 * We need to do that here, because enabling static branches is a
	 * blocking operation which obviously cannot be done while holding
	 * scheduler locks.
	 */
	sched_uclamp_enable();

	प्रत्यागम 0;
}

स्थिर bool uclamp_reset(ध्रुव संरचना sched_attr *attr,
			 गणना uclamp_id clamp_id,
			 संरचना uclamp_se *uc_se)
{
	/* Reset on sched class change for a non user-defined clamp value. */
	जइ (likely(!(attr->sched_flags & SCHED_FLAG_UTIL_CLAMP)) &&
	    !uc_se->user_defined)
		प्रत्यागम true;

	/* Reset on sched_util_{min,max} == -1. */
	जइ (clamp_id == UCLAMP_MIN &&
	    attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MIN &&
	    attr->sched_util_min == -1) {
		प्रत्यागम true;
	}

	जइ (clamp_id == UCLAMP_MAX &&
	    attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MAX &&
	    attr->sched_util_max == -1) {
		प्रत्यागम true;
	}

	प्रत्यागम false;
}

स्थिर शून्य __setscheduler_uclamp(संरचना task_struct *p,
				  ध्रुव संरचना sched_attr *attr)
{
	गणना uclamp_id clamp_id;

	for_each_clamp_id(clamp_id) {
		संरचना uclamp_se *uc_se = &p->uclamp_req[clamp_id];
		unsigned पूर्णाङ्क मानम्;

		जइ (!uclamp_reset(attr, clamp_id, uc_se))
			अनुवर्त;

		/*
		 * RT by default have a 100% boost value that could be modified
		 * at runtime.
		 */
		जइ (unlikely(rt_task(p) && clamp_id == UCLAMP_MIN))
			मानम् = sysctl_sched_uclamp_util_min_rt_default;
		अण्णहा
			मानम् = uclamp_none(clamp_id);

		uclamp_se_set(uc_se, मानम्, false);

	}

	जइ (likely(!(attr->sched_flags & SCHED_FLAG_UTIL_CLAMP)))
		प्रत्यागम;

	जइ (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MIN &&
	    attr->sched_util_min != -1) {
		uclamp_se_set(&p->uclamp_req[UCLAMP_MIN],
			      attr->sched_util_min, true);
	}

	जइ (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MAX &&
	    attr->sched_util_max != -1) {
		uclamp_se_set(&p->uclamp_req[UCLAMP_MAX],
			      attr->sched_util_max, true);
	}
}

#अण्णहा /* !CONFIG_UCLAMP_TASK: */

स्थिर अन्तःस्थ पूर्णाङ्क uclamp_validate(संरचना task_struct *p,
				  ध्रुव संरचना sched_attr *attr)
{
	प्रत्यागम -EOPNOTSUPP;
}
स्थिर शून्य __setscheduler_uclamp(संरचना task_struct *p,
				  ध्रुव संरचना sched_attr *attr) { }
#endif /* !CONFIG_UCLAMP_TASK */

/*
 * Allow unprivileged RT tasks to decrease priority.
 * Only issue a capable test if needed and only once to avoid an audit
 * event on permitted non-privileged operations:
 */
स्थिर पूर्णाङ्क user_check_sched_setscheduler(संरचना task_struct *p,
					 ध्रुव संरचना sched_attr *attr,
					 पूर्णाङ्क policy, पूर्णाङ्क reset_on_fork)
{
	जइ (fair_policy(policy)) {
		जइ (attr->sched_nice < task_nice(p) &&
		    !is_nice_reduction(p, attr->sched_nice))
			गच्छ req_priv;
	}

	जइ (rt_policy(policy)) {
		unsigned long rlim_rtprio = task_rlimit(p, RLIMIT_RTPRIO);

		/* Can't set/change the rt policy: */
		जइ (policy != p->policy && !rlim_rtprio)
			गच्छ req_priv;

		/* Can't increase priority: */
		जइ (attr->sched_priority > p->rt_priority &&
		    attr->sched_priority > rlim_rtprio)
			गच्छ req_priv;
	}

	/*
	 * Can't set/change SCHED_DEADLINE policy at all for now
	 * (safest behavior); in the future we would like to allow
	 * unprivileged DL tasks to increase their relative deadline
	 * or reduce their runtime (both ways reducing utilization)
	 */
	जइ (dl_policy(policy))
		गच्छ req_priv;

	/*
	 * Treat SCHED_IDLE as nice 20. Only allow a switch to
	 * SCHED_NORMAL if the RLIMIT_NICE would normally permit it.
	 */
	जइ (task_has_idle_policy(p) && !idle_policy(policy)) {
		जइ (!is_nice_reduction(p, task_nice(p)))
			गच्छ req_priv;
	}

	/* Can't change other user's priorities: */
	जइ (!check_same_owner(p))
		गच्छ req_priv;

	/* Normal users shall not reset the sched_reset_on_fork flag: */
	जइ (p->sched_reset_on_fork && !reset_on_fork)
		गच्छ req_priv;

	प्रत्यागम 0;

req_priv:
	जइ (!capable(CAP_SYS_NICE))
		प्रत्यागम -EPERM;

	प्रत्यागम 0;
}

पूर्णाङ्क __sched_setscheduler(संरचना task_struct *p,
			 ध्रुव संरचना sched_attr *attr,
			 bool user, bool pi)
{
	पूर्णाङ्क oldpolicy = -1, policy = attr->sched_policy;
	पूर्णाङ्क retval, oldprio, newprio;
	ध्रुव संरचना sched_class *prev_class, *next_class;
	संरचना balance_callback *head;
	संरचना rq_flags rf;
	पूर्णाङ्क reset_on_fork;
	पूर्णाङ्क queue_flags = DEQUEUE_SAVE | DEQUEUE_MOVE | DEQUEUE_NOCLOCK;
	संरचना rq *rq;
	bool cpuset_locked = false;

	/* The pi code expects interrupts enabled */
	BUG_ON(pi && in_interrupt());
recheck:
	/* Double check policy once rq lock held: */
	जइ (policy < 0) {
		reset_on_fork = p->sched_reset_on_fork;
		policy = oldpolicy = p->policy;
	} अण्णहा {
		reset_on_fork = !!(attr->sched_flags & SCHED_FLAG_RESET_ON_FORK);

		जइ (!valid_policy(policy))
			प्रत्यागम -EINVAL;
	}

	जइ (attr->sched_flags & ~(SCHED_FLAG_ALL | SCHED_FLAG_SUGOV))
		प्रत्यागम -EINVAL;

	/*
	 * Valid priorities for SCHED_FIFO and SCHED_RR are
	 * 1..MAX_RT_PRIO-1, valid priority for SCHED_NORMAL,
	 * SCHED_BATCH and SCHED_IDLE is 0.
	 */
	जइ (attr->sched_priority > MAX_RT_PRIO-1)
		प्रत्यागम -EINVAL;
	जइ ((dl_policy(policy) && !__checkparam_dl(attr)) ||
	    (rt_policy(policy) != (attr->sched_priority != 0)))
		प्रत्यागम -EINVAL;

	जइ (user) {
		retval = user_check_sched_setscheduler(p, attr, policy, reset_on_fork);
		जइ (retval)
			प्रत्यागम retval;

		जइ (attr->sched_flags & SCHED_FLAG_SUGOV)
			प्रत्यागम -EINVAL;

		retval = security_task_setscheduler(p);
		जइ (retval)
			प्रत्यागम retval;
	}

	/* Update task specific "requested" clamps */
	जइ (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP) {
		retval = uclamp_validate(p, attr);
		जइ (retval)
			प्रत्यागम retval;
	}

	/*
	 * SCHED_DEADLINE bandwidth accounting relies on stable cpusets
	 * information.
	 */
	जइ (dl_policy(policy) || dl_policy(p->policy)) {
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
	जइ (p == rq->stop) {
		retval = -EINVAL;
		गच्छ unlock;
	}

	retval = scx_check_setscheduler(p, policy);
	जइ (retval)
		गच्छ unlock;

	/*
	 * If not changing anything there's no need to proceed further,
	 * but store a possible modification of reset_on_fork.
	 */
	जइ (unlikely(policy == p->policy)) {
		जइ (fair_policy(policy) &&
		    (attr->sched_nice != task_nice(p) ||
		     (attr->sched_runtime != p->se.slice)))
			गच्छ change;
		जइ (rt_policy(policy) && attr->sched_priority != p->rt_priority)
			गच्छ change;
		जइ (dl_policy(policy) && dl_param_changed(p, attr))
			गच्छ change;
		जइ (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP)
			गच्छ change;

		p->sched_reset_on_fork = reset_on_fork;
		retval = 0;
		गच्छ unlock;
	}
change:

	जइ (user) {
#ifdef CONFIG_RT_GROUP_SCHED
		/*
		 * Do not allow real-time tasks into groups that have no runtime
		 * assigned.
		 */
		जइ (rt_group_sched_enabled() &&
				rt_bandwidth_enabled() && rt_policy(policy) &&
				task_group(p)->rt_bandwidth.rt_runtime == 0 &&
				!task_group_is_autogroup(task_group(p))) {
			retval = -EPERM;
			गच्छ unlock;
		}
#endif /* CONFIG_RT_GROUP_SCHED */
		जइ (dl_bandwidth_enabled() && dl_policy(policy) &&
				!(attr->sched_flags & SCHED_FLAG_SUGOV)) {
			cpumask_t *span = rq->rd->span;

			/*
			 * Don't allow tasks with an affinity mask smaller than
			 * the entire root_domain to become SCHED_DEADLINE. We
			 * will also fail if there's no bandwidth available.
			 */
			जइ (!cpumask_subset(span, p->cpus_ptr) ||
			    rq->rd->dl_bw.bw == 0) {
				retval = -EPERM;
				गच्छ unlock;
			}
		}
	}

	/* Re-check policy now with rq lock held: */
	जइ (unlikely(oldpolicy != -1 && oldpolicy != p->policy)) {
		policy = oldpolicy = -1;
		task_rq_unlock(rq, p, &rf);
		जइ (cpuset_locked)
			cpuset_unlock();
		गच्छ recheck;
	}

	/*
	 * If setscheduling to SCHED_DEADLINE (or changing the parameters
	 * of a SCHED_DEADLINE task) we need to check if enough bandwidth
	 * is available.
	 */
	जइ ((dl_policy(policy) || dl_task(p)) && sched_dl_overflow(p, policy, attr)) {
		retval = -EBUSY;
		गच्छ unlock;
	}

	p->sched_reset_on_fork = reset_on_fork;
	oldprio = p->prio;

	newprio = __normal_prio(policy, attr->sched_priority, attr->sched_nice);
	जइ (pi) {
		/*
		 * Take priority boosted tasks into account. If the new
		 * effective priority is unchanged, we just store the new
		 * normal parameters and do not touch the scheduler class and
		 * the runqueue. This will be done when the task deboost
		 * itself.
		 */
		newprio = rt_effective_prio(p, newprio);
		जइ (newprio == oldprio && !dl_prio(newprio))
			queue_flags &= ~DEQUEUE_MOVE;
	}

	prev_class = p->sched_class;
	next_class = __setscheduler_class(policy, newprio);

	जइ (prev_class != next_class)
		queue_flags |= DEQUEUE_CLASS;

	scoped_guard (sched_change, p, queue_flags) {

		जइ (!(attr->sched_flags & SCHED_FLAG_KEEP_PARAMS)) {
			__setscheduler_params(p, attr);
			p->sched_class = next_class;
			p->prio = newprio;
			__setscheduler_dl_pi(newprio, policy, p, scope);
		}
		__setscheduler_uclamp(p, attr);

		जइ (scope->queued) {
			/*
			 * We enqueue to tail when the priority of a task is
			 * increased (user space view).
			 */
			जइ (oldprio < p->prio)
				scope->flags |= ENQUEUE_HEAD;
		}
	}

	/* Avoid rq from going away on us: */
	preempt_disable();
	head = splice_balance_callbacks(rq);
	task_rq_unlock(rq, p, &rf);

	जइ (pi) {
		जइ (cpuset_locked)
			cpuset_unlock();
		rt_mutex_adjust_pi(p);
	}

	/* Run balance callbacks after we've adjusted the PI chain: */
	balance_callbacks(rq, head);
	preempt_enable();

	प्रत्यागम 0;

unlock:
	task_rq_unlock(rq, p, &rf);
	जइ (cpuset_locked)
		cpuset_unlock();
	प्रत्यागम retval;
}

स्थिर पूर्णाङ्क _sched_setscheduler(संरचना task_struct *p, पूर्णाङ्क policy,
			       ध्रुव संरचना sched_param *param, bool check)
{
	संरचना sched_attr attr = {
		.sched_policy   = policy,
		.sched_priority = param->sched_priority,
		.sched_nice	= PRIO_TO_NICE(p->static_prio),
	};

	जइ (p->se.custom_slice)
		attr.sched_runtime = p->se.slice;

	/* Fixup the legacy SCHED_RESET_ON_FORK hack. */
	जइ ((policy != SETPARAM_POLICY) && (policy & SCHED_RESET_ON_FORK)) {
		attr.sched_flags |= SCHED_FLAG_RESET_ON_FORK;
		policy &= ~SCHED_RESET_ON_FORK;
		attr.sched_policy = policy;
	}

	प्रत्यागम __sched_setscheduler(p, &attr, check, true);
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
पूर्णाङ्क sched_setscheduler(संरचना task_struct *p, पूर्णाङ्क policy,
		       ध्रुव संरचना sched_param *param)
{
	प्रत्यागम _sched_setscheduler(p, policy, param, true);
}

पूर्णाङ्क sched_setattr(संरचना task_struct *p, ध्रुव संरचना sched_attr *attr)
{
	प्रत्यागम __sched_setscheduler(p, attr, true, true);
}

पूर्णाङ्क sched_setattr_nocheck(संरचना task_struct *p, ध्रुव संरचना sched_attr *attr)
{
	प्रत्यागम __sched_setscheduler(p, attr, false, true);
}
EXPORT_चिह्नम्_GPL(sched_setattr_nocheck);

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
पूर्णाङ्क sched_setscheduler_nocheck(संरचना task_struct *p, पूर्णाङ्क policy,
			       ध्रुव संरचना sched_param *param)
{
	प्रत्यागम _sched_setscheduler(p, policy, param, false);
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
शून्य sched_set_fifo(संरचना task_struct *p)
{
	संरचना sched_param sp = { .sched_priority = MAX_RT_PRIO / 2 };
	WARN_ON_ONCE(sched_setscheduler_nocheck(p, SCHED_FIFO, &sp) != 0);
}
EXPORT_चिह्नम्_GPL(sched_set_fifo);

/*
 * For when you don't much care about FIFO, but want to be above SCHED_NORMAL.
 */
शून्य sched_set_fifo_नीचः(संरचना task_struct *p)
{
	संरचना sched_param sp = { .sched_priority = 1 };
	WARN_ON_ONCE(sched_setscheduler_nocheck(p, SCHED_FIFO, &sp) != 0);
}
EXPORT_चिह्नम्_GPL(sched_set_fifo_नीचः);

/*
 * Used when the primary interrupt handler is forced into a thread, in addition
 * to the (always threaded) secondary handler.  The secondary handler gets a
 * slightly lower priority so that the primary handler can preempt it, thereby
 * emulating the behavior of a non-PREEMPT_RT system where the primary handler
 * runs in hard interrupt context.
 */
शून्य sched_set_fifo_secondary(संरचना task_struct *p)
{
	संरचना sched_param sp = { .sched_priority = MAX_RT_PRIO / 2 - 1 };
	WARN_ON_ONCE(sched_setscheduler_nocheck(p, SCHED_FIFO, &sp) != 0);
}

शून्य sched_set_normal(संरचना task_struct *p, पूर्णाङ्क nice)
{
	संरचना sched_attr attr = {
		.sched_policy = SCHED_NORMAL,
		.sched_nice = nice,
	};
	WARN_ON_ONCE(sched_setattr_nocheck(p, &attr) != 0);
}
EXPORT_चिह्नम्_GPL(sched_set_normal);

स्थिर पूर्णाङ्क
do_sched_setscheduler(pid_t pid, पूर्णाङ्क policy, संरचना sched_param __user *param)
{
	संरचना sched_param lparam;

	जइ (unlikely(!param || pid < 0))
		प्रत्यागम -EINVAL;
	जइ (copy_from_user(&lparam, param, परिमाण(संरचना sched_param)))
		प्रत्यागम -EFAULT;

	CLASS(find_get_task, p)(pid);
	जइ (!p)
		प्रत्यागम -ESRCH;

	प्रत्यागम sched_setscheduler(p, policy, &lparam);
}

/*
 * Mimics kernel/events/core.c perf_copy_attr().
 */
स्थिर पूर्णाङ्क sched_copy_attr(संरचना sched_attr __user *uattr, संरचना sched_attr *attr)
{
	u32 size;
	पूर्णाङ्क ret;

	/* Zero the full structure, so that a short copy will be nice: */
	memset(attr, 0, परिमाण(*attr));

	ret = get_user(size, &uattr->size);
	जइ (ret)
		प्रत्यागम ret;

	/* ABI compatibility quirk: */
	जइ (!size)
		size = SCHED_ATTR_SIZE_VER0;
	जइ (size < SCHED_ATTR_SIZE_VER0 || size > PAGE_SIZE)
		गच्छ err_size;

	ret = copy_struct_from_user(attr, परिमाण(*attr), uattr, size);
	जइ (ret) {
		जइ (ret == -E2BIG)
			गच्छ err_size;
		प्रत्यागम ret;
	}

	जइ ((attr->sched_flags & SCHED_FLAG_UTIL_CLAMP) &&
	    size < SCHED_ATTR_SIZE_VER1)
		प्रत्यागम -EINVAL;

	/*
	 * XXX: Do we want to be lenient like existing syscalls; or do we want
	 * to be strict and return an error on out-of-bounds values?
	 */
	attr->sched_nice = clamp(attr->sched_nice, MIN_NICE, MAX_NICE);

	प्रत्यागम 0;

err_size:
	put_user(परिमाण(*attr), &uattr->size);
	प्रत्यागम -E2BIG;
}

स्थिर शून्य get_params(संरचना task_struct *p, संरचना sched_attr *attr, unsigned पूर्णाङ्क flags)
{
	जइ (task_has_dl_policy(p)) {
		__getparam_dl(p, attr, flags);
	} अण्णहा जइ (task_has_rt_policy(p)) {
		attr->sched_priority = p->rt_priority;
	} अण्णहा {
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
SYSCALL_DEFINE3(sched_setscheduler, pid_t, pid, पूर्णाङ्क, policy, संरचना sched_param __user *, param)
{
	जइ (policy < 0)
		प्रत्यागम -EINVAL;

	प्रत्यागम do_sched_setscheduler(pid, policy, param);
}

/**
 * sys_sched_setparam - set/change the RT priority of a thread
 * @pid: the pid in question.
 * @param: structure containing the new RT priority.
 *
 * Return: 0 on success. An error code otherwise.
 */
SYSCALL_DEFINE2(sched_setparam, pid_t, pid, संरचना sched_param __user *, param)
{
	प्रत्यागम do_sched_setscheduler(pid, SETPARAM_POLICY, param);
}

/**
 * sys_sched_setattr - same as above, but with extended sched_attr
 * @pid: the pid in question.
 * @uattr: structure containing the extended parameters.
 * @flags: for future extension.
 */
SYSCALL_DEFINE3(sched_setattr, pid_t, pid, संरचना sched_attr __user *, uattr,
			       unsigned पूर्णाङ्क, flags)
{
	संरचना sched_attr attr;
	पूर्णाङ्क retval;

	जइ (unlikely(!uattr || pid < 0 || flags))
		प्रत्यागम -EINVAL;

	retval = sched_copy_attr(uattr, &attr);
	जइ (retval)
		प्रत्यागम retval;

	जइ ((पूर्णाङ्क)attr.sched_policy < 0)
		प्रत्यागम -EINVAL;
	जइ (attr.sched_flags & SCHED_FLAG_KEEP_POLICY)
		attr.sched_policy = SETPARAM_POLICY;

	CLASS(find_get_task, p)(pid);
	जइ (!p)
		प्रत्यागम -ESRCH;

	जइ (attr.sched_flags & SCHED_FLAG_KEEP_PARAMS)
		get_params(p, &attr, 0);

	प्रत्यागम sched_setattr(p, &attr);
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
	संरचना task_struct *p;
	पूर्णाङ्क retval;

	जइ (pid < 0)
		प्रत्यागम -EINVAL;

	guard(rcu)();
	p = find_process_by_pid(pid);
	जइ (!p)
		प्रत्यागम -ESRCH;

	retval = security_task_getscheduler(p);
	जइ (!retval) {
		retval = p->policy;
		जइ (p->sched_reset_on_fork)
			retval |= SCHED_RESET_ON_FORK;
	}
	प्रत्यागम retval;
}

/**
 * sys_sched_getparam - get the RT priority of a thread
 * @pid: the pid in question.
 * @param: structure containing the RT priority.
 *
 * Return: On success, 0 and the RT priority is in @param. Otherwise, an error
 * code.
 */
SYSCALL_DEFINE2(sched_getparam, pid_t, pid, संरचना sched_param __user *, param)
{
	संरचना sched_param lp = { .sched_priority = 0 };
	संरचना task_struct *p;
	पूर्णाङ्क retval;

	जइ (unlikely(!param || pid < 0))
		प्रत्यागम -EINVAL;

	scoped_guard (rcu) {
		p = find_process_by_pid(pid);
		जइ (!p)
			प्रत्यागम -ESRCH;

		retval = security_task_getscheduler(p);
		जइ (retval)
			प्रत्यागम retval;

		जइ (task_has_rt_policy(p))
			lp.sched_priority = p->rt_priority;
	}

	/*
	 * This one might sleep, we cannot do it with a spinlock held ...
	 */
	प्रत्यागम copy_to_user(param, &lp, परिमाण(*param)) ? -EFAULT : 0;
}

/**
 * sys_sched_getattr - similar to sched_getparam, but with sched_attr
 * @pid: the pid in question.
 * @uattr: structure containing the extended parameters.
 * @usize: sizeof(attr) for fwd/bwd comp.
 * @flags: for future extension.
 */
SYSCALL_DEFINE4(sched_getattr, pid_t, pid, संरचना sched_attr __user *, uattr,
		unsigned पूर्णाङ्क, usize, unsigned पूर्णाङ्क, flags)
{
	संरचना sched_attr kattr = { };
	संरचना task_struct *p;
	पूर्णाङ्क retval;

	जइ (unlikely(!uattr || pid < 0 || usize > PAGE_SIZE ||
		     usize < SCHED_ATTR_SIZE_VER0))
		प्रत्यागम -EINVAL;

	scoped_guard (rcu) {
		p = find_process_by_pid(pid);
		जइ (!p)
			प्रत्यागम -ESRCH;

		जइ (flags) {
			जइ (!task_has_dl_policy(p) ||
			    flags != SCHED_GETATTR_FLAG_DL_DYNAMIC)
				प्रत्यागम -EINVAL;
		}

		retval = security_task_getscheduler(p);
		जइ (retval)
			प्रत्यागम retval;

		kattr.sched_policy = p->policy;
		जइ (p->sched_reset_on_fork)
			kattr.sched_flags |= SCHED_FLAG_RESET_ON_FORK;
		get_params(p, &kattr, flags);
		kattr.sched_flags &= SCHED_FLAG_ALL;

#ifdef CONFIG_UCLAMP_TASK
		/*
		 * This could race with another potential updater, but this is fine
		 * because it'll correctly read the old or the new value. We don't need
		 * to guarantee who wins the race as long as it doesn't return garbage.
		 */
		kattr.sched_util_min = p->uclamp_req[UCLAMP_MIN].मानम्;
		kattr.sched_util_max = p->uclamp_req[UCLAMP_MAX].मानम्;
#endif
	}

	kattr.size = min(usize, परिमाण(kattr));
	प्रत्यागम copy_struct_to_user(uattr, usize, &kattr, परिमाण(kattr), रिक्त);
}

पूर्णाङ्क dl_task_check_affinity(संरचना task_struct *p, ध्रुव संरचना cpumask *mask)
{
	/*
	 * If the task isn't a deadline task or admission control is
	 * disabled then we don't care about affinity changes.
	 */
	जइ (!task_has_dl_policy(p) || !dl_bandwidth_enabled())
		प्रत्यागम 0;

	/*
	 * The special/sugov task isn't part of regular bandwidth/admission
	 * control so let userspace change affinities.
	 */
	जइ (dl_entity_is_special(&p->dl))
		प्रत्यागम 0;

	/*
	 * Since bandwidth control happens on root_domain basis,
	 * if admission test is enabled, we only admit -deadline
	 * tasks allowed to run on all the CPUs in the task's
	 * root_domain.
	 */
	guard(rcu)();
	जइ (!cpumask_subset(task_rq(p)->rd->span, mask))
		प्रत्यागम -EBUSY;

	प्रत्यागम 0;
}

पूर्णाङ्क __sched_setaffinity(संरचना task_struct *p, संरचना affinity_context *ctx)
{
	पूर्णाङ्क retval;
	cpumask_var_t cpus_allowed, new_mask;

	जइ (!alloc_cpumask_var(&cpus_allowed, GFP_KERNEL))
		प्रत्यागम -ENOMEM;

	जइ (!alloc_cpumask_var(&new_mask, GFP_KERNEL)) {
		retval = -ENOMEM;
		गच्छ out_free_cpus_allowed;
	}

	cpuset_cpus_allowed(p, cpus_allowed);
	cpumask_and(new_mask, ctx->new_mask, cpus_allowed);

	ctx->new_mask = new_mask;
	ctx->flags |= SCA_CHECK;

	retval = dl_task_check_affinity(p, new_mask);
	जइ (retval)
		गच्छ out_free_new_mask;

	retval = __set_cpus_allowed_ptr(p, ctx);
	जइ (retval)
		गच्छ out_free_new_mask;

	cpuset_cpus_allowed(p, cpus_allowed);
	जइ (!cpumask_subset(new_mask, cpus_allowed)) {
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
		जइ (unlikely((ctx->flags & SCA_USER) && ctx->user_mask)) {
			bool empty = !cpumask_and(new_mask, new_mask,
						  ctx->user_mask);

			जइ (empty)
				cpumask_copy(new_mask, cpus_allowed);
		}
		__set_cpus_allowed_ptr(p, ctx);
		retval = -EINVAL;
	}

out_free_new_mask:
	free_cpumask_var(new_mask);
out_free_cpus_allowed:
	free_cpumask_var(cpus_allowed);
	प्रत्यागम retval;
}

long sched_setaffinity(pid_t pid, ध्रुव संरचना cpumask *in_mask)
{
	संरचना affinity_context ac;
	संरचना cpumask *user_mask;
	पूर्णाङ्क retval;

	CLASS(find_get_task, p)(pid);
	जइ (!p)
		प्रत्यागम -ESRCH;

	जइ (p->flags & PF_NO_SETAFFINITY)
		प्रत्यागम -EINVAL;

	जइ (!check_same_owner(p)) {
		guard(rcu)();
		जइ (!ns_capable(__task_cred(p)->user_ns, CAP_SYS_NICE))
			प्रत्यागम -EPERM;
	}

	retval = security_task_setscheduler(p);
	जइ (retval)
		प्रत्यागम retval;

	/*
	 * With non-SMP configs, user_cpus_ptr/user_mask isn't used and
	 * alloc_user_cpus_ptr() returns NULL.
	 */
	user_mask = alloc_user_cpus_ptr(NUMA_NO_ग्रन्थिः);
	जइ (user_mask) {
		cpumask_copy(user_mask, in_mask);
	} अण्णहा {
		प्रत्यागम -ENOMEM;
	}

	ac = (संरचना affinity_context){
		.new_mask  = in_mask,
		.user_mask = user_mask,
		.flags     = SCA_USER,
	};

	retval = __sched_setaffinity(p, &ac);
	kfree(ac.user_mask);

	प्रत्यागम retval;
}

स्थिर पूर्णाङ्क get_user_cpu_mask(unsigned long __user *user_mask_ptr, unsigned len,
			     संरचना cpumask *new_mask)
{
	जइ (len < cpumask_size())
		cpumask_clear(new_mask);
	अण्णहा जइ (len > cpumask_size())
		len = cpumask_size();

	प्रत्यागम copy_from_user(new_mask, user_mask_ptr, len) ? -EFAULT : 0;
}

/**
 * sys_sched_setaffinity - set the CPU affinity of a process
 * @pid: pid of the process
 * @len: length in bytes of the bitmask pointed to by user_mask_ptr
 * @user_mask_ptr: user-space pointer to the new CPU mask
 *
 * Return: 0 on success. An error code otherwise.
 */
SYSCALL_DEFINE3(sched_setaffinity, pid_t, pid, unsigned पूर्णाङ्क, len,
		unsigned long __user *, user_mask_ptr)
{
	cpumask_var_t new_mask;
	पूर्णाङ्क retval;

	जइ (!alloc_cpumask_var(&new_mask, GFP_KERNEL))
		प्रत्यागम -ENOMEM;

	retval = get_user_cpu_mask(user_mask_ptr, len, new_mask);
	जइ (retval == 0)
		retval = sched_setaffinity(pid, new_mask);
	free_cpumask_var(new_mask);
	प्रत्यागम retval;
}

long sched_getaffinity(pid_t pid, संरचना cpumask *mask)
{
	संरचना task_struct *p;
	पूर्णाङ्क retval;

	guard(rcu)();
	p = find_process_by_pid(pid);
	जइ (!p)
		प्रत्यागम -ESRCH;

	retval = security_task_getscheduler(p);
	जइ (retval)
		प्रत्यागम retval;

	guard(raw_spinlock_irqsave)(&p->pi_lock);
	cpumask_and(mask, &p->cpus_mask, cpu_active_mask);

	प्रत्यागम 0;
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
SYSCALL_DEFINE3(sched_getaffinity, pid_t, pid, unsigned पूर्णाङ्क, len,
		unsigned long __user *, user_mask_ptr)
{
	पूर्णाङ्क ret;
	cpumask_var_t mask;

	जइ ((len * BITS_PER_BYTE) < nr_cpu_ids)
		प्रत्यागम -EINVAL;
	जइ (len & (परिमाण(unsigned long)-1))
		प्रत्यागम -EINVAL;

	जइ (!zalloc_cpumask_var(&mask, GFP_KERNEL))
		प्रत्यागम -ENOMEM;

	ret = sched_getaffinity(pid, mask);
	जइ (ret == 0) {
		unsigned पूर्णाङ्क retlen = min(len, cpumask_size());

		जइ (copy_to_user(user_mask_ptr, cpumask_bits(mask), retlen))
			ret = -EFAULT;
		अण्णहा
			ret = retlen;
	}
	free_cpumask_var(mask);

	प्रत्यागम ret;
}

स्थिर शून्य do_sched_yield(शून्य)
{
	संरचना rq_flags rf;
	संरचना rq *rq;

	rq = this_rq_lock_irq(&rf);

	schedstat_inc(rq->yld_गणनम्);
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
	प्रत्यागम 0;
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
शून्य __sched yield(शून्य)
{
	set_current_state(TASK_RUNNING);
	do_sched_yield();
}
EXPORT_चिह्नम्(yield);

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
पूर्णाङ्क __sched yield_to(संरचना task_struct *p, bool preempt)
{
	संरचना task_struct *curr;
	संरचना rq *rq, *p_rq;
	पूर्णाङ्क yielded = 0;

	scoped_guard (raw_spinlock_irqsave, &p->pi_lock) {
		rq = this_rq();
		curr = rq->donor;

again:
		p_rq = task_rq(p);
		/*
		 * If we're the only runnable task on the rq and target rq also
		 * has only one task, there's absolutely no point in yielding.
		 */
		जइ (rq->nr_running == 1 && p_rq->nr_running == 1)
			प्रत्यागम -ESRCH;

		guard(double_rq_lock)(rq, p_rq);
		जइ (task_rq(p) != p_rq)
			गच्छ again;

		जइ (!curr->sched_class->yield_to_task)
			प्रत्यागम 0;

		जइ (curr->sched_class != p->sched_class)
			प्रत्यागम 0;

		जइ (task_on_cpu(p_rq, p) || !task_is_running(p))
			प्रत्यागम 0;

		yielded = curr->sched_class->yield_to_task(rq, p);
		जइ (yielded) {
			schedstat_inc(rq->yld_गणनम्);
			/*
			 * Make p's CPU reschedule; pick_next_entity
			 * takes care of fairness.
			 */
			जइ (preempt && rq != p_rq)
				resched_curr(p_rq);
		}
	}

	जइ (yielded)
		schedule();

	प्रत्यागम yielded;
}
EXPORT_चिह्नम्_GPL(yield_to);

/**
 * sys_sched_get_priority_max - return maximum RT priority.
 * @policy: scheduling class.
 *
 * Return: On success, this syscall returns the maximum
 * rt_priority that can be used by a given scheduling class.
 * On failure, a negative error code is returned.
 */
SYSCALL_DEFINE1(sched_get_priority_max, पूर्णाङ्क, policy)
{
	पूर्णाङ्क ret = -EINVAL;

	विकल्प (policy) {
	पक्ष SCHED_FIFO:
	पक्ष SCHED_RR:
		ret = MAX_RT_PRIO-1;
		भङ्ग;
	पक्ष SCHED_DEADLINE:
	पक्ष SCHED_NORMAL:
	पक्ष SCHED_BATCH:
	पक्ष SCHED_IDLE:
	पक्ष SCHED_EXT:
		ret = 0;
		भङ्ग;
	}
	प्रत्यागम ret;
}

/**
 * sys_sched_get_priority_min - return minimum RT priority.
 * @policy: scheduling class.
 *
 * Return: On success, this syscall returns the minimum
 * rt_priority that can be used by a given scheduling class.
 * On failure, a negative error code is returned.
 */
SYSCALL_DEFINE1(sched_get_priority_min, पूर्णाङ्क, policy)
{
	पूर्णाङ्क ret = -EINVAL;

	विकल्प (policy) {
	पक्ष SCHED_FIFO:
	पक्ष SCHED_RR:
		ret = 1;
		भङ्ग;
	पक्ष SCHED_DEADLINE:
	पक्ष SCHED_NORMAL:
	पक्ष SCHED_BATCH:
	पक्ष SCHED_IDLE:
	पक्ष SCHED_EXT:
		ret = 0;
	}
	प्रत्यागम ret;
}

स्थिर पूर्णाङ्क sched_rr_get_interval(pid_t pid, संरचना timespec64 *t)
{
	unsigned पूर्णाङ्क time_slice = 0;
	पूर्णाङ्क retval;

	जइ (pid < 0)
		प्रत्यागम -EINVAL;

	scoped_guard (rcu) {
		संरचना task_struct *p = find_process_by_pid(pid);
		जइ (!p)
			प्रत्यागम -ESRCH;

		retval = security_task_getscheduler(p);
		जइ (retval)
			प्रत्यागम retval;

		scoped_guard (task_rq_lock, p) {
			संरचना rq *rq = scope.rq;
			जइ (p->sched_class->get_rr_interval)
				time_slice = p->sched_class->get_rr_interval(rq, p);
		}
	}

	jiffies_to_timespec64(time_slice, t);
	प्रत्यागम 0;
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
		संरचना __kernel_timespec __user *, interval)
{
	संरचना timespec64 t;
	पूर्णाङ्क retval = sched_rr_get_interval(pid, &t);

	जइ (retval == 0)
		retval = put_timespec64(&t, interval);

	प्रत्यागम retval;
}

#ifdef CONFIG_COMPAT_32BIT_TIME
SYSCALL_DEFINE2(sched_rr_get_interval_time32, pid_t, pid,
		संरचना old_timespec32 __user *, interval)
{
	संरचना timespec64 t;
	पूर्णाङ्क retval = sched_rr_get_interval(pid, &t);

	जइ (retval == 0)
		retval = put_old_timespec32(&t, interval);
	प्रत्यागम retval;
}
#endif
