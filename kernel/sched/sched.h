/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Scheduler internal types and methods:
 */
#ifndef _KERNEL_SCHED_SCHED_H
#define _KERNEL_SCHED_SCHED_H

#അന്തര്ഭാവ <linux/prandom.h>
#അന്തര്ഭാവ <linux/sched/affinity.h>
#അന്തര്ഭാവ <linux/sched/autogroup.h>
#അന്തര്ഭാവ <linux/sched/cpufreq.h>
#അന്തര്ഭാവ <linux/sched/deadline.h>
#അന്തര്ഭാവ <linux/sched.h>
#അന്തര്ഭാവ <linux/sched/loadavg.h>
#അന്തര്ഭാവ <linux/sched/mm.h>
#അന്തര്ഭാവ <linux/sched/rseq_api.h>
#അന്തര്ഭാവ <linux/sched/signal.h>
#അന്തര്ഭാവ <linux/sched/smt.h>
#അന്തര്ഭാവ <linux/sched/stat.h>
#അന്തര്ഭാവ <linux/sched/sysctl.h>
#അന്തര്ഭാവ <linux/sched/task_flags.h>
#അന്തര്ഭാവ <linux/sched/task.h>
#അന്തര്ഭാവ <linux/sched/topology.h>
#അന്തര്ഭാവ <linux/atomic.h>
#അന്തര്ഭാവ <linux/bitmap.h>
#അന്തര്ഭാവ <linux/bug.h>
#അന്തര്ഭാവ <linux/capability.h>
#അന്തര്ഭാവ <linux/cgroup_api.h>
#അന്തര്ഭാവ <linux/cgroup.h>
#അന്തര്ഭാവ <linux/context_tracking.h>
#അന്തര്ഭാവ <linux/cpufreq.h>
#അന്തര്ഭാവ <linux/cpumask_api.h>
#അന്തര്ഭാവ <linux/cpuset.h>
#അന്തര്ഭാവ <linux/ctype.h>
#അന്തര്ഭാവ <linux/file.h>
#അന്തര്ഭാവ <linux/fs_api.h>
#അന്തര്ഭാവ <linux/hrtimer_api.h>
#അന്തര്ഭാവ <linux/interrupt.h>
#അന്തര്ഭാവ <linux/irq_work.h>
#അന്തര്ഭാവ <linux/jiffies.h>
#അന്തര്ഭാവ <linux/kref_api.h>
#അന്തര്ഭാവ <linux/kthread.h>
#അന്തര്ഭാവ <linux/ktime_api.h>
#അന്തര്ഭാവ <linux/lockdep_api.h>
#അന്തര്ഭാവ <linux/lockdep.h>
#അന്തര്ഭാവ <linux/memblock.h>
#അന്തര്ഭാവ <linux/memcontrol.h>
#അന്തര്ഭാവ <linux/minmax.h>
#അന്തര്ഭാവ <linux/mm.h>
#അന്തര്ഭാവ <linux/module.h>
#അന്തര്ഭാവ <linux/mutex_api.h>
#അന്തര്ഭാവ <linux/plist.h>
#അന്തര്ഭാവ <linux/poll.h>
#അന്തര്ഭാവ <linux/proc_fs.h>
#അന്തര്ഭാവ <linux/profile.h>
#അന്തര്ഭാവ <linux/psi.h>
#അന്തര്ഭാവ <linux/rcupdate.h>
#അന്തര്ഭാവ <linux/seq_file.h>
#അന്തര്ഭാവ <linux/seqlock.h>
#അന്തര്ഭാവ <linux/softirq.h>
#അന്തര്ഭാവ <linux/spinlock_api.h>
#അന്തര്ഭാവ <linux/static_key.h>
#അന്തര്ഭാവ <linux/stop_machine.h>
#അന്തര്ഭാവ <linux/syscalls_api.h>
#അന്തര്ഭാവ <linux/syscalls.h>
#അന്തര്ഭാവ <linux/tick.h>
#അന്തര്ഭാവ <linux/topology.h>
#അന്തര്ഭാവ <linux/types.h>
#അന്തര്ഭാവ <linux/u64_stats_sync_api.h>
#അന്തര്ഭാവ <linux/uaccess.h>
#അന്തര്ഭാവ <linux/vmstat.h>
#അന്തര്ഭാവ <linux/wait_api.h>
#അന്തര്ഭാവ <linux/wait_bit.h>
#അന്തര്ഭാവ <linux/workqueue_api.h>
#അന്തര്ഭാവ <linux/delayacct.h>
#അന്തര്ഭാവ <linux/mmu_context.h>

#അന്തര്ഭാവ <trace/events/power.h>
#അന്തര്ഭാവ <trace/events/sched.h>

#അന്തര്ഭാവ "../workqueue_internal.h"

ഘടന rq;
ഘടന cfs_rq;
ഘടന rt_rq;
ഘടന sched_group;
ഘടന cpuidle_state;

#എങ്കിൽ defined(CONFIG_PARAVIRT) && !defined(CONFIG_HAVE_PV_STEAL_CLOCK_GEN)
# include <asm/paravirt.h>
#endif

#അന്തര്ഭാവ <asm/barrier.h>

#അന്തര്ഭാവ "cpupri.h"
#അന്തര്ഭാവ "cpudeadline.h"

/* task_struct::on_rq states: */
#define TASK_ON_RQ_QUEUED	1
#define TASK_ON_RQ_MIGRATING	2

ബാഹ്യ __read_mostly പൂർണ്ണസംഖ്യ scheduler_running;

ബാഹ്യ unsigned long calc_load_update;
ബാഹ്യ atomic_long_t calc_load_tasks;

ബാഹ്യ ശൂന്യം calc_global_load_tick(ഘടന rq *this_rq);
ബാഹ്യ long calc_load_fold_active(ഘടന rq *this_rq, long adjust);

ബാഹ്യ ശൂന്യം call_trace_sched_update_nr_running(ഘടന rq *rq, പൂർണ്ണസംഖ്യ എണ്ണം);

ബാഹ്യ പൂർണ്ണസംഖ്യ sysctl_sched_rt_period;
ബാഹ്യ പൂർണ്ണസംഖ്യ sysctl_sched_rt_runtime;
ബാഹ്യ പൂർണ്ണസംഖ്യ sched_rr_timeslice;

/*
 * Asymmetric CPU capacity bits
 */
ഘടന asym_cap_data {
	ഘടന list_head link;
	ഘടന rcu_head rcu;
	unsigned long capacity;
	unsigned long cpus[];
};

ബാഹ്യ ഘടന list_head asym_cap_list;

#define cpu_capacity_span(asym_data) to_cpumask((asym_data)->cpus)

/*
 * Helpers for converting nanosecond timing to jiffy resolution
 */
#define NS_TO_JIFFIES(time)	((unsigned long)(time) / (NSEC_PER_SEC/HZ))

/*
 * Increase resolution of nice-level calculations for 64-bit architectures.
 * The extra resolution improves shares distribution and load balancing of
 * low-weight task groups (eg. nice +19 on an autogroup), deeper task-group
 * hierarchies, especially on larger systems. This is not a user-visible change
 * and does not change the user-interface for setting shares/weights.
 *
 * We increase resolution only if we have enough bits to allow this increased
 * resolution (i.e. 64-bit). The costs for increasing resolution when 32-bit
 * are pretty high and the returns do not justify the increased costs.
 *
 * Really only required when CONFIG_FAIR_GROUP_SCHED=y is also set, but to
 * increase coverage and consistency always enable it on 64-bit platforms.
 */
#ifdef CONFIG_64BIT
# define NICE_0_LOAD_SHIFT	(SCHED_FIXEDPOINT_SHIFT + SCHED_FIXEDPOINT_SHIFT)
# define scale_load(w)		((w) << SCHED_FIXEDPOINT_SHIFT)
# define scale_load_down(w)					\
({								\
	unsigned long __w = (w);				\
								\
	എങ്കിൽ (__w)						\
		__w = max(2UL, __w >> SCHED_FIXEDPOINT_SHIFT);	\
	__w;							\
})
#അല്ലെങ്കിൽ
# define NICE_0_LOAD_SHIFT	(SCHED_FIXEDPOINT_SHIFT)
# define scale_load(w)		(w)
# define scale_load_down(w)	(w)
#endif

/*
 * Task weight (visible to users) and its load (invisible to users) have
 * independent resolution, but they should be well calibrated. We use
 * scale_load() and scale_load_down(w) to convert between them. The
 * following must be true:
 *
 *  scale_load(sched_prio_to_weight[NICE_TO_PRIO(0)-MAX_RT_PRIO]) == NICE_0_LOAD
 *
 */
#define NICE_0_LOAD		(1L << NICE_0_LOAD_SHIFT)

/*
 * Single value that decides SCHED_DEADLINE internal math precision.
 * 10 -> just above 1us
 * 9  -> just above 0.5us
 */
#define DL_SCALE		10

/*
 * Single value that denotes runtime == period, ie unlimited time.
 */
