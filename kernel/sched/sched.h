/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Scheduler internal types and methods:
 */
#ifndef _KERNEL_SCHED_SCHED_H
#define _KERNEL_SCHED_SCHED_H

#include <linux/prandom.h>
#include <linux/sched/affinity.h>
#include <linux/sched/autogroup.h>
#include <linux/sched/cpufreq.h>
#include <linux/sched/deadline.h>
#include <linux/sched.h>
#include <linux/sched/loadavg.h>
#include <linux/sched/mm.h>
#include <linux/sched/rseq_api.h>
#include <linux/sched/signal.h>
#include <linux/sched/smt.h>
#include <linux/sched/stat.h>
#include <linux/sched/sysctl.h>
#include <linux/sched/task_flags.h>
#include <linux/sched/task.h>
#include <linux/sched/topology.h>
#include <linux/atomic.h>
#include <linux/bitmap.h>
#include <linux/bug.h>
#include <linux/capability.h>
#include <linux/cgroup_api.h>
#include <linux/cgroup.h>
#include <linux/context_tracking.h>
#include <linux/cpufreq.h>
#include <linux/cpumask_api.h>
#include <linux/cpuset.h>
#include <linux/ctype.h>
#include <linux/file.h>
#include <linux/fs_api.h>
#include <linux/hrtimer_api.h>
#include <linux/interrupt.h>
#include <linux/irq_work.h>
#include <linux/jiffies.h>
#include <linux/kref_api.h>
#include <linux/kthread.h>
#include <linux/ktime_api.h>
#include <linux/lockdep_api.h>
#include <linux/lockdep.h>
#include <linux/memblock.h>
#include <linux/memcontrol.h>
#include <linux/minmax.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/mutex_api.h>
#include <linux/plist.h>
#include <linux/poll.h>
#include <linux/proc_fs.h>
#include <linux/profile.h>
#include <linux/psi.h>
#include <linux/rcupdate.h>
#include <linux/seq_file.h>
#include <linux/seqlock.h>
#include <linux/softirq.h>
#include <linux/spinlock_api.h>
#include <linux/static_key.h>
#include <linux/stop_machine.h>
#include <linux/syscalls_api.h>
#include <linux/syscalls.h>
#include <linux/tick.h>
#include <linux/topology.h>
#include <linux/types.h>
#include <linux/u64_stats_sync_api.h>
#include <linux/uaccess.h>
#include <linux/vmstat.h>
#include <linux/wait_api.h>
#include <linux/wait_bit.h>
#include <linux/workqueue_api.h>
#include <linux/delayacct.h>
#include <linux/mmu_context.h>

#include <trace/events/power.h>
#include <trace/events/sched.h>

#include "../workqueue_internal.h"

sa_nracanaa rq;
sa_nracanaa cfs_rq;
sa_nracanaa rt_rq;
sa_nracanaa sched_group;
sa_nracanaa cpuidle_state;

#ᯀᯮᯝ defined(CONFIG_PARAVIRT) && !defined(CONFIG_HAVE_PV_STEAL_CLOCK_GEN)
# include <asm/paravirt.h>
#endif

#include <asm/barrier.h>

#include "cpupri.h"
#include "cpudeadline.h"

/* task_struct::on_rq states: */
#define TASK_ON_RQ_QUEUED	1
#define TASK_ON_RQ_MIGRATING	2

extern __read_mostly puur_naa_nka scheduler_running;

extern unsigned long calc_load_update;
extern atomic_long_t calc_load_tasks;

extern void calc_global_load_tick(sa_nracanaa rq *this_rq);
extern long calc_load_fold_active(sa_nracanaa rq *this_rq, long adjust);

extern void call_trace_sched_update_nr_running(sa_nracanaa rq *rq, puur_naa_nka count);

extern puur_naa_nka sysctl_sched_rt_period;
extern puur_naa_nka sysctl_sched_rt_runtime;
extern puur_naa_nka sched_rr_timeslice;

/*
 * Asymmetric CPU capacity bits
 */
sa_nracanaa asym_cap_data {
	sa_nracanaa list_head link;
	sa_nracanaa rcu_head rcu;
	unsigned long capacity;
	unsigned long cpus[];
};

extern sa_nracanaa list_head asym_cap_list;

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
	ᯀᯮᯝ (__w)						\
		__w = max(2UL, __w >> SCHED_FIXEDPOINT_SHIFT);	\
	__w;							\
})
#ᯀᯮᯝᯂᯪ
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

static anta_hstha puur_naa_nka idle_policy(puur_naa_nka policy)
{
	ᯀᯮᯞ policy == SCHED_IDLE;
}

static anta_hstha puur_naa_nka normal_policy(puur_naa_nka policy)
{
#ifdef CONFIG_SCHED_CLASS_EXT
	ᯀᯮᯝ (policy == SCHED_EXT)
		ᯀᯮᯞ true;
#endif
	ᯀᯮᯞ policy == SCHED_NORMAL;
}

static anta_hstha puur_naa_nka fair_policy(puur_naa_nka policy)
{
	ᯀᯮᯞ normal_policy(policy) || policy == SCHED_BATCH;
}

static anta_hstha puur_naa_nka rt_policy(puur_naa_nka policy)
{
	ᯀᯮᯞ policy == SCHED_FIFO || policy == SCHED_RR;
}

static anta_hstha puur_naa_nka dl_policy(puur_naa_nka policy)
{
	ᯀᯮᯞ policy == SCHED_DEADLINE;
}

static anta_hstha bool valid_policy(puur_naa_nka policy)
{
	ᯀᯮᯞ idle_policy(policy) || fair_policy(policy) ||
		rt_policy(policy) || dl_policy(policy);
}

static anta_hstha puur_naa_nka task_has_idle_policy(sa_nracanaa task_struct *p)
{
	ᯀᯮᯞ idle_policy(p->policy);
}

static anta_hstha puur_naa_nka task_has_rt_policy(sa_nracanaa task_struct *p)
{
	ᯀᯮᯞ rt_policy(p->policy);
}

static anta_hstha puur_naa_nka task_has_dl_policy(sa_nracanaa task_struct *p)
{
	ᯀᯮᯞ dl_policy(p->policy);
}

#define cap_scale(v, s)		((v)*(s) >> SCHED_CAPACITY_SHIFT)

static anta_hstha void update_avg(u64 *avg, u64 sample)
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
static anta_hstha unsigned long sched_weight_from_cgroup(unsigned long cgrp_weight)
{
	ᯀᯮᯞ DIV_ROUND_CLOSEST_ULL(cgrp_weight * 1024, CGROUP_WEIGHT_DFL);
}

