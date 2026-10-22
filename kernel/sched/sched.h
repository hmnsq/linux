/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Scheduler internal types and methods:
 */
#నిర్వచితం_కాకపోతే _KERNEL_SCHED_SCHED_H
#నిర్వచించు _KERNEL_SCHED_SCHED_H

#అన్తర్భావ <linux/prandom.h>
#అన్తర్భావ <linux/sched/affinity.h>
#అన్తర్భావ <linux/sched/autogroup.h>
#అన్తర్భావ <linux/sched/cpufreq.h>
#అన్తర్భావ <linux/sched/deadline.h>
#అన్తర్భావ <linux/sched.h>
#అన్తర్భావ <linux/sched/loadavg.h>
#అన్తర్భావ <linux/sched/mm.h>
#అన్తర్భావ <linux/sched/rseq_api.h>
#అన్తర్భావ <linux/sched/signal.h>
#అన్తర్భావ <linux/sched/smt.h>
#అన్తర్భావ <linux/sched/stat.h>
#అన్తర్భావ <linux/sched/sysctl.h>
#అన్తర్భావ <linux/sched/task_flags.h>
#అన్తర్భావ <linux/sched/task.h>
#అన్తర్భావ <linux/sched/topology.h>
#అన్తర్భావ <linux/atomic.h>
#అన్తర్భావ <linux/bitmap.h>
#అన్తర్భావ <linux/bug.h>
#అన్తర్భావ <linux/capability.h>
#అన్తర్భావ <linux/cgroup_api.h>
#అన్తర్భావ <linux/cgroup.h>
#అన్తర్భావ <linux/context_tracking.h>
#అన్తర్భావ <linux/cpufreq.h>
#అన్తర్భావ <linux/cpumask_api.h>
#అన్తర్భావ <linux/cpuset.h>
#అన్తర్భావ <linux/ctype.h>
#అన్తర్భావ <linux/file.h>
#అన్తర్భావ <linux/fs_api.h>
#అన్తర్భావ <linux/hrtimer_api.h>
#అన్తర్భావ <linux/interrupt.h>
#అన్తర్భావ <linux/irq_work.h>
#అన్తర్భావ <linux/jiffies.h>
#అన్తర్భావ <linux/kref_api.h>
#అన్తర్భావ <linux/kthread.h>
#అన్తర్భావ <linux/ktime_api.h>
#అన్తర్భావ <linux/lockdep_api.h>
#అన్తర్భావ <linux/lockdep.h>
#అన్తర్భావ <linux/memblock.h>
#అన్తర్భావ <linux/memcontrol.h>
#అన్తర్భావ <linux/minmax.h>
#అన్తర్భావ <linux/mm.h>
#అన్తర్భావ <linux/module.h>
#అన్తర్భావ <linux/mutex_api.h>
#అన్తర్భావ <linux/plist.h>
#అన్తర్భావ <linux/poll.h>
#అన్తర్భావ <linux/proc_fs.h>
#అన్తర్భావ <linux/profile.h>
#అన్తర్భావ <linux/psi.h>
#అన్తర్భావ <linux/rcupdate.h>
#అన్తర్భావ <linux/seq_file.h>
#అన్తర్భావ <linux/seqlock.h>
#అన్తర్భావ <linux/softirq.h>
#అన్తర్భావ <linux/spinlock_api.h>
#అన్తర్భావ <linux/static_key.h>
#అన్తర్భావ <linux/stop_machine.h>
#అన్తర్భావ <linux/syscalls_api.h>
#అన్తర్భావ <linux/syscalls.h>
#అన్తర్భావ <linux/tick.h>
#అన్తర్భావ <linux/topology.h>
#అన్తర్భావ <linux/types.h>
#అన్తర్భావ <linux/u64_stats_sync_api.h>
#అన్తర్భావ <linux/uaccess.h>
#అన్తర్భావ <linux/vmstat.h>
#అన్తర్భావ <linux/wait_api.h>
#అన్తర్భావ <linux/wait_bit.h>
#అన్తర్భావ <linux/workqueue_api.h>
#అన్తర్భావ <linux/delayacct.h>
#అన్తర్భావ <linux/mmu_context.h>

#అన్తర్భావ <trace/events/power.h>
#అన్తర్భావ <trace/events/sched.h>

#అన్తర్భావ "../workqueue_internal.h"

నిర్మాణం rq;
నిర్మాణం cfs_rq;
నిర్మాణం rt_rq;
నిర్మాణం sched_సమూహం;
నిర్మాణం cpuidle_state;

#ఒకవేళ defined(CONFIG_PARAVIRT) && !defined(CONFIG_HAVE_PV_STEAL_CLOCK_GEN)
# include <asm/paravirt.h>
#అయితే_ముగింపు

#అన్తర్భావ <asm/barrier.h>

#అన్తర్భావ "cpupri.h"
#అన్తర్భావ "cpudeadline.h"

/* task_struct::on_rq states: */
#నిర్వచించు TASK_ON_RQ_QUEUED	1
#నిర్వచించు TASK_ON_RQ_MIGRATING	2

బాహ్య __read_mostly పూర్ణాంకం scheduler_running;

బాహ్య గుర్తులేని దీర్ఘ calc_ఎక్కించు_update;
బాహ్య atomic_long_t calc_ఎక్కించు_tasks;

బాహ్య శూన్య calc_global_ఎక్కించు_tick(నిర్మాణం rq *this_rq);
బాహ్య దీర్ఘ calc_ఎక్కించు_fold_చురుకైన(నిర్మాణం rq *this_rq, దీర్ఘ adjust);

బాహ్య శూన్య call_trace_sched_update_nr_running(నిర్మాణం rq *rq, పూర్ణాంకం లెక్క);

బాహ్య పూర్ణాంకం sysctl_sched_rt_period;
బాహ్య పూర్ణాంకం sysctl_sched_rt_runtime;
బాహ్య పూర్ణాంకం sched_rr_timeslice;

/*
 * Asymmetric CPU capacity bits
 */
నిర్మాణం asym_cap_దత్తాంశం {
	నిర్మాణం list_తల link;
	నిర్మాణం rcu_తల rcu;
	గుర్తులేని దీర్ఘ సామర్థ్యం;
	గుర్తులేని దీర్ఘ cpus[];
};

బాహ్య నిర్మాణం list_తల asym_cap_list;

#నిర్వచించు cpu_సామర్థ్యం_పరిధి(asym_దత్తాంశం) గమ్యం_cpumask((asym_దత్తాంశం)->cpus)

/*
 * Helpers for converting nanosecond timing to jiffy resolution
 */
#నిర్వచించు NS_గమ్యం_JIFFIES(సమయం)	((గుర్తులేని దీర్ఘ)(సమయం) / (NSEC_చొప్పున_SEC/HZ))

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
#నిర్వచితమైతే CONFIG_64_బిట్టు
# define NICE_0_ఎక్కించు_జరుపు	(SCHED_FIXEDPOINT_జరుపు + SCHED_FIXEDPOINT_జరుపు)
# define scale_ఎక్కించు(w)		((w) << SCHED_FIXEDPOINT_జరుపు)
# define scale_ఎక్కించు_కిందికి(w)					\
({								\
	గుర్తులేని దీర్ఘ __w = (w);				\
								\
	ఒకవేళ (__w)						\
		__w = max(2UL, __w >> SCHED_FIXEDPOINT_జరుపు);	\
	__w;							\
})
#లేకపోతే
# define NICE_0_ఎక్కించు_జరుపు	(SCHED_FIXEDPOINT_జరుపు)
# define scale_ఎక్కించు(w)		(w)
# define scale_ఎక్కించు_కిందికి(w)	(w)
#అయితే_ముగింపు

/*
 * Task weight (visible to users) and its load (invisible to users) have
 * independent resolution, but they should be well calibrated. We use
 * scale_load() and scale_load_down(w) to convert between them. The
 * following must be true:
 *
 *  scale_load(sched_prio_to_weight[NICE_TO_PRIO(0)-MAX_RT_PRIO]) == NICE_0_LOAD
 *
 */
#నిర్వచించు NICE_0_ఎక్కించు		(1L << NICE_0_ఎక్కించు_జరుపు)

/*
 * Single value that decides SCHED_DEADLINE internal math precision.
 * 10 -> just above 1us
 * 9  -> just above 0.5us
 */
#నిర్వచించు DL_SCALE		10

/*
 * Single value that denotes runtime == period, ie unlimited time.
 */
#నిర్వచించు RUNTIME_INF		((u64)~0ULL)

స్థిర అంతఃవరుస పూర్ణాంకం idle_policy(పూర్ణాంకం policy)
{
	తిరిగి policy == SCHED_IDLE;
}

స్థిర అంతఃవరుస పూర్ణాంకం సాధారణ_policy(పూర్ణాంకం policy)
{
#నిర్వచితమైతే CONFIG_SCHED_CLASS_EXT
	ఒకవేళ (policy == SCHED_EXT)
		తిరిగి true;
#అయితే_ముగింపు
	తిరిగి policy == SCHED_సాధారణ;
}

స్థిర అంతఃవరుస పూర్ణాంకం fair_policy(పూర్ణాంకం policy)
{
	తిరిగి సాధారణ_policy(policy) || policy == SCHED_BATCH;
}

స్థిర అంతఃవరుస పూర్ణాంకం rt_policy(పూర్ణాంకం policy)
{
	తిరిగి policy == SCHED_FIFO || policy == SCHED_RR;
}

స్థిర అంతఃవరుస పూర్ణాంకం dl_policy(పూర్ణాంకం policy)
{
	తిరిగి policy == SCHED_DEADLINE;
}

స్థిర అంతఃవరుస bool valid_policy(పూర్ణాంకం policy)
{
	తిరిగి idle_policy(policy) || fair_policy(policy) ||
		rt_policy(policy) || dl_policy(policy);
}

స్థిర అంతఃవరుస పూర్ణాంకం task_has_idle_policy(నిర్మాణం task_struct *p)
{
	తిరిగి idle_policy(p->policy);
}

స్థిర అంతఃవరుస పూర్ణాంకం task_has_rt_policy(నిర్మాణం task_struct *p)
{
	తిరిగి rt_policy(p->policy);
}

స్థిర అంతఃవరుస పూర్ణాంకం task_has_dl_policy(నిర్మాణం task_struct *p)
{
	తిరిగి dl_policy(p->policy);
}

#నిర్వచించు cap_scale(v, s)		((v)*(s) >> SCHED_సామర్థ్యం_జరుపు)

స్థిర అంతఃవరుస శూన్య update_avg(u64 *avg, u64 sample)
{
	s64 diff = sample - *avg;

	*avg += diff / 8;
}

/*
 * Shifting a value by an exponent greater *or equal* to the size of said value
 * is UB; cap at size-1.
 */
#నిర్వచించు shr_bound(val, జరుపు)							\
	(val >> min_t(typeof(జరుపు), జరుపు, బిట్లు_చొప్పున_TYPE(typeof(val)) - 1))

/*
 * cgroup weight knobs should use the common MIN, DFL and MAX values which are
 * 1, 100 and 10000 respectively. While it loses a bit of range on both ends, it
 * maps pretty well onto the shares value used by scheduler and the round-trip
 * conversions preserve the original value over the entire range.
 */
స్థిర అంతఃవరుస గుర్తులేని దీర్ఘ sched_బరువు_from_cgroup(గుర్తులేని దీర్ఘ cgrp_బరువు)
{
	తిరిగి DIV_విడత_CLOSEST_ULL(cgrp_బరువు * 1024, CGROUP_బరువు_DFL);
}

