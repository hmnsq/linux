/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Scheduler internal types and methods:
 */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _KERNEL_SCHED_SCHED_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _KERNEL_SCHED_SCHED_H

#ਅਨ੍ਤਰ੍ਭਾਵ <linux/prandom.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/affinity.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/autogroup.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/cpufreq.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/deadline.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/loadavg.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/mm.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/rseq_api.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/signal.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/smt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/stat.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/sysctl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/task_flags.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/task.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/topology.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/atomic.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/bitmap.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/bug.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/capability.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/cgroup_api.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/cgroup.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/context_tracking.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/cpufreq.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/cpumask_api.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/cpuset.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/ctype.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/file.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/fs_api.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/hrtimer_api.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/interrupt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/irq_work.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/jiffies.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/kref_api.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/kthread.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/ktime_api.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/lockdep_api.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/lockdep.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/memblock.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/memcontrol.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/minmax.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/mm.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/module.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/mutex_api.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/plist.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/poll.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/proc_fs.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/profile.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/psi.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/rcupdate.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/seq_file.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/seqlock.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/softirq.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/spinlock_api.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/static_key.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/stop_machine.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/syscalls_api.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/syscalls.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/tick.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/topology.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/u64_stats_sync_api.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/uaccess.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/vmstat.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/wait_api.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/wait_bit.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/workqueue_api.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/delayacct.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/mmu_context.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <trace/events/power.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <trace/events/sched.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "../workqueue_internal.h"

ਬਣਤਰ rq;
ਬਣਤਰ cfs_rq;
ਬਣਤਰ rt_rq;
ਬਣਤਰ sched_ਸਮੂਹ;
ਬਣਤਰ cpuidle_state;

#ਜੇ defined(CONFIG_PARAVIRT) && !defined(CONFIG_HAVE_PV_STEAL_CLOCK_GEN)
# include <asm/paravirt.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ <asm/barrier.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "cpupri.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cpudeadline.h"

/* task_struct::on_rq states: */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TASK_ON_RQ_QUEUED	1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TASK_ON_RQ_MIGRATING	2

ਬਾਹਰੀ __read_mostly ਪੂਰਨ scheduler_running;

ਬਾਹਰੀ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ calc_ਲੱਦੋ_update;
ਬਾਹਰੀ atomic_long_t calc_ਲੱਦੋ_tasks;

ਬਾਹਰੀ ਖਾਲੀ calc_global_ਲੱਦੋ_tick(ਬਣਤਰ rq *this_rq);
ਬਾਹਰੀ ਲੰਮਾ calc_ਲੱਦੋ_fold_ਸਰਗਰਮ(ਬਣਤਰ rq *this_rq, ਲੰਮਾ adjust);

ਬਾਹਰੀ ਖਾਲੀ call_trace_sched_update_nr_running(ਬਣਤਰ rq *rq, ਪੂਰਨ ਗਿਣਤੀ_2);

ਬਾਹਰੀ ਪੂਰਨ sysctl_sched_rt_period;
ਬਾਹਰੀ ਪੂਰਨ sysctl_sched_rt_runtime;
ਬਾਹਰੀ ਪੂਰਨ sched_rr_timeslice;

/*
 * Asymmetric CPU capacity bits
 */
ਬਣਤਰ asym_cap_ਅੰਕੜੇ {
	ਬਣਤਰ list_ਸਿਰ link;
	ਬਣਤਰ rcu_ਸਿਰ rcu;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਸਮਰੱਥਾ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ cpus[];
};

ਬਾਹਰੀ ਬਣਤਰ list_ਸਿਰ asym_cap_list;

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cpu_ਸਮਰੱਥਾ_ਵਿਸਤਾਰ(asym_ਅੰਕੜੇ) ਮੰਜ਼ਿਲ_cpumask((asym_ਅੰਕੜੇ)->cpus)

/*
 * Helpers for converting nanosecond timing to jiffy resolution
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NS_ਮੰਜ਼ਿਲ_JIFFIES(ਸਮਾਂ)	((ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ)(ਸਮਾਂ) / (NSEC_ਪ੍ਰਤੀ_SEC/HZ))

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
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_64_ਦੋਆਧਾਰੀ_ਅੰਕ
# define NICE_0_ਲੱਦੋ_ਖਿਸਕਾਓ	(SCHED_FIXEDPOINT_ਖਿਸਕਾਓ + SCHED_FIXEDPOINT_ਖਿਸਕਾਓ)
# define scale_ਲੱਦੋ(w)		((w) << SCHED_FIXEDPOINT_ਖਿਸਕਾਓ)
# define scale_ਲੱਦੋ_ਥੱਲੇ(w)					\
({								\
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ __w = (w);				\
								\
	ਜੇ (__w)						\
		__w = max(2UL, __w >> SCHED_FIXEDPOINT_ਖਿਸਕਾਓ);	\
	__w;							\
})
#ਨਹੀਂਤਾਂ
# define NICE_0_ਲੱਦੋ_ਖਿਸਕਾਓ	(SCHED_FIXEDPOINT_ਖਿਸਕਾਓ)
# define scale_ਲੱਦੋ(w)		(w)
# define scale_ਲੱਦੋ_ਥੱਲੇ(w)	(w)
#ਜੇ_ਅੰਤ

/*
 * Task weight (visible to users) and its load (invisible to users) have
 * independent resolution, but they should be well calibrated. We use
 * scale_load() and scale_load_down(w) to convert between them. The
 * following must be true:
 *
 *  scale_load(sched_prio_to_weight[NICE_TO_PRIO(0)-MAX_RT_PRIO]) == NICE_0_LOAD
 *
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NICE_0_ਲੱਦੋ		(1L << NICE_0_ਲੱਦੋ_ਖਿਸਕਾਓ)

/*
 * Single value that decides SCHED_DEADLINE internal math precision.
 * 10 -> just above 1us
 * 9  -> just above 0.5us
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DL_SCALE		10

/*
 * Single value that denotes runtime == period, ie unlimited time.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ RUNTIME_INF		((u64)~0ULL)

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ idle_policy(ਪੂਰਨ policy)
{
	ਵਾਪਸ policy == SCHED_IDLE;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ ਸਾਧਾਰਨ_policy(ਪੂਰਨ policy)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CLASS_EXT
	ਜੇ (policy == SCHED_EXT)
		ਵਾਪਸ true;
#ਜੇ_ਅੰਤ
	ਵਾਪਸ policy == SCHED_ਸਾਧਾਰਨ;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ fair_policy(ਪੂਰਨ policy)
{
	ਵਾਪਸ ਸਾਧਾਰਨ_policy(policy) || policy == SCHED_BATCH;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ rt_policy(ਪੂਰਨ policy)
{
	ਵਾਪਸ policy == SCHED_FIFO || policy == SCHED_RR;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ dl_policy(ਪੂਰਨ policy)
{
	ਵਾਪਸ policy == SCHED_DEADLINE;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool valid_policy(ਪੂਰਨ policy)
{
	ਵਾਪਸ idle_policy(policy) || fair_policy(policy) ||
		rt_policy(policy) || dl_policy(policy);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ task_has_idle_policy(ਬਣਤਰ task_struct *p)
{
	ਵਾਪਸ idle_policy(p->policy);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ task_has_rt_policy(ਬਣਤਰ task_struct *p)
{
	ਵਾਪਸ rt_policy(p->policy);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ task_has_dl_policy(ਬਣਤਰ task_struct *p)
{
	ਵਾਪਸ dl_policy(p->policy);
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cap_scale(v, s)		((v)*(s) >> SCHED_ਸਮਰੱਥਾ_ਖਿਸਕਾਓ)

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ update_avg(u64 *avg, u64 sample)
{
	s64 diff = sample - *avg;

	*avg += diff / 8;
}

/*
 * Shifting a value by an exponent greater *or equal* to the size of said value
 * is UB; cap at size-1.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ shr_bound(val, ਖਿਸਕਾਓ)							\
	(val >> min_t(typeof(ਖਿਸਕਾਓ), ਖਿਸਕਾਓ, ਬਿੱਟ_ਪ੍ਰਤੀ_TYPE(typeof(val)) - 1))

/*
 * cgroup weight knobs should use the common MIN, DFL and MAX values which are
 * 1, 100 and 10000 respectively. While it loses a bit of range on both ends, it
 * maps pretty well onto the shares value used by scheduler and the round-trip
 * conversions preserve the original value over the entire range.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ sched_ਭਾਰ_from_cgroup(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ cgrp_ਭਾਰ)
{
	ਵਾਪਸ DIV_ਗੇੜਾ_CLOSEST_ULL(cgrp_ਭਾਰ * 1024, CGROUP_ਭਾਰ_DFL);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ sched_ਭਾਰ_ਮੰਜ਼ਿਲ_cgroup(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਭਾਰ)
{
	ਵਾਪਸ clamp_t(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ,
		       DIV_ਗੇੜਾ_CLOSEST_ULL(ਭਾਰ * CGROUP_ਭਾਰ_DFL, 1024),
		       CGROUP_ਭਾਰ_MIN, CGROUP_ਭਾਰ_MAX);
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
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SCHED_FLAG_SUGOV	0x10000000

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SCHED_DL_FLAGS		(SCHED_FLAG_RECLAIM | SCHED_FLAG_DL_OVERRUN | SCHED_FLAG_SUGOV)

ਸਥਿਰ ਸਤਰਅੰਦਰ bool dl_entity_is_special(ਅਚਲ ਬਣਤਰ sched_dl_entity *dl_se)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_CPU_FREQ_GOV_SCHEDUTIL
	ਵਾਪਸ unlikely(dl_se->flags & SCHED_FLAG_SUGOV);
#ਨਹੀਂਤਾਂ
	ਵਾਪਸ false;
#ਜੇ_ਅੰਤ
}

/*
 * Tells if entity @a should preempt entity @b.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ bool dl_entity_preempt(ਅਚਲ ਬਣਤਰ sched_dl_entity *a,
				     ਅਚਲ ਬਣਤਰ sched_dl_entity *b)
{
	ਵਾਪਸ dl_entity_is_special(a) ||
	       dl_ਸਮਾਂ_before(a->deadline, b->deadline);
}

/*
 * This is the priority-queue data structure of the RT scheduling class:
 */
ਬਣਤਰ rt_prio_array {
	DECLARE_BITMAP(bitmap, MAX_RT_PRIO+1); /* include 1 bit for delimiter */
	ਬਣਤਰ list_ਸਿਰ ਕਤਾਰ[MAX_RT_PRIO];
};

