/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Scheduler internal types and methods:
 */
#अगर_अघोषित _KERNEL_SCHED_SCHED_H
#घोषणा _KERNEL_SCHED_SCHED_H

#समावेश <linux/prandom.h>
#समावेश <linux/sched/affinity.h>
#समावेश <linux/sched/autogroup.h>
#समावेश <linux/sched/cpufreq.h>
#समावेश <linux/sched/deadline.h>
#समावेश <linux/sched.h>
#समावेश <linux/sched/loadavg.h>
#समावेश <linux/sched/mm.h>
#समावेश <linux/sched/rseq_api.h>
#समावेश <linux/sched/signal.h>
#समावेश <linux/sched/smt.h>
#समावेश <linux/sched/stat.h>
#समावेश <linux/sched/sysctl.h>
#समावेश <linux/sched/task_flags.h>
#समावेश <linux/sched/task.h>
#समावेश <linux/sched/topology.h>
#समावेश <linux/atomic.h>
#समावेश <linux/bitmap.h>
#समावेश <linux/bug.h>
#समावेश <linux/capability.h>
#समावेश <linux/cgroup_api.h>
#समावेश <linux/cgroup.h>
#समावेश <linux/context_tracking.h>
#समावेश <linux/cpufreq.h>
#समावेश <linux/cpumask_api.h>
#समावेश <linux/cpuset.h>
#समावेश <linux/ctype.h>
#समावेश <linux/file.h>
#समावेश <linux/fs_api.h>
#समावेश <linux/hrtimer_api.h>
#समावेश <linux/interrupt.h>
#समावेश <linux/irq_work.h>
#समावेश <linux/jiffies.h>
#समावेश <linux/kref_api.h>
#समावेश <linux/kthread.h>
#समावेश <linux/ktime_api.h>
#समावेश <linux/lockdep_api.h>
#समावेश <linux/lockdep.h>
#समावेश <linux/memblock.h>
#समावेश <linux/memcontrol.h>
#समावेश <linux/minmax.h>
#समावेश <linux/mm.h>
#समावेश <linux/module.h>
#समावेश <linux/mutex_api.h>
#समावेश <linux/plist.h>
#समावेश <linux/poll.h>
#समावेश <linux/proc_fs.h>
#समावेश <linux/profile.h>
#समावेश <linux/psi.h>
#समावेश <linux/rcupdate.h>
#समावेश <linux/seq_file.h>
#समावेश <linux/seqlock.h>
#समावेश <linux/softirq.h>
#समावेश <linux/spinlock_api.h>
#समावेश <linux/static_key.h>
#समावेश <linux/stop_machine.h>
#समावेश <linux/syscalls_api.h>
#समावेश <linux/syscalls.h>
#समावेश <linux/tick.h>
#समावेश <linux/topology.h>
#समावेश <linux/types.h>
#समावेश <linux/u64_stats_sync_api.h>
#समावेश <linux/uaccess.h>
#समावेश <linux/vmstat.h>
#समावेश <linux/wait_api.h>
#समावेश <linux/wait_bit.h>
#समावेश <linux/workqueue_api.h>
#समावेश <linux/delayacct.h>
#समावेश <linux/mmu_context.h>

#समावेश <trace/events/power.h>
#समावेश <trace/events/sched.h>

#समावेश "../workqueue_internal.h"

काष्ठा rq;
काष्ठा cfs_rq;
काष्ठा rt_rq;
काष्ठा sched_group;
काष्ठा cpuidle_state;

#अगर defined(CONFIG_PARAVIRT) && !defined(CONFIG_HAVE_PV_STEAL_CLOCK_GEN)
# include <asm/paravirt.h>
#पूर्ण_अगर

#समावेश <asm/barrier.h>

#समावेश "cpupri.h"
#समावेश "cpudeadline.h"

/* task_struct::on_rq states: */
#घोषणा TASK_ON_RQ_QUEUED	1
#घोषणा TASK_ON_RQ_MIGRATING	2

बाह्य __read_mostly पूर्णांक scheduler_running;

बाह्य अचिन्हित दीर्घ calc_load_update;
बाह्य atomic_long_t calc_load_tasks;

बाह्य व्योम calc_global_load_tick(काष्ठा rq *this_rq);
बाह्य दीर्घ calc_load_fold_active(काष्ठा rq *this_rq, दीर्घ adjust);

बाह्य व्योम call_trace_sched_update_nr_running(काष्ठा rq *rq, पूर्णांक गिनती);

बाह्य पूर्णांक sysctl_sched_rt_period;
बाह्य पूर्णांक sysctl_sched_rt_runtime;
बाह्य पूर्णांक sched_rr_timeslice;

/*
 * Asymmetric CPU capacity bits
 */
काष्ठा asym_cap_तथ्य {
	काष्ठा list_शीर्ष link;
	काष्ठा rcu_शीर्ष rcu;
	अचिन्हित दीर्घ capacity;
	अचिन्हित दीर्घ cpus[];
};

बाह्य काष्ठा list_शीर्ष asym_cap_list;

#घोषणा cpu_capacity_span(asym_तथ्य) to_cpumask((asym_तथ्य)->cpus)

/*
 * Helpers for converting nanosecond timing to jiffy resolution
 */
#घोषणा NS_TO_JIFFIES(समय)	((अचिन्हित दीर्घ)(समय) / (NSEC_PER_SEC/HZ))

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
#अगर_घोषित CONFIG_64BIT
# define NICE_0_LOAD_SHIFT	(SCHED_FIXEDPOINT_SHIFT + SCHED_FIXEDPOINT_SHIFT)
# define scale_load(w)		((w) << SCHED_FIXEDPOINT_SHIFT)
# define scale_load_down(w)					\
({								\
	अचिन्हित दीर्घ __w = (w);				\
								\
	अगर (__w)						\
		__w = max(2UL, __w >> SCHED_FIXEDPOINT_SHIFT);	\
	__w;							\
})
#अन्यथा
# define NICE_0_LOAD_SHIFT	(SCHED_FIXEDPOINT_SHIFT)
# define scale_load(w)		(w)
# define scale_load_down(w)	(w)
#पूर्ण_अगर

/*
 * Task weight (visible to users) and its load (invisible to users) have
 * independent resolution, but they should be well calibrated. We use
 * scale_load() and scale_load_down(w) to convert between them. The
 * following must be true:
 *
 *  scale_load(sched_prio_to_weight[NICE_TO_PRIO(0)-MAX_RT_PRIO]) == NICE_0_LOAD
 *
 */
#घोषणा NICE_0_LOAD		(1L << NICE_0_LOAD_SHIFT)

/*
 * Single value that decides SCHED_DEADLINE internal math precision.
 * 10 -> just above 1us
 * 9  -> just above 0.5us
 */
#घोषणा DL_SCALE		10

/*
 * Single value that denotes runtime == period, ie unlimited time.
 */
#घोषणा RUNTIME_INF		((u64)~0ULL)

अटल अंतरभूत पूर्णांक idle_policy(पूर्णांक policy)
{
	वापस policy == SCHED_IDLE;
}

अटल अंतरभूत पूर्णांक normal_policy(पूर्णांक policy)
{
#अगर_घोषित CONFIG_SCHED_CLASS_EXT
	अगर (policy == SCHED_EXT)
		वापस true;
#पूर्ण_अगर
	वापस policy == SCHED_NORMAL;
}

अटल अंतरभूत पूर्णांक fair_policy(पूर्णांक policy)
{
	वापस normal_policy(policy) || policy == SCHED_BATCH;
}

अटल अंतरभूत पूर्णांक rt_policy(पूर्णांक policy)
{
	वापस policy == SCHED_FIFO || policy == SCHED_RR;
}

अटल अंतरभूत पूर्णांक dl_policy(पूर्णांक policy)
{
	वापस policy == SCHED_DEADLINE;
}

अटल अंतरभूत bool valid_policy(पूर्णांक policy)
{
	वापस idle_policy(policy) || fair_policy(policy) ||
		rt_policy(policy) || dl_policy(policy);
}

अटल अंतरभूत पूर्णांक task_has_idle_policy(काष्ठा task_struct *p)
{
	वापस idle_policy(p->policy);
}

अटल अंतरभूत पूर्णांक task_has_rt_policy(काष्ठा task_struct *p)
{
	वापस rt_policy(p->policy);
}

अटल अंतरभूत पूर्णांक task_has_dl_policy(काष्ठा task_struct *p)
{
	वापस dl_policy(p->policy);
}

#घोषणा cap_scale(v, s)		((v)*(s) >> SCHED_CAPACITY_SHIFT)

अटल अंतरभूत व्योम update_avg(u64 *avg, u64 sample)
{
	s64 diff = sample - *avg;

	*avg += diff / 8;
}

/*
 * Shifting a value by an exponent greater *or equal* to the size of said value
 * is UB; cap at size-1.
 */
#घोषणा shr_bound(val, shift)							\
	(val >> min_t(typeof(shift), shift, BITS_PER_TYPE(typeof(val)) - 1))

/*
 * cgroup weight knobs should use the common MIN, DFL and MAX values which are
 * 1, 100 and 10000 respectively. While it loses a bit of range on both ends, it
 * maps pretty well onto the shares value used by scheduler and the round-trip
 * conversions preserve the original value over the entire range.
 */
अटल अंतरभूत अचिन्हित दीर्घ sched_weight_from_cgroup(अचिन्हित दीर्घ cgrp_weight)
{
	वापस DIV_ROUND_CLOSEST_ULL(cgrp_weight * 1024, CGROUP_WEIGHT_DFL);
}

