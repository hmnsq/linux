/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Scheduler internal types and methods:
 */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _KERNEL_SCHED_SCHED_H
#ವ್ಯಾಖ್ಯಾನಿಸು _KERNEL_SCHED_SCHED_H

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

ರಚನೆ rq;
ರಚನೆ cfs_rq;
ರಚನೆ rt_rq;
ರಚನೆ sched_ಗುಂಪು;
ರಚನೆ cpuidle_state;

#ಆದರೆ defined(CONFIG_PARAVIRT) && !defined(CONFIG_HAVE_PV_STEAL_CLOCK_GEN)
# include <asm/paravirt.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ <asm/barrier.h>

#ಅನ್ತರ್ಭಾವ "cpupri.h"
#ಅನ್ತರ್ಭಾವ "cpudeadline.h"

/* task_struct::on_rq states: */
#ವ್ಯಾಖ್ಯಾನಿಸು TASK_ON_RQ_QUEUED	1
#ವ್ಯಾಖ್ಯಾನಿಸು TASK_ON_RQ_MIGRATING	2

ಬಾಹ್ಯ __read_mostly ಪೂರ್ಣಾಂಕ scheduler_running;

ಬಾಹ್ಯ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ calc_ತುಂಬು_update;
ಬಾಹ್ಯ atomic_long_t calc_ತುಂಬು_tasks;

ಬಾಹ್ಯ ಶೂನ್ಯ calc_global_ತುಂಬು_tick(ರಚನೆ rq *this_rq);
ಬಾಹ್ಯ ದೀರ್ಘ calc_ತುಂಬು_fold_ಸಕ್ರಿಯ(ರಚನೆ rq *this_rq, ದೀರ್ಘ adjust);

ಬಾಹ್ಯ ಶೂನ್ಯ call_trace_sched_update_nr_running(ರಚನೆ rq *rq, ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2);

ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ sysctl_sched_rt_period;
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ sysctl_sched_rt_runtime;
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ sched_rr_timeslice;

/*
 * Asymmetric CPU capacity bits
 */
ರಚನೆ asym_cap_ದತ್ತಾಂಶ {
	ರಚನೆ list_ತಲೆ link;
	ರಚನೆ rcu_ತಲೆ rcu;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಸಾಮರ್ಥ್ಯ;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ cpus[];
};

ಬಾಹ್ಯ ರಚನೆ list_ತಲೆ asym_cap_list;

#ವ್ಯಾಖ್ಯಾನಿಸು cpu_ಸಾಮರ್ಥ್ಯ_ಹರವು(asym_ದತ್ತಾಂಶ) ಗಮ್ಯ_cpumask((asym_ದತ್ತಾಂಶ)->cpus)

/*
 * Helpers for converting nanosecond timing to jiffy resolution
 */
#ವ್ಯಾಖ್ಯಾನಿಸು NS_ಗಮ್ಯ_JIFFIES(ಸಮಯ)	((ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ)(ಸಮಯ) / (NSEC_ಪ್ರತಿ_SEC/HZ))

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
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_64_ಬಿಟ್
# define NICE_0_ತುಂಬು_ಸರಿಸು	(SCHED_FIXEDPOINT_ಸರಿಸು + SCHED_FIXEDPOINT_ಸರಿಸು)
# define scale_ತುಂಬು(w)		((w) << SCHED_FIXEDPOINT_ಸರಿಸು)
# define scale_ತುಂಬು_ಕೆಳಕ್ಕೆ(w)					\
({								\
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ __w = (w);				\
								\
	ಆದರೆ (__w)						\
		__w = max(2UL, __w >> SCHED_FIXEDPOINT_ಸರಿಸು);	\
	__w;							\
})
#ಇಲ್ಲದಿದ್ದರೆ
# define NICE_0_ತುಂಬು_ಸರಿಸು	(SCHED_FIXEDPOINT_ಸರಿಸು)
# define scale_ತುಂಬು(w)		(w)
# define scale_ತುಂಬು_ಕೆಳಕ್ಕೆ(w)	(w)
#ಆದರೆ_ಅಂತ್ಯ

/*
 * Task weight (visible to users) and its load (invisible to users) have
 * independent resolution, but they should be well calibrated. We use
 * scale_load() and scale_load_down(w) to convert between them. The
 * following must be true:
 *
 *  scale_load(sched_prio_to_weight[NICE_TO_PRIO(0)-MAX_RT_PRIO]) == NICE_0_LOAD
 *
 */
#ವ್ಯಾಖ್ಯಾನಿಸು NICE_0_ತುಂಬು		(1L << NICE_0_ತುಂಬು_ಸರಿಸು)

/*
 * Single value that decides SCHED_DEADLINE internal math precision.
 * 10 -> just above 1us
 * 9  -> just above 0.5us
 */
#ವ್ಯಾಖ್ಯಾನಿಸು DL_SCALE		10

/*
 * Single value that denotes runtime == period, ie unlimited time.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು RUNTIME_INF		((u64)~0ULL)

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ idle_policy(ಪೂರ್ಣಾಂಕ policy)
{
	ಹಿಂತಿರುಗಿ policy == SCHED_IDLE;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ ಸಾಮಾನ್ಯ_policy(ಪೂರ್ಣಾಂಕ policy)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CLASS_EXT
	ಆದರೆ (policy == SCHED_EXT)
		ಹಿಂತಿರುಗಿ true;
#ಆದರೆ_ಅಂತ್ಯ
	ಹಿಂತಿರುಗಿ policy == SCHED_ಸಾಮಾನ್ಯ;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ fair_policy(ಪೂರ್ಣಾಂಕ policy)
{
	ಹಿಂತಿರುಗಿ ಸಾಮಾನ್ಯ_policy(policy) || policy == SCHED_BATCH;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ rt_policy(ಪೂರ್ಣಾಂಕ policy)
{
	ಹಿಂತಿರುಗಿ policy == SCHED_FIFO || policy == SCHED_RR;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ dl_policy(ಪೂರ್ಣಾಂಕ policy)
{
	ಹಿಂತಿರುಗಿ policy == SCHED_DEADLINE;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool valid_policy(ಪೂರ್ಣಾಂಕ policy)
{
	ಹಿಂತಿರುಗಿ idle_policy(policy) || fair_policy(policy) ||
		rt_policy(policy) || dl_policy(policy);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ task_has_idle_policy(ರಚನೆ task_struct *p)
{
	ಹಿಂತಿರುಗಿ idle_policy(p->policy);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ task_has_rt_policy(ರಚನೆ task_struct *p)
{
	ಹಿಂತಿರುಗಿ rt_policy(p->policy);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ task_has_dl_policy(ರಚನೆ task_struct *p)
{
	ಹಿಂತಿರುಗಿ dl_policy(p->policy);
}

#ವ್ಯಾಖ್ಯಾನಿಸು cap_scale(v, s)		((v)*(s) >> SCHED_ಸಾಮರ್ಥ್ಯ_ಸರಿಸು)

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ update_avg(u64 *avg, u64 sample)
{
	s64 diff = sample - *avg;

	*avg += diff / 8;
}

/*
 * Shifting a value by an exponent greater *or equal* to the size of said value
 * is UB; cap at size-1.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು shr_bound(val, ಸರಿಸು)							\
	(val >> min_t(typeof(ಸರಿಸು), ಸರಿಸು, ಬಿಟ್ಟುಗಳು_ಪ್ರತಿ_TYPE(typeof(val)) - 1))

/*
 * cgroup weight knobs should use the common MIN, DFL and MAX values which are
 * 1, 100 and 10000 respectively. While it loses a bit of range on both ends, it
 * maps pretty well onto the shares value used by scheduler and the round-trip
 * conversions preserve the original value over the entire range.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ sched_ತೂಕ_from_cgroup(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ cgrp_ತೂಕ)
{
	ಹಿಂತಿರುಗಿ DIV_ಸುತ್ತು_CLOSEST_ULL(cgrp_ತೂಕ * 1024, CGROUP_ತೂಕ_DFL);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ sched_ತೂಕ_ಗಮ್ಯ_cgroup(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ತೂಕ)
{
	ಹಿಂತಿರುಗಿ clamp_t(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ,
		       DIV_ಸುತ್ತು_CLOSEST_ULL(ತೂಕ * CGROUP_ತೂಕ_DFL, 1024),
		       CGROUP_ತೂಕ_MIN, CGROUP_ತೂಕ_MAX);
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
#ವ್ಯಾಖ್ಯಾನಿಸು SCHED_FLAG_SUGOV	0x10000000

#ವ್ಯಾಖ್ಯಾನಿಸು SCHED_DL_FLAGS		(SCHED_FLAG_RECLAIM | SCHED_FLAG_DL_OVERRUN | SCHED_FLAG_SUGOV)

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool dl_entity_is_special(ಸ್ಥಿರಾಂಕ ರಚನೆ sched_dl_entity *dl_se)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_CPU_FREQ_GOV_SCHEDUTIL
	ಹಿಂತಿರುಗಿ unlikely(dl_se->flags & SCHED_FLAG_SUGOV);
#ಇಲ್ಲದಿದ್ದರೆ
	ಹಿಂತಿರುಗಿ false;
#ಆದರೆ_ಅಂತ್ಯ
}

/*
 * Tells if entity @a should preempt entity @b.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool dl_entity_preempt(ಸ್ಥಿರಾಂಕ ರಚನೆ sched_dl_entity *a,
				     ಸ್ಥಿರಾಂಕ ರಚನೆ sched_dl_entity *b)
{
	ಹಿಂತಿರುಗಿ dl_entity_is_special(a) ||
	       dl_ಸಮಯ_before(a->deadline, b->deadline);
}

/*
 * This is the priority-queue data structure of the RT scheduling class:
 */
ರಚನೆ rt_prio_array {
	DECLARE_BITMAP(bitmap, MAX_RT_PRIO+1); /* include 1 bit for delimiter */
	ರಚನೆ list_ತಲೆ ಸರತಿ[MAX_RT_PRIO];
};