స్థిర అంతఃవరుస గుర్తులేని దీర్ఘ sched_బరువు_గమ్యం_cgroup(గుర్తులేని దీర్ఘ బరువు)
{
	తిరిగి clamp_t(గుర్తులేని దీర్ఘ,
		       DIV_విడత_CLOSEST_ULL(బరువు * CGROUP_బరువు_DFL, 1024),
		       CGROUP_బరువు_MIN, CGROUP_బరువు_MAX);
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
#నిర్వచించు SCHED_FLAG_SUGOV	0x10000000

#నిర్వచించు SCHED_DL_FLAGS		(SCHED_FLAG_RECLAIM | SCHED_FLAG_DL_OVERRUN | SCHED_FLAG_SUGOV)

స్థిర అంతఃవరుస bool dl_entity_is_special(స్థిరరాశి నిర్మాణం sched_dl_entity *dl_se)
{
#నిర్వచితమైతే CONFIG_CPU_FREQ_GOV_SCHEDUTIL
	తిరిగి unlikely(dl_se->flags & SCHED_FLAG_SUGOV);
#లేకపోతే
	తిరిగి false;
#అయితే_ముగింపు
}

/*
 * Tells if entity @a should preempt entity @b.
 */
స్థిర అంతఃవరుస bool dl_entity_preempt(స్థిరరాశి నిర్మాణం sched_dl_entity *a,
				     స్థిరరాశి నిర్మాణం sched_dl_entity *b)
{
	తిరిగి dl_entity_is_special(a) ||
	       dl_సమయం_before(a->deadline, b->deadline);
}

/*
 * This is the priority-queue data structure of the RT scheduling class:
 */
నిర్మాణం rt_prio_array {
	DECLARE_BITMAP(bitmap, MAX_RT_PRIO+1); /* include 1 bit for delimiter */
	నిర్మాణం list_తల వరుస[MAX_RT_PRIO];
};

నిర్మాణం rt_bandwidth {
	/* nests inside the rq lock: */
	raw_spinlock_t		rt_runtime_తాళం;
	ktime_t			rt_period;
	u64			rt_runtime;
	నిర్మాణం hrtimer		rt_period_timer;
	గుర్తులేని పూర్ణాంకం		rt_period_చురుకైన;
};

స్థిర అంతఃవరుస పూర్ణాంకం dl_bandwidth_enabled(శూన్య)
{
	తిరిగి sysctl_sched_rt_runtime >= 0;
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
నిర్మాణం dl_bw {
	raw_spinlock_t		తాళం;
	u64			bw;
	u64			మొత్తం_bw;
};

బాహ్య శూన్య init_dl_bw(నిర్మాణం dl_bw *dl_b);
బాహ్య పూర్ణాంకం  sched_dl_global_validate(శూన్య);
బాహ్య శూన్య sched_dl_do_global(శూన్య);
బాహ్య పూర్ణాంకం  sched_dl_అధికప్రవాహం(నిర్మాణం task_struct *p, పూర్ణాంకం policy, స్థిరరాశి నిర్మాణం sched_attr *attr);
బాహ్య శూన్య __setparam_dl(నిర్మాణం task_struct *p, స్థిరరాశి నిర్మాణం sched_attr *attr);
బాహ్య శూన్య __getparam_dl(నిర్మాణం task_struct *p, నిర్మాణం sched_attr *attr, గుర్తులేని పూర్ణాంకం flags);
బాహ్య bool __checkparam_dl(స్థిరరాశి నిర్మాణం sched_attr *attr);
బాహ్య bool dl_param_మారింది(నిర్మాణం task_struct *p, స్థిరరాశి నిర్మాణం sched_attr *attr);
బాహ్య పూర్ణాంకం  dl_cpuset_cpumask_can_shrink(స్థిరరాశి నిర్మాణం cpumask *cur, స్థిరరాశి నిర్మాణం cpumask *trial);
బాహ్య పూర్ణాంకం  dl_bw_deactivate(పూర్ణాంకం cpu);
బాహ్య s64 dl_scaled_delta_exec(నిర్మాణం rq *rq, నిర్మాణం sched_dl_entity *dl_se, s64 delta_exec);
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
బాహ్య శూన్య dl_server_update_idle(నిర్మాణం sched_dl_entity *dl_se, s64 delta_exec);
బాహ్య శూన్య dl_server_update(నిర్మాణం sched_dl_entity *dl_se, s64 delta_exec);
బాహ్య శూన్య dl_server_ప్రారంభం(నిర్మాణం sched_dl_entity *dl_se);
బాహ్య శూన్య dl_server_stop(నిర్మాణం sched_dl_entity *dl_se);
బాహ్య శూన్య dl_server_init(నిర్మాణం sched_dl_entity *dl_se, నిర్మాణం rq *rq,
		    dl_server_pick_f pick_task);
బాహ్య శూన్య sched_init_dl_servers(శూన్య);

బాహ్య శూన్య fair_server_init(నిర్మాణం rq *rq);
బాహ్య శూన్య ext_server_init(నిర్మాణం rq *rq);
బాహ్య శూన్య __dl_server_attach_root(నిర్మాణం sched_dl_entity *dl_se, నిర్మాణం rq *rq);
బాహ్య పూర్ణాంకం dl_server_apply_params(నిర్మాణం sched_dl_entity *dl_se,
		    u64 runtime, u64 period, bool init);
బాహ్య పూర్ణాంకం dl_server_attach_bw(నిర్మాణం sched_dl_entity *dl_se);
బాహ్య శూన్య dl_server_detach_bw(నిర్మాణం sched_dl_entity *dl_se);
బాహ్య పూర్ణాంకం dl_server_మార్పిడి_bw(నిర్మాణం sched_dl_entity *detach_se,
			     నిర్మాణం sched_dl_entity *attach_se);

స్థిర అంతఃవరుస bool dl_server_చురుకైన(నిర్మాణం sched_dl_entity *dl_se)
{
	తిరిగి dl_se->dl_server_చురుకైన;
}

#నిర్వచితమైతే CONFIG_CGROUP_SCHED

బాహ్య నిర్మాణం list_తల task_groups;

#నిర్వచితమైతే CONFIG_సమూహం_SCHED_BANDWIDTH
బాహ్య స్థిరరాశి u64 max_bw_quota_period_us;

/*
 * default period for group bandwidth.
 * default: 0.1s, units: microseconds
 */
స్థిర అంతఃవరుస u64 default_bw_period_us(శూన్య)
{
	తిరిగి 100000ULL;
}
#అయితే_ముగింపు /* CONFIG_GROUP_SCHED_BANDWIDTH */

నిర్మాణం cfs_bandwidth {
#నిర్వచితమైతే CONFIG_CFS_BANDWIDTH
	raw_spinlock_t		తాళం;
	ktime_t			period;
	u64			quota;
	u64			runtime;
	u64			వ్యవధి;
	u64			runtime_snap;
	s64			క్రమానుగత_quota;

	u8			idle;
	u8			period_చురుకైన;
	u8			slack_started;
	నిర్మాణం hrtimer		period_timer;
	నిర్మాణం hrtimer		slack_timer;
	నిర్మాణం list_తల	throttled_cfs_rq;

	/* Statistics: */
	పూర్ణాంకం			nr_periods;
	పూర్ణాంకం			nr_throttled;
	పూర్ణాంకం			nr_వ్యవధి;
	u64			throttled_సమయం;
	u64			వ్యవధి_సమయం;
#అయితే_ముగింపు /* CONFIG_CFS_BANDWIDTH */
};

/* Task group related information */
నిర్మాణం task_సమూహం {
	నిర్మాణం cgroup_subsys_state css;

#నిర్వచితమైతే CONFIG_సమూహం_SCHED_బరువు
	/* A positive value indicates that this is a SCHED_IDLE group. */
	పూర్ణాంకం			idle;
#అయితే_ముగింపు

#నిర్వచితమైతే CONFIG_FAIR_సమూహం_SCHED
	/* runqueue "owned" by this group on each CPU */
	నిర్మాణం cfs_rq __percpu	*cfs_rq;
	గుర్తులేని దీర్ఘ		shares;
	/*
	 * load_avg can be heavily contended at clock tick time, so put
	 * it in its own cache-line separated from the fields above which
	 * will also be accessed at each tick.
	 */
	atomic_long_t		ఎక్కించు_avg ____cacheline_aligned;
	atomic_long_t		runnable_avg;

#అయితే_ముగింపు /* CONFIG_FAIR_GROUP_SCHED */

#నిర్వచితమైతే CONFIG_RT_సమూహం_SCHED
	నిర్మాణం sched_rt_entity	**rt_se;
	నిర్మాణం rt_rq		**rt_rq;

	నిర్మాణం rt_bandwidth	rt_bandwidth;
#అయితే_ముగింపు

	నిర్మాణం scx_task_సమూహం	scx;

	నిర్మాణం rcu_తల		rcu;
	నిర్మాణం list_తల	list;

	నిర్మాణం task_సమూహం	*మాతృ;
	నిర్మాణం list_తల	siblings;
	నిర్మాణం list_తల	children;

#నిర్వచితమైతే CONFIG_SCHED_AUTOGROUP
	నిర్మాణం autogroup	*autogroup;
#అయితే_ముగింపు

	నిర్మాణం cfs_bandwidth	cfs_bandwidth;

#నిర్వచితమైతే CONFIG_UCLAMP_TASK_సమూహం
	/* The two decimal precision [%] value requested from user-space */
	గుర్తులేని పూర్ణాంకం		uclamp_pct[UCLAMP_CNT];
	/* Clamp values requested for a task group */
	నిర్మాణం uclamp_se	uclamp_req[UCLAMP_CNT];
	/* Effective clamp values used for a task group */
	నిర్మాణం uclamp_se	uclamp[UCLAMP_CNT];
#అయితే_ముగింపు

};

#నిర్వచించు మూలం_TASK_సమూహం_ఎక్కించు	NICE_0_ఎక్కించు

రకనిర్వచనం పూర్ణాంకం (*tg_visitor)(నిర్మాణం task_సమూహం *, శూన్య *);

బాహ్య పూర్ణాంకం walk_tg_వృక్షం_from(నిర్మాణం task_సమూహం *from,
			     tg_visitor కిందికి, tg_visitor పైకి, శూన్య *దత్తాంశం);

/*
 * Iterate the full tree, calling @down when first entering a node and @up when
 * leaving it for the final time.
 *
 * Caller must hold rcu_lock or sufficient equivalent.
 */
స్థిర అంతఃవరుస పూర్ణాంకం walk_tg_వృక్షం(tg_visitor కిందికి, tg_visitor పైకి, శూన్య *దత్తాంశం)
{
	తిరిగి walk_tg_వృక్షం_from(&మూలం_task_సమూహం, కిందికి, పైకి, దత్తాంశం);
}

స్థిర అంతఃవరుస నిర్మాణం task_సమూహం *css_tg(నిర్మాణం cgroup_subsys_state *css)
{
	తిరిగి css ? container_యొక్క(css, నిర్మాణం task_సమూహం, css) : ఖాళీ;
}

బాహ్య పూర్ణాంకం tg_nop(నిర్మాణం task_సమూహం *tg, శూన్య *దత్తాంశం);

#నిర్వచితమైతే CONFIG_FAIR_సమూహం_SCHED
బాహ్య శూన్య free_fair_sched_సమూహం(నిర్మాణం task_సమూహం *tg);
బాహ్య పూర్ణాంకం alloc_fair_sched_సమూహం(నిర్మాణం task_సమూహం *tg, నిర్మాణం task_సమూహం *మాతృ);
బాహ్య శూన్య online_fair_sched_సమూహం(నిర్మాణం task_సమూహం *tg);
బాహ్య శూన్య unregister_fair_sched_సమూహం(నిర్మాణం task_సమూహం *tg);
బాహ్య శూన్య __sched_cgroup_mode_update(పూర్ణాంకం బహుళకం);
#లేకపోతే /* !CONFIG_FAIR_GROUP_SCHED: */
స్థిర అంతఃవరుస శూన్య free_fair_sched_సమూహం(నిర్మాణం task_సమూహం *tg) { }
స్థిర అంతఃవరుస పూర్ణాంకం alloc_fair_sched_సమూహం(నిర్మాణం task_సమూహం *tg, నిర్మాణం task_సమూహం *మాతృ)
{
       తిరిగి 1;
}
స్థిర అంతఃవరుస శూన్య online_fair_sched_సమూహం(నిర్మాణం task_సమూహం *tg) { }
స్థిర అంతఃవరుస శూన్య unregister_fair_sched_సమూహం(నిర్మాణం task_సమూహం *tg) { }
#అయితే_ముగింపు /* !CONFIG_FAIR_GROUP_SCHED */

బాహ్య శూన్య init_tg_cfs_నమోదు(నిర్మాణం task_సమూహం *tg, నిర్మాణం cfs_rq *cfs_rq,
			నిర్మాణం sched_entity *se, పూర్ణాంకం cpu,
			నిర్మాణం sched_entity *మాతృ);
బాహ్య శూన్య init_cfs_bandwidth(నిర్మాణం cfs_bandwidth *cfs_b, నిర్మాణం cfs_bandwidth *మాతృ);

బాహ్య శూన్య __refill_cfs_bandwidth_runtime(నిర్మాణం cfs_bandwidth *cfs_b);
బాహ్య శూన్య ప్రారంభం_cfs_bandwidth(నిర్మాణం cfs_bandwidth *cfs_b);
బాహ్య శూన్య unthrottle_cfs_rq(నిర్మాణం cfs_rq *cfs_rq);
బాహ్య bool cfs_task_bw_constrained(నిర్మాణం task_struct *p);

బాహ్య శూన్య init_tg_rt_నమోదు(నిర్మాణం task_సమూహం *tg, నిర్మాణం rt_rq *rt_rq,
		నిర్మాణం sched_rt_entity *rt_se, పూర్ణాంకం cpu,
		నిర్మాణం sched_rt_entity *మాతృ);
బాహ్య పూర్ణాంకం sched_సమూహం_సమితి_rt_runtime(నిర్మాణం task_సమూహం *tg, దీర్ఘ rt_runtime_us);
బాహ్య పూర్ణాంకం sched_సమూహం_సమితి_rt_period(నిర్మాణం task_సమూహం *tg, u64 rt_period_us);
బాహ్య దీర్ఘ sched_సమూహం_rt_runtime(నిర్మాణం task_సమూహం *tg);
బాహ్య దీర్ఘ sched_సమూహం_rt_period(నిర్మాణం task_సమూహం *tg);
బాహ్య పూర్ణాంకం sched_rt_can_attach(నిర్మాణం task_సమూహం *tg, నిర్మాణం task_struct *tsk);

బాహ్య నిర్మాణం task_సమూహం *sched_create_సమూహం(నిర్మాణం task_సమూహం *మాతృ);
బాహ్య శూన్య sched_online_సమూహం(నిర్మాణం task_సమూహం *tg,
			       నిర్మాణం task_సమూహం *మాతృ);
బాహ్య శూన్య sched_destroy_సమూహం(నిర్మాణం task_సమూహం *tg);
బాహ్య శూన్య sched_release_సమూహం(నిర్మాణం task_సమూహం *tg);

బాహ్య శూన్య sched_move_task(నిర్మాణం task_struct *tsk, bool for_autogroup);

#నిర్వచితమైతే CONFIG_FAIR_సమూహం_SCHED
బాహ్య పూర్ణాంకం sched_సమూహం_సమితి_shares(నిర్మాణం task_సమూహం *tg, గుర్తులేని దీర్ఘ shares);

బాహ్య పూర్ణాంకం sched_సమూహం_సమితి_idle(నిర్మాణం task_సమూహం *tg, దీర్ఘ idle);

బాహ్య శూన్య సమితి_task_rq_fair(నిర్మాణం sched_entity *se,
			     నిర్మాణం cfs_rq *prev, నిర్మాణం cfs_rq *తదుపరి);
#లేకపోతే /* !CONFIG_FAIR_GROUP_SCHED: */
స్థిర అంతఃవరుస పూర్ణాంకం sched_సమూహం_సమితి_shares(నిర్మాణం task_సమూహం *tg, గుర్తులేని దీర్ఘ shares) { తిరిగి 0; }
స్థిర అంతఃవరుస పూర్ణాంకం sched_సమూహం_సమితి_idle(నిర్మాణం task_సమూహం *tg, దీర్ఘ idle) { తిరిగి 0; }
#అయితే_ముగింపు /* !CONFIG_FAIR_GROUP_SCHED */

#లేకపోతే /* !CONFIG_CGROUP_SCHED: */

నిర్మాణం cfs_bandwidth { };

స్థిర అంతఃవరుస bool cfs_task_bw_constrained(నిర్మాణం task_struct *p) { తిరిగి false; }

#అయితే_ముగింపు /* !CONFIG_CGROUP_SCHED */

/*
 * A weight of 0 or 1 can cause arithmetics problems.
 * A weight of a cfs_rq is the sum of weights of which entities
 * are queued on this cfs_rq, so a weight of a entity should not be
 * too large, so as the shares value of a task group.
 * (The default weight is 1024 - so there's no practical
 *  limitation from this.)
 */
#నిర్వచించు MIN_SHARES		(1UL <<  1)
#నిర్వచించు MAX_SHARES		(1UL << 18)

బాహ్య శూన్య unregister_rt_sched_సమూహం(నిర్మాణం task_సమూహం *tg);
బాహ్య శూన్య free_rt_sched_సమూహం(నిర్మాణం task_సమూహం *tg);
బాహ్య పూర్ణాంకం alloc_rt_sched_సమూహం(నిర్మాణం task_సమూహం *tg, నిర్మాణం task_సమూహం *మాతృ);

/*
 * u64_u32_load/u64_u32_store
 *
 * Use a copy of a u64 value to protect against data race. This is only
 * applicable for 32-bits architectures.
 */
#నిర్వచితమైతే CONFIG_64_బిట్టు
# define u64_u32_ఎక్కించు_copy(var, copy)		var
# define u64_u32_store_copy(var, copy, val)	(var = val)
#లేకపోతే
# define u64_u32_ఎక్కించు_copy(var, copy)					\
({									\
	u64 __val, __val_copy;						\
	చేయి {								\
		__val_copy = copy;					\
		/*							\
		 * paired with u64_u32_store_copy(), ordering access	\
		 * to var and copy.					\
		 */							\
		smp_rmb();						\
		__val = var;						\
	} అయితే (__val != __val_copy);					\
	__val;								\
})
# define u64_u32_store_copy(var, copy, val)				\
చేయి {									\
	typeof(val) __val = (val);					\
	var = __val;							\
	/*								\
	 * paired with u64_u32_load_copy(), ordering access to var and	\
	 * copy.							\
	 */								\
	smp_wmb();							\
	copy = __val;							\
} అయితే (0)
#అయితే_ముగింపు
# define u64_u32_ఎక్కించు(var)		u64_u32_ఎక్కించు_copy(var, var##_copy)
# define u64_u32_store(var, val)	u64_u32_store_copy(var, var##_copy, val)

నిర్మాణం balance_callback {
	నిర్మాణం balance_callback *తదుపరి;
	శూన్య (*func)(నిర్మాణం rq *rq);
};

/* Fair scheduling SCHED_{NORMAL,BATCH,IDLE} related fields in a runqueue: */
నిర్మాణం cfs_rq {
	నిర్మాణం ఎక్కించు_బరువు	ఎక్కించు;
	గుర్తులేని పూర్ణాంకం		nr_queued;
	గుర్తులేని పూర్ణాంకం		h_nr_queued;		/* SCHED_{NORMAL,BATCH,IDLE} */
	గుర్తులేని పూర్ణాంకం		h_nr_runnable;		/* SCHED_{NORMAL,BATCH,IDLE} */
	గుర్తులేని పూర్ణాంకం		h_nr_idle;		/* SCHED_IDLE */

	s64			కూడిక_w_vruntime;
	u64			కూడిక_బరువు;
	u64			zero_vruntime;
	గుర్తులేని పూర్ణాంకం		కూడిక_జరుపు;

#నిర్వచితమైతే CONFIG_SCHED_CORE
	గుర్తులేని పూర్ణాంకం		forceidle_seq;
	u64			zero_vruntime_fi;
#అయితే_ముగింపు

	నిర్మాణం rb_మూలం_cached	tasks_timeline;

	/*
	 * 'curr' points to the currently running entity on this cfs_rq.
	 * It is set to NULL otherwise (i.e when none are currently running).
	 */
	నిర్మాణం sched_entity	*curr;
	నిర్మాణం sched_entity	*తదుపరి;

	/*
	 * CFS load tracking
	 */
	నిర్మాణం sched_entity	*h_curr;
	నిర్మాణం sched_avg	avg;
#నిర్వచితం_కాకపోతే CONFIG_64_బిట్టు
	u64			చివరి_update_సమయం_copy;
#అయితే_ముగింపు
	నిర్మాణం {
		raw_spinlock_t	తాళం ____cacheline_aligned;
		పూర్ణాంకం		nr;
		గుర్తులేని దీర్ఘ	ఎక్కించు_avg;
		గుర్తులేని దీర్ఘ	util_avg;
		గుర్తులేని దీర్ఘ	runnable_avg;
	} removed;

#నిర్వచితమైతే CONFIG_FAIR_సమూహం_SCHED
	u64			చివరి_update_tg_ఎక్కించు_avg;
	గుర్తులేని దీర్ఘ		tg_ఎక్కించు_avg_contrib;
	గుర్తులేని దీర్ఘ		tg_runnable_avg_contrib;
	దీర్ఘ			propagate;
	దీర్ఘ			prop_runnable_కూడిక;

	/*
	 *   h_load = weight * f(tg)
	 *
	 * Where f(tg) is the recursive weight fraction assigned to
	 * this group.
	 */
	గుర్తులేని దీర్ఘ		h_ఎక్కించు;
	u64			చివరి_h_ఎక్కించు_update;
	నిర్మాణం sched_entity	*h_ఎక్కించు_తదుపరి;

	నిర్మాణం rq		*rq;	/* CPU runqueue to which this cfs_rq is attached */

	/*
	 * leaf cfs_rqs are those that hold tasks (lowest schedulable entity in
	 * a hierarchy). Non-leaf lrqs hold other higher schedulable entities
	 * (like users, containers etc.)
	 *
	 * leaf_cfs_rq_list ties together list of leaf cfs_rq's in a CPU.
	 * This list is used during load balance.
	 */
	పూర్ణాంకం			on_list;
	నిర్మాణం list_తల	ఆకు_cfs_rq_list;
	నిర్మాణం task_సమూహం	*tg;	/* Group that "owns" this runqueue */

	/* Locally cached copy of our task_group's idle value */
	పూర్ణాంకం			idle;

# ifdef CONFIG_CFS_BANDWIDTH
	పూర్ణాంకం			runtime_enabled;
	s64			runtime_మిగిలిన;

	u64			throttled_pelt_idle;
#  ifndef CONFIG_64_బిట్టు
	u64                     throttled_pelt_idle_copy;
#  endif
	u64			throttled_clock;
	u64			throttled_clock_pelt;
	u64			throttled_clock_pelt_సమయం;
	u64			throttled_clock_self;
	u64			throttled_clock_self_సమయం;
	bool			throttled:1;
	bool			pelt_clock_throttled:1;
	పూర్ణాంకం			throttle_లెక్క;
	నిర్మాణం list_తల	throttled_list;
	నిర్మాణం list_తల	throttled_csd_list;
	నిర్మాణం list_తల        throttled_limbo_list;
# endif /* CONFIG_CFS_BANDWIDTH */
#అయితే_ముగింపు /* CONFIG_FAIR_GROUP_SCHED */
};

#నిర్వచితమైతే CONFIG_SCHED_CLASS_EXT
/* scx_rq->flags, protected by the rq lock */
గణన scx_rq_flags {
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
	SCX_RQ_మూలం_IDLE_RENOTIFY	= 1 << 8, /* the root is owed update_idle() */

	SCX_RQ_లోపల_WAKEUP	= 1 << 16,
	SCX_RQ_లోపల_DISPATCH	= 1 << 17,
};

/* per-rq rescue execution state, see scx_rescue_timerfn() */
నిర్మాణం scx_rq_rescue {
	నిర్మాణం scx_dispatch_q	dsq;			/* stranded tasks awaiting rescue */
	s64			budget;			/* execution token bucket, ns */
	u64			clock;			/* last budget accrual timestamp */
	నిర్మాణం task_struct	*curr;			/* task being rescued, one at a time */
	s64			ముక్క;			/* curr's admitted slice */
	u64			exec_snap;		/* sum_exec_runtime at admission */
	నిర్మాణం timer_list	timer;			/* paces admission and escalation */
	u64			kill_at;		/* last ejection, init before any */
};

నిర్మాణం scx_rq {
	నిర్మాణం scx_dispatch_q	local_dsq;
#నిర్వచితమైతే CONFIG_EXT_SUB_SCHED
	నిర్మాణం scx_dispatch_q	reject_dsq;		/* staging for cap-rejected tasks */
	నిర్మాణం scx_rq_rescue	rescue;
#అయితే_ముగింపు
	నిర్మాణం list_తల	runnable_list;		/* runnable tasks on this rq */
	నిర్మాణం list_తల	ddsp_deferred_locals;	/* deferred ddsps from enq */
	/* both stashed across the activate_task() in move_remote_task_to_local_dsq() */
	u64			remote_activate_enq_flags;
	నిర్మాణం scx_sched	*remote_activate_sch;
	u32			nr_running;
	u32			cpuperf_లక్ష్యం;		/* [0, SCHED_CAPACITY_SCALE] */
	bool			లోపల_select_cpu;
	bool			cpu_released;
	u32			flags;
	u32			nr_immed;		/* ENQ_IMMED tasks on local_dsq */
#నిర్వచితమైతే CONFIG_SCHED_CORE
	u32			తాళం_drop_seq;	/* nr dispatch lock releases */
#అయితే_ముగింపు
	u64			clock;			/* current per-rq clock -- see scx_bpf_now() */
#నిర్వచితమైతే CONFIG_EXT_SUB_SCHED
	నిర్మాణం llist_తల	ecaps_గమ్యం_sync;		/* pending ecaps syncs */
	నిర్మాణం task_struct	*sub_dispatch_prev;
#అయితే_ముగింపు
	cpumask_var_t		cpus_గమ్యం_sync;
	bool			kick_sync_pending;
	గుర్తులేని దీర్ఘ		kick_sync;

	నిర్మాణం list_తల	sched_pcpus_గమ్యం_kick;	/* see kick_cpus_irq_workfn() */

	raw_spinlock_t		deferred_reenq_తాళం;
	నిర్మాణం list_తల	deferred_reenq_locals;	/* scheds requesting reenq of local DSQ */
	నిర్మాణం list_తల	deferred_reenq_users;	/* user DSQs requesting reenq */
	నిర్మాణం balance_callback	deferred_bal_cb;
	నిర్మాణం balance_callback	kick_sync_bal_cb;
	నిర్మాణం irq_work		deferred_irq_work;
	నిర్మాణం irq_work		kick_cpus_irq_work;
};
#అయితే_ముగింపు /* CONFIG_SCHED_CLASS_EXT */

స్థిర అంతఃవరుస పూర్ణాంకం rt_bandwidth_enabled(శూన్య)
{
	తిరిగి sysctl_sched_rt_runtime >= 0;
}

/* RT IPI pull logic requires IRQ_WORK */
#ఒకవేళ defined(CONFIG_IRQ_WORK) && defined(CONFIG_SMP)
# define HAVE_RT_నెట్టు_IPI
#అయితే_ముగింపు

/* Real-Time classes' related field in a runqueue: */
నిర్మాణం rt_rq {
	నిర్మాణం rt_prio_array	చురుకైన;
	గుర్తులేని పూర్ణాంకం		rt_nr_running;
	గుర్తులేని పూర్ణాంకం		rr_nr_running;
	నిర్మాణం {
		పూర్ణాంకం		curr; /* highest queued rt task prio */
		పూర్ణాంకం		తదుపరి; /* next highest */
	} highest_prio;
	bool			overloaded;
	నిర్మాణం plist_తల	pushable_tasks;

	పూర్ణాంకం			rt_queued;

#నిర్వచితమైతే CONFIG_RT_సమూహం_SCHED
	పూర్ణాంకం			rt_throttled;
	u64			rt_సమయం; /* consumed RT time, goes up in update_curr_rt */
	u64			rt_runtime; /* allotted RT time, "slice" from rt_bandwidth, RT sharing/balancing */
	/* Nests inside the rq lock: */
	raw_spinlock_t		rt_runtime_తాళం;

	గుర్తులేని పూర్ణాంకం		rt_nr_boosted;

	నిర్మాణం rq		*rq; /* this is always top-level rq, cache? */
#అయితే_ముగింపు
#నిర్వచితమైతే CONFIG_CGROUP_SCHED
	నిర్మాణం task_సమూహం	*tg; /* this tg has "this" rt_rq on given CPU for runnable entities */
#అయితే_ముగింపు
};

స్థిర అంతఃవరుస bool rt_rq_is_runnable(నిర్మాణం rt_rq *rt_rq)
{
	తిరిగి rt_rq->rt_queued && rt_rq->rt_nr_running;
}

/* Deadline class' related fields in a runqueue */
నిర్మాణం dl_rq {
	/* runqueue is an rbtree, ordered by deadline */
	నిర్మాణం rb_మూలం_cached	మూలం;

	గుర్తులేని పూర్ణాంకం		dl_nr_running;

	/*
	 * Deadline values of the currently executing and the
	 * earliest ready task on this rq. Caching these facilitates
	 * the decision whether or not a ready but not running task
	 * should migrate somewhere else.
	 */
	నిర్మాణం {
		u64		curr;
		u64		తదుపరి;
	} earliest_dl;

	bool			overloaded;

	నిర్మాణం sched_dl_entity	*curr;
	/*
	 * Tasks on this rq that can be pushed away. They are kept in
	 * an rb-tree, ordered by tasks' deadlines, with caching
	 * of the leftmost (earliest deadline) element.
	 */
	నిర్మాణం rb_మూలం_cached	pushable_dl_tasks_మూలం;

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
	u64			bw_నిష్పత్తి;
};

#నిర్వచితమైతే CONFIG_FAIR_సమూహం_SCHED
/* Check whether a task group is root tg */
#నిర్వచించు is_మూలం_task_సమూహం(tg) ((tg) == &మూలం_task_సమూహం)
/* An entity is a task if it doesn't "own" a runqueue */
#నిర్వచించు entity_is_task(se)	(!se->my_q)

స్థిర అంతఃవరుస శూన్య se_update_runnable(నిర్మాణం sched_entity *se)
{
	ఒకవేళ (!entity_is_task(se))
		se->runnable_బరువు = se->my_q->h_nr_runnable;
}

స్థిర అంతఃవరుస దీర్ఘ se_runnable(నిర్మాణం sched_entity *se)
{
	ఒకవేళ (se->sched_delayed)
		తిరిగి false;

	ఒకవేళ (entity_is_task(se))
		తిరిగి !!se->on_rq;
	లేకపోతే
		తిరిగి se->runnable_బరువు;
}

#లేకపోతే /* !CONFIG_FAIR_GROUP_SCHED: */

#నిర్వచించు entity_is_task(se)	1

స్థిర అంతఃవరుస శూన్య se_update_runnable(నిర్మాణం sched_entity *se) { }

స్థిర అంతఃవరుస దీర్ఘ se_runnable(నిర్మాణం sched_entity *se)
{
	ఒకవేళ (se->sched_delayed)
		తిరిగి false;

	తిరిగి !!se->on_rq;
}

#అయితే_ముగింపు /* !CONFIG_FAIR_GROUP_SCHED */

/*
 * XXX we want to get rid of these helpers and use the full load resolution.
 */
స్థిర అంతఃవరుస దీర్ఘ se_బరువు(నిర్మాణం sched_entity *se)
{
	తిరిగి scale_ఎక్కించు_కిందికి(se->ఎక్కించు.బరువు);
}


స్థిర అంతఃవరుస bool sched_asym_prefer(పూర్ణాంకం a, పూర్ణాంకం b)
{
	తిరిగి arch_asym_cpu_ప్రాధాన్యత(a) > arch_asym_cpu_ప్రాధాన్యత(b);
}

నిర్మాణం perf_domain {
	నిర్మాణం em_perf_domain *em_pd;
	నిర్మాణం perf_domain *తదుపరి;
	నిర్మాణం rcu_తల rcu;
};

/*
 * We add the notion of a root-domain which will be used to define per-domain
 * variables. Each exclusive cpuset essentially defines an island domain by
 * fully partitioning the member CPUs from any other cpuset. Whenever a new
 * exclusive cpuset is created, we also create and attach a new root-domain
 * object.
 *
 */
నిర్మాణం మూలం_domain {
	atomic_t		refcount;
	atomic_t		rto_లెక్క;
	నిర్మాణం rcu_తల		rcu;
	cpumask_var_t		పరిధి;
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
	cpumask_var_t		dlo_ముసుగు;
	atomic_t		dlo_లెక్క;
	నిర్మాణం dl_bw		dl_bw;
	నిర్మాణం cpudl		cpudl;

	/*
	 * Indicate whether a root_domain's dl_bw has been checked or
	 * updated. It's monotonously increasing value.
	 *
	 * Also, some corner cases, like 'wrap around' is dangerous, but given
	 * that u64 is 'big enough'. So that shouldn't be a concern.
	 */
	u64 సందర్శించు_cookie;

#నిర్వచితమైతే HAVE_RT_నెట్టు_IPI
	/*
	 * For IPI pull requests, loop across the rto_mask.
	 */
	నిర్మాణం irq_work		rto_నెట్టు_work;
	raw_spinlock_t		rto_తాళం;
	/* These are only updated and read within rto_lock */
	పూర్ణాంకం			rto_loop;
	పూర్ణాంకం			rto_cpu;
	/* These atomics are updated outside of a lock */
	atomic_t		rto_loop_తదుపరి;
	atomic_t		rto_loop_ప్రారంభం;
#అయితే_ముగింపు /* HAVE_RT_PUSH_IPI */
	/*
	 * The "RT overload" flag: it gets set if a CPU has more than
	 * one runnable RT task.
	 */
	cpumask_var_t		rto_ముసుగు;
	నిర్మాణం cpupri		cpupri;

	/*
	 * NULL-terminated list of performance domains intersecting with the
	 * CPUs of the rd. Protected by RCU.
	 */
	నిర్మాణం perf_domain __rcu *pd;
};

బాహ్య శూన్య init_defrootdomain(శూన్య);
బాహ్య పూర్ణాంకం sched_init_domains(స్థిరరాశి నిర్మాణం cpumask *cpu_map);
బాహ్య శూన్య rq_attach_మూలం(నిర్మాణం rq *rq, నిర్మాణం మూలం_domain *rd);
బాహ్య శూన్య sched_get_rd(నిర్మాణం మూలం_domain *rd);
బాహ్య శూన్య sched_put_rd(నిర్మాణం మూలం_domain *rd);

స్థిర అంతఃవరుస పూర్ణాంకం get_rd_overloaded(నిర్మాణం మూలం_domain *rd)
{
	తిరిగి READ_ONCE(rd->overloaded);
}

స్థిర అంతఃవరుస శూన్య సమితి_rd_overloaded(నిర్మాణం మూలం_domain *rd, పూర్ణాంకం స్థితి_2)
{
	ఒకవేళ (get_rd_overloaded(rd) != స్థితి_2)
		WRITE_ONCE(rd->overloaded, స్థితి_2);
}

#నిర్వచితమైతే HAVE_RT_నెట్టు_IPI
బాహ్య శూన్య rto_నెట్టు_irq_work_func(నిర్మాణం irq_work *work);
#అయితే_ముగింపు

#నిర్వచితమైతే CONFIG_UCLAMP_TASK
/*
 * struct uclamp_bucket - Utilization clamp bucket
 * @value: utilization clamp value for tasks on this clamp bucket
 * @tasks: number of RUNNABLE tasks on this clamp bucket
 *
 * Keep track of how many tasks are RUNNABLE for a given utilization
 * clamp value.
 */
నిర్మాణం uclamp_పాత్ర {
	గుర్తులేని దీర్ఘ విలువ : బిట్లు_చొప్పున(SCHED_సామర్థ్యం_SCALE);
	గుర్తులేని దీర్ఘ tasks : బిట్లు_చొప్పున_LONG - బిట్లు_చొప్పున(SCHED_సామర్థ్యం_SCALE);
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
నిర్మాణం uclamp_rq {
	గుర్తులేని పూర్ణాంకం విలువ;
	నిర్మాణం uclamp_పాత్ర పాత్ర[UCLAMP_BUCKETS];
};

DECLARE_STATIC_కీలకం_FALSE(sched_uclamp_వాడిన);
#అయితే_ముగింపు /* CONFIG_UCLAMP_TASK */

/*
 * This is the main, per-CPU runqueue data structure.
 *
 * Locking rule: those places that want to lock multiple runqueues
 * (such as the load balancing or the thread migration code), lock
 * acquire operations must be ordered by ascending &runqueue.
 */
నిర్మాణం rq {
	/*
	 * The following members are loaded together, without holding the
	 * rq->lock, in an extremely hot loop in update_sg_lb_stats()
	 * (called from pick_next_task()). To reduce cache pollution from
	 * this operation, they are placed together on this dedicated cache
	 * line. Even though some of them are frequently modified, they are
	 * loaded much more frequently than they are stored.
	 */
	గుర్తులేని పూర్ణాంకం		nr_running;
#నిర్వచితమైతే CONFIG_NUMA_BALANCING
	గుర్తులేని పూర్ణాంకం		nr_numa_running;
	గుర్తులేని పూర్ణాంకం		nr_preferred_running;
#అయితే_ముగింపు
	గుర్తులేని పూర్ణాంకం		ttwu_pending;
	గుర్తులేని దీర్ఘ		cpu_సామర్థ్యం;
#నిర్వచితమైతే CONFIG_SCHED_PROXY_EXEC
	నిర్మాణం task_struct __rcu	*donor;  /* Scheduling context */
	నిర్మాణం task_struct __rcu	*curr;   /* Execution context */
#లేకపోతే
	సంఘం {
		నిర్మాణం task_struct __rcu *donor; /* Scheduler context */
		నిర్మాణం task_struct __rcu *curr;  /* Execution context */
	};
#అయితే_ముగింపు
	నిర్మాణం task_struct	*idle;
	/* padding left here deliberately */

	/*
	 * The next cacheline holds the (hot) runqueue lock, as well as
	 * some other less performance-critical fields.
	 */
	u64			nr_switches	____cacheline_aligned;

	/* runqueue lock: */
	raw_spinlock_t		__lock;

#నిర్వచితమైతే CONFIG_NO_HZ_COMMON
	గుర్తులేని పూర్ణాంకం		nohz_tick_stopped;
	atomic_t		nohz_flags;
	గుర్తులేని పూర్ణాంకం		has_blocked_ఎక్కించు;
	గుర్తులేని దీర్ఘ		చివరి_blocked_ఎక్కించు_update_tick;
	call_single_దత్తాంశం_t	nohz_csd;
#అయితే_ముగింపు /* CONFIG_NO_HZ_COMMON */

#నిర్వచితమైతే CONFIG_UCLAMP_TASK
	/* Utilization clamp values based on CPU's RUNNABLE tasks */
	నిర్మాణం uclamp_rq	uclamp[UCLAMP_CNT] ____cacheline_aligned;
	గుర్తులేని పూర్ణాంకం		uclamp_flags;
#నిర్వచించు UCLAMP_FLAG_IDLE 0x01
#అయితే_ముగింపు

	నిర్మాణం cfs_rq		cfs;
	నిర్మాణం rt_rq		rt;
	నిర్మాణం dl_rq		dl;
#నిర్వచితమైతే CONFIG_SCHED_CLASS_EXT
	నిర్మాణం scx_rq		scx;
	నిర్మాణం sched_dl_entity	ext_server;
#అయితే_ముగింపు
#నిర్వచితమైతే CONFIG_SCHED_CACHE
	raw_spinlock_t		cpu_epoch_తాళం ____cacheline_aligned;
	u64			cpu_runtime;
	గుర్తులేని దీర్ఘ		cpu_epoch;
	గుర్తులేని దీర్ఘ		cpu_epoch_తదుపరి;
#అయితే_ముగింపు

	నిర్మాణం sched_dl_entity	fair_server;

#నిర్వచితమైతే CONFIG_FAIR_సమూహం_SCHED
	/* list of leaf cfs_rq on this CPU: */
	నిర్మాణం list_తల	ఆకు_cfs_rq_list;
	నిర్మాణం list_తల	*tmp_alone_branch;
#అయితే_ముగింపు /* CONFIG_FAIR_GROUP_SCHED */

#నిర్వచితమైతే CONFIG_NUMA_BALANCING
	గుర్తులేని పూర్ణాంకం		numa_migrate_on;
#అయితే_ముగింపు

#నిర్వచితమైతే CONFIG_SCHED_CACHE
	గుర్తులేని పూర్ణాంకం		nr_pref_llc_running;
	గుర్తులేని పూర్ణాంకం		nr_llc_running;
#అయితే_ముగింపు

	/*
	 * This is part of a global counter where only the total sum
	 * over all CPUs matters. A task can increase this counter on
	 * one CPU and if it got migrated afterwards it may decrease
	 * it on another CPU. Always updated under the runqueue lock:
	 */
	గుర్తులేని దీర్ఘ		nr_uninterruptible;

	నిర్మాణం sched_dl_entity	*dl_server;
	నిర్మాణం task_struct	*stop;
	స్థిరరాశి నిర్మాణం sched_class *తదుపరి_class;
	గుర్తులేని దీర్ఘ		తదుపరి_balance;
	నిర్మాణం mm_struct	*prev_mm;

	/*
	 * The following fields of clock data are frequently referenced
	 * and updated together, and should go on their own cache line.
	 */
	u64			clock_task ____cacheline_aligned;
	u64			clock_pelt;
	u64			clock;
	గుర్తులేని దీర్ఘ		కోల్పోయిన_idle_సమయం;
	గుర్తులేని పూర్ణాంకం		clock_update_flags;
	u64			clock_pelt_idle;
	u64			clock_idle;

#నిర్వచితం_కాకపోతే CONFIG_64_బిట్టు
	u64			clock_pelt_idle_copy;
	u64			clock_idle_copy;
#అయితే_ముగింపు

	u64 చివరి_చూసిన_need_resched_ns;
	పూర్ణాంకం ticks_without_resched;

#నిర్వచితమైతే CONFIG_MEMBARRIER
	పూర్ణాంకం membarrier_state;
#అయితే_ముగింపు

	నిర్మాణం మూలం_domain		*rd;
	నిర్మాణం sched_domain __rcu	*sd;

	నిర్మాణం balance_callback *balance_callback;

	గుర్తులేని అక్షరం		nohz_idle_balance;
	గుర్తులేని అక్షరం		idle_balance;

	గుర్తులేని దీర్ఘ		misfit_task_ఎక్కించు;

	/* For active balancing */
	పూర్ణాంకం			చురుకైన_balance;
	పూర్ణాంకం			నెట్టు_cpu;
	నిర్మాణం cpu_stop_work	చురుకైన_balance_work;

	/* CPU of this runqueue: */
	పూర్ణాంకం			cpu;
	పూర్ణాంకం			online;

	నిర్మాణం list_తల cfs_tasks;

	నిర్మాణం sched_avg	avg_rt;
	నిర్మాణం sched_avg	avg_dl;
#నిర్వచితమైతే CONFIG_HAVE_SCHED_AVG_IRQ
	నిర్మాణం sched_avg	avg_irq;
#అయితే_ముగింపు
#నిర్వచితమైతే CONFIG_SCHED_HW_PRESSURE
	నిర్మాణం sched_avg	avg_hw;
#అయితే_ముగింపు
	u64			idle_stamp;
	u64			avg_idle;

	/* This is used to determine avg_idle's max value */
	u64			max_idle_balance_ఖర్చు;

#నిర్వచితమైతే CONFIG_HOTPLUG_CPU
	నిర్మాణం rcuwait		hotplug_wait;
#అయితే_ముగింపు

#నిర్వచితమైతే CONFIG_IRQ_సమయం_ACCOUNTING
	u64			prev_irq_సమయం;
	u64			psi_irq_సమయం;
#అయితే_ముగింపు
#నిర్వచితమైతే CONFIG_PARAVIRT
	u64			prev_steal_సమయం;
#అయితే_ముగింపు
#నిర్వచితమైతే CONFIG_PARAVIRT_సమయం_ACCOUNTING
	u64			prev_steal_సమయం_rq;
#అయితే_ముగింపు
#నిర్వచితమైతే CONFIG_PREFERRED_CPU
	bool			npc_నెట్టు_work_pending;
#అయితే_ముగింపు

	/* calc_load related fields */
	గుర్తులేని దీర్ఘ		calc_ఎక్కించు_update;
	దీర్ఘ			calc_ఎక్కించు_చురుకైన;

#నిర్వచితమైతే CONFIG_SCHED_HRTICK
	call_single_దత్తాంశం_t	hrtick_csd;
	నిర్మాణం hrtimer		hrtick_timer;
	ktime_t			hrtick_సమయం;
	ktime_t			hrtick_delay;
	గుర్తులేని పూర్ణాంకం		hrtick_sched;
#అయితే_ముగింపు

#నిర్వచితమైతే CONFIG_SCHEDSTATS
	/* latency stats */
	నిర్మాణం sched_info	rq_sched_info;
	గుర్తులేని దీర్ఘ దీర్ఘ	rq_cpu_సమయం;

	/* sys_sched_yield() stats */
	గుర్తులేని పూర్ణాంకం		yld_లెక్క;

	/* schedule() stats */
	గుర్తులేని పూర్ణాంకం		sched_లెక్క;
	గుర్తులేని పూర్ణాంకం		sched_goidle;

	/* try_to_wake_up() stats */
	గుర్తులేని పూర్ణాంకం		ttwu_లెక్క;
	గుర్తులేని పూర్ణాంకం		ttwu_local;
#అయితే_ముగింపు

#నిర్వచితమైతే CONFIG_CPU_IDLE
	/* Must be inspected within a RCU lock section */
	నిర్మాణం cpuidle_state	*idle_state;
#అయితే_ముగింపు

	గుర్తులేని పూర్ణాంకం		nr_pinned;
	గుర్తులేని పూర్ణాంకం		నెట్టు_busy;
	నిర్మాణం cpu_stop_work	నెట్టు_work;

#నిర్వచితమైతే CONFIG_SCHED_CORE
	/* per rq */
	నిర్మాణం rq		*core;
	నిర్మాణం task_struct	*core_pick;
	నిర్మాణం sched_dl_entity	*core_dl_server;
	గుర్తులేని పూర్ణాంకం		core_enabled;
	నిర్మాణం rb_మూలం		core_వృక్షం;

	/* shared state -- careful with sched_core_cpu_deactivate() */
	గుర్తులేని పూర్ణాంకం		core_task_seq;
	గుర్తులేని పూర్ణాంకం		core_pick_seq;
	గుర్తులేని దీర్ఘ		core_cookie;
	గుర్తులేని పూర్ణాంకం		core_forceidle_లెక్క;
	గుర్తులేని పూర్ణాంకం		core_forceidle_seq;
	గుర్తులేని పూర్ణాంకం		core_forceidle_occupation;
	u64			core_forceidle_ప్రారంభం;
	గుర్తులేని పూర్ణాంకం		core_pick_లోపల_flight;
#అయితే_ముగింపు /* CONFIG_SCHED_CORE */

	/* Scratch cpumask to be temporarily used under rq_lock */
	cpumask_var_t		scratch_ముసుగు;

#నిర్వచితమైతే CONFIG_CFS_BANDWIDTH
	call_single_దత్తాంశం_t	cfsb_csd;
	నిర్మాణం list_తల	cfsb_csd_list;
#అయితే_ముగింపు

	atomic_t		nr_iowait;
} __no_randomize_layout;

#నిర్వచితమైతే CONFIG_FAIR_సమూహం_SCHED

/* CPU runqueue to which this cfs_rq is attached */
స్థిర అంతఃవరుస నిర్మాణం rq *rq_యొక్క(నిర్మాణం cfs_rq *cfs_rq)
{
	తిరిగి cfs_rq->rq;
}

#లేకపోతే /* !CONFIG_FAIR_GROUP_SCHED: */

స్థిర అంతఃవరుస నిర్మాణం rq *rq_యొక్క(నిర్మాణం cfs_rq *cfs_rq)
{
	తిరిగి container_యొక్క(cfs_rq, నిర్మాణం rq, cfs);
}
#అయితే_ముగింపు /* !CONFIG_FAIR_GROUP_SCHED */

స్థిర అంతఃవరుస పూర్ణాంకం cpu_యొక్క(నిర్మాణం rq *rq)
{
	తిరిగి rq->cpu;
}

#నిర్వచించు MDF_నెట్టు		0x01

స్థిర అంతఃవరుస bool is_migration_disabled(నిర్మాణం task_struct *p)
{
	తిరిగి p->migration_disabled;
}

DECLARE_చొప్పున_CPU_SHARED_ALIGNED(నిర్మాణం rq, runqueues);
DECLARE_చొప్పున_CPU(నిర్మాణం rnd_state, sched_rnd_state);

స్థిర అంతఃవరుస u32 sched_rng(శూన్య)
{
	తిరిగి prandom_u32_state(this_cpu_ptr(&sched_rnd_state));
}

స్థిర __always_inline నిర్మాణం rq *__this_rq(శూన్య)
{
	తిరిగి this_cpu_ptr(&runqueues);
}

#నిర్వచించు cpu_rq(cpu)		(&చొప్పున_cpu(runqueues, (cpu)))
#నిర్వచించు this_rq()		__this_rq()
#నిర్వచించు task_rq(p)		cpu_rq(task_cpu(p))
#నిర్వచించు cpu_curr(cpu)		(cpu_rq(cpu)->curr)
#నిర్వచించు raw_rq()		raw_cpu_ptr(&runqueues)

స్థిర అంతఃవరుస bool idle_rq(నిర్మాణం rq *rq)
{
	తిరిగి rq->curr == rq->idle && !rq->nr_running && !rq->ttwu_pending;
}

/**
 * available_idle_cpu - is a given CPU idle for enqueuing work.
 * @cpu: the CPU in question.
 *
 * Return: 1 if the CPU is currently idle. 0 otherwise.
 */
స్థిర అంతఃవరుస bool అందుబాటు_idle_cpu(పూర్ణాంకం cpu)
{
	ఒకవేళ (!idle_rq(cpu_rq(cpu)))
		తిరిగి 0;

	ఒకవేళ (vcpu_is_preempted(cpu))
		తిరిగి 0;

	తిరిగి 1;
}

#నిర్వచితమైతే CONFIG_SCHED_PROXY_EXEC
స్థిర అంతఃవరుస శూన్య rq_సమితి_donor(నిర్మాణం rq *rq, నిర్మాణం task_struct *t)
{
	rcu_కేటాయించు_సూచిక(rq->donor, t);
}
#లేకపోతే
స్థిర అంతఃవరుస శూన్య rq_సమితి_donor(నిర్మాణం rq *rq, నిర్మాణం task_struct *t)
{
	/* Do nothing */
}
#అయితే_ముగింపు

#నిర్వచితమైతే CONFIG_SCHED_CORE
స్థిర అంతఃవరుస నిర్మాణం cpumask *sched_సమూహం_పరిధి(నిర్మాణం sched_సమూహం *sg);

DECLARE_STATIC_కీలకం_FALSE(__sched_core_enabled);

స్థిర అంతఃవరుస bool sched_core_enabled(నిర్మాణం rq *rq)
{
	తిరిగి static_branch_unlikely(&__sched_core_enabled) && rq->core_enabled;
}

స్థిర అంతఃవరుస bool sched_core_disabled(శూన్య)
{
	తిరిగి !static_branch_unlikely(&__sched_core_enabled);
}

/*
 * Be careful with this function; not for general use. The return value isn't
 * stable unless you actually hold a relevant rq->__lock.
 */
స్థిర అంతఃవరుస raw_spinlock_t *rq_lockp(నిర్మాణం rq *rq)
{
	ఒకవేళ (sched_core_enabled(rq))
		తిరిగి &rq->core->__lock;

	తిరిగి &rq->__lock;
}

స్థిర అంతఃవరుస raw_spinlock_t *__rq_lockp(నిర్మాణం rq *rq)
	__returns_ctx_lock(rq_lockp(rq)) /* alias them */
{
	ఒకవేళ (rq->core_enabled)
		తిరిగి &rq->core->__lock;

	తిరిగి &rq->__lock;
}

బాహ్య bool
cfs_prio_less(స్థిరరాశి నిర్మాణం task_struct *a, స్థిరరాశి నిర్మాణం task_struct *b, bool fi);

బాహ్య శూన్య task_vruntime_update(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, bool లోపల_fi);

/*
 * Helpers to check if the CPU's core cookie matches with the task's cookie
 * when core scheduling is enabled.
 * A special case is that the task's cookie always matches with CPU's core
 * cookie if the CPU is in an idle core.
 */
స్థిర అంతఃవరుస bool sched_cpu_cookie_సరిపోలిక(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	ఒకవేళ (!sched_core_enabled(rq))
		తిరిగి true;

	తిరిగి rq->core->core_cookie == p->core_cookie;
}

స్థిర అంతఃవరుస bool sched_core_cookie_సరిపోలిక(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
	bool idle_core = true;
	పూర్ణాంకం cpu;

	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	ఒకవేళ (!sched_core_enabled(rq))
		తిరిగి true;

	ఒకవేళ (rq->core->core_cookie == p->core_cookie)
		తిరిగి true;

	for_each_cpu(cpu, cpu_smt_ముసుగు(cpu_యొక్క(rq))) {
		ఒకవేళ (!అందుబాటు_idle_cpu(cpu)) {
			idle_core = false;
			ఆపు;
		}
	}

	/*
	 * A CPU in an idle core is always the best choice for tasks with
	 * cookies.
	 */
	తిరిగి idle_core;
}

స్థిర అంతఃవరుస bool sched_సమూహం_cookie_సరిపోలిక(నిర్మాణం rq *rq,
					    నిర్మాణం task_struct *p,
					    నిర్మాణం sched_సమూహం *సమూహం)
{
	పూర్ణాంకం cpu;

	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	ఒకవేళ (!sched_core_enabled(rq))
		తిరిగి true;

	for_each_cpu_and(cpu, sched_సమూహం_పరిధి(సమూహం), p->cpus_ptr) {
		ఒకవేళ (sched_core_cookie_సరిపోలిక(cpu_rq(cpu), p))
			తిరిగి true;
	}
	తిరిగి false;
}

స్థిర అంతఃవరుస bool sched_core_enqueued(నిర్మాణం task_struct *p)
{
	తిరిగి !RB_ఖాళీలు_కణుపు(&p->core_కణుపు);
}

బాహ్య శూన్య sched_core_వరుసలో_చేర్చు(నిర్మాణం rq *rq, నిర్మాణం task_struct *p);
బాహ్య శూన్య sched_core_వరుస_నుండి_తీయి(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, పూర్ణాంకం flags);

బాహ్య శూన్య sched_core_get(శూన్య);
బాహ్య శూన్య sched_core_put(శూన్య);

స్థిర అంతఃవరుస bool task_has_sched_core(నిర్మాణం task_struct *p)
{
	ఒకవేళ (sched_core_disabled())
		తిరిగి false;

	తిరిగి !!p->core_cookie;
}

#లేకపోతే /* !CONFIG_SCHED_CORE: */

స్థిర అంతఃవరుస bool sched_core_enabled(నిర్మాణం rq *rq)
{
	తిరిగి false;
}

స్థిర అంతఃవరుస bool sched_core_disabled(శూన్య)
{
	తిరిగి true;
}

స్థిర అంతఃవరుస raw_spinlock_t *rq_lockp(నిర్మాణం rq *rq)
{
	తిరిగి &rq->__lock;
}

స్థిర అంతఃవరుస raw_spinlock_t *__rq_lockp(నిర్మాణం rq *rq)
	__returns_ctx_lock(rq_lockp(rq)) /* alias them */
{
	తిరిగి &rq->__lock;
}

స్థిర అంతఃవరుస bool sched_cpu_cookie_సరిపోలిక(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
	తిరిగి true;
}

స్థిర అంతఃవరుస bool sched_core_cookie_సరిపోలిక(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
	తిరిగి true;
}

స్థిర అంతఃవరుస bool sched_సమూహం_cookie_సరిపోలిక(నిర్మాణం rq *rq,
					    నిర్మాణం task_struct *p,
					    నిర్మాణం sched_సమూహం *సమూహం)
{
	తిరిగి true;
}

స్థిర అంతఃవరుస bool task_has_sched_core(నిర్మాణం task_struct *p)
{
	తిరిగి false;
}

#అయితే_ముగింపు /* !CONFIG_SCHED_CORE */

#నిర్వచితమైతే CONFIG_RT_సమూహం_SCHED
# ifdef CONFIG_RT_సమూహం_SCHED_DEFAULT_DISABLED
DECLARE_STATIC_కీలకం_FALSE(rt_సమూహం_sched);
స్థిర అంతఃవరుస bool rt_సమూహం_sched_enabled(శూన్య)
{
	తిరిగి static_branch_unlikely(&rt_సమూహం_sched);
}
# లేకపోతే /* !CONFIG_RT_GROUP_SCHED_DEFAULT_DISABLED: */
DECLARE_STATIC_కీలకం_TRUE(rt_సమూహం_sched);
స్థిర అంతఃవరుస bool rt_సమూహం_sched_enabled(శూన్య)
{
	తిరిగి static_branch_likely(&rt_సమూహం_sched);
}
# endif /* !CONFIG_RT_GROUP_SCHED_DEFAULT_DISABLED */
#లేకపోతే /* !CONFIG_RT_GROUP_SCHED: */
# define rt_సమూహం_sched_enabled()	false
#అయితే_ముగింపు /* !CONFIG_RT_GROUP_SCHED */

స్థిర అంతఃవరుస శూన్య lockdep_assert_rq_held(నిర్మాణం rq *rq)
	__assumes_ctx_lock(__rq_lockp(rq))
{
	lockdep_assert_held(__rq_lockp(rq));
}

బాహ్య శూన్య raw_spin_rq_తాళం_nested(నిర్మాణం rq *rq, పూర్ణాంకం subclass)
	__acquires(__rq_lockp(rq));

బాహ్య bool raw_spin_rq_trylock(నిర్మాణం rq *rq)
	__cond_acquires(true, __rq_lockp(rq));

స్థిర అంతఃవరుస శూన్య raw_spin_rq_తాళం(నిర్మాణం rq *rq)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_తాళం_nested(rq, 0);
}

స్థిర అంతఃవరుస శూన్య raw_spin_rq_unlock(నిర్మాణం rq *rq)
	__releases(__rq_lockp(rq))
{
	raw_spin_unlock(rq_lockp(rq));
}

స్థిర అంతఃవరుస శూన్య raw_spin_rq_తాళం_irq(నిర్మాణం rq *rq)
	__acquires(__rq_lockp(rq))
{
	local_irq_disable();
	raw_spin_rq_తాళం(rq);
}

స్థిర అంతఃవరుస శూన్య raw_spin_rq_unlock_irq(నిర్మాణం rq *rq)
	__releases(__rq_lockp(rq))
{
	raw_spin_rq_unlock(rq);
	local_irq_enable();
}

స్థిర అంతఃవరుస గుర్తులేని దీర్ఘ raw_spin_rq_తాళం_irqsave_2(నిర్మాణం rq *rq)
	__acquires(__rq_lockp(rq))
{
	గుర్తులేని దీర్ఘ flags;

	local_irq_save(flags);
	raw_spin_rq_తాళం(rq);

	తిరిగి flags;
}

స్థిర అంతఃవరుస శూన్య raw_spin_rq_unlock_irqrestore(నిర్మాణం rq *rq, గుర్తులేని దీర్ఘ flags)
	__releases(__rq_lockp(rq))
{
	raw_spin_rq_unlock(rq);
	local_irq_restore(flags);
}

#నిర్వచించు raw_spin_rq_తాళం_irqsave(rq, flags)	\
చేయి {						\
	flags = raw_spin_rq_తాళం_irqsave_2(rq);	\
} అయితే (0)

బాహ్య శూన్య __update_idle_core(నిర్మాణం rq *rq);

స్థిర అంతఃవరుస శూన్య update_idle_core(నిర్మాణం rq *rq)
{
	ఒకవేళ (sched_smt_చురుకైన())
		__update_idle_core(rq);
}

#నిర్వచితమైతే CONFIG_FAIR_సమూహం_SCHED
స్థిర అంతఃవరుస నిర్మాణం task_struct *task_యొక్క(నిర్మాణం sched_entity *se)
{
	WARN_ON_ONCE(!entity_is_task(se));
	తిరిగి container_యొక్క(se, నిర్మాణం task_struct, se);
}

స్థిర అంతఃవరుస నిర్మాణం cfs_rq *task_cfs_rq(నిర్మాణం task_struct *p)
{
	తిరిగి p->se.cfs_rq;
}

/* runqueue on which this entity is (to be) queued */
స్థిర అంతఃవరుస నిర్మాణం cfs_rq *cfs_rq_యొక్క(స్థిరరాశి నిర్మాణం sched_entity *se)
{
	తిరిగి se->cfs_rq;
}

/* runqueue "owned" by this group */
స్థిర అంతఃవరుస నిర్మాణం cfs_rq *సమూహం_cfs_rq(నిర్మాణం sched_entity *grp)
{
	తిరిగి grp->my_q;
}

#లేకపోతే /* !CONFIG_FAIR_GROUP_SCHED: */

#నిర్వచించు task_యొక్క(_se)		container_యొక్క(_se, నిర్మాణం task_struct, se)

స్థిర అంతఃవరుస నిర్మాణం cfs_rq *task_cfs_rq(స్థిరరాశి నిర్మాణం task_struct *p)
{
	తిరిగి &task_rq(p)->cfs;
}

స్థిర అంతఃవరుస నిర్మాణం cfs_rq *cfs_rq_యొక్క(స్థిరరాశి నిర్మాణం sched_entity *se)
{
	స్థిరరాశి నిర్మాణం task_struct *p = task_యొక్క(se);
	నిర్మాణం rq *rq = task_rq(p);

	తిరిగి &rq->cfs;
}

/* runqueue "owned" by this group */
స్థిర అంతఃవరుస నిర్మాణం cfs_rq *సమూహం_cfs_rq(నిర్మాణం sched_entity *grp)
{
	తిరిగి ఖాళీ;
}

#అయితే_ముగింపు /* !CONFIG_FAIR_GROUP_SCHED */

బాహ్య శూన్య update_rq_avg_idle(నిర్మాణం rq *rq);
బాహ్య శూన్య update_rq_clock(నిర్మాణం rq *rq);

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
#నిర్వచించు RQCF_REQ_SKIP		0x01
#నిర్వచించు RQCF_ACT_SKIP		0x02
#నిర్వచించు RQCF_UPDATED		0x04

స్థిర అంతఃవరుస శూన్య assert_clock_updated(నిర్మాణం rq *rq)
{
	/*
	 * The only reason for not seeing a clock update since the
	 * last rq_pin_lock() is if we're currently skipping updates.
	 */
	WARN_ON_ONCE(rq->clock_update_flags < RQCF_ACT_SKIP);
}

స్థిర అంతఃవరుస u64 rq_clock(నిర్మాణం rq *rq)
{
	lockdep_assert_rq_held(rq);
	assert_clock_updated(rq);

	తిరిగి rq->clock;
}

స్థిర అంతఃవరుస u64 rq_clock_task(నిర్మాణం rq *rq)
{
	lockdep_assert_rq_held(rq);
	assert_clock_updated(rq);

	తిరిగి rq->clock_task;
}

స్థిర అంతఃవరుస శూన్య rq_clock_skip_update(నిర్మాణం rq *rq)
{
	lockdep_assert_rq_held(rq);
	rq->clock_update_flags |= RQCF_REQ_SKIP;
}

/*
 * See rt task throttling, which is the only time a skip
 * request is canceled.
 */
స్థిర అంతఃవరుస శూన్య rq_clock_cancel_skipupdate(నిర్మాణం rq *rq)
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
స్థిర అంతఃవరుస శూన్య rq_clock_ప్రారంభం_loop_update(నిర్మాణం rq *rq)
{
	lockdep_assert_rq_held(rq);
	WARN_ON_ONCE(rq->clock_update_flags & RQCF_ACT_SKIP);
	rq->clock_update_flags |= RQCF_ACT_SKIP;
}

స్థిర అంతఃవరుస శూన్య rq_clock_stop_loop_update(నిర్మాణం rq *rq)
{
	lockdep_assert_rq_held(rq);
	rq->clock_update_flags &= ~RQCF_ACT_SKIP;
}

నిర్మాణం rq_flags {
	గుర్తులేని దీర్ఘ flags;
	నిర్మాణం pin_cookie cookie;
	/*
	 * A copy of (rq::clock_update_flags & RQCF_UPDATED) for the
	 * current pin context is stashed here in case it needs to be
	 * restored in rq_repin_lock().
	 */
	గుర్తులేని పూర్ణాంకం clock_update_flags;
};

బాహ్య నిర్మాణం balance_callback balance_నెట్టు_callback;

#నిర్వచితమైతే CONFIG_SCHED_CLASS_EXT
బాహ్య స్థిరరాశి నిర్మాణం sched_class ext_sched_class;

DECLARE_STATIC_కీలకం_FALSE(__scx_enabled);	/* SCX BPF scheduler loaded */
DECLARE_STATIC_కీలకం_FALSE(__scx_switched_all);	/* all fair class tasks on SCX */

#నిర్వచించు scx_enabled()		static_branch_unlikely(&__scx_enabled)
#నిర్వచించు scx_switched_all()	static_branch_unlikely(&__scx_switched_all)

స్థిర అంతఃవరుస శూన్య scx_rq_clock_update(నిర్మాణం rq *rq, u64 clock)
{
	ఒకవేళ (!scx_enabled())
		తిరిగి;
	WRITE_ONCE(rq->scx.clock, clock);
	smp_store_release(&rq->scx.flags, rq->scx.flags | SCX_RQ_CLK_VALID);
}

స్థిర అంతఃవరుస శూన్య scx_rq_clock_invalidate(నిర్మాణం rq *rq)
{
	ఒకవేళ (!scx_enabled())
		తిరిగి;
	WRITE_ONCE(rq->scx.flags, rq->scx.flags & ~SCX_RQ_CLK_VALID);
}

#లేకపోతే /* !CONFIG_SCHED_CLASS_EXT: */
#నిర్వచించు scx_enabled()		false
#నిర్వచించు scx_switched_all()	false

స్థిర అంతఃవరుస శూన్య scx_rq_clock_update(నిర్మాణం rq *rq, u64 clock) {}
స్థిర అంతఃవరుస శూన్య scx_rq_clock_invalidate(నిర్మాణం rq *rq) {}
#అయితే_ముగింపు /* !CONFIG_SCHED_CLASS_EXT */

స్థిర అంతఃవరుస శూన్య assert_balance_callbacks_ఖాళీలు(నిర్మాణం rq *rq)
{
	WARN_ON_ONCE(IS_ENABLED(CONFIG_PROVE_LOCKING) &&
		     rq->balance_callback &&
		     rq->balance_callback != &balance_నెట్టు_callback);
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
స్థిర అంతఃవరుస శూన్య rq_pin_తాళం(నిర్మాణం rq *rq, నిర్మాణం rq_flags *rf)
{
	rf->cookie = lockdep_pin_తాళం(__rq_lockp(rq));

	rq->clock_update_flags &= (RQCF_REQ_SKIP|RQCF_ACT_SKIP);
	rf->clock_update_flags = 0;
	assert_balance_callbacks_ఖాళీలు(rq);
}

స్థిర అంతఃవరుస శూన్య rq_unpin_తాళం(నిర్మాణం rq *rq, నిర్మాణం rq_flags *rf)
{
	ఒకవేళ (rq->clock_update_flags > RQCF_ACT_SKIP)
		rf->clock_update_flags = RQCF_UPDATED;

	scx_rq_clock_invalidate(rq);
	lockdep_unpin_తాళం(__rq_lockp(rq), rf->cookie);
}

స్థిర అంతఃవరుస శూన్య rq_repin_తాళం(నిర్మాణం rq *rq, నిర్మాణం rq_flags *rf)
{
	lockdep_repin_తాళం(__rq_lockp(rq), rf->cookie);

	/*
	 * Restore the value we stashed in @rf for this pin context.
	 */
	rq->clock_update_flags |= rf->clock_update_flags;
}

#నిర్వచించు __task_rq_lock(...) __acquire_ret(___task_rq_lock(__VA_ARGS__), __rq_lockp(__ret))
బాహ్య నిర్మాణం rq *___task_rq_lock(నిర్మాణం task_struct *p, నిర్మాణం rq_flags *rf) __acquires_ret;

#నిర్వచించు task_rq_తాళం_2(...) __acquire_ret(task_rq_తాళం(__VA_ARGS__), __rq_lockp(__ret))
బాహ్య నిర్మాణం rq *task_rq_తాళం(నిర్మాణం task_struct *p, నిర్మాణం rq_flags *rf)
	__acquires(&p->pi_తాళం) __acquires_ret;

స్థిర అంతఃవరుస శూన్య
__task_rq_unlock(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, నిర్మాణం rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_తాళం(rq, rf);
	raw_spin_rq_unlock(rq);
}

స్థిర అంతఃవరుస శూన్య
task_rq_unlock(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, నిర్మాణం rq_flags *rf)
	__releases(__rq_lockp(rq), &p->pi_తాళం)
{
	__task_rq_unlock(rq, p, rf);
	raw_spin_unlock_irqrestore(&p->pi_తాళం, rf->flags);
}

DEFINE_తాళం_GUARD_1(task_rq_తాళం_2, నిర్మాణం task_struct,
		    _T->rq = task_rq_తాళం_2(_T->తాళం, &_T->rf),
		    task_rq_unlock(_T->rq, _T->తాళం, &_T->rf),
		    నిర్మాణం rq *rq; నిర్మాణం rq_flags rf)
DECLARE_తాళం_GUARD_1_ATTRS(task_rq_తాళం_2, __acquires(_T->pi_తాళం), __releases((*(నిర్మాణం task_struct **)_T)->pi_తాళం))
#నిర్వచించు class_task_rq_తాళం_constructor(_T) WITH_తాళం_GUARD_1_ATTRS(task_rq_తాళం_2, _T)

DEFINE_తాళం_GUARD_1(__task_rq_lock, నిర్మాణం task_struct,
		    _T->rq = __task_rq_lock(_T->తాళం, &_T->rf),
		    __task_rq_unlock(_T->rq, _T->తాళం, &_T->rf),
		    నిర్మాణం rq *rq; నిర్మాణం rq_flags rf)

స్థిర అంతఃవరుస శూన్య rq_తాళం_irqsave(నిర్మాణం rq *rq, నిర్మాణం rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_తాళం_irqsave(rq, rf->flags);
	rq_pin_తాళం(rq, rf);
}

స్థిర అంతఃవరుస శూన్య rq_తాళం_irq(నిర్మాణం rq *rq, నిర్మాణం rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_తాళం_irq(rq);
	rq_pin_తాళం(rq, rf);
}

స్థిర అంతఃవరుస శూన్య rq_తాళం(నిర్మాణం rq *rq, నిర్మాణం rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_తాళం(rq);
	rq_pin_తాళం(rq, rf);
}

స్థిర అంతఃవరుస శూన్య rq_unlock_irqrestore(నిర్మాణం rq *rq, నిర్మాణం rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_తాళం(rq, rf);
	raw_spin_rq_unlock_irqrestore(rq, rf->flags);
}

స్థిర అంతఃవరుస శూన్య rq_unlock_irq(నిర్మాణం rq *rq, నిర్మాణం rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_తాళం(rq, rf);
	raw_spin_rq_unlock_irq(rq);
}

స్థిర అంతఃవరుస శూన్య rq_unlock(నిర్మాణం rq *rq, నిర్మాణం rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_తాళం(rq, rf);
	raw_spin_rq_unlock(rq);
}

DEFINE_తాళం_GUARD_1(rq_తాళం, నిర్మాణం rq,
		    rq_తాళం(_T->తాళం, &_T->rf),
		    rq_unlock(_T->తాళం, &_T->rf),
		    నిర్మాణం rq_flags rf)

DECLARE_తాళం_GUARD_1_ATTRS(rq_తాళం, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(నిర్మాణం rq **)_T)));
#నిర్వచించు class_rq_తాళం_constructor(_T) WITH_తాళం_GUARD_1_ATTRS(rq_తాళం, _T)

DEFINE_తాళం_GUARD_1(rq_తాళం_irq, నిర్మాణం rq,
		    rq_తాళం_irq(_T->తాళం, &_T->rf),
		    rq_unlock_irq(_T->తాళం, &_T->rf),
		    నిర్మాణం rq_flags rf)

DECLARE_తాళం_GUARD_1_ATTRS(rq_తాళం_irq, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(నిర్మాణం rq **)_T)));
#నిర్వచించు class_rq_తాళం_irq_constructor(_T) WITH_తాళం_GUARD_1_ATTRS(rq_తాళం_irq, _T)

