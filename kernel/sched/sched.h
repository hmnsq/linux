/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Scheduler internal types and methods:
 */
#ifndef _KERNEL_SCHED_SCHED_H
#define _KERNEL_SCHED_SCHED_H

#அந்தர்பாவ <linux/prandom.h>
#அந்தர்பாவ <linux/sched/affinity.h>
#அந்தர்பாவ <linux/sched/autogroup.h>
#அந்தர்பாவ <linux/sched/cpufreq.h>
#அந்தர்பாவ <linux/sched/deadline.h>
#அந்தர்பாவ <linux/sched.h>
#அந்தர்பாவ <linux/sched/loadavg.h>
#அந்தர்பாவ <linux/sched/mm.h>
#அந்தர்பாவ <linux/sched/rseq_api.h>
#அந்தர்பாவ <linux/sched/signal.h>
#அந்தர்பாவ <linux/sched/smt.h>
#அந்தர்பாவ <linux/sched/stat.h>
#அந்தர்பாவ <linux/sched/sysctl.h>
#அந்தர்பாவ <linux/sched/task_flags.h>
#அந்தர்பாவ <linux/sched/task.h>
#அந்தர்பாவ <linux/sched/topology.h>
#அந்தர்பாவ <linux/atomic.h>
#அந்தர்பாவ <linux/bitmap.h>
#அந்தர்பாவ <linux/bug.h>
#அந்தர்பாவ <linux/capability.h>
#அந்தர்பாவ <linux/cgroup_api.h>
#அந்தர்பாவ <linux/cgroup.h>
#அந்தர்பாவ <linux/context_tracking.h>
#அந்தர்பாவ <linux/cpufreq.h>
#அந்தர்பாவ <linux/cpumask_api.h>
#அந்தர்பாவ <linux/cpuset.h>
#அந்தர்பாவ <linux/ctype.h>
#அந்தர்பாவ <linux/file.h>
#அந்தர்பாவ <linux/fs_api.h>
#அந்தர்பாவ <linux/hrtimer_api.h>
#அந்தர்பாவ <linux/interrupt.h>
#அந்தர்பாவ <linux/irq_work.h>
#அந்தர்பாவ <linux/jiffies.h>
#அந்தர்பாவ <linux/kref_api.h>
#அந்தர்பாவ <linux/kthread.h>
#அந்தர்பாவ <linux/ktime_api.h>
#அந்தர்பாவ <linux/lockdep_api.h>
#அந்தர்பாவ <linux/lockdep.h>
#அந்தர்பாவ <linux/memblock.h>
#அந்தர்பாவ <linux/memcontrol.h>
#அந்தர்பாவ <linux/minmax.h>
#அந்தர்பாவ <linux/mm.h>
#அந்தர்பாவ <linux/module.h>
#அந்தர்பாவ <linux/mutex_api.h>
#அந்தர்பாவ <linux/plist.h>
#அந்தர்பாவ <linux/poll.h>
#அந்தர்பாவ <linux/proc_fs.h>
#அந்தர்பாவ <linux/profile.h>
#அந்தர்பாவ <linux/psi.h>
#அந்தர்பாவ <linux/rcupdate.h>
#அந்தர்பாவ <linux/seq_file.h>
#அந்தர்பாவ <linux/seqlock.h>
#அந்தர்பாவ <linux/softirq.h>
#அந்தர்பாவ <linux/spinlock_api.h>
#அந்தர்பாவ <linux/static_key.h>
#அந்தர்பாவ <linux/stop_machine.h>
#அந்தர்பாவ <linux/syscalls_api.h>
#அந்தர்பாவ <linux/syscalls.h>
#அந்தர்பாவ <linux/tick.h>
#அந்தர்பாவ <linux/topology.h>
#அந்தர்பாவ <linux/types.h>
#அந்தர்பாவ <linux/u64_stats_sync_api.h>
#அந்தர்பாவ <linux/uaccess.h>
#அந்தர்பாவ <linux/vmstat.h>
#அந்தர்பாவ <linux/wait_api.h>
#அந்தர்பாவ <linux/wait_bit.h>
#அந்தர்பாவ <linux/workqueue_api.h>
#அந்தர்பாவ <linux/delayacct.h>
#அந்தர்பாவ <linux/mmu_context.h>

#அந்தர்பாவ <trace/events/power.h>
#அந்தர்பாவ <trace/events/sched.h>

#அந்தர்பாவ "../workqueue_internal.h"

கட்டமைப்பு rq;
கட்டமைப்பு cfs_rq;
கட்டமைப்பு rt_rq;
கட்டமைப்பு sched_group;
கட்டமைப்பு cpuidle_state;

#எனில் defined(CONFIG_PARAVIRT) && !defined(CONFIG_HAVE_PV_STEAL_CLOCK_GEN)
# include <asm/paravirt.h>
#endif

#அந்தர்பாவ <asm/barrier.h>

#அந்தர்பாவ "cpupri.h"
#அந்தர்பாவ "cpudeadline.h"

/* task_struct::on_rq states: */
#define TASK_ON_RQ_QUEUED	1
#define TASK_ON_RQ_MIGRATING	2

வெளி __read_mostly முழு scheduler_running;

வெளி unsigned long calc_load_update;
வெளி atomic_long_t calc_load_tasks;

வெளி வெறுமை calc_global_load_tick(கட்டமைப்பு rq *this_rq);
வெளி long calc_load_fold_active(கட்டமைப்பு rq *this_rq, long adjust);

வெளி வெறுமை call_trace_sched_update_nr_running(கட்டமைப்பு rq *rq, முழு மொத்தம்);

வெளி முழு sysctl_sched_rt_period;
வெளி முழு sysctl_sched_rt_runtime;
வெளி முழு sched_rr_timeslice;

/*
 * Asymmetric CPU capacity bits
 */
கட்டமைப்பு asym_cap_data {
	கட்டமைப்பு list_head link;
	கட்டமைப்பு rcu_head rcu;
	unsigned long capacity;
	unsigned long cpus[];
};

வெளி கட்டமைப்பு list_head asym_cap_list;

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
	எனில் (__w)						\
		__w = max(2UL, __w >> SCHED_FIXEDPOINT_SHIFT);	\
	__w;							\
})
#இல்லையெனில்
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

நிலையான உள்வரி முழு idle_policy(முழு policy)
{
	திரும்பு policy == SCHED_IDLE;
}

நிலையான உள்வரி முழு normal_policy(முழு policy)
{
#ifdef CONFIG_SCHED_CLASS_EXT
	எனில் (policy == SCHED_EXT)
		திரும்பு true;
#endif
	திரும்பு policy == SCHED_NORMAL;
}

நிலையான உள்வரி முழு fair_policy(முழு policy)
{
	திரும்பு normal_policy(policy) || policy == SCHED_BATCH;
}

நிலையான உள்வரி முழு rt_policy(முழு policy)
{
	திரும்பு policy == SCHED_FIFO || policy == SCHED_RR;
}

நிலையான உள்வரி முழு dl_policy(முழு policy)
{
	திரும்பு policy == SCHED_DEADLINE;
}

நிலையான உள்வரி bool valid_policy(முழு policy)
{
	திரும்பு idle_policy(policy) || fair_policy(policy) ||
		rt_policy(policy) || dl_policy(policy);
}

நிலையான உள்வரி முழு task_has_idle_policy(கட்டமைப்பு task_struct *p)
{
	திரும்பு idle_policy(p->policy);
}

நிலையான உள்வரி முழு task_has_rt_policy(கட்டமைப்பு task_struct *p)
{
	திரும்பு rt_policy(p->policy);
}

நிலையான உள்வரி முழு task_has_dl_policy(கட்டமைப்பு task_struct *p)
{
	திரும்பு dl_policy(p->policy);
}

#define cap_scale(v, s)		((v)*(s) >> SCHED_CAPACITY_SHIFT)

நிலையான உள்வரி வெறுமை update_avg(u64 *avg, u64 sample)
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
நிலையான உள்வரி unsigned long sched_weight_from_cgroup(unsigned long cgrp_weight)
{
	திரும்பு DIV_ROUND_CLOSEST_ULL(cgrp_weight * 1024, CGROUP_WEIGHT_DFL);
}

