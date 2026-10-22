// SPDX-License-Identifier: GPL-2.0-only
/*
 *  kernel/sched/syscalls.c
 *
 *  Core kernel scheduler syscalls related code
 *
 *  Copyright (C) 1991-2002  Linus Torvalds
 *  Copyright (C) 1998-2024  Ingo Molnar, Red Hat
 */
#समावेश <linux/sched.h>
#समावेश <linux/cpuset.h>
#समावेश <linux/sched/debug.h>

#समावेश <uapi/linux/sched/types.h>

#समावेश "sched.h"
#समावेश "autogroup.h"

अटल अंतरभूत पूर्णांक __normal_prio(पूर्णांक policy, पूर्णांक rt_prio, पूर्णांक nice)
{
	पूर्णांक prio;

	अगर (dl_policy(policy))
		prio = MAX_DL_PRIO - 1;
	अन्यथा अगर (rt_policy(policy))
		prio = MAX_RT_PRIO - 1 - rt_prio;
	अन्यथा
		prio = NICE_TO_PRIO(nice);

	वापस prio;
}

/*
 * Calculate the expected normal priority: i.e. priority
 * without taking RT-inheritance into account. Might be
 * boosted by interactivity modifiers. Changes upon fork,
 * setprio syscalls, and whenever the interactivity
 * estimator recalculates.
 */
अटल अंतरभूत पूर्णांक normal_prio(काष्ठा task_struct *p)
{
	वापस __normal_prio(p->policy, p->rt_priority, PRIO_TO_NICE(p->static_prio));
}

/*
 * Calculate the current priority, i.e. the priority
 * taken into account by the scheduler. This value might
 * be boosted by RT tasks, or might be boosted by
 * interactivity modifiers. Will be RT if the task got
 * RT-boosted. If not then it returns p->normal_prio.
 */
अटल पूर्णांक effective_prio(काष्ठा task_struct *p)
{
	p->normal_prio = normal_prio(p);
	/*
	 * If we are RT tasks or we were boosted to RT priority,
	 * keep the priority unchanged. Otherwise, update priority
	 * to the normal priority:
	 */
	अगर (!rt_or_dl_prio(p->prio))
		वापस p->normal_prio;
	वापस p->prio;
}

व्योम set_user_nice(काष्ठा task_struct *p, दीर्घ nice)
{
	पूर्णांक old_prio;

	अगर (task_nice(p) == nice || nice < MIN_NICE || nice > MAX_NICE)
		वापस;
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
	अगर (task_has_dl_policy(p) || task_has_rt_policy(p)) {
		p->static_prio = NICE_TO_PRIO(nice);
		वापस;
	}

	scoped_guard (sched_change, p, DEQUEUE_SAVE) {
		p->static_prio = NICE_TO_PRIO(nice);
		set_load_weight(p, true);
		old_prio = p->prio;
		p->prio = effective_prio(p);
	}
}
EXPORT_प्रतीक(set_user_nice);

/*
 * is_nice_reduction - check if nice value is an actual reduction
 *
 * Similar to can_nice() but does not perform a capability check.
 *
 * @p: task
 * @nice: nice value
 */
अटल bool is_nice_reduction(स्थिर काष्ठा task_struct *p, स्थिर पूर्णांक nice)
{
	/* Convert nice value [19,-20] to rlimit style value [1,40]: */
	पूर्णांक nice_rlim = nice_to_rlimit(nice);

	वापस (nice_rlim <= task_rlimit(p, RLIMIT_NICE));
}

/*
 * can_nice - check if a task can reduce its nice value
 * @p: task
 * @nice: nice value
 */
पूर्णांक can_nice(स्थिर काष्ठा task_struct *p, स्थिर पूर्णांक nice)
{
	वापस is_nice_reduction(p, nice) || capable(CAP_SYS_NICE);
}

#अगर_घोषित __ARCH_WANT_SYS_NICE

/*
 * sys_nice - change the priority of the current process.
 * @increment: priority increment
 *
 * sys_setpriority is a more generic, but much slower function that
 * does similar things.
 */
SYSCALL_DEFINE1(nice, पूर्णांक, increment)
{
	दीर्घ nice, retval;

	/*
	 * Setpriority might change our priority at the same moment.
	 * We don't have to worry. Conceptually one call occurs first
	 * and we have a single winner.
	 */
	increment = clamp(increment, -NICE_WIDTH, NICE_WIDTH);
	nice = task_nice(current) + increment;

	nice = clamp_val(nice, MIN_NICE, MAX_NICE);
	अगर (increment < 0 && !can_nice(current, nice))
		वापस -EPERM;

	retval = security_task_setnice(current, nice);
	अगर (retval)
		वापस retval;

	set_user_nice(current, nice);
	वापस 0;
}

#पूर्ण_अगर /* __ARCH_WANT_SYS_NICE */

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
पूर्णांक task_prio(स्थिर काष्ठा task_struct *p)
{
	वापस p->prio - MAX_RT_PRIO;
}

/**
 * idle_cpu - is a given CPU idle currently?
 * @cpu: the processor in question.
 *
 * Return: 1 if the CPU is currently idle. 0 otherwise.
 */
पूर्णांक idle_cpu(पूर्णांक cpu)
{
	वापस idle_rq(cpu_rq(cpu));
}

/**
 * idle_task - return the idle task for a given CPU.
 * @cpu: the processor in question.
 *
 * Return: The idle task for the CPU @cpu.
 */
काष्ठा task_struct *idle_task(पूर्णांक cpu)
{
	वापस cpu_rq(cpu)->idle;
}

#अगर_घोषित CONFIG_SCHED_CORE
पूर्णांक sched_core_idle_cpu(पूर्णांक cpu)
{
	काष्ठा rq *rq = cpu_rq(cpu);

	अगर (sched_core_enabled(rq) && rq->curr == rq->idle)
		वापस 1;

	वापस idle_cpu(cpu);
}
#पूर्ण_अगर /* CONFIG_SCHED_CORE */

