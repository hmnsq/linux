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

কাঠামো rq;
কাঠামো cfs_rq;
কাঠামো rt_rq;
কাঠামো sched_group;
কাঠামো cpuidle_state;

#যদি defined(CONFIG_PARAVIRT) && !defined(CONFIG_HAVE_PV_STEAL_CLOCK_GEN)
# include <asm/paravirt.h>
#endif

#অন্তর্ভাব <asm/barrier.h>

#অন্তর্ভাব "cpupri.h"
#অন্তর্ভাব "cpudeadline.h"

/* task_struct::on_rq states: */
#define TASK_ON_RQ_QUEUED	1
#define TASK_ON_RQ_MIGRATING	2

বাহ্যিক __read_mostly পূর্ণসংখ্যা scheduler_running;

বাহ্যিক unsigned long calc_load_update;
বাহ্যিক atomic_long_t calc_load_tasks;

বাহ্যিক শূন্য calc_global_load_tick(কাঠামো rq *this_rq);
বাহ্যিক long calc_load_fold_active(কাঠামো rq *this_rq, long adjust);

বাহ্যিক শূন্য call_trace_sched_update_nr_running(কাঠামো rq *rq, পূর্ণসংখ্যা গুনতি);

বাহ্যিক পূর্ণসংখ্যা sysctl_sched_rt_period;
বাহ্যিক পূর্ণসংখ্যা sysctl_sched_rt_runtime;
বাহ্যিক পূর্ণসংখ্যা sched_rr_timeslice;

/*
 * Asymmetric CPU capacity bits
 */
কাঠামো asym_cap_তথ্য {
	কাঠামো list_শীর্ষ link;
	কাঠামো rcu_শীর্ষ rcu;
	unsigned long capacity;
	unsigned long cpus[];
};

বাহ্যিক কাঠামো list_শীর্ষ asym_cap_list;

#define cpu_capacity_span(asym_তথ্য) to_cpumask((asym_তথ্য)->cpus)

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

স্থির অন্তঃরেখ পূর্ণসংখ্যা idle_policy(পূর্ণসংখ্যা policy)
{
	ফেরত policy == SCHED_IDLE;
}

স্থির অন্তঃরেখ পূর্ণসংখ্যা normal_policy(পূর্ণসংখ্যা policy)
{
#ifdef CONFIG_SCHED_CLASS_EXT
	যদি (policy == SCHED_EXT)
		ফেরত true;
#endif
	ফেরত policy == SCHED_NORMAL;
}

স্থির অন্তঃরেখ পূর্ণসংখ্যা fair_policy(পূর্ণসংখ্যা policy)
{
	ফেরত normal_policy(policy) || policy == SCHED_BATCH;
}

স্থির অন্তঃরেখ পূর্ণসংখ্যা rt_policy(পূর্ণসংখ্যা policy)
{
	ফেরত policy == SCHED_FIFO || policy == SCHED_RR;
}

স্থির অন্তঃরেখ পূর্ণসংখ্যা dl_policy(পূর্ণসংখ্যা policy)
{
	ফেরত policy == SCHED_DEADLINE;
}

স্থির অন্তঃরেখ bool valid_policy(পূর্ণসংখ্যা policy)
{
	ফেরত idle_policy(policy) || fair_policy(policy) ||
		rt_policy(policy) || dl_policy(policy);
}

স্থির অন্তঃরেখ পূর্ণসংখ্যা task_has_idle_policy(কাঠামো task_struct *p)
{
	ফেরত idle_policy(p->policy);
}

স্থির অন্তঃরেখ পূর্ণসংখ্যা task_has_rt_policy(কাঠামো task_struct *p)
{
	ফেরত rt_policy(p->policy);
}

স্থির অন্তঃরেখ পূর্ণসংখ্যা task_has_dl_policy(কাঠামো task_struct *p)
{
	ফেরত dl_policy(p->policy);
}

#define cap_scale(v, s)		((v)*(s) >> SCHED_CAPACITY_SHIFT)

স্থির অন্তঃরেখ শূন্য update_avg(u64 *avg, u64 sample)
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
স্থির অন্তঃরেখ unsigned long sched_weight_from_cgroup(unsigned long cgrp_weight)
{
	ফেরত DIV_ROUND_CLOSEST_ULL(cgrp_weight * 1024, CGROUP_WEIGHT_DFL);
}

