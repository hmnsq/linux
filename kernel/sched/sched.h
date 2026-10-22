/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Scheduler internal types and methods:
 */
#ifndef _KERNEL_SCHED_SCHED_H
#define _KERNEL_SCHED_SCHED_H

#অন্তর্ভাব <linux/prandom.h>
#অন্তর্ভাব <linux/sched/affinity.h>
#অন্তর্ভাব <linux/sched/autogroup.h>
#অন্তর্ভাব <linux/sched/cpufreq.h>
#অন্তর্ভাব <linux/sched/deadline.h>
#অন্তর্ভাব <linux/sched.h>
#অন্তর্ভাব <linux/sched/loadavg.h>
#অন্তর্ভাব <linux/sched/mm.h>
#অন্তর্ভাব <linux/sched/rseq_api.h>
#অন্তর্ভাব <linux/sched/signal.h>
#অন্তর্ভাব <linux/sched/smt.h>
#অন্তর্ভাব <linux/sched/stat.h>
#অন্তর্ভাব <linux/sched/sysctl.h>
#অন্তর্ভাব <linux/sched/task_flags.h>
#অন্তর্ভাব <linux/sched/task.h>
#অন্তর্ভাব <linux/sched/topology.h>
#অন্তর্ভাব <linux/atomic.h>
#অন্তর্ভাব <linux/bitmap.h>
#অন্তর্ভাব <linux/bug.h>
#অন্তর্ভাব <linux/capability.h>
#অন্তর্ভাব <linux/cgroup_api.h>
#অন্তর্ভাব <linux/cgroup.h>
#অন্তর্ভাব <linux/context_tracking.h>
#অন্তর্ভাব <linux/cpufreq.h>
#অন্তর্ভাব <linux/cpumask_api.h>
#অন্তর্ভাব <linux/cpuset.h>
#অন্তর্ভাব <linux/ctype.h>
#অন্তর্ভাব <linux/file.h>
#অন্তর্ভাব <linux/fs_api.h>
#অন্তর্ভাব <linux/hrtimer_api.h>
#অন্তর্ভাব <linux/interrupt.h>
#অন্তর্ভাব <linux/irq_work.h>
#অন্তর্ভাব <linux/jiffies.h>
#অন্তর্ভাব <linux/kref_api.h>
#অন্তর্ভাব <linux/kthread.h>
#অন্তর্ভাব <linux/ktime_api.h>
#অন্তর্ভাব <linux/lockdep_api.h>
#অন্তর্ভাব <linux/lockdep.h>
#অন্তর্ভাব <linux/memblock.h>
#অন্তর্ভাব <linux/memcontrol.h>
#অন্তর্ভাব <linux/minmax.h>
#অন্তর্ভাব <linux/mm.h>
#অন্তর্ভাব <linux/module.h>
#অন্তর্ভাব <linux/mutex_api.h>
#অন্তর্ভাব <linux/plist.h>
#অন্তর্ভাব <linux/poll.h>
#অন্তর্ভাব <linux/proc_fs.h>
#অন্তর্ভাব <linux/profile.h>
#অন্তর্ভাব <linux/psi.h>
#অন্তর্ভাব <linux/rcupdate.h>
#অন্তর্ভাব <linux/seq_file.h>
#অন্তর্ভাব <linux/seqlock.h>
#অন্তর্ভাব <linux/softirq.h>
#অন্তর্ভাব <linux/spinlock_api.h>
#অন্তর্ভাব <linux/static_key.h>
#অন্তর্ভাব <linux/stop_machine.h>
#অন্তর্ভাব <linux/syscalls_api.h>
#অন্তর্ভাব <linux/syscalls.h>
#অন্তর্ভাব <linux/tick.h>
#অন্তর্ভাব <linux/topology.h>
#অন্তর্ভাব <linux/types.h>
#অন্তর্ভাব <linux/u64_stats_sync_api.h>
#অন্তর্ভাব <linux/uaccess.h>
#অন্তর্ভাব <linux/vmstat.h>
#অন্তর্ভাব <linux/wait_api.h>
#অন্তর্ভাব <linux/wait_bit.h>
#অন্তর্ভাব <linux/workqueue_api.h>
#অন্তর্ভাব <linux/delayacct.h>
#অন্তর্ভাব <linux/mmu_context.h>

#অন্তর্ভাব <trace/events/power.h>
#অন্তর্ভাব <trace/events/sched.h>

#অন্তর্ভাব "../workqueue_internal.h"

সংরচনা rq;
সংরচনা cfs_rq;
সংরচনা rt_rq;
সংরচনা sched_group;
সংরচনা cpuidle_state;

#যদি defined(CONFIG_PARAVIRT) && !defined(CONFIG_HAVE_PV_STEAL_CLOCK_GEN)
# include <asm/paravirt.h>
#endif

#অন্তর্ভাব <asm/barrier.h>

#অন্তর্ভাব "cpupri.h"
#অন্তর্ভাব "cpudeadline.h"

/* task_struct::on_rq states: */
#define TASK_ON_RQ_QUEUED	1
#define TASK_ON_RQ_MIGRATING	2

বাহ্য __read_mostly পূর্ণাঙ্ক scheduler_running;

বাহ্য unsigned long calc_load_update;
বাহ্য atomic_long_t calc_load_tasks;

বাহ্য শূন্য calc_global_load_tick(সংরচনা rq *this_rq);
বাহ্য long calc_load_fold_active(সংরচনা rq *this_rq, long adjust);

বাহ্য শূন্য call_trace_sched_update_nr_running(সংরচনা rq *rq, পূর্ণাঙ্ক গুনতি);

বাহ্য পূর্ণাঙ্ক sysctl_sched_rt_period;
বাহ্য পূর্ণাঙ্ক sysctl_sched_rt_runtime;
বাহ্য পূর্ণাঙ্ক sched_rr_timeslice;

/*
 * Asymmetric CPU capacity bits
 */
সংরচনা asym_cap_data {
	সংরচনা list_head link;
	সংরচনা rcu_head rcu;
	unsigned long capacity;
	unsigned long cpus[];
};

বাহ্য সংরচনা list_head asym_cap_list;

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
	যদি (__w)						\
		__w = max(2UL, __w >> SCHED_FIXEDPOINT_SHIFT);	\
	__w;							\
})
#অন্যথা
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

স্থির অন্তঃস্থ পূর্ণাঙ্ক idle_policy(পূর্ণাঙ্ক policy)
{
	প্রত্যাগম policy == SCHED_IDLE;
}

স্থির অন্তঃস্থ পূর্ণাঙ্ক normal_policy(পূর্ণাঙ্ক policy)
{
#ifdef CONFIG_SCHED_CLASS_EXT
	যদি (policy == SCHED_EXT)
		প্রত্যাগম true;
#endif
	প্রত্যাগম policy == SCHED_NORMAL;
}

স্থির অন্তঃস্থ পূর্ণাঙ্ক fair_policy(পূর্ণাঙ্ক policy)
{
	প্রত্যাগম normal_policy(policy) || policy == SCHED_BATCH;
}

স্থির অন্তঃস্থ পূর্ণাঙ্ক rt_policy(পূর্ণাঙ্ক policy)
{
	প্রত্যাগম policy == SCHED_FIFO || policy == SCHED_RR;
}

স্থির অন্তঃস্থ পূর্ণাঙ্ক dl_policy(পূর্ণাঙ্ক policy)
{
	প্রত্যাগম policy == SCHED_DEADLINE;
}

স্থির অন্তঃস্থ bool valid_policy(পূর্ণাঙ্ক policy)
{
	প্রত্যাগম idle_policy(policy) || fair_policy(policy) ||
		rt_policy(policy) || dl_policy(policy);
}

স্থির অন্তঃস্থ পূর্ণাঙ্ক task_has_idle_policy(সংরচনা task_struct *p)
{
	প্রত্যাগম idle_policy(p->policy);
}

স্থির অন্তঃস্থ পূর্ণাঙ্ক task_has_rt_policy(সংরচনা task_struct *p)
{
	প্রত্যাগম rt_policy(p->policy);
}

স্থির অন্তঃস্থ পূর্ণাঙ্ক task_has_dl_policy(সংরচনা task_struct *p)
{
	প্রত্যাগম dl_policy(p->policy);
}

#define cap_scale(v, s)		((v)*(s) >> SCHED_CAPACITY_SHIFT)

স্থির অন্তঃস্থ শূন্য update_avg(u64 *avg, u64 sample)
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
স্থির অন্তঃস্থ unsigned long sched_weight_from_cgroup(unsigned long cgrp_weight)
{
	প্রত্যাগম DIV_ROUND_CLOSEST_ULL(cgrp_weight * 1024, CGROUP_WEIGHT_DFL);
}

