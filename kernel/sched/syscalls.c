// SPDX-License-Identifier: GPL-2.0-only
/*
 *  kernel/sched/syscalls.c
 *
 *  Core kernel scheduler syscalls related code
 *
 *  Copyright (C) 1991-2002  Linus Torvalds
 *  Copyright (C) 1998-2024  Ingo Molnar, Red Hat
 */
#شامل <linux/sched.h>
#شامل <linux/cpuset.h>
#شامل <linux/sched/debug.h>

#شامل <uapi/linux/sched/types.h>

#شامل "sched.h"
#شامل "autogroup.h"

ایستا درونخطی صحیح __normal_prio(صحیح policy, صحیح rt_prio, صحیح nice)
{
	صحیح prio;

	اگر (dl_policy(policy))
		prio = MAX_DL_PRIO - 1;
	وگرنه اگر (rt_policy(policy))
		prio = MAX_RT_PRIO - 1 - rt_prio;
	وگرنه
		prio = NICE_TO_PRIO(nice);

	بازگشت prio;
}

/*
 * Calculate the expected normal priority: i.e. priority
 * without taking RT-inheritance into account. Might be
 * boosted by interactivity modifiers. Changes upon fork,
 * setprio syscalls, and whenever the interactivity
 * estimator recalculates.
 */
ایستا درونخطی صحیح normal_prio(ساختار task_struct *p)
{
	بازگشت __normal_prio(p->policy, p->rt_priority, PRIO_TO_NICE(p->static_prio));
}

/*
 * Calculate the current priority, i.e. the priority
 * taken into account by the scheduler. This value might
 * be boosted by RT tasks, or might be boosted by
 * interactivity modifiers. Will be RT if the task got
 * RT-boosted. If not then it returns p->normal_prio.
 */
ایستا صحیح effective_prio(ساختار task_struct *p)
{
	p->normal_prio = normal_prio(p);
	/*
	 * If we are RT tasks or we were boosted to RT priority,
	 * keep the priority unchanged. Otherwise, update priority
	 * to the normal priority:
	 */
	اگر (!rt_or_dl_prio(p->prio))
		بازگشت p->normal_prio;
	بازگشت p->prio;
}

تهی set_user_nice(ساختار task_struct *p, long nice)
{
	صحیح old_prio;

	اگر (task_nice(p) == nice || nice < MIN_NICE || nice > MAX_NICE)
		بازگشت;
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
	اگر (task_has_dl_policy(p) || task_has_rt_policy(p)) {
		p->static_prio = NICE_TO_PRIO(nice);
		بازگشت;
	}

	scoped_guard (sched_change, p, DEQUEUE_SAVE) {
		p->static_prio = NICE_TO_PRIO(nice);
		set_load_weight(p, true);
		old_prio = p->prio;
		p->prio = effective_prio(p);
	}
}
EXPORT_نماد(set_user_nice);

/*
 * is_nice_reduction - check if nice value is an actual reduction
 *
 * Similar to can_nice() but does not perform a capability check.
 *
 * @p: task
 * @nice: nice value
 */
ایستا bool is_nice_reduction(ثابت ساختار task_struct *p, ثابت صحیح nice)
{
	/* Convert nice value [19,-20] to rlimit style value [1,40]: */
	صحیح nice_rlim = nice_to_rlimit(nice);

	بازگشت (nice_rlim <= task_rlimit(p, RLIMIT_NICE));
}

/*
 * can_nice - check if a task can reduce its nice value
 * @p: task
 * @nice: nice value
 */
صحیح can_nice(ثابت ساختار task_struct *p, ثابت صحیح nice)
{
	بازگشت is_nice_reduction(p, nice) || capable(CAP_SYS_NICE);
}

#ifdef __ARCH_WANT_SYS_NICE

/*
 * sys_nice - change the priority of the current process.
 * @increment: priority increment
 *
 * sys_setpriority is a more generic, but much slower function that
 * does similar things.
 */
SYSCALL_DEFINE1(nice, صحیح, increment)
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
	اگر (increment < 0 && !can_nice(current, nice))
		بازگشت -EPERM;

	retval = security_task_setnice(current, nice);
	اگر (retval)
		بازگشت retval;

	set_user_nice(current, nice);
	بازگشت 0;
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
صحیح task_prio(ثابت ساختار task_struct *p)
{
	بازگشت p->prio - MAX_RT_PRIO;
}

/**
 * idle_cpu - is a given CPU idle currently?
 * @cpu: the processor in question.
 *
 * Return: 1 if the CPU is currently idle. 0 otherwise.
 */
صحیح idle_cpu(صحیح cpu)
{
	بازگشت idle_rq(cpu_rq(cpu));
}

/**
 * idle_task - return the idle task for a given CPU.
 * @cpu: the processor in question.
 *
 * Return: The idle task for the CPU @cpu.
 */
ساختار task_struct *idle_task(صحیح cpu)
{
	بازگشت cpu_rq(cpu)->idle;
}

#ifdef CONFIG_SCHED_CORE
صحیح sched_core_idle_cpu(صحیح cpu)
{
	ساختار rq *rq = cpu_rq(cpu);

	اگر (sched_core_enabled(rq) && rq->curr == rq->idle)
		بازگشت 1;

	بازگشت idle_cpu(cpu);
}
#endif /* CONFIG_SCHED_CORE */

/**
 * find_process_by_pid - find a process with a matching PID value.
 * @pid: the pid in question.
 *
 * The task of @pid, if found. %NULL otherwise.
 */