স্থির অন্তঃরেখ unsigned long sched_weight_to_cgroup(unsigned long weight)
{
	ফেরত clamp_t(unsigned long,
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

স্থির অন্তঃরেখ bool dl_entity_is_special(ধ্রুবক কাঠামো sched_dl_entity *dl_se)
{
#ifdef CONFIG_CPU_FREQ_GOV_SCHEDUTIL
	ফেরত unlikely(dl_se->flags & SCHED_FLAG_SUGOV);
#অন্যথা
	ফেরত false;
#endif
}

/*
 * Tells if entity @a should preempt entity @b.
 */
স্থির অন্তঃরেখ bool dl_entity_preempt(ধ্রুবক কাঠামো sched_dl_entity *a,
				     ধ্রুবক কাঠামো sched_dl_entity *b)
{
	ফেরত dl_entity_is_special(a) ||
	       dl_time_before(a->deadline, b->deadline);
}

/*
 * This is the priority-queue data structure of the RT scheduling class:
 */
কাঠামো rt_prio_array {
	DECLARE_BITMAP(bitmap, MAX_RT_PRIO+1); /* include 1 bit for delimiter */
	কাঠামো list_শীর্ষ queue[MAX_RT_PRIO];
};

কাঠামো rt_bandwidth {
	/* nests inside the rq lock: */
	raw_spinlock_t		rt_runtime_lock;
	ktime_t			rt_period;
	u64			rt_runtime;
	কাঠামো hrtimer		rt_period_timer;
	unsigned পূর্ণসংখ্যা		rt_period_active;
};

স্থির অন্তঃরেখ পূর্ণসংখ্যা dl_bandwidth_enabled(শূন্য)
{
	ফেরত sysctl_sched_rt_runtime >= 0;
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
কাঠামো dl_bw {
	raw_spinlock_t		lock;
	u64			bw;
	u64			যোগ_bw;
};

বাহ্যিক শূন্য init_dl_bw(কাঠামো dl_bw *dl_b);
বাহ্যিক পূর্ণসংখ্যা  sched_dl_global_validate(শূন্য);
বাহ্যিক শূন্য sched_dl_do_global(শূন্য);
বাহ্যিক পূর্ণসংখ্যা  sched_dl_overflow(কাঠামো task_struct *p, পূর্ণসংখ্যা policy, ধ্রুবক কাঠামো sched_attr *attr);
বাহ্যিক শূন্য __setparam_dl(কাঠামো task_struct *p, ধ্রুবক কাঠামো sched_attr *attr);
বাহ্যিক শূন্য __getparam_dl(কাঠামো task_struct *p, কাঠামো sched_attr *attr, unsigned পূর্ণসংখ্যা flags);
বাহ্যিক bool __checkparam_dl(ধ্রুবক কাঠামো sched_attr *attr);
বাহ্যিক bool dl_param_changed(কাঠামো task_struct *p, ধ্রুবক কাঠামো sched_attr *attr);
বাহ্যিক পূর্ণসংখ্যা  dl_cpuset_cpumask_can_shrink(ধ্রুবক কাঠামো cpumask *cur, ধ্রুবক কাঠামো cpumask *trial);
বাহ্যিক পূর্ণসংখ্যা  dl_bw_deactivate(পূর্ণসংখ্যা cpu);
বাহ্যিক s64 dl_scaled_delta_exec(কাঠামো rq *rq, কাঠামো sched_dl_entity *dl_se, s64 delta_exec);
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
বাহ্যিক শূন্য dl_server_update_idle(কাঠামো sched_dl_entity *dl_se, s64 delta_exec);
বাহ্যিক শূন্য dl_server_update(কাঠামো sched_dl_entity *dl_se, s64 delta_exec);
বাহ্যিক শূন্য dl_server_start(কাঠামো sched_dl_entity *dl_se);
বাহ্যিক শূন্য dl_server_stop(কাঠামো sched_dl_entity *dl_se);
বাহ্যিক শূন্য dl_server_init(কাঠামো sched_dl_entity *dl_se, কাঠামো rq *rq,
		    dl_server_pick_f pick_task);
বাহ্যিক শূন্য sched_init_dl_servers(শূন্য);

বাহ্যিক শূন্য fair_server_init(কাঠামো rq *rq);
বাহ্যিক শূন্য ext_server_init(কাঠামো rq *rq);
বাহ্যিক শূন্য __dl_server_attach_root(কাঠামো sched_dl_entity *dl_se, কাঠামো rq *rq);
বাহ্যিক পূর্ণসংখ্যা dl_server_apply_params(কাঠামো sched_dl_entity *dl_se,
		    u64 runtime, u64 period, bool init);
বাহ্যিক পূর্ণসংখ্যা dl_server_attach_bw(কাঠামো sched_dl_entity *dl_se);
বাহ্যিক শূন্য dl_server_detach_bw(কাঠামো sched_dl_entity *dl_se);
বাহ্যিক পূর্ণসংখ্যা dl_server_swap_bw(কাঠামো sched_dl_entity *detach_se,
			     কাঠামো sched_dl_entity *attach_se);

স্থির অন্তঃরেখ bool dl_server_active(কাঠামো sched_dl_entity *dl_se)
{
	ফেরত dl_se->dl_server_active;
}

#ifdef CONFIG_CGROUP_SCHED

বাহ্যিক কাঠামো list_শীর্ষ task_groups;

#ifdef CONFIG_GROUP_SCHED_BANDWIDTH
বাহ্যিক ধ্রুবক u64 max_bw_quota_period_us;

/*
 * default period for group bandwidth.
 * default: 0.1s, units: microseconds
 */
স্থির অন্তঃরেখ u64 default_bw_period_us(শূন্য)
{
	ফেরত 100000ULL;
}
#endif /* CONFIG_GROUP_SCHED_BANDWIDTH */

কাঠামো cfs_bandwidth {
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
	কাঠামো hrtimer		period_timer;
	কাঠামো hrtimer		slack_timer;
	কাঠামো list_শীর্ষ	throttled_cfs_rq;

	/* Statistics: */
	পূর্ণসংখ্যা			nr_periods;
	পূর্ণসংখ্যা			nr_throttled;
	পূর্ণসংখ্যা			nr_burst;
	u64			throttled_time;
	u64			burst_time;
#endif /* CONFIG_CFS_BANDWIDTH */
};

/* Task group related information */
কাঠামো task_group {
	কাঠামো cgroup_subsys_state css;

#ifdef CONFIG_GROUP_SCHED_WEIGHT
	/* A positive value indicates that this is a SCHED_IDLE group. */
	পূর্ণসংখ্যা			idle;
#endif

#ifdef CONFIG_FAIR_GROUP_SCHED
	/* runqueue "owned" by this group on each CPU */
	কাঠামো cfs_rq __percpu	*cfs_rq;
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
	কাঠামো sched_rt_entity	**rt_se;
	কাঠামো rt_rq		**rt_rq;

	কাঠামো rt_bandwidth	rt_bandwidth;
#endif

	কাঠামো scx_task_group	scx;

	কাঠামো rcu_শীর্ষ		rcu;
	কাঠামো list_শীর্ষ	list;

	কাঠামো task_group	*parent;
	কাঠামো list_শীর্ষ	siblings;
	কাঠামো list_শীর্ষ	children;

#ifdef CONFIG_SCHED_AUTOGROUP
	কাঠামো autogroup	*autogroup;
#endif

	কাঠামো cfs_bandwidth	cfs_bandwidth;

#ifdef CONFIG_UCLAMP_TASK_GROUP
	/* The two decimal precision [%] value requested from user-space */
	unsigned পূর্ণসংখ্যা		uclamp_pct[UCLAMP_CNT];
	/* Clamp values requested for a task group */
	কাঠামো uclamp_se	uclamp_req[UCLAMP_CNT];
	/* Effective clamp values used for a task group */
	কাঠামো uclamp_se	uclamp[UCLAMP_CNT];
#endif

};

#define মূল_TASK_GROUP_LOAD	NICE_0_LOAD

প্রকারলেখ পূর্ণসংখ্যা (*tg_visitor)(কাঠামো task_group *, শূন্য *);

বাহ্যিক পূর্ণসংখ্যা walk_tg_বৃক্ষ_from(কাঠামো task_group *from,
			     tg_visitor down, tg_visitor up, শূন্য *তথ্য);

/*
 * Iterate the full tree, calling @down when first entering a node and @up when
 * leaving it for the final time.
 *
 * Caller must hold rcu_lock or sufficient equivalent.
 */
স্থির অন্তঃরেখ পূর্ণসংখ্যা walk_tg_বৃক্ষ(tg_visitor down, tg_visitor up, শূন্য *তথ্য)
{
	ফেরত walk_tg_বৃক্ষ_from(&মূল_task_group, down, up, তথ্য);
}

স্থির অন্তঃরেখ কাঠামো task_group *css_tg(কাঠামো cgroup_subsys_state *css)
{
	ফেরত css ? container_of(css, কাঠামো task_group, css) : ফাঁকা;
}

বাহ্যিক পূর্ণসংখ্যা tg_nop(কাঠামো task_group *tg, শূন্য *তথ্য);

#ifdef CONFIG_FAIR_GROUP_SCHED
বাহ্যিক শূন্য free_fair_sched_group(কাঠামো task_group *tg);
বাহ্যিক পূর্ণসংখ্যা alloc_fair_sched_group(কাঠামো task_group *tg, কাঠামো task_group *parent);
বাহ্যিক শূন্য online_fair_sched_group(কাঠামো task_group *tg);
বাহ্যিক শূন্য unregister_fair_sched_group(কাঠামো task_group *tg);
বাহ্যিক শূন্য __sched_cgroup_mode_update(পূর্ণসংখ্যা mode);
#অন্যথা /* !CONFIG_FAIR_GROUP_SCHED: */
স্থির অন্তঃরেখ শূন্য free_fair_sched_group(কাঠামো task_group *tg) { }
স্থির অন্তঃরেখ পূর্ণসংখ্যা alloc_fair_sched_group(কাঠামো task_group *tg, কাঠামো task_group *parent)
{
       ফেরত 1;
}
স্থির অন্তঃরেখ শূন্য online_fair_sched_group(কাঠামো task_group *tg) { }
স্থির অন্তঃরেখ শূন্য unregister_fair_sched_group(কাঠামো task_group *tg) { }
#endif /* !CONFIG_FAIR_GROUP_SCHED */

বাহ্যিক শূন্য init_tg_cfs_entry(কাঠামো task_group *tg, কাঠামো cfs_rq *cfs_rq,
			কাঠামো sched_entity *se, পূর্ণসংখ্যা cpu,
			কাঠামো sched_entity *parent);
বাহ্যিক শূন্য init_cfs_bandwidth(কাঠামো cfs_bandwidth *cfs_b, কাঠামো cfs_bandwidth *parent);

বাহ্যিক শূন্য __refill_cfs_bandwidth_runtime(কাঠামো cfs_bandwidth *cfs_b);
বাহ্যিক শূন্য start_cfs_bandwidth(কাঠামো cfs_bandwidth *cfs_b);
বাহ্যিক শূন্য unthrottle_cfs_rq(কাঠামো cfs_rq *cfs_rq);
বাহ্যিক bool cfs_task_bw_constrained(কাঠামো task_struct *p);

বাহ্যিক শূন্য init_tg_rt_entry(কাঠামো task_group *tg, কাঠামো rt_rq *rt_rq,
		কাঠামো sched_rt_entity *rt_se, পূর্ণসংখ্যা cpu,
		কাঠামো sched_rt_entity *parent);
বাহ্যিক পূর্ণসংখ্যা sched_group_set_rt_runtime(কাঠামো task_group *tg, long rt_runtime_us);
বাহ্যিক পূর্ণসংখ্যা sched_group_set_rt_period(কাঠামো task_group *tg, u64 rt_period_us);
বাহ্যিক long sched_group_rt_runtime(কাঠামো task_group *tg);
বাহ্যিক long sched_group_rt_period(কাঠামো task_group *tg);
বাহ্যিক পূর্ণসংখ্যা sched_rt_can_attach(কাঠামো task_group *tg, কাঠামো task_struct *tsk);

বাহ্যিক কাঠামো task_group *sched_create_group(কাঠামো task_group *parent);
বাহ্যিক শূন্য sched_online_group(কাঠামো task_group *tg,
			       কাঠামো task_group *parent);
বাহ্যিক শূন্য sched_destroy_group(কাঠামো task_group *tg);
বাহ্যিক শূন্য sched_release_group(কাঠামো task_group *tg);

বাহ্যিক শূন্য sched_move_task(কাঠামো task_struct *tsk, bool for_autogroup);

#ifdef CONFIG_FAIR_GROUP_SCHED
বাহ্যিক পূর্ণসংখ্যা sched_group_set_shares(কাঠামো task_group *tg, unsigned long shares);

বাহ্যিক পূর্ণসংখ্যা sched_group_set_idle(কাঠামো task_group *tg, long idle);

বাহ্যিক শূন্য set_task_rq_fair(কাঠামো sched_entity *se,
			     কাঠামো cfs_rq *পূর্ব, কাঠামো cfs_rq *পরবর্তী);
#অন্যথা /* !CONFIG_FAIR_GROUP_SCHED: */
স্থির অন্তঃরেখ পূর্ণসংখ্যা sched_group_set_shares(কাঠামো task_group *tg, unsigned long shares) { ফেরত 0; }
স্থির অন্তঃরেখ পূর্ণসংখ্যা sched_group_set_idle(কাঠামো task_group *tg, long idle) { ফেরত 0; }
#endif /* !CONFIG_FAIR_GROUP_SCHED */

#অন্যথা /* !CONFIG_CGROUP_SCHED: */

কাঠামো cfs_bandwidth { };

স্থির অন্তঃরেখ bool cfs_task_bw_constrained(কাঠামো task_struct *p) { ফেরত false; }

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

বাহ্যিক শূন্য unregister_rt_sched_group(কাঠামো task_group *tg);
বাহ্যিক শূন্য free_rt_sched_group(কাঠামো task_group *tg);
বাহ্যিক পূর্ণসংখ্যা alloc_rt_sched_group(কাঠামো task_group *tg, কাঠামো task_group *parent);

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
	করো {								\
		__val_copy = copy;					\
		/*							\
		 * paired with u64_u32_store_copy(), ordering access	\
		 * to var and copy.					\
		 */							\
		smp_rmb();						\
		__val = var;						\
	} যখন (__val != __val_copy);					\
	__val;								\
})
# define u64_u32_store_copy(var, copy, val)				\
করো {									\
	typeof(val) __val = (val);					\
	var = __val;							\
	/*								\
	 * paired with u64_u32_load_copy(), ordering access to var and	\
	 * copy.							\
	 */								\
	smp_wmb();							\
	copy = __val;							\
} যখন (0)
#endif
# define u64_u32_load(var)		u64_u32_load_copy(var, var##_copy)
# define u64_u32_store(var, val)	u64_u32_store_copy(var, var##_copy, val)

কাঠামো balance_callback {
	কাঠামো balance_callback *পরবর্তী;
	শূন্য (*func)(কাঠামো rq *rq);
};

/* Fair scheduling SCHED_{NORMAL,BATCH,IDLE} related fields in a runqueue: */
কাঠামো cfs_rq {
	কাঠামো load_weight	load;
	unsigned পূর্ণসংখ্যা		nr_queued;
	unsigned পূর্ণসংখ্যা		h_nr_queued;		/* SCHED_{NORMAL,BATCH,IDLE} */
	unsigned পূর্ণসংখ্যা		h_nr_runnable;		/* SCHED_{NORMAL,BATCH,IDLE} */
	unsigned পূর্ণসংখ্যা		h_nr_idle;		/* SCHED_IDLE */

	s64			যোগ_w_vruntime;
	u64			যোগ_weight;
	u64			zero_vruntime;
	unsigned পূর্ণসংখ্যা		যোগ_shift;

#ifdef CONFIG_SCHED_CORE
	unsigned পূর্ণসংখ্যা		forceidle_seq;
	u64			zero_vruntime_fi;
#endif

	কাঠামো rb_মূল_cached	tasks_timeline;

	/*
	 * 'curr' points to the currently running entity on this cfs_rq.
	 * It is set to NULL otherwise (i.e when none are currently running).
	 */
	কাঠামো sched_entity	*curr;
	কাঠামো sched_entity	*পরবর্তী;

	/*
	 * CFS load tracking
	 */
	কাঠামো sched_entity	*h_curr;
	কাঠামো sched_avg	avg;
#ifndef CONFIG_64BIT
	u64			last_update_time_copy;
#endif
	কাঠামো {
		raw_spinlock_t	lock ____cacheline_aligned;
		পূর্ণসংখ্যা		nr;
		unsigned long	load_avg;
		unsigned long	util_avg;
		unsigned long	runnable_avg;
	} removed;

#ifdef CONFIG_FAIR_GROUP_SCHED
	u64			last_update_tg_load_avg;
	unsigned long		tg_load_avg_contrib;
	unsigned long		tg_runnable_avg_contrib;
	long			propagate;
	long			prop_runnable_যোগ;

	/*
	 *   h_load = weight * f(tg)
	 *
	 * Where f(tg) is the recursive weight fraction assigned to
	 * this group.
	 */
	unsigned long		h_load;
	u64			last_h_load_update;
	কাঠামো sched_entity	*h_load_পরবর্তী;

	কাঠামো rq		*rq;	/* CPU runqueue to which this cfs_rq is attached */

	/*
	 * leaf cfs_rqs are those that hold tasks (lowest schedulable entity in
	 * a hierarchy). Non-leaf lrqs hold other higher schedulable entities
	 * (like users, containers etc.)
	 *
	 * leaf_cfs_rq_list ties together list of leaf cfs_rq's in a CPU.
	 * This list is used during load balance.
	 */
	পূর্ণসংখ্যা			on_list;
	কাঠামো list_শীর্ষ	leaf_cfs_rq_list;
	কাঠামো task_group	*tg;	/* Group that "owns" this runqueue */

	/* Locally cached copy of our task_group's idle value */
	পূর্ণসংখ্যা			idle;

# ifdef CONFIG_CFS_BANDWIDTH
	পূর্ণসংখ্যা			runtime_enabled;
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
	পূর্ণসংখ্যা			throttle_গুনতি;
	কাঠামো list_শীর্ষ	throttled_list;
	কাঠামো list_শীর্ষ	throttled_csd_list;
	কাঠামো list_শীর্ষ        throttled_limbo_list;
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
কাঠামো scx_rq_rescue {
	কাঠামো scx_dispatch_q	dsq;			/* stranded tasks awaiting rescue */
	s64			budget;			/* execution token bucket, ns */
	u64			clock;			/* last budget accrual timestamp */
	কাঠামো task_struct	*curr;			/* task being rescued, one at a time */
	s64			slice;			/* curr's admitted slice */
	u64			exec_snap;		/* sum_exec_runtime at admission */
	কাঠামো timer_list	timer;			/* paces admission and escalation */
	u64			kill_at;		/* last ejection, init before any */
};

কাঠামো scx_rq {
	কাঠামো scx_dispatch_q	local_dsq;
#ifdef CONFIG_EXT_SUB_SCHED
	কাঠামো scx_dispatch_q	reject_dsq;		/* staging for cap-rejected tasks */
	কাঠামো scx_rq_rescue	rescue;
#endif
	কাঠামো list_শীর্ষ	runnable_list;		/* runnable tasks on this rq */
	কাঠামো list_শীর্ষ	ddsp_deferred_locals;	/* deferred ddsps from enq */
	/* both stashed across the activate_task() in move_remote_task_to_local_dsq() */
	u64			remote_activate_enq_flags;
	কাঠামো scx_sched	*remote_activate_sch;
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
	কাঠামো llist_শীর্ষ	ecaps_to_sync;		/* pending ecaps syncs */
	কাঠামো task_struct	*sub_dispatch_পূর্ব;
#endif
	cpumask_var_t		cpus_to_sync;
	bool			kick_sync_pending;
	unsigned long		kick_sync;

	কাঠামো list_শীর্ষ	sched_pcpus_to_kick;	/* see kick_cpus_irq_workfn() */

	raw_spinlock_t		deferred_reenq_lock;
	কাঠামো list_শীর্ষ	deferred_reenq_locals;	/* scheds requesting reenq of local DSQ */
	কাঠামো list_শীর্ষ	deferred_reenq_users;	/* user DSQs requesting reenq */
	কাঠামো balance_callback	deferred_bal_cb;
	কাঠামো balance_callback	kick_sync_bal_cb;
	কাঠামো irq_work		deferred_irq_work;
	কাঠামো irq_work		kick_cpus_irq_work;
};
#endif /* CONFIG_SCHED_CLASS_EXT */

স্থির অন্তঃরেখ পূর্ণসংখ্যা rt_bandwidth_enabled(শূন্য)
{
	ফেরত sysctl_sched_rt_runtime >= 0;
}

/* RT IPI pull logic requires IRQ_WORK */
#যদি defined(CONFIG_IRQ_WORK) && defined(CONFIG_SMP)
# define HAVE_RT_ঠেলো_IPI
#endif

/* Real-Time classes' related field in a runqueue: */
কাঠামো rt_rq {
	কাঠামো rt_prio_array	active;
	unsigned পূর্ণসংখ্যা		rt_nr_running;
	unsigned পূর্ণসংখ্যা		rr_nr_running;
	কাঠামো {
		পূর্ণসংখ্যা		curr; /* highest queued rt task prio */
		পূর্ণসংখ্যা		পরবর্তী; /* next highest */
	} highest_prio;
	bool			overloaded;
	কাঠামো plist_শীর্ষ	pushable_tasks;

	পূর্ণসংখ্যা			rt_queued;

#ifdef CONFIG_RT_GROUP_SCHED
	পূর্ণসংখ্যা			rt_throttled;
	u64			rt_time; /* consumed RT time, goes up in update_curr_rt */
	u64			rt_runtime; /* allotted RT time, "slice" from rt_bandwidth, RT sharing/balancing */
	/* Nests inside the rq lock: */
	raw_spinlock_t		rt_runtime_lock;

	unsigned পূর্ণসংখ্যা		rt_nr_boosted;

	কাঠামো rq		*rq; /* this is always top-level rq, cache? */
#endif
#ifdef CONFIG_CGROUP_SCHED
	কাঠামো task_group	*tg; /* this tg has "this" rt_rq on given CPU for runnable entities */
#endif
};

স্থির অন্তঃরেখ bool rt_rq_is_runnable(কাঠামো rt_rq *rt_rq)
{
	ফেরত rt_rq->rt_queued && rt_rq->rt_nr_running;
}

/* Deadline class' related fields in a runqueue */
কাঠামো dl_rq {
	/* runqueue is an rbtree, ordered by deadline */
	কাঠামো rb_মূল_cached	মূল;

	unsigned পূর্ণসংখ্যা		dl_nr_running;

	/*
	 * Deadline values of the currently executing and the
	 * earliest ready task on this rq. Caching these facilitates
	 * the decision whether or not a ready but not running task
	 * should migrate somewhere else.
	 */
	কাঠামো {
		u64		curr;
		u64		পরবর্তী;
	} earliest_dl;

	bool			overloaded;

	কাঠামো sched_dl_entity	*curr;
	/*
	 * Tasks on this rq that can be pushed away. They are kept in
	 * an rb-tree, ordered by tasks' deadlines, with caching
	 * of the leftmost (earliest deadline) element.
	 */
	কাঠামো rb_মূল_cached	pushable_dl_tasks_মূল;

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

স্থির অন্তঃরেখ শূন্য se_update_runnable(কাঠামো sched_entity *se)
{
	যদি (!entity_is_task(se))
		se->runnable_weight = se->my_q->h_nr_runnable;
}

স্থির অন্তঃরেখ long se_runnable(কাঠামো sched_entity *se)
{
	যদি (se->sched_delayed)
		ফেরত false;

	যদি (entity_is_task(se))
		ফেরত !!se->on_rq;
	অন্যথা
		ফেরত se->runnable_weight;
}

#অন্যথা /* !CONFIG_FAIR_GROUP_SCHED: */

#define entity_is_task(se)	1

স্থির অন্তঃরেখ শূন্য se_update_runnable(কাঠামো sched_entity *se) { }

স্থির অন্তঃরেখ long se_runnable(কাঠামো sched_entity *se)
{
	যদি (se->sched_delayed)
		ফেরত false;

	ফেরত !!se->on_rq;
}

#endif /* !CONFIG_FAIR_GROUP_SCHED */

/*
 * XXX we want to get rid of these helpers and use the full load resolution.
 */
স্থির অন্তঃরেখ long se_weight(কাঠামো sched_entity *se)
{
	ফেরত scale_load_down(se->load.weight);
}


স্থির অন্তঃরেখ bool sched_asym_prefer(পূর্ণসংখ্যা a, পূর্ণসংখ্যা b)
{
	ফেরত arch_asym_cpu_priority(a) > arch_asym_cpu_priority(b);
}

কাঠামো perf_domain {
	কাঠামো em_perf_domain *em_pd;
	কাঠামো perf_domain *পরবর্তী;
	কাঠামো rcu_শীর্ষ rcu;
};

/*
 * We add the notion of a root-domain which will be used to define per-domain
 * variables. Each exclusive cpuset essentially defines an island domain by
 * fully partitioning the member CPUs from any other cpuset. Whenever a new
 * exclusive cpuset is created, we also create and attach a new root-domain
 * object.
 *
 */
কাঠামো মূল_domain {
	atomic_t		refcount;
	atomic_t		rto_গুনতি;
	কাঠামো rcu_শীর্ষ		rcu;
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
	কাঠামো dl_bw		dl_bw;
	কাঠামো cpudl		cpudl;

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
	কাঠামো irq_work		rto_ঠেলো_work;
	raw_spinlock_t		rto_lock;
	/* These are only updated and read within rto_lock */
	পূর্ণসংখ্যা			rto_loop;
	পূর্ণসংখ্যা			rto_cpu;
	/* These atomics are updated outside of a lock */
	atomic_t		rto_loop_পরবর্তী;
	atomic_t		rto_loop_start;
#endif /* HAVE_RT_PUSH_IPI */
	/*
	 * The "RT overload" flag: it gets set if a CPU has more than
	 * one runnable RT task.
	 */
	cpumask_var_t		rto_mask;
	কাঠামো cpupri		cpupri;

	/*
	 * NULL-terminated list of performance domains intersecting with the
	 * CPUs of the rd. Protected by RCU.
	 */
	কাঠামো perf_domain __rcu *pd;
};

বাহ্যিক শূন্য init_defrootdomain(শূন্য);
বাহ্যিক পূর্ণসংখ্যা sched_init_domains(ধ্রুবক কাঠামো cpumask *cpu_map);
বাহ্যিক শূন্য rq_attach_মূল(কাঠামো rq *rq, কাঠামো মূল_domain *rd);
বাহ্যিক শূন্য sched_get_rd(কাঠামো মূল_domain *rd);
বাহ্যিক শূন্য sched_put_rd(কাঠামো মূল_domain *rd);

স্থির অন্তঃরেখ পূর্ণসংখ্যা get_rd_overloaded(কাঠামো মূল_domain *rd)
{
	ফেরত READ_ONCE(rd->overloaded);
}

স্থির অন্তঃরেখ শূন্য set_rd_overloaded(কাঠামো মূল_domain *rd, পূর্ণসংখ্যা status)
{
	যদি (get_rd_overloaded(rd) != status)
		WRITE_ONCE(rd->overloaded, status);
}

#ifdef HAVE_RT_ঠেলো_IPI
বাহ্যিক শূন্য rto_ঠেলো_irq_work_func(কাঠামো irq_work *work);
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
কাঠামো uclamp_bucket {
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
কাঠামো uclamp_rq {
	unsigned পূর্ণসংখ্যা মান;
	কাঠামো uclamp_bucket bucket[UCLAMP_BUCKETS];
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
কাঠামো rq {
	/*
	 * The following members are loaded together, without holding the
	 * rq->lock, in an extremely hot loop in update_sg_lb_stats()
	 * (called from pick_next_task()). To reduce cache pollution from
	 * this operation, they are placed together on this dedicated cache
	 * line. Even though some of them are frequently modified, they are
	 * loaded much more frequently than they are stored.
	 */
	unsigned পূর্ণসংখ্যা		nr_running;
#ifdef CONFIG_NUMA_BALANCING
	unsigned পূর্ণসংখ্যা		nr_numa_running;
	unsigned পূর্ণসংখ্যা		nr_preferred_running;
#endif
	unsigned পূর্ণসংখ্যা		ttwu_pending;
	unsigned long		cpu_capacity;
#ifdef CONFIG_SCHED_PROXY_EXEC
	কাঠামো task_struct __rcu	*donor;  /* Scheduling context */
	কাঠামো task_struct __rcu	*curr;   /* Execution context */
#অন্যথা
	সংঘ {
		কাঠামো task_struct __rcu *donor; /* Scheduler context */
		কাঠামো task_struct __rcu *curr;  /* Execution context */
	};
#endif
	কাঠামো task_struct	*idle;
	/* padding left here deliberately */

	/*
	 * The next cacheline holds the (hot) runqueue lock, as well as
	 * some other less performance-critical fields.
	 */
	u64			nr_switches	____cacheline_aligned;

	/* runqueue lock: */
	raw_spinlock_t		__lock;

#ifdef CONFIG_NO_HZ_COMMON
	unsigned পূর্ণসংখ্যা		nohz_tick_stopped;
	atomic_t		nohz_flags;
	unsigned পূর্ণসংখ্যা		has_blocked_load;
	unsigned long		last_blocked_load_update_tick;
	call_single_তথ্য_t	nohz_csd;
#endif /* CONFIG_NO_HZ_COMMON */

#ifdef CONFIG_UCLAMP_TASK
	/* Utilization clamp values based on CPU's RUNNABLE tasks */
	কাঠামো uclamp_rq	uclamp[UCLAMP_CNT] ____cacheline_aligned;
	unsigned পূর্ণসংখ্যা		uclamp_flags;
#define UCLAMP_FLAG_IDLE 0x01
#endif

	কাঠামো cfs_rq		cfs;
	কাঠামো rt_rq		rt;
	কাঠামো dl_rq		dl;
#ifdef CONFIG_SCHED_CLASS_EXT
	কাঠামো scx_rq		scx;
	কাঠামো sched_dl_entity	ext_server;
#endif
#ifdef CONFIG_SCHED_CACHE
	raw_spinlock_t		cpu_epoch_lock ____cacheline_aligned;
	u64			cpu_runtime;
	unsigned long		cpu_epoch;
	unsigned long		cpu_epoch_পরবর্তী;
#endif

	কাঠামো sched_dl_entity	fair_server;

#ifdef CONFIG_FAIR_GROUP_SCHED
	/* list of leaf cfs_rq on this CPU: */
	কাঠামো list_শীর্ষ	leaf_cfs_rq_list;
	কাঠামো list_শীর্ষ	*tmp_alone_branch;
#endif /* CONFIG_FAIR_GROUP_SCHED */

#ifdef CONFIG_NUMA_BALANCING
	unsigned পূর্ণসংখ্যা		numa_migrate_on;
#endif

#ifdef CONFIG_SCHED_CACHE
	unsigned পূর্ণসংখ্যা		nr_pref_llc_running;
	unsigned পূর্ণসংখ্যা		nr_llc_running;
#endif

	/*
	 * This is part of a global counter where only the total sum
	 * over all CPUs matters. A task can increase this counter on
	 * one CPU and if it got migrated afterwards it may decrease
	 * it on another CPU. Always updated under the runqueue lock:
	 */
	unsigned long		nr_uninterruptible;

	কাঠামো sched_dl_entity	*dl_server;
	কাঠামো task_struct	*stop;
	ধ্রুবক কাঠামো sched_class *পরবর্তী_class;
	unsigned long		পরবর্তী_balance;
	কাঠামো mm_struct	*পূর্ব_mm;

	/*
	 * The following fields of clock data are frequently referenced
	 * and updated together, and should go on their own cache line.
	 */
	u64			clock_task ____cacheline_aligned;
	u64			clock_pelt;
	u64			clock;
	unsigned long		lost_idle_time;
	unsigned পূর্ণসংখ্যা		clock_update_flags;
	u64			clock_pelt_idle;
	u64			clock_idle;

#ifndef CONFIG_64BIT
	u64			clock_pelt_idle_copy;
	u64			clock_idle_copy;
#endif

	u64 last_seen_need_resched_ns;
	পূর্ণসংখ্যা ticks_without_resched;

#ifdef CONFIG_MEMBARRIER
	পূর্ণসংখ্যা membarrier_state;
#endif

	কাঠামো মূল_domain		*rd;
	কাঠামো sched_domain __rcu	*sd;

	কাঠামো balance_callback *balance_callback;

	unsigned অক্ষর		nohz_idle_balance;
	unsigned অক্ষর		idle_balance;

	unsigned long		misfit_task_load;

	/* For active balancing */
	পূর্ণসংখ্যা			active_balance;
	পূর্ণসংখ্যা			ঠেলো_cpu;
	কাঠামো cpu_stop_work	active_balance_work;

	/* CPU of this runqueue: */
	পূর্ণসংখ্যা			cpu;
	পূর্ণসংখ্যা			online;

	কাঠামো list_শীর্ষ cfs_tasks;

	কাঠামো sched_avg	avg_rt;
	কাঠামো sched_avg	avg_dl;
#ifdef CONFIG_HAVE_SCHED_AVG_IRQ
	কাঠামো sched_avg	avg_irq;
#endif
#ifdef CONFIG_SCHED_HW_PRESSURE
	কাঠামো sched_avg	avg_hw;
#endif
	u64			idle_stamp;
	u64			avg_idle;

	/* This is used to determine avg_idle's max value */
	u64			max_idle_balance_cost;

#ifdef CONFIG_HOTPLUG_CPU
	কাঠামো rcuwait		hotplug_wait;
#endif

#ifdef CONFIG_IRQ_TIME_ACCOUNTING
	u64			পূর্ব_irq_time;
	u64			psi_irq_time;
#endif
#ifdef CONFIG_PARAVIRT
	u64			পূর্ব_steal_time;
#endif
#ifdef CONFIG_PARAVIRT_TIME_ACCOUNTING
	u64			পূর্ব_steal_time_rq;
#endif
#ifdef CONFIG_PREFERRED_CPU
	bool			npc_ঠেলো_work_pending;
#endif

	/* calc_load related fields */
	unsigned long		calc_load_update;
	long			calc_load_active;

#ifdef CONFIG_SCHED_HRTICK
	call_single_তথ্য_t	hrtick_csd;
	কাঠামো hrtimer		hrtick_timer;
	ktime_t			hrtick_time;
	ktime_t			hrtick_delay;
	unsigned পূর্ণসংখ্যা		hrtick_sched;
#endif

#ifdef CONFIG_SCHEDSTATS
	/* latency stats */
	কাঠামো sched_info	rq_sched_info;
	unsigned long long	rq_cpu_time;

	/* sys_sched_yield() stats */
	unsigned পূর্ণসংখ্যা		yld_গুনতি;

	/* schedule() stats */
	unsigned পূর্ণসংখ্যা		sched_গুনতি;
	unsigned পূর্ণসংখ্যা		sched_goidle;

	/* try_to_wake_up() stats */
	unsigned পূর্ণসংখ্যা		ttwu_গুনতি;
	unsigned পূর্ণসংখ্যা		ttwu_local;
#endif

#ifdef CONFIG_CPU_IDLE
	/* Must be inspected within a RCU lock section */
	কাঠামো cpuidle_state	*idle_state;
#endif

	unsigned পূর্ণসংখ্যা		nr_pinned;
	unsigned পূর্ণসংখ্যা		ঠেলো_busy;
	কাঠামো cpu_stop_work	ঠেলো_work;

#ifdef CONFIG_SCHED_CORE
	/* per rq */
	কাঠামো rq		*core;
	কাঠামো task_struct	*core_pick;
	কাঠামো sched_dl_entity	*core_dl_server;
	unsigned পূর্ণসংখ্যা		core_enabled;
	কাঠামো rb_মূল		core_বৃক্ষ;

	/* shared state -- careful with sched_core_cpu_deactivate() */
	unsigned পূর্ণসংখ্যা		core_task_seq;
	unsigned পূর্ণসংখ্যা		core_pick_seq;
	unsigned long		core_cookie;
	unsigned পূর্ণসংখ্যা		core_forceidle_গুনতি;
	unsigned পূর্ণসংখ্যা		core_forceidle_seq;
	unsigned পূর্ণসংখ্যা		core_forceidle_occupation;
	u64			core_forceidle_start;
	unsigned পূর্ণসংখ্যা		core_pick_in_flight;
#endif /* CONFIG_SCHED_CORE */

	/* Scratch cpumask to be temporarily used under rq_lock */
	cpumask_var_t		scratch_mask;

#ifdef CONFIG_CFS_BANDWIDTH
	call_single_তথ্য_t	cfsb_csd;
	কাঠামো list_শীর্ষ	cfsb_csd_list;
#endif

	atomic_t		nr_iowait;
} __no_randomize_layout;

#ifdef CONFIG_FAIR_GROUP_SCHED

/* CPU runqueue to which this cfs_rq is attached */
স্থির অন্তঃরেখ কাঠামো rq *rq_of(কাঠামো cfs_rq *cfs_rq)
{
	ফেরত cfs_rq->rq;
}

#অন্যথা /* !CONFIG_FAIR_GROUP_SCHED: */

স্থির অন্তঃরেখ কাঠামো rq *rq_of(কাঠামো cfs_rq *cfs_rq)
{
	ফেরত container_of(cfs_rq, কাঠামো rq, cfs);
}
#endif /* !CONFIG_FAIR_GROUP_SCHED */

স্থির অন্তঃরেখ পূর্ণসংখ্যা cpu_of(কাঠামো rq *rq)
{
	ফেরত rq->cpu;
}

#define MDF_ঠেলো		0x01

স্থির অন্তঃরেখ bool is_migration_disabled(কাঠামো task_struct *p)
{
	ফেরত p->migration_disabled;
}

DECLARE_PER_CPU_SHARED_ALIGNED(কাঠামো rq, runqueues);
DECLARE_PER_CPU(কাঠামো rnd_state, sched_rnd_state);

স্থির অন্তঃরেখ u32 sched_rng(শূন্য)
{
	ফেরত prandom_u32_state(this_cpu_ptr(&sched_rnd_state));
}

স্থির __always_inline কাঠামো rq *__this_rq(শূন্য)
{
	ফেরত this_cpu_ptr(&runqueues);
}

#define cpu_rq(cpu)		(&per_cpu(runqueues, (cpu)))
#define this_rq()		__this_rq()
#define task_rq(p)		cpu_rq(task_cpu(p))
#define cpu_curr(cpu)		(cpu_rq(cpu)->curr)
#define raw_rq()		raw_cpu_ptr(&runqueues)

স্থির অন্তঃরেখ bool idle_rq(কাঠামো rq *rq)
{
	ফেরত rq->curr == rq->idle && !rq->nr_running && !rq->ttwu_pending;
}

/**
 * available_idle_cpu - is a given CPU idle for enqueuing work.
 * @cpu: the CPU in question.
 *
 * Return: 1 if the CPU is currently idle. 0 otherwise.
 */
স্থির অন্তঃরেখ bool available_idle_cpu(পূর্ণসংখ্যা cpu)
{
	যদি (!idle_rq(cpu_rq(cpu)))
		ফেরত 0;

	যদি (vcpu_is_preempted(cpu))
		ফেরত 0;

	ফেরত 1;
}

#ifdef CONFIG_SCHED_PROXY_EXEC
স্থির অন্তঃরেখ শূন্য rq_set_donor(কাঠামো rq *rq, কাঠামো task_struct *t)
{
	rcu_assign_pointer(rq->donor, t);
}
#অন্যথা
স্থির অন্তঃরেখ শূন্য rq_set_donor(কাঠামো rq *rq, কাঠামো task_struct *t)
{
	/* Do nothing */
}
#endif

#ifdef CONFIG_SCHED_CORE
স্থির অন্তঃরেখ কাঠামো cpumask *sched_group_span(কাঠামো sched_group *sg);

DECLARE_STATIC_চাবি_FALSE(__sched_core_enabled);

স্থির অন্তঃরেখ bool sched_core_enabled(কাঠামো rq *rq)
{
	ফেরত static_branch_unlikely(&__sched_core_enabled) && rq->core_enabled;
}

স্থির অন্তঃরেখ bool sched_core_disabled(শূন্য)
{
	ফেরত !static_branch_unlikely(&__sched_core_enabled);
}

/*
 * Be careful with this function; not for general use. The return value isn't
 * stable unless you actually hold a relevant rq->__lock.
 */
স্থির অন্তঃরেখ raw_spinlock_t *rq_lockp(কাঠামো rq *rq)
{
	যদি (sched_core_enabled(rq))
		ফেরত &rq->core->__lock;

	ফেরত &rq->__lock;
}

স্থির অন্তঃরেখ raw_spinlock_t *__rq_lockp(কাঠামো rq *rq)
	__returns_ctx_lock(rq_lockp(rq)) /* alias them */
{
	যদি (rq->core_enabled)
		ফেরত &rq->core->__lock;

	ফেরত &rq->__lock;
}

বাহ্যিক bool
cfs_prio_less(ধ্রুবক কাঠামো task_struct *a, ধ্রুবক কাঠামো task_struct *b, bool fi);

বাহ্যিক শূন্য task_vruntime_update(কাঠামো rq *rq, কাঠামো task_struct *p, bool in_fi);

/*
 * Helpers to check if the CPU's core cookie matches with the task's cookie
 * when core scheduling is enabled.
 * A special case is that the task's cookie always matches with CPU's core
 * cookie if the CPU is in an idle core.
 */
স্থির অন্তঃরেখ bool sched_cpu_cookie_match(কাঠামো rq *rq, কাঠামো task_struct *p)
{
	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	যদি (!sched_core_enabled(rq))
		ফেরত true;

	ফেরত rq->core->core_cookie == p->core_cookie;
}

স্থির অন্তঃরেখ bool sched_core_cookie_match(কাঠামো rq *rq, কাঠামো task_struct *p)
{
	bool idle_core = true;
	পূর্ণসংখ্যা cpu;

	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	যদি (!sched_core_enabled(rq))
		ফেরত true;

	যদি (rq->core->core_cookie == p->core_cookie)
		ফেরত true;

	for_each_cpu(cpu, cpu_smt_mask(cpu_of(rq))) {
		যদি (!available_idle_cpu(cpu)) {
			idle_core = false;
			বিরতি;
		}
	}

	/*
	 * A CPU in an idle core is always the best choice for tasks with
	 * cookies.
	 */
	ফেরত idle_core;
}

স্থির অন্তঃরেখ bool sched_group_cookie_match(কাঠামো rq *rq,
					    কাঠামো task_struct *p,
					    কাঠামো sched_group *group)
{
	পূর্ণসংখ্যা cpu;

	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	যদি (!sched_core_enabled(rq))
		ফেরত true;

	for_each_cpu_and(cpu, sched_group_span(group), p->cpus_ptr) {
		যদি (sched_core_cookie_match(cpu_rq(cpu), p))
			ফেরত true;
	}
	ফেরত false;
}

স্থির অন্তঃরেখ bool sched_core_enqueued(কাঠামো task_struct *p)
{
	ফেরত !RB_EMPTY_গ্রন্থি(&p->core_গ্রন্থি);
}

বাহ্যিক শূন্য sched_core_enqueue(কাঠামো rq *rq, কাঠামো task_struct *p);
বাহ্যিক শূন্য sched_core_dequeue(কাঠামো rq *rq, কাঠামো task_struct *p, পূর্ণসংখ্যা flags);

বাহ্যিক শূন্য sched_core_get(শূন্য);
বাহ্যিক শূন্য sched_core_put(শূন্য);

স্থির অন্তঃরেখ bool task_has_sched_core(কাঠামো task_struct *p)
{
	যদি (sched_core_disabled())
		ফেরত false;

	ফেরত !!p->core_cookie;
}

#অন্যথা /* !CONFIG_SCHED_CORE: */

স্থির অন্তঃরেখ bool sched_core_enabled(কাঠামো rq *rq)
{
	ফেরত false;
}

স্থির অন্তঃরেখ bool sched_core_disabled(শূন্য)
{
	ফেরত true;
}

স্থির অন্তঃরেখ raw_spinlock_t *rq_lockp(কাঠামো rq *rq)
{
	ফেরত &rq->__lock;
}

স্থির অন্তঃরেখ raw_spinlock_t *__rq_lockp(কাঠামো rq *rq)
	__returns_ctx_lock(rq_lockp(rq)) /* alias them */
{
	ফেরত &rq->__lock;
}

স্থির অন্তঃরেখ bool sched_cpu_cookie_match(কাঠামো rq *rq, কাঠামো task_struct *p)
{
	ফেরত true;
}

স্থির অন্তঃরেখ bool sched_core_cookie_match(কাঠামো rq *rq, কাঠামো task_struct *p)
{
	ফেরত true;
}

স্থির অন্তঃরেখ bool sched_group_cookie_match(কাঠামো rq *rq,
					    কাঠামো task_struct *p,
					    কাঠামো sched_group *group)
{
	ফেরত true;
}

স্থির অন্তঃরেখ bool task_has_sched_core(কাঠামো task_struct *p)
{
	ফেরত false;
}

#endif /* !CONFIG_SCHED_CORE */

#ifdef CONFIG_RT_GROUP_SCHED
# ifdef CONFIG_RT_GROUP_SCHED_DEFAULT_DISABLED
DECLARE_STATIC_চাবি_FALSE(rt_group_sched);
স্থির অন্তঃরেখ bool rt_group_sched_enabled(শূন্য)
{
	ফেরত static_branch_unlikely(&rt_group_sched);
}
# অন্যথা /* !CONFIG_RT_GROUP_SCHED_DEFAULT_DISABLED: */
DECLARE_STATIC_চাবি_TRUE(rt_group_sched);
স্থির অন্তঃরেখ bool rt_group_sched_enabled(শূন্য)
{
	ফেরত static_branch_likely(&rt_group_sched);
}
# endif /* !CONFIG_RT_GROUP_SCHED_DEFAULT_DISABLED */
#অন্যথা /* !CONFIG_RT_GROUP_SCHED: */
# define rt_group_sched_enabled()	false
#endif /* !CONFIG_RT_GROUP_SCHED */

স্থির অন্তঃরেখ শূন্য lockdep_assert_rq_held(কাঠামো rq *rq)
	__assumes_ctx_lock(__rq_lockp(rq))
{
	lockdep_assert_held(__rq_lockp(rq));
}

বাহ্যিক শূন্য raw_spin_rq_lock_nested(কাঠামো rq *rq, পূর্ণসংখ্যা subclass)
	__acquires(__rq_lockp(rq));

বাহ্যিক bool raw_spin_rq_trylock(কাঠামো rq *rq)
	__cond_acquires(true, __rq_lockp(rq));

স্থির অন্তঃরেখ শূন্য raw_spin_rq_lock(কাঠামো rq *rq)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_lock_nested(rq, 0);
}

স্থির অন্তঃরেখ শূন্য raw_spin_rq_unlock(কাঠামো rq *rq)
	__releases(__rq_lockp(rq))
{
	raw_spin_unlock(rq_lockp(rq));
}

স্থির অন্তঃরেখ শূন্য raw_spin_rq_lock_irq(কাঠামো rq *rq)
	__acquires(__rq_lockp(rq))
{
	local_irq_disable();
	raw_spin_rq_lock(rq);
}

স্থির অন্তঃরেখ শূন্য raw_spin_rq_unlock_irq(কাঠামো rq *rq)
	__releases(__rq_lockp(rq))
{
	raw_spin_rq_unlock(rq);
	local_irq_enable();
}

স্থির অন্তঃরেখ unsigned long _raw_spin_rq_lock_irqsave(কাঠামো rq *rq)
	__acquires(__rq_lockp(rq))
{
	unsigned long flags;

	local_irq_save(flags);
	raw_spin_rq_lock(rq);

	ফেরত flags;
}

স্থির অন্তঃরেখ শূন্য raw_spin_rq_unlock_irqrestore(কাঠামো rq *rq, unsigned long flags)
	__releases(__rq_lockp(rq))
{
	raw_spin_rq_unlock(rq);
	local_irq_restore(flags);
}

#define raw_spin_rq_lock_irqsave(rq, flags)	\
করো {						\
	flags = _raw_spin_rq_lock_irqsave(rq);	\
} যখন (0)

বাহ্যিক শূন্য __update_idle_core(কাঠামো rq *rq);

স্থির অন্তঃরেখ শূন্য update_idle_core(কাঠামো rq *rq)
{
	যদি (sched_smt_active())
		__update_idle_core(rq);
}

#ifdef CONFIG_FAIR_GROUP_SCHED
স্থির অন্তঃরেখ কাঠামো task_struct *task_of(কাঠামো sched_entity *se)
{
	WARN_ON_ONCE(!entity_is_task(se));
	ফেরত container_of(se, কাঠামো task_struct, se);
}

স্থির অন্তঃরেখ কাঠামো cfs_rq *task_cfs_rq(কাঠামো task_struct *p)
{
	ফেরত p->se.cfs_rq;
}

/* runqueue on which this entity is (to be) queued */
স্থির অন্তঃরেখ কাঠামো cfs_rq *cfs_rq_of(ধ্রুবক কাঠামো sched_entity *se)
{
	ফেরত se->cfs_rq;
}

/* runqueue "owned" by this group */
স্থির অন্তঃরেখ কাঠামো cfs_rq *group_cfs_rq(কাঠামো sched_entity *grp)
{
	ফেরত grp->my_q;
}

#অন্যথা /* !CONFIG_FAIR_GROUP_SCHED: */

#define task_of(_se)		container_of(_se, কাঠামো task_struct, se)

স্থির অন্তঃরেখ কাঠামো cfs_rq *task_cfs_rq(ধ্রুবক কাঠামো task_struct *p)
{
	ফেরত &task_rq(p)->cfs;
}

স্থির অন্তঃরেখ কাঠামো cfs_rq *cfs_rq_of(ধ্রুবক কাঠামো sched_entity *se)
{
	ধ্রুবক কাঠামো task_struct *p = task_of(se);
	কাঠামো rq *rq = task_rq(p);

	ফেরত &rq->cfs;
}

/* runqueue "owned" by this group */
স্থির অন্তঃরেখ কাঠামো cfs_rq *group_cfs_rq(কাঠামো sched_entity *grp)
{
	ফেরত ফাঁকা;
}

#endif /* !CONFIG_FAIR_GROUP_SCHED */

বাহ্যিক শূন্য update_rq_avg_idle(কাঠামো rq *rq);
বাহ্যিক শূন্য update_rq_clock(কাঠামো rq *rq);

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

স্থির অন্তঃরেখ শূন্য assert_clock_updated(কাঠামো rq *rq)
{
	/*
	 * The only reason for not seeing a clock update since the
	 * last rq_pin_lock() is if we're currently skipping updates.
	 */
	WARN_ON_ONCE(rq->clock_update_flags < RQCF_ACT_SKIP);
}

স্থির অন্তঃরেখ u64 rq_clock(কাঠামো rq *rq)
{
	lockdep_assert_rq_held(rq);
	assert_clock_updated(rq);

	ফেরত rq->clock;
}

স্থির অন্তঃরেখ u64 rq_clock_task(কাঠামো rq *rq)
{
	lockdep_assert_rq_held(rq);
	assert_clock_updated(rq);

	ফেরত rq->clock_task;
}

স্থির অন্তঃরেখ শূন্য rq_clock_skip_update(কাঠামো rq *rq)
{
	lockdep_assert_rq_held(rq);
	rq->clock_update_flags |= RQCF_REQ_SKIP;
}

/*
 * See rt task throttling, which is the only time a skip
 * request is canceled.
 */
স্থির অন্তঃরেখ শূন্য rq_clock_cancel_skipupdate(কাঠামো rq *rq)
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
স্থির অন্তঃরেখ শূন্য rq_clock_start_loop_update(কাঠামো rq *rq)
{
	lockdep_assert_rq_held(rq);
	WARN_ON_ONCE(rq->clock_update_flags & RQCF_ACT_SKIP);
	rq->clock_update_flags |= RQCF_ACT_SKIP;
}

স্থির অন্তঃরেখ শূন্য rq_clock_stop_loop_update(কাঠামো rq *rq)
{
	lockdep_assert_rq_held(rq);
	rq->clock_update_flags &= ~RQCF_ACT_SKIP;
}

কাঠামো rq_flags {
	unsigned long flags;
	কাঠামো pin_cookie cookie;
	/*
	 * A copy of (rq::clock_update_flags & RQCF_UPDATED) for the
	 * current pin context is stashed here in case it needs to be
	 * restored in rq_repin_lock().
	 */
	unsigned পূর্ণসংখ্যা clock_update_flags;
};

বাহ্যিক কাঠামো balance_callback balance_ঠেলো_callback;

#ifdef CONFIG_SCHED_CLASS_EXT
বাহ্যিক ধ্রুবক কাঠামো sched_class ext_sched_class;

DECLARE_STATIC_চাবি_FALSE(__scx_enabled);	/* SCX BPF scheduler loaded */
DECLARE_STATIC_চাবি_FALSE(__scx_switched_all);	/* all fair class tasks on SCX */

#define scx_enabled()		static_branch_unlikely(&__scx_enabled)
#define scx_switched_all()	static_branch_unlikely(&__scx_switched_all)

স্থির অন্তঃরেখ শূন্য scx_rq_clock_update(কাঠামো rq *rq, u64 clock)
{
	যদি (!scx_enabled())
		ফেরত;
	WRITE_ONCE(rq->scx.clock, clock);
	smp_store_release(&rq->scx.flags, rq->scx.flags | SCX_RQ_CLK_VALID);
}

স্থির অন্তঃরেখ শূন্য scx_rq_clock_invalidate(কাঠামো rq *rq)
{
	যদি (!scx_enabled())
		ফেরত;
	WRITE_ONCE(rq->scx.flags, rq->scx.flags & ~SCX_RQ_CLK_VALID);
}

#অন্যথা /* !CONFIG_SCHED_CLASS_EXT: */
#define scx_enabled()		false
#define scx_switched_all()	false

স্থির অন্তঃরেখ শূন্য scx_rq_clock_update(কাঠামো rq *rq, u64 clock) {}
স্থির অন্তঃরেখ শূন্য scx_rq_clock_invalidate(কাঠামো rq *rq) {}
#endif /* !CONFIG_SCHED_CLASS_EXT */

স্থির অন্তঃরেখ শূন্য assert_balance_callbacks_empty(কাঠামো rq *rq)
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
স্থির অন্তঃরেখ শূন্য rq_pin_lock(কাঠামো rq *rq, কাঠামো rq_flags *rf)
{
	rf->cookie = lockdep_pin_lock(__rq_lockp(rq));

	rq->clock_update_flags &= (RQCF_REQ_SKIP|RQCF_ACT_SKIP);
	rf->clock_update_flags = 0;
	assert_balance_callbacks_empty(rq);
}

স্থির অন্তঃরেখ শূন্য rq_unpin_lock(কাঠামো rq *rq, কাঠামো rq_flags *rf)
{
	যদি (rq->clock_update_flags > RQCF_ACT_SKIP)
		rf->clock_update_flags = RQCF_UPDATED;

	scx_rq_clock_invalidate(rq);
	lockdep_unpin_lock(__rq_lockp(rq), rf->cookie);
}

স্থির অন্তঃরেখ শূন্য rq_repin_lock(কাঠামো rq *rq, কাঠামো rq_flags *rf)
{
	lockdep_repin_lock(__rq_lockp(rq), rf->cookie);

	/*
	 * Restore the value we stashed in @rf for this pin context.
	 */
	rq->clock_update_flags |= rf->clock_update_flags;
}

#define __task_rq_lock(...) __acquire_ret(___task_rq_lock(__VA_ARGS__), __rq_lockp(__ret))
বাহ্যিক কাঠামো rq *___task_rq_lock(কাঠামো task_struct *p, কাঠামো rq_flags *rf) __acquires_ret;

#define task_rq_lock(...) __acquire_ret(_task_rq_lock(__VA_ARGS__), __rq_lockp(__ret))
বাহ্যিক কাঠামো rq *_task_rq_lock(কাঠামো task_struct *p, কাঠামো rq_flags *rf)
	__acquires(&p->pi_lock) __acquires_ret;

স্থির অন্তঃরেখ শূন্য
__task_rq_unlock(কাঠামো rq *rq, কাঠামো task_struct *p, কাঠামো rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_lock(rq, rf);
	raw_spin_rq_unlock(rq);
}

স্থির অন্তঃরেখ শূন্য
task_rq_unlock(কাঠামো rq *rq, কাঠামো task_struct *p, কাঠামো rq_flags *rf)
	__releases(__rq_lockp(rq), &p->pi_lock)
{
	__task_rq_unlock(rq, p, rf);
	raw_spin_unlock_irqrestore(&p->pi_lock, rf->flags);
}

DEFINE_LOCK_GUARD_1(task_rq_lock, কাঠামো task_struct,
		    _T->rq = task_rq_lock(_T->lock, &_T->rf),
		    task_rq_unlock(_T->rq, _T->lock, &_T->rf),
		    কাঠামো rq *rq; কাঠামো rq_flags rf)
DECLARE_LOCK_GUARD_1_ATTRS(task_rq_lock, __acquires(_T->pi_lock), __releases((*(কাঠামো task_struct **)_T)->pi_lock))
#define class_task_rq_lock_constructor(_T) WITH_LOCK_GUARD_1_ATTRS(task_rq_lock, _T)

DEFINE_LOCK_GUARD_1(__task_rq_lock, কাঠামো task_struct,
		    _T->rq = __task_rq_lock(_T->lock, &_T->rf),
		    __task_rq_unlock(_T->rq, _T->lock, &_T->rf),
		    কাঠামো rq *rq; কাঠামো rq_flags rf)

স্থির অন্তঃরেখ শূন্য rq_lock_irqsave(কাঠামো rq *rq, কাঠামো rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_lock_irqsave(rq, rf->flags);
	rq_pin_lock(rq, rf);
}

স্থির অন্তঃরেখ শূন্য rq_lock_irq(কাঠামো rq *rq, কাঠামো rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_lock_irq(rq);
	rq_pin_lock(rq, rf);
}

স্থির অন্তঃরেখ শূন্য rq_lock(কাঠামো rq *rq, কাঠামো rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_lock(rq);
	rq_pin_lock(rq, rf);
}

স্থির অন্তঃরেখ শূন্য rq_unlock_irqrestore(কাঠামো rq *rq, কাঠামো rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_lock(rq, rf);
	raw_spin_rq_unlock_irqrestore(rq, rf->flags);
}

স্থির অন্তঃরেখ শূন্য rq_unlock_irq(কাঠামো rq *rq, কাঠামো rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_lock(rq, rf);
	raw_spin_rq_unlock_irq(rq);
}

স্থির অন্তঃরেখ শূন্য rq_unlock(কাঠামো rq *rq, কাঠামো rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_lock(rq, rf);
	raw_spin_rq_unlock(rq);
}

DEFINE_LOCK_GUARD_1(rq_lock, কাঠামো rq,
		    rq_lock(_T->lock, &_T->rf),
		    rq_unlock(_T->lock, &_T->rf),
		    কাঠামো rq_flags rf)

DECLARE_LOCK_GUARD_1_ATTRS(rq_lock, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(কাঠামো rq **)_T)));
#define class_rq_lock_constructor(_T) WITH_LOCK_GUARD_1_ATTRS(rq_lock, _T)

DEFINE_LOCK_GUARD_1(rq_lock_irq, কাঠামো rq,
		    rq_lock_irq(_T->lock, &_T->rf),
		    rq_unlock_irq(_T->lock, &_T->rf),
		    কাঠামো rq_flags rf)

DECLARE_LOCK_GUARD_1_ATTRS(rq_lock_irq, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(কাঠামো rq **)_T)));
#define class_rq_lock_irq_constructor(_T) WITH_LOCK_GUARD_1_ATTRS(rq_lock_irq, _T)

