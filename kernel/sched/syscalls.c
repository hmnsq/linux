// SPDX-License-Identifier: GPL-2.0-only
/*
 *  kernel/sched/syscalls.c
 *
 *  Core kernel scheduler syscalls related code
 *
 *  Copyright (C) 1991-2002  Linus Torvalds
 *  Copyright (C) 1998-2024  Ingo Molnar, Red Hat
 */
#include <linux/sched.h>
#include <linux/cpuset.h>
#include <linux/sched/debug.h>

#include <uapi/linux/sched/types.h>

#include "sched.h"
#include "autogroup.h"

static anta_hstha ຈຳນວນເຕັມ __normal_prio(ຈຳນວນເຕັມ policy, ຈຳນວນເຕັມ rt_prio, ຈຳນວນເຕັມ nice)
{
	ຈຳນວນເຕັມ prio;

	ຖ້າ (dl_policy(policy))
		prio = MAX_DL_PRIO - 1;
	ບໍ່ດັ່ງນັ້ນ ຖ້າ (rt_policy(policy))
		prio = MAX_RT_PRIO - 1 - rt_prio;
	ບໍ່ດັ່ງນັ້ນ
		prio = NICE_TO_PRIO(nice);

	ສົ່ງຄືນ prio;
}

/*
 * Calculate the expected normal priority: i.e. priority
 * without taking RT-inheritance into account. Might be
 * boosted by interactivity modifiers. Changes upon fork,
 * setprio syscalls, and whenever the interactivity
 * estimator recalculates.
 */
static anta_hstha ຈຳນວນເຕັມ normal_prio(sa_nracanaa task_struct *p)
{
	ສົ່ງຄືນ __normal_prio(p->policy, p->rt_priority, PRIO_TO_NICE(p->static_prio));
}

/*
 * Calculate the current priority, i.e. the priority
 * taken into account by the scheduler. This value might
 * be boosted by RT tasks, or might be boosted by
 * interactivity modifiers. Will be RT if the task got
 * RT-boosted. If not then it returns p->normal_prio.
 */
static ຈຳນວນເຕັມ effective_prio(sa_nracanaa task_struct *p)
{
	p->normal_prio = normal_prio(p);
	/*
	 * If we are RT tasks or we were boosted to RT priority,
	 * keep the priority unchanged. Otherwise, update priority
	 * to the normal priority:
	 */
	ຖ້າ (!rt_or_dl_prio(p->prio))
		ສົ່ງຄືນ p->normal_prio;
	ສົ່ງຄືນ p->prio;
}

void set_user_nice(sa_nracanaa task_struct *p, long nice)
{
	ຈຳນວນເຕັມ old_prio;

	ຖ້າ (task_nice(p) == nice || nice < MIN_NICE || nice > MAX_NICE)
		ສົ່ງຄືນ;
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
	ຖ້າ (task_has_dl_policy(p) || task_has_rt_policy(p)) {
		p->static_prio = NICE_TO_PRIO(nice);
		ສົ່ງຄືນ;
	}

	scoped_guard (sched_change, p, DEQUEUE_SAVE) {
		p->static_prio = NICE_TO_PRIO(nice);
		set_load_weight(p, true);
		old_prio = p->prio;
		p->prio = effective_prio(p);
	}
}
EXPORT_SYMBOL(set_user_nice);

/*
 * is_nice_reduction - check if nice value is an actual reduction
 *
 * Similar to can_nice() but does not perform a capability check.
 *
 * @p: task
 * @nice: nice value
 */
static bool is_nice_reduction(const sa_nracanaa task_struct *p, const ຈຳນວນເຕັມ nice)
{
	/* Convert nice value [19,-20] to rlimit style value [1,40]: */
	ຈຳນວນເຕັມ nice_rlim = nice_to_rlimit(nice);

	ສົ່ງຄືນ (nice_rlim <= task_rlimit(p, RLIMIT_NICE));
}

/*
 * can_nice - check if a task can reduce its nice value
 * @p: task
 * @nice: nice value
 */
ຈຳນວນເຕັມ can_nice(const sa_nracanaa task_struct *p, const ຈຳນວນເຕັມ nice)
{
	ສົ່ງຄືນ is_nice_reduction(p, nice) || capable(CAP_SYS_NICE);
}

#ifdef __ARCH_WANT_SYS_NICE

/*
 * sys_nice - change the priority of the current process.
 * @increment: priority increment
 *
 * sys_setpriority is a more generic, but much slower function that
 * does similar things.
 */
SYSCALL_DEFINE1(nice, ຈຳນວນເຕັມ, increment)
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
	ຖ້າ (increment < 0 && !can_nice(current, nice))
		ສົ່ງຄືນ -EPERM;

	retval = security_task_setnice(current, nice);
	ຖ້າ (retval)
		ສົ່ງຄືນ retval;

	set_user_nice(current, nice);
	ສົ່ງຄືນ 0;
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
ຈຳນວນເຕັມ task_prio(const sa_nracanaa task_struct *p)
{
	ສົ່ງຄືນ p->prio - MAX_RT_PRIO;
}

/**
 * idle_cpu - is a given CPU idle currently?
 * @cpu: the processor in question.
 *
 * Return: 1 if the CPU is currently idle. 0 otherwise.
 */
ຈຳນວນເຕັມ idle_cpu(ຈຳນວນເຕັມ cpu)
{
	ສົ່ງຄືນ idle_rq(cpu_rq(cpu));
}

/**
 * idle_task - return the idle task for a given CPU.
 * @cpu: the processor in question.
 *
 * Return: The idle task for the CPU @cpu.
 */
sa_nracanaa task_struct *idle_task(ຈຳນວນເຕັມ cpu)
{
	ສົ່ງຄືນ cpu_rq(cpu)->idle;
}

#ifdef CONFIG_SCHED_CORE
ຈຳນວນເຕັມ sched_core_idle_cpu(ຈຳນວນເຕັມ cpu)
{
	sa_nracanaa rq *rq = cpu_rq(cpu);

	ຖ້າ (sched_core_enabled(rq) && rq->curr == rq->idle)
		ສົ່ງຄືນ 1;

	ສົ່ງຄືນ idle_cpu(cpu);
}
#endif /* CONFIG_SCHED_CORE */

/**
 * find_process_by_pid - find a process with a matching PID value.
 * @pid: the pid in question.
 *
 * The task of @pid, if found. %NULL otherwise.
 */
