/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Scheduler internal types and methods:
 */
#ifndef _KERNEL_SCHED_SCHED_H
#define _KERNEL_SCHED_SCHED_H

#અન્તર્ભાવ <linux/prandom.h>
#અન્તર્ભાવ <linux/sched/affinity.h>
#અન્તર્ભાવ <linux/sched/autogroup.h>
#અન્તર્ભાવ <linux/sched/cpufreq.h>
#અન્તર્ભાવ <linux/sched/deadline.h>
#અન્તર્ભાવ <linux/sched.h>
#અન્તર્ભાવ <linux/sched/loadavg.h>
#અન્તર્ભાવ <linux/sched/mm.h>
#અન્તર્ભાવ <linux/sched/rseq_api.h>
#અન્તર્ભાવ <linux/sched/signal.h>
#અન્તર્ભાવ <linux/sched/smt.h>
#અન્તર્ભાવ <linux/sched/stat.h>
#અન્તર્ભાવ <linux/sched/sysctl.h>
#અન્તર્ભાવ <linux/sched/task_flags.h>
#અન્તર્ભાવ <linux/sched/task.h>
#અન્તર્ભાવ <linux/sched/topology.h>
#અન્તર્ભાવ <linux/atomic.h>
#અન્તર્ભાવ <linux/bitmap.h>
#અન્તર્ભાવ <linux/bug.h>
#અન્તર્ભાવ <linux/capability.h>
#અન્તર્ભાવ <linux/cgroup_api.h>
#અન્તર્ભાવ <linux/cgroup.h>
#અન્તર્ભાવ <linux/context_tracking.h>
#અન્તર્ભાવ <linux/cpufreq.h>
#અન્તર્ભાવ <linux/cpumask_api.h>
#અન્તર્ભાવ <linux/cpuset.h>
#અન્તર્ભાવ <linux/ctype.h>
#અન્તર્ભાવ <linux/file.h>
#અન્તર્ભાવ <linux/fs_api.h>
#અન્તર્ભાવ <linux/hrtimer_api.h>
#અન્તર્ભાવ <linux/interrupt.h>
#અન્તર્ભાવ <linux/irq_work.h>
#અન્તર્ભાવ <linux/jiffies.h>
#અન્તર્ભાવ <linux/kref_api.h>
#અન્તર્ભાવ <linux/kthread.h>
#અન્તર્ભાવ <linux/ktime_api.h>
#અન્તર્ભાવ <linux/lockdep_api.h>
#અન્તર્ભાવ <linux/lockdep.h>
#અન્તર્ભાવ <linux/memblock.h>
#અન્તર્ભાવ <linux/memcontrol.h>
#અન્તર્ભાવ <linux/minmax.h>
#અન્તર્ભાવ <linux/mm.h>
#અન્તર્ભાવ <linux/module.h>
#અન્તર્ભાવ <linux/mutex_api.h>
#અન્તર્ભાવ <linux/plist.h>
#અન્તર્ભાવ <linux/poll.h>
#અન્તર્ભાવ <linux/proc_fs.h>
#અન્તર્ભાવ <linux/profile.h>
#અન્તર્ભાવ <linux/psi.h>
#અન્તર્ભાવ <linux/rcupdate.h>
#અન્તર્ભાવ <linux/seq_file.h>
#અન્તર્ભાવ <linux/seqlock.h>
#અન્તર્ભાવ <linux/softirq.h>
#અન્તર્ભાવ <linux/spinlock_api.h>
#અન્તર્ભાવ <linux/static_key.h>
#અન્તર્ભાવ <linux/stop_machine.h>
#અન્તર્ભાવ <linux/syscalls_api.h>
#અન્તર્ભાવ <linux/syscalls.h>
#અન્તર્ભાવ <linux/tick.h>
#અન્તર્ભાવ <linux/topology.h>
#અન્તર્ભાવ <linux/types.h>
#અન્તર્ભાવ <linux/u64_stats_sync_api.h>
#અન્તર્ભાવ <linux/uaccess.h>
#અન્તર્ભાવ <linux/vmstat.h>
#અન્તર્ભાવ <linux/wait_api.h>
#અન્તર્ભાવ <linux/wait_bit.h>
#અન્તર્ભાવ <linux/workqueue_api.h>
#અન્તર્ભાવ <linux/delayacct.h>
#અન્તર્ભાવ <linux/mmu_context.h>

#અન્તર્ભાવ <trace/events/power.h>
#અન્તર્ભાવ <trace/events/sched.h>

#અન્તર્ભાવ "../workqueue_internal.h"

સંરચના rq;
સંરચના cfs_rq;
સંરચના rt_rq;
સંરચના sched_group;
સંરચના cpuidle_state;

#જો defined(CONFIG_PARAVIRT) && !defined(CONFIG_HAVE_PV_STEAL_CLOCK_GEN)
# include <asm/paravirt.h>
#endif

#અન્તર્ભાવ <asm/barrier.h>

#અન્તર્ભાવ "cpupri.h"
#અન્તર્ભાવ "cpudeadline.h"

/* task_struct::on_rq states: */
#define TASK_ON_RQ_QUEUED	1
#define TASK_ON_RQ_MIGRATING	2

બાહ્ય __read_mostly પૂર્ણાંક scheduler_running;

બાહ્ય unsigned long calc_load_update;
બાહ્ય atomic_long_t calc_load_tasks;

બાહ્ય શૂન્ય calc_global_load_tick(સંરચના rq *this_rq);
બાહ્ય long calc_load_fold_active(સંરચના rq *this_rq, long adjust);

બાહ્ય શૂન્ય call_trace_sched_update_nr_running(સંરચના rq *rq, પૂર્ણાંક ગણતરી);

બાહ્ય પૂર્ણાંક sysctl_sched_rt_period;
બાહ્ય પૂર્ણાંક sysctl_sched_rt_runtime;
બાહ્ય પૂર્ણાંક sched_rr_timeslice;

/*
 * Asymmetric CPU capacity bits
 */
સંરચના asym_cap_data {
	સંરચના list_head link;
	સંરચના rcu_head rcu;
	unsigned long capacity;
	unsigned long cpus[];
};

બાહ્ય સંરચના list_head asym_cap_list;

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
	જો (__w)						\
		__w = max(2UL, __w >> SCHED_FIXEDPOINT_SHIFT);	\
	__w;							\
})
#નહીંતર
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

સ્થિર અંતઃરેખ પૂર્ણાંક idle_policy(પૂર્ણાંક policy)
{
	પાછા policy == SCHED_IDLE;
}

સ્થિર અંતઃરેખ પૂર્ણાંક normal_policy(પૂર્ણાંક policy)
{
#ifdef CONFIG_SCHED_CLASS_EXT
	જો (policy == SCHED_EXT)
		પાછા true;
#endif
	પાછા policy == SCHED_NORMAL;
}

સ્થિર અંતઃરેખ પૂર્ણાંક fair_policy(પૂર્ણાંક policy)
{
	પાછા normal_policy(policy) || policy == SCHED_BATCH;
}

સ્થિર અંતઃરેખ પૂર્ણાંક rt_policy(પૂર્ણાંક policy)
{
	પાછા policy == SCHED_FIFO || policy == SCHED_RR;
}

સ્થિર અંતઃરેખ પૂર્ણાંક dl_policy(પૂર્ણાંક policy)
{
	પાછા policy == SCHED_DEADLINE;
}

સ્થિર અંતઃરેખ bool valid_policy(પૂર્ણાંક policy)
{
	પાછા idle_policy(policy) || fair_policy(policy) ||
		rt_policy(policy) || dl_policy(policy);
}

સ્થિર અંતઃરેખ પૂર્ણાંક task_has_idle_policy(સંરચના task_struct *p)
{
	પાછા idle_policy(p->policy);
}

સ્થિર અંતઃરેખ પૂર્ણાંક task_has_rt_policy(સંરચના task_struct *p)
{
	પાછા rt_policy(p->policy);
}

સ્થિર અંતઃરેખ પૂર્ણાંક task_has_dl_policy(સંરચના task_struct *p)
{
	પાછા dl_policy(p->policy);
}

#define cap_scale(v, s)		((v)*(s) >> SCHED_CAPACITY_SHIFT)

સ્થિર અંતઃરેખ શૂન્ય update_avg(u64 *avg, u64 sample)
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
સ્થિર અંતઃરેખ unsigned long sched_weight_from_cgroup(unsigned long cgrp_weight)
{
	પાછા DIV_ROUND_CLOSEST_ULL(cgrp_weight * 1024, CGROUP_WEIGHT_DFL);
}