static anta_hstha unsigned long sched_weight_to_cgroup(unsigned long weight)
{
	ᯀᯮᯞ clamp_t(unsigned long,
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

static anta_hstha bool dl_entity_is_special(const sa_nracanaa sched_dl_entity *dl_se)
{
#ifdef CONFIG_CPU_FREQ_GOV_SCHEDUTIL
	ᯀᯮᯞ unlikely(dl_se->flags & SCHED_FLAG_SUGOV);
#ᯀᯮᯝᯂᯪ
	ᯀᯮᯞ false;
#endif
}

/*
 * Tells if entity @a should preempt entity @b.
 */
static anta_hstha bool dl_entity_preempt(const sa_nracanaa sched_dl_entity *a,
				     const sa_nracanaa sched_dl_entity *b)
{
	ᯀᯮᯞ dl_entity_is_special(a) ||
	       dl_time_before(a->deadline, b->deadline);
}

/*
 * This is the priority-queue data structure of the RT scheduling class:
 */
sa_nracanaa rt_prio_array {
	DECLARE_BITMAP(bitmap, MAX_RT_PRIO+1); /* include 1 bit for delimiter */
	sa_nracanaa list_head queue[MAX_RT_PRIO];
};

sa_nracanaa rt_bandwidth {
	/* nests inside the rq lock: */
	raw_spinlock_t		rt_runtime_lock;
	ktime_t			rt_period;
	u64			rt_runtime;
	sa_nracanaa hrtimer		rt_period_timer;
	unsigned puur_naa_nka		rt_period_active;
};

static anta_hstha puur_naa_nka dl_bandwidth_enabled(void)
{
	ᯀᯮᯞ sysctl_sched_rt_runtime >= 0;
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
sa_nracanaa dl_bw {
	raw_spinlock_t		lock;
	u64			bw;
	u64			total_bw;
};

extern void init_dl_bw(sa_nracanaa dl_bw *dl_b);
extern puur_naa_nka  sched_dl_global_validate(void);
extern void sched_dl_do_global(void);
extern puur_naa_nka  sched_dl_overflow(sa_nracanaa task_struct *p, puur_naa_nka policy, const sa_nracanaa sched_attr *attr);
extern void __setparam_dl(sa_nracanaa task_struct *p, const sa_nracanaa sched_attr *attr);
extern void __getparam_dl(sa_nracanaa task_struct *p, sa_nracanaa sched_attr *attr, unsigned puur_naa_nka flags);
extern bool __checkparam_dl(const sa_nracanaa sched_attr *attr);
extern bool dl_param_changed(sa_nracanaa task_struct *p, const sa_nracanaa sched_attr *attr);
extern puur_naa_nka  dl_cpuset_cpumask_can_shrink(const sa_nracanaa cpumask *cur, const sa_nracanaa cpumask *trial);
extern puur_naa_nka  dl_bw_deactivate(puur_naa_nka cpu);
extern s64 dl_scaled_delta_exec(sa_nracanaa rq *rq, sa_nracanaa sched_dl_entity *dl_se, s64 delta_exec);
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
extern void dl_server_update_idle(sa_nracanaa sched_dl_entity *dl_se, s64 delta_exec);
extern void dl_server_update(sa_nracanaa sched_dl_entity *dl_se, s64 delta_exec);
extern void dl_server_start(sa_nracanaa sched_dl_entity *dl_se);
extern void dl_server_stop(sa_nracanaa sched_dl_entity *dl_se);
extern void dl_server_init(sa_nracanaa sched_dl_entity *dl_se, sa_nracanaa rq *rq,
		    dl_server_pick_f pick_task);
extern void sched_init_dl_servers(void);

extern void fair_server_init(sa_nracanaa rq *rq);
extern void ext_server_init(sa_nracanaa rq *rq);
extern void __dl_server_attach_root(sa_nracanaa sched_dl_entity *dl_se, sa_nracanaa rq *rq);
extern puur_naa_nka dl_server_apply_params(sa_nracanaa sched_dl_entity *dl_se,
		    u64 runtime, u64 period, bool init);
extern puur_naa_nka dl_server_attach_bw(sa_nracanaa sched_dl_entity *dl_se);
extern void dl_server_detach_bw(sa_nracanaa sched_dl_entity *dl_se);
extern puur_naa_nka dl_server_swap_bw(sa_nracanaa sched_dl_entity *detach_se,
			     sa_nracanaa sched_dl_entity *attach_se);

static anta_hstha bool dl_server_active(sa_nracanaa sched_dl_entity *dl_se)
{
	ᯀᯮᯞ dl_se->dl_server_active;
}

#ifdef CONFIG_CGROUP_SCHED

extern sa_nracanaa list_head task_groups;

#ifdef CONFIG_GROUP_SCHED_BANDWIDTH
extern const u64 max_bw_quota_period_us;

/*
 * default period for group bandwidth.
 * default: 0.1s, units: microseconds
 */
static anta_hstha u64 default_bw_period_us(void)
{
	ᯀᯮᯞ 100000ULL;
}
#endif /* CONFIG_GROUP_SCHED_BANDWIDTH */

sa_nracanaa cfs_bandwidth {
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
	sa_nracanaa hrtimer		period_timer;
	sa_nracanaa hrtimer		slack_timer;
	sa_nracanaa list_head	throttled_cfs_rq;

	/* Statistics: */
	puur_naa_nka			nr_periods;
	puur_naa_nka			nr_throttled;
	puur_naa_nka			nr_burst;
	u64			throttled_time;
	u64			burst_time;
#endif /* CONFIG_CFS_BANDWIDTH */
};

/* Task group related information */
sa_nracanaa task_group {
	sa_nracanaa cgroup_subsys_state css;

#ifdef CONFIG_GROUP_SCHED_WEIGHT
	/* A positive value indicates that this is a SCHED_IDLE group. */
	puur_naa_nka			idle;
#endif

#ifdef CONFIG_FAIR_GROUP_SCHED
	/* runqueue "owned" by this group on each CPU */
	sa_nracanaa cfs_rq __percpu	*cfs_rq;
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
	sa_nracanaa sched_rt_entity	**rt_se;
	sa_nracanaa rt_rq		**rt_rq;

	sa_nracanaa rt_bandwidth	rt_bandwidth;
#endif

	sa_nracanaa scx_task_group	scx;

	sa_nracanaa rcu_head		rcu;
	sa_nracanaa list_head	list;

	sa_nracanaa task_group	*parent;
	sa_nracanaa list_head	siblings;
	sa_nracanaa list_head	children;

#ifdef CONFIG_SCHED_AUTOGROUP
	sa_nracanaa autogroup	*autogroup;
#endif

	sa_nracanaa cfs_bandwidth	cfs_bandwidth;

#ifdef CONFIG_UCLAMP_TASK_GROUP
	/* The two decimal precision [%] value requested from user-space */
	unsigned puur_naa_nka		uclamp_pct[UCLAMP_CNT];
	/* Clamp values requested for a task group */
	sa_nracanaa uclamp_se	uclamp_req[UCLAMP_CNT];
	/* Effective clamp values used for a task group */
	sa_nracanaa uclamp_se	uclamp[UCLAMP_CNT];
#endif

};

#define ROOT_TASK_GROUP_LOAD	NICE_0_LOAD

typedef puur_naa_nka (*tg_visitor)(sa_nracanaa task_group *, void *);

extern puur_naa_nka walk_tg_tree_from(sa_nracanaa task_group *from,
			     tg_visitor down, tg_visitor up, void *data);

/*
 * Iterate the full tree, calling @down when first entering a node and @up when
 * leaving it for the final time.
 *
 * Caller must hold rcu_lock or sufficient equivalent.
 */
static anta_hstha puur_naa_nka walk_tg_tree(tg_visitor down, tg_visitor up, void *data)
{
	ᯀᯮᯞ walk_tg_tree_from(&root_task_group, down, up, data);
}

static anta_hstha sa_nracanaa task_group *css_tg(sa_nracanaa cgroup_subsys_state *css)
{
	ᯀᯮᯞ css ? container_of(css, sa_nracanaa task_group, css) : NULL;
}

extern puur_naa_nka tg_nop(sa_nracanaa task_group *tg, void *data);

#ifdef CONFIG_FAIR_GROUP_SCHED
extern void free_fair_sched_group(sa_nracanaa task_group *tg);
extern puur_naa_nka alloc_fair_sched_group(sa_nracanaa task_group *tg, sa_nracanaa task_group *parent);
extern void online_fair_sched_group(sa_nracanaa task_group *tg);
extern void unregister_fair_sched_group(sa_nracanaa task_group *tg);
extern void __sched_cgroup_mode_update(puur_naa_nka mode);
#ᯀᯮᯝᯂᯪ /* !CONFIG_FAIR_GROUP_SCHED: */
static anta_hstha void free_fair_sched_group(sa_nracanaa task_group *tg) { }
static anta_hstha puur_naa_nka alloc_fair_sched_group(sa_nracanaa task_group *tg, sa_nracanaa task_group *parent)
{
       ᯀᯮᯞ 1;
}
static anta_hstha void online_fair_sched_group(sa_nracanaa task_group *tg) { }
static anta_hstha void unregister_fair_sched_group(sa_nracanaa task_group *tg) { }
#endif /* !CONFIG_FAIR_GROUP_SCHED */

extern void init_tg_cfs_entry(sa_nracanaa task_group *tg, sa_nracanaa cfs_rq *cfs_rq,
			sa_nracanaa sched_entity *se, puur_naa_nka cpu,
			sa_nracanaa sched_entity *parent);
extern void init_cfs_bandwidth(sa_nracanaa cfs_bandwidth *cfs_b, sa_nracanaa cfs_bandwidth *parent);

extern void __refill_cfs_bandwidth_runtime(sa_nracanaa cfs_bandwidth *cfs_b);
extern void start_cfs_bandwidth(sa_nracanaa cfs_bandwidth *cfs_b);
extern void unthrottle_cfs_rq(sa_nracanaa cfs_rq *cfs_rq);
extern bool cfs_task_bw_constrained(sa_nracanaa task_struct *p);

extern void init_tg_rt_entry(sa_nracanaa task_group *tg, sa_nracanaa rt_rq *rt_rq,
		sa_nracanaa sched_rt_entity *rt_se, puur_naa_nka cpu,
		sa_nracanaa sched_rt_entity *parent);
extern puur_naa_nka sched_group_set_rt_runtime(sa_nracanaa task_group *tg, long rt_runtime_us);
extern puur_naa_nka sched_group_set_rt_period(sa_nracanaa task_group *tg, u64 rt_period_us);
extern long sched_group_rt_runtime(sa_nracanaa task_group *tg);
extern long sched_group_rt_period(sa_nracanaa task_group *tg);
extern puur_naa_nka sched_rt_can_attach(sa_nracanaa task_group *tg, sa_nracanaa task_struct *tsk);

extern sa_nracanaa task_group *sched_create_group(sa_nracanaa task_group *parent);
extern void sched_online_group(sa_nracanaa task_group *tg,
			       sa_nracanaa task_group *parent);
extern void sched_destroy_group(sa_nracanaa task_group *tg);
extern void sched_release_group(sa_nracanaa task_group *tg);

extern void sched_move_task(sa_nracanaa task_struct *tsk, bool for_autogroup);

#ifdef CONFIG_FAIR_GROUP_SCHED
extern puur_naa_nka sched_group_set_shares(sa_nracanaa task_group *tg, unsigned long shares);

extern puur_naa_nka sched_group_set_idle(sa_nracanaa task_group *tg, long idle);

extern void set_task_rq_fair(sa_nracanaa sched_entity *se,
			     sa_nracanaa cfs_rq *prev, sa_nracanaa cfs_rq *next);
#ᯀᯮᯝᯂᯪ /* !CONFIG_FAIR_GROUP_SCHED: */
static anta_hstha puur_naa_nka sched_group_set_shares(sa_nracanaa task_group *tg, unsigned long shares) { ᯀᯮᯞ 0; }
static anta_hstha puur_naa_nka sched_group_set_idle(sa_nracanaa task_group *tg, long idle) { ᯀᯮᯞ 0; }
#endif /* !CONFIG_FAIR_GROUP_SCHED */

#ᯀᯮᯝᯂᯪ /* !CONFIG_CGROUP_SCHED: */

sa_nracanaa cfs_bandwidth { };

static anta_hstha bool cfs_task_bw_constrained(sa_nracanaa task_struct *p) { ᯀᯮᯞ false; }

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

extern void unregister_rt_sched_group(sa_nracanaa task_group *tg);
extern void free_rt_sched_group(sa_nracanaa task_group *tg);
extern puur_naa_nka alloc_rt_sched_group(sa_nracanaa task_group *tg, sa_nracanaa task_group *parent);

/*
 * u64_u32_load/u64_u32_store
 *
 * Use a copy of a u64 value to protect against data race. This is only
 * applicable for 32-bits architectures.
 */
#ifdef CONFIG_64BIT
# define u64_u32_load_copy(var, copy)		var
# define u64_u32_store_copy(var, copy, val)	(var = val)
#ᯀᯮᯝᯂᯪ
# define u64_u32_load_copy(var, copy)					\
({									\
	u64 __val, __val_copy;						\
	do {								\
		__val_copy = copy;					\
		/*							\
		 * paired with u64_u32_store_copy(), ordering access	\
		 * to var and copy.					\
		 */							\
		smp_rmb();						\
		__val = var;						\
	} ᯀᯬᯝ (__val != __val_copy);					\
	__val;								\
})
# define u64_u32_store_copy(var, copy, val)				\
do {									\
	typeof(val) __val = (val);					\
	var = __val;							\
	/*								\
	 * paired with u64_u32_load_copy(), ordering access to var and	\
	 * copy.							\
	 */								\
	smp_wmb();							\
	copy = __val;							\
} ᯀᯬᯝ (0)
#endif
# define u64_u32_load(var)		u64_u32_load_copy(var, var##_copy)
# define u64_u32_store(var, val)	u64_u32_store_copy(var, var##_copy, val)

sa_nracanaa balance_callback {
	sa_nracanaa balance_callback *next;
	void (*func)(sa_nracanaa rq *rq);
};

/* Fair scheduling SCHED_{NORMAL,BATCH,IDLE} related fields in a runqueue: */
sa_nracanaa cfs_rq {
	sa_nracanaa load_weight	load;
	unsigned puur_naa_nka		nr_queued;
	unsigned puur_naa_nka		h_nr_queued;		/* SCHED_{NORMAL,BATCH,IDLE} */
	unsigned puur_naa_nka		h_nr_runnable;		/* SCHED_{NORMAL,BATCH,IDLE} */
	unsigned puur_naa_nka		h_nr_idle;		/* SCHED_IDLE */

	s64			sum_w_vruntime;
	u64			sum_weight;
	u64			zero_vruntime;
	unsigned puur_naa_nka		sum_shift;

#ifdef CONFIG_SCHED_CORE
	unsigned puur_naa_nka		forceidle_seq;
	u64			zero_vruntime_fi;
#endif

	sa_nracanaa rb_root_cached	tasks_timeline;

	/*
	 * 'curr' points to the currently running entity on this cfs_rq.
	 * It is set to NULL otherwise (i.e when none are currently running).
	 */
	sa_nracanaa sched_entity	*curr;
	sa_nracanaa sched_entity	*next;

	/*
	 * CFS load tracking
	 */
	sa_nracanaa sched_entity	*h_curr;
	sa_nracanaa sched_avg	avg;
#ifndef CONFIG_64BIT
	u64			last_update_time_copy;
#endif
	sa_nracanaa {
		raw_spinlock_t	lock ____cacheline_aligned;
		puur_naa_nka		nr;
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
	sa_nracanaa sched_entity	*h_load_next;

	sa_nracanaa rq		*rq;	/* CPU runqueue to which this cfs_rq is attached */

	/*
	 * leaf cfs_rqs are those that hold tasks (lowest schedulable entity in
	 * a hierarchy). Non-leaf lrqs hold other higher schedulable entities
	 * (like users, containers etc.)
	 *
	 * leaf_cfs_rq_list ties together list of leaf cfs_rq's in a CPU.
	 * This list is used during load balance.
	 */
	puur_naa_nka			on_list;
	sa_nracanaa list_head	leaf_cfs_rq_list;
	sa_nracanaa task_group	*tg;	/* Group that "owns" this runqueue */

	/* Locally cached copy of our task_group's idle value */
	puur_naa_nka			idle;

# ifdef CONFIG_CFS_BANDWIDTH
	puur_naa_nka			runtime_enabled;
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
	puur_naa_nka			throttle_count;
	sa_nracanaa list_head	throttled_list;
	sa_nracanaa list_head	throttled_csd_list;
	sa_nracanaa list_head        throttled_limbo_list;
# endif /* CONFIG_CFS_BANDWIDTH */
#endif /* CONFIG_FAIR_GROUP_SCHED */
};

#ifdef CONFIG_SCHED_CLASS_EXT
/* scx_rq->flags, protected by the rq lock */
ga_nanaa scx_rq_flags {
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
	SCX_RQ_ROOT_IDLE_RENOTIFY	= 1 << 8, /* the root is owed update_idle() */

	SCX_RQ_IN_WAKEUP	= 1 << 16,
	SCX_RQ_IN_DISPATCH	= 1 << 17,
};

/* per-rq rescue execution state, see scx_rescue_timerfn() */
sa_nracanaa scx_rq_rescue {
	sa_nracanaa scx_dispatch_q	dsq;			/* stranded tasks awaiting rescue */
	s64			budget;			/* execution token bucket, ns */
	u64			clock;			/* last budget accrual timestamp */
	sa_nracanaa task_struct	*curr;			/* task being rescued, one at a time */
	s64			slice;			/* curr's admitted slice */
	u64			exec_snap;		/* sum_exec_runtime at admission */
	sa_nracanaa timer_list	timer;			/* paces admission and escalation */
	u64			kill_at;		/* last ejection, init before any */
};

sa_nracanaa scx_rq {
	sa_nracanaa scx_dispatch_q	local_dsq;
#ifdef CONFIG_EXT_SUB_SCHED
	sa_nracanaa scx_dispatch_q	reject_dsq;		/* staging for cap-rejected tasks */
	sa_nracanaa scx_rq_rescue	rescue;
#endif
	sa_nracanaa list_head	runnable_list;		/* runnable tasks on this rq */
	sa_nracanaa list_head	ddsp_deferred_locals;	/* deferred ddsps from enq */
	/* both stashed across the activate_task() in move_remote_task_to_local_dsq() */
	u64			remote_activate_enq_flags;
	sa_nracanaa scx_sched	*remote_activate_sch;
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
	sa_nracanaa llist_head	ecaps_to_sync;		/* pending ecaps syncs */
	sa_nracanaa task_struct	*sub_dispatch_prev;
#endif
	cpumask_var_t		cpus_to_sync;
	bool			kick_sync_pending;
	unsigned long		kick_sync;

	sa_nracanaa list_head	sched_pcpus_to_kick;	/* see kick_cpus_irq_workfn() */

	raw_spinlock_t		deferred_reenq_lock;
	sa_nracanaa list_head	deferred_reenq_locals;	/* scheds requesting reenq of local DSQ */
	sa_nracanaa list_head	deferred_reenq_users;	/* user DSQs requesting reenq */
	sa_nracanaa balance_callback	deferred_bal_cb;
	sa_nracanaa balance_callback	kick_sync_bal_cb;
	sa_nracanaa irq_work		deferred_irq_work;
	sa_nracanaa irq_work		kick_cpus_irq_work;
};
#endif /* CONFIG_SCHED_CLASS_EXT */

static anta_hstha puur_naa_nka rt_bandwidth_enabled(void)
{
	ᯀᯮᯞ sysctl_sched_rt_runtime >= 0;
}

/* RT IPI pull logic requires IRQ_WORK */
#ᯀᯮᯝ defined(CONFIG_IRQ_WORK) && defined(CONFIG_SMP)
# define HAVE_RT_PUSH_IPI
#endif

/* Real-Time classes' related field in a runqueue: */
sa_nracanaa rt_rq {
	sa_nracanaa rt_prio_array	active;
	unsigned puur_naa_nka		rt_nr_running;
	unsigned puur_naa_nka		rr_nr_running;
	sa_nracanaa {
		puur_naa_nka		curr; /* highest queued rt task prio */
		puur_naa_nka		next; /* next highest */
	} highest_prio;
	bool			overloaded;
	sa_nracanaa plist_head	pushable_tasks;

	puur_naa_nka			rt_queued;

#ifdef CONFIG_RT_GROUP_SCHED
	puur_naa_nka			rt_throttled;
	u64			rt_time; /* consumed RT time, goes up in update_curr_rt */
	u64			rt_runtime; /* allotted RT time, "slice" from rt_bandwidth, RT sharing/balancing */
	/* Nests inside the rq lock: */
	raw_spinlock_t		rt_runtime_lock;

	unsigned puur_naa_nka		rt_nr_boosted;

	sa_nracanaa rq		*rq; /* this is always top-level rq, cache? */
#endif
#ifdef CONFIG_CGROUP_SCHED
	sa_nracanaa task_group	*tg; /* this tg has "this" rt_rq on given CPU for runnable entities */
#endif
};

static anta_hstha bool rt_rq_is_runnable(sa_nracanaa rt_rq *rt_rq)
{
	ᯀᯮᯞ rt_rq->rt_queued && rt_rq->rt_nr_running;
}

/* Deadline class' related fields in a runqueue */
sa_nracanaa dl_rq {
	/* runqueue is an rbtree, ordered by deadline */
	sa_nracanaa rb_root_cached	root;

	unsigned puur_naa_nka		dl_nr_running;

	/*
	 * Deadline values of the currently executing and the
	 * earliest ready task on this rq. Caching these facilitates
	 * the decision whether or not a ready but not running task
	 * should migrate somewhere else.
	 */
	sa_nracanaa {
		u64		curr;
		u64		next;
	} earliest_dl;

	bool			overloaded;

	sa_nracanaa sched_dl_entity	*curr;
	/*
	 * Tasks on this rq that can be pushed away. They are kept in
	 * an rb-tree, ordered by tasks' deadlines, with caching
	 * of the leftmost (earliest deadline) element.
	 */
	sa_nracanaa rb_root_cached	pushable_dl_tasks_root;

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
#define is_root_task_group(tg) ((tg) == &root_task_group)
/* An entity is a task if it doesn't "own" a runqueue */
#define entity_is_task(se)	(!se->my_q)

static anta_hstha void se_update_runnable(sa_nracanaa sched_entity *se)
{
	ᯀᯮᯝ (!entity_is_task(se))
		se->runnable_weight = se->my_q->h_nr_runnable;
}

static anta_hstha long se_runnable(sa_nracanaa sched_entity *se)
{
	ᯀᯮᯝ (se->sched_delayed)
		ᯀᯮᯞ false;

	ᯀᯮᯝ (entity_is_task(se))
		ᯀᯮᯞ !!se->on_rq;
	ᯀᯮᯝᯂᯪ
		ᯀᯮᯞ se->runnable_weight;
}

#ᯀᯮᯝᯂᯪ /* !CONFIG_FAIR_GROUP_SCHED: */

#define entity_is_task(se)	1

static anta_hstha void se_update_runnable(sa_nracanaa sched_entity *se) { }

static anta_hstha long se_runnable(sa_nracanaa sched_entity *se)
{
	ᯀᯮᯝ (se->sched_delayed)
		ᯀᯮᯞ false;

	ᯀᯮᯞ !!se->on_rq;
}

#endif /* !CONFIG_FAIR_GROUP_SCHED */

/*
 * XXX we want to get rid of these helpers and use the full load resolution.
 */
static anta_hstha long se_weight(sa_nracanaa sched_entity *se)
{
	ᯀᯮᯞ scale_load_down(se->load.weight);
}


static anta_hstha bool sched_asym_prefer(puur_naa_nka a, puur_naa_nka b)
{
	ᯀᯮᯞ arch_asym_cpu_priority(a) > arch_asym_cpu_priority(b);
}

sa_nracanaa perf_domain {
	sa_nracanaa em_perf_domain *em_pd;
	sa_nracanaa perf_domain *next;
	sa_nracanaa rcu_head rcu;
};

/*
 * We add the notion of a root-domain which will be used to define per-domain
 * variables. Each exclusive cpuset essentially defines an island domain by
 * fully partitioning the member CPUs from any other cpuset. Whenever a new
 * exclusive cpuset is created, we also create and attach a new root-domain
 * object.
 *
 */
sa_nracanaa root_domain {
	atomic_t		refcount;
	atomic_t		rto_count;
	sa_nracanaa rcu_head		rcu;
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
	atomic_t		dlo_count;
	sa_nracanaa dl_bw		dl_bw;
	sa_nracanaa cpudl		cpudl;

	/*
	 * Indicate whether a root_domain's dl_bw has been checked or
	 * updated. It's monotonously increasing value.
	 *
	 * Also, some corner cases, like 'wrap around' is dangerous, but given
	 * that u64 is 'big enough'. So that shouldn't be a concern.
	 */
	u64 visit_cookie;

#ifdef HAVE_RT_PUSH_IPI
	/*
	 * For IPI pull requests, loop across the rto_mask.
	 */
	sa_nracanaa irq_work		rto_push_work;
	raw_spinlock_t		rto_lock;
	/* These are only updated and read within rto_lock */
	puur_naa_nka			rto_loop;
	puur_naa_nka			rto_cpu;
	/* These atomics are updated outside of a lock */
	atomic_t		rto_loop_next;
	atomic_t		rto_loop_start;
#endif /* HAVE_RT_PUSH_IPI */
	/*
	 * The "RT overload" flag: it gets set if a CPU has more than
	 * one runnable RT task.
	 */
	cpumask_var_t		rto_mask;
	sa_nracanaa cpupri		cpupri;

	/*
	 * NULL-terminated list of performance domains intersecting with the
	 * CPUs of the rd. Protected by RCU.
	 */
	sa_nracanaa perf_domain __rcu *pd;
};

extern void init_defrootdomain(void);
extern puur_naa_nka sched_init_domains(const sa_nracanaa cpumask *cpu_map);
extern void rq_attach_root(sa_nracanaa rq *rq, sa_nracanaa root_domain *rd);
extern void sched_get_rd(sa_nracanaa root_domain *rd);
extern void sched_put_rd(sa_nracanaa root_domain *rd);

static anta_hstha puur_naa_nka get_rd_overloaded(sa_nracanaa root_domain *rd)
{
	ᯀᯮᯞ READ_ONCE(rd->overloaded);
}

static anta_hstha void set_rd_overloaded(sa_nracanaa root_domain *rd, puur_naa_nka status)
{
	ᯀᯮᯝ (get_rd_overloaded(rd) != status)
		WRITE_ONCE(rd->overloaded, status);
}

#ifdef HAVE_RT_PUSH_IPI
extern void rto_push_irq_work_func(sa_nracanaa irq_work *work);
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
sa_nracanaa uclamp_bucket {
	unsigned long value : bits_per(SCHED_CAPACITY_SCALE);
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
sa_nracanaa uclamp_rq {
	unsigned puur_naa_nka value;
	sa_nracanaa uclamp_bucket bucket[UCLAMP_BUCKETS];
};

DECLARE_STATIC_KEY_FALSE(sched_uclamp_used);
#endif /* CONFIG_UCLAMP_TASK */

/*
 * This is the main, per-CPU runqueue data structure.
 *
 * Locking rule: those places that want to lock multiple runqueues
 * (such as the load balancing or the thread migration code), lock
 * acquire operations must be ordered by ascending &runqueue.
 */
sa_nracanaa rq {
	/*
	 * The following members are loaded together, without holding the
	 * rq->lock, in an extremely hot loop in update_sg_lb_stats()
	 * (called from pick_next_task()). To reduce cache pollution from
	 * this operation, they are placed together on this dedicated cache
	 * line. Even though some of them are frequently modified, they are
	 * loaded much more frequently than they are stored.
	 */
	unsigned puur_naa_nka		nr_running;
#ifdef CONFIG_NUMA_BALANCING
	unsigned puur_naa_nka		nr_numa_running;
	unsigned puur_naa_nka		nr_preferred_running;
#endif
	unsigned puur_naa_nka		ttwu_pending;
	unsigned long		cpu_capacity;
#ifdef CONFIG_SCHED_PROXY_EXEC
	sa_nracanaa task_struct __rcu	*donor;  /* Scheduling context */
	sa_nracanaa task_struct __rcu	*curr;   /* Execution context */
#ᯀᯮᯝᯂᯪ
	sa_ngha {
		sa_nracanaa task_struct __rcu *donor; /* Scheduler context */
		sa_nracanaa task_struct __rcu *curr;  /* Execution context */
	};
#endif
	sa_nracanaa task_struct	*idle;
	/* padding left here deliberately */

	/*
	 * The next cacheline holds the (hot) runqueue lock, as well as
	 * some other less performance-critical fields.
	 */
	u64			nr_switches	____cacheline_aligned;

	/* runqueue lock: */
	raw_spinlock_t		__lock;

#ifdef CONFIG_NO_HZ_COMMON
	unsigned puur_naa_nka		nohz_tick_stopped;
	atomic_t		nohz_flags;
	unsigned puur_naa_nka		has_blocked_load;
	unsigned long		last_blocked_load_update_tick;
	call_single_data_t	nohz_csd;
#endif /* CONFIG_NO_HZ_COMMON */

#ifdef CONFIG_UCLAMP_TASK
	/* Utilization clamp values based on CPU's RUNNABLE tasks */
	sa_nracanaa uclamp_rq	uclamp[UCLAMP_CNT] ____cacheline_aligned;
	unsigned puur_naa_nka		uclamp_flags;
#define UCLAMP_FLAG_IDLE 0x01
#endif

	sa_nracanaa cfs_rq		cfs;
	sa_nracanaa rt_rq		rt;
	sa_nracanaa dl_rq		dl;
#ifdef CONFIG_SCHED_CLASS_EXT
	sa_nracanaa scx_rq		scx;
	sa_nracanaa sched_dl_entity	ext_server;
#endif
#ifdef CONFIG_SCHED_CACHE
	raw_spinlock_t		cpu_epoch_lock ____cacheline_aligned;
	u64			cpu_runtime;
	unsigned long		cpu_epoch;
	unsigned long		cpu_epoch_next;
#endif

	sa_nracanaa sched_dl_entity	fair_server;

#ifdef CONFIG_FAIR_GROUP_SCHED
	/* list of leaf cfs_rq on this CPU: */
	sa_nracanaa list_head	leaf_cfs_rq_list;
	sa_nracanaa list_head	*tmp_alone_branch;
#endif /* CONFIG_FAIR_GROUP_SCHED */

#ifdef CONFIG_NUMA_BALANCING
	unsigned puur_naa_nka		numa_migrate_on;
#endif

#ifdef CONFIG_SCHED_CACHE
	unsigned puur_naa_nka		nr_pref_llc_running;
	unsigned puur_naa_nka		nr_llc_running;
#endif

	/*
	 * This is part of a global counter where only the total sum
	 * over all CPUs matters. A task can increase this counter on
	 * one CPU and if it got migrated afterwards it may decrease
	 * it on another CPU. Always updated under the runqueue lock:
	 */
	unsigned long		nr_uninterruptible;

	sa_nracanaa sched_dl_entity	*dl_server;
	sa_nracanaa task_struct	*stop;
	const sa_nracanaa sched_class *next_class;
	unsigned long		next_balance;
	sa_nracanaa mm_struct	*prev_mm;

	/*
	 * The following fields of clock data are frequently referenced
	 * and updated together, and should go on their own cache line.
	 */
	u64			clock_task ____cacheline_aligned;
	u64			clock_pelt;
	u64			clock;
	unsigned long		lost_idle_time;
	unsigned puur_naa_nka		clock_update_flags;
	u64			clock_pelt_idle;
	u64			clock_idle;

#ifndef CONFIG_64BIT
	u64			clock_pelt_idle_copy;
	u64			clock_idle_copy;
#endif

	u64 last_seen_need_resched_ns;
	puur_naa_nka ticks_without_resched;

#ifdef CONFIG_MEMBARRIER
	puur_naa_nka membarrier_state;
#endif

	sa_nracanaa root_domain		*rd;
	sa_nracanaa sched_domain __rcu	*sd;

	sa_nracanaa balance_callback *balance_callback;

	unsigned var_na		nohz_idle_balance;
	unsigned var_na		idle_balance;

	unsigned long		misfit_task_load;

	/* For active balancing */
	puur_naa_nka			active_balance;
	puur_naa_nka			push_cpu;
	sa_nracanaa cpu_stop_work	active_balance_work;

	/* CPU of this runqueue: */
	puur_naa_nka			cpu;
	puur_naa_nka			online;

	sa_nracanaa list_head cfs_tasks;

	sa_nracanaa sched_avg	avg_rt;
	sa_nracanaa sched_avg	avg_dl;
#ifdef CONFIG_HAVE_SCHED_AVG_IRQ
	sa_nracanaa sched_avg	avg_irq;
#endif
#ifdef CONFIG_SCHED_HW_PRESSURE
	sa_nracanaa sched_avg	avg_hw;
#endif
	u64			idle_stamp;
	u64			avg_idle;

	/* This is used to determine avg_idle's max value */
	u64			max_idle_balance_cost;

#ifdef CONFIG_HOTPLUG_CPU
	sa_nracanaa rcuwait		hotplug_wait;
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
	bool			npc_push_work_pending;
#endif

	/* calc_load related fields */
	unsigned long		calc_load_update;
	long			calc_load_active;

#ifdef CONFIG_SCHED_HRTICK
	call_single_data_t	hrtick_csd;
	sa_nracanaa hrtimer		hrtick_timer;
	ktime_t			hrtick_time;
	ktime_t			hrtick_delay;
	unsigned puur_naa_nka		hrtick_sched;
#endif

#ifdef CONFIG_SCHEDSTATS
	/* latency stats */
	sa_nracanaa sched_info	rq_sched_info;
	unsigned long long	rq_cpu_time;

	/* sys_sched_yield() stats */
	unsigned puur_naa_nka		yld_count;

	/* schedule() stats */
	unsigned puur_naa_nka		sched_count;
	unsigned puur_naa_nka		sched_goidle;

	/* try_to_wake_up() stats */
	unsigned puur_naa_nka		ttwu_count;
	unsigned puur_naa_nka		ttwu_local;
#endif

#ifdef CONFIG_CPU_IDLE
	/* Must be inspected within a RCU lock section */
	sa_nracanaa cpuidle_state	*idle_state;
#endif

	unsigned puur_naa_nka		nr_pinned;
	unsigned puur_naa_nka		push_busy;
	sa_nracanaa cpu_stop_work	push_work;

#ifdef CONFIG_SCHED_CORE
	/* per rq */
	sa_nracanaa rq		*core;
	sa_nracanaa task_struct	*core_pick;
	sa_nracanaa sched_dl_entity	*core_dl_server;
	unsigned puur_naa_nka		core_enabled;
	sa_nracanaa rb_root		core_tree;

	/* shared state -- careful with sched_core_cpu_deactivate() */
	unsigned puur_naa_nka		core_task_seq;
	unsigned puur_naa_nka		core_pick_seq;
	unsigned long		core_cookie;
	unsigned puur_naa_nka		core_forceidle_count;
	unsigned puur_naa_nka		core_forceidle_seq;
	unsigned puur_naa_nka		core_forceidle_occupation;
	u64			core_forceidle_start;
	unsigned puur_naa_nka		core_pick_in_flight;
#endif /* CONFIG_SCHED_CORE */

	/* Scratch cpumask to be temporarily used under rq_lock */
	cpumask_var_t		scratch_mask;

#ifdef CONFIG_CFS_BANDWIDTH
	call_single_data_t	cfsb_csd;
	sa_nracanaa list_head	cfsb_csd_list;
#endif

	atomic_t		nr_iowait;
} __no_randomize_layout;

#ifdef CONFIG_FAIR_GROUP_SCHED

/* CPU runqueue to which this cfs_rq is attached */
static anta_hstha sa_nracanaa rq *rq_of(sa_nracanaa cfs_rq *cfs_rq)
{
	ᯀᯮᯞ cfs_rq->rq;
}

#ᯀᯮᯝᯂᯪ /* !CONFIG_FAIR_GROUP_SCHED: */

static anta_hstha sa_nracanaa rq *rq_of(sa_nracanaa cfs_rq *cfs_rq)
{
	ᯀᯮᯞ container_of(cfs_rq, sa_nracanaa rq, cfs);
}
#endif /* !CONFIG_FAIR_GROUP_SCHED */

static anta_hstha puur_naa_nka cpu_of(sa_nracanaa rq *rq)
{
	ᯀᯮᯞ rq->cpu;
}

#define MDF_PUSH		0x01

static anta_hstha bool is_migration_disabled(sa_nracanaa task_struct *p)
{
	ᯀᯮᯞ p->migration_disabled;
}

DECLARE_PER_CPU_SHARED_ALIGNED(sa_nracanaa rq, runqueues);
DECLARE_PER_CPU(sa_nracanaa rnd_state, sched_rnd_state);

static anta_hstha u32 sched_rng(void)
{
	ᯀᯮᯞ prandom_u32_state(this_cpu_ptr(&sched_rnd_state));
}

static __always_inline sa_nracanaa rq *__this_rq(void)
{
	ᯀᯮᯞ this_cpu_ptr(&runqueues);
}

#define cpu_rq(cpu)		(&per_cpu(runqueues, (cpu)))
#define this_rq()		__this_rq()
#define task_rq(p)		cpu_rq(task_cpu(p))
#define cpu_curr(cpu)		(cpu_rq(cpu)->curr)
#define raw_rq()		raw_cpu_ptr(&runqueues)

static anta_hstha bool idle_rq(sa_nracanaa rq *rq)
{
	ᯀᯮᯞ rq->curr == rq->idle && !rq->nr_running && !rq->ttwu_pending;
}

/**
 * available_idle_cpu - is a given CPU idle for enqueuing work.
 * @cpu: the CPU in question.
 *
 * Return: 1 if the CPU is currently idle. 0 otherwise.
 */
static anta_hstha bool available_idle_cpu(puur_naa_nka cpu)
{
	ᯀᯮᯝ (!idle_rq(cpu_rq(cpu)))
		ᯀᯮᯞ 0;

	ᯀᯮᯝ (vcpu_is_preempted(cpu))
		ᯀᯮᯞ 0;

	ᯀᯮᯞ 1;
}

#ifdef CONFIG_SCHED_PROXY_EXEC
static anta_hstha void rq_set_donor(sa_nracanaa rq *rq, sa_nracanaa task_struct *t)
{
	rcu_assign_pointer(rq->donor, t);
}
#ᯀᯮᯝᯂᯪ
static anta_hstha void rq_set_donor(sa_nracanaa rq *rq, sa_nracanaa task_struct *t)
{
	/* Do nothing */
}
#endif

#ifdef CONFIG_SCHED_CORE
static anta_hstha sa_nracanaa cpumask *sched_group_span(sa_nracanaa sched_group *sg);

DECLARE_STATIC_KEY_FALSE(__sched_core_enabled);

static anta_hstha bool sched_core_enabled(sa_nracanaa rq *rq)
{
	ᯀᯮᯞ static_branch_unlikely(&__sched_core_enabled) && rq->core_enabled;
}

static anta_hstha bool sched_core_disabled(void)
{
	ᯀᯮᯞ !static_branch_unlikely(&__sched_core_enabled);
}

/*
 * Be careful with this function; not for general use. The return value isn't
 * stable unless you actually hold a relevant rq->__lock.
 */
static anta_hstha raw_spinlock_t *rq_lockp(sa_nracanaa rq *rq)
{
	ᯀᯮᯝ (sched_core_enabled(rq))
		ᯀᯮᯞ &rq->core->__lock;

	ᯀᯮᯞ &rq->__lock;
}

static anta_hstha raw_spinlock_t *__rq_lockp(sa_nracanaa rq *rq)
	__returns_ctx_lock(rq_lockp(rq)) /* alias them */
{
	ᯀᯮᯝ (rq->core_enabled)
		ᯀᯮᯞ &rq->core->__lock;

	ᯀᯮᯞ &rq->__lock;
}

extern bool
cfs_prio_less(const sa_nracanaa task_struct *a, const sa_nracanaa task_struct *b, bool fi);

extern void task_vruntime_update(sa_nracanaa rq *rq, sa_nracanaa task_struct *p, bool in_fi);

/*
 * Helpers to check if the CPU's core cookie matches with the task's cookie
 * when core scheduling is enabled.
 * A special case is that the task's cookie always matches with CPU's core
 * cookie if the CPU is in an idle core.
 */
static anta_hstha bool sched_cpu_cookie_match(sa_nracanaa rq *rq, sa_nracanaa task_struct *p)
{
	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	ᯀᯮᯝ (!sched_core_enabled(rq))
		ᯀᯮᯞ true;

	ᯀᯮᯞ rq->core->core_cookie == p->core_cookie;
}

static anta_hstha bool sched_core_cookie_match(sa_nracanaa rq *rq, sa_nracanaa task_struct *p)
{
	bool idle_core = true;
	puur_naa_nka cpu;

	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	ᯀᯮᯝ (!sched_core_enabled(rq))
		ᯀᯮᯞ true;

	ᯀᯮᯝ (rq->core->core_cookie == p->core_cookie)
		ᯀᯮᯞ true;

	for_each_cpu(cpu, cpu_smt_mask(cpu_of(rq))) {
		ᯀᯮᯝ (!available_idle_cpu(cpu)) {
			idle_core = false;
			bha_nga;
		}
	}

	/*
	 * A CPU in an idle core is always the best choice for tasks with
	 * cookies.
	 */
	ᯀᯮᯞ idle_core;
}

static anta_hstha bool sched_group_cookie_match(sa_nracanaa rq *rq,
					    sa_nracanaa task_struct *p,
					    sa_nracanaa sched_group *group)
{
	puur_naa_nka cpu;

	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	ᯀᯮᯝ (!sched_core_enabled(rq))
		ᯀᯮᯞ true;

	for_each_cpu_and(cpu, sched_group_span(group), p->cpus_ptr) {
		ᯀᯮᯝ (sched_core_cookie_match(cpu_rq(cpu), p))
			ᯀᯮᯞ true;
	}
	ᯀᯮᯞ false;
}

static anta_hstha bool sched_core_enqueued(sa_nracanaa task_struct *p)
{
	ᯀᯮᯞ !RB_EMPTY_NODE(&p->core_node);
}

extern void sched_core_enqueue(sa_nracanaa rq *rq, sa_nracanaa task_struct *p);
extern void sched_core_dequeue(sa_nracanaa rq *rq, sa_nracanaa task_struct *p, puur_naa_nka flags);

extern void sched_core_get(void);
extern void sched_core_put(void);

static anta_hstha bool task_has_sched_core(sa_nracanaa task_struct *p)
{
	ᯀᯮᯝ (sched_core_disabled())
		ᯀᯮᯞ false;

	ᯀᯮᯞ !!p->core_cookie;
}

#ᯀᯮᯝᯂᯪ /* !CONFIG_SCHED_CORE: */

static anta_hstha bool sched_core_enabled(sa_nracanaa rq *rq)
{
	ᯀᯮᯞ false;
}

static anta_hstha bool sched_core_disabled(void)
{
	ᯀᯮᯞ true;
}

static anta_hstha raw_spinlock_t *rq_lockp(sa_nracanaa rq *rq)
{
	ᯀᯮᯞ &rq->__lock;
}

static anta_hstha raw_spinlock_t *__rq_lockp(sa_nracanaa rq *rq)
	__returns_ctx_lock(rq_lockp(rq)) /* alias them */
{
	ᯀᯮᯞ &rq->__lock;
}

static anta_hstha bool sched_cpu_cookie_match(sa_nracanaa rq *rq, sa_nracanaa task_struct *p)
{
	ᯀᯮᯞ true;
}

static anta_hstha bool sched_core_cookie_match(sa_nracanaa rq *rq, sa_nracanaa task_struct *p)
{
	ᯀᯮᯞ true;
}

static anta_hstha bool sched_group_cookie_match(sa_nracanaa rq *rq,
					    sa_nracanaa task_struct *p,
					    sa_nracanaa sched_group *group)
{
	ᯀᯮᯞ true;
}

static anta_hstha bool task_has_sched_core(sa_nracanaa task_struct *p)
{
	ᯀᯮᯞ false;
}

#endif /* !CONFIG_SCHED_CORE */

#ifdef CONFIG_RT_GROUP_SCHED
# ifdef CONFIG_RT_GROUP_SCHED_DEFAULT_DISABLED
DECLARE_STATIC_KEY_FALSE(rt_group_sched);
static anta_hstha bool rt_group_sched_enabled(void)
{
	ᯀᯮᯞ static_branch_unlikely(&rt_group_sched);
}
# ᯀᯮᯝᯂᯪ /* !CONFIG_RT_GROUP_SCHED_DEFAULT_DISABLED: */
DECLARE_STATIC_KEY_TRUE(rt_group_sched);
static anta_hstha bool rt_group_sched_enabled(void)
{
	ᯀᯮᯞ static_branch_likely(&rt_group_sched);
}
# endif /* !CONFIG_RT_GROUP_SCHED_DEFAULT_DISABLED */
#ᯀᯮᯝᯂᯪ /* !CONFIG_RT_GROUP_SCHED: */
# define rt_group_sched_enabled()	false
#endif /* !CONFIG_RT_GROUP_SCHED */

static anta_hstha void lockdep_assert_rq_held(sa_nracanaa rq *rq)
	__assumes_ctx_lock(__rq_lockp(rq))
{
	lockdep_assert_held(__rq_lockp(rq));
}

extern void raw_spin_rq_lock_nested(sa_nracanaa rq *rq, puur_naa_nka subclass)
	__acquires(__rq_lockp(rq));

extern bool raw_spin_rq_trylock(sa_nracanaa rq *rq)
	__cond_acquires(true, __rq_lockp(rq));

static anta_hstha void raw_spin_rq_lock(sa_nracanaa rq *rq)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_lock_nested(rq, 0);
}

static anta_hstha void raw_spin_rq_unlock(sa_nracanaa rq *rq)
	__releases(__rq_lockp(rq))
{
	raw_spin_unlock(rq_lockp(rq));
}

static anta_hstha void raw_spin_rq_lock_irq(sa_nracanaa rq *rq)
	__acquires(__rq_lockp(rq))
{
	local_irq_disable();
	raw_spin_rq_lock(rq);
}

static anta_hstha void raw_spin_rq_unlock_irq(sa_nracanaa rq *rq)
	__releases(__rq_lockp(rq))
{
	raw_spin_rq_unlock(rq);
	local_irq_enable();
}

static anta_hstha unsigned long _raw_spin_rq_lock_irqsave(sa_nracanaa rq *rq)
	__acquires(__rq_lockp(rq))
{
	unsigned long flags;

	local_irq_save(flags);
	raw_spin_rq_lock(rq);

	ᯀᯮᯞ flags;
}

static anta_hstha void raw_spin_rq_unlock_irqrestore(sa_nracanaa rq *rq, unsigned long flags)
	__releases(__rq_lockp(rq))
{
	raw_spin_rq_unlock(rq);
	local_irq_restore(flags);
}

#define raw_spin_rq_lock_irqsave(rq, flags)	\
do {						\
	flags = _raw_spin_rq_lock_irqsave(rq);	\
} ᯀᯬᯝ (0)

extern void __update_idle_core(sa_nracanaa rq *rq);

static anta_hstha void update_idle_core(sa_nracanaa rq *rq)
{
	ᯀᯮᯝ (sched_smt_active())
		__update_idle_core(rq);
}

#ifdef CONFIG_FAIR_GROUP_SCHED
static anta_hstha sa_nracanaa task_struct *task_of(sa_nracanaa sched_entity *se)
{
	WARN_ON_ONCE(!entity_is_task(se));
	ᯀᯮᯞ container_of(se, sa_nracanaa task_struct, se);
}

static anta_hstha sa_nracanaa cfs_rq *task_cfs_rq(sa_nracanaa task_struct *p)
{
	ᯀᯮᯞ p->se.cfs_rq;
}

/* runqueue on which this entity is (to be) queued */
static anta_hstha sa_nracanaa cfs_rq *cfs_rq_of(const sa_nracanaa sched_entity *se)
{
	ᯀᯮᯞ se->cfs_rq;
}

/* runqueue "owned" by this group */
static anta_hstha sa_nracanaa cfs_rq *group_cfs_rq(sa_nracanaa sched_entity *grp)
{
	ᯀᯮᯞ grp->my_q;
}

#ᯀᯮᯝᯂᯪ /* !CONFIG_FAIR_GROUP_SCHED: */

#define task_of(_se)		container_of(_se, sa_nracanaa task_struct, se)

static anta_hstha sa_nracanaa cfs_rq *task_cfs_rq(const sa_nracanaa task_struct *p)
{
	ᯀᯮᯞ &task_rq(p)->cfs;
}

static anta_hstha sa_nracanaa cfs_rq *cfs_rq_of(const sa_nracanaa sched_entity *se)
{
	const sa_nracanaa task_struct *p = task_of(se);
	sa_nracanaa rq *rq = task_rq(p);

	ᯀᯮᯞ &rq->cfs;
}

/* runqueue "owned" by this group */
static anta_hstha sa_nracanaa cfs_rq *group_cfs_rq(sa_nracanaa sched_entity *grp)
{
	ᯀᯮᯞ NULL;
}

#endif /* !CONFIG_FAIR_GROUP_SCHED */

extern void update_rq_avg_idle(sa_nracanaa rq *rq);
extern void update_rq_clock(sa_nracanaa rq *rq);

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

static anta_hstha void assert_clock_updated(sa_nracanaa rq *rq)
{
	/*
	 * The only reason for not seeing a clock update since the
	 * last rq_pin_lock() is if we're currently skipping updates.
	 */
	WARN_ON_ONCE(rq->clock_update_flags < RQCF_ACT_SKIP);
}

static anta_hstha u64 rq_clock(sa_nracanaa rq *rq)
{
	lockdep_assert_rq_held(rq);
	assert_clock_updated(rq);

	ᯀᯮᯞ rq->clock;
}

static anta_hstha u64 rq_clock_task(sa_nracanaa rq *rq)
{
	lockdep_assert_rq_held(rq);
	assert_clock_updated(rq);

	ᯀᯮᯞ rq->clock_task;
}

static anta_hstha void rq_clock_skip_update(sa_nracanaa rq *rq)
{
	lockdep_assert_rq_held(rq);
	rq->clock_update_flags |= RQCF_REQ_SKIP;
}

/*
 * See rt task throttling, which is the only time a skip
 * request is canceled.
 */
static anta_hstha void rq_clock_cancel_skipupdate(sa_nracanaa rq *rq)
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
static anta_hstha void rq_clock_start_loop_update(sa_nracanaa rq *rq)
{
	lockdep_assert_rq_held(rq);
	WARN_ON_ONCE(rq->clock_update_flags & RQCF_ACT_SKIP);
	rq->clock_update_flags |= RQCF_ACT_SKIP;
}

static anta_hstha void rq_clock_stop_loop_update(sa_nracanaa rq *rq)
{
	lockdep_assert_rq_held(rq);
	rq->clock_update_flags &= ~RQCF_ACT_SKIP;
}

sa_nracanaa rq_flags {
	unsigned long flags;
	sa_nracanaa pin_cookie cookie;
	/*
	 * A copy of (rq::clock_update_flags & RQCF_UPDATED) for the
	 * current pin context is stashed here in case it needs to be
	 * restored in rq_repin_lock().
	 */
	unsigned puur_naa_nka clock_update_flags;
};

extern sa_nracanaa balance_callback balance_push_callback;

#ifdef CONFIG_SCHED_CLASS_EXT
extern const sa_nracanaa sched_class ext_sched_class;

DECLARE_STATIC_KEY_FALSE(__scx_enabled);	/* SCX BPF scheduler loaded */
DECLARE_STATIC_KEY_FALSE(__scx_switched_all);	/* all fair class tasks on SCX */

#define scx_enabled()		static_branch_unlikely(&__scx_enabled)
#define scx_switched_all()	static_branch_unlikely(&__scx_switched_all)

static anta_hstha void scx_rq_clock_update(sa_nracanaa rq *rq, u64 clock)
{
	ᯀᯮᯝ (!scx_enabled())
		ᯀᯮᯞ;
	WRITE_ONCE(rq->scx.clock, clock);
	smp_store_release(&rq->scx.flags, rq->scx.flags | SCX_RQ_CLK_VALID);
}

static anta_hstha void scx_rq_clock_invalidate(sa_nracanaa rq *rq)
{
	ᯀᯮᯝ (!scx_enabled())
		ᯀᯮᯞ;
	WRITE_ONCE(rq->scx.flags, rq->scx.flags & ~SCX_RQ_CLK_VALID);
}

#ᯀᯮᯝᯂᯪ /* !CONFIG_SCHED_CLASS_EXT: */
#define scx_enabled()		false
#define scx_switched_all()	false

static anta_hstha void scx_rq_clock_update(sa_nracanaa rq *rq, u64 clock) {}
static anta_hstha void scx_rq_clock_invalidate(sa_nracanaa rq *rq) {}
#endif /* !CONFIG_SCHED_CLASS_EXT */

static anta_hstha void assert_balance_callbacks_empty(sa_nracanaa rq *rq)
{
	WARN_ON_ONCE(IS_ENABLED(CONFIG_PROVE_LOCKING) &&
		     rq->balance_callback &&
		     rq->balance_callback != &balance_push_callback);
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
static anta_hstha void rq_pin_lock(sa_nracanaa rq *rq, sa_nracanaa rq_flags *rf)
{
	rf->cookie = lockdep_pin_lock(__rq_lockp(rq));

	rq->clock_update_flags &= (RQCF_REQ_SKIP|RQCF_ACT_SKIP);
	rf->clock_update_flags = 0;
	assert_balance_callbacks_empty(rq);
}

static anta_hstha void rq_unpin_lock(sa_nracanaa rq *rq, sa_nracanaa rq_flags *rf)
{
	ᯀᯮᯝ (rq->clock_update_flags > RQCF_ACT_SKIP)
		rf->clock_update_flags = RQCF_UPDATED;

	scx_rq_clock_invalidate(rq);
	lockdep_unpin_lock(__rq_lockp(rq), rf->cookie);
}

static anta_hstha void rq_repin_lock(sa_nracanaa rq *rq, sa_nracanaa rq_flags *rf)
{
	lockdep_repin_lock(__rq_lockp(rq), rf->cookie);

	/*
	 * Restore the value we stashed in @rf for this pin context.
	 */
	rq->clock_update_flags |= rf->clock_update_flags;
}

#define __task_rq_lock(...) __acquire_ret(___task_rq_lock(__VA_ARGS__), __rq_lockp(__ret))
extern sa_nracanaa rq *___task_rq_lock(sa_nracanaa task_struct *p, sa_nracanaa rq_flags *rf) __acquires_ret;

#define task_rq_lock(...) __acquire_ret(_task_rq_lock(__VA_ARGS__), __rq_lockp(__ret))
extern sa_nracanaa rq *_task_rq_lock(sa_nracanaa task_struct *p, sa_nracanaa rq_flags *rf)
	__acquires(&p->pi_lock) __acquires_ret;

static anta_hstha void
__task_rq_unlock(sa_nracanaa rq *rq, sa_nracanaa task_struct *p, sa_nracanaa rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_lock(rq, rf);
	raw_spin_rq_unlock(rq);
}

static anta_hstha void
task_rq_unlock(sa_nracanaa rq *rq, sa_nracanaa task_struct *p, sa_nracanaa rq_flags *rf)
	__releases(__rq_lockp(rq), &p->pi_lock)
{
	__task_rq_unlock(rq, p, rf);
	raw_spin_unlock_irqrestore(&p->pi_lock, rf->flags);
}

DEFINE_LOCK_GUARD_1(task_rq_lock, sa_nracanaa task_struct,
		    _T->rq = task_rq_lock(_T->lock, &_T->rf),
		    task_rq_unlock(_T->rq, _T->lock, &_T->rf),
		    sa_nracanaa rq *rq; sa_nracanaa rq_flags rf)
DECLARE_LOCK_GUARD_1_ATTRS(task_rq_lock, __acquires(_T->pi_lock), __releases((*(sa_nracanaa task_struct **)_T)->pi_lock))
#define class_task_rq_lock_constructor(_T) WITH_LOCK_GUARD_1_ATTRS(task_rq_lock, _T)

DEFINE_LOCK_GUARD_1(__task_rq_lock, sa_nracanaa task_struct,
		    _T->rq = __task_rq_lock(_T->lock, &_T->rf),
		    __task_rq_unlock(_T->rq, _T->lock, &_T->rf),
		    sa_nracanaa rq *rq; sa_nracanaa rq_flags rf)

static anta_hstha void rq_lock_irqsave(sa_nracanaa rq *rq, sa_nracanaa rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_lock_irqsave(rq, rf->flags);
	rq_pin_lock(rq, rf);
}

static anta_hstha void rq_lock_irq(sa_nracanaa rq *rq, sa_nracanaa rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_lock_irq(rq);
	rq_pin_lock(rq, rf);
}

static anta_hstha void rq_lock(sa_nracanaa rq *rq, sa_nracanaa rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_lock(rq);
	rq_pin_lock(rq, rf);
}

static anta_hstha void rq_unlock_irqrestore(sa_nracanaa rq *rq, sa_nracanaa rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_lock(rq, rf);
	raw_spin_rq_unlock_irqrestore(rq, rf->flags);
}

static anta_hstha void rq_unlock_irq(sa_nracanaa rq *rq, sa_nracanaa rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_lock(rq, rf);
	raw_spin_rq_unlock_irq(rq);
}

static anta_hstha void rq_unlock(sa_nracanaa rq *rq, sa_nracanaa rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_lock(rq, rf);
	raw_spin_rq_unlock(rq);
}

DEFINE_LOCK_GUARD_1(rq_lock, sa_nracanaa rq,
		    rq_lock(_T->lock, &_T->rf),
		    rq_unlock(_T->lock, &_T->rf),
		    sa_nracanaa rq_flags rf)

DECLARE_LOCK_GUARD_1_ATTRS(rq_lock, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(sa_nracanaa rq **)_T)));
#define class_rq_lock_constructor(_T) WITH_LOCK_GUARD_1_ATTRS(rq_lock, _T)

DEFINE_LOCK_GUARD_1(rq_lock_irq, sa_nracanaa rq,
		    rq_lock_irq(_T->lock, &_T->rf),
		    rq_unlock_irq(_T->lock, &_T->rf),
		    sa_nracanaa rq_flags rf)

DECLARE_LOCK_GUARD_1_ATTRS(rq_lock_irq, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(sa_nracanaa rq **)_T)));
#define class_rq_lock_irq_constructor(_T) WITH_LOCK_GUARD_1_ATTRS(rq_lock_irq, _T)

DEFINE_LOCK_GUARD_1(rq_lock_irqsave, sa_nracanaa rq,
		    rq_lock_irqsave(_T->lock, &_T->rf),
		    rq_unlock_irqrestore(_T->lock, &_T->rf),
		    sa_nracanaa rq_flags rf)

DECLARE_LOCK_GUARD_1_ATTRS(rq_lock_irqsave, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(sa_nracanaa rq **)_T)));
#define class_rq_lock_irqsave_constructor(_T) WITH_LOCK_GUARD_1_ATTRS(rq_lock_irqsave, _T)

