/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Scheduler internal types and methods:
 */
#ifndef _KERNEL_SCHED_SCHED_H
#define _KERNEL_SCHED_SCHED_H

#अन्तर्भाव <linux/prandom.h>
#अन्तर्भाव <linux/sched/affinity.h>
#अन्तर्भाव <linux/sched/autogroup.h>
#अन्तर्भाव <linux/sched/cpufreq.h>
#अन्तर्भाव <linux/sched/deadline.h>
#अन्तर्भाव <linux/sched.h>
#अन्तर्भाव <linux/sched/loadavg.h>
#अन्तर्भाव <linux/sched/mm.h>
#अन्तर्भाव <linux/sched/rseq_api.h>
#अन्तर्भाव <linux/sched/signal.h>
#अन्तर्भाव <linux/sched/smt.h>
#अन्तर्भाव <linux/sched/stat.h>
#अन्तर्भाव <linux/sched/sysctl.h>
#अन्तर्भाव <linux/sched/task_flags.h>
#अन्तर्भाव <linux/sched/task.h>
#अन्तर्भाव <linux/sched/topology.h>
#अन्तर्भाव <linux/atomic.h>
#अन्तर्भाव <linux/bitmap.h>
#अन्तर्भाव <linux/bug.h>
#अन्तर्भाव <linux/capability.h>
#अन्तर्भाव <linux/cgroup_api.h>
#अन्तर्भाव <linux/cgroup.h>
#अन्तर्भाव <linux/context_tracking.h>
#अन्तर्भाव <linux/cpufreq.h>
#अन्तर्भाव <linux/cpumask_api.h>
#अन्तर्भाव <linux/cpuset.h>
#अन्तर्भाव <linux/ctype.h>
#अन्तर्भाव <linux/file.h>
#अन्तर्भाव <linux/fs_api.h>
#अन्तर्भाव <linux/hrtimer_api.h>
#अन्तर्भाव <linux/interrupt.h>
#अन्तर्भाव <linux/irq_work.h>
#अन्तर्भाव <linux/jiffies.h>
#अन्तर्भाव <linux/kref_api.h>
#अन्तर्भाव <linux/kthread.h>
#अन्तर्भाव <linux/ktime_api.h>
#अन्तर्भाव <linux/lockdep_api.h>
#अन्तर्भाव <linux/lockdep.h>
#अन्तर्भाव <linux/memblock.h>
#अन्तर्भाव <linux/memcontrol.h>
#अन्तर्भाव <linux/minmax.h>
#अन्तर्भाव <linux/mm.h>
#अन्तर्भाव <linux/module.h>
#अन्तर्भाव <linux/mutex_api.h>
#अन्तर्भाव <linux/plist.h>
#अन्तर्भाव <linux/poll.h>
#अन्तर्भाव <linux/proc_fs.h>
#अन्तर्भाव <linux/profile.h>
#अन्तर्भाव <linux/psi.h>
#अन्तर्भाव <linux/rcupdate.h>
#अन्तर्भाव <linux/seq_file.h>
#अन्तर्भाव <linux/seqlock.h>
#अन्तर्भाव <linux/softirq.h>
#अन्तर्भाव <linux/spinlock_api.h>
#अन्तर्भाव <linux/static_key.h>
#अन्तर्भाव <linux/stop_machine.h>
#अन्तर्भाव <linux/syscalls_api.h>
#अन्तर्भाव <linux/syscalls.h>
#अन्तर्भाव <linux/tick.h>
#अन्तर्भाव <linux/topology.h>
#अन्तर्भाव <linux/types.h>
#अन्तर्भाव <linux/u64_stats_sync_api.h>
#अन्तर्भाव <linux/uaccess.h>
#अन्तर्भाव <linux/vmstat.h>
#अन्तर्भाव <linux/wait_api.h>
#अन्तर्भाव <linux/wait_bit.h>
#अन्तर्भाव <linux/workqueue_api.h>
#अन्तर्भाव <linux/delayacct.h>
#अन्तर्भाव <linux/mmu_context.h>

#अन्तर्भाव <trace/events/power.h>
#अन्तर्भाव <trace/events/sched.h>

#अन्तर्भाव "../workqueue_internal.h"

संरचना rq;
संरचना cfs_rq;
संरचना rt_rq;
संरचना sched_group;
संरचना cpuidle_state;

#जइ defined(CONFIG_PARAVIRT) && !defined(CONFIG_HAVE_PV_STEAL_CLOCK_GEN)
# include <asm/paravirt.h>
#endif

#अन्तर्भाव <asm/barrier.h>

#अन्तर्भाव "cpupri.h"
#अन्तर्भाव "cpudeadline.h"

/* task_struct::on_rq states: */
#define TASK_ON_RQ_QUEUED	1
#define TASK_ON_RQ_MIGRATING	2

बाह्य __read_mostly पूर्णाङ्क scheduler_running;

बाह्य unsigned long calc_load_update;
बाह्य atomic_long_t calc_load_tasks;

बाह्य शून्य calc_global_load_tick(संरचना rq *this_rq);
बाह्य long calc_load_fold_active(संरचना rq *this_rq, long adjust);

बाह्य शून्य call_trace_sched_update_nr_running(संरचना rq *rq, पूर्णाङ्क गणनम्);

बाह्य पूर्णाङ्क sysctl_sched_rt_period;
बाह्य पूर्णाङ्क sysctl_sched_rt_runtime;
बाह्य पूर्णाङ्क sched_rr_timeslice;

/*
 * Asymmetric CPU capacity bits
 */
संरचना asym_cap_data {
	संरचना list_head link;
	संरचना rcu_head rcu;
	unsigned long capacity;
	unsigned long cpus[];
};

बाह्य संरचना list_head asym_cap_list;

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
	जइ (__w)						\
		__w = max(2UL, __w >> SCHED_FIXEDPOINT_SHIFT);	\
	__w;							\
})
#अण्णहा
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

स्थिर अन्तःस्थ पूर्णाङ्क idle_policy(पूर्णाङ्क policy)
{
	प्रत्यागम policy == SCHED_IDLE;
}

स्थिर अन्तःस्थ पूर्णाङ्क normal_policy(पूर्णाङ्क policy)
{
#ifdef CONFIG_SCHED_CLASS_EXT
	जइ (policy == SCHED_EXT)
		प्रत्यागम true;
#endif
	प्रत्यागम policy == SCHED_NORMAL;
}

स्थिर अन्तःस्थ पूर्णाङ्क fair_policy(पूर्णाङ्क policy)
{
	प्रत्यागम normal_policy(policy) || policy == SCHED_BATCH;
}

स्थिर अन्तःस्थ पूर्णाङ्क rt_policy(पूर्णाङ्क policy)
{
	प्रत्यागम policy == SCHED_FIFO || policy == SCHED_RR;
}

स्थिर अन्तःस्थ पूर्णाङ्क dl_policy(पूर्णाङ्क policy)
{
	प्रत्यागम policy == SCHED_DEADLINE;
}

स्थिर अन्तःस्थ bool valid_policy(पूर्णाङ्क policy)
{
	प्रत्यागम idle_policy(policy) || fair_policy(policy) ||
		rt_policy(policy) || dl_policy(policy);
}

स्थिर अन्तःस्थ पूर्णाङ्क task_has_idle_policy(संरचना task_struct *p)
{
	प्रत्यागम idle_policy(p->policy);
}

स्थिर अन्तःस्थ पूर्णाङ्क task_has_rt_policy(संरचना task_struct *p)
{
	प्रत्यागम rt_policy(p->policy);
}

स्थिर अन्तःस्थ पूर्णाङ्क task_has_dl_policy(संरचना task_struct *p)
{
	प्रत्यागम dl_policy(p->policy);
}

#define cap_scale(v, s)		((v)*(s) >> SCHED_CAPACITY_SHIFT)

स्थिर अन्तःस्थ शून्य update_avg(u64 *avg, u64 sample)
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
स्थिर अन्तःस्थ unsigned long sched_weight_from_cgroup(unsigned long cgrp_weight)
{
	प्रत्यागम DIV_ROUND_CLOSEST_ULL(cgrp_weight * 1024, CGROUP_WEIGHT_DFL);
}