#define RUNTIME_INF		((u64)~0ULL)

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ idle_policy(പൂർണ്ണസംഖ്യ policy)
{
	തിരികെ policy == SCHED_IDLE;
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ normal_policy(പൂർണ്ണസംഖ്യ policy)
{
#ifdef CONFIG_SCHED_CLASS_EXT
	എങ്കിൽ (policy == SCHED_EXT)
		തിരികെ true;
#endif
	തിരികെ policy == SCHED_NORMAL;
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ fair_policy(പൂർണ്ണസംഖ്യ policy)
{
	തിരികെ normal_policy(policy) || policy == SCHED_BATCH;
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ rt_policy(പൂർണ്ണസംഖ്യ policy)
{
	തിരികെ policy == SCHED_FIFO || policy == SCHED_RR;
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ dl_policy(പൂർണ്ണസംഖ്യ policy)
{
	തിരികെ policy == SCHED_DEADLINE;
}

സ്ഥിതം വരിയിൽ bool valid_policy(പൂർണ്ണസംഖ്യ policy)
{
	തിരികെ idle_policy(policy) || fair_policy(policy) ||
		rt_policy(policy) || dl_policy(policy);
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ task_has_idle_policy(ഘടന task_struct *p)
{
	തിരികെ idle_policy(p->policy);
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ task_has_rt_policy(ഘടന task_struct *p)
{
	തിരികെ rt_policy(p->policy);
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ task_has_dl_policy(ഘടന task_struct *p)
{
	തിരികെ dl_policy(p->policy);
}

#define cap_scale(v, s)		((v)*(s) >> SCHED_CAPACITY_SHIFT)

സ്ഥിതം വരിയിൽ ശൂന്യം update_avg(u64 *avg, u64 sample)
{
	s64 diff = sample - *avg;

	*avg += diff / 8;
}

/*
 * Shifting a value by an exponent greater *or equal* to the size of said value
 * is UB; cap at size-1.
 */
#define shr_bound(val, shift)							\
	(val >> min_t(typeof(shift), shift, BITS_PER_TYPE(typeof(val)) - 1))

/*
 * cgroup weight knobs should use the common MIN, DFL and MAX values which are
 * 1, 100 and 10000 respectively. While it loses a bit of range on both ends, it
 * maps pretty well onto the shares value used by scheduler and the round-trip
 * conversions preserve the original value over the entire range.
 */
സ്ഥിതം വരിയിൽ unsigned long sched_weight_from_cgroup(unsigned long cgrp_weight)
{
	തിരികെ DIV_ROUND_CLOSEST_ULL(cgrp_weight * 1024, CGROUP_WEIGHT_DFL);
}

സ്ഥിതം വരിയിൽ unsigned long sched_weight_to_cgroup(unsigned long weight)
{
	തിരികെ clamp_t(unsigned long,
		       DIV_ROUND_CLOSEST_ULL(weight * CGROUP_WEIGHT_DFL, 1024),
		       CGROUP_WEIGHT_MIN, CGROUP_WEIGHT_MAX);
}

/*
 * !! For sched_setattr_nocheck() (kernel) only !!
 *
 * This is actually gross. :(
 *
 * It is used to make schedutil kworker(s) higher priority than SCHED_DEADLINE
 * tasks, but still be able to sleep. We need this on platforms that cannot
 * atomically change clock frequency. Remove once fast switching will be
 * available on such platforms.
 *
 * SUGOV stands for SchedUtil GOVernor.
 */
#define SCHED_FLAG_SUGOV	0x10000000

#define SCHED_DL_FLAGS		(SCHED_FLAG_RECLAIM | SCHED_FLAG_DL_OVERRUN | SCHED_FLAG_SUGOV)

സ്ഥിതം വരിയിൽ bool dl_entity_is_special(സ്ഥിരം ഘടന sched_dl_entity *dl_se)
{
#ifdef CONFIG_CPU_FREQ_GOV_SCHEDUTIL
	തിരികെ unlikely(dl_se->flags & SCHED_FLAG_SUGOV);
#അല്ലെങ്കിൽ
	തിരികെ false;
#endif
}

/*
 * Tells if entity @a should preempt entity @b.
 */
സ്ഥിതം വരിയിൽ bool dl_entity_preempt(സ്ഥിരം ഘടന sched_dl_entity *a,
				     സ്ഥിരം ഘടന sched_dl_entity *b)
{
	തിരികെ dl_entity_is_special(a) ||
	       dl_time_before(a->deadline, b->deadline);
}

/*
 * This is the priority-queue data structure of the RT scheduling class:
 */
ഘടന rt_prio_array {
	DECLARE_BITMAP(bitmap, MAX_RT_PRIO+1); /* include 1 bit for delimiter */
	ഘടന list_head queue[MAX_RT_PRIO];
};

ഘടന rt_bandwidth {
	/* nests inside the rq lock: */
	raw_spinlock_t		rt_runtime_lock;
	ktime_t			rt_period;
	u64			rt_runtime;
	ഘടന hrtimer		rt_period_timer;
	unsigned പൂർണ്ണസംഖ്യ		rt_period_active;
};

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ dl_bandwidth_enabled(ശൂന്യം)
{
	തിരികെ sysctl_sched_rt_runtime >= 0;
}

/*
 * To keep the bandwidth of -deadline tasks under control
 * we need some place where:
 *  - store the maximum -deadline bandwidth of each cpu;
 *  - cache the fraction of bandwidth that is currently allocated in
 *    each root domain;
 *
 * This is all done in the data structure below. It is similar to the
 * one used for RT-throttling (rt_bandwidth), with the main difference
 * that, since here we are only interested in admission control, we
 * do not decrease any runtime while the group "executes", neither we
 * need a timer to replenish it.
 *
 * With respect to SMP, bandwidth is given on a per root domain basis,
 * meaning that:
 *  - bw (< 100%) is the deadline bandwidth of each CPU;
 *  - total_bw is the currently allocated bandwidth in each root domain;
 */
ഘടന dl_bw {
	raw_spinlock_t		lock;
	u64			bw;
	u64			total_bw;
};

ബാഹ്യ ശൂന്യം init_dl_bw(ഘടന dl_bw *dl_b);
ബാഹ്യ പൂർണ്ണസംഖ്യ  sched_dl_global_validate(ശൂന്യം);
ബാഹ്യ ശൂന്യം sched_dl_do_global(ശൂന്യം);
ബാഹ്യ പൂർണ്ണസംഖ്യ  sched_dl_overflow(ഘടന task_struct *p, പൂർണ്ണസംഖ്യ policy, സ്ഥിരം ഘടന sched_attr *attr);
ബാഹ്യ ശൂന്യം __setparam_dl(ഘടന task_struct *p, സ്ഥിരം ഘടന sched_attr *attr);
ബാഹ്യ ശൂന്യം __getparam_dl(ഘടന task_struct *p, ഘടന sched_attr *attr, unsigned പൂർണ്ണസംഖ്യ flags);
ബാഹ്യ bool __checkparam_dl(സ്ഥിരം ഘടന sched_attr *attr);
ബാഹ്യ bool dl_param_changed(ഘടന task_struct *p, സ്ഥിരം ഘടന sched_attr *attr);
ബാഹ്യ പൂർണ്ണസംഖ്യ  dl_cpuset_cpumask_can_shrink(സ്ഥിരം ഘടന cpumask *cur, സ്ഥിരം ഘടന cpumask *trial);
ബാഹ്യ പൂർണ്ണസംഖ്യ  dl_bw_deactivate(പൂർണ്ണസംഖ്യ cpu);
ബാഹ്യ s64 dl_scaled_delta_exec(ഘടന rq *rq, ഘടന sched_dl_entity *dl_se, s64 delta_exec);
/*
 * SCHED_DEADLINE supports servers (nested scheduling) with the following
 * interface:
 *
 *   dl_se::rq -- runqueue we belong to.
 *
 *   dl_se::server_pick() -- nested pick_next_task(); we yield the period if this
 *                           returns NULL.
 *
 *   dl_server_update() -- called from update_curr_common(), propagates runtime
 *                         to the server.
 *
 *   dl_server_start() -- start the server when it has tasks; it will stop
 *			  automatically when there are no more tasks, per
 *			  dl_se::server_pick() returning NULL.
 *
 *   dl_server_stop() -- (force) stop the server; use when updating
 *                       parameters.
 *
 *   dl_server_init() -- initializes the server.
 *
 * When started the dl_server will (per dl_defer) schedule a timer for its
 * zero-laxity point -- that is, unlike regular EDF tasks which run ASAP, a
 * server will run at the very end of its period.
 *
 * This is done such that any runtime from the target class can be accounted
 * against the server -- through dl_server_update() above -- such that when it
 * becomes time to run, it might already be out of runtime and get deferred
 * until the next period. In this case dl_server_timer() will alternate
 * between defer and replenish but never actually enqueue the server.
 *
 * Only when the target class does not manage to exhaust the server's runtime
 * (there's actualy starvation in the given period), will the dl_server get on
 * the runqueue. Once queued it will pick tasks from the target class and run
 * them until either its runtime is exhaused, at which point its back to
 * dl_server_timer, or until there are no more tasks to run, at which point
 * the dl_server stops itself.
 *
 * By stopping at this point the dl_server retains bandwidth, which, if a new
 * task wakes up imminently (starting the server again), can be used --
 * subject to CBS wakeup rules -- without having to wait for the next period.
 *
 * Additionally, because of the dl_defer behaviour the start/stop behaviour is
 * naturally thottled to once per period, avoiding high context switch
 * workloads from spamming the hrtimer program/cancel paths.
 */
ബാഹ്യ ശൂന്യം dl_server_update_idle(ഘടന sched_dl_entity *dl_se, s64 delta_exec);
ബാഹ്യ ശൂന്യം dl_server_update(ഘടന sched_dl_entity *dl_se, s64 delta_exec);
ബാഹ്യ ശൂന്യം dl_server_start(ഘടന sched_dl_entity *dl_se);
ബാഹ്യ ശൂന്യം dl_server_stop(ഘടന sched_dl_entity *dl_se);
ബാഹ്യ ശൂന്യം dl_server_init(ഘടന sched_dl_entity *dl_se, ഘടന rq *rq,
		    dl_server_pick_f pick_task);
ബാഹ്യ ശൂന്യം sched_init_dl_servers(ശൂന്യം);

ബാഹ്യ ശൂന്യം fair_server_init(ഘടന rq *rq);
ബാഹ്യ ശൂന്യം ext_server_init(ഘടന rq *rq);
ബാഹ്യ ശൂന്യം __dl_server_attach_root(ഘടന sched_dl_entity *dl_se, ഘടന rq *rq);
ബാഹ്യ പൂർണ്ണസംഖ്യ dl_server_apply_params(ഘടന sched_dl_entity *dl_se,
		    u64 runtime, u64 period, bool init);
ബാഹ്യ പൂർണ്ണസംഖ്യ dl_server_attach_bw(ഘടന sched_dl_entity *dl_se);
ബാഹ്യ ശൂന്യം dl_server_detach_bw(ഘടന sched_dl_entity *dl_se);
ബാഹ്യ പൂർണ്ണസംഖ്യ dl_server_swap_bw(ഘടന sched_dl_entity *detach_se,
			     ഘടന sched_dl_entity *attach_se);

സ്ഥിതം വരിയിൽ bool dl_server_active(ഘടന sched_dl_entity *dl_se)
{
	തിരികെ dl_se->dl_server_active;
}

#ifdef CONFIG_CGROUP_SCHED

ബാഹ്യ ഘടന list_head task_groups;

#ifdef CONFIG_GROUP_SCHED_BANDWIDTH
ബാഹ്യ സ്ഥിരം u64 max_bw_quota_period_us;

/*
 * default period for group bandwidth.
 * default: 0.1s, units: microseconds
 */
സ്ഥിതം വരിയിൽ u64 default_bw_period_us(ശൂന്യം)
{
	തിരികെ 100000ULL;
}
#endif /* CONFIG_GROUP_SCHED_BANDWIDTH */

ഘടന cfs_bandwidth {
#ifdef CONFIG_CFS_BANDWIDTH
	raw_spinlock_t		lock;
	ktime_t			period;
	u64			quota;
	u64			runtime;
	u64			burst;
	u64			runtime_snap;
	s64			hierarchical_quota;

	u8			idle;
	u8			period_active;
	u8			slack_started;
	ഘടന hrtimer		period_timer;
	ഘടന hrtimer		slack_timer;
	ഘടന list_head	throttled_cfs_rq;

	/* Statistics: */
	പൂർണ്ണസംഖ്യ			nr_periods;
	പൂർണ്ണസംഖ്യ			nr_throttled;
	പൂർണ്ണസംഖ്യ			nr_burst;
	u64			throttled_time;
	u64			burst_time;
#endif /* CONFIG_CFS_BANDWIDTH */
};

/* Task group related information */
ഘടന task_group {
	ഘടന cgroup_subsys_state css;

#ifdef CONFIG_GROUP_SCHED_WEIGHT
	/* A positive value indicates that this is a SCHED_IDLE group. */
	പൂർണ്ണസംഖ്യ			idle;
#endif

#ifdef CONFIG_FAIR_GROUP_SCHED
	/* runqueue "owned" by this group on each CPU */
	ഘടന cfs_rq __percpu	*cfs_rq;
	unsigned long		shares;
	/*
	 * load_avg can be heavily contended at clock tick time, so put
	 * it in its own cache-line separated from the fields above which
	 * will also be accessed at each tick.
	 */
	atomic_long_t		load_avg ____cacheline_aligned;
	atomic_long_t		runnable_avg;

#endif /* CONFIG_FAIR_GROUP_SCHED */

#ifdef CONFIG_RT_GROUP_SCHED
	ഘടന sched_rt_entity	**rt_se;
	ഘടന rt_rq		**rt_rq;

	ഘടന rt_bandwidth	rt_bandwidth;
#endif

	ഘടന scx_task_group	scx;

	ഘടന rcu_head		rcu;
	ഘടന list_head	list;

	ഘടന task_group	*parent;
	ഘടന list_head	siblings;
	ഘടന list_head	children;

#ifdef CONFIG_SCHED_AUTOGROUP
	ഘടന autogroup	*autogroup;
#endif

	ഘടന cfs_bandwidth	cfs_bandwidth;

#ifdef CONFIG_UCLAMP_TASK_GROUP
	/* The two decimal precision [%] value requested from user-space */
	unsigned പൂർണ്ണസംഖ്യ		uclamp_pct[UCLAMP_CNT];
	/* Clamp values requested for a task group */
	ഘടന uclamp_se	uclamp_req[UCLAMP_CNT];
	/* Effective clamp values used for a task group */
	ഘടന uclamp_se	uclamp[UCLAMP_CNT];
#endif

};

#define വേര്_TASK_GROUP_LOAD	NICE_0_LOAD

തരനിർവചനം പൂർണ്ണസംഖ്യ (*tg_visitor)(ഘടന task_group *, ശൂന്യം *);

ബാഹ്യ പൂർണ്ണസംഖ്യ walk_tg_വൃക്ഷം_from(ഘടന task_group *from,
			     tg_visitor down, tg_visitor up, ശൂന്യം *data);

/*
 * Iterate the full tree, calling @down when first entering a node and @up when
 * leaving it for the final time.
 *
 * Caller must hold rcu_lock or sufficient equivalent.
 */
സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ walk_tg_വൃക്ഷം(tg_visitor down, tg_visitor up, ശൂന്യം *data)
{
	തിരികെ walk_tg_വൃക്ഷം_from(&വേര്_task_group, down, up, data);
}

സ്ഥിതം വരിയിൽ ഘടന task_group *css_tg(ഘടന cgroup_subsys_state *css)
{
	തിരികെ css ? container_of(css, ഘടന task_group, css) : ശൂന്യ;
}

ബാഹ്യ പൂർണ്ണസംഖ്യ tg_nop(ഘടന task_group *tg, ശൂന്യം *data);

#ifdef CONFIG_FAIR_GROUP_SCHED
ബാഹ്യ ശൂന്യം free_fair_sched_group(ഘടന task_group *tg);
ബാഹ്യ പൂർണ്ണസംഖ്യ alloc_fair_sched_group(ഘടന task_group *tg, ഘടന task_group *parent);
ബാഹ്യ ശൂന്യം online_fair_sched_group(ഘടന task_group *tg);
ബാഹ്യ ശൂന്യം unregister_fair_sched_group(ഘടന task_group *tg);
ബാഹ്യ ശൂന്യം __sched_cgroup_mode_update(പൂർണ്ണസംഖ്യ mode);
#അല്ലെങ്കിൽ /* !CONFIG_FAIR_GROUP_SCHED: */
സ്ഥിതം വരിയിൽ ശൂന്യം free_fair_sched_group(ഘടന task_group *tg) { }
സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ alloc_fair_sched_group(ഘടന task_group *tg, ഘടന task_group *parent)
{
       തിരികെ 1;
}
സ്ഥിതം വരിയിൽ ശൂന്യം online_fair_sched_group(ഘടന task_group *tg) { }
സ്ഥിതം വരിയിൽ ശൂന്യം unregister_fair_sched_group(ഘടന task_group *tg) { }
#endif /* !CONFIG_FAIR_GROUP_SCHED */

ബാഹ്യ ശൂന്യം init_tg_cfs_entry(ഘടന task_group *tg, ഘടന cfs_rq *cfs_rq,
			ഘടന sched_entity *se, പൂർണ്ണസംഖ്യ cpu,
			ഘടന sched_entity *parent);
ബാഹ്യ ശൂന്യം init_cfs_bandwidth(ഘടന cfs_bandwidth *cfs_b, ഘടന cfs_bandwidth *parent);

ബാഹ്യ ശൂന്യം __refill_cfs_bandwidth_runtime(ഘടന cfs_bandwidth *cfs_b);
ബാഹ്യ ശൂന്യം start_cfs_bandwidth(ഘടന cfs_bandwidth *cfs_b);
ബാഹ്യ ശൂന്യം unthrottle_cfs_rq(ഘടന cfs_rq *cfs_rq);
ബാഹ്യ bool cfs_task_bw_constrained(ഘടന task_struct *p);

ബാഹ്യ ശൂന്യം init_tg_rt_entry(ഘടന task_group *tg, ഘടന rt_rq *rt_rq,
		ഘടന sched_rt_entity *rt_se, പൂർണ്ണസംഖ്യ cpu,
		ഘടന sched_rt_entity *parent);
ബാഹ്യ പൂർണ്ണസംഖ്യ sched_group_set_rt_runtime(ഘടന task_group *tg, long rt_runtime_us);
ബാഹ്യ പൂർണ്ണസംഖ്യ sched_group_set_rt_period(ഘടന task_group *tg, u64 rt_period_us);
ബാഹ്യ long sched_group_rt_runtime(ഘടന task_group *tg);
ബാഹ്യ long sched_group_rt_period(ഘടന task_group *tg);
ബാഹ്യ പൂർണ്ണസംഖ്യ sched_rt_can_attach(ഘടന task_group *tg, ഘടന task_struct *tsk);

ബാഹ്യ ഘടന task_group *sched_create_group(ഘടന task_group *parent);
ബാഹ്യ ശൂന്യം sched_online_group(ഘടന task_group *tg,
			       ഘടന task_group *parent);
ബാഹ്യ ശൂന്യം sched_destroy_group(ഘടന task_group *tg);
ബാഹ്യ ശൂന്യം sched_release_group(ഘടന task_group *tg);

ബാഹ്യ ശൂന്യം sched_move_task(ഘടന task_struct *tsk, bool for_autogroup);

#ifdef CONFIG_FAIR_GROUP_SCHED
ബാഹ്യ പൂർണ്ണസംഖ്യ sched_group_set_shares(ഘടന task_group *tg, unsigned long shares);

ബാഹ്യ പൂർണ്ണസംഖ്യ sched_group_set_idle(ഘടന task_group *tg, long idle);

ബാഹ്യ ശൂന്യം set_task_rq_fair(ഘടന sched_entity *se,
			     ഘടന cfs_rq *prev, ഘടന cfs_rq *next);
#അല്ലെങ്കിൽ /* !CONFIG_FAIR_GROUP_SCHED: */
സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ sched_group_set_shares(ഘടന task_group *tg, unsigned long shares) { തിരികെ 0; }
സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ sched_group_set_idle(ഘടന task_group *tg, long idle) { തിരികെ 0; }
#endif /* !CONFIG_FAIR_GROUP_SCHED */

#അല്ലെങ്കിൽ /* !CONFIG_CGROUP_SCHED: */

ഘടന cfs_bandwidth { };

സ്ഥിതം വരിയിൽ bool cfs_task_bw_constrained(ഘടന task_struct *p) { തിരികെ false; }

#endif /* !CONFIG_CGROUP_SCHED */

/*
 * A weight of 0 or 1 can cause arithmetics problems.
 * A weight of a cfs_rq is the sum of weights of which entities
 * are queued on this cfs_rq, so a weight of a entity should not be
 * too large, so as the shares value of a task group.
 * (The default weight is 1024 - so there's no practical
 *  limitation from this.)
 */
#define MIN_SHARES		(1UL <<  1)
#define MAX_SHARES		(1UL << 18)

ബാഹ്യ ശൂന്യം unregister_rt_sched_group(ഘടന task_group *tg);
ബാഹ്യ ശൂന്യം free_rt_sched_group(ഘടന task_group *tg);
ബാഹ്യ പൂർണ്ണസംഖ്യ alloc_rt_sched_group(ഘടന task_group *tg, ഘടന task_group *parent);

/*
 * u64_u32_load/u64_u32_store
 *
 * Use a copy of a u64 value to protect against data race. This is only
 * applicable for 32-bits architectures.
 */
#ifdef CONFIG_64BIT
# define u64_u32_load_copy(var, copy)		var
# define u64_u32_store_copy(var, copy, val)	(var = val)
#അല്ലെങ്കിൽ
# define u64_u32_load_copy(var, copy)					\
({									\
	u64 __val, __val_copy;						\
	ചെയ്യുക {								\
		__val_copy = copy;					\
		/*							\
		 * paired with u64_u32_store_copy(), ordering access	\
		 * to var and copy.					\
		 */							\
		smp_rmb();						\
		__val = var;						\
	} ആകുമ്പോൾ (__val != __val_copy);					\
	__val;								\
})
# define u64_u32_store_copy(var, copy, val)				\
ചെയ്യുക {									\
	typeof(val) __val = (val);					\
	var = __val;							\
	/*								\
	 * paired with u64_u32_load_copy(), ordering access to var and	\
	 * copy.							\
	 */								\
	smp_wmb();							\
	copy = __val;							\
} ആകുമ്പോൾ (0)
#endif
# define u64_u32_load(var)		u64_u32_load_copy(var, var##_copy)
# define u64_u32_store(var, val)	u64_u32_store_copy(var, var##_copy, val)

ഘടന balance_callback {
	ഘടന balance_callback *next;
	ശൂന്യം (*func)(ഘടന rq *rq);
};

/* Fair scheduling SCHED_{NORMAL,BATCH,IDLE} related fields in a runqueue: */
ഘടന cfs_rq {
	ഘടന load_weight	load;
	unsigned പൂർണ്ണസംഖ്യ		nr_queued;
	unsigned പൂർണ്ണസംഖ്യ		h_nr_queued;		/* SCHED_{NORMAL,BATCH,IDLE} */
	unsigned പൂർണ്ണസംഖ്യ		h_nr_runnable;		/* SCHED_{NORMAL,BATCH,IDLE} */
	unsigned പൂർണ്ണസംഖ്യ		h_nr_idle;		/* SCHED_IDLE */

	s64			sum_w_vruntime;
	u64			sum_weight;
	u64			zero_vruntime;
	unsigned പൂർണ്ണസംഖ്യ		sum_shift;

#ifdef CONFIG_SCHED_CORE
	unsigned പൂർണ്ണസംഖ്യ		forceidle_seq;
	u64			zero_vruntime_fi;
#endif

	ഘടന rb_വേര്_cached	tasks_timeline;

	/*
	 * 'curr' points to the currently running entity on this cfs_rq.
	 * It is set to NULL otherwise (i.e when none are currently running).
	 */
	ഘടന sched_entity	*curr;
	ഘടന sched_entity	*next;

	/*
	 * CFS load tracking
	 */
	ഘടന sched_entity	*h_curr;
	ഘടന sched_avg	avg;
#ifndef CONFIG_64BIT
	u64			last_update_time_copy;
#endif
	ഘടന {
		raw_spinlock_t	lock ____cacheline_aligned;
		പൂർണ്ണസംഖ്യ		nr;
		unsigned long	load_avg;
		unsigned long	util_avg;
		unsigned long	runnable_avg;
	} removed;

#ifdef CONFIG_FAIR_GROUP_SCHED
	u64			last_update_tg_load_avg;
	unsigned long		tg_load_avg_contrib;
	unsigned long		tg_runnable_avg_contrib;
	long			propagate;
	long			prop_runnable_sum;

	/*
	 *   h_load = weight * f(tg)
	 *
	 * Where f(tg) is the recursive weight fraction assigned to
	 * this group.
	 */
	unsigned long		h_load;
	u64			last_h_load_update;
	ഘടന sched_entity	*h_load_next;

	ഘടന rq		*rq;	/* CPU runqueue to which this cfs_rq is attached */

	/*
	 * leaf cfs_rqs are those that hold tasks (lowest schedulable entity in
	 * a hierarchy). Non-leaf lrqs hold other higher schedulable entities
	 * (like users, containers etc.)
	 *
	 * leaf_cfs_rq_list ties together list of leaf cfs_rq's in a CPU.
	 * This list is used during load balance.
	 */
	പൂർണ്ണസംഖ്യ			on_list;
	ഘടന list_head	leaf_cfs_rq_list;
	ഘടന task_group	*tg;	/* Group that "owns" this runqueue */

	/* Locally cached copy of our task_group's idle value */
	പൂർണ്ണസംഖ്യ			idle;

# ifdef CONFIG_CFS_BANDWIDTH
	പൂർണ്ണസംഖ്യ			runtime_enabled;
	s64			runtime_remaining;

	u64			throttled_pelt_idle;
#  ifndef CONFIG_64BIT
	u64                     throttled_pelt_idle_copy;
#  endif
	u64			throttled_clock;
	u64			throttled_clock_pelt;
	u64			throttled_clock_pelt_time;
	u64			throttled_clock_self;
	u64			throttled_clock_self_time;
	bool			throttled:1;
	bool			pelt_clock_throttled:1;
	പൂർണ്ണസംഖ്യ			throttle_എണ്ണം;
	ഘടന list_head	throttled_list;
	ഘടന list_head	throttled_csd_list;
	ഘടന list_head        throttled_limbo_list;
# endif /* CONFIG_CFS_BANDWIDTH */
#endif /* CONFIG_FAIR_GROUP_SCHED */
};

#ifdef CONFIG_SCHED_CLASS_EXT
/* scx_rq->flags, protected by the rq lock */
എണ്ണൽ scx_rq_flags {
	/*
	 * A hotplugged CPU starts scheduling before rq_online_scx(). Track
	 * ops.cpu_on/offline() state so that ops.enqueue/dispatch() are called
	 * only while the BPF scheduler considers the CPU to be online.
	 */
	SCX_RQ_ONLINE		= 1 << 0,
	SCX_RQ_CAN_STOP_TICK	= 1 << 1,
	SCX_RQ_CLK_VALID	= 1 << 5, /* RQ clock is fresh and valid */
	SCX_RQ_BAL_CB_PENDING	= 1 << 6, /* must queue a cb after dispatching */
	SCX_RQ_SUB_IDLE_RENOTIFY	= 1 << 7, /* sub-scheds are owed update_idle() */
	SCX_RQ_വേര്_IDLE_RENOTIFY	= 1 << 8, /* the root is owed update_idle() */

	SCX_RQ_IN_WAKEUP	= 1 << 16,
	SCX_RQ_IN_DISPATCH	= 1 << 17,
};

/* per-rq rescue execution state, see scx_rescue_timerfn() */
ഘടന scx_rq_rescue {
	ഘടന scx_dispatch_q	dsq;			/* stranded tasks awaiting rescue */
	s64			budget;			/* execution token bucket, ns */
	u64			clock;			/* last budget accrual timestamp */
	ഘടന task_struct	*curr;			/* task being rescued, one at a time */
	s64			slice;			/* curr's admitted slice */
	u64			exec_snap;		/* sum_exec_runtime at admission */
	ഘടന timer_list	timer;			/* paces admission and escalation */
	u64			kill_at;		/* last ejection, init before any */
};

ഘടന scx_rq {
	ഘടന scx_dispatch_q	local_dsq;
#ifdef CONFIG_EXT_SUB_SCHED
	ഘടന scx_dispatch_q	reject_dsq;		/* staging for cap-rejected tasks */
	ഘടന scx_rq_rescue	rescue;
#endif
	ഘടന list_head	runnable_list;		/* runnable tasks on this rq */
	ഘടന list_head	ddsp_deferred_locals;	/* deferred ddsps from enq */
	/* both stashed across the activate_task() in move_remote_task_to_local_dsq() */
	u64			remote_activate_enq_flags;
	ഘടന scx_sched	*remote_activate_sch;
	u32			nr_running;
	u32			cpuperf_target;		/* [0, SCHED_CAPACITY_SCALE] */
	bool			in_select_cpu;
	bool			cpu_released;
	u32			flags;
	u32			nr_immed;		/* ENQ_IMMED tasks on local_dsq */
#ifdef CONFIG_SCHED_CORE
	u32			lock_drop_seq;	/* nr dispatch lock releases */
#endif
	u64			clock;			/* current per-rq clock -- see scx_bpf_now() */
#ifdef CONFIG_EXT_SUB_SCHED
	ഘടന llist_head	ecaps_to_sync;		/* pending ecaps syncs */
	ഘടന task_struct	*sub_dispatch_prev;
#endif
	cpumask_var_t		cpus_to_sync;
	bool			kick_sync_pending;
	unsigned long		kick_sync;

	ഘടന list_head	sched_pcpus_to_kick;	/* see kick_cpus_irq_workfn() */

	raw_spinlock_t		deferred_reenq_lock;
	ഘടന list_head	deferred_reenq_locals;	/* scheds requesting reenq of local DSQ */
	ഘടന list_head	deferred_reenq_users;	/* user DSQs requesting reenq */
	ഘടന balance_callback	deferred_bal_cb;
	ഘടന balance_callback	kick_sync_bal_cb;
	ഘടന irq_work		deferred_irq_work;
	ഘടന irq_work		kick_cpus_irq_work;
};
#endif /* CONFIG_SCHED_CLASS_EXT */

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ rt_bandwidth_enabled(ശൂന്യം)
{
	തിരികെ sysctl_sched_rt_runtime >= 0;
}

/* RT IPI pull logic requires IRQ_WORK */
#എങ്കിൽ defined(CONFIG_IRQ_WORK) && defined(CONFIG_SMP)
# define HAVE_RT_തള്ളുക_IPI
#endif

/* Real-Time classes' related field in a runqueue: */
ഘടന rt_rq {
	ഘടന rt_prio_array	active;
	unsigned പൂർണ്ണസംഖ്യ		rt_nr_running;
	unsigned പൂർണ്ണസംഖ്യ		rr_nr_running;
	ഘടന {
		പൂർണ്ണസംഖ്യ		curr; /* highest queued rt task prio */
		പൂർണ്ണസംഖ്യ		next; /* next highest */
	} highest_prio;
	bool			overloaded;
	ഘടന plist_head	pushable_tasks;

	പൂർണ്ണസംഖ്യ			rt_queued;

#ifdef CONFIG_RT_GROUP_SCHED
	പൂർണ്ണസംഖ്യ			rt_throttled;
	u64			rt_time; /* consumed RT time, goes up in update_curr_rt */
	u64			rt_runtime; /* allotted RT time, "slice" from rt_bandwidth, RT sharing/balancing */
	/* Nests inside the rq lock: */
	raw_spinlock_t		rt_runtime_lock;

	unsigned പൂർണ്ണസംഖ്യ		rt_nr_boosted;

	ഘടന rq		*rq; /* this is always top-level rq, cache? */
#endif
#ifdef CONFIG_CGROUP_SCHED
	ഘടന task_group	*tg; /* this tg has "this" rt_rq on given CPU for runnable entities */
#endif
};

സ്ഥിതം വരിയിൽ bool rt_rq_is_runnable(ഘടന rt_rq *rt_rq)
{
	തിരികെ rt_rq->rt_queued && rt_rq->rt_nr_running;
}

/* Deadline class' related fields in a runqueue */
ഘടന dl_rq {
	/* runqueue is an rbtree, ordered by deadline */
	ഘടന rb_വേര്_cached	വേര്;

	unsigned പൂർണ്ണസംഖ്യ		dl_nr_running;

	/*
	 * Deadline values of the currently executing and the
	 * earliest ready task on this rq. Caching these facilitates
	 * the decision whether or not a ready but not running task
	 * should migrate somewhere else.
	 */
	ഘടന {
		u64		curr;
		u64		next;
	} earliest_dl;

	bool			overloaded;

	ഘടന sched_dl_entity	*curr;
	/*
	 * Tasks on this rq that can be pushed away. They are kept in
	 * an rb-tree, ordered by tasks' deadlines, with caching
	 * of the leftmost (earliest deadline) element.
	 */
	ഘടന rb_വേര്_cached	pushable_dl_tasks_വേര്;

	/*
	 * "Active utilization" for this runqueue: increased when a
	 * task wakes up (becomes TASK_RUNNING) and decreased when a
	 * task blocks
	 */
	u64			running_bw;

	/*
	 * Utilization of the tasks "assigned" to this runqueue (including
	 * the tasks that are in runqueue and the tasks that executed on this
	 * CPU and blocked). Increased when a task moves to this runqueue, and
	 * decreased when the task moves away (migrates, changes scheduling
	 * policy, or terminates).
	 * This is needed to compute the "inactive utilization" for the
	 * runqueue (inactive utilization = this_bw - running_bw).
	 */
	u64			this_bw;
	u64			extra_bw;

	/*
	 * Maximum available bandwidth for reclaiming by SCHED_FLAG_RECLAIM
	 * tasks of this rq. Used in calculation of reclaimable bandwidth(GRUB).
	 */
	u64			max_bw;

	/*
	 * Inverse of the fraction of CPU utilization that can be reclaimed
	 * by the GRUB algorithm.
	 */
	u64			bw_ratio;
};

#ifdef CONFIG_FAIR_GROUP_SCHED
/* Check whether a task group is root tg */
#define is_വേര്_task_group(tg) ((tg) == &വേര്_task_group)
/* An entity is a task if it doesn't "own" a runqueue */
#define entity_is_task(se)	(!se->my_q)

സ്ഥിതം വരിയിൽ ശൂന്യം se_update_runnable(ഘടന sched_entity *se)
{
	എങ്കിൽ (!entity_is_task(se))
		se->runnable_weight = se->my_q->h_nr_runnable;
}

സ്ഥിതം വരിയിൽ long se_runnable(ഘടന sched_entity *se)
{
	എങ്കിൽ (se->sched_delayed)
		തിരികെ false;

	എങ്കിൽ (entity_is_task(se))
		തിരികെ !!se->on_rq;
	അല്ലെങ്കിൽ
		തിരികെ se->runnable_weight;
}

#അല്ലെങ്കിൽ /* !CONFIG_FAIR_GROUP_SCHED: */

#define entity_is_task(se)	1

സ്ഥിതം വരിയിൽ ശൂന്യം se_update_runnable(ഘടന sched_entity *se) { }

സ്ഥിതം വരിയിൽ long se_runnable(ഘടന sched_entity *se)
{
	എങ്കിൽ (se->sched_delayed)
		തിരികെ false;

	തിരികെ !!se->on_rq;
}

#endif /* !CONFIG_FAIR_GROUP_SCHED */

/*
 * XXX we want to get rid of these helpers and use the full load resolution.
 */
സ്ഥിതം വരിയിൽ long se_weight(ഘടന sched_entity *se)
{
	തിരികെ scale_load_down(se->load.weight);
}


സ്ഥിതം വരിയിൽ bool sched_asym_prefer(പൂർണ്ണസംഖ്യ a, പൂർണ്ണസംഖ്യ b)
{
	തിരികെ arch_asym_cpu_priority(a) > arch_asym_cpu_priority(b);
}

ഘടന perf_domain {
	ഘടന em_perf_domain *em_pd;
	ഘടന perf_domain *next;
	ഘടന rcu_head rcu;
};

/*
 * We add the notion of a root-domain which will be used to define per-domain
 * variables. Each exclusive cpuset essentially defines an island domain by
 * fully partitioning the member CPUs from any other cpuset. Whenever a new
 * exclusive cpuset is created, we also create and attach a new root-domain
 * object.
 *
 */
ഘടന വേര്_domain {
	atomic_t		refcount;
	atomic_t		rto_എണ്ണം;
	ഘടന rcu_head		rcu;
	cpumask_var_t		span;
	cpumask_var_t		online;

	/*
	 * Indicate pullable load on at least one CPU, e.g:
	 * - More than one runnable task
	 * - Running task is misfit
	 */
	bool			overloaded;

	/* Indicate one or more CPUs over-utilized (tipping point) */
	bool			overutilized;

	/*
	 * The bit corresponding to a CPU gets set here if such CPU has more
	 * than one runnable -deadline task (as it is below for RT tasks).
	 */
	cpumask_var_t		dlo_mask;
	atomic_t		dlo_എണ്ണം;
	ഘടന dl_bw		dl_bw;
	ഘടന cpudl		cpudl;

	/*
	 * Indicate whether a root_domain's dl_bw has been checked or
	 * updated. It's monotonously increasing value.
	 *
	 * Also, some corner cases, like 'wrap around' is dangerous, but given
	 * that u64 is 'big enough'. So that shouldn't be a concern.
	 */
	u64 visit_cookie;

#ifdef HAVE_RT_തള്ളുക_IPI
	/*
	 * For IPI pull requests, loop across the rto_mask.
	 */
	ഘടന irq_work		rto_തള്ളുക_work;
	raw_spinlock_t		rto_lock;
	/* These are only updated and read within rto_lock */
	പൂർണ്ണസംഖ്യ			rto_loop;
	പൂർണ്ണസംഖ്യ			rto_cpu;
	/* These atomics are updated outside of a lock */
	atomic_t		rto_loop_next;
	atomic_t		rto_loop_start;
#endif /* HAVE_RT_PUSH_IPI */
	/*
	 * The "RT overload" flag: it gets set if a CPU has more than
	 * one runnable RT task.
	 */
	cpumask_var_t		rto_mask;
	ഘടന cpupri		cpupri;

	/*
	 * NULL-terminated list of performance domains intersecting with the
	 * CPUs of the rd. Protected by RCU.
	 */
	ഘടന perf_domain __rcu *pd;
};

ബാഹ്യ ശൂന്യം init_defrootdomain(ശൂന്യം);
ബാഹ്യ പൂർണ്ണസംഖ്യ sched_init_domains(സ്ഥിരം ഘടന cpumask *cpu_map);
ബാഹ്യ ശൂന്യം rq_attach_വേര്(ഘടന rq *rq, ഘടന വേര്_domain *rd);
ബാഹ്യ ശൂന്യം sched_get_rd(ഘടന വേര്_domain *rd);
ബാഹ്യ ശൂന്യം sched_put_rd(ഘടന വേര്_domain *rd);

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ get_rd_overloaded(ഘടന വേര്_domain *rd)
{
	തിരികെ READ_ONCE(rd->overloaded);
}

സ്ഥിതം വരിയിൽ ശൂന്യം set_rd_overloaded(ഘടന വേര്_domain *rd, പൂർണ്ണസംഖ്യ status)
{
	എങ്കിൽ (get_rd_overloaded(rd) != status)
		WRITE_ONCE(rd->overloaded, status);
}

#ifdef HAVE_RT_തള്ളുക_IPI
ബാഹ്യ ശൂന്യം rto_തള്ളുക_irq_work_func(ഘടന irq_work *work);
#endif

#ifdef CONFIG_UCLAMP_TASK
/*
 * struct uclamp_bucket - Utilization clamp bucket
 * @value: utilization clamp value for tasks on this clamp bucket
 * @tasks: number of RUNNABLE tasks on this clamp bucket
 *
 * Keep track of how many tasks are RUNNABLE for a given utilization
 * clamp value.
 */
ഘടന uclamp_bucket {
	unsigned long മൂല്യം : bits_per(SCHED_CAPACITY_SCALE);
	unsigned long tasks : BITS_PER_LONG - bits_per(SCHED_CAPACITY_SCALE);
};

/*
 * struct uclamp_rq - rq's utilization clamp
 * @value: currently active clamp values for a rq
 * @bucket: utilization clamp buckets affecting a rq
 *
 * Keep track of RUNNABLE tasks on a rq to aggregate their clamp values.
 * A clamp value is affecting a rq when there is at least one task RUNNABLE
 * (or actually running) with that value.
 *
 * There are up to UCLAMP_CNT possible different clamp values, currently there
 * are only two: minimum utilization and maximum utilization.
 *
 * All utilization clamping values are MAX aggregated, since:
 * - for util_min: we want to run the CPU at least at the max of the minimum
 *   utilization required by its currently RUNNABLE tasks.
 * - for util_max: we want to allow the CPU to run up to the max of the
 *   maximum utilization allowed by its currently RUNNABLE tasks.
 *
 * Since on each system we expect only a limited number of different
 * utilization clamp values (UCLAMP_BUCKETS), use a simple array to track
 * the metrics required to compute all the per-rq utilization clamp values.
 */
ഘടന uclamp_rq {
	unsigned പൂർണ്ണസംഖ്യ മൂല്യം;
	ഘടന uclamp_bucket bucket[UCLAMP_BUCKETS];
};

DECLARE_STATIC_താക്കോൽ_FALSE(sched_uclamp_used);
#endif /* CONFIG_UCLAMP_TASK */

/*
 * This is the main, per-CPU runqueue data structure.
 *
 * Locking rule: those places that want to lock multiple runqueues
 * (such as the load balancing or the thread migration code), lock
 * acquire operations must be ordered by ascending &runqueue.
 */
ഘടന rq {
	/*
	 * The following members are loaded together, without holding the
	 * rq->lock, in an extremely hot loop in update_sg_lb_stats()
	 * (called from pick_next_task()). To reduce cache pollution from
	 * this operation, they are placed together on this dedicated cache
	 * line. Even though some of them are frequently modified, they are
	 * loaded much more frequently than they are stored.
	 */
	unsigned പൂർണ്ണസംഖ്യ		nr_running;
#ifdef CONFIG_NUMA_BALANCING
	unsigned പൂർണ്ണസംഖ്യ		nr_numa_running;
	unsigned പൂർണ്ണസംഖ്യ		nr_preferred_running;
#endif
	unsigned പൂർണ്ണസംഖ്യ		ttwu_pending;
	unsigned long		cpu_capacity;
#ifdef CONFIG_SCHED_PROXY_EXEC
	ഘടന task_struct __rcu	*donor;  /* Scheduling context */
	ഘടന task_struct __rcu	*curr;   /* Execution context */
#അല്ലെങ്കിൽ
	സംഘം {
		ഘടന task_struct __rcu *donor; /* Scheduler context */
		ഘടന task_struct __rcu *curr;  /* Execution context */
	};
#endif
	ഘടന task_struct	*idle;
	/* padding left here deliberately */

	/*
	 * The next cacheline holds the (hot) runqueue lock, as well as
	 * some other less performance-critical fields.
	 */
	u64			nr_switches	____cacheline_aligned;

	/* runqueue lock: */
	raw_spinlock_t		__lock;

#ifdef CONFIG_NO_HZ_COMMON
	unsigned പൂർണ്ണസംഖ്യ		nohz_tick_stopped;
	atomic_t		nohz_flags;
	unsigned പൂർണ്ണസംഖ്യ		has_blocked_load;
	unsigned long		last_blocked_load_update_tick;
	call_single_data_t	nohz_csd;
#endif /* CONFIG_NO_HZ_COMMON */

#ifdef CONFIG_UCLAMP_TASK
	/* Utilization clamp values based on CPU's RUNNABLE tasks */
	ഘടന uclamp_rq	uclamp[UCLAMP_CNT] ____cacheline_aligned;
	unsigned പൂർണ്ണസംഖ്യ		uclamp_flags;
#define UCLAMP_FLAG_IDLE 0x01
#endif

	ഘടന cfs_rq		cfs;
	ഘടന rt_rq		rt;
	ഘടന dl_rq		dl;
#ifdef CONFIG_SCHED_CLASS_EXT
	ഘടന scx_rq		scx;
	ഘടന sched_dl_entity	ext_server;
#endif
#ifdef CONFIG_SCHED_CACHE
	raw_spinlock_t		cpu_epoch_lock ____cacheline_aligned;
	u64			cpu_runtime;
	unsigned long		cpu_epoch;
	unsigned long		cpu_epoch_next;
#endif

	ഘടന sched_dl_entity	fair_server;

#ifdef CONFIG_FAIR_GROUP_SCHED
	/* list of leaf cfs_rq on this CPU: */
	ഘടന list_head	leaf_cfs_rq_list;
	ഘടന list_head	*tmp_alone_branch;
#endif /* CONFIG_FAIR_GROUP_SCHED */

#ifdef CONFIG_NUMA_BALANCING
	unsigned പൂർണ്ണസംഖ്യ		numa_migrate_on;
#endif

#ifdef CONFIG_SCHED_CACHE
	unsigned പൂർണ്ണസംഖ്യ		nr_pref_llc_running;
	unsigned പൂർണ്ണസംഖ്യ		nr_llc_running;
#endif

	/*
	 * This is part of a global counter where only the total sum
	 * over all CPUs matters. A task can increase this counter on
	 * one CPU and if it got migrated afterwards it may decrease
	 * it on another CPU. Always updated under the runqueue lock:
	 */
	unsigned long		nr_uninterruptible;

	ഘടന sched_dl_entity	*dl_server;
	ഘടന task_struct	*stop;
	സ്ഥിരം ഘടന sched_class *next_class;
	unsigned long		next_balance;
	ഘടന mm_struct	*prev_mm;

	/*
	 * The following fields of clock data are frequently referenced
	 * and updated together, and should go on their own cache line.
	 */
	u64			clock_task ____cacheline_aligned;
	u64			clock_pelt;
	u64			clock;
	unsigned long		lost_idle_time;
	unsigned പൂർണ്ണസംഖ്യ		clock_update_flags;
	u64			clock_pelt_idle;
	u64			clock_idle;

#ifndef CONFIG_64BIT
	u64			clock_pelt_idle_copy;
	u64			clock_idle_copy;
#endif

	u64 last_seen_need_resched_ns;
	പൂർണ്ണസംഖ്യ ticks_without_resched;

#ifdef CONFIG_MEMBARRIER
	പൂർണ്ണസംഖ്യ membarrier_state;
#endif

	ഘടന വേര്_domain		*rd;
	ഘടന sched_domain __rcu	*sd;

	ഘടന balance_callback *balance_callback;

	unsigned അക്ഷരം		nohz_idle_balance;
	unsigned അക്ഷരം		idle_balance;

	unsigned long		misfit_task_load;

	/* For active balancing */
	പൂർണ്ണസംഖ്യ			active_balance;
	പൂർണ്ണസംഖ്യ			തള്ളുക_cpu;
	ഘടന cpu_stop_work	active_balance_work;

	/* CPU of this runqueue: */
	പൂർണ്ണസംഖ്യ			cpu;
	പൂർണ്ണസംഖ്യ			online;

	ഘടന list_head cfs_tasks;

	ഘടന sched_avg	avg_rt;
	ഘടന sched_avg	avg_dl;
#ifdef CONFIG_HAVE_SCHED_AVG_IRQ
	ഘടന sched_avg	avg_irq;
#endif
#ifdef CONFIG_SCHED_HW_PRESSURE
	ഘടന sched_avg	avg_hw;
#endif
	u64			idle_stamp;
	u64			avg_idle;

	/* This is used to determine avg_idle's max value */
	u64			max_idle_balance_cost;

#ifdef CONFIG_HOTPLUG_CPU
	ഘടന rcuwait		hotplug_wait;
#endif

#ifdef CONFIG_IRQ_TIME_ACCOUNTING
	u64			prev_irq_time;
	u64			psi_irq_time;
#endif
#ifdef CONFIG_PARAVIRT
	u64			prev_steal_time;
#endif
#ifdef CONFIG_PARAVIRT_TIME_ACCOUNTING
	u64			prev_steal_time_rq;
#endif
#ifdef CONFIG_PREFERRED_CPU
	bool			npc_തള്ളുക_work_pending;
#endif

	/* calc_load related fields */
	unsigned long		calc_load_update;
	long			calc_load_active;

#ifdef CONFIG_SCHED_HRTICK
	call_single_data_t	hrtick_csd;
	ഘടന hrtimer		hrtick_timer;
	ktime_t			hrtick_time;
	ktime_t			hrtick_delay;
	unsigned പൂർണ്ണസംഖ്യ		hrtick_sched;
#endif

#ifdef CONFIG_SCHEDSTATS
	/* latency stats */
	ഘടന sched_info	rq_sched_info;
	unsigned long long	rq_cpu_time;

	/* sys_sched_yield() stats */
	unsigned പൂർണ്ണസംഖ്യ		yld_എണ്ണം;

	/* schedule() stats */
	unsigned പൂർണ്ണസംഖ്യ		sched_എണ്ണം;
	unsigned പൂർണ്ണസംഖ്യ		sched_goidle;

	/* try_to_wake_up() stats */
	unsigned പൂർണ്ണസംഖ്യ		ttwu_എണ്ണം;
	unsigned പൂർണ്ണസംഖ്യ		ttwu_local;
#endif

#ifdef CONFIG_CPU_IDLE
	/* Must be inspected within a RCU lock section */
	ഘടന cpuidle_state	*idle_state;
#endif

	unsigned പൂർണ്ണസംഖ്യ		nr_pinned;
	unsigned പൂർണ്ണസംഖ്യ		തള്ളുക_busy;
	ഘടന cpu_stop_work	തള്ളുക_work;

#ifdef CONFIG_SCHED_CORE
	/* per rq */
	ഘടന rq		*core;
	ഘടന task_struct	*core_pick;
	ഘടന sched_dl_entity	*core_dl_server;
	unsigned പൂർണ്ണസംഖ്യ		core_enabled;
	ഘടന rb_വേര്		core_വൃക്ഷം;

	/* shared state -- careful with sched_core_cpu_deactivate() */
	unsigned പൂർണ്ണസംഖ്യ		core_task_seq;
	unsigned പൂർണ്ണസംഖ്യ		core_pick_seq;
	unsigned long		core_cookie;
	unsigned പൂർണ്ണസംഖ്യ		core_forceidle_എണ്ണം;
	unsigned പൂർണ്ണസംഖ്യ		core_forceidle_seq;
	unsigned പൂർണ്ണസംഖ്യ		core_forceidle_occupation;
	u64			core_forceidle_start;
	unsigned പൂർണ്ണസംഖ്യ		core_pick_in_flight;
#endif /* CONFIG_SCHED_CORE */

	/* Scratch cpumask to be temporarily used under rq_lock */
	cpumask_var_t		scratch_mask;

#ifdef CONFIG_CFS_BANDWIDTH
	call_single_data_t	cfsb_csd;
	ഘടന list_head	cfsb_csd_list;
#endif

	atomic_t		nr_iowait;
} __no_randomize_layout;

#ifdef CONFIG_FAIR_GROUP_SCHED

/* CPU runqueue to which this cfs_rq is attached */
സ്ഥിതം വരിയിൽ ഘടന rq *rq_of(ഘടന cfs_rq *cfs_rq)
{
	തിരികെ cfs_rq->rq;
}

#അല്ലെങ്കിൽ /* !CONFIG_FAIR_GROUP_SCHED: */

സ്ഥിതം വരിയിൽ ഘടന rq *rq_of(ഘടന cfs_rq *cfs_rq)
{
	തിരികെ container_of(cfs_rq, ഘടന rq, cfs);
}
#endif /* !CONFIG_FAIR_GROUP_SCHED */

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ cpu_of(ഘടന rq *rq)
{
	തിരികെ rq->cpu;
}

#define MDF_തള്ളുക		0x01

സ്ഥിതം വരിയിൽ bool is_migration_disabled(ഘടന task_struct *p)
{
	തിരികെ p->migration_disabled;
}

DECLARE_PER_CPU_SHARED_ALIGNED(ഘടന rq, runqueues);
DECLARE_PER_CPU(ഘടന rnd_state, sched_rnd_state);

സ്ഥിതം വരിയിൽ u32 sched_rng(ശൂന്യം)
{
	തിരികെ prandom_u32_state(this_cpu_ptr(&sched_rnd_state));
}

സ്ഥിതം __always_inline ഘടന rq *__this_rq(ശൂന്യം)
{
	തിരികെ this_cpu_ptr(&runqueues);
}

#define cpu_rq(cpu)		(&per_cpu(runqueues, (cpu)))
#define this_rq()		__this_rq()
#define task_rq(p)		cpu_rq(task_cpu(p))
#define cpu_curr(cpu)		(cpu_rq(cpu)->curr)
#define raw_rq()		raw_cpu_ptr(&runqueues)

സ്ഥിതം വരിയിൽ bool idle_rq(ഘടന rq *rq)
{
	തിരികെ rq->curr == rq->idle && !rq->nr_running && !rq->ttwu_pending;
}

/**
 * available_idle_cpu - is a given CPU idle for enqueuing work.
 * @cpu: the CPU in question.
 *
 * Return: 1 if the CPU is currently idle. 0 otherwise.
 */
സ്ഥിതം വരിയിൽ bool available_idle_cpu(പൂർണ്ണസംഖ്യ cpu)
{
	എങ്കിൽ (!idle_rq(cpu_rq(cpu)))
		തിരികെ 0;

	എങ്കിൽ (vcpu_is_preempted(cpu))
		തിരികെ 0;

	തിരികെ 1;
}

#ifdef CONFIG_SCHED_PROXY_EXEC
സ്ഥിതം വരിയിൽ ശൂന്യം rq_set_donor(ഘടന rq *rq, ഘടന task_struct *t)
{
	rcu_assign_pointer(rq->donor, t);
}
#അല്ലെങ്കിൽ
സ്ഥിതം വരിയിൽ ശൂന്യം rq_set_donor(ഘടന rq *rq, ഘടന task_struct *t)
{
	/* Do nothing */
}
#endif

#ifdef CONFIG_SCHED_CORE
സ്ഥിതം വരിയിൽ ഘടന cpumask *sched_group_span(ഘടന sched_group *sg);

DECLARE_STATIC_താക്കോൽ_FALSE(__sched_core_enabled);

സ്ഥിതം വരിയിൽ bool sched_core_enabled(ഘടന rq *rq)
{
	തിരികെ static_branch_unlikely(&__sched_core_enabled) && rq->core_enabled;
}

സ്ഥിതം വരിയിൽ bool sched_core_disabled(ശൂന്യം)
{
	തിരികെ !static_branch_unlikely(&__sched_core_enabled);
}

/*
 * Be careful with this function; not for general use. The return value isn't
 * stable unless you actually hold a relevant rq->__lock.
 */
സ്ഥിതം വരിയിൽ raw_spinlock_t *rq_lockp(ഘടന rq *rq)
{
	എങ്കിൽ (sched_core_enabled(rq))
		തിരികെ &rq->core->__lock;

	തിരികെ &rq->__lock;
}

സ്ഥിതം വരിയിൽ raw_spinlock_t *__rq_lockp(ഘടന rq *rq)
	__returns_ctx_lock(rq_lockp(rq)) /* alias them */
{
	എങ്കിൽ (rq->core_enabled)
		തിരികെ &rq->core->__lock;

	തിരികെ &rq->__lock;
}

ബാഹ്യ bool
cfs_prio_less(സ്ഥിരം ഘടന task_struct *a, സ്ഥിരം ഘടന task_struct *b, bool fi);

ബാഹ്യ ശൂന്യം task_vruntime_update(ഘടന rq *rq, ഘടന task_struct *p, bool in_fi);

/*
 * Helpers to check if the CPU's core cookie matches with the task's cookie
 * when core scheduling is enabled.
 * A special case is that the task's cookie always matches with CPU's core
 * cookie if the CPU is in an idle core.
 */
സ്ഥിതം വരിയിൽ bool sched_cpu_cookie_match(ഘടന rq *rq, ഘടന task_struct *p)
{
	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	എങ്കിൽ (!sched_core_enabled(rq))
		തിരികെ true;

	തിരികെ rq->core->core_cookie == p->core_cookie;
}

സ്ഥിതം വരിയിൽ bool sched_core_cookie_match(ഘടന rq *rq, ഘടന task_struct *p)
{
	bool idle_core = true;
	പൂർണ്ണസംഖ്യ cpu;

	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	എങ്കിൽ (!sched_core_enabled(rq))
		തിരികെ true;

	എങ്കിൽ (rq->core->core_cookie == p->core_cookie)
		തിരികെ true;

	for_each_cpu(cpu, cpu_smt_mask(cpu_of(rq))) {
		എങ്കിൽ (!available_idle_cpu(cpu)) {
			idle_core = false;
			നിർത്തുക;
		}
	}

	/*
	 * A CPU in an idle core is always the best choice for tasks with
	 * cookies.
	 */
	തിരികെ idle_core;
}

സ്ഥിതം വരിയിൽ bool sched_group_cookie_match(ഘടന rq *rq,
					    ഘടന task_struct *p,
					    ഘടന sched_group *group)
{
	പൂർണ്ണസംഖ്യ cpu;

	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	എങ്കിൽ (!sched_core_enabled(rq))
		തിരികെ true;

	for_each_cpu_and(cpu, sched_group_span(group), p->cpus_ptr) {
		എങ്കിൽ (sched_core_cookie_match(cpu_rq(cpu), p))
			തിരികെ true;
	}
	തിരികെ false;
}

സ്ഥിതം വരിയിൽ bool sched_core_enqueued(ഘടന task_struct *p)
{
	തിരികെ !RB_EMPTY_കണ്ണി(&p->core_കണ്ണി);
}

ബാഹ്യ ശൂന്യം sched_core_enqueue(ഘടന rq *rq, ഘടന task_struct *p);
ബാഹ്യ ശൂന്യം sched_core_dequeue(ഘടന rq *rq, ഘടന task_struct *p, പൂർണ്ണസംഖ്യ flags);

ബാഹ്യ ശൂന്യം sched_core_get(ശൂന്യം);
ബാഹ്യ ശൂന്യം sched_core_put(ശൂന്യം);

സ്ഥിതം വരിയിൽ bool task_has_sched_core(ഘടന task_struct *p)
{
	എങ്കിൽ (sched_core_disabled())
		തിരികെ false;

	തിരികെ !!p->core_cookie;
}

#അല്ലെങ്കിൽ /* !CONFIG_SCHED_CORE: */

സ്ഥിതം വരിയിൽ bool sched_core_enabled(ഘടന rq *rq)
{
	തിരികെ false;
}

സ്ഥിതം വരിയിൽ bool sched_core_disabled(ശൂന്യം)
{
	തിരികെ true;
}

സ്ഥിതം വരിയിൽ raw_spinlock_t *rq_lockp(ഘടന rq *rq)
{
	തിരികെ &rq->__lock;
}

സ്ഥിതം വരിയിൽ raw_spinlock_t *__rq_lockp(ഘടന rq *rq)
	__returns_ctx_lock(rq_lockp(rq)) /* alias them */
{
	തിരികെ &rq->__lock;
}

സ്ഥിതം വരിയിൽ bool sched_cpu_cookie_match(ഘടന rq *rq, ഘടന task_struct *p)
{
	തിരികെ true;
}

സ്ഥിതം വരിയിൽ bool sched_core_cookie_match(ഘടന rq *rq, ഘടന task_struct *p)
{
	തിരികെ true;
}

സ്ഥിതം വരിയിൽ bool sched_group_cookie_match(ഘടന rq *rq,
					    ഘടന task_struct *p,
					    ഘടന sched_group *group)
{
	തിരികെ true;
}

സ്ഥിതം വരിയിൽ bool task_has_sched_core(ഘടന task_struct *p)
{
	തിരികെ false;
}

#endif /* !CONFIG_SCHED_CORE */

#ifdef CONFIG_RT_GROUP_SCHED
# ifdef CONFIG_RT_GROUP_SCHED_DEFAULT_DISABLED
DECLARE_STATIC_താക്കോൽ_FALSE(rt_group_sched);
സ്ഥിതം വരിയിൽ bool rt_group_sched_enabled(ശൂന്യം)
{
	തിരികെ static_branch_unlikely(&rt_group_sched);
}
# അല്ലെങ്കിൽ /* !CONFIG_RT_GROUP_SCHED_DEFAULT_DISABLED: */
DECLARE_STATIC_താക്കോൽ_TRUE(rt_group_sched);
സ്ഥിതം വരിയിൽ bool rt_group_sched_enabled(ശൂന്യം)
{
	തിരികെ static_branch_likely(&rt_group_sched);
}
# endif /* !CONFIG_RT_GROUP_SCHED_DEFAULT_DISABLED */
#അല്ലെങ്കിൽ /* !CONFIG_RT_GROUP_SCHED: */
# define rt_group_sched_enabled()	false
#endif /* !CONFIG_RT_GROUP_SCHED */

സ്ഥിതം വരിയിൽ ശൂന്യം lockdep_assert_rq_held(ഘടന rq *rq)
	__assumes_ctx_lock(__rq_lockp(rq))
{
	lockdep_assert_held(__rq_lockp(rq));
}

ബാഹ്യ ശൂന്യം raw_spin_rq_lock_nested(ഘടന rq *rq, പൂർണ്ണസംഖ്യ subclass)
	__acquires(__rq_lockp(rq));

ബാഹ്യ bool raw_spin_rq_trylock(ഘടന rq *rq)
	__cond_acquires(true, __rq_lockp(rq));

സ്ഥിതം വരിയിൽ ശൂന്യം raw_spin_rq_lock(ഘടന rq *rq)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_lock_nested(rq, 0);
}

സ്ഥിതം വരിയിൽ ശൂന്യം raw_spin_rq_unlock(ഘടന rq *rq)
	__releases(__rq_lockp(rq))
{
	raw_spin_unlock(rq_lockp(rq));
}

സ്ഥിതം വരിയിൽ ശൂന്യം raw_spin_rq_lock_irq(ഘടന rq *rq)
	__acquires(__rq_lockp(rq))
{
	local_irq_disable();
	raw_spin_rq_lock(rq);
}

സ്ഥിതം വരിയിൽ ശൂന്യം raw_spin_rq_unlock_irq(ഘടന rq *rq)
	__releases(__rq_lockp(rq))
{
	raw_spin_rq_unlock(rq);
	local_irq_enable();
}

സ്ഥിതം വരിയിൽ unsigned long _raw_spin_rq_lock_irqsave(ഘടന rq *rq)
	__acquires(__rq_lockp(rq))
{
	unsigned long flags;

	local_irq_save(flags);
	raw_spin_rq_lock(rq);

	തിരികെ flags;
}

സ്ഥിതം വരിയിൽ ശൂന്യം raw_spin_rq_unlock_irqrestore(ഘടന rq *rq, unsigned long flags)
	__releases(__rq_lockp(rq))
{
	raw_spin_rq_unlock(rq);
	local_irq_restore(flags);
}

#define raw_spin_rq_lock_irqsave(rq, flags)	\
ചെയ്യുക {						\
	flags = _raw_spin_rq_lock_irqsave(rq);	\
} ആകുമ്പോൾ (0)

ബാഹ്യ ശൂന്യം __update_idle_core(ഘടന rq *rq);

സ്ഥിതം വരിയിൽ ശൂന്യം update_idle_core(ഘടന rq *rq)
{
	എങ്കിൽ (sched_smt_active())
		__update_idle_core(rq);
}

#ifdef CONFIG_FAIR_GROUP_SCHED
സ്ഥിതം വരിയിൽ ഘടന task_struct *task_of(ഘടന sched_entity *se)
{
	WARN_ON_ONCE(!entity_is_task(se));
	തിരികെ container_of(se, ഘടന task_struct, se);
}

സ്ഥിതം വരിയിൽ ഘടന cfs_rq *task_cfs_rq(ഘടന task_struct *p)
{
	തിരികെ p->se.cfs_rq;
}

/* runqueue on which this entity is (to be) queued */
സ്ഥിതം വരിയിൽ ഘടന cfs_rq *cfs_rq_of(സ്ഥിരം ഘടന sched_entity *se)
{
	തിരികെ se->cfs_rq;
}

/* runqueue "owned" by this group */
സ്ഥിതം വരിയിൽ ഘടന cfs_rq *group_cfs_rq(ഘടന sched_entity *grp)
{
	തിരികെ grp->my_q;
}

#അല്ലെങ്കിൽ /* !CONFIG_FAIR_GROUP_SCHED: */

#define task_of(_se)		container_of(_se, ഘടന task_struct, se)

സ്ഥിതം വരിയിൽ ഘടന cfs_rq *task_cfs_rq(സ്ഥിരം ഘടന task_struct *p)
{
	തിരികെ &task_rq(p)->cfs;
}

സ്ഥിതം വരിയിൽ ഘടന cfs_rq *cfs_rq_of(സ്ഥിരം ഘടന sched_entity *se)
{
	സ്ഥിരം ഘടന task_struct *p = task_of(se);
	ഘടന rq *rq = task_rq(p);

	തിരികെ &rq->cfs;
}

/* runqueue "owned" by this group */
സ്ഥിതം വരിയിൽ ഘടന cfs_rq *group_cfs_rq(ഘടന sched_entity *grp)
{
	തിരികെ ശൂന്യ;
}

#endif /* !CONFIG_FAIR_GROUP_SCHED */

ബാഹ്യ ശൂന്യം update_rq_avg_idle(ഘടന rq *rq);
ബാഹ്യ ശൂന്യം update_rq_clock(ഘടന rq *rq);

/*
 * rq::clock_update_flags bits
 *
 * %RQCF_REQ_SKIP - will request skipping of clock update on the next
 *  call to __schedule(). This is an optimisation to avoid
 *  neighbouring rq clock updates.
 *
 * %RQCF_ACT_SKIP - is set from inside of __schedule() when skipping is
 *  in effect and calls to update_rq_clock() are being ignored.
 *
 * %RQCF_UPDATED - is a debug flag that indicates whether a call has been
 *  made to update_rq_clock() since the last time rq::lock was pinned.
 *
 * If inside of __schedule(), clock_update_flags will have been
 * shifted left (a left shift is a cheap operation for the fast path
 * to promote %RQCF_REQ_SKIP to %RQCF_ACT_SKIP), so you must use,
 *
 *	if (rq-clock_update_flags >= RQCF_UPDATED)
 *
 * to check if %RQCF_UPDATED is set. It'll never be shifted more than
 * one position though, because the next rq_unpin_lock() will shift it
 * back.
 */
#define RQCF_REQ_SKIP		0x01
#define RQCF_ACT_SKIP		0x02
#define RQCF_UPDATED		0x04

സ്ഥിതം വരിയിൽ ശൂന്യം assert_clock_updated(ഘടന rq *rq)
{
	/*
	 * The only reason for not seeing a clock update since the
	 * last rq_pin_lock() is if we're currently skipping updates.
	 */
	WARN_ON_ONCE(rq->clock_update_flags < RQCF_ACT_SKIP);
}

സ്ഥിതം വരിയിൽ u64 rq_clock(ഘടന rq *rq)
{
	lockdep_assert_rq_held(rq);
	assert_clock_updated(rq);

	തിരികെ rq->clock;
}

സ്ഥിതം വരിയിൽ u64 rq_clock_task(ഘടന rq *rq)
{
	lockdep_assert_rq_held(rq);
	assert_clock_updated(rq);

	തിരികെ rq->clock_task;
}

സ്ഥിതം വരിയിൽ ശൂന്യം rq_clock_skip_update(ഘടന rq *rq)
{
	lockdep_assert_rq_held(rq);
	rq->clock_update_flags |= RQCF_REQ_SKIP;
}

/*
 * See rt task throttling, which is the only time a skip
 * request is canceled.
 */
സ്ഥിതം വരിയിൽ ശൂന്യം rq_clock_cancel_skipupdate(ഘടന rq *rq)
{
	lockdep_assert_rq_held(rq);
	rq->clock_update_flags &= ~RQCF_REQ_SKIP;
}

/*
 * During cpu offlining and rq wide unthrottling, we can trigger
 * an update_rq_clock() for several cfs and rt runqueues (Typically
 * when using list_for_each_entry_*)
 * rq_clock_start_loop_update() can be called after updating the clock
 * once and before iterating over the list to prevent multiple update.
 * After the iterative traversal, we need to call rq_clock_stop_loop_update()
 * to clear RQCF_ACT_SKIP of rq->clock_update_flags.
 */
സ്ഥിതം വരിയിൽ ശൂന്യം rq_clock_start_loop_update(ഘടന rq *rq)
{
	lockdep_assert_rq_held(rq);
	WARN_ON_ONCE(rq->clock_update_flags & RQCF_ACT_SKIP);
	rq->clock_update_flags |= RQCF_ACT_SKIP;
}

സ്ഥിതം വരിയിൽ ശൂന്യം rq_clock_stop_loop_update(ഘടന rq *rq)
{
	lockdep_assert_rq_held(rq);
	rq->clock_update_flags &= ~RQCF_ACT_SKIP;
}

ഘടന rq_flags {
	unsigned long flags;
	ഘടന pin_cookie cookie;
	/*
	 * A copy of (rq::clock_update_flags & RQCF_UPDATED) for the
	 * current pin context is stashed here in case it needs to be
	 * restored in rq_repin_lock().
	 */
	unsigned പൂർണ്ണസംഖ്യ clock_update_flags;
};

ബാഹ്യ ഘടന balance_callback balance_തള്ളുക_callback;

#ifdef CONFIG_SCHED_CLASS_EXT
ബാഹ്യ സ്ഥിരം ഘടന sched_class ext_sched_class;

DECLARE_STATIC_താക്കോൽ_FALSE(__scx_enabled);	/* SCX BPF scheduler loaded */
DECLARE_STATIC_താക്കോൽ_FALSE(__scx_switched_all);	/* all fair class tasks on SCX */

#define scx_enabled()		static_branch_unlikely(&__scx_enabled)
#define scx_switched_all()	static_branch_unlikely(&__scx_switched_all)

സ്ഥിതം വരിയിൽ ശൂന്യം scx_rq_clock_update(ഘടന rq *rq, u64 clock)
{
	എങ്കിൽ (!scx_enabled())
		തിരികെ;
	WRITE_ONCE(rq->scx.clock, clock);
	smp_store_release(&rq->scx.flags, rq->scx.flags | SCX_RQ_CLK_VALID);
}

സ്ഥിതം വരിയിൽ ശൂന്യം scx_rq_clock_invalidate(ഘടന rq *rq)
{
	എങ്കിൽ (!scx_enabled())
		തിരികെ;
	WRITE_ONCE(rq->scx.flags, rq->scx.flags & ~SCX_RQ_CLK_VALID);
}

#അല്ലെങ്കിൽ /* !CONFIG_SCHED_CLASS_EXT: */
#define scx_enabled()		false
#define scx_switched_all()	false

സ്ഥിതം വരിയിൽ ശൂന്യം scx_rq_clock_update(ഘടന rq *rq, u64 clock) {}
സ്ഥിതം വരിയിൽ ശൂന്യം scx_rq_clock_invalidate(ഘടന rq *rq) {}
#endif /* !CONFIG_SCHED_CLASS_EXT */

സ്ഥിതം വരിയിൽ ശൂന്യം assert_balance_callbacks_empty(ഘടന rq *rq)
{
	WARN_ON_ONCE(IS_ENABLED(CONFIG_PROVE_LOCKING) &&
		     rq->balance_callback &&
		     rq->balance_callback != &balance_തള്ളുക_callback);
}

/*
 * Lockdep annotation that avoids accidental unlocks; it's like a
 * sticky/continuous lockdep_assert_held().
 *
 * This avoids code that has access to 'struct rq *rq' (basically everything in
 * the scheduler) from accidentally unlocking the rq if they do not also have a
 * copy of the (on-stack) 'struct rq_flags rf'.
 *
 * Also see Documentation/locking/lockdep-design.rst.
 */
സ്ഥിതം വരിയിൽ ശൂന്യം rq_pin_lock(ഘടന rq *rq, ഘടന rq_flags *rf)
{
	rf->cookie = lockdep_pin_lock(__rq_lockp(rq));

	rq->clock_update_flags &= (RQCF_REQ_SKIP|RQCF_ACT_SKIP);
	rf->clock_update_flags = 0;
	assert_balance_callbacks_empty(rq);
}

സ്ഥിതം വരിയിൽ ശൂന്യം rq_unpin_lock(ഘടന rq *rq, ഘടന rq_flags *rf)
{
	എങ്കിൽ (rq->clock_update_flags > RQCF_ACT_SKIP)
		rf->clock_update_flags = RQCF_UPDATED;

	scx_rq_clock_invalidate(rq);
	lockdep_unpin_lock(__rq_lockp(rq), rf->cookie);
}

സ്ഥിതം വരിയിൽ ശൂന്യം rq_repin_lock(ഘടന rq *rq, ഘടന rq_flags *rf)
{
	lockdep_repin_lock(__rq_lockp(rq), rf->cookie);

	/*
	 * Restore the value we stashed in @rf for this pin context.
	 */
	rq->clock_update_flags |= rf->clock_update_flags;
}

#define __task_rq_lock(...) __acquire_ret(___task_rq_lock(__VA_ARGS__), __rq_lockp(__ret))
ബാഹ്യ ഘടന rq *___task_rq_lock(ഘടന task_struct *p, ഘടന rq_flags *rf) __acquires_ret;

#define task_rq_lock(...) __acquire_ret(_task_rq_lock(__VA_ARGS__), __rq_lockp(__ret))
ബാഹ്യ ഘടന rq *_task_rq_lock(ഘടന task_struct *p, ഘടന rq_flags *rf)
	__acquires(&p->pi_lock) __acquires_ret;

സ്ഥിതം വരിയിൽ ശൂന്യം
__task_rq_unlock(ഘടന rq *rq, ഘടന task_struct *p, ഘടന rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_lock(rq, rf);
	raw_spin_rq_unlock(rq);
}

സ്ഥിതം വരിയിൽ ശൂന്യം
task_rq_unlock(ഘടന rq *rq, ഘടന task_struct *p, ഘടന rq_flags *rf)
	__releases(__rq_lockp(rq), &p->pi_lock)
{
	__task_rq_unlock(rq, p, rf);
	raw_spin_unlock_irqrestore(&p->pi_lock, rf->flags);
}

DEFINE_LOCK_GUARD_1(task_rq_lock, ഘടന task_struct,
		    _T->rq = task_rq_lock(_T->lock, &_T->rf),
		    task_rq_unlock(_T->rq, _T->lock, &_T->rf),
		    ഘടന rq *rq; ഘടന rq_flags rf)
DECLARE_LOCK_GUARD_1_ATTRS(task_rq_lock, __acquires(_T->pi_lock), __releases((*(ഘടന task_struct **)_T)->pi_lock))
#define class_task_rq_lock_constructor(_T) WITH_LOCK_GUARD_1_ATTRS(task_rq_lock, _T)

DEFINE_LOCK_GUARD_1(__task_rq_lock, ഘടന task_struct,
		    _T->rq = __task_rq_lock(_T->lock, &_T->rf),
		    __task_rq_unlock(_T->rq, _T->lock, &_T->rf),
		    ഘടന rq *rq; ഘടന rq_flags rf)

സ്ഥിതം വരിയിൽ ശൂന്യം rq_lock_irqsave(ഘടന rq *rq, ഘടന rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_lock_irqsave(rq, rf->flags);
	rq_pin_lock(rq, rf);
}

സ്ഥിതം വരിയിൽ ശൂന്യം rq_lock_irq(ഘടന rq *rq, ഘടന rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_lock_irq(rq);
	rq_pin_lock(rq, rf);
}

സ്ഥിതം വരിയിൽ ശൂന്യം rq_lock(ഘടന rq *rq, ഘടന rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_lock(rq);
	rq_pin_lock(rq, rf);
}

സ്ഥിതം വരിയിൽ ശൂന്യം rq_unlock_irqrestore(ഘടന rq *rq, ഘടന rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_lock(rq, rf);
	raw_spin_rq_unlock_irqrestore(rq, rf->flags);
}

സ്ഥിതം വരിയിൽ ശൂന്യം rq_unlock_irq(ഘടന rq *rq, ഘടന rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_lock(rq, rf);
	raw_spin_rq_unlock_irq(rq);
}

സ്ഥിതം വരിയിൽ ശൂന്യം rq_unlock(ഘടന rq *rq, ഘടന rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_lock(rq, rf);
	raw_spin_rq_unlock(rq);
}

DEFINE_LOCK_GUARD_1(rq_lock, ഘടന rq,
		    rq_lock(_T->lock, &_T->rf),
		    rq_unlock(_T->lock, &_T->rf),
		    ഘടന rq_flags rf)

DECLARE_LOCK_GUARD_1_ATTRS(rq_lock, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(ഘടന rq **)_T)));
#define class_rq_lock_constructor(_T) WITH_LOCK_GUARD_1_ATTRS(rq_lock, _T)

DEFINE_LOCK_GUARD_1(rq_lock_irq, ഘടന rq,
		    rq_lock_irq(_T->lock, &_T->rf),
		    rq_unlock_irq(_T->lock, &_T->rf),
		    ഘടന rq_flags rf)

DECLARE_LOCK_GUARD_1_ATTRS(rq_lock_irq, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(ഘടന rq **)_T)));
#define class_rq_lock_irq_constructor(_T) WITH_LOCK_GUARD_1_ATTRS(rq_lock_irq, _T)

DEFINE_LOCK_GUARD_1(rq_lock_irqsave, ഘടന rq,
		    rq_lock_irqsave(_T->lock, &_T->rf),
		    rq_unlock_irqrestore(_T->lock, &_T->rf),
		    ഘടന rq_flags rf)

DECLARE_LOCK_GUARD_1_ATTRS(rq_lock_irqsave, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(ഘടന rq **)_T)));
#define class_rq_lock_irqsave_constructor(_T) WITH_LOCK_GUARD_1_ATTRS(rq_lock_irqsave, _T)

