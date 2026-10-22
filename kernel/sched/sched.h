/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Scheduler internal types and methods:
 */
#यदि_अपरिभाषितम् _KERNEL_SCHED_SCHED_H
#परिभाषय _KERNEL_SCHED_SCHED_H

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
संरचना sched_गणः;
संरचना cpuidle_state;

#यदि defined(CONFIG_PARAVIRT) && !defined(CONFIG_HAVE_PV_STEAL_CLOCK_GEN)
# include <asm/paravirt.h>
#यदि_अन्तः

#अन्तर्भाव <asm/barrier.h>

#अन्तर्भाव "cpupri.h"
#अन्तर्भाव "cpudeadline.h"

/* task_struct::on_rq states: */
#परिभाषय TASK_ON_RQ_QUEUED	1
#परिभाषय TASK_ON_RQ_MIGRATING	2

बाह्य __read_mostly पूर्णाङ्क scheduler_running;

बाह्य अचिह्नित दीर्घ calc_आरोपय_update;
बाह्य atomic_long_t calc_आरोपय_tasks;

बाह्य शून्य calc_global_आरोपय_tick(संरचना rq *this_rq);
बाह्य दीर्घ calc_आरोपय_fold_सक्रिय(संरचना rq *this_rq, दीर्घ adjust);

बाह्य शून्य call_trace_sched_update_nr_running(संरचना rq *rq, पूर्णाङ्क गणनम्);

बाह्य पूर्णाङ्क sysctl_sched_rt_period;
बाह्य पूर्णाङ्क sysctl_sched_rt_runtime;
बाह्य पूर्णाङ्क sched_rr_timeslice;

/*
 * Asymmetric CPU capacity bits
 */
संरचना asym_cap_दत्त {
	संरचना list_शीर्ष link;
	संरचना rcu_शीर्ष rcu;
	अचिह्नित दीर्घ क्षमता;
	अचिह्नित दीर्घ cpus[];
};

बाह्य संरचना list_शीर्ष asym_cap_list;

#परिभाषय cpu_क्षमता_विस्तृतिः(asym_दत्त) गन्तव्यम्_cpumask((asym_दत्त)->cpus)

/*
 * Helpers for converting nanosecond timing to jiffy resolution
 */
#परिभाषय NS_गन्तव्यम्_JIFFIES(time)	((अचिह्नित दीर्घ)(time) / (NSEC_प्रत्येकम्_SEC/HZ))

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
#यदि_परिभाषितम् CONFIG_64BIT
# define NICE_0_आरोपय_सरणम्	(SCHED_FIXEDPOINT_सरणम् + SCHED_FIXEDPOINT_सरणम्)
# define scale_आरोपय(w)		((w) << SCHED_FIXEDPOINT_सरणम्)
# define scale_आरोपय_अधस्तात्(w)					\
({								\
	अचिह्नित दीर्घ __w = (w);				\
								\
	यदि (__w)						\
		__w = max(2UL, __w >> SCHED_FIXEDPOINT_सरणम्);	\
	__w;							\
})
#अन्यथा
# define NICE_0_आरोपय_सरणम्	(SCHED_FIXEDPOINT_सरणम्)
# define scale_आरोपय(w)		(w)
# define scale_आरोपय_अधस्तात्(w)	(w)
#यदि_अन्तः

/*
 * Task weight (visible to users) and its load (invisible to users) have
 * independent resolution, but they should be well calibrated. We use
 * scale_load() and scale_load_down(w) to convert between them. The
 * following must be true:
 *
 *  scale_load(sched_prio_to_weight[NICE_TO_PRIO(0)-MAX_RT_PRIO]) == NICE_0_LOAD
 *
 */
#परिभाषय NICE_0_आरोपय		(1L << NICE_0_आरोपय_सरणम्)

/*
 * Single value that decides SCHED_DEADLINE internal math precision.
 * 10 -> just above 1us
 * 9  -> just above 0.5us
 */
#परिभाषय DL_SCALE		10

/*
 * Single value that denotes runtime == period, ie unlimited time.
 */
#परिभाषय RUNTIME_INF		((u64)~0ULL)

स्थिर अन्तःस्थ पूर्णाङ्क idle_policy(पूर्णाङ्क policy)
{
	प्रत्यागम policy == SCHED_IDLE;
}

स्थिर अन्तःस्थ पूर्णाङ्क प्रसामान्यम्_policy(पूर्णाङ्क policy)
{
#यदि_परिभाषितम् CONFIG_SCHED_CLASS_EXT
	यदि (policy == SCHED_EXT)
		प्रत्यागम true;
#यदि_अन्तः
	प्रत्यागम policy == SCHED_प्रसामान्यम्;
}

