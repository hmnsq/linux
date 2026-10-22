/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Scheduler internal types and methods:
 */
#ifndef _KERNEL_SCHED_SCHED_H
#define _KERNEL_SCHED_SCHED_H

#شامل <linux/prandom.h>
#شامل <linux/sched/affinity.h>
#شامل <linux/sched/autogroup.h>
#شامل <linux/sched/cpufreq.h>
#شامل <linux/sched/deadline.h>
#شامل <linux/sched.h>
#شامل <linux/sched/loadavg.h>
#شامل <linux/sched/mm.h>
#شامل <linux/sched/rseq_api.h>
#شامل <linux/sched/signal.h>
#شامل <linux/sched/smt.h>
#شامل <linux/sched/stat.h>
#شامل <linux/sched/sysctl.h>
#شامل <linux/sched/task_flags.h>
#شامل <linux/sched/task.h>
#شامل <linux/sched/topology.h>
#شامل <linux/atomic.h>
#شامل <linux/bitmap.h>
#شامل <linux/bug.h>
#شامل <linux/capability.h>
#شامل <linux/cgroup_api.h>
#شامل <linux/cgroup.h>
#شامل <linux/context_tracking.h>
#شامل <linux/cpufreq.h>
#شامل <linux/cpumask_api.h>
#شامل <linux/cpuset.h>
#شامل <linux/ctype.h>
#شامل <linux/file.h>
#شامل <linux/fs_api.h>
#شامل <linux/hrtimer_api.h>
#شامل <linux/interrupt.h>
#شامل <linux/irq_work.h>
#شامل <linux/jiffies.h>
#شامل <linux/kref_api.h>
#شامل <linux/kthread.h>
#شامل <linux/ktime_api.h>
#شامل <linux/lockdep_api.h>
#شامل <linux/lockdep.h>
#شامل <linux/memblock.h>
#شامل <linux/memcontrol.h>
#شامل <linux/minmax.h>
#شامل <linux/mm.h>
#شامل <linux/module.h>
#شامل <linux/mutex_api.h>
#شامل <linux/plist.h>
#شامل <linux/poll.h>
#شامل <linux/proc_fs.h>
#شامل <linux/profile.h>
#شامل <linux/psi.h>
#شامل <linux/rcupdate.h>
#شامل <linux/seq_file.h>
#شامل <linux/seqlock.h>
#شامل <linux/softirq.h>
#شامل <linux/spinlock_api.h>
#شامل <linux/static_key.h>
#شامل <linux/stop_machine.h>
#شامل <linux/syscalls_api.h>
#شامل <linux/syscalls.h>
#شامل <linux/tick.h>
#شامل <linux/topology.h>
#شامل <linux/types.h>
#شامل <linux/u64_stats_sync_api.h>
#شامل <linux/uaccess.h>
#شامل <linux/vmstat.h>
#شامل <linux/wait_api.h>
#شامل <linux/wait_bit.h>
#شامل <linux/workqueue_api.h>
#شامل <linux/delayacct.h>
#شامل <linux/mmu_context.h>

#شامل <trace/events/power.h>
#شامل <trace/events/sched.h>

#شامل "../workqueue_internal.h"

ساختار rq;
ساختار cfs_rq;
ساختار rt_rq;
ساختار sched_group;
ساختار cpuidle_state;

#اگر defined(CONFIG_PARAVIRT) && !defined(CONFIG_HAVE_PV_STEAL_CLOCK_GEN)
# include <asm/paravirt.h>
#endif

#شامل <asm/barrier.h>

#شامل "cpupri.h"
#شامل "cpudeadline.h"

/* task_struct::on_rq states: */
#define TASK_ON_RQ_QUEUED	1
#define TASK_ON_RQ_MIGRATING	2

بیرونی __read_mostly صحیح scheduler_running;

بیرونی unsigned long calc_load_update;
بیرونی atomic_long_t calc_load_tasks;

بیرونی تهی calc_global_load_tick(ساختار rq *this_rq);
بیرونی long calc_load_fold_active(ساختار rq *this_rq, long adjust);

بیرونی تهی call_trace_sched_update_nr_running(ساختار rq *rq, صحیح شمار);

بیرونی صحیح sysctl_sched_rt_period;
بیرونی صحیح sysctl_sched_rt_runtime;
بیرونی صحیح sched_rr_timeslice;

/*
 * Asymmetric CPU capacity bits
 */
ساختار asym_cap_data {
	ساختار list_head link;
	ساختار rcu_head rcu;
	unsigned long capacity;
	unsigned long cpus[];
};

بیرونی ساختار list_head asym_cap_list;

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
	اگر (__w)						\
		__w = max(2UL, __w >> SCHED_FIXEDPOINT_SHIFT);	\
	__w;							\
})
#وگرنه
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

ایستا درونخطی صحیح idle_policy(صحیح policy)
{
	بازگشت policy == SCHED_IDLE;
}

ایستا درونخطی صحیح normal_policy(صحیح policy)
{
#ifdef CONFIG_SCHED_CLASS_EXT
	اگر (policy == SCHED_EXT)
		بازگشت true;
#endif
	بازگشت policy == SCHED_NORMAL;
}

ایستا درونخطی صحیح fair_policy(صحیح policy)
{
	بازگشت normal_policy(policy) || policy == SCHED_BATCH;
}

ایستا درونخطی صحیح rt_policy(صحیح policy)
{
	بازگشت policy == SCHED_FIFO || policy == SCHED_RR;
}

ایستا درونخطی صحیح dl_policy(صحیح policy)
{
	بازگشت policy == SCHED_DEADLINE;
}

ایستا درونخطی bool valid_policy(صحیح policy)
{
	بازگشت idle_policy(policy) || fair_policy(policy) ||
		rt_policy(policy) || dl_policy(policy);
}

ایستا درونخطی صحیح task_has_idle_policy(ساختار task_struct *p)
{
	بازگشت idle_policy(p->policy);
}

ایستا درونخطی صحیح task_has_rt_policy(ساختار task_struct *p)
{
	بازگشت rt_policy(p->policy);
}

ایستا درونخطی صحیح task_has_dl_policy(ساختار task_struct *p)
{
	بازگشت dl_policy(p->policy);
}

#define cap_scale(v, s)		((v)*(s) >> SCHED_CAPACITY_SHIFT)

ایستا درونخطی تهی update_avg(u64 *avg, u64 sample)
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
ایستا درونخطی unsigned long sched_weight_from_cgroup(unsigned long cgrp_weight)
{
	بازگشت DIV_ROUND_CLOSEST_ULL(cgrp_weight * 1024, CGROUP_WEIGHT_DFL);
}