நிலையான உள்வரி unsigned long sched_weight_to_cgroup(unsigned long weight)
{
	திரும்பு clamp_t(unsigned long,
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

நிலையான உள்வரி bool dl_entity_is_special(மாறிலி கட்டமைப்பு sched_dl_entity *dl_se)
{
#ifdef CONFIG_CPU_FREQ_GOV_SCHEDUTIL
	திரும்பு unlikely(dl_se->flags & SCHED_FLAG_SUGOV);
#இல்லையெனில்
	திரும்பு false;
#endif
}

/*
 * Tells if entity @a should preempt entity @b.
 */
நிலையான உள்வரி bool dl_entity_preempt(மாறிலி கட்டமைப்பு sched_dl_entity *a,
				     மாறிலி கட்டமைப்பு sched_dl_entity *b)
{
	திரும்பு dl_entity_is_special(a) ||
	       dl_time_before(a->deadline, b->deadline);
}

/*
 * This is the priority-queue data structure of the RT scheduling class:
 */
கட்டமைப்பு rt_prio_array {
	DECLARE_BITMAP(bitmap, MAX_RT_PRIO+1); /* include 1 bit for delimiter */
	கட்டமைப்பு list_head queue[MAX_RT_PRIO];
};

கட்டமைப்பு rt_bandwidth {
	/* nests inside the rq lock: */
	raw_spinlock_t		rt_runtime_lock;
	ktime_t			rt_period;
	u64			rt_runtime;
	கட்டமைப்பு hrtimer		rt_period_timer;
	unsigned முழு		rt_period_active;
};

நிலையான உள்வரி முழு dl_bandwidth_enabled(வெறுமை)
{
	திரும்பு sysctl_sched_rt_runtime >= 0;
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
கட்டமைப்பு dl_bw {
	raw_spinlock_t		lock;
	u64			bw;
	u64			total_bw;
};

வெளி வெறுமை init_dl_bw(கட்டமைப்பு dl_bw *dl_b);
வெளி முழு  sched_dl_global_validate(வெறுமை);
வெளி வெறுமை sched_dl_do_global(வெறுமை);
வெளி முழு  sched_dl_overflow(கட்டமைப்பு task_struct *p, முழு policy, மாறிலி கட்டமைப்பு sched_attr *attr);
வெளி வெறுமை __setparam_dl(கட்டமைப்பு task_struct *p, மாறிலி கட்டமைப்பு sched_attr *attr);
வெளி வெறுமை __getparam_dl(கட்டமைப்பு task_struct *p, கட்டமைப்பு sched_attr *attr, unsigned முழு flags);
வெளி bool __checkparam_dl(மாறிலி கட்டமைப்பு sched_attr *attr);
வெளி bool dl_param_changed(கட்டமைப்பு task_struct *p, மாறிலி கட்டமைப்பு sched_attr *attr);
வெளி முழு  dl_cpuset_cpumask_can_shrink(மாறிலி கட்டமைப்பு cpumask *cur, மாறிலி கட்டமைப்பு cpumask *trial);
வெளி முழு  dl_bw_deactivate(முழு cpu);
வெளி s64 dl_scaled_delta_exec(கட்டமைப்பு rq *rq, கட்டமைப்பு sched_dl_entity *dl_se, s64 delta_exec);
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
வெளி வெறுமை dl_server_update_idle(கட்டமைப்பு sched_dl_entity *dl_se, s64 delta_exec);
வெளி வெறுமை dl_server_update(கட்டமைப்பு sched_dl_entity *dl_se, s64 delta_exec);
வெளி வெறுமை dl_server_start(கட்டமைப்பு sched_dl_entity *dl_se);
வெளி வெறுமை dl_server_stop(கட்டமைப்பு sched_dl_entity *dl_se);
வெளி வெறுமை dl_server_init(கட்டமைப்பு sched_dl_entity *dl_se, கட்டமைப்பு rq *rq,
		    dl_server_pick_f pick_task);
வெளி வெறுமை sched_init_dl_servers(வெறுமை);

வெளி வெறுமை fair_server_init(கட்டமைப்பு rq *rq);
வெளி வெறுமை ext_server_init(கட்டமைப்பு rq *rq);
வெளி வெறுமை __dl_server_attach_root(கட்டமைப்பு sched_dl_entity *dl_se, கட்டமைப்பு rq *rq);
வெளி முழு dl_server_apply_params(கட்டமைப்பு sched_dl_entity *dl_se,
		    u64 runtime, u64 period, bool init);
வெளி முழு dl_server_attach_bw(கட்டமைப்பு sched_dl_entity *dl_se);
வெளி வெறுமை dl_server_detach_bw(கட்டமைப்பு sched_dl_entity *dl_se);
வெளி முழு dl_server_swap_bw(கட்டமைப்பு sched_dl_entity *detach_se,
			     கட்டமைப்பு sched_dl_entity *attach_se);

நிலையான உள்வரி bool dl_server_active(கட்டமைப்பு sched_dl_entity *dl_se)
{
	திரும்பு dl_se->dl_server_active;
}

#ifdef CONFIG_CGROUP_SCHED

வெளி கட்டமைப்பு list_head task_groups;

#ifdef CONFIG_GROUP_SCHED_BANDWIDTH
வெளி மாறிலி u64 max_bw_quota_period_us;

/*
 * default period for group bandwidth.
 * default: 0.1s, units: microseconds
 */
நிலையான உள்வரி u64 default_bw_period_us(வெறுமை)
{
	திரும்பு 100000ULL;
}
#endif /* CONFIG_GROUP_SCHED_BANDWIDTH */

கட்டமைப்பு cfs_bandwidth {
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
	கட்டமைப்பு hrtimer		period_timer;
	கட்டமைப்பு hrtimer		slack_timer;
	கட்டமைப்பு list_head	throttled_cfs_rq;

	/* Statistics: */
	முழு			nr_periods;
	முழு			nr_throttled;
	முழு			nr_burst;
	u64			throttled_time;
	u64			burst_time;
#endif /* CONFIG_CFS_BANDWIDTH */
};

/* Task group related information */
கட்டமைப்பு task_group {
	கட்டமைப்பு cgroup_subsys_state css;

#ifdef CONFIG_GROUP_SCHED_WEIGHT
	/* A positive value indicates that this is a SCHED_IDLE group. */
	முழு			idle;
#endif

#ifdef CONFIG_FAIR_GROUP_SCHED
	/* runqueue "owned" by this group on each CPU */
	கட்டமைப்பு cfs_rq __percpu	*cfs_rq;
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
	கட்டமைப்பு sched_rt_entity	**rt_se;
	கட்டமைப்பு rt_rq		**rt_rq;

	கட்டமைப்பு rt_bandwidth	rt_bandwidth;
#endif

	கட்டமைப்பு scx_task_group	scx;

	கட்டமைப்பு rcu_head		rcu;
	கட்டமைப்பு list_head	list;

	கட்டமைப்பு task_group	*parent;
	கட்டமைப்பு list_head	siblings;
	கட்டமைப்பு list_head	children;

#ifdef CONFIG_SCHED_AUTOGROUP
	கட்டமைப்பு autogroup	*autogroup;
#endif

	கட்டமைப்பு cfs_bandwidth	cfs_bandwidth;

#ifdef CONFIG_UCLAMP_TASK_GROUP
	/* The two decimal precision [%] value requested from user-space */
	unsigned முழு		uclamp_pct[UCLAMP_CNT];
	/* Clamp values requested for a task group */
	கட்டமைப்பு uclamp_se	uclamp_req[UCLAMP_CNT];
	/* Effective clamp values used for a task group */
	கட்டமைப்பு uclamp_se	uclamp[UCLAMP_CNT];
#endif

};

#define வேர்_TASK_GROUP_LOAD	NICE_0_LOAD

வகைவரை முழு (*tg_visitor)(கட்டமைப்பு task_group *, வெறுமை *);

வெளி முழு walk_tg_மரம்_from(கட்டமைப்பு task_group *from,
			     tg_visitor down, tg_visitor up, வெறுமை *data);

/*
 * Iterate the full tree, calling @down when first entering a node and @up when
 * leaving it for the final time.
 *
 * Caller must hold rcu_lock or sufficient equivalent.
 */
நிலையான உள்வரி முழு walk_tg_மரம்(tg_visitor down, tg_visitor up, வெறுமை *data)
{
	திரும்பு walk_tg_மரம்_from(&வேர்_task_group, down, up, data);
}

நிலையான உள்வரி கட்டமைப்பு task_group *css_tg(கட்டமைப்பு cgroup_subsys_state *css)
{
	திரும்பு css ? container_of(css, கட்டமைப்பு task_group, css) : வெற்று;
}

வெளி முழு tg_nop(கட்டமைப்பு task_group *tg, வெறுமை *data);

#ifdef CONFIG_FAIR_GROUP_SCHED
வெளி வெறுமை free_fair_sched_group(கட்டமைப்பு task_group *tg);
வெளி முழு alloc_fair_sched_group(கட்டமைப்பு task_group *tg, கட்டமைப்பு task_group *parent);
வெளி வெறுமை online_fair_sched_group(கட்டமைப்பு task_group *tg);
வெளி வெறுமை unregister_fair_sched_group(கட்டமைப்பு task_group *tg);
வெளி வெறுமை __sched_cgroup_mode_update(முழு mode);
#இல்லையெனில் /* !CONFIG_FAIR_GROUP_SCHED: */
நிலையான உள்வரி வெறுமை free_fair_sched_group(கட்டமைப்பு task_group *tg) { }
நிலையான உள்வரி முழு alloc_fair_sched_group(கட்டமைப்பு task_group *tg, கட்டமைப்பு task_group *parent)
{
       திரும்பு 1;
}
நிலையான உள்வரி வெறுமை online_fair_sched_group(கட்டமைப்பு task_group *tg) { }
நிலையான உள்வரி வெறுமை unregister_fair_sched_group(கட்டமைப்பு task_group *tg) { }
#endif /* !CONFIG_FAIR_GROUP_SCHED */

வெளி வெறுமை init_tg_cfs_entry(கட்டமைப்பு task_group *tg, கட்டமைப்பு cfs_rq *cfs_rq,
			கட்டமைப்பு sched_entity *se, முழு cpu,
			கட்டமைப்பு sched_entity *parent);
வெளி வெறுமை init_cfs_bandwidth(கட்டமைப்பு cfs_bandwidth *cfs_b, கட்டமைப்பு cfs_bandwidth *parent);

வெளி வெறுமை __refill_cfs_bandwidth_runtime(கட்டமைப்பு cfs_bandwidth *cfs_b);
வெளி வெறுமை start_cfs_bandwidth(கட்டமைப்பு cfs_bandwidth *cfs_b);
வெளி வெறுமை unthrottle_cfs_rq(கட்டமைப்பு cfs_rq *cfs_rq);
வெளி bool cfs_task_bw_constrained(கட்டமைப்பு task_struct *p);

வெளி வெறுமை init_tg_rt_entry(கட்டமைப்பு task_group *tg, கட்டமைப்பு rt_rq *rt_rq,
		கட்டமைப்பு sched_rt_entity *rt_se, முழு cpu,
		கட்டமைப்பு sched_rt_entity *parent);
வெளி முழு sched_group_set_rt_runtime(கட்டமைப்பு task_group *tg, long rt_runtime_us);
வெளி முழு sched_group_set_rt_period(கட்டமைப்பு task_group *tg, u64 rt_period_us);
வெளி long sched_group_rt_runtime(கட்டமைப்பு task_group *tg);
வெளி long sched_group_rt_period(கட்டமைப்பு task_group *tg);
வெளி முழு sched_rt_can_attach(கட்டமைப்பு task_group *tg, கட்டமைப்பு task_struct *tsk);

வெளி கட்டமைப்பு task_group *sched_create_group(கட்டமைப்பு task_group *parent);
வெளி வெறுமை sched_online_group(கட்டமைப்பு task_group *tg,
			       கட்டமைப்பு task_group *parent);
வெளி வெறுமை sched_destroy_group(கட்டமைப்பு task_group *tg);
வெளி வெறுமை sched_release_group(கட்டமைப்பு task_group *tg);

வெளி வெறுமை sched_move_task(கட்டமைப்பு task_struct *tsk, bool for_autogroup);

#ifdef CONFIG_FAIR_GROUP_SCHED
வெளி முழு sched_group_set_shares(கட்டமைப்பு task_group *tg, unsigned long shares);

வெளி முழு sched_group_set_idle(கட்டமைப்பு task_group *tg, long idle);

வெளி வெறுமை set_task_rq_fair(கட்டமைப்பு sched_entity *se,
			     கட்டமைப்பு cfs_rq *prev, கட்டமைப்பு cfs_rq *next);
#இல்லையெனில் /* !CONFIG_FAIR_GROUP_SCHED: */
நிலையான உள்வரி முழு sched_group_set_shares(கட்டமைப்பு task_group *tg, unsigned long shares) { திரும்பு 0; }
நிலையான உள்வரி முழு sched_group_set_idle(கட்டமைப்பு task_group *tg, long idle) { திரும்பு 0; }
#endif /* !CONFIG_FAIR_GROUP_SCHED */

#இல்லையெனில் /* !CONFIG_CGROUP_SCHED: */

கட்டமைப்பு cfs_bandwidth { };

நிலையான உள்வரி bool cfs_task_bw_constrained(கட்டமைப்பு task_struct *p) { திரும்பு false; }

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

வெளி வெறுமை unregister_rt_sched_group(கட்டமைப்பு task_group *tg);
வெளி வெறுமை free_rt_sched_group(கட்டமைப்பு task_group *tg);
வெளி முழு alloc_rt_sched_group(கட்டமைப்பு task_group *tg, கட்டமைப்பு task_group *parent);

/*
 * u64_u32_load/u64_u32_store
 *
 * Use a copy of a u64 value to protect against data race. This is only
 * applicable for 32-bits architectures.
 */
#ifdef CONFIG_64BIT
# define u64_u32_load_copy(var, copy)		var
# define u64_u32_store_copy(var, copy, val)	(var = val)
#இல்லையெனில்
# define u64_u32_load_copy(var, copy)					\
({									\
	u64 __val, __val_copy;						\
	செய் {								\
		__val_copy = copy;					\
		/*							\
		 * paired with u64_u32_store_copy(), ordering access	\
		 * to var and copy.					\
		 */							\
		smp_rmb();						\
		__val = var;						\
	} போது (__val != __val_copy);					\
	__val;								\
})
# define u64_u32_store_copy(var, copy, val)				\
செய் {									\
	typeof(val) __val = (val);					\
	var = __val;							\
	/*								\
	 * paired with u64_u32_load_copy(), ordering access to var and	\
	 * copy.							\
	 */								\
	smp_wmb();							\
	copy = __val;							\
} போது (0)
#endif
# define u64_u32_load(var)		u64_u32_load_copy(var, var##_copy)
# define u64_u32_store(var, val)	u64_u32_store_copy(var, var##_copy, val)

கட்டமைப்பு balance_callback {
	கட்டமைப்பு balance_callback *next;
	வெறுமை (*func)(கட்டமைப்பு rq *rq);
};

/* Fair scheduling SCHED_{NORMAL,BATCH,IDLE} related fields in a runqueue: */
கட்டமைப்பு cfs_rq {
	கட்டமைப்பு load_weight	load;
	unsigned முழு		nr_queued;
	unsigned முழு		h_nr_queued;		/* SCHED_{NORMAL,BATCH,IDLE} */
	unsigned முழு		h_nr_runnable;		/* SCHED_{NORMAL,BATCH,IDLE} */
	unsigned முழு		h_nr_idle;		/* SCHED_IDLE */

	s64			sum_w_vruntime;
	u64			sum_weight;
	u64			zero_vruntime;
	unsigned முழு		sum_shift;

#ifdef CONFIG_SCHED_CORE
	unsigned முழு		forceidle_seq;
	u64			zero_vruntime_fi;
#endif

	கட்டமைப்பு rb_வேர்_cached	tasks_timeline;

	/*
	 * 'curr' points to the currently running entity on this cfs_rq.
	 * It is set to NULL otherwise (i.e when none are currently running).
	 */
	கட்டமைப்பு sched_entity	*curr;
	கட்டமைப்பு sched_entity	*next;

	/*
	 * CFS load tracking
	 */
	கட்டமைப்பு sched_entity	*h_curr;
	கட்டமைப்பு sched_avg	avg;
#ifndef CONFIG_64BIT
	u64			last_update_time_copy;
#endif
	கட்டமைப்பு {
		raw_spinlock_t	lock ____cacheline_aligned;
		முழு		nr;
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
	கட்டமைப்பு sched_entity	*h_load_next;

	கட்டமைப்பு rq		*rq;	/* CPU runqueue to which this cfs_rq is attached */

	/*
	 * leaf cfs_rqs are those that hold tasks (lowest schedulable entity in
	 * a hierarchy). Non-leaf lrqs hold other higher schedulable entities
	 * (like users, containers etc.)
	 *
	 * leaf_cfs_rq_list ties together list of leaf cfs_rq's in a CPU.
	 * This list is used during load balance.
	 */
	முழு			on_list;
	கட்டமைப்பு list_head	leaf_cfs_rq_list;
	கட்டமைப்பு task_group	*tg;	/* Group that "owns" this runqueue */

	/* Locally cached copy of our task_group's idle value */
	முழு			idle;

# ifdef CONFIG_CFS_BANDWIDTH
	முழு			runtime_enabled;
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
	முழு			throttle_மொத்தம்;
	கட்டமைப்பு list_head	throttled_list;
	கட்டமைப்பு list_head	throttled_csd_list;
	கட்டமைப்பு list_head        throttled_limbo_list;
# endif /* CONFIG_CFS_BANDWIDTH */
#endif /* CONFIG_FAIR_GROUP_SCHED */
};

#ifdef CONFIG_SCHED_CLASS_EXT
/* scx_rq->flags, protected by the rq lock */
எண்ணிக்கை scx_rq_flags {
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
	SCX_RQ_வேர்_IDLE_RENOTIFY	= 1 << 8, /* the root is owed update_idle() */

	SCX_RQ_IN_WAKEUP	= 1 << 16,
	SCX_RQ_IN_DISPATCH	= 1 << 17,
};

/* per-rq rescue execution state, see scx_rescue_timerfn() */
கட்டமைப்பு scx_rq_rescue {
	கட்டமைப்பு scx_dispatch_q	dsq;			/* stranded tasks awaiting rescue */
	s64			budget;			/* execution token bucket, ns */
	u64			clock;			/* last budget accrual timestamp */
	கட்டமைப்பு task_struct	*curr;			/* task being rescued, one at a time */
	s64			slice;			/* curr's admitted slice */
	u64			exec_snap;		/* sum_exec_runtime at admission */
	கட்டமைப்பு timer_list	timer;			/* paces admission and escalation */
	u64			kill_at;		/* last ejection, init before any */
};

கட்டமைப்பு scx_rq {
	கட்டமைப்பு scx_dispatch_q	local_dsq;
#ifdef CONFIG_EXT_SUB_SCHED
	கட்டமைப்பு scx_dispatch_q	reject_dsq;		/* staging for cap-rejected tasks */
	கட்டமைப்பு scx_rq_rescue	rescue;
#endif
	கட்டமைப்பு list_head	runnable_list;		/* runnable tasks on this rq */
	கட்டமைப்பு list_head	ddsp_deferred_locals;	/* deferred ddsps from enq */
	/* both stashed across the activate_task() in move_remote_task_to_local_dsq() */
	u64			remote_activate_enq_flags;
	கட்டமைப்பு scx_sched	*remote_activate_sch;
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
	கட்டமைப்பு llist_head	ecaps_to_sync;		/* pending ecaps syncs */
	கட்டமைப்பு task_struct	*sub_dispatch_prev;
#endif
	cpumask_var_t		cpus_to_sync;
	bool			kick_sync_pending;
	unsigned long		kick_sync;

	கட்டமைப்பு list_head	sched_pcpus_to_kick;	/* see kick_cpus_irq_workfn() */

	raw_spinlock_t		deferred_reenq_lock;
	கட்டமைப்பு list_head	deferred_reenq_locals;	/* scheds requesting reenq of local DSQ */
	கட்டமைப்பு list_head	deferred_reenq_users;	/* user DSQs requesting reenq */
	கட்டமைப்பு balance_callback	deferred_bal_cb;
	கட்டமைப்பு balance_callback	kick_sync_bal_cb;
	கட்டமைப்பு irq_work		deferred_irq_work;
	கட்டமைப்பு irq_work		kick_cpus_irq_work;
};
#endif /* CONFIG_SCHED_CLASS_EXT */

நிலையான உள்வரி முழு rt_bandwidth_enabled(வெறுமை)
{
	திரும்பு sysctl_sched_rt_runtime >= 0;
}

/* RT IPI pull logic requires IRQ_WORK */
#எனில் defined(CONFIG_IRQ_WORK) && defined(CONFIG_SMP)
# define HAVE_RT_தள்ளு_IPI
#endif

/* Real-Time classes' related field in a runqueue: */
கட்டமைப்பு rt_rq {
	கட்டமைப்பு rt_prio_array	active;
	unsigned முழு		rt_nr_running;
	unsigned முழு		rr_nr_running;
	கட்டமைப்பு {
		முழு		curr; /* highest queued rt task prio */
		முழு		next; /* next highest */
	} highest_prio;
	bool			overloaded;
	கட்டமைப்பு plist_head	pushable_tasks;

	முழு			rt_queued;

#ifdef CONFIG_RT_GROUP_SCHED
	முழு			rt_throttled;
	u64			rt_time; /* consumed RT time, goes up in update_curr_rt */
	u64			rt_runtime; /* allotted RT time, "slice" from rt_bandwidth, RT sharing/balancing */
	/* Nests inside the rq lock: */
	raw_spinlock_t		rt_runtime_lock;

	unsigned முழு		rt_nr_boosted;

	கட்டமைப்பு rq		*rq; /* this is always top-level rq, cache? */
#endif
#ifdef CONFIG_CGROUP_SCHED
	கட்டமைப்பு task_group	*tg; /* this tg has "this" rt_rq on given CPU for runnable entities */
#endif
};

நிலையான உள்வரி bool rt_rq_is_runnable(கட்டமைப்பு rt_rq *rt_rq)
{
	திரும்பு rt_rq->rt_queued && rt_rq->rt_nr_running;
}

/* Deadline class' related fields in a runqueue */
கட்டமைப்பு dl_rq {
	/* runqueue is an rbtree, ordered by deadline */
	கட்டமைப்பு rb_வேர்_cached	வேர்;

	unsigned முழு		dl_nr_running;

	/*
	 * Deadline values of the currently executing and the
	 * earliest ready task on this rq. Caching these facilitates
	 * the decision whether or not a ready but not running task
	 * should migrate somewhere else.
	 */
	கட்டமைப்பு {
		u64		curr;
		u64		next;
	} earliest_dl;

	bool			overloaded;

	கட்டமைப்பு sched_dl_entity	*curr;
	/*
	 * Tasks on this rq that can be pushed away. They are kept in
	 * an rb-tree, ordered by tasks' deadlines, with caching
	 * of the leftmost (earliest deadline) element.
	 */
	கட்டமைப்பு rb_வேர்_cached	pushable_dl_tasks_வேர்;

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
#define is_வேர்_task_group(tg) ((tg) == &வேர்_task_group)
/* An entity is a task if it doesn't "own" a runqueue */
#define entity_is_task(se)	(!se->my_q)

நிலையான உள்வரி வெறுமை se_update_runnable(கட்டமைப்பு sched_entity *se)
{
	எனில் (!entity_is_task(se))
		se->runnable_weight = se->my_q->h_nr_runnable;
}

நிலையான உள்வரி long se_runnable(கட்டமைப்பு sched_entity *se)
{
	எனில் (se->sched_delayed)
		திரும்பு false;

	எனில் (entity_is_task(se))
		திரும்பு !!se->on_rq;
	இல்லையெனில்
		திரும்பு se->runnable_weight;
}

#இல்லையெனில் /* !CONFIG_FAIR_GROUP_SCHED: */

#define entity_is_task(se)	1

நிலையான உள்வரி வெறுமை se_update_runnable(கட்டமைப்பு sched_entity *se) { }

நிலையான உள்வரி long se_runnable(கட்டமைப்பு sched_entity *se)
{
	எனில் (se->sched_delayed)
		திரும்பு false;

	திரும்பு !!se->on_rq;
}

#endif /* !CONFIG_FAIR_GROUP_SCHED */

/*
 * XXX we want to get rid of these helpers and use the full load resolution.
 */
நிலையான உள்வரி long se_weight(கட்டமைப்பு sched_entity *se)
{
	திரும்பு scale_load_down(se->load.weight);
}


நிலையான உள்வரி bool sched_asym_prefer(முழு a, முழு b)
{
	திரும்பு arch_asym_cpu_priority(a) > arch_asym_cpu_priority(b);
}

கட்டமைப்பு perf_domain {
	கட்டமைப்பு em_perf_domain *em_pd;
	கட்டமைப்பு perf_domain *next;
	கட்டமைப்பு rcu_head rcu;
};

/*
 * We add the notion of a root-domain which will be used to define per-domain
 * variables. Each exclusive cpuset essentially defines an island domain by
 * fully partitioning the member CPUs from any other cpuset. Whenever a new
 * exclusive cpuset is created, we also create and attach a new root-domain
 * object.
 *
 */
கட்டமைப்பு வேர்_domain {
	atomic_t		refcount;
	atomic_t		rto_மொத்தம்;
	கட்டமைப்பு rcu_head		rcu;
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
	atomic_t		dlo_மொத்தம்;
	கட்டமைப்பு dl_bw		dl_bw;
	கட்டமைப்பு cpudl		cpudl;

	/*
	 * Indicate whether a root_domain's dl_bw has been checked or
	 * updated. It's monotonously increasing value.
	 *
	 * Also, some corner cases, like 'wrap around' is dangerous, but given
	 * that u64 is 'big enough'. So that shouldn't be a concern.
	 */
	u64 visit_cookie;

#ifdef HAVE_RT_தள்ளு_IPI
	/*
	 * For IPI pull requests, loop across the rto_mask.
	 */
	கட்டமைப்பு irq_work		rto_தள்ளு_work;
	raw_spinlock_t		rto_lock;
	/* These are only updated and read within rto_lock */
	முழு			rto_loop;
	முழு			rto_cpu;
	/* These atomics are updated outside of a lock */
	atomic_t		rto_loop_next;
	atomic_t		rto_loop_start;
#endif /* HAVE_RT_PUSH_IPI */
	/*
	 * The "RT overload" flag: it gets set if a CPU has more than
	 * one runnable RT task.
	 */
	cpumask_var_t		rto_mask;
	கட்டமைப்பு cpupri		cpupri;

	/*
	 * NULL-terminated list of performance domains intersecting with the
	 * CPUs of the rd. Protected by RCU.
	 */
	கட்டமைப்பு perf_domain __rcu *pd;
};

வெளி வெறுமை init_defrootdomain(வெறுமை);
வெளி முழு sched_init_domains(மாறிலி கட்டமைப்பு cpumask *cpu_map);
வெளி வெறுமை rq_attach_வேர்(கட்டமைப்பு rq *rq, கட்டமைப்பு வேர்_domain *rd);
வெளி வெறுமை sched_get_rd(கட்டமைப்பு வேர்_domain *rd);
வெளி வெறுமை sched_put_rd(கட்டமைப்பு வேர்_domain *rd);

நிலையான உள்வரி முழு get_rd_overloaded(கட்டமைப்பு வேர்_domain *rd)
{
	திரும்பு READ_ONCE(rd->overloaded);
}

நிலையான உள்வரி வெறுமை set_rd_overloaded(கட்டமைப்பு வேர்_domain *rd, முழு status)
{
	எனில் (get_rd_overloaded(rd) != status)
		WRITE_ONCE(rd->overloaded, status);
}

#ifdef HAVE_RT_தள்ளு_IPI
வெளி வெறுமை rto_தள்ளு_irq_work_func(கட்டமைப்பு irq_work *work);
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
கட்டமைப்பு uclamp_bucket {
	unsigned long மதிப்பு : bits_per(SCHED_CAPACITY_SCALE);
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
கட்டமைப்பு uclamp_rq {
	unsigned முழு மதிப்பு;
	கட்டமைப்பு uclamp_bucket bucket[UCLAMP_BUCKETS];
};

DECLARE_STATIC_சாவி_FALSE(sched_uclamp_used);
#endif /* CONFIG_UCLAMP_TASK */

/*
 * This is the main, per-CPU runqueue data structure.
 *
 * Locking rule: those places that want to lock multiple runqueues
 * (such as the load balancing or the thread migration code), lock
 * acquire operations must be ordered by ascending &runqueue.
 */
கட்டமைப்பு rq {
	/*
	 * The following members are loaded together, without holding the
	 * rq->lock, in an extremely hot loop in update_sg_lb_stats()
	 * (called from pick_next_task()). To reduce cache pollution from
	 * this operation, they are placed together on this dedicated cache
	 * line. Even though some of them are frequently modified, they are
	 * loaded much more frequently than they are stored.
	 */
	unsigned முழு		nr_running;
#ifdef CONFIG_NUMA_BALANCING
	unsigned முழு		nr_numa_running;
	unsigned முழு		nr_preferred_running;
#endif
	unsigned முழு		ttwu_pending;
	unsigned long		cpu_capacity;
#ifdef CONFIG_SCHED_PROXY_EXEC
	கட்டமைப்பு task_struct __rcu	*donor;  /* Scheduling context */
	கட்டமைப்பு task_struct __rcu	*curr;   /* Execution context */
#இல்லையெனில்
	ஒன்றிப்பு {
		கட்டமைப்பு task_struct __rcu *donor; /* Scheduler context */
		கட்டமைப்பு task_struct __rcu *curr;  /* Execution context */
	};
#endif
	கட்டமைப்பு task_struct	*idle;
	/* padding left here deliberately */

	/*
	 * The next cacheline holds the (hot) runqueue lock, as well as
	 * some other less performance-critical fields.
	 */
	u64			nr_switches	____cacheline_aligned;

	/* runqueue lock: */
	raw_spinlock_t		__lock;

#ifdef CONFIG_NO_HZ_COMMON
	unsigned முழு		nohz_tick_stopped;
	atomic_t		nohz_flags;
	unsigned முழு		has_blocked_load;
	unsigned long		last_blocked_load_update_tick;
	call_single_data_t	nohz_csd;
#endif /* CONFIG_NO_HZ_COMMON */

#ifdef CONFIG_UCLAMP_TASK
	/* Utilization clamp values based on CPU's RUNNABLE tasks */
	கட்டமைப்பு uclamp_rq	uclamp[UCLAMP_CNT] ____cacheline_aligned;
	unsigned முழு		uclamp_flags;
#define UCLAMP_FLAG_IDLE 0x01
#endif

	கட்டமைப்பு cfs_rq		cfs;
	கட்டமைப்பு rt_rq		rt;
	கட்டமைப்பு dl_rq		dl;
#ifdef CONFIG_SCHED_CLASS_EXT
	கட்டமைப்பு scx_rq		scx;
	கட்டமைப்பு sched_dl_entity	ext_server;
#endif
#ifdef CONFIG_SCHED_CACHE
	raw_spinlock_t		cpu_epoch_lock ____cacheline_aligned;
	u64			cpu_runtime;
	unsigned long		cpu_epoch;
	unsigned long		cpu_epoch_next;
#endif

	கட்டமைப்பு sched_dl_entity	fair_server;

#ifdef CONFIG_FAIR_GROUP_SCHED
	/* list of leaf cfs_rq on this CPU: */
	கட்டமைப்பு list_head	leaf_cfs_rq_list;
	கட்டமைப்பு list_head	*tmp_alone_branch;
#endif /* CONFIG_FAIR_GROUP_SCHED */

#ifdef CONFIG_NUMA_BALANCING
	unsigned முழு		numa_migrate_on;
#endif

#ifdef CONFIG_SCHED_CACHE
	unsigned முழு		nr_pref_llc_running;
	unsigned முழு		nr_llc_running;
#endif

	/*
	 * This is part of a global counter where only the total sum
	 * over all CPUs matters. A task can increase this counter on
	 * one CPU and if it got migrated afterwards it may decrease
	 * it on another CPU. Always updated under the runqueue lock:
	 */
	unsigned long		nr_uninterruptible;

	கட்டமைப்பு sched_dl_entity	*dl_server;
	கட்டமைப்பு task_struct	*stop;
	மாறிலி கட்டமைப்பு sched_class *next_class;
	unsigned long		next_balance;
	கட்டமைப்பு mm_struct	*prev_mm;

	/*
	 * The following fields of clock data are frequently referenced
	 * and updated together, and should go on their own cache line.
	 */
	u64			clock_task ____cacheline_aligned;
	u64			clock_pelt;
	u64			clock;
	unsigned long		lost_idle_time;
	unsigned முழு		clock_update_flags;
	u64			clock_pelt_idle;
	u64			clock_idle;

#ifndef CONFIG_64BIT
	u64			clock_pelt_idle_copy;
	u64			clock_idle_copy;
#endif

	u64 last_seen_need_resched_ns;
	முழு ticks_without_resched;

#ifdef CONFIG_MEMBARRIER
	முழு membarrier_state;
#endif

	கட்டமைப்பு வேர்_domain		*rd;
	கட்டமைப்பு sched_domain __rcu	*sd;

	கட்டமைப்பு balance_callback *balance_callback;

	unsigned எழுத்து		nohz_idle_balance;
	unsigned எழுத்து		idle_balance;

	unsigned long		misfit_task_load;

	/* For active balancing */
	முழு			active_balance;
	முழு			தள்ளு_cpu;
	கட்டமைப்பு cpu_stop_work	active_balance_work;

	/* CPU of this runqueue: */
	முழு			cpu;
	முழு			online;

	கட்டமைப்பு list_head cfs_tasks;

	கட்டமைப்பு sched_avg	avg_rt;
	கட்டமைப்பு sched_avg	avg_dl;
#ifdef CONFIG_HAVE_SCHED_AVG_IRQ
	கட்டமைப்பு sched_avg	avg_irq;
#endif
#ifdef CONFIG_SCHED_HW_PRESSURE
	கட்டமைப்பு sched_avg	avg_hw;
#endif
	u64			idle_stamp;
	u64			avg_idle;

	/* This is used to determine avg_idle's max value */
	u64			max_idle_balance_cost;

#ifdef CONFIG_HOTPLUG_CPU
	கட்டமைப்பு rcuwait		hotplug_wait;
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
	bool			npc_தள்ளு_work_pending;
#endif

	/* calc_load related fields */
	unsigned long		calc_load_update;
	long			calc_load_active;

#ifdef CONFIG_SCHED_HRTICK
	call_single_data_t	hrtick_csd;
	கட்டமைப்பு hrtimer		hrtick_timer;
	ktime_t			hrtick_time;
	ktime_t			hrtick_delay;
	unsigned முழு		hrtick_sched;
#endif

#ifdef CONFIG_SCHEDSTATS
	/* latency stats */
	கட்டமைப்பு sched_info	rq_sched_info;
	unsigned long long	rq_cpu_time;

	/* sys_sched_yield() stats */
	unsigned முழு		yld_மொத்தம்;

	/* schedule() stats */
	unsigned முழு		sched_மொத்தம்;
	unsigned முழு		sched_goidle;

	/* try_to_wake_up() stats */
	unsigned முழு		ttwu_மொத்தம்;
	unsigned முழு		ttwu_local;
#endif

#ifdef CONFIG_CPU_IDLE
	/* Must be inspected within a RCU lock section */
	கட்டமைப்பு cpuidle_state	*idle_state;
#endif

	unsigned முழு		nr_pinned;
	unsigned முழு		தள்ளு_busy;
	கட்டமைப்பு cpu_stop_work	தள்ளு_work;

#ifdef CONFIG_SCHED_CORE
	/* per rq */
	கட்டமைப்பு rq		*core;
	கட்டமைப்பு task_struct	*core_pick;
	கட்டமைப்பு sched_dl_entity	*core_dl_server;
	unsigned முழு		core_enabled;
	கட்டமைப்பு rb_வேர்		core_மரம்;

	/* shared state -- careful with sched_core_cpu_deactivate() */
	unsigned முழு		core_task_seq;
	unsigned முழு		core_pick_seq;
	unsigned long		core_cookie;
	unsigned முழு		core_forceidle_மொத்தம்;
	unsigned முழு		core_forceidle_seq;
	unsigned முழு		core_forceidle_occupation;
	u64			core_forceidle_start;
	unsigned முழு		core_pick_in_flight;
#endif /* CONFIG_SCHED_CORE */

	/* Scratch cpumask to be temporarily used under rq_lock */
	cpumask_var_t		scratch_mask;

#ifdef CONFIG_CFS_BANDWIDTH
	call_single_data_t	cfsb_csd;
	கட்டமைப்பு list_head	cfsb_csd_list;
#endif

	atomic_t		nr_iowait;
} __no_randomize_layout;

#ifdef CONFIG_FAIR_GROUP_SCHED

/* CPU runqueue to which this cfs_rq is attached */
நிலையான உள்வரி கட்டமைப்பு rq *rq_of(கட்டமைப்பு cfs_rq *cfs_rq)
{
	திரும்பு cfs_rq->rq;
}

#இல்லையெனில் /* !CONFIG_FAIR_GROUP_SCHED: */

நிலையான உள்வரி கட்டமைப்பு rq *rq_of(கட்டமைப்பு cfs_rq *cfs_rq)
{
	திரும்பு container_of(cfs_rq, கட்டமைப்பு rq, cfs);
}
#endif /* !CONFIG_FAIR_GROUP_SCHED */

நிலையான உள்வரி முழு cpu_of(கட்டமைப்பு rq *rq)
{
	திரும்பு rq->cpu;
}

#define MDF_தள்ளு		0x01

நிலையான உள்வரி bool is_migration_disabled(கட்டமைப்பு task_struct *p)
{
	திரும்பு p->migration_disabled;
}

DECLARE_PER_CPU_SHARED_ALIGNED(கட்டமைப்பு rq, runqueues);
DECLARE_PER_CPU(கட்டமைப்பு rnd_state, sched_rnd_state);

நிலையான உள்வரி u32 sched_rng(வெறுமை)
{
	திரும்பு prandom_u32_state(this_cpu_ptr(&sched_rnd_state));
}

நிலையான __always_inline கட்டமைப்பு rq *__this_rq(வெறுமை)
{
	திரும்பு this_cpu_ptr(&runqueues);
}

#define cpu_rq(cpu)		(&per_cpu(runqueues, (cpu)))
#define this_rq()		__this_rq()
#define task_rq(p)		cpu_rq(task_cpu(p))
#define cpu_curr(cpu)		(cpu_rq(cpu)->curr)
#define raw_rq()		raw_cpu_ptr(&runqueues)

நிலையான உள்வரி bool idle_rq(கட்டமைப்பு rq *rq)
{
	திரும்பு rq->curr == rq->idle && !rq->nr_running && !rq->ttwu_pending;
}

/**
 * available_idle_cpu - is a given CPU idle for enqueuing work.
 * @cpu: the CPU in question.
 *
 * Return: 1 if the CPU is currently idle. 0 otherwise.
 */
நிலையான உள்வரி bool available_idle_cpu(முழு cpu)
{
	எனில் (!idle_rq(cpu_rq(cpu)))
		திரும்பு 0;

	எனில் (vcpu_is_preempted(cpu))
		திரும்பு 0;

	திரும்பு 1;
}

#ifdef CONFIG_SCHED_PROXY_EXEC
நிலையான உள்வரி வெறுமை rq_set_donor(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *t)
{
	rcu_assign_pointer(rq->donor, t);
}
#இல்லையெனில்
நிலையான உள்வரி வெறுமை rq_set_donor(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *t)
{
	/* Do nothing */
}
#endif

#ifdef CONFIG_SCHED_CORE
நிலையான உள்வரி கட்டமைப்பு cpumask *sched_group_span(கட்டமைப்பு sched_group *sg);

DECLARE_STATIC_சாவி_FALSE(__sched_core_enabled);

நிலையான உள்வரி bool sched_core_enabled(கட்டமைப்பு rq *rq)
{
	திரும்பு static_branch_unlikely(&__sched_core_enabled) && rq->core_enabled;
}

நிலையான உள்வரி bool sched_core_disabled(வெறுமை)
{
	திரும்பு !static_branch_unlikely(&__sched_core_enabled);
}

/*
 * Be careful with this function; not for general use. The return value isn't
 * stable unless you actually hold a relevant rq->__lock.
 */
நிலையான உள்வரி raw_spinlock_t *rq_lockp(கட்டமைப்பு rq *rq)
{
	எனில் (sched_core_enabled(rq))
		திரும்பு &rq->core->__lock;

	திரும்பு &rq->__lock;
}

நிலையான உள்வரி raw_spinlock_t *__rq_lockp(கட்டமைப்பு rq *rq)
	__returns_ctx_lock(rq_lockp(rq)) /* alias them */
{
	எனில் (rq->core_enabled)
		திரும்பு &rq->core->__lock;

	திரும்பு &rq->__lock;
}

வெளி bool
cfs_prio_less(மாறிலி கட்டமைப்பு task_struct *a, மாறிலி கட்டமைப்பு task_struct *b, bool fi);

வெளி வெறுமை task_vruntime_update(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p, bool in_fi);

/*
 * Helpers to check if the CPU's core cookie matches with the task's cookie
 * when core scheduling is enabled.
 * A special case is that the task's cookie always matches with CPU's core
 * cookie if the CPU is in an idle core.
 */
நிலையான உள்வரி bool sched_cpu_cookie_match(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p)
{
	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	எனில் (!sched_core_enabled(rq))
		திரும்பு true;

	திரும்பு rq->core->core_cookie == p->core_cookie;
}

நிலையான உள்வரி bool sched_core_cookie_match(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p)
{
	bool idle_core = true;
	முழு cpu;

	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	எனில் (!sched_core_enabled(rq))
		திரும்பு true;

	எனில் (rq->core->core_cookie == p->core_cookie)
		திரும்பு true;

	for_each_cpu(cpu, cpu_smt_mask(cpu_of(rq))) {
		எனில் (!available_idle_cpu(cpu)) {
			idle_core = false;
			நிறுத்து;
		}
	}

	/*
	 * A CPU in an idle core is always the best choice for tasks with
	 * cookies.
	 */
	திரும்பு idle_core;
}

நிலையான உள்வரி bool sched_group_cookie_match(கட்டமைப்பு rq *rq,
					    கட்டமைப்பு task_struct *p,
					    கட்டமைப்பு sched_group *group)
{
	முழு cpu;

	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	எனில் (!sched_core_enabled(rq))
		திரும்பு true;

	for_each_cpu_and(cpu, sched_group_span(group), p->cpus_ptr) {
		எனில் (sched_core_cookie_match(cpu_rq(cpu), p))
			திரும்பு true;
	}
	திரும்பு false;
}

நிலையான உள்வரி bool sched_core_enqueued(கட்டமைப்பு task_struct *p)
{
	திரும்பு !RB_EMPTY_முனை(&p->core_முனை);
}

வெளி வெறுமை sched_core_enqueue(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p);
வெளி வெறுமை sched_core_dequeue(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p, முழு flags);

வெளி வெறுமை sched_core_get(வெறுமை);
வெளி வெறுமை sched_core_put(வெறுமை);

நிலையான உள்வரி bool task_has_sched_core(கட்டமைப்பு task_struct *p)
{
	எனில் (sched_core_disabled())
		திரும்பு false;

	திரும்பு !!p->core_cookie;
}

#இல்லையெனில் /* !CONFIG_SCHED_CORE: */

நிலையான உள்வரி bool sched_core_enabled(கட்டமைப்பு rq *rq)
{
	திரும்பு false;
}

நிலையான உள்வரி bool sched_core_disabled(வெறுமை)
{
	திரும்பு true;
}

நிலையான உள்வரி raw_spinlock_t *rq_lockp(கட்டமைப்பு rq *rq)
{
	திரும்பு &rq->__lock;
}

நிலையான உள்வரி raw_spinlock_t *__rq_lockp(கட்டமைப்பு rq *rq)
	__returns_ctx_lock(rq_lockp(rq)) /* alias them */
{
	திரும்பு &rq->__lock;
}

நிலையான உள்வரி bool sched_cpu_cookie_match(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p)
{
	திரும்பு true;
}

நிலையான உள்வரி bool sched_core_cookie_match(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p)
{
	திரும்பு true;
}

நிலையான உள்வரி bool sched_group_cookie_match(கட்டமைப்பு rq *rq,
					    கட்டமைப்பு task_struct *p,
					    கட்டமைப்பு sched_group *group)
{
	திரும்பு true;
}

நிலையான உள்வரி bool task_has_sched_core(கட்டமைப்பு task_struct *p)
{
	திரும்பு false;
}

#endif /* !CONFIG_SCHED_CORE */

#ifdef CONFIG_RT_GROUP_SCHED
# ifdef CONFIG_RT_GROUP_SCHED_DEFAULT_DISABLED
DECLARE_STATIC_சாவி_FALSE(rt_group_sched);
நிலையான உள்வரி bool rt_group_sched_enabled(வெறுமை)
{
	திரும்பு static_branch_unlikely(&rt_group_sched);
}
# இல்லையெனில் /* !CONFIG_RT_GROUP_SCHED_DEFAULT_DISABLED: */
DECLARE_STATIC_சாவி_TRUE(rt_group_sched);
நிலையான உள்வரி bool rt_group_sched_enabled(வெறுமை)
{
	திரும்பு static_branch_likely(&rt_group_sched);
}
# endif /* !CONFIG_RT_GROUP_SCHED_DEFAULT_DISABLED */
#இல்லையெனில் /* !CONFIG_RT_GROUP_SCHED: */
# define rt_group_sched_enabled()	false
#endif /* !CONFIG_RT_GROUP_SCHED */

நிலையான உள்வரி வெறுமை lockdep_assert_rq_held(கட்டமைப்பு rq *rq)
	__assumes_ctx_lock(__rq_lockp(rq))
{
	lockdep_assert_held(__rq_lockp(rq));
}

வெளி வெறுமை raw_spin_rq_lock_nested(கட்டமைப்பு rq *rq, முழு subclass)
	__acquires(__rq_lockp(rq));

வெளி bool raw_spin_rq_trylock(கட்டமைப்பு rq *rq)
	__cond_acquires(true, __rq_lockp(rq));

நிலையான உள்வரி வெறுமை raw_spin_rq_lock(கட்டமைப்பு rq *rq)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_lock_nested(rq, 0);
}

நிலையான உள்வரி வெறுமை raw_spin_rq_unlock(கட்டமைப்பு rq *rq)
	__releases(__rq_lockp(rq))
{
	raw_spin_unlock(rq_lockp(rq));
}

நிலையான உள்வரி வெறுமை raw_spin_rq_lock_irq(கட்டமைப்பு rq *rq)
	__acquires(__rq_lockp(rq))
{
	local_irq_disable();
	raw_spin_rq_lock(rq);
}

நிலையான உள்வரி வெறுமை raw_spin_rq_unlock_irq(கட்டமைப்பு rq *rq)
	__releases(__rq_lockp(rq))
{
	raw_spin_rq_unlock(rq);
	local_irq_enable();
}

நிலையான உள்வரி unsigned long _raw_spin_rq_lock_irqsave(கட்டமைப்பு rq *rq)
	__acquires(__rq_lockp(rq))
{
	unsigned long flags;

	local_irq_save(flags);
	raw_spin_rq_lock(rq);

	திரும்பு flags;
}

நிலையான உள்வரி வெறுமை raw_spin_rq_unlock_irqrestore(கட்டமைப்பு rq *rq, unsigned long flags)
	__releases(__rq_lockp(rq))
{
	raw_spin_rq_unlock(rq);
	local_irq_restore(flags);
}

#define raw_spin_rq_lock_irqsave(rq, flags)	\
செய் {						\
	flags = _raw_spin_rq_lock_irqsave(rq);	\
} போது (0)

வெளி வெறுமை __update_idle_core(கட்டமைப்பு rq *rq);

நிலையான உள்வரி வெறுமை update_idle_core(கட்டமைப்பு rq *rq)
{
	எனில் (sched_smt_active())
		__update_idle_core(rq);
}

#ifdef CONFIG_FAIR_GROUP_SCHED
நிலையான உள்வரி கட்டமைப்பு task_struct *task_of(கட்டமைப்பு sched_entity *se)
{
	WARN_ON_ONCE(!entity_is_task(se));
	திரும்பு container_of(se, கட்டமைப்பு task_struct, se);
}

நிலையான உள்வரி கட்டமைப்பு cfs_rq *task_cfs_rq(கட்டமைப்பு task_struct *p)
{
	திரும்பு p->se.cfs_rq;
}

/* runqueue on which this entity is (to be) queued */
நிலையான உள்வரி கட்டமைப்பு cfs_rq *cfs_rq_of(மாறிலி கட்டமைப்பு sched_entity *se)
{
	திரும்பு se->cfs_rq;
}

/* runqueue "owned" by this group */
நிலையான உள்வரி கட்டமைப்பு cfs_rq *group_cfs_rq(கட்டமைப்பு sched_entity *grp)
{
	திரும்பு grp->my_q;
}

#இல்லையெனில் /* !CONFIG_FAIR_GROUP_SCHED: */

#define task_of(_se)		container_of(_se, கட்டமைப்பு task_struct, se)

நிலையான உள்வரி கட்டமைப்பு cfs_rq *task_cfs_rq(மாறிலி கட்டமைப்பு task_struct *p)
{
	திரும்பு &task_rq(p)->cfs;
}

நிலையான உள்வரி கட்டமைப்பு cfs_rq *cfs_rq_of(மாறிலி கட்டமைப்பு sched_entity *se)
{
	மாறிலி கட்டமைப்பு task_struct *p = task_of(se);
	கட்டமைப்பு rq *rq = task_rq(p);

	திரும்பு &rq->cfs;
}

/* runqueue "owned" by this group */
நிலையான உள்வரி கட்டமைப்பு cfs_rq *group_cfs_rq(கட்டமைப்பு sched_entity *grp)
{
	திரும்பு வெற்று;
}

#endif /* !CONFIG_FAIR_GROUP_SCHED */

வெளி வெறுமை update_rq_avg_idle(கட்டமைப்பு rq *rq);
வெளி வெறுமை update_rq_clock(கட்டமைப்பு rq *rq);

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

நிலையான உள்வரி வெறுமை assert_clock_updated(கட்டமைப்பு rq *rq)
{
	/*
	 * The only reason for not seeing a clock update since the
	 * last rq_pin_lock() is if we're currently skipping updates.
	 */
	WARN_ON_ONCE(rq->clock_update_flags < RQCF_ACT_SKIP);
}

நிலையான உள்வரி u64 rq_clock(கட்டமைப்பு rq *rq)
{
	lockdep_assert_rq_held(rq);
	assert_clock_updated(rq);

	திரும்பு rq->clock;
}

நிலையான உள்வரி u64 rq_clock_task(கட்டமைப்பு rq *rq)
{
	lockdep_assert_rq_held(rq);
	assert_clock_updated(rq);

	திரும்பு rq->clock_task;
}

நிலையான உள்வரி வெறுமை rq_clock_skip_update(கட்டமைப்பு rq *rq)
{
	lockdep_assert_rq_held(rq);
	rq->clock_update_flags |= RQCF_REQ_SKIP;
}

/*
 * See rt task throttling, which is the only time a skip
 * request is canceled.
 */
நிலையான உள்வரி வெறுமை rq_clock_cancel_skipupdate(கட்டமைப்பு rq *rq)
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
நிலையான உள்வரி வெறுமை rq_clock_start_loop_update(கட்டமைப்பு rq *rq)
{
	lockdep_assert_rq_held(rq);
	WARN_ON_ONCE(rq->clock_update_flags & RQCF_ACT_SKIP);
	rq->clock_update_flags |= RQCF_ACT_SKIP;
}

நிலையான உள்வரி வெறுமை rq_clock_stop_loop_update(கட்டமைப்பு rq *rq)
{
	lockdep_assert_rq_held(rq);
	rq->clock_update_flags &= ~RQCF_ACT_SKIP;
}

கட்டமைப்பு rq_flags {
	unsigned long flags;
	கட்டமைப்பு pin_cookie cookie;
	/*
	 * A copy of (rq::clock_update_flags & RQCF_UPDATED) for the
	 * current pin context is stashed here in case it needs to be
	 * restored in rq_repin_lock().
	 */
	unsigned முழு clock_update_flags;
};

வெளி கட்டமைப்பு balance_callback balance_தள்ளு_callback;

#ifdef CONFIG_SCHED_CLASS_EXT
வெளி மாறிலி கட்டமைப்பு sched_class ext_sched_class;

DECLARE_STATIC_சாவி_FALSE(__scx_enabled);	/* SCX BPF scheduler loaded */
DECLARE_STATIC_சாவி_FALSE(__scx_switched_all);	/* all fair class tasks on SCX */

#define scx_enabled()		static_branch_unlikely(&__scx_enabled)
#define scx_switched_all()	static_branch_unlikely(&__scx_switched_all)

நிலையான உள்வரி வெறுமை scx_rq_clock_update(கட்டமைப்பு rq *rq, u64 clock)
{
	எனில் (!scx_enabled())
		திரும்பு;
	WRITE_ONCE(rq->scx.clock, clock);
	smp_store_release(&rq->scx.flags, rq->scx.flags | SCX_RQ_CLK_VALID);
}

நிலையான உள்வரி வெறுமை scx_rq_clock_invalidate(கட்டமைப்பு rq *rq)
{
	எனில் (!scx_enabled())
		திரும்பு;
	WRITE_ONCE(rq->scx.flags, rq->scx.flags & ~SCX_RQ_CLK_VALID);
}

#இல்லையெனில் /* !CONFIG_SCHED_CLASS_EXT: */
#define scx_enabled()		false
#define scx_switched_all()	false

நிலையான உள்வரி வெறுமை scx_rq_clock_update(கட்டமைப்பு rq *rq, u64 clock) {}
நிலையான உள்வரி வெறுமை scx_rq_clock_invalidate(கட்டமைப்பு rq *rq) {}
#endif /* !CONFIG_SCHED_CLASS_EXT */

நிலையான உள்வரி வெறுமை assert_balance_callbacks_empty(கட்டமைப்பு rq *rq)
{
	WARN_ON_ONCE(IS_ENABLED(CONFIG_PROVE_LOCKING) &&
		     rq->balance_callback &&
		     rq->balance_callback != &balance_தள்ளு_callback);
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
நிலையான உள்வரி வெறுமை rq_pin_lock(கட்டமைப்பு rq *rq, கட்டமைப்பு rq_flags *rf)
{
	rf->cookie = lockdep_pin_lock(__rq_lockp(rq));

	rq->clock_update_flags &= (RQCF_REQ_SKIP|RQCF_ACT_SKIP);
	rf->clock_update_flags = 0;
	assert_balance_callbacks_empty(rq);
}

நிலையான உள்வரி வெறுமை rq_unpin_lock(கட்டமைப்பு rq *rq, கட்டமைப்பு rq_flags *rf)
{
	எனில் (rq->clock_update_flags > RQCF_ACT_SKIP)
		rf->clock_update_flags = RQCF_UPDATED;

	scx_rq_clock_invalidate(rq);
	lockdep_unpin_lock(__rq_lockp(rq), rf->cookie);
}

நிலையான உள்வரி வெறுமை rq_repin_lock(கட்டமைப்பு rq *rq, கட்டமைப்பு rq_flags *rf)
{
	lockdep_repin_lock(__rq_lockp(rq), rf->cookie);

	/*
	 * Restore the value we stashed in @rf for this pin context.
	 */
	rq->clock_update_flags |= rf->clock_update_flags;
}

#define __task_rq_lock(...) __acquire_ret(___task_rq_lock(__VA_ARGS__), __rq_lockp(__ret))
வெளி கட்டமைப்பு rq *___task_rq_lock(கட்டமைப்பு task_struct *p, கட்டமைப்பு rq_flags *rf) __acquires_ret;

#define task_rq_lock(...) __acquire_ret(_task_rq_lock(__VA_ARGS__), __rq_lockp(__ret))
வெளி கட்டமைப்பு rq *_task_rq_lock(கட்டமைப்பு task_struct *p, கட்டமைப்பு rq_flags *rf)
	__acquires(&p->pi_lock) __acquires_ret;

நிலையான உள்வரி வெறுமை
__task_rq_unlock(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p, கட்டமைப்பு rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_lock(rq, rf);
	raw_spin_rq_unlock(rq);
}

நிலையான உள்வரி வெறுமை
task_rq_unlock(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p, கட்டமைப்பு rq_flags *rf)
	__releases(__rq_lockp(rq), &p->pi_lock)
{
	__task_rq_unlock(rq, p, rf);
	raw_spin_unlock_irqrestore(&p->pi_lock, rf->flags);
}

DEFINE_LOCK_GUARD_1(task_rq_lock, கட்டமைப்பு task_struct,
		    _T->rq = task_rq_lock(_T->lock, &_T->rf),
		    task_rq_unlock(_T->rq, _T->lock, &_T->rf),
		    கட்டமைப்பு rq *rq; கட்டமைப்பு rq_flags rf)
DECLARE_LOCK_GUARD_1_ATTRS(task_rq_lock, __acquires(_T->pi_lock), __releases((*(கட்டமைப்பு task_struct **)_T)->pi_lock))
#define class_task_rq_lock_constructor(_T) WITH_LOCK_GUARD_1_ATTRS(task_rq_lock, _T)

DEFINE_LOCK_GUARD_1(__task_rq_lock, கட்டமைப்பு task_struct,
		    _T->rq = __task_rq_lock(_T->lock, &_T->rf),
		    __task_rq_unlock(_T->rq, _T->lock, &_T->rf),
		    கட்டமைப்பு rq *rq; கட்டமைப்பு rq_flags rf)

நிலையான உள்வரி வெறுமை rq_lock_irqsave(கட்டமைப்பு rq *rq, கட்டமைப்பு rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_lock_irqsave(rq, rf->flags);
	rq_pin_lock(rq, rf);
}

நிலையான உள்வரி வெறுமை rq_lock_irq(கட்டமைப்பு rq *rq, கட்டமைப்பு rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_lock_irq(rq);
	rq_pin_lock(rq, rf);
}

நிலையான உள்வரி வெறுமை rq_lock(கட்டமைப்பு rq *rq, கட்டமைப்பு rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_lock(rq);
	rq_pin_lock(rq, rf);
}

நிலையான உள்வரி வெறுமை rq_unlock_irqrestore(கட்டமைப்பு rq *rq, கட்டமைப்பு rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_lock(rq, rf);
	raw_spin_rq_unlock_irqrestore(rq, rf->flags);
}

நிலையான உள்வரி வெறுமை rq_unlock_irq(கட்டமைப்பு rq *rq, கட்டமைப்பு rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_lock(rq, rf);
	raw_spin_rq_unlock_irq(rq);
}

நிலையான உள்வரி வெறுமை rq_unlock(கட்டமைப்பு rq *rq, கட்டமைப்பு rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_lock(rq, rf);
	raw_spin_rq_unlock(rq);
}

DEFINE_LOCK_GUARD_1(rq_lock, கட்டமைப்பு rq,
		    rq_lock(_T->lock, &_T->rf),
		    rq_unlock(_T->lock, &_T->rf),
		    கட்டமைப்பு rq_flags rf)

DECLARE_LOCK_GUARD_1_ATTRS(rq_lock, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(கட்டமைப்பு rq **)_T)));
#define class_rq_lock_constructor(_T) WITH_LOCK_GUARD_1_ATTRS(rq_lock, _T)

DEFINE_LOCK_GUARD_1(rq_lock_irq, கட்டமைப்பு rq,
		    rq_lock_irq(_T->lock, &_T->rf),
		    rq_unlock_irq(_T->lock, &_T->rf),
		    கட்டமைப்பு rq_flags rf)

DECLARE_LOCK_GUARD_1_ATTRS(rq_lock_irq, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(கட்டமைப்பு rq **)_T)));
#define class_rq_lock_irq_constructor(_T) WITH_LOCK_GUARD_1_ATTRS(rq_lock_irq, _T)

DEFINE_LOCK_GUARD_1(rq_lock_irqsave, கட்டமைப்பு rq,
		    rq_lock_irqsave(_T->lock, &_T->rf),
		    rq_unlock_irqrestore(_T->lock, &_T->rf),
		    கட்டமைப்பு rq_flags rf)

DECLARE_LOCK_GUARD_1_ATTRS(rq_lock_irqsave, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(கட்டமைப்பு rq **)_T)));
#define class_rq_lock_irqsave_constructor(_T) WITH_LOCK_GUARD_1_ATTRS(rq_lock_irqsave, _T)

