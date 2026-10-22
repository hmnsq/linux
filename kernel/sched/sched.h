/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Scheduler internal types and methods:
 */
#ifndef _KERNEL_SCHED_SCHED_H
#define _KERNEL_SCHED_SCHED_H

#ଅନ୍ତର୍ଭାଵ <linux/prandom.h>
#ଅନ୍ତର୍ଭାଵ <linux/sched/affinity.h>
#ଅନ୍ତର୍ଭାଵ <linux/sched/autogroup.h>
#ଅନ୍ତର୍ଭାଵ <linux/sched/cpufreq.h>
#ଅନ୍ତର୍ଭାଵ <linux/sched/deadline.h>
#ଅନ୍ତର୍ଭାଵ <linux/sched.h>
#ଅନ୍ତର୍ଭାଵ <linux/sched/loadavg.h>
#ଅନ୍ତର୍ଭାଵ <linux/sched/mm.h>
#ଅନ୍ତର୍ଭାଵ <linux/sched/rseq_api.h>
#ଅନ୍ତର୍ଭାଵ <linux/sched/signal.h>
#ଅନ୍ତର୍ଭାଵ <linux/sched/smt.h>
#ଅନ୍ତର୍ଭାଵ <linux/sched/stat.h>
#ଅନ୍ତର୍ଭାଵ <linux/sched/sysctl.h>
#ଅନ୍ତର୍ଭାଵ <linux/sched/task_flags.h>
#ଅନ୍ତର୍ଭାଵ <linux/sched/task.h>
#ଅନ୍ତର୍ଭାଵ <linux/sched/topology.h>
#ଅନ୍ତର୍ଭାଵ <linux/atomic.h>
#ଅନ୍ତର୍ଭାଵ <linux/bitmap.h>
#ଅନ୍ତର୍ଭାଵ <linux/bug.h>
#ଅନ୍ତର୍ଭାଵ <linux/capability.h>
#ଅନ୍ତର୍ଭାଵ <linux/cgroup_api.h>
#ଅନ୍ତର୍ଭାଵ <linux/cgroup.h>
#ଅନ୍ତର୍ଭାଵ <linux/context_tracking.h>
#ଅନ୍ତର୍ଭାଵ <linux/cpufreq.h>
#ଅନ୍ତର୍ଭାଵ <linux/cpumask_api.h>
#ଅନ୍ତର୍ଭାଵ <linux/cpuset.h>
#ଅନ୍ତର୍ଭାଵ <linux/ctype.h>
#ଅନ୍ତର୍ଭାଵ <linux/file.h>
#ଅନ୍ତର୍ଭାଵ <linux/fs_api.h>
#ଅନ୍ତର୍ଭାଵ <linux/hrtimer_api.h>
#ଅନ୍ତର୍ଭାଵ <linux/interrupt.h>
#ଅନ୍ତର୍ଭାଵ <linux/irq_work.h>
#ଅନ୍ତର୍ଭାଵ <linux/jiffies.h>
#ଅନ୍ତର୍ଭାଵ <linux/kref_api.h>
#ଅନ୍ତର୍ଭାଵ <linux/kthread.h>
#ଅନ୍ତର୍ଭାଵ <linux/ktime_api.h>
#ଅନ୍ତର୍ଭାଵ <linux/lockdep_api.h>
#ଅନ୍ତର୍ଭାଵ <linux/lockdep.h>
#ଅନ୍ତର୍ଭାଵ <linux/memblock.h>
#ଅନ୍ତର୍ଭାଵ <linux/memcontrol.h>
#ଅନ୍ତର୍ଭାଵ <linux/minmax.h>
#ଅନ୍ତର୍ଭାଵ <linux/mm.h>
#ଅନ୍ତର୍ଭାଵ <linux/module.h>
#ଅନ୍ତର୍ଭାଵ <linux/mutex_api.h>
#ଅନ୍ତର୍ଭାଵ <linux/plist.h>
#ଅନ୍ତର୍ଭାଵ <linux/poll.h>
#ଅନ୍ତର୍ଭାଵ <linux/proc_fs.h>
#ଅନ୍ତର୍ଭାଵ <linux/profile.h>
#ଅନ୍ତର୍ଭାଵ <linux/psi.h>
#ଅନ୍ତର୍ଭାଵ <linux/rcupdate.h>
#ଅନ୍ତର୍ଭାଵ <linux/seq_file.h>
#ଅନ୍ତର୍ଭାଵ <linux/seqlock.h>
#ଅନ୍ତର୍ଭାଵ <linux/softirq.h>
#ଅନ୍ତର୍ଭାଵ <linux/spinlock_api.h>
#ଅନ୍ତର୍ଭାଵ <linux/static_key.h>
#ଅନ୍ତର୍ଭାଵ <linux/stop_machine.h>
#ଅନ୍ତର୍ଭାଵ <linux/syscalls_api.h>
#ଅନ୍ତର୍ଭାଵ <linux/syscalls.h>
#ଅନ୍ତର୍ଭାଵ <linux/tick.h>
#ଅନ୍ତର୍ଭାଵ <linux/topology.h>
#ଅନ୍ତର୍ଭାଵ <linux/types.h>
#ଅନ୍ତର୍ଭାଵ <linux/u64_stats_sync_api.h>
#ଅନ୍ତର୍ଭାଵ <linux/uaccess.h>
#ଅନ୍ତର୍ଭାଵ <linux/vmstat.h>
#ଅନ୍ତର୍ଭାଵ <linux/wait_api.h>
#ଅନ୍ତର୍ଭାଵ <linux/wait_bit.h>
#ଅନ୍ତର୍ଭାଵ <linux/workqueue_api.h>
#ଅନ୍ତର୍ଭାଵ <linux/delayacct.h>
#ଅନ୍ତର୍ଭାଵ <linux/mmu_context.h>

#ଅନ୍ତର୍ଭାଵ <trace/events/power.h>
#ଅନ୍ତର୍ଭାଵ <trace/events/sched.h>

#ଅନ୍ତର୍ଭାଵ "../workqueue_internal.h"

ସଂରଚନା rq;
ସଂରଚନା cfs_rq;
ସଂରଚନା rt_rq;
ସଂରଚନା sched_group;
ସଂରଚନା cpuidle_state;

#ଯଦି defined(CONFIG_PARAVIRT) && !defined(CONFIG_HAVE_PV_STEAL_CLOCK_GEN)
# include <asm/paravirt.h>
#endif

#ଅନ୍ତର୍ଭାଵ <asm/barrier.h>

#ଅନ୍ତର୍ଭାଵ "cpupri.h"
#ଅନ୍ତର୍ଭାଵ "cpudeadline.h"

/* task_struct::on_rq states: */
#define TASK_ON_RQ_QUEUED	1
#define TASK_ON_RQ_MIGRATING	2

ବାହ୍ୟ __read_mostly ପୂର୍ଣ୍ଣସଂଖ୍ୟା scheduler_running;

ବାହ୍ୟ unsigned long calc_load_update;
ବାହ୍ୟ atomic_long_t calc_load_tasks;

ବାହ୍ୟ ଶୂନ୍ୟ calc_global_load_tick(ସଂରଚନା rq *this_rq);
ବାହ୍ୟ long calc_load_fold_active(ସଂରଚନା rq *this_rq, long adjust);

ବାହ୍ୟ ଶୂନ୍ୟ call_trace_sched_update_nr_running(ସଂରଚନା rq *rq, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2);

ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା sysctl_sched_rt_period;
ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା sysctl_sched_rt_runtime;
ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା sched_rr_timeslice;

/*
 * Asymmetric CPU capacity bits
 */
ସଂରଚନା asym_cap_data {
	ସଂରଚନା list_head link;
	ସଂରଚନା rcu_head rcu;
	unsigned long capacity;
	unsigned long cpus[];
};