अटल अंतरभूत अचिन्हित दीर्घ sched_weight_to_cgroup(अचिन्हित दीर्घ weight)
{
	वापस clamp_t(अचिन्हित दीर्घ,
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
#घोषणा SCHED_FLAG_SUGOV	0x10000000

#घोषणा SCHED_DL_FLAGS		(SCHED_FLAG_RECLAIM | SCHED_FLAG_DL_OVERRUN | SCHED_FLAG_SUGOV)

अटल अंतरभूत bool dl_entity_is_special(स्थिर काष्ठा sched_dl_entity *dl_se)
{
#अगर_घोषित CONFIG_CPU_FREQ_GOV_SCHEDUTIL
	वापस unlikely(dl_se->flags & SCHED_FLAG_SUGOV);
#अन्यथा
	वापस false;
#पूर्ण_अगर
}

/*
 * Tells if entity @a should preempt entity @b.
 */
अटल अंतरभूत bool dl_entity_preempt(स्थिर काष्ठा sched_dl_entity *a,
				     स्थिर काष्ठा sched_dl_entity *b)
{
	वापस dl_entity_is_special(a) ||
	       dl_time_before(a->deadline, b->deadline);
}

/*
 * This is the priority-queue data structure of the RT scheduling class:
 */
काष्ठा rt_prio_array {
	DECLARE_BITMAP(bitmap, MAX_RT_PRIO+1); /* include 1 bit for delimiter */
	काष्ठा list_शीर्ष queue[MAX_RT_PRIO];
};

काष्ठा rt_bandwidth {
	/* nests inside the rq lock: */
	raw_spinlock_t		rt_runtime_lock;
	ktime_t			rt_period;
	u64			rt_runtime;
	काष्ठा hrtimer		rt_period_timer;
	अचिन्हित पूर्णांक		rt_period_active;
};

अटल अंतरभूत पूर्णांक dl_bandwidth_enabled(व्योम)
{
	वापस sysctl_sched_rt_runtime >= 0;
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
काष्ठा dl_bw {
	raw_spinlock_t		lock;
	u64			bw;
	u64			योग_bw;
};

बाह्य व्योम init_dl_bw(काष्ठा dl_bw *dl_b);
बाह्य पूर्णांक  sched_dl_global_validate(व्योम);
बाह्य व्योम sched_dl_do_global(व्योम);
बाह्य पूर्णांक  sched_dl_overflow(काष्ठा task_struct *p, पूर्णांक policy, स्थिर काष्ठा sched_attr *attr);
बाह्य व्योम __setparam_dl(काष्ठा task_struct *p, स्थिर काष्ठा sched_attr *attr);
बाह्य व्योम __getparam_dl(काष्ठा task_struct *p, काष्ठा sched_attr *attr, अचिन्हित पूर्णांक flags);
बाह्य bool __checkparam_dl(स्थिर काष्ठा sched_attr *attr);
बाह्य bool dl_param_changed(काष्ठा task_struct *p, स्थिर काष्ठा sched_attr *attr);
बाह्य पूर्णांक  dl_cpuset_cpumask_can_shrink(स्थिर काष्ठा cpumask *cur, स्थिर काष्ठा cpumask *trial);
बाह्य पूर्णांक  dl_bw_deactivate(पूर्णांक cpu);
बाह्य s64 dl_scaled_delta_exec(काष्ठा rq *rq, काष्ठा sched_dl_entity *dl_se, s64 delta_exec);
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
बाह्य व्योम dl_server_update_idle(काष्ठा sched_dl_entity *dl_se, s64 delta_exec);
बाह्य व्योम dl_server_update(काष्ठा sched_dl_entity *dl_se, s64 delta_exec);
बाह्य व्योम dl_server_start(काष्ठा sched_dl_entity *dl_se);
बाह्य व्योम dl_server_stop(काष्ठा sched_dl_entity *dl_se);
बाह्य व्योम dl_server_init(काष्ठा sched_dl_entity *dl_se, काष्ठा rq *rq,
		    dl_server_pick_f pick_task);
बाह्य व्योम sched_init_dl_servers(व्योम);

बाह्य व्योम fair_server_init(काष्ठा rq *rq);
बाह्य व्योम ext_server_init(काष्ठा rq *rq);
बाह्य व्योम __dl_server_attach_root(काष्ठा sched_dl_entity *dl_se, काष्ठा rq *rq);
बाह्य पूर्णांक dl_server_apply_params(काष्ठा sched_dl_entity *dl_se,
		    u64 runtime, u64 period, bool init);
बाह्य पूर्णांक dl_server_attach_bw(काष्ठा sched_dl_entity *dl_se);
बाह्य व्योम dl_server_detach_bw(काष्ठा sched_dl_entity *dl_se);
बाह्य पूर्णांक dl_server_swap_bw(काष्ठा sched_dl_entity *detach_se,
			     काष्ठा sched_dl_entity *attach_se);

अटल अंतरभूत bool dl_server_active(काष्ठा sched_dl_entity *dl_se)
{
	वापस dl_se->dl_server_active;
}

#अगर_घोषित CONFIG_CGROUP_SCHED

बाह्य काष्ठा list_शीर्ष task_groups;

#अगर_घोषित CONFIG_GROUP_SCHED_BANDWIDTH
बाह्य स्थिर u64 max_bw_quota_period_us;

/*
 * default period for group bandwidth.
 * default: 0.1s, units: microseconds
 */
अटल अंतरभूत u64 default_bw_period_us(व्योम)
{
	वापस 100000ULL;
}
#पूर्ण_अगर /* CONFIG_GROUP_SCHED_BANDWIDTH */

काष्ठा cfs_bandwidth {
#अगर_घोषित CONFIG_CFS_BANDWIDTH
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
	काष्ठा hrtimer		period_timer;
	काष्ठा hrtimer		slack_timer;
	काष्ठा list_शीर्ष	throttled_cfs_rq;

	/* Statistics: */
	पूर्णांक			nr_periods;
	पूर्णांक			nr_throttled;
	पूर्णांक			nr_burst;
	u64			throttled_time;
	u64			burst_time;
#पूर्ण_अगर /* CONFIG_CFS_BANDWIDTH */
};

/* Task group related information */
काष्ठा task_group {
	काष्ठा cgroup_subsys_state css;

#अगर_घोषित CONFIG_GROUP_SCHED_WEIGHT
	/* A positive value indicates that this is a SCHED_IDLE group. */
	पूर्णांक			idle;
#पूर्ण_अगर

#अगर_घोषित CONFIG_FAIR_GROUP_SCHED
	/* runqueue "owned" by this group on each CPU */
	काष्ठा cfs_rq __percpu	*cfs_rq;
	अचिन्हित दीर्घ		shares;
	/*
	 * load_avg can be heavily contended at clock tick time, so put
	 * it in its own cache-line separated from the fields above which
	 * will also be accessed at each tick.
	 */
	atomic_long_t		load_avg ____cacheline_aligned;
	atomic_long_t		runnable_avg;

#पूर्ण_अगर /* CONFIG_FAIR_GROUP_SCHED */

#अगर_घोषित CONFIG_RT_GROUP_SCHED
	काष्ठा sched_rt_entity	**rt_se;
	काष्ठा rt_rq		**rt_rq;

	काष्ठा rt_bandwidth	rt_bandwidth;
#पूर्ण_अगर

	काष्ठा scx_task_group	scx;

	काष्ठा rcu_शीर्ष		rcu;
	काष्ठा list_शीर्ष	list;

	काष्ठा task_group	*parent;
	काष्ठा list_शीर्ष	siblings;
	काष्ठा list_शीर्ष	children;

#अगर_घोषित CONFIG_SCHED_AUTOGROUP
	काष्ठा autogroup	*autogroup;
#पूर्ण_अगर

	काष्ठा cfs_bandwidth	cfs_bandwidth;

#अगर_घोषित CONFIG_UCLAMP_TASK_GROUP
	/* The two decimal precision [%] value requested from user-space */
	अचिन्हित पूर्णांक		uclamp_pct[UCLAMP_CNT];
	/* Clamp values requested for a task group */
	काष्ठा uclamp_se	uclamp_req[UCLAMP_CNT];
	/* Effective clamp values used for a task group */
	काष्ठा uclamp_se	uclamp[UCLAMP_CNT];
#पूर्ण_अगर

};

#घोषणा मूल_TASK_GROUP_LOAD	NICE_0_LOAD

प्रकार पूर्णांक (*tg_visitor)(काष्ठा task_group *, व्योम *);

बाह्य पूर्णांक walk_tg_वृक्ष_from(काष्ठा task_group *from,
			     tg_visitor down, tg_visitor up, व्योम *तथ्य);

/*
 * Iterate the full tree, calling @down when first entering a node and @up when
 * leaving it for the final time.
 *
 * Caller must hold rcu_lock or sufficient equivalent.
 */
अटल अंतरभूत पूर्णांक walk_tg_वृक्ष(tg_visitor down, tg_visitor up, व्योम *तथ्य)
{
	वापस walk_tg_वृक्ष_from(&मूल_task_group, down, up, तथ्य);
}

अटल अंतरभूत काष्ठा task_group *css_tg(काष्ठा cgroup_subsys_state *css)
{
	वापस css ? container_of(css, काष्ठा task_group, css) : शून्य;
}

बाह्य पूर्णांक tg_nop(काष्ठा task_group *tg, व्योम *तथ्य);

#अगर_घोषित CONFIG_FAIR_GROUP_SCHED
बाह्य व्योम free_fair_sched_group(काष्ठा task_group *tg);
बाह्य पूर्णांक alloc_fair_sched_group(काष्ठा task_group *tg, काष्ठा task_group *parent);
बाह्य व्योम online_fair_sched_group(काष्ठा task_group *tg);
बाह्य व्योम unregister_fair_sched_group(काष्ठा task_group *tg);
बाह्य व्योम __sched_cgroup_mode_update(पूर्णांक mode);
#अन्यथा /* !CONFIG_FAIR_GROUP_SCHED: */
अटल अंतरभूत व्योम free_fair_sched_group(काष्ठा task_group *tg) { }
अटल अंतरभूत पूर्णांक alloc_fair_sched_group(काष्ठा task_group *tg, काष्ठा task_group *parent)
{
       वापस 1;
}
अटल अंतरभूत व्योम online_fair_sched_group(काष्ठा task_group *tg) { }
अटल अंतरभूत व्योम unregister_fair_sched_group(काष्ठा task_group *tg) { }
#पूर्ण_अगर /* !CONFIG_FAIR_GROUP_SCHED */

बाह्य व्योम init_tg_cfs_entry(काष्ठा task_group *tg, काष्ठा cfs_rq *cfs_rq,
			काष्ठा sched_entity *se, पूर्णांक cpu,
			काष्ठा sched_entity *parent);
बाह्य व्योम init_cfs_bandwidth(काष्ठा cfs_bandwidth *cfs_b, काष्ठा cfs_bandwidth *parent);

बाह्य व्योम __refill_cfs_bandwidth_runtime(काष्ठा cfs_bandwidth *cfs_b);
बाह्य व्योम start_cfs_bandwidth(काष्ठा cfs_bandwidth *cfs_b);
बाह्य व्योम unthrottle_cfs_rq(काष्ठा cfs_rq *cfs_rq);
बाह्य bool cfs_task_bw_constrained(काष्ठा task_struct *p);

बाह्य व्योम init_tg_rt_entry(काष्ठा task_group *tg, काष्ठा rt_rq *rt_rq,
		काष्ठा sched_rt_entity *rt_se, पूर्णांक cpu,
		काष्ठा sched_rt_entity *parent);
बाह्य पूर्णांक sched_group_set_rt_runtime(काष्ठा task_group *tg, दीर्घ rt_runtime_us);
बाह्य पूर्णांक sched_group_set_rt_period(काष्ठा task_group *tg, u64 rt_period_us);
बाह्य दीर्घ sched_group_rt_runtime(काष्ठा task_group *tg);
बाह्य दीर्घ sched_group_rt_period(काष्ठा task_group *tg);
बाह्य पूर्णांक sched_rt_can_attach(काष्ठा task_group *tg, काष्ठा task_struct *tsk);

बाह्य काष्ठा task_group *sched_create_group(काष्ठा task_group *parent);
बाह्य व्योम sched_online_group(काष्ठा task_group *tg,
			       काष्ठा task_group *parent);
बाह्य व्योम sched_destroy_group(काष्ठा task_group *tg);
बाह्य व्योम sched_release_group(काष्ठा task_group *tg);

बाह्य व्योम sched_move_task(काष्ठा task_struct *tsk, bool for_autogroup);

#अगर_घोषित CONFIG_FAIR_GROUP_SCHED
बाह्य पूर्णांक sched_group_set_shares(काष्ठा task_group *tg, अचिन्हित दीर्घ shares);

बाह्य पूर्णांक sched_group_set_idle(काष्ठा task_group *tg, दीर्घ idle);

बाह्य व्योम set_task_rq_fair(काष्ठा sched_entity *se,
			     काष्ठा cfs_rq *पूर्व, काष्ठा cfs_rq *अग्र);
#अन्यथा /* !CONFIG_FAIR_GROUP_SCHED: */
अटल अंतरभूत पूर्णांक sched_group_set_shares(काष्ठा task_group *tg, अचिन्हित दीर्घ shares) { वापस 0; }
अटल अंतरभूत पूर्णांक sched_group_set_idle(काष्ठा task_group *tg, दीर्घ idle) { वापस 0; }
#पूर्ण_अगर /* !CONFIG_FAIR_GROUP_SCHED */

#अन्यथा /* !CONFIG_CGROUP_SCHED: */

काष्ठा cfs_bandwidth { };

अटल अंतरभूत bool cfs_task_bw_constrained(काष्ठा task_struct *p) { वापस false; }

#पूर्ण_अगर /* !CONFIG_CGROUP_SCHED */

/*
 * A weight of 0 or 1 can cause arithmetics problems.
 * A weight of a cfs_rq is the sum of weights of which entities
 * are queued on this cfs_rq, so a weight of a entity should not be
 * too large, so as the shares value of a task group.
 * (The default weight is 1024 - so there's no practical
 *  limitation from this.)
 */
#घोषणा MIN_SHARES		(1UL <<  1)
#घोषणा MAX_SHARES		(1UL << 18)

बाह्य व्योम unregister_rt_sched_group(काष्ठा task_group *tg);
बाह्य व्योम free_rt_sched_group(काष्ठा task_group *tg);
बाह्य पूर्णांक alloc_rt_sched_group(काष्ठा task_group *tg, काष्ठा task_group *parent);

/*
 * u64_u32_load/u64_u32_store
 *
 * Use a copy of a u64 value to protect against data race. This is only
 * applicable for 32-bits architectures.
 */
#अगर_घोषित CONFIG_64BIT
# define u64_u32_load_copy(var, copy)		var
# define u64_u32_store_copy(var, copy, val)	(var = val)
#अन्यथा
# define u64_u32_load_copy(var, copy)					\
({									\
	u64 __val, __val_copy;						\
	करो {								\
		__val_copy = copy;					\
		/*							\
		 * paired with u64_u32_store_copy(), ordering access	\
		 * to var and copy.					\
		 */							\
		smp_rmb();						\
		__val = var;						\
	} जबतक (__val != __val_copy);					\
	__val;								\
})
# define u64_u32_store_copy(var, copy, val)				\
करो {									\
	typeof(val) __val = (val);					\
	var = __val;							\
	/*								\
	 * paired with u64_u32_load_copy(), ordering access to var and	\
	 * copy.							\
	 */								\
	smp_wmb();							\
	copy = __val;							\
} जबतक (0)
#पूर्ण_अगर
# define u64_u32_load(var)		u64_u32_load_copy(var, var##_copy)
# define u64_u32_store(var, val)	u64_u32_store_copy(var, var##_copy, val)

काष्ठा balance_callback {
	काष्ठा balance_callback *अग्र;
	व्योम (*func)(काष्ठा rq *rq);
};

/* Fair scheduling SCHED_{NORMAL,BATCH,IDLE} related fields in a runqueue: */
काष्ठा cfs_rq {
	काष्ठा load_weight	load;
	अचिन्हित पूर्णांक		nr_queued;
	अचिन्हित पूर्णांक		h_nr_queued;		/* SCHED_{NORMAL,BATCH,IDLE} */
	अचिन्हित पूर्णांक		h_nr_runnable;		/* SCHED_{NORMAL,BATCH,IDLE} */
	अचिन्हित पूर्णांक		h_nr_idle;		/* SCHED_IDLE */

	s64			योग_w_vruntime;
	u64			योग_weight;
	u64			zero_vruntime;
	अचिन्हित पूर्णांक		योग_shift;

#अगर_घोषित CONFIG_SCHED_CORE
	अचिन्हित पूर्णांक		forceidle_seq;
	u64			zero_vruntime_fi;
#पूर्ण_अगर

	काष्ठा rb_मूल_cached	tasks_timeline;

	/*
	 * 'curr' points to the currently running entity on this cfs_rq.
	 * It is set to NULL otherwise (i.e when none are currently running).
	 */
	काष्ठा sched_entity	*curr;
	काष्ठा sched_entity	*अग्र;

	/*
	 * CFS load tracking
	 */
	काष्ठा sched_entity	*h_curr;
	काष्ठा sched_avg	avg;
#अगर_अघोषित CONFIG_64BIT
	u64			last_update_time_copy;
#पूर्ण_अगर
	काष्ठा {
		raw_spinlock_t	lock ____cacheline_aligned;
		पूर्णांक		nr;
		अचिन्हित दीर्घ	load_avg;
		अचिन्हित दीर्घ	util_avg;
		अचिन्हित दीर्घ	runnable_avg;
	} removed;

#अगर_घोषित CONFIG_FAIR_GROUP_SCHED
	u64			last_update_tg_load_avg;
	अचिन्हित दीर्घ		tg_load_avg_contrib;
	अचिन्हित दीर्घ		tg_runnable_avg_contrib;
	दीर्घ			propagate;
	दीर्घ			prop_runnable_योग;

	/*
	 *   h_load = weight * f(tg)
	 *
	 * Where f(tg) is the recursive weight fraction assigned to
	 * this group.
	 */
	अचिन्हित दीर्घ		h_load;
	u64			last_h_load_update;
	काष्ठा sched_entity	*h_load_अग्र;

	काष्ठा rq		*rq;	/* CPU runqueue to which this cfs_rq is attached */

	/*
	 * leaf cfs_rqs are those that hold tasks (lowest schedulable entity in
	 * a hierarchy). Non-leaf lrqs hold other higher schedulable entities
	 * (like users, containers etc.)
	 *
	 * leaf_cfs_rq_list ties together list of leaf cfs_rq's in a CPU.
	 * This list is used during load balance.
	 */
	पूर्णांक			on_list;
	काष्ठा list_शीर्ष	leaf_cfs_rq_list;
	काष्ठा task_group	*tg;	/* Group that "owns" this runqueue */

	/* Locally cached copy of our task_group's idle value */
	पूर्णांक			idle;

# ifdef CONFIG_CFS_BANDWIDTH
	पूर्णांक			runtime_enabled;
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
	पूर्णांक			throttle_गिनती;
	काष्ठा list_शीर्ष	throttled_list;
	काष्ठा list_शीर्ष	throttled_csd_list;
	काष्ठा list_शीर्ष        throttled_limbo_list;
# endif /* CONFIG_CFS_BANDWIDTH */
#पूर्ण_अगर /* CONFIG_FAIR_GROUP_SCHED */
};

#अगर_घोषित CONFIG_SCHED_CLASS_EXT
/* scx_rq->flags, protected by the rq lock */
क्रमागत scx_rq_flags {
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
	SCX_RQ_मूल_IDLE_RENOTIFY	= 1 << 8, /* the root is owed update_idle() */

	SCX_RQ_IN_WAKEUP	= 1 << 16,
	SCX_RQ_IN_DISPATCH	= 1 << 17,
};

/* per-rq rescue execution state, see scx_rescue_timerfn() */
काष्ठा scx_rq_rescue {
	काष्ठा scx_dispatch_q	dsq;			/* stranded tasks awaiting rescue */
	s64			budget;			/* execution token bucket, ns */
	u64			घड़ी;			/* last budget accrual timestamp */
	काष्ठा task_struct	*curr;			/* task being rescued, one at a time */
	s64			slice;			/* curr's admitted slice */
	u64			exec_snap;		/* sum_exec_runtime at admission */
	काष्ठा timer_list	timer;			/* paces admission and escalation */
	u64			kill_at;		/* last ejection, init before any */
};

काष्ठा scx_rq {
	काष्ठा scx_dispatch_q	local_dsq;
#अगर_घोषित CONFIG_EXT_SUB_SCHED
	काष्ठा scx_dispatch_q	reject_dsq;		/* staging for cap-rejected tasks */
	काष्ठा scx_rq_rescue	rescue;
#पूर्ण_अगर
	काष्ठा list_शीर्ष	runnable_list;		/* runnable tasks on this rq */
	काष्ठा list_शीर्ष	ddsp_deferred_locals;	/* deferred ddsps from enq */
	/* both stashed across the activate_task() in move_remote_task_to_local_dsq() */
	u64			remote_activate_enq_flags;
	काष्ठा scx_sched	*remote_activate_sch;
	u32			nr_running;
	u32			cpuperf_target;		/* [0, SCHED_CAPACITY_SCALE] */
	bool			in_select_cpu;
	bool			cpu_released;
	u32			flags;
	u32			nr_immed;		/* ENQ_IMMED tasks on local_dsq */
#अगर_घोषित CONFIG_SCHED_CORE
	u32			lock_drop_seq;	/* nr dispatch lock releases */
#पूर्ण_अगर
	u64			घड़ी;			/* current per-rq clock -- see scx_bpf_now() */
#अगर_घोषित CONFIG_EXT_SUB_SCHED
	काष्ठा llist_शीर्ष	ecaps_to_sync;		/* pending ecaps syncs */
	काष्ठा task_struct	*sub_dispatch_पूर्व;
#पूर्ण_अगर
	cpumask_var_t		cpus_to_sync;
	bool			kick_sync_pending;
	अचिन्हित दीर्घ		kick_sync;

	काष्ठा list_शीर्ष	sched_pcpus_to_kick;	/* see kick_cpus_irq_workfn() */

	raw_spinlock_t		deferred_reenq_lock;
	काष्ठा list_शीर्ष	deferred_reenq_locals;	/* scheds requesting reenq of local DSQ */
	काष्ठा list_शीर्ष	deferred_reenq_users;	/* user DSQs requesting reenq */
	काष्ठा balance_callback	deferred_bal_cb;
	काष्ठा balance_callback	kick_sync_bal_cb;
	काष्ठा irq_work		deferred_irq_work;
	काष्ठा irq_work		kick_cpus_irq_work;
};
#पूर्ण_अगर /* CONFIG_SCHED_CLASS_EXT */

अटल अंतरभूत पूर्णांक rt_bandwidth_enabled(व्योम)
{
	वापस sysctl_sched_rt_runtime >= 0;
}

/* RT IPI pull logic requires IRQ_WORK */
#अगर defined(CONFIG_IRQ_WORK) && defined(CONFIG_SMP)
# define HAVE_RT_धकेलो_IPI
#पूर्ण_अगर

/* Real-Time classes' related field in a runqueue: */
काष्ठा rt_rq {
	काष्ठा rt_prio_array	active;
	अचिन्हित पूर्णांक		rt_nr_running;
	अचिन्हित पूर्णांक		rr_nr_running;
	काष्ठा {
		पूर्णांक		curr; /* highest queued rt task prio */
		पूर्णांक		अग्र; /* next highest */
	} highest_prio;
	bool			overloaded;
	काष्ठा plist_शीर्ष	pushable_tasks;

	पूर्णांक			rt_queued;

#अगर_घोषित CONFIG_RT_GROUP_SCHED
	पूर्णांक			rt_throttled;
	u64			rt_time; /* consumed RT time, goes up in update_curr_rt */
	u64			rt_runtime; /* allotted RT time, "slice" from rt_bandwidth, RT sharing/balancing */
	/* Nests inside the rq lock: */
	raw_spinlock_t		rt_runtime_lock;

	अचिन्हित पूर्णांक		rt_nr_boosted;

	काष्ठा rq		*rq; /* this is always top-level rq, cache? */
#पूर्ण_अगर
#अगर_घोषित CONFIG_CGROUP_SCHED
	काष्ठा task_group	*tg; /* this tg has "this" rt_rq on given CPU for runnable entities */
#पूर्ण_अगर
};

अटल अंतरभूत bool rt_rq_is_runnable(काष्ठा rt_rq *rt_rq)
{
	वापस rt_rq->rt_queued && rt_rq->rt_nr_running;
}

/* Deadline class' related fields in a runqueue */
काष्ठा dl_rq {
	/* runqueue is an rbtree, ordered by deadline */
	काष्ठा rb_मूल_cached	मूल;

	अचिन्हित पूर्णांक		dl_nr_running;

	/*
	 * Deadline values of the currently executing and the
	 * earliest ready task on this rq. Caching these facilitates
	 * the decision whether or not a ready but not running task
	 * should migrate somewhere else.
	 */
	काष्ठा {
		u64		curr;
		u64		अग्र;
	} earliest_dl;

	bool			overloaded;

	काष्ठा sched_dl_entity	*curr;
	/*
	 * Tasks on this rq that can be pushed away. They are kept in
	 * an rb-tree, ordered by tasks' deadlines, with caching
	 * of the leftmost (earliest deadline) element.
	 */
	काष्ठा rb_मूल_cached	pushable_dl_tasks_मूल;

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

#अगर_घोषित CONFIG_FAIR_GROUP_SCHED
/* Check whether a task group is root tg */
#घोषणा is_मूल_task_group(tg) ((tg) == &मूल_task_group)
/* An entity is a task if it doesn't "own" a runqueue */
#घोषणा entity_is_task(se)	(!se->my_q)

अटल अंतरभूत व्योम se_update_runnable(काष्ठा sched_entity *se)
{
	अगर (!entity_is_task(se))
		se->runnable_weight = se->my_q->h_nr_runnable;
}

अटल अंतरभूत दीर्घ se_runnable(काष्ठा sched_entity *se)
{
	अगर (se->sched_delayed)
		वापस false;

	अगर (entity_is_task(se))
		वापस !!se->on_rq;
	अन्यथा
		वापस se->runnable_weight;
}

#अन्यथा /* !CONFIG_FAIR_GROUP_SCHED: */

#घोषणा entity_is_task(se)	1

अटल अंतरभूत व्योम se_update_runnable(काष्ठा sched_entity *se) { }

अटल अंतरभूत दीर्घ se_runnable(काष्ठा sched_entity *se)
{
	अगर (se->sched_delayed)
		वापस false;

	वापस !!se->on_rq;
}

#पूर्ण_अगर /* !CONFIG_FAIR_GROUP_SCHED */

/*
 * XXX we want to get rid of these helpers and use the full load resolution.
 */
अटल अंतरभूत दीर्घ se_weight(काष्ठा sched_entity *se)
{
	वापस scale_load_down(se->load.weight);
}


अटल अंतरभूत bool sched_asym_prefer(पूर्णांक a, पूर्णांक b)
{
	वापस arch_asym_cpu_priority(a) > arch_asym_cpu_priority(b);
}

काष्ठा perf_domain {
	काष्ठा em_perf_domain *em_pd;
	काष्ठा perf_domain *अग्र;
	काष्ठा rcu_शीर्ष rcu;
};

/*
 * We add the notion of a root-domain which will be used to define per-domain
 * variables. Each exclusive cpuset essentially defines an island domain by
 * fully partitioning the member CPUs from any other cpuset. Whenever a new
 * exclusive cpuset is created, we also create and attach a new root-domain
 * object.
 *
 */
काष्ठा मूल_domain {
	atomic_t		refcount;
	atomic_t		rto_गिनती;
	काष्ठा rcu_शीर्ष		rcu;
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
	atomic_t		dlo_गिनती;
	काष्ठा dl_bw		dl_bw;
	काष्ठा cpudl		cpudl;

	/*
	 * Indicate whether a root_domain's dl_bw has been checked or
	 * updated. It's monotonously increasing value.
	 *
	 * Also, some corner cases, like 'wrap around' is dangerous, but given
	 * that u64 is 'big enough'. So that shouldn't be a concern.
	 */
	u64 visit_cookie;

#अगर_घोषित HAVE_RT_धकेलो_IPI
	/*
	 * For IPI pull requests, loop across the rto_mask.
	 */
	काष्ठा irq_work		rto_धकेलो_work;
	raw_spinlock_t		rto_lock;
	/* These are only updated and read within rto_lock */
	पूर्णांक			rto_loop;
	पूर्णांक			rto_cpu;
	/* These atomics are updated outside of a lock */
	atomic_t		rto_loop_अग्र;
	atomic_t		rto_loop_start;
#पूर्ण_अगर /* HAVE_RT_PUSH_IPI */
	/*
	 * The "RT overload" flag: it gets set if a CPU has more than
	 * one runnable RT task.
	 */
	cpumask_var_t		rto_mask;
	काष्ठा cpupri		cpupri;

	/*
	 * NULL-terminated list of performance domains intersecting with the
	 * CPUs of the rd. Protected by RCU.
	 */
	काष्ठा perf_domain __rcu *pd;
};

बाह्य व्योम init_defrootdomain(व्योम);
बाह्य पूर्णांक sched_init_domains(स्थिर काष्ठा cpumask *cpu_map);
बाह्य व्योम rq_attach_मूल(काष्ठा rq *rq, काष्ठा मूल_domain *rd);
बाह्य व्योम sched_get_rd(काष्ठा मूल_domain *rd);
बाह्य व्योम sched_put_rd(काष्ठा मूल_domain *rd);

अटल अंतरभूत पूर्णांक get_rd_overloaded(काष्ठा मूल_domain *rd)
{
	वापस READ_ONCE(rd->overloaded);
}

अटल अंतरभूत व्योम set_rd_overloaded(काष्ठा मूल_domain *rd, पूर्णांक status)
{
	अगर (get_rd_overloaded(rd) != status)
		WRITE_ONCE(rd->overloaded, status);
}

#अगर_घोषित HAVE_RT_धकेलो_IPI
बाह्य व्योम rto_धकेलो_irq_work_func(काष्ठा irq_work *work);
#पूर्ण_अगर

#अगर_घोषित CONFIG_UCLAMP_TASK
/*
 * struct uclamp_bucket - Utilization clamp bucket
 * @value: utilization clamp value for tasks on this clamp bucket
 * @tasks: number of RUNNABLE tasks on this clamp bucket
 *
 * Keep track of how many tasks are RUNNABLE for a given utilization
 * clamp value.
 */
काष्ठा uclamp_bucket {
	अचिन्हित दीर्घ मूल्य : bits_per(SCHED_CAPACITY_SCALE);
	अचिन्हित दीर्घ tasks : BITS_PER_LONG - bits_per(SCHED_CAPACITY_SCALE);
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
काष्ठा uclamp_rq {
	अचिन्हित पूर्णांक मूल्य;
	काष्ठा uclamp_bucket bucket[UCLAMP_BUCKETS];
};

DECLARE_STATIC_कुंजी_FALSE(sched_uclamp_used);
#पूर्ण_अगर /* CONFIG_UCLAMP_TASK */

/*
 * This is the main, per-CPU runqueue data structure.
 *
 * Locking rule: those places that want to lock multiple runqueues
 * (such as the load balancing or the thread migration code), lock
 * acquire operations must be ordered by ascending &runqueue.
 */
काष्ठा rq {
	/*
	 * The following members are loaded together, without holding the
	 * rq->lock, in an extremely hot loop in update_sg_lb_stats()
	 * (called from pick_next_task()). To reduce cache pollution from
	 * this operation, they are placed together on this dedicated cache
	 * line. Even though some of them are frequently modified, they are
	 * loaded much more frequently than they are stored.
	 */
	अचिन्हित पूर्णांक		nr_running;
#अगर_घोषित CONFIG_NUMA_BALANCING
	अचिन्हित पूर्णांक		nr_numa_running;
	अचिन्हित पूर्णांक		nr_preferred_running;
#पूर्ण_अगर
	अचिन्हित पूर्णांक		ttwu_pending;
	अचिन्हित दीर्घ		cpu_capacity;
#अगर_घोषित CONFIG_SCHED_PROXY_EXEC
	काष्ठा task_struct __rcu	*donor;  /* Scheduling context */
	काष्ठा task_struct __rcu	*curr;   /* Execution context */
#अन्यथा
	जोड़ {
		काष्ठा task_struct __rcu *donor; /* Scheduler context */
		काष्ठा task_struct __rcu *curr;  /* Execution context */
	};
#पूर्ण_अगर
	काष्ठा task_struct	*idle;
	/* padding left here deliberately */

	/*
	 * The next cacheline holds the (hot) runqueue lock, as well as
	 * some other less performance-critical fields.
	 */
	u64			nr_switches	____cacheline_aligned;

	/* runqueue lock: */
	raw_spinlock_t		__lock;

#अगर_घोषित CONFIG_NO_HZ_COMMON
	अचिन्हित पूर्णांक		nohz_tick_stopped;
	atomic_t		nohz_flags;
	अचिन्हित पूर्णांक		has_blocked_load;
	अचिन्हित दीर्घ		last_blocked_load_update_tick;
	call_single_तथ्य_t	nohz_csd;
#पूर्ण_अगर /* CONFIG_NO_HZ_COMMON */

#अगर_घोषित CONFIG_UCLAMP_TASK
	/* Utilization clamp values based on CPU's RUNNABLE tasks */
	काष्ठा uclamp_rq	uclamp[UCLAMP_CNT] ____cacheline_aligned;
	अचिन्हित पूर्णांक		uclamp_flags;
#घोषणा UCLAMP_FLAG_IDLE 0x01
#पूर्ण_अगर

	काष्ठा cfs_rq		cfs;
	काष्ठा rt_rq		rt;
	काष्ठा dl_rq		dl;
#अगर_घोषित CONFIG_SCHED_CLASS_EXT
	काष्ठा scx_rq		scx;
	काष्ठा sched_dl_entity	ext_server;
#पूर्ण_अगर
#अगर_घोषित CONFIG_SCHED_CACHE
	raw_spinlock_t		cpu_epoch_lock ____cacheline_aligned;
	u64			cpu_runtime;
	अचिन्हित दीर्घ		cpu_epoch;
	अचिन्हित दीर्घ		cpu_epoch_अग्र;
#पूर्ण_अगर

	काष्ठा sched_dl_entity	fair_server;

#अगर_घोषित CONFIG_FAIR_GROUP_SCHED
	/* list of leaf cfs_rq on this CPU: */
	काष्ठा list_शीर्ष	leaf_cfs_rq_list;
	काष्ठा list_शीर्ष	*tmp_alone_branch;
#पूर्ण_अगर /* CONFIG_FAIR_GROUP_SCHED */

#अगर_घोषित CONFIG_NUMA_BALANCING
	अचिन्हित पूर्णांक		numa_migrate_on;
#पूर्ण_अगर

#अगर_घोषित CONFIG_SCHED_CACHE
	अचिन्हित पूर्णांक		nr_pref_llc_running;
	अचिन्हित पूर्णांक		nr_llc_running;
#पूर्ण_अगर

	/*
	 * This is part of a global counter where only the total sum
	 * over all CPUs matters. A task can increase this counter on
	 * one CPU and if it got migrated afterwards it may decrease
	 * it on another CPU. Always updated under the runqueue lock:
	 */
	अचिन्हित दीर्घ		nr_uninterruptible;

	काष्ठा sched_dl_entity	*dl_server;
	काष्ठा task_struct	*stop;
	स्थिर काष्ठा sched_class *अग्र_class;
	अचिन्हित दीर्घ		अग्र_balance;
	काष्ठा mm_struct	*पूर्व_mm;

	/*
	 * The following fields of clock data are frequently referenced
	 * and updated together, and should go on their own cache line.
	 */
	u64			clock_task ____cacheline_aligned;
	u64			clock_pelt;
	u64			घड़ी;
	अचिन्हित दीर्घ		lost_idle_time;
	अचिन्हित पूर्णांक		clock_update_flags;
	u64			clock_pelt_idle;
	u64			clock_idle;

#अगर_अघोषित CONFIG_64BIT
	u64			clock_pelt_idle_copy;
	u64			clock_idle_copy;
#पूर्ण_अगर

	u64 last_seen_need_resched_ns;
	पूर्णांक ticks_without_resched;

#अगर_घोषित CONFIG_MEMBARRIER
	पूर्णांक membarrier_state;
#पूर्ण_अगर

	काष्ठा मूल_domain		*rd;
	काष्ठा sched_domain __rcu	*sd;

	काष्ठा balance_callback *balance_callback;

	अचिन्हित अक्षर		nohz_idle_balance;
	अचिन्हित अक्षर		idle_balance;

	अचिन्हित दीर्घ		misfit_task_load;

	/* For active balancing */
	पूर्णांक			active_balance;
	पूर्णांक			धकेलो_cpu;
	काष्ठा cpu_stop_work	active_balance_work;

	/* CPU of this runqueue: */
	पूर्णांक			cpu;
	पूर्णांक			online;

	काष्ठा list_शीर्ष cfs_tasks;

	काष्ठा sched_avg	avg_rt;
	काष्ठा sched_avg	avg_dl;
#अगर_घोषित CONFIG_HAVE_SCHED_AVG_IRQ
	काष्ठा sched_avg	avg_irq;
#पूर्ण_अगर
#अगर_घोषित CONFIG_SCHED_HW_PRESSURE
	काष्ठा sched_avg	avg_hw;
#पूर्ण_अगर
	u64			idle_stamp;
	u64			avg_idle;

	/* This is used to determine avg_idle's max value */
	u64			max_idle_balance_cost;

#अगर_घोषित CONFIG_HOTPLUG_CPU
	काष्ठा rcuwait		hotplug_wait;
#पूर्ण_अगर

#अगर_घोषित CONFIG_IRQ_TIME_ACCOUNTING
	u64			पूर्व_irq_time;
	u64			psi_irq_time;
#पूर्ण_अगर
#अगर_घोषित CONFIG_PARAVIRT
	u64			पूर्व_steal_time;
#पूर्ण_अगर
#अगर_घोषित CONFIG_PARAVIRT_TIME_ACCOUNTING
	u64			पूर्व_steal_time_rq;
#पूर्ण_अगर
#अगर_घोषित CONFIG_PREFERRED_CPU
	bool			npc_धकेलो_work_pending;
#पूर्ण_अगर

	/* calc_load related fields */
	अचिन्हित दीर्घ		calc_load_update;
	दीर्घ			calc_load_active;

#अगर_घोषित CONFIG_SCHED_HRTICK
	call_single_तथ्य_t	hrtick_csd;
	काष्ठा hrtimer		hrtick_timer;
	ktime_t			hrtick_time;
	ktime_t			hrtick_delay;
	अचिन्हित पूर्णांक		hrtick_sched;
#पूर्ण_अगर

#अगर_घोषित CONFIG_SCHEDSTATS
	/* latency stats */
	काष्ठा sched_info	rq_sched_info;
	अचिन्हित दीर्घ दीर्घ	rq_cpu_time;

	/* sys_sched_yield() stats */
	अचिन्हित पूर्णांक		yld_गिनती;

	/* schedule() stats */
	अचिन्हित पूर्णांक		sched_गिनती;
	अचिन्हित पूर्णांक		sched_goidle;

	/* try_to_wake_up() stats */
	अचिन्हित पूर्णांक		ttwu_गिनती;
	अचिन्हित पूर्णांक		ttwu_local;
#पूर्ण_अगर

#अगर_घोषित CONFIG_CPU_IDLE
	/* Must be inspected within a RCU lock section */
	काष्ठा cpuidle_state	*idle_state;
#पूर्ण_अगर

	अचिन्हित पूर्णांक		nr_pinned;
	अचिन्हित पूर्णांक		धकेलो_busy;
	काष्ठा cpu_stop_work	धकेलो_work;

#अगर_घोषित CONFIG_SCHED_CORE
	/* per rq */
	काष्ठा rq		*core;
	काष्ठा task_struct	*core_pick;
	काष्ठा sched_dl_entity	*core_dl_server;
	अचिन्हित पूर्णांक		core_enabled;
	काष्ठा rb_मूल		core_वृक्ष;

	/* shared state -- careful with sched_core_cpu_deactivate() */
	अचिन्हित पूर्णांक		core_task_seq;
	अचिन्हित पूर्णांक		core_pick_seq;
	अचिन्हित दीर्घ		core_cookie;
	अचिन्हित पूर्णांक		core_forceidle_गिनती;
	अचिन्हित पूर्णांक		core_forceidle_seq;
	अचिन्हित पूर्णांक		core_forceidle_occupation;
	u64			core_forceidle_start;
	अचिन्हित पूर्णांक		core_pick_in_flight;
#पूर्ण_अगर /* CONFIG_SCHED_CORE */

	/* Scratch cpumask to be temporarily used under rq_lock */
	cpumask_var_t		scratch_mask;

#अगर_घोषित CONFIG_CFS_BANDWIDTH
	call_single_तथ्य_t	cfsb_csd;
	काष्ठा list_शीर्ष	cfsb_csd_list;
#पूर्ण_अगर

	atomic_t		nr_iowait;
} __no_randomize_layout;

#अगर_घोषित CONFIG_FAIR_GROUP_SCHED

/* CPU runqueue to which this cfs_rq is attached */
अटल अंतरभूत काष्ठा rq *rq_of(काष्ठा cfs_rq *cfs_rq)
{
	वापस cfs_rq->rq;
}

#अन्यथा /* !CONFIG_FAIR_GROUP_SCHED: */

अटल अंतरभूत काष्ठा rq *rq_of(काष्ठा cfs_rq *cfs_rq)
{
	वापस container_of(cfs_rq, काष्ठा rq, cfs);
}
#पूर्ण_अगर /* !CONFIG_FAIR_GROUP_SCHED */

अटल अंतरभूत पूर्णांक cpu_of(काष्ठा rq *rq)
{
	वापस rq->cpu;
}

#घोषणा MDF_धकेलो		0x01

अटल अंतरभूत bool is_migration_disabled(काष्ठा task_struct *p)
{
	वापस p->migration_disabled;
}

DECLARE_PER_CPU_SHARED_ALIGNED(काष्ठा rq, runqueues);
DECLARE_PER_CPU(काष्ठा rnd_state, sched_rnd_state);

अटल अंतरभूत u32 sched_rng(व्योम)
{
	वापस prandom_u32_state(this_cpu_ptr(&sched_rnd_state));
}

अटल __always_inline काष्ठा rq *__this_rq(व्योम)
{
	वापस this_cpu_ptr(&runqueues);
}

#घोषणा cpu_rq(cpu)		(&per_cpu(runqueues, (cpu)))
#घोषणा this_rq()		__this_rq()
#घोषणा task_rq(p)		cpu_rq(task_cpu(p))
#घोषणा cpu_curr(cpu)		(cpu_rq(cpu)->curr)
#घोषणा raw_rq()		raw_cpu_ptr(&runqueues)

अटल अंतरभूत bool idle_rq(काष्ठा rq *rq)
{
	वापस rq->curr == rq->idle && !rq->nr_running && !rq->ttwu_pending;
}

/**
 * available_idle_cpu - is a given CPU idle for enqueuing work.
 * @cpu: the CPU in question.
 *
 * Return: 1 if the CPU is currently idle. 0 otherwise.
 */
अटल अंतरभूत bool available_idle_cpu(पूर्णांक cpu)
{
	अगर (!idle_rq(cpu_rq(cpu)))
		वापस 0;

	अगर (vcpu_is_preempted(cpu))
		वापस 0;

	वापस 1;
}

#अगर_घोषित CONFIG_SCHED_PROXY_EXEC
अटल अंतरभूत व्योम rq_set_donor(काष्ठा rq *rq, काष्ठा task_struct *t)
{
	rcu_assign_pointer(rq->donor, t);
}
#अन्यथा
अटल अंतरभूत व्योम rq_set_donor(काष्ठा rq *rq, काष्ठा task_struct *t)
{
	/* Do nothing */
}
#पूर्ण_अगर

#अगर_घोषित CONFIG_SCHED_CORE
अटल अंतरभूत काष्ठा cpumask *sched_group_span(काष्ठा sched_group *sg);

DECLARE_STATIC_कुंजी_FALSE(__sched_core_enabled);

अटल अंतरभूत bool sched_core_enabled(काष्ठा rq *rq)
{
	वापस static_branch_unlikely(&__sched_core_enabled) && rq->core_enabled;
}

अटल अंतरभूत bool sched_core_disabled(व्योम)
{
	वापस !static_branch_unlikely(&__sched_core_enabled);
}

/*
 * Be careful with this function; not for general use. The return value isn't
 * stable unless you actually hold a relevant rq->__lock.
 */
अटल अंतरभूत raw_spinlock_t *rq_lockp(काष्ठा rq *rq)
{
	अगर (sched_core_enabled(rq))
		वापस &rq->core->__lock;

	वापस &rq->__lock;
}

अटल अंतरभूत raw_spinlock_t *__rq_lockp(काष्ठा rq *rq)
	__returns_ctx_lock(rq_lockp(rq)) /* alias them */
{
	अगर (rq->core_enabled)
		वापस &rq->core->__lock;

	वापस &rq->__lock;
}

बाह्य bool
cfs_prio_less(स्थिर काष्ठा task_struct *a, स्थिर काष्ठा task_struct *b, bool fi);

बाह्य व्योम task_vruntime_update(काष्ठा rq *rq, काष्ठा task_struct *p, bool in_fi);

/*
 * Helpers to check if the CPU's core cookie matches with the task's cookie
 * when core scheduling is enabled.
 * A special case is that the task's cookie always matches with CPU's core
 * cookie if the CPU is in an idle core.
 */
अटल अंतरभूत bool sched_cpu_cookie_match(काष्ठा rq *rq, काष्ठा task_struct *p)
{
	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	अगर (!sched_core_enabled(rq))
		वापस true;

	वापस rq->core->core_cookie == p->core_cookie;
}

अटल अंतरभूत bool sched_core_cookie_match(काष्ठा rq *rq, काष्ठा task_struct *p)
{
	bool idle_core = true;
	पूर्णांक cpu;

	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	अगर (!sched_core_enabled(rq))
		वापस true;

	अगर (rq->core->core_cookie == p->core_cookie)
		वापस true;

	for_each_cpu(cpu, cpu_smt_mask(cpu_of(rq))) {
		अगर (!available_idle_cpu(cpu)) {
			idle_core = false;
			अवरोध;
		}
	}

	/*
	 * A CPU in an idle core is always the best choice for tasks with
	 * cookies.
	 */
	वापस idle_core;
}

अटल अंतरभूत bool sched_group_cookie_match(काष्ठा rq *rq,
					    काष्ठा task_struct *p,
					    काष्ठा sched_group *group)
{
	पूर्णांक cpu;

	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	अगर (!sched_core_enabled(rq))
		वापस true;

	for_each_cpu_and(cpu, sched_group_span(group), p->cpus_ptr) {
		अगर (sched_core_cookie_match(cpu_rq(cpu), p))
			वापस true;
	}
	वापस false;
}

अटल अंतरभूत bool sched_core_enqueued(काष्ठा task_struct *p)
{
	वापस !RB_EMPTY_गाँठ(&p->core_गाँठ);
}

बाह्य व्योम sched_core_enqueue(काष्ठा rq *rq, काष्ठा task_struct *p);
बाह्य व्योम sched_core_dequeue(काष्ठा rq *rq, काष्ठा task_struct *p, पूर्णांक flags);

बाह्य व्योम sched_core_get(व्योम);
बाह्य व्योम sched_core_put(व्योम);

अटल अंतरभूत bool task_has_sched_core(काष्ठा task_struct *p)
{
	अगर (sched_core_disabled())
		वापस false;

	वापस !!p->core_cookie;
}

#अन्यथा /* !CONFIG_SCHED_CORE: */

अटल अंतरभूत bool sched_core_enabled(काष्ठा rq *rq)
{
	वापस false;
}

अटल अंतरभूत bool sched_core_disabled(व्योम)
{
	वापस true;
}

अटल अंतरभूत raw_spinlock_t *rq_lockp(काष्ठा rq *rq)
{
	वापस &rq->__lock;
}

अटल अंतरभूत raw_spinlock_t *__rq_lockp(काष्ठा rq *rq)
	__returns_ctx_lock(rq_lockp(rq)) /* alias them */
{
	वापस &rq->__lock;
}

अटल अंतरभूत bool sched_cpu_cookie_match(काष्ठा rq *rq, काष्ठा task_struct *p)
{
	वापस true;
}

अटल अंतरभूत bool sched_core_cookie_match(काष्ठा rq *rq, काष्ठा task_struct *p)
{
	वापस true;
}

अटल अंतरभूत bool sched_group_cookie_match(काष्ठा rq *rq,
					    काष्ठा task_struct *p,
					    काष्ठा sched_group *group)
{
	वापस true;
}

अटल अंतरभूत bool task_has_sched_core(काष्ठा task_struct *p)
{
	वापस false;
}

#पूर्ण_अगर /* !CONFIG_SCHED_CORE */

#अगर_घोषित CONFIG_RT_GROUP_SCHED
# ifdef CONFIG_RT_GROUP_SCHED_DEFAULT_DISABLED
DECLARE_STATIC_कुंजी_FALSE(rt_group_sched);
अटल अंतरभूत bool rt_group_sched_enabled(व्योम)
{
	वापस static_branch_unlikely(&rt_group_sched);
}
# अन्यथा /* !CONFIG_RT_GROUP_SCHED_DEFAULT_DISABLED: */
DECLARE_STATIC_कुंजी_TRUE(rt_group_sched);
अटल अंतरभूत bool rt_group_sched_enabled(व्योम)
{
	वापस static_branch_likely(&rt_group_sched);
}
# endif /* !CONFIG_RT_GROUP_SCHED_DEFAULT_DISABLED */
#अन्यथा /* !CONFIG_RT_GROUP_SCHED: */
# define rt_group_sched_enabled()	false
#पूर्ण_अगर /* !CONFIG_RT_GROUP_SCHED */

अटल अंतरभूत व्योम lockdep_assert_rq_held(काष्ठा rq *rq)
	__assumes_ctx_lock(__rq_lockp(rq))
{
	lockdep_assert_held(__rq_lockp(rq));
}

बाह्य व्योम raw_spin_rq_lock_nested(काष्ठा rq *rq, पूर्णांक subclass)
	__acquires(__rq_lockp(rq));

बाह्य bool raw_spin_rq_trylock(काष्ठा rq *rq)
	__cond_acquires(true, __rq_lockp(rq));

अटल अंतरभूत व्योम raw_spin_rq_lock(काष्ठा rq *rq)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_lock_nested(rq, 0);
}

अटल अंतरभूत व्योम raw_spin_rq_unlock(काष्ठा rq *rq)
	__releases(__rq_lockp(rq))
{
	raw_spin_unlock(rq_lockp(rq));
}

अटल अंतरभूत व्योम raw_spin_rq_lock_irq(काष्ठा rq *rq)
	__acquires(__rq_lockp(rq))
{
	local_irq_disable();
	raw_spin_rq_lock(rq);
}

अटल अंतरभूत व्योम raw_spin_rq_unlock_irq(काष्ठा rq *rq)
	__releases(__rq_lockp(rq))
{
	raw_spin_rq_unlock(rq);
	local_irq_enable();
}

अटल अंतरभूत अचिन्हित दीर्घ _raw_spin_rq_lock_irqsave(काष्ठा rq *rq)
	__acquires(__rq_lockp(rq))
{
	अचिन्हित दीर्घ flags;

	local_irq_save(flags);
	raw_spin_rq_lock(rq);

	वापस flags;
}

अटल अंतरभूत व्योम raw_spin_rq_unlock_irqrestore(काष्ठा rq *rq, अचिन्हित दीर्घ flags)
	__releases(__rq_lockp(rq))
{
	raw_spin_rq_unlock(rq);
	local_irq_restore(flags);
}

#घोषणा raw_spin_rq_lock_irqsave(rq, flags)	\
करो {						\
	flags = _raw_spin_rq_lock_irqsave(rq);	\
} जबतक (0)

बाह्य व्योम __update_idle_core(काष्ठा rq *rq);

अटल अंतरभूत व्योम update_idle_core(काष्ठा rq *rq)
{
	अगर (sched_smt_active())
		__update_idle_core(rq);
}

#अगर_घोषित CONFIG_FAIR_GROUP_SCHED
अटल अंतरभूत काष्ठा task_struct *task_of(काष्ठा sched_entity *se)
{
	WARN_ON_ONCE(!entity_is_task(se));
	वापस container_of(se, काष्ठा task_struct, se);
}

अटल अंतरभूत काष्ठा cfs_rq *task_cfs_rq(काष्ठा task_struct *p)
{
	वापस p->se.cfs_rq;
}

/* runqueue on which this entity is (to be) queued */
अटल अंतरभूत काष्ठा cfs_rq *cfs_rq_of(स्थिर काष्ठा sched_entity *se)
{
	वापस se->cfs_rq;
}

/* runqueue "owned" by this group */
अटल अंतरभूत काष्ठा cfs_rq *group_cfs_rq(काष्ठा sched_entity *grp)
{
	वापस grp->my_q;
}

#अन्यथा /* !CONFIG_FAIR_GROUP_SCHED: */

#घोषणा task_of(_se)		container_of(_se, काष्ठा task_struct, se)

अटल अंतरभूत काष्ठा cfs_rq *task_cfs_rq(स्थिर काष्ठा task_struct *p)
{
	वापस &task_rq(p)->cfs;
}

अटल अंतरभूत काष्ठा cfs_rq *cfs_rq_of(स्थिर काष्ठा sched_entity *se)
{
	स्थिर काष्ठा task_struct *p = task_of(se);
	काष्ठा rq *rq = task_rq(p);

	वापस &rq->cfs;
}

/* runqueue "owned" by this group */
अटल अंतरभूत काष्ठा cfs_rq *group_cfs_rq(काष्ठा sched_entity *grp)
{
	वापस शून्य;
}

#पूर्ण_अगर /* !CONFIG_FAIR_GROUP_SCHED */

बाह्य व्योम update_rq_avg_idle(काष्ठा rq *rq);
बाह्य व्योम update_rq_clock(काष्ठा rq *rq);

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
#घोषणा RQCF_REQ_SKIP		0x01
#घोषणा RQCF_ACT_SKIP		0x02
#घोषणा RQCF_UPDATED		0x04

अटल अंतरभूत व्योम assert_clock_updated(काष्ठा rq *rq)
{
	/*
	 * The only reason for not seeing a clock update since the
	 * last rq_pin_lock() is if we're currently skipping updates.
	 */
	WARN_ON_ONCE(rq->clock_update_flags < RQCF_ACT_SKIP);
}

अटल अंतरभूत u64 rq_clock(काष्ठा rq *rq)
{
	lockdep_assert_rq_held(rq);
	assert_clock_updated(rq);

	वापस rq->घड़ी;
}

अटल अंतरभूत u64 rq_clock_task(काष्ठा rq *rq)
{
	lockdep_assert_rq_held(rq);
	assert_clock_updated(rq);

	वापस rq->clock_task;
}

अटल अंतरभूत व्योम rq_clock_skip_update(काष्ठा rq *rq)
{
	lockdep_assert_rq_held(rq);
	rq->clock_update_flags |= RQCF_REQ_SKIP;
}

/*
 * See rt task throttling, which is the only time a skip
 * request is canceled.
 */
अटल अंतरभूत व्योम rq_clock_cancel_skipupdate(काष्ठा rq *rq)
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
अटल अंतरभूत व्योम rq_clock_start_loop_update(काष्ठा rq *rq)
{
	lockdep_assert_rq_held(rq);
	WARN_ON_ONCE(rq->clock_update_flags & RQCF_ACT_SKIP);
	rq->clock_update_flags |= RQCF_ACT_SKIP;
}

अटल अंतरभूत व्योम rq_clock_stop_loop_update(काष्ठा rq *rq)
{
	lockdep_assert_rq_held(rq);
	rq->clock_update_flags &= ~RQCF_ACT_SKIP;
}

काष्ठा rq_flags {
	अचिन्हित दीर्घ flags;
	काष्ठा pin_cookie cookie;
	/*
	 * A copy of (rq::clock_update_flags & RQCF_UPDATED) for the
	 * current pin context is stashed here in case it needs to be
	 * restored in rq_repin_lock().
	 */
	अचिन्हित पूर्णांक clock_update_flags;
};

बाह्य काष्ठा balance_callback balance_धकेलो_callback;

#अगर_घोषित CONFIG_SCHED_CLASS_EXT
बाह्य स्थिर काष्ठा sched_class ext_sched_class;

DECLARE_STATIC_कुंजी_FALSE(__scx_enabled);	/* SCX BPF scheduler loaded */
DECLARE_STATIC_कुंजी_FALSE(__scx_switched_all);	/* all fair class tasks on SCX */

#घोषणा scx_enabled()		static_branch_unlikely(&__scx_enabled)
#घोषणा scx_switched_all()	static_branch_unlikely(&__scx_switched_all)

अटल अंतरभूत व्योम scx_rq_clock_update(काष्ठा rq *rq, u64 घड़ी)
{
	अगर (!scx_enabled())
		वापस;
	WRITE_ONCE(rq->scx.घड़ी, घड़ी);
	smp_store_release(&rq->scx.flags, rq->scx.flags | SCX_RQ_CLK_VALID);
}

अटल अंतरभूत व्योम scx_rq_clock_invalidate(काष्ठा rq *rq)
{
	अगर (!scx_enabled())
		वापस;
	WRITE_ONCE(rq->scx.flags, rq->scx.flags & ~SCX_RQ_CLK_VALID);
}

#अन्यथा /* !CONFIG_SCHED_CLASS_EXT: */
#घोषणा scx_enabled()		false
#घोषणा scx_switched_all()	false

अटल अंतरभूत व्योम scx_rq_clock_update(काष्ठा rq *rq, u64 घड़ी) {}
अटल अंतरभूत व्योम scx_rq_clock_invalidate(काष्ठा rq *rq) {}
#पूर्ण_अगर /* !CONFIG_SCHED_CLASS_EXT */

अटल अंतरभूत व्योम assert_balance_callbacks_empty(काष्ठा rq *rq)
{
	WARN_ON_ONCE(IS_ENABLED(CONFIG_PROVE_LOCKING) &&
		     rq->balance_callback &&
		     rq->balance_callback != &balance_धकेलो_callback);
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
अटल अंतरभूत व्योम rq_pin_lock(काष्ठा rq *rq, काष्ठा rq_flags *rf)
{
	rf->cookie = lockdep_pin_lock(__rq_lockp(rq));

	rq->clock_update_flags &= (RQCF_REQ_SKIP|RQCF_ACT_SKIP);
	rf->clock_update_flags = 0;
	assert_balance_callbacks_empty(rq);
}

अटल अंतरभूत व्योम rq_unpin_lock(काष्ठा rq *rq, काष्ठा rq_flags *rf)
{
	अगर (rq->clock_update_flags > RQCF_ACT_SKIP)
		rf->clock_update_flags = RQCF_UPDATED;

	scx_rq_clock_invalidate(rq);
	lockdep_unpin_lock(__rq_lockp(rq), rf->cookie);
}

अटल अंतरभूत व्योम rq_repin_lock(काष्ठा rq *rq, काष्ठा rq_flags *rf)
{
	lockdep_repin_lock(__rq_lockp(rq), rf->cookie);

	/*
	 * Restore the value we stashed in @rf for this pin context.
	 */
	rq->clock_update_flags |= rf->clock_update_flags;
}

#घोषणा __task_rq_lock(...) __acquire_ret(___task_rq_lock(__VA_ARGS__), __rq_lockp(__ret))
बाह्य काष्ठा rq *___task_rq_lock(काष्ठा task_struct *p, काष्ठा rq_flags *rf) __acquires_ret;

#घोषणा task_rq_lock(...) __acquire_ret(_task_rq_lock(__VA_ARGS__), __rq_lockp(__ret))
बाह्य काष्ठा rq *_task_rq_lock(काष्ठा task_struct *p, काष्ठा rq_flags *rf)
	__acquires(&p->pi_lock) __acquires_ret;

अटल अंतरभूत व्योम
__task_rq_unlock(काष्ठा rq *rq, काष्ठा task_struct *p, काष्ठा rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_lock(rq, rf);
	raw_spin_rq_unlock(rq);
}

अटल अंतरभूत व्योम
task_rq_unlock(काष्ठा rq *rq, काष्ठा task_struct *p, काष्ठा rq_flags *rf)
	__releases(__rq_lockp(rq), &p->pi_lock)
{
	__task_rq_unlock(rq, p, rf);
	raw_spin_unlock_irqrestore(&p->pi_lock, rf->flags);
}

DEFINE_LOCK_GUARD_1(task_rq_lock, काष्ठा task_struct,
		    _T->rq = task_rq_lock(_T->lock, &_T->rf),
		    task_rq_unlock(_T->rq, _T->lock, &_T->rf),
		    काष्ठा rq *rq; काष्ठा rq_flags rf)
DECLARE_LOCK_GUARD_1_ATTRS(task_rq_lock, __acquires(_T->pi_lock), __releases((*(काष्ठा task_struct **)_T)->pi_lock))
#घोषणा class_task_rq_lock_constructor(_T) WITH_LOCK_GUARD_1_ATTRS(task_rq_lock, _T)

DEFINE_LOCK_GUARD_1(__task_rq_lock, काष्ठा task_struct,
		    _T->rq = __task_rq_lock(_T->lock, &_T->rf),
		    __task_rq_unlock(_T->rq, _T->lock, &_T->rf),
		    काष्ठा rq *rq; काष्ठा rq_flags rf)

अटल अंतरभूत व्योम rq_lock_irqsave(काष्ठा rq *rq, काष्ठा rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_lock_irqsave(rq, rf->flags);
	rq_pin_lock(rq, rf);
}

अटल अंतरभूत व्योम rq_lock_irq(काष्ठा rq *rq, काष्ठा rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_lock_irq(rq);
	rq_pin_lock(rq, rf);
}

अटल अंतरभूत व्योम rq_lock(काष्ठा rq *rq, काष्ठा rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_lock(rq);
	rq_pin_lock(rq, rf);
}

अटल अंतरभूत व्योम rq_unlock_irqrestore(काष्ठा rq *rq, काष्ठा rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_lock(rq, rf);
	raw_spin_rq_unlock_irqrestore(rq, rf->flags);
}

अटल अंतरभूत व्योम rq_unlock_irq(काष्ठा rq *rq, काष्ठा rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_lock(rq, rf);
	raw_spin_rq_unlock_irq(rq);
}

अटल अंतरभूत व्योम rq_unlock(काष्ठा rq *rq, काष्ठा rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_lock(rq, rf);
	raw_spin_rq_unlock(rq);
}

DEFINE_LOCK_GUARD_1(rq_lock, काष्ठा rq,
		    rq_lock(_T->lock, &_T->rf),
		    rq_unlock(_T->lock, &_T->rf),
		    काष्ठा rq_flags rf)

DECLARE_LOCK_GUARD_1_ATTRS(rq_lock, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(काष्ठा rq **)_T)));
#घोषणा class_rq_lock_constructor(_T) WITH_LOCK_GUARD_1_ATTRS(rq_lock, _T)

DEFINE_LOCK_GUARD_1(rq_lock_irq, काष्ठा rq,
		    rq_lock_irq(_T->lock, &_T->rf),
		    rq_unlock_irq(_T->lock, &_T->rf),
		    काष्ठा rq_flags rf)

DECLARE_LOCK_GUARD_1_ATTRS(rq_lock_irq, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(काष्ठा rq **)_T)));
#घोषणा class_rq_lock_irq_constructor(_T) WITH_LOCK_GUARD_1_ATTRS(rq_lock_irq, _T)

DEFINE_LOCK_GUARD_1(rq_lock_irqsave, काष्ठा rq,
		    rq_lock_irqsave(_T->lock, &_T->rf),
		    rq_unlock_irqrestore(_T->lock, &_T->rf),
		    काष्ठा rq_flags rf)

DECLARE_LOCK_GUARD_1_ATTRS(rq_lock_irqsave, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(काष्ठा rq **)_T)));
#घोषणा class_rq_lock_irqsave_constructor(_T) WITH_LOCK_GUARD_1_ATTRS(rq_lock_irqsave, _T)

