// SPDX-License-Identifier: GPL-2.0-only
/*
 *  kernel/sched/syscalls.c
 *
 *  Core kernel scheduler syscalls related code
 *
 *  Copyright (C) 1991-2002  Linus Torvalds
 *  Copyright (C) 1998-2024  Ingo Molnar, Red Hat
 */
#அந்தர்பாவ <linux/sched.h>
#அந்தர்பாவ <linux/cpuset.h>
#அந்தர்பாவ <linux/sched/debug.h>

#அந்தர்பாவ <uapi/linux/sched/types.h>

#அந்தர்பாவ "sched.h"
#அந்தர்பாவ "autogroup.h"

நிலையான உள்வரி முழு __normal_prio(முழு policy, முழு rt_prio, முழு nice)
{
	முழு prio;

	எனில் (dl_policy(policy))
		prio = MAX_DL_PRIO - 1;
	இல்லையெனில் எனில் (rt_policy(policy))
		prio = MAX_RT_PRIO - 1 - rt_prio;
	இல்லையெனில்
		prio = NICE_TO_PRIO(nice);

	திரும்பு prio;
}

/*
 * Calculate the expected normal priority: i.e. priority
 * without taking RT-inheritance into account. Might be
 * boosted by interactivity modifiers. Changes upon fork,
 * setprio syscalls, and whenever the interactivity
 * estimator recalculates.
 */
நிலையான உள்வரி முழு normal_prio(கட்டமைப்பு task_struct *p)
{
	திரும்பு __normal_prio(p->policy, p->rt_priority, PRIO_TO_NICE(p->static_prio));
}

/*
 * Calculate the current priority, i.e. the priority
 * taken into account by the scheduler. This value might
 * be boosted by RT tasks, or might be boosted by
 * interactivity modifiers. Will be RT if the task got
 * RT-boosted. If not then it returns p->normal_prio.
 */
நிலையான முழு effective_prio(கட்டமைப்பு task_struct *p)
{
	p->normal_prio = normal_prio(p);
	/*
	 * If we are RT tasks or we were boosted to RT priority,
	 * keep the priority unchanged. Otherwise, update priority
	 * to the normal priority:
	 */
	எனில் (!rt_or_dl_prio(p->prio))
		திரும்பு p->normal_prio;
	திரும்பு p->prio;
}

வெறுமை set_user_nice(கட்டமைப்பு task_struct *p, long nice)
{
	முழு old_prio;

	எனில் (task_nice(p) == nice || nice < MIN_NICE || nice > MAX_NICE)
		திரும்பு;
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
	எனில் (task_has_dl_policy(p) || task_has_rt_policy(p)) {
		p->static_prio = NICE_TO_PRIO(nice);
		திரும்பு;
	}

	scoped_guard (sched_change, p, DEQUEUE_SAVE) {
		p->static_prio = NICE_TO_PRIO(nice);
		set_load_weight(p, true);
		old_prio = p->prio;
		p->prio = effective_prio(p);
	}
}
EXPORT_குறியீடு(set_user_nice);

/*
 * is_nice_reduction - check if nice value is an actual reduction
 *
 * Similar to can_nice() but does not perform a capability check.
 *
 * @p: task
 * @nice: nice value
 */
நிலையான bool is_nice_reduction(மாறிலி கட்டமைப்பு task_struct *p, மாறிலி முழு nice)
{
	/* Convert nice value [19,-20] to rlimit style value [1,40]: */
	முழு nice_rlim = nice_to_rlimit(nice);

	திரும்பு (nice_rlim <= task_rlimit(p, RLIMIT_NICE));
}

/*
 * can_nice - check if a task can reduce its nice value
 * @p: task
 * @nice: nice value
 */
முழு can_nice(மாறிலி கட்டமைப்பு task_struct *p, மாறிலி முழு nice)
{
	திரும்பு is_nice_reduction(p, nice) || capable(CAP_SYS_NICE);
}

#ifdef __ARCH_WANT_SYS_NICE

/*
 * sys_nice - change the priority of the current process.
 * @increment: priority increment
 *
 * sys_setpriority is a more generic, but much slower function that
 * does similar things.
 */
SYSCALL_DEFINE1(nice, முழு, increment)
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
	எனில் (increment < 0 && !can_nice(current, nice))
		திரும்பு -EPERM;

	retval = security_task_setnice(current, nice);
	எனில் (retval)
		திரும்பு retval;

	set_user_nice(current, nice);
	திரும்பு 0;
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
முழு task_prio(மாறிலி கட்டமைப்பு task_struct *p)
{
	திரும்பு p->prio - MAX_RT_PRIO;
}

/**
 * idle_cpu - is a given CPU idle currently?
 * @cpu: the processor in question.
 *
 * Return: 1 if the CPU is currently idle. 0 otherwise.
 */
முழு idle_cpu(முழு cpu)
{
	திரும்பு idle_rq(cpu_rq(cpu));
}

/**
 * idle_task - return the idle task for a given CPU.
 * @cpu: the processor in question.
 *
 * Return: The idle task for the CPU @cpu.
 */
கட்டமைப்பு task_struct *idle_task(முழு cpu)
{
	திரும்பு cpu_rq(cpu)->idle;
}

#ifdef CONFIG_SCHED_CORE
முழு sched_core_idle_cpu(முழு cpu)
{
	கட்டமைப்பு rq *rq = cpu_rq(cpu);

	எனில் (sched_core_enabled(rq) && rq->curr == rq->idle)
		திரும்பு 1;

	திரும்பு idle_cpu(cpu);
}
#endif /* CONFIG_SCHED_CORE */

/**
 * find_process_by_pid - find a process with a matching PID value.
 * @pid: the pid in question.
 *
 * The task of @pid, if found. %NULL otherwise.
 */
நிலையான கட்டமைப்பு task_struct *find_process_by_pid(pid_t pid)
{
	திரும்பு pid ? find_task_by_vpid(pid) : current;
}

