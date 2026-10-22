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

ساکن اندرونی صحیح __normal_prio(صحیح policy, صحیح rt_prio, صحیح nice)
{
	صحیح prio;

	اگر (dl_policy(policy))
		prio = MAX_DL_PRIO - 1;
	ورنہ اگر (rt_policy(policy))
		prio = MAX_RT_PRIO - 1 - rt_prio;
	ورنہ
		prio = NICE_مقصد_PRIO(nice);

	واپس prio;
}

/*
 * Calculate the expected normal priority: i.e. priority
 * without taking RT-inheritance into account. Might be
 * boosted by interactivity modifiers. Changes upon fork,
 * setprio syscalls, and whenever the interactivity
 * estimator recalculates.
 */
ساکن اندرونی صحیح معمول_prio(ساخت task_struct *p)
{
	واپس __normal_prio(p->policy, p->rt_ترجیح, PRIO_مقصد_NICE(p->static_prio));
}

/*
 * Calculate the current priority, i.e. the priority
 * taken into account by the scheduler. This value might
 * be boosted by RT tasks, or might be boosted by
 * interactivity modifiers. Will be RT if the task got
 * RT-boosted. If not then it returns p->normal_prio.
 */
ساکن صحیح effective_prio(ساخت task_struct *p)
{
	p->معمول_prio = معمول_prio(p);
	/*
	 * If we are RT tasks or we were boosted to RT priority,
	 * keep the priority unchanged. Otherwise, update priority
	 * to the normal priority:
	 */
	اگر (!rt_or_dl_prio(p->prio))
		واپس p->معمول_prio;
	واپس p->prio;
}

خالی سیٹ_user_nice(ساخت task_struct *p, طویل nice)
{
	صحیح old_prio;

	اگر (task_nice(p) == nice || nice < MIN_NICE || nice > MAX_NICE)
		واپس;
	/*
	 * We have to be careful, if called from sys_setpriority(),
	 * the task might be in the middle of scheduling on another CPU.
	 */
	guard(task_rq_تالا_2)(p);

	/*
	 * The RT priorities are set via sched_setscheduler(), but we still
	 * allow the 'normal' nice value to be set - but as expected
	 * it won't have any effect on scheduling until the task is
	 * SCHED_DEADLINE, SCHED_FIFO or SCHED_RR:
	 */
	اگر (task_has_dl_policy(p) || task_has_rt_policy(p)) {
		p->static_prio = NICE_مقصد_PRIO(nice);
		واپس;
	}

	scoped_guard (sched_change, p, قطار_سے_نکالو_SAVE) {
		p->static_prio = NICE_مقصد_PRIO(nice);
		سیٹ_بھرو_وزن(p, true);
		old_prio = p->prio;
		p->prio = effective_prio(p);
	}
}
EXPORT_علامت(سیٹ_user_nice);

/*
 * is_nice_reduction - check if nice value is an actual reduction
 *
 * Similar to can_nice() but does not perform a capability check.
 *
 * @p: task
 * @nice: nice value
 */
ساکن bool is_nice_reduction(ثابت ساخت task_struct *p, ثابت صحیح nice)
{
	/* Convert nice value [19,-20] to rlimit style value [1,40]: */
	صحیح nice_rlim = nice_مقصد_rlimit(nice);

	واپس (nice_rlim <= task_rlimit(p, RLIMIT_NICE));
}

/*
 * can_nice - check if a task can reduce its nice value
 * @p: task
 * @nice: nice value
 */
صحیح can_nice(ثابت ساخت task_struct *p, ثابت صحیح nice)
{
	واپس is_nice_reduction(p, nice) || capable(CAP_SYS_NICE);
}

#اگر_تعریف_شدہ __ARCH_WANT_SYS_NICE

/*
 * sys_nice - change the priority of the current process.
 * @increment: priority increment
 *
 * sys_setpriority is a more generic, but much slower function that
 * does similar things.
 */
SYSCALL_DEFINE1(nice, صحیح, increment)
{
	طویل nice, retval;

	/*
	 * Setpriority might change our priority at the same moment.
	 * We don't have to worry. Conceptually one call occurs first
	 * and we have a single winner.
	 */
	increment = clamp(increment, -NICE_چوڑائی, NICE_چوڑائی);
	nice = task_nice(current) + increment;

	nice = clamp_val(nice, MIN_NICE, MAX_NICE);
	اگر (increment < 0 && !can_nice(current, nice))
		واپس -EPERM;

	retval = security_task_setnice(current, nice);
	اگر (retval)
		واپس retval;

	سیٹ_user_nice(current, nice);
	واپس 0;
}

#اگر_ختم /* __ARCH_WANT_SYS_NICE */

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
صحیح task_prio(ثابت ساخت task_struct *p)
{
	واپس p->prio - MAX_RT_PRIO;
}

/**
 * idle_cpu - is a given CPU idle currently?
 * @cpu: the processor in question.
 *
 * Return: 1 if the CPU is currently idle. 0 otherwise.
 */
صحیح idle_cpu(صحیح cpu)
{
	واپس idle_rq(cpu_rq(cpu));
}

/**
 * idle_task - return the idle task for a given CPU.
 * @cpu: the processor in question.
 *
 * Return: The idle task for the CPU @cpu.
 */
ساخت task_struct *idle_task(صحیح cpu)
{
	واپس cpu_rq(cpu)->idle;
}

#اگر_تعریف_شدہ CONFIG_SCHED_CORE
صحیح sched_core_idle_cpu(صحیح cpu)
{
	ساخت rq *rq = cpu_rq(cpu);

	اگر (sched_core_enabled(rq) && rq->curr == rq->idle)
		واپس 1;

	واپس idle_cpu(cpu);
}
#اگر_ختم /* CONFIG_SCHED_CORE */

/**
 * find_process_by_pid - find a process with a matching PID value.
 * @pid: the pid in question.
 *
 * The task of @pid, if found. %NULL otherwise.
 */