DEFINE_LOCK_GUARD_1(rq_lock_irqsave, কাঠামো rq,
		    rq_lock_irqsave(_T->lock, &_T->rf),
		    rq_unlock_irqrestore(_T->lock, &_T->rf),
		    কাঠামো rq_flags rf)

DECLARE_LOCK_GUARD_1_ATTRS(rq_lock_irqsave, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(কাঠামো rq **)_T)));
#define class_rq_lock_irqsave_constructor(_T) WITH_LOCK_GUARD_1_ATTRS(rq_lock_irqsave, _T)

DEFINE_LOCK_GUARD_1(raw_spin_rq_lock_irqsave, কাঠামো rq,
		    raw_spin_rq_lock_irqsave(_T->lock, _T->flags),
		    raw_spin_rq_unlock_irqrestore(_T->lock, _T->flags),
		    unsigned long flags)

DECLARE_LOCK_GUARD_1_ATTRS(raw_spin_rq_lock_irqsave, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(কাঠামো rq **)_T)));
#define class_raw_spin_rq_lock_irqsave_constructor(_T) \
	WITH_LOCK_GUARD_1_ATTRS(raw_spin_rq_lock_irqsave, _T)

#define this_rq_lock_irq(...) __acquire_ret(_this_rq_lock_irq(__VA_ARGS__), __rq_lockp(__ret))
স্থির অন্তঃরেখ কাঠামো rq *_this_rq_lock_irq(কাঠামো rq_flags *rf) __acquires_ret
{
	কাঠামো rq *rq;

	local_irq_disable();
	rq = this_rq();
	rq_lock(rq, rf);

	ফেরত rq;
}

