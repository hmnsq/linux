// SPDX-License-Identifier: GPL-2.0-only
/*
 *  kernel/sched/syscalls.c
 *
 *  Core kernel scheduler syscalls related code
 *
 *  Copyright (C) 1991-2002  Linus Torvalds
 *  Copyright (C) 1998-2024  Ingo Molnar, Red Hat
 */
#অন্তর্ভাব <linux/sched.h>
#অন্তর্ভাব <linux/cpuset.h>
#অন্তর্ভাব <linux/sched/debug.h>

#অন্তর্ভাব <uapi/linux/sched/types.h>

#অন্তর্ভাব "sched.h"
#অন্তর্ভাব "autogroup.h"

স্থির অন্তঃরেখ পূর্ণসংখ্যা __normal_prio(পূর্ণসংখ্যা policy, পূর্ণসংখ্যা rt_prio, পূর্ণসংখ্যা nice)
{
	পূর্ণসংখ্যা prio;

	যদি (dl_policy(policy))
		prio = MAX_DL_PRIO - 1;
	অন্যথা যদি (rt_policy(policy))
		prio = MAX_RT_PRIO - 1 - rt_prio;
	অন্যথা
		prio = NICE_TO_PRIO(nice);

	ফেরত prio;
}

/*
 * Calculate the expected normal priority: i.e. priority
 * without taking RT-inheritance into account. Might be
 * boosted by interactivity modifiers. Changes upon fork,
 * setprio syscalls, and whenever the interactivity
 * estimator recalculates.
 */
স্থির অন্তঃরেখ পূর্ণসংখ্যা normal_prio(কাঠামো task_struct *p)
{
	ফেরত __normal_prio(p->policy, p->rt_priority, PRIO_TO_NICE(p->static_prio));
}

/*
 * Calculate the current priority, i.e. the priority
 * taken into account by the scheduler. This value might
 * be boosted by RT tasks, or might be boosted by
 * interactivity modifiers. Will be RT if the task got
 * RT-boosted. If not then it returns p->normal_prio.
 */
স্থির পূর্ণসংখ্যা effective_prio(কাঠামো task_struct *p)
{
	p->normal_prio = normal_prio(p);
	/*
	 * If we are RT tasks or we were boosted to RT priority,
	 * keep the priority unchanged. Otherwise, update priority
	 * to the normal priority:
	 */
	যদি (!rt_or_dl_prio(p->prio))
		ফেরত p->normal_prio;
	ফেরত p->prio;
}

শূন্য set_user_nice(কাঠামো task_struct *p, long nice)
{
	পূর্ণসংখ্যা old_prio;

	যদি (task_nice(p) == nice || nice < MIN_NICE || nice > MAX_NICE)
		ফেরত;
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
	যদি (task_has_dl_policy(p) || task_has_rt_policy(p)) {
		p->static_prio = NICE_TO_PRIO(nice);
		ফেরত;
	}

	scoped_guard (sched_change, p, DEQUEUE_SAVE) {
		p->static_prio = NICE_TO_PRIO(nice);
		set_load_weight(p, true);
		old_prio = p->prio;
		p->prio = effective_prio(p);
	}
}
EXPORT_প্রতীক(set_user_nice);

/*
 * is_nice_reduction - check if nice value is an actual reduction
 *
 * Similar to can_nice() but does not perform a capability check.
 *
 * @p: task
 * @nice: nice value
 */
স্থির bool is_nice_reduction(ধ্রুবক কাঠামো task_struct *p, ধ্রুবক পূর্ণসংখ্যা nice)
{
	/* Convert nice value [19,-20] to rlimit style value [1,40]: */
	পূর্ণসংখ্যা nice_rlim = nice_to_rlimit(nice);

	ফেরত (nice_rlim <= task_rlimit(p, RLIMIT_NICE));
}

/*
 * can_nice - check if a task can reduce its nice value
 * @p: task
 * @nice: nice value
 */
পূর্ণসংখ্যা can_nice(ধ্রুবক কাঠামো task_struct *p, ধ্রুবক পূর্ণসংখ্যা nice)
{
	ফেরত is_nice_reduction(p, nice) || capable(CAP_SYS_NICE);
}

#ifdef __ARCH_WANT_SYS_NICE

/*
 * sys_nice - change the priority of the current process.
 * @increment: priority increment
 *
 * sys_setpriority is a more generic, but much slower function that
 * does similar things.
 */
SYSCALL_DEFINE1(nice, পূর্ণসংখ্যা, increment)
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
	যদি (increment < 0 && !can_nice(current, nice))
		ফেরত -EPERM;

	retval = security_task_setnice(current, nice);
	যদি (retval)
		ফেরত retval;

	set_user_nice(current, nice);
	ফেরত 0;
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
পূর্ণসংখ্যা task_prio(ধ্রুবক কাঠামো task_struct *p)
{
	ফেরত p->prio - MAX_RT_PRIO;
}

/**
 * idle_cpu - is a given CPU idle currently?
 * @cpu: the processor in question.
 *
 * Return: 1 if the CPU is currently idle. 0 otherwise.
 */
পূর্ণসংখ্যা idle_cpu(পূর্ণসংখ্যা cpu)
{
	ফেরত idle_rq(cpu_rq(cpu));
}

/**
 * idle_task - return the idle task for a given CPU.
 * @cpu: the processor in question.
 *
 * Return: The idle task for the CPU @cpu.
 */
কাঠামো task_struct *idle_task(পূর্ণসংখ্যা cpu)
{
	ফেরত cpu_rq(cpu)->idle;
}

#ifdef CONFIG_SCHED_CORE
পূর্ণসংখ্যা sched_core_idle_cpu(পূর্ণসংখ্যা cpu)
{
	কাঠামো rq *rq = cpu_rq(cpu);

	যদি (sched_core_enabled(rq) && rq->curr == rq->idle)
		ফেরত 1;

	ফেরত idle_cpu(cpu);
}
#endif /* CONFIG_SCHED_CORE */