DEFINE_LOCK_GUARD_1(raw_spin_rq_lock_irqsave, ഘടന rq,
		    raw_spin_rq_lock_irqsave(_T->lock, _T->flags),
		    raw_spin_rq_unlock_irqrestore(_T->lock, _T->flags),
		    unsigned long flags)

DECLARE_LOCK_GUARD_1_ATTRS(raw_spin_rq_lock_irqsave, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(ഘടന rq **)_T)));
#define class_raw_spin_rq_lock_irqsave_constructor(_T) \
	WITH_LOCK_GUARD_1_ATTRS(raw_spin_rq_lock_irqsave, _T)

#define this_rq_lock_irq(...) __acquire_ret(_this_rq_lock_irq(__VA_ARGS__), __rq_lockp(__ret))
സ്ഥിതം വരിയിൽ ഘടന rq *_this_rq_lock_irq(ഘടന rq_flags *rf) __acquires_ret
{
	ഘടന rq *rq;

	local_irq_disable();
	rq = this_rq();
	rq_lock(rq, rf);

	തിരികെ rq;
}

#ifdef CONFIG_NUMA

എണ്ണൽ numa_topology_type {
	NUMA_DIRECT,
	NUMA_GLUELESS_MESH,
	NUMA_BACKPLANE,
};