ایستا درونخطی unsigned long sched_weight_to_cgroup(unsigned long weight)
{
	بازگشت clamp_t(unsigned long,
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

ایستا درونخطی bool dl_entity_is_special(ثابت ساختار sched_dl_entity *dl_se)
{
#ifdef CONFIG_CPU_FREQ_GOV_SCHEDUTIL
	بازگشت unlikely(dl_se->flags & SCHED_FLAG_SUGOV);
#وگرنه
	بازگشت false;
#endif
}

/*
 * Tells if entity @a should preempt entity @b.
 */
ایستا درونخطی bool dl_entity_preempt(ثابت ساختار sched_dl_entity *a,
				     ثابت ساختار sched_dl_entity *b)
{
	بازگشت dl_entity_is_special(a) ||
	       dl_time_before(a->deadline, b->deadline);
}

/*
 * This is the priority-queue data structure of the RT scheduling class:
 */
ساختار rt_prio_array {
	DECLARE_BITMAP(bitmap, MAX_RT_PRIO+1); /* include 1 bit for delimiter */
	ساختار list_head queue[MAX_RT_PRIO];
};

ساختار rt_bandwidth {
	/* nests inside the rq lock: */
	raw_spinlock_t		rt_runtime_lock;
	ktime_t			rt_period;
	u64			rt_runtime;
	ساختار hrtimer		rt_period_timer;
	unsigned صحیح		rt_period_active;
};

ایستا درونخطی صحیح dl_bandwidth_enabled(تهی)
{
	بازگشت sysctl_sched_rt_runtime >= 0;
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
ساختار dl_bw {
	raw_spinlock_t		lock;
	u64			bw;
	u64			total_bw;
};

بیرونی تهی init_dl_bw(ساختار dl_bw *dl_b);
بیرونی صحیح  sched_dl_global_validate(تهی);
بیرونی تهی sched_dl_do_global(تهی);
بیرونی صحیح  sched_dl_overflow(ساختار task_struct *p, صحیح policy, ثابت ساختار sched_attr *attr);
بیرونی تهی __setparam_dl(ساختار task_struct *p, ثابت ساختار sched_attr *attr);
بیرونی تهی __getparam_dl(ساختار task_struct *p, ساختار sched_attr *attr, unsigned صحیح flags);
بیرونی bool __checkparam_dl(ثابت ساختار sched_attr *attr);
بیرونی bool dl_param_changed(ساختار task_struct *p, ثابت ساختار sched_attr *attr);
بیرونی صحیح  dl_cpuset_cpumask_can_shrink(ثابت ساختار cpumask *cur, ثابت ساختار cpumask *trial);
بیرونی صحیح  dl_bw_deactivate(صحیح cpu);
بیرونی s64 dl_scaled_delta_exec(ساختار rq *rq, ساختار sched_dl_entity *dl_se, s64 delta_exec);
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
بیرونی تهی dl_server_update_idle(ساختار sched_dl_entity *dl_se, s64 delta_exec);
بیرونی تهی dl_server_update(ساختار sched_dl_entity *dl_se, s64 delta_exec);
بیرونی تهی dl_server_start(ساختار sched_dl_entity *dl_se);
بیرونی تهی dl_server_stop(ساختار sched_dl_entity *dl_se);
بیرونی تهی dl_server_init(ساختار sched_dl_entity *dl_se, ساختار rq *rq,
		    dl_server_pick_f pick_task);
بیرونی تهی sched_init_dl_servers(تهی);

بیرونی تهی fair_server_init(ساختار rq *rq);
بیرونی تهی ext_server_init(ساختار rq *rq);
بیرونی تهی __dl_server_attach_root(ساختار sched_dl_entity *dl_se, ساختار rq *rq);
بیرونی صحیح dl_server_apply_params(ساختار sched_dl_entity *dl_se,
		    u64 runtime, u64 period, bool init);
بیرونی صحیح dl_server_attach_bw(ساختار sched_dl_entity *dl_se);
بیرونی تهی dl_server_detach_bw(ساختار sched_dl_entity *dl_se);
بیرونی صحیح dl_server_swap_bw(ساختار sched_dl_entity *detach_se,
			     ساختار sched_dl_entity *attach_se);

ایستا درونخطی bool dl_server_active(ساختار sched_dl_entity *dl_se)
{
	بازگشت dl_se->dl_server_active;
}

#ifdef CONFIG_CGROUP_SCHED

بیرونی ساختار list_head task_groups;

#ifdef CONFIG_GROUP_SCHED_BANDWIDTH
بیرونی ثابت u64 max_bw_quota_period_us;

/*
 * default period for group bandwidth.
 * default: 0.1s, units: microseconds
 */
ایستا درونخطی u64 default_bw_period_us(تهی)
{
	بازگشت 100000ULL;
}
#endif /* CONFIG_GROUP_SCHED_BANDWIDTH */

ساختار cfs_bandwidth {
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
	ساختار hrtimer		period_timer;
	ساختار hrtimer		slack_timer;
	ساختار list_head	throttled_cfs_rq;

	/* Statistics: */
	صحیح			nr_periods;
	صحیح			nr_throttled;
	صحیح			nr_burst;
	u64			throttled_time;
	u64			burst_time;
#endif /* CONFIG_CFS_BANDWIDTH */
};

/* Task group related information */
ساختار task_group {
	ساختار cgroup_subsys_state css;

#ifdef CONFIG_GROUP_SCHED_WEIGHT
	/* A positive value indicates that this is a SCHED_IDLE group. */
	صحیح			idle;
#endif

#ifdef CONFIG_FAIR_GROUP_SCHED
	/* runqueue "owned" by this group on each CPU */
	ساختار cfs_rq __percpu	*cfs_rq;
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
	ساختار sched_rt_entity	**rt_se;
	ساختار rt_rq		**rt_rq;

	ساختار rt_bandwidth	rt_bandwidth;
#endif

	ساختار scx_task_group	scx;

	ساختار rcu_head		rcu;
	ساختار list_head	list;

	ساختار task_group	*parent;
	ساختار list_head	siblings;
	ساختار list_head	children;

#ifdef CONFIG_SCHED_AUTOGROUP
	ساختار autogroup	*autogroup;
#endif

	ساختار cfs_bandwidth	cfs_bandwidth;

#ifdef CONFIG_UCLAMP_TASK_GROUP
	/* The two decimal precision [%] value requested from user-space */
	unsigned صحیح		uclamp_pct[UCLAMP_CNT];
	/* Clamp values requested for a task group */
	ساختار uclamp_se	uclamp_req[UCLAMP_CNT];
	/* Effective clamp values used for a task group */
	ساختار uclamp_se	uclamp[UCLAMP_CNT];
#endif

};

#define ریشه_TASK_GROUP_LOAD	NICE_0_LOAD

تعریفنوع صحیح (*tg_visitor)(ساختار task_group *, تهی *);

بیرونی صحیح walk_tg_درخت_from(ساختار task_group *from,
			     tg_visitor down, tg_visitor up, تهی *data);

/*
 * Iterate the full tree, calling @down when first entering a node and @up when
 * leaving it for the final time.
 *
 * Caller must hold rcu_lock or sufficient equivalent.
 */
ایستا درونخطی صحیح walk_tg_درخت(tg_visitor down, tg_visitor up, تهی *data)
{
	بازگشت walk_tg_درخت_from(&ریشه_task_group, down, up, data);
}

ایستا درونخطی ساختار task_group *css_tg(ساختار cgroup_subsys_state *css)
{
	بازگشت css ? container_of(css, ساختار task_group, css) : پوچ;
}

بیرونی صحیح tg_nop(ساختار task_group *tg, تهی *data);

#ifdef CONFIG_FAIR_GROUP_SCHED
بیرونی تهی free_fair_sched_group(ساختار task_group *tg);
بیرونی صحیح alloc_fair_sched_group(ساختار task_group *tg, ساختار task_group *parent);
بیرونی تهی online_fair_sched_group(ساختار task_group *tg);
بیرونی تهی unregister_fair_sched_group(ساختار task_group *tg);
بیرونی تهی __sched_cgroup_mode_update(صحیح mode);
#وگرنه /* !CONFIG_FAIR_GROUP_SCHED: */
ایستا درونخطی تهی free_fair_sched_group(ساختار task_group *tg) { }
ایستا درونخطی صحیح alloc_fair_sched_group(ساختار task_group *tg, ساختار task_group *parent)
{
       بازگشت 1;
}
ایستا درونخطی تهی online_fair_sched_group(ساختار task_group *tg) { }
ایستا درونخطی تهی unregister_fair_sched_group(ساختار task_group *tg) { }
#endif /* !CONFIG_FAIR_GROUP_SCHED */

بیرونی تهی init_tg_cfs_entry(ساختار task_group *tg, ساختار cfs_rq *cfs_rq,
			ساختار sched_entity *se, صحیح cpu,
			ساختار sched_entity *parent);
بیرونی تهی init_cfs_bandwidth(ساختار cfs_bandwidth *cfs_b, ساختار cfs_bandwidth *parent);

بیرونی تهی __refill_cfs_bandwidth_runtime(ساختار cfs_bandwidth *cfs_b);
بیرونی تهی start_cfs_bandwidth(ساختار cfs_bandwidth *cfs_b);
بیرونی تهی unthrottle_cfs_rq(ساختار cfs_rq *cfs_rq);
بیرونی bool cfs_task_bw_constrained(ساختار task_struct *p);

بیرونی تهی init_tg_rt_entry(ساختار task_group *tg, ساختار rt_rq *rt_rq,
		ساختار sched_rt_entity *rt_se, صحیح cpu,
		ساختار sched_rt_entity *parent);
بیرونی صحیح sched_group_set_rt_runtime(ساختار task_group *tg, long rt_runtime_us);
بیرونی صحیح sched_group_set_rt_period(ساختار task_group *tg, u64 rt_period_us);
بیرونی long sched_group_rt_runtime(ساختار task_group *tg);
بیرونی long sched_group_rt_period(ساختار task_group *tg);
بیرونی صحیح sched_rt_can_attach(ساختار task_group *tg, ساختار task_struct *tsk);

بیرونی ساختار task_group *sched_create_group(ساختار task_group *parent);
بیرونی تهی sched_online_group(ساختار task_group *tg,
			       ساختار task_group *parent);
بیرونی تهی sched_destroy_group(ساختار task_group *tg);
بیرونی تهی sched_release_group(ساختار task_group *tg);

بیرونی تهی sched_move_task(ساختار task_struct *tsk, bool for_autogroup);

#ifdef CONFIG_FAIR_GROUP_SCHED
بیرونی صحیح sched_group_set_shares(ساختار task_group *tg, unsigned long shares);

بیرونی صحیح sched_group_set_idle(ساختار task_group *tg, long idle);

بیرونی تهی set_task_rq_fair(ساختار sched_entity *se,
			     ساختار cfs_rq *prev, ساختار cfs_rq *next);
#وگرنه /* !CONFIG_FAIR_GROUP_SCHED: */
ایستا درونخطی صحیح sched_group_set_shares(ساختار task_group *tg, unsigned long shares) { بازگشت 0; }
ایستا درونخطی صحیح sched_group_set_idle(ساختار task_group *tg, long idle) { بازگشت 0; }
#endif /* !CONFIG_FAIR_GROUP_SCHED */

#وگرنه /* !CONFIG_CGROUP_SCHED: */

ساختار cfs_bandwidth { };

ایستا درونخطی bool cfs_task_bw_constrained(ساختار task_struct *p) { بازگشت false; }

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

بیرونی تهی unregister_rt_sched_group(ساختار task_group *tg);
بیرونی تهی free_rt_sched_group(ساختار task_group *tg);
بیرونی صحیح alloc_rt_sched_group(ساختار task_group *tg, ساختار task_group *parent);

/*
 * u64_u32_load/u64_u32_store
 *
 * Use a copy of a u64 value to protect against data race. This is only
 * applicable for 32-bits architectures.
 */
#ifdef CONFIG_64BIT
# define u64_u32_load_copy(var, copy)		var
# define u64_u32_store_copy(var, copy, val)	(var = val)
#وگرنه
# define u64_u32_load_copy(var, copy)					\
({									\
	u64 __val, __val_copy;						\
	انجامبده {								\
		__val_copy = copy;					\
		/*							\
		 * paired with u64_u32_store_copy(), ordering access	\
		 * to var and copy.					\
		 */							\
		smp_rmb();						\
		__val = var;						\
	} وقتی (__val != __val_copy);					\
	__val;								\
})
# define u64_u32_store_copy(var, copy, val)				\
انجامبده {									\
	typeof(val) __val = (val);					\
	var = __val;							\
	/*								\
	 * paired with u64_u32_load_copy(), ordering access to var and	\
	 * copy.							\
	 */								\
	smp_wmb();							\
	copy = __val;							\
} وقتی (0)
#endif
# define u64_u32_load(var)		u64_u32_load_copy(var, var##_copy)
# define u64_u32_store(var, val)	u64_u32_store_copy(var, var##_copy, val)

ساختار balance_callback {
	ساختار balance_callback *next;
	تهی (*func)(ساختار rq *rq);
};

/* Fair scheduling SCHED_{NORMAL,BATCH,IDLE} related fields in a runqueue: */
ساختار cfs_rq {
	ساختار load_weight	load;
	unsigned صحیح		nr_queued;
	unsigned صحیح		h_nr_queued;		/* SCHED_{NORMAL,BATCH,IDLE} */
	unsigned صحیح		h_nr_runnable;		/* SCHED_{NORMAL,BATCH,IDLE} */
	unsigned صحیح		h_nr_idle;		/* SCHED_IDLE */

	s64			sum_w_vruntime;
	u64			sum_weight;
	u64			zero_vruntime;
	unsigned صحیح		sum_shift;

#ifdef CONFIG_SCHED_CORE
	unsigned صحیح		forceidle_seq;
	u64			zero_vruntime_fi;
#endif

	ساختار rb_ریشه_cached	tasks_timeline;

	/*
	 * 'curr' points to the currently running entity on this cfs_rq.
	 * It is set to NULL otherwise (i.e when none are currently running).
	 */
	ساختار sched_entity	*curr;
	ساختار sched_entity	*next;

	/*
	 * CFS load tracking
	 */
	ساختار sched_entity	*h_curr;
	ساختار sched_avg	avg;
#ifndef CONFIG_64BIT
	u64			last_update_time_copy;
#endif
	ساختار {
		raw_spinlock_t	lock ____cacheline_aligned;
		صحیح		nr;
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
	ساختار sched_entity	*h_load_next;

	ساختار rq		*rq;	/* CPU runqueue to which this cfs_rq is attached */

	/*
	 * leaf cfs_rqs are those that hold tasks (lowest schedulable entity in
	 * a hierarchy). Non-leaf lrqs hold other higher schedulable entities
	 * (like users, containers etc.)
	 *
	 * leaf_cfs_rq_list ties together list of leaf cfs_rq's in a CPU.
	 * This list is used during load balance.
	 */
	صحیح			on_list;
	ساختار list_head	leaf_cfs_rq_list;
	ساختار task_group	*tg;	/* Group that "owns" this runqueue */

	/* Locally cached copy of our task_group's idle value */
	صحیح			idle;

# ifdef CONFIG_CFS_BANDWIDTH
	صحیح			runtime_enabled;
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
	صحیح			throttle_شمار;
	ساختار list_head	throttled_list;
	ساختار list_head	throttled_csd_list;
	ساختار list_head        throttled_limbo_list;
# endif /* CONFIG_CFS_BANDWIDTH */
#endif /* CONFIG_FAIR_GROUP_SCHED */
};

#ifdef CONFIG_SCHED_CLASS_EXT
/* scx_rq->flags, protected by the rq lock */
شمارش scx_rq_flags {
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
	SCX_RQ_ریشه_IDLE_RENOTIFY	= 1 << 8, /* the root is owed update_idle() */

	SCX_RQ_IN_WAKEUP	= 1 << 16,
	SCX_RQ_IN_DISPATCH	= 1 << 17,
};

/* per-rq rescue execution state, see scx_rescue_timerfn() */
ساختار scx_rq_rescue {
	ساختار scx_dispatch_q	dsq;			/* stranded tasks awaiting rescue */
	s64			budget;			/* execution token bucket, ns */
	u64			clock;			/* last budget accrual timestamp */
	ساختار task_struct	*curr;			/* task being rescued, one at a time */
	s64			slice;			/* curr's admitted slice */
	u64			exec_snap;		/* sum_exec_runtime at admission */
	ساختار timer_list	timer;			/* paces admission and escalation */
	u64			kill_at;		/* last ejection, init before any */
};

ساختار scx_rq {
	ساختار scx_dispatch_q	local_dsq;
#ifdef CONFIG_EXT_SUB_SCHED
	ساختار scx_dispatch_q	reject_dsq;		/* staging for cap-rejected tasks */
	ساختار scx_rq_rescue	rescue;
#endif
	ساختار list_head	runnable_list;		/* runnable tasks on this rq */
	ساختار list_head	ddsp_deferred_locals;	/* deferred ddsps from enq */
	/* both stashed across the activate_task() in move_remote_task_to_local_dsq() */
	u64			remote_activate_enq_flags;
	ساختار scx_sched	*remote_activate_sch;
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
	ساختار llist_head	ecaps_to_sync;		/* pending ecaps syncs */
	ساختار task_struct	*sub_dispatch_prev;
#endif
	cpumask_var_t		cpus_to_sync;
	bool			kick_sync_pending;
	unsigned long		kick_sync;

	ساختار list_head	sched_pcpus_to_kick;	/* see kick_cpus_irq_workfn() */

	raw_spinlock_t		deferred_reenq_lock;
	ساختار list_head	deferred_reenq_locals;	/* scheds requesting reenq of local DSQ */
	ساختار list_head	deferred_reenq_users;	/* user DSQs requesting reenq */
	ساختار balance_callback	deferred_bal_cb;
	ساختار balance_callback	kick_sync_bal_cb;
	ساختار irq_work		deferred_irq_work;
	ساختار irq_work		kick_cpus_irq_work;
};
#endif /* CONFIG_SCHED_CLASS_EXT */

ایستا درونخطی صحیح rt_bandwidth_enabled(تهی)
{
	بازگشت sysctl_sched_rt_runtime >= 0;
}

/* RT IPI pull logic requires IRQ_WORK */
#اگر defined(CONFIG_IRQ_WORK) && defined(CONFIG_SMP)
# define HAVE_RT_افزودن_IPI
#endif

/* Real-Time classes' related field in a runqueue: */
ساختار rt_rq {
	ساختار rt_prio_array	active;
	unsigned صحیح		rt_nr_running;
	unsigned صحیح		rr_nr_running;
	ساختار {
		صحیح		curr; /* highest queued rt task prio */
		صحیح		next; /* next highest */
	} highest_prio;
	bool			overloaded;
	ساختار plist_head	pushable_tasks;

	صحیح			rt_queued;

#ifdef CONFIG_RT_GROUP_SCHED
	صحیح			rt_throttled;
	u64			rt_time; /* consumed RT time, goes up in update_curr_rt */
	u64			rt_runtime; /* allotted RT time, "slice" from rt_bandwidth, RT sharing/balancing */
	/* Nests inside the rq lock: */
	raw_spinlock_t		rt_runtime_lock;

	unsigned صحیح		rt_nr_boosted;

	ساختار rq		*rq; /* this is always top-level rq, cache? */
#endif
#ifdef CONFIG_CGROUP_SCHED
	ساختار task_group	*tg; /* this tg has "this" rt_rq on given CPU for runnable entities */
#endif
};

ایستا درونخطی bool rt_rq_is_runnable(ساختار rt_rq *rt_rq)
{
	بازگشت rt_rq->rt_queued && rt_rq->rt_nr_running;
}

/* Deadline class' related fields in a runqueue */
ساختار dl_rq {
	/* runqueue is an rbtree, ordered by deadline */
	ساختار rb_ریشه_cached	ریشه;

	unsigned صحیح		dl_nr_running;

	/*
	 * Deadline values of the currently executing and the
	 * earliest ready task on this rq. Caching these facilitates
	 * the decision whether or not a ready but not running task
	 * should migrate somewhere else.
	 */
	ساختار {
		u64		curr;
		u64		next;
	} earliest_dl;

	bool			overloaded;

	ساختار sched_dl_entity	*curr;
	/*
	 * Tasks on this rq that can be pushed away. They are kept in
	 * an rb-tree, ordered by tasks' deadlines, with caching
	 * of the leftmost (earliest deadline) element.
	 */
	ساختار rb_ریشه_cached	pushable_dl_tasks_ریشه;

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
#define is_ریشه_task_group(tg) ((tg) == &ریشه_task_group)
/* An entity is a task if it doesn't "own" a runqueue */
#define entity_is_task(se)	(!se->my_q)

ایستا درونخطی تهی se_update_runnable(ساختار sched_entity *se)
{
	اگر (!entity_is_task(se))
		se->runnable_weight = se->my_q->h_nr_runnable;
}

ایستا درونخطی long se_runnable(ساختار sched_entity *se)
{
	اگر (se->sched_delayed)
		بازگشت false;

	اگر (entity_is_task(se))
		بازگشت !!se->on_rq;
	وگرنه
		بازگشت se->runnable_weight;
}

#وگرنه /* !CONFIG_FAIR_GROUP_SCHED: */

#define entity_is_task(se)	1

ایستا درونخطی تهی se_update_runnable(ساختار sched_entity *se) { }

ایستا درونخطی long se_runnable(ساختار sched_entity *se)
{
	اگر (se->sched_delayed)
		بازگشت false;

	بازگشت !!se->on_rq;
}

#endif /* !CONFIG_FAIR_GROUP_SCHED */

/*
 * XXX we want to get rid of these helpers and use the full load resolution.
 */
ایستا درونخطی long se_weight(ساختار sched_entity *se)
{
	بازگشت scale_load_down(se->load.weight);
}


ایستا درونخطی bool sched_asym_prefer(صحیح a, صحیح b)
{
	بازگشت arch_asym_cpu_priority(a) > arch_asym_cpu_priority(b);
}

ساختار perf_domain {
	ساختار em_perf_domain *em_pd;
	ساختار perf_domain *next;
	ساختار rcu_head rcu;
};

/*
 * We add the notion of a root-domain which will be used to define per-domain
 * variables. Each exclusive cpuset essentially defines an island domain by
 * fully partitioning the member CPUs from any other cpuset. Whenever a new
 * exclusive cpuset is created, we also create and attach a new root-domain
 * object.
 *
 */
ساختار ریشه_domain {
	atomic_t		refcount;
	atomic_t		rto_شمار;
	ساختار rcu_head		rcu;
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
	atomic_t		dlo_شمار;
	ساختار dl_bw		dl_bw;
	ساختار cpudl		cpudl;

	/*
	 * Indicate whether a root_domain's dl_bw has been checked or
	 * updated. It's monotonously increasing value.
	 *
	 * Also, some corner cases, like 'wrap around' is dangerous, but given
	 * that u64 is 'big enough'. So that shouldn't be a concern.
	 */
	u64 visit_cookie;

#ifdef HAVE_RT_افزودن_IPI
	/*
	 * For IPI pull requests, loop across the rto_mask.
	 */
	ساختار irq_work		rto_افزودن_work;
	raw_spinlock_t		rto_lock;
	/* These are only updated and read within rto_lock */
	صحیح			rto_loop;
	صحیح			rto_cpu;
	/* These atomics are updated outside of a lock */
	atomic_t		rto_loop_next;
	atomic_t		rto_loop_start;
#endif /* HAVE_RT_PUSH_IPI */
	/*
	 * The "RT overload" flag: it gets set if a CPU has more than
	 * one runnable RT task.
	 */
	cpumask_var_t		rto_mask;
	ساختار cpupri		cpupri;

	/*
	 * NULL-terminated list of performance domains intersecting with the
	 * CPUs of the rd. Protected by RCU.
	 */
	ساختار perf_domain __rcu *pd;
};

بیرونی تهی init_defrootdomain(تهی);
بیرونی صحیح sched_init_domains(ثابت ساختار cpumask *cpu_map);
بیرونی تهی rq_attach_ریشه(ساختار rq *rq, ساختار ریشه_domain *rd);
بیرونی تهی sched_get_rd(ساختار ریشه_domain *rd);
بیرونی تهی sched_put_rd(ساختار ریشه_domain *rd);

ایستا درونخطی صحیح get_rd_overloaded(ساختار ریشه_domain *rd)
{
	بازگشت READ_ONCE(rd->overloaded);
}

ایستا درونخطی تهی set_rd_overloaded(ساختار ریشه_domain *rd, صحیح status)
{
	اگر (get_rd_overloaded(rd) != status)
		WRITE_ONCE(rd->overloaded, status);
}

#ifdef HAVE_RT_افزودن_IPI
بیرونی تهی rto_افزودن_irq_work_func(ساختار irq_work *work);
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
ساختار uclamp_bucket {
	unsigned long مقدار : bits_per(SCHED_CAPACITY_SCALE);
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
ساختار uclamp_rq {
	unsigned صحیح مقدار;
	ساختار uclamp_bucket bucket[UCLAMP_BUCKETS];
};

DECLARE_STATIC_کلید_FALSE(sched_uclamp_used);
#endif /* CONFIG_UCLAMP_TASK */

/*
 * This is the main, per-CPU runqueue data structure.
 *
 * Locking rule: those places that want to lock multiple runqueues
 * (such as the load balancing or the thread migration code), lock
 * acquire operations must be ordered by ascending &runqueue.
 */
ساختار rq {
	/*
	 * The following members are loaded together, without holding the
	 * rq->lock, in an extremely hot loop in update_sg_lb_stats()
	 * (called from pick_next_task()). To reduce cache pollution from
	 * this operation, they are placed together on this dedicated cache
	 * line. Even though some of them are frequently modified, they are
	 * loaded much more frequently than they are stored.
	 */
	unsigned صحیح		nr_running;
#ifdef CONFIG_NUMA_BALANCING
	unsigned صحیح		nr_numa_running;
	unsigned صحیح		nr_preferred_running;
#endif
	unsigned صحیح		ttwu_pending;
	unsigned long		cpu_capacity;
#ifdef CONFIG_SCHED_PROXY_EXEC
	ساختار task_struct __rcu	*donor;  /* Scheduling context */
	ساختار task_struct __rcu	*curr;   /* Execution context */
#وگرنه
	اجتماع {
		ساختار task_struct __rcu *donor; /* Scheduler context */
		ساختار task_struct __rcu *curr;  /* Execution context */
	};
#endif
	ساختار task_struct	*idle;
	/* padding left here deliberately */

	/*
	 * The next cacheline holds the (hot) runqueue lock, as well as
	 * some other less performance-critical fields.
	 */
	u64			nr_switches	____cacheline_aligned;

	/* runqueue lock: */
	raw_spinlock_t		__lock;

#ifdef CONFIG_NO_HZ_COMMON
	unsigned صحیح		nohz_tick_stopped;
	atomic_t		nohz_flags;
	unsigned صحیح		has_blocked_load;
	unsigned long		last_blocked_load_update_tick;
	call_single_data_t	nohz_csd;
#endif /* CONFIG_NO_HZ_COMMON */

#ifdef CONFIG_UCLAMP_TASK
	/* Utilization clamp values based on CPU's RUNNABLE tasks */
	ساختار uclamp_rq	uclamp[UCLAMP_CNT] ____cacheline_aligned;
	unsigned صحیح		uclamp_flags;
#define UCLAMP_FLAG_IDLE 0x01
#endif

	ساختار cfs_rq		cfs;
	ساختار rt_rq		rt;
	ساختار dl_rq		dl;
#ifdef CONFIG_SCHED_CLASS_EXT
	ساختار scx_rq		scx;
	ساختار sched_dl_entity	ext_server;
#endif
#ifdef CONFIG_SCHED_CACHE
	raw_spinlock_t		cpu_epoch_lock ____cacheline_aligned;
	u64			cpu_runtime;
	unsigned long		cpu_epoch;
	unsigned long		cpu_epoch_next;
#endif

	ساختار sched_dl_entity	fair_server;

#ifdef CONFIG_FAIR_GROUP_SCHED
	/* list of leaf cfs_rq on this CPU: */
	ساختار list_head	leaf_cfs_rq_list;
	ساختار list_head	*tmp_alone_branch;
#endif /* CONFIG_FAIR_GROUP_SCHED */

#ifdef CONFIG_NUMA_BALANCING
	unsigned صحیح		numa_migrate_on;
#endif

#ifdef CONFIG_SCHED_CACHE
	unsigned صحیح		nr_pref_llc_running;
	unsigned صحیح		nr_llc_running;
#endif

	/*
	 * This is part of a global counter where only the total sum
	 * over all CPUs matters. A task can increase this counter on
	 * one CPU and if it got migrated afterwards it may decrease
	 * it on another CPU. Always updated under the runqueue lock:
	 */
	unsigned long		nr_uninterruptible;

	ساختار sched_dl_entity	*dl_server;
	ساختار task_struct	*stop;
	ثابت ساختار sched_class *next_class;
	unsigned long		next_balance;
	ساختار mm_struct	*prev_mm;

	/*
	 * The following fields of clock data are frequently referenced
	 * and updated together, and should go on their own cache line.
	 */
	u64			clock_task ____cacheline_aligned;
	u64			clock_pelt;
	u64			clock;
	unsigned long		lost_idle_time;
	unsigned صحیح		clock_update_flags;
	u64			clock_pelt_idle;
	u64			clock_idle;

#ifndef CONFIG_64BIT
	u64			clock_pelt_idle_copy;
	u64			clock_idle_copy;
#endif

	u64 last_seen_need_resched_ns;
	صحیح ticks_without_resched;

#ifdef CONFIG_MEMBARRIER
	صحیح membarrier_state;
#endif

	ساختار ریشه_domain		*rd;
	ساختار sched_domain __rcu	*sd;

	ساختار balance_callback *balance_callback;

	unsigned نویسه		nohz_idle_balance;
	unsigned نویسه		idle_balance;

	unsigned long		misfit_task_load;

	/* For active balancing */
	صحیح			active_balance;
	صحیح			افزودن_cpu;
	ساختار cpu_stop_work	active_balance_work;

	/* CPU of this runqueue: */
	صحیح			cpu;
	صحیح			online;

	ساختار list_head cfs_tasks;

	ساختار sched_avg	avg_rt;
	ساختار sched_avg	avg_dl;
#ifdef CONFIG_HAVE_SCHED_AVG_IRQ
	ساختار sched_avg	avg_irq;
#endif
#ifdef CONFIG_SCHED_HW_PRESSURE
	ساختار sched_avg	avg_hw;
#endif
	u64			idle_stamp;
	u64			avg_idle;

	/* This is used to determine avg_idle's max value */
	u64			max_idle_balance_cost;

#ifdef CONFIG_HOTPLUG_CPU
	ساختار rcuwait		hotplug_wait;
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
	bool			npc_افزودن_work_pending;
#endif

	/* calc_load related fields */
	unsigned long		calc_load_update;
	long			calc_load_active;

#ifdef CONFIG_SCHED_HRTICK
	call_single_data_t	hrtick_csd;
	ساختار hrtimer		hrtick_timer;
	ktime_t			hrtick_time;
	ktime_t			hrtick_delay;
	unsigned صحیح		hrtick_sched;
#endif

#ifdef CONFIG_SCHEDSTATS
	/* latency stats */
	ساختار sched_info	rq_sched_info;
	unsigned long long	rq_cpu_time;

	/* sys_sched_yield() stats */
	unsigned صحیح		yld_شمار;

	/* schedule() stats */
	unsigned صحیح		sched_شمار;
	unsigned صحیح		sched_goidle;

	/* try_to_wake_up() stats */
	unsigned صحیح		ttwu_شمار;
	unsigned صحیح		ttwu_local;
#endif

#ifdef CONFIG_CPU_IDLE
	/* Must be inspected within a RCU lock section */
	ساختار cpuidle_state	*idle_state;
#endif

	unsigned صحیح		nr_pinned;
	unsigned صحیح		افزودن_busy;
	ساختار cpu_stop_work	افزودن_work;

#ifdef CONFIG_SCHED_CORE
	/* per rq */
	ساختار rq		*core;
	ساختار task_struct	*core_pick;
	ساختار sched_dl_entity	*core_dl_server;
	unsigned صحیح		core_enabled;
	ساختار rb_ریشه		core_درخت;

	/* shared state -- careful with sched_core_cpu_deactivate() */
	unsigned صحیح		core_task_seq;
	unsigned صحیح		core_pick_seq;
	unsigned long		core_cookie;
	unsigned صحیح		core_forceidle_شمار;
	unsigned صحیح		core_forceidle_seq;
	unsigned صحیح		core_forceidle_occupation;
	u64			core_forceidle_start;
	unsigned صحیح		core_pick_in_flight;
#endif /* CONFIG_SCHED_CORE */

	/* Scratch cpumask to be temporarily used under rq_lock */
	cpumask_var_t		scratch_mask;

#ifdef CONFIG_CFS_BANDWIDTH
	call_single_data_t	cfsb_csd;
	ساختار list_head	cfsb_csd_list;
#endif

	atomic_t		nr_iowait;
} __no_randomize_layout;

#ifdef CONFIG_FAIR_GROUP_SCHED

/* CPU runqueue to which this cfs_rq is attached */
ایستا درونخطی ساختار rq *rq_of(ساختار cfs_rq *cfs_rq)
{
	بازگشت cfs_rq->rq;
}

#وگرنه /* !CONFIG_FAIR_GROUP_SCHED: */

ایستا درونخطی ساختار rq *rq_of(ساختار cfs_rq *cfs_rq)
{
	بازگشت container_of(cfs_rq, ساختار rq, cfs);
}
#endif /* !CONFIG_FAIR_GROUP_SCHED */

ایستا درونخطی صحیح cpu_of(ساختار rq *rq)
{
	بازگشت rq->cpu;
}

#define MDF_افزودن		0x01

ایستا درونخطی bool is_migration_disabled(ساختار task_struct *p)
{
	بازگشت p->migration_disabled;
}

DECLARE_PER_CPU_SHARED_ALIGNED(ساختار rq, runqueues);
DECLARE_PER_CPU(ساختار rnd_state, sched_rnd_state);

ایستا درونخطی u32 sched_rng(تهی)
{
	بازگشت prandom_u32_state(this_cpu_ptr(&sched_rnd_state));
}

ایستا __always_inline ساختار rq *__this_rq(تهی)
{
	بازگشت this_cpu_ptr(&runqueues);
}

#define cpu_rq(cpu)		(&per_cpu(runqueues, (cpu)))
#define this_rq()		__this_rq()
#define task_rq(p)		cpu_rq(task_cpu(p))
#define cpu_curr(cpu)		(cpu_rq(cpu)->curr)
#define raw_rq()		raw_cpu_ptr(&runqueues)

ایستا درونخطی bool idle_rq(ساختار rq *rq)
{
	بازگشت rq->curr == rq->idle && !rq->nr_running && !rq->ttwu_pending;
}

/**
 * available_idle_cpu - is a given CPU idle for enqueuing work.
 * @cpu: the CPU in question.
 *
 * Return: 1 if the CPU is currently idle. 0 otherwise.
 */
ایستا درونخطی bool available_idle_cpu(صحیح cpu)
{
	اگر (!idle_rq(cpu_rq(cpu)))
		بازگشت 0;

	اگر (vcpu_is_preempted(cpu))
		بازگشت 0;

	بازگشت 1;
}

#ifdef CONFIG_SCHED_PROXY_EXEC
ایستا درونخطی تهی rq_set_donor(ساختار rq *rq, ساختار task_struct *t)
{
	rcu_assign_pointer(rq->donor, t);
}
#وگرنه
ایستا درونخطی تهی rq_set_donor(ساختار rq *rq, ساختار task_struct *t)
{
	/* Do nothing */
}
#endif

#ifdef CONFIG_SCHED_CORE
ایستا درونخطی ساختار cpumask *sched_group_span(ساختار sched_group *sg);

DECLARE_STATIC_کلید_FALSE(__sched_core_enabled);

ایستا درونخطی bool sched_core_enabled(ساختار rq *rq)
{
	بازگشت static_branch_unlikely(&__sched_core_enabled) && rq->core_enabled;
}

ایستا درونخطی bool sched_core_disabled(تهی)
{
	بازگشت !static_branch_unlikely(&__sched_core_enabled);
}

/*
 * Be careful with this function; not for general use. The return value isn't
 * stable unless you actually hold a relevant rq->__lock.
 */
ایستا درونخطی raw_spinlock_t *rq_lockp(ساختار rq *rq)
{
	اگر (sched_core_enabled(rq))
		بازگشت &rq->core->__lock;

	بازگشت &rq->__lock;
}

ایستا درونخطی raw_spinlock_t *__rq_lockp(ساختار rq *rq)
	__returns_ctx_lock(rq_lockp(rq)) /* alias them */
{
	اگر (rq->core_enabled)
		بازگشت &rq->core->__lock;

	بازگشت &rq->__lock;
}

بیرونی bool
cfs_prio_less(ثابت ساختار task_struct *a, ثابت ساختار task_struct *b, bool fi);

بیرونی تهی task_vruntime_update(ساختار rq *rq, ساختار task_struct *p, bool in_fi);

/*
 * Helpers to check if the CPU's core cookie matches with the task's cookie
 * when core scheduling is enabled.
 * A special case is that the task's cookie always matches with CPU's core
 * cookie if the CPU is in an idle core.
 */
ایستا درونخطی bool sched_cpu_cookie_match(ساختار rq *rq, ساختار task_struct *p)
{
	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	اگر (!sched_core_enabled(rq))
		بازگشت true;

	بازگشت rq->core->core_cookie == p->core_cookie;
}

ایستا درونخطی bool sched_core_cookie_match(ساختار rq *rq, ساختار task_struct *p)
{
	bool idle_core = true;
	صحیح cpu;

	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	اگر (!sched_core_enabled(rq))
		بازگشت true;

	اگر (rq->core->core_cookie == p->core_cookie)
		بازگشت true;

	for_each_cpu(cpu, cpu_smt_mask(cpu_of(rq))) {
		اگر (!available_idle_cpu(cpu)) {
			idle_core = false;
			بشکن;
		}
	}

	/*
	 * A CPU in an idle core is always the best choice for tasks with
	 * cookies.
	 */
	بازگشت idle_core;
}

ایستا درونخطی bool sched_group_cookie_match(ساختار rq *rq,
					    ساختار task_struct *p,
					    ساختار sched_group *group)
{
	صحیح cpu;

	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	اگر (!sched_core_enabled(rq))
		بازگشت true;

	for_each_cpu_and(cpu, sched_group_span(group), p->cpus_ptr) {
		اگر (sched_core_cookie_match(cpu_rq(cpu), p))
			بازگشت true;
	}
	بازگشت false;
}

ایستا درونخطی bool sched_core_enqueued(ساختار task_struct *p)
{
	بازگشت !RB_EMPTY_گره(&p->core_گره);
}

بیرونی تهی sched_core_enqueue(ساختار rq *rq, ساختار task_struct *p);
بیرونی تهی sched_core_dequeue(ساختار rq *rq, ساختار task_struct *p, صحیح flags);

بیرونی تهی sched_core_get(تهی);
بیرونی تهی sched_core_put(تهی);

ایستا درونخطی bool task_has_sched_core(ساختار task_struct *p)
{
	اگر (sched_core_disabled())
		بازگشت false;

	بازگشت !!p->core_cookie;
}

#وگرنه /* !CONFIG_SCHED_CORE: */

ایستا درونخطی bool sched_core_enabled(ساختار rq *rq)
{
	بازگشت false;
}

ایستا درونخطی bool sched_core_disabled(تهی)
{
	بازگشت true;
}

ایستا درونخطی raw_spinlock_t *rq_lockp(ساختار rq *rq)
{
	بازگشت &rq->__lock;
}

ایستا درونخطی raw_spinlock_t *__rq_lockp(ساختار rq *rq)
	__returns_ctx_lock(rq_lockp(rq)) /* alias them */
{
	بازگشت &rq->__lock;
}

ایستا درونخطی bool sched_cpu_cookie_match(ساختار rq *rq, ساختار task_struct *p)
{
	بازگشت true;
}

ایستا درونخطی bool sched_core_cookie_match(ساختار rq *rq, ساختار task_struct *p)
{
	بازگشت true;
}

ایستا درونخطی bool sched_group_cookie_match(ساختار rq *rq,
					    ساختار task_struct *p,
					    ساختار sched_group *group)
{
	بازگشت true;
}

ایستا درونخطی bool task_has_sched_core(ساختار task_struct *p)
{
	بازگشت false;
}

#endif /* !CONFIG_SCHED_CORE */

#ifdef CONFIG_RT_GROUP_SCHED
# ifdef CONFIG_RT_GROUP_SCHED_DEFAULT_DISABLED
DECLARE_STATIC_کلید_FALSE(rt_group_sched);
ایستا درونخطی bool rt_group_sched_enabled(تهی)
{
	بازگشت static_branch_unlikely(&rt_group_sched);
}
# وگرنه /* !CONFIG_RT_GROUP_SCHED_DEFAULT_DISABLED: */
DECLARE_STATIC_کلید_TRUE(rt_group_sched);
ایستا درونخطی bool rt_group_sched_enabled(تهی)
{
	بازگشت static_branch_likely(&rt_group_sched);
}
# endif /* !CONFIG_RT_GROUP_SCHED_DEFAULT_DISABLED */
#وگرنه /* !CONFIG_RT_GROUP_SCHED: */
# define rt_group_sched_enabled()	false
#endif /* !CONFIG_RT_GROUP_SCHED */

ایستا درونخطی تهی lockdep_assert_rq_held(ساختار rq *rq)
	__assumes_ctx_lock(__rq_lockp(rq))
{
	lockdep_assert_held(__rq_lockp(rq));
}

بیرونی تهی raw_spin_rq_lock_nested(ساختار rq *rq, صحیح subclass)
	__acquires(__rq_lockp(rq));

بیرونی bool raw_spin_rq_trylock(ساختار rq *rq)
	__cond_acquires(true, __rq_lockp(rq));

ایستا درونخطی تهی raw_spin_rq_lock(ساختار rq *rq)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_lock_nested(rq, 0);
}

ایستا درونخطی تهی raw_spin_rq_unlock(ساختار rq *rq)
	__releases(__rq_lockp(rq))
{
	raw_spin_unlock(rq_lockp(rq));
}

ایستا درونخطی تهی raw_spin_rq_lock_irq(ساختار rq *rq)
	__acquires(__rq_lockp(rq))
{
	local_irq_disable();
	raw_spin_rq_lock(rq);
}

ایستا درونخطی تهی raw_spin_rq_unlock_irq(ساختار rq *rq)
	__releases(__rq_lockp(rq))
{
	raw_spin_rq_unlock(rq);
	local_irq_enable();
}

ایستا درونخطی unsigned long _raw_spin_rq_lock_irqsave(ساختار rq *rq)
	__acquires(__rq_lockp(rq))
{
	unsigned long flags;

	local_irq_save(flags);
	raw_spin_rq_lock(rq);

	بازگشت flags;
}

ایستا درونخطی تهی raw_spin_rq_unlock_irqrestore(ساختار rq *rq, unsigned long flags)
	__releases(__rq_lockp(rq))
{
	raw_spin_rq_unlock(rq);
	local_irq_restore(flags);
}

#define raw_spin_rq_lock_irqsave(rq, flags)	\
انجامبده {						\
	flags = _raw_spin_rq_lock_irqsave(rq);	\
} وقتی (0)

بیرونی تهی __update_idle_core(ساختار rq *rq);

ایستا درونخطی تهی update_idle_core(ساختار rq *rq)
{
	اگر (sched_smt_active())
		__update_idle_core(rq);
}

#ifdef CONFIG_FAIR_GROUP_SCHED
ایستا درونخطی ساختار task_struct *task_of(ساختار sched_entity *se)
{
	WARN_ON_ONCE(!entity_is_task(se));
	بازگشت container_of(se, ساختار task_struct, se);
}

ایستا درونخطی ساختار cfs_rq *task_cfs_rq(ساختار task_struct *p)
{
	بازگشت p->se.cfs_rq;
}

/* runqueue on which this entity is (to be) queued */
ایستا درونخطی ساختار cfs_rq *cfs_rq_of(ثابت ساختار sched_entity *se)
{
	بازگشت se->cfs_rq;
}

/* runqueue "owned" by this group */
ایستا درونخطی ساختار cfs_rq *group_cfs_rq(ساختار sched_entity *grp)
{
	بازگشت grp->my_q;
}

#وگرنه /* !CONFIG_FAIR_GROUP_SCHED: */

#define task_of(_se)		container_of(_se, ساختار task_struct, se)

ایستا درونخطی ساختار cfs_rq *task_cfs_rq(ثابت ساختار task_struct *p)
{
	بازگشت &task_rq(p)->cfs;
}

ایستا درونخطی ساختار cfs_rq *cfs_rq_of(ثابت ساختار sched_entity *se)
{
	ثابت ساختار task_struct *p = task_of(se);
	ساختار rq *rq = task_rq(p);

	بازگشت &rq->cfs;
}

/* runqueue "owned" by this group */
ایستا درونخطی ساختار cfs_rq *group_cfs_rq(ساختار sched_entity *grp)
{
	بازگشت پوچ;
}

#endif /* !CONFIG_FAIR_GROUP_SCHED */

بیرونی تهی update_rq_avg_idle(ساختار rq *rq);
بیرونی تهی update_rq_clock(ساختار rq *rq);

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

ایستا درونخطی تهی assert_clock_updated(ساختار rq *rq)
{
	/*
	 * The only reason for not seeing a clock update since the
	 * last rq_pin_lock() is if we're currently skipping updates.
	 */
	WARN_ON_ONCE(rq->clock_update_flags < RQCF_ACT_SKIP);
}

ایستا درونخطی u64 rq_clock(ساختار rq *rq)
{
	lockdep_assert_rq_held(rq);
	assert_clock_updated(rq);

	بازگشت rq->clock;
}

ایستا درونخطی u64 rq_clock_task(ساختار rq *rq)
{
	lockdep_assert_rq_held(rq);
	assert_clock_updated(rq);

	بازگشت rq->clock_task;
}

ایستا درونخطی تهی rq_clock_skip_update(ساختار rq *rq)
{
	lockdep_assert_rq_held(rq);
	rq->clock_update_flags |= RQCF_REQ_SKIP;
}

/*
 * See rt task throttling, which is the only time a skip
 * request is canceled.
 */
ایستا درونخطی تهی rq_clock_cancel_skipupdate(ساختار rq *rq)
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
ایستا درونخطی تهی rq_clock_start_loop_update(ساختار rq *rq)
{
	lockdep_assert_rq_held(rq);
	WARN_ON_ONCE(rq->clock_update_flags & RQCF_ACT_SKIP);
	rq->clock_update_flags |= RQCF_ACT_SKIP;
}

ایستا درونخطی تهی rq_clock_stop_loop_update(ساختار rq *rq)
{
	lockdep_assert_rq_held(rq);
	rq->clock_update_flags &= ~RQCF_ACT_SKIP;
}

ساختار rq_flags {
	unsigned long flags;
	ساختار pin_cookie cookie;
	/*
	 * A copy of (rq::clock_update_flags & RQCF_UPDATED) for the
	 * current pin context is stashed here in case it needs to be
	 * restored in rq_repin_lock().
	 */
	unsigned صحیح clock_update_flags;
};

بیرونی ساختار balance_callback balance_افزودن_callback;

#ifdef CONFIG_SCHED_CLASS_EXT
بیرونی ثابت ساختار sched_class ext_sched_class;

DECLARE_STATIC_کلید_FALSE(__scx_enabled);	/* SCX BPF scheduler loaded */
DECLARE_STATIC_کلید_FALSE(__scx_switched_all);	/* all fair class tasks on SCX */

#define scx_enabled()		static_branch_unlikely(&__scx_enabled)
#define scx_switched_all()	static_branch_unlikely(&__scx_switched_all)

ایستا درونخطی تهی scx_rq_clock_update(ساختار rq *rq, u64 clock)
{
	اگر (!scx_enabled())
		بازگشت;
	WRITE_ONCE(rq->scx.clock, clock);
	smp_store_release(&rq->scx.flags, rq->scx.flags | SCX_RQ_CLK_VALID);
}

ایستا درونخطی تهی scx_rq_clock_invalidate(ساختار rq *rq)
{
	اگر (!scx_enabled())
		بازگشت;
	WRITE_ONCE(rq->scx.flags, rq->scx.flags & ~SCX_RQ_CLK_VALID);
}

#وگرنه /* !CONFIG_SCHED_CLASS_EXT: */
#define scx_enabled()		false
#define scx_switched_all()	false

ایستا درونخطی تهی scx_rq_clock_update(ساختار rq *rq, u64 clock) {}
ایستا درونخطی تهی scx_rq_clock_invalidate(ساختار rq *rq) {}
#endif /* !CONFIG_SCHED_CLASS_EXT */

ایستا درونخطی تهی assert_balance_callbacks_empty(ساختار rq *rq)
{
	WARN_ON_ONCE(IS_ENABLED(CONFIG_PROVE_LOCKING) &&
		     rq->balance_callback &&
		     rq->balance_callback != &balance_افزودن_callback);
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
ایستا درونخطی تهی rq_pin_lock(ساختار rq *rq, ساختار rq_flags *rf)
{
	rf->cookie = lockdep_pin_lock(__rq_lockp(rq));

	rq->clock_update_flags &= (RQCF_REQ_SKIP|RQCF_ACT_SKIP);
	rf->clock_update_flags = 0;
	assert_balance_callbacks_empty(rq);
}

ایستا درونخطی تهی rq_unpin_lock(ساختار rq *rq, ساختار rq_flags *rf)
{
	اگر (rq->clock_update_flags > RQCF_ACT_SKIP)
		rf->clock_update_flags = RQCF_UPDATED;

	scx_rq_clock_invalidate(rq);
	lockdep_unpin_lock(__rq_lockp(rq), rf->cookie);
}

ایستا درونخطی تهی rq_repin_lock(ساختار rq *rq, ساختار rq_flags *rf)
{
	lockdep_repin_lock(__rq_lockp(rq), rf->cookie);

	/*
	 * Restore the value we stashed in @rf for this pin context.
	 */
	rq->clock_update_flags |= rf->clock_update_flags;
}

#define __task_rq_lock(...) __acquire_ret(___task_rq_lock(__VA_ARGS__), __rq_lockp(__ret))
بیرونی ساختار rq *___task_rq_lock(ساختار task_struct *p, ساختار rq_flags *rf) __acquires_ret;

#define task_rq_lock(...) __acquire_ret(_task_rq_lock(__VA_ARGS__), __rq_lockp(__ret))
بیرونی ساختار rq *_task_rq_lock(ساختار task_struct *p, ساختار rq_flags *rf)
	__acquires(&p->pi_lock) __acquires_ret;

ایستا درونخطی تهی
__task_rq_unlock(ساختار rq *rq, ساختار task_struct *p, ساختار rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_lock(rq, rf);
	raw_spin_rq_unlock(rq);
}

ایستا درونخطی تهی
task_rq_unlock(ساختار rq *rq, ساختار task_struct *p, ساختار rq_flags *rf)
	__releases(__rq_lockp(rq), &p->pi_lock)
{
	__task_rq_unlock(rq, p, rf);
	raw_spin_unlock_irqrestore(&p->pi_lock, rf->flags);
}

DEFINE_LOCK_GUARD_1(task_rq_lock, ساختار task_struct,
		    _T->rq = task_rq_lock(_T->lock, &_T->rf),
		    task_rq_unlock(_T->rq, _T->lock, &_T->rf),
		    ساختار rq *rq; ساختار rq_flags rf)
DECLARE_LOCK_GUARD_1_ATTRS(task_rq_lock, __acquires(_T->pi_lock), __releases((*(ساختار task_struct **)_T)->pi_lock))
#define class_task_rq_lock_constructor(_T) WITH_LOCK_GUARD_1_ATTRS(task_rq_lock, _T)

DEFINE_LOCK_GUARD_1(__task_rq_lock, ساختار task_struct,
		    _T->rq = __task_rq_lock(_T->lock, &_T->rf),
		    __task_rq_unlock(_T->rq, _T->lock, &_T->rf),
		    ساختار rq *rq; ساختار rq_flags rf)

ایستا درونخطی تهی rq_lock_irqsave(ساختار rq *rq, ساختار rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_lock_irqsave(rq, rf->flags);
	rq_pin_lock(rq, rf);
}

ایستا درونخطی تهی rq_lock_irq(ساختار rq *rq, ساختار rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_lock_irq(rq);
	rq_pin_lock(rq, rf);
}

ایستا درونخطی تهی rq_lock(ساختار rq *rq, ساختار rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_lock(rq);
	rq_pin_lock(rq, rf);
}

ایستا درونخطی تهی rq_unlock_irqrestore(ساختار rq *rq, ساختار rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_lock(rq, rf);
	raw_spin_rq_unlock_irqrestore(rq, rf->flags);
}

ایستا درونخطی تهی rq_unlock_irq(ساختار rq *rq, ساختار rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_lock(rq, rf);
	raw_spin_rq_unlock_irq(rq);
}

ایستا درونخطی تهی rq_unlock(ساختار rq *rq, ساختار rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_lock(rq, rf);
	raw_spin_rq_unlock(rq);
}

DEFINE_LOCK_GUARD_1(rq_lock, ساختار rq,
		    rq_lock(_T->lock, &_T->rf),
		    rq_unlock(_T->lock, &_T->rf),
		    ساختار rq_flags rf)

DECLARE_LOCK_GUARD_1_ATTRS(rq_lock, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(ساختار rq **)_T)));
#define class_rq_lock_constructor(_T) WITH_LOCK_GUARD_1_ATTRS(rq_lock, _T)

DEFINE_LOCK_GUARD_1(rq_lock_irq, ساختار rq,
		    rq_lock_irq(_T->lock, &_T->rf),
		    rq_unlock_irq(_T->lock, &_T->rf),
		    ساختار rq_flags rf)

DECLARE_LOCK_GUARD_1_ATTRS(rq_lock_irq, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(ساختار rq **)_T)));
#define class_rq_lock_irq_constructor(_T) WITH_LOCK_GUARD_1_ATTRS(rq_lock_irq, _T)

DEFINE_LOCK_GUARD_1(rq_lock_irqsave, ساختار rq,
		    rq_lock_irqsave(_T->lock, &_T->rf),
		    rq_unlock_irqrestore(_T->lock, &_T->rf),
		    ساختار rq_flags rf)

DECLARE_LOCK_GUARD_1_ATTRS(rq_lock_irqsave, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(ساختار rq **)_T)));
#define class_rq_lock_irqsave_constructor(_T) WITH_LOCK_GUARD_1_ATTRS(rq_lock_irqsave, _T)