/**
 * find_process_by_pid - find a process with a matching PID value.
 * @pid: the pid in question.
 *
 * The task of @pid, if found. %NULL otherwise.
 */
স্থির কাঠামো task_struct *find_process_by_pid(pid_t pid)
{
	ফেরত pid ? find_task_by_vpid(pid) : current;
}

স্থির কাঠামো task_struct *find_get_task(pid_t pid)
{
	কাঠামো task_struct *p;
	guard(rcu)();

	p = find_process_by_pid(pid);
	যদি (likely(p))
		get_task_struct(p);

	ফেরত p;
}

DEFINE_CLASS(find_get_task, কাঠামো task_struct *, যদি (_T) put_task_struct(_T),
	     find_get_task(pid), pid_t pid)

/*
 * sched_setparam() passes in -1 for its policy, to let the functions
 * it calls know not to change it.
 */
#define SETPARAM_POLICY	-1

স্থির শূন্য __setscheduler_params(কাঠামো task_struct *p,
		ধ্রুবক কাঠামো sched_attr *attr)
{
	পূর্ণসংখ্যা policy = attr->sched_policy;

	যদি (policy == SETPARAM_POLICY)
		policy = p->policy;

	p->policy = policy;

	যদি (dl_policy(policy))
		__setparam_dl(p, attr);
	অন্যথা যদি (fair_policy(policy))
		__setparam_fair(p, attr);

	/* rt-policy tasks do not have a timerslack */
	যদি (rt_or_dl_task_policy(p)) {
		p->timer_slack_ns = 0;
	} অন্যথা যদি (p->timer_slack_ns == 0) {
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
স্থির bool check_same_owner(কাঠামো task_struct *p)
{
	ধ্রুবক কাঠামো cred *cred = current_cred(), *pcred;
	guard(rcu)();

	pcred = __task_cred(p);
	ফেরত (uid_eq(cred->euid, pcred->euid) ||
		uid_eq(cred->euid, pcred->uid));
}

#ifdef CONFIG_RT_MUTEXES
স্থির অন্তঃরেখ শূন্য __setscheduler_dl_pi(পূর্ণসংখ্যা newprio, পূর্ণসংখ্যা policy,
			      কাঠামো task_struct *p,
			      কাঠামো sched_change_ctx *scope)
{
	/*
	 * In case a DEADLINE task (either proper or boosted) gets
	 * setscheduled to a lower priority class, check if it neeeds to
	 * inherit parameters from a potential pi_task. In that case make
	 * sure replenishment happens with the next enqueue.
	 */

	যদি (dl_prio(newprio) && !dl_policy(policy)) {
		কাঠামো task_struct *pi_task = rt_mutex_get_শীর্ষ_task(p);

		যদি (pi_task) {
			p->dl.pi_se = pi_task->dl.pi_se;
			scope->flags |= ENQUEUE_REPLENISH;
		}
	}
}
#অন্যথা /* !CONFIG_RT_MUTEXES */
স্থির অন্তঃরেখ শূন্য __setscheduler_dl_pi(পূর্ণসংখ্যা newprio, পূর্ণসংখ্যা policy,
			      কাঠামো task_struct *p,
			      কাঠামো sched_change_ctx *scope)
{
}
#endif /* !CONFIG_RT_MUTEXES */

#ifdef CONFIG_UCLAMP_TASK

স্থির পূর্ণসংখ্যা uclamp_validate(কাঠামো task_struct *p,
			   ধ্রুবক কাঠামো sched_attr *attr)
{
	পূর্ণসংখ্যা util_min = p->uclamp_req[UCLAMP_MIN].মান;
	পূর্ণসংখ্যা util_max = p->uclamp_req[UCLAMP_MAX].মান;

	যদি (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MIN) {
		util_min = attr->sched_util_min;

		যদি (util_min + 1 > SCHED_CAPACITY_SCALE + 1)
			ফেরত -EINVAL;
	}

	যদি (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MAX) {
		util_max = attr->sched_util_max;

		যদি (util_max + 1 > SCHED_CAPACITY_SCALE + 1)
			ফেরত -EINVAL;
	}

	যদি (util_min != -1 && util_max != -1 && util_min > util_max)
		ফেরত -EINVAL;

	/*
	 * We have valid uclamp attributes; make sure uclamp is enabled.
	 *
	 * We need to do that here, because enabling static branches is a
	 * blocking operation which obviously cannot be done while holding
	 * scheduler locks.
	 */
	sched_uclamp_enable();

	ফেরত 0;
}

স্থির bool uclamp_reset(ধ্রুবক কাঠামো sched_attr *attr,
			 গণনা uclamp_id clamp_id,
			 কাঠামো uclamp_se *uc_se)
{
	/* Reset on sched class change for a non user-defined clamp value. */
	যদি (likely(!(attr->sched_flags & SCHED_FLAG_UTIL_CLAMP)) &&
	    !uc_se->user_defined)
		ফেরত true;

	/* Reset on sched_util_{min,max} == -1. */
	যদি (clamp_id == UCLAMP_MIN &&
	    attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MIN &&
	    attr->sched_util_min == -1) {
		ফেরত true;
	}

	যদি (clamp_id == UCLAMP_MAX &&
	    attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MAX &&
	    attr->sched_util_max == -1) {
		ফেরত true;
	}

	ফেরত false;
}

স্থির শূন্য __setscheduler_uclamp(কাঠামো task_struct *p,
				  ধ্রুবক কাঠামো sched_attr *attr)
{
	গণনা uclamp_id clamp_id;

	for_each_clamp_id(clamp_id) {
		কাঠামো uclamp_se *uc_se = &p->uclamp_req[clamp_id];
		unsigned পূর্ণসংখ্যা মান;

		যদি (!uclamp_reset(attr, clamp_id, uc_se))
			চালু;

		/*
		 * RT by default have a 100% boost value that could be modified
		 * at runtime.
		 */
		যদি (unlikely(rt_task(p) && clamp_id == UCLAMP_MIN))
			মান = sysctl_sched_uclamp_util_min_rt_default;
		অন্যথা
			মান = uclamp_none(clamp_id);

		uclamp_se_set(uc_se, মান, false);

	}

	যদি (likely(!(attr->sched_flags & SCHED_FLAG_UTIL_CLAMP)))
		ফেরত;

	যদি (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MIN &&
	    attr->sched_util_min != -1) {
		uclamp_se_set(&p->uclamp_req[UCLAMP_MIN],
			      attr->sched_util_min, true);
	}

	যদি (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MAX &&
	    attr->sched_util_max != -1) {
		uclamp_se_set(&p->uclamp_req[UCLAMP_MAX],
			      attr->sched_util_max, true);
	}
}

#অন্যথা /* !CONFIG_UCLAMP_TASK: */

স্থির অন্তঃরেখ পূর্ণসংখ্যা uclamp_validate(কাঠামো task_struct *p,
				  ধ্রুবক কাঠামো sched_attr *attr)
{
	ফেরত -EOPNOTSUPP;
}
স্থির শূন্য __setscheduler_uclamp(কাঠামো task_struct *p,
				  ধ্রুবক কাঠামো sched_attr *attr) { }
#endif /* !CONFIG_UCLAMP_TASK */

/*
 * Allow unprivileged RT tasks to decrease priority.
 * Only issue a capable test if needed and only once to avoid an audit
 * event on permitted non-privileged operations:
 */
স্থির পূর্ণসংখ্যা user_check_sched_setscheduler(কাঠামো task_struct *p,
					 ধ্রুবক কাঠামো sched_attr *attr,
					 পূর্ণসংখ্যা policy, পূর্ণসংখ্যা reset_on_fork)
{
	যদি (fair_policy(policy)) {
		যদি (attr->sched_nice < task_nice(p) &&
		    !is_nice_reduction(p, attr->sched_nice))
			যাও req_priv;
	}

	যদি (rt_policy(policy)) {
		unsigned long rlim_rtprio = task_rlimit(p, RLIMIT_RTPRIO);

		/* Can't set/change the rt policy: */
		যদি (policy != p->policy && !rlim_rtprio)
			যাও req_priv;

		/* Can't increase priority: */
		যদি (attr->sched_priority > p->rt_priority &&
		    attr->sched_priority > rlim_rtprio)
			যাও req_priv;
	}

	/*
	 * Can't set/change SCHED_DEADLINE policy at all for now
	 * (safest behavior); in the future we would like to allow
	 * unprivileged DL tasks to increase their relative deadline
	 * or reduce their runtime (both ways reducing utilization)
	 */
	যদি (dl_policy(policy))
		যাও req_priv;

	/*
	 * Treat SCHED_IDLE as nice 20. Only allow a switch to
	 * SCHED_NORMAL if the RLIMIT_NICE would normally permit it.
	 */
	যদি (task_has_idle_policy(p) && !idle_policy(policy)) {
		যদি (!is_nice_reduction(p, task_nice(p)))
			যাও req_priv;
	}

	/* Can't change other user's priorities: */
	যদি (!check_same_owner(p))
		যাও req_priv;

	/* Normal users shall not reset the sched_reset_on_fork flag: */
	যদি (p->sched_reset_on_fork && !reset_on_fork)
		যাও req_priv;

	ফেরত 0;

req_priv:
	যদি (!capable(CAP_SYS_NICE))
		ফেরত -EPERM;

	ফেরত 0;
}

পূর্ণসংখ্যা __sched_setscheduler(কাঠামো task_struct *p,
			 ধ্রুবক কাঠামো sched_attr *attr,
			 bool user, bool pi)
{
	পূর্ণসংখ্যা oldpolicy = -1, policy = attr->sched_policy;
	পূর্ণসংখ্যা retval, oldprio, newprio;
	ধ্রুবক কাঠামো sched_class *পূর্ব_class, *পরবর্তী_class;
	কাঠামো balance_callback *শীর্ষ;
	কাঠামো rq_flags rf;
	পূর্ণসংখ্যা reset_on_fork;
	পূর্ণসংখ্যা queue_flags = DEQUEUE_SAVE | DEQUEUE_MOVE | DEQUEUE_NOCLOCK;
	কাঠামো rq *rq;
	bool cpuset_locked = false;

	/* The pi code expects interrupts enabled */
	BUG_ON(pi && in_interrupt());
recheck:
	/* Double check policy once rq lock held: */
	যদি (policy < 0) {
		reset_on_fork = p->sched_reset_on_fork;
		policy = oldpolicy = p->policy;
	} অন্যথা {
		reset_on_fork = !!(attr->sched_flags & SCHED_FLAG_RESET_ON_FORK);

		যদি (!valid_policy(policy))
			ফেরত -EINVAL;
	}

	যদি (attr->sched_flags & ~(SCHED_FLAG_ALL | SCHED_FLAG_SUGOV))
		ফেরত -EINVAL;

	/*
	 * Valid priorities for SCHED_FIFO and SCHED_RR are
	 * 1..MAX_RT_PRIO-1, valid priority for SCHED_NORMAL,
	 * SCHED_BATCH and SCHED_IDLE is 0.
	 */
	যদি (attr->sched_priority > MAX_RT_PRIO-1)
		ফেরত -EINVAL;
	যদি ((dl_policy(policy) && !__checkparam_dl(attr)) ||
	    (rt_policy(policy) != (attr->sched_priority != 0)))
		ফেরত -EINVAL;

	যদি (user) {
		retval = user_check_sched_setscheduler(p, attr, policy, reset_on_fork);
		যদি (retval)
			ফেরত retval;

		যদি (attr->sched_flags & SCHED_FLAG_SUGOV)
			ফেরত -EINVAL;

		retval = security_task_setscheduler(p);
		যদি (retval)
			ফেরত retval;
	}

	/* Update task specific "requested" clamps */
	যদি (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP) {
		retval = uclamp_validate(p, attr);
		যদি (retval)
			ফেরত retval;
	}

	/*
	 * SCHED_DEADLINE bandwidth accounting relies on stable cpusets
	 * information.
	 */
	যদি (dl_policy(policy) || dl_policy(p->policy)) {
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
	যদি (p == rq->stop) {
		retval = -EINVAL;
		যাও unlock;
	}

	retval = scx_check_setscheduler(p, policy);
	যদি (retval)
		যাও unlock;

	/*
	 * If not changing anything there's no need to proceed further,
	 * but store a possible modification of reset_on_fork.
	 */
	যদি (unlikely(policy == p->policy)) {
		যদি (fair_policy(policy) &&
		    (attr->sched_nice != task_nice(p) ||
		     (attr->sched_runtime != p->se.slice)))
			যাও change;
		যদি (rt_policy(policy) && attr->sched_priority != p->rt_priority)
			যাও change;
		যদি (dl_policy(policy) && dl_param_changed(p, attr))
			যাও change;
		যদি (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP)
			যাও change;

		p->sched_reset_on_fork = reset_on_fork;
		retval = 0;
		যাও unlock;
	}
change:

	যদি (user) {
#ifdef CONFIG_RT_GROUP_SCHED
		/*
		 * Do not allow real-time tasks into groups that have no runtime
		 * assigned.
		 */
		যদি (rt_group_sched_enabled() &&
				rt_bandwidth_enabled() && rt_policy(policy) &&
				task_group(p)->rt_bandwidth.rt_runtime == 0 &&
				!task_group_is_autogroup(task_group(p))) {
			retval = -EPERM;
			যাও unlock;
		}
#endif /* CONFIG_RT_GROUP_SCHED */
		যদি (dl_bandwidth_enabled() && dl_policy(policy) &&
				!(attr->sched_flags & SCHED_FLAG_SUGOV)) {
			cpumask_t *span = rq->rd->span;

			/*
			 * Don't allow tasks with an affinity mask smaller than
			 * the entire root_domain to become SCHED_DEADLINE. We
			 * will also fail if there's no bandwidth available.
			 */
			যদি (!cpumask_subset(span, p->cpus_ptr) ||
			    rq->rd->dl_bw.bw == 0) {
				retval = -EPERM;
				যাও unlock;
			}
		}
	}

	/* Re-check policy now with rq lock held: */
	যদি (unlikely(oldpolicy != -1 && oldpolicy != p->policy)) {
		policy = oldpolicy = -1;
		task_rq_unlock(rq, p, &rf);
		যদি (cpuset_locked)
			cpuset_unlock();
		যাও recheck;
	}

	/*
	 * If setscheduling to SCHED_DEADLINE (or changing the parameters
	 * of a SCHED_DEADLINE task) we need to check if enough bandwidth
	 * is available.
	 */
	যদি ((dl_policy(policy) || dl_task(p)) && sched_dl_overflow(p, policy, attr)) {
		retval = -EBUSY;
		যাও unlock;
	}

	p->sched_reset_on_fork = reset_on_fork;
	oldprio = p->prio;

	newprio = __normal_prio(policy, attr->sched_priority, attr->sched_nice);
	যদি (pi) {
		/*
		 * Take priority boosted tasks into account. If the new
		 * effective priority is unchanged, we just store the new
		 * normal parameters and do not touch the scheduler class and
		 * the runqueue. This will be done when the task deboost
		 * itself.
		 */
		newprio = rt_effective_prio(p, newprio);
		যদি (newprio == oldprio && !dl_prio(newprio))
			queue_flags &= ~DEQUEUE_MOVE;
	}

	পূর্ব_class = p->sched_class;
	পরবর্তী_class = __setscheduler_class(policy, newprio);

	যদি (পূর্ব_class != পরবর্তী_class)
		queue_flags |= DEQUEUE_CLASS;

	scoped_guard (sched_change, p, queue_flags) {

		যদি (!(attr->sched_flags & SCHED_FLAG_KEEP_PARAMS)) {
			__setscheduler_params(p, attr);
			p->sched_class = পরবর্তী_class;
			p->prio = newprio;
			__setscheduler_dl_pi(newprio, policy, p, scope);
		}
		__setscheduler_uclamp(p, attr);

		যদি (scope->queued) {
			/*
			 * We enqueue to tail when the priority of a task is
			 * increased (user space view).
			 */
			যদি (oldprio < p->prio)
				scope->flags |= ENQUEUE_শীর্ষ;
		}
	}

	/* Avoid rq from going away on us: */
	preempt_disable();
	শীর্ষ = splice_balance_callbacks(rq);
	task_rq_unlock(rq, p, &rf);

	যদি (pi) {
		যদি (cpuset_locked)
			cpuset_unlock();
		rt_mutex_adjust_pi(p);
	}

	/* Run balance callbacks after we've adjusted the PI chain: */
	balance_callbacks(rq, শীর্ষ);
	preempt_enable();

	ফেরত 0;

unlock:
	task_rq_unlock(rq, p, &rf);
	যদি (cpuset_locked)
		cpuset_unlock();
	ফেরত retval;
}

স্থির পূর্ণসংখ্যা _sched_setscheduler(কাঠামো task_struct *p, পূর্ণসংখ্যা policy,
			       ধ্রুবক কাঠামো sched_param *param, bool check)
{
	কাঠামো sched_attr attr = {
		.sched_policy   = policy,
		.sched_priority = param->sched_priority,
		.sched_nice	= PRIO_TO_NICE(p->static_prio),
	};

	যদি (p->se.custom_slice)
		attr.sched_runtime = p->se.slice;

	/* Fixup the legacy SCHED_RESET_ON_FORK hack. */
	যদি ((policy != SETPARAM_POLICY) && (policy & SCHED_RESET_ON_FORK)) {
		attr.sched_flags |= SCHED_FLAG_RESET_ON_FORK;
		policy &= ~SCHED_RESET_ON_FORK;
		attr.sched_policy = policy;
	}

	ফেরত __sched_setscheduler(p, &attr, check, true);
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
পূর্ণসংখ্যা sched_setscheduler(কাঠামো task_struct *p, পূর্ণসংখ্যা policy,
		       ধ্রুবক কাঠামো sched_param *param)
{
	ফেরত _sched_setscheduler(p, policy, param, true);
}

পূর্ণসংখ্যা sched_setattr(কাঠামো task_struct *p, ধ্রুবক কাঠামো sched_attr *attr)
{
	ফেরত __sched_setscheduler(p, attr, true, true);
}

পূর্ণসংখ্যা sched_setattr_nocheck(কাঠামো task_struct *p, ধ্রুবক কাঠামো sched_attr *attr)
{
	ফেরত __sched_setscheduler(p, attr, false, true);
}
EXPORT_প্রতীক_GPL(sched_setattr_nocheck);

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
পূর্ণসংখ্যা sched_setscheduler_nocheck(কাঠামো task_struct *p, পূর্ণসংখ্যা policy,
			       ধ্রুবক কাঠামো sched_param *param)
{
	ফেরত _sched_setscheduler(p, policy, param, false);
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
শূন্য sched_set_fifo(কাঠামো task_struct *p)
{
	কাঠামো sched_param sp = { .sched_priority = MAX_RT_PRIO / 2 };
	WARN_ON_ONCE(sched_setscheduler_nocheck(p, SCHED_FIFO, &sp) != 0);
}
EXPORT_প্রতীক_GPL(sched_set_fifo);

/*
 * For when you don't much care about FIFO, but want to be above SCHED_NORMAL.
 */
শূন্য sched_set_fifo_নিচের(কাঠামো task_struct *p)
{
	কাঠামো sched_param sp = { .sched_priority = 1 };
	WARN_ON_ONCE(sched_setscheduler_nocheck(p, SCHED_FIFO, &sp) != 0);
}
EXPORT_প্রতীক_GPL(sched_set_fifo_নিচের);

/*
 * Used when the primary interrupt handler is forced into a thread, in addition
 * to the (always threaded) secondary handler.  The secondary handler gets a
 * slightly lower priority so that the primary handler can preempt it, thereby
 * emulating the behavior of a non-PREEMPT_RT system where the primary handler
 * runs in hard interrupt context.
 */
শূন্য sched_set_fifo_secondary(কাঠামো task_struct *p)
{
	কাঠামো sched_param sp = { .sched_priority = MAX_RT_PRIO / 2 - 1 };
	WARN_ON_ONCE(sched_setscheduler_nocheck(p, SCHED_FIFO, &sp) != 0);
}

শূন্য sched_set_normal(কাঠামো task_struct *p, পূর্ণসংখ্যা nice)
{
	কাঠামো sched_attr attr = {
		.sched_policy = SCHED_NORMAL,
		.sched_nice = nice,
	};
	WARN_ON_ONCE(sched_setattr_nocheck(p, &attr) != 0);
}
EXPORT_প্রতীক_GPL(sched_set_normal);

স্থির পূর্ণসংখ্যা
do_sched_setscheduler(pid_t pid, পূর্ণসংখ্যা policy, কাঠামো sched_param __user *param)
{
	কাঠামো sched_param lparam;

	যদি (unlikely(!param || pid < 0))
		ফেরত -EINVAL;
	যদি (copy_from_user(&lparam, param, আকার(কাঠামো sched_param)))
		ফেরত -EFAULT;

	CLASS(find_get_task, p)(pid);
	যদি (!p)
		ফেরত -ESRCH;

	ফেরত sched_setscheduler(p, policy, &lparam);
}

/*
 * Mimics kernel/events/core.c perf_copy_attr().
 */
স্থির পূর্ণসংখ্যা sched_copy_attr(কাঠামো sched_attr __user *uattr, কাঠামো sched_attr *attr)
{
	u32 আকার_2;
	পূর্ণসংখ্যা ret;

	/* Zero the full structure, so that a short copy will be nice: */
	memset(attr, 0, আকার(*attr));

	ret = get_user(আকার_2, &uattr->আকার_2);
	যদি (ret)
		ফেরত ret;

	/* ABI compatibility quirk: */
	যদি (!আকার_2)
		আকার_2 = SCHED_ATTR_আকার_VER0;
	যদি (আকার_2 < SCHED_ATTR_আকার_VER0 || আকার_2 > PAGE_আকার)
		যাও err_আকার;

	ret = copy_struct_from_user(attr, আকার(*attr), uattr, আকার_2);
	যদি (ret) {
		যদি (ret == -E2BIG)
			যাও err_আকার;
		ফেরত ret;
	}

	যদি ((attr->sched_flags & SCHED_FLAG_UTIL_CLAMP) &&
	    আকার_2 < SCHED_ATTR_আকার_VER1)
		ফেরত -EINVAL;

	/*
	 * XXX: Do we want to be lenient like existing syscalls; or do we want
	 * to be strict and return an error on out-of-bounds values?
	 */
	attr->sched_nice = clamp(attr->sched_nice, MIN_NICE, MAX_NICE);

	ফেরত 0;

err_আকার:
	put_user(আকার(*attr), &uattr->আকার_2);
	ফেরত -E2BIG;
}

স্থির শূন্য get_params(কাঠামো task_struct *p, কাঠামো sched_attr *attr, unsigned পূর্ণসংখ্যা flags)
{
	যদি (task_has_dl_policy(p)) {
		__getparam_dl(p, attr, flags);
	} অন্যথা যদি (task_has_rt_policy(p)) {
		attr->sched_priority = p->rt_priority;
	} অন্যথা {
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
SYSCALL_DEFINE3(sched_setscheduler, pid_t, pid, পূর্ণসংখ্যা, policy, কাঠামো sched_param __user *, param)
{
	যদি (policy < 0)
		ফেরত -EINVAL;

	ফেরত do_sched_setscheduler(pid, policy, param);
}

/**
 * sys_sched_setparam - set/change the RT priority of a thread
 * @pid: the pid in question.
 * @param: structure containing the new RT priority.
 *
 * Return: 0 on success. An error code otherwise.
 */
SYSCALL_DEFINE2(sched_setparam, pid_t, pid, কাঠামো sched_param __user *, param)
{
	ফেরত do_sched_setscheduler(pid, SETPARAM_POLICY, param);
}

/**
 * sys_sched_setattr - same as above, but with extended sched_attr
 * @pid: the pid in question.
 * @uattr: structure containing the extended parameters.
 * @flags: for future extension.
 */
SYSCALL_DEFINE3(sched_setattr, pid_t, pid, কাঠামো sched_attr __user *, uattr,
			       unsigned পূর্ণসংখ্যা, flags)
{
	কাঠামো sched_attr attr;
	পূর্ণসংখ্যা retval;

	যদি (unlikely(!uattr || pid < 0 || flags))
		ফেরত -EINVAL;

	retval = sched_copy_attr(uattr, &attr);
	যদি (retval)
		ফেরত retval;

	যদি ((পূর্ণসংখ্যা)attr.sched_policy < 0)
		ফেরত -EINVAL;
	যদি (attr.sched_flags & SCHED_FLAG_KEEP_POLICY)
		attr.sched_policy = SETPARAM_POLICY;

	CLASS(find_get_task, p)(pid);
	যদি (!p)
		ফেরত -ESRCH;

	যদি (attr.sched_flags & SCHED_FLAG_KEEP_PARAMS)
		get_params(p, &attr, 0);

	ফেরত sched_setattr(p, &attr);
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
	কাঠামো task_struct *p;
	পূর্ণসংখ্যা retval;

	যদি (pid < 0)
		ফেরত -EINVAL;

	guard(rcu)();
	p = find_process_by_pid(pid);
	যদি (!p)
		ফেরত -ESRCH;

	retval = security_task_getscheduler(p);
	যদি (!retval) {
		retval = p->policy;
		যদি (p->sched_reset_on_fork)
			retval |= SCHED_RESET_ON_FORK;
	}
	ফেরত retval;
}

/**
 * sys_sched_getparam - get the RT priority of a thread
 * @pid: the pid in question.
 * @param: structure containing the RT priority.
 *
 * Return: On success, 0 and the RT priority is in @param. Otherwise, an error
 * code.
 */
SYSCALL_DEFINE2(sched_getparam, pid_t, pid, কাঠামো sched_param __user *, param)
{
	কাঠামো sched_param lp = { .sched_priority = 0 };
	কাঠামো task_struct *p;
	পূর্ণসংখ্যা retval;

	যদি (unlikely(!param || pid < 0))
		ফেরত -EINVAL;

	scoped_guard (rcu) {
		p = find_process_by_pid(pid);
		যদি (!p)
			ফেরত -ESRCH;

		retval = security_task_getscheduler(p);
		যদি (retval)
			ফেরত retval;

		যদি (task_has_rt_policy(p))
			lp.sched_priority = p->rt_priority;
	}

	/*
	 * This one might sleep, we cannot do it with a spinlock held ...
	 */
	ফেরত copy_to_user(param, &lp, আকার(*param)) ? -EFAULT : 0;
}

/**
 * sys_sched_getattr - similar to sched_getparam, but with sched_attr
 * @pid: the pid in question.
 * @uattr: structure containing the extended parameters.
 * @usize: sizeof(attr) for fwd/bwd comp.
 * @flags: for future extension.
 */
SYSCALL_DEFINE4(sched_getattr, pid_t, pid, কাঠামো sched_attr __user *, uattr,
		unsigned পূর্ণসংখ্যা, usize, unsigned পূর্ণসংখ্যা, flags)
{
	কাঠামো sched_attr kattr = { };
	কাঠামো task_struct *p;
	পূর্ণসংখ্যা retval;

	যদি (unlikely(!uattr || pid < 0 || usize > PAGE_আকার ||
		     usize < SCHED_ATTR_আকার_VER0))
		ফেরত -EINVAL;

	scoped_guard (rcu) {
		p = find_process_by_pid(pid);
		যদি (!p)
			ফেরত -ESRCH;

		যদি (flags) {
			যদি (!task_has_dl_policy(p) ||
			    flags != SCHED_GETATTR_FLAG_DL_DYNAMIC)
				ফেরত -EINVAL;
		}

		retval = security_task_getscheduler(p);
		যদি (retval)
			ফেরত retval;

		kattr.sched_policy = p->policy;
		যদি (p->sched_reset_on_fork)
			kattr.sched_flags |= SCHED_FLAG_RESET_ON_FORK;
		get_params(p, &kattr, flags);
		kattr.sched_flags &= SCHED_FLAG_ALL;

#ifdef CONFIG_UCLAMP_TASK
		/*
		 * This could race with another potential updater, but this is fine
		 * because it'll correctly read the old or the new value. We don't need
		 * to guarantee who wins the race as long as it doesn't return garbage.
		 */
		kattr.sched_util_min = p->uclamp_req[UCLAMP_MIN].মান;
		kattr.sched_util_max = p->uclamp_req[UCLAMP_MAX].মান;
#endif
	}

	kattr.আকার_2 = min(usize, আকার(kattr));
	ফেরত copy_struct_to_user(uattr, usize, &kattr, আকার(kattr), ফাঁকা);
}

পূর্ণসংখ্যা dl_task_check_affinity(কাঠামো task_struct *p, ধ্রুবক কাঠামো cpumask *mask)
{
	/*
	 * If the task isn't a deadline task or admission control is
	 * disabled then we don't care about affinity changes.
	 */
	যদি (!task_has_dl_policy(p) || !dl_bandwidth_enabled())
		ফেরত 0;

	/*
	 * The special/sugov task isn't part of regular bandwidth/admission
	 * control so let userspace change affinities.
	 */
	যদি (dl_entity_is_special(&p->dl))
		ফেরত 0;

	/*
	 * Since bandwidth control happens on root_domain basis,
	 * if admission test is enabled, we only admit -deadline
	 * tasks allowed to run on all the CPUs in the task's
	 * root_domain.
	 */
	guard(rcu)();
	যদি (!cpumask_subset(task_rq(p)->rd->span, mask))
		ফেরত -EBUSY;

	ফেরত 0;
}

পূর্ণসংখ্যা __sched_setaffinity(কাঠামো task_struct *p, কাঠামো affinity_context *ctx)
{
	পূর্ণসংখ্যা retval;
	cpumask_var_t cpus_allowed, new_mask;

	যদি (!alloc_cpumask_var(&cpus_allowed, GFP_KERNEL))
		ফেরত -ENOMEM;

	যদি (!alloc_cpumask_var(&new_mask, GFP_KERNEL)) {
		retval = -ENOMEM;
		যাও out_free_cpus_allowed;
	}

	cpuset_cpus_allowed(p, cpus_allowed);
	cpumask_and(new_mask, ctx->new_mask, cpus_allowed);

	ctx->new_mask = new_mask;
	ctx->flags |= SCA_CHECK;

	retval = dl_task_check_affinity(p, new_mask);
	যদি (retval)
		যাও out_free_new_mask;

	retval = __set_cpus_allowed_ptr(p, ctx);
	যদি (retval)
		যাও out_free_new_mask;

	cpuset_cpus_allowed(p, cpus_allowed);
	যদি (!cpumask_subset(new_mask, cpus_allowed)) {
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
		যদি (unlikely((ctx->flags & SCA_USER) && ctx->user_mask)) {
			bool empty = !cpumask_and(new_mask, new_mask,
						  ctx->user_mask);

			যদি (empty)
				cpumask_copy(new_mask, cpus_allowed);
		}
		__set_cpus_allowed_ptr(p, ctx);
		retval = -EINVAL;
	}

out_free_new_mask:
	free_cpumask_var(new_mask);
out_free_cpus_allowed:
	free_cpumask_var(cpus_allowed);
	ফেরত retval;
}

long sched_setaffinity(pid_t pid, ধ্রুবক কাঠামো cpumask *in_mask)
{
	কাঠামো affinity_context ac;
	কাঠামো cpumask *user_mask;
	পূর্ণসংখ্যা retval;

	CLASS(find_get_task, p)(pid);
	যদি (!p)
		ফেরত -ESRCH;

	যদি (p->flags & PF_NO_SETAFFINITY)
		ফেরত -EINVAL;

	যদি (!check_same_owner(p)) {
		guard(rcu)();
		যদি (!ns_capable(__task_cred(p)->user_ns, CAP_SYS_NICE))
			ফেরত -EPERM;
	}

	retval = security_task_setscheduler(p);
	যদি (retval)
		ফেরত retval;

	/*
	 * With non-SMP configs, user_cpus_ptr/user_mask isn't used and
	 * alloc_user_cpus_ptr() returns NULL.
	 */
	user_mask = alloc_user_cpus_ptr(NUMA_NO_গ্রন্থি);
	যদি (user_mask) {
		cpumask_copy(user_mask, in_mask);
	} অন্যথা {
		ফেরত -ENOMEM;
	}

	ac = (কাঠামো affinity_context){
		.new_mask  = in_mask,
		.user_mask = user_mask,
		.flags     = SCA_USER,
	};

	retval = __sched_setaffinity(p, &ac);
	kfree(ac.user_mask);

	ফেরত retval;
}

স্থির পূর্ণসংখ্যা get_user_cpu_mask(unsigned long __user *user_mask_ptr, unsigned len,
			     কাঠামো cpumask *new_mask)
{
	যদি (len < cpumask_আকার())
		cpumask_clear(new_mask);
	অন্যথা যদি (len > cpumask_আকার())
		len = cpumask_আকার();

	ফেরত copy_from_user(new_mask, user_mask_ptr, len) ? -EFAULT : 0;
}

/**
 * sys_sched_setaffinity - set the CPU affinity of a process
 * @pid: pid of the process
 * @len: length in bytes of the bitmask pointed to by user_mask_ptr
 * @user_mask_ptr: user-space pointer to the new CPU mask
 *
 * Return: 0 on success. An error code otherwise.
 */
SYSCALL_DEFINE3(sched_setaffinity, pid_t, pid, unsigned পূর্ণসংখ্যা, len,
		unsigned long __user *, user_mask_ptr)
{
	cpumask_var_t new_mask;
	পূর্ণসংখ্যা retval;

	যদি (!alloc_cpumask_var(&new_mask, GFP_KERNEL))
		ফেরত -ENOMEM;

	retval = get_user_cpu_mask(user_mask_ptr, len, new_mask);
	যদি (retval == 0)
		retval = sched_setaffinity(pid, new_mask);
	free_cpumask_var(new_mask);
	ফেরত retval;
}

long sched_getaffinity(pid_t pid, কাঠামো cpumask *mask)
{
	কাঠামো task_struct *p;
	পূর্ণসংখ্যা retval;

	guard(rcu)();
	p = find_process_by_pid(pid);
	যদি (!p)
		ফেরত -ESRCH;

	retval = security_task_getscheduler(p);
	যদি (retval)
		ফেরত retval;

	guard(raw_spinlock_irqsave)(&p->pi_lock);
	cpumask_and(mask, &p->cpus_mask, cpu_active_mask);

	ফেরত 0;
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
SYSCALL_DEFINE3(sched_getaffinity, pid_t, pid, unsigned পূর্ণসংখ্যা, len,
		unsigned long __user *, user_mask_ptr)
{
	পূর্ণসংখ্যা ret;
	cpumask_var_t mask;

	যদি ((len * BITS_PER_BYTE) < nr_cpu_ids)
		ফেরত -EINVAL;
	যদি (len & (আকার(unsigned long)-1))
		ফেরত -EINVAL;

	যদি (!zalloc_cpumask_var(&mask, GFP_KERNEL))
		ফেরত -ENOMEM;

	ret = sched_getaffinity(pid, mask);
	যদি (ret == 0) {
		unsigned পূর্ণসংখ্যা retlen = min(len, cpumask_আকার());

		যদি (copy_to_user(user_mask_ptr, cpumask_bits(mask), retlen))
			ret = -EFAULT;
		অন্যথা
			ret = retlen;
	}
	free_cpumask_var(mask);

	ফেরত ret;
}

স্থির শূন্য do_sched_yield(শূন্য)
{
	কাঠামো rq_flags rf;
	কাঠামো rq *rq;

	rq = this_rq_lock_irq(&rf);

	schedstat_inc(rq->yld_গুনতি);
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
	ফেরত 0;
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
শূন্য __sched yield(শূন্য)
{
	set_current_state(TASK_RUNNING);
	do_sched_yield();
}
EXPORT_প্রতীক(yield);

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
পূর্ণসংখ্যা __sched yield_to(কাঠামো task_struct *p, bool preempt)
{
	কাঠামো task_struct *curr;
	কাঠামো rq *rq, *p_rq;
	পূর্ণসংখ্যা yielded = 0;

	scoped_guard (raw_spinlock_irqsave, &p->pi_lock) {
		rq = this_rq();
		curr = rq->donor;

again:
		p_rq = task_rq(p);
		/*
		 * If we're the only runnable task on the rq and target rq also
		 * has only one task, there's absolutely no point in yielding.
		 */
		যদি (rq->nr_running == 1 && p_rq->nr_running == 1)
			ফেরত -ESRCH;

		guard(double_rq_lock)(rq, p_rq);
		যদি (task_rq(p) != p_rq)
			যাও again;

		যদি (!curr->sched_class->yield_to_task)
			ফেরত 0;

		যদি (curr->sched_class != p->sched_class)
			ফেরত 0;

		যদি (task_on_cpu(p_rq, p) || !task_is_running(p))
			ফেরত 0;

		yielded = curr->sched_class->yield_to_task(rq, p);
		যদি (yielded) {
			schedstat_inc(rq->yld_গুনতি);
			/*
			 * Make p's CPU reschedule; pick_next_entity
			 * takes care of fairness.
			 */
			যদি (preempt && rq != p_rq)
				resched_curr(p_rq);
		}
	}

	যদি (yielded)
		schedule();

	ফেরত yielded;
}
EXPORT_প্রতীক_GPL(yield_to);

/**
 * sys_sched_get_priority_max - return maximum RT priority.
 * @policy: scheduling class.
 *
 * Return: On success, this syscall returns the maximum
 * rt_priority that can be used by a given scheduling class.
 * On failure, a negative error code is returned.
 */
SYSCALL_DEFINE1(sched_get_priority_max, পূর্ণসংখ্যা, policy)
{
	পূর্ণসংখ্যা ret = -EINVAL;

	নির্বাচন (policy) {
	ক্ষেত্র SCHED_FIFO:
	ক্ষেত্র SCHED_RR:
		ret = MAX_RT_PRIO-1;
		বিরতি;
	ক্ষেত্র SCHED_DEADLINE:
	ক্ষেত্র SCHED_NORMAL:
	ক্ষেত্র SCHED_BATCH:
	ক্ষেত্র SCHED_IDLE:
	ক্ষেত্র SCHED_EXT:
		ret = 0;
		বিরতি;
	}
	ফেরত ret;
}

/**
 * sys_sched_get_priority_min - return minimum RT priority.
 * @policy: scheduling class.
 *
 * Return: On success, this syscall returns the minimum
 * rt_priority that can be used by a given scheduling class.
 * On failure, a negative error code is returned.
 */
SYSCALL_DEFINE1(sched_get_priority_min, পূর্ণসংখ্যা, policy)
{
	পূর্ণসংখ্যা ret = -EINVAL;

	নির্বাচন (policy) {
	ক্ষেত্র SCHED_FIFO:
	ক্ষেত্র SCHED_RR:
		ret = 1;
		বিরতি;
	ক্ষেত্র SCHED_DEADLINE:
	ক্ষেত্র SCHED_NORMAL:
	ক্ষেত্র SCHED_BATCH:
	ক্ষেত্র SCHED_IDLE:
	ক্ষেত্র SCHED_EXT:
		ret = 0;
	}
	ফেরত ret;
}

স্থির পূর্ণসংখ্যা sched_rr_get_interval(pid_t pid, কাঠামো timespec64 *t)
{
	unsigned পূর্ণসংখ্যা time_slice = 0;
	পূর্ণসংখ্যা retval;

	যদি (pid < 0)
		ফেরত -EINVAL;

	scoped_guard (rcu) {
		কাঠামো task_struct *p = find_process_by_pid(pid);
		যদি (!p)
			ফেরত -ESRCH;

		retval = security_task_getscheduler(p);
		যদি (retval)
			ফেরত retval;

		scoped_guard (task_rq_lock, p) {
			কাঠামো rq *rq = scope.rq;
			যদি (p->sched_class->get_rr_interval)
				time_slice = p->sched_class->get_rr_interval(rq, p);
		}
	}

	jiffies_to_timespec64(time_slice, t);
	ফেরত 0;
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
		কাঠামো __kernel_timespec __user *, interval)
{
	কাঠামো timespec64 t;
	পূর্ণসংখ্যা retval = sched_rr_get_interval(pid, &t);

	যদি (retval == 0)
		retval = put_timespec64(&t, interval);

	ফেরত retval;
}

#ifdef CONFIG_COMPAT_32BIT_TIME
SYSCALL_DEFINE2(sched_rr_get_interval_time32, pid_t, pid,
		কাঠামো old_timespec32 __user *, interval)
{
	কাঠামো timespec64 t;
	পূর্ণসংখ্যা retval = sched_rr_get_interval(pid, &t);

	যদি (retval == 0)
		retval = put_old_timespec32(&t, interval);
	ফেরত retval;
}
#endif