DEFINE_LOCK_GUARD_1(raw_spin_rq_lock_irqsave, काष्ठा rq,
		    raw_spin_rq_lock_irqsave(_T->lock, _T->flags),
		    raw_spin_rq_unlock_irqrestore(_T->lock, _T->flags),
		    अचिन्हित दीर्घ flags)

DECLARE_LOCK_GUARD_1_ATTRS(raw_spin_rq_lock_irqsave, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(काष्ठा rq **)_T)));
#घोषणा class_raw_spin_rq_lock_irqsave_constructor(_T) \
	WITH_LOCK_GUARD_1_ATTRS(raw_spin_rq_lock_irqsave, _T)

#घोषणा this_rq_lock_irq(...) __acquire_ret(_this_rq_lock_irq(__VA_ARGS__), __rq_lockp(__ret))
अटल अंतरभूत काष्ठा rq *_this_rq_lock_irq(काष्ठा rq_flags *rf) __acquires_ret
{
	काष्ठा rq *rq;

	local_irq_disable();
	rq = this_rq();
	rq_lock(rq, rf);

	वापस rq;
}

#अगर_घोषित CONFIG_NUMA

क्रमागत numa_topology_type {
	NUMA_DIRECT,
	NUMA_GLUELESS_MESH,
	NUMA_BACKPLANE,
};