#ifdef CONFIG_NUMA

গণনা numa_topology_type {
	NUMA_DIRECT,
	NUMA_GLUELESS_MESH,
	NUMA_BACKPLANE,
};

বাহ্যিক গণনা numa_topology_type sched_numa_topology_type;
বাহ্যিক পূর্ণসংখ্যা sched_max_numa_distance;
বাহ্যিক bool find_numa_distance(পূর্ণসংখ্যা distance);
বাহ্যিক শূন্য sched_init_numa(পূর্ণসংখ্যা offline_গ্রন্থি);
বাহ্যিক শূন্য sched_update_numa(পূর্ণসংখ্যা cpu, bool online);
বাহ্যিক শূন্য sched_domains_numa_masks_set(unsigned পূর্ণসংখ্যা cpu);
বাহ্যিক শূন্য sched_domains_numa_masks_clear(unsigned পূর্ণসংখ্যা cpu);
বাহ্যিক পূর্ণসংখ্যা sched_numa_find_closest(ধ্রুবক কাঠামো cpumask *cpus, পূর্ণসংখ্যা cpu);

#অন্যথা /* !CONFIG_NUMA: */

স্থির অন্তঃরেখ শূন্য sched_init_numa(পূর্ণসংখ্যা offline_গ্রন্থি) { }
স্থির অন্তঃরেখ শূন্য sched_update_numa(পূর্ণসংখ্যা cpu, bool online) { }
স্থির অন্তঃরেখ শূন্য sched_domains_numa_masks_set(unsigned পূর্ণসংখ্যা cpu) { }
স্থির অন্তঃরেখ শূন্য sched_domains_numa_masks_clear(unsigned পূর্ণসংখ্যা cpu) { }