স্থির অন্তঃস্থ unsigned long sched_weight_to_cgroup(unsigned long weight)
{
	প্রত্যাগম clamp_t(unsigned long,
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

স্থির অন্তঃস্থ bool dl_entity_is_special(ধ্রুব সংরচনা sched_dl_entity *dl_se)
{
#ifdef CONFIG_CPU_FREQ_GOV_SCHEDUTIL
	প্রত্যাগম unlikely(dl_se->flags & SCHED_FLAG_SUGOV);
#অন্যথা
	প্রত্যাগম false;
#endif
}

/*
 * Tells if entity @a should preempt entity @b.
 */
স্থির অন্তঃস্থ bool dl_entity_preempt(ধ্রুব সংরচনা sched_dl_entity *a,
				     ধ্রুব সংরচনা sched_dl_entity *b)
{
	প্রত্যাগম dl_entity_is_special(a) ||
	       dl_time_before(a->deadline, b->deadline);
}

/*
 * This is the priority-queue data structure of the RT scheduling class:
 */
সংরচনা rt_prio_array {
	DECLARE_BITMAP(bitmap, MAX_RT_PRIO+1); /* include 1 bit for delimiter */
	সংরচনা list_head queue[MAX_RT_PRIO];
};

সংরচনা rt_bandwidth {
	/* nests inside the rq lock: */
	raw_spinlock_t		rt_runtime_lock;
	ktime_t			rt_period;
	u64			rt_runtime;
	সংরচনা hrtimer		rt_period_timer;
	unsigned পূর্ণাঙ্ক		rt_period_active;
};

স্থির অন্তঃস্থ পূর্ণাঙ্ক dl_bandwidth_enabled(শূন্য)
{
	প্রত্যাগম sysctl_sched_rt_runtime >= 0;
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
সংরচনা dl_bw {
	raw_spinlock_t		lock;
	u64			bw;
	u64			total_bw;
};

বাহ্য শূন্য init_dl_bw(সংরচনা dl_bw *dl_b);
বাহ্য পূর্ণাঙ্ক  sched_dl_global_validate(শূন্য);
বাহ্য শূন্য sched_dl_do_global(শূন্য);
বাহ্য পূর্ণাঙ্ক  sched_dl_overflow(সংরচনা task_struct *p, পূর্ণাঙ্ক policy, ধ্রুব সংরচনা sched_attr *attr);
বাহ্য শূন্য __setparam_dl(সংরচনা task_struct *p, ধ্রুব সংরচনা sched_attr *attr);
বাহ্য শূন্য __getparam_dl(সংরচনা task_struct *p, সংরচনা sched_attr *attr, unsigned পূর্ণাঙ্ক flags);
বাহ্য bool __checkparam_dl(ধ্রুব সংরচনা sched_attr *attr);
বাহ্য bool dl_param_changed(সংরচনা task_struct *p, ধ্রুব সংরচনা sched_attr *attr);
বাহ্য পূর্ণাঙ্ক  dl_cpuset_cpumask_can_shrink(ধ্রুব সংরচনা cpumask *cur, ধ্রুব সংরচনা cpumask *trial);
বাহ্য পূর্ণাঙ্ক  dl_bw_deactivate(পূর্ণাঙ্ক cpu);
বাহ্য s64 dl_scaled_delta_exec(সংরচনা rq *rq, সংরচনা sched_dl_entity *dl_se, s64 delta_exec);
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
বাহ্য শূন্য dl_server_update_idle(সংরচনা sched_dl_entity *dl_se, s64 delta_exec);
বাহ্য শূন্য dl_server_update(সংরচনা sched_dl_entity *dl_se, s64 delta_exec);
বাহ্য শূন্য dl_server_start(সংরচনা sched_dl_entity *dl_se);
বাহ্য শূন্য dl_server_stop(সংরচনা sched_dl_entity *dl_se);
বাহ্য শূন্য dl_server_init(সংরচনা sched_dl_entity *dl_se, সংরচনা rq *rq,
		    dl_server_pick_f pick_task);
বাহ্য শূন্য sched_init_dl_servers(শূন্য);

বাহ্য শূন্য fair_server_init(সংরচনা rq *rq);
বাহ্য শূন্য ext_server_init(সংরচনা rq *rq);
বাহ্য শূন্য __dl_server_attach_root(সংরচনা sched_dl_entity *dl_se, সংরচনা rq *rq);
বাহ্য পূর্ণাঙ্ক dl_server_apply_params(সংরচনা sched_dl_entity *dl_se,
		    u64 runtime, u64 period, bool init);
বাহ্য পূর্ণাঙ্ক dl_server_attach_bw(সংরচনা sched_dl_entity *dl_se);
বাহ্য শূন্য dl_server_detach_bw(সংরচনা sched_dl_entity *dl_se);
বাহ্য পূর্ণাঙ্ক dl_server_swap_bw(সংরচনা sched_dl_entity *detach_se,
			     সংরচনা sched_dl_entity *attach_se);

স্থির অন্তঃস্থ bool dl_server_active(সংরচনা sched_dl_entity *dl_se)
{
	প্রত্যাগম dl_se->dl_server_active;
}

#ifdef CONFIG_CGROUP_SCHED

বাহ্য সংরচনা list_head task_groups;

#ifdef CONFIG_GROUP_SCHED_BANDWIDTH
বাহ্য ধ্রুব u64 max_bw_quota_period_us;

/*
 * default period for group bandwidth.
 * default: 0.1s, units: microseconds
 */
স্থির অন্তঃস্থ u64 default_bw_period_us(শূন্য)
{
	প্রত্যাগম 100000ULL;
}
#endif /* CONFIG_GROUP_SCHED_BANDWIDTH */

সংরচনা cfs_bandwidth {
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
	সংরচনা hrtimer		period_timer;
	সংরচনা hrtimer		slack_timer;
	সংরচনা list_head	throttled_cfs_rq;

	/* Statistics: */
	পূর্ণাঙ্ক			nr_periods;
	পূর্ণাঙ্ক			nr_throttled;
	পূর্ণাঙ্ক			nr_burst;
	u64			throttled_time;
	u64			burst_time;
#endif /* CONFIG_CFS_BANDWIDTH */
};

/* Task group related information */
সংরচনা task_group {
	সংরচনা cgroup_subsys_state css;

#ifdef CONFIG_GROUP_SCHED_WEIGHT
	/* A positive value indicates that this is a SCHED_IDLE group. */
	পূর্ণাঙ্ক			idle;
#endif

#ifdef CONFIG_FAIR_GROUP_SCHED
	/* runqueue "owned" by this group on each CPU */
	সংরচনা cfs_rq __percpu	*cfs_rq;
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
	সংরচনা sched_rt_entity	**rt_se;
	সংরচনা rt_rq		**rt_rq;

	সংরচনা rt_bandwidth	rt_bandwidth;
#endif

	সংরচনা scx_task_group	scx;

	সংরচনা rcu_head		rcu;
	সংরচনা list_head	list;

	সংরচনা task_group	*parent;
	সংরচনা list_head	siblings;
	সংরচনা list_head	children;

#ifdef CONFIG_SCHED_AUTOGROUP
	সংরচনা autogroup	*autogroup;
#endif

	সংরচনা cfs_bandwidth	cfs_bandwidth;

#ifdef CONFIG_UCLAMP_TASK_GROUP
	/* The two decimal precision [%] value requested from user-space */
	unsigned পূর্ণাঙ্ক		uclamp_pct[UCLAMP_CNT];
	/* Clamp values requested for a task group */
	সংরচনা uclamp_se	uclamp_req[UCLAMP_CNT];
	/* Effective clamp values used for a task group */
	সংরচনা uclamp_se	uclamp[UCLAMP_CNT];
#endif

};

#define মূল_TASK_GROUP_LOAD	NICE_0_LOAD

প্রকারনির্দেশ পূর্ণাঙ্ক (*tg_visitor)(সংরচনা task_group *, শূন্য *);

বাহ্য পূর্ণাঙ্ক walk_tg_বৃক্ষ_from(সংরচনা task_group *from,
			     tg_visitor down, tg_visitor up, শূন্য *data);

/*
 * Iterate the full tree, calling @down when first entering a node and @up when
 * leaving it for the final time.
 *
 * Caller must hold rcu_lock or sufficient equivalent.
 */
স্থির অন্তঃস্থ পূর্ণাঙ্ক walk_tg_বৃক্ষ(tg_visitor down, tg_visitor up, শূন্য *data)
{
	প্রত্যাগম walk_tg_বৃক্ষ_from(&মূল_task_group, down, up, data);
}

স্থির অন্তঃস্থ সংরচনা task_group *css_tg(সংরচনা cgroup_subsys_state *css)
{
	প্রত্যাগম css ? container_of(css, সংরচনা task_group, css) : রিক্ত;
}

বাহ্য পূর্ণাঙ্ক tg_nop(সংরচনা task_group *tg, শূন্য *data);

#ifdef CONFIG_FAIR_GROUP_SCHED
বাহ্য শূন্য free_fair_sched_group(সংরচনা task_group *tg);
বাহ্য পূর্ণাঙ্ক alloc_fair_sched_group(সংরচনা task_group *tg, সংরচনা task_group *parent);
বাহ্য শূন্য online_fair_sched_group(সংরচনা task_group *tg);
বাহ্য শূন্য unregister_fair_sched_group(সংরচনা task_group *tg);
বাহ্য শূন্য __sched_cgroup_mode_update(পূর্ণাঙ্ক mode);
#অন্যথা /* !CONFIG_FAIR_GROUP_SCHED: */
স্থির অন্তঃস্থ শূন্য free_fair_sched_group(সংরচনা task_group *tg) { }
স্থির অন্তঃস্থ পূর্ণাঙ্ক alloc_fair_sched_group(সংরচনা task_group *tg, সংরচনা task_group *parent)
{
       প্রত্যাগম 1;
}
স্থির অন্তঃস্থ শূন্য online_fair_sched_group(সংরচনা task_group *tg) { }
স্থির অন্তঃস্থ শূন্য unregister_fair_sched_group(সংরচনা task_group *tg) { }
#endif /* !CONFIG_FAIR_GROUP_SCHED */

বাহ্য শূন্য init_tg_cfs_entry(সংরচনা task_group *tg, সংরচনা cfs_rq *cfs_rq,
			সংরচনা sched_entity *se, পূর্ণাঙ্ক cpu,
			সংরচনা sched_entity *parent);
বাহ্য শূন্য init_cfs_bandwidth(সংরচনা cfs_bandwidth *cfs_b, সংরচনা cfs_bandwidth *parent);

বাহ্য শূন্য __refill_cfs_bandwidth_runtime(সংরচনা cfs_bandwidth *cfs_b);
বাহ্য শূন্য start_cfs_bandwidth(সংরচনা cfs_bandwidth *cfs_b);
বাহ্য শূন্য unthrottle_cfs_rq(সংরচনা cfs_rq *cfs_rq);
বাহ্য bool cfs_task_bw_constrained(সংরচনা task_struct *p);

বাহ্য শূন্য init_tg_rt_entry(সংরচনা task_group *tg, সংরচনা rt_rq *rt_rq,
		সংরচনা sched_rt_entity *rt_se, পূর্ণাঙ্ক cpu,
		সংরচনা sched_rt_entity *parent);
বাহ্য পূর্ণাঙ্ক sched_group_set_rt_runtime(সংরচনা task_group *tg, long rt_runtime_us);
বাহ্য পূর্ণাঙ্ক sched_group_set_rt_period(সংরচনা task_group *tg, u64 rt_period_us);
বাহ্য long sched_group_rt_runtime(সংরচনা task_group *tg);
বাহ্য long sched_group_rt_period(সংরচনা task_group *tg);
বাহ্য পূর্ণাঙ্ক sched_rt_can_attach(সংরচনা task_group *tg, সংরচনা task_struct *tsk);

বাহ্য সংরচনা task_group *sched_create_group(সংরচনা task_group *parent);
বাহ্য শূন্য sched_online_group(সংরচনা task_group *tg,
			       সংরচনা task_group *parent);
বাহ্য শূন্য sched_destroy_group(সংরচনা task_group *tg);
বাহ্য শূন্য sched_release_group(সংরচনা task_group *tg);

বাহ্য শূন্য sched_move_task(সংরচনা task_struct *tsk, bool for_autogroup);

#ifdef CONFIG_FAIR_GROUP_SCHED
বাহ্য পূর্ণাঙ্ক sched_group_set_shares(সংরচনা task_group *tg, unsigned long shares);

বাহ্য পূর্ণাঙ্ক sched_group_set_idle(সংরচনা task_group *tg, long idle);

বাহ্য শূন্য set_task_rq_fair(সংরচনা sched_entity *se,
			     সংরচনা cfs_rq *prev, সংরচনা cfs_rq *next);
#অন্যথা /* !CONFIG_FAIR_GROUP_SCHED: */
স্থির অন্তঃস্থ পূর্ণাঙ্ক sched_group_set_shares(সংরচনা task_group *tg, unsigned long shares) { প্রত্যাগম 0; }
স্থির অন্তঃস্থ পূর্ণাঙ্ক sched_group_set_idle(সংরচনা task_group *tg, long idle) { প্রত্যাগম 0; }
#endif /* !CONFIG_FAIR_GROUP_SCHED */

#অন্যথা /* !CONFIG_CGROUP_SCHED: */

সংরচনা cfs_bandwidth { };

স্থির অন্তঃস্থ bool cfs_task_bw_constrained(সংরচনা task_struct *p) { প্রত্যাগম false; }

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

বাহ্য শূন্য unregister_rt_sched_group(সংরচনা task_group *tg);
বাহ্য শূন্য free_rt_sched_group(সংরচনা task_group *tg);
বাহ্য পূর্ণাঙ্ক alloc_rt_sched_group(সংরচনা task_group *tg, সংরচনা task_group *parent);

/*
 * u64_u32_load/u64_u32_store
 *
 * Use a copy of a u64 value to protect against data race. This is only
 * applicable for 32-bits architectures.
 */
#ifdef CONFIG_64BIT
# define u64_u32_load_copy(var, copy)		var
# define u64_u32_store_copy(var, copy, val)	(var = val)
#অন্যথা
# define u64_u32_load_copy(var, copy)					\
({									\
	u64 __val, __val_copy;						\
	কুরু {								\
		__val_copy = copy;					\
		/*							\
		 * paired with u64_u32_store_copy(), ordering access	\
		 * to var and copy.					\
		 */							\
		smp_rmb();						\
		__val = var;						\
	} যাবত্ (__val != __val_copy);					\
	__val;								\
})
# define u64_u32_store_copy(var, copy, val)				\
কুরু {									\
	typeof(val) __val = (val);					\
	var = __val;							\
	/*								\
	 * paired with u64_u32_load_copy(), ordering access to var and	\
	 * copy.							\
	 */								\
	smp_wmb();							\
	copy = __val;							\
} যাবত্ (0)
#endif
# define u64_u32_load(var)		u64_u32_load_copy(var, var##_copy)
# define u64_u32_store(var, val)	u64_u32_store_copy(var, var##_copy, val)

সংরচনা balance_callback {
	সংরচনা balance_callback *next;
	শূন্য (*func)(সংরচনা rq *rq);
};

/* Fair scheduling SCHED_{NORMAL,BATCH,IDLE} related fields in a runqueue: */
সংরচনা cfs_rq {
	সংরচনা load_weight	load;
	unsigned পূর্ণাঙ্ক		nr_queued;
	unsigned পূর্ণাঙ্ক		h_nr_queued;		/* SCHED_{NORMAL,BATCH,IDLE} */
	unsigned পূর্ণাঙ্ক		h_nr_runnable;		/* SCHED_{NORMAL,BATCH,IDLE} */
	unsigned পূর্ণাঙ্ক		h_nr_idle;		/* SCHED_IDLE */

	s64			sum_w_vruntime;
	u64			sum_weight;
	u64			zero_vruntime;
	unsigned পূর্ণাঙ্ক		sum_shift;

#ifdef CONFIG_SCHED_CORE
	unsigned পূর্ণাঙ্ক		forceidle_seq;
	u64			zero_vruntime_fi;
#endif

	সংরচনা rb_মূল_cached	tasks_timeline;

	/*
	 * 'curr' points to the currently running entity on this cfs_rq.
	 * It is set to NULL otherwise (i.e when none are currently running).
	 */
	সংরচনা sched_entity	*curr;
	সংরচনা sched_entity	*next;

	/*
	 * CFS load tracking
	 */
	সংরচনা sched_entity	*h_curr;
	সংরচনা sched_avg	avg;
#ifndef CONFIG_64BIT
	u64			last_update_time_copy;
#endif
	সংরচনা {
		raw_spinlock_t	lock ____cacheline_aligned;
		পূর্ণাঙ্ক		nr;
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
	সংরচনা sched_entity	*h_load_next;

	সংরচনা rq		*rq;	/* CPU runqueue to which this cfs_rq is attached */

	/*
	 * leaf cfs_rqs are those that hold tasks (lowest schedulable entity in
	 * a hierarchy). Non-leaf lrqs hold other higher schedulable entities
	 * (like users, containers etc.)
	 *
	 * leaf_cfs_rq_list ties together list of leaf cfs_rq's in a CPU.
	 * This list is used during load balance.
	 */
	পূর্ণাঙ্ক			on_list;
	সংরচনা list_head	leaf_cfs_rq_list;
	সংরচনা task_group	*tg;	/* Group that "owns" this runqueue */

	/* Locally cached copy of our task_group's idle value */
	পূর্ণাঙ্ক			idle;

# ifdef CONFIG_CFS_BANDWIDTH
	পূর্ণাঙ্ক			runtime_enabled;
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
	পূর্ণাঙ্ক			throttle_গুনতি;
	সংরচনা list_head	throttled_list;
	সংরচনা list_head	throttled_csd_list;
	সংরচনা list_head        throttled_limbo_list;
# endif /* CONFIG_CFS_BANDWIDTH */
#endif /* CONFIG_FAIR_GROUP_SCHED */
};

#ifdef CONFIG_SCHED_CLASS_EXT
/* scx_rq->flags, protected by the rq lock */
গণনা scx_rq_flags {
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
	SCX_RQ_মূল_IDLE_RENOTIFY	= 1 << 8, /* the root is owed update_idle() */

	SCX_RQ_IN_WAKEUP	= 1 << 16,
	SCX_RQ_IN_DISPATCH	= 1 << 17,
};

/* per-rq rescue execution state, see scx_rescue_timerfn() */
সংরচনা scx_rq_rescue {
	সংরচনা scx_dispatch_q	dsq;			/* stranded tasks awaiting rescue */
	s64			budget;			/* execution token bucket, ns */
	u64			clock;			/* last budget accrual timestamp */
	সংরচনা task_struct	*curr;			/* task being rescued, one at a time */
	s64			slice;			/* curr's admitted slice */
	u64			exec_snap;		/* sum_exec_runtime at admission */
	সংরচনা timer_list	timer;			/* paces admission and escalation */
	u64			kill_at;		/* last ejection, init before any */
};

সংরচনা scx_rq {
	সংরচনা scx_dispatch_q	local_dsq;
#ifdef CONFIG_EXT_SUB_SCHED
	সংরচনা scx_dispatch_q	reject_dsq;		/* staging for cap-rejected tasks */
	সংরচনা scx_rq_rescue	rescue;
#endif
	সংরচনা list_head	runnable_list;		/* runnable tasks on this rq */
	সংরচনা list_head	ddsp_deferred_locals;	/* deferred ddsps from enq */
	/* both stashed across the activate_task() in move_remote_task_to_local_dsq() */
	u64			remote_activate_enq_flags;
	সংরচনা scx_sched	*remote_activate_sch;
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
	সংরচনা llist_head	ecaps_to_sync;		/* pending ecaps syncs */
	সংরচনা task_struct	*sub_dispatch_prev;
#endif
	cpumask_var_t		cpus_to_sync;
	bool			kick_sync_pending;
	unsigned long		kick_sync;

	সংরচনা list_head	sched_pcpus_to_kick;	/* see kick_cpus_irq_workfn() */

	raw_spinlock_t		deferred_reenq_lock;
	সংরচনা list_head	deferred_reenq_locals;	/* scheds requesting reenq of local DSQ */
	সংরচনা list_head	deferred_reenq_users;	/* user DSQs requesting reenq */
	সংরচনা balance_callback	deferred_bal_cb;
	সংরচনা balance_callback	kick_sync_bal_cb;
	সংরচনা irq_work		deferred_irq_work;
	সংরচনা irq_work		kick_cpus_irq_work;
};
#endif /* CONFIG_SCHED_CLASS_EXT */

স্থির অন্তঃস্থ পূর্ণাঙ্ক rt_bandwidth_enabled(শূন্য)
{
	প্রত্যাগম sysctl_sched_rt_runtime >= 0;
}

/* RT IPI pull logic requires IRQ_WORK */
#যদি defined(CONFIG_IRQ_WORK) && defined(CONFIG_SMP)
# define HAVE_RT_ঠেলো_IPI
#endif

/* Real-Time classes' related field in a runqueue: */
সংরচনা rt_rq {
	সংরচনা rt_prio_array	active;
	unsigned পূর্ণাঙ্ক		rt_nr_running;
	unsigned পূর্ণাঙ্ক		rr_nr_running;
	সংরচনা {
		পূর্ণাঙ্ক		curr; /* highest queued rt task prio */
		পূর্ণাঙ্ক		next; /* next highest */
	} highest_prio;
	bool			overloaded;
	সংরচনা plist_head	pushable_tasks;

	পূর্ণাঙ্ক			rt_queued;

#ifdef CONFIG_RT_GROUP_SCHED
	পূর্ণাঙ্ক			rt_throttled;
	u64			rt_time; /* consumed RT time, goes up in update_curr_rt */
	u64			rt_runtime; /* allotted RT time, "slice" from rt_bandwidth, RT sharing/balancing */
	/* Nests inside the rq lock: */
	raw_spinlock_t		rt_runtime_lock;

	unsigned পূর্ণাঙ্ক		rt_nr_boosted;

	সংরচনা rq		*rq; /* this is always top-level rq, cache? */
#endif
#ifdef CONFIG_CGROUP_SCHED
	সংরচনা task_group	*tg; /* this tg has "this" rt_rq on given CPU for runnable entities */
#endif
};

স্থির অন্তঃস্থ bool rt_rq_is_runnable(সংরচনা rt_rq *rt_rq)
{
	প্রত্যাগম rt_rq->rt_queued && rt_rq->rt_nr_running;
}

/* Deadline class' related fields in a runqueue */
সংরচনা dl_rq {
	/* runqueue is an rbtree, ordered by deadline */
	সংরচনা rb_মূল_cached	মূল;

	unsigned পূর্ণাঙ্ক		dl_nr_running;

	/*
	 * Deadline values of the currently executing and the
	 * earliest ready task on this rq. Caching these facilitates
	 * the decision whether or not a ready but not running task
	 * should migrate somewhere else.
	 */
	সংরচনা {
		u64		curr;
		u64		next;
	} earliest_dl;

	bool			overloaded;

	সংরচনা sched_dl_entity	*curr;
	/*
	 * Tasks on this rq that can be pushed away. They are kept in
	 * an rb-tree, ordered by tasks' deadlines, with caching
	 * of the leftmost (earliest deadline) element.
	 */
	সংরচনা rb_মূল_cached	pushable_dl_tasks_মূল;

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
#define is_মূল_task_group(tg) ((tg) == &মূল_task_group)
/* An entity is a task if it doesn't "own" a runqueue */
#define entity_is_task(se)	(!se->my_q)

স্থির অন্তঃস্থ শূন্য se_update_runnable(সংরচনা sched_entity *se)
{
	যদি (!entity_is_task(se))
		se->runnable_weight = se->my_q->h_nr_runnable;
}

স্থির অন্তঃস্থ long se_runnable(সংরচনা sched_entity *se)
{
	যদি (se->sched_delayed)
		প্রত্যাগম false;

	যদি (entity_is_task(se))
		প্রত্যাগম !!se->on_rq;
	অন্যথা
		প্রত্যাগম se->runnable_weight;
}

#অন্যথা /* !CONFIG_FAIR_GROUP_SCHED: */

#define entity_is_task(se)	1

স্থির অন্তঃস্থ শূন্য se_update_runnable(সংরচনা sched_entity *se) { }

স্থির অন্তঃস্থ long se_runnable(সংরচনা sched_entity *se)
{
	যদি (se->sched_delayed)
		প্রত্যাগম false;

	প্রত্যাগম !!se->on_rq;
}

#endif /* !CONFIG_FAIR_GROUP_SCHED */

/*
 * XXX we want to get rid of these helpers and use the full load resolution.
 */
স্থির অন্তঃস্থ long se_weight(সংরচনা sched_entity *se)
{
	প্রত্যাগম scale_load_down(se->load.weight);
}


স্থির অন্তঃস্থ bool sched_asym_prefer(পূর্ণাঙ্ক a, পূর্ণাঙ্ক b)
{
	প্রত্যাগম arch_asym_cpu_priority(a) > arch_asym_cpu_priority(b);
}

সংরচনা perf_domain {
	সংরচনা em_perf_domain *em_pd;
	সংরচনা perf_domain *next;
	সংরচনা rcu_head rcu;
};

/*
 * We add the notion of a root-domain which will be used to define per-domain
 * variables. Each exclusive cpuset essentially defines an island domain by
 * fully partitioning the member CPUs from any other cpuset. Whenever a new
 * exclusive cpuset is created, we also create and attach a new root-domain
 * object.
 *
 */
সংরচনা মূল_domain {
	atomic_t		refcount;
	atomic_t		rto_গুনতি;
	সংরচনা rcu_head		rcu;
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
	atomic_t		dlo_গুনতি;
	সংরচনা dl_bw		dl_bw;
	সংরচনা cpudl		cpudl;

	/*
	 * Indicate whether a root_domain's dl_bw has been checked or
	 * updated. It's monotonously increasing value.
	 *
	 * Also, some corner cases, like 'wrap around' is dangerous, but given
	 * that u64 is 'big enough'. So that shouldn't be a concern.
	 */
	u64 visit_cookie;

#ifdef HAVE_RT_ঠেলো_IPI
	/*
	 * For IPI pull requests, loop across the rto_mask.
	 */
	সংরচনা irq_work		rto_ঠেলো_work;
	raw_spinlock_t		rto_lock;
	/* These are only updated and read within rto_lock */
	পূর্ণাঙ্ক			rto_loop;
	পূর্ণাঙ্ক			rto_cpu;
	/* These atomics are updated outside of a lock */
	atomic_t		rto_loop_next;
	atomic_t		rto_loop_start;
#endif /* HAVE_RT_PUSH_IPI */
	/*
	 * The "RT overload" flag: it gets set if a CPU has more than
	 * one runnable RT task.
	 */
	cpumask_var_t		rto_mask;
	সংরচনা cpupri		cpupri;

	/*
	 * NULL-terminated list of performance domains intersecting with the
	 * CPUs of the rd. Protected by RCU.
	 */
	সংরচনা perf_domain __rcu *pd;
};

বাহ্য শূন্য init_defrootdomain(শূন্য);
বাহ্য পূর্ণাঙ্ক sched_init_domains(ধ্রুব সংরচনা cpumask *cpu_map);
বাহ্য শূন্য rq_attach_মূল(সংরচনা rq *rq, সংরচনা মূল_domain *rd);
বাহ্য শূন্য sched_get_rd(সংরচনা মূল_domain *rd);
বাহ্য শূন্য sched_put_rd(সংরচনা মূল_domain *rd);

স্থির অন্তঃস্থ পূর্ণাঙ্ক get_rd_overloaded(সংরচনা মূল_domain *rd)
{
	প্রত্যাগম READ_ONCE(rd->overloaded);
}

স্থির অন্তঃস্থ শূন্য set_rd_overloaded(সংরচনা মূল_domain *rd, পূর্ণাঙ্ক status)
{
	যদি (get_rd_overloaded(rd) != status)
		WRITE_ONCE(rd->overloaded, status);
}

#ifdef HAVE_RT_ঠেলো_IPI
বাহ্য শূন্য rto_ঠেলো_irq_work_func(সংরচনা irq_work *work);
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
সংরচনা uclamp_bucket {
	unsigned long মান : bits_per(SCHED_CAPACITY_SCALE);
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
সংরচনা uclamp_rq {
	unsigned পূর্ণাঙ্ক মান;
	সংরচনা uclamp_bucket bucket[UCLAMP_BUCKETS];
};

DECLARE_STATIC_চাবি_FALSE(sched_uclamp_used);
#endif /* CONFIG_UCLAMP_TASK */

/*
 * This is the main, per-CPU runqueue data structure.
 *
 * Locking rule: those places that want to lock multiple runqueues
 * (such as the load balancing or the thread migration code), lock
 * acquire operations must be ordered by ascending &runqueue.
 */
সংরচনা rq {
	/*
	 * The following members are loaded together, without holding the
	 * rq->lock, in an extremely hot loop in update_sg_lb_stats()
	 * (called from pick_next_task()). To reduce cache pollution from
	 * this operation, they are placed together on this dedicated cache
	 * line. Even though some of them are frequently modified, they are
	 * loaded much more frequently than they are stored.
	 */
	unsigned পূর্ণাঙ্ক		nr_running;
#ifdef CONFIG_NUMA_BALANCING
	unsigned পূর্ণাঙ্ক		nr_numa_running;
	unsigned পূর্ণাঙ্ক		nr_preferred_running;
#endif
	unsigned পূর্ণাঙ্ক		ttwu_pending;
	unsigned long		cpu_capacity;
#ifdef CONFIG_SCHED_PROXY_EXEC
	সংরচনা task_struct __rcu	*donor;  /* Scheduling context */
	সংরচনা task_struct __rcu	*curr;   /* Execution context */
#অন্যথা
	সঙ্ঘ {
		সংরচনা task_struct __rcu *donor; /* Scheduler context */
		সংরচনা task_struct __rcu *curr;  /* Execution context */
	};
#endif
	সংরচনা task_struct	*idle;
	/* padding left here deliberately */

	/*
	 * The next cacheline holds the (hot) runqueue lock, as well as
	 * some other less performance-critical fields.
	 */
	u64			nr_switches	____cacheline_aligned;

	/* runqueue lock: */
	raw_spinlock_t		__lock;

#ifdef CONFIG_NO_HZ_COMMON
	unsigned পূর্ণাঙ্ক		nohz_tick_stopped;
	atomic_t		nohz_flags;
	unsigned পূর্ণাঙ্ক		has_blocked_load;
	unsigned long		last_blocked_load_update_tick;
	call_single_data_t	nohz_csd;
#endif /* CONFIG_NO_HZ_COMMON */

#ifdef CONFIG_UCLAMP_TASK
	/* Utilization clamp values based on CPU's RUNNABLE tasks */
	সংরচনা uclamp_rq	uclamp[UCLAMP_CNT] ____cacheline_aligned;
	unsigned পূর্ণাঙ্ক		uclamp_flags;
#define UCLAMP_FLAG_IDLE 0x01
#endif

	সংরচনা cfs_rq		cfs;
	সংরচনা rt_rq		rt;
	সংরচনা dl_rq		dl;
#ifdef CONFIG_SCHED_CLASS_EXT
	সংরচনা scx_rq		scx;
	সংরচনা sched_dl_entity	ext_server;
#endif
#ifdef CONFIG_SCHED_CACHE
	raw_spinlock_t		cpu_epoch_lock ____cacheline_aligned;
	u64			cpu_runtime;
	unsigned long		cpu_epoch;
	unsigned long		cpu_epoch_next;
#endif

	সংরচনা sched_dl_entity	fair_server;

#ifdef CONFIG_FAIR_GROUP_SCHED
	/* list of leaf cfs_rq on this CPU: */
	সংরচনা list_head	leaf_cfs_rq_list;
	সংরচনা list_head	*tmp_alone_branch;
#endif /* CONFIG_FAIR_GROUP_SCHED */

#ifdef CONFIG_NUMA_BALANCING
	unsigned পূর্ণাঙ্ক		numa_migrate_on;
#endif

#ifdef CONFIG_SCHED_CACHE
	unsigned পূর্ণাঙ্ক		nr_pref_llc_running;
	unsigned পূর্ণাঙ্ক		nr_llc_running;
#endif

	/*
	 * This is part of a global counter where only the total sum
	 * over all CPUs matters. A task can increase this counter on
	 * one CPU and if it got migrated afterwards it may decrease
	 * it on another CPU. Always updated under the runqueue lock:
	 */
	unsigned long		nr_uninterruptible;

	সংরচনা sched_dl_entity	*dl_server;
	সংরচনা task_struct	*stop;
	ধ্রুব সংরচনা sched_class *next_class;
	unsigned long		next_balance;
	সংরচনা mm_struct	*prev_mm;

	/*
	 * The following fields of clock data are frequently referenced
	 * and updated together, and should go on their own cache line.
	 */
	u64			clock_task ____cacheline_aligned;
	u64			clock_pelt;
	u64			clock;
	unsigned long		lost_idle_time;
	unsigned পূর্ণাঙ্ক		clock_update_flags;
	u64			clock_pelt_idle;
	u64			clock_idle;

#ifndef CONFIG_64BIT
	u64			clock_pelt_idle_copy;
	u64			clock_idle_copy;
#endif

	u64 last_seen_need_resched_ns;
	পূর্ণাঙ্ক ticks_without_resched;

#ifdef CONFIG_MEMBARRIER
	পূর্ণাঙ্ক membarrier_state;
#endif

	সংরচনা মূল_domain		*rd;
	সংরচনা sched_domain __rcu	*sd;

	সংরচনা balance_callback *balance_callback;

	unsigned বর্ণ		nohz_idle_balance;
	unsigned বর্ণ		idle_balance;

	unsigned long		misfit_task_load;

	/* For active balancing */
	পূর্ণাঙ্ক			active_balance;
	পূর্ণাঙ্ক			ঠেলো_cpu;
	সংরচনা cpu_stop_work	active_balance_work;

	/* CPU of this runqueue: */
	পূর্ণাঙ্ক			cpu;
	পূর্ণাঙ্ক			online;

	সংরচনা list_head cfs_tasks;

	সংরচনা sched_avg	avg_rt;
	সংরচনা sched_avg	avg_dl;
#ifdef CONFIG_HAVE_SCHED_AVG_IRQ
	সংরচনা sched_avg	avg_irq;
#endif
#ifdef CONFIG_SCHED_HW_PRESSURE
	সংরচনা sched_avg	avg_hw;
#endif
	u64			idle_stamp;
	u64			avg_idle;

	/* This is used to determine avg_idle's max value */
	u64			max_idle_balance_cost;

#ifdef CONFIG_HOTPLUG_CPU
	সংরচনা rcuwait		hotplug_wait;
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
	bool			npc_ঠেলো_work_pending;
#endif

	/* calc_load related fields */
	unsigned long		calc_load_update;
	long			calc_load_active;

#ifdef CONFIG_SCHED_HRTICK
	call_single_data_t	hrtick_csd;
	সংরচনা hrtimer		hrtick_timer;
	ktime_t			hrtick_time;
	ktime_t			hrtick_delay;
	unsigned পূর্ণাঙ্ক		hrtick_sched;
#endif

#ifdef CONFIG_SCHEDSTATS
	/* latency stats */
	সংরচনা sched_info	rq_sched_info;
	unsigned long long	rq_cpu_time;

	/* sys_sched_yield() stats */
	unsigned পূর্ণাঙ্ক		yld_গুনতি;

	/* schedule() stats */
	unsigned পূর্ণাঙ্ক		sched_গুনতি;
	unsigned পূর্ণাঙ্ক		sched_goidle;

	/* try_to_wake_up() stats */
	unsigned পূর্ণাঙ্ক		ttwu_গুনতি;
	unsigned পূর্ণাঙ্ক		ttwu_local;
#endif

#ifdef CONFIG_CPU_IDLE
	/* Must be inspected within a RCU lock section */
	সংরচনা cpuidle_state	*idle_state;
#endif

	unsigned পূর্ণাঙ্ক		nr_pinned;
	unsigned পূর্ণাঙ্ক		ঠেলো_busy;
	সংরচনা cpu_stop_work	ঠেলো_work;

#ifdef CONFIG_SCHED_CORE
	/* per rq */
	সংরচনা rq		*core;
	সংরচনা task_struct	*core_pick;
	সংরচনা sched_dl_entity	*core_dl_server;
	unsigned পূর্ণাঙ্ক		core_enabled;
	সংরচনা rb_মূল		core_বৃক্ষ;

	/* shared state -- careful with sched_core_cpu_deactivate() */
	unsigned পূর্ণাঙ্ক		core_task_seq;
	unsigned পূর্ণাঙ্ক		core_pick_seq;
	unsigned long		core_cookie;
	unsigned পূর্ণাঙ্ক		core_forceidle_গুনতি;
	unsigned পূর্ণাঙ্ক		core_forceidle_seq;
	unsigned পূর্ণাঙ্ক		core_forceidle_occupation;
	u64			core_forceidle_start;
	unsigned পূর্ণাঙ্ক		core_pick_in_flight;
#endif /* CONFIG_SCHED_CORE */

	/* Scratch cpumask to be temporarily used under rq_lock */
	cpumask_var_t		scratch_mask;

#ifdef CONFIG_CFS_BANDWIDTH
	call_single_data_t	cfsb_csd;
	সংরচনা list_head	cfsb_csd_list;
#endif

	atomic_t		nr_iowait;
} __no_randomize_layout;

#ifdef CONFIG_FAIR_GROUP_SCHED

/* CPU runqueue to which this cfs_rq is attached */
স্থির অন্তঃস্থ সংরচনা rq *rq_of(সংরচনা cfs_rq *cfs_rq)
{
	প্রত্যাগম cfs_rq->rq;
}

#অন্যথা /* !CONFIG_FAIR_GROUP_SCHED: */

স্থির অন্তঃস্থ সংরচনা rq *rq_of(সংরচনা cfs_rq *cfs_rq)
{
	প্রত্যাগম container_of(cfs_rq, সংরচনা rq, cfs);
}
#endif /* !CONFIG_FAIR_GROUP_SCHED */

স্থির অন্তঃস্থ পূর্ণাঙ্ক cpu_of(সংরচনা rq *rq)
{
	প্রত্যাগম rq->cpu;
}

#define MDF_ঠেলো		0x01

স্থির অন্তঃস্থ bool is_migration_disabled(সংরচনা task_struct *p)
{
	প্রত্যাগম p->migration_disabled;
}

DECLARE_PER_CPU_SHARED_ALIGNED(সংরচনা rq, runqueues);
DECLARE_PER_CPU(সংরচনা rnd_state, sched_rnd_state);

স্থির অন্তঃস্থ u32 sched_rng(শূন্য)
{
	প্রত্যাগম prandom_u32_state(this_cpu_ptr(&sched_rnd_state));
}

স্থির __always_inline সংরচনা rq *__this_rq(শূন্য)
{
	প্রত্যাগম this_cpu_ptr(&runqueues);
}

#define cpu_rq(cpu)		(&per_cpu(runqueues, (cpu)))
#define this_rq()		__this_rq()
#define task_rq(p)		cpu_rq(task_cpu(p))
#define cpu_curr(cpu)		(cpu_rq(cpu)->curr)
#define raw_rq()		raw_cpu_ptr(&runqueues)

স্থির অন্তঃস্থ bool idle_rq(সংরচনা rq *rq)
{
	প্রত্যাগম rq->curr == rq->idle && !rq->nr_running && !rq->ttwu_pending;
}

/**
 * available_idle_cpu - is a given CPU idle for enqueuing work.
 * @cpu: the CPU in question.
 *
 * Return: 1 if the CPU is currently idle. 0 otherwise.
 */
স্থির অন্তঃস্থ bool available_idle_cpu(পূর্ণাঙ্ক cpu)
{
	যদি (!idle_rq(cpu_rq(cpu)))
		প্রত্যাগম 0;

	যদি (vcpu_is_preempted(cpu))
		প্রত্যাগম 0;

	প্রত্যাগম 1;
}

#ifdef CONFIG_SCHED_PROXY_EXEC
স্থির অন্তঃস্থ শূন্য rq_set_donor(সংরচনা rq *rq, সংরচনা task_struct *t)
{
	rcu_assign_pointer(rq->donor, t);
}
#অন্যথা
স্থির অন্তঃস্থ শূন্য rq_set_donor(সংরচনা rq *rq, সংরচনা task_struct *t)
{
	/* Do nothing */
}
#endif

#ifdef CONFIG_SCHED_CORE
স্থির অন্তঃস্থ সংরচনা cpumask *sched_group_span(সংরচনা sched_group *sg);

DECLARE_STATIC_চাবি_FALSE(__sched_core_enabled);

স্থির অন্তঃস্থ bool sched_core_enabled(সংরচনা rq *rq)
{
	প্রত্যাগম static_branch_unlikely(&__sched_core_enabled) && rq->core_enabled;
}

স্থির অন্তঃস্থ bool sched_core_disabled(শূন্য)
{
	প্রত্যাগম !static_branch_unlikely(&__sched_core_enabled);
}

/*
 * Be careful with this function; not for general use. The return value isn't
 * stable unless you actually hold a relevant rq->__lock.
 */
স্থির অন্তঃস্থ raw_spinlock_t *rq_lockp(সংরচনা rq *rq)
{
	যদি (sched_core_enabled(rq))
		প্রত্যাগম &rq->core->__lock;

	প্রত্যাগম &rq->__lock;
}

স্থির অন্তঃস্থ raw_spinlock_t *__rq_lockp(সংরচনা rq *rq)
	__returns_ctx_lock(rq_lockp(rq)) /* alias them */
{
	যদি (rq->core_enabled)
		প্রত্যাগম &rq->core->__lock;

	প্রত্যাগম &rq->__lock;
}

বাহ্য bool
cfs_prio_less(ধ্রুব সংরচনা task_struct *a, ধ্রুব সংরচনা task_struct *b, bool fi);

বাহ্য শূন্য task_vruntime_update(সংরচনা rq *rq, সংরচনা task_struct *p, bool in_fi);

/*
 * Helpers to check if the CPU's core cookie matches with the task's cookie
 * when core scheduling is enabled.
 * A special case is that the task's cookie always matches with CPU's core
 * cookie if the CPU is in an idle core.
 */
স্থির অন্তঃস্থ bool sched_cpu_cookie_match(সংরচনা rq *rq, সংরচনা task_struct *p)
{
	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	যদি (!sched_core_enabled(rq))
		প্রত্যাগম true;

	প্রত্যাগম rq->core->core_cookie == p->core_cookie;
}

স্থির অন্তঃস্থ bool sched_core_cookie_match(সংরচনা rq *rq, সংরচনা task_struct *p)
{
	bool idle_core = true;
	পূর্ণাঙ্ক cpu;

	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	যদি (!sched_core_enabled(rq))
		প্রত্যাগম true;

	যদি (rq->core->core_cookie == p->core_cookie)
		প্রত্যাগম true;

	for_each_cpu(cpu, cpu_smt_mask(cpu_of(rq))) {
		যদি (!available_idle_cpu(cpu)) {
			idle_core = false;
			ভঙ্গ;
		}
	}

	/*
	 * A CPU in an idle core is always the best choice for tasks with
	 * cookies.
	 */
	প্রত্যাগম idle_core;
}

স্থির অন্তঃস্থ bool sched_group_cookie_match(সংরচনা rq *rq,
					    সংরচনা task_struct *p,
					    সংরচনা sched_group *group)
{
	পূর্ণাঙ্ক cpu;

	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	যদি (!sched_core_enabled(rq))
		প্রত্যাগম true;

	for_each_cpu_and(cpu, sched_group_span(group), p->cpus_ptr) {
		যদি (sched_core_cookie_match(cpu_rq(cpu), p))
			প্রত্যাগম true;
	}
	প্রত্যাগম false;
}

স্থির অন্তঃস্থ bool sched_core_enqueued(সংরচনা task_struct *p)
{
	প্রত্যাগম !RB_EMPTY_গ্রন্থি(&p->core_গ্রন্থি);
}

বাহ্য শূন্য sched_core_enqueue(সংরচনা rq *rq, সংরচনা task_struct *p);
বাহ্য শূন্য sched_core_dequeue(সংরচনা rq *rq, সংরচনা task_struct *p, পূর্ণাঙ্ক flags);

বাহ্য শূন্য sched_core_get(শূন্য);
বাহ্য শূন্য sched_core_put(শূন্য);

স্থির অন্তঃস্থ bool task_has_sched_core(সংরচনা task_struct *p)
{
	যদি (sched_core_disabled())
		প্রত্যাগম false;

	প্রত্যাগম !!p->core_cookie;
}

#অন্যথা /* !CONFIG_SCHED_CORE: */

স্থির অন্তঃস্থ bool sched_core_enabled(সংরচনা rq *rq)
{
	প্রত্যাগম false;
}

স্থির অন্তঃস্থ bool sched_core_disabled(শূন্য)
{
	প্রত্যাগম true;
}

স্থির অন্তঃস্থ raw_spinlock_t *rq_lockp(সংরচনা rq *rq)
{
	প্রত্যাগম &rq->__lock;
}

স্থির অন্তঃস্থ raw_spinlock_t *__rq_lockp(সংরচনা rq *rq)
	__returns_ctx_lock(rq_lockp(rq)) /* alias them */
{
	প্রত্যাগম &rq->__lock;
}

স্থির অন্তঃস্থ bool sched_cpu_cookie_match(সংরচনা rq *rq, সংরচনা task_struct *p)
{
	প্রত্যাগম true;
}

স্থির অন্তঃস্থ bool sched_core_cookie_match(সংরচনা rq *rq, সংরচনা task_struct *p)
{
	প্রত্যাগম true;
}

স্থির অন্তঃস্থ bool sched_group_cookie_match(সংরচনা rq *rq,
					    সংরচনা task_struct *p,
					    সংরচনা sched_group *group)
{
	প্রত্যাগম true;
}

স্থির অন্তঃস্থ bool task_has_sched_core(সংরচনা task_struct *p)
{
	প্রত্যাগম false;
}

#endif /* !CONFIG_SCHED_CORE */

#ifdef CONFIG_RT_GROUP_SCHED
# ifdef CONFIG_RT_GROUP_SCHED_DEFAULT_DISABLED
DECLARE_STATIC_চাবি_FALSE(rt_group_sched);
স্থির অন্তঃস্থ bool rt_group_sched_enabled(শূন্য)
{
	প্রত্যাগম static_branch_unlikely(&rt_group_sched);
}
# অন্যথা /* !CONFIG_RT_GROUP_SCHED_DEFAULT_DISABLED: */
DECLARE_STATIC_চাবি_TRUE(rt_group_sched);
স্থির অন্তঃস্থ bool rt_group_sched_enabled(শূন্য)
{
	প্রত্যাগম static_branch_likely(&rt_group_sched);
}
# endif /* !CONFIG_RT_GROUP_SCHED_DEFAULT_DISABLED */
#অন্যথা /* !CONFIG_RT_GROUP_SCHED: */
# define rt_group_sched_enabled()	false
#endif /* !CONFIG_RT_GROUP_SCHED */

স্থির অন্তঃস্থ শূন্য lockdep_assert_rq_held(সংরচনা rq *rq)
	__assumes_ctx_lock(__rq_lockp(rq))
{
	lockdep_assert_held(__rq_lockp(rq));
}

বাহ্য শূন্য raw_spin_rq_lock_nested(সংরচনা rq *rq, পূর্ণাঙ্ক subclass)
	__acquires(__rq_lockp(rq));

বাহ্য bool raw_spin_rq_trylock(সংরচনা rq *rq)
	__cond_acquires(true, __rq_lockp(rq));

স্থির অন্তঃস্থ শূন্য raw_spin_rq_lock(সংরচনা rq *rq)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_lock_nested(rq, 0);
}

স্থির অন্তঃস্থ শূন্য raw_spin_rq_unlock(সংরচনা rq *rq)
	__releases(__rq_lockp(rq))
{
	raw_spin_unlock(rq_lockp(rq));
}

স্থির অন্তঃস্থ শূন্য raw_spin_rq_lock_irq(সংরচনা rq *rq)
	__acquires(__rq_lockp(rq))
{
	local_irq_disable();
	raw_spin_rq_lock(rq);
}

স্থির অন্তঃস্থ শূন্য raw_spin_rq_unlock_irq(সংরচনা rq *rq)
	__releases(__rq_lockp(rq))
{
	raw_spin_rq_unlock(rq);
	local_irq_enable();
}

স্থির অন্তঃস্থ unsigned long _raw_spin_rq_lock_irqsave(সংরচনা rq *rq)
	__acquires(__rq_lockp(rq))
{
	unsigned long flags;

	local_irq_save(flags);
	raw_spin_rq_lock(rq);

	প্রত্যাগম flags;
}

স্থির অন্তঃস্থ শূন্য raw_spin_rq_unlock_irqrestore(সংরচনা rq *rq, unsigned long flags)
	__releases(__rq_lockp(rq))
{
	raw_spin_rq_unlock(rq);
	local_irq_restore(flags);
}

#define raw_spin_rq_lock_irqsave(rq, flags)	\
কুরু {						\
	flags = _raw_spin_rq_lock_irqsave(rq);	\
} যাবত্ (0)

বাহ্য শূন্য __update_idle_core(সংরচনা rq *rq);

স্থির অন্তঃস্থ শূন্য update_idle_core(সংরচনা rq *rq)
{
	যদি (sched_smt_active())
		__update_idle_core(rq);
}

#ifdef CONFIG_FAIR_GROUP_SCHED
স্থির অন্তঃস্থ সংরচনা task_struct *task_of(সংরচনা sched_entity *se)
{
	WARN_ON_ONCE(!entity_is_task(se));
	প্রত্যাগম container_of(se, সংরচনা task_struct, se);
}

স্থির অন্তঃস্থ সংরচনা cfs_rq *task_cfs_rq(সংরচনা task_struct *p)
{
	প্রত্যাগম p->se.cfs_rq;
}

/* runqueue on which this entity is (to be) queued */
স্থির অন্তঃস্থ সংরচনা cfs_rq *cfs_rq_of(ধ্রুব সংরচনা sched_entity *se)
{
	প্রত্যাগম se->cfs_rq;
}

/* runqueue "owned" by this group */
স্থির অন্তঃস্থ সংরচনা cfs_rq *group_cfs_rq(সংরচনা sched_entity *grp)
{
	প্রত্যাগম grp->my_q;
}

#অন্যথা /* !CONFIG_FAIR_GROUP_SCHED: */

#define task_of(_se)		container_of(_se, সংরচনা task_struct, se)

স্থির অন্তঃস্থ সংরচনা cfs_rq *task_cfs_rq(ধ্রুব সংরচনা task_struct *p)
{
	প্রত্যাগম &task_rq(p)->cfs;
}

স্থির অন্তঃস্থ সংরচনা cfs_rq *cfs_rq_of(ধ্রুব সংরচনা sched_entity *se)
{
	ধ্রুব সংরচনা task_struct *p = task_of(se);
	সংরচনা rq *rq = task_rq(p);

	প্রত্যাগম &rq->cfs;
}

/* runqueue "owned" by this group */
স্থির অন্তঃস্থ সংরচনা cfs_rq *group_cfs_rq(সংরচনা sched_entity *grp)
{
	প্রত্যাগম রিক্ত;
}

#endif /* !CONFIG_FAIR_GROUP_SCHED */

বাহ্য শূন্য update_rq_avg_idle(সংরচনা rq *rq);
বাহ্য শূন্য update_rq_clock(সংরচনা rq *rq);

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

স্থির অন্তঃস্থ শূন্য assert_clock_updated(সংরচনা rq *rq)
{
	/*
	 * The only reason for not seeing a clock update since the
	 * last rq_pin_lock() is if we're currently skipping updates.
	 */
	WARN_ON_ONCE(rq->clock_update_flags < RQCF_ACT_SKIP);
}

স্থির অন্তঃস্থ u64 rq_clock(সংরচনা rq *rq)
{
	lockdep_assert_rq_held(rq);
	assert_clock_updated(rq);

	প্রত্যাগম rq->clock;
}

স্থির অন্তঃস্থ u64 rq_clock_task(সংরচনা rq *rq)
{
	lockdep_assert_rq_held(rq);
	assert_clock_updated(rq);

	প্রত্যাগম rq->clock_task;
}

স্থির অন্তঃস্থ শূন্য rq_clock_skip_update(সংরচনা rq *rq)
{
	lockdep_assert_rq_held(rq);
	rq->clock_update_flags |= RQCF_REQ_SKIP;
}

/*
 * See rt task throttling, which is the only time a skip
 * request is canceled.
 */
স্থির অন্তঃস্থ শূন্য rq_clock_cancel_skipupdate(সংরচনা rq *rq)
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
স্থির অন্তঃস্থ শূন্য rq_clock_start_loop_update(সংরচনা rq *rq)
{
	lockdep_assert_rq_held(rq);
	WARN_ON_ONCE(rq->clock_update_flags & RQCF_ACT_SKIP);
	rq->clock_update_flags |= RQCF_ACT_SKIP;
}

স্থির অন্তঃস্থ শূন্য rq_clock_stop_loop_update(সংরচনা rq *rq)
{
	lockdep_assert_rq_held(rq);
	rq->clock_update_flags &= ~RQCF_ACT_SKIP;
}

সংরচনা rq_flags {
	unsigned long flags;
	সংরচনা pin_cookie cookie;
	/*
	 * A copy of (rq::clock_update_flags & RQCF_UPDATED) for the
	 * current pin context is stashed here in case it needs to be
	 * restored in rq_repin_lock().
	 */
	unsigned পূর্ণাঙ্ক clock_update_flags;
};

বাহ্য সংরচনা balance_callback balance_ঠেলো_callback;

#ifdef CONFIG_SCHED_CLASS_EXT
বাহ্য ধ্রুব সংরচনা sched_class ext_sched_class;

DECLARE_STATIC_চাবি_FALSE(__scx_enabled);	/* SCX BPF scheduler loaded */
DECLARE_STATIC_চাবি_FALSE(__scx_switched_all);	/* all fair class tasks on SCX */

#define scx_enabled()		static_branch_unlikely(&__scx_enabled)
#define scx_switched_all()	static_branch_unlikely(&__scx_switched_all)

স্থির অন্তঃস্থ শূন্য scx_rq_clock_update(সংরচনা rq *rq, u64 clock)
{
	যদি (!scx_enabled())
		প্রত্যাগম;
	WRITE_ONCE(rq->scx.clock, clock);
	smp_store_release(&rq->scx.flags, rq->scx.flags | SCX_RQ_CLK_VALID);
}

স্থির অন্তঃস্থ শূন্য scx_rq_clock_invalidate(সংরচনা rq *rq)
{
	যদি (!scx_enabled())
		প্রত্যাগম;
	WRITE_ONCE(rq->scx.flags, rq->scx.flags & ~SCX_RQ_CLK_VALID);
}

#অন্যথা /* !CONFIG_SCHED_CLASS_EXT: */
#define scx_enabled()		false
#define scx_switched_all()	false

স্থির অন্তঃস্থ শূন্য scx_rq_clock_update(সংরচনা rq *rq, u64 clock) {}
স্থির অন্তঃস্থ শূন্য scx_rq_clock_invalidate(সংরচনা rq *rq) {}
#endif /* !CONFIG_SCHED_CLASS_EXT */

স্থির অন্তঃস্থ শূন্য assert_balance_callbacks_empty(সংরচনা rq *rq)
{
	WARN_ON_ONCE(IS_ENABLED(CONFIG_PROVE_LOCKING) &&
		     rq->balance_callback &&
		     rq->balance_callback != &balance_ঠেলো_callback);
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
স্থির অন্তঃস্থ শূন্য rq_pin_lock(সংরচনা rq *rq, সংরচনা rq_flags *rf)
{
	rf->cookie = lockdep_pin_lock(__rq_lockp(rq));

	rq->clock_update_flags &= (RQCF_REQ_SKIP|RQCF_ACT_SKIP);
	rf->clock_update_flags = 0;
	assert_balance_callbacks_empty(rq);
}

স্থির অন্তঃস্থ শূন্য rq_unpin_lock(সংরচনা rq *rq, সংরচনা rq_flags *rf)
{
	যদি (rq->clock_update_flags > RQCF_ACT_SKIP)
		rf->clock_update_flags = RQCF_UPDATED;

	scx_rq_clock_invalidate(rq);
	lockdep_unpin_lock(__rq_lockp(rq), rf->cookie);
}

স্থির অন্তঃস্থ শূন্য rq_repin_lock(সংরচনা rq *rq, সংরচনা rq_flags *rf)
{
	lockdep_repin_lock(__rq_lockp(rq), rf->cookie);

	/*
	 * Restore the value we stashed in @rf for this pin context.
	 */
	rq->clock_update_flags |= rf->clock_update_flags;
}

#define __task_rq_lock(...) __acquire_ret(___task_rq_lock(__VA_ARGS__), __rq_lockp(__ret))
বাহ্য সংরচনা rq *___task_rq_lock(সংরচনা task_struct *p, সংরচনা rq_flags *rf) __acquires_ret;

#define task_rq_lock(...) __acquire_ret(_task_rq_lock(__VA_ARGS__), __rq_lockp(__ret))
বাহ্য সংরচনা rq *_task_rq_lock(সংরচনা task_struct *p, সংরচনা rq_flags *rf)
	__acquires(&p->pi_lock) __acquires_ret;

স্থির অন্তঃস্থ শূন্য
__task_rq_unlock(সংরচনা rq *rq, সংরচনা task_struct *p, সংরচনা rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_lock(rq, rf);
	raw_spin_rq_unlock(rq);
}

স্থির অন্তঃস্থ শূন্য
task_rq_unlock(সংরচনা rq *rq, সংরচনা task_struct *p, সংরচনা rq_flags *rf)
	__releases(__rq_lockp(rq), &p->pi_lock)
{
	__task_rq_unlock(rq, p, rf);
	raw_spin_unlock_irqrestore(&p->pi_lock, rf->flags);
}

DEFINE_LOCK_GUARD_1(task_rq_lock, সংরচনা task_struct,
		    _T->rq = task_rq_lock(_T->lock, &_T->rf),
		    task_rq_unlock(_T->rq, _T->lock, &_T->rf),
		    সংরচনা rq *rq; সংরচনা rq_flags rf)
DECLARE_LOCK_GUARD_1_ATTRS(task_rq_lock, __acquires(_T->pi_lock), __releases((*(সংরচনা task_struct **)_T)->pi_lock))
#define class_task_rq_lock_constructor(_T) WITH_LOCK_GUARD_1_ATTRS(task_rq_lock, _T)

DEFINE_LOCK_GUARD_1(__task_rq_lock, সংরচনা task_struct,
		    _T->rq = __task_rq_lock(_T->lock, &_T->rf),
		    __task_rq_unlock(_T->rq, _T->lock, &_T->rf),
		    সংরচনা rq *rq; সংরচনা rq_flags rf)

স্থির অন্তঃস্থ শূন্য rq_lock_irqsave(সংরচনা rq *rq, সংরচনা rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_lock_irqsave(rq, rf->flags);
	rq_pin_lock(rq, rf);
}

স্থির অন্তঃস্থ শূন্য rq_lock_irq(সংরচনা rq *rq, সংরচনা rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_lock_irq(rq);
	rq_pin_lock(rq, rf);
}

স্থির অন্তঃস্থ শূন্য rq_lock(সংরচনা rq *rq, সংরচনা rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_lock(rq);
	rq_pin_lock(rq, rf);
}

স্থির অন্তঃস্থ শূন্য rq_unlock_irqrestore(সংরচনা rq *rq, সংরচনা rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_lock(rq, rf);
	raw_spin_rq_unlock_irqrestore(rq, rf->flags);
}

স্থির অন্তঃস্থ শূন্য rq_unlock_irq(সংরচনা rq *rq, সংরচনা rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_lock(rq, rf);
	raw_spin_rq_unlock_irq(rq);
}

স্থির অন্তঃস্থ শূন্য rq_unlock(সংরচনা rq *rq, সংরচনা rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_lock(rq, rf);
	raw_spin_rq_unlock(rq);
}

DEFINE_LOCK_GUARD_1(rq_lock, সংরচনা rq,
		    rq_lock(_T->lock, &_T->rf),
		    rq_unlock(_T->lock, &_T->rf),
		    সংরচনা rq_flags rf)

DECLARE_LOCK_GUARD_1_ATTRS(rq_lock, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(সংরচনা rq **)_T)));
#define class_rq_lock_constructor(_T) WITH_LOCK_GUARD_1_ATTRS(rq_lock, _T)

DEFINE_LOCK_GUARD_1(rq_lock_irq, সংরচনা rq,
		    rq_lock_irq(_T->lock, &_T->rf),
		    rq_unlock_irq(_T->lock, &_T->rf),
		    সংরচনা rq_flags rf)

DECLARE_LOCK_GUARD_1_ATTRS(rq_lock_irq, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(সংরচনা rq **)_T)));
#define class_rq_lock_irq_constructor(_T) WITH_LOCK_GUARD_1_ATTRS(rq_lock_irq, _T)

DEFINE_LOCK_GUARD_1(rq_lock_irqsave, সংরচনা rq,
		    rq_lock_irqsave(_T->lock, &_T->rf),
		    rq_unlock_irqrestore(_T->lock, &_T->rf),
		    সংরচনা rq_flags rf)

DECLARE_LOCK_GUARD_1_ATTRS(rq_lock_irqsave, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(সংরচনা rq **)_T)));
#define class_rq_lock_irqsave_constructor(_T) WITH_LOCK_GUARD_1_ATTRS(rq_lock_irqsave, _T)