DEFINE_LOCK_GUARD_1(raw_spin_rq_lock_irqsave, ساختار rq,
		    raw_spin_rq_lock_irqsave(_T->lock, _T->flags),
		    raw_spin_rq_unlock_irqrestore(_T->lock, _T->flags),
		    unsigned long flags)

DECLARE_LOCK_GUARD_1_ATTRS(raw_spin_rq_lock_irqsave, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(ساختار rq **)_T)));
#define class_raw_spin_rq_lock_irqsave_constructor(_T) \
	WITH_LOCK_GUARD_1_ATTRS(raw_spin_rq_lock_irqsave, _T)

#define this_rq_lock_irq(...) __acquire_ret(_this_rq_lock_irq(__VA_ARGS__), __rq_lockp(__ret))
ایستا درونخطی ساختار rq *_this_rq_lock_irq(ساختار rq_flags *rf) __acquires_ret
{
	ساختار rq *rq;

	local_irq_disable();
	rq = this_rq();
	rq_lock(rq, rf);

	بازگشت rq;
}

#ifdef CONFIG_NUMA

شمارش numa_topology_type {
	NUMA_DIRECT,
	NUMA_GLUELESS_MESH,
	NUMA_BACKPLANE,
};

بیرونی شمارش numa_topology_type sched_numa_topology_type;
بیرونی صحیح sched_max_numa_distance;
بیرونی bool find_numa_distance(صحیح distance);
بیرونی تهی sched_init_numa(صحیح offline_گره);
بیرونی تهی sched_update_numa(صحیح cpu, bool online);
بیرونی تهی sched_domains_numa_masks_set(unsigned صحیح cpu);
بیرونی تهی sched_domains_numa_masks_clear(unsigned صحیح cpu);
بیرونی صحیح sched_numa_find_closest(ثابت ساختار cpumask *cpus, صحیح cpu);