ਬਣਤਰ rt_bandwidth {
	/* nests inside the rq lock: */
	raw_spinlock_t		rt_runtime_ਤਾਲਾ;
	ktime_t			rt_period;
	u64			rt_runtime;
	ਬਣਤਰ hrtimer		rt_period_timer;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		rt_period_ਸਰਗਰਮ;
};

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ dl_bandwidth_enabled(ਖਾਲੀ)
{
	ਵਾਪਸ sysctl_sched_rt_runtime >= 0;
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
ਬਣਤਰ dl_bw {
	raw_spinlock_t		ਤਾਲਾ;
	u64			bw;
	u64			ਕੁੱਲ_bw;
};

ਬਾਹਰੀ ਖਾਲੀ init_dl_bw(ਬਣਤਰ dl_bw *dl_b);
ਬਾਹਰੀ ਪੂਰਨ  sched_dl_global_validate(ਖਾਲੀ);
ਬਾਹਰੀ ਖਾਲੀ sched_dl_do_global(ਖਾਲੀ);
ਬਾਹਰੀ ਪੂਰਨ  sched_dl_ਉਛਾਲ(ਬਣਤਰ task_struct *p, ਪੂਰਨ policy, ਅਚਲ ਬਣਤਰ sched_attr *attr);
ਬਾਹਰੀ ਖਾਲੀ __setparam_dl(ਬਣਤਰ task_struct *p, ਅਚਲ ਬਣਤਰ sched_attr *attr);
ਬਾਹਰੀ ਖਾਲੀ __getparam_dl(ਬਣਤਰ task_struct *p, ਬਣਤਰ sched_attr *attr, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags);
ਬਾਹਰੀ bool __checkparam_dl(ਅਚਲ ਬਣਤਰ sched_attr *attr);
ਬਾਹਰੀ bool dl_param_ਬਦਲਿਆ(ਬਣਤਰ task_struct *p, ਅਚਲ ਬਣਤਰ sched_attr *attr);
ਬਾਹਰੀ ਪੂਰਨ  dl_cpuset_cpumask_can_shrink(ਅਚਲ ਬਣਤਰ cpumask *cur, ਅਚਲ ਬਣਤਰ cpumask *trial);
ਬਾਹਰੀ ਪੂਰਨ  dl_bw_deactivate(ਪੂਰਨ cpu);
ਬਾਹਰੀ s64 dl_scaled_delta_exec(ਬਣਤਰ rq *rq, ਬਣਤਰ sched_dl_entity *dl_se, s64 delta_exec);
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
ਬਾਹਰੀ ਖਾਲੀ dl_server_update_idle(ਬਣਤਰ sched_dl_entity *dl_se, s64 delta_exec);
ਬਾਹਰੀ ਖਾਲੀ dl_server_update(ਬਣਤਰ sched_dl_entity *dl_se, s64 delta_exec);
ਬਾਹਰੀ ਖਾਲੀ dl_server_ਸ਼ੁਰੂ(ਬਣਤਰ sched_dl_entity *dl_se);
ਬਾਹਰੀ ਖਾਲੀ dl_server_stop(ਬਣਤਰ sched_dl_entity *dl_se);
ਬਾਹਰੀ ਖਾਲੀ dl_server_init(ਬਣਤਰ sched_dl_entity *dl_se, ਬਣਤਰ rq *rq,
		    dl_server_pick_f pick_task);
ਬਾਹਰੀ ਖਾਲੀ sched_init_dl_servers(ਖਾਲੀ);

ਬਾਹਰੀ ਖਾਲੀ fair_server_init(ਬਣਤਰ rq *rq);
ਬਾਹਰੀ ਖਾਲੀ ext_server_init(ਬਣਤਰ rq *rq);
ਬਾਹਰੀ ਖਾਲੀ __dl_server_attach_root(ਬਣਤਰ sched_dl_entity *dl_se, ਬਣਤਰ rq *rq);
ਬਾਹਰੀ ਪੂਰਨ dl_server_apply_params(ਬਣਤਰ sched_dl_entity *dl_se,
		    u64 runtime, u64 period, bool init);
ਬਾਹਰੀ ਪੂਰਨ dl_server_attach_bw(ਬਣਤਰ sched_dl_entity *dl_se);
ਬਾਹਰੀ ਖਾਲੀ dl_server_detach_bw(ਬਣਤਰ sched_dl_entity *dl_se);
ਬਾਹਰੀ ਪੂਰਨ dl_server_ਅਦਲਾ_ਬਦਲੀ_bw(ਬਣਤਰ sched_dl_entity *detach_se,
			     ਬਣਤਰ sched_dl_entity *attach_se);

ਸਥਿਰ ਸਤਰਅੰਦਰ bool dl_server_ਸਰਗਰਮ(ਬਣਤਰ sched_dl_entity *dl_se)
{
	ਵਾਪਸ dl_se->dl_server_ਸਰਗਰਮ;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_CGROUP_SCHED

ਬਾਹਰੀ ਬਣਤਰ list_ਸਿਰ task_groups;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_ਸਮੂਹ_SCHED_BANDWIDTH
ਬਾਹਰੀ ਅਚਲ u64 max_bw_quota_period_us;

/*
 * default period for group bandwidth.
 * default: 0.1s, units: microseconds
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ u64 default_bw_period_us(ਖਾਲੀ)
{
	ਵਾਪਸ 100000ULL;
}
#ਜੇ_ਅੰਤ /* CONFIG_GROUP_SCHED_BANDWIDTH */

ਬਣਤਰ cfs_bandwidth {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_CFS_BANDWIDTH
	raw_spinlock_t		ਤਾਲਾ;
	ktime_t			period;
	u64			quota;
	u64			runtime;
	u64			ਮਿਆਦ;
	u64			runtime_snap;
	s64			ਦਰਜਾਵਾਰ_quota;

	u8			idle;
	u8			period_ਸਰਗਰਮ;
	u8			slack_started;
	ਬਣਤਰ hrtimer		period_timer;
	ਬਣਤਰ hrtimer		slack_timer;
	ਬਣਤਰ list_ਸਿਰ	throttled_cfs_rq;

	/* Statistics: */
	ਪੂਰਨ			nr_periods;
	ਪੂਰਨ			nr_throttled;
	ਪੂਰਨ			nr_ਮਿਆਦ;
	u64			throttled_ਸਮਾਂ;
	u64			ਮਿਆਦ_ਸਮਾਂ;
#ਜੇ_ਅੰਤ /* CONFIG_CFS_BANDWIDTH */
};

/* Task group related information */
ਬਣਤਰ task_ਸਮੂਹ {
	ਬਣਤਰ cgroup_subsys_state css;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_ਸਮੂਹ_SCHED_ਭਾਰ
	/* A positive value indicates that this is a SCHED_IDLE group. */
	ਪੂਰਨ			idle;
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_FAIR_ਸਮੂਹ_SCHED
	/* runqueue "owned" by this group on each CPU */
	ਬਣਤਰ cfs_rq __percpu	*cfs_rq;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ		shares;
	/*
	 * load_avg can be heavily contended at clock tick time, so put
	 * it in its own cache-line separated from the fields above which
	 * will also be accessed at each tick.
	 */
	atomic_long_t		ਲੱਦੋ_avg ____cacheline_aligned;
	atomic_long_t		runnable_avg;

#ਜੇ_ਅੰਤ /* CONFIG_FAIR_GROUP_SCHED */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_RT_ਸਮੂਹ_SCHED
	ਬਣਤਰ sched_rt_entity	**rt_se;
	ਬਣਤਰ rt_rq		**rt_rq;

	ਬਣਤਰ rt_bandwidth	rt_bandwidth;
#ਜੇ_ਅੰਤ

	ਬਣਤਰ scx_task_ਸਮੂਹ	scx;

	ਬਣਤਰ rcu_ਸਿਰ		rcu;
	ਬਣਤਰ list_ਸਿਰ	list;

	ਬਣਤਰ task_ਸਮੂਹ	*ਮਾਪਾ;
	ਬਣਤਰ list_ਸਿਰ	siblings;
	ਬਣਤਰ list_ਸਿਰ	children;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_AUTOGROUP
	ਬਣਤਰ autogroup	*autogroup;
#ਜੇ_ਅੰਤ

	ਬਣਤਰ cfs_bandwidth	cfs_bandwidth;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_UCLAMP_TASK_ਸਮੂਹ
	/* The two decimal precision [%] value requested from user-space */
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		uclamp_pct[UCLAMP_CNT];
	/* Clamp values requested for a task group */
	ਬਣਤਰ uclamp_se	uclamp_req[UCLAMP_CNT];
	/* Effective clamp values used for a task group */
	ਬਣਤਰ uclamp_se	uclamp[UCLAMP_CNT];
#ਜੇ_ਅੰਤ

};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਜੜ੍ਹ_TASK_ਸਮੂਹ_ਲੱਦੋ	NICE_0_ਲੱਦੋ

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਪੂਰਨ (*tg_visitor)(ਬਣਤਰ task_ਸਮੂਹ *, ਖਾਲੀ *);

ਬਾਹਰੀ ਪੂਰਨ walk_tg_ਰੁੱਖ_from(ਬਣਤਰ task_ਸਮੂਹ *from,
			     tg_visitor ਥੱਲੇ, tg_visitor ਉੱਪਰ, ਖਾਲੀ *ਅੰਕੜੇ);

/*
 * Iterate the full tree, calling @down when first entering a node and @up when
 * leaving it for the final time.
 *
 * Caller must hold rcu_lock or sufficient equivalent.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ walk_tg_ਰੁੱਖ(tg_visitor ਥੱਲੇ, tg_visitor ਉੱਪਰ, ਖਾਲੀ *ਅੰਕੜੇ)
{
	ਵਾਪਸ walk_tg_ਰੁੱਖ_from(&ਜੜ੍ਹ_task_ਸਮੂਹ, ਥੱਲੇ, ਉੱਪਰ, ਅੰਕੜੇ);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ task_ਸਮੂਹ *css_tg(ਬਣਤਰ cgroup_subsys_state *css)
{
	ਵਾਪਸ css ? container_ਦਾ(css, ਬਣਤਰ task_ਸਮੂਹ, css) : ਨਿਰਾ;
}

ਬਾਹਰੀ ਪੂਰਨ tg_nop(ਬਣਤਰ task_ਸਮੂਹ *tg, ਖਾਲੀ *ਅੰਕੜੇ);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_FAIR_ਸਮੂਹ_SCHED
ਬਾਹਰੀ ਖਾਲੀ free_fair_sched_ਸਮੂਹ(ਬਣਤਰ task_ਸਮੂਹ *tg);
ਬਾਹਰੀ ਪੂਰਨ alloc_fair_sched_ਸਮੂਹ(ਬਣਤਰ task_ਸਮੂਹ *tg, ਬਣਤਰ task_ਸਮੂਹ *ਮਾਪਾ);
ਬਾਹਰੀ ਖਾਲੀ online_fair_sched_ਸਮੂਹ(ਬਣਤਰ task_ਸਮੂਹ *tg);
ਬਾਹਰੀ ਖਾਲੀ unregister_fair_sched_ਸਮੂਹ(ਬਣਤਰ task_ਸਮੂਹ *tg);
ਬਾਹਰੀ ਖਾਲੀ __sched_cgroup_mode_update(ਪੂਰਨ ਬਹੁਲਕ);
#ਨਹੀਂਤਾਂ /* !CONFIG_FAIR_GROUP_SCHED: */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ free_fair_sched_ਸਮੂਹ(ਬਣਤਰ task_ਸਮੂਹ *tg) { }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ alloc_fair_sched_ਸਮੂਹ(ਬਣਤਰ task_ਸਮੂਹ *tg, ਬਣਤਰ task_ਸਮੂਹ *ਮਾਪਾ)
{
       ਵਾਪਸ 1;
}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ online_fair_sched_ਸਮੂਹ(ਬਣਤਰ task_ਸਮੂਹ *tg) { }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ unregister_fair_sched_ਸਮੂਹ(ਬਣਤਰ task_ਸਮੂਹ *tg) { }
#ਜੇ_ਅੰਤ /* !CONFIG_FAIR_GROUP_SCHED */

ਬਾਹਰੀ ਖਾਲੀ init_tg_cfs_ਇੰਦਰਾਜ(ਬਣਤਰ task_ਸਮੂਹ *tg, ਬਣਤਰ cfs_rq *cfs_rq,
			ਬਣਤਰ sched_entity *se, ਪੂਰਨ cpu,
			ਬਣਤਰ sched_entity *ਮਾਪਾ);
ਬਾਹਰੀ ਖਾਲੀ init_cfs_bandwidth(ਬਣਤਰ cfs_bandwidth *cfs_b, ਬਣਤਰ cfs_bandwidth *ਮਾਪਾ);

ਬਾਹਰੀ ਖਾਲੀ __refill_cfs_bandwidth_runtime(ਬਣਤਰ cfs_bandwidth *cfs_b);
ਬਾਹਰੀ ਖਾਲੀ ਸ਼ੁਰੂ_cfs_bandwidth(ਬਣਤਰ cfs_bandwidth *cfs_b);
ਬਾਹਰੀ ਖਾਲੀ unthrottle_cfs_rq(ਬਣਤਰ cfs_rq *cfs_rq);
ਬਾਹਰੀ bool cfs_task_bw_constrained(ਬਣਤਰ task_struct *p);

ਬਾਹਰੀ ਖਾਲੀ init_tg_rt_ਇੰਦਰਾਜ(ਬਣਤਰ task_ਸਮੂਹ *tg, ਬਣਤਰ rt_rq *rt_rq,
		ਬਣਤਰ sched_rt_entity *rt_se, ਪੂਰਨ cpu,
		ਬਣਤਰ sched_rt_entity *ਮਾਪਾ);
ਬਾਹਰੀ ਪੂਰਨ sched_ਸਮੂਹ_ਸੈੱਟ_rt_runtime(ਬਣਤਰ task_ਸਮੂਹ *tg, ਲੰਮਾ rt_runtime_us);
ਬਾਹਰੀ ਪੂਰਨ sched_ਸਮੂਹ_ਸੈੱਟ_rt_period(ਬਣਤਰ task_ਸਮੂਹ *tg, u64 rt_period_us);
ਬਾਹਰੀ ਲੰਮਾ sched_ਸਮੂਹ_rt_runtime(ਬਣਤਰ task_ਸਮੂਹ *tg);
ਬਾਹਰੀ ਲੰਮਾ sched_ਸਮੂਹ_rt_period(ਬਣਤਰ task_ਸਮੂਹ *tg);
ਬਾਹਰੀ ਪੂਰਨ sched_rt_can_attach(ਬਣਤਰ task_ਸਮੂਹ *tg, ਬਣਤਰ task_struct *tsk);

ਬਾਹਰੀ ਬਣਤਰ task_ਸਮੂਹ *sched_create_ਸਮੂਹ(ਬਣਤਰ task_ਸਮੂਹ *ਮਾਪਾ);
ਬਾਹਰੀ ਖਾਲੀ sched_online_ਸਮੂਹ(ਬਣਤਰ task_ਸਮੂਹ *tg,
			       ਬਣਤਰ task_ਸਮੂਹ *ਮਾਪਾ);
ਬਾਹਰੀ ਖਾਲੀ sched_destroy_ਸਮੂਹ(ਬਣਤਰ task_ਸਮੂਹ *tg);
ਬਾਹਰੀ ਖਾਲੀ sched_release_ਸਮੂਹ(ਬਣਤਰ task_ਸਮੂਹ *tg);

ਬਾਹਰੀ ਖਾਲੀ sched_move_task(ਬਣਤਰ task_struct *tsk, bool for_autogroup);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_FAIR_ਸਮੂਹ_SCHED
ਬਾਹਰੀ ਪੂਰਨ sched_ਸਮੂਹ_ਸੈੱਟ_shares(ਬਣਤਰ task_ਸਮੂਹ *tg, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ shares);

ਬਾਹਰੀ ਪੂਰਨ sched_ਸਮੂਹ_ਸੈੱਟ_idle(ਬਣਤਰ task_ਸਮੂਹ *tg, ਲੰਮਾ idle);

ਬਾਹਰੀ ਖਾਲੀ ਸੈੱਟ_task_rq_fair(ਬਣਤਰ sched_entity *se,
			     ਬਣਤਰ cfs_rq *prev, ਬਣਤਰ cfs_rq *ਅਗਲਾ);
#ਨਹੀਂਤਾਂ /* !CONFIG_FAIR_GROUP_SCHED: */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ sched_ਸਮੂਹ_ਸੈੱਟ_shares(ਬਣਤਰ task_ਸਮੂਹ *tg, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ shares) { ਵਾਪਸ 0; }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ sched_ਸਮੂਹ_ਸੈੱਟ_idle(ਬਣਤਰ task_ਸਮੂਹ *tg, ਲੰਮਾ idle) { ਵਾਪਸ 0; }
#ਜੇ_ਅੰਤ /* !CONFIG_FAIR_GROUP_SCHED */

#ਨਹੀਂਤਾਂ /* !CONFIG_CGROUP_SCHED: */

ਬਣਤਰ cfs_bandwidth { };

ਸਥਿਰ ਸਤਰਅੰਦਰ bool cfs_task_bw_constrained(ਬਣਤਰ task_struct *p) { ਵਾਪਸ false; }

#ਜੇ_ਅੰਤ /* !CONFIG_CGROUP_SCHED */

/*
 * A weight of 0 or 1 can cause arithmetics problems.
 * A weight of a cfs_rq is the sum of weights of which entities
 * are queued on this cfs_rq, so a weight of a entity should not be
 * too large, so as the shares value of a task group.
 * (The default weight is 1024 - so there's no practical
 *  limitation from this.)
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MIN_SHARES		(1UL <<  1)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MAX_SHARES		(1UL << 18)

ਬਾਹਰੀ ਖਾਲੀ unregister_rt_sched_ਸਮੂਹ(ਬਣਤਰ task_ਸਮੂਹ *tg);
ਬਾਹਰੀ ਖਾਲੀ free_rt_sched_ਸਮੂਹ(ਬਣਤਰ task_ਸਮੂਹ *tg);
ਬਾਹਰੀ ਪੂਰਨ alloc_rt_sched_ਸਮੂਹ(ਬਣਤਰ task_ਸਮੂਹ *tg, ਬਣਤਰ task_ਸਮੂਹ *ਮਾਪਾ);

/*
 * u64_u32_load/u64_u32_store
 *
 * Use a copy of a u64 value to protect against data race. This is only
 * applicable for 32-bits architectures.
 */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_64_ਦੋਆਧਾਰੀ_ਅੰਕ
# define u64_u32_ਲੱਦੋ_copy(var, copy)		var
# define u64_u32_store_copy(var, copy, val)	(var = val)
#ਨਹੀਂਤਾਂ
# define u64_u32_ਲੱਦੋ_copy(var, copy)					\
({									\
	u64 __val, __val_copy;						\
	ਕਰੋ {								\
		__val_copy = copy;					\
		/*							\
		 * paired with u64_u32_store_copy(), ordering access	\
		 * to var and copy.					\
		 */							\
		smp_rmb();						\
		__val = var;						\
	} ਜਦੋਂ (__val != __val_copy);					\
	__val;								\
})
# define u64_u32_store_copy(var, copy, val)				\
ਕਰੋ {									\
	typeof(val) __val = (val);					\
	var = __val;							\
	/*								\
	 * paired with u64_u32_load_copy(), ordering access to var and	\
	 * copy.							\
	 */								\
	smp_wmb();							\
	copy = __val;							\
} ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ
# define u64_u32_ਲੱਦੋ(var)		u64_u32_ਲੱਦੋ_copy(var, var##_copy)
# define u64_u32_store(var, val)	u64_u32_store_copy(var, var##_copy, val)

ਬਣਤਰ balance_callback {
	ਬਣਤਰ balance_callback *ਅਗਲਾ;
	ਖਾਲੀ (*func)(ਬਣਤਰ rq *rq);
};

/* Fair scheduling SCHED_{NORMAL,BATCH,IDLE} related fields in a runqueue: */
ਬਣਤਰ cfs_rq {
	ਬਣਤਰ ਲੱਦੋ_ਭਾਰ	ਲੱਦੋ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		nr_queued;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		h_nr_queued;		/* SCHED_{NORMAL,BATCH,IDLE} */
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		h_nr_runnable;		/* SCHED_{NORMAL,BATCH,IDLE} */
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		h_nr_idle;		/* SCHED_IDLE */

	s64			ਜੋੜ_w_vruntime;
	u64			ਜੋੜ_ਭਾਰ;
	u64			zero_vruntime;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		ਜੋੜ_ਖਿਸਕਾਓ;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CORE
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		forceidle_seq;
	u64			zero_vruntime_fi;
#ਜੇ_ਅੰਤ

	ਬਣਤਰ rb_ਜੜ੍ਹ_cached	tasks_timeline;

	/*
	 * 'curr' points to the currently running entity on this cfs_rq.
	 * It is set to NULL otherwise (i.e when none are currently running).
	 */
	ਬਣਤਰ sched_entity	*curr;
	ਬਣਤਰ sched_entity	*ਅਗਲਾ;

	/*
	 * CFS load tracking
	 */
	ਬਣਤਰ sched_entity	*h_curr;
	ਬਣਤਰ sched_avg	avg;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CONFIG_64_ਦੋਆਧਾਰੀ_ਅੰਕ
	u64			ਆਖਰੀ_update_ਸਮਾਂ_copy;
#ਜੇ_ਅੰਤ
	ਬਣਤਰ {
		raw_spinlock_t	ਤਾਲਾ ____cacheline_aligned;
		ਪੂਰਨ		nr;
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ	ਲੱਦੋ_avg;
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ	util_avg;
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ	runnable_avg;
	} removed;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_FAIR_ਸਮੂਹ_SCHED
	u64			ਆਖਰੀ_update_tg_ਲੱਦੋ_avg;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ		tg_ਲੱਦੋ_avg_contrib;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ		tg_runnable_avg_contrib;
	ਲੰਮਾ			propagate;
	ਲੰਮਾ			prop_runnable_ਜੋੜ;

	/*
	 *   h_load = weight * f(tg)
	 *
	 * Where f(tg) is the recursive weight fraction assigned to
	 * this group.
	 */
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ		h_ਲੱਦੋ;
	u64			ਆਖਰੀ_h_ਲੱਦੋ_update;
	ਬਣਤਰ sched_entity	*h_ਲੱਦੋ_ਅਗਲਾ;

	ਬਣਤਰ rq		*rq;	/* CPU runqueue to which this cfs_rq is attached */

	/*
	 * leaf cfs_rqs are those that hold tasks (lowest schedulable entity in
	 * a hierarchy). Non-leaf lrqs hold other higher schedulable entities
	 * (like users, containers etc.)
	 *
	 * leaf_cfs_rq_list ties together list of leaf cfs_rq's in a CPU.
	 * This list is used during load balance.
	 */
	ਪੂਰਨ			on_list;
	ਬਣਤਰ list_ਸਿਰ	ਪੱਤੀ_cfs_rq_list;
	ਬਣਤਰ task_ਸਮੂਹ	*tg;	/* Group that "owns" this runqueue */

	/* Locally cached copy of our task_group's idle value */
	ਪੂਰਨ			idle;

# ifdef CONFIG_CFS_BANDWIDTH
	ਪੂਰਨ			runtime_enabled;
	s64			runtime_ਬਚਿਆ;

	u64			throttled_pelt_idle;
#  ifndef CONFIG_64_ਦੋਆਧਾਰੀ_ਅੰਕ
	u64                     throttled_pelt_idle_copy;
#  endif
	u64			throttled_clock;
	u64			throttled_clock_pelt;
	u64			throttled_clock_pelt_ਸਮਾਂ;
	u64			throttled_clock_self;
	u64			throttled_clock_self_ਸਮਾਂ;
	bool			throttled:1;
	bool			pelt_clock_throttled:1;
	ਪੂਰਨ			throttle_ਗਿਣਤੀ;
	ਬਣਤਰ list_ਸਿਰ	throttled_list;
	ਬਣਤਰ list_ਸਿਰ	throttled_csd_list;
	ਬਣਤਰ list_ਸਿਰ        throttled_limbo_list;
# endif /* CONFIG_CFS_BANDWIDTH */
#ਜੇ_ਅੰਤ /* CONFIG_FAIR_GROUP_SCHED */
};

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CLASS_EXT
/* scx_rq->flags, protected by the rq lock */
ਗਿਣਤੀ scx_rq_flags {
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
	SCX_RQ_ਜੜ੍ਹ_IDLE_RENOTIFY	= 1 << 8, /* the root is owed update_idle() */

	SCX_RQ_ਵਿੱਚ_WAKEUP	= 1 << 16,
	SCX_RQ_ਵਿੱਚ_DISPATCH	= 1 << 17,
};

/* per-rq rescue execution state, see scx_rescue_timerfn() */
ਬਣਤਰ scx_rq_rescue {
	ਬਣਤਰ scx_dispatch_q	dsq;			/* stranded tasks awaiting rescue */
	s64			budget;			/* execution token bucket, ns */
	u64			clock;			/* last budget accrual timestamp */
	ਬਣਤਰ task_struct	*curr;			/* task being rescued, one at a time */
	s64			ਟੁਕੜਾ;			/* curr's admitted slice */
	u64			exec_snap;		/* sum_exec_runtime at admission */
	ਬਣਤਰ timer_list	timer;			/* paces admission and escalation */
	u64			kill_at;		/* last ejection, init before any */
};

ਬਣਤਰ scx_rq {
	ਬਣਤਰ scx_dispatch_q	local_dsq;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_EXT_SUB_SCHED
	ਬਣਤਰ scx_dispatch_q	reject_dsq;		/* staging for cap-rejected tasks */
	ਬਣਤਰ scx_rq_rescue	rescue;
#ਜੇ_ਅੰਤ
	ਬਣਤਰ list_ਸਿਰ	runnable_list;		/* runnable tasks on this rq */
	ਬਣਤਰ list_ਸਿਰ	ddsp_deferred_locals;	/* deferred ddsps from enq */
	/* both stashed across the activate_task() in move_remote_task_to_local_dsq() */
	u64			remote_activate_enq_flags;
	ਬਣਤਰ scx_sched	*remote_activate_sch;
	u32			nr_running;
	u32			cpuperf_ਨਿਸ਼ਾਨਾ;		/* [0, SCHED_CAPACITY_SCALE] */
	bool			ਵਿੱਚ_select_cpu;
	bool			cpu_released;
	u32			flags;
	u32			nr_immed;		/* ENQ_IMMED tasks on local_dsq */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CORE
	u32			ਤਾਲਾ_drop_seq;	/* nr dispatch lock releases */
#ਜੇ_ਅੰਤ
	u64			clock;			/* current per-rq clock -- see scx_bpf_now() */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_EXT_SUB_SCHED
	ਬਣਤਰ llist_ਸਿਰ	ecaps_ਮੰਜ਼ਿਲ_sync;		/* pending ecaps syncs */
	ਬਣਤਰ task_struct	*sub_dispatch_prev;
#ਜੇ_ਅੰਤ
	cpumask_var_t		cpus_ਮੰਜ਼ਿਲ_sync;
	bool			kick_sync_pending;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ		kick_sync;

	ਬਣਤਰ list_ਸਿਰ	sched_pcpus_ਮੰਜ਼ਿਲ_kick;	/* see kick_cpus_irq_workfn() */

	raw_spinlock_t		deferred_reenq_ਤਾਲਾ;
	ਬਣਤਰ list_ਸਿਰ	deferred_reenq_locals;	/* scheds requesting reenq of local DSQ */
	ਬਣਤਰ list_ਸਿਰ	deferred_reenq_users;	/* user DSQs requesting reenq */
	ਬਣਤਰ balance_callback	deferred_bal_cb;
	ਬਣਤਰ balance_callback	kick_sync_bal_cb;
	ਬਣਤਰ irq_work		deferred_irq_work;
	ਬਣਤਰ irq_work		kick_cpus_irq_work;
};
#ਜੇ_ਅੰਤ /* CONFIG_SCHED_CLASS_EXT */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ rt_bandwidth_enabled(ਖਾਲੀ)
{
	ਵਾਪਸ sysctl_sched_rt_runtime >= 0;
}

/* RT IPI pull logic requires IRQ_WORK */
#ਜੇ defined(CONFIG_IRQ_WORK) && defined(CONFIG_SMP)
# define HAVE_RT_ਧੱਕੋ_IPI
#ਜੇ_ਅੰਤ

/* Real-Time classes' related field in a runqueue: */
ਬਣਤਰ rt_rq {
	ਬਣਤਰ rt_prio_array	ਸਰਗਰਮ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		rt_nr_running;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		rr_nr_running;
	ਬਣਤਰ {
		ਪੂਰਨ		curr; /* highest queued rt task prio */
		ਪੂਰਨ		ਅਗਲਾ; /* next highest */
	} highest_prio;
	bool			overloaded;
	ਬਣਤਰ plist_ਸਿਰ	pushable_tasks;

	ਪੂਰਨ			rt_queued;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_RT_ਸਮੂਹ_SCHED
	ਪੂਰਨ			rt_throttled;
	u64			rt_ਸਮਾਂ; /* consumed RT time, goes up in update_curr_rt */
	u64			rt_runtime; /* allotted RT time, "slice" from rt_bandwidth, RT sharing/balancing */
	/* Nests inside the rq lock: */
	raw_spinlock_t		rt_runtime_ਤਾਲਾ;

	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		rt_nr_boosted;

	ਬਣਤਰ rq		*rq; /* this is always top-level rq, cache? */
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_CGROUP_SCHED
	ਬਣਤਰ task_ਸਮੂਹ	*tg; /* this tg has "this" rt_rq on given CPU for runnable entities */
#ਜੇ_ਅੰਤ
};

ਸਥਿਰ ਸਤਰਅੰਦਰ bool rt_rq_is_runnable(ਬਣਤਰ rt_rq *rt_rq)
{
	ਵਾਪਸ rt_rq->rt_queued && rt_rq->rt_nr_running;
}

/* Deadline class' related fields in a runqueue */
ਬਣਤਰ dl_rq {
	/* runqueue is an rbtree, ordered by deadline */
	ਬਣਤਰ rb_ਜੜ੍ਹ_cached	ਜੜ੍ਹ;

	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		dl_nr_running;

	/*
	 * Deadline values of the currently executing and the
	 * earliest ready task on this rq. Caching these facilitates
	 * the decision whether or not a ready but not running task
	 * should migrate somewhere else.
	 */
	ਬਣਤਰ {
		u64		curr;
		u64		ਅਗਲਾ;
	} earliest_dl;

	bool			overloaded;

	ਬਣਤਰ sched_dl_entity	*curr;
	/*
	 * Tasks on this rq that can be pushed away. They are kept in
	 * an rb-tree, ordered by tasks' deadlines, with caching
	 * of the leftmost (earliest deadline) element.
	 */
	ਬਣਤਰ rb_ਜੜ੍ਹ_cached	pushable_dl_tasks_ਜੜ੍ਹ;

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
	u64			bw_ਅਨੁਪਾਤ;
};

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_FAIR_ਸਮੂਹ_SCHED
/* Check whether a task group is root tg */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ is_ਜੜ੍ਹ_task_ਸਮੂਹ(tg) ((tg) == &ਜੜ੍ਹ_task_ਸਮੂਹ)
/* An entity is a task if it doesn't "own" a runqueue */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ entity_is_task(se)	(!se->my_q)

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ se_update_runnable(ਬਣਤਰ sched_entity *se)
{
	ਜੇ (!entity_is_task(se))
		se->runnable_ਭਾਰ = se->my_q->h_nr_runnable;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਲੰਮਾ se_runnable(ਬਣਤਰ sched_entity *se)
{
	ਜੇ (se->sched_delayed)
		ਵਾਪਸ false;

	ਜੇ (entity_is_task(se))
		ਵਾਪਸ !!se->on_rq;
	ਨਹੀਂਤਾਂ
		ਵਾਪਸ se->runnable_ਭਾਰ;
}

#ਨਹੀਂਤਾਂ /* !CONFIG_FAIR_GROUP_SCHED: */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ entity_is_task(se)	1

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ se_update_runnable(ਬਣਤਰ sched_entity *se) { }

ਸਥਿਰ ਸਤਰਅੰਦਰ ਲੰਮਾ se_runnable(ਬਣਤਰ sched_entity *se)
{
	ਜੇ (se->sched_delayed)
		ਵਾਪਸ false;

	ਵਾਪਸ !!se->on_rq;
}

#ਜੇ_ਅੰਤ /* !CONFIG_FAIR_GROUP_SCHED */

/*
 * XXX we want to get rid of these helpers and use the full load resolution.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਲੰਮਾ se_ਭਾਰ(ਬਣਤਰ sched_entity *se)
{
	ਵਾਪਸ scale_ਲੱਦੋ_ਥੱਲੇ(se->ਲੱਦੋ.ਭਾਰ);
}


ਸਥਿਰ ਸਤਰਅੰਦਰ bool sched_asym_prefer(ਪੂਰਨ a, ਪੂਰਨ b)
{
	ਵਾਪਸ arch_asym_cpu_ਤਰਜੀਹ(a) > arch_asym_cpu_ਤਰਜੀਹ(b);
}

ਬਣਤਰ perf_domain {
	ਬਣਤਰ em_perf_domain *em_pd;
	ਬਣਤਰ perf_domain *ਅਗਲਾ;
	ਬਣਤਰ rcu_ਸਿਰ rcu;
};

/*
 * We add the notion of a root-domain which will be used to define per-domain
 * variables. Each exclusive cpuset essentially defines an island domain by
 * fully partitioning the member CPUs from any other cpuset. Whenever a new
 * exclusive cpuset is created, we also create and attach a new root-domain
 * object.
 *
 */
ਬਣਤਰ ਜੜ੍ਹ_domain {
	atomic_t		refcount;
	atomic_t		rto_ਗਿਣਤੀ;
	ਬਣਤਰ rcu_ਸਿਰ		rcu;
	cpumask_var_t		ਵਿਸਤਾਰ;
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
	cpumask_var_t		dlo_ਨਕਾਬ;
	atomic_t		dlo_ਗਿਣਤੀ;
	ਬਣਤਰ dl_bw		dl_bw;
	ਬਣਤਰ cpudl		cpudl;

	/*
	 * Indicate whether a root_domain's dl_bw has been checked or
	 * updated. It's monotonously increasing value.
	 *
	 * Also, some corner cases, like 'wrap around' is dangerous, but given
	 * that u64 is 'big enough'. So that shouldn't be a concern.
	 */
	u64 ਦੇਖੋ_cookie;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_RT_ਧੱਕੋ_IPI
	/*
	 * For IPI pull requests, loop across the rto_mask.
	 */
	ਬਣਤਰ irq_work		rto_ਧੱਕੋ_work;
	raw_spinlock_t		rto_ਤਾਲਾ;
	/* These are only updated and read within rto_lock */
	ਪੂਰਨ			rto_loop;
	ਪੂਰਨ			rto_cpu;
	/* These atomics are updated outside of a lock */
	atomic_t		rto_loop_ਅਗਲਾ;
	atomic_t		rto_loop_ਸ਼ੁਰੂ;
#ਜੇ_ਅੰਤ /* HAVE_RT_PUSH_IPI */
	/*
	 * The "RT overload" flag: it gets set if a CPU has more than
	 * one runnable RT task.
	 */
	cpumask_var_t		rto_ਨਕਾਬ;
	ਬਣਤਰ cpupri		cpupri;

	/*
	 * NULL-terminated list of performance domains intersecting with the
	 * CPUs of the rd. Protected by RCU.
	 */
	ਬਣਤਰ perf_domain __rcu *pd;
};

ਬਾਹਰੀ ਖਾਲੀ init_defrootdomain(ਖਾਲੀ);
ਬਾਹਰੀ ਪੂਰਨ sched_init_domains(ਅਚਲ ਬਣਤਰ cpumask *cpu_map);
ਬਾਹਰੀ ਖਾਲੀ rq_attach_ਜੜ੍ਹ(ਬਣਤਰ rq *rq, ਬਣਤਰ ਜੜ੍ਹ_domain *rd);
ਬਾਹਰੀ ਖਾਲੀ sched_get_rd(ਬਣਤਰ ਜੜ੍ਹ_domain *rd);
ਬਾਹਰੀ ਖਾਲੀ sched_put_rd(ਬਣਤਰ ਜੜ੍ਹ_domain *rd);

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ get_rd_overloaded(ਬਣਤਰ ਜੜ੍ਹ_domain *rd)
{
	ਵਾਪਸ READ_ONCE(rd->overloaded);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ ਸੈੱਟ_rd_overloaded(ਬਣਤਰ ਜੜ੍ਹ_domain *rd, ਪੂਰਨ ਸਥਿਤੀ)
{
	ਜੇ (get_rd_overloaded(rd) != ਸਥਿਤੀ)
		WRITE_ONCE(rd->overloaded, ਸਥਿਤੀ);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_RT_ਧੱਕੋ_IPI
ਬਾਹਰੀ ਖਾਲੀ rto_ਧੱਕੋ_irq_work_func(ਬਣਤਰ irq_work *work);
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_UCLAMP_TASK
/*
 * struct uclamp_bucket - Utilization clamp bucket
 * @value: utilization clamp value for tasks on this clamp bucket
 * @tasks: number of RUNNABLE tasks on this clamp bucket
 *
 * Keep track of how many tasks are RUNNABLE for a given utilization
 * clamp value.
 */
ਬਣਤਰ uclamp_ਬਾਲਟੀ {
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਮੁੱਲ : ਬਿੱਟ_ਪ੍ਰਤੀ(SCHED_ਸਮਰੱਥਾ_SCALE);
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ tasks : ਬਿੱਟ_ਪ੍ਰਤੀ_LONG - ਬਿੱਟ_ਪ੍ਰਤੀ(SCHED_ਸਮਰੱਥਾ_SCALE);
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
ਬਣਤਰ uclamp_rq {
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮੁੱਲ;
	ਬਣਤਰ uclamp_ਬਾਲਟੀ ਬਾਲਟੀ[UCLAMP_BUCKETS];
};

DECLARE_STATIC_ਕੁੰਜੀ_FALSE(sched_uclamp_ਵਰਤਿਆ);
#ਜੇ_ਅੰਤ /* CONFIG_UCLAMP_TASK */

/*
 * This is the main, per-CPU runqueue data structure.
 *
 * Locking rule: those places that want to lock multiple runqueues
 * (such as the load balancing or the thread migration code), lock
 * acquire operations must be ordered by ascending &runqueue.
 */
ਬਣਤਰ rq {
	/*
	 * The following members are loaded together, without holding the
	 * rq->lock, in an extremely hot loop in update_sg_lb_stats()
	 * (called from pick_next_task()). To reduce cache pollution from
	 * this operation, they are placed together on this dedicated cache
	 * line. Even though some of them are frequently modified, they are
	 * loaded much more frequently than they are stored.
	 */
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		nr_running;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_NUMA_BALANCING
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		nr_numa_running;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		nr_preferred_running;
#ਜੇ_ਅੰਤ
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		ttwu_pending;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ		cpu_ਸਮਰੱਥਾ;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_PROXY_EXEC
	ਬਣਤਰ task_struct __rcu	*donor;  /* Scheduling context */
	ਬਣਤਰ task_struct __rcu	*curr;   /* Execution context */
#ਨਹੀਂਤਾਂ
	ਸੰਘ {
		ਬਣਤਰ task_struct __rcu *donor; /* Scheduler context */
		ਬਣਤਰ task_struct __rcu *curr;  /* Execution context */
	};
#ਜੇ_ਅੰਤ
	ਬਣਤਰ task_struct	*idle;
	/* padding left here deliberately */

	/*
	 * The next cacheline holds the (hot) runqueue lock, as well as
	 * some other less performance-critical fields.
	 */
	u64			nr_switches	____cacheline_aligned;

	/* runqueue lock: */
	raw_spinlock_t		__lock;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_NO_HZ_COMMON
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		nohz_tick_stopped;
	atomic_t		nohz_flags;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		has_blocked_ਲੱਦੋ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ		ਆਖਰੀ_blocked_ਲੱਦੋ_update_tick;
	call_single_ਅੰਕੜੇ_t	nohz_csd;
#ਜੇ_ਅੰਤ /* CONFIG_NO_HZ_COMMON */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_UCLAMP_TASK
	/* Utilization clamp values based on CPU's RUNNABLE tasks */
	ਬਣਤਰ uclamp_rq	uclamp[UCLAMP_CNT] ____cacheline_aligned;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		uclamp_flags;
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ UCLAMP_FLAG_IDLE 0x01
#ਜੇ_ਅੰਤ

	ਬਣਤਰ cfs_rq		cfs;
	ਬਣਤਰ rt_rq		rt;
	ਬਣਤਰ dl_rq		dl;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CLASS_EXT
	ਬਣਤਰ scx_rq		scx;
	ਬਣਤਰ sched_dl_entity	ext_server;
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CACHE
	raw_spinlock_t		cpu_epoch_ਤਾਲਾ ____cacheline_aligned;
	u64			cpu_runtime;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ		cpu_epoch;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ		cpu_epoch_ਅਗਲਾ;
#ਜੇ_ਅੰਤ

	ਬਣਤਰ sched_dl_entity	fair_server;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_FAIR_ਸਮੂਹ_SCHED
	/* list of leaf cfs_rq on this CPU: */
	ਬਣਤਰ list_ਸਿਰ	ਪੱਤੀ_cfs_rq_list;
	ਬਣਤਰ list_ਸਿਰ	*tmp_alone_branch;
#ਜੇ_ਅੰਤ /* CONFIG_FAIR_GROUP_SCHED */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_NUMA_BALANCING
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		numa_migrate_on;
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CACHE
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		nr_pref_llc_running;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		nr_llc_running;
#ਜੇ_ਅੰਤ

	/*
	 * This is part of a global counter where only the total sum
	 * over all CPUs matters. A task can increase this counter on
	 * one CPU and if it got migrated afterwards it may decrease
	 * it on another CPU. Always updated under the runqueue lock:
	 */
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ		nr_uninterruptible;

	ਬਣਤਰ sched_dl_entity	*dl_server;
	ਬਣਤਰ task_struct	*stop;
	ਅਚਲ ਬਣਤਰ sched_class *ਅਗਲਾ_class;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ		ਅਗਲਾ_balance;
	ਬਣਤਰ mm_struct	*prev_mm;

	/*
	 * The following fields of clock data are frequently referenced
	 * and updated together, and should go on their own cache line.
	 */
	u64			clock_task ____cacheline_aligned;
	u64			clock_pelt;
	u64			clock;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ		ਗੁਆਚਾ_idle_ਸਮਾਂ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		clock_update_flags;
	u64			clock_pelt_idle;
	u64			clock_idle;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CONFIG_64_ਦੋਆਧਾਰੀ_ਅੰਕ
	u64			clock_pelt_idle_copy;
	u64			clock_idle_copy;
#ਜੇ_ਅੰਤ

	u64 ਆਖਰੀ_ਵੇਖਿਆ_need_resched_ns;
	ਪੂਰਨ ticks_without_resched;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_MEMBARRIER
	ਪੂਰਨ membarrier_state;
#ਜੇ_ਅੰਤ

	ਬਣਤਰ ਜੜ੍ਹ_domain		*rd;
	ਬਣਤਰ sched_domain __rcu	*sd;

	ਬਣਤਰ balance_callback *balance_callback;

	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ		nohz_idle_balance;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ		idle_balance;

	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ		misfit_task_ਲੱਦੋ;

	/* For active balancing */
	ਪੂਰਨ			ਸਰਗਰਮ_balance;
	ਪੂਰਨ			ਧੱਕੋ_cpu;
	ਬਣਤਰ cpu_stop_work	ਸਰਗਰਮ_balance_work;

	/* CPU of this runqueue: */
	ਪੂਰਨ			cpu;
	ਪੂਰਨ			online;

	ਬਣਤਰ list_ਸਿਰ cfs_tasks;

	ਬਣਤਰ sched_avg	avg_rt;
	ਬਣਤਰ sched_avg	avg_dl;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_HAVE_SCHED_AVG_IRQ
	ਬਣਤਰ sched_avg	avg_irq;
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_HW_PRESSURE
	ਬਣਤਰ sched_avg	avg_hw;
#ਜੇ_ਅੰਤ
	u64			idle_stamp;
	u64			avg_idle;

	/* This is used to determine avg_idle's max value */
	u64			max_idle_balance_ਲਾਗਤ;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_HOTPLUG_CPU
	ਬਣਤਰ rcuwait		hotplug_wait;
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_IRQ_ਸਮਾਂ_ACCOUNTING
	u64			prev_irq_ਸਮਾਂ;
	u64			psi_irq_ਸਮਾਂ;
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_PARAVIRT
	u64			prev_steal_ਸਮਾਂ;
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_PARAVIRT_ਸਮਾਂ_ACCOUNTING
	u64			prev_steal_ਸਮਾਂ_rq;
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_PREFERRED_CPU
	bool			npc_ਧੱਕੋ_work_pending;
#ਜੇ_ਅੰਤ

	/* calc_load related fields */
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ		calc_ਲੱਦੋ_update;
	ਲੰਮਾ			calc_ਲੱਦੋ_ਸਰਗਰਮ;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_HRTICK
	call_single_ਅੰਕੜੇ_t	hrtick_csd;
	ਬਣਤਰ hrtimer		hrtick_timer;
	ktime_t			hrtick_ਸਮਾਂ;
	ktime_t			hrtick_delay;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		hrtick_sched;
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHEDSTATS
	/* latency stats */
	ਬਣਤਰ sched_info	rq_sched_info;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ	rq_cpu_ਸਮਾਂ;

	/* sys_sched_yield() stats */
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		yld_ਗਿਣਤੀ;

	/* schedule() stats */
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		sched_ਗਿਣਤੀ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		sched_goidle;

	/* try_to_wake_up() stats */
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		ttwu_ਗਿਣਤੀ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		ttwu_local;
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_CPU_IDLE
	/* Must be inspected within a RCU lock section */
	ਬਣਤਰ cpuidle_state	*idle_state;
#ਜੇ_ਅੰਤ

	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		nr_pinned;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		ਧੱਕੋ_busy;
	ਬਣਤਰ cpu_stop_work	ਧੱਕੋ_work;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CORE
	/* per rq */
	ਬਣਤਰ rq		*core;
	ਬਣਤਰ task_struct	*core_pick;
	ਬਣਤਰ sched_dl_entity	*core_dl_server;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		core_enabled;
	ਬਣਤਰ rb_ਜੜ੍ਹ		core_ਰੁੱਖ;

	/* shared state -- careful with sched_core_cpu_deactivate() */
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		core_task_seq;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		core_pick_seq;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ		core_cookie;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		core_forceidle_ਗਿਣਤੀ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		core_forceidle_seq;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		core_forceidle_occupation;
	u64			core_forceidle_ਸ਼ੁਰੂ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		core_pick_ਵਿੱਚ_flight;
#ਜੇ_ਅੰਤ /* CONFIG_SCHED_CORE */

	/* Scratch cpumask to be temporarily used under rq_lock */
	cpumask_var_t		scratch_ਨਕਾਬ;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_CFS_BANDWIDTH
	call_single_ਅੰਕੜੇ_t	cfsb_csd;
	ਬਣਤਰ list_ਸਿਰ	cfsb_csd_list;
#ਜੇ_ਅੰਤ

	atomic_t		nr_iowait;
} __no_randomize_layout;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_FAIR_ਸਮੂਹ_SCHED

/* CPU runqueue to which this cfs_rq is attached */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ rq *rq_ਦਾ(ਬਣਤਰ cfs_rq *cfs_rq)
{
	ਵਾਪਸ cfs_rq->rq;
}

#ਨਹੀਂਤਾਂ /* !CONFIG_FAIR_GROUP_SCHED: */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ rq *rq_ਦਾ(ਬਣਤਰ cfs_rq *cfs_rq)
{
	ਵਾਪਸ container_ਦਾ(cfs_rq, ਬਣਤਰ rq, cfs);
}
#ਜੇ_ਅੰਤ /* !CONFIG_FAIR_GROUP_SCHED */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ cpu_ਦਾ(ਬਣਤਰ rq *rq)
{
	ਵਾਪਸ rq->cpu;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MDF_ਧੱਕੋ		0x01

ਸਥਿਰ ਸਤਰਅੰਦਰ bool is_migration_disabled(ਬਣਤਰ task_struct *p)
{
	ਵਾਪਸ p->migration_disabled;
}

DECLARE_ਪ੍ਰਤੀ_CPU_SHARED_ALIGNED(ਬਣਤਰ rq, runqueues);
DECLARE_ਪ੍ਰਤੀ_CPU(ਬਣਤਰ rnd_state, sched_rnd_state);

ਸਥਿਰ ਸਤਰਅੰਦਰ u32 sched_rng(ਖਾਲੀ)
{
	ਵਾਪਸ prandom_u32_state(this_cpu_ptr(&sched_rnd_state));
}

ਸਥਿਰ __always_inline ਬਣਤਰ rq *__this_rq(ਖਾਲੀ)
{
	ਵਾਪਸ this_cpu_ptr(&runqueues);
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cpu_rq(cpu)		(&ਪ੍ਰਤੀ_cpu(runqueues, (cpu)))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ this_rq()		__this_rq()
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ task_rq(p)		cpu_rq(task_cpu(p))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ cpu_curr(cpu)		(cpu_rq(cpu)->curr)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ raw_rq()		raw_cpu_ptr(&runqueues)

ਸਥਿਰ ਸਤਰਅੰਦਰ bool idle_rq(ਬਣਤਰ rq *rq)
{
	ਵਾਪਸ rq->curr == rq->idle && !rq->nr_running && !rq->ttwu_pending;
}

/**
 * available_idle_cpu - is a given CPU idle for enqueuing work.
 * @cpu: the CPU in question.
 *
 * Return: 1 if the CPU is currently idle. 0 otherwise.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ bool ਉਪਲਬਧ_idle_cpu(ਪੂਰਨ cpu)
{
	ਜੇ (!idle_rq(cpu_rq(cpu)))
		ਵਾਪਸ 0;

	ਜੇ (vcpu_is_preempted(cpu))
		ਵਾਪਸ 0;

	ਵਾਪਸ 1;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_PROXY_EXEC
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ rq_ਸੈੱਟ_donor(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *t)
{
	rcu_ਸੌਂਪੋ_ਸੂਚਕ(rq->donor, t);
}
#ਨਹੀਂਤਾਂ
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ rq_ਸੈੱਟ_donor(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *t)
{
	/* Do nothing */
}
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CORE
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ cpumask *sched_ਸਮੂਹ_ਵਿਸਤਾਰ(ਬਣਤਰ sched_ਸਮੂਹ *sg);

DECLARE_STATIC_ਕੁੰਜੀ_FALSE(__sched_core_enabled);

ਸਥਿਰ ਸਤਰਅੰਦਰ bool sched_core_enabled(ਬਣਤਰ rq *rq)
{
	ਵਾਪਸ static_branch_unlikely(&__sched_core_enabled) && rq->core_enabled;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool sched_core_disabled(ਖਾਲੀ)
{
	ਵਾਪਸ !static_branch_unlikely(&__sched_core_enabled);
}

/*
 * Be careful with this function; not for general use. The return value isn't
 * stable unless you actually hold a relevant rq->__lock.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ raw_spinlock_t *rq_lockp(ਬਣਤਰ rq *rq)
{
	ਜੇ (sched_core_enabled(rq))
		ਵਾਪਸ &rq->core->__lock;

	ਵਾਪਸ &rq->__lock;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ raw_spinlock_t *__rq_lockp(ਬਣਤਰ rq *rq)
	__returns_ctx_lock(rq_lockp(rq)) /* alias them */
{
	ਜੇ (rq->core_enabled)
		ਵਾਪਸ &rq->core->__lock;

	ਵਾਪਸ &rq->__lock;
}

ਬਾਹਰੀ bool
cfs_prio_less(ਅਚਲ ਬਣਤਰ task_struct *a, ਅਚਲ ਬਣਤਰ task_struct *b, bool fi);

ਬਾਹਰੀ ਖਾਲੀ task_vruntime_update(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, bool ਵਿੱਚ_fi);

/*
 * Helpers to check if the CPU's core cookie matches with the task's cookie
 * when core scheduling is enabled.
 * A special case is that the task's cookie always matches with CPU's core
 * cookie if the CPU is in an idle core.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ bool sched_cpu_cookie_ਮੇਲ(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p)
{
	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	ਜੇ (!sched_core_enabled(rq))
		ਵਾਪਸ true;

	ਵਾਪਸ rq->core->core_cookie == p->core_cookie;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool sched_core_cookie_ਮੇਲ(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p)
{
	bool idle_core = true;
	ਪੂਰਨ cpu;

	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	ਜੇ (!sched_core_enabled(rq))
		ਵਾਪਸ true;

	ਜੇ (rq->core->core_cookie == p->core_cookie)
		ਵਾਪਸ true;

	for_each_cpu(cpu, cpu_smt_ਨਕਾਬ(cpu_ਦਾ(rq))) {
		ਜੇ (!ਉਪਲਬਧ_idle_cpu(cpu)) {
			idle_core = false;
			ਰੋਕੋ;
		}
	}

	/*
	 * A CPU in an idle core is always the best choice for tasks with
	 * cookies.
	 */
	ਵਾਪਸ idle_core;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool sched_ਸਮੂਹ_cookie_ਮੇਲ(ਬਣਤਰ rq *rq,
					    ਬਣਤਰ task_struct *p,
					    ਬਣਤਰ sched_ਸਮੂਹ *ਸਮੂਹ)
{
	ਪੂਰਨ cpu;

	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	ਜੇ (!sched_core_enabled(rq))
		ਵਾਪਸ true;

	for_each_cpu_and(cpu, sched_ਸਮੂਹ_ਵਿਸਤਾਰ(ਸਮੂਹ), p->cpus_ptr) {
		ਜੇ (sched_core_cookie_ਮੇਲ(cpu_rq(cpu), p))
			ਵਾਪਸ true;
	}
	ਵਾਪਸ false;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool sched_core_enqueued(ਬਣਤਰ task_struct *p)
{
	ਵਾਪਸ !RB_ਖਾਲੀ_ਥਾਵਾਂ_ਗੰਢ(&p->core_ਗੰਢ);
}

ਬਾਹਰੀ ਖਾਲੀ sched_core_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p);
ਬਾਹਰੀ ਖਾਲੀ sched_core_ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, ਪੂਰਨ flags);

ਬਾਹਰੀ ਖਾਲੀ sched_core_get(ਖਾਲੀ);
ਬਾਹਰੀ ਖਾਲੀ sched_core_put(ਖਾਲੀ);

ਸਥਿਰ ਸਤਰਅੰਦਰ bool task_has_sched_core(ਬਣਤਰ task_struct *p)
{
	ਜੇ (sched_core_disabled())
		ਵਾਪਸ false;

	ਵਾਪਸ !!p->core_cookie;
}

#ਨਹੀਂਤਾਂ /* !CONFIG_SCHED_CORE: */

ਸਥਿਰ ਸਤਰਅੰਦਰ bool sched_core_enabled(ਬਣਤਰ rq *rq)
{
	ਵਾਪਸ false;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool sched_core_disabled(ਖਾਲੀ)
{
	ਵਾਪਸ true;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ raw_spinlock_t *rq_lockp(ਬਣਤਰ rq *rq)
{
	ਵਾਪਸ &rq->__lock;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ raw_spinlock_t *__rq_lockp(ਬਣਤਰ rq *rq)
	__returns_ctx_lock(rq_lockp(rq)) /* alias them */
{
	ਵਾਪਸ &rq->__lock;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool sched_cpu_cookie_ਮੇਲ(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p)
{
	ਵਾਪਸ true;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool sched_core_cookie_ਮੇਲ(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p)
{
	ਵਾਪਸ true;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool sched_ਸਮੂਹ_cookie_ਮੇਲ(ਬਣਤਰ rq *rq,
					    ਬਣਤਰ task_struct *p,
					    ਬਣਤਰ sched_ਸਮੂਹ *ਸਮੂਹ)
{
	ਵਾਪਸ true;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool task_has_sched_core(ਬਣਤਰ task_struct *p)
{
	ਵਾਪਸ false;
}

#ਜੇ_ਅੰਤ /* !CONFIG_SCHED_CORE */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_RT_ਸਮੂਹ_SCHED
# ifdef CONFIG_RT_ਸਮੂਹ_SCHED_DEFAULT_DISABLED
DECLARE_STATIC_ਕੁੰਜੀ_FALSE(rt_ਸਮੂਹ_sched);
ਸਥਿਰ ਸਤਰਅੰਦਰ bool rt_ਸਮੂਹ_sched_enabled(ਖਾਲੀ)
{
	ਵਾਪਸ static_branch_unlikely(&rt_ਸਮੂਹ_sched);
}
# ਨਹੀਂਤਾਂ /* !CONFIG_RT_GROUP_SCHED_DEFAULT_DISABLED: */
DECLARE_STATIC_ਕੁੰਜੀ_TRUE(rt_ਸਮੂਹ_sched);
ਸਥਿਰ ਸਤਰਅੰਦਰ bool rt_ਸਮੂਹ_sched_enabled(ਖਾਲੀ)
{
	ਵਾਪਸ static_branch_likely(&rt_ਸਮੂਹ_sched);
}
# endif /* !CONFIG_RT_GROUP_SCHED_DEFAULT_DISABLED */
#ਨਹੀਂਤਾਂ /* !CONFIG_RT_GROUP_SCHED: */
# define rt_ਸਮੂਹ_sched_enabled()	false
#ਜੇ_ਅੰਤ /* !CONFIG_RT_GROUP_SCHED */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ lockdep_assert_rq_held(ਬਣਤਰ rq *rq)
	__assumes_ctx_lock(__rq_lockp(rq))
{
	lockdep_assert_held(__rq_lockp(rq));
}

ਬਾਹਰੀ ਖਾਲੀ raw_spin_rq_ਤਾਲਾ_nested(ਬਣਤਰ rq *rq, ਪੂਰਨ subclass)
	__acquires(__rq_lockp(rq));

ਬਾਹਰੀ bool raw_spin_rq_trylock(ਬਣਤਰ rq *rq)
	__cond_acquires(true, __rq_lockp(rq));

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ raw_spin_rq_ਤਾਲਾ(ਬਣਤਰ rq *rq)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_ਤਾਲਾ_nested(rq, 0);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ raw_spin_rq_unlock(ਬਣਤਰ rq *rq)
	__releases(__rq_lockp(rq))
{
	raw_spin_unlock(rq_lockp(rq));
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ raw_spin_rq_ਤਾਲਾ_irq(ਬਣਤਰ rq *rq)
	__acquires(__rq_lockp(rq))
{
	local_irq_disable();
	raw_spin_rq_ਤਾਲਾ(rq);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ raw_spin_rq_unlock_irq(ਬਣਤਰ rq *rq)
	__releases(__rq_lockp(rq))
{
	raw_spin_rq_unlock(rq);
	local_irq_enable();
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ raw_spin_rq_ਤਾਲਾ_irqsave_2(ਬਣਤਰ rq *rq)
	__acquires(__rq_lockp(rq))
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags;

	local_irq_save(flags);
	raw_spin_rq_ਤਾਲਾ(rq);

	ਵਾਪਸ flags;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ raw_spin_rq_unlock_irqrestore(ਬਣਤਰ rq *rq, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags)
	__releases(__rq_lockp(rq))
{
	raw_spin_rq_unlock(rq);
	local_irq_restore(flags);
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ raw_spin_rq_ਤਾਲਾ_irqsave(rq, flags)	\
ਕਰੋ {						\
	flags = raw_spin_rq_ਤਾਲਾ_irqsave_2(rq);	\
} ਜਦੋਂ (0)

ਬਾਹਰੀ ਖਾਲੀ __update_idle_core(ਬਣਤਰ rq *rq);

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ update_idle_core(ਬਣਤਰ rq *rq)
{
	ਜੇ (sched_smt_ਸਰਗਰਮ())
		__update_idle_core(rq);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_FAIR_ਸਮੂਹ_SCHED
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ task_struct *task_ਦਾ(ਬਣਤਰ sched_entity *se)
{
	WARN_ON_ONCE(!entity_is_task(se));
	ਵਾਪਸ container_ਦਾ(se, ਬਣਤਰ task_struct, se);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ cfs_rq *task_cfs_rq(ਬਣਤਰ task_struct *p)
{
	ਵਾਪਸ p->se.cfs_rq;
}

/* runqueue on which this entity is (to be) queued */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ cfs_rq *cfs_rq_ਦਾ(ਅਚਲ ਬਣਤਰ sched_entity *se)
{
	ਵਾਪਸ se->cfs_rq;
}

/* runqueue "owned" by this group */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ cfs_rq *ਸਮੂਹ_cfs_rq(ਬਣਤਰ sched_entity *grp)
{
	ਵਾਪਸ grp->my_q;
}

#ਨਹੀਂਤਾਂ /* !CONFIG_FAIR_GROUP_SCHED: */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ task_ਦਾ(_se)		container_ਦਾ(_se, ਬਣਤਰ task_struct, se)

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ cfs_rq *task_cfs_rq(ਅਚਲ ਬਣਤਰ task_struct *p)
{
	ਵਾਪਸ &task_rq(p)->cfs;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ cfs_rq *cfs_rq_ਦਾ(ਅਚਲ ਬਣਤਰ sched_entity *se)
{
	ਅਚਲ ਬਣਤਰ task_struct *p = task_ਦਾ(se);
	ਬਣਤਰ rq *rq = task_rq(p);

	ਵਾਪਸ &rq->cfs;
}

/* runqueue "owned" by this group */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ cfs_rq *ਸਮੂਹ_cfs_rq(ਬਣਤਰ sched_entity *grp)
{
	ਵਾਪਸ ਨਿਰਾ;
}

#ਜੇ_ਅੰਤ /* !CONFIG_FAIR_GROUP_SCHED */

ਬਾਹਰੀ ਖਾਲੀ update_rq_avg_idle(ਬਣਤਰ rq *rq);
ਬਾਹਰੀ ਖਾਲੀ update_rq_clock(ਬਣਤਰ rq *rq);

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
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ RQCF_REQ_SKIP		0x01
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ RQCF_ACT_SKIP		0x02
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ RQCF_UPDATED		0x04

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ assert_clock_updated(ਬਣਤਰ rq *rq)
{
	/*
	 * The only reason for not seeing a clock update since the
	 * last rq_pin_lock() is if we're currently skipping updates.
	 */
	WARN_ON_ONCE(rq->clock_update_flags < RQCF_ACT_SKIP);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ u64 rq_clock(ਬਣਤਰ rq *rq)
{
	lockdep_assert_rq_held(rq);
	assert_clock_updated(rq);

	ਵਾਪਸ rq->clock;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ u64 rq_clock_task(ਬਣਤਰ rq *rq)
{
	lockdep_assert_rq_held(rq);
	assert_clock_updated(rq);

	ਵਾਪਸ rq->clock_task;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ rq_clock_skip_update(ਬਣਤਰ rq *rq)
{
	lockdep_assert_rq_held(rq);
	rq->clock_update_flags |= RQCF_REQ_SKIP;
}

/*
 * See rt task throttling, which is the only time a skip
 * request is canceled.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ rq_clock_cancel_skipupdate(ਬਣਤਰ rq *rq)
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
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ rq_clock_ਸ਼ੁਰੂ_loop_update(ਬਣਤਰ rq *rq)
{
	lockdep_assert_rq_held(rq);
	WARN_ON_ONCE(rq->clock_update_flags & RQCF_ACT_SKIP);
	rq->clock_update_flags |= RQCF_ACT_SKIP;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ rq_clock_stop_loop_update(ਬਣਤਰ rq *rq)
{
	lockdep_assert_rq_held(rq);
	rq->clock_update_flags &= ~RQCF_ACT_SKIP;
}

ਬਣਤਰ rq_flags {
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags;
	ਬਣਤਰ pin_cookie cookie;
	/*
	 * A copy of (rq::clock_update_flags & RQCF_UPDATED) for the
	 * current pin context is stashed here in case it needs to be
	 * restored in rq_repin_lock().
	 */
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ clock_update_flags;
};

ਬਾਹਰੀ ਬਣਤਰ balance_callback balance_ਧੱਕੋ_callback;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CLASS_EXT
ਬਾਹਰੀ ਅਚਲ ਬਣਤਰ sched_class ext_sched_class;

DECLARE_STATIC_ਕੁੰਜੀ_FALSE(__scx_enabled);	/* SCX BPF scheduler loaded */
DECLARE_STATIC_ਕੁੰਜੀ_FALSE(__scx_switched_all);	/* all fair class tasks on SCX */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ scx_enabled()		static_branch_unlikely(&__scx_enabled)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ scx_switched_all()	static_branch_unlikely(&__scx_switched_all)

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ scx_rq_clock_update(ਬਣਤਰ rq *rq, u64 clock)
{
	ਜੇ (!scx_enabled())
		ਵਾਪਸ;
	WRITE_ONCE(rq->scx.clock, clock);
	smp_store_release(&rq->scx.flags, rq->scx.flags | SCX_RQ_CLK_VALID);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ scx_rq_clock_invalidate(ਬਣਤਰ rq *rq)
{
	ਜੇ (!scx_enabled())
		ਵਾਪਸ;
	WRITE_ONCE(rq->scx.flags, rq->scx.flags & ~SCX_RQ_CLK_VALID);
}

#ਨਹੀਂਤਾਂ /* !CONFIG_SCHED_CLASS_EXT: */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ scx_enabled()		false
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ scx_switched_all()	false

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ scx_rq_clock_update(ਬਣਤਰ rq *rq, u64 clock) {}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ scx_rq_clock_invalidate(ਬਣਤਰ rq *rq) {}
#ਜੇ_ਅੰਤ /* !CONFIG_SCHED_CLASS_EXT */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ assert_balance_callbacks_ਖਾਲੀ_ਥਾਵਾਂ(ਬਣਤਰ rq *rq)
{
	WARN_ON_ONCE(IS_ENABLED(CONFIG_PROVE_LOCKING) &&
		     rq->balance_callback &&
		     rq->balance_callback != &balance_ਧੱਕੋ_callback);
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
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ rq_pin_ਤਾਲਾ(ਬਣਤਰ rq *rq, ਬਣਤਰ rq_flags *rf)
{
	rf->cookie = lockdep_pin_ਤਾਲਾ(__rq_lockp(rq));

	rq->clock_update_flags &= (RQCF_REQ_SKIP|RQCF_ACT_SKIP);
	rf->clock_update_flags = 0;
	assert_balance_callbacks_ਖਾਲੀ_ਥਾਵਾਂ(rq);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ rq_unpin_ਤਾਲਾ(ਬਣਤਰ rq *rq, ਬਣਤਰ rq_flags *rf)
{
	ਜੇ (rq->clock_update_flags > RQCF_ACT_SKIP)
		rf->clock_update_flags = RQCF_UPDATED;

	scx_rq_clock_invalidate(rq);
	lockdep_unpin_ਤਾਲਾ(__rq_lockp(rq), rf->cookie);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ rq_repin_ਤਾਲਾ(ਬਣਤਰ rq *rq, ਬਣਤਰ rq_flags *rf)
{
	lockdep_repin_ਤਾਲਾ(__rq_lockp(rq), rf->cookie);

	/*
	 * Restore the value we stashed in @rf for this pin context.
	 */
	rq->clock_update_flags |= rf->clock_update_flags;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __task_rq_lock(...) __acquire_ret(___task_rq_lock(__VA_ARGS__), __rq_lockp(__ret))
ਬਾਹਰੀ ਬਣਤਰ rq *___task_rq_lock(ਬਣਤਰ task_struct *p, ਬਣਤਰ rq_flags *rf) __acquires_ret;

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ task_rq_ਤਾਲਾ_2(...) __acquire_ret(task_rq_ਤਾਲਾ(__VA_ARGS__), __rq_lockp(__ret))
ਬਾਹਰੀ ਬਣਤਰ rq *task_rq_ਤਾਲਾ(ਬਣਤਰ task_struct *p, ਬਣਤਰ rq_flags *rf)
	__acquires(&p->pi_ਤਾਲਾ) __acquires_ret;

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
__task_rq_unlock(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, ਬਣਤਰ rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_ਤਾਲਾ(rq, rf);
	raw_spin_rq_unlock(rq);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
task_rq_unlock(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, ਬਣਤਰ rq_flags *rf)
	__releases(__rq_lockp(rq), &p->pi_ਤਾਲਾ)
{
	__task_rq_unlock(rq, p, rf);
	raw_spin_unlock_irqrestore(&p->pi_ਤਾਲਾ, rf->flags);
}

DEFINE_ਤਾਲਾ_GUARD_1(task_rq_ਤਾਲਾ_2, ਬਣਤਰ task_struct,
		    _T->rq = task_rq_ਤਾਲਾ_2(_T->ਤਾਲਾ, &_T->rf),
		    task_rq_unlock(_T->rq, _T->ਤਾਲਾ, &_T->rf),
		    ਬਣਤਰ rq *rq; ਬਣਤਰ rq_flags rf)
DECLARE_ਤਾਲਾ_GUARD_1_ATTRS(task_rq_ਤਾਲਾ_2, __acquires(_T->pi_ਤਾਲਾ), __releases((*(ਬਣਤਰ task_struct **)_T)->pi_ਤਾਲਾ))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ class_task_rq_ਤਾਲਾ_constructor(_T) WITH_ਤਾਲਾ_GUARD_1_ATTRS(task_rq_ਤਾਲਾ_2, _T)

DEFINE_ਤਾਲਾ_GUARD_1(__task_rq_lock, ਬਣਤਰ task_struct,
		    _T->rq = __task_rq_lock(_T->ਤਾਲਾ, &_T->rf),
		    __task_rq_unlock(_T->rq, _T->ਤਾਲਾ, &_T->rf),
		    ਬਣਤਰ rq *rq; ਬਣਤਰ rq_flags rf)

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ rq_ਤਾਲਾ_irqsave(ਬਣਤਰ rq *rq, ਬਣਤਰ rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_ਤਾਲਾ_irqsave(rq, rf->flags);
	rq_pin_ਤਾਲਾ(rq, rf);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ rq_ਤਾਲਾ_irq(ਬਣਤਰ rq *rq, ਬਣਤਰ rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_ਤਾਲਾ_irq(rq);
	rq_pin_ਤਾਲਾ(rq, rf);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ rq_ਤਾਲਾ(ਬਣਤਰ rq *rq, ਬਣਤਰ rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_ਤਾਲਾ(rq);
	rq_pin_ਤਾਲਾ(rq, rf);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ rq_unlock_irqrestore(ਬਣਤਰ rq *rq, ਬਣਤਰ rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_ਤਾਲਾ(rq, rf);
	raw_spin_rq_unlock_irqrestore(rq, rf->flags);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ rq_unlock_irq(ਬਣਤਰ rq *rq, ਬਣਤਰ rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_ਤਾਲਾ(rq, rf);
	raw_spin_rq_unlock_irq(rq);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ rq_unlock(ਬਣਤਰ rq *rq, ਬਣਤਰ rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_ਤਾਲਾ(rq, rf);
	raw_spin_rq_unlock(rq);
}

DEFINE_ਤਾਲਾ_GUARD_1(rq_ਤਾਲਾ, ਬਣਤਰ rq,
		    rq_ਤਾਲਾ(_T->ਤਾਲਾ, &_T->rf),
		    rq_unlock(_T->ਤਾਲਾ, &_T->rf),
		    ਬਣਤਰ rq_flags rf)

DECLARE_ਤਾਲਾ_GUARD_1_ATTRS(rq_ਤਾਲਾ, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(ਬਣਤਰ rq **)_T)));
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ class_rq_ਤਾਲਾ_constructor(_T) WITH_ਤਾਲਾ_GUARD_1_ATTRS(rq_ਤਾਲਾ, _T)

DEFINE_ਤਾਲਾ_GUARD_1(rq_ਤਾਲਾ_irq, ਬਣਤਰ rq,
		    rq_ਤਾਲਾ_irq(_T->ਤਾਲਾ, &_T->rf),
		    rq_unlock_irq(_T->ਤਾਲਾ, &_T->rf),
		    ਬਣਤਰ rq_flags rf)

DECLARE_ਤਾਲਾ_GUARD_1_ATTRS(rq_ਤਾਲਾ_irq, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(ਬਣਤਰ rq **)_T)));
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ class_rq_ਤਾਲਾ_irq_constructor(_T) WITH_ਤਾਲਾ_GUARD_1_ATTRS(rq_ਤਾਲਾ_irq, _T)

DEFINE_ਤਾਲਾ_GUARD_1(rq_ਤਾਲਾ_irqsave, ਬਣਤਰ rq,
		    rq_ਤਾਲਾ_irqsave(_T->ਤਾਲਾ, &_T->rf),
		    rq_unlock_irqrestore(_T->ਤਾਲਾ, &_T->rf),
		    ਬਣਤਰ rq_flags rf)

DECLARE_ਤਾਲਾ_GUARD_1_ATTRS(rq_ਤਾਲਾ_irqsave, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(ਬਣਤਰ rq **)_T)));
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ class_rq_ਤਾਲਾ_irqsave_constructor(_T) WITH_ਤਾਲਾ_GUARD_1_ATTRS(rq_ਤਾਲਾ_irqsave, _T)

DEFINE_ਤਾਲਾ_GUARD_1(raw_spin_rq_ਤਾਲਾ_irqsave, ਬਣਤਰ rq,
		    raw_spin_rq_ਤਾਲਾ_irqsave(_T->ਤਾਲਾ, _T->flags),
		    raw_spin_rq_unlock_irqrestore(_T->ਤਾਲਾ, _T->flags),
		    ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags)

DECLARE_ਤਾਲਾ_GUARD_1_ATTRS(raw_spin_rq_ਤਾਲਾ_irqsave, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(ਬਣਤਰ rq **)_T)));
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ class_raw_spin_rq_ਤਾਲਾ_irqsave_constructor(_T) \
	WITH_ਤਾਲਾ_GUARD_1_ATTRS(raw_spin_rq_ਤਾਲਾ_irqsave, _T)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ this_rq_ਤਾਲਾ_irq(...) __acquire_ret(this_rq_ਤਾਲਾ_irq_2(__VA_ARGS__), __rq_lockp(__ret))
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ rq *this_rq_ਤਾਲਾ_irq_2(ਬਣਤਰ rq_flags *rf) __acquires_ret
{
	ਬਣਤਰ rq *rq;

	local_irq_disable();
	rq = this_rq();
	rq_ਤਾਲਾ(rq, rf);

	ਵਾਪਸ rq;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_NUMA

ਗਿਣਤੀ numa_topology_type {
	NUMA_ਸਿੱਧਾ,
	NUMA_GLUELESS_MESH,
	NUMA_BACKPLANE,
};

ਬਾਹਰੀ ਗਿਣਤੀ numa_topology_type sched_numa_topology_type;
ਬਾਹਰੀ ਪੂਰਨ sched_max_numa_ਦੂਰੀ;
ਬਾਹਰੀ bool ਲੱਭੋ_numa_ਦੂਰੀ(ਪੂਰਨ ਦੂਰੀ);
ਬਾਹਰੀ ਖਾਲੀ sched_init_numa(ਪੂਰਨ offline_ਗੰਢ);
ਬਾਹਰੀ ਖਾਲੀ sched_update_numa(ਪੂਰਨ cpu, bool online);
ਬਾਹਰੀ ਖਾਲੀ sched_domains_numa_masks_ਸੈੱਟ(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cpu);
ਬਾਹਰੀ ਖਾਲੀ sched_domains_numa_masks_clear(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cpu);
ਬਾਹਰੀ ਪੂਰਨ sched_numa_ਲੱਭੋ_closest(ਅਚਲ ਬਣਤਰ cpumask *cpus, ਪੂਰਨ cpu);

#ਨਹੀਂਤਾਂ /* !CONFIG_NUMA: */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ sched_init_numa(ਪੂਰਨ offline_ਗੰਢ) { }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ sched_update_numa(ਪੂਰਨ cpu, bool online) { }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ sched_domains_numa_masks_ਸੈੱਟ(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cpu) { }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ sched_domains_numa_masks_clear(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cpu) { }

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ sched_numa_ਲੱਭੋ_closest(ਅਚਲ ਬਣਤਰ cpumask *cpus, ਪੂਰਨ cpu)
{
	ਵਾਪਸ nr_cpu_ids;
}

#ਜੇ_ਅੰਤ /* !CONFIG_NUMA */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_NUMA_BALANCING

/* The regions in numa_faults array from task_struct */
ਗਿਣਤੀ numa_ਨੁਕਸ_stats {
	NUMA_MEM = 0,
	NUMA_CPU,
	NUMA_MEMBUF,
	NUMA_CPUBUF
};

ਬਾਹਰੀ ਖਾਲੀ sched_setnuma(ਬਣਤਰ task_struct *p, ਪੂਰਨ ਗੰਢ);
ਬਾਹਰੀ ਪੂਰਨ migrate_task_ਮੰਜ਼ਿਲ(ਬਣਤਰ task_struct *p, ਪੂਰਨ cpu);
ਬਾਹਰੀ ਪੂਰਨ migrate_ਅਦਲਾ_ਬਦਲੀ(ਬਣਤਰ task_struct *p, ਬਣਤਰ task_struct *t,
			ਪੂਰਨ cpu, ਪੂਰਨ scpu);
ਬਾਹਰੀ ਖਾਲੀ init_numa_balancing(u64 clone_flags, ਬਣਤਰ task_struct *p);

#ਨਹੀਂਤਾਂ /* !CONFIG_NUMA_BALANCING: */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
init_numa_balancing(u64 clone_flags, ਬਣਤਰ task_struct *p)
{
}

#ਜੇ_ਅੰਤ /* !CONFIG_NUMA_BALANCING */

ਪੂਰਨ task_llc(ਅਚਲ ਬਣਤਰ task_struct *p);

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
ਕਤਾਰ_balance_callback(ਬਣਤਰ rq *rq,
		       ਬਣਤਰ balance_callback *ਸਿਰ,
		       ਖਾਲੀ (*func)(ਬਣਤਰ rq *rq))
{
	lockdep_assert_rq_held(rq);

	/*
	 * Don't (re)queue an already queued item; nor queue anything when
	 * balance_push() is active, see the comment with
	 * balance_push_callback.
	 */
	ਜੇ (unlikely(ਸਿਰ->ਅਗਲਾ || rq->balance_callback == &balance_ਧੱਕੋ_callback))
		ਵਾਪਸ;

	ਸਿਰ->func = func;
	ਸਿਰ->ਅਗਲਾ = rq->balance_callback;
	rq->balance_callback = ਸਿਰ;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ rcu_dereference_sched_domain(p) \
	rcu_dereference_all_check((p), lockdep_is_held(&sched_domains_mutex))

/*
 * The domain tree (rq->sd) is protected by RCU's quiescent state transition.
 * See destroy_sched_domains: call_rcu for details.
 *
 * The domain tree of any CPU may only be accessed from within
 * preempt-disabled sections.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ for_each_domain(cpu, __sd) \
	ਲਈ (__sd = rcu_dereference_sched_domain(cpu_rq(cpu)->sd); \
			__sd; __sd = __sd->ਮਾਪਾ)

/* A mask of all the SD flags that have the SDF_SHARED_CHILD metaflag */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SD_FLAG(name, mflags) (name * !!((mflags) & SDF_SHARED_ਬੱਚਾ)) |
ਸਥਿਰ ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ SD_SHARED_ਬੱਚਾ_ਨਕਾਬ =
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/sd_flags.h>
0;
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ SD_FLAG

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
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ sched_domain *highest_flag_domain(ਪੂਰਨ cpu, ਪੂਰਨ flag)
{
	ਬਣਤਰ sched_domain *sd, *hsd = ਨਿਰਾ;

	for_each_domain(cpu, sd) {
		ਜੇ (sd->flags & flag) {
			hsd = sd;
			ਜਾਰੀ;
		}

		/*
		 * Stop the search if @flag is known to be shared at lower
		 * levels. It will not be found further up.
		 */
		ਜੇ (flag & SD_SHARED_ਬੱਚਾ_ਨਕਾਬ)
			ਰੋਕੋ;
	}

	ਵਾਪਸ hsd;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ sched_domain *ਸਭ_ਤੋਂ_ਨੀਵਾਂ_flag_domain(ਪੂਰਨ cpu, ਪੂਰਨ flag)
{
	ਬਣਤਰ sched_domain *sd;

	for_each_domain(cpu, sd) {
		ਜੇ (sd->flags & flag)
			ਰੋਕੋ;
	}

	ਵਾਪਸ sd;
}

DECLARE_ਪ੍ਰਤੀ_CPU(ਬਣਤਰ sched_domain __rcu *, sd_llc);
DECLARE_ਪ੍ਰਤੀ_CPU(ਪੂਰਨ, sd_llc_ਮਾਪ);
DECLARE_ਪ੍ਰਤੀ_CPU(ਪੂਰਨ, sd_llc_id);
DECLARE_ਪ੍ਰਤੀ_CPU(ਪੂਰਨ, sd_share_id);
DECLARE_ਪ੍ਰਤੀ_CPU(ਬਣਤਰ sched_domain_shared __rcu *, sd_llc_shared);
DECLARE_ਪ੍ਰਤੀ_CPU(ਬਣਤਰ sched_domain_shared __rcu *, sd_balance_shared);
DECLARE_ਪ੍ਰਤੀ_CPU(ਬਣਤਰ sched_domain __rcu *, sd_numa);
DECLARE_ਪ੍ਰਤੀ_CPU(ਬਣਤਰ sched_domain __rcu *, sd_asym_packing);
DECLARE_ਪ੍ਰਤੀ_CPU(ਬਣਤਰ sched_domain __rcu *, sd_asym_cpucapacity);

ਬਾਹਰੀ ਬਣਤਰ static_ਕੁੰਜੀ_false sched_asym_cpucapacity;
ਬਾਹਰੀ ਬਣਤਰ static_ਕੁੰਜੀ_false sched_cluster_ਸਰਗਰਮ;

ਸਥਿਰ __always_inline bool sched_asym_cpucap_ਸਰਗਰਮ(ਖਾਲੀ)
{
	ਵਾਪਸ static_branch_unlikely(&sched_asym_cpucapacity);
}

ਬਣਤਰ sched_ਸਮੂਹ_ਸਮਰੱਥਾ {
	atomic_t		ref;
	/*
	 * CPU capacity of this group, SCHED_CAPACITY_SCALE being max capacity
	 * for a single CPU.
	 */
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ		ਸਮਰੱਥਾ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ		min_ਸਮਰੱਥਾ;		/* Min per-CPU capacity in group */
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ		max_ਸਮਰੱਥਾ;		/* Max per-CPU capacity in group */
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ		ਅਗਲਾ_update;
	ਪੂਰਨ			imbalance;		/* XXX unrelated to capacity but shared group state */

	ਪੂਰਨ			id;

	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ		cpumask[];		/* Balance mask */
};

ਬਣਤਰ sched_ਸਮੂਹ {
	ਬਣਤਰ sched_ਸਮੂਹ	*ਅਗਲਾ;			/* Must be a circular list */
	atomic_t		ref;

	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		ਸਮੂਹ_ਭਾਰ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		cores;
	ਬਣਤਰ sched_ਸਮੂਹ_ਸਮਰੱਥਾ *sgc;
	ਪੂਰਨ			asym_prefer_cpu;	/* CPU of highest priority in group */
	ਪੂਰਨ			flags;

	/*
	 * The CPUs this group covers.
	 *
	 * NOTE: this field is variable length. (Allocated dynamically
	 * by attaching extra space to the end of the structure,
	 * depending on how many CPUs the kernel has booted up with)
	 */
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ		cpumask[];
};

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ cpumask *sched_ਸਮੂਹ_ਵਿਸਤਾਰ(ਬਣਤਰ sched_ਸਮੂਹ *sg)
{
	ਵਾਪਸ ਮੰਜ਼ਿਲ_cpumask(sg->cpumask);
}

/*
 * See build_balance_mask().
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ cpumask *ਸਮੂਹ_balance_ਨਕਾਬ(ਬਣਤਰ sched_ਸਮੂਹ *sg)
{
	ਵਾਪਸ ਮੰਜ਼ਿਲ_cpumask(sg->sgc->cpumask);
}

ਬਾਹਰੀ ਪੂਰਨ ਸਮੂਹ_balance_cpu(ਬਣਤਰ sched_ਸਮੂਹ *sg);

ਬਾਹਰੀ ਖਾਲੀ update_sched_domain_debugfs(ਖਾਲੀ);
ਬਾਹਰੀ ਖਾਲੀ dirty_sched_domain_sysctl(ਪੂਰਨ cpu);

ਬਾਹਰੀ ਪੂਰਨ sched_update_scaling(ਖਾਲੀ);

ਸਥਿਰ ਸਤਰਅੰਦਰ ਅਚਲ ਬਣਤਰ cpumask *task_user_cpus(ਬਣਤਰ task_struct *p)
{
	ਜੇ (!p->user_cpus_ptr)
		ਵਾਪਸ cpu_possible_ਨਕਾਬ; /* &init_task.cpus_mask */
	ਵਾਪਸ p->user_cpus_ptr;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_CGROUP_SCHED

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
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ task_ਸਮੂਹ *task_ਸਮੂਹ(ਬਣਤਰ task_struct *p)
{
	ਵਾਪਸ p->sched_task_ਸਮੂਹ;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_FAIR_ਸਮੂਹ_SCHED
/*
 * Defined here to be available before stats.h is included, since
 * stats.h has dependencies on things defined later in this file.
 */
ਬਣਤਰ cfs_tg_state {
	ਬਣਤਰ cfs_rq		cfs_rq;
	ਬਣਤਰ sched_entity	se;
	ਬਣਤਰ sched_statistics	stats;
} __no_randomize_layout;

/* Access a specific CPU's cfs_rq from a task group */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ cfs_rq *tg_cfs_rq(ਬਣਤਰ task_ਸਮੂਹ *tg, ਪੂਰਨ cpu)
{
	ਵਾਪਸ ਪ੍ਰਤੀ_cpu_ptr(tg->cfs_rq, cpu);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ sched_entity *tg_se(ਬਣਤਰ task_ਸਮੂਹ *tg, ਪੂਰਨ cpu)
{
	ਬਣਤਰ cfs_tg_state *state;

	ਜੇ (is_ਜੜ੍ਹ_task_ਸਮੂਹ(tg))
		ਵਾਪਸ ਨਿਰਾ;

	state = container_ਦਾ(tg_cfs_rq(tg, cpu), ਬਣਤਰ cfs_tg_state, cfs_rq);
	ਵਾਪਸ &state->se;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ sched_entity *cfs_rq_se(ਬਣਤਰ cfs_rq *cfs_rq)
{
	ਬਣਤਰ cfs_tg_state *state;

	ਜੇ (is_ਜੜ੍ਹ_task_ਸਮੂਹ(cfs_rq->tg))
		ਵਾਪਸ ਨਿਰਾ;

	state = container_ਦਾ(cfs_rq, ਬਣਤਰ cfs_tg_state, cfs_rq);
	ਵਾਪਸ &state->se;
}
#ਜੇ_ਅੰਤ

/* Change a task's cfs_rq and parent entity if it moves across CPUs/groups */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ ਸੈੱਟ_task_rq(ਬਣਤਰ task_struct *p, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cpu)
{
#ਜੇ defined(CONFIG_FAIR_ਸਮੂਹ_SCHED) || defined(CONFIG_RT_ਸਮੂਹ_SCHED)
	ਬਣਤਰ task_ਸਮੂਹ *tg = task_ਸਮੂਹ(p);
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_FAIR_ਸਮੂਹ_SCHED
	ਸੈੱਟ_task_rq_fair(&p->se, p->se.cfs_rq, tg_cfs_rq(tg, cpu));
	p->se.cfs_rq = tg_cfs_rq(tg, cpu);
	p->se.ਮਾਪਾ = tg_se(tg, cpu);
	p->se.ਡੂੰਘਾਈ = p->se.ਮਾਪਾ ? p->se.ਮਾਪਾ->ਡੂੰਘਾਈ + 1 : 0;
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_RT_ਸਮੂਹ_SCHED
	/*
	 * p->rt.rt_rq is NULL initially and it is easier to assign
	 * root_task_group's rt_rq than switching in rt_rq_of_se()
	 * Clobbers tg(!)
	 */
	ਜੇ (!rt_ਸਮੂਹ_sched_enabled())
		tg = &ਜੜ੍ਹ_task_ਸਮੂਹ;
	p->rt.rt_rq  = tg->rt_rq[cpu];
	p->rt.ਮਾਪਾ = tg->rt_se[cpu];
#ਜੇ_ਅੰਤ /* CONFIG_RT_GROUP_SCHED */
}

#ਨਹੀਂਤਾਂ /* !CONFIG_CGROUP_SCHED: */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ ਸੈੱਟ_task_rq(ਬਣਤਰ task_struct *p, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cpu) { }

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ task_ਸਮੂਹ *task_ਸਮੂਹ(ਬਣਤਰ task_struct *p)
{
	ਵਾਪਸ ਨਿਰਾ;
}

#ਜੇ_ਅੰਤ /* !CONFIG_CGROUP_SCHED */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ __set_task_cpu(ਬਣਤਰ task_struct *p, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cpu)
{
	ਸੈੱਟ_task_rq(p, cpu);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SMP
	/*
	 * After ->cpu is set up to a new value, task_rq_lock(p, ...) can be
	 * successfully executed on another CPU. We must ensure that updates of
	 * per-task data have been completed by this moment.
	 */
	smp_wmb();
	WRITE_ONCE(task_thread_info(p)->cpu, cpu);
	p->wake_cpu = cpu;
	rseq_sched_ਸੈੱਟ_ids_ਬਦਲਿਆ(p);
#ਜੇ_ਅੰਤ /* CONFIG_SMP */
}

/*
 * Tunables:
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SCHED_FEAT(name, enabled)	\
	__SCHED_FEAT_##name ,

ਗਿਣਤੀ {
#ਅਨ੍ਤਰ੍ਭਾਵ "features.h"
	__SCHED_FEAT_NR,
};

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ SCHED_FEAT

/*
 * To support run-time toggling of sched features, all the translation units
 * (but core.c) reference the sysctl_sched_features defined in core.c.
 */
ਬਾਹਰੀ __read_mostly ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ sysctl_sched_features;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_JUMP_LABEL

ਸੰਘ sched_feat_ਕੁੰਜੀ {
	ਬਣਤਰ static_ਕੁੰਜੀ_true	ਕੁੰਜੀ_true;
	ਬਣਤਰ static_ਕੁੰਜੀ_false	ਕੁੰਜੀ_false;
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ sched_feat_branch_true(ਕੁੰਜੀ)	static_branch_likely(&(ਕੁੰਜੀ)->ਕੁੰਜੀ_true)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ sched_feat_branch_false(ਕੁੰਜੀ)	static_branch_unlikely(&(ਕੁੰਜੀ)->ਕੁੰਜੀ_false)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SCHED_FEAT(name, enabled)			\
ਸਥਿਰ __always_inline bool				\
static_branch_##name(ਸੰਘ sched_feat_ਕੁੰਜੀ *ਕੁੰਜੀ) 	\
{							\
	ਵਾਪਸ sched_feat_branch_##enabled(ਕੁੰਜੀ);	\
}

#ਅਨ੍ਤਰ੍ਭਾਵ "features.h"
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ SCHED_FEAT

ਬਾਹਰੀ ਸੰਘ sched_feat_ਕੁੰਜੀ sched_feat_ਕੁੰਜੀਆਂ[__SCHED_FEAT_NR];
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ sched_feat(x) (static_branch_##x(&sched_feat_ਕੁੰਜੀਆਂ[__SCHED_FEAT_##x]))

#ਨਹੀਂਤਾਂ /* !CONFIG_JUMP_LABEL: */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ sched_feat(x) (sysctl_sched_features & (1UL << __SCHED_FEAT_##x))

#ਜੇ_ਅੰਤ /* !CONFIG_JUMP_LABEL */

ਬਾਹਰੀ ਬਣਤਰ static_ਕੁੰਜੀ_false sched_numa_balancing;
ਬਾਹਰੀ ਬਣਤਰ static_ਕੁੰਜੀ_false sched_schedstats;

ਸਥਿਰ ਸਤਰਅੰਦਰ u64 global_rt_period(ਖਾਲੀ)
{
	ਵਾਪਸ (u64)sysctl_sched_rt_period * NSEC_ਪ੍ਰਤੀ_USEC;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ u64 global_rt_runtime(ਖਾਲੀ)
{
	ਜੇ (sysctl_sched_rt_runtime < 0)
		ਵਾਪਸ RUNTIME_INF;

	ਵਾਪਸ (u64)sysctl_sched_rt_runtime * NSEC_ਪ੍ਰਤੀ_USEC;
}

/*
 * Is p the current execution context?
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ task_current(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p)
{
	ਵਾਪਸ rq->curr == p;
}

/*
 * Is p the current scheduling context?
 *
 * Note that it might be the current execution context at the same time if
 * rq->curr == rq->donor == p.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ task_current_donor(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p)
{
	ਵਾਪਸ rq->donor == p;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool task_is_blocked(ਬਣਤਰ task_struct *p)
{
	ਜੇ (!sched_proxy_exec())
		ਵਾਪਸ false;

	ਵਾਪਸ !!p->blocked_on;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_PROXY_EXEC
ਖਾਲੀ sched_proxy_ਖੰਡ_task(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p);
#ਨਹੀਂਤਾਂ
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ sched_proxy_ਖੰਡ_task(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p) {}
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ task_on_cpu(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p)
{
	ਵਾਪਸ p->on_cpu;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ task_on_rq_queued(ਬਣਤਰ task_struct *p)
{
	ਵਾਪਸ READ_ONCE(p->on_rq) == TASK_ON_RQ_QUEUED;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ task_on_rq_migrating(ਬਣਤਰ task_struct *p)
{
	ਵਾਪਸ READ_ONCE(p->on_rq) == TASK_ON_RQ_MIGRATING;
}

/* Wake flags. The first three directly map to some SD flag value */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ WF_EXEC			0x02 /* Wakeup after exec; maps to SD_BALANCE_EXEC */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ WF_FORK			0x04 /* Wakeup after fork; maps to SD_BALANCE_FORK */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ WF_TTWU			0x08 /* Wakeup;            maps to SD_BALANCE_WAKE */
/*
 * Hint that the caller expects the waker to sleep soon.
 * Scheduler classes may use it for placement or preemption.
 * Callers must not rely on it to prevent migration,
 * preserve CPU locality or make the wakee run next.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ WF_SYNC			0x10
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ WF_MIGRATED		0x20 /* Internal use, task got migrated */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ WF_CURRENT_CPU		0x40 /* Prefer to move the wakee to the current CPU. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ WF_RQ_SELECTED		0x80 /* ->select_task_rq() was called */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ WF_TTWU_RQ		0x100 /* Wakeup completed through ttwu_runnable() */

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

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਭਾਰ_IDLEPRIO		3
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ WMULT_IDLEPRIO		1431655765

ਬਾਹਰੀ ਅਚਲ ਪੂਰਨ		sched_prio_ਮੰਜ਼ਿਲ_ਭਾਰ[40];
ਬਾਹਰੀ ਅਚਲ u32		sched_prio_ਮੰਜ਼ਿਲ_wmult[40];

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

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_SLEEP		0x0001 /* Matches ENQUEUE_WAKEUP */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_SAVE		0x0002 /* Matches ENQUEUE_RESTORE */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_MOVE		0x0004 /* Matches ENQUEUE_MOVE */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_NOCLOCK		0x0008 /* Matches ENQUEUE_NOCLOCK */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_MIGRATING	0x0010 /* Matches ENQUEUE_MIGRATING */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_DELAYED		0x0020 /* Matches ENQUEUE_DELAYED */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_CLASS		0x0040 /* Matches ENQUEUE_CLASS */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_SPECIAL		0x00010000
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_THROTTLE	0x00020000

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_WAKEUP		0x0001
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_RESTORE		0x0002
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_MOVE		0x0004
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_NOCLOCK		0x0008

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_MIGRATING	0x0010
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_DELAYED		0x0020
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_CLASS		0x0040

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_ਸਿਰ		0x00010000
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_REPLENISH	0x00020000
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_MIGRATED	0x00040000
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_INITIAL		0x00080000
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_RQ_SELECTED	0x00100000
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_QUEUED		0x00200000

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ RETRY_TASK		((ਖਾਲੀ *)-1UL)

ਬਣਤਰ affinity_context {
	ਅਚਲ ਬਣਤਰ cpumask	*new_ਨਕਾਬ;
	ਬਣਤਰ cpumask		*user_ਨਕਾਬ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		flags;
};

ਬਾਹਰੀ s64 update_curr_common(ਬਣਤਰ rq *rq);

ਗਿਣਤੀ snt_e {
	SNT_ਸਾਧਾਰਨ,	/* set_next_task() */
	SNT_PICK,	/* put_prev_set_next_task(): prev != next */
	SNT_REPICK,	/* put_prev_set_next_task(): prev == next */
};

ਬਣਤਰ sched_class {

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_UCLAMP_TASK
	ਪੂਰਨ uclamp_enabled;
#ਜੇ_ਅੰਤ

	/*
	 * move_queued_task/activate_task/enqueue_task: rq->lock
	 * ttwu_do_activate/activate_task/enqueue_task: rq->lock
	 * wake_up_new_task/activate_task/enqueue_task: task_rq_lock
	 * ttwu_runnable/enqueue_task: task_rq_lock
	 * proxy_task_current: rq->lock
	 * sched_change_end
	 */
	ਖਾਲੀ (*ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_task) (ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, ਪੂਰਨ flags);
	/*
	 * move_queued_task/deactivate_task/dequeue_task: rq->lock
	 * __schedule/block_task/dequeue_task: rq->lock
	 * proxy_task_current: rq->lock
	 * wait_task_inactive: task_rq_lock
	 * sched_change_begin
	 */
	bool (*ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_task) (ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, ਪੂਰਨ flags);

	/*
	 * do_sched_yield: rq->lock
	 */
	ਖਾਲੀ (*yield_task)   (ਬਣਤਰ rq *rq);
	/*
	 * yield_to: rq->lock (double)
	 */
	bool (*yield_ਮੰਜ਼ਿਲ_task)(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p);

	/*
	 * move_queued_task: rq->lock
	 * __migrate_swap_task: rq->lock
	 * ttwu_do_activate: rq->lock
	 * ttwu_runnable: task_rq_lock
	 * wake_up_new_task: task_rq_lock
	 */
	ਖਾਲੀ (*wakeup_preempt)(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, ਪੂਰਨ flags);

	/*
	 * schedule/pick_next_task/prev_balance: rq->lock
	 */
	ਪੂਰਨ (*balance)(ਬਣਤਰ rq *rq, ਬਣਤਰ rq_flags *rf);

	/*
	 * schedule/pick_next_task: rq->lock
	 */
	ਬਣਤਰ task_struct *(*pick_task)(ਬਣਤਰ rq *rq, ਬਣਤਰ rq_flags *rf);

	/*
	 * sched_change:
	 * __schedule: rq->lock
	 */
	ਖਾਲੀ (*put_prev_task)(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, ਬਣਤਰ task_struct *ਅਗਲਾ);
	ਖਾਲੀ (*ਸੈੱਟ_ਅਗਲਾ_task)(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, ਗਿਣਤੀ snt_e type);

	/*
	 * select_task_rq: p->pi_lock
	 * sched_exec: p->pi_lock
	 */
	ਪੂਰਨ  (*select_task_rq)(ਬਣਤਰ task_struct *p, ਪੂਰਨ task_cpu, ਪੂਰਨ flags);

	/*
	 * set_task_cpu: p->pi_lock || rq->lock (ttwu like)
	 */
	ਖਾਲੀ (*migrate_task_rq)(ਬਣਤਰ task_struct *p, ਪੂਰਨ new_cpu);

	/*
	 * ttwu_do_activate: rq->lock
	 * wake_up_new_task: task_rq_lock
	 */
	ਖਾਲੀ (*task_woken)(ਬਣਤਰ rq *this_rq, ਬਣਤਰ task_struct *task);

	/*
	 * do_set_cpus_allowed: task_rq_lock + sched_change
	 */
	ਖਾਲੀ (*ਸੈੱਟ_cpus_allowed)(ਬਣਤਰ task_struct *p, ਬਣਤਰ affinity_context *ctx);

	/*
	 * sched_set_rq_{on,off}line: rq->lock
	 */
	ਖਾਲੀ (*rq_online)(ਬਣਤਰ rq *rq);
	ਖਾਲੀ (*rq_offline)(ਬਣਤਰ rq *rq);

	/*
	 * push_cpu_stop: p->pi_lock && rq->lock
	 */
	ਬਣਤਰ rq *(*ਲੱਭੋ_ਤਾਲਾ_rq)(ਬਣਤਰ task_struct *p, ਬਣਤਰ rq *rq);

	/*
	 * hrtick: rq->lock
	 * sched_tick: rq->lock
	 * sched_tick_remote: rq->lock
	 */
	ਖਾਲੀ (*task_tick)(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, ਪੂਰਨ queued);
	/*
	 * sched_cgroup_fork: p->pi_lock
	 */
	ਖਾਲੀ (*task_fork)(ਬਣਤਰ task_struct *p);
	/*
	 * finish_task_switch: no locks
	 */
	ਖਾਲੀ (*task_dead)(ਬਣਤਰ task_struct *p);

	/*
	 * sched_change
	 */
	ਖਾਲੀ (*switching_from)(ਬਣਤਰ rq *this_rq, ਬਣਤਰ task_struct *task);
	ਖਾਲੀ (*switched_from) (ਬਣਤਰ rq *this_rq, ਬਣਤਰ task_struct *task);
	ਖਾਲੀ (*switching_ਮੰਜ਼ਿਲ)  (ਬਣਤਰ rq *this_rq, ਬਣਤਰ task_struct *task);
	ਖਾਲੀ (*switched_ਮੰਜ਼ਿਲ)   (ਬਣਤਰ rq *this_rq, ਬਣਤਰ task_struct *task);
	u64  (*get_prio)     (ਬਣਤਰ rq *this_rq, ਬਣਤਰ task_struct *task);
	ਖਾਲੀ (*prio_ਬਦਲਿਆ) (ਬਣਤਰ rq *this_rq, ਬਣਤਰ task_struct *task,
			      u64 oldprio);

	/*
	 * set_load_weight: task_rq_lock + sched_change
	 * __setscheduler_parms: task_rq_lock + sched_change
	 */
	ਖਾਲੀ (*reweight_task)(ਬਣਤਰ rq *this_rq, ਬਣਤਰ task_struct *task,
			      ਅਚਲ ਬਣਤਰ ਲੱਦੋ_ਭਾਰ *lw);

	/*
	 * sched_rr_get_interval: task_rq_lock
	 */
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ (*get_rr_interval)(ਬਣਤਰ rq *rq,
					ਬਣਤਰ task_struct *task);

	/*
	 * task_sched_runtime: task_rq_lock
	 */
	ਖਾਲੀ (*update_curr)(ਬਣਤਰ rq *rq);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_FAIR_ਸਮੂਹ_SCHED
	/*
	 * sched_change_group: task_rq_lock + sched_change
	 */
	ਖਾਲੀ (*task_change_ਸਮੂਹ)(ਬਣਤਰ task_struct *p);
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CORE
	/*
	 * pick_next_task: rq->lock
	 * try_steal_cookie: rq->lock (double)
	 */
	ਪੂਰਨ (*task_is_throttled)(ਬਣਤਰ task_struct *p, ਪੂਰਨ cpu);
#ਜੇ_ਅੰਤ
};

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ put_prev_task(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *prev)
{
	WARN_ON_ONCE(rq->donor != prev);
	prev->sched_class->put_prev_task(rq, prev, ਨਿਰਾ);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ ਸੈੱਟ_ਅਗਲਾ_task(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *ਅਗਲਾ)
{
	ਅਗਲਾ->sched_class->ਸੈੱਟ_ਅਗਲਾ_task(rq, ਅਗਲਾ, SNT_ਸਾਧਾਰਨ);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
__put_prev_set_next_dl_server(ਬਣਤਰ rq *rq,
			      ਬਣਤਰ task_struct *prev,
			      ਬਣਤਰ task_struct *ਅਗਲਾ)
{
	prev->dl_server = ਨਿਰਾ;
	ਅਗਲਾ->dl_server = rq->dl_server;
	rq->dl_server = ਨਿਰਾ;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ put_prev_ਸੈੱਟ_ਅਗਲਾ_task(ਬਣਤਰ rq *rq,
					  ਬਣਤਰ task_struct *prev,
					  ਬਣਤਰ task_struct *ਅਗਲਾ)
{
	WARN_ON_ONCE(rq->donor != prev);

	__put_prev_set_next_dl_server(rq, prev, ਅਗਲਾ);

	ਜੇ (ਅਗਲਾ == prev) {
		ਅਗਲਾ->sched_class->ਸੈੱਟ_ਅਗਲਾ_task(rq, ਅਗਲਾ, SNT_REPICK);
		ਵਾਪਸ;
	}

	prev->sched_class->put_prev_task(rq, prev, ਅਗਲਾ);
	ਅਗਲਾ->sched_class->ਸੈੱਟ_ਅਗਲਾ_task(rq, ਅਗਲਾ, SNT_PICK);
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
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEFINE_SCHED_CLASS(name) \
ਅਚਲ ਬਣਤਰ sched_class name##_sched_class \
	__aligned(__alignof__(ਬਣਤਰ sched_class)) \
	__section("__" #name "_sched_class")

/* Defined in include/asm-generic/vmlinux.lds.h */
ਬਾਹਰੀ ਬਣਤਰ sched_class __sched_class_highest[];
ਬਾਹਰੀ ਬਣਤਰ sched_class __sched_class_lowest[];

ਬਾਹਰੀ ਅਚਲ ਬਣਤਰ sched_class stop_sched_class;
ਬਾਹਰੀ ਅਚਲ ਬਣਤਰ sched_class dl_sched_class;
ਬਾਹਰੀ ਅਚਲ ਬਣਤਰ sched_class rt_sched_class;
ਬਾਹਰੀ ਅਚਲ ਬਣਤਰ sched_class fair_sched_class;
ਬਾਹਰੀ ਅਚਲ ਬਣਤਰ sched_class idle_sched_class;

/*
 * Iterate only active classes. SCX can take over all fair tasks or be
 * completely disabled. If the former, skip fair. If the latter, skip SCX.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਅਚਲ ਬਣਤਰ sched_class *ਅਗਲਾ_ਸਰਗਰਮ_class(ਅਚਲ ਬਣਤਰ sched_class *class)
{
	class++;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CLASS_EXT
	ਜੇ (scx_switched_all() && class == &fair_sched_class)
		class++;
	ਜੇ (!scx_enabled() && class == &ext_sched_class)
		class++;
#ਜੇ_ਅੰਤ
	ਵਾਪਸ class;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ for_class_range(class, _from, ਮੰਜ਼ਿਲ_2) \
	ਲਈ (class = (_from); class < (ਮੰਜ਼ਿਲ_2); class++)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ for_each_class(class) \
	for_class_range(class, __sched_class_highest, __sched_class_lowest)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ for_ਸਰਗਰਮ_class_range(class, _from, ਮੰਜ਼ਿਲ_2)				\
	ਲਈ (class = (_from); class != (ਮੰਜ਼ਿਲ_2); class = ਅਗਲਾ_ਸਰਗਰਮ_class(class))

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ for_each_ਸਰਗਰਮ_class(class)						\
	for_ਸਰਗਰਮ_class_range(class, __sched_class_highest, __sched_class_lowest)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ sched_class_above(_a, _b)	((_a) < (_b))

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ rq_modified_begin(ਬਣਤਰ rq *rq, ਅਚਲ ਬਣਤਰ sched_class *class)
{
	ਜੇ (sched_class_above(rq->ਅਗਲਾ_class, class))
		rq->ਅਗਲਾ_class = class;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool rq_modified_above(ਬਣਤਰ rq *rq, ਅਚਲ ਬਣਤਰ sched_class *class)
{
	ਵਾਪਸ sched_class_above(rq->ਅਗਲਾ_class, class);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool sched_stop_runnable(ਬਣਤਰ rq *rq)
{
	ਵਾਪਸ rq->stop && task_on_rq_queued(rq->stop);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool sched_dl_runnable(ਬਣਤਰ rq *rq)
{
	ਵਾਪਸ rq->dl.dl_nr_running > 0;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool sched_rt_runnable(ਬਣਤਰ rq *rq)
{
	ਵਾਪਸ rq->rt.rt_queued > 0;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool sched_fair_runnable(ਬਣਤਰ rq *rq)
{
	ਵਾਪਸ rq->cfs.nr_queued > 0;
}

ਬਾਹਰੀ ਬਣਤਰ task_struct *pick_task_fair(ਬਣਤਰ rq *rq, ਬਣਤਰ rq_flags *rf);
ਬਾਹਰੀ ਬਣਤਰ task_struct *pick_task_idle(ਬਣਤਰ rq *rq, ਬਣਤਰ rq_flags *rf);

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SCA_CHECK		0x01
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SCA_MIGRATE_DISABLE	0x02
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SCA_MIGRATE_ENABLE	0x04
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SCA_USER		0x08

ਬਾਹਰੀ ਖਾਲੀ update_ਸਮੂਹ_ਸਮਰੱਥਾ(ਬਣਤਰ sched_domain *sd, ਪੂਰਨ cpu);

ਬਾਹਰੀ ਖਾਲੀ sched_balance_trigger(ਬਣਤਰ rq *rq);

ਬਾਹਰੀ ਪੂਰਨ __set_cpus_allowed_ptr(ਬਣਤਰ task_struct *p, ਬਣਤਰ affinity_context *ctx);
ਬਾਹਰੀ ਖਾਲੀ ਸੈੱਟ_cpus_allowed_common(ਬਣਤਰ task_struct *p, ਬਣਤਰ affinity_context *ctx);

ਸਥਿਰ ਸਤਰਅੰਦਰ bool task_allowed_on_cpu(ਬਣਤਰ task_struct *p, ਪੂਰਨ cpu)
{
	/* When not in the task's cpumask, no point in looking further. */
	ਜੇ (!cpumask_test_cpu(cpu, p->cpus_ptr))
		ਵਾਪਸ false;

	/* Can @cpu run a user thread? */
	ਜੇ (!(p->flags & PF_KTHREAD) && !task_cpu_possible(cpu, p))
		ਵਾਪਸ false;

	ਵਾਪਸ true;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ cpumask_t *alloc_user_cpus_ptr(ਪੂਰਨ ਗੰਢ)
{
	/*
	 * See set_cpus_allowed_force() above for the rcu_head usage.
	 */
	ਪੂਰਨ ਮਾਪ = max_t(ਪੂਰਨ, cpumask_ਮਾਪ(), ਆਕਾਰ(ਬਣਤਰ rcu_ਸਿਰ));

	ਵਾਪਸ kmalloc_ਗੰਢ(ਮਾਪ, GFP_KERNEL, ਗੰਢ);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ task_struct *get_ਧੱਕੋ_task(ਬਣਤਰ rq *rq)
{
	ਬਣਤਰ task_struct *p = rq->donor;

	lockdep_assert_rq_held(rq);

	ਜੇ (rq->ਧੱਕੋ_busy)
		ਵਾਪਸ ਨਿਰਾ;

	ਜੇ (p->nr_cpus_allowed == 1)
		ਵਾਪਸ ਨਿਰਾ;

	ਜੇ (p->migration_disabled)
		ਵਾਪਸ ਨਿਰਾ;

	rq->ਧੱਕੋ_busy = true;
	ਵਾਪਸ get_task_struct(p);
}

ਬਾਹਰੀ ਪੂਰਨ ਧੱਕੋ_cpu_stop(ਖਾਲੀ *arg);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_CPU_IDLE

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ idle_ਸੈੱਟ_state(ਬਣਤਰ rq *rq,
				  ਬਣਤਰ cpuidle_state *idle_state)
{
	rq->idle_state = idle_state;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ cpuidle_state *idle_get_state(ਬਣਤਰ rq *rq)
{
	lockdep_assert(rcu_read_ਤਾਲਾ_any_held());

	ਵਾਪਸ rq->idle_state;
}

#ਨਹੀਂਤਾਂ /* !CONFIG_CPU_IDLE: */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ idle_ਸੈੱਟ_state(ਬਣਤਰ rq *rq,
				  ਬਣਤਰ cpuidle_state *idle_state)
{
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ cpuidle_state *idle_get_state(ਬਣਤਰ rq *rq)
{
	ਵਾਪਸ ਨਿਰਾ;
}

#ਜੇ_ਅੰਤ /* !CONFIG_CPU_IDLE */

ਬਾਹਰੀ ਖਾਲੀ ਸਮਾਂ_ਸਾਰਣੀ_idle(ਖਾਲੀ);
asmlinkage ਖਾਲੀ ਸਮਾਂ_ਸਾਰਣੀ_user(ਖਾਲੀ);

ਬਾਹਰੀ ਖਾਲੀ sysrq_sched_debug_show(ਖਾਲੀ);
ਬਾਹਰੀ ਖਾਲੀ sched_init_granularity(ਖਾਲੀ);
ਬਾਹਰੀ ਖਾਲੀ update_max_interval(ਖਾਲੀ);

ਬਾਹਰੀ ਖਾਲੀ init_sched_dl_class(ਖਾਲੀ);
ਬਾਹਰੀ ਖਾਲੀ init_sched_rt_class(ਖਾਲੀ);
ਬਾਹਰੀ ਖਾਲੀ init_sched_fair_class(ਖਾਲੀ);

ਬਾਹਰੀ ਖਾਲੀ resched_curr(ਬਣਤਰ rq *rq);
ਬਾਹਰੀ ਖਾਲੀ resched_curr_lazy(ਬਣਤਰ rq *rq);
ਬਾਹਰੀ ਖਾਲੀ resched_cpu(ਪੂਰਨ cpu);

ਬਾਹਰੀ ਖਾਲੀ init_rt_bandwidth(ਬਣਤਰ rt_bandwidth *rt_b, u64 period, u64 runtime);
ਬਾਹਰੀ bool sched_rt_bandwidth_account(ਬਣਤਰ rt_rq *rt_rq);

ਬਾਹਰੀ ਖਾਲੀ init_dl_entity(ਬਣਤਰ sched_dl_entity *dl_se);

ਬਾਹਰੀ ਖਾਲੀ init_cfs_throttle_work(ਬਣਤਰ task_struct *p);

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BW_ਖਿਸਕਾਓ		20
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BW_UNIT			(1 << BW_ਖਿਸਕਾਓ)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਅਨੁਪਾਤ_ਖਿਸਕਾਓ		8
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MAX_BW_ਬਿੱਟ		(64 - BW_ਖਿਸਕਾਓ)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MAX_BW			((1ULL << MAX_BW_ਬਿੱਟ) - 1)

ਬਾਹਰੀ u64 ਮੰਜ਼ਿਲ_ਅਨੁਪਾਤ(u64 period, u64 runtime);

ਬਾਹਰੀ ਖਾਲੀ init_entity_runnable_ਔਸਤਨ(ਬਣਤਰ sched_entity *se);
ਬਾਹਰੀ ਖਾਲੀ post_init_entity_util_avg(ਬਣਤਰ task_struct *p);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_NO_HZ_ਭਰਿਆ
ਬਾਹਰੀ bool sched_can_stop_tick(ਬਣਤਰ rq *rq);
ਬਾਹਰੀ ਪੂਰਨ __init sched_tick_offload_init(ਖਾਲੀ);

/*
 * Tick may be needed by tasks in the runqueue depending on their policy and
 * requirements. If tick is needed, lets send the target an IPI to kick it out of
 * nohz mode if necessary.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ sched_update_tick_dependency(ਬਣਤਰ rq *rq)
{
	ਪੂਰਨ cpu = cpu_ਦਾ(rq);

	ਜੇ (!tick_nohz_ਭਰਿਆ_cpu(cpu))
		ਵਾਪਸ;

	ਜੇ (sched_can_stop_tick(rq))
		tick_nohz_dep_clear_cpu(cpu, TICK_DEP_ਦੋਆਧਾਰੀ_ਅੰਕ_SCHED);
	ਨਹੀਂਤਾਂ
		tick_nohz_dep_ਸੈੱਟ_cpu(cpu, TICK_DEP_ਦੋਆਧਾਰੀ_ਅੰਕ_SCHED);
}
#ਨਹੀਂਤਾਂ /* !CONFIG_NO_HZ_FULL: */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ sched_tick_offload_init(ਖਾਲੀ) { ਵਾਪਸ 0; }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ sched_update_tick_dependency(ਬਣਤਰ rq *rq) { }
#ਜੇ_ਅੰਤ /* !CONFIG_NO_HZ_FULL */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ ਜੋੜੋ_nr_running(ਬਣਤਰ rq *rq, ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ prev_nr = rq->nr_running;

	rq->nr_running = prev_nr + ਗਿਣਤੀ_2;
	ਜੇ (trace_sched_update_nr_running_tp_enabled()) {
		call_trace_sched_update_nr_running(rq, ਗਿਣਤੀ_2);
	}

	ਜੇ (prev_nr < 2 && rq->nr_running >= 2)
		ਸੈੱਟ_rd_overloaded(rq->rd, 1);

	sched_update_tick_dependency(rq);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ sub_nr_running(ਬਣਤਰ rq *rq, ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2)
{
	rq->nr_running -= ਗਿਣਤੀ_2;
	ਜੇ (trace_sched_update_nr_running_tp_enabled()) {
		call_trace_sched_update_nr_running(rq, -ਗਿਣਤੀ_2);
	}

	/* Check if we still need preemption */
	sched_update_tick_dependency(rq);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ __block_task(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p)
{
	ਜੇ (p->sched_contributes_ਮੰਜ਼ਿਲ_ਲੱਦੋ)
		rq->nr_uninterruptible++;

	ਜੇ (p->ਵਿੱਚ_iowait) {
		atomic_inc(&rq->nr_iowait);
		delayacct_blkio_ਸ਼ੁਰੂ();
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

ਬਾਹਰੀ ਖਾਲੀ activate_task(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, ਪੂਰਨ flags);
ਬਾਹਰੀ ਖਾਲੀ deactivate_task(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, ਪੂਰਨ flags);

ਬਾਹਰੀ ਖਾਲੀ wakeup_preempt(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, ਪੂਰਨ flags);

/*
 * attach_task() -- attach the task detached by detach_task() to its new rq.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ attach_task(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p)
{
	lockdep_assert_rq_held(rq);

	WARN_ON_ONCE(task_rq(p) != rq);
	activate_task(rq, p, ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_NOCLOCK);
	wakeup_preempt(rq, p, 0);
}

/*
 * attach_one_task() -- attaches the task returned from detach_one_task() to
 * its new rq.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ attach_one_task(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p)
{
	guard(rq_ਤਾਲਾ)(rq);
	update_rq_clock(rq);
	attach_task(rq, p);
}

/*
 * __attach_tasks() - attaches a list of tasks (using se.group_node) to
 * the new rq
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ __attach_tasks(ਬਣਤਰ rq *rq, ਬਣਤਰ list_ਸਿਰ *tasks)
{
	guard(rq_ਤਾਲਾ)(rq);
	update_rq_clock(rq);

	ਜਦੋਂ (!list_ਖਾਲੀ_ਥਾਵਾਂ(tasks)) {
		ਬਣਤਰ task_struct *p;

		p = list_ਪਹਿਲਾ_ਇੰਦਰਾਜ(tasks, ਬਣਤਰ task_struct, se.ਸਮੂਹ_ਗੰਢ);
		list_del_init(&p->se.ਸਮੂਹ_ਗੰਢ);

		attach_task(rq, p);
	}
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_PREEMPT_RT
# define SCHED_NR_MIGRATE_BREAK 8
#ਨਹੀਂਤਾਂ
# define SCHED_NR_MIGRATE_BREAK 32
#ਜੇ_ਅੰਤ

ਬਾਹਰੀ __read_mostly ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ sysctl_sched_nr_migrate;
ਬਾਹਰੀ __read_mostly ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ sysctl_sched_migration_ਲਾਗਤ;

ਬਾਹਰੀ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ sysctl_sched_ਅਧਾਰ_ਟੁਕੜਾ;

ਬਾਹਰੀ ਪੂਰਨ sysctl_resched_latency_warn_ms;
ਬਾਹਰੀ ਪੂਰਨ sysctl_resched_latency_warn_once;

ਬਾਹਰੀ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ sysctl_sched_tunable_scaling;

ਬਾਹਰੀ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ sysctl_numa_balancing_ਜਾਂਚ_delay;
ਬਾਹਰੀ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ sysctl_numa_balancing_ਜਾਂਚ_period_min;
ਬਾਹਰੀ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ sysctl_numa_balancing_ਜਾਂਚ_period_max;
ਬਾਹਰੀ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ sysctl_numa_balancing_ਜਾਂਚ_ਮਾਪ;
ਬਾਹਰੀ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ sysctl_numa_balancing_hot_ਹੱਦ;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_HRTICK

/*
 * Use hrtick when:
 *  - enabled by features
 *  - hrtimer is actually high res
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ bool hrtick_enabled(ਬਣਤਰ rq *rq)
{
	ਵਾਪਸ cpu_ਸਰਗਰਮ(cpu_ਦਾ(rq)) && hrtimer_highres_enabled();
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool hrtick_enabled_fair(ਬਣਤਰ rq *rq)
{
	ਵਾਪਸ sched_feat(HRTICK) && hrtick_enabled(rq);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool hrtick_enabled_dl(ਬਣਤਰ rq *rq)
{
	ਵਾਪਸ sched_feat(HRTICK_DL) && hrtick_enabled(rq);
}

ਬਾਹਰੀ ਖਾਲੀ hrtick_ਸ਼ੁਰੂ(ਬਣਤਰ rq *rq, u64 delay);
ਸਥਿਰ ਸਤਰਅੰਦਰ bool hrtick_ਸਰਗਰਮ(ਬਣਤਰ rq *rq)
{
	ਵਾਪਸ hrtimer_ਸਰਗਰਮ(&rq->hrtick_timer);
}

#ਨਹੀਂਤਾਂ /* !CONFIG_SCHED_HRTICK: */
ਸਥਿਰ ਸਤਰਅੰਦਰ bool hrtick_enabled_fair(ਬਣਤਰ rq *rq) { ਵਾਪਸ false; }
ਸਥਿਰ ਸਤਰਅੰਦਰ bool hrtick_enabled_dl(ਬਣਤਰ rq *rq) { ਵਾਪਸ false; }
ਸਥਿਰ ਸਤਰਅੰਦਰ bool hrtick_enabled(ਬਣਤਰ rq *rq) { ਵਾਪਸ false; }
#ਜੇ_ਅੰਤ /* !CONFIG_SCHED_HRTICK */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ arch_scale_freq_tick
ਸਥਿਰ __always_inline ਖਾਲੀ arch_scale_freq_tick(ਖਾਲੀ) { }
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ arch_scale_freq_ਸਮਰੱਥਾ
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
ਸਥਿਰ __always_inline
ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ arch_scale_freq_ਸਮਰੱਥਾ(ਪੂਰਨ cpu)
{
	ਵਾਪਸ SCHED_ਸਮਰੱਥਾ_SCALE;
}
#ਜੇ_ਅੰਤ

/*
 * In double_lock_balance()/double_rq_lock(), we use raw_spin_rq_lock() to
 * acquire rq lock instead of rq_lock(). So at the end of these two functions
 * we need to call double_rq_clock_clear_update() to clear RQCF_UPDATED of
 * rq->clock_update_flags to avoid the WARN_DOUBLE_CLOCK warning.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ double_rq_clock_clear_update(ਬਣਤਰ rq *rq1, ਬਣਤਰ rq *rq2)
{
	rq1->clock_update_flags &= (RQCF_REQ_SKIP|RQCF_ACT_SKIP);
	rq2->clock_update_flags &= (RQCF_REQ_SKIP|RQCF_ACT_SKIP);
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEFINE_ਤਾਲਾ_GUARD_2(name, type, ਤਾਲਾ_2, _unlock, ...)				\
__DEFINE_UNLOCK_GUARD(name, type, _unlock, type *lock2; __VA_ARGS__)			\
ਸਥਿਰ ਸਤਰਅੰਦਰ class_##name##_t class_##name##_constructor(type *ਤਾਲਾ, type *lock2)	\
	__no_context_analysis								\
{ class_##name##_t _t = { .ਤਾਲਾ = ਤਾਲਾ, .lock2 = lock2 }, *_T = &_t;			\
  ਤਾਲਾ_2; ਵਾਪਸ _t; }
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DECLARE_ਤਾਲਾ_GUARD_2_ATTRS(_name, ਤਾਲਾ_2, _unlock1, _unlock2)			\
ਸਥਿਰ ਸਤਰਅੰਦਰ class_##_name##_t class_##_name##_constructor(ਤਾਲਾ_3##_name##_t *_T1,	\
							    ਤਾਲਾ_3##_name##_t *_T2) ਤਾਲਾ_2; \
ਸਥਿਰ __always_inline ਖਾਲੀ __class_##_name##_cleanup_ctx1(class_##_name##_t **_T1)	\
	__no_context_analysis _unlock1 { }						\
ਸਥਿਰ __always_inline ਖਾਲੀ __class_##_name##_cleanup_ctx2(class_##_name##_t **_T2)	\
	__no_context_analysis _unlock2 { }
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ WITH_ਤਾਲਾ_GUARD_2_ATTRS(_name, _T1, _T2)					\
	class_##_name##_constructor(_T1, _T2),						\
	*__UNIQUE_ID(unlock1) __cleanup(__class_##_name##_cleanup_ctx1) = (ਖਾਲੀ *)(_T1),\
	*__UNIQUE_ID(unlock2) __cleanup(__class_##_name##_cleanup_ctx2) = (ਖਾਲੀ *)(_T2)

ਸਥਿਰ ਸਤਰਅੰਦਰ bool rq_ਕ੍ਰਮ_less(ਬਣਤਰ rq *rq1, ਬਣਤਰ rq *rq2)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CORE
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
	ਜੇ (rq1->core->cpu < rq2->core->cpu)
		ਵਾਪਸ true;
	ਜੇ (rq1->core->cpu > rq2->core->cpu)
		ਵਾਪਸ false;

	/*
	 * __sched_core_flip() relies on SMT having cpu-id lock order.
	 */
#ਜੇ_ਅੰਤ /* CONFIG_SCHED_CORE */
	ਵਾਪਸ rq1->cpu < rq2->cpu;
}

ਬਾਹਰੀ ਖਾਲੀ double_rq_ਤਾਲਾ(ਬਣਤਰ rq *rq1, ਬਣਤਰ rq *rq2)
	__acquires(__rq_lockp(rq1), __rq_lockp(rq2));

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_PREEMPTION

/*
 * fair double_lock_balance: Safely acquires both rq->locks in a fair
 * way at the expense of forcing extra atomic operations in all
 * invocations.  This assures that the double_lock is acquired using the
 * same underlying policy as the spinlock_t on this architecture, which
 * reduces latency compared to the unfair variant below.  However, it
 * also adds more overhead and therefore may reduce throughput.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ double_ਤਾਲਾ_balance_2(ਬਣਤਰ rq *this_rq, ਬਣਤਰ rq *busiest)
	__must_hold(__rq_lockp(this_rq))
	__acquires(__rq_lockp(busiest))
{
	raw_spin_rq_unlock(this_rq);
	double_rq_ਤਾਲਾ(this_rq, busiest);

	ਵਾਪਸ 1;
}

#ਨਹੀਂਤਾਂ /* !CONFIG_PREEMPTION: */
/*
 * Unfair double_lock_balance: Optimizes throughput at the expense of
 * latency by eliminating extra atomic operations when the locks are
 * already in proper order on entry.  This favors lower CPU-ids and will
 * grant the double lock to lower CPUs over higher ids under contention,
 * regardless of entry order into the function.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ double_ਤਾਲਾ_balance_2(ਬਣਤਰ rq *this_rq, ਬਣਤਰ rq *busiest)
	__must_hold(__rq_lockp(this_rq))
	__acquires(__rq_lockp(busiest))
{
	ਜੇ (__rq_lockp(this_rq) == __rq_lockp(busiest)) {
		__acquire(__rq_lockp(busiest)); /* already held */
		double_rq_clock_clear_update(this_rq, busiest);
		ਵਾਪਸ 0;
	}

	ਜੇ (likely(raw_spin_rq_trylock(busiest))) {
		double_rq_clock_clear_update(this_rq, busiest);
		ਵਾਪਸ 0;
	}

	ਜੇ (rq_ਕ੍ਰਮ_less(this_rq, busiest)) {
		raw_spin_rq_ਤਾਲਾ_nested(busiest, SINGLE_ਡੂੰਘਾਈ_NESTING);
		double_rq_clock_clear_update(this_rq, busiest);
		ਵਾਪਸ 0;
	}

	raw_spin_rq_unlock(this_rq);
	double_rq_ਤਾਲਾ(this_rq, busiest);

	ਵਾਪਸ 1;
}

#ਜੇ_ਅੰਤ /* !CONFIG_PREEMPTION */

/*
 * double_lock_balance - lock the busiest runqueue, this_rq is locked already.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ double_ਤਾਲਾ_balance(ਬਣਤਰ rq *this_rq, ਬਣਤਰ rq *busiest)
	__must_hold(__rq_lockp(this_rq))
	__acquires(__rq_lockp(busiest))
{
	lockdep_assert_irqs_disabled();

	ਵਾਪਸ double_ਤਾਲਾ_balance_2(this_rq, busiest);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ double_unlock_balance(ਬਣਤਰ rq *this_rq, ਬਣਤਰ rq *busiest)
	__releases(__rq_lockp(busiest))
{
	ਜੇ (__rq_lockp(this_rq) != __rq_lockp(busiest))
		raw_spin_rq_unlock(busiest);
	ਨਹੀਂਤਾਂ
		__release(__rq_lockp(busiest)); /* fake release */
	ਤਾਲਾ_ਸੈੱਟ_subclass(&__rq_lockp(this_rq)->dep_map, 0, _RET_IP_);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ double_ਤਾਲਾ(spinlock_t *l1, spinlock_t *l2)
	__acquires(l1, l2)
{
	ਜੇ (l1 > l2)
		ਅਦਲਾ_ਬਦਲੀ(l1, l2);

	spin_ਤਾਲਾ(l1);
	spin_ਤਾਲਾ_nested(l2, SINGLE_ਡੂੰਘਾਈ_NESTING);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ double_ਤਾਲਾ_irq(spinlock_t *l1, spinlock_t *l2)
	__acquires(l1, l2)
{
	ਜੇ (l1 > l2)
		ਅਦਲਾ_ਬਦਲੀ(l1, l2);

	spin_ਤਾਲਾ_irq(l1);
	spin_ਤਾਲਾ_nested(l2, SINGLE_ਡੂੰਘਾਈ_NESTING);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ double_raw_ਤਾਲਾ(raw_spinlock_t *l1, raw_spinlock_t *l2)
	__acquires(l1, l2)
{
	ਜੇ (l1 > l2)
		ਅਦਲਾ_ਬਦਲੀ(l1, l2);

	raw_spin_ਤਾਲਾ(l1);
	raw_spin_ਤਾਲਾ_nested(l2, SINGLE_ਡੂੰਘਾਈ_NESTING);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ double_raw_unlock(raw_spinlock_t *l1, raw_spinlock_t *l2)
	__releases(l1, l2)
{
	raw_spin_unlock(l1);
	raw_spin_unlock(l2);
}

DEFINE_ਤਾਲਾ_GUARD_2(double_raw_spinlock, raw_spinlock_t,
		    double_raw_ਤਾਲਾ(_T->ਤਾਲਾ, _T->lock2),
		    double_raw_unlock(_T->ਤਾਲਾ, _T->lock2))

DECLARE_ਤਾਲਾ_GUARD_2_ATTRS(double_raw_spinlock,
			   __acquires(_T1, _T2),
			   __releases(*(raw_spinlock_t **)_T1),
			   __releases(*(raw_spinlock_t **)_T2));
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ class_double_raw_spinlock_constructor(_T1, _T2) \
	WITH_ਤਾਲਾ_GUARD_2_ATTRS(double_raw_spinlock, _T1, _T2)

/*
 * double_rq_unlock - safely unlock two runqueues
 *
 * Note this does not restore interrupts like task_rq_unlock,
 * you need to do so manually after calling.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ double_rq_unlock(ਬਣਤਰ rq *rq1, ਬਣਤਰ rq *rq2)
	__releases(__rq_lockp(rq1), __rq_lockp(rq2))
{
	ਜੇ (__rq_lockp(rq1) != __rq_lockp(rq2))
		raw_spin_rq_unlock(rq2);
	ਨਹੀਂਤਾਂ
		__release(__rq_lockp(rq2)); /* fake release */
	raw_spin_rq_unlock(rq1);
}

ਬਾਹਰੀ ਖਾਲੀ ਸੈੱਟ_rq_online (ਬਣਤਰ rq *rq);
ਬਾਹਰੀ ਖਾਲੀ ਸੈੱਟ_rq_offline(ਬਣਤਰ rq *rq);

ਬਾਹਰੀ bool sched_smp_initialized;

DEFINE_ਤਾਲਾ_GUARD_2(double_rq_ਤਾਲਾ, ਬਣਤਰ rq,
		    double_rq_ਤਾਲਾ(_T->ਤਾਲਾ, _T->lock2),
		    double_rq_unlock(_T->ਤਾਲਾ, _T->lock2))

ਬਾਹਰੀ ਬਣਤਰ sched_entity *__pick_root_entity(ਬਣਤਰ cfs_rq *cfs_rq);
ਬਾਹਰੀ ਬਣਤਰ sched_entity *__pick_first_entity(ਬਣਤਰ cfs_rq *cfs_rq);
ਬਾਹਰੀ ਬਣਤਰ sched_entity *__pick_last_entity(ਬਣਤਰ cfs_rq *cfs_rq);

ਬਾਹਰੀ bool sched_debug_verbose;

ਬਾਹਰੀ ਖਾਲੀ print_cfs_stats(ਬਣਤਰ seq_file *m, ਪੂਰਨ cpu);
ਬਾਹਰੀ ਖਾਲੀ print_rt_stats(ਬਣਤਰ seq_file *m, ਪੂਰਨ cpu);
ਬਾਹਰੀ ਖਾਲੀ print_dl_stats(ਬਣਤਰ seq_file *m, ਪੂਰਨ cpu);
ਬਾਹਰੀ ਖਾਲੀ print_cfs_rq(ਬਣਤਰ seq_file *m, ਪੂਰਨ cpu, ਬਣਤਰ cfs_rq *cfs_rq);
ਬਾਹਰੀ ਖਾਲੀ print_rt_rq(ਬਣਤਰ seq_file *m, ਪੂਰਨ cpu, ਬਣਤਰ rt_rq *rt_rq);
ਬਾਹਰੀ ਖਾਲੀ print_dl_rq(ਬਣਤਰ seq_file *m, ਪੂਰਨ cpu, ਬਣਤਰ dl_rq *dl_rq);

ਬਾਹਰੀ ਖਾਲੀ resched_latency_warn(ਪੂਰਨ cpu, u64 latency);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_NUMA_BALANCING
ਬਾਹਰੀ ਖਾਲੀ show_numa_stats(ਬਣਤਰ task_struct *p, ਬਣਤਰ seq_file *m);
ਬਾਹਰੀ ਖਾਲੀ
print_numa_stats(ਬਣਤਰ seq_file *m, ਪੂਰਨ ਗੰਢ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ tsf,
		 ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ tpf, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ gsf, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ gpf);
#ਜੇ_ਅੰਤ /* CONFIG_NUMA_BALANCING */

ਬਾਹਰੀ ਖਾਲੀ init_cfs_rq(ਬਣਤਰ cfs_rq *cfs_rq);
ਬਾਹਰੀ ਖਾਲੀ init_rt_rq(ਬਣਤਰ rt_rq *rt_rq);
ਬਾਹਰੀ ਖਾਲੀ init_dl_rq(ਬਣਤਰ dl_rq *dl_rq);

ਬਾਹਰੀ ਖਾਲੀ cfs_bandwidth_usage_inc(ਖਾਲੀ);
ਬਾਹਰੀ ਖਾਲੀ cfs_bandwidth_usage_dec(ਖਾਲੀ);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_NO_HZ_COMMON

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NOHZ_BALANCE_KICK_ਦੋਆਧਾਰੀ_ਅੰਕ	0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NOHZ_STATS_KICK_ਦੋਆਧਾਰੀ_ਅੰਕ	1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NOHZ_NEWILB_KICK_ਦੋਆਧਾਰੀ_ਅੰਕ	2
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NOHZ_ਅਗਲਾ_KICK_ਦੋਆਧਾਰੀ_ਅੰਕ	3

/* Run sched_balance_domains() */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NOHZ_BALANCE_KICK	ਦੋਆਧਾਰੀ_ਅੰਕ(NOHZ_BALANCE_KICK_ਦੋਆਧਾਰੀ_ਅੰਕ)
/* Update blocked load */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NOHZ_STATS_KICK		ਦੋਆਧਾਰੀ_ਅੰਕ(NOHZ_STATS_KICK_ਦੋਆਧਾਰੀ_ਅੰਕ)
/* Update blocked load when entering idle */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NOHZ_NEWILB_KICK	ਦੋਆਧਾਰੀ_ਅੰਕ(NOHZ_NEWILB_KICK_ਦੋਆਧਾਰੀ_ਅੰਕ)
/* Update nohz.next_balance */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NOHZ_ਅਗਲਾ_KICK		ਦੋਆਧਾਰੀ_ਅੰਕ(NOHZ_ਅਗਲਾ_KICK_ਦੋਆਧਾਰੀ_ਅੰਕ)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NOHZ_KICK_ਨਕਾਬ		(NOHZ_BALANCE_KICK | NOHZ_STATS_KICK | NOHZ_ਅਗਲਾ_KICK)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ nohz_flags(cpu)		(&cpu_rq(cpu)->nohz_flags)

ਬਾਹਰੀ ਖਾਲੀ nohz_balance_exit_idle(ਬਣਤਰ rq *rq);
#ਨਹੀਂਤਾਂ /* !CONFIG_NO_HZ_COMMON: */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ nohz_balance_exit_idle(ਬਣਤਰ rq *rq) { }
#ਜੇ_ਅੰਤ /* !CONFIG_NO_HZ_COMMON */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_NO_HZ_COMMON
ਬਾਹਰੀ ਖਾਲੀ nohz_ਲੜੀਵਾਰ_idle_balance(ਪੂਰਨ cpu);
#ਨਹੀਂਤਾਂ
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ nohz_ਲੜੀਵਾਰ_idle_balance(ਪੂਰਨ cpu) { }
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "stats.h"

#ਜੇ defined(CONFIG_SCHED_CORE) && defined(CONFIG_SCHEDSTATS)

ਬਾਹਰੀ ਖਾਲੀ __sched_core_account_forceidle(ਬਣਤਰ rq *rq);

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ sched_core_account_forceidle(ਬਣਤਰ rq *rq)
{
	ਜੇ (schedstat_enabled())
		__sched_core_account_forceidle(rq);
}

ਬਾਹਰੀ ਖਾਲੀ __sched_core_tick(ਬਣਤਰ rq *rq);

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ sched_core_tick(ਬਣਤਰ rq *rq)
{
	ਜੇ (sched_core_enabled(rq) && schedstat_enabled())
		__sched_core_tick(rq);
}

#ਨਹੀਂਤਾਂ /* !(CONFIG_SCHED_CORE && CONFIG_SCHEDSTATS): */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ sched_core_account_forceidle(ਬਣਤਰ rq *rq) { }

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ sched_core_tick(ਬਣਤਰ rq *rq) { }

#ਜੇ_ਅੰਤ /* !(CONFIG_SCHED_CORE && CONFIG_SCHEDSTATS) */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_IRQ_ਸਮਾਂ_ACCOUNTING

ਬਣਤਰ irqtime {
	u64			ਕੁੱਲ;
	u64			tick_delta;
	u64			irq_ਸ਼ੁਰੂ_ਸਮਾਂ;
	ਬਣਤਰ u64_stats_sync	sync;
};

DECLARE_ਪ੍ਰਤੀ_CPU(ਬਣਤਰ irqtime, cpu_irqtime);
DECLARE_STATIC_ਕੁੰਜੀ_FALSE(sched_clock_irqtime);

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ irqtime_enabled(ਖਾਲੀ)
{
	ਵਾਪਸ static_branch_likely(&sched_clock_irqtime);
}

/*
 * Returns the irqtime minus the softirq time computed by ksoftirqd.
 * Otherwise ksoftirqd's sum_exec_runtime is subtracted its own runtime
 * and never move forward.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ u64 irq_ਸਮਾਂ_read(ਪੂਰਨ cpu)
{
	ਬਣਤਰ irqtime *irqtime = &ਪ੍ਰਤੀ_cpu(cpu_irqtime, cpu);
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ seq;
	u64 ਕੁੱਲ;

	ਕਰੋ {
		seq = __u64_stats_fetch_begin(&irqtime->sync);
		ਕੁੱਲ = irqtime->ਕੁੱਲ;
	} ਜਦੋਂ (__u64_stats_fetch_retry(&irqtime->sync, seq));

	ਵਾਪਸ ਕੁੱਲ;
}

#ਨਹੀਂਤਾਂ /* !CONFIG_IRQ_TIME_ACCOUNTING: */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ irqtime_enabled(ਖਾਲੀ)
{
	ਵਾਪਸ 0;
}

#ਜੇ_ਅੰਤ /* !CONFIG_IRQ_TIME_ACCOUNTING */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_CPU_FREQ

DECLARE_ਪ੍ਰਤੀ_CPU(ਬਣਤਰ update_util_ਅੰਕੜੇ __rcu *, cpufreq_update_util_ਅੰਕੜੇ);

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
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ cpufreq_update_util(ਬਣਤਰ rq *rq, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags)
{
	ਬਣਤਰ update_util_ਅੰਕੜੇ *ਅੰਕੜੇ;

	ਅੰਕੜੇ = rcu_dereference_sched(*ਪ੍ਰਤੀ_cpu_ptr(&cpufreq_update_util_ਅੰਕੜੇ,
						  cpu_ਦਾ(rq)));
	ਜੇ (ਅੰਕੜੇ)
		ਅੰਕੜੇ->func(ਅੰਕੜੇ, rq_clock(rq), flags);
}
#ਨਹੀਂਤਾਂ /* !CONFIG_CPU_FREQ: */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ cpufreq_update_util(ਬਣਤਰ rq *rq, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags) { }
#ਜੇ_ਅੰਤ /* !CONFIG_CPU_FREQ */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ arch_scale_freq_ਸਮਰੱਥਾ
# ifndef arch_scale_freq_invariant
#  define arch_scale_freq_invariant()	true
# endif
#ਨਹੀਂਤਾਂ
# define arch_scale_freq_invariant()	false
#ਜੇ_ਅੰਤ

ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ effective_cpu_util(ਪੂਰਨ cpu, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ util_cfs,
				 ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ *min,
				 ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ *max);

ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ sugov_effective_cpu_perf(ਪੂਰਨ cpu, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ actual,
				 ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ min,
				 ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ max);


/*
 * Verify the fitness of task @p to run on @cpu taking into account the
 * CPU original capacity and the runtime/deadline ratio of the task.
 *
 * The function will return true if the original capacity of @cpu is
 * greater than or equal to task's deadline density right shifted by
 * (BW_SHIFT - SCHED_CAPACITY_SHIFT) and false otherwise.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ bool dl_task_ਸਮਾਉਂਦਾ_ਸਮਰੱਥਾ(ਬਣਤਰ task_struct *p, ਪੂਰਨ cpu)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ cap = arch_scale_cpu_ਸਮਰੱਥਾ(cpu);

	ਵਾਪਸ cap >= p->dl.dl_density >> (BW_ਖਿਸਕਾਓ - SCHED_ਸਮਰੱਥਾ_ਖਿਸਕਾਓ);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ cpu_bw_dl(ਬਣਤਰ rq *rq)
{
	ਵਾਪਸ (rq->dl.running_bw * SCHED_ਸਮਰੱਥਾ_SCALE) >> BW_ਖਿਸਕਾਓ;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ cpu_util_dl(ਬਣਤਰ rq *rq)
{
	ਵਾਪਸ READ_ONCE(rq->avg_dl.util_avg);
}


ਬਾਹਰੀ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ cpu_util_cfs(ਪੂਰਨ cpu);
ਬਾਹਰੀ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ cpu_util_cfs_boost(ਪੂਰਨ cpu);

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ cpu_util_rt(ਬਣਤਰ rq *rq)
{
	ਵਾਪਸ READ_ONCE(rq->avg_rt.util_avg);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_UCLAMP_TASK

ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ uclamp_eff_ਮੁੱਲ(ਬਣਤਰ task_struct *p, ਗਿਣਤੀ uclamp_id clamp_id);

/*
 * When uclamp is compiled in, the aggregation at rq level is 'turned off'
 * by default in the fast path and only gets turned on once userspace performs
 * an operation that requires it.
 *
 * Returns true if userspace opted-in to use uclamp and aggregation at rq level
 * hence is active.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ bool uclamp_is_ਵਰਤਿਆ(ਖਾਲੀ)
{
	ਵਾਪਸ static_branch_likely(&sched_uclamp_ਵਰਤਿਆ);
}

/*
 * Enabling static branches would get the cpus_read_lock(),
 * check whether uclamp_is_used before enable it to avoid always
 * calling cpus_read_lock(). Because we never disable this
 * static key once enable it.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ sched_uclamp_enable(ਖਾਲੀ)
{
	ਜੇ (!uclamp_is_ਵਰਤਿਆ())
		static_branch_enable(&sched_uclamp_ਵਰਤਿਆ);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ uclamp_rq_get(ਬਣਤਰ rq *rq,
					  ਗਿਣਤੀ uclamp_id clamp_id)
{
	ਵਾਪਸ READ_ONCE(rq->uclamp[clamp_id].ਮੁੱਲ);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ uclamp_rq_ਸੈੱਟ(ਬਣਤਰ rq *rq, ਗਿਣਤੀ uclamp_id clamp_id,
				 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮੁੱਲ)
{
	WRITE_ONCE(rq->uclamp[clamp_id].ਮੁੱਲ, ਮੁੱਲ);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool uclamp_rq_is_idle(ਬਣਤਰ rq *rq)
{
	ਵਾਪਸ rq->uclamp_flags & UCLAMP_FLAG_IDLE;
}

/* Is the rq being capped/throttled by uclamp_max? */
ਸਥਿਰ ਸਤਰਅੰਦਰ bool uclamp_rq_is_capped(ਬਣਤਰ rq *rq)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ rq_util;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ max_util;

	ਜੇ (!uclamp_is_ਵਰਤਿਆ())
		ਵਾਪਸ false;

	rq_util = cpu_util_cfs(cpu_ਦਾ(rq)) + cpu_util_rt(rq);
	max_util = READ_ONCE(rq->uclamp[UCLAMP_MAX].ਮੁੱਲ);

	ਵਾਪਸ max_util != SCHED_ਸਮਰੱਥਾ_SCALE && rq_util >= max_util;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ for_each_clamp_id(clamp_id) \
	ਲਈ ((clamp_id) = 0; (clamp_id) < UCLAMP_CNT; (clamp_id)++)

ਬਾਹਰੀ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ sysctl_sched_uclamp_util_min_rt_default;


ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ uclamp_none(ਗਿਣਤੀ uclamp_id clamp_id)
{
	ਜੇ (clamp_id == UCLAMP_MIN)
		ਵਾਪਸ 0;
	ਵਾਪਸ SCHED_ਸਮਰੱਥਾ_SCALE;
}

/* Integer rounded range for each bucket */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ UCLAMP_ਬਾਲਟੀ_DELTA DIV_ਗੇੜਾ_CLOSEST(SCHED_ਸਮਰੱਥਾ_SCALE, UCLAMP_BUCKETS)

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ uclamp_ਬਾਲਟੀ_id(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ clamp_ਮੁੱਲ)
{
	ਵਾਪਸ min_t(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ, clamp_ਮੁੱਲ / UCLAMP_ਬਾਲਟੀ_DELTA, UCLAMP_BUCKETS - 1);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
uclamp_se_ਸੈੱਟ(ਬਣਤਰ uclamp_se *uc_se, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮੁੱਲ, bool user_defined)
{
	uc_se->ਮੁੱਲ = ਮੁੱਲ;
	uc_se->ਬਾਲਟੀ_id = uclamp_ਬਾਲਟੀ_id(ਮੁੱਲ);
	uc_se->user_defined = user_defined;
}

#ਨਹੀਂਤਾਂ /* !CONFIG_UCLAMP_TASK: */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ
uclamp_eff_ਮੁੱਲ(ਬਣਤਰ task_struct *p, ਗਿਣਤੀ uclamp_id clamp_id)
{
	ਜੇ (clamp_id == UCLAMP_MIN)
		ਵਾਪਸ 0;

	ਵਾਪਸ SCHED_ਸਮਰੱਥਾ_SCALE;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool uclamp_rq_is_capped(ਬਣਤਰ rq *rq) { ਵਾਪਸ false; }

ਸਥਿਰ ਸਤਰਅੰਦਰ bool uclamp_is_ਵਰਤਿਆ(ਖਾਲੀ)
{
	ਵਾਪਸ false;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ sched_uclamp_enable(ਖਾਲੀ) {}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ
uclamp_rq_get(ਬਣਤਰ rq *rq, ਗਿਣਤੀ uclamp_id clamp_id)
{
	ਜੇ (clamp_id == UCLAMP_MIN)
		ਵਾਪਸ 0;

	ਵਾਪਸ SCHED_ਸਮਰੱਥਾ_SCALE;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
uclamp_rq_ਸੈੱਟ(ਬਣਤਰ rq *rq, ਗਿਣਤੀ uclamp_id clamp_id, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮੁੱਲ)
{
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool uclamp_rq_is_idle(ਬਣਤਰ rq *rq)
{
	ਵਾਪਸ false;
}

#ਜੇ_ਅੰਤ /* !CONFIG_UCLAMP_TASK */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_HAVE_SCHED_AVG_IRQ

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ cpu_util_irq(ਬਣਤਰ rq *rq)
{
	ਵਾਪਸ READ_ONCE(rq->avg_irq.util_avg);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ
ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ scale_irq_ਸਮਰੱਥਾ(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ util, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ irq, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ max)
{
	util *= (max - irq);
	util /= max;

	ਵਾਪਸ util;

}

#ਨਹੀਂਤਾਂ /* !CONFIG_HAVE_SCHED_AVG_IRQ: */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ cpu_util_irq(ਬਣਤਰ rq *rq)
{
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ
ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ scale_irq_ਸਮਰੱਥਾ(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ util, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ irq, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ max)
{
	ਵਾਪਸ util;
}

#ਜੇ_ਅੰਤ /* !CONFIG_HAVE_SCHED_AVG_IRQ */

ਬਾਹਰੀ ਖਾਲੀ __setparam_fair(ਬਣਤਰ task_struct *p, ਅਚਲ ਬਣਤਰ sched_attr *attr);

#ਜੇ defined(CONFIG_ENERGY_MODEL) && defined(CONFIG_CPU_FREQ_GOV_SCHEDUTIL)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ perf_domain_ਵਿਸਤਾਰ(pd) (ਮੰਜ਼ਿਲ_cpumask(((pd)->em_pd->cpus)))

DECLARE_STATIC_ਕੁੰਜੀ_FALSE(sched_energy_present);

ਸਥਿਰ ਸਤਰਅੰਦਰ bool sched_energy_enabled(ਖਾਲੀ)
{
	ਵਾਪਸ static_branch_unlikely(&sched_energy_present);
}

#ਨਹੀਂਤਾਂ /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL): */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ perf_domain_ਵਿਸਤਾਰ(pd) ਨਿਰਾ

ਸਥਿਰ ਸਤਰਅੰਦਰ bool sched_energy_enabled(ਖਾਲੀ) { ਵਾਪਸ false; }

#ਜੇ_ਅੰਤ /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL) */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_MEMBARRIER

/*
 * The scheduler provides memory barriers required by membarrier between:
 * - prior user-space memory accesses and store to rq->membarrier_state,
 * - store to rq->membarrier_state and following user-space memory accesses.
 * In the same way it provides those guarantees around store to rq->curr.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ membarrier_switch_mm(ਬਣਤਰ rq *rq,
					ਬਣਤਰ mm_struct *prev_mm,
					ਬਣਤਰ mm_struct *ਅਗਲਾ_mm)
{
	ਪੂਰਨ membarrier_state;

	ਜੇ (prev_mm == ਅਗਲਾ_mm)
		ਵਾਪਸ;

	membarrier_state = atomic_read(&ਅਗਲਾ_mm->membarrier_state);
	ਜੇ (READ_ONCE(rq->membarrier_state) == membarrier_state)
		ਵਾਪਸ;

	WRITE_ONCE(rq->membarrier_state, membarrier_state);
}

#ਨਹੀਂਤਾਂ /* !CONFIG_MEMBARRIER: */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ membarrier_switch_mm(ਬਣਤਰ rq *rq,
					ਬਣਤਰ mm_struct *prev_mm,
					ਬਣਤਰ mm_struct *ਅਗਲਾ_mm)
{
}

#ਜੇ_ਅੰਤ /* !CONFIG_MEMBARRIER */

ਸਥਿਰ ਸਤਰਅੰਦਰ bool is_ਪ੍ਰਤੀ_cpu_kthread(ਬਣਤਰ task_struct *p)
{
	ਜੇ (!(p->flags & PF_KTHREAD))
		ਵਾਪਸ false;

	ਜੇ (p->nr_cpus_allowed != 1)
		ਵਾਪਸ false;

	ਵਾਪਸ true;
}

ਬਾਹਰੀ ਖਾਲੀ swake_ਉੱਪਰ_all_locked(ਬਣਤਰ swait_ਕਤਾਰ_ਸਿਰ *q);
ਬਾਹਰੀ ਖਾਲੀ __prepare_to_swait(ਬਣਤਰ swait_ਕਤਾਰ_ਸਿਰ *q, ਬਣਤਰ swait_ਕਤਾਰ *wait);

ਬਾਹਰੀ ਪੂਰਨ try_ਮੰਜ਼ਿਲ_wake_ਉੱਪਰ(ਬਣਤਰ task_struct *tsk, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ state, ਪੂਰਨ wake_flags);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_PREEMPT_DYNAMIC
ਬਾਹਰੀ ਪੂਰਨ preempt_dynamic_ਬਹੁਲਕ;
ਬਾਹਰੀ ਪੂਰਨ sched_dynamic_ਬਹੁਲਕ(ਅਚਲ ਅੱਖਰ *str);
ਬਾਹਰੀ ਖਾਲੀ sched_dynamic_update(ਪੂਰਨ ਬਹੁਲਕ);
#ਜੇ_ਅੰਤ
ਬਾਹਰੀ ਅਚਲ ਅੱਖਰ *preempt_modes[];

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_MM_CID

ਸਥਿਰ __always_inline bool cid_on_cpu(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cid)
{
	ਵਾਪਸ cid & MM_CID_ONCPU;
}

ਸਥਿਰ __always_inline bool cid_ਵਿੱਚ_transit(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cid)
{
	ਵਾਪਸ cid & MM_CID_TRANSIT;
}

ਸਥਿਰ __always_inline ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cpu_cid_ਮੰਜ਼ਿਲ_cid(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cid)
{
	ਵਾਪਸ cid & ~MM_CID_ONCPU;
}

ਸਥਿਰ __always_inline ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cid_ਮੰਜ਼ਿਲ_cpu_cid(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cid)
{
	ਵਾਪਸ cid | MM_CID_ONCPU;
}

ਸਥਿਰ __always_inline ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cid_ਮੰਜ਼ਿਲ_transit_cid(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cid)
{
	ਵਾਪਸ cid | MM_CID_TRANSIT;
}

ਸਥਿਰ __always_inline ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cid_from_transit_cid(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cid)
{
	ਵਾਪਸ cid & ~MM_CID_TRANSIT;
}

ਸਥਿਰ __always_inline bool cid_on_task(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cid)
{
	/* True if none of the MM_CID_ONCPU, MM_CID_TRANSIT, MM_CID_UNSET bits is set */
	ਵਾਪਸ cid < MM_CID_TRANSIT;
}

ਸਥਿਰ __always_inline ਖਾਲੀ mm_drop_cid(ਬਣਤਰ mm_struct *mm, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cid)
{
	clear_ਦੋਆਧਾਰੀ_ਅੰਕ(cid, mm_cidmask(mm));
}

ਸਥਿਰ __always_inline ਖਾਲੀ mm_unset_cid_on_task(ਬਣਤਰ task_struct *t)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cid = t->mm_cid.cid;

	t->mm_cid.cid = MM_CID_UNSET;
	ਜੇ (cid_on_task(cid))
		mm_drop_cid(t->mm, cid);
}

ਸਥਿਰ __always_inline ਖਾਲੀ mm_drop_cid_on_cpu(ਬਣਤਰ mm_struct *mm, ਬਣਤਰ mm_cid_pcpu *pcp)
{
	/* Clear the ONCPU bit, but do not set UNSET in the per CPU storage */
	ਜੇ (cid_on_cpu(pcp->cid)) {
		pcp->cid = cpu_cid_ਮੰਜ਼ਿਲ_cid(pcp->cid);
		mm_drop_cid(mm, pcp->cid);
	}
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ __mm_get_cid(ਬਣਤਰ mm_struct *mm, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ max_cids)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cid = ਲੱਭੋ_ਪਹਿਲਾ_zero_ਦੋਆਧਾਰੀ_ਅੰਕ(mm_cidmask(mm), max_cids);

	ਜੇ (cid >= max_cids)
		ਵਾਪਸ MM_CID_UNSET;
	ਜੇ (test_and_ਸੈੱਟ_ਦੋਆਧਾਰੀ_ਅੰਕ(cid, mm_cidmask(mm)))
		ਵਾਪਸ MM_CID_UNSET;
	ਵਾਪਸ cid;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ mm_get_cid(ਬਣਤਰ mm_struct *mm)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cid = __mm_get_cid(mm, READ_ONCE(mm->mm_cid.max_cids));

	ਜਦੋਂ (cid == MM_CID_UNSET) {
		cpu_relax();
		cid = __mm_get_cid(mm, ਅੰਕ_possible_cpus());
	}
	ਵਾਪਸ cid;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ mm_cid_converge(ਬਣਤਰ mm_struct *mm, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ orig_cid,
					   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ max_cids)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ new_cid, cid = cpu_cid_ਮੰਜ਼ਿਲ_cid(orig_cid);

	/* Is it in the optimal CID space? */
	ਜੇ (likely(cid < max_cids))
		ਵਾਪਸ orig_cid;

	/* Try to find one in the optimal space. Otherwise keep the provided. */
	new_cid = __mm_get_cid(mm, max_cids);
	ਜੇ (new_cid != MM_CID_UNSET) {
		mm_drop_cid(mm, cid);
		/* Preserve the ONCPU mode of the original CID */
		ਵਾਪਸ new_cid | (orig_cid & MM_CID_ONCPU);
	}
	ਵਾਪਸ orig_cid;
}

ਸਥਿਰ __always_inline ਖਾਲੀ mm_cid_update_task_cid(ਬਣਤਰ task_struct *t, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cid)
{
	ਜੇ (t->mm_cid.cid != cid) {
		t->mm_cid.cid = cid;
		rseq_sched_ਸੈੱਟ_ids_ਬਦਲਿਆ(t);
	}
}

ਸਥਿਰ __always_inline ਖਾਲੀ mm_cid_update_pcpu_cid(ਬਣਤਰ mm_struct *mm, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cid)
{
	__this_cpu_write(mm->mm_cid.pcpu->cid, cid);
}

ਸਥਿਰ __always_inline ਖਾਲੀ mm_cid_from_cpu(ਬਣਤਰ task_struct *t, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cpu_cid,
					    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਬਹੁਲਕ)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ max_cids, tcid = t->mm_cid.cid;
	ਬਣਤਰ mm_struct *mm = t->mm;

	max_cids = READ_ONCE(mm->mm_cid.max_cids);
	/* Optimize for the common case where both have the ONCPU bit set */
	ਜੇ (likely(cid_on_cpu(cpu_cid & tcid))) {
		ਜੇ (likely(cpu_cid_ਮੰਜ਼ਿਲ_cid(cpu_cid) < max_cids)) {
			mm_cid_update_task_cid(t, cpu_cid);
			ਵਾਪਸ;
		}
		/* Try to converge into the optimal CID space */
		cpu_cid = mm_cid_converge(mm, cpu_cid, max_cids);
	} ਨਹੀਂਤਾਂ {
		/* Hand over or drop the task owned CID */
		ਜੇ (cid_on_task(tcid)) {
			ਜੇ (cid_on_cpu(cpu_cid))
				mm_unset_cid_on_task(t);
			ਨਹੀਂਤਾਂ
				cpu_cid = cid_ਮੰਜ਼ਿਲ_cpu_cid(tcid);
		}
		/* Still nothing, allocate a new one */
		ਜੇ (!cid_on_cpu(cpu_cid))
			cpu_cid = cid_ਮੰਜ਼ਿਲ_cpu_cid(mm_get_cid(mm));

		/* Handle the transition mode flag if required */
		ਜੇ (ਬਹੁਲਕ & MM_CID_TRANSIT)
			cpu_cid = cpu_cid_ਮੰਜ਼ਿਲ_cid(cpu_cid) | MM_CID_TRANSIT;
	}
	mm_cid_update_pcpu_cid(mm, cpu_cid);
	mm_cid_update_task_cid(t, cpu_cid);
}

ਸਥਿਰ __always_inline ਖਾਲੀ mm_cid_from_task(ਬਣਤਰ task_struct *t, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cpu_cid,
					     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਬਹੁਲਕ)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ max_cids, tcid = t->mm_cid.cid;
	ਬਣਤਰ mm_struct *mm = t->mm;

	max_cids = READ_ONCE(mm->mm_cid.max_cids);
	/* Optimize for the common case, where both have the ONCPU bit clear */
	ਜੇ (likely(cid_on_task(tcid | cpu_cid))) {
		ਜੇ (likely(tcid < max_cids)) {
			mm_cid_update_pcpu_cid(mm, tcid);
			ਵਾਪਸ;
		}
		/* Try to converge into the optimal CID space */
		tcid = mm_cid_converge(mm, tcid, max_cids);
	} ਨਹੀਂਤਾਂ {
		/* Hand over or drop the CPU owned CID */
		ਜੇ (cid_on_cpu(cpu_cid)) {
			ਜੇ (cid_on_task(tcid))
				mm_drop_cid_on_cpu(mm, this_cpu_ptr(mm->mm_cid.pcpu));
			ਨਹੀਂਤਾਂ
				tcid = cpu_cid_ਮੰਜ਼ਿਲ_cid(cpu_cid);
		}
		/* Still nothing, allocate a new one */
		ਜੇ (!cid_on_task(tcid))
			tcid = mm_get_cid(mm);
		/* Set the transition mode flag if required */
		tcid |= ਬਹੁਲਕ & MM_CID_TRANSIT;
	}
	mm_cid_update_pcpu_cid(mm, tcid);
	mm_cid_update_task_cid(t, tcid);
}

ਸਥਿਰ __always_inline ਖਾਲੀ mm_cid_schedin(ਬਣਤਰ task_struct *ਅਗਲਾ)
{
	ਬਣਤਰ mm_struct *mm = ਅਗਲਾ->mm;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cpu_cid, ਬਹੁਲਕ;

	ਜੇ (!ਅਗਲਾ->mm_cid.ਸਰਗਰਮ)
		ਵਾਪਸ;

	cpu_cid = __this_cpu_read(mm->mm_cid.pcpu->cid);
	ਬਹੁਲਕ = READ_ONCE(mm->mm_cid.ਬਹੁਲਕ);
	ਜੇ (likely(!cid_on_cpu(ਬਹੁਲਕ)))
		mm_cid_from_task(ਅਗਲਾ, cpu_cid, ਬਹੁਲਕ);
	ਨਹੀਂਤਾਂ
		mm_cid_from_cpu(ਅਗਲਾ, cpu_cid, ਬਹੁਲਕ);
}

ਸਥਿਰ __always_inline ਖਾਲੀ mm_cid_schedout(ਬਣਤਰ task_struct *prev)
{
	ਬਣਤਰ mm_struct *mm = prev->mm;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਬਹੁਲਕ, cid;

	/* During mode transitions CIDs are temporary and need to be dropped */
	ਜੇ (likely(!cid_ਵਿੱਚ_transit(prev->mm_cid.cid)))
		ਵਾਪਸ;

	ਬਹੁਲਕ = READ_ONCE(mm->mm_cid.ਬਹੁਲਕ);
	cid = cid_from_transit_cid(prev->mm_cid.cid);

	/*
	 * If transition mode is done, transfer ownership when the CID is
	 * within the convergence range to optimize the next schedule in.
	 */
	ਜੇ (!cid_ਵਿੱਚ_transit(ਬਹੁਲਕ) && cid < READ_ONCE(mm->mm_cid.max_cids)) {
		ਜੇ (cid_on_cpu(ਬਹੁਲਕ))
			cid = cid_ਮੰਜ਼ਿਲ_cpu_cid(cid);

		/* Update both so that the next schedule in goes into the fast path */
		mm_cid_update_pcpu_cid(mm, cid);
		prev->mm_cid.cid = cid;
	} ਨਹੀਂਤਾਂ {
		mm_drop_cid(mm, cid);
		prev->mm_cid.cid = MM_CID_UNSET;
	}
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ mm_cid_switch_ਮੰਜ਼ਿਲ(ਬਣਤਰ task_struct *prev, ਬਣਤਰ task_struct *ਅਗਲਾ)
{
	mm_cid_schedout(prev);
	mm_cid_schedin(ਅਗਲਾ);
}

#ਨਹੀਂਤਾਂ /* !CONFIG_SCHED_MM_CID: */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ mm_cid_switch_ਮੰਜ਼ਿਲ(ਬਣਤਰ task_struct *prev, ਬਣਤਰ task_struct *ਅਗਲਾ) { }
#ਜੇ_ਅੰਤ /* !CONFIG_SCHED_MM_CID */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CACHE
DECLARE_STATIC_ਕੁੰਜੀ_FALSE(sched_cache_present);
DECLARE_STATIC_ਕੁੰਜੀ_FALSE(sched_cache_ਸਰਗਰਮ);
ਬਾਹਰੀ ਪੂਰਨ sysctl_sched_cache_user;
ਬਾਹਰੀ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ llc_aggr_tolerance;
ਬਾਹਰੀ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ llc_epoch_period;
ਬਾਹਰੀ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ llc_epoch_affinity_timeout;
ਬਾਹਰੀ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ llc_imb_pct;
ਬਾਹਰੀ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ llc_overaggr_pct;

ਸਥਿਰ ਸਤਰਅੰਦਰ bool sched_cache_enabled(ਖਾਲੀ)
{
	ਵਾਪਸ static_branch_unlikely(&sched_cache_ਸਰਗਰਮ);
}

ਬਾਹਰੀ ਖਾਲੀ sched_cache_ਸਰਗਰਮ_ਸੈੱਟ(ਖਾਲੀ);

#ਜੇ_ਅੰਤ

ਖਾਲੀ sched_domains_free_llc_id(ਪੂਰਨ cpu);

ਬਾਹਰੀ ਖਾਲੀ init_sched_mm(ਬਣਤਰ task_struct *p);

ਬਾਹਰੀ u64 avg_vruntime(ਬਣਤਰ cfs_rq *cfs_rq);
ਬਾਹਰੀ ਪੂਰਨ entity_eligible(ਬਣਤਰ cfs_rq *cfs_rq, ਬਣਤਰ sched_entity *se);
ਸਥਿਰ ਸਤਰਅੰਦਰ
ਖਾਲੀ move_queued_task_locked(ਬਣਤਰ rq *src_rq, ਬਣਤਰ rq *dst_rq, ਬਣਤਰ task_struct *task)
{
	lockdep_assert_rq_held(src_rq);
	lockdep_assert_rq_held(dst_rq);

	deactivate_task(src_rq, task, 0);
	ਸੈੱਟ_task_cpu(task, dst_rq->cpu);
	activate_task(dst_rq, task, 0);
	wakeup_preempt(dst_rq, task, 0);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ
bool task_is_pushable(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, ਪੂਰਨ cpu)
{
	ਜੇ (!task_on_cpu(rq, p) &&
	    cpumask_test_cpu(cpu, &p->cpus_ਨਕਾਬ))
		ਵਾਪਸ true;

	ਵਾਪਸ false;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_RT_MUTEXES

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ __rt_effective_prio(ਬਣਤਰ task_struct *pi_task, ਪੂਰਨ prio)
{
	ਜੇ (pi_task)
		prio = min(prio, pi_task->prio);

	ਵਾਪਸ prio;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ rt_effective_prio(ਬਣਤਰ task_struct *p, ਪੂਰਨ prio)
{
	ਬਣਤਰ task_struct *pi_task = rt_mutex_get_ਸਿਖਰ_task(p);

	ਵਾਪਸ __rt_effective_prio(pi_task, prio);
}

#ਨਹੀਂਤਾਂ /* !CONFIG_RT_MUTEXES: */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ rt_effective_prio(ਬਣਤਰ task_struct *p, ਪੂਰਨ prio)
{
	ਵਾਪਸ prio;
}

#ਜੇ_ਅੰਤ /* !CONFIG_RT_MUTEXES */

ਬਾਹਰੀ ਪੂਰਨ __sched_setscheduler(ਬਣਤਰ task_struct *p, ਅਚਲ ਬਣਤਰ sched_attr *attr, bool user, bool pi);
ਬਾਹਰੀ ਪੂਰਨ __sched_setaffinity(ਬਣਤਰ task_struct *p, ਬਣਤਰ affinity_context *ctx);
ਬਾਹਰੀ ਅਚਲ ਬਣਤਰ sched_class *__setscheduler_class(ਪੂਰਨ policy, ਪੂਰਨ prio);
ਬਾਹਰੀ ਖਾਲੀ ਸੈੱਟ_ਲੱਦੋ_ਭਾਰ(ਬਣਤਰ task_struct *p, bool update_ਲੱਦੋ);
ਬਾਹਰੀ ਖਾਲੀ ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_task(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, ਪੂਰਨ flags);
ਬਾਹਰੀ bool ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_task(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, ਪੂਰਨ flags);

ਬਾਹਰੀ ਬਣਤਰ balance_callback *splice_balance_callbacks(ਬਣਤਰ rq *rq);

ਬਾਹਰੀ ਖਾਲੀ __balance_callbacks(ਬਣਤਰ rq *rq, ਬਣਤਰ rq_flags *rf);
ਬਾਹਰੀ ਖਾਲੀ balance_callbacks(ਬਣਤਰ rq *rq, ਬਣਤਰ balance_callback *ਸਿਰ);

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
ਬਣਤਰ sched_change_ctx {
	u64			prio;
	ਬਣਤਰ task_struct	*p;
	ਅਚਲ ਬਣਤਰ sched_class *class;
	ਪੂਰਨ			flags;
	bool			queued;
	bool			running;
};

ਬਣਤਰ sched_change_ctx *sched_change_begin(ਬਣਤਰ task_struct *p, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags);
ਖਾਲੀ sched_change_ਸਿਰਾ(ਬਣਤਰ sched_change_ctx *ctx);

DEFINE_CLASS(sched_change, ਬਣਤਰ sched_change_ctx *,
	     sched_change_ਸਿਰਾ(_T),
	     sched_change_begin(p, flags),
	     ਬਣਤਰ task_struct *p, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags)

DEFINE_CLASS_IS_UNCONDITIONAL(sched_change)

#ਅਨ੍ਤਰ੍ਭਾਵ "ext/ext.h"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_PREFERRED_CPU
ਖਾਲੀ sched_ਧੱਕੋ_current_non_preferred_cpu(ਬਣਤਰ rq *rq);
#ਨਹੀਂਤਾਂ	/* !CONFIG_PREFERRED_CPU */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ sched_ਧੱਕੋ_current_non_preferred_cpu(ਬਣਤਰ rq *rq) { }
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ /* _KERNEL_SCHED_SCHED_H */