ബാഹ്യ എണ്ണൽ numa_topology_type sched_numa_topology_type;
ബാഹ്യ പൂർണ്ണസംഖ്യ sched_max_numa_distance;
ബാഹ്യ bool find_numa_distance(പൂർണ്ണസംഖ്യ distance);
ബാഹ്യ ശൂന്യം sched_init_numa(പൂർണ്ണസംഖ്യ offline_കണ്ണി);
ബാഹ്യ ശൂന്യം sched_update_numa(പൂർണ്ണസംഖ്യ cpu, bool online);
ബാഹ്യ ശൂന്യം sched_domains_numa_masks_set(unsigned പൂർണ്ണസംഖ്യ cpu);
ബാഹ്യ ശൂന്യം sched_domains_numa_masks_clear(unsigned പൂർണ്ണസംഖ്യ cpu);
ബാഹ്യ പൂർണ്ണസംഖ്യ sched_numa_find_closest(സ്ഥിരം ഘടന cpumask *cpus, പൂർണ്ണസംഖ്യ cpu);

#അല്ലെങ്കിൽ /* !CONFIG_NUMA: */

സ്ഥിതം വരിയിൽ ശൂന്യം sched_init_numa(പൂർണ്ണസംഖ്യ offline_കണ്ണി) { }
സ്ഥിതം വരിയിൽ ശൂന്യം sched_update_numa(പൂർണ്ണസംഖ്യ cpu, bool online) { }
സ്ഥിതം വരിയിൽ ശൂന്യം sched_domains_numa_masks_set(unsigned പൂർണ്ണസംഖ്യ cpu) { }
സ്ഥിതം വരിയിൽ ശൂന്യം sched_domains_numa_masks_clear(unsigned പൂർണ്ണസംഖ്യ cpu) { }

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ sched_numa_find_closest(സ്ഥിരം ഘടന cpumask *cpus, പൂർണ്ണസംഖ്യ cpu)
{
	തിരികെ nr_cpu_ids;
}

#endif /* !CONFIG_NUMA */

#ifdef CONFIG_NUMA_BALANCING

/* The regions in numa_faults array from task_struct */
എണ്ണൽ numa_faults_stats {
	NUMA_MEM = 0,
	NUMA_CPU,
	NUMA_MEMBUF,
	NUMA_CPUBUF
};

ബാഹ്യ ശൂന്യം sched_setnuma(ഘടന task_struct *p, പൂർണ്ണസംഖ്യ കണ്ണി);
ബാഹ്യ പൂർണ്ണസംഖ്യ migrate_task_to(ഘടന task_struct *p, പൂർണ്ണസംഖ്യ cpu);
ബാഹ്യ പൂർണ്ണസംഖ്യ migrate_swap(ഘടന task_struct *p, ഘടന task_struct *t,
			പൂർണ്ണസംഖ്യ cpu, പൂർണ്ണസംഖ്യ scpu);
ബാഹ്യ ശൂന്യം init_numa_balancing(u64 clone_flags, ഘടന task_struct *p);

#അല്ലെങ്കിൽ /* !CONFIG_NUMA_BALANCING: */

സ്ഥിതം വരിയിൽ ശൂന്യം
init_numa_balancing(u64 clone_flags, ഘടന task_struct *p)
{
}

#endif /* !CONFIG_NUMA_BALANCING */

പൂർണ്ണസംഖ്യ task_llc(സ്ഥിരം ഘടന task_struct *p);

സ്ഥിതം വരിയിൽ ശൂന്യം
queue_balance_callback(ഘടന rq *rq,
		       ഘടന balance_callback *head,
		       ശൂന്യം (*func)(ഘടന rq *rq))
{
	lockdep_assert_rq_held(rq);

	/*
	 * Don't (re)queue an already queued item; nor queue anything when
	 * balance_push() is active, see the comment with
	 * balance_push_callback.
	 */
	എങ്കിൽ (unlikely(head->next || rq->balance_callback == &balance_തള്ളുക_callback))
		തിരികെ;

	head->func = func;
	head->next = rq->balance_callback;
	rq->balance_callback = head;
}

#define rcu_dereference_sched_domain(p) \
	rcu_dereference_all_check((p), lockdep_is_held(&sched_domains_mutex))

/*
 * The domain tree (rq->sd) is protected by RCU's quiescent state transition.
 * See destroy_sched_domains: call_rcu for details.
 *
 * The domain tree of any CPU may only be accessed from within
 * preempt-disabled sections.
 */
#define for_each_domain(cpu, __sd) \
	വേണ്ടി (__sd = rcu_dereference_sched_domain(cpu_rq(cpu)->sd); \
			__sd; __sd = __sd->parent)

/* A mask of all the SD flags that have the SDF_SHARED_CHILD metaflag */
#define SD_FLAG(name, mflags) (name * !!((mflags) & SDF_SHARED_CHILD)) |
സ്ഥിതം സ്ഥിരം unsigned പൂർണ്ണസംഖ്യ SD_SHARED_CHILD_MASK =
#അന്തര്ഭാവ <linux/sched/sd_flags.h>
0;
#undef SD_FLAG

/**
 * highest_flag_domain - Return highest sched_domain containing flag.
 * @cpu:	The CPU whose highest level of sched domain is to
 *		be returned.
 * @flag:	The flag to check for the highest sched_domain
 *		for the given CPU.
 *
 * Returns the highest sched_domain of a CPU which contains @flag. If @flag has
 * the SDF_SHARED_CHILD metaflag, all the children domains also have @flag.
 */