স্থির অন্তঃরেখ পূর্ণসংখ্যা sched_numa_find_closest(ধ্রুবক কাঠামো cpumask *cpus, পূর্ণসংখ্যা cpu)
{
	ফেরত nr_cpu_ids;
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

বাহ্যিক শূন্য sched_setnuma(কাঠামো task_struct *p, পূর্ণসংখ্যা গ্রন্থি);
বাহ্যিক পূর্ণসংখ্যা migrate_task_to(কাঠামো task_struct *p, পূর্ণসংখ্যা cpu);
বাহ্যিক পূর্ণসংখ্যা migrate_swap(কাঠামো task_struct *p, কাঠামো task_struct *t,
			পূর্ণসংখ্যা cpu, পূর্ণসংখ্যা scpu);
বাহ্যিক শূন্য init_numa_balancing(u64 clone_flags, কাঠামো task_struct *p);

#অন্যথা /* !CONFIG_NUMA_BALANCING: */

স্থির অন্তঃরেখ শূন্য
init_numa_balancing(u64 clone_flags, কাঠামো task_struct *p)
{
}

#endif /* !CONFIG_NUMA_BALANCING */

পূর্ণসংখ্যা task_llc(ধ্রুবক কাঠামো task_struct *p);

স্থির অন্তঃরেখ শূন্য
queue_balance_callback(কাঠামো rq *rq,
		       কাঠামো balance_callback *শীর্ষ,
		       শূন্য (*func)(কাঠামো rq *rq))
{
	lockdep_assert_rq_held(rq);

	/*
	 * Don't (re)queue an already queued item; nor queue anything when
	 * balance_push() is active, see the comment with
	 * balance_push_callback.
	 */
	যদি (unlikely(শীর্ষ->পরবর্তী || rq->balance_callback == &balance_ঠেলো_callback))
		ফেরত;

	শীর্ষ->func = func;
	শীর্ষ->পরবর্তী = rq->balance_callback;
	rq->balance_callback = শীর্ষ;
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
	জন্য (__sd = rcu_dereference_sched_domain(cpu_rq(cpu)->sd); \
			__sd; __sd = __sd->parent)

/* A mask of all the SD flags that have the SDF_SHARED_CHILD metaflag */
#define SD_FLAG(name, mflags) (name * !!((mflags) & SDF_SHARED_CHILD)) |
স্থির ধ্রুবক unsigned পূর্ণসংখ্যা SD_SHARED_CHILD_MASK =
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
স্থির অন্তঃরেখ কাঠামো sched_domain *highest_flag_domain(পূর্ণসংখ্যা cpu, পূর্ণসংখ্যা flag)
{
	কাঠামো sched_domain *sd, *hsd = ফাঁকা;

	for_each_domain(cpu, sd) {
		যদি (sd->flags & flag) {
			hsd = sd;
			চালু;
		}

		/*
		 * Stop the search if @flag is known to be shared at lower
		 * levels. It will not be found further up.
		 */
		যদি (flag & SD_SHARED_CHILD_MASK)
			বিরতি;
	}

	ফেরত hsd;
}

স্থির অন্তঃরেখ কাঠামো sched_domain *lowest_flag_domain(পূর্ণসংখ্যা cpu, পূর্ণসংখ্যা flag)
{
	কাঠামো sched_domain *sd;

	for_each_domain(cpu, sd) {
		যদি (sd->flags & flag)
			বিরতি;
	}

	ফেরত sd;
}

DECLARE_PER_CPU(কাঠামো sched_domain __rcu *, sd_llc);
DECLARE_PER_CPU(পূর্ণসংখ্যা, sd_llc_আকার);
DECLARE_PER_CPU(পূর্ণসংখ্যা, sd_llc_id);
DECLARE_PER_CPU(পূর্ণসংখ্যা, sd_share_id);
DECLARE_PER_CPU(কাঠামো sched_domain_shared __rcu *, sd_llc_shared);
DECLARE_PER_CPU(কাঠামো sched_domain_shared __rcu *, sd_balance_shared);
DECLARE_PER_CPU(কাঠামো sched_domain __rcu *, sd_numa);
DECLARE_PER_CPU(কাঠামো sched_domain __rcu *, sd_asym_packing);
DECLARE_PER_CPU(কাঠামো sched_domain __rcu *, sd_asym_cpucapacity);

বাহ্যিক কাঠামো static_চাবি_false sched_asym_cpucapacity;
বাহ্যিক কাঠামো static_চাবি_false sched_cluster_active;

স্থির __always_inline bool sched_asym_cpucap_active(শূন্য)
{
	ফেরত static_branch_unlikely(&sched_asym_cpucapacity);
}

কাঠামো sched_group_capacity {
	atomic_t		ref;
	/*
	 * CPU capacity of this group, SCHED_CAPACITY_SCALE being max capacity
	 * for a single CPU.
	 */
	unsigned long		capacity;
	unsigned long		min_capacity;		/* Min per-CPU capacity in group */
	unsigned long		max_capacity;		/* Max per-CPU capacity in group */
	unsigned long		পরবর্তী_update;
	পূর্ণসংখ্যা			imbalance;		/* XXX unrelated to capacity but shared group state */

	পূর্ণসংখ্যা			id;

	unsigned long		cpumask[];		/* Balance mask */
};

কাঠামো sched_group {
	কাঠামো sched_group	*পরবর্তী;			/* Must be a circular list */
	atomic_t		ref;

	unsigned পূর্ণসংখ্যা		group_weight;
	unsigned পূর্ণসংখ্যা		cores;
	কাঠামো sched_group_capacity *sgc;
	পূর্ণসংখ্যা			asym_prefer_cpu;	/* CPU of highest priority in group */
	পূর্ণসংখ্যা			flags;

	/*
	 * The CPUs this group covers.
	 *
	 * NOTE: this field is variable length. (Allocated dynamically
	 * by attaching extra space to the end of the structure,
	 * depending on how many CPUs the kernel has booted up with)
	 */
	unsigned long		cpumask[];
};

স্থির অন্তঃরেখ কাঠামো cpumask *sched_group_span(কাঠামো sched_group *sg)
{
	ফেরত to_cpumask(sg->cpumask);
}

/*
 * See build_balance_mask().
 */
স্থির অন্তঃরেখ কাঠামো cpumask *group_balance_mask(কাঠামো sched_group *sg)
{
	ফেরত to_cpumask(sg->sgc->cpumask);
}

বাহ্যিক পূর্ণসংখ্যা group_balance_cpu(কাঠামো sched_group *sg);

বাহ্যিক শূন্য update_sched_domain_debugfs(শূন্য);
বাহ্যিক শূন্য dirty_sched_domain_sysctl(পূর্ণসংখ্যা cpu);

বাহ্যিক পূর্ণসংখ্যা sched_update_scaling(শূন্য);

স্থির অন্তঃরেখ ধ্রুবক কাঠামো cpumask *task_user_cpus(কাঠামো task_struct *p)
{
	যদি (!p->user_cpus_ptr)
		ফেরত cpu_possible_mask; /* &init_task.cpus_mask */
	ফেরত p->user_cpus_ptr;
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
স্থির অন্তঃরেখ কাঠামো task_group *task_group(কাঠামো task_struct *p)
{
	ফেরত p->sched_task_group;
}

#ifdef CONFIG_FAIR_GROUP_SCHED
/*
 * Defined here to be available before stats.h is included, since
 * stats.h has dependencies on things defined later in this file.
 */
কাঠামো cfs_tg_state {
	কাঠামো cfs_rq		cfs_rq;
	কাঠামো sched_entity	se;
	কাঠামো sched_statistics	stats;
} __no_randomize_layout;

/* Access a specific CPU's cfs_rq from a task group */
স্থির অন্তঃরেখ কাঠামো cfs_rq *tg_cfs_rq(কাঠামো task_group *tg, পূর্ণসংখ্যা cpu)
{
	ফেরত per_cpu_ptr(tg->cfs_rq, cpu);
}

স্থির অন্তঃরেখ কাঠামো sched_entity *tg_se(কাঠামো task_group *tg, পূর্ণসংখ্যা cpu)
{
	কাঠামো cfs_tg_state *state;

	যদি (is_মূল_task_group(tg))
		ফেরত ফাঁকা;

	state = container_of(tg_cfs_rq(tg, cpu), কাঠামো cfs_tg_state, cfs_rq);
	ফেরত &state->se;
}

স্থির অন্তঃরেখ কাঠামো sched_entity *cfs_rq_se(কাঠামো cfs_rq *cfs_rq)
{
	কাঠামো cfs_tg_state *state;

	যদি (is_মূল_task_group(cfs_rq->tg))
		ফেরত ফাঁকা;

	state = container_of(cfs_rq, কাঠামো cfs_tg_state, cfs_rq);
	ফেরত &state->se;
}
#endif

/* Change a task's cfs_rq and parent entity if it moves across CPUs/groups */
স্থির অন্তঃরেখ শূন্য set_task_rq(কাঠামো task_struct *p, unsigned পূর্ণসংখ্যা cpu)
{
#যদি defined(CONFIG_FAIR_GROUP_SCHED) || defined(CONFIG_RT_GROUP_SCHED)
	কাঠামো task_group *tg = task_group(p);
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

স্থির অন্তঃরেখ শূন্য set_task_rq(কাঠামো task_struct *p, unsigned পূর্ণসংখ্যা cpu) { }

স্থির অন্তঃরেখ কাঠামো task_group *task_group(কাঠামো task_struct *p)
{
	ফেরত ফাঁকা;
}

#endif /* !CONFIG_CGROUP_SCHED */

স্থির অন্তঃরেখ শূন্য __set_task_cpu(কাঠামো task_struct *p, unsigned পূর্ণসংখ্যা cpu)
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
বাহ্যিক __read_mostly unsigned পূর্ণসংখ্যা sysctl_sched_features;

#ifdef CONFIG_JUMP_LABEL

সংঘ sched_feat_চাবি {
	কাঠামো static_চাবি_true	চাবি_true;
	কাঠামো static_চাবি_false	চাবি_false;
};

#define sched_feat_branch_true(চাবি)	static_branch_likely(&(চাবি)->চাবি_true)
#define sched_feat_branch_false(চাবি)	static_branch_unlikely(&(চাবি)->চাবি_false)

#define SCHED_FEAT(name, enabled)			\
স্থির __always_inline bool				\
static_branch_##name(সংঘ sched_feat_চাবি *চাবি) 	\
{							\
	ফেরত sched_feat_branch_##enabled(চাবি);	\
}

#অন্তর্ভাব "features.h"
#undef SCHED_FEAT

বাহ্যিক সংঘ sched_feat_চাবি sched_feat_চাবিগুলি[__SCHED_FEAT_NR];
#define sched_feat(x) (static_branch_##x(&sched_feat_চাবিগুলি[__SCHED_FEAT_##x]))

#অন্যথা /* !CONFIG_JUMP_LABEL: */

#define sched_feat(x) (sysctl_sched_features & (1UL << __SCHED_FEAT_##x))

#endif /* !CONFIG_JUMP_LABEL */

বাহ্যিক কাঠামো static_চাবি_false sched_numa_balancing;
বাহ্যিক কাঠামো static_চাবি_false sched_schedstats;

স্থির অন্তঃরেখ u64 global_rt_period(শূন্য)
{
	ফেরত (u64)sysctl_sched_rt_period * NSEC_PER_USEC;
}

স্থির অন্তঃরেখ u64 global_rt_runtime(শূন্য)
{
	যদি (sysctl_sched_rt_runtime < 0)
		ফেরত RUNTIME_INF;

	ফেরত (u64)sysctl_sched_rt_runtime * NSEC_PER_USEC;
}

/*
 * Is p the current execution context?
 */
স্থির অন্তঃরেখ পূর্ণসংখ্যা task_current(কাঠামো rq *rq, কাঠামো task_struct *p)
{
	ফেরত rq->curr == p;
}

/*
 * Is p the current scheduling context?
 *
 * Note that it might be the current execution context at the same time if
 * rq->curr == rq->donor == p.
 */
স্থির অন্তঃরেখ পূর্ণসংখ্যা task_current_donor(কাঠামো rq *rq, কাঠামো task_struct *p)
{
	ফেরত rq->donor == p;
}

স্থির অন্তঃরেখ bool task_is_blocked(কাঠামো task_struct *p)
{
	যদি (!sched_proxy_exec())
		ফেরত false;

	ফেরত !!p->blocked_on;
}

#ifdef CONFIG_SCHED_PROXY_EXEC
শূন্য sched_proxy_block_task(কাঠামো rq *rq, কাঠামো task_struct *p);
#অন্যথা
স্থির অন্তঃরেখ শূন্য sched_proxy_block_task(কাঠামো rq *rq, কাঠামো task_struct *p) {}
#endif

স্থির অন্তঃরেখ পূর্ণসংখ্যা task_on_cpu(কাঠামো rq *rq, কাঠামো task_struct *p)
{
	ফেরত p->on_cpu;
}

স্থির অন্তঃরেখ পূর্ণসংখ্যা task_on_rq_queued(কাঠামো task_struct *p)
{
	ফেরত READ_ONCE(p->on_rq) == TASK_ON_RQ_QUEUED;
}

স্থির অন্তঃরেখ পূর্ণসংখ্যা task_on_rq_migrating(কাঠামো task_struct *p)
{
	ফেরত READ_ONCE(p->on_rq) == TASK_ON_RQ_MIGRATING;
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

বাহ্যিক ধ্রুবক পূর্ণসংখ্যা		sched_prio_to_weight[40];
বাহ্যিক ধ্রুবক u32		sched_prio_to_wmult[40];

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

#define ENQUEUE_শীর্ষ		0x00010000
#define ENQUEUE_REPLENISH	0x00020000
#define ENQUEUE_MIGRATED	0x00040000
#define ENQUEUE_INITIAL		0x00080000
#define ENQUEUE_RQ_SELECTED	0x00100000
#define ENQUEUE_QUEUED		0x00200000

#define RETRY_TASK		((শূন্য *)-1UL)

কাঠামো affinity_context {
	ধ্রুবক কাঠামো cpumask	*new_mask;
	কাঠামো cpumask		*user_mask;
	unsigned পূর্ণসংখ্যা		flags;
};

বাহ্যিক s64 update_curr_common(কাঠামো rq *rq);

গণনা snt_e {
	SNT_NORMAL,	/* set_next_task() */
	SNT_PICK,	/* put_prev_set_next_task(): prev != next */
	SNT_REPICK,	/* put_prev_set_next_task(): prev == next */
};

কাঠামো sched_class {

#ifdef CONFIG_UCLAMP_TASK
	পূর্ণসংখ্যা uclamp_enabled;
#endif

	/*
	 * move_queued_task/activate_task/enqueue_task: rq->lock
	 * ttwu_do_activate/activate_task/enqueue_task: rq->lock
	 * wake_up_new_task/activate_task/enqueue_task: task_rq_lock
	 * ttwu_runnable/enqueue_task: task_rq_lock
	 * proxy_task_current: rq->lock
	 * sched_change_end
	 */
	শূন্য (*enqueue_task) (কাঠামো rq *rq, কাঠামো task_struct *p, পূর্ণসংখ্যা flags);
	/*
	 * move_queued_task/deactivate_task/dequeue_task: rq->lock
	 * __schedule/block_task/dequeue_task: rq->lock
	 * proxy_task_current: rq->lock
	 * wait_task_inactive: task_rq_lock
	 * sched_change_begin
	 */
	bool (*dequeue_task) (কাঠামো rq *rq, কাঠামো task_struct *p, পূর্ণসংখ্যা flags);

	/*
	 * do_sched_yield: rq->lock
	 */
	শূন্য (*yield_task)   (কাঠামো rq *rq);
	/*
	 * yield_to: rq->lock (double)
	 */
	bool (*yield_to_task)(কাঠামো rq *rq, কাঠামো task_struct *p);

	/*
	 * move_queued_task: rq->lock
	 * __migrate_swap_task: rq->lock
	 * ttwu_do_activate: rq->lock
	 * ttwu_runnable: task_rq_lock
	 * wake_up_new_task: task_rq_lock
	 */
	শূন্য (*wakeup_preempt)(কাঠামো rq *rq, কাঠামো task_struct *p, পূর্ণসংখ্যা flags);

	/*
	 * schedule/pick_next_task/prev_balance: rq->lock
	 */
	পূর্ণসংখ্যা (*balance)(কাঠামো rq *rq, কাঠামো rq_flags *rf);

	/*
	 * schedule/pick_next_task: rq->lock
	 */
	কাঠামো task_struct *(*pick_task)(কাঠামো rq *rq, কাঠামো rq_flags *rf);

	/*
	 * sched_change:
	 * __schedule: rq->lock
	 */
	শূন্য (*put_পূর্ব_task)(কাঠামো rq *rq, কাঠামো task_struct *p, কাঠামো task_struct *পরবর্তী);
	শূন্য (*set_পরবর্তী_task)(কাঠামো rq *rq, কাঠামো task_struct *p, গণনা snt_e type);

	/*
	 * select_task_rq: p->pi_lock
	 * sched_exec: p->pi_lock
	 */
	পূর্ণসংখ্যা  (*select_task_rq)(কাঠামো task_struct *p, পূর্ণসংখ্যা task_cpu, পূর্ণসংখ্যা flags);

	/*
	 * set_task_cpu: p->pi_lock || rq->lock (ttwu like)
	 */
	শূন্য (*migrate_task_rq)(কাঠামো task_struct *p, পূর্ণসংখ্যা new_cpu);

	/*
	 * ttwu_do_activate: rq->lock
	 * wake_up_new_task: task_rq_lock
	 */
	শূন্য (*task_woken)(কাঠামো rq *this_rq, কাঠামো task_struct *task);

	/*
	 * do_set_cpus_allowed: task_rq_lock + sched_change
	 */
	শূন্য (*set_cpus_allowed)(কাঠামো task_struct *p, কাঠামো affinity_context *ctx);

	/*
	 * sched_set_rq_{on,off}line: rq->lock
	 */
	শূন্য (*rq_online)(কাঠামো rq *rq);
	শূন্য (*rq_offline)(কাঠামো rq *rq);

	/*
	 * push_cpu_stop: p->pi_lock && rq->lock
	 */
	কাঠামো rq *(*find_lock_rq)(কাঠামো task_struct *p, কাঠামো rq *rq);

	/*
	 * hrtick: rq->lock
	 * sched_tick: rq->lock
	 * sched_tick_remote: rq->lock
	 */
	শূন্য (*task_tick)(কাঠামো rq *rq, কাঠামো task_struct *p, পূর্ণসংখ্যা queued);
	/*
	 * sched_cgroup_fork: p->pi_lock
	 */
	শূন্য (*task_fork)(কাঠামো task_struct *p);
	/*
	 * finish_task_switch: no locks
	 */
	শূন্য (*task_dead)(কাঠামো task_struct *p);

	/*
	 * sched_change
	 */
	শূন্য (*switching_from)(কাঠামো rq *this_rq, কাঠামো task_struct *task);
	শূন্য (*switched_from) (কাঠামো rq *this_rq, কাঠামো task_struct *task);
	শূন্য (*switching_to)  (কাঠামো rq *this_rq, কাঠামো task_struct *task);
	শূন্য (*switched_to)   (কাঠামো rq *this_rq, কাঠামো task_struct *task);
	u64  (*get_prio)     (কাঠামো rq *this_rq, কাঠামো task_struct *task);
	শূন্য (*prio_changed) (কাঠামো rq *this_rq, কাঠামো task_struct *task,
			      u64 oldprio);

	/*
	 * set_load_weight: task_rq_lock + sched_change
	 * __setscheduler_parms: task_rq_lock + sched_change
	 */
	শূন্য (*reweight_task)(কাঠামো rq *this_rq, কাঠামো task_struct *task,
			      ধ্রুবক কাঠামো load_weight *lw);

	/*
	 * sched_rr_get_interval: task_rq_lock
	 */
	unsigned পূর্ণসংখ্যা (*get_rr_interval)(কাঠামো rq *rq,
					কাঠামো task_struct *task);

	/*
	 * task_sched_runtime: task_rq_lock
	 */
	শূন্য (*update_curr)(কাঠামো rq *rq);

#ifdef CONFIG_FAIR_GROUP_SCHED
	/*
	 * sched_change_group: task_rq_lock + sched_change
	 */
	শূন্য (*task_change_group)(কাঠামো task_struct *p);
#endif

#ifdef CONFIG_SCHED_CORE
	/*
	 * pick_next_task: rq->lock
	 * try_steal_cookie: rq->lock (double)
	 */
	পূর্ণসংখ্যা (*task_is_throttled)(কাঠামো task_struct *p, পূর্ণসংখ্যা cpu);
#endif
};

স্থির অন্তঃরেখ শূন্য put_পূর্ব_task(কাঠামো rq *rq, কাঠামো task_struct *পূর্ব)
{
	WARN_ON_ONCE(rq->donor != পূর্ব);
	পূর্ব->sched_class->put_পূর্ব_task(rq, পূর্ব, ফাঁকা);
}

স্থির অন্তঃরেখ শূন্য set_পরবর্তী_task(কাঠামো rq *rq, কাঠামো task_struct *পরবর্তী)
{
	পরবর্তী->sched_class->set_পরবর্তী_task(rq, পরবর্তী, SNT_NORMAL);
}

স্থির অন্তঃরেখ শূন্য
__put_prev_set_next_dl_server(কাঠামো rq *rq,
			      কাঠামো task_struct *পূর্ব,
			      কাঠামো task_struct *পরবর্তী)
{
	পূর্ব->dl_server = ফাঁকা;
	পরবর্তী->dl_server = rq->dl_server;
	rq->dl_server = ফাঁকা;
}

স্থির অন্তঃরেখ শূন্য put_পূর্ব_set_পরবর্তী_task(কাঠামো rq *rq,
					  কাঠামো task_struct *পূর্ব,
					  কাঠামো task_struct *পরবর্তী)
{
	WARN_ON_ONCE(rq->donor != পূর্ব);

	__put_prev_set_next_dl_server(rq, পূর্ব, পরবর্তী);

	যদি (পরবর্তী == পূর্ব) {
		পরবর্তী->sched_class->set_পরবর্তী_task(rq, পরবর্তী, SNT_REPICK);
		ফেরত;
	}

	পূর্ব->sched_class->put_পূর্ব_task(rq, পূর্ব, পরবর্তী);
	পরবর্তী->sched_class->set_পরবর্তী_task(rq, পরবর্তী, SNT_PICK);
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
ধ্রুবক কাঠামো sched_class name##_sched_class \
	__aligned(__alignof__(কাঠামো sched_class)) \
	__section("__" #name "_sched_class")

/* Defined in include/asm-generic/vmlinux.lds.h */
বাহ্যিক কাঠামো sched_class __sched_class_highest[];
বাহ্যিক কাঠামো sched_class __sched_class_lowest[];

বাহ্যিক ধ্রুবক কাঠামো sched_class stop_sched_class;
বাহ্যিক ধ্রুবক কাঠামো sched_class dl_sched_class;
বাহ্যিক ধ্রুবক কাঠামো sched_class rt_sched_class;
বাহ্যিক ধ্রুবক কাঠামো sched_class fair_sched_class;
বাহ্যিক ধ্রুবক কাঠামো sched_class idle_sched_class;

/*
 * Iterate only active classes. SCX can take over all fair tasks or be
 * completely disabled. If the former, skip fair. If the latter, skip SCX.
 */
স্থির অন্তঃরেখ ধ্রুবক কাঠামো sched_class *পরবর্তী_active_class(ধ্রুবক কাঠামো sched_class *class)
{
	class++;
#ifdef CONFIG_SCHED_CLASS_EXT
	যদি (scx_switched_all() && class == &fair_sched_class)
		class++;
	যদি (!scx_enabled() && class == &ext_sched_class)
		class++;
#endif
	ফেরত class;
}

#define for_class_range(class, _from, _to) \
	জন্য (class = (_from); class < (_to); class++)

#define for_each_class(class) \
	for_class_range(class, __sched_class_highest, __sched_class_lowest)

#define for_active_class_range(class, _from, _to)				\
	জন্য (class = (_from); class != (_to); class = পরবর্তী_active_class(class))

#define for_each_active_class(class)						\
	for_active_class_range(class, __sched_class_highest, __sched_class_lowest)

#define sched_class_above(_a, _b)	((_a) < (_b))

স্থির অন্তঃরেখ শূন্য rq_modified_begin(কাঠামো rq *rq, ধ্রুবক কাঠামো sched_class *class)
{
	যদি (sched_class_above(rq->পরবর্তী_class, class))
		rq->পরবর্তী_class = class;
}

স্থির অন্তঃরেখ bool rq_modified_above(কাঠামো rq *rq, ধ্রুবক কাঠামো sched_class *class)
{
	ফেরত sched_class_above(rq->পরবর্তী_class, class);
}

স্থির অন্তঃরেখ bool sched_stop_runnable(কাঠামো rq *rq)
{
	ফেরত rq->stop && task_on_rq_queued(rq->stop);
}

স্থির অন্তঃরেখ bool sched_dl_runnable(কাঠামো rq *rq)
{
	ফেরত rq->dl.dl_nr_running > 0;
}

স্থির অন্তঃরেখ bool sched_rt_runnable(কাঠামো rq *rq)
{
	ফেরত rq->rt.rt_queued > 0;
}

স্থির অন্তঃরেখ bool sched_fair_runnable(কাঠামো rq *rq)
{
	ফেরত rq->cfs.nr_queued > 0;
}

বাহ্যিক কাঠামো task_struct *pick_task_fair(কাঠামো rq *rq, কাঠামো rq_flags *rf);
বাহ্যিক কাঠামো task_struct *pick_task_idle(কাঠামো rq *rq, কাঠামো rq_flags *rf);

#define SCA_CHECK		0x01
#define SCA_MIGRATE_DISABLE	0x02
#define SCA_MIGRATE_ENABLE	0x04
#define SCA_USER		0x08

বাহ্যিক শূন্য update_group_capacity(কাঠামো sched_domain *sd, পূর্ণসংখ্যা cpu);

বাহ্যিক শূন্য sched_balance_trigger(কাঠামো rq *rq);

বাহ্যিক পূর্ণসংখ্যা __set_cpus_allowed_ptr(কাঠামো task_struct *p, কাঠামো affinity_context *ctx);
বাহ্যিক শূন্য set_cpus_allowed_common(কাঠামো task_struct *p, কাঠামো affinity_context *ctx);

স্থির অন্তঃরেখ bool task_allowed_on_cpu(কাঠামো task_struct *p, পূর্ণসংখ্যা cpu)
{
	/* When not in the task's cpumask, no point in looking further. */
	যদি (!cpumask_test_cpu(cpu, p->cpus_ptr))
		ফেরত false;

	/* Can @cpu run a user thread? */
	যদি (!(p->flags & PF_KTHREAD) && !task_cpu_possible(cpu, p))
		ফেরত false;

	ফেরত true;
}

স্থির অন্তঃরেখ cpumask_t *alloc_user_cpus_ptr(পূর্ণসংখ্যা গ্রন্থি)
{
	/*
	 * See set_cpus_allowed_force() above for the rcu_head usage.
	 */
	পূর্ণসংখ্যা আকার_2 = max_t(পূর্ণসংখ্যা, cpumask_আকার(), আকার(কাঠামো rcu_শীর্ষ));

	ফেরত kmalloc_গ্রন্থি(আকার_2, GFP_KERNEL, গ্রন্থি);
}

স্থির অন্তঃরেখ কাঠামো task_struct *get_ঠেলো_task(কাঠামো rq *rq)
{
	কাঠামো task_struct *p = rq->donor;

	lockdep_assert_rq_held(rq);

	যদি (rq->ঠেলো_busy)
		ফেরত ফাঁকা;

	যদি (p->nr_cpus_allowed == 1)
		ফেরত ফাঁকা;

	যদি (p->migration_disabled)
		ফেরত ফাঁকা;

	rq->ঠেলো_busy = true;
	ফেরত get_task_struct(p);
}

বাহ্যিক পূর্ণসংখ্যা ঠেলো_cpu_stop(শূন্য *arg);

#ifdef CONFIG_CPU_IDLE

স্থির অন্তঃরেখ শূন্য idle_set_state(কাঠামো rq *rq,
				  কাঠামো cpuidle_state *idle_state)
{
	rq->idle_state = idle_state;
}

স্থির অন্তঃরেখ কাঠামো cpuidle_state *idle_get_state(কাঠামো rq *rq)
{
	lockdep_assert(rcu_read_lock_any_held());

	ফেরত rq->idle_state;
}

#অন্যথা /* !CONFIG_CPU_IDLE: */

স্থির অন্তঃরেখ শূন্য idle_set_state(কাঠামো rq *rq,
				  কাঠামো cpuidle_state *idle_state)
{
}

স্থির অন্তঃরেখ কাঠামো cpuidle_state *idle_get_state(কাঠামো rq *rq)
{
	ফেরত ফাঁকা;
}

#endif /* !CONFIG_CPU_IDLE */

বাহ্যিক শূন্য schedule_idle(শূন্য);
asmlinkage শূন্য schedule_user(শূন্য);

বাহ্যিক শূন্য sysrq_sched_debug_show(শূন্য);
বাহ্যিক শূন্য sched_init_granularity(শূন্য);
বাহ্যিক শূন্য update_max_interval(শূন্য);

বাহ্যিক শূন্য init_sched_dl_class(শূন্য);
বাহ্যিক শূন্য init_sched_rt_class(শূন্য);
বাহ্যিক শূন্য init_sched_fair_class(শূন্য);

বাহ্যিক শূন্য resched_curr(কাঠামো rq *rq);
বাহ্যিক শূন্য resched_curr_lazy(কাঠামো rq *rq);
বাহ্যিক শূন্য resched_cpu(পূর্ণসংখ্যা cpu);

বাহ্যিক শূন্য init_rt_bandwidth(কাঠামো rt_bandwidth *rt_b, u64 period, u64 runtime);
বাহ্যিক bool sched_rt_bandwidth_account(কাঠামো rt_rq *rt_rq);

বাহ্যিক শূন্য init_dl_entity(কাঠামো sched_dl_entity *dl_se);

বাহ্যিক শূন্য init_cfs_throttle_work(কাঠামো task_struct *p);

#define BW_SHIFT		20
#define BW_UNIT			(1 << BW_SHIFT)
#define RATIO_SHIFT		8
#define MAX_BW_BITS		(64 - BW_SHIFT)
#define MAX_BW			((1ULL << MAX_BW_BITS) - 1)

বাহ্যিক u64 to_ratio(u64 period, u64 runtime);

বাহ্যিক শূন্য init_entity_runnable_average(কাঠামো sched_entity *se);
বাহ্যিক শূন্য post_init_entity_util_avg(কাঠামো task_struct *p);

#ifdef CONFIG_NO_HZ_FULL
বাহ্যিক bool sched_can_stop_tick(কাঠামো rq *rq);
বাহ্যিক পূর্ণসংখ্যা __init sched_tick_offload_init(শূন্য);

/*
 * Tick may be needed by tasks in the runqueue depending on their policy and
 * requirements. If tick is needed, lets send the target an IPI to kick it out of
 * nohz mode if necessary.
 */
স্থির অন্তঃরেখ শূন্য sched_update_tick_dependency(কাঠামো rq *rq)
{
	পূর্ণসংখ্যা cpu = cpu_of(rq);

	যদি (!tick_nohz_full_cpu(cpu))
		ফেরত;

	যদি (sched_can_stop_tick(rq))
		tick_nohz_dep_clear_cpu(cpu, TICK_DEP_BIT_SCHED);
	অন্যথা
		tick_nohz_dep_set_cpu(cpu, TICK_DEP_BIT_SCHED);
}
#অন্যথা /* !CONFIG_NO_HZ_FULL: */
স্থির অন্তঃরেখ পূর্ণসংখ্যা sched_tick_offload_init(শূন্য) { ফেরত 0; }
স্থির অন্তঃরেখ শূন্য sched_update_tick_dependency(কাঠামো rq *rq) { }
#endif /* !CONFIG_NO_HZ_FULL */

স্থির অন্তঃরেখ শূন্য add_nr_running(কাঠামো rq *rq, unsigned গুনতি)
{
	unsigned পূর্ব_nr = rq->nr_running;

	rq->nr_running = পূর্ব_nr + গুনতি;
	যদি (trace_sched_update_nr_running_tp_enabled()) {
		call_trace_sched_update_nr_running(rq, গুনতি);
	}

	যদি (পূর্ব_nr < 2 && rq->nr_running >= 2)
		set_rd_overloaded(rq->rd, 1);

	sched_update_tick_dependency(rq);
}

স্থির অন্তঃরেখ শূন্য sub_nr_running(কাঠামো rq *rq, unsigned গুনতি)
{
	rq->nr_running -= গুনতি;
	যদি (trace_sched_update_nr_running_tp_enabled()) {
		call_trace_sched_update_nr_running(rq, -গুনতি);
	}

	/* Check if we still need preemption */
	sched_update_tick_dependency(rq);
}

স্থির অন্তঃরেখ শূন্য __block_task(কাঠামো rq *rq, কাঠামো task_struct *p)
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

বাহ্যিক শূন্য activate_task(কাঠামো rq *rq, কাঠামো task_struct *p, পূর্ণসংখ্যা flags);
বাহ্যিক শূন্য deactivate_task(কাঠামো rq *rq, কাঠামো task_struct *p, পূর্ণসংখ্যা flags);

বাহ্যিক শূন্য wakeup_preempt(কাঠামো rq *rq, কাঠামো task_struct *p, পূর্ণসংখ্যা flags);

/*
 * attach_task() -- attach the task detached by detach_task() to its new rq.
 */
স্থির অন্তঃরেখ শূন্য attach_task(কাঠামো rq *rq, কাঠামো task_struct *p)
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
স্থির অন্তঃরেখ শূন্য attach_one_task(কাঠামো rq *rq, কাঠামো task_struct *p)
{
	guard(rq_lock)(rq);
	update_rq_clock(rq);
	attach_task(rq, p);
}

/*
 * __attach_tasks() - attaches a list of tasks (using se.group_node) to
 * the new rq
 */
স্থির অন্তঃরেখ শূন্য __attach_tasks(কাঠামো rq *rq, কাঠামো list_শীর্ষ *tasks)
{
	guard(rq_lock)(rq);
	update_rq_clock(rq);

	যখন (!list_empty(tasks)) {
		কাঠামো task_struct *p;

		p = list_first_entry(tasks, কাঠামো task_struct, se.group_গ্রন্থি);
		list_del_init(&p->se.group_গ্রন্থি);

		attach_task(rq, p);
	}
}

#ifdef CONFIG_PREEMPT_RT
# define SCHED_NR_MIGRATE_BREAK 8
#অন্যথা
# define SCHED_NR_MIGRATE_BREAK 32
#endif

বাহ্যিক __read_mostly unsigned পূর্ণসংখ্যা sysctl_sched_nr_migrate;
বাহ্যিক __read_mostly unsigned পূর্ণসংখ্যা sysctl_sched_migration_cost;

বাহ্যিক unsigned পূর্ণসংখ্যা sysctl_sched_base_slice;

বাহ্যিক পূর্ণসংখ্যা sysctl_resched_latency_warn_ms;
বাহ্যিক পূর্ণসংখ্যা sysctl_resched_latency_warn_once;

বাহ্যিক unsigned পূর্ণসংখ্যা sysctl_sched_tunable_scaling;

বাহ্যিক unsigned পূর্ণসংখ্যা sysctl_numa_balancing_scan_delay;
বাহ্যিক unsigned পূর্ণসংখ্যা sysctl_numa_balancing_scan_period_min;
বাহ্যিক unsigned পূর্ণসংখ্যা sysctl_numa_balancing_scan_period_max;
বাহ্যিক unsigned পূর্ণসংখ্যা sysctl_numa_balancing_scan_আকার;
বাহ্যিক unsigned পূর্ণসংখ্যা sysctl_numa_balancing_hot_threshold;

#ifdef CONFIG_SCHED_HRTICK

/*
 * Use hrtick when:
 *  - enabled by features
 *  - hrtimer is actually high res
 */
স্থির অন্তঃরেখ bool hrtick_enabled(কাঠামো rq *rq)
{
	ফেরত cpu_active(cpu_of(rq)) && hrtimer_highres_enabled();
}

স্থির অন্তঃরেখ bool hrtick_enabled_fair(কাঠামো rq *rq)
{
	ফেরত sched_feat(HRTICK) && hrtick_enabled(rq);
}

স্থির অন্তঃরেখ bool hrtick_enabled_dl(কাঠামো rq *rq)
{
	ফেরত sched_feat(HRTICK_DL) && hrtick_enabled(rq);
}

বাহ্যিক শূন্য hrtick_start(কাঠামো rq *rq, u64 delay);
স্থির অন্তঃরেখ bool hrtick_active(কাঠামো rq *rq)
{
	ফেরত hrtimer_active(&rq->hrtick_timer);
}

#অন্যথা /* !CONFIG_SCHED_HRTICK: */
স্থির অন্তঃরেখ bool hrtick_enabled_fair(কাঠামো rq *rq) { ফেরত false; }
স্থির অন্তঃরেখ bool hrtick_enabled_dl(কাঠামো rq *rq) { ফেরত false; }
স্থির অন্তঃরেখ bool hrtick_enabled(কাঠামো rq *rq) { ফেরত false; }
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
unsigned long arch_scale_freq_capacity(পূর্ণসংখ্যা cpu)
{
	ফেরত SCHED_CAPACITY_SCALE;
}
#endif

/*
 * In double_lock_balance()/double_rq_lock(), we use raw_spin_rq_lock() to
 * acquire rq lock instead of rq_lock(). So at the end of these two functions
 * we need to call double_rq_clock_clear_update() to clear RQCF_UPDATED of
 * rq->clock_update_flags to avoid the WARN_DOUBLE_CLOCK warning.
 */
স্থির অন্তঃরেখ শূন্য double_rq_clock_clear_update(কাঠামো rq *rq1, কাঠামো rq *rq2)
{
	rq1->clock_update_flags &= (RQCF_REQ_SKIP|RQCF_ACT_SKIP);
	rq2->clock_update_flags &= (RQCF_REQ_SKIP|RQCF_ACT_SKIP);
}

#define DEFINE_LOCK_GUARD_2(name, type, _lock, _unlock, ...)				\
__DEFINE_UNLOCK_GUARD(name, type, _unlock, type *lock2; __VA_ARGS__)			\
স্থির অন্তঃরেখ class_##name##_t class_##name##_constructor(type *lock, type *lock2)	\
	__no_context_analysis								\
{ class_##name##_t _t = { .lock = lock, .lock2 = lock2 }, *_T = &_t;			\
  _lock; ফেরত _t; }
#define DECLARE_LOCK_GUARD_2_ATTRS(_name, _lock, _unlock1, _unlock2)			\
স্থির অন্তঃরেখ class_##_name##_t class_##_name##_constructor(lock_##_name##_t *_T1,	\
							    lock_##_name##_t *_T2) _lock; \
স্থির __always_inline শূন্য __class_##_name##_cleanup_ctx1(class_##_name##_t **_T1)	\
	__no_context_analysis _unlock1 { }						\
স্থির __always_inline শূন্য __class_##_name##_cleanup_ctx2(class_##_name##_t **_T2)	\
	__no_context_analysis _unlock2 { }
#define WITH_LOCK_GUARD_2_ATTRS(_name, _T1, _T2)					\
	class_##_name##_constructor(_T1, _T2),						\
	*__UNIQUE_ID(unlock1) __cleanup(__class_##_name##_cleanup_ctx1) = (শূন্য *)(_T1),\
	*__UNIQUE_ID(unlock2) __cleanup(__class_##_name##_cleanup_ctx2) = (শূন্য *)(_T2)

স্থির অন্তঃরেখ bool rq_order_less(কাঠামো rq *rq1, কাঠামো rq *rq2)
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
		ফেরত true;
	যদি (rq1->core->cpu > rq2->core->cpu)
		ফেরত false;

	/*
	 * __sched_core_flip() relies on SMT having cpu-id lock order.
	 */
#endif /* CONFIG_SCHED_CORE */
	ফেরত rq1->cpu < rq2->cpu;
}

বাহ্যিক শূন্য double_rq_lock(কাঠামো rq *rq1, কাঠামো rq *rq2)
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
স্থির অন্তঃরেখ পূর্ণসংখ্যা _double_lock_balance(কাঠামো rq *this_rq, কাঠামো rq *busiest)
	__must_hold(__rq_lockp(this_rq))
	__acquires(__rq_lockp(busiest))
{
	raw_spin_rq_unlock(this_rq);
	double_rq_lock(this_rq, busiest);

	ফেরত 1;
}

#অন্যথা /* !CONFIG_PREEMPTION: */
/*
 * Unfair double_lock_balance: Optimizes throughput at the expense of
 * latency by eliminating extra atomic operations when the locks are
 * already in proper order on entry.  This favors lower CPU-ids and will
 * grant the double lock to lower CPUs over higher ids under contention,
 * regardless of entry order into the function.
 */
স্থির অন্তঃরেখ পূর্ণসংখ্যা _double_lock_balance(কাঠামো rq *this_rq, কাঠামো rq *busiest)
	__must_hold(__rq_lockp(this_rq))
	__acquires(__rq_lockp(busiest))
{
	যদি (__rq_lockp(this_rq) == __rq_lockp(busiest)) {
		__acquire(__rq_lockp(busiest)); /* already held */
		double_rq_clock_clear_update(this_rq, busiest);
		ফেরত 0;
	}

	যদি (likely(raw_spin_rq_trylock(busiest))) {
		double_rq_clock_clear_update(this_rq, busiest);
		ফেরত 0;
	}

	যদি (rq_order_less(this_rq, busiest)) {
		raw_spin_rq_lock_nested(busiest, SINGLE_DEPTH_NESTING);
		double_rq_clock_clear_update(this_rq, busiest);
		ফেরত 0;
	}

	raw_spin_rq_unlock(this_rq);
	double_rq_lock(this_rq, busiest);

	ফেরত 1;
}

#endif /* !CONFIG_PREEMPTION */

/*
 * double_lock_balance - lock the busiest runqueue, this_rq is locked already.
 */
স্থির অন্তঃরেখ পূর্ণসংখ্যা double_lock_balance(কাঠামো rq *this_rq, কাঠামো rq *busiest)
	__must_hold(__rq_lockp(this_rq))
	__acquires(__rq_lockp(busiest))
{
	lockdep_assert_irqs_disabled();

	ফেরত _double_lock_balance(this_rq, busiest);
}

স্থির অন্তঃরেখ শূন্য double_unlock_balance(কাঠামো rq *this_rq, কাঠামো rq *busiest)
	__releases(__rq_lockp(busiest))
{
	যদি (__rq_lockp(this_rq) != __rq_lockp(busiest))
		raw_spin_rq_unlock(busiest);
	অন্যথা
		__release(__rq_lockp(busiest)); /* fake release */
	lock_set_subclass(&__rq_lockp(this_rq)->dep_map, 0, _RET_IP_);
}

স্থির অন্তঃরেখ শূন্য double_lock(spinlock_t *l1, spinlock_t *l2)
	__acquires(l1, l2)
{
	যদি (l1 > l2)
		swap(l1, l2);

	spin_lock(l1);
	spin_lock_nested(l2, SINGLE_DEPTH_NESTING);
}

স্থির অন্তঃরেখ শূন্য double_lock_irq(spinlock_t *l1, spinlock_t *l2)
	__acquires(l1, l2)
{
	যদি (l1 > l2)
		swap(l1, l2);

	spin_lock_irq(l1);
	spin_lock_nested(l2, SINGLE_DEPTH_NESTING);
}

স্থির অন্তঃরেখ শূন্য double_raw_lock(raw_spinlock_t *l1, raw_spinlock_t *l2)
	__acquires(l1, l2)
{
	যদি (l1 > l2)
		swap(l1, l2);

	raw_spin_lock(l1);
	raw_spin_lock_nested(l2, SINGLE_DEPTH_NESTING);
}

স্থির অন্তঃরেখ শূন্য double_raw_unlock(raw_spinlock_t *l1, raw_spinlock_t *l2)
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
স্থির অন্তঃরেখ শূন্য double_rq_unlock(কাঠামো rq *rq1, কাঠামো rq *rq2)
	__releases(__rq_lockp(rq1), __rq_lockp(rq2))
{
	যদি (__rq_lockp(rq1) != __rq_lockp(rq2))
		raw_spin_rq_unlock(rq2);
	অন্যথা
		__release(__rq_lockp(rq2)); /* fake release */
	raw_spin_rq_unlock(rq1);
}

বাহ্যিক শূন্য set_rq_online (কাঠামো rq *rq);
বাহ্যিক শূন্য set_rq_offline(কাঠামো rq *rq);

বাহ্যিক bool sched_smp_initialized;

DEFINE_LOCK_GUARD_2(double_rq_lock, কাঠামো rq,
		    double_rq_lock(_T->lock, _T->lock2),
		    double_rq_unlock(_T->lock, _T->lock2))

বাহ্যিক কাঠামো sched_entity *__pick_root_entity(কাঠামো cfs_rq *cfs_rq);
বাহ্যিক কাঠামো sched_entity *__pick_first_entity(কাঠামো cfs_rq *cfs_rq);
বাহ্যিক কাঠামো sched_entity *__pick_last_entity(কাঠামো cfs_rq *cfs_rq);

বাহ্যিক bool sched_debug_verbose;

বাহ্যিক শূন্য print_cfs_stats(কাঠামো seq_file *m, পূর্ণসংখ্যা cpu);
বাহ্যিক শূন্য print_rt_stats(কাঠামো seq_file *m, পূর্ণসংখ্যা cpu);
বাহ্যিক শূন্য print_dl_stats(কাঠামো seq_file *m, পূর্ণসংখ্যা cpu);
বাহ্যিক শূন্য print_cfs_rq(কাঠামো seq_file *m, পূর্ণসংখ্যা cpu, কাঠামো cfs_rq *cfs_rq);
বাহ্যিক শূন্য print_rt_rq(কাঠামো seq_file *m, পূর্ণসংখ্যা cpu, কাঠামো rt_rq *rt_rq);
বাহ্যিক শূন্য print_dl_rq(কাঠামো seq_file *m, পূর্ণসংখ্যা cpu, কাঠামো dl_rq *dl_rq);

বাহ্যিক শূন্য resched_latency_warn(পূর্ণসংখ্যা cpu, u64 latency);

#ifdef CONFIG_NUMA_BALANCING
বাহ্যিক শূন্য show_numa_stats(কাঠামো task_struct *p, কাঠামো seq_file *m);
বাহ্যিক শূন্য
print_numa_stats(কাঠামো seq_file *m, পূর্ণসংখ্যা গ্রন্থি, unsigned long tsf,
		 unsigned long tpf, unsigned long gsf, unsigned long gpf);
#endif /* CONFIG_NUMA_BALANCING */

বাহ্যিক শূন্য init_cfs_rq(কাঠামো cfs_rq *cfs_rq);
বাহ্যিক শূন্য init_rt_rq(কাঠামো rt_rq *rt_rq);
বাহ্যিক শূন্য init_dl_rq(কাঠামো dl_rq *dl_rq);

বাহ্যিক শূন্য cfs_bandwidth_usage_inc(শূন্য);
বাহ্যিক শূন্য cfs_bandwidth_usage_dec(শূন্য);

#ifdef CONFIG_NO_HZ_COMMON

#define NOHZ_BALANCE_KICK_BIT	0
#define NOHZ_STATS_KICK_BIT	1
#define NOHZ_NEWILB_KICK_BIT	2
#define NOHZ_পরবর্তী_KICK_BIT	3

/* Run sched_balance_domains() */
#define NOHZ_BALANCE_KICK	BIT(NOHZ_BALANCE_KICK_BIT)
/* Update blocked load */
#define NOHZ_STATS_KICK		BIT(NOHZ_STATS_KICK_BIT)
/* Update blocked load when entering idle */
#define NOHZ_NEWILB_KICK	BIT(NOHZ_NEWILB_KICK_BIT)
/* Update nohz.next_balance */
#define NOHZ_পরবর্তী_KICK		BIT(NOHZ_পরবর্তী_KICK_BIT)

#define NOHZ_KICK_MASK		(NOHZ_BALANCE_KICK | NOHZ_STATS_KICK | NOHZ_পরবর্তী_KICK)

#define nohz_flags(cpu)		(&cpu_rq(cpu)->nohz_flags)

বাহ্যিক শূন্য nohz_balance_exit_idle(কাঠামো rq *rq);
#অন্যথা /* !CONFIG_NO_HZ_COMMON: */
স্থির অন্তঃরেখ শূন্য nohz_balance_exit_idle(কাঠামো rq *rq) { }
#endif /* !CONFIG_NO_HZ_COMMON */

#ifdef CONFIG_NO_HZ_COMMON
বাহ্যিক শূন্য nohz_run_idle_balance(পূর্ণসংখ্যা cpu);
#অন্যথা
স্থির অন্তঃরেখ শূন্য nohz_run_idle_balance(পূর্ণসংখ্যা cpu) { }
#endif

#অন্তর্ভাব "stats.h"

#যদি defined(CONFIG_SCHED_CORE) && defined(CONFIG_SCHEDSTATS)

বাহ্যিক শূন্য __sched_core_account_forceidle(কাঠামো rq *rq);

স্থির অন্তঃরেখ শূন্য sched_core_account_forceidle(কাঠামো rq *rq)
{
	যদি (schedstat_enabled())
		__sched_core_account_forceidle(rq);
}

বাহ্যিক শূন্য __sched_core_tick(কাঠামো rq *rq);

স্থির অন্তঃরেখ শূন্য sched_core_tick(কাঠামো rq *rq)
{
	যদি (sched_core_enabled(rq) && schedstat_enabled())
		__sched_core_tick(rq);
}

#অন্যথা /* !(CONFIG_SCHED_CORE && CONFIG_SCHEDSTATS): */

স্থির অন্তঃরেখ শূন্য sched_core_account_forceidle(কাঠামো rq *rq) { }

স্থির অন্তঃরেখ শূন্য sched_core_tick(কাঠামো rq *rq) { }

#endif /* !(CONFIG_SCHED_CORE && CONFIG_SCHEDSTATS) */

#ifdef CONFIG_IRQ_TIME_ACCOUNTING

কাঠামো irqtime {
	u64			যোগ_2;
	u64			tick_delta;
	u64			irq_start_time;
	কাঠামো u64_stats_sync	sync;
};

DECLARE_PER_CPU(কাঠামো irqtime, cpu_irqtime);
DECLARE_STATIC_চাবি_FALSE(sched_clock_irqtime);

স্থির অন্তঃরেখ পূর্ণসংখ্যা irqtime_enabled(শূন্য)
{
	ফেরত static_branch_likely(&sched_clock_irqtime);
}

/*
 * Returns the irqtime minus the softirq time computed by ksoftirqd.
 * Otherwise ksoftirqd's sum_exec_runtime is subtracted its own runtime
 * and never move forward.
 */
স্থির অন্তঃরেখ u64 irq_time_read(পূর্ণসংখ্যা cpu)
{
	কাঠামো irqtime *irqtime = &per_cpu(cpu_irqtime, cpu);
	unsigned পূর্ণসংখ্যা seq;
	u64 যোগ_2;

	করো {
		seq = __u64_stats_fetch_begin(&irqtime->sync);
		যোগ_2 = irqtime->যোগ_2;
	} যখন (__u64_stats_fetch_retry(&irqtime->sync, seq));

	ফেরত যোগ_2;
}

#অন্যথা /* !CONFIG_IRQ_TIME_ACCOUNTING: */

স্থির অন্তঃরেখ পূর্ণসংখ্যা irqtime_enabled(শূন্য)
{
	ফেরত 0;
}

#endif /* !CONFIG_IRQ_TIME_ACCOUNTING */

#ifdef CONFIG_CPU_FREQ

DECLARE_PER_CPU(কাঠামো update_util_তথ্য __rcu *, cpufreq_update_util_তথ্য);

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
স্থির অন্তঃরেখ শূন্য cpufreq_update_util(কাঠামো rq *rq, unsigned পূর্ণসংখ্যা flags)
{
	কাঠামো update_util_তথ্য *তথ্য;

	তথ্য = rcu_dereference_sched(*per_cpu_ptr(&cpufreq_update_util_তথ্য,
						  cpu_of(rq)));
	যদি (তথ্য)
		তথ্য->func(তথ্য, rq_clock(rq), flags);
}
#অন্যথা /* !CONFIG_CPU_FREQ: */
স্থির অন্তঃরেখ শূন্য cpufreq_update_util(কাঠামো rq *rq, unsigned পূর্ণসংখ্যা flags) { }
#endif /* !CONFIG_CPU_FREQ */

#ifdef arch_scale_freq_capacity
# ifndef arch_scale_freq_invariant
#  define arch_scale_freq_invariant()	true
# endif
#অন্যথা
# define arch_scale_freq_invariant()	false
#endif

unsigned long effective_cpu_util(পূর্ণসংখ্যা cpu, unsigned long util_cfs,
				 unsigned long *min,
				 unsigned long *max);

unsigned long sugov_effective_cpu_perf(পূর্ণসংখ্যা cpu, unsigned long actual,
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
স্থির অন্তঃরেখ bool dl_task_fits_capacity(কাঠামো task_struct *p, পূর্ণসংখ্যা cpu)
{
	unsigned long cap = arch_scale_cpu_capacity(cpu);

	ফেরত cap >= p->dl.dl_density >> (BW_SHIFT - SCHED_CAPACITY_SHIFT);
}

স্থির অন্তঃরেখ unsigned long cpu_bw_dl(কাঠামো rq *rq)
{
	ফেরত (rq->dl.running_bw * SCHED_CAPACITY_SCALE) >> BW_SHIFT;
}

স্থির অন্তঃরেখ unsigned long cpu_util_dl(কাঠামো rq *rq)
{
	ফেরত READ_ONCE(rq->avg_dl.util_avg);
}


বাহ্যিক unsigned long cpu_util_cfs(পূর্ণসংখ্যা cpu);
বাহ্যিক unsigned long cpu_util_cfs_boost(পূর্ণসংখ্যা cpu);

স্থির অন্তঃরেখ unsigned long cpu_util_rt(কাঠামো rq *rq)
{
	ফেরত READ_ONCE(rq->avg_rt.util_avg);
}

#ifdef CONFIG_UCLAMP_TASK

unsigned long uclamp_eff_মান(কাঠামো task_struct *p, গণনা uclamp_id clamp_id);

/*
 * When uclamp is compiled in, the aggregation at rq level is 'turned off'
 * by default in the fast path and only gets turned on once userspace performs
 * an operation that requires it.
 *
 * Returns true if userspace opted-in to use uclamp and aggregation at rq level
 * hence is active.
 */
স্থির অন্তঃরেখ bool uclamp_is_used(শূন্য)
{
	ফেরত static_branch_likely(&sched_uclamp_used);
}

/*
 * Enabling static branches would get the cpus_read_lock(),
 * check whether uclamp_is_used before enable it to avoid always
 * calling cpus_read_lock(). Because we never disable this
 * static key once enable it.
 */
স্থির অন্তঃরেখ শূন্য sched_uclamp_enable(শূন্য)
{
	যদি (!uclamp_is_used())
		static_branch_enable(&sched_uclamp_used);
}

স্থির অন্তঃরেখ unsigned long uclamp_rq_get(কাঠামো rq *rq,
					  গণনা uclamp_id clamp_id)
{
	ফেরত READ_ONCE(rq->uclamp[clamp_id].মান);
}

স্থির অন্তঃরেখ শূন্য uclamp_rq_set(কাঠামো rq *rq, গণনা uclamp_id clamp_id,
				 unsigned পূর্ণসংখ্যা মান)
{
	WRITE_ONCE(rq->uclamp[clamp_id].মান, মান);
}

স্থির অন্তঃরেখ bool uclamp_rq_is_idle(কাঠামো rq *rq)
{
	ফেরত rq->uclamp_flags & UCLAMP_FLAG_IDLE;
}

/* Is the rq being capped/throttled by uclamp_max? */
স্থির অন্তঃরেখ bool uclamp_rq_is_capped(কাঠামো rq *rq)
{
	unsigned long rq_util;
	unsigned long max_util;

	যদি (!uclamp_is_used())
		ফেরত false;

	rq_util = cpu_util_cfs(cpu_of(rq)) + cpu_util_rt(rq);
	max_util = READ_ONCE(rq->uclamp[UCLAMP_MAX].মান);

	ফেরত max_util != SCHED_CAPACITY_SCALE && rq_util >= max_util;
}

#define for_each_clamp_id(clamp_id) \
	জন্য ((clamp_id) = 0; (clamp_id) < UCLAMP_CNT; (clamp_id)++)

বাহ্যিক unsigned পূর্ণসংখ্যা sysctl_sched_uclamp_util_min_rt_default;


স্থির অন্তঃরেখ unsigned পূর্ণসংখ্যা uclamp_none(গণনা uclamp_id clamp_id)
{
	যদি (clamp_id == UCLAMP_MIN)
		ফেরত 0;
	ফেরত SCHED_CAPACITY_SCALE;
}

/* Integer rounded range for each bucket */
#define UCLAMP_BUCKET_DELTA DIV_ROUND_CLOSEST(SCHED_CAPACITY_SCALE, UCLAMP_BUCKETS)

স্থির অন্তঃরেখ unsigned পূর্ণসংখ্যা uclamp_bucket_id(unsigned পূর্ণসংখ্যা clamp_মান)
{
	ফেরত min_t(unsigned পূর্ণসংখ্যা, clamp_মান / UCLAMP_BUCKET_DELTA, UCLAMP_BUCKETS - 1);
}

স্থির অন্তঃরেখ শূন্য
uclamp_se_set(কাঠামো uclamp_se *uc_se, unsigned পূর্ণসংখ্যা মান, bool user_defined)
{
	uc_se->মান = মান;
	uc_se->bucket_id = uclamp_bucket_id(মান);
	uc_se->user_defined = user_defined;
}

#অন্যথা /* !CONFIG_UCLAMP_TASK: */

স্থির অন্তঃরেখ unsigned long
uclamp_eff_মান(কাঠামো task_struct *p, গণনা uclamp_id clamp_id)
{
	যদি (clamp_id == UCLAMP_MIN)
		ফেরত 0;

	ফেরত SCHED_CAPACITY_SCALE;
}

স্থির অন্তঃরেখ bool uclamp_rq_is_capped(কাঠামো rq *rq) { ফেরত false; }

স্থির অন্তঃরেখ bool uclamp_is_used(শূন্য)
{
	ফেরত false;
}

স্থির অন্তঃরেখ শূন্য sched_uclamp_enable(শূন্য) {}

স্থির অন্তঃরেখ unsigned long
uclamp_rq_get(কাঠামো rq *rq, গণনা uclamp_id clamp_id)
{
	যদি (clamp_id == UCLAMP_MIN)
		ফেরত 0;

	ফেরত SCHED_CAPACITY_SCALE;
}

স্থির অন্তঃরেখ শূন্য
uclamp_rq_set(কাঠামো rq *rq, গণনা uclamp_id clamp_id, unsigned পূর্ণসংখ্যা মান)
{
}

স্থির অন্তঃরেখ bool uclamp_rq_is_idle(কাঠামো rq *rq)
{
	ফেরত false;
}

#endif /* !CONFIG_UCLAMP_TASK */

#ifdef CONFIG_HAVE_SCHED_AVG_IRQ

স্থির অন্তঃরেখ unsigned long cpu_util_irq(কাঠামো rq *rq)
{
	ফেরত READ_ONCE(rq->avg_irq.util_avg);
}

স্থির অন্তঃরেখ
unsigned long scale_irq_capacity(unsigned long util, unsigned long irq, unsigned long max)
{
	util *= (max - irq);
	util /= max;

	ফেরত util;

}

#অন্যথা /* !CONFIG_HAVE_SCHED_AVG_IRQ: */

স্থির অন্তঃরেখ unsigned long cpu_util_irq(কাঠামো rq *rq)
{
	ফেরত 0;
}

স্থির অন্তঃরেখ
unsigned long scale_irq_capacity(unsigned long util, unsigned long irq, unsigned long max)
{
	ফেরত util;
}

#endif /* !CONFIG_HAVE_SCHED_AVG_IRQ */

বাহ্যিক শূন্য __setparam_fair(কাঠামো task_struct *p, ধ্রুবক কাঠামো sched_attr *attr);

#যদি defined(CONFIG_ENERGY_MODEL) && defined(CONFIG_CPU_FREQ_GOV_SCHEDUTIL)

#define perf_domain_span(pd) (to_cpumask(((pd)->em_pd->cpus)))

DECLARE_STATIC_চাবি_FALSE(sched_energy_present);

স্থির অন্তঃরেখ bool sched_energy_enabled(শূন্য)
{
	ফেরত static_branch_unlikely(&sched_energy_present);
}

#অন্যথা /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL): */

#define perf_domain_span(pd) ফাঁকা

স্থির অন্তঃরেখ bool sched_energy_enabled(শূন্য) { ফেরত false; }

#endif /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL) */

#ifdef CONFIG_MEMBARRIER

/*
 * The scheduler provides memory barriers required by membarrier between:
 * - prior user-space memory accesses and store to rq->membarrier_state,
 * - store to rq->membarrier_state and following user-space memory accesses.
 * In the same way it provides those guarantees around store to rq->curr.
 */
স্থির অন্তঃরেখ শূন্য membarrier_switch_mm(কাঠামো rq *rq,
					কাঠামো mm_struct *পূর্ব_mm,
					কাঠামো mm_struct *পরবর্তী_mm)
{
	পূর্ণসংখ্যা membarrier_state;

	যদি (পূর্ব_mm == পরবর্তী_mm)
		ফেরত;

	membarrier_state = atomic_read(&পরবর্তী_mm->membarrier_state);
	যদি (READ_ONCE(rq->membarrier_state) == membarrier_state)
		ফেরত;

	WRITE_ONCE(rq->membarrier_state, membarrier_state);
}

#অন্যথা /* !CONFIG_MEMBARRIER: */

স্থির অন্তঃরেখ শূন্য membarrier_switch_mm(কাঠামো rq *rq,
					কাঠামো mm_struct *পূর্ব_mm,
					কাঠামো mm_struct *পরবর্তী_mm)
{
}

#endif /* !CONFIG_MEMBARRIER */

স্থির অন্তঃরেখ bool is_per_cpu_kthread(কাঠামো task_struct *p)
{
	যদি (!(p->flags & PF_KTHREAD))
		ফেরত false;

	যদি (p->nr_cpus_allowed != 1)
		ফেরত false;

	ফেরত true;
}

বাহ্যিক শূন্য swake_up_all_locked(কাঠামো swait_queue_শীর্ষ *q);
বাহ্যিক শূন্য __prepare_to_swait(কাঠামো swait_queue_শীর্ষ *q, কাঠামো swait_queue *wait);

বাহ্যিক পূর্ণসংখ্যা try_to_wake_up(কাঠামো task_struct *tsk, unsigned পূর্ণসংখ্যা state, পূর্ণসংখ্যা wake_flags);

#ifdef CONFIG_PREEMPT_DYNAMIC
বাহ্যিক পূর্ণসংখ্যা preempt_dynamic_mode;
বাহ্যিক পূর্ণসংখ্যা sched_dynamic_mode(ধ্রুবক অক্ষর *str);
বাহ্যিক শূন্য sched_dynamic_update(পূর্ণসংখ্যা mode);
#endif
বাহ্যিক ধ্রুবক অক্ষর *preempt_modes[];

#ifdef CONFIG_SCHED_MM_CID

স্থির __always_inline bool cid_on_cpu(unsigned পূর্ণসংখ্যা cid)
{
	ফেরত cid & MM_CID_ONCPU;
}

স্থির __always_inline bool cid_in_transit(unsigned পূর্ণসংখ্যা cid)
{
	ফেরত cid & MM_CID_TRANSIT;
}

স্থির __always_inline unsigned পূর্ণসংখ্যা cpu_cid_to_cid(unsigned পূর্ণসংখ্যা cid)
{
	ফেরত cid & ~MM_CID_ONCPU;
}

স্থির __always_inline unsigned পূর্ণসংখ্যা cid_to_cpu_cid(unsigned পূর্ণসংখ্যা cid)
{
	ফেরত cid | MM_CID_ONCPU;
}

স্থির __always_inline unsigned পূর্ণসংখ্যা cid_to_transit_cid(unsigned পূর্ণসংখ্যা cid)
{
	ফেরত cid | MM_CID_TRANSIT;
}

স্থির __always_inline unsigned পূর্ণসংখ্যা cid_from_transit_cid(unsigned পূর্ণসংখ্যা cid)
{
	ফেরত cid & ~MM_CID_TRANSIT;
}

স্থির __always_inline bool cid_on_task(unsigned পূর্ণসংখ্যা cid)
{
	/* True if none of the MM_CID_ONCPU, MM_CID_TRANSIT, MM_CID_UNSET bits is set */
	ফেরত cid < MM_CID_TRANSIT;
}

স্থির __always_inline শূন্য mm_drop_cid(কাঠামো mm_struct *mm, unsigned পূর্ণসংখ্যা cid)
{
	clear_bit(cid, mm_cidmask(mm));
}

স্থির __always_inline শূন্য mm_unset_cid_on_task(কাঠামো task_struct *t)
{
	unsigned পূর্ণসংখ্যা cid = t->mm_cid.cid;

	t->mm_cid.cid = MM_CID_UNSET;
	যদি (cid_on_task(cid))
		mm_drop_cid(t->mm, cid);
}

স্থির __always_inline শূন্য mm_drop_cid_on_cpu(কাঠামো mm_struct *mm, কাঠামো mm_cid_pcpu *pcp)
{
	/* Clear the ONCPU bit, but do not set UNSET in the per CPU storage */
	যদি (cid_on_cpu(pcp->cid)) {
		pcp->cid = cpu_cid_to_cid(pcp->cid);
		mm_drop_cid(mm, pcp->cid);
	}
}

স্থির অন্তঃরেখ unsigned পূর্ণসংখ্যা __mm_get_cid(কাঠামো mm_struct *mm, unsigned পূর্ণসংখ্যা max_cids)
{
	unsigned পূর্ণসংখ্যা cid = find_first_zero_bit(mm_cidmask(mm), max_cids);

	যদি (cid >= max_cids)
		ফেরত MM_CID_UNSET;
	যদি (test_and_set_bit(cid, mm_cidmask(mm)))
		ফেরত MM_CID_UNSET;
	ফেরত cid;
}

স্থির অন্তঃরেখ unsigned পূর্ণসংখ্যা mm_get_cid(কাঠামো mm_struct *mm)
{
	unsigned পূর্ণসংখ্যা cid = __mm_get_cid(mm, READ_ONCE(mm->mm_cid.max_cids));

	যখন (cid == MM_CID_UNSET) {
		cpu_relax();
		cid = __mm_get_cid(mm, অঙ্ক_possible_cpus());
	}
	ফেরত cid;
}

স্থির অন্তঃরেখ unsigned পূর্ণসংখ্যা mm_cid_converge(কাঠামো mm_struct *mm, unsigned পূর্ণসংখ্যা orig_cid,
					   unsigned পূর্ণসংখ্যা max_cids)
{
	unsigned পূর্ণসংখ্যা new_cid, cid = cpu_cid_to_cid(orig_cid);

	/* Is it in the optimal CID space? */
	যদি (likely(cid < max_cids))
		ফেরত orig_cid;

	/* Try to find one in the optimal space. Otherwise keep the provided. */
	new_cid = __mm_get_cid(mm, max_cids);
	যদি (new_cid != MM_CID_UNSET) {
		mm_drop_cid(mm, cid);
		/* Preserve the ONCPU mode of the original CID */
		ফেরত new_cid | (orig_cid & MM_CID_ONCPU);
	}
	ফেরত orig_cid;
}

স্থির __always_inline শূন্য mm_cid_update_task_cid(কাঠামো task_struct *t, unsigned পূর্ণসংখ্যা cid)
{
	যদি (t->mm_cid.cid != cid) {
		t->mm_cid.cid = cid;
		rseq_sched_set_ids_changed(t);
	}
}

স্থির __always_inline শূন্য mm_cid_update_pcpu_cid(কাঠামো mm_struct *mm, unsigned পূর্ণসংখ্যা cid)
{
	__this_cpu_write(mm->mm_cid.pcpu->cid, cid);
}

স্থির __always_inline শূন্য mm_cid_from_cpu(কাঠামো task_struct *t, unsigned পূর্ণসংখ্যা cpu_cid,
					    unsigned পূর্ণসংখ্যা mode)
{
	unsigned পূর্ণসংখ্যা max_cids, tcid = t->mm_cid.cid;
	কাঠামো mm_struct *mm = t->mm;

	max_cids = READ_ONCE(mm->mm_cid.max_cids);
	/* Optimize for the common case where both have the ONCPU bit set */
	যদি (likely(cid_on_cpu(cpu_cid & tcid))) {
		যদি (likely(cpu_cid_to_cid(cpu_cid) < max_cids)) {
			mm_cid_update_task_cid(t, cpu_cid);
			ফেরত;
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

স্থির __always_inline শূন্য mm_cid_from_task(কাঠামো task_struct *t, unsigned পূর্ণসংখ্যা cpu_cid,
					     unsigned পূর্ণসংখ্যা mode)
{
	unsigned পূর্ণসংখ্যা max_cids, tcid = t->mm_cid.cid;
	কাঠামো mm_struct *mm = t->mm;

	max_cids = READ_ONCE(mm->mm_cid.max_cids);
	/* Optimize for the common case, where both have the ONCPU bit clear */
	যদি (likely(cid_on_task(tcid | cpu_cid))) {
		যদি (likely(tcid < max_cids)) {
			mm_cid_update_pcpu_cid(mm, tcid);
			ফেরত;
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

স্থির __always_inline শূন্য mm_cid_schedin(কাঠামো task_struct *পরবর্তী)
{
	কাঠামো mm_struct *mm = পরবর্তী->mm;
	unsigned পূর্ণসংখ্যা cpu_cid, mode;

	যদি (!পরবর্তী->mm_cid.active)
		ফেরত;

	cpu_cid = __this_cpu_read(mm->mm_cid.pcpu->cid);
	mode = READ_ONCE(mm->mm_cid.mode);
	যদি (likely(!cid_on_cpu(mode)))
		mm_cid_from_task(পরবর্তী, cpu_cid, mode);
	অন্যথা
		mm_cid_from_cpu(পরবর্তী, cpu_cid, mode);
}

স্থির __always_inline শূন্য mm_cid_schedout(কাঠামো task_struct *পূর্ব)
{
	কাঠামো mm_struct *mm = পূর্ব->mm;
	unsigned পূর্ণসংখ্যা mode, cid;

	/* During mode transitions CIDs are temporary and need to be dropped */
	যদি (likely(!cid_in_transit(পূর্ব->mm_cid.cid)))
		ফেরত;

	mode = READ_ONCE(mm->mm_cid.mode);
	cid = cid_from_transit_cid(পূর্ব->mm_cid.cid);

	/*
	 * If transition mode is done, transfer ownership when the CID is
	 * within the convergence range to optimize the next schedule in.
	 */
	যদি (!cid_in_transit(mode) && cid < READ_ONCE(mm->mm_cid.max_cids)) {
		যদি (cid_on_cpu(mode))
			cid = cid_to_cpu_cid(cid);

		/* Update both so that the next schedule in goes into the fast path */
		mm_cid_update_pcpu_cid(mm, cid);
		পূর্ব->mm_cid.cid = cid;
	} অন্যথা {
		mm_drop_cid(mm, cid);
		পূর্ব->mm_cid.cid = MM_CID_UNSET;
	}
}

স্থির অন্তঃরেখ শূন্য mm_cid_switch_to(কাঠামো task_struct *পূর্ব, কাঠামো task_struct *পরবর্তী)
{
	mm_cid_schedout(পূর্ব);
	mm_cid_schedin(পরবর্তী);
}

#অন্যথা /* !CONFIG_SCHED_MM_CID: */
স্থির অন্তঃরেখ শূন্য mm_cid_switch_to(কাঠামো task_struct *পূর্ব, কাঠামো task_struct *পরবর্তী) { }
#endif /* !CONFIG_SCHED_MM_CID */

#ifdef CONFIG_SCHED_CACHE
DECLARE_STATIC_চাবি_FALSE(sched_cache_present);
DECLARE_STATIC_চাবি_FALSE(sched_cache_active);
বাহ্যিক পূর্ণসংখ্যা sysctl_sched_cache_user;
বাহ্যিক unsigned পূর্ণসংখ্যা llc_aggr_tolerance;
বাহ্যিক unsigned পূর্ণসংখ্যা llc_epoch_period;
বাহ্যিক unsigned পূর্ণসংখ্যা llc_epoch_affinity_timeout;
বাহ্যিক unsigned পূর্ণসংখ্যা llc_imb_pct;
বাহ্যিক unsigned পূর্ণসংখ্যা llc_overaggr_pct;

স্থির অন্তঃরেখ bool sched_cache_enabled(শূন্য)
{
	ফেরত static_branch_unlikely(&sched_cache_active);
}

বাহ্যিক শূন্য sched_cache_active_set(শূন্য);

#endif

শূন্য sched_domains_free_llc_id(পূর্ণসংখ্যা cpu);

বাহ্যিক শূন্য init_sched_mm(কাঠামো task_struct *p);

বাহ্যিক u64 avg_vruntime(কাঠামো cfs_rq *cfs_rq);
বাহ্যিক পূর্ণসংখ্যা entity_eligible(কাঠামো cfs_rq *cfs_rq, কাঠামো sched_entity *se);
স্থির অন্তঃরেখ
শূন্য move_queued_task_locked(কাঠামো rq *src_rq, কাঠামো rq *dst_rq, কাঠামো task_struct *task)
{
	lockdep_assert_rq_held(src_rq);
	lockdep_assert_rq_held(dst_rq);

	deactivate_task(src_rq, task, 0);
	set_task_cpu(task, dst_rq->cpu);
	activate_task(dst_rq, task, 0);
	wakeup_preempt(dst_rq, task, 0);
}

স্থির অন্তঃরেখ
bool task_is_pushable(কাঠামো rq *rq, কাঠামো task_struct *p, পূর্ণসংখ্যা cpu)
{
	যদি (!task_on_cpu(rq, p) &&
	    cpumask_test_cpu(cpu, &p->cpus_mask))
		ফেরত true;

	ফেরত false;
}

#ifdef CONFIG_RT_MUTEXES

স্থির অন্তঃরেখ পূর্ণসংখ্যা __rt_effective_prio(কাঠামো task_struct *pi_task, পূর্ণসংখ্যা prio)
{
	যদি (pi_task)
		prio = min(prio, pi_task->prio);

	ফেরত prio;
}

স্থির অন্তঃরেখ পূর্ণসংখ্যা rt_effective_prio(কাঠামো task_struct *p, পূর্ণসংখ্যা prio)
{
	কাঠামো task_struct *pi_task = rt_mutex_get_শীর্ষ_task(p);

	ফেরত __rt_effective_prio(pi_task, prio);
}

#অন্যথা /* !CONFIG_RT_MUTEXES: */

স্থির অন্তঃরেখ পূর্ণসংখ্যা rt_effective_prio(কাঠামো task_struct *p, পূর্ণসংখ্যা prio)
{
	ফেরত prio;
}

#endif /* !CONFIG_RT_MUTEXES */

বাহ্যিক পূর্ণসংখ্যা __sched_setscheduler(কাঠামো task_struct *p, ধ্রুবক কাঠামো sched_attr *attr, bool user, bool pi);
বাহ্যিক পূর্ণসংখ্যা __sched_setaffinity(কাঠামো task_struct *p, কাঠামো affinity_context *ctx);
বাহ্যিক ধ্রুবক কাঠামো sched_class *__setscheduler_class(পূর্ণসংখ্যা policy, পূর্ণসংখ্যা prio);
বাহ্যিক শূন্য set_load_weight(কাঠামো task_struct *p, bool update_load);
বাহ্যিক শূন্য enqueue_task(কাঠামো rq *rq, কাঠামো task_struct *p, পূর্ণসংখ্যা flags);
বাহ্যিক bool dequeue_task(কাঠামো rq *rq, কাঠামো task_struct *p, পূর্ণসংখ্যা flags);

বাহ্যিক কাঠামো balance_callback *splice_balance_callbacks(কাঠামো rq *rq);

বাহ্যিক শূন্য __balance_callbacks(কাঠামো rq *rq, কাঠামো rq_flags *rf);
বাহ্যিক শূন্য balance_callbacks(কাঠামো rq *rq, কাঠামো balance_callback *শীর্ষ);

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
কাঠামো sched_change_ctx {
	u64			prio;
	কাঠামো task_struct	*p;
	ধ্রুবক কাঠামো sched_class *class;
	পূর্ণসংখ্যা			flags;
	bool			queued;
	bool			running;
};

কাঠামো sched_change_ctx *sched_change_begin(কাঠামো task_struct *p, unsigned পূর্ণসংখ্যা flags);
শূন্য sched_change_end(কাঠামো sched_change_ctx *ctx);

DEFINE_CLASS(sched_change, কাঠামো sched_change_ctx *,
	     sched_change_end(_T),
	     sched_change_begin(p, flags),
	     কাঠামো task_struct *p, unsigned পূর্ণসংখ্যা flags)

DEFINE_CLASS_IS_UNCONDITIONAL(sched_change)

#অন্তর্ভাব "ext/ext.h"

#ifdef CONFIG_PREFERRED_CPU
শূন্য sched_ঠেলো_current_non_preferred_cpu(কাঠামো rq *rq);
#অন্যথা	/* !CONFIG_PREFERRED_CPU */
স্থির অন্তঃরেখ শূন্য sched_ঠেলো_current_non_preferred_cpu(কাঠামো rq *rq) { }
#endif

#endif /* _KERNEL_SCHED_SCHED_H */