बाह्य क्रमागत numa_topology_type sched_numa_topology_type;
बाह्य पूर्णांक sched_max_numa_distance;
बाह्य bool find_numa_distance(पूर्णांक distance);
बाह्य व्योम sched_init_numa(पूर्णांक offline_गाँठ);
बाह्य व्योम sched_update_numa(पूर्णांक cpu, bool online);
बाह्य व्योम sched_domains_numa_masks_set(अचिन्हित पूर्णांक cpu);
बाह्य व्योम sched_domains_numa_masks_clear(अचिन्हित पूर्णांक cpu);
बाह्य पूर्णांक sched_numa_find_closest(स्थिर काष्ठा cpumask *cpus, पूर्णांक cpu);

#अन्यथा /* !CONFIG_NUMA: */

अटल अंतरभूत व्योम sched_init_numa(पूर्णांक offline_गाँठ) { }
अटल अंतरभूत व्योम sched_update_numa(पूर्णांक cpu, bool online) { }
अटल अंतरभूत व्योम sched_domains_numa_masks_set(अचिन्हित पूर्णांक cpu) { }
अटल अंतरभूत व्योम sched_domains_numa_masks_clear(अचिन्हित पूर्णांक cpu) { }

अटल अंतरभूत पूर्णांक sched_numa_find_closest(स्थिर काष्ठा cpumask *cpus, पूर्णांक cpu)
{
	वापस nr_cpu_ids;
}

#पूर्ण_अगर /* !CONFIG_NUMA */

#अगर_घोषित CONFIG_NUMA_BALANCING

/* The regions in numa_faults array from task_struct */
क्रमागत numa_faults_stats {
	NUMA_MEM = 0,
	NUMA_CPU,
	NUMA_MEMBUF,
	NUMA_CPUBUF
};

बाह्य व्योम sched_setnuma(काष्ठा task_struct *p, पूर्णांक गाँठ);
बाह्य पूर्णांक migrate_task_to(काष्ठा task_struct *p, पूर्णांक cpu);
बाह्य पूर्णांक migrate_swap(काष्ठा task_struct *p, काष्ठा task_struct *t,
			पूर्णांक cpu, पूर्णांक scpu);
बाह्य व्योम init_numa_balancing(u64 clone_flags, काष्ठा task_struct *p);

#अन्यथा /* !CONFIG_NUMA_BALANCING: */

अटल अंतरभूत व्योम
init_numa_balancing(u64 clone_flags, काष्ठा task_struct *p)
{
}

#पूर्ण_अगर /* !CONFIG_NUMA_BALANCING */

पूर्णांक task_llc(स्थिर काष्ठा task_struct *p);

अटल अंतरभूत व्योम
queue_balance_callback(काष्ठा rq *rq,
		       काष्ठा balance_callback *शीर्ष,
		       व्योम (*func)(काष्ठा rq *rq))
{
	lockdep_assert_rq_held(rq);

	/*
	 * Don't (re)queue an already queued item; nor queue anything when
	 * balance_push() is active, see the comment with
	 * balance_push_callback.
	 */
	अगर (unlikely(शीर्ष->अग्र || rq->balance_callback == &balance_धकेलो_callback))
		वापस;

	शीर्ष->func = func;
	शीर्ष->अग्र = rq->balance_callback;
	rq->balance_callback = शीर्ष;
}

#घोषणा rcu_dereference_sched_domain(p) \
	rcu_dereference_all_check((p), lockdep_is_held(&sched_domains_mutex))

/*
 * The domain tree (rq->sd) is protected by RCU's quiescent state transition.
 * See destroy_sched_domains: call_rcu for details.
 *
 * The domain tree of any CPU may only be accessed from within
 * preempt-disabled sections.
 */
#घोषणा for_each_domain(cpu, __sd) \
	क्रम (__sd = rcu_dereference_sched_domain(cpu_rq(cpu)->sd); \
			__sd; __sd = __sd->parent)

/* A mask of all the SD flags that have the SDF_SHARED_CHILD metaflag */
#घोषणा SD_FLAG(name, mflags) (name * !!((mflags) & SDF_SHARED_CHILD)) |
अटल स्थिर अचिन्हित पूर्णांक SD_SHARED_CHILD_MASK =
#समावेश <linux/sched/sd_flags.h>
0;
#अघोषित SD_FLAG

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
अटल अंतरभूत काष्ठा sched_domain *highest_flag_domain(पूर्णांक cpu, पूर्णांक flag)
{
	काष्ठा sched_domain *sd, *hsd = शून्य;

	for_each_domain(cpu, sd) {
		अगर (sd->flags & flag) {
			hsd = sd;
			जारी;
		}

		/*
		 * Stop the search if @flag is known to be shared at lower
		 * levels. It will not be found further up.
		 */
		अगर (flag & SD_SHARED_CHILD_MASK)
			अवरोध;
	}

	वापस hsd;
}