DEFINE_LOCK_GUARD_1(raw_spin_rq_lock_irqsave, সংরচনা rq,
		    raw_spin_rq_lock_irqsave(_T->lock, _T->flags),
		    raw_spin_rq_unlock_irqrestore(_T->lock, _T->flags),
		    unsigned long flags)

DECLARE_LOCK_GUARD_1_ATTRS(raw_spin_rq_lock_irqsave, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(সংরচনা rq **)_T)));
#define class_raw_spin_rq_lock_irqsave_constructor(_T) \
	WITH_LOCK_GUARD_1_ATTRS(raw_spin_rq_lock_irqsave, _T)

#define this_rq_lock_irq(...) __acquire_ret(_this_rq_lock_irq(__VA_ARGS__), __rq_lockp(__ret))
স্থির অন্তঃস্থ সংরচনা rq *_this_rq_lock_irq(সংরচনা rq_flags *rf) __acquires_ret
{
	সংরচনা rq *rq;

	local_irq_disable();
	rq = this_rq();
	rq_lock(rq, rf);

	প্রত্যাগম rq;
}

#ifdef CONFIG_NUMA

গণনা numa_topology_type {
	NUMA_DIRECT,
	NUMA_GLUELESS_MESH,
	NUMA_BACKPLANE,
};