ایستا ساختار task_struct *find_process_by_pid(pid_t pid)
{
	بازگشت pid ? find_task_by_vpid(pid) : current;
}

ایستا ساختار task_struct *find_get_task(pid_t pid)
{
	ساختار task_struct *p;
	guard(rcu)();

	p = find_process_by_pid(pid);
	اگر (likely(p))
		get_task_struct(p);

	بازگشت p;
}

DEFINE_CLASS(find_get_task, ساختار task_struct *, اگر (_T) put_task_struct(_T),
	     find_get_task(pid), pid_t pid)

/*
 * sched_setparam() passes in -1 for its policy, to let the functions
 * it calls know not to change it.
 */
#define SETPARAM_POLICY	-1

ایستا تهی __setscheduler_params(ساختار task_struct *p,
		ثابت ساختار sched_attr *attr)
{
	صحیح policy = attr->sched_policy;

	اگر (policy == SETPARAM_POLICY)
		policy = p->policy;

	p->policy = policy;

	اگر (dl_policy(policy))
		__setparam_dl(p, attr);
	وگرنه اگر (fair_policy(policy))
		__setparam_fair(p, attr);

	/* rt-policy tasks do not have a timerslack */
	اگر (rt_or_dl_task_policy(p)) {
		p->timer_slack_ns = 0;
	} وگرنه اگر (p->timer_slack_ns == 0) {
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
ایستا bool check_same_owner(ساختار task_struct *p)
{
	ثابت ساختار cred *cred = current_cred(), *pcred;
	guard(rcu)();

	pcred = __task_cred(p);
	بازگشت (uid_eq(cred->euid, pcred->euid) ||
		uid_eq(cred->euid, pcred->uid));
}

#ifdef CONFIG_RT_MUTEXES
ایستا درونخطی تهی __setscheduler_dl_pi(صحیح newprio, صحیح policy,
			      ساختار task_struct *p,
			      ساختار sched_change_ctx *scope)
{
	/*
	 * In case a DEADLINE task (either proper or boosted) gets
	 * setscheduled to a lower priority class, check if it neeeds to
	 * inherit parameters from a potential pi_task. In that case make
	 * sure replenishment happens with the next enqueue.
	 */

	اگر (dl_prio(newprio) && !dl_policy(policy)) {
		ساختار task_struct *pi_task = rt_mutex_get_بالاترین_task(p);

		اگر (pi_task) {
			p->dl.pi_se = pi_task->dl.pi_se;
			scope->flags |= ENQUEUE_REPLENISH;
		}
	}
}
#وگرنه /* !CONFIG_RT_MUTEXES */
ایستا درونخطی تهی __setscheduler_dl_pi(صحیح newprio, صحیح policy,
			      ساختار task_struct *p,
			      ساختار sched_change_ctx *scope)
{
}
#endif /* !CONFIG_RT_MUTEXES */

#ifdef CONFIG_UCLAMP_TASK

ایستا صحیح uclamp_validate(ساختار task_struct *p,
			   ثابت ساختار sched_attr *attr)
{
	صحیح util_min = p->uclamp_req[UCLAMP_MIN].مقدار;
	صحیح util_max = p->uclamp_req[UCLAMP_MAX].مقدار;

	اگر (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MIN) {
		util_min = attr->sched_util_min;

		اگر (util_min + 1 > SCHED_CAPACITY_SCALE + 1)
			بازگشت -EINVAL;
	}

	اگر (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MAX) {
		util_max = attr->sched_util_max;

		اگر (util_max + 1 > SCHED_CAPACITY_SCALE + 1)
			بازگشت -EINVAL;
	}

	اگر (util_min != -1 && util_max != -1 && util_min > util_max)
		بازگشت -EINVAL;

	/*
	 * We have valid uclamp attributes; make sure uclamp is enabled.
	 *
	 * We need to do that here, because enabling static branches is a
	 * blocking operation which obviously cannot be done while holding
	 * scheduler locks.
	 */
	sched_uclamp_enable();

	بازگشت 0;
}

ایستا bool uclamp_reset(ثابت ساختار sched_attr *attr,
			 شمارش uclamp_id clamp_id,
			 ساختار uclamp_se *uc_se)
{
	/* Reset on sched class change for a non user-defined clamp value. */
	اگر (likely(!(attr->sched_flags & SCHED_FLAG_UTIL_CLAMP)) &&
	    !uc_se->user_defined)
		بازگشت true;

	/* Reset on sched_util_{min,max} == -1. */
	اگر (clamp_id == UCLAMP_MIN &&
	    attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MIN &&
	    attr->sched_util_min == -1) {
		بازگشت true;
	}

	اگر (clamp_id == UCLAMP_MAX &&
	    attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MAX &&
	    attr->sched_util_max == -1) {
		بازگشت true;
	}

	بازگشت false;
}

ایستا تهی __setscheduler_uclamp(ساختار task_struct *p,
				  ثابت ساختار sched_attr *attr)
{
	شمارش uclamp_id clamp_id;

	for_each_clamp_id(clamp_id) {
		ساختار uclamp_se *uc_se = &p->uclamp_req[clamp_id];
		unsigned صحیح مقدار;

		اگر (!uclamp_reset(attr, clamp_id, uc_se))
			ادامه;

		/*
		 * RT by default have a 100% boost value that could be modified
		 * at runtime.
		 */
		اگر (unlikely(rt_task(p) && clamp_id == UCLAMP_MIN))
			مقدار = sysctl_sched_uclamp_util_min_rt_default;
		وگرنه
			مقدار = uclamp_none(clamp_id);

		uclamp_se_set(uc_se, مقدار, false);

	}

	اگر (likely(!(attr->sched_flags & SCHED_FLAG_UTIL_CLAMP)))
		بازگشت;

	اگر (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MIN &&
	    attr->sched_util_min != -1) {
		uclamp_se_set(&p->uclamp_req[UCLAMP_MIN],
			      attr->sched_util_min, true);
	}

	اگر (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MAX &&
	    attr->sched_util_max != -1) {
		uclamp_se_set(&p->uclamp_req[UCLAMP_MAX],
			      attr->sched_util_max, true);
	}
}

#وگرنه /* !CONFIG_UCLAMP_TASK: */

ایستا درونخطی صحیح uclamp_validate(ساختار task_struct *p,
				  ثابت ساختار sched_attr *attr)
{
	بازگشت -EOPNOTSUPP;
}
ایستا تهی __setscheduler_uclamp(ساختار task_struct *p,
				  ثابت ساختار sched_attr *attr) { }
#endif /* !CONFIG_UCLAMP_TASK */

/*
 * Allow unprivileged RT tasks to decrease priority.
 * Only issue a capable test if needed and only once to avoid an audit
 * event on permitted non-privileged operations:
 */
ایستا صحیح user_check_sched_setscheduler(ساختار task_struct *p,
					 ثابت ساختار sched_attr *attr,
					 صحیح policy, صحیح reset_on_fork)
{
	اگر (fair_policy(policy)) {
		اگر (attr->sched_nice < task_nice(p) &&
		    !is_nice_reduction(p, attr->sched_nice))
			برو req_priv;
	}

	اگر (rt_policy(policy)) {
		unsigned long rlim_rtprio = task_rlimit(p, RLIMIT_RTPRIO);

		/* Can't set/change the rt policy: */
		اگر (policy != p->policy && !rlim_rtprio)
			برو req_priv;

		/* Can't increase priority: */
		اگر (attr->sched_priority > p->rt_priority &&
		    attr->sched_priority > rlim_rtprio)
			برو req_priv;
	}

	/*
	 * Can't set/change SCHED_DEADLINE policy at all for now
	 * (safest behavior); in the future we would like to allow
	 * unprivileged DL tasks to increase their relative deadline
	 * or reduce their runtime (both ways reducing utilization)
	 */
	اگر (dl_policy(policy))
		برو req_priv;

	/*
	 * Treat SCHED_IDLE as nice 20. Only allow a switch to
	 * SCHED_NORMAL if the RLIMIT_NICE would normally permit it.
	 */
	اگر (task_has_idle_policy(p) && !idle_policy(policy)) {
		اگر (!is_nice_reduction(p, task_nice(p)))
			برو req_priv;
	}

	/* Can't change other user's priorities: */
	اگر (!check_same_owner(p))
		برو req_priv;

	/* Normal users shall not reset the sched_reset_on_fork flag: */
	اگر (p->sched_reset_on_fork && !reset_on_fork)
		برو req_priv;

	بازگشت 0;

req_priv:
	اگر (!capable(CAP_SYS_NICE))
		بازگشت -EPERM;

	بازگشت 0;
}

صحیح __sched_setscheduler(ساختار task_struct *p,
			 ثابت ساختار sched_attr *attr,
			 bool user, bool pi)
{
	صحیح oldpolicy = -1, policy = attr->sched_policy;
	صحیح retval, oldprio, newprio;
	ثابت ساختار sched_class *prev_class, *next_class;
	ساختار balance_callback *head;
	ساختار rq_flags rf;
	صحیح reset_on_fork;
	صحیح queue_flags = DEQUEUE_SAVE | DEQUEUE_MOVE | DEQUEUE_NOCLOCK;
	ساختار rq *rq;
	bool cpuset_locked = false;

	/* The pi code expects interrupts enabled */
	BUG_ON(pi && in_interrupt());
recheck:
	/* Double check policy once rq lock held: */
	اگر (policy < 0) {
		reset_on_fork = p->sched_reset_on_fork;
		policy = oldpolicy = p->policy;
	} وگرنه {
		reset_on_fork = !!(attr->sched_flags & SCHED_FLAG_RESET_ON_FORK);

		اگر (!valid_policy(policy))
			بازگشت -EINVAL;
	}

	اگر (attr->sched_flags & ~(SCHED_FLAG_ALL | SCHED_FLAG_SUGOV))
		بازگشت -EINVAL;

	/*
	 * Valid priorities for SCHED_FIFO and SCHED_RR are
	 * 1..MAX_RT_PRIO-1, valid priority for SCHED_NORMAL,
	 * SCHED_BATCH and SCHED_IDLE is 0.
	 */
	اگر (attr->sched_priority > MAX_RT_PRIO-1)
		بازگشت -EINVAL;
	اگر ((dl_policy(policy) && !__checkparam_dl(attr)) ||
	    (rt_policy(policy) != (attr->sched_priority != 0)))
		بازگشت -EINVAL;

	اگر (user) {
		retval = user_check_sched_setscheduler(p, attr, policy, reset_on_fork);
		اگر (retval)
			بازگشت retval;

		اگر (attr->sched_flags & SCHED_FLAG_SUGOV)
			بازگشت -EINVAL;

		retval = security_task_setscheduler(p);
		اگر (retval)
			بازگشت retval;
	}

	/* Update task specific "requested" clamps */
	اگر (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP) {
		retval = uclamp_validate(p, attr);
		اگر (retval)
			بازگشت retval;
	}

	/*
	 * SCHED_DEADLINE bandwidth accounting relies on stable cpusets
	 * information.
	 */
	اگر (dl_policy(policy) || dl_policy(p->policy)) {
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
	اگر (p == rq->stop) {
		retval = -EINVAL;
		برو unlock;
	}

	retval = scx_check_setscheduler(p, policy);
	اگر (retval)
		برو unlock;

	/*
	 * If not changing anything there's no need to proceed further,
	 * but store a possible modification of reset_on_fork.
	 */
	اگر (unlikely(policy == p->policy)) {
		اگر (fair_policy(policy) &&
		    (attr->sched_nice != task_nice(p) ||
		     (attr->sched_runtime != p->se.slice)))
			برو change;
		اگر (rt_policy(policy) && attr->sched_priority != p->rt_priority)
			برو change;
		اگر (dl_policy(policy) && dl_param_changed(p, attr))
			برو change;
		اگر (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP)
			برو change;

		p->sched_reset_on_fork = reset_on_fork;
		retval = 0;
		برو unlock;
	}
change:

	اگر (user) {
#ifdef CONFIG_RT_GROUP_SCHED
		/*
		 * Do not allow real-time tasks into groups that have no runtime
		 * assigned.
		 */
		اگر (rt_group_sched_enabled() &&
				rt_bandwidth_enabled() && rt_policy(policy) &&
				task_group(p)->rt_bandwidth.rt_runtime == 0 &&
				!task_group_is_autogroup(task_group(p))) {
			retval = -EPERM;
			برو unlock;
		}
#endif /* CONFIG_RT_GROUP_SCHED */
		اگر (dl_bandwidth_enabled() && dl_policy(policy) &&
				!(attr->sched_flags & SCHED_FLAG_SUGOV)) {
			cpumask_t *span = rq->rd->span;

			/*
			 * Don't allow tasks with an affinity mask smaller than
			 * the entire root_domain to become SCHED_DEADLINE. We
			 * will also fail if there's no bandwidth available.
			 */
			اگر (!cpumask_subset(span, p->cpus_ptr) ||
			    rq->rd->dl_bw.bw == 0) {
				retval = -EPERM;
				برو unlock;
			}
		}
	}

	/* Re-check policy now with rq lock held: */
	اگر (unlikely(oldpolicy != -1 && oldpolicy != p->policy)) {
		policy = oldpolicy = -1;
		task_rq_unlock(rq, p, &rf);
		اگر (cpuset_locked)
			cpuset_unlock();
		برو recheck;
	}

	/*
	 * If setscheduling to SCHED_DEADLINE (or changing the parameters
	 * of a SCHED_DEADLINE task) we need to check if enough bandwidth
	 * is available.
	 */
	اگر ((dl_policy(policy) || dl_task(p)) && sched_dl_overflow(p, policy, attr)) {
		retval = -EBUSY;
		برو unlock;
	}

	p->sched_reset_on_fork = reset_on_fork;
	oldprio = p->prio;

	newprio = __normal_prio(policy, attr->sched_priority, attr->sched_nice);
	اگر (pi) {
		/*
		 * Take priority boosted tasks into account. If the new
		 * effective priority is unchanged, we just store the new
		 * normal parameters and do not touch the scheduler class and
		 * the runqueue. This will be done when the task deboost
		 * itself.
		 */
		newprio = rt_effective_prio(p, newprio);
		اگر (newprio == oldprio && !dl_prio(newprio))
			queue_flags &= ~DEQUEUE_MOVE;
	}

	prev_class = p->sched_class;
	next_class = __setscheduler_class(policy, newprio);

	اگر (prev_class != next_class)
		queue_flags |= DEQUEUE_CLASS;

	scoped_guard (sched_change, p, queue_flags) {

		اگر (!(attr->sched_flags & SCHED_FLAG_KEEP_PARAMS)) {
			__setscheduler_params(p, attr);
			p->sched_class = next_class;
			p->prio = newprio;
			__setscheduler_dl_pi(newprio, policy, p, scope);
		}
		__setscheduler_uclamp(p, attr);

		اگر (scope->queued) {
			/*
			 * We enqueue to tail when the priority of a task is
			 * increased (user space view).
			 */
			اگر (oldprio < p->prio)
				scope->flags |= ENQUEUE_HEAD;
		}
	}

	/* Avoid rq from going away on us: */
	preempt_disable();
	head = splice_balance_callbacks(rq);
	task_rq_unlock(rq, p, &rf);

	اگر (pi) {
		اگر (cpuset_locked)
			cpuset_unlock();
		rt_mutex_adjust_pi(p);
	}

	/* Run balance callbacks after we've adjusted the PI chain: */
	balance_callbacks(rq, head);
	preempt_enable();

	بازگشت 0;

unlock:
	task_rq_unlock(rq, p, &rf);
	اگر (cpuset_locked)
		cpuset_unlock();
	بازگشت retval;
}

ایستا صحیح _sched_setscheduler(ساختار task_struct *p, صحیح policy,
			       ثابت ساختار sched_param *param, bool check)
{
	ساختار sched_attr attr = {
		.sched_policy   = policy,
		.sched_priority = param->sched_priority,
		.sched_nice	= PRIO_TO_NICE(p->static_prio),
	};

	اگر (p->se.custom_slice)
		attr.sched_runtime = p->se.slice;

	/* Fixup the legacy SCHED_RESET_ON_FORK hack. */
	اگر ((policy != SETPARAM_POLICY) && (policy & SCHED_RESET_ON_FORK)) {
		attr.sched_flags |= SCHED_FLAG_RESET_ON_FORK;
		policy &= ~SCHED_RESET_ON_FORK;
		attr.sched_policy = policy;
	}

	بازگشت __sched_setscheduler(p, &attr, check, true);
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
صحیح sched_setscheduler(ساختار task_struct *p, صحیح policy,
		       ثابت ساختار sched_param *param)
{
	بازگشت _sched_setscheduler(p, policy, param, true);
}

صحیح sched_setattr(ساختار task_struct *p, ثابت ساختار sched_attr *attr)
{
	بازگشت __sched_setscheduler(p, attr, true, true);
}

صحیح sched_setattr_nocheck(ساختار task_struct *p, ثابت ساختار sched_attr *attr)
{
	بازگشت __sched_setscheduler(p, attr, false, true);
}
EXPORT_نماد_GPL(sched_setattr_nocheck);

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
صحیح sched_setscheduler_nocheck(ساختار task_struct *p, صحیح policy,
			       ثابت ساختار sched_param *param)
{
	بازگشت _sched_setscheduler(p, policy, param, false);
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
تهی sched_set_fifo(ساختار task_struct *p)
{
	ساختار sched_param sp = { .sched_priority = MAX_RT_PRIO / 2 };
	WARN_ON_ONCE(sched_setscheduler_nocheck(p, SCHED_FIFO, &sp) != 0);
}
EXPORT_نماد_GPL(sched_set_fifo);

/*
 * For when you don't much care about FIFO, but want to be above SCHED_NORMAL.
 */
تهی sched_set_fifo_کم(ساختار task_struct *p)
{
	ساختار sched_param sp = { .sched_priority = 1 };
	WARN_ON_ONCE(sched_setscheduler_nocheck(p, SCHED_FIFO, &sp) != 0);
}
EXPORT_نماد_GPL(sched_set_fifo_کم);

/*
 * Used when the primary interrupt handler is forced into a thread, in addition
 * to the (always threaded) secondary handler.  The secondary handler gets a
 * slightly lower priority so that the primary handler can preempt it, thereby
 * emulating the behavior of a non-PREEMPT_RT system where the primary handler
 * runs in hard interrupt context.
 */
تهی sched_set_fifo_secondary(ساختار task_struct *p)
{
	ساختار sched_param sp = { .sched_priority = MAX_RT_PRIO / 2 - 1 };
	WARN_ON_ONCE(sched_setscheduler_nocheck(p, SCHED_FIFO, &sp) != 0);
}

تهی sched_set_normal(ساختار task_struct *p, صحیح nice)
{
	ساختار sched_attr attr = {
		.sched_policy = SCHED_NORMAL,
		.sched_nice = nice,
	};
	WARN_ON_ONCE(sched_setattr_nocheck(p, &attr) != 0);
}
EXPORT_نماد_GPL(sched_set_normal);

ایستا صحیح
do_sched_setscheduler(pid_t pid, صحیح policy, ساختار sched_param __user *param)
{
	ساختار sched_param lparam;

	اگر (unlikely(!param || pid < 0))
		بازگشت -EINVAL;
	اگر (copy_from_user(&lparam, param, اندازه(ساختار sched_param)))
		بازگشت -EFAULT;

	CLASS(find_get_task, p)(pid);
	اگر (!p)
		بازگشت -ESRCH;

	بازگشت sched_setscheduler(p, policy, &lparam);
}

/*
 * Mimics kernel/events/core.c perf_copy_attr().
 */
ایستا صحیح sched_copy_attr(ساختار sched_attr __user *uattr, ساختار sched_attr *attr)
{
	u32 size;
	صحیح ret;

	/* Zero the full structure, so that a short copy will be nice: */
	memset(attr, 0, اندازه(*attr));

	ret = get_user(size, &uattr->size);
	اگر (ret)
		بازگشت ret;

	/* ABI compatibility quirk: */
	اگر (!size)
		size = SCHED_ATTR_SIZE_VER0;
	اگر (size < SCHED_ATTR_SIZE_VER0 || size > PAGE_SIZE)
		برو err_size;

	ret = copy_struct_from_user(attr, اندازه(*attr), uattr, size);
	اگر (ret) {
		اگر (ret == -E2BIG)
			برو err_size;
		بازگشت ret;
	}

	اگر ((attr->sched_flags & SCHED_FLAG_UTIL_CLAMP) &&
	    size < SCHED_ATTR_SIZE_VER1)
		بازگشت -EINVAL;

	/*
	 * XXX: Do we want to be lenient like existing syscalls; or do we want
	 * to be strict and return an error on out-of-bounds values?
	 */
	attr->sched_nice = clamp(attr->sched_nice, MIN_NICE, MAX_NICE);

	بازگشت 0;

err_size:
	put_user(اندازه(*attr), &uattr->size);
	بازگشت -E2BIG;
}

ایستا تهی get_params(ساختار task_struct *p, ساختار sched_attr *attr, unsigned صحیح flags)
{
	اگر (task_has_dl_policy(p)) {
		__getparam_dl(p, attr, flags);
	} وگرنه اگر (task_has_rt_policy(p)) {
		attr->sched_priority = p->rt_priority;
	} وگرنه {
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
SYSCALL_DEFINE3(sched_setscheduler, pid_t, pid, صحیح, policy, ساختار sched_param __user *, param)
{
	اگر (policy < 0)
		بازگشت -EINVAL;

	بازگشت do_sched_setscheduler(pid, policy, param);
}

/**
 * sys_sched_setparam - set/change the RT priority of a thread
 * @pid: the pid in question.
 * @param: structure containing the new RT priority.
 *
 * Return: 0 on success. An error code otherwise.
 */
SYSCALL_DEFINE2(sched_setparam, pid_t, pid, ساختار sched_param __user *, param)
{
	بازگشت do_sched_setscheduler(pid, SETPARAM_POLICY, param);
}

/**
 * sys_sched_setattr - same as above, but with extended sched_attr
 * @pid: the pid in question.
 * @uattr: structure containing the extended parameters.
 * @flags: for future extension.
 */
SYSCALL_DEFINE3(sched_setattr, pid_t, pid, ساختار sched_attr __user *, uattr,
			       unsigned صحیح, flags)
{
	ساختار sched_attr attr;
	صحیح retval;

	اگر (unlikely(!uattr || pid < 0 || flags))
		بازگشت -EINVAL;

	retval = sched_copy_attr(uattr, &attr);
	اگر (retval)
		بازگشت retval;

	اگر ((صحیح)attr.sched_policy < 0)
		بازگشت -EINVAL;
	اگر (attr.sched_flags & SCHED_FLAG_KEEP_POLICY)
		attr.sched_policy = SETPARAM_POLICY;

	CLASS(find_get_task, p)(pid);
	اگر (!p)
		بازگشت -ESRCH;

	اگر (attr.sched_flags & SCHED_FLAG_KEEP_PARAMS)
		get_params(p, &attr, 0);

	بازگشت sched_setattr(p, &attr);
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
	ساختار task_struct *p;
	صحیح retval;

	اگر (pid < 0)
		بازگشت -EINVAL;

	guard(rcu)();
	p = find_process_by_pid(pid);
	اگر (!p)
		بازگشت -ESRCH;

	retval = security_task_getscheduler(p);
	اگر (!retval) {
		retval = p->policy;
		اگر (p->sched_reset_on_fork)
			retval |= SCHED_RESET_ON_FORK;
	}
	بازگشت retval;
}

/**
 * sys_sched_getparam - get the RT priority of a thread
 * @pid: the pid in question.
 * @param: structure containing the RT priority.
 *
 * Return: On success, 0 and the RT priority is in @param. Otherwise, an error
 * code.
 */
SYSCALL_DEFINE2(sched_getparam, pid_t, pid, ساختار sched_param __user *, param)
{
	ساختار sched_param lp = { .sched_priority = 0 };
	ساختار task_struct *p;
	صحیح retval;

	اگر (unlikely(!param || pid < 0))
		بازگشت -EINVAL;

	scoped_guard (rcu) {
		p = find_process_by_pid(pid);
		اگر (!p)
			بازگشت -ESRCH;

		retval = security_task_getscheduler(p);
		اگر (retval)
			بازگشت retval;

		اگر (task_has_rt_policy(p))
			lp.sched_priority = p->rt_priority;
	}

	/*
	 * This one might sleep, we cannot do it with a spinlock held ...
	 */
	بازگشت copy_to_user(param, &lp, اندازه(*param)) ? -EFAULT : 0;
}

/**
 * sys_sched_getattr - similar to sched_getparam, but with sched_attr
 * @pid: the pid in question.
 * @uattr: structure containing the extended parameters.
 * @usize: sizeof(attr) for fwd/bwd comp.
 * @flags: for future extension.
 */
SYSCALL_DEFINE4(sched_getattr, pid_t, pid, ساختار sched_attr __user *, uattr,
		unsigned صحیح, usize, unsigned صحیح, flags)
{
	ساختار sched_attr kattr = { };
	ساختار task_struct *p;
	صحیح retval;

	اگر (unlikely(!uattr || pid < 0 || usize > PAGE_SIZE ||
		     usize < SCHED_ATTR_SIZE_VER0))
		بازگشت -EINVAL;

	scoped_guard (rcu) {
		p = find_process_by_pid(pid);
		اگر (!p)
			بازگشت -ESRCH;

		اگر (flags) {
			اگر (!task_has_dl_policy(p) ||
			    flags != SCHED_GETATTR_FLAG_DL_DYNAMIC)
				بازگشت -EINVAL;
		}

		retval = security_task_getscheduler(p);
		اگر (retval)
			بازگشت retval;

		kattr.sched_policy = p->policy;
		اگر (p->sched_reset_on_fork)
			kattr.sched_flags |= SCHED_FLAG_RESET_ON_FORK;
		get_params(p, &kattr, flags);
		kattr.sched_flags &= SCHED_FLAG_ALL;

#ifdef CONFIG_UCLAMP_TASK
		/*
		 * This could race with another potential updater, but this is fine
		 * because it'll correctly read the old or the new value. We don't need
		 * to guarantee who wins the race as long as it doesn't return garbage.
		 */
		kattr.sched_util_min = p->uclamp_req[UCLAMP_MIN].مقدار;
		kattr.sched_util_max = p->uclamp_req[UCLAMP_MAX].مقدار;
#endif
	}

	kattr.size = min(usize, اندازه(kattr));
	بازگشت copy_struct_to_user(uattr, usize, &kattr, اندازه(kattr), پوچ);
}

صحیح dl_task_check_affinity(ساختار task_struct *p, ثابت ساختار cpumask *mask)
{
	/*
	 * If the task isn't a deadline task or admission control is
	 * disabled then we don't care about affinity changes.
	 */
	اگر (!task_has_dl_policy(p) || !dl_bandwidth_enabled())
		بازگشت 0;

	/*
	 * The special/sugov task isn't part of regular bandwidth/admission
	 * control so let userspace change affinities.
	 */
	اگر (dl_entity_is_special(&p->dl))
		بازگشت 0;

	/*
	 * Since bandwidth control happens on root_domain basis,
	 * if admission test is enabled, we only admit -deadline
	 * tasks allowed to run on all the CPUs in the task's
	 * root_domain.
	 */
	guard(rcu)();
	اگر (!cpumask_subset(task_rq(p)->rd->span, mask))
		بازگشت -EBUSY;

	بازگشت 0;
}

صحیح __sched_setaffinity(ساختار task_struct *p, ساختار affinity_context *ctx)
{
	صحیح retval;
	cpumask_var_t cpus_allowed, new_mask;

	اگر (!alloc_cpumask_var(&cpus_allowed, GFP_KERNEL))
		بازگشت -ENOMEM;

	اگر (!alloc_cpumask_var(&new_mask, GFP_KERNEL)) {
		retval = -ENOMEM;
		برو out_free_cpus_allowed;
	}

	cpuset_cpus_allowed(p, cpus_allowed);
	cpumask_and(new_mask, ctx->new_mask, cpus_allowed);

	ctx->new_mask = new_mask;
	ctx->flags |= SCA_CHECK;

	retval = dl_task_check_affinity(p, new_mask);
	اگر (retval)
		برو out_free_new_mask;

	retval = __set_cpus_allowed_ptr(p, ctx);
	اگر (retval)
		برو out_free_new_mask;

	cpuset_cpus_allowed(p, cpus_allowed);
	اگر (!cpumask_subset(new_mask, cpus_allowed)) {
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
		اگر (unlikely((ctx->flags & SCA_USER) && ctx->user_mask)) {
			bool empty = !cpumask_and(new_mask, new_mask,
						  ctx->user_mask);

			اگر (empty)
				cpumask_copy(new_mask, cpus_allowed);
		}
		__set_cpus_allowed_ptr(p, ctx);
		retval = -EINVAL;
	}

out_free_new_mask:
	free_cpumask_var(new_mask);
out_free_cpus_allowed:
	free_cpumask_var(cpus_allowed);
	بازگشت retval;
}

long sched_setaffinity(pid_t pid, ثابت ساختار cpumask *in_mask)
{
	ساختار affinity_context ac;
	ساختار cpumask *user_mask;
	صحیح retval;

	CLASS(find_get_task, p)(pid);
	اگر (!p)
		بازگشت -ESRCH;

	اگر (p->flags & PF_NO_SETAFFINITY)
		بازگشت -EINVAL;

	اگر (!check_same_owner(p)) {
		guard(rcu)();
		اگر (!ns_capable(__task_cred(p)->user_ns, CAP_SYS_NICE))
			بازگشت -EPERM;
	}

	retval = security_task_setscheduler(p);
	اگر (retval)
		بازگشت retval;

	/*
	 * With non-SMP configs, user_cpus_ptr/user_mask isn't used and
	 * alloc_user_cpus_ptr() returns NULL.
	 */
	user_mask = alloc_user_cpus_ptr(NUMA_NO_گره);
	اگر (user_mask) {
		cpumask_copy(user_mask, in_mask);
	} وگرنه {
		بازگشت -ENOMEM;
	}

	ac = (ساختار affinity_context){
		.new_mask  = in_mask,
		.user_mask = user_mask,
		.flags     = SCA_USER,
	};

	retval = __sched_setaffinity(p, &ac);
	kfree(ac.user_mask);

	بازگشت retval;
}

ایستا صحیح get_user_cpu_mask(unsigned long __user *user_mask_ptr, unsigned len,
			     ساختار cpumask *new_mask)
{
	اگر (len < cpumask_size())
		cpumask_clear(new_mask);
	وگرنه اگر (len > cpumask_size())
		len = cpumask_size();

	بازگشت copy_from_user(new_mask, user_mask_ptr, len) ? -EFAULT : 0;
}

/**
 * sys_sched_setaffinity - set the CPU affinity of a process
 * @pid: pid of the process
 * @len: length in bytes of the bitmask pointed to by user_mask_ptr
 * @user_mask_ptr: user-space pointer to the new CPU mask
 *
 * Return: 0 on success. An error code otherwise.
 */
SYSCALL_DEFINE3(sched_setaffinity, pid_t, pid, unsigned صحیح, len,
		unsigned long __user *, user_mask_ptr)
{
	cpumask_var_t new_mask;
	صحیح retval;

	اگر (!alloc_cpumask_var(&new_mask, GFP_KERNEL))
		بازگشت -ENOMEM;

	retval = get_user_cpu_mask(user_mask_ptr, len, new_mask);
	اگر (retval == 0)
		retval = sched_setaffinity(pid, new_mask);
	free_cpumask_var(new_mask);
	بازگشت retval;
}

long sched_getaffinity(pid_t pid, ساختار cpumask *mask)
{
	ساختار task_struct *p;
	صحیح retval;

	guard(rcu)();
	p = find_process_by_pid(pid);
	اگر (!p)
		بازگشت -ESRCH;

	retval = security_task_getscheduler(p);
	اگر (retval)
		بازگشت retval;

	guard(raw_spinlock_irqsave)(&p->pi_lock);
	cpumask_and(mask, &p->cpus_mask, cpu_active_mask);

	بازگشت 0;
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
SYSCALL_DEFINE3(sched_getaffinity, pid_t, pid, unsigned صحیح, len,
		unsigned long __user *, user_mask_ptr)
{
	صحیح ret;
	cpumask_var_t mask;

	اگر ((len * BITS_PER_BYTE) < nr_cpu_ids)
		بازگشت -EINVAL;
	اگر (len & (اندازه(unsigned long)-1))
		بازگشت -EINVAL;

	اگر (!zalloc_cpumask_var(&mask, GFP_KERNEL))
		بازگشت -ENOMEM;

	ret = sched_getaffinity(pid, mask);
	اگر (ret == 0) {
		unsigned صحیح retlen = min(len, cpumask_size());

		اگر (copy_to_user(user_mask_ptr, cpumask_bits(mask), retlen))
			ret = -EFAULT;
		وگرنه
			ret = retlen;
	}
	free_cpumask_var(mask);

	بازگشت ret;
}

ایستا تهی do_sched_yield(تهی)
{
	ساختار rq_flags rf;
	ساختار rq *rq;

	rq = this_rq_lock_irq(&rf);

	schedstat_inc(rq->yld_شمار);
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
	بازگشت 0;
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
تهی __sched yield(تهی)
{
	set_current_state(TASK_RUNNING);
	do_sched_yield();
}
EXPORT_نماد(yield);

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
صحیح __sched yield_to(ساختار task_struct *p, bool preempt)
{
	ساختار task_struct *curr;
	ساختار rq *rq, *p_rq;
	صحیح yielded = 0;

	scoped_guard (raw_spinlock_irqsave, &p->pi_lock) {
		rq = this_rq();
		curr = rq->donor;

again:
		p_rq = task_rq(p);
		/*
		 * If we're the only runnable task on the rq and target rq also
		 * has only one task, there's absolutely no point in yielding.
		 */
		اگر (rq->nr_running == 1 && p_rq->nr_running == 1)
			بازگشت -ESRCH;

		guard(double_rq_lock)(rq, p_rq);
		اگر (task_rq(p) != p_rq)
			برو again;

		اگر (!curr->sched_class->yield_to_task)
			بازگشت 0;

		اگر (curr->sched_class != p->sched_class)
			بازگشت 0;

		اگر (task_on_cpu(p_rq, p) || !task_is_running(p))
			بازگشت 0;

		yielded = curr->sched_class->yield_to_task(rq, p);
		اگر (yielded) {
			schedstat_inc(rq->yld_شمار);
			/*
			 * Make p's CPU reschedule; pick_next_entity
			 * takes care of fairness.
			 */
			اگر (preempt && rq != p_rq)
				resched_curr(p_rq);
		}
	}

	اگر (yielded)
		schedule();

	بازگشت yielded;
}
EXPORT_نماد_GPL(yield_to);

/**
 * sys_sched_get_priority_max - return maximum RT priority.
 * @policy: scheduling class.
 *
 * Return: On success, this syscall returns the maximum
 * rt_priority that can be used by a given scheduling class.
 * On failure, a negative error code is returned.
 */
SYSCALL_DEFINE1(sched_get_priority_max, صحیح, policy)
{
	صحیح ret = -EINVAL;

	انتخاب (policy) {
	حالت SCHED_FIFO:
	حالت SCHED_RR:
		ret = MAX_RT_PRIO-1;
		بشکن;
	حالت SCHED_DEADLINE:
	حالت SCHED_NORMAL:
	حالت SCHED_BATCH:
	حالت SCHED_IDLE:
	حالت SCHED_EXT:
		ret = 0;
		بشکن;
	}
	بازگشت ret;
}

/**
 * sys_sched_get_priority_min - return minimum RT priority.
 * @policy: scheduling class.
 *
 * Return: On success, this syscall returns the minimum
 * rt_priority that can be used by a given scheduling class.
 * On failure, a negative error code is returned.
 */
SYSCALL_DEFINE1(sched_get_priority_min, صحیح, policy)
{
	صحیح ret = -EINVAL;

	انتخاب (policy) {
	حالت SCHED_FIFO:
	حالت SCHED_RR:
		ret = 1;
		بشکن;
	حالت SCHED_DEADLINE:
	حالت SCHED_NORMAL:
	حالت SCHED_BATCH:
	حالت SCHED_IDLE:
	حالت SCHED_EXT:
		ret = 0;
	}
	بازگشت ret;
}

ایستا صحیح sched_rr_get_interval(pid_t pid, ساختار timespec64 *t)
{
	unsigned صحیح time_slice = 0;
	صحیح retval;

	اگر (pid < 0)
		بازگشت -EINVAL;

	scoped_guard (rcu) {
		ساختار task_struct *p = find_process_by_pid(pid);
		اگر (!p)
			بازگشت -ESRCH;

		retval = security_task_getscheduler(p);
		اگر (retval)
			بازگشت retval;

		scoped_guard (task_rq_lock, p) {
			ساختار rq *rq = scope.rq;
			اگر (p->sched_class->get_rr_interval)
				time_slice = p->sched_class->get_rr_interval(rq, p);
		}
	}

	jiffies_to_timespec64(time_slice, t);
	بازگشت 0;
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
		ساختار __kernel_timespec __user *, interval)
{
	ساختار timespec64 t;
	صحیح retval = sched_rr_get_interval(pid, &t);

	اگر (retval == 0)
		retval = put_timespec64(&t, interval);

	بازگشت retval;
}

#ifdef CONFIG_COMPAT_32BIT_TIME
SYSCALL_DEFINE2(sched_rr_get_interval_time32, pid_t, pid,
		ساختار old_timespec32 __user *, interval)
{
	ساختار timespec64 t;
	صحیح retval = sched_rr_get_interval(pid, &t);

	اگر (retval == 0)
		retval = put_old_timespec32(&t, interval);
	بازگشت retval;
}
#endif