ساکن ساخت task_struct *ڈھونڈو_process_by_pid(pid_t pid)
{
	واپس pid ? ڈھونڈو_task_by_vpid(pid) : current;
}

ساکن ساخت task_struct *ڈھونڈو_get_task(pid_t pid)
{
	ساخت task_struct *p;
	guard(rcu)();

	p = ڈھونڈو_process_by_pid(pid);
	اگر (likely(p))
		get_task_struct(p);

	واپس p;
}

DEFINE_CLASS(ڈھونڈو_get_task, ساخت task_struct *, اگر (_T) put_task_struct(_T),
	     ڈھونڈو_get_task(pid), pid_t pid)

/*
 * sched_setparam() passes in -1 for its policy, to let the functions
 * it calls know not to change it.
 */
#تعریف_کرو SETPARAM_POLICY	-1

ساکن خالی __setscheduler_params(ساخت task_struct *p,
		ثابت ساخت sched_attr *attr)
{
	صحیح policy = attr->sched_policy;

	اگر (policy == SETPARAM_POLICY)
		policy = p->policy;

	p->policy = policy;

	اگر (dl_policy(policy))
		__setparam_dl(p, attr);
	ورنہ اگر (fair_policy(policy))
		__setparam_fair(p, attr);

	/* rt-policy tasks do not have a timerslack */
	اگر (rt_or_dl_task_policy(p)) {
		p->timer_slack_ns = 0;
	} ورنہ اگر (p->timer_slack_ns == 0) {
		/* when switching back to non-rt policy, restore timerslack */
		p->timer_slack_ns = p->default_timer_slack_ns;
	}

	/*
	 * __sched_setscheduler() ensures attr->sched_priority == 0 when
	 * !rt_policy. Always setting this ensures that things like
	 * getparam()/getattr() don't report silly values for !rt tasks.
	 */
	p->rt_ترجیح = attr->sched_ترجیح;
	p->معمول_prio = معمول_prio(p);
	سیٹ_بھرو_وزن(p, true);
}

/*
 * Check the target process has a UID that matches the current process's:
 */
ساکن bool check_same_owner(ساخت task_struct *p)
{
	ثابت ساخت cred *cred = current_cred(), *pcred;
	guard(rcu)();

	pcred = __task_cred(p);
	واپس (uid_eq(cred->euid, pcred->euid) ||
		uid_eq(cred->euid, pcred->uid));
}

#اگر_تعریف_شدہ CONFIG_RT_MUTEXES
ساکن اندرونی خالی __setscheduler_dl_pi(صحیح newprio, صحیح policy,
			      ساخت task_struct *p,
			      ساخت sched_change_ctx *scope)
{
	/*
	 * In case a DEADLINE task (either proper or boosted) gets
	 * setscheduled to a lower priority class, check if it neeeds to
	 * inherit parameters from a potential pi_task. In that case make
	 * sure replenishment happens with the next enqueue.
	 */

	اگر (dl_prio(newprio) && !dl_policy(policy)) {
		ساخت task_struct *pi_task = rt_mutex_get_چوٹی_task(p);

		اگر (pi_task) {
			p->dl.pi_se = pi_task->dl.pi_se;
			scope->flags |= قطار_میں_ڈالو_REPLENISH;
		}
	}
}
#ورنہ /* !CONFIG_RT_MUTEXES */
ساکن اندرونی خالی __setscheduler_dl_pi(صحیح newprio, صحیح policy,
			      ساخت task_struct *p,
			      ساخت sched_change_ctx *scope)
{
}
#اگر_ختم /* !CONFIG_RT_MUTEXES */

#اگر_تعریف_شدہ CONFIG_UCLAMP_TASK

ساکن صحیح uclamp_validate(ساخت task_struct *p,
			   ثابت ساخت sched_attr *attr)
{
	صحیح util_min = p->uclamp_req[UCLAMP_MIN].قدر;
	صحیح util_max = p->uclamp_req[UCLAMP_MAX].قدر;

	اگر (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MIN) {
		util_min = attr->sched_util_min;

		اگر (util_min + 1 > SCHED_استعداد_SCALE + 1)
			واپس -EINVAL;
	}

	اگر (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MAX) {
		util_max = attr->sched_util_max;

		اگر (util_max + 1 > SCHED_استعداد_SCALE + 1)
			واپس -EINVAL;
	}

	اگر (util_min != -1 && util_max != -1 && util_min > util_max)
		واپس -EINVAL;

	/*
	 * We have valid uclamp attributes; make sure uclamp is enabled.
	 *
	 * We need to do that here, because enabling static branches is a
	 * blocking operation which obviously cannot be done while holding
	 * scheduler locks.
	 */
	sched_uclamp_enable();

	واپس 0;
}

ساکن bool uclamp_reset(ثابت ساخت sched_attr *attr,
			 شمار uclamp_id clamp_id,
			 ساخت uclamp_se *uc_se)
{
	/* Reset on sched class change for a non user-defined clamp value. */
	اگر (likely(!(attr->sched_flags & SCHED_FLAG_UTIL_CLAMP)) &&
	    !uc_se->user_defined)
		واپس true;

	/* Reset on sched_util_{min,max} == -1. */
	اگر (clamp_id == UCLAMP_MIN &&
	    attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MIN &&
	    attr->sched_util_min == -1) {
		واپس true;
	}

	اگر (clamp_id == UCLAMP_MAX &&
	    attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MAX &&
	    attr->sched_util_max == -1) {
		واپس true;
	}

	واپس false;
}