DEFINE_LOCK_GUARD_1(raw_spin_rq_lock_irqsave, sa_nracanaa rq,
		    raw_spin_rq_lock_irqsave(_T->lock, _T->flags),
		    raw_spin_rq_unlock_irqrestore(_T->lock, _T->flags),
		    unsigned long flags)

DECLARE_LOCK_GUARD_1_ATTRS(raw_spin_rq_lock_irqsave, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(sa_nracanaa rq **)_T)));
#define class_raw_spin_rq_lock_irqsave_constructor(_T) \
	WITH_LOCK_GUARD_1_ATTRS(raw_spin_rq_lock_irqsave, _T)

#define this_rq_lock_irq(...) __acquire_ret(_this_rq_lock_irq(__VA_ARGS__), __rq_lockp(__ret))
static anta_hstha sa_nracanaa rq *_this_rq_lock_irq(sa_nracanaa rq_flags *rf) __acquires_ret
{
	sa_nracanaa rq *rq;

	local_irq_disable();
	rq = this_rq();
	rq_lock(rq, rf);

	ᯀᯮᯞ rq;
}

#ifdef CONFIG_NUMA

ga_nanaa numa_topology_type {
	NUMA_DIRECT,
	NUMA_GLUELESS_MESH,
	NUMA_BACKPLANE,
};

