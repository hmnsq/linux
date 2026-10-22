/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Scheduler internal types and methods:
 */
#ifndef _KERNEL_SCHED_SCHED_H
#define _KERNEL_SCHED_SCHED_H

#ಅನ್ತರ್ಭಾವ <linux/prandom.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/affinity.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/autogroup.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/cpufreq.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/deadline.h>
#ಅನ್ತರ್ಭಾವ <linux/sched.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/loadavg.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/mm.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/rseq_api.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/signal.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/smt.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/stat.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/sysctl.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/task_flags.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/task.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/topology.h>
#ಅನ್ತರ್ಭಾವ <linux/atomic.h>
#ಅನ್ತರ್ಭಾವ <linux/bitmap.h>
#ಅನ್ತರ್ಭಾವ <linux/bug.h>
#ಅನ್ತರ್ಭಾವ <linux/capability.h>
#ಅನ್ತರ್ಭಾವ <linux/cgroup_api.h>
#ಅನ್ತರ್ಭಾವ <linux/cgroup.h>
#ಅನ್ತರ್ಭಾವ <linux/context_tracking.h>
#ಅನ್ತರ್ಭಾವ <linux/cpufreq.h>
#ಅನ್ತರ್ಭಾವ <linux/cpumask_api.h>
#ಅನ್ತರ್ಭಾವ <linux/cpuset.h>
#ಅನ್ತರ್ಭಾವ <linux/ctype.h>
#ಅನ್ತರ್ಭಾವ <linux/file.h>
#ಅನ್ತರ್ಭಾವ <linux/fs_api.h>
#ಅನ್ತರ್ಭಾವ <linux/hrtimer_api.h>
#ಅನ್ತರ್ಭಾವ <linux/interrupt.h>
#ಅನ್ತರ್ಭಾವ <linux/irq_work.h>
#ಅನ್ತರ್ಭಾವ <linux/jiffies.h>
#ಅನ್ತರ್ಭಾವ <linux/kref_api.h>
#ಅನ್ತರ್ಭಾವ <linux/kthread.h>
#ಅನ್ತರ್ಭಾವ <linux/ktime_api.h>
#ಅನ್ತರ್ಭಾವ <linux/lockdep_api.h>
#ಅನ್ತರ್ಭಾವ <linux/lockdep.h>
#ಅನ್ತರ್ಭಾವ <linux/memblock.h>
#ಅನ್ತರ್ಭಾವ <linux/memcontrol.h>
#ಅನ್ತರ್ಭಾವ <linux/minmax.h>
#ಅನ್ತರ್ಭಾವ <linux/mm.h>
#ಅನ್ತರ್ಭಾವ <linux/module.h>
#ಅನ್ತರ್ಭಾವ <linux/mutex_api.h>
#ಅನ್ತರ್ಭಾವ <linux/plist.h>
#ಅನ್ತರ್ಭಾವ <linux/poll.h>
#ಅನ್ತರ್ಭಾವ <linux/proc_fs.h>
#ಅನ್ತರ್ಭಾವ <linux/profile.h>
#ಅನ್ತರ್ಭಾವ <linux/psi.h>
#ಅನ್ತರ್ಭಾವ <linux/rcupdate.h>
#ಅನ್ತರ್ಭಾವ <linux/seq_file.h>
#ಅನ್ತರ್ಭಾವ <linux/seqlock.h>
#ಅನ್ತರ್ಭಾವ <linux/softirq.h>
#ಅನ್ತರ್ಭಾವ <linux/spinlock_api.h>
#ಅನ್ತರ್ಭಾವ <linux/static_key.h>
#ಅನ್ತರ್ಭಾವ <linux/stop_machine.h>
#ಅನ್ತರ್ಭಾವ <linux/syscalls_api.h>
#ಅನ್ತರ್ಭಾವ <linux/syscalls.h>
#ಅನ್ತರ್ಭಾವ <linux/tick.h>
#ಅನ್ತರ್ಭಾವ <linux/topology.h>
#ಅನ್ತರ್ಭಾವ <linux/types.h>
#ಅನ್ತರ್ಭಾವ <linux/u64_stats_sync_api.h>
#ಅನ್ತರ್ಭಾವ <linux/uaccess.h>
#ಅನ್ತರ್ಭಾವ <linux/vmstat.h>
#ಅನ್ತರ್ಭಾವ <linux/wait_api.h>
#ಅನ್ತರ್ಭಾವ <linux/wait_bit.h>
#ಅನ್ತರ್ಭಾವ <linux/workqueue_api.h>
#ಅನ್ತರ್ಭಾವ <linux/delayacct.h>
#ಅನ್ತರ್ಭಾವ <linux/mmu_context.h>

#ಅನ್ತರ್ಭಾವ <trace/events/power.h>
#ಅನ್ತರ್ಭಾವ <trace/events/sched.h>

#ಅನ್ತರ್ಭಾವ "../workqueue_internal.h"

ಸಂರಚನಾ rq;
ಸಂರಚನಾ cfs_rq;
ಸಂರಚನಾ rt_rq;
ಸಂರಚನಾ sched_group;
ಸಂರಚನಾ cpuidle_state;

#ಯದಿ defined(CONFIG_PARAVIRT) && !defined(CONFIG_HAVE_PV_STEAL_CLOCK_GEN)
# include <asm/paravirt.h>
#endif

#ಅನ್ತರ್ಭಾವ <asm/barrier.h>

#ಅನ್ತರ್ಭಾವ "cpupri.h"
#ಅನ್ತರ್ಭಾವ "cpudeadline.h"

/* task_struct::on_rq states: */
#define TASK_ON_RQ_QUEUED	1
#define TASK_ON_RQ_MIGRATING	2

ಬಾಹ್ಯ __read_mostly ಪೂರ್ಣಾಙ್ಕ scheduler_running;

ಬಾಹ್ಯ unsigned long calc_load_update;
ಬಾಹ್ಯ atomic_long_t calc_load_tasks;

ಬಾಹ್ಯ ಶೂನ್ಯ calc_global_load_tick(ಸಂರಚನಾ rq *this_rq);
ಬಾಹ್ಯ long calc_load_fold_active(ಸಂರಚನಾ rq *this_rq, long adjust);

ಬಾಹ್ಯ ಶೂನ್ಯ call_trace_sched_update_nr_running(ಸಂರಚನಾ rq *rq, ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ);

ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ sysctl_sched_rt_period;
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ sysctl_sched_rt_runtime;
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ sched_rr_timeslice;

/*
 * Asymmetric CPU capacity bits
 */
ಸಂರಚನಾ asym_cap_data {
	ಸಂರಚನಾ list_head link;
	ಸಂರಚನಾ rcu_head rcu;
	unsigned long capacity;
	unsigned long cpus[];
};