સ્થિર અંતઃરેખ unsigned long sched_weight_to_cgroup(unsigned long weight)
{
	પાછા clamp_t(unsigned long,
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

સ્થિર અંતઃરેખ bool dl_entity_is_special(અચલ સંરચના sched_dl_entity *dl_se)
{
#ifdef CONFIG_CPU_FREQ_GOV_SCHEDUTIL
	પાછા unlikely(dl_se->flags & SCHED_FLAG_SUGOV);
#નહીંતર
	પાછા false;
#endif
}

/*
 * Tells if entity @a should preempt entity @b.
 */
સ્થિર અંતઃરેખ bool dl_entity_preempt(અચલ સંરચના sched_dl_entity *a,
				     અચલ સંરચના sched_dl_entity *b)
{
	પાછા dl_entity_is_special(a) ||
	       dl_time_before(a->deadline, b->deadline);
}

/*
 * This is the priority-queue data structure of the RT scheduling class:
 */
સંરચના rt_prio_array {
	DECLARE_BITMAP(bitmap, MAX_RT_PRIO+1); /* include 1 bit for delimiter */
	સંરચના list_head queue[MAX_RT_PRIO];
};

સંરચના rt_bandwidth {
	/* nests inside the rq lock: */
	raw_spinlock_t		rt_runtime_lock;
	ktime_t			rt_period;
	u64			rt_runtime;
	સંરચના hrtimer		rt_period_timer;
	unsigned પૂર્ણાંક		rt_period_active;
};

સ્થિર અંતઃરેખ પૂર્ણાંક dl_bandwidth_enabled(શૂન્ય)
{
	પાછા sysctl_sched_rt_runtime >= 0;
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
સંરચના dl_bw {
	raw_spinlock_t		lock;
	u64			bw;
	u64			total_bw;
};

બાહ્ય શૂન્ય init_dl_bw(સંરચના dl_bw *dl_b);
બાહ્ય પૂર્ણાંક  sched_dl_global_validate(શૂન્ય);
બાહ્ય શૂન્ય sched_dl_do_global(શૂન્ય);
બાહ્ય પૂર્ણાંક  sched_dl_overflow(સંરચના task_struct *p, પૂર્ણાંક policy, અચલ સંરચના sched_attr *attr);
બાહ્ય શૂન્ય __setparam_dl(સંરચના task_struct *p, અચલ સંરચના sched_attr *attr);
બાહ્ય શૂન્ય __getparam_dl(સંરચના task_struct *p, સંરચના sched_attr *attr, unsigned પૂર્ણાંક flags);
બાહ્ય bool __checkparam_dl(અચલ સંરચના sched_attr *attr);
બાહ્ય bool dl_param_changed(સંરચના task_struct *p, અચલ સંરચના sched_attr *attr);
બાહ્ય પૂર્ણાંક  dl_cpuset_cpumask_can_shrink(અચલ સંરચના cpumask *cur, અચલ સંરચના cpumask *trial);
બાહ્ય પૂર્ણાંક  dl_bw_deactivate(પૂર્ણાંક cpu);
બાહ્ય s64 dl_scaled_delta_exec(સંરચના rq *rq, સંરચના sched_dl_entity *dl_se, s64 delta_exec);
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
બાહ્ય શૂન્ય dl_server_update_idle(સંરચના sched_dl_entity *dl_se, s64 delta_exec);
બાહ્ય શૂન્ય dl_server_update(સંરચના sched_dl_entity *dl_se, s64 delta_exec);
બાહ્ય શૂન્ય dl_server_start(સંરચના sched_dl_entity *dl_se);
બાહ્ય શૂન્ય dl_server_stop(સંરચના sched_dl_entity *dl_se);
બાહ્ય શૂન્ય dl_server_init(સંરચના sched_dl_entity *dl_se, સંરચના rq *rq,
		    dl_server_pick_f pick_task);
બાહ્ય શૂન્ય sched_init_dl_servers(શૂન્ય);

બાહ્ય શૂન્ય fair_server_init(સંરચના rq *rq);
બાહ્ય શૂન્ય ext_server_init(સંરચના rq *rq);
બાહ્ય શૂન્ય __dl_server_attach_root(સંરચના sched_dl_entity *dl_se, સંરચના rq *rq);
બાહ્ય પૂર્ણાંક dl_server_apply_params(સંરચના sched_dl_entity *dl_se,
		    u64 runtime, u64 period, bool init);
બાહ્ય પૂર્ણાંક dl_server_attach_bw(સંરચના sched_dl_entity *dl_se);
બાહ્ય શૂન્ય dl_server_detach_bw(સંરચના sched_dl_entity *dl_se);
બાહ્ય પૂર્ણાંક dl_server_swap_bw(સંરચના sched_dl_entity *detach_se,
			     સંરચના sched_dl_entity *attach_se);

સ્થિર અંતઃરેખ bool dl_server_active(સંરચના sched_dl_entity *dl_se)
{
	પાછા dl_se->dl_server_active;
}

#ifdef CONFIG_CGROUP_SCHED

બાહ્ય સંરચના list_head task_groups;

#ifdef CONFIG_GROUP_SCHED_BANDWIDTH
બાહ્ય અચલ u64 max_bw_quota_period_us;

/*
 * default period for group bandwidth.
 * default: 0.1s, units: microseconds
 */
સ્થિર અંતઃરેખ u64 default_bw_period_us(શૂન્ય)
{
	પાછા 100000ULL;
}
#endif /* CONFIG_GROUP_SCHED_BANDWIDTH */

સંરચના cfs_bandwidth {
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
	સંરચના hrtimer		period_timer;
	સંરચના hrtimer		slack_timer;
	સંરચના list_head	throttled_cfs_rq;

	/* Statistics: */
	પૂર્ણાંક			nr_periods;
	પૂર્ણાંક			nr_throttled;
	પૂર્ણાંક			nr_burst;
	u64			throttled_time;
	u64			burst_time;
#endif /* CONFIG_CFS_BANDWIDTH */
};

/* Task group related information */
સંરચના task_group {
	સંરચના cgroup_subsys_state css;

#ifdef CONFIG_GROUP_SCHED_WEIGHT
	/* A positive value indicates that this is a SCHED_IDLE group. */
	પૂર્ણાંક			idle;
#endif

#ifdef CONFIG_FAIR_GROUP_SCHED
	/* runqueue "owned" by this group on each CPU */
	સંરચના cfs_rq __percpu	*cfs_rq;
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
	સંરચના sched_rt_entity	**rt_se;
	સંરચના rt_rq		**rt_rq;

	સંરચના rt_bandwidth	rt_bandwidth;
#endif

	સંરચના scx_task_group	scx;

	સંરચના rcu_head		rcu;
	સંરચના list_head	list;

	સંરચના task_group	*parent;
	સંરચના list_head	siblings;
	સંરચના list_head	children;

#ifdef CONFIG_SCHED_AUTOGROUP
	સંરચના autogroup	*autogroup;
#endif

	સંરચના cfs_bandwidth	cfs_bandwidth;

#ifdef CONFIG_UCLAMP_TASK_GROUP
	/* The two decimal precision [%] value requested from user-space */
	unsigned પૂર્ણાંક		uclamp_pct[UCLAMP_CNT];
	/* Clamp values requested for a task group */
	સંરચના uclamp_se	uclamp_req[UCLAMP_CNT];
	/* Effective clamp values used for a task group */
	સંરચના uclamp_se	uclamp[UCLAMP_CNT];
#endif

};

#define મૂળ_TASK_GROUP_LOAD	NICE_0_LOAD

પ્રકારવ્યાખ્યા પૂર્ણાંક (*tg_visitor)(સંરચના task_group *, શૂન્ય *);

બાહ્ય પૂર્ણાંક walk_tg_વૃક્ષ_from(સંરચના task_group *from,
			     tg_visitor down, tg_visitor up, શૂન્ય *data);

/*
 * Iterate the full tree, calling @down when first entering a node and @up when
 * leaving it for the final time.
 *
 * Caller must hold rcu_lock or sufficient equivalent.
 */
સ્થિર અંતઃરેખ પૂર્ણાંક walk_tg_વૃક્ષ(tg_visitor down, tg_visitor up, શૂન્ય *data)
{
	પાછા walk_tg_વૃક્ષ_from(&મૂળ_task_group, down, up, data);
}

સ્થિર અંતઃરેખ સંરચના task_group *css_tg(સંરચના cgroup_subsys_state *css)
{
	પાછા css ? container_of(css, સંરચના task_group, css) : ખાલી;
}

બાહ્ય પૂર્ણાંક tg_nop(સંરચના task_group *tg, શૂન્ય *data);

#ifdef CONFIG_FAIR_GROUP_SCHED
બાહ્ય શૂન્ય free_fair_sched_group(સંરચના task_group *tg);
બાહ્ય પૂર્ણાંક alloc_fair_sched_group(સંરચના task_group *tg, સંરચના task_group *parent);
બાહ્ય શૂન્ય online_fair_sched_group(સંરચના task_group *tg);
બાહ્ય શૂન્ય unregister_fair_sched_group(સંરચના task_group *tg);
બાહ્ય શૂન્ય __sched_cgroup_mode_update(પૂર્ણાંક mode);
#નહીંતર /* !CONFIG_FAIR_GROUP_SCHED: */
સ્થિર અંતઃરેખ શૂન્ય free_fair_sched_group(સંરચના task_group *tg) { }
સ્થિર અંતઃરેખ પૂર્ણાંક alloc_fair_sched_group(સંરચના task_group *tg, સંરચના task_group *parent)
{
       પાછા 1;
}
સ્થિર અંતઃરેખ શૂન્ય online_fair_sched_group(સંરચના task_group *tg) { }
સ્થિર અંતઃરેખ શૂન્ય unregister_fair_sched_group(સંરચના task_group *tg) { }
#endif /* !CONFIG_FAIR_GROUP_SCHED */

બાહ્ય શૂન્ય init_tg_cfs_entry(સંરચના task_group *tg, સંરચના cfs_rq *cfs_rq,
			સંરચના sched_entity *se, પૂર્ણાંક cpu,
			સંરચના sched_entity *parent);
બાહ્ય શૂન્ય init_cfs_bandwidth(સંરચના cfs_bandwidth *cfs_b, સંરચના cfs_bandwidth *parent);

બાહ્ય શૂન્ય __refill_cfs_bandwidth_runtime(સંરચના cfs_bandwidth *cfs_b);
બાહ્ય શૂન્ય start_cfs_bandwidth(સંરચના cfs_bandwidth *cfs_b);
બાહ્ય શૂન્ય unthrottle_cfs_rq(સંરચના cfs_rq *cfs_rq);
બાહ્ય bool cfs_task_bw_constrained(સંરચના task_struct *p);

બાહ્ય શૂન્ય init_tg_rt_entry(સંરચના task_group *tg, સંરચના rt_rq *rt_rq,
		સંરચના sched_rt_entity *rt_se, પૂર્ણાંક cpu,
		સંરચના sched_rt_entity *parent);
બાહ્ય પૂર્ણાંક sched_group_set_rt_runtime(સંરચના task_group *tg, long rt_runtime_us);
બાહ્ય પૂર્ણાંક sched_group_set_rt_period(સંરચના task_group *tg, u64 rt_period_us);
બાહ્ય long sched_group_rt_runtime(સંરચના task_group *tg);
બાહ્ય long sched_group_rt_period(સંરચના task_group *tg);
બાહ્ય પૂર્ણાંક sched_rt_can_attach(સંરચના task_group *tg, સંરચના task_struct *tsk);

બાહ્ય સંરચના task_group *sched_create_group(સંરચના task_group *parent);
બાહ્ય શૂન્ય sched_online_group(સંરચના task_group *tg,
			       સંરચના task_group *parent);
બાહ્ય શૂન્ય sched_destroy_group(સંરચના task_group *tg);
બાહ્ય શૂન્ય sched_release_group(સંરચના task_group *tg);

બાહ્ય શૂન્ય sched_move_task(સંરચના task_struct *tsk, bool for_autogroup);

#ifdef CONFIG_FAIR_GROUP_SCHED
બાહ્ય પૂર્ણાંક sched_group_set_shares(સંરચના task_group *tg, unsigned long shares);

બાહ્ય પૂર્ણાંક sched_group_set_idle(સંરચના task_group *tg, long idle);

બાહ્ય શૂન્ય set_task_rq_fair(સંરચના sched_entity *se,
			     સંરચના cfs_rq *prev, સંરચના cfs_rq *next);
#નહીંતર /* !CONFIG_FAIR_GROUP_SCHED: */
સ્થિર અંતઃરેખ પૂર્ણાંક sched_group_set_shares(સંરચના task_group *tg, unsigned long shares) { પાછા 0; }
સ્થિર અંતઃરેખ પૂર્ણાંક sched_group_set_idle(સંરચના task_group *tg, long idle) { પાછા 0; }
#endif /* !CONFIG_FAIR_GROUP_SCHED */

#નહીંતર /* !CONFIG_CGROUP_SCHED: */

સંરચના cfs_bandwidth { };

સ્થિર અંતઃરેખ bool cfs_task_bw_constrained(સંરચના task_struct *p) { પાછા false; }

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

બાહ્ય શૂન્ય unregister_rt_sched_group(સંરચના task_group *tg);
બાહ્ય શૂન્ય free_rt_sched_group(સંરચના task_group *tg);
બાહ્ય પૂર્ણાંક alloc_rt_sched_group(સંરચના task_group *tg, સંરચના task_group *parent);

/*
 * u64_u32_load/u64_u32_store
 *
 * Use a copy of a u64 value to protect against data race. This is only
 * applicable for 32-bits architectures.
 */
#ifdef CONFIG_64BIT
# define u64_u32_load_copy(var, copy)		var
# define u64_u32_store_copy(var, copy, val)	(var = val)
#નહીંતર
# define u64_u32_load_copy(var, copy)					\
({									\
	u64 __val, __val_copy;						\
	કરો {								\
		__val_copy = copy;					\
		/*							\
		 * paired with u64_u32_store_copy(), ordering access	\
		 * to var and copy.					\
		 */							\
		smp_rmb();						\
		__val = var;						\
	} જ્યારે (__val != __val_copy);					\
	__val;								\
})
# define u64_u32_store_copy(var, copy, val)				\
કરો {									\
	typeof(val) __val = (val);					\
	var = __val;							\
	/*								\
	 * paired with u64_u32_load_copy(), ordering access to var and	\
	 * copy.							\
	 */								\
	smp_wmb();							\
	copy = __val;							\
} જ્યારે (0)
#endif
# define u64_u32_load(var)		u64_u32_load_copy(var, var##_copy)
# define u64_u32_store(var, val)	u64_u32_store_copy(var, var##_copy, val)

સંરચના balance_callback {
	સંરચના balance_callback *next;
	શૂન્ય (*func)(સંરચના rq *rq);
};

/* Fair scheduling SCHED_{NORMAL,BATCH,IDLE} related fields in a runqueue: */
સંરચના cfs_rq {
	સંરચના load_weight	load;
	unsigned પૂર્ણાંક		nr_queued;
	unsigned પૂર્ણાંક		h_nr_queued;		/* SCHED_{NORMAL,BATCH,IDLE} */
	unsigned પૂર્ણાંક		h_nr_runnable;		/* SCHED_{NORMAL,BATCH,IDLE} */
	unsigned પૂર્ણાંક		h_nr_idle;		/* SCHED_IDLE */

	s64			sum_w_vruntime;
	u64			sum_weight;
	u64			zero_vruntime;
	unsigned પૂર્ણાંક		sum_shift;

#ifdef CONFIG_SCHED_CORE
	unsigned પૂર્ણાંક		forceidle_seq;
	u64			zero_vruntime_fi;
#endif

	સંરચના rb_મૂળ_cached	tasks_timeline;

	/*
	 * 'curr' points to the currently running entity on this cfs_rq.
	 * It is set to NULL otherwise (i.e when none are currently running).
	 */
	સંરચના sched_entity	*curr;
	સંરચના sched_entity	*next;

	/*
	 * CFS load tracking
	 */
	સંરચના sched_entity	*h_curr;
	સંરચના sched_avg	avg;
#ifndef CONFIG_64BIT
	u64			last_update_time_copy;
#endif
	સંરચના {
		raw_spinlock_t	lock ____cacheline_aligned;
		પૂર્ણાંક		nr;
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
	સંરચના sched_entity	*h_load_next;

	સંરચના rq		*rq;	/* CPU runqueue to which this cfs_rq is attached */

	/*
	 * leaf cfs_rqs are those that hold tasks (lowest schedulable entity in
	 * a hierarchy). Non-leaf lrqs hold other higher schedulable entities
	 * (like users, containers etc.)
	 *
	 * leaf_cfs_rq_list ties together list of leaf cfs_rq's in a CPU.
	 * This list is used during load balance.
	 */
	પૂર્ણાંક			on_list;
	સંરચના list_head	leaf_cfs_rq_list;
	સંરચના task_group	*tg;	/* Group that "owns" this runqueue */

	/* Locally cached copy of our task_group's idle value */
	પૂર્ણાંક			idle;

# ifdef CONFIG_CFS_BANDWIDTH
	પૂર્ણાંક			runtime_enabled;
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
	પૂર્ણાંક			throttle_ગણતરી;
	સંરચના list_head	throttled_list;
	સંરચના list_head	throttled_csd_list;
	સંરચના list_head        throttled_limbo_list;
# endif /* CONFIG_CFS_BANDWIDTH */
#endif /* CONFIG_FAIR_GROUP_SCHED */
};

#ifdef CONFIG_SCHED_CLASS_EXT
/* scx_rq->flags, protected by the rq lock */
ગણના scx_rq_flags {
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
	SCX_RQ_મૂળ_IDLE_RENOTIFY	= 1 << 8, /* the root is owed update_idle() */

	SCX_RQ_IN_WAKEUP	= 1 << 16,
	SCX_RQ_IN_DISPATCH	= 1 << 17,
};

/* per-rq rescue execution state, see scx_rescue_timerfn() */
સંરચના scx_rq_rescue {
	સંરચના scx_dispatch_q	dsq;			/* stranded tasks awaiting rescue */
	s64			budget;			/* execution token bucket, ns */
	u64			clock;			/* last budget accrual timestamp */
	સંરચના task_struct	*curr;			/* task being rescued, one at a time */
	s64			slice;			/* curr's admitted slice */
	u64			exec_snap;		/* sum_exec_runtime at admission */
	સંરચના timer_list	timer;			/* paces admission and escalation */
	u64			kill_at;		/* last ejection, init before any */
};

સંરચના scx_rq {
	સંરચના scx_dispatch_q	local_dsq;
#ifdef CONFIG_EXT_SUB_SCHED
	સંરચના scx_dispatch_q	reject_dsq;		/* staging for cap-rejected tasks */
	સંરચના scx_rq_rescue	rescue;
#endif
	સંરચના list_head	runnable_list;		/* runnable tasks on this rq */
	સંરચના list_head	ddsp_deferred_locals;	/* deferred ddsps from enq */
	/* both stashed across the activate_task() in move_remote_task_to_local_dsq() */
	u64			remote_activate_enq_flags;
	સંરચના scx_sched	*remote_activate_sch;
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
	સંરચના llist_head	ecaps_to_sync;		/* pending ecaps syncs */
	સંરચના task_struct	*sub_dispatch_prev;
#endif
	cpumask_var_t		cpus_to_sync;
	bool			kick_sync_pending;
	unsigned long		kick_sync;

	સંરચના list_head	sched_pcpus_to_kick;	/* see kick_cpus_irq_workfn() */

	raw_spinlock_t		deferred_reenq_lock;
	સંરચના list_head	deferred_reenq_locals;	/* scheds requesting reenq of local DSQ */
	સંરચના list_head	deferred_reenq_users;	/* user DSQs requesting reenq */
	સંરચના balance_callback	deferred_bal_cb;
	સંરચના balance_callback	kick_sync_bal_cb;
	સંરચના irq_work		deferred_irq_work;
	સંરચના irq_work		kick_cpus_irq_work;
};
#endif /* CONFIG_SCHED_CLASS_EXT */

સ્થિર અંતઃરેખ પૂર્ણાંક rt_bandwidth_enabled(શૂન્ય)
{
	પાછા sysctl_sched_rt_runtime >= 0;
}

/* RT IPI pull logic requires IRQ_WORK */
#જો defined(CONFIG_IRQ_WORK) && defined(CONFIG_SMP)
# define HAVE_RT_ધકેલો_IPI
#endif

/* Real-Time classes' related field in a runqueue: */
સંરચના rt_rq {
	સંરચના rt_prio_array	active;
	unsigned પૂર્ણાંક		rt_nr_running;
	unsigned પૂર્ણાંક		rr_nr_running;
	સંરચના {
		પૂર્ણાંક		curr; /* highest queued rt task prio */
		પૂર્ણાંક		next; /* next highest */
	} highest_prio;
	bool			overloaded;
	સંરચના plist_head	pushable_tasks;

	પૂર્ણાંક			rt_queued;

#ifdef CONFIG_RT_GROUP_SCHED
	પૂર્ણાંક			rt_throttled;
	u64			rt_time; /* consumed RT time, goes up in update_curr_rt */
	u64			rt_runtime; /* allotted RT time, "slice" from rt_bandwidth, RT sharing/balancing */
	/* Nests inside the rq lock: */
	raw_spinlock_t		rt_runtime_lock;

	unsigned પૂર્ણાંક		rt_nr_boosted;

	સંરચના rq		*rq; /* this is always top-level rq, cache? */
#endif
#ifdef CONFIG_CGROUP_SCHED
	સંરચના task_group	*tg; /* this tg has "this" rt_rq on given CPU for runnable entities */
#endif
};

સ્થિર અંતઃરેખ bool rt_rq_is_runnable(સંરચના rt_rq *rt_rq)
{
	પાછા rt_rq->rt_queued && rt_rq->rt_nr_running;
}

/* Deadline class' related fields in a runqueue */
સંરચના dl_rq {
	/* runqueue is an rbtree, ordered by deadline */
	સંરચના rb_મૂળ_cached	મૂળ;

	unsigned પૂર્ણાંક		dl_nr_running;

	/*
	 * Deadline values of the currently executing and the
	 * earliest ready task on this rq. Caching these facilitates
	 * the decision whether or not a ready but not running task
	 * should migrate somewhere else.
	 */
	સંરચના {
		u64		curr;
		u64		next;
	} earliest_dl;

	bool			overloaded;

	સંરચના sched_dl_entity	*curr;
	/*
	 * Tasks on this rq that can be pushed away. They are kept in
	 * an rb-tree, ordered by tasks' deadlines, with caching
	 * of the leftmost (earliest deadline) element.
	 */
	સંરચના rb_મૂળ_cached	pushable_dl_tasks_મૂળ;

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
#define is_મૂળ_task_group(tg) ((tg) == &મૂળ_task_group)
/* An entity is a task if it doesn't "own" a runqueue */
#define entity_is_task(se)	(!se->my_q)

સ્થિર અંતઃરેખ શૂન્ય se_update_runnable(સંરચના sched_entity *se)
{
	જો (!entity_is_task(se))
		se->runnable_weight = se->my_q->h_nr_runnable;
}

સ્થિર અંતઃરેખ long se_runnable(સંરચના sched_entity *se)
{
	જો (se->sched_delayed)
		પાછા false;

	જો (entity_is_task(se))
		પાછા !!se->on_rq;
	નહીંતર
		પાછા se->runnable_weight;
}

#નહીંતર /* !CONFIG_FAIR_GROUP_SCHED: */

#define entity_is_task(se)	1

સ્થિર અંતઃરેખ શૂન્ય se_update_runnable(સંરચના sched_entity *se) { }

સ્થિર અંતઃરેખ long se_runnable(સંરચના sched_entity *se)
{
	જો (se->sched_delayed)
		પાછા false;

	પાછા !!se->on_rq;
}

#endif /* !CONFIG_FAIR_GROUP_SCHED */

/*
 * XXX we want to get rid of these helpers and use the full load resolution.
 */
સ્થિર અંતઃરેખ long se_weight(સંરચના sched_entity *se)
{
	પાછા scale_load_down(se->load.weight);
}


સ્થિર અંતઃરેખ bool sched_asym_prefer(પૂર્ણાંક a, પૂર્ણાંક b)
{
	પાછા arch_asym_cpu_priority(a) > arch_asym_cpu_priority(b);
}

સંરચના perf_domain {
	સંરચના em_perf_domain *em_pd;
	સંરચના perf_domain *next;
	સંરચના rcu_head rcu;
};

/*
 * We add the notion of a root-domain which will be used to define per-domain
 * variables. Each exclusive cpuset essentially defines an island domain by
 * fully partitioning the member CPUs from any other cpuset. Whenever a new
 * exclusive cpuset is created, we also create and attach a new root-domain
 * object.
 *
 */
સંરચના મૂળ_domain {
	atomic_t		refcount;
	atomic_t		rto_ગણતરી;
	સંરચના rcu_head		rcu;
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
	atomic_t		dlo_ગણતરી;
	સંરચના dl_bw		dl_bw;
	સંરચના cpudl		cpudl;

	/*
	 * Indicate whether a root_domain's dl_bw has been checked or
	 * updated. It's monotonously increasing value.
	 *
	 * Also, some corner cases, like 'wrap around' is dangerous, but given
	 * that u64 is 'big enough'. So that shouldn't be a concern.
	 */
	u64 visit_cookie;

#ifdef HAVE_RT_ધકેલો_IPI
	/*
	 * For IPI pull requests, loop across the rto_mask.
	 */
	સંરચના irq_work		rto_ધકેલો_work;
	raw_spinlock_t		rto_lock;
	/* These are only updated and read within rto_lock */
	પૂર્ણાંક			rto_loop;
	પૂર્ણાંક			rto_cpu;
	/* These atomics are updated outside of a lock */
	atomic_t		rto_loop_next;
	atomic_t		rto_loop_start;
#endif /* HAVE_RT_PUSH_IPI */
	/*
	 * The "RT overload" flag: it gets set if a CPU has more than
	 * one runnable RT task.
	 */
	cpumask_var_t		rto_mask;
	સંરચના cpupri		cpupri;

	/*
	 * NULL-terminated list of performance domains intersecting with the
	 * CPUs of the rd. Protected by RCU.
	 */
	સંરચના perf_domain __rcu *pd;
};

બાહ્ય શૂન્ય init_defrootdomain(શૂન્ય);
બાહ્ય પૂર્ણાંક sched_init_domains(અચલ સંરચના cpumask *cpu_map);
બાહ્ય શૂન્ય rq_attach_મૂળ(સંરચના rq *rq, સંરચના મૂળ_domain *rd);
બાહ્ય શૂન્ય sched_get_rd(સંરચના મૂળ_domain *rd);
બાહ્ય શૂન્ય sched_put_rd(સંરચના મૂળ_domain *rd);

સ્થિર અંતઃરેખ પૂર્ણાંક get_rd_overloaded(સંરચના મૂળ_domain *rd)
{
	પાછા READ_ONCE(rd->overloaded);
}

સ્થિર અંતઃરેખ શૂન્ય set_rd_overloaded(સંરચના મૂળ_domain *rd, પૂર્ણાંક status)
{
	જો (get_rd_overloaded(rd) != status)
		WRITE_ONCE(rd->overloaded, status);
}

#ifdef HAVE_RT_ધકેલો_IPI
બાહ્ય શૂન્ય rto_ધકેલો_irq_work_func(સંરચના irq_work *work);
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
સંરચના uclamp_bucket {
	unsigned long કિંમત : bits_per(SCHED_CAPACITY_SCALE);
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
સંરચના uclamp_rq {
	unsigned પૂર્ણાંક કિંમત;
	સંરચના uclamp_bucket bucket[UCLAMP_BUCKETS];
};

DECLARE_STATIC_ચાવી_FALSE(sched_uclamp_used);
#endif /* CONFIG_UCLAMP_TASK */

/*
 * This is the main, per-CPU runqueue data structure.
 *
 * Locking rule: those places that want to lock multiple runqueues
 * (such as the load balancing or the thread migration code), lock
 * acquire operations must be ordered by ascending &runqueue.
 */
સંરચના rq {
	/*
	 * The following members are loaded together, without holding the
	 * rq->lock, in an extremely hot loop in update_sg_lb_stats()
	 * (called from pick_next_task()). To reduce cache pollution from
	 * this operation, they are placed together on this dedicated cache
	 * line. Even though some of them are frequently modified, they are
	 * loaded much more frequently than they are stored.
	 */
	unsigned પૂર્ણાંક		nr_running;
#ifdef CONFIG_NUMA_BALANCING
	unsigned પૂર્ણાંક		nr_numa_running;
	unsigned પૂર્ણાંક		nr_preferred_running;
#endif
	unsigned પૂર્ણાંક		ttwu_pending;
	unsigned long		cpu_capacity;
#ifdef CONFIG_SCHED_PROXY_EXEC
	સંરચના task_struct __rcu	*donor;  /* Scheduling context */
	સંરચના task_struct __rcu	*curr;   /* Execution context */
#નહીંતર
	સંઘ {
		સંરચના task_struct __rcu *donor; /* Scheduler context */
		સંરચના task_struct __rcu *curr;  /* Execution context */
	};
#endif
	સંરચના task_struct	*idle;
	/* padding left here deliberately */

	/*
	 * The next cacheline holds the (hot) runqueue lock, as well as
	 * some other less performance-critical fields.
	 */
	u64			nr_switches	____cacheline_aligned;

	/* runqueue lock: */
	raw_spinlock_t		__lock;

#ifdef CONFIG_NO_HZ_COMMON
	unsigned પૂર્ણાંક		nohz_tick_stopped;
	atomic_t		nohz_flags;
	unsigned પૂર્ણાંક		has_blocked_load;
	unsigned long		last_blocked_load_update_tick;
	call_single_data_t	nohz_csd;
#endif /* CONFIG_NO_HZ_COMMON */

#ifdef CONFIG_UCLAMP_TASK
	/* Utilization clamp values based on CPU's RUNNABLE tasks */
	સંરચના uclamp_rq	uclamp[UCLAMP_CNT] ____cacheline_aligned;
	unsigned પૂર્ણાંક		uclamp_flags;
#define UCLAMP_FLAG_IDLE 0x01
#endif

	સંરચના cfs_rq		cfs;
	સંરચના rt_rq		rt;
	સંરચના dl_rq		dl;
#ifdef CONFIG_SCHED_CLASS_EXT
	સંરચના scx_rq		scx;
	સંરચના sched_dl_entity	ext_server;
#endif
#ifdef CONFIG_SCHED_CACHE
	raw_spinlock_t		cpu_epoch_lock ____cacheline_aligned;
	u64			cpu_runtime;
	unsigned long		cpu_epoch;
	unsigned long		cpu_epoch_next;
#endif

	સંરચના sched_dl_entity	fair_server;

#ifdef CONFIG_FAIR_GROUP_SCHED
	/* list of leaf cfs_rq on this CPU: */
	સંરચના list_head	leaf_cfs_rq_list;
	સંરચના list_head	*tmp_alone_branch;
#endif /* CONFIG_FAIR_GROUP_SCHED */

#ifdef CONFIG_NUMA_BALANCING
	unsigned પૂર્ણાંક		numa_migrate_on;
#endif

#ifdef CONFIG_SCHED_CACHE
	unsigned પૂર્ણાંક		nr_pref_llc_running;
	unsigned પૂર્ણાંક		nr_llc_running;
#endif

	/*
	 * This is part of a global counter where only the total sum
	 * over all CPUs matters. A task can increase this counter on
	 * one CPU and if it got migrated afterwards it may decrease
	 * it on another CPU. Always updated under the runqueue lock:
	 */
	unsigned long		nr_uninterruptible;

	સંરચના sched_dl_entity	*dl_server;
	સંરચના task_struct	*stop;
	અચલ સંરચના sched_class *next_class;
	unsigned long		next_balance;
	સંરચના mm_struct	*prev_mm;

	/*
	 * The following fields of clock data are frequently referenced
	 * and updated together, and should go on their own cache line.
	 */
	u64			clock_task ____cacheline_aligned;
	u64			clock_pelt;
	u64			clock;
	unsigned long		lost_idle_time;
	unsigned પૂર્ણાંક		clock_update_flags;
	u64			clock_pelt_idle;
	u64			clock_idle;

#ifndef CONFIG_64BIT
	u64			clock_pelt_idle_copy;
	u64			clock_idle_copy;
#endif

	u64 last_seen_need_resched_ns;
	પૂર્ણાંક ticks_without_resched;

#ifdef CONFIG_MEMBARRIER
	પૂર્ણાંક membarrier_state;
#endif

	સંરચના મૂળ_domain		*rd;
	સંરચના sched_domain __rcu	*sd;

	સંરચના balance_callback *balance_callback;

	unsigned અક્ષર		nohz_idle_balance;
	unsigned અક્ષર		idle_balance;

	unsigned long		misfit_task_load;

	/* For active balancing */
	પૂર્ણાંક			active_balance;
	પૂર્ણાંક			ધકેલો_cpu;
	સંરચના cpu_stop_work	active_balance_work;

	/* CPU of this runqueue: */
	પૂર્ણાંક			cpu;
	પૂર્ણાંક			online;

	સંરચના list_head cfs_tasks;

	સંરચના sched_avg	avg_rt;
	સંરચના sched_avg	avg_dl;
#ifdef CONFIG_HAVE_SCHED_AVG_IRQ
	સંરચના sched_avg	avg_irq;
#endif
#ifdef CONFIG_SCHED_HW_PRESSURE
	સંરચના sched_avg	avg_hw;
#endif
	u64			idle_stamp;
	u64			avg_idle;

	/* This is used to determine avg_idle's max value */
	u64			max_idle_balance_cost;

#ifdef CONFIG_HOTPLUG_CPU
	સંરચના rcuwait		hotplug_wait;
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
	bool			npc_ધકેલો_work_pending;
#endif

	/* calc_load related fields */
	unsigned long		calc_load_update;
	long			calc_load_active;

#ifdef CONFIG_SCHED_HRTICK
	call_single_data_t	hrtick_csd;
	સંરચના hrtimer		hrtick_timer;
	ktime_t			hrtick_time;
	ktime_t			hrtick_delay;
	unsigned પૂર્ણાંક		hrtick_sched;
#endif

#ifdef CONFIG_SCHEDSTATS
	/* latency stats */
	સંરચના sched_info	rq_sched_info;
	unsigned long long	rq_cpu_time;

	/* sys_sched_yield() stats */
	unsigned પૂર્ણાંક		yld_ગણતરી;

	/* schedule() stats */
	unsigned પૂર્ણાંક		sched_ગણતરી;
	unsigned પૂર્ણાંક		sched_goidle;

	/* try_to_wake_up() stats */
	unsigned પૂર્ણાંક		ttwu_ગણતરી;
	unsigned પૂર્ણાંક		ttwu_local;
#endif

#ifdef CONFIG_CPU_IDLE
	/* Must be inspected within a RCU lock section */
	સંરચના cpuidle_state	*idle_state;
#endif

	unsigned પૂર્ણાંક		nr_pinned;
	unsigned પૂર્ણાંક		ધકેલો_busy;
	સંરચના cpu_stop_work	ધકેલો_work;

#ifdef CONFIG_SCHED_CORE
	/* per rq */
	સંરચના rq		*core;
	સંરચના task_struct	*core_pick;
	સંરચના sched_dl_entity	*core_dl_server;
	unsigned પૂર્ણાંક		core_enabled;
	સંરચના rb_મૂળ		core_વૃક્ષ;

	/* shared state -- careful with sched_core_cpu_deactivate() */
	unsigned પૂર્ણાંક		core_task_seq;
	unsigned પૂર્ણાંક		core_pick_seq;
	unsigned long		core_cookie;
	unsigned પૂર્ણાંક		core_forceidle_ગણતરી;
	unsigned પૂર્ણાંક		core_forceidle_seq;
	unsigned પૂર્ણાંક		core_forceidle_occupation;
	u64			core_forceidle_start;
	unsigned પૂર્ણાંક		core_pick_in_flight;
#endif /* CONFIG_SCHED_CORE */

	/* Scratch cpumask to be temporarily used under rq_lock */
	cpumask_var_t		scratch_mask;

#ifdef CONFIG_CFS_BANDWIDTH
	call_single_data_t	cfsb_csd;
	સંરચના list_head	cfsb_csd_list;
#endif

	atomic_t		nr_iowait;
} __no_randomize_layout;

#ifdef CONFIG_FAIR_GROUP_SCHED

/* CPU runqueue to which this cfs_rq is attached */
સ્થિર અંતઃરેખ સંરચના rq *rq_of(સંરચના cfs_rq *cfs_rq)
{
	પાછા cfs_rq->rq;
}

#નહીંતર /* !CONFIG_FAIR_GROUP_SCHED: */

સ્થિર અંતઃરેખ સંરચના rq *rq_of(સંરચના cfs_rq *cfs_rq)
{
	પાછા container_of(cfs_rq, સંરચના rq, cfs);
}
#endif /* !CONFIG_FAIR_GROUP_SCHED */

સ્થિર અંતઃરેખ પૂર્ણાંક cpu_of(સંરચના rq *rq)
{
	પાછા rq->cpu;
}

#define MDF_ધકેલો		0x01

સ્થિર અંતઃરેખ bool is_migration_disabled(સંરચના task_struct *p)
{
	પાછા p->migration_disabled;
}

DECLARE_PER_CPU_SHARED_ALIGNED(સંરચના rq, runqueues);
DECLARE_PER_CPU(સંરચના rnd_state, sched_rnd_state);

સ્થિર અંતઃરેખ u32 sched_rng(શૂન્ય)
{
	પાછા prandom_u32_state(this_cpu_ptr(&sched_rnd_state));
}

સ્થિર __always_inline સંરચના rq *__this_rq(શૂન્ય)
{
	પાછા this_cpu_ptr(&runqueues);
}

#define cpu_rq(cpu)		(&per_cpu(runqueues, (cpu)))
#define this_rq()		__this_rq()
#define task_rq(p)		cpu_rq(task_cpu(p))
#define cpu_curr(cpu)		(cpu_rq(cpu)->curr)
#define raw_rq()		raw_cpu_ptr(&runqueues)

સ્થિર અંતઃરેખ bool idle_rq(સંરચના rq *rq)
{
	પાછા rq->curr == rq->idle && !rq->nr_running && !rq->ttwu_pending;
}

/**
 * available_idle_cpu - is a given CPU idle for enqueuing work.
 * @cpu: the CPU in question.
 *
 * Return: 1 if the CPU is currently idle. 0 otherwise.
 */
સ્થિર અંતઃરેખ bool available_idle_cpu(પૂર્ણાંક cpu)
{
	જો (!idle_rq(cpu_rq(cpu)))
		પાછા 0;

	જો (vcpu_is_preempted(cpu))
		પાછા 0;

	પાછા 1;
}

#ifdef CONFIG_SCHED_PROXY_EXEC
સ્થિર અંતઃરેખ શૂન્ય rq_set_donor(સંરચના rq *rq, સંરચના task_struct *t)
{
	rcu_assign_pointer(rq->donor, t);
}
#નહીંતર
સ્થિર અંતઃરેખ શૂન્ય rq_set_donor(સંરચના rq *rq, સંરચના task_struct *t)
{
	/* Do nothing */
}
#endif

#ifdef CONFIG_SCHED_CORE
સ્થિર અંતઃરેખ સંરચના cpumask *sched_group_span(સંરચના sched_group *sg);

DECLARE_STATIC_ચાવી_FALSE(__sched_core_enabled);

સ્થિર અંતઃરેખ bool sched_core_enabled(સંરચના rq *rq)
{
	પાછા static_branch_unlikely(&__sched_core_enabled) && rq->core_enabled;
}

સ્થિર અંતઃરેખ bool sched_core_disabled(શૂન્ય)
{
	પાછા !static_branch_unlikely(&__sched_core_enabled);
}

/*
 * Be careful with this function; not for general use. The return value isn't
 * stable unless you actually hold a relevant rq->__lock.
 */
સ્થિર અંતઃરેખ raw_spinlock_t *rq_lockp(સંરચના rq *rq)
{
	જો (sched_core_enabled(rq))
		પાછા &rq->core->__lock;

	પાછા &rq->__lock;
}

સ્થિર અંતઃરેખ raw_spinlock_t *__rq_lockp(સંરચના rq *rq)
	__returns_ctx_lock(rq_lockp(rq)) /* alias them */
{
	જો (rq->core_enabled)
		પાછા &rq->core->__lock;

	પાછા &rq->__lock;
}

બાહ્ય bool
cfs_prio_less(અચલ સંરચના task_struct *a, અચલ સંરચના task_struct *b, bool fi);

બાહ્ય શૂન્ય task_vruntime_update(સંરચના rq *rq, સંરચના task_struct *p, bool in_fi);

/*
 * Helpers to check if the CPU's core cookie matches with the task's cookie
 * when core scheduling is enabled.
 * A special case is that the task's cookie always matches with CPU's core
 * cookie if the CPU is in an idle core.
 */
સ્થિર અંતઃરેખ bool sched_cpu_cookie_match(સંરચના rq *rq, સંરચના task_struct *p)
{
	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	જો (!sched_core_enabled(rq))
		પાછા true;

	પાછા rq->core->core_cookie == p->core_cookie;
}

સ્થિર અંતઃરેખ bool sched_core_cookie_match(સંરચના rq *rq, સંરચના task_struct *p)
{
	bool idle_core = true;
	પૂર્ણાંક cpu;

	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	જો (!sched_core_enabled(rq))
		પાછા true;

	જો (rq->core->core_cookie == p->core_cookie)
		પાછા true;

	for_each_cpu(cpu, cpu_smt_mask(cpu_of(rq))) {
		જો (!available_idle_cpu(cpu)) {
			idle_core = false;
			તોડો;
		}
	}

	/*
	 * A CPU in an idle core is always the best choice for tasks with
	 * cookies.
	 */
	પાછા idle_core;
}

સ્થિર અંતઃરેખ bool sched_group_cookie_match(સંરચના rq *rq,
					    સંરચના task_struct *p,
					    સંરચના sched_group *group)
{
	પૂર્ણાંક cpu;

	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	જો (!sched_core_enabled(rq))
		પાછા true;

	for_each_cpu_and(cpu, sched_group_span(group), p->cpus_ptr) {
		જો (sched_core_cookie_match(cpu_rq(cpu), p))
			પાછા true;
	}
	પાછા false;
}

સ્થિર અંતઃરેખ bool sched_core_enqueued(સંરચના task_struct *p)
{
	પાછા !RB_EMPTY_ગાંઠ(&p->core_ગાંઠ);
}

બાહ્ય શૂન્ય sched_core_enqueue(સંરચના rq *rq, સંરચના task_struct *p);
બાહ્ય શૂન્ય sched_core_dequeue(સંરચના rq *rq, સંરચના task_struct *p, પૂર્ણાંક flags);

બાહ્ય શૂન્ય sched_core_get(શૂન્ય);
બાહ્ય શૂન્ય sched_core_put(શૂન્ય);

સ્થિર અંતઃરેખ bool task_has_sched_core(સંરચના task_struct *p)
{
	જો (sched_core_disabled())
		પાછા false;

	પાછા !!p->core_cookie;
}

#નહીંતર /* !CONFIG_SCHED_CORE: */

સ્થિર અંતઃરેખ bool sched_core_enabled(સંરચના rq *rq)
{
	પાછા false;
}

સ્થિર અંતઃરેખ bool sched_core_disabled(શૂન્ય)
{
	પાછા true;
}

સ્થિર અંતઃરેખ raw_spinlock_t *rq_lockp(સંરચના rq *rq)
{
	પાછા &rq->__lock;
}

સ્થિર અંતઃરેખ raw_spinlock_t *__rq_lockp(સંરચના rq *rq)
	__returns_ctx_lock(rq_lockp(rq)) /* alias them */
{
	પાછા &rq->__lock;
}

સ્થિર અંતઃરેખ bool sched_cpu_cookie_match(સંરચના rq *rq, સંરચના task_struct *p)
{
	પાછા true;
}

સ્થિર અંતઃરેખ bool sched_core_cookie_match(સંરચના rq *rq, સંરચના task_struct *p)
{
	પાછા true;
}

સ્થિર અંતઃરેખ bool sched_group_cookie_match(સંરચના rq *rq,
					    સંરચના task_struct *p,
					    સંરચના sched_group *group)
{
	પાછા true;
}

સ્થિર અંતઃરેખ bool task_has_sched_core(સંરચના task_struct *p)
{
	પાછા false;
}

#endif /* !CONFIG_SCHED_CORE */

#ifdef CONFIG_RT_GROUP_SCHED
# ifdef CONFIG_RT_GROUP_SCHED_DEFAULT_DISABLED
DECLARE_STATIC_ચાવી_FALSE(rt_group_sched);
સ્થિર અંતઃરેખ bool rt_group_sched_enabled(શૂન્ય)
{
	પાછા static_branch_unlikely(&rt_group_sched);
}
# નહીંતર /* !CONFIG_RT_GROUP_SCHED_DEFAULT_DISABLED: */
DECLARE_STATIC_ચાવી_TRUE(rt_group_sched);
સ્થિર અંતઃરેખ bool rt_group_sched_enabled(શૂન્ય)
{
	પાછા static_branch_likely(&rt_group_sched);
}
# endif /* !CONFIG_RT_GROUP_SCHED_DEFAULT_DISABLED */
#નહીંતર /* !CONFIG_RT_GROUP_SCHED: */
# define rt_group_sched_enabled()	false
#endif /* !CONFIG_RT_GROUP_SCHED */

સ્થિર અંતઃરેખ શૂન્ય lockdep_assert_rq_held(સંરચના rq *rq)
	__assumes_ctx_lock(__rq_lockp(rq))
{
	lockdep_assert_held(__rq_lockp(rq));
}

બાહ્ય શૂન્ય raw_spin_rq_lock_nested(સંરચના rq *rq, પૂર્ણાંક subclass)
	__acquires(__rq_lockp(rq));

બાહ્ય bool raw_spin_rq_trylock(સંરચના rq *rq)
	__cond_acquires(true, __rq_lockp(rq));

સ્થિર અંતઃરેખ શૂન્ય raw_spin_rq_lock(સંરચના rq *rq)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_lock_nested(rq, 0);
}

સ્થિર અંતઃરેખ શૂન્ય raw_spin_rq_unlock(સંરચના rq *rq)
	__releases(__rq_lockp(rq))
{
	raw_spin_unlock(rq_lockp(rq));
}

સ્થિર અંતઃરેખ શૂન્ય raw_spin_rq_lock_irq(સંરચના rq *rq)
	__acquires(__rq_lockp(rq))
{
	local_irq_disable();
	raw_spin_rq_lock(rq);
}

સ્થિર અંતઃરેખ શૂન્ય raw_spin_rq_unlock_irq(સંરચના rq *rq)
	__releases(__rq_lockp(rq))
{
	raw_spin_rq_unlock(rq);
	local_irq_enable();
}

સ્થિર અંતઃરેખ unsigned long _raw_spin_rq_lock_irqsave(સંરચના rq *rq)
	__acquires(__rq_lockp(rq))
{
	unsigned long flags;

	local_irq_save(flags);
	raw_spin_rq_lock(rq);

	પાછા flags;
}

સ્થિર અંતઃરેખ શૂન્ય raw_spin_rq_unlock_irqrestore(સંરચના rq *rq, unsigned long flags)
	__releases(__rq_lockp(rq))
{
	raw_spin_rq_unlock(rq);
	local_irq_restore(flags);
}

#define raw_spin_rq_lock_irqsave(rq, flags)	\
કરો {						\
	flags = _raw_spin_rq_lock_irqsave(rq);	\
} જ્યારે (0)

બાહ્ય શૂન્ય __update_idle_core(સંરચના rq *rq);

સ્થિર અંતઃરેખ શૂન્ય update_idle_core(સંરચના rq *rq)
{
	જો (sched_smt_active())
		__update_idle_core(rq);
}

#ifdef CONFIG_FAIR_GROUP_SCHED
સ્થિર અંતઃરેખ સંરચના task_struct *task_of(સંરચના sched_entity *se)
{
	WARN_ON_ONCE(!entity_is_task(se));
	પાછા container_of(se, સંરચના task_struct, se);
}

સ્થિર અંતઃરેખ સંરચના cfs_rq *task_cfs_rq(સંરચના task_struct *p)
{
	પાછા p->se.cfs_rq;
}

/* runqueue on which this entity is (to be) queued */
સ્થિર અંતઃરેખ સંરચના cfs_rq *cfs_rq_of(અચલ સંરચના sched_entity *se)
{
	પાછા se->cfs_rq;
}

/* runqueue "owned" by this group */
સ્થિર અંતઃરેખ સંરચના cfs_rq *group_cfs_rq(સંરચના sched_entity *grp)
{
	પાછા grp->my_q;
}

#નહીંતર /* !CONFIG_FAIR_GROUP_SCHED: */

#define task_of(_se)		container_of(_se, સંરચના task_struct, se)

સ્થિર અંતઃરેખ સંરચના cfs_rq *task_cfs_rq(અચલ સંરચના task_struct *p)
{
	પાછા &task_rq(p)->cfs;
}

સ્થિર અંતઃરેખ સંરચના cfs_rq *cfs_rq_of(અચલ સંરચના sched_entity *se)
{
	અચલ સંરચના task_struct *p = task_of(se);
	સંરચના rq *rq = task_rq(p);

	પાછા &rq->cfs;
}

/* runqueue "owned" by this group */
સ્થિર અંતઃરેખ સંરચના cfs_rq *group_cfs_rq(સંરચના sched_entity *grp)
{
	પાછા ખાલી;
}

#endif /* !CONFIG_FAIR_GROUP_SCHED */

બાહ્ય શૂન્ય update_rq_avg_idle(સંરચના rq *rq);
બાહ્ય શૂન્ય update_rq_clock(સંરચના rq *rq);

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

સ્થિર અંતઃરેખ શૂન્ય assert_clock_updated(સંરચના rq *rq)
{
	/*
	 * The only reason for not seeing a clock update since the
	 * last rq_pin_lock() is if we're currently skipping updates.
	 */
	WARN_ON_ONCE(rq->clock_update_flags < RQCF_ACT_SKIP);
}

સ્થિર અંતઃરેખ u64 rq_clock(સંરચના rq *rq)
{
	lockdep_assert_rq_held(rq);
	assert_clock_updated(rq);

	પાછા rq->clock;
}

સ્થિર અંતઃરેખ u64 rq_clock_task(સંરચના rq *rq)
{
	lockdep_assert_rq_held(rq);
	assert_clock_updated(rq);

	પાછા rq->clock_task;
}

સ્થિર અંતઃરેખ શૂન્ય rq_clock_skip_update(સંરચના rq *rq)
{
	lockdep_assert_rq_held(rq);
	rq->clock_update_flags |= RQCF_REQ_SKIP;
}

/*
 * See rt task throttling, which is the only time a skip
 * request is canceled.
 */
સ્થિર અંતઃરેખ શૂન્ય rq_clock_cancel_skipupdate(સંરચના rq *rq)
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
સ્થિર અંતઃરેખ શૂન્ય rq_clock_start_loop_update(સંરચના rq *rq)
{
	lockdep_assert_rq_held(rq);
	WARN_ON_ONCE(rq->clock_update_flags & RQCF_ACT_SKIP);
	rq->clock_update_flags |= RQCF_ACT_SKIP;
}

સ્થિર અંતઃરેખ શૂન્ય rq_clock_stop_loop_update(સંરચના rq *rq)
{
	lockdep_assert_rq_held(rq);
	rq->clock_update_flags &= ~RQCF_ACT_SKIP;
}

સંરચના rq_flags {
	unsigned long flags;
	સંરચના pin_cookie cookie;
	/*
	 * A copy of (rq::clock_update_flags & RQCF_UPDATED) for the
	 * current pin context is stashed here in case it needs to be
	 * restored in rq_repin_lock().
	 */
	unsigned પૂર્ણાંક clock_update_flags;
};

બાહ્ય સંરચના balance_callback balance_ધકેલો_callback;

#ifdef CONFIG_SCHED_CLASS_EXT
બાહ્ય અચલ સંરચના sched_class ext_sched_class;

DECLARE_STATIC_ચાવી_FALSE(__scx_enabled);	/* SCX BPF scheduler loaded */
DECLARE_STATIC_ચાવી_FALSE(__scx_switched_all);	/* all fair class tasks on SCX */

#define scx_enabled()		static_branch_unlikely(&__scx_enabled)
#define scx_switched_all()	static_branch_unlikely(&__scx_switched_all)

સ્થિર અંતઃરેખ શૂન્ય scx_rq_clock_update(સંરચના rq *rq, u64 clock)
{
	જો (!scx_enabled())
		પાછા;
	WRITE_ONCE(rq->scx.clock, clock);
	smp_store_release(&rq->scx.flags, rq->scx.flags | SCX_RQ_CLK_VALID);
}

સ્થિર અંતઃરેખ શૂન્ય scx_rq_clock_invalidate(સંરચના rq *rq)
{
	જો (!scx_enabled())
		પાછા;
	WRITE_ONCE(rq->scx.flags, rq->scx.flags & ~SCX_RQ_CLK_VALID);
}

#નહીંતર /* !CONFIG_SCHED_CLASS_EXT: */
#define scx_enabled()		false
#define scx_switched_all()	false

સ્થિર અંતઃરેખ શૂન્ય scx_rq_clock_update(સંરચના rq *rq, u64 clock) {}
સ્થિર અંતઃરેખ શૂન્ય scx_rq_clock_invalidate(સંરચના rq *rq) {}
#endif /* !CONFIG_SCHED_CLASS_EXT */

સ્થિર અંતઃરેખ શૂન્ય assert_balance_callbacks_empty(સંરચના rq *rq)
{
	WARN_ON_ONCE(IS_ENABLED(CONFIG_PROVE_LOCKING) &&
		     rq->balance_callback &&
		     rq->balance_callback != &balance_ધકેલો_callback);
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
સ્થિર અંતઃરેખ શૂન્ય rq_pin_lock(સંરચના rq *rq, સંરચના rq_flags *rf)
{
	rf->cookie = lockdep_pin_lock(__rq_lockp(rq));

	rq->clock_update_flags &= (RQCF_REQ_SKIP|RQCF_ACT_SKIP);
	rf->clock_update_flags = 0;
	assert_balance_callbacks_empty(rq);
}

સ્થિર અંતઃરેખ શૂન્ય rq_unpin_lock(સંરચના rq *rq, સંરચના rq_flags *rf)
{
	જો (rq->clock_update_flags > RQCF_ACT_SKIP)
		rf->clock_update_flags = RQCF_UPDATED;

	scx_rq_clock_invalidate(rq);
	lockdep_unpin_lock(__rq_lockp(rq), rf->cookie);
}

સ્થિર અંતઃરેખ શૂન્ય rq_repin_lock(સંરચના rq *rq, સંરચના rq_flags *rf)
{
	lockdep_repin_lock(__rq_lockp(rq), rf->cookie);

	/*
	 * Restore the value we stashed in @rf for this pin context.
	 */
	rq->clock_update_flags |= rf->clock_update_flags;
}

#define __task_rq_lock(...) __acquire_ret(___task_rq_lock(__VA_ARGS__), __rq_lockp(__ret))
બાહ્ય સંરચના rq *___task_rq_lock(સંરચના task_struct *p, સંરચના rq_flags *rf) __acquires_ret;

#define task_rq_lock(...) __acquire_ret(_task_rq_lock(__VA_ARGS__), __rq_lockp(__ret))
બાહ્ય સંરચના rq *_task_rq_lock(સંરચના task_struct *p, સંરચના rq_flags *rf)
	__acquires(&p->pi_lock) __acquires_ret;

સ્થિર અંતઃરેખ શૂન્ય
__task_rq_unlock(સંરચના rq *rq, સંરચના task_struct *p, સંરચના rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_lock(rq, rf);
	raw_spin_rq_unlock(rq);
}

સ્થિર અંતઃરેખ શૂન્ય
task_rq_unlock(સંરચના rq *rq, સંરચના task_struct *p, સંરચના rq_flags *rf)
	__releases(__rq_lockp(rq), &p->pi_lock)
{
	__task_rq_unlock(rq, p, rf);
	raw_spin_unlock_irqrestore(&p->pi_lock, rf->flags);
}

DEFINE_LOCK_GUARD_1(task_rq_lock, સંરચના task_struct,
		    _T->rq = task_rq_lock(_T->lock, &_T->rf),
		    task_rq_unlock(_T->rq, _T->lock, &_T->rf),
		    સંરચના rq *rq; સંરચના rq_flags rf)
DECLARE_LOCK_GUARD_1_ATTRS(task_rq_lock, __acquires(_T->pi_lock), __releases((*(સંરચના task_struct **)_T)->pi_lock))
#define class_task_rq_lock_constructor(_T) WITH_LOCK_GUARD_1_ATTRS(task_rq_lock, _T)

DEFINE_LOCK_GUARD_1(__task_rq_lock, સંરચના task_struct,
		    _T->rq = __task_rq_lock(_T->lock, &_T->rf),
		    __task_rq_unlock(_T->rq, _T->lock, &_T->rf),
		    સંરચના rq *rq; સંરચના rq_flags rf)

સ્થિર અંતઃરેખ શૂન્ય rq_lock_irqsave(સંરચના rq *rq, સંરચના rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_lock_irqsave(rq, rf->flags);
	rq_pin_lock(rq, rf);
}

સ્થિર અંતઃરેખ શૂન્ય rq_lock_irq(સંરચના rq *rq, સંરચના rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_lock_irq(rq);
	rq_pin_lock(rq, rf);
}

સ્થિર અંતઃરેખ શૂન્ય rq_lock(સંરચના rq *rq, સંરચના rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_lock(rq);
	rq_pin_lock(rq, rf);
}

સ્થિર અંતઃરેખ શૂન્ય rq_unlock_irqrestore(સંરચના rq *rq, સંરચના rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_lock(rq, rf);
	raw_spin_rq_unlock_irqrestore(rq, rf->flags);
}

સ્થિર અંતઃરેખ શૂન્ય rq_unlock_irq(સંરચના rq *rq, સંરચના rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_lock(rq, rf);
	raw_spin_rq_unlock_irq(rq);
}

સ્થિર અંતઃરેખ શૂન્ય rq_unlock(સંરચના rq *rq, સંરચના rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_lock(rq, rf);
	raw_spin_rq_unlock(rq);
}

DEFINE_LOCK_GUARD_1(rq_lock, સંરચના rq,
		    rq_lock(_T->lock, &_T->rf),
		    rq_unlock(_T->lock, &_T->rf),
		    સંરચના rq_flags rf)

DECLARE_LOCK_GUARD_1_ATTRS(rq_lock, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(સંરચના rq **)_T)));
#define class_rq_lock_constructor(_T) WITH_LOCK_GUARD_1_ATTRS(rq_lock, _T)

DEFINE_LOCK_GUARD_1(rq_lock_irq, સંરચના rq,
		    rq_lock_irq(_T->lock, &_T->rf),
		    rq_unlock_irq(_T->lock, &_T->rf),
		    સંરચના rq_flags rf)

DECLARE_LOCK_GUARD_1_ATTRS(rq_lock_irq, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(સંરચના rq **)_T)));
#define class_rq_lock_irq_constructor(_T) WITH_LOCK_GUARD_1_ATTRS(rq_lock_irq, _T)

DEFINE_LOCK_GUARD_1(rq_lock_irqsave, સંરચના rq,
		    rq_lock_irqsave(_T->lock, &_T->rf),
		    rq_unlock_irqrestore(_T->lock, &_T->rf),
		    સંરચના rq_flags rf)

DECLARE_LOCK_GUARD_1_ATTRS(rq_lock_irqsave, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(સંરચના rq **)_T)));
#define class_rq_lock_irqsave_constructor(_T) WITH_LOCK_GUARD_1_ATTRS(rq_lock_irqsave, _T)