स्थिर अन्तःस्थ unsigned long sched_weight_to_cgroup(unsigned long weight)
{
	प्रत्यागम clamp_t(unsigned long,
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

स्थिर अन्तःस्थ bool dl_entity_is_special(ध्रुव संरचना sched_dl_entity *dl_se)
{
#ifdef CONFIG_CPU_FREQ_GOV_SCHEDUTIL
	प्रत्यागम unlikely(dl_se->flags & SCHED_FLAG_SUGOV);
#अण्णहा
	प्रत्यागम false;
#endif
}

/*
 * Tells if entity @a should preempt entity @b.
 */
स्थिर अन्तःस्थ bool dl_entity_preempt(ध्रुव संरचना sched_dl_entity *a,
				     ध्रुव संरचना sched_dl_entity *b)
{
	प्रत्यागम dl_entity_is_special(a) ||
	       dl_time_before(a->deadline, b->deadline);
}

/*
 * This is the priority-queue data structure of the RT scheduling class:
 */
संरचना rt_prio_array {
	DECLARE_BITMAP(bitmap, MAX_RT_PRIO+1); /* include 1 bit for delimiter */
	संरचना list_head queue[MAX_RT_PRIO];
};

संरचना rt_bandwidth {
	/* nests inside the rq lock: */
	raw_spinlock_t		rt_runtime_lock;
	ktime_t			rt_period;
	u64			rt_runtime;
	संरचना hrtimer		rt_period_timer;
	unsigned पूर्णाङ्क		rt_period_active;
};

स्थिर अन्तःस्थ पूर्णाङ्क dl_bandwidth_enabled(शून्य)
{
	प्रत्यागम sysctl_sched_rt_runtime >= 0;
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
संरचना dl_bw {
	raw_spinlock_t		lock;
	u64			bw;
	u64			total_bw;
};

बाह्य शून्य init_dl_bw(संरचना dl_bw *dl_b);
बाह्य पूर्णाङ्क  sched_dl_global_validate(शून्य);
बाह्य शून्य sched_dl_do_global(शून्य);
बाह्य पूर्णाङ्क  sched_dl_overflow(संरचना task_struct *p, पूर्णाङ्क policy, ध्रुव संरचना sched_attr *attr);
बाह्य शून्य __setparam_dl(संरचना task_struct *p, ध्रुव संरचना sched_attr *attr);
बाह्य शून्य __getparam_dl(संरचना task_struct *p, संरचना sched_attr *attr, unsigned पूर्णाङ्क flags);
बाह्य bool __checkparam_dl(ध्रुव संरचना sched_attr *attr);
बाह्य bool dl_param_changed(संरचना task_struct *p, ध्रुव संरचना sched_attr *attr);
बाह्य पूर्णाङ्क  dl_cpuset_cpumask_can_shrink(ध्रुव संरचना cpumask *cur, ध्रुव संरचना cpumask *trial);
बाह्य पूर्णाङ्क  dl_bw_deactivate(पूर्णाङ्क cpu);
बाह्य s64 dl_scaled_delta_exec(संरचना rq *rq, संरचना sched_dl_entity *dl_se, s64 delta_exec);
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
बाह्य शून्य dl_server_update_idle(संरचना sched_dl_entity *dl_se, s64 delta_exec);
बाह्य शून्य dl_server_update(संरचना sched_dl_entity *dl_se, s64 delta_exec);
बाह्य शून्य dl_server_start(संरचना sched_dl_entity *dl_se);
बाह्य शून्य dl_server_stop(संरचना sched_dl_entity *dl_se);
बाह्य शून्य dl_server_init(संरचना sched_dl_entity *dl_se, संरचना rq *rq,
		    dl_server_pick_f pick_task);
बाह्य शून्य sched_init_dl_servers(शून्य);

बाह्य शून्य fair_server_init(संरचना rq *rq);
बाह्य शून्य ext_server_init(संरचना rq *rq);
बाह्य शून्य __dl_server_attach_root(संरचना sched_dl_entity *dl_se, संरचना rq *rq);
बाह्य पूर्णाङ्क dl_server_apply_params(संरचना sched_dl_entity *dl_se,
		    u64 runtime, u64 period, bool init);
बाह्य पूर्णाङ्क dl_server_attach_bw(संरचना sched_dl_entity *dl_se);
बाह्य शून्य dl_server_detach_bw(संरचना sched_dl_entity *dl_se);
बाह्य पूर्णाङ्क dl_server_swap_bw(संरचना sched_dl_entity *detach_se,
			     संरचना sched_dl_entity *attach_se);

स्थिर अन्तःस्थ bool dl_server_active(संरचना sched_dl_entity *dl_se)
{
	प्रत्यागम dl_se->dl_server_active;
}

#ifdef CONFIG_CGROUP_SCHED

बाह्य संरचना list_head task_groups;

#ifdef CONFIG_GROUP_SCHED_BANDWIDTH
बाह्य ध्रुव u64 max_bw_quota_period_us;

/*
 * default period for group bandwidth.
 * default: 0.1s, units: microseconds
 */
स्थिर अन्तःस्थ u64 default_bw_period_us(शून्य)
{
	प्रत्यागम 100000ULL;
}
#endif /* CONFIG_GROUP_SCHED_BANDWIDTH */

संरचना cfs_bandwidth {
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
	संरचना hrtimer		period_timer;
	संरचना hrtimer		slack_timer;
	संरचना list_head	throttled_cfs_rq;

	/* Statistics: */
	पूर्णाङ्क			nr_periods;
	पूर्णाङ्क			nr_throttled;
	पूर्णाङ्क			nr_burst;
	u64			throttled_time;
	u64			burst_time;
#endif /* CONFIG_CFS_BANDWIDTH */
};

/* Task group related information */
संरचना task_group {
	संरचना cgroup_subsys_state css;

#ifdef CONFIG_GROUP_SCHED_WEIGHT
	/* A positive value indicates that this is a SCHED_IDLE group. */
	पूर्णाङ्क			idle;
#endif

#ifdef CONFIG_FAIR_GROUP_SCHED
	/* runqueue "owned" by this group on each CPU */
	संरचना cfs_rq __percpu	*cfs_rq;
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
	संरचना sched_rt_entity	**rt_se;
	संरचना rt_rq		**rt_rq;

	संरचना rt_bandwidth	rt_bandwidth;
#endif

	संरचना scx_task_group	scx;

	संरचना rcu_head		rcu;
	संरचना list_head	list;

	संरचना task_group	*parent;
	संरचना list_head	siblings;
	संरचना list_head	children;

#ifdef CONFIG_SCHED_AUTOGROUP
	संरचना autogroup	*autogroup;
#endif

	संरचना cfs_bandwidth	cfs_bandwidth;

#ifdef CONFIG_UCLAMP_TASK_GROUP
	/* The two decimal precision [%] value requested from user-space */
	unsigned पूर्णाङ्क		uclamp_pct[UCLAMP_CNT];
	/* Clamp values requested for a task group */
	संरचना uclamp_se	uclamp_req[UCLAMP_CNT];
	/* Effective clamp values used for a task group */
	संरचना uclamp_se	uclamp[UCLAMP_CNT];
#endif

};

#define मूलम्_TASK_GROUP_LOAD	NICE_0_LOAD

प्रकारनिर्देश पूर्णाङ्क (*tg_visitor)(संरचना task_group *, शून्य *);

बाह्य पूर्णाङ्क walk_tg_वृक्षः_from(संरचना task_group *from,
			     tg_visitor down, tg_visitor up, शून्य *data);

/*
 * Iterate the full tree, calling @down when first entering a node and @up when
 * leaving it for the final time.
 *
 * Caller must hold rcu_lock or sufficient equivalent.
 */
स्थिर अन्तःस्थ पूर्णाङ्क walk_tg_वृक्षः(tg_visitor down, tg_visitor up, शून्य *data)
{
	प्रत्यागम walk_tg_वृक्षः_from(&मूलम्_task_group, down, up, data);
}

स्थिर अन्तःस्थ संरचना task_group *css_tg(संरचना cgroup_subsys_state *css)
{
	प्रत्यागम css ? container_of(css, संरचना task_group, css) : रिक्त;
}

बाह्य पूर्णाङ्क tg_nop(संरचना task_group *tg, शून्य *data);

#ifdef CONFIG_FAIR_GROUP_SCHED
बाह्य शून्य free_fair_sched_group(संरचना task_group *tg);
बाह्य पूर्णाङ्क alloc_fair_sched_group(संरचना task_group *tg, संरचना task_group *parent);
बाह्य शून्य online_fair_sched_group(संरचना task_group *tg);
बाह्य शून्य unregister_fair_sched_group(संरचना task_group *tg);
बाह्य शून्य __sched_cgroup_mode_update(पूर्णाङ्क mode);
#अण्णहा /* !CONFIG_FAIR_GROUP_SCHED: */
स्थिर अन्तःस्थ शून्य free_fair_sched_group(संरचना task_group *tg) { }
स्थिर अन्तःस्थ पूर्णाङ्क alloc_fair_sched_group(संरचना task_group *tg, संरचना task_group *parent)
{
       प्रत्यागम 1;
}
स्थिर अन्तःस्थ शून्य online_fair_sched_group(संरचना task_group *tg) { }
स्थिर अन्तःस्थ शून्य unregister_fair_sched_group(संरचना task_group *tg) { }
#endif /* !CONFIG_FAIR_GROUP_SCHED */

बाह्य शून्य init_tg_cfs_entry(संरचना task_group *tg, संरचना cfs_rq *cfs_rq,
			संरचना sched_entity *se, पूर्णाङ्क cpu,
			संरचना sched_entity *parent);
बाह्य शून्य init_cfs_bandwidth(संरचना cfs_bandwidth *cfs_b, संरचना cfs_bandwidth *parent);

बाह्य शून्य __refill_cfs_bandwidth_runtime(संरचना cfs_bandwidth *cfs_b);
बाह्य शून्य start_cfs_bandwidth(संरचना cfs_bandwidth *cfs_b);
बाह्य शून्य unthrottle_cfs_rq(संरचना cfs_rq *cfs_rq);
बाह्य bool cfs_task_bw_constrained(संरचना task_struct *p);

बाह्य शून्य init_tg_rt_entry(संरचना task_group *tg, संरचना rt_rq *rt_rq,
		संरचना sched_rt_entity *rt_se, पूर्णाङ्क cpu,
		संरचना sched_rt_entity *parent);
बाह्य पूर्णाङ्क sched_group_set_rt_runtime(संरचना task_group *tg, long rt_runtime_us);
बाह्य पूर्णाङ्क sched_group_set_rt_period(संरचना task_group *tg, u64 rt_period_us);
बाह्य long sched_group_rt_runtime(संरचना task_group *tg);
बाह्य long sched_group_rt_period(संरचना task_group *tg);
बाह्य पूर्णाङ्क sched_rt_can_attach(संरचना task_group *tg, संरचना task_struct *tsk);

बाह्य संरचना task_group *sched_create_group(संरचना task_group *parent);
बाह्य शून्य sched_online_group(संरचना task_group *tg,
			       संरचना task_group *parent);
बाह्य शून्य sched_destroy_group(संरचना task_group *tg);
बाह्य शून्य sched_release_group(संरचना task_group *tg);

बाह्य शून्य sched_move_task(संरचना task_struct *tsk, bool for_autogroup);

#ifdef CONFIG_FAIR_GROUP_SCHED
बाह्य पूर्णाङ्क sched_group_set_shares(संरचना task_group *tg, unsigned long shares);

बाह्य पूर्णाङ्क sched_group_set_idle(संरचना task_group *tg, long idle);

बाह्य शून्य set_task_rq_fair(संरचना sched_entity *se,
			     संरचना cfs_rq *prev, संरचना cfs_rq *next);
#अण्णहा /* !CONFIG_FAIR_GROUP_SCHED: */
स्थिर अन्तःस्थ पूर्णाङ्क sched_group_set_shares(संरचना task_group *tg, unsigned long shares) { प्रत्यागम 0; }
स्थिर अन्तःस्थ पूर्णाङ्क sched_group_set_idle(संरचना task_group *tg, long idle) { प्रत्यागम 0; }
#endif /* !CONFIG_FAIR_GROUP_SCHED */

#अण्णहा /* !CONFIG_CGROUP_SCHED: */

संरचना cfs_bandwidth { };

स्थिर अन्तःस्थ bool cfs_task_bw_constrained(संरचना task_struct *p) { प्रत्यागम false; }

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

बाह्य शून्य unregister_rt_sched_group(संरचना task_group *tg);
बाह्य शून्य free_rt_sched_group(संरचना task_group *tg);
बाह्य पूर्णाङ्क alloc_rt_sched_group(संरचना task_group *tg, संरचना task_group *parent);

/*
 * u64_u32_load/u64_u32_store
 *
 * Use a copy of a u64 value to protect against data race. This is only
 * applicable for 32-bits architectures.
 */
#ifdef CONFIG_64BIT
# define u64_u32_load_copy(var, copy)		var
# define u64_u32_store_copy(var, copy, val)	(var = val)
#अण्णहा
# define u64_u32_load_copy(var, copy)					\
({									\
	u64 __val, __val_copy;						\
	करेहि {								\
		__val_copy = copy;					\
		/*							\
		 * paired with u64_u32_store_copy(), ordering access	\
		 * to var and copy.					\
		 */							\
		smp_rmb();						\
		__val = var;						\
	} जाव (__val != __val_copy);					\
	__val;								\
})
# define u64_u32_store_copy(var, copy, val)				\
करेहि {									\
	typeof(val) __val = (val);					\
	var = __val;							\
	/*								\
	 * paired with u64_u32_load_copy(), ordering access to var and	\
	 * copy.							\
	 */								\
	smp_wmb();							\
	copy = __val;							\
} जाव (0)
#endif
# define u64_u32_load(var)		u64_u32_load_copy(var, var##_copy)
# define u64_u32_store(var, val)	u64_u32_store_copy(var, var##_copy, val)

संरचना balance_callback {
	संरचना balance_callback *next;
	शून्य (*func)(संरचना rq *rq);
};

/* Fair scheduling SCHED_{NORMAL,BATCH,IDLE} related fields in a runqueue: */
संरचना cfs_rq {
	संरचना load_weight	load;
	unsigned पूर्णाङ्क		nr_queued;
	unsigned पूर्णाङ्क		h_nr_queued;		/* SCHED_{NORMAL,BATCH,IDLE} */
	unsigned पूर्णाङ्क		h_nr_runnable;		/* SCHED_{NORMAL,BATCH,IDLE} */
	unsigned पूर्णाङ्क		h_nr_idle;		/* SCHED_IDLE */

	s64			sum_w_vruntime;
	u64			sum_weight;
	u64			zero_vruntime;
	unsigned पूर्णाङ्क		sum_shift;

#ifdef CONFIG_SCHED_CORE
	unsigned पूर्णाङ्क		forceidle_seq;
	u64			zero_vruntime_fi;
#endif

	संरचना rb_मूलम्_cached	tasks_timeline;

	/*
	 * 'curr' points to the currently running entity on this cfs_rq.
	 * It is set to NULL otherwise (i.e when none are currently running).
	 */
	संरचना sched_entity	*curr;
	संरचना sched_entity	*next;

	/*
	 * CFS load tracking
	 */
	संरचना sched_entity	*h_curr;
	संरचना sched_avg	avg;
#ifndef CONFIG_64BIT
	u64			last_update_time_copy;
#endif
	संरचना {
		raw_spinlock_t	lock ____cacheline_aligned;
		पूर्णाङ्क		nr;
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
	संरचना sched_entity	*h_load_next;

	संरचना rq		*rq;	/* CPU runqueue to which this cfs_rq is attached */

	/*
	 * leaf cfs_rqs are those that hold tasks (lowest schedulable entity in
	 * a hierarchy). Non-leaf lrqs hold other higher schedulable entities
	 * (like users, containers etc.)
	 *
	 * leaf_cfs_rq_list ties together list of leaf cfs_rq's in a CPU.
	 * This list is used during load balance.
	 */
	पूर्णाङ्क			on_list;
	संरचना list_head	leaf_cfs_rq_list;
	संरचना task_group	*tg;	/* Group that "owns" this runqueue */

	/* Locally cached copy of our task_group's idle value */
	पूर्णाङ्क			idle;

# ifdef CONFIG_CFS_BANDWIDTH
	पूर्णाङ्क			runtime_enabled;
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
	पूर्णाङ्क			throttle_गणनम्;
	संरचना list_head	throttled_list;
	संरचना list_head	throttled_csd_list;
	संरचना list_head        throttled_limbo_list;
# endif /* CONFIG_CFS_BANDWIDTH */
#endif /* CONFIG_FAIR_GROUP_SCHED */
};

#ifdef CONFIG_SCHED_CLASS_EXT
/* scx_rq->flags, protected by the rq lock */
गणना scx_rq_flags {
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
	SCX_RQ_मूलम्_IDLE_RENOTIFY	= 1 << 8, /* the root is owed update_idle() */

	SCX_RQ_IN_WAKEUP	= 1 << 16,
	SCX_RQ_IN_DISPATCH	= 1 << 17,
};

/* per-rq rescue execution state, see scx_rescue_timerfn() */
संरचना scx_rq_rescue {
	संरचना scx_dispatch_q	dsq;			/* stranded tasks awaiting rescue */
	s64			budget;			/* execution token bucket, ns */
	u64			clock;			/* last budget accrual timestamp */
	संरचना task_struct	*curr;			/* task being rescued, one at a time */
	s64			slice;			/* curr's admitted slice */
	u64			exec_snap;		/* sum_exec_runtime at admission */
	संरचना timer_list	timer;			/* paces admission and escalation */
	u64			kill_at;		/* last ejection, init before any */
};

संरचना scx_rq {
	संरचना scx_dispatch_q	local_dsq;
#ifdef CONFIG_EXT_SUB_SCHED
	संरचना scx_dispatch_q	reject_dsq;		/* staging for cap-rejected tasks */
	संरचना scx_rq_rescue	rescue;
#endif
	संरचना list_head	runnable_list;		/* runnable tasks on this rq */
	संरचना list_head	ddsp_deferred_locals;	/* deferred ddsps from enq */
	/* both stashed across the activate_task() in move_remote_task_to_local_dsq() */
	u64			remote_activate_enq_flags;
	संरचना scx_sched	*remote_activate_sch;
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
	संरचना llist_head	ecaps_to_sync;		/* pending ecaps syncs */
	संरचना task_struct	*sub_dispatch_prev;
#endif
	cpumask_var_t		cpus_to_sync;
	bool			kick_sync_pending;
	unsigned long		kick_sync;

	संरचना list_head	sched_pcpus_to_kick;	/* see kick_cpus_irq_workfn() */

	raw_spinlock_t		deferred_reenq_lock;
	संरचना list_head	deferred_reenq_locals;	/* scheds requesting reenq of local DSQ */
	संरचना list_head	deferred_reenq_users;	/* user DSQs requesting reenq */
	संरचना balance_callback	deferred_bal_cb;
	संरचना balance_callback	kick_sync_bal_cb;
	संरचना irq_work		deferred_irq_work;
	संरचना irq_work		kick_cpus_irq_work;
};
#endif /* CONFIG_SCHED_CLASS_EXT */

स्थिर अन्तःस्थ पूर्णाङ्क rt_bandwidth_enabled(शून्य)
{
	प्रत्यागम sysctl_sched_rt_runtime >= 0;
}

/* RT IPI pull logic requires IRQ_WORK */
#जइ defined(CONFIG_IRQ_WORK) && defined(CONFIG_SMP)
# define HAVE_RT_प्रक्षिप_IPI
#endif

/* Real-Time classes' related field in a runqueue: */
संरचना rt_rq {
	संरचना rt_prio_array	active;
	unsigned पूर्णाङ्क		rt_nr_running;
	unsigned पूर्णाङ्क		rr_nr_running;
	संरचना {
		पूर्णाङ्क		curr; /* highest queued rt task prio */
		पूर्णाङ्क		next; /* next highest */
	} highest_prio;
	bool			overloaded;
	संरचना plist_head	pushable_tasks;

	पूर्णाङ्क			rt_queued;

#ifdef CONFIG_RT_GROUP_SCHED
	पूर्णाङ्क			rt_throttled;
	u64			rt_time; /* consumed RT time, goes up in update_curr_rt */
	u64			rt_runtime; /* allotted RT time, "slice" from rt_bandwidth, RT sharing/balancing */
	/* Nests inside the rq lock: */
	raw_spinlock_t		rt_runtime_lock;

	unsigned पूर्णाङ्क		rt_nr_boosted;

	संरचना rq		*rq; /* this is always top-level rq, cache? */
#endif
#ifdef CONFIG_CGROUP_SCHED
	संरचना task_group	*tg; /* this tg has "this" rt_rq on given CPU for runnable entities */
#endif
};

स्थिर अन्तःस्थ bool rt_rq_is_runnable(संरचना rt_rq *rt_rq)
{
	प्रत्यागम rt_rq->rt_queued && rt_rq->rt_nr_running;
}

/* Deadline class' related fields in a runqueue */
संरचना dl_rq {
	/* runqueue is an rbtree, ordered by deadline */
	संरचना rb_मूलम्_cached	मूलम्;

	unsigned पूर्णाङ्क		dl_nr_running;

	/*
	 * Deadline values of the currently executing and the
	 * earliest ready task on this rq. Caching these facilitates
	 * the decision whether or not a ready but not running task
	 * should migrate somewhere else.
	 */
	संरचना {
		u64		curr;
		u64		next;
	} earliest_dl;

	bool			overloaded;

	संरचना sched_dl_entity	*curr;
	/*
	 * Tasks on this rq that can be pushed away. They are kept in
	 * an rb-tree, ordered by tasks' deadlines, with caching
	 * of the leftmost (earliest deadline) element.
	 */
	संरचना rb_मूलम्_cached	pushable_dl_tasks_मूलम्;

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
#define is_मूलम्_task_group(tg) ((tg) == &मूलम्_task_group)
/* An entity is a task if it doesn't "own" a runqueue */
#define entity_is_task(se)	(!se->my_q)

स्थिर अन्तःस्थ शून्य se_update_runnable(संरचना sched_entity *se)
{
	जइ (!entity_is_task(se))
		se->runnable_weight = se->my_q->h_nr_runnable;
}

स्थिर अन्तःस्थ long se_runnable(संरचना sched_entity *se)
{
	जइ (se->sched_delayed)
		प्रत्यागम false;

	जइ (entity_is_task(se))
		प्रत्यागम !!se->on_rq;
	अण्णहा
		प्रत्यागम se->runnable_weight;
}

#अण्णहा /* !CONFIG_FAIR_GROUP_SCHED: */

#define entity_is_task(se)	1

स्थिर अन्तःस्थ शून्य se_update_runnable(संरचना sched_entity *se) { }

स्थिर अन्तःस्थ long se_runnable(संरचना sched_entity *se)
{
	जइ (se->sched_delayed)
		प्रत्यागम false;

	प्रत्यागम !!se->on_rq;
}

#endif /* !CONFIG_FAIR_GROUP_SCHED */

/*
 * XXX we want to get rid of these helpers and use the full load resolution.
 */
स्थिर अन्तःस्थ long se_weight(संरचना sched_entity *se)
{
	प्रत्यागम scale_load_down(se->load.weight);
}


स्थिर अन्तःस्थ bool sched_asym_prefer(पूर्णाङ्क a, पूर्णाङ्क b)
{
	प्रत्यागम arch_asym_cpu_priority(a) > arch_asym_cpu_priority(b);
}

संरचना perf_domain {
	संरचना em_perf_domain *em_pd;
	संरचना perf_domain *next;
	संरचना rcu_head rcu;
};

/*
 * We add the notion of a root-domain which will be used to define per-domain
 * variables. Each exclusive cpuset essentially defines an island domain by
 * fully partitioning the member CPUs from any other cpuset. Whenever a new
 * exclusive cpuset is created, we also create and attach a new root-domain
 * object.
 *
 */
संरचना मूलम्_domain {
	atomic_t		refcount;
	atomic_t		rto_गणनम्;
	संरचना rcu_head		rcu;
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
	atomic_t		dlo_गणनम्;
	संरचना dl_bw		dl_bw;
	संरचना cpudl		cpudl;

	/*
	 * Indicate whether a root_domain's dl_bw has been checked or
	 * updated. It's monotonously increasing value.
	 *
	 * Also, some corner cases, like 'wrap around' is dangerous, but given
	 * that u64 is 'big enough'. So that shouldn't be a concern.
	 */
	u64 visit_cookie;

#ifdef HAVE_RT_प्रक्षिप_IPI
	/*
	 * For IPI pull requests, loop across the rto_mask.
	 */
	संरचना irq_work		rto_प्रक्षिप_work;
	raw_spinlock_t		rto_lock;
	/* These are only updated and read within rto_lock */
	पूर्णाङ्क			rto_loop;
	पूर्णाङ्क			rto_cpu;
	/* These atomics are updated outside of a lock */
	atomic_t		rto_loop_next;
	atomic_t		rto_loop_start;
#endif /* HAVE_RT_PUSH_IPI */
	/*
	 * The "RT overload" flag: it gets set if a CPU has more than
	 * one runnable RT task.
	 */
	cpumask_var_t		rto_mask;
	संरचना cpupri		cpupri;

	/*
	 * NULL-terminated list of performance domains intersecting with the
	 * CPUs of the rd. Protected by RCU.
	 */
	संरचना perf_domain __rcu *pd;
};

बाह्य शून्य init_defrootdomain(शून्य);
बाह्य पूर्णाङ्क sched_init_domains(ध्रुव संरचना cpumask *cpu_map);
बाह्य शून्य rq_attach_मूलम्(संरचना rq *rq, संरचना मूलम्_domain *rd);
बाह्य शून्य sched_get_rd(संरचना मूलम्_domain *rd);
बाह्य शून्य sched_put_rd(संरचना मूलम्_domain *rd);

स्थिर अन्तःस्थ पूर्णाङ्क get_rd_overloaded(संरचना मूलम्_domain *rd)
{
	प्रत्यागम READ_ONCE(rd->overloaded);
}

स्थिर अन्तःस्थ शून्य set_rd_overloaded(संरचना मूलम्_domain *rd, पूर्णाङ्क status)
{
	जइ (get_rd_overloaded(rd) != status)
		WRITE_ONCE(rd->overloaded, status);
}

#ifdef HAVE_RT_प्रक्षिप_IPI
बाह्य शून्य rto_प्रक्षिप_irq_work_func(संरचना irq_work *work);
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
संरचना uclamp_bucket {
	unsigned long मानम् : bits_per(SCHED_CAPACITY_SCALE);
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
संरचना uclamp_rq {
	unsigned पूर्णाङ्क मानम्;
	संरचना uclamp_bucket bucket[UCLAMP_BUCKETS];
};

DECLARE_STATIC_कुञ्चिका_FALSE(sched_uclamp_used);
#endif /* CONFIG_UCLAMP_TASK */

/*
 * This is the main, per-CPU runqueue data structure.
 *
 * Locking rule: those places that want to lock multiple runqueues
 * (such as the load balancing or the thread migration code), lock
 * acquire operations must be ordered by ascending &runqueue.
 */
संरचना rq {
	/*
	 * The following members are loaded together, without holding the
	 * rq->lock, in an extremely hot loop in update_sg_lb_stats()
	 * (called from pick_next_task()). To reduce cache pollution from
	 * this operation, they are placed together on this dedicated cache
	 * line. Even though some of them are frequently modified, they are
	 * loaded much more frequently than they are stored.
	 */
	unsigned पूर्णाङ्क		nr_running;
#ifdef CONFIG_NUMA_BALANCING
	unsigned पूर्णाङ्क		nr_numa_running;
	unsigned पूर्णाङ्क		nr_preferred_running;
#endif
	unsigned पूर्णाङ्क		ttwu_pending;
	unsigned long		cpu_capacity;
#ifdef CONFIG_SCHED_PROXY_EXEC
	संरचना task_struct __rcu	*donor;  /* Scheduling context */
	संरचना task_struct __rcu	*curr;   /* Execution context */
#अण्णहा
	सङ्घ {
		संरचना task_struct __rcu *donor; /* Scheduler context */
		संरचना task_struct __rcu *curr;  /* Execution context */
	};
#endif
	संरचना task_struct	*idle;
	/* padding left here deliberately */

	/*
	 * The next cacheline holds the (hot) runqueue lock, as well as
	 * some other less performance-critical fields.
	 */
	u64			nr_switches	____cacheline_aligned;

	/* runqueue lock: */
	raw_spinlock_t		__lock;

#ifdef CONFIG_NO_HZ_COMMON
	unsigned पूर्णाङ्क		nohz_tick_stopped;
	atomic_t		nohz_flags;
	unsigned पूर्णाङ्क		has_blocked_load;
	unsigned long		last_blocked_load_update_tick;
	call_single_data_t	nohz_csd;
#endif /* CONFIG_NO_HZ_COMMON */

#ifdef CONFIG_UCLAMP_TASK
	/* Utilization clamp values based on CPU's RUNNABLE tasks */
	संरचना uclamp_rq	uclamp[UCLAMP_CNT] ____cacheline_aligned;
	unsigned पूर्णाङ्क		uclamp_flags;
#define UCLAMP_FLAG_IDLE 0x01
#endif

	संरचना cfs_rq		cfs;
	संरचना rt_rq		rt;
	संरचना dl_rq		dl;
#ifdef CONFIG_SCHED_CLASS_EXT
	संरचना scx_rq		scx;
	संरचना sched_dl_entity	ext_server;
#endif
#ifdef CONFIG_SCHED_CACHE
	raw_spinlock_t		cpu_epoch_lock ____cacheline_aligned;
	u64			cpu_runtime;
	unsigned long		cpu_epoch;
	unsigned long		cpu_epoch_next;
#endif

	संरचना sched_dl_entity	fair_server;

#ifdef CONFIG_FAIR_GROUP_SCHED
	/* list of leaf cfs_rq on this CPU: */
	संरचना list_head	leaf_cfs_rq_list;
	संरचना list_head	*tmp_alone_branch;
#endif /* CONFIG_FAIR_GROUP_SCHED */

#ifdef CONFIG_NUMA_BALANCING
	unsigned पूर्णाङ्क		numa_migrate_on;
#endif

#ifdef CONFIG_SCHED_CACHE
	unsigned पूर्णाङ्क		nr_pref_llc_running;
	unsigned पूर्णाङ्क		nr_llc_running;
#endif

	/*
	 * This is part of a global counter where only the total sum
	 * over all CPUs matters. A task can increase this counter on
	 * one CPU and if it got migrated afterwards it may decrease
	 * it on another CPU. Always updated under the runqueue lock:
	 */
	unsigned long		nr_uninterruptible;

	संरचना sched_dl_entity	*dl_server;
	संरचना task_struct	*stop;
	ध्रुव संरचना sched_class *next_class;
	unsigned long		next_balance;
	संरचना mm_struct	*prev_mm;

	/*
	 * The following fields of clock data are frequently referenced
	 * and updated together, and should go on their own cache line.
	 */
	u64			clock_task ____cacheline_aligned;
	u64			clock_pelt;
	u64			clock;
	unsigned long		lost_idle_time;
	unsigned पूर्णाङ्क		clock_update_flags;
	u64			clock_pelt_idle;
	u64			clock_idle;

#ifndef CONFIG_64BIT
	u64			clock_pelt_idle_copy;
	u64			clock_idle_copy;
#endif

	u64 last_seen_need_resched_ns;
	पूर्णाङ्क ticks_without_resched;

#ifdef CONFIG_MEMBARRIER
	पूर्णाङ्क membarrier_state;
#endif

	संरचना मूलम्_domain		*rd;
	संरचना sched_domain __rcu	*sd;

	संरचना balance_callback *balance_callback;

	unsigned वर्ण		nohz_idle_balance;
	unsigned वर्ण		idle_balance;

	unsigned long		misfit_task_load;

	/* For active balancing */
	पूर्णाङ्क			active_balance;
	पूर्णाङ्क			प्रक्षिप_cpu;
	संरचना cpu_stop_work	active_balance_work;

	/* CPU of this runqueue: */
	पूर्णाङ्क			cpu;
	पूर्णाङ्क			online;

	संरचना list_head cfs_tasks;

	संरचना sched_avg	avg_rt;
	संरचना sched_avg	avg_dl;
#ifdef CONFIG_HAVE_SCHED_AVG_IRQ
	संरचना sched_avg	avg_irq;
#endif
#ifdef CONFIG_SCHED_HW_PRESSURE
	संरचना sched_avg	avg_hw;
#endif
	u64			idle_stamp;
	u64			avg_idle;

	/* This is used to determine avg_idle's max value */
	u64			max_idle_balance_cost;

#ifdef CONFIG_HOTPLUG_CPU
	संरचना rcuwait		hotplug_wait;
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
	bool			npc_प्रक्षिप_work_pending;
#endif

	/* calc_load related fields */
	unsigned long		calc_load_update;
	long			calc_load_active;

#ifdef CONFIG_SCHED_HRTICK
	call_single_data_t	hrtick_csd;
	संरचना hrtimer		hrtick_timer;
	ktime_t			hrtick_time;
	ktime_t			hrtick_delay;
	unsigned पूर्णाङ्क		hrtick_sched;
#endif

#ifdef CONFIG_SCHEDSTATS
	/* latency stats */
	संरचना sched_info	rq_sched_info;
	unsigned long long	rq_cpu_time;

	/* sys_sched_yield() stats */
	unsigned पूर्णाङ्क		yld_गणनम्;

	/* schedule() stats */
	unsigned पूर्णाङ्क		sched_गणनम्;
	unsigned पूर्णाङ्क		sched_goidle;

	/* try_to_wake_up() stats */
	unsigned पूर्णाङ्क		ttwu_गणनम्;
	unsigned पूर्णाङ्क		ttwu_local;
#endif

#ifdef CONFIG_CPU_IDLE
	/* Must be inspected within a RCU lock section */
	संरचना cpuidle_state	*idle_state;
#endif

	unsigned पूर्णाङ्क		nr_pinned;
	unsigned पूर्णाङ्क		प्रक्षिप_busy;
	संरचना cpu_stop_work	प्रक्षिप_work;

#ifdef CONFIG_SCHED_CORE
	/* per rq */
	संरचना rq		*core;
	संरचना task_struct	*core_pick;
	संरचना sched_dl_entity	*core_dl_server;
	unsigned पूर्णाङ्क		core_enabled;
	संरचना rb_मूलम्		core_वृक्षः;

	/* shared state -- careful with sched_core_cpu_deactivate() */
	unsigned पूर्णाङ्क		core_task_seq;
	unsigned पूर्णाङ्क		core_pick_seq;
	unsigned long		core_cookie;
	unsigned पूर्णाङ्क		core_forceidle_गणनम्;
	unsigned पूर्णाङ्क		core_forceidle_seq;
	unsigned पूर्णाङ्क		core_forceidle_occupation;
	u64			core_forceidle_start;
	unsigned पूर्णाङ्क		core_pick_in_flight;
#endif /* CONFIG_SCHED_CORE */

	/* Scratch cpumask to be temporarily used under rq_lock */
	cpumask_var_t		scratch_mask;

#ifdef CONFIG_CFS_BANDWIDTH
	call_single_data_t	cfsb_csd;
	संरचना list_head	cfsb_csd_list;
#endif

	atomic_t		nr_iowait;
} __no_randomize_layout;

#ifdef CONFIG_FAIR_GROUP_SCHED

/* CPU runqueue to which this cfs_rq is attached */
स्थिर अन्तःस्थ संरचना rq *rq_of(संरचना cfs_rq *cfs_rq)
{
	प्रत्यागम cfs_rq->rq;
}

#अण्णहा /* !CONFIG_FAIR_GROUP_SCHED: */

स्थिर अन्तःस्थ संरचना rq *rq_of(संरचना cfs_rq *cfs_rq)
{
	प्रत्यागम container_of(cfs_rq, संरचना rq, cfs);
}
#endif /* !CONFIG_FAIR_GROUP_SCHED */

स्थिर अन्तःस्थ पूर्णाङ्क cpu_of(संरचना rq *rq)
{
	प्रत्यागम rq->cpu;
}

#define MDF_प्रक्षिप		0x01

स्थिर अन्तःस्थ bool is_migration_disabled(संरचना task_struct *p)
{
	प्रत्यागम p->migration_disabled;
}

DECLARE_PER_CPU_SHARED_ALIGNED(संरचना rq, runqueues);
DECLARE_PER_CPU(संरचना rnd_state, sched_rnd_state);

स्थिर अन्तःस्थ u32 sched_rng(शून्य)
{
	प्रत्यागम prandom_u32_state(this_cpu_ptr(&sched_rnd_state));
}

स्थिर __always_inline संरचना rq *__this_rq(शून्य)
{
	प्रत्यागम this_cpu_ptr(&runqueues);
}

#define cpu_rq(cpu)		(&per_cpu(runqueues, (cpu)))
#define this_rq()		__this_rq()
#define task_rq(p)		cpu_rq(task_cpu(p))
#define cpu_curr(cpu)		(cpu_rq(cpu)->curr)
#define raw_rq()		raw_cpu_ptr(&runqueues)

स्थिर अन्तःस्थ bool idle_rq(संरचना rq *rq)
{
	प्रत्यागम rq->curr == rq->idle && !rq->nr_running && !rq->ttwu_pending;
}

/**
 * available_idle_cpu - is a given CPU idle for enqueuing work.
 * @cpu: the CPU in question.
 *
 * Return: 1 if the CPU is currently idle. 0 otherwise.
 */
स्थिर अन्तःस्थ bool available_idle_cpu(पूर्णाङ्क cpu)
{
	जइ (!idle_rq(cpu_rq(cpu)))
		प्रत्यागम 0;

	जइ (vcpu_is_preempted(cpu))
		प्रत्यागम 0;

	प्रत्यागम 1;
}

#ifdef CONFIG_SCHED_PROXY_EXEC
स्थिर अन्तःस्थ शून्य rq_set_donor(संरचना rq *rq, संरचना task_struct *t)
{
	rcu_assign_pointer(rq->donor, t);
}
#अण्णहा
स्थिर अन्तःस्थ शून्य rq_set_donor(संरचना rq *rq, संरचना task_struct *t)
{
	/* Do nothing */
}
#endif

#ifdef CONFIG_SCHED_CORE
स्थिर अन्तःस्थ संरचना cpumask *sched_group_span(संरचना sched_group *sg);

DECLARE_STATIC_कुञ्चिका_FALSE(__sched_core_enabled);

स्थिर अन्तःस्थ bool sched_core_enabled(संरचना rq *rq)
{
	प्रत्यागम static_branch_unlikely(&__sched_core_enabled) && rq->core_enabled;
}

स्थिर अन्तःस्थ bool sched_core_disabled(शून्य)
{
	प्रत्यागम !static_branch_unlikely(&__sched_core_enabled);
}

/*
 * Be careful with this function; not for general use. The return value isn't
 * stable unless you actually hold a relevant rq->__lock.
 */
स्थिर अन्तःस्थ raw_spinlock_t *rq_lockp(संरचना rq *rq)
{
	जइ (sched_core_enabled(rq))
		प्रत्यागम &rq->core->__lock;

	प्रत्यागम &rq->__lock;
}

स्थिर अन्तःस्थ raw_spinlock_t *__rq_lockp(संरचना rq *rq)
	__returns_ctx_lock(rq_lockp(rq)) /* alias them */
{
	जइ (rq->core_enabled)
		प्रत्यागम &rq->core->__lock;

	प्रत्यागम &rq->__lock;
}

बाह्य bool
cfs_prio_less(ध्रुव संरचना task_struct *a, ध्रुव संरचना task_struct *b, bool fi);

बाह्य शून्य task_vruntime_update(संरचना rq *rq, संरचना task_struct *p, bool in_fi);

/*
 * Helpers to check if the CPU's core cookie matches with the task's cookie
 * when core scheduling is enabled.
 * A special case is that the task's cookie always matches with CPU's core
 * cookie if the CPU is in an idle core.
 */
स्थिर अन्तःस्थ bool sched_cpu_cookie_match(संरचना rq *rq, संरचना task_struct *p)
{
	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	जइ (!sched_core_enabled(rq))
		प्रत्यागम true;

	प्रत्यागम rq->core->core_cookie == p->core_cookie;
}

स्थिर अन्तःस्थ bool sched_core_cookie_match(संरचना rq *rq, संरचना task_struct *p)
{
	bool idle_core = true;
	पूर्णाङ्क cpu;

	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	जइ (!sched_core_enabled(rq))
		प्रत्यागम true;

	जइ (rq->core->core_cookie == p->core_cookie)
		प्रत्यागम true;

	for_each_cpu(cpu, cpu_smt_mask(cpu_of(rq))) {
		जइ (!available_idle_cpu(cpu)) {
			idle_core = false;
			भङ्ग;
		}
	}

	/*
	 * A CPU in an idle core is always the best choice for tasks with
	 * cookies.
	 */
	प्रत्यागम idle_core;
}

स्थिर अन्तःस्थ bool sched_group_cookie_match(संरचना rq *rq,
					    संरचना task_struct *p,
					    संरचना sched_group *group)
{
	पूर्णाङ्क cpu;

	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	जइ (!sched_core_enabled(rq))
		प्रत्यागम true;

	for_each_cpu_and(cpu, sched_group_span(group), p->cpus_ptr) {
		जइ (sched_core_cookie_match(cpu_rq(cpu), p))
			प्रत्यागम true;
	}
	प्रत्यागम false;
}

स्थिर अन्तःस्थ bool sched_core_enqueued(संरचना task_struct *p)
{
	प्रत्यागम !RB_EMPTY_ग्रन्थिः(&p->core_ग्रन्थिः);
}

बाह्य शून्य sched_core_enqueue(संरचना rq *rq, संरचना task_struct *p);
बाह्य शून्य sched_core_dequeue(संरचना rq *rq, संरचना task_struct *p, पूर्णाङ्क flags);

बाह्य शून्य sched_core_get(शून्य);
बाह्य शून्य sched_core_put(शून्य);

स्थिर अन्तःस्थ bool task_has_sched_core(संरचना task_struct *p)
{
	जइ (sched_core_disabled())
		प्रत्यागम false;

	प्रत्यागम !!p->core_cookie;
}

#अण्णहा /* !CONFIG_SCHED_CORE: */

स्थिर अन्तःस्थ bool sched_core_enabled(संरचना rq *rq)
{
	प्रत्यागम false;
}

स्थिर अन्तःस्थ bool sched_core_disabled(शून्य)
{
	प्रत्यागम true;
}

स्थिर अन्तःस्थ raw_spinlock_t *rq_lockp(संरचना rq *rq)
{
	प्रत्यागम &rq->__lock;
}

स्थिर अन्तःस्थ raw_spinlock_t *__rq_lockp(संरचना rq *rq)
	__returns_ctx_lock(rq_lockp(rq)) /* alias them */
{
	प्रत्यागम &rq->__lock;
}

स्थिर अन्तःस्थ bool sched_cpu_cookie_match(संरचना rq *rq, संरचना task_struct *p)
{
	प्रत्यागम true;
}

स्थिर अन्तःस्थ bool sched_core_cookie_match(संरचना rq *rq, संरचना task_struct *p)
{
	प्रत्यागम true;
}

स्थिर अन्तःस्थ bool sched_group_cookie_match(संरचना rq *rq,
					    संरचना task_struct *p,
					    संरचना sched_group *group)
{
	प्रत्यागम true;
}

स्थिर अन्तःस्थ bool task_has_sched_core(संरचना task_struct *p)
{
	प्रत्यागम false;
}

#endif /* !CONFIG_SCHED_CORE */

#ifdef CONFIG_RT_GROUP_SCHED
# ifdef CONFIG_RT_GROUP_SCHED_DEFAULT_DISABLED
DECLARE_STATIC_कुञ्चिका_FALSE(rt_group_sched);
स्थिर अन्तःस्थ bool rt_group_sched_enabled(शून्य)
{
	प्रत्यागम static_branch_unlikely(&rt_group_sched);
}
# अण्णहा /* !CONFIG_RT_GROUP_SCHED_DEFAULT_DISABLED: */
DECLARE_STATIC_कुञ्चिका_TRUE(rt_group_sched);
स्थिर अन्तःस्थ bool rt_group_sched_enabled(शून्य)
{
	प्रत्यागम static_branch_likely(&rt_group_sched);
}
# endif /* !CONFIG_RT_GROUP_SCHED_DEFAULT_DISABLED */
#अण्णहा /* !CONFIG_RT_GROUP_SCHED: */
# define rt_group_sched_enabled()	false
#endif /* !CONFIG_RT_GROUP_SCHED */

स्थिर अन्तःस्थ शून्य lockdep_assert_rq_held(संरचना rq *rq)
	__assumes_ctx_lock(__rq_lockp(rq))
{
	lockdep_assert_held(__rq_lockp(rq));
}

बाह्य शून्य raw_spin_rq_lock_nested(संरचना rq *rq, पूर्णाङ्क subclass)
	__acquires(__rq_lockp(rq));

बाह्य bool raw_spin_rq_trylock(संरचना rq *rq)
	__cond_acquires(true, __rq_lockp(rq));

स्थिर अन्तःस्थ शून्य raw_spin_rq_lock(संरचना rq *rq)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_lock_nested(rq, 0);
}

स्थिर अन्तःस्थ शून्य raw_spin_rq_unlock(संरचना rq *rq)
	__releases(__rq_lockp(rq))
{
	raw_spin_unlock(rq_lockp(rq));
}

स्थिर अन्तःस्थ शून्य raw_spin_rq_lock_irq(संरचना rq *rq)
	__acquires(__rq_lockp(rq))
{
	local_irq_disable();
	raw_spin_rq_lock(rq);
}

स्थिर अन्तःस्थ शून्य raw_spin_rq_unlock_irq(संरचना rq *rq)
	__releases(__rq_lockp(rq))
{
	raw_spin_rq_unlock(rq);
	local_irq_enable();
}

स्थिर अन्तःस्थ unsigned long _raw_spin_rq_lock_irqsave(संरचना rq *rq)
	__acquires(__rq_lockp(rq))
{
	unsigned long flags;

	local_irq_save(flags);
	raw_spin_rq_lock(rq);

	प्रत्यागम flags;
}

स्थिर अन्तःस्थ शून्य raw_spin_rq_unlock_irqrestore(संरचना rq *rq, unsigned long flags)
	__releases(__rq_lockp(rq))
{
	raw_spin_rq_unlock(rq);
	local_irq_restore(flags);
}

#define raw_spin_rq_lock_irqsave(rq, flags)	\
करेहि {						\
	flags = _raw_spin_rq_lock_irqsave(rq);	\
} जाव (0)

बाह्य शून्य __update_idle_core(संरचना rq *rq);

स्थिर अन्तःस्थ शून्य update_idle_core(संरचना rq *rq)
{
	जइ (sched_smt_active())
		__update_idle_core(rq);
}

#ifdef CONFIG_FAIR_GROUP_SCHED
स्थिर अन्तःस्थ संरचना task_struct *task_of(संरचना sched_entity *se)
{
	WARN_ON_ONCE(!entity_is_task(se));
	प्रत्यागम container_of(se, संरचना task_struct, se);
}

स्थिर अन्तःस्थ संरचना cfs_rq *task_cfs_rq(संरचना task_struct *p)
{
	प्रत्यागम p->se.cfs_rq;
}

/* runqueue on which this entity is (to be) queued */
स्थिर अन्तःस्थ संरचना cfs_rq *cfs_rq_of(ध्रुव संरचना sched_entity *se)
{
	प्रत्यागम se->cfs_rq;
}

/* runqueue "owned" by this group */
स्थिर अन्तःस्थ संरचना cfs_rq *group_cfs_rq(संरचना sched_entity *grp)
{
	प्रत्यागम grp->my_q;
}

#अण्णहा /* !CONFIG_FAIR_GROUP_SCHED: */

#define task_of(_se)		container_of(_se, संरचना task_struct, se)

स्थिर अन्तःस्थ संरचना cfs_rq *task_cfs_rq(ध्रुव संरचना task_struct *p)
{
	प्रत्यागम &task_rq(p)->cfs;
}

स्थिर अन्तःस्थ संरचना cfs_rq *cfs_rq_of(ध्रुव संरचना sched_entity *se)
{
	ध्रुव संरचना task_struct *p = task_of(se);
	संरचना rq *rq = task_rq(p);

	प्रत्यागम &rq->cfs;
}

/* runqueue "owned" by this group */
स्थिर अन्तःस्थ संरचना cfs_rq *group_cfs_rq(संरचना sched_entity *grp)
{
	प्रत्यागम रिक्त;
}

#endif /* !CONFIG_FAIR_GROUP_SCHED */

बाह्य शून्य update_rq_avg_idle(संरचना rq *rq);
बाह्य शून्य update_rq_clock(संरचना rq *rq);

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

स्थिर अन्तःस्थ शून्य assert_clock_updated(संरचना rq *rq)
{
	/*
	 * The only reason for not seeing a clock update since the
	 * last rq_pin_lock() is if we're currently skipping updates.
	 */
	WARN_ON_ONCE(rq->clock_update_flags < RQCF_ACT_SKIP);
}

स्थिर अन्तःस्थ u64 rq_clock(संरचना rq *rq)
{
	lockdep_assert_rq_held(rq);
	assert_clock_updated(rq);

	प्रत्यागम rq->clock;
}

स्थिर अन्तःस्थ u64 rq_clock_task(संरचना rq *rq)
{
	lockdep_assert_rq_held(rq);
	assert_clock_updated(rq);

	प्रत्यागम rq->clock_task;
}

स्थिर अन्तःस्थ शून्य rq_clock_skip_update(संरचना rq *rq)
{
	lockdep_assert_rq_held(rq);
	rq->clock_update_flags |= RQCF_REQ_SKIP;
}

/*
 * See rt task throttling, which is the only time a skip
 * request is canceled.
 */
स्थिर अन्तःस्थ शून्य rq_clock_cancel_skipupdate(संरचना rq *rq)
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
स्थिर अन्तःस्थ शून्य rq_clock_start_loop_update(संरचना rq *rq)
{
	lockdep_assert_rq_held(rq);
	WARN_ON_ONCE(rq->clock_update_flags & RQCF_ACT_SKIP);
	rq->clock_update_flags |= RQCF_ACT_SKIP;
}

स्थिर अन्तःस्थ शून्य rq_clock_stop_loop_update(संरचना rq *rq)
{
	lockdep_assert_rq_held(rq);
	rq->clock_update_flags &= ~RQCF_ACT_SKIP;
}

संरचना rq_flags {
	unsigned long flags;
	संरचना pin_cookie cookie;
	/*
	 * A copy of (rq::clock_update_flags & RQCF_UPDATED) for the
	 * current pin context is stashed here in case it needs to be
	 * restored in rq_repin_lock().
	 */
	unsigned पूर्णाङ्क clock_update_flags;
};

बाह्य संरचना balance_callback balance_प्रक्षिप_callback;

#ifdef CONFIG_SCHED_CLASS_EXT
बाह्य ध्रुव संरचना sched_class ext_sched_class;

DECLARE_STATIC_कुञ्चिका_FALSE(__scx_enabled);	/* SCX BPF scheduler loaded */
DECLARE_STATIC_कुञ्चिका_FALSE(__scx_switched_all);	/* all fair class tasks on SCX */

#define scx_enabled()		static_branch_unlikely(&__scx_enabled)
#define scx_switched_all()	static_branch_unlikely(&__scx_switched_all)

स्थिर अन्तःस्थ शून्य scx_rq_clock_update(संरचना rq *rq, u64 clock)
{
	जइ (!scx_enabled())
		प्रत्यागम;
	WRITE_ONCE(rq->scx.clock, clock);
	smp_store_release(&rq->scx.flags, rq->scx.flags | SCX_RQ_CLK_VALID);
}

स्थिर अन्तःस्थ शून्य scx_rq_clock_invalidate(संरचना rq *rq)
{
	जइ (!scx_enabled())
		प्रत्यागम;
	WRITE_ONCE(rq->scx.flags, rq->scx.flags & ~SCX_RQ_CLK_VALID);
}

#अण्णहा /* !CONFIG_SCHED_CLASS_EXT: */
#define scx_enabled()		false
#define scx_switched_all()	false

स्थिर अन्तःस्थ शून्य scx_rq_clock_update(संरचना rq *rq, u64 clock) {}
स्थिर अन्तःस्थ शून्य scx_rq_clock_invalidate(संरचना rq *rq) {}
#endif /* !CONFIG_SCHED_CLASS_EXT */

स्थिर अन्तःस्थ शून्य assert_balance_callbacks_empty(संरचना rq *rq)
{
	WARN_ON_ONCE(IS_ENABLED(CONFIG_PROVE_LOCKING) &&
		     rq->balance_callback &&
		     rq->balance_callback != &balance_प्रक्षिप_callback);
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
स्थिर अन्तःस्थ शून्य rq_pin_lock(संरचना rq *rq, संरचना rq_flags *rf)
{
	rf->cookie = lockdep_pin_lock(__rq_lockp(rq));

	rq->clock_update_flags &= (RQCF_REQ_SKIP|RQCF_ACT_SKIP);
	rf->clock_update_flags = 0;
	assert_balance_callbacks_empty(rq);
}

स्थिर अन्तःस्थ शून्य rq_unpin_lock(संरचना rq *rq, संरचना rq_flags *rf)
{
	जइ (rq->clock_update_flags > RQCF_ACT_SKIP)
		rf->clock_update_flags = RQCF_UPDATED;

	scx_rq_clock_invalidate(rq);
	lockdep_unpin_lock(__rq_lockp(rq), rf->cookie);
}

स्थिर अन्तःस्थ शून्य rq_repin_lock(संरचना rq *rq, संरचना rq_flags *rf)
{
	lockdep_repin_lock(__rq_lockp(rq), rf->cookie);

	/*
	 * Restore the value we stashed in @rf for this pin context.
	 */
	rq->clock_update_flags |= rf->clock_update_flags;
}

#define __task_rq_lock(...) __acquire_ret(___task_rq_lock(__VA_ARGS__), __rq_lockp(__ret))
बाह्य संरचना rq *___task_rq_lock(संरचना task_struct *p, संरचना rq_flags *rf) __acquires_ret;

#define task_rq_lock(...) __acquire_ret(_task_rq_lock(__VA_ARGS__), __rq_lockp(__ret))
बाह्य संरचना rq *_task_rq_lock(संरचना task_struct *p, संरचना rq_flags *rf)
	__acquires(&p->pi_lock) __acquires_ret;

स्थिर अन्तःस्थ शून्य
__task_rq_unlock(संरचना rq *rq, संरचना task_struct *p, संरचना rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_lock(rq, rf);
	raw_spin_rq_unlock(rq);
}

स्थिर अन्तःस्थ शून्य
task_rq_unlock(संरचना rq *rq, संरचना task_struct *p, संरचना rq_flags *rf)
	__releases(__rq_lockp(rq), &p->pi_lock)
{
	__task_rq_unlock(rq, p, rf);
	raw_spin_unlock_irqrestore(&p->pi_lock, rf->flags);
}

DEFINE_LOCK_GUARD_1(task_rq_lock, संरचना task_struct,
		    _T->rq = task_rq_lock(_T->lock, &_T->rf),
		    task_rq_unlock(_T->rq, _T->lock, &_T->rf),
		    संरचना rq *rq; संरचना rq_flags rf)
DECLARE_LOCK_GUARD_1_ATTRS(task_rq_lock, __acquires(_T->pi_lock), __releases((*(संरचना task_struct **)_T)->pi_lock))
#define class_task_rq_lock_constructor(_T) WITH_LOCK_GUARD_1_ATTRS(task_rq_lock, _T)

DEFINE_LOCK_GUARD_1(__task_rq_lock, संरचना task_struct,
		    _T->rq = __task_rq_lock(_T->lock, &_T->rf),
		    __task_rq_unlock(_T->rq, _T->lock, &_T->rf),
		    संरचना rq *rq; संरचना rq_flags rf)

स्थिर अन्तःस्थ शून्य rq_lock_irqsave(संरचना rq *rq, संरचना rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_lock_irqsave(rq, rf->flags);
	rq_pin_lock(rq, rf);
}

स्थिर अन्तःस्थ शून्य rq_lock_irq(संरचना rq *rq, संरचना rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_lock_irq(rq);
	rq_pin_lock(rq, rf);
}

स्थिर अन्तःस्थ शून्य rq_lock(संरचना rq *rq, संरचना rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_lock(rq);
	rq_pin_lock(rq, rf);
}

स्थिर अन्तःस्थ शून्य rq_unlock_irqrestore(संरचना rq *rq, संरचना rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_lock(rq, rf);
	raw_spin_rq_unlock_irqrestore(rq, rf->flags);
}

स्थिर अन्तःस्थ शून्य rq_unlock_irq(संरचना rq *rq, संरचना rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_lock(rq, rf);
	raw_spin_rq_unlock_irq(rq);
}

स्थिर अन्तःस्थ शून्य rq_unlock(संरचना rq *rq, संरचना rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_lock(rq, rf);
	raw_spin_rq_unlock(rq);
}

DEFINE_LOCK_GUARD_1(rq_lock, संरचना rq,
		    rq_lock(_T->lock, &_T->rf),
		    rq_unlock(_T->lock, &_T->rf),
		    संरचना rq_flags rf)

DECLARE_LOCK_GUARD_1_ATTRS(rq_lock, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(संरचना rq **)_T)));
#define class_rq_lock_constructor(_T) WITH_LOCK_GUARD_1_ATTRS(rq_lock, _T)

DEFINE_LOCK_GUARD_1(rq_lock_irq, संरचना rq,
		    rq_lock_irq(_T->lock, &_T->rf),
		    rq_unlock_irq(_T->lock, &_T->rf),
		    संरचना rq_flags rf)

DECLARE_LOCK_GUARD_1_ATTRS(rq_lock_irq, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(संरचना rq **)_T)));
#define class_rq_lock_irq_constructor(_T) WITH_LOCK_GUARD_1_ATTRS(rq_lock_irq, _T)

DEFINE_LOCK_GUARD_1(rq_lock_irqsave, संरचना rq,
		    rq_lock_irqsave(_T->lock, &_T->rf),
		    rq_unlock_irqrestore(_T->lock, &_T->rf),
		    संरचना rq_flags rf)

DECLARE_LOCK_GUARD_1_ATTRS(rq_lock_irqsave, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(संरचना rq **)_T)));
#define class_rq_lock_irqsave_constructor(_T) WITH_LOCK_GUARD_1_ATTRS(rq_lock_irqsave, _T)