സ്ഥിതം വരിയിൽ ഘടന sched_domain *highest_flag_domain(പൂർണ്ണസംഖ്യ cpu, പൂർണ്ണസംഖ്യ flag)
{
	ഘടന sched_domain *sd, *hsd = ശൂന്യ;

	for_each_domain(cpu, sd) {
		എങ്കിൽ (sd->flags & flag) {
			hsd = sd;
			തുടരുക;
		}

		/*
		 * Stop the search if @flag is known to be shared at lower
		 * levels. It will not be found further up.
		 */
		എങ്കിൽ (flag & SD_SHARED_CHILD_MASK)
			നിർത്തുക;
	}

	തിരികെ hsd;
}

സ്ഥിതം വരിയിൽ ഘടന sched_domain *lowest_flag_domain(പൂർണ്ണസംഖ്യ cpu, പൂർണ്ണസംഖ്യ flag)
{
	ഘടന sched_domain *sd;

	for_each_domain(cpu, sd) {
		എങ്കിൽ (sd->flags & flag)
			നിർത്തുക;
	}

	തിരികെ sd;
}

DECLARE_PER_CPU(ഘടന sched_domain __rcu *, sd_llc);
DECLARE_PER_CPU(പൂർണ്ണസംഖ്യ, sd_llc_size);
DECLARE_PER_CPU(പൂർണ്ണസംഖ്യ, sd_llc_id);
DECLARE_PER_CPU(പൂർണ്ണസംഖ്യ, sd_share_id);
DECLARE_PER_CPU(ഘടന sched_domain_shared __rcu *, sd_llc_shared);
DECLARE_PER_CPU(ഘടന sched_domain_shared __rcu *, sd_balance_shared);
DECLARE_PER_CPU(ഘടന sched_domain __rcu *, sd_numa);
DECLARE_PER_CPU(ഘടന sched_domain __rcu *, sd_asym_packing);
DECLARE_PER_CPU(ഘടന sched_domain __rcu *, sd_asym_cpucapacity);

ബാഹ്യ ഘടന static_താക്കോൽ_false sched_asym_cpucapacity;
ബാഹ്യ ഘടന static_താക്കോൽ_false sched_cluster_active;

സ്ഥിതം __always_inline bool sched_asym_cpucap_active(ശൂന്യം)
{
	തിരികെ static_branch_unlikely(&sched_asym_cpucapacity);
}

ഘടന sched_group_capacity {
	atomic_t		ref;
	/*
	 * CPU capacity of this group, SCHED_CAPACITY_SCALE being max capacity
	 * for a single CPU.
	 */
	unsigned long		capacity;
	unsigned long		min_capacity;		/* Min per-CPU capacity in group */
	unsigned long		max_capacity;		/* Max per-CPU capacity in group */
	unsigned long		next_update;
	പൂർണ്ണസംഖ്യ			imbalance;		/* XXX unrelated to capacity but shared group state */

	പൂർണ്ണസംഖ്യ			id;

	unsigned long		cpumask[];		/* Balance mask */
};

ഘടന sched_group {
	ഘടന sched_group	*next;			/* Must be a circular list */
	atomic_t		ref;

	unsigned പൂർണ്ണസംഖ്യ		group_weight;
	unsigned പൂർണ്ണസംഖ്യ		cores;
	ഘടന sched_group_capacity *sgc;
	പൂർണ്ണസംഖ്യ			asym_prefer_cpu;	/* CPU of highest priority in group */
	പൂർണ്ണസംഖ്യ			flags;

	/*
	 * The CPUs this group covers.
	 *
	 * NOTE: this field is variable length. (Allocated dynamically
	 * by attaching extra space to the end of the structure,
	 * depending on how many CPUs the kernel has booted up with)
	 */
	unsigned long		cpumask[];
};

സ്ഥിതം വരിയിൽ ഘടന cpumask *sched_group_span(ഘടന sched_group *sg)
{
	തിരികെ to_cpumask(sg->cpumask);
}

/*
 * See build_balance_mask().
 */
സ്ഥിതം വരിയിൽ ഘടന cpumask *group_balance_mask(ഘടന sched_group *sg)
{
	തിരികെ to_cpumask(sg->sgc->cpumask);
}

ബാഹ്യ പൂർണ്ണസംഖ്യ group_balance_cpu(ഘടന sched_group *sg);

ബാഹ്യ ശൂന്യം update_sched_domain_debugfs(ശൂന്യം);
ബാഹ്യ ശൂന്യം dirty_sched_domain_sysctl(പൂർണ്ണസംഖ്യ cpu);

ബാഹ്യ പൂർണ്ണസംഖ്യ sched_update_scaling(ശൂന്യം);

സ്ഥിതം വരിയിൽ സ്ഥിരം ഘടന cpumask *task_user_cpus(ഘടന task_struct *p)
{
	എങ്കിൽ (!p->user_cpus_ptr)
		തിരികെ cpu_possible_mask; /* &init_task.cpus_mask */
	തിരികെ p->user_cpus_ptr;
}

#ifdef CONFIG_CGROUP_SCHED

/*
 * Return the group to which this tasks belongs.
 *
 * We cannot use task_css() and friends because the cgroup subsystem
 * changes that value before the cgroup_subsys::attach() method is called,
 * therefore we cannot pin it and might observe the wrong value.
 *
 * The same is true for autogroup's p->signal->autogroup->tg, the autogroup
 * core changes this before calling sched_move_task().
 *
 * Instead we use a 'copy' which is updated from sched_move_task() while
 * holding both task_struct::pi_lock and rq::lock.
 */
സ്ഥിതം വരിയിൽ ഘടന task_group *task_group(ഘടന task_struct *p)
{
	തിരികെ p->sched_task_group;
}

#ifdef CONFIG_FAIR_GROUP_SCHED
/*
 * Defined here to be available before stats.h is included, since
 * stats.h has dependencies on things defined later in this file.
 */
ഘടന cfs_tg_state {
	ഘടന cfs_rq		cfs_rq;
	ഘടന sched_entity	se;
	ഘടന sched_statistics	stats;
} __no_randomize_layout;

/* Access a specific CPU's cfs_rq from a task group */
സ്ഥിതം വരിയിൽ ഘടന cfs_rq *tg_cfs_rq(ഘടന task_group *tg, പൂർണ്ണസംഖ്യ cpu)
{
	തിരികെ per_cpu_ptr(tg->cfs_rq, cpu);
}

സ്ഥിതം വരിയിൽ ഘടന sched_entity *tg_se(ഘടന task_group *tg, പൂർണ്ണസംഖ്യ cpu)
{
	ഘടന cfs_tg_state *state;

	എങ്കിൽ (is_വേര്_task_group(tg))
		തിരികെ ശൂന്യ;

	state = container_of(tg_cfs_rq(tg, cpu), ഘടന cfs_tg_state, cfs_rq);
	തിരികെ &state->se;
}

സ്ഥിതം വരിയിൽ ഘടന sched_entity *cfs_rq_se(ഘടന cfs_rq *cfs_rq)
{
	ഘടന cfs_tg_state *state;

	എങ്കിൽ (is_വേര്_task_group(cfs_rq->tg))
		തിരികെ ശൂന്യ;

	state = container_of(cfs_rq, ഘടന cfs_tg_state, cfs_rq);
	തിരികെ &state->se;
}
#endif

/* Change a task's cfs_rq and parent entity if it moves across CPUs/groups */
സ്ഥിതം വരിയിൽ ശൂന്യം set_task_rq(ഘടന task_struct *p, unsigned പൂർണ്ണസംഖ്യ cpu)
{
#എങ്കിൽ defined(CONFIG_FAIR_GROUP_SCHED) || defined(CONFIG_RT_GROUP_SCHED)
	ഘടന task_group *tg = task_group(p);
#endif

#ifdef CONFIG_FAIR_GROUP_SCHED
	set_task_rq_fair(&p->se, p->se.cfs_rq, tg_cfs_rq(tg, cpu));
	p->se.cfs_rq = tg_cfs_rq(tg, cpu);
	p->se.parent = tg_se(tg, cpu);
	p->se.depth = p->se.parent ? p->se.parent->depth + 1 : 0;
#endif

#ifdef CONFIG_RT_GROUP_SCHED
	/*
	 * p->rt.rt_rq is NULL initially and it is easier to assign
	 * root_task_group's rt_rq than switching in rt_rq_of_se()
	 * Clobbers tg(!)
	 */
	എങ്കിൽ (!rt_group_sched_enabled())
		tg = &വേര്_task_group;
	p->rt.rt_rq  = tg->rt_rq[cpu];
	p->rt.parent = tg->rt_se[cpu];
#endif /* CONFIG_RT_GROUP_SCHED */
}

#അല്ലെങ്കിൽ /* !CONFIG_CGROUP_SCHED: */

സ്ഥിതം വരിയിൽ ശൂന്യം set_task_rq(ഘടന task_struct *p, unsigned പൂർണ്ണസംഖ്യ cpu) { }

സ്ഥിതം വരിയിൽ ഘടന task_group *task_group(ഘടന task_struct *p)
{
	തിരികെ ശൂന്യ;
}

#endif /* !CONFIG_CGROUP_SCHED */

സ്ഥിതം വരിയിൽ ശൂന്യം __set_task_cpu(ഘടന task_struct *p, unsigned പൂർണ്ണസംഖ്യ cpu)
{
	set_task_rq(p, cpu);
#ifdef CONFIG_SMP
	/*
	 * After ->cpu is set up to a new value, task_rq_lock(p, ...) can be
	 * successfully executed on another CPU. We must ensure that updates of
	 * per-task data have been completed by this moment.
	 */
	smp_wmb();
	WRITE_ONCE(task_thread_info(p)->cpu, cpu);
	p->wake_cpu = cpu;
	rseq_sched_set_ids_changed(p);
#endif /* CONFIG_SMP */
}

/*
 * Tunables:
 */

#define SCHED_FEAT(name, enabled)	\
	__SCHED_FEAT_##name ,

എണ്ണൽ {
#അന്തര്ഭാവ "features.h"
	__SCHED_FEAT_NR,
};

#undef SCHED_FEAT

/*
 * To support run-time toggling of sched features, all the translation units
 * (but core.c) reference the sysctl_sched_features defined in core.c.
 */
ബാഹ്യ __read_mostly unsigned പൂർണ്ണസംഖ്യ sysctl_sched_features;

#ifdef CONFIG_JUMP_LABEL

സംഘം sched_feat_താക്കോൽ {
	ഘടന static_താക്കോൽ_true	താക്കോൽ_true;
	ഘടന static_താക്കോൽ_false	താക്കോൽ_false;
};

#define sched_feat_branch_true(താക്കോൽ)	static_branch_likely(&(താക്കോൽ)->താക്കോൽ_true)
#define sched_feat_branch_false(താക്കോൽ)	static_branch_unlikely(&(താക്കോൽ)->താക്കോൽ_false)

#define SCHED_FEAT(name, enabled)			\
സ്ഥിതം __always_inline bool				\
static_branch_##name(സംഘം sched_feat_താക്കോൽ *താക്കോൽ) 	\
{							\
	തിരികെ sched_feat_branch_##enabled(താക്കോൽ);	\
}

#അന്തര്ഭാവ "features.h"
#undef SCHED_FEAT

ബാഹ്യ സംഘം sched_feat_താക്കോൽ sched_feat_താക്കോലുകൾ[__SCHED_FEAT_NR];
#define sched_feat(x) (static_branch_##x(&sched_feat_താക്കോലുകൾ[__SCHED_FEAT_##x]))

#അല്ലെങ്കിൽ /* !CONFIG_JUMP_LABEL: */

#define sched_feat(x) (sysctl_sched_features & (1UL << __SCHED_FEAT_##x))

#endif /* !CONFIG_JUMP_LABEL */

ബാഹ്യ ഘടന static_താക്കോൽ_false sched_numa_balancing;
ബാഹ്യ ഘടന static_താക്കോൽ_false sched_schedstats;

സ്ഥിതം വരിയിൽ u64 global_rt_period(ശൂന്യം)
{
	തിരികെ (u64)sysctl_sched_rt_period * NSEC_PER_USEC;
}

സ്ഥിതം വരിയിൽ u64 global_rt_runtime(ശൂന്യം)
{
	എങ്കിൽ (sysctl_sched_rt_runtime < 0)
		തിരികെ RUNTIME_INF;

	തിരികെ (u64)sysctl_sched_rt_runtime * NSEC_PER_USEC;
}

/*
 * Is p the current execution context?
 */
സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ task_current(ഘടന rq *rq, ഘടന task_struct *p)
{
	തിരികെ rq->curr == p;
}

/*
 * Is p the current scheduling context?
 *
 * Note that it might be the current execution context at the same time if
 * rq->curr == rq->donor == p.
 */
സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ task_current_donor(ഘടന rq *rq, ഘടന task_struct *p)
{
	തിരികെ rq->donor == p;
}

സ്ഥിതം വരിയിൽ bool task_is_blocked(ഘടന task_struct *p)
{
	എങ്കിൽ (!sched_proxy_exec())
		തിരികെ false;

	തിരികെ !!p->blocked_on;
}