#وگرنه /* !CONFIG_NUMA: */

ایستا درونخطی تهی sched_init_numa(صحیح offline_گره) { }
ایستا درونخطی تهی sched_update_numa(صحیح cpu, bool online) { }
ایستا درونخطی تهی sched_domains_numa_masks_set(unsigned صحیح cpu) { }
ایستا درونخطی تهی sched_domains_numa_masks_clear(unsigned صحیح cpu) { }

ایستا درونخطی صحیح sched_numa_find_closest(ثابت ساختار cpumask *cpus, صحیح cpu)
{
	بازگشت nr_cpu_ids;
}

#endif /* !CONFIG_NUMA */

#ifdef CONFIG_NUMA_BALANCING

/* The regions in numa_faults array from task_struct */
شمارش numa_faults_stats {
	NUMA_MEM = 0,
	NUMA_CPU,
	NUMA_MEMBUF,
	NUMA_CPUBUF
};

بیرونی تهی sched_setnuma(ساختار task_struct *p, صحیح گره);
بیرونی صحیح migrate_task_to(ساختار task_struct *p, صحیح cpu);
بیرونی صحیح migrate_swap(ساختار task_struct *p, ساختار task_struct *t,
			صحیح cpu, صحیح scpu);
بیرونی تهی init_numa_balancing(u64 clone_flags, ساختار task_struct *p);

#وگرنه /* !CONFIG_NUMA_BALANCING: */

ایستا درونخطی تهی
init_numa_balancing(u64 clone_flags, ساختار task_struct *p)
{
}

#endif /* !CONFIG_NUMA_BALANCING */

صحیح task_llc(ثابت ساختار task_struct *p);