अटल अंतरभूत काष्ठा sched_domain *lowest_flag_domain(पूर्णांक cpu, पूर्णांक flag)
{
	काष्ठा sched_domain *sd;

	for_each_domain(cpu, sd) {
		अगर (sd->flags & flag)
			अवरोध;
	}

	वापस sd;
}

DECLARE_PER_CPU(काष्ठा sched_domain __rcu *, sd_llc);
DECLARE_PER_CPU(पूर्णांक, sd_llc_आकार);
DECLARE_PER_CPU(पूर्णांक, sd_llc_id);
DECLARE_PER_CPU(पूर्णांक, sd_share_id);
DECLARE_PER_CPU(काष्ठा sched_domain_shared __rcu *, sd_llc_shared);
DECLARE_PER_CPU(काष्ठा sched_domain_shared __rcu *, sd_balance_shared);
DECLARE_PER_CPU(काष्ठा sched_domain __rcu *, sd_numa);
DECLARE_PER_CPU(काष्ठा sched_domain __rcu *, sd_asym_packing);
DECLARE_PER_CPU(काष्ठा sched_domain __rcu *, sd_asym_cpucapacity);

बाह्य काष्ठा static_कुंजी_false sched_asym_cpucapacity;
बाह्य काष्ठा static_कुंजी_false sched_cluster_active;

अटल __always_inline bool sched_asym_cpucap_active(व्योम)
{
	वापस static_branch_unlikely(&sched_asym_cpucapacity);
}

काष्ठा sched_group_capacity {
	atomic_t		ref;
	/*
	 * CPU capacity of this group, SCHED_CAPACITY_SCALE being max capacity
	 * for a single CPU.
	 */
	अचिन्हित दीर्घ		capacity;
	अचिन्हित दीर्घ		min_capacity;		/* Min per-CPU capacity in group */
	अचिन्हित दीर्घ		max_capacity;		/* Max per-CPU capacity in group */
	अचिन्हित दीर्घ		अग्र_update;
	पूर्णांक			imbalance;		/* XXX unrelated to capacity but shared group state */

	पूर्णांक			id;

	अचिन्हित दीर्घ		cpumask[];		/* Balance mask */
};

काष्ठा sched_group {
	काष्ठा sched_group	*अग्र;			/* Must be a circular list */
	atomic_t		ref;

	अचिन्हित पूर्णांक		group_weight;
	अचिन्हित पूर्णांक		cores;
	काष्ठा sched_group_capacity *sgc;
	पूर्णांक			asym_prefer_cpu;	/* CPU of highest priority in group */
	पूर्णांक			flags;

	/*
	 * The CPUs this group covers.
	 *
	 * NOTE: this field is variable length. (Allocated dynamically
	 * by attaching extra space to the end of the structure,
	 * depending on how many CPUs the kernel has booted up with)
	 */
	अचिन्हित दीर्घ		cpumask[];
};

अटल अंतरभूत काष्ठा cpumask *sched_group_span(काष्ठा sched_group *sg)
{
	वापस to_cpumask(sg->cpumask);
}

/*
 * See build_balance_mask().
 */
अटल अंतरभूत काष्ठा cpumask *group_balance_mask(काष्ठा sched_group *sg)
{
	वापस to_cpumask(sg->sgc->cpumask);
}

बाह्य पूर्णांक group_balance_cpu(काष्ठा sched_group *sg);

बाह्य व्योम update_sched_domain_debugfs(व्योम);
बाह्य व्योम dirty_sched_domain_sysctl(पूर्णांक cpu);

बाह्य पूर्णांक sched_update_scaling(व्योम);

अटल अंतरभूत स्थिर काष्ठा cpumask *task_user_cpus(काष्ठा task_struct *p)
{
	अगर (!p->user_cpus_ptr)
		वापस cpu_possible_mask; /* &init_task.cpus_mask */
	वापस p->user_cpus_ptr;
}

#अगर_घोषित CONFIG_CGROUP_SCHED

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
अटल अंतरभूत काष्ठा task_group *task_group(काष्ठा task_struct *p)
{
	वापस p->sched_task_group;
}

#अगर_घोषित CONFIG_FAIR_GROUP_SCHED
/*
 * Defined here to be available before stats.h is included, since
 * stats.h has dependencies on things defined later in this file.
 */
काष्ठा cfs_tg_state {
	काष्ठा cfs_rq		cfs_rq;
	काष्ठा sched_entity	se;
	काष्ठा sched_statistics	stats;
} __no_randomize_layout;

/* Access a specific CPU's cfs_rq from a task group */
अटल अंतरभूत काष्ठा cfs_rq *tg_cfs_rq(काष्ठा task_group *tg, पूर्णांक cpu)
{
	वापस per_cpu_ptr(tg->cfs_rq, cpu);
}

अटल अंतरभूत काष्ठा sched_entity *tg_se(काष्ठा task_group *tg, पूर्णांक cpu)
{
	काष्ठा cfs_tg_state *state;

	अगर (is_मूल_task_group(tg))
		वापस शून्य;

	state = container_of(tg_cfs_rq(tg, cpu), काष्ठा cfs_tg_state, cfs_rq);
	वापस &state->se;
}

अटल अंतरभूत काष्ठा sched_entity *cfs_rq_se(काष्ठा cfs_rq *cfs_rq)
{
	काष्ठा cfs_tg_state *state;

	अगर (is_मूल_task_group(cfs_rq->tg))
		वापस शून्य;

	state = container_of(cfs_rq, काष्ठा cfs_tg_state, cfs_rq);
	वापस &state->se;
}
#पूर्ण_अगर

/* Change a task's cfs_rq and parent entity if it moves across CPUs/groups */
अटल अंतरभूत व्योम set_task_rq(काष्ठा task_struct *p, अचिन्हित पूर्णांक cpu)
{
#अगर defined(CONFIG_FAIR_GROUP_SCHED) || defined(CONFIG_RT_GROUP_SCHED)
	काष्ठा task_group *tg = task_group(p);
#पूर्ण_अगर

#अगर_घोषित CONFIG_FAIR_GROUP_SCHED
	set_task_rq_fair(&p->se, p->se.cfs_rq, tg_cfs_rq(tg, cpu));
	p->se.cfs_rq = tg_cfs_rq(tg, cpu);
	p->se.parent = tg_se(tg, cpu);
	p->se.depth = p->se.parent ? p->se.parent->depth + 1 : 0;
#पूर्ण_अगर

#अगर_घोषित CONFIG_RT_GROUP_SCHED
	/*
	 * p->rt.rt_rq is NULL initially and it is easier to assign
	 * root_task_group's rt_rq than switching in rt_rq_of_se()
	 * Clobbers tg(!)
	 */
	अगर (!rt_group_sched_enabled())
		tg = &मूल_task_group;
	p->rt.rt_rq  = tg->rt_rq[cpu];
	p->rt.parent = tg->rt_se[cpu];
#पूर्ण_अगर /* CONFIG_RT_GROUP_SCHED */
}

#अन्यथा /* !CONFIG_CGROUP_SCHED: */

अटल अंतरभूत व्योम set_task_rq(काष्ठा task_struct *p, अचिन्हित पूर्णांक cpu) { }

अटल अंतरभूत काष्ठा task_group *task_group(काष्ठा task_struct *p)
{
	वापस शून्य;
}

#पूर्ण_अगर /* !CONFIG_CGROUP_SCHED */

अटल अंतरभूत व्योम __set_task_cpu(काष्ठा task_struct *p, अचिन्हित पूर्णांक cpu)
{
	set_task_rq(p, cpu);
#अगर_घोषित CONFIG_SMP
	/*
	 * After ->cpu is set up to a new value, task_rq_lock(p, ...) can be
	 * successfully executed on another CPU. We must ensure that updates of
	 * per-task data have been completed by this moment.
	 */
	smp_wmb();
	WRITE_ONCE(task_thread_info(p)->cpu, cpu);
	p->wake_cpu = cpu;
	rseq_sched_set_ids_changed(p);
#पूर्ण_अगर /* CONFIG_SMP */
}

/*
 * Tunables:
 */

#घोषणा SCHED_FEAT(name, enabled)	\
	__SCHED_FEAT_##name ,

क्रमागत {
#समावेश "features.h"
	__SCHED_FEAT_NR,
};

#अघोषित SCHED_FEAT

/*
 * To support run-time toggling of sched features, all the translation units
 * (but core.c) reference the sysctl_sched_features defined in core.c.
 */
बाह्य __read_mostly अचिन्हित पूर्णांक sysctl_sched_features;

#अगर_घोषित CONFIG_JUMP_LABEL

जोड़ sched_feat_कुंजी {
	काष्ठा static_कुंजी_true	कुंजी_true;
	काष्ठा static_कुंजी_false	कुंजी_false;
};

#घोषणा sched_feat_branch_true(कुंजी)	static_branch_likely(&(कुंजी)->कुंजी_true)
#घोषणा sched_feat_branch_false(कुंजी)	static_branch_unlikely(&(कुंजी)->कुंजी_false)

#घोषणा SCHED_FEAT(name, enabled)			\
अटल __always_inline bool				\
static_branch_##name(जोड़ sched_feat_कुंजी *कुंजी) 	\
{							\
	वापस sched_feat_branch_##enabled(कुंजी);	\
}

#समावेश "features.h"
#अघोषित SCHED_FEAT

बाह्य जोड़ sched_feat_कुंजी sched_feat_कुंजियाँ[__SCHED_FEAT_NR];
#घोषणा sched_feat(x) (static_branch_##x(&sched_feat_कुंजियाँ[__SCHED_FEAT_##x]))

#अन्यथा /* !CONFIG_JUMP_LABEL: */

#घोषणा sched_feat(x) (sysctl_sched_features & (1UL << __SCHED_FEAT_##x))

#पूर्ण_अगर /* !CONFIG_JUMP_LABEL */

बाह्य काष्ठा static_कुंजी_false sched_numa_balancing;
बाह्य काष्ठा static_कुंजी_false sched_schedstats;

अटल अंतरभूत u64 global_rt_period(व्योम)
{
	वापस (u64)sysctl_sched_rt_period * NSEC_PER_USEC;
}

अटल अंतरभूत u64 global_rt_runtime(व्योम)
{
	अगर (sysctl_sched_rt_runtime < 0)
		वापस RUNTIME_INF;

	वापस (u64)sysctl_sched_rt_runtime * NSEC_PER_USEC;
}

/*
 * Is p the current execution context?
 */
अटल अंतरभूत पूर्णांक task_current(काष्ठा rq *rq, काष्ठा task_struct *p)
{
	वापस rq->curr == p;
}

/*
 * Is p the current scheduling context?
 *
 * Note that it might be the current execution context at the same time if
 * rq->curr == rq->donor == p.
 */
अटल अंतरभूत पूर्णांक task_current_donor(काष्ठा rq *rq, काष्ठा task_struct *p)
{
	वापस rq->donor == p;
}

अटल अंतरभूत bool task_is_blocked(काष्ठा task_struct *p)
{
	अगर (!sched_proxy_exec())
		वापस false;

	वापस !!p->blocked_on;
}

#अगर_घोषित CONFIG_SCHED_PROXY_EXEC
व्योम sched_proxy_block_task(काष्ठा rq *rq, काष्ठा task_struct *p);
#अन्यथा
अटल अंतरभूत व्योम sched_proxy_block_task(काष्ठा rq *rq, काष्ठा task_struct *p) {}
#पूर्ण_अगर

अटल अंतरभूत पूर्णांक task_on_cpu(काष्ठा rq *rq, काष्ठा task_struct *p)
{
	वापस p->on_cpu;
}

अटल अंतरभूत पूर्णांक task_on_rq_queued(काष्ठा task_struct *p)
{
	वापस READ_ONCE(p->on_rq) == TASK_ON_RQ_QUEUED;
}

अटल अंतरभूत पूर्णांक task_on_rq_migrating(काष्ठा task_struct *p)
{
	वापस READ_ONCE(p->on_rq) == TASK_ON_RQ_MIGRATING;
}

/* Wake flags. The first three directly map to some SD flag value */
#घोषणा WF_EXEC			0x02 /* Wakeup after exec; maps to SD_BALANCE_EXEC */
#घोषणा WF_FORK			0x04 /* Wakeup after fork; maps to SD_BALANCE_FORK */
#घोषणा WF_TTWU			0x08 /* Wakeup;            maps to SD_BALANCE_WAKE */
/*
 * Hint that the caller expects the waker to sleep soon.
 * Scheduler classes may use it for placement or preemption.
 * Callers must not rely on it to prevent migration,
 * preserve CPU locality or make the wakee run next.
 */
#घोषणा WF_SYNC			0x10
#घोषणा WF_MIGRATED		0x20 /* Internal use, task got migrated */
#घोषणा WF_CURRENT_CPU		0x40 /* Prefer to move the wakee to the current CPU. */
#घोषणा WF_RQ_SELECTED		0x80 /* ->select_task_rq() was called */
#घोषणा WF_TTWU_RQ		0x100 /* Wakeup completed through ttwu_runnable() */

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

#घोषणा WEIGHT_IDLEPRIO		3
#घोषणा WMULT_IDLEPRIO		1431655765

बाह्य स्थिर पूर्णांक		sched_prio_to_weight[40];
बाह्य स्थिर u32		sched_prio_to_wmult[40];

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

#घोषणा DEQUEUE_SLEEP		0x0001 /* Matches ENQUEUE_WAKEUP */
#घोषणा DEQUEUE_SAVE		0x0002 /* Matches ENQUEUE_RESTORE */
#घोषणा DEQUEUE_MOVE		0x0004 /* Matches ENQUEUE_MOVE */
#घोषणा DEQUEUE_NOCLOCK		0x0008 /* Matches ENQUEUE_NOCLOCK */

#घोषणा DEQUEUE_MIGRATING	0x0010 /* Matches ENQUEUE_MIGRATING */
#घोषणा DEQUEUE_DELAYED		0x0020 /* Matches ENQUEUE_DELAYED */
#घोषणा DEQUEUE_CLASS		0x0040 /* Matches ENQUEUE_CLASS */

#घोषणा DEQUEUE_SPECIAL		0x00010000
#घोषणा DEQUEUE_THROTTLE	0x00020000

#घोषणा ENQUEUE_WAKEUP		0x0001
#घोषणा ENQUEUE_RESTORE		0x0002
#घोषणा ENQUEUE_MOVE		0x0004
#घोषणा ENQUEUE_NOCLOCK		0x0008

#घोषणा ENQUEUE_MIGRATING	0x0010
#घोषणा ENQUEUE_DELAYED		0x0020
#घोषणा ENQUEUE_CLASS		0x0040

#घोषणा ENQUEUE_शीर्ष		0x00010000
#घोषणा ENQUEUE_REPLENISH	0x00020000
#घोषणा ENQUEUE_MIGRATED	0x00040000
#घोषणा ENQUEUE_INITIAL		0x00080000
#घोषणा ENQUEUE_RQ_SELECTED	0x00100000
#घोषणा ENQUEUE_QUEUED		0x00200000

#घोषणा RETRY_TASK		((व्योम *)-1UL)

काष्ठा affinity_context {
	स्थिर काष्ठा cpumask	*new_mask;
	काष्ठा cpumask		*user_mask;
	अचिन्हित पूर्णांक		flags;
};

बाह्य s64 update_curr_common(काष्ठा rq *rq);

क्रमागत snt_e {
	SNT_NORMAL,	/* set_next_task() */
	SNT_PICK,	/* put_prev_set_next_task(): prev != next */
	SNT_REPICK,	/* put_prev_set_next_task(): prev == next */
};

काष्ठा sched_class {

#अगर_घोषित CONFIG_UCLAMP_TASK
	पूर्णांक uclamp_enabled;
#पूर्ण_अगर

	/*
	 * move_queued_task/activate_task/enqueue_task: rq->lock
	 * ttwu_do_activate/activate_task/enqueue_task: rq->lock
	 * wake_up_new_task/activate_task/enqueue_task: task_rq_lock
	 * ttwu_runnable/enqueue_task: task_rq_lock
	 * proxy_task_current: rq->lock
	 * sched_change_end
	 */
	व्योम (*enqueue_task) (काष्ठा rq *rq, काष्ठा task_struct *p, पूर्णांक flags);
	/*
	 * move_queued_task/deactivate_task/dequeue_task: rq->lock
	 * __schedule/block_task/dequeue_task: rq->lock
	 * proxy_task_current: rq->lock
	 * wait_task_inactive: task_rq_lock
	 * sched_change_begin
	 */
	bool (*dequeue_task) (काष्ठा rq *rq, काष्ठा task_struct *p, पूर्णांक flags);

	/*
	 * do_sched_yield: rq->lock
	 */
	व्योम (*yield_task)   (काष्ठा rq *rq);
	/*
	 * yield_to: rq->lock (double)
	 */
	bool (*yield_to_task)(काष्ठा rq *rq, काष्ठा task_struct *p);

	/*
	 * move_queued_task: rq->lock
	 * __migrate_swap_task: rq->lock
	 * ttwu_do_activate: rq->lock
	 * ttwu_runnable: task_rq_lock
	 * wake_up_new_task: task_rq_lock
	 */
	व्योम (*wakeup_preempt)(काष्ठा rq *rq, काष्ठा task_struct *p, पूर्णांक flags);

	/*
	 * schedule/pick_next_task/prev_balance: rq->lock
	 */
	पूर्णांक (*balance)(काष्ठा rq *rq, काष्ठा rq_flags *rf);

	/*
	 * schedule/pick_next_task: rq->lock
	 */
	काष्ठा task_struct *(*pick_task)(काष्ठा rq *rq, काष्ठा rq_flags *rf);

	/*
	 * sched_change:
	 * __schedule: rq->lock
	 */
	व्योम (*put_पूर्व_task)(काष्ठा rq *rq, काष्ठा task_struct *p, काष्ठा task_struct *अग्र);
	व्योम (*set_अग्र_task)(काष्ठा rq *rq, काष्ठा task_struct *p, क्रमागत snt_e type);

	/*
	 * select_task_rq: p->pi_lock
	 * sched_exec: p->pi_lock
	 */
	पूर्णांक  (*select_task_rq)(काष्ठा task_struct *p, पूर्णांक task_cpu, पूर्णांक flags);

	/*
	 * set_task_cpu: p->pi_lock || rq->lock (ttwu like)
	 */
	व्योम (*migrate_task_rq)(काष्ठा task_struct *p, पूर्णांक new_cpu);

	/*
	 * ttwu_do_activate: rq->lock
	 * wake_up_new_task: task_rq_lock
	 */
	व्योम (*task_woken)(काष्ठा rq *this_rq, काष्ठा task_struct *task);

	/*
	 * do_set_cpus_allowed: task_rq_lock + sched_change
	 */
	व्योम (*set_cpus_allowed)(काष्ठा task_struct *p, काष्ठा affinity_context *ctx);

	/*
	 * sched_set_rq_{on,off}line: rq->lock
	 */
	व्योम (*rq_online)(काष्ठा rq *rq);
	व्योम (*rq_offline)(काष्ठा rq *rq);

	/*
	 * push_cpu_stop: p->pi_lock && rq->lock
	 */
	काष्ठा rq *(*find_lock_rq)(काष्ठा task_struct *p, काष्ठा rq *rq);

	/*
	 * hrtick: rq->lock
	 * sched_tick: rq->lock
	 * sched_tick_remote: rq->lock
	 */
	व्योम (*task_tick)(काष्ठा rq *rq, काष्ठा task_struct *p, पूर्णांक queued);
	/*
	 * sched_cgroup_fork: p->pi_lock
	 */
	व्योम (*task_fork)(काष्ठा task_struct *p);
	/*
	 * finish_task_switch: no locks
	 */
	व्योम (*task_dead)(काष्ठा task_struct *p);

	/*
	 * sched_change
	 */
	व्योम (*switching_from)(काष्ठा rq *this_rq, काष्ठा task_struct *task);
	व्योम (*switched_from) (काष्ठा rq *this_rq, काष्ठा task_struct *task);
	व्योम (*switching_to)  (काष्ठा rq *this_rq, काष्ठा task_struct *task);
	व्योम (*switched_to)   (काष्ठा rq *this_rq, काष्ठा task_struct *task);
	u64  (*get_prio)     (काष्ठा rq *this_rq, काष्ठा task_struct *task);
	व्योम (*prio_changed) (काष्ठा rq *this_rq, काष्ठा task_struct *task,
			      u64 oldprio);

	/*
	 * set_load_weight: task_rq_lock + sched_change
	 * __setscheduler_parms: task_rq_lock + sched_change
	 */
	व्योम (*reweight_task)(काष्ठा rq *this_rq, काष्ठा task_struct *task,
			      स्थिर काष्ठा load_weight *lw);

	/*
	 * sched_rr_get_interval: task_rq_lock
	 */
	अचिन्हित पूर्णांक (*get_rr_interval)(काष्ठा rq *rq,
					काष्ठा task_struct *task);

	/*
	 * task_sched_runtime: task_rq_lock
	 */
	व्योम (*update_curr)(काष्ठा rq *rq);

#अगर_घोषित CONFIG_FAIR_GROUP_SCHED
	/*
	 * sched_change_group: task_rq_lock + sched_change
	 */
	व्योम (*task_change_group)(काष्ठा task_struct *p);
#पूर्ण_अगर

#अगर_घोषित CONFIG_SCHED_CORE
	/*
	 * pick_next_task: rq->lock
	 * try_steal_cookie: rq->lock (double)
	 */
	पूर्णांक (*task_is_throttled)(काष्ठा task_struct *p, पूर्णांक cpu);
#पूर्ण_अगर
};