DEFINE_LOCK_GUARD_1(raw_spin_rq_lock_irqsave, கட்டமைப்பு rq,
		    raw_spin_rq_lock_irqsave(_T->lock, _T->flags),
		    raw_spin_rq_unlock_irqrestore(_T->lock, _T->flags),
		    unsigned long flags)

DECLARE_LOCK_GUARD_1_ATTRS(raw_spin_rq_lock_irqsave, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(கட்டமைப்பு rq **)_T)));
#define class_raw_spin_rq_lock_irqsave_constructor(_T) \
	WITH_LOCK_GUARD_1_ATTRS(raw_spin_rq_lock_irqsave, _T)

#define this_rq_lock_irq(...) __acquire_ret(_this_rq_lock_irq(__VA_ARGS__), __rq_lockp(__ret))
நிலையான உள்வரி கட்டமைப்பு rq *_this_rq_lock_irq(கட்டமைப்பு rq_flags *rf) __acquires_ret
{
	கட்டமைப்பு rq *rq;

	local_irq_disable();
	rq = this_rq();
	rq_lock(rq, rf);

	திரும்பு rq;
}

#ifdef CONFIG_NUMA

எண்ணிக்கை numa_topology_type {
	NUMA_DIRECT,
	NUMA_GLUELESS_MESH,
	NUMA_BACKPLANE,
};