DEFINE_తాళం_GUARD_1(rq_తాళం_irqsave, నిర్మాణం rq,
		    rq_తాళం_irqsave(_T->తాళం, &_T->rf),
		    rq_unlock_irqrestore(_T->తాళం, &_T->rf),
		    నిర్మాణం rq_flags rf)

DECLARE_తాళం_GUARD_1_ATTRS(rq_తాళం_irqsave, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(నిర్మాణం rq **)_T)));
#నిర్వచించు class_rq_తాళం_irqsave_constructor(_T) WITH_తాళం_GUARD_1_ATTRS(rq_తాళం_irqsave, _T)

DEFINE_తాళం_GUARD_1(raw_spin_rq_తాళం_irqsave, నిర్మాణం rq,
		    raw_spin_rq_తాళం_irqsave(_T->తాళం, _T->flags),
		    raw_spin_rq_unlock_irqrestore(_T->తాళం, _T->flags),
		    గుర్తులేని దీర్ఘ flags)

DECLARE_తాళం_GUARD_1_ATTRS(raw_spin_rq_తాళం_irqsave, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(నిర్మాణం rq **)_T)));
#నిర్వచించు class_raw_spin_rq_తాళం_irqsave_constructor(_T) \
	WITH_తాళం_GUARD_1_ATTRS(raw_spin_rq_తాళం_irqsave, _T)