DEFINE_LOCK_GUARD_1(raw_spin_rq_lock_irqsave, સંરચના rq,
		    raw_spin_rq_lock_irqsave(_T->lock, _T->flags),
		    raw_spin_rq_unlock_irqrestore(_T->lock, _T->flags),
		    unsigned long flags)

DECLARE_LOCK_GUARD_1_ATTRS(raw_spin_rq_lock_irqsave, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(સંરચના rq **)_T)));
#define class_raw_spin_rq_lock_irqsave_constructor(_T) \
	WITH_LOCK_GUARD_1_ATTRS(raw_spin_rq_lock_irqsave, _T)

#define this_rq_lock_irq(...) __acquire_ret(_this_rq_lock_irq(__VA_ARGS__), __rq_lockp(__ret))
સ્થિર અંતઃરેખ સંરચના rq *_this_rq_lock_irq(સંરચના rq_flags *rf) __acquires_ret
{
	સંરચના rq *rq;

	local_irq_disable();
	rq = this_rq();
	rq_lock(rq, rf);

	પાછા rq;
}

#ifdef CONFIG_NUMA

ગણના numa_topology_type {
	NUMA_DIRECT,
	NUMA_GLUELESS_MESH,
	NUMA_BACKPLANE,
};

બાહ્ય ગણના numa_topology_type sched_numa_topology_type;
બાહ્ય પૂર્ણાંક sched_max_numa_distance;
બાહ્ય bool find_numa_distance(પૂર્ણાંક distance);
બાહ્ય શૂન્ય sched_init_numa(પૂર્ણાંક offline_ગાંઠ);
બાહ્ય શૂન્ય sched_update_numa(પૂર્ણાંક cpu, bool online);
બાહ્ય શૂન્ય sched_domains_numa_masks_set(unsigned પૂર્ણાંક cpu);
બાહ્ય શૂન્ય sched_domains_numa_masks_clear(unsigned પૂર્ણાંક cpu);
બાહ્ય પૂર્ણાંક sched_numa_find_closest(અચલ સંરચના cpumask *cpus, પૂર્ણાંક cpu);