வெளி எண்ணிக்கை numa_topology_type sched_numa_topology_type;
வெளி முழு sched_max_numa_distance;
வெளி bool find_numa_distance(முழு distance);
வெளி வெறுமை sched_init_numa(முழு offline_முனை);
வெளி வெறுமை sched_update_numa(முழு cpu, bool online);
வெளி வெறுமை sched_domains_numa_masks_set(unsigned முழு cpu);
வெளி வெறுமை sched_domains_numa_masks_clear(unsigned முழு cpu);
வெளி முழு sched_numa_find_closest(மாறிலி கட்டமைப்பு cpumask *cpus, முழு cpu);

#இல்லையெனில் /* !CONFIG_NUMA: */

நிலையான உள்வரி வெறுமை sched_init_numa(முழு offline_முனை) { }
நிலையான உள்வரி வெறுமை sched_update_numa(முழு cpu, bool online) { }
நிலையான உள்வரி வெறுமை sched_domains_numa_masks_set(unsigned முழு cpu) { }
நிலையான உள்வரி வெறுமை sched_domains_numa_masks_clear(unsigned முழு cpu) { }

நிலையான உள்வரி முழு sched_numa_find_closest(மாறிலி கட்டமைப்பு cpumask *cpus, முழு cpu)
{
	திரும்பு nr_cpu_ids;
}

#endif /* !CONFIG_NUMA */

#ifdef CONFIG_NUMA_BALANCING

/* The regions in numa_faults array from task_struct */
எண்ணிக்கை numa_faults_stats {
	NUMA_MEM = 0,
	NUMA_CPU,
	NUMA_MEMBUF,
	NUMA_CPUBUF
};

வெளி வெறுமை sched_setnuma(கட்டமைப்பு task_struct *p, முழு முனை);
வெளி முழு migrate_task_to(கட்டமைப்பு task_struct *p, முழு cpu);
வெளி முழு migrate_swap(கட்டமைப்பு task_struct *p, கட்டமைப்பு task_struct *t,
			முழு cpu, முழு scpu);
வெளி வெறுமை init_numa_balancing(u64 clone_flags, கட்டமைப்பு task_struct *p);

#இல்லையெனில் /* !CONFIG_NUMA_BALANCING: */

நிலையான உள்வரி வெறுமை
init_numa_balancing(u64 clone_flags, கட்டமைப்பு task_struct *p)
{
}

#endif /* !CONFIG_NUMA_BALANCING */

முழு task_llc(மாறிலி கட்டமைப்பு task_struct *p);