#నిర్వచించు this_rq_తాళం_irq(...) __acquire_ret(this_rq_తాళం_irq_2(__VA_ARGS__), __rq_lockp(__ret))
స్థిర అంతఃవరుస నిర్మాణం rq *this_rq_తాళం_irq_2(నిర్మాణం rq_flags *rf) __acquires_ret
{
	నిర్మాణం rq *rq;

	local_irq_disable();
	rq = this_rq();
	rq_తాళం(rq, rf);

	తిరిగి rq;
}

#నిర్వచితమైతే CONFIG_NUMA

గణన numa_topology_type {
	NUMA_ప్రత్యక్ష,
	NUMA_GLUELESS_MESH,
	NUMA_BACKPLANE,
};

బాహ్య గణన numa_topology_type sched_numa_topology_type;
బాహ్య పూర్ణాంకం sched_max_numa_దూరం;
బాహ్య bool కనుగొను_numa_దూరం(పూర్ణాంకం దూరం);
బాహ్య శూన్య sched_init_numa(పూర్ణాంకం offline_కణుపు);
బాహ్య శూన్య sched_update_numa(పూర్ణాంకం cpu, bool online);
బాహ్య శూన్య sched_domains_numa_masks_సమితి(గుర్తులేని పూర్ణాంకం cpu);
బాహ్య శూన్య sched_domains_numa_masks_clear(గుర్తులేని పూర్ణాంకం cpu);
బాహ్య పూర్ణాంకం sched_numa_కనుగొను_closest(స్థిరరాశి నిర్మాణం cpumask *cpus, పూర్ణాంకం cpu);

#లేకపోతే /* !CONFIG_NUMA: */

స్థిర అంతఃవరుస శూన్య sched_init_numa(పూర్ణాంకం offline_కణుపు) { }
స్థిర అంతఃవరుస శూన్య sched_update_numa(పూర్ణాంకం cpu, bool online) { }
స్థిర అంతఃవరుస శూన్య sched_domains_numa_masks_సమితి(గుర్తులేని పూర్ణాంకం cpu) { }
స్థిర అంతఃవరుస శూన్య sched_domains_numa_masks_clear(గుర్తులేని పూర్ణాంకం cpu) { }

స్థిర అంతఃవరుస పూర్ణాంకం sched_numa_కనుగొను_closest(స్థిరరాశి నిర్మాణం cpumask *cpus, పూర్ణాంకం cpu)
{
	తిరిగి nr_cpu_ids;
}

#అయితే_ముగింపు /* !CONFIG_NUMA */

#నిర్వచితమైతే CONFIG_NUMA_BALANCING

/* The regions in numa_faults array from task_struct */
గణన numa_దోషాలు_stats {
	NUMA_MEM = 0,
	NUMA_CPU,
	NUMA_MEMBUF,
	NUMA_CPUBUF
};

బాహ్య శూన్య sched_setnuma(నిర్మాణం task_struct *p, పూర్ణాంకం కణుపు);
బాహ్య పూర్ణాంకం migrate_task_గమ్యం(నిర్మాణం task_struct *p, పూర్ణాంకం cpu);
బాహ్య పూర్ణాంకం migrate_మార్పిడి(నిర్మాణం task_struct *p, నిర్మాణం task_struct *t,
			పూర్ణాంకం cpu, పూర్ణాంకం scpu);
బాహ్య శూన్య init_numa_balancing(u64 clone_flags, నిర్మాణం task_struct *p);

#లేకపోతే /* !CONFIG_NUMA_BALANCING: */

స్థిర అంతఃవరుస శూన్య
init_numa_balancing(u64 clone_flags, నిర్మాణం task_struct *p)
{
}

#అయితే_ముగింపు /* !CONFIG_NUMA_BALANCING */

పూర్ణాంకం task_llc(స్థిరరాశి నిర్మాణం task_struct *p);

స్థిర అంతఃవరుస శూన్య
వరుస_balance_callback(నిర్మాణం rq *rq,
		       నిర్మాణం balance_callback *తల,
		       శూన్య (*func)(నిర్మాణం rq *rq))
{
	lockdep_assert_rq_held(rq);

	/*
	 * Don't (re)queue an already queued item; nor queue anything when
	 * balance_push() is active, see the comment with
	 * balance_push_callback.
	 */
	ఒకవేళ (unlikely(తల->తదుపరి || rq->balance_callback == &balance_నెట్టు_callback))
		తిరిగి;

	తల->func = func;
	తల->తదుపరి = rq->balance_callback;
	rq->balance_callback = తల;
}

#నిర్వచించు rcu_dereference_sched_domain(p) \
	rcu_dereference_all_check((p), lockdep_is_held(&sched_domains_mutex))

/*
 * The domain tree (rq->sd) is protected by RCU's quiescent state transition.
 * See destroy_sched_domains: call_rcu for details.
 *
 * The domain tree of any CPU may only be accessed from within
 * preempt-disabled sections.
 */
#నిర్వచించు for_each_domain(cpu, __sd) \
	కొరకు (__sd = rcu_dereference_sched_domain(cpu_rq(cpu)->sd); \
			__sd; __sd = __sd->మాతృ)

/* A mask of all the SD flags that have the SDF_SHARED_CHILD metaflag */
#నిర్వచించు SD_FLAG(name, mflags) (name * !!((mflags) & SDF_SHARED_బిడ్డ)) |
స్థిర స్థిరరాశి గుర్తులేని పూర్ణాంకం SD_SHARED_బిడ్డ_ముసుగు =
#అన్తర్భావ <linux/sched/sd_flags.h>
0;
#నిర్వచనం_రద్దు SD_FLAG

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
స్థిర అంతఃవరుస నిర్మాణం sched_domain *highest_flag_domain(పూర్ణాంకం cpu, పూర్ణాంకం flag)
{
	నిర్మాణం sched_domain *sd, *hsd = ఖాళీ;

	for_each_domain(cpu, sd) {
		ఒకవేళ (sd->flags & flag) {
			hsd = sd;
			కొనసాగు;
		}

		/*
		 * Stop the search if @flag is known to be shared at lower
		 * levels. It will not be found further up.
		 */
		ఒకవేళ (flag & SD_SHARED_బిడ్డ_ముసుగు)
			ఆపు;
	}

	తిరిగి hsd;
}