स्थिर अन्तःस्थ पूर्णाङ्क fair_policy(पूर्णाङ्क policy)
{
	प्रत्यागम प्रसामान्यम्_policy(policy) || policy == SCHED_BATCH;
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

#परिभाषय cap_scale(v, s)		((v)*(s) >> SCHED_क्षमता_सरणम्)

स्थिर अन्तःस्थ शून्य update_avg(u64 *avg, u64 sample)
{
	s64 diff = sample - *avg;

	*avg += diff / 8;
}

/*
 * Shifting a value by an exponent greater *or equal* to the size of said value
 * is UB; cap at size-1.
 */
#परिभाषय shr_bound(val, सरणम्)							\
	(val >> min_t(typeof(सरणम्), सरणम्, द्वयङ्काः_प्रत्येकम्_TYPE(typeof(val)) - 1))

/*
 * cgroup weight knobs should use the common MIN, DFL and MAX values which are
 * 1, 100 and 10000 respectively. While it loses a bit of range on both ends, it
 * maps pretty well onto the shares value used by scheduler and the round-trip
 * conversions preserve the original value over the entire range.
 */
स्थिर अन्तःस्थ अचिह्नित दीर्घ sched_weight_from_cgroup(अचिह्नित दीर्घ cgrp_weight)
{
	प्रत्यागम DIV_आवर्तनम्_CLOSEST_ULL(cgrp_weight * 1024, CGROUP_WEIGHT_DFL);
}

स्थिर अन्तःस्थ अचिह्नित दीर्घ sched_weight_गन्तव्यम्_cgroup(अचिह्नित दीर्घ weight)
{
	प्रत्यागम clamp_t(अचिह्नित दीर्घ,
		       DIV_आवर्तनम्_CLOSEST_ULL(weight * CGROUP_WEIGHT_DFL, 1024),
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
#परिभाषय SCHED_FLAG_SUGOV	0x10000000

#परिभाषय SCHED_DL_FLAGS		(SCHED_FLAG_RECLAIM | SCHED_FLAG_DL_OVERRUN | SCHED_FLAG_SUGOV)

स्थिर अन्तःस्थ bool dl_entity_is_special(ध्रुव संरचना sched_dl_entity *dl_se)
{
#यदि_परिभाषितम् CONFIG_CPU_FREQ_GOV_SCHEDUTIL
	प्रत्यागम unlikely(dl_se->flags & SCHED_FLAG_SUGOV);
#अन्यथा
	प्रत्यागम false;
#यदि_अन्तः
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
	संरचना list_शीर्ष queue[MAX_RT_PRIO];
};

संरचना rt_bandwidth {
	/* nests inside the rq lock: */
	raw_spinlock_t		rt_runtime_तालकम्;
	ktime_t			rt_period;
	u64			rt_runtime;
	संरचना hrtimer		rt_period_timer;
	अचिह्नित पूर्णाङ्क		rt_period_सक्रिय;
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
	raw_spinlock_t		तालकम्;
	u64			bw;
	u64			योग_bw;
};

बाह्य शून्य init_dl_bw(संरचना dl_bw *dl_b);
बाह्य पूर्णाङ्क  sched_dl_global_validate(शून्य);
बाह्य शून्य sched_dl_do_global(शून्य);
बाह्य पूर्णाङ्क  sched_dl_अतिप्रवाहः(संरचना task_struct *p, पूर्णाङ्क policy, ध्रुव संरचना sched_attr *attr);
बाह्य शून्य __setparam_dl(संरचना task_struct *p, ध्रुव संरचना sched_attr *attr);
बाह्य शून्य __getparam_dl(संरचना task_struct *p, संरचना sched_attr *attr, अचिह्नित पूर्णाङ्क flags);
बाह्य bool __checkparam_dl(ध्रुव संरचना sched_attr *attr);
बाह्य bool dl_param_परिवर्तितम्(संरचना task_struct *p, ध्रुव संरचना sched_attr *attr);
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

स्थिर अन्तःस्थ bool dl_server_सक्रिय(संरचना sched_dl_entity *dl_se)
{
	प्रत्यागम dl_se->dl_server_सक्रिय;
}

#यदि_परिभाषितम् CONFIG_CGROUP_SCHED

बाह्य संरचना list_शीर्ष task_groups;

#यदि_परिभाषितम् CONFIG_गणः_SCHED_BANDWIDTH
बाह्य ध्रुव u64 max_bw_quota_period_us;

/*
 * default period for group bandwidth.
 * default: 0.1s, units: microseconds
 */
स्थिर अन्तःस्थ u64 default_bw_period_us(शून्य)
{
	प्रत्यागम 100000ULL;
}
#यदि_अन्तः /* CONFIG_GROUP_SCHED_BANDWIDTH */

संरचना cfs_bandwidth {
#यदि_परिभाषितम् CONFIG_CFS_BANDWIDTH
	raw_spinlock_t		तालकम्;
	ktime_t			period;
	u64			quota;
	u64			runtime;
	u64			burst;
	u64			runtime_snap;
	s64			श्रेणीबद्धम्_quota;

	u8			idle;
	u8			period_सक्रिय;
	u8			slack_started;
	संरचना hrtimer		period_timer;
	संरचना hrtimer		slack_timer;
	संरचना list_शीर्ष	throttled_cfs_rq;

	/* Statistics: */
	पूर्णाङ्क			nr_periods;
	पूर्णाङ्क			nr_throttled;
	पूर्णाङ्क			nr_burst;
	u64			throttled_time;
	u64			burst_time;
#यदि_अन्तः /* CONFIG_CFS_BANDWIDTH */
};

/* Task group related information */
संरचना task_गणः {
	संरचना cgroup_subsys_state css;

#यदि_परिभाषितम् CONFIG_गणः_SCHED_WEIGHT
	/* A positive value indicates that this is a SCHED_IDLE group. */
	पूर्णाङ्क			idle;
#यदि_अन्तः

#यदि_परिभाषितम् CONFIG_FAIR_गणः_SCHED
	/* runqueue "owned" by this group on each CPU */
	संरचना cfs_rq __percpu	*cfs_rq;
	अचिह्नित दीर्घ		shares;
	/*
	 * load_avg can be heavily contended at clock tick time, so put
	 * it in its own cache-line separated from the fields above which
	 * will also be accessed at each tick.
	 */
	atomic_long_t		आरोपय_avg ____cacheline_aligned;
	atomic_long_t		runnable_avg;

#यदि_अन्तः /* CONFIG_FAIR_GROUP_SCHED */

#यदि_परिभाषितम् CONFIG_RT_गणः_SCHED
	संरचना sched_rt_entity	**rt_se;
	संरचना rt_rq		**rt_rq;

	संरचना rt_bandwidth	rt_bandwidth;
#यदि_अन्तः

	संरचना scx_task_गणः	scx;

	संरचना rcu_शीर्ष		rcu;
	संरचना list_शीर्ष	list;

	संरचना task_गणः	*parent;
	संरचना list_शीर्ष	siblings;
	संरचना list_शीर्ष	children;

#यदि_परिभाषितम् CONFIG_SCHED_AUTOGROUP
	संरचना autogroup	*autogroup;
#यदि_अन्तः

	संरचना cfs_bandwidth	cfs_bandwidth;

#यदि_परिभाषितम् CONFIG_UCLAMP_TASK_गणः
	/* The two decimal precision [%] value requested from user-space */
	अचिह्नित पूर्णाङ्क		uclamp_pct[UCLAMP_CNT];
	/* Clamp values requested for a task group */
	संरचना uclamp_se	uclamp_req[UCLAMP_CNT];
	/* Effective clamp values used for a task group */
	संरचना uclamp_se	uclamp[UCLAMP_CNT];
#यदि_अन्तः

};

#परिभाषय मूलम्_TASK_गणः_आरोपय	NICE_0_आरोपय

प्रकारनिर्देश पूर्णाङ्क (*tg_visitor)(संरचना task_गणः *, शून्य *);

बाह्य पूर्णाङ्क walk_tg_वृक्षः_from(संरचना task_गणः *from,
			     tg_visitor अधस्तात्, tg_visitor ऊर्ध्वम्, शून्य *दत्त);

/*
 * Iterate the full tree, calling @down when first entering a node and @up when
 * leaving it for the final time.
 *
 * Caller must hold rcu_lock or sufficient equivalent.
 */
स्थिर अन्तःस्थ पूर्णाङ्क walk_tg_वृक्षः(tg_visitor अधस्तात्, tg_visitor ऊर्ध्वम्, शून्य *दत्त)
{
	प्रत्यागम walk_tg_वृक्षः_from(&मूलम्_task_गणः, अधस्तात्, ऊर्ध्वम्, दत्त);
}

स्थिर अन्तःस्थ संरचना task_गणः *css_tg(संरचना cgroup_subsys_state *css)
{
	प्रत्यागम css ? container_सम्बन्धि(css, संरचना task_गणः, css) : रिक्त;
}

बाह्य पूर्णाङ्क tg_nop(संरचना task_गणः *tg, शून्य *दत्त);

#यदि_परिभाषितम् CONFIG_FAIR_गणः_SCHED
बाह्य शून्य free_fair_sched_गणः(संरचना task_गणः *tg);
बाह्य पूर्णाङ्क alloc_fair_sched_गणः(संरचना task_गणः *tg, संरचना task_गणः *parent);
बाह्य शून्य online_fair_sched_गणः(संरचना task_गणः *tg);
बाह्य शून्य unregister_fair_sched_गणः(संरचना task_गणः *tg);
बाह्य शून्य __sched_cgroup_mode_update(पूर्णाङ्क बहुलकम्);
#अन्यथा /* !CONFIG_FAIR_GROUP_SCHED: */
स्थिर अन्तःस्थ शून्य free_fair_sched_गणः(संरचना task_गणः *tg) { }
स्थिर अन्तःस्थ पूर्णाङ्क alloc_fair_sched_गणः(संरचना task_गणः *tg, संरचना task_गणः *parent)
{
       प्रत्यागम 1;
}
स्थिर अन्तःस्थ शून्य online_fair_sched_गणः(संरचना task_गणः *tg) { }
स्थिर अन्तःस्थ शून्य unregister_fair_sched_गणः(संरचना task_गणः *tg) { }
#यदि_अन्तः /* !CONFIG_FAIR_GROUP_SCHED */

बाह्य शून्य init_tg_cfs_प्रविष्टिः(संरचना task_गणः *tg, संरचना cfs_rq *cfs_rq,
			संरचना sched_entity *se, पूर्णाङ्क cpu,
			संरचना sched_entity *parent);
बाह्य शून्य init_cfs_bandwidth(संरचना cfs_bandwidth *cfs_b, संरचना cfs_bandwidth *parent);

बाह्य शून्य __refill_cfs_bandwidth_runtime(संरचना cfs_bandwidth *cfs_b);
बाह्य शून्य start_cfs_bandwidth(संरचना cfs_bandwidth *cfs_b);
बाह्य शून्य unthrottle_cfs_rq(संरचना cfs_rq *cfs_rq);
बाह्य bool cfs_task_bw_constrained(संरचना task_struct *p);

बाह्य शून्य init_tg_rt_प्रविष्टिः(संरचना task_गणः *tg, संरचना rt_rq *rt_rq,
		संरचना sched_rt_entity *rt_se, पूर्णाङ्क cpu,
		संरचना sched_rt_entity *parent);
बाह्य पूर्णाङ्क sched_गणः_समुच्चयः_rt_runtime(संरचना task_गणः *tg, दीर्घ rt_runtime_us);
बाह्य पूर्णाङ्क sched_गणः_समुच्चयः_rt_period(संरचना task_गणः *tg, u64 rt_period_us);
बाह्य दीर्घ sched_गणः_rt_runtime(संरचना task_गणः *tg);
बाह्य दीर्घ sched_गणः_rt_period(संरचना task_गणः *tg);
बाह्य पूर्णाङ्क sched_rt_can_attach(संरचना task_गणः *tg, संरचना task_struct *tsk);

बाह्य संरचना task_गणः *sched_create_गणः(संरचना task_गणः *parent);
बाह्य शून्य sched_online_गणः(संरचना task_गणः *tg,
			       संरचना task_गणः *parent);
बाह्य शून्य sched_destroy_गणः(संरचना task_गणः *tg);
बाह्य शून्य sched_release_गणः(संरचना task_गणः *tg);

बाह्य शून्य sched_move_task(संरचना task_struct *tsk, bool for_autogroup);

#यदि_परिभाषितम् CONFIG_FAIR_गणः_SCHED
बाह्य पूर्णाङ्क sched_गणः_समुच्चयः_shares(संरचना task_गणः *tg, अचिह्नित दीर्घ shares);

बाह्य पूर्णाङ्क sched_गणः_समुच्चयः_idle(संरचना task_गणः *tg, दीर्घ idle);

बाह्य शून्य समुच्चयः_task_rq_fair(संरचना sched_entity *se,
			     संरचना cfs_rq *पूर्व, संरचना cfs_rq *अग्र);
#अन्यथा /* !CONFIG_FAIR_GROUP_SCHED: */
स्थिर अन्तःस्थ पूर्णाङ्क sched_गणः_समुच्चयः_shares(संरचना task_गणः *tg, अचिह्नित दीर्घ shares) { प्रत्यागम 0; }
स्थिर अन्तःस्थ पूर्णाङ्क sched_गणः_समुच्चयः_idle(संरचना task_गणः *tg, दीर्घ idle) { प्रत्यागम 0; }
#यदि_अन्तः /* !CONFIG_FAIR_GROUP_SCHED */

#अन्यथा /* !CONFIG_CGROUP_SCHED: */

संरचना cfs_bandwidth { };

स्थिर अन्तःस्थ bool cfs_task_bw_constrained(संरचना task_struct *p) { प्रत्यागम false; }

#यदि_अन्तः /* !CONFIG_CGROUP_SCHED */

/*
 * A weight of 0 or 1 can cause arithmetics problems.
 * A weight of a cfs_rq is the sum of weights of which entities
 * are queued on this cfs_rq, so a weight of a entity should not be
 * too large, so as the shares value of a task group.
 * (The default weight is 1024 - so there's no practical
 *  limitation from this.)
 */
#परिभाषय MIN_SHARES		(1UL <<  1)
#परिभाषय MAX_SHARES		(1UL << 18)

बाह्य शून्य unregister_rt_sched_गणः(संरचना task_गणः *tg);
बाह्य शून्य free_rt_sched_गणः(संरचना task_गणः *tg);
बाह्य पूर्णाङ्क alloc_rt_sched_गणः(संरचना task_गणः *tg, संरचना task_गणः *parent);

/*
 * u64_u32_load/u64_u32_store
 *
 * Use a copy of a u64 value to protect against data race. This is only
 * applicable for 32-bits architectures.
 */
#यदि_परिभाषितम् CONFIG_64BIT
# define u64_u32_आरोपय_copy(var, copy)		var
# define u64_u32_store_copy(var, copy, val)	(var = val)
#अन्यथा
# define u64_u32_आरोपय_copy(var, copy)					\
({									\
	u64 __val, __val_copy;						\
	कुरु {								\
		__val_copy = copy;					\
		/*							\
		 * paired with u64_u32_store_copy(), ordering access	\
		 * to var and copy.					\
		 */							\
		smp_rmb();						\
		__val = var;						\
	} यावत् (__val != __val_copy);					\
	__val;								\
})
# define u64_u32_store_copy(var, copy, val)				\
कुरु {									\
	typeof(val) __val = (val);					\
	var = __val;							\
	/*								\
	 * paired with u64_u32_load_copy(), ordering access to var and	\
	 * copy.							\
	 */								\
	smp_wmb();							\
	copy = __val;							\
} यावत् (0)
#यदि_अन्तः
# define u64_u32_आरोपय(var)		u64_u32_आरोपय_copy(var, var##_copy)
# define u64_u32_store(var, val)	u64_u32_store_copy(var, var##_copy, val)

संरचना balance_callback {
	संरचना balance_callback *अग्र;
	शून्य (*func)(संरचना rq *rq);
};

/* Fair scheduling SCHED_{NORMAL,BATCH,IDLE} related fields in a runqueue: */
संरचना cfs_rq {
	संरचना आरोपय_weight	आरोपय;
	अचिह्नित पूर्णाङ्क		nr_queued;
	अचिह्नित पूर्णाङ्क		h_nr_queued;		/* SCHED_{NORMAL,BATCH,IDLE} */
	अचिह्नित पूर्णाङ्क		h_nr_runnable;		/* SCHED_{NORMAL,BATCH,IDLE} */
	अचिह्नित पूर्णाङ्क		h_nr_idle;		/* SCHED_IDLE */

	s64			योग_w_vruntime;
	u64			योग_weight;
	u64			zero_vruntime;
	अचिह्नित पूर्णाङ्क		योग_सरणम्;

#यदि_परिभाषितम् CONFIG_SCHED_CORE
	अचिह्नित पूर्णाङ्क		forceidle_seq;
	u64			zero_vruntime_fi;
#यदि_अन्तः

	संरचना rb_मूलम्_cached	tasks_timeline;

	/*
	 * 'curr' points to the currently running entity on this cfs_rq.
	 * It is set to NULL otherwise (i.e when none are currently running).
	 */
	संरचना sched_entity	*curr;
	संरचना sched_entity	*अग्र;

	/*
	 * CFS load tracking
	 */
	संरचना sched_entity	*h_curr;
	संरचना sched_avg	avg;
#यदि_अपरिभाषितम् CONFIG_64BIT
	u64			चरमः_update_time_copy;
#यदि_अन्तः
	संरचना {
		raw_spinlock_t	तालकम् ____cacheline_aligned;
		पूर्णाङ्क		nr;
		अचिह्नित दीर्घ	आरोपय_avg;
		अचिह्नित दीर्घ	util_avg;
		अचिह्नित दीर्घ	runnable_avg;
	} removed;

#यदि_परिभाषितम् CONFIG_FAIR_गणः_SCHED
	u64			चरमः_update_tg_आरोपय_avg;
	अचिह्नित दीर्घ		tg_आरोपय_avg_contrib;
	अचिह्नित दीर्घ		tg_runnable_avg_contrib;
	दीर्घ			propagate;
	दीर्घ			prop_runnable_योग;

	/*
	 *   h_load = weight * f(tg)
	 *
	 * Where f(tg) is the recursive weight fraction assigned to
	 * this group.
	 */
	अचिह्नित दीर्घ		h_आरोपय;
	u64			चरमः_h_आरोपय_update;
	संरचना sched_entity	*h_आरोपय_अग्र;

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
	संरचना list_शीर्ष	पर्णम्_cfs_rq_list;
	संरचना task_गणः	*tg;	/* Group that "owns" this runqueue */

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
	संरचना list_शीर्ष	throttled_list;
	संरचना list_शीर्ष	throttled_csd_list;
	संरचना list_शीर्ष        throttled_limbo_list;
# endif /* CONFIG_CFS_BANDWIDTH */
#यदि_अन्तः /* CONFIG_FAIR_GROUP_SCHED */
};

#यदि_परिभाषितम् CONFIG_SCHED_CLASS_EXT
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

	SCX_RQ_अन्तर्_WAKEUP	= 1 << 16,
	SCX_RQ_अन्तर्_DISPATCH	= 1 << 17,
};

/* per-rq rescue execution state, see scx_rescue_timerfn() */
संरचना scx_rq_rescue {
	संरचना scx_dispatch_q	dsq;			/* stranded tasks awaiting rescue */
	s64			budget;			/* execution token bucket, ns */
	u64			clock;			/* last budget accrual timestamp */
	संरचना task_struct	*curr;			/* task being rescued, one at a time */
	s64			खण्डिका;			/* curr's admitted slice */
	u64			exec_snap;		/* sum_exec_runtime at admission */
	संरचना timer_list	timer;			/* paces admission and escalation */
	u64			kill_at;		/* last ejection, init before any */
};

संरचना scx_rq {
	संरचना scx_dispatch_q	local_dsq;
#यदि_परिभाषितम् CONFIG_EXT_SUB_SCHED
	संरचना scx_dispatch_q	reject_dsq;		/* staging for cap-rejected tasks */
	संरचना scx_rq_rescue	rescue;
#यदि_अन्तः
	संरचना list_शीर्ष	runnable_list;		/* runnable tasks on this rq */
	संरचना list_शीर्ष	ddsp_deferred_locals;	/* deferred ddsps from enq */
	/* both stashed across the activate_task() in move_remote_task_to_local_dsq() */
	u64			remote_activate_enq_flags;
	संरचना scx_sched	*remote_activate_sch;
	u32			nr_running;
	u32			cpuperf_target;		/* [0, SCHED_CAPACITY_SCALE] */
	bool			अन्तर्_select_cpu;
	bool			cpu_released;
	u32			flags;
	u32			nr_immed;		/* ENQ_IMMED tasks on local_dsq */
#यदि_परिभाषितम् CONFIG_SCHED_CORE
	u32			तालकम्_drop_seq;	/* nr dispatch lock releases */
#यदि_अन्तः
	u64			clock;			/* current per-rq clock -- see scx_bpf_now() */
#यदि_परिभाषितम् CONFIG_EXT_SUB_SCHED
	संरचना llist_शीर्ष	ecaps_गन्तव्यम्_sync;		/* pending ecaps syncs */
	संरचना task_struct	*sub_dispatch_पूर्व;
#यदि_अन्तः
	cpumask_var_t		cpus_गन्तव्यम्_sync;
	bool			kick_sync_pending;
	अचिह्नित दीर्घ		kick_sync;

	संरचना list_शीर्ष	sched_pcpus_गन्तव्यम्_kick;	/* see kick_cpus_irq_workfn() */

	raw_spinlock_t		deferred_reenq_तालकम्;
	संरचना list_शीर्ष	deferred_reenq_locals;	/* scheds requesting reenq of local DSQ */
	संरचना list_शीर्ष	deferred_reenq_users;	/* user DSQs requesting reenq */
	संरचना balance_callback	deferred_bal_cb;
	संरचना balance_callback	kick_sync_bal_cb;
	संरचना irq_work		deferred_irq_work;
	संरचना irq_work		kick_cpus_irq_work;
};
#यदि_अन्तः /* CONFIG_SCHED_CLASS_EXT */

स्थिर अन्तःस्थ पूर्णाङ्क rt_bandwidth_enabled(शून्य)
{
	प्रत्यागम sysctl_sched_rt_runtime >= 0;
}

/* RT IPI pull logic requires IRQ_WORK */
#यदि defined(CONFIG_IRQ_WORK) && defined(CONFIG_SMP)
# define HAVE_RT_प्रक्षिप_IPI
#यदि_अन्तः

/* Real-Time classes' related field in a runqueue: */
संरचना rt_rq {
	संरचना rt_prio_array	सक्रिय;
	अचिह्नित पूर्णाङ्क		rt_nr_running;
	अचिह्नित पूर्णाङ्क		rr_nr_running;
	संरचना {
		पूर्णाङ्क		curr; /* highest queued rt task prio */
		पूर्णाङ्क		अग्र; /* next highest */
	} highest_prio;
	bool			overloaded;
	संरचना plist_शीर्ष	pushable_tasks;

	पूर्णाङ्क			rt_queued;

#यदि_परिभाषितम् CONFIG_RT_गणः_SCHED
	पूर्णाङ्क			rt_throttled;
	u64			rt_time; /* consumed RT time, goes up in update_curr_rt */
	u64			rt_runtime; /* allotted RT time, "slice" from rt_bandwidth, RT sharing/balancing */
	/* Nests inside the rq lock: */
	raw_spinlock_t		rt_runtime_तालकम्;

	अचिह्नित पूर्णाङ्क		rt_nr_boosted;

	संरचना rq		*rq; /* this is always top-level rq, cache? */
#यदि_अन्तः
#यदि_परिभाषितम् CONFIG_CGROUP_SCHED
	संरचना task_गणः	*tg; /* this tg has "this" rt_rq on given CPU for runnable entities */
#यदि_अन्तः
};

स्थिर अन्तःस्थ bool rt_rq_is_runnable(संरचना rt_rq *rt_rq)
{
	प्रत्यागम rt_rq->rt_queued && rt_rq->rt_nr_running;
}

/* Deadline class' related fields in a runqueue */
संरचना dl_rq {
	/* runqueue is an rbtree, ordered by deadline */
	संरचना rb_मूलम्_cached	मूलम्;

	अचिह्नित पूर्णाङ्क		dl_nr_running;

	/*
	 * Deadline values of the currently executing and the
	 * earliest ready task on this rq. Caching these facilitates
	 * the decision whether or not a ready but not running task
	 * should migrate somewhere else.
	 */
	संरचना {
		u64		curr;
		u64		अग्र;
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
	u64			bw_अनुपातः;
};

#यदि_परिभाषितम् CONFIG_FAIR_गणः_SCHED
/* Check whether a task group is root tg */
#परिभाषय is_मूलम्_task_गणः(tg) ((tg) == &मूलम्_task_गणः)
/* An entity is a task if it doesn't "own" a runqueue */
#परिभाषय entity_is_task(se)	(!se->my_q)

स्थिर अन्तःस्थ शून्य se_update_runnable(संरचना sched_entity *se)
{
	यदि (!entity_is_task(se))
		se->runnable_weight = se->my_q->h_nr_runnable;
}

स्थिर अन्तःस्थ दीर्घ se_runnable(संरचना sched_entity *se)
{
	यदि (se->sched_delayed)
		प्रत्यागम false;

	यदि (entity_is_task(se))
		प्रत्यागम !!se->on_rq;
	अन्यथा
		प्रत्यागम se->runnable_weight;
}

#अन्यथा /* !CONFIG_FAIR_GROUP_SCHED: */

#परिभाषय entity_is_task(se)	1

स्थिर अन्तःस्थ शून्य se_update_runnable(संरचना sched_entity *se) { }

स्थिर अन्तःस्थ दीर्घ se_runnable(संरचना sched_entity *se)
{
	यदि (se->sched_delayed)
		प्रत्यागम false;

	प्रत्यागम !!se->on_rq;
}

#यदि_अन्तः /* !CONFIG_FAIR_GROUP_SCHED */

/*
 * XXX we want to get rid of these helpers and use the full load resolution.
 */
स्थिर अन्तःस्थ दीर्घ se_weight(संरचना sched_entity *se)
{
	प्रत्यागम scale_आरोपय_अधस्तात्(se->आरोपय.weight);
}


स्थिर अन्तःस्थ bool sched_asym_prefer(पूर्णाङ्क a, पूर्णाङ्क b)
{
	प्रत्यागम arch_asym_cpu_प्राथमिकता(a) > arch_asym_cpu_प्राथमिकता(b);
}

संरचना perf_domain {
	संरचना em_perf_domain *em_pd;
	संरचना perf_domain *अग्र;
	संरचना rcu_शीर्ष rcu;
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
	संरचना rcu_शीर्ष		rcu;
	cpumask_var_t		विस्तृतिः;
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
	cpumask_var_t		dlo_आवरणम्;
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
	u64 दर्शय_cookie;

#यदि_परिभाषितम् HAVE_RT_प्रक्षिप_IPI
	/*
	 * For IPI pull requests, loop across the rto_mask.
	 */
	संरचना irq_work		rto_प्रक्षिप_work;
	raw_spinlock_t		rto_तालकम्;
	/* These are only updated and read within rto_lock */
	पूर्णाङ्क			rto_loop;
	पूर्णाङ्क			rto_cpu;
	/* These atomics are updated outside of a lock */
	atomic_t		rto_loop_अग्र;
	atomic_t		rto_loop_start;
#यदि_अन्तः /* HAVE_RT_PUSH_IPI */
	/*
	 * The "RT overload" flag: it gets set if a CPU has more than
	 * one runnable RT task.
	 */
	cpumask_var_t		rto_आवरणम्;
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

स्थिर अन्तःस्थ शून्य समुच्चयः_rd_overloaded(संरचना मूलम्_domain *rd, पूर्णाङ्क स्थितिः)
{
	यदि (get_rd_overloaded(rd) != स्थितिः)
		WRITE_ONCE(rd->overloaded, स्थितिः);
}

#यदि_परिभाषितम् HAVE_RT_प्रक्षिप_IPI
बाह्य शून्य rto_प्रक्षिप_irq_work_func(संरचना irq_work *work);
#यदि_अन्तः

#यदि_परिभाषितम् CONFIG_UCLAMP_TASK
/*
 * struct uclamp_bucket - Utilization clamp bucket
 * @value: utilization clamp value for tasks on this clamp bucket
 * @tasks: number of RUNNABLE tasks on this clamp bucket
 *
 * Keep track of how many tasks are RUNNABLE for a given utilization
 * clamp value.
 */
संरचना uclamp_bucket {
	अचिह्नित दीर्घ मानम् : द्वयङ्काः_प्रत्येकम्(SCHED_क्षमता_SCALE);
	अचिह्नित दीर्घ tasks : द्वयङ्काः_प्रत्येकम्_LONG - द्वयङ्काः_प्रत्येकम्(SCHED_क्षमता_SCALE);
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
	अचिह्नित पूर्णाङ्क मानम्;
	संरचना uclamp_bucket bucket[UCLAMP_BUCKETS];
};

DECLARE_STATIC_कुञ्चिका_FALSE(sched_uclamp_प्रयुक्तम्);
#यदि_अन्तः /* CONFIG_UCLAMP_TASK */

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
	अचिह्नित पूर्णाङ्क		nr_running;
#यदि_परिभाषितम् CONFIG_NUMA_BALANCING
	अचिह्नित पूर्णाङ्क		nr_numa_running;
	अचिह्नित पूर्णाङ्क		nr_preferred_running;
#यदि_अन्तः
	अचिह्नित पूर्णाङ्क		ttwu_pending;
	अचिह्नित दीर्घ		cpu_क्षमता;
#यदि_परिभाषितम् CONFIG_SCHED_PROXY_EXEC
	संरचना task_struct __rcu	*donor;  /* Scheduling context */
	संरचना task_struct __rcu	*curr;   /* Execution context */
#अन्यथा
	सङ्घ {
		संरचना task_struct __rcu *donor; /* Scheduler context */
		संरचना task_struct __rcu *curr;  /* Execution context */
	};
#यदि_अन्तः
	संरचना task_struct	*idle;
	/* padding left here deliberately */

	/*
	 * The next cacheline holds the (hot) runqueue lock, as well as
	 * some other less performance-critical fields.
	 */
	u64			nr_switches	____cacheline_aligned;

	/* runqueue lock: */
	raw_spinlock_t		__lock;

#यदि_परिभाषितम् CONFIG_NO_HZ_COMMON
	अचिह्नित पूर्णाङ्क		nohz_tick_stopped;
	atomic_t		nohz_flags;
	अचिह्नित पूर्णाङ्क		has_blocked_आरोपय;
	अचिह्नित दीर्घ		चरमः_blocked_आरोपय_update_tick;
	call_single_दत्त_t	nohz_csd;
#यदि_अन्तः /* CONFIG_NO_HZ_COMMON */

#यदि_परिभाषितम् CONFIG_UCLAMP_TASK
	/* Utilization clamp values based on CPU's RUNNABLE tasks */
	संरचना uclamp_rq	uclamp[UCLAMP_CNT] ____cacheline_aligned;
	अचिह्नित पूर्णाङ्क		uclamp_flags;
#परिभाषय UCLAMP_FLAG_IDLE 0x01
#यदि_अन्तः

	संरचना cfs_rq		cfs;
	संरचना rt_rq		rt;
	संरचना dl_rq		dl;
#यदि_परिभाषितम् CONFIG_SCHED_CLASS_EXT
	संरचना scx_rq		scx;
	संरचना sched_dl_entity	ext_server;
#यदि_अन्तः
#यदि_परिभाषितम् CONFIG_SCHED_CACHE
	raw_spinlock_t		cpu_epoch_तालकम् ____cacheline_aligned;
	u64			cpu_runtime;
	अचिह्नित दीर्घ		cpu_epoch;
	अचिह्नित दीर्घ		cpu_epoch_अग्र;
#यदि_अन्तः

	संरचना sched_dl_entity	fair_server;

#यदि_परिभाषितम् CONFIG_FAIR_गणः_SCHED
	/* list of leaf cfs_rq on this CPU: */
	संरचना list_शीर्ष	पर्णम्_cfs_rq_list;
	संरचना list_शीर्ष	*tmp_alone_branch;
#यदि_अन्तः /* CONFIG_FAIR_GROUP_SCHED */

#यदि_परिभाषितम् CONFIG_NUMA_BALANCING
	अचिह्नित पूर्णाङ्क		numa_migrate_on;
#यदि_अन्तः

#यदि_परिभाषितम् CONFIG_SCHED_CACHE
	अचिह्नित पूर्णाङ्क		nr_pref_llc_running;
	अचिह्नित पूर्णाङ्क		nr_llc_running;
#यदि_अन्तः

	/*
	 * This is part of a global counter where only the total sum
	 * over all CPUs matters. A task can increase this counter on
	 * one CPU and if it got migrated afterwards it may decrease
	 * it on another CPU. Always updated under the runqueue lock:
	 */
	अचिह्नित दीर्घ		nr_uninterruptible;

	संरचना sched_dl_entity	*dl_server;
	संरचना task_struct	*stop;
	ध्रुव संरचना sched_class *अग्र_class;
	अचिह्नित दीर्घ		अग्र_balance;
	संरचना mm_struct	*पूर्व_mm;

	/*
	 * The following fields of clock data are frequently referenced
	 * and updated together, and should go on their own cache line.
	 */
	u64			clock_task ____cacheline_aligned;
	u64			clock_pelt;
	u64			clock;
	अचिह्नित दीर्घ		नष्टम्_idle_time;
	अचिह्नित पूर्णाङ्क		clock_update_flags;
	u64			clock_pelt_idle;
	u64			clock_idle;

#यदि_अपरिभाषितम् CONFIG_64BIT
	u64			clock_pelt_idle_copy;
	u64			clock_idle_copy;
#यदि_अन्तः

	u64 चरमः_आलोकितम्_need_resched_ns;
	पूर्णाङ्क ticks_without_resched;

#यदि_परिभाषितम् CONFIG_MEMBARRIER
	पूर्णाङ्क membarrier_state;
#यदि_अन्तः

	संरचना मूलम्_domain		*rd;
	संरचना sched_domain __rcu	*sd;

	संरचना balance_callback *balance_callback;

	अचिह्नित वर्ण		nohz_idle_balance;
	अचिह्नित वर्ण		idle_balance;

	अचिह्नित दीर्घ		misfit_task_आरोपय;

	/* For active balancing */
	पूर्णाङ्क			सक्रिय_balance;
	पूर्णाङ्क			प्रक्षिप_cpu;
	संरचना cpu_stop_work	सक्रिय_balance_work;

	/* CPU of this runqueue: */
	पूर्णाङ्क			cpu;
	पूर्णाङ्क			online;

	संरचना list_शीर्ष cfs_tasks;

	संरचना sched_avg	avg_rt;
	संरचना sched_avg	avg_dl;
#यदि_परिभाषितम् CONFIG_HAVE_SCHED_AVG_IRQ
	संरचना sched_avg	avg_irq;
#यदि_अन्तः
#यदि_परिभाषितम् CONFIG_SCHED_HW_PRESSURE
	संरचना sched_avg	avg_hw;
#यदि_अन्तः
	u64			idle_stamp;
	u64			avg_idle;

	/* This is used to determine avg_idle's max value */
	u64			max_idle_balance_cost;

#यदि_परिभाषितम् CONFIG_HOTPLUG_CPU
	संरचना rcuwait		hotplug_wait;
#यदि_अन्तः

#यदि_परिभाषितम् CONFIG_IRQ_TIME_ACCOUNTING
	u64			पूर्व_irq_time;
	u64			psi_irq_time;
#यदि_अन्तः
#यदि_परिभाषितम् CONFIG_PARAVIRT
	u64			पूर्व_steal_time;
#यदि_अन्तः
#यदि_परिभाषितम् CONFIG_PARAVIRT_TIME_ACCOUNTING
	u64			पूर्व_steal_time_rq;
#यदि_अन्तः
#यदि_परिभाषितम् CONFIG_PREFERRED_CPU
	bool			npc_प्रक्षिप_work_pending;
#यदि_अन्तः

	/* calc_load related fields */
	अचिह्नित दीर्घ		calc_आरोपय_update;
	दीर्घ			calc_आरोपय_सक्रिय;

#यदि_परिभाषितम् CONFIG_SCHED_HRTICK
	call_single_दत्त_t	hrtick_csd;
	संरचना hrtimer		hrtick_timer;
	ktime_t			hrtick_time;
	ktime_t			hrtick_delay;
	अचिह्नित पूर्णाङ्क		hrtick_sched;
#यदि_अन्तः

#यदि_परिभाषितम् CONFIG_SCHEDSTATS
	/* latency stats */
	संरचना sched_info	rq_sched_info;
	अचिह्नित दीर्घ दीर्घ	rq_cpu_time;

	/* sys_sched_yield() stats */
	अचिह्नित पूर्णाङ्क		yld_गणनम्;

	/* schedule() stats */
	अचिह्नित पूर्णाङ्क		sched_गणनम्;
	अचिह्नित पूर्णाङ्क		sched_goidle;

	/* try_to_wake_up() stats */
	अचिह्नित पूर्णाङ्क		ttwu_गणनम्;
	अचिह्नित पूर्णाङ्क		ttwu_local;
#यदि_अन्तः

#यदि_परिभाषितम् CONFIG_CPU_IDLE
	/* Must be inspected within a RCU lock section */
	संरचना cpuidle_state	*idle_state;
#यदि_अन्तः

	अचिह्नित पूर्णाङ्क		nr_pinned;
	अचिह्नित पूर्णाङ्क		प्रक्षिप_busy;
	संरचना cpu_stop_work	प्रक्षिप_work;

#यदि_परिभाषितम् CONFIG_SCHED_CORE
	/* per rq */
	संरचना rq		*core;
	संरचना task_struct	*core_pick;
	संरचना sched_dl_entity	*core_dl_server;
	अचिह्नित पूर्णाङ्क		core_enabled;
	संरचना rb_मूलम्		core_वृक्षः;

	/* shared state -- careful with sched_core_cpu_deactivate() */
	अचिह्नित पूर्णाङ्क		core_task_seq;
	अचिह्नित पूर्णाङ्क		core_pick_seq;
	अचिह्नित दीर्घ		core_cookie;
	अचिह्नित पूर्णाङ्क		core_forceidle_गणनम्;
	अचिह्नित पूर्णाङ्क		core_forceidle_seq;
	अचिह्नित पूर्णाङ्क		core_forceidle_occupation;
	u64			core_forceidle_start;
	अचिह्नित पूर्णाङ्क		core_pick_अन्तर्_flight;
#यदि_अन्तः /* CONFIG_SCHED_CORE */

	/* Scratch cpumask to be temporarily used under rq_lock */
	cpumask_var_t		scratch_आवरणम्;

#यदि_परिभाषितम् CONFIG_CFS_BANDWIDTH
	call_single_दत्त_t	cfsb_csd;
	संरचना list_शीर्ष	cfsb_csd_list;
#यदि_अन्तः

	atomic_t		nr_iowait;
} __no_randomize_layout;

#यदि_परिभाषितम् CONFIG_FAIR_गणः_SCHED

/* CPU runqueue to which this cfs_rq is attached */
स्थिर अन्तःस्थ संरचना rq *rq_सम्बन्धि(संरचना cfs_rq *cfs_rq)
{
	प्रत्यागम cfs_rq->rq;
}

#अन्यथा /* !CONFIG_FAIR_GROUP_SCHED: */

स्थिर अन्तःस्थ संरचना rq *rq_सम्बन्धि(संरचना cfs_rq *cfs_rq)
{
	प्रत्यागम container_सम्बन्धि(cfs_rq, संरचना rq, cfs);
}
#यदि_अन्तः /* !CONFIG_FAIR_GROUP_SCHED */

स्थिर अन्तःस्थ पूर्णाङ्क cpu_सम्बन्धि(संरचना rq *rq)
{
	प्रत्यागम rq->cpu;
}

#परिभाषय MDF_प्रक्षिप		0x01

स्थिर अन्तःस्थ bool is_migration_disabled(संरचना task_struct *p)
{
	प्रत्यागम p->migration_disabled;
}

DECLARE_प्रत्येकम्_CPU_SHARED_ALIGNED(संरचना rq, runqueues);
DECLARE_प्रत्येकम्_CPU(संरचना rnd_state, sched_rnd_state);

स्थिर अन्तःस्थ u32 sched_rng(शून्य)
{
	प्रत्यागम prandom_u32_state(this_cpu_ptr(&sched_rnd_state));
}

स्थिर __always_inline संरचना rq *__this_rq(शून्य)
{
	प्रत्यागम this_cpu_ptr(&runqueues);
}

#परिभाषय cpu_rq(cpu)		(&प्रत्येकम्_cpu(runqueues, (cpu)))
#परिभाषय this_rq()		__this_rq()
#परिभाषय task_rq(p)		cpu_rq(task_cpu(p))
#परिभाषय cpu_curr(cpu)		(cpu_rq(cpu)->curr)
#परिभाषय raw_rq()		raw_cpu_ptr(&runqueues)

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
स्थिर अन्तःस्थ bool उपलब्धम्_idle_cpu(पूर्णाङ्क cpu)
{
	यदि (!idle_rq(cpu_rq(cpu)))
		प्रत्यागम 0;

	यदि (vcpu_is_preempted(cpu))
		प्रत्यागम 0;

	प्रत्यागम 1;
}

#यदि_परिभाषितम् CONFIG_SCHED_PROXY_EXEC
स्थिर अन्तःस्थ शून्य rq_समुच्चयः_donor(संरचना rq *rq, संरचना task_struct *t)
{
	rcu_नियोजय_pointer(rq->donor, t);
}
#अन्यथा
स्थिर अन्तःस्थ शून्य rq_समुच्चयः_donor(संरचना rq *rq, संरचना task_struct *t)
{
	/* Do nothing */
}
#यदि_अन्तः

#यदि_परिभाषितम् CONFIG_SCHED_CORE
स्थिर अन्तःस्थ संरचना cpumask *sched_गणः_विस्तृतिः(संरचना sched_गणः *sg);

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
	यदि (sched_core_enabled(rq))
		प्रत्यागम &rq->core->__lock;

	प्रत्यागम &rq->__lock;
}

स्थिर अन्तःस्थ raw_spinlock_t *__rq_lockp(संरचना rq *rq)
	__returns_ctx_lock(rq_lockp(rq)) /* alias them */
{
	यदि (rq->core_enabled)
		प्रत्यागम &rq->core->__lock;

	प्रत्यागम &rq->__lock;
}

बाह्य bool
cfs_prio_less(ध्रुव संरचना task_struct *a, ध्रुव संरचना task_struct *b, bool fi);

बाह्य शून्य task_vruntime_update(संरचना rq *rq, संरचना task_struct *p, bool अन्तर्_fi);

/*
 * Helpers to check if the CPU's core cookie matches with the task's cookie
 * when core scheduling is enabled.
 * A special case is that the task's cookie always matches with CPU's core
 * cookie if the CPU is in an idle core.
 */
स्थिर अन्तःस्थ bool sched_cpu_cookie_मेलनम्(संरचना rq *rq, संरचना task_struct *p)
{
	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	यदि (!sched_core_enabled(rq))
		प्रत्यागम true;

	प्रत्यागम rq->core->core_cookie == p->core_cookie;
}

स्थिर अन्तःस्थ bool sched_core_cookie_मेलनम्(संरचना rq *rq, संरचना task_struct *p)
{
	bool idle_core = true;
	पूर्णाङ्क cpu;

	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	यदि (!sched_core_enabled(rq))
		प्रत्यागम true;

	यदि (rq->core->core_cookie == p->core_cookie)
		प्रत्यागम true;

	for_each_cpu(cpu, cpu_smt_आवरणम्(cpu_सम्बन्धि(rq))) {
		यदि (!उपलब्धम्_idle_cpu(cpu)) {
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

स्थिर अन्तःस्थ bool sched_गणः_cookie_मेलनम्(संरचना rq *rq,
					    संरचना task_struct *p,
					    संरचना sched_गणः *गणः)
{
	पूर्णाङ्क cpu;

	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	यदि (!sched_core_enabled(rq))
		प्रत्यागम true;

	for_each_cpu_and(cpu, sched_गणः_विस्तृतिः(गणः), p->cpus_ptr) {
		यदि (sched_core_cookie_मेलनम्(cpu_rq(cpu), p))
			प्रत्यागम true;
	}
	प्रत्यागम false;
}

स्थिर अन्तःस्थ bool sched_core_enqueued(संरचना task_struct *p)
{
	प्रत्यागम !RB_रिक्तानि_ग्रन्थिः(&p->core_ग्रन्थिः);
}

बाह्य शून्य sched_core_पङ्क्तौ_निक्षिप(संरचना rq *rq, संरचना task_struct *p);
बाह्य शून्य sched_core_पङ्क्तेः_निष्कासय(संरचना rq *rq, संरचना task_struct *p, पूर्णाङ्क flags);

बाह्य शून्य sched_core_get(शून्य);
बाह्य शून्य sched_core_put(शून्य);

स्थिर अन्तःस्थ bool task_has_sched_core(संरचना task_struct *p)
{
	यदि (sched_core_disabled())
		प्रत्यागम false;

	प्रत्यागम !!p->core_cookie;
}

#अन्यथा /* !CONFIG_SCHED_CORE: */

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

स्थिर अन्तःस्थ bool sched_cpu_cookie_मेलनम्(संरचना rq *rq, संरचना task_struct *p)
{
	प्रत्यागम true;
}

स्थिर अन्तःस्थ bool sched_core_cookie_मेलनम्(संरचना rq *rq, संरचना task_struct *p)
{
	प्रत्यागम true;
}

स्थिर अन्तःस्थ bool sched_गणः_cookie_मेलनम्(संरचना rq *rq,
					    संरचना task_struct *p,
					    संरचना sched_गणः *गणः)
{
	प्रत्यागम true;
}

स्थिर अन्तःस्थ bool task_has_sched_core(संरचना task_struct *p)
{
	प्रत्यागम false;
}

#यदि_अन्तः /* !CONFIG_SCHED_CORE */

#यदि_परिभाषितम् CONFIG_RT_गणः_SCHED
# ifdef CONFIG_RT_गणः_SCHED_DEFAULT_DISABLED
DECLARE_STATIC_कुञ्चिका_FALSE(rt_गणः_sched);
स्थिर अन्तःस्थ bool rt_गणः_sched_enabled(शून्य)
{
	प्रत्यागम static_branch_unlikely(&rt_गणः_sched);
}
# अन्यथा /* !CONFIG_RT_GROUP_SCHED_DEFAULT_DISABLED: */
DECLARE_STATIC_कुञ्चिका_TRUE(rt_गणः_sched);
स्थिर अन्तःस्थ bool rt_गणः_sched_enabled(शून्य)
{
	प्रत्यागम static_branch_likely(&rt_गणः_sched);
}
# endif /* !CONFIG_RT_GROUP_SCHED_DEFAULT_DISABLED */
#अन्यथा /* !CONFIG_RT_GROUP_SCHED: */
# define rt_गणः_sched_enabled()	false
#यदि_अन्तः /* !CONFIG_RT_GROUP_SCHED */

स्थिर अन्तःस्थ शून्य lockdep_assert_rq_held(संरचना rq *rq)
	__assumes_ctx_lock(__rq_lockp(rq))
{
	lockdep_assert_held(__rq_lockp(rq));
}

बाह्य शून्य raw_spin_rq_तालकम्_nested(संरचना rq *rq, पूर्णाङ्क subclass)
	__acquires(__rq_lockp(rq));

बाह्य bool raw_spin_rq_trylock(संरचना rq *rq)
	__cond_acquires(true, __rq_lockp(rq));

स्थिर अन्तःस्थ शून्य raw_spin_rq_तालकम्(संरचना rq *rq)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_तालकम्_nested(rq, 0);
}

स्थिर अन्तःस्थ शून्य raw_spin_rq_unlock(संरचना rq *rq)
	__releases(__rq_lockp(rq))
{
	raw_spin_unlock(rq_lockp(rq));
}

स्थिर अन्तःस्थ शून्य raw_spin_rq_तालकम्_irq(संरचना rq *rq)
	__acquires(__rq_lockp(rq))
{
	local_irq_disable();
	raw_spin_rq_तालकम्(rq);
}

स्थिर अन्तःस्थ शून्य raw_spin_rq_unlock_irq(संरचना rq *rq)
	__releases(__rq_lockp(rq))
{
	raw_spin_rq_unlock(rq);
	local_irq_enable();
}

स्थिर अन्तःस्थ अचिह्नित दीर्घ raw_spin_rq_तालकम्_irqsave_2(संरचना rq *rq)
	__acquires(__rq_lockp(rq))
{
	अचिह्नित दीर्घ flags;

	local_irq_save(flags);
	raw_spin_rq_तालकम्(rq);

	प्रत्यागम flags;
}

स्थिर अन्तःस्थ शून्य raw_spin_rq_unlock_irqrestore(संरचना rq *rq, अचिह्नित दीर्घ flags)
	__releases(__rq_lockp(rq))
{
	raw_spin_rq_unlock(rq);
	local_irq_restore(flags);
}

#परिभाषय raw_spin_rq_तालकम्_irqsave(rq, flags)	\
कुरु {						\
	flags = raw_spin_rq_तालकम्_irqsave_2(rq);	\
} यावत् (0)

बाह्य शून्य __update_idle_core(संरचना rq *rq);

स्थिर अन्तःस्थ शून्य update_idle_core(संरचना rq *rq)
{
	यदि (sched_smt_सक्रिय())
		__update_idle_core(rq);
}

#यदि_परिभाषितम् CONFIG_FAIR_गणः_SCHED
स्थिर अन्तःस्थ संरचना task_struct *task_सम्बन्धि(संरचना sched_entity *se)
{
	WARN_ON_ONCE(!entity_is_task(se));
	प्रत्यागम container_सम्बन्धि(se, संरचना task_struct, se);
}

स्थिर अन्तःस्थ संरचना cfs_rq *task_cfs_rq(संरचना task_struct *p)
{
	प्रत्यागम p->se.cfs_rq;
}

/* runqueue on which this entity is (to be) queued */
स्थिर अन्तःस्थ संरचना cfs_rq *cfs_rq_सम्बन्धि(ध्रुव संरचना sched_entity *se)
{
	प्रत्यागम se->cfs_rq;
}

/* runqueue "owned" by this group */
स्थिर अन्तःस्थ संरचना cfs_rq *गणः_cfs_rq(संरचना sched_entity *grp)
{
	प्रत्यागम grp->my_q;
}

#अन्यथा /* !CONFIG_FAIR_GROUP_SCHED: */

#परिभाषय task_सम्बन्धि(_se)		container_सम्बन्धि(_se, संरचना task_struct, se)

स्थिर अन्तःस्थ संरचना cfs_rq *task_cfs_rq(ध्रुव संरचना task_struct *p)
{
	प्रत्यागम &task_rq(p)->cfs;
}

स्थिर अन्तःस्थ संरचना cfs_rq *cfs_rq_सम्बन्धि(ध्रुव संरचना sched_entity *se)
{
	ध्रुव संरचना task_struct *p = task_सम्बन्धि(se);
	संरचना rq *rq = task_rq(p);

	प्रत्यागम &rq->cfs;
}

/* runqueue "owned" by this group */
स्थिर अन्तःस्थ संरचना cfs_rq *गणः_cfs_rq(संरचना sched_entity *grp)
{
	प्रत्यागम रिक्त;
}

#यदि_अन्तः /* !CONFIG_FAIR_GROUP_SCHED */

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
#परिभाषय RQCF_REQ_SKIP		0x01
#परिभाषय RQCF_ACT_SKIP		0x02
#परिभाषय RQCF_UPDATED		0x04

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
	अचिह्नित दीर्घ flags;
	संरचना pin_cookie cookie;
	/*
	 * A copy of (rq::clock_update_flags & RQCF_UPDATED) for the
	 * current pin context is stashed here in case it needs to be
	 * restored in rq_repin_lock().
	 */
	अचिह्नित पूर्णाङ्क clock_update_flags;
};

बाह्य संरचना balance_callback balance_प्रक्षिप_callback;

#यदि_परिभाषितम् CONFIG_SCHED_CLASS_EXT
बाह्य ध्रुव संरचना sched_class ext_sched_class;

DECLARE_STATIC_कुञ्चिका_FALSE(__scx_enabled);	/* SCX BPF scheduler loaded */
DECLARE_STATIC_कुञ्चिका_FALSE(__scx_switched_all);	/* all fair class tasks on SCX */

#परिभाषय scx_enabled()		static_branch_unlikely(&__scx_enabled)
#परिभाषय scx_switched_all()	static_branch_unlikely(&__scx_switched_all)

स्थिर अन्तःस्थ शून्य scx_rq_clock_update(संरचना rq *rq, u64 clock)
{
	यदि (!scx_enabled())
		प्रत्यागम;
	WRITE_ONCE(rq->scx.clock, clock);
	smp_store_release(&rq->scx.flags, rq->scx.flags | SCX_RQ_CLK_VALID);
}

स्थिर अन्तःस्थ शून्य scx_rq_clock_invalidate(संरचना rq *rq)
{
	यदि (!scx_enabled())
		प्रत्यागम;
	WRITE_ONCE(rq->scx.flags, rq->scx.flags & ~SCX_RQ_CLK_VALID);
}

#अन्यथा /* !CONFIG_SCHED_CLASS_EXT: */
#परिभाषय scx_enabled()		false
#परिभाषय scx_switched_all()	false

स्थिर अन्तःस्थ शून्य scx_rq_clock_update(संरचना rq *rq, u64 clock) {}
स्थिर अन्तःस्थ शून्य scx_rq_clock_invalidate(संरचना rq *rq) {}
#यदि_अन्तः /* !CONFIG_SCHED_CLASS_EXT */

स्थिर अन्तःस्थ शून्य assert_balance_callbacks_रिक्तानि(संरचना rq *rq)
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
स्थिर अन्तःस्थ शून्य rq_pin_तालकम्(संरचना rq *rq, संरचना rq_flags *rf)
{
	rf->cookie = lockdep_pin_तालकम्(__rq_lockp(rq));

	rq->clock_update_flags &= (RQCF_REQ_SKIP|RQCF_ACT_SKIP);
	rf->clock_update_flags = 0;
	assert_balance_callbacks_रिक्तानि(rq);
}

स्थिर अन्तःस्थ शून्य rq_unpin_तालकम्(संरचना rq *rq, संरचना rq_flags *rf)
{
	यदि (rq->clock_update_flags > RQCF_ACT_SKIP)
		rf->clock_update_flags = RQCF_UPDATED;

	scx_rq_clock_invalidate(rq);
	lockdep_unpin_तालकम्(__rq_lockp(rq), rf->cookie);
}

स्थिर अन्तःस्थ शून्य rq_repin_तालकम्(संरचना rq *rq, संरचना rq_flags *rf)
{
	lockdep_repin_तालकम्(__rq_lockp(rq), rf->cookie);

	/*
	 * Restore the value we stashed in @rf for this pin context.
	 */
	rq->clock_update_flags |= rf->clock_update_flags;
}

#परिभाषय __task_rq_lock(...) __acquire_ret(___task_rq_lock(__VA_ARGS__), __rq_lockp(__ret))
बाह्य संरचना rq *___task_rq_lock(संरचना task_struct *p, संरचना rq_flags *rf) __acquires_ret;

#परिभाषय task_rq_तालकम्_2(...) __acquire_ret(task_rq_तालकम्(__VA_ARGS__), __rq_lockp(__ret))
बाह्य संरचना rq *task_rq_तालकम्(संरचना task_struct *p, संरचना rq_flags *rf)
	__acquires(&p->pi_तालकम्) __acquires_ret;

स्थिर अन्तःस्थ शून्य
__task_rq_unlock(संरचना rq *rq, संरचना task_struct *p, संरचना rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_तालकम्(rq, rf);
	raw_spin_rq_unlock(rq);
}

स्थिर अन्तःस्थ शून्य
task_rq_unlock(संरचना rq *rq, संरचना task_struct *p, संरचना rq_flags *rf)
	__releases(__rq_lockp(rq), &p->pi_तालकम्)
{
	__task_rq_unlock(rq, p, rf);
	raw_spin_unlock_irqrestore(&p->pi_तालकम्, rf->flags);
}

DEFINE_तालकम्_GUARD_1(task_rq_तालकम्_2, संरचना task_struct,
		    _T->rq = task_rq_तालकम्_2(_T->तालकम्, &_T->rf),
		    task_rq_unlock(_T->rq, _T->तालकम्, &_T->rf),
		    संरचना rq *rq; संरचना rq_flags rf)
DECLARE_तालकम्_GUARD_1_ATTRS(task_rq_तालकम्_2, __acquires(_T->pi_तालकम्), __releases((*(संरचना task_struct **)_T)->pi_तालकम्))
#परिभाषय class_task_rq_तालकम्_constructor(_T) WITH_तालकम्_GUARD_1_ATTRS(task_rq_तालकम्_2, _T)

DEFINE_तालकम्_GUARD_1(__task_rq_lock, संरचना task_struct,
		    _T->rq = __task_rq_lock(_T->तालकम्, &_T->rf),
		    __task_rq_unlock(_T->rq, _T->तालकम्, &_T->rf),
		    संरचना rq *rq; संरचना rq_flags rf)

स्थिर अन्तःस्थ शून्य rq_तालकम्_irqsave(संरचना rq *rq, संरचना rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_तालकम्_irqsave(rq, rf->flags);
	rq_pin_तालकम्(rq, rf);
}

स्थिर अन्तःस्थ शून्य rq_तालकम्_irq(संरचना rq *rq, संरचना rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_तालकम्_irq(rq);
	rq_pin_तालकम्(rq, rf);
}

स्थिर अन्तःस्थ शून्य rq_तालकम्(संरचना rq *rq, संरचना rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_तालकम्(rq);
	rq_pin_तालकम्(rq, rf);
}

स्थिर अन्तःस्थ शून्य rq_unlock_irqrestore(संरचना rq *rq, संरचना rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_तालकम्(rq, rf);
	raw_spin_rq_unlock_irqrestore(rq, rf->flags);
}

स्थिर अन्तःस्थ शून्य rq_unlock_irq(संरचना rq *rq, संरचना rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_तालकम्(rq, rf);
	raw_spin_rq_unlock_irq(rq);
}

स्थिर अन्तःस्थ शून्य rq_unlock(संरचना rq *rq, संरचना rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_तालकम्(rq, rf);
	raw_spin_rq_unlock(rq);
}

DEFINE_तालकम्_GUARD_1(rq_तालकम्, संरचना rq,
		    rq_तालकम्(_T->तालकम्, &_T->rf),
		    rq_unlock(_T->तालकम्, &_T->rf),
		    संरचना rq_flags rf)

DECLARE_तालकम्_GUARD_1_ATTRS(rq_तालकम्, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(संरचना rq **)_T)));
#परिभाषय class_rq_तालकम्_constructor(_T) WITH_तालकम्_GUARD_1_ATTRS(rq_तालकम्, _T)

DEFINE_तालकम्_GUARD_1(rq_तालकम्_irq, संरचना rq,
		    rq_तालकम्_irq(_T->तालकम्, &_T->rf),
		    rq_unlock_irq(_T->तालकम्, &_T->rf),
		    संरचना rq_flags rf)

DECLARE_तालकम्_GUARD_1_ATTRS(rq_तालकम्_irq, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(संरचना rq **)_T)));
#परिभाषय class_rq_तालकम्_irq_constructor(_T) WITH_तालकम्_GUARD_1_ATTRS(rq_तालकम्_irq, _T)