/**
 * find_process_by_pid - find a process with a matching PID value.
 * @pid: the pid in question.
 *
 * The task of @pid, if found. %NULL otherwise.
 */
अटल काष्ठा task_struct *find_process_by_pid(pid_t pid)
{
	वापस pid ? find_task_by_vpid(pid) : current;
}

अटल काष्ठा task_struct *find_get_task(pid_t pid)
{
	काष्ठा task_struct *p;
	guard(rcu)();

	p = find_process_by_pid(pid);
	अगर (likely(p))
		get_task_struct(p);

	वापस p;
}

DEFINE_CLASS(find_get_task, काष्ठा task_struct *, अगर (_T) put_task_struct(_T),
	     find_get_task(pid), pid_t pid)

/*
 * sched_setparam() passes in -1 for its policy, to let the functions
 * it calls know not to change it.
 */
#घोषणा SETPARAM_POLICY	-1

अटल व्योम __setscheduler_params(काष्ठा task_struct *p,
		स्थिर काष्ठा sched_attr *attr)
{
	पूर्णांक policy = attr->sched_policy;

	अगर (policy == SETPARAM_POLICY)
		policy = p->policy;

	p->policy = policy;

	अगर (dl_policy(policy))
		__setparam_dl(p, attr);
	अन्यथा अगर (fair_policy(policy))
		__setparam_fair(p, attr);

	/* rt-policy tasks do not have a timerslack */
	अगर (rt_or_dl_task_policy(p)) {
		p->timer_slack_ns = 0;
	} अन्यथा अगर (p->timer_slack_ns == 0) {
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
अटल bool check_same_owner(काष्ठा task_struct *p)
{
	स्थिर काष्ठा cred *cred = current_cred(), *pcred;
	guard(rcu)();

	pcred = __task_cred(p);
	वापस (uid_eq(cred->euid, pcred->euid) ||
		uid_eq(cred->euid, pcred->uid));
}

#अगर_घोषित CONFIG_RT_MUTEXES
अटल अंतरभूत व्योम __setscheduler_dl_pi(पूर्णांक newprio, पूर्णांक policy,
			      काष्ठा task_struct *p,
			      काष्ठा sched_change_ctx *scope)
{
	/*
	 * In case a DEADLINE task (either proper or boosted) gets
	 * setscheduled to a lower priority class, check if it neeeds to
	 * inherit parameters from a potential pi_task. In that case make
	 * sure replenishment happens with the next enqueue.
	 */

	अगर (dl_prio(newprio) && !dl_policy(policy)) {
		काष्ठा task_struct *pi_task = rt_mutex_get_शीर्ष_task(p);

		अगर (pi_task) {
			p->dl.pi_se = pi_task->dl.pi_se;
			scope->flags |= ENQUEUE_REPLENISH;
		}
	}
}
#अन्यथा /* !CONFIG_RT_MUTEXES */
अटल अंतरभूत व्योम __setscheduler_dl_pi(पूर्णांक newprio, पूर्णांक policy,
			      काष्ठा task_struct *p,
			      काष्ठा sched_change_ctx *scope)
{
}
#पूर्ण_अगर /* !CONFIG_RT_MUTEXES */

#अगर_घोषित CONFIG_UCLAMP_TASK

अटल पूर्णांक uclamp_validate(काष्ठा task_struct *p,
			   स्थिर काष्ठा sched_attr *attr)
{
	पूर्णांक util_min = p->uclamp_req[UCLAMP_MIN].मूल्य;
	पूर्णांक util_max = p->uclamp_req[UCLAMP_MAX].मूल्य;

	अगर (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MIN) {
		util_min = attr->sched_util_min;

		अगर (util_min + 1 > SCHED_CAPACITY_SCALE + 1)
			वापस -EINVAL;
	}

	अगर (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MAX) {
		util_max = attr->sched_util_max;

		अगर (util_max + 1 > SCHED_CAPACITY_SCALE + 1)
			वापस -EINVAL;
	}

	अगर (util_min != -1 && util_max != -1 && util_min > util_max)
		वापस -EINVAL;

	/*
	 * We have valid uclamp attributes; make sure uclamp is enabled.
	 *
	 * We need to do that here, because enabling static branches is a
	 * blocking operation which obviously cannot be done while holding
	 * scheduler locks.
	 */
	sched_uclamp_enable();

	वापस 0;
}

अटल bool uclamp_reset(स्थिर काष्ठा sched_attr *attr,
			 क्रमागत uclamp_id clamp_id,
			 काष्ठा uclamp_se *uc_se)
{
	/* Reset on sched class change for a non user-defined clamp value. */
	अगर (likely(!(attr->sched_flags & SCHED_FLAG_UTIL_CLAMP)) &&
	    !uc_se->user_defined)
		वापस true;

	/* Reset on sched_util_{min,max} == -1. */
	अगर (clamp_id == UCLAMP_MIN &&
	    attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MIN &&
	    attr->sched_util_min == -1) {
		वापस true;
	}

	अगर (clamp_id == UCLAMP_MAX &&
	    attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MAX &&
	    attr->sched_util_max == -1) {
		वापस true;
	}

	वापस false;
}

अटल व्योम __setscheduler_uclamp(काष्ठा task_struct *p,
				  स्थिर काष्ठा sched_attr *attr)
{
	क्रमागत uclamp_id clamp_id;

	for_each_clamp_id(clamp_id) {
		काष्ठा uclamp_se *uc_se = &p->uclamp_req[clamp_id];
		अचिन्हित पूर्णांक मूल्य;

		अगर (!uclamp_reset(attr, clamp_id, uc_se))
			जारी;

		/*
		 * RT by default have a 100% boost value that could be modified
		 * at runtime.
		 */
		अगर (unlikely(rt_task(p) && clamp_id == UCLAMP_MIN))
			मूल्य = sysctl_sched_uclamp_util_min_rt_default;
		अन्यथा
			मूल्य = uclamp_none(clamp_id);

		uclamp_se_set(uc_se, मूल्य, false);

	}

	अगर (likely(!(attr->sched_flags & SCHED_FLAG_UTIL_CLAMP)))
		वापस;

	अगर (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MIN &&
	    attr->sched_util_min != -1) {
		uclamp_se_set(&p->uclamp_req[UCLAMP_MIN],
			      attr->sched_util_min, true);
	}

	अगर (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MAX &&
	    attr->sched_util_max != -1) {
		uclamp_se_set(&p->uclamp_req[UCLAMP_MAX],
			      attr->sched_util_max, true);
	}
}

#अन्यथा /* !CONFIG_UCLAMP_TASK: */

अटल अंतरभूत पूर्णांक uclamp_validate(काष्ठा task_struct *p,
				  स्थिर काष्ठा sched_attr *attr)
{
	वापस -EOPNOTSUPP;
}
अटल व्योम __setscheduler_uclamp(काष्ठा task_struct *p,
				  स्थिर काष्ठा sched_attr *attr) { }
#पूर्ण_अगर /* !CONFIG_UCLAMP_TASK */

/*
 * Allow unprivileged RT tasks to decrease priority.
 * Only issue a capable test if needed and only once to avoid an audit
 * event on permitted non-privileged operations:
 */
अटल पूर्णांक user_check_sched_setscheduler(काष्ठा task_struct *p,
					 स्थिर काष्ठा sched_attr *attr,
					 पूर्णांक policy, पूर्णांक reset_on_fork)
{
	अगर (fair_policy(policy)) {
		अगर (attr->sched_nice < task_nice(p) &&
		    !is_nice_reduction(p, attr->sched_nice))
			जाओ req_priv;
	}

	अगर (rt_policy(policy)) {
		अचिन्हित दीर्घ rlim_rtprio = task_rlimit(p, RLIMIT_RTPRIO);

		/* Can't set/change the rt policy: */
		अगर (policy != p->policy && !rlim_rtprio)
			जाओ req_priv;

		/* Can't increase priority: */
		अगर (attr->sched_priority > p->rt_priority &&
		    attr->sched_priority > rlim_rtprio)
			जाओ req_priv;
	}

	/*
	 * Can't set/change SCHED_DEADLINE policy at all for now
	 * (safest behavior); in the future we would like to allow
	 * unprivileged DL tasks to increase their relative deadline
	 * or reduce their runtime (both ways reducing utilization)
	 */
	अगर (dl_policy(policy))
		जाओ req_priv;

	/*
	 * Treat SCHED_IDLE as nice 20. Only allow a switch to
	 * SCHED_NORMAL if the RLIMIT_NICE would normally permit it.
	 */
	अगर (task_has_idle_policy(p) && !idle_policy(policy)) {
		अगर (!is_nice_reduction(p, task_nice(p)))
			जाओ req_priv;
	}

	/* Can't change other user's priorities: */
	अगर (!check_same_owner(p))
		जाओ req_priv;

	/* Normal users shall not reset the sched_reset_on_fork flag: */
	अगर (p->sched_reset_on_fork && !reset_on_fork)
		जाओ req_priv;

	वापस 0;

req_priv:
	अगर (!capable(CAP_SYS_NICE))
		वापस -EPERM;

	वापस 0;
}

पूर्णांक __sched_setscheduler(काष्ठा task_struct *p,
			 स्थिर काष्ठा sched_attr *attr,
			 bool user, bool pi)
{
	पूर्णांक oldpolicy = -1, policy = attr->sched_policy;
	पूर्णांक retval, oldprio, newprio;
	स्थिर काष्ठा sched_class *पूर्व_class, *अग्र_class;
	काष्ठा balance_callback *शीर्ष;
	काष्ठा rq_flags rf;
	पूर्णांक reset_on_fork;
	पूर्णांक queue_flags = DEQUEUE_SAVE | DEQUEUE_MOVE | DEQUEUE_NOCLOCK;
	काष्ठा rq *rq;
	bool cpuset_locked = false;

	/* The pi code expects interrupts enabled */
	BUG_ON(pi && in_interrupt());
recheck:
	/* Double check policy once rq lock held: */
	अगर (policy < 0) {
		reset_on_fork = p->sched_reset_on_fork;
		policy = oldpolicy = p->policy;
	} अन्यथा {
		reset_on_fork = !!(attr->sched_flags & SCHED_FLAG_RESET_ON_FORK);

		अगर (!valid_policy(policy))
			वापस -EINVAL;
	}

	अगर (attr->sched_flags & ~(SCHED_FLAG_ALL | SCHED_FLAG_SUGOV))
		वापस -EINVAL;

	/*
	 * Valid priorities for SCHED_FIFO and SCHED_RR are
	 * 1..MAX_RT_PRIO-1, valid priority for SCHED_NORMAL,
	 * SCHED_BATCH and SCHED_IDLE is 0.
	 */
	अगर (attr->sched_priority > MAX_RT_PRIO-1)
		वापस -EINVAL;
	अगर ((dl_policy(policy) && !__checkparam_dl(attr)) ||
	    (rt_policy(policy) != (attr->sched_priority != 0)))
		वापस -EINVAL;

	अगर (user) {
		retval = user_check_sched_setscheduler(p, attr, policy, reset_on_fork);
		अगर (retval)
			वापस retval;

		अगर (attr->sched_flags & SCHED_FLAG_SUGOV)
			वापस -EINVAL;

		retval = security_task_setscheduler(p);
		अगर (retval)
			वापस retval;
	}

	/* Update task specific "requested" clamps */
	अगर (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP) {
		retval = uclamp_validate(p, attr);
		अगर (retval)
			वापस retval;
	}

	/*
	 * SCHED_DEADLINE bandwidth accounting relies on stable cpusets
	 * information.
	 */
	अगर (dl_policy(policy) || dl_policy(p->policy)) {
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
	अगर (p == rq->stop) {
		retval = -EINVAL;
		जाओ unlock;
	}

	retval = scx_check_setscheduler(p, policy);
	अगर (retval)
		जाओ unlock;

	/*
	 * If not changing anything there's no need to proceed further,
	 * but store a possible modification of reset_on_fork.
	 */
	अगर (unlikely(policy == p->policy)) {
		अगर (fair_policy(policy) &&
		    (attr->sched_nice != task_nice(p) ||
		     (attr->sched_runtime != p->se.slice)))
			जाओ change;
		अगर (rt_policy(policy) && attr->sched_priority != p->rt_priority)
			जाओ change;
		अगर (dl_policy(policy) && dl_param_changed(p, attr))
			जाओ change;
		अगर (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP)
			जाओ change;

		p->sched_reset_on_fork = reset_on_fork;
		retval = 0;
		जाओ unlock;
	}
change:

	अगर (user) {
#अगर_घोषित CONFIG_RT_GROUP_SCHED
		/*
		 * Do not allow real-time tasks into groups that have no runtime
		 * assigned.
		 */
		अगर (rt_group_sched_enabled() &&
				rt_bandwidth_enabled() && rt_policy(policy) &&
				task_group(p)->rt_bandwidth.rt_runtime == 0 &&
				!task_group_is_autogroup(task_group(p))) {
			retval = -EPERM;
			जाओ unlock;
		}
#पूर्ण_अगर /* CONFIG_RT_GROUP_SCHED */
		अगर (dl_bandwidth_enabled() && dl_policy(policy) &&
				!(attr->sched_flags & SCHED_FLAG_SUGOV)) {
			cpumask_t *span = rq->rd->span;

			/*
			 * Don't allow tasks with an affinity mask smaller than
			 * the entire root_domain to become SCHED_DEADLINE. We
			 * will also fail if there's no bandwidth available.
			 */
			अगर (!cpumask_subset(span, p->cpus_ptr) ||
			    rq->rd->dl_bw.bw == 0) {
				retval = -EPERM;
				जाओ unlock;
			}
		}
	}

	/* Re-check policy now with rq lock held: */
	अगर (unlikely(oldpolicy != -1 && oldpolicy != p->policy)) {
		policy = oldpolicy = -1;
		task_rq_unlock(rq, p, &rf);
		अगर (cpuset_locked)
			cpuset_unlock();
		जाओ recheck;
	}

	/*
	 * If setscheduling to SCHED_DEADLINE (or changing the parameters
	 * of a SCHED_DEADLINE task) we need to check if enough bandwidth
	 * is available.
	 */
	अगर ((dl_policy(policy) || dl_task(p)) && sched_dl_overflow(p, policy, attr)) {
		retval = -EBUSY;
		जाओ unlock;
	}

	p->sched_reset_on_fork = reset_on_fork;
	oldprio = p->prio;

	newprio = __normal_prio(policy, attr->sched_priority, attr->sched_nice);
	अगर (pi) {
		/*
		 * Take priority boosted tasks into account. If the new
		 * effective priority is unchanged, we just store the new
		 * normal parameters and do not touch the scheduler class and
		 * the runqueue. This will be done when the task deboost
		 * itself.
		 */
		newprio = rt_effective_prio(p, newprio);
		अगर (newprio == oldprio && !dl_prio(newprio))
			queue_flags &= ~DEQUEUE_MOVE;
	}

	पूर्व_class = p->sched_class;
	अग्र_class = __setscheduler_class(policy, newprio);

	अगर (पूर्व_class != अग्र_class)
		queue_flags |= DEQUEUE_CLASS;

	scoped_guard (sched_change, p, queue_flags) {

		अगर (!(attr->sched_flags & SCHED_FLAG_KEEP_PARAMS)) {
			__setscheduler_params(p, attr);
			p->sched_class = अग्र_class;
			p->prio = newprio;
			__setscheduler_dl_pi(newprio, policy, p, scope);
		}
		__setscheduler_uclamp(p, attr);

		अगर (scope->queued) {
			/*
			 * We enqueue to tail when the priority of a task is
			 * increased (user space view).
			 */
			अगर (oldprio < p->prio)
				scope->flags |= ENQUEUE_शीर्ष;
		}
	}

	/* Avoid rq from going away on us: */
	preempt_disable();
	शीर्ष = splice_balance_callbacks(rq);
	task_rq_unlock(rq, p, &rf);

	अगर (pi) {
		अगर (cpuset_locked)
			cpuset_unlock();
		rt_mutex_adjust_pi(p);
	}

	/* Run balance callbacks after we've adjusted the PI chain: */
	balance_callbacks(rq, शीर्ष);
	preempt_enable();

	वापस 0;

unlock:
	task_rq_unlock(rq, p, &rf);
	अगर (cpuset_locked)
		cpuset_unlock();
	वापस retval;
}

अटल पूर्णांक _sched_setscheduler(काष्ठा task_struct *p, पूर्णांक policy,
			       स्थिर काष्ठा sched_param *param, bool check)
{
	काष्ठा sched_attr attr = {
		.sched_policy   = policy,
		.sched_priority = param->sched_priority,
		.sched_nice	= PRIO_TO_NICE(p->static_prio),
	};

	अगर (p->se.custom_slice)
		attr.sched_runtime = p->se.slice;

	/* Fixup the legacy SCHED_RESET_ON_FORK hack. */
	अगर ((policy != SETPARAM_POLICY) && (policy & SCHED_RESET_ON_FORK)) {
		attr.sched_flags |= SCHED_FLAG_RESET_ON_FORK;
		policy &= ~SCHED_RESET_ON_FORK;
		attr.sched_policy = policy;
	}

	वापस __sched_setscheduler(p, &attr, check, true);
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
पूर्णांक sched_setscheduler(काष्ठा task_struct *p, पूर्णांक policy,
		       स्थिर काष्ठा sched_param *param)
{
	वापस _sched_setscheduler(p, policy, param, true);
}

पूर्णांक sched_setattr(काष्ठा task_struct *p, स्थिर काष्ठा sched_attr *attr)
{
	वापस __sched_setscheduler(p, attr, true, true);
}

पूर्णांक sched_setattr_nocheck(काष्ठा task_struct *p, स्थिर काष्ठा sched_attr *attr)
{
	वापस __sched_setscheduler(p, attr, false, true);
}
EXPORT_प्रतीक_GPL(sched_setattr_nocheck);

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
पूर्णांक sched_setscheduler_nocheck(काष्ठा task_struct *p, पूर्णांक policy,
			       स्थिर काष्ठा sched_param *param)
{
	वापस _sched_setscheduler(p, policy, param, false);
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
व्योम sched_set_fifo(काष्ठा task_struct *p)
{
	काष्ठा sched_param sp = { .sched_priority = MAX_RT_PRIO / 2 };
	WARN_ON_ONCE(sched_setscheduler_nocheck(p, SCHED_FIFO, &sp) != 0);
}
EXPORT_प्रतीक_GPL(sched_set_fifo);

/*
 * For when you don't much care about FIFO, but want to be above SCHED_NORMAL.
 */
व्योम sched_set_fifo_निचला(काष्ठा task_struct *p)
{
	काष्ठा sched_param sp = { .sched_priority = 1 };
	WARN_ON_ONCE(sched_setscheduler_nocheck(p, SCHED_FIFO, &sp) != 0);
}
EXPORT_प्रतीक_GPL(sched_set_fifo_निचला);

/*
 * Used when the primary interrupt handler is forced into a thread, in addition
 * to the (always threaded) secondary handler.  The secondary handler gets a
 * slightly lower priority so that the primary handler can preempt it, thereby
 * emulating the behavior of a non-PREEMPT_RT system where the primary handler
 * runs in hard interrupt context.
 */
व्योम sched_set_fifo_secondary(काष्ठा task_struct *p)
{
	काष्ठा sched_param sp = { .sched_priority = MAX_RT_PRIO / 2 - 1 };
	WARN_ON_ONCE(sched_setscheduler_nocheck(p, SCHED_FIFO, &sp) != 0);
}

व्योम sched_set_normal(काष्ठा task_struct *p, पूर्णांक nice)
{
	काष्ठा sched_attr attr = {
		.sched_policy = SCHED_NORMAL,
		.sched_nice = nice,
	};
	WARN_ON_ONCE(sched_setattr_nocheck(p, &attr) != 0);
}
EXPORT_प्रतीक_GPL(sched_set_normal);

अटल पूर्णांक
do_sched_setscheduler(pid_t pid, पूर्णांक policy, काष्ठा sched_param __user *param)
{
	काष्ठा sched_param lparam;

	अगर (unlikely(!param || pid < 0))
		वापस -EINVAL;
	अगर (copy_from_user(&lparam, param, माप(काष्ठा sched_param)))
		वापस -EFAULT;

	CLASS(find_get_task, p)(pid);
	अगर (!p)
		वापस -ESRCH;

	वापस sched_setscheduler(p, policy, &lparam);
}

/*
 * Mimics kernel/events/core.c perf_copy_attr().
 */
अटल पूर्णांक sched_copy_attr(काष्ठा sched_attr __user *uattr, काष्ठा sched_attr *attr)
{
	u32 आकार;
	पूर्णांक ret;

	/* Zero the full structure, so that a short copy will be nice: */
	स_रखो(attr, 0, माप(*attr));

	ret = get_user(आकार, &uattr->आकार);
	अगर (ret)
		वापस ret;

	/* ABI compatibility quirk: */
	अगर (!आकार)
		आकार = SCHED_ATTR_आकार_VER0;
	अगर (आकार < SCHED_ATTR_आकार_VER0 || आकार > PAGE_आकार)
		जाओ err_आकार;

	ret = copy_struct_from_user(attr, माप(*attr), uattr, आकार);
	अगर (ret) {
		अगर (ret == -E2BIG)
			जाओ err_आकार;
		वापस ret;
	}

	अगर ((attr->sched_flags & SCHED_FLAG_UTIL_CLAMP) &&
	    आकार < SCHED_ATTR_आकार_VER1)
		वापस -EINVAL;

	/*
	 * XXX: Do we want to be lenient like existing syscalls; or do we want
	 * to be strict and return an error on out-of-bounds values?
	 */
	attr->sched_nice = clamp(attr->sched_nice, MIN_NICE, MAX_NICE);

	वापस 0;

err_आकार:
	put_user(माप(*attr), &uattr->आकार);
	वापस -E2BIG;
}

अटल व्योम get_params(काष्ठा task_struct *p, काष्ठा sched_attr *attr, अचिन्हित पूर्णांक flags)
{
	अगर (task_has_dl_policy(p)) {
		__getparam_dl(p, attr, flags);
	} अन्यथा अगर (task_has_rt_policy(p)) {
		attr->sched_priority = p->rt_priority;
	} अन्यथा {
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
SYSCALL_DEFINE3(sched_setscheduler, pid_t, pid, पूर्णांक, policy, काष्ठा sched_param __user *, param)
{
	अगर (policy < 0)
		वापस -EINVAL;

	वापस do_sched_setscheduler(pid, policy, param);
}

/**
 * sys_sched_setparam - set/change the RT priority of a thread
 * @pid: the pid in question.
 * @param: structure containing the new RT priority.
 *
 * Return: 0 on success. An error code otherwise.
 */
SYSCALL_DEFINE2(sched_setparam, pid_t, pid, काष्ठा sched_param __user *, param)
{
	वापस do_sched_setscheduler(pid, SETPARAM_POLICY, param);
}

/**
 * sys_sched_setattr - same as above, but with extended sched_attr
 * @pid: the pid in question.
 * @uattr: structure containing the extended parameters.
 * @flags: for future extension.
 */
SYSCALL_DEFINE3(sched_setattr, pid_t, pid, काष्ठा sched_attr __user *, uattr,
			       अचिन्हित पूर्णांक, flags)
{
	काष्ठा sched_attr attr;
	पूर्णांक retval;

	अगर (unlikely(!uattr || pid < 0 || flags))
		वापस -EINVAL;

	retval = sched_copy_attr(uattr, &attr);
	अगर (retval)
		वापस retval;

	अगर ((पूर्णांक)attr.sched_policy < 0)
		वापस -EINVAL;
	अगर (attr.sched_flags & SCHED_FLAG_KEEP_POLICY)
		attr.sched_policy = SETPARAM_POLICY;

	CLASS(find_get_task, p)(pid);
	अगर (!p)
		वापस -ESRCH;

	अगर (attr.sched_flags & SCHED_FLAG_KEEP_PARAMS)
		get_params(p, &attr, 0);

	वापस sched_setattr(p, &attr);
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
	काष्ठा task_struct *p;
	पूर्णांक retval;

	अगर (pid < 0)
		वापस -EINVAL;

	guard(rcu)();
	p = find_process_by_pid(pid);
	अगर (!p)
		वापस -ESRCH;

	retval = security_task_getscheduler(p);
	अगर (!retval) {
		retval = p->policy;
		अगर (p->sched_reset_on_fork)
			retval |= SCHED_RESET_ON_FORK;
	}
	वापस retval;
}

/**
 * sys_sched_getparam - get the RT priority of a thread
 * @pid: the pid in question.
 * @param: structure containing the RT priority.
 *
 * Return: On success, 0 and the RT priority is in @param. Otherwise, an error
 * code.
 */
SYSCALL_DEFINE2(sched_getparam, pid_t, pid, काष्ठा sched_param __user *, param)
{
	काष्ठा sched_param lp = { .sched_priority = 0 };
	काष्ठा task_struct *p;
	पूर्णांक retval;

	अगर (unlikely(!param || pid < 0))
		वापस -EINVAL;

	scoped_guard (rcu) {
		p = find_process_by_pid(pid);
		अगर (!p)
			वापस -ESRCH;

		retval = security_task_getscheduler(p);
		अगर (retval)
			वापस retval;

		अगर (task_has_rt_policy(p))
			lp.sched_priority = p->rt_priority;
	}

	/*
	 * This one might sleep, we cannot do it with a spinlock held ...
	 */
	वापस copy_to_user(param, &lp, माप(*param)) ? -EFAULT : 0;
}

/**
 * sys_sched_getattr - similar to sched_getparam, but with sched_attr
 * @pid: the pid in question.
 * @uattr: structure containing the extended parameters.
 * @usize: sizeof(attr) for fwd/bwd comp.
 * @flags: for future extension.
 */
SYSCALL_DEFINE4(sched_getattr, pid_t, pid, काष्ठा sched_attr __user *, uattr,
		अचिन्हित पूर्णांक, usize, अचिन्हित पूर्णांक, flags)
{
	काष्ठा sched_attr kattr = { };
	काष्ठा task_struct *p;
	पूर्णांक retval;

	अगर (unlikely(!uattr || pid < 0 || usize > PAGE_आकार ||
		     usize < SCHED_ATTR_आकार_VER0))
		वापस -EINVAL;

	scoped_guard (rcu) {
		p = find_process_by_pid(pid);
		अगर (!p)
			वापस -ESRCH;

		अगर (flags) {
			अगर (!task_has_dl_policy(p) ||
			    flags != SCHED_GETATTR_FLAG_DL_DYNAMIC)
				वापस -EINVAL;
		}

		retval = security_task_getscheduler(p);
		अगर (retval)
			वापस retval;

		kattr.sched_policy = p->policy;
		अगर (p->sched_reset_on_fork)
			kattr.sched_flags |= SCHED_FLAG_RESET_ON_FORK;
		get_params(p, &kattr, flags);
		kattr.sched_flags &= SCHED_FLAG_ALL;

#अगर_घोषित CONFIG_UCLAMP_TASK
		/*
		 * This could race with another potential updater, but this is fine
		 * because it'll correctly read the old or the new value. We don't need
		 * to guarantee who wins the race as long as it doesn't return garbage.
		 */
		kattr.sched_util_min = p->uclamp_req[UCLAMP_MIN].मूल्य;
		kattr.sched_util_max = p->uclamp_req[UCLAMP_MAX].मूल्य;
#पूर्ण_अगर
	}

	kattr.आकार = min(usize, माप(kattr));
	वापस copy_struct_to_user(uattr, usize, &kattr, माप(kattr), शून्य);
}

पूर्णांक dl_task_check_affinity(काष्ठा task_struct *p, स्थिर काष्ठा cpumask *mask)
{
	/*
	 * If the task isn't a deadline task or admission control is
	 * disabled then we don't care about affinity changes.
	 */
	अगर (!task_has_dl_policy(p) || !dl_bandwidth_enabled())
		वापस 0;

	/*
	 * The special/sugov task isn't part of regular bandwidth/admission
	 * control so let userspace change affinities.
	 */
	अगर (dl_entity_is_special(&p->dl))
		वापस 0;

	/*
	 * Since bandwidth control happens on root_domain basis,
	 * if admission test is enabled, we only admit -deadline
	 * tasks allowed to run on all the CPUs in the task's
	 * root_domain.
	 */
	guard(rcu)();
	अगर (!cpumask_subset(task_rq(p)->rd->span, mask))
		वापस -EBUSY;

	वापस 0;
}

पूर्णांक __sched_setaffinity(काष्ठा task_struct *p, काष्ठा affinity_context *ctx)
{
	पूर्णांक retval;
	cpumask_var_t cpus_allowed, new_mask;

	अगर (!alloc_cpumask_var(&cpus_allowed, GFP_KERNEL))
		वापस -ENOMEM;

	अगर (!alloc_cpumask_var(&new_mask, GFP_KERNEL)) {
		retval = -ENOMEM;
		जाओ out_free_cpus_allowed;
	}

	cpuset_cpus_allowed(p, cpus_allowed);
	cpumask_and(new_mask, ctx->new_mask, cpus_allowed);

	ctx->new_mask = new_mask;
	ctx->flags |= SCA_CHECK;

	retval = dl_task_check_affinity(p, new_mask);
	अगर (retval)
		जाओ out_free_new_mask;

	retval = __set_cpus_allowed_ptr(p, ctx);
	अगर (retval)
		जाओ out_free_new_mask;

	cpuset_cpus_allowed(p, cpus_allowed);
	अगर (!cpumask_subset(new_mask, cpus_allowed)) {
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
		अगर (unlikely((ctx->flags & SCA_USER) && ctx->user_mask)) {
			bool empty = !cpumask_and(new_mask, new_mask,
						  ctx->user_mask);

			अगर (empty)
				cpumask_copy(new_mask, cpus_allowed);
		}
		__set_cpus_allowed_ptr(p, ctx);
		retval = -EINVAL;
	}

out_free_new_mask:
	free_cpumask_var(new_mask);
out_free_cpus_allowed:
	free_cpumask_var(cpus_allowed);
	वापस retval;
}

दीर्घ sched_setaffinity(pid_t pid, स्थिर काष्ठा cpumask *in_mask)
{
	काष्ठा affinity_context ac;
	काष्ठा cpumask *user_mask;
	पूर्णांक retval;

	CLASS(find_get_task, p)(pid);
	अगर (!p)
		वापस -ESRCH;

	अगर (p->flags & PF_NO_SETAFFINITY)
		वापस -EINVAL;

	अगर (!check_same_owner(p)) {
		guard(rcu)();
		अगर (!ns_capable(__task_cred(p)->user_ns, CAP_SYS_NICE))
			वापस -EPERM;
	}

	retval = security_task_setscheduler(p);
	अगर (retval)
		वापस retval;

	/*
	 * With non-SMP configs, user_cpus_ptr/user_mask isn't used and
	 * alloc_user_cpus_ptr() returns NULL.
	 */
	user_mask = alloc_user_cpus_ptr(NUMA_NO_गाँठ);
	अगर (user_mask) {
		cpumask_copy(user_mask, in_mask);
	} अन्यथा {
		वापस -ENOMEM;
	}

	ac = (काष्ठा affinity_context){
		.new_mask  = in_mask,
		.user_mask = user_mask,
		.flags     = SCA_USER,
	};

	retval = __sched_setaffinity(p, &ac);
	kfree(ac.user_mask);

	वापस retval;
}

अटल पूर्णांक get_user_cpu_mask(अचिन्हित दीर्घ __user *user_mask_ptr, अचिन्हित len,
			     काष्ठा cpumask *new_mask)
{
	अगर (len < cpumask_आकार())
		cpumask_clear(new_mask);
	अन्यथा अगर (len > cpumask_आकार())
		len = cpumask_आकार();

	वापस copy_from_user(new_mask, user_mask_ptr, len) ? -EFAULT : 0;
}

/**
 * sys_sched_setaffinity - set the CPU affinity of a process
 * @pid: pid of the process
 * @len: length in bytes of the bitmask pointed to by user_mask_ptr
 * @user_mask_ptr: user-space pointer to the new CPU mask
 *
 * Return: 0 on success. An error code otherwise.
 */
SYSCALL_DEFINE3(sched_setaffinity, pid_t, pid, अचिन्हित पूर्णांक, len,
		अचिन्हित दीर्घ __user *, user_mask_ptr)
{
	cpumask_var_t new_mask;
	पूर्णांक retval;

	अगर (!alloc_cpumask_var(&new_mask, GFP_KERNEL))
		वापस -ENOMEM;

	retval = get_user_cpu_mask(user_mask_ptr, len, new_mask);
	अगर (retval == 0)
		retval = sched_setaffinity(pid, new_mask);
	free_cpumask_var(new_mask);
	वापस retval;
}

दीर्घ sched_getaffinity(pid_t pid, काष्ठा cpumask *mask)
{
	काष्ठा task_struct *p;
	पूर्णांक retval;

	guard(rcu)();
	p = find_process_by_pid(pid);
	अगर (!p)
		वापस -ESRCH;

	retval = security_task_getscheduler(p);
	अगर (retval)
		वापस retval;

	guard(raw_spinlock_irqsave)(&p->pi_lock);
	cpumask_and(mask, &p->cpus_mask, cpu_active_mask);

	वापस 0;
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
SYSCALL_DEFINE3(sched_getaffinity, pid_t, pid, अचिन्हित पूर्णांक, len,
		अचिन्हित दीर्घ __user *, user_mask_ptr)
{
	पूर्णांक ret;
	cpumask_var_t mask;

	अगर ((len * BITS_PER_BYTE) < nr_cpu_ids)
		वापस -EINVAL;
	अगर (len & (माप(अचिन्हित दीर्घ)-1))
		वापस -EINVAL;

	अगर (!zalloc_cpumask_var(&mask, GFP_KERNEL))
		वापस -ENOMEM;

	ret = sched_getaffinity(pid, mask);
	अगर (ret == 0) {
		अचिन्हित पूर्णांक retlen = min(len, cpumask_आकार());

		अगर (copy_to_user(user_mask_ptr, cpumask_bits(mask), retlen))
			ret = -EFAULT;
		अन्यथा
			ret = retlen;
	}
	free_cpumask_var(mask);

	वापस ret;
}

अटल व्योम do_sched_yield(व्योम)
{
	काष्ठा rq_flags rf;
	काष्ठा rq *rq;

	rq = this_rq_lock_irq(&rf);

	schedstat_inc(rq->yld_गिनती);
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
	वापस 0;
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
व्योम __sched yield(व्योम)
{
	set_current_state(TASK_RUNNING);
	do_sched_yield();
}
EXPORT_प्रतीक(yield);

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
पूर्णांक __sched yield_to(काष्ठा task_struct *p, bool preempt)
{
	काष्ठा task_struct *curr;
	काष्ठा rq *rq, *p_rq;
	पूर्णांक yielded = 0;

	scoped_guard (raw_spinlock_irqsave, &p->pi_lock) {
		rq = this_rq();
		curr = rq->donor;

again:
		p_rq = task_rq(p);
		/*
		 * If we're the only runnable task on the rq and target rq also
		 * has only one task, there's absolutely no point in yielding.
		 */
		अगर (rq->nr_running == 1 && p_rq->nr_running == 1)
			वापस -ESRCH;

		guard(double_rq_lock)(rq, p_rq);
		अगर (task_rq(p) != p_rq)
			जाओ again;

		अगर (!curr->sched_class->yield_to_task)
			वापस 0;

		अगर (curr->sched_class != p->sched_class)
			वापस 0;

		अगर (task_on_cpu(p_rq, p) || !task_is_running(p))
			वापस 0;

		yielded = curr->sched_class->yield_to_task(rq, p);
		अगर (yielded) {
			schedstat_inc(rq->yld_गिनती);
			/*
			 * Make p's CPU reschedule; pick_next_entity
			 * takes care of fairness.
			 */
			अगर (preempt && rq != p_rq)
				resched_curr(p_rq);
		}
	}

	अगर (yielded)
		schedule();

	वापस yielded;
}
EXPORT_प्रतीक_GPL(yield_to);

/**
 * sys_sched_get_priority_max - return maximum RT priority.
 * @policy: scheduling class.
 *
 * Return: On success, this syscall returns the maximum
 * rt_priority that can be used by a given scheduling class.
 * On failure, a negative error code is returned.
 */
SYSCALL_DEFINE1(sched_get_priority_max, पूर्णांक, policy)
{
	पूर्णांक ret = -EINVAL;

	चयन (policy) {
	हाल SCHED_FIFO:
	हाल SCHED_RR:
		ret = MAX_RT_PRIO-1;
		अवरोध;
	हाल SCHED_DEADLINE:
	हाल SCHED_NORMAL:
	हाल SCHED_BATCH:
	हाल SCHED_IDLE:
	हाल SCHED_EXT:
		ret = 0;
		अवरोध;
	}
	वापस ret;
}

/**
 * sys_sched_get_priority_min - return minimum RT priority.
 * @policy: scheduling class.
 *
 * Return: On success, this syscall returns the minimum
 * rt_priority that can be used by a given scheduling class.
 * On failure, a negative error code is returned.
 */
SYSCALL_DEFINE1(sched_get_priority_min, पूर्णांक, policy)
{
	पूर्णांक ret = -EINVAL;

	चयन (policy) {
	हाल SCHED_FIFO:
	हाल SCHED_RR:
		ret = 1;
		अवरोध;
	हाल SCHED_DEADLINE:
	हाल SCHED_NORMAL:
	हाल SCHED_BATCH:
	हाल SCHED_IDLE:
	हाल SCHED_EXT:
		ret = 0;
	}
	वापस ret;
}

अटल पूर्णांक sched_rr_get_interval(pid_t pid, काष्ठा timespec64 *t)
{
	अचिन्हित पूर्णांक time_slice = 0;
	पूर्णांक retval;

	अगर (pid < 0)
		वापस -EINVAL;

	scoped_guard (rcu) {
		काष्ठा task_struct *p = find_process_by_pid(pid);
		अगर (!p)
			वापस -ESRCH;

		retval = security_task_getscheduler(p);
		अगर (retval)
			वापस retval;

		scoped_guard (task_rq_lock, p) {
			काष्ठा rq *rq = scope.rq;
			अगर (p->sched_class->get_rr_interval)
				time_slice = p->sched_class->get_rr_interval(rq, p);
		}
	}

	jiffies_to_timespec64(time_slice, t);
	वापस 0;
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
		काष्ठा __kernel_timespec __user *, interval)
{
	काष्ठा timespec64 t;
	पूर्णांक retval = sched_rr_get_interval(pid, &t);

	अगर (retval == 0)
		retval = put_timespec64(&t, interval);

	वापस retval;
}

#अगर_घोषित CONFIG_COMPAT_32BIT_TIME
SYSCALL_DEFINE2(sched_rr_get_interval_time32, pid_t, pid,
		काष्ठा old_timespec32 __user *, interval)
{
	काष्ठा timespec64 t;
	पूर्णांक retval = sched_rr_get_interval(pid, &t);

	अगर (retval == 0)
		retval = put_old_timespec32(&t, interval);
	वापस retval;
}
#पूर्ण_अगर