ایستا درونخطی تهی
queue_balance_callback(ساختار rq *rq,
		       ساختار balance_callback *head,
		       تهی (*func)(ساختار rq *rq))
{
	lockdep_assert_rq_held(rq);

	/*
	 * Don't (re)queue an already queued item; nor queue anything when
	 * balance_push() is active, see the comment with
	 * balance_push_callback.
	 */
	اگر (unlikely(head->next || rq->balance_callback == &balance_افزودن_callback))
		بازگشت;

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
	برای (__sd = rcu_dereference_sched_domain(cpu_rq(cpu)->sd); \
			__sd; __sd = __sd->parent)

/* A mask of all the SD flags that have the SDF_SHARED_CHILD metaflag */
#define SD_FLAG(name, mflags) (name * !!((mflags) & SDF_SHARED_CHILD)) |
ایستا ثابت unsigned صحیح SD_SHARED_CHILD_MASK =
#شامل <linux/sched/sd_flags.h>
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
ایستا درونخطی ساختار sched_domain *highest_flag_domain(صحیح cpu, صحیح flag)
{
	ساختار sched_domain *sd, *hsd = پوچ;

	for_each_domain(cpu, sd) {
		اگر (sd->flags & flag) {
			hsd = sd;
			ادامه;
		}

		/*
		 * Stop the search if @flag is known to be shared at lower
		 * levels. It will not be found further up.
		 */
		اگر (flag & SD_SHARED_CHILD_MASK)
			بشکن;
	}

	بازگشت hsd;
}

ایستا درونخطی ساختار sched_domain *lowest_flag_domain(صحیح cpu, صحیح flag)
{
	ساختار sched_domain *sd;

	for_each_domain(cpu, sd) {
		اگر (sd->flags & flag)
			بشکن;
	}

	بازگشت sd;
}

DECLARE_PER_CPU(ساختار sched_domain __rcu *, sd_llc);
DECLARE_PER_CPU(صحیح, sd_llc_size);
DECLARE_PER_CPU(صحیح, sd_llc_id);
DECLARE_PER_CPU(صحیح, sd_share_id);
DECLARE_PER_CPU(ساختار sched_domain_shared __rcu *, sd_llc_shared);
DECLARE_PER_CPU(ساختار sched_domain_shared __rcu *, sd_balance_shared);
DECLARE_PER_CPU(ساختار sched_domain __rcu *, sd_numa);
DECLARE_PER_CPU(ساختار sched_domain __rcu *, sd_asym_packing);
DECLARE_PER_CPU(ساختار sched_domain __rcu *, sd_asym_cpucapacity);

بیرونی ساختار static_کلید_false sched_asym_cpucapacity;
بیرونی ساختار static_کلید_false sched_cluster_active;

ایستا __always_inline bool sched_asym_cpucap_active(تهی)
{
	بازگشت static_branch_unlikely(&sched_asym_cpucapacity);
}

ساختار sched_group_capacity {
	atomic_t		ref;
	/*
	 * CPU capacity of this group, SCHED_CAPACITY_SCALE being max capacity
	 * for a single CPU.
	 */
	unsigned long		capacity;
	unsigned long		min_capacity;		/* Min per-CPU capacity in group */
	unsigned long		max_capacity;		/* Max per-CPU capacity in group */
	unsigned long		next_update;
	صحیح			imbalance;		/* XXX unrelated to capacity but shared group state */

	صحیح			id;

	unsigned long		cpumask[];		/* Balance mask */
};

ساختار sched_group {
	ساختار sched_group	*next;			/* Must be a circular list */
	atomic_t		ref;

	unsigned صحیح		group_weight;
	unsigned صحیح		cores;
	ساختار sched_group_capacity *sgc;
	صحیح			asym_prefer_cpu;	/* CPU of highest priority in group */
	صحیح			flags;

	/*
	 * The CPUs this group covers.
	 *
	 * NOTE: this field is variable length. (Allocated dynamically
	 * by attaching extra space to the end of the structure,
	 * depending on how many CPUs the kernel has booted up with)
	 */
	unsigned long		cpumask[];
};

ایستا درونخطی ساختار cpumask *sched_group_span(ساختار sched_group *sg)
{
	بازگشت to_cpumask(sg->cpumask);
}

/*
 * See build_balance_mask().
 */
ایستا درونخطی ساختار cpumask *group_balance_mask(ساختار sched_group *sg)
{
	بازگشت to_cpumask(sg->sgc->cpumask);
}

بیرونی صحیح group_balance_cpu(ساختار sched_group *sg);

بیرونی تهی update_sched_domain_debugfs(تهی);
بیرونی تهی dirty_sched_domain_sysctl(صحیح cpu);

بیرونی صحیح sched_update_scaling(تهی);

ایستا درونخطی ثابت ساختار cpumask *task_user_cpus(ساختار task_struct *p)
{
	اگر (!p->user_cpus_ptr)
		بازگشت cpu_possible_mask; /* &init_task.cpus_mask */
	بازگشت p->user_cpus_ptr;
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
ایستا درونخطی ساختار task_group *task_group(ساختار task_struct *p)
{
	بازگشت p->sched_task_group;
}

#ifdef CONFIG_FAIR_GROUP_SCHED
/*
 * Defined here to be available before stats.h is included, since
 * stats.h has dependencies on things defined later in this file.
 */
ساختار cfs_tg_state {
	ساختار cfs_rq		cfs_rq;
	ساختار sched_entity	se;
	ساختار sched_statistics	stats;
} __no_randomize_layout;

/* Access a specific CPU's cfs_rq from a task group */
ایستا درونخطی ساختار cfs_rq *tg_cfs_rq(ساختار task_group *tg, صحیح cpu)
{
	بازگشت per_cpu_ptr(tg->cfs_rq, cpu);
}

ایستا درونخطی ساختار sched_entity *tg_se(ساختار task_group *tg, صحیح cpu)
{
	ساختار cfs_tg_state *state;

	اگر (is_ریشه_task_group(tg))
		بازگشت پوچ;

	state = container_of(tg_cfs_rq(tg, cpu), ساختار cfs_tg_state, cfs_rq);
	بازگشت &state->se;
}

ایستا درونخطی ساختار sched_entity *cfs_rq_se(ساختار cfs_rq *cfs_rq)
{
	ساختار cfs_tg_state *state;

	اگر (is_ریشه_task_group(cfs_rq->tg))
		بازگشت پوچ;

	state = container_of(cfs_rq, ساختار cfs_tg_state, cfs_rq);
	بازگشت &state->se;
}
#endif

/* Change a task's cfs_rq and parent entity if it moves across CPUs/groups */
ایستا درونخطی تهی set_task_rq(ساختار task_struct *p, unsigned صحیح cpu)
{
#اگر defined(CONFIG_FAIR_GROUP_SCHED) || defined(CONFIG_RT_GROUP_SCHED)
	ساختار task_group *tg = task_group(p);
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
	اگر (!rt_group_sched_enabled())
		tg = &ریشه_task_group;
	p->rt.rt_rq  = tg->rt_rq[cpu];
	p->rt.parent = tg->rt_se[cpu];
#endif /* CONFIG_RT_GROUP_SCHED */
}

#وگرنه /* !CONFIG_CGROUP_SCHED: */

ایستا درونخطی تهی set_task_rq(ساختار task_struct *p, unsigned صحیح cpu) { }

ایستا درونخطی ساختار task_group *task_group(ساختار task_struct *p)
{
	بازگشت پوچ;
}

#endif /* !CONFIG_CGROUP_SCHED */

ایستا درونخطی تهی __set_task_cpu(ساختار task_struct *p, unsigned صحیح cpu)
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

شمارش {
#شامل "features.h"
	__SCHED_FEAT_NR,
};

#undef SCHED_FEAT

/*
 * To support run-time toggling of sched features, all the translation units
 * (but core.c) reference the sysctl_sched_features defined in core.c.
 */
بیرونی __read_mostly unsigned صحیح sysctl_sched_features;

#ifdef CONFIG_JUMP_LABEL

اجتماع sched_feat_کلید {
	ساختار static_کلید_true	کلید_true;
	ساختار static_کلید_false	کلید_false;
};

#define sched_feat_branch_true(کلید)	static_branch_likely(&(کلید)->کلید_true)
#define sched_feat_branch_false(کلید)	static_branch_unlikely(&(کلید)->کلید_false)

#define SCHED_FEAT(name, enabled)			\
ایستا __always_inline bool				\
static_branch_##name(اجتماع sched_feat_کلید *کلید) 	\
{							\
	بازگشت sched_feat_branch_##enabled(کلید);	\
}

#شامل "features.h"
#undef SCHED_FEAT

بیرونی اجتماع sched_feat_کلید sched_feat_کلیدها[__SCHED_FEAT_NR];
#define sched_feat(x) (static_branch_##x(&sched_feat_کلیدها[__SCHED_FEAT_##x]))

#وگرنه /* !CONFIG_JUMP_LABEL: */

#define sched_feat(x) (sysctl_sched_features & (1UL << __SCHED_FEAT_##x))

#endif /* !CONFIG_JUMP_LABEL */

بیرونی ساختار static_کلید_false sched_numa_balancing;
بیرونی ساختار static_کلید_false sched_schedstats;

ایستا درونخطی u64 global_rt_period(تهی)
{
	بازگشت (u64)sysctl_sched_rt_period * NSEC_PER_USEC;
}

ایستا درونخطی u64 global_rt_runtime(تهی)
{
	اگر (sysctl_sched_rt_runtime < 0)
		بازگشت RUNTIME_INF;

	بازگشت (u64)sysctl_sched_rt_runtime * NSEC_PER_USEC;
}

/*
 * Is p the current execution context?
 */
ایستا درونخطی صحیح task_current(ساختار rq *rq, ساختار task_struct *p)
{
	بازگشت rq->curr == p;
}

/*
 * Is p the current scheduling context?
 *
 * Note that it might be the current execution context at the same time if
 * rq->curr == rq->donor == p.
 */
ایستا درونخطی صحیح task_current_donor(ساختار rq *rq, ساختار task_struct *p)
{
	بازگشت rq->donor == p;
}

ایستا درونخطی bool task_is_blocked(ساختار task_struct *p)
{
	اگر (!sched_proxy_exec())
		بازگشت false;

	بازگشت !!p->blocked_on;
}

#ifdef CONFIG_SCHED_PROXY_EXEC
تهی sched_proxy_block_task(ساختار rq *rq, ساختار task_struct *p);
#وگرنه
ایستا درونخطی تهی sched_proxy_block_task(ساختار rq *rq, ساختار task_struct *p) {}
#endif

ایستا درونخطی صحیح task_on_cpu(ساختار rq *rq, ساختار task_struct *p)
{
	بازگشت p->on_cpu;
}

ایستا درونخطی صحیح task_on_rq_queued(ساختار task_struct *p)
{
	بازگشت READ_ONCE(p->on_rq) == TASK_ON_RQ_QUEUED;
}

ایستا درونخطی صحیح task_on_rq_migrating(ساختار task_struct *p)
{
	بازگشت READ_ONCE(p->on_rq) == TASK_ON_RQ_MIGRATING;
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

بیرونی ثابت صحیح		sched_prio_to_weight[40];
بیرونی ثابت u32		sched_prio_to_wmult[40];

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

#define RETRY_TASK		((تهی *)-1UL)

ساختار affinity_context {
	ثابت ساختار cpumask	*new_mask;
	ساختار cpumask		*user_mask;
	unsigned صحیح		flags;
};

بیرونی s64 update_curr_common(ساختار rq *rq);

شمارش snt_e {
	SNT_NORMAL,	/* set_next_task() */
	SNT_PICK,	/* put_prev_set_next_task(): prev != next */
	SNT_REPICK,	/* put_prev_set_next_task(): prev == next */
};

ساختار sched_class {

#ifdef CONFIG_UCLAMP_TASK
	صحیح uclamp_enabled;
#endif

	/*
	 * move_queued_task/activate_task/enqueue_task: rq->lock
	 * ttwu_do_activate/activate_task/enqueue_task: rq->lock
	 * wake_up_new_task/activate_task/enqueue_task: task_rq_lock
	 * ttwu_runnable/enqueue_task: task_rq_lock
	 * proxy_task_current: rq->lock
	 * sched_change_end
	 */
	تهی (*enqueue_task) (ساختار rq *rq, ساختار task_struct *p, صحیح flags);
	/*
	 * move_queued_task/deactivate_task/dequeue_task: rq->lock
	 * __schedule/block_task/dequeue_task: rq->lock
	 * proxy_task_current: rq->lock
	 * wait_task_inactive: task_rq_lock
	 * sched_change_begin
	 */
	bool (*dequeue_task) (ساختار rq *rq, ساختار task_struct *p, صحیح flags);

	/*
	 * do_sched_yield: rq->lock
	 */
	تهی (*yield_task)   (ساختار rq *rq);
	/*
	 * yield_to: rq->lock (double)
	 */
	bool (*yield_to_task)(ساختار rq *rq, ساختار task_struct *p);

	/*
	 * move_queued_task: rq->lock
	 * __migrate_swap_task: rq->lock
	 * ttwu_do_activate: rq->lock
	 * ttwu_runnable: task_rq_lock
	 * wake_up_new_task: task_rq_lock
	 */
	تهی (*wakeup_preempt)(ساختار rq *rq, ساختار task_struct *p, صحیح flags);

	/*
	 * schedule/pick_next_task/prev_balance: rq->lock
	 */
	صحیح (*balance)(ساختار rq *rq, ساختار rq_flags *rf);

	/*
	 * schedule/pick_next_task: rq->lock
	 */
	ساختار task_struct *(*pick_task)(ساختار rq *rq, ساختار rq_flags *rf);

	/*
	 * sched_change:
	 * __schedule: rq->lock
	 */
	تهی (*put_prev_task)(ساختار rq *rq, ساختار task_struct *p, ساختار task_struct *next);
	تهی (*set_next_task)(ساختار rq *rq, ساختار task_struct *p, شمارش snt_e type);

	/*
	 * select_task_rq: p->pi_lock
	 * sched_exec: p->pi_lock
	 */
	صحیح  (*select_task_rq)(ساختار task_struct *p, صحیح task_cpu, صحیح flags);

	/*
	 * set_task_cpu: p->pi_lock || rq->lock (ttwu like)
	 */
	تهی (*migrate_task_rq)(ساختار task_struct *p, صحیح new_cpu);

	/*
	 * ttwu_do_activate: rq->lock
	 * wake_up_new_task: task_rq_lock
	 */
	تهی (*task_woken)(ساختار rq *this_rq, ساختار task_struct *task);

	/*
	 * do_set_cpus_allowed: task_rq_lock + sched_change
	 */
	تهی (*set_cpus_allowed)(ساختار task_struct *p, ساختار affinity_context *ctx);

	/*
	 * sched_set_rq_{on,off}line: rq->lock
	 */
	تهی (*rq_online)(ساختار rq *rq);
	تهی (*rq_offline)(ساختار rq *rq);

	/*
	 * push_cpu_stop: p->pi_lock && rq->lock
	 */
	ساختار rq *(*find_lock_rq)(ساختار task_struct *p, ساختار rq *rq);

	/*
	 * hrtick: rq->lock
	 * sched_tick: rq->lock
	 * sched_tick_remote: rq->lock
	 */
	تهی (*task_tick)(ساختار rq *rq, ساختار task_struct *p, صحیح queued);
	/*
	 * sched_cgroup_fork: p->pi_lock
	 */
	تهی (*task_fork)(ساختار task_struct *p);
	/*
	 * finish_task_switch: no locks
	 */
	تهی (*task_dead)(ساختار task_struct *p);

	/*
	 * sched_change
	 */
	تهی (*switching_from)(ساختار rq *this_rq, ساختار task_struct *task);
	تهی (*switched_from) (ساختار rq *this_rq, ساختار task_struct *task);
	تهی (*switching_to)  (ساختار rq *this_rq, ساختار task_struct *task);
	تهی (*switched_to)   (ساختار rq *this_rq, ساختار task_struct *task);
	u64  (*get_prio)     (ساختار rq *this_rq, ساختار task_struct *task);
	تهی (*prio_changed) (ساختار rq *this_rq, ساختار task_struct *task,
			      u64 oldprio);

	/*
	 * set_load_weight: task_rq_lock + sched_change
	 * __setscheduler_parms: task_rq_lock + sched_change
	 */
	تهی (*reweight_task)(ساختار rq *this_rq, ساختار task_struct *task,
			      ثابت ساختار load_weight *lw);

	/*
	 * sched_rr_get_interval: task_rq_lock
	 */
	unsigned صحیح (*get_rr_interval)(ساختار rq *rq,
					ساختار task_struct *task);

	/*
	 * task_sched_runtime: task_rq_lock
	 */
	تهی (*update_curr)(ساختار rq *rq);

#ifdef CONFIG_FAIR_GROUP_SCHED
	/*
	 * sched_change_group: task_rq_lock + sched_change
	 */
	تهی (*task_change_group)(ساختار task_struct *p);
#endif

#ifdef CONFIG_SCHED_CORE
	/*
	 * pick_next_task: rq->lock
	 * try_steal_cookie: rq->lock (double)
	 */
	صحیح (*task_is_throttled)(ساختار task_struct *p, صحیح cpu);
#endif
};

ایستا درونخطی تهی put_prev_task(ساختار rq *rq, ساختار task_struct *prev)
{
	WARN_ON_ONCE(rq->donor != prev);
	prev->sched_class->put_prev_task(rq, prev, پوچ);
}

ایستا درونخطی تهی set_next_task(ساختار rq *rq, ساختار task_struct *next)
{
	next->sched_class->set_next_task(rq, next, SNT_NORMAL);
}

ایستا درونخطی تهی
__put_prev_set_next_dl_server(ساختار rq *rq,
			      ساختار task_struct *prev,
			      ساختار task_struct *next)
{
	prev->dl_server = پوچ;
	next->dl_server = rq->dl_server;
	rq->dl_server = پوچ;
}

ایستا درونخطی تهی put_prev_set_next_task(ساختار rq *rq,
					  ساختار task_struct *prev,
					  ساختار task_struct *next)
{
	WARN_ON_ONCE(rq->donor != prev);

	__put_prev_set_next_dl_server(rq, prev, next);

	اگر (next == prev) {
		next->sched_class->set_next_task(rq, next, SNT_REPICK);
		بازگشت;
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
ثابت ساختار sched_class name##_sched_class \
	__aligned(__alignof__(ساختار sched_class)) \
	__section("__" #name "_sched_class")

/* Defined in include/asm-generic/vmlinux.lds.h */
بیرونی ساختار sched_class __sched_class_highest[];
بیرونی ساختار sched_class __sched_class_lowest[];

بیرونی ثابت ساختار sched_class stop_sched_class;
بیرونی ثابت ساختار sched_class dl_sched_class;
بیرونی ثابت ساختار sched_class rt_sched_class;
بیرونی ثابت ساختار sched_class fair_sched_class;
بیرونی ثابت ساختار sched_class idle_sched_class;

/*
 * Iterate only active classes. SCX can take over all fair tasks or be
 * completely disabled. If the former, skip fair. If the latter, skip SCX.
 */
ایستا درونخطی ثابت ساختار sched_class *next_active_class(ثابت ساختار sched_class *class)
{
	class++;
#ifdef CONFIG_SCHED_CLASS_EXT
	اگر (scx_switched_all() && class == &fair_sched_class)
		class++;
	اگر (!scx_enabled() && class == &ext_sched_class)
		class++;
#endif
	بازگشت class;
}

#define for_class_range(class, _from, _to) \
	برای (class = (_from); class < (_to); class++)

#define for_each_class(class) \
	for_class_range(class, __sched_class_highest, __sched_class_lowest)

#define for_active_class_range(class, _from, _to)				\
	برای (class = (_from); class != (_to); class = next_active_class(class))

#define for_each_active_class(class)						\
	for_active_class_range(class, __sched_class_highest, __sched_class_lowest)

#define sched_class_above(_a, _b)	((_a) < (_b))

ایستا درونخطی تهی rq_modified_begin(ساختار rq *rq, ثابت ساختار sched_class *class)
{
	اگر (sched_class_above(rq->next_class, class))
		rq->next_class = class;
}

ایستا درونخطی bool rq_modified_above(ساختار rq *rq, ثابت ساختار sched_class *class)
{
	بازگشت sched_class_above(rq->next_class, class);
}

ایستا درونخطی bool sched_stop_runnable(ساختار rq *rq)
{
	بازگشت rq->stop && task_on_rq_queued(rq->stop);
}

ایستا درونخطی bool sched_dl_runnable(ساختار rq *rq)
{
	بازگشت rq->dl.dl_nr_running > 0;
}

ایستا درونخطی bool sched_rt_runnable(ساختار rq *rq)
{
	بازگشت rq->rt.rt_queued > 0;
}

ایستا درونخطی bool sched_fair_runnable(ساختار rq *rq)
{
	بازگشت rq->cfs.nr_queued > 0;
}

بیرونی ساختار task_struct *pick_task_fair(ساختار rq *rq, ساختار rq_flags *rf);
بیرونی ساختار task_struct *pick_task_idle(ساختار rq *rq, ساختار rq_flags *rf);

#define SCA_CHECK		0x01
#define SCA_MIGRATE_DISABLE	0x02
#define SCA_MIGRATE_ENABLE	0x04
#define SCA_USER		0x08

بیرونی تهی update_group_capacity(ساختار sched_domain *sd, صحیح cpu);

بیرونی تهی sched_balance_trigger(ساختار rq *rq);

بیرونی صحیح __set_cpus_allowed_ptr(ساختار task_struct *p, ساختار affinity_context *ctx);
بیرونی تهی set_cpus_allowed_common(ساختار task_struct *p, ساختار affinity_context *ctx);

ایستا درونخطی bool task_allowed_on_cpu(ساختار task_struct *p, صحیح cpu)
{
	/* When not in the task's cpumask, no point in looking further. */
	اگر (!cpumask_test_cpu(cpu, p->cpus_ptr))
		بازگشت false;

	/* Can @cpu run a user thread? */
	اگر (!(p->flags & PF_KTHREAD) && !task_cpu_possible(cpu, p))
		بازگشت false;

	بازگشت true;
}

ایستا درونخطی cpumask_t *alloc_user_cpus_ptr(صحیح گره)
{
	/*
	 * See set_cpus_allowed_force() above for the rcu_head usage.
	 */
	صحیح size = max_t(صحیح, cpumask_size(), اندازه(ساختار rcu_head));

	بازگشت kmalloc_گره(size, GFP_KERNEL, گره);
}

ایستا درونخطی ساختار task_struct *get_افزودن_task(ساختار rq *rq)
{
	ساختار task_struct *p = rq->donor;

	lockdep_assert_rq_held(rq);

	اگر (rq->افزودن_busy)
		بازگشت پوچ;

	اگر (p->nr_cpus_allowed == 1)
		بازگشت پوچ;

	اگر (p->migration_disabled)
		بازگشت پوچ;

	rq->افزودن_busy = true;
	بازگشت get_task_struct(p);
}

بیرونی صحیح افزودن_cpu_stop(تهی *arg);

#ifdef CONFIG_CPU_IDLE

ایستا درونخطی تهی idle_set_state(ساختار rq *rq,
				  ساختار cpuidle_state *idle_state)
{
	rq->idle_state = idle_state;
}

ایستا درونخطی ساختار cpuidle_state *idle_get_state(ساختار rq *rq)
{
	lockdep_assert(rcu_read_lock_any_held());

	بازگشت rq->idle_state;
}

#وگرنه /* !CONFIG_CPU_IDLE: */

ایستا درونخطی تهی idle_set_state(ساختار rq *rq,
				  ساختار cpuidle_state *idle_state)
{
}

ایستا درونخطی ساختار cpuidle_state *idle_get_state(ساختار rq *rq)
{
	بازگشت پوچ;
}

#endif /* !CONFIG_CPU_IDLE */

بیرونی تهی schedule_idle(تهی);
asmlinkage تهی schedule_user(تهی);

بیرونی تهی sysrq_sched_debug_show(تهی);
بیرونی تهی sched_init_granularity(تهی);
بیرونی تهی update_max_interval(تهی);

بیرونی تهی init_sched_dl_class(تهی);
بیرونی تهی init_sched_rt_class(تهی);
بیرونی تهی init_sched_fair_class(تهی);

بیرونی تهی resched_curr(ساختار rq *rq);
بیرونی تهی resched_curr_lazy(ساختار rq *rq);
بیرونی تهی resched_cpu(صحیح cpu);

بیرونی تهی init_rt_bandwidth(ساختار rt_bandwidth *rt_b, u64 period, u64 runtime);
بیرونی bool sched_rt_bandwidth_account(ساختار rt_rq *rt_rq);

بیرونی تهی init_dl_entity(ساختار sched_dl_entity *dl_se);

بیرونی تهی init_cfs_throttle_work(ساختار task_struct *p);

#define BW_SHIFT		20
#define BW_UNIT			(1 << BW_SHIFT)
#define RATIO_SHIFT		8
#define MAX_BW_BITS		(64 - BW_SHIFT)
#define MAX_BW			((1ULL << MAX_BW_BITS) - 1)

بیرونی u64 to_ratio(u64 period, u64 runtime);

بیرونی تهی init_entity_runnable_average(ساختار sched_entity *se);
بیرونی تهی post_init_entity_util_avg(ساختار task_struct *p);

#ifdef CONFIG_NO_HZ_FULL
بیرونی bool sched_can_stop_tick(ساختار rq *rq);
بیرونی صحیح __init sched_tick_offload_init(تهی);

/*
 * Tick may be needed by tasks in the runqueue depending on their policy and
 * requirements. If tick is needed, lets send the target an IPI to kick it out of
 * nohz mode if necessary.
 */
ایستا درونخطی تهی sched_update_tick_dependency(ساختار rq *rq)
{
	صحیح cpu = cpu_of(rq);

	اگر (!tick_nohz_full_cpu(cpu))
		بازگشت;

	اگر (sched_can_stop_tick(rq))
		tick_nohz_dep_clear_cpu(cpu, TICK_DEP_BIT_SCHED);
	وگرنه
		tick_nohz_dep_set_cpu(cpu, TICK_DEP_BIT_SCHED);
}
#وگرنه /* !CONFIG_NO_HZ_FULL: */
ایستا درونخطی صحیح sched_tick_offload_init(تهی) { بازگشت 0; }
ایستا درونخطی تهی sched_update_tick_dependency(ساختار rq *rq) { }
#endif /* !CONFIG_NO_HZ_FULL */

ایستا درونخطی تهی add_nr_running(ساختار rq *rq, unsigned شمار)
{
	unsigned prev_nr = rq->nr_running;

	rq->nr_running = prev_nr + شمار;
	اگر (trace_sched_update_nr_running_tp_enabled()) {
		call_trace_sched_update_nr_running(rq, شمار);
	}

	اگر (prev_nr < 2 && rq->nr_running >= 2)
		set_rd_overloaded(rq->rd, 1);

	sched_update_tick_dependency(rq);
}

ایستا درونخطی تهی sub_nr_running(ساختار rq *rq, unsigned شمار)
{
	rq->nr_running -= شمار;
	اگر (trace_sched_update_nr_running_tp_enabled()) {
		call_trace_sched_update_nr_running(rq, -شمار);
	}

	/* Check if we still need preemption */
	sched_update_tick_dependency(rq);
}

ایستا درونخطی تهی __block_task(ساختار rq *rq, ساختار task_struct *p)
{
	اگر (p->sched_contributes_to_load)
		rq->nr_uninterruptible++;

	اگر (p->in_iowait) {
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

بیرونی تهی activate_task(ساختار rq *rq, ساختار task_struct *p, صحیح flags);
بیرونی تهی deactivate_task(ساختار rq *rq, ساختار task_struct *p, صحیح flags);

بیرونی تهی wakeup_preempt(ساختار rq *rq, ساختار task_struct *p, صحیح flags);

/*
 * attach_task() -- attach the task detached by detach_task() to its new rq.
 */
ایستا درونخطی تهی attach_task(ساختار rq *rq, ساختار task_struct *p)
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
ایستا درونخطی تهی attach_one_task(ساختار rq *rq, ساختار task_struct *p)
{
	guard(rq_lock)(rq);
	update_rq_clock(rq);
	attach_task(rq, p);
}

/*
 * __attach_tasks() - attaches a list of tasks (using se.group_node) to
 * the new rq
 */
ایستا درونخطی تهی __attach_tasks(ساختار rq *rq, ساختار list_head *tasks)
{
	guard(rq_lock)(rq);
	update_rq_clock(rq);

	وقتی (!list_empty(tasks)) {
		ساختار task_struct *p;

		p = list_first_entry(tasks, ساختار task_struct, se.group_گره);
		list_del_init(&p->se.group_گره);

		attach_task(rq, p);
	}
}

#ifdef CONFIG_PREEMPT_RT
# define SCHED_NR_MIGRATE_BREAK 8
#وگرنه
# define SCHED_NR_MIGRATE_BREAK 32
#endif

بیرونی __read_mostly unsigned صحیح sysctl_sched_nr_migrate;
بیرونی __read_mostly unsigned صحیح sysctl_sched_migration_cost;

بیرونی unsigned صحیح sysctl_sched_base_slice;

بیرونی صحیح sysctl_resched_latency_warn_ms;
بیرونی صحیح sysctl_resched_latency_warn_once;

بیرونی unsigned صحیح sysctl_sched_tunable_scaling;

بیرونی unsigned صحیح sysctl_numa_balancing_scan_delay;
بیرونی unsigned صحیح sysctl_numa_balancing_scan_period_min;
بیرونی unsigned صحیح sysctl_numa_balancing_scan_period_max;
بیرونی unsigned صحیح sysctl_numa_balancing_scan_size;
بیرونی unsigned صحیح sysctl_numa_balancing_hot_threshold;

#ifdef CONFIG_SCHED_HRTICK

/*
 * Use hrtick when:
 *  - enabled by features
 *  - hrtimer is actually high res
 */
ایستا درونخطی bool hrtick_enabled(ساختار rq *rq)
{
	بازگشت cpu_active(cpu_of(rq)) && hrtimer_highres_enabled();
}

ایستا درونخطی bool hrtick_enabled_fair(ساختار rq *rq)
{
	بازگشت sched_feat(HRTICK) && hrtick_enabled(rq);
}

ایستا درونخطی bool hrtick_enabled_dl(ساختار rq *rq)
{
	بازگشت sched_feat(HRTICK_DL) && hrtick_enabled(rq);
}

بیرونی تهی hrtick_start(ساختار rq *rq, u64 delay);
ایستا درونخطی bool hrtick_active(ساختار rq *rq)
{
	بازگشت hrtimer_active(&rq->hrtick_timer);
}

#وگرنه /* !CONFIG_SCHED_HRTICK: */
ایستا درونخطی bool hrtick_enabled_fair(ساختار rq *rq) { بازگشت false; }
ایستا درونخطی bool hrtick_enabled_dl(ساختار rq *rq) { بازگشت false; }
ایستا درونخطی bool hrtick_enabled(ساختار rq *rq) { بازگشت false; }
#endif /* !CONFIG_SCHED_HRTICK */

#ifndef arch_scale_freq_tick
ایستا __always_inline تهی arch_scale_freq_tick(تهی) { }
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
ایستا __always_inline
unsigned long arch_scale_freq_capacity(صحیح cpu)
{
	بازگشت SCHED_CAPACITY_SCALE;
}
#endif

/*
 * In double_lock_balance()/double_rq_lock(), we use raw_spin_rq_lock() to
 * acquire rq lock instead of rq_lock(). So at the end of these two functions
 * we need to call double_rq_clock_clear_update() to clear RQCF_UPDATED of
 * rq->clock_update_flags to avoid the WARN_DOUBLE_CLOCK warning.
 */
ایستا درونخطی تهی double_rq_clock_clear_update(ساختار rq *rq1, ساختار rq *rq2)
{
	rq1->clock_update_flags &= (RQCF_REQ_SKIP|RQCF_ACT_SKIP);
	rq2->clock_update_flags &= (RQCF_REQ_SKIP|RQCF_ACT_SKIP);
}

#define DEFINE_LOCK_GUARD_2(name, type, _lock, _unlock, ...)				\
__DEFINE_UNLOCK_GUARD(name, type, _unlock, type *lock2; __VA_ARGS__)			\
ایستا درونخطی class_##name##_t class_##name##_constructor(type *lock, type *lock2)	\
	__no_context_analysis								\
{ class_##name##_t _t = { .lock = lock, .lock2 = lock2 }, *_T = &_t;			\
  _lock; بازگشت _t; }
#define DECLARE_LOCK_GUARD_2_ATTRS(_name, _lock, _unlock1, _unlock2)			\
ایستا درونخطی class_##_name##_t class_##_name##_constructor(lock_##_name##_t *_T1,	\
							    lock_##_name##_t *_T2) _lock; \
ایستا __always_inline تهی __class_##_name##_cleanup_ctx1(class_##_name##_t **_T1)	\
	__no_context_analysis _unlock1 { }						\
ایستا __always_inline تهی __class_##_name##_cleanup_ctx2(class_##_name##_t **_T2)	\
	__no_context_analysis _unlock2 { }
#define WITH_LOCK_GUARD_2_ATTRS(_name, _T1, _T2)					\
	class_##_name##_constructor(_T1, _T2),						\
	*__UNIQUE_ID(unlock1) __cleanup(__class_##_name##_cleanup_ctx1) = (تهی *)(_T1),\
	*__UNIQUE_ID(unlock2) __cleanup(__class_##_name##_cleanup_ctx2) = (تهی *)(_T2)

ایستا درونخطی bool rq_order_less(ساختار rq *rq1, ساختار rq *rq2)
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
	اگر (rq1->core->cpu < rq2->core->cpu)
		بازگشت true;
	اگر (rq1->core->cpu > rq2->core->cpu)
		بازگشت false;

	/*
	 * __sched_core_flip() relies on SMT having cpu-id lock order.
	 */
#endif /* CONFIG_SCHED_CORE */
	بازگشت rq1->cpu < rq2->cpu;
}

بیرونی تهی double_rq_lock(ساختار rq *rq1, ساختار rq *rq2)
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
ایستا درونخطی صحیح _double_lock_balance(ساختار rq *this_rq, ساختار rq *busiest)
	__must_hold(__rq_lockp(this_rq))
	__acquires(__rq_lockp(busiest))
{
	raw_spin_rq_unlock(this_rq);
	double_rq_lock(this_rq, busiest);

	بازگشت 1;
}

#وگرنه /* !CONFIG_PREEMPTION: */
/*
 * Unfair double_lock_balance: Optimizes throughput at the expense of
 * latency by eliminating extra atomic operations when the locks are
 * already in proper order on entry.  This favors lower CPU-ids and will
 * grant the double lock to lower CPUs over higher ids under contention,
 * regardless of entry order into the function.
 */
ایستا درونخطی صحیح _double_lock_balance(ساختار rq *this_rq, ساختار rq *busiest)
	__must_hold(__rq_lockp(this_rq))
	__acquires(__rq_lockp(busiest))
{
	اگر (__rq_lockp(this_rq) == __rq_lockp(busiest)) {
		__acquire(__rq_lockp(busiest)); /* already held */
		double_rq_clock_clear_update(this_rq, busiest);
		بازگشت 0;
	}

	اگر (likely(raw_spin_rq_trylock(busiest))) {
		double_rq_clock_clear_update(this_rq, busiest);
		بازگشت 0;
	}

	اگر (rq_order_less(this_rq, busiest)) {
		raw_spin_rq_lock_nested(busiest, SINGLE_DEPTH_NESTING);
		double_rq_clock_clear_update(this_rq, busiest);
		بازگشت 0;
	}

	raw_spin_rq_unlock(this_rq);
	double_rq_lock(this_rq, busiest);

	بازگشت 1;
}

#endif /* !CONFIG_PREEMPTION */

/*
 * double_lock_balance - lock the busiest runqueue, this_rq is locked already.
 */
ایستا درونخطی صحیح double_lock_balance(ساختار rq *this_rq, ساختار rq *busiest)
	__must_hold(__rq_lockp(this_rq))
	__acquires(__rq_lockp(busiest))
{
	lockdep_assert_irqs_disabled();

	بازگشت _double_lock_balance(this_rq, busiest);
}

ایستا درونخطی تهی double_unlock_balance(ساختار rq *this_rq, ساختار rq *busiest)
	__releases(__rq_lockp(busiest))
{
	اگر (__rq_lockp(this_rq) != __rq_lockp(busiest))
		raw_spin_rq_unlock(busiest);
	وگرنه
		__release(__rq_lockp(busiest)); /* fake release */
	lock_set_subclass(&__rq_lockp(this_rq)->dep_map, 0, _RET_IP_);
}

ایستا درونخطی تهی double_lock(spinlock_t *l1, spinlock_t *l2)
	__acquires(l1, l2)
{
	اگر (l1 > l2)
		swap(l1, l2);

	spin_lock(l1);
	spin_lock_nested(l2, SINGLE_DEPTH_NESTING);
}

ایستا درونخطی تهی double_lock_irq(spinlock_t *l1, spinlock_t *l2)
	__acquires(l1, l2)
{
	اگر (l1 > l2)
		swap(l1, l2);

	spin_lock_irq(l1);
	spin_lock_nested(l2, SINGLE_DEPTH_NESTING);
}

ایستا درونخطی تهی double_raw_lock(raw_spinlock_t *l1, raw_spinlock_t *l2)
	__acquires(l1, l2)
{
	اگر (l1 > l2)
		swap(l1, l2);

	raw_spin_lock(l1);
	raw_spin_lock_nested(l2, SINGLE_DEPTH_NESTING);
}

ایستا درونخطی تهی double_raw_unlock(raw_spinlock_t *l1, raw_spinlock_t *l2)
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
ایستا درونخطی تهی double_rq_unlock(ساختار rq *rq1, ساختار rq *rq2)
	__releases(__rq_lockp(rq1), __rq_lockp(rq2))
{
	اگر (__rq_lockp(rq1) != __rq_lockp(rq2))
		raw_spin_rq_unlock(rq2);
	وگرنه
		__release(__rq_lockp(rq2)); /* fake release */
	raw_spin_rq_unlock(rq1);
}

بیرونی تهی set_rq_online (ساختار rq *rq);
بیرونی تهی set_rq_offline(ساختار rq *rq);

بیرونی bool sched_smp_initialized;

DEFINE_LOCK_GUARD_2(double_rq_lock, ساختار rq,
		    double_rq_lock(_T->lock, _T->lock2),
		    double_rq_unlock(_T->lock, _T->lock2))

بیرونی ساختار sched_entity *__pick_root_entity(ساختار cfs_rq *cfs_rq);
بیرونی ساختار sched_entity *__pick_first_entity(ساختار cfs_rq *cfs_rq);
بیرونی ساختار sched_entity *__pick_last_entity(ساختار cfs_rq *cfs_rq);

بیرونی bool sched_debug_verbose;

بیرونی تهی print_cfs_stats(ساختار seq_file *m, صحیح cpu);
بیرونی تهی print_rt_stats(ساختار seq_file *m, صحیح cpu);
بیرونی تهی print_dl_stats(ساختار seq_file *m, صحیح cpu);
بیرونی تهی print_cfs_rq(ساختار seq_file *m, صحیح cpu, ساختار cfs_rq *cfs_rq);
بیرونی تهی print_rt_rq(ساختار seq_file *m, صحیح cpu, ساختار rt_rq *rt_rq);
بیرونی تهی print_dl_rq(ساختار seq_file *m, صحیح cpu, ساختار dl_rq *dl_rq);

بیرونی تهی resched_latency_warn(صحیح cpu, u64 latency);

#ifdef CONFIG_NUMA_BALANCING
بیرونی تهی show_numa_stats(ساختار task_struct *p, ساختار seq_file *m);
بیرونی تهی
print_numa_stats(ساختار seq_file *m, صحیح گره, unsigned long tsf,
		 unsigned long tpf, unsigned long gsf, unsigned long gpf);
#endif /* CONFIG_NUMA_BALANCING */

بیرونی تهی init_cfs_rq(ساختار cfs_rq *cfs_rq);
بیرونی تهی init_rt_rq(ساختار rt_rq *rt_rq);
بیرونی تهی init_dl_rq(ساختار dl_rq *dl_rq);

بیرونی تهی cfs_bandwidth_usage_inc(تهی);
بیرونی تهی cfs_bandwidth_usage_dec(تهی);

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

بیرونی تهی nohz_balance_exit_idle(ساختار rq *rq);
#وگرنه /* !CONFIG_NO_HZ_COMMON: */
ایستا درونخطی تهی nohz_balance_exit_idle(ساختار rq *rq) { }
#endif /* !CONFIG_NO_HZ_COMMON */

#ifdef CONFIG_NO_HZ_COMMON
بیرونی تهی nohz_run_idle_balance(صحیح cpu);
#وگرنه
ایستا درونخطی تهی nohz_run_idle_balance(صحیح cpu) { }
#endif

#شامل "stats.h"

#اگر defined(CONFIG_SCHED_CORE) && defined(CONFIG_SCHEDSTATS)

بیرونی تهی __sched_core_account_forceidle(ساختار rq *rq);

ایستا درونخطی تهی sched_core_account_forceidle(ساختار rq *rq)
{
	اگر (schedstat_enabled())
		__sched_core_account_forceidle(rq);
}

بیرونی تهی __sched_core_tick(ساختار rq *rq);

ایستا درونخطی تهی sched_core_tick(ساختار rq *rq)
{
	اگر (sched_core_enabled(rq) && schedstat_enabled())
		__sched_core_tick(rq);
}

#وگرنه /* !(CONFIG_SCHED_CORE && CONFIG_SCHEDSTATS): */

ایستا درونخطی تهی sched_core_account_forceidle(ساختار rq *rq) { }

ایستا درونخطی تهی sched_core_tick(ساختار rq *rq) { }

#endif /* !(CONFIG_SCHED_CORE && CONFIG_SCHEDSTATS) */

#ifdef CONFIG_IRQ_TIME_ACCOUNTING

ساختار irqtime {
	u64			total;
	u64			tick_delta;
	u64			irq_start_time;
	ساختار u64_stats_sync	sync;
};

DECLARE_PER_CPU(ساختار irqtime, cpu_irqtime);
DECLARE_STATIC_کلید_FALSE(sched_clock_irqtime);

ایستا درونخطی صحیح irqtime_enabled(تهی)
{
	بازگشت static_branch_likely(&sched_clock_irqtime);
}

/*
 * Returns the irqtime minus the softirq time computed by ksoftirqd.
 * Otherwise ksoftirqd's sum_exec_runtime is subtracted its own runtime
 * and never move forward.
 */
ایستا درونخطی u64 irq_time_read(صحیح cpu)
{
	ساختار irqtime *irqtime = &per_cpu(cpu_irqtime, cpu);
	unsigned صحیح seq;
	u64 total;

	انجامبده {
		seq = __u64_stats_fetch_begin(&irqtime->sync);
		total = irqtime->total;
	} وقتی (__u64_stats_fetch_retry(&irqtime->sync, seq));

	بازگشت total;
}

#وگرنه /* !CONFIG_IRQ_TIME_ACCOUNTING: */

ایستا درونخطی صحیح irqtime_enabled(تهی)
{
	بازگشت 0;
}

#endif /* !CONFIG_IRQ_TIME_ACCOUNTING */

#ifdef CONFIG_CPU_FREQ

DECLARE_PER_CPU(ساختار update_util_data __rcu *, cpufreq_update_util_data);

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
ایستا درونخطی تهی cpufreq_update_util(ساختار rq *rq, unsigned صحیح flags)
{
	ساختار update_util_data *data;

	data = rcu_dereference_sched(*per_cpu_ptr(&cpufreq_update_util_data,
						  cpu_of(rq)));
	اگر (data)
		data->func(data, rq_clock(rq), flags);
}
#وگرنه /* !CONFIG_CPU_FREQ: */
ایستا درونخطی تهی cpufreq_update_util(ساختار rq *rq, unsigned صحیح flags) { }
#endif /* !CONFIG_CPU_FREQ */

#ifdef arch_scale_freq_capacity
# ifndef arch_scale_freq_invariant
#  define arch_scale_freq_invariant()	true
# endif
#وگرنه
# define arch_scale_freq_invariant()	false
#endif

unsigned long effective_cpu_util(صحیح cpu, unsigned long util_cfs,
				 unsigned long *min,
				 unsigned long *max);

unsigned long sugov_effective_cpu_perf(صحیح cpu, unsigned long actual,
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
ایستا درونخطی bool dl_task_fits_capacity(ساختار task_struct *p, صحیح cpu)
{
	unsigned long cap = arch_scale_cpu_capacity(cpu);

	بازگشت cap >= p->dl.dl_density >> (BW_SHIFT - SCHED_CAPACITY_SHIFT);
}

ایستا درونخطی unsigned long cpu_bw_dl(ساختار rq *rq)
{
	بازگشت (rq->dl.running_bw * SCHED_CAPACITY_SCALE) >> BW_SHIFT;
}

ایستا درونخطی unsigned long cpu_util_dl(ساختار rq *rq)
{
	بازگشت READ_ONCE(rq->avg_dl.util_avg);
}


بیرونی unsigned long cpu_util_cfs(صحیح cpu);
بیرونی unsigned long cpu_util_cfs_boost(صحیح cpu);

ایستا درونخطی unsigned long cpu_util_rt(ساختار rq *rq)
{
	بازگشت READ_ONCE(rq->avg_rt.util_avg);
}

#ifdef CONFIG_UCLAMP_TASK

unsigned long uclamp_eff_مقدار(ساختار task_struct *p, شمارش uclamp_id clamp_id);

/*
 * When uclamp is compiled in, the aggregation at rq level is 'turned off'
 * by default in the fast path and only gets turned on once userspace performs
 * an operation that requires it.
 *
 * Returns true if userspace opted-in to use uclamp and aggregation at rq level
 * hence is active.
 */
ایستا درونخطی bool uclamp_is_used(تهی)
{
	بازگشت static_branch_likely(&sched_uclamp_used);
}

/*
 * Enabling static branches would get the cpus_read_lock(),
 * check whether uclamp_is_used before enable it to avoid always
 * calling cpus_read_lock(). Because we never disable this
 * static key once enable it.
 */
ایستا درونخطی تهی sched_uclamp_enable(تهی)
{
	اگر (!uclamp_is_used())
		static_branch_enable(&sched_uclamp_used);
}

ایستا درونخطی unsigned long uclamp_rq_get(ساختار rq *rq,
					  شمارش uclamp_id clamp_id)
{
	بازگشت READ_ONCE(rq->uclamp[clamp_id].مقدار);
}

ایستا درونخطی تهی uclamp_rq_set(ساختار rq *rq, شمارش uclamp_id clamp_id,
				 unsigned صحیح مقدار)
{
	WRITE_ONCE(rq->uclamp[clamp_id].مقدار, مقدار);
}

ایستا درونخطی bool uclamp_rq_is_idle(ساختار rq *rq)
{
	بازگشت rq->uclamp_flags & UCLAMP_FLAG_IDLE;
}

/* Is the rq being capped/throttled by uclamp_max? */
ایستا درونخطی bool uclamp_rq_is_capped(ساختار rq *rq)
{
	unsigned long rq_util;
	unsigned long max_util;

	اگر (!uclamp_is_used())
		بازگشت false;

	rq_util = cpu_util_cfs(cpu_of(rq)) + cpu_util_rt(rq);
	max_util = READ_ONCE(rq->uclamp[UCLAMP_MAX].مقدار);

	بازگشت max_util != SCHED_CAPACITY_SCALE && rq_util >= max_util;
}

#define for_each_clamp_id(clamp_id) \
	برای ((clamp_id) = 0; (clamp_id) < UCLAMP_CNT; (clamp_id)++)

بیرونی unsigned صحیح sysctl_sched_uclamp_util_min_rt_default;


ایستا درونخطی unsigned صحیح uclamp_none(شمارش uclamp_id clamp_id)
{
	اگر (clamp_id == UCLAMP_MIN)
		بازگشت 0;
	بازگشت SCHED_CAPACITY_SCALE;
}

/* Integer rounded range for each bucket */
#define UCLAMP_BUCKET_DELTA DIV_ROUND_CLOSEST(SCHED_CAPACITY_SCALE, UCLAMP_BUCKETS)

ایستا درونخطی unsigned صحیح uclamp_bucket_id(unsigned صحیح clamp_مقدار)
{
	بازگشت min_t(unsigned صحیح, clamp_مقدار / UCLAMP_BUCKET_DELTA, UCLAMP_BUCKETS - 1);
}

ایستا درونخطی تهی
uclamp_se_set(ساختار uclamp_se *uc_se, unsigned صحیح مقدار, bool user_defined)
{
	uc_se->مقدار = مقدار;
	uc_se->bucket_id = uclamp_bucket_id(مقدار);
	uc_se->user_defined = user_defined;
}

#وگرنه /* !CONFIG_UCLAMP_TASK: */

ایستا درونخطی unsigned long
uclamp_eff_مقدار(ساختار task_struct *p, شمارش uclamp_id clamp_id)
{
	اگر (clamp_id == UCLAMP_MIN)
		بازگشت 0;

	بازگشت SCHED_CAPACITY_SCALE;
}

ایستا درونخطی bool uclamp_rq_is_capped(ساختار rq *rq) { بازگشت false; }

ایستا درونخطی bool uclamp_is_used(تهی)
{
	بازگشت false;
}

ایستا درونخطی تهی sched_uclamp_enable(تهی) {}

ایستا درونخطی unsigned long
uclamp_rq_get(ساختار rq *rq, شمارش uclamp_id clamp_id)
{
	اگر (clamp_id == UCLAMP_MIN)
		بازگشت 0;

	بازگشت SCHED_CAPACITY_SCALE;
}

ایستا درونخطی تهی
uclamp_rq_set(ساختار rq *rq, شمارش uclamp_id clamp_id, unsigned صحیح مقدار)
{
}

ایستا درونخطی bool uclamp_rq_is_idle(ساختار rq *rq)
{
	بازگشت false;
}

#endif /* !CONFIG_UCLAMP_TASK */

#ifdef CONFIG_HAVE_SCHED_AVG_IRQ

ایستا درونخطی unsigned long cpu_util_irq(ساختار rq *rq)
{
	بازگشت READ_ONCE(rq->avg_irq.util_avg);
}

ایستا درونخطی
unsigned long scale_irq_capacity(unsigned long util, unsigned long irq, unsigned long max)
{
	util *= (max - irq);
	util /= max;

	بازگشت util;

}

#وگرنه /* !CONFIG_HAVE_SCHED_AVG_IRQ: */

ایستا درونخطی unsigned long cpu_util_irq(ساختار rq *rq)
{
	بازگشت 0;
}

ایستا درونخطی
unsigned long scale_irq_capacity(unsigned long util, unsigned long irq, unsigned long max)
{
	بازگشت util;
}

#endif /* !CONFIG_HAVE_SCHED_AVG_IRQ */

بیرونی تهی __setparam_fair(ساختار task_struct *p, ثابت ساختار sched_attr *attr);

#اگر defined(CONFIG_ENERGY_MODEL) && defined(CONFIG_CPU_FREQ_GOV_SCHEDUTIL)

#define perf_domain_span(pd) (to_cpumask(((pd)->em_pd->cpus)))

DECLARE_STATIC_کلید_FALSE(sched_energy_present);

ایستا درونخطی bool sched_energy_enabled(تهی)
{
	بازگشت static_branch_unlikely(&sched_energy_present);
}

#وگرنه /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL): */

#define perf_domain_span(pd) پوچ

ایستا درونخطی bool sched_energy_enabled(تهی) { بازگشت false; }

#endif /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL) */

#ifdef CONFIG_MEMBARRIER

/*
 * The scheduler provides memory barriers required by membarrier between:
 * - prior user-space memory accesses and store to rq->membarrier_state,
 * - store to rq->membarrier_state and following user-space memory accesses.
 * In the same way it provides those guarantees around store to rq->curr.
 */
ایستا درونخطی تهی membarrier_switch_mm(ساختار rq *rq,
					ساختار mm_struct *prev_mm,
					ساختار mm_struct *next_mm)
{
	صحیح membarrier_state;

	اگر (prev_mm == next_mm)
		بازگشت;

	membarrier_state = atomic_read(&next_mm->membarrier_state);
	اگر (READ_ONCE(rq->membarrier_state) == membarrier_state)
		بازگشت;

	WRITE_ONCE(rq->membarrier_state, membarrier_state);
}

#وگرنه /* !CONFIG_MEMBARRIER: */

ایستا درونخطی تهی membarrier_switch_mm(ساختار rq *rq,
					ساختار mm_struct *prev_mm,
					ساختار mm_struct *next_mm)
{
}

#endif /* !CONFIG_MEMBARRIER */

ایستا درونخطی bool is_per_cpu_kthread(ساختار task_struct *p)
{
	اگر (!(p->flags & PF_KTHREAD))
		بازگشت false;

	اگر (p->nr_cpus_allowed != 1)
		بازگشت false;

	بازگشت true;
}

بیرونی تهی swake_up_all_locked(ساختار swait_queue_head *q);
بیرونی تهی __prepare_to_swait(ساختار swait_queue_head *q, ساختار swait_queue *wait);

بیرونی صحیح try_to_wake_up(ساختار task_struct *tsk, unsigned صحیح state, صحیح wake_flags);

#ifdef CONFIG_PREEMPT_DYNAMIC
بیرونی صحیح preempt_dynamic_mode;
بیرونی صحیح sched_dynamic_mode(ثابت نویسه *str);
بیرونی تهی sched_dynamic_update(صحیح mode);
#endif
بیرونی ثابت نویسه *preempt_modes[];

#ifdef CONFIG_SCHED_MM_CID

ایستا __always_inline bool cid_on_cpu(unsigned صحیح cid)
{
	بازگشت cid & MM_CID_ONCPU;
}

ایستا __always_inline bool cid_in_transit(unsigned صحیح cid)
{
	بازگشت cid & MM_CID_TRANSIT;
}

ایستا __always_inline unsigned صحیح cpu_cid_to_cid(unsigned صحیح cid)
{
	بازگشت cid & ~MM_CID_ONCPU;
}

ایستا __always_inline unsigned صحیح cid_to_cpu_cid(unsigned صحیح cid)
{
	بازگشت cid | MM_CID_ONCPU;
}

ایستا __always_inline unsigned صحیح cid_to_transit_cid(unsigned صحیح cid)
{
	بازگشت cid | MM_CID_TRANSIT;
}

ایستا __always_inline unsigned صحیح cid_from_transit_cid(unsigned صحیح cid)
{
	بازگشت cid & ~MM_CID_TRANSIT;
}

ایستا __always_inline bool cid_on_task(unsigned صحیح cid)
{
	/* True if none of the MM_CID_ONCPU, MM_CID_TRANSIT, MM_CID_UNSET bits is set */
	بازگشت cid < MM_CID_TRANSIT;
}

ایستا __always_inline تهی mm_drop_cid(ساختار mm_struct *mm, unsigned صحیح cid)
{
	clear_bit(cid, mm_cidmask(mm));
}

ایستا __always_inline تهی mm_unset_cid_on_task(ساختار task_struct *t)
{
	unsigned صحیح cid = t->mm_cid.cid;

	t->mm_cid.cid = MM_CID_UNSET;
	اگر (cid_on_task(cid))
		mm_drop_cid(t->mm, cid);
}

ایستا __always_inline تهی mm_drop_cid_on_cpu(ساختار mm_struct *mm, ساختار mm_cid_pcpu *pcp)
{
	/* Clear the ONCPU bit, but do not set UNSET in the per CPU storage */
	اگر (cid_on_cpu(pcp->cid)) {
		pcp->cid = cpu_cid_to_cid(pcp->cid);
		mm_drop_cid(mm, pcp->cid);
	}
}

ایستا درونخطی unsigned صحیح __mm_get_cid(ساختار mm_struct *mm, unsigned صحیح max_cids)
{
	unsigned صحیح cid = find_first_zero_bit(mm_cidmask(mm), max_cids);

	اگر (cid >= max_cids)
		بازگشت MM_CID_UNSET;
	اگر (test_and_set_bit(cid, mm_cidmask(mm)))
		بازگشت MM_CID_UNSET;
	بازگشت cid;
}

ایستا درونخطی unsigned صحیح mm_get_cid(ساختار mm_struct *mm)
{
	unsigned صحیح cid = __mm_get_cid(mm, READ_ONCE(mm->mm_cid.max_cids));

	وقتی (cid == MM_CID_UNSET) {
		cpu_relax();
		cid = __mm_get_cid(mm, رقم_possible_cpus());
	}
	بازگشت cid;
}

ایستا درونخطی unsigned صحیح mm_cid_converge(ساختار mm_struct *mm, unsigned صحیح orig_cid,
					   unsigned صحیح max_cids)
{
	unsigned صحیح new_cid, cid = cpu_cid_to_cid(orig_cid);

	/* Is it in the optimal CID space? */
	اگر (likely(cid < max_cids))
		بازگشت orig_cid;

	/* Try to find one in the optimal space. Otherwise keep the provided. */
	new_cid = __mm_get_cid(mm, max_cids);
	اگر (new_cid != MM_CID_UNSET) {
		mm_drop_cid(mm, cid);
		/* Preserve the ONCPU mode of the original CID */
		بازگشت new_cid | (orig_cid & MM_CID_ONCPU);
	}
	بازگشت orig_cid;
}

ایستا __always_inline تهی mm_cid_update_task_cid(ساختار task_struct *t, unsigned صحیح cid)
{
	اگر (t->mm_cid.cid != cid) {
		t->mm_cid.cid = cid;
		rseq_sched_set_ids_changed(t);
	}
}

ایستا __always_inline تهی mm_cid_update_pcpu_cid(ساختار mm_struct *mm, unsigned صحیح cid)
{
	__this_cpu_write(mm->mm_cid.pcpu->cid, cid);
}

ایستا __always_inline تهی mm_cid_from_cpu(ساختار task_struct *t, unsigned صحیح cpu_cid,
					    unsigned صحیح mode)
{
	unsigned صحیح max_cids, tcid = t->mm_cid.cid;
	ساختار mm_struct *mm = t->mm;

	max_cids = READ_ONCE(mm->mm_cid.max_cids);
	/* Optimize for the common case where both have the ONCPU bit set */
	اگر (likely(cid_on_cpu(cpu_cid & tcid))) {
		اگر (likely(cpu_cid_to_cid(cpu_cid) < max_cids)) {
			mm_cid_update_task_cid(t, cpu_cid);
			بازگشت;
		}
		/* Try to converge into the optimal CID space */
		cpu_cid = mm_cid_converge(mm, cpu_cid, max_cids);
	} وگرنه {
		/* Hand over or drop the task owned CID */
		اگر (cid_on_task(tcid)) {
			اگر (cid_on_cpu(cpu_cid))
				mm_unset_cid_on_task(t);
			وگرنه
				cpu_cid = cid_to_cpu_cid(tcid);
		}
		/* Still nothing, allocate a new one */
		اگر (!cid_on_cpu(cpu_cid))
			cpu_cid = cid_to_cpu_cid(mm_get_cid(mm));

		/* Handle the transition mode flag if required */
		اگر (mode & MM_CID_TRANSIT)
			cpu_cid = cpu_cid_to_cid(cpu_cid) | MM_CID_TRANSIT;
	}
	mm_cid_update_pcpu_cid(mm, cpu_cid);
	mm_cid_update_task_cid(t, cpu_cid);
}

ایستا __always_inline تهی mm_cid_from_task(ساختار task_struct *t, unsigned صحیح cpu_cid,
					     unsigned صحیح mode)
{
	unsigned صحیح max_cids, tcid = t->mm_cid.cid;
	ساختار mm_struct *mm = t->mm;

	max_cids = READ_ONCE(mm->mm_cid.max_cids);
	/* Optimize for the common case, where both have the ONCPU bit clear */
	اگر (likely(cid_on_task(tcid | cpu_cid))) {
		اگر (likely(tcid < max_cids)) {
			mm_cid_update_pcpu_cid(mm, tcid);
			بازگشت;
		}
		/* Try to converge into the optimal CID space */
		tcid = mm_cid_converge(mm, tcid, max_cids);
	} وگرنه {
		/* Hand over or drop the CPU owned CID */
		اگر (cid_on_cpu(cpu_cid)) {
			اگر (cid_on_task(tcid))
				mm_drop_cid_on_cpu(mm, this_cpu_ptr(mm->mm_cid.pcpu));
			وگرنه
				tcid = cpu_cid_to_cid(cpu_cid);
		}
		/* Still nothing, allocate a new one */
		اگر (!cid_on_task(tcid))
			tcid = mm_get_cid(mm);
		/* Set the transition mode flag if required */
		tcid |= mode & MM_CID_TRANSIT;
	}
	mm_cid_update_pcpu_cid(mm, tcid);
	mm_cid_update_task_cid(t, tcid);
}

ایستا __always_inline تهی mm_cid_schedin(ساختار task_struct *next)
{
	ساختار mm_struct *mm = next->mm;
	unsigned صحیح cpu_cid, mode;

	اگر (!next->mm_cid.active)
		بازگشت;

	cpu_cid = __this_cpu_read(mm->mm_cid.pcpu->cid);
	mode = READ_ONCE(mm->mm_cid.mode);
	اگر (likely(!cid_on_cpu(mode)))
		mm_cid_from_task(next, cpu_cid, mode);
	وگرنه
		mm_cid_from_cpu(next, cpu_cid, mode);
}

ایستا __always_inline تهی mm_cid_schedout(ساختار task_struct *prev)
{
	ساختار mm_struct *mm = prev->mm;
	unsigned صحیح mode, cid;

	/* During mode transitions CIDs are temporary and need to be dropped */
	اگر (likely(!cid_in_transit(prev->mm_cid.cid)))
		بازگشت;

	mode = READ_ONCE(mm->mm_cid.mode);
	cid = cid_from_transit_cid(prev->mm_cid.cid);

	/*
	 * If transition mode is done, transfer ownership when the CID is
	 * within the convergence range to optimize the next schedule in.
	 */
	اگر (!cid_in_transit(mode) && cid < READ_ONCE(mm->mm_cid.max_cids)) {
		اگر (cid_on_cpu(mode))
			cid = cid_to_cpu_cid(cid);

		/* Update both so that the next schedule in goes into the fast path */
		mm_cid_update_pcpu_cid(mm, cid);
		prev->mm_cid.cid = cid;
	} وگرنه {
		mm_drop_cid(mm, cid);
		prev->mm_cid.cid = MM_CID_UNSET;
	}
}

ایستا درونخطی تهی mm_cid_switch_to(ساختار task_struct *prev, ساختار task_struct *next)
{
	mm_cid_schedout(prev);
	mm_cid_schedin(next);
}

#وگرنه /* !CONFIG_SCHED_MM_CID: */
ایستا درونخطی تهی mm_cid_switch_to(ساختار task_struct *prev, ساختار task_struct *next) { }
#endif /* !CONFIG_SCHED_MM_CID */

#ifdef CONFIG_SCHED_CACHE
DECLARE_STATIC_کلید_FALSE(sched_cache_present);
DECLARE_STATIC_کلید_FALSE(sched_cache_active);
بیرونی صحیح sysctl_sched_cache_user;
بیرونی unsigned صحیح llc_aggr_tolerance;
بیرونی unsigned صحیح llc_epoch_period;
بیرونی unsigned صحیح llc_epoch_affinity_timeout;
بیرونی unsigned صحیح llc_imb_pct;
بیرونی unsigned صحیح llc_overaggr_pct;

ایستا درونخطی bool sched_cache_enabled(تهی)
{
	بازگشت static_branch_unlikely(&sched_cache_active);
}

بیرونی تهی sched_cache_active_set(تهی);

#endif

تهی sched_domains_free_llc_id(صحیح cpu);

بیرونی تهی init_sched_mm(ساختار task_struct *p);

بیرونی u64 avg_vruntime(ساختار cfs_rq *cfs_rq);
بیرونی صحیح entity_eligible(ساختار cfs_rq *cfs_rq, ساختار sched_entity *se);
ایستا درونخطی
تهی move_queued_task_locked(ساختار rq *src_rq, ساختار rq *dst_rq, ساختار task_struct *task)
{
	lockdep_assert_rq_held(src_rq);
	lockdep_assert_rq_held(dst_rq);

	deactivate_task(src_rq, task, 0);
	set_task_cpu(task, dst_rq->cpu);
	activate_task(dst_rq, task, 0);
	wakeup_preempt(dst_rq, task, 0);
}

ایستا درونخطی
bool task_is_pushable(ساختار rq *rq, ساختار task_struct *p, صحیح cpu)
{
	اگر (!task_on_cpu(rq, p) &&
	    cpumask_test_cpu(cpu, &p->cpus_mask))
		بازگشت true;

	بازگشت false;
}

#ifdef CONFIG_RT_MUTEXES

ایستا درونخطی صحیح __rt_effective_prio(ساختار task_struct *pi_task, صحیح prio)
{
	اگر (pi_task)
		prio = min(prio, pi_task->prio);

	بازگشت prio;
}

ایستا درونخطی صحیح rt_effective_prio(ساختار task_struct *p, صحیح prio)
{
	ساختار task_struct *pi_task = rt_mutex_get_بالاترین_task(p);

	بازگشت __rt_effective_prio(pi_task, prio);
}

#وگرنه /* !CONFIG_RT_MUTEXES: */

ایستا درونخطی صحیح rt_effective_prio(ساختار task_struct *p, صحیح prio)
{
	بازگشت prio;
}

#endif /* !CONFIG_RT_MUTEXES */

بیرونی صحیح __sched_setscheduler(ساختار task_struct *p, ثابت ساختار sched_attr *attr, bool user, bool pi);
بیرونی صحیح __sched_setaffinity(ساختار task_struct *p, ساختار affinity_context *ctx);
بیرونی ثابت ساختار sched_class *__setscheduler_class(صحیح policy, صحیح prio);
بیرونی تهی set_load_weight(ساختار task_struct *p, bool update_load);
بیرونی تهی enqueue_task(ساختار rq *rq, ساختار task_struct *p, صحیح flags);
بیرونی bool dequeue_task(ساختار rq *rq, ساختار task_struct *p, صحیح flags);

بیرونی ساختار balance_callback *splice_balance_callbacks(ساختار rq *rq);

بیرونی تهی __balance_callbacks(ساختار rq *rq, ساختار rq_flags *rf);
بیرونی تهی balance_callbacks(ساختار rq *rq, ساختار balance_callback *head);

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
ساختار sched_change_ctx {
	u64			prio;
	ساختار task_struct	*p;
	ثابت ساختار sched_class *class;
	صحیح			flags;
	bool			queued;
	bool			running;
};

ساختار sched_change_ctx *sched_change_begin(ساختار task_struct *p, unsigned صحیح flags);
تهی sched_change_end(ساختار sched_change_ctx *ctx);

DEFINE_CLASS(sched_change, ساختار sched_change_ctx *,
	     sched_change_end(_T),
	     sched_change_begin(p, flags),
	     ساختار task_struct *p, unsigned صحیح flags)

DEFINE_CLASS_IS_UNCONDITIONAL(sched_change)

#شامل "ext/ext.h"

#ifdef CONFIG_PREFERRED_CPU
تهی sched_افزودن_current_non_preferred_cpu(ساختار rq *rq);
#وگرنه	/* !CONFIG_PREFERRED_CPU */
ایستا درونخطی تهی sched_افزودن_current_non_preferred_cpu(ساختار rq *rq) { }
#endif

#endif /* _KERNEL_SCHED_SCHED_H */