বাহ্য গণনা numa_topology_type sched_numa_topology_type;
বাহ্য পূর্ণাঙ্ক sched_max_numa_distance;
বাহ্য bool find_numa_distance(পূর্ণাঙ্ক distance);
বাহ্য শূন্য sched_init_numa(পূর্ণাঙ্ক offline_গ্রন্থি);
বাহ্য শূন্য sched_update_numa(পূর্ণাঙ্ক cpu, bool online);
বাহ্য শূন্য sched_domains_numa_masks_set(unsigned পূর্ণাঙ্ক cpu);
বাহ্য শূন্য sched_domains_numa_masks_clear(unsigned পূর্ণাঙ্ক cpu);
বাহ্য পূর্ণাঙ্ক sched_numa_find_closest(ধ্রুব সংরচনা cpumask *cpus, পূর্ণাঙ্ক cpu);

#অন্যথা /* !CONFIG_NUMA: */

স্থির অন্তঃস্থ শূন্য sched_init_numa(পূর্ণাঙ্ক offline_গ্রন্থি) { }
স্থির অন্তঃস্থ শূন্য sched_update_numa(পূর্ণাঙ্ক cpu, bool online) { }
স্থির অন্তঃস্থ শূন্য sched_domains_numa_masks_set(unsigned পূর্ণাঙ্ক cpu) { }
স্থির অন্তঃস্থ শূন্য sched_domains_numa_masks_clear(unsigned পূর্ণাঙ্ক cpu) { }

স্থির অন্তঃস্থ পূর্ণাঙ্ক sched_numa_find_closest(ধ্রুব সংরচনা cpumask *cpus, পূর্ণাঙ্ক cpu)
{
	প্রত্যাগম nr_cpu_ids;
}

#endif /* !CONFIG_NUMA */

#ifdef CONFIG_NUMA_BALANCING

/* The regions in numa_faults array from task_struct */
গণনা numa_faults_stats {
	NUMA_MEM = 0,
	NUMA_CPU,
	NUMA_MEMBUF,
	NUMA_CPUBUF
};

বাহ্য শূন্য sched_setnuma(সংরচনা task_struct *p, পূর্ণাঙ্ক গ্রন্থি);
বাহ্য পূর্ণাঙ্ক migrate_task_to(সংরচনা task_struct *p, পূর্ণাঙ্ক cpu);
বাহ্য পূর্ণাঙ্ক migrate_swap(সংরচনা task_struct *p, সংরচনা task_struct *t,
			পূর্ণাঙ্ক cpu, পূর্ণাঙ্ক scpu);
বাহ্য শূন্য init_numa_balancing(u64 clone_flags, সংরচনা task_struct *p);

#অন্যথা /* !CONFIG_NUMA_BALANCING: */

স্থির অন্তঃস্থ শূন্য
init_numa_balancing(u64 clone_flags, সংরচনা task_struct *p)
{
}

#endif /* !CONFIG_NUMA_BALANCING */

পূর্ণাঙ্ক task_llc(ধ্রুব সংরচনা task_struct *p);