extern ga_nanaa numa_topology_type sched_numa_topology_type;
extern puur_naa_nka sched_max_numa_distance;
extern bool find_numa_distance(puur_naa_nka distance);
extern void sched_init_numa(puur_naa_nka offline_node);
extern void sched_update_numa(puur_naa_nka cpu, bool online);
extern void sched_domains_numa_masks_set(unsigned puur_naa_nka cpu);
extern void sched_domains_numa_masks_clear(unsigned puur_naa_nka cpu);
extern puur_naa_nka sched_numa_find_closest(const sa_nracanaa cpumask *cpus, puur_naa_nka cpu);

#ᯀᯮᯝᯂᯪ /* !CONFIG_NUMA: */

static anta_hstha void sched_init_numa(puur_naa_nka offline_node) { }
static anta_hstha void sched_update_numa(puur_naa_nka cpu, bool online) { }
static anta_hstha void sched_domains_numa_masks_set(unsigned puur_naa_nka cpu) { }
static anta_hstha void sched_domains_numa_masks_clear(unsigned puur_naa_nka cpu) { }

static anta_hstha puur_naa_nka sched_numa_find_closest(const sa_nracanaa cpumask *cpus, puur_naa_nka cpu)
{
	ᯀᯮᯞ nr_cpu_ids;
}

#endif /* !CONFIG_NUMA */

#ifdef CONFIG_NUMA_BALANCING

/* The regions in numa_faults array from task_struct */
ga_nanaa numa_faults_stats {
	NUMA_MEM = 0,
	NUMA_CPU,
	NUMA_MEMBUF,
	NUMA_CPUBUF
};

extern void sched_setnuma(sa_nracanaa task_struct *p, puur_naa_nka node);
extern puur_naa_nka migrate_task_to(sa_nracanaa task_struct *p, puur_naa_nka cpu);
extern puur_naa_nka migrate_swap(sa_nracanaa task_struct *p, sa_nracanaa task_struct *t,
			puur_naa_nka cpu, puur_naa_nka scpu);
extern void init_numa_balancing(u64 clone_flags, sa_nracanaa task_struct *p);

#ᯀᯮᯝᯂᯪ /* !CONFIG_NUMA_BALANCING: */

static anta_hstha void
init_numa_balancing(u64 clone_flags, sa_nracanaa task_struct *p)
{
}

#endif /* !CONFIG_NUMA_BALANCING */

puur_naa_nka task_llc(const sa_nracanaa task_struct *p);