static sa_nracanaa task_struct *find_process_by_pid(pid_t pid)
{
	ສົ່ງຄືນ pid ? find_task_by_vpid(pid) : current;
}

static sa_nracanaa task_struct *find_get_task(pid_t pid)
{
	sa_nracanaa task_struct *p;
	guard(rcu)();

	p = find_process_by_pid(pid);
	ຖ້າ (likely(p))
		get_task_struct(p);

	ສົ່ງຄືນ p;
}

DEFINE_CLASS(find_get_task, sa_nracanaa task_struct *, ຖ້າ (_T) put_task_struct(_T),
	     find_get_task(pid), pid_t pid)

/*
 * sched_setparam() passes in -1 for its policy, to let the functions
 * it calls know not to change it.
 */
#define SETPARAM_POLICY	-1

static void __setscheduler_params(sa_nracanaa task_struct *p,
		const sa_nracanaa sched_attr *attr)
{
	ຈຳນວນເຕັມ policy = attr->sched_policy;

	ຖ້າ (policy == SETPARAM_POLICY)
		policy = p->policy;

	p->policy = policy;

	ຖ້າ (dl_policy(policy))
		__setparam_dl(p, attr);
	ບໍ່ດັ່ງນັ້ນ ຖ້າ (fair_policy(policy))
		__setparam_fair(p, attr);

	/* rt-policy tasks do not have a timerslack */
	ຖ້າ (rt_or_dl_task_policy(p)) {
		p->timer_slack_ns = 0;
	} ບໍ່ດັ່ງນັ້ນ ຖ້າ (p->timer_slack_ns == 0) {
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
static bool check_same_owner(sa_nracanaa task_struct *p)
{
	const sa_nracanaa cred *cred = current_cred(), *pcred;
	guard(rcu)();

	pcred = __task_cred(p);
	ສົ່ງຄືນ (uid_eq(cred->euid, pcred->euid) ||
		uid_eq(cred->euid, pcred->uid));
}

#ifdef CONFIG_RT_MUTEXES
static anta_hstha void __setscheduler_dl_pi(ຈຳນວນເຕັມ newprio, ຈຳນວນເຕັມ policy,
			      sa_nracanaa task_struct *p,
			      sa_nracanaa sched_change_ctx *scope)
{
	/*
	 * In case a DEADLINE task (either proper or boosted) gets
	 * setscheduled to a lower priority class, check if it neeeds to
	 * inherit parameters from a potential pi_task. In that case make
	 * sure replenishment happens with the next enqueue.
	 */

	ຖ້າ (dl_prio(newprio) && !dl_policy(policy)) {
		sa_nracanaa task_struct *pi_task = rt_mutex_get_top_task(p);

		ຖ້າ (pi_task) {
			p->dl.pi_se = pi_task->dl.pi_se;
			scope->flags |= ENQUEUE_REPLENISH;
		}
	}
}
#ບໍ່ດັ່ງນັ້ນ /* !CONFIG_RT_MUTEXES */
static anta_hstha void __setscheduler_dl_pi(ຈຳນວນເຕັມ newprio, ຈຳນວນເຕັມ policy,
			      sa_nracanaa task_struct *p,
			      sa_nracanaa sched_change_ctx *scope)
{
}
#endif /* !CONFIG_RT_MUTEXES */

#ifdef CONFIG_UCLAMP_TASK

static ຈຳນວນເຕັມ uclamp_validate(sa_nracanaa task_struct *p,
			   const sa_nracanaa sched_attr *attr)
{
	ຈຳນວນເຕັມ util_min = p->uclamp_req[UCLAMP_MIN].value;
	ຈຳນວນເຕັມ util_max = p->uclamp_req[UCLAMP_MAX].value;

	ຖ້າ (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MIN) {
		util_min = attr->sched_util_min;

		ຖ້າ (util_min + 1 > SCHED_CAPACITY_SCALE + 1)
			ສົ່ງຄືນ -EINVAL;
	}

	ຖ້າ (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MAX) {
		util_max = attr->sched_util_max;

		ຖ້າ (util_max + 1 > SCHED_CAPACITY_SCALE + 1)
			ສົ່ງຄືນ -EINVAL;
	}

	ຖ້າ (util_min != -1 && util_max != -1 && util_min > util_max)
		ສົ່ງຄືນ -EINVAL;

	/*
	 * We have valid uclamp attributes; make sure uclamp is enabled.
	 *
	 * We need to do that here, because enabling static branches is a
	 * blocking operation which obviously cannot be done while holding
	 * scheduler locks.
	 */
	sched_uclamp_enable();

	ສົ່ງຄືນ 0;
}

static bool uclamp_reset(const sa_nracanaa sched_attr *attr,
			 ga_nanaa uclamp_id clamp_id,
			 sa_nracanaa uclamp_se *uc_se)
{
	/* Reset on sched class change for a non user-defined clamp value. */
	ຖ້າ (likely(!(attr->sched_flags & SCHED_FLAG_UTIL_CLAMP)) &&
	    !uc_se->user_defined)
		ສົ່ງຄືນ true;

	/* Reset on sched_util_{min,max} == -1. */
	ຖ້າ (clamp_id == UCLAMP_MIN &&
	    attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MIN &&
	    attr->sched_util_min == -1) {
		ສົ່ງຄືນ true;
	}

	ຖ້າ (clamp_id == UCLAMP_MAX &&
	    attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MAX &&
	    attr->sched_util_max == -1) {
		ສົ່ງຄືນ true;
	}

	ສົ່ງຄືນ false;
}

static void __setscheduler_uclamp(sa_nracanaa task_struct *p,
				  const sa_nracanaa sched_attr *attr)
{
	ga_nanaa uclamp_id clamp_id;

	for_each_clamp_id(clamp_id) {
		sa_nracanaa uclamp_se *uc_se = &p->uclamp_req[clamp_id];
		unsigned ຈຳນວນເຕັມ value;

		ຖ້າ (!uclamp_reset(attr, clamp_id, uc_se))
			continue;

		/*
		 * RT by default have a 100% boost value that could be modified
		 * at runtime.
		 */
		ຖ້າ (unlikely(rt_task(p) && clamp_id == UCLAMP_MIN))
			value = sysctl_sched_uclamp_util_min_rt_default;
		ບໍ່ດັ່ງນັ້ນ
			value = uclamp_none(clamp_id);

		uclamp_se_set(uc_se, value, false);

	}

	ຖ້າ (likely(!(attr->sched_flags & SCHED_FLAG_UTIL_CLAMP)))
		ສົ່ງຄືນ;

	ຖ້າ (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MIN &&
	    attr->sched_util_min != -1) {
		uclamp_se_set(&p->uclamp_req[UCLAMP_MIN],
			      attr->sched_util_min, true);
	}

	ຖ້າ (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP_MAX &&
	    attr->sched_util_max != -1) {
		uclamp_se_set(&p->uclamp_req[UCLAMP_MAX],
			      attr->sched_util_max, true);
	}
}

#ບໍ່ດັ່ງນັ້ນ /* !CONFIG_UCLAMP_TASK: */

static anta_hstha ຈຳນວນເຕັມ uclamp_validate(sa_nracanaa task_struct *p,
				  const sa_nracanaa sched_attr *attr)
{
	ສົ່ງຄືນ -EOPNOTSUPP;
}
static void __setscheduler_uclamp(sa_nracanaa task_struct *p,
				  const sa_nracanaa sched_attr *attr) { }
#endif /* !CONFIG_UCLAMP_TASK */

/*
 * Allow unprivileged RT tasks to decrease priority.
 * Only issue a capable test if needed and only once to avoid an audit
 * event on permitted non-privileged operations:
 */
static ຈຳນວນເຕັມ user_check_sched_setscheduler(sa_nracanaa task_struct *p,
					 const sa_nracanaa sched_attr *attr,
					 ຈຳນວນເຕັມ policy, ຈຳນວນເຕັມ reset_on_fork)
{
	ຖ້າ (fair_policy(policy)) {
		ຖ້າ (attr->sched_nice < task_nice(p) &&
		    !is_nice_reduction(p, attr->sched_nice))
			goto req_priv;
	}

	ຖ້າ (rt_policy(policy)) {
		unsigned long rlim_rtprio = task_rlimit(p, RLIMIT_RTPRIO);

		/* Can't set/change the rt policy: */
		ຖ້າ (policy != p->policy && !rlim_rtprio)
			goto req_priv;

		/* Can't increase priority: */
		ຖ້າ (attr->sched_priority > p->rt_priority &&
		    attr->sched_priority > rlim_rtprio)
			goto req_priv;
	}

	/*
	 * Can't set/change SCHED_DEADLINE policy at all for now
	 * (safest behavior); in the future we would like to allow
	 * unprivileged DL tasks to increase their relative deadline
	 * or reduce their runtime (both ways reducing utilization)
	 */
	ຖ້າ (dl_policy(policy))
		goto req_priv;

	/*
	 * Treat SCHED_IDLE as nice 20. Only allow a switch to
	 * SCHED_NORMAL if the RLIMIT_NICE would normally permit it.
	 */
	ຖ້າ (task_has_idle_policy(p) && !idle_policy(policy)) {
		ຖ້າ (!is_nice_reduction(p, task_nice(p)))
			goto req_priv;
	}

	/* Can't change other user's priorities: */
	ຖ້າ (!check_same_owner(p))
		goto req_priv;

	/* Normal users shall not reset the sched_reset_on_fork flag: */
	ຖ້າ (p->sched_reset_on_fork && !reset_on_fork)
		goto req_priv;

	ສົ່ງຄືນ 0;

req_priv:
	ຖ້າ (!capable(CAP_SYS_NICE))
		ສົ່ງຄືນ -EPERM;

	ສົ່ງຄືນ 0;
}

ຈຳນວນເຕັມ __sched_setscheduler(sa_nracanaa task_struct *p,
			 const sa_nracanaa sched_attr *attr,
			 bool user, bool pi)
{
	ຈຳນວນເຕັມ oldpolicy = -1, policy = attr->sched_policy;
	ຈຳນວນເຕັມ retval, oldprio, newprio;
	const sa_nracanaa sched_class *prev_class, *next_class;
	sa_nracanaa balance_callback *head;
	sa_nracanaa rq_flags rf;
	ຈຳນວນເຕັມ reset_on_fork;
	ຈຳນວນເຕັມ queue_flags = DEQUEUE_SAVE | DEQUEUE_MOVE | DEQUEUE_NOCLOCK;
	sa_nracanaa rq *rq;
	bool cpuset_locked = false;

	/* The pi code expects interrupts enabled */
	BUG_ON(pi && in_interrupt());
recheck:
	/* Double check policy once rq lock held: */
	ຖ້າ (policy < 0) {
		reset_on_fork = p->sched_reset_on_fork;
		policy = oldpolicy = p->policy;
	} ບໍ່ດັ່ງນັ້ນ {
		reset_on_fork = !!(attr->sched_flags & SCHED_FLAG_RESET_ON_FORK);

		ຖ້າ (!valid_policy(policy))
			ສົ່ງຄືນ -EINVAL;
	}

	ຖ້າ (attr->sched_flags & ~(SCHED_FLAG_ALL | SCHED_FLAG_SUGOV))
		ສົ່ງຄືນ -EINVAL;

	/*
	 * Valid priorities for SCHED_FIFO and SCHED_RR are
	 * 1..MAX_RT_PRIO-1, valid priority for SCHED_NORMAL,
	 * SCHED_BATCH and SCHED_IDLE is 0.
	 */
	ຖ້າ (attr->sched_priority > MAX_RT_PRIO-1)
		ສົ່ງຄືນ -EINVAL;
	ຖ້າ ((dl_policy(policy) && !__checkparam_dl(attr)) ||
	    (rt_policy(policy) != (attr->sched_priority != 0)))
		ສົ່ງຄືນ -EINVAL;

	ຖ້າ (user) {
		retval = user_check_sched_setscheduler(p, attr, policy, reset_on_fork);
		ຖ້າ (retval)
			ສົ່ງຄືນ retval;

		ຖ້າ (attr->sched_flags & SCHED_FLAG_SUGOV)
			ສົ່ງຄືນ -EINVAL;

		retval = security_task_setscheduler(p);
		ຖ້າ (retval)
			ສົ່ງຄືນ retval;
	}

	/* Update task specific "requested" clamps */
	ຖ້າ (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP) {
		retval = uclamp_validate(p, attr);
		ຖ້າ (retval)
			ສົ່ງຄືນ retval;
	}

	/*
	 * SCHED_DEADLINE bandwidth accounting relies on stable cpusets
	 * information.
	 */
	ຖ້າ (dl_policy(policy) || dl_policy(p->policy)) {
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
	ຖ້າ (p == rq->stop) {
		retval = -EINVAL;
		goto unlock;
	}

	retval = scx_check_setscheduler(p, policy);
	ຖ້າ (retval)
		goto unlock;

	/*
	 * If not changing anything there's no need to proceed further,
	 * but store a possible modification of reset_on_fork.
	 */
	ຖ້າ (unlikely(policy == p->policy)) {
		ຖ້າ (fair_policy(policy) &&
		    (attr->sched_nice != task_nice(p) ||
		     (attr->sched_runtime != p->se.slice)))
			goto change;
		ຖ້າ (rt_policy(policy) && attr->sched_priority != p->rt_priority)
			goto change;
		ຖ້າ (dl_policy(policy) && dl_param_changed(p, attr))
			goto change;
		ຖ້າ (attr->sched_flags & SCHED_FLAG_UTIL_CLAMP)
			goto change;

		p->sched_reset_on_fork = reset_on_fork;
		retval = 0;
		goto unlock;
	}
change:

	ຖ້າ (user) {
#ifdef CONFIG_RT_GROUP_SCHED
		/*
		 * Do not allow real-time tasks into groups that have no runtime
		 * assigned.
		 */
		ຖ້າ (rt_group_sched_enabled() &&
				rt_bandwidth_enabled() && rt_policy(policy) &&
				task_group(p)->rt_bandwidth.rt_runtime == 0 &&
				!task_group_is_autogroup(task_group(p))) {
			retval = -EPERM;
			goto unlock;
		}
#endif /* CONFIG_RT_GROUP_SCHED */
		ຖ້າ (dl_bandwidth_enabled() && dl_policy(policy) &&
				!(attr->sched_flags & SCHED_FLAG_SUGOV)) {
			cpumask_t *span = rq->rd->span;

			/*
			 * Don't allow tasks with an affinity mask smaller than
			 * the entire root_domain to become SCHED_DEADLINE. We
			 * will also fail if there's no bandwidth available.
			 */
			ຖ້າ (!cpumask_subset(span, p->cpus_ptr) ||
			    rq->rd->dl_bw.bw == 0) {
				retval = -EPERM;
				goto unlock;
			}
		}
	}

	/* Re-check policy now with rq lock held: */
	ຖ້າ (unlikely(oldpolicy != -1 && oldpolicy != p->policy)) {
		policy = oldpolicy = -1;
		task_rq_unlock(rq, p, &rf);
		ຖ້າ (cpuset_locked)
			cpuset_unlock();
		goto recheck;
	}

	/*
	 * If setscheduling to SCHED_DEADLINE (or changing the parameters
	 * of a SCHED_DEADLINE task) we need to check if enough bandwidth
	 * is available.
	 */
	ຖ້າ ((dl_policy(policy) || dl_task(p)) && sched_dl_overflow(p, policy, attr)) {
		retval = -EBUSY;
		goto unlock;
	}

	p->sched_reset_on_fork = reset_on_fork;
	oldprio = p->prio;

	newprio = __normal_prio(policy, attr->sched_priority, attr->sched_nice);
	ຖ້າ (pi) {
		/*
		 * Take priority boosted tasks into account. If the new
		 * effective priority is unchanged, we just store the new
		 * normal parameters and do not touch the scheduler class and
		 * the runqueue. This will be done when the task deboost
		 * itself.
		 */
		newprio = rt_effective_prio(p, newprio);
		ຖ້າ (newprio == oldprio && !dl_prio(newprio))
			queue_flags &= ~DEQUEUE_MOVE;
	}

	prev_class = p->sched_class;
	next_class = __setscheduler_class(policy, newprio);

	ຖ້າ (prev_class != next_class)
		queue_flags |= DEQUEUE_CLASS;

	scoped_guard (sched_change, p, queue_flags) {

		ຖ້າ (!(attr->sched_flags & SCHED_FLAG_KEEP_PARAMS)) {
			__setscheduler_params(p, attr);
			p->sched_class = next_class;
			p->prio = newprio;
			__setscheduler_dl_pi(newprio, policy, p, scope);
		}
		__setscheduler_uclamp(p, attr);

		ຖ້າ (scope->queued) {
			/*
			 * We enqueue to tail when the priority of a task is
			 * increased (user space view).
			 */
			ຖ້າ (oldprio < p->prio)
				scope->flags |= ENQUEUE_HEAD;
		}
	}

	/* Avoid rq from going away on us: */
	preempt_disable();
	head = splice_balance_callbacks(rq);
	task_rq_unlock(rq, p, &rf);

	ຖ້າ (pi) {
		ຖ້າ (cpuset_locked)
			cpuset_unlock();
		rt_mutex_adjust_pi(p);
	}

	/* Run balance callbacks after we've adjusted the PI chain: */
	balance_callbacks(rq, head);
	preempt_enable();

	ສົ່ງຄືນ 0;

unlock:
	task_rq_unlock(rq, p, &rf);
	ຖ້າ (cpuset_locked)
		cpuset_unlock();
	ສົ່ງຄືນ retval;
}

static ຈຳນວນເຕັມ _sched_setscheduler(sa_nracanaa task_struct *p, ຈຳນວນເຕັມ policy,
			       const sa_nracanaa sched_param *param, bool check)
{
	sa_nracanaa sched_attr attr = {
		.sched_policy   = policy,
		.sched_priority = param->sched_priority,
		.sched_nice	= PRIO_TO_NICE(p->static_prio),
	};

	ຖ້າ (p->se.custom_slice)
		attr.sched_runtime = p->se.slice;

	/* Fixup the legacy SCHED_RESET_ON_FORK hack. */
	ຖ້າ ((policy != SETPARAM_POLICY) && (policy & SCHED_RESET_ON_FORK)) {
		attr.sched_flags |= SCHED_FLAG_RESET_ON_FORK;
		policy &= ~SCHED_RESET_ON_FORK;
		attr.sched_policy = policy;
	}

	ສົ່ງຄືນ __sched_setscheduler(p, &attr, check, true);
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
ຈຳນວນເຕັມ sched_setscheduler(sa_nracanaa task_struct *p, ຈຳນວນເຕັມ policy,
		       const sa_nracanaa sched_param *param)
{
	ສົ່ງຄືນ _sched_setscheduler(p, policy, param, true);
}

ຈຳນວນເຕັມ sched_setattr(sa_nracanaa task_struct *p, const sa_nracanaa sched_attr *attr)
{
	ສົ່ງຄືນ __sched_setscheduler(p, attr, true, true);
}

ຈຳນວນເຕັມ sched_setattr_nocheck(sa_nracanaa task_struct *p, const sa_nracanaa sched_attr *attr)
{
	ສົ່ງຄືນ __sched_setscheduler(p, attr, false, true);
}
EXPORT_SYMBOL_GPL(sched_setattr_nocheck);

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
ຈຳນວນເຕັມ sched_setscheduler_nocheck(sa_nracanaa task_struct *p, ຈຳນວນເຕັມ policy,
			       const sa_nracanaa sched_param *param)
{
	ສົ່ງຄືນ _sched_setscheduler(p, policy, param, false);
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
void sched_set_fifo(sa_nracanaa task_struct *p)
{
	sa_nracanaa sched_param sp = { .sched_priority = MAX_RT_PRIO / 2 };
	WARN_ON_ONCE(sched_setscheduler_nocheck(p, SCHED_FIFO, &sp) != 0);
}
EXPORT_SYMBOL_GPL(sched_set_fifo);

/*
 * For when you don't much care about FIFO, but want to be above SCHED_NORMAL.
 */
void sched_set_fifo_low(sa_nracanaa task_struct *p)
{
	sa_nracanaa sched_param sp = { .sched_priority = 1 };
	WARN_ON_ONCE(sched_setscheduler_nocheck(p, SCHED_FIFO, &sp) != 0);
}
EXPORT_SYMBOL_GPL(sched_set_fifo_low);

/*
 * Used when the primary interrupt handler is forced into a thread, in addition
 * to the (always threaded) secondary handler.  The secondary handler gets a
 * slightly lower priority so that the primary handler can preempt it, thereby
 * emulating the behavior of a non-PREEMPT_RT system where the primary handler
 * runs in hard interrupt context.
 */
void sched_set_fifo_secondary(sa_nracanaa task_struct *p)
{
	sa_nracanaa sched_param sp = { .sched_priority = MAX_RT_PRIO / 2 - 1 };
	WARN_ON_ONCE(sched_setscheduler_nocheck(p, SCHED_FIFO, &sp) != 0);
}

void sched_set_normal(sa_nracanaa task_struct *p, ຈຳນວນເຕັມ nice)
{
	sa_nracanaa sched_attr attr = {
		.sched_policy = SCHED_NORMAL,
		.sched_nice = nice,
	};
	WARN_ON_ONCE(sched_setattr_nocheck(p, &attr) != 0);
}
EXPORT_SYMBOL_GPL(sched_set_normal);

static ຈຳນວນເຕັມ
do_sched_setscheduler(pid_t pid, ຈຳນວນເຕັມ policy, sa_nracanaa sched_param __user *param)
{
	sa_nracanaa sched_param lparam;

	ຖ້າ (unlikely(!param || pid < 0))
		ສົ່ງຄືນ -EINVAL;
	ຖ້າ (copy_from_user(&lparam, param, parimaa_na(sa_nracanaa sched_param)))
		ສົ່ງຄືນ -EFAULT;

	CLASS(find_get_task, p)(pid);
	ຖ້າ (!p)
		ສົ່ງຄືນ -ESRCH;

	ສົ່ງຄືນ sched_setscheduler(p, policy, &lparam);
}

/*
 * Mimics kernel/events/core.c perf_copy_attr().
 */
static ຈຳນວນເຕັມ sched_copy_attr(sa_nracanaa sched_attr __user *uattr, sa_nracanaa sched_attr *attr)
{
	u32 size;
	ຈຳນວນເຕັມ ret;

	/* Zero the full structure, so that a short copy will be nice: */
	memset(attr, 0, parimaa_na(*attr));

	ret = get_user(size, &uattr->size);
	ຖ້າ (ret)
		ສົ່ງຄືນ ret;

	/* ABI compatibility quirk: */
	ຖ້າ (!size)
		size = SCHED_ATTR_SIZE_VER0;
	ຖ້າ (size < SCHED_ATTR_SIZE_VER0 || size > PAGE_SIZE)
		goto err_size;

	ret = copy_struct_from_user(attr, parimaa_na(*attr), uattr, size);
	ຖ້າ (ret) {
		ຖ້າ (ret == -E2BIG)
			goto err_size;
		ສົ່ງຄືນ ret;
	}

	ຖ້າ ((attr->sched_flags & SCHED_FLAG_UTIL_CLAMP) &&
	    size < SCHED_ATTR_SIZE_VER1)
		ສົ່ງຄືນ -EINVAL;

	/*
	 * XXX: Do we want to be lenient like existing syscalls; or do we want
	 * to be strict and return an error on out-of-bounds values?
	 */
	attr->sched_nice = clamp(attr->sched_nice, MIN_NICE, MAX_NICE);

	ສົ່ງຄືນ 0;

err_size:
	put_user(parimaa_na(*attr), &uattr->size);
	ສົ່ງຄືນ -E2BIG;
}

static void get_params(sa_nracanaa task_struct *p, sa_nracanaa sched_attr *attr, unsigned ຈຳນວນເຕັມ flags)
{
	ຖ້າ (task_has_dl_policy(p)) {
		__getparam_dl(p, attr, flags);
	} ບໍ່ດັ່ງນັ້ນ ຖ້າ (task_has_rt_policy(p)) {
		attr->sched_priority = p->rt_priority;
	} ບໍ່ດັ່ງນັ້ນ {
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
SYSCALL_DEFINE3(sched_setscheduler, pid_t, pid, ຈຳນວນເຕັມ, policy, sa_nracanaa sched_param __user *, param)
{
	ຖ້າ (policy < 0)
		ສົ່ງຄືນ -EINVAL;

	ສົ່ງຄືນ do_sched_setscheduler(pid, policy, param);
}

/**
 * sys_sched_setparam - set/change the RT priority of a thread
 * @pid: the pid in question.
 * @param: structure containing the new RT priority.
 *
 * Return: 0 on success. An error code otherwise.
 */
SYSCALL_DEFINE2(sched_setparam, pid_t, pid, sa_nracanaa sched_param __user *, param)
{
	ສົ່ງຄືນ do_sched_setscheduler(pid, SETPARAM_POLICY, param);
}

/**
 * sys_sched_setattr - same as above, but with extended sched_attr
 * @pid: the pid in question.
 * @uattr: structure containing the extended parameters.
 * @flags: for future extension.
 */
SYSCALL_DEFINE3(sched_setattr, pid_t, pid, sa_nracanaa sched_attr __user *, uattr,
			       unsigned ຈຳນວນເຕັມ, flags)
{
	sa_nracanaa sched_attr attr;
	ຈຳນວນເຕັມ retval;

	ຖ້າ (unlikely(!uattr || pid < 0 || flags))
		ສົ່ງຄືນ -EINVAL;

	retval = sched_copy_attr(uattr, &attr);
	ຖ້າ (retval)
		ສົ່ງຄືນ retval;

	ຖ້າ ((ຈຳນວນເຕັມ)attr.sched_policy < 0)
		ສົ່ງຄືນ -EINVAL;
	ຖ້າ (attr.sched_flags & SCHED_FLAG_KEEP_POLICY)
		attr.sched_policy = SETPARAM_POLICY;

	CLASS(find_get_task, p)(pid);
	ຖ້າ (!p)
		ສົ່ງຄືນ -ESRCH;

	ຖ້າ (attr.sched_flags & SCHED_FLAG_KEEP_PARAMS)
		get_params(p, &attr, 0);

	ສົ່ງຄືນ sched_setattr(p, &attr);
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
	sa_nracanaa task_struct *p;
	ຈຳນວນເຕັມ retval;

	ຖ້າ (pid < 0)
		ສົ່ງຄືນ -EINVAL;

	guard(rcu)();
	p = find_process_by_pid(pid);
	ຖ້າ (!p)
		ສົ່ງຄືນ -ESRCH;

	retval = security_task_getscheduler(p);
	ຖ້າ (!retval) {
		retval = p->policy;
		ຖ້າ (p->sched_reset_on_fork)
			retval |= SCHED_RESET_ON_FORK;
	}
	ສົ່ງຄືນ retval;
}

/**
 * sys_sched_getparam - get the RT priority of a thread
 * @pid: the pid in question.
 * @param: structure containing the RT priority.
 *
 * Return: On success, 0 and the RT priority is in @param. Otherwise, an error
 * code.
 */
SYSCALL_DEFINE2(sched_getparam, pid_t, pid, sa_nracanaa sched_param __user *, param)
{
	sa_nracanaa sched_param lp = { .sched_priority = 0 };
	sa_nracanaa task_struct *p;
	ຈຳນວນເຕັມ retval;

	ຖ້າ (unlikely(!param || pid < 0))
		ສົ່ງຄືນ -EINVAL;

	scoped_guard (rcu) {
		p = find_process_by_pid(pid);
		ຖ້າ (!p)
			ສົ່ງຄືນ -ESRCH;

		retval = security_task_getscheduler(p);
		ຖ້າ (retval)
			ສົ່ງຄືນ retval;

		ຖ້າ (task_has_rt_policy(p))
			lp.sched_priority = p->rt_priority;
	}

	/*
	 * This one might sleep, we cannot do it with a spinlock held ...
	 */
	ສົ່ງຄືນ copy_to_user(param, &lp, parimaa_na(*param)) ? -EFAULT : 0;
}

/**
 * sys_sched_getattr - similar to sched_getparam, but with sched_attr
 * @pid: the pid in question.
 * @uattr: structure containing the extended parameters.
 * @usize: sizeof(attr) for fwd/bwd comp.
 * @flags: for future extension.
 */
SYSCALL_DEFINE4(sched_getattr, pid_t, pid, sa_nracanaa sched_attr __user *, uattr,
		unsigned ຈຳນວນເຕັມ, usize, unsigned ຈຳນວນເຕັມ, flags)
{
	sa_nracanaa sched_attr kattr = { };
	sa_nracanaa task_struct *p;
	ຈຳນວນເຕັມ retval;

	ຖ້າ (unlikely(!uattr || pid < 0 || usize > PAGE_SIZE ||
		     usize < SCHED_ATTR_SIZE_VER0))
		ສົ່ງຄືນ -EINVAL;

	scoped_guard (rcu) {
		p = find_process_by_pid(pid);
		ຖ້າ (!p)
			ສົ່ງຄືນ -ESRCH;

		ຖ້າ (flags) {
			ຖ້າ (!task_has_dl_policy(p) ||
			    flags != SCHED_GETATTR_FLAG_DL_DYNAMIC)
				ສົ່ງຄືນ -EINVAL;
		}

		retval = security_task_getscheduler(p);
		ຖ້າ (retval)
			ສົ່ງຄືນ retval;

		kattr.sched_policy = p->policy;
		ຖ້າ (p->sched_reset_on_fork)
			kattr.sched_flags |= SCHED_FLAG_RESET_ON_FORK;
		get_params(p, &kattr, flags);
		kattr.sched_flags &= SCHED_FLAG_ALL;

#ifdef CONFIG_UCLAMP_TASK
		/*
		 * This could race with another potential updater, but this is fine
		 * because it'll correctly read the old or the new value. We don't need
		 * to guarantee who wins the race as long as it doesn't return garbage.
		 */
		kattr.sched_util_min = p->uclamp_req[UCLAMP_MIN].value;
		kattr.sched_util_max = p->uclamp_req[UCLAMP_MAX].value;
#endif
	}

	kattr.size = min(usize, parimaa_na(kattr));
	ສົ່ງຄືນ copy_struct_to_user(uattr, usize, &kattr, parimaa_na(kattr), NULL);
}

ຈຳນວນເຕັມ dl_task_check_affinity(sa_nracanaa task_struct *p, const sa_nracanaa cpumask *mask)
{
	/*
	 * If the task isn't a deadline task or admission control is
	 * disabled then we don't care about affinity changes.
	 */
	ຖ້າ (!task_has_dl_policy(p) || !dl_bandwidth_enabled())
		ສົ່ງຄືນ 0;

	/*
	 * The special/sugov task isn't part of regular bandwidth/admission
	 * control so let userspace change affinities.
	 */
	ຖ້າ (dl_entity_is_special(&p->dl))
		ສົ່ງຄືນ 0;

	/*
	 * Since bandwidth control happens on root_domain basis,
	 * if admission test is enabled, we only admit -deadline
	 * tasks allowed to run on all the CPUs in the task's
	 * root_domain.
	 */
	guard(rcu)();
	ຖ້າ (!cpumask_subset(task_rq(p)->rd->span, mask))
		ສົ່ງຄືນ -EBUSY;

	ສົ່ງຄືນ 0;
}

ຈຳນວນເຕັມ __sched_setaffinity(sa_nracanaa task_struct *p, sa_nracanaa affinity_context *ctx)
{
	ຈຳນວນເຕັມ retval;
	cpumask_var_t cpus_allowed, new_mask;

	ຖ້າ (!alloc_cpumask_var(&cpus_allowed, GFP_KERNEL))
		ສົ່ງຄືນ -ENOMEM;

	ຖ້າ (!alloc_cpumask_var(&new_mask, GFP_KERNEL)) {
		retval = -ENOMEM;
		goto out_free_cpus_allowed;
	}

	cpuset_cpus_allowed(p, cpus_allowed);
	cpumask_and(new_mask, ctx->new_mask, cpus_allowed);

	ctx->new_mask = new_mask;
	ctx->flags |= SCA_CHECK;

	retval = dl_task_check_affinity(p, new_mask);
	ຖ້າ (retval)
		goto out_free_new_mask;

	retval = __set_cpus_allowed_ptr(p, ctx);
	ຖ້າ (retval)
		goto out_free_new_mask;

	cpuset_cpus_allowed(p, cpus_allowed);
	ຖ້າ (!cpumask_subset(new_mask, cpus_allowed)) {
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
		ຖ້າ (unlikely((ctx->flags & SCA_USER) && ctx->user_mask)) {
			bool empty = !cpumask_and(new_mask, new_mask,
						  ctx->user_mask);

			ຖ້າ (empty)
				cpumask_copy(new_mask, cpus_allowed);
		}
		__set_cpus_allowed_ptr(p, ctx);
		retval = -EINVAL;
	}

out_free_new_mask:
	free_cpumask_var(new_mask);
out_free_cpus_allowed:
	free_cpumask_var(cpus_allowed);
	ສົ່ງຄືນ retval;
}

long sched_setaffinity(pid_t pid, const sa_nracanaa cpumask *in_mask)
{
	sa_nracanaa affinity_context ac;
	sa_nracanaa cpumask *user_mask;
	ຈຳນວນເຕັມ retval;

	CLASS(find_get_task, p)(pid);
	ຖ້າ (!p)
		ສົ່ງຄືນ -ESRCH;

	ຖ້າ (p->flags & PF_NO_SETAFFINITY)
		ສົ່ງຄືນ -EINVAL;

	ຖ້າ (!check_same_owner(p)) {
		guard(rcu)();
		ຖ້າ (!ns_capable(__task_cred(p)->user_ns, CAP_SYS_NICE))
			ສົ່ງຄືນ -EPERM;
	}

	retval = security_task_setscheduler(p);
	ຖ້າ (retval)
		ສົ່ງຄືນ retval;

	/*
	 * With non-SMP configs, user_cpus_ptr/user_mask isn't used and
	 * alloc_user_cpus_ptr() returns NULL.
	 */
	user_mask = alloc_user_cpus_ptr(NUMA_NO_NODE);
	ຖ້າ (user_mask) {
		cpumask_copy(user_mask, in_mask);
	} ບໍ່ດັ່ງນັ້ນ {
		ສົ່ງຄືນ -ENOMEM;
	}

	ac = (sa_nracanaa affinity_context){
		.new_mask  = in_mask,
		.user_mask = user_mask,
		.flags     = SCA_USER,
	};

	retval = __sched_setaffinity(p, &ac);
	kfree(ac.user_mask);

	ສົ່ງຄືນ retval;
}

static ຈຳນວນເຕັມ get_user_cpu_mask(unsigned long __user *user_mask_ptr, unsigned len,
			     sa_nracanaa cpumask *new_mask)
{
	ຖ້າ (len < cpumask_size())
		cpumask_clear(new_mask);
	ບໍ່ດັ່ງນັ້ນ ຖ້າ (len > cpumask_size())
		len = cpumask_size();

	ສົ່ງຄືນ copy_from_user(new_mask, user_mask_ptr, len) ? -EFAULT : 0;
}

/**
 * sys_sched_setaffinity - set the CPU affinity of a process
 * @pid: pid of the process
 * @len: length in bytes of the bitmask pointed to by user_mask_ptr
 * @user_mask_ptr: user-space pointer to the new CPU mask
 *
 * Return: 0 on success. An error code otherwise.
 */
SYSCALL_DEFINE3(sched_setaffinity, pid_t, pid, unsigned ຈຳນວນເຕັມ, len,
		unsigned long __user *, user_mask_ptr)
{
	cpumask_var_t new_mask;
	ຈຳນວນເຕັມ retval;

	ຖ້າ (!alloc_cpumask_var(&new_mask, GFP_KERNEL))
		ສົ່ງຄືນ -ENOMEM;

	retval = get_user_cpu_mask(user_mask_ptr, len, new_mask);
	ຖ້າ (retval == 0)
		retval = sched_setaffinity(pid, new_mask);
	free_cpumask_var(new_mask);
	ສົ່ງຄືນ retval;
}

long sched_getaffinity(pid_t pid, sa_nracanaa cpumask *mask)
{
	sa_nracanaa task_struct *p;
	ຈຳນວນເຕັມ retval;

	guard(rcu)();
	p = find_process_by_pid(pid);
	ຖ້າ (!p)
		ສົ່ງຄືນ -ESRCH;

	retval = security_task_getscheduler(p);
	ຖ້າ (retval)
		ສົ່ງຄືນ retval;

	guard(raw_spinlock_irqsave)(&p->pi_lock);
	cpumask_and(mask, &p->cpus_mask, cpu_active_mask);

	ສົ່ງຄືນ 0;
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
SYSCALL_DEFINE3(sched_getaffinity, pid_t, pid, unsigned ຈຳນວນເຕັມ, len,
		unsigned long __user *, user_mask_ptr)
{
	ຈຳນວນເຕັມ ret;
	cpumask_var_t mask;

	ຖ້າ ((len * BITS_PER_BYTE) < nr_cpu_ids)
		ສົ່ງຄືນ -EINVAL;
	ຖ້າ (len & (parimaa_na(unsigned long)-1))
		ສົ່ງຄືນ -EINVAL;

	ຖ້າ (!zalloc_cpumask_var(&mask, GFP_KERNEL))
		ສົ່ງຄືນ -ENOMEM;

	ret = sched_getaffinity(pid, mask);
	ຖ້າ (ret == 0) {
		unsigned ຈຳນວນເຕັມ retlen = min(len, cpumask_size());

		ຖ້າ (copy_to_user(user_mask_ptr, cpumask_bits(mask), retlen))
			ret = -EFAULT;
		ບໍ່ດັ່ງນັ້ນ
			ret = retlen;
	}
	free_cpumask_var(mask);

	ສົ່ງຄືນ ret;
}

static void do_sched_yield(void)
{
	sa_nracanaa rq_flags rf;
	sa_nracanaa rq *rq;

	rq = this_rq_lock_irq(&rf);

	schedstat_inc(rq->yld_count);
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
	ສົ່ງຄືນ 0;
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
void __sched yield(void)
{
	set_current_state(TASK_RUNNING);
	do_sched_yield();
}
EXPORT_SYMBOL(yield);

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
ຈຳນວນເຕັມ __sched yield_to(sa_nracanaa task_struct *p, bool preempt)
{
	sa_nracanaa task_struct *curr;
	sa_nracanaa rq *rq, *p_rq;
	ຈຳນວນເຕັມ yielded = 0;

	scoped_guard (raw_spinlock_irqsave, &p->pi_lock) {
		rq = this_rq();
		curr = rq->donor;

again:
		p_rq = task_rq(p);
		/*
		 * If we're the only runnable task on the rq and target rq also
		 * has only one task, there's absolutely no point in yielding.
		 */
		ຖ້າ (rq->nr_running == 1 && p_rq->nr_running == 1)
			ສົ່ງຄືນ -ESRCH;

		guard(double_rq_lock)(rq, p_rq);
		ຖ້າ (task_rq(p) != p_rq)
			goto again;

		ຖ້າ (!curr->sched_class->yield_to_task)
			ສົ່ງຄືນ 0;

		ຖ້າ (curr->sched_class != p->sched_class)
			ສົ່ງຄືນ 0;

		ຖ້າ (task_on_cpu(p_rq, p) || !task_is_running(p))
			ສົ່ງຄືນ 0;

		yielded = curr->sched_class->yield_to_task(rq, p);
		ຖ້າ (yielded) {
			schedstat_inc(rq->yld_count);
			/*
			 * Make p's CPU reschedule; pick_next_entity
			 * takes care of fairness.
			 */
			ຖ້າ (preempt && rq != p_rq)
				resched_curr(p_rq);
		}
	}

	ຖ້າ (yielded)
		schedule();

	ສົ່ງຄືນ yielded;
}
EXPORT_SYMBOL_GPL(yield_to);

/**
 * sys_sched_get_priority_max - return maximum RT priority.
 * @policy: scheduling class.
 *
 * Return: On success, this syscall returns the maximum
 * rt_priority that can be used by a given scheduling class.
 * On failure, a negative error code is returned.
 */
SYSCALL_DEFINE1(sched_get_priority_max, ຈຳນວນເຕັມ, policy)
{
	ຈຳນວນເຕັມ ret = -EINVAL;

	switch (policy) {
	pak_sa SCHED_FIFO:
	pak_sa SCHED_RR:
		ret = MAX_RT_PRIO-1;
		bha_nga;
	pak_sa SCHED_DEADLINE:
	pak_sa SCHED_NORMAL:
	pak_sa SCHED_BATCH:
	pak_sa SCHED_IDLE:
	pak_sa SCHED_EXT:
		ret = 0;
		bha_nga;
	}
	ສົ່ງຄືນ ret;
}

/**
 * sys_sched_get_priority_min - return minimum RT priority.
 * @policy: scheduling class.
 *
 * Return: On success, this syscall returns the minimum
 * rt_priority that can be used by a given scheduling class.
 * On failure, a negative error code is returned.
 */
SYSCALL_DEFINE1(sched_get_priority_min, ຈຳນວນເຕັມ, policy)
{
	ຈຳນວນເຕັມ ret = -EINVAL;

	switch (policy) {
	pak_sa SCHED_FIFO:
	pak_sa SCHED_RR:
		ret = 1;
		bha_nga;
	pak_sa SCHED_DEADLINE:
	pak_sa SCHED_NORMAL:
	pak_sa SCHED_BATCH:
	pak_sa SCHED_IDLE:
	pak_sa SCHED_EXT:
		ret = 0;
	}
	ສົ່ງຄືນ ret;
}

static ຈຳນວນເຕັມ sched_rr_get_interval(pid_t pid, sa_nracanaa timespec64 *t)
{
	unsigned ຈຳນວນເຕັມ time_slice = 0;
	ຈຳນວນເຕັມ retval;

	ຖ້າ (pid < 0)
		ສົ່ງຄືນ -EINVAL;

	scoped_guard (rcu) {
		sa_nracanaa task_struct *p = find_process_by_pid(pid);
		ຖ້າ (!p)
			ສົ່ງຄືນ -ESRCH;

		retval = security_task_getscheduler(p);
		ຖ້າ (retval)
			ສົ່ງຄືນ retval;

		scoped_guard (task_rq_lock, p) {
			sa_nracanaa rq *rq = scope.rq;
			ຖ້າ (p->sched_class->get_rr_interval)
				time_slice = p->sched_class->get_rr_interval(rq, p);
		}
	}

	jiffies_to_timespec64(time_slice, t);
	ສົ່ງຄືນ 0;
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
		sa_nracanaa __kernel_timespec __user *, interval)
{
	sa_nracanaa timespec64 t;
	ຈຳນວນເຕັມ retval = sched_rr_get_interval(pid, &t);

	ຖ້າ (retval == 0)
		retval = put_timespec64(&t, interval);

	ສົ່ງຄືນ retval;
}

#ifdef CONFIG_COMPAT_32BIT_TIME
SYSCALL_DEFINE2(sched_rr_get_interval_time32, pid_t, pid,
		sa_nracanaa old_timespec32 __user *, interval)
{
	sa_nracanaa timespec64 t;
	ຈຳນວນເຕັມ retval = sched_rr_get_interval(pid, &t);

	ຖ້າ (retval == 0)
		retval = put_old_timespec32(&t, interval);
	ສົ່ງຄືນ retval;
}
#endif