DEFINE_LOCK_GUARD_1(raw_spin_rq_lock_irqsave, संरचना rq,
		    raw_spin_rq_lock_irqsave(_T->lock, _T->flags),
		    raw_spin_rq_unlock_irqrestore(_T->lock, _T->flags),
		    unsigned long flags)

DECLARE_LOCK_GUARD_1_ATTRS(raw_spin_rq_lock_irqsave, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(संरचना rq **)_T)));
#define class_raw_spin_rq_lock_irqsave_constructor(_T) \
	WITH_LOCK_GUARD_1_ATTRS(raw_spin_rq_lock_irqsave, _T)

#define this_rq_lock_irq(...) __acquire_ret(_this_rq_lock_irq(__VA_ARGS__), __rq_lockp(__ret))
स्थिर अन्तःस्थ संरचना rq *_this_rq_lock_irq(संरचना rq_flags *rf) __acquires_ret
{
	संरचना rq *rq;

	local_irq_disable();
	rq = this_rq();
	rq_lock(rq, rf);

	प्रत्यागम rq;
}

#ifdef CONFIG_NUMA

गणना numa_topology_type {
	NUMA_DIRECT,
	NUMA_GLUELESS_MESH,
	NUMA_BACKPLANE,
};

बाह्य गणना numa_topology_type sched_numa_topology_type;
बाह्य पूर्णाङ्क sched_max_numa_distance;
बाह्य bool find_numa_distance(पूर्णाङ्क distance);
बाह्य शून्य sched_init_numa(पूर्णाङ्क offline_ग्रन्थिः);
बाह्य शून्य sched_update_numa(पूर्णाङ्क cpu, bool online);
बाह्य शून्य sched_domains_numa_masks_set(unsigned पूर्णाङ्क cpu);
बाह्य शून्य sched_domains_numa_masks_clear(unsigned पूर्णाङ्क cpu);
बाह्य पूर्णाङ्क sched_numa_find_closest(ध्रुव संरचना cpumask *cpus, पूर्णाङ्क cpu);