#નહીંતર /* !CONFIG_NUMA: */

સ્થિર અંતઃરેખ શૂન્ય sched_init_numa(પૂર્ણાંક offline_ગાંઠ) { }
સ્થિર અંતઃરેખ શૂન્ય sched_update_numa(પૂર્ણાંક cpu, bool online) { }
સ્થિર અંતઃરેખ શૂન્ય sched_domains_numa_masks_set(unsigned પૂર્ણાંક cpu) { }
સ્થિર અંતઃરેખ શૂન્ય sched_domains_numa_masks_clear(unsigned પૂર્ણાંક cpu) { }

સ્થિર અંતઃરેખ પૂર્ણાંક sched_numa_find_closest(અચલ સંરચના cpumask *cpus, પૂર્ણાંક cpu)
{
	પાછા nr_cpu_ids;
}

#endif /* !CONFIG_NUMA */

#ifdef CONFIG_NUMA_BALANCING

/* The regions in numa_faults array from task_struct */
ગણના numa_faults_stats {
	NUMA_MEM = 0,
	NUMA_CPU,
	NUMA_MEMBUF,
	NUMA_CPUBUF
};

બાહ્ય શૂન્ય sched_setnuma(સંરચના task_struct *p, પૂર્ણાંક ગાંઠ);
બાહ્ય પૂર્ણાંક migrate_task_to(સંરચના task_struct *p, પૂર્ણાંક cpu);
બાહ્ય પૂર્ણાંક migrate_swap(સંરચના task_struct *p, સંરચના task_struct *t,
			પૂર્ણાંક cpu, પૂર્ણાંક scpu);
બાહ્ય શૂન્ય init_numa_balancing(u64 clone_flags, સંરચના task_struct *p);

#નહીંતર /* !CONFIG_NUMA_BALANCING: */

સ્થિર અંતઃરેખ શૂન્ય
init_numa_balancing(u64 clone_flags, સંરચના task_struct *p)
{
}

#endif /* !CONFIG_NUMA_BALANCING */

પૂર્ણાંક task_llc(અચલ સંરચના task_struct *p);