DEFINE_तालकम्_GUARD_1(rq_तालकम्_irqsave, संरचना rq,
		    rq_तालकम्_irqsave(_T->तालकम्, &_T->rf),
		    rq_unlock_irqrestore(_T->तालकम्, &_T->rf),
		    संरचना rq_flags rf)

DECLARE_तालकम्_GUARD_1_ATTRS(rq_तालकम्_irqsave, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(संरचना rq **)_T)));
#परिभाषय class_rq_तालकम्_irqsave_constructor(_T) WITH_तालकम्_GUARD_1_ATTRS(rq_तालकम्_irqsave, _T)

DEFINE_तालकम्_GUARD_1(raw_spin_rq_तालकम्_irqsave, संरचना rq,
		    raw_spin_rq_तालकम्_irqsave(_T->तालकम्, _T->flags),
		    raw_spin_rq_unlock_irqrestore(_T->तालकम्, _T->flags),
		    अचिह्नित दीर्घ flags)

DECLARE_तालकम्_GUARD_1_ATTRS(raw_spin_rq_तालकम्_irqsave, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(संरचना rq **)_T)));
#परिभाषय class_raw_spin_rq_तालकम्_irqsave_constructor(_T) \
	WITH_तालकम्_GUARD_1_ATTRS(raw_spin_rq_तालकम्_irqsave, _T)