#अण्णहा /* !CONFIG_NUMA: */

स्थिर अन्तःस्थ शून्य sched_init_numa(पूर्णाङ्क offline_ग्रन्थिः) { }
स्थिर अन्तःस्थ शून्य sched_update_numa(पूर्णाङ्क cpu, bool online) { }
स्थिर अन्तःस्थ शून्य sched_domains_numa_masks_set(unsigned पूर्णाङ्क cpu) { }
स्थिर अन्तःस्थ शून्य sched_domains_numa_masks_clear(unsigned पूर्णाङ्क cpu) { }

स्थिर अन्तःस्थ पूर्णाङ्क sched_numa_find_closest(ध्रुव संरचना cpumask *cpus, पूर्णाङ्क cpu)
{
	प्रत्यागम nr_cpu_ids;
}

#endif /* !CONFIG_NUMA */

#ifdef CONFIG_NUMA_BALANCING

/* The regions in numa_faults array from task_struct */
गणना numa_faults_stats {
	NUMA_MEM = 0,
	NUMA_CPU,
	NUMA_MEMBUF,
	NUMA_CPUBUF
};

बाह्य शून्य sched_setnuma(संरचना task_struct *p, पूर्णाङ्क ग्रन्थिः);
बाह्य पूर्णाङ्क migrate_task_to(संरचना task_struct *p, पूर्णाङ्क cpu);
बाह्य पूर्णाङ्क migrate_swap(संरचना task_struct *p, संरचना task_struct *t,
			पूर्णाङ्क cpu, पूर्णाङ्क scpu);
बाह्य शून्य init_numa_balancing(u64 clone_flags, संरचना task_struct *p);

#अण्णहा /* !CONFIG_NUMA_BALANCING: */

स्थिर अन्तःस्थ शून्य
init_numa_balancing(u64 clone_flags, संरचना task_struct *p)
{
}

#endif /* !CONFIG_NUMA_BALANCING */

पूर्णाङ्क task_llc(ध्रुव संरचना task_struct *p);