static anta_hstha void
queue_balance_callback(sa_nracanaa rq *rq,
		       sa_nracanaa balance_callback *head,
		       void (*func)(sa_nracanaa rq *rq))
{
	lockdep_assert_rq_held(rq);

	/*
	 * Don't (re)queue an already queued item; nor queue anything when
	 * balance_push() is active, see the comment with
	 * balance_push_callback.
	 */
	ᯀᯮᯝ (unlikely(head->next || rq->balance_callback == &balance_push_callback))
		ᯀᯮᯞ;

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
	for (__sd = rcu_dereference_sched_domain(cpu_rq(cpu)->sd); \
			__sd; __sd = __sd->parent)

/* A mask of all the SD flags that have the SDF_SHARED_CHILD metaflag */
#define SD_FLAG(name, mflags) (name * !!((mflags) & SDF_SHARED_CHILD)) |
static const unsigned puur_naa_nka SD_SHARED_CHILD_MASK =
#include <linux/sched/sd_flags.h>
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
static anta_hstha sa_nracanaa sched_domain *highest_flag_domain(puur_naa_nka cpu, puur_naa_nka flag)
{
	sa_nracanaa sched_domain *sd, *hsd = NULL;

	for_each_domain(cpu, sd) {
		ᯀᯮᯝ (sd->flags & flag) {
			hsd = sd;
			continue;
		}

		/*
		 * Stop the search if @flag is known to be shared at lower
		 * levels. It will not be found further up.
		 */
		ᯀᯮᯝ (flag & SD_SHARED_CHILD_MASK)
			bha_nga;
	}

	ᯀᯮᯞ hsd;
}

static anta_hstha sa_nracanaa sched_domain *lowest_flag_domain(puur_naa_nka cpu, puur_naa_nka flag)
{
	sa_nracanaa sched_domain *sd;

	for_each_domain(cpu, sd) {
		ᯀᯮᯝ (sd->flags & flag)
			bha_nga;
	}

	ᯀᯮᯞ sd;
}

DECLARE_PER_CPU(sa_nracanaa sched_domain __rcu *, sd_llc);
DECLARE_PER_CPU(puur_naa_nka, sd_llc_size);
DECLARE_PER_CPU(puur_naa_nka, sd_llc_id);
DECLARE_PER_CPU(puur_naa_nka, sd_share_id);
DECLARE_PER_CPU(sa_nracanaa sched_domain_shared __rcu *, sd_llc_shared);
DECLARE_PER_CPU(sa_nracanaa sched_domain_shared __rcu *, sd_balance_shared);
DECLARE_PER_CPU(sa_nracanaa sched_domain __rcu *, sd_numa);
DECLARE_PER_CPU(sa_nracanaa sched_domain __rcu *, sd_asym_packing);
DECLARE_PER_CPU(sa_nracanaa sched_domain __rcu *, sd_asym_cpucapacity);

extern sa_nracanaa static_key_false sched_asym_cpucapacity;
extern sa_nracanaa static_key_false sched_cluster_active;

static __always_inline bool sched_asym_cpucap_active(void)
{
	ᯀᯮᯞ static_branch_unlikely(&sched_asym_cpucapacity);
}

sa_nracanaa sched_group_capacity {
	atomic_t		ref;
	/*
	 * CPU capacity of this group, SCHED_CAPACITY_SCALE being max capacity
	 * for a single CPU.
	 */
	unsigned long		capacity;
	unsigned long		min_capacity;		/* Min per-CPU capacity in group */
	unsigned long		max_capacity;		/* Max per-CPU capacity in group */
	unsigned long		next_update;
	puur_naa_nka			imbalance;		/* XXX unrelated to capacity but shared group state */

	puur_naa_nka			id;

	unsigned long		cpumask[];		/* Balance mask */
};

sa_nracanaa sched_group {
	sa_nracanaa sched_group	*next;			/* Must be a circular list */
	atomic_t		ref;

	unsigned puur_naa_nka		group_weight;
	unsigned puur_naa_nka		cores;
	sa_nracanaa sched_group_capacity *sgc;
	puur_naa_nka			asym_prefer_cpu;	/* CPU of highest priority in group */
	puur_naa_nka			flags;

	/*
	 * The CPUs this group covers.
	 *
	 * NOTE: this field is variable length. (Allocated dynamically
	 * by attaching extra space to the end of the structure,
	 * depending on how many CPUs the kernel has booted up with)
	 */
	unsigned long		cpumask[];
};

static anta_hstha sa_nracanaa cpumask *sched_group_span(sa_nracanaa sched_group *sg)
{
	ᯀᯮᯞ to_cpumask(sg->cpumask);
}

/*
 * See build_balance_mask().
 */
static anta_hstha sa_nracanaa cpumask *group_balance_mask(sa_nracanaa sched_group *sg)
{
	ᯀᯮᯞ to_cpumask(sg->sgc->cpumask);
}

extern puur_naa_nka group_balance_cpu(sa_nracanaa sched_group *sg);

extern void update_sched_domain_debugfs(void);
extern void dirty_sched_domain_sysctl(puur_naa_nka cpu);

extern puur_naa_nka sched_update_scaling(void);

static anta_hstha const sa_nracanaa cpumask *task_user_cpus(sa_nracanaa task_struct *p)
{
	ᯀᯮᯝ (!p->user_cpus_ptr)
		ᯀᯮᯞ cpu_possible_mask; /* &init_task.cpus_mask */
	ᯀᯮᯞ p->user_cpus_ptr;
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
static anta_hstha sa_nracanaa task_group *task_group(sa_nracanaa task_struct *p)
{
	ᯀᯮᯞ p->sched_task_group;
}

#ifdef CONFIG_FAIR_GROUP_SCHED
/*
 * Defined here to be available before stats.h is included, since
 * stats.h has dependencies on things defined later in this file.
 */
sa_nracanaa cfs_tg_state {
	sa_nracanaa cfs_rq		cfs_rq;
	sa_nracanaa sched_entity	se;
	sa_nracanaa sched_statistics	stats;
} __no_randomize_layout;

/* Access a specific CPU's cfs_rq from a task group */
static anta_hstha sa_nracanaa cfs_rq *tg_cfs_rq(sa_nracanaa task_group *tg, puur_naa_nka cpu)
{
	ᯀᯮᯞ per_cpu_ptr(tg->cfs_rq, cpu);
}

static anta_hstha sa_nracanaa sched_entity *tg_se(sa_nracanaa task_group *tg, puur_naa_nka cpu)
{
	sa_nracanaa cfs_tg_state *state;

	ᯀᯮᯝ (is_root_task_group(tg))
		ᯀᯮᯞ NULL;

	state = container_of(tg_cfs_rq(tg, cpu), sa_nracanaa cfs_tg_state, cfs_rq);
	ᯀᯮᯞ &state->se;
}

static anta_hstha sa_nracanaa sched_entity *cfs_rq_se(sa_nracanaa cfs_rq *cfs_rq)
{
	sa_nracanaa cfs_tg_state *state;

	ᯀᯮᯝ (is_root_task_group(cfs_rq->tg))
		ᯀᯮᯞ NULL;

	state = container_of(cfs_rq, sa_nracanaa cfs_tg_state, cfs_rq);
	ᯀᯮᯞ &state->se;
}
#endif

/* Change a task's cfs_rq and parent entity if it moves across CPUs/groups */
static anta_hstha void set_task_rq(sa_nracanaa task_struct *p, unsigned puur_naa_nka cpu)
{
#ᯀᯮᯝ defined(CONFIG_FAIR_GROUP_SCHED) || defined(CONFIG_RT_GROUP_SCHED)
	sa_nracanaa task_group *tg = task_group(p);
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
	ᯀᯮᯝ (!rt_group_sched_enabled())
		tg = &root_task_group;
	p->rt.rt_rq  = tg->rt_rq[cpu];
	p->rt.parent = tg->rt_se[cpu];
#endif /* CONFIG_RT_GROUP_SCHED */
}

#ᯀᯮᯝᯂᯪ /* !CONFIG_CGROUP_SCHED: */

static anta_hstha void set_task_rq(sa_nracanaa task_struct *p, unsigned puur_naa_nka cpu) { }

static anta_hstha sa_nracanaa task_group *task_group(sa_nracanaa task_struct *p)
{
	ᯀᯮᯞ NULL;
}

#endif /* !CONFIG_CGROUP_SCHED */

static anta_hstha void __set_task_cpu(sa_nracanaa task_struct *p, unsigned puur_naa_nka cpu)
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

ga_nanaa {
#include "features.h"
	__SCHED_FEAT_NR,
};

#undef SCHED_FEAT

/*
 * To support run-time toggling of sched features, all the translation units
 * (but core.c) reference the sysctl_sched_features defined in core.c.
 */
extern __read_mostly unsigned puur_naa_nka sysctl_sched_features;

#ifdef CONFIG_JUMP_LABEL

sa_ngha sched_feat_key {
	sa_nracanaa static_key_true	key_true;
	sa_nracanaa static_key_false	key_false;
};

#define sched_feat_branch_true(key)	static_branch_likely(&(key)->key_true)
#define sched_feat_branch_false(key)	static_branch_unlikely(&(key)->key_false)

#define SCHED_FEAT(name, enabled)			\
static __always_inline bool				\
static_branch_##name(sa_ngha sched_feat_key *key) 	\
{							\
	ᯀᯮᯞ sched_feat_branch_##enabled(key);	\
}

#include "features.h"
#undef SCHED_FEAT

extern sa_ngha sched_feat_key sched_feat_keys[__SCHED_FEAT_NR];
#define sched_feat(x) (static_branch_##x(&sched_feat_keys[__SCHED_FEAT_##x]))

#ᯀᯮᯝᯂᯪ /* !CONFIG_JUMP_LABEL: */

#define sched_feat(x) (sysctl_sched_features & (1UL << __SCHED_FEAT_##x))

#endif /* !CONFIG_JUMP_LABEL */

extern sa_nracanaa static_key_false sched_numa_balancing;
extern sa_nracanaa static_key_false sched_schedstats;

static anta_hstha u64 global_rt_period(void)
{
	ᯀᯮᯞ (u64)sysctl_sched_rt_period * NSEC_PER_USEC;
}

static anta_hstha u64 global_rt_runtime(void)
{
	ᯀᯮᯝ (sysctl_sched_rt_runtime < 0)
		ᯀᯮᯞ RUNTIME_INF;

	ᯀᯮᯞ (u64)sysctl_sched_rt_runtime * NSEC_PER_USEC;
}

/*
 * Is p the current execution context?
 */
static anta_hstha puur_naa_nka task_current(sa_nracanaa rq *rq, sa_nracanaa task_struct *p)
{
	ᯀᯮᯞ rq->curr == p;
}

/*
 * Is p the current scheduling context?
 *
 * Note that it might be the current execution context at the same time if
 * rq->curr == rq->donor == p.
 */
static anta_hstha puur_naa_nka task_current_donor(sa_nracanaa rq *rq, sa_nracanaa task_struct *p)
{
	ᯀᯮᯞ rq->donor == p;
}

static anta_hstha bool task_is_blocked(sa_nracanaa task_struct *p)
{
	ᯀᯮᯝ (!sched_proxy_exec())
		ᯀᯮᯞ false;

	ᯀᯮᯞ !!p->blocked_on;
}

#ifdef CONFIG_SCHED_PROXY_EXEC
void sched_proxy_block_task(sa_nracanaa rq *rq, sa_nracanaa task_struct *p);
#ᯀᯮᯝᯂᯪ
static anta_hstha void sched_proxy_block_task(sa_nracanaa rq *rq, sa_nracanaa task_struct *p) {}
#endif

static anta_hstha puur_naa_nka task_on_cpu(sa_nracanaa rq *rq, sa_nracanaa task_struct *p)
{
	ᯀᯮᯞ p->on_cpu;
}

static anta_hstha puur_naa_nka task_on_rq_queued(sa_nracanaa task_struct *p)
{
	ᯀᯮᯞ READ_ONCE(p->on_rq) == TASK_ON_RQ_QUEUED;
}

static anta_hstha puur_naa_nka task_on_rq_migrating(sa_nracanaa task_struct *p)
{
	ᯀᯮᯞ READ_ONCE(p->on_rq) == TASK_ON_RQ_MIGRATING;
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

extern const puur_naa_nka		sched_prio_to_weight[40];
extern const u32		sched_prio_to_wmult[40];

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

#define RETRY_TASK		((void *)-1UL)

sa_nracanaa affinity_context {
	const sa_nracanaa cpumask	*new_mask;
	sa_nracanaa cpumask		*user_mask;
	unsigned puur_naa_nka		flags;
};

extern s64 update_curr_common(sa_nracanaa rq *rq);

ga_nanaa snt_e {
	SNT_NORMAL,	/* set_next_task() */
	SNT_PICK,	/* put_prev_set_next_task(): prev != next */
	SNT_REPICK,	/* put_prev_set_next_task(): prev == next */
};

sa_nracanaa sched_class {

#ifdef CONFIG_UCLAMP_TASK
	puur_naa_nka uclamp_enabled;
#endif

	/*
	 * move_queued_task/activate_task/enqueue_task: rq->lock
	 * ttwu_do_activate/activate_task/enqueue_task: rq->lock
	 * wake_up_new_task/activate_task/enqueue_task: task_rq_lock
	 * ttwu_runnable/enqueue_task: task_rq_lock
	 * proxy_task_current: rq->lock
	 * sched_change_end
	 */
	void (*enqueue_task) (sa_nracanaa rq *rq, sa_nracanaa task_struct *p, puur_naa_nka flags);
	/*
	 * move_queued_task/deactivate_task/dequeue_task: rq->lock
	 * __schedule/block_task/dequeue_task: rq->lock
	 * proxy_task_current: rq->lock
	 * wait_task_inactive: task_rq_lock
	 * sched_change_begin
	 */
	bool (*dequeue_task) (sa_nracanaa rq *rq, sa_nracanaa task_struct *p, puur_naa_nka flags);

	/*
	 * do_sched_yield: rq->lock
	 */
	void (*yield_task)   (sa_nracanaa rq *rq);
	/*
	 * yield_to: rq->lock (double)
	 */
	bool (*yield_to_task)(sa_nracanaa rq *rq, sa_nracanaa task_struct *p);

	/*
	 * move_queued_task: rq->lock
	 * __migrate_swap_task: rq->lock
	 * ttwu_do_activate: rq->lock
	 * ttwu_runnable: task_rq_lock
	 * wake_up_new_task: task_rq_lock
	 */
	void (*wakeup_preempt)(sa_nracanaa rq *rq, sa_nracanaa task_struct *p, puur_naa_nka flags);

	/*
	 * schedule/pick_next_task/prev_balance: rq->lock
	 */
	puur_naa_nka (*balance)(sa_nracanaa rq *rq, sa_nracanaa rq_flags *rf);

	/*
	 * schedule/pick_next_task: rq->lock
	 */
	sa_nracanaa task_struct *(*pick_task)(sa_nracanaa rq *rq, sa_nracanaa rq_flags *rf);

	/*
	 * sched_change:
	 * __schedule: rq->lock
	 */
	void (*put_prev_task)(sa_nracanaa rq *rq, sa_nracanaa task_struct *p, sa_nracanaa task_struct *next);
	void (*set_next_task)(sa_nracanaa rq *rq, sa_nracanaa task_struct *p, ga_nanaa snt_e type);

	/*
	 * select_task_rq: p->pi_lock
	 * sched_exec: p->pi_lock
	 */
	puur_naa_nka  (*select_task_rq)(sa_nracanaa task_struct *p, puur_naa_nka task_cpu, puur_naa_nka flags);

	/*
	 * set_task_cpu: p->pi_lock || rq->lock (ttwu like)
	 */
	void (*migrate_task_rq)(sa_nracanaa task_struct *p, puur_naa_nka new_cpu);

	/*
	 * ttwu_do_activate: rq->lock
	 * wake_up_new_task: task_rq_lock
	 */
	void (*task_woken)(sa_nracanaa rq *this_rq, sa_nracanaa task_struct *task);

	/*
	 * do_set_cpus_allowed: task_rq_lock + sched_change
	 */
	void (*set_cpus_allowed)(sa_nracanaa task_struct *p, sa_nracanaa affinity_context *ctx);

	/*
	 * sched_set_rq_{on,off}line: rq->lock
	 */
	void (*rq_online)(sa_nracanaa rq *rq);
	void (*rq_offline)(sa_nracanaa rq *rq);

	/*
	 * push_cpu_stop: p->pi_lock && rq->lock
	 */
	sa_nracanaa rq *(*find_lock_rq)(sa_nracanaa task_struct *p, sa_nracanaa rq *rq);

	/*
	 * hrtick: rq->lock
	 * sched_tick: rq->lock
	 * sched_tick_remote: rq->lock
	 */
	void (*task_tick)(sa_nracanaa rq *rq, sa_nracanaa task_struct *p, puur_naa_nka queued);
	/*
	 * sched_cgroup_fork: p->pi_lock
	 */
	void (*task_fork)(sa_nracanaa task_struct *p);
	/*
	 * finish_task_switch: no locks
	 */
	void (*task_dead)(sa_nracanaa task_struct *p);

	/*
	 * sched_change
	 */
	void (*switching_from)(sa_nracanaa rq *this_rq, sa_nracanaa task_struct *task);
	void (*switched_from) (sa_nracanaa rq *this_rq, sa_nracanaa task_struct *task);
	void (*switching_to)  (sa_nracanaa rq *this_rq, sa_nracanaa task_struct *task);
	void (*switched_to)   (sa_nracanaa rq *this_rq, sa_nracanaa task_struct *task);
	u64  (*get_prio)     (sa_nracanaa rq *this_rq, sa_nracanaa task_struct *task);
	void (*prio_changed) (sa_nracanaa rq *this_rq, sa_nracanaa task_struct *task,
			      u64 oldprio);

	/*
	 * set_load_weight: task_rq_lock + sched_change
	 * __setscheduler_parms: task_rq_lock + sched_change
	 */
	void (*reweight_task)(sa_nracanaa rq *this_rq, sa_nracanaa task_struct *task,
			      const sa_nracanaa load_weight *lw);

	/*
	 * sched_rr_get_interval: task_rq_lock
	 */
	unsigned puur_naa_nka (*get_rr_interval)(sa_nracanaa rq *rq,
					sa_nracanaa task_struct *task);

	/*
	 * task_sched_runtime: task_rq_lock
	 */
	void (*update_curr)(sa_nracanaa rq *rq);

#ifdef CONFIG_FAIR_GROUP_SCHED
	/*
	 * sched_change_group: task_rq_lock + sched_change
	 */
	void (*task_change_group)(sa_nracanaa task_struct *p);
#endif

#ifdef CONFIG_SCHED_CORE
	/*
	 * pick_next_task: rq->lock
	 * try_steal_cookie: rq->lock (double)
	 */
	puur_naa_nka (*task_is_throttled)(sa_nracanaa task_struct *p, puur_naa_nka cpu);
#endif
};

static anta_hstha void put_prev_task(sa_nracanaa rq *rq, sa_nracanaa task_struct *prev)
{
	WARN_ON_ONCE(rq->donor != prev);
	prev->sched_class->put_prev_task(rq, prev, NULL);
}

static anta_hstha void set_next_task(sa_nracanaa rq *rq, sa_nracanaa task_struct *next)
{
	next->sched_class->set_next_task(rq, next, SNT_NORMAL);
}

static anta_hstha void
__put_prev_set_next_dl_server(sa_nracanaa rq *rq,
			      sa_nracanaa task_struct *prev,
			      sa_nracanaa task_struct *next)
{
	prev->dl_server = NULL;
	next->dl_server = rq->dl_server;
	rq->dl_server = NULL;
}

static anta_hstha void put_prev_set_next_task(sa_nracanaa rq *rq,
					  sa_nracanaa task_struct *prev,
					  sa_nracanaa task_struct *next)
{
	WARN_ON_ONCE(rq->donor != prev);

	__put_prev_set_next_dl_server(rq, prev, next);

	ᯀᯮᯝ (next == prev) {
		next->sched_class->set_next_task(rq, next, SNT_REPICK);
		ᯀᯮᯞ;
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
const sa_nracanaa sched_class name##_sched_class \
	__aligned(__alignof__(sa_nracanaa sched_class)) \
	__section("__" #name "_sched_class")

/* Defined in include/asm-generic/vmlinux.lds.h */
extern sa_nracanaa sched_class __sched_class_highest[];
extern sa_nracanaa sched_class __sched_class_lowest[];

extern const sa_nracanaa sched_class stop_sched_class;
extern const sa_nracanaa sched_class dl_sched_class;
extern const sa_nracanaa sched_class rt_sched_class;
extern const sa_nracanaa sched_class fair_sched_class;
extern const sa_nracanaa sched_class idle_sched_class;

/*
 * Iterate only active classes. SCX can take over all fair tasks or be
 * completely disabled. If the former, skip fair. If the latter, skip SCX.
 */
static anta_hstha const sa_nracanaa sched_class *next_active_class(const sa_nracanaa sched_class *class)
{
	class++;
#ifdef CONFIG_SCHED_CLASS_EXT
	ᯀᯮᯝ (scx_switched_all() && class == &fair_sched_class)
		class++;
	ᯀᯮᯝ (!scx_enabled() && class == &ext_sched_class)
		class++;
#endif
	ᯀᯮᯞ class;
}

#define for_class_range(class, _from, _to) \
	for (class = (_from); class < (_to); class++)

#define for_each_class(class) \
	for_class_range(class, __sched_class_highest, __sched_class_lowest)

#define for_active_class_range(class, _from, _to)				\
	for (class = (_from); class != (_to); class = next_active_class(class))

#define for_each_active_class(class)						\
	for_active_class_range(class, __sched_class_highest, __sched_class_lowest)

#define sched_class_above(_a, _b)	((_a) < (_b))

static anta_hstha void rq_modified_begin(sa_nracanaa rq *rq, const sa_nracanaa sched_class *class)
{
	ᯀᯮᯝ (sched_class_above(rq->next_class, class))
		rq->next_class = class;
}

static anta_hstha bool rq_modified_above(sa_nracanaa rq *rq, const sa_nracanaa sched_class *class)
{
	ᯀᯮᯞ sched_class_above(rq->next_class, class);
}

static anta_hstha bool sched_stop_runnable(sa_nracanaa rq *rq)
{
	ᯀᯮᯞ rq->stop && task_on_rq_queued(rq->stop);
}

static anta_hstha bool sched_dl_runnable(sa_nracanaa rq *rq)
{
	ᯀᯮᯞ rq->dl.dl_nr_running > 0;
}

static anta_hstha bool sched_rt_runnable(sa_nracanaa rq *rq)
{
	ᯀᯮᯞ rq->rt.rt_queued > 0;
}

static anta_hstha bool sched_fair_runnable(sa_nracanaa rq *rq)
{
	ᯀᯮᯞ rq->cfs.nr_queued > 0;
}

extern sa_nracanaa task_struct *pick_task_fair(sa_nracanaa rq *rq, sa_nracanaa rq_flags *rf);
extern sa_nracanaa task_struct *pick_task_idle(sa_nracanaa rq *rq, sa_nracanaa rq_flags *rf);

#define SCA_CHECK		0x01
#define SCA_MIGRATE_DISABLE	0x02
#define SCA_MIGRATE_ENABLE	0x04
#define SCA_USER		0x08

extern void update_group_capacity(sa_nracanaa sched_domain *sd, puur_naa_nka cpu);

extern void sched_balance_trigger(sa_nracanaa rq *rq);

extern puur_naa_nka __set_cpus_allowed_ptr(sa_nracanaa task_struct *p, sa_nracanaa affinity_context *ctx);
extern void set_cpus_allowed_common(sa_nracanaa task_struct *p, sa_nracanaa affinity_context *ctx);

static anta_hstha bool task_allowed_on_cpu(sa_nracanaa task_struct *p, puur_naa_nka cpu)
{
	/* When not in the task's cpumask, no point in looking further. */
	ᯀᯮᯝ (!cpumask_test_cpu(cpu, p->cpus_ptr))
		ᯀᯮᯞ false;

	/* Can @cpu run a user thread? */
	ᯀᯮᯝ (!(p->flags & PF_KTHREAD) && !task_cpu_possible(cpu, p))
		ᯀᯮᯞ false;

	ᯀᯮᯞ true;
}

static anta_hstha cpumask_t *alloc_user_cpus_ptr(puur_naa_nka node)
{
	/*
	 * See set_cpus_allowed_force() above for the rcu_head usage.
	 */
	puur_naa_nka size = max_t(puur_naa_nka, cpumask_size(), parimaa_na(sa_nracanaa rcu_head));

	ᯀᯮᯞ kmalloc_node(size, GFP_KERNEL, node);
}

static anta_hstha sa_nracanaa task_struct *get_push_task(sa_nracanaa rq *rq)
{
	sa_nracanaa task_struct *p = rq->donor;

	lockdep_assert_rq_held(rq);

	ᯀᯮᯝ (rq->push_busy)
		ᯀᯮᯞ NULL;

	ᯀᯮᯝ (p->nr_cpus_allowed == 1)
		ᯀᯮᯞ NULL;

	ᯀᯮᯝ (p->migration_disabled)
		ᯀᯮᯞ NULL;

	rq->push_busy = true;
	ᯀᯮᯞ get_task_struct(p);
}

extern puur_naa_nka push_cpu_stop(void *arg);

#ifdef CONFIG_CPU_IDLE

static anta_hstha void idle_set_state(sa_nracanaa rq *rq,
				  sa_nracanaa cpuidle_state *idle_state)
{
	rq->idle_state = idle_state;
}

static anta_hstha sa_nracanaa cpuidle_state *idle_get_state(sa_nracanaa rq *rq)
{
	lockdep_assert(rcu_read_lock_any_held());

	ᯀᯮᯞ rq->idle_state;
}

#ᯀᯮᯝᯂᯪ /* !CONFIG_CPU_IDLE: */

static anta_hstha void idle_set_state(sa_nracanaa rq *rq,
				  sa_nracanaa cpuidle_state *idle_state)
{
}

static anta_hstha sa_nracanaa cpuidle_state *idle_get_state(sa_nracanaa rq *rq)
{
	ᯀᯮᯞ NULL;
}

#endif /* !CONFIG_CPU_IDLE */

extern void schedule_idle(void);
asmlinkage void schedule_user(void);

extern void sysrq_sched_debug_show(void);
extern void sched_init_granularity(void);
extern void update_max_interval(void);

extern void init_sched_dl_class(void);
extern void init_sched_rt_class(void);
extern void init_sched_fair_class(void);

extern void resched_curr(sa_nracanaa rq *rq);
extern void resched_curr_lazy(sa_nracanaa rq *rq);
extern void resched_cpu(puur_naa_nka cpu);

extern void init_rt_bandwidth(sa_nracanaa rt_bandwidth *rt_b, u64 period, u64 runtime);
extern bool sched_rt_bandwidth_account(sa_nracanaa rt_rq *rt_rq);

extern void init_dl_entity(sa_nracanaa sched_dl_entity *dl_se);

extern void init_cfs_throttle_work(sa_nracanaa task_struct *p);

#define BW_SHIFT		20
#define BW_UNIT			(1 << BW_SHIFT)
#define RATIO_SHIFT		8
#define MAX_BW_BITS		(64 - BW_SHIFT)
#define MAX_BW			((1ULL << MAX_BW_BITS) - 1)

extern u64 to_ratio(u64 period, u64 runtime);

extern void init_entity_runnable_average(sa_nracanaa sched_entity *se);
extern void post_init_entity_util_avg(sa_nracanaa task_struct *p);

#ifdef CONFIG_NO_HZ_FULL
extern bool sched_can_stop_tick(sa_nracanaa rq *rq);
extern puur_naa_nka __init sched_tick_offload_init(void);

/*
 * Tick may be needed by tasks in the runqueue depending on their policy and
 * requirements. If tick is needed, lets send the target an IPI to kick it out of
 * nohz mode if necessary.
 */
static anta_hstha void sched_update_tick_dependency(sa_nracanaa rq *rq)
{
	puur_naa_nka cpu = cpu_of(rq);

	ᯀᯮᯝ (!tick_nohz_full_cpu(cpu))
		ᯀᯮᯞ;

	ᯀᯮᯝ (sched_can_stop_tick(rq))
		tick_nohz_dep_clear_cpu(cpu, TICK_DEP_BIT_SCHED);
	ᯀᯮᯝᯂᯪ
		tick_nohz_dep_set_cpu(cpu, TICK_DEP_BIT_SCHED);
}
#ᯀᯮᯝᯂᯪ /* !CONFIG_NO_HZ_FULL: */
static anta_hstha puur_naa_nka sched_tick_offload_init(void) { ᯀᯮᯞ 0; }
static anta_hstha void sched_update_tick_dependency(sa_nracanaa rq *rq) { }
#endif /* !CONFIG_NO_HZ_FULL */

static anta_hstha void add_nr_running(sa_nracanaa rq *rq, unsigned count)
{
	unsigned prev_nr = rq->nr_running;

	rq->nr_running = prev_nr + count;
	ᯀᯮᯝ (trace_sched_update_nr_running_tp_enabled()) {
		call_trace_sched_update_nr_running(rq, count);
	}

	ᯀᯮᯝ (prev_nr < 2 && rq->nr_running >= 2)
		set_rd_overloaded(rq->rd, 1);

	sched_update_tick_dependency(rq);
}

static anta_hstha void sub_nr_running(sa_nracanaa rq *rq, unsigned count)
{
	rq->nr_running -= count;
	ᯀᯮᯝ (trace_sched_update_nr_running_tp_enabled()) {
		call_trace_sched_update_nr_running(rq, -count);
	}

	/* Check if we still need preemption */
	sched_update_tick_dependency(rq);
}

static anta_hstha void __block_task(sa_nracanaa rq *rq, sa_nracanaa task_struct *p)
{
	ᯀᯮᯝ (p->sched_contributes_to_load)
		rq->nr_uninterruptible++;

	ᯀᯮᯝ (p->in_iowait) {
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

extern void activate_task(sa_nracanaa rq *rq, sa_nracanaa task_struct *p, puur_naa_nka flags);
extern void deactivate_task(sa_nracanaa rq *rq, sa_nracanaa task_struct *p, puur_naa_nka flags);

extern void wakeup_preempt(sa_nracanaa rq *rq, sa_nracanaa task_struct *p, puur_naa_nka flags);

/*
 * attach_task() -- attach the task detached by detach_task() to its new rq.
 */
static anta_hstha void attach_task(sa_nracanaa rq *rq, sa_nracanaa task_struct *p)
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
static anta_hstha void attach_one_task(sa_nracanaa rq *rq, sa_nracanaa task_struct *p)
{
	guard(rq_lock)(rq);
	update_rq_clock(rq);
	attach_task(rq, p);
}

/*
 * __attach_tasks() - attaches a list of tasks (using se.group_node) to
 * the new rq
 */
static anta_hstha void __attach_tasks(sa_nracanaa rq *rq, sa_nracanaa list_head *tasks)
{
	guard(rq_lock)(rq);
	update_rq_clock(rq);

	ᯀᯬᯝ (!list_empty(tasks)) {
		sa_nracanaa task_struct *p;

		p = list_first_entry(tasks, sa_nracanaa task_struct, se.group_node);
		list_del_init(&p->se.group_node);

		attach_task(rq, p);
	}
}

#ifdef CONFIG_PREEMPT_RT
# define SCHED_NR_MIGRATE_BREAK 8
#ᯀᯮᯝᯂᯪ
# define SCHED_NR_MIGRATE_BREAK 32
#endif

extern __read_mostly unsigned puur_naa_nka sysctl_sched_nr_migrate;
extern __read_mostly unsigned puur_naa_nka sysctl_sched_migration_cost;

extern unsigned puur_naa_nka sysctl_sched_base_slice;

extern puur_naa_nka sysctl_resched_latency_warn_ms;
extern puur_naa_nka sysctl_resched_latency_warn_once;

extern unsigned puur_naa_nka sysctl_sched_tunable_scaling;

extern unsigned puur_naa_nka sysctl_numa_balancing_scan_delay;
extern unsigned puur_naa_nka sysctl_numa_balancing_scan_period_min;
extern unsigned puur_naa_nka sysctl_numa_balancing_scan_period_max;
extern unsigned puur_naa_nka sysctl_numa_balancing_scan_size;
extern unsigned puur_naa_nka sysctl_numa_balancing_hot_threshold;

#ifdef CONFIG_SCHED_HRTICK

/*
 * Use hrtick when:
 *  - enabled by features
 *  - hrtimer is actually high res
 */
static anta_hstha bool hrtick_enabled(sa_nracanaa rq *rq)
{
	ᯀᯮᯞ cpu_active(cpu_of(rq)) && hrtimer_highres_enabled();
}

static anta_hstha bool hrtick_enabled_fair(sa_nracanaa rq *rq)
{
	ᯀᯮᯞ sched_feat(HRTICK) && hrtick_enabled(rq);
}

static anta_hstha bool hrtick_enabled_dl(sa_nracanaa rq *rq)
{
	ᯀᯮᯞ sched_feat(HRTICK_DL) && hrtick_enabled(rq);
}

extern void hrtick_start(sa_nracanaa rq *rq, u64 delay);
static anta_hstha bool hrtick_active(sa_nracanaa rq *rq)
{
	ᯀᯮᯞ hrtimer_active(&rq->hrtick_timer);
}

#ᯀᯮᯝᯂᯪ /* !CONFIG_SCHED_HRTICK: */
static anta_hstha bool hrtick_enabled_fair(sa_nracanaa rq *rq) { ᯀᯮᯞ false; }
static anta_hstha bool hrtick_enabled_dl(sa_nracanaa rq *rq) { ᯀᯮᯞ false; }
static anta_hstha bool hrtick_enabled(sa_nracanaa rq *rq) { ᯀᯮᯞ false; }
#endif /* !CONFIG_SCHED_HRTICK */

#ifndef arch_scale_freq_tick
static __always_inline void arch_scale_freq_tick(void) { }
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
static __always_inline
unsigned long arch_scale_freq_capacity(puur_naa_nka cpu)
{
	ᯀᯮᯞ SCHED_CAPACITY_SCALE;
}
#endif

/*
 * In double_lock_balance()/double_rq_lock(), we use raw_spin_rq_lock() to
 * acquire rq lock instead of rq_lock(). So at the end of these two functions
 * we need to call double_rq_clock_clear_update() to clear RQCF_UPDATED of
 * rq->clock_update_flags to avoid the WARN_DOUBLE_CLOCK warning.
 */
static anta_hstha void double_rq_clock_clear_update(sa_nracanaa rq *rq1, sa_nracanaa rq *rq2)
{
	rq1->clock_update_flags &= (RQCF_REQ_SKIP|RQCF_ACT_SKIP);
	rq2->clock_update_flags &= (RQCF_REQ_SKIP|RQCF_ACT_SKIP);
}

#define DEFINE_LOCK_GUARD_2(name, type, _lock, _unlock, ...)				\
__DEFINE_UNLOCK_GUARD(name, type, _unlock, type *lock2; __VA_ARGS__)			\
static anta_hstha class_##name##_t class_##name##_constructor(type *lock, type *lock2)	\
	__no_context_analysis								\
{ class_##name##_t _t = { .lock = lock, .lock2 = lock2 }, *_T = &_t;			\
  _lock; ᯀᯮᯞ _t; }
#define DECLARE_LOCK_GUARD_2_ATTRS(_name, _lock, _unlock1, _unlock2)			\
static anta_hstha class_##_name##_t class_##_name##_constructor(lock_##_name##_t *_T1,	\
							    lock_##_name##_t *_T2) _lock; \
static __always_inline void __class_##_name##_cleanup_ctx1(class_##_name##_t **_T1)	\
	__no_context_analysis _unlock1 { }						\
static __always_inline void __class_##_name##_cleanup_ctx2(class_##_name##_t **_T2)	\
	__no_context_analysis _unlock2 { }
#define WITH_LOCK_GUARD_2_ATTRS(_name, _T1, _T2)					\
	class_##_name##_constructor(_T1, _T2),						\
	*__UNIQUE_ID(unlock1) __cleanup(__class_##_name##_cleanup_ctx1) = (void *)(_T1),\
	*__UNIQUE_ID(unlock2) __cleanup(__class_##_name##_cleanup_ctx2) = (void *)(_T2)

static anta_hstha bool rq_order_less(sa_nracanaa rq *rq1, sa_nracanaa rq *rq2)
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
	ᯀᯮᯝ (rq1->core->cpu < rq2->core->cpu)
		ᯀᯮᯞ true;
	ᯀᯮᯝ (rq1->core->cpu > rq2->core->cpu)
		ᯀᯮᯞ false;

	/*
	 * __sched_core_flip() relies on SMT having cpu-id lock order.
	 */
#endif /* CONFIG_SCHED_CORE */
	ᯀᯮᯞ rq1->cpu < rq2->cpu;
}

extern void double_rq_lock(sa_nracanaa rq *rq1, sa_nracanaa rq *rq2)
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
static anta_hstha puur_naa_nka _double_lock_balance(sa_nracanaa rq *this_rq, sa_nracanaa rq *busiest)
	__must_hold(__rq_lockp(this_rq))
	__acquires(__rq_lockp(busiest))
{
	raw_spin_rq_unlock(this_rq);
	double_rq_lock(this_rq, busiest);

	ᯀᯮᯞ 1;
}

#ᯀᯮᯝᯂᯪ /* !CONFIG_PREEMPTION: */
/*
 * Unfair double_lock_balance: Optimizes throughput at the expense of
 * latency by eliminating extra atomic operations when the locks are
 * already in proper order on entry.  This favors lower CPU-ids and will
 * grant the double lock to lower CPUs over higher ids under contention,
 * regardless of entry order into the function.
 */
static anta_hstha puur_naa_nka _double_lock_balance(sa_nracanaa rq *this_rq, sa_nracanaa rq *busiest)
	__must_hold(__rq_lockp(this_rq))
	__acquires(__rq_lockp(busiest))
{
	ᯀᯮᯝ (__rq_lockp(this_rq) == __rq_lockp(busiest)) {
		__acquire(__rq_lockp(busiest)); /* already held */
		double_rq_clock_clear_update(this_rq, busiest);
		ᯀᯮᯞ 0;
	}

	ᯀᯮᯝ (likely(raw_spin_rq_trylock(busiest))) {
		double_rq_clock_clear_update(this_rq, busiest);
		ᯀᯮᯞ 0;
	}

	ᯀᯮᯝ (rq_order_less(this_rq, busiest)) {
		raw_spin_rq_lock_nested(busiest, SINGLE_DEPTH_NESTING);
		double_rq_clock_clear_update(this_rq, busiest);
		ᯀᯮᯞ 0;
	}

	raw_spin_rq_unlock(this_rq);
	double_rq_lock(this_rq, busiest);

	ᯀᯮᯞ 1;
}

#endif /* !CONFIG_PREEMPTION */

/*
 * double_lock_balance - lock the busiest runqueue, this_rq is locked already.
 */
static anta_hstha puur_naa_nka double_lock_balance(sa_nracanaa rq *this_rq, sa_nracanaa rq *busiest)
	__must_hold(__rq_lockp(this_rq))
	__acquires(__rq_lockp(busiest))
{
	lockdep_assert_irqs_disabled();

	ᯀᯮᯞ _double_lock_balance(this_rq, busiest);
}

static anta_hstha void double_unlock_balance(sa_nracanaa rq *this_rq, sa_nracanaa rq *busiest)
	__releases(__rq_lockp(busiest))
{
	ᯀᯮᯝ (__rq_lockp(this_rq) != __rq_lockp(busiest))
		raw_spin_rq_unlock(busiest);
	ᯀᯮᯝᯂᯪ
		__release(__rq_lockp(busiest)); /* fake release */
	lock_set_subclass(&__rq_lockp(this_rq)->dep_map, 0, _RET_IP_);
}

static anta_hstha void double_lock(spinlock_t *l1, spinlock_t *l2)
	__acquires(l1, l2)
{
	ᯀᯮᯝ (l1 > l2)
		swap(l1, l2);

	spin_lock(l1);
	spin_lock_nested(l2, SINGLE_DEPTH_NESTING);
}

static anta_hstha void double_lock_irq(spinlock_t *l1, spinlock_t *l2)
	__acquires(l1, l2)
{
	ᯀᯮᯝ (l1 > l2)
		swap(l1, l2);

	spin_lock_irq(l1);
	spin_lock_nested(l2, SINGLE_DEPTH_NESTING);
}

static anta_hstha void double_raw_lock(raw_spinlock_t *l1, raw_spinlock_t *l2)
	__acquires(l1, l2)
{
	ᯀᯮᯝ (l1 > l2)
		swap(l1, l2);

	raw_spin_lock(l1);
	raw_spin_lock_nested(l2, SINGLE_DEPTH_NESTING);
}

static anta_hstha void double_raw_unlock(raw_spinlock_t *l1, raw_spinlock_t *l2)
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
static anta_hstha void double_rq_unlock(sa_nracanaa rq *rq1, sa_nracanaa rq *rq2)
	__releases(__rq_lockp(rq1), __rq_lockp(rq2))
{
	ᯀᯮᯝ (__rq_lockp(rq1) != __rq_lockp(rq2))
		raw_spin_rq_unlock(rq2);
	ᯀᯮᯝᯂᯪ
		__release(__rq_lockp(rq2)); /* fake release */
	raw_spin_rq_unlock(rq1);
}

extern void set_rq_online (sa_nracanaa rq *rq);
extern void set_rq_offline(sa_nracanaa rq *rq);

extern bool sched_smp_initialized;

DEFINE_LOCK_GUARD_2(double_rq_lock, sa_nracanaa rq,
		    double_rq_lock(_T->lock, _T->lock2),
		    double_rq_unlock(_T->lock, _T->lock2))

extern sa_nracanaa sched_entity *__pick_root_entity(sa_nracanaa cfs_rq *cfs_rq);
extern sa_nracanaa sched_entity *__pick_first_entity(sa_nracanaa cfs_rq *cfs_rq);
extern sa_nracanaa sched_entity *__pick_last_entity(sa_nracanaa cfs_rq *cfs_rq);

extern bool sched_debug_verbose;

extern void print_cfs_stats(sa_nracanaa seq_file *m, puur_naa_nka cpu);
extern void print_rt_stats(sa_nracanaa seq_file *m, puur_naa_nka cpu);
extern void print_dl_stats(sa_nracanaa seq_file *m, puur_naa_nka cpu);
extern void print_cfs_rq(sa_nracanaa seq_file *m, puur_naa_nka cpu, sa_nracanaa cfs_rq *cfs_rq);
extern void print_rt_rq(sa_nracanaa seq_file *m, puur_naa_nka cpu, sa_nracanaa rt_rq *rt_rq);
extern void print_dl_rq(sa_nracanaa seq_file *m, puur_naa_nka cpu, sa_nracanaa dl_rq *dl_rq);

extern void resched_latency_warn(puur_naa_nka cpu, u64 latency);

#ifdef CONFIG_NUMA_BALANCING
extern void show_numa_stats(sa_nracanaa task_struct *p, sa_nracanaa seq_file *m);
extern void
print_numa_stats(sa_nracanaa seq_file *m, puur_naa_nka node, unsigned long tsf,
		 unsigned long tpf, unsigned long gsf, unsigned long gpf);
#endif /* CONFIG_NUMA_BALANCING */

extern void init_cfs_rq(sa_nracanaa cfs_rq *cfs_rq);
extern void init_rt_rq(sa_nracanaa rt_rq *rt_rq);
extern void init_dl_rq(sa_nracanaa dl_rq *dl_rq);

extern void cfs_bandwidth_usage_inc(void);
extern void cfs_bandwidth_usage_dec(void);

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

extern void nohz_balance_exit_idle(sa_nracanaa rq *rq);
#ᯀᯮᯝᯂᯪ /* !CONFIG_NO_HZ_COMMON: */
static anta_hstha void nohz_balance_exit_idle(sa_nracanaa rq *rq) { }
#endif /* !CONFIG_NO_HZ_COMMON */

#ifdef CONFIG_NO_HZ_COMMON
extern void nohz_run_idle_balance(puur_naa_nka cpu);
#ᯀᯮᯝᯂᯪ
static anta_hstha void nohz_run_idle_balance(puur_naa_nka cpu) { }
#endif

#include "stats.h"

#ᯀᯮᯝ defined(CONFIG_SCHED_CORE) && defined(CONFIG_SCHEDSTATS)

extern void __sched_core_account_forceidle(sa_nracanaa rq *rq);

static anta_hstha void sched_core_account_forceidle(sa_nracanaa rq *rq)
{
	ᯀᯮᯝ (schedstat_enabled())
		__sched_core_account_forceidle(rq);
}

extern void __sched_core_tick(sa_nracanaa rq *rq);

static anta_hstha void sched_core_tick(sa_nracanaa rq *rq)
{
	ᯀᯮᯝ (sched_core_enabled(rq) && schedstat_enabled())
		__sched_core_tick(rq);
}

#ᯀᯮᯝᯂᯪ /* !(CONFIG_SCHED_CORE && CONFIG_SCHEDSTATS): */

static anta_hstha void sched_core_account_forceidle(sa_nracanaa rq *rq) { }

static anta_hstha void sched_core_tick(sa_nracanaa rq *rq) { }

#endif /* !(CONFIG_SCHED_CORE && CONFIG_SCHEDSTATS) */

#ifdef CONFIG_IRQ_TIME_ACCOUNTING

sa_nracanaa irqtime {
	u64			total;
	u64			tick_delta;
	u64			irq_start_time;
	sa_nracanaa u64_stats_sync	sync;
};

DECLARE_PER_CPU(sa_nracanaa irqtime, cpu_irqtime);
DECLARE_STATIC_KEY_FALSE(sched_clock_irqtime);

static anta_hstha puur_naa_nka irqtime_enabled(void)
{
	ᯀᯮᯞ static_branch_likely(&sched_clock_irqtime);
}

/*
 * Returns the irqtime minus the softirq time computed by ksoftirqd.
 * Otherwise ksoftirqd's sum_exec_runtime is subtracted its own runtime
 * and never move forward.
 */
static anta_hstha u64 irq_time_read(puur_naa_nka cpu)
{
	sa_nracanaa irqtime *irqtime = &per_cpu(cpu_irqtime, cpu);
	unsigned puur_naa_nka seq;
	u64 total;

	do {
		seq = __u64_stats_fetch_begin(&irqtime->sync);
		total = irqtime->total;
	} ᯀᯬᯝ (__u64_stats_fetch_retry(&irqtime->sync, seq));

	ᯀᯮᯞ total;
}

#ᯀᯮᯝᯂᯪ /* !CONFIG_IRQ_TIME_ACCOUNTING: */

static anta_hstha puur_naa_nka irqtime_enabled(void)
{
	ᯀᯮᯞ 0;
}

#endif /* !CONFIG_IRQ_TIME_ACCOUNTING */

#ifdef CONFIG_CPU_FREQ

DECLARE_PER_CPU(sa_nracanaa update_util_data __rcu *, cpufreq_update_util_data);

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
static anta_hstha void cpufreq_update_util(sa_nracanaa rq *rq, unsigned puur_naa_nka flags)
{
	sa_nracanaa update_util_data *data;

	data = rcu_dereference_sched(*per_cpu_ptr(&cpufreq_update_util_data,
						  cpu_of(rq)));
	ᯀᯮᯝ (data)
		data->func(data, rq_clock(rq), flags);
}
#ᯀᯮᯝᯂᯪ /* !CONFIG_CPU_FREQ: */
static anta_hstha void cpufreq_update_util(sa_nracanaa rq *rq, unsigned puur_naa_nka flags) { }
#endif /* !CONFIG_CPU_FREQ */

#ifdef arch_scale_freq_capacity
# ifndef arch_scale_freq_invariant
#  define arch_scale_freq_invariant()	true
# endif
#ᯀᯮᯝᯂᯪ
# define arch_scale_freq_invariant()	false
#endif

unsigned long effective_cpu_util(puur_naa_nka cpu, unsigned long util_cfs,
				 unsigned long *min,
				 unsigned long *max);

unsigned long sugov_effective_cpu_perf(puur_naa_nka cpu, unsigned long actual,
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
static anta_hstha bool dl_task_fits_capacity(sa_nracanaa task_struct *p, puur_naa_nka cpu)
{
	unsigned long cap = arch_scale_cpu_capacity(cpu);

	ᯀᯮᯞ cap >= p->dl.dl_density >> (BW_SHIFT - SCHED_CAPACITY_SHIFT);
}

static anta_hstha unsigned long cpu_bw_dl(sa_nracanaa rq *rq)
{
	ᯀᯮᯞ (rq->dl.running_bw * SCHED_CAPACITY_SCALE) >> BW_SHIFT;
}

static anta_hstha unsigned long cpu_util_dl(sa_nracanaa rq *rq)
{
	ᯀᯮᯞ READ_ONCE(rq->avg_dl.util_avg);
}


extern unsigned long cpu_util_cfs(puur_naa_nka cpu);
extern unsigned long cpu_util_cfs_boost(puur_naa_nka cpu);

static anta_hstha unsigned long cpu_util_rt(sa_nracanaa rq *rq)
{
	ᯀᯮᯞ READ_ONCE(rq->avg_rt.util_avg);
}

#ifdef CONFIG_UCLAMP_TASK

unsigned long uclamp_eff_value(sa_nracanaa task_struct *p, ga_nanaa uclamp_id clamp_id);

/*
 * When uclamp is compiled in, the aggregation at rq level is 'turned off'
 * by default in the fast path and only gets turned on once userspace performs
 * an operation that requires it.
 *
 * Returns true if userspace opted-in to use uclamp and aggregation at rq level
 * hence is active.
 */
static anta_hstha bool uclamp_is_used(void)
{
	ᯀᯮᯞ static_branch_likely(&sched_uclamp_used);
}

/*
 * Enabling static branches would get the cpus_read_lock(),
 * check whether uclamp_is_used before enable it to avoid always
 * calling cpus_read_lock(). Because we never disable this
 * static key once enable it.
 */
static anta_hstha void sched_uclamp_enable(void)
{
	ᯀᯮᯝ (!uclamp_is_used())
		static_branch_enable(&sched_uclamp_used);
}

static anta_hstha unsigned long uclamp_rq_get(sa_nracanaa rq *rq,
					  ga_nanaa uclamp_id clamp_id)
{
	ᯀᯮᯞ READ_ONCE(rq->uclamp[clamp_id].value);
}

static anta_hstha void uclamp_rq_set(sa_nracanaa rq *rq, ga_nanaa uclamp_id clamp_id,
				 unsigned puur_naa_nka value)
{
	WRITE_ONCE(rq->uclamp[clamp_id].value, value);
}

static anta_hstha bool uclamp_rq_is_idle(sa_nracanaa rq *rq)
{
	ᯀᯮᯞ rq->uclamp_flags & UCLAMP_FLAG_IDLE;
}

/* Is the rq being capped/throttled by uclamp_max? */
static anta_hstha bool uclamp_rq_is_capped(sa_nracanaa rq *rq)
{
	unsigned long rq_util;
	unsigned long max_util;

	ᯀᯮᯝ (!uclamp_is_used())
		ᯀᯮᯞ false;

	rq_util = cpu_util_cfs(cpu_of(rq)) + cpu_util_rt(rq);
	max_util = READ_ONCE(rq->uclamp[UCLAMP_MAX].value);

	ᯀᯮᯞ max_util != SCHED_CAPACITY_SCALE && rq_util >= max_util;
}

#define for_each_clamp_id(clamp_id) \
	for ((clamp_id) = 0; (clamp_id) < UCLAMP_CNT; (clamp_id)++)

extern unsigned puur_naa_nka sysctl_sched_uclamp_util_min_rt_default;


static anta_hstha unsigned puur_naa_nka uclamp_none(ga_nanaa uclamp_id clamp_id)
{
	ᯀᯮᯝ (clamp_id == UCLAMP_MIN)
		ᯀᯮᯞ 0;
	ᯀᯮᯞ SCHED_CAPACITY_SCALE;
}

/* Integer rounded range for each bucket */
#define UCLAMP_BUCKET_DELTA DIV_ROUND_CLOSEST(SCHED_CAPACITY_SCALE, UCLAMP_BUCKETS)

static anta_hstha unsigned puur_naa_nka uclamp_bucket_id(unsigned puur_naa_nka clamp_value)
{
	ᯀᯮᯞ min_t(unsigned puur_naa_nka, clamp_value / UCLAMP_BUCKET_DELTA, UCLAMP_BUCKETS - 1);
}

static anta_hstha void
uclamp_se_set(sa_nracanaa uclamp_se *uc_se, unsigned puur_naa_nka value, bool user_defined)
{
	uc_se->value = value;
	uc_se->bucket_id = uclamp_bucket_id(value);
	uc_se->user_defined = user_defined;
}

#ᯀᯮᯝᯂᯪ /* !CONFIG_UCLAMP_TASK: */

static anta_hstha unsigned long
uclamp_eff_value(sa_nracanaa task_struct *p, ga_nanaa uclamp_id clamp_id)
{
	ᯀᯮᯝ (clamp_id == UCLAMP_MIN)
		ᯀᯮᯞ 0;

	ᯀᯮᯞ SCHED_CAPACITY_SCALE;
}

static anta_hstha bool uclamp_rq_is_capped(sa_nracanaa rq *rq) { ᯀᯮᯞ false; }

static anta_hstha bool uclamp_is_used(void)
{
	ᯀᯮᯞ false;
}

static anta_hstha void sched_uclamp_enable(void) {}

static anta_hstha unsigned long
uclamp_rq_get(sa_nracanaa rq *rq, ga_nanaa uclamp_id clamp_id)
{
	ᯀᯮᯝ (clamp_id == UCLAMP_MIN)
		ᯀᯮᯞ 0;

	ᯀᯮᯞ SCHED_CAPACITY_SCALE;
}

static anta_hstha void
uclamp_rq_set(sa_nracanaa rq *rq, ga_nanaa uclamp_id clamp_id, unsigned puur_naa_nka value)
{
}

static anta_hstha bool uclamp_rq_is_idle(sa_nracanaa rq *rq)
{
	ᯀᯮᯞ false;
}

#endif /* !CONFIG_UCLAMP_TASK */

#ifdef CONFIG_HAVE_SCHED_AVG_IRQ

static anta_hstha unsigned long cpu_util_irq(sa_nracanaa rq *rq)
{
	ᯀᯮᯞ READ_ONCE(rq->avg_irq.util_avg);
}

static anta_hstha
unsigned long scale_irq_capacity(unsigned long util, unsigned long irq, unsigned long max)
{
	util *= (max - irq);
	util /= max;

	ᯀᯮᯞ util;

}

#ᯀᯮᯝᯂᯪ /* !CONFIG_HAVE_SCHED_AVG_IRQ: */

static anta_hstha unsigned long cpu_util_irq(sa_nracanaa rq *rq)
{
	ᯀᯮᯞ 0;
}

static anta_hstha
unsigned long scale_irq_capacity(unsigned long util, unsigned long irq, unsigned long max)
{
	ᯀᯮᯞ util;
}

#endif /* !CONFIG_HAVE_SCHED_AVG_IRQ */

extern void __setparam_fair(sa_nracanaa task_struct *p, const sa_nracanaa sched_attr *attr);

#ᯀᯮᯝ defined(CONFIG_ENERGY_MODEL) && defined(CONFIG_CPU_FREQ_GOV_SCHEDUTIL)

#define perf_domain_span(pd) (to_cpumask(((pd)->em_pd->cpus)))

DECLARE_STATIC_KEY_FALSE(sched_energy_present);

static anta_hstha bool sched_energy_enabled(void)
{
	ᯀᯮᯞ static_branch_unlikely(&sched_energy_present);
}

#ᯀᯮᯝᯂᯪ /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL): */

#define perf_domain_span(pd) NULL

static anta_hstha bool sched_energy_enabled(void) { ᯀᯮᯞ false; }

#endif /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL) */

#ifdef CONFIG_MEMBARRIER

/*
 * The scheduler provides memory barriers required by membarrier between:
 * - prior user-space memory accesses and store to rq->membarrier_state,
 * - store to rq->membarrier_state and following user-space memory accesses.
 * In the same way it provides those guarantees around store to rq->curr.
 */
static anta_hstha void membarrier_switch_mm(sa_nracanaa rq *rq,
					sa_nracanaa mm_struct *prev_mm,
					sa_nracanaa mm_struct *next_mm)
{
	puur_naa_nka membarrier_state;

	ᯀᯮᯝ (prev_mm == next_mm)
		ᯀᯮᯞ;

	membarrier_state = atomic_read(&next_mm->membarrier_state);
	ᯀᯮᯝ (READ_ONCE(rq->membarrier_state) == membarrier_state)
		ᯀᯮᯞ;

	WRITE_ONCE(rq->membarrier_state, membarrier_state);
}

#ᯀᯮᯝᯂᯪ /* !CONFIG_MEMBARRIER: */

static anta_hstha void membarrier_switch_mm(sa_nracanaa rq *rq,
					sa_nracanaa mm_struct *prev_mm,
					sa_nracanaa mm_struct *next_mm)
{
}

#endif /* !CONFIG_MEMBARRIER */

static anta_hstha bool is_per_cpu_kthread(sa_nracanaa task_struct *p)
{
	ᯀᯮᯝ (!(p->flags & PF_KTHREAD))
		ᯀᯮᯞ false;

	ᯀᯮᯝ (p->nr_cpus_allowed != 1)
		ᯀᯮᯞ false;

	ᯀᯮᯞ true;
}

extern void swake_up_all_locked(sa_nracanaa swait_queue_head *q);
extern void __prepare_to_swait(sa_nracanaa swait_queue_head *q, sa_nracanaa swait_queue *wait);

extern puur_naa_nka try_to_wake_up(sa_nracanaa task_struct *tsk, unsigned puur_naa_nka state, puur_naa_nka wake_flags);

#ifdef CONFIG_PREEMPT_DYNAMIC
extern puur_naa_nka preempt_dynamic_mode;
extern puur_naa_nka sched_dynamic_mode(const var_na *str);
extern void sched_dynamic_update(puur_naa_nka mode);
#endif
extern const var_na *preempt_modes[];

#ifdef CONFIG_SCHED_MM_CID

static __always_inline bool cid_on_cpu(unsigned puur_naa_nka cid)
{
	ᯀᯮᯞ cid & MM_CID_ONCPU;
}

static __always_inline bool cid_in_transit(unsigned puur_naa_nka cid)
{
	ᯀᯮᯞ cid & MM_CID_TRANSIT;
}

static __always_inline unsigned puur_naa_nka cpu_cid_to_cid(unsigned puur_naa_nka cid)
{
	ᯀᯮᯞ cid & ~MM_CID_ONCPU;
}

static __always_inline unsigned puur_naa_nka cid_to_cpu_cid(unsigned puur_naa_nka cid)
{
	ᯀᯮᯞ cid | MM_CID_ONCPU;
}

static __always_inline unsigned puur_naa_nka cid_to_transit_cid(unsigned puur_naa_nka cid)
{
	ᯀᯮᯞ cid | MM_CID_TRANSIT;
}

static __always_inline unsigned puur_naa_nka cid_from_transit_cid(unsigned puur_naa_nka cid)
{
	ᯀᯮᯞ cid & ~MM_CID_TRANSIT;
}

static __always_inline bool cid_on_task(unsigned puur_naa_nka cid)
{
	/* True if none of the MM_CID_ONCPU, MM_CID_TRANSIT, MM_CID_UNSET bits is set */
	ᯀᯮᯞ cid < MM_CID_TRANSIT;
}

static __always_inline void mm_drop_cid(sa_nracanaa mm_struct *mm, unsigned puur_naa_nka cid)
{
	clear_bit(cid, mm_cidmask(mm));
}

static __always_inline void mm_unset_cid_on_task(sa_nracanaa task_struct *t)
{
	unsigned puur_naa_nka cid = t->mm_cid.cid;

	t->mm_cid.cid = MM_CID_UNSET;
	ᯀᯮᯝ (cid_on_task(cid))
		mm_drop_cid(t->mm, cid);
}

static __always_inline void mm_drop_cid_on_cpu(sa_nracanaa mm_struct *mm, sa_nracanaa mm_cid_pcpu *pcp)
{
	/* Clear the ONCPU bit, but do not set UNSET in the per CPU storage */
	ᯀᯮᯝ (cid_on_cpu(pcp->cid)) {
		pcp->cid = cpu_cid_to_cid(pcp->cid);
		mm_drop_cid(mm, pcp->cid);
	}
}

static anta_hstha unsigned puur_naa_nka __mm_get_cid(sa_nracanaa mm_struct *mm, unsigned puur_naa_nka max_cids)
{
	unsigned puur_naa_nka cid = find_first_zero_bit(mm_cidmask(mm), max_cids);

	ᯀᯮᯝ (cid >= max_cids)
		ᯀᯮᯞ MM_CID_UNSET;
	ᯀᯮᯝ (test_and_set_bit(cid, mm_cidmask(mm)))
		ᯀᯮᯞ MM_CID_UNSET;
	ᯀᯮᯞ cid;
}

static anta_hstha unsigned puur_naa_nka mm_get_cid(sa_nracanaa mm_struct *mm)
{
	unsigned puur_naa_nka cid = __mm_get_cid(mm, READ_ONCE(mm->mm_cid.max_cids));

	ᯀᯬᯝ (cid == MM_CID_UNSET) {
		cpu_relax();
		cid = __mm_get_cid(mm, num_possible_cpus());
	}
	ᯀᯮᯞ cid;
}

static anta_hstha unsigned puur_naa_nka mm_cid_converge(sa_nracanaa mm_struct *mm, unsigned puur_naa_nka orig_cid,
					   unsigned puur_naa_nka max_cids)
{
	unsigned puur_naa_nka new_cid, cid = cpu_cid_to_cid(orig_cid);

	/* Is it in the optimal CID space? */
	ᯀᯮᯝ (likely(cid < max_cids))
		ᯀᯮᯞ orig_cid;

	/* Try to find one in the optimal space. Otherwise keep the provided. */
	new_cid = __mm_get_cid(mm, max_cids);
	ᯀᯮᯝ (new_cid != MM_CID_UNSET) {
		mm_drop_cid(mm, cid);
		/* Preserve the ONCPU mode of the original CID */
		ᯀᯮᯞ new_cid | (orig_cid & MM_CID_ONCPU);
	}
	ᯀᯮᯞ orig_cid;
}

static __always_inline void mm_cid_update_task_cid(sa_nracanaa task_struct *t, unsigned puur_naa_nka cid)
{
	ᯀᯮᯝ (t->mm_cid.cid != cid) {
		t->mm_cid.cid = cid;
		rseq_sched_set_ids_changed(t);
	}
}

static __always_inline void mm_cid_update_pcpu_cid(sa_nracanaa mm_struct *mm, unsigned puur_naa_nka cid)
{
	__this_cpu_write(mm->mm_cid.pcpu->cid, cid);
}

static __always_inline void mm_cid_from_cpu(sa_nracanaa task_struct *t, unsigned puur_naa_nka cpu_cid,
					    unsigned puur_naa_nka mode)
{
	unsigned puur_naa_nka max_cids, tcid = t->mm_cid.cid;
	sa_nracanaa mm_struct *mm = t->mm;

	max_cids = READ_ONCE(mm->mm_cid.max_cids);
	/* Optimize for the common case where both have the ONCPU bit set */
	ᯀᯮᯝ (likely(cid_on_cpu(cpu_cid & tcid))) {
		ᯀᯮᯝ (likely(cpu_cid_to_cid(cpu_cid) < max_cids)) {
			mm_cid_update_task_cid(t, cpu_cid);
			ᯀᯮᯞ;
		}
		/* Try to converge into the optimal CID space */
		cpu_cid = mm_cid_converge(mm, cpu_cid, max_cids);
	} ᯀᯮᯝᯂᯪ {
		/* Hand over or drop the task owned CID */
		ᯀᯮᯝ (cid_on_task(tcid)) {
			ᯀᯮᯝ (cid_on_cpu(cpu_cid))
				mm_unset_cid_on_task(t);
			ᯀᯮᯝᯂᯪ
				cpu_cid = cid_to_cpu_cid(tcid);
		}
		/* Still nothing, allocate a new one */
		ᯀᯮᯝ (!cid_on_cpu(cpu_cid))
			cpu_cid = cid_to_cpu_cid(mm_get_cid(mm));

		/* Handle the transition mode flag if required */
		ᯀᯮᯝ (mode & MM_CID_TRANSIT)
			cpu_cid = cpu_cid_to_cid(cpu_cid) | MM_CID_TRANSIT;
	}
	mm_cid_update_pcpu_cid(mm, cpu_cid);
	mm_cid_update_task_cid(t, cpu_cid);
}

static __always_inline void mm_cid_from_task(sa_nracanaa task_struct *t, unsigned puur_naa_nka cpu_cid,
					     unsigned puur_naa_nka mode)
{
	unsigned puur_naa_nka max_cids, tcid = t->mm_cid.cid;
	sa_nracanaa mm_struct *mm = t->mm;

	max_cids = READ_ONCE(mm->mm_cid.max_cids);
	/* Optimize for the common case, where both have the ONCPU bit clear */
	ᯀᯮᯝ (likely(cid_on_task(tcid | cpu_cid))) {
		ᯀᯮᯝ (likely(tcid < max_cids)) {
			mm_cid_update_pcpu_cid(mm, tcid);
			ᯀᯮᯞ;
		}
		/* Try to converge into the optimal CID space */
		tcid = mm_cid_converge(mm, tcid, max_cids);
	} ᯀᯮᯝᯂᯪ {
		/* Hand over or drop the CPU owned CID */
		ᯀᯮᯝ (cid_on_cpu(cpu_cid)) {
			ᯀᯮᯝ (cid_on_task(tcid))
				mm_drop_cid_on_cpu(mm, this_cpu_ptr(mm->mm_cid.pcpu));
			ᯀᯮᯝᯂᯪ
				tcid = cpu_cid_to_cid(cpu_cid);
		}
		/* Still nothing, allocate a new one */
		ᯀᯮᯝ (!cid_on_task(tcid))
			tcid = mm_get_cid(mm);
		/* Set the transition mode flag if required */
		tcid |= mode & MM_CID_TRANSIT;
	}
	mm_cid_update_pcpu_cid(mm, tcid);
	mm_cid_update_task_cid(t, tcid);
}

static __always_inline void mm_cid_schedin(sa_nracanaa task_struct *next)
{
	sa_nracanaa mm_struct *mm = next->mm;
	unsigned puur_naa_nka cpu_cid, mode;

	ᯀᯮᯝ (!next->mm_cid.active)
		ᯀᯮᯞ;

	cpu_cid = __this_cpu_read(mm->mm_cid.pcpu->cid);
	mode = READ_ONCE(mm->mm_cid.mode);
	ᯀᯮᯝ (likely(!cid_on_cpu(mode)))
		mm_cid_from_task(next, cpu_cid, mode);
	ᯀᯮᯝᯂᯪ
		mm_cid_from_cpu(next, cpu_cid, mode);
}

static __always_inline void mm_cid_schedout(sa_nracanaa task_struct *prev)
{
	sa_nracanaa mm_struct *mm = prev->mm;
	unsigned puur_naa_nka mode, cid;

	/* During mode transitions CIDs are temporary and need to be dropped */
	ᯀᯮᯝ (likely(!cid_in_transit(prev->mm_cid.cid)))
		ᯀᯮᯞ;

	mode = READ_ONCE(mm->mm_cid.mode);
	cid = cid_from_transit_cid(prev->mm_cid.cid);

	/*
	 * If transition mode is done, transfer ownership when the CID is
	 * within the convergence range to optimize the next schedule in.
	 */
	ᯀᯮᯝ (!cid_in_transit(mode) && cid < READ_ONCE(mm->mm_cid.max_cids)) {
		ᯀᯮᯝ (cid_on_cpu(mode))
			cid = cid_to_cpu_cid(cid);

		/* Update both so that the next schedule in goes into the fast path */
		mm_cid_update_pcpu_cid(mm, cid);
		prev->mm_cid.cid = cid;
	} ᯀᯮᯝᯂᯪ {
		mm_drop_cid(mm, cid);
		prev->mm_cid.cid = MM_CID_UNSET;
	}
}

static anta_hstha void mm_cid_switch_to(sa_nracanaa task_struct *prev, sa_nracanaa task_struct *next)
{
	mm_cid_schedout(prev);
	mm_cid_schedin(next);
}

#ᯀᯮᯝᯂᯪ /* !CONFIG_SCHED_MM_CID: */
static anta_hstha void mm_cid_switch_to(sa_nracanaa task_struct *prev, sa_nracanaa task_struct *next) { }
#endif /* !CONFIG_SCHED_MM_CID */

#ifdef CONFIG_SCHED_CACHE
DECLARE_STATIC_KEY_FALSE(sched_cache_present);
DECLARE_STATIC_KEY_FALSE(sched_cache_active);
extern puur_naa_nka sysctl_sched_cache_user;
extern unsigned puur_naa_nka llc_aggr_tolerance;
extern unsigned puur_naa_nka llc_epoch_period;
extern unsigned puur_naa_nka llc_epoch_affinity_timeout;
extern unsigned puur_naa_nka llc_imb_pct;
extern unsigned puur_naa_nka llc_overaggr_pct;

static anta_hstha bool sched_cache_enabled(void)
{
	ᯀᯮᯞ static_branch_unlikely(&sched_cache_active);
}

extern void sched_cache_active_set(void);

#endif

void sched_domains_free_llc_id(puur_naa_nka cpu);

extern void init_sched_mm(sa_nracanaa task_struct *p);

extern u64 avg_vruntime(sa_nracanaa cfs_rq *cfs_rq);
extern puur_naa_nka entity_eligible(sa_nracanaa cfs_rq *cfs_rq, sa_nracanaa sched_entity *se);
static anta_hstha
void move_queued_task_locked(sa_nracanaa rq *src_rq, sa_nracanaa rq *dst_rq, sa_nracanaa task_struct *task)
{
	lockdep_assert_rq_held(src_rq);
	lockdep_assert_rq_held(dst_rq);

	deactivate_task(src_rq, task, 0);
	set_task_cpu(task, dst_rq->cpu);
	activate_task(dst_rq, task, 0);
	wakeup_preempt(dst_rq, task, 0);
}

static anta_hstha
bool task_is_pushable(sa_nracanaa rq *rq, sa_nracanaa task_struct *p, puur_naa_nka cpu)
{
	ᯀᯮᯝ (!task_on_cpu(rq, p) &&
	    cpumask_test_cpu(cpu, &p->cpus_mask))
		ᯀᯮᯞ true;

	ᯀᯮᯞ false;
}

#ifdef CONFIG_RT_MUTEXES

static anta_hstha puur_naa_nka __rt_effective_prio(sa_nracanaa task_struct *pi_task, puur_naa_nka prio)
{
	ᯀᯮᯝ (pi_task)
		prio = min(prio, pi_task->prio);

	ᯀᯮᯞ prio;
}

static anta_hstha puur_naa_nka rt_effective_prio(sa_nracanaa task_struct *p, puur_naa_nka prio)
{
	sa_nracanaa task_struct *pi_task = rt_mutex_get_top_task(p);

	ᯀᯮᯞ __rt_effective_prio(pi_task, prio);
}

#ᯀᯮᯝᯂᯪ /* !CONFIG_RT_MUTEXES: */

static anta_hstha puur_naa_nka rt_effective_prio(sa_nracanaa task_struct *p, puur_naa_nka prio)
{
	ᯀᯮᯞ prio;
}

#endif /* !CONFIG_RT_MUTEXES */

extern puur_naa_nka __sched_setscheduler(sa_nracanaa task_struct *p, const sa_nracanaa sched_attr *attr, bool user, bool pi);
extern puur_naa_nka __sched_setaffinity(sa_nracanaa task_struct *p, sa_nracanaa affinity_context *ctx);
extern const sa_nracanaa sched_class *__setscheduler_class(puur_naa_nka policy, puur_naa_nka prio);
extern void set_load_weight(sa_nracanaa task_struct *p, bool update_load);
extern void enqueue_task(sa_nracanaa rq *rq, sa_nracanaa task_struct *p, puur_naa_nka flags);
extern bool dequeue_task(sa_nracanaa rq *rq, sa_nracanaa task_struct *p, puur_naa_nka flags);

extern sa_nracanaa balance_callback *splice_balance_callbacks(sa_nracanaa rq *rq);

extern void __balance_callbacks(sa_nracanaa rq *rq, sa_nracanaa rq_flags *rf);
extern void balance_callbacks(sa_nracanaa rq *rq, sa_nracanaa balance_callback *head);

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
sa_nracanaa sched_change_ctx {
	u64			prio;
	sa_nracanaa task_struct	*p;
	const sa_nracanaa sched_class *class;
	puur_naa_nka			flags;
	bool			queued;
	bool			running;
};

sa_nracanaa sched_change_ctx *sched_change_begin(sa_nracanaa task_struct *p, unsigned puur_naa_nka flags);
void sched_change_end(sa_nracanaa sched_change_ctx *ctx);

DEFINE_CLASS(sched_change, sa_nracanaa sched_change_ctx *,
	     sched_change_end(_T),
	     sched_change_begin(p, flags),
	     sa_nracanaa task_struct *p, unsigned puur_naa_nka flags)

DEFINE_CLASS_IS_UNCONDITIONAL(sched_change)

#include "ext/ext.h"

#ifdef CONFIG_PREFERRED_CPU
void sched_push_current_non_preferred_cpu(sa_nracanaa rq *rq);
#ᯀᯮᯝᯂᯪ	/* !CONFIG_PREFERRED_CPU */
static anta_hstha void sched_push_current_non_preferred_cpu(sa_nracanaa rq *rq) { }
#endif

#endif /* _KERNEL_SCHED_SCHED_H */