ರಚನೆ rt_bandwidth {
	/* nests inside the rq lock: */
	raw_spinlock_t		rt_runtime_ಬೀಗ;
	ktime_t			rt_period;
	u64			rt_runtime;
	ರಚನೆ hrtimer		rt_period_timer;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		rt_period_ಸಕ್ರಿಯ;
};

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ dl_bandwidth_enabled(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ sysctl_sched_rt_runtime >= 0;
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
ರಚನೆ dl_bw {
	raw_spinlock_t		ಬೀಗ;
	u64			bw;
	u64			ಒಟ್ಟು_bw;
};

ಬಾಹ್ಯ ಶೂನ್ಯ init_dl_bw(ರಚನೆ dl_bw *dl_b);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ  sched_dl_global_validate(ಶೂನ್ಯ);
ಬಾಹ್ಯ ಶೂನ್ಯ sched_dl_do_global(ಶೂನ್ಯ);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ  sched_dl_ಅತಿಪ್ರವಾಹ(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ policy, ಸ್ಥಿರಾಂಕ ರಚನೆ sched_attr *attr);
ಬಾಹ್ಯ ಶೂನ್ಯ __setparam_dl(ರಚನೆ task_struct *p, ಸ್ಥಿರಾಂಕ ರಚನೆ sched_attr *attr);
ಬಾಹ್ಯ ಶೂನ್ಯ __getparam_dl(ರಚನೆ task_struct *p, ರಚನೆ sched_attr *attr, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags);
ಬಾಹ್ಯ bool __checkparam_dl(ಸ್ಥಿರಾಂಕ ರಚನೆ sched_attr *attr);
ಬಾಹ್ಯ bool dl_param_ಬದಲಾಯಿತು(ರಚನೆ task_struct *p, ಸ್ಥಿರಾಂಕ ರಚನೆ sched_attr *attr);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ  dl_cpuset_cpumask_can_shrink(ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *cur, ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *trial);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ  dl_bw_deactivate(ಪೂರ್ಣಾಂಕ cpu);
ಬಾಹ್ಯ s64 dl_scaled_delta_exec(ರಚನೆ rq *rq, ರಚನೆ sched_dl_entity *dl_se, s64 delta_exec);
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
ಬಾಹ್ಯ ಶೂನ್ಯ dl_server_update_idle(ರಚನೆ sched_dl_entity *dl_se, s64 delta_exec);
ಬಾಹ್ಯ ಶೂನ್ಯ dl_server_update(ರಚನೆ sched_dl_entity *dl_se, s64 delta_exec);
ಬಾಹ್ಯ ಶೂನ್ಯ dl_server_ಆರಂಭ(ರಚನೆ sched_dl_entity *dl_se);
ಬಾಹ್ಯ ಶೂನ್ಯ dl_server_stop(ರಚನೆ sched_dl_entity *dl_se);
ಬಾಹ್ಯ ಶೂನ್ಯ dl_server_init(ರಚನೆ sched_dl_entity *dl_se, ರಚನೆ rq *rq,
		    dl_server_pick_f pick_task);
ಬಾಹ್ಯ ಶೂನ್ಯ sched_init_dl_servers(ಶೂನ್ಯ);

ಬಾಹ್ಯ ಶೂನ್ಯ fair_server_init(ರಚನೆ rq *rq);
ಬಾಹ್ಯ ಶೂನ್ಯ ext_server_init(ರಚನೆ rq *rq);
ಬಾಹ್ಯ ಶೂನ್ಯ __dl_server_attach_root(ರಚನೆ sched_dl_entity *dl_se, ರಚನೆ rq *rq);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ dl_server_apply_params(ರಚನೆ sched_dl_entity *dl_se,
		    u64 runtime, u64 period, bool init);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ dl_server_attach_bw(ರಚನೆ sched_dl_entity *dl_se);
ಬಾಹ್ಯ ಶೂನ್ಯ dl_server_detach_bw(ರಚನೆ sched_dl_entity *dl_se);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ dl_server_ಅದಲುಬದಲು_bw(ರಚನೆ sched_dl_entity *detach_se,
			     ರಚನೆ sched_dl_entity *attach_se);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool dl_server_ಸಕ್ರಿಯ(ರಚನೆ sched_dl_entity *dl_se)
{
	ಹಿಂತಿರುಗಿ dl_se->dl_server_ಸಕ್ರಿಯ;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_CGROUP_SCHED

ಬಾಹ್ಯ ರಚನೆ list_ತಲೆ task_groups;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_ಗುಂಪು_SCHED_BANDWIDTH
ಬಾಹ್ಯ ಸ್ಥಿರಾಂಕ u64 max_bw_quota_period_us;

/*
 * default period for group bandwidth.
 * default: 0.1s, units: microseconds
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ u64 default_bw_period_us(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ 100000ULL;
}
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_GROUP_SCHED_BANDWIDTH */

ರಚನೆ cfs_bandwidth {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_CFS_BANDWIDTH
	raw_spinlock_t		ಬೀಗ;
	ktime_t			period;
	u64			quota;
	u64			runtime;
	u64			ಅವಧಿ;
	u64			runtime_snap;
	s64			ಶ್ರೇಣೀಕೃತ_quota;

	u8			idle;
	u8			period_ಸಕ್ರಿಯ;
	u8			slack_started;
	ರಚನೆ hrtimer		period_timer;
	ರಚನೆ hrtimer		slack_timer;
	ರಚನೆ list_ತಲೆ	throttled_cfs_rq;

	/* Statistics: */
	ಪೂರ್ಣಾಂಕ			nr_periods;
	ಪೂರ್ಣಾಂಕ			nr_throttled;
	ಪೂರ್ಣಾಂಕ			nr_ಅವಧಿ;
	u64			throttled_ಸಮಯ;
	u64			ಅವಧಿ_ಸಮಯ;
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_CFS_BANDWIDTH */
};

/* Task group related information */
ರಚನೆ task_ಗುಂಪು {
	ರಚನೆ cgroup_subsys_state css;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_ಗುಂಪು_SCHED_ತೂಕ
	/* A positive value indicates that this is a SCHED_IDLE group. */
	ಪೂರ್ಣಾಂಕ			idle;
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_FAIR_ಗುಂಪು_SCHED
	/* runqueue "owned" by this group on each CPU */
	ರಚನೆ cfs_rq __percpu	*cfs_rq;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ		shares;
	/*
	 * load_avg can be heavily contended at clock tick time, so put
	 * it in its own cache-line separated from the fields above which
	 * will also be accessed at each tick.
	 */
	atomic_long_t		ತುಂಬು_avg ____cacheline_aligned;
	atomic_long_t		runnable_avg;

#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FAIR_GROUP_SCHED */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_RT_ಗುಂಪು_SCHED
	ರಚನೆ sched_rt_entity	**rt_se;
	ರಚನೆ rt_rq		**rt_rq;

	ರಚನೆ rt_bandwidth	rt_bandwidth;
#ಆದರೆ_ಅಂತ್ಯ

	ರಚನೆ scx_task_ಗುಂಪು	scx;

	ರಚನೆ rcu_ತಲೆ		rcu;
	ರಚನೆ list_ತಲೆ	list;

	ರಚನೆ task_ಗುಂಪು	*ಪೋಷಕ;
	ರಚನೆ list_ತಲೆ	siblings;
	ರಚನೆ list_ತಲೆ	children;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_AUTOGROUP
	ರಚನೆ autogroup	*autogroup;
#ಆದರೆ_ಅಂತ್ಯ

	ರಚನೆ cfs_bandwidth	cfs_bandwidth;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_UCLAMP_TASK_ಗುಂಪು
	/* The two decimal precision [%] value requested from user-space */
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		uclamp_pct[UCLAMP_CNT];
	/* Clamp values requested for a task group */
	ರಚನೆ uclamp_se	uclamp_req[UCLAMP_CNT];
	/* Effective clamp values used for a task group */
	ರಚನೆ uclamp_se	uclamp[UCLAMP_CNT];
#ಆದರೆ_ಅಂತ್ಯ

};

#ವ್ಯಾಖ್ಯಾನಿಸು ಬೇರು_TASK_ಗುಂಪು_ತುಂಬು	NICE_0_ತುಂಬು

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಪೂರ್ಣಾಂಕ (*tg_visitor)(ರಚನೆ task_ಗುಂಪು *, ಶೂನ್ಯ *);

ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ walk_tg_ವೃಕ್ಷ_from(ರಚನೆ task_ಗುಂಪು *from,
			     tg_visitor ಕೆಳಕ್ಕೆ, tg_visitor ಮೇಲೆ, ಶೂನ್ಯ *ದತ್ತಾಂಶ);

/*
 * Iterate the full tree, calling @down when first entering a node and @up when
 * leaving it for the final time.
 *
 * Caller must hold rcu_lock or sufficient equivalent.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ walk_tg_ವೃಕ್ಷ(tg_visitor ಕೆಳಕ್ಕೆ, tg_visitor ಮೇಲೆ, ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	ಹಿಂತಿರುಗಿ walk_tg_ವೃಕ್ಷ_from(&ಬೇರು_task_ಗುಂಪು, ಕೆಳಕ್ಕೆ, ಮೇಲೆ, ದತ್ತಾಂಶ);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ task_ಗುಂಪು *css_tg(ರಚನೆ cgroup_subsys_state *css)
{
	ಹಿಂತಿರುಗಿ css ? container_ದ(css, ರಚನೆ task_ಗುಂಪು, css) : ಖಾಲಿ;
}

ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ tg_nop(ರಚನೆ task_ಗುಂಪು *tg, ಶೂನ್ಯ *ದತ್ತಾಂಶ);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_FAIR_ಗುಂಪು_SCHED
ಬಾಹ್ಯ ಶೂನ್ಯ free_fair_sched_ಗುಂಪು(ರಚನೆ task_ಗುಂಪು *tg);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ alloc_fair_sched_ಗುಂಪು(ರಚನೆ task_ಗುಂಪು *tg, ರಚನೆ task_ಗುಂಪು *ಪೋಷಕ);
ಬಾಹ್ಯ ಶೂನ್ಯ online_fair_sched_ಗುಂಪು(ರಚನೆ task_ಗುಂಪು *tg);
ಬಾಹ್ಯ ಶೂನ್ಯ unregister_fair_sched_ಗುಂಪು(ರಚನೆ task_ಗುಂಪು *tg);
ಬಾಹ್ಯ ಶೂನ್ಯ __sched_cgroup_mode_update(ಪೂರ್ಣಾಂಕ ಬಹುಲಕ);
#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_FAIR_GROUP_SCHED: */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ free_fair_sched_ಗುಂಪು(ರಚನೆ task_ಗುಂಪು *tg) { }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ alloc_fair_sched_ಗುಂಪು(ರಚನೆ task_ಗುಂಪು *tg, ರಚನೆ task_ಗುಂಪು *ಪೋಷಕ)
{
       ಹಿಂತಿರುಗಿ 1;
}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ online_fair_sched_ಗುಂಪು(ರಚನೆ task_ಗುಂಪು *tg) { }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ unregister_fair_sched_ಗುಂಪು(ರಚನೆ task_ಗುಂಪು *tg) { }
#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_FAIR_GROUP_SCHED */

ಬಾಹ್ಯ ಶೂನ್ಯ init_tg_cfs_ನಮೂದು(ರಚನೆ task_ಗುಂಪು *tg, ರಚನೆ cfs_rq *cfs_rq,
			ರಚನೆ sched_entity *se, ಪೂರ್ಣಾಂಕ cpu,
			ರಚನೆ sched_entity *ಪೋಷಕ);
ಬಾಹ್ಯ ಶೂನ್ಯ init_cfs_bandwidth(ರಚನೆ cfs_bandwidth *cfs_b, ರಚನೆ cfs_bandwidth *ಪೋಷಕ);

ಬಾಹ್ಯ ಶೂನ್ಯ __refill_cfs_bandwidth_runtime(ರಚನೆ cfs_bandwidth *cfs_b);
ಬಾಹ್ಯ ಶೂನ್ಯ ಆರಂಭ_cfs_bandwidth(ರಚನೆ cfs_bandwidth *cfs_b);
ಬಾಹ್ಯ ಶೂನ್ಯ unthrottle_cfs_rq(ರಚನೆ cfs_rq *cfs_rq);
ಬಾಹ್ಯ bool cfs_task_bw_constrained(ರಚನೆ task_struct *p);

ಬಾಹ್ಯ ಶೂನ್ಯ init_tg_rt_ನಮೂದು(ರಚನೆ task_ಗುಂಪು *tg, ರಚನೆ rt_rq *rt_rq,
		ರಚನೆ sched_rt_entity *rt_se, ಪೂರ್ಣಾಂಕ cpu,
		ರಚನೆ sched_rt_entity *ಪೋಷಕ);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ sched_ಗುಂಪು_ಗಣ_rt_runtime(ರಚನೆ task_ಗುಂಪು *tg, ದೀರ್ಘ rt_runtime_us);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ sched_ಗುಂಪು_ಗಣ_rt_period(ರಚನೆ task_ಗುಂಪು *tg, u64 rt_period_us);
ಬಾಹ್ಯ ದೀರ್ಘ sched_ಗುಂಪು_rt_runtime(ರಚನೆ task_ಗುಂಪು *tg);
ಬಾಹ್ಯ ದೀರ್ಘ sched_ಗುಂಪು_rt_period(ರಚನೆ task_ಗುಂಪು *tg);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ sched_rt_can_attach(ರಚನೆ task_ಗುಂಪು *tg, ರಚನೆ task_struct *tsk);

ಬಾಹ್ಯ ರಚನೆ task_ಗುಂಪು *sched_create_ಗುಂಪು(ರಚನೆ task_ಗುಂಪು *ಪೋಷಕ);
ಬಾಹ್ಯ ಶೂನ್ಯ sched_online_ಗುಂಪು(ರಚನೆ task_ಗುಂಪು *tg,
			       ರಚನೆ task_ಗುಂಪು *ಪೋಷಕ);
ಬಾಹ್ಯ ಶೂನ್ಯ sched_destroy_ಗುಂಪು(ರಚನೆ task_ಗುಂಪು *tg);
ಬಾಹ್ಯ ಶೂನ್ಯ sched_release_ಗುಂಪು(ರಚನೆ task_ಗುಂಪು *tg);

ಬಾಹ್ಯ ಶೂನ್ಯ sched_move_task(ರಚನೆ task_struct *tsk, bool for_autogroup);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_FAIR_ಗುಂಪು_SCHED
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ sched_ಗುಂಪು_ಗಣ_shares(ರಚನೆ task_ಗುಂಪು *tg, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ shares);

ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ sched_ಗುಂಪು_ಗಣ_idle(ರಚನೆ task_ಗುಂಪು *tg, ದೀರ್ಘ idle);

ಬಾಹ್ಯ ಶೂನ್ಯ ಗಣ_task_rq_fair(ರಚನೆ sched_entity *se,
			     ರಚನೆ cfs_rq *prev, ರಚನೆ cfs_rq *ಮುಂದಿನ);
#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_FAIR_GROUP_SCHED: */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ sched_ಗುಂಪು_ಗಣ_shares(ರಚನೆ task_ಗುಂಪು *tg, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ shares) { ಹಿಂತಿರುಗಿ 0; }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ sched_ಗುಂಪು_ಗಣ_idle(ರಚನೆ task_ಗುಂಪು *tg, ದೀರ್ಘ idle) { ಹಿಂತಿರುಗಿ 0; }
#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_FAIR_GROUP_SCHED */

#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_CGROUP_SCHED: */

ರಚನೆ cfs_bandwidth { };

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool cfs_task_bw_constrained(ರಚನೆ task_struct *p) { ಹಿಂತಿರುಗಿ false; }

#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_CGROUP_SCHED */

/*
 * A weight of 0 or 1 can cause arithmetics problems.
 * A weight of a cfs_rq is the sum of weights of which entities
 * are queued on this cfs_rq, so a weight of a entity should not be
 * too large, so as the shares value of a task group.
 * (The default weight is 1024 - so there's no practical
 *  limitation from this.)
 */
#ವ್ಯಾಖ್ಯಾನಿಸು MIN_SHARES		(1UL <<  1)
#ವ್ಯಾಖ್ಯಾನಿಸು MAX_SHARES		(1UL << 18)

ಬಾಹ್ಯ ಶೂನ್ಯ unregister_rt_sched_ಗುಂಪು(ರಚನೆ task_ಗುಂಪು *tg);
ಬಾಹ್ಯ ಶೂನ್ಯ free_rt_sched_ಗುಂಪು(ರಚನೆ task_ಗುಂಪು *tg);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ alloc_rt_sched_ಗುಂಪು(ರಚನೆ task_ಗುಂಪು *tg, ರಚನೆ task_ಗುಂಪು *ಪೋಷಕ);

/*
 * u64_u32_load/u64_u32_store
 *
 * Use a copy of a u64 value to protect against data race. This is only
 * applicable for 32-bits architectures.
 */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_64_ಬಿಟ್
# define u64_u32_ತುಂಬು_copy(var, copy)		var
# define u64_u32_store_copy(var, copy, val)	(var = val)
#ಇಲ್ಲದಿದ್ದರೆ
# define u64_u32_ತುಂಬು_copy(var, copy)					\
({									\
	u64 __val, __val_copy;						\
	ಮಾಡು {								\
		__val_copy = copy;					\
		/*							\
		 * paired with u64_u32_store_copy(), ordering access	\
		 * to var and copy.					\
		 */							\
		smp_rmb();						\
		__val = var;						\
	} ಆಗ (__val != __val_copy);					\
	__val;								\
})
# define u64_u32_store_copy(var, copy, val)				\
ಮಾಡು {									\
	typeof(val) __val = (val);					\
	var = __val;							\
	/*								\
	 * paired with u64_u32_load_copy(), ordering access to var and	\
	 * copy.							\
	 */								\
	smp_wmb();							\
	copy = __val;							\
} ಆಗ (0)
#ಆದರೆ_ಅಂತ್ಯ
# define u64_u32_ತುಂಬು(var)		u64_u32_ತುಂಬು_copy(var, var##_copy)
# define u64_u32_store(var, val)	u64_u32_store_copy(var, var##_copy, val)

ರಚನೆ balance_callback {
	ರಚನೆ balance_callback *ಮುಂದಿನ;
	ಶೂನ್ಯ (*func)(ರಚನೆ rq *rq);
};

/* Fair scheduling SCHED_{NORMAL,BATCH,IDLE} related fields in a runqueue: */
ರಚನೆ cfs_rq {
	ರಚನೆ ತುಂಬು_ತೂಕ	ತುಂಬು;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		nr_queued;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		h_nr_queued;		/* SCHED_{NORMAL,BATCH,IDLE} */
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		h_nr_runnable;		/* SCHED_{NORMAL,BATCH,IDLE} */
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		h_nr_idle;		/* SCHED_IDLE */

	s64			ಮೊತ್ತ_w_vruntime;
	u64			ಮೊತ್ತ_ತೂಕ;
	u64			zero_vruntime;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		ಮೊತ್ತ_ಸರಿಸು;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CORE
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		forceidle_seq;
	u64			zero_vruntime_fi;
#ಆದರೆ_ಅಂತ್ಯ

	ರಚನೆ rb_ಬೇರು_cached	tasks_timeline;

	/*
	 * 'curr' points to the currently running entity on this cfs_rq.
	 * It is set to NULL otherwise (i.e when none are currently running).
	 */
	ರಚನೆ sched_entity	*curr;
	ರಚನೆ sched_entity	*ಮುಂದಿನ;

	/*
	 * CFS load tracking
	 */
	ರಚನೆ sched_entity	*h_curr;
	ರಚನೆ sched_avg	avg;
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CONFIG_64_ಬಿಟ್
	u64			ಕೊನೆಯ_update_ಸಮಯ_copy;
#ಆದರೆ_ಅಂತ್ಯ
	ರಚನೆ {
		raw_spinlock_t	ಬೀಗ ____cacheline_aligned;
		ಪೂರ್ಣಾಂಕ		nr;
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ	ತುಂಬು_avg;
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ	util_avg;
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ	runnable_avg;
	} removed;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_FAIR_ಗುಂಪು_SCHED
	u64			ಕೊನೆಯ_update_tg_ತುಂಬು_avg;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ		tg_ತುಂಬು_avg_contrib;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ		tg_runnable_avg_contrib;
	ದೀರ್ಘ			propagate;
	ದೀರ್ಘ			prop_runnable_ಮೊತ್ತ;

	/*
	 *   h_load = weight * f(tg)
	 *
	 * Where f(tg) is the recursive weight fraction assigned to
	 * this group.
	 */
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ		h_ತುಂಬು;
	u64			ಕೊನೆಯ_h_ತುಂಬು_update;
	ರಚನೆ sched_entity	*h_ತುಂಬು_ಮುಂದಿನ;

	ರಚನೆ rq		*rq;	/* CPU runqueue to which this cfs_rq is attached */

	/*
	 * leaf cfs_rqs are those that hold tasks (lowest schedulable entity in
	 * a hierarchy). Non-leaf lrqs hold other higher schedulable entities
	 * (like users, containers etc.)
	 *
	 * leaf_cfs_rq_list ties together list of leaf cfs_rq's in a CPU.
	 * This list is used during load balance.
	 */
	ಪೂರ್ಣಾಂಕ			on_list;
	ರಚನೆ list_ತಲೆ	ಎಲೆ_cfs_rq_list;
	ರಚನೆ task_ಗುಂಪು	*tg;	/* Group that "owns" this runqueue */

	/* Locally cached copy of our task_group's idle value */
	ಪೂರ್ಣಾಂಕ			idle;

# ifdef CONFIG_CFS_BANDWIDTH
	ಪೂರ್ಣಾಂಕ			runtime_enabled;
	s64			runtime_ಉಳಿದ;

	u64			throttled_pelt_idle;
#  ifndef CONFIG_64_ಬಿಟ್
	u64                     throttled_pelt_idle_copy;
#  endif
	u64			throttled_clock;
	u64			throttled_clock_pelt;
	u64			throttled_clock_pelt_ಸಮಯ;
	u64			throttled_clock_self;
	u64			throttled_clock_self_ಸಮಯ;
	bool			throttled:1;
	bool			pelt_clock_throttled:1;
	ಪೂರ್ಣಾಂಕ			throttle_ಎಣಿಕೆ;
	ರಚನೆ list_ತಲೆ	throttled_list;
	ರಚನೆ list_ತಲೆ	throttled_csd_list;
	ರಚನೆ list_ತಲೆ        throttled_limbo_list;
# endif /* CONFIG_CFS_BANDWIDTH */
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FAIR_GROUP_SCHED */
};

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CLASS_EXT
/* scx_rq->flags, protected by the rq lock */
ಎಣಿಕೆ scx_rq_flags {
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

	SCX_RQ_ಒಳ_WAKEUP	= 1 << 16,
	SCX_RQ_ಒಳ_DISPATCH	= 1 << 17,
};

/* per-rq rescue execution state, see scx_rescue_timerfn() */
ರಚನೆ scx_rq_rescue {
	ರಚನೆ scx_dispatch_q	dsq;			/* stranded tasks awaiting rescue */
	s64			budget;			/* execution token bucket, ns */
	u64			clock;			/* last budget accrual timestamp */
	ರಚನೆ task_struct	*curr;			/* task being rescued, one at a time */
	s64			ತುಂಡು;			/* curr's admitted slice */
	u64			exec_snap;		/* sum_exec_runtime at admission */
	ರಚನೆ timer_list	timer;			/* paces admission and escalation */
	u64			kill_at;		/* last ejection, init before any */
};

ರಚನೆ scx_rq {
	ರಚನೆ scx_dispatch_q	local_dsq;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_EXT_SUB_SCHED
	ರಚನೆ scx_dispatch_q	reject_dsq;		/* staging for cap-rejected tasks */
	ರಚನೆ scx_rq_rescue	rescue;
#ಆದರೆ_ಅಂತ್ಯ
	ರಚನೆ list_ತಲೆ	runnable_list;		/* runnable tasks on this rq */
	ರಚನೆ list_ತಲೆ	ddsp_deferred_locals;	/* deferred ddsps from enq */
	/* both stashed across the activate_task() in move_remote_task_to_local_dsq() */
	u64			remote_activate_enq_flags;
	ರಚನೆ scx_sched	*remote_activate_sch;
	u32			nr_running;
	u32			cpuperf_ಗುರಿ;		/* [0, SCHED_CAPACITY_SCALE] */
	bool			ಒಳ_select_cpu;
	bool			cpu_released;
	u32			flags;
	u32			nr_immed;		/* ENQ_IMMED tasks on local_dsq */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CORE
	u32			ಬೀಗ_drop_seq;	/* nr dispatch lock releases */
#ಆದರೆ_ಅಂತ್ಯ
	u64			clock;			/* current per-rq clock -- see scx_bpf_now() */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_EXT_SUB_SCHED
	ರಚನೆ llist_ತಲೆ	ecaps_ಗಮ್ಯ_sync;		/* pending ecaps syncs */
	ರಚನೆ task_struct	*sub_dispatch_prev;
#ಆದರೆ_ಅಂತ್ಯ
	cpumask_var_t		cpus_ಗಮ್ಯ_sync;
	bool			kick_sync_pending;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ		kick_sync;

	ರಚನೆ list_ತಲೆ	sched_pcpus_ಗಮ್ಯ_kick;	/* see kick_cpus_irq_workfn() */

	raw_spinlock_t		deferred_reenq_ಬೀಗ;
	ರಚನೆ list_ತಲೆ	deferred_reenq_locals;	/* scheds requesting reenq of local DSQ */
	ರಚನೆ list_ತಲೆ	deferred_reenq_users;	/* user DSQs requesting reenq */
	ರಚನೆ balance_callback	deferred_bal_cb;
	ರಚನೆ balance_callback	kick_sync_bal_cb;
	ರಚನೆ irq_work		deferred_irq_work;
	ರಚನೆ irq_work		kick_cpus_irq_work;
};
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_SCHED_CLASS_EXT */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ rt_bandwidth_enabled(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ sysctl_sched_rt_runtime >= 0;
}

/* RT IPI pull logic requires IRQ_WORK */
#ಆದರೆ defined(CONFIG_IRQ_WORK) && defined(CONFIG_SMP)
# define HAVE_RT_ತಳ್ಳು_IPI
#ಆದರೆ_ಅಂತ್ಯ

/* Real-Time classes' related field in a runqueue: */
ರಚನೆ rt_rq {
	ರಚನೆ rt_prio_array	ಸಕ್ರಿಯ;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		rt_nr_running;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		rr_nr_running;
	ರಚನೆ {
		ಪೂರ್ಣಾಂಕ		curr; /* highest queued rt task prio */
		ಪೂರ್ಣಾಂಕ		ಮುಂದಿನ; /* next highest */
	} highest_prio;
	bool			overloaded;
	ರಚನೆ plist_ತಲೆ	pushable_tasks;

	ಪೂರ್ಣಾಂಕ			rt_queued;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_RT_ಗುಂಪು_SCHED
	ಪೂರ್ಣಾಂಕ			rt_throttled;
	u64			rt_ಸಮಯ; /* consumed RT time, goes up in update_curr_rt */
	u64			rt_runtime; /* allotted RT time, "slice" from rt_bandwidth, RT sharing/balancing */
	/* Nests inside the rq lock: */
	raw_spinlock_t		rt_runtime_ಬೀಗ;

	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		rt_nr_boosted;

	ರಚನೆ rq		*rq; /* this is always top-level rq, cache? */
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_CGROUP_SCHED
	ರಚನೆ task_ಗುಂಪು	*tg; /* this tg has "this" rt_rq on given CPU for runnable entities */
#ಆದರೆ_ಅಂತ್ಯ
};

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool rt_rq_is_runnable(ರಚನೆ rt_rq *rt_rq)
{
	ಹಿಂತಿರುಗಿ rt_rq->rt_queued && rt_rq->rt_nr_running;
}

/* Deadline class' related fields in a runqueue */
ರಚನೆ dl_rq {
	/* runqueue is an rbtree, ordered by deadline */
	ರಚನೆ rb_ಬೇರು_cached	ಬೇರು;

	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		dl_nr_running;

	/*
	 * Deadline values of the currently executing and the
	 * earliest ready task on this rq. Caching these facilitates
	 * the decision whether or not a ready but not running task
	 * should migrate somewhere else.
	 */
	ರಚನೆ {
		u64		curr;
		u64		ಮುಂದಿನ;
	} earliest_dl;

	bool			overloaded;

	ರಚನೆ sched_dl_entity	*curr;
	/*
	 * Tasks on this rq that can be pushed away. They are kept in
	 * an rb-tree, ordered by tasks' deadlines, with caching
	 * of the leftmost (earliest deadline) element.
	 */
	ರಚನೆ rb_ಬೇರು_cached	pushable_dl_tasks_ಬೇರು;

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
	u64			bw_ಅನುಪಾತ;
};

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_FAIR_ಗುಂಪು_SCHED
/* Check whether a task group is root tg */
#ವ್ಯಾಖ್ಯಾನಿಸು is_ಬೇರು_task_ಗುಂಪು(tg) ((tg) == &ಬೇರು_task_ಗುಂಪು)
/* An entity is a task if it doesn't "own" a runqueue */
#ವ್ಯಾಖ್ಯಾನಿಸು entity_is_task(se)	(!se->my_q)

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ se_update_runnable(ರಚನೆ sched_entity *se)
{
	ಆದರೆ (!entity_is_task(se))
		se->runnable_ತೂಕ = se->my_q->h_nr_runnable;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ದೀರ್ಘ se_runnable(ರಚನೆ sched_entity *se)
{
	ಆದರೆ (se->sched_delayed)
		ಹಿಂತಿರುಗಿ false;

	ಆದರೆ (entity_is_task(se))
		ಹಿಂತಿರುಗಿ !!se->on_rq;
	ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ se->runnable_ತೂಕ;
}

#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_FAIR_GROUP_SCHED: */

#ವ್ಯಾಖ್ಯಾನಿಸು entity_is_task(se)	1

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ se_update_runnable(ರಚನೆ sched_entity *se) { }

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ದೀರ್ಘ se_runnable(ರಚನೆ sched_entity *se)
{
	ಆದರೆ (se->sched_delayed)
		ಹಿಂತಿರುಗಿ false;

	ಹಿಂತಿರುಗಿ !!se->on_rq;
}

#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_FAIR_GROUP_SCHED */

/*
 * XXX we want to get rid of these helpers and use the full load resolution.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ದೀರ್ಘ se_ತೂಕ(ರಚನೆ sched_entity *se)
{
	ಹಿಂತಿರುಗಿ scale_ತುಂಬು_ಕೆಳಕ್ಕೆ(se->ತುಂಬು.ತೂಕ);
}


ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool sched_asym_prefer(ಪೂರ್ಣಾಂಕ a, ಪೂರ್ಣಾಂಕ b)
{
	ಹಿಂತಿರುಗಿ arch_asym_cpu_ಆದ್ಯತೆ(a) > arch_asym_cpu_ಆದ್ಯತೆ(b);
}

ರಚನೆ perf_domain {
	ರಚನೆ em_perf_domain *em_pd;
	ರಚನೆ perf_domain *ಮುಂದಿನ;
	ರಚನೆ rcu_ತಲೆ rcu;
};

/*
 * We add the notion of a root-domain which will be used to define per-domain
 * variables. Each exclusive cpuset essentially defines an island domain by
 * fully partitioning the member CPUs from any other cpuset. Whenever a new
 * exclusive cpuset is created, we also create and attach a new root-domain
 * object.
 *
 */
ರಚನೆ ಬೇರು_domain {
	atomic_t		refcount;
	atomic_t		rto_ಎಣಿಕೆ;
	ರಚನೆ rcu_ತಲೆ		rcu;
	cpumask_var_t		ಹರವು;
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
	cpumask_var_t		dlo_ಮುಖವಾಡ;
	atomic_t		dlo_ಎಣಿಕೆ;
	ರಚನೆ dl_bw		dl_bw;
	ರಚನೆ cpudl		cpudl;

	/*
	 * Indicate whether a root_domain's dl_bw has been checked or
	 * updated. It's monotonously increasing value.
	 *
	 * Also, some corner cases, like 'wrap around' is dangerous, but given
	 * that u64 is 'big enough'. So that shouldn't be a concern.
	 */
	u64 ಭೇಟಿ_cookie;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_RT_ತಳ್ಳು_IPI
	/*
	 * For IPI pull requests, loop across the rto_mask.
	 */
	ರಚನೆ irq_work		rto_ತಳ್ಳು_work;
	raw_spinlock_t		rto_ಬೀಗ;
	/* These are only updated and read within rto_lock */
	ಪೂರ್ಣಾಂಕ			rto_loop;
	ಪೂರ್ಣಾಂಕ			rto_cpu;
	/* These atomics are updated outside of a lock */
	atomic_t		rto_loop_ಮುಂದಿನ;
	atomic_t		rto_loop_ಆರಂಭ;
#ಆದರೆ_ಅಂತ್ಯ /* HAVE_RT_PUSH_IPI */
	/*
	 * The "RT overload" flag: it gets set if a CPU has more than
	 * one runnable RT task.
	 */
	cpumask_var_t		rto_ಮುಖವಾಡ;
	ರಚನೆ cpupri		cpupri;

	/*
	 * NULL-terminated list of performance domains intersecting with the
	 * CPUs of the rd. Protected by RCU.
	 */
	ರಚನೆ perf_domain __rcu *pd;
};

ಬಾಹ್ಯ ಶೂನ್ಯ init_defrootdomain(ಶೂನ್ಯ);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ sched_init_domains(ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *cpu_map);
ಬಾಹ್ಯ ಶೂನ್ಯ rq_attach_ಬೇರು(ರಚನೆ rq *rq, ರಚನೆ ಬೇರು_domain *rd);
ಬಾಹ್ಯ ಶೂನ್ಯ sched_get_rd(ರಚನೆ ಬೇರು_domain *rd);
ಬಾಹ್ಯ ಶೂನ್ಯ sched_put_rd(ರಚನೆ ಬೇರು_domain *rd);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ get_rd_overloaded(ರಚನೆ ಬೇರು_domain *rd)
{
	ಹಿಂತಿರುಗಿ READ_ONCE(rd->overloaded);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ ಗಣ_rd_overloaded(ರಚನೆ ಬೇರು_domain *rd, ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
	ಆದರೆ (get_rd_overloaded(rd) != ಸ್ಥಿತಿ_2)
		WRITE_ONCE(rd->overloaded, ಸ್ಥಿತಿ_2);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_RT_ತಳ್ಳು_IPI
ಬಾಹ್ಯ ಶೂನ್ಯ rto_ತಳ್ಳು_irq_work_func(ರಚನೆ irq_work *work);
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_UCLAMP_TASK
/*
 * struct uclamp_bucket - Utilization clamp bucket
 * @value: utilization clamp value for tasks on this clamp bucket
 * @tasks: number of RUNNABLE tasks on this clamp bucket
 *
 * Keep track of how many tasks are RUNNABLE for a given utilization
 * clamp value.
 */
ರಚನೆ uclamp_ಪಾತ್ರೆ {
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಮೌಲ್ಯ : ಬಿಟ್ಟುಗಳು_ಪ್ರತಿ(SCHED_ಸಾಮರ್ಥ್ಯ_SCALE);
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ tasks : ಬಿಟ್ಟುಗಳು_ಪ್ರತಿ_LONG - ಬಿಟ್ಟುಗಳು_ಪ್ರತಿ(SCHED_ಸಾಮರ್ಥ್ಯ_SCALE);
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
ರಚನೆ uclamp_rq {
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಮೌಲ್ಯ;
	ರಚನೆ uclamp_ಪಾತ್ರೆ ಪಾತ್ರೆ[UCLAMP_BUCKETS];
};

DECLARE_STATIC_ಕೀಲಿ_FALSE(sched_uclamp_ಬಳಸಿದ);
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_UCLAMP_TASK */

/*
 * This is the main, per-CPU runqueue data structure.
 *
 * Locking rule: those places that want to lock multiple runqueues
 * (such as the load balancing or the thread migration code), lock
 * acquire operations must be ordered by ascending &runqueue.
 */
ರಚನೆ rq {
	/*
	 * The following members are loaded together, without holding the
	 * rq->lock, in an extremely hot loop in update_sg_lb_stats()
	 * (called from pick_next_task()). To reduce cache pollution from
	 * this operation, they are placed together on this dedicated cache
	 * line. Even though some of them are frequently modified, they are
	 * loaded much more frequently than they are stored.
	 */
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		nr_running;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_NUMA_BALANCING
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		nr_numa_running;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		nr_preferred_running;
#ಆದರೆ_ಅಂತ್ಯ
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		ttwu_pending;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ		cpu_ಸಾಮರ್ಥ್ಯ;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_PROXY_EXEC
	ರಚನೆ task_struct __rcu	*donor;  /* Scheduling context */
	ರಚನೆ task_struct __rcu	*curr;   /* Execution context */
#ಇಲ್ಲದಿದ್ದರೆ
	ಒಕ್ಕೂಟ {
		ರಚನೆ task_struct __rcu *donor; /* Scheduler context */
		ರಚನೆ task_struct __rcu *curr;  /* Execution context */
	};
#ಆದರೆ_ಅಂತ್ಯ
	ರಚನೆ task_struct	*idle;
	/* padding left here deliberately */

	/*
	 * The next cacheline holds the (hot) runqueue lock, as well as
	 * some other less performance-critical fields.
	 */
	u64			nr_switches	____cacheline_aligned;

	/* runqueue lock: */
	raw_spinlock_t		__lock;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_NO_HZ_COMMON
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		nohz_tick_stopped;
	atomic_t		nohz_flags;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		has_blocked_ತುಂಬು;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ		ಕೊನೆಯ_blocked_ತುಂಬು_update_tick;
	call_single_ದತ್ತಾಂಶ_t	nohz_csd;
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_NO_HZ_COMMON */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_UCLAMP_TASK
	/* Utilization clamp values based on CPU's RUNNABLE tasks */
	ರಚನೆ uclamp_rq	uclamp[UCLAMP_CNT] ____cacheline_aligned;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		uclamp_flags;
#ವ್ಯಾಖ್ಯಾನಿಸು UCLAMP_FLAG_IDLE 0x01
#ಆದರೆ_ಅಂತ್ಯ

	ರಚನೆ cfs_rq		cfs;
	ರಚನೆ rt_rq		rt;
	ರಚನೆ dl_rq		dl;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CLASS_EXT
	ರಚನೆ scx_rq		scx;
	ರಚನೆ sched_dl_entity	ext_server;
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CACHE
	raw_spinlock_t		cpu_epoch_ಬೀಗ ____cacheline_aligned;
	u64			cpu_runtime;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ		cpu_epoch;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ		cpu_epoch_ಮುಂದಿನ;
#ಆದರೆ_ಅಂತ್ಯ

	ರಚನೆ sched_dl_entity	fair_server;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_FAIR_ಗುಂಪು_SCHED
	/* list of leaf cfs_rq on this CPU: */
	ರಚನೆ list_ತಲೆ	ಎಲೆ_cfs_rq_list;
	ರಚನೆ list_ತಲೆ	*tmp_alone_branch;
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FAIR_GROUP_SCHED */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_NUMA_BALANCING
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		numa_migrate_on;
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CACHE
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		nr_pref_llc_running;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		nr_llc_running;
#ಆದರೆ_ಅಂತ್ಯ

	/*
	 * This is part of a global counter where only the total sum
	 * over all CPUs matters. A task can increase this counter on
	 * one CPU and if it got migrated afterwards it may decrease
	 * it on another CPU. Always updated under the runqueue lock:
	 */
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ		nr_uninterruptible;

	ರಚನೆ sched_dl_entity	*dl_server;
	ರಚನೆ task_struct	*stop;
	ಸ್ಥಿರಾಂಕ ರಚನೆ sched_class *ಮುಂದಿನ_class;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ		ಮುಂದಿನ_balance;
	ರಚನೆ mm_struct	*prev_mm;

	/*
	 * The following fields of clock data are frequently referenced
	 * and updated together, and should go on their own cache line.
	 */
	u64			clock_task ____cacheline_aligned;
	u64			clock_pelt;
	u64			clock;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ		ಕಳೆದುಹೋದ_idle_ಸಮಯ;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		clock_update_flags;
	u64			clock_pelt_idle;
	u64			clock_idle;

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CONFIG_64_ಬಿಟ್
	u64			clock_pelt_idle_copy;
	u64			clock_idle_copy;
#ಆದರೆ_ಅಂತ್ಯ

	u64 ಕೊನೆಯ_ನೋಡಿದ_need_resched_ns;
	ಪೂರ್ಣಾಂಕ ticks_without_resched;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_MEMBARRIER
	ಪೂರ್ಣಾಂಕ membarrier_state;
#ಆದರೆ_ಅಂತ್ಯ

	ರಚನೆ ಬೇರು_domain		*rd;
	ರಚನೆ sched_domain __rcu	*sd;

	ರಚನೆ balance_callback *balance_callback;

	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ		nohz_idle_balance;
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ		idle_balance;

	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ		misfit_task_ತುಂಬು;

	/* For active balancing */
	ಪೂರ್ಣಾಂಕ			ಸಕ್ರಿಯ_balance;
	ಪೂರ್ಣಾಂಕ			ತಳ್ಳು_cpu;
	ರಚನೆ cpu_stop_work	ಸಕ್ರಿಯ_balance_work;

	/* CPU of this runqueue: */
	ಪೂರ್ಣಾಂಕ			cpu;
	ಪೂರ್ಣಾಂಕ			online;

	ರಚನೆ list_ತಲೆ cfs_tasks;

	ರಚನೆ sched_avg	avg_rt;
	ರಚನೆ sched_avg	avg_dl;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_HAVE_SCHED_AVG_IRQ
	ರಚನೆ sched_avg	avg_irq;
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_HW_PRESSURE
	ರಚನೆ sched_avg	avg_hw;
#ಆದರೆ_ಅಂತ್ಯ
	u64			idle_stamp;
	u64			avg_idle;

	/* This is used to determine avg_idle's max value */
	u64			max_idle_balance_ವೆಚ್ಚ;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_HOTPLUG_CPU
	ರಚನೆ rcuwait		hotplug_wait;
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_IRQ_ಸಮಯ_ACCOUNTING
	u64			prev_irq_ಸಮಯ;
	u64			psi_irq_ಸಮಯ;
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_PARAVIRT
	u64			prev_steal_ಸಮಯ;
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_PARAVIRT_ಸಮಯ_ACCOUNTING
	u64			prev_steal_ಸಮಯ_rq;
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_PREFERRED_CPU
	bool			npc_ತಳ್ಳು_work_pending;
#ಆದರೆ_ಅಂತ್ಯ

	/* calc_load related fields */
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ		calc_ತುಂಬು_update;
	ದೀರ್ಘ			calc_ತುಂಬು_ಸಕ್ರಿಯ;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_HRTICK
	call_single_ದತ್ತಾಂಶ_t	hrtick_csd;
	ರಚನೆ hrtimer		hrtick_timer;
	ktime_t			hrtick_ಸಮಯ;
	ktime_t			hrtick_delay;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		hrtick_sched;
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHEDSTATS
	/* latency stats */
	ರಚನೆ sched_info	rq_sched_info;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ	rq_cpu_ಸಮಯ;

	/* sys_sched_yield() stats */
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		yld_ಎಣಿಕೆ;

	/* schedule() stats */
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		sched_ಎಣಿಕೆ;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		sched_goidle;

	/* try_to_wake_up() stats */
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		ttwu_ಎಣಿಕೆ;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		ttwu_local;
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_CPU_IDLE
	/* Must be inspected within a RCU lock section */
	ರಚನೆ cpuidle_state	*idle_state;
#ಆದರೆ_ಅಂತ್ಯ

	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		nr_pinned;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		ತಳ್ಳು_busy;
	ರಚನೆ cpu_stop_work	ತಳ್ಳು_work;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CORE
	/* per rq */
	ರಚನೆ rq		*core;
	ರಚನೆ task_struct	*core_pick;
	ರಚನೆ sched_dl_entity	*core_dl_server;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		core_enabled;
	ರಚನೆ rb_ಬೇರು		core_ವೃಕ್ಷ;

	/* shared state -- careful with sched_core_cpu_deactivate() */
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		core_task_seq;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		core_pick_seq;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ		core_cookie;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		core_forceidle_ಎಣಿಕೆ;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		core_forceidle_seq;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		core_forceidle_occupation;
	u64			core_forceidle_ಆರಂಭ;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		core_pick_ಒಳ_flight;
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_SCHED_CORE */

	/* Scratch cpumask to be temporarily used under rq_lock */
	cpumask_var_t		scratch_ಮುಖವಾಡ;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_CFS_BANDWIDTH
	call_single_ದತ್ತಾಂಶ_t	cfsb_csd;
	ರಚನೆ list_ತಲೆ	cfsb_csd_list;
#ಆದರೆ_ಅಂತ್ಯ

	atomic_t		nr_iowait;
} __no_randomize_layout;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_FAIR_ಗುಂಪು_SCHED

/* CPU runqueue to which this cfs_rq is attached */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ rq *rq_ದ(ರಚನೆ cfs_rq *cfs_rq)
{
	ಹಿಂತಿರುಗಿ cfs_rq->rq;
}

#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_FAIR_GROUP_SCHED: */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ rq *rq_ದ(ರಚನೆ cfs_rq *cfs_rq)
{
	ಹಿಂತಿರುಗಿ container_ದ(cfs_rq, ರಚನೆ rq, cfs);
}
#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_FAIR_GROUP_SCHED */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ cpu_ದ(ರಚನೆ rq *rq)
{
	ಹಿಂತಿರುಗಿ rq->cpu;
}

#ವ್ಯಾಖ್ಯಾನಿಸು MDF_ತಳ್ಳು		0x01

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool is_migration_disabled(ರಚನೆ task_struct *p)
{
	ಹಿಂತಿರುಗಿ p->migration_disabled;
}

DECLARE_ಪ್ರತಿ_CPU_SHARED_ALIGNED(ರಚನೆ rq, runqueues);
DECLARE_ಪ್ರತಿ_CPU(ರಚನೆ rnd_state, sched_rnd_state);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ u32 sched_rng(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ prandom_u32_state(this_cpu_ptr(&sched_rnd_state));
}

ಸ್ಥಿರ __always_inline ರಚನೆ rq *__this_rq(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ this_cpu_ptr(&runqueues);
}

#ವ್ಯಾಖ್ಯಾನಿಸು cpu_rq(cpu)		(&ಪ್ರತಿ_cpu(runqueues, (cpu)))
#ವ್ಯಾಖ್ಯಾನಿಸು this_rq()		__this_rq()
#ವ್ಯಾಖ್ಯಾನಿಸು task_rq(p)		cpu_rq(task_cpu(p))
#ವ್ಯಾಖ್ಯಾನಿಸು cpu_curr(cpu)		(cpu_rq(cpu)->curr)
#ವ್ಯಾಖ್ಯಾನಿಸು raw_rq()		raw_cpu_ptr(&runqueues)

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool idle_rq(ರಚನೆ rq *rq)
{
	ಹಿಂತಿರುಗಿ rq->curr == rq->idle && !rq->nr_running && !rq->ttwu_pending;
}

/**
 * available_idle_cpu - is a given CPU idle for enqueuing work.
 * @cpu: the CPU in question.
 *
 * Return: 1 if the CPU is currently idle. 0 otherwise.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool ಲಭ್ಯ_idle_cpu(ಪೂರ್ಣಾಂಕ cpu)
{
	ಆದರೆ (!idle_rq(cpu_rq(cpu)))
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (vcpu_is_preempted(cpu))
		ಹಿಂತಿರುಗಿ 0;

	ಹಿಂತಿರುಗಿ 1;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_PROXY_EXEC
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ rq_ಗಣ_donor(ರಚನೆ rq *rq, ರಚನೆ task_struct *t)
{
	rcu_ನಿಯೋಜಿಸು_ಸೂಚಕ(rq->donor, t);
}
#ಇಲ್ಲದಿದ್ದರೆ
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ rq_ಗಣ_donor(ರಚನೆ rq *rq, ರಚನೆ task_struct *t)
{
	/* Do nothing */
}
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CORE
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ cpumask *sched_ಗುಂಪು_ಹರವು(ರಚನೆ sched_ಗುಂಪು *sg);

DECLARE_STATIC_ಕೀಲಿ_FALSE(__sched_core_enabled);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool sched_core_enabled(ರಚನೆ rq *rq)
{
	ಹಿಂತಿರುಗಿ static_branch_unlikely(&__sched_core_enabled) && rq->core_enabled;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool sched_core_disabled(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ !static_branch_unlikely(&__sched_core_enabled);
}

/*
 * Be careful with this function; not for general use. The return value isn't
 * stable unless you actually hold a relevant rq->__lock.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ raw_spinlock_t *rq_lockp(ರಚನೆ rq *rq)
{
	ಆದರೆ (sched_core_enabled(rq))
		ಹಿಂತಿರುಗಿ &rq->core->__lock;

	ಹಿಂತಿರುಗಿ &rq->__lock;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ raw_spinlock_t *__rq_lockp(ರಚನೆ rq *rq)
	__returns_ctx_lock(rq_lockp(rq)) /* alias them */
{
	ಆದರೆ (rq->core_enabled)
		ಹಿಂತಿರುಗಿ &rq->core->__lock;

	ಹಿಂತಿರುಗಿ &rq->__lock;
}

ಬಾಹ್ಯ bool
cfs_prio_less(ಸ್ಥಿರಾಂಕ ರಚನೆ task_struct *a, ಸ್ಥಿರಾಂಕ ರಚನೆ task_struct *b, bool fi);

ಬಾಹ್ಯ ಶೂನ್ಯ task_vruntime_update(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, bool ಒಳ_fi);

/*
 * Helpers to check if the CPU's core cookie matches with the task's cookie
 * when core scheduling is enabled.
 * A special case is that the task's cookie always matches with CPU's core
 * cookie if the CPU is in an idle core.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool sched_cpu_cookie_ಹೊಂದಿಕೆ(ರಚನೆ rq *rq, ರಚನೆ task_struct *p)
{
	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	ಆದರೆ (!sched_core_enabled(rq))
		ಹಿಂತಿರುಗಿ true;

	ಹಿಂತಿರುಗಿ rq->core->core_cookie == p->core_cookie;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool sched_core_cookie_ಹೊಂದಿಕೆ(ರಚನೆ rq *rq, ರಚನೆ task_struct *p)
{
	bool idle_core = true;
	ಪೂರ್ಣಾಂಕ cpu;

	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	ಆದರೆ (!sched_core_enabled(rq))
		ಹಿಂತಿರುಗಿ true;

	ಆದರೆ (rq->core->core_cookie == p->core_cookie)
		ಹಿಂತಿರುಗಿ true;

	for_each_cpu(cpu, cpu_smt_ಮುಖವಾಡ(cpu_ದ(rq))) {
		ಆದರೆ (!ಲಭ್ಯ_idle_cpu(cpu)) {
			idle_core = false;
			ನಿಲ್ಲಿಸು;
		}
	}

	/*
	 * A CPU in an idle core is always the best choice for tasks with
	 * cookies.
	 */
	ಹಿಂತಿರುಗಿ idle_core;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool sched_ಗುಂಪು_cookie_ಹೊಂದಿಕೆ(ರಚನೆ rq *rq,
					    ರಚನೆ task_struct *p,
					    ರಚನೆ sched_ಗುಂಪು *ಗುಂಪು)
{
	ಪೂರ್ಣಾಂಕ cpu;

	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	ಆದರೆ (!sched_core_enabled(rq))
		ಹಿಂತಿರುಗಿ true;

	for_each_cpu_and(cpu, sched_ಗುಂಪು_ಹರವು(ಗುಂಪು), p->cpus_ptr) {
		ಆದರೆ (sched_core_cookie_ಹೊಂದಿಕೆ(cpu_rq(cpu), p))
			ಹಿಂತಿರುಗಿ true;
	}
	ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool sched_core_enqueued(ರಚನೆ task_struct *p)
{
	ಹಿಂತಿರುಗಿ !RB_ಖಾಲಿಗಳು_ಗಂಟು(&p->core_ಗಂಟು);
}

ಬಾಹ್ಯ ಶೂನ್ಯ sched_core_ಸರತಿಗೆ_ಸೇರಿಸು(ರಚನೆ rq *rq, ರಚನೆ task_struct *p);
ಬಾಹ್ಯ ಶೂನ್ಯ sched_core_ಸರತಿಯಿಂದ_ತೆಗೆ(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ flags);

ಬಾಹ್ಯ ಶೂನ್ಯ sched_core_get(ಶೂನ್ಯ);
ಬಾಹ್ಯ ಶೂನ್ಯ sched_core_put(ಶೂನ್ಯ);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool task_has_sched_core(ರಚನೆ task_struct *p)
{
	ಆದರೆ (sched_core_disabled())
		ಹಿಂತಿರುಗಿ false;

	ಹಿಂತಿರುಗಿ !!p->core_cookie;
}

#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_SCHED_CORE: */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool sched_core_enabled(ರಚನೆ rq *rq)
{
	ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool sched_core_disabled(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ raw_spinlock_t *rq_lockp(ರಚನೆ rq *rq)
{
	ಹಿಂತಿರುಗಿ &rq->__lock;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ raw_spinlock_t *__rq_lockp(ರಚನೆ rq *rq)
	__returns_ctx_lock(rq_lockp(rq)) /* alias them */
{
	ಹಿಂತಿರುಗಿ &rq->__lock;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool sched_cpu_cookie_ಹೊಂದಿಕೆ(ರಚನೆ rq *rq, ರಚನೆ task_struct *p)
{
	ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool sched_core_cookie_ಹೊಂದಿಕೆ(ರಚನೆ rq *rq, ರಚನೆ task_struct *p)
{
	ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool sched_ಗುಂಪು_cookie_ಹೊಂದಿಕೆ(ರಚನೆ rq *rq,
					    ರಚನೆ task_struct *p,
					    ರಚನೆ sched_ಗುಂಪು *ಗುಂಪು)
{
	ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool task_has_sched_core(ರಚನೆ task_struct *p)
{
	ಹಿಂತಿರುಗಿ false;
}

#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_SCHED_CORE */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_RT_ಗುಂಪು_SCHED
# ifdef CONFIG_RT_ಗುಂಪು_SCHED_DEFAULT_DISABLED
DECLARE_STATIC_ಕೀಲಿ_FALSE(rt_ಗುಂಪು_sched);
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool rt_ಗುಂಪು_sched_enabled(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ static_branch_unlikely(&rt_ಗುಂಪು_sched);
}
# ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_RT_GROUP_SCHED_DEFAULT_DISABLED: */
DECLARE_STATIC_ಕೀಲಿ_TRUE(rt_ಗುಂಪು_sched);
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool rt_ಗುಂಪು_sched_enabled(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ static_branch_likely(&rt_ಗುಂಪು_sched);
}
# endif /* !CONFIG_RT_GROUP_SCHED_DEFAULT_DISABLED */
#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_RT_GROUP_SCHED: */
# define rt_ಗುಂಪು_sched_enabled()	false
#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_RT_GROUP_SCHED */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ lockdep_assert_rq_held(ರಚನೆ rq *rq)
	__assumes_ctx_lock(__rq_lockp(rq))
{
	lockdep_assert_held(__rq_lockp(rq));
}

ಬಾಹ್ಯ ಶೂನ್ಯ raw_spin_rq_ಬೀಗ_nested(ರಚನೆ rq *rq, ಪೂರ್ಣಾಂಕ subclass)
	__acquires(__rq_lockp(rq));

ಬಾಹ್ಯ bool raw_spin_rq_trylock(ರಚನೆ rq *rq)
	__cond_acquires(true, __rq_lockp(rq));

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ raw_spin_rq_ಬೀಗ(ರಚನೆ rq *rq)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_ಬೀಗ_nested(rq, 0);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ raw_spin_rq_unlock(ರಚನೆ rq *rq)
	__releases(__rq_lockp(rq))
{
	raw_spin_unlock(rq_lockp(rq));
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ raw_spin_rq_ಬೀಗ_irq(ರಚನೆ rq *rq)
	__acquires(__rq_lockp(rq))
{
	local_irq_disable();
	raw_spin_rq_ಬೀಗ(rq);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ raw_spin_rq_unlock_irq(ರಚನೆ rq *rq)
	__releases(__rq_lockp(rq))
{
	raw_spin_rq_unlock(rq);
	local_irq_enable();
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ raw_spin_rq_ಬೀಗ_irqsave_2(ರಚನೆ rq *rq)
	__acquires(__rq_lockp(rq))
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags;

	local_irq_save(flags);
	raw_spin_rq_ಬೀಗ(rq);

	ಹಿಂತಿರುಗಿ flags;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ raw_spin_rq_unlock_irqrestore(ರಚನೆ rq *rq, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags)
	__releases(__rq_lockp(rq))
{
	raw_spin_rq_unlock(rq);
	local_irq_restore(flags);
}

#ವ್ಯಾಖ್ಯಾನಿಸು raw_spin_rq_ಬೀಗ_irqsave(rq, flags)	\
ಮಾಡು {						\
	flags = raw_spin_rq_ಬೀಗ_irqsave_2(rq);	\
} ಆಗ (0)

ಬಾಹ್ಯ ಶೂನ್ಯ __update_idle_core(ರಚನೆ rq *rq);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ update_idle_core(ರಚನೆ rq *rq)
{
	ಆದರೆ (sched_smt_ಸಕ್ರಿಯ())
		__update_idle_core(rq);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_FAIR_ಗುಂಪು_SCHED
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ task_struct *task_ದ(ರಚನೆ sched_entity *se)
{
	WARN_ON_ONCE(!entity_is_task(se));
	ಹಿಂತಿರುಗಿ container_ದ(se, ರಚನೆ task_struct, se);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ cfs_rq *task_cfs_rq(ರಚನೆ task_struct *p)
{
	ಹಿಂತಿರುಗಿ p->se.cfs_rq;
}

/* runqueue on which this entity is (to be) queued */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ cfs_rq *cfs_rq_ದ(ಸ್ಥಿರಾಂಕ ರಚನೆ sched_entity *se)
{
	ಹಿಂತಿರುಗಿ se->cfs_rq;
}

/* runqueue "owned" by this group */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ cfs_rq *ಗುಂಪು_cfs_rq(ರಚನೆ sched_entity *grp)
{
	ಹಿಂತಿರುಗಿ grp->my_q;
}

#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_FAIR_GROUP_SCHED: */

#ವ್ಯಾಖ್ಯಾನಿಸು task_ದ(_se)		container_ದ(_se, ರಚನೆ task_struct, se)

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ cfs_rq *task_cfs_rq(ಸ್ಥಿರಾಂಕ ರಚನೆ task_struct *p)
{
	ಹಿಂತಿರುಗಿ &task_rq(p)->cfs;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ cfs_rq *cfs_rq_ದ(ಸ್ಥಿರಾಂಕ ರಚನೆ sched_entity *se)
{
	ಸ್ಥಿರಾಂಕ ರಚನೆ task_struct *p = task_ದ(se);
	ರಚನೆ rq *rq = task_rq(p);

	ಹಿಂತಿರುಗಿ &rq->cfs;
}

/* runqueue "owned" by this group */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ cfs_rq *ಗುಂಪು_cfs_rq(ರಚನೆ sched_entity *grp)
{
	ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_FAIR_GROUP_SCHED */

ಬಾಹ್ಯ ಶೂನ್ಯ update_rq_avg_idle(ರಚನೆ rq *rq);
ಬಾಹ್ಯ ಶೂನ್ಯ update_rq_clock(ರಚನೆ rq *rq);

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
#ವ್ಯಾಖ್ಯಾನಿಸು RQCF_REQ_SKIP		0x01
#ವ್ಯಾಖ್ಯಾನಿಸು RQCF_ACT_SKIP		0x02
#ವ್ಯಾಖ್ಯಾನಿಸು RQCF_UPDATED		0x04

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ assert_clock_updated(ರಚನೆ rq *rq)
{
	/*
	 * The only reason for not seeing a clock update since the
	 * last rq_pin_lock() is if we're currently skipping updates.
	 */
	WARN_ON_ONCE(rq->clock_update_flags < RQCF_ACT_SKIP);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ u64 rq_clock(ರಚನೆ rq *rq)
{
	lockdep_assert_rq_held(rq);
	assert_clock_updated(rq);

	ಹಿಂತಿರುಗಿ rq->clock;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ u64 rq_clock_task(ರಚನೆ rq *rq)
{
	lockdep_assert_rq_held(rq);
	assert_clock_updated(rq);

	ಹಿಂತಿರುಗಿ rq->clock_task;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ rq_clock_skip_update(ರಚನೆ rq *rq)
{
	lockdep_assert_rq_held(rq);
	rq->clock_update_flags |= RQCF_REQ_SKIP;
}

/*
 * See rt task throttling, which is the only time a skip
 * request is canceled.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ rq_clock_cancel_skipupdate(ರಚನೆ rq *rq)
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
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ rq_clock_ಆರಂಭ_loop_update(ರಚನೆ rq *rq)
{
	lockdep_assert_rq_held(rq);
	WARN_ON_ONCE(rq->clock_update_flags & RQCF_ACT_SKIP);
	rq->clock_update_flags |= RQCF_ACT_SKIP;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ rq_clock_stop_loop_update(ರಚನೆ rq *rq)
{
	lockdep_assert_rq_held(rq);
	rq->clock_update_flags &= ~RQCF_ACT_SKIP;
}

ರಚನೆ rq_flags {
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags;
	ರಚನೆ pin_cookie cookie;
	/*
	 * A copy of (rq::clock_update_flags & RQCF_UPDATED) for the
	 * current pin context is stashed here in case it needs to be
	 * restored in rq_repin_lock().
	 */
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ clock_update_flags;
};

ಬಾಹ್ಯ ರಚನೆ balance_callback balance_ತಳ್ಳು_callback;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CLASS_EXT
ಬಾಹ್ಯ ಸ್ಥಿರಾಂಕ ರಚನೆ sched_class ext_sched_class;

DECLARE_STATIC_ಕೀಲಿ_FALSE(__scx_enabled);	/* SCX BPF scheduler loaded */
DECLARE_STATIC_ಕೀಲಿ_FALSE(__scx_switched_all);	/* all fair class tasks on SCX */

#ವ್ಯಾಖ್ಯಾನಿಸು scx_enabled()		static_branch_unlikely(&__scx_enabled)
#ವ್ಯಾಖ್ಯಾನಿಸು scx_switched_all()	static_branch_unlikely(&__scx_switched_all)

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ scx_rq_clock_update(ರಚನೆ rq *rq, u64 clock)
{
	ಆದರೆ (!scx_enabled())
		ಹಿಂತಿರುಗಿ;
	WRITE_ONCE(rq->scx.clock, clock);
	smp_store_release(&rq->scx.flags, rq->scx.flags | SCX_RQ_CLK_VALID);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ scx_rq_clock_invalidate(ರಚನೆ rq *rq)
{
	ಆದರೆ (!scx_enabled())
		ಹಿಂತಿರುಗಿ;
	WRITE_ONCE(rq->scx.flags, rq->scx.flags & ~SCX_RQ_CLK_VALID);
}

#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_SCHED_CLASS_EXT: */
#ವ್ಯಾಖ್ಯಾನಿಸು scx_enabled()		false
#ವ್ಯಾಖ್ಯಾನಿಸು scx_switched_all()	false

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ scx_rq_clock_update(ರಚನೆ rq *rq, u64 clock) {}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ scx_rq_clock_invalidate(ರಚನೆ rq *rq) {}
#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_SCHED_CLASS_EXT */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ assert_balance_callbacks_ಖಾಲಿಗಳು(ರಚನೆ rq *rq)
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
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ rq_pin_ಬೀಗ(ರಚನೆ rq *rq, ರಚನೆ rq_flags *rf)
{
	rf->cookie = lockdep_pin_ಬೀಗ(__rq_lockp(rq));

	rq->clock_update_flags &= (RQCF_REQ_SKIP|RQCF_ACT_SKIP);
	rf->clock_update_flags = 0;
	assert_balance_callbacks_ಖಾಲಿಗಳು(rq);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ rq_unpin_ಬೀಗ(ರಚನೆ rq *rq, ರಚನೆ rq_flags *rf)
{
	ಆದರೆ (rq->clock_update_flags > RQCF_ACT_SKIP)
		rf->clock_update_flags = RQCF_UPDATED;

	scx_rq_clock_invalidate(rq);
	lockdep_unpin_ಬೀಗ(__rq_lockp(rq), rf->cookie);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ rq_repin_ಬೀಗ(ರಚನೆ rq *rq, ರಚನೆ rq_flags *rf)
{
	lockdep_repin_ಬೀಗ(__rq_lockp(rq), rf->cookie);

	/*
	 * Restore the value we stashed in @rf for this pin context.
	 */
	rq->clock_update_flags |= rf->clock_update_flags;
}

#ವ್ಯಾಖ್ಯಾನಿಸು __task_rq_lock(...) __acquire_ret(___task_rq_lock(__VA_ARGS__), __rq_lockp(__ret))
ಬಾಹ್ಯ ರಚನೆ rq *___task_rq_lock(ರಚನೆ task_struct *p, ರಚನೆ rq_flags *rf) __acquires_ret;

#ವ್ಯಾಖ್ಯಾನಿಸು task_rq_ಬೀಗ_2(...) __acquire_ret(task_rq_ಬೀಗ(__VA_ARGS__), __rq_lockp(__ret))
ಬಾಹ್ಯ ರಚನೆ rq *task_rq_ಬೀಗ(ರಚನೆ task_struct *p, ರಚನೆ rq_flags *rf)
	__acquires(&p->pi_ಬೀಗ) __acquires_ret;

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
__task_rq_unlock(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, ರಚನೆ rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_ಬೀಗ(rq, rf);
	raw_spin_rq_unlock(rq);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
task_rq_unlock(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, ರಚನೆ rq_flags *rf)
	__releases(__rq_lockp(rq), &p->pi_ಬೀಗ)
{
	__task_rq_unlock(rq, p, rf);
	raw_spin_unlock_irqrestore(&p->pi_ಬೀಗ, rf->flags);
}

DEFINE_ಬೀಗ_GUARD_1(task_rq_ಬೀಗ_2, ರಚನೆ task_struct,
		    _T->rq = task_rq_ಬೀಗ_2(_T->ಬೀಗ, &_T->rf),
		    task_rq_unlock(_T->rq, _T->ಬೀಗ, &_T->rf),
		    ರಚನೆ rq *rq; ರಚನೆ rq_flags rf)
DECLARE_ಬೀಗ_GUARD_1_ATTRS(task_rq_ಬೀಗ_2, __acquires(_T->pi_ಬೀಗ), __releases((*(ರಚನೆ task_struct **)_T)->pi_ಬೀಗ))
#ವ್ಯಾಖ್ಯಾನಿಸು class_task_rq_ಬೀಗ_constructor(_T) WITH_ಬೀಗ_GUARD_1_ATTRS(task_rq_ಬೀಗ_2, _T)

DEFINE_ಬೀಗ_GUARD_1(__task_rq_lock, ರಚನೆ task_struct,
		    _T->rq = __task_rq_lock(_T->ಬೀಗ, &_T->rf),
		    __task_rq_unlock(_T->rq, _T->ಬೀಗ, &_T->rf),
		    ರಚನೆ rq *rq; ರಚನೆ rq_flags rf)

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ rq_ಬೀಗ_irqsave(ರಚನೆ rq *rq, ರಚನೆ rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_ಬೀಗ_irqsave(rq, rf->flags);
	rq_pin_ಬೀಗ(rq, rf);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ rq_ಬೀಗ_irq(ರಚನೆ rq *rq, ರಚನೆ rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_ಬೀಗ_irq(rq);
	rq_pin_ಬೀಗ(rq, rf);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ rq_ಬೀಗ(ರಚನೆ rq *rq, ರಚನೆ rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_ಬೀಗ(rq);
	rq_pin_ಬೀಗ(rq, rf);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ rq_unlock_irqrestore(ರಚನೆ rq *rq, ರಚನೆ rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_ಬೀಗ(rq, rf);
	raw_spin_rq_unlock_irqrestore(rq, rf->flags);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ rq_unlock_irq(ರಚನೆ rq *rq, ರಚನೆ rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_ಬೀಗ(rq, rf);
	raw_spin_rq_unlock_irq(rq);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ rq_unlock(ರಚನೆ rq *rq, ರಚನೆ rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_ಬೀಗ(rq, rf);
	raw_spin_rq_unlock(rq);
}

DEFINE_ಬೀಗ_GUARD_1(rq_ಬೀಗ, ರಚನೆ rq,
		    rq_ಬೀಗ(_T->ಬೀಗ, &_T->rf),
		    rq_unlock(_T->ಬೀಗ, &_T->rf),
		    ರಚನೆ rq_flags rf)

DECLARE_ಬೀಗ_GUARD_1_ATTRS(rq_ಬೀಗ, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(ರಚನೆ rq **)_T)));
#ವ್ಯಾಖ್ಯಾನಿಸು class_rq_ಬೀಗ_constructor(_T) WITH_ಬೀಗ_GUARD_1_ATTRS(rq_ಬೀಗ, _T)

DEFINE_ಬೀಗ_GUARD_1(rq_ಬೀಗ_irq, ರಚನೆ rq,
		    rq_ಬೀಗ_irq(_T->ಬೀಗ, &_T->rf),
		    rq_unlock_irq(_T->ಬೀಗ, &_T->rf),
		    ರಚನೆ rq_flags rf)

DECLARE_ಬೀಗ_GUARD_1_ATTRS(rq_ಬೀಗ_irq, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(ರಚನೆ rq **)_T)));
#ವ್ಯಾಖ್ಯಾನಿಸು class_rq_ಬೀಗ_irq_constructor(_T) WITH_ಬೀಗ_GUARD_1_ATTRS(rq_ಬೀಗ_irq, _T)

DEFINE_ಬೀಗ_GUARD_1(rq_ಬೀಗ_irqsave, ರಚನೆ rq,
		    rq_ಬೀಗ_irqsave(_T->ಬೀಗ, &_T->rf),
		    rq_unlock_irqrestore(_T->ಬೀಗ, &_T->rf),
		    ರಚನೆ rq_flags rf)

DECLARE_ಬೀಗ_GUARD_1_ATTRS(rq_ಬೀಗ_irqsave, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(ರಚನೆ rq **)_T)));
#ವ್ಯಾಖ್ಯಾನಿಸು class_rq_ಬೀಗ_irqsave_constructor(_T) WITH_ಬೀಗ_GUARD_1_ATTRS(rq_ಬೀಗ_irqsave, _T)

DEFINE_ಬೀಗ_GUARD_1(raw_spin_rq_ಬೀಗ_irqsave, ರಚನೆ rq,
		    raw_spin_rq_ಬೀಗ_irqsave(_T->ಬೀಗ, _T->flags),
		    raw_spin_rq_unlock_irqrestore(_T->ಬೀಗ, _T->flags),
		    ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags)

DECLARE_ಬೀಗ_GUARD_1_ATTRS(raw_spin_rq_ಬೀಗ_irqsave, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(ರಚನೆ rq **)_T)));
#ವ್ಯಾಖ್ಯಾನಿಸು class_raw_spin_rq_ಬೀಗ_irqsave_constructor(_T) \
	WITH_ಬೀಗ_GUARD_1_ATTRS(raw_spin_rq_ಬೀಗ_irqsave, _T)

#ವ್ಯಾಖ್ಯಾನಿಸು this_rq_ಬೀಗ_irq(...) __acquire_ret(this_rq_ಬೀಗ_irq_2(__VA_ARGS__), __rq_lockp(__ret))
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ rq *this_rq_ಬೀಗ_irq_2(ರಚನೆ rq_flags *rf) __acquires_ret
{
	ರಚನೆ rq *rq;

	local_irq_disable();
	rq = this_rq();
	rq_ಬೀಗ(rq, rf);

	ಹಿಂತಿರುಗಿ rq;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_NUMA

ಎಣಿಕೆ numa_topology_type {
	NUMA_ನೇರ,
	NUMA_GLUELESS_MESH,
	NUMA_BACKPLANE,
};

ಬಾಹ್ಯ ಎಣಿಕೆ numa_topology_type sched_numa_topology_type;
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ sched_max_numa_ದೂರ;
ಬಾಹ್ಯ bool ಕಂಡುಹಿಡಿ_numa_ದೂರ(ಪೂರ್ಣಾಂಕ ದೂರ);
ಬಾಹ್ಯ ಶೂನ್ಯ sched_init_numa(ಪೂರ್ಣಾಂಕ offline_ಗಂಟು);
ಬಾಹ್ಯ ಶೂನ್ಯ sched_update_numa(ಪೂರ್ಣಾಂಕ cpu, bool online);
ಬಾಹ್ಯ ಶೂನ್ಯ sched_domains_numa_masks_ಗಣ(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cpu);
ಬಾಹ್ಯ ಶೂನ್ಯ sched_domains_numa_masks_clear(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cpu);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ sched_numa_ಕಂಡುಹಿಡಿ_closest(ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *cpus, ಪೂರ್ಣಾಂಕ cpu);

#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_NUMA: */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ sched_init_numa(ಪೂರ್ಣಾಂಕ offline_ಗಂಟು) { }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ sched_update_numa(ಪೂರ್ಣಾಂಕ cpu, bool online) { }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ sched_domains_numa_masks_ಗಣ(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cpu) { }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ sched_domains_numa_masks_clear(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cpu) { }

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ sched_numa_ಕಂಡುಹಿಡಿ_closest(ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *cpus, ಪೂರ್ಣಾಂಕ cpu)
{
	ಹಿಂತಿರುಗಿ nr_cpu_ids;
}

#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_NUMA */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_NUMA_BALANCING

/* The regions in numa_faults array from task_struct */
ಎಣಿಕೆ numa_ದೋಷಗಳು_stats {
	NUMA_MEM = 0,
	NUMA_CPU,
	NUMA_MEMBUF,
	NUMA_CPUBUF
};

ಬಾಹ್ಯ ಶೂನ್ಯ sched_setnuma(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ ಗಂಟು);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ migrate_task_ಗಮ್ಯ(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ cpu);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ migrate_ಅದಲುಬದಲು(ರಚನೆ task_struct *p, ರಚನೆ task_struct *t,
			ಪೂರ್ಣಾಂಕ cpu, ಪೂರ್ಣಾಂಕ scpu);
ಬಾಹ್ಯ ಶೂನ್ಯ init_numa_balancing(u64 clone_flags, ರಚನೆ task_struct *p);

#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_NUMA_BALANCING: */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
init_numa_balancing(u64 clone_flags, ರಚನೆ task_struct *p)
{
}

#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_NUMA_BALANCING */

ಪೂರ್ಣಾಂಕ task_llc(ಸ್ಥಿರಾಂಕ ರಚನೆ task_struct *p);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
ಸರತಿ_balance_callback(ರಚನೆ rq *rq,
		       ರಚನೆ balance_callback *ತಲೆ,
		       ಶೂನ್ಯ (*func)(ರಚನೆ rq *rq))
{
	lockdep_assert_rq_held(rq);

	/*
	 * Don't (re)queue an already queued item; nor queue anything when
	 * balance_push() is active, see the comment with
	 * balance_push_callback.
	 */
	ಆದರೆ (unlikely(ತಲೆ->ಮುಂದಿನ || rq->balance_callback == &balance_ತಳ್ಳು_callback))
		ಹಿಂತಿರುಗಿ;

	ತಲೆ->func = func;
	ತಲೆ->ಮುಂದಿನ = rq->balance_callback;
	rq->balance_callback = ತಲೆ;
}

#ವ್ಯಾಖ್ಯಾನಿಸು rcu_dereference_sched_domain(p) \
	rcu_dereference_all_check((p), lockdep_is_held(&sched_domains_mutex))

/*
 * The domain tree (rq->sd) is protected by RCU's quiescent state transition.
 * See destroy_sched_domains: call_rcu for details.
 *
 * The domain tree of any CPU may only be accessed from within
 * preempt-disabled sections.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು for_each_domain(cpu, __sd) \
	ಗಾಗಿ (__sd = rcu_dereference_sched_domain(cpu_rq(cpu)->sd); \
			__sd; __sd = __sd->ಪೋಷಕ)

/* A mask of all the SD flags that have the SDF_SHARED_CHILD metaflag */
#ವ್ಯಾಖ್ಯಾನಿಸು SD_FLAG(name, mflags) (name * !!((mflags) & SDF_SHARED_ಮಗು)) |
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ SD_SHARED_ಮಗು_ಮುಖವಾಡ =
#ಅನ್ತರ್ಭಾವ <linux/sched/sd_flags.h>
0;
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು SD_FLAG

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
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ sched_domain *highest_flag_domain(ಪೂರ್ಣಾಂಕ cpu, ಪೂರ್ಣಾಂಕ flag)
{
	ರಚನೆ sched_domain *sd, *hsd = ಖಾಲಿ;

	for_each_domain(cpu, sd) {
		ಆದರೆ (sd->flags & flag) {
			hsd = sd;
			ಮುಂದುವರಿ;
		}

		/*
		 * Stop the search if @flag is known to be shared at lower
		 * levels. It will not be found further up.
		 */
		ಆದರೆ (flag & SD_SHARED_ಮಗು_ಮುಖವಾಡ)
			ನಿಲ್ಲಿಸು;
	}

	ಹಿಂತಿರುಗಿ hsd;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ sched_domain *ಅತಿಕಡಿಮೆ_flag_domain(ಪೂರ್ಣಾಂಕ cpu, ಪೂರ್ಣಾಂಕ flag)
{
	ರಚನೆ sched_domain *sd;

	for_each_domain(cpu, sd) {
		ಆದರೆ (sd->flags & flag)
			ನಿಲ್ಲಿಸು;
	}

	ಹಿಂತಿರುಗಿ sd;
}

DECLARE_ಪ್ರತಿ_CPU(ರಚನೆ sched_domain __rcu *, sd_llc);
DECLARE_ಪ್ರತಿ_CPU(ಪೂರ್ಣಾಂಕ, sd_llc_ಗಾತ್ರ);
DECLARE_ಪ್ರತಿ_CPU(ಪೂರ್ಣಾಂಕ, sd_llc_id);
DECLARE_ಪ್ರತಿ_CPU(ಪೂರ್ಣಾಂಕ, sd_share_id);
DECLARE_ಪ್ರತಿ_CPU(ರಚನೆ sched_domain_shared __rcu *, sd_llc_shared);
DECLARE_ಪ್ರತಿ_CPU(ರಚನೆ sched_domain_shared __rcu *, sd_balance_shared);
DECLARE_ಪ್ರತಿ_CPU(ರಚನೆ sched_domain __rcu *, sd_numa);
DECLARE_ಪ್ರತಿ_CPU(ರಚನೆ sched_domain __rcu *, sd_asym_packing);
DECLARE_ಪ್ರತಿ_CPU(ರಚನೆ sched_domain __rcu *, sd_asym_cpucapacity);

ಬಾಹ್ಯ ರಚನೆ static_ಕೀಲಿ_false sched_asym_cpucapacity;
ಬಾಹ್ಯ ರಚನೆ static_ಕೀಲಿ_false sched_cluster_ಸಕ್ರಿಯ;

ಸ್ಥಿರ __always_inline bool sched_asym_cpucap_ಸಕ್ರಿಯ(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ static_branch_unlikely(&sched_asym_cpucapacity);
}

ರಚನೆ sched_ಗುಂಪು_ಸಾಮರ್ಥ್ಯ {
	atomic_t		ref;
	/*
	 * CPU capacity of this group, SCHED_CAPACITY_SCALE being max capacity
	 * for a single CPU.
	 */
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ		ಸಾಮರ್ಥ್ಯ;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ		min_ಸಾಮರ್ಥ್ಯ;		/* Min per-CPU capacity in group */
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ		max_ಸಾಮರ್ಥ್ಯ;		/* Max per-CPU capacity in group */
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ		ಮುಂದಿನ_update;
	ಪೂರ್ಣಾಂಕ			imbalance;		/* XXX unrelated to capacity but shared group state */

	ಪೂರ್ಣಾಂಕ			id;

	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ		cpumask[];		/* Balance mask */
};

ರಚನೆ sched_ಗುಂಪು {
	ರಚನೆ sched_ಗುಂಪು	*ಮುಂದಿನ;			/* Must be a circular list */
	atomic_t		ref;

	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		ಗುಂಪು_ತೂಕ;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		cores;
	ರಚನೆ sched_ಗುಂಪು_ಸಾಮರ್ಥ್ಯ *sgc;
	ಪೂರ್ಣಾಂಕ			asym_prefer_cpu;	/* CPU of highest priority in group */
	ಪೂರ್ಣಾಂಕ			flags;

	/*
	 * The CPUs this group covers.
	 *
	 * NOTE: this field is variable length. (Allocated dynamically
	 * by attaching extra space to the end of the structure,
	 * depending on how many CPUs the kernel has booted up with)
	 */
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ		cpumask[];
};

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ cpumask *sched_ಗುಂಪು_ಹರವು(ರಚನೆ sched_ಗುಂಪು *sg)
{
	ಹಿಂತಿರುಗಿ ಗಮ್ಯ_cpumask(sg->cpumask);
}

/*
 * See build_balance_mask().
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ cpumask *ಗುಂಪು_balance_ಮುಖವಾಡ(ರಚನೆ sched_ಗುಂಪು *sg)
{
	ಹಿಂತಿರುಗಿ ಗಮ್ಯ_cpumask(sg->sgc->cpumask);
}

ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ಗುಂಪು_balance_cpu(ರಚನೆ sched_ಗುಂಪು *sg);

ಬಾಹ್ಯ ಶೂನ್ಯ update_sched_domain_debugfs(ಶೂನ್ಯ);
ಬಾಹ್ಯ ಶೂನ್ಯ dirty_sched_domain_sysctl(ಪೂರ್ಣಾಂಕ cpu);

ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ sched_update_scaling(ಶೂನ್ಯ);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask *task_user_cpus(ರಚನೆ task_struct *p)
{
	ಆದರೆ (!p->user_cpus_ptr)
		ಹಿಂತಿರುಗಿ cpu_possible_ಮುಖವಾಡ; /* &init_task.cpus_mask */
	ಹಿಂತಿರುಗಿ p->user_cpus_ptr;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_CGROUP_SCHED

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
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ task_ಗುಂಪು *task_ಗುಂಪು(ರಚನೆ task_struct *p)
{
	ಹಿಂತಿರುಗಿ p->sched_task_ಗುಂಪು;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_FAIR_ಗುಂಪು_SCHED
/*
 * Defined here to be available before stats.h is included, since
 * stats.h has dependencies on things defined later in this file.
 */
ರಚನೆ cfs_tg_state {
	ರಚನೆ cfs_rq		cfs_rq;
	ರಚನೆ sched_entity	se;
	ರಚನೆ sched_statistics	stats;
} __no_randomize_layout;

/* Access a specific CPU's cfs_rq from a task group */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ cfs_rq *tg_cfs_rq(ರಚನೆ task_ಗುಂಪು *tg, ಪೂರ್ಣಾಂಕ cpu)
{
	ಹಿಂತಿರುಗಿ ಪ್ರತಿ_cpu_ptr(tg->cfs_rq, cpu);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ sched_entity *tg_se(ರಚನೆ task_ಗುಂಪು *tg, ಪೂರ್ಣಾಂಕ cpu)
{
	ರಚನೆ cfs_tg_state *state;

	ಆದರೆ (is_ಬೇರು_task_ಗುಂಪು(tg))
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	state = container_ದ(tg_cfs_rq(tg, cpu), ರಚನೆ cfs_tg_state, cfs_rq);
	ಹಿಂತಿರುಗಿ &state->se;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ sched_entity *cfs_rq_se(ರಚನೆ cfs_rq *cfs_rq)
{
	ರಚನೆ cfs_tg_state *state;

	ಆದರೆ (is_ಬೇರು_task_ಗುಂಪು(cfs_rq->tg))
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	state = container_ದ(cfs_rq, ರಚನೆ cfs_tg_state, cfs_rq);
	ಹಿಂತಿರುಗಿ &state->se;
}
#ಆದರೆ_ಅಂತ್ಯ

/* Change a task's cfs_rq and parent entity if it moves across CPUs/groups */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ ಗಣ_task_rq(ರಚನೆ task_struct *p, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cpu)
{
#ಆದರೆ defined(CONFIG_FAIR_ಗುಂಪು_SCHED) || defined(CONFIG_RT_ಗುಂಪು_SCHED)
	ರಚನೆ task_ಗುಂಪು *tg = task_ಗುಂಪು(p);
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_FAIR_ಗುಂಪು_SCHED
	ಗಣ_task_rq_fair(&p->se, p->se.cfs_rq, tg_cfs_rq(tg, cpu));
	p->se.cfs_rq = tg_cfs_rq(tg, cpu);
	p->se.ಪೋಷಕ = tg_se(tg, cpu);
	p->se.ಆಳ = p->se.ಪೋಷಕ ? p->se.ಪೋಷಕ->ಆಳ + 1 : 0;
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_RT_ಗುಂಪು_SCHED
	/*
	 * p->rt.rt_rq is NULL initially and it is easier to assign
	 * root_task_group's rt_rq than switching in rt_rq_of_se()
	 * Clobbers tg(!)
	 */
	ಆದರೆ (!rt_ಗುಂಪು_sched_enabled())
		tg = &ಬೇರು_task_ಗುಂಪು;
	p->rt.rt_rq  = tg->rt_rq[cpu];
	p->rt.ಪೋಷಕ = tg->rt_se[cpu];
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_RT_GROUP_SCHED */
}

#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_CGROUP_SCHED: */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ ಗಣ_task_rq(ರಚನೆ task_struct *p, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cpu) { }

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ task_ಗುಂಪು *task_ಗುಂಪು(ರಚನೆ task_struct *p)
{
	ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_CGROUP_SCHED */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ __set_task_cpu(ರಚನೆ task_struct *p, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cpu)
{
	ಗಣ_task_rq(p, cpu);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SMP
	/*
	 * After ->cpu is set up to a new value, task_rq_lock(p, ...) can be
	 * successfully executed on another CPU. We must ensure that updates of
	 * per-task data have been completed by this moment.
	 */
	smp_wmb();
	WRITE_ONCE(task_thread_info(p)->cpu, cpu);
	p->wake_cpu = cpu;
	rseq_sched_ಗಣ_ids_ಬದಲಾಯಿತು(p);
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_SMP */
}

/*
 * Tunables:
 */

#ವ್ಯಾಖ್ಯಾನಿಸು SCHED_FEAT(name, enabled)	\
	__SCHED_FEAT_##name ,

ಎಣಿಕೆ {
#ಅನ್ತರ್ಭಾವ "features.h"
	__SCHED_FEAT_NR,
};

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು SCHED_FEAT

/*
 * To support run-time toggling of sched features, all the translation units
 * (but core.c) reference the sysctl_sched_features defined in core.c.
 */
ಬಾಹ್ಯ __read_mostly ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ sysctl_sched_features;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_JUMP_LABEL

ಒಕ್ಕೂಟ sched_feat_ಕೀಲಿ {
	ರಚನೆ static_ಕೀಲಿ_true	ಕೀಲಿ_true;
	ರಚನೆ static_ಕೀಲಿ_false	ಕೀಲಿ_false;
};

#ವ್ಯಾಖ್ಯಾನಿಸು sched_feat_branch_true(ಕೀಲಿ)	static_branch_likely(&(ಕೀಲಿ)->ಕೀಲಿ_true)
#ವ್ಯಾಖ್ಯಾನಿಸು sched_feat_branch_false(ಕೀಲಿ)	static_branch_unlikely(&(ಕೀಲಿ)->ಕೀಲಿ_false)

#ವ್ಯಾಖ್ಯಾನಿಸು SCHED_FEAT(name, enabled)			\
ಸ್ಥಿರ __always_inline bool				\
static_branch_##name(ಒಕ್ಕೂಟ sched_feat_ಕೀಲಿ *ಕೀಲಿ) 	\
{							\
	ಹಿಂತಿರುಗಿ sched_feat_branch_##enabled(ಕೀಲಿ);	\
}

#ಅನ್ತರ್ಭಾವ "features.h"
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು SCHED_FEAT

ಬಾಹ್ಯ ಒಕ್ಕೂಟ sched_feat_ಕೀಲಿ sched_feat_ಕೀಲಿಗಳು[__SCHED_FEAT_NR];
#ವ್ಯಾಖ್ಯಾನಿಸು sched_feat(x) (static_branch_##x(&sched_feat_ಕೀಲಿಗಳು[__SCHED_FEAT_##x]))

#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_JUMP_LABEL: */

#ವ್ಯಾಖ್ಯಾನಿಸು sched_feat(x) (sysctl_sched_features & (1UL << __SCHED_FEAT_##x))

#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_JUMP_LABEL */

ಬಾಹ್ಯ ರಚನೆ static_ಕೀಲಿ_false sched_numa_balancing;
ಬಾಹ್ಯ ರಚನೆ static_ಕೀಲಿ_false sched_schedstats;

ಸ್ಥಿರ ಸಾಲೊಳಗೆ u64 global_rt_period(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ (u64)sysctl_sched_rt_period * NSEC_ಪ್ರತಿ_USEC;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ u64 global_rt_runtime(ಶೂನ್ಯ)
{
	ಆದರೆ (sysctl_sched_rt_runtime < 0)
		ಹಿಂತಿರುಗಿ RUNTIME_INF;

	ಹಿಂತಿರುಗಿ (u64)sysctl_sched_rt_runtime * NSEC_ಪ್ರತಿ_USEC;
}

/*
 * Is p the current execution context?
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ task_current(ರಚನೆ rq *rq, ರಚನೆ task_struct *p)
{
	ಹಿಂತಿರುಗಿ rq->curr == p;
}

/*
 * Is p the current scheduling context?
 *
 * Note that it might be the current execution context at the same time if
 * rq->curr == rq->donor == p.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ task_current_donor(ರಚನೆ rq *rq, ರಚನೆ task_struct *p)
{
	ಹಿಂತಿರುಗಿ rq->donor == p;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool task_is_blocked(ರಚನೆ task_struct *p)
{
	ಆದರೆ (!sched_proxy_exec())
		ಹಿಂತಿರುಗಿ false;

	ಹಿಂತಿರುಗಿ !!p->blocked_on;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_PROXY_EXEC
ಶೂನ್ಯ sched_proxy_ಖಂಡ_task(ರಚನೆ rq *rq, ರಚನೆ task_struct *p);
#ಇಲ್ಲದಿದ್ದರೆ
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ sched_proxy_ಖಂಡ_task(ರಚನೆ rq *rq, ರಚನೆ task_struct *p) {}
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ task_on_cpu(ರಚನೆ rq *rq, ರಚನೆ task_struct *p)
{
	ಹಿಂತಿರುಗಿ p->on_cpu;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ task_on_rq_queued(ರಚನೆ task_struct *p)
{
	ಹಿಂತಿರುಗಿ READ_ONCE(p->on_rq) == TASK_ON_RQ_QUEUED;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ task_on_rq_migrating(ರಚನೆ task_struct *p)
{
	ಹಿಂತಿರುಗಿ READ_ONCE(p->on_rq) == TASK_ON_RQ_MIGRATING;
}

/* Wake flags. The first three directly map to some SD flag value */
#ವ್ಯಾಖ್ಯಾನಿಸು WF_EXEC			0x02 /* Wakeup after exec; maps to SD_BALANCE_EXEC */
#ವ್ಯಾಖ್ಯಾನಿಸು WF_FORK			0x04 /* Wakeup after fork; maps to SD_BALANCE_FORK */
#ವ್ಯಾಖ್ಯಾನಿಸು WF_TTWU			0x08 /* Wakeup;            maps to SD_BALANCE_WAKE */
/*
 * Hint that the caller expects the waker to sleep soon.
 * Scheduler classes may use it for placement or preemption.
 * Callers must not rely on it to prevent migration,
 * preserve CPU locality or make the wakee run next.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು WF_SYNC			0x10
#ವ್ಯಾಖ್ಯಾನಿಸು WF_MIGRATED		0x20 /* Internal use, task got migrated */
#ವ್ಯಾಖ್ಯಾನಿಸು WF_CURRENT_CPU		0x40 /* Prefer to move the wakee to the current CPU. */
#ವ್ಯಾಖ್ಯಾನಿಸು WF_RQ_SELECTED		0x80 /* ->select_task_rq() was called */
#ವ್ಯಾಖ್ಯಾನಿಸು WF_TTWU_RQ		0x100 /* Wakeup completed through ttwu_runnable() */

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

#ವ್ಯಾಖ್ಯಾನಿಸು ತೂಕ_IDLEPRIO		3
#ವ್ಯಾಖ್ಯಾನಿಸು WMULT_IDLEPRIO		1431655765

ಬಾಹ್ಯ ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ		sched_prio_ಗಮ್ಯ_ತೂಕ[40];
ಬಾಹ್ಯ ಸ್ಥಿರಾಂಕ u32		sched_prio_ಗಮ್ಯ_wmult[40];

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

#ವ್ಯಾಖ್ಯಾನಿಸು ಸರತಿಯಿಂದ_ತೆಗೆ_SLEEP		0x0001 /* Matches ENQUEUE_WAKEUP */
#ವ್ಯಾಖ್ಯಾನಿಸು ಸರತಿಯಿಂದ_ತೆಗೆ_SAVE		0x0002 /* Matches ENQUEUE_RESTORE */
#ವ್ಯಾಖ್ಯಾನಿಸು ಸರತಿಯಿಂದ_ತೆಗೆ_MOVE		0x0004 /* Matches ENQUEUE_MOVE */
#ವ್ಯಾಖ್ಯಾನಿಸು ಸರತಿಯಿಂದ_ತೆಗೆ_NOCLOCK		0x0008 /* Matches ENQUEUE_NOCLOCK */

#ವ್ಯಾಖ್ಯಾನಿಸು ಸರತಿಯಿಂದ_ತೆಗೆ_MIGRATING	0x0010 /* Matches ENQUEUE_MIGRATING */
#ವ್ಯಾಖ್ಯಾನಿಸು ಸರತಿಯಿಂದ_ತೆಗೆ_DELAYED		0x0020 /* Matches ENQUEUE_DELAYED */
#ವ್ಯಾಖ್ಯಾನಿಸು ಸರತಿಯಿಂದ_ತೆಗೆ_CLASS		0x0040 /* Matches ENQUEUE_CLASS */

#ವ್ಯಾಖ್ಯಾನಿಸು ಸರತಿಯಿಂದ_ತೆಗೆ_SPECIAL		0x00010000
#ವ್ಯಾಖ್ಯಾನಿಸು ಸರತಿಯಿಂದ_ತೆಗೆ_THROTTLE	0x00020000

#ವ್ಯಾಖ್ಯಾನಿಸು ಸರತಿಗೆ_ಸೇರಿಸು_WAKEUP		0x0001
#ವ್ಯಾಖ್ಯಾನಿಸು ಸರತಿಗೆ_ಸೇರಿಸು_RESTORE		0x0002
#ವ್ಯಾಖ್ಯಾನಿಸು ಸರತಿಗೆ_ಸೇರಿಸು_MOVE		0x0004
#ವ್ಯಾಖ್ಯಾನಿಸು ಸರತಿಗೆ_ಸೇರಿಸು_NOCLOCK		0x0008

#ವ್ಯಾಖ್ಯಾನಿಸು ಸರತಿಗೆ_ಸೇರಿಸು_MIGRATING	0x0010
#ವ್ಯಾಖ್ಯಾನಿಸು ಸರತಿಗೆ_ಸೇರಿಸು_DELAYED		0x0020
#ವ್ಯಾಖ್ಯಾನಿಸು ಸರತಿಗೆ_ಸೇರಿಸು_CLASS		0x0040

#ವ್ಯಾಖ್ಯಾನಿಸು ಸರತಿಗೆ_ಸೇರಿಸು_ತಲೆ		0x00010000
#ವ್ಯಾಖ್ಯಾನಿಸು ಸರತಿಗೆ_ಸೇರಿಸು_REPLENISH	0x00020000
#ವ್ಯಾಖ್ಯಾನಿಸು ಸರತಿಗೆ_ಸೇರಿಸು_MIGRATED	0x00040000
#ವ್ಯಾಖ್ಯಾನಿಸು ಸರತಿಗೆ_ಸೇರಿಸು_INITIAL		0x00080000
#ವ್ಯಾಖ್ಯಾನಿಸು ಸರತಿಗೆ_ಸೇರಿಸು_RQ_SELECTED	0x00100000
#ವ್ಯಾಖ್ಯಾನಿಸು ಸರತಿಗೆ_ಸೇರಿಸು_QUEUED		0x00200000

#ವ್ಯಾಖ್ಯಾನಿಸು RETRY_TASK		((ಶೂನ್ಯ *)-1UL)

ರಚನೆ affinity_context {
	ಸ್ಥಿರಾಂಕ ರಚನೆ cpumask	*new_ಮುಖವಾಡ;
	ರಚನೆ cpumask		*user_ಮುಖವಾಡ;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		flags;
};

ಬಾಹ್ಯ s64 update_curr_common(ರಚನೆ rq *rq);

ಎಣಿಕೆ snt_e {
	SNT_ಸಾಮಾನ್ಯ,	/* set_next_task() */
	SNT_PICK,	/* put_prev_set_next_task(): prev != next */
	SNT_REPICK,	/* put_prev_set_next_task(): prev == next */
};

ರಚನೆ sched_class {

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_UCLAMP_TASK
	ಪೂರ್ಣಾಂಕ uclamp_enabled;
#ಆದರೆ_ಅಂತ್ಯ

	/*
	 * move_queued_task/activate_task/enqueue_task: rq->lock
	 * ttwu_do_activate/activate_task/enqueue_task: rq->lock
	 * wake_up_new_task/activate_task/enqueue_task: task_rq_lock
	 * ttwu_runnable/enqueue_task: task_rq_lock
	 * proxy_task_current: rq->lock
	 * sched_change_end
	 */
	ಶೂನ್ಯ (*ಸರತಿಗೆ_ಸೇರಿಸು_task) (ರಚನೆ rq *rq, ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ flags);
	/*
	 * move_queued_task/deactivate_task/dequeue_task: rq->lock
	 * __schedule/block_task/dequeue_task: rq->lock
	 * proxy_task_current: rq->lock
	 * wait_task_inactive: task_rq_lock
	 * sched_change_begin
	 */
	bool (*ಸರತಿಯಿಂದ_ತೆಗೆ_task) (ರಚನೆ rq *rq, ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ flags);

	/*
	 * do_sched_yield: rq->lock
	 */
	ಶೂನ್ಯ (*yield_task)   (ರಚನೆ rq *rq);
	/*
	 * yield_to: rq->lock (double)
	 */
	bool (*yield_ಗಮ್ಯ_task)(ರಚನೆ rq *rq, ರಚನೆ task_struct *p);

	/*
	 * move_queued_task: rq->lock
	 * __migrate_swap_task: rq->lock
	 * ttwu_do_activate: rq->lock
	 * ttwu_runnable: task_rq_lock
	 * wake_up_new_task: task_rq_lock
	 */
	ಶೂನ್ಯ (*wakeup_preempt)(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ flags);

	/*
	 * schedule/pick_next_task/prev_balance: rq->lock
	 */
	ಪೂರ್ಣಾಂಕ (*balance)(ರಚನೆ rq *rq, ರಚನೆ rq_flags *rf);

	/*
	 * schedule/pick_next_task: rq->lock
	 */
	ರಚನೆ task_struct *(*pick_task)(ರಚನೆ rq *rq, ರಚನೆ rq_flags *rf);

	/*
	 * sched_change:
	 * __schedule: rq->lock
	 */
	ಶೂನ್ಯ (*put_prev_task)(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, ರಚನೆ task_struct *ಮುಂದಿನ);
	ಶೂನ್ಯ (*ಗಣ_ಮುಂದಿನ_task)(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, ಎಣಿಕೆ snt_e type);

	/*
	 * select_task_rq: p->pi_lock
	 * sched_exec: p->pi_lock
	 */
	ಪೂರ್ಣಾಂಕ  (*select_task_rq)(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ task_cpu, ಪೂರ್ಣಾಂಕ flags);

	/*
	 * set_task_cpu: p->pi_lock || rq->lock (ttwu like)
	 */
	ಶೂನ್ಯ (*migrate_task_rq)(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ new_cpu);

	/*
	 * ttwu_do_activate: rq->lock
	 * wake_up_new_task: task_rq_lock
	 */
	ಶೂನ್ಯ (*task_woken)(ರಚನೆ rq *this_rq, ರಚನೆ task_struct *task);

	/*
	 * do_set_cpus_allowed: task_rq_lock + sched_change
	 */
	ಶೂನ್ಯ (*ಗಣ_cpus_allowed)(ರಚನೆ task_struct *p, ರಚನೆ affinity_context *ctx);

	/*
	 * sched_set_rq_{on,off}line: rq->lock
	 */
	ಶೂನ್ಯ (*rq_online)(ರಚನೆ rq *rq);
	ಶೂನ್ಯ (*rq_offline)(ರಚನೆ rq *rq);

	/*
	 * push_cpu_stop: p->pi_lock && rq->lock
	 */
	ರಚನೆ rq *(*ಕಂಡುಹಿಡಿ_ಬೀಗ_rq)(ರಚನೆ task_struct *p, ರಚನೆ rq *rq);

	/*
	 * hrtick: rq->lock
	 * sched_tick: rq->lock
	 * sched_tick_remote: rq->lock
	 */
	ಶೂನ್ಯ (*task_tick)(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ queued);
	/*
	 * sched_cgroup_fork: p->pi_lock
	 */
	ಶೂನ್ಯ (*task_fork)(ರಚನೆ task_struct *p);
	/*
	 * finish_task_switch: no locks
	 */
	ಶೂನ್ಯ (*task_dead)(ರಚನೆ task_struct *p);

	/*
	 * sched_change
	 */
	ಶೂನ್ಯ (*switching_from)(ರಚನೆ rq *this_rq, ರಚನೆ task_struct *task);
	ಶೂನ್ಯ (*switched_from) (ರಚನೆ rq *this_rq, ರಚನೆ task_struct *task);
	ಶೂನ್ಯ (*switching_ಗಮ್ಯ)  (ರಚನೆ rq *this_rq, ರಚನೆ task_struct *task);
	ಶೂನ್ಯ (*switched_ಗಮ್ಯ)   (ರಚನೆ rq *this_rq, ರಚನೆ task_struct *task);
	u64  (*get_prio)     (ರಚನೆ rq *this_rq, ರಚನೆ task_struct *task);
	ಶೂನ್ಯ (*prio_ಬದಲಾಯಿತು) (ರಚನೆ rq *this_rq, ರಚನೆ task_struct *task,
			      u64 oldprio);

	/*
	 * set_load_weight: task_rq_lock + sched_change
	 * __setscheduler_parms: task_rq_lock + sched_change
	 */
	ಶೂನ್ಯ (*reweight_task)(ರಚನೆ rq *this_rq, ರಚನೆ task_struct *task,
			      ಸ್ಥಿರಾಂಕ ರಚನೆ ತುಂಬು_ತೂಕ *lw);

	/*
	 * sched_rr_get_interval: task_rq_lock
	 */
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ (*get_rr_interval)(ರಚನೆ rq *rq,
					ರಚನೆ task_struct *task);

	/*
	 * task_sched_runtime: task_rq_lock
	 */
	ಶೂನ್ಯ (*update_curr)(ರಚನೆ rq *rq);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_FAIR_ಗುಂಪು_SCHED
	/*
	 * sched_change_group: task_rq_lock + sched_change
	 */
	ಶೂನ್ಯ (*task_change_ಗುಂಪು)(ರಚನೆ task_struct *p);
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CORE
	/*
	 * pick_next_task: rq->lock
	 * try_steal_cookie: rq->lock (double)
	 */
	ಪೂರ್ಣಾಂಕ (*task_is_throttled)(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ cpu);
#ಆದರೆ_ಅಂತ್ಯ
};

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ put_prev_task(ರಚನೆ rq *rq, ರಚನೆ task_struct *prev)
{
	WARN_ON_ONCE(rq->donor != prev);
	prev->sched_class->put_prev_task(rq, prev, ಖಾಲಿ);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ ಗಣ_ಮುಂದಿನ_task(ರಚನೆ rq *rq, ರಚನೆ task_struct *ಮುಂದಿನ)
{
	ಮುಂದಿನ->sched_class->ಗಣ_ಮುಂದಿನ_task(rq, ಮುಂದಿನ, SNT_ಸಾಮಾನ್ಯ);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
__put_prev_set_next_dl_server(ರಚನೆ rq *rq,
			      ರಚನೆ task_struct *prev,
			      ರಚನೆ task_struct *ಮುಂದಿನ)
{
	prev->dl_server = ಖಾಲಿ;
	ಮುಂದಿನ->dl_server = rq->dl_server;
	rq->dl_server = ಖಾಲಿ;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ put_prev_ಗಣ_ಮುಂದಿನ_task(ರಚನೆ rq *rq,
					  ರಚನೆ task_struct *prev,
					  ರಚನೆ task_struct *ಮುಂದಿನ)
{
	WARN_ON_ONCE(rq->donor != prev);

	__put_prev_set_next_dl_server(rq, prev, ಮುಂದಿನ);

	ಆದರೆ (ಮುಂದಿನ == prev) {
		ಮುಂದಿನ->sched_class->ಗಣ_ಮುಂದಿನ_task(rq, ಮುಂದಿನ, SNT_REPICK);
		ಹಿಂತಿರುಗಿ;
	}

	prev->sched_class->put_prev_task(rq, prev, ಮುಂದಿನ);
	ಮುಂದಿನ->sched_class->ಗಣ_ಮುಂದಿನ_task(rq, ಮುಂದಿನ, SNT_PICK);
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
#ವ್ಯಾಖ್ಯಾನಿಸು DEFINE_SCHED_CLASS(name) \
ಸ್ಥಿರಾಂಕ ರಚನೆ sched_class name##_sched_class \
	__aligned(__alignof__(ರಚನೆ sched_class)) \
	__section("__" #name "_sched_class")

/* Defined in include/asm-generic/vmlinux.lds.h */
ಬಾಹ್ಯ ರಚನೆ sched_class __sched_class_highest[];
ಬಾಹ್ಯ ರಚನೆ sched_class __sched_class_lowest[];

ಬಾಹ್ಯ ಸ್ಥಿರಾಂಕ ರಚನೆ sched_class stop_sched_class;
ಬಾಹ್ಯ ಸ್ಥಿರಾಂಕ ರಚನೆ sched_class dl_sched_class;
ಬಾಹ್ಯ ಸ್ಥಿರಾಂಕ ರಚನೆ sched_class rt_sched_class;
ಬಾಹ್ಯ ಸ್ಥಿರಾಂಕ ರಚನೆ sched_class fair_sched_class;
ಬಾಹ್ಯ ಸ್ಥಿರಾಂಕ ರಚನೆ sched_class idle_sched_class;

/*
 * Iterate only active classes. SCX can take over all fair tasks or be
 * completely disabled. If the former, skip fair. If the latter, skip SCX.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಸ್ಥಿರಾಂಕ ರಚನೆ sched_class *ಮುಂದಿನ_ಸಕ್ರಿಯ_class(ಸ್ಥಿರಾಂಕ ರಚನೆ sched_class *class)
{
	class++;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CLASS_EXT
	ಆದರೆ (scx_switched_all() && class == &fair_sched_class)
		class++;
	ಆದರೆ (!scx_enabled() && class == &ext_sched_class)
		class++;
#ಆದರೆ_ಅಂತ್ಯ
	ಹಿಂತಿರುಗಿ class;
}

#ವ್ಯಾಖ್ಯಾನಿಸು for_class_range(class, _from, ಗಮ್ಯ_2) \
	ಗಾಗಿ (class = (_from); class < (ಗಮ್ಯ_2); class++)

#ವ್ಯಾಖ್ಯಾನಿಸು for_each_class(class) \
	for_class_range(class, __sched_class_highest, __sched_class_lowest)

#ವ್ಯಾಖ್ಯಾನಿಸು for_ಸಕ್ರಿಯ_class_range(class, _from, ಗಮ್ಯ_2)				\
	ಗಾಗಿ (class = (_from); class != (ಗಮ್ಯ_2); class = ಮುಂದಿನ_ಸಕ್ರಿಯ_class(class))

#ವ್ಯಾಖ್ಯಾನಿಸು for_each_ಸಕ್ರಿಯ_class(class)						\
	for_ಸಕ್ರಿಯ_class_range(class, __sched_class_highest, __sched_class_lowest)

#ವ್ಯಾಖ್ಯಾನಿಸು sched_class_above(_a, _b)	((_a) < (_b))

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ rq_modified_begin(ರಚನೆ rq *rq, ಸ್ಥಿರಾಂಕ ರಚನೆ sched_class *class)
{
	ಆದರೆ (sched_class_above(rq->ಮುಂದಿನ_class, class))
		rq->ಮುಂದಿನ_class = class;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool rq_modified_above(ರಚನೆ rq *rq, ಸ್ಥಿರಾಂಕ ರಚನೆ sched_class *class)
{
	ಹಿಂತಿರುಗಿ sched_class_above(rq->ಮುಂದಿನ_class, class);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool sched_stop_runnable(ರಚನೆ rq *rq)
{
	ಹಿಂತಿರುಗಿ rq->stop && task_on_rq_queued(rq->stop);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool sched_dl_runnable(ರಚನೆ rq *rq)
{
	ಹಿಂತಿರುಗಿ rq->dl.dl_nr_running > 0;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool sched_rt_runnable(ರಚನೆ rq *rq)
{
	ಹಿಂತಿರುಗಿ rq->rt.rt_queued > 0;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool sched_fair_runnable(ರಚನೆ rq *rq)
{
	ಹಿಂತಿರುಗಿ rq->cfs.nr_queued > 0;
}

ಬಾಹ್ಯ ರಚನೆ task_struct *pick_task_fair(ರಚನೆ rq *rq, ರಚನೆ rq_flags *rf);
ಬಾಹ್ಯ ರಚನೆ task_struct *pick_task_idle(ರಚನೆ rq *rq, ರಚನೆ rq_flags *rf);

#ವ್ಯಾಖ್ಯಾನಿಸು SCA_CHECK		0x01
#ವ್ಯಾಖ್ಯಾನಿಸು SCA_MIGRATE_DISABLE	0x02
#ವ್ಯಾಖ್ಯಾನಿಸು SCA_MIGRATE_ENABLE	0x04
#ವ್ಯಾಖ್ಯಾನಿಸು SCA_USER		0x08

ಬಾಹ್ಯ ಶೂನ್ಯ update_ಗುಂಪು_ಸಾಮರ್ಥ್ಯ(ರಚನೆ sched_domain *sd, ಪೂರ್ಣಾಂಕ cpu);

ಬಾಹ್ಯ ಶೂನ್ಯ sched_balance_trigger(ರಚನೆ rq *rq);

ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ __set_cpus_allowed_ptr(ರಚನೆ task_struct *p, ರಚನೆ affinity_context *ctx);
ಬಾಹ್ಯ ಶೂನ್ಯ ಗಣ_cpus_allowed_common(ರಚನೆ task_struct *p, ರಚನೆ affinity_context *ctx);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool task_allowed_on_cpu(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ cpu)
{
	/* When not in the task's cpumask, no point in looking further. */
	ಆದರೆ (!cpumask_test_cpu(cpu, p->cpus_ptr))
		ಹಿಂತಿರುಗಿ false;

	/* Can @cpu run a user thread? */
	ಆದರೆ (!(p->flags & PF_KTHREAD) && !task_cpu_possible(cpu, p))
		ಹಿಂತಿರುಗಿ false;

	ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ cpumask_t *alloc_user_cpus_ptr(ಪೂರ್ಣಾಂಕ ಗಂಟು)
{
	/*
	 * See set_cpus_allowed_force() above for the rcu_head usage.
	 */
	ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2 = max_t(ಪೂರ್ಣಾಂಕ, cpumask_ಗಾತ್ರ(), ಗಾತ್ರ(ರಚನೆ rcu_ತಲೆ));

	ಹಿಂತಿರುಗಿ kmalloc_ಗಂಟು(ಗಾತ್ರ_2, GFP_KERNEL, ಗಂಟು);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ task_struct *get_ತಳ್ಳು_task(ರಚನೆ rq *rq)
{
	ರಚನೆ task_struct *p = rq->donor;

	lockdep_assert_rq_held(rq);

	ಆದರೆ (rq->ತಳ್ಳು_busy)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	ಆದರೆ (p->nr_cpus_allowed == 1)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	ಆದರೆ (p->migration_disabled)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	rq->ತಳ್ಳು_busy = true;
	ಹಿಂತಿರುಗಿ get_task_struct(p);
}

ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ತಳ್ಳು_cpu_stop(ಶೂನ್ಯ *arg);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_CPU_IDLE

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ idle_ಗಣ_state(ರಚನೆ rq *rq,
				  ರಚನೆ cpuidle_state *idle_state)
{
	rq->idle_state = idle_state;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ cpuidle_state *idle_get_state(ರಚನೆ rq *rq)
{
	lockdep_assert(rcu_read_ಬೀಗ_any_held());

	ಹಿಂತಿರುಗಿ rq->idle_state;
}

#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_CPU_IDLE: */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ idle_ಗಣ_state(ರಚನೆ rq *rq,
				  ರಚನೆ cpuidle_state *idle_state)
{
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ cpuidle_state *idle_get_state(ರಚನೆ rq *rq)
{
	ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_CPU_IDLE */

ಬಾಹ್ಯ ಶೂನ್ಯ ವೇಳಾಪಟ್ಟಿ_idle(ಶೂನ್ಯ);
asmlinkage ಶೂನ್ಯ ವೇಳಾಪಟ್ಟಿ_user(ಶೂನ್ಯ);

ಬಾಹ್ಯ ಶೂನ್ಯ sysrq_sched_debug_show(ಶೂನ್ಯ);
ಬಾಹ್ಯ ಶೂನ್ಯ sched_init_granularity(ಶೂನ್ಯ);
ಬಾಹ್ಯ ಶೂನ್ಯ update_max_interval(ಶೂನ್ಯ);

ಬಾಹ್ಯ ಶೂನ್ಯ init_sched_dl_class(ಶೂನ್ಯ);
ಬಾಹ್ಯ ಶೂನ್ಯ init_sched_rt_class(ಶೂನ್ಯ);
ಬಾಹ್ಯ ಶೂನ್ಯ init_sched_fair_class(ಶೂನ್ಯ);

ಬಾಹ್ಯ ಶೂನ್ಯ resched_curr(ರಚನೆ rq *rq);
ಬಾಹ್ಯ ಶೂನ್ಯ resched_curr_lazy(ರಚನೆ rq *rq);
ಬಾಹ್ಯ ಶೂನ್ಯ resched_cpu(ಪೂರ್ಣಾಂಕ cpu);

ಬಾಹ್ಯ ಶೂನ್ಯ init_rt_bandwidth(ರಚನೆ rt_bandwidth *rt_b, u64 period, u64 runtime);
ಬಾಹ್ಯ bool sched_rt_bandwidth_account(ರಚನೆ rt_rq *rt_rq);

ಬಾಹ್ಯ ಶೂನ್ಯ init_dl_entity(ರಚನೆ sched_dl_entity *dl_se);

ಬಾಹ್ಯ ಶೂನ್ಯ init_cfs_throttle_work(ರಚನೆ task_struct *p);

#ವ್ಯಾಖ್ಯಾನಿಸು BW_ಸರಿಸು		20
#ವ್ಯಾಖ್ಯಾನಿಸು BW_UNIT			(1 << BW_ಸರಿಸು)
#ವ್ಯಾಖ್ಯಾನಿಸು ಅನುಪಾತ_ಸರಿಸು		8
#ವ್ಯಾಖ್ಯಾನಿಸು MAX_BW_ಬಿಟ್ಟುಗಳು		(64 - BW_ಸರಿಸು)
#ವ್ಯಾಖ್ಯಾನಿಸು MAX_BW			((1ULL << MAX_BW_ಬಿಟ್ಟುಗಳು) - 1)

ಬಾಹ್ಯ u64 ಗಮ್ಯ_ಅನುಪಾತ(u64 period, u64 runtime);

ಬಾಹ್ಯ ಶೂನ್ಯ init_entity_runnable_ಸರಾಸರಿ_ಮೌಲ್ಯ(ರಚನೆ sched_entity *se);
ಬಾಹ್ಯ ಶೂನ್ಯ post_init_entity_util_avg(ರಚನೆ task_struct *p);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_NO_HZ_ತುಂಬಿದ
ಬಾಹ್ಯ bool sched_can_stop_tick(ರಚನೆ rq *rq);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ __init sched_tick_offload_init(ಶೂನ್ಯ);

/*
 * Tick may be needed by tasks in the runqueue depending on their policy and
 * requirements. If tick is needed, lets send the target an IPI to kick it out of
 * nohz mode if necessary.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ sched_update_tick_dependency(ರಚನೆ rq *rq)
{
	ಪೂರ್ಣಾಂಕ cpu = cpu_ದ(rq);

	ಆದರೆ (!tick_nohz_ತುಂಬಿದ_cpu(cpu))
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (sched_can_stop_tick(rq))
		tick_nohz_dep_clear_cpu(cpu, TICK_DEP_ಬಿಟ್_SCHED);
	ಇಲ್ಲದಿದ್ದರೆ
		tick_nohz_dep_ಗಣ_cpu(cpu, TICK_DEP_ಬಿಟ್_SCHED);
}
#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_NO_HZ_FULL: */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ sched_tick_offload_init(ಶೂನ್ಯ) { ಹಿಂತಿರುಗಿ 0; }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ sched_update_tick_dependency(ರಚನೆ rq *rq) { }
#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_NO_HZ_FULL */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ ಕೂಡಿಸು_nr_running(ರಚನೆ rq *rq, ಚಿಹ್ನೆರಹಿತ ಎಣಿಕೆ_2)
{
	ಚಿಹ್ನೆರಹಿತ prev_nr = rq->nr_running;

	rq->nr_running = prev_nr + ಎಣಿಕೆ_2;
	ಆದರೆ (trace_sched_update_nr_running_tp_enabled()) {
		call_trace_sched_update_nr_running(rq, ಎಣಿಕೆ_2);
	}

	ಆದರೆ (prev_nr < 2 && rq->nr_running >= 2)
		ಗಣ_rd_overloaded(rq->rd, 1);

	sched_update_tick_dependency(rq);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ sub_nr_running(ರಚನೆ rq *rq, ಚಿಹ್ನೆರಹಿತ ಎಣಿಕೆ_2)
{
	rq->nr_running -= ಎಣಿಕೆ_2;
	ಆದರೆ (trace_sched_update_nr_running_tp_enabled()) {
		call_trace_sched_update_nr_running(rq, -ಎಣಿಕೆ_2);
	}

	/* Check if we still need preemption */
	sched_update_tick_dependency(rq);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ __block_task(ರಚನೆ rq *rq, ರಚನೆ task_struct *p)
{
	ಆದರೆ (p->sched_contributes_ಗಮ್ಯ_ತುಂಬು)
		rq->nr_uninterruptible++;

	ಆದರೆ (p->ಒಳ_iowait) {
		atomic_inc(&rq->nr_iowait);
		delayacct_blkio_ಆರಂಭ();
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

ಬಾಹ್ಯ ಶೂನ್ಯ activate_task(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ flags);
ಬಾಹ್ಯ ಶೂನ್ಯ deactivate_task(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ flags);

ಬಾಹ್ಯ ಶೂನ್ಯ wakeup_preempt(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ flags);

/*
 * attach_task() -- attach the task detached by detach_task() to its new rq.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ attach_task(ರಚನೆ rq *rq, ರಚನೆ task_struct *p)
{
	lockdep_assert_rq_held(rq);

	WARN_ON_ONCE(task_rq(p) != rq);
	activate_task(rq, p, ಸರತಿಗೆ_ಸೇರಿಸು_NOCLOCK);
	wakeup_preempt(rq, p, 0);
}

/*
 * attach_one_task() -- attaches the task returned from detach_one_task() to
 * its new rq.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ attach_one_task(ರಚನೆ rq *rq, ರಚನೆ task_struct *p)
{
	guard(rq_ಬೀಗ)(rq);
	update_rq_clock(rq);
	attach_task(rq, p);
}

/*
 * __attach_tasks() - attaches a list of tasks (using se.group_node) to
 * the new rq
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ __attach_tasks(ರಚನೆ rq *rq, ರಚನೆ list_ತಲೆ *tasks)
{
	guard(rq_ಬೀಗ)(rq);
	update_rq_clock(rq);

	ಆಗ (!list_ಖಾಲಿಗಳು(tasks)) {
		ರಚನೆ task_struct *p;

		p = list_ಮೊದಲ_ನಮೂದು(tasks, ರಚನೆ task_struct, se.ಗುಂಪು_ಗಂಟು);
		list_del_init(&p->se.ಗುಂಪು_ಗಂಟು);

		attach_task(rq, p);
	}
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_PREEMPT_RT
# define SCHED_NR_MIGRATE_BREAK 8
#ಇಲ್ಲದಿದ್ದರೆ
# define SCHED_NR_MIGRATE_BREAK 32
#ಆದರೆ_ಅಂತ್ಯ

ಬಾಹ್ಯ __read_mostly ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ sysctl_sched_nr_migrate;
ಬಾಹ್ಯ __read_mostly ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ sysctl_sched_migration_ವೆಚ್ಚ;

ಬಾಹ್ಯ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ sysctl_sched_ತಳ_ತುಂಡು;

ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ sysctl_resched_latency_warn_ms;
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ sysctl_resched_latency_warn_once;

ಬಾಹ್ಯ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ sysctl_sched_tunable_scaling;

ಬಾಹ್ಯ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ sysctl_numa_balancing_ಪರಿಶೀಲನೆ_delay;
ಬಾಹ್ಯ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ sysctl_numa_balancing_ಪರಿಶೀಲನೆ_period_min;
ಬಾಹ್ಯ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ sysctl_numa_balancing_ಪರಿಶೀಲನೆ_period_max;
ಬಾಹ್ಯ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ sysctl_numa_balancing_ಪರಿಶೀಲನೆ_ಗಾತ್ರ;
ಬಾಹ್ಯ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ sysctl_numa_balancing_hot_ಮಿತಿ;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_HRTICK

/*
 * Use hrtick when:
 *  - enabled by features
 *  - hrtimer is actually high res
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool hrtick_enabled(ರಚನೆ rq *rq)
{
	ಹಿಂತಿರುಗಿ cpu_ಸಕ್ರಿಯ(cpu_ದ(rq)) && hrtimer_highres_enabled();
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool hrtick_enabled_fair(ರಚನೆ rq *rq)
{
	ಹಿಂತಿರುಗಿ sched_feat(HRTICK) && hrtick_enabled(rq);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool hrtick_enabled_dl(ರಚನೆ rq *rq)
{
	ಹಿಂತಿರುಗಿ sched_feat(HRTICK_DL) && hrtick_enabled(rq);
}

ಬಾಹ್ಯ ಶೂನ್ಯ hrtick_ಆರಂಭ(ರಚನೆ rq *rq, u64 delay);
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool hrtick_ಸಕ್ರಿಯ(ರಚನೆ rq *rq)
{
	ಹಿಂತಿರುಗಿ hrtimer_ಸಕ್ರಿಯ(&rq->hrtick_timer);
}

#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_SCHED_HRTICK: */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool hrtick_enabled_fair(ರಚನೆ rq *rq) { ಹಿಂತಿರುಗಿ false; }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool hrtick_enabled_dl(ರಚನೆ rq *rq) { ಹಿಂತಿರುಗಿ false; }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool hrtick_enabled(ರಚನೆ rq *rq) { ಹಿಂತಿರುಗಿ false; }
#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_SCHED_HRTICK */

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ arch_scale_freq_tick
ಸ್ಥಿರ __always_inline ಶೂನ್ಯ arch_scale_freq_tick(ಶೂನ್ಯ) { }
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ arch_scale_freq_ಸಾಮರ್ಥ್ಯ
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
ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ arch_scale_freq_ಸಾಮರ್ಥ್ಯ(ಪೂರ್ಣಾಂಕ cpu)
{
	ಹಿಂತಿರುಗಿ SCHED_ಸಾಮರ್ಥ್ಯ_SCALE;
}
#ಆದರೆ_ಅಂತ್ಯ

/*
 * In double_lock_balance()/double_rq_lock(), we use raw_spin_rq_lock() to
 * acquire rq lock instead of rq_lock(). So at the end of these two functions
 * we need to call double_rq_clock_clear_update() to clear RQCF_UPDATED of
 * rq->clock_update_flags to avoid the WARN_DOUBLE_CLOCK warning.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ double_rq_clock_clear_update(ರಚನೆ rq *rq1, ರಚನೆ rq *rq2)
{
	rq1->clock_update_flags &= (RQCF_REQ_SKIP|RQCF_ACT_SKIP);
	rq2->clock_update_flags &= (RQCF_REQ_SKIP|RQCF_ACT_SKIP);
}

#ವ್ಯಾಖ್ಯಾನಿಸು DEFINE_ಬೀಗ_GUARD_2(name, type, ಬೀಗ_2, _unlock, ...)				\
__DEFINE_UNLOCK_GUARD(name, type, _unlock, type *lock2; __VA_ARGS__)			\
ಸ್ಥಿರ ಸಾಲೊಳಗೆ class_##name##_t class_##name##_constructor(type *ಬೀಗ, type *lock2)	\
	__no_context_analysis								\
{ class_##name##_t _t = { .ಬೀಗ = ಬೀಗ, .lock2 = lock2 }, *_T = &_t;			\
  ಬೀಗ_2; ಹಿಂತಿರುಗಿ _t; }
#ವ್ಯಾಖ್ಯಾನಿಸು DECLARE_ಬೀಗ_GUARD_2_ATTRS(_name, ಬೀಗ_2, _unlock1, _unlock2)			\
ಸ್ಥಿರ ಸಾಲೊಳಗೆ class_##_name##_t class_##_name##_constructor(ಬೀಗ_3##_name##_t *_T1,	\
							    ಬೀಗ_3##_name##_t *_T2) ಬೀಗ_2; \
ಸ್ಥಿರ __always_inline ಶೂನ್ಯ __class_##_name##_cleanup_ctx1(class_##_name##_t **_T1)	\
	__no_context_analysis _unlock1 { }						\
ಸ್ಥಿರ __always_inline ಶೂನ್ಯ __class_##_name##_cleanup_ctx2(class_##_name##_t **_T2)	\
	__no_context_analysis _unlock2 { }
#ವ್ಯಾಖ್ಯಾನಿಸು WITH_ಬೀಗ_GUARD_2_ATTRS(_name, _T1, _T2)					\
	class_##_name##_constructor(_T1, _T2),						\
	*__UNIQUE_ID(unlock1) __cleanup(__class_##_name##_cleanup_ctx1) = (ಶೂನ್ಯ *)(_T1),\
	*__UNIQUE_ID(unlock2) __cleanup(__class_##_name##_cleanup_ctx2) = (ಶೂನ್ಯ *)(_T2)

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool rq_ಕ್ರಮ_less(ರಚನೆ rq *rq1, ರಚನೆ rq *rq2)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CORE
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
	ಆದರೆ (rq1->core->cpu < rq2->core->cpu)
		ಹಿಂತಿರುಗಿ true;
	ಆದರೆ (rq1->core->cpu > rq2->core->cpu)
		ಹಿಂತಿರುಗಿ false;

	/*
	 * __sched_core_flip() relies on SMT having cpu-id lock order.
	 */
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_SCHED_CORE */
	ಹಿಂತಿರುಗಿ rq1->cpu < rq2->cpu;
}

ಬಾಹ್ಯ ಶೂನ್ಯ double_rq_ಬೀಗ(ರಚನೆ rq *rq1, ರಚನೆ rq *rq2)
	__acquires(__rq_lockp(rq1), __rq_lockp(rq2));

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_PREEMPTION

/*
 * fair double_lock_balance: Safely acquires both rq->locks in a fair
 * way at the expense of forcing extra atomic operations in all
 * invocations.  This assures that the double_lock is acquired using the
 * same underlying policy as the spinlock_t on this architecture, which
 * reduces latency compared to the unfair variant below.  However, it
 * also adds more overhead and therefore may reduce throughput.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ double_ಬೀಗ_balance_2(ರಚನೆ rq *this_rq, ರಚನೆ rq *busiest)
	__must_hold(__rq_lockp(this_rq))
	__acquires(__rq_lockp(busiest))
{
	raw_spin_rq_unlock(this_rq);
	double_rq_ಬೀಗ(this_rq, busiest);

	ಹಿಂತಿರುಗಿ 1;
}

#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_PREEMPTION: */
/*
 * Unfair double_lock_balance: Optimizes throughput at the expense of
 * latency by eliminating extra atomic operations when the locks are
 * already in proper order on entry.  This favors lower CPU-ids and will
 * grant the double lock to lower CPUs over higher ids under contention,
 * regardless of entry order into the function.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ double_ಬೀಗ_balance_2(ರಚನೆ rq *this_rq, ರಚನೆ rq *busiest)
	__must_hold(__rq_lockp(this_rq))
	__acquires(__rq_lockp(busiest))
{
	ಆದರೆ (__rq_lockp(this_rq) == __rq_lockp(busiest)) {
		__acquire(__rq_lockp(busiest)); /* already held */
		double_rq_clock_clear_update(this_rq, busiest);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (likely(raw_spin_rq_trylock(busiest))) {
		double_rq_clock_clear_update(this_rq, busiest);
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (rq_ಕ್ರಮ_less(this_rq, busiest)) {
		raw_spin_rq_ಬೀಗ_nested(busiest, SINGLE_ಆಳ_NESTING);
		double_rq_clock_clear_update(this_rq, busiest);
		ಹಿಂತಿರುಗಿ 0;
	}

	raw_spin_rq_unlock(this_rq);
	double_rq_ಬೀಗ(this_rq, busiest);

	ಹಿಂತಿರುಗಿ 1;
}

#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_PREEMPTION */

/*
 * double_lock_balance - lock the busiest runqueue, this_rq is locked already.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ double_ಬೀಗ_balance(ರಚನೆ rq *this_rq, ರಚನೆ rq *busiest)
	__must_hold(__rq_lockp(this_rq))
	__acquires(__rq_lockp(busiest))
{
	lockdep_assert_irqs_disabled();

	ಹಿಂತಿರುಗಿ double_ಬೀಗ_balance_2(this_rq, busiest);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ double_unlock_balance(ರಚನೆ rq *this_rq, ರಚನೆ rq *busiest)
	__releases(__rq_lockp(busiest))
{
	ಆದರೆ (__rq_lockp(this_rq) != __rq_lockp(busiest))
		raw_spin_rq_unlock(busiest);
	ಇಲ್ಲದಿದ್ದರೆ
		__release(__rq_lockp(busiest)); /* fake release */
	ಬೀಗ_ಗಣ_subclass(&__rq_lockp(this_rq)->dep_map, 0, _RET_IP_);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ double_ಬೀಗ(spinlock_t *l1, spinlock_t *l2)
	__acquires(l1, l2)
{
	ಆದರೆ (l1 > l2)
		ಅದಲುಬದಲು(l1, l2);

	spin_ಬೀಗ(l1);
	spin_ಬೀಗ_nested(l2, SINGLE_ಆಳ_NESTING);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ double_ಬೀಗ_irq(spinlock_t *l1, spinlock_t *l2)
	__acquires(l1, l2)
{
	ಆದರೆ (l1 > l2)
		ಅದಲುಬದಲು(l1, l2);

	spin_ಬೀಗ_irq(l1);
	spin_ಬೀಗ_nested(l2, SINGLE_ಆಳ_NESTING);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ double_raw_ಬೀಗ(raw_spinlock_t *l1, raw_spinlock_t *l2)
	__acquires(l1, l2)
{
	ಆದರೆ (l1 > l2)
		ಅದಲುಬದಲು(l1, l2);

	raw_spin_ಬೀಗ(l1);
	raw_spin_ಬೀಗ_nested(l2, SINGLE_ಆಳ_NESTING);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ double_raw_unlock(raw_spinlock_t *l1, raw_spinlock_t *l2)
	__releases(l1, l2)
{
	raw_spin_unlock(l1);
	raw_spin_unlock(l2);
}

DEFINE_ಬೀಗ_GUARD_2(double_raw_spinlock, raw_spinlock_t,
		    double_raw_ಬೀಗ(_T->ಬೀಗ, _T->lock2),
		    double_raw_unlock(_T->ಬೀಗ, _T->lock2))

DECLARE_ಬೀಗ_GUARD_2_ATTRS(double_raw_spinlock,
			   __acquires(_T1, _T2),
			   __releases(*(raw_spinlock_t **)_T1),
			   __releases(*(raw_spinlock_t **)_T2));
#ವ್ಯಾಖ್ಯಾನಿಸು class_double_raw_spinlock_constructor(_T1, _T2) \
	WITH_ಬೀಗ_GUARD_2_ATTRS(double_raw_spinlock, _T1, _T2)

/*
 * double_rq_unlock - safely unlock two runqueues
 *
 * Note this does not restore interrupts like task_rq_unlock,
 * you need to do so manually after calling.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ double_rq_unlock(ರಚನೆ rq *rq1, ರಚನೆ rq *rq2)
	__releases(__rq_lockp(rq1), __rq_lockp(rq2))
{
	ಆದರೆ (__rq_lockp(rq1) != __rq_lockp(rq2))
		raw_spin_rq_unlock(rq2);
	ಇಲ್ಲದಿದ್ದರೆ
		__release(__rq_lockp(rq2)); /* fake release */
	raw_spin_rq_unlock(rq1);
}

ಬಾಹ್ಯ ಶೂನ್ಯ ಗಣ_rq_online (ರಚನೆ rq *rq);
ಬಾಹ್ಯ ಶೂನ್ಯ ಗಣ_rq_offline(ರಚನೆ rq *rq);

ಬಾಹ್ಯ bool sched_smp_initialized;

DEFINE_ಬೀಗ_GUARD_2(double_rq_ಬೀಗ, ರಚನೆ rq,
		    double_rq_ಬೀಗ(_T->ಬೀಗ, _T->lock2),
		    double_rq_unlock(_T->ಬೀಗ, _T->lock2))

ಬಾಹ್ಯ ರಚನೆ sched_entity *__pick_root_entity(ರಚನೆ cfs_rq *cfs_rq);
ಬಾಹ್ಯ ರಚನೆ sched_entity *__pick_first_entity(ರಚನೆ cfs_rq *cfs_rq);
ಬಾಹ್ಯ ರಚನೆ sched_entity *__pick_last_entity(ರಚನೆ cfs_rq *cfs_rq);

ಬಾಹ್ಯ bool sched_debug_verbose;

ಬಾಹ್ಯ ಶೂನ್ಯ print_cfs_stats(ರಚನೆ seq_file *m, ಪೂರ್ಣಾಂಕ cpu);
ಬಾಹ್ಯ ಶೂನ್ಯ print_rt_stats(ರಚನೆ seq_file *m, ಪೂರ್ಣಾಂಕ cpu);
ಬಾಹ್ಯ ಶೂನ್ಯ print_dl_stats(ರಚನೆ seq_file *m, ಪೂರ್ಣಾಂಕ cpu);
ಬಾಹ್ಯ ಶೂನ್ಯ print_cfs_rq(ರಚನೆ seq_file *m, ಪೂರ್ಣಾಂಕ cpu, ರಚನೆ cfs_rq *cfs_rq);
ಬಾಹ್ಯ ಶೂನ್ಯ print_rt_rq(ರಚನೆ seq_file *m, ಪೂರ್ಣಾಂಕ cpu, ರಚನೆ rt_rq *rt_rq);
ಬಾಹ್ಯ ಶೂನ್ಯ print_dl_rq(ರಚನೆ seq_file *m, ಪೂರ್ಣಾಂಕ cpu, ರಚನೆ dl_rq *dl_rq);

ಬಾಹ್ಯ ಶೂನ್ಯ resched_latency_warn(ಪೂರ್ಣಾಂಕ cpu, u64 latency);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_NUMA_BALANCING
ಬಾಹ್ಯ ಶೂನ್ಯ show_numa_stats(ರಚನೆ task_struct *p, ರಚನೆ seq_file *m);
ಬಾಹ್ಯ ಶೂನ್ಯ
print_numa_stats(ರಚನೆ seq_file *m, ಪೂರ್ಣಾಂಕ ಗಂಟು, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ tsf,
		 ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ tpf, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ gsf, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ gpf);
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_NUMA_BALANCING */

ಬಾಹ್ಯ ಶೂನ್ಯ init_cfs_rq(ರಚನೆ cfs_rq *cfs_rq);
ಬಾಹ್ಯ ಶೂನ್ಯ init_rt_rq(ರಚನೆ rt_rq *rt_rq);
ಬಾಹ್ಯ ಶೂನ್ಯ init_dl_rq(ರಚನೆ dl_rq *dl_rq);

ಬಾಹ್ಯ ಶೂನ್ಯ cfs_bandwidth_usage_inc(ಶೂನ್ಯ);
ಬಾಹ್ಯ ಶೂನ್ಯ cfs_bandwidth_usage_dec(ಶೂನ್ಯ);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_NO_HZ_COMMON

#ವ್ಯಾಖ್ಯಾನಿಸು NOHZ_BALANCE_KICK_ಬಿಟ್	0
#ವ್ಯಾಖ್ಯಾನಿಸು NOHZ_STATS_KICK_ಬಿಟ್	1
#ವ್ಯಾಖ್ಯಾನಿಸು NOHZ_NEWILB_KICK_ಬಿಟ್	2
#ವ್ಯಾಖ್ಯಾನಿಸು NOHZ_ಮುಂದಿನ_KICK_ಬಿಟ್	3

/* Run sched_balance_domains() */
#ವ್ಯಾಖ್ಯಾನಿಸು NOHZ_BALANCE_KICK	ಬಿಟ್(NOHZ_BALANCE_KICK_ಬಿಟ್)
/* Update blocked load */
#ವ್ಯಾಖ್ಯಾನಿಸು NOHZ_STATS_KICK		ಬಿಟ್(NOHZ_STATS_KICK_ಬಿಟ್)
/* Update blocked load when entering idle */
#ವ್ಯಾಖ್ಯಾನಿಸು NOHZ_NEWILB_KICK	ಬಿಟ್(NOHZ_NEWILB_KICK_ಬಿಟ್)
/* Update nohz.next_balance */
#ವ್ಯಾಖ್ಯಾನಿಸು NOHZ_ಮುಂದಿನ_KICK		ಬಿಟ್(NOHZ_ಮುಂದಿನ_KICK_ಬಿಟ್)

#ವ್ಯಾಖ್ಯಾನಿಸು NOHZ_KICK_ಮುಖವಾಡ		(NOHZ_BALANCE_KICK | NOHZ_STATS_KICK | NOHZ_ಮುಂದಿನ_KICK)

#ವ್ಯಾಖ್ಯಾನಿಸು nohz_flags(cpu)		(&cpu_rq(cpu)->nohz_flags)

ಬಾಹ್ಯ ಶೂನ್ಯ nohz_balance_exit_idle(ರಚನೆ rq *rq);
#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_NO_HZ_COMMON: */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ nohz_balance_exit_idle(ರಚನೆ rq *rq) { }
#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_NO_HZ_COMMON */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_NO_HZ_COMMON
ಬಾಹ್ಯ ಶೂನ್ಯ nohz_ಸತತ_idle_balance(ಪೂರ್ಣಾಂಕ cpu);
#ಇಲ್ಲದಿದ್ದರೆ
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ nohz_ಸತತ_idle_balance(ಪೂರ್ಣಾಂಕ cpu) { }
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "stats.h"

#ಆದರೆ defined(CONFIG_SCHED_CORE) && defined(CONFIG_SCHEDSTATS)

ಬಾಹ್ಯ ಶೂನ್ಯ __sched_core_account_forceidle(ರಚನೆ rq *rq);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ sched_core_account_forceidle(ರಚನೆ rq *rq)
{
	ಆದರೆ (schedstat_enabled())
		__sched_core_account_forceidle(rq);
}

ಬಾಹ್ಯ ಶೂನ್ಯ __sched_core_tick(ರಚನೆ rq *rq);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ sched_core_tick(ರಚನೆ rq *rq)
{
	ಆದರೆ (sched_core_enabled(rq) && schedstat_enabled())
		__sched_core_tick(rq);
}

#ಇಲ್ಲದಿದ್ದರೆ /* !(CONFIG_SCHED_CORE && CONFIG_SCHEDSTATS): */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ sched_core_account_forceidle(ರಚನೆ rq *rq) { }

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ sched_core_tick(ರಚನೆ rq *rq) { }

#ಆದರೆ_ಅಂತ್ಯ /* !(CONFIG_SCHED_CORE && CONFIG_SCHEDSTATS) */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_IRQ_ಸಮಯ_ACCOUNTING

ರಚನೆ irqtime {
	u64			ಒಟ್ಟು;
	u64			tick_delta;
	u64			irq_ಆರಂಭ_ಸಮಯ;
	ರಚನೆ u64_stats_sync	sync;
};

DECLARE_ಪ್ರತಿ_CPU(ರಚನೆ irqtime, cpu_irqtime);
DECLARE_STATIC_ಕೀಲಿ_FALSE(sched_clock_irqtime);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ irqtime_enabled(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ static_branch_likely(&sched_clock_irqtime);
}

/*
 * Returns the irqtime minus the softirq time computed by ksoftirqd.
 * Otherwise ksoftirqd's sum_exec_runtime is subtracted its own runtime
 * and never move forward.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ u64 irq_ಸಮಯ_read(ಪೂರ್ಣಾಂಕ cpu)
{
	ರಚನೆ irqtime *irqtime = &ಪ್ರತಿ_cpu(cpu_irqtime, cpu);
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ seq;
	u64 ಒಟ್ಟು;

	ಮಾಡು {
		seq = __u64_stats_fetch_begin(&irqtime->sync);
		ಒಟ್ಟು = irqtime->ಒಟ್ಟು;
	} ಆಗ (__u64_stats_fetch_retry(&irqtime->sync, seq));

	ಹಿಂತಿರುಗಿ ಒಟ್ಟು;
}

#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_IRQ_TIME_ACCOUNTING: */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ irqtime_enabled(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ 0;
}

#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_IRQ_TIME_ACCOUNTING */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_CPU_FREQ

DECLARE_ಪ್ರತಿ_CPU(ರಚನೆ update_util_ದತ್ತಾಂಶ __rcu *, cpufreq_update_util_ದತ್ತಾಂಶ);

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
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ cpufreq_update_util(ರಚನೆ rq *rq, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags)
{
	ರಚನೆ update_util_ದತ್ತಾಂಶ *ದತ್ತಾಂಶ;

	ದತ್ತಾಂಶ = rcu_dereference_sched(*ಪ್ರತಿ_cpu_ptr(&cpufreq_update_util_ದತ್ತಾಂಶ,
						  cpu_ದ(rq)));
	ಆದರೆ (ದತ್ತಾಂಶ)
		ದತ್ತಾಂಶ->func(ದತ್ತಾಂಶ, rq_clock(rq), flags);
}
#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_CPU_FREQ: */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ cpufreq_update_util(ರಚನೆ rq *rq, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags) { }
#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_CPU_FREQ */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ arch_scale_freq_ಸಾಮರ್ಥ್ಯ
# ifndef arch_scale_freq_invariant
#  define arch_scale_freq_invariant()	true
# endif
#ಇಲ್ಲದಿದ್ದರೆ
# define arch_scale_freq_invariant()	false
#ಆದರೆ_ಅಂತ್ಯ

ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ effective_cpu_util(ಪೂರ್ಣಾಂಕ cpu, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ util_cfs,
				 ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ *min,
				 ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ *max);

ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ sugov_effective_cpu_perf(ಪೂರ್ಣಾಂಕ cpu, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ actual,
				 ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ min,
				 ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ max);


/*
 * Verify the fitness of task @p to run on @cpu taking into account the
 * CPU original capacity and the runtime/deadline ratio of the task.
 *
 * The function will return true if the original capacity of @cpu is
 * greater than or equal to task's deadline density right shifted by
 * (BW_SHIFT - SCHED_CAPACITY_SHIFT) and false otherwise.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool dl_task_ಹೊಂದುತ್ತದೆ_ಸಾಮರ್ಥ್ಯ(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ cpu)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ cap = arch_scale_cpu_ಸಾಮರ್ಥ್ಯ(cpu);

	ಹಿಂತಿರುಗಿ cap >= p->dl.dl_density >> (BW_ಸರಿಸು - SCHED_ಸಾಮರ್ಥ್ಯ_ಸರಿಸು);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ cpu_bw_dl(ರಚನೆ rq *rq)
{
	ಹಿಂತಿರುಗಿ (rq->dl.running_bw * SCHED_ಸಾಮರ್ಥ್ಯ_SCALE) >> BW_ಸರಿಸು;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ cpu_util_dl(ರಚನೆ rq *rq)
{
	ಹಿಂತಿರುಗಿ READ_ONCE(rq->avg_dl.util_avg);
}


ಬಾಹ್ಯ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ cpu_util_cfs(ಪೂರ್ಣಾಂಕ cpu);
ಬಾಹ್ಯ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ cpu_util_cfs_boost(ಪೂರ್ಣಾಂಕ cpu);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ cpu_util_rt(ರಚನೆ rq *rq)
{
	ಹಿಂತಿರುಗಿ READ_ONCE(rq->avg_rt.util_avg);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_UCLAMP_TASK

ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ uclamp_eff_ಮೌಲ್ಯ(ರಚನೆ task_struct *p, ಎಣಿಕೆ uclamp_id clamp_id);

/*
 * When uclamp is compiled in, the aggregation at rq level is 'turned off'
 * by default in the fast path and only gets turned on once userspace performs
 * an operation that requires it.
 *
 * Returns true if userspace opted-in to use uclamp and aggregation at rq level
 * hence is active.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool uclamp_is_ಬಳಸಿದ(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ static_branch_likely(&sched_uclamp_ಬಳಸಿದ);
}

/*
 * Enabling static branches would get the cpus_read_lock(),
 * check whether uclamp_is_used before enable it to avoid always
 * calling cpus_read_lock(). Because we never disable this
 * static key once enable it.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ sched_uclamp_enable(ಶೂನ್ಯ)
{
	ಆದರೆ (!uclamp_is_ಬಳಸಿದ())
		static_branch_enable(&sched_uclamp_ಬಳಸಿದ);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ uclamp_rq_get(ರಚನೆ rq *rq,
					  ಎಣಿಕೆ uclamp_id clamp_id)
{
	ಹಿಂತಿರುಗಿ READ_ONCE(rq->uclamp[clamp_id].ಮೌಲ್ಯ);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ uclamp_rq_ಗಣ(ರಚನೆ rq *rq, ಎಣಿಕೆ uclamp_id clamp_id,
				 ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಮೌಲ್ಯ)
{
	WRITE_ONCE(rq->uclamp[clamp_id].ಮೌಲ್ಯ, ಮೌಲ್ಯ);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool uclamp_rq_is_idle(ರಚನೆ rq *rq)
{
	ಹಿಂತಿರುಗಿ rq->uclamp_flags & UCLAMP_FLAG_IDLE;
}

/* Is the rq being capped/throttled by uclamp_max? */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool uclamp_rq_is_capped(ರಚನೆ rq *rq)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ rq_util;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ max_util;

	ಆದರೆ (!uclamp_is_ಬಳಸಿದ())
		ಹಿಂತಿರುಗಿ false;

	rq_util = cpu_util_cfs(cpu_ದ(rq)) + cpu_util_rt(rq);
	max_util = READ_ONCE(rq->uclamp[UCLAMP_MAX].ಮೌಲ್ಯ);

	ಹಿಂತಿರುಗಿ max_util != SCHED_ಸಾಮರ್ಥ್ಯ_SCALE && rq_util >= max_util;
}

#ವ್ಯಾಖ್ಯಾನಿಸು for_each_clamp_id(clamp_id) \
	ಗಾಗಿ ((clamp_id) = 0; (clamp_id) < UCLAMP_CNT; (clamp_id)++)

ಬಾಹ್ಯ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ sysctl_sched_uclamp_util_min_rt_default;


ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ uclamp_none(ಎಣಿಕೆ uclamp_id clamp_id)
{
	ಆದರೆ (clamp_id == UCLAMP_MIN)
		ಹಿಂತಿರುಗಿ 0;
	ಹಿಂತಿರುಗಿ SCHED_ಸಾಮರ್ಥ್ಯ_SCALE;
}

/* Integer rounded range for each bucket */
#ವ್ಯಾಖ್ಯಾನಿಸು UCLAMP_ಪಾತ್ರೆ_DELTA DIV_ಸುತ್ತು_CLOSEST(SCHED_ಸಾಮರ್ಥ್ಯ_SCALE, UCLAMP_BUCKETS)

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ uclamp_ಪಾತ್ರೆ_id(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ clamp_ಮೌಲ್ಯ)
{
	ಹಿಂತಿರುಗಿ min_t(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ, clamp_ಮೌಲ್ಯ / UCLAMP_ಪಾತ್ರೆ_DELTA, UCLAMP_BUCKETS - 1);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
uclamp_se_ಗಣ(ರಚನೆ uclamp_se *uc_se, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಮೌಲ್ಯ, bool user_defined)
{
	uc_se->ಮೌಲ್ಯ = ಮೌಲ್ಯ;
	uc_se->ಪಾತ್ರೆ_id = uclamp_ಪಾತ್ರೆ_id(ಮೌಲ್ಯ);
	uc_se->user_defined = user_defined;
}

#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_UCLAMP_TASK: */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ
uclamp_eff_ಮೌಲ್ಯ(ರಚನೆ task_struct *p, ಎಣಿಕೆ uclamp_id clamp_id)
{
	ಆದರೆ (clamp_id == UCLAMP_MIN)
		ಹಿಂತಿರುಗಿ 0;

	ಹಿಂತಿರುಗಿ SCHED_ಸಾಮರ್ಥ್ಯ_SCALE;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool uclamp_rq_is_capped(ರಚನೆ rq *rq) { ಹಿಂತಿರುಗಿ false; }

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool uclamp_is_ಬಳಸಿದ(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ sched_uclamp_enable(ಶೂನ್ಯ) {}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ
uclamp_rq_get(ರಚನೆ rq *rq, ಎಣಿಕೆ uclamp_id clamp_id)
{
	ಆದರೆ (clamp_id == UCLAMP_MIN)
		ಹಿಂತಿರುಗಿ 0;

	ಹಿಂತಿರುಗಿ SCHED_ಸಾಮರ್ಥ್ಯ_SCALE;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
uclamp_rq_ಗಣ(ರಚನೆ rq *rq, ಎಣಿಕೆ uclamp_id clamp_id, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಮೌಲ್ಯ)
{
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool uclamp_rq_is_idle(ರಚನೆ rq *rq)
{
	ಹಿಂತಿರುಗಿ false;
}

#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_UCLAMP_TASK */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_HAVE_SCHED_AVG_IRQ

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ cpu_util_irq(ರಚನೆ rq *rq)
{
	ಹಿಂತಿರುಗಿ READ_ONCE(rq->avg_irq.util_avg);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ
ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ scale_irq_ಸಾಮರ್ಥ್ಯ(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ util, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ irq, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ max)
{
	util *= (max - irq);
	util /= max;

	ಹಿಂತಿರುಗಿ util;

}

#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_HAVE_SCHED_AVG_IRQ: */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ cpu_util_irq(ರಚನೆ rq *rq)
{
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ
ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ scale_irq_ಸಾಮರ್ಥ್ಯ(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ util, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ irq, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ max)
{
	ಹಿಂತಿರುಗಿ util;
}

#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_HAVE_SCHED_AVG_IRQ */

ಬಾಹ್ಯ ಶೂನ್ಯ __setparam_fair(ರಚನೆ task_struct *p, ಸ್ಥಿರಾಂಕ ರಚನೆ sched_attr *attr);

#ಆದರೆ defined(CONFIG_ENERGY_MODEL) && defined(CONFIG_CPU_FREQ_GOV_SCHEDUTIL)

#ವ್ಯಾಖ್ಯಾನಿಸು perf_domain_ಹರವು(pd) (ಗಮ್ಯ_cpumask(((pd)->em_pd->cpus)))

DECLARE_STATIC_ಕೀಲಿ_FALSE(sched_energy_present);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool sched_energy_enabled(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ static_branch_unlikely(&sched_energy_present);
}

#ಇಲ್ಲದಿದ್ದರೆ /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL): */

#ವ್ಯಾಖ್ಯಾನಿಸು perf_domain_ಹರವು(pd) ಖಾಲಿ

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool sched_energy_enabled(ಶೂನ್ಯ) { ಹಿಂತಿರುಗಿ false; }

#ಆದರೆ_ಅಂತ್ಯ /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL) */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_MEMBARRIER

/*
 * The scheduler provides memory barriers required by membarrier between:
 * - prior user-space memory accesses and store to rq->membarrier_state,
 * - store to rq->membarrier_state and following user-space memory accesses.
 * In the same way it provides those guarantees around store to rq->curr.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ membarrier_switch_mm(ರಚನೆ rq *rq,
					ರಚನೆ mm_struct *prev_mm,
					ರಚನೆ mm_struct *ಮುಂದಿನ_mm)
{
	ಪೂರ್ಣಾಂಕ membarrier_state;

	ಆದರೆ (prev_mm == ಮುಂದಿನ_mm)
		ಹಿಂತಿರುಗಿ;

	membarrier_state = atomic_read(&ಮುಂದಿನ_mm->membarrier_state);
	ಆದರೆ (READ_ONCE(rq->membarrier_state) == membarrier_state)
		ಹಿಂತಿರುಗಿ;

	WRITE_ONCE(rq->membarrier_state, membarrier_state);
}

#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_MEMBARRIER: */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ membarrier_switch_mm(ರಚನೆ rq *rq,
					ರಚನೆ mm_struct *prev_mm,
					ರಚನೆ mm_struct *ಮುಂದಿನ_mm)
{
}

#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_MEMBARRIER */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool is_ಪ್ರತಿ_cpu_kthread(ರಚನೆ task_struct *p)
{
	ಆದರೆ (!(p->flags & PF_KTHREAD))
		ಹಿಂತಿರುಗಿ false;

	ಆದರೆ (p->nr_cpus_allowed != 1)
		ಹಿಂತಿರುಗಿ false;

	ಹಿಂತಿರುಗಿ true;
}

ಬಾಹ್ಯ ಶೂನ್ಯ swake_ಮೇಲೆ_all_locked(ರಚನೆ swait_ಸರತಿ_ತಲೆ *q);
ಬಾಹ್ಯ ಶೂನ್ಯ __prepare_to_swait(ರಚನೆ swait_ಸರತಿ_ತಲೆ *q, ರಚನೆ swait_ಸರತಿ *wait);

ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ try_ಗಮ್ಯ_wake_ಮೇಲೆ(ರಚನೆ task_struct *tsk, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ state, ಪೂರ್ಣಾಂಕ wake_flags);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_PREEMPT_DYNAMIC
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ preempt_dynamic_ಬಹುಲಕ;
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ sched_dynamic_ಬಹುಲಕ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str);
ಬಾಹ್ಯ ಶೂನ್ಯ sched_dynamic_update(ಪೂರ್ಣಾಂಕ ಬಹುಲಕ);
#ಆದರೆ_ಅಂತ್ಯ
ಬಾಹ್ಯ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *preempt_modes[];

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_MM_CID

ಸ್ಥಿರ __always_inline bool cid_on_cpu(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cid)
{
	ಹಿಂತಿರುಗಿ cid & MM_CID_ONCPU;
}

ಸ್ಥಿರ __always_inline bool cid_ಒಳ_transit(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cid)
{
	ಹಿಂತಿರುಗಿ cid & MM_CID_TRANSIT;
}

ಸ್ಥಿರ __always_inline ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cpu_cid_ಗಮ್ಯ_cid(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cid)
{
	ಹಿಂತಿರುಗಿ cid & ~MM_CID_ONCPU;
}

ಸ್ಥಿರ __always_inline ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cid_ಗಮ್ಯ_cpu_cid(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cid)
{
	ಹಿಂತಿರುಗಿ cid | MM_CID_ONCPU;
}

ಸ್ಥಿರ __always_inline ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cid_ಗಮ್ಯ_transit_cid(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cid)
{
	ಹಿಂತಿರುಗಿ cid | MM_CID_TRANSIT;
}

ಸ್ಥಿರ __always_inline ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cid_from_transit_cid(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cid)
{
	ಹಿಂತಿರುಗಿ cid & ~MM_CID_TRANSIT;
}

ಸ್ಥಿರ __always_inline bool cid_on_task(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cid)
{
	/* True if none of the MM_CID_ONCPU, MM_CID_TRANSIT, MM_CID_UNSET bits is set */
	ಹಿಂತಿರುಗಿ cid < MM_CID_TRANSIT;
}

ಸ್ಥಿರ __always_inline ಶೂನ್ಯ mm_drop_cid(ರಚನೆ mm_struct *mm, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cid)
{
	clear_ಬಿಟ್(cid, mm_cidmask(mm));
}

ಸ್ಥಿರ __always_inline ಶೂನ್ಯ mm_unset_cid_on_task(ರಚನೆ task_struct *t)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cid = t->mm_cid.cid;

	t->mm_cid.cid = MM_CID_UNSET;
	ಆದರೆ (cid_on_task(cid))
		mm_drop_cid(t->mm, cid);
}

ಸ್ಥಿರ __always_inline ಶೂನ್ಯ mm_drop_cid_on_cpu(ರಚನೆ mm_struct *mm, ರಚನೆ mm_cid_pcpu *pcp)
{
	/* Clear the ONCPU bit, but do not set UNSET in the per CPU storage */
	ಆದರೆ (cid_on_cpu(pcp->cid)) {
		pcp->cid = cpu_cid_ಗಮ್ಯ_cid(pcp->cid);
		mm_drop_cid(mm, pcp->cid);
	}
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ __mm_get_cid(ರಚನೆ mm_struct *mm, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ max_cids)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cid = ಕಂಡುಹಿಡಿ_ಮೊದಲ_zero_ಬಿಟ್(mm_cidmask(mm), max_cids);

	ಆದರೆ (cid >= max_cids)
		ಹಿಂತಿರುಗಿ MM_CID_UNSET;
	ಆದರೆ (test_and_ಗಣ_ಬಿಟ್(cid, mm_cidmask(mm)))
		ಹಿಂತಿರುಗಿ MM_CID_UNSET;
	ಹಿಂತಿರುಗಿ cid;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ mm_get_cid(ರಚನೆ mm_struct *mm)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cid = __mm_get_cid(mm, READ_ONCE(mm->mm_cid.max_cids));

	ಆಗ (cid == MM_CID_UNSET) {
		cpu_relax();
		cid = __mm_get_cid(mm, ಅಂಕೆ_possible_cpus());
	}
	ಹಿಂತಿರುಗಿ cid;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ mm_cid_converge(ರಚನೆ mm_struct *mm, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ orig_cid,
					   ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ max_cids)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ new_cid, cid = cpu_cid_ಗಮ್ಯ_cid(orig_cid);

	/* Is it in the optimal CID space? */
	ಆದರೆ (likely(cid < max_cids))
		ಹಿಂತಿರುಗಿ orig_cid;

	/* Try to find one in the optimal space. Otherwise keep the provided. */
	new_cid = __mm_get_cid(mm, max_cids);
	ಆದರೆ (new_cid != MM_CID_UNSET) {
		mm_drop_cid(mm, cid);
		/* Preserve the ONCPU mode of the original CID */
		ಹಿಂತಿರುಗಿ new_cid | (orig_cid & MM_CID_ONCPU);
	}
	ಹಿಂತಿರುಗಿ orig_cid;
}

ಸ್ಥಿರ __always_inline ಶೂನ್ಯ mm_cid_update_task_cid(ರಚನೆ task_struct *t, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cid)
{
	ಆದರೆ (t->mm_cid.cid != cid) {
		t->mm_cid.cid = cid;
		rseq_sched_ಗಣ_ids_ಬದಲಾಯಿತು(t);
	}
}

ಸ್ಥಿರ __always_inline ಶೂನ್ಯ mm_cid_update_pcpu_cid(ರಚನೆ mm_struct *mm, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cid)
{
	__this_cpu_write(mm->mm_cid.pcpu->cid, cid);
}

ಸ್ಥಿರ __always_inline ಶೂನ್ಯ mm_cid_from_cpu(ರಚನೆ task_struct *t, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cpu_cid,
					    ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಬಹುಲಕ)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ max_cids, tcid = t->mm_cid.cid;
	ರಚನೆ mm_struct *mm = t->mm;

	max_cids = READ_ONCE(mm->mm_cid.max_cids);
	/* Optimize for the common case where both have the ONCPU bit set */
	ಆದರೆ (likely(cid_on_cpu(cpu_cid & tcid))) {
		ಆದರೆ (likely(cpu_cid_ಗಮ್ಯ_cid(cpu_cid) < max_cids)) {
			mm_cid_update_task_cid(t, cpu_cid);
			ಹಿಂತಿರುಗಿ;
		}
		/* Try to converge into the optimal CID space */
		cpu_cid = mm_cid_converge(mm, cpu_cid, max_cids);
	} ಇಲ್ಲದಿದ್ದರೆ {
		/* Hand over or drop the task owned CID */
		ಆದರೆ (cid_on_task(tcid)) {
			ಆದರೆ (cid_on_cpu(cpu_cid))
				mm_unset_cid_on_task(t);
			ಇಲ್ಲದಿದ್ದರೆ
				cpu_cid = cid_ಗಮ್ಯ_cpu_cid(tcid);
		}
		/* Still nothing, allocate a new one */
		ಆದರೆ (!cid_on_cpu(cpu_cid))
			cpu_cid = cid_ಗಮ್ಯ_cpu_cid(mm_get_cid(mm));

		/* Handle the transition mode flag if required */
		ಆದರೆ (ಬಹುಲಕ & MM_CID_TRANSIT)
			cpu_cid = cpu_cid_ಗಮ್ಯ_cid(cpu_cid) | MM_CID_TRANSIT;
	}
	mm_cid_update_pcpu_cid(mm, cpu_cid);
	mm_cid_update_task_cid(t, cpu_cid);
}

ಸ್ಥಿರ __always_inline ಶೂನ್ಯ mm_cid_from_task(ರಚನೆ task_struct *t, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cpu_cid,
					     ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಬಹುಲಕ)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ max_cids, tcid = t->mm_cid.cid;
	ರಚನೆ mm_struct *mm = t->mm;

	max_cids = READ_ONCE(mm->mm_cid.max_cids);
	/* Optimize for the common case, where both have the ONCPU bit clear */
	ಆದರೆ (likely(cid_on_task(tcid | cpu_cid))) {
		ಆದರೆ (likely(tcid < max_cids)) {
			mm_cid_update_pcpu_cid(mm, tcid);
			ಹಿಂತಿರುಗಿ;
		}
		/* Try to converge into the optimal CID space */
		tcid = mm_cid_converge(mm, tcid, max_cids);
	} ಇಲ್ಲದಿದ್ದರೆ {
		/* Hand over or drop the CPU owned CID */
		ಆದರೆ (cid_on_cpu(cpu_cid)) {
			ಆದರೆ (cid_on_task(tcid))
				mm_drop_cid_on_cpu(mm, this_cpu_ptr(mm->mm_cid.pcpu));
			ಇಲ್ಲದಿದ್ದರೆ
				tcid = cpu_cid_ಗಮ್ಯ_cid(cpu_cid);
		}
		/* Still nothing, allocate a new one */
		ಆದರೆ (!cid_on_task(tcid))
			tcid = mm_get_cid(mm);
		/* Set the transition mode flag if required */
		tcid |= ಬಹುಲಕ & MM_CID_TRANSIT;
	}
	mm_cid_update_pcpu_cid(mm, tcid);
	mm_cid_update_task_cid(t, tcid);
}

ಸ್ಥಿರ __always_inline ಶೂನ್ಯ mm_cid_schedin(ರಚನೆ task_struct *ಮುಂದಿನ)
{
	ರಚನೆ mm_struct *mm = ಮುಂದಿನ->mm;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cpu_cid, ಬಹುಲಕ;

	ಆದರೆ (!ಮುಂದಿನ->mm_cid.ಸಕ್ರಿಯ)
		ಹಿಂತಿರುಗಿ;

	cpu_cid = __this_cpu_read(mm->mm_cid.pcpu->cid);
	ಬಹುಲಕ = READ_ONCE(mm->mm_cid.ಬಹುಲಕ);
	ಆದರೆ (likely(!cid_on_cpu(ಬಹುಲಕ)))
		mm_cid_from_task(ಮುಂದಿನ, cpu_cid, ಬಹುಲಕ);
	ಇಲ್ಲದಿದ್ದರೆ
		mm_cid_from_cpu(ಮುಂದಿನ, cpu_cid, ಬಹುಲಕ);
}

ಸ್ಥಿರ __always_inline ಶೂನ್ಯ mm_cid_schedout(ರಚನೆ task_struct *prev)
{
	ರಚನೆ mm_struct *mm = prev->mm;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಬಹುಲಕ, cid;

	/* During mode transitions CIDs are temporary and need to be dropped */
	ಆದರೆ (likely(!cid_ಒಳ_transit(prev->mm_cid.cid)))
		ಹಿಂತಿರುಗಿ;

	ಬಹುಲಕ = READ_ONCE(mm->mm_cid.ಬಹುಲಕ);
	cid = cid_from_transit_cid(prev->mm_cid.cid);

	/*
	 * If transition mode is done, transfer ownership when the CID is
	 * within the convergence range to optimize the next schedule in.
	 */
	ಆದರೆ (!cid_ಒಳ_transit(ಬಹುಲಕ) && cid < READ_ONCE(mm->mm_cid.max_cids)) {
		ಆದರೆ (cid_on_cpu(ಬಹುಲಕ))
			cid = cid_ಗಮ್ಯ_cpu_cid(cid);

		/* Update both so that the next schedule in goes into the fast path */
		mm_cid_update_pcpu_cid(mm, cid);
		prev->mm_cid.cid = cid;
	} ಇಲ್ಲದಿದ್ದರೆ {
		mm_drop_cid(mm, cid);
		prev->mm_cid.cid = MM_CID_UNSET;
	}
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ mm_cid_switch_ಗಮ್ಯ(ರಚನೆ task_struct *prev, ರಚನೆ task_struct *ಮುಂದಿನ)
{
	mm_cid_schedout(prev);
	mm_cid_schedin(ಮುಂದಿನ);
}

#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_SCHED_MM_CID: */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ mm_cid_switch_ಗಮ್ಯ(ರಚನೆ task_struct *prev, ರಚನೆ task_struct *ಮುಂದಿನ) { }
#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_SCHED_MM_CID */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CACHE
DECLARE_STATIC_ಕೀಲಿ_FALSE(sched_cache_present);
DECLARE_STATIC_ಕೀಲಿ_FALSE(sched_cache_ಸಕ್ರಿಯ);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ sysctl_sched_cache_user;
ಬಾಹ್ಯ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ llc_aggr_tolerance;
ಬಾಹ್ಯ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ llc_epoch_period;
ಬಾಹ್ಯ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ llc_epoch_affinity_timeout;
ಬಾಹ್ಯ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ llc_imb_pct;
ಬಾಹ್ಯ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ llc_overaggr_pct;

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool sched_cache_enabled(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ static_branch_unlikely(&sched_cache_ಸಕ್ರಿಯ);
}

ಬಾಹ್ಯ ಶೂನ್ಯ sched_cache_ಸಕ್ರಿಯ_ಗಣ(ಶೂನ್ಯ);

#ಆದರೆ_ಅಂತ್ಯ

ಶೂನ್ಯ sched_domains_free_llc_id(ಪೂರ್ಣಾಂಕ cpu);

ಬಾಹ್ಯ ಶೂನ್ಯ init_sched_mm(ರಚನೆ task_struct *p);

ಬಾಹ್ಯ u64 avg_vruntime(ರಚನೆ cfs_rq *cfs_rq);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ entity_eligible(ರಚನೆ cfs_rq *cfs_rq, ರಚನೆ sched_entity *se);
ಸ್ಥಿರ ಸಾಲೊಳಗೆ
ಶೂನ್ಯ move_queued_task_locked(ರಚನೆ rq *src_rq, ರಚನೆ rq *dst_rq, ರಚನೆ task_struct *task)
{
	lockdep_assert_rq_held(src_rq);
	lockdep_assert_rq_held(dst_rq);

	deactivate_task(src_rq, task, 0);
	ಗಣ_task_cpu(task, dst_rq->cpu);
	activate_task(dst_rq, task, 0);
	wakeup_preempt(dst_rq, task, 0);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ
bool task_is_pushable(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ cpu)
{
	ಆದರೆ (!task_on_cpu(rq, p) &&
	    cpumask_test_cpu(cpu, &p->cpus_ಮುಖವಾಡ))
		ಹಿಂತಿರುಗಿ true;

	ಹಿಂತಿರುಗಿ false;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_RT_MUTEXES

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ __rt_effective_prio(ರಚನೆ task_struct *pi_task, ಪೂರ್ಣಾಂಕ prio)
{
	ಆದರೆ (pi_task)
		prio = min(prio, pi_task->prio);

	ಹಿಂತಿರುಗಿ prio;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ rt_effective_prio(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ prio)
{
	ರಚನೆ task_struct *pi_task = rt_mutex_get_ತುದಿ_task(p);

	ಹಿಂತಿರುಗಿ __rt_effective_prio(pi_task, prio);
}

#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_RT_MUTEXES: */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ rt_effective_prio(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ prio)
{
	ಹಿಂತಿರುಗಿ prio;
}

#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_RT_MUTEXES */

ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ __sched_setscheduler(ರಚನೆ task_struct *p, ಸ್ಥಿರಾಂಕ ರಚನೆ sched_attr *attr, bool user, bool pi);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ __sched_setaffinity(ರಚನೆ task_struct *p, ರಚನೆ affinity_context *ctx);
ಬಾಹ್ಯ ಸ್ಥಿರಾಂಕ ರಚನೆ sched_class *__setscheduler_class(ಪೂರ್ಣಾಂಕ policy, ಪೂರ್ಣಾಂಕ prio);
ಬಾಹ್ಯ ಶೂನ್ಯ ಗಣ_ತುಂಬು_ತೂಕ(ರಚನೆ task_struct *p, bool update_ತುಂಬು);
ಬಾಹ್ಯ ಶೂನ್ಯ ಸರತಿಗೆ_ಸೇರಿಸು_task(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ flags);
ಬಾಹ್ಯ bool ಸರತಿಯಿಂದ_ತೆಗೆ_task(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ flags);

ಬಾಹ್ಯ ರಚನೆ balance_callback *splice_balance_callbacks(ರಚನೆ rq *rq);

ಬಾಹ್ಯ ಶೂನ್ಯ __balance_callbacks(ರಚನೆ rq *rq, ರಚನೆ rq_flags *rf);
ಬಾಹ್ಯ ಶೂನ್ಯ balance_callbacks(ರಚನೆ rq *rq, ರಚನೆ balance_callback *ತಲೆ);

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
ರಚನೆ sched_change_ctx {
	u64			prio;
	ರಚನೆ task_struct	*p;
	ಸ್ಥಿರಾಂಕ ರಚನೆ sched_class *class;
	ಪೂರ್ಣಾಂಕ			flags;
	bool			queued;
	bool			running;
};

ರಚನೆ sched_change_ctx *sched_change_begin(ರಚನೆ task_struct *p, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags);
ಶೂನ್ಯ sched_change_ಕೊನೆ(ರಚನೆ sched_change_ctx *ctx);

DEFINE_CLASS(sched_change, ರಚನೆ sched_change_ctx *,
	     sched_change_ಕೊನೆ(_T),
	     sched_change_begin(p, flags),
	     ರಚನೆ task_struct *p, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags)

DEFINE_CLASS_IS_UNCONDITIONAL(sched_change)

#ಅನ್ತರ್ಭಾವ "ext/ext.h"

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_PREFERRED_CPU
ಶೂನ್ಯ sched_ತಳ್ಳು_current_non_preferred_cpu(ರಚನೆ rq *rq);
#ಇಲ್ಲದಿದ್ದರೆ	/* !CONFIG_PREFERRED_CPU */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ sched_ತಳ್ಳು_current_non_preferred_cpu(ರಚನೆ rq *rq) { }
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ /* _KERNEL_SCHED_SCHED_H */