ବାହ୍ୟ ସଂରଚନା list_head asym_cap_list;

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
	ଯଦି (__w)						\
		__w = max(2UL, __w >> SCHED_FIXEDPOINT_SHIFT);	\
	__w;							\
})
#ଅନ୍ୟଥା
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

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା idle_policy(ପୂର୍ଣ୍ଣସଂଖ୍ୟା policy)
{
	ଫେରନ୍ତୁ policy == SCHED_IDLE;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା normal_policy(ପୂର୍ଣ୍ଣସଂଖ୍ୟା policy)
{
#ifdef CONFIG_SCHED_CLASS_EXT
	ଯଦି (policy == SCHED_EXT)
		ଫେରନ୍ତୁ true;
#endif
	ଫେରନ୍ତୁ policy == SCHED_NORMAL;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା fair_policy(ପୂର୍ଣ୍ଣସଂଖ୍ୟା policy)
{
	ଫେରନ୍ତୁ normal_policy(policy) || policy == SCHED_BATCH;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା rt_policy(ପୂର୍ଣ୍ଣସଂଖ୍ୟା policy)
{
	ଫେରନ୍ତୁ policy == SCHED_FIFO || policy == SCHED_RR;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା dl_policy(ପୂର୍ଣ୍ଣସଂଖ୍ୟା policy)
{
	ଫେରନ୍ତୁ policy == SCHED_DEADLINE;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool valid_policy(ପୂର୍ଣ୍ଣସଂଖ୍ୟା policy)
{
	ଫେରନ୍ତୁ idle_policy(policy) || fair_policy(policy) ||
		rt_policy(policy) || dl_policy(policy);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା task_has_idle_policy(ସଂରଚନା task_struct *p)
{
	ଫେରନ୍ତୁ idle_policy(p->policy);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା task_has_rt_policy(ସଂରଚନା task_struct *p)
{
	ଫେରନ୍ତୁ rt_policy(p->policy);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା task_has_dl_policy(ସଂରଚନା task_struct *p)
{
	ଫେରନ୍ତୁ dl_policy(p->policy);
}

#define cap_scale(v, s)		((v)*(s) >> SCHED_CAPACITY_SHIFT)

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ update_avg(u64 *avg, u64 sample)
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
ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned long sched_weight_from_cgroup(unsigned long cgrp_weight)
{
	ଫେରନ୍ତୁ DIV_ROUND_CLOSEST_ULL(cgrp_weight * 1024, CGROUP_WEIGHT_DFL);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned long sched_weight_to_cgroup(unsigned long weight)
{
	ଫେରନ୍ତୁ clamp_t(unsigned long,
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

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool dl_entity_is_special(ଅଚଳ ସଂରଚନା sched_dl_entity *dl_se)
{
#ifdef CONFIG_CPU_FREQ_GOV_SCHEDUTIL
	ଫେରନ୍ତୁ unlikely(dl_se->flags & SCHED_FLAG_SUGOV);
#ଅନ୍ୟଥା
	ଫେରନ୍ତୁ false;
#endif
}

/*
 * Tells if entity @a should preempt entity @b.
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool dl_entity_preempt(ଅଚଳ ସଂରଚନା sched_dl_entity *a,
				     ଅଚଳ ସଂରଚନା sched_dl_entity *b)
{
	ଫେରନ୍ତୁ dl_entity_is_special(a) ||
	       dl_time_before(a->deadline, b->deadline);
}

/*
 * This is the priority-queue data structure of the RT scheduling class:
 */
ସଂରଚନା rt_prio_array {
	DECLARE_BITMAP(bitmap, MAX_RT_PRIO+1); /* include 1 bit for delimiter */
	ସଂରଚନା list_head queue[MAX_RT_PRIO];
};

ସଂରଚନା rt_bandwidth {
	/* nests inside the rq lock: */
	raw_spinlock_t		rt_runtime_lock;
	ktime_t			rt_period;
	u64			rt_runtime;
	ସଂରଚନା hrtimer		rt_period_timer;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		rt_period_active;
};

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା dl_bandwidth_enabled(ଶୂନ୍ୟ)
{
	ଫେରନ୍ତୁ sysctl_sched_rt_runtime >= 0;
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
ସଂରଚନା dl_bw {
	raw_spinlock_t		lock;
	u64			bw;
	u64			total_bw;
};

ବାହ୍ୟ ଶୂନ୍ୟ init_dl_bw(ସଂରଚନା dl_bw *dl_b);
ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା  sched_dl_global_validate(ଶୂନ୍ୟ);
ବାହ୍ୟ ଶୂନ୍ୟ sched_dl_do_global(ଶୂନ୍ୟ);
ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା  sched_dl_overflow(ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା policy, ଅଚଳ ସଂରଚନା sched_attr *attr);
ବାହ୍ୟ ଶୂନ୍ୟ __setparam_dl(ସଂରଚନା task_struct *p, ଅଚଳ ସଂରଚନା sched_attr *attr);
ବାହ୍ୟ ଶୂନ୍ୟ __getparam_dl(ସଂରଚନା task_struct *p, ସଂରଚନା sched_attr *attr, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags);
ବାହ୍ୟ bool __checkparam_dl(ଅଚଳ ସଂରଚନା sched_attr *attr);
ବାହ୍ୟ bool dl_param_changed(ସଂରଚନା task_struct *p, ଅଚଳ ସଂରଚନା sched_attr *attr);
ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା  dl_cpuset_cpumask_can_shrink(ଅଚଳ ସଂରଚନା cpumask *cur, ଅଚଳ ସଂରଚନା cpumask *trial);
ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା  dl_bw_deactivate(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu);
ବାହ୍ୟ s64 dl_scaled_delta_exec(ସଂରଚନା rq *rq, ସଂରଚନା sched_dl_entity *dl_se, s64 delta_exec);
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
ବାହ୍ୟ ଶୂନ୍ୟ dl_server_update_idle(ସଂରଚନା sched_dl_entity *dl_se, s64 delta_exec);
ବାହ୍ୟ ଶୂନ୍ୟ dl_server_update(ସଂରଚନା sched_dl_entity *dl_se, s64 delta_exec);
ବାହ୍ୟ ଶୂନ୍ୟ dl_server_start(ସଂରଚନା sched_dl_entity *dl_se);
ବାହ୍ୟ ଶୂନ୍ୟ dl_server_stop(ସଂରଚନା sched_dl_entity *dl_se);
ବାହ୍ୟ ଶୂନ୍ୟ dl_server_init(ସଂରଚନା sched_dl_entity *dl_se, ସଂରଚନା rq *rq,
		    dl_server_pick_f pick_task);
ବାହ୍ୟ ଶୂନ୍ୟ sched_init_dl_servers(ଶୂନ୍ୟ);

ବାହ୍ୟ ଶୂନ୍ୟ fair_server_init(ସଂରଚନା rq *rq);
ବାହ୍ୟ ଶୂନ୍ୟ ext_server_init(ସଂରଚନା rq *rq);
ବାହ୍ୟ ଶୂନ୍ୟ __dl_server_attach_root(ସଂରଚନା sched_dl_entity *dl_se, ସଂରଚନା rq *rq);
ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା dl_server_apply_params(ସଂରଚନା sched_dl_entity *dl_se,
		    u64 runtime, u64 period, bool init);
ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା dl_server_attach_bw(ସଂରଚନା sched_dl_entity *dl_se);
ବାହ୍ୟ ଶୂନ୍ୟ dl_server_detach_bw(ସଂରଚନା sched_dl_entity *dl_se);
ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା dl_server_swap_bw(ସଂରଚନା sched_dl_entity *detach_se,
			     ସଂରଚନା sched_dl_entity *attach_se);

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool dl_server_active(ସଂରଚନା sched_dl_entity *dl_se)
{
	ଫେରନ୍ତୁ dl_se->dl_server_active;
}

#ifdef CONFIG_CGROUP_SCHED

ବାହ୍ୟ ସଂରଚନା list_head task_groups;

#ifdef CONFIG_GROUP_SCHED_BANDWIDTH
ବାହ୍ୟ ଅଚଳ u64 max_bw_quota_period_us;

/*
 * default period for group bandwidth.
 * default: 0.1s, units: microseconds
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ u64 default_bw_period_us(ଶୂନ୍ୟ)
{
	ଫେରନ୍ତୁ 100000ULL;
}
#endif /* CONFIG_GROUP_SCHED_BANDWIDTH */

ସଂରଚନା cfs_bandwidth {
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
	ସଂରଚନା hrtimer		period_timer;
	ସଂରଚନା hrtimer		slack_timer;
	ସଂରଚନା list_head	throttled_cfs_rq;

	/* Statistics: */
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			nr_periods;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			nr_throttled;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			nr_burst;
	u64			throttled_time;
	u64			burst_time;
#endif /* CONFIG_CFS_BANDWIDTH */
};

/* Task group related information */
ସଂରଚନା task_group {
	ସଂରଚନା cgroup_subsys_state css;

#ifdef CONFIG_GROUP_SCHED_WEIGHT
	/* A positive value indicates that this is a SCHED_IDLE group. */
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			idle;
#endif

#ifdef CONFIG_FAIR_GROUP_SCHED
	/* runqueue "owned" by this group on each CPU */
	ସଂରଚନା cfs_rq __percpu	*cfs_rq;
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
	ସଂରଚନା sched_rt_entity	**rt_se;
	ସଂରଚନା rt_rq		**rt_rq;

	ସଂରଚନା rt_bandwidth	rt_bandwidth;
#endif

	ସଂରଚନା scx_task_group	scx;

	ସଂରଚନା rcu_head		rcu;
	ସଂରଚନା list_head	list;

	ସଂରଚନା task_group	*parent;
	ସଂରଚନା list_head	siblings;
	ସଂରଚନା list_head	children;

#ifdef CONFIG_SCHED_AUTOGROUP
	ସଂରଚନା autogroup	*autogroup;
#endif

	ସଂରଚନା cfs_bandwidth	cfs_bandwidth;

#ifdef CONFIG_UCLAMP_TASK_GROUP
	/* The two decimal precision [%] value requested from user-space */
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		uclamp_pct[UCLAMP_CNT];
	/* Clamp values requested for a task group */
	ସଂରଚନା uclamp_se	uclamp_req[UCLAMP_CNT];
	/* Effective clamp values used for a task group */
	ସଂରଚନା uclamp_se	uclamp[UCLAMP_CNT];
#endif

};

#define ମୂଳ_TASK_GROUP_LOAD	NICE_0_LOAD

ପ୍ରକାରବ୍ୟାଖ୍ୟା ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*tg_visitor)(ସଂରଚନା task_group *, ଶୂନ୍ୟ *);

ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା walk_tg_ବୃକ୍ଷ_from(ସଂରଚନା task_group *from,
			     tg_visitor down, tg_visitor up, ଶୂନ୍ୟ *data);

/*
 * Iterate the full tree, calling @down when first entering a node and @up when
 * leaving it for the final time.
 *
 * Caller must hold rcu_lock or sufficient equivalent.
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା walk_tg_ବୃକ୍ଷ(tg_visitor down, tg_visitor up, ଶୂନ୍ୟ *data)
{
	ଫେରନ୍ତୁ walk_tg_ବୃକ୍ଷ_from(&ମୂଳ_task_group, down, up, data);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା task_group *css_tg(ସଂରଚନା cgroup_subsys_state *css)
{
	ଫେରନ୍ତୁ css ? container_of(css, ସଂରଚନା task_group, css) : ଖାଲି;
}

ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା tg_nop(ସଂରଚନା task_group *tg, ଶୂନ୍ୟ *data);

#ifdef CONFIG_FAIR_GROUP_SCHED
ବାହ୍ୟ ଶୂନ୍ୟ free_fair_sched_group(ସଂରଚନା task_group *tg);
ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା alloc_fair_sched_group(ସଂରଚନା task_group *tg, ସଂରଚନା task_group *parent);
ବାହ୍ୟ ଶୂନ୍ୟ online_fair_sched_group(ସଂରଚନା task_group *tg);
ବାହ୍ୟ ଶୂନ୍ୟ unregister_fair_sched_group(ସଂରଚନା task_group *tg);
ବାହ୍ୟ ଶୂନ୍ୟ __sched_cgroup_mode_update(ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode);
#ଅନ୍ୟଥା /* !CONFIG_FAIR_GROUP_SCHED: */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ free_fair_sched_group(ସଂରଚନା task_group *tg) { }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା alloc_fair_sched_group(ସଂରଚନା task_group *tg, ସଂରଚନା task_group *parent)
{
       ଫେରନ୍ତୁ 1;
}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ online_fair_sched_group(ସଂରଚନା task_group *tg) { }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ unregister_fair_sched_group(ସଂରଚନା task_group *tg) { }
#endif /* !CONFIG_FAIR_GROUP_SCHED */

ବାହ୍ୟ ଶୂନ୍ୟ init_tg_cfs_entry(ସଂରଚନା task_group *tg, ସଂରଚନା cfs_rq *cfs_rq,
			ସଂରଚନା sched_entity *se, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu,
			ସଂରଚନା sched_entity *parent);
ବାହ୍ୟ ଶୂନ୍ୟ init_cfs_bandwidth(ସଂରଚନା cfs_bandwidth *cfs_b, ସଂରଚନା cfs_bandwidth *parent);

ବାହ୍ୟ ଶୂନ୍ୟ __refill_cfs_bandwidth_runtime(ସଂରଚନା cfs_bandwidth *cfs_b);
ବାହ୍ୟ ଶୂନ୍ୟ start_cfs_bandwidth(ସଂରଚନା cfs_bandwidth *cfs_b);
ବାହ୍ୟ ଶୂନ୍ୟ unthrottle_cfs_rq(ସଂରଚନା cfs_rq *cfs_rq);
ବାହ୍ୟ bool cfs_task_bw_constrained(ସଂରଚନା task_struct *p);

ବାହ୍ୟ ଶୂନ୍ୟ init_tg_rt_entry(ସଂରଚନା task_group *tg, ସଂରଚନା rt_rq *rt_rq,
		ସଂରଚନା sched_rt_entity *rt_se, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu,
		ସଂରଚନା sched_rt_entity *parent);
ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା sched_group_set_rt_runtime(ସଂରଚନା task_group *tg, long rt_runtime_us);
ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା sched_group_set_rt_period(ସଂରଚନା task_group *tg, u64 rt_period_us);
ବାହ୍ୟ long sched_group_rt_runtime(ସଂରଚନା task_group *tg);
ବାହ୍ୟ long sched_group_rt_period(ସଂରଚନା task_group *tg);
ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା sched_rt_can_attach(ସଂରଚନା task_group *tg, ସଂରଚନା task_struct *tsk);

ବାହ୍ୟ ସଂରଚନା task_group *sched_create_group(ସଂରଚନା task_group *parent);
ବାହ୍ୟ ଶୂନ୍ୟ sched_online_group(ସଂରଚନା task_group *tg,
			       ସଂରଚନା task_group *parent);
ବାହ୍ୟ ଶୂନ୍ୟ sched_destroy_group(ସଂରଚନା task_group *tg);
ବାହ୍ୟ ଶୂନ୍ୟ sched_release_group(ସଂରଚନା task_group *tg);

ବାହ୍ୟ ଶୂନ୍ୟ sched_move_task(ସଂରଚନା task_struct *tsk, bool for_autogroup);

#ifdef CONFIG_FAIR_GROUP_SCHED
ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା sched_group_set_shares(ସଂରଚନା task_group *tg, unsigned long shares);

ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା sched_group_set_idle(ସଂରଚନା task_group *tg, long idle);

ବାହ୍ୟ ଶୂନ୍ୟ set_task_rq_fair(ସଂରଚନା sched_entity *se,
			     ସଂରଚନା cfs_rq *prev, ସଂରଚନା cfs_rq *next);
#ଅନ୍ୟଥା /* !CONFIG_FAIR_GROUP_SCHED: */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା sched_group_set_shares(ସଂରଚନା task_group *tg, unsigned long shares) { ଫେରନ୍ତୁ 0; }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା sched_group_set_idle(ସଂରଚନା task_group *tg, long idle) { ଫେରନ୍ତୁ 0; }
#endif /* !CONFIG_FAIR_GROUP_SCHED */

#ଅନ୍ୟଥା /* !CONFIG_CGROUP_SCHED: */

ସଂରଚନା cfs_bandwidth { };

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool cfs_task_bw_constrained(ସଂରଚନା task_struct *p) { ଫେରନ୍ତୁ false; }

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

ବାହ୍ୟ ଶୂନ୍ୟ unregister_rt_sched_group(ସଂରଚନା task_group *tg);
ବାହ୍ୟ ଶୂନ୍ୟ free_rt_sched_group(ସଂରଚନା task_group *tg);
ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା alloc_rt_sched_group(ସଂରଚନା task_group *tg, ସଂରଚନା task_group *parent);

/*
 * u64_u32_load/u64_u32_store
 *
 * Use a copy of a u64 value to protect against data race. This is only
 * applicable for 32-bits architectures.
 */
#ifdef CONFIG_64BIT
# define u64_u32_load_copy(var, copy)		var
# define u64_u32_store_copy(var, copy, val)	(var = val)
#ଅନ୍ୟଥା
# define u64_u32_load_copy(var, copy)					\
({									\
	u64 __val, __val_copy;						\
	କର {								\
		__val_copy = copy;					\
		/*							\
		 * paired with u64_u32_store_copy(), ordering access	\
		 * to var and copy.					\
		 */							\
		smp_rmb();						\
		__val = var;						\
	} ଯେତେବେଳେ (__val != __val_copy);					\
	__val;								\
})
# define u64_u32_store_copy(var, copy, val)				\
କର {									\
	typeof(val) __val = (val);					\
	var = __val;							\
	/*								\
	 * paired with u64_u32_load_copy(), ordering access to var and	\
	 * copy.							\
	 */								\
	smp_wmb();							\
	copy = __val;							\
} ଯେତେବେଳେ (0)
#endif
# define u64_u32_load(var)		u64_u32_load_copy(var, var##_copy)
# define u64_u32_store(var, val)	u64_u32_store_copy(var, var##_copy, val)

ସଂରଚନା balance_callback {
	ସଂରଚନା balance_callback *next;
	ଶୂନ୍ୟ (*func)(ସଂରଚନା rq *rq);
};

/* Fair scheduling SCHED_{NORMAL,BATCH,IDLE} related fields in a runqueue: */
ସଂରଚନା cfs_rq {
	ସଂରଚନା load_weight	load;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		nr_queued;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		h_nr_queued;		/* SCHED_{NORMAL,BATCH,IDLE} */
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		h_nr_runnable;		/* SCHED_{NORMAL,BATCH,IDLE} */
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		h_nr_idle;		/* SCHED_IDLE */

	s64			sum_w_vruntime;
	u64			sum_weight;
	u64			zero_vruntime;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		sum_shift;

#ifdef CONFIG_SCHED_CORE
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		forceidle_seq;
	u64			zero_vruntime_fi;
#endif

	ସଂରଚନା rb_ମୂଳ_cached	tasks_timeline;

	/*
	 * 'curr' points to the currently running entity on this cfs_rq.
	 * It is set to NULL otherwise (i.e when none are currently running).
	 */
	ସଂରଚନା sched_entity	*curr;
	ସଂରଚନା sched_entity	*next;

	/*
	 * CFS load tracking
	 */
	ସଂରଚନା sched_entity	*h_curr;
	ସଂରଚନା sched_avg	avg;
#ifndef CONFIG_64BIT
	u64			last_update_time_copy;
#endif
	ସଂରଚନା {
		raw_spinlock_t	lock ____cacheline_aligned;
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା		nr;
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
	ସଂରଚନା sched_entity	*h_load_next;

	ସଂରଚନା rq		*rq;	/* CPU runqueue to which this cfs_rq is attached */

	/*
	 * leaf cfs_rqs are those that hold tasks (lowest schedulable entity in
	 * a hierarchy). Non-leaf lrqs hold other higher schedulable entities
	 * (like users, containers etc.)
	 *
	 * leaf_cfs_rq_list ties together list of leaf cfs_rq's in a CPU.
	 * This list is used during load balance.
	 */
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			on_list;
	ସଂରଚନା list_head	leaf_cfs_rq_list;
	ସଂରଚନା task_group	*tg;	/* Group that "owns" this runqueue */

	/* Locally cached copy of our task_group's idle value */
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			idle;

# ifdef CONFIG_CFS_BANDWIDTH
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			runtime_enabled;
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
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			throttle_ଗଣନା;
	ସଂରଚନା list_head	throttled_list;
	ସଂରଚନା list_head	throttled_csd_list;
	ସଂରଚନା list_head        throttled_limbo_list;
# endif /* CONFIG_CFS_BANDWIDTH */
#endif /* CONFIG_FAIR_GROUP_SCHED */
};

#ifdef CONFIG_SCHED_CLASS_EXT
/* scx_rq->flags, protected by the rq lock */
ଗଣନା scx_rq_flags {
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
	SCX_RQ_ମୂଳ_IDLE_RENOTIFY	= 1 << 8, /* the root is owed update_idle() */

	SCX_RQ_IN_WAKEUP	= 1 << 16,
	SCX_RQ_IN_DISPATCH	= 1 << 17,
};

/* per-rq rescue execution state, see scx_rescue_timerfn() */
ସଂରଚନା scx_rq_rescue {
	ସଂରଚନା scx_dispatch_q	dsq;			/* stranded tasks awaiting rescue */
	s64			budget;			/* execution token bucket, ns */
	u64			clock;			/* last budget accrual timestamp */
	ସଂରଚନା task_struct	*curr;			/* task being rescued, one at a time */
	s64			slice;			/* curr's admitted slice */
	u64			exec_snap;		/* sum_exec_runtime at admission */
	ସଂରଚନା timer_list	timer;			/* paces admission and escalation */
	u64			kill_at;		/* last ejection, init before any */
};

ସଂରଚନା scx_rq {
	ସଂରଚନା scx_dispatch_q	local_dsq;
#ifdef CONFIG_EXT_SUB_SCHED
	ସଂରଚନା scx_dispatch_q	reject_dsq;		/* staging for cap-rejected tasks */
	ସଂରଚନା scx_rq_rescue	rescue;
#endif
	ସଂରଚନା list_head	runnable_list;		/* runnable tasks on this rq */
	ସଂରଚନା list_head	ddsp_deferred_locals;	/* deferred ddsps from enq */
	/* both stashed across the activate_task() in move_remote_task_to_local_dsq() */
	u64			remote_activate_enq_flags;
	ସଂରଚନା scx_sched	*remote_activate_sch;
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
	ସଂରଚନା llist_head	ecaps_to_sync;		/* pending ecaps syncs */
	ସଂରଚନା task_struct	*sub_dispatch_prev;
#endif
	cpumask_var_t		cpus_to_sync;
	bool			kick_sync_pending;
	unsigned long		kick_sync;

	ସଂରଚନା list_head	sched_pcpus_to_kick;	/* see kick_cpus_irq_workfn() */

	raw_spinlock_t		deferred_reenq_lock;
	ସଂରଚନା list_head	deferred_reenq_locals;	/* scheds requesting reenq of local DSQ */
	ସଂରଚନା list_head	deferred_reenq_users;	/* user DSQs requesting reenq */
	ସଂରଚନା balance_callback	deferred_bal_cb;
	ସଂରଚନା balance_callback	kick_sync_bal_cb;
	ସଂରଚନା irq_work		deferred_irq_work;
	ସଂରଚନା irq_work		kick_cpus_irq_work;
};
#endif /* CONFIG_SCHED_CLASS_EXT */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା rt_bandwidth_enabled(ଶୂନ୍ୟ)
{
	ଫେରନ୍ତୁ sysctl_sched_rt_runtime >= 0;
}

/* RT IPI pull logic requires IRQ_WORK */
#ଯଦି defined(CONFIG_IRQ_WORK) && defined(CONFIG_SMP)
# define HAVE_RT_ଠେଲ_IPI
#endif

/* Real-Time classes' related field in a runqueue: */
ସଂରଚନା rt_rq {
	ସଂରଚନା rt_prio_array	active;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		rt_nr_running;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		rr_nr_running;
	ସଂରଚନା {
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା		curr; /* highest queued rt task prio */
		ପୂର୍ଣ୍ଣସଂଖ୍ୟା		next; /* next highest */
	} highest_prio;
	bool			overloaded;
	ସଂରଚନା plist_head	pushable_tasks;

	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			rt_queued;

#ifdef CONFIG_RT_GROUP_SCHED
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			rt_throttled;
	u64			rt_time; /* consumed RT time, goes up in update_curr_rt */
	u64			rt_runtime; /* allotted RT time, "slice" from rt_bandwidth, RT sharing/balancing */
	/* Nests inside the rq lock: */
	raw_spinlock_t		rt_runtime_lock;

	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		rt_nr_boosted;

	ସଂରଚନା rq		*rq; /* this is always top-level rq, cache? */
#endif
#ifdef CONFIG_CGROUP_SCHED
	ସଂରଚନା task_group	*tg; /* this tg has "this" rt_rq on given CPU for runnable entities */
#endif
};

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool rt_rq_is_runnable(ସଂରଚନା rt_rq *rt_rq)
{
	ଫେରନ୍ତୁ rt_rq->rt_queued && rt_rq->rt_nr_running;
}

/* Deadline class' related fields in a runqueue */
ସଂରଚନା dl_rq {
	/* runqueue is an rbtree, ordered by deadline */
	ସଂରଚନା rb_ମୂଳ_cached	ମୂଳ;

	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		dl_nr_running;

	/*
	 * Deadline values of the currently executing and the
	 * earliest ready task on this rq. Caching these facilitates
	 * the decision whether or not a ready but not running task
	 * should migrate somewhere else.
	 */
	ସଂରଚନା {
		u64		curr;
		u64		next;
	} earliest_dl;

	bool			overloaded;

	ସଂରଚନା sched_dl_entity	*curr;
	/*
	 * Tasks on this rq that can be pushed away. They are kept in
	 * an rb-tree, ordered by tasks' deadlines, with caching
	 * of the leftmost (earliest deadline) element.
	 */
	ସଂରଚନା rb_ମୂଳ_cached	pushable_dl_tasks_ମୂଳ;

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
#define is_ମୂଳ_task_group(tg) ((tg) == &ମୂଳ_task_group)
/* An entity is a task if it doesn't "own" a runqueue */
#define entity_is_task(se)	(!se->my_q)

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ se_update_runnable(ସଂରଚନା sched_entity *se)
{
	ଯଦି (!entity_is_task(se))
		se->runnable_weight = se->my_q->h_nr_runnable;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ long se_runnable(ସଂରଚନା sched_entity *se)
{
	ଯଦି (se->sched_delayed)
		ଫେରନ୍ତୁ false;

	ଯଦି (entity_is_task(se))
		ଫେରନ୍ତୁ !!se->on_rq;
	ଅନ୍ୟଥା
		ଫେରନ୍ତୁ se->runnable_weight;
}

#ଅନ୍ୟଥା /* !CONFIG_FAIR_GROUP_SCHED: */

#define entity_is_task(se)	1

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ se_update_runnable(ସଂରଚନା sched_entity *se) { }

ସ୍ଥିର ଧାଡ଼ିଭିତରେ long se_runnable(ସଂରଚନା sched_entity *se)
{
	ଯଦି (se->sched_delayed)
		ଫେରନ୍ତୁ false;

	ଫେରନ୍ତୁ !!se->on_rq;
}

#endif /* !CONFIG_FAIR_GROUP_SCHED */

/*
 * XXX we want to get rid of these helpers and use the full load resolution.
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ long se_weight(ସଂରଚନା sched_entity *se)
{
	ଫେରନ୍ତୁ scale_load_down(se->load.weight);
}


ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool sched_asym_prefer(ପୂର୍ଣ୍ଣସଂଖ୍ୟା a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା b)
{
	ଫେରନ୍ତୁ arch_asym_cpu_priority(a) > arch_asym_cpu_priority(b);
}

ସଂରଚନା perf_domain {
	ସଂରଚନା em_perf_domain *em_pd;
	ସଂରଚନା perf_domain *next;
	ସଂରଚନା rcu_head rcu;
};

/*
 * We add the notion of a root-domain which will be used to define per-domain
 * variables. Each exclusive cpuset essentially defines an island domain by
 * fully partitioning the member CPUs from any other cpuset. Whenever a new
 * exclusive cpuset is created, we also create and attach a new root-domain
 * object.
 *
 */
ସଂରଚନା ମୂଳ_domain {
	atomic_t		refcount;
	atomic_t		rto_ଗଣନା;
	ସଂରଚନା rcu_head		rcu;
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
	atomic_t		dlo_ଗଣନା;
	ସଂରଚନା dl_bw		dl_bw;
	ସଂରଚନା cpudl		cpudl;

	/*
	 * Indicate whether a root_domain's dl_bw has been checked or
	 * updated. It's monotonously increasing value.
	 *
	 * Also, some corner cases, like 'wrap around' is dangerous, but given
	 * that u64 is 'big enough'. So that shouldn't be a concern.
	 */
	u64 visit_cookie;

#ifdef HAVE_RT_ଠେଲ_IPI
	/*
	 * For IPI pull requests, loop across the rto_mask.
	 */
	ସଂରଚନା irq_work		rto_ଠେଲ_work;
	raw_spinlock_t		rto_lock;
	/* These are only updated and read within rto_lock */
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			rto_loop;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			rto_cpu;
	/* These atomics are updated outside of a lock */
	atomic_t		rto_loop_next;
	atomic_t		rto_loop_start;
#endif /* HAVE_RT_PUSH_IPI */
	/*
	 * The "RT overload" flag: it gets set if a CPU has more than
	 * one runnable RT task.
	 */
	cpumask_var_t		rto_mask;
	ସଂରଚନା cpupri		cpupri;

	/*
	 * NULL-terminated list of performance domains intersecting with the
	 * CPUs of the rd. Protected by RCU.
	 */
	ସଂରଚନା perf_domain __rcu *pd;
};

ବାହ୍ୟ ଶୂନ୍ୟ init_defrootdomain(ଶୂନ୍ୟ);
ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା sched_init_domains(ଅଚଳ ସଂରଚନା cpumask *cpu_map);
ବାହ୍ୟ ଶୂନ୍ୟ rq_attach_ମୂଳ(ସଂରଚନା rq *rq, ସଂରଚନା ମୂଳ_domain *rd);
ବାହ୍ୟ ଶୂନ୍ୟ sched_get_rd(ସଂରଚନା ମୂଳ_domain *rd);
ବାହ୍ୟ ଶୂନ୍ୟ sched_put_rd(ସଂରଚନା ମୂଳ_domain *rd);

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_rd_overloaded(ସଂରଚନା ମୂଳ_domain *rd)
{
	ଫେରନ୍ତୁ READ_ONCE(rd->overloaded);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ set_rd_overloaded(ସଂରଚନା ମୂଳ_domain *rd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
	ଯଦି (get_rd_overloaded(rd) != status)
		WRITE_ONCE(rd->overloaded, status);
}

#ifdef HAVE_RT_ଠେଲ_IPI
ବାହ୍ୟ ଶୂନ୍ୟ rto_ଠେଲ_irq_work_func(ସଂରଚନା irq_work *work);
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
ସଂରଚନା uclamp_bucket {
	unsigned long ମୂଲ୍ୟ : bits_per(SCHED_CAPACITY_SCALE);
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
ସଂରଚନା uclamp_rq {
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୂଲ୍ୟ;
	ସଂରଚନା uclamp_bucket bucket[UCLAMP_BUCKETS];
};

DECLARE_STATIC_ଚାବି_FALSE(sched_uclamp_used);
#endif /* CONFIG_UCLAMP_TASK */

/*
 * This is the main, per-CPU runqueue data structure.
 *
 * Locking rule: those places that want to lock multiple runqueues
 * (such as the load balancing or the thread migration code), lock
 * acquire operations must be ordered by ascending &runqueue.
 */
ସଂରଚନା rq {
	/*
	 * The following members are loaded together, without holding the
	 * rq->lock, in an extremely hot loop in update_sg_lb_stats()
	 * (called from pick_next_task()). To reduce cache pollution from
	 * this operation, they are placed together on this dedicated cache
	 * line. Even though some of them are frequently modified, they are
	 * loaded much more frequently than they are stored.
	 */
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		nr_running;
#ifdef CONFIG_NUMA_BALANCING
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		nr_numa_running;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		nr_preferred_running;
#endif
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		ttwu_pending;
	unsigned long		cpu_capacity;
#ifdef CONFIG_SCHED_PROXY_EXEC
	ସଂରଚନା task_struct __rcu	*donor;  /* Scheduling context */
	ସଂରଚନା task_struct __rcu	*curr;   /* Execution context */
#ଅନ୍ୟଥା
	ସଂଘ {
		ସଂରଚନା task_struct __rcu *donor; /* Scheduler context */
		ସଂରଚନା task_struct __rcu *curr;  /* Execution context */
	};
#endif
	ସଂରଚନା task_struct	*idle;
	/* padding left here deliberately */

	/*
	 * The next cacheline holds the (hot) runqueue lock, as well as
	 * some other less performance-critical fields.
	 */
	u64			nr_switches	____cacheline_aligned;

	/* runqueue lock: */
	raw_spinlock_t		__lock;

#ifdef CONFIG_NO_HZ_COMMON
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		nohz_tick_stopped;
	atomic_t		nohz_flags;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		has_blocked_load;
	unsigned long		last_blocked_load_update_tick;
	call_single_data_t	nohz_csd;
#endif /* CONFIG_NO_HZ_COMMON */

#ifdef CONFIG_UCLAMP_TASK
	/* Utilization clamp values based on CPU's RUNNABLE tasks */
	ସଂରଚନା uclamp_rq	uclamp[UCLAMP_CNT] ____cacheline_aligned;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		uclamp_flags;
#define UCLAMP_FLAG_IDLE 0x01
#endif

	ସଂରଚନା cfs_rq		cfs;
	ସଂରଚନା rt_rq		rt;
	ସଂରଚନା dl_rq		dl;
#ifdef CONFIG_SCHED_CLASS_EXT
	ସଂରଚନା scx_rq		scx;
	ସଂରଚନା sched_dl_entity	ext_server;
#endif
#ifdef CONFIG_SCHED_CACHE
	raw_spinlock_t		cpu_epoch_lock ____cacheline_aligned;
	u64			cpu_runtime;
	unsigned long		cpu_epoch;
	unsigned long		cpu_epoch_next;
#endif

	ସଂରଚନା sched_dl_entity	fair_server;

#ifdef CONFIG_FAIR_GROUP_SCHED
	/* list of leaf cfs_rq on this CPU: */
	ସଂରଚନା list_head	leaf_cfs_rq_list;
	ସଂରଚନା list_head	*tmp_alone_branch;
#endif /* CONFIG_FAIR_GROUP_SCHED */

#ifdef CONFIG_NUMA_BALANCING
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		numa_migrate_on;
#endif

#ifdef CONFIG_SCHED_CACHE
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		nr_pref_llc_running;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		nr_llc_running;
#endif

	/*
	 * This is part of a global counter where only the total sum
	 * over all CPUs matters. A task can increase this counter on
	 * one CPU and if it got migrated afterwards it may decrease
	 * it on another CPU. Always updated under the runqueue lock:
	 */
	unsigned long		nr_uninterruptible;

	ସଂରଚନା sched_dl_entity	*dl_server;
	ସଂରଚନା task_struct	*stop;
	ଅଚଳ ସଂରଚନା sched_class *next_class;
	unsigned long		next_balance;
	ସଂରଚନା mm_struct	*prev_mm;

	/*
	 * The following fields of clock data are frequently referenced
	 * and updated together, and should go on their own cache line.
	 */
	u64			clock_task ____cacheline_aligned;
	u64			clock_pelt;
	u64			clock;
	unsigned long		lost_idle_time;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		clock_update_flags;
	u64			clock_pelt_idle;
	u64			clock_idle;

#ifndef CONFIG_64BIT
	u64			clock_pelt_idle_copy;
	u64			clock_idle_copy;
#endif

	u64 last_seen_need_resched_ns;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା ticks_without_resched;

#ifdef CONFIG_MEMBARRIER
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା membarrier_state;
#endif

	ସଂରଚନା ମୂଳ_domain		*rd;
	ସଂରଚନା sched_domain __rcu	*sd;

	ସଂରଚନା balance_callback *balance_callback;

	unsigned ଅକ୍ଷର		nohz_idle_balance;
	unsigned ଅକ୍ଷର		idle_balance;

	unsigned long		misfit_task_load;

	/* For active balancing */
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			active_balance;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			ଠେଲ_cpu;
	ସଂରଚନା cpu_stop_work	active_balance_work;

	/* CPU of this runqueue: */
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			cpu;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			online;

	ସଂରଚନା list_head cfs_tasks;

	ସଂରଚନା sched_avg	avg_rt;
	ସଂରଚନା sched_avg	avg_dl;
#ifdef CONFIG_HAVE_SCHED_AVG_IRQ
	ସଂରଚନା sched_avg	avg_irq;
#endif
#ifdef CONFIG_SCHED_HW_PRESSURE
	ସଂରଚନା sched_avg	avg_hw;
#endif
	u64			idle_stamp;
	u64			avg_idle;

	/* This is used to determine avg_idle's max value */
	u64			max_idle_balance_cost;

#ifdef CONFIG_HOTPLUG_CPU
	ସଂରଚନା rcuwait		hotplug_wait;
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
	bool			npc_ଠେଲ_work_pending;
#endif

	/* calc_load related fields */
	unsigned long		calc_load_update;
	long			calc_load_active;

#ifdef CONFIG_SCHED_HRTICK
	call_single_data_t	hrtick_csd;
	ସଂରଚନା hrtimer		hrtick_timer;
	ktime_t			hrtick_time;
	ktime_t			hrtick_delay;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		hrtick_sched;
#endif

#ifdef CONFIG_SCHEDSTATS
	/* latency stats */
	ସଂରଚନା sched_info	rq_sched_info;
	unsigned long long	rq_cpu_time;

	/* sys_sched_yield() stats */
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		yld_ଗଣନା;

	/* schedule() stats */
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		sched_ଗଣନା;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		sched_goidle;

	/* try_to_wake_up() stats */
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		ttwu_ଗଣନା;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		ttwu_local;
#endif

#ifdef CONFIG_CPU_IDLE
	/* Must be inspected within a RCU lock section */
	ସଂରଚନା cpuidle_state	*idle_state;
#endif

	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		nr_pinned;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		ଠେଲ_busy;
	ସଂରଚନା cpu_stop_work	ଠେଲ_work;

#ifdef CONFIG_SCHED_CORE
	/* per rq */
	ସଂରଚନା rq		*core;
	ସଂରଚନା task_struct	*core_pick;
	ସଂରଚନା sched_dl_entity	*core_dl_server;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		core_enabled;
	ସଂରଚନା rb_ମୂଳ		core_ବୃକ୍ଷ;

	/* shared state -- careful with sched_core_cpu_deactivate() */
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		core_task_seq;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		core_pick_seq;
	unsigned long		core_cookie;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		core_forceidle_ଗଣନା;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		core_forceidle_seq;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		core_forceidle_occupation;
	u64			core_forceidle_start;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		core_pick_in_flight;
#endif /* CONFIG_SCHED_CORE */

	/* Scratch cpumask to be temporarily used under rq_lock */
	cpumask_var_t		scratch_mask;

#ifdef CONFIG_CFS_BANDWIDTH
	call_single_data_t	cfsb_csd;
	ସଂରଚନା list_head	cfsb_csd_list;
#endif

	atomic_t		nr_iowait;
} __no_randomize_layout;

#ifdef CONFIG_FAIR_GROUP_SCHED

/* CPU runqueue to which this cfs_rq is attached */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା rq *rq_of(ସଂରଚନା cfs_rq *cfs_rq)
{
	ଫେରନ୍ତୁ cfs_rq->rq;
}

#ଅନ୍ୟଥା /* !CONFIG_FAIR_GROUP_SCHED: */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା rq *rq_of(ସଂରଚନା cfs_rq *cfs_rq)
{
	ଫେରନ୍ତୁ container_of(cfs_rq, ସଂରଚନା rq, cfs);
}
#endif /* !CONFIG_FAIR_GROUP_SCHED */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu_of(ସଂରଚନା rq *rq)
{
	ଫେରନ୍ତୁ rq->cpu;
}

#define MDF_ଠେଲ		0x01

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool is_migration_disabled(ସଂରଚନା task_struct *p)
{
	ଫେରନ୍ତୁ p->migration_disabled;
}

DECLARE_PER_CPU_SHARED_ALIGNED(ସଂରଚନା rq, runqueues);
DECLARE_PER_CPU(ସଂରଚନା rnd_state, sched_rnd_state);

ସ୍ଥିର ଧାଡ଼ିଭିତରେ u32 sched_rng(ଶୂନ୍ୟ)
{
	ଫେରନ୍ତୁ prandom_u32_state(this_cpu_ptr(&sched_rnd_state));
}

ସ୍ଥିର __always_inline ସଂରଚନା rq *__this_rq(ଶୂନ୍ୟ)
{
	ଫେରନ୍ତୁ this_cpu_ptr(&runqueues);
}

#define cpu_rq(cpu)		(&per_cpu(runqueues, (cpu)))
#define this_rq()		__this_rq()
#define task_rq(p)		cpu_rq(task_cpu(p))
#define cpu_curr(cpu)		(cpu_rq(cpu)->curr)
#define raw_rq()		raw_cpu_ptr(&runqueues)

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool idle_rq(ସଂରଚନା rq *rq)
{
	ଫେରନ୍ତୁ rq->curr == rq->idle && !rq->nr_running && !rq->ttwu_pending;
}

/**
 * available_idle_cpu - is a given CPU idle for enqueuing work.
 * @cpu: the CPU in question.
 *
 * Return: 1 if the CPU is currently idle. 0 otherwise.
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool available_idle_cpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ଯଦି (!idle_rq(cpu_rq(cpu)))
		ଫେରନ୍ତୁ 0;

	ଯଦି (vcpu_is_preempted(cpu))
		ଫେରନ୍ତୁ 0;

	ଫେରନ୍ତୁ 1;
}

#ifdef CONFIG_SCHED_PROXY_EXEC
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ rq_set_donor(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *t)
{
	rcu_assign_pointer(rq->donor, t);
}
#ଅନ୍ୟଥା
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ rq_set_donor(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *t)
{
	/* Do nothing */
}
#endif

#ifdef CONFIG_SCHED_CORE
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା cpumask *sched_group_span(ସଂରଚନା sched_group *sg);

DECLARE_STATIC_ଚାବି_FALSE(__sched_core_enabled);

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool sched_core_enabled(ସଂରଚନା rq *rq)
{
	ଫେରନ୍ତୁ static_branch_unlikely(&__sched_core_enabled) && rq->core_enabled;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool sched_core_disabled(ଶୂନ୍ୟ)
{
	ଫେରନ୍ତୁ !static_branch_unlikely(&__sched_core_enabled);
}

/*
 * Be careful with this function; not for general use. The return value isn't
 * stable unless you actually hold a relevant rq->__lock.
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ raw_spinlock_t *rq_lockp(ସଂରଚନା rq *rq)
{
	ଯଦି (sched_core_enabled(rq))
		ଫେରନ୍ତୁ &rq->core->__lock;

	ଫେରନ୍ତୁ &rq->__lock;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ raw_spinlock_t *__rq_lockp(ସଂରଚନା rq *rq)
	__returns_ctx_lock(rq_lockp(rq)) /* alias them */
{
	ଯଦି (rq->core_enabled)
		ଫେରନ୍ତୁ &rq->core->__lock;

	ଫେରନ୍ତୁ &rq->__lock;
}

ବାହ୍ୟ bool
cfs_prio_less(ଅଚଳ ସଂରଚନା task_struct *a, ଅଚଳ ସଂରଚନା task_struct *b, bool fi);

ବାହ୍ୟ ଶୂନ୍ୟ task_vruntime_update(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p, bool in_fi);

/*
 * Helpers to check if the CPU's core cookie matches with the task's cookie
 * when core scheduling is enabled.
 * A special case is that the task's cookie always matches with CPU's core
 * cookie if the CPU is in an idle core.
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool sched_cpu_cookie_match(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p)
{
	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	ଯଦି (!sched_core_enabled(rq))
		ଫେରନ୍ତୁ true;

	ଫେରନ୍ତୁ rq->core->core_cookie == p->core_cookie;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool sched_core_cookie_match(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p)
{
	bool idle_core = true;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu;

	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	ଯଦି (!sched_core_enabled(rq))
		ଫେରନ୍ତୁ true;

	ଯଦି (rq->core->core_cookie == p->core_cookie)
		ଫେରନ୍ତୁ true;

	for_each_cpu(cpu, cpu_smt_mask(cpu_of(rq))) {
		ଯଦି (!available_idle_cpu(cpu)) {
			idle_core = false;
			ଭାଙ୍ଗ;
		}
	}

	/*
	 * A CPU in an idle core is always the best choice for tasks with
	 * cookies.
	 */
	ଫେରନ୍ତୁ idle_core;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool sched_group_cookie_match(ସଂରଚନା rq *rq,
					    ସଂରଚନା task_struct *p,
					    ସଂରଚନା sched_group *group)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu;

	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	ଯଦି (!sched_core_enabled(rq))
		ଫେରନ୍ତୁ true;

	for_each_cpu_and(cpu, sched_group_span(group), p->cpus_ptr) {
		ଯଦି (sched_core_cookie_match(cpu_rq(cpu), p))
			ଫେରନ୍ତୁ true;
	}
	ଫେରନ୍ତୁ false;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool sched_core_enqueued(ସଂରଚନା task_struct *p)
{
	ଫେରନ୍ତୁ !RB_EMPTY_ଗଣ୍ଠି(&p->core_ଗଣ୍ଠି);
}

ବାହ୍ୟ ଶୂନ୍ୟ sched_core_enqueue(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p);
ବାହ୍ୟ ଶୂନ୍ୟ sched_core_dequeue(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags);

ବାହ୍ୟ ଶୂନ୍ୟ sched_core_get(ଶୂନ୍ୟ);
ବାହ୍ୟ ଶୂନ୍ୟ sched_core_put(ଶୂନ୍ୟ);

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool task_has_sched_core(ସଂରଚନା task_struct *p)
{
	ଯଦି (sched_core_disabled())
		ଫେରନ୍ତୁ false;

	ଫେରନ୍ତୁ !!p->core_cookie;
}

#ଅନ୍ୟଥା /* !CONFIG_SCHED_CORE: */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool sched_core_enabled(ସଂରଚନା rq *rq)
{
	ଫେରନ୍ତୁ false;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool sched_core_disabled(ଶୂନ୍ୟ)
{
	ଫେରନ୍ତୁ true;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ raw_spinlock_t *rq_lockp(ସଂରଚନା rq *rq)
{
	ଫେରନ୍ତୁ &rq->__lock;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ raw_spinlock_t *__rq_lockp(ସଂରଚନା rq *rq)
	__returns_ctx_lock(rq_lockp(rq)) /* alias them */
{
	ଫେରନ୍ତୁ &rq->__lock;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool sched_cpu_cookie_match(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p)
{
	ଫେରନ୍ତୁ true;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool sched_core_cookie_match(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p)
{
	ଫେରନ୍ତୁ true;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool sched_group_cookie_match(ସଂରଚନା rq *rq,
					    ସଂରଚନା task_struct *p,
					    ସଂରଚନା sched_group *group)
{
	ଫେରନ୍ତୁ true;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool task_has_sched_core(ସଂରଚନା task_struct *p)
{
	ଫେରନ୍ତୁ false;
}

#endif /* !CONFIG_SCHED_CORE */

#ifdef CONFIG_RT_GROUP_SCHED
# ifdef CONFIG_RT_GROUP_SCHED_DEFAULT_DISABLED
DECLARE_STATIC_ଚାବି_FALSE(rt_group_sched);
ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool rt_group_sched_enabled(ଶୂନ୍ୟ)
{
	ଫେରନ୍ତୁ static_branch_unlikely(&rt_group_sched);
}
# ଅନ୍ୟଥା /* !CONFIG_RT_GROUP_SCHED_DEFAULT_DISABLED: */
DECLARE_STATIC_ଚାବି_TRUE(rt_group_sched);
ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool rt_group_sched_enabled(ଶୂନ୍ୟ)
{
	ଫେରନ୍ତୁ static_branch_likely(&rt_group_sched);
}
# endif /* !CONFIG_RT_GROUP_SCHED_DEFAULT_DISABLED */
#ଅନ୍ୟଥା /* !CONFIG_RT_GROUP_SCHED: */
# define rt_group_sched_enabled()	false
#endif /* !CONFIG_RT_GROUP_SCHED */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ lockdep_assert_rq_held(ସଂରଚନା rq *rq)
	__assumes_ctx_lock(__rq_lockp(rq))
{
	lockdep_assert_held(__rq_lockp(rq));
}

ବାହ୍ୟ ଶୂନ୍ୟ raw_spin_rq_lock_nested(ସଂରଚନା rq *rq, ପୂର୍ଣ୍ଣସଂଖ୍ୟା subclass)
	__acquires(__rq_lockp(rq));

ବାହ୍ୟ bool raw_spin_rq_trylock(ସଂରଚନା rq *rq)
	__cond_acquires(true, __rq_lockp(rq));

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ raw_spin_rq_lock(ସଂରଚନା rq *rq)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_lock_nested(rq, 0);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ raw_spin_rq_unlock(ସଂରଚନା rq *rq)
	__releases(__rq_lockp(rq))
{
	raw_spin_unlock(rq_lockp(rq));
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ raw_spin_rq_lock_irq(ସଂରଚନା rq *rq)
	__acquires(__rq_lockp(rq))
{
	local_irq_disable();
	raw_spin_rq_lock(rq);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ raw_spin_rq_unlock_irq(ସଂରଚନା rq *rq)
	__releases(__rq_lockp(rq))
{
	raw_spin_rq_unlock(rq);
	local_irq_enable();
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned long _raw_spin_rq_lock_irqsave(ସଂରଚନା rq *rq)
	__acquires(__rq_lockp(rq))
{
	unsigned long flags;

	local_irq_save(flags);
	raw_spin_rq_lock(rq);

	ଫେରନ୍ତୁ flags;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ raw_spin_rq_unlock_irqrestore(ସଂରଚନା rq *rq, unsigned long flags)
	__releases(__rq_lockp(rq))
{
	raw_spin_rq_unlock(rq);
	local_irq_restore(flags);
}

#define raw_spin_rq_lock_irqsave(rq, flags)	\
କର {						\
	flags = _raw_spin_rq_lock_irqsave(rq);	\
} ଯେତେବେଳେ (0)

ବାହ୍ୟ ଶୂନ୍ୟ __update_idle_core(ସଂରଚନା rq *rq);

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ update_idle_core(ସଂରଚନା rq *rq)
{
	ଯଦି (sched_smt_active())
		__update_idle_core(rq);
}

#ifdef CONFIG_FAIR_GROUP_SCHED
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା task_struct *task_of(ସଂରଚନା sched_entity *se)
{
	WARN_ON_ONCE(!entity_is_task(se));
	ଫେରନ୍ତୁ container_of(se, ସଂରଚନା task_struct, se);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା cfs_rq *task_cfs_rq(ସଂରଚନା task_struct *p)
{
	ଫେରନ୍ତୁ p->se.cfs_rq;
}

/* runqueue on which this entity is (to be) queued */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା cfs_rq *cfs_rq_of(ଅଚଳ ସଂରଚନା sched_entity *se)
{
	ଫେରନ୍ତୁ se->cfs_rq;
}

/* runqueue "owned" by this group */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା cfs_rq *group_cfs_rq(ସଂରଚନା sched_entity *grp)
{
	ଫେରନ୍ତୁ grp->my_q;
}

#ଅନ୍ୟଥା /* !CONFIG_FAIR_GROUP_SCHED: */

#define task_of(_se)		container_of(_se, ସଂରଚନା task_struct, se)

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା cfs_rq *task_cfs_rq(ଅଚଳ ସଂରଚନା task_struct *p)
{
	ଫେରନ୍ତୁ &task_rq(p)->cfs;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା cfs_rq *cfs_rq_of(ଅଚଳ ସଂରଚନା sched_entity *se)
{
	ଅଚଳ ସଂରଚନା task_struct *p = task_of(se);
	ସଂରଚନା rq *rq = task_rq(p);

	ଫେରନ୍ତୁ &rq->cfs;
}

/* runqueue "owned" by this group */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା cfs_rq *group_cfs_rq(ସଂରଚନା sched_entity *grp)
{
	ଫେରନ୍ତୁ ଖାଲି;
}

#endif /* !CONFIG_FAIR_GROUP_SCHED */

ବାହ୍ୟ ଶୂନ୍ୟ update_rq_avg_idle(ସଂରଚନା rq *rq);
ବାହ୍ୟ ଶୂନ୍ୟ update_rq_clock(ସଂରଚନା rq *rq);

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

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ assert_clock_updated(ସଂରଚନା rq *rq)
{
	/*
	 * The only reason for not seeing a clock update since the
	 * last rq_pin_lock() is if we're currently skipping updates.
	 */
	WARN_ON_ONCE(rq->clock_update_flags < RQCF_ACT_SKIP);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ u64 rq_clock(ସଂରଚନା rq *rq)
{
	lockdep_assert_rq_held(rq);
	assert_clock_updated(rq);

	ଫେରନ୍ତୁ rq->clock;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ u64 rq_clock_task(ସଂରଚନା rq *rq)
{
	lockdep_assert_rq_held(rq);
	assert_clock_updated(rq);

	ଫେରନ୍ତୁ rq->clock_task;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ rq_clock_skip_update(ସଂରଚନା rq *rq)
{
	lockdep_assert_rq_held(rq);
	rq->clock_update_flags |= RQCF_REQ_SKIP;
}

/*
 * See rt task throttling, which is the only time a skip
 * request is canceled.
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ rq_clock_cancel_skipupdate(ସଂରଚନା rq *rq)
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
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ rq_clock_start_loop_update(ସଂରଚନା rq *rq)
{
	lockdep_assert_rq_held(rq);
	WARN_ON_ONCE(rq->clock_update_flags & RQCF_ACT_SKIP);
	rq->clock_update_flags |= RQCF_ACT_SKIP;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ rq_clock_stop_loop_update(ସଂରଚନା rq *rq)
{
	lockdep_assert_rq_held(rq);
	rq->clock_update_flags &= ~RQCF_ACT_SKIP;
}

ସଂରଚନା rq_flags {
	unsigned long flags;
	ସଂରଚନା pin_cookie cookie;
	/*
	 * A copy of (rq::clock_update_flags & RQCF_UPDATED) for the
	 * current pin context is stashed here in case it needs to be
	 * restored in rq_repin_lock().
	 */
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା clock_update_flags;
};

ବାହ୍ୟ ସଂରଚନା balance_callback balance_ଠେଲ_callback;

#ifdef CONFIG_SCHED_CLASS_EXT
ବାହ୍ୟ ଅଚଳ ସଂରଚନା sched_class ext_sched_class;

DECLARE_STATIC_ଚାବି_FALSE(__scx_enabled);	/* SCX BPF scheduler loaded */
DECLARE_STATIC_ଚାବି_FALSE(__scx_switched_all);	/* all fair class tasks on SCX */

#define scx_enabled()		static_branch_unlikely(&__scx_enabled)
#define scx_switched_all()	static_branch_unlikely(&__scx_switched_all)

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ scx_rq_clock_update(ସଂରଚନା rq *rq, u64 clock)
{
	ଯଦି (!scx_enabled())
		ଫେରନ୍ତୁ;
	WRITE_ONCE(rq->scx.clock, clock);
	smp_store_release(&rq->scx.flags, rq->scx.flags | SCX_RQ_CLK_VALID);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ scx_rq_clock_invalidate(ସଂରଚନା rq *rq)
{
	ଯଦି (!scx_enabled())
		ଫେରନ୍ତୁ;
	WRITE_ONCE(rq->scx.flags, rq->scx.flags & ~SCX_RQ_CLK_VALID);
}

#ଅନ୍ୟଥା /* !CONFIG_SCHED_CLASS_EXT: */
#define scx_enabled()		false
#define scx_switched_all()	false

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ scx_rq_clock_update(ସଂରଚନା rq *rq, u64 clock) {}
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ scx_rq_clock_invalidate(ସଂରଚନା rq *rq) {}
#endif /* !CONFIG_SCHED_CLASS_EXT */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ assert_balance_callbacks_empty(ସଂରଚନା rq *rq)
{
	WARN_ON_ONCE(IS_ENABLED(CONFIG_PROVE_LOCKING) &&
		     rq->balance_callback &&
		     rq->balance_callback != &balance_ଠେଲ_callback);
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
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ rq_pin_lock(ସଂରଚନା rq *rq, ସଂରଚନା rq_flags *rf)
{
	rf->cookie = lockdep_pin_lock(__rq_lockp(rq));

	rq->clock_update_flags &= (RQCF_REQ_SKIP|RQCF_ACT_SKIP);
	rf->clock_update_flags = 0;
	assert_balance_callbacks_empty(rq);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ rq_unpin_lock(ସଂରଚନା rq *rq, ସଂରଚନା rq_flags *rf)
{
	ଯଦି (rq->clock_update_flags > RQCF_ACT_SKIP)
		rf->clock_update_flags = RQCF_UPDATED;

	scx_rq_clock_invalidate(rq);
	lockdep_unpin_lock(__rq_lockp(rq), rf->cookie);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ rq_repin_lock(ସଂରଚନା rq *rq, ସଂରଚନା rq_flags *rf)
{
	lockdep_repin_lock(__rq_lockp(rq), rf->cookie);

	/*
	 * Restore the value we stashed in @rf for this pin context.
	 */
	rq->clock_update_flags |= rf->clock_update_flags;
}

#define __task_rq_lock(...) __acquire_ret(___task_rq_lock(__VA_ARGS__), __rq_lockp(__ret))
ବାହ୍ୟ ସଂରଚନା rq *___task_rq_lock(ସଂରଚନା task_struct *p, ସଂରଚନା rq_flags *rf) __acquires_ret;

#define task_rq_lock(...) __acquire_ret(_task_rq_lock(__VA_ARGS__), __rq_lockp(__ret))
ବାହ୍ୟ ସଂରଚନା rq *_task_rq_lock(ସଂରଚନା task_struct *p, ସଂରଚନା rq_flags *rf)
	__acquires(&p->pi_lock) __acquires_ret;

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
__task_rq_unlock(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p, ସଂରଚନା rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_lock(rq, rf);
	raw_spin_rq_unlock(rq);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
task_rq_unlock(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p, ସଂରଚନା rq_flags *rf)
	__releases(__rq_lockp(rq), &p->pi_lock)
{
	__task_rq_unlock(rq, p, rf);
	raw_spin_unlock_irqrestore(&p->pi_lock, rf->flags);
}

DEFINE_LOCK_GUARD_1(task_rq_lock, ସଂରଚନା task_struct,
		    _T->rq = task_rq_lock(_T->lock, &_T->rf),
		    task_rq_unlock(_T->rq, _T->lock, &_T->rf),
		    ସଂରଚନା rq *rq; ସଂରଚନା rq_flags rf)
DECLARE_LOCK_GUARD_1_ATTRS(task_rq_lock, __acquires(_T->pi_lock), __releases((*(ସଂରଚନା task_struct **)_T)->pi_lock))
#define class_task_rq_lock_constructor(_T) WITH_LOCK_GUARD_1_ATTRS(task_rq_lock, _T)

DEFINE_LOCK_GUARD_1(__task_rq_lock, ସଂରଚନା task_struct,
		    _T->rq = __task_rq_lock(_T->lock, &_T->rf),
		    __task_rq_unlock(_T->rq, _T->lock, &_T->rf),
		    ସଂରଚନା rq *rq; ସଂରଚନା rq_flags rf)

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ rq_lock_irqsave(ସଂରଚନା rq *rq, ସଂରଚନା rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_lock_irqsave(rq, rf->flags);
	rq_pin_lock(rq, rf);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ rq_lock_irq(ସଂରଚନା rq *rq, ସଂରଚନା rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_lock_irq(rq);
	rq_pin_lock(rq, rf);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ rq_lock(ସଂରଚନା rq *rq, ସଂରଚନା rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_lock(rq);
	rq_pin_lock(rq, rf);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ rq_unlock_irqrestore(ସଂରଚନା rq *rq, ସଂରଚନା rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_lock(rq, rf);
	raw_spin_rq_unlock_irqrestore(rq, rf->flags);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ rq_unlock_irq(ସଂରଚନା rq *rq, ସଂରଚନା rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_lock(rq, rf);
	raw_spin_rq_unlock_irq(rq);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ rq_unlock(ସଂରଚନା rq *rq, ସଂରଚନା rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_lock(rq, rf);
	raw_spin_rq_unlock(rq);
}

DEFINE_LOCK_GUARD_1(rq_lock, ସଂରଚନା rq,
		    rq_lock(_T->lock, &_T->rf),
		    rq_unlock(_T->lock, &_T->rf),
		    ସଂରଚନା rq_flags rf)

DECLARE_LOCK_GUARD_1_ATTRS(rq_lock, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(ସଂରଚନା rq **)_T)));
#define class_rq_lock_constructor(_T) WITH_LOCK_GUARD_1_ATTRS(rq_lock, _T)

DEFINE_LOCK_GUARD_1(rq_lock_irq, ସଂରଚନା rq,
		    rq_lock_irq(_T->lock, &_T->rf),
		    rq_unlock_irq(_T->lock, &_T->rf),
		    ସଂରଚନା rq_flags rf)

DECLARE_LOCK_GUARD_1_ATTRS(rq_lock_irq, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(ସଂରଚନା rq **)_T)));
#define class_rq_lock_irq_constructor(_T) WITH_LOCK_GUARD_1_ATTRS(rq_lock_irq, _T)

DEFINE_LOCK_GUARD_1(rq_lock_irqsave, ସଂରଚନା rq,
		    rq_lock_irqsave(_T->lock, &_T->rf),
		    rq_unlock_irqrestore(_T->lock, &_T->rf),
		    ସଂରଚନା rq_flags rf)

DECLARE_LOCK_GUARD_1_ATTRS(rq_lock_irqsave, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(ସଂରଚନା rq **)_T)));
#define class_rq_lock_irqsave_constructor(_T) WITH_LOCK_GUARD_1_ATTRS(rq_lock_irqsave, _T)

DEFINE_LOCK_GUARD_1(raw_spin_rq_lock_irqsave, ସଂରଚନା rq,
		    raw_spin_rq_lock_irqsave(_T->lock, _T->flags),
		    raw_spin_rq_unlock_irqrestore(_T->lock, _T->flags),
		    unsigned long flags)

DECLARE_LOCK_GUARD_1_ATTRS(raw_spin_rq_lock_irqsave, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(ସଂରଚନା rq **)_T)));
#define class_raw_spin_rq_lock_irqsave_constructor(_T) \
	WITH_LOCK_GUARD_1_ATTRS(raw_spin_rq_lock_irqsave, _T)

#define this_rq_lock_irq(...) __acquire_ret(_this_rq_lock_irq(__VA_ARGS__), __rq_lockp(__ret))
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା rq *_this_rq_lock_irq(ସଂରଚନା rq_flags *rf) __acquires_ret
{
	ସଂରଚନା rq *rq;

	local_irq_disable();
	rq = this_rq();
	rq_lock(rq, rf);

	ଫେରନ୍ତୁ rq;
}

#ifdef CONFIG_NUMA

ଗଣନା numa_topology_type {
	NUMA_DIRECT,
	NUMA_GLUELESS_MESH,
	NUMA_BACKPLANE,
};

ବାହ୍ୟ ଗଣନା numa_topology_type sched_numa_topology_type;
ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା sched_max_numa_distance;
ବାହ୍ୟ bool find_numa_distance(ପୂର୍ଣ୍ଣସଂଖ୍ୟା distance);
ବାହ୍ୟ ଶୂନ୍ୟ sched_init_numa(ପୂର୍ଣ୍ଣସଂଖ୍ୟା offline_ଗଣ୍ଠି);
ବାହ୍ୟ ଶୂନ୍ୟ sched_update_numa(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu, bool online);
ବାହ୍ୟ ଶୂନ୍ୟ sched_domains_numa_masks_set(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu);
ବାହ୍ୟ ଶୂନ୍ୟ sched_domains_numa_masks_clear(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu);
ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା sched_numa_find_closest(ଅଚଳ ସଂରଚନା cpumask *cpus, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu);

#ଅନ୍ୟଥା /* !CONFIG_NUMA: */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ sched_init_numa(ପୂର୍ଣ୍ଣସଂଖ୍ୟା offline_ଗଣ୍ଠି) { }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ sched_update_numa(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu, bool online) { }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ sched_domains_numa_masks_set(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu) { }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ sched_domains_numa_masks_clear(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu) { }

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା sched_numa_find_closest(ଅଚଳ ସଂରଚନା cpumask *cpus, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ଫେରନ୍ତୁ nr_cpu_ids;
}

#endif /* !CONFIG_NUMA */

#ifdef CONFIG_NUMA_BALANCING

/* The regions in numa_faults array from task_struct */
ଗଣନା numa_faults_stats {
	NUMA_MEM = 0,
	NUMA_CPU,
	NUMA_MEMBUF,
	NUMA_CPUBUF
};

ବାହ୍ୟ ଶୂନ୍ୟ sched_setnuma(ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣ୍ଠି);
ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା migrate_task_to(ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu);
ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା migrate_swap(ସଂରଚନା task_struct *p, ସଂରଚନା task_struct *t,
			ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu, ପୂର୍ଣ୍ଣସଂଖ୍ୟା scpu);
ବାହ୍ୟ ଶୂନ୍ୟ init_numa_balancing(u64 clone_flags, ସଂରଚନା task_struct *p);

#ଅନ୍ୟଥା /* !CONFIG_NUMA_BALANCING: */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
init_numa_balancing(u64 clone_flags, ସଂରଚନା task_struct *p)
{
}

#endif /* !CONFIG_NUMA_BALANCING */

ପୂର୍ଣ୍ଣସଂଖ୍ୟା task_llc(ଅଚଳ ସଂରଚନା task_struct *p);

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
queue_balance_callback(ସଂରଚନା rq *rq,
		       ସଂରଚନା balance_callback *head,
		       ଶୂନ୍ୟ (*func)(ସଂରଚନା rq *rq))
{
	lockdep_assert_rq_held(rq);

	/*
	 * Don't (re)queue an already queued item; nor queue anything when
	 * balance_push() is active, see the comment with
	 * balance_push_callback.
	 */
	ଯଦି (unlikely(head->next || rq->balance_callback == &balance_ଠେଲ_callback))
		ଫେରନ୍ତୁ;

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
	ପାଇଁ (__sd = rcu_dereference_sched_domain(cpu_rq(cpu)->sd); \
			__sd; __sd = __sd->parent)

/* A mask of all the SD flags that have the SDF_SHARED_CHILD metaflag */
#define SD_FLAG(name, mflags) (name * !!((mflags) & SDF_SHARED_CHILD)) |
ସ୍ଥିର ଅଚଳ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା SD_SHARED_CHILD_MASK =
#ଅନ୍ତର୍ଭାଵ <linux/sched/sd_flags.h>
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
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା sched_domain *highest_flag_domain(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flag)
{
	ସଂରଚନା sched_domain *sd, *hsd = ଖାଲି;

	for_each_domain(cpu, sd) {
		ଯଦି (sd->flags & flag) {
			hsd = sd;
			ଜାରି;
		}

		/*
		 * Stop the search if @flag is known to be shared at lower
		 * levels. It will not be found further up.
		 */
		ଯଦି (flag & SD_SHARED_CHILD_MASK)
			ଭାଙ୍ଗ;
	}

	ଫେରନ୍ତୁ hsd;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା sched_domain *lowest_flag_domain(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flag)
{
	ସଂରଚନା sched_domain *sd;

	for_each_domain(cpu, sd) {
		ଯଦି (sd->flags & flag)
			ଭାଙ୍ଗ;
	}

	ଫେରନ୍ତୁ sd;
}

DECLARE_PER_CPU(ସଂରଚନା sched_domain __rcu *, sd_llc);
DECLARE_PER_CPU(ପୂର୍ଣ୍ଣସଂଖ୍ୟା, sd_llc_size);
DECLARE_PER_CPU(ପୂର୍ଣ୍ଣସଂଖ୍ୟା, sd_llc_id);
DECLARE_PER_CPU(ପୂର୍ଣ୍ଣସଂଖ୍ୟା, sd_share_id);
DECLARE_PER_CPU(ସଂରଚନା sched_domain_shared __rcu *, sd_llc_shared);
DECLARE_PER_CPU(ସଂରଚନା sched_domain_shared __rcu *, sd_balance_shared);
DECLARE_PER_CPU(ସଂରଚନା sched_domain __rcu *, sd_numa);
DECLARE_PER_CPU(ସଂରଚନା sched_domain __rcu *, sd_asym_packing);
DECLARE_PER_CPU(ସଂରଚନା sched_domain __rcu *, sd_asym_cpucapacity);

ବାହ୍ୟ ସଂରଚନା static_ଚାବି_false sched_asym_cpucapacity;
ବାହ୍ୟ ସଂରଚନା static_ଚାବି_false sched_cluster_active;

ସ୍ଥିର __always_inline bool sched_asym_cpucap_active(ଶୂନ୍ୟ)
{
	ଫେରନ୍ତୁ static_branch_unlikely(&sched_asym_cpucapacity);
}

ସଂରଚନା sched_group_capacity {
	atomic_t		ref;
	/*
	 * CPU capacity of this group, SCHED_CAPACITY_SCALE being max capacity
	 * for a single CPU.
	 */
	unsigned long		capacity;
	unsigned long		min_capacity;		/* Min per-CPU capacity in group */
	unsigned long		max_capacity;		/* Max per-CPU capacity in group */
	unsigned long		next_update;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			imbalance;		/* XXX unrelated to capacity but shared group state */

	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			id;

	unsigned long		cpumask[];		/* Balance mask */
};

ସଂରଚନା sched_group {
	ସଂରଚନା sched_group	*next;			/* Must be a circular list */
	atomic_t		ref;

	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		group_weight;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		cores;
	ସଂରଚନା sched_group_capacity *sgc;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			asym_prefer_cpu;	/* CPU of highest priority in group */
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			flags;

	/*
	 * The CPUs this group covers.
	 *
	 * NOTE: this field is variable length. (Allocated dynamically
	 * by attaching extra space to the end of the structure,
	 * depending on how many CPUs the kernel has booted up with)
	 */
	unsigned long		cpumask[];
};

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା cpumask *sched_group_span(ସଂରଚନା sched_group *sg)
{
	ଫେରନ୍ତୁ to_cpumask(sg->cpumask);
}

/*
 * See build_balance_mask().
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା cpumask *group_balance_mask(ସଂରଚନା sched_group *sg)
{
	ଫେରନ୍ତୁ to_cpumask(sg->sgc->cpumask);
}

ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା group_balance_cpu(ସଂରଚନା sched_group *sg);

ବାହ୍ୟ ଶୂନ୍ୟ update_sched_domain_debugfs(ଶୂନ୍ୟ);
ବାହ୍ୟ ଶୂନ୍ୟ dirty_sched_domain_sysctl(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu);

ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା sched_update_scaling(ଶୂନ୍ୟ);

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଅଚଳ ସଂରଚନା cpumask *task_user_cpus(ସଂରଚନା task_struct *p)
{
	ଯଦି (!p->user_cpus_ptr)
		ଫେରନ୍ତୁ cpu_possible_mask; /* &init_task.cpus_mask */
	ଫେରନ୍ତୁ p->user_cpus_ptr;
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
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା task_group *task_group(ସଂରଚନା task_struct *p)
{
	ଫେରନ୍ତୁ p->sched_task_group;
}

#ifdef CONFIG_FAIR_GROUP_SCHED
/*
 * Defined here to be available before stats.h is included, since
 * stats.h has dependencies on things defined later in this file.
 */
ସଂରଚନା cfs_tg_state {
	ସଂରଚନା cfs_rq		cfs_rq;
	ସଂରଚନା sched_entity	se;
	ସଂରଚନା sched_statistics	stats;
} __no_randomize_layout;

/* Access a specific CPU's cfs_rq from a task group */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା cfs_rq *tg_cfs_rq(ସଂରଚନା task_group *tg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ଫେରନ୍ତୁ per_cpu_ptr(tg->cfs_rq, cpu);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା sched_entity *tg_se(ସଂରଚନା task_group *tg, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ସଂରଚନା cfs_tg_state *state;

	ଯଦି (is_ମୂଳ_task_group(tg))
		ଫେରନ୍ତୁ ଖାଲି;

	state = container_of(tg_cfs_rq(tg, cpu), ସଂରଚନା cfs_tg_state, cfs_rq);
	ଫେରନ୍ତୁ &state->se;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା sched_entity *cfs_rq_se(ସଂରଚନା cfs_rq *cfs_rq)
{
	ସଂରଚନା cfs_tg_state *state;

	ଯଦି (is_ମୂଳ_task_group(cfs_rq->tg))
		ଫେରନ୍ତୁ ଖାଲି;

	state = container_of(cfs_rq, ସଂରଚନା cfs_tg_state, cfs_rq);
	ଫେରନ୍ତୁ &state->se;
}
#endif

/* Change a task's cfs_rq and parent entity if it moves across CPUs/groups */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ set_task_rq(ସଂରଚନା task_struct *p, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
#ଯଦି defined(CONFIG_FAIR_GROUP_SCHED) || defined(CONFIG_RT_GROUP_SCHED)
	ସଂରଚନା task_group *tg = task_group(p);
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
	ଯଦି (!rt_group_sched_enabled())
		tg = &ମୂଳ_task_group;
	p->rt.rt_rq  = tg->rt_rq[cpu];
	p->rt.parent = tg->rt_se[cpu];
#endif /* CONFIG_RT_GROUP_SCHED */
}

#ଅନ୍ୟଥା /* !CONFIG_CGROUP_SCHED: */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ set_task_rq(ସଂରଚନା task_struct *p, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu) { }

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା task_group *task_group(ସଂରଚନା task_struct *p)
{
	ଫେରନ୍ତୁ ଖାଲି;
}

#endif /* !CONFIG_CGROUP_SCHED */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ __set_task_cpu(ସଂରଚନା task_struct *p, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
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

ଗଣନା {
#ଅନ୍ତର୍ଭାଵ "features.h"
	__SCHED_FEAT_NR,
};

#undef SCHED_FEAT

/*
 * To support run-time toggling of sched features, all the translation units
 * (but core.c) reference the sysctl_sched_features defined in core.c.
 */
ବାହ୍ୟ __read_mostly unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା sysctl_sched_features;

#ifdef CONFIG_JUMP_LABEL

ସଂଘ sched_feat_ଚାବି {
	ସଂରଚନା static_ଚାବି_true	ଚାବି_true;
	ସଂରଚନା static_ଚାବି_false	ଚାବି_false;
};

#define sched_feat_branch_true(ଚାବି)	static_branch_likely(&(ଚାବି)->ଚାବି_true)
#define sched_feat_branch_false(ଚାବି)	static_branch_unlikely(&(ଚାବି)->ଚାବି_false)

#define SCHED_FEAT(name, enabled)			\
ସ୍ଥିର __always_inline bool				\
static_branch_##name(ସଂଘ sched_feat_ଚାବି *ଚାବି) 	\
{							\
	ଫେରନ୍ତୁ sched_feat_branch_##enabled(ଚାବି);	\
}

#ଅନ୍ତର୍ଭାଵ "features.h"
#undef SCHED_FEAT

ବାହ୍ୟ ସଂଘ sched_feat_ଚାବି sched_feat_ଚାବିଗୁଡ଼ିକ[__SCHED_FEAT_NR];
#define sched_feat(x) (static_branch_##x(&sched_feat_ଚାବିଗୁଡ଼ିକ[__SCHED_FEAT_##x]))

#ଅନ୍ୟଥା /* !CONFIG_JUMP_LABEL: */

#define sched_feat(x) (sysctl_sched_features & (1UL << __SCHED_FEAT_##x))

#endif /* !CONFIG_JUMP_LABEL */

ବାହ୍ୟ ସଂରଚନା static_ଚାବି_false sched_numa_balancing;
ବାହ୍ୟ ସଂରଚନା static_ଚାବି_false sched_schedstats;

ସ୍ଥିର ଧାଡ଼ିଭିତରେ u64 global_rt_period(ଶୂନ୍ୟ)
{
	ଫେରନ୍ତୁ (u64)sysctl_sched_rt_period * NSEC_PER_USEC;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ u64 global_rt_runtime(ଶୂନ୍ୟ)
{
	ଯଦି (sysctl_sched_rt_runtime < 0)
		ଫେରନ୍ତୁ RUNTIME_INF;

	ଫେରନ୍ତୁ (u64)sysctl_sched_rt_runtime * NSEC_PER_USEC;
}

/*
 * Is p the current execution context?
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା task_current(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p)
{
	ଫେରନ୍ତୁ rq->curr == p;
}

/*
 * Is p the current scheduling context?
 *
 * Note that it might be the current execution context at the same time if
 * rq->curr == rq->donor == p.
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା task_current_donor(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p)
{
	ଫେରନ୍ତୁ rq->donor == p;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool task_is_blocked(ସଂରଚନା task_struct *p)
{
	ଯଦି (!sched_proxy_exec())
		ଫେରନ୍ତୁ false;

	ଫେରନ୍ତୁ !!p->blocked_on;
}

#ifdef CONFIG_SCHED_PROXY_EXEC
ଶୂନ୍ୟ sched_proxy_block_task(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p);
#ଅନ୍ୟଥା
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ sched_proxy_block_task(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p) {}
#endif

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା task_on_cpu(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p)
{
	ଫେରନ୍ତୁ p->on_cpu;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା task_on_rq_queued(ସଂରଚନା task_struct *p)
{
	ଫେରନ୍ତୁ READ_ONCE(p->on_rq) == TASK_ON_RQ_QUEUED;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା task_on_rq_migrating(ସଂରଚନା task_struct *p)
{
	ଫେରନ୍ତୁ READ_ONCE(p->on_rq) == TASK_ON_RQ_MIGRATING;
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

ବାହ୍ୟ ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା		sched_prio_to_weight[40];
ବାହ୍ୟ ଅଚଳ u32		sched_prio_to_wmult[40];

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

#define RETRY_TASK		((ଶୂନ୍ୟ *)-1UL)

ସଂରଚନା affinity_context {
	ଅଚଳ ସଂରଚନା cpumask	*new_mask;
	ସଂରଚନା cpumask		*user_mask;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା		flags;
};

ବାହ୍ୟ s64 update_curr_common(ସଂରଚନା rq *rq);

ଗଣନା snt_e {
	SNT_NORMAL,	/* set_next_task() */
	SNT_PICK,	/* put_prev_set_next_task(): prev != next */
	SNT_REPICK,	/* put_prev_set_next_task(): prev == next */
};

ସଂରଚନା sched_class {

#ifdef CONFIG_UCLAMP_TASK
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା uclamp_enabled;
#endif

	/*
	 * move_queued_task/activate_task/enqueue_task: rq->lock
	 * ttwu_do_activate/activate_task/enqueue_task: rq->lock
	 * wake_up_new_task/activate_task/enqueue_task: task_rq_lock
	 * ttwu_runnable/enqueue_task: task_rq_lock
	 * proxy_task_current: rq->lock
	 * sched_change_end
	 */
	ଶୂନ୍ୟ (*enqueue_task) (ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags);
	/*
	 * move_queued_task/deactivate_task/dequeue_task: rq->lock
	 * __schedule/block_task/dequeue_task: rq->lock
	 * proxy_task_current: rq->lock
	 * wait_task_inactive: task_rq_lock
	 * sched_change_begin
	 */
	bool (*dequeue_task) (ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags);

	/*
	 * do_sched_yield: rq->lock
	 */
	ଶୂନ୍ୟ (*yield_task)   (ସଂରଚନା rq *rq);
	/*
	 * yield_to: rq->lock (double)
	 */
	bool (*yield_to_task)(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p);

	/*
	 * move_queued_task: rq->lock
	 * __migrate_swap_task: rq->lock
	 * ttwu_do_activate: rq->lock
	 * ttwu_runnable: task_rq_lock
	 * wake_up_new_task: task_rq_lock
	 */
	ଶୂନ୍ୟ (*wakeup_preempt)(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags);

	/*
	 * schedule/pick_next_task/prev_balance: rq->lock
	 */
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*balance)(ସଂରଚନା rq *rq, ସଂରଚନା rq_flags *rf);

	/*
	 * schedule/pick_next_task: rq->lock
	 */
	ସଂରଚନା task_struct *(*pick_task)(ସଂରଚନା rq *rq, ସଂରଚନା rq_flags *rf);

	/*
	 * sched_change:
	 * __schedule: rq->lock
	 */
	ଶୂନ୍ୟ (*put_prev_task)(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p, ସଂରଚନା task_struct *next);
	ଶୂନ୍ୟ (*set_next_task)(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p, ଗଣନା snt_e type);

	/*
	 * select_task_rq: p->pi_lock
	 * sched_exec: p->pi_lock
	 */
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା  (*select_task_rq)(ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା task_cpu, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags);

	/*
	 * set_task_cpu: p->pi_lock || rq->lock (ttwu like)
	 */
	ଶୂନ୍ୟ (*migrate_task_rq)(ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_cpu);

	/*
	 * ttwu_do_activate: rq->lock
	 * wake_up_new_task: task_rq_lock
	 */
	ଶୂନ୍ୟ (*task_woken)(ସଂରଚନା rq *this_rq, ସଂରଚନା task_struct *task);

	/*
	 * do_set_cpus_allowed: task_rq_lock + sched_change
	 */
	ଶୂନ୍ୟ (*set_cpus_allowed)(ସଂରଚନା task_struct *p, ସଂରଚନା affinity_context *ctx);

	/*
	 * sched_set_rq_{on,off}line: rq->lock
	 */
	ଶୂନ୍ୟ (*rq_online)(ସଂରଚନା rq *rq);
	ଶୂନ୍ୟ (*rq_offline)(ସଂରଚନା rq *rq);

	/*
	 * push_cpu_stop: p->pi_lock && rq->lock
	 */
	ସଂରଚନା rq *(*find_lock_rq)(ସଂରଚନା task_struct *p, ସଂରଚନା rq *rq);

	/*
	 * hrtick: rq->lock
	 * sched_tick: rq->lock
	 * sched_tick_remote: rq->lock
	 */
	ଶୂନ୍ୟ (*task_tick)(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା queued);
	/*
	 * sched_cgroup_fork: p->pi_lock
	 */
	ଶୂନ୍ୟ (*task_fork)(ସଂରଚନା task_struct *p);
	/*
	 * finish_task_switch: no locks
	 */
	ଶୂନ୍ୟ (*task_dead)(ସଂରଚନା task_struct *p);

	/*
	 * sched_change
	 */
	ଶୂନ୍ୟ (*switching_from)(ସଂରଚନା rq *this_rq, ସଂରଚନା task_struct *task);
	ଶୂନ୍ୟ (*switched_from) (ସଂରଚନା rq *this_rq, ସଂରଚନା task_struct *task);
	ଶୂନ୍ୟ (*switching_to)  (ସଂରଚନା rq *this_rq, ସଂରଚନା task_struct *task);
	ଶୂନ୍ୟ (*switched_to)   (ସଂରଚନା rq *this_rq, ସଂରଚନା task_struct *task);
	u64  (*get_prio)     (ସଂରଚନା rq *this_rq, ସଂରଚନା task_struct *task);
	ଶୂନ୍ୟ (*prio_changed) (ସଂରଚନା rq *this_rq, ସଂରଚନା task_struct *task,
			      u64 oldprio);

	/*
	 * set_load_weight: task_rq_lock + sched_change
	 * __setscheduler_parms: task_rq_lock + sched_change
	 */
	ଶୂନ୍ୟ (*reweight_task)(ସଂରଚନା rq *this_rq, ସଂରଚନା task_struct *task,
			      ଅଚଳ ସଂରଚନା load_weight *lw);

	/*
	 * sched_rr_get_interval: task_rq_lock
	 */
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*get_rr_interval)(ସଂରଚନା rq *rq,
					ସଂରଚନା task_struct *task);

	/*
	 * task_sched_runtime: task_rq_lock
	 */
	ଶୂନ୍ୟ (*update_curr)(ସଂରଚନା rq *rq);

#ifdef CONFIG_FAIR_GROUP_SCHED
	/*
	 * sched_change_group: task_rq_lock + sched_change
	 */
	ଶୂନ୍ୟ (*task_change_group)(ସଂରଚନା task_struct *p);
#endif

#ifdef CONFIG_SCHED_CORE
	/*
	 * pick_next_task: rq->lock
	 * try_steal_cookie: rq->lock (double)
	 */
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*task_is_throttled)(ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu);
#endif
};

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ put_prev_task(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *prev)
{
	WARN_ON_ONCE(rq->donor != prev);
	prev->sched_class->put_prev_task(rq, prev, ଖାଲି);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ set_next_task(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *next)
{
	next->sched_class->set_next_task(rq, next, SNT_NORMAL);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
__put_prev_set_next_dl_server(ସଂରଚନା rq *rq,
			      ସଂରଚନା task_struct *prev,
			      ସଂରଚନା task_struct *next)
{
	prev->dl_server = ଖାଲି;
	next->dl_server = rq->dl_server;
	rq->dl_server = ଖାଲି;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ put_prev_set_next_task(ସଂରଚନା rq *rq,
					  ସଂରଚନା task_struct *prev,
					  ସଂରଚନା task_struct *next)
{
	WARN_ON_ONCE(rq->donor != prev);

	__put_prev_set_next_dl_server(rq, prev, next);

	ଯଦି (next == prev) {
		next->sched_class->set_next_task(rq, next, SNT_REPICK);
		ଫେରନ୍ତୁ;
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
ଅଚଳ ସଂରଚନା sched_class name##_sched_class \
	__aligned(__alignof__(ସଂରଚନା sched_class)) \
	__section("__" #name "_sched_class")

/* Defined in include/asm-generic/vmlinux.lds.h */
ବାହ୍ୟ ସଂରଚନା sched_class __sched_class_highest[];
ବାହ୍ୟ ସଂରଚନା sched_class __sched_class_lowest[];

ବାହ୍ୟ ଅଚଳ ସଂରଚନା sched_class stop_sched_class;
ବାହ୍ୟ ଅଚଳ ସଂରଚନା sched_class dl_sched_class;
ବାହ୍ୟ ଅଚଳ ସଂରଚନା sched_class rt_sched_class;
ବାହ୍ୟ ଅଚଳ ସଂରଚନା sched_class fair_sched_class;
ବାହ୍ୟ ଅଚଳ ସଂରଚନା sched_class idle_sched_class;

/*
 * Iterate only active classes. SCX can take over all fair tasks or be
 * completely disabled. If the former, skip fair. If the latter, skip SCX.
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଅଚଳ ସଂରଚନା sched_class *next_active_class(ଅଚଳ ସଂରଚନା sched_class *class)
{
	class++;
#ifdef CONFIG_SCHED_CLASS_EXT
	ଯଦି (scx_switched_all() && class == &fair_sched_class)
		class++;
	ଯଦି (!scx_enabled() && class == &ext_sched_class)
		class++;
#endif
	ଫେରନ୍ତୁ class;
}

#define for_class_range(class, _from, _to) \
	ପାଇଁ (class = (_from); class < (_to); class++)

#define for_each_class(class) \
	for_class_range(class, __sched_class_highest, __sched_class_lowest)

#define for_active_class_range(class, _from, _to)				\
	ପାଇଁ (class = (_from); class != (_to); class = next_active_class(class))

#define for_each_active_class(class)						\
	for_active_class_range(class, __sched_class_highest, __sched_class_lowest)

#define sched_class_above(_a, _b)	((_a) < (_b))

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ rq_modified_begin(ସଂରଚନା rq *rq, ଅଚଳ ସଂରଚନା sched_class *class)
{
	ଯଦି (sched_class_above(rq->next_class, class))
		rq->next_class = class;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool rq_modified_above(ସଂରଚନା rq *rq, ଅଚଳ ସଂରଚନା sched_class *class)
{
	ଫେରନ୍ତୁ sched_class_above(rq->next_class, class);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool sched_stop_runnable(ସଂରଚନା rq *rq)
{
	ଫେରନ୍ତୁ rq->stop && task_on_rq_queued(rq->stop);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool sched_dl_runnable(ସଂରଚନା rq *rq)
{
	ଫେରନ୍ତୁ rq->dl.dl_nr_running > 0;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool sched_rt_runnable(ସଂରଚନା rq *rq)
{
	ଫେରନ୍ତୁ rq->rt.rt_queued > 0;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool sched_fair_runnable(ସଂରଚନା rq *rq)
{
	ଫେରନ୍ତୁ rq->cfs.nr_queued > 0;
}

ବାହ୍ୟ ସଂରଚନା task_struct *pick_task_fair(ସଂରଚନା rq *rq, ସଂରଚନା rq_flags *rf);
ବାହ୍ୟ ସଂରଚନା task_struct *pick_task_idle(ସଂରଚନା rq *rq, ସଂରଚନା rq_flags *rf);

#define SCA_CHECK		0x01
#define SCA_MIGRATE_DISABLE	0x02
#define SCA_MIGRATE_ENABLE	0x04
#define SCA_USER		0x08

ବାହ୍ୟ ଶୂନ୍ୟ update_group_capacity(ସଂରଚନା sched_domain *sd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu);

ବାହ୍ୟ ଶୂନ୍ୟ sched_balance_trigger(ସଂରଚନା rq *rq);

ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା __set_cpus_allowed_ptr(ସଂରଚନା task_struct *p, ସଂରଚନା affinity_context *ctx);
ବାହ୍ୟ ଶୂନ୍ୟ set_cpus_allowed_common(ସଂରଚନା task_struct *p, ସଂରଚନା affinity_context *ctx);

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool task_allowed_on_cpu(ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	/* When not in the task's cpumask, no point in looking further. */
	ଯଦି (!cpumask_test_cpu(cpu, p->cpus_ptr))
		ଫେରନ୍ତୁ false;

	/* Can @cpu run a user thread? */
	ଯଦି (!(p->flags & PF_KTHREAD) && !task_cpu_possible(cpu, p))
		ଫେରନ୍ତୁ false;

	ଫେରନ୍ତୁ true;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ cpumask_t *alloc_user_cpus_ptr(ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣ୍ଠି)
{
	/*
	 * See set_cpus_allowed_force() above for the rcu_head usage.
	 */
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା size = max_t(ପୂର୍ଣ୍ଣସଂଖ୍ୟା, cpumask_size(), ଆକାର(ସଂରଚନା rcu_head));

	ଫେରନ୍ତୁ kmalloc_ଗଣ୍ଠି(size, GFP_KERNEL, ଗଣ୍ଠି);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା task_struct *get_ଠେଲ_task(ସଂରଚନା rq *rq)
{
	ସଂରଚନା task_struct *p = rq->donor;

	lockdep_assert_rq_held(rq);

	ଯଦି (rq->ଠେଲ_busy)
		ଫେରନ୍ତୁ ଖାଲି;

	ଯଦି (p->nr_cpus_allowed == 1)
		ଫେରନ୍ତୁ ଖାଲି;

	ଯଦି (p->migration_disabled)
		ଫେରନ୍ତୁ ଖାଲି;

	rq->ଠେଲ_busy = true;
	ଫେରନ୍ତୁ get_task_struct(p);
}

ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଠେଲ_cpu_stop(ଶୂନ୍ୟ *arg);

#ifdef CONFIG_CPU_IDLE

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ idle_set_state(ସଂରଚନା rq *rq,
				  ସଂରଚନା cpuidle_state *idle_state)
{
	rq->idle_state = idle_state;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା cpuidle_state *idle_get_state(ସଂରଚନା rq *rq)
{
	lockdep_assert(rcu_read_lock_any_held());

	ଫେରନ୍ତୁ rq->idle_state;
}

#ଅନ୍ୟଥା /* !CONFIG_CPU_IDLE: */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ idle_set_state(ସଂରଚନା rq *rq,
				  ସଂରଚନା cpuidle_state *idle_state)
{
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ସଂରଚନା cpuidle_state *idle_get_state(ସଂରଚନା rq *rq)
{
	ଫେରନ୍ତୁ ଖାଲି;
}

#endif /* !CONFIG_CPU_IDLE */

ବାହ୍ୟ ଶୂନ୍ୟ schedule_idle(ଶୂନ୍ୟ);
asmlinkage ଶୂନ୍ୟ schedule_user(ଶୂନ୍ୟ);

ବାହ୍ୟ ଶୂନ୍ୟ sysrq_sched_debug_show(ଶୂନ୍ୟ);
ବାହ୍ୟ ଶୂନ୍ୟ sched_init_granularity(ଶୂନ୍ୟ);
ବାହ୍ୟ ଶୂନ୍ୟ update_max_interval(ଶୂନ୍ୟ);

ବାହ୍ୟ ଶୂନ୍ୟ init_sched_dl_class(ଶୂନ୍ୟ);
ବାହ୍ୟ ଶୂନ୍ୟ init_sched_rt_class(ଶୂନ୍ୟ);
ବାହ୍ୟ ଶୂନ୍ୟ init_sched_fair_class(ଶୂନ୍ୟ);

ବାହ୍ୟ ଶୂନ୍ୟ resched_curr(ସଂରଚନା rq *rq);
ବାହ୍ୟ ଶୂନ୍ୟ resched_curr_lazy(ସଂରଚନା rq *rq);
ବାହ୍ୟ ଶୂନ୍ୟ resched_cpu(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu);

ବାହ୍ୟ ଶୂନ୍ୟ init_rt_bandwidth(ସଂରଚନା rt_bandwidth *rt_b, u64 period, u64 runtime);
ବାହ୍ୟ bool sched_rt_bandwidth_account(ସଂରଚନା rt_rq *rt_rq);

ବାହ୍ୟ ଶୂନ୍ୟ init_dl_entity(ସଂରଚନା sched_dl_entity *dl_se);

ବାହ୍ୟ ଶୂନ୍ୟ init_cfs_throttle_work(ସଂରଚନା task_struct *p);

#define BW_SHIFT		20
#define BW_UNIT			(1 << BW_SHIFT)
#define RATIO_SHIFT		8
#define MAX_BW_BITS		(64 - BW_SHIFT)
#define MAX_BW			((1ULL << MAX_BW_BITS) - 1)

ବାହ୍ୟ u64 to_ratio(u64 period, u64 runtime);

ବାହ୍ୟ ଶୂନ୍ୟ init_entity_runnable_average(ସଂରଚନା sched_entity *se);
ବାହ୍ୟ ଶୂନ୍ୟ post_init_entity_util_avg(ସଂରଚନା task_struct *p);

#ifdef CONFIG_NO_HZ_FULL
ବାହ୍ୟ bool sched_can_stop_tick(ସଂରଚନା rq *rq);
ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା __init sched_tick_offload_init(ଶୂନ୍ୟ);

/*
 * Tick may be needed by tasks in the runqueue depending on their policy and
 * requirements. If tick is needed, lets send the target an IPI to kick it out of
 * nohz mode if necessary.
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ sched_update_tick_dependency(ସଂରଚନା rq *rq)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu = cpu_of(rq);

	ଯଦି (!tick_nohz_full_cpu(cpu))
		ଫେରନ୍ତୁ;

	ଯଦି (sched_can_stop_tick(rq))
		tick_nohz_dep_clear_cpu(cpu, TICK_DEP_BIT_SCHED);
	ଅନ୍ୟଥା
		tick_nohz_dep_set_cpu(cpu, TICK_DEP_BIT_SCHED);
}
#ଅନ୍ୟଥା /* !CONFIG_NO_HZ_FULL: */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା sched_tick_offload_init(ଶୂନ୍ୟ) { ଫେରନ୍ତୁ 0; }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ sched_update_tick_dependency(ସଂରଚନା rq *rq) { }
#endif /* !CONFIG_NO_HZ_FULL */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ add_nr_running(ସଂରଚନା rq *rq, unsigned ଗଣନା_2)
{
	unsigned prev_nr = rq->nr_running;

	rq->nr_running = prev_nr + ଗଣନା_2;
	ଯଦି (trace_sched_update_nr_running_tp_enabled()) {
		call_trace_sched_update_nr_running(rq, ଗଣନା_2);
	}

	ଯଦି (prev_nr < 2 && rq->nr_running >= 2)
		set_rd_overloaded(rq->rd, 1);

	sched_update_tick_dependency(rq);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ sub_nr_running(ସଂରଚନା rq *rq, unsigned ଗଣନା_2)
{
	rq->nr_running -= ଗଣନା_2;
	ଯଦି (trace_sched_update_nr_running_tp_enabled()) {
		call_trace_sched_update_nr_running(rq, -ଗଣନା_2);
	}

	/* Check if we still need preemption */
	sched_update_tick_dependency(rq);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ __block_task(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p)
{
	ଯଦି (p->sched_contributes_to_load)
		rq->nr_uninterruptible++;

	ଯଦି (p->in_iowait) {
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

ବାହ୍ୟ ଶୂନ୍ୟ activate_task(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags);
ବାହ୍ୟ ଶୂନ୍ୟ deactivate_task(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags);

ବାହ୍ୟ ଶୂନ୍ୟ wakeup_preempt(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags);

/*
 * attach_task() -- attach the task detached by detach_task() to its new rq.
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ attach_task(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p)
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
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ attach_one_task(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p)
{
	guard(rq_lock)(rq);
	update_rq_clock(rq);
	attach_task(rq, p);
}

/*
 * __attach_tasks() - attaches a list of tasks (using se.group_node) to
 * the new rq
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ __attach_tasks(ସଂରଚନା rq *rq, ସଂରଚନା list_head *tasks)
{
	guard(rq_lock)(rq);
	update_rq_clock(rq);

	ଯେତେବେଳେ (!list_empty(tasks)) {
		ସଂରଚନା task_struct *p;

		p = list_first_entry(tasks, ସଂରଚନା task_struct, se.group_ଗଣ୍ଠି);
		list_del_init(&p->se.group_ଗଣ୍ଠି);

		attach_task(rq, p);
	}
}

#ifdef CONFIG_PREEMPT_RT
# define SCHED_NR_MIGRATE_BREAK 8
#ଅନ୍ୟଥା
# define SCHED_NR_MIGRATE_BREAK 32
#endif

ବାହ୍ୟ __read_mostly unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା sysctl_sched_nr_migrate;
ବାହ୍ୟ __read_mostly unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା sysctl_sched_migration_cost;

ବାହ୍ୟ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା sysctl_sched_base_slice;

ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା sysctl_resched_latency_warn_ms;
ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା sysctl_resched_latency_warn_once;

ବାହ୍ୟ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା sysctl_sched_tunable_scaling;

ବାହ୍ୟ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା sysctl_numa_balancing_scan_delay;
ବାହ୍ୟ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା sysctl_numa_balancing_scan_period_min;
ବାହ୍ୟ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା sysctl_numa_balancing_scan_period_max;
ବାହ୍ୟ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା sysctl_numa_balancing_scan_size;
ବାହ୍ୟ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା sysctl_numa_balancing_hot_threshold;

#ifdef CONFIG_SCHED_HRTICK

/*
 * Use hrtick when:
 *  - enabled by features
 *  - hrtimer is actually high res
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool hrtick_enabled(ସଂରଚନା rq *rq)
{
	ଫେରନ୍ତୁ cpu_active(cpu_of(rq)) && hrtimer_highres_enabled();
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool hrtick_enabled_fair(ସଂରଚନା rq *rq)
{
	ଫେରନ୍ତୁ sched_feat(HRTICK) && hrtick_enabled(rq);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool hrtick_enabled_dl(ସଂରଚନା rq *rq)
{
	ଫେରନ୍ତୁ sched_feat(HRTICK_DL) && hrtick_enabled(rq);
}

ବାହ୍ୟ ଶୂନ୍ୟ hrtick_start(ସଂରଚନା rq *rq, u64 delay);
ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool hrtick_active(ସଂରଚନା rq *rq)
{
	ଫେରନ୍ତୁ hrtimer_active(&rq->hrtick_timer);
}

#ଅନ୍ୟଥା /* !CONFIG_SCHED_HRTICK: */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool hrtick_enabled_fair(ସଂରଚନା rq *rq) { ଫେରନ୍ତୁ false; }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool hrtick_enabled_dl(ସଂରଚନା rq *rq) { ଫେରନ୍ତୁ false; }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool hrtick_enabled(ସଂରଚନା rq *rq) { ଫେରନ୍ତୁ false; }
#endif /* !CONFIG_SCHED_HRTICK */

#ifndef arch_scale_freq_tick
ସ୍ଥିର __always_inline ଶୂନ୍ୟ arch_scale_freq_tick(ଶୂନ୍ୟ) { }
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
ସ୍ଥିର __always_inline
unsigned long arch_scale_freq_capacity(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ଫେରନ୍ତୁ SCHED_CAPACITY_SCALE;
}
#endif

/*
 * In double_lock_balance()/double_rq_lock(), we use raw_spin_rq_lock() to
 * acquire rq lock instead of rq_lock(). So at the end of these two functions
 * we need to call double_rq_clock_clear_update() to clear RQCF_UPDATED of
 * rq->clock_update_flags to avoid the WARN_DOUBLE_CLOCK warning.
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ double_rq_clock_clear_update(ସଂରଚନା rq *rq1, ସଂରଚନା rq *rq2)
{
	rq1->clock_update_flags &= (RQCF_REQ_SKIP|RQCF_ACT_SKIP);
	rq2->clock_update_flags &= (RQCF_REQ_SKIP|RQCF_ACT_SKIP);
}

#define DEFINE_LOCK_GUARD_2(name, type, _lock, _unlock, ...)				\
__DEFINE_UNLOCK_GUARD(name, type, _unlock, type *lock2; __VA_ARGS__)			\
ସ୍ଥିର ଧାଡ଼ିଭିତରେ class_##name##_t class_##name##_constructor(type *lock, type *lock2)	\
	__no_context_analysis								\
{ class_##name##_t _t = { .lock = lock, .lock2 = lock2 }, *_T = &_t;			\
  _lock; ଫେରନ୍ତୁ _t; }
#define DECLARE_LOCK_GUARD_2_ATTRS(_name, _lock, _unlock1, _unlock2)			\
ସ୍ଥିର ଧାଡ଼ିଭିତରେ class_##_name##_t class_##_name##_constructor(lock_##_name##_t *_T1,	\
							    lock_##_name##_t *_T2) _lock; \
ସ୍ଥିର __always_inline ଶୂନ୍ୟ __class_##_name##_cleanup_ctx1(class_##_name##_t **_T1)	\
	__no_context_analysis _unlock1 { }						\
ସ୍ଥିର __always_inline ଶୂନ୍ୟ __class_##_name##_cleanup_ctx2(class_##_name##_t **_T2)	\
	__no_context_analysis _unlock2 { }
#define WITH_LOCK_GUARD_2_ATTRS(_name, _T1, _T2)					\
	class_##_name##_constructor(_T1, _T2),						\
	*__UNIQUE_ID(unlock1) __cleanup(__class_##_name##_cleanup_ctx1) = (ଶୂନ୍ୟ *)(_T1),\
	*__UNIQUE_ID(unlock2) __cleanup(__class_##_name##_cleanup_ctx2) = (ଶୂନ୍ୟ *)(_T2)

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool rq_order_less(ସଂରଚନା rq *rq1, ସଂରଚନା rq *rq2)
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
	ଯଦି (rq1->core->cpu < rq2->core->cpu)
		ଫେରନ୍ତୁ true;
	ଯଦି (rq1->core->cpu > rq2->core->cpu)
		ଫେରନ୍ତୁ false;

	/*
	 * __sched_core_flip() relies on SMT having cpu-id lock order.
	 */
#endif /* CONFIG_SCHED_CORE */
	ଫେରନ୍ତୁ rq1->cpu < rq2->cpu;
}

ବାହ୍ୟ ଶୂନ୍ୟ double_rq_lock(ସଂରଚନା rq *rq1, ସଂରଚନା rq *rq2)
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
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା _double_lock_balance(ସଂରଚନା rq *this_rq, ସଂରଚନା rq *busiest)
	__must_hold(__rq_lockp(this_rq))
	__acquires(__rq_lockp(busiest))
{
	raw_spin_rq_unlock(this_rq);
	double_rq_lock(this_rq, busiest);

	ଫେରନ୍ତୁ 1;
}

#ଅନ୍ୟଥା /* !CONFIG_PREEMPTION: */
/*
 * Unfair double_lock_balance: Optimizes throughput at the expense of
 * latency by eliminating extra atomic operations when the locks are
 * already in proper order on entry.  This favors lower CPU-ids and will
 * grant the double lock to lower CPUs over higher ids under contention,
 * regardless of entry order into the function.
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା _double_lock_balance(ସଂରଚନା rq *this_rq, ସଂରଚନା rq *busiest)
	__must_hold(__rq_lockp(this_rq))
	__acquires(__rq_lockp(busiest))
{
	ଯଦି (__rq_lockp(this_rq) == __rq_lockp(busiest)) {
		__acquire(__rq_lockp(busiest)); /* already held */
		double_rq_clock_clear_update(this_rq, busiest);
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (likely(raw_spin_rq_trylock(busiest))) {
		double_rq_clock_clear_update(this_rq, busiest);
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (rq_order_less(this_rq, busiest)) {
		raw_spin_rq_lock_nested(busiest, SINGLE_DEPTH_NESTING);
		double_rq_clock_clear_update(this_rq, busiest);
		ଫେରନ୍ତୁ 0;
	}

	raw_spin_rq_unlock(this_rq);
	double_rq_lock(this_rq, busiest);

	ଫେରନ୍ତୁ 1;
}

#endif /* !CONFIG_PREEMPTION */

/*
 * double_lock_balance - lock the busiest runqueue, this_rq is locked already.
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା double_lock_balance(ସଂରଚନା rq *this_rq, ସଂରଚନା rq *busiest)
	__must_hold(__rq_lockp(this_rq))
	__acquires(__rq_lockp(busiest))
{
	lockdep_assert_irqs_disabled();

	ଫେରନ୍ତୁ _double_lock_balance(this_rq, busiest);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ double_unlock_balance(ସଂରଚନା rq *this_rq, ସଂରଚନା rq *busiest)
	__releases(__rq_lockp(busiest))
{
	ଯଦି (__rq_lockp(this_rq) != __rq_lockp(busiest))
		raw_spin_rq_unlock(busiest);
	ଅନ୍ୟଥା
		__release(__rq_lockp(busiest)); /* fake release */
	lock_set_subclass(&__rq_lockp(this_rq)->dep_map, 0, _RET_IP_);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ double_lock(spinlock_t *l1, spinlock_t *l2)
	__acquires(l1, l2)
{
	ଯଦି (l1 > l2)
		swap(l1, l2);

	spin_lock(l1);
	spin_lock_nested(l2, SINGLE_DEPTH_NESTING);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ double_lock_irq(spinlock_t *l1, spinlock_t *l2)
	__acquires(l1, l2)
{
	ଯଦି (l1 > l2)
		swap(l1, l2);

	spin_lock_irq(l1);
	spin_lock_nested(l2, SINGLE_DEPTH_NESTING);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ double_raw_lock(raw_spinlock_t *l1, raw_spinlock_t *l2)
	__acquires(l1, l2)
{
	ଯଦି (l1 > l2)
		swap(l1, l2);

	raw_spin_lock(l1);
	raw_spin_lock_nested(l2, SINGLE_DEPTH_NESTING);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ double_raw_unlock(raw_spinlock_t *l1, raw_spinlock_t *l2)
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
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ double_rq_unlock(ସଂରଚନା rq *rq1, ସଂରଚନା rq *rq2)
	__releases(__rq_lockp(rq1), __rq_lockp(rq2))
{
	ଯଦି (__rq_lockp(rq1) != __rq_lockp(rq2))
		raw_spin_rq_unlock(rq2);
	ଅନ୍ୟଥା
		__release(__rq_lockp(rq2)); /* fake release */
	raw_spin_rq_unlock(rq1);
}

ବାହ୍ୟ ଶୂନ୍ୟ set_rq_online (ସଂରଚନା rq *rq);
ବାହ୍ୟ ଶୂନ୍ୟ set_rq_offline(ସଂରଚନା rq *rq);

ବାହ୍ୟ bool sched_smp_initialized;

DEFINE_LOCK_GUARD_2(double_rq_lock, ସଂରଚନା rq,
		    double_rq_lock(_T->lock, _T->lock2),
		    double_rq_unlock(_T->lock, _T->lock2))

ବାହ୍ୟ ସଂରଚନା sched_entity *__pick_root_entity(ସଂରଚନା cfs_rq *cfs_rq);
ବାହ୍ୟ ସଂରଚନା sched_entity *__pick_first_entity(ସଂରଚନା cfs_rq *cfs_rq);
ବାହ୍ୟ ସଂରଚନା sched_entity *__pick_last_entity(ସଂରଚନା cfs_rq *cfs_rq);

ବାହ୍ୟ bool sched_debug_verbose;

ବାହ୍ୟ ଶୂନ୍ୟ print_cfs_stats(ସଂରଚନା seq_file *m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu);
ବାହ୍ୟ ଶୂନ୍ୟ print_rt_stats(ସଂରଚନା seq_file *m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu);
ବାହ୍ୟ ଶୂନ୍ୟ print_dl_stats(ସଂରଚନା seq_file *m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu);
ବାହ୍ୟ ଶୂନ୍ୟ print_cfs_rq(ସଂରଚନା seq_file *m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu, ସଂରଚନା cfs_rq *cfs_rq);
ବାହ୍ୟ ଶୂନ୍ୟ print_rt_rq(ସଂରଚନା seq_file *m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu, ସଂରଚନା rt_rq *rt_rq);
ବାହ୍ୟ ଶୂନ୍ୟ print_dl_rq(ସଂରଚନା seq_file *m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu, ସଂରଚନା dl_rq *dl_rq);

ବାହ୍ୟ ଶୂନ୍ୟ resched_latency_warn(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu, u64 latency);

#ifdef CONFIG_NUMA_BALANCING
ବାହ୍ୟ ଶୂନ୍ୟ show_numa_stats(ସଂରଚନା task_struct *p, ସଂରଚନା seq_file *m);
ବାହ୍ୟ ଶୂନ୍ୟ
print_numa_stats(ସଂରଚନା seq_file *m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣ୍ଠି, unsigned long tsf,
		 unsigned long tpf, unsigned long gsf, unsigned long gpf);
#endif /* CONFIG_NUMA_BALANCING */

ବାହ୍ୟ ଶୂନ୍ୟ init_cfs_rq(ସଂରଚନା cfs_rq *cfs_rq);
ବାହ୍ୟ ଶୂନ୍ୟ init_rt_rq(ସଂରଚନା rt_rq *rt_rq);
ବାହ୍ୟ ଶୂନ୍ୟ init_dl_rq(ସଂରଚନା dl_rq *dl_rq);

ବାହ୍ୟ ଶୂନ୍ୟ cfs_bandwidth_usage_inc(ଶୂନ୍ୟ);
ବାହ୍ୟ ଶୂନ୍ୟ cfs_bandwidth_usage_dec(ଶୂନ୍ୟ);

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

ବାହ୍ୟ ଶୂନ୍ୟ nohz_balance_exit_idle(ସଂରଚନା rq *rq);
#ଅନ୍ୟଥା /* !CONFIG_NO_HZ_COMMON: */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ nohz_balance_exit_idle(ସଂରଚନା rq *rq) { }
#endif /* !CONFIG_NO_HZ_COMMON */

#ifdef CONFIG_NO_HZ_COMMON
ବାହ୍ୟ ଶୂନ୍ୟ nohz_run_idle_balance(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu);
#ଅନ୍ୟଥା
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ nohz_run_idle_balance(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu) { }
#endif

#ଅନ୍ତର୍ଭାଵ "stats.h"

#ଯଦି defined(CONFIG_SCHED_CORE) && defined(CONFIG_SCHEDSTATS)

ବାହ୍ୟ ଶୂନ୍ୟ __sched_core_account_forceidle(ସଂରଚନା rq *rq);

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ sched_core_account_forceidle(ସଂରଚନା rq *rq)
{
	ଯଦି (schedstat_enabled())
		__sched_core_account_forceidle(rq);
}

ବାହ୍ୟ ଶୂନ୍ୟ __sched_core_tick(ସଂରଚନା rq *rq);

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ sched_core_tick(ସଂରଚନା rq *rq)
{
	ଯଦି (sched_core_enabled(rq) && schedstat_enabled())
		__sched_core_tick(rq);
}

#ଅନ୍ୟଥା /* !(CONFIG_SCHED_CORE && CONFIG_SCHEDSTATS): */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ sched_core_account_forceidle(ସଂରଚନା rq *rq) { }

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ sched_core_tick(ସଂରଚନା rq *rq) { }

#endif /* !(CONFIG_SCHED_CORE && CONFIG_SCHEDSTATS) */

#ifdef CONFIG_IRQ_TIME_ACCOUNTING

ସଂରଚନା irqtime {
	u64			total;
	u64			tick_delta;
	u64			irq_start_time;
	ସଂରଚନା u64_stats_sync	sync;
};

DECLARE_PER_CPU(ସଂରଚନା irqtime, cpu_irqtime);
DECLARE_STATIC_ଚାବି_FALSE(sched_clock_irqtime);

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା irqtime_enabled(ଶୂନ୍ୟ)
{
	ଫେରନ୍ତୁ static_branch_likely(&sched_clock_irqtime);
}

/*
 * Returns the irqtime minus the softirq time computed by ksoftirqd.
 * Otherwise ksoftirqd's sum_exec_runtime is subtracted its own runtime
 * and never move forward.
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ u64 irq_time_read(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ସଂରଚନା irqtime *irqtime = &per_cpu(cpu_irqtime, cpu);
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା seq;
	u64 total;

	କର {
		seq = __u64_stats_fetch_begin(&irqtime->sync);
		total = irqtime->total;
	} ଯେତେବେଳେ (__u64_stats_fetch_retry(&irqtime->sync, seq));

	ଫେରନ୍ତୁ total;
}

#ଅନ୍ୟଥା /* !CONFIG_IRQ_TIME_ACCOUNTING: */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା irqtime_enabled(ଶୂନ୍ୟ)
{
	ଫେରନ୍ତୁ 0;
}

#endif /* !CONFIG_IRQ_TIME_ACCOUNTING */

#ifdef CONFIG_CPU_FREQ

DECLARE_PER_CPU(ସଂରଚନା update_util_data __rcu *, cpufreq_update_util_data);

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
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ cpufreq_update_util(ସଂରଚନା rq *rq, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
	ସଂରଚନା update_util_data *data;

	data = rcu_dereference_sched(*per_cpu_ptr(&cpufreq_update_util_data,
						  cpu_of(rq)));
	ଯଦି (data)
		data->func(data, rq_clock(rq), flags);
}
#ଅନ୍ୟଥା /* !CONFIG_CPU_FREQ: */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ cpufreq_update_util(ସଂରଚନା rq *rq, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags) { }
#endif /* !CONFIG_CPU_FREQ */

#ifdef arch_scale_freq_capacity
# ifndef arch_scale_freq_invariant
#  define arch_scale_freq_invariant()	true
# endif
#ଅନ୍ୟଥା
# define arch_scale_freq_invariant()	false
#endif

unsigned long effective_cpu_util(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu, unsigned long util_cfs,
				 unsigned long *min,
				 unsigned long *max);

unsigned long sugov_effective_cpu_perf(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu, unsigned long actual,
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
ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool dl_task_fits_capacity(ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	unsigned long cap = arch_scale_cpu_capacity(cpu);

	ଫେରନ୍ତୁ cap >= p->dl.dl_density >> (BW_SHIFT - SCHED_CAPACITY_SHIFT);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned long cpu_bw_dl(ସଂରଚନା rq *rq)
{
	ଫେରନ୍ତୁ (rq->dl.running_bw * SCHED_CAPACITY_SCALE) >> BW_SHIFT;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned long cpu_util_dl(ସଂରଚନା rq *rq)
{
	ଫେରନ୍ତୁ READ_ONCE(rq->avg_dl.util_avg);
}


ବାହ୍ୟ unsigned long cpu_util_cfs(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu);
ବାହ୍ୟ unsigned long cpu_util_cfs_boost(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu);

ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned long cpu_util_rt(ସଂରଚନା rq *rq)
{
	ଫେରନ୍ତୁ READ_ONCE(rq->avg_rt.util_avg);
}

#ifdef CONFIG_UCLAMP_TASK

unsigned long uclamp_eff_ମୂଲ୍ୟ(ସଂରଚନା task_struct *p, ଗଣନା uclamp_id clamp_id);

/*
 * When uclamp is compiled in, the aggregation at rq level is 'turned off'
 * by default in the fast path and only gets turned on once userspace performs
 * an operation that requires it.
 *
 * Returns true if userspace opted-in to use uclamp and aggregation at rq level
 * hence is active.
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool uclamp_is_used(ଶୂନ୍ୟ)
{
	ଫେରନ୍ତୁ static_branch_likely(&sched_uclamp_used);
}

/*
 * Enabling static branches would get the cpus_read_lock(),
 * check whether uclamp_is_used before enable it to avoid always
 * calling cpus_read_lock(). Because we never disable this
 * static key once enable it.
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ sched_uclamp_enable(ଶୂନ୍ୟ)
{
	ଯଦି (!uclamp_is_used())
		static_branch_enable(&sched_uclamp_used);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned long uclamp_rq_get(ସଂରଚନା rq *rq,
					  ଗଣନା uclamp_id clamp_id)
{
	ଫେରନ୍ତୁ READ_ONCE(rq->uclamp[clamp_id].ମୂଲ୍ୟ);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ uclamp_rq_set(ସଂରଚନା rq *rq, ଗଣନା uclamp_id clamp_id,
				 unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୂଲ୍ୟ)
{
	WRITE_ONCE(rq->uclamp[clamp_id].ମୂଲ୍ୟ, ମୂଲ୍ୟ);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool uclamp_rq_is_idle(ସଂରଚନା rq *rq)
{
	ଫେରନ୍ତୁ rq->uclamp_flags & UCLAMP_FLAG_IDLE;
}

/* Is the rq being capped/throttled by uclamp_max? */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool uclamp_rq_is_capped(ସଂରଚନା rq *rq)
{
	unsigned long rq_util;
	unsigned long max_util;

	ଯଦି (!uclamp_is_used())
		ଫେରନ୍ତୁ false;

	rq_util = cpu_util_cfs(cpu_of(rq)) + cpu_util_rt(rq);
	max_util = READ_ONCE(rq->uclamp[UCLAMP_MAX].ମୂଲ୍ୟ);

	ଫେରନ୍ତୁ max_util != SCHED_CAPACITY_SCALE && rq_util >= max_util;
}

#define for_each_clamp_id(clamp_id) \
	ପାଇଁ ((clamp_id) = 0; (clamp_id) < UCLAMP_CNT; (clamp_id)++)

ବାହ୍ୟ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା sysctl_sched_uclamp_util_min_rt_default;


ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା uclamp_none(ଗଣନା uclamp_id clamp_id)
{
	ଯଦି (clamp_id == UCLAMP_MIN)
		ଫେରନ୍ତୁ 0;
	ଫେରନ୍ତୁ SCHED_CAPACITY_SCALE;
}

/* Integer rounded range for each bucket */
#define UCLAMP_BUCKET_DELTA DIV_ROUND_CLOSEST(SCHED_CAPACITY_SCALE, UCLAMP_BUCKETS)

ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା uclamp_bucket_id(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା clamp_ମୂଲ୍ୟ)
{
	ଫେରନ୍ତୁ min_t(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା, clamp_ମୂଲ୍ୟ / UCLAMP_BUCKET_DELTA, UCLAMP_BUCKETS - 1);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
uclamp_se_set(ସଂରଚନା uclamp_se *uc_se, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୂଲ୍ୟ, bool user_defined)
{
	uc_se->ମୂଲ୍ୟ = ମୂଲ୍ୟ;
	uc_se->bucket_id = uclamp_bucket_id(ମୂଲ୍ୟ);
	uc_se->user_defined = user_defined;
}

#ଅନ୍ୟଥା /* !CONFIG_UCLAMP_TASK: */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned long
uclamp_eff_ମୂଲ୍ୟ(ସଂରଚନା task_struct *p, ଗଣନା uclamp_id clamp_id)
{
	ଯଦି (clamp_id == UCLAMP_MIN)
		ଫେରନ୍ତୁ 0;

	ଫେରନ୍ତୁ SCHED_CAPACITY_SCALE;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool uclamp_rq_is_capped(ସଂରଚନା rq *rq) { ଫେରନ୍ତୁ false; }

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool uclamp_is_used(ଶୂନ୍ୟ)
{
	ଫେରନ୍ତୁ false;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ sched_uclamp_enable(ଶୂନ୍ୟ) {}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned long
uclamp_rq_get(ସଂରଚନା rq *rq, ଗଣନା uclamp_id clamp_id)
{
	ଯଦି (clamp_id == UCLAMP_MIN)
		ଫେରନ୍ତୁ 0;

	ଫେରନ୍ତୁ SCHED_CAPACITY_SCALE;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
uclamp_rq_set(ସଂରଚନା rq *rq, ଗଣନା uclamp_id clamp_id, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୂଲ୍ୟ)
{
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool uclamp_rq_is_idle(ସଂରଚନା rq *rq)
{
	ଫେରନ୍ତୁ false;
}

#endif /* !CONFIG_UCLAMP_TASK */

#ifdef CONFIG_HAVE_SCHED_AVG_IRQ

ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned long cpu_util_irq(ସଂରଚନା rq *rq)
{
	ଫେରନ୍ତୁ READ_ONCE(rq->avg_irq.util_avg);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ
unsigned long scale_irq_capacity(unsigned long util, unsigned long irq, unsigned long max)
{
	util *= (max - irq);
	util /= max;

	ଫେରନ୍ତୁ util;

}

#ଅନ୍ୟଥା /* !CONFIG_HAVE_SCHED_AVG_IRQ: */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned long cpu_util_irq(ସଂରଚନା rq *rq)
{
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ
unsigned long scale_irq_capacity(unsigned long util, unsigned long irq, unsigned long max)
{
	ଫେରନ୍ତୁ util;
}

#endif /* !CONFIG_HAVE_SCHED_AVG_IRQ */

ବାହ୍ୟ ଶୂନ୍ୟ __setparam_fair(ସଂରଚନା task_struct *p, ଅଚଳ ସଂରଚନା sched_attr *attr);

#ଯଦି defined(CONFIG_ENERGY_MODEL) && defined(CONFIG_CPU_FREQ_GOV_SCHEDUTIL)

#define perf_domain_span(pd) (to_cpumask(((pd)->em_pd->cpus)))

DECLARE_STATIC_ଚାବି_FALSE(sched_energy_present);

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool sched_energy_enabled(ଶୂନ୍ୟ)
{
	ଫେରନ୍ତୁ static_branch_unlikely(&sched_energy_present);
}

#ଅନ୍ୟଥା /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL): */

#define perf_domain_span(pd) ଖାଲି

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool sched_energy_enabled(ଶୂନ୍ୟ) { ଫେରନ୍ତୁ false; }

#endif /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL) */

#ifdef CONFIG_MEMBARRIER

/*
 * The scheduler provides memory barriers required by membarrier between:
 * - prior user-space memory accesses and store to rq->membarrier_state,
 * - store to rq->membarrier_state and following user-space memory accesses.
 * In the same way it provides those guarantees around store to rq->curr.
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ membarrier_switch_mm(ସଂରଚନା rq *rq,
					ସଂରଚନା mm_struct *prev_mm,
					ସଂରଚନା mm_struct *next_mm)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା membarrier_state;

	ଯଦି (prev_mm == next_mm)
		ଫେରନ୍ତୁ;

	membarrier_state = atomic_read(&next_mm->membarrier_state);
	ଯଦି (READ_ONCE(rq->membarrier_state) == membarrier_state)
		ଫେରନ୍ତୁ;

	WRITE_ONCE(rq->membarrier_state, membarrier_state);
}

#ଅନ୍ୟଥା /* !CONFIG_MEMBARRIER: */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ membarrier_switch_mm(ସଂରଚନା rq *rq,
					ସଂରଚନା mm_struct *prev_mm,
					ସଂରଚନା mm_struct *next_mm)
{
}

#endif /* !CONFIG_MEMBARRIER */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool is_per_cpu_kthread(ସଂରଚନା task_struct *p)
{
	ଯଦି (!(p->flags & PF_KTHREAD))
		ଫେରନ୍ତୁ false;

	ଯଦି (p->nr_cpus_allowed != 1)
		ଫେରନ୍ତୁ false;

	ଫେରନ୍ତୁ true;
}

ବାହ୍ୟ ଶୂନ୍ୟ swake_up_all_locked(ସଂରଚନା swait_queue_head *q);
ବାହ୍ୟ ଶୂନ୍ୟ __prepare_to_swait(ସଂରଚନା swait_queue_head *q, ସଂରଚନା swait_queue *wait);

ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା try_to_wake_up(ସଂରଚନା task_struct *tsk, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା state, ପୂର୍ଣ୍ଣସଂଖ୍ୟା wake_flags);

#ifdef CONFIG_PREEMPT_DYNAMIC
ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା preempt_dynamic_mode;
ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା sched_dynamic_mode(ଅଚଳ ଅକ୍ଷର *str);
ବାହ୍ୟ ଶୂନ୍ୟ sched_dynamic_update(ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode);
#endif
ବାହ୍ୟ ଅଚଳ ଅକ୍ଷର *preempt_modes[];

#ifdef CONFIG_SCHED_MM_CID

ସ୍ଥିର __always_inline bool cid_on_cpu(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cid)
{
	ଫେରନ୍ତୁ cid & MM_CID_ONCPU;
}

ସ୍ଥିର __always_inline bool cid_in_transit(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cid)
{
	ଫେରନ୍ତୁ cid & MM_CID_TRANSIT;
}

ସ୍ଥିର __always_inline unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu_cid_to_cid(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cid)
{
	ଫେରନ୍ତୁ cid & ~MM_CID_ONCPU;
}

ସ୍ଥିର __always_inline unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cid_to_cpu_cid(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cid)
{
	ଫେରନ୍ତୁ cid | MM_CID_ONCPU;
}

ସ୍ଥିର __always_inline unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cid_to_transit_cid(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cid)
{
	ଫେରନ୍ତୁ cid | MM_CID_TRANSIT;
}

ସ୍ଥିର __always_inline unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cid_from_transit_cid(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cid)
{
	ଫେରନ୍ତୁ cid & ~MM_CID_TRANSIT;
}

ସ୍ଥିର __always_inline bool cid_on_task(unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cid)
{
	/* True if none of the MM_CID_ONCPU, MM_CID_TRANSIT, MM_CID_UNSET bits is set */
	ଫେରନ୍ତୁ cid < MM_CID_TRANSIT;
}

ସ୍ଥିର __always_inline ଶୂନ୍ୟ mm_drop_cid(ସଂରଚନା mm_struct *mm, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cid)
{
	clear_bit(cid, mm_cidmask(mm));
}

ସ୍ଥିର __always_inline ଶୂନ୍ୟ mm_unset_cid_on_task(ସଂରଚନା task_struct *t)
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cid = t->mm_cid.cid;

	t->mm_cid.cid = MM_CID_UNSET;
	ଯଦି (cid_on_task(cid))
		mm_drop_cid(t->mm, cid);
}

ସ୍ଥିର __always_inline ଶୂନ୍ୟ mm_drop_cid_on_cpu(ସଂରଚନା mm_struct *mm, ସଂରଚନା mm_cid_pcpu *pcp)
{
	/* Clear the ONCPU bit, but do not set UNSET in the per CPU storage */
	ଯଦି (cid_on_cpu(pcp->cid)) {
		pcp->cid = cpu_cid_to_cid(pcp->cid);
		mm_drop_cid(mm, pcp->cid);
	}
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା __mm_get_cid(ସଂରଚନା mm_struct *mm, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_cids)
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cid = find_first_zero_bit(mm_cidmask(mm), max_cids);

	ଯଦି (cid >= max_cids)
		ଫେରନ୍ତୁ MM_CID_UNSET;
	ଯଦି (test_and_set_bit(cid, mm_cidmask(mm)))
		ଫେରନ୍ତୁ MM_CID_UNSET;
	ଫେରନ୍ତୁ cid;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା mm_get_cid(ସଂରଚନା mm_struct *mm)
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cid = __mm_get_cid(mm, READ_ONCE(mm->mm_cid.max_cids));

	ଯେତେବେଳେ (cid == MM_CID_UNSET) {
		cpu_relax();
		cid = __mm_get_cid(mm, ଅଙ୍କ_possible_cpus());
	}
	ଫେରନ୍ତୁ cid;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା mm_cid_converge(ସଂରଚନା mm_struct *mm, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା orig_cid,
					   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_cids)
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_cid, cid = cpu_cid_to_cid(orig_cid);

	/* Is it in the optimal CID space? */
	ଯଦି (likely(cid < max_cids))
		ଫେରନ୍ତୁ orig_cid;

	/* Try to find one in the optimal space. Otherwise keep the provided. */
	new_cid = __mm_get_cid(mm, max_cids);
	ଯଦି (new_cid != MM_CID_UNSET) {
		mm_drop_cid(mm, cid);
		/* Preserve the ONCPU mode of the original CID */
		ଫେରନ୍ତୁ new_cid | (orig_cid & MM_CID_ONCPU);
	}
	ଫେରନ୍ତୁ orig_cid;
}

ସ୍ଥିର __always_inline ଶୂନ୍ୟ mm_cid_update_task_cid(ସଂରଚନା task_struct *t, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cid)
{
	ଯଦି (t->mm_cid.cid != cid) {
		t->mm_cid.cid = cid;
		rseq_sched_set_ids_changed(t);
	}
}

ସ୍ଥିର __always_inline ଶୂନ୍ୟ mm_cid_update_pcpu_cid(ସଂରଚନା mm_struct *mm, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cid)
{
	__this_cpu_write(mm->mm_cid.pcpu->cid, cid);
}

ସ୍ଥିର __always_inline ଶୂନ୍ୟ mm_cid_from_cpu(ସଂରଚନା task_struct *t, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu_cid,
					    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode)
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_cids, tcid = t->mm_cid.cid;
	ସଂରଚନା mm_struct *mm = t->mm;

	max_cids = READ_ONCE(mm->mm_cid.max_cids);
	/* Optimize for the common case where both have the ONCPU bit set */
	ଯଦି (likely(cid_on_cpu(cpu_cid & tcid))) {
		ଯଦି (likely(cpu_cid_to_cid(cpu_cid) < max_cids)) {
			mm_cid_update_task_cid(t, cpu_cid);
			ଫେରନ୍ତୁ;
		}
		/* Try to converge into the optimal CID space */
		cpu_cid = mm_cid_converge(mm, cpu_cid, max_cids);
	} ଅନ୍ୟଥା {
		/* Hand over or drop the task owned CID */
		ଯଦି (cid_on_task(tcid)) {
			ଯଦି (cid_on_cpu(cpu_cid))
				mm_unset_cid_on_task(t);
			ଅନ୍ୟଥା
				cpu_cid = cid_to_cpu_cid(tcid);
		}
		/* Still nothing, allocate a new one */
		ଯଦି (!cid_on_cpu(cpu_cid))
			cpu_cid = cid_to_cpu_cid(mm_get_cid(mm));

		/* Handle the transition mode flag if required */
		ଯଦି (mode & MM_CID_TRANSIT)
			cpu_cid = cpu_cid_to_cid(cpu_cid) | MM_CID_TRANSIT;
	}
	mm_cid_update_pcpu_cid(mm, cpu_cid);
	mm_cid_update_task_cid(t, cpu_cid);
}

ସ୍ଥିର __always_inline ଶୂନ୍ୟ mm_cid_from_task(ସଂରଚନା task_struct *t, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu_cid,
					     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode)
{
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା max_cids, tcid = t->mm_cid.cid;
	ସଂରଚନା mm_struct *mm = t->mm;

	max_cids = READ_ONCE(mm->mm_cid.max_cids);
	/* Optimize for the common case, where both have the ONCPU bit clear */
	ଯଦି (likely(cid_on_task(tcid | cpu_cid))) {
		ଯଦି (likely(tcid < max_cids)) {
			mm_cid_update_pcpu_cid(mm, tcid);
			ଫେରନ୍ତୁ;
		}
		/* Try to converge into the optimal CID space */
		tcid = mm_cid_converge(mm, tcid, max_cids);
	} ଅନ୍ୟଥା {
		/* Hand over or drop the CPU owned CID */
		ଯଦି (cid_on_cpu(cpu_cid)) {
			ଯଦି (cid_on_task(tcid))
				mm_drop_cid_on_cpu(mm, this_cpu_ptr(mm->mm_cid.pcpu));
			ଅନ୍ୟଥା
				tcid = cpu_cid_to_cid(cpu_cid);
		}
		/* Still nothing, allocate a new one */
		ଯଦି (!cid_on_task(tcid))
			tcid = mm_get_cid(mm);
		/* Set the transition mode flag if required */
		tcid |= mode & MM_CID_TRANSIT;
	}
	mm_cid_update_pcpu_cid(mm, tcid);
	mm_cid_update_task_cid(t, tcid);
}

ସ୍ଥିର __always_inline ଶୂନ୍ୟ mm_cid_schedin(ସଂରଚନା task_struct *next)
{
	ସଂରଚନା mm_struct *mm = next->mm;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu_cid, mode;

	ଯଦି (!next->mm_cid.active)
		ଫେରନ୍ତୁ;

	cpu_cid = __this_cpu_read(mm->mm_cid.pcpu->cid);
	mode = READ_ONCE(mm->mm_cid.mode);
	ଯଦି (likely(!cid_on_cpu(mode)))
		mm_cid_from_task(next, cpu_cid, mode);
	ଅନ୍ୟଥା
		mm_cid_from_cpu(next, cpu_cid, mode);
}

ସ୍ଥିର __always_inline ଶୂନ୍ୟ mm_cid_schedout(ସଂରଚନା task_struct *prev)
{
	ସଂରଚନା mm_struct *mm = prev->mm;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode, cid;

	/* During mode transitions CIDs are temporary and need to be dropped */
	ଯଦି (likely(!cid_in_transit(prev->mm_cid.cid)))
		ଫେରନ୍ତୁ;

	mode = READ_ONCE(mm->mm_cid.mode);
	cid = cid_from_transit_cid(prev->mm_cid.cid);

	/*
	 * If transition mode is done, transfer ownership when the CID is
	 * within the convergence range to optimize the next schedule in.
	 */
	ଯଦି (!cid_in_transit(mode) && cid < READ_ONCE(mm->mm_cid.max_cids)) {
		ଯଦି (cid_on_cpu(mode))
			cid = cid_to_cpu_cid(cid);

		/* Update both so that the next schedule in goes into the fast path */
		mm_cid_update_pcpu_cid(mm, cid);
		prev->mm_cid.cid = cid;
	} ଅନ୍ୟଥା {
		mm_drop_cid(mm, cid);
		prev->mm_cid.cid = MM_CID_UNSET;
	}
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ mm_cid_switch_to(ସଂରଚନା task_struct *prev, ସଂରଚନା task_struct *next)
{
	mm_cid_schedout(prev);
	mm_cid_schedin(next);
}

#ଅନ୍ୟଥା /* !CONFIG_SCHED_MM_CID: */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ mm_cid_switch_to(ସଂରଚନା task_struct *prev, ସଂରଚନା task_struct *next) { }
#endif /* !CONFIG_SCHED_MM_CID */

#ifdef CONFIG_SCHED_CACHE
DECLARE_STATIC_ଚାବି_FALSE(sched_cache_present);
DECLARE_STATIC_ଚାବି_FALSE(sched_cache_active);
ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା sysctl_sched_cache_user;
ବାହ୍ୟ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା llc_aggr_tolerance;
ବାହ୍ୟ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା llc_epoch_period;
ବାହ୍ୟ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା llc_epoch_affinity_timeout;
ବାହ୍ୟ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା llc_imb_pct;
ବାହ୍ୟ unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା llc_overaggr_pct;

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool sched_cache_enabled(ଶୂନ୍ୟ)
{
	ଫେରନ୍ତୁ static_branch_unlikely(&sched_cache_active);
}

ବାହ୍ୟ ଶୂନ୍ୟ sched_cache_active_set(ଶୂନ୍ୟ);

#endif

ଶୂନ୍ୟ sched_domains_free_llc_id(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu);

ବାହ୍ୟ ଶୂନ୍ୟ init_sched_mm(ସଂରଚନା task_struct *p);

ବାହ୍ୟ u64 avg_vruntime(ସଂରଚନା cfs_rq *cfs_rq);
ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା entity_eligible(ସଂରଚନା cfs_rq *cfs_rq, ସଂରଚନା sched_entity *se);
ସ୍ଥିର ଧାଡ଼ିଭିତରେ
ଶୂନ୍ୟ move_queued_task_locked(ସଂରଚନା rq *src_rq, ସଂରଚନା rq *dst_rq, ସଂରଚନା task_struct *task)
{
	lockdep_assert_rq_held(src_rq);
	lockdep_assert_rq_held(dst_rq);

	deactivate_task(src_rq, task, 0);
	set_task_cpu(task, dst_rq->cpu);
	activate_task(dst_rq, task, 0);
	wakeup_preempt(dst_rq, task, 0);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ
bool task_is_pushable(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu)
{
	ଯଦି (!task_on_cpu(rq, p) &&
	    cpumask_test_cpu(cpu, &p->cpus_mask))
		ଫେରନ୍ତୁ true;

	ଫେରନ୍ତୁ false;
}

#ifdef CONFIG_RT_MUTEXES

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା __rt_effective_prio(ସଂରଚନା task_struct *pi_task, ପୂର୍ଣ୍ଣସଂଖ୍ୟା prio)
{
	ଯଦି (pi_task)
		prio = min(prio, pi_task->prio);

	ଫେରନ୍ତୁ prio;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା rt_effective_prio(ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା prio)
{
	ସଂରଚନା task_struct *pi_task = rt_mutex_get_ଶୀର୍ଷ_task(p);

	ଫେରନ୍ତୁ __rt_effective_prio(pi_task, prio);
}

#ଅନ୍ୟଥା /* !CONFIG_RT_MUTEXES: */

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା rt_effective_prio(ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା prio)
{
	ଫେରନ୍ତୁ prio;
}

#endif /* !CONFIG_RT_MUTEXES */

ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା __sched_setscheduler(ସଂରଚନା task_struct *p, ଅଚଳ ସଂରଚନା sched_attr *attr, bool user, bool pi);
ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା __sched_setaffinity(ସଂରଚନା task_struct *p, ସଂରଚନା affinity_context *ctx);
ବାହ୍ୟ ଅଚଳ ସଂରଚନା sched_class *__setscheduler_class(ପୂର୍ଣ୍ଣସଂଖ୍ୟା policy, ପୂର୍ଣ୍ଣସଂଖ୍ୟା prio);
ବାହ୍ୟ ଶୂନ୍ୟ set_load_weight(ସଂରଚନା task_struct *p, bool update_load);
ବାହ୍ୟ ଶୂନ୍ୟ enqueue_task(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags);
ବାହ୍ୟ bool dequeue_task(ସଂରଚନା rq *rq, ସଂରଚନା task_struct *p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags);

ବାହ୍ୟ ସଂରଚନା balance_callback *splice_balance_callbacks(ସଂରଚନା rq *rq);

ବାହ୍ୟ ଶୂନ୍ୟ __balance_callbacks(ସଂରଚନା rq *rq, ସଂରଚନା rq_flags *rf);
ବାହ୍ୟ ଶୂନ୍ୟ balance_callbacks(ସଂରଚନା rq *rq, ସଂରଚନା balance_callback *head);

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
ସଂରଚନା sched_change_ctx {
	u64			prio;
	ସଂରଚନା task_struct	*p;
	ଅଚଳ ସଂରଚନା sched_class *class;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			flags;
	bool			queued;
	bool			running;
};

ସଂରଚନା sched_change_ctx *sched_change_begin(ସଂରଚନା task_struct *p, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags);
ଶୂନ୍ୟ sched_change_end(ସଂରଚନା sched_change_ctx *ctx);

DEFINE_CLASS(sched_change, ସଂରଚନା sched_change_ctx *,
	     sched_change_end(_T),
	     sched_change_begin(p, flags),
	     ସଂରଚନା task_struct *p, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)

DEFINE_CLASS_IS_UNCONDITIONAL(sched_change)

#ଅନ୍ତର୍ଭାଵ "ext/ext.h"

#ifdef CONFIG_PREFERRED_CPU
ଶୂନ୍ୟ sched_ଠେଲ_current_non_preferred_cpu(ସଂରଚନା rq *rq);
#ଅନ୍ୟଥା	/* !CONFIG_PREFERRED_CPU */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ sched_ଠେଲ_current_non_preferred_cpu(ସଂରଚନା rq *rq) { }
#endif

#endif /* _KERNEL_SCHED_SCHED_H */