#परिभाषय this_rq_तालकम्_irq(...) __acquire_ret(this_rq_तालकम्_irq_2(__VA_ARGS__), __rq_lockp(__ret))
स्थिर अन्तःस्थ संरचना rq *this_rq_तालकम्_irq_2(संरचना rq_flags *rf) __acquires_ret
{
	संरचना rq *rq;

	local_irq_disable();
	rq = this_rq();
	rq_तालकम्(rq, rf);

	प्रत्यागम rq;
}

#यदि_परिभाषितम् CONFIG_NUMA

गणना numa_topology_type {
	NUMA_प्रत्यक्षम्,
	NUMA_GLUELESS_MESH,
	NUMA_BACKPLANE,
};

बाह्य गणना numa_topology_type sched_numa_topology_type;
बाह्य पूर्णाङ्क sched_max_numa_distance;
बाह्य bool find_numa_distance(पूर्णाङ्क distance);
बाह्य शून्य sched_init_numa(पूर्णाङ्क offline_ग्रन्थिः);
बाह्य शून्य sched_update_numa(पूर्णाङ्क cpu, bool online);
बाह्य शून्य sched_domains_numa_masks_समुच्चयः(अचिह्नित पूर्णाङ्क cpu);
बाह्य शून्य sched_domains_numa_masks_clear(अचिह्नित पूर्णाङ्क cpu);
बाह्य पूर्णाङ्क sched_numa_find_closest(ध्रुव संरचना cpumask *cpus, पूर्णाङ्क cpu);

#अन्यथा /* !CONFIG_NUMA: */

स्थिर अन्तःस्थ शून्य sched_init_numa(पूर्णाङ्क offline_ग्रन्थिः) { }
स्थिर अन्तःस्थ शून्य sched_update_numa(पूर्णाङ्क cpu, bool online) { }
स्थिर अन्तःस्थ शून्य sched_domains_numa_masks_समुच्चयः(अचिह्नित पूर्णाङ्क cpu) { }
स्थिर अन्तःस्थ शून्य sched_domains_numa_masks_clear(अचिह्नित पूर्णाङ्क cpu) { }

स्थिर अन्तःस्थ पूर्णाङ्क sched_numa_find_closest(ध्रुव संरचना cpumask *cpus, पूर्णाङ्क cpu)
{
	प्रत्यागम nr_cpu_ids;
}

#यदि_अन्तः /* !CONFIG_NUMA */

#यदि_परिभाषितम् CONFIG_NUMA_BALANCING

/* The regions in numa_faults array from task_struct */
गणना numa_दोषाः_stats {
	NUMA_MEM = 0,
	NUMA_CPU,
	NUMA_MEMBUF,
	NUMA_CPUBUF
};

बाह्य शून्य sched_setnuma(संरचना task_struct *p, पूर्णाङ्क ग्रन्थिः);
बाह्य पूर्णाङ्क migrate_task_गन्तव्यम्(संरचना task_struct *p, पूर्णाङ्क cpu);
बाह्य पूर्णाङ्क migrate_swap(संरचना task_struct *p, संरचना task_struct *t,
			पूर्णाङ्क cpu, पूर्णाङ्क scpu);
बाह्य शून्य init_numa_balancing(u64 clone_flags, संरचना task_struct *p);

#अन्यथा /* !CONFIG_NUMA_BALANCING: */

स्थिर अन्तःस्थ शून्य
init_numa_balancing(u64 clone_flags, संरचना task_struct *p)
{
}

#यदि_अन्तः /* !CONFIG_NUMA_BALANCING */

पूर्णाङ्क task_llc(ध्रुव संरचना task_struct *p);

स्थिर अन्तःस्थ शून्य
queue_balance_callback(संरचना rq *rq,
		       संरचना balance_callback *शीर्ष,
		       शून्य (*func)(संरचना rq *rq))
{
	lockdep_assert_rq_held(rq);

	/*
	 * Don't (re)queue an already queued item; nor queue anything when
	 * balance_push() is active, see the comment with
	 * balance_push_callback.
	 */
	यदि (unlikely(शीर्ष->अग्र || rq->balance_callback == &balance_प्रक्षिप_callback))
		प्रत्यागम;

	शीर्ष->func = func;
	शीर्ष->अग्र = rq->balance_callback;
	rq->balance_callback = शीर्ष;
}

#परिभाषय rcu_dereference_sched_domain(p) \
	rcu_dereference_all_check((p), lockdep_is_held(&sched_domains_mutex))

/*
 * The domain tree (rq->sd) is protected by RCU's quiescent state transition.
 * See destroy_sched_domains: call_rcu for details.
 *
 * The domain tree of any CPU may only be accessed from within
 * preempt-disabled sections.
 */
#परिभाषय for_each_domain(cpu, __sd) \
	प्रति (__sd = rcu_dereference_sched_domain(cpu_rq(cpu)->sd); \
			__sd; __sd = __sd->parent)

/* A mask of all the SD flags that have the SDF_SHARED_CHILD metaflag */
#परिभाषय SD_FLAG(name, mflags) (name * !!((mflags) & SDF_SHARED_CHILD)) |
स्थिर ध्रुव अचिह्नित पूर्णाङ्क SD_SHARED_CHILD_आवरणम् =
#अन्तर्भाव <linux/sched/sd_flags.h>
0;
#अपरिभाषय SD_FLAG

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
		यदि (sd->flags & flag) {
			hsd = sd;
			अनुवर्त;
		}

		/*
		 * Stop the search if @flag is known to be shared at lower
		 * levels. It will not be found further up.
		 */
		यदि (flag & SD_SHARED_CHILD_आवरणम्)
			भङ्ग;
	}

	प्रत्यागम hsd;
}

स्थिर अन्तःस्थ संरचना sched_domain *निम्नतमम्_flag_domain(पूर्णाङ्क cpu, पूर्णाङ्क flag)
{
	संरचना sched_domain *sd;

	for_each_domain(cpu, sd) {
		यदि (sd->flags & flag)
			भङ्ग;
	}

	प्रत्यागम sd;
}

DECLARE_प्रत्येकम्_CPU(संरचना sched_domain __rcu *, sd_llc);
DECLARE_प्रत्येकम्_CPU(पूर्णाङ्क, sd_llc_आकार);
DECLARE_प्रत्येकम्_CPU(पूर्णाङ्क, sd_llc_id);
DECLARE_प्रत्येकम्_CPU(पूर्णाङ्क, sd_share_id);
DECLARE_प्रत्येकम्_CPU(संरचना sched_domain_shared __rcu *, sd_llc_shared);
DECLARE_प्रत्येकम्_CPU(संरचना sched_domain_shared __rcu *, sd_balance_shared);
DECLARE_प्रत्येकम्_CPU(संरचना sched_domain __rcu *, sd_numa);
DECLARE_प्रत्येकम्_CPU(संरचना sched_domain __rcu *, sd_asym_packing);
DECLARE_प्रत्येकम्_CPU(संरचना sched_domain __rcu *, sd_asym_cpucapacity);

बाह्य संरचना static_कुञ्चिका_false sched_asym_cpucapacity;
बाह्य संरचना static_कुञ्चिका_false sched_cluster_सक्रिय;

स्थिर __always_inline bool sched_asym_cpucap_सक्रिय(शून्य)
{
	प्रत्यागम static_branch_unlikely(&sched_asym_cpucapacity);
}

संरचना sched_गणः_क्षमता {
	atomic_t		ref;
	/*
	 * CPU capacity of this group, SCHED_CAPACITY_SCALE being max capacity
	 * for a single CPU.
	 */
	अचिह्नित दीर्घ		क्षमता;
	अचिह्नित दीर्घ		min_क्षमता;		/* Min per-CPU capacity in group */
	अचिह्नित दीर्घ		max_क्षमता;		/* Max per-CPU capacity in group */
	अचिह्नित दीर्घ		अग्र_update;
	पूर्णाङ्क			imbalance;		/* XXX unrelated to capacity but shared group state */

	पूर्णाङ्क			id;

	अचिह्नित दीर्घ		cpumask[];		/* Balance mask */
};