ساکن خالی __setscheduler_uclamp(ساخت task_struct *p,
				  ثابت ساخت sched_attr *attr)
{
	شمار uclamp_id clamp_id;

	for_each_clamp_id(clamp_id) {
		ساخت uclamp_se *uc_se = &p->uclamp_req[clamp_id];
		غیرعلامتی صحیح قدر;

		اگر (!uclamp_reset(attr, clamp_id, uc_se))
			جاری;

		/*
		 * RT by default have a 100% boost value that could be modified
		 * at runtime.
		 */
		اگر (unlikely(rt_task(p) && clamp_id == UCLAMP_MIN))
			قدر = sysctl_sched_uclamp_util_min_rt_default;
		ورنہ
			قدر = uclamp_none(clamp_id);

		uclamp_se_سیٹ(uc_se, قدر, false);

	}

	اگر (likely(!(attr->sched_flags & SCHED_FLAG_UTIL_CLAMP)))
		واپس;

	اگر (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MIN &&
	    attr->sched_util_min != -1) {
		uclamp_se_سیٹ(&p->uclamp_req[UCLAMP_MIN],
			      attr->sched_util_min, true);
	}

	اگر (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MAX &&
	    attr->sched_util_max != -1) {
		uclamp_se_سیٹ(&p->uclamp_req[UCLAMP_MAX],
			      attr->sched_util_max, true);
	}
}

#ورنہ /* !CONFIG_UCLAMP_TASK: */

ساکن اندرونی صحیح uclamp_validate(ساخت task_struct *p,
				  ثابت ساخت sched_attr *attr)
{
	واپس -EOPNOTSUPP;
}
ساکن خالی __setscheduler_uclamp(ساخت task_struct *p,
				  ثابت ساخت sched_attr *attr) { }
#اگر_ختم /* !CONFIG_UCLAMP_TASK */

/*
 * Allow unprivileged RT tasks to decrease priority.
 * Only issue a capable test if needed and only once to avoid an audit
 * event on permitted non-privileged operations:
 */
ساکن صحیح user_check_sched_setscheduler(ساخت task_struct *p,
					 ثابت ساخت sched_attr *attr,
					 صحیح policy, صحیح reset_on_fork)
{
	اگر (fair_policy(policy)) {
		اگر (attr->sched_nice < task_nice(p) &&
		    !is_nice_reduction(p, attr->sched_nice))
			جاؤ req_priv;
	}

	اگر (rt_policy(policy)) {
		غیرعلامتی طویل rlim_rtprio = task_rlimit(p, RLIMIT_RTPRIO);

		/* Can't set/change the rt policy: */
		اگر (policy != p->policy && !rlim_rtprio)
			جاؤ req_priv;

		/* Can't increase priority: */
		اگر (attr->sched_ترجیح > p->rt_ترجیح &&
		    attr->sched_ترجیح > rlim_rtprio)
			جاؤ req_priv;
	}

	/*
	 * Can't set/change SCHED_DEADLINE policy at all for now
	 * (safest behavior); in the future we would like to allow
	 * unprivileged DL tasks to increase their relative deadline
	 * or reduce their runtime (both ways reducing utilization)
	 */
	اگر (dl_policy(policy))
		جاؤ req_priv;

	/*
	 * Treat SCHED_IDLE as nice 20. Only allow a switch to
	 * SCHED_NORMAL if the RLIMIT_NICE would normally permit it.
	 */
	اگر (task_has_idle_policy(p) && !idle_policy(policy)) {
		اگر (!is_nice_reduction(p, task_nice(p)))
			جاؤ req_priv;
	}

	/* Can't change other user's priorities: */
	اگر (!check_same_owner(p))
		جاؤ req_priv;

	/* Normal users shall not reset the sched_reset_on_fork flag: */
	اگر (p->sched_reset_on_fork && !reset_on_fork)
		جاؤ req_priv;

	واپس 0;

req_priv:
	اگر (!capable(CAP_SYS_NICE))
		واپس -EPERM;

	واپس 0;
}