ಬಾಹ್ಯ ಸಂರಚನಾ list_head asym_cap_list;

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
	ಯದಿ (__w)						\
		__w = max(2UL, __w >> SCHED_FIXEDPOINT_SHIFT);	\
	__w;							\
})
#ಅನ್ಯಥಾ
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

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ idle_policy(ಪೂರ್ಣಾಙ್ಕ policy)
{
	ಪ್ರತ್ಯಾಗಮ policy == SCHED_IDLE;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ normal_policy(ಪೂರ್ಣಾಙ್ಕ policy)
{
#ifdef CONFIG_SCHED_CLASS_EXT
	ಯದಿ (policy == SCHED_EXT)
		ಪ್ರತ್ಯಾಗಮ true;
#endif
	ಪ್ರತ್ಯಾಗಮ policy == SCHED_NORMAL;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ fair_policy(ಪೂರ್ಣಾಙ್ಕ policy)
{
	ಪ್ರತ್ಯಾಗಮ normal_policy(policy) || policy == SCHED_BATCH;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ rt_policy(ಪೂರ್ಣಾಙ್ಕ policy)
{
	ಪ್ರತ್ಯಾಗಮ policy == SCHED_FIFO || policy == SCHED_RR;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ dl_policy(ಪೂರ್ಣಾಙ್ಕ policy)
{
	ಪ್ರತ್ಯಾಗಮ policy == SCHED_DEADLINE;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool valid_policy(ಪೂರ್ಣಾಙ್ಕ policy)
{
	ಪ್ರತ್ಯಾಗಮ idle_policy(policy) || fair_policy(policy) ||
		rt_policy(policy) || dl_policy(policy);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ task_has_idle_policy(ಸಂರಚನಾ task_struct *p)
{
	ಪ್ರತ್ಯಾಗಮ idle_policy(p->policy);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ task_has_rt_policy(ಸಂರಚನಾ task_struct *p)
{
	ಪ್ರತ್ಯಾಗಮ rt_policy(p->policy);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ task_has_dl_policy(ಸಂರಚನಾ task_struct *p)
{
	ಪ್ರತ್ಯಾಗಮ dl_policy(p->policy);
}

#define cap_scale(v, s)		((v)*(s) >> SCHED_CAPACITY_SHIFT)

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ update_avg(u64 *avg, u64 sample)
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
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ unsigned long sched_weight_from_cgroup(unsigned long cgrp_weight)
{
	ಪ್ರತ್ಯಾಗಮ DIV_ROUND_CLOSEST_ULL(cgrp_weight * 1024, CGROUP_WEIGHT_DFL);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ unsigned long sched_weight_to_cgroup(unsigned long weight)
{
	ಪ್ರತ್ಯಾಗಮ clamp_t(unsigned long,
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

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool dl_entity_is_special(ಧ್ರುವ ಸಂರಚನಾ sched_dl_entity *dl_se)
{
#ifdef CONFIG_CPU_FREQ_GOV_SCHEDUTIL
	ಪ್ರತ್ಯಾಗಮ unlikely(dl_se->flags & SCHED_FLAG_SUGOV);
#ಅನ್ಯಥಾ
	ಪ್ರತ್ಯಾಗಮ false;
#endif
}

/*
 * Tells if entity @a should preempt entity @b.
 */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool dl_entity_preempt(ಧ್ರುವ ಸಂರಚನಾ sched_dl_entity *a,
				     ಧ್ರುವ ಸಂರಚನಾ sched_dl_entity *b)
{
	ಪ್ರತ್ಯಾಗಮ dl_entity_is_special(a) ||
	       dl_time_before(a->deadline, b->deadline);
}

/*
 * This is the priority-queue data structure of the RT scheduling class:
 */
ಸಂರಚನಾ rt_prio_array {
	DECLARE_BITMAP(bitmap, MAX_RT_PRIO+1); /* include 1 bit for delimiter */
	ಸಂರಚನಾ list_head queue[MAX_RT_PRIO];
};

ಸಂರಚನಾ rt_bandwidth {
	/* nests inside the rq lock: */
	raw_spinlock_t		rt_runtime_lock;
	ktime_t			rt_period;
	u64			rt_runtime;
	ಸಂರಚನಾ hrtimer		rt_period_timer;
	unsigned ಪೂರ್ಣಾಙ್ಕ		rt_period_active;
};

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ dl_bandwidth_enabled(ಶೂನ್ಯ)
{
	ಪ್ರತ್ಯಾಗಮ sysctl_sched_rt_runtime >= 0;
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
ಸಂರಚನಾ dl_bw {
	raw_spinlock_t		lock;
	u64			bw;
	u64			total_bw;
};

ಬಾಹ್ಯ ಶೂನ್ಯ init_dl_bw(ಸಂರಚನಾ dl_bw *dl_b);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ  sched_dl_global_validate(ಶೂನ್ಯ);
ಬಾಹ್ಯ ಶೂನ್ಯ sched_dl_do_global(ಶೂನ್ಯ);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ  sched_dl_overflow(ಸಂರಚನಾ task_struct *p, ಪೂರ್ಣಾಙ್ಕ policy, ಧ್ರುವ ಸಂರಚನಾ sched_attr *attr);
ಬಾಹ್ಯ ಶೂನ್ಯ __setparam_dl(ಸಂರಚನಾ task_struct *p, ಧ್ರುವ ಸಂರಚನಾ sched_attr *attr);
ಬಾಹ್ಯ ಶೂನ್ಯ __getparam_dl(ಸಂರಚನಾ task_struct *p, ಸಂರಚನಾ sched_attr *attr, unsigned ಪೂರ್ಣಾಙ್ಕ flags);
ಬಾಹ್ಯ bool __checkparam_dl(ಧ್ರುವ ಸಂರಚನಾ sched_attr *attr);
ಬಾಹ್ಯ bool dl_param_changed(ಸಂರಚನಾ task_struct *p, ಧ್ರುವ ಸಂರಚನಾ sched_attr *attr);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ  dl_cpuset_cpumask_can_shrink(ಧ್ರುವ ಸಂರಚನಾ cpumask *cur, ಧ್ರುವ ಸಂರಚನಾ cpumask *trial);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ  dl_bw_deactivate(ಪೂರ್ಣಾಙ್ಕ cpu);
ಬಾಹ್ಯ s64 dl_scaled_delta_exec(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ sched_dl_entity *dl_se, s64 delta_exec);
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
ಬಾಹ್ಯ ಶೂನ್ಯ dl_server_update_idle(ಸಂರಚನಾ sched_dl_entity *dl_se, s64 delta_exec);
ಬಾಹ್ಯ ಶೂನ್ಯ dl_server_update(ಸಂರಚನಾ sched_dl_entity *dl_se, s64 delta_exec);
ಬಾಹ್ಯ ಶೂನ್ಯ dl_server_start(ಸಂರಚನಾ sched_dl_entity *dl_se);
ಬಾಹ್ಯ ಶೂನ್ಯ dl_server_stop(ಸಂರಚನಾ sched_dl_entity *dl_se);
ಬಾಹ್ಯ ಶೂನ್ಯ dl_server_init(ಸಂರಚನಾ sched_dl_entity *dl_se, ಸಂರಚನಾ rq *rq,
		    dl_server_pick_f pick_task);
ಬಾಹ್ಯ ಶೂನ್ಯ sched_init_dl_servers(ಶೂನ್ಯ);

ಬಾಹ್ಯ ಶೂನ್ಯ fair_server_init(ಸಂರಚನಾ rq *rq);
ಬಾಹ್ಯ ಶೂನ್ಯ ext_server_init(ಸಂರಚನಾ rq *rq);
ಬಾಹ್ಯ ಶೂನ್ಯ __dl_server_attach_root(ಸಂರಚನಾ sched_dl_entity *dl_se, ಸಂರಚನಾ rq *rq);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ dl_server_apply_params(ಸಂರಚನಾ sched_dl_entity *dl_se,
		    u64 runtime, u64 period, bool init);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ dl_server_attach_bw(ಸಂರಚನಾ sched_dl_entity *dl_se);
ಬಾಹ್ಯ ಶೂನ್ಯ dl_server_detach_bw(ಸಂರಚನಾ sched_dl_entity *dl_se);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ dl_server_swap_bw(ಸಂರಚನಾ sched_dl_entity *detach_se,
			     ಸಂರಚನಾ sched_dl_entity *attach_se);

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool dl_server_active(ಸಂರಚನಾ sched_dl_entity *dl_se)
{
	ಪ್ರತ್ಯಾಗಮ dl_se->dl_server_active;
}

#ifdef CONFIG_CGROUP_SCHED

ಬಾಹ್ಯ ಸಂರಚನಾ list_head task_groups;

#ifdef CONFIG_GROUP_SCHED_BANDWIDTH
ಬಾಹ್ಯ ಧ್ರುವ u64 max_bw_quota_period_us;

/*
 * default period for group bandwidth.
 * default: 0.1s, units: microseconds
 */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ u64 default_bw_period_us(ಶೂನ್ಯ)
{
	ಪ್ರತ್ಯಾಗಮ 100000ULL;
}
#endif /* CONFIG_GROUP_SCHED_BANDWIDTH */

ಸಂರಚನಾ cfs_bandwidth {
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
	ಸಂರಚನಾ hrtimer		period_timer;
	ಸಂರಚನಾ hrtimer		slack_timer;
	ಸಂರಚನಾ list_head	throttled_cfs_rq;

	/* Statistics: */
	ಪೂರ್ಣಾಙ್ಕ			nr_periods;
	ಪೂರ್ಣಾಙ್ಕ			nr_throttled;
	ಪೂರ್ಣಾಙ್ಕ			nr_burst;
	u64			throttled_time;
	u64			burst_time;
#endif /* CONFIG_CFS_BANDWIDTH */
};

/* Task group related information */
ಸಂರಚನಾ task_group {
	ಸಂರಚನಾ cgroup_subsys_state css;

#ifdef CONFIG_GROUP_SCHED_WEIGHT
	/* A positive value indicates that this is a SCHED_IDLE group. */
	ಪೂರ್ಣಾಙ್ಕ			idle;
#endif

#ifdef CONFIG_FAIR_GROUP_SCHED
	/* runqueue "owned" by this group on each CPU */
	ಸಂರಚನಾ cfs_rq __percpu	*cfs_rq;
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
	ಸಂರಚನಾ sched_rt_entity	**rt_se;
	ಸಂರಚನಾ rt_rq		**rt_rq;

	ಸಂರಚನಾ rt_bandwidth	rt_bandwidth;
#endif

	ಸಂರಚನಾ scx_task_group	scx;

	ಸಂರಚನಾ rcu_head		rcu;
	ಸಂರಚನಾ list_head	list;

	ಸಂರಚನಾ task_group	*parent;
	ಸಂರಚನಾ list_head	siblings;
	ಸಂರಚನಾ list_head	children;

#ifdef CONFIG_SCHED_AUTOGROUP
	ಸಂರಚನಾ autogroup	*autogroup;
#endif

	ಸಂರಚನಾ cfs_bandwidth	cfs_bandwidth;

#ifdef CONFIG_UCLAMP_TASK_GROUP
	/* The two decimal precision [%] value requested from user-space */
	unsigned ಪೂರ್ಣಾಙ್ಕ		uclamp_pct[UCLAMP_CNT];
	/* Clamp values requested for a task group */
	ಸಂರಚನಾ uclamp_se	uclamp_req[UCLAMP_CNT];
	/* Effective clamp values used for a task group */
	ಸಂರಚನಾ uclamp_se	uclamp[UCLAMP_CNT];
#endif

};

#define ಬೇರು_TASK_GROUP_LOAD	NICE_0_LOAD

ಪ್ರಕಾರನಿರ್ದೇಶ ಪೂರ್ಣಾಙ್ಕ (*tg_visitor)(ಸಂರಚನಾ task_group *, ಶೂನ್ಯ *);

ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ walk_tg_ವೃಕ್ಷ_from(ಸಂರಚನಾ task_group *from,
			     tg_visitor down, tg_visitor up, ಶೂನ್ಯ *data);

/*
 * Iterate the full tree, calling @down when first entering a node and @up when
 * leaving it for the final time.
 *
 * Caller must hold rcu_lock or sufficient equivalent.
 */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ walk_tg_ವೃಕ್ಷ(tg_visitor down, tg_visitor up, ಶೂನ್ಯ *data)
{
	ಪ್ರತ್ಯಾಗಮ walk_tg_ವೃಕ್ಷ_from(&ಬೇರು_task_group, down, up, data);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ task_group *css_tg(ಸಂರಚನಾ cgroup_subsys_state *css)
{
	ಪ್ರತ್ಯಾಗಮ css ? container_of(css, ಸಂರಚನಾ task_group, css) : ರಿಕ್ತ;
}

ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ tg_nop(ಸಂರಚನಾ task_group *tg, ಶೂನ್ಯ *data);

#ifdef CONFIG_FAIR_GROUP_SCHED
ಬಾಹ್ಯ ಶೂನ್ಯ free_fair_sched_group(ಸಂರಚನಾ task_group *tg);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ alloc_fair_sched_group(ಸಂರಚನಾ task_group *tg, ಸಂರಚನಾ task_group *parent);
ಬಾಹ್ಯ ಶೂನ್ಯ online_fair_sched_group(ಸಂರಚನಾ task_group *tg);
ಬಾಹ್ಯ ಶೂನ್ಯ unregister_fair_sched_group(ಸಂರಚನಾ task_group *tg);
ಬಾಹ್ಯ ಶೂನ್ಯ __sched_cgroup_mode_update(ಪೂರ್ಣಾಙ್ಕ mode);
#ಅನ್ಯಥಾ /* !CONFIG_FAIR_GROUP_SCHED: */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ free_fair_sched_group(ಸಂರಚನಾ task_group *tg) { }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ alloc_fair_sched_group(ಸಂರಚನಾ task_group *tg, ಸಂರಚನಾ task_group *parent)
{
       ಪ್ರತ್ಯಾಗಮ 1;
}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ online_fair_sched_group(ಸಂರಚನಾ task_group *tg) { }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ unregister_fair_sched_group(ಸಂರಚನಾ task_group *tg) { }
#endif /* !CONFIG_FAIR_GROUP_SCHED */

ಬಾಹ್ಯ ಶೂನ್ಯ init_tg_cfs_entry(ಸಂರಚನಾ task_group *tg, ಸಂರಚನಾ cfs_rq *cfs_rq,
			ಸಂರಚನಾ sched_entity *se, ಪೂರ್ಣಾಙ್ಕ cpu,
			ಸಂರಚನಾ sched_entity *parent);
ಬಾಹ್ಯ ಶೂನ್ಯ init_cfs_bandwidth(ಸಂರಚನಾ cfs_bandwidth *cfs_b, ಸಂರಚನಾ cfs_bandwidth *parent);

ಬಾಹ್ಯ ಶೂನ್ಯ __refill_cfs_bandwidth_runtime(ಸಂರಚನಾ cfs_bandwidth *cfs_b);
ಬಾಹ್ಯ ಶೂನ್ಯ start_cfs_bandwidth(ಸಂರಚನಾ cfs_bandwidth *cfs_b);
ಬಾಹ್ಯ ಶೂನ್ಯ unthrottle_cfs_rq(ಸಂರಚನಾ cfs_rq *cfs_rq);
ಬಾಹ್ಯ bool cfs_task_bw_constrained(ಸಂರಚನಾ task_struct *p);

ಬಾಹ್ಯ ಶೂನ್ಯ init_tg_rt_entry(ಸಂರಚನಾ task_group *tg, ಸಂರಚನಾ rt_rq *rt_rq,
		ಸಂರಚನಾ sched_rt_entity *rt_se, ಪೂರ್ಣಾಙ್ಕ cpu,
		ಸಂರಚನಾ sched_rt_entity *parent);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ sched_group_set_rt_runtime(ಸಂರಚನಾ task_group *tg, long rt_runtime_us);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ sched_group_set_rt_period(ಸಂರಚನಾ task_group *tg, u64 rt_period_us);
ಬಾಹ್ಯ long sched_group_rt_runtime(ಸಂರಚನಾ task_group *tg);
ಬಾಹ್ಯ long sched_group_rt_period(ಸಂರಚನಾ task_group *tg);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ sched_rt_can_attach(ಸಂರಚನಾ task_group *tg, ಸಂರಚನಾ task_struct *tsk);

ಬಾಹ್ಯ ಸಂರಚನಾ task_group *sched_create_group(ಸಂರಚನಾ task_group *parent);
ಬಾಹ್ಯ ಶೂನ್ಯ sched_online_group(ಸಂರಚನಾ task_group *tg,
			       ಸಂರಚನಾ task_group *parent);
ಬಾಹ್ಯ ಶೂನ್ಯ sched_destroy_group(ಸಂರಚನಾ task_group *tg);
ಬಾಹ್ಯ ಶೂನ್ಯ sched_release_group(ಸಂರಚನಾ task_group *tg);

ಬಾಹ್ಯ ಶೂನ್ಯ sched_move_task(ಸಂರಚನಾ task_struct *tsk, bool for_autogroup);

#ifdef CONFIG_FAIR_GROUP_SCHED
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ sched_group_set_shares(ಸಂರಚನಾ task_group *tg, unsigned long shares);

ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ sched_group_set_idle(ಸಂರಚನಾ task_group *tg, long idle);

ಬಾಹ್ಯ ಶೂನ್ಯ set_task_rq_fair(ಸಂರಚನಾ sched_entity *se,
			     ಸಂರಚನಾ cfs_rq *prev, ಸಂರಚನಾ cfs_rq *next);
#ಅನ್ಯಥಾ /* !CONFIG_FAIR_GROUP_SCHED: */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ sched_group_set_shares(ಸಂರಚನಾ task_group *tg, unsigned long shares) { ಪ್ರತ್ಯಾಗಮ 0; }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ sched_group_set_idle(ಸಂರಚನಾ task_group *tg, long idle) { ಪ್ರತ್ಯಾಗಮ 0; }
#endif /* !CONFIG_FAIR_GROUP_SCHED */

#ಅನ್ಯಥಾ /* !CONFIG_CGROUP_SCHED: */

ಸಂರಚನಾ cfs_bandwidth { };

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool cfs_task_bw_constrained(ಸಂರಚನಾ task_struct *p) { ಪ್ರತ್ಯಾಗಮ false; }

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

ಬಾಹ್ಯ ಶೂನ್ಯ unregister_rt_sched_group(ಸಂರಚನಾ task_group *tg);
ಬಾಹ್ಯ ಶೂನ್ಯ free_rt_sched_group(ಸಂರಚನಾ task_group *tg);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ alloc_rt_sched_group(ಸಂರಚನಾ task_group *tg, ಸಂರಚನಾ task_group *parent);

/*
 * u64_u32_load/u64_u32_store
 *
 * Use a copy of a u64 value to protect against data race. This is only
 * applicable for 32-bits architectures.
 */
#ifdef CONFIG_64BIT
# define u64_u32_load_copy(var, copy)		var
# define u64_u32_store_copy(var, copy, val)	(var = val)
#ಅನ್ಯಥಾ
# define u64_u32_load_copy(var, copy)					\
({									\
	u64 __val, __val_copy;						\
	ಕುರು {								\
		__val_copy = copy;					\
		/*							\
		 * paired with u64_u32_store_copy(), ordering access	\
		 * to var and copy.					\
		 */							\
		smp_rmb();						\
		__val = var;						\
	} ಯಾವತ್ (__val != __val_copy);					\
	__val;								\
})
# define u64_u32_store_copy(var, copy, val)				\
ಕುರು {									\
	typeof(val) __val = (val);					\
	var = __val;							\
	/*								\
	 * paired with u64_u32_load_copy(), ordering access to var and	\
	 * copy.							\
	 */								\
	smp_wmb();							\
	copy = __val;							\
} ಯಾವತ್ (0)
#endif
# define u64_u32_load(var)		u64_u32_load_copy(var, var##_copy)
# define u64_u32_store(var, val)	u64_u32_store_copy(var, var##_copy, val)

ಸಂರಚನಾ balance_callback {
	ಸಂರಚನಾ balance_callback *next;
	ಶೂನ್ಯ (*func)(ಸಂರಚನಾ rq *rq);
};

/* Fair scheduling SCHED_{NORMAL,BATCH,IDLE} related fields in a runqueue: */
ಸಂರಚನಾ cfs_rq {
	ಸಂರಚನಾ load_weight	load;
	unsigned ಪೂರ್ಣಾಙ್ಕ		nr_queued;
	unsigned ಪೂರ್ಣಾಙ್ಕ		h_nr_queued;		/* SCHED_{NORMAL,BATCH,IDLE} */
	unsigned ಪೂರ್ಣಾಙ್ಕ		h_nr_runnable;		/* SCHED_{NORMAL,BATCH,IDLE} */
	unsigned ಪೂರ್ಣಾಙ್ಕ		h_nr_idle;		/* SCHED_IDLE */

	s64			sum_w_vruntime;
	u64			sum_weight;
	u64			zero_vruntime;
	unsigned ಪೂರ್ಣಾಙ್ಕ		sum_shift;

#ifdef CONFIG_SCHED_CORE
	unsigned ಪೂರ್ಣಾಙ್ಕ		forceidle_seq;
	u64			zero_vruntime_fi;
#endif

	ಸಂರಚನಾ rb_ಬೇರು_cached	tasks_timeline;

	/*
	 * 'curr' points to the currently running entity on this cfs_rq.
	 * It is set to NULL otherwise (i.e when none are currently running).
	 */
	ಸಂರಚನಾ sched_entity	*curr;
	ಸಂರಚನಾ sched_entity	*next;

	/*
	 * CFS load tracking
	 */
	ಸಂರಚನಾ sched_entity	*h_curr;
	ಸಂರಚನಾ sched_avg	avg;
#ifndef CONFIG_64BIT
	u64			last_update_time_copy;
#endif
	ಸಂರಚನಾ {
		raw_spinlock_t	lock ____cacheline_aligned;
		ಪೂರ್ಣಾಙ್ಕ		nr;
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
	ಸಂರಚನಾ sched_entity	*h_load_next;

	ಸಂರಚನಾ rq		*rq;	/* CPU runqueue to which this cfs_rq is attached */

	/*
	 * leaf cfs_rqs are those that hold tasks (lowest schedulable entity in
	 * a hierarchy). Non-leaf lrqs hold other higher schedulable entities
	 * (like users, containers etc.)
	 *
	 * leaf_cfs_rq_list ties together list of leaf cfs_rq's in a CPU.
	 * This list is used during load balance.
	 */
	ಪೂರ್ಣಾಙ್ಕ			on_list;
	ಸಂರಚನಾ list_head	leaf_cfs_rq_list;
	ಸಂರಚನಾ task_group	*tg;	/* Group that "owns" this runqueue */

	/* Locally cached copy of our task_group's idle value */
	ಪೂರ್ಣಾಙ್ಕ			idle;

# ifdef CONFIG_CFS_BANDWIDTH
	ಪೂರ್ಣಾಙ್ಕ			runtime_enabled;
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
	ಪೂರ್ಣಾಙ್ಕ			throttle_ಎಣಿಕೆ;
	ಸಂರಚನಾ list_head	throttled_list;
	ಸಂರಚನಾ list_head	throttled_csd_list;
	ಸಂರಚನಾ list_head        throttled_limbo_list;
# endif /* CONFIG_CFS_BANDWIDTH */
#endif /* CONFIG_FAIR_GROUP_SCHED */
};

#ifdef CONFIG_SCHED_CLASS_EXT
/* scx_rq->flags, protected by the rq lock */
ಗಣನಾ scx_rq_flags {
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
	SCX_RQ_ಬೇರು_IDLE_RENOTIFY	= 1 << 8, /* the root is owed update_idle() */

	SCX_RQ_IN_WAKEUP	= 1 << 16,
	SCX_RQ_IN_DISPATCH	= 1 << 17,
};

/* per-rq rescue execution state, see scx_rescue_timerfn() */
ಸಂರಚನಾ scx_rq_rescue {
	ಸಂರಚನಾ scx_dispatch_q	dsq;			/* stranded tasks awaiting rescue */
	s64			budget;			/* execution token bucket, ns */
	u64			clock;			/* last budget accrual timestamp */
	ಸಂರಚನಾ task_struct	*curr;			/* task being rescued, one at a time */
	s64			slice;			/* curr's admitted slice */
	u64			exec_snap;		/* sum_exec_runtime at admission */
	ಸಂರಚನಾ timer_list	timer;			/* paces admission and escalation */
	u64			kill_at;		/* last ejection, init before any */
};

ಸಂರಚನಾ scx_rq {
	ಸಂರಚನಾ scx_dispatch_q	local_dsq;
#ifdef CONFIG_EXT_SUB_SCHED
	ಸಂರಚನಾ scx_dispatch_q	reject_dsq;		/* staging for cap-rejected tasks */
	ಸಂರಚನಾ scx_rq_rescue	rescue;
#endif
	ಸಂರಚನಾ list_head	runnable_list;		/* runnable tasks on this rq */
	ಸಂರಚನಾ list_head	ddsp_deferred_locals;	/* deferred ddsps from enq */
	/* both stashed across the activate_task() in move_remote_task_to_local_dsq() */
	u64			remote_activate_enq_flags;
	ಸಂರಚನಾ scx_sched	*remote_activate_sch;
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
	ಸಂರಚನಾ llist_head	ecaps_to_sync;		/* pending ecaps syncs */
	ಸಂರಚನಾ task_struct	*sub_dispatch_prev;
#endif
	cpumask_var_t		cpus_to_sync;
	bool			kick_sync_pending;
	unsigned long		kick_sync;

	ಸಂರಚನಾ list_head	sched_pcpus_to_kick;	/* see kick_cpus_irq_workfn() */

	raw_spinlock_t		deferred_reenq_lock;
	ಸಂರಚನಾ list_head	deferred_reenq_locals;	/* scheds requesting reenq of local DSQ */
	ಸಂರಚನಾ list_head	deferred_reenq_users;	/* user DSQs requesting reenq */
	ಸಂರಚನಾ balance_callback	deferred_bal_cb;
	ಸಂರಚನಾ balance_callback	kick_sync_bal_cb;
	ಸಂರಚನಾ irq_work		deferred_irq_work;
	ಸಂರಚನಾ irq_work		kick_cpus_irq_work;
};
#endif /* CONFIG_SCHED_CLASS_EXT */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ rt_bandwidth_enabled(ಶೂನ್ಯ)
{
	ಪ್ರತ್ಯಾಗಮ sysctl_sched_rt_runtime >= 0;
}

/* RT IPI pull logic requires IRQ_WORK */
#ಯದಿ defined(CONFIG_IRQ_WORK) && defined(CONFIG_SMP)
# define HAVE_RT_ತಳ್ಳು_IPI
#endif

/* Real-Time classes' related field in a runqueue: */
ಸಂರಚನಾ rt_rq {
	ಸಂರಚನಾ rt_prio_array	active;
	unsigned ಪೂರ್ಣಾಙ್ಕ		rt_nr_running;
	unsigned ಪೂರ್ಣಾಙ್ಕ		rr_nr_running;
	ಸಂರಚನಾ {
		ಪೂರ್ಣಾಙ್ಕ		curr; /* highest queued rt task prio */
		ಪೂರ್ಣಾಙ್ಕ		next; /* next highest */
	} highest_prio;
	bool			overloaded;
	ಸಂರಚನಾ plist_head	pushable_tasks;

	ಪೂರ್ಣಾಙ್ಕ			rt_queued;

#ifdef CONFIG_RT_GROUP_SCHED
	ಪೂರ್ಣಾಙ್ಕ			rt_throttled;
	u64			rt_time; /* consumed RT time, goes up in update_curr_rt */
	u64			rt_runtime; /* allotted RT time, "slice" from rt_bandwidth, RT sharing/balancing */
	/* Nests inside the rq lock: */
	raw_spinlock_t		rt_runtime_lock;

	unsigned ಪೂರ್ಣಾಙ್ಕ		rt_nr_boosted;

	ಸಂರಚನಾ rq		*rq; /* this is always top-level rq, cache? */
#endif
#ifdef CONFIG_CGROUP_SCHED
	ಸಂರಚನಾ task_group	*tg; /* this tg has "this" rt_rq on given CPU for runnable entities */
#endif
};

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool rt_rq_is_runnable(ಸಂರಚನಾ rt_rq *rt_rq)
{
	ಪ್ರತ್ಯಾಗಮ rt_rq->rt_queued && rt_rq->rt_nr_running;
}

/* Deadline class' related fields in a runqueue */
ಸಂರಚನಾ dl_rq {
	/* runqueue is an rbtree, ordered by deadline */
	ಸಂರಚನಾ rb_ಬೇರು_cached	ಬೇರು;

	unsigned ಪೂರ್ಣಾಙ್ಕ		dl_nr_running;

	/*
	 * Deadline values of the currently executing and the
	 * earliest ready task on this rq. Caching these facilitates
	 * the decision whether or not a ready but not running task
	 * should migrate somewhere else.
	 */
	ಸಂರಚನಾ {
		u64		curr;
		u64		next;
	} earliest_dl;

	bool			overloaded;

	ಸಂರಚನಾ sched_dl_entity	*curr;
	/*
	 * Tasks on this rq that can be pushed away. They are kept in
	 * an rb-tree, ordered by tasks' deadlines, with caching
	 * of the leftmost (earliest deadline) element.
	 */
	ಸಂರಚನಾ rb_ಬೇರು_cached	pushable_dl_tasks_ಬೇರು;

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
#define is_ಬೇರು_task_group(tg) ((tg) == &ಬೇರು_task_group)
/* An entity is a task if it doesn't "own" a runqueue */
#define entity_is_task(se)	(!se->my_q)

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ se_update_runnable(ಸಂರಚನಾ sched_entity *se)
{
	ಯದಿ (!entity_is_task(se))
		se->runnable_weight = se->my_q->h_nr_runnable;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ long se_runnable(ಸಂರಚನಾ sched_entity *se)
{
	ಯದಿ (se->sched_delayed)
		ಪ್ರತ್ಯಾಗಮ false;

	ಯದಿ (entity_is_task(se))
		ಪ್ರತ್ಯಾಗಮ !!se->on_rq;
	ಅನ್ಯಥಾ
		ಪ್ರತ್ಯಾಗಮ se->runnable_weight;
}

#ಅನ್ಯಥಾ /* !CONFIG_FAIR_GROUP_SCHED: */

#define entity_is_task(se)	1

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ se_update_runnable(ಸಂರಚನಾ sched_entity *se) { }

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ long se_runnable(ಸಂರಚನಾ sched_entity *se)
{
	ಯದಿ (se->sched_delayed)
		ಪ್ರತ್ಯಾಗಮ false;

	ಪ್ರತ್ಯಾಗಮ !!se->on_rq;
}

#endif /* !CONFIG_FAIR_GROUP_SCHED */

/*
 * XXX we want to get rid of these helpers and use the full load resolution.
 */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ long se_weight(ಸಂರಚನಾ sched_entity *se)
{
	ಪ್ರತ್ಯಾಗಮ scale_load_down(se->load.weight);
}


ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool sched_asym_prefer(ಪೂರ್ಣಾಙ್ಕ a, ಪೂರ್ಣಾಙ್ಕ b)
{
	ಪ್ರತ್ಯಾಗಮ arch_asym_cpu_priority(a) > arch_asym_cpu_priority(b);
}

ಸಂರಚನಾ perf_domain {
	ಸಂರಚನಾ em_perf_domain *em_pd;
	ಸಂರಚನಾ perf_domain *next;
	ಸಂರಚನಾ rcu_head rcu;
};

/*
 * We add the notion of a root-domain which will be used to define per-domain
 * variables. Each exclusive cpuset essentially defines an island domain by
 * fully partitioning the member CPUs from any other cpuset. Whenever a new
 * exclusive cpuset is created, we also create and attach a new root-domain
 * object.
 *
 */
ಸಂರಚನಾ ಬೇರು_domain {
	atomic_t		refcount;
	atomic_t		rto_ಎಣಿಕೆ;
	ಸಂರಚನಾ rcu_head		rcu;
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
	atomic_t		dlo_ಎಣಿಕೆ;
	ಸಂರಚನಾ dl_bw		dl_bw;
	ಸಂರಚನಾ cpudl		cpudl;

	/*
	 * Indicate whether a root_domain's dl_bw has been checked or
	 * updated. It's monotonously increasing value.
	 *
	 * Also, some corner cases, like 'wrap around' is dangerous, but given
	 * that u64 is 'big enough'. So that shouldn't be a concern.
	 */
	u64 visit_cookie;

#ifdef HAVE_RT_ತಳ್ಳು_IPI
	/*
	 * For IPI pull requests, loop across the rto_mask.
	 */
	ಸಂರಚನಾ irq_work		rto_ತಳ್ಳು_work;
	raw_spinlock_t		rto_lock;
	/* These are only updated and read within rto_lock */
	ಪೂರ್ಣಾಙ್ಕ			rto_loop;
	ಪೂರ್ಣಾಙ್ಕ			rto_cpu;
	/* These atomics are updated outside of a lock */
	atomic_t		rto_loop_next;
	atomic_t		rto_loop_start;
#endif /* HAVE_RT_PUSH_IPI */
	/*
	 * The "RT overload" flag: it gets set if a CPU has more than
	 * one runnable RT task.
	 */
	cpumask_var_t		rto_mask;
	ಸಂರಚನಾ cpupri		cpupri;

	/*
	 * NULL-terminated list of performance domains intersecting with the
	 * CPUs of the rd. Protected by RCU.
	 */
	ಸಂರಚನಾ perf_domain __rcu *pd;
};

ಬಾಹ್ಯ ಶೂನ್ಯ init_defrootdomain(ಶೂನ್ಯ);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ sched_init_domains(ಧ್ರುವ ಸಂರಚನಾ cpumask *cpu_map);
ಬಾಹ್ಯ ಶೂನ್ಯ rq_attach_ಬೇರು(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ ಬೇರು_domain *rd);
ಬಾಹ್ಯ ಶೂನ್ಯ sched_get_rd(ಸಂರಚನಾ ಬೇರು_domain *rd);
ಬಾಹ್ಯ ಶೂನ್ಯ sched_put_rd(ಸಂರಚನಾ ಬೇರು_domain *rd);

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ get_rd_overloaded(ಸಂರಚನಾ ಬೇರು_domain *rd)
{
	ಪ್ರತ್ಯಾಗಮ READ_ONCE(rd->overloaded);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ set_rd_overloaded(ಸಂರಚನಾ ಬೇರು_domain *rd, ಪೂರ್ಣಾಙ್ಕ status)
{
	ಯದಿ (get_rd_overloaded(rd) != status)
		WRITE_ONCE(rd->overloaded, status);
}

#ifdef HAVE_RT_ತಳ್ಳು_IPI
ಬಾಹ್ಯ ಶೂನ್ಯ rto_ತಳ್ಳು_irq_work_func(ಸಂರಚನಾ irq_work *work);
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
ಸಂರಚನಾ uclamp_bucket {
	unsigned long ಮೌಲ್ಯ : bits_per(SCHED_CAPACITY_SCALE);
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
ಸಂರಚನಾ uclamp_rq {
	unsigned ಪೂರ್ಣಾಙ್ಕ ಮೌಲ್ಯ;
	ಸಂರಚನಾ uclamp_bucket bucket[UCLAMP_BUCKETS];
};

DECLARE_STATIC_ಕೀಲಿ_FALSE(sched_uclamp_used);
#endif /* CONFIG_UCLAMP_TASK */

/*
 * This is the main, per-CPU runqueue data structure.
 *
 * Locking rule: those places that want to lock multiple runqueues
 * (such as the load balancing or the thread migration code), lock
 * acquire operations must be ordered by ascending &runqueue.
 */
ಸಂರಚನಾ rq {
	/*
	 * The following members are loaded together, without holding the
	 * rq->lock, in an extremely hot loop in update_sg_lb_stats()
	 * (called from pick_next_task()). To reduce cache pollution from
	 * this operation, they are placed together on this dedicated cache
	 * line. Even though some of them are frequently modified, they are
	 * loaded much more frequently than they are stored.
	 */
	unsigned ಪೂರ್ಣಾಙ್ಕ		nr_running;
#ifdef CONFIG_NUMA_BALANCING
	unsigned ಪೂರ್ಣಾಙ್ಕ		nr_numa_running;
	unsigned ಪೂರ್ಣಾಙ್ಕ		nr_preferred_running;
#endif
	unsigned ಪೂರ್ಣಾಙ್ಕ		ttwu_pending;
	unsigned long		cpu_capacity;
#ifdef CONFIG_SCHED_PROXY_EXEC
	ಸಂರಚನಾ task_struct __rcu	*donor;  /* Scheduling context */
	ಸಂರಚನಾ task_struct __rcu	*curr;   /* Execution context */
#ಅನ್ಯಥಾ
	ಸಙ್ಘ {
		ಸಂರಚನಾ task_struct __rcu *donor; /* Scheduler context */
		ಸಂರಚನಾ task_struct __rcu *curr;  /* Execution context */
	};
#endif
	ಸಂರಚನಾ task_struct	*idle;
	/* padding left here deliberately */

	/*
	 * The next cacheline holds the (hot) runqueue lock, as well as
	 * some other less performance-critical fields.
	 */
	u64			nr_switches	____cacheline_aligned;

	/* runqueue lock: */
	raw_spinlock_t		__lock;

#ifdef CONFIG_NO_HZ_COMMON
	unsigned ಪೂರ್ಣಾಙ್ಕ		nohz_tick_stopped;
	atomic_t		nohz_flags;
	unsigned ಪೂರ್ಣಾಙ್ಕ		has_blocked_load;
	unsigned long		last_blocked_load_update_tick;
	call_single_data_t	nohz_csd;
#endif /* CONFIG_NO_HZ_COMMON */

#ifdef CONFIG_UCLAMP_TASK
	/* Utilization clamp values based on CPU's RUNNABLE tasks */
	ಸಂರಚನಾ uclamp_rq	uclamp[UCLAMP_CNT] ____cacheline_aligned;
	unsigned ಪೂರ್ಣಾಙ್ಕ		uclamp_flags;
#define UCLAMP_FLAG_IDLE 0x01
#endif

	ಸಂರಚನಾ cfs_rq		cfs;
	ಸಂರಚನಾ rt_rq		rt;
	ಸಂರಚನಾ dl_rq		dl;
#ifdef CONFIG_SCHED_CLASS_EXT
	ಸಂರಚನಾ scx_rq		scx;
	ಸಂರಚನಾ sched_dl_entity	ext_server;
#endif
#ifdef CONFIG_SCHED_CACHE
	raw_spinlock_t		cpu_epoch_lock ____cacheline_aligned;
	u64			cpu_runtime;
	unsigned long		cpu_epoch;
	unsigned long		cpu_epoch_next;
#endif

	ಸಂರಚನಾ sched_dl_entity	fair_server;

#ifdef CONFIG_FAIR_GROUP_SCHED
	/* list of leaf cfs_rq on this CPU: */
	ಸಂರಚನಾ list_head	leaf_cfs_rq_list;
	ಸಂರಚನಾ list_head	*tmp_alone_branch;
#endif /* CONFIG_FAIR_GROUP_SCHED */

#ifdef CONFIG_NUMA_BALANCING
	unsigned ಪೂರ್ಣಾಙ್ಕ		numa_migrate_on;
#endif

#ifdef CONFIG_SCHED_CACHE
	unsigned ಪೂರ್ಣಾಙ್ಕ		nr_pref_llc_running;
	unsigned ಪೂರ್ಣಾಙ್ಕ		nr_llc_running;
#endif

	/*
	 * This is part of a global counter where only the total sum
	 * over all CPUs matters. A task can increase this counter on
	 * one CPU and if it got migrated afterwards it may decrease
	 * it on another CPU. Always updated under the runqueue lock:
	 */
	unsigned long		nr_uninterruptible;

	ಸಂರಚನಾ sched_dl_entity	*dl_server;
	ಸಂರಚನಾ task_struct	*stop;
	ಧ್ರುವ ಸಂರಚನಾ sched_class *next_class;
	unsigned long		next_balance;
	ಸಂರಚನಾ mm_struct	*prev_mm;

	/*
	 * The following fields of clock data are frequently referenced
	 * and updated together, and should go on their own cache line.
	 */
	u64			clock_task ____cacheline_aligned;
	u64			clock_pelt;
	u64			clock;
	unsigned long		lost_idle_time;
	unsigned ಪೂರ್ಣಾಙ್ಕ		clock_update_flags;
	u64			clock_pelt_idle;
	u64			clock_idle;

#ifndef CONFIG_64BIT
	u64			clock_pelt_idle_copy;
	u64			clock_idle_copy;
#endif

	u64 last_seen_need_resched_ns;
	ಪೂರ್ಣಾಙ್ಕ ticks_without_resched;

#ifdef CONFIG_MEMBARRIER
	ಪೂರ್ಣಾಙ್ಕ membarrier_state;
#endif

	ಸಂರಚನಾ ಬೇರು_domain		*rd;
	ಸಂರಚನಾ sched_domain __rcu	*sd;

	ಸಂರಚನಾ balance_callback *balance_callback;

	unsigned ವರ್ಣ		nohz_idle_balance;
	unsigned ವರ್ಣ		idle_balance;

	unsigned long		misfit_task_load;

	/* For active balancing */
	ಪೂರ್ಣಾಙ್ಕ			active_balance;
	ಪೂರ್ಣಾಙ್ಕ			ತಳ್ಳು_cpu;
	ಸಂರಚನಾ cpu_stop_work	active_balance_work;

	/* CPU of this runqueue: */
	ಪೂರ್ಣಾಙ್ಕ			cpu;
	ಪೂರ್ಣಾಙ್ಕ			online;

	ಸಂರಚನಾ list_head cfs_tasks;

	ಸಂರಚನಾ sched_avg	avg_rt;
	ಸಂರಚನಾ sched_avg	avg_dl;
#ifdef CONFIG_HAVE_SCHED_AVG_IRQ
	ಸಂರಚನಾ sched_avg	avg_irq;
#endif
#ifdef CONFIG_SCHED_HW_PRESSURE
	ಸಂರಚನಾ sched_avg	avg_hw;
#endif
	u64			idle_stamp;
	u64			avg_idle;

	/* This is used to determine avg_idle's max value */
	u64			max_idle_balance_cost;

#ifdef CONFIG_HOTPLUG_CPU
	ಸಂರಚನಾ rcuwait		hotplug_wait;
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
	bool			npc_ತಳ್ಳು_work_pending;
#endif

	/* calc_load related fields */
	unsigned long		calc_load_update;
	long			calc_load_active;

#ifdef CONFIG_SCHED_HRTICK
	call_single_data_t	hrtick_csd;
	ಸಂರಚನಾ hrtimer		hrtick_timer;
	ktime_t			hrtick_time;
	ktime_t			hrtick_delay;
	unsigned ಪೂರ್ಣಾಙ್ಕ		hrtick_sched;
#endif

#ifdef CONFIG_SCHEDSTATS
	/* latency stats */
	ಸಂರಚನಾ sched_info	rq_sched_info;
	unsigned long long	rq_cpu_time;

	/* sys_sched_yield() stats */
	unsigned ಪೂರ್ಣಾಙ್ಕ		yld_ಎಣಿಕೆ;

	/* schedule() stats */
	unsigned ಪೂರ್ಣಾಙ್ಕ		sched_ಎಣಿಕೆ;
	unsigned ಪೂರ್ಣಾಙ್ಕ		sched_goidle;

	/* try_to_wake_up() stats */
	unsigned ಪೂರ್ಣಾಙ್ಕ		ttwu_ಎಣಿಕೆ;
	unsigned ಪೂರ್ಣಾಙ್ಕ		ttwu_local;
#endif

#ifdef CONFIG_CPU_IDLE
	/* Must be inspected within a RCU lock section */
	ಸಂರಚನಾ cpuidle_state	*idle_state;
#endif

	unsigned ಪೂರ್ಣಾಙ್ಕ		nr_pinned;
	unsigned ಪೂರ್ಣಾಙ್ಕ		ತಳ್ಳು_busy;
	ಸಂರಚನಾ cpu_stop_work	ತಳ್ಳು_work;

#ifdef CONFIG_SCHED_CORE
	/* per rq */
	ಸಂರಚನಾ rq		*core;
	ಸಂರಚನಾ task_struct	*core_pick;
	ಸಂರಚನಾ sched_dl_entity	*core_dl_server;
	unsigned ಪೂರ್ಣಾಙ್ಕ		core_enabled;
	ಸಂರಚನಾ rb_ಬೇರು		core_ವೃಕ್ಷ;

	/* shared state -- careful with sched_core_cpu_deactivate() */
	unsigned ಪೂರ್ಣಾಙ್ಕ		core_task_seq;
	unsigned ಪೂರ್ಣಾಙ್ಕ		core_pick_seq;
	unsigned long		core_cookie;
	unsigned ಪೂರ್ಣಾಙ್ಕ		core_forceidle_ಎಣಿಕೆ;
	unsigned ಪೂರ್ಣಾಙ್ಕ		core_forceidle_seq;
	unsigned ಪೂರ್ಣಾಙ್ಕ		core_forceidle_occupation;
	u64			core_forceidle_start;
	unsigned ಪೂರ್ಣಾಙ್ಕ		core_pick_in_flight;
#endif /* CONFIG_SCHED_CORE */

	/* Scratch cpumask to be temporarily used under rq_lock */
	cpumask_var_t		scratch_mask;

#ifdef CONFIG_CFS_BANDWIDTH
	call_single_data_t	cfsb_csd;
	ಸಂರಚನಾ list_head	cfsb_csd_list;
#endif

	atomic_t		nr_iowait;
} __no_randomize_layout;

#ifdef CONFIG_FAIR_GROUP_SCHED

/* CPU runqueue to which this cfs_rq is attached */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ rq *rq_of(ಸಂರಚನಾ cfs_rq *cfs_rq)
{
	ಪ್ರತ್ಯಾಗಮ cfs_rq->rq;
}

#ಅನ್ಯಥಾ /* !CONFIG_FAIR_GROUP_SCHED: */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ rq *rq_of(ಸಂರಚನಾ cfs_rq *cfs_rq)
{
	ಪ್ರತ್ಯಾಗಮ container_of(cfs_rq, ಸಂರಚನಾ rq, cfs);
}
#endif /* !CONFIG_FAIR_GROUP_SCHED */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ cpu_of(ಸಂರಚನಾ rq *rq)
{
	ಪ್ರತ್ಯಾಗಮ rq->cpu;
}

#define MDF_ತಳ್ಳು		0x01

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool is_migration_disabled(ಸಂರಚನಾ task_struct *p)
{
	ಪ್ರತ್ಯಾಗಮ p->migration_disabled;
}

DECLARE_PER_CPU_SHARED_ALIGNED(ಸಂರಚನಾ rq, runqueues);
DECLARE_PER_CPU(ಸಂರಚನಾ rnd_state, sched_rnd_state);

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ u32 sched_rng(ಶೂನ್ಯ)
{
	ಪ್ರತ್ಯಾಗಮ prandom_u32_state(this_cpu_ptr(&sched_rnd_state));
}

ಸ್ಥಿರ __always_inline ಸಂರಚನಾ rq *__this_rq(ಶೂನ್ಯ)
{
	ಪ್ರತ್ಯಾಗಮ this_cpu_ptr(&runqueues);
}

#define cpu_rq(cpu)		(&per_cpu(runqueues, (cpu)))
#define this_rq()		__this_rq()
#define task_rq(p)		cpu_rq(task_cpu(p))
#define cpu_curr(cpu)		(cpu_rq(cpu)->curr)
#define raw_rq()		raw_cpu_ptr(&runqueues)

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool idle_rq(ಸಂರಚನಾ rq *rq)
{
	ಪ್ರತ್ಯಾಗಮ rq->curr == rq->idle && !rq->nr_running && !rq->ttwu_pending;
}

/**
 * available_idle_cpu - is a given CPU idle for enqueuing work.
 * @cpu: the CPU in question.
 *
 * Return: 1 if the CPU is currently idle. 0 otherwise.
 */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool available_idle_cpu(ಪೂರ್ಣಾಙ್ಕ cpu)
{
	ಯದಿ (!idle_rq(cpu_rq(cpu)))
		ಪ್ರತ್ಯಾಗಮ 0;

	ಯದಿ (vcpu_is_preempted(cpu))
		ಪ್ರತ್ಯಾಗಮ 0;

	ಪ್ರತ್ಯಾಗಮ 1;
}

#ifdef CONFIG_SCHED_PROXY_EXEC
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ rq_set_donor(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *t)
{
	rcu_assign_pointer(rq->donor, t);
}
#ಅನ್ಯಥಾ
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ rq_set_donor(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *t)
{
	/* Do nothing */
}
#endif

#ifdef CONFIG_SCHED_CORE
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ cpumask *sched_group_span(ಸಂರಚನಾ sched_group *sg);

DECLARE_STATIC_ಕೀಲಿ_FALSE(__sched_core_enabled);

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool sched_core_enabled(ಸಂರಚನಾ rq *rq)
{
	ಪ್ರತ್ಯಾಗಮ static_branch_unlikely(&__sched_core_enabled) && rq->core_enabled;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool sched_core_disabled(ಶೂನ್ಯ)
{
	ಪ್ರತ್ಯಾಗಮ !static_branch_unlikely(&__sched_core_enabled);
}

/*
 * Be careful with this function; not for general use. The return value isn't
 * stable unless you actually hold a relevant rq->__lock.
 */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ raw_spinlock_t *rq_lockp(ಸಂರಚನಾ rq *rq)
{
	ಯದಿ (sched_core_enabled(rq))
		ಪ್ರತ್ಯಾಗಮ &rq->core->__lock;

	ಪ್ರತ್ಯಾಗಮ &rq->__lock;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ raw_spinlock_t *__rq_lockp(ಸಂರಚನಾ rq *rq)
	__returns_ctx_lock(rq_lockp(rq)) /* alias them */
{
	ಯದಿ (rq->core_enabled)
		ಪ್ರತ್ಯಾಗಮ &rq->core->__lock;

	ಪ್ರತ್ಯಾಗಮ &rq->__lock;
}

ಬಾಹ್ಯ bool
cfs_prio_less(ಧ್ರುವ ಸಂರಚನಾ task_struct *a, ಧ್ರುವ ಸಂರಚನಾ task_struct *b, bool fi);

ಬಾಹ್ಯ ಶೂನ್ಯ task_vruntime_update(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p, bool in_fi);

/*
 * Helpers to check if the CPU's core cookie matches with the task's cookie
 * when core scheduling is enabled.
 * A special case is that the task's cookie always matches with CPU's core
 * cookie if the CPU is in an idle core.
 */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool sched_cpu_cookie_match(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p)
{
	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	ಯದಿ (!sched_core_enabled(rq))
		ಪ್ರತ್ಯಾಗಮ true;

	ಪ್ರತ್ಯಾಗಮ rq->core->core_cookie == p->core_cookie;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool sched_core_cookie_match(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p)
{
	bool idle_core = true;
	ಪೂರ್ಣಾಙ್ಕ cpu;

	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	ಯದಿ (!sched_core_enabled(rq))
		ಪ್ರತ್ಯಾಗಮ true;

	ಯದಿ (rq->core->core_cookie == p->core_cookie)
		ಪ್ರತ್ಯಾಗಮ true;

	for_each_cpu(cpu, cpu_smt_mask(cpu_of(rq))) {
		ಯದಿ (!available_idle_cpu(cpu)) {
			idle_core = false;
			ಭಙ್ಗ;
		}
	}

	/*
	 * A CPU in an idle core is always the best choice for tasks with
	 * cookies.
	 */
	ಪ್ರತ್ಯಾಗಮ idle_core;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool sched_group_cookie_match(ಸಂರಚನಾ rq *rq,
					    ಸಂರಚನಾ task_struct *p,
					    ಸಂರಚನಾ sched_group *group)
{
	ಪೂರ್ಣಾಙ್ಕ cpu;

	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	ಯದಿ (!sched_core_enabled(rq))
		ಪ್ರತ್ಯಾಗಮ true;

	for_each_cpu_and(cpu, sched_group_span(group), p->cpus_ptr) {
		ಯದಿ (sched_core_cookie_match(cpu_rq(cpu), p))
			ಪ್ರತ್ಯಾಗಮ true;
	}
	ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool sched_core_enqueued(ಸಂರಚನಾ task_struct *p)
{
	ಪ್ರತ್ಯಾಗಮ !RB_EMPTY_ಗಂಟು(&p->core_ಗಂಟು);
}

ಬಾಹ್ಯ ಶೂನ್ಯ sched_core_enqueue(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p);
ಬಾಹ್ಯ ಶೂನ್ಯ sched_core_dequeue(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p, ಪೂರ್ಣಾಙ್ಕ flags);

ಬಾಹ್ಯ ಶೂನ್ಯ sched_core_get(ಶೂನ್ಯ);
ಬಾಹ್ಯ ಶೂನ್ಯ sched_core_put(ಶೂನ್ಯ);

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool task_has_sched_core(ಸಂರಚನಾ task_struct *p)
{
	ಯದಿ (sched_core_disabled())
		ಪ್ರತ್ಯಾಗಮ false;

	ಪ್ರತ್ಯಾಗಮ !!p->core_cookie;
}

#ಅನ್ಯಥಾ /* !CONFIG_SCHED_CORE: */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool sched_core_enabled(ಸಂರಚನಾ rq *rq)
{
	ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool sched_core_disabled(ಶೂನ್ಯ)
{
	ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ raw_spinlock_t *rq_lockp(ಸಂರಚನಾ rq *rq)
{
	ಪ್ರತ್ಯಾಗಮ &rq->__lock;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ raw_spinlock_t *__rq_lockp(ಸಂರಚನಾ rq *rq)
	__returns_ctx_lock(rq_lockp(rq)) /* alias them */
{
	ಪ್ರತ್ಯಾಗಮ &rq->__lock;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool sched_cpu_cookie_match(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p)
{
	ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool sched_core_cookie_match(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p)
{
	ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool sched_group_cookie_match(ಸಂರಚನಾ rq *rq,
					    ಸಂರಚನಾ task_struct *p,
					    ಸಂರಚನಾ sched_group *group)
{
	ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool task_has_sched_core(ಸಂರಚನಾ task_struct *p)
{
	ಪ್ರತ್ಯಾಗಮ false;
}

#endif /* !CONFIG_SCHED_CORE */

#ifdef CONFIG_RT_GROUP_SCHED
# ifdef CONFIG_RT_GROUP_SCHED_DEFAULT_DISABLED
DECLARE_STATIC_ಕೀಲಿ_FALSE(rt_group_sched);
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool rt_group_sched_enabled(ಶೂನ್ಯ)
{
	ಪ್ರತ್ಯಾಗಮ static_branch_unlikely(&rt_group_sched);
}
# ಅನ್ಯಥಾ /* !CONFIG_RT_GROUP_SCHED_DEFAULT_DISABLED: */
DECLARE_STATIC_ಕೀಲಿ_TRUE(rt_group_sched);
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool rt_group_sched_enabled(ಶೂನ್ಯ)
{
	ಪ್ರತ್ಯಾಗಮ static_branch_likely(&rt_group_sched);
}
# endif /* !CONFIG_RT_GROUP_SCHED_DEFAULT_DISABLED */
#ಅನ್ಯಥಾ /* !CONFIG_RT_GROUP_SCHED: */
# define rt_group_sched_enabled()	false
#endif /* !CONFIG_RT_GROUP_SCHED */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ lockdep_assert_rq_held(ಸಂರಚನಾ rq *rq)
	__assumes_ctx_lock(__rq_lockp(rq))
{
	lockdep_assert_held(__rq_lockp(rq));
}

ಬಾಹ್ಯ ಶೂನ್ಯ raw_spin_rq_lock_nested(ಸಂರಚನಾ rq *rq, ಪೂರ್ಣಾಙ್ಕ subclass)
	__acquires(__rq_lockp(rq));

ಬಾಹ್ಯ bool raw_spin_rq_trylock(ಸಂರಚನಾ rq *rq)
	__cond_acquires(true, __rq_lockp(rq));

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ raw_spin_rq_lock(ಸಂರಚನಾ rq *rq)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_lock_nested(rq, 0);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ raw_spin_rq_unlock(ಸಂರಚನಾ rq *rq)
	__releases(__rq_lockp(rq))
{
	raw_spin_unlock(rq_lockp(rq));
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ raw_spin_rq_lock_irq(ಸಂರಚನಾ rq *rq)
	__acquires(__rq_lockp(rq))
{
	local_irq_disable();
	raw_spin_rq_lock(rq);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ raw_spin_rq_unlock_irq(ಸಂರಚನಾ rq *rq)
	__releases(__rq_lockp(rq))
{
	raw_spin_rq_unlock(rq);
	local_irq_enable();
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ unsigned long _raw_spin_rq_lock_irqsave(ಸಂರಚನಾ rq *rq)
	__acquires(__rq_lockp(rq))
{
	unsigned long flags;

	local_irq_save(flags);
	raw_spin_rq_lock(rq);

	ಪ್ರತ್ಯಾಗಮ flags;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ raw_spin_rq_unlock_irqrestore(ಸಂರಚನಾ rq *rq, unsigned long flags)
	__releases(__rq_lockp(rq))
{
	raw_spin_rq_unlock(rq);
	local_irq_restore(flags);
}

#define raw_spin_rq_lock_irqsave(rq, flags)	\
ಕುರು {						\
	flags = _raw_spin_rq_lock_irqsave(rq);	\
} ಯಾವತ್ (0)

ಬಾಹ್ಯ ಶೂನ್ಯ __update_idle_core(ಸಂರಚನಾ rq *rq);

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ update_idle_core(ಸಂರಚನಾ rq *rq)
{
	ಯದಿ (sched_smt_active())
		__update_idle_core(rq);
}

#ifdef CONFIG_FAIR_GROUP_SCHED
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ task_struct *task_of(ಸಂರಚನಾ sched_entity *se)
{
	WARN_ON_ONCE(!entity_is_task(se));
	ಪ್ರತ್ಯಾಗಮ container_of(se, ಸಂರಚನಾ task_struct, se);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ cfs_rq *task_cfs_rq(ಸಂರಚನಾ task_struct *p)
{
	ಪ್ರತ್ಯಾಗಮ p->se.cfs_rq;
}

/* runqueue on which this entity is (to be) queued */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ cfs_rq *cfs_rq_of(ಧ್ರುವ ಸಂರಚನಾ sched_entity *se)
{
	ಪ್ರತ್ಯಾಗಮ se->cfs_rq;
}

/* runqueue "owned" by this group */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ cfs_rq *group_cfs_rq(ಸಂರಚನಾ sched_entity *grp)
{
	ಪ್ರತ್ಯಾಗಮ grp->my_q;
}

#ಅನ್ಯಥಾ /* !CONFIG_FAIR_GROUP_SCHED: */

#define task_of(_se)		container_of(_se, ಸಂರಚನಾ task_struct, se)

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ cfs_rq *task_cfs_rq(ಧ್ರುವ ಸಂರಚನಾ task_struct *p)
{
	ಪ್ರತ್ಯಾಗಮ &task_rq(p)->cfs;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ cfs_rq *cfs_rq_of(ಧ್ರುವ ಸಂರಚನಾ sched_entity *se)
{
	ಧ್ರುವ ಸಂರಚನಾ task_struct *p = task_of(se);
	ಸಂರಚನಾ rq *rq = task_rq(p);

	ಪ್ರತ್ಯಾಗಮ &rq->cfs;
}

/* runqueue "owned" by this group */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ cfs_rq *group_cfs_rq(ಸಂರಚನಾ sched_entity *grp)
{
	ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

#endif /* !CONFIG_FAIR_GROUP_SCHED */

ಬಾಹ್ಯ ಶೂನ್ಯ update_rq_avg_idle(ಸಂರಚನಾ rq *rq);
ಬಾಹ್ಯ ಶೂನ್ಯ update_rq_clock(ಸಂರಚನಾ rq *rq);

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

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ assert_clock_updated(ಸಂರಚನಾ rq *rq)
{
	/*
	 * The only reason for not seeing a clock update since the
	 * last rq_pin_lock() is if we're currently skipping updates.
	 */
	WARN_ON_ONCE(rq->clock_update_flags < RQCF_ACT_SKIP);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ u64 rq_clock(ಸಂರಚನಾ rq *rq)
{
	lockdep_assert_rq_held(rq);
	assert_clock_updated(rq);

	ಪ್ರತ್ಯಾಗಮ rq->clock;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ u64 rq_clock_task(ಸಂರಚನಾ rq *rq)
{
	lockdep_assert_rq_held(rq);
	assert_clock_updated(rq);

	ಪ್ರತ್ಯಾಗಮ rq->clock_task;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ rq_clock_skip_update(ಸಂರಚನಾ rq *rq)
{
	lockdep_assert_rq_held(rq);
	rq->clock_update_flags |= RQCF_REQ_SKIP;
}

/*
 * See rt task throttling, which is the only time a skip
 * request is canceled.
 */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ rq_clock_cancel_skipupdate(ಸಂರಚನಾ rq *rq)
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
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ rq_clock_start_loop_update(ಸಂರಚನಾ rq *rq)
{
	lockdep_assert_rq_held(rq);
	WARN_ON_ONCE(rq->clock_update_flags & RQCF_ACT_SKIP);
	rq->clock_update_flags |= RQCF_ACT_SKIP;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ rq_clock_stop_loop_update(ಸಂರಚನಾ rq *rq)
{
	lockdep_assert_rq_held(rq);
	rq->clock_update_flags &= ~RQCF_ACT_SKIP;
}

ಸಂರಚನಾ rq_flags {
	unsigned long flags;
	ಸಂರಚನಾ pin_cookie cookie;
	/*
	 * A copy of (rq::clock_update_flags & RQCF_UPDATED) for the
	 * current pin context is stashed here in case it needs to be
	 * restored in rq_repin_lock().
	 */
	unsigned ಪೂರ್ಣಾಙ್ಕ clock_update_flags;
};

ಬಾಹ್ಯ ಸಂರಚನಾ balance_callback balance_ತಳ್ಳು_callback;

#ifdef CONFIG_SCHED_CLASS_EXT
ಬಾಹ್ಯ ಧ್ರುವ ಸಂರಚನಾ sched_class ext_sched_class;

DECLARE_STATIC_ಕೀಲಿ_FALSE(__scx_enabled);	/* SCX BPF scheduler loaded */
DECLARE_STATIC_ಕೀಲಿ_FALSE(__scx_switched_all);	/* all fair class tasks on SCX */

#define scx_enabled()		static_branch_unlikely(&__scx_enabled)
#define scx_switched_all()	static_branch_unlikely(&__scx_switched_all)

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ scx_rq_clock_update(ಸಂರಚನಾ rq *rq, u64 clock)
{
	ಯದಿ (!scx_enabled())
		ಪ್ರತ್ಯಾಗಮ;
	WRITE_ONCE(rq->scx.clock, clock);
	smp_store_release(&rq->scx.flags, rq->scx.flags | SCX_RQ_CLK_VALID);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ scx_rq_clock_invalidate(ಸಂರಚನಾ rq *rq)
{
	ಯದಿ (!scx_enabled())
		ಪ್ರತ್ಯಾಗಮ;
	WRITE_ONCE(rq->scx.flags, rq->scx.flags & ~SCX_RQ_CLK_VALID);
}

#ಅನ್ಯಥಾ /* !CONFIG_SCHED_CLASS_EXT: */
#define scx_enabled()		false
#define scx_switched_all()	false

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ scx_rq_clock_update(ಸಂರಚನಾ rq *rq, u64 clock) {}
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ scx_rq_clock_invalidate(ಸಂರಚನಾ rq *rq) {}
#endif /* !CONFIG_SCHED_CLASS_EXT */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ assert_balance_callbacks_empty(ಸಂರಚನಾ rq *rq)
{
	WARN_ON_ONCE(IS_ENABLED(CONFIG_PROVE_LOCKING) &&
		     rq->balance_callback &&
		     rq->balance_callback != &balance_ತಳ್ಳು_callback);
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
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ rq_pin_lock(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ rq_flags *rf)
{
	rf->cookie = lockdep_pin_lock(__rq_lockp(rq));

	rq->clock_update_flags &= (RQCF_REQ_SKIP|RQCF_ACT_SKIP);
	rf->clock_update_flags = 0;
	assert_balance_callbacks_empty(rq);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ rq_unpin_lock(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ rq_flags *rf)
{
	ಯದಿ (rq->clock_update_flags > RQCF_ACT_SKIP)
		rf->clock_update_flags = RQCF_UPDATED;

	scx_rq_clock_invalidate(rq);
	lockdep_unpin_lock(__rq_lockp(rq), rf->cookie);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ rq_repin_lock(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ rq_flags *rf)
{
	lockdep_repin_lock(__rq_lockp(rq), rf->cookie);

	/*
	 * Restore the value we stashed in @rf for this pin context.
	 */
	rq->clock_update_flags |= rf->clock_update_flags;
}

#define __task_rq_lock(...) __acquire_ret(___task_rq_lock(__VA_ARGS__), __rq_lockp(__ret))
ಬಾಹ್ಯ ಸಂರಚನಾ rq *___task_rq_lock(ಸಂರಚನಾ task_struct *p, ಸಂರಚನಾ rq_flags *rf) __acquires_ret;

#define task_rq_lock(...) __acquire_ret(_task_rq_lock(__VA_ARGS__), __rq_lockp(__ret))
ಬಾಹ್ಯ ಸಂರಚನಾ rq *_task_rq_lock(ಸಂರಚನಾ task_struct *p, ಸಂರಚನಾ rq_flags *rf)
	__acquires(&p->pi_lock) __acquires_ret;

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
__task_rq_unlock(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p, ಸಂರಚನಾ rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_lock(rq, rf);
	raw_spin_rq_unlock(rq);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
task_rq_unlock(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p, ಸಂರಚನಾ rq_flags *rf)
	__releases(__rq_lockp(rq), &p->pi_lock)
{
	__task_rq_unlock(rq, p, rf);
	raw_spin_unlock_irqrestore(&p->pi_lock, rf->flags);
}

DEFINE_LOCK_GUARD_1(task_rq_lock, ಸಂರಚನಾ task_struct,
		    _T->rq = task_rq_lock(_T->lock, &_T->rf),
		    task_rq_unlock(_T->rq, _T->lock, &_T->rf),
		    ಸಂರಚನಾ rq *rq; ಸಂರಚನಾ rq_flags rf)
DECLARE_LOCK_GUARD_1_ATTRS(task_rq_lock, __acquires(_T->pi_lock), __releases((*(ಸಂರಚನಾ task_struct **)_T)->pi_lock))
#define class_task_rq_lock_constructor(_T) WITH_LOCK_GUARD_1_ATTRS(task_rq_lock, _T)

DEFINE_LOCK_GUARD_1(__task_rq_lock, ಸಂರಚನಾ task_struct,
		    _T->rq = __task_rq_lock(_T->lock, &_T->rf),
		    __task_rq_unlock(_T->rq, _T->lock, &_T->rf),
		    ಸಂರಚನಾ rq *rq; ಸಂರಚನಾ rq_flags rf)

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ rq_lock_irqsave(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_lock_irqsave(rq, rf->flags);
	rq_pin_lock(rq, rf);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ rq_lock_irq(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_lock_irq(rq);
	rq_pin_lock(rq, rf);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ rq_lock(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_lock(rq);
	rq_pin_lock(rq, rf);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ rq_unlock_irqrestore(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_lock(rq, rf);
	raw_spin_rq_unlock_irqrestore(rq, rf->flags);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ rq_unlock_irq(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_lock(rq, rf);
	raw_spin_rq_unlock_irq(rq);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ rq_unlock(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_lock(rq, rf);
	raw_spin_rq_unlock(rq);
}

DEFINE_LOCK_GUARD_1(rq_lock, ಸಂರಚನಾ rq,
		    rq_lock(_T->lock, &_T->rf),
		    rq_unlock(_T->lock, &_T->rf),
		    ಸಂರಚನಾ rq_flags rf)

DECLARE_LOCK_GUARD_1_ATTRS(rq_lock, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(ಸಂರಚನಾ rq **)_T)));
#define class_rq_lock_constructor(_T) WITH_LOCK_GUARD_1_ATTRS(rq_lock, _T)

DEFINE_LOCK_GUARD_1(rq_lock_irq, ಸಂರಚನಾ rq,
		    rq_lock_irq(_T->lock, &_T->rf),
		    rq_unlock_irq(_T->lock, &_T->rf),
		    ಸಂರಚನಾ rq_flags rf)

DECLARE_LOCK_GUARD_1_ATTRS(rq_lock_irq, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(ಸಂರಚನಾ rq **)_T)));
#define class_rq_lock_irq_constructor(_T) WITH_LOCK_GUARD_1_ATTRS(rq_lock_irq, _T)

DEFINE_LOCK_GUARD_1(rq_lock_irqsave, ಸಂರಚನಾ rq,
		    rq_lock_irqsave(_T->lock, &_T->rf),
		    rq_unlock_irqrestore(_T->lock, &_T->rf),
		    ಸಂರಚನಾ rq_flags rf)

DECLARE_LOCK_GUARD_1_ATTRS(rq_lock_irqsave, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(ಸಂರಚನಾ rq **)_T)));
#define class_rq_lock_irqsave_constructor(_T) WITH_LOCK_GUARD_1_ATTRS(rq_lock_irqsave, _T)

DEFINE_LOCK_GUARD_1(raw_spin_rq_lock_irqsave, ಸಂರಚನಾ rq,
		    raw_spin_rq_lock_irqsave(_T->lock, _T->flags),
		    raw_spin_rq_unlock_irqrestore(_T->lock, _T->flags),
		    unsigned long flags)

DECLARE_LOCK_GUARD_1_ATTRS(raw_spin_rq_lock_irqsave, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(ಸಂರಚನಾ rq **)_T)));
#define class_raw_spin_rq_lock_irqsave_constructor(_T) \
	WITH_LOCK_GUARD_1_ATTRS(raw_spin_rq_lock_irqsave, _T)

#define this_rq_lock_irq(...) __acquire_ret(_this_rq_lock_irq(__VA_ARGS__), __rq_lockp(__ret))
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ rq *_this_rq_lock_irq(ಸಂರಚನಾ rq_flags *rf) __acquires_ret
{
	ಸಂರಚನಾ rq *rq;

	local_irq_disable();
	rq = this_rq();
	rq_lock(rq, rf);

	ಪ್ರತ್ಯಾಗಮ rq;
}

#ifdef CONFIG_NUMA

ಗಣನಾ numa_topology_type {
	NUMA_DIRECT,
	NUMA_GLUELESS_MESH,
	NUMA_BACKPLANE,
};

ಬಾಹ್ಯ ಗಣನಾ numa_topology_type sched_numa_topology_type;
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ sched_max_numa_distance;
ಬಾಹ್ಯ bool find_numa_distance(ಪೂರ್ಣಾಙ್ಕ distance);
ಬಾಹ್ಯ ಶೂನ್ಯ sched_init_numa(ಪೂರ್ಣಾಙ್ಕ offline_ಗಂಟು);
ಬಾಹ್ಯ ಶೂನ್ಯ sched_update_numa(ಪೂರ್ಣಾಙ್ಕ cpu, bool online);
ಬಾಹ್ಯ ಶೂನ್ಯ sched_domains_numa_masks_set(unsigned ಪೂರ್ಣಾಙ್ಕ cpu);
ಬಾಹ್ಯ ಶೂನ್ಯ sched_domains_numa_masks_clear(unsigned ಪೂರ್ಣಾಙ್ಕ cpu);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ sched_numa_find_closest(ಧ್ರುವ ಸಂರಚನಾ cpumask *cpus, ಪೂರ್ಣಾಙ್ಕ cpu);

#ಅನ್ಯಥಾ /* !CONFIG_NUMA: */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ sched_init_numa(ಪೂರ್ಣಾಙ್ಕ offline_ಗಂಟು) { }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ sched_update_numa(ಪೂರ್ಣಾಙ್ಕ cpu, bool online) { }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ sched_domains_numa_masks_set(unsigned ಪೂರ್ಣಾಙ್ಕ cpu) { }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ sched_domains_numa_masks_clear(unsigned ಪೂರ್ಣಾಙ್ಕ cpu) { }

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ sched_numa_find_closest(ಧ್ರುವ ಸಂರಚನಾ cpumask *cpus, ಪೂರ್ಣಾಙ್ಕ cpu)
{
	ಪ್ರತ್ಯಾಗಮ nr_cpu_ids;
}

#endif /* !CONFIG_NUMA */

#ifdef CONFIG_NUMA_BALANCING

/* The regions in numa_faults array from task_struct */
ಗಣನಾ numa_faults_stats {
	NUMA_MEM = 0,
	NUMA_CPU,
	NUMA_MEMBUF,
	NUMA_CPUBUF
};

ಬಾಹ್ಯ ಶೂನ್ಯ sched_setnuma(ಸಂರಚನಾ task_struct *p, ಪೂರ್ಣಾಙ್ಕ ಗಂಟು);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ migrate_task_to(ಸಂರಚನಾ task_struct *p, ಪೂರ್ಣಾಙ್ಕ cpu);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ migrate_swap(ಸಂರಚನಾ task_struct *p, ಸಂರಚನಾ task_struct *t,
			ಪೂರ್ಣಾಙ್ಕ cpu, ಪೂರ್ಣಾಙ್ಕ scpu);
ಬಾಹ್ಯ ಶೂನ್ಯ init_numa_balancing(u64 clone_flags, ಸಂರಚನಾ task_struct *p);

#ಅನ್ಯಥಾ /* !CONFIG_NUMA_BALANCING: */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
init_numa_balancing(u64 clone_flags, ಸಂರಚನಾ task_struct *p)
{
}

#endif /* !CONFIG_NUMA_BALANCING */

ಪೂರ್ಣಾಙ್ಕ task_llc(ಧ್ರುವ ಸಂರಚನಾ task_struct *p);

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
queue_balance_callback(ಸಂರಚನಾ rq *rq,
		       ಸಂರಚನಾ balance_callback *head,
		       ಶೂನ್ಯ (*func)(ಸಂರಚನಾ rq *rq))
{
	lockdep_assert_rq_held(rq);

	/*
	 * Don't (re)queue an already queued item; nor queue anything when
	 * balance_push() is active, see the comment with
	 * balance_push_callback.
	 */
	ಯದಿ (unlikely(head->next || rq->balance_callback == &balance_ತಳ್ಳು_callback))
		ಪ್ರತ್ಯಾಗಮ;

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
	ಪ್ರತಿ (__sd = rcu_dereference_sched_domain(cpu_rq(cpu)->sd); \
			__sd; __sd = __sd->parent)

/* A mask of all the SD flags that have the SDF_SHARED_CHILD metaflag */
#define SD_FLAG(name, mflags) (name * !!((mflags) & SDF_SHARED_CHILD)) |
ಸ್ಥಿರ ಧ್ರುವ unsigned ಪೂರ್ಣಾಙ್ಕ SD_SHARED_CHILD_MASK =
#ಅನ್ತರ್ಭಾವ <linux/sched/sd_flags.h>
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
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ sched_domain *highest_flag_domain(ಪೂರ್ಣಾಙ್ಕ cpu, ಪೂರ್ಣಾಙ್ಕ flag)
{
	ಸಂರಚನಾ sched_domain *sd, *hsd = ರಿಕ್ತ;

	for_each_domain(cpu, sd) {
		ಯದಿ (sd->flags & flag) {
			hsd = sd;
			ಅನುವರ್ತ;
		}

		/*
		 * Stop the search if @flag is known to be shared at lower
		 * levels. It will not be found further up.
		 */
		ಯದಿ (flag & SD_SHARED_CHILD_MASK)
			ಭಙ್ಗ;
	}

	ಪ್ರತ್ಯಾಗಮ hsd;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ sched_domain *lowest_flag_domain(ಪೂರ್ಣಾಙ್ಕ cpu, ಪೂರ್ಣಾಙ್ಕ flag)
{
	ಸಂರಚನಾ sched_domain *sd;

	for_each_domain(cpu, sd) {
		ಯದಿ (sd->flags & flag)
			ಭಙ್ಗ;
	}

	ಪ್ರತ್ಯಾಗಮ sd;
}

DECLARE_PER_CPU(ಸಂರಚನಾ sched_domain __rcu *, sd_llc);
DECLARE_PER_CPU(ಪೂರ್ಣಾಙ್ಕ, sd_llc_size);
DECLARE_PER_CPU(ಪೂರ್ಣಾಙ್ಕ, sd_llc_id);
DECLARE_PER_CPU(ಪೂರ್ಣಾಙ್ಕ, sd_share_id);
DECLARE_PER_CPU(ಸಂರಚನಾ sched_domain_shared __rcu *, sd_llc_shared);
DECLARE_PER_CPU(ಸಂರಚನಾ sched_domain_shared __rcu *, sd_balance_shared);
DECLARE_PER_CPU(ಸಂರಚನಾ sched_domain __rcu *, sd_numa);
DECLARE_PER_CPU(ಸಂರಚನಾ sched_domain __rcu *, sd_asym_packing);
DECLARE_PER_CPU(ಸಂರಚನಾ sched_domain __rcu *, sd_asym_cpucapacity);

ಬಾಹ್ಯ ಸಂರಚನಾ static_ಕೀಲಿ_false sched_asym_cpucapacity;
ಬಾಹ್ಯ ಸಂರಚನಾ static_ಕೀಲಿ_false sched_cluster_active;

ಸ್ಥಿರ __always_inline bool sched_asym_cpucap_active(ಶೂನ್ಯ)
{
	ಪ್ರತ್ಯಾಗಮ static_branch_unlikely(&sched_asym_cpucapacity);
}

ಸಂರಚನಾ sched_group_capacity {
	atomic_t		ref;
	/*
	 * CPU capacity of this group, SCHED_CAPACITY_SCALE being max capacity
	 * for a single CPU.
	 */
	unsigned long		capacity;
	unsigned long		min_capacity;		/* Min per-CPU capacity in group */
	unsigned long		max_capacity;		/* Max per-CPU capacity in group */
	unsigned long		next_update;
	ಪೂರ್ಣಾಙ್ಕ			imbalance;		/* XXX unrelated to capacity but shared group state */

	ಪೂರ್ಣಾಙ್ಕ			id;

	unsigned long		cpumask[];		/* Balance mask */
};

ಸಂರಚನಾ sched_group {
	ಸಂರಚನಾ sched_group	*next;			/* Must be a circular list */
	atomic_t		ref;

	unsigned ಪೂರ್ಣಾಙ್ಕ		group_weight;
	unsigned ಪೂರ್ಣಾಙ್ಕ		cores;
	ಸಂರಚನಾ sched_group_capacity *sgc;
	ಪೂರ್ಣಾಙ್ಕ			asym_prefer_cpu;	/* CPU of highest priority in group */
	ಪೂರ್ಣಾಙ್ಕ			flags;

	/*
	 * The CPUs this group covers.
	 *
	 * NOTE: this field is variable length. (Allocated dynamically
	 * by attaching extra space to the end of the structure,
	 * depending on how many CPUs the kernel has booted up with)
	 */
	unsigned long		cpumask[];
};

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ cpumask *sched_group_span(ಸಂರಚನಾ sched_group *sg)
{
	ಪ್ರತ್ಯಾಗಮ to_cpumask(sg->cpumask);
}

/*
 * See build_balance_mask().
 */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ cpumask *group_balance_mask(ಸಂರಚನಾ sched_group *sg)
{
	ಪ್ರತ್ಯಾಗಮ to_cpumask(sg->sgc->cpumask);
}

ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ group_balance_cpu(ಸಂರಚನಾ sched_group *sg);

ಬಾಹ್ಯ ಶೂನ್ಯ update_sched_domain_debugfs(ಶೂನ್ಯ);
ಬಾಹ್ಯ ಶೂನ್ಯ dirty_sched_domain_sysctl(ಪೂರ್ಣಾಙ್ಕ cpu);

ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ sched_update_scaling(ಶೂನ್ಯ);

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಧ್ರುವ ಸಂರಚನಾ cpumask *task_user_cpus(ಸಂರಚನಾ task_struct *p)
{
	ಯದಿ (!p->user_cpus_ptr)
		ಪ್ರತ್ಯಾಗಮ cpu_possible_mask; /* &init_task.cpus_mask */
	ಪ್ರತ್ಯಾಗಮ p->user_cpus_ptr;
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
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ task_group *task_group(ಸಂರಚನಾ task_struct *p)
{
	ಪ್ರತ್ಯಾಗಮ p->sched_task_group;
}

#ifdef CONFIG_FAIR_GROUP_SCHED
/*
 * Defined here to be available before stats.h is included, since
 * stats.h has dependencies on things defined later in this file.
 */
ಸಂರಚನಾ cfs_tg_state {
	ಸಂರಚನಾ cfs_rq		cfs_rq;
	ಸಂರಚನಾ sched_entity	se;
	ಸಂರಚನಾ sched_statistics	stats;
} __no_randomize_layout;

/* Access a specific CPU's cfs_rq from a task group */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ cfs_rq *tg_cfs_rq(ಸಂರಚನಾ task_group *tg, ಪೂರ್ಣಾಙ್ಕ cpu)
{
	ಪ್ರತ್ಯಾಗಮ per_cpu_ptr(tg->cfs_rq, cpu);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ sched_entity *tg_se(ಸಂರಚನಾ task_group *tg, ಪೂರ್ಣಾಙ್ಕ cpu)
{
	ಸಂರಚನಾ cfs_tg_state *state;

	ಯದಿ (is_ಬೇರು_task_group(tg))
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

	state = container_of(tg_cfs_rq(tg, cpu), ಸಂರಚನಾ cfs_tg_state, cfs_rq);
	ಪ್ರತ್ಯಾಗಮ &state->se;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ sched_entity *cfs_rq_se(ಸಂರಚನಾ cfs_rq *cfs_rq)
{
	ಸಂರಚನಾ cfs_tg_state *state;

	ಯದಿ (is_ಬೇರು_task_group(cfs_rq->tg))
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

	state = container_of(cfs_rq, ಸಂರಚನಾ cfs_tg_state, cfs_rq);
	ಪ್ರತ್ಯಾಗಮ &state->se;
}
#endif

/* Change a task's cfs_rq and parent entity if it moves across CPUs/groups */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ set_task_rq(ಸಂರಚನಾ task_struct *p, unsigned ಪೂರ್ಣಾಙ್ಕ cpu)
{
#ಯದಿ defined(CONFIG_FAIR_GROUP_SCHED) || defined(CONFIG_RT_GROUP_SCHED)
	ಸಂರಚನಾ task_group *tg = task_group(p);
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
	ಯದಿ (!rt_group_sched_enabled())
		tg = &ಬೇರು_task_group;
	p->rt.rt_rq  = tg->rt_rq[cpu];
	p->rt.parent = tg->rt_se[cpu];
#endif /* CONFIG_RT_GROUP_SCHED */
}

#ಅನ್ಯಥಾ /* !CONFIG_CGROUP_SCHED: */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ set_task_rq(ಸಂರಚನಾ task_struct *p, unsigned ಪೂರ್ಣಾಙ್ಕ cpu) { }

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ task_group *task_group(ಸಂರಚನಾ task_struct *p)
{
	ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

#endif /* !CONFIG_CGROUP_SCHED */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ __set_task_cpu(ಸಂರಚನಾ task_struct *p, unsigned ಪೂರ್ಣಾಙ್ಕ cpu)
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

ಗಣನಾ {
#ಅನ್ತರ್ಭಾವ "features.h"
	__SCHED_FEAT_NR,
};

#undef SCHED_FEAT

/*
 * To support run-time toggling of sched features, all the translation units
 * (but core.c) reference the sysctl_sched_features defined in core.c.
 */
ಬಾಹ್ಯ __read_mostly unsigned ಪೂರ್ಣಾಙ್ಕ sysctl_sched_features;

#ifdef CONFIG_JUMP_LABEL

ಸಙ್ಘ sched_feat_ಕೀಲಿ {
	ಸಂರಚನಾ static_ಕೀಲಿ_true	ಕೀಲಿ_true;
	ಸಂರಚನಾ static_ಕೀಲಿ_false	ಕೀಲಿ_false;
};

#define sched_feat_branch_true(ಕೀಲಿ)	static_branch_likely(&(ಕೀಲಿ)->ಕೀಲಿ_true)
#define sched_feat_branch_false(ಕೀಲಿ)	static_branch_unlikely(&(ಕೀಲಿ)->ಕೀಲಿ_false)

#define SCHED_FEAT(name, enabled)			\
ಸ್ಥಿರ __always_inline bool				\
static_branch_##name(ಸಙ್ಘ sched_feat_ಕೀಲಿ *ಕೀಲಿ) 	\
{							\
	ಪ್ರತ್ಯಾಗಮ sched_feat_branch_##enabled(ಕೀಲಿ);	\
}

#ಅನ್ತರ್ಭಾವ "features.h"
#undef SCHED_FEAT

ಬಾಹ್ಯ ಸಙ್ಘ sched_feat_ಕೀಲಿ sched_feat_ಕೀಲಿಗಳು[__SCHED_FEAT_NR];
#define sched_feat(x) (static_branch_##x(&sched_feat_ಕೀಲಿಗಳು[__SCHED_FEAT_##x]))

#ಅನ್ಯಥಾ /* !CONFIG_JUMP_LABEL: */

#define sched_feat(x) (sysctl_sched_features & (1UL << __SCHED_FEAT_##x))

#endif /* !CONFIG_JUMP_LABEL */

ಬಾಹ್ಯ ಸಂರಚನಾ static_ಕೀಲಿ_false sched_numa_balancing;
ಬಾಹ್ಯ ಸಂರಚನಾ static_ಕೀಲಿ_false sched_schedstats;

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ u64 global_rt_period(ಶೂನ್ಯ)
{
	ಪ್ರತ್ಯಾಗಮ (u64)sysctl_sched_rt_period * NSEC_PER_USEC;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ u64 global_rt_runtime(ಶೂನ್ಯ)
{
	ಯದಿ (sysctl_sched_rt_runtime < 0)
		ಪ್ರತ್ಯಾಗಮ RUNTIME_INF;

	ಪ್ರತ್ಯಾಗಮ (u64)sysctl_sched_rt_runtime * NSEC_PER_USEC;
}

/*
 * Is p the current execution context?
 */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ task_current(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p)
{
	ಪ್ರತ್ಯಾಗಮ rq->curr == p;
}

/*
 * Is p the current scheduling context?
 *
 * Note that it might be the current execution context at the same time if
 * rq->curr == rq->donor == p.
 */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ task_current_donor(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p)
{
	ಪ್ರತ್ಯಾಗಮ rq->donor == p;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool task_is_blocked(ಸಂರಚನಾ task_struct *p)
{
	ಯದಿ (!sched_proxy_exec())
		ಪ್ರತ್ಯಾಗಮ false;

	ಪ್ರತ್ಯಾಗಮ !!p->blocked_on;
}

#ifdef CONFIG_SCHED_PROXY_EXEC
ಶೂನ್ಯ sched_proxy_block_task(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p);
#ಅನ್ಯಥಾ
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ sched_proxy_block_task(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p) {}
#endif

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ task_on_cpu(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p)
{
	ಪ್ರತ್ಯಾಗಮ p->on_cpu;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ task_on_rq_queued(ಸಂರಚನಾ task_struct *p)
{
	ಪ್ರತ್ಯಾಗಮ READ_ONCE(p->on_rq) == TASK_ON_RQ_QUEUED;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ task_on_rq_migrating(ಸಂರಚನಾ task_struct *p)
{
	ಪ್ರತ್ಯಾಗಮ READ_ONCE(p->on_rq) == TASK_ON_RQ_MIGRATING;
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

ಬಾಹ್ಯ ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ		sched_prio_to_weight[40];
ಬಾಹ್ಯ ಧ್ರುವ u32		sched_prio_to_wmult[40];

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

#define RETRY_TASK		((ಶೂನ್ಯ *)-1UL)

ಸಂರಚನಾ affinity_context {
	ಧ್ರುವ ಸಂರಚನಾ cpumask	*new_mask;
	ಸಂರಚನಾ cpumask		*user_mask;
	unsigned ಪೂರ್ಣಾಙ್ಕ		flags;
};

ಬಾಹ್ಯ s64 update_curr_common(ಸಂರಚನಾ rq *rq);

ಗಣನಾ snt_e {
	SNT_NORMAL,	/* set_next_task() */
	SNT_PICK,	/* put_prev_set_next_task(): prev != next */
	SNT_REPICK,	/* put_prev_set_next_task(): prev == next */
};

ಸಂರಚನಾ sched_class {

#ifdef CONFIG_UCLAMP_TASK
	ಪೂರ್ಣಾಙ್ಕ uclamp_enabled;
#endif

	/*
	 * move_queued_task/activate_task/enqueue_task: rq->lock
	 * ttwu_do_activate/activate_task/enqueue_task: rq->lock
	 * wake_up_new_task/activate_task/enqueue_task: task_rq_lock
	 * ttwu_runnable/enqueue_task: task_rq_lock
	 * proxy_task_current: rq->lock
	 * sched_change_end
	 */
	ಶೂನ್ಯ (*enqueue_task) (ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p, ಪೂರ್ಣಾಙ್ಕ flags);
	/*
	 * move_queued_task/deactivate_task/dequeue_task: rq->lock
	 * __schedule/block_task/dequeue_task: rq->lock
	 * proxy_task_current: rq->lock
	 * wait_task_inactive: task_rq_lock
	 * sched_change_begin
	 */
	bool (*dequeue_task) (ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p, ಪೂರ್ಣಾಙ್ಕ flags);

	/*
	 * do_sched_yield: rq->lock
	 */
	ಶೂನ್ಯ (*yield_task)   (ಸಂರಚನಾ rq *rq);
	/*
	 * yield_to: rq->lock (double)
	 */
	bool (*yield_to_task)(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p);

	/*
	 * move_queued_task: rq->lock
	 * __migrate_swap_task: rq->lock
	 * ttwu_do_activate: rq->lock
	 * ttwu_runnable: task_rq_lock
	 * wake_up_new_task: task_rq_lock
	 */
	ಶೂನ್ಯ (*wakeup_preempt)(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p, ಪೂರ್ಣಾಙ್ಕ flags);

	/*
	 * schedule/pick_next_task/prev_balance: rq->lock
	 */
	ಪೂರ್ಣಾಙ್ಕ (*balance)(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ rq_flags *rf);

	/*
	 * schedule/pick_next_task: rq->lock
	 */
	ಸಂರಚನಾ task_struct *(*pick_task)(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ rq_flags *rf);

	/*
	 * sched_change:
	 * __schedule: rq->lock
	 */
	ಶೂನ್ಯ (*put_prev_task)(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p, ಸಂರಚನಾ task_struct *next);
	ಶೂನ್ಯ (*set_next_task)(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p, ಗಣನಾ snt_e type);

	/*
	 * select_task_rq: p->pi_lock
	 * sched_exec: p->pi_lock
	 */
	ಪೂರ್ಣಾಙ್ಕ  (*select_task_rq)(ಸಂರಚನಾ task_struct *p, ಪೂರ್ಣಾಙ್ಕ task_cpu, ಪೂರ್ಣಾಙ್ಕ flags);

	/*
	 * set_task_cpu: p->pi_lock || rq->lock (ttwu like)
	 */
	ಶೂನ್ಯ (*migrate_task_rq)(ಸಂರಚನಾ task_struct *p, ಪೂರ್ಣಾಙ್ಕ new_cpu);

	/*
	 * ttwu_do_activate: rq->lock
	 * wake_up_new_task: task_rq_lock
	 */
	ಶೂನ್ಯ (*task_woken)(ಸಂರಚನಾ rq *this_rq, ಸಂರಚನಾ task_struct *task);

	/*
	 * do_set_cpus_allowed: task_rq_lock + sched_change
	 */
	ಶೂನ್ಯ (*set_cpus_allowed)(ಸಂರಚನಾ task_struct *p, ಸಂರಚನಾ affinity_context *ctx);

	/*
	 * sched_set_rq_{on,off}line: rq->lock
	 */
	ಶೂನ್ಯ (*rq_online)(ಸಂರಚನಾ rq *rq);
	ಶೂನ್ಯ (*rq_offline)(ಸಂರಚನಾ rq *rq);

	/*
	 * push_cpu_stop: p->pi_lock && rq->lock
	 */
	ಸಂರಚನಾ rq *(*find_lock_rq)(ಸಂರಚನಾ task_struct *p, ಸಂರಚನಾ rq *rq);

	/*
	 * hrtick: rq->lock
	 * sched_tick: rq->lock
	 * sched_tick_remote: rq->lock
	 */
	ಶೂನ್ಯ (*task_tick)(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p, ಪೂರ್ಣಾಙ್ಕ queued);
	/*
	 * sched_cgroup_fork: p->pi_lock
	 */
	ಶೂನ್ಯ (*task_fork)(ಸಂರಚನಾ task_struct *p);
	/*
	 * finish_task_switch: no locks
	 */
	ಶೂನ್ಯ (*task_dead)(ಸಂರಚನಾ task_struct *p);

	/*
	 * sched_change
	 */
	ಶೂನ್ಯ (*switching_from)(ಸಂರಚನಾ rq *this_rq, ಸಂರಚನಾ task_struct *task);
	ಶೂನ್ಯ (*switched_from) (ಸಂರಚನಾ rq *this_rq, ಸಂರಚನಾ task_struct *task);
	ಶೂನ್ಯ (*switching_to)  (ಸಂರಚನಾ rq *this_rq, ಸಂರಚನಾ task_struct *task);
	ಶೂನ್ಯ (*switched_to)   (ಸಂರಚನಾ rq *this_rq, ಸಂರಚನಾ task_struct *task);
	u64  (*get_prio)     (ಸಂರಚನಾ rq *this_rq, ಸಂರಚನಾ task_struct *task);
	ಶೂನ್ಯ (*prio_changed) (ಸಂರಚನಾ rq *this_rq, ಸಂರಚನಾ task_struct *task,
			      u64 oldprio);

	/*
	 * set_load_weight: task_rq_lock + sched_change
	 * __setscheduler_parms: task_rq_lock + sched_change
	 */
	ಶೂನ್ಯ (*reweight_task)(ಸಂರಚನಾ rq *this_rq, ಸಂರಚನಾ task_struct *task,
			      ಧ್ರುವ ಸಂರಚನಾ load_weight *lw);

	/*
	 * sched_rr_get_interval: task_rq_lock
	 */
	unsigned ಪೂರ್ಣಾಙ್ಕ (*get_rr_interval)(ಸಂರಚನಾ rq *rq,
					ಸಂರಚನಾ task_struct *task);

	/*
	 * task_sched_runtime: task_rq_lock
	 */
	ಶೂನ್ಯ (*update_curr)(ಸಂರಚನಾ rq *rq);

#ifdef CONFIG_FAIR_GROUP_SCHED
	/*
	 * sched_change_group: task_rq_lock + sched_change
	 */
	ಶೂನ್ಯ (*task_change_group)(ಸಂರಚನಾ task_struct *p);
#endif

#ifdef CONFIG_SCHED_CORE
	/*
	 * pick_next_task: rq->lock
	 * try_steal_cookie: rq->lock (double)
	 */
	ಪೂರ್ಣಾಙ್ಕ (*task_is_throttled)(ಸಂರಚನಾ task_struct *p, ಪೂರ್ಣಾಙ್ಕ cpu);
#endif
};

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ put_prev_task(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *prev)
{
	WARN_ON_ONCE(rq->donor != prev);
	prev->sched_class->put_prev_task(rq, prev, ರಿಕ್ತ);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ set_next_task(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *next)
{
	next->sched_class->set_next_task(rq, next, SNT_NORMAL);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
__put_prev_set_next_dl_server(ಸಂರಚನಾ rq *rq,
			      ಸಂರಚನಾ task_struct *prev,
			      ಸಂರಚನಾ task_struct *next)
{
	prev->dl_server = ರಿಕ್ತ;
	next->dl_server = rq->dl_server;
	rq->dl_server = ರಿಕ್ತ;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ put_prev_set_next_task(ಸಂರಚನಾ rq *rq,
					  ಸಂರಚನಾ task_struct *prev,
					  ಸಂರಚನಾ task_struct *next)
{
	WARN_ON_ONCE(rq->donor != prev);

	__put_prev_set_next_dl_server(rq, prev, next);

	ಯದಿ (next == prev) {
		next->sched_class->set_next_task(rq, next, SNT_REPICK);
		ಪ್ರತ್ಯಾಗಮ;
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
ಧ್ರುವ ಸಂರಚನಾ sched_class name##_sched_class \
	__aligned(__alignof__(ಸಂರಚನಾ sched_class)) \
	__section("__" #name "_sched_class")

/* Defined in include/asm-generic/vmlinux.lds.h */
ಬಾಹ್ಯ ಸಂರಚನಾ sched_class __sched_class_highest[];
ಬಾಹ್ಯ ಸಂರಚನಾ sched_class __sched_class_lowest[];

ಬಾಹ್ಯ ಧ್ರುವ ಸಂರಚನಾ sched_class stop_sched_class;
ಬಾಹ್ಯ ಧ್ರುವ ಸಂರಚನಾ sched_class dl_sched_class;
ಬಾಹ್ಯ ಧ್ರುವ ಸಂರಚನಾ sched_class rt_sched_class;
ಬಾಹ್ಯ ಧ್ರುವ ಸಂರಚನಾ sched_class fair_sched_class;
ಬಾಹ್ಯ ಧ್ರುವ ಸಂರಚನಾ sched_class idle_sched_class;

/*
 * Iterate only active classes. SCX can take over all fair tasks or be
 * completely disabled. If the former, skip fair. If the latter, skip SCX.
 */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಧ್ರುವ ಸಂರಚನಾ sched_class *next_active_class(ಧ್ರುವ ಸಂರಚನಾ sched_class *class)
{
	class++;
#ifdef CONFIG_SCHED_CLASS_EXT
	ಯದಿ (scx_switched_all() && class == &fair_sched_class)
		class++;
	ಯದಿ (!scx_enabled() && class == &ext_sched_class)
		class++;
#endif
	ಪ್ರತ್ಯಾಗಮ class;
}

#define for_class_range(class, _from, _to) \
	ಪ್ರತಿ (class = (_from); class < (_to); class++)

#define for_each_class(class) \
	for_class_range(class, __sched_class_highest, __sched_class_lowest)

#define for_active_class_range(class, _from, _to)				\
	ಪ್ರತಿ (class = (_from); class != (_to); class = next_active_class(class))

#define for_each_active_class(class)						\
	for_active_class_range(class, __sched_class_highest, __sched_class_lowest)

#define sched_class_above(_a, _b)	((_a) < (_b))

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ rq_modified_begin(ಸಂರಚನಾ rq *rq, ಧ್ರುವ ಸಂರಚನಾ sched_class *class)
{
	ಯದಿ (sched_class_above(rq->next_class, class))
		rq->next_class = class;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool rq_modified_above(ಸಂರಚನಾ rq *rq, ಧ್ರುವ ಸಂರಚನಾ sched_class *class)
{
	ಪ್ರತ್ಯಾಗಮ sched_class_above(rq->next_class, class);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool sched_stop_runnable(ಸಂರಚನಾ rq *rq)
{
	ಪ್ರತ್ಯಾಗಮ rq->stop && task_on_rq_queued(rq->stop);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool sched_dl_runnable(ಸಂರಚನಾ rq *rq)
{
	ಪ್ರತ್ಯಾಗಮ rq->dl.dl_nr_running > 0;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool sched_rt_runnable(ಸಂರಚನಾ rq *rq)
{
	ಪ್ರತ್ಯಾಗಮ rq->rt.rt_queued > 0;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool sched_fair_runnable(ಸಂರಚನಾ rq *rq)
{
	ಪ್ರತ್ಯಾಗಮ rq->cfs.nr_queued > 0;
}

ಬಾಹ್ಯ ಸಂರಚನಾ task_struct *pick_task_fair(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ rq_flags *rf);
ಬಾಹ್ಯ ಸಂರಚನಾ task_struct *pick_task_idle(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ rq_flags *rf);

#define SCA_CHECK		0x01
#define SCA_MIGRATE_DISABLE	0x02
#define SCA_MIGRATE_ENABLE	0x04
#define SCA_USER		0x08

ಬಾಹ್ಯ ಶೂನ್ಯ update_group_capacity(ಸಂರಚನಾ sched_domain *sd, ಪೂರ್ಣಾಙ್ಕ cpu);

ಬಾಹ್ಯ ಶೂನ್ಯ sched_balance_trigger(ಸಂರಚನಾ rq *rq);

ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ __set_cpus_allowed_ptr(ಸಂರಚನಾ task_struct *p, ಸಂರಚನಾ affinity_context *ctx);
ಬಾಹ್ಯ ಶೂನ್ಯ set_cpus_allowed_common(ಸಂರಚನಾ task_struct *p, ಸಂರಚನಾ affinity_context *ctx);

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool task_allowed_on_cpu(ಸಂರಚನಾ task_struct *p, ಪೂರ್ಣಾಙ್ಕ cpu)
{
	/* When not in the task's cpumask, no point in looking further. */
	ಯದಿ (!cpumask_test_cpu(cpu, p->cpus_ptr))
		ಪ್ರತ್ಯಾಗಮ false;

	/* Can @cpu run a user thread? */
	ಯದಿ (!(p->flags & PF_KTHREAD) && !task_cpu_possible(cpu, p))
		ಪ್ರತ್ಯಾಗಮ false;

	ಪ್ರತ್ಯಾಗಮ true;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ cpumask_t *alloc_user_cpus_ptr(ಪೂರ್ಣಾಙ್ಕ ಗಂಟು)
{
	/*
	 * See set_cpus_allowed_force() above for the rcu_head usage.
	 */
	ಪೂರ್ಣಾಙ್ಕ size = max_t(ಪೂರ್ಣಾಙ್ಕ, cpumask_size(), ಪರಿಮಾಣ(ಸಂರಚನಾ rcu_head));

	ಪ್ರತ್ಯಾಗಮ kmalloc_ಗಂಟು(size, GFP_KERNEL, ಗಂಟು);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ task_struct *get_ತಳ್ಳು_task(ಸಂರಚನಾ rq *rq)
{
	ಸಂರಚನಾ task_struct *p = rq->donor;

	lockdep_assert_rq_held(rq);

	ಯದಿ (rq->ತಳ್ಳು_busy)
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

	ಯದಿ (p->nr_cpus_allowed == 1)
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

	ಯದಿ (p->migration_disabled)
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

	rq->ತಳ್ಳು_busy = true;
	ಪ್ರತ್ಯಾಗಮ get_task_struct(p);
}

ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ ತಳ್ಳು_cpu_stop(ಶೂನ್ಯ *arg);

#ifdef CONFIG_CPU_IDLE

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ idle_set_state(ಸಂರಚನಾ rq *rq,
				  ಸಂರಚನಾ cpuidle_state *idle_state)
{
	rq->idle_state = idle_state;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ cpuidle_state *idle_get_state(ಸಂರಚನಾ rq *rq)
{
	lockdep_assert(rcu_read_lock_any_held());

	ಪ್ರತ್ಯಾಗಮ rq->idle_state;
}

#ಅನ್ಯಥಾ /* !CONFIG_CPU_IDLE: */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ idle_set_state(ಸಂರಚನಾ rq *rq,
				  ಸಂರಚನಾ cpuidle_state *idle_state)
{
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಸಂರಚನಾ cpuidle_state *idle_get_state(ಸಂರಚನಾ rq *rq)
{
	ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

#endif /* !CONFIG_CPU_IDLE */

ಬಾಹ್ಯ ಶೂನ್ಯ schedule_idle(ಶೂನ್ಯ);
asmlinkage ಶೂನ್ಯ schedule_user(ಶೂನ್ಯ);

ಬಾಹ್ಯ ಶೂನ್ಯ sysrq_sched_debug_show(ಶೂನ್ಯ);
ಬಾಹ್ಯ ಶೂನ್ಯ sched_init_granularity(ಶೂನ್ಯ);
ಬಾಹ್ಯ ಶೂನ್ಯ update_max_interval(ಶೂನ್ಯ);

ಬಾಹ್ಯ ಶೂನ್ಯ init_sched_dl_class(ಶೂನ್ಯ);
ಬಾಹ್ಯ ಶೂನ್ಯ init_sched_rt_class(ಶೂನ್ಯ);
ಬಾಹ್ಯ ಶೂನ್ಯ init_sched_fair_class(ಶೂನ್ಯ);

ಬಾಹ್ಯ ಶೂನ್ಯ resched_curr(ಸಂರಚನಾ rq *rq);
ಬಾಹ್ಯ ಶೂನ್ಯ resched_curr_lazy(ಸಂರಚನಾ rq *rq);
ಬಾಹ್ಯ ಶೂನ್ಯ resched_cpu(ಪೂರ್ಣಾಙ್ಕ cpu);

ಬಾಹ್ಯ ಶೂನ್ಯ init_rt_bandwidth(ಸಂರಚನಾ rt_bandwidth *rt_b, u64 period, u64 runtime);
ಬಾಹ್ಯ bool sched_rt_bandwidth_account(ಸಂರಚನಾ rt_rq *rt_rq);

ಬಾಹ್ಯ ಶೂನ್ಯ init_dl_entity(ಸಂರಚನಾ sched_dl_entity *dl_se);

ಬಾಹ್ಯ ಶೂನ್ಯ init_cfs_throttle_work(ಸಂರಚನಾ task_struct *p);

#define BW_SHIFT		20
#define BW_UNIT			(1 << BW_SHIFT)
#define RATIO_SHIFT		8
#define MAX_BW_BITS		(64 - BW_SHIFT)
#define MAX_BW			((1ULL << MAX_BW_BITS) - 1)

ಬಾಹ್ಯ u64 to_ratio(u64 period, u64 runtime);

ಬಾಹ್ಯ ಶೂನ್ಯ init_entity_runnable_average(ಸಂರಚನಾ sched_entity *se);
ಬಾಹ್ಯ ಶೂನ್ಯ post_init_entity_util_avg(ಸಂರಚನಾ task_struct *p);

#ifdef CONFIG_NO_HZ_FULL
ಬಾಹ್ಯ bool sched_can_stop_tick(ಸಂರಚನಾ rq *rq);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ __init sched_tick_offload_init(ಶೂನ್ಯ);

/*
 * Tick may be needed by tasks in the runqueue depending on their policy and
 * requirements. If tick is needed, lets send the target an IPI to kick it out of
 * nohz mode if necessary.
 */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ sched_update_tick_dependency(ಸಂರಚನಾ rq *rq)
{
	ಪೂರ್ಣಾಙ್ಕ cpu = cpu_of(rq);

	ಯದಿ (!tick_nohz_full_cpu(cpu))
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (sched_can_stop_tick(rq))
		tick_nohz_dep_clear_cpu(cpu, TICK_DEP_BIT_SCHED);
	ಅನ್ಯಥಾ
		tick_nohz_dep_set_cpu(cpu, TICK_DEP_BIT_SCHED);
}
#ಅನ್ಯಥಾ /* !CONFIG_NO_HZ_FULL: */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ sched_tick_offload_init(ಶೂನ್ಯ) { ಪ್ರತ್ಯಾಗಮ 0; }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ sched_update_tick_dependency(ಸಂರಚನಾ rq *rq) { }
#endif /* !CONFIG_NO_HZ_FULL */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ add_nr_running(ಸಂರಚನಾ rq *rq, unsigned ಎಣಿಕೆ)
{
	unsigned prev_nr = rq->nr_running;

	rq->nr_running = prev_nr + ಎಣಿಕೆ;
	ಯದಿ (trace_sched_update_nr_running_tp_enabled()) {
		call_trace_sched_update_nr_running(rq, ಎಣಿಕೆ);
	}

	ಯದಿ (prev_nr < 2 && rq->nr_running >= 2)
		set_rd_overloaded(rq->rd, 1);

	sched_update_tick_dependency(rq);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ sub_nr_running(ಸಂರಚನಾ rq *rq, unsigned ಎಣಿಕೆ)
{
	rq->nr_running -= ಎಣಿಕೆ;
	ಯದಿ (trace_sched_update_nr_running_tp_enabled()) {
		call_trace_sched_update_nr_running(rq, -ಎಣಿಕೆ);
	}

	/* Check if we still need preemption */
	sched_update_tick_dependency(rq);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ __block_task(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p)
{
	ಯದಿ (p->sched_contributes_to_load)
		rq->nr_uninterruptible++;

	ಯದಿ (p->in_iowait) {
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

ಬಾಹ್ಯ ಶೂನ್ಯ activate_task(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p, ಪೂರ್ಣಾಙ್ಕ flags);
ಬಾಹ್ಯ ಶೂನ್ಯ deactivate_task(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p, ಪೂರ್ಣಾಙ್ಕ flags);

ಬಾಹ್ಯ ಶೂನ್ಯ wakeup_preempt(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p, ಪೂರ್ಣಾಙ್ಕ flags);

/*
 * attach_task() -- attach the task detached by detach_task() to its new rq.
 */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ attach_task(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p)
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
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ attach_one_task(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p)
{
	guard(rq_lock)(rq);
	update_rq_clock(rq);
	attach_task(rq, p);
}

/*
 * __attach_tasks() - attaches a list of tasks (using se.group_node) to
 * the new rq
 */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ __attach_tasks(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ list_head *tasks)
{
	guard(rq_lock)(rq);
	update_rq_clock(rq);

	ಯಾವತ್ (!list_empty(tasks)) {
		ಸಂರಚನಾ task_struct *p;

		p = list_first_entry(tasks, ಸಂರಚನಾ task_struct, se.group_ಗಂಟು);
		list_del_init(&p->se.group_ಗಂಟು);

		attach_task(rq, p);
	}
}

#ifdef CONFIG_PREEMPT_RT
# define SCHED_NR_MIGRATE_BREAK 8
#ಅನ್ಯಥಾ
# define SCHED_NR_MIGRATE_BREAK 32
#endif

ಬಾಹ್ಯ __read_mostly unsigned ಪೂರ್ಣಾಙ್ಕ sysctl_sched_nr_migrate;
ಬಾಹ್ಯ __read_mostly unsigned ಪೂರ್ಣಾಙ್ಕ sysctl_sched_migration_cost;

ಬಾಹ್ಯ unsigned ಪೂರ್ಣಾಙ್ಕ sysctl_sched_base_slice;

ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ sysctl_resched_latency_warn_ms;
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ sysctl_resched_latency_warn_once;

ಬಾಹ್ಯ unsigned ಪೂರ್ಣಾಙ್ಕ sysctl_sched_tunable_scaling;

ಬಾಹ್ಯ unsigned ಪೂರ್ಣಾಙ್ಕ sysctl_numa_balancing_scan_delay;
ಬಾಹ್ಯ unsigned ಪೂರ್ಣಾಙ್ಕ sysctl_numa_balancing_scan_period_min;
ಬಾಹ್ಯ unsigned ಪೂರ್ಣಾಙ್ಕ sysctl_numa_balancing_scan_period_max;
ಬಾಹ್ಯ unsigned ಪೂರ್ಣಾಙ್ಕ sysctl_numa_balancing_scan_size;
ಬಾಹ್ಯ unsigned ಪೂರ್ಣಾಙ್ಕ sysctl_numa_balancing_hot_threshold;

#ifdef CONFIG_SCHED_HRTICK

/*
 * Use hrtick when:
 *  - enabled by features
 *  - hrtimer is actually high res
 */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool hrtick_enabled(ಸಂರಚನಾ rq *rq)
{
	ಪ್ರತ್ಯಾಗಮ cpu_active(cpu_of(rq)) && hrtimer_highres_enabled();
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool hrtick_enabled_fair(ಸಂರಚನಾ rq *rq)
{
	ಪ್ರತ್ಯಾಗಮ sched_feat(HRTICK) && hrtick_enabled(rq);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool hrtick_enabled_dl(ಸಂರಚನಾ rq *rq)
{
	ಪ್ರತ್ಯಾಗಮ sched_feat(HRTICK_DL) && hrtick_enabled(rq);
}

ಬಾಹ್ಯ ಶೂನ್ಯ hrtick_start(ಸಂರಚನಾ rq *rq, u64 delay);
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool hrtick_active(ಸಂರಚನಾ rq *rq)
{
	ಪ್ರತ್ಯಾಗಮ hrtimer_active(&rq->hrtick_timer);
}

#ಅನ್ಯಥಾ /* !CONFIG_SCHED_HRTICK: */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool hrtick_enabled_fair(ಸಂರಚನಾ rq *rq) { ಪ್ರತ್ಯಾಗಮ false; }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool hrtick_enabled_dl(ಸಂರಚನಾ rq *rq) { ಪ್ರತ್ಯಾಗಮ false; }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool hrtick_enabled(ಸಂರಚನಾ rq *rq) { ಪ್ರತ್ಯಾಗಮ false; }
#endif /* !CONFIG_SCHED_HRTICK */

#ifndef arch_scale_freq_tick
ಸ್ಥಿರ __always_inline ಶೂನ್ಯ arch_scale_freq_tick(ಶೂನ್ಯ) { }
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
ಸ್ಥಿರ __always_inline
unsigned long arch_scale_freq_capacity(ಪೂರ್ಣಾಙ್ಕ cpu)
{
	ಪ್ರತ್ಯಾಗಮ SCHED_CAPACITY_SCALE;
}
#endif

/*
 * In double_lock_balance()/double_rq_lock(), we use raw_spin_rq_lock() to
 * acquire rq lock instead of rq_lock(). So at the end of these two functions
 * we need to call double_rq_clock_clear_update() to clear RQCF_UPDATED of
 * rq->clock_update_flags to avoid the WARN_DOUBLE_CLOCK warning.
 */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ double_rq_clock_clear_update(ಸಂರಚನಾ rq *rq1, ಸಂರಚನಾ rq *rq2)
{
	rq1->clock_update_flags &= (RQCF_REQ_SKIP|RQCF_ACT_SKIP);
	rq2->clock_update_flags &= (RQCF_REQ_SKIP|RQCF_ACT_SKIP);
}

#define DEFINE_LOCK_GUARD_2(name, type, _lock, _unlock, ...)				\
__DEFINE_UNLOCK_GUARD(name, type, _unlock, type *lock2; __VA_ARGS__)			\
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ class_##name##_t class_##name##_constructor(type *lock, type *lock2)	\
	__no_context_analysis								\
{ class_##name##_t _t = { .lock = lock, .lock2 = lock2 }, *_T = &_t;			\
  _lock; ಪ್ರತ್ಯಾಗಮ _t; }
#define DECLARE_LOCK_GUARD_2_ATTRS(_name, _lock, _unlock1, _unlock2)			\
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ class_##_name##_t class_##_name##_constructor(lock_##_name##_t *_T1,	\
							    lock_##_name##_t *_T2) _lock; \
ಸ್ಥಿರ __always_inline ಶೂನ್ಯ __class_##_name##_cleanup_ctx1(class_##_name##_t **_T1)	\
	__no_context_analysis _unlock1 { }						\
ಸ್ಥಿರ __always_inline ಶೂನ್ಯ __class_##_name##_cleanup_ctx2(class_##_name##_t **_T2)	\
	__no_context_analysis _unlock2 { }
#define WITH_LOCK_GUARD_2_ATTRS(_name, _T1, _T2)					\
	class_##_name##_constructor(_T1, _T2),						\
	*__UNIQUE_ID(unlock1) __cleanup(__class_##_name##_cleanup_ctx1) = (ಶೂನ್ಯ *)(_T1),\
	*__UNIQUE_ID(unlock2) __cleanup(__class_##_name##_cleanup_ctx2) = (ಶೂನ್ಯ *)(_T2)

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool rq_order_less(ಸಂರಚನಾ rq *rq1, ಸಂರಚನಾ rq *rq2)
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
	ಯದಿ (rq1->core->cpu < rq2->core->cpu)
		ಪ್ರತ್ಯಾಗಮ true;
	ಯದಿ (rq1->core->cpu > rq2->core->cpu)
		ಪ್ರತ್ಯಾಗಮ false;

	/*
	 * __sched_core_flip() relies on SMT having cpu-id lock order.
	 */
#endif /* CONFIG_SCHED_CORE */
	ಪ್ರತ್ಯಾಗಮ rq1->cpu < rq2->cpu;
}

ಬಾಹ್ಯ ಶೂನ್ಯ double_rq_lock(ಸಂರಚನಾ rq *rq1, ಸಂರಚನಾ rq *rq2)
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
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ _double_lock_balance(ಸಂರಚನಾ rq *this_rq, ಸಂರಚನಾ rq *busiest)
	__must_hold(__rq_lockp(this_rq))
	__acquires(__rq_lockp(busiest))
{
	raw_spin_rq_unlock(this_rq);
	double_rq_lock(this_rq, busiest);

	ಪ್ರತ್ಯಾಗಮ 1;
}

#ಅನ್ಯಥಾ /* !CONFIG_PREEMPTION: */
/*
 * Unfair double_lock_balance: Optimizes throughput at the expense of
 * latency by eliminating extra atomic operations when the locks are
 * already in proper order on entry.  This favors lower CPU-ids and will
 * grant the double lock to lower CPUs over higher ids under contention,
 * regardless of entry order into the function.
 */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ _double_lock_balance(ಸಂರಚನಾ rq *this_rq, ಸಂರಚನಾ rq *busiest)
	__must_hold(__rq_lockp(this_rq))
	__acquires(__rq_lockp(busiest))
{
	ಯದಿ (__rq_lockp(this_rq) == __rq_lockp(busiest)) {
		__acquire(__rq_lockp(busiest)); /* already held */
		double_rq_clock_clear_update(this_rq, busiest);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (likely(raw_spin_rq_trylock(busiest))) {
		double_rq_clock_clear_update(this_rq, busiest);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (rq_order_less(this_rq, busiest)) {
		raw_spin_rq_lock_nested(busiest, SINGLE_DEPTH_NESTING);
		double_rq_clock_clear_update(this_rq, busiest);
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	raw_spin_rq_unlock(this_rq);
	double_rq_lock(this_rq, busiest);

	ಪ್ರತ್ಯಾಗಮ 1;
}

#endif /* !CONFIG_PREEMPTION */

/*
 * double_lock_balance - lock the busiest runqueue, this_rq is locked already.
 */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ double_lock_balance(ಸಂರಚನಾ rq *this_rq, ಸಂರಚನಾ rq *busiest)
	__must_hold(__rq_lockp(this_rq))
	__acquires(__rq_lockp(busiest))
{
	lockdep_assert_irqs_disabled();

	ಪ್ರತ್ಯಾಗಮ _double_lock_balance(this_rq, busiest);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ double_unlock_balance(ಸಂರಚನಾ rq *this_rq, ಸಂರಚನಾ rq *busiest)
	__releases(__rq_lockp(busiest))
{
	ಯದಿ (__rq_lockp(this_rq) != __rq_lockp(busiest))
		raw_spin_rq_unlock(busiest);
	ಅನ್ಯಥಾ
		__release(__rq_lockp(busiest)); /* fake release */
	lock_set_subclass(&__rq_lockp(this_rq)->dep_map, 0, _RET_IP_);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ double_lock(spinlock_t *l1, spinlock_t *l2)
	__acquires(l1, l2)
{
	ಯದಿ (l1 > l2)
		swap(l1, l2);

	spin_lock(l1);
	spin_lock_nested(l2, SINGLE_DEPTH_NESTING);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ double_lock_irq(spinlock_t *l1, spinlock_t *l2)
	__acquires(l1, l2)
{
	ಯದಿ (l1 > l2)
		swap(l1, l2);

	spin_lock_irq(l1);
	spin_lock_nested(l2, SINGLE_DEPTH_NESTING);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ double_raw_lock(raw_spinlock_t *l1, raw_spinlock_t *l2)
	__acquires(l1, l2)
{
	ಯದಿ (l1 > l2)
		swap(l1, l2);

	raw_spin_lock(l1);
	raw_spin_lock_nested(l2, SINGLE_DEPTH_NESTING);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ double_raw_unlock(raw_spinlock_t *l1, raw_spinlock_t *l2)
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
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ double_rq_unlock(ಸಂರಚನಾ rq *rq1, ಸಂರಚನಾ rq *rq2)
	__releases(__rq_lockp(rq1), __rq_lockp(rq2))
{
	ಯದಿ (__rq_lockp(rq1) != __rq_lockp(rq2))
		raw_spin_rq_unlock(rq2);
	ಅನ್ಯಥಾ
		__release(__rq_lockp(rq2)); /* fake release */
	raw_spin_rq_unlock(rq1);
}

ಬಾಹ್ಯ ಶೂನ್ಯ set_rq_online (ಸಂರಚನಾ rq *rq);
ಬಾಹ್ಯ ಶೂನ್ಯ set_rq_offline(ಸಂರಚನಾ rq *rq);

ಬಾಹ್ಯ bool sched_smp_initialized;

DEFINE_LOCK_GUARD_2(double_rq_lock, ಸಂರಚನಾ rq,
		    double_rq_lock(_T->lock, _T->lock2),
		    double_rq_unlock(_T->lock, _T->lock2))

ಬಾಹ್ಯ ಸಂರಚನಾ sched_entity *__pick_root_entity(ಸಂರಚನಾ cfs_rq *cfs_rq);
ಬಾಹ್ಯ ಸಂರಚನಾ sched_entity *__pick_first_entity(ಸಂರಚನಾ cfs_rq *cfs_rq);
ಬಾಹ್ಯ ಸಂರಚನಾ sched_entity *__pick_last_entity(ಸಂರಚನಾ cfs_rq *cfs_rq);

ಬಾಹ್ಯ bool sched_debug_verbose;

ಬಾಹ್ಯ ಶೂನ್ಯ print_cfs_stats(ಸಂರಚನಾ seq_file *m, ಪೂರ್ಣಾಙ್ಕ cpu);
ಬಾಹ್ಯ ಶೂನ್ಯ print_rt_stats(ಸಂರಚನಾ seq_file *m, ಪೂರ್ಣಾಙ್ಕ cpu);
ಬಾಹ್ಯ ಶೂನ್ಯ print_dl_stats(ಸಂರಚನಾ seq_file *m, ಪೂರ್ಣಾಙ್ಕ cpu);
ಬಾಹ್ಯ ಶೂನ್ಯ print_cfs_rq(ಸಂರಚನಾ seq_file *m, ಪೂರ್ಣಾಙ್ಕ cpu, ಸಂರಚನಾ cfs_rq *cfs_rq);
ಬಾಹ್ಯ ಶೂನ್ಯ print_rt_rq(ಸಂರಚನಾ seq_file *m, ಪೂರ್ಣಾಙ್ಕ cpu, ಸಂರಚನಾ rt_rq *rt_rq);
ಬಾಹ್ಯ ಶೂನ್ಯ print_dl_rq(ಸಂರಚನಾ seq_file *m, ಪೂರ್ಣಾಙ್ಕ cpu, ಸಂರಚನಾ dl_rq *dl_rq);

ಬಾಹ್ಯ ಶೂನ್ಯ resched_latency_warn(ಪೂರ್ಣಾಙ್ಕ cpu, u64 latency);

#ifdef CONFIG_NUMA_BALANCING
ಬಾಹ್ಯ ಶೂನ್ಯ show_numa_stats(ಸಂರಚನಾ task_struct *p, ಸಂರಚನಾ seq_file *m);
ಬಾಹ್ಯ ಶೂನ್ಯ
print_numa_stats(ಸಂರಚನಾ seq_file *m, ಪೂರ್ಣಾಙ್ಕ ಗಂಟು, unsigned long tsf,
		 unsigned long tpf, unsigned long gsf, unsigned long gpf);
#endif /* CONFIG_NUMA_BALANCING */

ಬಾಹ್ಯ ಶೂನ್ಯ init_cfs_rq(ಸಂರಚನಾ cfs_rq *cfs_rq);
ಬಾಹ್ಯ ಶೂನ್ಯ init_rt_rq(ಸಂರಚನಾ rt_rq *rt_rq);
ಬಾಹ್ಯ ಶೂನ್ಯ init_dl_rq(ಸಂರಚನಾ dl_rq *dl_rq);

ಬಾಹ್ಯ ಶೂನ್ಯ cfs_bandwidth_usage_inc(ಶೂನ್ಯ);
ಬಾಹ್ಯ ಶೂನ್ಯ cfs_bandwidth_usage_dec(ಶೂನ್ಯ);

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

ಬಾಹ್ಯ ಶೂನ್ಯ nohz_balance_exit_idle(ಸಂರಚನಾ rq *rq);
#ಅನ್ಯಥಾ /* !CONFIG_NO_HZ_COMMON: */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ nohz_balance_exit_idle(ಸಂರಚನಾ rq *rq) { }
#endif /* !CONFIG_NO_HZ_COMMON */

#ifdef CONFIG_NO_HZ_COMMON
ಬಾಹ್ಯ ಶೂನ್ಯ nohz_run_idle_balance(ಪೂರ್ಣಾಙ್ಕ cpu);
#ಅನ್ಯಥಾ
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ nohz_run_idle_balance(ಪೂರ್ಣಾಙ್ಕ cpu) { }
#endif

#ಅನ್ತರ್ಭಾವ "stats.h"

#ಯದಿ defined(CONFIG_SCHED_CORE) && defined(CONFIG_SCHEDSTATS)

ಬಾಹ್ಯ ಶೂನ್ಯ __sched_core_account_forceidle(ಸಂರಚನಾ rq *rq);

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ sched_core_account_forceidle(ಸಂರಚನಾ rq *rq)
{
	ಯದಿ (schedstat_enabled())
		__sched_core_account_forceidle(rq);
}

ಬಾಹ್ಯ ಶೂನ್ಯ __sched_core_tick(ಸಂರಚನಾ rq *rq);

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ sched_core_tick(ಸಂರಚನಾ rq *rq)
{
	ಯದಿ (sched_core_enabled(rq) && schedstat_enabled())
		__sched_core_tick(rq);
}

#ಅನ್ಯಥಾ /* !(CONFIG_SCHED_CORE && CONFIG_SCHEDSTATS): */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ sched_core_account_forceidle(ಸಂರಚನಾ rq *rq) { }

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ sched_core_tick(ಸಂರಚನಾ rq *rq) { }

#endif /* !(CONFIG_SCHED_CORE && CONFIG_SCHEDSTATS) */

#ifdef CONFIG_IRQ_TIME_ACCOUNTING

ಸಂರಚನಾ irqtime {
	u64			total;
	u64			tick_delta;
	u64			irq_start_time;
	ಸಂರಚನಾ u64_stats_sync	sync;
};

DECLARE_PER_CPU(ಸಂರಚನಾ irqtime, cpu_irqtime);
DECLARE_STATIC_ಕೀಲಿ_FALSE(sched_clock_irqtime);

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ irqtime_enabled(ಶೂನ್ಯ)
{
	ಪ್ರತ್ಯಾಗಮ static_branch_likely(&sched_clock_irqtime);
}

/*
 * Returns the irqtime minus the softirq time computed by ksoftirqd.
 * Otherwise ksoftirqd's sum_exec_runtime is subtracted its own runtime
 * and never move forward.
 */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ u64 irq_time_read(ಪೂರ್ಣಾಙ್ಕ cpu)
{
	ಸಂರಚನಾ irqtime *irqtime = &per_cpu(cpu_irqtime, cpu);
	unsigned ಪೂರ್ಣಾಙ್ಕ seq;
	u64 total;

	ಕುರು {
		seq = __u64_stats_fetch_begin(&irqtime->sync);
		total = irqtime->total;
	} ಯಾವತ್ (__u64_stats_fetch_retry(&irqtime->sync, seq));

	ಪ್ರತ್ಯಾಗಮ total;
}

#ಅನ್ಯಥಾ /* !CONFIG_IRQ_TIME_ACCOUNTING: */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ irqtime_enabled(ಶೂನ್ಯ)
{
	ಪ್ರತ್ಯಾಗಮ 0;
}

#endif /* !CONFIG_IRQ_TIME_ACCOUNTING */

#ifdef CONFIG_CPU_FREQ

DECLARE_PER_CPU(ಸಂರಚನಾ update_util_data __rcu *, cpufreq_update_util_data);

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
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ cpufreq_update_util(ಸಂರಚನಾ rq *rq, unsigned ಪೂರ್ಣಾಙ್ಕ flags)
{
	ಸಂರಚನಾ update_util_data *data;

	data = rcu_dereference_sched(*per_cpu_ptr(&cpufreq_update_util_data,
						  cpu_of(rq)));
	ಯದಿ (data)
		data->func(data, rq_clock(rq), flags);
}
#ಅನ್ಯಥಾ /* !CONFIG_CPU_FREQ: */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ cpufreq_update_util(ಸಂರಚನಾ rq *rq, unsigned ಪೂರ್ಣಾಙ್ಕ flags) { }
#endif /* !CONFIG_CPU_FREQ */

#ifdef arch_scale_freq_capacity
# ifndef arch_scale_freq_invariant
#  define arch_scale_freq_invariant()	true
# endif
#ಅನ್ಯಥಾ
# define arch_scale_freq_invariant()	false
#endif

unsigned long effective_cpu_util(ಪೂರ್ಣಾಙ್ಕ cpu, unsigned long util_cfs,
				 unsigned long *min,
				 unsigned long *max);

unsigned long sugov_effective_cpu_perf(ಪೂರ್ಣಾಙ್ಕ cpu, unsigned long actual,
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
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool dl_task_fits_capacity(ಸಂರಚನಾ task_struct *p, ಪೂರ್ಣಾಙ್ಕ cpu)
{
	unsigned long cap = arch_scale_cpu_capacity(cpu);

	ಪ್ರತ್ಯಾಗಮ cap >= p->dl.dl_density >> (BW_SHIFT - SCHED_CAPACITY_SHIFT);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ unsigned long cpu_bw_dl(ಸಂರಚನಾ rq *rq)
{
	ಪ್ರತ್ಯಾಗಮ (rq->dl.running_bw * SCHED_CAPACITY_SCALE) >> BW_SHIFT;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ unsigned long cpu_util_dl(ಸಂರಚನಾ rq *rq)
{
	ಪ್ರತ್ಯಾಗಮ READ_ONCE(rq->avg_dl.util_avg);
}


ಬಾಹ್ಯ unsigned long cpu_util_cfs(ಪೂರ್ಣಾಙ್ಕ cpu);
ಬಾಹ್ಯ unsigned long cpu_util_cfs_boost(ಪೂರ್ಣಾಙ್ಕ cpu);

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ unsigned long cpu_util_rt(ಸಂರಚನಾ rq *rq)
{
	ಪ್ರತ್ಯಾಗಮ READ_ONCE(rq->avg_rt.util_avg);
}

#ifdef CONFIG_UCLAMP_TASK

unsigned long uclamp_eff_ಮೌಲ್ಯ(ಸಂರಚನಾ task_struct *p, ಗಣನಾ uclamp_id clamp_id);

/*
 * When uclamp is compiled in, the aggregation at rq level is 'turned off'
 * by default in the fast path and only gets turned on once userspace performs
 * an operation that requires it.
 *
 * Returns true if userspace opted-in to use uclamp and aggregation at rq level
 * hence is active.
 */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool uclamp_is_used(ಶೂನ್ಯ)
{
	ಪ್ರತ್ಯಾಗಮ static_branch_likely(&sched_uclamp_used);
}

/*
 * Enabling static branches would get the cpus_read_lock(),
 * check whether uclamp_is_used before enable it to avoid always
 * calling cpus_read_lock(). Because we never disable this
 * static key once enable it.
 */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ sched_uclamp_enable(ಶೂನ್ಯ)
{
	ಯದಿ (!uclamp_is_used())
		static_branch_enable(&sched_uclamp_used);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ unsigned long uclamp_rq_get(ಸಂರಚನಾ rq *rq,
					  ಗಣನಾ uclamp_id clamp_id)
{
	ಪ್ರತ್ಯಾಗಮ READ_ONCE(rq->uclamp[clamp_id].ಮೌಲ್ಯ);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ uclamp_rq_set(ಸಂರಚನಾ rq *rq, ಗಣನಾ uclamp_id clamp_id,
				 unsigned ಪೂರ್ಣಾಙ್ಕ ಮೌಲ್ಯ)
{
	WRITE_ONCE(rq->uclamp[clamp_id].ಮೌಲ್ಯ, ಮೌಲ್ಯ);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool uclamp_rq_is_idle(ಸಂರಚನಾ rq *rq)
{
	ಪ್ರತ್ಯಾಗಮ rq->uclamp_flags & UCLAMP_FLAG_IDLE;
}

/* Is the rq being capped/throttled by uclamp_max? */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool uclamp_rq_is_capped(ಸಂರಚನಾ rq *rq)
{
	unsigned long rq_util;
	unsigned long max_util;

	ಯದಿ (!uclamp_is_used())
		ಪ್ರತ್ಯಾಗಮ false;

	rq_util = cpu_util_cfs(cpu_of(rq)) + cpu_util_rt(rq);
	max_util = READ_ONCE(rq->uclamp[UCLAMP_MAX].ಮೌಲ್ಯ);

	ಪ್ರತ್ಯಾಗಮ max_util != SCHED_CAPACITY_SCALE && rq_util >= max_util;
}

#define for_each_clamp_id(clamp_id) \
	ಪ್ರತಿ ((clamp_id) = 0; (clamp_id) < UCLAMP_CNT; (clamp_id)++)

ಬಾಹ್ಯ unsigned ಪೂರ್ಣಾಙ್ಕ sysctl_sched_uclamp_util_min_rt_default;


ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ unsigned ಪೂರ್ಣಾಙ್ಕ uclamp_none(ಗಣನಾ uclamp_id clamp_id)
{
	ಯದಿ (clamp_id == UCLAMP_MIN)
		ಪ್ರತ್ಯಾಗಮ 0;
	ಪ್ರತ್ಯಾಗಮ SCHED_CAPACITY_SCALE;
}

/* Integer rounded range for each bucket */
#define UCLAMP_BUCKET_DELTA DIV_ROUND_CLOSEST(SCHED_CAPACITY_SCALE, UCLAMP_BUCKETS)

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ unsigned ಪೂರ್ಣಾಙ್ಕ uclamp_bucket_id(unsigned ಪೂರ್ಣಾಙ್ಕ clamp_ಮೌಲ್ಯ)
{
	ಪ್ರತ್ಯಾಗಮ min_t(unsigned ಪೂರ್ಣಾಙ್ಕ, clamp_ಮೌಲ್ಯ / UCLAMP_BUCKET_DELTA, UCLAMP_BUCKETS - 1);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
uclamp_se_set(ಸಂರಚನಾ uclamp_se *uc_se, unsigned ಪೂರ್ಣಾಙ್ಕ ಮೌಲ್ಯ, bool user_defined)
{
	uc_se->ಮೌಲ್ಯ = ಮೌಲ್ಯ;
	uc_se->bucket_id = uclamp_bucket_id(ಮೌಲ್ಯ);
	uc_se->user_defined = user_defined;
}

#ಅನ್ಯಥಾ /* !CONFIG_UCLAMP_TASK: */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ unsigned long
uclamp_eff_ಮೌಲ್ಯ(ಸಂರಚನಾ task_struct *p, ಗಣನಾ uclamp_id clamp_id)
{
	ಯದಿ (clamp_id == UCLAMP_MIN)
		ಪ್ರತ್ಯಾಗಮ 0;

	ಪ್ರತ್ಯಾಗಮ SCHED_CAPACITY_SCALE;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool uclamp_rq_is_capped(ಸಂರಚನಾ rq *rq) { ಪ್ರತ್ಯಾಗಮ false; }

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool uclamp_is_used(ಶೂನ್ಯ)
{
	ಪ್ರತ್ಯಾಗಮ false;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ sched_uclamp_enable(ಶೂನ್ಯ) {}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ unsigned long
uclamp_rq_get(ಸಂರಚನಾ rq *rq, ಗಣನಾ uclamp_id clamp_id)
{
	ಯದಿ (clamp_id == UCLAMP_MIN)
		ಪ್ರತ್ಯಾಗಮ 0;

	ಪ್ರತ್ಯಾಗಮ SCHED_CAPACITY_SCALE;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
uclamp_rq_set(ಸಂರಚನಾ rq *rq, ಗಣನಾ uclamp_id clamp_id, unsigned ಪೂರ್ಣಾಙ್ಕ ಮೌಲ್ಯ)
{
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool uclamp_rq_is_idle(ಸಂರಚನಾ rq *rq)
{
	ಪ್ರತ್ಯಾಗಮ false;
}

#endif /* !CONFIG_UCLAMP_TASK */

#ifdef CONFIG_HAVE_SCHED_AVG_IRQ

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ unsigned long cpu_util_irq(ಸಂರಚನಾ rq *rq)
{
	ಪ್ರತ್ಯಾಗಮ READ_ONCE(rq->avg_irq.util_avg);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ
unsigned long scale_irq_capacity(unsigned long util, unsigned long irq, unsigned long max)
{
	util *= (max - irq);
	util /= max;

	ಪ್ರತ್ಯಾಗಮ util;

}

#ಅನ್ಯಥಾ /* !CONFIG_HAVE_SCHED_AVG_IRQ: */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ unsigned long cpu_util_irq(ಸಂರಚನಾ rq *rq)
{
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ
unsigned long scale_irq_capacity(unsigned long util, unsigned long irq, unsigned long max)
{
	ಪ್ರತ್ಯಾಗಮ util;
}

#endif /* !CONFIG_HAVE_SCHED_AVG_IRQ */

ಬಾಹ್ಯ ಶೂನ್ಯ __setparam_fair(ಸಂರಚನಾ task_struct *p, ಧ್ರುವ ಸಂರಚನಾ sched_attr *attr);

#ಯದಿ defined(CONFIG_ENERGY_MODEL) && defined(CONFIG_CPU_FREQ_GOV_SCHEDUTIL)

#define perf_domain_span(pd) (to_cpumask(((pd)->em_pd->cpus)))

DECLARE_STATIC_ಕೀಲಿ_FALSE(sched_energy_present);

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool sched_energy_enabled(ಶೂನ್ಯ)
{
	ಪ್ರತ್ಯಾಗಮ static_branch_unlikely(&sched_energy_present);
}

#ಅನ್ಯಥಾ /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL): */

#define perf_domain_span(pd) ರಿಕ್ತ

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool sched_energy_enabled(ಶೂನ್ಯ) { ಪ್ರತ್ಯಾಗಮ false; }

#endif /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL) */

#ifdef CONFIG_MEMBARRIER

/*
 * The scheduler provides memory barriers required by membarrier between:
 * - prior user-space memory accesses and store to rq->membarrier_state,
 * - store to rq->membarrier_state and following user-space memory accesses.
 * In the same way it provides those guarantees around store to rq->curr.
 */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ membarrier_switch_mm(ಸಂರಚನಾ rq *rq,
					ಸಂರಚನಾ mm_struct *prev_mm,
					ಸಂರಚನಾ mm_struct *next_mm)
{
	ಪೂರ್ಣಾಙ್ಕ membarrier_state;

	ಯದಿ (prev_mm == next_mm)
		ಪ್ರತ್ಯಾಗಮ;

	membarrier_state = atomic_read(&next_mm->membarrier_state);
	ಯದಿ (READ_ONCE(rq->membarrier_state) == membarrier_state)
		ಪ್ರತ್ಯಾಗಮ;

	WRITE_ONCE(rq->membarrier_state, membarrier_state);
}

#ಅನ್ಯಥಾ /* !CONFIG_MEMBARRIER: */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ membarrier_switch_mm(ಸಂರಚನಾ rq *rq,
					ಸಂರಚನಾ mm_struct *prev_mm,
					ಸಂರಚನಾ mm_struct *next_mm)
{
}

#endif /* !CONFIG_MEMBARRIER */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool is_per_cpu_kthread(ಸಂರಚನಾ task_struct *p)
{
	ಯದಿ (!(p->flags & PF_KTHREAD))
		ಪ್ರತ್ಯಾಗಮ false;

	ಯದಿ (p->nr_cpus_allowed != 1)
		ಪ್ರತ್ಯಾಗಮ false;

	ಪ್ರತ್ಯಾಗಮ true;
}

ಬಾಹ್ಯ ಶೂನ್ಯ swake_up_all_locked(ಸಂರಚನಾ swait_queue_head *q);
ಬಾಹ್ಯ ಶೂನ್ಯ __prepare_to_swait(ಸಂರಚನಾ swait_queue_head *q, ಸಂರಚನಾ swait_queue *wait);

ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ try_to_wake_up(ಸಂರಚನಾ task_struct *tsk, unsigned ಪೂರ್ಣಾಙ್ಕ state, ಪೂರ್ಣಾಙ್ಕ wake_flags);

#ifdef CONFIG_PREEMPT_DYNAMIC
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ preempt_dynamic_mode;
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ sched_dynamic_mode(ಧ್ರುವ ವರ್ಣ *str);
ಬಾಹ್ಯ ಶೂನ್ಯ sched_dynamic_update(ಪೂರ್ಣಾಙ್ಕ mode);
#endif
ಬಾಹ್ಯ ಧ್ರುವ ವರ್ಣ *preempt_modes[];

#ifdef CONFIG_SCHED_MM_CID

ಸ್ಥಿರ __always_inline bool cid_on_cpu(unsigned ಪೂರ್ಣಾಙ್ಕ cid)
{
	ಪ್ರತ್ಯಾಗಮ cid & MM_CID_ONCPU;
}

ಸ್ಥಿರ __always_inline bool cid_in_transit(unsigned ಪೂರ್ಣಾಙ್ಕ cid)
{
	ಪ್ರತ್ಯಾಗಮ cid & MM_CID_TRANSIT;
}

ಸ್ಥಿರ __always_inline unsigned ಪೂರ್ಣಾಙ್ಕ cpu_cid_to_cid(unsigned ಪೂರ್ಣಾಙ್ಕ cid)
{
	ಪ್ರತ್ಯಾಗಮ cid & ~MM_CID_ONCPU;
}

ಸ್ಥಿರ __always_inline unsigned ಪೂರ್ಣಾಙ್ಕ cid_to_cpu_cid(unsigned ಪೂರ್ಣಾಙ್ಕ cid)
{
	ಪ್ರತ್ಯಾಗಮ cid | MM_CID_ONCPU;
}

ಸ್ಥಿರ __always_inline unsigned ಪೂರ್ಣಾಙ್ಕ cid_to_transit_cid(unsigned ಪೂರ್ಣಾಙ್ಕ cid)
{
	ಪ್ರತ್ಯಾಗಮ cid | MM_CID_TRANSIT;
}

ಸ್ಥಿರ __always_inline unsigned ಪೂರ್ಣಾಙ್ಕ cid_from_transit_cid(unsigned ಪೂರ್ಣಾಙ್ಕ cid)
{
	ಪ್ರತ್ಯಾಗಮ cid & ~MM_CID_TRANSIT;
}

ಸ್ಥಿರ __always_inline bool cid_on_task(unsigned ಪೂರ್ಣಾಙ್ಕ cid)
{
	/* True if none of the MM_CID_ONCPU, MM_CID_TRANSIT, MM_CID_UNSET bits is set */
	ಪ್ರತ್ಯಾಗಮ cid < MM_CID_TRANSIT;
}

ಸ್ಥಿರ __always_inline ಶೂನ್ಯ mm_drop_cid(ಸಂರಚನಾ mm_struct *mm, unsigned ಪೂರ್ಣಾಙ್ಕ cid)
{
	clear_bit(cid, mm_cidmask(mm));
}

ಸ್ಥಿರ __always_inline ಶೂನ್ಯ mm_unset_cid_on_task(ಸಂರಚನಾ task_struct *t)
{
	unsigned ಪೂರ್ಣಾಙ್ಕ cid = t->mm_cid.cid;

	t->mm_cid.cid = MM_CID_UNSET;
	ಯದಿ (cid_on_task(cid))
		mm_drop_cid(t->mm, cid);
}

ಸ್ಥಿರ __always_inline ಶೂನ್ಯ mm_drop_cid_on_cpu(ಸಂರಚನಾ mm_struct *mm, ಸಂರಚನಾ mm_cid_pcpu *pcp)
{
	/* Clear the ONCPU bit, but do not set UNSET in the per CPU storage */
	ಯದಿ (cid_on_cpu(pcp->cid)) {
		pcp->cid = cpu_cid_to_cid(pcp->cid);
		mm_drop_cid(mm, pcp->cid);
	}
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ unsigned ಪೂರ್ಣಾಙ್ಕ __mm_get_cid(ಸಂರಚನಾ mm_struct *mm, unsigned ಪೂರ್ಣಾಙ್ಕ max_cids)
{
	unsigned ಪೂರ್ಣಾಙ್ಕ cid = find_first_zero_bit(mm_cidmask(mm), max_cids);

	ಯದಿ (cid >= max_cids)
		ಪ್ರತ್ಯಾಗಮ MM_CID_UNSET;
	ಯದಿ (test_and_set_bit(cid, mm_cidmask(mm)))
		ಪ್ರತ್ಯಾಗಮ MM_CID_UNSET;
	ಪ್ರತ್ಯಾಗಮ cid;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ unsigned ಪೂರ್ಣಾಙ್ಕ mm_get_cid(ಸಂರಚನಾ mm_struct *mm)
{
	unsigned ಪೂರ್ಣಾಙ್ಕ cid = __mm_get_cid(mm, READ_ONCE(mm->mm_cid.max_cids));

	ಯಾವತ್ (cid == MM_CID_UNSET) {
		cpu_relax();
		cid = __mm_get_cid(mm, ಅಂಕೆ_possible_cpus());
	}
	ಪ್ರತ್ಯಾಗಮ cid;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ unsigned ಪೂರ್ಣಾಙ್ಕ mm_cid_converge(ಸಂರಚನಾ mm_struct *mm, unsigned ಪೂರ್ಣಾಙ್ಕ orig_cid,
					   unsigned ಪೂರ್ಣಾಙ್ಕ max_cids)
{
	unsigned ಪೂರ್ಣಾಙ್ಕ new_cid, cid = cpu_cid_to_cid(orig_cid);

	/* Is it in the optimal CID space? */
	ಯದಿ (likely(cid < max_cids))
		ಪ್ರತ್ಯಾಗಮ orig_cid;

	/* Try to find one in the optimal space. Otherwise keep the provided. */
	new_cid = __mm_get_cid(mm, max_cids);
	ಯದಿ (new_cid != MM_CID_UNSET) {
		mm_drop_cid(mm, cid);
		/* Preserve the ONCPU mode of the original CID */
		ಪ್ರತ್ಯಾಗಮ new_cid | (orig_cid & MM_CID_ONCPU);
	}
	ಪ್ರತ್ಯಾಗಮ orig_cid;
}

ಸ್ಥಿರ __always_inline ಶೂನ್ಯ mm_cid_update_task_cid(ಸಂರಚನಾ task_struct *t, unsigned ಪೂರ್ಣಾಙ್ಕ cid)
{
	ಯದಿ (t->mm_cid.cid != cid) {
		t->mm_cid.cid = cid;
		rseq_sched_set_ids_changed(t);
	}
}

ಸ್ಥಿರ __always_inline ಶೂನ್ಯ mm_cid_update_pcpu_cid(ಸಂರಚನಾ mm_struct *mm, unsigned ಪೂರ್ಣಾಙ್ಕ cid)
{
	__this_cpu_write(mm->mm_cid.pcpu->cid, cid);
}

ಸ್ಥಿರ __always_inline ಶೂನ್ಯ mm_cid_from_cpu(ಸಂರಚನಾ task_struct *t, unsigned ಪೂರ್ಣಾಙ್ಕ cpu_cid,
					    unsigned ಪೂರ್ಣಾಙ್ಕ mode)
{
	unsigned ಪೂರ್ಣಾಙ್ಕ max_cids, tcid = t->mm_cid.cid;
	ಸಂರಚನಾ mm_struct *mm = t->mm;

	max_cids = READ_ONCE(mm->mm_cid.max_cids);
	/* Optimize for the common case where both have the ONCPU bit set */
	ಯದಿ (likely(cid_on_cpu(cpu_cid & tcid))) {
		ಯದಿ (likely(cpu_cid_to_cid(cpu_cid) < max_cids)) {
			mm_cid_update_task_cid(t, cpu_cid);
			ಪ್ರತ್ಯಾಗಮ;
		}
		/* Try to converge into the optimal CID space */
		cpu_cid = mm_cid_converge(mm, cpu_cid, max_cids);
	} ಅನ್ಯಥಾ {
		/* Hand over or drop the task owned CID */
		ಯದಿ (cid_on_task(tcid)) {
			ಯದಿ (cid_on_cpu(cpu_cid))
				mm_unset_cid_on_task(t);
			ಅನ್ಯಥಾ
				cpu_cid = cid_to_cpu_cid(tcid);
		}
		/* Still nothing, allocate a new one */
		ಯದಿ (!cid_on_cpu(cpu_cid))
			cpu_cid = cid_to_cpu_cid(mm_get_cid(mm));

		/* Handle the transition mode flag if required */
		ಯದಿ (mode & MM_CID_TRANSIT)
			cpu_cid = cpu_cid_to_cid(cpu_cid) | MM_CID_TRANSIT;
	}
	mm_cid_update_pcpu_cid(mm, cpu_cid);
	mm_cid_update_task_cid(t, cpu_cid);
}

ಸ್ಥಿರ __always_inline ಶೂನ್ಯ mm_cid_from_task(ಸಂರಚನಾ task_struct *t, unsigned ಪೂರ್ಣಾಙ್ಕ cpu_cid,
					     unsigned ಪೂರ್ಣಾಙ್ಕ mode)
{
	unsigned ಪೂರ್ಣಾಙ್ಕ max_cids, tcid = t->mm_cid.cid;
	ಸಂರಚನಾ mm_struct *mm = t->mm;

	max_cids = READ_ONCE(mm->mm_cid.max_cids);
	/* Optimize for the common case, where both have the ONCPU bit clear */
	ಯದಿ (likely(cid_on_task(tcid | cpu_cid))) {
		ಯದಿ (likely(tcid < max_cids)) {
			mm_cid_update_pcpu_cid(mm, tcid);
			ಪ್ರತ್ಯಾಗಮ;
		}
		/* Try to converge into the optimal CID space */
		tcid = mm_cid_converge(mm, tcid, max_cids);
	} ಅನ್ಯಥಾ {
		/* Hand over or drop the CPU owned CID */
		ಯದಿ (cid_on_cpu(cpu_cid)) {
			ಯದಿ (cid_on_task(tcid))
				mm_drop_cid_on_cpu(mm, this_cpu_ptr(mm->mm_cid.pcpu));
			ಅನ್ಯಥಾ
				tcid = cpu_cid_to_cid(cpu_cid);
		}
		/* Still nothing, allocate a new one */
		ಯದಿ (!cid_on_task(tcid))
			tcid = mm_get_cid(mm);
		/* Set the transition mode flag if required */
		tcid |= mode & MM_CID_TRANSIT;
	}
	mm_cid_update_pcpu_cid(mm, tcid);
	mm_cid_update_task_cid(t, tcid);
}

ಸ್ಥಿರ __always_inline ಶೂನ್ಯ mm_cid_schedin(ಸಂರಚನಾ task_struct *next)
{
	ಸಂರಚನಾ mm_struct *mm = next->mm;
	unsigned ಪೂರ್ಣಾಙ್ಕ cpu_cid, mode;

	ಯದಿ (!next->mm_cid.active)
		ಪ್ರತ್ಯಾಗಮ;

	cpu_cid = __this_cpu_read(mm->mm_cid.pcpu->cid);
	mode = READ_ONCE(mm->mm_cid.mode);
	ಯದಿ (likely(!cid_on_cpu(mode)))
		mm_cid_from_task(next, cpu_cid, mode);
	ಅನ್ಯಥಾ
		mm_cid_from_cpu(next, cpu_cid, mode);
}

ಸ್ಥಿರ __always_inline ಶೂನ್ಯ mm_cid_schedout(ಸಂರಚನಾ task_struct *prev)
{
	ಸಂರಚನಾ mm_struct *mm = prev->mm;
	unsigned ಪೂರ್ಣಾಙ್ಕ mode, cid;

	/* During mode transitions CIDs are temporary and need to be dropped */
	ಯದಿ (likely(!cid_in_transit(prev->mm_cid.cid)))
		ಪ್ರತ್ಯಾಗಮ;

	mode = READ_ONCE(mm->mm_cid.mode);
	cid = cid_from_transit_cid(prev->mm_cid.cid);

	/*
	 * If transition mode is done, transfer ownership when the CID is
	 * within the convergence range to optimize the next schedule in.
	 */
	ಯದಿ (!cid_in_transit(mode) && cid < READ_ONCE(mm->mm_cid.max_cids)) {
		ಯದಿ (cid_on_cpu(mode))
			cid = cid_to_cpu_cid(cid);

		/* Update both so that the next schedule in goes into the fast path */
		mm_cid_update_pcpu_cid(mm, cid);
		prev->mm_cid.cid = cid;
	} ಅನ್ಯಥಾ {
		mm_drop_cid(mm, cid);
		prev->mm_cid.cid = MM_CID_UNSET;
	}
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ mm_cid_switch_to(ಸಂರಚನಾ task_struct *prev, ಸಂರಚನಾ task_struct *next)
{
	mm_cid_schedout(prev);
	mm_cid_schedin(next);
}

#ಅನ್ಯಥಾ /* !CONFIG_SCHED_MM_CID: */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ mm_cid_switch_to(ಸಂರಚನಾ task_struct *prev, ಸಂರಚನಾ task_struct *next) { }
#endif /* !CONFIG_SCHED_MM_CID */

#ifdef CONFIG_SCHED_CACHE
DECLARE_STATIC_ಕೀಲಿ_FALSE(sched_cache_present);
DECLARE_STATIC_ಕೀಲಿ_FALSE(sched_cache_active);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ sysctl_sched_cache_user;
ಬಾಹ್ಯ unsigned ಪೂರ್ಣಾಙ್ಕ llc_aggr_tolerance;
ಬಾಹ್ಯ unsigned ಪೂರ್ಣಾಙ್ಕ llc_epoch_period;
ಬಾಹ್ಯ unsigned ಪೂರ್ಣಾಙ್ಕ llc_epoch_affinity_timeout;
ಬಾಹ್ಯ unsigned ಪೂರ್ಣಾಙ್ಕ llc_imb_pct;
ಬಾಹ್ಯ unsigned ಪೂರ್ಣಾಙ್ಕ llc_overaggr_pct;

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool sched_cache_enabled(ಶೂನ್ಯ)
{
	ಪ್ರತ್ಯಾಗಮ static_branch_unlikely(&sched_cache_active);
}

ಬಾಹ್ಯ ಶೂನ್ಯ sched_cache_active_set(ಶೂನ್ಯ);

#endif

ಶೂನ್ಯ sched_domains_free_llc_id(ಪೂರ್ಣಾಙ್ಕ cpu);

ಬಾಹ್ಯ ಶೂನ್ಯ init_sched_mm(ಸಂರಚನಾ task_struct *p);

ಬಾಹ್ಯ u64 avg_vruntime(ಸಂರಚನಾ cfs_rq *cfs_rq);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ entity_eligible(ಸಂರಚನಾ cfs_rq *cfs_rq, ಸಂರಚನಾ sched_entity *se);
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ
ಶೂನ್ಯ move_queued_task_locked(ಸಂರಚನಾ rq *src_rq, ಸಂರಚನಾ rq *dst_rq, ಸಂರಚನಾ task_struct *task)
{
	lockdep_assert_rq_held(src_rq);
	lockdep_assert_rq_held(dst_rq);

	deactivate_task(src_rq, task, 0);
	set_task_cpu(task, dst_rq->cpu);
	activate_task(dst_rq, task, 0);
	wakeup_preempt(dst_rq, task, 0);
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ
bool task_is_pushable(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p, ಪೂರ್ಣಾಙ್ಕ cpu)
{
	ಯದಿ (!task_on_cpu(rq, p) &&
	    cpumask_test_cpu(cpu, &p->cpus_mask))
		ಪ್ರತ್ಯಾಗಮ true;

	ಪ್ರತ್ಯಾಗಮ false;
}

#ifdef CONFIG_RT_MUTEXES

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ __rt_effective_prio(ಸಂರಚನಾ task_struct *pi_task, ಪೂರ್ಣಾಙ್ಕ prio)
{
	ಯದಿ (pi_task)
		prio = min(prio, pi_task->prio);

	ಪ್ರತ್ಯಾಗಮ prio;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ rt_effective_prio(ಸಂರಚನಾ task_struct *p, ಪೂರ್ಣಾಙ್ಕ prio)
{
	ಸಂರಚನಾ task_struct *pi_task = rt_mutex_get_ತುದಿ_task(p);

	ಪ್ರತ್ಯಾಗಮ __rt_effective_prio(pi_task, prio);
}

#ಅನ್ಯಥಾ /* !CONFIG_RT_MUTEXES: */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ rt_effective_prio(ಸಂರಚನಾ task_struct *p, ಪೂರ್ಣಾಙ್ಕ prio)
{
	ಪ್ರತ್ಯಾಗಮ prio;
}

#endif /* !CONFIG_RT_MUTEXES */

ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ __sched_setscheduler(ಸಂರಚನಾ task_struct *p, ಧ್ರುವ ಸಂರಚನಾ sched_attr *attr, bool user, bool pi);
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ __sched_setaffinity(ಸಂರಚನಾ task_struct *p, ಸಂರಚನಾ affinity_context *ctx);
ಬಾಹ್ಯ ಧ್ರುವ ಸಂರಚನಾ sched_class *__setscheduler_class(ಪೂರ್ಣಾಙ್ಕ policy, ಪೂರ್ಣಾಙ್ಕ prio);
ಬಾಹ್ಯ ಶೂನ್ಯ set_load_weight(ಸಂರಚನಾ task_struct *p, bool update_load);
ಬಾಹ್ಯ ಶೂನ್ಯ enqueue_task(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p, ಪೂರ್ಣಾಙ್ಕ flags);
ಬಾಹ್ಯ bool dequeue_task(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ task_struct *p, ಪೂರ್ಣಾಙ್ಕ flags);

ಬಾಹ್ಯ ಸಂರಚನಾ balance_callback *splice_balance_callbacks(ಸಂರಚನಾ rq *rq);

ಬಾಹ್ಯ ಶೂನ್ಯ __balance_callbacks(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ rq_flags *rf);
ಬಾಹ್ಯ ಶೂನ್ಯ balance_callbacks(ಸಂರಚನಾ rq *rq, ಸಂರಚನಾ balance_callback *head);

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
ಸಂರಚನಾ sched_change_ctx {
	u64			prio;
	ಸಂರಚನಾ task_struct	*p;
	ಧ್ರುವ ಸಂರಚನಾ sched_class *class;
	ಪೂರ್ಣಾಙ್ಕ			flags;
	bool			queued;
	bool			running;
};

ಸಂರಚನಾ sched_change_ctx *sched_change_begin(ಸಂರಚನಾ task_struct *p, unsigned ಪೂರ್ಣಾಙ್ಕ flags);
ಶೂನ್ಯ sched_change_end(ಸಂರಚನಾ sched_change_ctx *ctx);

DEFINE_CLASS(sched_change, ಸಂರಚನಾ sched_change_ctx *,
	     sched_change_end(_T),
	     sched_change_begin(p, flags),
	     ಸಂರಚನಾ task_struct *p, unsigned ಪೂರ್ಣಾಙ್ಕ flags)

DEFINE_CLASS_IS_UNCONDITIONAL(sched_change)

#ಅನ್ತರ್ಭಾವ "ext/ext.h"

#ifdef CONFIG_PREFERRED_CPU
ಶೂನ್ಯ sched_ತಳ್ಳು_current_non_preferred_cpu(ಸಂರಚನಾ rq *rq);
#ಅನ್ಯಥಾ	/* !CONFIG_PREFERRED_CPU */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ sched_ತಳ್ಳು_current_non_preferred_cpu(ಸಂರಚನಾ rq *rq) { }
#endif

#endif /* _KERNEL_SCHED_SCHED_H */