संरचना sched_गणः {
	संरचना sched_गणः	*अग्र;			/* Must be a circular list */
	atomic_t		ref;

	अचिह्नित पूर्णाङ्क		गणः_weight;
	अचिह्नित पूर्णाङ्क		cores;
	संरचना sched_गणः_क्षमता *sgc;
	पूर्णाङ्क			asym_prefer_cpu;	/* CPU of highest priority in group */
	पूर्णाङ्क			flags;

	/*
	 * The CPUs this group covers.
	 *
	 * NOTE: this field is variable length. (Allocated dynamically
	 * by attaching extra space to the end of the structure,
	 * depending on how many CPUs the kernel has booted up with)
	 */
	अचिह्नित दीर्घ		cpumask[];
};

स्थिर अन्तःस्थ संरचना cpumask *sched_गणः_विस्तृतिः(संरचना sched_गणः *sg)
{
	प्रत्यागम गन्तव्यम्_cpumask(sg->cpumask);
}

/*
 * See build_balance_mask().
 */
स्थिर अन्तःस्थ संरचना cpumask *गणः_balance_आवरणम्(संरचना sched_गणः *sg)
{
	प्रत्यागम गन्तव्यम्_cpumask(sg->sgc->cpumask);
}

बाह्य पूर्णाङ्क गणः_balance_cpu(संरचना sched_गणः *sg);

बाह्य शून्य update_sched_domain_debugfs(शून्य);
बाह्य शून्य dirty_sched_domain_sysctl(पूर्णाङ्क cpu);

बाह्य पूर्णाङ्क sched_update_scaling(शून्य);

स्थिर अन्तःस्थ ध्रुव संरचना cpumask *task_user_cpus(संरचना task_struct *p)
{
	यदि (!p->user_cpus_ptr)
		प्रत्यागम cpu_possible_आवरणम्; /* &init_task.cpus_mask */
	प्रत्यागम p->user_cpus_ptr;
}

#यदि_परिभाषितम् CONFIG_CGROUP_SCHED

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
स्थिर अन्तःस्थ संरचना task_गणः *task_गणः(संरचना task_struct *p)
{
	प्रत्यागम p->sched_task_गणः;
}

#यदि_परिभाषितम् CONFIG_FAIR_गणः_SCHED
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
स्थिर अन्तःस्थ संरचना cfs_rq *tg_cfs_rq(संरचना task_गणः *tg, पूर्णाङ्क cpu)
{
	प्रत्यागम प्रत्येकम्_cpu_ptr(tg->cfs_rq, cpu);
}

स्थिर अन्तःस्थ संरचना sched_entity *tg_se(संरचना task_गणः *tg, पूर्णाङ्क cpu)
{
	संरचना cfs_tg_state *state;

	यदि (is_मूलम्_task_गणः(tg))
		प्रत्यागम रिक्त;

	state = container_सम्बन्धि(tg_cfs_rq(tg, cpu), संरचना cfs_tg_state, cfs_rq);
	प्रत्यागम &state->se;
}

स्थिर अन्तःस्थ संरचना sched_entity *cfs_rq_se(संरचना cfs_rq *cfs_rq)
{
	संरचना cfs_tg_state *state;

	यदि (is_मूलम्_task_गणः(cfs_rq->tg))
		प्रत्यागम रिक्त;

	state = container_सम्बन्धि(cfs_rq, संरचना cfs_tg_state, cfs_rq);
	प्रत्यागम &state->se;
}
#यदि_अन्तः

/* Change a task's cfs_rq and parent entity if it moves across CPUs/groups */
स्थिर अन्तःस्थ शून्य समुच्चयः_task_rq(संरचना task_struct *p, अचिह्नित पूर्णाङ्क cpu)
{
#यदि defined(CONFIG_FAIR_गणः_SCHED) || defined(CONFIG_RT_गणः_SCHED)
	संरचना task_गणः *tg = task_गणः(p);
#यदि_अन्तः

#यदि_परिभाषितम् CONFIG_FAIR_गणः_SCHED
	समुच्चयः_task_rq_fair(&p->se, p->se.cfs_rq, tg_cfs_rq(tg, cpu));
	p->se.cfs_rq = tg_cfs_rq(tg, cpu);
	p->se.parent = tg_se(tg, cpu);
	p->se.गभीरता = p->se.parent ? p->se.parent->गभीरता + 1 : 0;
#यदि_अन्तः

#यदि_परिभाषितम् CONFIG_RT_गणः_SCHED
	/*
	 * p->rt.rt_rq is NULL initially and it is easier to assign
	 * root_task_group's rt_rq than switching in rt_rq_of_se()
	 * Clobbers tg(!)
	 */
	यदि (!rt_गणः_sched_enabled())
		tg = &मूलम्_task_गणः;
	p->rt.rt_rq  = tg->rt_rq[cpu];
	p->rt.parent = tg->rt_se[cpu];
#यदि_अन्तः /* CONFIG_RT_GROUP_SCHED */
}

#अन्यथा /* !CONFIG_CGROUP_SCHED: */

स्थिर अन्तःस्थ शून्य समुच्चयः_task_rq(संरचना task_struct *p, अचिह्नित पूर्णाङ्क cpu) { }

स्थिर अन्तःस्थ संरचना task_गणः *task_गणः(संरचना task_struct *p)
{
	प्रत्यागम रिक्त;
}

#यदि_अन्तः /* !CONFIG_CGROUP_SCHED */

स्थिर अन्तःस्थ शून्य __set_task_cpu(संरचना task_struct *p, अचिह्नित पूर्णाङ्क cpu)
{
	समुच्चयः_task_rq(p, cpu);
#यदि_परिभाषितम् CONFIG_SMP
	/*
	 * After ->cpu is set up to a new value, task_rq_lock(p, ...) can be
	 * successfully executed on another CPU. We must ensure that updates of
	 * per-task data have been completed by this moment.
	 */
	smp_wmb();
	WRITE_ONCE(task_thread_info(p)->cpu, cpu);
	p->wake_cpu = cpu;
	rseq_sched_समुच्चयः_ids_परिवर्तितम्(p);
#यदि_अन्तः /* CONFIG_SMP */
}

/*
 * Tunables:
 */

#परिभाषय SCHED_FEAT(name, enabled)	\
	__SCHED_FEAT_##name ,

गणना {
#अन्तर्भाव "features.h"
	__SCHED_FEAT_NR,
};

#अपरिभाषय SCHED_FEAT

/*
 * To support run-time toggling of sched features, all the translation units
 * (but core.c) reference the sysctl_sched_features defined in core.c.
 */
बाह्य __read_mostly अचिह्नित पूर्णाङ्क sysctl_sched_features;

#यदि_परिभाषितम् CONFIG_JUMP_LABEL

सङ्घ sched_feat_कुञ्चिका {
	संरचना static_कुञ्चिका_true	कुञ्चिका_true;
	संरचना static_कुञ्चिका_false	कुञ्चिका_false;
};

#परिभाषय sched_feat_branch_true(कुञ्चिका)	static_branch_likely(&(कुञ्चिका)->कुञ्चिका_true)
#परिभाषय sched_feat_branch_false(कुञ्चिका)	static_branch_unlikely(&(कुञ्चिका)->कुञ्चिका_false)

#परिभाषय SCHED_FEAT(name, enabled)			\
स्थिर __always_inline bool				\
static_branch_##name(सङ्घ sched_feat_कुञ्चिका *कुञ्चिका) 	\
{							\
	प्रत्यागम sched_feat_branch_##enabled(कुञ्चिका);	\
}

#अन्तर्भाव "features.h"
#अपरिभाषय SCHED_FEAT

बाह्य सङ्घ sched_feat_कुञ्चिका sched_feat_कुञ्चिकाः[__SCHED_FEAT_NR];
#परिभाषय sched_feat(x) (static_branch_##x(&sched_feat_कुञ्चिकाः[__SCHED_FEAT_##x]))

#अन्यथा /* !CONFIG_JUMP_LABEL: */

#परिभाषय sched_feat(x) (sysctl_sched_features & (1UL << __SCHED_FEAT_##x))

#यदि_अन्तः /* !CONFIG_JUMP_LABEL */

बाह्य संरचना static_कुञ्चिका_false sched_numa_balancing;
बाह्य संरचना static_कुञ्चिका_false sched_schedstats;

स्थिर अन्तःस्थ u64 global_rt_period(शून्य)
{
	प्रत्यागम (u64)sysctl_sched_rt_period * NSEC_प्रत्येकम्_USEC;
}

स्थिर अन्तःस्थ u64 global_rt_runtime(शून्य)
{
	यदि (sysctl_sched_rt_runtime < 0)
		प्रत्यागम RUNTIME_INF;

	प्रत्यागम (u64)sysctl_sched_rt_runtime * NSEC_प्रत्येकम्_USEC;
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
	यदि (!sched_proxy_exec())
		प्रत्यागम false;

	प्रत्यागम !!p->blocked_on;
}

#यदि_परिभाषितम् CONFIG_SCHED_PROXY_EXEC
शून्य sched_proxy_खण्डम्_task(संरचना rq *rq, संरचना task_struct *p);
#अन्यथा
स्थिर अन्तःस्थ शून्य sched_proxy_खण्डम्_task(संरचना rq *rq, संरचना task_struct *p) {}
#यदि_अन्तः

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
#परिभाषय WF_EXEC			0x02 /* Wakeup after exec; maps to SD_BALANCE_EXEC */
#परिभाषय WF_FORK			0x04 /* Wakeup after fork; maps to SD_BALANCE_FORK */
#परिभाषय WF_TTWU			0x08 /* Wakeup;            maps to SD_BALANCE_WAKE */
/*
 * Hint that the caller expects the waker to sleep soon.
 * Scheduler classes may use it for placement or preemption.
 * Callers must not rely on it to prevent migration,
 * preserve CPU locality or make the wakee run next.
 */
#परिभाषय WF_SYNC			0x10
#परिभाषय WF_MIGRATED		0x20 /* Internal use, task got migrated */
#परिभाषय WF_CURRENT_CPU		0x40 /* Prefer to move the wakee to the current CPU. */
#परिभाषय WF_RQ_SELECTED		0x80 /* ->select_task_rq() was called */
#परिभाषय WF_TTWU_RQ		0x100 /* Wakeup completed through ttwu_runnable() */

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

#परिभाषय WEIGHT_IDLEPRIO		3
#परिभाषय WMULT_IDLEPRIO		1431655765

बाह्य ध्रुव पूर्णाङ्क		sched_prio_गन्तव्यम्_weight[40];
बाह्य ध्रुव u32		sched_prio_गन्तव्यम्_wmult[40];

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

#परिभाषय पङ्क्तेः_निष्कासय_SLEEP		0x0001 /* Matches ENQUEUE_WAKEUP */
#परिभाषय पङ्क्तेः_निष्कासय_SAVE		0x0002 /* Matches ENQUEUE_RESTORE */
#परिभाषय पङ्क्तेः_निष्कासय_MOVE		0x0004 /* Matches ENQUEUE_MOVE */
#परिभाषय पङ्क्तेः_निष्कासय_NOCLOCK		0x0008 /* Matches ENQUEUE_NOCLOCK */

#परिभाषय पङ्क्तेः_निष्कासय_MIGRATING	0x0010 /* Matches ENQUEUE_MIGRATING */
#परिभाषय पङ्क्तेः_निष्कासय_DELAYED		0x0020 /* Matches ENQUEUE_DELAYED */
#परिभाषय पङ्क्तेः_निष्कासय_CLASS		0x0040 /* Matches ENQUEUE_CLASS */

#परिभाषय पङ्क्तेः_निष्कासय_SPECIAL		0x00010000
#परिभाषय पङ्क्तेः_निष्कासय_THROTTLE	0x00020000

#परिभाषय पङ्क्तौ_निक्षिप_WAKEUP		0x0001
#परिभाषय पङ्क्तौ_निक्षिप_RESTORE		0x0002
#परिभाषय पङ्क्तौ_निक्षिप_MOVE		0x0004
#परिभाषय पङ्क्तौ_निक्षिप_NOCLOCK		0x0008

#परिभाषय पङ्क्तौ_निक्षिप_MIGRATING	0x0010
#परिभाषय पङ्क्तौ_निक्षिप_DELAYED		0x0020
#परिभाषय पङ्क्तौ_निक्षिप_CLASS		0x0040

#परिभाषय पङ्क्तौ_निक्षिप_शीर्ष		0x00010000
#परिभाषय पङ्क्तौ_निक्षिप_REPLENISH	0x00020000
#परिभाषय पङ्क्तौ_निक्षिप_MIGRATED	0x00040000
#परिभाषय पङ्क्तौ_निक्षिप_INITIAL		0x00080000
#परिभाषय पङ्क्तौ_निक्षिप_RQ_SELECTED	0x00100000
#परिभाषय पङ्क्तौ_निक्षिप_QUEUED		0x00200000

#परिभाषय RETRY_TASK		((शून्य *)-1UL)

संरचना affinity_context {
	ध्रुव संरचना cpumask	*new_आवरणम्;
	संरचना cpumask		*user_आवरणम्;
	अचिह्नित पूर्णाङ्क		flags;
};

बाह्य s64 update_curr_common(संरचना rq *rq);

गणना snt_e {
	SNT_प्रसामान्यम्,	/* set_next_task() */
	SNT_PICK,	/* put_prev_set_next_task(): prev != next */
	SNT_REPICK,	/* put_prev_set_next_task(): prev == next */
};

संरचना sched_class {

#यदि_परिभाषितम् CONFIG_UCLAMP_TASK
	पूर्णाङ्क uclamp_enabled;
#यदि_अन्तः

	/*
	 * move_queued_task/activate_task/enqueue_task: rq->lock
	 * ttwu_do_activate/activate_task/enqueue_task: rq->lock
	 * wake_up_new_task/activate_task/enqueue_task: task_rq_lock
	 * ttwu_runnable/enqueue_task: task_rq_lock
	 * proxy_task_current: rq->lock
	 * sched_change_end
	 */
	शून्य (*पङ्क्तौ_निक्षिप_task) (संरचना rq *rq, संरचना task_struct *p, पूर्णाङ्क flags);
	/*
	 * move_queued_task/deactivate_task/dequeue_task: rq->lock
	 * __schedule/block_task/dequeue_task: rq->lock
	 * proxy_task_current: rq->lock
	 * wait_task_inactive: task_rq_lock
	 * sched_change_begin
	 */
	bool (*पङ्क्तेः_निष्कासय_task) (संरचना rq *rq, संरचना task_struct *p, पूर्णाङ्क flags);

	/*
	 * do_sched_yield: rq->lock
	 */
	शून्य (*yield_task)   (संरचना rq *rq);
	/*
	 * yield_to: rq->lock (double)
	 */
	bool (*yield_गन्तव्यम्_task)(संरचना rq *rq, संरचना task_struct *p);

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
	शून्य (*put_पूर्व_task)(संरचना rq *rq, संरचना task_struct *p, संरचना task_struct *अग्र);
	शून्य (*समुच्चयः_अग्र_task)(संरचना rq *rq, संरचना task_struct *p, गणना snt_e type);

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
	शून्य (*समुच्चयः_cpus_allowed)(संरचना task_struct *p, संरचना affinity_context *ctx);

	/*
	 * sched_set_rq_{on,off}line: rq->lock
	 */
	शून्य (*rq_online)(संरचना rq *rq);
	शून्य (*rq_offline)(संरचना rq *rq);

	/*
	 * push_cpu_stop: p->pi_lock && rq->lock
	 */
	संरचना rq *(*find_तालकम्_rq)(संरचना task_struct *p, संरचना rq *rq);

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
	शून्य (*switching_गन्तव्यम्)  (संरचना rq *this_rq, संरचना task_struct *task);
	शून्य (*switched_गन्तव्यम्)   (संरचना rq *this_rq, संरचना task_struct *task);
	u64  (*get_prio)     (संरचना rq *this_rq, संरचना task_struct *task);
	शून्य (*prio_परिवर्तितम्) (संरचना rq *this_rq, संरचना task_struct *task,
			      u64 oldprio);

	/*
	 * set_load_weight: task_rq_lock + sched_change
	 * __setscheduler_parms: task_rq_lock + sched_change
	 */
	शून्य (*reweight_task)(संरचना rq *this_rq, संरचना task_struct *task,
			      ध्रुव संरचना आरोपय_weight *lw);

	/*
	 * sched_rr_get_interval: task_rq_lock
	 */
	अचिह्नित पूर्णाङ्क (*get_rr_interval)(संरचना rq *rq,
					संरचना task_struct *task);

	/*
	 * task_sched_runtime: task_rq_lock
	 */
	शून्य (*update_curr)(संरचना rq *rq);

#यदि_परिभाषितम् CONFIG_FAIR_गणः_SCHED
	/*
	 * sched_change_group: task_rq_lock + sched_change
	 */
	शून्य (*task_change_गणः)(संरचना task_struct *p);
#यदि_अन्तः

#यदि_परिभाषितम् CONFIG_SCHED_CORE
	/*
	 * pick_next_task: rq->lock
	 * try_steal_cookie: rq->lock (double)
	 */
	पूर्णाङ्क (*task_is_throttled)(संरचना task_struct *p, पूर्णाङ्क cpu);
#यदि_अन्तः
};

स्थिर अन्तःस्थ शून्य put_पूर्व_task(संरचना rq *rq, संरचना task_struct *पूर्व)
{
	WARN_ON_ONCE(rq->donor != पूर्व);
	पूर्व->sched_class->put_पूर्व_task(rq, पूर्व, रिक्त);
}

स्थिर अन्तःस्थ शून्य समुच्चयः_अग्र_task(संरचना rq *rq, संरचना task_struct *अग्र)
{
	अग्र->sched_class->समुच्चयः_अग्र_task(rq, अग्र, SNT_प्रसामान्यम्);
}

स्थिर अन्तःस्थ शून्य
__put_prev_set_next_dl_server(संरचना rq *rq,
			      संरचना task_struct *पूर्व,
			      संरचना task_struct *अग्र)
{
	पूर्व->dl_server = रिक्त;
	अग्र->dl_server = rq->dl_server;
	rq->dl_server = रिक्त;
}