صحیح __sched_setscheduler(ساخت task_struct *p,
			 ثابت ساخت sched_attr *attr,
			 bool user, bool pi)
{
	صحیح oldpolicy = -1, policy = attr->sched_policy;
	صحیح retval, oldprio, newprio;
	ثابت ساخت sched_class *پچھلا_class, *اگلا_class;
	ساخت balance_callback *سر;
	ساخت rq_flags rf;
	صحیح reset_on_fork;
	صحیح قطار_flags = قطار_سے_نکالو_SAVE | قطار_سے_نکالو_MOVE | قطار_سے_نکالو_NOCLOCK;
	ساخت rq *rq;
	bool cpuset_locked = false;

	/* The pi code expects interrupts enabled */
	BUG_ON(pi && اندر_مداخلت());
recheck:
	/* Double check policy once rq lock held: */
	اگر (policy < 0) {
		reset_on_fork = p->sched_reset_on_fork;
		policy = oldpolicy = p->policy;
	} ورنہ {
		reset_on_fork = !!(attr->sched_flags & SCHED_FLAG_RESET_ON_FORK);

		اگر (!valid_policy(policy))
			واپس -EINVAL;
	}

	اگر (attr->sched_flags & ~(SCHED_FLAG_ALL | SCHED_FLAG_SUGOV))
		واپس -EINVAL;

	/*
	 * Valid priorities for SCHED_FIFO and SCHED_RR are
	 * 1..MAX_RT_PRIO-1, valid priority for SCHED_NORMAL,
	 * SCHED_BATCH and SCHED_IDLE is 0.
	 */
	اگر (attr->sched_ترجیح > MAX_RT_PRIO-1)
		واپس -EINVAL;
	اگر ((dl_policy(policy) && !__checkparam_dl(attr)) ||
	    (rt_policy(policy) != (attr->sched_ترجیح != 0)))
		واپس -EINVAL;

	اگر (user) {
		retval = user_check_sched_setscheduler(p, attr, policy, reset_on_fork);
		اگر (retval)
			واپس retval;

		اگر (attr->sched_flags & SCHED_FLAG_SUGOV)
			واپس -EINVAL;

		retval = security_task_setscheduler(p);
		اگر (retval)
			واپس retval;
	}

	/* Update task specific "requested" clamps */
	اگر (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP) {
		retval = uclamp_validate(p, attr);
		اگر (retval)
			واپس retval;
	}

	/*
	 * SCHED_DEADLINE bandwidth accounting relies on stable cpusets
	 * information.
	 */
	اگر (dl_policy(policy) || dl_policy(p->policy)) {
		cpuset_locked = true;
		cpuset_تالا();
	}

	/*
	 * Make sure no PI-waiters arrive (or leave) while we are
	 * changing the priority of the task:
	 *
	 * To be able to change p->policy safely, the appropriate
	 * runqueue lock must be held.
	 */
	rq = task_rq_تالا_2(p, &rf);
	update_rq_clock(rq);

	/*
	 * Changing the policy of the stop threads its a very bad idea:
	 */
	اگر (p == rq->stop) {
		retval = -EINVAL;
		جاؤ unlock;
	}

	retval = scx_check_setscheduler(p, policy);
	اگر (retval)
		جاؤ unlock;

	/*
	 * If not changing anything there's no need to proceed further,
	 * but store a possible modification of reset_on_fork.
	 */
	اگر (unlikely(policy == p->policy)) {
		اگر (fair_policy(policy) &&
		    (attr->sched_nice != task_nice(p) ||
		     (attr->sched_runtime != p->se.قاش)))
			جاؤ change;
		اگر (rt_policy(policy) && attr->sched_ترجیح != p->rt_ترجیح)
			جاؤ change;
		اگر (dl_policy(policy) && dl_param_بدلا(p, attr))
			جاؤ change;
		اگر (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP)
			جاؤ change;

		p->sched_reset_on_fork = reset_on_fork;
		retval = 0;
		جاؤ unlock;
	}
change:

	اگر (user) {
#اگر_تعریف_شدہ CONFIG_RT_گروہ_SCHED
		/*
		 * Do not allow real-time tasks into groups that have no runtime
		 * assigned.
		 */
		اگر (rt_گروہ_sched_enabled() &&
				rt_bandwidth_enabled() && rt_policy(policy) &&
				task_گروہ(p)->rt_bandwidth.rt_runtime == 0 &&
				!task_گروہ_is_autogroup(task_گروہ(p))) {
			retval = -EPERM;
			جاؤ unlock;
		}
#اگر_ختم /* CONFIG_RT_GROUP_SCHED */
		اگر (dl_bandwidth_enabled() && dl_policy(policy) &&
				!(attr->sched_flags & SCHED_FLAG_SUGOV)) {
			cpumask_t *وسعت = rq->rd->وسعت;

			/*
			 * Don't allow tasks with an affinity mask smaller than
			 * the entire root_domain to become SCHED_DEADLINE. We
			 * will also fail if there's no bandwidth available.
			 */
			اگر (!cpumask_subset(وسعت, p->cpus_ptr) ||
			    rq->rd->dl_bw.bw == 0) {
				retval = -EPERM;
				جاؤ unlock;
			}
		}
	}

	/* Re-check policy now with rq lock held: */
	اگر (unlikely(oldpolicy != -1 && oldpolicy != p->policy)) {
		policy = oldpolicy = -1;
		task_rq_unlock(rq, p, &rf);
		اگر (cpuset_locked)
			cpuset_unlock();
		جاؤ recheck;
	}

	/*
	 * If setscheduling to SCHED_DEADLINE (or changing the parameters
	 * of a SCHED_DEADLINE task) we need to check if enough bandwidth
	 * is available.
	 */
	اگر ((dl_policy(policy) || dl_task(p)) && sched_dl_حد_سے_تجاوز(p, policy, attr)) {
		retval = -EBUSY;
		جاؤ unlock;
	}

	p->sched_reset_on_fork = reset_on_fork;
	oldprio = p->prio;

	newprio = __normal_prio(policy, attr->sched_ترجیح, attr->sched_nice);
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
			قطار_flags &= ~قطار_سے_نکالو_MOVE;
	}

	پچھلا_class = p->sched_class;
	اگلا_class = __setscheduler_class(policy, newprio);

	اگر (پچھلا_class != اگلا_class)
		قطار_flags |= قطار_سے_نکالو_CLASS;

	scoped_guard (sched_change, p, قطار_flags) {

		اگر (!(attr->sched_flags & SCHED_FLAG_KEEP_PARAMS)) {
			__setscheduler_params(p, attr);
			p->sched_class = اگلا_class;
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
				scope->flags |= قطار_میں_ڈالو_سر;
		}
	}

	/* Avoid rq from going away on us: */
	preempt_disable();
	سر = splice_balance_callbacks(rq);
	task_rq_unlock(rq, p, &rf);

	اگر (pi) {
		اگر (cpuset_locked)
			cpuset_unlock();
		rt_mutex_adjust_pi(p);
	}

	/* Run balance callbacks after we've adjusted the PI chain: */
	balance_callbacks(rq, سر);
	preempt_enable();

	واپس 0;

unlock:
	task_rq_unlock(rq, p, &rf);
	اگر (cpuset_locked)
		cpuset_unlock();
	واپس retval;
}