স্থির অন্তঃস্থ শূন্য
queue_balance_callback(সংরচনা rq *rq,
		       সংরচনা balance_callback *head,
		       শূন্য (*func)(সংরচনা rq *rq))
{
	lockdep_assert_rq_held(rq);

	/*
	 * Don't (re)queue an already queued item; nor queue anything when
	 * balance_push() is active, see the comment with
	 * balance_push_callback.
	 */
	যদি (unlikely(head->next || rq->balance_callback == &balance_ঠেলো_callback))
		প্রত্যাগম;

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
	প্রতি (__sd = rcu_dereference_sched_domain(cpu_rq(cpu)->sd); \
			__sd; __sd = __sd->parent)

/* A mask of all the SD flags that have the SDF_SHARED_CHILD metaflag */
#define SD_FLAG(name, mflags) (name * !!((mflags) & SDF_SHARED_CHILD)) |
স্থির ধ্রুব unsigned পূর্ণাঙ্ক SD_SHARED_CHILD_MASK =
#অন্তর্ভাব <linux/sched/sd_flags.h>
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
স্থির অন্তঃস্থ সংরচনা sched_domain *highest_flag_domain(পূর্ণাঙ্ক cpu, পূর্ণাঙ্ক flag)
{
	সংরচনা sched_domain *sd, *hsd = রিক্ত;

	for_each_domain(cpu, sd) {
		যদি (sd->flags & flag) {
			hsd = sd;
			অনুবর্ত;
		}

		/*
		 * Stop the search if @flag is known to be shared at lower
		 * levels. It will not be found further up.
		 */
		যদি (flag & SD_SHARED_CHILD_MASK)
			ভঙ্গ;
	}

	প্রত্যাগম hsd;
}

স্থির অন্তঃস্থ সংরচনা sched_domain *lowest_flag_domain(পূর্ণাঙ্ক cpu, পূর্ণাঙ্ক flag)
{
	সংরচনা sched_domain *sd;

	for_each_domain(cpu, sd) {
		যদি (sd->flags & flag)
			ভঙ্গ;
	}

	প্রত্যাগম sd;
}

DECLARE_PER_CPU(সংরচনা sched_domain __rcu *, sd_llc);
DECLARE_PER_CPU(পূর্ণাঙ্ক, sd_llc_size);
DECLARE_PER_CPU(পূর্ণাঙ্ক, sd_llc_id);
DECLARE_PER_CPU(পূর্ণাঙ্ক, sd_share_id);
DECLARE_PER_CPU(সংরচনা sched_domain_shared __rcu *, sd_llc_shared);
DECLARE_PER_CPU(সংরচনা sched_domain_shared __rcu *, sd_balance_shared);
DECLARE_PER_CPU(সংরচনা sched_domain __rcu *, sd_numa);
DECLARE_PER_CPU(সংরচনা sched_domain __rcu *, sd_asym_packing);
DECLARE_PER_CPU(সংরচনা sched_domain __rcu *, sd_asym_cpucapacity);

বাহ্য সংরচনা static_চাবি_false sched_asym_cpucapacity;
বাহ্য সংরচনা static_চাবি_false sched_cluster_active;

স্থির __always_inline bool sched_asym_cpucap_active(শূন্য)
{
	প্রত্যাগম static_branch_unlikely(&sched_asym_cpucapacity);
}

সংরচনা sched_group_capacity {
	atomic_t		ref;
	/*
	 * CPU capacity of this group, SCHED_CAPACITY_SCALE being max capacity
	 * for a single CPU.
	 */
	unsigned long		capacity;
	unsigned long		min_capacity;		/* Min per-CPU capacity in group */
	unsigned long		max_capacity;		/* Max per-CPU capacity in group */
	unsigned long		next_update;
	পূর্ণাঙ্ক			imbalance;		/* XXX unrelated to capacity but shared group state */

	পূর্ণাঙ্ক			id;

	unsigned long		cpumask[];		/* Balance mask */
};

সংরচনা sched_group {
	সংরচনা sched_group	*next;			/* Must be a circular list */
	atomic_t		ref;

	unsigned পূর্ণাঙ্ক		group_weight;
	unsigned পূর্ণাঙ্ক		cores;
	সংরচনা sched_group_capacity *sgc;
	পূর্ণাঙ্ক			asym_prefer_cpu;	/* CPU of highest priority in group */
	পূর্ণাঙ্ক			flags;

	/*
	 * The CPUs this group covers.
	 *
	 * NOTE: this field is variable length. (Allocated dynamically
	 * by attaching extra space to the end of the structure,
	 * depending on how many CPUs the kernel has booted up with)
	 */
	unsigned long		cpumask[];
};

স্থির অন্তঃস্থ সংরচনা cpumask *sched_group_span(সংরচনা sched_group *sg)
{
	প্রত্যাগম to_cpumask(sg->cpumask);
}

/*
 * See build_balance_mask().
 */
স্থির অন্তঃস্থ সংরচনা cpumask *group_balance_mask(সংরচনা sched_group *sg)
{
	প্রত্যাগম to_cpumask(sg->sgc->cpumask);
}

বাহ্য পূর্ণাঙ্ক group_balance_cpu(সংরচনা sched_group *sg);

বাহ্য শূন্য update_sched_domain_debugfs(শূন্য);
বাহ্য শূন্য dirty_sched_domain_sysctl(পূর্ণাঙ্ক cpu);

বাহ্য পূর্ণাঙ্ক sched_update_scaling(শূন্য);

স্থির অন্তঃস্থ ধ্রুব সংরচনা cpumask *task_user_cpus(সংরচনা task_struct *p)
{
	যদি (!p->user_cpus_ptr)
		প্রত্যাগম cpu_possible_mask; /* &init_task.cpus_mask */
	প্রত্যাগম p->user_cpus_ptr;
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
স্থির অন্তঃস্থ সংরচনা task_group *task_group(সংরচনা task_struct *p)
{
	প্রত্যাগম p->sched_task_group;
}

#ifdef CONFIG_FAIR_GROUP_SCHED
/*
 * Defined here to be available before stats.h is included, since
 * stats.h has dependencies on things defined later in this file.
 */
সংরচনা cfs_tg_state {
	সংরচনা cfs_rq		cfs_rq;
	সংরচনা sched_entity	se;
	সংরচনা sched_statistics	stats;
} __no_randomize_layout;

/* Access a specific CPU's cfs_rq from a task group */
স্থির অন্তঃস্থ সংরচনা cfs_rq *tg_cfs_rq(সংরচনা task_group *tg, পূর্ণাঙ্ক cpu)
{
	প্রত্যাগম per_cpu_ptr(tg->cfs_rq, cpu);
}

স্থির অন্তঃস্থ সংরচনা sched_entity *tg_se(সংরচনা task_group *tg, পূর্ণাঙ্ক cpu)
{
	সংরচনা cfs_tg_state *state;

	যদি (is_মূল_task_group(tg))
		প্রত্যাগম রিক্ত;

	state = container_of(tg_cfs_rq(tg, cpu), সংরচনা cfs_tg_state, cfs_rq);
	প্রত্যাগম &state->se;
}

স্থির অন্তঃস্থ সংরচনা sched_entity *cfs_rq_se(সংরচনা cfs_rq *cfs_rq)
{
	সংরচনা cfs_tg_state *state;

	যদি (is_মূল_task_group(cfs_rq->tg))
		প্রত্যাগম রিক্ত;

	state = container_of(cfs_rq, সংরচনা cfs_tg_state, cfs_rq);
	প্রত্যাগম &state->se;
}
#endif

/* Change a task's cfs_rq and parent entity if it moves across CPUs/groups */
স্থির অন্তঃস্থ শূন্য set_task_rq(সংরচনা task_struct *p, unsigned পূর্ণাঙ্ক cpu)
{
#যদি defined(CONFIG_FAIR_GROUP_SCHED) || defined(CONFIG_RT_GROUP_SCHED)
	সংরচনা task_group *tg = task_group(p);
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
	যদি (!rt_group_sched_enabled())
		tg = &মূল_task_group;
	p->rt.rt_rq  = tg->rt_rq[cpu];
	p->rt.parent = tg->rt_se[cpu];
#endif /* CONFIG_RT_GROUP_SCHED */
}

#অন্যথা /* !CONFIG_CGROUP_SCHED: */

স্থির অন্তঃস্থ শূন্য set_task_rq(সংরচনা task_struct *p, unsigned পূর্ণাঙ্ক cpu) { }

স্থির অন্তঃস্থ সংরচনা task_group *task_group(সংরচনা task_struct *p)
{
	প্রত্যাগম রিক্ত;
}

#endif /* !CONFIG_CGROUP_SCHED */

স্থির অন্তঃস্থ শূন্য __set_task_cpu(সংরচনা task_struct *p, unsigned পূর্ণাঙ্ক cpu)
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

গণনা {
#অন্তর্ভাব "features.h"
	__SCHED_FEAT_NR,
};

#undef SCHED_FEAT

/*
 * To support run-time toggling of sched features, all the translation units
 * (but core.c) reference the sysctl_sched_features defined in core.c.
 */
বাহ্য __read_mostly unsigned পূর্ণাঙ্ক sysctl_sched_features;

#ifdef CONFIG_JUMP_LABEL

সঙ্ঘ sched_feat_চাবি {
	সংরচনা static_চাবি_true	চাবি_true;
	সংরচনা static_চাবি_false	চাবি_false;
};

#define sched_feat_branch_true(চাবি)	static_branch_likely(&(চাবি)->চাবি_true)
#define sched_feat_branch_false(চাবি)	static_branch_unlikely(&(চাবি)->চাবি_false)

#define SCHED_FEAT(name, enabled)			\
স্থির __always_inline bool				\
static_branch_##name(সঙ্ঘ sched_feat_চাবি *চাবি) 	\
{							\
	প্রত্যাগম sched_feat_branch_##enabled(চাবি);	\
}

#অন্তর্ভাব "features.h"
#undef SCHED_FEAT

বাহ্য সঙ্ঘ sched_feat_চাবি sched_feat_চাবিগুলি[__SCHED_FEAT_NR];
#define sched_feat(x) (static_branch_##x(&sched_feat_চাবিগুলি[__SCHED_FEAT_##x]))

#অন্যথা /* !CONFIG_JUMP_LABEL: */

#define sched_feat(x) (sysctl_sched_features & (1UL << __SCHED_FEAT_##x))

#endif /* !CONFIG_JUMP_LABEL */

বাহ্য সংরচনা static_চাবি_false sched_numa_balancing;
বাহ্য সংরচনা static_চাবি_false sched_schedstats;

স্থির অন্তঃস্থ u64 global_rt_period(শূন্য)
{
	প্রত্যাগম (u64)sysctl_sched_rt_period * NSEC_PER_USEC;
}

স্থির অন্তঃস্থ u64 global_rt_runtime(শূন্য)
{
	যদি (sysctl_sched_rt_runtime < 0)
		প্রত্যাগম RUNTIME_INF;

	প্রত্যাগম (u64)sysctl_sched_rt_runtime * NSEC_PER_USEC;
}

/*
 * Is p the current execution context?
 */
স্থির অন্তঃস্থ পূর্ণাঙ্ক task_current(সংরচনা rq *rq, সংরচনা task_struct *p)
{
	প্রত্যাগম rq->curr == p;
}

/*
 * Is p the current scheduling context?
 *
 * Note that it might be the current execution context at the same time if
 * rq->curr == rq->donor == p.
 */
স্থির অন্তঃস্থ পূর্ণাঙ্ক task_current_donor(সংরচনা rq *rq, সংরচনা task_struct *p)
{
	প্রত্যাগম rq->donor == p;
}

স্থির অন্তঃস্থ bool task_is_blocked(সংরচনা task_struct *p)
{
	যদি (!sched_proxy_exec())
		প্রত্যাগম false;

	প্রত্যাগম !!p->blocked_on;
}

#ifdef CONFIG_SCHED_PROXY_EXEC
শূন্য sched_proxy_block_task(সংরচনা rq *rq, সংরচনা task_struct *p);
#অন্যথা
স্থির অন্তঃস্থ শূন্য sched_proxy_block_task(সংরচনা rq *rq, সংরচনা task_struct *p) {}
#endif

স্থির অন্তঃস্থ পূর্ণাঙ্ক task_on_cpu(সংরচনা rq *rq, সংরচনা task_struct *p)
{
	প্রত্যাগম p->on_cpu;
}

স্থির অন্তঃস্থ পূর্ণাঙ্ক task_on_rq_queued(সংরচনা task_struct *p)
{
	প্রত্যাগম READ_ONCE(p->on_rq) == TASK_ON_RQ_QUEUED;
}

স্থির অন্তঃস্থ পূর্ণাঙ্ক task_on_rq_migrating(সংরচনা task_struct *p)
{
	প্রত্যাগম READ_ONCE(p->on_rq) == TASK_ON_RQ_MIGRATING;
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

বাহ্য ধ্রুব পূর্ণাঙ্ক		sched_prio_to_weight[40];
বাহ্য ধ্রুব u32		sched_prio_to_wmult[40];

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

#define RETRY_TASK		((শূন্য *)-1UL)

সংরচনা affinity_context {
	ধ্রুব সংরচনা cpumask	*new_mask;
	সংরচনা cpumask		*user_mask;
	unsigned পূর্ণাঙ্ক		flags;
};

বাহ্য s64 update_curr_common(সংরচনা rq *rq);

গণনা snt_e {
	SNT_NORMAL,	/* set_next_task() */
	SNT_PICK,	/* put_prev_set_next_task(): prev != next */
	SNT_REPICK,	/* put_prev_set_next_task(): prev == next */
};

সংরচনা sched_class {

#ifdef CONFIG_UCLAMP_TASK
	পূর্ণাঙ্ক uclamp_enabled;
#endif

	/*
	 * move_queued_task/activate_task/enqueue_task: rq->lock
	 * ttwu_do_activate/activate_task/enqueue_task: rq->lock
	 * wake_up_new_task/activate_task/enqueue_task: task_rq_lock
	 * ttwu_runnable/enqueue_task: task_rq_lock
	 * proxy_task_current: rq->lock
	 * sched_change_end
	 */
	শূন্য (*enqueue_task) (সংরচনা rq *rq, সংরচনা task_struct *p, পূর্ণাঙ্ক flags);
	/*
	 * move_queued_task/deactivate_task/dequeue_task: rq->lock
	 * __schedule/block_task/dequeue_task: rq->lock
	 * proxy_task_current: rq->lock
	 * wait_task_inactive: task_rq_lock
	 * sched_change_begin
	 */
	bool (*dequeue_task) (সংরচনা rq *rq, সংরচনা task_struct *p, পূর্ণাঙ্ক flags);

	/*
	 * do_sched_yield: rq->lock
	 */
	শূন্য (*yield_task)   (সংরচনা rq *rq);
	/*
	 * yield_to: rq->lock (double)
	 */
	bool (*yield_to_task)(সংরচনা rq *rq, সংরচনা task_struct *p);

	/*
	 * move_queued_task: rq->lock
	 * __migrate_swap_task: rq->lock
	 * ttwu_do_activate: rq->lock
	 * ttwu_runnable: task_rq_lock
	 * wake_up_new_task: task_rq_lock
	 */
	শূন্য (*wakeup_preempt)(সংরচনা rq *rq, সংরচনা task_struct *p, পূর্ণাঙ্ক flags);

	/*
	 * schedule/pick_next_task/prev_balance: rq->lock
	 */
	পূর্ণাঙ্ক (*balance)(সংরচনা rq *rq, সংরচনা rq_flags *rf);

	/*
	 * schedule/pick_next_task: rq->lock
	 */
	সংরচনা task_struct *(*pick_task)(সংরচনা rq *rq, সংরচনা rq_flags *rf);

	/*
	 * sched_change:
	 * __schedule: rq->lock
	 */
	শূন্য (*put_prev_task)(সংরচনা rq *rq, সংরচনা task_struct *p, সংরচনা task_struct *next);
	শূন্য (*set_next_task)(সংরচনা rq *rq, সংরচনা task_struct *p, গণনা snt_e type);

	/*
	 * select_task_rq: p->pi_lock
	 * sched_exec: p->pi_lock
	 */
	পূর্ণাঙ্ক  (*select_task_rq)(সংরচনা task_struct *p, পূর্ণাঙ্ক task_cpu, পূর্ণাঙ্ক flags);

	/*
	 * set_task_cpu: p->pi_lock || rq->lock (ttwu like)
	 */
	শূন্য (*migrate_task_rq)(সংরচনা task_struct *p, পূর্ণাঙ্ক new_cpu);

	/*
	 * ttwu_do_activate: rq->lock
	 * wake_up_new_task: task_rq_lock
	 */
	শূন্য (*task_woken)(সংরচনা rq *this_rq, সংরচনা task_struct *task);

	/*
	 * do_set_cpus_allowed: task_rq_lock + sched_change
	 */
	শূন্য (*set_cpus_allowed)(সংরচনা task_struct *p, সংরচনা affinity_context *ctx);

	/*
	 * sched_set_rq_{on,off}line: rq->lock
	 */
	শূন্য (*rq_online)(সংরচনা rq *rq);
	শূন্য (*rq_offline)(সংরচনা rq *rq);

	/*
	 * push_cpu_stop: p->pi_lock && rq->lock
	 */
	সংরচনা rq *(*find_lock_rq)(সংরচনা task_struct *p, সংরচনা rq *rq);

	/*
	 * hrtick: rq->lock
	 * sched_tick: rq->lock
	 * sched_tick_remote: rq->lock
	 */
	শূন্য (*task_tick)(সংরচনা rq *rq, সংরচনা task_struct *p, পূর্ণাঙ্ক queued);
	/*
	 * sched_cgroup_fork: p->pi_lock
	 */
	শূন্য (*task_fork)(সংরচনা task_struct *p);
	/*
	 * finish_task_switch: no locks
	 */
	শূন্য (*task_dead)(সংরচনা task_struct *p);

	/*
	 * sched_change
	 */
	শূন্য (*switching_from)(সংরচনা rq *this_rq, সংরচনা task_struct *task);
	শূন্য (*switched_from) (সংরচনা rq *this_rq, সংরচনা task_struct *task);
	শূন্য (*switching_to)  (সংরচনা rq *this_rq, সংরচনা task_struct *task);
	শূন্য (*switched_to)   (সংরচনা rq *this_rq, সংরচনা task_struct *task);
	u64  (*get_prio)     (সংরচনা rq *this_rq, সংরচনা task_struct *task);
	শূন্য (*prio_changed) (সংরচনা rq *this_rq, সংরচনা task_struct *task,
			      u64 oldprio);

	/*
	 * set_load_weight: task_rq_lock + sched_change
	 * __setscheduler_parms: task_rq_lock + sched_change
	 */
	শূন্য (*reweight_task)(সংরচনা rq *this_rq, সংরচনা task_struct *task,
			      ধ্রুব সংরচনা load_weight *lw);

	/*
	 * sched_rr_get_interval: task_rq_lock
	 */
	unsigned পূর্ণাঙ্ক (*get_rr_interval)(সংরচনা rq *rq,
					সংরচনা task_struct *task);

	/*
	 * task_sched_runtime: task_rq_lock
	 */
	শূন্য (*update_curr)(সংরচনা rq *rq);

#ifdef CONFIG_FAIR_GROUP_SCHED
	/*
	 * sched_change_group: task_rq_lock + sched_change
	 */
	শূন্য (*task_change_group)(সংরচনা task_struct *p);
#endif

#ifdef CONFIG_SCHED_CORE
	/*
	 * pick_next_task: rq->lock
	 * try_steal_cookie: rq->lock (double)
	 */
	পূর্ণাঙ্ক (*task_is_throttled)(সংরচনা task_struct *p, পূর্ণাঙ্ক cpu);
#endif
};

স্থির অন্তঃস্থ শূন্য put_prev_task(সংরচনা rq *rq, সংরচনা task_struct *prev)
{
	WARN_ON_ONCE(rq->donor != prev);
	prev->sched_class->put_prev_task(rq, prev, রিক্ত);
}

স্থির অন্তঃস্থ শূন্য set_next_task(সংরচনা rq *rq, সংরচনা task_struct *next)
{
	next->sched_class->set_next_task(rq, next, SNT_NORMAL);
}

স্থির অন্তঃস্থ শূন্য
__put_prev_set_next_dl_server(সংরচনা rq *rq,
			      সংরচনা task_struct *prev,
			      সংরচনা task_struct *next)
{
	prev->dl_server = রিক্ত;
	next->dl_server = rq->dl_server;
	rq->dl_server = রিক্ত;
}

স্থির অন্তঃস্থ শূন্য put_prev_set_next_task(সংরচনা rq *rq,
					  সংরচনা task_struct *prev,
					  সংরচনা task_struct *next)
{
	WARN_ON_ONCE(rq->donor != prev);

	__put_prev_set_next_dl_server(rq, prev, next);

	যদি (next == prev) {
		next->sched_class->set_next_task(rq, next, SNT_REPICK);
		প্রত্যাগম;
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
ধ্রুব সংরচনা sched_class name##_sched_class \
	__aligned(__alignof__(সংরচনা sched_class)) \
	__section("__" #name "_sched_class")

/* Defined in include/asm-generic/vmlinux.lds.h */
বাহ্য সংরচনা sched_class __sched_class_highest[];
বাহ্য সংরচনা sched_class __sched_class_lowest[];

বাহ্য ধ্রুব সংরচনা sched_class stop_sched_class;
বাহ্য ধ্রুব সংরচনা sched_class dl_sched_class;
বাহ্য ধ্রুব সংরচনা sched_class rt_sched_class;
বাহ্য ধ্রুব সংরচনা sched_class fair_sched_class;
বাহ্য ধ্রুব সংরচনা sched_class idle_sched_class;

/*
 * Iterate only active classes. SCX can take over all fair tasks or be
 * completely disabled. If the former, skip fair. If the latter, skip SCX.
 */
স্থির অন্তঃস্থ ধ্রুব সংরচনা sched_class *next_active_class(ধ্রুব সংরচনা sched_class *class)
{
	class++;
#ifdef CONFIG_SCHED_CLASS_EXT
	যদি (scx_switched_all() && class == &fair_sched_class)
		class++;
	যদি (!scx_enabled() && class == &ext_sched_class)
		class++;
#endif
	প্রত্যাগম class;
}

#define for_class_range(class, _from, _to) \
	প্রতি (class = (_from); class < (_to); class++)

#define for_each_class(class) \
	for_class_range(class, __sched_class_highest, __sched_class_lowest)

#define for_active_class_range(class, _from, _to)				\
	প্রতি (class = (_from); class != (_to); class = next_active_class(class))

#define for_each_active_class(class)						\
	for_active_class_range(class, __sched_class_highest, __sched_class_lowest)

#define sched_class_above(_a, _b)	((_a) < (_b))

স্থির অন্তঃস্থ শূন্য rq_modified_begin(সংরচনা rq *rq, ধ্রুব সংরচনা sched_class *class)
{
	যদি (sched_class_above(rq->next_class, class))
		rq->next_class = class;
}

স্থির অন্তঃস্থ bool rq_modified_above(সংরচনা rq *rq, ধ্রুব সংরচনা sched_class *class)
{
	প্রত্যাগম sched_class_above(rq->next_class, class);
}

স্থির অন্তঃস্থ bool sched_stop_runnable(সংরচনা rq *rq)
{
	প্রত্যাগম rq->stop && task_on_rq_queued(rq->stop);
}

স্থির অন্তঃস্থ bool sched_dl_runnable(সংরচনা rq *rq)
{
	প্রত্যাগম rq->dl.dl_nr_running > 0;
}

স্থির অন্তঃস্থ bool sched_rt_runnable(সংরচনা rq *rq)
{
	প্রত্যাগম rq->rt.rt_queued > 0;
}

স্থির অন্তঃস্থ bool sched_fair_runnable(সংরচনা rq *rq)
{
	প্রত্যাগম rq->cfs.nr_queued > 0;
}

বাহ্য সংরচনা task_struct *pick_task_fair(সংরচনা rq *rq, সংরচনা rq_flags *rf);
বাহ্য সংরচনা task_struct *pick_task_idle(সংরচনা rq *rq, সংরচনা rq_flags *rf);

#define SCA_CHECK		0x01
#define SCA_MIGRATE_DISABLE	0x02
#define SCA_MIGRATE_ENABLE	0x04
#define SCA_USER		0x08

বাহ্য শূন্য update_group_capacity(সংরচনা sched_domain *sd, পূর্ণাঙ্ক cpu);

বাহ্য শূন্য sched_balance_trigger(সংরচনা rq *rq);

বাহ্য পূর্ণাঙ্ক __set_cpus_allowed_ptr(সংরচনা task_struct *p, সংরচনা affinity_context *ctx);
বাহ্য শূন্য set_cpus_allowed_common(সংরচনা task_struct *p, সংরচনা affinity_context *ctx);

স্থির অন্তঃস্থ bool task_allowed_on_cpu(সংরচনা task_struct *p, পূর্ণাঙ্ক cpu)
{
	/* When not in the task's cpumask, no point in looking further. */
	যদি (!cpumask_test_cpu(cpu, p->cpus_ptr))
		প্রত্যাগম false;

	/* Can @cpu run a user thread? */
	যদি (!(p->flags & PF_KTHREAD) && !task_cpu_possible(cpu, p))
		প্রত্যাগম false;

	প্রত্যাগম true;
}

স্থির অন্তঃস্থ cpumask_t *alloc_user_cpus_ptr(পূর্ণাঙ্ক গ্রন্থি)
{
	/*
	 * See set_cpus_allowed_force() above for the rcu_head usage.
	 */
	পূর্ণাঙ্ক size = max_t(পূর্ণাঙ্ক, cpumask_size(), পরিমাণ(সংরচনা rcu_head));

	প্রত্যাগম kmalloc_গ্রন্থি(size, GFP_KERNEL, গ্রন্থি);
}

স্থির অন্তঃস্থ সংরচনা task_struct *get_ঠেলো_task(সংরচনা rq *rq)
{
	সংরচনা task_struct *p = rq->donor;

	lockdep_assert_rq_held(rq);

	যদি (rq->ঠেলো_busy)
		প্রত্যাগম রিক্ত;

	যদি (p->nr_cpus_allowed == 1)
		প্রত্যাগম রিক্ত;

	যদি (p->migration_disabled)
		প্রত্যাগম রিক্ত;

	rq->ঠেলো_busy = true;
	প্রত্যাগম get_task_struct(p);
}

বাহ্য পূর্ণাঙ্ক ঠেলো_cpu_stop(শূন্য *arg);

#ifdef CONFIG_CPU_IDLE

স্থির অন্তঃস্থ শূন্য idle_set_state(সংরচনা rq *rq,
				  সংরচনা cpuidle_state *idle_state)
{
	rq->idle_state = idle_state;
}

স্থির অন্তঃস্থ সংরচনা cpuidle_state *idle_get_state(সংরচনা rq *rq)
{
	lockdep_assert(rcu_read_lock_any_held());

	প্রত্যাগম rq->idle_state;
}

#অন্যথা /* !CONFIG_CPU_IDLE: */

স্থির অন্তঃস্থ শূন্য idle_set_state(সংরচনা rq *rq,
				  সংরচনা cpuidle_state *idle_state)
{
}

স্থির অন্তঃস্থ সংরচনা cpuidle_state *idle_get_state(সংরচনা rq *rq)
{
	প্রত্যাগম রিক্ত;
}

#endif /* !CONFIG_CPU_IDLE */

বাহ্য শূন্য schedule_idle(শূন্য);
asmlinkage শূন্য schedule_user(শূন্য);

বাহ্য শূন্য sysrq_sched_debug_show(শূন্য);
বাহ্য শূন্য sched_init_granularity(শূন্য);
বাহ্য শূন্য update_max_interval(শূন্য);

বাহ্য শূন্য init_sched_dl_class(শূন্য);
বাহ্য শূন্য init_sched_rt_class(শূন্য);
বাহ্য শূন্য init_sched_fair_class(শূন্য);

বাহ্য শূন্য resched_curr(সংরচনা rq *rq);
বাহ্য শূন্য resched_curr_lazy(সংরচনা rq *rq);
বাহ্য শূন্য resched_cpu(পূর্ণাঙ্ক cpu);

বাহ্য শূন্য init_rt_bandwidth(সংরচনা rt_bandwidth *rt_b, u64 period, u64 runtime);
বাহ্য bool sched_rt_bandwidth_account(সংরচনা rt_rq *rt_rq);

বাহ্য শূন্য init_dl_entity(সংরচনা sched_dl_entity *dl_se);

বাহ্য শূন্য init_cfs_throttle_work(সংরচনা task_struct *p);

#define BW_SHIFT		20
#define BW_UNIT			(1 << BW_SHIFT)
#define RATIO_SHIFT		8
#define MAX_BW_BITS		(64 - BW_SHIFT)
#define MAX_BW			((1ULL << MAX_BW_BITS) - 1)

বাহ্য u64 to_ratio(u64 period, u64 runtime);

বাহ্য শূন্য init_entity_runnable_average(সংরচনা sched_entity *se);
বাহ্য শূন্য post_init_entity_util_avg(সংরচনা task_struct *p);

#ifdef CONFIG_NO_HZ_FULL
বাহ্য bool sched_can_stop_tick(সংরচনা rq *rq);
বাহ্য পূর্ণাঙ্ক __init sched_tick_offload_init(শূন্য);

/*
 * Tick may be needed by tasks in the runqueue depending on their policy and
 * requirements. If tick is needed, lets send the target an IPI to kick it out of
 * nohz mode if necessary.
 */
স্থির অন্তঃস্থ শূন্য sched_update_tick_dependency(সংরচনা rq *rq)
{
	পূর্ণাঙ্ক cpu = cpu_of(rq);

	যদি (!tick_nohz_full_cpu(cpu))
		প্রত্যাগম;

	যদি (sched_can_stop_tick(rq))
		tick_nohz_dep_clear_cpu(cpu, TICK_DEP_BIT_SCHED);
	অন্যথা
		tick_nohz_dep_set_cpu(cpu, TICK_DEP_BIT_SCHED);
}
#অন্যথা /* !CONFIG_NO_HZ_FULL: */
স্থির অন্তঃস্থ পূর্ণাঙ্ক sched_tick_offload_init(শূন্য) { প্রত্যাগম 0; }
স্থির অন্তঃস্থ শূন্য sched_update_tick_dependency(সংরচনা rq *rq) { }
#endif /* !CONFIG_NO_HZ_FULL */

স্থির অন্তঃস্থ শূন্য add_nr_running(সংরচনা rq *rq, unsigned গুনতি)
{
	unsigned prev_nr = rq->nr_running;

	rq->nr_running = prev_nr + গুনতি;
	যদি (trace_sched_update_nr_running_tp_enabled()) {
		call_trace_sched_update_nr_running(rq, গুনতি);
	}

	যদি (prev_nr < 2 && rq->nr_running >= 2)
		set_rd_overloaded(rq->rd, 1);

	sched_update_tick_dependency(rq);
}

স্থির অন্তঃস্থ শূন্য sub_nr_running(সংরচনা rq *rq, unsigned গুনতি)
{
	rq->nr_running -= গুনতি;
	যদি (trace_sched_update_nr_running_tp_enabled()) {
		call_trace_sched_update_nr_running(rq, -গুনতি);
	}

	/* Check if we still need preemption */
	sched_update_tick_dependency(rq);
}

স্থির অন্তঃস্থ শূন্য __block_task(সংরচনা rq *rq, সংরচনা task_struct *p)
{
	যদি (p->sched_contributes_to_load)
		rq->nr_uninterruptible++;

	যদি (p->in_iowait) {
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

বাহ্য শূন্য activate_task(সংরচনা rq *rq, সংরচনা task_struct *p, পূর্ণাঙ্ক flags);
বাহ্য শূন্য deactivate_task(সংরচনা rq *rq, সংরচনা task_struct *p, পূর্ণাঙ্ক flags);

বাহ্য শূন্য wakeup_preempt(সংরচনা rq *rq, সংরচনা task_struct *p, পূর্ণাঙ্ক flags);

/*
 * attach_task() -- attach the task detached by detach_task() to its new rq.
 */
স্থির অন্তঃস্থ শূন্য attach_task(সংরচনা rq *rq, সংরচনা task_struct *p)
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
স্থির অন্তঃস্থ শূন্য attach_one_task(সংরচনা rq *rq, সংরচনা task_struct *p)
{
	guard(rq_lock)(rq);
	update_rq_clock(rq);
	attach_task(rq, p);
}

/*
 * __attach_tasks() - attaches a list of tasks (using se.group_node) to
 * the new rq
 */
স্থির অন্তঃস্থ শূন্য __attach_tasks(সংরচনা rq *rq, সংরচনা list_head *tasks)
{
	guard(rq_lock)(rq);
	update_rq_clock(rq);

	যাবত্ (!list_empty(tasks)) {
		সংরচনা task_struct *p;

		p = list_first_entry(tasks, সংরচনা task_struct, se.group_গ্রন্থি);
		list_del_init(&p->se.group_গ্রন্থি);

		attach_task(rq, p);
	}
}

#ifdef CONFIG_PREEMPT_RT
# define SCHED_NR_MIGRATE_BREAK 8
#অন্যথা
# define SCHED_NR_MIGRATE_BREAK 32
#endif

বাহ্য __read_mostly unsigned পূর্ণাঙ্ক sysctl_sched_nr_migrate;
বাহ্য __read_mostly unsigned পূর্ণাঙ্ক sysctl_sched_migration_cost;

বাহ্য unsigned পূর্ণাঙ্ক sysctl_sched_base_slice;

বাহ্য পূর্ণাঙ্ক sysctl_resched_latency_warn_ms;
বাহ্য পূর্ণাঙ্ক sysctl_resched_latency_warn_once;

বাহ্য unsigned পূর্ণাঙ্ক sysctl_sched_tunable_scaling;

বাহ্য unsigned পূর্ণাঙ্ক sysctl_numa_balancing_scan_delay;
বাহ্য unsigned পূর্ণাঙ্ক sysctl_numa_balancing_scan_period_min;
বাহ্য unsigned পূর্ণাঙ্ক sysctl_numa_balancing_scan_period_max;
বাহ্য unsigned পূর্ণাঙ্ক sysctl_numa_balancing_scan_size;
বাহ্য unsigned পূর্ণাঙ্ক sysctl_numa_balancing_hot_threshold;

#ifdef CONFIG_SCHED_HRTICK

/*
 * Use hrtick when:
 *  - enabled by features
 *  - hrtimer is actually high res
 */
স্থির অন্তঃস্থ bool hrtick_enabled(সংরচনা rq *rq)
{
	প্রত্যাগম cpu_active(cpu_of(rq)) && hrtimer_highres_enabled();
}

স্থির অন্তঃস্থ bool hrtick_enabled_fair(সংরচনা rq *rq)
{
	প্রত্যাগম sched_feat(HRTICK) && hrtick_enabled(rq);
}

স্থির অন্তঃস্থ bool hrtick_enabled_dl(সংরচনা rq *rq)
{
	প্রত্যাগম sched_feat(HRTICK_DL) && hrtick_enabled(rq);
}

বাহ্য শূন্য hrtick_start(সংরচনা rq *rq, u64 delay);
স্থির অন্তঃস্থ bool hrtick_active(সংরচনা rq *rq)
{
	প্রত্যাগম hrtimer_active(&rq->hrtick_timer);
}

#অন্যথা /* !CONFIG_SCHED_HRTICK: */
স্থির অন্তঃস্থ bool hrtick_enabled_fair(সংরচনা rq *rq) { প্রত্যাগম false; }
স্থির অন্তঃস্থ bool hrtick_enabled_dl(সংরচনা rq *rq) { প্রত্যাগম false; }
স্থির অন্তঃস্থ bool hrtick_enabled(সংরচনা rq *rq) { প্রত্যাগম false; }
#endif /* !CONFIG_SCHED_HRTICK */

#ifndef arch_scale_freq_tick
স্থির __always_inline শূন্য arch_scale_freq_tick(শূন্য) { }
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
স্থির __always_inline
unsigned long arch_scale_freq_capacity(পূর্ণাঙ্ক cpu)
{
	প্রত্যাগম SCHED_CAPACITY_SCALE;
}
#endif

/*
 * In double_lock_balance()/double_rq_lock(), we use raw_spin_rq_lock() to
 * acquire rq lock instead of rq_lock(). So at the end of these two functions
 * we need to call double_rq_clock_clear_update() to clear RQCF_UPDATED of
 * rq->clock_update_flags to avoid the WARN_DOUBLE_CLOCK warning.
 */
স্থির অন্তঃস্থ শূন্য double_rq_clock_clear_update(সংরচনা rq *rq1, সংরচনা rq *rq2)
{
	rq1->clock_update_flags &= (RQCF_REQ_SKIP|RQCF_ACT_SKIP);
	rq2->clock_update_flags &= (RQCF_REQ_SKIP|RQCF_ACT_SKIP);
}

#define DEFINE_LOCK_GUARD_2(name, type, _lock, _unlock, ...)				\
__DEFINE_UNLOCK_GUARD(name, type, _unlock, type *lock2; __VA_ARGS__)			\
স্থির অন্তঃস্থ class_##name##_t class_##name##_constructor(type *lock, type *lock2)	\
	__no_context_analysis								\
{ class_##name##_t _t = { .lock = lock, .lock2 = lock2 }, *_T = &_t;			\
  _lock; প্রত্যাগম _t; }
#define DECLARE_LOCK_GUARD_2_ATTRS(_name, _lock, _unlock1, _unlock2)			\
স্থির অন্তঃস্থ class_##_name##_t class_##_name##_constructor(lock_##_name##_t *_T1,	\
							    lock_##_name##_t *_T2) _lock; \
স্থির __always_inline শূন্য __class_##_name##_cleanup_ctx1(class_##_name##_t **_T1)	\
	__no_context_analysis _unlock1 { }						\
স্থির __always_inline শূন্য __class_##_name##_cleanup_ctx2(class_##_name##_t **_T2)	\
	__no_context_analysis _unlock2 { }
#define WITH_LOCK_GUARD_2_ATTRS(_name, _T1, _T2)					\
	class_##_name##_constructor(_T1, _T2),						\
	*__UNIQUE_ID(unlock1) __cleanup(__class_##_name##_cleanup_ctx1) = (শূন্য *)(_T1),\
	*__UNIQUE_ID(unlock2) __cleanup(__class_##_name##_cleanup_ctx2) = (শূন্য *)(_T2)

স্থির অন্তঃস্থ bool rq_order_less(সংরচনা rq *rq1, সংরচনা rq *rq2)
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
	যদি (rq1->core->cpu < rq2->core->cpu)
		প্রত্যাগম true;
	যদি (rq1->core->cpu > rq2->core->cpu)
		প্রত্যাগম false;

	/*
	 * __sched_core_flip() relies on SMT having cpu-id lock order.
	 */
#endif /* CONFIG_SCHED_CORE */
	প্রত্যাগম rq1->cpu < rq2->cpu;
}

বাহ্য শূন্য double_rq_lock(সংরচনা rq *rq1, সংরচনা rq *rq2)
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
স্থির অন্তঃস্থ পূর্ণাঙ্ক _double_lock_balance(সংরচনা rq *this_rq, সংরচনা rq *busiest)
	__must_hold(__rq_lockp(this_rq))
	__acquires(__rq_lockp(busiest))
{
	raw_spin_rq_unlock(this_rq);
	double_rq_lock(this_rq, busiest);

	প্রত্যাগম 1;
}

#অন্যথা /* !CONFIG_PREEMPTION: */
/*
 * Unfair double_lock_balance: Optimizes throughput at the expense of
 * latency by eliminating extra atomic operations when the locks are
 * already in proper order on entry.  This favors lower CPU-ids and will
 * grant the double lock to lower CPUs over higher ids under contention,
 * regardless of entry order into the function.
 */
স্থির অন্তঃস্থ পূর্ণাঙ্ক _double_lock_balance(সংরচনা rq *this_rq, সংরচনা rq *busiest)
	__must_hold(__rq_lockp(this_rq))
	__acquires(__rq_lockp(busiest))
{
	যদি (__rq_lockp(this_rq) == __rq_lockp(busiest)) {
		__acquire(__rq_lockp(busiest)); /* already held */
		double_rq_clock_clear_update(this_rq, busiest);
		প্রত্যাগম 0;
	}

	যদি (likely(raw_spin_rq_trylock(busiest))) {
		double_rq_clock_clear_update(this_rq, busiest);
		প্রত্যাগম 0;
	}

	যদি (rq_order_less(this_rq, busiest)) {
		raw_spin_rq_lock_nested(busiest, SINGLE_DEPTH_NESTING);
		double_rq_clock_clear_update(this_rq, busiest);
		প্রত্যাগম 0;
	}

	raw_spin_rq_unlock(this_rq);
	double_rq_lock(this_rq, busiest);

	প্রত্যাগম 1;
}

#endif /* !CONFIG_PREEMPTION */

/*
 * double_lock_balance - lock the busiest runqueue, this_rq is locked already.
 */
স্থির অন্তঃস্থ পূর্ণাঙ্ক double_lock_balance(সংরচনা rq *this_rq, সংরচনা rq *busiest)
	__must_hold(__rq_lockp(this_rq))
	__acquires(__rq_lockp(busiest))
{
	lockdep_assert_irqs_disabled();

	প্রত্যাগম _double_lock_balance(this_rq, busiest);
}

স্থির অন্তঃস্থ শূন্য double_unlock_balance(সংরচনা rq *this_rq, সংরচনা rq *busiest)
	__releases(__rq_lockp(busiest))
{
	যদি (__rq_lockp(this_rq) != __rq_lockp(busiest))
		raw_spin_rq_unlock(busiest);
	অন্যথা
		__release(__rq_lockp(busiest)); /* fake release */
	lock_set_subclass(&__rq_lockp(this_rq)->dep_map, 0, _RET_IP_);
}

স্থির অন্তঃস্থ শূন্য double_lock(spinlock_t *l1, spinlock_t *l2)
	__acquires(l1, l2)
{
	যদি (l1 > l2)
		swap(l1, l2);

	spin_lock(l1);
	spin_lock_nested(l2, SINGLE_DEPTH_NESTING);
}

স্থির অন্তঃস্থ শূন্য double_lock_irq(spinlock_t *l1, spinlock_t *l2)
	__acquires(l1, l2)
{
	যদি (l1 > l2)
		swap(l1, l2);

	spin_lock_irq(l1);
	spin_lock_nested(l2, SINGLE_DEPTH_NESTING);
}

স্থির অন্তঃস্থ শূন্য double_raw_lock(raw_spinlock_t *l1, raw_spinlock_t *l2)
	__acquires(l1, l2)
{
	যদি (l1 > l2)
		swap(l1, l2);

	raw_spin_lock(l1);
	raw_spin_lock_nested(l2, SINGLE_DEPTH_NESTING);
}

স্থির অন্তঃস্থ শূন্য double_raw_unlock(raw_spinlock_t *l1, raw_spinlock_t *l2)
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
স্থির অন্তঃস্থ শূন্য double_rq_unlock(সংরচনা rq *rq1, সংরচনা rq *rq2)
	__releases(__rq_lockp(rq1), __rq_lockp(rq2))
{
	যদি (__rq_lockp(rq1) != __rq_lockp(rq2))
		raw_spin_rq_unlock(rq2);
	অন্যথা
		__release(__rq_lockp(rq2)); /* fake release */
	raw_spin_rq_unlock(rq1);
}

বাহ্য শূন্য set_rq_online (সংরচনা rq *rq);
বাহ্য শূন্য set_rq_offline(সংরচনা rq *rq);

বাহ্য bool sched_smp_initialized;

DEFINE_LOCK_GUARD_2(double_rq_lock, সংরচনা rq,
		    double_rq_lock(_T->lock, _T->lock2),
		    double_rq_unlock(_T->lock, _T->lock2))

বাহ্য সংরচনা sched_entity *__pick_root_entity(সংরচনা cfs_rq *cfs_rq);
বাহ্য সংরচনা sched_entity *__pick_first_entity(সংরচনা cfs_rq *cfs_rq);
বাহ্য সংরচনা sched_entity *__pick_last_entity(সংরচনা cfs_rq *cfs_rq);

বাহ্য bool sched_debug_verbose;

বাহ্য শূন্য print_cfs_stats(সংরচনা seq_file *m, পূর্ণাঙ্ক cpu);
বাহ্য শূন্য print_rt_stats(সংরচনা seq_file *m, পূর্ণাঙ্ক cpu);
বাহ্য শূন্য print_dl_stats(সংরচনা seq_file *m, পূর্ণাঙ্ক cpu);
বাহ্য শূন্য print_cfs_rq(সংরচনা seq_file *m, পূর্ণাঙ্ক cpu, সংরচনা cfs_rq *cfs_rq);
বাহ্য শূন্য print_rt_rq(সংরচনা seq_file *m, পূর্ণাঙ্ক cpu, সংরচনা rt_rq *rt_rq);
বাহ্য শূন্য print_dl_rq(সংরচনা seq_file *m, পূর্ণাঙ্ক cpu, সংরচনা dl_rq *dl_rq);

বাহ্য শূন্য resched_latency_warn(পূর্ণাঙ্ক cpu, u64 latency);

#ifdef CONFIG_NUMA_BALANCING
বাহ্য শূন্য show_numa_stats(সংরচনা task_struct *p, সংরচনা seq_file *m);
বাহ্য শূন্য
print_numa_stats(সংরচনা seq_file *m, পূর্ণাঙ্ক গ্রন্থি, unsigned long tsf,
		 unsigned long tpf, unsigned long gsf, unsigned long gpf);
#endif /* CONFIG_NUMA_BALANCING */

বাহ্য শূন্য init_cfs_rq(সংরচনা cfs_rq *cfs_rq);
বাহ্য শূন্য init_rt_rq(সংরচনা rt_rq *rt_rq);
বাহ্য শূন্য init_dl_rq(সংরচনা dl_rq *dl_rq);

বাহ্য শূন্য cfs_bandwidth_usage_inc(শূন্য);
বাহ্য শূন্য cfs_bandwidth_usage_dec(শূন্য);

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

বাহ্য শূন্য nohz_balance_exit_idle(সংরচনা rq *rq);
#অন্যথা /* !CONFIG_NO_HZ_COMMON: */
স্থির অন্তঃস্থ শূন্য nohz_balance_exit_idle(সংরচনা rq *rq) { }
#endif /* !CONFIG_NO_HZ_COMMON */

#ifdef CONFIG_NO_HZ_COMMON
বাহ্য শূন্য nohz_run_idle_balance(পূর্ণাঙ্ক cpu);
#অন্যথা
স্থির অন্তঃস্থ শূন্য nohz_run_idle_balance(পূর্ণাঙ্ক cpu) { }
#endif

#অন্তর্ভাব "stats.h"

#যদি defined(CONFIG_SCHED_CORE) && defined(CONFIG_SCHEDSTATS)

বাহ্য শূন্য __sched_core_account_forceidle(সংরচনা rq *rq);

স্থির অন্তঃস্থ শূন্য sched_core_account_forceidle(সংরচনা rq *rq)
{
	যদি (schedstat_enabled())
		__sched_core_account_forceidle(rq);
}

বাহ্য শূন্য __sched_core_tick(সংরচনা rq *rq);

স্থির অন্তঃস্থ শূন্য sched_core_tick(সংরচনা rq *rq)
{
	যদি (sched_core_enabled(rq) && schedstat_enabled())
		__sched_core_tick(rq);
}

#অন্যথা /* !(CONFIG_SCHED_CORE && CONFIG_SCHEDSTATS): */

স্থির অন্তঃস্থ শূন্য sched_core_account_forceidle(সংরচনা rq *rq) { }

স্থির অন্তঃস্থ শূন্য sched_core_tick(সংরচনা rq *rq) { }

#endif /* !(CONFIG_SCHED_CORE && CONFIG_SCHEDSTATS) */

#ifdef CONFIG_IRQ_TIME_ACCOUNTING

সংরচনা irqtime {
	u64			total;
	u64			tick_delta;
	u64			irq_start_time;
	সংরচনা u64_stats_sync	sync;
};

DECLARE_PER_CPU(সংরচনা irqtime, cpu_irqtime);
DECLARE_STATIC_চাবি_FALSE(sched_clock_irqtime);

স্থির অন্তঃস্থ পূর্ণাঙ্ক irqtime_enabled(শূন্য)
{
	প্রত্যাগম static_branch_likely(&sched_clock_irqtime);
}

/*
 * Returns the irqtime minus the softirq time computed by ksoftirqd.
 * Otherwise ksoftirqd's sum_exec_runtime is subtracted its own runtime
 * and never move forward.
 */
স্থির অন্তঃস্থ u64 irq_time_read(পূর্ণাঙ্ক cpu)
{
	সংরচনা irqtime *irqtime = &per_cpu(cpu_irqtime, cpu);
	unsigned পূর্ণাঙ্ক seq;
	u64 total;

	কুরু {
		seq = __u64_stats_fetch_begin(&irqtime->sync);
		total = irqtime->total;
	} যাবত্ (__u64_stats_fetch_retry(&irqtime->sync, seq));

	প্রত্যাগম total;
}

#অন্যথা /* !CONFIG_IRQ_TIME_ACCOUNTING: */

স্থির অন্তঃস্থ পূর্ণাঙ্ক irqtime_enabled(শূন্য)
{
	প্রত্যাগম 0;
}

#endif /* !CONFIG_IRQ_TIME_ACCOUNTING */

#ifdef CONFIG_CPU_FREQ

DECLARE_PER_CPU(সংরচনা update_util_data __rcu *, cpufreq_update_util_data);

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
স্থির অন্তঃস্থ শূন্য cpufreq_update_util(সংরচনা rq *rq, unsigned পূর্ণাঙ্ক flags)
{
	সংরচনা update_util_data *data;

	data = rcu_dereference_sched(*per_cpu_ptr(&cpufreq_update_util_data,
						  cpu_of(rq)));
	যদি (data)
		data->func(data, rq_clock(rq), flags);
}
#অন্যথা /* !CONFIG_CPU_FREQ: */
স্থির অন্তঃস্থ শূন্য cpufreq_update_util(সংরচনা rq *rq, unsigned পূর্ণাঙ্ক flags) { }
#endif /* !CONFIG_CPU_FREQ */

#ifdef arch_scale_freq_capacity
# ifndef arch_scale_freq_invariant
#  define arch_scale_freq_invariant()	true
# endif
#অন্যথা
# define arch_scale_freq_invariant()	false
#endif

unsigned long effective_cpu_util(পূর্ণাঙ্ক cpu, unsigned long util_cfs,
				 unsigned long *min,
				 unsigned long *max);

unsigned long sugov_effective_cpu_perf(পূর্ণাঙ্ক cpu, unsigned long actual,
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
স্থির অন্তঃস্থ bool dl_task_fits_capacity(সংরচনা task_struct *p, পূর্ণাঙ্ক cpu)
{
	unsigned long cap = arch_scale_cpu_capacity(cpu);

	প্রত্যাগম cap >= p->dl.dl_density >> (BW_SHIFT - SCHED_CAPACITY_SHIFT);
}

স্থির অন্তঃস্থ unsigned long cpu_bw_dl(সংরচনা rq *rq)
{
	প্রত্যাগম (rq->dl.running_bw * SCHED_CAPACITY_SCALE) >> BW_SHIFT;
}

স্থির অন্তঃস্থ unsigned long cpu_util_dl(সংরচনা rq *rq)
{
	প্রত্যাগম READ_ONCE(rq->avg_dl.util_avg);
}


বাহ্য unsigned long cpu_util_cfs(পূর্ণাঙ্ক cpu);
বাহ্য unsigned long cpu_util_cfs_boost(পূর্ণাঙ্ক cpu);

স্থির অন্তঃস্থ unsigned long cpu_util_rt(সংরচনা rq *rq)
{
	প্রত্যাগম READ_ONCE(rq->avg_rt.util_avg);
}

#ifdef CONFIG_UCLAMP_TASK

unsigned long uclamp_eff_মান(সংরচনা task_struct *p, গণনা uclamp_id clamp_id);

/*
 * When uclamp is compiled in, the aggregation at rq level is 'turned off'
 * by default in the fast path and only gets turned on once userspace performs
 * an operation that requires it.
 *
 * Returns true if userspace opted-in to use uclamp and aggregation at rq level
 * hence is active.
 */
স্থির অন্তঃস্থ bool uclamp_is_used(শূন্য)
{
	প্রত্যাগম static_branch_likely(&sched_uclamp_used);
}

/*
 * Enabling static branches would get the cpus_read_lock(),
 * check whether uclamp_is_used before enable it to avoid always
 * calling cpus_read_lock(). Because we never disable this
 * static key once enable it.
 */
স্থির অন্তঃস্থ শূন্য sched_uclamp_enable(শূন্য)
{
	যদি (!uclamp_is_used())
		static_branch_enable(&sched_uclamp_used);
}

স্থির অন্তঃস্থ unsigned long uclamp_rq_get(সংরচনা rq *rq,
					  গণনা uclamp_id clamp_id)
{
	প্রত্যাগম READ_ONCE(rq->uclamp[clamp_id].মান);
}

স্থির অন্তঃস্থ শূন্য uclamp_rq_set(সংরচনা rq *rq, গণনা uclamp_id clamp_id,
				 unsigned পূর্ণাঙ্ক মান)
{
	WRITE_ONCE(rq->uclamp[clamp_id].মান, মান);
}

স্থির অন্তঃস্থ bool uclamp_rq_is_idle(সংরচনা rq *rq)
{
	প্রত্যাগম rq->uclamp_flags & UCLAMP_FLAG_IDLE;
}

/* Is the rq being capped/throttled by uclamp_max? */
স্থির অন্তঃস্থ bool uclamp_rq_is_capped(সংরচনা rq *rq)
{
	unsigned long rq_util;
	unsigned long max_util;

	যদি (!uclamp_is_used())
		প্রত্যাগম false;

	rq_util = cpu_util_cfs(cpu_of(rq)) + cpu_util_rt(rq);
	max_util = READ_ONCE(rq->uclamp[UCLAMP_MAX].মান);

	প্রত্যাগম max_util != SCHED_CAPACITY_SCALE && rq_util >= max_util;
}

#define for_each_clamp_id(clamp_id) \
	প্রতি ((clamp_id) = 0; (clamp_id) < UCLAMP_CNT; (clamp_id)++)

বাহ্য unsigned পূর্ণাঙ্ক sysctl_sched_uclamp_util_min_rt_default;


স্থির অন্তঃস্থ unsigned পূর্ণাঙ্ক uclamp_none(গণনা uclamp_id clamp_id)
{
	যদি (clamp_id == UCLAMP_MIN)
		প্রত্যাগম 0;
	প্রত্যাগম SCHED_CAPACITY_SCALE;
}

/* Integer rounded range for each bucket */
#define UCLAMP_BUCKET_DELTA DIV_ROUND_CLOSEST(SCHED_CAPACITY_SCALE, UCLAMP_BUCKETS)

স্থির অন্তঃস্থ unsigned পূর্ণাঙ্ক uclamp_bucket_id(unsigned পূর্ণাঙ্ক clamp_মান)
{
	প্রত্যাগম min_t(unsigned পূর্ণাঙ্ক, clamp_মান / UCLAMP_BUCKET_DELTA, UCLAMP_BUCKETS - 1);
}

স্থির অন্তঃস্থ শূন্য
uclamp_se_set(সংরচনা uclamp_se *uc_se, unsigned পূর্ণাঙ্ক মান, bool user_defined)
{
	uc_se->মান = মান;
	uc_se->bucket_id = uclamp_bucket_id(মান);
	uc_se->user_defined = user_defined;
}

#অন্যথা /* !CONFIG_UCLAMP_TASK: */

স্থির অন্তঃস্থ unsigned long
uclamp_eff_মান(সংরচনা task_struct *p, গণনা uclamp_id clamp_id)
{
	যদি (clamp_id == UCLAMP_MIN)
		প্রত্যাগম 0;

	প্রত্যাগম SCHED_CAPACITY_SCALE;
}

স্থির অন্তঃস্থ bool uclamp_rq_is_capped(সংরচনা rq *rq) { প্রত্যাগম false; }

স্থির অন্তঃস্থ bool uclamp_is_used(শূন্য)
{
	প্রত্যাগম false;
}

স্থির অন্তঃস্থ শূন্য sched_uclamp_enable(শূন্য) {}

স্থির অন্তঃস্থ unsigned long
uclamp_rq_get(সংরচনা rq *rq, গণনা uclamp_id clamp_id)
{
	যদি (clamp_id == UCLAMP_MIN)
		প্রত্যাগম 0;

	প্রত্যাগম SCHED_CAPACITY_SCALE;
}

স্থির অন্তঃস্থ শূন্য
uclamp_rq_set(সংরচনা rq *rq, গণনা uclamp_id clamp_id, unsigned পূর্ণাঙ্ক মান)
{
}

স্থির অন্তঃস্থ bool uclamp_rq_is_idle(সংরচনা rq *rq)
{
	প্রত্যাগম false;
}

#endif /* !CONFIG_UCLAMP_TASK */

#ifdef CONFIG_HAVE_SCHED_AVG_IRQ

স্থির অন্তঃস্থ unsigned long cpu_util_irq(সংরচনা rq *rq)
{
	প্রত্যাগম READ_ONCE(rq->avg_irq.util_avg);
}

স্থির অন্তঃস্থ
unsigned long scale_irq_capacity(unsigned long util, unsigned long irq, unsigned long max)
{
	util *= (max - irq);
	util /= max;

	প্রত্যাগম util;

}

#অন্যথা /* !CONFIG_HAVE_SCHED_AVG_IRQ: */

স্থির অন্তঃস্থ unsigned long cpu_util_irq(সংরচনা rq *rq)
{
	প্রত্যাগম 0;
}

স্থির অন্তঃস্থ
unsigned long scale_irq_capacity(unsigned long util, unsigned long irq, unsigned long max)
{
	প্রত্যাগম util;
}

#endif /* !CONFIG_HAVE_SCHED_AVG_IRQ */

বাহ্য শূন্য __setparam_fair(সংরচনা task_struct *p, ধ্রুব সংরচনা sched_attr *attr);

#যদি defined(CONFIG_ENERGY_MODEL) && defined(CONFIG_CPU_FREQ_GOV_SCHEDUTIL)

#define perf_domain_span(pd) (to_cpumask(((pd)->em_pd->cpus)))

DECLARE_STATIC_চাবি_FALSE(sched_energy_present);

স্থির অন্তঃস্থ bool sched_energy_enabled(শূন্য)
{
	প্রত্যাগম static_branch_unlikely(&sched_energy_present);
}

#অন্যথা /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL): */

#define perf_domain_span(pd) রিক্ত

স্থির অন্তঃস্থ bool sched_energy_enabled(শূন্য) { প্রত্যাগম false; }

#endif /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL) */

#ifdef CONFIG_MEMBARRIER

/*
 * The scheduler provides memory barriers required by membarrier between:
 * - prior user-space memory accesses and store to rq->membarrier_state,
 * - store to rq->membarrier_state and following user-space memory accesses.
 * In the same way it provides those guarantees around store to rq->curr.
 */
স্থির অন্তঃস্থ শূন্য membarrier_switch_mm(সংরচনা rq *rq,
					সংরচনা mm_struct *prev_mm,
					সংরচনা mm_struct *next_mm)
{
	পূর্ণাঙ্ক membarrier_state;

	যদি (prev_mm == next_mm)
		প্রত্যাগম;

	membarrier_state = atomic_read(&next_mm->membarrier_state);
	যদি (READ_ONCE(rq->membarrier_state) == membarrier_state)
		প্রত্যাগম;

	WRITE_ONCE(rq->membarrier_state, membarrier_state);
}

#অন্যথা /* !CONFIG_MEMBARRIER: */

স্থির অন্তঃস্থ শূন্য membarrier_switch_mm(সংরচনা rq *rq,
					সংরচনা mm_struct *prev_mm,
					সংরচনা mm_struct *next_mm)
{
}

#endif /* !CONFIG_MEMBARRIER */

স্থির অন্তঃস্থ bool is_per_cpu_kthread(সংরচনা task_struct *p)
{
	যদি (!(p->flags & PF_KTHREAD))
		প্রত্যাগম false;

	যদি (p->nr_cpus_allowed != 1)
		প্রত্যাগম false;

	প্রত্যাগম true;
}

বাহ্য শূন্য swake_up_all_locked(সংরচনা swait_queue_head *q);
বাহ্য শূন্য __prepare_to_swait(সংরচনা swait_queue_head *q, সংরচনা swait_queue *wait);

বাহ্য পূর্ণাঙ্ক try_to_wake_up(সংরচনা task_struct *tsk, unsigned পূর্ণাঙ্ক state, পূর্ণাঙ্ক wake_flags);

#ifdef CONFIG_PREEMPT_DYNAMIC
বাহ্য পূর্ণাঙ্ক preempt_dynamic_mode;
বাহ্য পূর্ণাঙ্ক sched_dynamic_mode(ধ্রুব বর্ণ *str);
বাহ্য শূন্য sched_dynamic_update(পূর্ণাঙ্ক mode);
#endif
বাহ্য ধ্রুব বর্ণ *preempt_modes[];

#ifdef CONFIG_SCHED_MM_CID

স্থির __always_inline bool cid_on_cpu(unsigned পূর্ণাঙ্ক cid)
{
	প্রত্যাগম cid & MM_CID_ONCPU;
}

স্থির __always_inline bool cid_in_transit(unsigned পূর্ণাঙ্ক cid)
{
	প্রত্যাগম cid & MM_CID_TRANSIT;
}

স্থির __always_inline unsigned পূর্ণাঙ্ক cpu_cid_to_cid(unsigned পূর্ণাঙ্ক cid)
{
	প্রত্যাগম cid & ~MM_CID_ONCPU;
}

স্থির __always_inline unsigned পূর্ণাঙ্ক cid_to_cpu_cid(unsigned পূর্ণাঙ্ক cid)
{
	প্রত্যাগম cid | MM_CID_ONCPU;
}

স্থির __always_inline unsigned পূর্ণাঙ্ক cid_to_transit_cid(unsigned পূর্ণাঙ্ক cid)
{
	প্রত্যাগম cid | MM_CID_TRANSIT;
}

স্থির __always_inline unsigned পূর্ণাঙ্ক cid_from_transit_cid(unsigned পূর্ণাঙ্ক cid)
{
	প্রত্যাগম cid & ~MM_CID_TRANSIT;
}

স্থির __always_inline bool cid_on_task(unsigned পূর্ণাঙ্ক cid)
{
	/* True if none of the MM_CID_ONCPU, MM_CID_TRANSIT, MM_CID_UNSET bits is set */
	প্রত্যাগম cid < MM_CID_TRANSIT;
}

স্থির __always_inline শূন্য mm_drop_cid(সংরচনা mm_struct *mm, unsigned পূর্ণাঙ্ক cid)
{
	clear_bit(cid, mm_cidmask(mm));
}

স্থির __always_inline শূন্য mm_unset_cid_on_task(সংরচনা task_struct *t)
{
	unsigned পূর্ণাঙ্ক cid = t->mm_cid.cid;

	t->mm_cid.cid = MM_CID_UNSET;
	যদি (cid_on_task(cid))
		mm_drop_cid(t->mm, cid);
}

স্থির __always_inline শূন্য mm_drop_cid_on_cpu(সংরচনা mm_struct *mm, সংরচনা mm_cid_pcpu *pcp)
{
	/* Clear the ONCPU bit, but do not set UNSET in the per CPU storage */
	যদি (cid_on_cpu(pcp->cid)) {
		pcp->cid = cpu_cid_to_cid(pcp->cid);
		mm_drop_cid(mm, pcp->cid);
	}
}

স্থির অন্তঃস্থ unsigned পূর্ণাঙ্ক __mm_get_cid(সংরচনা mm_struct *mm, unsigned পূর্ণাঙ্ক max_cids)
{
	unsigned পূর্ণাঙ্ক cid = find_first_zero_bit(mm_cidmask(mm), max_cids);

	যদি (cid >= max_cids)
		প্রত্যাগম MM_CID_UNSET;
	যদি (test_and_set_bit(cid, mm_cidmask(mm)))
		প্রত্যাগম MM_CID_UNSET;
	প্রত্যাগম cid;
}

স্থির অন্তঃস্থ unsigned পূর্ণাঙ্ক mm_get_cid(সংরচনা mm_struct *mm)
{
	unsigned পূর্ণাঙ্ক cid = __mm_get_cid(mm, READ_ONCE(mm->mm_cid.max_cids));

	যাবত্ (cid == MM_CID_UNSET) {
		cpu_relax();
		cid = __mm_get_cid(mm, অঙ্ক_possible_cpus());
	}
	প্রত্যাগম cid;
}

স্থির অন্তঃস্থ unsigned পূর্ণাঙ্ক mm_cid_converge(সংরচনা mm_struct *mm, unsigned পূর্ণাঙ্ক orig_cid,
					   unsigned পূর্ণাঙ্ক max_cids)
{
	unsigned পূর্ণাঙ্ক new_cid, cid = cpu_cid_to_cid(orig_cid);

	/* Is it in the optimal CID space? */
	যদি (likely(cid < max_cids))
		প্রত্যাগম orig_cid;

	/* Try to find one in the optimal space. Otherwise keep the provided. */
	new_cid = __mm_get_cid(mm, max_cids);
	যদি (new_cid != MM_CID_UNSET) {
		mm_drop_cid(mm, cid);
		/* Preserve the ONCPU mode of the original CID */
		প্রত্যাগম new_cid | (orig_cid & MM_CID_ONCPU);
	}
	প্রত্যাগম orig_cid;
}

স্থির __always_inline শূন্য mm_cid_update_task_cid(সংরচনা task_struct *t, unsigned পূর্ণাঙ্ক cid)
{
	যদি (t->mm_cid.cid != cid) {
		t->mm_cid.cid = cid;
		rseq_sched_set_ids_changed(t);
	}
}

স্থির __always_inline শূন্য mm_cid_update_pcpu_cid(সংরচনা mm_struct *mm, unsigned পূর্ণাঙ্ক cid)
{
	__this_cpu_write(mm->mm_cid.pcpu->cid, cid);
}

স্থির __always_inline শূন্য mm_cid_from_cpu(সংরচনা task_struct *t, unsigned পূর্ণাঙ্ক cpu_cid,
					    unsigned পূর্ণাঙ্ক mode)
{
	unsigned পূর্ণাঙ্ক max_cids, tcid = t->mm_cid.cid;
	সংরচনা mm_struct *mm = t->mm;

	max_cids = READ_ONCE(mm->mm_cid.max_cids);
	/* Optimize for the common case where both have the ONCPU bit set */
	যদি (likely(cid_on_cpu(cpu_cid & tcid))) {
		যদি (likely(cpu_cid_to_cid(cpu_cid) < max_cids)) {
			mm_cid_update_task_cid(t, cpu_cid);
			প্রত্যাগম;
		}
		/* Try to converge into the optimal CID space */
		cpu_cid = mm_cid_converge(mm, cpu_cid, max_cids);
	} অন্যথা {
		/* Hand over or drop the task owned CID */
		যদি (cid_on_task(tcid)) {
			যদি (cid_on_cpu(cpu_cid))
				mm_unset_cid_on_task(t);
			অন্যথা
				cpu_cid = cid_to_cpu_cid(tcid);
		}
		/* Still nothing, allocate a new one */
		যদি (!cid_on_cpu(cpu_cid))
			cpu_cid = cid_to_cpu_cid(mm_get_cid(mm));

		/* Handle the transition mode flag if required */
		যদি (mode & MM_CID_TRANSIT)
			cpu_cid = cpu_cid_to_cid(cpu_cid) | MM_CID_TRANSIT;
	}
	mm_cid_update_pcpu_cid(mm, cpu_cid);
	mm_cid_update_task_cid(t, cpu_cid);
}

স্থির __always_inline শূন্য mm_cid_from_task(সংরচনা task_struct *t, unsigned পূর্ণাঙ্ক cpu_cid,
					     unsigned পূর্ণাঙ্ক mode)
{
	unsigned পূর্ণাঙ্ক max_cids, tcid = t->mm_cid.cid;
	সংরচনা mm_struct *mm = t->mm;

	max_cids = READ_ONCE(mm->mm_cid.max_cids);
	/* Optimize for the common case, where both have the ONCPU bit clear */
	যদি (likely(cid_on_task(tcid | cpu_cid))) {
		যদি (likely(tcid < max_cids)) {
			mm_cid_update_pcpu_cid(mm, tcid);
			প্রত্যাগম;
		}
		/* Try to converge into the optimal CID space */
		tcid = mm_cid_converge(mm, tcid, max_cids);
	} অন্যথা {
		/* Hand over or drop the CPU owned CID */
		যদি (cid_on_cpu(cpu_cid)) {
			যদি (cid_on_task(tcid))
				mm_drop_cid_on_cpu(mm, this_cpu_ptr(mm->mm_cid.pcpu));
			অন্যথা
				tcid = cpu_cid_to_cid(cpu_cid);
		}
		/* Still nothing, allocate a new one */
		যদি (!cid_on_task(tcid))
			tcid = mm_get_cid(mm);
		/* Set the transition mode flag if required */
		tcid |= mode & MM_CID_TRANSIT;
	}
	mm_cid_update_pcpu_cid(mm, tcid);
	mm_cid_update_task_cid(t, tcid);
}

স্থির __always_inline শূন্য mm_cid_schedin(সংরচনা task_struct *next)
{
	সংরচনা mm_struct *mm = next->mm;
	unsigned পূর্ণাঙ্ক cpu_cid, mode;

	যদি (!next->mm_cid.active)
		প্রত্যাগম;

	cpu_cid = __this_cpu_read(mm->mm_cid.pcpu->cid);
	mode = READ_ONCE(mm->mm_cid.mode);
	যদি (likely(!cid_on_cpu(mode)))
		mm_cid_from_task(next, cpu_cid, mode);
	অন্যথা
		mm_cid_from_cpu(next, cpu_cid, mode);
}

স্থির __always_inline শূন্য mm_cid_schedout(সংরচনা task_struct *prev)
{
	সংরচনা mm_struct *mm = prev->mm;
	unsigned পূর্ণাঙ্ক mode, cid;

	/* During mode transitions CIDs are temporary and need to be dropped */
	যদি (likely(!cid_in_transit(prev->mm_cid.cid)))
		প্রত্যাগম;

	mode = READ_ONCE(mm->mm_cid.mode);
	cid = cid_from_transit_cid(prev->mm_cid.cid);

	/*
	 * If transition mode is done, transfer ownership when the CID is
	 * within the convergence range to optimize the next schedule in.
	 */
	যদি (!cid_in_transit(mode) && cid < READ_ONCE(mm->mm_cid.max_cids)) {
		যদি (cid_on_cpu(mode))
			cid = cid_to_cpu_cid(cid);

		/* Update both so that the next schedule in goes into the fast path */
		mm_cid_update_pcpu_cid(mm, cid);
		prev->mm_cid.cid = cid;
	} অন্যথা {
		mm_drop_cid(mm, cid);
		prev->mm_cid.cid = MM_CID_UNSET;
	}
}

স্থির অন্তঃস্থ শূন্য mm_cid_switch_to(সংরচনা task_struct *prev, সংরচনা task_struct *next)
{
	mm_cid_schedout(prev);
	mm_cid_schedin(next);
}

#অন্যথা /* !CONFIG_SCHED_MM_CID: */
স্থির অন্তঃস্থ শূন্য mm_cid_switch_to(সংরচনা task_struct *prev, সংরচনা task_struct *next) { }
#endif /* !CONFIG_SCHED_MM_CID */

#ifdef CONFIG_SCHED_CACHE
DECLARE_STATIC_চাবি_FALSE(sched_cache_present);
DECLARE_STATIC_চাবি_FALSE(sched_cache_active);
বাহ্য পূর্ণাঙ্ক sysctl_sched_cache_user;
বাহ্য unsigned পূর্ণাঙ্ক llc_aggr_tolerance;
বাহ্য unsigned পূর্ণাঙ্ক llc_epoch_period;
বাহ্য unsigned পূর্ণাঙ্ক llc_epoch_affinity_timeout;
বাহ্য unsigned পূর্ণাঙ্ক llc_imb_pct;
বাহ্য unsigned পূর্ণাঙ্ক llc_overaggr_pct;

স্থির অন্তঃস্থ bool sched_cache_enabled(শূন্য)
{
	প্রত্যাগম static_branch_unlikely(&sched_cache_active);
}

বাহ্য শূন্য sched_cache_active_set(শূন্য);

#endif

শূন্য sched_domains_free_llc_id(পূর্ণাঙ্ক cpu);

বাহ্য শূন্য init_sched_mm(সংরচনা task_struct *p);

বাহ্য u64 avg_vruntime(সংরচনা cfs_rq *cfs_rq);
বাহ্য পূর্ণাঙ্ক entity_eligible(সংরচনা cfs_rq *cfs_rq, সংরচনা sched_entity *se);
স্থির অন্তঃস্থ
শূন্য move_queued_task_locked(সংরচনা rq *src_rq, সংরচনা rq *dst_rq, সংরচনা task_struct *task)
{
	lockdep_assert_rq_held(src_rq);
	lockdep_assert_rq_held(dst_rq);

	deactivate_task(src_rq, task, 0);
	set_task_cpu(task, dst_rq->cpu);
	activate_task(dst_rq, task, 0);
	wakeup_preempt(dst_rq, task, 0);
}

স্থির অন্তঃস্থ
bool task_is_pushable(সংরচনা rq *rq, সংরচনা task_struct *p, পূর্ণাঙ্ক cpu)
{
	যদি (!task_on_cpu(rq, p) &&
	    cpumask_test_cpu(cpu, &p->cpus_mask))
		প্রত্যাগম true;

	প্রত্যাগম false;
}

#ifdef CONFIG_RT_MUTEXES

স্থির অন্তঃস্থ পূর্ণাঙ্ক __rt_effective_prio(সংরচনা task_struct *pi_task, পূর্ণাঙ্ক prio)
{
	যদি (pi_task)
		prio = min(prio, pi_task->prio);

	প্রত্যাগম prio;
}

স্থির অন্তঃস্থ পূর্ণাঙ্ক rt_effective_prio(সংরচনা task_struct *p, পূর্ণাঙ্ক prio)
{
	সংরচনা task_struct *pi_task = rt_mutex_get_শীর্ষ_task(p);

	প্রত্যাগম __rt_effective_prio(pi_task, prio);
}

#অন্যথা /* !CONFIG_RT_MUTEXES: */

স্থির অন্তঃস্থ পূর্ণাঙ্ক rt_effective_prio(সংরচনা task_struct *p, পূর্ণাঙ্ক prio)
{
	প্রত্যাগম prio;
}

#endif /* !CONFIG_RT_MUTEXES */

বাহ্য পূর্ণাঙ্ক __sched_setscheduler(সংরচনা task_struct *p, ধ্রুব সংরচনা sched_attr *attr, bool user, bool pi);
বাহ্য পূর্ণাঙ্ক __sched_setaffinity(সংরচনা task_struct *p, সংরচনা affinity_context *ctx);
বাহ্য ধ্রুব সংরচনা sched_class *__setscheduler_class(পূর্ণাঙ্ক policy, পূর্ণাঙ্ক prio);
বাহ্য শূন্য set_load_weight(সংরচনা task_struct *p, bool update_load);
বাহ্য শূন্য enqueue_task(সংরচনা rq *rq, সংরচনা task_struct *p, পূর্ণাঙ্ক flags);
বাহ্য bool dequeue_task(সংরচনা rq *rq, সংরচনা task_struct *p, পূর্ণাঙ্ক flags);

বাহ্য সংরচনা balance_callback *splice_balance_callbacks(সংরচনা rq *rq);

বাহ্য শূন্য __balance_callbacks(সংরচনা rq *rq, সংরচনা rq_flags *rf);
বাহ্য শূন্য balance_callbacks(সংরচনা rq *rq, সংরচনা balance_callback *head);

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
সংরচনা sched_change_ctx {
	u64			prio;
	সংরচনা task_struct	*p;
	ধ্রুব সংরচনা sched_class *class;
	পূর্ণাঙ্ক			flags;
	bool			queued;
	bool			running;
};

সংরচনা sched_change_ctx *sched_change_begin(সংরচনা task_struct *p, unsigned পূর্ণাঙ্ক flags);
শূন্য sched_change_end(সংরচনা sched_change_ctx *ctx);

DEFINE_CLASS(sched_change, সংরচনা sched_change_ctx *,
	     sched_change_end(_T),
	     sched_change_begin(p, flags),
	     সংরচনা task_struct *p, unsigned পূর্ণাঙ্ক flags)

DEFINE_CLASS_IS_UNCONDITIONAL(sched_change)

#অন্তর্ভাব "ext/ext.h"

#ifdef CONFIG_PREFERRED_CPU
শূন্য sched_ঠেলো_current_non_preferred_cpu(সংরচনা rq *rq);
#অন্যথা	/* !CONFIG_PREFERRED_CPU */
স্থির অন্তঃস্থ শূন্য sched_ঠেলো_current_non_preferred_cpu(সংরচনা rq *rq) { }
#endif

#endif /* _KERNEL_SCHED_SCHED_H */