स्थिर अन्तःस्थ शून्य put_पूर्व_समुच्चयः_अग्र_task(संरचना rq *rq,
					  संरचना task_struct *पूर्व,
					  संरचना task_struct *अग्र)
{
	WARN_ON_ONCE(rq->donor != पूर्व);

	__put_prev_set_next_dl_server(rq, पूर्व, अग्र);

	यदि (अग्र == पूर्व) {
		अग्र->sched_class->समुच्चयः_अग्र_task(rq, अग्र, SNT_REPICK);
		प्रत्यागम;
	}

	पूर्व->sched_class->put_पूर्व_task(rq, पूर्व, अग्र);
	अग्र->sched_class->समुच्चयः_अग्र_task(rq, अग्र, SNT_PICK);
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
#परिभाषय DEFINE_SCHED_CLASS(name) \
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
स्थिर अन्तःस्थ ध्रुव संरचना sched_class *अग्र_सक्रिय_class(ध्रुव संरचना sched_class *class)
{
	class++;
#यदि_परिभाषितम् CONFIG_SCHED_CLASS_EXT
	यदि (scx_switched_all() && class == &fair_sched_class)
		class++;
	यदि (!scx_enabled() && class == &ext_sched_class)
		class++;
#यदि_अन्तः
	प्रत्यागम class;
}

#परिभाषय for_class_range(class, _from, गन्तव्यम्_2) \
	प्रति (class = (_from); class < (गन्तव्यम्_2); class++)

#परिभाषय for_each_class(class) \
	for_class_range(class, __sched_class_highest, __sched_class_lowest)

#परिभाषय for_सक्रिय_class_range(class, _from, गन्तव्यम्_2)				\
	प्रति (class = (_from); class != (गन्तव्यम्_2); class = अग्र_सक्रिय_class(class))

#परिभाषय for_each_सक्रिय_class(class)						\
	for_सक्रिय_class_range(class, __sched_class_highest, __sched_class_lowest)

#परिभाषय sched_class_above(_a, _b)	((_a) < (_b))

स्थिर अन्तःस्थ शून्य rq_modified_begin(संरचना rq *rq, ध्रुव संरचना sched_class *class)
{
	यदि (sched_class_above(rq->अग्र_class, class))
		rq->अग्र_class = class;
}

स्थिर अन्तःस्थ bool rq_modified_above(संरचना rq *rq, ध्रुव संरचना sched_class *class)
{
	प्रत्यागम sched_class_above(rq->अग्र_class, class);
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

#परिभाषय SCA_CHECK		0x01
#परिभाषय SCA_MIGRATE_DISABLE	0x02
#परिभाषय SCA_MIGRATE_ENABLE	0x04
#परिभाषय SCA_USER		0x08

बाह्य शून्य update_गणः_क्षमता(संरचना sched_domain *sd, पूर्णाङ्क cpu);

बाह्य शून्य sched_balance_trigger(संरचना rq *rq);

बाह्य पूर्णाङ्क __set_cpus_allowed_ptr(संरचना task_struct *p, संरचना affinity_context *ctx);
बाह्य शून्य समुच्चयः_cpus_allowed_common(संरचना task_struct *p, संरचना affinity_context *ctx);

स्थिर अन्तःस्थ bool task_allowed_on_cpu(संरचना task_struct *p, पूर्णाङ्क cpu)
{
	/* When not in the task's cpumask, no point in looking further. */
	यदि (!cpumask_test_cpu(cpu, p->cpus_ptr))
		प्रत्यागम false;

	/* Can @cpu run a user thread? */
	यदि (!(p->flags & PF_KTHREAD) && !task_cpu_possible(cpu, p))
		प्रत्यागम false;

	प्रत्यागम true;
}

स्थिर अन्तःस्थ cpumask_t *alloc_user_cpus_ptr(पूर्णाङ्क ग्रन्थिः)
{
	/*
	 * See set_cpus_allowed_force() above for the rcu_head usage.
	 */
	पूर्णाङ्क आकार = max_t(पूर्णाङ्क, cpumask_आकार(), परिमाण(संरचना rcu_शीर्ष));

	प्रत्यागम kmalloc_ग्रन्थिः(आकार, GFP_KERNEL, ग्रन्थिः);
}

स्थिर अन्तःस्थ संरचना task_struct *get_प्रक्षिप_task(संरचना rq *rq)
{
	संरचना task_struct *p = rq->donor;

	lockdep_assert_rq_held(rq);

	यदि (rq->प्रक्षिप_busy)
		प्रत्यागम रिक्त;

	यदि (p->nr_cpus_allowed == 1)
		प्रत्यागम रिक्त;

	यदि (p->migration_disabled)
		प्रत्यागम रिक्त;

	rq->प्रक्षिप_busy = true;
	प्रत्यागम get_task_struct(p);
}

बाह्य पूर्णाङ्क प्रक्षिप_cpu_stop(शून्य *arg);

#यदि_परिभाषितम् CONFIG_CPU_IDLE

स्थिर अन्तःस्थ शून्य idle_समुच्चयः_state(संरचना rq *rq,
				  संरचना cpuidle_state *idle_state)
{
	rq->idle_state = idle_state;
}

स्थिर अन्तःस्थ संरचना cpuidle_state *idle_get_state(संरचना rq *rq)
{
	lockdep_assert(rcu_read_तालकम्_any_held());

	प्रत्यागम rq->idle_state;
}

#अन्यथा /* !CONFIG_CPU_IDLE: */

स्थिर अन्तःस्थ शून्य idle_समुच्चयः_state(संरचना rq *rq,
				  संरचना cpuidle_state *idle_state)
{
}

स्थिर अन्तःस्थ संरचना cpuidle_state *idle_get_state(संरचना rq *rq)
{
	प्रत्यागम रिक्त;
}

#यदि_अन्तः /* !CONFIG_CPU_IDLE */

बाह्य शून्य कालसारणी_idle(शून्य);
asmlinkage शून्य कालसारणी_user(शून्य);

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

#परिभाषय BW_सरणम्		20
#परिभाषय BW_UNIT			(1 << BW_सरणम्)
#परिभाषय अनुपातः_सरणम्		8
#परिभाषय MAX_BW_द्वयङ्काः		(64 - BW_सरणम्)
#परिभाषय MAX_BW			((1ULL << MAX_BW_द्वयङ्काः) - 1)

बाह्य u64 गन्तव्यम्_अनुपातः(u64 period, u64 runtime);

बाह्य शून्य init_entity_runnable_मध्यमानम्(संरचना sched_entity *se);
बाह्य शून्य post_init_entity_util_avg(संरचना task_struct *p);

#यदि_परिभाषितम् CONFIG_NO_HZ_पूर्णम्
बाह्य bool sched_can_stop_tick(संरचना rq *rq);
बाह्य पूर्णाङ्क __init sched_tick_offload_init(शून्य);

/*
 * Tick may be needed by tasks in the runqueue depending on their policy and
 * requirements. If tick is needed, lets send the target an IPI to kick it out of
 * nohz mode if necessary.
 */
स्थिर अन्तःस्थ शून्य sched_update_tick_dependency(संरचना rq *rq)
{
	पूर्णाङ्क cpu = cpu_सम्बन्धि(rq);

	यदि (!tick_nohz_पूर्णम्_cpu(cpu))
		प्रत्यागम;

	यदि (sched_can_stop_tick(rq))
		tick_nohz_dep_clear_cpu(cpu, TICK_DEP_BIT_SCHED);
	अन्यथा
		tick_nohz_dep_समुच्चयः_cpu(cpu, TICK_DEP_BIT_SCHED);
}
#अन्यथा /* !CONFIG_NO_HZ_FULL: */
स्थिर अन्तःस्थ पूर्णाङ्क sched_tick_offload_init(शून्य) { प्रत्यागम 0; }
स्थिर अन्तःस्थ शून्य sched_update_tick_dependency(संरचना rq *rq) { }
#यदि_अन्तः /* !CONFIG_NO_HZ_FULL */

स्थिर अन्तःस्थ शून्य सङ्कलय_nr_running(संरचना rq *rq, अचिह्नित गणनम्)
{
	अचिह्नित पूर्व_nr = rq->nr_running;

	rq->nr_running = पूर्व_nr + गणनम्;
	यदि (trace_sched_update_nr_running_tp_enabled()) {
		call_trace_sched_update_nr_running(rq, गणनम्);
	}

	यदि (पूर्व_nr < 2 && rq->nr_running >= 2)
		समुच्चयः_rd_overloaded(rq->rd, 1);

	sched_update_tick_dependency(rq);
}

स्थिर अन्तःस्थ शून्य sub_nr_running(संरचना rq *rq, अचिह्नित गणनम्)
{
	rq->nr_running -= गणनम्;
	यदि (trace_sched_update_nr_running_tp_enabled()) {
		call_trace_sched_update_nr_running(rq, -गणनम्);
	}

	/* Check if we still need preemption */
	sched_update_tick_dependency(rq);
}

स्थिर अन्तःस्थ शून्य __block_task(संरचना rq *rq, संरचना task_struct *p)
{
	यदि (p->sched_contributes_गन्तव्यम्_आरोपय)
		rq->nr_uninterruptible++;

	यदि (p->अन्तर्_iowait) {
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
	activate_task(rq, p, पङ्क्तौ_निक्षिप_NOCLOCK);
	wakeup_preempt(rq, p, 0);
}

/*
 * attach_one_task() -- attaches the task returned from detach_one_task() to
 * its new rq.
 */
स्थिर अन्तःस्थ शून्य attach_one_task(संरचना rq *rq, संरचना task_struct *p)
{
	guard(rq_तालकम्)(rq);
	update_rq_clock(rq);
	attach_task(rq, p);
}

/*
 * __attach_tasks() - attaches a list of tasks (using se.group_node) to
 * the new rq
 */
स्थिर अन्तःस्थ शून्य __attach_tasks(संरचना rq *rq, संरचना list_शीर्ष *tasks)
{
	guard(rq_तालकम्)(rq);
	update_rq_clock(rq);

	यावत् (!list_रिक्तानि(tasks)) {
		संरचना task_struct *p;

		p = list_first_प्रविष्टिः(tasks, संरचना task_struct, se.गणः_ग्रन्थिः);
		list_del_init(&p->se.गणः_ग्रन्थिः);

		attach_task(rq, p);
	}
}

#यदि_परिभाषितम् CONFIG_PREEMPT_RT
# define SCHED_NR_MIGRATE_BREAK 8
#अन्यथा
# define SCHED_NR_MIGRATE_BREAK 32
#यदि_अन्तः

बाह्य __read_mostly अचिह्नित पूर्णाङ्क sysctl_sched_nr_migrate;
बाह्य __read_mostly अचिह्नित पूर्णाङ्क sysctl_sched_migration_cost;

बाह्य अचिह्नित पूर्णाङ्क sysctl_sched_आधारः_खण्डिका;

बाह्य पूर्णाङ्क sysctl_resched_latency_warn_ms;
बाह्य पूर्णाङ्क sysctl_resched_latency_warn_once;

बाह्य अचिह्नित पूर्णाङ्क sysctl_sched_tunable_scaling;

बाह्य अचिह्नित पूर्णाङ्क sysctl_numa_balancing_अवलोकनम्_delay;
बाह्य अचिह्नित पूर्णाङ्क sysctl_numa_balancing_अवलोकनम्_period_min;
बाह्य अचिह्नित पूर्णाङ्क sysctl_numa_balancing_अवलोकनम्_period_max;
बाह्य अचिह्नित पूर्णाङ्क sysctl_numa_balancing_अवलोकनम्_आकार;
बाह्य अचिह्नित पूर्णाङ्क sysctl_numa_balancing_hot_देहली;

#यदि_परिभाषितम् CONFIG_SCHED_HRTICK

/*
 * Use hrtick when:
 *  - enabled by features
 *  - hrtimer is actually high res
 */
स्थिर अन्तःस्थ bool hrtick_enabled(संरचना rq *rq)
{
	प्रत्यागम cpu_सक्रिय(cpu_सम्बन्धि(rq)) && hrtimer_highres_enabled();
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
स्थिर अन्तःस्थ bool hrtick_सक्रिय(संरचना rq *rq)
{
	प्रत्यागम hrtimer_सक्रिय(&rq->hrtick_timer);
}

#अन्यथा /* !CONFIG_SCHED_HRTICK: */
स्थिर अन्तःस्थ bool hrtick_enabled_fair(संरचना rq *rq) { प्रत्यागम false; }
स्थिर अन्तःस्थ bool hrtick_enabled_dl(संरचना rq *rq) { प्रत्यागम false; }
स्थिर अन्तःस्थ bool hrtick_enabled(संरचना rq *rq) { प्रत्यागम false; }
#यदि_अन्तः /* !CONFIG_SCHED_HRTICK */

#यदि_अपरिभाषितम् arch_scale_freq_tick
स्थिर __always_inline शून्य arch_scale_freq_tick(शून्य) { }
#यदि_अन्तः

#यदि_अपरिभाषितम् arch_scale_freq_क्षमता
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
अचिह्नित दीर्घ arch_scale_freq_क्षमता(पूर्णाङ्क cpu)
{
	प्रत्यागम SCHED_क्षमता_SCALE;
}
#यदि_अन्तः

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

#परिभाषय DEFINE_तालकम्_GUARD_2(name, type, तालकम्_2, _unlock, ...)				\
__DEFINE_UNLOCK_GUARD(name, type, _unlock, type *lock2; __VA_ARGS__)			\
स्थिर अन्तःस्थ class_##name##_t class_##name##_constructor(type *तालकम्, type *lock2)	\
	__no_context_analysis								\
{ class_##name##_t _t = { .तालकम् = तालकम्, .lock2 = lock2 }, *_T = &_t;			\
  तालकम्_2; प्रत्यागम _t; }
#परिभाषय DECLARE_तालकम्_GUARD_2_ATTRS(_name, तालकम्_2, _unlock1, _unlock2)			\
स्थिर अन्तःस्थ class_##_name##_t class_##_name##_constructor(तालकम्_3##_name##_t *_T1,	\
							    तालकम्_3##_name##_t *_T2) तालकम्_2; \
स्थिर __always_inline शून्य __class_##_name##_cleanup_ctx1(class_##_name##_t **_T1)	\
	__no_context_analysis _unlock1 { }						\
स्थिर __always_inline शून्य __class_##_name##_cleanup_ctx2(class_##_name##_t **_T2)	\
	__no_context_analysis _unlock2 { }
#परिभाषय WITH_तालकम्_GUARD_2_ATTRS(_name, _T1, _T2)					\
	class_##_name##_constructor(_T1, _T2),						\
	*__UNIQUE_ID(unlock1) __cleanup(__class_##_name##_cleanup_ctx1) = (शून्य *)(_T1),\
	*__UNIQUE_ID(unlock2) __cleanup(__class_##_name##_cleanup_ctx2) = (शून्य *)(_T2)

स्थिर अन्तःस्थ bool rq_क्रमः_less(संरचना rq *rq1, संरचना rq *rq2)
{
#यदि_परिभाषितम् CONFIG_SCHED_CORE
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
	यदि (rq1->core->cpu < rq2->core->cpu)
		प्रत्यागम true;
	यदि (rq1->core->cpu > rq2->core->cpu)
		प्रत्यागम false;

	/*
	 * __sched_core_flip() relies on SMT having cpu-id lock order.
	 */
#यदि_अन्तः /* CONFIG_SCHED_CORE */
	प्रत्यागम rq1->cpu < rq2->cpu;
}

बाह्य शून्य double_rq_तालकम्(संरचना rq *rq1, संरचना rq *rq2)
	__acquires(__rq_lockp(rq1), __rq_lockp(rq2));

#यदि_परिभाषितम् CONFIG_PREEMPTION

/*
 * fair double_lock_balance: Safely acquires both rq->locks in a fair
 * way at the expense of forcing extra atomic operations in all
 * invocations.  This assures that the double_lock is acquired using the
 * same underlying policy as the spinlock_t on this architecture, which
 * reduces latency compared to the unfair variant below.  However, it
 * also adds more overhead and therefore may reduce throughput.
 */
स्थिर अन्तःस्थ पूर्णाङ्क double_तालकम्_balance_2(संरचना rq *this_rq, संरचना rq *busiest)
	__must_hold(__rq_lockp(this_rq))
	__acquires(__rq_lockp(busiest))
{
	raw_spin_rq_unlock(this_rq);
	double_rq_तालकम्(this_rq, busiest);

	प्रत्यागम 1;
}

#अन्यथा /* !CONFIG_PREEMPTION: */
/*
 * Unfair double_lock_balance: Optimizes throughput at the expense of
 * latency by eliminating extra atomic operations when the locks are
 * already in proper order on entry.  This favors lower CPU-ids and will
 * grant the double lock to lower CPUs over higher ids under contention,
 * regardless of entry order into the function.
 */
स्थिर अन्तःस्थ पूर्णाङ्क double_तालकम्_balance_2(संरचना rq *this_rq, संरचना rq *busiest)
	__must_hold(__rq_lockp(this_rq))
	__acquires(__rq_lockp(busiest))
{
	यदि (__rq_lockp(this_rq) == __rq_lockp(busiest)) {
		__acquire(__rq_lockp(busiest)); /* already held */
		double_rq_clock_clear_update(this_rq, busiest);
		प्रत्यागम 0;
	}

	यदि (likely(raw_spin_rq_trylock(busiest))) {
		double_rq_clock_clear_update(this_rq, busiest);
		प्रत्यागम 0;
	}

	यदि (rq_क्रमः_less(this_rq, busiest)) {
		raw_spin_rq_तालकम्_nested(busiest, SINGLE_गभीरता_NESTING);
		double_rq_clock_clear_update(this_rq, busiest);
		प्रत्यागम 0;
	}

	raw_spin_rq_unlock(this_rq);
	double_rq_तालकम्(this_rq, busiest);

	प्रत्यागम 1;
}

#यदि_अन्तः /* !CONFIG_PREEMPTION */

/*
 * double_lock_balance - lock the busiest runqueue, this_rq is locked already.
 */
स्थिर अन्तःस्थ पूर्णाङ्क double_तालकम्_balance(संरचना rq *this_rq, संरचना rq *busiest)
	__must_hold(__rq_lockp(this_rq))
	__acquires(__rq_lockp(busiest))
{
	lockdep_assert_irqs_disabled();

	प्रत्यागम double_तालकम्_balance_2(this_rq, busiest);
}

स्थिर अन्तःस्थ शून्य double_unlock_balance(संरचना rq *this_rq, संरचना rq *busiest)
	__releases(__rq_lockp(busiest))
{
	यदि (__rq_lockp(this_rq) != __rq_lockp(busiest))
		raw_spin_rq_unlock(busiest);
	अन्यथा
		__release(__rq_lockp(busiest)); /* fake release */
	तालकम्_समुच्चयः_subclass(&__rq_lockp(this_rq)->dep_map, 0, _RET_IP_);
}

स्थिर अन्तःस्थ शून्य double_तालकम्(spinlock_t *l1, spinlock_t *l2)
	__acquires(l1, l2)
{
	यदि (l1 > l2)
		swap(l1, l2);

	spin_तालकम्(l1);
	spin_तालकम्_nested(l2, SINGLE_गभीरता_NESTING);
}

स्थिर अन्तःस्थ शून्य double_तालकम्_irq(spinlock_t *l1, spinlock_t *l2)
	__acquires(l1, l2)
{
	यदि (l1 > l2)
		swap(l1, l2);

	spin_तालकम्_irq(l1);
	spin_तालकम्_nested(l2, SINGLE_गभीरता_NESTING);
}

स्थिर अन्तःस्थ शून्य double_raw_तालकम्(raw_spinlock_t *l1, raw_spinlock_t *l2)
	__acquires(l1, l2)
{
	यदि (l1 > l2)
		swap(l1, l2);

	raw_spin_तालकम्(l1);
	raw_spin_तालकम्_nested(l2, SINGLE_गभीरता_NESTING);
}

स्थिर अन्तःस्थ शून्य double_raw_unlock(raw_spinlock_t *l1, raw_spinlock_t *l2)
	__releases(l1, l2)
{
	raw_spin_unlock(l1);
	raw_spin_unlock(l2);
}

DEFINE_तालकम्_GUARD_2(double_raw_spinlock, raw_spinlock_t,
		    double_raw_तालकम्(_T->तालकम्, _T->lock2),
		    double_raw_unlock(_T->तालकम्, _T->lock2))

DECLARE_तालकम्_GUARD_2_ATTRS(double_raw_spinlock,
			   __acquires(_T1, _T2),
			   __releases(*(raw_spinlock_t **)_T1),
			   __releases(*(raw_spinlock_t **)_T2));
#परिभाषय class_double_raw_spinlock_constructor(_T1, _T2) \
	WITH_तालकम्_GUARD_2_ATTRS(double_raw_spinlock, _T1, _T2)

/*
 * double_rq_unlock - safely unlock two runqueues
 *
 * Note this does not restore interrupts like task_rq_unlock,
 * you need to do so manually after calling.
 */
स्थिर अन्तःस्थ शून्य double_rq_unlock(संरचना rq *rq1, संरचना rq *rq2)
	__releases(__rq_lockp(rq1), __rq_lockp(rq2))
{
	यदि (__rq_lockp(rq1) != __rq_lockp(rq2))
		raw_spin_rq_unlock(rq2);
	अन्यथा
		__release(__rq_lockp(rq2)); /* fake release */
	raw_spin_rq_unlock(rq1);
}

बाह्य शून्य समुच्चयः_rq_online (संरचना rq *rq);
बाह्य शून्य समुच्चयः_rq_offline(संरचना rq *rq);

बाह्य bool sched_smp_initialized;

DEFINE_तालकम्_GUARD_2(double_rq_तालकम्, संरचना rq,
		    double_rq_तालकम्(_T->तालकम्, _T->lock2),
		    double_rq_unlock(_T->तालकम्, _T->lock2))

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

#यदि_परिभाषितम् CONFIG_NUMA_BALANCING
बाह्य शून्य show_numa_stats(संरचना task_struct *p, संरचना seq_file *m);
बाह्य शून्य
print_numa_stats(संरचना seq_file *m, पूर्णाङ्क ग्रन्थिः, अचिह्नित दीर्घ tsf,
		 अचिह्नित दीर्घ tpf, अचिह्नित दीर्घ gsf, अचिह्नित दीर्घ gpf);
#यदि_अन्तः /* CONFIG_NUMA_BALANCING */

बाह्य शून्य init_cfs_rq(संरचना cfs_rq *cfs_rq);
बाह्य शून्य init_rt_rq(संरचना rt_rq *rt_rq);
बाह्य शून्य init_dl_rq(संरचना dl_rq *dl_rq);

बाह्य शून्य cfs_bandwidth_usage_inc(शून्य);
बाह्य शून्य cfs_bandwidth_usage_dec(शून्य);

#यदि_परिभाषितम् CONFIG_NO_HZ_COMMON

#परिभाषय NOHZ_BALANCE_KICK_BIT	0
#परिभाषय NOHZ_STATS_KICK_BIT	1
#परिभाषय NOHZ_NEWILB_KICK_BIT	2
#परिभाषय NOHZ_अग्र_KICK_BIT	3

/* Run sched_balance_domains() */
#परिभाषय NOHZ_BALANCE_KICK	BIT(NOHZ_BALANCE_KICK_BIT)
/* Update blocked load */
#परिभाषय NOHZ_STATS_KICK		BIT(NOHZ_STATS_KICK_BIT)
/* Update blocked load when entering idle */
#परिभाषय NOHZ_NEWILB_KICK	BIT(NOHZ_NEWILB_KICK_BIT)
/* Update nohz.next_balance */
#परिभाषय NOHZ_अग्र_KICK		BIT(NOHZ_अग्र_KICK_BIT)

#परिभाषय NOHZ_KICK_आवरणम्		(NOHZ_BALANCE_KICK | NOHZ_STATS_KICK | NOHZ_अग्र_KICK)

#परिभाषय nohz_flags(cpu)		(&cpu_rq(cpu)->nohz_flags)

बाह्य शून्य nohz_balance_exit_idle(संरचना rq *rq);
#अन्यथा /* !CONFIG_NO_HZ_COMMON: */
स्थिर अन्तःस्थ शून्य nohz_balance_exit_idle(संरचना rq *rq) { }
#यदि_अन्तः /* !CONFIG_NO_HZ_COMMON */

#यदि_परिभाषितम् CONFIG_NO_HZ_COMMON
बाह्य शून्य nohz_सततम्_idle_balance(पूर्णाङ्क cpu);
#अन्यथा
स्थिर अन्तःस्थ शून्य nohz_सततम्_idle_balance(पूर्णाङ्क cpu) { }
#यदि_अन्तः

#अन्तर्भाव "stats.h"

#यदि defined(CONFIG_SCHED_CORE) && defined(CONFIG_SCHEDSTATS)

बाह्य शून्य __sched_core_account_forceidle(संरचना rq *rq);

स्थिर अन्तःस्थ शून्य sched_core_account_forceidle(संरचना rq *rq)
{
	यदि (schedstat_enabled())
		__sched_core_account_forceidle(rq);
}

बाह्य शून्य __sched_core_tick(संरचना rq *rq);

स्थिर अन्तःस्थ शून्य sched_core_tick(संरचना rq *rq)
{
	यदि (sched_core_enabled(rq) && schedstat_enabled())
		__sched_core_tick(rq);
}

#अन्यथा /* !(CONFIG_SCHED_CORE && CONFIG_SCHEDSTATS): */

स्थिर अन्तःस्थ शून्य sched_core_account_forceidle(संरचना rq *rq) { }

स्थिर अन्तःस्थ शून्य sched_core_tick(संरचना rq *rq) { }

#यदि_अन्तः /* !(CONFIG_SCHED_CORE && CONFIG_SCHEDSTATS) */

#यदि_परिभाषितम् CONFIG_IRQ_TIME_ACCOUNTING

संरचना irqtime {
	u64			योग_2;
	u64			tick_delta;
	u64			irq_start_time;
	संरचना u64_stats_sync	sync;
};

DECLARE_प्रत्येकम्_CPU(संरचना irqtime, cpu_irqtime);
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
	संरचना irqtime *irqtime = &प्रत्येकम्_cpu(cpu_irqtime, cpu);
	अचिह्नित पूर्णाङ्क seq;
	u64 योग_2;

	कुरु {
		seq = __u64_stats_fetch_begin(&irqtime->sync);
		योग_2 = irqtime->योग_2;
	} यावत् (__u64_stats_fetch_retry(&irqtime->sync, seq));

	प्रत्यागम योग_2;
}

#अन्यथा /* !CONFIG_IRQ_TIME_ACCOUNTING: */

स्थिर अन्तःस्थ पूर्णाङ्क irqtime_enabled(शून्य)
{
	प्रत्यागम 0;
}

#यदि_अन्तः /* !CONFIG_IRQ_TIME_ACCOUNTING */

#यदि_परिभाषितम् CONFIG_CPU_FREQ

DECLARE_प्रत्येकम्_CPU(संरचना update_util_दत्त __rcu *, cpufreq_update_util_दत्त);

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
स्थिर अन्तःस्थ शून्य cpufreq_update_util(संरचना rq *rq, अचिह्नित पूर्णाङ्क flags)
{
	संरचना update_util_दत्त *दत्त;

	दत्त = rcu_dereference_sched(*प्रत्येकम्_cpu_ptr(&cpufreq_update_util_दत्त,
						  cpu_सम्बन्धि(rq)));
	यदि (दत्त)
		दत्त->func(दत्त, rq_clock(rq), flags);
}
#अन्यथा /* !CONFIG_CPU_FREQ: */
स्थिर अन्तःस्थ शून्य cpufreq_update_util(संरचना rq *rq, अचिह्नित पूर्णाङ्क flags) { }
#यदि_अन्तः /* !CONFIG_CPU_FREQ */

#यदि_परिभाषितम् arch_scale_freq_क्षमता
# ifndef arch_scale_freq_invariant
#  define arch_scale_freq_invariant()	true
# endif
#अन्यथा
# define arch_scale_freq_invariant()	false
#यदि_अन्तः

अचिह्नित दीर्घ effective_cpu_util(पूर्णाङ्क cpu, अचिह्नित दीर्घ util_cfs,
				 अचिह्नित दीर्घ *min,
				 अचिह्नित दीर्घ *max);

अचिह्नित दीर्घ sugov_effective_cpu_perf(पूर्णाङ्क cpu, अचिह्नित दीर्घ actual,
				 अचिह्नित दीर्घ min,
				 अचिह्नित दीर्घ max);


/*
 * Verify the fitness of task @p to run on @cpu taking into account the
 * CPU original capacity and the runtime/deadline ratio of the task.
 *
 * The function will return true if the original capacity of @cpu is
 * greater than or equal to task's deadline density right shifted by
 * (BW_SHIFT - SCHED_CAPACITY_SHIFT) and false otherwise.
 */
स्थिर अन्तःस्थ bool dl_task_माति_क्षमता(संरचना task_struct *p, पूर्णाङ्क cpu)
{
	अचिह्नित दीर्घ cap = arch_scale_cpu_क्षमता(cpu);

	प्रत्यागम cap >= p->dl.dl_density >> (BW_सरणम् - SCHED_क्षमता_सरणम्);
}

स्थिर अन्तःस्थ अचिह्नित दीर्घ cpu_bw_dl(संरचना rq *rq)
{
	प्रत्यागम (rq->dl.running_bw * SCHED_क्षमता_SCALE) >> BW_सरणम्;
}

स्थिर अन्तःस्थ अचिह्नित दीर्घ cpu_util_dl(संरचना rq *rq)
{
	प्रत्यागम READ_ONCE(rq->avg_dl.util_avg);
}


बाह्य अचिह्नित दीर्घ cpu_util_cfs(पूर्णाङ्क cpu);
बाह्य अचिह्नित दीर्घ cpu_util_cfs_boost(पूर्णाङ्क cpu);

स्थिर अन्तःस्थ अचिह्नित दीर्घ cpu_util_rt(संरचना rq *rq)
{
	प्रत्यागम READ_ONCE(rq->avg_rt.util_avg);
}

#यदि_परिभाषितम् CONFIG_UCLAMP_TASK

अचिह्नित दीर्घ uclamp_eff_मानम्(संरचना task_struct *p, गणना uclamp_id clamp_id);

/*
 * When uclamp is compiled in, the aggregation at rq level is 'turned off'
 * by default in the fast path and only gets turned on once userspace performs
 * an operation that requires it.
 *
 * Returns true if userspace opted-in to use uclamp and aggregation at rq level
 * hence is active.
 */
स्थिर अन्तःस्थ bool uclamp_is_प्रयुक्तम्(शून्य)
{
	प्रत्यागम static_branch_likely(&sched_uclamp_प्रयुक्तम्);
}

/*
 * Enabling static branches would get the cpus_read_lock(),
 * check whether uclamp_is_used before enable it to avoid always
 * calling cpus_read_lock(). Because we never disable this
 * static key once enable it.
 */
स्थिर अन्तःस्थ शून्य sched_uclamp_enable(शून्य)
{
	यदि (!uclamp_is_प्रयुक्तम्())
		static_branch_enable(&sched_uclamp_प्रयुक्तम्);
}

स्थिर अन्तःस्थ अचिह्नित दीर्घ uclamp_rq_get(संरचना rq *rq,
					  गणना uclamp_id clamp_id)
{
	प्रत्यागम READ_ONCE(rq->uclamp[clamp_id].मानम्);
}

स्थिर अन्तःस्थ शून्य uclamp_rq_समुच्चयः(संरचना rq *rq, गणना uclamp_id clamp_id,
				 अचिह्नित पूर्णाङ्क मानम्)
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
	अचिह्नित दीर्घ rq_util;
	अचिह्नित दीर्घ max_util;

	यदि (!uclamp_is_प्रयुक्तम्())
		प्रत्यागम false;

	rq_util = cpu_util_cfs(cpu_सम्बन्धि(rq)) + cpu_util_rt(rq);
	max_util = READ_ONCE(rq->uclamp[UCLAMP_MAX].मानम्);

	प्रत्यागम max_util != SCHED_क्षमता_SCALE && rq_util >= max_util;
}

#परिभाषय for_each_clamp_id(clamp_id) \
	प्रति ((clamp_id) = 0; (clamp_id) < UCLAMP_CNT; (clamp_id)++)

बाह्य अचिह्नित पूर्णाङ्क sysctl_sched_uclamp_util_min_rt_default;


स्थिर अन्तःस्थ अचिह्नित पूर्णाङ्क uclamp_none(गणना uclamp_id clamp_id)
{
	यदि (clamp_id == UCLAMP_MIN)
		प्रत्यागम 0;
	प्रत्यागम SCHED_क्षमता_SCALE;
}

/* Integer rounded range for each bucket */
#परिभाषय UCLAMP_BUCKET_DELTA DIV_आवर्तनम्_CLOSEST(SCHED_क्षमता_SCALE, UCLAMP_BUCKETS)

स्थिर अन्तःस्थ अचिह्नित पूर्णाङ्क uclamp_bucket_id(अचिह्नित पूर्णाङ्क clamp_मानम्)
{
	प्रत्यागम min_t(अचिह्नित पूर्णाङ्क, clamp_मानम् / UCLAMP_BUCKET_DELTA, UCLAMP_BUCKETS - 1);
}

स्थिर अन्तःस्थ शून्य
uclamp_se_समुच्चयः(संरचना uclamp_se *uc_se, अचिह्नित पूर्णाङ्क मानम्, bool user_defined)
{
	uc_se->मानम् = मानम्;
	uc_se->bucket_id = uclamp_bucket_id(मानम्);
	uc_se->user_defined = user_defined;
}

#अन्यथा /* !CONFIG_UCLAMP_TASK: */

स्थिर अन्तःस्थ अचिह्नित दीर्घ
uclamp_eff_मानम्(संरचना task_struct *p, गणना uclamp_id clamp_id)
{
	यदि (clamp_id == UCLAMP_MIN)
		प्रत्यागम 0;

	प्रत्यागम SCHED_क्षमता_SCALE;
}

स्थिर अन्तःस्थ bool uclamp_rq_is_capped(संरचना rq *rq) { प्रत्यागम false; }

स्थिर अन्तःस्थ bool uclamp_is_प्रयुक्तम्(शून्य)
{
	प्रत्यागम false;
}

स्थिर अन्तःस्थ शून्य sched_uclamp_enable(शून्य) {}

स्थिर अन्तःस्थ अचिह्नित दीर्घ
uclamp_rq_get(संरचना rq *rq, गणना uclamp_id clamp_id)
{
	यदि (clamp_id == UCLAMP_MIN)
		प्रत्यागम 0;

	प्रत्यागम SCHED_क्षमता_SCALE;
}

स्थिर अन्तःस्थ शून्य
uclamp_rq_समुच्चयः(संरचना rq *rq, गणना uclamp_id clamp_id, अचिह्नित पूर्णाङ्क मानम्)
{
}

स्थिर अन्तःस्थ bool uclamp_rq_is_idle(संरचना rq *rq)
{
	प्रत्यागम false;
}

#यदि_अन्तः /* !CONFIG_UCLAMP_TASK */

#यदि_परिभाषितम् CONFIG_HAVE_SCHED_AVG_IRQ

स्थिर अन्तःस्थ अचिह्नित दीर्घ cpu_util_irq(संरचना rq *rq)
{
	प्रत्यागम READ_ONCE(rq->avg_irq.util_avg);
}

स्थिर अन्तःस्थ
अचिह्नित दीर्घ scale_irq_क्षमता(अचिह्नित दीर्घ util, अचिह्नित दीर्घ irq, अचिह्नित दीर्घ max)
{
	util *= (max - irq);
	util /= max;

	प्रत्यागम util;

}

#अन्यथा /* !CONFIG_HAVE_SCHED_AVG_IRQ: */

स्थिर अन्तःस्थ अचिह्नित दीर्घ cpu_util_irq(संरचना rq *rq)
{
	प्रत्यागम 0;
}

स्थिर अन्तःस्थ
अचिह्नित दीर्घ scale_irq_क्षमता(अचिह्नित दीर्घ util, अचिह्नित दीर्घ irq, अचिह्नित दीर्घ max)
{
	प्रत्यागम util;
}

#यदि_अन्तः /* !CONFIG_HAVE_SCHED_AVG_IRQ */

बाह्य शून्य __setparam_fair(संरचना task_struct *p, ध्रुव संरचना sched_attr *attr);

#यदि defined(CONFIG_ENERGY_MODEL) && defined(CONFIG_CPU_FREQ_GOV_SCHEDUTIL)

#परिभाषय perf_domain_विस्तृतिः(pd) (गन्तव्यम्_cpumask(((pd)->em_pd->cpus)))

DECLARE_STATIC_कुञ्चिका_FALSE(sched_energy_present);

स्थिर अन्तःस्थ bool sched_energy_enabled(शून्य)
{
	प्रत्यागम static_branch_unlikely(&sched_energy_present);
}

#अन्यथा /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL): */

#परिभाषय perf_domain_विस्तृतिः(pd) रिक्त

स्थिर अन्तःस्थ bool sched_energy_enabled(शून्य) { प्रत्यागम false; }

#यदि_अन्तः /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL) */

#यदि_परिभाषितम् CONFIG_MEMBARRIER

/*
 * The scheduler provides memory barriers required by membarrier between:
 * - prior user-space memory accesses and store to rq->membarrier_state,
 * - store to rq->membarrier_state and following user-space memory accesses.
 * In the same way it provides those guarantees around store to rq->curr.
 */
स्थिर अन्तःस्थ शून्य membarrier_switch_mm(संरचना rq *rq,
					संरचना mm_struct *पूर्व_mm,
					संरचना mm_struct *अग्र_mm)
{
	पूर्णाङ्क membarrier_state;

	यदि (पूर्व_mm == अग्र_mm)
		प्रत्यागम;

	membarrier_state = atomic_read(&अग्र_mm->membarrier_state);
	यदि (READ_ONCE(rq->membarrier_state) == membarrier_state)
		प्रत्यागम;

	WRITE_ONCE(rq->membarrier_state, membarrier_state);
}

#अन्यथा /* !CONFIG_MEMBARRIER: */

स्थिर अन्तःस्थ शून्य membarrier_switch_mm(संरचना rq *rq,
					संरचना mm_struct *पूर्व_mm,
					संरचना mm_struct *अग्र_mm)
{
}

#यदि_अन्तः /* !CONFIG_MEMBARRIER */

स्थिर अन्तःस्थ bool is_प्रत्येकम्_cpu_kthread(संरचना task_struct *p)
{
	यदि (!(p->flags & PF_KTHREAD))
		प्रत्यागम false;

	यदि (p->nr_cpus_allowed != 1)
		प्रत्यागम false;

	प्रत्यागम true;
}

बाह्य शून्य swake_ऊर्ध्वम्_all_locked(संरचना swait_queue_शीर्ष *q);
बाह्य शून्य __prepare_to_swait(संरचना swait_queue_शीर्ष *q, संरचना swait_queue *प्रतीक्षस्व);

बाह्य पूर्णाङ्क try_गन्तव्यम्_wake_ऊर्ध्वम्(संरचना task_struct *tsk, अचिह्नित पूर्णाङ्क state, पूर्णाङ्क wake_flags);

#यदि_परिभाषितम् CONFIG_PREEMPT_DYNAMIC
बाह्य पूर्णाङ्क preempt_dynamic_बहुलकम्;
बाह्य पूर्णाङ्क sched_dynamic_बहुलकम्(ध्रुव वर्ण *str);
बाह्य शून्य sched_dynamic_update(पूर्णाङ्क बहुलकम्);
#यदि_अन्तः
बाह्य ध्रुव वर्ण *preempt_modes[];

#यदि_परिभाषितम् CONFIG_SCHED_MM_CID

स्थिर __always_inline bool cid_on_cpu(अचिह्नित पूर्णाङ्क cid)
{
	प्रत्यागम cid & MM_CID_ONCPU;
}

स्थिर __always_inline bool cid_अन्तर्_transit(अचिह्नित पूर्णाङ्क cid)
{
	प्रत्यागम cid & MM_CID_TRANSIT;
}

स्थिर __always_inline अचिह्नित पूर्णाङ्क cpu_cid_गन्तव्यम्_cid(अचिह्नित पूर्णाङ्क cid)
{
	प्रत्यागम cid & ~MM_CID_ONCPU;
}

स्थिर __always_inline अचिह्नित पूर्णाङ्क cid_गन्तव्यम्_cpu_cid(अचिह्नित पूर्णाङ्क cid)
{
	प्रत्यागम cid | MM_CID_ONCPU;
}

स्थिर __always_inline अचिह्नित पूर्णाङ्क cid_गन्तव्यम्_transit_cid(अचिह्नित पूर्णाङ्क cid)
{
	प्रत्यागम cid | MM_CID_TRANSIT;
}

स्थिर __always_inline अचिह्नित पूर्णाङ्क cid_from_transit_cid(अचिह्नित पूर्णाङ्क cid)
{
	प्रत्यागम cid & ~MM_CID_TRANSIT;
}

स्थिर __always_inline bool cid_on_task(अचिह्नित पूर्णाङ्क cid)
{
	/* True if none of the MM_CID_ONCPU, MM_CID_TRANSIT, MM_CID_UNSET bits is set */
	प्रत्यागम cid < MM_CID_TRANSIT;
}

स्थिर __always_inline शून्य mm_drop_cid(संरचना mm_struct *mm, अचिह्नित पूर्णाङ्क cid)
{
	clear_bit(cid, mm_cidmask(mm));
}

स्थिर __always_inline शून्य mm_unset_cid_on_task(संरचना task_struct *t)
{
	अचिह्नित पूर्णाङ्क cid = t->mm_cid.cid;

	t->mm_cid.cid = MM_CID_UNSET;
	यदि (cid_on_task(cid))
		mm_drop_cid(t->mm, cid);
}

स्थिर __always_inline शून्य mm_drop_cid_on_cpu(संरचना mm_struct *mm, संरचना mm_cid_pcpu *pcp)
{
	/* Clear the ONCPU bit, but do not set UNSET in the per CPU storage */
	यदि (cid_on_cpu(pcp->cid)) {
		pcp->cid = cpu_cid_गन्तव्यम्_cid(pcp->cid);
		mm_drop_cid(mm, pcp->cid);
	}
}

स्थिर अन्तःस्थ अचिह्नित पूर्णाङ्क __mm_get_cid(संरचना mm_struct *mm, अचिह्नित पूर्णाङ्क max_cids)
{
	अचिह्नित पूर्णाङ्क cid = find_first_zero_bit(mm_cidmask(mm), max_cids);

	यदि (cid >= max_cids)
		प्रत्यागम MM_CID_UNSET;
	यदि (test_and_समुच्चयः_bit(cid, mm_cidmask(mm)))
		प्रत्यागम MM_CID_UNSET;
	प्रत्यागम cid;
}

स्थिर अन्तःस्थ अचिह्नित पूर्णाङ्क mm_get_cid(संरचना mm_struct *mm)
{
	अचिह्नित पूर्णाङ्क cid = __mm_get_cid(mm, READ_ONCE(mm->mm_cid.max_cids));

	यावत् (cid == MM_CID_UNSET) {
		cpu_relax();
		cid = __mm_get_cid(mm, अङ्कः_possible_cpus());
	}
	प्रत्यागम cid;
}

स्थिर अन्तःस्थ अचिह्नित पूर्णाङ्क mm_cid_converge(संरचना mm_struct *mm, अचिह्नित पूर्णाङ्क orig_cid,
					   अचिह्नित पूर्णाङ्क max_cids)
{
	अचिह्नित पूर्णाङ्क new_cid, cid = cpu_cid_गन्तव्यम्_cid(orig_cid);

	/* Is it in the optimal CID space? */
	यदि (likely(cid < max_cids))
		प्रत्यागम orig_cid;

	/* Try to find one in the optimal space. Otherwise keep the provided. */
	new_cid = __mm_get_cid(mm, max_cids);
	यदि (new_cid != MM_CID_UNSET) {
		mm_drop_cid(mm, cid);
		/* Preserve the ONCPU mode of the original CID */
		प्रत्यागम new_cid | (orig_cid & MM_CID_ONCPU);
	}
	प्रत्यागम orig_cid;
}

स्थिर __always_inline शून्य mm_cid_update_task_cid(संरचना task_struct *t, अचिह्नित पूर्णाङ्क cid)
{
	यदि (t->mm_cid.cid != cid) {
		t->mm_cid.cid = cid;
		rseq_sched_समुच्चयः_ids_परिवर्तितम्(t);
	}
}

स्थिर __always_inline शून्य mm_cid_update_pcpu_cid(संरचना mm_struct *mm, अचिह्नित पूर्णाङ्क cid)
{
	__this_cpu_write(mm->mm_cid.pcpu->cid, cid);
}

स्थिर __always_inline शून्य mm_cid_from_cpu(संरचना task_struct *t, अचिह्नित पूर्णाङ्क cpu_cid,
					    अचिह्नित पूर्णाङ्क बहुलकम्)
{
	अचिह्नित पूर्णाङ्क max_cids, tcid = t->mm_cid.cid;
	संरचना mm_struct *mm = t->mm;

	max_cids = READ_ONCE(mm->mm_cid.max_cids);
	/* Optimize for the common case where both have the ONCPU bit set */
	यदि (likely(cid_on_cpu(cpu_cid & tcid))) {
		यदि (likely(cpu_cid_गन्तव्यम्_cid(cpu_cid) < max_cids)) {
			mm_cid_update_task_cid(t, cpu_cid);
			प्रत्यागम;
		}
		/* Try to converge into the optimal CID space */
		cpu_cid = mm_cid_converge(mm, cpu_cid, max_cids);
	} अन्यथा {
		/* Hand over or drop the task owned CID */
		यदि (cid_on_task(tcid)) {
			यदि (cid_on_cpu(cpu_cid))
				mm_unset_cid_on_task(t);
			अन्यथा
				cpu_cid = cid_गन्तव्यम्_cpu_cid(tcid);
		}
		/* Still nothing, allocate a new one */
		यदि (!cid_on_cpu(cpu_cid))
			cpu_cid = cid_गन्तव्यम्_cpu_cid(mm_get_cid(mm));

		/* Handle the transition mode flag if required */
		यदि (बहुलकम् & MM_CID_TRANSIT)
			cpu_cid = cpu_cid_गन्तव्यम्_cid(cpu_cid) | MM_CID_TRANSIT;
	}
	mm_cid_update_pcpu_cid(mm, cpu_cid);
	mm_cid_update_task_cid(t, cpu_cid);
}

स्थिर __always_inline शून्य mm_cid_from_task(संरचना task_struct *t, अचिह्नित पूर्णाङ्क cpu_cid,
					     अचिह्नित पूर्णाङ्क बहुलकम्)
{
	अचिह्नित पूर्णाङ्क max_cids, tcid = t->mm_cid.cid;
	संरचना mm_struct *mm = t->mm;

	max_cids = READ_ONCE(mm->mm_cid.max_cids);
	/* Optimize for the common case, where both have the ONCPU bit clear */
	यदि (likely(cid_on_task(tcid | cpu_cid))) {
		यदि (likely(tcid < max_cids)) {
			mm_cid_update_pcpu_cid(mm, tcid);
			प्रत्यागम;
		}
		/* Try to converge into the optimal CID space */
		tcid = mm_cid_converge(mm, tcid, max_cids);
	} अन्यथा {
		/* Hand over or drop the CPU owned CID */
		यदि (cid_on_cpu(cpu_cid)) {
			यदि (cid_on_task(tcid))
				mm_drop_cid_on_cpu(mm, this_cpu_ptr(mm->mm_cid.pcpu));
			अन्यथा
				tcid = cpu_cid_गन्तव्यम्_cid(cpu_cid);
		}
		/* Still nothing, allocate a new one */
		यदि (!cid_on_task(tcid))
			tcid = mm_get_cid(mm);
		/* Set the transition mode flag if required */
		tcid |= बहुलकम् & MM_CID_TRANSIT;
	}
	mm_cid_update_pcpu_cid(mm, tcid);
	mm_cid_update_task_cid(t, tcid);
}

स्थिर __always_inline शून्य mm_cid_schedin(संरचना task_struct *अग्र)
{
	संरचना mm_struct *mm = अग्र->mm;
	अचिह्नित पूर्णाङ्क cpu_cid, बहुलकम्;

	यदि (!अग्र->mm_cid.सक्रिय)
		प्रत्यागम;

	cpu_cid = __this_cpu_read(mm->mm_cid.pcpu->cid);
	बहुलकम् = READ_ONCE(mm->mm_cid.बहुलकम्);
	यदि (likely(!cid_on_cpu(बहुलकम्)))
		mm_cid_from_task(अग्र, cpu_cid, बहुलकम्);
	अन्यथा
		mm_cid_from_cpu(अग्र, cpu_cid, बहुलकम्);
}

स्थिर __always_inline शून्य mm_cid_schedout(संरचना task_struct *पूर्व)
{
	संरचना mm_struct *mm = पूर्व->mm;
	अचिह्नित पूर्णाङ्क बहुलकम्, cid;

	/* During mode transitions CIDs are temporary and need to be dropped */
	यदि (likely(!cid_अन्तर्_transit(पूर्व->mm_cid.cid)))
		प्रत्यागम;

	बहुलकम् = READ_ONCE(mm->mm_cid.बहुलकम्);
	cid = cid_from_transit_cid(पूर्व->mm_cid.cid);

	/*
	 * If transition mode is done, transfer ownership when the CID is
	 * within the convergence range to optimize the next schedule in.
	 */
	यदि (!cid_अन्तर्_transit(बहुलकम्) && cid < READ_ONCE(mm->mm_cid.max_cids)) {
		यदि (cid_on_cpu(बहुलकम्))
			cid = cid_गन्तव्यम्_cpu_cid(cid);

		/* Update both so that the next schedule in goes into the fast path */
		mm_cid_update_pcpu_cid(mm, cid);
		पूर्व->mm_cid.cid = cid;
	} अन्यथा {
		mm_drop_cid(mm, cid);
		पूर्व->mm_cid.cid = MM_CID_UNSET;
	}
}

स्थिर अन्तःस्थ शून्य mm_cid_switch_गन्तव्यम्(संरचना task_struct *पूर्व, संरचना task_struct *अग्र)
{
	mm_cid_schedout(पूर्व);
	mm_cid_schedin(अग्र);
}

#अन्यथा /* !CONFIG_SCHED_MM_CID: */
स्थिर अन्तःस्थ शून्य mm_cid_switch_गन्तव्यम्(संरचना task_struct *पूर्व, संरचना task_struct *अग्र) { }
#यदि_अन्तः /* !CONFIG_SCHED_MM_CID */

#यदि_परिभाषितम् CONFIG_SCHED_CACHE
DECLARE_STATIC_कुञ्चिका_FALSE(sched_cache_present);
DECLARE_STATIC_कुञ्चिका_FALSE(sched_cache_सक्रिय);
बाह्य पूर्णाङ्क sysctl_sched_cache_user;
बाह्य अचिह्नित पूर्णाङ्क llc_aggr_tolerance;
बाह्य अचिह्नित पूर्णाङ्क llc_epoch_period;
बाह्य अचिह्नित पूर्णाङ्क llc_epoch_affinity_timeout;
बाह्य अचिह्नित पूर्णाङ्क llc_imb_pct;
बाह्य अचिह्नित पूर्णाङ्क llc_overaggr_pct;

स्थिर अन्तःस्थ bool sched_cache_enabled(शून्य)
{
	प्रत्यागम static_branch_unlikely(&sched_cache_सक्रिय);
}

बाह्य शून्य sched_cache_सक्रिय_समुच्चयः(शून्य);

#यदि_अन्तः

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
	समुच्चयः_task_cpu(task, dst_rq->cpu);
	activate_task(dst_rq, task, 0);
	wakeup_preempt(dst_rq, task, 0);
}

स्थिर अन्तःस्थ
bool task_is_pushable(संरचना rq *rq, संरचना task_struct *p, पूर्णाङ्क cpu)
{
	यदि (!task_on_cpu(rq, p) &&
	    cpumask_test_cpu(cpu, &p->cpus_आवरणम्))
		प्रत्यागम true;

	प्रत्यागम false;
}

#यदि_परिभाषितम् CONFIG_RT_MUTEXES

स्थिर अन्तःस्थ पूर्णाङ्क __rt_effective_prio(संरचना task_struct *pi_task, पूर्णाङ्क prio)
{
	यदि (pi_task)
		prio = min(prio, pi_task->prio);

	प्रत्यागम prio;
}

स्थिर अन्तःस्थ पूर्णाङ्क rt_effective_prio(संरचना task_struct *p, पूर्णाङ्क prio)
{
	संरचना task_struct *pi_task = rt_mutex_get_शिखरम्_task(p);

	प्रत्यागम __rt_effective_prio(pi_task, prio);
}

#अन्यथा /* !CONFIG_RT_MUTEXES: */

स्थिर अन्तःस्थ पूर्णाङ्क rt_effective_prio(संरचना task_struct *p, पूर्णाङ्क prio)
{
	प्रत्यागम prio;
}

#यदि_अन्तः /* !CONFIG_RT_MUTEXES */

बाह्य पूर्णाङ्क __sched_setscheduler(संरचना task_struct *p, ध्रुव संरचना sched_attr *attr, bool user, bool pi);
बाह्य पूर्णाङ्क __sched_setaffinity(संरचना task_struct *p, संरचना affinity_context *ctx);
बाह्य ध्रुव संरचना sched_class *__setscheduler_class(पूर्णाङ्क policy, पूर्णाङ्क prio);
बाह्य शून्य समुच्चयः_आरोपय_weight(संरचना task_struct *p, bool update_आरोपय);
बाह्य शून्य पङ्क्तौ_निक्षिप_task(संरचना rq *rq, संरचना task_struct *p, पूर्णाङ्क flags);
बाह्य bool पङ्क्तेः_निष्कासय_task(संरचना rq *rq, संरचना task_struct *p, पूर्णाङ्क flags);

बाह्य संरचना balance_callback *splice_balance_callbacks(संरचना rq *rq);

बाह्य शून्य __balance_callbacks(संरचना rq *rq, संरचना rq_flags *rf);
बाह्य शून्य balance_callbacks(संरचना rq *rq, संरचना balance_callback *शीर्ष);

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

संरचना sched_change_ctx *sched_change_begin(संरचना task_struct *p, अचिह्नित पूर्णाङ्क flags);
शून्य sched_change_अन्तः(संरचना sched_change_ctx *ctx);

DEFINE_CLASS(sched_change, संरचना sched_change_ctx *,
	     sched_change_अन्तः(_T),
	     sched_change_begin(p, flags),
	     संरचना task_struct *p, अचिह्नित पूर्णाङ्क flags)

DEFINE_CLASS_IS_UNCONDITIONAL(sched_change)

#अन्तर्भाव "ext/ext.h"

#यदि_परिभाषितम् CONFIG_PREFERRED_CPU
शून्य sched_प्रक्षिप_current_non_preferred_cpu(संरचना rq *rq);
#अन्यथा	/* !CONFIG_PREFERRED_CPU */
स्थिर अन्तःस्थ शून्य sched_प्रक्षिप_current_non_preferred_cpu(संरचना rq *rq) { }
#यदि_अन्तः

#यदि_अन्तः /* _KERNEL_SCHED_SCHED_H */