સ્થિર અંતઃરેખ શૂન્ય
queue_balance_callback(સંરચના rq *rq,
		       સંરચના balance_callback *head,
		       શૂન્ય (*func)(સંરચના rq *rq))
{
	lockdep_assert_rq_held(rq);

	/*
	 * Don't (re)queue an already queued item; nor queue anything when
	 * balance_push() is active, see the comment with
	 * balance_push_callback.
	 */
	જો (unlikely(head->next || rq->balance_callback == &balance_ધકેલો_callback))
		પાછા;

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
	માટે (__sd = rcu_dereference_sched_domain(cpu_rq(cpu)->sd); \
			__sd; __sd = __sd->parent)

/* A mask of all the SD flags that have the SDF_SHARED_CHILD metaflag */
#define SD_FLAG(name, mflags) (name * !!((mflags) & SDF_SHARED_CHILD)) |
સ્થિર અચલ unsigned પૂર્ણાંક SD_SHARED_CHILD_MASK =
#અન્તર્ભાવ <linux/sched/sd_flags.h>
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
સ્થિર અંતઃરેખ સંરચના sched_domain *highest_flag_domain(પૂર્ણાંક cpu, પૂર્ણાંક flag)
{
	સંરચના sched_domain *sd, *hsd = ખાલી;

	for_each_domain(cpu, sd) {
		જો (sd->flags & flag) {
			hsd = sd;
			ચાલુ;
		}

		/*
		 * Stop the search if @flag is known to be shared at lower
		 * levels. It will not be found further up.
		 */
		જો (flag & SD_SHARED_CHILD_MASK)
			તોડો;
	}

	પાછા hsd;
}

સ્થિર અંતઃરેખ સંરચના sched_domain *lowest_flag_domain(પૂર્ણાંક cpu, પૂર્ણાંક flag)
{
	સંરચના sched_domain *sd;

	for_each_domain(cpu, sd) {
		જો (sd->flags & flag)
			તોડો;
	}

	પાછા sd;
}

DECLARE_PER_CPU(સંરચના sched_domain __rcu *, sd_llc);
DECLARE_PER_CPU(પૂર્ણાંક, sd_llc_size);
DECLARE_PER_CPU(પૂર્ણાંક, sd_llc_id);
DECLARE_PER_CPU(પૂર્ણાંક, sd_share_id);
DECLARE_PER_CPU(સંરચના sched_domain_shared __rcu *, sd_llc_shared);
DECLARE_PER_CPU(સંરચના sched_domain_shared __rcu *, sd_balance_shared);
DECLARE_PER_CPU(સંરચના sched_domain __rcu *, sd_numa);
DECLARE_PER_CPU(સંરચના sched_domain __rcu *, sd_asym_packing);
DECLARE_PER_CPU(સંરચના sched_domain __rcu *, sd_asym_cpucapacity);

બાહ્ય સંરચના static_ચાવી_false sched_asym_cpucapacity;
બાહ્ય સંરચના static_ચાવી_false sched_cluster_active;

સ્થિર __always_inline bool sched_asym_cpucap_active(શૂન્ય)
{
	પાછા static_branch_unlikely(&sched_asym_cpucapacity);
}

સંરચના sched_group_capacity {
	atomic_t		ref;
	/*
	 * CPU capacity of this group, SCHED_CAPACITY_SCALE being max capacity
	 * for a single CPU.
	 */
	unsigned long		capacity;
	unsigned long		min_capacity;		/* Min per-CPU capacity in group */
	unsigned long		max_capacity;		/* Max per-CPU capacity in group */
	unsigned long		next_update;
	પૂર્ણાંક			imbalance;		/* XXX unrelated to capacity but shared group state */

	પૂર્ણાંક			id;

	unsigned long		cpumask[];		/* Balance mask */
};

સંરચના sched_group {
	સંરચના sched_group	*next;			/* Must be a circular list */
	atomic_t		ref;

	unsigned પૂર્ણાંક		group_weight;
	unsigned પૂર્ણાંક		cores;
	સંરચના sched_group_capacity *sgc;
	પૂર્ણાંક			asym_prefer_cpu;	/* CPU of highest priority in group */
	પૂર્ણાંક			flags;

	/*
	 * The CPUs this group covers.
	 *
	 * NOTE: this field is variable length. (Allocated dynamically
	 * by attaching extra space to the end of the structure,
	 * depending on how many CPUs the kernel has booted up with)
	 */
	unsigned long		cpumask[];
};

સ્થિર અંતઃરેખ સંરચના cpumask *sched_group_span(સંરચના sched_group *sg)
{
	પાછા to_cpumask(sg->cpumask);
}

/*
 * See build_balance_mask().
 */
સ્થિર અંતઃરેખ સંરચના cpumask *group_balance_mask(સંરચના sched_group *sg)
{
	પાછા to_cpumask(sg->sgc->cpumask);
}

બાહ્ય પૂર્ણાંક group_balance_cpu(સંરચના sched_group *sg);

બાહ્ય શૂન્ય update_sched_domain_debugfs(શૂન્ય);
બાહ્ય શૂન્ય dirty_sched_domain_sysctl(પૂર્ણાંક cpu);

બાહ્ય પૂર્ણાંક sched_update_scaling(શૂન્ય);

સ્થિર અંતઃરેખ અચલ સંરચના cpumask *task_user_cpus(સંરચના task_struct *p)
{
	જો (!p->user_cpus_ptr)
		પાછા cpu_possible_mask; /* &init_task.cpus_mask */
	પાછા p->user_cpus_ptr;
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
સ્થિર અંતઃરેખ સંરચના task_group *task_group(સંરચના task_struct *p)
{
	પાછા p->sched_task_group;
}

#ifdef CONFIG_FAIR_GROUP_SCHED
/*
 * Defined here to be available before stats.h is included, since
 * stats.h has dependencies on things defined later in this file.
 */
સંરચના cfs_tg_state {
	સંરચના cfs_rq		cfs_rq;
	સંરચના sched_entity	se;
	સંરચના sched_statistics	stats;
} __no_randomize_layout;

/* Access a specific CPU's cfs_rq from a task group */
સ્થિર અંતઃરેખ સંરચના cfs_rq *tg_cfs_rq(સંરચના task_group *tg, પૂર્ણાંક cpu)
{
	પાછા per_cpu_ptr(tg->cfs_rq, cpu);
}

સ્થિર અંતઃરેખ સંરચના sched_entity *tg_se(સંરચના task_group *tg, પૂર્ણાંક cpu)
{
	સંરચના cfs_tg_state *state;

	જો (is_મૂળ_task_group(tg))
		પાછા ખાલી;

	state = container_of(tg_cfs_rq(tg, cpu), સંરચના cfs_tg_state, cfs_rq);
	પાછા &state->se;
}

સ્થિર અંતઃરેખ સંરચના sched_entity *cfs_rq_se(સંરચના cfs_rq *cfs_rq)
{
	સંરચના cfs_tg_state *state;

	જો (is_મૂળ_task_group(cfs_rq->tg))
		પાછા ખાલી;

	state = container_of(cfs_rq, સંરચના cfs_tg_state, cfs_rq);
	પાછા &state->se;
}
#endif

/* Change a task's cfs_rq and parent entity if it moves across CPUs/groups */
સ્થિર અંતઃરેખ શૂન્ય set_task_rq(સંરચના task_struct *p, unsigned પૂર્ણાંક cpu)
{
#જો defined(CONFIG_FAIR_GROUP_SCHED) || defined(CONFIG_RT_GROUP_SCHED)
	સંરચના task_group *tg = task_group(p);
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
	જો (!rt_group_sched_enabled())
		tg = &મૂળ_task_group;
	p->rt.rt_rq  = tg->rt_rq[cpu];
	p->rt.parent = tg->rt_se[cpu];
#endif /* CONFIG_RT_GROUP_SCHED */
}

#નહીંતર /* !CONFIG_CGROUP_SCHED: */

સ્થિર અંતઃરેખ શૂન્ય set_task_rq(સંરચના task_struct *p, unsigned પૂર્ણાંક cpu) { }

સ્થિર અંતઃરેખ સંરચના task_group *task_group(સંરચના task_struct *p)
{
	પાછા ખાલી;
}

#endif /* !CONFIG_CGROUP_SCHED */

સ્થિર અંતઃરેખ શૂન્ય __set_task_cpu(સંરચના task_struct *p, unsigned પૂર્ણાંક cpu)
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

ગણના {
#અન્તર્ભાવ "features.h"
	__SCHED_FEAT_NR,
};

#undef SCHED_FEAT

/*
 * To support run-time toggling of sched features, all the translation units
 * (but core.c) reference the sysctl_sched_features defined in core.c.
 */
બાહ્ય __read_mostly unsigned પૂર્ણાંક sysctl_sched_features;

#ifdef CONFIG_JUMP_LABEL

સંઘ sched_feat_ચાવી {
	સંરચના static_ચાવી_true	ચાવી_true;
	સંરચના static_ચાવી_false	ચાવી_false;
};

#define sched_feat_branch_true(ચાવી)	static_branch_likely(&(ચાવી)->ચાવી_true)
#define sched_feat_branch_false(ચાવી)	static_branch_unlikely(&(ચાવી)->ચાવી_false)

#define SCHED_FEAT(name, enabled)			\
સ્થિર __always_inline bool				\
static_branch_##name(સંઘ sched_feat_ચાવી *ચાવી) 	\
{							\
	પાછા sched_feat_branch_##enabled(ચાવી);	\
}

#અન્તર્ભાવ "features.h"
#undef SCHED_FEAT

બાહ્ય સંઘ sched_feat_ચાવી sched_feat_ચાવીઓ[__SCHED_FEAT_NR];
#define sched_feat(x) (static_branch_##x(&sched_feat_ચાવીઓ[__SCHED_FEAT_##x]))

#નહીંતર /* !CONFIG_JUMP_LABEL: */

#define sched_feat(x) (sysctl_sched_features & (1UL << __SCHED_FEAT_##x))

#endif /* !CONFIG_JUMP_LABEL */

બાહ્ય સંરચના static_ચાવી_false sched_numa_balancing;
બાહ્ય સંરચના static_ચાવી_false sched_schedstats;

સ્થિર અંતઃરેખ u64 global_rt_period(શૂન્ય)
{
	પાછા (u64)sysctl_sched_rt_period * NSEC_PER_USEC;
}

સ્થિર અંતઃરેખ u64 global_rt_runtime(શૂન્ય)
{
	જો (sysctl_sched_rt_runtime < 0)
		પાછા RUNTIME_INF;

	પાછા (u64)sysctl_sched_rt_runtime * NSEC_PER_USEC;
}

/*
 * Is p the current execution context?
 */
સ્થિર અંતઃરેખ પૂર્ણાંક task_current(સંરચના rq *rq, સંરચના task_struct *p)
{
	પાછા rq->curr == p;
}

/*
 * Is p the current scheduling context?
 *
 * Note that it might be the current execution context at the same time if
 * rq->curr == rq->donor == p.
 */
સ્થિર અંતઃરેખ પૂર્ણાંક task_current_donor(સંરચના rq *rq, સંરચના task_struct *p)
{
	પાછા rq->donor == p;
}

સ્થિર અંતઃરેખ bool task_is_blocked(સંરચના task_struct *p)
{
	જો (!sched_proxy_exec())
		પાછા false;

	પાછા !!p->blocked_on;
}

#ifdef CONFIG_SCHED_PROXY_EXEC
શૂન્ય sched_proxy_block_task(સંરચના rq *rq, સંરચના task_struct *p);
#નહીંતર
સ્થિર અંતઃરેખ શૂન્ય sched_proxy_block_task(સંરચના rq *rq, સંરચના task_struct *p) {}
#endif

સ્થિર અંતઃરેખ પૂર્ણાંક task_on_cpu(સંરચના rq *rq, સંરચના task_struct *p)
{
	પાછા p->on_cpu;
}

સ્થિર અંતઃરેખ પૂર્ણાંક task_on_rq_queued(સંરચના task_struct *p)
{
	પાછા READ_ONCE(p->on_rq) == TASK_ON_RQ_QUEUED;
}

સ્થિર અંતઃરેખ પૂર્ણાંક task_on_rq_migrating(સંરચના task_struct *p)
{
	પાછા READ_ONCE(p->on_rq) == TASK_ON_RQ_MIGRATING;
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

બાહ્ય અચલ પૂર્ણાંક		sched_prio_to_weight[40];
બાહ્ય અચલ u32		sched_prio_to_wmult[40];

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

#define RETRY_TASK		((શૂન્ય *)-1UL)

સંરચના affinity_context {
	અચલ સંરચના cpumask	*new_mask;
	સંરચના cpumask		*user_mask;
	unsigned પૂર્ણાંક		flags;
};

બાહ્ય s64 update_curr_common(સંરચના rq *rq);

ગણના snt_e {
	SNT_NORMAL,	/* set_next_task() */
	SNT_PICK,	/* put_prev_set_next_task(): prev != next */
	SNT_REPICK,	/* put_prev_set_next_task(): prev == next */
};

સંરચના sched_class {

#ifdef CONFIG_UCLAMP_TASK
	પૂર્ણાંક uclamp_enabled;
#endif

	/*
	 * move_queued_task/activate_task/enqueue_task: rq->lock
	 * ttwu_do_activate/activate_task/enqueue_task: rq->lock
	 * wake_up_new_task/activate_task/enqueue_task: task_rq_lock
	 * ttwu_runnable/enqueue_task: task_rq_lock
	 * proxy_task_current: rq->lock
	 * sched_change_end
	 */
	શૂન્ય (*enqueue_task) (સંરચના rq *rq, સંરચના task_struct *p, પૂર્ણાંક flags);
	/*
	 * move_queued_task/deactivate_task/dequeue_task: rq->lock
	 * __schedule/block_task/dequeue_task: rq->lock
	 * proxy_task_current: rq->lock
	 * wait_task_inactive: task_rq_lock
	 * sched_change_begin
	 */
	bool (*dequeue_task) (સંરચના rq *rq, સંરચના task_struct *p, પૂર્ણાંક flags);

	/*
	 * do_sched_yield: rq->lock
	 */
	શૂન્ય (*yield_task)   (સંરચના rq *rq);
	/*
	 * yield_to: rq->lock (double)
	 */
	bool (*yield_to_task)(સંરચના rq *rq, સંરચના task_struct *p);

	/*
	 * move_queued_task: rq->lock
	 * __migrate_swap_task: rq->lock
	 * ttwu_do_activate: rq->lock
	 * ttwu_runnable: task_rq_lock
	 * wake_up_new_task: task_rq_lock
	 */
	શૂન્ય (*wakeup_preempt)(સંરચના rq *rq, સંરચના task_struct *p, પૂર્ણાંક flags);

	/*
	 * schedule/pick_next_task/prev_balance: rq->lock
	 */
	પૂર્ણાંક (*balance)(સંરચના rq *rq, સંરચના rq_flags *rf);

	/*
	 * schedule/pick_next_task: rq->lock
	 */
	સંરચના task_struct *(*pick_task)(સંરચના rq *rq, સંરચના rq_flags *rf);

	/*
	 * sched_change:
	 * __schedule: rq->lock
	 */
	શૂન્ય (*put_prev_task)(સંરચના rq *rq, સંરચના task_struct *p, સંરચના task_struct *next);
	શૂન્ય (*set_next_task)(સંરચના rq *rq, સંરચના task_struct *p, ગણના snt_e type);

	/*
	 * select_task_rq: p->pi_lock
	 * sched_exec: p->pi_lock
	 */
	પૂર્ણાંક  (*select_task_rq)(સંરચના task_struct *p, પૂર્ણાંક task_cpu, પૂર્ણાંક flags);

	/*
	 * set_task_cpu: p->pi_lock || rq->lock (ttwu like)
	 */
	શૂન્ય (*migrate_task_rq)(સંરચના task_struct *p, પૂર્ણાંક new_cpu);

	/*
	 * ttwu_do_activate: rq->lock
	 * wake_up_new_task: task_rq_lock
	 */
	શૂન્ય (*task_woken)(સંરચના rq *this_rq, સંરચના task_struct *task);

	/*
	 * do_set_cpus_allowed: task_rq_lock + sched_change
	 */
	શૂન્ય (*set_cpus_allowed)(સંરચના task_struct *p, સંરચના affinity_context *ctx);

	/*
	 * sched_set_rq_{on,off}line: rq->lock
	 */
	શૂન્ય (*rq_online)(સંરચના rq *rq);
	શૂન્ય (*rq_offline)(સંરચના rq *rq);

	/*
	 * push_cpu_stop: p->pi_lock && rq->lock
	 */
	સંરચના rq *(*find_lock_rq)(સંરચના task_struct *p, સંરચના rq *rq);

	/*
	 * hrtick: rq->lock
	 * sched_tick: rq->lock
	 * sched_tick_remote: rq->lock
	 */
	શૂન્ય (*task_tick)(સંરચના rq *rq, સંરચના task_struct *p, પૂર્ણાંક queued);
	/*
	 * sched_cgroup_fork: p->pi_lock
	 */
	શૂન્ય (*task_fork)(સંરચના task_struct *p);
	/*
	 * finish_task_switch: no locks
	 */
	શૂન્ય (*task_dead)(સંરચના task_struct *p);

	/*
	 * sched_change
	 */
	શૂન્ય (*switching_from)(સંરચના rq *this_rq, સંરચના task_struct *task);
	શૂન્ય (*switched_from) (સંરચના rq *this_rq, સંરચના task_struct *task);
	શૂન્ય (*switching_to)  (સંરચના rq *this_rq, સંરચના task_struct *task);
	શૂન્ય (*switched_to)   (સંરચના rq *this_rq, સંરચના task_struct *task);
	u64  (*get_prio)     (સંરચના rq *this_rq, સંરચના task_struct *task);
	શૂન્ય (*prio_changed) (સંરચના rq *this_rq, સંરચના task_struct *task,
			      u64 oldprio);

	/*
	 * set_load_weight: task_rq_lock + sched_change
	 * __setscheduler_parms: task_rq_lock + sched_change
	 */
	શૂન્ય (*reweight_task)(સંરચના rq *this_rq, સંરચના task_struct *task,
			      અચલ સંરચના load_weight *lw);

	/*
	 * sched_rr_get_interval: task_rq_lock
	 */
	unsigned પૂર્ણાંક (*get_rr_interval)(સંરચના rq *rq,
					સંરચના task_struct *task);

	/*
	 * task_sched_runtime: task_rq_lock
	 */
	શૂન્ય (*update_curr)(સંરચના rq *rq);

#ifdef CONFIG_FAIR_GROUP_SCHED
	/*
	 * sched_change_group: task_rq_lock + sched_change
	 */
	શૂન્ય (*task_change_group)(સંરચના task_struct *p);
#endif

#ifdef CONFIG_SCHED_CORE
	/*
	 * pick_next_task: rq->lock
	 * try_steal_cookie: rq->lock (double)
	 */
	પૂર્ણાંક (*task_is_throttled)(સંરચના task_struct *p, પૂર્ણાંક cpu);
#endif
};

સ્થિર અંતઃરેખ શૂન્ય put_prev_task(સંરચના rq *rq, સંરચના task_struct *prev)
{
	WARN_ON_ONCE(rq->donor != prev);
	prev->sched_class->put_prev_task(rq, prev, ખાલી);
}

સ્થિર અંતઃરેખ શૂન્ય set_next_task(સંરચના rq *rq, સંરચના task_struct *next)
{
	next->sched_class->set_next_task(rq, next, SNT_NORMAL);
}

સ્થિર અંતઃરેખ શૂન્ય
__put_prev_set_next_dl_server(સંરચના rq *rq,
			      સંરચના task_struct *prev,
			      સંરચના task_struct *next)
{
	prev->dl_server = ખાલી;
	next->dl_server = rq->dl_server;
	rq->dl_server = ખાલી;
}

સ્થિર અંતઃરેખ શૂન્ય put_prev_set_next_task(સંરચના rq *rq,
					  સંરચના task_struct *prev,
					  સંરચના task_struct *next)
{
	WARN_ON_ONCE(rq->donor != prev);

	__put_prev_set_next_dl_server(rq, prev, next);

	જો (next == prev) {
		next->sched_class->set_next_task(rq, next, SNT_REPICK);
		પાછા;
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
અચલ સંરચના sched_class name##_sched_class \
	__aligned(__alignof__(સંરચના sched_class)) \
	__section("__" #name "_sched_class")

/* Defined in include/asm-generic/vmlinux.lds.h */
બાહ્ય સંરચના sched_class __sched_class_highest[];
બાહ્ય સંરચના sched_class __sched_class_lowest[];

બાહ્ય અચલ સંરચના sched_class stop_sched_class;
બાહ્ય અચલ સંરચના sched_class dl_sched_class;
બાહ્ય અચલ સંરચના sched_class rt_sched_class;
બાહ્ય અચલ સંરચના sched_class fair_sched_class;
બાહ્ય અચલ સંરચના sched_class idle_sched_class;

/*
 * Iterate only active classes. SCX can take over all fair tasks or be
 * completely disabled. If the former, skip fair. If the latter, skip SCX.
 */
સ્થિર અંતઃરેખ અચલ સંરચના sched_class *next_active_class(અચલ સંરચના sched_class *class)
{
	class++;
#ifdef CONFIG_SCHED_CLASS_EXT
	જો (scx_switched_all() && class == &fair_sched_class)
		class++;
	જો (!scx_enabled() && class == &ext_sched_class)
		class++;
#endif
	પાછા class;
}

#define for_class_range(class, _from, _to) \
	માટે (class = (_from); class < (_to); class++)

#define for_each_class(class) \
	for_class_range(class, __sched_class_highest, __sched_class_lowest)

#define for_active_class_range(class, _from, _to)				\
	માટે (class = (_from); class != (_to); class = next_active_class(class))

#define for_each_active_class(class)						\
	for_active_class_range(class, __sched_class_highest, __sched_class_lowest)

#define sched_class_above(_a, _b)	((_a) < (_b))

સ્થિર અંતઃરેખ શૂન્ય rq_modified_begin(સંરચના rq *rq, અચલ સંરચના sched_class *class)
{
	જો (sched_class_above(rq->next_class, class))
		rq->next_class = class;
}

સ્થિર અંતઃરેખ bool rq_modified_above(સંરચના rq *rq, અચલ સંરચના sched_class *class)
{
	પાછા sched_class_above(rq->next_class, class);
}

સ્થિર અંતઃરેખ bool sched_stop_runnable(સંરચના rq *rq)
{
	પાછા rq->stop && task_on_rq_queued(rq->stop);
}

સ્થિર અંતઃરેખ bool sched_dl_runnable(સંરચના rq *rq)
{
	પાછા rq->dl.dl_nr_running > 0;
}

સ્થિર અંતઃરેખ bool sched_rt_runnable(સંરચના rq *rq)
{
	પાછા rq->rt.rt_queued > 0;
}

સ્થિર અંતઃરેખ bool sched_fair_runnable(સંરચના rq *rq)
{
	પાછા rq->cfs.nr_queued > 0;
}

બાહ્ય સંરચના task_struct *pick_task_fair(સંરચના rq *rq, સંરચના rq_flags *rf);
બાહ્ય સંરચના task_struct *pick_task_idle(સંરચના rq *rq, સંરચના rq_flags *rf);

#define SCA_CHECK		0x01
#define SCA_MIGRATE_DISABLE	0x02
#define SCA_MIGRATE_ENABLE	0x04
#define SCA_USER		0x08

બાહ્ય શૂન્ય update_group_capacity(સંરચના sched_domain *sd, પૂર્ણાંક cpu);

બાહ્ય શૂન્ય sched_balance_trigger(સંરચના rq *rq);

બાહ્ય પૂર્ણાંક __set_cpus_allowed_ptr(સંરચના task_struct *p, સંરચના affinity_context *ctx);
બાહ્ય શૂન્ય set_cpus_allowed_common(સંરચના task_struct *p, સંરચના affinity_context *ctx);

સ્થિર અંતઃરેખ bool task_allowed_on_cpu(સંરચના task_struct *p, પૂર્ણાંક cpu)
{
	/* When not in the task's cpumask, no point in looking further. */
	જો (!cpumask_test_cpu(cpu, p->cpus_ptr))
		પાછા false;

	/* Can @cpu run a user thread? */
	જો (!(p->flags & PF_KTHREAD) && !task_cpu_possible(cpu, p))
		પાછા false;

	પાછા true;
}

સ્થિર અંતઃરેખ cpumask_t *alloc_user_cpus_ptr(પૂર્ણાંક ગાંઠ)
{
	/*
	 * See set_cpus_allowed_force() above for the rcu_head usage.
	 */
	પૂર્ણાંક size = max_t(પૂર્ણાંક, cpumask_size(), કદ(સંરચના rcu_head));

	પાછા kmalloc_ગાંઠ(size, GFP_KERNEL, ગાંઠ);
}

સ્થિર અંતઃરેખ સંરચના task_struct *get_ધકેલો_task(સંરચના rq *rq)
{
	સંરચના task_struct *p = rq->donor;

	lockdep_assert_rq_held(rq);

	જો (rq->ધકેલો_busy)
		પાછા ખાલી;

	જો (p->nr_cpus_allowed == 1)
		પાછા ખાલી;

	જો (p->migration_disabled)
		પાછા ખાલી;

	rq->ધકેલો_busy = true;
	પાછા get_task_struct(p);
}

બાહ્ય પૂર્ણાંક ધકેલો_cpu_stop(શૂન્ય *arg);

#ifdef CONFIG_CPU_IDLE

સ્થિર અંતઃરેખ શૂન્ય idle_set_state(સંરચના rq *rq,
				  સંરચના cpuidle_state *idle_state)
{
	rq->idle_state = idle_state;
}

સ્થિર અંતઃરેખ સંરચના cpuidle_state *idle_get_state(સંરચના rq *rq)
{
	lockdep_assert(rcu_read_lock_any_held());

	પાછા rq->idle_state;
}

#નહીંતર /* !CONFIG_CPU_IDLE: */

સ્થિર અંતઃરેખ શૂન્ય idle_set_state(સંરચના rq *rq,
				  સંરચના cpuidle_state *idle_state)
{
}

સ્થિર અંતઃરેખ સંરચના cpuidle_state *idle_get_state(સંરચના rq *rq)
{
	પાછા ખાલી;
}

#endif /* !CONFIG_CPU_IDLE */

બાહ્ય શૂન્ય schedule_idle(શૂન્ય);
asmlinkage શૂન્ય schedule_user(શૂન્ય);

બાહ્ય શૂન્ય sysrq_sched_debug_show(શૂન્ય);
બાહ્ય શૂન્ય sched_init_granularity(શૂન્ય);
બાહ્ય શૂન્ય update_max_interval(શૂન્ય);

બાહ્ય શૂન્ય init_sched_dl_class(શૂન્ય);
બાહ્ય શૂન્ય init_sched_rt_class(શૂન્ય);
બાહ્ય શૂન્ય init_sched_fair_class(શૂન્ય);

બાહ્ય શૂન્ય resched_curr(સંરચના rq *rq);
બાહ્ય શૂન્ય resched_curr_lazy(સંરચના rq *rq);
બાહ્ય શૂન્ય resched_cpu(પૂર્ણાંક cpu);

બાહ્ય શૂન્ય init_rt_bandwidth(સંરચના rt_bandwidth *rt_b, u64 period, u64 runtime);
બાહ્ય bool sched_rt_bandwidth_account(સંરચના rt_rq *rt_rq);

બાહ્ય શૂન્ય init_dl_entity(સંરચના sched_dl_entity *dl_se);

બાહ્ય શૂન્ય init_cfs_throttle_work(સંરચના task_struct *p);

#define BW_SHIFT		20
#define BW_UNIT			(1 << BW_SHIFT)
#define RATIO_SHIFT		8
#define MAX_BW_BITS		(64 - BW_SHIFT)
#define MAX_BW			((1ULL << MAX_BW_BITS) - 1)

બાહ્ય u64 to_ratio(u64 period, u64 runtime);

બાહ્ય શૂન્ય init_entity_runnable_average(સંરચના sched_entity *se);
બાહ્ય શૂન્ય post_init_entity_util_avg(સંરચના task_struct *p);

#ifdef CONFIG_NO_HZ_FULL
બાહ્ય bool sched_can_stop_tick(સંરચના rq *rq);
બાહ્ય પૂર્ણાંક __init sched_tick_offload_init(શૂન્ય);

/*
 * Tick may be needed by tasks in the runqueue depending on their policy and
 * requirements. If tick is needed, lets send the target an IPI to kick it out of
 * nohz mode if necessary.
 */
સ્થિર અંતઃરેખ શૂન્ય sched_update_tick_dependency(સંરચના rq *rq)
{
	પૂર્ણાંક cpu = cpu_of(rq);

	જો (!tick_nohz_full_cpu(cpu))
		પાછા;

	જો (sched_can_stop_tick(rq))
		tick_nohz_dep_clear_cpu(cpu, TICK_DEP_BIT_SCHED);
	નહીંતર
		tick_nohz_dep_set_cpu(cpu, TICK_DEP_BIT_SCHED);
}
#નહીંતર /* !CONFIG_NO_HZ_FULL: */
સ્થિર અંતઃરેખ પૂર્ણાંક sched_tick_offload_init(શૂન્ય) { પાછા 0; }
સ્થિર અંતઃરેખ શૂન્ય sched_update_tick_dependency(સંરચના rq *rq) { }
#endif /* !CONFIG_NO_HZ_FULL */

સ્થિર અંતઃરેખ શૂન્ય add_nr_running(સંરચના rq *rq, unsigned ગણતરી)
{
	unsigned prev_nr = rq->nr_running;

	rq->nr_running = prev_nr + ગણતરી;
	જો (trace_sched_update_nr_running_tp_enabled()) {
		call_trace_sched_update_nr_running(rq, ગણતરી);
	}

	જો (prev_nr < 2 && rq->nr_running >= 2)
		set_rd_overloaded(rq->rd, 1);

	sched_update_tick_dependency(rq);
}

સ્થિર અંતઃરેખ શૂન્ય sub_nr_running(સંરચના rq *rq, unsigned ગણતરી)
{
	rq->nr_running -= ગણતરી;
	જો (trace_sched_update_nr_running_tp_enabled()) {
		call_trace_sched_update_nr_running(rq, -ગણતરી);
	}

	/* Check if we still need preemption */
	sched_update_tick_dependency(rq);
}

સ્થિર અંતઃરેખ શૂન્ય __block_task(સંરચના rq *rq, સંરચના task_struct *p)
{
	જો (p->sched_contributes_to_load)
		rq->nr_uninterruptible++;

	જો (p->in_iowait) {
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

બાહ્ય શૂન્ય activate_task(સંરચના rq *rq, સંરચના task_struct *p, પૂર્ણાંક flags);
બાહ્ય શૂન્ય deactivate_task(સંરચના rq *rq, સંરચના task_struct *p, પૂર્ણાંક flags);

બાહ્ય શૂન્ય wakeup_preempt(સંરચના rq *rq, સંરચના task_struct *p, પૂર્ણાંક flags);

/*
 * attach_task() -- attach the task detached by detach_task() to its new rq.
 */
સ્થિર અંતઃરેખ શૂન્ય attach_task(સંરચના rq *rq, સંરચના task_struct *p)
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
સ્થિર અંતઃરેખ શૂન્ય attach_one_task(સંરચના rq *rq, સંરચના task_struct *p)
{
	guard(rq_lock)(rq);
	update_rq_clock(rq);
	attach_task(rq, p);
}

/*
 * __attach_tasks() - attaches a list of tasks (using se.group_node) to
 * the new rq
 */
સ્થિર અંતઃરેખ શૂન્ય __attach_tasks(સંરચના rq *rq, સંરચના list_head *tasks)
{
	guard(rq_lock)(rq);
	update_rq_clock(rq);

	જ્યારે (!list_empty(tasks)) {
		સંરચના task_struct *p;

		p = list_first_entry(tasks, સંરચના task_struct, se.group_ગાંઠ);
		list_del_init(&p->se.group_ગાંઠ);

		attach_task(rq, p);
	}
}

#ifdef CONFIG_PREEMPT_RT
# define SCHED_NR_MIGRATE_BREAK 8
#નહીંતર
# define SCHED_NR_MIGRATE_BREAK 32
#endif

બાહ્ય __read_mostly unsigned પૂર્ણાંક sysctl_sched_nr_migrate;
બાહ્ય __read_mostly unsigned પૂર્ણાંક sysctl_sched_migration_cost;

બાહ્ય unsigned પૂર્ણાંક sysctl_sched_base_slice;

બાહ્ય પૂર્ણાંક sysctl_resched_latency_warn_ms;
બાહ્ય પૂર્ણાંક sysctl_resched_latency_warn_once;

બાહ્ય unsigned પૂર્ણાંક sysctl_sched_tunable_scaling;

બાહ્ય unsigned પૂર્ણાંક sysctl_numa_balancing_scan_delay;
બાહ્ય unsigned પૂર્ણાંક sysctl_numa_balancing_scan_period_min;
બાહ્ય unsigned પૂર્ણાંક sysctl_numa_balancing_scan_period_max;
બાહ્ય unsigned પૂર્ણાંક sysctl_numa_balancing_scan_size;
બાહ્ય unsigned પૂર્ણાંક sysctl_numa_balancing_hot_threshold;

#ifdef CONFIG_SCHED_HRTICK

/*
 * Use hrtick when:
 *  - enabled by features
 *  - hrtimer is actually high res
 */
સ્થિર અંતઃરેખ bool hrtick_enabled(સંરચના rq *rq)
{
	પાછા cpu_active(cpu_of(rq)) && hrtimer_highres_enabled();
}

સ્થિર અંતઃરેખ bool hrtick_enabled_fair(સંરચના rq *rq)
{
	પાછા sched_feat(HRTICK) && hrtick_enabled(rq);
}

સ્થિર અંતઃરેખ bool hrtick_enabled_dl(સંરચના rq *rq)
{
	પાછા sched_feat(HRTICK_DL) && hrtick_enabled(rq);
}

બાહ્ય શૂન્ય hrtick_start(સંરચના rq *rq, u64 delay);
સ્થિર અંતઃરેખ bool hrtick_active(સંરચના rq *rq)
{
	પાછા hrtimer_active(&rq->hrtick_timer);
}

#નહીંતર /* !CONFIG_SCHED_HRTICK: */
સ્થિર અંતઃરેખ bool hrtick_enabled_fair(સંરચના rq *rq) { પાછા false; }
સ્થિર અંતઃરેખ bool hrtick_enabled_dl(સંરચના rq *rq) { પાછા false; }
સ્થિર અંતઃરેખ bool hrtick_enabled(સંરચના rq *rq) { પાછા false; }
#endif /* !CONFIG_SCHED_HRTICK */

#ifndef arch_scale_freq_tick
સ્થિર __always_inline શૂન્ય arch_scale_freq_tick(શૂન્ય) { }
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
સ્થિર __always_inline
unsigned long arch_scale_freq_capacity(પૂર્ણાંક cpu)
{
	પાછા SCHED_CAPACITY_SCALE;
}
#endif

/*
 * In double_lock_balance()/double_rq_lock(), we use raw_spin_rq_lock() to
 * acquire rq lock instead of rq_lock(). So at the end of these two functions
 * we need to call double_rq_clock_clear_update() to clear RQCF_UPDATED of
 * rq->clock_update_flags to avoid the WARN_DOUBLE_CLOCK warning.
 */
સ્થિર અંતઃરેખ શૂન્ય double_rq_clock_clear_update(સંરચના rq *rq1, સંરચના rq *rq2)
{
	rq1->clock_update_flags &= (RQCF_REQ_SKIP|RQCF_ACT_SKIP);
	rq2->clock_update_flags &= (RQCF_REQ_SKIP|RQCF_ACT_SKIP);
}

#define DEFINE_LOCK_GUARD_2(name, type, _lock, _unlock, ...)				\
__DEFINE_UNLOCK_GUARD(name, type, _unlock, type *lock2; __VA_ARGS__)			\
સ્થિર અંતઃરેખ class_##name##_t class_##name##_constructor(type *lock, type *lock2)	\
	__no_context_analysis								\
{ class_##name##_t _t = { .lock = lock, .lock2 = lock2 }, *_T = &_t;			\
  _lock; પાછા _t; }
#define DECLARE_LOCK_GUARD_2_ATTRS(_name, _lock, _unlock1, _unlock2)			\
સ્થિર અંતઃરેખ class_##_name##_t class_##_name##_constructor(lock_##_name##_t *_T1,	\
							    lock_##_name##_t *_T2) _lock; \
સ્થિર __always_inline શૂન્ય __class_##_name##_cleanup_ctx1(class_##_name##_t **_T1)	\
	__no_context_analysis _unlock1 { }						\
સ્થિર __always_inline શૂન્ય __class_##_name##_cleanup_ctx2(class_##_name##_t **_T2)	\
	__no_context_analysis _unlock2 { }
#define WITH_LOCK_GUARD_2_ATTRS(_name, _T1, _T2)					\
	class_##_name##_constructor(_T1, _T2),						\
	*__UNIQUE_ID(unlock1) __cleanup(__class_##_name##_cleanup_ctx1) = (શૂન્ય *)(_T1),\
	*__UNIQUE_ID(unlock2) __cleanup(__class_##_name##_cleanup_ctx2) = (શૂન્ય *)(_T2)

સ્થિર અંતઃરેખ bool rq_order_less(સંરચના rq *rq1, સંરચના rq *rq2)
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
	જો (rq1->core->cpu < rq2->core->cpu)
		પાછા true;
	જો (rq1->core->cpu > rq2->core->cpu)
		પાછા false;

	/*
	 * __sched_core_flip() relies on SMT having cpu-id lock order.
	 */
#endif /* CONFIG_SCHED_CORE */
	પાછા rq1->cpu < rq2->cpu;
}

બાહ્ય શૂન્ય double_rq_lock(સંરચના rq *rq1, સંરચના rq *rq2)
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
સ્થિર અંતઃરેખ પૂર્ણાંક _double_lock_balance(સંરચના rq *this_rq, સંરચના rq *busiest)
	__must_hold(__rq_lockp(this_rq))
	__acquires(__rq_lockp(busiest))
{
	raw_spin_rq_unlock(this_rq);
	double_rq_lock(this_rq, busiest);

	પાછા 1;
}

#નહીંતર /* !CONFIG_PREEMPTION: */
/*
 * Unfair double_lock_balance: Optimizes throughput at the expense of
 * latency by eliminating extra atomic operations when the locks are
 * already in proper order on entry.  This favors lower CPU-ids and will
 * grant the double lock to lower CPUs over higher ids under contention,
 * regardless of entry order into the function.
 */
સ્થિર અંતઃરેખ પૂર્ણાંક _double_lock_balance(સંરચના rq *this_rq, સંરચના rq *busiest)
	__must_hold(__rq_lockp(this_rq))
	__acquires(__rq_lockp(busiest))
{
	જો (__rq_lockp(this_rq) == __rq_lockp(busiest)) {
		__acquire(__rq_lockp(busiest)); /* already held */
		double_rq_clock_clear_update(this_rq, busiest);
		પાછા 0;
	}

	જો (likely(raw_spin_rq_trylock(busiest))) {
		double_rq_clock_clear_update(this_rq, busiest);
		પાછા 0;
	}

	જો (rq_order_less(this_rq, busiest)) {
		raw_spin_rq_lock_nested(busiest, SINGLE_DEPTH_NESTING);
		double_rq_clock_clear_update(this_rq, busiest);
		પાછા 0;
	}

	raw_spin_rq_unlock(this_rq);
	double_rq_lock(this_rq, busiest);

	પાછા 1;
}

#endif /* !CONFIG_PREEMPTION */

/*
 * double_lock_balance - lock the busiest runqueue, this_rq is locked already.
 */
સ્થિર અંતઃરેખ પૂર્ણાંક double_lock_balance(સંરચના rq *this_rq, સંરચના rq *busiest)
	__must_hold(__rq_lockp(this_rq))
	__acquires(__rq_lockp(busiest))
{
	lockdep_assert_irqs_disabled();

	પાછા _double_lock_balance(this_rq, busiest);
}

સ્થિર અંતઃરેખ શૂન્ય double_unlock_balance(સંરચના rq *this_rq, સંરચના rq *busiest)
	__releases(__rq_lockp(busiest))
{
	જો (__rq_lockp(this_rq) != __rq_lockp(busiest))
		raw_spin_rq_unlock(busiest);
	નહીંતર
		__release(__rq_lockp(busiest)); /* fake release */
	lock_set_subclass(&__rq_lockp(this_rq)->dep_map, 0, _RET_IP_);
}

સ્થિર અંતઃરેખ શૂન્ય double_lock(spinlock_t *l1, spinlock_t *l2)
	__acquires(l1, l2)
{
	જો (l1 > l2)
		swap(l1, l2);

	spin_lock(l1);
	spin_lock_nested(l2, SINGLE_DEPTH_NESTING);
}

સ્થિર અંતઃરેખ શૂન્ય double_lock_irq(spinlock_t *l1, spinlock_t *l2)
	__acquires(l1, l2)
{
	જો (l1 > l2)
		swap(l1, l2);

	spin_lock_irq(l1);
	spin_lock_nested(l2, SINGLE_DEPTH_NESTING);
}

સ્થિર અંતઃરેખ શૂન્ય double_raw_lock(raw_spinlock_t *l1, raw_spinlock_t *l2)
	__acquires(l1, l2)
{
	જો (l1 > l2)
		swap(l1, l2);

	raw_spin_lock(l1);
	raw_spin_lock_nested(l2, SINGLE_DEPTH_NESTING);
}

સ્થિર અંતઃરેખ શૂન્ય double_raw_unlock(raw_spinlock_t *l1, raw_spinlock_t *l2)
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
સ્થિર અંતઃરેખ શૂન્ય double_rq_unlock(સંરચના rq *rq1, સંરચના rq *rq2)
	__releases(__rq_lockp(rq1), __rq_lockp(rq2))
{
	જો (__rq_lockp(rq1) != __rq_lockp(rq2))
		raw_spin_rq_unlock(rq2);
	નહીંતર
		__release(__rq_lockp(rq2)); /* fake release */
	raw_spin_rq_unlock(rq1);
}

બાહ્ય શૂન્ય set_rq_online (સંરચના rq *rq);
બાહ્ય શૂન્ય set_rq_offline(સંરચના rq *rq);

બાહ્ય bool sched_smp_initialized;

DEFINE_LOCK_GUARD_2(double_rq_lock, સંરચના rq,
		    double_rq_lock(_T->lock, _T->lock2),
		    double_rq_unlock(_T->lock, _T->lock2))

બાહ્ય સંરચના sched_entity *__pick_root_entity(સંરચના cfs_rq *cfs_rq);
બાહ્ય સંરચના sched_entity *__pick_first_entity(સંરચના cfs_rq *cfs_rq);
બાહ્ય સંરચના sched_entity *__pick_last_entity(સંરચના cfs_rq *cfs_rq);

બાહ્ય bool sched_debug_verbose;

બાહ્ય શૂન્ય print_cfs_stats(સંરચના seq_file *m, પૂર્ણાંક cpu);
બાહ્ય શૂન્ય print_rt_stats(સંરચના seq_file *m, પૂર્ણાંક cpu);
બાહ્ય શૂન્ય print_dl_stats(સંરચના seq_file *m, પૂર્ણાંક cpu);
બાહ્ય શૂન્ય print_cfs_rq(સંરચના seq_file *m, પૂર્ણાંક cpu, સંરચના cfs_rq *cfs_rq);
બાહ્ય શૂન્ય print_rt_rq(સંરચના seq_file *m, પૂર્ણાંક cpu, સંરચના rt_rq *rt_rq);
બાહ્ય શૂન્ય print_dl_rq(સંરચના seq_file *m, પૂર્ણાંક cpu, સંરચના dl_rq *dl_rq);

બાહ્ય શૂન્ય resched_latency_warn(પૂર્ણાંક cpu, u64 latency);

#ifdef CONFIG_NUMA_BALANCING
બાહ્ય શૂન્ય show_numa_stats(સંરચના task_struct *p, સંરચના seq_file *m);
બાહ્ય શૂન્ય
print_numa_stats(સંરચના seq_file *m, પૂર્ણાંક ગાંઠ, unsigned long tsf,
		 unsigned long tpf, unsigned long gsf, unsigned long gpf);
#endif /* CONFIG_NUMA_BALANCING */

બાહ્ય શૂન્ય init_cfs_rq(સંરચના cfs_rq *cfs_rq);
બાહ્ય શૂન્ય init_rt_rq(સંરચના rt_rq *rt_rq);
બાહ્ય શૂન્ય init_dl_rq(સંરચના dl_rq *dl_rq);

બાહ્ય શૂન્ય cfs_bandwidth_usage_inc(શૂન્ય);
બાહ્ય શૂન્ય cfs_bandwidth_usage_dec(શૂન્ય);

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

બાહ્ય શૂન્ય nohz_balance_exit_idle(સંરચના rq *rq);
#નહીંતર /* !CONFIG_NO_HZ_COMMON: */
સ્થિર અંતઃરેખ શૂન્ય nohz_balance_exit_idle(સંરચના rq *rq) { }
#endif /* !CONFIG_NO_HZ_COMMON */

#ifdef CONFIG_NO_HZ_COMMON
બાહ્ય શૂન્ય nohz_run_idle_balance(પૂર્ણાંક cpu);
#નહીંતર
સ્થિર અંતઃરેખ શૂન્ય nohz_run_idle_balance(પૂર્ણાંક cpu) { }
#endif

#અન્તર્ભાવ "stats.h"

#જો defined(CONFIG_SCHED_CORE) && defined(CONFIG_SCHEDSTATS)

બાહ્ય શૂન્ય __sched_core_account_forceidle(સંરચના rq *rq);

સ્થિર અંતઃરેખ શૂન્ય sched_core_account_forceidle(સંરચના rq *rq)
{
	જો (schedstat_enabled())
		__sched_core_account_forceidle(rq);
}

બાહ્ય શૂન્ય __sched_core_tick(સંરચના rq *rq);

સ્થિર અંતઃરેખ શૂન્ય sched_core_tick(સંરચના rq *rq)
{
	જો (sched_core_enabled(rq) && schedstat_enabled())
		__sched_core_tick(rq);
}

#નહીંતર /* !(CONFIG_SCHED_CORE && CONFIG_SCHEDSTATS): */

સ્થિર અંતઃરેખ શૂન્ય sched_core_account_forceidle(સંરચના rq *rq) { }

સ્થિર અંતઃરેખ શૂન્ય sched_core_tick(સંરચના rq *rq) { }

#endif /* !(CONFIG_SCHED_CORE && CONFIG_SCHEDSTATS) */

#ifdef CONFIG_IRQ_TIME_ACCOUNTING

સંરચના irqtime {
	u64			total;
	u64			tick_delta;
	u64			irq_start_time;
	સંરચના u64_stats_sync	sync;
};

DECLARE_PER_CPU(સંરચના irqtime, cpu_irqtime);
DECLARE_STATIC_ચાવી_FALSE(sched_clock_irqtime);

સ્થિર અંતઃરેખ પૂર્ણાંક irqtime_enabled(શૂન્ય)
{
	પાછા static_branch_likely(&sched_clock_irqtime);
}

/*
 * Returns the irqtime minus the softirq time computed by ksoftirqd.
 * Otherwise ksoftirqd's sum_exec_runtime is subtracted its own runtime
 * and never move forward.
 */
સ્થિર અંતઃરેખ u64 irq_time_read(પૂર્ણાંક cpu)
{
	સંરચના irqtime *irqtime = &per_cpu(cpu_irqtime, cpu);
	unsigned પૂર્ણાંક seq;
	u64 total;

	કરો {
		seq = __u64_stats_fetch_begin(&irqtime->sync);
		total = irqtime->total;
	} જ્યારે (__u64_stats_fetch_retry(&irqtime->sync, seq));

	પાછા total;
}

#નહીંતર /* !CONFIG_IRQ_TIME_ACCOUNTING: */

સ્થિર અંતઃરેખ પૂર્ણાંક irqtime_enabled(શૂન્ય)
{
	પાછા 0;
}

#endif /* !CONFIG_IRQ_TIME_ACCOUNTING */

#ifdef CONFIG_CPU_FREQ

DECLARE_PER_CPU(સંરચના update_util_data __rcu *, cpufreq_update_util_data);

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
સ્થિર અંતઃરેખ શૂન્ય cpufreq_update_util(સંરચના rq *rq, unsigned પૂર્ણાંક flags)
{
	સંરચના update_util_data *data;

	data = rcu_dereference_sched(*per_cpu_ptr(&cpufreq_update_util_data,
						  cpu_of(rq)));
	જો (data)
		data->func(data, rq_clock(rq), flags);
}
#નહીંતર /* !CONFIG_CPU_FREQ: */
સ્થિર અંતઃરેખ શૂન્ય cpufreq_update_util(સંરચના rq *rq, unsigned પૂર્ણાંક flags) { }
#endif /* !CONFIG_CPU_FREQ */

#ifdef arch_scale_freq_capacity
# ifndef arch_scale_freq_invariant
#  define arch_scale_freq_invariant()	true
# endif
#નહીંતર
# define arch_scale_freq_invariant()	false
#endif

unsigned long effective_cpu_util(પૂર્ણાંક cpu, unsigned long util_cfs,
				 unsigned long *min,
				 unsigned long *max);

unsigned long sugov_effective_cpu_perf(પૂર્ણાંક cpu, unsigned long actual,
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
સ્થિર અંતઃરેખ bool dl_task_fits_capacity(સંરચના task_struct *p, પૂર્ણાંક cpu)
{
	unsigned long cap = arch_scale_cpu_capacity(cpu);

	પાછા cap >= p->dl.dl_density >> (BW_SHIFT - SCHED_CAPACITY_SHIFT);
}

સ્થિર અંતઃરેખ unsigned long cpu_bw_dl(સંરચના rq *rq)
{
	પાછા (rq->dl.running_bw * SCHED_CAPACITY_SCALE) >> BW_SHIFT;
}

સ્થિર અંતઃરેખ unsigned long cpu_util_dl(સંરચના rq *rq)
{
	પાછા READ_ONCE(rq->avg_dl.util_avg);
}


બાહ્ય unsigned long cpu_util_cfs(પૂર્ણાંક cpu);
બાહ્ય unsigned long cpu_util_cfs_boost(પૂર્ણાંક cpu);

સ્થિર અંતઃરેખ unsigned long cpu_util_rt(સંરચના rq *rq)
{
	પાછા READ_ONCE(rq->avg_rt.util_avg);
}

#ifdef CONFIG_UCLAMP_TASK

unsigned long uclamp_eff_કિંમત(સંરચના task_struct *p, ગણના uclamp_id clamp_id);

/*
 * When uclamp is compiled in, the aggregation at rq level is 'turned off'
 * by default in the fast path and only gets turned on once userspace performs
 * an operation that requires it.
 *
 * Returns true if userspace opted-in to use uclamp and aggregation at rq level
 * hence is active.
 */
સ્થિર અંતઃરેખ bool uclamp_is_used(શૂન્ય)
{
	પાછા static_branch_likely(&sched_uclamp_used);
}

/*
 * Enabling static branches would get the cpus_read_lock(),
 * check whether uclamp_is_used before enable it to avoid always
 * calling cpus_read_lock(). Because we never disable this
 * static key once enable it.
 */
સ્થિર અંતઃરેખ શૂન્ય sched_uclamp_enable(શૂન્ય)
{
	જો (!uclamp_is_used())
		static_branch_enable(&sched_uclamp_used);
}

સ્થિર અંતઃરેખ unsigned long uclamp_rq_get(સંરચના rq *rq,
					  ગણના uclamp_id clamp_id)
{
	પાછા READ_ONCE(rq->uclamp[clamp_id].કિંમત);
}

સ્થિર અંતઃરેખ શૂન્ય uclamp_rq_set(સંરચના rq *rq, ગણના uclamp_id clamp_id,
				 unsigned પૂર્ણાંક કિંમત)
{
	WRITE_ONCE(rq->uclamp[clamp_id].કિંમત, કિંમત);
}

સ્થિર અંતઃરેખ bool uclamp_rq_is_idle(સંરચના rq *rq)
{
	પાછા rq->uclamp_flags & UCLAMP_FLAG_IDLE;
}

/* Is the rq being capped/throttled by uclamp_max? */
સ્થિર અંતઃરેખ bool uclamp_rq_is_capped(સંરચના rq *rq)
{
	unsigned long rq_util;
	unsigned long max_util;

	જો (!uclamp_is_used())
		પાછા false;

	rq_util = cpu_util_cfs(cpu_of(rq)) + cpu_util_rt(rq);
	max_util = READ_ONCE(rq->uclamp[UCLAMP_MAX].કિંમત);

	પાછા max_util != SCHED_CAPACITY_SCALE && rq_util >= max_util;
}

#define for_each_clamp_id(clamp_id) \
	માટે ((clamp_id) = 0; (clamp_id) < UCLAMP_CNT; (clamp_id)++)

બાહ્ય unsigned પૂર્ણાંક sysctl_sched_uclamp_util_min_rt_default;


સ્થિર અંતઃરેખ unsigned પૂર્ણાંક uclamp_none(ગણના uclamp_id clamp_id)
{
	જો (clamp_id == UCLAMP_MIN)
		પાછા 0;
	પાછા SCHED_CAPACITY_SCALE;
}

/* Integer rounded range for each bucket */
#define UCLAMP_BUCKET_DELTA DIV_ROUND_CLOSEST(SCHED_CAPACITY_SCALE, UCLAMP_BUCKETS)

સ્થિર અંતઃરેખ unsigned પૂર્ણાંક uclamp_bucket_id(unsigned પૂર્ણાંક clamp_કિંમત)
{
	પાછા min_t(unsigned પૂર્ણાંક, clamp_કિંમત / UCLAMP_BUCKET_DELTA, UCLAMP_BUCKETS - 1);
}

સ્થિર અંતઃરેખ શૂન્ય
uclamp_se_set(સંરચના uclamp_se *uc_se, unsigned પૂર્ણાંક કિંમત, bool user_defined)
{
	uc_se->કિંમત = કિંમત;
	uc_se->bucket_id = uclamp_bucket_id(કિંમત);
	uc_se->user_defined = user_defined;
}

#નહીંતર /* !CONFIG_UCLAMP_TASK: */

સ્થિર અંતઃરેખ unsigned long
uclamp_eff_કિંમત(સંરચના task_struct *p, ગણના uclamp_id clamp_id)
{
	જો (clamp_id == UCLAMP_MIN)
		પાછા 0;

	પાછા SCHED_CAPACITY_SCALE;
}

સ્થિર અંતઃરેખ bool uclamp_rq_is_capped(સંરચના rq *rq) { પાછા false; }

સ્થિર અંતઃરેખ bool uclamp_is_used(શૂન્ય)
{
	પાછા false;
}

સ્થિર અંતઃરેખ શૂન્ય sched_uclamp_enable(શૂન્ય) {}

સ્થિર અંતઃરેખ unsigned long
uclamp_rq_get(સંરચના rq *rq, ગણના uclamp_id clamp_id)
{
	જો (clamp_id == UCLAMP_MIN)
		પાછા 0;

	પાછા SCHED_CAPACITY_SCALE;
}

સ્થિર અંતઃરેખ શૂન્ય
uclamp_rq_set(સંરચના rq *rq, ગણના uclamp_id clamp_id, unsigned પૂર્ણાંક કિંમત)
{
}

સ્થિર અંતઃરેખ bool uclamp_rq_is_idle(સંરચના rq *rq)
{
	પાછા false;
}

#endif /* !CONFIG_UCLAMP_TASK */

#ifdef CONFIG_HAVE_SCHED_AVG_IRQ

સ્થિર અંતઃરેખ unsigned long cpu_util_irq(સંરચના rq *rq)
{
	પાછા READ_ONCE(rq->avg_irq.util_avg);
}

સ્થિર અંતઃરેખ
unsigned long scale_irq_capacity(unsigned long util, unsigned long irq, unsigned long max)
{
	util *= (max - irq);
	util /= max;

	પાછા util;

}

#નહીંતર /* !CONFIG_HAVE_SCHED_AVG_IRQ: */

સ્થિર અંતઃરેખ unsigned long cpu_util_irq(સંરચના rq *rq)
{
	પાછા 0;
}

સ્થિર અંતઃરેખ
unsigned long scale_irq_capacity(unsigned long util, unsigned long irq, unsigned long max)
{
	પાછા util;
}

#endif /* !CONFIG_HAVE_SCHED_AVG_IRQ */

બાહ્ય શૂન્ય __setparam_fair(સંરચના task_struct *p, અચલ સંરચના sched_attr *attr);

#જો defined(CONFIG_ENERGY_MODEL) && defined(CONFIG_CPU_FREQ_GOV_SCHEDUTIL)

#define perf_domain_span(pd) (to_cpumask(((pd)->em_pd->cpus)))

DECLARE_STATIC_ચાવી_FALSE(sched_energy_present);

સ્થિર અંતઃરેખ bool sched_energy_enabled(શૂન્ય)
{
	પાછા static_branch_unlikely(&sched_energy_present);
}

#નહીંતર /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL): */

#define perf_domain_span(pd) ખાલી

સ્થિર અંતઃરેખ bool sched_energy_enabled(શૂન્ય) { પાછા false; }

#endif /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL) */

#ifdef CONFIG_MEMBARRIER

/*
 * The scheduler provides memory barriers required by membarrier between:
 * - prior user-space memory accesses and store to rq->membarrier_state,
 * - store to rq->membarrier_state and following user-space memory accesses.
 * In the same way it provides those guarantees around store to rq->curr.
 */
સ્થિર અંતઃરેખ શૂન્ય membarrier_switch_mm(સંરચના rq *rq,
					સંરચના mm_struct *prev_mm,
					સંરચના mm_struct *next_mm)
{
	પૂર્ણાંક membarrier_state;

	જો (prev_mm == next_mm)
		પાછા;

	membarrier_state = atomic_read(&next_mm->membarrier_state);
	જો (READ_ONCE(rq->membarrier_state) == membarrier_state)
		પાછા;

	WRITE_ONCE(rq->membarrier_state, membarrier_state);
}

#નહીંતર /* !CONFIG_MEMBARRIER: */

સ્થિર અંતઃરેખ શૂન્ય membarrier_switch_mm(સંરચના rq *rq,
					સંરચના mm_struct *prev_mm,
					સંરચના mm_struct *next_mm)
{
}

#endif /* !CONFIG_MEMBARRIER */

સ્થિર અંતઃરેખ bool is_per_cpu_kthread(સંરચના task_struct *p)
{
	જો (!(p->flags & PF_KTHREAD))
		પાછા false;

	જો (p->nr_cpus_allowed != 1)
		પાછા false;

	પાછા true;
}

બાહ્ય શૂન્ય swake_up_all_locked(સંરચના swait_queue_head *q);
બાહ્ય શૂન્ય __prepare_to_swait(સંરચના swait_queue_head *q, સંરચના swait_queue *wait);

બાહ્ય પૂર્ણાંક try_to_wake_up(સંરચના task_struct *tsk, unsigned પૂર્ણાંક state, પૂર્ણાંક wake_flags);

#ifdef CONFIG_PREEMPT_DYNAMIC
બાહ્ય પૂર્ણાંક preempt_dynamic_mode;
બાહ્ય પૂર્ણાંક sched_dynamic_mode(અચલ અક્ષર *str);
બાહ્ય શૂન્ય sched_dynamic_update(પૂર્ણાંક mode);
#endif
બાહ્ય અચલ અક્ષર *preempt_modes[];

#ifdef CONFIG_SCHED_MM_CID

સ્થિર __always_inline bool cid_on_cpu(unsigned પૂર્ણાંક cid)
{
	પાછા cid & MM_CID_ONCPU;
}

સ્થિર __always_inline bool cid_in_transit(unsigned પૂર્ણાંક cid)
{
	પાછા cid & MM_CID_TRANSIT;
}

સ્થિર __always_inline unsigned પૂર્ણાંક cpu_cid_to_cid(unsigned પૂર્ણાંક cid)
{
	પાછા cid & ~MM_CID_ONCPU;
}

સ્થિર __always_inline unsigned પૂર્ણાંક cid_to_cpu_cid(unsigned પૂર્ણાંક cid)
{
	પાછા cid | MM_CID_ONCPU;
}

સ્થિર __always_inline unsigned પૂર્ણાંક cid_to_transit_cid(unsigned પૂર્ણાંક cid)
{
	પાછા cid | MM_CID_TRANSIT;
}

સ્થિર __always_inline unsigned પૂર્ણાંક cid_from_transit_cid(unsigned પૂર્ણાંક cid)
{
	પાછા cid & ~MM_CID_TRANSIT;
}

સ્થિર __always_inline bool cid_on_task(unsigned પૂર્ણાંક cid)
{
	/* True if none of the MM_CID_ONCPU, MM_CID_TRANSIT, MM_CID_UNSET bits is set */
	પાછા cid < MM_CID_TRANSIT;
}

સ્થિર __always_inline શૂન્ય mm_drop_cid(સંરચના mm_struct *mm, unsigned પૂર્ણાંક cid)
{
	clear_bit(cid, mm_cidmask(mm));
}

સ્થિર __always_inline શૂન્ય mm_unset_cid_on_task(સંરચના task_struct *t)
{
	unsigned પૂર્ણાંક cid = t->mm_cid.cid;

	t->mm_cid.cid = MM_CID_UNSET;
	જો (cid_on_task(cid))
		mm_drop_cid(t->mm, cid);
}

સ્થિર __always_inline શૂન્ય mm_drop_cid_on_cpu(સંરચના mm_struct *mm, સંરચના mm_cid_pcpu *pcp)
{
	/* Clear the ONCPU bit, but do not set UNSET in the per CPU storage */
	જો (cid_on_cpu(pcp->cid)) {
		pcp->cid = cpu_cid_to_cid(pcp->cid);
		mm_drop_cid(mm, pcp->cid);
	}
}

સ્થિર અંતઃરેખ unsigned પૂર્ણાંક __mm_get_cid(સંરચના mm_struct *mm, unsigned પૂર્ણાંક max_cids)
{
	unsigned પૂર્ણાંક cid = find_first_zero_bit(mm_cidmask(mm), max_cids);

	જો (cid >= max_cids)
		પાછા MM_CID_UNSET;
	જો (test_and_set_bit(cid, mm_cidmask(mm)))
		પાછા MM_CID_UNSET;
	પાછા cid;
}

સ્થિર અંતઃરેખ unsigned પૂર્ણાંક mm_get_cid(સંરચના mm_struct *mm)
{
	unsigned પૂર્ણાંક cid = __mm_get_cid(mm, READ_ONCE(mm->mm_cid.max_cids));

	જ્યારે (cid == MM_CID_UNSET) {
		cpu_relax();
		cid = __mm_get_cid(mm, અંક_possible_cpus());
	}
	પાછા cid;
}

સ્થિર અંતઃરેખ unsigned પૂર્ણાંક mm_cid_converge(સંરચના mm_struct *mm, unsigned પૂર્ણાંક orig_cid,
					   unsigned પૂર્ણાંક max_cids)
{
	unsigned પૂર્ણાંક new_cid, cid = cpu_cid_to_cid(orig_cid);

	/* Is it in the optimal CID space? */
	જો (likely(cid < max_cids))
		પાછા orig_cid;

	/* Try to find one in the optimal space. Otherwise keep the provided. */
	new_cid = __mm_get_cid(mm, max_cids);
	જો (new_cid != MM_CID_UNSET) {
		mm_drop_cid(mm, cid);
		/* Preserve the ONCPU mode of the original CID */
		પાછા new_cid | (orig_cid & MM_CID_ONCPU);
	}
	પાછા orig_cid;
}

સ્થિર __always_inline શૂન્ય mm_cid_update_task_cid(સંરચના task_struct *t, unsigned પૂર્ણાંક cid)
{
	જો (t->mm_cid.cid != cid) {
		t->mm_cid.cid = cid;
		rseq_sched_set_ids_changed(t);
	}
}

સ્થિર __always_inline શૂન્ય mm_cid_update_pcpu_cid(સંરચના mm_struct *mm, unsigned પૂર્ણાંક cid)
{
	__this_cpu_write(mm->mm_cid.pcpu->cid, cid);
}

સ્થિર __always_inline શૂન્ય mm_cid_from_cpu(સંરચના task_struct *t, unsigned પૂર્ણાંક cpu_cid,
					    unsigned પૂર્ણાંક mode)
{
	unsigned પૂર્ણાંક max_cids, tcid = t->mm_cid.cid;
	સંરચના mm_struct *mm = t->mm;

	max_cids = READ_ONCE(mm->mm_cid.max_cids);
	/* Optimize for the common case where both have the ONCPU bit set */
	જો (likely(cid_on_cpu(cpu_cid & tcid))) {
		જો (likely(cpu_cid_to_cid(cpu_cid) < max_cids)) {
			mm_cid_update_task_cid(t, cpu_cid);
			પાછા;
		}
		/* Try to converge into the optimal CID space */
		cpu_cid = mm_cid_converge(mm, cpu_cid, max_cids);
	} નહીંતર {
		/* Hand over or drop the task owned CID */
		જો (cid_on_task(tcid)) {
			જો (cid_on_cpu(cpu_cid))
				mm_unset_cid_on_task(t);
			નહીંતર
				cpu_cid = cid_to_cpu_cid(tcid);
		}
		/* Still nothing, allocate a new one */
		જો (!cid_on_cpu(cpu_cid))
			cpu_cid = cid_to_cpu_cid(mm_get_cid(mm));

		/* Handle the transition mode flag if required */
		જો (mode & MM_CID_TRANSIT)
			cpu_cid = cpu_cid_to_cid(cpu_cid) | MM_CID_TRANSIT;
	}
	mm_cid_update_pcpu_cid(mm, cpu_cid);
	mm_cid_update_task_cid(t, cpu_cid);
}

સ્થિર __always_inline શૂન્ય mm_cid_from_task(સંરચના task_struct *t, unsigned પૂર્ણાંક cpu_cid,
					     unsigned પૂર્ણાંક mode)
{
	unsigned પૂર્ણાંક max_cids, tcid = t->mm_cid.cid;
	સંરચના mm_struct *mm = t->mm;

	max_cids = READ_ONCE(mm->mm_cid.max_cids);
	/* Optimize for the common case, where both have the ONCPU bit clear */
	જો (likely(cid_on_task(tcid | cpu_cid))) {
		જો (likely(tcid < max_cids)) {
			mm_cid_update_pcpu_cid(mm, tcid);
			પાછા;
		}
		/* Try to converge into the optimal CID space */
		tcid = mm_cid_converge(mm, tcid, max_cids);
	} નહીંતર {
		/* Hand over or drop the CPU owned CID */
		જો (cid_on_cpu(cpu_cid)) {
			જો (cid_on_task(tcid))
				mm_drop_cid_on_cpu(mm, this_cpu_ptr(mm->mm_cid.pcpu));
			નહીંતર
				tcid = cpu_cid_to_cid(cpu_cid);
		}
		/* Still nothing, allocate a new one */
		જો (!cid_on_task(tcid))
			tcid = mm_get_cid(mm);
		/* Set the transition mode flag if required */
		tcid |= mode & MM_CID_TRANSIT;
	}
	mm_cid_update_pcpu_cid(mm, tcid);
	mm_cid_update_task_cid(t, tcid);
}

સ્થિર __always_inline શૂન્ય mm_cid_schedin(સંરચના task_struct *next)
{
	સંરચના mm_struct *mm = next->mm;
	unsigned પૂર્ણાંક cpu_cid, mode;

	જો (!next->mm_cid.active)
		પાછા;

	cpu_cid = __this_cpu_read(mm->mm_cid.pcpu->cid);
	mode = READ_ONCE(mm->mm_cid.mode);
	જો (likely(!cid_on_cpu(mode)))
		mm_cid_from_task(next, cpu_cid, mode);
	નહીંતર
		mm_cid_from_cpu(next, cpu_cid, mode);
}

સ્થિર __always_inline શૂન્ય mm_cid_schedout(સંરચના task_struct *prev)
{
	સંરચના mm_struct *mm = prev->mm;
	unsigned પૂર્ણાંક mode, cid;

	/* During mode transitions CIDs are temporary and need to be dropped */
	જો (likely(!cid_in_transit(prev->mm_cid.cid)))
		પાછા;

	mode = READ_ONCE(mm->mm_cid.mode);
	cid = cid_from_transit_cid(prev->mm_cid.cid);

	/*
	 * If transition mode is done, transfer ownership when the CID is
	 * within the convergence range to optimize the next schedule in.
	 */
	જો (!cid_in_transit(mode) && cid < READ_ONCE(mm->mm_cid.max_cids)) {
		જો (cid_on_cpu(mode))
			cid = cid_to_cpu_cid(cid);

		/* Update both so that the next schedule in goes into the fast path */
		mm_cid_update_pcpu_cid(mm, cid);
		prev->mm_cid.cid = cid;
	} નહીંતર {
		mm_drop_cid(mm, cid);
		prev->mm_cid.cid = MM_CID_UNSET;
	}
}

સ્થિર અંતઃરેખ શૂન્ય mm_cid_switch_to(સંરચના task_struct *prev, સંરચના task_struct *next)
{
	mm_cid_schedout(prev);
	mm_cid_schedin(next);
}

#નહીંતર /* !CONFIG_SCHED_MM_CID: */
સ્થિર અંતઃરેખ શૂન્ય mm_cid_switch_to(સંરચના task_struct *prev, સંરચના task_struct *next) { }
#endif /* !CONFIG_SCHED_MM_CID */

#ifdef CONFIG_SCHED_CACHE
DECLARE_STATIC_ચાવી_FALSE(sched_cache_present);
DECLARE_STATIC_ચાવી_FALSE(sched_cache_active);
બાહ્ય પૂર્ણાંક sysctl_sched_cache_user;
બાહ્ય unsigned પૂર્ણાંક llc_aggr_tolerance;
બાહ્ય unsigned પૂર્ણાંક llc_epoch_period;
બાહ્ય unsigned પૂર્ણાંક llc_epoch_affinity_timeout;
બાહ્ય unsigned પૂર્ણાંક llc_imb_pct;
બાહ્ય unsigned પૂર્ણાંક llc_overaggr_pct;

સ્થિર અંતઃરેખ bool sched_cache_enabled(શૂન્ય)
{
	પાછા static_branch_unlikely(&sched_cache_active);
}

બાહ્ય શૂન્ય sched_cache_active_set(શૂન્ય);

#endif

શૂન્ય sched_domains_free_llc_id(પૂર્ણાંક cpu);

બાહ્ય શૂન્ય init_sched_mm(સંરચના task_struct *p);

બાહ્ય u64 avg_vruntime(સંરચના cfs_rq *cfs_rq);
બાહ્ય પૂર્ણાંક entity_eligible(સંરચના cfs_rq *cfs_rq, સંરચના sched_entity *se);
સ્થિર અંતઃરેખ
શૂન્ય move_queued_task_locked(સંરચના rq *src_rq, સંરચના rq *dst_rq, સંરચના task_struct *task)
{
	lockdep_assert_rq_held(src_rq);
	lockdep_assert_rq_held(dst_rq);

	deactivate_task(src_rq, task, 0);
	set_task_cpu(task, dst_rq->cpu);
	activate_task(dst_rq, task, 0);
	wakeup_preempt(dst_rq, task, 0);
}

સ્થિર અંતઃરેખ
bool task_is_pushable(સંરચના rq *rq, સંરચના task_struct *p, પૂર્ણાંક cpu)
{
	જો (!task_on_cpu(rq, p) &&
	    cpumask_test_cpu(cpu, &p->cpus_mask))
		પાછા true;

	પાછા false;
}

#ifdef CONFIG_RT_MUTEXES

સ્થિર અંતઃરેખ પૂર્ણાંક __rt_effective_prio(સંરચના task_struct *pi_task, પૂર્ણાંક prio)
{
	જો (pi_task)
		prio = min(prio, pi_task->prio);

	પાછા prio;
}

સ્થિર અંતઃરેખ પૂર્ણાંક rt_effective_prio(સંરચના task_struct *p, પૂર્ણાંક prio)
{
	સંરચના task_struct *pi_task = rt_mutex_get_ટોચ_task(p);

	પાછા __rt_effective_prio(pi_task, prio);
}

#નહીંતર /* !CONFIG_RT_MUTEXES: */

સ્થિર અંતઃરેખ પૂર્ણાંક rt_effective_prio(સંરચના task_struct *p, પૂર્ણાંક prio)
{
	પાછા prio;
}

#endif /* !CONFIG_RT_MUTEXES */

બાહ્ય પૂર્ણાંક __sched_setscheduler(સંરચના task_struct *p, અચલ સંરચના sched_attr *attr, bool user, bool pi);
બાહ્ય પૂર્ણાંક __sched_setaffinity(સંરચના task_struct *p, સંરચના affinity_context *ctx);
બાહ્ય અચલ સંરચના sched_class *__setscheduler_class(પૂર્ણાંક policy, પૂર્ણાંક prio);
બાહ્ય શૂન્ય set_load_weight(સંરચના task_struct *p, bool update_load);
બાહ્ય શૂન્ય enqueue_task(સંરચના rq *rq, સંરચના task_struct *p, પૂર્ણાંક flags);
બાહ્ય bool dequeue_task(સંરચના rq *rq, સંરચના task_struct *p, પૂર્ણાંક flags);

બાહ્ય સંરચના balance_callback *splice_balance_callbacks(સંરચના rq *rq);

બાહ્ય શૂન્ય __balance_callbacks(સંરચના rq *rq, સંરચના rq_flags *rf);
બાહ્ય શૂન્ય balance_callbacks(સંરચના rq *rq, સંરચના balance_callback *head);

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
સંરચના sched_change_ctx {
	u64			prio;
	સંરચના task_struct	*p;
	અચલ સંરચના sched_class *class;
	પૂર્ણાંક			flags;
	bool			queued;
	bool			running;
};

સંરચના sched_change_ctx *sched_change_begin(સંરચના task_struct *p, unsigned પૂર્ણાંક flags);
શૂન્ય sched_change_end(સંરચના sched_change_ctx *ctx);

DEFINE_CLASS(sched_change, સંરચના sched_change_ctx *,
	     sched_change_end(_T),
	     sched_change_begin(p, flags),
	     સંરચના task_struct *p, unsigned પૂર્ણાંક flags)

DEFINE_CLASS_IS_UNCONDITIONAL(sched_change)

#અન્તર્ભાવ "ext/ext.h"

#ifdef CONFIG_PREFERRED_CPU
શૂન્ય sched_ધકેલો_current_non_preferred_cpu(સંરચના rq *rq);
#નહીંતર	/* !CONFIG_PREFERRED_CPU */
સ્થિર અંતઃરેખ શૂન્ય sched_ધકેલો_current_non_preferred_cpu(સંરચના rq *rq) { }
#endif

#endif /* _KERNEL_SCHED_SCHED_H */