अटल अंतरभूत व्योम put_पूर्व_task(काष्ठा rq *rq, काष्ठा task_struct *पूर्व)
{
	WARN_ON_ONCE(rq->donor != पूर्व);
	पूर्व->sched_class->put_पूर्व_task(rq, पूर्व, शून्य);
}

अटल अंतरभूत व्योम set_अग्र_task(काष्ठा rq *rq, काष्ठा task_struct *अग्र)
{
	अग्र->sched_class->set_अग्र_task(rq, अग्र, SNT_NORMAL);
}

अटल अंतरभूत व्योम
__put_prev_set_next_dl_server(काष्ठा rq *rq,
			      काष्ठा task_struct *पूर्व,
			      काष्ठा task_struct *अग्र)
{
	पूर्व->dl_server = शून्य;
	अग्र->dl_server = rq->dl_server;
	rq->dl_server = शून्य;
}

अटल अंतरभूत व्योम put_पूर्व_set_अग्र_task(काष्ठा rq *rq,
					  काष्ठा task_struct *पूर्व,
					  काष्ठा task_struct *अग्र)
{
	WARN_ON_ONCE(rq->donor != पूर्व);

	__put_prev_set_next_dl_server(rq, पूर्व, अग्र);

	अगर (अग्र == पूर्व) {
		अग्र->sched_class->set_अग्र_task(rq, अग्र, SNT_REPICK);
		वापस;
	}

	पूर्व->sched_class->put_पूर्व_task(rq, पूर्व, अग्र);
	अग्र->sched_class->set_अग्र_task(rq, अग्र, SNT_PICK);
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
#घोषणा DEFINE_SCHED_CLASS(name) \
स्थिर काष्ठा sched_class name##_sched_class \
	__aligned(__alignof__(काष्ठा sched_class)) \
	__section("__" #name "_sched_class")

/* Defined in include/asm-generic/vmlinux.lds.h */
बाह्य काष्ठा sched_class __sched_class_highest[];
बाह्य काष्ठा sched_class __sched_class_lowest[];

बाह्य स्थिर काष्ठा sched_class stop_sched_class;
बाह्य स्थिर काष्ठा sched_class dl_sched_class;
बाह्य स्थिर काष्ठा sched_class rt_sched_class;
बाह्य स्थिर काष्ठा sched_class fair_sched_class;
बाह्य स्थिर काष्ठा sched_class idle_sched_class;

/*
 * Iterate only active classes. SCX can take over all fair tasks or be
 * completely disabled. If the former, skip fair. If the latter, skip SCX.
 */
अटल अंतरभूत स्थिर काष्ठा sched_class *अग्र_active_class(स्थिर काष्ठा sched_class *class)
{
	class++;
#अगर_घोषित CONFIG_SCHED_CLASS_EXT
	अगर (scx_switched_all() && class == &fair_sched_class)
		class++;
	अगर (!scx_enabled() && class == &ext_sched_class)
		class++;
#पूर्ण_अगर
	वापस class;
}

#घोषणा for_class_range(class, _from, _to) \
	क्रम (class = (_from); class < (_to); class++)

#घोषणा for_each_class(class) \
	for_class_range(class, __sched_class_highest, __sched_class_lowest)

#घोषणा for_active_class_range(class, _from, _to)				\
	क्रम (class = (_from); class != (_to); class = अग्र_active_class(class))

#घोषणा for_each_active_class(class)						\
	for_active_class_range(class, __sched_class_highest, __sched_class_lowest)

#घोषणा sched_class_above(_a, _b)	((_a) < (_b))

अटल अंतरभूत व्योम rq_modified_begin(काष्ठा rq *rq, स्थिर काष्ठा sched_class *class)
{
	अगर (sched_class_above(rq->अग्र_class, class))
		rq->अग्र_class = class;
}

अटल अंतरभूत bool rq_modified_above(काष्ठा rq *rq, स्थिर काष्ठा sched_class *class)
{
	वापस sched_class_above(rq->अग्र_class, class);
}

अटल अंतरभूत bool sched_stop_runnable(काष्ठा rq *rq)
{
	वापस rq->stop && task_on_rq_queued(rq->stop);
}

अटल अंतरभूत bool sched_dl_runnable(काष्ठा rq *rq)
{
	वापस rq->dl.dl_nr_running > 0;
}

अटल अंतरभूत bool sched_rt_runnable(काष्ठा rq *rq)
{
	वापस rq->rt.rt_queued > 0;
}

अटल अंतरभूत bool sched_fair_runnable(काष्ठा rq *rq)
{
	वापस rq->cfs.nr_queued > 0;
}

बाह्य काष्ठा task_struct *pick_task_fair(काष्ठा rq *rq, काष्ठा rq_flags *rf);
बाह्य काष्ठा task_struct *pick_task_idle(काष्ठा rq *rq, काष्ठा rq_flags *rf);

#घोषणा SCA_CHECK		0x01
#घोषणा SCA_MIGRATE_DISABLE	0x02
#घोषणा SCA_MIGRATE_ENABLE	0x04
#घोषणा SCA_USER		0x08

बाह्य व्योम update_group_capacity(काष्ठा sched_domain *sd, पूर्णांक cpu);

बाह्य व्योम sched_balance_trigger(काष्ठा rq *rq);

बाह्य पूर्णांक __set_cpus_allowed_ptr(काष्ठा task_struct *p, काष्ठा affinity_context *ctx);
बाह्य व्योम set_cpus_allowed_common(काष्ठा task_struct *p, काष्ठा affinity_context *ctx);

अटल अंतरभूत bool task_allowed_on_cpu(काष्ठा task_struct *p, पूर्णांक cpu)
{
	/* When not in the task's cpumask, no point in looking further. */
	अगर (!cpumask_test_cpu(cpu, p->cpus_ptr))
		वापस false;

	/* Can @cpu run a user thread? */
	अगर (!(p->flags & PF_KTHREAD) && !task_cpu_possible(cpu, p))
		वापस false;

	वापस true;
}

अटल अंतरभूत cpumask_t *alloc_user_cpus_ptr(पूर्णांक गाँठ)
{
	/*
	 * See set_cpus_allowed_force() above for the rcu_head usage.
	 */
	पूर्णांक आकार = max_t(पूर्णांक, cpumask_आकार(), माप(काष्ठा rcu_शीर्ष));

	वापस kmalloc_गाँठ(आकार, GFP_KERNEL, गाँठ);
}

अटल अंतरभूत काष्ठा task_struct *get_धकेलो_task(काष्ठा rq *rq)
{
	काष्ठा task_struct *p = rq->donor;

	lockdep_assert_rq_held(rq);

	अगर (rq->धकेलो_busy)
		वापस शून्य;

	अगर (p->nr_cpus_allowed == 1)
		वापस शून्य;

	अगर (p->migration_disabled)
		वापस शून्य;

	rq->धकेलो_busy = true;
	वापस get_task_struct(p);
}

बाह्य पूर्णांक धकेलो_cpu_stop(व्योम *arg);

#अगर_घोषित CONFIG_CPU_IDLE

अटल अंतरभूत व्योम idle_set_state(काष्ठा rq *rq,
				  काष्ठा cpuidle_state *idle_state)
{
	rq->idle_state = idle_state;
}

अटल अंतरभूत काष्ठा cpuidle_state *idle_get_state(काष्ठा rq *rq)
{
	lockdep_assert(rcu_read_lock_any_held());

	वापस rq->idle_state;
}

#अन्यथा /* !CONFIG_CPU_IDLE: */

अटल अंतरभूत व्योम idle_set_state(काष्ठा rq *rq,
				  काष्ठा cpuidle_state *idle_state)
{
}

अटल अंतरभूत काष्ठा cpuidle_state *idle_get_state(काष्ठा rq *rq)
{
	वापस शून्य;
}

#पूर्ण_अगर /* !CONFIG_CPU_IDLE */

बाह्य व्योम schedule_idle(व्योम);
asmlinkage व्योम schedule_user(व्योम);

बाह्य व्योम sysrq_sched_debug_show(व्योम);
बाह्य व्योम sched_init_granularity(व्योम);
बाह्य व्योम update_max_interval(व्योम);

बाह्य व्योम init_sched_dl_class(व्योम);
बाह्य व्योम init_sched_rt_class(व्योम);
बाह्य व्योम init_sched_fair_class(व्योम);

बाह्य व्योम resched_curr(काष्ठा rq *rq);
बाह्य व्योम resched_curr_lazy(काष्ठा rq *rq);
बाह्य व्योम resched_cpu(पूर्णांक cpu);

बाह्य व्योम init_rt_bandwidth(काष्ठा rt_bandwidth *rt_b, u64 period, u64 runtime);
बाह्य bool sched_rt_bandwidth_account(काष्ठा rt_rq *rt_rq);

बाह्य व्योम init_dl_entity(काष्ठा sched_dl_entity *dl_se);

बाह्य व्योम init_cfs_throttle_work(काष्ठा task_struct *p);

#घोषणा BW_SHIFT		20
#घोषणा BW_UNIT			(1 << BW_SHIFT)
#घोषणा RATIO_SHIFT		8
#घोषणा MAX_BW_BITS		(64 - BW_SHIFT)
#घोषणा MAX_BW			((1ULL << MAX_BW_BITS) - 1)

बाह्य u64 to_ratio(u64 period, u64 runtime);

बाह्य व्योम init_entity_runnable_average(काष्ठा sched_entity *se);
बाह्य व्योम post_init_entity_util_avg(काष्ठा task_struct *p);

#अगर_घोषित CONFIG_NO_HZ_FULL
बाह्य bool sched_can_stop_tick(काष्ठा rq *rq);
बाह्य पूर्णांक __init sched_tick_offload_init(व्योम);

/*
 * Tick may be needed by tasks in the runqueue depending on their policy and
 * requirements. If tick is needed, lets send the target an IPI to kick it out of
 * nohz mode if necessary.
 */
अटल अंतरभूत व्योम sched_update_tick_dependency(काष्ठा rq *rq)
{
	पूर्णांक cpu = cpu_of(rq);

	अगर (!tick_nohz_full_cpu(cpu))
		वापस;

	अगर (sched_can_stop_tick(rq))
		tick_nohz_dep_clear_cpu(cpu, TICK_DEP_BIT_SCHED);
	अन्यथा
		tick_nohz_dep_set_cpu(cpu, TICK_DEP_BIT_SCHED);
}
#अन्यथा /* !CONFIG_NO_HZ_FULL: */
अटल अंतरभूत पूर्णांक sched_tick_offload_init(व्योम) { वापस 0; }
अटल अंतरभूत व्योम sched_update_tick_dependency(काष्ठा rq *rq) { }
#पूर्ण_अगर /* !CONFIG_NO_HZ_FULL */

अटल अंतरभूत व्योम add_nr_running(काष्ठा rq *rq, अचिन्हित गिनती)
{
	अचिन्हित पूर्व_nr = rq->nr_running;

	rq->nr_running = पूर्व_nr + गिनती;
	अगर (trace_sched_update_nr_running_tp_enabled()) {
		call_trace_sched_update_nr_running(rq, गिनती);
	}

	अगर (पूर्व_nr < 2 && rq->nr_running >= 2)
		set_rd_overloaded(rq->rd, 1);

	sched_update_tick_dependency(rq);
}

अटल अंतरभूत व्योम sub_nr_running(काष्ठा rq *rq, अचिन्हित गिनती)
{
	rq->nr_running -= गिनती;
	अगर (trace_sched_update_nr_running_tp_enabled()) {
		call_trace_sched_update_nr_running(rq, -गिनती);
	}

	/* Check if we still need preemption */
	sched_update_tick_dependency(rq);
}

अटल अंतरभूत व्योम __block_task(काष्ठा rq *rq, काष्ठा task_struct *p)
{
	अगर (p->sched_contributes_to_load)
		rq->nr_uninterruptible++;

	अगर (p->in_iowait) {
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

बाह्य व्योम activate_task(काष्ठा rq *rq, काष्ठा task_struct *p, पूर्णांक flags);
बाह्य व्योम deactivate_task(काष्ठा rq *rq, काष्ठा task_struct *p, पूर्णांक flags);

बाह्य व्योम wakeup_preempt(काष्ठा rq *rq, काष्ठा task_struct *p, पूर्णांक flags);

/*
 * attach_task() -- attach the task detached by detach_task() to its new rq.
 */
अटल अंतरभूत व्योम attach_task(काष्ठा rq *rq, काष्ठा task_struct *p)
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
अटल अंतरभूत व्योम attach_one_task(काष्ठा rq *rq, काष्ठा task_struct *p)
{
	guard(rq_lock)(rq);
	update_rq_clock(rq);
	attach_task(rq, p);
}

/*
 * __attach_tasks() - attaches a list of tasks (using se.group_node) to
 * the new rq
 */
अटल अंतरभूत व्योम __attach_tasks(काष्ठा rq *rq, काष्ठा list_शीर्ष *tasks)
{
	guard(rq_lock)(rq);
	update_rq_clock(rq);

	जबतक (!list_empty(tasks)) {
		काष्ठा task_struct *p;

		p = list_first_entry(tasks, काष्ठा task_struct, se.group_गाँठ);
		list_del_init(&p->se.group_गाँठ);

		attach_task(rq, p);
	}
}

#अगर_घोषित CONFIG_PREEMPT_RT
# define SCHED_NR_MIGRATE_BREAK 8
#अन्यथा
# define SCHED_NR_MIGRATE_BREAK 32
#पूर्ण_अगर

बाह्य __read_mostly अचिन्हित पूर्णांक sysctl_sched_nr_migrate;
बाह्य __read_mostly अचिन्हित पूर्णांक sysctl_sched_migration_cost;

बाह्य अचिन्हित पूर्णांक sysctl_sched_base_slice;

बाह्य पूर्णांक sysctl_resched_latency_warn_ms;
बाह्य पूर्णांक sysctl_resched_latency_warn_once;

बाह्य अचिन्हित पूर्णांक sysctl_sched_tunable_scaling;

बाह्य अचिन्हित पूर्णांक sysctl_numa_balancing_scan_delay;
बाह्य अचिन्हित पूर्णांक sysctl_numa_balancing_scan_period_min;
बाह्य अचिन्हित पूर्णांक sysctl_numa_balancing_scan_period_max;
बाह्य अचिन्हित पूर्णांक sysctl_numa_balancing_scan_आकार;
बाह्य अचिन्हित पूर्णांक sysctl_numa_balancing_hot_threshold;

#अगर_घोषित CONFIG_SCHED_HRTICK

/*
 * Use hrtick when:
 *  - enabled by features
 *  - hrtimer is actually high res
 */
अटल अंतरभूत bool hrtick_enabled(काष्ठा rq *rq)
{
	वापस cpu_active(cpu_of(rq)) && hrtimer_highres_enabled();
}

अटल अंतरभूत bool hrtick_enabled_fair(काष्ठा rq *rq)
{
	वापस sched_feat(HRTICK) && hrtick_enabled(rq);
}

अटल अंतरभूत bool hrtick_enabled_dl(काष्ठा rq *rq)
{
	वापस sched_feat(HRTICK_DL) && hrtick_enabled(rq);
}

बाह्य व्योम hrtick_start(काष्ठा rq *rq, u64 delay);
अटल अंतरभूत bool hrtick_active(काष्ठा rq *rq)
{
	वापस hrtimer_active(&rq->hrtick_timer);
}

#अन्यथा /* !CONFIG_SCHED_HRTICK: */
अटल अंतरभूत bool hrtick_enabled_fair(काष्ठा rq *rq) { वापस false; }
अटल अंतरभूत bool hrtick_enabled_dl(काष्ठा rq *rq) { वापस false; }
अटल अंतरभूत bool hrtick_enabled(काष्ठा rq *rq) { वापस false; }
#पूर्ण_अगर /* !CONFIG_SCHED_HRTICK */

#अगर_अघोषित arch_scale_freq_tick
अटल __always_inline व्योम arch_scale_freq_tick(व्योम) { }
#पूर्ण_अगर

#अगर_अघोषित arch_scale_freq_capacity
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
अटल __always_inline
अचिन्हित दीर्घ arch_scale_freq_capacity(पूर्णांक cpu)
{
	वापस SCHED_CAPACITY_SCALE;
}
#पूर्ण_अगर

/*
 * In double_lock_balance()/double_rq_lock(), we use raw_spin_rq_lock() to
 * acquire rq lock instead of rq_lock(). So at the end of these two functions
 * we need to call double_rq_clock_clear_update() to clear RQCF_UPDATED of
 * rq->clock_update_flags to avoid the WARN_DOUBLE_CLOCK warning.
 */
अटल अंतरभूत व्योम double_rq_clock_clear_update(काष्ठा rq *rq1, काष्ठा rq *rq2)
{
	rq1->clock_update_flags &= (RQCF_REQ_SKIP|RQCF_ACT_SKIP);
	rq2->clock_update_flags &= (RQCF_REQ_SKIP|RQCF_ACT_SKIP);
}

#घोषणा DEFINE_LOCK_GUARD_2(name, type, _lock, _unlock, ...)				\
__DEFINE_UNLOCK_GUARD(name, type, _unlock, type *lock2; __VA_ARGS__)			\
अटल अंतरभूत class_##name##_t class_##name##_constructor(type *lock, type *lock2)	\
	__no_context_analysis								\
{ class_##name##_t _t = { .lock = lock, .lock2 = lock2 }, *_T = &_t;			\
  _lock; वापस _t; }
#घोषणा DECLARE_LOCK_GUARD_2_ATTRS(_name, _lock, _unlock1, _unlock2)			\
अटल अंतरभूत class_##_name##_t class_##_name##_constructor(lock_##_name##_t *_T1,	\
							    lock_##_name##_t *_T2) _lock; \
अटल __always_inline व्योम __class_##_name##_cleanup_ctx1(class_##_name##_t **_T1)	\
	__no_context_analysis _unlock1 { }						\
अटल __always_inline व्योम __class_##_name##_cleanup_ctx2(class_##_name##_t **_T2)	\
	__no_context_analysis _unlock2 { }
#घोषणा WITH_LOCK_GUARD_2_ATTRS(_name, _T1, _T2)					\
	class_##_name##_constructor(_T1, _T2),						\
	*__UNIQUE_ID(unlock1) __cleanup(__class_##_name##_cleanup_ctx1) = (व्योम *)(_T1),\
	*__UNIQUE_ID(unlock2) __cleanup(__class_##_name##_cleanup_ctx2) = (व्योम *)(_T2)

अटल अंतरभूत bool rq_order_less(काष्ठा rq *rq1, काष्ठा rq *rq2)
{
#अगर_घोषित CONFIG_SCHED_CORE
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
	अगर (rq1->core->cpu < rq2->core->cpu)
		वापस true;
	अगर (rq1->core->cpu > rq2->core->cpu)
		वापस false;

	/*
	 * __sched_core_flip() relies on SMT having cpu-id lock order.
	 */
#पूर्ण_अगर /* CONFIG_SCHED_CORE */
	वापस rq1->cpu < rq2->cpu;
}

बाह्य व्योम double_rq_lock(काष्ठा rq *rq1, काष्ठा rq *rq2)
	__acquires(__rq_lockp(rq1), __rq_lockp(rq2));

#अगर_घोषित CONFIG_PREEMPTION

/*
 * fair double_lock_balance: Safely acquires both rq->locks in a fair
 * way at the expense of forcing extra atomic operations in all
 * invocations.  This assures that the double_lock is acquired using the
 * same underlying policy as the spinlock_t on this architecture, which
 * reduces latency compared to the unfair variant below.  However, it
 * also adds more overhead and therefore may reduce throughput.
 */
अटल अंतरभूत पूर्णांक _double_lock_balance(काष्ठा rq *this_rq, काष्ठा rq *busiest)
	__must_hold(__rq_lockp(this_rq))
	__acquires(__rq_lockp(busiest))
{
	raw_spin_rq_unlock(this_rq);
	double_rq_lock(this_rq, busiest);

	वापस 1;
}

#अन्यथा /* !CONFIG_PREEMPTION: */
/*
 * Unfair double_lock_balance: Optimizes throughput at the expense of
 * latency by eliminating extra atomic operations when the locks are
 * already in proper order on entry.  This favors lower CPU-ids and will
 * grant the double lock to lower CPUs over higher ids under contention,
 * regardless of entry order into the function.
 */
अटल अंतरभूत पूर्णांक _double_lock_balance(काष्ठा rq *this_rq, काष्ठा rq *busiest)
	__must_hold(__rq_lockp(this_rq))
	__acquires(__rq_lockp(busiest))
{
	अगर (__rq_lockp(this_rq) == __rq_lockp(busiest)) {
		__acquire(__rq_lockp(busiest)); /* already held */
		double_rq_clock_clear_update(this_rq, busiest);
		वापस 0;
	}

	अगर (likely(raw_spin_rq_trylock(busiest))) {
		double_rq_clock_clear_update(this_rq, busiest);
		वापस 0;
	}

	अगर (rq_order_less(this_rq, busiest)) {
		raw_spin_rq_lock_nested(busiest, SINGLE_DEPTH_NESTING);
		double_rq_clock_clear_update(this_rq, busiest);
		वापस 0;
	}

	raw_spin_rq_unlock(this_rq);
	double_rq_lock(this_rq, busiest);

	वापस 1;
}

#पूर्ण_अगर /* !CONFIG_PREEMPTION */

/*
 * double_lock_balance - lock the busiest runqueue, this_rq is locked already.
 */
अटल अंतरभूत पूर्णांक double_lock_balance(काष्ठा rq *this_rq, काष्ठा rq *busiest)
	__must_hold(__rq_lockp(this_rq))
	__acquires(__rq_lockp(busiest))
{
	lockdep_assert_irqs_disabled();

	वापस _double_lock_balance(this_rq, busiest);
}

अटल अंतरभूत व्योम double_unlock_balance(काष्ठा rq *this_rq, काष्ठा rq *busiest)
	__releases(__rq_lockp(busiest))
{
	अगर (__rq_lockp(this_rq) != __rq_lockp(busiest))
		raw_spin_rq_unlock(busiest);
	अन्यथा
		__release(__rq_lockp(busiest)); /* fake release */
	lock_set_subclass(&__rq_lockp(this_rq)->dep_map, 0, _RET_IP_);
}

अटल अंतरभूत व्योम double_lock(spinlock_t *l1, spinlock_t *l2)
	__acquires(l1, l2)
{
	अगर (l1 > l2)
		swap(l1, l2);

	spin_lock(l1);
	spin_lock_nested(l2, SINGLE_DEPTH_NESTING);
}

अटल अंतरभूत व्योम double_lock_irq(spinlock_t *l1, spinlock_t *l2)
	__acquires(l1, l2)
{
	अगर (l1 > l2)
		swap(l1, l2);

	spin_lock_irq(l1);
	spin_lock_nested(l2, SINGLE_DEPTH_NESTING);
}

अटल अंतरभूत व्योम double_raw_lock(raw_spinlock_t *l1, raw_spinlock_t *l2)
	__acquires(l1, l2)
{
	अगर (l1 > l2)
		swap(l1, l2);

	raw_spin_lock(l1);
	raw_spin_lock_nested(l2, SINGLE_DEPTH_NESTING);
}

अटल अंतरभूत व्योम double_raw_unlock(raw_spinlock_t *l1, raw_spinlock_t *l2)
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
#घोषणा class_double_raw_spinlock_constructor(_T1, _T2) \
	WITH_LOCK_GUARD_2_ATTRS(double_raw_spinlock, _T1, _T2)

/*
 * double_rq_unlock - safely unlock two runqueues
 *
 * Note this does not restore interrupts like task_rq_unlock,
 * you need to do so manually after calling.
 */
अटल अंतरभूत व्योम double_rq_unlock(काष्ठा rq *rq1, काष्ठा rq *rq2)
	__releases(__rq_lockp(rq1), __rq_lockp(rq2))
{
	अगर (__rq_lockp(rq1) != __rq_lockp(rq2))
		raw_spin_rq_unlock(rq2);
	अन्यथा
		__release(__rq_lockp(rq2)); /* fake release */
	raw_spin_rq_unlock(rq1);
}

बाह्य व्योम set_rq_online (काष्ठा rq *rq);
बाह्य व्योम set_rq_offline(काष्ठा rq *rq);

बाह्य bool sched_smp_initialized;

DEFINE_LOCK_GUARD_2(double_rq_lock, काष्ठा rq,
		    double_rq_lock(_T->lock, _T->lock2),
		    double_rq_unlock(_T->lock, _T->lock2))

बाह्य काष्ठा sched_entity *__pick_root_entity(काष्ठा cfs_rq *cfs_rq);
बाह्य काष्ठा sched_entity *__pick_first_entity(काष्ठा cfs_rq *cfs_rq);
बाह्य काष्ठा sched_entity *__pick_last_entity(काष्ठा cfs_rq *cfs_rq);

बाह्य bool sched_debug_verbose;

बाह्य व्योम print_cfs_stats(काष्ठा seq_file *m, पूर्णांक cpu);
बाह्य व्योम print_rt_stats(काष्ठा seq_file *m, पूर्णांक cpu);
बाह्य व्योम print_dl_stats(काष्ठा seq_file *m, पूर्णांक cpu);
बाह्य व्योम print_cfs_rq(काष्ठा seq_file *m, पूर्णांक cpu, काष्ठा cfs_rq *cfs_rq);
बाह्य व्योम print_rt_rq(काष्ठा seq_file *m, पूर्णांक cpu, काष्ठा rt_rq *rt_rq);
बाह्य व्योम print_dl_rq(काष्ठा seq_file *m, पूर्णांक cpu, काष्ठा dl_rq *dl_rq);

बाह्य व्योम resched_latency_warn(पूर्णांक cpu, u64 latency);

#अगर_घोषित CONFIG_NUMA_BALANCING
बाह्य व्योम show_numa_stats(काष्ठा task_struct *p, काष्ठा seq_file *m);
बाह्य व्योम
print_numa_stats(काष्ठा seq_file *m, पूर्णांक गाँठ, अचिन्हित दीर्घ tsf,
		 अचिन्हित दीर्घ tpf, अचिन्हित दीर्घ gsf, अचिन्हित दीर्घ gpf);
#पूर्ण_अगर /* CONFIG_NUMA_BALANCING */

बाह्य व्योम init_cfs_rq(काष्ठा cfs_rq *cfs_rq);
बाह्य व्योम init_rt_rq(काष्ठा rt_rq *rt_rq);
बाह्य व्योम init_dl_rq(काष्ठा dl_rq *dl_rq);

बाह्य व्योम cfs_bandwidth_usage_inc(व्योम);
बाह्य व्योम cfs_bandwidth_usage_dec(व्योम);

#अगर_घोषित CONFIG_NO_HZ_COMMON

#घोषणा NOHZ_BALANCE_KICK_BIT	0
#घोषणा NOHZ_STATS_KICK_BIT	1
#घोषणा NOHZ_NEWILB_KICK_BIT	2
#घोषणा NOHZ_अग्र_KICK_BIT	3

/* Run sched_balance_domains() */
#घोषणा NOHZ_BALANCE_KICK	BIT(NOHZ_BALANCE_KICK_BIT)
/* Update blocked load */
#घोषणा NOHZ_STATS_KICK		BIT(NOHZ_STATS_KICK_BIT)
/* Update blocked load when entering idle */
#घोषणा NOHZ_NEWILB_KICK	BIT(NOHZ_NEWILB_KICK_BIT)
/* Update nohz.next_balance */
#घोषणा NOHZ_अग्र_KICK		BIT(NOHZ_अग्र_KICK_BIT)

#घोषणा NOHZ_KICK_MASK		(NOHZ_BALANCE_KICK | NOHZ_STATS_KICK | NOHZ_अग्र_KICK)

#घोषणा nohz_flags(cpu)		(&cpu_rq(cpu)->nohz_flags)

बाह्य व्योम nohz_balance_exit_idle(काष्ठा rq *rq);
#अन्यथा /* !CONFIG_NO_HZ_COMMON: */
अटल अंतरभूत व्योम nohz_balance_exit_idle(काष्ठा rq *rq) { }
#पूर्ण_अगर /* !CONFIG_NO_HZ_COMMON */

#अगर_घोषित CONFIG_NO_HZ_COMMON
बाह्य व्योम nohz_run_idle_balance(पूर्णांक cpu);
#अन्यथा
अटल अंतरभूत व्योम nohz_run_idle_balance(पूर्णांक cpu) { }
#पूर्ण_अगर

#समावेश "stats.h"

#अगर defined(CONFIG_SCHED_CORE) && defined(CONFIG_SCHEDSTATS)

बाह्य व्योम __sched_core_account_forceidle(काष्ठा rq *rq);

अटल अंतरभूत व्योम sched_core_account_forceidle(काष्ठा rq *rq)
{
	अगर (schedstat_enabled())
		__sched_core_account_forceidle(rq);
}

बाह्य व्योम __sched_core_tick(काष्ठा rq *rq);

अटल अंतरभूत व्योम sched_core_tick(काष्ठा rq *rq)
{
	अगर (sched_core_enabled(rq) && schedstat_enabled())
		__sched_core_tick(rq);
}

#अन्यथा /* !(CONFIG_SCHED_CORE && CONFIG_SCHEDSTATS): */

अटल अंतरभूत व्योम sched_core_account_forceidle(काष्ठा rq *rq) { }

अटल अंतरभूत व्योम sched_core_tick(काष्ठा rq *rq) { }

#पूर्ण_अगर /* !(CONFIG_SCHED_CORE && CONFIG_SCHEDSTATS) */

#अगर_घोषित CONFIG_IRQ_TIME_ACCOUNTING

काष्ठा irqtime {
	u64			योग_2;
	u64			tick_delta;
	u64			irq_start_time;
	काष्ठा u64_stats_sync	sync;
};

DECLARE_PER_CPU(काष्ठा irqtime, cpu_irqtime);
DECLARE_STATIC_कुंजी_FALSE(sched_clock_irqtime);

अटल अंतरभूत पूर्णांक irqtime_enabled(व्योम)
{
	वापस static_branch_likely(&sched_clock_irqtime);
}

/*
 * Returns the irqtime minus the softirq time computed by ksoftirqd.
 * Otherwise ksoftirqd's sum_exec_runtime is subtracted its own runtime
 * and never move forward.
 */
अटल अंतरभूत u64 irq_time_read(पूर्णांक cpu)
{
	काष्ठा irqtime *irqtime = &per_cpu(cpu_irqtime, cpu);
	अचिन्हित पूर्णांक seq;
	u64 योग_2;

	करो {
		seq = __u64_stats_fetch_begin(&irqtime->sync);
		योग_2 = irqtime->योग_2;
	} जबतक (__u64_stats_fetch_retry(&irqtime->sync, seq));

	वापस योग_2;
}

#अन्यथा /* !CONFIG_IRQ_TIME_ACCOUNTING: */

अटल अंतरभूत पूर्णांक irqtime_enabled(व्योम)
{
	वापस 0;
}

#पूर्ण_अगर /* !CONFIG_IRQ_TIME_ACCOUNTING */

#अगर_घोषित CONFIG_CPU_FREQ

DECLARE_PER_CPU(काष्ठा update_util_तथ्य __rcu *, cpufreq_update_util_तथ्य);

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
अटल अंतरभूत व्योम cpufreq_update_util(काष्ठा rq *rq, अचिन्हित पूर्णांक flags)
{
	काष्ठा update_util_तथ्य *तथ्य;

	तथ्य = rcu_dereference_sched(*per_cpu_ptr(&cpufreq_update_util_तथ्य,
						  cpu_of(rq)));
	अगर (तथ्य)
		तथ्य->func(तथ्य, rq_clock(rq), flags);
}
#अन्यथा /* !CONFIG_CPU_FREQ: */
अटल अंतरभूत व्योम cpufreq_update_util(काष्ठा rq *rq, अचिन्हित पूर्णांक flags) { }
#पूर्ण_अगर /* !CONFIG_CPU_FREQ */

#अगर_घोषित arch_scale_freq_capacity
# ifndef arch_scale_freq_invariant
#  define arch_scale_freq_invariant()	true
# endif
#अन्यथा
# define arch_scale_freq_invariant()	false
#पूर्ण_अगर

अचिन्हित दीर्घ effective_cpu_util(पूर्णांक cpu, अचिन्हित दीर्घ util_cfs,
				 अचिन्हित दीर्घ *min,
				 अचिन्हित दीर्घ *max);

अचिन्हित दीर्घ sugov_effective_cpu_perf(पूर्णांक cpu, अचिन्हित दीर्घ actual,
				 अचिन्हित दीर्घ min,
				 अचिन्हित दीर्घ max);


/*
 * Verify the fitness of task @p to run on @cpu taking into account the
 * CPU original capacity and the runtime/deadline ratio of the task.
 *
 * The function will return true if the original capacity of @cpu is
 * greater than or equal to task's deadline density right shifted by
 * (BW_SHIFT - SCHED_CAPACITY_SHIFT) and false otherwise.
 */
अटल अंतरभूत bool dl_task_fits_capacity(काष्ठा task_struct *p, पूर्णांक cpu)
{
	अचिन्हित दीर्घ cap = arch_scale_cpu_capacity(cpu);

	वापस cap >= p->dl.dl_density >> (BW_SHIFT - SCHED_CAPACITY_SHIFT);
}

अटल अंतरभूत अचिन्हित दीर्घ cpu_bw_dl(काष्ठा rq *rq)
{
	वापस (rq->dl.running_bw * SCHED_CAPACITY_SCALE) >> BW_SHIFT;
}

अटल अंतरभूत अचिन्हित दीर्घ cpu_util_dl(काष्ठा rq *rq)
{
	वापस READ_ONCE(rq->avg_dl.util_avg);
}


बाह्य अचिन्हित दीर्घ cpu_util_cfs(पूर्णांक cpu);
बाह्य अचिन्हित दीर्घ cpu_util_cfs_boost(पूर्णांक cpu);

अटल अंतरभूत अचिन्हित दीर्घ cpu_util_rt(काष्ठा rq *rq)
{
	वापस READ_ONCE(rq->avg_rt.util_avg);
}

#अगर_घोषित CONFIG_UCLAMP_TASK

अचिन्हित दीर्घ uclamp_eff_मूल्य(काष्ठा task_struct *p, क्रमागत uclamp_id clamp_id);

/*
 * When uclamp is compiled in, the aggregation at rq level is 'turned off'
 * by default in the fast path and only gets turned on once userspace performs
 * an operation that requires it.
 *
 * Returns true if userspace opted-in to use uclamp and aggregation at rq level
 * hence is active.
 */
अटल अंतरभूत bool uclamp_is_used(व्योम)
{
	वापस static_branch_likely(&sched_uclamp_used);
}

/*
 * Enabling static branches would get the cpus_read_lock(),
 * check whether uclamp_is_used before enable it to avoid always
 * calling cpus_read_lock(). Because we never disable this
 * static key once enable it.
 */
अटल अंतरभूत व्योम sched_uclamp_enable(व्योम)
{
	अगर (!uclamp_is_used())
		static_branch_enable(&sched_uclamp_used);
}

अटल अंतरभूत अचिन्हित दीर्घ uclamp_rq_get(काष्ठा rq *rq,
					  क्रमागत uclamp_id clamp_id)
{
	वापस READ_ONCE(rq->uclamp[clamp_id].मूल्य);
}

अटल अंतरभूत व्योम uclamp_rq_set(काष्ठा rq *rq, क्रमागत uclamp_id clamp_id,
				 अचिन्हित पूर्णांक मूल्य)
{
	WRITE_ONCE(rq->uclamp[clamp_id].मूल्य, मूल्य);
}

अटल अंतरभूत bool uclamp_rq_is_idle(काष्ठा rq *rq)
{
	वापस rq->uclamp_flags & UCLAMP_FLAG_IDLE;
}

/* Is the rq being capped/throttled by uclamp_max? */
अटल अंतरभूत bool uclamp_rq_is_capped(काष्ठा rq *rq)
{
	अचिन्हित दीर्घ rq_util;
	अचिन्हित दीर्घ max_util;

	अगर (!uclamp_is_used())
		वापस false;

	rq_util = cpu_util_cfs(cpu_of(rq)) + cpu_util_rt(rq);
	max_util = READ_ONCE(rq->uclamp[UCLAMP_MAX].मूल्य);

	वापस max_util != SCHED_CAPACITY_SCALE && rq_util >= max_util;
}

#घोषणा for_each_clamp_id(clamp_id) \
	क्रम ((clamp_id) = 0; (clamp_id) < UCLAMP_CNT; (clamp_id)++)

बाह्य अचिन्हित पूर्णांक sysctl_sched_uclamp_util_min_rt_default;


अटल अंतरभूत अचिन्हित पूर्णांक uclamp_none(क्रमागत uclamp_id clamp_id)
{
	अगर (clamp_id == UCLAMP_MIN)
		वापस 0;
	वापस SCHED_CAPACITY_SCALE;
}

/* Integer rounded range for each bucket */
#घोषणा UCLAMP_BUCKET_DELTA DIV_ROUND_CLOSEST(SCHED_CAPACITY_SCALE, UCLAMP_BUCKETS)

अटल अंतरभूत अचिन्हित पूर्णांक uclamp_bucket_id(अचिन्हित पूर्णांक clamp_मूल्य)
{
	वापस min_t(अचिन्हित पूर्णांक, clamp_मूल्य / UCLAMP_BUCKET_DELTA, UCLAMP_BUCKETS - 1);
}

अटल अंतरभूत व्योम
uclamp_se_set(काष्ठा uclamp_se *uc_se, अचिन्हित पूर्णांक मूल्य, bool user_defined)
{
	uc_se->मूल्य = मूल्य;
	uc_se->bucket_id = uclamp_bucket_id(मूल्य);
	uc_se->user_defined = user_defined;
}

#अन्यथा /* !CONFIG_UCLAMP_TASK: */

अटल अंतरभूत अचिन्हित दीर्घ
uclamp_eff_मूल्य(काष्ठा task_struct *p, क्रमागत uclamp_id clamp_id)
{
	अगर (clamp_id == UCLAMP_MIN)
		वापस 0;

	वापस SCHED_CAPACITY_SCALE;
}

अटल अंतरभूत bool uclamp_rq_is_capped(काष्ठा rq *rq) { वापस false; }

अटल अंतरभूत bool uclamp_is_used(व्योम)
{
	वापस false;
}

अटल अंतरभूत व्योम sched_uclamp_enable(व्योम) {}

अटल अंतरभूत अचिन्हित दीर्घ
uclamp_rq_get(काष्ठा rq *rq, क्रमागत uclamp_id clamp_id)
{
	अगर (clamp_id == UCLAMP_MIN)
		वापस 0;

	वापस SCHED_CAPACITY_SCALE;
}

अटल अंतरभूत व्योम
uclamp_rq_set(काष्ठा rq *rq, क्रमागत uclamp_id clamp_id, अचिन्हित पूर्णांक मूल्य)
{
}

अटल अंतरभूत bool uclamp_rq_is_idle(काष्ठा rq *rq)
{
	वापस false;
}

#पूर्ण_अगर /* !CONFIG_UCLAMP_TASK */

#अगर_घोषित CONFIG_HAVE_SCHED_AVG_IRQ

अटल अंतरभूत अचिन्हित दीर्घ cpu_util_irq(काष्ठा rq *rq)
{
	वापस READ_ONCE(rq->avg_irq.util_avg);
}

अटल अंतरभूत
अचिन्हित दीर्घ scale_irq_capacity(अचिन्हित दीर्घ util, अचिन्हित दीर्घ irq, अचिन्हित दीर्घ max)
{
	util *= (max - irq);
	util /= max;

	वापस util;

}

#अन्यथा /* !CONFIG_HAVE_SCHED_AVG_IRQ: */

अटल अंतरभूत अचिन्हित दीर्घ cpu_util_irq(काष्ठा rq *rq)
{
	वापस 0;
}

अटल अंतरभूत
अचिन्हित दीर्घ scale_irq_capacity(अचिन्हित दीर्घ util, अचिन्हित दीर्घ irq, अचिन्हित दीर्घ max)
{
	वापस util;
}

#पूर्ण_अगर /* !CONFIG_HAVE_SCHED_AVG_IRQ */

बाह्य व्योम __setparam_fair(काष्ठा task_struct *p, स्थिर काष्ठा sched_attr *attr);

#अगर defined(CONFIG_ENERGY_MODEL) && defined(CONFIG_CPU_FREQ_GOV_SCHEDUTIL)

#घोषणा perf_domain_span(pd) (to_cpumask(((pd)->em_pd->cpus)))

DECLARE_STATIC_कुंजी_FALSE(sched_energy_present);

अटल अंतरभूत bool sched_energy_enabled(व्योम)
{
	वापस static_branch_unlikely(&sched_energy_present);
}

#अन्यथा /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL): */

#घोषणा perf_domain_span(pd) शून्य

अटल अंतरभूत bool sched_energy_enabled(व्योम) { वापस false; }

#पूर्ण_अगर /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL) */

#अगर_घोषित CONFIG_MEMBARRIER

/*
 * The scheduler provides memory barriers required by membarrier between:
 * - prior user-space memory accesses and store to rq->membarrier_state,
 * - store to rq->membarrier_state and following user-space memory accesses.
 * In the same way it provides those guarantees around store to rq->curr.
 */
अटल अंतरभूत व्योम membarrier_switch_mm(काष्ठा rq *rq,
					काष्ठा mm_struct *पूर्व_mm,
					काष्ठा mm_struct *अग्र_mm)
{
	पूर्णांक membarrier_state;

	अगर (पूर्व_mm == अग्र_mm)
		वापस;

	membarrier_state = atomic_read(&अग्र_mm->membarrier_state);
	अगर (READ_ONCE(rq->membarrier_state) == membarrier_state)
		वापस;

	WRITE_ONCE(rq->membarrier_state, membarrier_state);
}

#अन्यथा /* !CONFIG_MEMBARRIER: */

अटल अंतरभूत व्योम membarrier_switch_mm(काष्ठा rq *rq,
					काष्ठा mm_struct *पूर्व_mm,
					काष्ठा mm_struct *अग्र_mm)
{
}

#पूर्ण_अगर /* !CONFIG_MEMBARRIER */

अटल अंतरभूत bool is_per_cpu_kthread(काष्ठा task_struct *p)
{
	अगर (!(p->flags & PF_KTHREAD))
		वापस false;

	अगर (p->nr_cpus_allowed != 1)
		वापस false;

	वापस true;
}

बाह्य व्योम swake_up_all_locked(काष्ठा swait_queue_शीर्ष *q);
बाह्य व्योम __prepare_to_swait(काष्ठा swait_queue_शीर्ष *q, काष्ठा swait_queue *रुको);

बाह्य पूर्णांक try_to_wake_up(काष्ठा task_struct *tsk, अचिन्हित पूर्णांक state, पूर्णांक wake_flags);

#अगर_घोषित CONFIG_PREEMPT_DYNAMIC
बाह्य पूर्णांक preempt_dynamic_mode;
बाह्य पूर्णांक sched_dynamic_mode(स्थिर अक्षर *str);
बाह्य व्योम sched_dynamic_update(पूर्णांक mode);
#पूर्ण_अगर
बाह्य स्थिर अक्षर *preempt_modes[];

#अगर_घोषित CONFIG_SCHED_MM_CID

अटल __always_inline bool cid_on_cpu(अचिन्हित पूर्णांक cid)
{
	वापस cid & MM_CID_ONCPU;
}

अटल __always_inline bool cid_in_transit(अचिन्हित पूर्णांक cid)
{
	वापस cid & MM_CID_TRANSIT;
}

अटल __always_inline अचिन्हित पूर्णांक cpu_cid_to_cid(अचिन्हित पूर्णांक cid)
{
	वापस cid & ~MM_CID_ONCPU;
}

अटल __always_inline अचिन्हित पूर्णांक cid_to_cpu_cid(अचिन्हित पूर्णांक cid)
{
	वापस cid | MM_CID_ONCPU;
}

अटल __always_inline अचिन्हित पूर्णांक cid_to_transit_cid(अचिन्हित पूर्णांक cid)
{
	वापस cid | MM_CID_TRANSIT;
}

अटल __always_inline अचिन्हित पूर्णांक cid_from_transit_cid(अचिन्हित पूर्णांक cid)
{
	वापस cid & ~MM_CID_TRANSIT;
}

अटल __always_inline bool cid_on_task(अचिन्हित पूर्णांक cid)
{
	/* True if none of the MM_CID_ONCPU, MM_CID_TRANSIT, MM_CID_UNSET bits is set */
	वापस cid < MM_CID_TRANSIT;
}

अटल __always_inline व्योम mm_drop_cid(काष्ठा mm_struct *mm, अचिन्हित पूर्णांक cid)
{
	clear_bit(cid, mm_cidmask(mm));
}

अटल __always_inline व्योम mm_unset_cid_on_task(काष्ठा task_struct *t)
{
	अचिन्हित पूर्णांक cid = t->mm_cid.cid;

	t->mm_cid.cid = MM_CID_UNSET;
	अगर (cid_on_task(cid))
		mm_drop_cid(t->mm, cid);
}

अटल __always_inline व्योम mm_drop_cid_on_cpu(काष्ठा mm_struct *mm, काष्ठा mm_cid_pcpu *pcp)
{
	/* Clear the ONCPU bit, but do not set UNSET in the per CPU storage */
	अगर (cid_on_cpu(pcp->cid)) {
		pcp->cid = cpu_cid_to_cid(pcp->cid);
		mm_drop_cid(mm, pcp->cid);
	}
}

अटल अंतरभूत अचिन्हित पूर्णांक __mm_get_cid(काष्ठा mm_struct *mm, अचिन्हित पूर्णांक max_cids)
{
	अचिन्हित पूर्णांक cid = find_first_zero_bit(mm_cidmask(mm), max_cids);

	अगर (cid >= max_cids)
		वापस MM_CID_UNSET;
	अगर (test_and_set_bit(cid, mm_cidmask(mm)))
		वापस MM_CID_UNSET;
	वापस cid;
}

अटल अंतरभूत अचिन्हित पूर्णांक mm_get_cid(काष्ठा mm_struct *mm)
{
	अचिन्हित पूर्णांक cid = __mm_get_cid(mm, READ_ONCE(mm->mm_cid.max_cids));

	जबतक (cid == MM_CID_UNSET) {
		cpu_relax();
		cid = __mm_get_cid(mm, अंक_possible_cpus());
	}
	वापस cid;
}

अटल अंतरभूत अचिन्हित पूर्णांक mm_cid_converge(काष्ठा mm_struct *mm, अचिन्हित पूर्णांक orig_cid,
					   अचिन्हित पूर्णांक max_cids)
{
	अचिन्हित पूर्णांक new_cid, cid = cpu_cid_to_cid(orig_cid);

	/* Is it in the optimal CID space? */
	अगर (likely(cid < max_cids))
		वापस orig_cid;

	/* Try to find one in the optimal space. Otherwise keep the provided. */
	new_cid = __mm_get_cid(mm, max_cids);
	अगर (new_cid != MM_CID_UNSET) {
		mm_drop_cid(mm, cid);
		/* Preserve the ONCPU mode of the original CID */
		वापस new_cid | (orig_cid & MM_CID_ONCPU);
	}
	वापस orig_cid;
}

अटल __always_inline व्योम mm_cid_update_task_cid(काष्ठा task_struct *t, अचिन्हित पूर्णांक cid)
{
	अगर (t->mm_cid.cid != cid) {
		t->mm_cid.cid = cid;
		rseq_sched_set_ids_changed(t);
	}
}

अटल __always_inline व्योम mm_cid_update_pcpu_cid(काष्ठा mm_struct *mm, अचिन्हित पूर्णांक cid)
{
	__this_cpu_write(mm->mm_cid.pcpu->cid, cid);
}

अटल __always_inline व्योम mm_cid_from_cpu(काष्ठा task_struct *t, अचिन्हित पूर्णांक cpu_cid,
					    अचिन्हित पूर्णांक mode)
{
	अचिन्हित पूर्णांक max_cids, tcid = t->mm_cid.cid;
	काष्ठा mm_struct *mm = t->mm;

	max_cids = READ_ONCE(mm->mm_cid.max_cids);
	/* Optimize for the common case where both have the ONCPU bit set */
	अगर (likely(cid_on_cpu(cpu_cid & tcid))) {
		अगर (likely(cpu_cid_to_cid(cpu_cid) < max_cids)) {
			mm_cid_update_task_cid(t, cpu_cid);
			वापस;
		}
		/* Try to converge into the optimal CID space */
		cpu_cid = mm_cid_converge(mm, cpu_cid, max_cids);
	} अन्यथा {
		/* Hand over or drop the task owned CID */
		अगर (cid_on_task(tcid)) {
			अगर (cid_on_cpu(cpu_cid))
				mm_unset_cid_on_task(t);
			अन्यथा
				cpu_cid = cid_to_cpu_cid(tcid);
		}
		/* Still nothing, allocate a new one */
		अगर (!cid_on_cpu(cpu_cid))
			cpu_cid = cid_to_cpu_cid(mm_get_cid(mm));

		/* Handle the transition mode flag if required */
		अगर (mode & MM_CID_TRANSIT)
			cpu_cid = cpu_cid_to_cid(cpu_cid) | MM_CID_TRANSIT;
	}
	mm_cid_update_pcpu_cid(mm, cpu_cid);
	mm_cid_update_task_cid(t, cpu_cid);
}

अटल __always_inline व्योम mm_cid_from_task(काष्ठा task_struct *t, अचिन्हित पूर्णांक cpu_cid,
					     अचिन्हित पूर्णांक mode)
{
	अचिन्हित पूर्णांक max_cids, tcid = t->mm_cid.cid;
	काष्ठा mm_struct *mm = t->mm;

	max_cids = READ_ONCE(mm->mm_cid.max_cids);
	/* Optimize for the common case, where both have the ONCPU bit clear */
	अगर (likely(cid_on_task(tcid | cpu_cid))) {
		अगर (likely(tcid < max_cids)) {
			mm_cid_update_pcpu_cid(mm, tcid);
			वापस;
		}
		/* Try to converge into the optimal CID space */
		tcid = mm_cid_converge(mm, tcid, max_cids);
	} अन्यथा {
		/* Hand over or drop the CPU owned CID */
		अगर (cid_on_cpu(cpu_cid)) {
			अगर (cid_on_task(tcid))
				mm_drop_cid_on_cpu(mm, this_cpu_ptr(mm->mm_cid.pcpu));
			अन्यथा
				tcid = cpu_cid_to_cid(cpu_cid);
		}
		/* Still nothing, allocate a new one */
		अगर (!cid_on_task(tcid))
			tcid = mm_get_cid(mm);
		/* Set the transition mode flag if required */
		tcid |= mode & MM_CID_TRANSIT;
	}
	mm_cid_update_pcpu_cid(mm, tcid);
	mm_cid_update_task_cid(t, tcid);
}

अटल __always_inline व्योम mm_cid_schedin(काष्ठा task_struct *अग्र)
{
	काष्ठा mm_struct *mm = अग्र->mm;
	अचिन्हित पूर्णांक cpu_cid, mode;

	अगर (!अग्र->mm_cid.active)
		वापस;

	cpu_cid = __this_cpu_read(mm->mm_cid.pcpu->cid);
	mode = READ_ONCE(mm->mm_cid.mode);
	अगर (likely(!cid_on_cpu(mode)))
		mm_cid_from_task(अग्र, cpu_cid, mode);
	अन्यथा
		mm_cid_from_cpu(अग्र, cpu_cid, mode);
}

अटल __always_inline व्योम mm_cid_schedout(काष्ठा task_struct *पूर्व)
{
	काष्ठा mm_struct *mm = पूर्व->mm;
	अचिन्हित पूर्णांक mode, cid;

	/* During mode transitions CIDs are temporary and need to be dropped */
	अगर (likely(!cid_in_transit(पूर्व->mm_cid.cid)))
		वापस;

	mode = READ_ONCE(mm->mm_cid.mode);
	cid = cid_from_transit_cid(पूर्व->mm_cid.cid);

	/*
	 * If transition mode is done, transfer ownership when the CID is
	 * within the convergence range to optimize the next schedule in.
	 */
	अगर (!cid_in_transit(mode) && cid < READ_ONCE(mm->mm_cid.max_cids)) {
		अगर (cid_on_cpu(mode))
			cid = cid_to_cpu_cid(cid);

		/* Update both so that the next schedule in goes into the fast path */
		mm_cid_update_pcpu_cid(mm, cid);
		पूर्व->mm_cid.cid = cid;
	} अन्यथा {
		mm_drop_cid(mm, cid);
		पूर्व->mm_cid.cid = MM_CID_UNSET;
	}
}

अटल अंतरभूत व्योम mm_cid_switch_to(काष्ठा task_struct *पूर्व, काष्ठा task_struct *अग्र)
{
	mm_cid_schedout(पूर्व);
	mm_cid_schedin(अग्र);
}

#अन्यथा /* !CONFIG_SCHED_MM_CID: */
अटल अंतरभूत व्योम mm_cid_switch_to(काष्ठा task_struct *पूर्व, काष्ठा task_struct *अग्र) { }
#पूर्ण_अगर /* !CONFIG_SCHED_MM_CID */

#अगर_घोषित CONFIG_SCHED_CACHE
DECLARE_STATIC_कुंजी_FALSE(sched_cache_present);
DECLARE_STATIC_कुंजी_FALSE(sched_cache_active);
बाह्य पूर्णांक sysctl_sched_cache_user;
बाह्य अचिन्हित पूर्णांक llc_aggr_tolerance;
बाह्य अचिन्हित पूर्णांक llc_epoch_period;
बाह्य अचिन्हित पूर्णांक llc_epoch_affinity_timeout;
बाह्य अचिन्हित पूर्णांक llc_imb_pct;
बाह्य अचिन्हित पूर्णांक llc_overaggr_pct;

अटल अंतरभूत bool sched_cache_enabled(व्योम)
{
	वापस static_branch_unlikely(&sched_cache_active);
}

बाह्य व्योम sched_cache_active_set(व्योम);

#पूर्ण_अगर

व्योम sched_domains_free_llc_id(पूर्णांक cpu);

बाह्य व्योम init_sched_mm(काष्ठा task_struct *p);

बाह्य u64 avg_vruntime(काष्ठा cfs_rq *cfs_rq);
बाह्य पूर्णांक entity_eligible(काष्ठा cfs_rq *cfs_rq, काष्ठा sched_entity *se);
अटल अंतरभूत
व्योम move_queued_task_locked(काष्ठा rq *src_rq, काष्ठा rq *dst_rq, काष्ठा task_struct *task)
{
	lockdep_assert_rq_held(src_rq);
	lockdep_assert_rq_held(dst_rq);

	deactivate_task(src_rq, task, 0);
	set_task_cpu(task, dst_rq->cpu);
	activate_task(dst_rq, task, 0);
	wakeup_preempt(dst_rq, task, 0);
}

अटल अंतरभूत
bool task_is_pushable(काष्ठा rq *rq, काष्ठा task_struct *p, पूर्णांक cpu)
{
	अगर (!task_on_cpu(rq, p) &&
	    cpumask_test_cpu(cpu, &p->cpus_mask))
		वापस true;

	वापस false;
}

#अगर_घोषित CONFIG_RT_MUTEXES

अटल अंतरभूत पूर्णांक __rt_effective_prio(काष्ठा task_struct *pi_task, पूर्णांक prio)
{
	अगर (pi_task)
		prio = min(prio, pi_task->prio);

	वापस prio;
}

अटल अंतरभूत पूर्णांक rt_effective_prio(काष्ठा task_struct *p, पूर्णांक prio)
{
	काष्ठा task_struct *pi_task = rt_mutex_get_शीर्ष_task(p);

	वापस __rt_effective_prio(pi_task, prio);
}

#अन्यथा /* !CONFIG_RT_MUTEXES: */

अटल अंतरभूत पूर्णांक rt_effective_prio(काष्ठा task_struct *p, पूर्णांक prio)
{
	वापस prio;
}

#पूर्ण_अगर /* !CONFIG_RT_MUTEXES */

बाह्य पूर्णांक __sched_setscheduler(काष्ठा task_struct *p, स्थिर काष्ठा sched_attr *attr, bool user, bool pi);
बाह्य पूर्णांक __sched_setaffinity(काष्ठा task_struct *p, काष्ठा affinity_context *ctx);
बाह्य स्थिर काष्ठा sched_class *__setscheduler_class(पूर्णांक policy, पूर्णांक prio);
बाह्य व्योम set_load_weight(काष्ठा task_struct *p, bool update_load);
बाह्य व्योम enqueue_task(काष्ठा rq *rq, काष्ठा task_struct *p, पूर्णांक flags);
बाह्य bool dequeue_task(काष्ठा rq *rq, काष्ठा task_struct *p, पूर्णांक flags);

बाह्य काष्ठा balance_callback *splice_balance_callbacks(काष्ठा rq *rq);

बाह्य व्योम __balance_callbacks(काष्ठा rq *rq, काष्ठा rq_flags *rf);
बाह्य व्योम balance_callbacks(काष्ठा rq *rq, काष्ठा balance_callback *शीर्ष);

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
काष्ठा sched_change_ctx {
	u64			prio;
	काष्ठा task_struct	*p;
	स्थिर काष्ठा sched_class *class;
	पूर्णांक			flags;
	bool			queued;
	bool			running;
};

काष्ठा sched_change_ctx *sched_change_begin(काष्ठा task_struct *p, अचिन्हित पूर्णांक flags);
व्योम sched_change_end(काष्ठा sched_change_ctx *ctx);

DEFINE_CLASS(sched_change, काष्ठा sched_change_ctx *,
	     sched_change_end(_T),
	     sched_change_begin(p, flags),
	     काष्ठा task_struct *p, अचिन्हित पूर्णांक flags)

DEFINE_CLASS_IS_UNCONDITIONAL(sched_change)

#समावेश "ext/ext.h"

#अगर_घोषित CONFIG_PREFERRED_CPU
व्योम sched_धकेलो_current_non_preferred_cpu(काष्ठा rq *rq);
#अन्यथा	/* !CONFIG_PREFERRED_CPU */
अटल अंतरभूत व्योम sched_धकेलो_current_non_preferred_cpu(काष्ठा rq *rq) { }
#पूर्ण_अगर

#पूर्ण_अगर /* _KERNEL_SCHED_SCHED_H */