స్థిర అంతఃవరుస నిర్మాణం sched_domain *అత్యల్ప_flag_domain(పూర్ణాంకం cpu, పూర్ణాంకం flag)
{
	నిర్మాణం sched_domain *sd;

	for_each_domain(cpu, sd) {
		ఒకవేళ (sd->flags & flag)
			ఆపు;
	}

	తిరిగి sd;
}

DECLARE_చొప్పున_CPU(నిర్మాణం sched_domain __rcu *, sd_llc);
DECLARE_చొప్పున_CPU(పూర్ణాంకం, sd_llc_కొలత);
DECLARE_చొప్పున_CPU(పూర్ణాంకం, sd_llc_id);
DECLARE_చొప్పున_CPU(పూర్ణాంకం, sd_share_id);
DECLARE_చొప్పున_CPU(నిర్మాణం sched_domain_shared __rcu *, sd_llc_shared);
DECLARE_చొప్పున_CPU(నిర్మాణం sched_domain_shared __rcu *, sd_balance_shared);
DECLARE_చొప్పున_CPU(నిర్మాణం sched_domain __rcu *, sd_numa);
DECLARE_చొప్పున_CPU(నిర్మాణం sched_domain __rcu *, sd_asym_packing);
DECLARE_చొప్పున_CPU(నిర్మాణం sched_domain __rcu *, sd_asym_cpucapacity);

బాహ్య నిర్మాణం static_కీలకం_false sched_asym_cpucapacity;
బాహ్య నిర్మాణం static_కీలకం_false sched_cluster_చురుకైన;

స్థిర __always_inline bool sched_asym_cpucap_చురుకైన(శూన్య)
{
	తిరిగి static_branch_unlikely(&sched_asym_cpucapacity);
}

నిర్మాణం sched_సమూహం_సామర్థ్యం {
	atomic_t		ref;
	/*
	 * CPU capacity of this group, SCHED_CAPACITY_SCALE being max capacity
	 * for a single CPU.
	 */
	గుర్తులేని దీర్ఘ		సామర్థ్యం;
	గుర్తులేని దీర్ఘ		min_సామర్థ్యం;		/* Min per-CPU capacity in group */
	గుర్తులేని దీర్ఘ		max_సామర్థ్యం;		/* Max per-CPU capacity in group */
	గుర్తులేని దీర్ఘ		తదుపరి_update;
	పూర్ణాంకం			imbalance;		/* XXX unrelated to capacity but shared group state */

	పూర్ణాంకం			id;

	గుర్తులేని దీర్ఘ		cpumask[];		/* Balance mask */
};

నిర్మాణం sched_సమూహం {
	నిర్మాణం sched_సమూహం	*తదుపరి;			/* Must be a circular list */
	atomic_t		ref;

	గుర్తులేని పూర్ణాంకం		సమూహం_బరువు;
	గుర్తులేని పూర్ణాంకం		cores;
	నిర్మాణం sched_సమూహం_సామర్థ్యం *sgc;
	పూర్ణాంకం			asym_prefer_cpu;	/* CPU of highest priority in group */
	పూర్ణాంకం			flags;

	/*
	 * The CPUs this group covers.
	 *
	 * NOTE: this field is variable length. (Allocated dynamically
	 * by attaching extra space to the end of the structure,
	 * depending on how many CPUs the kernel has booted up with)
	 */
	గుర్తులేని దీర్ఘ		cpumask[];
};

స్థిర అంతఃవరుస నిర్మాణం cpumask *sched_సమూహం_పరిధి(నిర్మాణం sched_సమూహం *sg)
{
	తిరిగి గమ్యం_cpumask(sg->cpumask);
}

/*
 * See build_balance_mask().
 */
స్థిర అంతఃవరుస నిర్మాణం cpumask *సమూహం_balance_ముసుగు(నిర్మాణం sched_సమూహం *sg)
{
	తిరిగి గమ్యం_cpumask(sg->sgc->cpumask);
}

బాహ్య పూర్ణాంకం సమూహం_balance_cpu(నిర్మాణం sched_సమూహం *sg);

బాహ్య శూన్య update_sched_domain_debugfs(శూన్య);
బాహ్య శూన్య dirty_sched_domain_sysctl(పూర్ణాంకం cpu);

బాహ్య పూర్ణాంకం sched_update_scaling(శూన్య);

స్థిర అంతఃవరుస స్థిరరాశి నిర్మాణం cpumask *task_user_cpus(నిర్మాణం task_struct *p)
{
	ఒకవేళ (!p->user_cpus_ptr)
		తిరిగి cpu_possible_ముసుగు; /* &init_task.cpus_mask */
	తిరిగి p->user_cpus_ptr;
}

#నిర్వచితమైతే CONFIG_CGROUP_SCHED

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
స్థిర అంతఃవరుస నిర్మాణం task_సమూహం *task_సమూహం(నిర్మాణం task_struct *p)
{
	తిరిగి p->sched_task_సమూహం;
}

#నిర్వచితమైతే CONFIG_FAIR_సమూహం_SCHED
/*
 * Defined here to be available before stats.h is included, since
 * stats.h has dependencies on things defined later in this file.
 */
నిర్మాణం cfs_tg_state {
	నిర్మాణం cfs_rq		cfs_rq;
	నిర్మాణం sched_entity	se;
	నిర్మాణం sched_statistics	stats;
} __no_randomize_layout;

/* Access a specific CPU's cfs_rq from a task group */
స్థిర అంతఃవరుస నిర్మాణం cfs_rq *tg_cfs_rq(నిర్మాణం task_సమూహం *tg, పూర్ణాంకం cpu)
{
	తిరిగి చొప్పున_cpu_ptr(tg->cfs_rq, cpu);
}

స్థిర అంతఃవరుస నిర్మాణం sched_entity *tg_se(నిర్మాణం task_సమూహం *tg, పూర్ణాంకం cpu)
{
	నిర్మాణం cfs_tg_state *state;

	ఒకవేళ (is_మూలం_task_సమూహం(tg))
		తిరిగి ఖాళీ;

	state = container_యొక్క(tg_cfs_rq(tg, cpu), నిర్మాణం cfs_tg_state, cfs_rq);
	తిరిగి &state->se;
}

స్థిర అంతఃవరుస నిర్మాణం sched_entity *cfs_rq_se(నిర్మాణం cfs_rq *cfs_rq)
{
	నిర్మాణం cfs_tg_state *state;

	ఒకవేళ (is_మూలం_task_సమూహం(cfs_rq->tg))
		తిరిగి ఖాళీ;

	state = container_యొక్క(cfs_rq, నిర్మాణం cfs_tg_state, cfs_rq);
	తిరిగి &state->se;
}
#అయితే_ముగింపు

/* Change a task's cfs_rq and parent entity if it moves across CPUs/groups */
స్థిర అంతఃవరుస శూన్య సమితి_task_rq(నిర్మాణం task_struct *p, గుర్తులేని పూర్ణాంకం cpu)
{
#ఒకవేళ defined(CONFIG_FAIR_సమూహం_SCHED) || defined(CONFIG_RT_సమూహం_SCHED)
	నిర్మాణం task_సమూహం *tg = task_సమూహం(p);
#అయితే_ముగింపు

#నిర్వచితమైతే CONFIG_FAIR_సమూహం_SCHED
	సమితి_task_rq_fair(&p->se, p->se.cfs_rq, tg_cfs_rq(tg, cpu));
	p->se.cfs_rq = tg_cfs_rq(tg, cpu);
	p->se.మాతృ = tg_se(tg, cpu);
	p->se.లోతు = p->se.మాతృ ? p->se.మాతృ->లోతు + 1 : 0;
#అయితే_ముగింపు

#నిర్వచితమైతే CONFIG_RT_సమూహం_SCHED
	/*
	 * p->rt.rt_rq is NULL initially and it is easier to assign
	 * root_task_group's rt_rq than switching in rt_rq_of_se()
	 * Clobbers tg(!)
	 */
	ఒకవేళ (!rt_సమూహం_sched_enabled())
		tg = &మూలం_task_సమూహం;
	p->rt.rt_rq  = tg->rt_rq[cpu];
	p->rt.మాతృ = tg->rt_se[cpu];
#అయితే_ముగింపు /* CONFIG_RT_GROUP_SCHED */
}

#లేకపోతే /* !CONFIG_CGROUP_SCHED: */

స్థిర అంతఃవరుస శూన్య సమితి_task_rq(నిర్మాణం task_struct *p, గుర్తులేని పూర్ణాంకం cpu) { }

స్థిర అంతఃవరుస నిర్మాణం task_సమూహం *task_సమూహం(నిర్మాణం task_struct *p)
{
	తిరిగి ఖాళీ;
}

#అయితే_ముగింపు /* !CONFIG_CGROUP_SCHED */

స్థిర అంతఃవరుస శూన్య __set_task_cpu(నిర్మాణం task_struct *p, గుర్తులేని పూర్ణాంకం cpu)
{
	సమితి_task_rq(p, cpu);
#నిర్వచితమైతే CONFIG_SMP
	/*
	 * After ->cpu is set up to a new value, task_rq_lock(p, ...) can be
	 * successfully executed on another CPU. We must ensure that updates of
	 * per-task data have been completed by this moment.
	 */
	smp_wmb();
	WRITE_ONCE(task_thread_info(p)->cpu, cpu);
	p->wake_cpu = cpu;
	rseq_sched_సమితి_ids_మారింది(p);
#అయితే_ముగింపు /* CONFIG_SMP */
}

/*
 * Tunables:
 */

#నిర్వచించు SCHED_FEAT(name, enabled)	\
	__SCHED_FEAT_##name ,

గణన {
#అన్తర్భావ "features.h"
	__SCHED_FEAT_NR,
};

#నిర్వచనం_రద్దు SCHED_FEAT

/*
 * To support run-time toggling of sched features, all the translation units
 * (but core.c) reference the sysctl_sched_features defined in core.c.
 */
బాహ్య __read_mostly గుర్తులేని పూర్ణాంకం sysctl_sched_features;

#నిర్వచితమైతే CONFIG_JUMP_LABEL

సంఘం sched_feat_కీలకం {
	నిర్మాణం static_కీలకం_true	కీలకం_true;
	నిర్మాణం static_కీలకం_false	కీలకం_false;
};

#నిర్వచించు sched_feat_branch_true(కీలకం)	static_branch_likely(&(కీలకం)->కీలకం_true)
#నిర్వచించు sched_feat_branch_false(కీలకం)	static_branch_unlikely(&(కీలకం)->కీలకం_false)

#నిర్వచించు SCHED_FEAT(name, enabled)			\
స్థిర __always_inline bool				\
static_branch_##name(సంఘం sched_feat_కీలకం *కీలకం) 	\
{							\
	తిరిగి sched_feat_branch_##enabled(కీలకం);	\
}

#అన్తర్భావ "features.h"
#నిర్వచనం_రద్దు SCHED_FEAT

బాహ్య సంఘం sched_feat_కీలకం sched_feat_కీలకాలు[__SCHED_FEAT_NR];
#నిర్వచించు sched_feat(x) (static_branch_##x(&sched_feat_కీలకాలు[__SCHED_FEAT_##x]))

#లేకపోతే /* !CONFIG_JUMP_LABEL: */

#నిర్వచించు sched_feat(x) (sysctl_sched_features & (1UL << __SCHED_FEAT_##x))

#అయితే_ముగింపు /* !CONFIG_JUMP_LABEL */

బాహ్య నిర్మాణం static_కీలకం_false sched_numa_balancing;
బాహ్య నిర్మాణం static_కీలకం_false sched_schedstats;

స్థిర అంతఃవరుస u64 global_rt_period(శూన్య)
{
	తిరిగి (u64)sysctl_sched_rt_period * NSEC_చొప్పున_USEC;
}

స్థిర అంతఃవరుస u64 global_rt_runtime(శూన్య)
{
	ఒకవేళ (sysctl_sched_rt_runtime < 0)
		తిరిగి RUNTIME_INF;

	తిరిగి (u64)sysctl_sched_rt_runtime * NSEC_చొప్పున_USEC;
}

/*
 * Is p the current execution context?
 */
స్థిర అంతఃవరుస పూర్ణాంకం task_current(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
	తిరిగి rq->curr == p;
}

/*
 * Is p the current scheduling context?
 *
 * Note that it might be the current execution context at the same time if
 * rq->curr == rq->donor == p.
 */
స్థిర అంతఃవరుస పూర్ణాంకం task_current_donor(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
	తిరిగి rq->donor == p;
}

స్థిర అంతఃవరుస bool task_is_blocked(నిర్మాణం task_struct *p)
{
	ఒకవేళ (!sched_proxy_exec())
		తిరిగి false;

	తిరిగి !!p->blocked_on;
}

#నిర్వచితమైతే CONFIG_SCHED_PROXY_EXEC
శూన్య sched_proxy_ఖండం_task(నిర్మాణం rq *rq, నిర్మాణం task_struct *p);
#లేకపోతే
స్థిర అంతఃవరుస శూన్య sched_proxy_ఖండం_task(నిర్మాణం rq *rq, నిర్మాణం task_struct *p) {}
#అయితే_ముగింపు

స్థిర అంతఃవరుస పూర్ణాంకం task_on_cpu(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
	తిరిగి p->on_cpu;
}

స్థిర అంతఃవరుస పూర్ణాంకం task_on_rq_queued(నిర్మాణం task_struct *p)
{
	తిరిగి READ_ONCE(p->on_rq) == TASK_ON_RQ_QUEUED;
}

స్థిర అంతఃవరుస పూర్ణాంకం task_on_rq_migrating(నిర్మాణం task_struct *p)
{
	తిరిగి READ_ONCE(p->on_rq) == TASK_ON_RQ_MIGRATING;
}

/* Wake flags. The first three directly map to some SD flag value */
#నిర్వచించు WF_EXEC			0x02 /* Wakeup after exec; maps to SD_BALANCE_EXEC */
#నిర్వచించు WF_FORK			0x04 /* Wakeup after fork; maps to SD_BALANCE_FORK */
#నిర్వచించు WF_TTWU			0x08 /* Wakeup;            maps to SD_BALANCE_WAKE */
/*
 * Hint that the caller expects the waker to sleep soon.
 * Scheduler classes may use it for placement or preemption.
 * Callers must not rely on it to prevent migration,
 * preserve CPU locality or make the wakee run next.
 */
#నిర్వచించు WF_SYNC			0x10
#నిర్వచించు WF_MIGRATED		0x20 /* Internal use, task got migrated */
#నిర్వచించు WF_CURRENT_CPU		0x40 /* Prefer to move the wakee to the current CPU. */
#నిర్వచించు WF_RQ_SELECTED		0x80 /* ->select_task_rq() was called */
#నిర్వచించు WF_TTWU_RQ		0x100 /* Wakeup completed through ttwu_runnable() */

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

#నిర్వచించు బరువు_IDLEPRIO		3
#నిర్వచించు WMULT_IDLEPRIO		1431655765

బాహ్య స్థిరరాశి పూర్ణాంకం		sched_prio_గమ్యం_బరువు[40];
బాహ్య స్థిరరాశి u32		sched_prio_గమ్యం_wmult[40];

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

#నిర్వచించు వరుస_నుండి_తీయి_SLEEP		0x0001 /* Matches ENQUEUE_WAKEUP */
#నిర్వచించు వరుస_నుండి_తీయి_SAVE		0x0002 /* Matches ENQUEUE_RESTORE */
#నిర్వచించు వరుస_నుండి_తీయి_MOVE		0x0004 /* Matches ENQUEUE_MOVE */
#నిర్వచించు వరుస_నుండి_తీయి_NOCLOCK		0x0008 /* Matches ENQUEUE_NOCLOCK */

#నిర్వచించు వరుస_నుండి_తీయి_MIGRATING	0x0010 /* Matches ENQUEUE_MIGRATING */
#నిర్వచించు వరుస_నుండి_తీయి_DELAYED		0x0020 /* Matches ENQUEUE_DELAYED */
#నిర్వచించు వరుస_నుండి_తీయి_CLASS		0x0040 /* Matches ENQUEUE_CLASS */

#నిర్వచించు వరుస_నుండి_తీయి_SPECIAL		0x00010000
#నిర్వచించు వరుస_నుండి_తీయి_THROTTLE	0x00020000

#నిర్వచించు వరుసలో_చేర్చు_WAKEUP		0x0001
#నిర్వచించు వరుసలో_చేర్చు_RESTORE		0x0002
#నిర్వచించు వరుసలో_చేర్చు_MOVE		0x0004
#నిర్వచించు వరుసలో_చేర్చు_NOCLOCK		0x0008

#నిర్వచించు వరుసలో_చేర్చు_MIGRATING	0x0010
#నిర్వచించు వరుసలో_చేర్చు_DELAYED		0x0020
#నిర్వచించు వరుసలో_చేర్చు_CLASS		0x0040

#నిర్వచించు వరుసలో_చేర్చు_తల		0x00010000
#నిర్వచించు వరుసలో_చేర్చు_REPLENISH	0x00020000
#నిర్వచించు వరుసలో_చేర్చు_MIGRATED	0x00040000
#నిర్వచించు వరుసలో_చేర్చు_INITIAL		0x00080000
#నిర్వచించు వరుసలో_చేర్చు_RQ_SELECTED	0x00100000
#నిర్వచించు వరుసలో_చేర్చు_QUEUED		0x00200000

#నిర్వచించు RETRY_TASK		((శూన్య *)-1UL)

నిర్మాణం affinity_context {
	స్థిరరాశి నిర్మాణం cpumask	*new_ముసుగు;
	నిర్మాణం cpumask		*user_ముసుగు;
	గుర్తులేని పూర్ణాంకం		flags;
};

బాహ్య s64 update_curr_common(నిర్మాణం rq *rq);

గణన snt_e {
	SNT_సాధారణ,	/* set_next_task() */
	SNT_PICK,	/* put_prev_set_next_task(): prev != next */
	SNT_REPICK,	/* put_prev_set_next_task(): prev == next */
};

నిర్మాణం sched_class {

#నిర్వచితమైతే CONFIG_UCLAMP_TASK
	పూర్ణాంకం uclamp_enabled;
#అయితే_ముగింపు

	/*
	 * move_queued_task/activate_task/enqueue_task: rq->lock
	 * ttwu_do_activate/activate_task/enqueue_task: rq->lock
	 * wake_up_new_task/activate_task/enqueue_task: task_rq_lock
	 * ttwu_runnable/enqueue_task: task_rq_lock
	 * proxy_task_current: rq->lock
	 * sched_change_end
	 */
	శూన్య (*వరుసలో_చేర్చు_task) (నిర్మాణం rq *rq, నిర్మాణం task_struct *p, పూర్ణాంకం flags);
	/*
	 * move_queued_task/deactivate_task/dequeue_task: rq->lock
	 * __schedule/block_task/dequeue_task: rq->lock
	 * proxy_task_current: rq->lock
	 * wait_task_inactive: task_rq_lock
	 * sched_change_begin
	 */
	bool (*వరుస_నుండి_తీయి_task) (నిర్మాణం rq *rq, నిర్మాణం task_struct *p, పూర్ణాంకం flags);

	/*
	 * do_sched_yield: rq->lock
	 */
	శూన్య (*yield_task)   (నిర్మాణం rq *rq);
	/*
	 * yield_to: rq->lock (double)
	 */
	bool (*yield_గమ్యం_task)(నిర్మాణం rq *rq, నిర్మాణం task_struct *p);

	/*
	 * move_queued_task: rq->lock
	 * __migrate_swap_task: rq->lock
	 * ttwu_do_activate: rq->lock
	 * ttwu_runnable: task_rq_lock
	 * wake_up_new_task: task_rq_lock
	 */
	శూన్య (*wakeup_preempt)(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, పూర్ణాంకం flags);

	/*
	 * schedule/pick_next_task/prev_balance: rq->lock
	 */
	పూర్ణాంకం (*balance)(నిర్మాణం rq *rq, నిర్మాణం rq_flags *rf);

	/*
	 * schedule/pick_next_task: rq->lock
	 */
	నిర్మాణం task_struct *(*pick_task)(నిర్మాణం rq *rq, నిర్మాణం rq_flags *rf);

	/*
	 * sched_change:
	 * __schedule: rq->lock
	 */
	శూన్య (*put_prev_task)(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, నిర్మాణం task_struct *తదుపరి);
	శూన్య (*సమితి_తదుపరి_task)(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, గణన snt_e type);

	/*
	 * select_task_rq: p->pi_lock
	 * sched_exec: p->pi_lock
	 */
	పూర్ణాంకం  (*select_task_rq)(నిర్మాణం task_struct *p, పూర్ణాంకం task_cpu, పూర్ణాంకం flags);

	/*
	 * set_task_cpu: p->pi_lock || rq->lock (ttwu like)
	 */
	శూన్య (*migrate_task_rq)(నిర్మాణం task_struct *p, పూర్ణాంకం new_cpu);

	/*
	 * ttwu_do_activate: rq->lock
	 * wake_up_new_task: task_rq_lock
	 */
	శూన్య (*task_woken)(నిర్మాణం rq *this_rq, నిర్మాణం task_struct *task);

	/*
	 * do_set_cpus_allowed: task_rq_lock + sched_change
	 */
	శూన్య (*సమితి_cpus_allowed)(నిర్మాణం task_struct *p, నిర్మాణం affinity_context *ctx);

	/*
	 * sched_set_rq_{on,off}line: rq->lock
	 */
	శూన్య (*rq_online)(నిర్మాణం rq *rq);
	శూన్య (*rq_offline)(నిర్మాణం rq *rq);

	/*
	 * push_cpu_stop: p->pi_lock && rq->lock
	 */
	నిర్మాణం rq *(*కనుగొను_తాళం_rq)(నిర్మాణం task_struct *p, నిర్మాణం rq *rq);

	/*
	 * hrtick: rq->lock
	 * sched_tick: rq->lock
	 * sched_tick_remote: rq->lock
	 */
	శూన్య (*task_tick)(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, పూర్ణాంకం queued);
	/*
	 * sched_cgroup_fork: p->pi_lock
	 */
	శూన్య (*task_fork)(నిర్మాణం task_struct *p);
	/*
	 * finish_task_switch: no locks
	 */
	శూన్య (*task_dead)(నిర్మాణం task_struct *p);

	/*
	 * sched_change
	 */
	శూన్య (*switching_from)(నిర్మాణం rq *this_rq, నిర్మాణం task_struct *task);
	శూన్య (*switched_from) (నిర్మాణం rq *this_rq, నిర్మాణం task_struct *task);
	శూన్య (*switching_గమ్యం)  (నిర్మాణం rq *this_rq, నిర్మాణం task_struct *task);
	శూన్య (*switched_గమ్యం)   (నిర్మాణం rq *this_rq, నిర్మాణం task_struct *task);
	u64  (*get_prio)     (నిర్మాణం rq *this_rq, నిర్మాణం task_struct *task);
	శూన్య (*prio_మారింది) (నిర్మాణం rq *this_rq, నిర్మాణం task_struct *task,
			      u64 oldprio);

	/*
	 * set_load_weight: task_rq_lock + sched_change
	 * __setscheduler_parms: task_rq_lock + sched_change
	 */
	శూన్య (*reweight_task)(నిర్మాణం rq *this_rq, నిర్మాణం task_struct *task,
			      స్థిరరాశి నిర్మాణం ఎక్కించు_బరువు *lw);

	/*
	 * sched_rr_get_interval: task_rq_lock
	 */
	గుర్తులేని పూర్ణాంకం (*get_rr_interval)(నిర్మాణం rq *rq,
					నిర్మాణం task_struct *task);

	/*
	 * task_sched_runtime: task_rq_lock
	 */
	శూన్య (*update_curr)(నిర్మాణం rq *rq);

#నిర్వచితమైతే CONFIG_FAIR_సమూహం_SCHED
	/*
	 * sched_change_group: task_rq_lock + sched_change
	 */
	శూన్య (*task_change_సమూహం)(నిర్మాణం task_struct *p);
#అయితే_ముగింపు

#నిర్వచితమైతే CONFIG_SCHED_CORE
	/*
	 * pick_next_task: rq->lock
	 * try_steal_cookie: rq->lock (double)
	 */
	పూర్ణాంకం (*task_is_throttled)(నిర్మాణం task_struct *p, పూర్ణాంకం cpu);
#అయితే_ముగింపు
};