#ifdef CONFIG_SCHED_PROXY_EXEC
ശൂന്യം sched_proxy_block_task(ഘടന rq *rq, ഘടന task_struct *p);
#അല്ലെങ്കിൽ
സ്ഥിതം വരിയിൽ ശൂന്യം sched_proxy_block_task(ഘടന rq *rq, ഘടന task_struct *p) {}
#endif

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ task_on_cpu(ഘടന rq *rq, ഘടന task_struct *p)
{
	തിരികെ p->on_cpu;
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ task_on_rq_queued(ഘടന task_struct *p)
{
	തിരികെ READ_ONCE(p->on_rq) == TASK_ON_RQ_QUEUED;
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ task_on_rq_migrating(ഘടന task_struct *p)
{
	തിരികെ READ_ONCE(p->on_rq) == TASK_ON_RQ_MIGRATING;
}

/* Wake flags. The first three directly map to some SD flag value */
#define WF_EXEC			0x02 /* Wakeup after exec; maps to SD_BALANCE_EXEC */
#define WF_FORK			0x04 /* Wakeup after fork; maps to SD_BALANCE_FORK */
#define WF_TTWU			0x08 /* Wakeup;            maps to SD_BALANCE_WAKE */
/*
 * Hint that the caller expects the waker to sleep soon.
 * Scheduler classes may use it for placement or preemption.
 * Callers must not rely on it to prevent migration,
 * preserve CPU locality or make the wakee run next.
 */
#define WF_SYNC			0x10
#define WF_MIGRATED		0x20 /* Internal use, task got migrated */
#define WF_CURRENT_CPU		0x40 /* Prefer to move the wakee to the current CPU. */
#define WF_RQ_SELECTED		0x80 /* ->select_task_rq() was called */
#define WF_TTWU_RQ		0x100 /* Wakeup completed through ttwu_runnable() */

static_assert(WF_EXEC == SD_BALANCE_EXEC);
static_assert(WF_FORK == SD_BALANCE_FORK);
static_assert(WF_TTWU == SD_BALANCE_WAKE);

/*
 * To aid in avoiding the subversion of "niceness" due to uneven distribution
 * of tasks with abnormal "nice" values across CPUs the contribution that
 * each task makes to its run queue's load is weighted according to its
 * scheduling class and "nice" value. For SCHED_NORMAL tasks this is just a
 * scaled version of the new time slice allocation that they receive on time
 * slice expiry etc.
 */

#define WEIGHT_IDLEPRIO		3
#define WMULT_IDLEPRIO		1431655765

ബാഹ്യ സ്ഥിരം പൂർണ്ണസംഖ്യ		sched_prio_to_weight[40];
ബാഹ്യ സ്ഥിരം u32		sched_prio_to_wmult[40];

/*
 * {de,en}queue flags:
 *
 * SLEEP/WAKEUP - task is no-longer/just-became runnable
 *
 * SAVE/RESTORE - an otherwise spurious dequeue/enqueue, done to ensure tasks
 *                are in a known state which allows modification. Such pairs
 *                should preserve as much state as possible.
 *
 * MOVE - paired with SAVE/RESTORE, explicitly does not preserve the location
 *        in the runqueue. IOW the priority is allowed to change. Callers
 *        must expect to deal with balance callbacks.
 *
 * NOCLOCK - skip the update_rq_clock() (avoids double updates)
 *
 * MIGRATION - p->on_rq == TASK_ON_RQ_MIGRATING (used for DEADLINE)
 *
 * DELAYED - de/re-queue a sched_delayed task
 *
 * CLASS - going to update p->sched_class; makes sched_change call the
 *         various switch methods.
 *
 * ENQUEUE_HEAD      - place at front of runqueue (tail if not specified)
 * ENQUEUE_REPLENISH - CBS (replenish runtime and postpone deadline)
 * ENQUEUE_MIGRATED  - the task was migrated during wakeup
 * ENQUEUE_RQ_SELECTED - ->select_task_rq() was called
 *
 * XXX SAVE/RESTORE in combination with CLASS doesn't really make sense, but
 * SCHED_DEADLINE seems to rely on this for now.
 */

#define DEQUEUE_SLEEP		0x0001 /* Matches ENQUEUE_WAKEUP */
#define DEQUEUE_SAVE		0x0002 /* Matches ENQUEUE_RESTORE */
#define DEQUEUE_MOVE		0x0004 /* Matches ENQUEUE_MOVE */
#define DEQUEUE_NOCLOCK		0x0008 /* Matches ENQUEUE_NOCLOCK */

#define DEQUEUE_MIGRATING	0x0010 /* Matches ENQUEUE_MIGRATING */
#define DEQUEUE_DELAYED		0x0020 /* Matches ENQUEUE_DELAYED */
#define DEQUEUE_CLASS		0x0040 /* Matches ENQUEUE_CLASS */

#define DEQUEUE_SPECIAL		0x00010000
#define DEQUEUE_THROTTLE	0x00020000

#define ENQUEUE_WAKEUP		0x0001
#define ENQUEUE_RESTORE		0x0002
#define ENQUEUE_MOVE		0x0004
#define ENQUEUE_NOCLOCK		0x0008

#define ENQUEUE_MIGRATING	0x0010
#define ENQUEUE_DELAYED		0x0020
#define ENQUEUE_CLASS		0x0040

#define ENQUEUE_HEAD		0x00010000
#define ENQUEUE_REPLENISH	0x00020000
#define ENQUEUE_MIGRATED	0x00040000
#define ENQUEUE_INITIAL		0x00080000
#define ENQUEUE_RQ_SELECTED	0x00100000
#define ENQUEUE_QUEUED		0x00200000

#define RETRY_TASK		((ശൂന്യം *)-1UL)

ഘടന affinity_context {
	സ്ഥിരം ഘടന cpumask	*new_mask;
	ഘടന cpumask		*user_mask;
	unsigned പൂർണ്ണസംഖ്യ		flags;
};

ബാഹ്യ s64 update_curr_common(ഘടന rq *rq);

എണ്ണൽ snt_e {
	SNT_NORMAL,	/* set_next_task() */
	SNT_PICK,	/* put_prev_set_next_task(): prev != next */
	SNT_REPICK,	/* put_prev_set_next_task(): prev == next */
};

ഘടന sched_class {

#ifdef CONFIG_UCLAMP_TASK
	പൂർണ്ണസംഖ്യ uclamp_enabled;
#endif

	/*
	 * move_queued_task/activate_task/enqueue_task: rq->lock
	 * ttwu_do_activate/activate_task/enqueue_task: rq->lock
	 * wake_up_new_task/activate_task/enqueue_task: task_rq_lock
	 * ttwu_runnable/enqueue_task: task_rq_lock
	 * proxy_task_current: rq->lock
	 * sched_change_end
	 */
	ശൂന്യം (*enqueue_task) (ഘടന rq *rq, ഘടന task_struct *p, പൂർണ്ണസംഖ്യ flags);
	/*
	 * move_queued_task/deactivate_task/dequeue_task: rq->lock
	 * __schedule/block_task/dequeue_task: rq->lock
	 * proxy_task_current: rq->lock
	 * wait_task_inactive: task_rq_lock
	 * sched_change_begin
	 */
	bool (*dequeue_task) (ഘടന rq *rq, ഘടന task_struct *p, പൂർണ്ണസംഖ്യ flags);

	/*
	 * do_sched_yield: rq->lock
	 */
	ശൂന്യം (*yield_task)   (ഘടന rq *rq);
	/*
	 * yield_to: rq->lock (double)
	 */
	bool (*yield_to_task)(ഘടന rq *rq, ഘടന task_struct *p);

	/*
	 * move_queued_task: rq->lock
	 * __migrate_swap_task: rq->lock
	 * ttwu_do_activate: rq->lock
	 * ttwu_runnable: task_rq_lock
	 * wake_up_new_task: task_rq_lock
	 */
	ശൂന്യം (*wakeup_preempt)(ഘടന rq *rq, ഘടന task_struct *p, പൂർണ്ണസംഖ്യ flags);

	/*
	 * schedule/pick_next_task/prev_balance: rq->lock
	 */
	പൂർണ്ണസംഖ്യ (*balance)(ഘടന rq *rq, ഘടന rq_flags *rf);

	/*
	 * schedule/pick_next_task: rq->lock
	 */
	ഘടന task_struct *(*pick_task)(ഘടന rq *rq, ഘടന rq_flags *rf);

	/*
	 * sched_change:
	 * __schedule: rq->lock
	 */
	ശൂന്യം (*put_prev_task)(ഘടന rq *rq, ഘടന task_struct *p, ഘടന task_struct *next);
	ശൂന്യം (*set_next_task)(ഘടന rq *rq, ഘടന task_struct *p, എണ്ണൽ snt_e type);

	/*
	 * select_task_rq: p->pi_lock
	 * sched_exec: p->pi_lock
	 */
	പൂർണ്ണസംഖ്യ  (*select_task_rq)(ഘടന task_struct *p, പൂർണ്ണസംഖ്യ task_cpu, പൂർണ്ണസംഖ്യ flags);

	/*
	 * set_task_cpu: p->pi_lock || rq->lock (ttwu like)
	 */
	ശൂന്യം (*migrate_task_rq)(ഘടന task_struct *p, പൂർണ്ണസംഖ്യ new_cpu);

	/*
	 * ttwu_do_activate: rq->lock
	 * wake_up_new_task: task_rq_lock
	 */
	ശൂന്യം (*task_woken)(ഘടന rq *this_rq, ഘടന task_struct *task);

	/*
	 * do_set_cpus_allowed: task_rq_lock + sched_change
	 */
	ശൂന്യം (*set_cpus_allowed)(ഘടന task_struct *p, ഘടന affinity_context *ctx);

	/*
	 * sched_set_rq_{on,off}line: rq->lock
	 */
	ശൂന്യം (*rq_online)(ഘടന rq *rq);
	ശൂന്യം (*rq_offline)(ഘടന rq *rq);

	/*
	 * push_cpu_stop: p->pi_lock && rq->lock
	 */
	ഘടന rq *(*find_lock_rq)(ഘടന task_struct *p, ഘടന rq *rq);

	/*
	 * hrtick: rq->lock
	 * sched_tick: rq->lock
	 * sched_tick_remote: rq->lock
	 */
	ശൂന്യം (*task_tick)(ഘടന rq *rq, ഘടന task_struct *p, പൂർണ്ണസംഖ്യ queued);
	/*
	 * sched_cgroup_fork: p->pi_lock
	 */
	ശൂന്യം (*task_fork)(ഘടന task_struct *p);
	/*
	 * finish_task_switch: no locks
	 */
	ശൂന്യം (*task_dead)(ഘടന task_struct *p);

	/*
	 * sched_change
	 */
	ശൂന്യം (*switching_from)(ഘടന rq *this_rq, ഘടന task_struct *task);
	ശൂന്യം (*switched_from) (ഘടന rq *this_rq, ഘടന task_struct *task);
	ശൂന്യം (*switching_to)  (ഘടന rq *this_rq, ഘടന task_struct *task);
	ശൂന്യം (*switched_to)   (ഘടന rq *this_rq, ഘടന task_struct *task);
	u64  (*get_prio)     (ഘടന rq *this_rq, ഘടന task_struct *task);
	ശൂന്യം (*prio_changed) (ഘടന rq *this_rq, ഘടന task_struct *task,
			      u64 oldprio);

	/*
	 * set_load_weight: task_rq_lock + sched_change
	 * __setscheduler_parms: task_rq_lock + sched_change
	 */
	ശൂന്യം (*reweight_task)(ഘടന rq *this_rq, ഘടന task_struct *task,
			      സ്ഥിരം ഘടന load_weight *lw);

	/*
	 * sched_rr_get_interval: task_rq_lock
	 */
	unsigned പൂർണ്ണസംഖ്യ (*get_rr_interval)(ഘടന rq *rq,
					ഘടന task_struct *task);

	/*
	 * task_sched_runtime: task_rq_lock
	 */
	ശൂന്യം (*update_curr)(ഘടന rq *rq);

#ifdef CONFIG_FAIR_GROUP_SCHED
	/*
	 * sched_change_group: task_rq_lock + sched_change
	 */
	ശൂന്യം (*task_change_group)(ഘടന task_struct *p);
#endif

#ifdef CONFIG_SCHED_CORE
	/*
	 * pick_next_task: rq->lock
	 * try_steal_cookie: rq->lock (double)
	 */
	പൂർണ്ണസംഖ്യ (*task_is_throttled)(ഘടന task_struct *p, പൂർണ്ണസംഖ്യ cpu);
#endif
};

സ്ഥിതം വരിയിൽ ശൂന്യം put_prev_task(ഘടന rq *rq, ഘടന task_struct *prev)
{
	WARN_ON_ONCE(rq->donor != prev);
	prev->sched_class->put_prev_task(rq, prev, ശൂന്യ);
}

സ്ഥിതം വരിയിൽ ശൂന്യം set_next_task(ഘടന rq *rq, ഘടന task_struct *next)
{
	next->sched_class->set_next_task(rq, next, SNT_NORMAL);
}

സ്ഥിതം വരിയിൽ ശൂന്യം
__put_prev_set_next_dl_server(ഘടന rq *rq,
			      ഘടന task_struct *prev,
			      ഘടന task_struct *next)
{
	prev->dl_server = ശൂന്യ;
	next->dl_server = rq->dl_server;
	rq->dl_server = ശൂന്യ;
}

സ്ഥിതം വരിയിൽ ശൂന്യം put_prev_set_next_task(ഘടന rq *rq,
					  ഘടന task_struct *prev,
					  ഘടന task_struct *next)
{
	WARN_ON_ONCE(rq->donor != prev);

	__put_prev_set_next_dl_server(rq, prev, next);

	എങ്കിൽ (next == prev) {
		next->sched_class->set_next_task(rq, next, SNT_REPICK);
		തിരികെ;
	}

	prev->sched_class->put_prev_task(rq, prev, next);
	next->sched_class->set_next_task(rq, next, SNT_PICK);
}

/*
 * Helper to define a sched_class instance; each one is placed in a separate
 * section which is ordered by the linker script:
 *
 *   include/asm-generic/vmlinux.lds.h
 *
 * *CAREFUL* they are laid out in *REVERSE* order!!!
 *
 * Also enforce alignment on the instance, not the type, to guarantee layout.
 */
#define DEFINE_SCHED_CLASS(name) \
സ്ഥിരം ഘടന sched_class name##_sched_class \
	__aligned(__alignof__(ഘടന sched_class)) \
	__section("__" #name "_sched_class")

/* Defined in include/asm-generic/vmlinux.lds.h */
ബാഹ്യ ഘടന sched_class __sched_class_highest[];
ബാഹ്യ ഘടന sched_class __sched_class_lowest[];

ബാഹ്യ സ്ഥിരം ഘടന sched_class stop_sched_class;
ബാഹ്യ സ്ഥിരം ഘടന sched_class dl_sched_class;
ബാഹ്യ സ്ഥിരം ഘടന sched_class rt_sched_class;
ബാഹ്യ സ്ഥിരം ഘടന sched_class fair_sched_class;
ബാഹ്യ സ്ഥിരം ഘടന sched_class idle_sched_class;

/*
 * Iterate only active classes. SCX can take over all fair tasks or be
 * completely disabled. If the former, skip fair. If the latter, skip SCX.
 */
സ്ഥിതം വരിയിൽ സ്ഥിരം ഘടന sched_class *next_active_class(സ്ഥിരം ഘടന sched_class *class)
{
	class++;
#ifdef CONFIG_SCHED_CLASS_EXT
	എങ്കിൽ (scx_switched_all() && class == &fair_sched_class)
		class++;
	എങ്കിൽ (!scx_enabled() && class == &ext_sched_class)
		class++;
#endif
	തിരികെ class;
}

#define for_class_range(class, _from, _to) \
	വേണ്ടി (class = (_from); class < (_to); class++)

#define for_each_class(class) \
	for_class_range(class, __sched_class_highest, __sched_class_lowest)

#define for_active_class_range(class, _from, _to)				\
	വേണ്ടി (class = (_from); class != (_to); class = next_active_class(class))

#define for_each_active_class(class)						\
	for_active_class_range(class, __sched_class_highest, __sched_class_lowest)

#define sched_class_above(_a, _b)	((_a) < (_b))

സ്ഥിതം വരിയിൽ ശൂന്യം rq_modified_begin(ഘടന rq *rq, സ്ഥിരം ഘടന sched_class *class)
{
	എങ്കിൽ (sched_class_above(rq->next_class, class))
		rq->next_class = class;
}

സ്ഥിതം വരിയിൽ bool rq_modified_above(ഘടന rq *rq, സ്ഥിരം ഘടന sched_class *class)
{
	തിരികെ sched_class_above(rq->next_class, class);
}

സ്ഥിതം വരിയിൽ bool sched_stop_runnable(ഘടന rq *rq)
{
	തിരികെ rq->stop && task_on_rq_queued(rq->stop);
}

സ്ഥിതം വരിയിൽ bool sched_dl_runnable(ഘടന rq *rq)
{
	തിരികെ rq->dl.dl_nr_running > 0;
}

സ്ഥിതം വരിയിൽ bool sched_rt_runnable(ഘടന rq *rq)
{
	തിരികെ rq->rt.rt_queued > 0;
}

സ്ഥിതം വരിയിൽ bool sched_fair_runnable(ഘടന rq *rq)
{
	തിരികെ rq->cfs.nr_queued > 0;
}

ബാഹ്യ ഘടന task_struct *pick_task_fair(ഘടന rq *rq, ഘടന rq_flags *rf);
ബാഹ്യ ഘടന task_struct *pick_task_idle(ഘടന rq *rq, ഘടന rq_flags *rf);

#define SCA_CHECK		0x01
#define SCA_MIGRATE_DISABLE	0x02
#define SCA_MIGRATE_ENABLE	0x04
#define SCA_USER		0x08

ബാഹ്യ ശൂന്യം update_group_capacity(ഘടന sched_domain *sd, പൂർണ്ണസംഖ്യ cpu);

ബാഹ്യ ശൂന്യം sched_balance_trigger(ഘടന rq *rq);

ബാഹ്യ പൂർണ്ണസംഖ്യ __set_cpus_allowed_ptr(ഘടന task_struct *p, ഘടന affinity_context *ctx);
ബാഹ്യ ശൂന്യം set_cpus_allowed_common(ഘടന task_struct *p, ഘടന affinity_context *ctx);

സ്ഥിതം വരിയിൽ bool task_allowed_on_cpu(ഘടന task_struct *p, പൂർണ്ണസംഖ്യ cpu)
{
	/* When not in the task's cpumask, no point in looking further. */
	എങ്കിൽ (!cpumask_test_cpu(cpu, p->cpus_ptr))
		തിരികെ false;

	/* Can @cpu run a user thread? */
	എങ്കിൽ (!(p->flags & PF_KTHREAD) && !task_cpu_possible(cpu, p))
		തിരികെ false;

	തിരികെ true;
}

സ്ഥിതം വരിയിൽ cpumask_t *alloc_user_cpus_ptr(പൂർണ്ണസംഖ്യ കണ്ണി)
{
	/*
	 * See set_cpus_allowed_force() above for the rcu_head usage.
	 */
	പൂർണ്ണസംഖ്യ size = max_t(പൂർണ്ണസംഖ്യ, cpumask_size(), വലുപ്പം(ഘടന rcu_head));

	തിരികെ kmalloc_കണ്ണി(size, GFP_KERNEL, കണ്ണി);
}

സ്ഥിതം വരിയിൽ ഘടന task_struct *get_തള്ളുക_task(ഘടന rq *rq)
{
	ഘടന task_struct *p = rq->donor;

	lockdep_assert_rq_held(rq);

	എങ്കിൽ (rq->തള്ളുക_busy)
		തിരികെ ശൂന്യ;

	എങ്കിൽ (p->nr_cpus_allowed == 1)
		തിരികെ ശൂന്യ;

	എങ്കിൽ (p->migration_disabled)
		തിരികെ ശൂന്യ;

	rq->തള്ളുക_busy = true;
	തിരികെ get_task_struct(p);
}

ബാഹ്യ പൂർണ്ണസംഖ്യ തള്ളുക_cpu_stop(ശൂന്യം *arg);

#ifdef CONFIG_CPU_IDLE

സ്ഥിതം വരിയിൽ ശൂന്യം idle_set_state(ഘടന rq *rq,
				  ഘടന cpuidle_state *idle_state)
{
	rq->idle_state = idle_state;
}

സ്ഥിതം വരിയിൽ ഘടന cpuidle_state *idle_get_state(ഘടന rq *rq)
{
	lockdep_assert(rcu_read_lock_any_held());

	തിരികെ rq->idle_state;
}

#അല്ലെങ്കിൽ /* !CONFIG_CPU_IDLE: */

സ്ഥിതം വരിയിൽ ശൂന്യം idle_set_state(ഘടന rq *rq,
				  ഘടന cpuidle_state *idle_state)
{
}

സ്ഥിതം വരിയിൽ ഘടന cpuidle_state *idle_get_state(ഘടന rq *rq)
{
	തിരികെ ശൂന്യ;
}

#endif /* !CONFIG_CPU_IDLE */

ബാഹ്യ ശൂന്യം schedule_idle(ശൂന്യം);
asmlinkage ശൂന്യം schedule_user(ശൂന്യം);

ബാഹ്യ ശൂന്യം sysrq_sched_debug_show(ശൂന്യം);
ബാഹ്യ ശൂന്യം sched_init_granularity(ശൂന്യം);
ബാഹ്യ ശൂന്യം update_max_interval(ശൂന്യം);

ബാഹ്യ ശൂന്യം init_sched_dl_class(ശൂന്യം);
ബാഹ്യ ശൂന്യം init_sched_rt_class(ശൂന്യം);
ബാഹ്യ ശൂന്യം init_sched_fair_class(ശൂന്യം);

ബാഹ്യ ശൂന്യം resched_curr(ഘടന rq *rq);
ബാഹ്യ ശൂന്യം resched_curr_lazy(ഘടന rq *rq);
ബാഹ്യ ശൂന്യം resched_cpu(പൂർണ്ണസംഖ്യ cpu);

ബാഹ്യ ശൂന്യം init_rt_bandwidth(ഘടന rt_bandwidth *rt_b, u64 period, u64 runtime);
ബാഹ്യ bool sched_rt_bandwidth_account(ഘടന rt_rq *rt_rq);

ബാഹ്യ ശൂന്യം init_dl_entity(ഘടന sched_dl_entity *dl_se);

ബാഹ്യ ശൂന്യം init_cfs_throttle_work(ഘടന task_struct *p);

#define BW_SHIFT		20
#define BW_UNIT			(1 << BW_SHIFT)
#define RATIO_SHIFT		8
#define MAX_BW_BITS		(64 - BW_SHIFT)
#define MAX_BW			((1ULL << MAX_BW_BITS) - 1)

ബാഹ്യ u64 to_ratio(u64 period, u64 runtime);

ബാഹ്യ ശൂന്യം init_entity_runnable_average(ഘടന sched_entity *se);
ബാഹ്യ ശൂന്യം post_init_entity_util_avg(ഘടന task_struct *p);

#ifdef CONFIG_NO_HZ_FULL
ബാഹ്യ bool sched_can_stop_tick(ഘടന rq *rq);
ബാഹ്യ പൂർണ്ണസംഖ്യ __init sched_tick_offload_init(ശൂന്യം);

/*
 * Tick may be needed by tasks in the runqueue depending on their policy and
 * requirements. If tick is needed, lets send the target an IPI to kick it out of
 * nohz mode if necessary.
 */
സ്ഥിതം വരിയിൽ ശൂന്യം sched_update_tick_dependency(ഘടന rq *rq)
{
	പൂർണ്ണസംഖ്യ cpu = cpu_of(rq);

	എങ്കിൽ (!tick_nohz_full_cpu(cpu))
		തിരികെ;

	എങ്കിൽ (sched_can_stop_tick(rq))
		tick_nohz_dep_clear_cpu(cpu, TICK_DEP_BIT_SCHED);
	അല്ലെങ്കിൽ
		tick_nohz_dep_set_cpu(cpu, TICK_DEP_BIT_SCHED);
}
#അല്ലെങ്കിൽ /* !CONFIG_NO_HZ_FULL: */
സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ sched_tick_offload_init(ശൂന്യം) { തിരികെ 0; }
സ്ഥിതം വരിയിൽ ശൂന്യം sched_update_tick_dependency(ഘടന rq *rq) { }
#endif /* !CONFIG_NO_HZ_FULL */

സ്ഥിതം വരിയിൽ ശൂന്യം add_nr_running(ഘടന rq *rq, unsigned എണ്ണം)
{
	unsigned prev_nr = rq->nr_running;

	rq->nr_running = prev_nr + എണ്ണം;
	എങ്കിൽ (trace_sched_update_nr_running_tp_enabled()) {
		call_trace_sched_update_nr_running(rq, എണ്ണം);
	}

	എങ്കിൽ (prev_nr < 2 && rq->nr_running >= 2)
		set_rd_overloaded(rq->rd, 1);

	sched_update_tick_dependency(rq);
}

സ്ഥിതം വരിയിൽ ശൂന്യം sub_nr_running(ഘടന rq *rq, unsigned എണ്ണം)
{
	rq->nr_running -= എണ്ണം;
	എങ്കിൽ (trace_sched_update_nr_running_tp_enabled()) {
		call_trace_sched_update_nr_running(rq, -എണ്ണം);
	}

	/* Check if we still need preemption */
	sched_update_tick_dependency(rq);
}

സ്ഥിതം വരിയിൽ ശൂന്യം __block_task(ഘടന rq *rq, ഘടന task_struct *p)
{
	എങ്കിൽ (p->sched_contributes_to_load)
		rq->nr_uninterruptible++;

	എങ്കിൽ (p->in_iowait) {
		atomic_inc(&rq->nr_iowait);
		delayacct_blkio_start();
	}

	ASSERT_EXCLUSIVE_WRITER(p->on_rq);

	/*
	 * The moment this write goes through, ttwu() can swoop in and migrate
	 * this task, rendering our rq->__lock ineffective.
	 *
	 * __schedule()				try_to_wake_up()
	 *   LOCK rq->__lock			  LOCK p->pi_lock
	 *   pick_next_task()
	 *     pick_next_task_fair()
	 *       pick_next_entity()
	 *         dequeue_entities()
	 *           __block_task()
	 *             RELEASE p->on_rq = 0	  if (p->on_rq && ...)
	 *					    break;
	 *
	 *					  ACQUIRE (after ctrl-dep)
	 *
	 *					  cpu = select_task_rq();
	 *					  set_task_cpu(p, cpu);
	 *					  ttwu_queue()
	 *					    ttwu_do_activate()
	 *					      LOCK rq->__lock
	 *					      activate_task()
	 *					        STORE p->on_rq = 1
	 *   UNLOCK rq->__lock
	 *
	 * Callers must ensure to not reference @p after this -- we no longer
	 * own it.
	 */
	smp_store_release(&p->on_rq, 0);
}

ബാഹ്യ ശൂന്യം activate_task(ഘടന rq *rq, ഘടന task_struct *p, പൂർണ്ണസംഖ്യ flags);
ബാഹ്യ ശൂന്യം deactivate_task(ഘടന rq *rq, ഘടന task_struct *p, പൂർണ്ണസംഖ്യ flags);

ബാഹ്യ ശൂന്യം wakeup_preempt(ഘടന rq *rq, ഘടന task_struct *p, പൂർണ്ണസംഖ്യ flags);

/*
 * attach_task() -- attach the task detached by detach_task() to its new rq.
 */
സ്ഥിതം വരിയിൽ ശൂന്യം attach_task(ഘടന rq *rq, ഘടന task_struct *p)
{
	lockdep_assert_rq_held(rq);

	WARN_ON_ONCE(task_rq(p) != rq);
	activate_task(rq, p, ENQUEUE_NOCLOCK);
	wakeup_preempt(rq, p, 0);
}

/*
 * attach_one_task() -- attaches the task returned from detach_one_task() to
 * its new rq.
 */
സ്ഥിതം വരിയിൽ ശൂന്യം attach_one_task(ഘടന rq *rq, ഘടന task_struct *p)
{
	guard(rq_lock)(rq);
	update_rq_clock(rq);
	attach_task(rq, p);
}

/*
 * __attach_tasks() - attaches a list of tasks (using se.group_node) to
 * the new rq
 */
സ്ഥിതം വരിയിൽ ശൂന്യം __attach_tasks(ഘടന rq *rq, ഘടന list_head *tasks)
{
	guard(rq_lock)(rq);
	update_rq_clock(rq);

	ആകുമ്പോൾ (!list_empty(tasks)) {
		ഘടന task_struct *p;

		p = list_first_entry(tasks, ഘടന task_struct, se.group_കണ്ണി);
		list_del_init(&p->se.group_കണ്ണി);

		attach_task(rq, p);
	}
}

#ifdef CONFIG_PREEMPT_RT
# define SCHED_NR_MIGRATE_BREAK 8
#അല്ലെങ്കിൽ
# define SCHED_NR_MIGRATE_BREAK 32
#endif

ബാഹ്യ __read_mostly unsigned പൂർണ്ണസംഖ്യ sysctl_sched_nr_migrate;
ബാഹ്യ __read_mostly unsigned പൂർണ്ണസംഖ്യ sysctl_sched_migration_cost;

ബാഹ്യ unsigned പൂർണ്ണസംഖ്യ sysctl_sched_base_slice;

ബാഹ്യ പൂർണ്ണസംഖ്യ sysctl_resched_latency_warn_ms;
ബാഹ്യ പൂർണ്ണസംഖ്യ sysctl_resched_latency_warn_once;

ബാഹ്യ unsigned പൂർണ്ണസംഖ്യ sysctl_sched_tunable_scaling;

ബാഹ്യ unsigned പൂർണ്ണസംഖ്യ sysctl_numa_balancing_scan_delay;
ബാഹ്യ unsigned പൂർണ്ണസംഖ്യ sysctl_numa_balancing_scan_period_min;
ബാഹ്യ unsigned പൂർണ്ണസംഖ്യ sysctl_numa_balancing_scan_period_max;
ബാഹ്യ unsigned പൂർണ്ണസംഖ്യ sysctl_numa_balancing_scan_size;
ബാഹ്യ unsigned പൂർണ്ണസംഖ്യ sysctl_numa_balancing_hot_threshold;

#ifdef CONFIG_SCHED_HRTICK

/*
 * Use hrtick when:
 *  - enabled by features
 *  - hrtimer is actually high res
 */
സ്ഥിതം വരിയിൽ bool hrtick_enabled(ഘടന rq *rq)
{
	തിരികെ cpu_active(cpu_of(rq)) && hrtimer_highres_enabled();
}

സ്ഥിതം വരിയിൽ bool hrtick_enabled_fair(ഘടന rq *rq)
{
	തിരികെ sched_feat(HRTICK) && hrtick_enabled(rq);
}

സ്ഥിതം വരിയിൽ bool hrtick_enabled_dl(ഘടന rq *rq)
{
	തിരികെ sched_feat(HRTICK_DL) && hrtick_enabled(rq);
}

ബാഹ്യ ശൂന്യം hrtick_start(ഘടന rq *rq, u64 delay);
സ്ഥിതം വരിയിൽ bool hrtick_active(ഘടന rq *rq)
{
	തിരികെ hrtimer_active(&rq->hrtick_timer);
}

#അല്ലെങ്കിൽ /* !CONFIG_SCHED_HRTICK: */
സ്ഥിതം വരിയിൽ bool hrtick_enabled_fair(ഘടന rq *rq) { തിരികെ false; }
സ്ഥിതം വരിയിൽ bool hrtick_enabled_dl(ഘടന rq *rq) { തിരികെ false; }
സ്ഥിതം വരിയിൽ bool hrtick_enabled(ഘടന rq *rq) { തിരികെ false; }
#endif /* !CONFIG_SCHED_HRTICK */

#ifndef arch_scale_freq_tick
സ്ഥിതം __always_inline ശൂന്യം arch_scale_freq_tick(ശൂന്യം) { }
#endif

#ifndef arch_scale_freq_capacity
/**
 * arch_scale_freq_capacity - get the frequency scale factor of a given CPU.
 * @cpu: the CPU in question.
 *
 * Return: the frequency scale factor normalized against SCHED_CAPACITY_SCALE, i.e.
 *
 *     f_curr
 *     ------ * SCHED_CAPACITY_SCALE
 *     f_max
 */
സ്ഥിതം __always_inline
unsigned long arch_scale_freq_capacity(പൂർണ്ണസംഖ്യ cpu)
{
	തിരികെ SCHED_CAPACITY_SCALE;
}
#endif

/*
 * In double_lock_balance()/double_rq_lock(), we use raw_spin_rq_lock() to
 * acquire rq lock instead of rq_lock(). So at the end of these two functions
 * we need to call double_rq_clock_clear_update() to clear RQCF_UPDATED of
 * rq->clock_update_flags to avoid the WARN_DOUBLE_CLOCK warning.
 */
സ്ഥിതം വരിയിൽ ശൂന്യം double_rq_clock_clear_update(ഘടന rq *rq1, ഘടന rq *rq2)
{
	rq1->clock_update_flags &= (RQCF_REQ_SKIP|RQCF_ACT_SKIP);
	rq2->clock_update_flags &= (RQCF_REQ_SKIP|RQCF_ACT_SKIP);
}

#define DEFINE_LOCK_GUARD_2(name, type, _lock, _unlock, ...)				\
__DEFINE_UNLOCK_GUARD(name, type, _unlock, type *lock2; __VA_ARGS__)			\
സ്ഥിതം വരിയിൽ class_##name##_t class_##name##_constructor(type *lock, type *lock2)	\
	__no_context_analysis								\
{ class_##name##_t _t = { .lock = lock, .lock2 = lock2 }, *_T = &_t;			\
  _lock; തിരികെ _t; }
#define DECLARE_LOCK_GUARD_2_ATTRS(_name, _lock, _unlock1, _unlock2)			\
സ്ഥിതം വരിയിൽ class_##_name##_t class_##_name##_constructor(lock_##_name##_t *_T1,	\
							    lock_##_name##_t *_T2) _lock; \
സ്ഥിതം __always_inline ശൂന്യം __class_##_name##_cleanup_ctx1(class_##_name##_t **_T1)	\
	__no_context_analysis _unlock1 { }						\
സ്ഥിതം __always_inline ശൂന്യം __class_##_name##_cleanup_ctx2(class_##_name##_t **_T2)	\
	__no_context_analysis _unlock2 { }
#define WITH_LOCK_GUARD_2_ATTRS(_name, _T1, _T2)					\
	class_##_name##_constructor(_T1, _T2),						\
	*__UNIQUE_ID(unlock1) __cleanup(__class_##_name##_cleanup_ctx1) = (ശൂന്യം *)(_T1),\
	*__UNIQUE_ID(unlock2) __cleanup(__class_##_name##_cleanup_ctx2) = (ശൂന്യം *)(_T2)

സ്ഥിതം വരിയിൽ bool rq_order_less(ഘടന rq *rq1, ഘടന rq *rq2)
{
#ifdef CONFIG_SCHED_CORE
	/*
	 * In order to not have {0,2},{1,3} turn into into an AB-BA,
	 * order by core-id first and cpu-id second.
	 *
	 * Notably:
	 *
	 *	double_rq_lock(0,3); will take core-0, core-1 lock
	 *	double_rq_lock(1,2); will take core-1, core-0 lock
	 *
	 * when only cpu-id is considered.
	 */
	എങ്കിൽ (rq1->core->cpu < rq2->core->cpu)
		തിരികെ true;
	എങ്കിൽ (rq1->core->cpu > rq2->core->cpu)
		തിരികെ false;

	/*
	 * __sched_core_flip() relies on SMT having cpu-id lock order.
	 */
#endif /* CONFIG_SCHED_CORE */
	തിരികെ rq1->cpu < rq2->cpu;
}

ബാഹ്യ ശൂന്യം double_rq_lock(ഘടന rq *rq1, ഘടന rq *rq2)
	__acquires(__rq_lockp(rq1), __rq_lockp(rq2));

#ifdef CONFIG_PREEMPTION

/*
 * fair double_lock_balance: Safely acquires both rq->locks in a fair
 * way at the expense of forcing extra atomic operations in all
 * invocations.  This assures that the double_lock is acquired using the
 * same underlying policy as the spinlock_t on this architecture, which
 * reduces latency compared to the unfair variant below.  However, it
 * also adds more overhead and therefore may reduce throughput.
 */
സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ _double_lock_balance(ഘടന rq *this_rq, ഘടന rq *busiest)
	__must_hold(__rq_lockp(this_rq))
	__acquires(__rq_lockp(busiest))
{
	raw_spin_rq_unlock(this_rq);
	double_rq_lock(this_rq, busiest);

	തിരികെ 1;
}

#അല്ലെങ്കിൽ /* !CONFIG_PREEMPTION: */
/*
 * Unfair double_lock_balance: Optimizes throughput at the expense of
 * latency by eliminating extra atomic operations when the locks are
 * already in proper order on entry.  This favors lower CPU-ids and will
 * grant the double lock to lower CPUs over higher ids under contention,
 * regardless of entry order into the function.
 */
സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ _double_lock_balance(ഘടന rq *this_rq, ഘടന rq *busiest)
	__must_hold(__rq_lockp(this_rq))
	__acquires(__rq_lockp(busiest))
{
	എങ്കിൽ (__rq_lockp(this_rq) == __rq_lockp(busiest)) {
		__acquire(__rq_lockp(busiest)); /* already held */
		double_rq_clock_clear_update(this_rq, busiest);
		തിരികെ 0;
	}

	എങ്കിൽ (likely(raw_spin_rq_trylock(busiest))) {
		double_rq_clock_clear_update(this_rq, busiest);
		തിരികെ 0;
	}

	എങ്കിൽ (rq_order_less(this_rq, busiest)) {
		raw_spin_rq_lock_nested(busiest, SINGLE_DEPTH_NESTING);
		double_rq_clock_clear_update(this_rq, busiest);
		തിരികെ 0;
	}

	raw_spin_rq_unlock(this_rq);
	double_rq_lock(this_rq, busiest);

	തിരികെ 1;
}

#endif /* !CONFIG_PREEMPTION */

/*
 * double_lock_balance - lock the busiest runqueue, this_rq is locked already.
 */
സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ double_lock_balance(ഘടന rq *this_rq, ഘടന rq *busiest)
	__must_hold(__rq_lockp(this_rq))
	__acquires(__rq_lockp(busiest))
{
	lockdep_assert_irqs_disabled();

	തിരികെ _double_lock_balance(this_rq, busiest);
}

സ്ഥിതം വരിയിൽ ശൂന്യം double_unlock_balance(ഘടന rq *this_rq, ഘടന rq *busiest)
	__releases(__rq_lockp(busiest))
{
	എങ്കിൽ (__rq_lockp(this_rq) != __rq_lockp(busiest))
		raw_spin_rq_unlock(busiest);
	അല്ലെങ്കിൽ
		__release(__rq_lockp(busiest)); /* fake release */
	lock_set_subclass(&__rq_lockp(this_rq)->dep_map, 0, _RET_IP_);
}

സ്ഥിതം വരിയിൽ ശൂന്യം double_lock(spinlock_t *l1, spinlock_t *l2)
	__acquires(l1, l2)
{
	എങ്കിൽ (l1 > l2)
		swap(l1, l2);

	spin_lock(l1);
	spin_lock_nested(l2, SINGLE_DEPTH_NESTING);
}

സ്ഥിതം വരിയിൽ ശൂന്യം double_lock_irq(spinlock_t *l1, spinlock_t *l2)
	__acquires(l1, l2)
{
	എങ്കിൽ (l1 > l2)
		swap(l1, l2);

	spin_lock_irq(l1);
	spin_lock_nested(l2, SINGLE_DEPTH_NESTING);
}

സ്ഥിതം വരിയിൽ ശൂന്യം double_raw_lock(raw_spinlock_t *l1, raw_spinlock_t *l2)
	__acquires(l1, l2)
{
	എങ്കിൽ (l1 > l2)
		swap(l1, l2);

	raw_spin_lock(l1);
	raw_spin_lock_nested(l2, SINGLE_DEPTH_NESTING);
}

സ്ഥിതം വരിയിൽ ശൂന്യം double_raw_unlock(raw_spinlock_t *l1, raw_spinlock_t *l2)
	__releases(l1, l2)
{
	raw_spin_unlock(l1);
	raw_spin_unlock(l2);
}

DEFINE_LOCK_GUARD_2(double_raw_spinlock, raw_spinlock_t,
		    double_raw_lock(_T->lock, _T->lock2),
		    double_raw_unlock(_T->lock, _T->lock2))

DECLARE_LOCK_GUARD_2_ATTRS(double_raw_spinlock,
			   __acquires(_T1, _T2),
			   __releases(*(raw_spinlock_t **)_T1),
			   __releases(*(raw_spinlock_t **)_T2));
#define class_double_raw_spinlock_constructor(_T1, _T2) \
	WITH_LOCK_GUARD_2_ATTRS(double_raw_spinlock, _T1, _T2)

/*
 * double_rq_unlock - safely unlock two runqueues
 *
 * Note this does not restore interrupts like task_rq_unlock,
 * you need to do so manually after calling.
 */
സ്ഥിതം വരിയിൽ ശൂന്യം double_rq_unlock(ഘടന rq *rq1, ഘടന rq *rq2)
	__releases(__rq_lockp(rq1), __rq_lockp(rq2))
{
	എങ്കിൽ (__rq_lockp(rq1) != __rq_lockp(rq2))
		raw_spin_rq_unlock(rq2);
	അല്ലെങ്കിൽ
		__release(__rq_lockp(rq2)); /* fake release */
	raw_spin_rq_unlock(rq1);
}

ബാഹ്യ ശൂന്യം set_rq_online (ഘടന rq *rq);
ബാഹ്യ ശൂന്യം set_rq_offline(ഘടന rq *rq);

ബാഹ്യ bool sched_smp_initialized;

DEFINE_LOCK_GUARD_2(double_rq_lock, ഘടന rq,
		    double_rq_lock(_T->lock, _T->lock2),
		    double_rq_unlock(_T->lock, _T->lock2))

ബാഹ്യ ഘടന sched_entity *__pick_root_entity(ഘടന cfs_rq *cfs_rq);
ബാഹ്യ ഘടന sched_entity *__pick_first_entity(ഘടന cfs_rq *cfs_rq);
ബാഹ്യ ഘടന sched_entity *__pick_last_entity(ഘടന cfs_rq *cfs_rq);

ബാഹ്യ bool sched_debug_verbose;

ബാഹ്യ ശൂന്യം print_cfs_stats(ഘടന seq_file *m, പൂർണ്ണസംഖ്യ cpu);
ബാഹ്യ ശൂന്യം print_rt_stats(ഘടന seq_file *m, പൂർണ്ണസംഖ്യ cpu);
ബാഹ്യ ശൂന്യം print_dl_stats(ഘടന seq_file *m, പൂർണ്ണസംഖ്യ cpu);
ബാഹ്യ ശൂന്യം print_cfs_rq(ഘടന seq_file *m, പൂർണ്ണസംഖ്യ cpu, ഘടന cfs_rq *cfs_rq);
ബാഹ്യ ശൂന്യം print_rt_rq(ഘടന seq_file *m, പൂർണ്ണസംഖ്യ cpu, ഘടന rt_rq *rt_rq);
ബാഹ്യ ശൂന്യം print_dl_rq(ഘടന seq_file *m, പൂർണ്ണസംഖ്യ cpu, ഘടന dl_rq *dl_rq);

ബാഹ്യ ശൂന്യം resched_latency_warn(പൂർണ്ണസംഖ്യ cpu, u64 latency);

#ifdef CONFIG_NUMA_BALANCING
ബാഹ്യ ശൂന്യം show_numa_stats(ഘടന task_struct *p, ഘടന seq_file *m);
ബാഹ്യ ശൂന്യം
print_numa_stats(ഘടന seq_file *m, പൂർണ്ണസംഖ്യ കണ്ണി, unsigned long tsf,
		 unsigned long tpf, unsigned long gsf, unsigned long gpf);
#endif /* CONFIG_NUMA_BALANCING */

ബാഹ്യ ശൂന്യം init_cfs_rq(ഘടന cfs_rq *cfs_rq);
ബാഹ്യ ശൂന്യം init_rt_rq(ഘടന rt_rq *rt_rq);
ബാഹ്യ ശൂന്യം init_dl_rq(ഘടന dl_rq *dl_rq);

ബാഹ്യ ശൂന്യം cfs_bandwidth_usage_inc(ശൂന്യം);
ബാഹ്യ ശൂന്യം cfs_bandwidth_usage_dec(ശൂന്യം);

#ifdef CONFIG_NO_HZ_COMMON

#define NOHZ_BALANCE_KICK_BIT	0
#define NOHZ_STATS_KICK_BIT	1
#define NOHZ_NEWILB_KICK_BIT	2
#define NOHZ_NEXT_KICK_BIT	3

/* Run sched_balance_domains() */
#define NOHZ_BALANCE_KICK	BIT(NOHZ_BALANCE_KICK_BIT)
/* Update blocked load */
#define NOHZ_STATS_KICK		BIT(NOHZ_STATS_KICK_BIT)
/* Update blocked load when entering idle */
#define NOHZ_NEWILB_KICK	BIT(NOHZ_NEWILB_KICK_BIT)
/* Update nohz.next_balance */
#define NOHZ_NEXT_KICK		BIT(NOHZ_NEXT_KICK_BIT)

#define NOHZ_KICK_MASK		(NOHZ_BALANCE_KICK | NOHZ_STATS_KICK | NOHZ_NEXT_KICK)

#define nohz_flags(cpu)		(&cpu_rq(cpu)->nohz_flags)

ബാഹ്യ ശൂന്യം nohz_balance_exit_idle(ഘടന rq *rq);
#അല്ലെങ്കിൽ /* !CONFIG_NO_HZ_COMMON: */
സ്ഥിതം വരിയിൽ ശൂന്യം nohz_balance_exit_idle(ഘടന rq *rq) { }
#endif /* !CONFIG_NO_HZ_COMMON */

#ifdef CONFIG_NO_HZ_COMMON
ബാഹ്യ ശൂന്യം nohz_run_idle_balance(പൂർണ്ണസംഖ്യ cpu);
#അല്ലെങ്കിൽ
സ്ഥിതം വരിയിൽ ശൂന്യം nohz_run_idle_balance(പൂർണ്ണസംഖ്യ cpu) { }
#endif

#അന്തര്ഭാവ "stats.h"

#എങ്കിൽ defined(CONFIG_SCHED_CORE) && defined(CONFIG_SCHEDSTATS)

ബാഹ്യ ശൂന്യം __sched_core_account_forceidle(ഘടന rq *rq);

സ്ഥിതം വരിയിൽ ശൂന്യം sched_core_account_forceidle(ഘടന rq *rq)
{
	എങ്കിൽ (schedstat_enabled())
		__sched_core_account_forceidle(rq);
}

ബാഹ്യ ശൂന്യം __sched_core_tick(ഘടന rq *rq);

സ്ഥിതം വരിയിൽ ശൂന്യം sched_core_tick(ഘടന rq *rq)
{
	എങ്കിൽ (sched_core_enabled(rq) && schedstat_enabled())
		__sched_core_tick(rq);
}

#അല്ലെങ്കിൽ /* !(CONFIG_SCHED_CORE && CONFIG_SCHEDSTATS): */

സ്ഥിതം വരിയിൽ ശൂന്യം sched_core_account_forceidle(ഘടന rq *rq) { }

സ്ഥിതം വരിയിൽ ശൂന്യം sched_core_tick(ഘടന rq *rq) { }

#endif /* !(CONFIG_SCHED_CORE && CONFIG_SCHEDSTATS) */

#ifdef CONFIG_IRQ_TIME_ACCOUNTING

ഘടന irqtime {
	u64			total;
	u64			tick_delta;
	u64			irq_start_time;
	ഘടന u64_stats_sync	sync;
};

DECLARE_PER_CPU(ഘടന irqtime, cpu_irqtime);
DECLARE_STATIC_താക്കോൽ_FALSE(sched_clock_irqtime);

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ irqtime_enabled(ശൂന്യം)
{
	തിരികെ static_branch_likely(&sched_clock_irqtime);
}

/*
 * Returns the irqtime minus the softirq time computed by ksoftirqd.
 * Otherwise ksoftirqd's sum_exec_runtime is subtracted its own runtime
 * and never move forward.
 */
സ്ഥിതം വരിയിൽ u64 irq_time_read(പൂർണ്ണസംഖ്യ cpu)
{
	ഘടന irqtime *irqtime = &per_cpu(cpu_irqtime, cpu);
	unsigned പൂർണ്ണസംഖ്യ seq;
	u64 total;

	ചെയ്യുക {
		seq = __u64_stats_fetch_begin(&irqtime->sync);
		total = irqtime->total;
	} ആകുമ്പോൾ (__u64_stats_fetch_retry(&irqtime->sync, seq));

	തിരികെ total;
}

#അല്ലെങ്കിൽ /* !CONFIG_IRQ_TIME_ACCOUNTING: */

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ irqtime_enabled(ശൂന്യം)
{
	തിരികെ 0;
}

#endif /* !CONFIG_IRQ_TIME_ACCOUNTING */

#ifdef CONFIG_CPU_FREQ

DECLARE_PER_CPU(ഘടന update_util_data __rcu *, cpufreq_update_util_data);

/**
 * cpufreq_update_util - Take a note about CPU utilization changes.
 * @rq: Runqueue to carry out the update for.
 * @flags: Update reason flags.
 *
 * This function is called by the scheduler on the CPU whose utilization is
 * being updated.
 *
 * It can only be called from RCU-sched read-side critical sections.
 *
 * The way cpufreq is currently arranged requires it to evaluate the CPU
 * performance state (frequency/voltage) on a regular basis to prevent it from
 * being stuck in a completely inadequate performance level for too long.
 * That is not guaranteed to happen if the updates are only triggered from CFS
 * and DL, though, because they may not be coming in if only RT tasks are
 * active all the time (or there are RT tasks only).
 *
 * As a workaround for that issue, this function is called periodically by the
 * RT sched class to trigger extra cpufreq updates to prevent it from stalling,
 * but that really is a band-aid.  Going forward it should be replaced with
 * solutions targeted more specifically at RT tasks.
 */
സ്ഥിതം വരിയിൽ ശൂന്യം cpufreq_update_util(ഘടന rq *rq, unsigned പൂർണ്ണസംഖ്യ flags)
{
	ഘടന update_util_data *data;

	data = rcu_dereference_sched(*per_cpu_ptr(&cpufreq_update_util_data,
						  cpu_of(rq)));
	എങ്കിൽ (data)
		data->func(data, rq_clock(rq), flags);
}
#അല്ലെങ്കിൽ /* !CONFIG_CPU_FREQ: */
സ്ഥിതം വരിയിൽ ശൂന്യം cpufreq_update_util(ഘടന rq *rq, unsigned പൂർണ്ണസംഖ്യ flags) { }
#endif /* !CONFIG_CPU_FREQ */

#ifdef arch_scale_freq_capacity
# ifndef arch_scale_freq_invariant
#  define arch_scale_freq_invariant()	true
# endif
#അല്ലെങ്കിൽ
# define arch_scale_freq_invariant()	false
#endif

unsigned long effective_cpu_util(പൂർണ്ണസംഖ്യ cpu, unsigned long util_cfs,
				 unsigned long *min,
				 unsigned long *max);

unsigned long sugov_effective_cpu_perf(പൂർണ്ണസംഖ്യ cpu, unsigned long actual,
				 unsigned long min,
				 unsigned long max);


/*
 * Verify the fitness of task @p to run on @cpu taking into account the
 * CPU original capacity and the runtime/deadline ratio of the task.
 *
 * The function will return true if the original capacity of @cpu is
 * greater than or equal to task's deadline density right shifted by
 * (BW_SHIFT - SCHED_CAPACITY_SHIFT) and false otherwise.
 */
സ്ഥിതം വരിയിൽ bool dl_task_fits_capacity(ഘടന task_struct *p, പൂർണ്ണസംഖ്യ cpu)
{
	unsigned long cap = arch_scale_cpu_capacity(cpu);

	തിരികെ cap >= p->dl.dl_density >> (BW_SHIFT - SCHED_CAPACITY_SHIFT);
}

സ്ഥിതം വരിയിൽ unsigned long cpu_bw_dl(ഘടന rq *rq)
{
	തിരികെ (rq->dl.running_bw * SCHED_CAPACITY_SCALE) >> BW_SHIFT;
}

സ്ഥിതം വരിയിൽ unsigned long cpu_util_dl(ഘടന rq *rq)
{
	തിരികെ READ_ONCE(rq->avg_dl.util_avg);
}


ബാഹ്യ unsigned long cpu_util_cfs(പൂർണ്ണസംഖ്യ cpu);
ബാഹ്യ unsigned long cpu_util_cfs_boost(പൂർണ്ണസംഖ്യ cpu);

സ്ഥിതം വരിയിൽ unsigned long cpu_util_rt(ഘടന rq *rq)
{
	തിരികെ READ_ONCE(rq->avg_rt.util_avg);
}

#ifdef CONFIG_UCLAMP_TASK

unsigned long uclamp_eff_മൂല്യം(ഘടന task_struct *p, എണ്ണൽ uclamp_id clamp_id);

/*
 * When uclamp is compiled in, the aggregation at rq level is 'turned off'
 * by default in the fast path and only gets turned on once userspace performs
 * an operation that requires it.
 *
 * Returns true if userspace opted-in to use uclamp and aggregation at rq level
 * hence is active.
 */
സ്ഥിതം വരിയിൽ bool uclamp_is_used(ശൂന്യം)
{
	തിരികെ static_branch_likely(&sched_uclamp_used);
}

/*
 * Enabling static branches would get the cpus_read_lock(),
 * check whether uclamp_is_used before enable it to avoid always
 * calling cpus_read_lock(). Because we never disable this
 * static key once enable it.
 */
സ്ഥിതം വരിയിൽ ശൂന്യം sched_uclamp_enable(ശൂന്യം)
{
	എങ്കിൽ (!uclamp_is_used())
		static_branch_enable(&sched_uclamp_used);
}

സ്ഥിതം വരിയിൽ unsigned long uclamp_rq_get(ഘടന rq *rq,
					  എണ്ണൽ uclamp_id clamp_id)
{
	തിരികെ READ_ONCE(rq->uclamp[clamp_id].മൂല്യം);
}

സ്ഥിതം വരിയിൽ ശൂന്യം uclamp_rq_set(ഘടന rq *rq, എണ്ണൽ uclamp_id clamp_id,
				 unsigned പൂർണ്ണസംഖ്യ മൂല്യം)
{
	WRITE_ONCE(rq->uclamp[clamp_id].മൂല്യം, മൂല്യം);
}

സ്ഥിതം വരിയിൽ bool uclamp_rq_is_idle(ഘടന rq *rq)
{
	തിരികെ rq->uclamp_flags & UCLAMP_FLAG_IDLE;
}

/* Is the rq being capped/throttled by uclamp_max? */
സ്ഥിതം വരിയിൽ bool uclamp_rq_is_capped(ഘടന rq *rq)
{
	unsigned long rq_util;
	unsigned long max_util;

	എങ്കിൽ (!uclamp_is_used())
		തിരികെ false;

	rq_util = cpu_util_cfs(cpu_of(rq)) + cpu_util_rt(rq);
	max_util = READ_ONCE(rq->uclamp[UCLAMP_MAX].മൂല്യം);

	തിരികെ max_util != SCHED_CAPACITY_SCALE && rq_util >= max_util;
}

#define for_each_clamp_id(clamp_id) \
	വേണ്ടി ((clamp_id) = 0; (clamp_id) < UCLAMP_CNT; (clamp_id)++)

ബാഹ്യ unsigned പൂർണ്ണസംഖ്യ sysctl_sched_uclamp_util_min_rt_default;


സ്ഥിതം വരിയിൽ unsigned പൂർണ്ണസംഖ്യ uclamp_none(എണ്ണൽ uclamp_id clamp_id)
{
	എങ്കിൽ (clamp_id == UCLAMP_MIN)
		തിരികെ 0;
	തിരികെ SCHED_CAPACITY_SCALE;
}

/* Integer rounded range for each bucket */
#define UCLAMP_BUCKET_DELTA DIV_ROUND_CLOSEST(SCHED_CAPACITY_SCALE, UCLAMP_BUCKETS)

സ്ഥിതം വരിയിൽ unsigned പൂർണ്ണസംഖ്യ uclamp_bucket_id(unsigned പൂർണ്ണസംഖ്യ clamp_മൂല്യം)
{
	തിരികെ min_t(unsigned പൂർണ്ണസംഖ്യ, clamp_മൂല്യം / UCLAMP_BUCKET_DELTA, UCLAMP_BUCKETS - 1);
}

സ്ഥിതം വരിയിൽ ശൂന്യം
uclamp_se_set(ഘടന uclamp_se *uc_se, unsigned പൂർണ്ണസംഖ്യ മൂല്യം, bool user_defined)
{
	uc_se->മൂല്യം = മൂല്യം;
	uc_se->bucket_id = uclamp_bucket_id(മൂല്യം);
	uc_se->user_defined = user_defined;
}

#അല്ലെങ്കിൽ /* !CONFIG_UCLAMP_TASK: */

സ്ഥിതം വരിയിൽ unsigned long
uclamp_eff_മൂല്യം(ഘടന task_struct *p, എണ്ണൽ uclamp_id clamp_id)
{
	എങ്കിൽ (clamp_id == UCLAMP_MIN)
		തിരികെ 0;

	തിരികെ SCHED_CAPACITY_SCALE;
}

സ്ഥിതം വരിയിൽ bool uclamp_rq_is_capped(ഘടന rq *rq) { തിരികെ false; }

സ്ഥിതം വരിയിൽ bool uclamp_is_used(ശൂന്യം)
{
	തിരികെ false;
}

സ്ഥിതം വരിയിൽ ശൂന്യം sched_uclamp_enable(ശൂന്യം) {}

സ്ഥിതം വരിയിൽ unsigned long
uclamp_rq_get(ഘടന rq *rq, എണ്ണൽ uclamp_id clamp_id)
{
	എങ്കിൽ (clamp_id == UCLAMP_MIN)
		തിരികെ 0;

	തിരികെ SCHED_CAPACITY_SCALE;
}

സ്ഥിതം വരിയിൽ ശൂന്യം
uclamp_rq_set(ഘടന rq *rq, എണ്ണൽ uclamp_id clamp_id, unsigned പൂർണ്ണസംഖ്യ മൂല്യം)
{
}

സ്ഥിതം വരിയിൽ bool uclamp_rq_is_idle(ഘടന rq *rq)
{
	തിരികെ false;
}

#endif /* !CONFIG_UCLAMP_TASK */

#ifdef CONFIG_HAVE_SCHED_AVG_IRQ

സ്ഥിതം വരിയിൽ unsigned long cpu_util_irq(ഘടന rq *rq)
{
	തിരികെ READ_ONCE(rq->avg_irq.util_avg);
}

സ്ഥിതം വരിയിൽ
unsigned long scale_irq_capacity(unsigned long util, unsigned long irq, unsigned long max)
{
	util *= (max - irq);
	util /= max;

	തിരികെ util;

}

#അല്ലെങ്കിൽ /* !CONFIG_HAVE_SCHED_AVG_IRQ: */

സ്ഥിതം വരിയിൽ unsigned long cpu_util_irq(ഘടന rq *rq)
{
	തിരികെ 0;
}

സ്ഥിതം വരിയിൽ
unsigned long scale_irq_capacity(unsigned long util, unsigned long irq, unsigned long max)
{
	തിരികെ util;
}

#endif /* !CONFIG_HAVE_SCHED_AVG_IRQ */

ബാഹ്യ ശൂന്യം __setparam_fair(ഘടന task_struct *p, സ്ഥിരം ഘടന sched_attr *attr);

#എങ്കിൽ defined(CONFIG_ENERGY_MODEL) && defined(CONFIG_CPU_FREQ_GOV_SCHEDUTIL)

#define perf_domain_span(pd) (to_cpumask(((pd)->em_pd->cpus)))

DECLARE_STATIC_താക്കോൽ_FALSE(sched_energy_present);

സ്ഥിതം വരിയിൽ bool sched_energy_enabled(ശൂന്യം)
{
	തിരികെ static_branch_unlikely(&sched_energy_present);
}

#അല്ലെങ്കിൽ /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL): */

#define perf_domain_span(pd) ശൂന്യ

സ്ഥിതം വരിയിൽ bool sched_energy_enabled(ശൂന്യം) { തിരികെ false; }

#endif /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL) */

#ifdef CONFIG_MEMBARRIER

/*
 * The scheduler provides memory barriers required by membarrier between:
 * - prior user-space memory accesses and store to rq->membarrier_state,
 * - store to rq->membarrier_state and following user-space memory accesses.
 * In the same way it provides those guarantees around store to rq->curr.
 */
സ്ഥിതം വരിയിൽ ശൂന്യം membarrier_switch_mm(ഘടന rq *rq,
					ഘടന mm_struct *prev_mm,
					ഘടന mm_struct *next_mm)
{
	പൂർണ്ണസംഖ്യ membarrier_state;

	എങ്കിൽ (prev_mm == next_mm)
		തിരികെ;

	membarrier_state = atomic_read(&next_mm->membarrier_state);
	എങ്കിൽ (READ_ONCE(rq->membarrier_state) == membarrier_state)
		തിരികെ;

	WRITE_ONCE(rq->membarrier_state, membarrier_state);
}

#അല്ലെങ്കിൽ /* !CONFIG_MEMBARRIER: */

സ്ഥിതം വരിയിൽ ശൂന്യം membarrier_switch_mm(ഘടന rq *rq,
					ഘടന mm_struct *prev_mm,
					ഘടന mm_struct *next_mm)
{
}

#endif /* !CONFIG_MEMBARRIER */

സ്ഥിതം വരിയിൽ bool is_per_cpu_kthread(ഘടന task_struct *p)
{
	എങ്കിൽ (!(p->flags & PF_KTHREAD))
		തിരികെ false;

	എങ്കിൽ (p->nr_cpus_allowed != 1)
		തിരികെ false;

	തിരികെ true;
}

ബാഹ്യ ശൂന്യം swake_up_all_locked(ഘടന swait_queue_head *q);
ബാഹ്യ ശൂന്യം __prepare_to_swait(ഘടന swait_queue_head *q, ഘടന swait_queue *wait);

ബാഹ്യ പൂർണ്ണസംഖ്യ try_to_wake_up(ഘടന task_struct *tsk, unsigned പൂർണ്ണസംഖ്യ state, പൂർണ്ണസംഖ്യ wake_flags);

#ifdef CONFIG_PREEMPT_DYNAMIC
ബാഹ്യ പൂർണ്ണസംഖ്യ preempt_dynamic_mode;
ബാഹ്യ പൂർണ്ണസംഖ്യ sched_dynamic_mode(സ്ഥിരം അക്ഷരം *str);
ബാഹ്യ ശൂന്യം sched_dynamic_update(പൂർണ്ണസംഖ്യ mode);
#endif
ബാഹ്യ സ്ഥിരം അക്ഷരം *preempt_modes[];

#ifdef CONFIG_SCHED_MM_CID

സ്ഥിതം __always_inline bool cid_on_cpu(unsigned പൂർണ്ണസംഖ്യ cid)
{
	തിരികെ cid & MM_CID_ONCPU;
}

സ്ഥിതം __always_inline bool cid_in_transit(unsigned പൂർണ്ണസംഖ്യ cid)
{
	തിരികെ cid & MM_CID_TRANSIT;
}

സ്ഥിതം __always_inline unsigned പൂർണ്ണസംഖ്യ cpu_cid_to_cid(unsigned പൂർണ്ണസംഖ്യ cid)
{
	തിരികെ cid & ~MM_CID_ONCPU;
}

സ്ഥിതം __always_inline unsigned പൂർണ്ണസംഖ്യ cid_to_cpu_cid(unsigned പൂർണ്ണസംഖ്യ cid)
{
	തിരികെ cid | MM_CID_ONCPU;
}

സ്ഥിതം __always_inline unsigned പൂർണ്ണസംഖ്യ cid_to_transit_cid(unsigned പൂർണ്ണസംഖ്യ cid)
{
	തിരികെ cid | MM_CID_TRANSIT;
}

സ്ഥിതം __always_inline unsigned പൂർണ്ണസംഖ്യ cid_from_transit_cid(unsigned പൂർണ്ണസംഖ്യ cid)
{
	തിരികെ cid & ~MM_CID_TRANSIT;
}

സ്ഥിതം __always_inline bool cid_on_task(unsigned പൂർണ്ണസംഖ്യ cid)
{
	/* True if none of the MM_CID_ONCPU, MM_CID_TRANSIT, MM_CID_UNSET bits is set */
	തിരികെ cid < MM_CID_TRANSIT;
}

സ്ഥിതം __always_inline ശൂന്യം mm_drop_cid(ഘടന mm_struct *mm, unsigned പൂർണ്ണസംഖ്യ cid)
{
	clear_bit(cid, mm_cidmask(mm));
}

സ്ഥിതം __always_inline ശൂന്യം mm_unset_cid_on_task(ഘടന task_struct *t)
{
	unsigned പൂർണ്ണസംഖ്യ cid = t->mm_cid.cid;

	t->mm_cid.cid = MM_CID_UNSET;
	എങ്കിൽ (cid_on_task(cid))
		mm_drop_cid(t->mm, cid);
}

സ്ഥിതം __always_inline ശൂന്യം mm_drop_cid_on_cpu(ഘടന mm_struct *mm, ഘടന mm_cid_pcpu *pcp)
{
	/* Clear the ONCPU bit, but do not set UNSET in the per CPU storage */
	എങ്കിൽ (cid_on_cpu(pcp->cid)) {
		pcp->cid = cpu_cid_to_cid(pcp->cid);
		mm_drop_cid(mm, pcp->cid);
	}
}

സ്ഥിതം വരിയിൽ unsigned പൂർണ്ണസംഖ്യ __mm_get_cid(ഘടന mm_struct *mm, unsigned പൂർണ്ണസംഖ്യ max_cids)
{
	unsigned പൂർണ്ണസംഖ്യ cid = find_first_zero_bit(mm_cidmask(mm), max_cids);

	എങ്കിൽ (cid >= max_cids)
		തിരികെ MM_CID_UNSET;
	എങ്കിൽ (test_and_set_bit(cid, mm_cidmask(mm)))
		തിരികെ MM_CID_UNSET;
	തിരികെ cid;
}

സ്ഥിതം വരിയിൽ unsigned പൂർണ്ണസംഖ്യ mm_get_cid(ഘടന mm_struct *mm)
{
	unsigned പൂർണ്ണസംഖ്യ cid = __mm_get_cid(mm, READ_ONCE(mm->mm_cid.max_cids));

	ആകുമ്പോൾ (cid == MM_CID_UNSET) {
		cpu_relax();
		cid = __mm_get_cid(mm, അക്കം_possible_cpus());
	}
	തിരികെ cid;
}

സ്ഥിതം വരിയിൽ unsigned പൂർണ്ണസംഖ്യ mm_cid_converge(ഘടന mm_struct *mm, unsigned പൂർണ്ണസംഖ്യ orig_cid,
					   unsigned പൂർണ്ണസംഖ്യ max_cids)
{
	unsigned പൂർണ്ണസംഖ്യ new_cid, cid = cpu_cid_to_cid(orig_cid);

	/* Is it in the optimal CID space? */
	എങ്കിൽ (likely(cid < max_cids))
		തിരികെ orig_cid;

	/* Try to find one in the optimal space. Otherwise keep the provided. */
	new_cid = __mm_get_cid(mm, max_cids);
	എങ്കിൽ (new_cid != MM_CID_UNSET) {
		mm_drop_cid(mm, cid);
		/* Preserve the ONCPU mode of the original CID */
		തിരികെ new_cid | (orig_cid & MM_CID_ONCPU);
	}
	തിരികെ orig_cid;
}

സ്ഥിതം __always_inline ശൂന്യം mm_cid_update_task_cid(ഘടന task_struct *t, unsigned പൂർണ്ണസംഖ്യ cid)
{
	എങ്കിൽ (t->mm_cid.cid != cid) {
		t->mm_cid.cid = cid;
		rseq_sched_set_ids_changed(t);
	}
}

സ്ഥിതം __always_inline ശൂന്യം mm_cid_update_pcpu_cid(ഘടന mm_struct *mm, unsigned പൂർണ്ണസംഖ്യ cid)
{
	__this_cpu_write(mm->mm_cid.pcpu->cid, cid);
}

സ്ഥിതം __always_inline ശൂന്യം mm_cid_from_cpu(ഘടന task_struct *t, unsigned പൂർണ്ണസംഖ്യ cpu_cid,
					    unsigned പൂർണ്ണസംഖ്യ mode)
{
	unsigned പൂർണ്ണസംഖ്യ max_cids, tcid = t->mm_cid.cid;
	ഘടന mm_struct *mm = t->mm;

	max_cids = READ_ONCE(mm->mm_cid.max_cids);
	/* Optimize for the common case where both have the ONCPU bit set */
	എങ്കിൽ (likely(cid_on_cpu(cpu_cid & tcid))) {
		എങ്കിൽ (likely(cpu_cid_to_cid(cpu_cid) < max_cids)) {
			mm_cid_update_task_cid(t, cpu_cid);
			തിരികെ;
		}
		/* Try to converge into the optimal CID space */
		cpu_cid = mm_cid_converge(mm, cpu_cid, max_cids);
	} അല്ലെങ്കിൽ {
		/* Hand over or drop the task owned CID */
		എങ്കിൽ (cid_on_task(tcid)) {
			എങ്കിൽ (cid_on_cpu(cpu_cid))
				mm_unset_cid_on_task(t);
			അല്ലെങ്കിൽ
				cpu_cid = cid_to_cpu_cid(tcid);
		}
		/* Still nothing, allocate a new one */
		എങ്കിൽ (!cid_on_cpu(cpu_cid))
			cpu_cid = cid_to_cpu_cid(mm_get_cid(mm));

		/* Handle the transition mode flag if required */
		എങ്കിൽ (mode & MM_CID_TRANSIT)
			cpu_cid = cpu_cid_to_cid(cpu_cid) | MM_CID_TRANSIT;
	}
	mm_cid_update_pcpu_cid(mm, cpu_cid);
	mm_cid_update_task_cid(t, cpu_cid);
}

സ്ഥിതം __always_inline ശൂന്യം mm_cid_from_task(ഘടന task_struct *t, unsigned പൂർണ്ണസംഖ്യ cpu_cid,
					     unsigned പൂർണ്ണസംഖ്യ mode)
{
	unsigned പൂർണ്ണസംഖ്യ max_cids, tcid = t->mm_cid.cid;
	ഘടന mm_struct *mm = t->mm;

	max_cids = READ_ONCE(mm->mm_cid.max_cids);
	/* Optimize for the common case, where both have the ONCPU bit clear */
	എങ്കിൽ (likely(cid_on_task(tcid | cpu_cid))) {
		എങ്കിൽ (likely(tcid < max_cids)) {
			mm_cid_update_pcpu_cid(mm, tcid);
			തിരികെ;
		}
		/* Try to converge into the optimal CID space */
		tcid = mm_cid_converge(mm, tcid, max_cids);
	} അല്ലെങ്കിൽ {
		/* Hand over or drop the CPU owned CID */
		എങ്കിൽ (cid_on_cpu(cpu_cid)) {
			എങ്കിൽ (cid_on_task(tcid))
				mm_drop_cid_on_cpu(mm, this_cpu_ptr(mm->mm_cid.pcpu));
			അല്ലെങ്കിൽ
				tcid = cpu_cid_to_cid(cpu_cid);
		}
		/* Still nothing, allocate a new one */
		എങ്കിൽ (!cid_on_task(tcid))
			tcid = mm_get_cid(mm);
		/* Set the transition mode flag if required */
		tcid |= mode & MM_CID_TRANSIT;
	}
	mm_cid_update_pcpu_cid(mm, tcid);
	mm_cid_update_task_cid(t, tcid);
}

സ്ഥിതം __always_inline ശൂന്യം mm_cid_schedin(ഘടന task_struct *next)
{
	ഘടന mm_struct *mm = next->mm;
	unsigned പൂർണ്ണസംഖ്യ cpu_cid, mode;

	എങ്കിൽ (!next->mm_cid.active)
		തിരികെ;

	cpu_cid = __this_cpu_read(mm->mm_cid.pcpu->cid);
	mode = READ_ONCE(mm->mm_cid.mode);
	എങ്കിൽ (likely(!cid_on_cpu(mode)))
		mm_cid_from_task(next, cpu_cid, mode);
	അല്ലെങ്കിൽ
		mm_cid_from_cpu(next, cpu_cid, mode);
}

സ്ഥിതം __always_inline ശൂന്യം mm_cid_schedout(ഘടന task_struct *prev)
{
	ഘടന mm_struct *mm = prev->mm;
	unsigned പൂർണ്ണസംഖ്യ mode, cid;

	/* During mode transitions CIDs are temporary and need to be dropped */
	എങ്കിൽ (likely(!cid_in_transit(prev->mm_cid.cid)))
		തിരികെ;

	mode = READ_ONCE(mm->mm_cid.mode);
	cid = cid_from_transit_cid(prev->mm_cid.cid);

	/*
	 * If transition mode is done, transfer ownership when the CID is
	 * within the convergence range to optimize the next schedule in.
	 */
	എങ്കിൽ (!cid_in_transit(mode) && cid < READ_ONCE(mm->mm_cid.max_cids)) {
		എങ്കിൽ (cid_on_cpu(mode))
			cid = cid_to_cpu_cid(cid);

		/* Update both so that the next schedule in goes into the fast path */
		mm_cid_update_pcpu_cid(mm, cid);
		prev->mm_cid.cid = cid;
	} അല്ലെങ്കിൽ {
		mm_drop_cid(mm, cid);
		prev->mm_cid.cid = MM_CID_UNSET;
	}
}

സ്ഥിതം വരിയിൽ ശൂന്യം mm_cid_switch_to(ഘടന task_struct *prev, ഘടന task_struct *next)
{
	mm_cid_schedout(prev);
	mm_cid_schedin(next);
}

#അല്ലെങ്കിൽ /* !CONFIG_SCHED_MM_CID: */
സ്ഥിതം വരിയിൽ ശൂന്യം mm_cid_switch_to(ഘടന task_struct *prev, ഘടന task_struct *next) { }
#endif /* !CONFIG_SCHED_MM_CID */

#ifdef CONFIG_SCHED_CACHE
DECLARE_STATIC_താക്കോൽ_FALSE(sched_cache_present);
DECLARE_STATIC_താക്കോൽ_FALSE(sched_cache_active);
ബാഹ്യ പൂർണ്ണസംഖ്യ sysctl_sched_cache_user;
ബാഹ്യ unsigned പൂർണ്ണസംഖ്യ llc_aggr_tolerance;
ബാഹ്യ unsigned പൂർണ്ണസംഖ്യ llc_epoch_period;
ബാഹ്യ unsigned പൂർണ്ണസംഖ്യ llc_epoch_affinity_timeout;
ബാഹ്യ unsigned പൂർണ്ണസംഖ്യ llc_imb_pct;
ബാഹ്യ unsigned പൂർണ്ണസംഖ്യ llc_overaggr_pct;

സ്ഥിതം വരിയിൽ bool sched_cache_enabled(ശൂന്യം)
{
	തിരികെ static_branch_unlikely(&sched_cache_active);
}

ബാഹ്യ ശൂന്യം sched_cache_active_set(ശൂന്യം);

#endif

ശൂന്യം sched_domains_free_llc_id(പൂർണ്ണസംഖ്യ cpu);

ബാഹ്യ ശൂന്യം init_sched_mm(ഘടന task_struct *p);

ബാഹ്യ u64 avg_vruntime(ഘടന cfs_rq *cfs_rq);
ബാഹ്യ പൂർണ്ണസംഖ്യ entity_eligible(ഘടന cfs_rq *cfs_rq, ഘടന sched_entity *se);
സ്ഥിതം വരിയിൽ
ശൂന്യം move_queued_task_locked(ഘടന rq *src_rq, ഘടന rq *dst_rq, ഘടന task_struct *task)
{
	lockdep_assert_rq_held(src_rq);
	lockdep_assert_rq_held(dst_rq);

	deactivate_task(src_rq, task, 0);
	set_task_cpu(task, dst_rq->cpu);
	activate_task(dst_rq, task, 0);
	wakeup_preempt(dst_rq, task, 0);
}

സ്ഥിതം വരിയിൽ
bool task_is_pushable(ഘടന rq *rq, ഘടന task_struct *p, പൂർണ്ണസംഖ്യ cpu)
{
	എങ്കിൽ (!task_on_cpu(rq, p) &&
	    cpumask_test_cpu(cpu, &p->cpus_mask))
		തിരികെ true;

	തിരികെ false;
}

#ifdef CONFIG_RT_MUTEXES

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ __rt_effective_prio(ഘടന task_struct *pi_task, പൂർണ്ണസംഖ്യ prio)
{
	എങ്കിൽ (pi_task)
		prio = min(prio, pi_task->prio);

	തിരികെ prio;
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ rt_effective_prio(ഘടന task_struct *p, പൂർണ്ണസംഖ്യ prio)
{
	ഘടന task_struct *pi_task = rt_mutex_get_മുകൾ_task(p);

	തിരികെ __rt_effective_prio(pi_task, prio);
}

#അല്ലെങ്കിൽ /* !CONFIG_RT_MUTEXES: */

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ rt_effective_prio(ഘടന task_struct *p, പൂർണ്ണസംഖ്യ prio)
{
	തിരികെ prio;
}

#endif /* !CONFIG_RT_MUTEXES */

ബാഹ്യ പൂർണ്ണസംഖ്യ __sched_setscheduler(ഘടന task_struct *p, സ്ഥിരം ഘടന sched_attr *attr, bool user, bool pi);
ബാഹ്യ പൂർണ്ണസംഖ്യ __sched_setaffinity(ഘടന task_struct *p, ഘടന affinity_context *ctx);
ബാഹ്യ സ്ഥിരം ഘടന sched_class *__setscheduler_class(പൂർണ്ണസംഖ്യ policy, പൂർണ്ണസംഖ്യ prio);
ബാഹ്യ ശൂന്യം set_load_weight(ഘടന task_struct *p, bool update_load);
ബാഹ്യ ശൂന്യം enqueue_task(ഘടന rq *rq, ഘടന task_struct *p, പൂർണ്ണസംഖ്യ flags);
ബാഹ്യ bool dequeue_task(ഘടന rq *rq, ഘടന task_struct *p, പൂർണ്ണസംഖ്യ flags);

ബാഹ്യ ഘടന balance_callback *splice_balance_callbacks(ഘടന rq *rq);

ബാഹ്യ ശൂന്യം __balance_callbacks(ഘടന rq *rq, ഘടന rq_flags *rf);
ബാഹ്യ ശൂന്യം balance_callbacks(ഘടന rq *rq, ഘടന balance_callback *head);

/*
 * The 'sched_change' pattern is the safe, easy and slow way of changing a
 * task's scheduling properties. It dequeues a task, such that the scheduler
 * is fully unaware of it; at which point its properties can be modified;
 * after which it is enqueued again.
 *
 * Typically this must be called while holding task_rq_lock, since most/all
 * properties are serialized under those locks. There are currently two
 * exceptions to this rule in sched/ext which only hold rq->lock: scx_bypass()
 * and rq_offline_scx().
 */

/*
 * This structure is a temporary, used to preserve/convey the queueing state
 * of the task between sched_change_begin() and sched_change_end(). Ensuring
 * the task's queueing state is idempotent across the operation.
 */
ഘടന sched_change_ctx {
	u64			prio;
	ഘടന task_struct	*p;
	സ്ഥിരം ഘടന sched_class *class;
	പൂർണ്ണസംഖ്യ			flags;
	bool			queued;
	bool			running;
};

ഘടന sched_change_ctx *sched_change_begin(ഘടന task_struct *p, unsigned പൂർണ്ണസംഖ്യ flags);
ശൂന്യം sched_change_end(ഘടന sched_change_ctx *ctx);

DEFINE_CLASS(sched_change, ഘടന sched_change_ctx *,
	     sched_change_end(_T),
	     sched_change_begin(p, flags),
	     ഘടന task_struct *p, unsigned പൂർണ്ണസംഖ്യ flags)

DEFINE_CLASS_IS_UNCONDITIONAL(sched_change)

#അന്തര്ഭാവ "ext/ext.h"

#ifdef CONFIG_PREFERRED_CPU
ശൂന്യം sched_തള്ളുക_current_non_preferred_cpu(ഘടന rq *rq);
#അല്ലെങ്കിൽ	/* !CONFIG_PREFERRED_CPU */
സ്ഥിതം വരിയിൽ ശൂന്യം sched_തള്ളുക_current_non_preferred_cpu(ഘടന rq *rq) { }
#endif

#endif /* _KERNEL_SCHED_SCHED_H */