स्थिर अन्तःस्थ शून्य
queue_balance_callback(संरचना rq *rq,
		       संरचना balance_callback *head,
		       शून्य (*func)(संरचना rq *rq))
{
	lockdep_assert_rq_held(rq);

	/*
	 * Don't (re)queue an already queued item; nor queue anything when
	 * balance_push() is active, see the comment with
	 * balance_push_callback.
	 */
	जइ (unlikely(head->next || rq->balance_callback == &balance_प्रक्षिप_callback))
		प्रत्यागम;

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
	प्रति (__sd = rcu_dereference_sched_domain(cpu_rq(cpu)->sd); \
			__sd; __sd = __sd->parent)

/* A mask of all the SD flags that have the SDF_SHARED_CHILD metaflag */
#define SD_FLAG(name, mflags) (name * !!((mflags) & SDF_SHARED_CHILD)) |
स्थिर ध्रुव unsigned पूर्णाङ्क SD_SHARED_CHILD_MASK =
#अन्तर्भाव <linux/sched/sd_flags.h>
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
स्थिर अन्तःस्थ संरचना sched_domain *highest_flag_domain(पूर्णाङ्क cpu, पूर्णाङ्क flag)
{
	संरचना sched_domain *sd, *hsd = रिक्त;

	for_each_domain(cpu, sd) {
		जइ (sd->flags & flag) {
			hsd = sd;
			अनुवर्त;
		}

		/*
		 * Stop the search if @flag is known to be shared at lower
		 * levels. It will not be found further up.
		 */
		जइ (flag & SD_SHARED_CHILD_MASK)
			भङ्ग;
	}

	प्रत्यागम hsd;
}

स्थिर अन्तःस्थ संरचना sched_domain *lowest_flag_domain(पूर्णाङ्क cpu, पूर्णाङ्क flag)
{
	संरचना sched_domain *sd;

	for_each_domain(cpu, sd) {
		जइ (sd->flags & flag)
			भङ्ग;
	}

	प्रत्यागम sd;
}

DECLARE_PER_CPU(संरचना sched_domain __rcu *, sd_llc);
DECLARE_PER_CPU(पूर्णाङ्क, sd_llc_size);
DECLARE_PER_CPU(पूर्णाङ्क, sd_llc_id);
DECLARE_PER_CPU(पूर्णाङ्क, sd_share_id);
DECLARE_PER_CPU(संरचना sched_domain_shared __rcu *, sd_llc_shared);
DECLARE_PER_CPU(संरचना sched_domain_shared __rcu *, sd_balance_shared);
DECLARE_PER_CPU(संरचना sched_domain __rcu *, sd_numa);
DECLARE_PER_CPU(संरचना sched_domain __rcu *, sd_asym_packing);
DECLARE_PER_CPU(संरचना sched_domain __rcu *, sd_asym_cpucapacity);

बाह्य संरचना static_कुञ्चिका_false sched_asym_cpucapacity;
बाह्य संरचना static_कुञ्चिका_false sched_cluster_active;

स्थिर __always_inline bool sched_asym_cpucap_active(शून्य)
{
	प्रत्यागम static_branch_unlikely(&sched_asym_cpucapacity);
}

संरचना sched_group_capacity {
	atomic_t		ref;
	/*
	 * CPU capacity of this group, SCHED_CAPACITY_SCALE being max capacity
	 * for a single CPU.
	 */
	unsigned long		capacity;
	unsigned long		min_capacity;		/* Min per-CPU capacity in group */
	unsigned long		max_capacity;		/* Max per-CPU capacity in group */
	unsigned long		next_update;
	पूर्णाङ्क			imbalance;		/* XXX unrelated to capacity but shared group state */

	पूर्णाङ्क			id;

	unsigned long		cpumask[];		/* Balance mask */
};

संरचना sched_group {
	संरचना sched_group	*next;			/* Must be a circular list */
	atomic_t		ref;

	unsigned पूर्णाङ्क		group_weight;
	unsigned पूर्णाङ्क		cores;
	संरचना sched_group_capacity *sgc;
	पूर्णाङ्क			asym_prefer_cpu;	/* CPU of highest priority in group */
	पूर्णाङ्क			flags;

	/*
	 * The CPUs this group covers.
	 *
	 * NOTE: this field is variable length. (Allocated dynamically
	 * by attaching extra space to the end of the structure,
	 * depending on how many CPUs the kernel has booted up with)
	 */
	unsigned long		cpumask[];
};

स्थिर अन्तःस्थ संरचना cpumask *sched_group_span(संरचना sched_group *sg)
{
	प्रत्यागम to_cpumask(sg->cpumask);
}

/*
 * See build_balance_mask().
 */
स्थिर अन्तःस्थ संरचना cpumask *group_balance_mask(संरचना sched_group *sg)
{
	प्रत्यागम to_cpumask(sg->sgc->cpumask);
}

बाह्य पूर्णाङ्क group_balance_cpu(संरचना sched_group *sg);

बाह्य शून्य update_sched_domain_debugfs(शून्य);
बाह्य शून्य dirty_sched_domain_sysctl(पूर्णाङ्क cpu);

बाह्य पूर्णाङ्क sched_update_scaling(शून्य);

स्थिर अन्तःस्थ ध्रुव संरचना cpumask *task_user_cpus(संरचना task_struct *p)
{
	जइ (!p->user_cpus_ptr)
		प्रत्यागम cpu_possible_mask; /* &init_task.cpus_mask */
	प्रत्यागम p->user_cpus_ptr;
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
स्थिर अन्तःस्थ संरचना task_group *task_group(संरचना task_struct *p)
{
	प्रत्यागम p->sched_task_group;
}

#ifdef CONFIG_FAIR_GROUP_SCHED
/*
 * Defined here to be available before stats.h is included, since
 * stats.h has dependencies on things defined later in this file.
 */
संरचना cfs_tg_state {
	संरचना cfs_rq		cfs_rq;
	संरचना sched_entity	se;
	संरचना sched_statistics	stats;
} __no_randomize_layout;

/* Access a specific CPU's cfs_rq from a task group */
स्थिर अन्तःस्थ संरचना cfs_rq *tg_cfs_rq(संरचना task_group *tg, पूर्णाङ्क cpu)
{
	प्रत्यागम per_cpu_ptr(tg->cfs_rq, cpu);
}

स्थिर अन्तःस्थ संरचना sched_entity *tg_se(संरचना task_group *tg, पूर्णाङ्क cpu)
{
	संरचना cfs_tg_state *state;

	जइ (is_मूलम्_task_group(tg))
		प्रत्यागम रिक्त;

	state = container_of(tg_cfs_rq(tg, cpu), संरचना cfs_tg_state, cfs_rq);
	प्रत्यागम &state->se;
}

स्थिर अन्तःस्थ संरचना sched_entity *cfs_rq_se(संरचना cfs_rq *cfs_rq)
{
	संरचना cfs_tg_state *state;

	जइ (is_मूलम्_task_group(cfs_rq->tg))
		प्रत्यागम रिक्त;

	state = container_of(cfs_rq, संरचना cfs_tg_state, cfs_rq);
	प्रत्यागम &state->se;
}
#endif

/* Change a task's cfs_rq and parent entity if it moves across CPUs/groups */
स्थिर अन्तःस्थ शून्य set_task_rq(संरचना task_struct *p, unsigned पूर्णाङ्क cpu)
{
#जइ defined(CONFIG_FAIR_GROUP_SCHED) || defined(CONFIG_RT_GROUP_SCHED)
	संरचना task_group *tg = task_group(p);
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
	जइ (!rt_group_sched_enabled())
		tg = &मूलम्_task_group;
	p->rt.rt_rq  = tg->rt_rq[cpu];
	p->rt.parent = tg->rt_se[cpu];
#endif /* CONFIG_RT_GROUP_SCHED */
}

#अण्णहा /* !CONFIG_CGROUP_SCHED: */

स्थिर अन्तःस्थ शून्य set_task_rq(संरचना task_struct *p, unsigned पूर्णाङ्क cpu) { }

स्थिर अन्तःस्थ संरचना task_group *task_group(संरचना task_struct *p)
{
	प्रत्यागम रिक्त;
}

#endif /* !CONFIG_CGROUP_SCHED */

स्थिर अन्तःस्थ शून्य __set_task_cpu(संरचना task_struct *p, unsigned पूर्णाङ्क cpu)
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

गणना {
#अन्तर्भाव "features.h"
	__SCHED_FEAT_NR,
};

#undef SCHED_FEAT

/*
 * To support run-time toggling of sched features, all the translation units
 * (but core.c) reference the sysctl_sched_features defined in core.c.
 */
बाह्य __read_mostly unsigned पूर्णाङ्क sysctl_sched_features;

#ifdef CONFIG_JUMP_LABEL

सङ्घ sched_feat_कुञ्चिका {
	संरचना static_कुञ्चिका_true	कुञ्चिका_true;
	संरचना static_कुञ्चिका_false	कुञ्चिका_false;
};

#define sched_feat_branch_true(कुञ्चिका)	static_branch_likely(&(कुञ्चिका)->कुञ्चिका_true)
#define sched_feat_branch_false(कुञ्चिका)	static_branch_unlikely(&(कुञ्चिका)->कुञ्चिका_false)

#define SCHED_FEAT(name, enabled)			\
स्थिर __always_inline bool				\
static_branch_##name(सङ्घ sched_feat_कुञ्चिका *कुञ्चिका) 	\
{							\
	प्रत्यागम sched_feat_branch_##enabled(कुञ्चिका);	\
}

#अन्तर्भाव "features.h"
#undef SCHED_FEAT

बाह्य सङ्घ sched_feat_कुञ्चिका sched_feat_कुञ्चिकाः[__SCHED_FEAT_NR];
#define sched_feat(x) (static_branch_##x(&sched_feat_कुञ्चिकाः[__SCHED_FEAT_##x]))

#अण्णहा /* !CONFIG_JUMP_LABEL: */

#define sched_feat(x) (sysctl_sched_features & (1UL << __SCHED_FEAT_##x))

#endif /* !CONFIG_JUMP_LABEL */

बाह्य संरचना static_कुञ्चिका_false sched_numa_balancing;
बाह्य संरचना static_कुञ्चिका_false sched_schedstats;

स्थिर अन्तःस्थ u64 global_rt_period(शून्य)
{
	प्रत्यागम (u64)sysctl_sched_rt_period * NSEC_PER_USEC;
}

स्थिर अन्तःस्थ u64 global_rt_runtime(शून्य)
{
	जइ (sysctl_sched_rt_runtime < 0)
		प्रत्यागम RUNTIME_INF;

	प्रत्यागम (u64)sysctl_sched_rt_runtime * NSEC_PER_USEC;
}

/*
 * Is p the current execution context?
 */
स्थिर अन्तःस्थ पूर्णाङ्क task_current(संरचना rq *rq, संरचना task_struct *p)
{
	प्रत्यागम rq->curr == p;
}

/*
 * Is p the current scheduling context?
 *
 * Note that it might be the current execution context at the same time if
 * rq->curr == rq->donor == p.
 */
स्थिर अन्तःस्थ पूर्णाङ्क task_current_donor(संरचना rq *rq, संरचना task_struct *p)
{
	प्रत्यागम rq->donor == p;
}

स्थिर अन्तःस्थ bool task_is_blocked(संरचना task_struct *p)
{
	जइ (!sched_proxy_exec())
		प्रत्यागम false;

	प्रत्यागम !!p->blocked_on;
}

#ifdef CONFIG_SCHED_PROXY_EXEC
शून्य sched_proxy_block_task(संरचना rq *rq, संरचना task_struct *p);
#अण्णहा
स्थिर अन्तःस्थ शून्य sched_proxy_block_task(संरचना rq *rq, संरचना task_struct *p) {}
#endif

स्थिर अन्तःस्थ पूर्णाङ्क task_on_cpu(संरचना rq *rq, संरचना task_struct *p)
{
	प्रत्यागम p->on_cpu;
}

स्थिर अन्तःस्थ पूर्णाङ्क task_on_rq_queued(संरचना task_struct *p)
{
	प्रत्यागम READ_ONCE(p->on_rq) == TASK_ON_RQ_QUEUED;
}

स्थिर अन्तःस्थ पूर्णाङ्क task_on_rq_migrating(संरचना task_struct *p)
{
	प्रत्यागम READ_ONCE(p->on_rq) == TASK_ON_RQ_MIGRATING;
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

बाह्य ध्रुव पूर्णाङ्क		sched_prio_to_weight[40];
बाह्य ध्रुव u32		sched_prio_to_wmult[40];

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

#define RETRY_TASK		((शून्य *)-1UL)

संरचना affinity_context {
	ध्रुव संरचना cpumask	*new_mask;
	संरचना cpumask		*user_mask;
	unsigned पूर्णाङ्क		flags;
};

बाह्य s64 update_curr_common(संरचना rq *rq);

गणना snt_e {
	SNT_NORMAL,	/* set_next_task() */
	SNT_PICK,	/* put_prev_set_next_task(): prev != next */
	SNT_REPICK,	/* put_prev_set_next_task(): prev == next */
};

संरचना sched_class {

#ifdef CONFIG_UCLAMP_TASK
	पूर्णाङ्क uclamp_enabled;
#endif

	/*
	 * move_queued_task/activate_task/enqueue_task: rq->lock
	 * ttwu_do_activate/activate_task/enqueue_task: rq->lock
	 * wake_up_new_task/activate_task/enqueue_task: task_rq_lock
	 * ttwu_runnable/enqueue_task: task_rq_lock
	 * proxy_task_current: rq->lock
	 * sched_change_end
	 */
	शून्य (*enqueue_task) (संरचना rq *rq, संरचना task_struct *p, पूर्णाङ्क flags);
	/*
	 * move_queued_task/deactivate_task/dequeue_task: rq->lock
	 * __schedule/block_task/dequeue_task: rq->lock
	 * proxy_task_current: rq->lock
	 * wait_task_inactive: task_rq_lock
	 * sched_change_begin
	 */
	bool (*dequeue_task) (संरचना rq *rq, संरचना task_struct *p, पूर्णाङ्क flags);

	/*
	 * do_sched_yield: rq->lock
	 */
	शून्य (*yield_task)   (संरचना rq *rq);
	/*
	 * yield_to: rq->lock (double)
	 */
	bool (*yield_to_task)(संरचना rq *rq, संरचना task_struct *p);

	/*
	 * move_queued_task: rq->lock
	 * __migrate_swap_task: rq->lock
	 * ttwu_do_activate: rq->lock
	 * ttwu_runnable: task_rq_lock
	 * wake_up_new_task: task_rq_lock
	 */
	शून्य (*wakeup_preempt)(संरचना rq *rq, संरचना task_struct *p, पूर्णाङ्क flags);

	/*
	 * schedule/pick_next_task/prev_balance: rq->lock
	 */
	पूर्णाङ्क (*balance)(संरचना rq *rq, संरचना rq_flags *rf);

	/*
	 * schedule/pick_next_task: rq->lock
	 */
	संरचना task_struct *(*pick_task)(संरचना rq *rq, संरचना rq_flags *rf);

	/*
	 * sched_change:
	 * __schedule: rq->lock
	 */
	शून्य (*put_prev_task)(संरचना rq *rq, संरचना task_struct *p, संरचना task_struct *next);
	शून्य (*set_next_task)(संरचना rq *rq, संरचना task_struct *p, गणना snt_e type);

	/*
	 * select_task_rq: p->pi_lock
	 * sched_exec: p->pi_lock
	 */
	पूर्णाङ्क  (*select_task_rq)(संरचना task_struct *p, पूर्णाङ्क task_cpu, पूर्णाङ्क flags);

	/*
	 * set_task_cpu: p->pi_lock || rq->lock (ttwu like)
	 */
	शून्य (*migrate_task_rq)(संरचना task_struct *p, पूर्णाङ्क new_cpu);

	/*
	 * ttwu_do_activate: rq->lock
	 * wake_up_new_task: task_rq_lock
	 */
	शून्य (*task_woken)(संरचना rq *this_rq, संरचना task_struct *task);

	/*
	 * do_set_cpus_allowed: task_rq_lock + sched_change
	 */
	शून्य (*set_cpus_allowed)(संरचना task_struct *p, संरचना affinity_context *ctx);

	/*
	 * sched_set_rq_{on,off}line: rq->lock
	 */
	शून्य (*rq_online)(संरचना rq *rq);
	शून्य (*rq_offline)(संरचना rq *rq);

	/*
	 * push_cpu_stop: p->pi_lock && rq->lock
	 */
	संरचना rq *(*find_lock_rq)(संरचना task_struct *p, संरचना rq *rq);

	/*
	 * hrtick: rq->lock
	 * sched_tick: rq->lock
	 * sched_tick_remote: rq->lock
	 */
	शून्य (*task_tick)(संरचना rq *rq, संरचना task_struct *p, पूर्णाङ्क queued);
	/*
	 * sched_cgroup_fork: p->pi_lock
	 */
	शून्य (*task_fork)(संरचना task_struct *p);
	/*
	 * finish_task_switch: no locks
	 */
	शून्य (*task_dead)(संरचना task_struct *p);

	/*
	 * sched_change
	 */
	शून्य (*switching_from)(संरचना rq *this_rq, संरचना task_struct *task);
	शून्य (*switched_from) (संरचना rq *this_rq, संरचना task_struct *task);
	शून्य (*switching_to)  (संरचना rq *this_rq, संरचना task_struct *task);
	शून्य (*switched_to)   (संरचना rq *this_rq, संरचना task_struct *task);
	u64  (*get_prio)     (संरचना rq *this_rq, संरचना task_struct *task);
	शून्य (*prio_changed) (संरचना rq *this_rq, संरचना task_struct *task,
			      u64 oldprio);

	/*
	 * set_load_weight: task_rq_lock + sched_change
	 * __setscheduler_parms: task_rq_lock + sched_change
	 */
	शून्य (*reweight_task)(संरचना rq *this_rq, संरचना task_struct *task,
			      ध्रुव संरचना load_weight *lw);

	/*
	 * sched_rr_get_interval: task_rq_lock
	 */
	unsigned पूर्णाङ्क (*get_rr_interval)(संरचना rq *rq,
					संरचना task_struct *task);

	/*
	 * task_sched_runtime: task_rq_lock
	 */
	शून्य (*update_curr)(संरचना rq *rq);

#ifdef CONFIG_FAIR_GROUP_SCHED
	/*
	 * sched_change_group: task_rq_lock + sched_change
	 */
	शून्य (*task_change_group)(संरचना task_struct *p);
#endif

#ifdef CONFIG_SCHED_CORE
	/*
	 * pick_next_task: rq->lock
	 * try_steal_cookie: rq->lock (double)
	 */
	पूर्णाङ्क (*task_is_throttled)(संरचना task_struct *p, पूर्णाङ्क cpu);
#endif
};

स्थिर अन्तःस्थ शून्य put_prev_task(संरचना rq *rq, संरचना task_struct *prev)
{
	WARN_ON_ONCE(rq->donor != prev);
	prev->sched_class->put_prev_task(rq, prev, रिक्त);
}

स्थिर अन्तःस्थ शून्य set_next_task(संरचना rq *rq, संरचना task_struct *next)
{
	next->sched_class->set_next_task(rq, next, SNT_NORMAL);
}

स्थिर अन्तःस्थ शून्य
__put_prev_set_next_dl_server(संरचना rq *rq,
			      संरचना task_struct *prev,
			      संरचना task_struct *next)
{
	prev->dl_server = रिक्त;
	next->dl_server = rq->dl_server;
	rq->dl_server = रिक्त;
}

स्थिर अन्तःस्थ शून्य put_prev_set_next_task(संरचना rq *rq,
					  संरचना task_struct *prev,
					  संरचना task_struct *next)
{
	WARN_ON_ONCE(rq->donor != prev);

	__put_prev_set_next_dl_server(rq, prev, next);

	जइ (next == prev) {
		next->sched_class->set_next_task(rq, next, SNT_REPICK);
		प्रत्यागम;
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
ध्रुव संरचना sched_class name##_sched_class \
	__aligned(__alignof__(संरचना sched_class)) \
	__section("__" #name "_sched_class")

/* Defined in include/asm-generic/vmlinux.lds.h */
बाह्य संरचना sched_class __sched_class_highest[];
बाह्य संरचना sched_class __sched_class_lowest[];

बाह्य ध्रुव संरचना sched_class stop_sched_class;
बाह्य ध्रुव संरचना sched_class dl_sched_class;
बाह्य ध्रुव संरचना sched_class rt_sched_class;
बाह्य ध्रुव संरचना sched_class fair_sched_class;
बाह्य ध्रुव संरचना sched_class idle_sched_class;

/*
 * Iterate only active classes. SCX can take over all fair tasks or be
 * completely disabled. If the former, skip fair. If the latter, skip SCX.
 */
स्थिर अन्तःस्थ ध्रुव संरचना sched_class *next_active_class(ध्रुव संरचना sched_class *class)
{
	class++;
#ifdef CONFIG_SCHED_CLASS_EXT
	जइ (scx_switched_all() && class == &fair_sched_class)
		class++;
	जइ (!scx_enabled() && class == &ext_sched_class)
		class++;
#endif
	प्रत्यागम class;
}

#define for_class_range(class, _from, _to) \
	प्रति (class = (_from); class < (_to); class++)

#define for_each_class(class) \
	for_class_range(class, __sched_class_highest, __sched_class_lowest)

#define for_active_class_range(class, _from, _to)				\
	प्रति (class = (_from); class != (_to); class = next_active_class(class))

#define for_each_active_class(class)						\
	for_active_class_range(class, __sched_class_highest, __sched_class_lowest)

#define sched_class_above(_a, _b)	((_a) < (_b))

स्थिर अन्तःस्थ शून्य rq_modified_begin(संरचना rq *rq, ध्रुव संरचना sched_class *class)
{
	जइ (sched_class_above(rq->next_class, class))
		rq->next_class = class;
}

स्थिर अन्तःस्थ bool rq_modified_above(संरचना rq *rq, ध्रुव संरचना sched_class *class)
{
	प्रत्यागम sched_class_above(rq->next_class, class);
}

स्थिर अन्तःस्थ bool sched_stop_runnable(संरचना rq *rq)
{
	प्रत्यागम rq->stop && task_on_rq_queued(rq->stop);
}

स्थिर अन्तःस्थ bool sched_dl_runnable(संरचना rq *rq)
{
	प्रत्यागम rq->dl.dl_nr_running > 0;
}

स्थिर अन्तःस्थ bool sched_rt_runnable(संरचना rq *rq)
{
	प्रत्यागम rq->rt.rt_queued > 0;
}

स्थिर अन्तःस्थ bool sched_fair_runnable(संरचना rq *rq)
{
	प्रत्यागम rq->cfs.nr_queued > 0;
}

बाह्य संरचना task_struct *pick_task_fair(संरचना rq *rq, संरचना rq_flags *rf);
बाह्य संरचना task_struct *pick_task_idle(संरचना rq *rq, संरचना rq_flags *rf);

#define SCA_CHECK		0x01
#define SCA_MIGRATE_DISABLE	0x02
#define SCA_MIGRATE_ENABLE	0x04
#define SCA_USER		0x08

बाह्य शून्य update_group_capacity(संरचना sched_domain *sd, पूर्णाङ्क cpu);

बाह्य शून्य sched_balance_trigger(संरचना rq *rq);

बाह्य पूर्णाङ्क __set_cpus_allowed_ptr(संरचना task_struct *p, संरचना affinity_context *ctx);
बाह्य शून्य set_cpus_allowed_common(संरचना task_struct *p, संरचना affinity_context *ctx);

स्थिर अन्तःस्थ bool task_allowed_on_cpu(संरचना task_struct *p, पूर्णाङ्क cpu)
{
	/* When not in the task's cpumask, no point in looking further. */
	जइ (!cpumask_test_cpu(cpu, p->cpus_ptr))
		प्रत्यागम false;

	/* Can @cpu run a user thread? */
	जइ (!(p->flags & PF_KTHREAD) && !task_cpu_possible(cpu, p))
		प्रत्यागम false;

	प्रत्यागम true;
}

स्थिर अन्तःस्थ cpumask_t *alloc_user_cpus_ptr(पूर्णाङ्क ग्रन्थिः)
{
	/*
	 * See set_cpus_allowed_force() above for the rcu_head usage.
	 */
	पूर्णाङ्क size = max_t(पूर्णाङ्क, cpumask_size(), परिमाण(संरचना rcu_head));

	प्रत्यागम kmalloc_ग्रन्थिः(size, GFP_KERNEL, ग्रन्थिः);
}

स्थिर अन्तःस्थ संरचना task_struct *get_प्रक्षिप_task(संरचना rq *rq)
{
	संरचना task_struct *p = rq->donor;

	lockdep_assert_rq_held(rq);

	जइ (rq->प्रक्षिप_busy)
		प्रत्यागम रिक्त;

	जइ (p->nr_cpus_allowed == 1)
		प्रत्यागम रिक्त;

	जइ (p->migration_disabled)
		प्रत्यागम रिक्त;

	rq->प्रक्षिप_busy = true;
	प्रत्यागम get_task_struct(p);
}

बाह्य पूर्णाङ्क प्रक्षिप_cpu_stop(शून्य *arg);

#ifdef CONFIG_CPU_IDLE

स्थिर अन्तःस्थ शून्य idle_set_state(संरचना rq *rq,
				  संरचना cpuidle_state *idle_state)
{
	rq->idle_state = idle_state;
}

स्थिर अन्तःस्थ संरचना cpuidle_state *idle_get_state(संरचना rq *rq)
{
	lockdep_assert(rcu_read_lock_any_held());

	प्रत्यागम rq->idle_state;
}

#अण्णहा /* !CONFIG_CPU_IDLE: */

स्थिर अन्तःस्थ शून्य idle_set_state(संरचना rq *rq,
				  संरचना cpuidle_state *idle_state)
{
}

स्थिर अन्तःस्थ संरचना cpuidle_state *idle_get_state(संरचना rq *rq)
{
	प्रत्यागम रिक्त;
}

#endif /* !CONFIG_CPU_IDLE */

बाह्य शून्य schedule_idle(शून्य);
asmlinkage शून्य schedule_user(शून्य);

बाह्य शून्य sysrq_sched_debug_show(शून्य);
बाह्य शून्य sched_init_granularity(शून्य);
बाह्य शून्य update_max_interval(शून्य);

बाह्य शून्य init_sched_dl_class(शून्य);
बाह्य शून्य init_sched_rt_class(शून्य);
बाह्य शून्य init_sched_fair_class(शून्य);

बाह्य शून्य resched_curr(संरचना rq *rq);
बाह्य शून्य resched_curr_lazy(संरचना rq *rq);
बाह्य शून्य resched_cpu(पूर्णाङ्क cpu);

बाह्य शून्य init_rt_bandwidth(संरचना rt_bandwidth *rt_b, u64 period, u64 runtime);
बाह्य bool sched_rt_bandwidth_account(संरचना rt_rq *rt_rq);

बाह्य शून्य init_dl_entity(संरचना sched_dl_entity *dl_se);

बाह्य शून्य init_cfs_throttle_work(संरचना task_struct *p);

#define BW_SHIFT		20
#define BW_UNIT			(1 << BW_SHIFT)
#define RATIO_SHIFT		8
#define MAX_BW_BITS		(64 - BW_SHIFT)
#define MAX_BW			((1ULL << MAX_BW_BITS) - 1)

बाह्य u64 to_ratio(u64 period, u64 runtime);

बाह्य शून्य init_entity_runnable_average(संरचना sched_entity *se);
बाह्य शून्य post_init_entity_util_avg(संरचना task_struct *p);

#ifdef CONFIG_NO_HZ_FULL
बाह्य bool sched_can_stop_tick(संरचना rq *rq);
बाह्य पूर्णाङ्क __init sched_tick_offload_init(शून्य);

/*
 * Tick may be needed by tasks in the runqueue depending on their policy and
 * requirements. If tick is needed, lets send the target an IPI to kick it out of
 * nohz mode if necessary.
 */
स्थिर अन्तःस्थ शून्य sched_update_tick_dependency(संरचना rq *rq)
{
	पूर्णाङ्क cpu = cpu_of(rq);

	जइ (!tick_nohz_full_cpu(cpu))
		प्रत्यागम;

	जइ (sched_can_stop_tick(rq))
		tick_nohz_dep_clear_cpu(cpu, TICK_DEP_BIT_SCHED);
	अण्णहा
		tick_nohz_dep_set_cpu(cpu, TICK_DEP_BIT_SCHED);
}
#अण्णहा /* !CONFIG_NO_HZ_FULL: */
स्थिर अन्तःस्थ पूर्णाङ्क sched_tick_offload_init(शून्य) { प्रत्यागम 0; }
स्थिर अन्तःस्थ शून्य sched_update_tick_dependency(संरचना rq *rq) { }
#endif /* !CONFIG_NO_HZ_FULL */

स्थिर अन्तःस्थ शून्य add_nr_running(संरचना rq *rq, unsigned गणनम्)
{
	unsigned prev_nr = rq->nr_running;

	rq->nr_running = prev_nr + गणनम्;
	जइ (trace_sched_update_nr_running_tp_enabled()) {
		call_trace_sched_update_nr_running(rq, गणनम्);
	}

	जइ (prev_nr < 2 && rq->nr_running >= 2)
		set_rd_overloaded(rq->rd, 1);

	sched_update_tick_dependency(rq);
}

स्थिर अन्तःस्थ शून्य sub_nr_running(संरचना rq *rq, unsigned गणनम्)
{
	rq->nr_running -= गणनम्;
	जइ (trace_sched_update_nr_running_tp_enabled()) {
		call_trace_sched_update_nr_running(rq, -गणनम्);
	}

	/* Check if we still need preemption */
	sched_update_tick_dependency(rq);
}

स्थिर अन्तःस्थ शून्य __block_task(संरचना rq *rq, संरचना task_struct *p)
{
	जइ (p->sched_contributes_to_load)
		rq->nr_uninterruptible++;

	जइ (p->in_iowait) {
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

बाह्य शून्य activate_task(संरचना rq *rq, संरचना task_struct *p, पूर्णाङ्क flags);
बाह्य शून्य deactivate_task(संरचना rq *rq, संरचना task_struct *p, पूर्णाङ्क flags);

बाह्य शून्य wakeup_preempt(संरचना rq *rq, संरचना task_struct *p, पूर्णाङ्क flags);

/*
 * attach_task() -- attach the task detached by detach_task() to its new rq.
 */
स्थिर अन्तःस्थ शून्य attach_task(संरचना rq *rq, संरचना task_struct *p)
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
स्थिर अन्तःस्थ शून्य attach_one_task(संरचना rq *rq, संरचना task_struct *p)
{
	guard(rq_lock)(rq);
	update_rq_clock(rq);
	attach_task(rq, p);
}

/*
 * __attach_tasks() - attaches a list of tasks (using se.group_node) to
 * the new rq
 */
स्थिर अन्तःस्थ शून्य __attach_tasks(संरचना rq *rq, संरचना list_head *tasks)
{
	guard(rq_lock)(rq);
	update_rq_clock(rq);

	जाव (!list_empty(tasks)) {
		संरचना task_struct *p;

		p = list_first_entry(tasks, संरचना task_struct, se.group_ग्रन्थिः);
		list_del_init(&p->se.group_ग्रन्थिः);

		attach_task(rq, p);
	}
}

#ifdef CONFIG_PREEMPT_RT
# define SCHED_NR_MIGRATE_BREAK 8
#अण्णहा
# define SCHED_NR_MIGRATE_BREAK 32
#endif

बाह्य __read_mostly unsigned पूर्णाङ्क sysctl_sched_nr_migrate;
बाह्य __read_mostly unsigned पूर्णाङ्क sysctl_sched_migration_cost;

बाह्य unsigned पूर्णाङ्क sysctl_sched_base_slice;

बाह्य पूर्णाङ्क sysctl_resched_latency_warn_ms;
बाह्य पूर्णाङ्क sysctl_resched_latency_warn_once;

बाह्य unsigned पूर्णाङ्क sysctl_sched_tunable_scaling;

बाह्य unsigned पूर्णाङ्क sysctl_numa_balancing_scan_delay;
बाह्य unsigned पूर्णाङ्क sysctl_numa_balancing_scan_period_min;
बाह्य unsigned पूर्णाङ्क sysctl_numa_balancing_scan_period_max;
बाह्य unsigned पूर्णाङ्क sysctl_numa_balancing_scan_size;
बाह्य unsigned पूर्णाङ्क sysctl_numa_balancing_hot_threshold;

#ifdef CONFIG_SCHED_HRTICK

/*
 * Use hrtick when:
 *  - enabled by features
 *  - hrtimer is actually high res
 */
स्थिर अन्तःस्थ bool hrtick_enabled(संरचना rq *rq)
{
	प्रत्यागम cpu_active(cpu_of(rq)) && hrtimer_highres_enabled();
}

स्थिर अन्तःस्थ bool hrtick_enabled_fair(संरचना rq *rq)
{
	प्रत्यागम sched_feat(HRTICK) && hrtick_enabled(rq);
}

स्थिर अन्तःस्थ bool hrtick_enabled_dl(संरचना rq *rq)
{
	प्रत्यागम sched_feat(HRTICK_DL) && hrtick_enabled(rq);
}

बाह्य शून्य hrtick_start(संरचना rq *rq, u64 delay);
स्थिर अन्तःस्थ bool hrtick_active(संरचना rq *rq)
{
	प्रत्यागम hrtimer_active(&rq->hrtick_timer);
}

#अण्णहा /* !CONFIG_SCHED_HRTICK: */
स्थिर अन्तःस्थ bool hrtick_enabled_fair(संरचना rq *rq) { प्रत्यागम false; }
स्थिर अन्तःस्थ bool hrtick_enabled_dl(संरचना rq *rq) { प्रत्यागम false; }
स्थिर अन्तःस्थ bool hrtick_enabled(संरचना rq *rq) { प्रत्यागम false; }
#endif /* !CONFIG_SCHED_HRTICK */

#ifndef arch_scale_freq_tick
स्थिर __always_inline शून्य arch_scale_freq_tick(शून्य) { }
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
स्थिर __always_inline
unsigned long arch_scale_freq_capacity(पूर्णाङ्क cpu)
{
	प्रत्यागम SCHED_CAPACITY_SCALE;
}
#endif

/*
 * In double_lock_balance()/double_rq_lock(), we use raw_spin_rq_lock() to
 * acquire rq lock instead of rq_lock(). So at the end of these two functions
 * we need to call double_rq_clock_clear_update() to clear RQCF_UPDATED of
 * rq->clock_update_flags to avoid the WARN_DOUBLE_CLOCK warning.
 */
स्थिर अन्तःस्थ शून्य double_rq_clock_clear_update(संरचना rq *rq1, संरचना rq *rq2)
{
	rq1->clock_update_flags &= (RQCF_REQ_SKIP|RQCF_ACT_SKIP);
	rq2->clock_update_flags &= (RQCF_REQ_SKIP|RQCF_ACT_SKIP);
}

#define DEFINE_LOCK_GUARD_2(name, type, _lock, _unlock, ...)				\
__DEFINE_UNLOCK_GUARD(name, type, _unlock, type *lock2; __VA_ARGS__)			\
स्थिर अन्तःस्थ class_##name##_t class_##name##_constructor(type *lock, type *lock2)	\
	__no_context_analysis								\
{ class_##name##_t _t = { .lock = lock, .lock2 = lock2 }, *_T = &_t;			\
  _lock; प्रत्यागम _t; }
#define DECLARE_LOCK_GUARD_2_ATTRS(_name, _lock, _unlock1, _unlock2)			\
स्थिर अन्तःस्थ class_##_name##_t class_##_name##_constructor(lock_##_name##_t *_T1,	\
							    lock_##_name##_t *_T2) _lock; \
स्थिर __always_inline शून्य __class_##_name##_cleanup_ctx1(class_##_name##_t **_T1)	\
	__no_context_analysis _unlock1 { }						\
स्थिर __always_inline शून्य __class_##_name##_cleanup_ctx2(class_##_name##_t **_T2)	\
	__no_context_analysis _unlock2 { }
#define WITH_LOCK_GUARD_2_ATTRS(_name, _T1, _T2)					\
	class_##_name##_constructor(_T1, _T2),						\
	*__UNIQUE_ID(unlock1) __cleanup(__class_##_name##_cleanup_ctx1) = (शून्य *)(_T1),\
	*__UNIQUE_ID(unlock2) __cleanup(__class_##_name##_cleanup_ctx2) = (शून्य *)(_T2)

स्थिर अन्तःस्थ bool rq_order_less(संरचना rq *rq1, संरचना rq *rq2)
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
	जइ (rq1->core->cpu < rq2->core->cpu)
		प्रत्यागम true;
	जइ (rq1->core->cpu > rq2->core->cpu)
		प्रत्यागम false;

	/*
	 * __sched_core_flip() relies on SMT having cpu-id lock order.
	 */
#endif /* CONFIG_SCHED_CORE */
	प्रत्यागम rq1->cpu < rq2->cpu;
}

बाह्य शून्य double_rq_lock(संरचना rq *rq1, संरचना rq *rq2)
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
स्थिर अन्तःस्थ पूर्णाङ्क _double_lock_balance(संरचना rq *this_rq, संरचना rq *busiest)
	__must_hold(__rq_lockp(this_rq))
	__acquires(__rq_lockp(busiest))
{
	raw_spin_rq_unlock(this_rq);
	double_rq_lock(this_rq, busiest);

	प्रत्यागम 1;
}

#अण्णहा /* !CONFIG_PREEMPTION: */
/*
 * Unfair double_lock_balance: Optimizes throughput at the expense of
 * latency by eliminating extra atomic operations when the locks are
 * already in proper order on entry.  This favors lower CPU-ids and will
 * grant the double lock to lower CPUs over higher ids under contention,
 * regardless of entry order into the function.
 */
स्थिर अन्तःस्थ पूर्णाङ्क _double_lock_balance(संरचना rq *this_rq, संरचना rq *busiest)
	__must_hold(__rq_lockp(this_rq))
	__acquires(__rq_lockp(busiest))
{
	जइ (__rq_lockp(this_rq) == __rq_lockp(busiest)) {
		__acquire(__rq_lockp(busiest)); /* already held */
		double_rq_clock_clear_update(this_rq, busiest);
		प्रत्यागम 0;
	}

	जइ (likely(raw_spin_rq_trylock(busiest))) {
		double_rq_clock_clear_update(this_rq, busiest);
		प्रत्यागम 0;
	}

	जइ (rq_order_less(this_rq, busiest)) {
		raw_spin_rq_lock_nested(busiest, SINGLE_DEPTH_NESTING);
		double_rq_clock_clear_update(this_rq, busiest);
		प्रत्यागम 0;
	}

	raw_spin_rq_unlock(this_rq);
	double_rq_lock(this_rq, busiest);

	प्रत्यागम 1;
}

#endif /* !CONFIG_PREEMPTION */

/*
 * double_lock_balance - lock the busiest runqueue, this_rq is locked already.
 */
स्थिर अन्तःस्थ पूर्णाङ्क double_lock_balance(संरचना rq *this_rq, संरचना rq *busiest)
	__must_hold(__rq_lockp(this_rq))
	__acquires(__rq_lockp(busiest))
{
	lockdep_assert_irqs_disabled();

	प्रत्यागम _double_lock_balance(this_rq, busiest);
}

स्थिर अन्तःस्थ शून्य double_unlock_balance(संरचना rq *this_rq, संरचना rq *busiest)
	__releases(__rq_lockp(busiest))
{
	जइ (__rq_lockp(this_rq) != __rq_lockp(busiest))
		raw_spin_rq_unlock(busiest);
	अण्णहा
		__release(__rq_lockp(busiest)); /* fake release */
	lock_set_subclass(&__rq_lockp(this_rq)->dep_map, 0, _RET_IP_);
}

स्थिर अन्तःस्थ शून्य double_lock(spinlock_t *l1, spinlock_t *l2)
	__acquires(l1, l2)
{
	जइ (l1 > l2)
		swap(l1, l2);

	spin_lock(l1);
	spin_lock_nested(l2, SINGLE_DEPTH_NESTING);
}

स्थिर अन्तःस्थ शून्य double_lock_irq(spinlock_t *l1, spinlock_t *l2)
	__acquires(l1, l2)
{
	जइ (l1 > l2)
		swap(l1, l2);

	spin_lock_irq(l1);
	spin_lock_nested(l2, SINGLE_DEPTH_NESTING);
}

स्थिर अन्तःस्थ शून्य double_raw_lock(raw_spinlock_t *l1, raw_spinlock_t *l2)
	__acquires(l1, l2)
{
	जइ (l1 > l2)
		swap(l1, l2);

	raw_spin_lock(l1);
	raw_spin_lock_nested(l2, SINGLE_DEPTH_NESTING);
}

स्थिर अन्तःस्थ शून्य double_raw_unlock(raw_spinlock_t *l1, raw_spinlock_t *l2)
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
स्थिर अन्तःस्थ शून्य double_rq_unlock(संरचना rq *rq1, संरचना rq *rq2)
	__releases(__rq_lockp(rq1), __rq_lockp(rq2))
{
	जइ (__rq_lockp(rq1) != __rq_lockp(rq2))
		raw_spin_rq_unlock(rq2);
	अण्णहा
		__release(__rq_lockp(rq2)); /* fake release */
	raw_spin_rq_unlock(rq1);
}

बाह्य शून्य set_rq_online (संरचना rq *rq);
बाह्य शून्य set_rq_offline(संरचना rq *rq);

बाह्य bool sched_smp_initialized;

DEFINE_LOCK_GUARD_2(double_rq_lock, संरचना rq,
		    double_rq_lock(_T->lock, _T->lock2),
		    double_rq_unlock(_T->lock, _T->lock2))

बाह्य संरचना sched_entity *__pick_root_entity(संरचना cfs_rq *cfs_rq);
बाह्य संरचना sched_entity *__pick_first_entity(संरचना cfs_rq *cfs_rq);
बाह्य संरचना sched_entity *__pick_last_entity(संरचना cfs_rq *cfs_rq);

बाह्य bool sched_debug_verbose;

बाह्य शून्य print_cfs_stats(संरचना seq_file *m, पूर्णाङ्क cpu);
बाह्य शून्य print_rt_stats(संरचना seq_file *m, पूर्णाङ्क cpu);
बाह्य शून्य print_dl_stats(संरचना seq_file *m, पूर्णाङ्क cpu);
बाह्य शून्य print_cfs_rq(संरचना seq_file *m, पूर्णाङ्क cpu, संरचना cfs_rq *cfs_rq);
बाह्य शून्य print_rt_rq(संरचना seq_file *m, पूर्णाङ्क cpu, संरचना rt_rq *rt_rq);
बाह्य शून्य print_dl_rq(संरचना seq_file *m, पूर्णाङ्क cpu, संरचना dl_rq *dl_rq);

बाह्य शून्य resched_latency_warn(पूर्णाङ्क cpu, u64 latency);

#ifdef CONFIG_NUMA_BALANCING
बाह्य शून्य show_numa_stats(संरचना task_struct *p, संरचना seq_file *m);
बाह्य शून्य
print_numa_stats(संरचना seq_file *m, पूर्णाङ्क ग्रन्थिः, unsigned long tsf,
		 unsigned long tpf, unsigned long gsf, unsigned long gpf);
#endif /* CONFIG_NUMA_BALANCING */

बाह्य शून्य init_cfs_rq(संरचना cfs_rq *cfs_rq);
बाह्य शून्य init_rt_rq(संरचना rt_rq *rt_rq);
बाह्य शून्य init_dl_rq(संरचना dl_rq *dl_rq);

बाह्य शून्य cfs_bandwidth_usage_inc(शून्य);
बाह्य शून्य cfs_bandwidth_usage_dec(शून्य);

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

बाह्य शून्य nohz_balance_exit_idle(संरचना rq *rq);
#अण्णहा /* !CONFIG_NO_HZ_COMMON: */
स्थिर अन्तःस्थ शून्य nohz_balance_exit_idle(संरचना rq *rq) { }
#endif /* !CONFIG_NO_HZ_COMMON */

#ifdef CONFIG_NO_HZ_COMMON
बाह्य शून्य nohz_run_idle_balance(पूर्णाङ्क cpu);
#अण्णहा
स्थिर अन्तःस्थ शून्य nohz_run_idle_balance(पूर्णाङ्क cpu) { }
#endif

#अन्तर्भाव "stats.h"

#जइ defined(CONFIG_SCHED_CORE) && defined(CONFIG_SCHEDSTATS)

बाह्य शून्य __sched_core_account_forceidle(संरचना rq *rq);

स्थिर अन्तःस्थ शून्य sched_core_account_forceidle(संरचना rq *rq)
{
	जइ (schedstat_enabled())
		__sched_core_account_forceidle(rq);
}

बाह्य शून्य __sched_core_tick(संरचना rq *rq);

स्थिर अन्तःस्थ शून्य sched_core_tick(संरचना rq *rq)
{
	जइ (sched_core_enabled(rq) && schedstat_enabled())
		__sched_core_tick(rq);
}

#अण्णहा /* !(CONFIG_SCHED_CORE && CONFIG_SCHEDSTATS): */

स्थिर अन्तःस्थ शून्य sched_core_account_forceidle(संरचना rq *rq) { }

स्थिर अन्तःस्थ शून्य sched_core_tick(संरचना rq *rq) { }

#endif /* !(CONFIG_SCHED_CORE && CONFIG_SCHEDSTATS) */

#ifdef CONFIG_IRQ_TIME_ACCOUNTING

संरचना irqtime {
	u64			total;
	u64			tick_delta;
	u64			irq_start_time;
	संरचना u64_stats_sync	sync;
};

DECLARE_PER_CPU(संरचना irqtime, cpu_irqtime);
DECLARE_STATIC_कुञ्चिका_FALSE(sched_clock_irqtime);

स्थिर अन्तःस्थ पूर्णाङ्क irqtime_enabled(शून्य)
{
	प्रत्यागम static_branch_likely(&sched_clock_irqtime);
}

/*
 * Returns the irqtime minus the softirq time computed by ksoftirqd.
 * Otherwise ksoftirqd's sum_exec_runtime is subtracted its own runtime
 * and never move forward.
 */
स्थिर अन्तःस्थ u64 irq_time_read(पूर्णाङ्क cpu)
{
	संरचना irqtime *irqtime = &per_cpu(cpu_irqtime, cpu);
	unsigned पूर्णाङ्क seq;
	u64 total;

	करेहि {
		seq = __u64_stats_fetch_begin(&irqtime->sync);
		total = irqtime->total;
	} जाव (__u64_stats_fetch_retry(&irqtime->sync, seq));

	प्रत्यागम total;
}

#अण्णहा /* !CONFIG_IRQ_TIME_ACCOUNTING: */

स्थिर अन्तःस्थ पूर्णाङ्क irqtime_enabled(शून्य)
{
	प्रत्यागम 0;
}

#endif /* !CONFIG_IRQ_TIME_ACCOUNTING */

#ifdef CONFIG_CPU_FREQ

DECLARE_PER_CPU(संरचना update_util_data __rcu *, cpufreq_update_util_data);

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
स्थिर अन्तःस्थ शून्य cpufreq_update_util(संरचना rq *rq, unsigned पूर्णाङ्क flags)
{
	संरचना update_util_data *data;

	data = rcu_dereference_sched(*per_cpu_ptr(&cpufreq_update_util_data,
						  cpu_of(rq)));
	जइ (data)
		data->func(data, rq_clock(rq), flags);
}
#अण्णहा /* !CONFIG_CPU_FREQ: */
स्थिर अन्तःस्थ शून्य cpufreq_update_util(संरचना rq *rq, unsigned पूर्णाङ्क flags) { }
#endif /* !CONFIG_CPU_FREQ */

#ifdef arch_scale_freq_capacity
# ifndef arch_scale_freq_invariant
#  define arch_scale_freq_invariant()	true
# endif
#अण्णहा
# define arch_scale_freq_invariant()	false
#endif

unsigned long effective_cpu_util(पूर्णाङ्क cpu, unsigned long util_cfs,
				 unsigned long *min,
				 unsigned long *max);

unsigned long sugov_effective_cpu_perf(पूर्णाङ्क cpu, unsigned long actual,
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
स्थिर अन्तःस्थ bool dl_task_fits_capacity(संरचना task_struct *p, पूर्णाङ्क cpu)
{
	unsigned long cap = arch_scale_cpu_capacity(cpu);

	प्रत्यागम cap >= p->dl.dl_density >> (BW_SHIFT - SCHED_CAPACITY_SHIFT);
}

स्थिर अन्तःस्थ unsigned long cpu_bw_dl(संरचना rq *rq)
{
	प्रत्यागम (rq->dl.running_bw * SCHED_CAPACITY_SCALE) >> BW_SHIFT;
}

स्थिर अन्तःस्थ unsigned long cpu_util_dl(संरचना rq *rq)
{
	प्रत्यागम READ_ONCE(rq->avg_dl.util_avg);
}


बाह्य unsigned long cpu_util_cfs(पूर्णाङ्क cpu);
बाह्य unsigned long cpu_util_cfs_boost(पूर्णाङ्क cpu);

स्थिर अन्तःस्थ unsigned long cpu_util_rt(संरचना rq *rq)
{
	प्रत्यागम READ_ONCE(rq->avg_rt.util_avg);
}

#ifdef CONFIG_UCLAMP_TASK

unsigned long uclamp_eff_मानम्(संरचना task_struct *p, गणना uclamp_id clamp_id);

/*
 * When uclamp is compiled in, the aggregation at rq level is 'turned off'
 * by default in the fast path and only gets turned on once userspace performs
 * an operation that requires it.
 *
 * Returns true if userspace opted-in to use uclamp and aggregation at rq level
 * hence is active.
 */
स्थिर अन्तःस्थ bool uclamp_is_used(शून्य)
{
	प्रत्यागम static_branch_likely(&sched_uclamp_used);
}

/*
 * Enabling static branches would get the cpus_read_lock(),
 * check whether uclamp_is_used before enable it to avoid always
 * calling cpus_read_lock(). Because we never disable this
 * static key once enable it.
 */
स्थिर अन्तःस्थ शून्य sched_uclamp_enable(शून्य)
{
	जइ (!uclamp_is_used())
		static_branch_enable(&sched_uclamp_used);
}

स्थिर अन्तःस्थ unsigned long uclamp_rq_get(संरचना rq *rq,
					  गणना uclamp_id clamp_id)
{
	प्रत्यागम READ_ONCE(rq->uclamp[clamp_id].मानम्);
}

स्थिर अन्तःस्थ शून्य uclamp_rq_set(संरचना rq *rq, गणना uclamp_id clamp_id,
				 unsigned पूर्णाङ्क मानम्)
{
	WRITE_ONCE(rq->uclamp[clamp_id].मानम्, मानम्);
}

स्थिर अन्तःस्थ bool uclamp_rq_is_idle(संरचना rq *rq)
{
	प्रत्यागम rq->uclamp_flags & UCLAMP_FLAG_IDLE;
}

/* Is the rq being capped/throttled by uclamp_max? */
स्थिर अन्तःस्थ bool uclamp_rq_is_capped(संरचना rq *rq)
{
	unsigned long rq_util;
	unsigned long max_util;

	जइ (!uclamp_is_used())
		प्रत्यागम false;

	rq_util = cpu_util_cfs(cpu_of(rq)) + cpu_util_rt(rq);
	max_util = READ_ONCE(rq->uclamp[UCLAMP_MAX].मानम्);

	प्रत्यागम max_util != SCHED_CAPACITY_SCALE && rq_util >= max_util;
}

#define for_each_clamp_id(clamp_id) \
	प्रति ((clamp_id) = 0; (clamp_id) < UCLAMP_CNT; (clamp_id)++)

बाह्य unsigned पूर्णाङ्क sysctl_sched_uclamp_util_min_rt_default;


स्थिर अन्तःस्थ unsigned पूर्णाङ्क uclamp_none(गणना uclamp_id clamp_id)
{
	जइ (clamp_id == UCLAMP_MIN)
		प्रत्यागम 0;
	प्रत्यागम SCHED_CAPACITY_SCALE;
}

/* Integer rounded range for each bucket */
#define UCLAMP_BUCKET_DELTA DIV_ROUND_CLOSEST(SCHED_CAPACITY_SCALE, UCLAMP_BUCKETS)

स्थिर अन्तःस्थ unsigned पूर्णाङ्क uclamp_bucket_id(unsigned पूर्णाङ्क clamp_मानम्)
{
	प्रत्यागम min_t(unsigned पूर्णाङ्क, clamp_मानम् / UCLAMP_BUCKET_DELTA, UCLAMP_BUCKETS - 1);
}

स्थिर अन्तःस्थ शून्य
uclamp_se_set(संरचना uclamp_se *uc_se, unsigned पूर्णाङ्क मानम्, bool user_defined)
{
	uc_se->मानम् = मानम्;
	uc_se->bucket_id = uclamp_bucket_id(मानम्);
	uc_se->user_defined = user_defined;
}

#अण्णहा /* !CONFIG_UCLAMP_TASK: */

स्थिर अन्तःस्थ unsigned long
uclamp_eff_मानम्(संरचना task_struct *p, गणना uclamp_id clamp_id)
{
	जइ (clamp_id == UCLAMP_MIN)
		प्रत्यागम 0;

	प्रत्यागम SCHED_CAPACITY_SCALE;
}

स्थिर अन्तःस्थ bool uclamp_rq_is_capped(संरचना rq *rq) { प्रत्यागम false; }

स्थिर अन्तःस्थ bool uclamp_is_used(शून्य)
{
	प्रत्यागम false;
}

स्थिर अन्तःस्थ शून्य sched_uclamp_enable(शून्य) {}

स्थिर अन्तःस्थ unsigned long
uclamp_rq_get(संरचना rq *rq, गणना uclamp_id clamp_id)
{
	जइ (clamp_id == UCLAMP_MIN)
		प्रत्यागम 0;

	प्रत्यागम SCHED_CAPACITY_SCALE;
}

स्थिर अन्तःस्थ शून्य
uclamp_rq_set(संरचना rq *rq, गणना uclamp_id clamp_id, unsigned पूर्णाङ्क मानम्)
{
}

स्थिर अन्तःस्थ bool uclamp_rq_is_idle(संरचना rq *rq)
{
	प्रत्यागम false;
}

#endif /* !CONFIG_UCLAMP_TASK */

#ifdef CONFIG_HAVE_SCHED_AVG_IRQ

स्थिर अन्तःस्थ unsigned long cpu_util_irq(संरचना rq *rq)
{
	प्रत्यागम READ_ONCE(rq->avg_irq.util_avg);
}

स्थिर अन्तःस्थ
unsigned long scale_irq_capacity(unsigned long util, unsigned long irq, unsigned long max)
{
	util *= (max - irq);
	util /= max;

	प्रत्यागम util;

}

#अण्णहा /* !CONFIG_HAVE_SCHED_AVG_IRQ: */

स्थिर अन्तःस्थ unsigned long cpu_util_irq(संरचना rq *rq)
{
	प्रत्यागम 0;
}

स्थिर अन्तःस्थ
unsigned long scale_irq_capacity(unsigned long util, unsigned long irq, unsigned long max)
{
	प्रत्यागम util;
}

#endif /* !CONFIG_HAVE_SCHED_AVG_IRQ */

बाह्य शून्य __setparam_fair(संरचना task_struct *p, ध्रुव संरचना sched_attr *attr);

#जइ defined(CONFIG_ENERGY_MODEL) && defined(CONFIG_CPU_FREQ_GOV_SCHEDUTIL)

#define perf_domain_span(pd) (to_cpumask(((pd)->em_pd->cpus)))

DECLARE_STATIC_कुञ्चिका_FALSE(sched_energy_present);

स्थिर अन्तःस्थ bool sched_energy_enabled(शून्य)
{
	प्रत्यागम static_branch_unlikely(&sched_energy_present);
}

#अण्णहा /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL): */

#define perf_domain_span(pd) रिक्त

स्थिर अन्तःस्थ bool sched_energy_enabled(शून्य) { प्रत्यागम false; }

#endif /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL) */

#ifdef CONFIG_MEMBARRIER

/*
 * The scheduler provides memory barriers required by membarrier between:
 * - prior user-space memory accesses and store to rq->membarrier_state,
 * - store to rq->membarrier_state and following user-space memory accesses.
 * In the same way it provides those guarantees around store to rq->curr.
 */
स्थिर अन्तःस्थ शून्य membarrier_switch_mm(संरचना rq *rq,
					संरचना mm_struct *prev_mm,
					संरचना mm_struct *next_mm)
{
	पूर्णाङ्क membarrier_state;

	जइ (prev_mm == next_mm)
		प्रत्यागम;

	membarrier_state = atomic_read(&next_mm->membarrier_state);
	जइ (READ_ONCE(rq->membarrier_state) == membarrier_state)
		प्रत्यागम;

	WRITE_ONCE(rq->membarrier_state, membarrier_state);
}

#अण्णहा /* !CONFIG_MEMBARRIER: */

स्थिर अन्तःस्थ शून्य membarrier_switch_mm(संरचना rq *rq,
					संरचना mm_struct *prev_mm,
					संरचना mm_struct *next_mm)
{
}

#endif /* !CONFIG_MEMBARRIER */

स्थिर अन्तःस्थ bool is_per_cpu_kthread(संरचना task_struct *p)
{
	जइ (!(p->flags & PF_KTHREAD))
		प्रत्यागम false;

	जइ (p->nr_cpus_allowed != 1)
		प्रत्यागम false;

	प्रत्यागम true;
}

बाह्य शून्य swake_up_all_locked(संरचना swait_queue_head *q);
बाह्य शून्य __prepare_to_swait(संरचना swait_queue_head *q, संरचना swait_queue *wait);

बाह्य पूर्णाङ्क try_to_wake_up(संरचना task_struct *tsk, unsigned पूर्णाङ्क state, पूर्णाङ्क wake_flags);

#ifdef CONFIG_PREEMPT_DYNAMIC
बाह्य पूर्णाङ्क preempt_dynamic_mode;
बाह्य पूर्णाङ्क sched_dynamic_mode(ध्रुव वर्ण *str);
बाह्य शून्य sched_dynamic_update(पूर्णाङ्क mode);
#endif
बाह्य ध्रुव वर्ण *preempt_modes[];

#ifdef CONFIG_SCHED_MM_CID

स्थिर __always_inline bool cid_on_cpu(unsigned पूर्णाङ्क cid)
{
	प्रत्यागम cid & MM_CID_ONCPU;
}

स्थिर __always_inline bool cid_in_transit(unsigned पूर्णाङ्क cid)
{
	प्रत्यागम cid & MM_CID_TRANSIT;
}

स्थिर __always_inline unsigned पूर्णाङ्क cpu_cid_to_cid(unsigned पूर्णाङ्क cid)
{
	प्रत्यागम cid & ~MM_CID_ONCPU;
}

स्थिर __always_inline unsigned पूर्णाङ्क cid_to_cpu_cid(unsigned पूर्णाङ्क cid)
{
	प्रत्यागम cid | MM_CID_ONCPU;
}

स्थिर __always_inline unsigned पूर्णाङ्क cid_to_transit_cid(unsigned पूर्णाङ्क cid)
{
	प्रत्यागम cid | MM_CID_TRANSIT;
}

स्थिर __always_inline unsigned पूर्णाङ्क cid_from_transit_cid(unsigned पूर्णाङ्क cid)
{
	प्रत्यागम cid & ~MM_CID_TRANSIT;
}

स्थिर __always_inline bool cid_on_task(unsigned पूर्णाङ्क cid)
{
	/* True if none of the MM_CID_ONCPU, MM_CID_TRANSIT, MM_CID_UNSET bits is set */
	प्रत्यागम cid < MM_CID_TRANSIT;
}

स्थिर __always_inline शून्य mm_drop_cid(संरचना mm_struct *mm, unsigned पूर्णाङ्क cid)
{
	clear_bit(cid, mm_cidmask(mm));
}

स्थिर __always_inline शून्य mm_unset_cid_on_task(संरचना task_struct *t)
{
	unsigned पूर्णाङ्क cid = t->mm_cid.cid;

	t->mm_cid.cid = MM_CID_UNSET;
	जइ (cid_on_task(cid))
		mm_drop_cid(t->mm, cid);
}

स्थिर __always_inline शून्य mm_drop_cid_on_cpu(संरचना mm_struct *mm, संरचना mm_cid_pcpu *pcp)
{
	/* Clear the ONCPU bit, but do not set UNSET in the per CPU storage */
	जइ (cid_on_cpu(pcp->cid)) {
		pcp->cid = cpu_cid_to_cid(pcp->cid);
		mm_drop_cid(mm, pcp->cid);
	}
}

स्थिर अन्तःस्थ unsigned पूर्णाङ्क __mm_get_cid(संरचना mm_struct *mm, unsigned पूर्णाङ्क max_cids)
{
	unsigned पूर्णाङ्क cid = find_first_zero_bit(mm_cidmask(mm), max_cids);

	जइ (cid >= max_cids)
		प्रत्यागम MM_CID_UNSET;
	जइ (test_and_set_bit(cid, mm_cidmask(mm)))
		प्रत्यागम MM_CID_UNSET;
	प्रत्यागम cid;
}

स्थिर अन्तःस्थ unsigned पूर्णाङ्क mm_get_cid(संरचना mm_struct *mm)
{
	unsigned पूर्णाङ्क cid = __mm_get_cid(mm, READ_ONCE(mm->mm_cid.max_cids));

	जाव (cid == MM_CID_UNSET) {
		cpu_relax();
		cid = __mm_get_cid(mm, अङ्कः_possible_cpus());
	}
	प्रत्यागम cid;
}

स्थिर अन्तःस्थ unsigned पूर्णाङ्क mm_cid_converge(संरचना mm_struct *mm, unsigned पूर्णाङ्क orig_cid,
					   unsigned पूर्णाङ्क max_cids)
{
	unsigned पूर्णाङ्क new_cid, cid = cpu_cid_to_cid(orig_cid);

	/* Is it in the optimal CID space? */
	जइ (likely(cid < max_cids))
		प्रत्यागम orig_cid;

	/* Try to find one in the optimal space. Otherwise keep the provided. */
	new_cid = __mm_get_cid(mm, max_cids);
	जइ (new_cid != MM_CID_UNSET) {
		mm_drop_cid(mm, cid);
		/* Preserve the ONCPU mode of the original CID */
		प्रत्यागम new_cid | (orig_cid & MM_CID_ONCPU);
	}
	प्रत्यागम orig_cid;
}

स्थिर __always_inline शून्य mm_cid_update_task_cid(संरचना task_struct *t, unsigned पूर्णाङ्क cid)
{
	जइ (t->mm_cid.cid != cid) {
		t->mm_cid.cid = cid;
		rseq_sched_set_ids_changed(t);
	}
}

स्थिर __always_inline शून्य mm_cid_update_pcpu_cid(संरचना mm_struct *mm, unsigned पूर्णाङ्क cid)
{
	__this_cpu_write(mm->mm_cid.pcpu->cid, cid);
}

स्थिर __always_inline शून्य mm_cid_from_cpu(संरचना task_struct *t, unsigned पूर्णाङ्क cpu_cid,
					    unsigned पूर्णाङ्क mode)
{
	unsigned पूर्णाङ्क max_cids, tcid = t->mm_cid.cid;
	संरचना mm_struct *mm = t->mm;

	max_cids = READ_ONCE(mm->mm_cid.max_cids);
	/* Optimize for the common case where both have the ONCPU bit set */
	जइ (likely(cid_on_cpu(cpu_cid & tcid))) {
		जइ (likely(cpu_cid_to_cid(cpu_cid) < max_cids)) {
			mm_cid_update_task_cid(t, cpu_cid);
			प्रत्यागम;
		}
		/* Try to converge into the optimal CID space */
		cpu_cid = mm_cid_converge(mm, cpu_cid, max_cids);
	} अण्णहा {
		/* Hand over or drop the task owned CID */
		जइ (cid_on_task(tcid)) {
			जइ (cid_on_cpu(cpu_cid))
				mm_unset_cid_on_task(t);
			अण्णहा
				cpu_cid = cid_to_cpu_cid(tcid);
		}
		/* Still nothing, allocate a new one */
		जइ (!cid_on_cpu(cpu_cid))
			cpu_cid = cid_to_cpu_cid(mm_get_cid(mm));

		/* Handle the transition mode flag if required */
		जइ (mode & MM_CID_TRANSIT)
			cpu_cid = cpu_cid_to_cid(cpu_cid) | MM_CID_TRANSIT;
	}
	mm_cid_update_pcpu_cid(mm, cpu_cid);
	mm_cid_update_task_cid(t, cpu_cid);
}

स्थिर __always_inline शून्य mm_cid_from_task(संरचना task_struct *t, unsigned पूर्णाङ्क cpu_cid,
					     unsigned पूर्णाङ्क mode)
{
	unsigned पूर्णाङ्क max_cids, tcid = t->mm_cid.cid;
	संरचना mm_struct *mm = t->mm;

	max_cids = READ_ONCE(mm->mm_cid.max_cids);
	/* Optimize for the common case, where both have the ONCPU bit clear */
	जइ (likely(cid_on_task(tcid | cpu_cid))) {
		जइ (likely(tcid < max_cids)) {
			mm_cid_update_pcpu_cid(mm, tcid);
			प्रत्यागम;
		}
		/* Try to converge into the optimal CID space */
		tcid = mm_cid_converge(mm, tcid, max_cids);
	} अण्णहा {
		/* Hand over or drop the CPU owned CID */
		जइ (cid_on_cpu(cpu_cid)) {
			जइ (cid_on_task(tcid))
				mm_drop_cid_on_cpu(mm, this_cpu_ptr(mm->mm_cid.pcpu));
			अण्णहा
				tcid = cpu_cid_to_cid(cpu_cid);
		}
		/* Still nothing, allocate a new one */
		जइ (!cid_on_task(tcid))
			tcid = mm_get_cid(mm);
		/* Set the transition mode flag if required */
		tcid |= mode & MM_CID_TRANSIT;
	}
	mm_cid_update_pcpu_cid(mm, tcid);
	mm_cid_update_task_cid(t, tcid);
}

स्थिर __always_inline शून्य mm_cid_schedin(संरचना task_struct *next)
{
	संरचना mm_struct *mm = next->mm;
	unsigned पूर्णाङ्क cpu_cid, mode;

	जइ (!next->mm_cid.active)
		प्रत्यागम;

	cpu_cid = __this_cpu_read(mm->mm_cid.pcpu->cid);
	mode = READ_ONCE(mm->mm_cid.mode);
	जइ (likely(!cid_on_cpu(mode)))
		mm_cid_from_task(next, cpu_cid, mode);
	अण्णहा
		mm_cid_from_cpu(next, cpu_cid, mode);
}

स्थिर __always_inline शून्य mm_cid_schedout(संरचना task_struct *prev)
{
	संरचना mm_struct *mm = prev->mm;
	unsigned पूर्णाङ्क mode, cid;

	/* During mode transitions CIDs are temporary and need to be dropped */
	जइ (likely(!cid_in_transit(prev->mm_cid.cid)))
		प्रत्यागम;

	mode = READ_ONCE(mm->mm_cid.mode);
	cid = cid_from_transit_cid(prev->mm_cid.cid);

	/*
	 * If transition mode is done, transfer ownership when the CID is
	 * within the convergence range to optimize the next schedule in.
	 */
	जइ (!cid_in_transit(mode) && cid < READ_ONCE(mm->mm_cid.max_cids)) {
		जइ (cid_on_cpu(mode))
			cid = cid_to_cpu_cid(cid);

		/* Update both so that the next schedule in goes into the fast path */
		mm_cid_update_pcpu_cid(mm, cid);
		prev->mm_cid.cid = cid;
	} अण्णहा {
		mm_drop_cid(mm, cid);
		prev->mm_cid.cid = MM_CID_UNSET;
	}
}

स्थिर अन्तःस्थ शून्य mm_cid_switch_to(संरचना task_struct *prev, संरचना task_struct *next)
{
	mm_cid_schedout(prev);
	mm_cid_schedin(next);
}

#अण्णहा /* !CONFIG_SCHED_MM_CID: */
स्थिर अन्तःस्थ शून्य mm_cid_switch_to(संरचना task_struct *prev, संरचना task_struct *next) { }
#endif /* !CONFIG_SCHED_MM_CID */

#ifdef CONFIG_SCHED_CACHE
DECLARE_STATIC_कुञ्चिका_FALSE(sched_cache_present);
DECLARE_STATIC_कुञ्चिका_FALSE(sched_cache_active);
बाह्य पूर्णाङ्क sysctl_sched_cache_user;
बाह्य unsigned पूर्णाङ्क llc_aggr_tolerance;
बाह्य unsigned पूर्णाङ्क llc_epoch_period;
बाह्य unsigned पूर्णाङ्क llc_epoch_affinity_timeout;
बाह्य unsigned पूर्णाङ्क llc_imb_pct;
बाह्य unsigned पूर्णाङ्क llc_overaggr_pct;

स्थिर अन्तःस्थ bool sched_cache_enabled(शून्य)
{
	प्रत्यागम static_branch_unlikely(&sched_cache_active);
}

बाह्य शून्य sched_cache_active_set(शून्य);

#endif

शून्य sched_domains_free_llc_id(पूर्णाङ्क cpu);

बाह्य शून्य init_sched_mm(संरचना task_struct *p);

बाह्य u64 avg_vruntime(संरचना cfs_rq *cfs_rq);
बाह्य पूर्णाङ्क entity_eligible(संरचना cfs_rq *cfs_rq, संरचना sched_entity *se);
स्थिर अन्तःस्थ
शून्य move_queued_task_locked(संरचना rq *src_rq, संरचना rq *dst_rq, संरचना task_struct *task)
{
	lockdep_assert_rq_held(src_rq);
	lockdep_assert_rq_held(dst_rq);

	deactivate_task(src_rq, task, 0);
	set_task_cpu(task, dst_rq->cpu);
	activate_task(dst_rq, task, 0);
	wakeup_preempt(dst_rq, task, 0);
}

स्थिर अन्तःस्थ
bool task_is_pushable(संरचना rq *rq, संरचना task_struct *p, पूर्णाङ्क cpu)
{
	जइ (!task_on_cpu(rq, p) &&
	    cpumask_test_cpu(cpu, &p->cpus_mask))
		प्रत्यागम true;

	प्रत्यागम false;
}

#ifdef CONFIG_RT_MUTEXES

स्थिर अन्तःस्थ पूर्णाङ्क __rt_effective_prio(संरचना task_struct *pi_task, पूर्णाङ्क prio)
{
	जइ (pi_task)
		prio = min(prio, pi_task->prio);

	प्रत्यागम prio;
}

स्थिर अन्तःस्थ पूर्णाङ्क rt_effective_prio(संरचना task_struct *p, पूर्णाङ्क prio)
{
	संरचना task_struct *pi_task = rt_mutex_get_शिखरम्_task(p);

	प्रत्यागम __rt_effective_prio(pi_task, prio);
}

#अण्णहा /* !CONFIG_RT_MUTEXES: */

स्थिर अन्तःस्थ पूर्णाङ्क rt_effective_prio(संरचना task_struct *p, पूर्णाङ्क prio)
{
	प्रत्यागम prio;
}

#endif /* !CONFIG_RT_MUTEXES */

बाह्य पूर्णाङ्क __sched_setscheduler(संरचना task_struct *p, ध्रुव संरचना sched_attr *attr, bool user, bool pi);
बाह्य पूर्णाङ्क __sched_setaffinity(संरचना task_struct *p, संरचना affinity_context *ctx);
बाह्य ध्रुव संरचना sched_class *__setscheduler_class(पूर्णाङ्क policy, पूर्णाङ्क prio);
बाह्य शून्य set_load_weight(संरचना task_struct *p, bool update_load);
बाह्य शून्य enqueue_task(संरचना rq *rq, संरचना task_struct *p, पूर्णाङ्क flags);
बाह्य bool dequeue_task(संरचना rq *rq, संरचना task_struct *p, पूर्णाङ्क flags);

बाह्य संरचना balance_callback *splice_balance_callbacks(संरचना rq *rq);

बाह्य शून्य __balance_callbacks(संरचना rq *rq, संरचना rq_flags *rf);
बाह्य शून्य balance_callbacks(संरचना rq *rq, संरचना balance_callback *head);

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
संरचना sched_change_ctx {
	u64			prio;
	संरचना task_struct	*p;
	ध्रुव संरचना sched_class *class;
	पूर्णाङ्क			flags;
	bool			queued;
	bool			running;
};

संरचना sched_change_ctx *sched_change_begin(संरचना task_struct *p, unsigned पूर्णाङ्क flags);
शून्य sched_change_end(संरचना sched_change_ctx *ctx);

DEFINE_CLASS(sched_change, संरचना sched_change_ctx *,
	     sched_change_end(_T),
	     sched_change_begin(p, flags),
	     संरचना task_struct *p, unsigned पूर्णाङ्क flags)

DEFINE_CLASS_IS_UNCONDITIONAL(sched_change)

#अन्तर्भाव "ext/ext.h"

#ifdef CONFIG_PREFERRED_CPU
शून्य sched_प्रक्षिप_current_non_preferred_cpu(संरचना rq *rq);
#अण्णहा	/* !CONFIG_PREFERRED_CPU */
स्थिर अन्तःस्थ शून्य sched_प्रक्षिप_current_non_preferred_cpu(संरचना rq *rq) { }
#endif

#endif /* _KERNEL_SCHED_SCHED_H */