ساکن صحیح _sched_setscheduler(ساخت task_struct *p, صحیح policy,
			       ثابت ساخت sched_param *param, bool check)
{
	ساخت sched_attr attr = {
		.sched_policy   = policy,
		.sched_ترجیح = param->sched_ترجیح,
		.sched_nice	= PRIO_مقصد_NICE(p->static_prio),
	};

	اگر (p->se.custom_قاش)
		attr.sched_runtime = p->se.قاش;

	/* Fixup the legacy SCHED_RESET_ON_FORK hack. */
	اگر ((policy != SETPARAM_POLICY) && (policy & SCHED_RESET_ON_FORK)) {
		attr.sched_flags |= SCHED_FLAG_RESET_ON_FORK;
		policy &= ~SCHED_RESET_ON_FORK;
		attr.sched_policy = policy;
	}

	واپس __sched_setscheduler(p, &attr, check, true);
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
صحیح sched_setscheduler(ساخت task_struct *p, صحیح policy,
		       ثابت ساخت sched_param *param)
{
	واپس _sched_setscheduler(p, policy, param, true);
}

صحیح sched_setattr(ساخت task_struct *p, ثابت ساخت sched_attr *attr)
{
	واپس __sched_setscheduler(p, attr, true, true);
}

صحیح sched_setattr_nocheck(ساخت task_struct *p, ثابت ساخت sched_attr *attr)
{
	واپس __sched_setscheduler(p, attr, false, true);
}
EXPORT_علامت_GPL(sched_setattr_nocheck);

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
صحیح sched_setscheduler_nocheck(ساخت task_struct *p, صحیح policy,
			       ثابت ساخت sched_param *param)
{
	واپس _sched_setscheduler(p, policy, param, false);
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
خالی sched_سیٹ_fifo(ساخت task_struct *p)
{
	ساخت sched_param sp = { .sched_ترجیح = MAX_RT_PRIO / 2 };
	WARN_ON_ONCE(sched_setscheduler_nocheck(p, SCHED_FIFO, &sp) != 0);
}
EXPORT_علامت_GPL(sched_سیٹ_fifo);

/*
 * For when you don't much care about FIFO, but want to be above SCHED_NORMAL.
 */
خالی sched_سیٹ_fifo_کم(ساخت task_struct *p)
{
	ساخت sched_param sp = { .sched_ترجیح = 1 };
	WARN_ON_ONCE(sched_setscheduler_nocheck(p, SCHED_FIFO, &sp) != 0);
}
EXPORT_علامت_GPL(sched_سیٹ_fifo_کم);

/*
 * Used when the primary interrupt handler is forced into a thread, in addition
 * to the (always threaded) secondary handler.  The secondary handler gets a
 * slightly lower priority so that the primary handler can preempt it, thereby
 * emulating the behavior of a non-PREEMPT_RT system where the primary handler
 * runs in hard interrupt context.
 */
خالی sched_سیٹ_fifo_secondary(ساخت task_struct *p)
{
	ساخت sched_param sp = { .sched_ترجیح = MAX_RT_PRIO / 2 - 1 };
	WARN_ON_ONCE(sched_setscheduler_nocheck(p, SCHED_FIFO, &sp) != 0);
}

خالی sched_سیٹ_معمول(ساخت task_struct *p, صحیح nice)
{
	ساخت sched_attr attr = {
		.sched_policy = SCHED_معمول,
		.sched_nice = nice,
	};
	WARN_ON_ONCE(sched_setattr_nocheck(p, &attr) != 0);
}
EXPORT_علامت_GPL(sched_سیٹ_معمول);

ساکن صحیح
do_sched_setscheduler(pid_t pid, صحیح policy, ساخت sched_param __user *param)
{
	ساخت sched_param lparam;

	اگر (unlikely(!param || pid < 0))
		واپس -EINVAL;
	اگر (copy_from_user(&lparam, param, حجم(ساخت sched_param)))
		واپس -EFAULT;

	CLASS(ڈھونڈو_get_task, p)(pid);
	اگر (!p)
		واپس -ESRCH;

	واپس sched_setscheduler(p, policy, &lparam);
}

/*
 * Mimics kernel/events/core.c perf_copy_attr().
 */
ساکن صحیح sched_copy_attr(ساخت sched_attr __user *uattr, ساخت sched_attr *attr)
{
	u32 سائز;
	صحیح ret;

	/* Zero the full structure, so that a short copy will be nice: */
	memset(attr, 0, حجم(*attr));

	ret = get_user(سائز, &uattr->سائز);
	اگر (ret)
		واپس ret;

	/* ABI compatibility quirk: */
	اگر (!سائز)
		سائز = SCHED_ATTR_سائز_VER0;
	اگر (سائز < SCHED_ATTR_سائز_VER0 || سائز > صفحہ_سائز)
		جاؤ err_سائز;

	ret = copy_struct_from_user(attr, حجم(*attr), uattr, سائز);
	اگر (ret) {
		اگر (ret == -E2BIG)
			جاؤ err_سائز;
		واپس ret;
	}

	اگر ((attr->sched_flags & SCHED_FLAG_UTIL_CLAMP) &&
	    سائز < SCHED_ATTR_سائز_VER1)
		واپس -EINVAL;

	/*
	 * XXX: Do we want to be lenient like existing syscalls; or do we want
	 * to be strict and return an error on out-of-bounds values?
	 */
	attr->sched_nice = clamp(attr->sched_nice, MIN_NICE, MAX_NICE);

	واپس 0;

err_سائز:
	put_user(حجم(*attr), &uattr->سائز);
	واپس -E2BIG;
}

ساکن خالی get_params(ساخت task_struct *p, ساخت sched_attr *attr, غیرعلامتی صحیح flags)
{
	اگر (task_has_dl_policy(p)) {
		__getparam_dl(p, attr, flags);
	} ورنہ اگر (task_has_rt_policy(p)) {
		attr->sched_ترجیح = p->rt_ترجیح;
	} ورنہ {
		attr->sched_nice = task_nice(p);
		attr->sched_runtime = p->se.قاش;
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
SYSCALL_DEFINE3(sched_setscheduler, pid_t, pid, صحیح, policy, ساخت sched_param __user *, param)
{
	اگر (policy < 0)
		واپس -EINVAL;

	واپس do_sched_setscheduler(pid, policy, param);
}

/**
 * sys_sched_setparam - set/change the RT priority of a thread
 * @pid: the pid in question.
 * @param: structure containing the new RT priority.
 *
 * Return: 0 on success. An error code otherwise.
 */
SYSCALL_DEFINE2(sched_setparam, pid_t, pid, ساخت sched_param __user *, param)
{
	واپس do_sched_setscheduler(pid, SETPARAM_POLICY, param);
}

/**
 * sys_sched_setattr - same as above, but with extended sched_attr
 * @pid: the pid in question.
 * @uattr: structure containing the extended parameters.
 * @flags: for future extension.
 */
SYSCALL_DEFINE3(sched_setattr, pid_t, pid, ساخت sched_attr __user *, uattr,
			       غیرعلامتی صحیح, flags)
{
	ساخت sched_attr attr;
	صحیح retval;

	اگر (unlikely(!uattr || pid < 0 || flags))
		واپس -EINVAL;

	retval = sched_copy_attr(uattr, &attr);
	اگر (retval)
		واپس retval;

	اگر ((صحیح)attr.sched_policy < 0)
		واپس -EINVAL;
	اگر (attr.sched_flags & SCHED_FLAG_KEEP_POLICY)
		attr.sched_policy = SETPARAM_POLICY;

	CLASS(ڈھونڈو_get_task, p)(pid);
	اگر (!p)
		واپس -ESRCH;

	اگر (attr.sched_flags & SCHED_FLAG_KEEP_PARAMS)
		get_params(p, &attr, 0);

	واپس sched_setattr(p, &attr);
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
	ساخت task_struct *p;
	صحیح retval;

	اگر (pid < 0)
		واپس -EINVAL;

	guard(rcu)();
	p = ڈھونڈو_process_by_pid(pid);
	اگر (!p)
		واپس -ESRCH;

	retval = security_task_getscheduler(p);
	اگر (!retval) {
		retval = p->policy;
		اگر (p->sched_reset_on_fork)
			retval |= SCHED_RESET_ON_FORK;
	}
	واپس retval;
}

/**
 * sys_sched_getparam - get the RT priority of a thread
 * @pid: the pid in question.
 * @param: structure containing the RT priority.
 *
 * Return: On success, 0 and the RT priority is in @param. Otherwise, an error
 * code.
 */
SYSCALL_DEFINE2(sched_getparam, pid_t, pid, ساخت sched_param __user *, param)
{
	ساخت sched_param lp = { .sched_ترجیح = 0 };
	ساخت task_struct *p;
	صحیح retval;

	اگر (unlikely(!param || pid < 0))
		واپس -EINVAL;

	scoped_guard (rcu) {
		p = ڈھونڈو_process_by_pid(pid);
		اگر (!p)
			واپس -ESRCH;

		retval = security_task_getscheduler(p);
		اگر (retval)
			واپس retval;

		اگر (task_has_rt_policy(p))
			lp.sched_ترجیح = p->rt_ترجیح;
	}

	/*
	 * This one might sleep, we cannot do it with a spinlock held ...
	 */
	واپس copy_مقصد_user(param, &lp, حجم(*param)) ? -EFAULT : 0;
}

/**
 * sys_sched_getattr - similar to sched_getparam, but with sched_attr
 * @pid: the pid in question.
 * @uattr: structure containing the extended parameters.
 * @usize: sizeof(attr) for fwd/bwd comp.
 * @flags: for future extension.
 */
SYSCALL_DEFINE4(sched_getattr, pid_t, pid, ساخت sched_attr __user *, uattr,
		غیرعلامتی صحیح, usize, غیرعلامتی صحیح, flags)
{
	ساخت sched_attr kattr = { };
	ساخت task_struct *p;
	صحیح retval;

	اگر (unlikely(!uattr || pid < 0 || usize > صفحہ_سائز ||
		     usize < SCHED_ATTR_سائز_VER0))
		واپس -EINVAL;

	scoped_guard (rcu) {
		p = ڈھونڈو_process_by_pid(pid);
		اگر (!p)
			واپس -ESRCH;

		اگر (flags) {
			اگر (!task_has_dl_policy(p) ||
			    flags != SCHED_GETATTR_FLAG_DL_DYNAMIC)
				واپس -EINVAL;
		}

		retval = security_task_getscheduler(p);
		اگر (retval)
			واپس retval;

		kattr.sched_policy = p->policy;
		اگر (p->sched_reset_on_fork)
			kattr.sched_flags |= SCHED_FLAG_RESET_ON_FORK;
		get_params(p, &kattr, flags);
		kattr.sched_flags &= SCHED_FLAG_ALL;

#اگر_تعریف_شدہ CONFIG_UCLAMP_TASK
		/*
		 * This could race with another potential updater, but this is fine
		 * because it'll correctly read the old or the new value. We don't need
		 * to guarantee who wins the race as long as it doesn't return garbage.
		 */
		kattr.sched_util_min = p->uclamp_req[UCLAMP_MIN].قدر;
		kattr.sched_util_max = p->uclamp_req[UCLAMP_MAX].قدر;
#اگر_ختم
	}

	kattr.سائز = min(usize, حجم(kattr));
	واپس copy_struct_مقصد_user(uattr, usize, &kattr, حجم(kattr), تہی);
}

صحیح dl_task_check_affinity(ساخت task_struct *p, ثابت ساخت cpumask *نقاب)
{
	/*
	 * If the task isn't a deadline task or admission control is
	 * disabled then we don't care about affinity changes.
	 */
	اگر (!task_has_dl_policy(p) || !dl_bandwidth_enabled())
		واپس 0;

	/*
	 * The special/sugov task isn't part of regular bandwidth/admission
	 * control so let userspace change affinities.
	 */
	اگر (dl_entity_is_special(&p->dl))
		واپس 0;

	/*
	 * Since bandwidth control happens on root_domain basis,
	 * if admission test is enabled, we only admit -deadline
	 * tasks allowed to run on all the CPUs in the task's
	 * root_domain.
	 */
	guard(rcu)();
	اگر (!cpumask_subset(task_rq(p)->rd->وسعت, نقاب))
		واپس -EBUSY;

	واپس 0;
}

صحیح __sched_setaffinity(ساخت task_struct *p, ساخت affinity_context *ctx)
{
	صحیح retval;
	cpumask_var_t cpus_allowed, new_نقاب;

	اگر (!alloc_cpumask_var(&cpus_allowed, GFP_KERNEL))
		واپس -ENOMEM;

	اگر (!alloc_cpumask_var(&new_نقاب, GFP_KERNEL)) {
		retval = -ENOMEM;
		جاؤ برآمد_free_cpus_allowed;
	}

	cpuset_cpus_allowed(p, cpus_allowed);
	cpumask_and(new_نقاب, ctx->new_نقاب, cpus_allowed);

	ctx->new_نقاب = new_نقاب;
	ctx->flags |= SCA_CHECK;

	retval = dl_task_check_affinity(p, new_نقاب);
	اگر (retval)
		جاؤ برآمد_free_new_نقاب;

	retval = __set_cpus_allowed_ptr(p, ctx);
	اگر (retval)
		جاؤ برآمد_free_new_نقاب;

	cpuset_cpus_allowed(p, cpus_allowed);
	اگر (!cpumask_subset(new_نقاب, cpus_allowed)) {
		/*
		 * We must have raced with a concurrent cpuset update.
		 * Just reset the cpumask to the cpuset's cpus_allowed.
		 */
		cpumask_copy(new_نقاب, cpus_allowed);

		/*
		 * If SCA_USER is set, a 2nd call to __set_cpus_allowed_ptr()
		 * will restore the previous user_cpus_ptr value.
		 *
		 * In the unlikely event a previous user_cpus_ptr exists,
		 * we need to further restrict the mask to what is allowed
		 * by that old user_cpus_ptr.
		 */
		اگر (unlikely((ctx->flags & SCA_USER) && ctx->user_نقاب)) {
			bool خالی_خانے = !cpumask_and(new_نقاب, new_نقاب,
						  ctx->user_نقاب);

			اگر (خالی_خانے)
				cpumask_copy(new_نقاب, cpus_allowed);
		}
		__set_cpus_allowed_ptr(p, ctx);
		retval = -EINVAL;
	}

برآمد_free_new_نقاب:
	free_cpumask_var(new_نقاب);
برآمد_free_cpus_allowed:
	free_cpumask_var(cpus_allowed);
	واپس retval;
}

طویل sched_setaffinity(pid_t pid, ثابت ساخت cpumask *اندر_نقاب)
{
	ساخت affinity_context ac;
	ساخت cpumask *user_نقاب;
	صحیح retval;

	CLASS(ڈھونڈو_get_task, p)(pid);
	اگر (!p)
		واپس -ESRCH;

	اگر (p->flags & PF_NO_SETAFFINITY)
		واپس -EINVAL;

	اگر (!check_same_owner(p)) {
		guard(rcu)();
		اگر (!ns_capable(__task_cred(p)->user_ns, CAP_SYS_NICE))
			واپس -EPERM;
	}

	retval = security_task_setscheduler(p);
	اگر (retval)
		واپس retval;

	/*
	 * With non-SMP configs, user_cpus_ptr/user_mask isn't used and
	 * alloc_user_cpus_ptr() returns NULL.
	 */
	user_نقاب = alloc_user_cpus_ptr(NUMA_NO_گرہ);
	اگر (user_نقاب) {
		cpumask_copy(user_نقاب, اندر_نقاب);
	} ورنہ {
		واپس -ENOMEM;
	}

	ac = (ساخت affinity_context){
		.new_نقاب  = اندر_نقاب,
		.user_نقاب = user_نقاب,
		.flags     = SCA_USER,
	};

	retval = __sched_setaffinity(p, &ac);
	kfree(ac.user_نقاب);

	واپس retval;
}

ساکن صحیح get_user_cpu_نقاب(غیرعلامتی طویل __user *user_نقاب_ptr, غیرعلامتی len,
			     ساخت cpumask *new_نقاب)
{
	اگر (len < cpumask_سائز())
		cpumask_clear(new_نقاب);
	ورنہ اگر (len > cpumask_سائز())
		len = cpumask_سائز();

	واپس copy_from_user(new_نقاب, user_نقاب_ptr, len) ? -EFAULT : 0;
}

/**
 * sys_sched_setaffinity - set the CPU affinity of a process
 * @pid: pid of the process
 * @len: length in bytes of the bitmask pointed to by user_mask_ptr
 * @user_mask_ptr: user-space pointer to the new CPU mask
 *
 * Return: 0 on success. An error code otherwise.
 */
SYSCALL_DEFINE3(sched_setaffinity, pid_t, pid, غیرعلامتی صحیح, len,
		غیرعلامتی طویل __user *, user_نقاب_ptr)
{
	cpumask_var_t new_نقاب;
	صحیح retval;

	اگر (!alloc_cpumask_var(&new_نقاب, GFP_KERNEL))
		واپس -ENOMEM;

	retval = get_user_cpu_نقاب(user_نقاب_ptr, len, new_نقاب);
	اگر (retval == 0)
		retval = sched_setaffinity(pid, new_نقاب);
	free_cpumask_var(new_نقاب);
	واپس retval;
}

طویل sched_getaffinity(pid_t pid, ساخت cpumask *نقاب)
{
	ساخت task_struct *p;
	صحیح retval;

	guard(rcu)();
	p = ڈھونڈو_process_by_pid(pid);
	اگر (!p)
		واپس -ESRCH;

	retval = security_task_getscheduler(p);
	اگر (retval)
		واپس retval;

	guard(raw_spinlock_irqsave)(&p->pi_تالا);
	cpumask_and(نقاب, &p->cpus_نقاب, cpu_فعال_نقاب);

	واپس 0;
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
SYSCALL_DEFINE3(sched_getaffinity, pid_t, pid, غیرعلامتی صحیح, len,
		غیرعلامتی طویل __user *, user_نقاب_ptr)
{
	صحیح ret;
	cpumask_var_t نقاب;

	اگر ((len * بٹ_فی_BYTE) < nr_cpu_ids)
		واپس -EINVAL;
	اگر (len & (حجم(غیرعلامتی طویل)-1))
		واپس -EINVAL;

	اگر (!zalloc_cpumask_var(&نقاب, GFP_KERNEL))
		واپس -ENOMEM;

	ret = sched_getaffinity(pid, نقاب);
	اگر (ret == 0) {
		غیرعلامتی صحیح retlen = min(len, cpumask_سائز());

		اگر (copy_مقصد_user(user_نقاب_ptr, cpumask_بٹ(نقاب), retlen))
			ret = -EFAULT;
		ورنہ
			ret = retlen;
	}
	free_cpumask_var(نقاب);

	واپس ret;
}

ساکن خالی do_sched_yield(خالی)
{
	ساخت rq_flags rf;
	ساخت rq *rq;

	rq = this_rq_تالا_irq(&rf);

	schedstat_inc(rq->yld_گنتی);
	rq->donor->sched_class->yield_task(rq);

	preempt_disable();
	rq_unlock_irq(rq, &rf);
	sched_preempt_enable_no_resched();

	جدول_وقت();
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
	واپس 0;
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
خالی __sched yield(خالی)
{
	سیٹ_current_state(TASK_RUNNING);
	do_sched_yield();
}
EXPORT_علامت(yield);

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
صحیح __sched yield_مقصد(ساخت task_struct *p, bool preempt)
{
	ساخت task_struct *curr;
	ساخت rq *rq, *p_rq;
	صحیح yielded = 0;

	scoped_guard (raw_spinlock_irqsave, &p->pi_تالا) {
		rq = this_rq();
		curr = rq->donor;

again:
		p_rq = task_rq(p);
		/*
		 * If we're the only runnable task on the rq and target rq also
		 * has only one task, there's absolutely no point in yielding.
		 */
		اگر (rq->nr_running == 1 && p_rq->nr_running == 1)
			واپس -ESRCH;

		guard(double_rq_تالا)(rq, p_rq);
		اگر (task_rq(p) != p_rq)
			جاؤ again;

		اگر (!curr->sched_class->yield_مقصد_task)
			واپس 0;

		اگر (curr->sched_class != p->sched_class)
			واپس 0;

		اگر (task_on_cpu(p_rq, p) || !task_is_running(p))
			واپس 0;

		yielded = curr->sched_class->yield_مقصد_task(rq, p);
		اگر (yielded) {
			schedstat_inc(rq->yld_گنتی);
			/*
			 * Make p's CPU reschedule; pick_next_entity
			 * takes care of fairness.
			 */
			اگر (preempt && rq != p_rq)
				resched_curr(p_rq);
		}
	}

	اگر (yielded)
		جدول_وقت();

	واپس yielded;
}
EXPORT_علامت_GPL(yield_مقصد);

/**
 * sys_sched_get_priority_max - return maximum RT priority.
 * @policy: scheduling class.
 *
 * Return: On success, this syscall returns the maximum
 * rt_priority that can be used by a given scheduling class.
 * On failure, a negative error code is returned.
 */
SYSCALL_DEFINE1(sched_get_ترجیح_max, صحیح, policy)
{
	صحیح ret = -EINVAL;

	انتخاب (policy) {
	حالت SCHED_FIFO:
	حالت SCHED_RR:
		ret = MAX_RT_PRIO-1;
		توڑو;
	حالت SCHED_DEADLINE:
	حالت SCHED_معمول:
	حالت SCHED_BATCH:
	حالت SCHED_IDLE:
	حالت SCHED_EXT:
		ret = 0;
		توڑو;
	}
	واپس ret;
}

/**
 * sys_sched_get_priority_min - return minimum RT priority.
 * @policy: scheduling class.
 *
 * Return: On success, this syscall returns the minimum
 * rt_priority that can be used by a given scheduling class.
 * On failure, a negative error code is returned.
 */
SYSCALL_DEFINE1(sched_get_ترجیح_min, صحیح, policy)
{
	صحیح ret = -EINVAL;

	انتخاب (policy) {
	حالت SCHED_FIFO:
	حالت SCHED_RR:
		ret = 1;
		توڑو;
	حالت SCHED_DEADLINE:
	حالت SCHED_معمول:
	حالت SCHED_BATCH:
	حالت SCHED_IDLE:
	حالت SCHED_EXT:
		ret = 0;
	}
	واپس ret;
}

ساکن صحیح sched_rr_get_interval(pid_t pid, ساخت timespec64 *t)
{
	غیرعلامتی صحیح وقت_قاش = 0;
	صحیح retval;

	اگر (pid < 0)
		واپس -EINVAL;

	scoped_guard (rcu) {
		ساخت task_struct *p = ڈھونڈو_process_by_pid(pid);
		اگر (!p)
			واپس -ESRCH;

		retval = security_task_getscheduler(p);
		اگر (retval)
			واپس retval;

		scoped_guard (task_rq_تالا_2, p) {
			ساخت rq *rq = scope.rq;
			اگر (p->sched_class->get_rr_interval)
				وقت_قاش = p->sched_class->get_rr_interval(rq, p);
		}
	}

	jiffies_مقصد_timespec64(وقت_قاش, t);
	واپس 0;
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
		ساخت __kernel_timespec __user *, interval)
{
	ساخت timespec64 t;
	صحیح retval = sched_rr_get_interval(pid, &t);

	اگر (retval == 0)
		retval = put_timespec64(&t, interval);

	واپس retval;
}

#اگر_تعریف_شدہ CONFIG_COMPAT_32_ثنائی_ہندسہ_وقت
SYSCALL_DEFINE2(sched_rr_get_interval_time32, pid_t, pid,
		ساخت old_timespec32 __user *, interval)
{
	ساخت timespec64 t;
	صحیح retval = sched_rr_get_interval(pid, &t);

	اگر (retval == 0)
		retval = put_old_timespec32(&t, interval);
	واپس retval;
}
#اگر_ختم