స్థిర అంతఃవరుస శూన్య put_prev_task(నిర్మాణం rq *rq, నిర్మాణం task_struct *prev)
{
	WARN_ON_ONCE(rq->donor != prev);
	prev->sched_class->put_prev_task(rq, prev, ఖాళీ);
}

స్థిర అంతఃవరుస శూన్య సమితి_తదుపరి_task(నిర్మాణం rq *rq, నిర్మాణం task_struct *తదుపరి)
{
	తదుపరి->sched_class->సమితి_తదుపరి_task(rq, తదుపరి, SNT_సాధారణ);
}

స్థిర అంతఃవరుస శూన్య
__put_prev_set_next_dl_server(నిర్మాణం rq *rq,
			      నిర్మాణం task_struct *prev,
			      నిర్మాణం task_struct *తదుపరి)
{
	prev->dl_server = ఖాళీ;
	తదుపరి->dl_server = rq->dl_server;
	rq->dl_server = ఖాళీ;
}

స్థిర అంతఃవరుస శూన్య put_prev_సమితి_తదుపరి_task(నిర్మాణం rq *rq,
					  నిర్మాణం task_struct *prev,
					  నిర్మాణం task_struct *తదుపరి)
{
	WARN_ON_ONCE(rq->donor != prev);

	__put_prev_set_next_dl_server(rq, prev, తదుపరి);

	ఒకవేళ (తదుపరి == prev) {
		తదుపరి->sched_class->సమితి_తదుపరి_task(rq, తదుపరి, SNT_REPICK);
		తిరిగి;
	}

	prev->sched_class->put_prev_task(rq, prev, తదుపరి);
	తదుపరి->sched_class->సమితి_తదుపరి_task(rq, తదుపరి, SNT_PICK);
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
#నిర్వచించు DEFINE_SCHED_CLASS(name) \
స్థిరరాశి నిర్మాణం sched_class name##_sched_class \
	__aligned(__alignof__(నిర్మాణం sched_class)) \
	__section("__" #name "_sched_class")

/* Defined in include/asm-generic/vmlinux.lds.h */
బాహ్య నిర్మాణం sched_class __sched_class_highest[];
బాహ్య నిర్మాణం sched_class __sched_class_lowest[];

బాహ్య స్థిరరాశి నిర్మాణం sched_class stop_sched_class;
బాహ్య స్థిరరాశి నిర్మాణం sched_class dl_sched_class;
బాహ్య స్థిరరాశి నిర్మాణం sched_class rt_sched_class;
బాహ్య స్థిరరాశి నిర్మాణం sched_class fair_sched_class;
బాహ్య స్థిరరాశి నిర్మాణం sched_class idle_sched_class;

/*
 * Iterate only active classes. SCX can take over all fair tasks or be
 * completely disabled. If the former, skip fair. If the latter, skip SCX.
 */
స్థిర అంతఃవరుస స్థిరరాశి నిర్మాణం sched_class *తదుపరి_చురుకైన_class(స్థిరరాశి నిర్మాణం sched_class *class)
{
	class++;
#నిర్వచితమైతే CONFIG_SCHED_CLASS_EXT
	ఒకవేళ (scx_switched_all() && class == &fair_sched_class)
		class++;
	ఒకవేళ (!scx_enabled() && class == &ext_sched_class)
		class++;
#అయితే_ముగింపు
	తిరిగి class;
}

#నిర్వచించు for_class_range(class, _from, గమ్యం_2) \
	కొరకు (class = (_from); class < (గమ్యం_2); class++)

#నిర్వచించు for_each_class(class) \
	for_class_range(class, __sched_class_highest, __sched_class_lowest)

#నిర్వచించు for_చురుకైన_class_range(class, _from, గమ్యం_2)				\
	కొరకు (class = (_from); class != (గమ్యం_2); class = తదుపరి_చురుకైన_class(class))

#నిర్వచించు for_each_చురుకైన_class(class)						\
	for_చురుకైన_class_range(class, __sched_class_highest, __sched_class_lowest)

#నిర్వచించు sched_class_above(_a, _b)	((_a) < (_b))

స్థిర అంతఃవరుస శూన్య rq_modified_begin(నిర్మాణం rq *rq, స్థిరరాశి నిర్మాణం sched_class *class)
{
	ఒకవేళ (sched_class_above(rq->తదుపరి_class, class))
		rq->తదుపరి_class = class;
}

స్థిర అంతఃవరుస bool rq_modified_above(నిర్మాణం rq *rq, స్థిరరాశి నిర్మాణం sched_class *class)
{
	తిరిగి sched_class_above(rq->తదుపరి_class, class);
}

స్థిర అంతఃవరుస bool sched_stop_runnable(నిర్మాణం rq *rq)
{
	తిరిగి rq->stop && task_on_rq_queued(rq->stop);
}

స్థిర అంతఃవరుస bool sched_dl_runnable(నిర్మాణం rq *rq)
{
	తిరిగి rq->dl.dl_nr_running > 0;
}

స్థిర అంతఃవరుస bool sched_rt_runnable(నిర్మాణం rq *rq)
{
	తిరిగి rq->rt.rt_queued > 0;
}

స్థిర అంతఃవరుస bool sched_fair_runnable(నిర్మాణం rq *rq)
{
	తిరిగి rq->cfs.nr_queued > 0;
}

బాహ్య నిర్మాణం task_struct *pick_task_fair(నిర్మాణం rq *rq, నిర్మాణం rq_flags *rf);
బాహ్య నిర్మాణం task_struct *pick_task_idle(నిర్మాణం rq *rq, నిర్మాణం rq_flags *rf);

#నిర్వచించు SCA_CHECK		0x01
#నిర్వచించు SCA_MIGRATE_DISABLE	0x02
#నిర్వచించు SCA_MIGRATE_ENABLE	0x04
#నిర్వచించు SCA_USER		0x08

బాహ్య శూన్య update_సమూహం_సామర్థ్యం(నిర్మాణం sched_domain *sd, పూర్ణాంకం cpu);

బాహ్య శూన్య sched_balance_trigger(నిర్మాణం rq *rq);

బాహ్య పూర్ణాంకం __set_cpus_allowed_ptr(నిర్మాణం task_struct *p, నిర్మాణం affinity_context *ctx);
బాహ్య శూన్య సమితి_cpus_allowed_common(నిర్మాణం task_struct *p, నిర్మాణం affinity_context *ctx);

స్థిర అంతఃవరుస bool task_allowed_on_cpu(నిర్మాణం task_struct *p, పూర్ణాంకం cpu)
{
	/* When not in the task's cpumask, no point in looking further. */
	ఒకవేళ (!cpumask_test_cpu(cpu, p->cpus_ptr))
		తిరిగి false;

	/* Can @cpu run a user thread? */
	ఒకవేళ (!(p->flags & PF_KTHREAD) && !task_cpu_possible(cpu, p))
		తిరిగి false;

	తిరిగి true;
}

స్థిర అంతఃవరుస cpumask_t *alloc_user_cpus_ptr(పూర్ణాంకం కణుపు)
{
	/*
	 * See set_cpus_allowed_force() above for the rcu_head usage.
	 */
	పూర్ణాంకం కొలత = max_t(పూర్ణాంకం, cpumask_కొలత(), పరిమాణం(నిర్మాణం rcu_తల));

	తిరిగి kmalloc_కణుపు(కొలత, GFP_KERNEL, కణుపు);
}

స్థిర అంతఃవరుస నిర్మాణం task_struct *get_నెట్టు_task(నిర్మాణం rq *rq)
{
	నిర్మాణం task_struct *p = rq->donor;

	lockdep_assert_rq_held(rq);

	ఒకవేళ (rq->నెట్టు_busy)
		తిరిగి ఖాళీ;

	ఒకవేళ (p->nr_cpus_allowed == 1)
		తిరిగి ఖాళీ;

	ఒకవేళ (p->migration_disabled)
		తిరిగి ఖాళీ;

	rq->నెట్టు_busy = true;
	తిరిగి get_task_struct(p);
}

బాహ్య పూర్ణాంకం నెట్టు_cpu_stop(శూన్య *arg);

#నిర్వచితమైతే CONFIG_CPU_IDLE

స్థిర అంతఃవరుస శూన్య idle_సమితి_state(నిర్మాణం rq *rq,
				  నిర్మాణం cpuidle_state *idle_state)
{
	rq->idle_state = idle_state;
}

స్థిర అంతఃవరుస నిర్మాణం cpuidle_state *idle_get_state(నిర్మాణం rq *rq)
{
	lockdep_assert(rcu_read_తాళం_any_held());

	తిరిగి rq->idle_state;
}

#లేకపోతే /* !CONFIG_CPU_IDLE: */

స్థిర అంతఃవరుస శూన్య idle_సమితి_state(నిర్మాణం rq *rq,
				  నిర్మాణం cpuidle_state *idle_state)
{
}

స్థిర అంతఃవరుస నిర్మాణం cpuidle_state *idle_get_state(నిర్మాణం rq *rq)
{
	తిరిగి ఖాళీ;
}

#అయితే_ముగింపు /* !CONFIG_CPU_IDLE */

బాహ్య శూన్య కాలపట్టిక_idle(శూన్య);
asmlinkage శూన్య కాలపట్టిక_user(శూన్య);

బాహ్య శూన్య sysrq_sched_debug_show(శూన్య);
బాహ్య శూన్య sched_init_granularity(శూన్య);
బాహ్య శూన్య update_max_interval(శూన్య);

బాహ్య శూన్య init_sched_dl_class(శూన్య);
బాహ్య శూన్య init_sched_rt_class(శూన్య);
బాహ్య శూన్య init_sched_fair_class(శూన్య);

బాహ్య శూన్య resched_curr(నిర్మాణం rq *rq);
బాహ్య శూన్య resched_curr_lazy(నిర్మాణం rq *rq);
బాహ్య శూన్య resched_cpu(పూర్ణాంకం cpu);

బాహ్య శూన్య init_rt_bandwidth(నిర్మాణం rt_bandwidth *rt_b, u64 period, u64 runtime);
బాహ్య bool sched_rt_bandwidth_account(నిర్మాణం rt_rq *rt_rq);

బాహ్య శూన్య init_dl_entity(నిర్మాణం sched_dl_entity *dl_se);

బాహ్య శూన్య init_cfs_throttle_work(నిర్మాణం task_struct *p);

#నిర్వచించు BW_జరుపు		20
#నిర్వచించు BW_UNIT			(1 << BW_జరుపు)
#నిర్వచించు నిష్పత్తి_జరుపు		8
#నిర్వచించు MAX_BW_బిట్లు		(64 - BW_జరుపు)
#నిర్వచించు MAX_BW			((1ULL << MAX_BW_బిట్లు) - 1)

బాహ్య u64 గమ్యం_నిష్పత్తి(u64 period, u64 runtime);

బాహ్య శూన్య init_entity_runnable_సరాసరి(నిర్మాణం sched_entity *se);
బాహ్య శూన్య post_init_entity_util_avg(నిర్మాణం task_struct *p);

#నిర్వచితమైతే CONFIG_NO_HZ_నిండిన
బాహ్య bool sched_can_stop_tick(నిర్మాణం rq *rq);
బాహ్య పూర్ణాంకం __init sched_tick_offload_init(శూన్య);

/*
 * Tick may be needed by tasks in the runqueue depending on their policy and
 * requirements. If tick is needed, lets send the target an IPI to kick it out of
 * nohz mode if necessary.
 */
స్థిర అంతఃవరుస శూన్య sched_update_tick_dependency(నిర్మాణం rq *rq)
{
	పూర్ణాంకం cpu = cpu_యొక్క(rq);

	ఒకవేళ (!tick_nohz_నిండిన_cpu(cpu))
		తిరిగి;

	ఒకవేళ (sched_can_stop_tick(rq))
		tick_nohz_dep_clear_cpu(cpu, TICK_DEP_బిట్టు_SCHED);
	లేకపోతే
		tick_nohz_dep_సమితి_cpu(cpu, TICK_DEP_బిట్టు_SCHED);
}
#లేకపోతే /* !CONFIG_NO_HZ_FULL: */
స్థిర అంతఃవరుస పూర్ణాంకం sched_tick_offload_init(శూన్య) { తిరిగి 0; }
స్థిర అంతఃవరుస శూన్య sched_update_tick_dependency(నిర్మాణం rq *rq) { }
#అయితే_ముగింపు /* !CONFIG_NO_HZ_FULL */

స్థిర అంతఃవరుస శూన్య కూడు_nr_running(నిర్మాణం rq *rq, గుర్తులేని లెక్క)
{
	గుర్తులేని prev_nr = rq->nr_running;

	rq->nr_running = prev_nr + లెక్క;
	ఒకవేళ (trace_sched_update_nr_running_tp_enabled()) {
		call_trace_sched_update_nr_running(rq, లెక్క);
	}

	ఒకవేళ (prev_nr < 2 && rq->nr_running >= 2)
		సమితి_rd_overloaded(rq->rd, 1);

	sched_update_tick_dependency(rq);
}

స్థిర అంతఃవరుస శూన్య sub_nr_running(నిర్మాణం rq *rq, గుర్తులేని లెక్క)
{
	rq->nr_running -= లెక్క;
	ఒకవేళ (trace_sched_update_nr_running_tp_enabled()) {
		call_trace_sched_update_nr_running(rq, -లెక్క);
	}

	/* Check if we still need preemption */
	sched_update_tick_dependency(rq);
}

స్థిర అంతఃవరుస శూన్య __block_task(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
	ఒకవేళ (p->sched_contributes_గమ్యం_ఎక్కించు)
		rq->nr_uninterruptible++;

	ఒకవేళ (p->లోపల_iowait) {
		atomic_inc(&rq->nr_iowait);
		delayacct_blkio_ప్రారంభం();
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

బాహ్య శూన్య activate_task(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, పూర్ణాంకం flags);
బాహ్య శూన్య deactivate_task(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, పూర్ణాంకం flags);

బాహ్య శూన్య wakeup_preempt(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, పూర్ణాంకం flags);

/*
 * attach_task() -- attach the task detached by detach_task() to its new rq.
 */
స్థిర అంతఃవరుస శూన్య attach_task(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
	lockdep_assert_rq_held(rq);

	WARN_ON_ONCE(task_rq(p) != rq);
	activate_task(rq, p, వరుసలో_చేర్చు_NOCLOCK);
	wakeup_preempt(rq, p, 0);
}

/*
 * attach_one_task() -- attaches the task returned from detach_one_task() to
 * its new rq.
 */
స్థిర అంతఃవరుస శూన్య attach_one_task(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
	guard(rq_తాళం)(rq);
	update_rq_clock(rq);
	attach_task(rq, p);
}

/*
 * __attach_tasks() - attaches a list of tasks (using se.group_node) to
 * the new rq
 */
స్థిర అంతఃవరుస శూన్య __attach_tasks(నిర్మాణం rq *rq, నిర్మాణం list_తల *tasks)
{
	guard(rq_తాళం)(rq);
	update_rq_clock(rq);

	అయితే (!list_ఖాళీలు(tasks)) {
		నిర్మాణం task_struct *p;

		p = list_మొదటి_నమోదు(tasks, నిర్మాణం task_struct, se.సమూహం_కణుపు);
		list_del_init(&p->se.సమూహం_కణుపు);

		attach_task(rq, p);
	}
}

#నిర్వచితమైతే CONFIG_PREEMPT_RT
# define SCHED_NR_MIGRATE_BREAK 8
#లేకపోతే
# define SCHED_NR_MIGRATE_BREAK 32
#అయితే_ముగింపు

బాహ్య __read_mostly గుర్తులేని పూర్ణాంకం sysctl_sched_nr_migrate;
బాహ్య __read_mostly గుర్తులేని పూర్ణాంకం sysctl_sched_migration_ఖర్చు;

బాహ్య గుర్తులేని పూర్ణాంకం sysctl_sched_పునాది_ముక్క;

బాహ్య పూర్ణాంకం sysctl_resched_latency_warn_ms;
బాహ్య పూర్ణాంకం sysctl_resched_latency_warn_once;

బాహ్య గుర్తులేని పూర్ణాంకం sysctl_sched_tunable_scaling;

బాహ్య గుర్తులేని పూర్ణాంకం sysctl_numa_balancing_పరిశీలన_delay;
బాహ్య గుర్తులేని పూర్ణాంకం sysctl_numa_balancing_పరిశీలన_period_min;
బాహ్య గుర్తులేని పూర్ణాంకం sysctl_numa_balancing_పరిశీలన_period_max;
బాహ్య గుర్తులేని పూర్ణాంకం sysctl_numa_balancing_పరిశీలన_కొలత;
బాహ్య గుర్తులేని పూర్ణాంకం sysctl_numa_balancing_hot_పరిమితి;

#నిర్వచితమైతే CONFIG_SCHED_HRTICK

/*
 * Use hrtick when:
 *  - enabled by features
 *  - hrtimer is actually high res
 */
స్థిర అంతఃవరుస bool hrtick_enabled(నిర్మాణం rq *rq)
{
	తిరిగి cpu_చురుకైన(cpu_యొక్క(rq)) && hrtimer_highres_enabled();
}

స్థిర అంతఃవరుస bool hrtick_enabled_fair(నిర్మాణం rq *rq)
{
	తిరిగి sched_feat(HRTICK) && hrtick_enabled(rq);
}

స్థిర అంతఃవరుస bool hrtick_enabled_dl(నిర్మాణం rq *rq)
{
	తిరిగి sched_feat(HRTICK_DL) && hrtick_enabled(rq);
}

బాహ్య శూన్య hrtick_ప్రారంభం(నిర్మాణం rq *rq, u64 delay);
స్థిర అంతఃవరుస bool hrtick_చురుకైన(నిర్మాణం rq *rq)
{
	తిరిగి hrtimer_చురుకైన(&rq->hrtick_timer);
}

#లేకపోతే /* !CONFIG_SCHED_HRTICK: */
స్థిర అంతఃవరుస bool hrtick_enabled_fair(నిర్మాణం rq *rq) { తిరిగి false; }
స్థిర అంతఃవరుస bool hrtick_enabled_dl(నిర్మాణం rq *rq) { తిరిగి false; }
స్థిర అంతఃవరుస bool hrtick_enabled(నిర్మాణం rq *rq) { తిరిగి false; }
#అయితే_ముగింపు /* !CONFIG_SCHED_HRTICK */

#నిర్వచితం_కాకపోతే arch_scale_freq_tick
స్థిర __always_inline శూన్య arch_scale_freq_tick(శూన్య) { }
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే arch_scale_freq_సామర్థ్యం
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
స్థిర __always_inline
గుర్తులేని దీర్ఘ arch_scale_freq_సామర్థ్యం(పూర్ణాంకం cpu)
{
	తిరిగి SCHED_సామర్థ్యం_SCALE;
}
#అయితే_ముగింపు

/*
 * In double_lock_balance()/double_rq_lock(), we use raw_spin_rq_lock() to
 * acquire rq lock instead of rq_lock(). So at the end of these two functions
 * we need to call double_rq_clock_clear_update() to clear RQCF_UPDATED of
 * rq->clock_update_flags to avoid the WARN_DOUBLE_CLOCK warning.
 */
స్థిర అంతఃవరుస శూన్య double_rq_clock_clear_update(నిర్మాణం rq *rq1, నిర్మాణం rq *rq2)
{
	rq1->clock_update_flags &= (RQCF_REQ_SKIP|RQCF_ACT_SKIP);
	rq2->clock_update_flags &= (RQCF_REQ_SKIP|RQCF_ACT_SKIP);
}

#నిర్వచించు DEFINE_తాళం_GUARD_2(name, type, తాళం_2, _unlock, ...)				\
__DEFINE_UNLOCK_GUARD(name, type, _unlock, type *lock2; __VA_ARGS__)			\
స్థిర అంతఃవరుస class_##name##_t class_##name##_constructor(type *తాళం, type *lock2)	\
	__no_context_analysis								\
{ class_##name##_t _t = { .తాళం = తాళం, .lock2 = lock2 }, *_T = &_t;			\
  తాళం_2; తిరిగి _t; }
#నిర్వచించు DECLARE_తాళం_GUARD_2_ATTRS(_name, తాళం_2, _unlock1, _unlock2)			\
స్థిర అంతఃవరుస class_##_name##_t class_##_name##_constructor(తాళం_3##_name##_t *_T1,	\
							    తాళం_3##_name##_t *_T2) తాళం_2; \
స్థిర __always_inline శూన్య __class_##_name##_cleanup_ctx1(class_##_name##_t **_T1)	\
	__no_context_analysis _unlock1 { }						\
స్థిర __always_inline శూన్య __class_##_name##_cleanup_ctx2(class_##_name##_t **_T2)	\
	__no_context_analysis _unlock2 { }
#నిర్వచించు WITH_తాళం_GUARD_2_ATTRS(_name, _T1, _T2)					\
	class_##_name##_constructor(_T1, _T2),						\
	*__UNIQUE_ID(unlock1) __cleanup(__class_##_name##_cleanup_ctx1) = (శూన్య *)(_T1),\
	*__UNIQUE_ID(unlock2) __cleanup(__class_##_name##_cleanup_ctx2) = (శూన్య *)(_T2)

స్థిర అంతఃవరుస bool rq_క్రమం_less(నిర్మాణం rq *rq1, నిర్మాణం rq *rq2)
{
#నిర్వచితమైతే CONFIG_SCHED_CORE
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
	ఒకవేళ (rq1->core->cpu < rq2->core->cpu)
		తిరిగి true;
	ఒకవేళ (rq1->core->cpu > rq2->core->cpu)
		తిరిగి false;

	/*
	 * __sched_core_flip() relies on SMT having cpu-id lock order.
	 */
#అయితే_ముగింపు /* CONFIG_SCHED_CORE */
	తిరిగి rq1->cpu < rq2->cpu;
}

బాహ్య శూన్య double_rq_తాళం(నిర్మాణం rq *rq1, నిర్మాణం rq *rq2)
	__acquires(__rq_lockp(rq1), __rq_lockp(rq2));

#నిర్వచితమైతే CONFIG_PREEMPTION

/*
 * fair double_lock_balance: Safely acquires both rq->locks in a fair
 * way at the expense of forcing extra atomic operations in all
 * invocations.  This assures that the double_lock is acquired using the
 * same underlying policy as the spinlock_t on this architecture, which
 * reduces latency compared to the unfair variant below.  However, it
 * also adds more overhead and therefore may reduce throughput.
 */
స్థిర అంతఃవరుస పూర్ణాంకం double_తాళం_balance_2(నిర్మాణం rq *this_rq, నిర్మాణం rq *busiest)
	__must_hold(__rq_lockp(this_rq))
	__acquires(__rq_lockp(busiest))
{
	raw_spin_rq_unlock(this_rq);
	double_rq_తాళం(this_rq, busiest);

	తిరిగి 1;
}

#లేకపోతే /* !CONFIG_PREEMPTION: */
/*
 * Unfair double_lock_balance: Optimizes throughput at the expense of
 * latency by eliminating extra atomic operations when the locks are
 * already in proper order on entry.  This favors lower CPU-ids and will
 * grant the double lock to lower CPUs over higher ids under contention,
 * regardless of entry order into the function.
 */
స్థిర అంతఃవరుస పూర్ణాంకం double_తాళం_balance_2(నిర్మాణం rq *this_rq, నిర్మాణం rq *busiest)
	__must_hold(__rq_lockp(this_rq))
	__acquires(__rq_lockp(busiest))
{
	ఒకవేళ (__rq_lockp(this_rq) == __rq_lockp(busiest)) {
		__acquire(__rq_lockp(busiest)); /* already held */
		double_rq_clock_clear_update(this_rq, busiest);
		తిరిగి 0;
	}

	ఒకవేళ (likely(raw_spin_rq_trylock(busiest))) {
		double_rq_clock_clear_update(this_rq, busiest);
		తిరిగి 0;
	}

	ఒకవేళ (rq_క్రమం_less(this_rq, busiest)) {
		raw_spin_rq_తాళం_nested(busiest, SINGLE_లోతు_NESTING);
		double_rq_clock_clear_update(this_rq, busiest);
		తిరిగి 0;
	}

	raw_spin_rq_unlock(this_rq);
	double_rq_తాళం(this_rq, busiest);

	తిరిగి 1;
}

#అయితే_ముగింపు /* !CONFIG_PREEMPTION */

/*
 * double_lock_balance - lock the busiest runqueue, this_rq is locked already.
 */
స్థిర అంతఃవరుస పూర్ణాంకం double_తాళం_balance(నిర్మాణం rq *this_rq, నిర్మాణం rq *busiest)
	__must_hold(__rq_lockp(this_rq))
	__acquires(__rq_lockp(busiest))
{
	lockdep_assert_irqs_disabled();

	తిరిగి double_తాళం_balance_2(this_rq, busiest);
}

స్థిర అంతఃవరుస శూన్య double_unlock_balance(నిర్మాణం rq *this_rq, నిర్మాణం rq *busiest)
	__releases(__rq_lockp(busiest))
{
	ఒకవేళ (__rq_lockp(this_rq) != __rq_lockp(busiest))
		raw_spin_rq_unlock(busiest);
	లేకపోతే
		__release(__rq_lockp(busiest)); /* fake release */
	తాళం_సమితి_subclass(&__rq_lockp(this_rq)->dep_map, 0, _RET_IP_);
}

స్థిర అంతఃవరుస శూన్య double_తాళం(spinlock_t *l1, spinlock_t *l2)
	__acquires(l1, l2)
{
	ఒకవేళ (l1 > l2)
		మార్పిడి(l1, l2);

	spin_తాళం(l1);
	spin_తాళం_nested(l2, SINGLE_లోతు_NESTING);
}

స్థిర అంతఃవరుస శూన్య double_తాళం_irq(spinlock_t *l1, spinlock_t *l2)
	__acquires(l1, l2)
{
	ఒకవేళ (l1 > l2)
		మార్పిడి(l1, l2);

	spin_తాళం_irq(l1);
	spin_తాళం_nested(l2, SINGLE_లోతు_NESTING);
}

స్థిర అంతఃవరుస శూన్య double_raw_తాళం(raw_spinlock_t *l1, raw_spinlock_t *l2)
	__acquires(l1, l2)
{
	ఒకవేళ (l1 > l2)
		మార్పిడి(l1, l2);

	raw_spin_తాళం(l1);
	raw_spin_తాళం_nested(l2, SINGLE_లోతు_NESTING);
}

స్థిర అంతఃవరుస శూన్య double_raw_unlock(raw_spinlock_t *l1, raw_spinlock_t *l2)
	__releases(l1, l2)
{
	raw_spin_unlock(l1);
	raw_spin_unlock(l2);
}

DEFINE_తాళం_GUARD_2(double_raw_spinlock, raw_spinlock_t,
		    double_raw_తాళం(_T->తాళం, _T->lock2),
		    double_raw_unlock(_T->తాళం, _T->lock2))

DECLARE_తాళం_GUARD_2_ATTRS(double_raw_spinlock,
			   __acquires(_T1, _T2),
			   __releases(*(raw_spinlock_t **)_T1),
			   __releases(*(raw_spinlock_t **)_T2));
#నిర్వచించు class_double_raw_spinlock_constructor(_T1, _T2) \
	WITH_తాళం_GUARD_2_ATTRS(double_raw_spinlock, _T1, _T2)

/*
 * double_rq_unlock - safely unlock two runqueues
 *
 * Note this does not restore interrupts like task_rq_unlock,
 * you need to do so manually after calling.
 */
స్థిర అంతఃవరుస శూన్య double_rq_unlock(నిర్మాణం rq *rq1, నిర్మాణం rq *rq2)
	__releases(__rq_lockp(rq1), __rq_lockp(rq2))
{
	ఒకవేళ (__rq_lockp(rq1) != __rq_lockp(rq2))
		raw_spin_rq_unlock(rq2);
	లేకపోతే
		__release(__rq_lockp(rq2)); /* fake release */
	raw_spin_rq_unlock(rq1);
}

బాహ్య శూన్య సమితి_rq_online (నిర్మాణం rq *rq);
బాహ్య శూన్య సమితి_rq_offline(నిర్మాణం rq *rq);

బాహ్య bool sched_smp_initialized;

DEFINE_తాళం_GUARD_2(double_rq_తాళం, నిర్మాణం rq,
		    double_rq_తాళం(_T->తాళం, _T->lock2),
		    double_rq_unlock(_T->తాళం, _T->lock2))

బాహ్య నిర్మాణం sched_entity *__pick_root_entity(నిర్మాణం cfs_rq *cfs_rq);
బాహ్య నిర్మాణం sched_entity *__pick_first_entity(నిర్మాణం cfs_rq *cfs_rq);
బాహ్య నిర్మాణం sched_entity *__pick_last_entity(నిర్మాణం cfs_rq *cfs_rq);

బాహ్య bool sched_debug_verbose;

బాహ్య శూన్య print_cfs_stats(నిర్మాణం seq_file *m, పూర్ణాంకం cpu);
బాహ్య శూన్య print_rt_stats(నిర్మాణం seq_file *m, పూర్ణాంకం cpu);
బాహ్య శూన్య print_dl_stats(నిర్మాణం seq_file *m, పూర్ణాంకం cpu);
బాహ్య శూన్య print_cfs_rq(నిర్మాణం seq_file *m, పూర్ణాంకం cpu, నిర్మాణం cfs_rq *cfs_rq);
బాహ్య శూన్య print_rt_rq(నిర్మాణం seq_file *m, పూర్ణాంకం cpu, నిర్మాణం rt_rq *rt_rq);
బాహ్య శూన్య print_dl_rq(నిర్మాణం seq_file *m, పూర్ణాంకం cpu, నిర్మాణం dl_rq *dl_rq);

బాహ్య శూన్య resched_latency_warn(పూర్ణాంకం cpu, u64 latency);

#నిర్వచితమైతే CONFIG_NUMA_BALANCING
బాహ్య శూన్య show_numa_stats(నిర్మాణం task_struct *p, నిర్మాణం seq_file *m);
బాహ్య శూన్య
print_numa_stats(నిర్మాణం seq_file *m, పూర్ణాంకం కణుపు, గుర్తులేని దీర్ఘ tsf,
		 గుర్తులేని దీర్ఘ tpf, గుర్తులేని దీర్ఘ gsf, గుర్తులేని దీర్ఘ gpf);
#అయితే_ముగింపు /* CONFIG_NUMA_BALANCING */

బాహ్య శూన్య init_cfs_rq(నిర్మాణం cfs_rq *cfs_rq);
బాహ్య శూన్య init_rt_rq(నిర్మాణం rt_rq *rt_rq);
బాహ్య శూన్య init_dl_rq(నిర్మాణం dl_rq *dl_rq);

బాహ్య శూన్య cfs_bandwidth_usage_inc(శూన్య);
బాహ్య శూన్య cfs_bandwidth_usage_dec(శూన్య);

#నిర్వచితమైతే CONFIG_NO_HZ_COMMON

#నిర్వచించు NOHZ_BALANCE_KICK_బిట్టు	0
#నిర్వచించు NOHZ_STATS_KICK_బిట్టు	1
#నిర్వచించు NOHZ_NEWILB_KICK_బిట్టు	2
#నిర్వచించు NOHZ_తదుపరి_KICK_బిట్టు	3

/* Run sched_balance_domains() */
#నిర్వచించు NOHZ_BALANCE_KICK	బిట్టు(NOHZ_BALANCE_KICK_బిట్టు)
/* Update blocked load */
#నిర్వచించు NOHZ_STATS_KICK		బిట్టు(NOHZ_STATS_KICK_బిట్టు)
/* Update blocked load when entering idle */
#నిర్వచించు NOHZ_NEWILB_KICK	బిట్టు(NOHZ_NEWILB_KICK_బిట్టు)
/* Update nohz.next_balance */
#నిర్వచించు NOHZ_తదుపరి_KICK		బిట్టు(NOHZ_తదుపరి_KICK_బిట్టు)

#నిర్వచించు NOHZ_KICK_ముసుగు		(NOHZ_BALANCE_KICK | NOHZ_STATS_KICK | NOHZ_తదుపరి_KICK)

#నిర్వచించు nohz_flags(cpu)		(&cpu_rq(cpu)->nohz_flags)

బాహ్య శూన్య nohz_balance_exit_idle(నిర్మాణం rq *rq);
#లేకపోతే /* !CONFIG_NO_HZ_COMMON: */
స్థిర అంతఃవరుస శూన్య nohz_balance_exit_idle(నిర్మాణం rq *rq) { }
#అయితే_ముగింపు /* !CONFIG_NO_HZ_COMMON */

#నిర్వచితమైతే CONFIG_NO_HZ_COMMON
బాహ్య శూన్య nohz_నిరంతరం_idle_balance(పూర్ణాంకం cpu);
#లేకపోతే
స్థిర అంతఃవరుస శూన్య nohz_నిరంతరం_idle_balance(పూర్ణాంకం cpu) { }
#అయితే_ముగింపు

#అన్తర్భావ "stats.h"

#ఒకవేళ defined(CONFIG_SCHED_CORE) && defined(CONFIG_SCHEDSTATS)

బాహ్య శూన్య __sched_core_account_forceidle(నిర్మాణం rq *rq);

స్థిర అంతఃవరుస శూన్య sched_core_account_forceidle(నిర్మాణం rq *rq)
{
	ఒకవేళ (schedstat_enabled())
		__sched_core_account_forceidle(rq);
}

బాహ్య శూన్య __sched_core_tick(నిర్మాణం rq *rq);

స్థిర అంతఃవరుస శూన్య sched_core_tick(నిర్మాణం rq *rq)
{
	ఒకవేళ (sched_core_enabled(rq) && schedstat_enabled())
		__sched_core_tick(rq);
}

#లేకపోతే /* !(CONFIG_SCHED_CORE && CONFIG_SCHEDSTATS): */

స్థిర అంతఃవరుస శూన్య sched_core_account_forceidle(నిర్మాణం rq *rq) { }

స్థిర అంతఃవరుస శూన్య sched_core_tick(నిర్మాణం rq *rq) { }

#అయితే_ముగింపు /* !(CONFIG_SCHED_CORE && CONFIG_SCHEDSTATS) */

#నిర్వచితమైతే CONFIG_IRQ_సమయం_ACCOUNTING

నిర్మాణం irqtime {
	u64			మొత్తం;
	u64			tick_delta;
	u64			irq_ప్రారంభం_సమయం;
	నిర్మాణం u64_stats_sync	sync;
};

DECLARE_చొప్పున_CPU(నిర్మాణం irqtime, cpu_irqtime);
DECLARE_STATIC_కీలకం_FALSE(sched_clock_irqtime);

స్థిర అంతఃవరుస పూర్ణాంకం irqtime_enabled(శూన్య)
{
	తిరిగి static_branch_likely(&sched_clock_irqtime);
}

/*
 * Returns the irqtime minus the softirq time computed by ksoftirqd.
 * Otherwise ksoftirqd's sum_exec_runtime is subtracted its own runtime
 * and never move forward.
 */
స్థిర అంతఃవరుస u64 irq_సమయం_read(పూర్ణాంకం cpu)
{
	నిర్మాణం irqtime *irqtime = &చొప్పున_cpu(cpu_irqtime, cpu);
	గుర్తులేని పూర్ణాంకం seq;
	u64 మొత్తం;

	చేయి {
		seq = __u64_stats_fetch_begin(&irqtime->sync);
		మొత్తం = irqtime->మొత్తం;
	} అయితే (__u64_stats_fetch_retry(&irqtime->sync, seq));

	తిరిగి మొత్తం;
}

#లేకపోతే /* !CONFIG_IRQ_TIME_ACCOUNTING: */

స్థిర అంతఃవరుస పూర్ణాంకం irqtime_enabled(శూన్య)
{
	తిరిగి 0;
}

#అయితే_ముగింపు /* !CONFIG_IRQ_TIME_ACCOUNTING */

#నిర్వచితమైతే CONFIG_CPU_FREQ

DECLARE_చొప్పున_CPU(నిర్మాణం update_util_దత్తాంశం __rcu *, cpufreq_update_util_దత్తాంశం);

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
స్థిర అంతఃవరుస శూన్య cpufreq_update_util(నిర్మాణం rq *rq, గుర్తులేని పూర్ణాంకం flags)
{
	నిర్మాణం update_util_దత్తాంశం *దత్తాంశం;

	దత్తాంశం = rcu_dereference_sched(*చొప్పున_cpu_ptr(&cpufreq_update_util_దత్తాంశం,
						  cpu_యొక్క(rq)));
	ఒకవేళ (దత్తాంశం)
		దత్తాంశం->func(దత్తాంశం, rq_clock(rq), flags);
}
#లేకపోతే /* !CONFIG_CPU_FREQ: */
స్థిర అంతఃవరుస శూన్య cpufreq_update_util(నిర్మాణం rq *rq, గుర్తులేని పూర్ణాంకం flags) { }
#అయితే_ముగింపు /* !CONFIG_CPU_FREQ */

#నిర్వచితమైతే arch_scale_freq_సామర్థ్యం
# ifndef arch_scale_freq_invariant
#  define arch_scale_freq_invariant()	true
# endif
#లేకపోతే
# define arch_scale_freq_invariant()	false
#అయితే_ముగింపు

గుర్తులేని దీర్ఘ effective_cpu_util(పూర్ణాంకం cpu, గుర్తులేని దీర్ఘ util_cfs,
				 గుర్తులేని దీర్ఘ *min,
				 గుర్తులేని దీర్ఘ *max);

గుర్తులేని దీర్ఘ sugov_effective_cpu_perf(పూర్ణాంకం cpu, గుర్తులేని దీర్ఘ actual,
				 గుర్తులేని దీర్ఘ min,
				 గుర్తులేని దీర్ఘ max);


/*
 * Verify the fitness of task @p to run on @cpu taking into account the
 * CPU original capacity and the runtime/deadline ratio of the task.
 *
 * The function will return true if the original capacity of @cpu is
 * greater than or equal to task's deadline density right shifted by
 * (BW_SHIFT - SCHED_CAPACITY_SHIFT) and false otherwise.
 */
స్థిర అంతఃవరుస bool dl_task_సరిపోతుంది_సామర్థ్యం(నిర్మాణం task_struct *p, పూర్ణాంకం cpu)
{
	గుర్తులేని దీర్ఘ cap = arch_scale_cpu_సామర్థ్యం(cpu);

	తిరిగి cap >= p->dl.dl_density >> (BW_జరుపు - SCHED_సామర్థ్యం_జరుపు);
}

స్థిర అంతఃవరుస గుర్తులేని దీర్ఘ cpu_bw_dl(నిర్మాణం rq *rq)
{
	తిరిగి (rq->dl.running_bw * SCHED_సామర్థ్యం_SCALE) >> BW_జరుపు;
}

స్థిర అంతఃవరుస గుర్తులేని దీర్ఘ cpu_util_dl(నిర్మాణం rq *rq)
{
	తిరిగి READ_ONCE(rq->avg_dl.util_avg);
}


బాహ్య గుర్తులేని దీర్ఘ cpu_util_cfs(పూర్ణాంకం cpu);
బాహ్య గుర్తులేని దీర్ఘ cpu_util_cfs_boost(పూర్ణాంకం cpu);

స్థిర అంతఃవరుస గుర్తులేని దీర్ఘ cpu_util_rt(నిర్మాణం rq *rq)
{
	తిరిగి READ_ONCE(rq->avg_rt.util_avg);
}

#నిర్వచితమైతే CONFIG_UCLAMP_TASK

గుర్తులేని దీర్ఘ uclamp_eff_విలువ(నిర్మాణం task_struct *p, గణన uclamp_id clamp_id);

/*
 * When uclamp is compiled in, the aggregation at rq level is 'turned off'
 * by default in the fast path and only gets turned on once userspace performs
 * an operation that requires it.
 *
 * Returns true if userspace opted-in to use uclamp and aggregation at rq level
 * hence is active.
 */
స్థిర అంతఃవరుస bool uclamp_is_వాడిన(శూన్య)
{
	తిరిగి static_branch_likely(&sched_uclamp_వాడిన);
}

/*
 * Enabling static branches would get the cpus_read_lock(),
 * check whether uclamp_is_used before enable it to avoid always
 * calling cpus_read_lock(). Because we never disable this
 * static key once enable it.
 */
స్థిర అంతఃవరుస శూన్య sched_uclamp_enable(శూన్య)
{
	ఒకవేళ (!uclamp_is_వాడిన())
		static_branch_enable(&sched_uclamp_వాడిన);
}

స్థిర అంతఃవరుస గుర్తులేని దీర్ఘ uclamp_rq_get(నిర్మాణం rq *rq,
					  గణన uclamp_id clamp_id)
{
	తిరిగి READ_ONCE(rq->uclamp[clamp_id].విలువ);
}

స్థిర అంతఃవరుస శూన్య uclamp_rq_సమితి(నిర్మాణం rq *rq, గణన uclamp_id clamp_id,
				 గుర్తులేని పూర్ణాంకం విలువ)
{
	WRITE_ONCE(rq->uclamp[clamp_id].విలువ, విలువ);
}

స్థిర అంతఃవరుస bool uclamp_rq_is_idle(నిర్మాణం rq *rq)
{
	తిరిగి rq->uclamp_flags & UCLAMP_FLAG_IDLE;
}

/* Is the rq being capped/throttled by uclamp_max? */
స్థిర అంతఃవరుస bool uclamp_rq_is_capped(నిర్మాణం rq *rq)
{
	గుర్తులేని దీర్ఘ rq_util;
	గుర్తులేని దీర్ఘ max_util;

	ఒకవేళ (!uclamp_is_వాడిన())
		తిరిగి false;

	rq_util = cpu_util_cfs(cpu_యొక్క(rq)) + cpu_util_rt(rq);
	max_util = READ_ONCE(rq->uclamp[UCLAMP_MAX].విలువ);

	తిరిగి max_util != SCHED_సామర్థ్యం_SCALE && rq_util >= max_util;
}

#నిర్వచించు for_each_clamp_id(clamp_id) \
	కొరకు ((clamp_id) = 0; (clamp_id) < UCLAMP_CNT; (clamp_id)++)

బాహ్య గుర్తులేని పూర్ణాంకం sysctl_sched_uclamp_util_min_rt_default;


స్థిర అంతఃవరుస గుర్తులేని పూర్ణాంకం uclamp_none(గణన uclamp_id clamp_id)
{
	ఒకవేళ (clamp_id == UCLAMP_MIN)
		తిరిగి 0;
	తిరిగి SCHED_సామర్థ్యం_SCALE;
}

/* Integer rounded range for each bucket */
#నిర్వచించు UCLAMP_పాత్ర_DELTA DIV_విడత_CLOSEST(SCHED_సామర్థ్యం_SCALE, UCLAMP_BUCKETS)

స్థిర అంతఃవరుస గుర్తులేని పూర్ణాంకం uclamp_పాత్ర_id(గుర్తులేని పూర్ణాంకం clamp_విలువ)
{
	తిరిగి min_t(గుర్తులేని పూర్ణాంకం, clamp_విలువ / UCLAMP_పాత్ర_DELTA, UCLAMP_BUCKETS - 1);
}

స్థిర అంతఃవరుస శూన్య
uclamp_se_సమితి(నిర్మాణం uclamp_se *uc_se, గుర్తులేని పూర్ణాంకం విలువ, bool user_defined)
{
	uc_se->విలువ = విలువ;
	uc_se->పాత్ర_id = uclamp_పాత్ర_id(విలువ);
	uc_se->user_defined = user_defined;
}

#లేకపోతే /* !CONFIG_UCLAMP_TASK: */

స్థిర అంతఃవరుస గుర్తులేని దీర్ఘ
uclamp_eff_విలువ(నిర్మాణం task_struct *p, గణన uclamp_id clamp_id)
{
	ఒకవేళ (clamp_id == UCLAMP_MIN)
		తిరిగి 0;

	తిరిగి SCHED_సామర్థ్యం_SCALE;
}

స్థిర అంతఃవరుస bool uclamp_rq_is_capped(నిర్మాణం rq *rq) { తిరిగి false; }

స్థిర అంతఃవరుస bool uclamp_is_వాడిన(శూన్య)
{
	తిరిగి false;
}

స్థిర అంతఃవరుస శూన్య sched_uclamp_enable(శూన్య) {}

స్థిర అంతఃవరుస గుర్తులేని దీర్ఘ
uclamp_rq_get(నిర్మాణం rq *rq, గణన uclamp_id clamp_id)
{
	ఒకవేళ (clamp_id == UCLAMP_MIN)
		తిరిగి 0;

	తిరిగి SCHED_సామర్థ్యం_SCALE;
}

స్థిర అంతఃవరుస శూన్య
uclamp_rq_సమితి(నిర్మాణం rq *rq, గణన uclamp_id clamp_id, గుర్తులేని పూర్ణాంకం విలువ)
{
}

స్థిర అంతఃవరుస bool uclamp_rq_is_idle(నిర్మాణం rq *rq)
{
	తిరిగి false;
}

#అయితే_ముగింపు /* !CONFIG_UCLAMP_TASK */

#నిర్వచితమైతే CONFIG_HAVE_SCHED_AVG_IRQ

స్థిర అంతఃవరుస గుర్తులేని దీర్ఘ cpu_util_irq(నిర్మాణం rq *rq)
{
	తిరిగి READ_ONCE(rq->avg_irq.util_avg);
}

స్థిర అంతఃవరుస
గుర్తులేని దీర్ఘ scale_irq_సామర్థ్యం(గుర్తులేని దీర్ఘ util, గుర్తులేని దీర్ఘ irq, గుర్తులేని దీర్ఘ max)
{
	util *= (max - irq);
	util /= max;

	తిరిగి util;

}

#లేకపోతే /* !CONFIG_HAVE_SCHED_AVG_IRQ: */

స్థిర అంతఃవరుస గుర్తులేని దీర్ఘ cpu_util_irq(నిర్మాణం rq *rq)
{
	తిరిగి 0;
}

స్థిర అంతఃవరుస
గుర్తులేని దీర్ఘ scale_irq_సామర్థ్యం(గుర్తులేని దీర్ఘ util, గుర్తులేని దీర్ఘ irq, గుర్తులేని దీర్ఘ max)
{
	తిరిగి util;
}

#అయితే_ముగింపు /* !CONFIG_HAVE_SCHED_AVG_IRQ */

బాహ్య శూన్య __setparam_fair(నిర్మాణం task_struct *p, స్థిరరాశి నిర్మాణం sched_attr *attr);

#ఒకవేళ defined(CONFIG_ENERGY_MODEL) && defined(CONFIG_CPU_FREQ_GOV_SCHEDUTIL)

#నిర్వచించు perf_domain_పరిధి(pd) (గమ్యం_cpumask(((pd)->em_pd->cpus)))

DECLARE_STATIC_కీలకం_FALSE(sched_energy_present);

స్థిర అంతఃవరుస bool sched_energy_enabled(శూన్య)
{
	తిరిగి static_branch_unlikely(&sched_energy_present);
}

#లేకపోతే /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL): */

#నిర్వచించు perf_domain_పరిధి(pd) ఖాళీ

స్థిర అంతఃవరుస bool sched_energy_enabled(శూన్య) { తిరిగి false; }

#అయితే_ముగింపు /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL) */

#నిర్వచితమైతే CONFIG_MEMBARRIER

/*
 * The scheduler provides memory barriers required by membarrier between:
 * - prior user-space memory accesses and store to rq->membarrier_state,
 * - store to rq->membarrier_state and following user-space memory accesses.
 * In the same way it provides those guarantees around store to rq->curr.
 */
స్థిర అంతఃవరుస శూన్య membarrier_switch_mm(నిర్మాణం rq *rq,
					నిర్మాణం mm_struct *prev_mm,
					నిర్మాణం mm_struct *తదుపరి_mm)
{
	పూర్ణాంకం membarrier_state;

	ఒకవేళ (prev_mm == తదుపరి_mm)
		తిరిగి;

	membarrier_state = atomic_read(&తదుపరి_mm->membarrier_state);
	ఒకవేళ (READ_ONCE(rq->membarrier_state) == membarrier_state)
		తిరిగి;

	WRITE_ONCE(rq->membarrier_state, membarrier_state);
}

#లేకపోతే /* !CONFIG_MEMBARRIER: */

స్థిర అంతఃవరుస శూన్య membarrier_switch_mm(నిర్మాణం rq *rq,
					నిర్మాణం mm_struct *prev_mm,
					నిర్మాణం mm_struct *తదుపరి_mm)
{
}

#అయితే_ముగింపు /* !CONFIG_MEMBARRIER */

స్థిర అంతఃవరుస bool is_చొప్పున_cpu_kthread(నిర్మాణం task_struct *p)
{
	ఒకవేళ (!(p->flags & PF_KTHREAD))
		తిరిగి false;

	ఒకవేళ (p->nr_cpus_allowed != 1)
		తిరిగి false;

	తిరిగి true;
}

బాహ్య శూన్య swake_పైకి_all_locked(నిర్మాణం swait_వరుస_తల *q);
బాహ్య శూన్య __prepare_to_swait(నిర్మాణం swait_వరుస_తల *q, నిర్మాణం swait_వరుస *wait);

బాహ్య పూర్ణాంకం try_గమ్యం_wake_పైకి(నిర్మాణం task_struct *tsk, గుర్తులేని పూర్ణాంకం state, పూర్ణాంకం wake_flags);

#నిర్వచితమైతే CONFIG_PREEMPT_DYNAMIC
బాహ్య పూర్ణాంకం preempt_dynamic_బహుళకం;
బాహ్య పూర్ణాంకం sched_dynamic_బహుళకం(స్థిరరాశి అక్షరం *str);
బాహ్య శూన్య sched_dynamic_update(పూర్ణాంకం బహుళకం);
#అయితే_ముగింపు
బాహ్య స్థిరరాశి అక్షరం *preempt_modes[];

#నిర్వచితమైతే CONFIG_SCHED_MM_CID

స్థిర __always_inline bool cid_on_cpu(గుర్తులేని పూర్ణాంకం cid)
{
	తిరిగి cid & MM_CID_ONCPU;
}

స్థిర __always_inline bool cid_లోపల_transit(గుర్తులేని పూర్ణాంకం cid)
{
	తిరిగి cid & MM_CID_TRANSIT;
}

స్థిర __always_inline గుర్తులేని పూర్ణాంకం cpu_cid_గమ్యం_cid(గుర్తులేని పూర్ణాంకం cid)
{
	తిరిగి cid & ~MM_CID_ONCPU;
}

స్థిర __always_inline గుర్తులేని పూర్ణాంకం cid_గమ్యం_cpu_cid(గుర్తులేని పూర్ణాంకం cid)
{
	తిరిగి cid | MM_CID_ONCPU;
}

స్థిర __always_inline గుర్తులేని పూర్ణాంకం cid_గమ్యం_transit_cid(గుర్తులేని పూర్ణాంకం cid)
{
	తిరిగి cid | MM_CID_TRANSIT;
}

స్థిర __always_inline గుర్తులేని పూర్ణాంకం cid_from_transit_cid(గుర్తులేని పూర్ణాంకం cid)
{
	తిరిగి cid & ~MM_CID_TRANSIT;
}

స్థిర __always_inline bool cid_on_task(గుర్తులేని పూర్ణాంకం cid)
{
	/* True if none of the MM_CID_ONCPU, MM_CID_TRANSIT, MM_CID_UNSET bits is set */
	తిరిగి cid < MM_CID_TRANSIT;
}

స్థిర __always_inline శూన్య mm_drop_cid(నిర్మాణం mm_struct *mm, గుర్తులేని పూర్ణాంకం cid)
{
	clear_బిట్టు(cid, mm_cidmask(mm));
}

స్థిర __always_inline శూన్య mm_unset_cid_on_task(నిర్మాణం task_struct *t)
{
	గుర్తులేని పూర్ణాంకం cid = t->mm_cid.cid;

	t->mm_cid.cid = MM_CID_UNSET;
	ఒకవేళ (cid_on_task(cid))
		mm_drop_cid(t->mm, cid);
}

స్థిర __always_inline శూన్య mm_drop_cid_on_cpu(నిర్మాణం mm_struct *mm, నిర్మాణం mm_cid_pcpu *pcp)
{
	/* Clear the ONCPU bit, but do not set UNSET in the per CPU storage */
	ఒకవేళ (cid_on_cpu(pcp->cid)) {
		pcp->cid = cpu_cid_గమ్యం_cid(pcp->cid);
		mm_drop_cid(mm, pcp->cid);
	}
}

స్థిర అంతఃవరుస గుర్తులేని పూర్ణాంకం __mm_get_cid(నిర్మాణం mm_struct *mm, గుర్తులేని పూర్ణాంకం max_cids)
{
	గుర్తులేని పూర్ణాంకం cid = కనుగొను_మొదటి_zero_బిట్టు(mm_cidmask(mm), max_cids);

	ఒకవేళ (cid >= max_cids)
		తిరిగి MM_CID_UNSET;
	ఒకవేళ (test_and_సమితి_బిట్టు(cid, mm_cidmask(mm)))
		తిరిగి MM_CID_UNSET;
	తిరిగి cid;
}

స్థిర అంతఃవరుస గుర్తులేని పూర్ణాంకం mm_get_cid(నిర్మాణం mm_struct *mm)
{
	గుర్తులేని పూర్ణాంకం cid = __mm_get_cid(mm, READ_ONCE(mm->mm_cid.max_cids));

	అయితే (cid == MM_CID_UNSET) {
		cpu_relax();
		cid = __mm_get_cid(mm, అంకె_possible_cpus());
	}
	తిరిగి cid;
}

స్థిర అంతఃవరుస గుర్తులేని పూర్ణాంకం mm_cid_converge(నిర్మాణం mm_struct *mm, గుర్తులేని పూర్ణాంకం orig_cid,
					   గుర్తులేని పూర్ణాంకం max_cids)
{
	గుర్తులేని పూర్ణాంకం new_cid, cid = cpu_cid_గమ్యం_cid(orig_cid);

	/* Is it in the optimal CID space? */
	ఒకవేళ (likely(cid < max_cids))
		తిరిగి orig_cid;

	/* Try to find one in the optimal space. Otherwise keep the provided. */
	new_cid = __mm_get_cid(mm, max_cids);
	ఒకవేళ (new_cid != MM_CID_UNSET) {
		mm_drop_cid(mm, cid);
		/* Preserve the ONCPU mode of the original CID */
		తిరిగి new_cid | (orig_cid & MM_CID_ONCPU);
	}
	తిరిగి orig_cid;
}

స్థిర __always_inline శూన్య mm_cid_update_task_cid(నిర్మాణం task_struct *t, గుర్తులేని పూర్ణాంకం cid)
{
	ఒకవేళ (t->mm_cid.cid != cid) {
		t->mm_cid.cid = cid;
		rseq_sched_సమితి_ids_మారింది(t);
	}
}

స్థిర __always_inline శూన్య mm_cid_update_pcpu_cid(నిర్మాణం mm_struct *mm, గుర్తులేని పూర్ణాంకం cid)
{
	__this_cpu_write(mm->mm_cid.pcpu->cid, cid);
}

స్థిర __always_inline శూన్య mm_cid_from_cpu(నిర్మాణం task_struct *t, గుర్తులేని పూర్ణాంకం cpu_cid,
					    గుర్తులేని పూర్ణాంకం బహుళకం)
{
	గుర్తులేని పూర్ణాంకం max_cids, tcid = t->mm_cid.cid;
	నిర్మాణం mm_struct *mm = t->mm;

	max_cids = READ_ONCE(mm->mm_cid.max_cids);
	/* Optimize for the common case where both have the ONCPU bit set */
	ఒకవేళ (likely(cid_on_cpu(cpu_cid & tcid))) {
		ఒకవేళ (likely(cpu_cid_గమ్యం_cid(cpu_cid) < max_cids)) {
			mm_cid_update_task_cid(t, cpu_cid);
			తిరిగి;
		}
		/* Try to converge into the optimal CID space */
		cpu_cid = mm_cid_converge(mm, cpu_cid, max_cids);
	} లేకపోతే {
		/* Hand over or drop the task owned CID */
		ఒకవేళ (cid_on_task(tcid)) {
			ఒకవేళ (cid_on_cpu(cpu_cid))
				mm_unset_cid_on_task(t);
			లేకపోతే
				cpu_cid = cid_గమ్యం_cpu_cid(tcid);
		}
		/* Still nothing, allocate a new one */
		ఒకవేళ (!cid_on_cpu(cpu_cid))
			cpu_cid = cid_గమ్యం_cpu_cid(mm_get_cid(mm));

		/* Handle the transition mode flag if required */
		ఒకవేళ (బహుళకం & MM_CID_TRANSIT)
			cpu_cid = cpu_cid_గమ్యం_cid(cpu_cid) | MM_CID_TRANSIT;
	}
	mm_cid_update_pcpu_cid(mm, cpu_cid);
	mm_cid_update_task_cid(t, cpu_cid);
}

స్థిర __always_inline శూన్య mm_cid_from_task(నిర్మాణం task_struct *t, గుర్తులేని పూర్ణాంకం cpu_cid,
					     గుర్తులేని పూర్ణాంకం బహుళకం)
{
	గుర్తులేని పూర్ణాంకం max_cids, tcid = t->mm_cid.cid;
	నిర్మాణం mm_struct *mm = t->mm;

	max_cids = READ_ONCE(mm->mm_cid.max_cids);
	/* Optimize for the common case, where both have the ONCPU bit clear */
	ఒకవేళ (likely(cid_on_task(tcid | cpu_cid))) {
		ఒకవేళ (likely(tcid < max_cids)) {
			mm_cid_update_pcpu_cid(mm, tcid);
			తిరిగి;
		}
		/* Try to converge into the optimal CID space */
		tcid = mm_cid_converge(mm, tcid, max_cids);
	} లేకపోతే {
		/* Hand over or drop the CPU owned CID */
		ఒకవేళ (cid_on_cpu(cpu_cid)) {
			ఒకవేళ (cid_on_task(tcid))
				mm_drop_cid_on_cpu(mm, this_cpu_ptr(mm->mm_cid.pcpu));
			లేకపోతే
				tcid = cpu_cid_గమ్యం_cid(cpu_cid);
		}
		/* Still nothing, allocate a new one */
		ఒకవేళ (!cid_on_task(tcid))
			tcid = mm_get_cid(mm);
		/* Set the transition mode flag if required */
		tcid |= బహుళకం & MM_CID_TRANSIT;
	}
	mm_cid_update_pcpu_cid(mm, tcid);
	mm_cid_update_task_cid(t, tcid);
}

స్థిర __always_inline శూన్య mm_cid_schedin(నిర్మాణం task_struct *తదుపరి)
{
	నిర్మాణం mm_struct *mm = తదుపరి->mm;
	గుర్తులేని పూర్ణాంకం cpu_cid, బహుళకం;

	ఒకవేళ (!తదుపరి->mm_cid.చురుకైన)
		తిరిగి;

	cpu_cid = __this_cpu_read(mm->mm_cid.pcpu->cid);
	బహుళకం = READ_ONCE(mm->mm_cid.బహుళకం);
	ఒకవేళ (likely(!cid_on_cpu(బహుళకం)))
		mm_cid_from_task(తదుపరి, cpu_cid, బహుళకం);
	లేకపోతే
		mm_cid_from_cpu(తదుపరి, cpu_cid, బహుళకం);
}

స్థిర __always_inline శూన్య mm_cid_schedout(నిర్మాణం task_struct *prev)
{
	నిర్మాణం mm_struct *mm = prev->mm;
	గుర్తులేని పూర్ణాంకం బహుళకం, cid;

	/* During mode transitions CIDs are temporary and need to be dropped */
	ఒకవేళ (likely(!cid_లోపల_transit(prev->mm_cid.cid)))
		తిరిగి;

	బహుళకం = READ_ONCE(mm->mm_cid.బహుళకం);
	cid = cid_from_transit_cid(prev->mm_cid.cid);

	/*
	 * If transition mode is done, transfer ownership when the CID is
	 * within the convergence range to optimize the next schedule in.
	 */
	ఒకవేళ (!cid_లోపల_transit(బహుళకం) && cid < READ_ONCE(mm->mm_cid.max_cids)) {
		ఒకవేళ (cid_on_cpu(బహుళకం))
			cid = cid_గమ్యం_cpu_cid(cid);

		/* Update both so that the next schedule in goes into the fast path */
		mm_cid_update_pcpu_cid(mm, cid);
		prev->mm_cid.cid = cid;
	} లేకపోతే {
		mm_drop_cid(mm, cid);
		prev->mm_cid.cid = MM_CID_UNSET;
	}
}

స్థిర అంతఃవరుస శూన్య mm_cid_switch_గమ్యం(నిర్మాణం task_struct *prev, నిర్మాణం task_struct *తదుపరి)
{
	mm_cid_schedout(prev);
	mm_cid_schedin(తదుపరి);
}

#లేకపోతే /* !CONFIG_SCHED_MM_CID: */
స్థిర అంతఃవరుస శూన్య mm_cid_switch_గమ్యం(నిర్మాణం task_struct *prev, నిర్మాణం task_struct *తదుపరి) { }
#అయితే_ముగింపు /* !CONFIG_SCHED_MM_CID */

#నిర్వచితమైతే CONFIG_SCHED_CACHE
DECLARE_STATIC_కీలకం_FALSE(sched_cache_present);
DECLARE_STATIC_కీలకం_FALSE(sched_cache_చురుకైన);
బాహ్య పూర్ణాంకం sysctl_sched_cache_user;
బాహ్య గుర్తులేని పూర్ణాంకం llc_aggr_tolerance;
బాహ్య గుర్తులేని పూర్ణాంకం llc_epoch_period;
బాహ్య గుర్తులేని పూర్ణాంకం llc_epoch_affinity_timeout;
బాహ్య గుర్తులేని పూర్ణాంకం llc_imb_pct;
బాహ్య గుర్తులేని పూర్ణాంకం llc_overaggr_pct;

స్థిర అంతఃవరుస bool sched_cache_enabled(శూన్య)
{
	తిరిగి static_branch_unlikely(&sched_cache_చురుకైన);
}

బాహ్య శూన్య sched_cache_చురుకైన_సమితి(శూన్య);

#అయితే_ముగింపు

శూన్య sched_domains_free_llc_id(పూర్ణాంకం cpu);

బాహ్య శూన్య init_sched_mm(నిర్మాణం task_struct *p);

బాహ్య u64 avg_vruntime(నిర్మాణం cfs_rq *cfs_rq);
బాహ్య పూర్ణాంకం entity_eligible(నిర్మాణం cfs_rq *cfs_rq, నిర్మాణం sched_entity *se);
స్థిర అంతఃవరుస
శూన్య move_queued_task_locked(నిర్మాణం rq *src_rq, నిర్మాణం rq *dst_rq, నిర్మాణం task_struct *task)
{
	lockdep_assert_rq_held(src_rq);
	lockdep_assert_rq_held(dst_rq);

	deactivate_task(src_rq, task, 0);
	సమితి_task_cpu(task, dst_rq->cpu);
	activate_task(dst_rq, task, 0);
	wakeup_preempt(dst_rq, task, 0);
}

స్థిర అంతఃవరుస
bool task_is_pushable(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, పూర్ణాంకం cpu)
{
	ఒకవేళ (!task_on_cpu(rq, p) &&
	    cpumask_test_cpu(cpu, &p->cpus_ముసుగు))
		తిరిగి true;

	తిరిగి false;
}

#నిర్వచితమైతే CONFIG_RT_MUTEXES

స్థిర అంతఃవరుస పూర్ణాంకం __rt_effective_prio(నిర్మాణం task_struct *pi_task, పూర్ణాంకం prio)
{
	ఒకవేళ (pi_task)
		prio = min(prio, pi_task->prio);

	తిరిగి prio;
}

స్థిర అంతఃవరుస పూర్ణాంకం rt_effective_prio(నిర్మాణం task_struct *p, పూర్ణాంకం prio)
{
	నిర్మాణం task_struct *pi_task = rt_mutex_get_శిఖరం_task(p);

	తిరిగి __rt_effective_prio(pi_task, prio);
}

#లేకపోతే /* !CONFIG_RT_MUTEXES: */

స్థిర అంతఃవరుస పూర్ణాంకం rt_effective_prio(నిర్మాణం task_struct *p, పూర్ణాంకం prio)
{
	తిరిగి prio;
}

#అయితే_ముగింపు /* !CONFIG_RT_MUTEXES */

బాహ్య పూర్ణాంకం __sched_setscheduler(నిర్మాణం task_struct *p, స్థిరరాశి నిర్మాణం sched_attr *attr, bool user, bool pi);
బాహ్య పూర్ణాంకం __sched_setaffinity(నిర్మాణం task_struct *p, నిర్మాణం affinity_context *ctx);
బాహ్య స్థిరరాశి నిర్మాణం sched_class *__setscheduler_class(పూర్ణాంకం policy, పూర్ణాంకం prio);
బాహ్య శూన్య సమితి_ఎక్కించు_బరువు(నిర్మాణం task_struct *p, bool update_ఎక్కించు);
బాహ్య శూన్య వరుసలో_చేర్చు_task(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, పూర్ణాంకం flags);
బాహ్య bool వరుస_నుండి_తీయి_task(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, పూర్ణాంకం flags);

బాహ్య నిర్మాణం balance_callback *splice_balance_callbacks(నిర్మాణం rq *rq);

బాహ్య శూన్య __balance_callbacks(నిర్మాణం rq *rq, నిర్మాణం rq_flags *rf);
బాహ్య శూన్య balance_callbacks(నిర్మాణం rq *rq, నిర్మాణం balance_callback *తల);

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
నిర్మాణం sched_change_ctx {
	u64			prio;
	నిర్మాణం task_struct	*p;
	స్థిరరాశి నిర్మాణం sched_class *class;
	పూర్ణాంకం			flags;
	bool			queued;
	bool			running;
};

నిర్మాణం sched_change_ctx *sched_change_begin(నిర్మాణం task_struct *p, గుర్తులేని పూర్ణాంకం flags);
శూన్య sched_change_చివర(నిర్మాణం sched_change_ctx *ctx);

DEFINE_CLASS(sched_change, నిర్మాణం sched_change_ctx *,
	     sched_change_చివర(_T),
	     sched_change_begin(p, flags),
	     నిర్మాణం task_struct *p, గుర్తులేని పూర్ణాంకం flags)

DEFINE_CLASS_IS_UNCONDITIONAL(sched_change)

#అన్తర్భావ "ext/ext.h"

#నిర్వచితమైతే CONFIG_PREFERRED_CPU
శూన్య sched_నెట్టు_current_non_preferred_cpu(నిర్మాణం rq *rq);
#లేకపోతే	/* !CONFIG_PREFERRED_CPU */
స్థిర అంతఃవరుస శూన్య sched_నెట్టు_current_non_preferred_cpu(నిర్మాణం rq *rq) { }
#అయితే_ముగింపు

#అయితే_ముగింపు /* _KERNEL_SCHED_SCHED_H */