நிலையான உள்வரி வெறுமை
queue_balance_callback(கட்டமைப்பு rq *rq,
		       கட்டமைப்பு balance_callback *head,
		       வெறுமை (*func)(கட்டமைப்பு rq *rq))
{
	lockdep_assert_rq_held(rq);

	/*
	 * Don't (re)queue an already queued item; nor queue anything when
	 * balance_push() is active, see the comment with
	 * balance_push_callback.
	 */
	எனில் (unlikely(head->next || rq->balance_callback == &balance_தள்ளு_callback))
		திரும்பு;

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
	க்கு (__sd = rcu_dereference_sched_domain(cpu_rq(cpu)->sd); \
			__sd; __sd = __sd->parent)

/* A mask of all the SD flags that have the SDF_SHARED_CHILD metaflag */
#define SD_FLAG(name, mflags) (name * !!((mflags) & SDF_SHARED_CHILD)) |
நிலையான மாறிலி unsigned முழு SD_SHARED_CHILD_MASK =
#அந்தர்பாவ <linux/sched/sd_flags.h>
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
நிலையான உள்வரி கட்டமைப்பு sched_domain *highest_flag_domain(முழு cpu, முழு flag)
{
	கட்டமைப்பு sched_domain *sd, *hsd = வெற்று;

	for_each_domain(cpu, sd) {
		எனில் (sd->flags & flag) {
			hsd = sd;
			தொடர்;
		}

		/*
		 * Stop the search if @flag is known to be shared at lower
		 * levels. It will not be found further up.
		 */
		எனில் (flag & SD_SHARED_CHILD_MASK)
			நிறுத்து;
	}

	திரும்பு hsd;
}

நிலையான உள்வரி கட்டமைப்பு sched_domain *lowest_flag_domain(முழு cpu, முழு flag)
{
	கட்டமைப்பு sched_domain *sd;

	for_each_domain(cpu, sd) {
		எனில் (sd->flags & flag)
			நிறுத்து;
	}

	திரும்பு sd;
}

DECLARE_PER_CPU(கட்டமைப்பு sched_domain __rcu *, sd_llc);
DECLARE_PER_CPU(முழு, sd_llc_size);
DECLARE_PER_CPU(முழு, sd_llc_id);
DECLARE_PER_CPU(முழு, sd_share_id);
DECLARE_PER_CPU(கட்டமைப்பு sched_domain_shared __rcu *, sd_llc_shared);
DECLARE_PER_CPU(கட்டமைப்பு sched_domain_shared __rcu *, sd_balance_shared);
DECLARE_PER_CPU(கட்டமைப்பு sched_domain __rcu *, sd_numa);
DECLARE_PER_CPU(கட்டமைப்பு sched_domain __rcu *, sd_asym_packing);
DECLARE_PER_CPU(கட்டமைப்பு sched_domain __rcu *, sd_asym_cpucapacity);

வெளி கட்டமைப்பு static_சாவி_false sched_asym_cpucapacity;
வெளி கட்டமைப்பு static_சாவி_false sched_cluster_active;

நிலையான __always_inline bool sched_asym_cpucap_active(வெறுமை)
{
	திரும்பு static_branch_unlikely(&sched_asym_cpucapacity);
}

கட்டமைப்பு sched_group_capacity {
	atomic_t		ref;
	/*
	 * CPU capacity of this group, SCHED_CAPACITY_SCALE being max capacity
	 * for a single CPU.
	 */
	unsigned long		capacity;
	unsigned long		min_capacity;		/* Min per-CPU capacity in group */
	unsigned long		max_capacity;		/* Max per-CPU capacity in group */
	unsigned long		next_update;
	முழு			imbalance;		/* XXX unrelated to capacity but shared group state */

	முழு			id;

	unsigned long		cpumask[];		/* Balance mask */
};

கட்டமைப்பு sched_group {
	கட்டமைப்பு sched_group	*next;			/* Must be a circular list */
	atomic_t		ref;

	unsigned முழு		group_weight;
	unsigned முழு		cores;
	கட்டமைப்பு sched_group_capacity *sgc;
	முழு			asym_prefer_cpu;	/* CPU of highest priority in group */
	முழு			flags;

	/*
	 * The CPUs this group covers.
	 *
	 * NOTE: this field is variable length. (Allocated dynamically
	 * by attaching extra space to the end of the structure,
	 * depending on how many CPUs the kernel has booted up with)
	 */
	unsigned long		cpumask[];
};

நிலையான உள்வரி கட்டமைப்பு cpumask *sched_group_span(கட்டமைப்பு sched_group *sg)
{
	திரும்பு to_cpumask(sg->cpumask);
}

/*
 * See build_balance_mask().
 */
நிலையான உள்வரி கட்டமைப்பு cpumask *group_balance_mask(கட்டமைப்பு sched_group *sg)
{
	திரும்பு to_cpumask(sg->sgc->cpumask);
}

வெளி முழு group_balance_cpu(கட்டமைப்பு sched_group *sg);

வெளி வெறுமை update_sched_domain_debugfs(வெறுமை);
வெளி வெறுமை dirty_sched_domain_sysctl(முழு cpu);

வெளி முழு sched_update_scaling(வெறுமை);

நிலையான உள்வரி மாறிலி கட்டமைப்பு cpumask *task_user_cpus(கட்டமைப்பு task_struct *p)
{
	எனில் (!p->user_cpus_ptr)
		திரும்பு cpu_possible_mask; /* &init_task.cpus_mask */
	திரும்பு p->user_cpus_ptr;
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
நிலையான உள்வரி கட்டமைப்பு task_group *task_group(கட்டமைப்பு task_struct *p)
{
	திரும்பு p->sched_task_group;
}

#ifdef CONFIG_FAIR_GROUP_SCHED
/*
 * Defined here to be available before stats.h is included, since
 * stats.h has dependencies on things defined later in this file.
 */
கட்டமைப்பு cfs_tg_state {
	கட்டமைப்பு cfs_rq		cfs_rq;
	கட்டமைப்பு sched_entity	se;
	கட்டமைப்பு sched_statistics	stats;
} __no_randomize_layout;

/* Access a specific CPU's cfs_rq from a task group */
நிலையான உள்வரி கட்டமைப்பு cfs_rq *tg_cfs_rq(கட்டமைப்பு task_group *tg, முழு cpu)
{
	திரும்பு per_cpu_ptr(tg->cfs_rq, cpu);
}

நிலையான உள்வரி கட்டமைப்பு sched_entity *tg_se(கட்டமைப்பு task_group *tg, முழு cpu)
{
	கட்டமைப்பு cfs_tg_state *state;

	எனில் (is_வேர்_task_group(tg))
		திரும்பு வெற்று;

	state = container_of(tg_cfs_rq(tg, cpu), கட்டமைப்பு cfs_tg_state, cfs_rq);
	திரும்பு &state->se;
}

நிலையான உள்வரி கட்டமைப்பு sched_entity *cfs_rq_se(கட்டமைப்பு cfs_rq *cfs_rq)
{
	கட்டமைப்பு cfs_tg_state *state;

	எனில் (is_வேர்_task_group(cfs_rq->tg))
		திரும்பு வெற்று;

	state = container_of(cfs_rq, கட்டமைப்பு cfs_tg_state, cfs_rq);
	திரும்பு &state->se;
}
#endif

/* Change a task's cfs_rq and parent entity if it moves across CPUs/groups */
நிலையான உள்வரி வெறுமை set_task_rq(கட்டமைப்பு task_struct *p, unsigned முழு cpu)
{
#எனில் defined(CONFIG_FAIR_GROUP_SCHED) || defined(CONFIG_RT_GROUP_SCHED)
	கட்டமைப்பு task_group *tg = task_group(p);
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
	எனில் (!rt_group_sched_enabled())
		tg = &வேர்_task_group;
	p->rt.rt_rq  = tg->rt_rq[cpu];
	p->rt.parent = tg->rt_se[cpu];
#endif /* CONFIG_RT_GROUP_SCHED */
}

#இல்லையெனில் /* !CONFIG_CGROUP_SCHED: */

நிலையான உள்வரி வெறுமை set_task_rq(கட்டமைப்பு task_struct *p, unsigned முழு cpu) { }

நிலையான உள்வரி கட்டமைப்பு task_group *task_group(கட்டமைப்பு task_struct *p)
{
	திரும்பு வெற்று;
}

#endif /* !CONFIG_CGROUP_SCHED */

நிலையான உள்வரி வெறுமை __set_task_cpu(கட்டமைப்பு task_struct *p, unsigned முழு cpu)
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

எண்ணிக்கை {
#அந்தர்பாவ "features.h"
	__SCHED_FEAT_NR,
};

#undef SCHED_FEAT

/*
 * To support run-time toggling of sched features, all the translation units
 * (but core.c) reference the sysctl_sched_features defined in core.c.
 */
வெளி __read_mostly unsigned முழு sysctl_sched_features;

#ifdef CONFIG_JUMP_LABEL

ஒன்றிப்பு sched_feat_சாவி {
	கட்டமைப்பு static_சாவி_true	சாவி_true;
	கட்டமைப்பு static_சாவி_false	சாவி_false;
};

#define sched_feat_branch_true(சாவி)	static_branch_likely(&(சாவி)->சாவி_true)
#define sched_feat_branch_false(சாவி)	static_branch_unlikely(&(சாவி)->சாவி_false)

#define SCHED_FEAT(name, enabled)			\
நிலையான __always_inline bool				\
static_branch_##name(ஒன்றிப்பு sched_feat_சாவி *சாவி) 	\
{							\
	திரும்பு sched_feat_branch_##enabled(சாவி);	\
}

#அந்தர்பாவ "features.h"
#undef SCHED_FEAT

வெளி ஒன்றிப்பு sched_feat_சாவி sched_feat_சாவிகள்[__SCHED_FEAT_NR];
#define sched_feat(x) (static_branch_##x(&sched_feat_சாவிகள்[__SCHED_FEAT_##x]))

#இல்லையெனில் /* !CONFIG_JUMP_LABEL: */

#define sched_feat(x) (sysctl_sched_features & (1UL << __SCHED_FEAT_##x))

#endif /* !CONFIG_JUMP_LABEL */

வெளி கட்டமைப்பு static_சாவி_false sched_numa_balancing;
வெளி கட்டமைப்பு static_சாவி_false sched_schedstats;

நிலையான உள்வரி u64 global_rt_period(வெறுமை)
{
	திரும்பு (u64)sysctl_sched_rt_period * NSEC_PER_USEC;
}

நிலையான உள்வரி u64 global_rt_runtime(வெறுமை)
{
	எனில் (sysctl_sched_rt_runtime < 0)
		திரும்பு RUNTIME_INF;

	திரும்பு (u64)sysctl_sched_rt_runtime * NSEC_PER_USEC;
}

/*
 * Is p the current execution context?
 */
நிலையான உள்வரி முழு task_current(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p)
{
	திரும்பு rq->curr == p;
}

/*
 * Is p the current scheduling context?
 *
 * Note that it might be the current execution context at the same time if
 * rq->curr == rq->donor == p.
 */
நிலையான உள்வரி முழு task_current_donor(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p)
{
	திரும்பு rq->donor == p;
}

நிலையான உள்வரி bool task_is_blocked(கட்டமைப்பு task_struct *p)
{
	எனில் (!sched_proxy_exec())
		திரும்பு false;

	திரும்பு !!p->blocked_on;
}

#ifdef CONFIG_SCHED_PROXY_EXEC
வெறுமை sched_proxy_block_task(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p);
#இல்லையெனில்
நிலையான உள்வரி வெறுமை sched_proxy_block_task(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p) {}
#endif

நிலையான உள்வரி முழு task_on_cpu(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p)
{
	திரும்பு p->on_cpu;
}

நிலையான உள்வரி முழு task_on_rq_queued(கட்டமைப்பு task_struct *p)
{
	திரும்பு READ_ONCE(p->on_rq) == TASK_ON_RQ_QUEUED;
}

நிலையான உள்வரி முழு task_on_rq_migrating(கட்டமைப்பு task_struct *p)
{
	திரும்பு READ_ONCE(p->on_rq) == TASK_ON_RQ_MIGRATING;
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

வெளி மாறிலி முழு		sched_prio_to_weight[40];
வெளி மாறிலி u32		sched_prio_to_wmult[40];

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

#define RETRY_TASK		((வெறுமை *)-1UL)

கட்டமைப்பு affinity_context {
	மாறிலி கட்டமைப்பு cpumask	*new_mask;
	கட்டமைப்பு cpumask		*user_mask;
	unsigned முழு		flags;
};

வெளி s64 update_curr_common(கட்டமைப்பு rq *rq);

எண்ணிக்கை snt_e {
	SNT_NORMAL,	/* set_next_task() */
	SNT_PICK,	/* put_prev_set_next_task(): prev != next */
	SNT_REPICK,	/* put_prev_set_next_task(): prev == next */
};

கட்டமைப்பு sched_class {

#ifdef CONFIG_UCLAMP_TASK
	முழு uclamp_enabled;
#endif

	/*
	 * move_queued_task/activate_task/enqueue_task: rq->lock
	 * ttwu_do_activate/activate_task/enqueue_task: rq->lock
	 * wake_up_new_task/activate_task/enqueue_task: task_rq_lock
	 * ttwu_runnable/enqueue_task: task_rq_lock
	 * proxy_task_current: rq->lock
	 * sched_change_end
	 */
	வெறுமை (*enqueue_task) (கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p, முழு flags);
	/*
	 * move_queued_task/deactivate_task/dequeue_task: rq->lock
	 * __schedule/block_task/dequeue_task: rq->lock
	 * proxy_task_current: rq->lock
	 * wait_task_inactive: task_rq_lock
	 * sched_change_begin
	 */
	bool (*dequeue_task) (கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p, முழு flags);

	/*
	 * do_sched_yield: rq->lock
	 */
	வெறுமை (*yield_task)   (கட்டமைப்பு rq *rq);
	/*
	 * yield_to: rq->lock (double)
	 */
	bool (*yield_to_task)(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p);

	/*
	 * move_queued_task: rq->lock
	 * __migrate_swap_task: rq->lock
	 * ttwu_do_activate: rq->lock
	 * ttwu_runnable: task_rq_lock
	 * wake_up_new_task: task_rq_lock
	 */
	வெறுமை (*wakeup_preempt)(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p, முழு flags);

	/*
	 * schedule/pick_next_task/prev_balance: rq->lock
	 */
	முழு (*balance)(கட்டமைப்பு rq *rq, கட்டமைப்பு rq_flags *rf);

	/*
	 * schedule/pick_next_task: rq->lock
	 */
	கட்டமைப்பு task_struct *(*pick_task)(கட்டமைப்பு rq *rq, கட்டமைப்பு rq_flags *rf);

	/*
	 * sched_change:
	 * __schedule: rq->lock
	 */
	வெறுமை (*put_prev_task)(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p, கட்டமைப்பு task_struct *next);
	வெறுமை (*set_next_task)(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p, எண்ணிக்கை snt_e type);

	/*
	 * select_task_rq: p->pi_lock
	 * sched_exec: p->pi_lock
	 */
	முழு  (*select_task_rq)(கட்டமைப்பு task_struct *p, முழு task_cpu, முழு flags);

	/*
	 * set_task_cpu: p->pi_lock || rq->lock (ttwu like)
	 */
	வெறுமை (*migrate_task_rq)(கட்டமைப்பு task_struct *p, முழு new_cpu);

	/*
	 * ttwu_do_activate: rq->lock
	 * wake_up_new_task: task_rq_lock
	 */
	வெறுமை (*task_woken)(கட்டமைப்பு rq *this_rq, கட்டமைப்பு task_struct *task);

	/*
	 * do_set_cpus_allowed: task_rq_lock + sched_change
	 */
	வெறுமை (*set_cpus_allowed)(கட்டமைப்பு task_struct *p, கட்டமைப்பு affinity_context *ctx);

	/*
	 * sched_set_rq_{on,off}line: rq->lock
	 */
	வெறுமை (*rq_online)(கட்டமைப்பு rq *rq);
	வெறுமை (*rq_offline)(கட்டமைப்பு rq *rq);

	/*
	 * push_cpu_stop: p->pi_lock && rq->lock
	 */
	கட்டமைப்பு rq *(*find_lock_rq)(கட்டமைப்பு task_struct *p, கட்டமைப்பு rq *rq);

	/*
	 * hrtick: rq->lock
	 * sched_tick: rq->lock
	 * sched_tick_remote: rq->lock
	 */
	வெறுமை (*task_tick)(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p, முழு queued);
	/*
	 * sched_cgroup_fork: p->pi_lock
	 */
	வெறுமை (*task_fork)(கட்டமைப்பு task_struct *p);
	/*
	 * finish_task_switch: no locks
	 */
	வெறுமை (*task_dead)(கட்டமைப்பு task_struct *p);

	/*
	 * sched_change
	 */
	வெறுமை (*switching_from)(கட்டமைப்பு rq *this_rq, கட்டமைப்பு task_struct *task);
	வெறுமை (*switched_from) (கட்டமைப்பு rq *this_rq, கட்டமைப்பு task_struct *task);
	வெறுமை (*switching_to)  (கட்டமைப்பு rq *this_rq, கட்டமைப்பு task_struct *task);
	வெறுமை (*switched_to)   (கட்டமைப்பு rq *this_rq, கட்டமைப்பு task_struct *task);
	u64  (*get_prio)     (கட்டமைப்பு rq *this_rq, கட்டமைப்பு task_struct *task);
	வெறுமை (*prio_changed) (கட்டமைப்பு rq *this_rq, கட்டமைப்பு task_struct *task,
			      u64 oldprio);

	/*
	 * set_load_weight: task_rq_lock + sched_change
	 * __setscheduler_parms: task_rq_lock + sched_change
	 */
	வெறுமை (*reweight_task)(கட்டமைப்பு rq *this_rq, கட்டமைப்பு task_struct *task,
			      மாறிலி கட்டமைப்பு load_weight *lw);

	/*
	 * sched_rr_get_interval: task_rq_lock
	 */
	unsigned முழு (*get_rr_interval)(கட்டமைப்பு rq *rq,
					கட்டமைப்பு task_struct *task);

	/*
	 * task_sched_runtime: task_rq_lock
	 */
	வெறுமை (*update_curr)(கட்டமைப்பு rq *rq);

#ifdef CONFIG_FAIR_GROUP_SCHED
	/*
	 * sched_change_group: task_rq_lock + sched_change
	 */
	வெறுமை (*task_change_group)(கட்டமைப்பு task_struct *p);
#endif

#ifdef CONFIG_SCHED_CORE
	/*
	 * pick_next_task: rq->lock
	 * try_steal_cookie: rq->lock (double)
	 */
	முழு (*task_is_throttled)(கட்டமைப்பு task_struct *p, முழு cpu);
#endif
};

நிலையான உள்வரி வெறுமை put_prev_task(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *prev)
{
	WARN_ON_ONCE(rq->donor != prev);
	prev->sched_class->put_prev_task(rq, prev, வெற்று);
}

நிலையான உள்வரி வெறுமை set_next_task(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *next)
{
	next->sched_class->set_next_task(rq, next, SNT_NORMAL);
}

நிலையான உள்வரி வெறுமை
__put_prev_set_next_dl_server(கட்டமைப்பு rq *rq,
			      கட்டமைப்பு task_struct *prev,
			      கட்டமைப்பு task_struct *next)
{
	prev->dl_server = வெற்று;
	next->dl_server = rq->dl_server;
	rq->dl_server = வெற்று;
}

நிலையான உள்வரி வெறுமை put_prev_set_next_task(கட்டமைப்பு rq *rq,
					  கட்டமைப்பு task_struct *prev,
					  கட்டமைப்பு task_struct *next)
{
	WARN_ON_ONCE(rq->donor != prev);

	__put_prev_set_next_dl_server(rq, prev, next);

	எனில் (next == prev) {
		next->sched_class->set_next_task(rq, next, SNT_REPICK);
		திரும்பு;
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
மாறிலி கட்டமைப்பு sched_class name##_sched_class \
	__aligned(__alignof__(கட்டமைப்பு sched_class)) \
	__section("__" #name "_sched_class")

/* Defined in include/asm-generic/vmlinux.lds.h */
வெளி கட்டமைப்பு sched_class __sched_class_highest[];
வெளி கட்டமைப்பு sched_class __sched_class_lowest[];

வெளி மாறிலி கட்டமைப்பு sched_class stop_sched_class;
வெளி மாறிலி கட்டமைப்பு sched_class dl_sched_class;
வெளி மாறிலி கட்டமைப்பு sched_class rt_sched_class;
வெளி மாறிலி கட்டமைப்பு sched_class fair_sched_class;
வெளி மாறிலி கட்டமைப்பு sched_class idle_sched_class;

/*
 * Iterate only active classes. SCX can take over all fair tasks or be
 * completely disabled. If the former, skip fair. If the latter, skip SCX.
 */
நிலையான உள்வரி மாறிலி கட்டமைப்பு sched_class *next_active_class(மாறிலி கட்டமைப்பு sched_class *class)
{
	class++;
#ifdef CONFIG_SCHED_CLASS_EXT
	எனில் (scx_switched_all() && class == &fair_sched_class)
		class++;
	எனில் (!scx_enabled() && class == &ext_sched_class)
		class++;
#endif
	திரும்பு class;
}

#define for_class_range(class, _from, _to) \
	க்கு (class = (_from); class < (_to); class++)

#define for_each_class(class) \
	for_class_range(class, __sched_class_highest, __sched_class_lowest)

#define for_active_class_range(class, _from, _to)				\
	க்கு (class = (_from); class != (_to); class = next_active_class(class))

#define for_each_active_class(class)						\
	for_active_class_range(class, __sched_class_highest, __sched_class_lowest)

#define sched_class_above(_a, _b)	((_a) < (_b))

நிலையான உள்வரி வெறுமை rq_modified_begin(கட்டமைப்பு rq *rq, மாறிலி கட்டமைப்பு sched_class *class)
{
	எனில் (sched_class_above(rq->next_class, class))
		rq->next_class = class;
}

நிலையான உள்வரி bool rq_modified_above(கட்டமைப்பு rq *rq, மாறிலி கட்டமைப்பு sched_class *class)
{
	திரும்பு sched_class_above(rq->next_class, class);
}

நிலையான உள்வரி bool sched_stop_runnable(கட்டமைப்பு rq *rq)
{
	திரும்பு rq->stop && task_on_rq_queued(rq->stop);
}

நிலையான உள்வரி bool sched_dl_runnable(கட்டமைப்பு rq *rq)
{
	திரும்பு rq->dl.dl_nr_running > 0;
}

நிலையான உள்வரி bool sched_rt_runnable(கட்டமைப்பு rq *rq)
{
	திரும்பு rq->rt.rt_queued > 0;
}

நிலையான உள்வரி bool sched_fair_runnable(கட்டமைப்பு rq *rq)
{
	திரும்பு rq->cfs.nr_queued > 0;
}

வெளி கட்டமைப்பு task_struct *pick_task_fair(கட்டமைப்பு rq *rq, கட்டமைப்பு rq_flags *rf);
வெளி கட்டமைப்பு task_struct *pick_task_idle(கட்டமைப்பு rq *rq, கட்டமைப்பு rq_flags *rf);

#define SCA_CHECK		0x01
#define SCA_MIGRATE_DISABLE	0x02
#define SCA_MIGRATE_ENABLE	0x04
#define SCA_USER		0x08

வெளி வெறுமை update_group_capacity(கட்டமைப்பு sched_domain *sd, முழு cpu);

வெளி வெறுமை sched_balance_trigger(கட்டமைப்பு rq *rq);

வெளி முழு __set_cpus_allowed_ptr(கட்டமைப்பு task_struct *p, கட்டமைப்பு affinity_context *ctx);
வெளி வெறுமை set_cpus_allowed_common(கட்டமைப்பு task_struct *p, கட்டமைப்பு affinity_context *ctx);

நிலையான உள்வரி bool task_allowed_on_cpu(கட்டமைப்பு task_struct *p, முழு cpu)
{
	/* When not in the task's cpumask, no point in looking further. */
	எனில் (!cpumask_test_cpu(cpu, p->cpus_ptr))
		திரும்பு false;

	/* Can @cpu run a user thread? */
	எனில் (!(p->flags & PF_KTHREAD) && !task_cpu_possible(cpu, p))
		திரும்பு false;

	திரும்பு true;
}

நிலையான உள்வரி cpumask_t *alloc_user_cpus_ptr(முழு முனை)
{
	/*
	 * See set_cpus_allowed_force() above for the rcu_head usage.
	 */
	முழு size = max_t(முழு, cpumask_size(), அளவு(கட்டமைப்பு rcu_head));

	திரும்பு kmalloc_முனை(size, GFP_KERNEL, முனை);
}

நிலையான உள்வரி கட்டமைப்பு task_struct *get_தள்ளு_task(கட்டமைப்பு rq *rq)
{
	கட்டமைப்பு task_struct *p = rq->donor;

	lockdep_assert_rq_held(rq);

	எனில் (rq->தள்ளு_busy)
		திரும்பு வெற்று;

	எனில் (p->nr_cpus_allowed == 1)
		திரும்பு வெற்று;

	எனில் (p->migration_disabled)
		திரும்பு வெற்று;

	rq->தள்ளு_busy = true;
	திரும்பு get_task_struct(p);
}

வெளி முழு தள்ளு_cpu_stop(வெறுமை *arg);

#ifdef CONFIG_CPU_IDLE

நிலையான உள்வரி வெறுமை idle_set_state(கட்டமைப்பு rq *rq,
				  கட்டமைப்பு cpuidle_state *idle_state)
{
	rq->idle_state = idle_state;
}

நிலையான உள்வரி கட்டமைப்பு cpuidle_state *idle_get_state(கட்டமைப்பு rq *rq)
{
	lockdep_assert(rcu_read_lock_any_held());

	திரும்பு rq->idle_state;
}

#இல்லையெனில் /* !CONFIG_CPU_IDLE: */

நிலையான உள்வரி வெறுமை idle_set_state(கட்டமைப்பு rq *rq,
				  கட்டமைப்பு cpuidle_state *idle_state)
{
}

நிலையான உள்வரி கட்டமைப்பு cpuidle_state *idle_get_state(கட்டமைப்பு rq *rq)
{
	திரும்பு வெற்று;
}

#endif /* !CONFIG_CPU_IDLE */

வெளி வெறுமை schedule_idle(வெறுமை);
asmlinkage வெறுமை schedule_user(வெறுமை);

வெளி வெறுமை sysrq_sched_debug_show(வெறுமை);
வெளி வெறுமை sched_init_granularity(வெறுமை);
வெளி வெறுமை update_max_interval(வெறுமை);

வெளி வெறுமை init_sched_dl_class(வெறுமை);
வெளி வெறுமை init_sched_rt_class(வெறுமை);
வெளி வெறுமை init_sched_fair_class(வெறுமை);

வெளி வெறுமை resched_curr(கட்டமைப்பு rq *rq);
வெளி வெறுமை resched_curr_lazy(கட்டமைப்பு rq *rq);
வெளி வெறுமை resched_cpu(முழு cpu);

வெளி வெறுமை init_rt_bandwidth(கட்டமைப்பு rt_bandwidth *rt_b, u64 period, u64 runtime);
வெளி bool sched_rt_bandwidth_account(கட்டமைப்பு rt_rq *rt_rq);

வெளி வெறுமை init_dl_entity(கட்டமைப்பு sched_dl_entity *dl_se);

வெளி வெறுமை init_cfs_throttle_work(கட்டமைப்பு task_struct *p);

#define BW_SHIFT		20
#define BW_UNIT			(1 << BW_SHIFT)
#define RATIO_SHIFT		8
#define MAX_BW_BITS		(64 - BW_SHIFT)
#define MAX_BW			((1ULL << MAX_BW_BITS) - 1)

வெளி u64 to_ratio(u64 period, u64 runtime);

வெளி வெறுமை init_entity_runnable_average(கட்டமைப்பு sched_entity *se);
வெளி வெறுமை post_init_entity_util_avg(கட்டமைப்பு task_struct *p);

#ifdef CONFIG_NO_HZ_FULL
வெளி bool sched_can_stop_tick(கட்டமைப்பு rq *rq);
வெளி முழு __init sched_tick_offload_init(வெறுமை);

/*
 * Tick may be needed by tasks in the runqueue depending on their policy and
 * requirements. If tick is needed, lets send the target an IPI to kick it out of
 * nohz mode if necessary.
 */
நிலையான உள்வரி வெறுமை sched_update_tick_dependency(கட்டமைப்பு rq *rq)
{
	முழு cpu = cpu_of(rq);

	எனில் (!tick_nohz_full_cpu(cpu))
		திரும்பு;

	எனில் (sched_can_stop_tick(rq))
		tick_nohz_dep_clear_cpu(cpu, TICK_DEP_BIT_SCHED);
	இல்லையெனில்
		tick_nohz_dep_set_cpu(cpu, TICK_DEP_BIT_SCHED);
}
#இல்லையெனில் /* !CONFIG_NO_HZ_FULL: */
நிலையான உள்வரி முழு sched_tick_offload_init(வெறுமை) { திரும்பு 0; }
நிலையான உள்வரி வெறுமை sched_update_tick_dependency(கட்டமைப்பு rq *rq) { }
#endif /* !CONFIG_NO_HZ_FULL */

நிலையான உள்வரி வெறுமை add_nr_running(கட்டமைப்பு rq *rq, unsigned மொத்தம்)
{
	unsigned prev_nr = rq->nr_running;

	rq->nr_running = prev_nr + மொத்தம்;
	எனில் (trace_sched_update_nr_running_tp_enabled()) {
		call_trace_sched_update_nr_running(rq, மொத்தம்);
	}

	எனில் (prev_nr < 2 && rq->nr_running >= 2)
		set_rd_overloaded(rq->rd, 1);

	sched_update_tick_dependency(rq);
}

நிலையான உள்வரி வெறுமை sub_nr_running(கட்டமைப்பு rq *rq, unsigned மொத்தம்)
{
	rq->nr_running -= மொத்தம்;
	எனில் (trace_sched_update_nr_running_tp_enabled()) {
		call_trace_sched_update_nr_running(rq, -மொத்தம்);
	}

	/* Check if we still need preemption */
	sched_update_tick_dependency(rq);
}

நிலையான உள்வரி வெறுமை __block_task(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p)
{
	எனில் (p->sched_contributes_to_load)
		rq->nr_uninterruptible++;

	எனில் (p->in_iowait) {
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

வெளி வெறுமை activate_task(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p, முழு flags);
வெளி வெறுமை deactivate_task(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p, முழு flags);

வெளி வெறுமை wakeup_preempt(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p, முழு flags);

/*
 * attach_task() -- attach the task detached by detach_task() to its new rq.
 */
நிலையான உள்வரி வெறுமை attach_task(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p)
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
நிலையான உள்வரி வெறுமை attach_one_task(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p)
{
	guard(rq_lock)(rq);
	update_rq_clock(rq);
	attach_task(rq, p);
}

/*
 * __attach_tasks() - attaches a list of tasks (using se.group_node) to
 * the new rq
 */
நிலையான உள்வரி வெறுமை __attach_tasks(கட்டமைப்பு rq *rq, கட்டமைப்பு list_head *tasks)
{
	guard(rq_lock)(rq);
	update_rq_clock(rq);

	போது (!list_empty(tasks)) {
		கட்டமைப்பு task_struct *p;

		p = list_first_entry(tasks, கட்டமைப்பு task_struct, se.group_முனை);
		list_del_init(&p->se.group_முனை);

		attach_task(rq, p);
	}
}

#ifdef CONFIG_PREEMPT_RT
# define SCHED_NR_MIGRATE_BREAK 8
#இல்லையெனில்
# define SCHED_NR_MIGRATE_BREAK 32
#endif

வெளி __read_mostly unsigned முழு sysctl_sched_nr_migrate;
வெளி __read_mostly unsigned முழு sysctl_sched_migration_cost;

வெளி unsigned முழு sysctl_sched_base_slice;

வெளி முழு sysctl_resched_latency_warn_ms;
வெளி முழு sysctl_resched_latency_warn_once;

வெளி unsigned முழு sysctl_sched_tunable_scaling;

வெளி unsigned முழு sysctl_numa_balancing_scan_delay;
வெளி unsigned முழு sysctl_numa_balancing_scan_period_min;
வெளி unsigned முழு sysctl_numa_balancing_scan_period_max;
வெளி unsigned முழு sysctl_numa_balancing_scan_size;
வெளி unsigned முழு sysctl_numa_balancing_hot_threshold;

#ifdef CONFIG_SCHED_HRTICK

/*
 * Use hrtick when:
 *  - enabled by features
 *  - hrtimer is actually high res
 */
நிலையான உள்வரி bool hrtick_enabled(கட்டமைப்பு rq *rq)
{
	திரும்பு cpu_active(cpu_of(rq)) && hrtimer_highres_enabled();
}

நிலையான உள்வரி bool hrtick_enabled_fair(கட்டமைப்பு rq *rq)
{
	திரும்பு sched_feat(HRTICK) && hrtick_enabled(rq);
}

நிலையான உள்வரி bool hrtick_enabled_dl(கட்டமைப்பு rq *rq)
{
	திரும்பு sched_feat(HRTICK_DL) && hrtick_enabled(rq);
}

வெளி வெறுமை hrtick_start(கட்டமைப்பு rq *rq, u64 delay);
நிலையான உள்வரி bool hrtick_active(கட்டமைப்பு rq *rq)
{
	திரும்பு hrtimer_active(&rq->hrtick_timer);
}

#இல்லையெனில் /* !CONFIG_SCHED_HRTICK: */
நிலையான உள்வரி bool hrtick_enabled_fair(கட்டமைப்பு rq *rq) { திரும்பு false; }
நிலையான உள்வரி bool hrtick_enabled_dl(கட்டமைப்பு rq *rq) { திரும்பு false; }
நிலையான உள்வரி bool hrtick_enabled(கட்டமைப்பு rq *rq) { திரும்பு false; }
#endif /* !CONFIG_SCHED_HRTICK */

#ifndef arch_scale_freq_tick
நிலையான __always_inline வெறுமை arch_scale_freq_tick(வெறுமை) { }
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
நிலையான __always_inline
unsigned long arch_scale_freq_capacity(முழு cpu)
{
	திரும்பு SCHED_CAPACITY_SCALE;
}
#endif

/*
 * In double_lock_balance()/double_rq_lock(), we use raw_spin_rq_lock() to
 * acquire rq lock instead of rq_lock(). So at the end of these two functions
 * we need to call double_rq_clock_clear_update() to clear RQCF_UPDATED of
 * rq->clock_update_flags to avoid the WARN_DOUBLE_CLOCK warning.
 */
நிலையான உள்வரி வெறுமை double_rq_clock_clear_update(கட்டமைப்பு rq *rq1, கட்டமைப்பு rq *rq2)
{
	rq1->clock_update_flags &= (RQCF_REQ_SKIP|RQCF_ACT_SKIP);
	rq2->clock_update_flags &= (RQCF_REQ_SKIP|RQCF_ACT_SKIP);
}

#define DEFINE_LOCK_GUARD_2(name, type, _lock, _unlock, ...)				\
__DEFINE_UNLOCK_GUARD(name, type, _unlock, type *lock2; __VA_ARGS__)			\
நிலையான உள்வரி class_##name##_t class_##name##_constructor(type *lock, type *lock2)	\
	__no_context_analysis								\
{ class_##name##_t _t = { .lock = lock, .lock2 = lock2 }, *_T = &_t;			\
  _lock; திரும்பு _t; }
#define DECLARE_LOCK_GUARD_2_ATTRS(_name, _lock, _unlock1, _unlock2)			\
நிலையான உள்வரி class_##_name##_t class_##_name##_constructor(lock_##_name##_t *_T1,	\
							    lock_##_name##_t *_T2) _lock; \
நிலையான __always_inline வெறுமை __class_##_name##_cleanup_ctx1(class_##_name##_t **_T1)	\
	__no_context_analysis _unlock1 { }						\
நிலையான __always_inline வெறுமை __class_##_name##_cleanup_ctx2(class_##_name##_t **_T2)	\
	__no_context_analysis _unlock2 { }
#define WITH_LOCK_GUARD_2_ATTRS(_name, _T1, _T2)					\
	class_##_name##_constructor(_T1, _T2),						\
	*__UNIQUE_ID(unlock1) __cleanup(__class_##_name##_cleanup_ctx1) = (வெறுமை *)(_T1),\
	*__UNIQUE_ID(unlock2) __cleanup(__class_##_name##_cleanup_ctx2) = (வெறுமை *)(_T2)

நிலையான உள்வரி bool rq_order_less(கட்டமைப்பு rq *rq1, கட்டமைப்பு rq *rq2)
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
	எனில் (rq1->core->cpu < rq2->core->cpu)
		திரும்பு true;
	எனில் (rq1->core->cpu > rq2->core->cpu)
		திரும்பு false;

	/*
	 * __sched_core_flip() relies on SMT having cpu-id lock order.
	 */
#endif /* CONFIG_SCHED_CORE */
	திரும்பு rq1->cpu < rq2->cpu;
}

வெளி வெறுமை double_rq_lock(கட்டமைப்பு rq *rq1, கட்டமைப்பு rq *rq2)
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
நிலையான உள்வரி முழு _double_lock_balance(கட்டமைப்பு rq *this_rq, கட்டமைப்பு rq *busiest)
	__must_hold(__rq_lockp(this_rq))
	__acquires(__rq_lockp(busiest))
{
	raw_spin_rq_unlock(this_rq);
	double_rq_lock(this_rq, busiest);

	திரும்பு 1;
}

#இல்லையெனில் /* !CONFIG_PREEMPTION: */
/*
 * Unfair double_lock_balance: Optimizes throughput at the expense of
 * latency by eliminating extra atomic operations when the locks are
 * already in proper order on entry.  This favors lower CPU-ids and will
 * grant the double lock to lower CPUs over higher ids under contention,
 * regardless of entry order into the function.
 */
நிலையான உள்வரி முழு _double_lock_balance(கட்டமைப்பு rq *this_rq, கட்டமைப்பு rq *busiest)
	__must_hold(__rq_lockp(this_rq))
	__acquires(__rq_lockp(busiest))
{
	எனில் (__rq_lockp(this_rq) == __rq_lockp(busiest)) {
		__acquire(__rq_lockp(busiest)); /* already held */
		double_rq_clock_clear_update(this_rq, busiest);
		திரும்பு 0;
	}

	எனில் (likely(raw_spin_rq_trylock(busiest))) {
		double_rq_clock_clear_update(this_rq, busiest);
		திரும்பு 0;
	}

	எனில் (rq_order_less(this_rq, busiest)) {
		raw_spin_rq_lock_nested(busiest, SINGLE_DEPTH_NESTING);
		double_rq_clock_clear_update(this_rq, busiest);
		திரும்பு 0;
	}

	raw_spin_rq_unlock(this_rq);
	double_rq_lock(this_rq, busiest);

	திரும்பு 1;
}

#endif /* !CONFIG_PREEMPTION */

/*
 * double_lock_balance - lock the busiest runqueue, this_rq is locked already.
 */
நிலையான உள்வரி முழு double_lock_balance(கட்டமைப்பு rq *this_rq, கட்டமைப்பு rq *busiest)
	__must_hold(__rq_lockp(this_rq))
	__acquires(__rq_lockp(busiest))
{
	lockdep_assert_irqs_disabled();

	திரும்பு _double_lock_balance(this_rq, busiest);
}

நிலையான உள்வரி வெறுமை double_unlock_balance(கட்டமைப்பு rq *this_rq, கட்டமைப்பு rq *busiest)
	__releases(__rq_lockp(busiest))
{
	எனில் (__rq_lockp(this_rq) != __rq_lockp(busiest))
		raw_spin_rq_unlock(busiest);
	இல்லையெனில்
		__release(__rq_lockp(busiest)); /* fake release */
	lock_set_subclass(&__rq_lockp(this_rq)->dep_map, 0, _RET_IP_);
}

நிலையான உள்வரி வெறுமை double_lock(spinlock_t *l1, spinlock_t *l2)
	__acquires(l1, l2)
{
	எனில் (l1 > l2)
		swap(l1, l2);

	spin_lock(l1);
	spin_lock_nested(l2, SINGLE_DEPTH_NESTING);
}

நிலையான உள்வரி வெறுமை double_lock_irq(spinlock_t *l1, spinlock_t *l2)
	__acquires(l1, l2)
{
	எனில் (l1 > l2)
		swap(l1, l2);

	spin_lock_irq(l1);
	spin_lock_nested(l2, SINGLE_DEPTH_NESTING);
}

நிலையான உள்வரி வெறுமை double_raw_lock(raw_spinlock_t *l1, raw_spinlock_t *l2)
	__acquires(l1, l2)
{
	எனில் (l1 > l2)
		swap(l1, l2);

	raw_spin_lock(l1);
	raw_spin_lock_nested(l2, SINGLE_DEPTH_NESTING);
}

நிலையான உள்வரி வெறுமை double_raw_unlock(raw_spinlock_t *l1, raw_spinlock_t *l2)
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
நிலையான உள்வரி வெறுமை double_rq_unlock(கட்டமைப்பு rq *rq1, கட்டமைப்பு rq *rq2)
	__releases(__rq_lockp(rq1), __rq_lockp(rq2))
{
	எனில் (__rq_lockp(rq1) != __rq_lockp(rq2))
		raw_spin_rq_unlock(rq2);
	இல்லையெனில்
		__release(__rq_lockp(rq2)); /* fake release */
	raw_spin_rq_unlock(rq1);
}

வெளி வெறுமை set_rq_online (கட்டமைப்பு rq *rq);
வெளி வெறுமை set_rq_offline(கட்டமைப்பு rq *rq);

வெளி bool sched_smp_initialized;

DEFINE_LOCK_GUARD_2(double_rq_lock, கட்டமைப்பு rq,
		    double_rq_lock(_T->lock, _T->lock2),
		    double_rq_unlock(_T->lock, _T->lock2))

வெளி கட்டமைப்பு sched_entity *__pick_root_entity(கட்டமைப்பு cfs_rq *cfs_rq);
வெளி கட்டமைப்பு sched_entity *__pick_first_entity(கட்டமைப்பு cfs_rq *cfs_rq);
வெளி கட்டமைப்பு sched_entity *__pick_last_entity(கட்டமைப்பு cfs_rq *cfs_rq);

வெளி bool sched_debug_verbose;

வெளி வெறுமை print_cfs_stats(கட்டமைப்பு seq_file *m, முழு cpu);
வெளி வெறுமை print_rt_stats(கட்டமைப்பு seq_file *m, முழு cpu);
வெளி வெறுமை print_dl_stats(கட்டமைப்பு seq_file *m, முழு cpu);
வெளி வெறுமை print_cfs_rq(கட்டமைப்பு seq_file *m, முழு cpu, கட்டமைப்பு cfs_rq *cfs_rq);
வெளி வெறுமை print_rt_rq(கட்டமைப்பு seq_file *m, முழு cpu, கட்டமைப்பு rt_rq *rt_rq);
வெளி வெறுமை print_dl_rq(கட்டமைப்பு seq_file *m, முழு cpu, கட்டமைப்பு dl_rq *dl_rq);

வெளி வெறுமை resched_latency_warn(முழு cpu, u64 latency);

#ifdef CONFIG_NUMA_BALANCING
வெளி வெறுமை show_numa_stats(கட்டமைப்பு task_struct *p, கட்டமைப்பு seq_file *m);
வெளி வெறுமை
print_numa_stats(கட்டமைப்பு seq_file *m, முழு முனை, unsigned long tsf,
		 unsigned long tpf, unsigned long gsf, unsigned long gpf);
#endif /* CONFIG_NUMA_BALANCING */

வெளி வெறுமை init_cfs_rq(கட்டமைப்பு cfs_rq *cfs_rq);
வெளி வெறுமை init_rt_rq(கட்டமைப்பு rt_rq *rt_rq);
வெளி வெறுமை init_dl_rq(கட்டமைப்பு dl_rq *dl_rq);

வெளி வெறுமை cfs_bandwidth_usage_inc(வெறுமை);
வெளி வெறுமை cfs_bandwidth_usage_dec(வெறுமை);

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

வெளி வெறுமை nohz_balance_exit_idle(கட்டமைப்பு rq *rq);
#இல்லையெனில் /* !CONFIG_NO_HZ_COMMON: */
நிலையான உள்வரி வெறுமை nohz_balance_exit_idle(கட்டமைப்பு rq *rq) { }
#endif /* !CONFIG_NO_HZ_COMMON */

#ifdef CONFIG_NO_HZ_COMMON
வெளி வெறுமை nohz_run_idle_balance(முழு cpu);
#இல்லையெனில்
நிலையான உள்வரி வெறுமை nohz_run_idle_balance(முழு cpu) { }
#endif

#அந்தர்பாவ "stats.h"

#எனில் defined(CONFIG_SCHED_CORE) && defined(CONFIG_SCHEDSTATS)

வெளி வெறுமை __sched_core_account_forceidle(கட்டமைப்பு rq *rq);

நிலையான உள்வரி வெறுமை sched_core_account_forceidle(கட்டமைப்பு rq *rq)
{
	எனில் (schedstat_enabled())
		__sched_core_account_forceidle(rq);
}

வெளி வெறுமை __sched_core_tick(கட்டமைப்பு rq *rq);

நிலையான உள்வரி வெறுமை sched_core_tick(கட்டமைப்பு rq *rq)
{
	எனில் (sched_core_enabled(rq) && schedstat_enabled())
		__sched_core_tick(rq);
}

#இல்லையெனில் /* !(CONFIG_SCHED_CORE && CONFIG_SCHEDSTATS): */

நிலையான உள்வரி வெறுமை sched_core_account_forceidle(கட்டமைப்பு rq *rq) { }

நிலையான உள்வரி வெறுமை sched_core_tick(கட்டமைப்பு rq *rq) { }

#endif /* !(CONFIG_SCHED_CORE && CONFIG_SCHEDSTATS) */

#ifdef CONFIG_IRQ_TIME_ACCOUNTING

கட்டமைப்பு irqtime {
	u64			total;
	u64			tick_delta;
	u64			irq_start_time;
	கட்டமைப்பு u64_stats_sync	sync;
};

DECLARE_PER_CPU(கட்டமைப்பு irqtime, cpu_irqtime);
DECLARE_STATIC_சாவி_FALSE(sched_clock_irqtime);

நிலையான உள்வரி முழு irqtime_enabled(வெறுமை)
{
	திரும்பு static_branch_likely(&sched_clock_irqtime);
}

/*
 * Returns the irqtime minus the softirq time computed by ksoftirqd.
 * Otherwise ksoftirqd's sum_exec_runtime is subtracted its own runtime
 * and never move forward.
 */
நிலையான உள்வரி u64 irq_time_read(முழு cpu)
{
	கட்டமைப்பு irqtime *irqtime = &per_cpu(cpu_irqtime, cpu);
	unsigned முழு seq;
	u64 total;

	செய் {
		seq = __u64_stats_fetch_begin(&irqtime->sync);
		total = irqtime->total;
	} போது (__u64_stats_fetch_retry(&irqtime->sync, seq));

	திரும்பு total;
}

#இல்லையெனில் /* !CONFIG_IRQ_TIME_ACCOUNTING: */

நிலையான உள்வரி முழு irqtime_enabled(வெறுமை)
{
	திரும்பு 0;
}

#endif /* !CONFIG_IRQ_TIME_ACCOUNTING */

#ifdef CONFIG_CPU_FREQ

DECLARE_PER_CPU(கட்டமைப்பு update_util_data __rcu *, cpufreq_update_util_data);

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
நிலையான உள்வரி வெறுமை cpufreq_update_util(கட்டமைப்பு rq *rq, unsigned முழு flags)
{
	கட்டமைப்பு update_util_data *data;

	data = rcu_dereference_sched(*per_cpu_ptr(&cpufreq_update_util_data,
						  cpu_of(rq)));
	எனில் (data)
		data->func(data, rq_clock(rq), flags);
}
#இல்லையெனில் /* !CONFIG_CPU_FREQ: */
நிலையான உள்வரி வெறுமை cpufreq_update_util(கட்டமைப்பு rq *rq, unsigned முழு flags) { }
#endif /* !CONFIG_CPU_FREQ */

#ifdef arch_scale_freq_capacity
# ifndef arch_scale_freq_invariant
#  define arch_scale_freq_invariant()	true
# endif
#இல்லையெனில்
# define arch_scale_freq_invariant()	false
#endif

unsigned long effective_cpu_util(முழு cpu, unsigned long util_cfs,
				 unsigned long *min,
				 unsigned long *max);

unsigned long sugov_effective_cpu_perf(முழு cpu, unsigned long actual,
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
நிலையான உள்வரி bool dl_task_fits_capacity(கட்டமைப்பு task_struct *p, முழு cpu)
{
	unsigned long cap = arch_scale_cpu_capacity(cpu);

	திரும்பு cap >= p->dl.dl_density >> (BW_SHIFT - SCHED_CAPACITY_SHIFT);
}

நிலையான உள்வரி unsigned long cpu_bw_dl(கட்டமைப்பு rq *rq)
{
	திரும்பு (rq->dl.running_bw * SCHED_CAPACITY_SCALE) >> BW_SHIFT;
}

நிலையான உள்வரி unsigned long cpu_util_dl(கட்டமைப்பு rq *rq)
{
	திரும்பு READ_ONCE(rq->avg_dl.util_avg);
}


வெளி unsigned long cpu_util_cfs(முழு cpu);
வெளி unsigned long cpu_util_cfs_boost(முழு cpu);

நிலையான உள்வரி unsigned long cpu_util_rt(கட்டமைப்பு rq *rq)
{
	திரும்பு READ_ONCE(rq->avg_rt.util_avg);
}

#ifdef CONFIG_UCLAMP_TASK

unsigned long uclamp_eff_மதிப்பு(கட்டமைப்பு task_struct *p, எண்ணிக்கை uclamp_id clamp_id);

/*
 * When uclamp is compiled in, the aggregation at rq level is 'turned off'
 * by default in the fast path and only gets turned on once userspace performs
 * an operation that requires it.
 *
 * Returns true if userspace opted-in to use uclamp and aggregation at rq level
 * hence is active.
 */
நிலையான உள்வரி bool uclamp_is_used(வெறுமை)
{
	திரும்பு static_branch_likely(&sched_uclamp_used);
}

/*
 * Enabling static branches would get the cpus_read_lock(),
 * check whether uclamp_is_used before enable it to avoid always
 * calling cpus_read_lock(). Because we never disable this
 * static key once enable it.
 */
நிலையான உள்வரி வெறுமை sched_uclamp_enable(வெறுமை)
{
	எனில் (!uclamp_is_used())
		static_branch_enable(&sched_uclamp_used);
}

நிலையான உள்வரி unsigned long uclamp_rq_get(கட்டமைப்பு rq *rq,
					  எண்ணிக்கை uclamp_id clamp_id)
{
	திரும்பு READ_ONCE(rq->uclamp[clamp_id].மதிப்பு);
}

நிலையான உள்வரி வெறுமை uclamp_rq_set(கட்டமைப்பு rq *rq, எண்ணிக்கை uclamp_id clamp_id,
				 unsigned முழு மதிப்பு)
{
	WRITE_ONCE(rq->uclamp[clamp_id].மதிப்பு, மதிப்பு);
}

நிலையான உள்வரி bool uclamp_rq_is_idle(கட்டமைப்பு rq *rq)
{
	திரும்பு rq->uclamp_flags & UCLAMP_FLAG_IDLE;
}

/* Is the rq being capped/throttled by uclamp_max? */
நிலையான உள்வரி bool uclamp_rq_is_capped(கட்டமைப்பு rq *rq)
{
	unsigned long rq_util;
	unsigned long max_util;

	எனில் (!uclamp_is_used())
		திரும்பு false;

	rq_util = cpu_util_cfs(cpu_of(rq)) + cpu_util_rt(rq);
	max_util = READ_ONCE(rq->uclamp[UCLAMP_MAX].மதிப்பு);

	திரும்பு max_util != SCHED_CAPACITY_SCALE && rq_util >= max_util;
}

#define for_each_clamp_id(clamp_id) \
	க்கு ((clamp_id) = 0; (clamp_id) < UCLAMP_CNT; (clamp_id)++)

வெளி unsigned முழு sysctl_sched_uclamp_util_min_rt_default;


நிலையான உள்வரி unsigned முழு uclamp_none(எண்ணிக்கை uclamp_id clamp_id)
{
	எனில் (clamp_id == UCLAMP_MIN)
		திரும்பு 0;
	திரும்பு SCHED_CAPACITY_SCALE;
}

/* Integer rounded range for each bucket */
#define UCLAMP_BUCKET_DELTA DIV_ROUND_CLOSEST(SCHED_CAPACITY_SCALE, UCLAMP_BUCKETS)

நிலையான உள்வரி unsigned முழு uclamp_bucket_id(unsigned முழு clamp_மதிப்பு)
{
	திரும்பு min_t(unsigned முழு, clamp_மதிப்பு / UCLAMP_BUCKET_DELTA, UCLAMP_BUCKETS - 1);
}

நிலையான உள்வரி வெறுமை
uclamp_se_set(கட்டமைப்பு uclamp_se *uc_se, unsigned முழு மதிப்பு, bool user_defined)
{
	uc_se->மதிப்பு = மதிப்பு;
	uc_se->bucket_id = uclamp_bucket_id(மதிப்பு);
	uc_se->user_defined = user_defined;
}

#இல்லையெனில் /* !CONFIG_UCLAMP_TASK: */

நிலையான உள்வரி unsigned long
uclamp_eff_மதிப்பு(கட்டமைப்பு task_struct *p, எண்ணிக்கை uclamp_id clamp_id)
{
	எனில் (clamp_id == UCLAMP_MIN)
		திரும்பு 0;

	திரும்பு SCHED_CAPACITY_SCALE;
}

நிலையான உள்வரி bool uclamp_rq_is_capped(கட்டமைப்பு rq *rq) { திரும்பு false; }

நிலையான உள்வரி bool uclamp_is_used(வெறுமை)
{
	திரும்பு false;
}

நிலையான உள்வரி வெறுமை sched_uclamp_enable(வெறுமை) {}

நிலையான உள்வரி unsigned long
uclamp_rq_get(கட்டமைப்பு rq *rq, எண்ணிக்கை uclamp_id clamp_id)
{
	எனில் (clamp_id == UCLAMP_MIN)
		திரும்பு 0;

	திரும்பு SCHED_CAPACITY_SCALE;
}

நிலையான உள்வரி வெறுமை
uclamp_rq_set(கட்டமைப்பு rq *rq, எண்ணிக்கை uclamp_id clamp_id, unsigned முழு மதிப்பு)
{
}

நிலையான உள்வரி bool uclamp_rq_is_idle(கட்டமைப்பு rq *rq)
{
	திரும்பு false;
}

#endif /* !CONFIG_UCLAMP_TASK */

#ifdef CONFIG_HAVE_SCHED_AVG_IRQ

நிலையான உள்வரி unsigned long cpu_util_irq(கட்டமைப்பு rq *rq)
{
	திரும்பு READ_ONCE(rq->avg_irq.util_avg);
}

நிலையான உள்வரி
unsigned long scale_irq_capacity(unsigned long util, unsigned long irq, unsigned long max)
{
	util *= (max - irq);
	util /= max;

	திரும்பு util;

}

#இல்லையெனில் /* !CONFIG_HAVE_SCHED_AVG_IRQ: */

நிலையான உள்வரி unsigned long cpu_util_irq(கட்டமைப்பு rq *rq)
{
	திரும்பு 0;
}

நிலையான உள்வரி
unsigned long scale_irq_capacity(unsigned long util, unsigned long irq, unsigned long max)
{
	திரும்பு util;
}

#endif /* !CONFIG_HAVE_SCHED_AVG_IRQ */

வெளி வெறுமை __setparam_fair(கட்டமைப்பு task_struct *p, மாறிலி கட்டமைப்பு sched_attr *attr);

#எனில் defined(CONFIG_ENERGY_MODEL) && defined(CONFIG_CPU_FREQ_GOV_SCHEDUTIL)

#define perf_domain_span(pd) (to_cpumask(((pd)->em_pd->cpus)))

DECLARE_STATIC_சாவி_FALSE(sched_energy_present);

நிலையான உள்வரி bool sched_energy_enabled(வெறுமை)
{
	திரும்பு static_branch_unlikely(&sched_energy_present);
}

#இல்லையெனில் /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL): */

#define perf_domain_span(pd) வெற்று

நிலையான உள்வரி bool sched_energy_enabled(வெறுமை) { திரும்பு false; }

#endif /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL) */

#ifdef CONFIG_MEMBARRIER

/*
 * The scheduler provides memory barriers required by membarrier between:
 * - prior user-space memory accesses and store to rq->membarrier_state,
 * - store to rq->membarrier_state and following user-space memory accesses.
 * In the same way it provides those guarantees around store to rq->curr.
 */
நிலையான உள்வரி வெறுமை membarrier_switch_mm(கட்டமைப்பு rq *rq,
					கட்டமைப்பு mm_struct *prev_mm,
					கட்டமைப்பு mm_struct *next_mm)
{
	முழு membarrier_state;

	எனில் (prev_mm == next_mm)
		திரும்பு;

	membarrier_state = atomic_read(&next_mm->membarrier_state);
	எனில் (READ_ONCE(rq->membarrier_state) == membarrier_state)
		திரும்பு;

	WRITE_ONCE(rq->membarrier_state, membarrier_state);
}

#இல்லையெனில் /* !CONFIG_MEMBARRIER: */

நிலையான உள்வரி வெறுமை membarrier_switch_mm(கட்டமைப்பு rq *rq,
					கட்டமைப்பு mm_struct *prev_mm,
					கட்டமைப்பு mm_struct *next_mm)
{
}

#endif /* !CONFIG_MEMBARRIER */

நிலையான உள்வரி bool is_per_cpu_kthread(கட்டமைப்பு task_struct *p)
{
	எனில் (!(p->flags & PF_KTHREAD))
		திரும்பு false;

	எனில் (p->nr_cpus_allowed != 1)
		திரும்பு false;

	திரும்பு true;
}

வெளி வெறுமை swake_up_all_locked(கட்டமைப்பு swait_queue_head *q);
வெளி வெறுமை __prepare_to_swait(கட்டமைப்பு swait_queue_head *q, கட்டமைப்பு swait_queue *wait);

வெளி முழு try_to_wake_up(கட்டமைப்பு task_struct *tsk, unsigned முழு state, முழு wake_flags);

#ifdef CONFIG_PREEMPT_DYNAMIC
வெளி முழு preempt_dynamic_mode;
வெளி முழு sched_dynamic_mode(மாறிலி எழுத்து *str);
வெளி வெறுமை sched_dynamic_update(முழு mode);
#endif
வெளி மாறிலி எழுத்து *preempt_modes[];

#ifdef CONFIG_SCHED_MM_CID

நிலையான __always_inline bool cid_on_cpu(unsigned முழு cid)
{
	திரும்பு cid & MM_CID_ONCPU;
}

நிலையான __always_inline bool cid_in_transit(unsigned முழு cid)
{
	திரும்பு cid & MM_CID_TRANSIT;
}

நிலையான __always_inline unsigned முழு cpu_cid_to_cid(unsigned முழு cid)
{
	திரும்பு cid & ~MM_CID_ONCPU;
}

நிலையான __always_inline unsigned முழு cid_to_cpu_cid(unsigned முழு cid)
{
	திரும்பு cid | MM_CID_ONCPU;
}

நிலையான __always_inline unsigned முழு cid_to_transit_cid(unsigned முழு cid)
{
	திரும்பு cid | MM_CID_TRANSIT;
}

நிலையான __always_inline unsigned முழு cid_from_transit_cid(unsigned முழு cid)
{
	திரும்பு cid & ~MM_CID_TRANSIT;
}

நிலையான __always_inline bool cid_on_task(unsigned முழு cid)
{
	/* True if none of the MM_CID_ONCPU, MM_CID_TRANSIT, MM_CID_UNSET bits is set */
	திரும்பு cid < MM_CID_TRANSIT;
}

நிலையான __always_inline வெறுமை mm_drop_cid(கட்டமைப்பு mm_struct *mm, unsigned முழு cid)
{
	clear_bit(cid, mm_cidmask(mm));
}

நிலையான __always_inline வெறுமை mm_unset_cid_on_task(கட்டமைப்பு task_struct *t)
{
	unsigned முழு cid = t->mm_cid.cid;

	t->mm_cid.cid = MM_CID_UNSET;
	எனில் (cid_on_task(cid))
		mm_drop_cid(t->mm, cid);
}

நிலையான __always_inline வெறுமை mm_drop_cid_on_cpu(கட்டமைப்பு mm_struct *mm, கட்டமைப்பு mm_cid_pcpu *pcp)
{
	/* Clear the ONCPU bit, but do not set UNSET in the per CPU storage */
	எனில் (cid_on_cpu(pcp->cid)) {
		pcp->cid = cpu_cid_to_cid(pcp->cid);
		mm_drop_cid(mm, pcp->cid);
	}
}

நிலையான உள்வரி unsigned முழு __mm_get_cid(கட்டமைப்பு mm_struct *mm, unsigned முழு max_cids)
{
	unsigned முழு cid = find_first_zero_bit(mm_cidmask(mm), max_cids);

	எனில் (cid >= max_cids)
		திரும்பு MM_CID_UNSET;
	எனில் (test_and_set_bit(cid, mm_cidmask(mm)))
		திரும்பு MM_CID_UNSET;
	திரும்பு cid;
}

நிலையான உள்வரி unsigned முழு mm_get_cid(கட்டமைப்பு mm_struct *mm)
{
	unsigned முழு cid = __mm_get_cid(mm, READ_ONCE(mm->mm_cid.max_cids));

	போது (cid == MM_CID_UNSET) {
		cpu_relax();
		cid = __mm_get_cid(mm, இலக்கம்_possible_cpus());
	}
	திரும்பு cid;
}

நிலையான உள்வரி unsigned முழு mm_cid_converge(கட்டமைப்பு mm_struct *mm, unsigned முழு orig_cid,
					   unsigned முழு max_cids)
{
	unsigned முழு new_cid, cid = cpu_cid_to_cid(orig_cid);

	/* Is it in the optimal CID space? */
	எனில் (likely(cid < max_cids))
		திரும்பு orig_cid;

	/* Try to find one in the optimal space. Otherwise keep the provided. */
	new_cid = __mm_get_cid(mm, max_cids);
	எனில் (new_cid != MM_CID_UNSET) {
		mm_drop_cid(mm, cid);
		/* Preserve the ONCPU mode of the original CID */
		திரும்பு new_cid | (orig_cid & MM_CID_ONCPU);
	}
	திரும்பு orig_cid;
}

நிலையான __always_inline வெறுமை mm_cid_update_task_cid(கட்டமைப்பு task_struct *t, unsigned முழு cid)
{
	எனில் (t->mm_cid.cid != cid) {
		t->mm_cid.cid = cid;
		rseq_sched_set_ids_changed(t);
	}
}

நிலையான __always_inline வெறுமை mm_cid_update_pcpu_cid(கட்டமைப்பு mm_struct *mm, unsigned முழு cid)
{
	__this_cpu_write(mm->mm_cid.pcpu->cid, cid);
}

நிலையான __always_inline வெறுமை mm_cid_from_cpu(கட்டமைப்பு task_struct *t, unsigned முழு cpu_cid,
					    unsigned முழு mode)
{
	unsigned முழு max_cids, tcid = t->mm_cid.cid;
	கட்டமைப்பு mm_struct *mm = t->mm;

	max_cids = READ_ONCE(mm->mm_cid.max_cids);
	/* Optimize for the common case where both have the ONCPU bit set */
	எனில் (likely(cid_on_cpu(cpu_cid & tcid))) {
		எனில் (likely(cpu_cid_to_cid(cpu_cid) < max_cids)) {
			mm_cid_update_task_cid(t, cpu_cid);
			திரும்பு;
		}
		/* Try to converge into the optimal CID space */
		cpu_cid = mm_cid_converge(mm, cpu_cid, max_cids);
	} இல்லையெனில் {
		/* Hand over or drop the task owned CID */
		எனில் (cid_on_task(tcid)) {
			எனில் (cid_on_cpu(cpu_cid))
				mm_unset_cid_on_task(t);
			இல்லையெனில்
				cpu_cid = cid_to_cpu_cid(tcid);
		}
		/* Still nothing, allocate a new one */
		எனில் (!cid_on_cpu(cpu_cid))
			cpu_cid = cid_to_cpu_cid(mm_get_cid(mm));

		/* Handle the transition mode flag if required */
		எனில் (mode & MM_CID_TRANSIT)
			cpu_cid = cpu_cid_to_cid(cpu_cid) | MM_CID_TRANSIT;
	}
	mm_cid_update_pcpu_cid(mm, cpu_cid);
	mm_cid_update_task_cid(t, cpu_cid);
}

நிலையான __always_inline வெறுமை mm_cid_from_task(கட்டமைப்பு task_struct *t, unsigned முழு cpu_cid,
					     unsigned முழு mode)
{
	unsigned முழு max_cids, tcid = t->mm_cid.cid;
	கட்டமைப்பு mm_struct *mm = t->mm;

	max_cids = READ_ONCE(mm->mm_cid.max_cids);
	/* Optimize for the common case, where both have the ONCPU bit clear */
	எனில் (likely(cid_on_task(tcid | cpu_cid))) {
		எனில் (likely(tcid < max_cids)) {
			mm_cid_update_pcpu_cid(mm, tcid);
			திரும்பு;
		}
		/* Try to converge into the optimal CID space */
		tcid = mm_cid_converge(mm, tcid, max_cids);
	} இல்லையெனில் {
		/* Hand over or drop the CPU owned CID */
		எனில் (cid_on_cpu(cpu_cid)) {
			எனில் (cid_on_task(tcid))
				mm_drop_cid_on_cpu(mm, this_cpu_ptr(mm->mm_cid.pcpu));
			இல்லையெனில்
				tcid = cpu_cid_to_cid(cpu_cid);
		}
		/* Still nothing, allocate a new one */
		எனில் (!cid_on_task(tcid))
			tcid = mm_get_cid(mm);
		/* Set the transition mode flag if required */
		tcid |= mode & MM_CID_TRANSIT;
	}
	mm_cid_update_pcpu_cid(mm, tcid);
	mm_cid_update_task_cid(t, tcid);
}

நிலையான __always_inline வெறுமை mm_cid_schedin(கட்டமைப்பு task_struct *next)
{
	கட்டமைப்பு mm_struct *mm = next->mm;
	unsigned முழு cpu_cid, mode;

	எனில் (!next->mm_cid.active)
		திரும்பு;

	cpu_cid = __this_cpu_read(mm->mm_cid.pcpu->cid);
	mode = READ_ONCE(mm->mm_cid.mode);
	எனில் (likely(!cid_on_cpu(mode)))
		mm_cid_from_task(next, cpu_cid, mode);
	இல்லையெனில்
		mm_cid_from_cpu(next, cpu_cid, mode);
}

நிலையான __always_inline வெறுமை mm_cid_schedout(கட்டமைப்பு task_struct *prev)
{
	கட்டமைப்பு mm_struct *mm = prev->mm;
	unsigned முழு mode, cid;

	/* During mode transitions CIDs are temporary and need to be dropped */
	எனில் (likely(!cid_in_transit(prev->mm_cid.cid)))
		திரும்பு;

	mode = READ_ONCE(mm->mm_cid.mode);
	cid = cid_from_transit_cid(prev->mm_cid.cid);

	/*
	 * If transition mode is done, transfer ownership when the CID is
	 * within the convergence range to optimize the next schedule in.
	 */
	எனில் (!cid_in_transit(mode) && cid < READ_ONCE(mm->mm_cid.max_cids)) {
		எனில் (cid_on_cpu(mode))
			cid = cid_to_cpu_cid(cid);

		/* Update both so that the next schedule in goes into the fast path */
		mm_cid_update_pcpu_cid(mm, cid);
		prev->mm_cid.cid = cid;
	} இல்லையெனில் {
		mm_drop_cid(mm, cid);
		prev->mm_cid.cid = MM_CID_UNSET;
	}
}

நிலையான உள்வரி வெறுமை mm_cid_switch_to(கட்டமைப்பு task_struct *prev, கட்டமைப்பு task_struct *next)
{
	mm_cid_schedout(prev);
	mm_cid_schedin(next);
}

#இல்லையெனில் /* !CONFIG_SCHED_MM_CID: */
நிலையான உள்வரி வெறுமை mm_cid_switch_to(கட்டமைப்பு task_struct *prev, கட்டமைப்பு task_struct *next) { }
#endif /* !CONFIG_SCHED_MM_CID */

#ifdef CONFIG_SCHED_CACHE
DECLARE_STATIC_சாவி_FALSE(sched_cache_present);
DECLARE_STATIC_சாவி_FALSE(sched_cache_active);
வெளி முழு sysctl_sched_cache_user;
வெளி unsigned முழு llc_aggr_tolerance;
வெளி unsigned முழு llc_epoch_period;
வெளி unsigned முழு llc_epoch_affinity_timeout;
வெளி unsigned முழு llc_imb_pct;
வெளி unsigned முழு llc_overaggr_pct;

நிலையான உள்வரி bool sched_cache_enabled(வெறுமை)
{
	திரும்பு static_branch_unlikely(&sched_cache_active);
}

வெளி வெறுமை sched_cache_active_set(வெறுமை);

#endif

வெறுமை sched_domains_free_llc_id(முழு cpu);

வெளி வெறுமை init_sched_mm(கட்டமைப்பு task_struct *p);

வெளி u64 avg_vruntime(கட்டமைப்பு cfs_rq *cfs_rq);
வெளி முழு entity_eligible(கட்டமைப்பு cfs_rq *cfs_rq, கட்டமைப்பு sched_entity *se);
நிலையான உள்வரி
வெறுமை move_queued_task_locked(கட்டமைப்பு rq *src_rq, கட்டமைப்பு rq *dst_rq, கட்டமைப்பு task_struct *task)
{
	lockdep_assert_rq_held(src_rq);
	lockdep_assert_rq_held(dst_rq);

	deactivate_task(src_rq, task, 0);
	set_task_cpu(task, dst_rq->cpu);
	activate_task(dst_rq, task, 0);
	wakeup_preempt(dst_rq, task, 0);
}

நிலையான உள்வரி
bool task_is_pushable(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p, முழு cpu)
{
	எனில் (!task_on_cpu(rq, p) &&
	    cpumask_test_cpu(cpu, &p->cpus_mask))
		திரும்பு true;

	திரும்பு false;
}

#ifdef CONFIG_RT_MUTEXES

நிலையான உள்வரி முழு __rt_effective_prio(கட்டமைப்பு task_struct *pi_task, முழு prio)
{
	எனில் (pi_task)
		prio = min(prio, pi_task->prio);

	திரும்பு prio;
}

நிலையான உள்வரி முழு rt_effective_prio(கட்டமைப்பு task_struct *p, முழு prio)
{
	கட்டமைப்பு task_struct *pi_task = rt_mutex_get_உச்சி_task(p);

	திரும்பு __rt_effective_prio(pi_task, prio);
}

#இல்லையெனில் /* !CONFIG_RT_MUTEXES: */

நிலையான உள்வரி முழு rt_effective_prio(கட்டமைப்பு task_struct *p, முழு prio)
{
	திரும்பு prio;
}

#endif /* !CONFIG_RT_MUTEXES */

வெளி முழு __sched_setscheduler(கட்டமைப்பு task_struct *p, மாறிலி கட்டமைப்பு sched_attr *attr, bool user, bool pi);
வெளி முழு __sched_setaffinity(கட்டமைப்பு task_struct *p, கட்டமைப்பு affinity_context *ctx);
வெளி மாறிலி கட்டமைப்பு sched_class *__setscheduler_class(முழு policy, முழு prio);
வெளி வெறுமை set_load_weight(கட்டமைப்பு task_struct *p, bool update_load);
வெளி வெறுமை enqueue_task(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p, முழு flags);
வெளி bool dequeue_task(கட்டமைப்பு rq *rq, கட்டமைப்பு task_struct *p, முழு flags);

வெளி கட்டமைப்பு balance_callback *splice_balance_callbacks(கட்டமைப்பு rq *rq);

வெளி வெறுமை __balance_callbacks(கட்டமைப்பு rq *rq, கட்டமைப்பு rq_flags *rf);
வெளி வெறுமை balance_callbacks(கட்டமைப்பு rq *rq, கட்டமைப்பு balance_callback *head);

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
கட்டமைப்பு sched_change_ctx {
	u64			prio;
	கட்டமைப்பு task_struct	*p;
	மாறிலி கட்டமைப்பு sched_class *class;
	முழு			flags;
	bool			queued;
	bool			running;
};

கட்டமைப்பு sched_change_ctx *sched_change_begin(கட்டமைப்பு task_struct *p, unsigned முழு flags);
வெறுமை sched_change_end(கட்டமைப்பு sched_change_ctx *ctx);

DEFINE_CLASS(sched_change, கட்டமைப்பு sched_change_ctx *,
	     sched_change_end(_T),
	     sched_change_begin(p, flags),
	     கட்டமைப்பு task_struct *p, unsigned முழு flags)

DEFINE_CLASS_IS_UNCONDITIONAL(sched_change)

#அந்தர்பாவ "ext/ext.h"

#ifdef CONFIG_PREFERRED_CPU
வெறுமை sched_தள்ளு_current_non_preferred_cpu(கட்டமைப்பு rq *rq);
#இல்லையெனில்	/* !CONFIG_PREFERRED_CPU */
நிலையான உள்வரி வெறுமை sched_தள்ளு_current_non_preferred_cpu(கட்டமைப்பு rq *rq) { }
#endif

#endif /* _KERNEL_SCHED_SCHED_H */