நிலையான கட்டமைப்பு task_struct *find_get_task(pid_t pid)
{
	கட்டமைப்பு task_struct *p;
	guard(rcu)();

	p = find_process_by_pid(pid);
	எனில் (likely(p))
		get_task_struct(p);

	திரும்பு p;
}

DEFINE_CLASS(find_get_task, கட்டமைப்பு task_struct *, எனில் (_T) put_task_struct(_T),
	     find_get_task(pid), pid_t pid)

/*
 * sched_setparam() passes in -1 for its policy, to let the functions
 * it calls know not to change it.
 */
#define SETPARAM_POLICY	-1

நிலையான வெறுமை __setscheduler_params(கட்டமைப்பு task_struct *p,
		மாறிலி கட்டமைப்பு sched_attr *attr)
{
	முழு policy = attr->sched_policy;

	எனில் (policy == SETPARAM_POLICY)
		policy = p->policy;

	p->policy = policy;

	எனில் (dl_policy(policy))
		__setparam_dl(p, attr);
	இல்லையெனில் எனில் (fair_policy(policy))
		__setparam_fair(p, attr);

	/* rt-policy tasks do not have a timerslack */
	எனில் (rt_or_dl_task_policy(p)) {
		p->timer_slack_ns = 0;
	} இல்லையெனில் எனில் (p->timer_slack_ns == 0) {
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
நிலையான bool check_same_owner(கட்டமைப்பு task_struct *p)
{
	மாறிலி கட்டமைப்பு cred *cred = current_cred(), *pcred;
	guard(rcu)();

	pcred = __task_cred(p);
	திரும்பு (uid_eq(cred->euid, pcred->euid) ||
		uid_eq(cred->euid, pcred->uid));
}

#ifdef CONFIG_RT_MUTEXES
நிலையான உள்வரி வெறுமை __setscheduler_dl_pi(முழு newprio, முழு policy,
			      கட்டமைப்பு task_struct *p,
			      கட்டமைப்பு sched_change_ctx *scope)
{
	/*
	 * In case a DEADLINE task (either proper or boosted) gets
	 * setscheduled to a lower priority class, check if it neeeds to
	 * inherit parameters from a potential pi_task. In that case make
	 * sure replenishment happens with the next enqueue.
	 */

	எனில் (dl_prio(newprio) && !dl_policy(policy)) {
		கட்டமைப்பு task_struct *pi_task = rt_mutex_get_உச்சி_task(p);

		எனில் (pi_task) {
			p->dl.pi_se = pi_task->dl.pi_se;
			scope->flags |= ENQUEUE_REPLENISH;
		}
	}
}
#இல்லையெனில் /* !CONFIG_RT_MUTEXES */
நிலையான உள்வரி வெறுமை __setscheduler_dl_pi(முழு newprio, முழு policy,
			      கட்டமைப்பு task_struct *p,
			      கட்டமைப்பு sched_change_ctx *scope)
{
}
#endif /* !CONFIG_RT_MUTEXES */

#ifdef CONFIG_UCLAMP_TASK

நிலையான முழு uclamp_validate(கட்டமைப்பு task_struct *p,
			   மாறிலி கட்டமைப்பு sched_attr *attr)
{
	முழு util_min = p->uclamp_req[UCLAMP_MIN].மதிப்பு;
	முழு util_max = p->uclamp_req[UCLAMP_MAX].மதிப்பு;

	எனில் (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MIN) {
		util_min = attr->sched_util_min;

		எனில் (util_min + 1 > SCHED_CAPACITY_SCALE + 1)
			திரும்பு -EINVAL;
	}

	எனில் (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MAX) {
		util_max = attr->sched_util_max;

		எனில் (util_max + 1 > SCHED_CAPACITY_SCALE + 1)
			திரும்பு -EINVAL;
	}

	எனில் (util_min != -1 && util_max != -1 && util_min > util_max)
		திரும்பு -EINVAL;

	/*
	 * We have valid uclamp attributes; make sure uclamp is enabled.
	 *
	 * We need to do that here, because enabling static branches is a
	 * blocking operation which obviously cannot be done while holding
	 * scheduler locks.
	 */
	sched_uclamp_enable();

	திரும்பு 0;
}

நிலையான bool uclamp_reset(மாறிலி கட்டமைப்பு sched_attr *attr,
			 எண்ணிக்கை uclamp_id clamp_id,
			 கட்டமைப்பு uclamp_se *uc_se)
{
	/* Reset on sched class change for a non user-defined clamp value. */
	எனில் (likely(!(attr->sched_flags & SCHED_FLAG_UTIL_CLAMP)) &&
	    !uc_se->user_defined)
		திரும்பு true;

	/* Reset on sched_util_{min,max} == -1. */
	எனில் (clamp_id == UCLAMP_MIN &&
	    attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MIN &&
	    attr->sched_util_min == -1) {
		திரும்பு true;
	}

	எனில் (clamp_id == UCLAMP_MAX &&
	    attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MAX &&
	    attr->sched_util_max == -1) {
		திரும்பு true;
	}

	திரும்பு false;
}

நிலையான வெறுமை __setscheduler_uclamp(கட்டமைப்பு task_struct *p,
				  மாறிலி கட்டமைப்பு sched_attr *attr)
{
	எண்ணிக்கை uclamp_id clamp_id;

	for_each_clamp_id(clamp_id) {
		கட்டமைப்பு uclamp_se *uc_se = &p->uclamp_req[clamp_id];
		unsigned முழு மதிப்பு;

		எனில் (!uclamp_reset(attr, clamp_id, uc_se))
			தொடர்;

		/*
		 * RT by default have a 100% boost value that could be modified
		 * at runtime.
		 */
		எனில் (unlikely(rt_task(p) && clamp_id == UCLAMP_MIN))
			மதிப்பு = sysctl_sched_uclamp_util_min_rt_default;
		இல்லையெனில்
			மதிப்பு = uclamp_none(clamp_id);

		uclamp_se_set(uc_se, மதிப்பு, false);

	}

	எனில் (likely(!(attr->sched_flags & SCHED_FLAG_UTIL_CLAMP)))
		திரும்பு;

	எனில் (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MIN &&
	    attr->sched_util_min != -1) {
		uclamp_se_set(&p->uclamp_req[UCLAMP_MIN],
			      attr->sched_util_min, true);
	}

	எனில் (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MAX &&
	    attr->sched_util_max != -1) {
		uclamp_se_set(&p->uclamp_req[UCLAMP_MAX],
			      attr->sched_util_max, true);
	}
}

#இல்லையெனில் /* !CONFIG_UCLAMP_TASK: */

நிலையான உள்வரி முழு uclamp_validate(கட்டமைப்பு task_struct *p,
				  மாறிலி கட்டமைப்பு sched_attr *attr)
{
	திரும்பு -EOPNOTSUPP;
}
நிலையான வெறுமை __setscheduler_uclamp(கட்டமைப்பு task_struct *p,
				  மாறிலி கட்டமைப்பு sched_attr *attr) { }
#endif /* !CONFIG_UCLAMP_TASK */

/*
 * Allow unprivileged RT tasks to decrease priority.
 * Only issue a capable test if needed and only once to avoid an audit
 * event on permitted non-privileged operations:
 */
நிலையான முழு user_check_sched_setscheduler(கட்டமைப்பு task_struct *p,
					 மாறிலி கட்டமைப்பு sched_attr *attr,
					 முழு policy, முழு reset_on_fork)
{
	எனில் (fair_policy(policy)) {
		எனில் (attr->sched_nice < task_nice(p) &&
		    !is_nice_reduction(p, attr->sched_nice))
			செல் req_priv;
	}

	எனில் (rt_policy(policy)) {
		unsigned long rlim_rtprio = task_rlimit(p, RLIMIT_RTPRIO);

		/* Can't set/change the rt policy: */
		எனில் (policy != p->policy && !rlim_rtprio)
			செல் req_priv;

		/* Can't increase priority: */
		எனில் (attr->sched_priority > p->rt_priority &&
		    attr->sched_priority > rlim_rtprio)
			செல் req_priv;
	}

	/*
	 * Can't set/change SCHED_DEADLINE policy at all for now
	 * (safest behavior); in the future we would like to allow
	 * unprivileged DL tasks to increase their relative deadline
	 * or reduce their runtime (both ways reducing utilization)
	 */
	எனில் (dl_policy(policy))
		செல் req_priv;

	/*
	 * Treat SCHED_IDLE as nice 20. Only allow a switch to
	 * SCHED_NORMAL if the RLIMIT_NICE would normally permit it.
	 */
	எனில் (task_has_idle_policy(p) && !idle_policy(policy)) {
		எனில் (!is_nice_reduction(p, task_nice(p)))
			செல் req_priv;
	}

	/* Can't change other user's priorities: */
	எனில் (!check_same_owner(p))
		செல் req_priv;

	/* Normal users shall not reset the sched_reset_on_fork flag: */
	எனில் (p->sched_reset_on_fork && !reset_on_fork)
		செல் req_priv;

	திரும்பு 0;

req_priv:
	எனில் (!capable(CAP_SYS_NICE))
		திரும்பு -EPERM;

	திரும்பு 0;
}

முழு __sched_setscheduler(கட்டமைப்பு task_struct *p,
			 மாறிலி கட்டமைப்பு sched_attr *attr,
			 bool user, bool pi)
{
	முழு oldpolicy = -1, policy = attr->sched_policy;
	முழு retval, oldprio, newprio;
	மாறிலி கட்டமைப்பு sched_class *prev_class, *next_class;
	கட்டமைப்பு balance_callback *head;
	கட்டமைப்பு rq_flags rf;
	முழு reset_on_fork;
	முழு queue_flags = DEQUEUE_SAVE | DEQUEUE_MOVE | DEQUEUE_NOCLOCK;
	கட்டமைப்பு rq *rq;
	bool cpuset_locked = false;

	/* The pi code expects interrupts enabled */
	BUG_ON(pi && in_interrupt());
recheck:
	/* Double check policy once rq lock held: */
	எனில் (policy < 0) {
		reset_on_fork = p->sched_reset_on_fork;
		policy = oldpolicy = p->policy;
	} இல்லையெனில் {
		reset_on_fork = !!(attr->sched_flags & SCHED_FLAG_RESET_ON_FORK);

		எனில் (!valid_policy(policy))
			திரும்பு -EINVAL;
	}

	எனில் (attr->sched_flags & ~(SCHED_FLAG_ALL | SCHED_FLAG_SUGOV))
		திரும்பு -EINVAL;

	/*
	 * Valid priorities for SCHED_FIFO and SCHED_RR are
	 * 1..MAX_RT_PRIO-1, valid priority for SCHED_NORMAL,
	 * SCHED_BATCH and SCHED_IDLE is 0.
	 */
	எனில் (attr->sched_priority > MAX_RT_PRIO-1)
		திரும்பு -EINVAL;
	எனில் ((dl_policy(policy) && !__checkparam_dl(attr)) ||
	    (rt_policy(policy) != (attr->sched_priority != 0)))
		திரும்பு -EINVAL;

	எனில் (user) {
		retval = user_check_sched_setscheduler(p, attr, policy, reset_on_fork);
		எனில் (retval)
			திரும்பு retval;

		எனில் (attr->sched_flags & SCHED_FLAG_SUGOV)
			திரும்பு -EINVAL;

		retval = security_task_setscheduler(p);
		எனில் (retval)
			திரும்பு retval;
	}

	/* Update task specific "requested" clamps */
	எனில் (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP) {
		retval = uclamp_validate(p, attr);
		எனில் (retval)
			திரும்பு retval;
	}

	/*
	 * SCHED_DEADLINE bandwidth accounting relies on stable cpusets
	 * information.
	 */
	எனில் (dl_policy(policy) || dl_policy(p->policy)) {
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
	எனில் (p == rq->stop) {
		retval = -EINVAL;
		செல் unlock;
	}

	retval = scx_check_setscheduler(p, policy);
	எனில் (retval)
		செல் unlock;

	/*
	 * If not changing anything there's no need to proceed further,
	 * but store a possible modification of reset_on_fork.
	 */
	எனில் (unlikely(policy == p->policy)) {
		எனில் (fair_policy(policy) &&
		    (attr->sched_nice != task_nice(p) ||
		     (attr->sched_runtime != p->se.slice)))
			செல் change;
		எனில் (rt_policy(policy) && attr->sched_priority != p->rt_priority)
			செல் change;
		எனில் (dl_policy(policy) && dl_param_changed(p, attr))
			செல் change;
		எனில் (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP)
			செல் change;

		p->sched_reset_on_fork = reset_on_fork;
		retval = 0;
		செல் unlock;
	}
change:

	எனில் (user) {
#ifdef CONFIG_RT_GROUP_SCHED
		/*
		 * Do not allow real-time tasks into groups that have no runtime
		 * assigned.
		 */
		எனில் (rt_group_sched_enabled() &&
				rt_bandwidth_enabled() && rt_policy(policy) &&
				task_group(p)->rt_bandwidth.rt_runtime == 0 &&
				!task_group_is_autogroup(task_group(p))) {
			retval = -EPERM;
			செல் unlock;
		}
#endif /* CONFIG_RT_GROUP_SCHED */
		எனில் (dl_bandwidth_enabled() && dl_policy(policy) &&
				!(attr->sched_flags & SCHED_FLAG_SUGOV)) {
			cpumask_t *span = rq->rd->span;

			/*
			 * Don't allow tasks with an affinity mask smaller than
			 * the entire root_domain to become SCHED_DEADLINE. We
			 * will also fail if there's no bandwidth available.
			 */
			எனில் (!cpumask_subset(span, p->cpus_ptr) ||
			    rq->rd->dl_bw.bw == 0) {
				retval = -EPERM;
				செல் unlock;
			}
		}
	}

	/* Re-check policy now with rq lock held: */
	எனில் (unlikely(oldpolicy != -1 && oldpolicy != p->policy)) {
		policy = oldpolicy = -1;
		task_rq_unlock(rq, p, &rf);
		எனில் (cpuset_locked)
			cpuset_unlock();
		செல் recheck;
	}

	/*
	 * If setscheduling to SCHED_DEADLINE (or changing the parameters
	 * of a SCHED_DEADLINE task) we need to check if enough bandwidth
	 * is available.
	 */
	எனில் ((dl_policy(policy) || dl_task(p)) && sched_dl_overflow(p, policy, attr)) {
		retval = -EBUSY;
		செல் unlock;
	}

	p->sched_reset_on_fork = reset_on_fork;
	oldprio = p->prio;

	newprio = __normal_prio(policy, attr->sched_priority, attr->sched_nice);
	எனில் (pi) {
		/*
		 * Take priority boosted tasks into account. If the new
		 * effective priority is unchanged, we just store the new
		 * normal parameters and do not touch the scheduler class and
		 * the runqueue. This will be done when the task deboost
		 * itself.
		 */
		newprio = rt_effective_prio(p, newprio);
		எனில் (newprio == oldprio && !dl_prio(newprio))
			queue_flags &= ~DEQUEUE_MOVE;
	}

	prev_class = p->sched_class;
	next_class = __setscheduler_class(policy, newprio);

	எனில் (prev_class != next_class)
		queue_flags |= DEQUEUE_CLASS;

	scoped_guard (sched_change, p, queue_flags) {

		எனில் (!(attr->sched_flags & SCHED_FLAG_KEEP_PARAMS)) {
			__setscheduler_params(p, attr);
			p->sched_class = next_class;
			p->prio = newprio;
			__setscheduler_dl_pi(newprio, policy, p, scope);
		}
		__setscheduler_uclamp(p, attr);

		எனில் (scope->queued) {
			/*
			 * We enqueue to tail when the priority of a task is
			 * increased (user space view).
			 */
			எனில் (oldprio < p->prio)
				scope->flags |= ENQUEUE_HEAD;
		}
	}

	/* Avoid rq from going away on us: */
	preempt_disable();
	head = splice_balance_callbacks(rq);
	task_rq_unlock(rq, p, &rf);

	எனில் (pi) {
		எனில் (cpuset_locked)
			cpuset_unlock();
		rt_mutex_adjust_pi(p);
	}

	/* Run balance callbacks after we've adjusted the PI chain: */
	balance_callbacks(rq, head);
	preempt_enable();

	திரும்பு 0;

unlock:
	task_rq_unlock(rq, p, &rf);
	எனில் (cpuset_locked)
		cpuset_unlock();
	திரும்பு retval;
}

நிலையான முழு _sched_setscheduler(கட்டமைப்பு task_struct *p, முழு policy,
			       மாறிலி கட்டமைப்பு sched_param *param, bool check)
{
	கட்டமைப்பு sched_attr attr = {
		.sched_policy   = policy,
		.sched_priority = param->sched_priority,
		.sched_nice	= PRIO_TO_NICE(p->static_prio),
	};

	எனில் (p->se.custom_slice)
		attr.sched_runtime = p->se.slice;

	/* Fixup the legacy SCHED_RESET_ON_FORK hack. */
	எனில் ((policy != SETPARAM_POLICY) && (policy & SCHED_RESET_ON_FORK)) {
		attr.sched_flags |= SCHED_FLAG_RESET_ON_FORK;
		policy &= ~SCHED_RESET_ON_FORK;
		attr.sched_policy = policy;
	}

	திரும்பு __sched_setscheduler(p, &attr, check, true);
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
முழு sched_setscheduler(கட்டமைப்பு task_struct *p, முழு policy,
		       மாறிலி கட்டமைப்பு sched_param *param)
{
	திரும்பு _sched_setscheduler(p, policy, param, true);
}

முழு sched_setattr(கட்டமைப்பு task_struct *p, மாறிலி கட்டமைப்பு sched_attr *attr)
{
	திரும்பு __sched_setscheduler(p, attr, true, true);
}

முழு sched_setattr_nocheck(கட்டமைப்பு task_struct *p, மாறிலி கட்டமைப்பு sched_attr *attr)
{
	திரும்பு __sched_setscheduler(p, attr, false, true);
}
EXPORT_குறியீடு_GPL(sched_setattr_nocheck);

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
முழு sched_setscheduler_nocheck(கட்டமைப்பு task_struct *p, முழு policy,
			       மாறிலி கட்டமைப்பு sched_param *param)
{
	திரும்பு _sched_setscheduler(p, policy, param, false);
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
வெறுமை sched_set_fifo(கட்டமைப்பு task_struct *p)
{
	கட்டமைப்பு sched_param sp = { .sched_priority = MAX_RT_PRIO / 2 };
	WARN_ON_ONCE(sched_setscheduler_nocheck(p, SCHED_FIFO, &sp) != 0);
}
EXPORT_குறியீடு_GPL(sched_set_fifo);

/*
 * For when you don't much care about FIFO, but want to be above SCHED_NORMAL.
 */
வெறுமை sched_set_fifo_கீழ்(கட்டமைப்பு task_struct *p)
{
	கட்டமைப்பு sched_param sp = { .sched_priority = 1 };
	WARN_ON_ONCE(sched_setscheduler_nocheck(p, SCHED_FIFO, &sp) != 0);
}
EXPORT_குறியீடு_GPL(sched_set_fifo_கீழ்);

/*
 * Used when the primary interrupt handler is forced into a thread, in addition
 * to the (always threaded) secondary handler.  The secondary handler gets a
 * slightly lower priority so that the primary handler can preempt it, thereby
 * emulating the behavior of a non-PREEMPT_RT system where the primary handler
 * runs in hard interrupt context.
 */
வெறுமை sched_set_fifo_secondary(கட்டமைப்பு task_struct *p)
{
	கட்டமைப்பு sched_param sp = { .sched_priority = MAX_RT_PRIO / 2 - 1 };
	WARN_ON_ONCE(sched_setscheduler_nocheck(p, SCHED_FIFO, &sp) != 0);
}

வெறுமை sched_set_normal(கட்டமைப்பு task_struct *p, முழு nice)
{
	கட்டமைப்பு sched_attr attr = {
		.sched_policy = SCHED_NORMAL,
		.sched_nice = nice,
	};
	WARN_ON_ONCE(sched_setattr_nocheck(p, &attr) != 0);
}
EXPORT_குறியீடு_GPL(sched_set_normal);

நிலையான முழு
do_sched_setscheduler(pid_t pid, முழு policy, கட்டமைப்பு sched_param __user *param)
{
	கட்டமைப்பு sched_param lparam;

	எனில் (unlikely(!param || pid < 0))
		திரும்பு -EINVAL;
	எனில் (copy_from_user(&lparam, param, அளவு(கட்டமைப்பு sched_param)))
		திரும்பு -EFAULT;

	CLASS(find_get_task, p)(pid);
	எனில் (!p)
		திரும்பு -ESRCH;

	திரும்பு sched_setscheduler(p, policy, &lparam);
}

/*
 * Mimics kernel/events/core.c perf_copy_attr().
 */
நிலையான முழு sched_copy_attr(கட்டமைப்பு sched_attr __user *uattr, கட்டமைப்பு sched_attr *attr)
{
	u32 size;
	முழு ret;

	/* Zero the full structure, so that a short copy will be nice: */
	memset(attr, 0, அளவு(*attr));

	ret = get_user(size, &uattr->size);
	எனில் (ret)
		திரும்பு ret;

	/* ABI compatibility quirk: */
	எனில் (!size)
		size = SCHED_ATTR_SIZE_VER0;
	எனில் (size < SCHED_ATTR_SIZE_VER0 || size > PAGE_SIZE)
		செல் err_size;

	ret = copy_struct_from_user(attr, அளவு(*attr), uattr, size);
	எனில் (ret) {
		எனில் (ret == -E2BIG)
			செல் err_size;
		திரும்பு ret;
	}

	எனில் ((attr->sched_flags & SCHED_FLAG_UTIL_CLAMP) &&
	    size < SCHED_ATTR_SIZE_VER1)
		திரும்பு -EINVAL;

	/*
	 * XXX: Do we want to be lenient like existing syscalls; or do we want
	 * to be strict and return an error on out-of-bounds values?
	 */
	attr->sched_nice = clamp(attr->sched_nice, MIN_NICE, MAX_NICE);

	திரும்பு 0;

err_size:
	put_user(அளவு(*attr), &uattr->size);
	திரும்பு -E2BIG;
}

நிலையான வெறுமை get_params(கட்டமைப்பு task_struct *p, கட்டமைப்பு sched_attr *attr, unsigned முழு flags)
{
	எனில் (task_has_dl_policy(p)) {
		__getparam_dl(p, attr, flags);
	} இல்லையெனில் எனில் (task_has_rt_policy(p)) {
		attr->sched_priority = p->rt_priority;
	} இல்லையெனில் {
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
SYSCALL_DEFINE3(sched_setscheduler, pid_t, pid, முழு, policy, கட்டமைப்பு sched_param __user *, param)
{
	எனில் (policy < 0)
		திரும்பு -EINVAL;

	திரும்பு do_sched_setscheduler(pid, policy, param);
}

/**
 * sys_sched_setparam - set/change the RT priority of a thread
 * @pid: the pid in question.
 * @param: structure containing the new RT priority.
 *
 * Return: 0 on success. An error code otherwise.
 */
SYSCALL_DEFINE2(sched_setparam, pid_t, pid, கட்டமைப்பு sched_param __user *, param)
{
	திரும்பு do_sched_setscheduler(pid, SETPARAM_POLICY, param);
}

/**
 * sys_sched_setattr - same as above, but with extended sched_attr
 * @pid: the pid in question.
 * @uattr: structure containing the extended parameters.
 * @flags: for future extension.
 */
SYSCALL_DEFINE3(sched_setattr, pid_t, pid, கட்டமைப்பு sched_attr __user *, uattr,
			       unsigned முழு, flags)
{
	கட்டமைப்பு sched_attr attr;
	முழு retval;

	எனில் (unlikely(!uattr || pid < 0 || flags))
		திரும்பு -EINVAL;

	retval = sched_copy_attr(uattr, &attr);
	எனில் (retval)
		திரும்பு retval;

	எனில் ((முழு)attr.sched_policy < 0)
		திரும்பு -EINVAL;
	எனில் (attr.sched_flags & SCHED_FLAG_KEEP_POLICY)
		attr.sched_policy = SETPARAM_POLICY;

	CLASS(find_get_task, p)(pid);
	எனில் (!p)
		திரும்பு -ESRCH;

	எனில் (attr.sched_flags & SCHED_FLAG_KEEP_PARAMS)
		get_params(p, &attr, 0);

	திரும்பு sched_setattr(p, &attr);
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
	கட்டமைப்பு task_struct *p;
	முழு retval;

	எனில் (pid < 0)
		திரும்பு -EINVAL;

	guard(rcu)();
	p = find_process_by_pid(pid);
	எனில் (!p)
		திரும்பு -ESRCH;

	retval = security_task_getscheduler(p);
	எனில் (!retval) {
		retval = p->policy;
		எனில் (p->sched_reset_on_fork)
			retval |= SCHED_RESET_ON_FORK;
	}
	திரும்பு retval;
}

/**
 * sys_sched_getparam - get the RT priority of a thread
 * @pid: the pid in question.
 * @param: structure containing the RT priority.
 *
 * Return: On success, 0 and the RT priority is in @param. Otherwise, an error
 * code.
 */
SYSCALL_DEFINE2(sched_getparam, pid_t, pid, கட்டமைப்பு sched_param __user *, param)
{
	கட்டமைப்பு sched_param lp = { .sched_priority = 0 };
	கட்டமைப்பு task_struct *p;
	முழு retval;

	எனில் (unlikely(!param || pid < 0))
		திரும்பு -EINVAL;

	scoped_guard (rcu) {
		p = find_process_by_pid(pid);
		எனில் (!p)
			திரும்பு -ESRCH;

		retval = security_task_getscheduler(p);
		எனில் (retval)
			திரும்பு retval;

		எனில் (task_has_rt_policy(p))
			lp.sched_priority = p->rt_priority;
	}

	/*
	 * This one might sleep, we cannot do it with a spinlock held ...
	 */
	திரும்பு copy_to_user(param, &lp, அளவு(*param)) ? -EFAULT : 0;
}

/**
 * sys_sched_getattr - similar to sched_getparam, but with sched_attr
 * @pid: the pid in question.
 * @uattr: structure containing the extended parameters.
 * @usize: sizeof(attr) for fwd/bwd comp.
 * @flags: for future extension.
 */
SYSCALL_DEFINE4(sched_getattr, pid_t, pid, கட்டமைப்பு sched_attr __user *, uattr,
		unsigned முழு, usize, unsigned முழு, flags)
{
	கட்டமைப்பு sched_attr kattr = { };
	கட்டமைப்பு task_struct *p;
	முழு retval;

	எனில் (unlikely(!uattr || pid < 0 || usize > PAGE_SIZE ||
		     usize < SCHED_ATTR_SIZE_VER0))
		திரும்பு -EINVAL;

	scoped_guard (rcu) {
		p = find_process_by_pid(pid);
		எனில் (!p)
			திரும்பு -ESRCH;

		எனில் (flags) {
			எனில் (!task_has_dl_policy(p) ||
			    flags != SCHED_GETATTR_FLAG_DL_DYNAMIC)
				திரும்பு -EINVAL;
		}

		retval = security_task_getscheduler(p);
		எனில் (retval)
			திரும்பு retval;

		kattr.sched_policy = p->policy;
		எனில் (p->sched_reset_on_fork)
			kattr.sched_flags |= SCHED_FLAG_RESET_ON_FORK;
		get_params(p, &kattr, flags);
		kattr.sched_flags &= SCHED_FLAG_ALL;

#ifdef CONFIG_UCLAMP_TASK
		/*
		 * This could race with another potential updater, but this is fine
		 * because it'll correctly read the old or the new value. We don't need
		 * to guarantee who wins the race as long as it doesn't return garbage.
		 */
		kattr.sched_util_min = p->uclamp_req[UCLAMP_MIN].மதிப்பு;
		kattr.sched_util_max = p->uclamp_req[UCLAMP_MAX].மதிப்பு;
#endif
	}

	kattr.size = min(usize, அளவு(kattr));
	திரும்பு copy_struct_to_user(uattr, usize, &kattr, அளவு(kattr), வெற்று);
}

முழு dl_task_check_affinity(கட்டமைப்பு task_struct *p, மாறிலி கட்டமைப்பு cpumask *mask)
{
	/*
	 * If the task isn't a deadline task or admission control is
	 * disabled then we don't care about affinity changes.
	 */
	எனில் (!task_has_dl_policy(p) || !dl_bandwidth_enabled())
		திரும்பு 0;

	/*
	 * The special/sugov task isn't part of regular bandwidth/admission
	 * control so let userspace change affinities.
	 */
	எனில் (dl_entity_is_special(&p->dl))
		திரும்பு 0;

	/*
	 * Since bandwidth control happens on root_domain basis,
	 * if admission test is enabled, we only admit -deadline
	 * tasks allowed to run on all the CPUs in the task's
	 * root_domain.
	 */
	guard(rcu)();
	எனில் (!cpumask_subset(task_rq(p)->rd->span, mask))
		திரும்பு -EBUSY;

	திரும்பு 0;
}

முழு __sched_setaffinity(கட்டமைப்பு task_struct *p, கட்டமைப்பு affinity_context *ctx)
{
	முழு retval;
	cpumask_var_t cpus_allowed, new_mask;

	எனில் (!alloc_cpumask_var(&cpus_allowed, GFP_KERNEL))
		திரும்பு -ENOMEM;

	எனில் (!alloc_cpumask_var(&new_mask, GFP_KERNEL)) {
		retval = -ENOMEM;
		செல் out_free_cpus_allowed;
	}

	cpuset_cpus_allowed(p, cpus_allowed);
	cpumask_and(new_mask, ctx->new_mask, cpus_allowed);

	ctx->new_mask = new_mask;
	ctx->flags |= SCA_CHECK;

	retval = dl_task_check_affinity(p, new_mask);
	எனில் (retval)
		செல் out_free_new_mask;

	retval = __set_cpus_allowed_ptr(p, ctx);
	எனில் (retval)
		செல் out_free_new_mask;

	cpuset_cpus_allowed(p, cpus_allowed);
	எனில் (!cpumask_subset(new_mask, cpus_allowed)) {
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
		எனில் (unlikely((ctx->flags & SCA_USER) && ctx->user_mask)) {
			bool empty = !cpumask_and(new_mask, new_mask,
						  ctx->user_mask);

			எனில் (empty)
				cpumask_copy(new_mask, cpus_allowed);
		}
		__set_cpus_allowed_ptr(p, ctx);
		retval = -EINVAL;
	}

out_free_new_mask:
	free_cpumask_var(new_mask);
out_free_cpus_allowed:
	free_cpumask_var(cpus_allowed);
	திரும்பு retval;
}

long sched_setaffinity(pid_t pid, மாறிலி கட்டமைப்பு cpumask *in_mask)
{
	கட்டமைப்பு affinity_context ac;
	கட்டமைப்பு cpumask *user_mask;
	முழு retval;

	CLASS(find_get_task, p)(pid);
	எனில் (!p)
		திரும்பு -ESRCH;

	எனில் (p->flags & PF_NO_SETAFFINITY)
		திரும்பு -EINVAL;

	எனில் (!check_same_owner(p)) {
		guard(rcu)();
		எனில் (!ns_capable(__task_cred(p)->user_ns, CAP_SYS_NICE))
			திரும்பு -EPERM;
	}

	retval = security_task_setscheduler(p);
	எனில் (retval)
		திரும்பு retval;

	/*
	 * With non-SMP configs, user_cpus_ptr/user_mask isn't used and
	 * alloc_user_cpus_ptr() returns NULL.
	 */
	user_mask = alloc_user_cpus_ptr(NUMA_NO_முனை);
	எனில் (user_mask) {
		cpumask_copy(user_mask, in_mask);
	} இல்லையெனில் {
		திரும்பு -ENOMEM;
	}

	ac = (கட்டமைப்பு affinity_context){
		.new_mask  = in_mask,
		.user_mask = user_mask,
		.flags     = SCA_USER,
	};

	retval = __sched_setaffinity(p, &ac);
	kfree(ac.user_mask);

	திரும்பு retval;
}

நிலையான முழு get_user_cpu_mask(unsigned long __user *user_mask_ptr, unsigned len,
			     கட்டமைப்பு cpumask *new_mask)
{
	எனில் (len < cpumask_size())
		cpumask_clear(new_mask);
	இல்லையெனில் எனில் (len > cpumask_size())
		len = cpumask_size();

	திரும்பு copy_from_user(new_mask, user_mask_ptr, len) ? -EFAULT : 0;
}

/**
 * sys_sched_setaffinity - set the CPU affinity of a process
 * @pid: pid of the process
 * @len: length in bytes of the bitmask pointed to by user_mask_ptr
 * @user_mask_ptr: user-space pointer to the new CPU mask
 *
 * Return: 0 on success. An error code otherwise.
 */
SYSCALL_DEFINE3(sched_setaffinity, pid_t, pid, unsigned முழு, len,
		unsigned long __user *, user_mask_ptr)
{
	cpumask_var_t new_mask;
	முழு retval;

	எனில் (!alloc_cpumask_var(&new_mask, GFP_KERNEL))
		திரும்பு -ENOMEM;

	retval = get_user_cpu_mask(user_mask_ptr, len, new_mask);
	எனில் (retval == 0)
		retval = sched_setaffinity(pid, new_mask);
	free_cpumask_var(new_mask);
	திரும்பு retval;
}

long sched_getaffinity(pid_t pid, கட்டமைப்பு cpumask *mask)
{
	கட்டமைப்பு task_struct *p;
	முழு retval;

	guard(rcu)();
	p = find_process_by_pid(pid);
	எனில் (!p)
		திரும்பு -ESRCH;

	retval = security_task_getscheduler(p);
	எனில் (retval)
		திரும்பு retval;

	guard(raw_spinlock_irqsave)(&p->pi_lock);
	cpumask_and(mask, &p->cpus_mask, cpu_active_mask);

	திரும்பு 0;
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
SYSCALL_DEFINE3(sched_getaffinity, pid_t, pid, unsigned முழு, len,
		unsigned long __user *, user_mask_ptr)
{
	முழு ret;
	cpumask_var_t mask;

	எனில் ((len * BITS_PER_BYTE) < nr_cpu_ids)
		திரும்பு -EINVAL;
	எனில் (len & (அளவு(unsigned long)-1))
		திரும்பு -EINVAL;

	எனில் (!zalloc_cpumask_var(&mask, GFP_KERNEL))
		திரும்பு -ENOMEM;

	ret = sched_getaffinity(pid, mask);
	எனில் (ret == 0) {
		unsigned முழு retlen = min(len, cpumask_size());

		எனில் (copy_to_user(user_mask_ptr, cpumask_bits(mask), retlen))
			ret = -EFAULT;
		இல்லையெனில்
			ret = retlen;
	}
	free_cpumask_var(mask);

	திரும்பு ret;
}

நிலையான வெறுமை do_sched_yield(வெறுமை)
{
	கட்டமைப்பு rq_flags rf;
	கட்டமைப்பு rq *rq;

	rq = this_rq_lock_irq(&rf);

	schedstat_inc(rq->yld_மொத்தம்);
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
	திரும்பு 0;
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
வெறுமை __sched yield(வெறுமை)
{
	set_current_state(TASK_RUNNING);
	do_sched_yield();
}
EXPORT_குறியீடு(yield);

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
முழு __sched yield_to(கட்டமைப்பு task_struct *p, bool preempt)
{
	கட்டமைப்பு task_struct *curr;
	கட்டமைப்பு rq *rq, *p_rq;
	முழு yielded = 0;

	scoped_guard (raw_spinlock_irqsave, &p->pi_lock) {
		rq = this_rq();
		curr = rq->donor;

again:
		p_rq = task_rq(p);
		/*
		 * If we're the only runnable task on the rq and target rq also
		 * has only one task, there's absolutely no point in yielding.
		 */
		எனில் (rq->nr_running == 1 && p_rq->nr_running == 1)
			திரும்பு -ESRCH;

		guard(double_rq_lock)(rq, p_rq);
		எனில் (task_rq(p) != p_rq)
			செல் again;

		எனில் (!curr->sched_class->yield_to_task)
			திரும்பு 0;

		எனில் (curr->sched_class != p->sched_class)
			திரும்பு 0;

		எனில் (task_on_cpu(p_rq, p) || !task_is_running(p))
			திரும்பு 0;

		yielded = curr->sched_class->yield_to_task(rq, p);
		எனில் (yielded) {
			schedstat_inc(rq->yld_மொத்தம்);
			/*
			 * Make p's CPU reschedule; pick_next_entity
			 * takes care of fairness.
			 */
			எனில் (preempt && rq != p_rq)
				resched_curr(p_rq);
		}
	}

	எனில் (yielded)
		schedule();

	திரும்பு yielded;
}
EXPORT_குறியீடு_GPL(yield_to);

/**
 * sys_sched_get_priority_max - return maximum RT priority.
 * @policy: scheduling class.
 *
 * Return: On success, this syscall returns the maximum
 * rt_priority that can be used by a given scheduling class.
 * On failure, a negative error code is returned.
 */
SYSCALL_DEFINE1(sched_get_priority_max, முழு, policy)
{
	முழு ret = -EINVAL;

	தேர்வு (policy) {
	நிலை SCHED_FIFO:
	நிலை SCHED_RR:
		ret = MAX_RT_PRIO-1;
		நிறுத்து;
	நிலை SCHED_DEADLINE:
	நிலை SCHED_NORMAL:
	நிலை SCHED_BATCH:
	நிலை SCHED_IDLE:
	நிலை SCHED_EXT:
		ret = 0;
		நிறுத்து;
	}
	திரும்பு ret;
}

/**
 * sys_sched_get_priority_min - return minimum RT priority.
 * @policy: scheduling class.
 *
 * Return: On success, this syscall returns the minimum
 * rt_priority that can be used by a given scheduling class.
 * On failure, a negative error code is returned.
 */
SYSCALL_DEFINE1(sched_get_priority_min, முழு, policy)
{
	முழு ret = -EINVAL;

	தேர்வு (policy) {
	நிலை SCHED_FIFO:
	நிலை SCHED_RR:
		ret = 1;
		நிறுத்து;
	நிலை SCHED_DEADLINE:
	நிலை SCHED_NORMAL:
	நிலை SCHED_BATCH:
	நிலை SCHED_IDLE:
	நிலை SCHED_EXT:
		ret = 0;
	}
	திரும்பு ret;
}

நிலையான முழு sched_rr_get_interval(pid_t pid, கட்டமைப்பு timespec64 *t)
{
	unsigned முழு time_slice = 0;
	முழு retval;

	எனில் (pid < 0)
		திரும்பு -EINVAL;

	scoped_guard (rcu) {
		கட்டமைப்பு task_struct *p = find_process_by_pid(pid);
		எனில் (!p)
			திரும்பு -ESRCH;

		retval = security_task_getscheduler(p);
		எனில் (retval)
			திரும்பு retval;

		scoped_guard (task_rq_lock, p) {
			கட்டமைப்பு rq *rq = scope.rq;
			எனில் (p->sched_class->get_rr_interval)
				time_slice = p->sched_class->get_rr_interval(rq, p);
		}
	}

	jiffies_to_timespec64(time_slice, t);
	திரும்பு 0;
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
		கட்டமைப்பு __kernel_timespec __user *, interval)
{
	கட்டமைப்பு timespec64 t;
	முழு retval = sched_rr_get_interval(pid, &t);

	எனில் (retval == 0)
		retval = put_timespec64(&t, interval);

	திரும்பு retval;
}

#ifdef CONFIG_COMPAT_32BIT_TIME
SYSCALL_DEFINE2(sched_rr_get_interval_time32, pid_t, pid,
		கட்டமைப்பு old_timespec32 __user *, interval)
{
	கட்டமைப்பு timespec64 t;
	முழு retval = sched_rr_get_interval(pid, &t);

	எனில் (retval == 0)
		retval = put_old_timespec32(&t, interval);
	திரும்பு retval;
}
#endif
