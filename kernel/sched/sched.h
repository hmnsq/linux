/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Scheduler internal types and methods:
 */
#جيڪڏهن_وصف_نه_ٿيل _KERNEL_SCHED_SCHED_H
#وصف_ڪريو _KERNEL_SCHED_SCHED_H

#ضمن <linux/prandom.h>
#ضمن <linux/sched/affinity.h>
#ضمن <linux/sched/autogroup.h>
#ضمن <linux/sched/cpufreq.h>
#ضمن <linux/sched/deadline.h>
#ضمن <linux/sched.h>
#ضمن <linux/sched/loadavg.h>
#ضمن <linux/sched/mm.h>
#ضمن <linux/sched/rseq_api.h>
#ضمن <linux/sched/signal.h>
#ضمن <linux/sched/smt.h>
#ضمن <linux/sched/stat.h>
#ضمن <linux/sched/sysctl.h>
#ضمن <linux/sched/task_flags.h>
#ضمن <linux/sched/task.h>
#ضمن <linux/sched/topology.h>
#ضمن <linux/atomic.h>
#ضمن <linux/bitmap.h>
#ضمن <linux/bug.h>
#ضمن <linux/capability.h>
#ضمن <linux/cgroup_api.h>
#ضمن <linux/cgroup.h>
#ضمن <linux/context_tracking.h>
#ضمن <linux/cpufreq.h>
#ضمن <linux/cpumask_api.h>
#ضمن <linux/cpuset.h>
#ضمن <linux/ctype.h>
#ضمن <linux/file.h>
#ضمن <linux/fs_api.h>
#ضمن <linux/hrtimer_api.h>
#ضمن <linux/interrupt.h>
#ضمن <linux/irq_work.h>
#ضمن <linux/jiffies.h>
#ضمن <linux/kref_api.h>
#ضمن <linux/kthread.h>
#ضمن <linux/ktime_api.h>
#ضمن <linux/lockdep_api.h>
#ضمن <linux/lockdep.h>
#ضمن <linux/memblock.h>
#ضمن <linux/memcontrol.h>
#ضمن <linux/minmax.h>
#ضمن <linux/mm.h>
#ضمن <linux/module.h>
#ضمن <linux/mutex_api.h>
#ضمن <linux/plist.h>
#ضمن <linux/poll.h>
#ضمن <linux/proc_fs.h>
#ضمن <linux/profile.h>
#ضمن <linux/psi.h>
#ضمن <linux/rcupdate.h>
#ضمن <linux/seq_file.h>
#ضمن <linux/seqlock.h>
#ضمن <linux/softirq.h>
#ضمن <linux/spinlock_api.h>
#ضمن <linux/static_key.h>
#ضمن <linux/stop_machine.h>
#ضمن <linux/syscalls_api.h>
#ضمن <linux/syscalls.h>
#ضمن <linux/tick.h>
#ضمن <linux/topology.h>
#ضمن <linux/types.h>
#ضمن <linux/u64_stats_sync_api.h>
#ضمن <linux/uaccess.h>
#ضمن <linux/vmstat.h>
#ضمن <linux/wait_api.h>
#ضمن <linux/wait_bit.h>
#ضمن <linux/workqueue_api.h>
#ضمن <linux/delayacct.h>
#ضمن <linux/mmu_context.h>

#ضمن <trace/events/power.h>
#ضمن <trace/events/sched.h>

#ضمن "../workqueue_internal.h"

بنية rq;
بنية cfs_rq;
بنية rt_rq;
بنية sched_گروهه;
بنية cpuidle_state;

#إذا defined(CONFIG_PARAVIRT) && !defined(CONFIG_HAVE_PV_STEAL_CLOCK_GEN)
# include <asm/paravirt.h>
#جيڪڏهن_پورو

#ضمن <asm/barrier.h>

#ضمن "cpupri.h"
#ضمن "cpudeadline.h"

/* task_struct::on_rq states: */
#وصف_ڪريو TASK_ON_RQ_QUEUED	1
#وصف_ڪريو TASK_ON_RQ_MIGRATING	2

خارجي __read_mostly صحيح scheduler_running;

خارجي بنانشان ڊگهو calc_ڀريو_update;
خارجي atomic_long_t calc_ڀريو_tasks;

خارجي فراغ calc_global_ڀريو_tick(بنية rq *this_rq);
خارجي ڊگهو calc_ڀريو_fold_سرگرم(بنية rq *this_rq, ڊگهو adjust);

خارجي فراغ call_trace_sched_update_nr_running(بنية rq *rq, صحيح ڳڻپ);

خارجي صحيح sysctl_sched_rt_period;
خارجي صحيح sysctl_sched_rt_runtime;
خارجي صحيح sched_rr_timeslice;

/*
 * Asymmetric CPU capacity bits
 */
بنية asym_cap_معلومات {
	بنية list_سر link;
	بنية rcu_سر rcu;
	بنانشان ڊگهو صلاحيت;
	بنانشان ڊگهو cpus[];
};

خارجي بنية list_سر asym_cap_list;

#وصف_ڪريو cpu_صلاحيت_وسعت(asym_معلومات) منزل_cpumask((asym_معلومات)->cpus)

/*
 * Helpers for converting nanosecond timing to jiffy resolution
 */
#وصف_ڪريو NS_منزل_JIFFIES(وقت)	((بنانشان ڊگهو)(وقت) / (NSEC_في_SEC/HZ))

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
#جيڪڏهن_وصف_ٿيل CONFIG_64_ٻٽو_انگ
# define NICE_0_ڀريو_سرڪايو	(SCHED_FIXEDPOINT_سرڪايو + SCHED_FIXEDPOINT_سرڪايو)
# define scale_ڀريو(w)		((w) << SCHED_FIXEDPOINT_سرڪايو)
# define scale_ڀريو_هيٺ_ڏانهن(w)					\
({								\
	بنانشان ڊگهو __w = (w);				\
								\
	إذا (__w)						\
		__w = max(2UL, __w >> SCHED_FIXEDPOINT_سرڪايو);	\
	__w;							\
})
#وإلا
# define NICE_0_ڀريو_سرڪايو	(SCHED_FIXEDPOINT_سرڪايو)
# define scale_ڀريو(w)		(w)
# define scale_ڀريو_هيٺ_ڏانهن(w)	(w)
#جيڪڏهن_پورو

/*
 * Task weight (visible to users) and its load (invisible to users) have
 * independent resolution, but they should be well calibrated. We use
 * scale_load() and scale_load_down(w) to convert between them. The
 * following must be true:
 *
 *  scale_load(sched_prio_to_weight[NICE_TO_PRIO(0)-MAX_RT_PRIO]) == NICE_0_LOAD
 *
 */
#وصف_ڪريو NICE_0_ڀريو		(1L << NICE_0_ڀريو_سرڪايو)

/*
 * Single value that decides SCHED_DEADLINE internal math precision.
 * 10 -> just above 1us
 * 9  -> just above 0.5us
 */
#وصف_ڪريو DL_SCALE		10

/*
 * Single value that denotes runtime == period, ie unlimited time.
 */
#وصف_ڪريو RUNTIME_INF		((u64)~0ULL)

ساكن مضمن صحيح idle_policy(صحيح policy)
{
	أرجع policy == SCHED_IDLE;
}

ساكن مضمن صحيح عام_policy(صحيح policy)
{
#جيڪڏهن_وصف_ٿيل CONFIG_SCHED_CLASS_EXT
	إذا (policy == SCHED_EXT)
		أرجع true;
#جيڪڏهن_پورو
	أرجع policy == SCHED_عام;
}

ساكن مضمن صحيح fair_policy(صحيح policy)
{
	أرجع عام_policy(policy) || policy == SCHED_BATCH;
}

ساكن مضمن صحيح rt_policy(صحيح policy)
{
	أرجع policy == SCHED_FIFO || policy == SCHED_RR;
}

ساكن مضمن صحيح dl_policy(صحيح policy)
{
	أرجع policy == SCHED_DEADLINE;
}

ساكن مضمن bool valid_policy(صحيح policy)
{
	أرجع idle_policy(policy) || fair_policy(policy) ||
		rt_policy(policy) || dl_policy(policy);
}

ساكن مضمن صحيح task_has_idle_policy(بنية task_struct *p)
{
	أرجع idle_policy(p->policy);
}

ساكن مضمن صحيح task_has_rt_policy(بنية task_struct *p)
{
	أرجع rt_policy(p->policy);
}

ساكن مضمن صحيح task_has_dl_policy(بنية task_struct *p)
{
	أرجع dl_policy(p->policy);
}

#وصف_ڪريو cap_scale(v, s)		((v)*(s) >> SCHED_صلاحيت_سرڪايو)

ساكن مضمن فراغ update_avg(u64 *avg, u64 sample)
{
	s64 diff = sample - *avg;

	*avg += diff / 8;
}

/*
 * Shifting a value by an exponent greater *or equal* to the size of said value
 * is UB; cap at size-1.
 */
#وصف_ڪريو shr_bound(val, سرڪايو)							\
	(val >> min_t(typeof(سرڪايو), سرڪايو, بٽ_في_TYPE(typeof(val)) - 1))

/*
 * cgroup weight knobs should use the common MIN, DFL and MAX values which are
 * 1, 100 and 10000 respectively. While it loses a bit of range on both ends, it
 * maps pretty well onto the shares value used by scheduler and the round-trip
 * conversions preserve the original value over the entire range.
 */
ساكن مضمن بنانشان ڊگهو sched_وزن_from_cgroup(بنانشان ڊگهو cgrp_وزن)
{
	أرجع DIV_ڦيرو_CLOSEST_ULL(cgrp_وزن * 1024, CGROUP_وزن_DFL);
}

ساكن مضمن بنانشان ڊگهو sched_وزن_منزل_cgroup(بنانشان ڊگهو وزن)
{
	أرجع clamp_t(بنانشان ڊگهو,
		       DIV_ڦيرو_CLOSEST_ULL(وزن * CGROUP_وزن_DFL, 1024),
		       CGROUP_وزن_MIN, CGROUP_وزن_MAX);
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
#وصف_ڪريو SCHED_FLAG_SUGOV	0x10000000

#وصف_ڪريو SCHED_DL_FLAGS		(SCHED_FLAG_RECLAIM | SCHED_FLAG_DL_OVERRUN | SCHED_FLAG_SUGOV)

ساكن مضمن bool dl_entity_is_special(ثابت بنية sched_dl_entity *dl_se)
{
#جيڪڏهن_وصف_ٿيل CONFIG_CPU_FREQ_GOV_SCHEDUTIL
	أرجع unlikely(dl_se->flags & SCHED_FLAG_SUGOV);
#وإلا
	أرجع false;
#جيڪڏهن_پورو
}

/*
 * Tells if entity @a should preempt entity @b.
 */
ساكن مضمن bool dl_entity_preempt(ثابت بنية sched_dl_entity *a,
				     ثابت بنية sched_dl_entity *b)
{
	أرجع dl_entity_is_special(a) ||
	       dl_وقت_before(a->deadline, b->deadline);
}

/*
 * This is the priority-queue data structure of the RT scheduling class:
 */
بنية rt_prio_array {
	DECLARE_BITMAP(bitmap, MAX_RT_PRIO+1); /* include 1 bit for delimiter */
	بنية list_سر قطار[MAX_RT_PRIO];
};

بنية rt_bandwidth {
	/* nests inside the rq lock: */
	raw_spinlock_t		rt_runtime_تالو;
	ktime_t			rt_period;
	u64			rt_runtime;
	بنية hrtimer		rt_period_timer;
	بنانشان صحيح		rt_period_سرگرم;
};

ساكن مضمن صحيح dl_bandwidth_enabled(فراغ)
{
	أرجع sysctl_sched_rt_runtime >= 0;
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
بنية dl_bw {
	raw_spinlock_t		تالو;
	u64			bw;
	u64			ڪل_bw;
};

خارجي فراغ init_dl_bw(بنية dl_bw *dl_b);
خارجي صحيح  sched_dl_global_validate(فراغ);
خارجي فراغ sched_dl_do_global(فراغ);
خارجي صحيح  sched_dl_حد_کان_مٿي(بنية task_struct *p, صحيح policy, ثابت بنية sched_attr *attr);
خارجي فراغ __setparam_dl(بنية task_struct *p, ثابت بنية sched_attr *attr);
خارجي فراغ __getparam_dl(بنية task_struct *p, بنية sched_attr *attr, بنانشان صحيح flags);
خارجي bool __checkparam_dl(ثابت بنية sched_attr *attr);
خارجي bool dl_param_بدليو(بنية task_struct *p, ثابت بنية sched_attr *attr);
خارجي صحيح  dl_cpuset_cpumask_can_shrink(ثابت بنية cpumask *cur, ثابت بنية cpumask *trial);
خارجي صحيح  dl_bw_deactivate(صحيح cpu);
خارجي s64 dl_scaled_delta_exec(بنية rq *rq, بنية sched_dl_entity *dl_se, s64 delta_exec);
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
خارجي فراغ dl_server_update_idle(بنية sched_dl_entity *dl_se, s64 delta_exec);
خارجي فراغ dl_server_update(بنية sched_dl_entity *dl_se, s64 delta_exec);
خارجي فراغ dl_server_شروعات(بنية sched_dl_entity *dl_se);
خارجي فراغ dl_server_stop(بنية sched_dl_entity *dl_se);
خارجي فراغ dl_server_init(بنية sched_dl_entity *dl_se, بنية rq *rq,
		    dl_server_pick_f pick_task);
خارجي فراغ sched_init_dl_servers(فراغ);

خارجي فراغ fair_server_init(بنية rq *rq);
خارجي فراغ ext_server_init(بنية rq *rq);
خارجي فراغ __dl_server_attach_root(بنية sched_dl_entity *dl_se, بنية rq *rq);
خارجي صحيح dl_server_apply_params(بنية sched_dl_entity *dl_se,
		    u64 runtime, u64 period, bool init);
خارجي صحيح dl_server_attach_bw(بنية sched_dl_entity *dl_se);
خارجي فراغ dl_server_detach_bw(بنية sched_dl_entity *dl_se);
خارجي صحيح dl_server_مٽا_سٽا_bw(بنية sched_dl_entity *detach_se,
			     بنية sched_dl_entity *attach_se);

ساكن مضمن bool dl_server_سرگرم(بنية sched_dl_entity *dl_se)
{
	أرجع dl_se->dl_server_سرگرم;
}

#جيڪڏهن_وصف_ٿيل CONFIG_CGROUP_SCHED

خارجي بنية list_سر task_groups;

#جيڪڏهن_وصف_ٿيل CONFIG_گروهه_SCHED_BANDWIDTH
خارجي ثابت u64 max_bw_quota_period_us;

/*
 * default period for group bandwidth.
 * default: 0.1s, units: microseconds
 */
ساكن مضمن u64 default_bw_period_us(فراغ)
{
	أرجع 100000ULL;
}
#جيڪڏهن_پورو /* CONFIG_GROUP_SCHED_BANDWIDTH */

بنية cfs_bandwidth {
#جيڪڏهن_وصف_ٿيل CONFIG_CFS_BANDWIDTH
	raw_spinlock_t		تالو;
	ktime_t			period;
	u64			quota;
	u64			runtime;
	u64			مدو;
	u64			runtime_snap;
	s64			درجيوار_quota;

	u8			idle;
	u8			period_سرگرم;
	u8			slack_started;
	بنية hrtimer		period_timer;
	بنية hrtimer		slack_timer;
	بنية list_سر	throttled_cfs_rq;

	/* Statistics: */
	صحيح			nr_periods;
	صحيح			nr_throttled;
	صحيح			nr_مدو;
	u64			throttled_وقت;
	u64			مدو_وقت;
#جيڪڏهن_پورو /* CONFIG_CFS_BANDWIDTH */
};

/* Task group related information */
بنية task_گروهه {
	بنية cgroup_subsys_state css;

#جيڪڏهن_وصف_ٿيل CONFIG_گروهه_SCHED_وزن
	/* A positive value indicates that this is a SCHED_IDLE group. */
	صحيح			idle;
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل CONFIG_FAIR_گروهه_SCHED
	/* runqueue "owned" by this group on each CPU */
	بنية cfs_rq __percpu	*cfs_rq;
	بنانشان ڊگهو		shares;
	/*
	 * load_avg can be heavily contended at clock tick time, so put
	 * it in its own cache-line separated from the fields above which
	 * will also be accessed at each tick.
	 */
	atomic_long_t		ڀريو_avg ____cacheline_aligned;
	atomic_long_t		runnable_avg;

#جيڪڏهن_پورو /* CONFIG_FAIR_GROUP_SCHED */

#جيڪڏهن_وصف_ٿيل CONFIG_RT_گروهه_SCHED
	بنية sched_rt_entity	**rt_se;
	بنية rt_rq		**rt_rq;

	بنية rt_bandwidth	rt_bandwidth;
#جيڪڏهن_پورو

	بنية scx_task_گروهه	scx;

	بنية rcu_سر		rcu;
	بنية list_سر	list;

	بنية task_گروهه	*والد;
	بنية list_سر	siblings;
	بنية list_سر	children;

#جيڪڏهن_وصف_ٿيل CONFIG_SCHED_AUTOGROUP
	بنية autogroup	*autogroup;
#جيڪڏهن_پورو

	بنية cfs_bandwidth	cfs_bandwidth;

#جيڪڏهن_وصف_ٿيل CONFIG_UCLAMP_TASK_گروهه
	/* The two decimal precision [%] value requested from user-space */
	بنانشان صحيح		uclamp_pct[UCLAMP_CNT];
	/* Clamp values requested for a task group */
	بنية uclamp_se	uclamp_req[UCLAMP_CNT];
	/* Effective clamp values used for a task group */
	بنية uclamp_se	uclamp[UCLAMP_CNT];
#جيڪڏهن_پورو

};

#وصف_ڪريو پاڙ_TASK_گروهه_ڀريو	NICE_0_ڀريو

تعريفنوع صحيح (*tg_visitor)(بنية task_گروهه *, فراغ *);

خارجي صحيح walk_tg_وڻ_from(بنية task_گروهه *from,
			     tg_visitor هيٺ_ڏانهن, tg_visitor مٿي, فراغ *معلومات);

/*
 * Iterate the full tree, calling @down when first entering a node and @up when
 * leaving it for the final time.
 *
 * Caller must hold rcu_lock or sufficient equivalent.
 */
ساكن مضمن صحيح walk_tg_وڻ(tg_visitor هيٺ_ڏانهن, tg_visitor مٿي, فراغ *معلومات)
{
	أرجع walk_tg_وڻ_from(&پاڙ_task_گروهه, هيٺ_ڏانهن, مٿي, معلومات);
}

ساكن مضمن بنية task_گروهه *css_tg(بنية cgroup_subsys_state *css)
{
	أرجع css ? container_جو(css, بنية task_گروهه, css) : عدم;
}

خارجي صحيح tg_nop(بنية task_گروهه *tg, فراغ *معلومات);

#جيڪڏهن_وصف_ٿيل CONFIG_FAIR_گروهه_SCHED
خارجي فراغ free_fair_sched_گروهه(بنية task_گروهه *tg);
خارجي صحيح alloc_fair_sched_گروهه(بنية task_گروهه *tg, بنية task_گروهه *والد);
خارجي فراغ online_fair_sched_گروهه(بنية task_گروهه *tg);
خارجي فراغ unregister_fair_sched_گروهه(بنية task_گروهه *tg);
خارجي فراغ __sched_cgroup_mode_update(صحيح گهڻائي);
#وإلا /* !CONFIG_FAIR_GROUP_SCHED: */
ساكن مضمن فراغ free_fair_sched_گروهه(بنية task_گروهه *tg) { }
ساكن مضمن صحيح alloc_fair_sched_گروهه(بنية task_گروهه *tg, بنية task_گروهه *والد)
{
       أرجع 1;
}
ساكن مضمن فراغ online_fair_sched_گروهه(بنية task_گروهه *tg) { }
ساكن مضمن فراغ unregister_fair_sched_گروهه(بنية task_گروهه *tg) { }
#جيڪڏهن_پورو /* !CONFIG_FAIR_GROUP_SCHED */

خارجي فراغ init_tg_cfs_داخلا(بنية task_گروهه *tg, بنية cfs_rq *cfs_rq,
			بنية sched_entity *se, صحيح cpu,
			بنية sched_entity *والد);
خارجي فراغ init_cfs_bandwidth(بنية cfs_bandwidth *cfs_b, بنية cfs_bandwidth *والد);

خارجي فراغ __refill_cfs_bandwidth_runtime(بنية cfs_bandwidth *cfs_b);
خارجي فراغ شروعات_cfs_bandwidth(بنية cfs_bandwidth *cfs_b);
خارجي فراغ unthrottle_cfs_rq(بنية cfs_rq *cfs_rq);
خارجي bool cfs_task_bw_constrained(بنية task_struct *p);

خارجي فراغ init_tg_rt_داخلا(بنية task_گروهه *tg, بنية rt_rq *rt_rq,
		بنية sched_rt_entity *rt_se, صحيح cpu,
		بنية sched_rt_entity *والد);
خارجي صحيح sched_گروهه_سيٽ_rt_runtime(بنية task_گروهه *tg, ڊگهو rt_runtime_us);
خارجي صحيح sched_گروهه_سيٽ_rt_period(بنية task_گروهه *tg, u64 rt_period_us);
خارجي ڊگهو sched_گروهه_rt_runtime(بنية task_گروهه *tg);
خارجي ڊگهو sched_گروهه_rt_period(بنية task_گروهه *tg);
خارجي صحيح sched_rt_can_attach(بنية task_گروهه *tg, بنية task_struct *tsk);

خارجي بنية task_گروهه *sched_create_گروهه(بنية task_گروهه *والد);
خارجي فراغ sched_online_گروهه(بنية task_گروهه *tg,
			       بنية task_گروهه *والد);
خارجي فراغ sched_destroy_گروهه(بنية task_گروهه *tg);
خارجي فراغ sched_release_گروهه(بنية task_گروهه *tg);

خارجي فراغ sched_move_task(بنية task_struct *tsk, bool for_autogroup);

#جيڪڏهن_وصف_ٿيل CONFIG_FAIR_گروهه_SCHED
خارجي صحيح sched_گروهه_سيٽ_shares(بنية task_گروهه *tg, بنانشان ڊگهو shares);

خارجي صحيح sched_گروهه_سيٽ_idle(بنية task_گروهه *tg, ڊگهو idle);

خارجي فراغ سيٽ_task_rq_fair(بنية sched_entity *se,
			     بنية cfs_rq *prev, بنية cfs_rq *ايندڙ);
#وإلا /* !CONFIG_FAIR_GROUP_SCHED: */
ساكن مضمن صحيح sched_گروهه_سيٽ_shares(بنية task_گروهه *tg, بنانشان ڊگهو shares) { أرجع 0; }
ساكن مضمن صحيح sched_گروهه_سيٽ_idle(بنية task_گروهه *tg, ڊگهو idle) { أرجع 0; }
#جيڪڏهن_پورو /* !CONFIG_FAIR_GROUP_SCHED */

#وإلا /* !CONFIG_CGROUP_SCHED: */

بنية cfs_bandwidth { };

ساكن مضمن bool cfs_task_bw_constrained(بنية task_struct *p) { أرجع false; }

#جيڪڏهن_پورو /* !CONFIG_CGROUP_SCHED */

/*
 * A weight of 0 or 1 can cause arithmetics problems.
 * A weight of a cfs_rq is the sum of weights of which entities
 * are queued on this cfs_rq, so a weight of a entity should not be
 * too large, so as the shares value of a task group.
 * (The default weight is 1024 - so there's no practical
 *  limitation from this.)
 */
#وصف_ڪريو MIN_SHARES		(1UL <<  1)
#وصف_ڪريو MAX_SHARES		(1UL << 18)

خارجي فراغ unregister_rt_sched_گروهه(بنية task_گروهه *tg);
خارجي فراغ free_rt_sched_گروهه(بنية task_گروهه *tg);
خارجي صحيح alloc_rt_sched_گروهه(بنية task_گروهه *tg, بنية task_گروهه *والد);

/*
 * u64_u32_load/u64_u32_store
 *
 * Use a copy of a u64 value to protect against data race. This is only
 * applicable for 32-bits architectures.
 */
#جيڪڏهن_وصف_ٿيل CONFIG_64_ٻٽو_انگ
# define u64_u32_ڀريو_copy(var, copy)		var
# define u64_u32_store_copy(var, copy, val)	(var = val)
#وإلا
# define u64_u32_ڀريو_copy(var, copy)					\
({									\
	u64 __val, __val_copy;						\
	افعل {								\
		__val_copy = copy;					\
		/*							\
		 * paired with u64_u32_store_copy(), ordering access	\
		 * to var and copy.					\
		 */							\
		smp_rmb();						\
		__val = var;						\
	} طالما (__val != __val_copy);					\
	__val;								\
})
# define u64_u32_store_copy(var, copy, val)				\
افعل {									\
	typeof(val) __val = (val);					\
	var = __val;							\
	/*								\
	 * paired with u64_u32_load_copy(), ordering access to var and	\
	 * copy.							\
	 */								\
	smp_wmb();							\
	copy = __val;							\
} طالما (0)
#جيڪڏهن_پورو
# define u64_u32_ڀريو(var)		u64_u32_ڀريو_copy(var, var##_copy)
# define u64_u32_store(var, val)	u64_u32_store_copy(var, var##_copy, val)

بنية balance_callback {
	بنية balance_callback *ايندڙ;
	فراغ (*func)(بنية rq *rq);
};

/* Fair scheduling SCHED_{NORMAL,BATCH,IDLE} related fields in a runqueue: */
بنية cfs_rq {
	بنية ڀريو_وزن	ڀريو;
	بنانشان صحيح		nr_queued;
	بنانشان صحيح		h_nr_queued;		/* SCHED_{NORMAL,BATCH,IDLE} */
	بنانشان صحيح		h_nr_runnable;		/* SCHED_{NORMAL,BATCH,IDLE} */
	بنانشان صحيح		h_nr_idle;		/* SCHED_IDLE */

	s64			جوڙ_w_vruntime;
	u64			جوڙ_وزن;
	u64			zero_vruntime;
	بنانشان صحيح		جوڙ_سرڪايو;

#جيڪڏهن_وصف_ٿيل CONFIG_SCHED_CORE
	بنانشان صحيح		forceidle_seq;
	u64			zero_vruntime_fi;
#جيڪڏهن_پورو

	بنية rb_پاڙ_cached	tasks_timeline;

	/*
	 * 'curr' points to the currently running entity on this cfs_rq.
	 * It is set to NULL otherwise (i.e when none are currently running).
	 */
	بنية sched_entity	*curr;
	بنية sched_entity	*ايندڙ;

	/*
	 * CFS load tracking
	 */
	بنية sched_entity	*h_curr;
	بنية sched_avg	avg;
#جيڪڏهن_وصف_نه_ٿيل CONFIG_64_ٻٽو_انگ
	u64			آخري_update_وقت_copy;
#جيڪڏهن_پورو
	بنية {
		raw_spinlock_t	تالو ____cacheline_aligned;
		صحيح		nr;
		بنانشان ڊگهو	ڀريو_avg;
		بنانشان ڊگهو	util_avg;
		بنانشان ڊگهو	runnable_avg;
	} removed;

#جيڪڏهن_وصف_ٿيل CONFIG_FAIR_گروهه_SCHED
	u64			آخري_update_tg_ڀريو_avg;
	بنانشان ڊگهو		tg_ڀريو_avg_contrib;
	بنانشان ڊگهو		tg_runnable_avg_contrib;
	ڊگهو			propagate;
	ڊگهو			prop_runnable_جوڙ;

	/*
	 *   h_load = weight * f(tg)
	 *
	 * Where f(tg) is the recursive weight fraction assigned to
	 * this group.
	 */
	بنانشان ڊگهو		h_ڀريو;
	u64			آخري_h_ڀريو_update;
	بنية sched_entity	*h_ڀريو_ايندڙ;

	بنية rq		*rq;	/* CPU runqueue to which this cfs_rq is attached */

	/*
	 * leaf cfs_rqs are those that hold tasks (lowest schedulable entity in
	 * a hierarchy). Non-leaf lrqs hold other higher schedulable entities
	 * (like users, containers etc.)
	 *
	 * leaf_cfs_rq_list ties together list of leaf cfs_rq's in a CPU.
	 * This list is used during load balance.
	 */
	صحيح			on_list;
	بنية list_سر	پن_cfs_rq_list;
	بنية task_گروهه	*tg;	/* Group that "owns" this runqueue */

	/* Locally cached copy of our task_group's idle value */
	صحيح			idle;

# ifdef CONFIG_CFS_BANDWIDTH
	صحيح			runtime_enabled;
	s64			runtime_بچيل;

	u64			throttled_pelt_idle;
#  ifndef CONFIG_64_ٻٽو_انگ
	u64                     throttled_pelt_idle_copy;
#  endif
	u64			throttled_clock;
	u64			throttled_clock_pelt;
	u64			throttled_clock_pelt_وقت;
	u64			throttled_clock_self;
	u64			throttled_clock_self_وقت;
	bool			throttled:1;
	bool			pelt_clock_throttled:1;
	صحيح			throttle_ڳڻپ;
	بنية list_سر	throttled_list;
	بنية list_سر	throttled_csd_list;
	بنية list_سر        throttled_limbo_list;
# endif /* CONFIG_CFS_BANDWIDTH */
#جيڪڏهن_پورو /* CONFIG_FAIR_GROUP_SCHED */
};

#جيڪڏهن_وصف_ٿيل CONFIG_SCHED_CLASS_EXT
/* scx_rq->flags, protected by the rq lock */
تعداد scx_rq_flags {
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
	SCX_RQ_پاڙ_IDLE_RENOTIFY	= 1 << 8, /* the root is owed update_idle() */

	SCX_RQ_اندريون_WAKEUP	= 1 << 16,
	SCX_RQ_اندريون_DISPATCH	= 1 << 17,
};

/* per-rq rescue execution state, see scx_rescue_timerfn() */
بنية scx_rq_rescue {
	بنية scx_dispatch_q	dsq;			/* stranded tasks awaiting rescue */
	s64			budget;			/* execution token bucket, ns */
	u64			clock;			/* last budget accrual timestamp */
	بنية task_struct	*curr;			/* task being rescued, one at a time */
	s64			ٽڪرو;			/* curr's admitted slice */
	u64			exec_snap;		/* sum_exec_runtime at admission */
	بنية timer_list	timer;			/* paces admission and escalation */
	u64			kill_at;		/* last ejection, init before any */
};

بنية scx_rq {
	بنية scx_dispatch_q	local_dsq;
#جيڪڏهن_وصف_ٿيل CONFIG_EXT_SUB_SCHED
	بنية scx_dispatch_q	reject_dsq;		/* staging for cap-rejected tasks */
	بنية scx_rq_rescue	rescue;
#جيڪڏهن_پورو
	بنية list_سر	runnable_list;		/* runnable tasks on this rq */
	بنية list_سر	ddsp_deferred_locals;	/* deferred ddsps from enq */
	/* both stashed across the activate_task() in move_remote_task_to_local_dsq() */
	u64			remote_activate_enq_flags;
	بنية scx_sched	*remote_activate_sch;
	u32			nr_running;
	u32			cpuperf_هدف;		/* [0, SCHED_CAPACITY_SCALE] */
	bool			اندريون_select_cpu;
	bool			cpu_released;
	u32			flags;
	u32			nr_immed;		/* ENQ_IMMED tasks on local_dsq */
#جيڪڏهن_وصف_ٿيل CONFIG_SCHED_CORE
	u32			تالو_drop_seq;	/* nr dispatch lock releases */
#جيڪڏهن_پورو
	u64			clock;			/* current per-rq clock -- see scx_bpf_now() */
#جيڪڏهن_وصف_ٿيل CONFIG_EXT_SUB_SCHED
	بنية llist_سر	ecaps_منزل_sync;		/* pending ecaps syncs */
	بنية task_struct	*sub_dispatch_prev;
#جيڪڏهن_پورو
	cpumask_var_t		cpus_منزل_sync;
	bool			kick_sync_pending;
	بنانشان ڊگهو		kick_sync;

	بنية list_سر	sched_pcpus_منزل_kick;	/* see kick_cpus_irq_workfn() */

	raw_spinlock_t		deferred_reenq_تالو;
	بنية list_سر	deferred_reenq_locals;	/* scheds requesting reenq of local DSQ */
	بنية list_سر	deferred_reenq_users;	/* user DSQs requesting reenq */
	بنية balance_callback	deferred_bal_cb;
	بنية balance_callback	kick_sync_bal_cb;
	بنية irq_work		deferred_irq_work;
	بنية irq_work		kick_cpus_irq_work;
};
#جيڪڏهن_پورو /* CONFIG_SCHED_CLASS_EXT */

ساكن مضمن صحيح rt_bandwidth_enabled(فراغ)
{
	أرجع sysctl_sched_rt_runtime >= 0;
}

/* RT IPI pull logic requires IRQ_WORK */
#إذا defined(CONFIG_IRQ_WORK) && defined(CONFIG_SMP)
# define HAVE_RT_ڌڪو_IPI
#جيڪڏهن_پورو

/* Real-Time classes' related field in a runqueue: */
بنية rt_rq {
	بنية rt_prio_array	سرگرم;
	بنانشان صحيح		rt_nr_running;
	بنانشان صحيح		rr_nr_running;
	بنية {
		صحيح		curr; /* highest queued rt task prio */
		صحيح		ايندڙ; /* next highest */
	} highest_prio;
	bool			overloaded;
	بنية plist_سر	pushable_tasks;

	صحيح			rt_queued;

#جيڪڏهن_وصف_ٿيل CONFIG_RT_گروهه_SCHED
	صحيح			rt_throttled;
	u64			rt_وقت; /* consumed RT time, goes up in update_curr_rt */
	u64			rt_runtime; /* allotted RT time, "slice" from rt_bandwidth, RT sharing/balancing */
	/* Nests inside the rq lock: */
	raw_spinlock_t		rt_runtime_تالو;

	بنانشان صحيح		rt_nr_boosted;

	بنية rq		*rq; /* this is always top-level rq, cache? */
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل CONFIG_CGROUP_SCHED
	بنية task_گروهه	*tg; /* this tg has "this" rt_rq on given CPU for runnable entities */
#جيڪڏهن_پورو
};

ساكن مضمن bool rt_rq_is_runnable(بنية rt_rq *rt_rq)
{
	أرجع rt_rq->rt_queued && rt_rq->rt_nr_running;
}

/* Deadline class' related fields in a runqueue */
بنية dl_rq {
	/* runqueue is an rbtree, ordered by deadline */
	بنية rb_پاڙ_cached	پاڙ;

	بنانشان صحيح		dl_nr_running;

	/*
	 * Deadline values of the currently executing and the
	 * earliest ready task on this rq. Caching these facilitates
	 * the decision whether or not a ready but not running task
	 * should migrate somewhere else.
	 */
	بنية {
		u64		curr;
		u64		ايندڙ;
	} earliest_dl;

	bool			overloaded;

	بنية sched_dl_entity	*curr;
	/*
	 * Tasks on this rq that can be pushed away. They are kept in
	 * an rb-tree, ordered by tasks' deadlines, with caching
	 * of the leftmost (earliest deadline) element.
	 */
	بنية rb_پاڙ_cached	pushable_dl_tasks_پاڙ;

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
	u64			bw_تناسب;
};

#جيڪڏهن_وصف_ٿيل CONFIG_FAIR_گروهه_SCHED
/* Check whether a task group is root tg */
#وصف_ڪريو is_پاڙ_task_گروهه(tg) ((tg) == &پاڙ_task_گروهه)
/* An entity is a task if it doesn't "own" a runqueue */
#وصف_ڪريو entity_is_task(se)	(!se->my_q)

ساكن مضمن فراغ se_update_runnable(بنية sched_entity *se)
{
	إذا (!entity_is_task(se))
		se->runnable_وزن = se->my_q->h_nr_runnable;
}

ساكن مضمن ڊگهو se_runnable(بنية sched_entity *se)
{
	إذا (se->sched_delayed)
		أرجع false;

	إذا (entity_is_task(se))
		أرجع !!se->on_rq;
	وإلا
		أرجع se->runnable_وزن;
}

#وإلا /* !CONFIG_FAIR_GROUP_SCHED: */

#وصف_ڪريو entity_is_task(se)	1

ساكن مضمن فراغ se_update_runnable(بنية sched_entity *se) { }

ساكن مضمن ڊگهو se_runnable(بنية sched_entity *se)
{
	إذا (se->sched_delayed)
		أرجع false;

	أرجع !!se->on_rq;
}

#جيڪڏهن_پورو /* !CONFIG_FAIR_GROUP_SCHED */

/*
 * XXX we want to get rid of these helpers and use the full load resolution.
 */
ساكن مضمن ڊگهو se_وزن(بنية sched_entity *se)
{
	أرجع scale_ڀريو_هيٺ_ڏانهن(se->ڀريو.وزن);
}


ساكن مضمن bool sched_asym_prefer(صحيح a, صحيح b)
{
	أرجع arch_asym_cpu_ترجيح(a) > arch_asym_cpu_ترجيح(b);
}

بنية perf_domain {
	بنية em_perf_domain *em_pd;
	بنية perf_domain *ايندڙ;
	بنية rcu_سر rcu;
};

/*
 * We add the notion of a root-domain which will be used to define per-domain
 * variables. Each exclusive cpuset essentially defines an island domain by
 * fully partitioning the member CPUs from any other cpuset. Whenever a new
 * exclusive cpuset is created, we also create and attach a new root-domain
 * object.
 *
 */
بنية پاڙ_domain {
	atomic_t		refcount;
	atomic_t		rto_ڳڻپ;
	بنية rcu_سر		rcu;
	cpumask_var_t		وسعت;
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
	cpumask_var_t		dlo_نقاب;
	atomic_t		dlo_ڳڻپ;
	بنية dl_bw		dl_bw;
	بنية cpudl		cpudl;

	/*
	 * Indicate whether a root_domain's dl_bw has been checked or
	 * updated. It's monotonously increasing value.
	 *
	 * Also, some corner cases, like 'wrap around' is dangerous, but given
	 * that u64 is 'big enough'. So that shouldn't be a concern.
	 */
	u64 گهمو_cookie;

#جيڪڏهن_وصف_ٿيل HAVE_RT_ڌڪو_IPI
	/*
	 * For IPI pull requests, loop across the rto_mask.
	 */
	بنية irq_work		rto_ڌڪو_work;
	raw_spinlock_t		rto_تالو;
	/* These are only updated and read within rto_lock */
	صحيح			rto_loop;
	صحيح			rto_cpu;
	/* These atomics are updated outside of a lock */
	atomic_t		rto_loop_ايندڙ;
	atomic_t		rto_loop_شروعات;
#جيڪڏهن_پورو /* HAVE_RT_PUSH_IPI */
	/*
	 * The "RT overload" flag: it gets set if a CPU has more than
	 * one runnable RT task.
	 */
	cpumask_var_t		rto_نقاب;
	بنية cpupri		cpupri;

	/*
	 * NULL-terminated list of performance domains intersecting with the
	 * CPUs of the rd. Protected by RCU.
	 */
	بنية perf_domain __rcu *pd;
};

خارجي فراغ init_defrootdomain(فراغ);
خارجي صحيح sched_init_domains(ثابت بنية cpumask *cpu_map);
خارجي فراغ rq_attach_پاڙ(بنية rq *rq, بنية پاڙ_domain *rd);
خارجي فراغ sched_get_rd(بنية پاڙ_domain *rd);
خارجي فراغ sched_put_rd(بنية پاڙ_domain *rd);

ساكن مضمن صحيح get_rd_overloaded(بنية پاڙ_domain *rd)
{
	أرجع READ_ONCE(rd->overloaded);
}

ساكن مضمن فراغ سيٽ_rd_overloaded(بنية پاڙ_domain *rd, صحيح حالت)
{
	إذا (get_rd_overloaded(rd) != حالت)
		WRITE_ONCE(rd->overloaded, حالت);
}

#جيڪڏهن_وصف_ٿيل HAVE_RT_ڌڪو_IPI
خارجي فراغ rto_ڌڪو_irq_work_func(بنية irq_work *work);
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل CONFIG_UCLAMP_TASK
/*
 * struct uclamp_bucket - Utilization clamp bucket
 * @value: utilization clamp value for tasks on this clamp bucket
 * @tasks: number of RUNNABLE tasks on this clamp bucket
 *
 * Keep track of how many tasks are RUNNABLE for a given utilization
 * clamp value.
 */
بنية uclamp_ٻالٽي {
	بنانشان ڊگهو قدر : بٽ_في(SCHED_صلاحيت_SCALE);
	بنانشان ڊگهو tasks : بٽ_في_LONG - بٽ_في(SCHED_صلاحيت_SCALE);
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
بنية uclamp_rq {
	بنانشان صحيح قدر;
	بنية uclamp_ٻالٽي ٻالٽي[UCLAMP_BUCKETS];
};

DECLARE_STATIC_ڪنجي_FALSE(sched_uclamp_استعمال_ٿيل);
#جيڪڏهن_پورو /* CONFIG_UCLAMP_TASK */

/*
 * This is the main, per-CPU runqueue data structure.
 *
 * Locking rule: those places that want to lock multiple runqueues
 * (such as the load balancing or the thread migration code), lock
 * acquire operations must be ordered by ascending &runqueue.
 */
بنية rq {
	/*
	 * The following members are loaded together, without holding the
	 * rq->lock, in an extremely hot loop in update_sg_lb_stats()
	 * (called from pick_next_task()). To reduce cache pollution from
	 * this operation, they are placed together on this dedicated cache
	 * line. Even though some of them are frequently modified, they are
	 * loaded much more frequently than they are stored.
	 */
	بنانشان صحيح		nr_running;
#جيڪڏهن_وصف_ٿيل CONFIG_NUMA_BALANCING
	بنانشان صحيح		nr_numa_running;
	بنانشان صحيح		nr_preferred_running;
#جيڪڏهن_پورو
	بنانشان صحيح		ttwu_pending;
	بنانشان ڊگهو		cpu_صلاحيت;
#جيڪڏهن_وصف_ٿيل CONFIG_SCHED_PROXY_EXEC
	بنية task_struct __rcu	*donor;  /* Scheduling context */
	بنية task_struct __rcu	*curr;   /* Execution context */
#وإلا
	اتحاد {
		بنية task_struct __rcu *donor; /* Scheduler context */
		بنية task_struct __rcu *curr;  /* Execution context */
	};
#جيڪڏهن_پورو
	بنية task_struct	*idle;
	/* padding left here deliberately */

	/*
	 * The next cacheline holds the (hot) runqueue lock, as well as
	 * some other less performance-critical fields.
	 */
	u64			nr_switches	____cacheline_aligned;

	/* runqueue lock: */
	raw_spinlock_t		__lock;

#جيڪڏهن_وصف_ٿيل CONFIG_NO_HZ_COMMON
	بنانشان صحيح		nohz_tick_stopped;
	atomic_t		nohz_flags;
	بنانشان صحيح		has_blocked_ڀريو;
	بنانشان ڊگهو		آخري_blocked_ڀريو_update_tick;
	call_single_معلومات_t	nohz_csd;
#جيڪڏهن_پورو /* CONFIG_NO_HZ_COMMON */

#جيڪڏهن_وصف_ٿيل CONFIG_UCLAMP_TASK
	/* Utilization clamp values based on CPU's RUNNABLE tasks */
	بنية uclamp_rq	uclamp[UCLAMP_CNT] ____cacheline_aligned;
	بنانشان صحيح		uclamp_flags;
#وصف_ڪريو UCLAMP_FLAG_IDLE 0x01
#جيڪڏهن_پورو

	بنية cfs_rq		cfs;
	بنية rt_rq		rt;
	بنية dl_rq		dl;
#جيڪڏهن_وصف_ٿيل CONFIG_SCHED_CLASS_EXT
	بنية scx_rq		scx;
	بنية sched_dl_entity	ext_server;
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل CONFIG_SCHED_CACHE
	raw_spinlock_t		cpu_epoch_تالو ____cacheline_aligned;
	u64			cpu_runtime;
	بنانشان ڊگهو		cpu_epoch;
	بنانشان ڊگهو		cpu_epoch_ايندڙ;
#جيڪڏهن_پورو

	بنية sched_dl_entity	fair_server;

#جيڪڏهن_وصف_ٿيل CONFIG_FAIR_گروهه_SCHED
	/* list of leaf cfs_rq on this CPU: */
	بنية list_سر	پن_cfs_rq_list;
	بنية list_سر	*tmp_alone_branch;
#جيڪڏهن_پورو /* CONFIG_FAIR_GROUP_SCHED */

#جيڪڏهن_وصف_ٿيل CONFIG_NUMA_BALANCING
	بنانشان صحيح		numa_migrate_on;
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل CONFIG_SCHED_CACHE
	بنانشان صحيح		nr_pref_llc_running;
	بنانشان صحيح		nr_llc_running;
#جيڪڏهن_پورو

	/*
	 * This is part of a global counter where only the total sum
	 * over all CPUs matters. A task can increase this counter on
	 * one CPU and if it got migrated afterwards it may decrease
	 * it on another CPU. Always updated under the runqueue lock:
	 */
	بنانشان ڊگهو		nr_uninterruptible;

	بنية sched_dl_entity	*dl_server;
	بنية task_struct	*stop;
	ثابت بنية sched_class *ايندڙ_class;
	بنانشان ڊگهو		ايندڙ_balance;
	بنية mm_struct	*prev_mm;

	/*
	 * The following fields of clock data are frequently referenced
	 * and updated together, and should go on their own cache line.
	 */
	u64			clock_task ____cacheline_aligned;
	u64			clock_pelt;
	u64			clock;
	بنانشان ڊگهو		گم_idle_وقت;
	بنانشان صحيح		clock_update_flags;
	u64			clock_pelt_idle;
	u64			clock_idle;

#جيڪڏهن_وصف_نه_ٿيل CONFIG_64_ٻٽو_انگ
	u64			clock_pelt_idle_copy;
	u64			clock_idle_copy;
#جيڪڏهن_پورو

	u64 آخري_ڏٺل_need_resched_ns;
	صحيح ticks_without_resched;

#جيڪڏهن_وصف_ٿيل CONFIG_MEMBARRIER
	صحيح membarrier_state;
#جيڪڏهن_پورو

	بنية پاڙ_domain		*rd;
	بنية sched_domain __rcu	*sd;

	بنية balance_callback *balance_callback;

	بنانشان حرف		nohz_idle_balance;
	بنانشان حرف		idle_balance;

	بنانشان ڊگهو		misfit_task_ڀريو;

	/* For active balancing */
	صحيح			سرگرم_balance;
	صحيح			ڌڪو_cpu;
	بنية cpu_stop_work	سرگرم_balance_work;

	/* CPU of this runqueue: */
	صحيح			cpu;
	صحيح			online;

	بنية list_سر cfs_tasks;

	بنية sched_avg	avg_rt;
	بنية sched_avg	avg_dl;
#جيڪڏهن_وصف_ٿيل CONFIG_HAVE_SCHED_AVG_IRQ
	بنية sched_avg	avg_irq;
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل CONFIG_SCHED_HW_PRESSURE
	بنية sched_avg	avg_hw;
#جيڪڏهن_پورو
	u64			idle_stamp;
	u64			avg_idle;

	/* This is used to determine avg_idle's max value */
	u64			max_idle_balance_لاڳت;

#جيڪڏهن_وصف_ٿيل CONFIG_HOTPLUG_CPU
	بنية rcuwait		hotplug_wait;
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل CONFIG_IRQ_وقت_ACCOUNTING
	u64			prev_irq_وقت;
	u64			psi_irq_وقت;
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل CONFIG_PARAVIRT
	u64			prev_steal_وقت;
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل CONFIG_PARAVIRT_وقت_ACCOUNTING
	u64			prev_steal_وقت_rq;
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل CONFIG_PREFERRED_CPU
	bool			npc_ڌڪو_work_pending;
#جيڪڏهن_پورو

	/* calc_load related fields */
	بنانشان ڊگهو		calc_ڀريو_update;
	ڊگهو			calc_ڀريو_سرگرم;

#جيڪڏهن_وصف_ٿيل CONFIG_SCHED_HRTICK
	call_single_معلومات_t	hrtick_csd;
	بنية hrtimer		hrtick_timer;
	ktime_t			hrtick_وقت;
	ktime_t			hrtick_delay;
	بنانشان صحيح		hrtick_sched;
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل CONFIG_SCHEDSTATS
	/* latency stats */
	بنية sched_info	rq_sched_info;
	بنانشان ڊگهو ڊگهو	rq_cpu_وقت;

	/* sys_sched_yield() stats */
	بنانشان صحيح		yld_ڳڻپ;

	/* schedule() stats */
	بنانشان صحيح		sched_ڳڻپ;
	بنانشان صحيح		sched_goidle;

	/* try_to_wake_up() stats */
	بنانشان صحيح		ttwu_ڳڻپ;
	بنانشان صحيح		ttwu_local;
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل CONFIG_CPU_IDLE
	/* Must be inspected within a RCU lock section */
	بنية cpuidle_state	*idle_state;
#جيڪڏهن_پورو

	بنانشان صحيح		nr_pinned;
	بنانشان صحيح		ڌڪو_busy;
	بنية cpu_stop_work	ڌڪو_work;

#جيڪڏهن_وصف_ٿيل CONFIG_SCHED_CORE
	/* per rq */
	بنية rq		*core;
	بنية task_struct	*core_pick;
	بنية sched_dl_entity	*core_dl_server;
	بنانشان صحيح		core_enabled;
	بنية rb_پاڙ		core_وڻ;

	/* shared state -- careful with sched_core_cpu_deactivate() */
	بنانشان صحيح		core_task_seq;
	بنانشان صحيح		core_pick_seq;
	بنانشان ڊگهو		core_cookie;
	بنانشان صحيح		core_forceidle_ڳڻپ;
	بنانشان صحيح		core_forceidle_seq;
	بنانشان صحيح		core_forceidle_occupation;
	u64			core_forceidle_شروعات;
	بنانشان صحيح		core_pick_اندريون_flight;
#جيڪڏهن_پورو /* CONFIG_SCHED_CORE */

	/* Scratch cpumask to be temporarily used under rq_lock */
	cpumask_var_t		scratch_نقاب;

#جيڪڏهن_وصف_ٿيل CONFIG_CFS_BANDWIDTH
	call_single_معلومات_t	cfsb_csd;
	بنية list_سر	cfsb_csd_list;
#جيڪڏهن_پورو

	atomic_t		nr_iowait;
} __no_randomize_layout;

#جيڪڏهن_وصف_ٿيل CONFIG_FAIR_گروهه_SCHED

/* CPU runqueue to which this cfs_rq is attached */
ساكن مضمن بنية rq *rq_جو(بنية cfs_rq *cfs_rq)
{
	أرجع cfs_rq->rq;
}

#وإلا /* !CONFIG_FAIR_GROUP_SCHED: */

ساكن مضمن بنية rq *rq_جو(بنية cfs_rq *cfs_rq)
{
	أرجع container_جو(cfs_rq, بنية rq, cfs);
}
#جيڪڏهن_پورو /* !CONFIG_FAIR_GROUP_SCHED */

ساكن مضمن صحيح cpu_جو(بنية rq *rq)
{
	أرجع rq->cpu;
}

#وصف_ڪريو MDF_ڌڪو		0x01

ساكن مضمن bool is_migration_disabled(بنية task_struct *p)
{
	أرجع p->migration_disabled;
}

DECLARE_في_CPU_SHARED_ALIGNED(بنية rq, runqueues);
DECLARE_في_CPU(بنية rnd_state, sched_rnd_state);

ساكن مضمن u32 sched_rng(فراغ)
{
	أرجع prandom_u32_state(this_cpu_ptr(&sched_rnd_state));
}

ساكن __always_inline بنية rq *__this_rq(فراغ)
{
	أرجع this_cpu_ptr(&runqueues);
}

#وصف_ڪريو cpu_rq(cpu)		(&في_cpu(runqueues, (cpu)))
#وصف_ڪريو this_rq()		__this_rq()
#وصف_ڪريو task_rq(p)		cpu_rq(task_cpu(p))
#وصف_ڪريو cpu_curr(cpu)		(cpu_rq(cpu)->curr)
#وصف_ڪريو raw_rq()		raw_cpu_ptr(&runqueues)

ساكن مضمن bool idle_rq(بنية rq *rq)
{
	أرجع rq->curr == rq->idle && !rq->nr_running && !rq->ttwu_pending;
}

/**
 * available_idle_cpu - is a given CPU idle for enqueuing work.
 * @cpu: the CPU in question.
 *
 * Return: 1 if the CPU is currently idle. 0 otherwise.
 */
ساكن مضمن bool موجود_idle_cpu(صحيح cpu)
{
	إذا (!idle_rq(cpu_rq(cpu)))
		أرجع 0;

	إذا (vcpu_is_preempted(cpu))
		أرجع 0;

	أرجع 1;
}

#جيڪڏهن_وصف_ٿيل CONFIG_SCHED_PROXY_EXEC
ساكن مضمن فراغ rq_سيٽ_donor(بنية rq *rq, بنية task_struct *t)
{
	rcu_سونپيو_اشارو(rq->donor, t);
}
#وإلا
ساكن مضمن فراغ rq_سيٽ_donor(بنية rq *rq, بنية task_struct *t)
{
	/* Do nothing */
}
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل CONFIG_SCHED_CORE
ساكن مضمن بنية cpumask *sched_گروهه_وسعت(بنية sched_گروهه *sg);

DECLARE_STATIC_ڪنجي_FALSE(__sched_core_enabled);

ساكن مضمن bool sched_core_enabled(بنية rq *rq)
{
	أرجع static_branch_unlikely(&__sched_core_enabled) && rq->core_enabled;
}

ساكن مضمن bool sched_core_disabled(فراغ)
{
	أرجع !static_branch_unlikely(&__sched_core_enabled);
}

/*
 * Be careful with this function; not for general use. The return value isn't
 * stable unless you actually hold a relevant rq->__lock.
 */
ساكن مضمن raw_spinlock_t *rq_lockp(بنية rq *rq)
{
	إذا (sched_core_enabled(rq))
		أرجع &rq->core->__lock;

	أرجع &rq->__lock;
}

ساكن مضمن raw_spinlock_t *__rq_lockp(بنية rq *rq)
	__returns_ctx_lock(rq_lockp(rq)) /* alias them */
{
	إذا (rq->core_enabled)
		أرجع &rq->core->__lock;

	أرجع &rq->__lock;
}

خارجي bool
cfs_prio_less(ثابت بنية task_struct *a, ثابت بنية task_struct *b, bool fi);

خارجي فراغ task_vruntime_update(بنية rq *rq, بنية task_struct *p, bool اندريون_fi);

/*
 * Helpers to check if the CPU's core cookie matches with the task's cookie
 * when core scheduling is enabled.
 * A special case is that the task's cookie always matches with CPU's core
 * cookie if the CPU is in an idle core.
 */
ساكن مضمن bool sched_cpu_cookie_ميلاپ(بنية rq *rq, بنية task_struct *p)
{
	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	إذا (!sched_core_enabled(rq))
		أرجع true;

	أرجع rq->core->core_cookie == p->core_cookie;
}

ساكن مضمن bool sched_core_cookie_ميلاپ(بنية rq *rq, بنية task_struct *p)
{
	bool idle_core = true;
	صحيح cpu;

	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	إذا (!sched_core_enabled(rq))
		أرجع true;

	إذا (rq->core->core_cookie == p->core_cookie)
		أرجع true;

	for_each_cpu(cpu, cpu_smt_نقاب(cpu_جو(rq))) {
		إذا (!موجود_idle_cpu(cpu)) {
			idle_core = false;
			اكسر;
		}
	}

	/*
	 * A CPU in an idle core is always the best choice for tasks with
	 * cookies.
	 */
	أرجع idle_core;
}

ساكن مضمن bool sched_گروهه_cookie_ميلاپ(بنية rq *rq,
					    بنية task_struct *p,
					    بنية sched_گروهه *گروهه)
{
	صحيح cpu;

	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	إذا (!sched_core_enabled(rq))
		أرجع true;

	for_each_cpu_and(cpu, sched_گروهه_وسعت(گروهه), p->cpus_ptr) {
		إذا (sched_core_cookie_ميلاپ(cpu_rq(cpu), p))
			أرجع true;
	}
	أرجع false;
}

ساكن مضمن bool sched_core_enqueued(بنية task_struct *p)
{
	أرجع !RB_خالي_جايون_ڳنڍ(&p->core_ڳنڍ);
}

خارجي فراغ sched_core_قطار_اندر_وجهو(بنية rq *rq, بنية task_struct *p);
خارجي فراغ sched_core_قطار_مان_ڪڍو(بنية rq *rq, بنية task_struct *p, صحيح flags);

خارجي فراغ sched_core_get(فراغ);
خارجي فراغ sched_core_put(فراغ);

ساكن مضمن bool task_has_sched_core(بنية task_struct *p)
{
	إذا (sched_core_disabled())
		أرجع false;

	أرجع !!p->core_cookie;
}

#وإلا /* !CONFIG_SCHED_CORE: */

ساكن مضمن bool sched_core_enabled(بنية rq *rq)
{
	أرجع false;
}

ساكن مضمن bool sched_core_disabled(فراغ)
{
	أرجع true;
}

ساكن مضمن raw_spinlock_t *rq_lockp(بنية rq *rq)
{
	أرجع &rq->__lock;
}

ساكن مضمن raw_spinlock_t *__rq_lockp(بنية rq *rq)
	__returns_ctx_lock(rq_lockp(rq)) /* alias them */
{
	أرجع &rq->__lock;
}

ساكن مضمن bool sched_cpu_cookie_ميلاپ(بنية rq *rq, بنية task_struct *p)
{
	أرجع true;
}

ساكن مضمن bool sched_core_cookie_ميلاپ(بنية rq *rq, بنية task_struct *p)
{
	أرجع true;
}

ساكن مضمن bool sched_گروهه_cookie_ميلاپ(بنية rq *rq,
					    بنية task_struct *p,
					    بنية sched_گروهه *گروهه)
{
	أرجع true;
}

ساكن مضمن bool task_has_sched_core(بنية task_struct *p)
{
	أرجع false;
}

#جيڪڏهن_پورو /* !CONFIG_SCHED_CORE */

#جيڪڏهن_وصف_ٿيل CONFIG_RT_گروهه_SCHED
# ifdef CONFIG_RT_گروهه_SCHED_DEFAULT_DISABLED
DECLARE_STATIC_ڪنجي_FALSE(rt_گروهه_sched);
ساكن مضمن bool rt_گروهه_sched_enabled(فراغ)
{
	أرجع static_branch_unlikely(&rt_گروهه_sched);
}
# وإلا /* !CONFIG_RT_GROUP_SCHED_DEFAULT_DISABLED: */
DECLARE_STATIC_ڪنجي_TRUE(rt_گروهه_sched);
ساكن مضمن bool rt_گروهه_sched_enabled(فراغ)
{
	أرجع static_branch_likely(&rt_گروهه_sched);
}
# endif /* !CONFIG_RT_GROUP_SCHED_DEFAULT_DISABLED */
#وإلا /* !CONFIG_RT_GROUP_SCHED: */
# define rt_گروهه_sched_enabled()	false
#جيڪڏهن_پورو /* !CONFIG_RT_GROUP_SCHED */

ساكن مضمن فراغ lockdep_assert_rq_held(بنية rq *rq)
	__assumes_ctx_lock(__rq_lockp(rq))
{
	lockdep_assert_held(__rq_lockp(rq));
}

خارجي فراغ raw_spin_rq_تالو_nested(بنية rq *rq, صحيح subclass)
	__acquires(__rq_lockp(rq));

خارجي bool raw_spin_rq_trylock(بنية rq *rq)
	__cond_acquires(true, __rq_lockp(rq));

ساكن مضمن فراغ raw_spin_rq_تالو(بنية rq *rq)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_تالو_nested(rq, 0);
}

ساكن مضمن فراغ raw_spin_rq_unlock(بنية rq *rq)
	__releases(__rq_lockp(rq))
{
	raw_spin_unlock(rq_lockp(rq));
}

ساكن مضمن فراغ raw_spin_rq_تالو_irq(بنية rq *rq)
	__acquires(__rq_lockp(rq))
{
	local_irq_disable();
	raw_spin_rq_تالو(rq);
}

ساكن مضمن فراغ raw_spin_rq_unlock_irq(بنية rq *rq)
	__releases(__rq_lockp(rq))
{
	raw_spin_rq_unlock(rq);
	local_irq_enable();
}

ساكن مضمن بنانشان ڊگهو raw_spin_rq_تالو_irqsave_2(بنية rq *rq)
	__acquires(__rq_lockp(rq))
{
	بنانشان ڊگهو flags;

	local_irq_save(flags);
	raw_spin_rq_تالو(rq);

	أرجع flags;
}

ساكن مضمن فراغ raw_spin_rq_unlock_irqrestore(بنية rq *rq, بنانشان ڊگهو flags)
	__releases(__rq_lockp(rq))
{
	raw_spin_rq_unlock(rq);
	local_irq_restore(flags);
}

#وصف_ڪريو raw_spin_rq_تالو_irqsave(rq, flags)	\
افعل {						\
	flags = raw_spin_rq_تالو_irqsave_2(rq);	\
} طالما (0)

خارجي فراغ __update_idle_core(بنية rq *rq);

ساكن مضمن فراغ update_idle_core(بنية rq *rq)
{
	إذا (sched_smt_سرگرم())
		__update_idle_core(rq);
}

#جيڪڏهن_وصف_ٿيل CONFIG_FAIR_گروهه_SCHED
ساكن مضمن بنية task_struct *task_جو(بنية sched_entity *se)
{
	WARN_ON_ONCE(!entity_is_task(se));
	أرجع container_جو(se, بنية task_struct, se);
}

ساكن مضمن بنية cfs_rq *task_cfs_rq(بنية task_struct *p)
{
	أرجع p->se.cfs_rq;
}

/* runqueue on which this entity is (to be) queued */
ساكن مضمن بنية cfs_rq *cfs_rq_جو(ثابت بنية sched_entity *se)
{
	أرجع se->cfs_rq;
}

/* runqueue "owned" by this group */
ساكن مضمن بنية cfs_rq *گروهه_cfs_rq(بنية sched_entity *grp)
{
	أرجع grp->my_q;
}

#وإلا /* !CONFIG_FAIR_GROUP_SCHED: */

#وصف_ڪريو task_جو(_se)		container_جو(_se, بنية task_struct, se)

ساكن مضمن بنية cfs_rq *task_cfs_rq(ثابت بنية task_struct *p)
{
	أرجع &task_rq(p)->cfs;
}

ساكن مضمن بنية cfs_rq *cfs_rq_جو(ثابت بنية sched_entity *se)
{
	ثابت بنية task_struct *p = task_جو(se);
	بنية rq *rq = task_rq(p);

	أرجع &rq->cfs;
}

/* runqueue "owned" by this group */
ساكن مضمن بنية cfs_rq *گروهه_cfs_rq(بنية sched_entity *grp)
{
	أرجع عدم;
}

#جيڪڏهن_پورو /* !CONFIG_FAIR_GROUP_SCHED */

خارجي فراغ update_rq_avg_idle(بنية rq *rq);
خارجي فراغ update_rq_clock(بنية rq *rq);

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
#وصف_ڪريو RQCF_REQ_SKIP		0x01
#وصف_ڪريو RQCF_ACT_SKIP		0x02
#وصف_ڪريو RQCF_UPDATED		0x04

ساكن مضمن فراغ assert_clock_updated(بنية rq *rq)
{
	/*
	 * The only reason for not seeing a clock update since the
	 * last rq_pin_lock() is if we're currently skipping updates.
	 */
	WARN_ON_ONCE(rq->clock_update_flags < RQCF_ACT_SKIP);
}

ساكن مضمن u64 rq_clock(بنية rq *rq)
{
	lockdep_assert_rq_held(rq);
	assert_clock_updated(rq);

	أرجع rq->clock;
}

ساكن مضمن u64 rq_clock_task(بنية rq *rq)
{
	lockdep_assert_rq_held(rq);
	assert_clock_updated(rq);

	أرجع rq->clock_task;
}

ساكن مضمن فراغ rq_clock_skip_update(بنية rq *rq)
{
	lockdep_assert_rq_held(rq);
	rq->clock_update_flags |= RQCF_REQ_SKIP;
}

/*
 * See rt task throttling, which is the only time a skip
 * request is canceled.
 */
ساكن مضمن فراغ rq_clock_cancel_skipupdate(بنية rq *rq)
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
ساكن مضمن فراغ rq_clock_شروعات_loop_update(بنية rq *rq)
{
	lockdep_assert_rq_held(rq);
	WARN_ON_ONCE(rq->clock_update_flags & RQCF_ACT_SKIP);
	rq->clock_update_flags |= RQCF_ACT_SKIP;
}

ساكن مضمن فراغ rq_clock_stop_loop_update(بنية rq *rq)
{
	lockdep_assert_rq_held(rq);
	rq->clock_update_flags &= ~RQCF_ACT_SKIP;
}

بنية rq_flags {
	بنانشان ڊگهو flags;
	بنية pin_cookie cookie;
	/*
	 * A copy of (rq::clock_update_flags & RQCF_UPDATED) for the
	 * current pin context is stashed here in case it needs to be
	 * restored in rq_repin_lock().
	 */
	بنانشان صحيح clock_update_flags;
};

خارجي بنية balance_callback balance_ڌڪو_callback;

#جيڪڏهن_وصف_ٿيل CONFIG_SCHED_CLASS_EXT
خارجي ثابت بنية sched_class ext_sched_class;

DECLARE_STATIC_ڪنجي_FALSE(__scx_enabled);	/* SCX BPF scheduler loaded */
DECLARE_STATIC_ڪنجي_FALSE(__scx_switched_all);	/* all fair class tasks on SCX */

#وصف_ڪريو scx_enabled()		static_branch_unlikely(&__scx_enabled)
#وصف_ڪريو scx_switched_all()	static_branch_unlikely(&__scx_switched_all)

ساكن مضمن فراغ scx_rq_clock_update(بنية rq *rq, u64 clock)
{
	إذا (!scx_enabled())
		أرجع;
	WRITE_ONCE(rq->scx.clock, clock);
	smp_store_release(&rq->scx.flags, rq->scx.flags | SCX_RQ_CLK_VALID);
}

ساكن مضمن فراغ scx_rq_clock_invalidate(بنية rq *rq)
{
	إذا (!scx_enabled())
		أرجع;
	WRITE_ONCE(rq->scx.flags, rq->scx.flags & ~SCX_RQ_CLK_VALID);
}

#وإلا /* !CONFIG_SCHED_CLASS_EXT: */
#وصف_ڪريو scx_enabled()		false
#وصف_ڪريو scx_switched_all()	false

ساكن مضمن فراغ scx_rq_clock_update(بنية rq *rq, u64 clock) {}
ساكن مضمن فراغ scx_rq_clock_invalidate(بنية rq *rq) {}
#جيڪڏهن_پورو /* !CONFIG_SCHED_CLASS_EXT */

ساكن مضمن فراغ assert_balance_callbacks_خالي_جايون(بنية rq *rq)
{
	WARN_ON_ONCE(IS_ENABLED(CONFIG_PROVE_LOCKING) &&
		     rq->balance_callback &&
		     rq->balance_callback != &balance_ڌڪو_callback);
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
ساكن مضمن فراغ rq_pin_تالو(بنية rq *rq, بنية rq_flags *rf)
{
	rf->cookie = lockdep_pin_تالو(__rq_lockp(rq));

	rq->clock_update_flags &= (RQCF_REQ_SKIP|RQCF_ACT_SKIP);
	rf->clock_update_flags = 0;
	assert_balance_callbacks_خالي_جايون(rq);
}

ساكن مضمن فراغ rq_unpin_تالو(بنية rq *rq, بنية rq_flags *rf)
{
	إذا (rq->clock_update_flags > RQCF_ACT_SKIP)
		rf->clock_update_flags = RQCF_UPDATED;

	scx_rq_clock_invalidate(rq);
	lockdep_unpin_تالو(__rq_lockp(rq), rf->cookie);
}

ساكن مضمن فراغ rq_repin_تالو(بنية rq *rq, بنية rq_flags *rf)
{
	lockdep_repin_تالو(__rq_lockp(rq), rf->cookie);

	/*
	 * Restore the value we stashed in @rf for this pin context.
	 */
	rq->clock_update_flags |= rf->clock_update_flags;
}

#وصف_ڪريو __task_rq_lock(...) __acquire_ret(___task_rq_lock(__VA_ARGS__), __rq_lockp(__ret))
خارجي بنية rq *___task_rq_lock(بنية task_struct *p, بنية rq_flags *rf) __acquires_ret;

#وصف_ڪريو task_rq_تالو_2(...) __acquire_ret(task_rq_تالو(__VA_ARGS__), __rq_lockp(__ret))
خارجي بنية rq *task_rq_تالو(بنية task_struct *p, بنية rq_flags *rf)
	__acquires(&p->pi_تالو) __acquires_ret;

ساكن مضمن فراغ
__task_rq_unlock(بنية rq *rq, بنية task_struct *p, بنية rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_تالو(rq, rf);
	raw_spin_rq_unlock(rq);
}

ساكن مضمن فراغ
task_rq_unlock(بنية rq *rq, بنية task_struct *p, بنية rq_flags *rf)
	__releases(__rq_lockp(rq), &p->pi_تالو)
{
	__task_rq_unlock(rq, p, rf);
	raw_spin_unlock_irqrestore(&p->pi_تالو, rf->flags);
}

DEFINE_تالو_GUARD_1(task_rq_تالو_2, بنية task_struct,
		    _T->rq = task_rq_تالو_2(_T->تالو, &_T->rf),
		    task_rq_unlock(_T->rq, _T->تالو, &_T->rf),
		    بنية rq *rq; بنية rq_flags rf)
DECLARE_تالو_GUARD_1_ATTRS(task_rq_تالو_2, __acquires(_T->pi_تالو), __releases((*(بنية task_struct **)_T)->pi_تالو))
#وصف_ڪريو class_task_rq_تالو_constructor(_T) WITH_تالو_GUARD_1_ATTRS(task_rq_تالو_2, _T)

DEFINE_تالو_GUARD_1(__task_rq_lock, بنية task_struct,
		    _T->rq = __task_rq_lock(_T->تالو, &_T->rf),
		    __task_rq_unlock(_T->rq, _T->تالو, &_T->rf),
		    بنية rq *rq; بنية rq_flags rf)

ساكن مضمن فراغ rq_تالو_irqsave(بنية rq *rq, بنية rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_تالو_irqsave(rq, rf->flags);
	rq_pin_تالو(rq, rf);
}

ساكن مضمن فراغ rq_تالو_irq(بنية rq *rq, بنية rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_تالو_irq(rq);
	rq_pin_تالو(rq, rf);
}

ساكن مضمن فراغ rq_تالو(بنية rq *rq, بنية rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_تالو(rq);
	rq_pin_تالو(rq, rf);
}

ساكن مضمن فراغ rq_unlock_irqrestore(بنية rq *rq, بنية rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_تالو(rq, rf);
	raw_spin_rq_unlock_irqrestore(rq, rf->flags);
}

ساكن مضمن فراغ rq_unlock_irq(بنية rq *rq, بنية rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_تالو(rq, rf);
	raw_spin_rq_unlock_irq(rq);
}

ساكن مضمن فراغ rq_unlock(بنية rq *rq, بنية rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_تالو(rq, rf);
	raw_spin_rq_unlock(rq);
}

DEFINE_تالو_GUARD_1(rq_تالو, بنية rq,
		    rq_تالو(_T->تالو, &_T->rf),
		    rq_unlock(_T->تالو, &_T->rf),
		    بنية rq_flags rf)

DECLARE_تالو_GUARD_1_ATTRS(rq_تالو, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(بنية rq **)_T)));
#وصف_ڪريو class_rq_تالو_constructor(_T) WITH_تالو_GUARD_1_ATTRS(rq_تالو, _T)

DEFINE_تالو_GUARD_1(rq_تالو_irq, بنية rq,
		    rq_تالو_irq(_T->تالو, &_T->rf),
		    rq_unlock_irq(_T->تالو, &_T->rf),
		    بنية rq_flags rf)

DECLARE_تالو_GUARD_1_ATTRS(rq_تالو_irq, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(بنية rq **)_T)));
#وصف_ڪريو class_rq_تالو_irq_constructor(_T) WITH_تالو_GUARD_1_ATTRS(rq_تالو_irq, _T)

DEFINE_تالو_GUARD_1(rq_تالو_irqsave, بنية rq,
		    rq_تالو_irqsave(_T->تالو, &_T->rf),
		    rq_unlock_irqrestore(_T->تالو, &_T->rf),
		    بنية rq_flags rf)

DECLARE_تالو_GUARD_1_ATTRS(rq_تالو_irqsave, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(بنية rq **)_T)));
#وصف_ڪريو class_rq_تالو_irqsave_constructor(_T) WITH_تالو_GUARD_1_ATTRS(rq_تالو_irqsave, _T)

DEFINE_تالو_GUARD_1(raw_spin_rq_تالو_irqsave, بنية rq,
		    raw_spin_rq_تالو_irqsave(_T->تالو, _T->flags),
		    raw_spin_rq_unlock_irqrestore(_T->تالو, _T->flags),
		    بنانشان ڊگهو flags)

DECLARE_تالو_GUARD_1_ATTRS(raw_spin_rq_تالو_irqsave, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(بنية rq **)_T)));
#وصف_ڪريو class_raw_spin_rq_تالو_irqsave_constructor(_T) \
	WITH_تالو_GUARD_1_ATTRS(raw_spin_rq_تالو_irqsave, _T)

#وصف_ڪريو this_rq_تالو_irq(...) __acquire_ret(this_rq_تالو_irq_2(__VA_ARGS__), __rq_lockp(__ret))
ساكن مضمن بنية rq *this_rq_تالو_irq_2(بنية rq_flags *rf) __acquires_ret
{
	بنية rq *rq;

	local_irq_disable();
	rq = this_rq();
	rq_تالو(rq, rf);

	أرجع rq;
}

#جيڪڏهن_وصف_ٿيل CONFIG_NUMA

تعداد numa_topology_type {
	NUMA_سڌو,
	NUMA_GLUELESS_MESH,
	NUMA_BACKPLANE,
};

خارجي تعداد numa_topology_type sched_numa_topology_type;
خارجي صحيح sched_max_numa_مفاصلو;
خارجي bool ڳوليو_numa_مفاصلو(صحيح مفاصلو);
خارجي فراغ sched_init_numa(صحيح offline_ڳنڍ);
خارجي فراغ sched_update_numa(صحيح cpu, bool online);
خارجي فراغ sched_domains_numa_masks_سيٽ(بنانشان صحيح cpu);
خارجي فراغ sched_domains_numa_masks_clear(بنانشان صحيح cpu);
خارجي صحيح sched_numa_ڳوليو_closest(ثابت بنية cpumask *cpus, صحيح cpu);

#وإلا /* !CONFIG_NUMA: */

ساكن مضمن فراغ sched_init_numa(صحيح offline_ڳنڍ) { }
ساكن مضمن فراغ sched_update_numa(صحيح cpu, bool online) { }
ساكن مضمن فراغ sched_domains_numa_masks_سيٽ(بنانشان صحيح cpu) { }
ساكن مضمن فراغ sched_domains_numa_masks_clear(بنانشان صحيح cpu) { }

ساكن مضمن صحيح sched_numa_ڳوليو_closest(ثابت بنية cpumask *cpus, صحيح cpu)
{
	أرجع nr_cpu_ids;
}

#جيڪڏهن_پورو /* !CONFIG_NUMA */

#جيڪڏهن_وصف_ٿيل CONFIG_NUMA_BALANCING

/* The regions in numa_faults array from task_struct */
تعداد numa_نقص_stats {
	NUMA_MEM = 0,
	NUMA_CPU,
	NUMA_MEMBUF,
	NUMA_CPUBUF
};

خارجي فراغ sched_setnuma(بنية task_struct *p, صحيح ڳنڍ);
خارجي صحيح migrate_task_منزل(بنية task_struct *p, صحيح cpu);
خارجي صحيح migrate_مٽا_سٽا(بنية task_struct *p, بنية task_struct *t,
			صحيح cpu, صحيح scpu);
خارجي فراغ init_numa_balancing(u64 clone_flags, بنية task_struct *p);

#وإلا /* !CONFIG_NUMA_BALANCING: */

ساكن مضمن فراغ
init_numa_balancing(u64 clone_flags, بنية task_struct *p)
{
}

#جيڪڏهن_پورو /* !CONFIG_NUMA_BALANCING */

صحيح task_llc(ثابت بنية task_struct *p);

ساكن مضمن فراغ
قطار_balance_callback(بنية rq *rq,
		       بنية balance_callback *سر,
		       فراغ (*func)(بنية rq *rq))
{
	lockdep_assert_rq_held(rq);

	/*
	 * Don't (re)queue an already queued item; nor queue anything when
	 * balance_push() is active, see the comment with
	 * balance_push_callback.
	 */
	إذا (unlikely(سر->ايندڙ || rq->balance_callback == &balance_ڌڪو_callback))
		أرجع;

	سر->func = func;
	سر->ايندڙ = rq->balance_callback;
	rq->balance_callback = سر;
}

#وصف_ڪريو rcu_dereference_sched_domain(p) \
	rcu_dereference_all_check((p), lockdep_is_held(&sched_domains_mutex))

/*
 * The domain tree (rq->sd) is protected by RCU's quiescent state transition.
 * See destroy_sched_domains: call_rcu for details.
 *
 * The domain tree of any CPU may only be accessed from within
 * preempt-disabled sections.
 */
#وصف_ڪريو for_each_domain(cpu, __sd) \
	لكل (__sd = rcu_dereference_sched_domain(cpu_rq(cpu)->sd); \
			__sd; __sd = __sd->والد)

/* A mask of all the SD flags that have the SDF_SHARED_CHILD metaflag */
#وصف_ڪريو SD_FLAG(name, mflags) (name * !!((mflags) & SDF_SHARED_ٻار)) |
ساكن ثابت بنانشان صحيح SD_SHARED_ٻار_نقاب =
#ضمن <linux/sched/sd_flags.h>
0;
#وصف_رد SD_FLAG

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
ساكن مضمن بنية sched_domain *highest_flag_domain(صحيح cpu, صحيح flag)
{
	بنية sched_domain *sd, *hsd = عدم;

	for_each_domain(cpu, sd) {
		إذا (sd->flags & flag) {
			hsd = sd;
			استمر;
		}

		/*
		 * Stop the search if @flag is known to be shared at lower
		 * levels. It will not be found further up.
		 */
		إذا (flag & SD_SHARED_ٻار_نقاب)
			اكسر;
	}

	أرجع hsd;
}

ساكن مضمن بنية sched_domain *سڀ_کان_هيٺ_flag_domain(صحيح cpu, صحيح flag)
{
	بنية sched_domain *sd;

	for_each_domain(cpu, sd) {
		إذا (sd->flags & flag)
			اكسر;
	}

	أرجع sd;
}

DECLARE_في_CPU(بنية sched_domain __rcu *, sd_llc);
DECLARE_في_CPU(صحيح, sd_llc_سائيز);
DECLARE_في_CPU(صحيح, sd_llc_id);
DECLARE_في_CPU(صحيح, sd_share_id);
DECLARE_في_CPU(بنية sched_domain_shared __rcu *, sd_llc_shared);
DECLARE_في_CPU(بنية sched_domain_shared __rcu *, sd_balance_shared);
DECLARE_في_CPU(بنية sched_domain __rcu *, sd_numa);
DECLARE_في_CPU(بنية sched_domain __rcu *, sd_asym_packing);
DECLARE_في_CPU(بنية sched_domain __rcu *, sd_asym_cpucapacity);

خارجي بنية static_ڪنجي_false sched_asym_cpucapacity;
خارجي بنية static_ڪنجي_false sched_cluster_سرگرم;

ساكن __always_inline bool sched_asym_cpucap_سرگرم(فراغ)
{
	أرجع static_branch_unlikely(&sched_asym_cpucapacity);
}

بنية sched_گروهه_صلاحيت {
	atomic_t		ref;
	/*
	 * CPU capacity of this group, SCHED_CAPACITY_SCALE being max capacity
	 * for a single CPU.
	 */
	بنانشان ڊگهو		صلاحيت;
	بنانشان ڊگهو		min_صلاحيت;		/* Min per-CPU capacity in group */
	بنانشان ڊگهو		max_صلاحيت;		/* Max per-CPU capacity in group */
	بنانشان ڊگهو		ايندڙ_update;
	صحيح			imbalance;		/* XXX unrelated to capacity but shared group state */

	صحيح			id;

	بنانشان ڊگهو		cpumask[];		/* Balance mask */
};

بنية sched_گروهه {
	بنية sched_گروهه	*ايندڙ;			/* Must be a circular list */
	atomic_t		ref;

	بنانشان صحيح		گروهه_وزن;
	بنانشان صحيح		cores;
	بنية sched_گروهه_صلاحيت *sgc;
	صحيح			asym_prefer_cpu;	/* CPU of highest priority in group */
	صحيح			flags;

	/*
	 * The CPUs this group covers.
	 *
	 * NOTE: this field is variable length. (Allocated dynamically
	 * by attaching extra space to the end of the structure,
	 * depending on how many CPUs the kernel has booted up with)
	 */
	بنانشان ڊگهو		cpumask[];
};

ساكن مضمن بنية cpumask *sched_گروهه_وسعت(بنية sched_گروهه *sg)
{
	أرجع منزل_cpumask(sg->cpumask);
}

/*
 * See build_balance_mask().
 */
ساكن مضمن بنية cpumask *گروهه_balance_نقاب(بنية sched_گروهه *sg)
{
	أرجع منزل_cpumask(sg->sgc->cpumask);
}

خارجي صحيح گروهه_balance_cpu(بنية sched_گروهه *sg);

خارجي فراغ update_sched_domain_debugfs(فراغ);
خارجي فراغ dirty_sched_domain_sysctl(صحيح cpu);

خارجي صحيح sched_update_scaling(فراغ);

ساكن مضمن ثابت بنية cpumask *task_user_cpus(بنية task_struct *p)
{
	إذا (!p->user_cpus_ptr)
		أرجع cpu_possible_نقاب; /* &init_task.cpus_mask */
	أرجع p->user_cpus_ptr;
}

#جيڪڏهن_وصف_ٿيل CONFIG_CGROUP_SCHED

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
ساكن مضمن بنية task_گروهه *task_گروهه(بنية task_struct *p)
{
	أرجع p->sched_task_گروهه;
}

#جيڪڏهن_وصف_ٿيل CONFIG_FAIR_گروهه_SCHED
/*
 * Defined here to be available before stats.h is included, since
 * stats.h has dependencies on things defined later in this file.
 */
بنية cfs_tg_state {
	بنية cfs_rq		cfs_rq;
	بنية sched_entity	se;
	بنية sched_statistics	stats;
} __no_randomize_layout;

/* Access a specific CPU's cfs_rq from a task group */
ساكن مضمن بنية cfs_rq *tg_cfs_rq(بنية task_گروهه *tg, صحيح cpu)
{
	أرجع في_cpu_ptr(tg->cfs_rq, cpu);
}

ساكن مضمن بنية sched_entity *tg_se(بنية task_گروهه *tg, صحيح cpu)
{
	بنية cfs_tg_state *state;

	إذا (is_پاڙ_task_گروهه(tg))
		أرجع عدم;

	state = container_جو(tg_cfs_rq(tg, cpu), بنية cfs_tg_state, cfs_rq);
	أرجع &state->se;
}

ساكن مضمن بنية sched_entity *cfs_rq_se(بنية cfs_rq *cfs_rq)
{
	بنية cfs_tg_state *state;

	إذا (is_پاڙ_task_گروهه(cfs_rq->tg))
		أرجع عدم;

	state = container_جو(cfs_rq, بنية cfs_tg_state, cfs_rq);
	أرجع &state->se;
}
#جيڪڏهن_پورو

/* Change a task's cfs_rq and parent entity if it moves across CPUs/groups */
ساكن مضمن فراغ سيٽ_task_rq(بنية task_struct *p, بنانشان صحيح cpu)
{
#إذا defined(CONFIG_FAIR_گروهه_SCHED) || defined(CONFIG_RT_گروهه_SCHED)
	بنية task_گروهه *tg = task_گروهه(p);
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل CONFIG_FAIR_گروهه_SCHED
	سيٽ_task_rq_fair(&p->se, p->se.cfs_rq, tg_cfs_rq(tg, cpu));
	p->se.cfs_rq = tg_cfs_rq(tg, cpu);
	p->se.والد = tg_se(tg, cpu);
	p->se.اونهائي = p->se.والد ? p->se.والد->اونهائي + 1 : 0;
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل CONFIG_RT_گروهه_SCHED
	/*
	 * p->rt.rt_rq is NULL initially and it is easier to assign
	 * root_task_group's rt_rq than switching in rt_rq_of_se()
	 * Clobbers tg(!)
	 */
	إذا (!rt_گروهه_sched_enabled())
		tg = &پاڙ_task_گروهه;
	p->rt.rt_rq  = tg->rt_rq[cpu];
	p->rt.والد = tg->rt_se[cpu];
#جيڪڏهن_پورو /* CONFIG_RT_GROUP_SCHED */
}

#وإلا /* !CONFIG_CGROUP_SCHED: */

ساكن مضمن فراغ سيٽ_task_rq(بنية task_struct *p, بنانشان صحيح cpu) { }

ساكن مضمن بنية task_گروهه *task_گروهه(بنية task_struct *p)
{
	أرجع عدم;
}

#جيڪڏهن_پورو /* !CONFIG_CGROUP_SCHED */

ساكن مضمن فراغ __set_task_cpu(بنية task_struct *p, بنانشان صحيح cpu)
{
	سيٽ_task_rq(p, cpu);
#جيڪڏهن_وصف_ٿيل CONFIG_SMP
	/*
	 * After ->cpu is set up to a new value, task_rq_lock(p, ...) can be
	 * successfully executed on another CPU. We must ensure that updates of
	 * per-task data have been completed by this moment.
	 */
	smp_wmb();
	WRITE_ONCE(task_thread_info(p)->cpu, cpu);
	p->wake_cpu = cpu;
	rseq_sched_سيٽ_ids_بدليو(p);
#جيڪڏهن_پورو /* CONFIG_SMP */
}

/*
 * Tunables:
 */

#وصف_ڪريو SCHED_FEAT(name, enabled)	\
	__SCHED_FEAT_##name ,

تعداد {
#ضمن "features.h"
	__SCHED_FEAT_NR,
};

#وصف_رد SCHED_FEAT

/*
 * To support run-time toggling of sched features, all the translation units
 * (but core.c) reference the sysctl_sched_features defined in core.c.
 */
خارجي __read_mostly بنانشان صحيح sysctl_sched_features;

#جيڪڏهن_وصف_ٿيل CONFIG_JUMP_LABEL

اتحاد sched_feat_ڪنجي {
	بنية static_ڪنجي_true	ڪنجي_true;
	بنية static_ڪنجي_false	ڪنجي_false;
};

#وصف_ڪريو sched_feat_branch_true(ڪنجي)	static_branch_likely(&(ڪنجي)->ڪنجي_true)
#وصف_ڪريو sched_feat_branch_false(ڪنجي)	static_branch_unlikely(&(ڪنجي)->ڪنجي_false)

#وصف_ڪريو SCHED_FEAT(name, enabled)			\
ساكن __always_inline bool				\
static_branch_##name(اتحاد sched_feat_ڪنجي *ڪنجي) 	\
{							\
	أرجع sched_feat_branch_##enabled(ڪنجي);	\
}

#ضمن "features.h"
#وصف_رد SCHED_FEAT

خارجي اتحاد sched_feat_ڪنجي sched_feat_ڪنجيون[__SCHED_FEAT_NR];
#وصف_ڪريو sched_feat(x) (static_branch_##x(&sched_feat_ڪنجيون[__SCHED_FEAT_##x]))

#وإلا /* !CONFIG_JUMP_LABEL: */

#وصف_ڪريو sched_feat(x) (sysctl_sched_features & (1UL << __SCHED_FEAT_##x))

#جيڪڏهن_پورو /* !CONFIG_JUMP_LABEL */

خارجي بنية static_ڪنجي_false sched_numa_balancing;
خارجي بنية static_ڪنجي_false sched_schedstats;

ساكن مضمن u64 global_rt_period(فراغ)
{
	أرجع (u64)sysctl_sched_rt_period * NSEC_في_USEC;
}

ساكن مضمن u64 global_rt_runtime(فراغ)
{
	إذا (sysctl_sched_rt_runtime < 0)
		أرجع RUNTIME_INF;

	أرجع (u64)sysctl_sched_rt_runtime * NSEC_في_USEC;
}

/*
 * Is p the current execution context?
 */
ساكن مضمن صحيح task_current(بنية rq *rq, بنية task_struct *p)
{
	أرجع rq->curr == p;
}

/*
 * Is p the current scheduling context?
 *
 * Note that it might be the current execution context at the same time if
 * rq->curr == rq->donor == p.
 */
ساكن مضمن صحيح task_current_donor(بنية rq *rq, بنية task_struct *p)
{
	أرجع rq->donor == p;
}

ساكن مضمن bool task_is_blocked(بنية task_struct *p)
{
	إذا (!sched_proxy_exec())
		أرجع false;

	أرجع !!p->blocked_on;
}

#جيڪڏهن_وصف_ٿيل CONFIG_SCHED_PROXY_EXEC
فراغ sched_proxy_بلاڪ_task(بنية rq *rq, بنية task_struct *p);
#وإلا
ساكن مضمن فراغ sched_proxy_بلاڪ_task(بنية rq *rq, بنية task_struct *p) {}
#جيڪڏهن_پورو

ساكن مضمن صحيح task_on_cpu(بنية rq *rq, بنية task_struct *p)
{
	أرجع p->on_cpu;
}

ساكن مضمن صحيح task_on_rq_queued(بنية task_struct *p)
{
	أرجع READ_ONCE(p->on_rq) == TASK_ON_RQ_QUEUED;
}

ساكن مضمن صحيح task_on_rq_migrating(بنية task_struct *p)
{
	أرجع READ_ONCE(p->on_rq) == TASK_ON_RQ_MIGRATING;
}

/* Wake flags. The first three directly map to some SD flag value */
#وصف_ڪريو WF_EXEC			0x02 /* Wakeup after exec; maps to SD_BALANCE_EXEC */
#وصف_ڪريو WF_FORK			0x04 /* Wakeup after fork; maps to SD_BALANCE_FORK */
#وصف_ڪريو WF_TTWU			0x08 /* Wakeup;            maps to SD_BALANCE_WAKE */
/*
 * Hint that the caller expects the waker to sleep soon.
 * Scheduler classes may use it for placement or preemption.
 * Callers must not rely on it to prevent migration,
 * preserve CPU locality or make the wakee run next.
 */
#وصف_ڪريو WF_SYNC			0x10
#وصف_ڪريو WF_MIGRATED		0x20 /* Internal use, task got migrated */
#وصف_ڪريو WF_CURRENT_CPU		0x40 /* Prefer to move the wakee to the current CPU. */
#وصف_ڪريو WF_RQ_SELECTED		0x80 /* ->select_task_rq() was called */
#وصف_ڪريو WF_TTWU_RQ		0x100 /* Wakeup completed through ttwu_runnable() */

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

#وصف_ڪريو وزن_IDLEPRIO		3
#وصف_ڪريو WMULT_IDLEPRIO		1431655765

خارجي ثابت صحيح		sched_prio_منزل_وزن[40];
خارجي ثابت u32		sched_prio_منزل_wmult[40];

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

#وصف_ڪريو قطار_مان_ڪڍو_SLEEP		0x0001 /* Matches ENQUEUE_WAKEUP */
#وصف_ڪريو قطار_مان_ڪڍو_SAVE		0x0002 /* Matches ENQUEUE_RESTORE */
#وصف_ڪريو قطار_مان_ڪڍو_MOVE		0x0004 /* Matches ENQUEUE_MOVE */
#وصف_ڪريو قطار_مان_ڪڍو_NOCLOCK		0x0008 /* Matches ENQUEUE_NOCLOCK */

#وصف_ڪريو قطار_مان_ڪڍو_MIGRATING	0x0010 /* Matches ENQUEUE_MIGRATING */
#وصف_ڪريو قطار_مان_ڪڍو_DELAYED		0x0020 /* Matches ENQUEUE_DELAYED */
#وصف_ڪريو قطار_مان_ڪڍو_CLASS		0x0040 /* Matches ENQUEUE_CLASS */

#وصف_ڪريو قطار_مان_ڪڍو_SPECIAL		0x00010000
#وصف_ڪريو قطار_مان_ڪڍو_THROTTLE	0x00020000

#وصف_ڪريو قطار_اندر_وجهو_WAKEUP		0x0001
#وصف_ڪريو قطار_اندر_وجهو_RESTORE		0x0002
#وصف_ڪريو قطار_اندر_وجهو_MOVE		0x0004
#وصف_ڪريو قطار_اندر_وجهو_NOCLOCK		0x0008

#وصف_ڪريو قطار_اندر_وجهو_MIGRATING	0x0010
#وصف_ڪريو قطار_اندر_وجهو_DELAYED		0x0020
#وصف_ڪريو قطار_اندر_وجهو_CLASS		0x0040

#وصف_ڪريو قطار_اندر_وجهو_سر		0x00010000
#وصف_ڪريو قطار_اندر_وجهو_REPLENISH	0x00020000
#وصف_ڪريو قطار_اندر_وجهو_MIGRATED	0x00040000
#وصف_ڪريو قطار_اندر_وجهو_INITIAL		0x00080000
#وصف_ڪريو قطار_اندر_وجهو_RQ_SELECTED	0x00100000
#وصف_ڪريو قطار_اندر_وجهو_QUEUED		0x00200000

#وصف_ڪريو RETRY_TASK		((فراغ *)-1UL)

بنية affinity_context {
	ثابت بنية cpumask	*new_نقاب;
	بنية cpumask		*user_نقاب;
	بنانشان صحيح		flags;
};

خارجي s64 update_curr_common(بنية rq *rq);

تعداد snt_e {
	SNT_عام,	/* set_next_task() */
	SNT_PICK,	/* put_prev_set_next_task(): prev != next */
	SNT_REPICK,	/* put_prev_set_next_task(): prev == next */
};

بنية sched_class {

#جيڪڏهن_وصف_ٿيل CONFIG_UCLAMP_TASK
	صحيح uclamp_enabled;
#جيڪڏهن_پورو

	/*
	 * move_queued_task/activate_task/enqueue_task: rq->lock
	 * ttwu_do_activate/activate_task/enqueue_task: rq->lock
	 * wake_up_new_task/activate_task/enqueue_task: task_rq_lock
	 * ttwu_runnable/enqueue_task: task_rq_lock
	 * proxy_task_current: rq->lock
	 * sched_change_end
	 */
	فراغ (*قطار_اندر_وجهو_task) (بنية rq *rq, بنية task_struct *p, صحيح flags);
	/*
	 * move_queued_task/deactivate_task/dequeue_task: rq->lock
	 * __schedule/block_task/dequeue_task: rq->lock
	 * proxy_task_current: rq->lock
	 * wait_task_inactive: task_rq_lock
	 * sched_change_begin
	 */
	bool (*قطار_مان_ڪڍو_task) (بنية rq *rq, بنية task_struct *p, صحيح flags);

	/*
	 * do_sched_yield: rq->lock
	 */
	فراغ (*yield_task)   (بنية rq *rq);
	/*
	 * yield_to: rq->lock (double)
	 */
	bool (*yield_منزل_task)(بنية rq *rq, بنية task_struct *p);

	/*
	 * move_queued_task: rq->lock
	 * __migrate_swap_task: rq->lock
	 * ttwu_do_activate: rq->lock
	 * ttwu_runnable: task_rq_lock
	 * wake_up_new_task: task_rq_lock
	 */
	فراغ (*wakeup_preempt)(بنية rq *rq, بنية task_struct *p, صحيح flags);

	/*
	 * schedule/pick_next_task/prev_balance: rq->lock
	 */
	صحيح (*balance)(بنية rq *rq, بنية rq_flags *rf);

	/*
	 * schedule/pick_next_task: rq->lock
	 */
	بنية task_struct *(*pick_task)(بنية rq *rq, بنية rq_flags *rf);

	/*
	 * sched_change:
	 * __schedule: rq->lock
	 */
	فراغ (*put_prev_task)(بنية rq *rq, بنية task_struct *p, بنية task_struct *ايندڙ);
	فراغ (*سيٽ_ايندڙ_task)(بنية rq *rq, بنية task_struct *p, تعداد snt_e type);

	/*
	 * select_task_rq: p->pi_lock
	 * sched_exec: p->pi_lock
	 */
	صحيح  (*select_task_rq)(بنية task_struct *p, صحيح task_cpu, صحيح flags);

	/*
	 * set_task_cpu: p->pi_lock || rq->lock (ttwu like)
	 */
	فراغ (*migrate_task_rq)(بنية task_struct *p, صحيح new_cpu);

	/*
	 * ttwu_do_activate: rq->lock
	 * wake_up_new_task: task_rq_lock
	 */
	فراغ (*task_woken)(بنية rq *this_rq, بنية task_struct *task);

	/*
	 * do_set_cpus_allowed: task_rq_lock + sched_change
	 */
	فراغ (*سيٽ_cpus_allowed)(بنية task_struct *p, بنية affinity_context *ctx);

	/*
	 * sched_set_rq_{on,off}line: rq->lock
	 */
	فراغ (*rq_online)(بنية rq *rq);
	فراغ (*rq_offline)(بنية rq *rq);

	/*
	 * push_cpu_stop: p->pi_lock && rq->lock
	 */
	بنية rq *(*ڳوليو_تالو_rq)(بنية task_struct *p, بنية rq *rq);

	/*
	 * hrtick: rq->lock
	 * sched_tick: rq->lock
	 * sched_tick_remote: rq->lock
	 */
	فراغ (*task_tick)(بنية rq *rq, بنية task_struct *p, صحيح queued);
	/*
	 * sched_cgroup_fork: p->pi_lock
	 */
	فراغ (*task_fork)(بنية task_struct *p);
	/*
	 * finish_task_switch: no locks
	 */
	فراغ (*task_dead)(بنية task_struct *p);

	/*
	 * sched_change
	 */
	فراغ (*switching_from)(بنية rq *this_rq, بنية task_struct *task);
	فراغ (*switched_from) (بنية rq *this_rq, بنية task_struct *task);
	فراغ (*switching_منزل)  (بنية rq *this_rq, بنية task_struct *task);
	فراغ (*switched_منزل)   (بنية rq *this_rq, بنية task_struct *task);
	u64  (*get_prio)     (بنية rq *this_rq, بنية task_struct *task);
	فراغ (*prio_بدليو) (بنية rq *this_rq, بنية task_struct *task,
			      u64 oldprio);

	/*
	 * set_load_weight: task_rq_lock + sched_change
	 * __setscheduler_parms: task_rq_lock + sched_change
	 */
	فراغ (*reweight_task)(بنية rq *this_rq, بنية task_struct *task,
			      ثابت بنية ڀريو_وزن *lw);

	/*
	 * sched_rr_get_interval: task_rq_lock
	 */
	بنانشان صحيح (*get_rr_interval)(بنية rq *rq,
					بنية task_struct *task);

	/*
	 * task_sched_runtime: task_rq_lock
	 */
	فراغ (*update_curr)(بنية rq *rq);

#جيڪڏهن_وصف_ٿيل CONFIG_FAIR_گروهه_SCHED
	/*
	 * sched_change_group: task_rq_lock + sched_change
	 */
	فراغ (*task_change_گروهه)(بنية task_struct *p);
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل CONFIG_SCHED_CORE
	/*
	 * pick_next_task: rq->lock
	 * try_steal_cookie: rq->lock (double)
	 */
	صحيح (*task_is_throttled)(بنية task_struct *p, صحيح cpu);
#جيڪڏهن_پورو
};

ساكن مضمن فراغ put_prev_task(بنية rq *rq, بنية task_struct *prev)
{
	WARN_ON_ONCE(rq->donor != prev);
	prev->sched_class->put_prev_task(rq, prev, عدم);
}

ساكن مضمن فراغ سيٽ_ايندڙ_task(بنية rq *rq, بنية task_struct *ايندڙ)
{
	ايندڙ->sched_class->سيٽ_ايندڙ_task(rq, ايندڙ, SNT_عام);
}

ساكن مضمن فراغ
__put_prev_set_next_dl_server(بنية rq *rq,
			      بنية task_struct *prev,
			      بنية task_struct *ايندڙ)
{
	prev->dl_server = عدم;
	ايندڙ->dl_server = rq->dl_server;
	rq->dl_server = عدم;
}

ساكن مضمن فراغ put_prev_سيٽ_ايندڙ_task(بنية rq *rq,
					  بنية task_struct *prev,
					  بنية task_struct *ايندڙ)
{
	WARN_ON_ONCE(rq->donor != prev);

	__put_prev_set_next_dl_server(rq, prev, ايندڙ);

	إذا (ايندڙ == prev) {
		ايندڙ->sched_class->سيٽ_ايندڙ_task(rq, ايندڙ, SNT_REPICK);
		أرجع;
	}

	prev->sched_class->put_prev_task(rq, prev, ايندڙ);
	ايندڙ->sched_class->سيٽ_ايندڙ_task(rq, ايندڙ, SNT_PICK);
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
#وصف_ڪريو DEFINE_SCHED_CLASS(name) \
ثابت بنية sched_class name##_sched_class \
	__aligned(__alignof__(بنية sched_class)) \
	__section("__" #name "_sched_class")

/* Defined in include/asm-generic/vmlinux.lds.h */
خارجي بنية sched_class __sched_class_highest[];
خارجي بنية sched_class __sched_class_lowest[];

خارجي ثابت بنية sched_class stop_sched_class;
خارجي ثابت بنية sched_class dl_sched_class;
خارجي ثابت بنية sched_class rt_sched_class;
خارجي ثابت بنية sched_class fair_sched_class;
خارجي ثابت بنية sched_class idle_sched_class;

/*
 * Iterate only active classes. SCX can take over all fair tasks or be
 * completely disabled. If the former, skip fair. If the latter, skip SCX.
 */
ساكن مضمن ثابت بنية sched_class *ايندڙ_سرگرم_class(ثابت بنية sched_class *class)
{
	class++;
#جيڪڏهن_وصف_ٿيل CONFIG_SCHED_CLASS_EXT
	إذا (scx_switched_all() && class == &fair_sched_class)
		class++;
	إذا (!scx_enabled() && class == &ext_sched_class)
		class++;
#جيڪڏهن_پورو
	أرجع class;
}

#وصف_ڪريو for_class_range(class, _from, منزل_2) \
	لكل (class = (_from); class < (منزل_2); class++)

#وصف_ڪريو for_each_class(class) \
	for_class_range(class, __sched_class_highest, __sched_class_lowest)

#وصف_ڪريو for_سرگرم_class_range(class, _from, منزل_2)				\
	لكل (class = (_from); class != (منزل_2); class = ايندڙ_سرگرم_class(class))

#وصف_ڪريو for_each_سرگرم_class(class)						\
	for_سرگرم_class_range(class, __sched_class_highest, __sched_class_lowest)

#وصف_ڪريو sched_class_above(_a, _b)	((_a) < (_b))

ساكن مضمن فراغ rq_modified_begin(بنية rq *rq, ثابت بنية sched_class *class)
{
	إذا (sched_class_above(rq->ايندڙ_class, class))
		rq->ايندڙ_class = class;
}

ساكن مضمن bool rq_modified_above(بنية rq *rq, ثابت بنية sched_class *class)
{
	أرجع sched_class_above(rq->ايندڙ_class, class);
}

ساكن مضمن bool sched_stop_runnable(بنية rq *rq)
{
	أرجع rq->stop && task_on_rq_queued(rq->stop);
}

ساكن مضمن bool sched_dl_runnable(بنية rq *rq)
{
	أرجع rq->dl.dl_nr_running > 0;
}

ساكن مضمن bool sched_rt_runnable(بنية rq *rq)
{
	أرجع rq->rt.rt_queued > 0;
}

ساكن مضمن bool sched_fair_runnable(بنية rq *rq)
{
	أرجع rq->cfs.nr_queued > 0;
}

خارجي بنية task_struct *pick_task_fair(بنية rq *rq, بنية rq_flags *rf);
خارجي بنية task_struct *pick_task_idle(بنية rq *rq, بنية rq_flags *rf);

#وصف_ڪريو SCA_CHECK		0x01
#وصف_ڪريو SCA_MIGRATE_DISABLE	0x02
#وصف_ڪريو SCA_MIGRATE_ENABLE	0x04
#وصف_ڪريو SCA_USER		0x08

خارجي فراغ update_گروهه_صلاحيت(بنية sched_domain *sd, صحيح cpu);

خارجي فراغ sched_balance_trigger(بنية rq *rq);

خارجي صحيح __set_cpus_allowed_ptr(بنية task_struct *p, بنية affinity_context *ctx);
خارجي فراغ سيٽ_cpus_allowed_common(بنية task_struct *p, بنية affinity_context *ctx);

ساكن مضمن bool task_allowed_on_cpu(بنية task_struct *p, صحيح cpu)
{
	/* When not in the task's cpumask, no point in looking further. */
	إذا (!cpumask_test_cpu(cpu, p->cpus_ptr))
		أرجع false;

	/* Can @cpu run a user thread? */
	إذا (!(p->flags & PF_KTHREAD) && !task_cpu_possible(cpu, p))
		أرجع false;

	أرجع true;
}

ساكن مضمن cpumask_t *alloc_user_cpus_ptr(صحيح ڳنڍ)
{
	/*
	 * See set_cpus_allowed_force() above for the rcu_head usage.
	 */
	صحيح سائيز = max_t(صحيح, cpumask_سائيز(), حجم(بنية rcu_سر));

	أرجع kmalloc_ڳنڍ(سائيز, GFP_KERNEL, ڳنڍ);
}

ساكن مضمن بنية task_struct *get_ڌڪو_task(بنية rq *rq)
{
	بنية task_struct *p = rq->donor;

	lockdep_assert_rq_held(rq);

	إذا (rq->ڌڪو_busy)
		أرجع عدم;

	إذا (p->nr_cpus_allowed == 1)
		أرجع عدم;

	إذا (p->migration_disabled)
		أرجع عدم;

	rq->ڌڪو_busy = true;
	أرجع get_task_struct(p);
}

خارجي صحيح ڌڪو_cpu_stop(فراغ *arg);

#جيڪڏهن_وصف_ٿيل CONFIG_CPU_IDLE

ساكن مضمن فراغ idle_سيٽ_state(بنية rq *rq,
				  بنية cpuidle_state *idle_state)
{
	rq->idle_state = idle_state;
}

ساكن مضمن بنية cpuidle_state *idle_get_state(بنية rq *rq)
{
	lockdep_assert(rcu_read_تالو_any_held());

	أرجع rq->idle_state;
}

#وإلا /* !CONFIG_CPU_IDLE: */

ساكن مضمن فراغ idle_سيٽ_state(بنية rq *rq,
				  بنية cpuidle_state *idle_state)
{
}

ساكن مضمن بنية cpuidle_state *idle_get_state(بنية rq *rq)
{
	أرجع عدم;
}

#جيڪڏهن_پورو /* !CONFIG_CPU_IDLE */

خارجي فراغ وقت_جدول_idle(فراغ);
asmlinkage فراغ وقت_جدول_user(فراغ);

خارجي فراغ sysrq_sched_debug_show(فراغ);
خارجي فراغ sched_init_granularity(فراغ);
خارجي فراغ update_max_interval(فراغ);

خارجي فراغ init_sched_dl_class(فراغ);
خارجي فراغ init_sched_rt_class(فراغ);
خارجي فراغ init_sched_fair_class(فراغ);

خارجي فراغ resched_curr(بنية rq *rq);
خارجي فراغ resched_curr_lazy(بنية rq *rq);
خارجي فراغ resched_cpu(صحيح cpu);

خارجي فراغ init_rt_bandwidth(بنية rt_bandwidth *rt_b, u64 period, u64 runtime);
خارجي bool sched_rt_bandwidth_account(بنية rt_rq *rt_rq);

خارجي فراغ init_dl_entity(بنية sched_dl_entity *dl_se);

خارجي فراغ init_cfs_throttle_work(بنية task_struct *p);

#وصف_ڪريو BW_سرڪايو		20
#وصف_ڪريو BW_UNIT			(1 << BW_سرڪايو)
#وصف_ڪريو تناسب_سرڪايو		8
#وصف_ڪريو MAX_BW_بٽ		(64 - BW_سرڪايو)
#وصف_ڪريو MAX_BW			((1ULL << MAX_BW_بٽ) - 1)

خارجي u64 منزل_تناسب(u64 period, u64 runtime);

خارجي فراغ init_entity_runnable_اوسط(بنية sched_entity *se);
خارجي فراغ post_init_entity_util_avg(بنية task_struct *p);

#جيڪڏهن_وصف_ٿيل CONFIG_NO_HZ_ڀريل
خارجي bool sched_can_stop_tick(بنية rq *rq);
خارجي صحيح __init sched_tick_offload_init(فراغ);

/*
 * Tick may be needed by tasks in the runqueue depending on their policy and
 * requirements. If tick is needed, lets send the target an IPI to kick it out of
 * nohz mode if necessary.
 */
ساكن مضمن فراغ sched_update_tick_dependency(بنية rq *rq)
{
	صحيح cpu = cpu_جو(rq);

	إذا (!tick_nohz_ڀريل_cpu(cpu))
		أرجع;

	إذا (sched_can_stop_tick(rq))
		tick_nohz_dep_clear_cpu(cpu, TICK_DEP_ٻٽو_انگ_SCHED);
	وإلا
		tick_nohz_dep_سيٽ_cpu(cpu, TICK_DEP_ٻٽو_انگ_SCHED);
}
#وإلا /* !CONFIG_NO_HZ_FULL: */
ساكن مضمن صحيح sched_tick_offload_init(فراغ) { أرجع 0; }
ساكن مضمن فراغ sched_update_tick_dependency(بنية rq *rq) { }
#جيڪڏهن_پورو /* !CONFIG_NO_HZ_FULL */

ساكن مضمن فراغ جوڙيو_nr_running(بنية rq *rq, بنانشان ڳڻپ)
{
	بنانشان prev_nr = rq->nr_running;

	rq->nr_running = prev_nr + ڳڻپ;
	إذا (trace_sched_update_nr_running_tp_enabled()) {
		call_trace_sched_update_nr_running(rq, ڳڻپ);
	}

	إذا (prev_nr < 2 && rq->nr_running >= 2)
		سيٽ_rd_overloaded(rq->rd, 1);

	sched_update_tick_dependency(rq);
}

ساكن مضمن فراغ sub_nr_running(بنية rq *rq, بنانشان ڳڻپ)
{
	rq->nr_running -= ڳڻپ;
	إذا (trace_sched_update_nr_running_tp_enabled()) {
		call_trace_sched_update_nr_running(rq, -ڳڻپ);
	}

	/* Check if we still need preemption */
	sched_update_tick_dependency(rq);
}

ساكن مضمن فراغ __block_task(بنية rq *rq, بنية task_struct *p)
{
	إذا (p->sched_contributes_منزل_ڀريو)
		rq->nr_uninterruptible++;

	إذا (p->اندريون_iowait) {
		atomic_inc(&rq->nr_iowait);
		delayacct_blkio_شروعات();
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

خارجي فراغ activate_task(بنية rq *rq, بنية task_struct *p, صحيح flags);
خارجي فراغ deactivate_task(بنية rq *rq, بنية task_struct *p, صحيح flags);

خارجي فراغ wakeup_preempt(بنية rq *rq, بنية task_struct *p, صحيح flags);

/*
 * attach_task() -- attach the task detached by detach_task() to its new rq.
 */
ساكن مضمن فراغ attach_task(بنية rq *rq, بنية task_struct *p)
{
	lockdep_assert_rq_held(rq);

	WARN_ON_ONCE(task_rq(p) != rq);
	activate_task(rq, p, قطار_اندر_وجهو_NOCLOCK);
	wakeup_preempt(rq, p, 0);
}

/*
 * attach_one_task() -- attaches the task returned from detach_one_task() to
 * its new rq.
 */
ساكن مضمن فراغ attach_one_task(بنية rq *rq, بنية task_struct *p)
{
	guard(rq_تالو)(rq);
	update_rq_clock(rq);
	attach_task(rq, p);
}

/*
 * __attach_tasks() - attaches a list of tasks (using se.group_node) to
 * the new rq
 */
ساكن مضمن فراغ __attach_tasks(بنية rq *rq, بنية list_سر *tasks)
{
	guard(rq_تالو)(rq);
	update_rq_clock(rq);

	طالما (!list_خالي_جايون(tasks)) {
		بنية task_struct *p;

		p = list_پهريون_داخلا(tasks, بنية task_struct, se.گروهه_ڳنڍ);
		list_del_init(&p->se.گروهه_ڳنڍ);

		attach_task(rq, p);
	}
}

#جيڪڏهن_وصف_ٿيل CONFIG_PREEMPT_RT
# define SCHED_NR_MIGRATE_BREAK 8
#وإلا
# define SCHED_NR_MIGRATE_BREAK 32
#جيڪڏهن_پورو

خارجي __read_mostly بنانشان صحيح sysctl_sched_nr_migrate;
خارجي __read_mostly بنانشان صحيح sysctl_sched_migration_لاڳت;

خارجي بنانشان صحيح sysctl_sched_بنياد_ٽڪرو;

خارجي صحيح sysctl_resched_latency_warn_ms;
خارجي صحيح sysctl_resched_latency_warn_once;

خارجي بنانشان صحيح sysctl_sched_tunable_scaling;

خارجي بنانشان صحيح sysctl_numa_balancing_جاچ_delay;
خارجي بنانشان صحيح sysctl_numa_balancing_جاچ_period_min;
خارجي بنانشان صحيح sysctl_numa_balancing_جاچ_period_max;
خارجي بنانشان صحيح sysctl_numa_balancing_جاچ_سائيز;
خارجي بنانشان صحيح sysctl_numa_balancing_hot_حد;

#جيڪڏهن_وصف_ٿيل CONFIG_SCHED_HRTICK

/*
 * Use hrtick when:
 *  - enabled by features
 *  - hrtimer is actually high res
 */
ساكن مضمن bool hrtick_enabled(بنية rq *rq)
{
	أرجع cpu_سرگرم(cpu_جو(rq)) && hrtimer_highres_enabled();
}

ساكن مضمن bool hrtick_enabled_fair(بنية rq *rq)
{
	أرجع sched_feat(HRTICK) && hrtick_enabled(rq);
}

ساكن مضمن bool hrtick_enabled_dl(بنية rq *rq)
{
	أرجع sched_feat(HRTICK_DL) && hrtick_enabled(rq);
}

خارجي فراغ hrtick_شروعات(بنية rq *rq, u64 delay);
ساكن مضمن bool hrtick_سرگرم(بنية rq *rq)
{
	أرجع hrtimer_سرگرم(&rq->hrtick_timer);
}

#وإلا /* !CONFIG_SCHED_HRTICK: */
ساكن مضمن bool hrtick_enabled_fair(بنية rq *rq) { أرجع false; }
ساكن مضمن bool hrtick_enabled_dl(بنية rq *rq) { أرجع false; }
ساكن مضمن bool hrtick_enabled(بنية rq *rq) { أرجع false; }
#جيڪڏهن_پورو /* !CONFIG_SCHED_HRTICK */

#جيڪڏهن_وصف_نه_ٿيل arch_scale_freq_tick
ساكن __always_inline فراغ arch_scale_freq_tick(فراغ) { }
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل arch_scale_freq_صلاحيت
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
ساكن __always_inline
بنانشان ڊگهو arch_scale_freq_صلاحيت(صحيح cpu)
{
	أرجع SCHED_صلاحيت_SCALE;
}
#جيڪڏهن_پورو

/*
 * In double_lock_balance()/double_rq_lock(), we use raw_spin_rq_lock() to
 * acquire rq lock instead of rq_lock(). So at the end of these two functions
 * we need to call double_rq_clock_clear_update() to clear RQCF_UPDATED of
 * rq->clock_update_flags to avoid the WARN_DOUBLE_CLOCK warning.
 */
ساكن مضمن فراغ double_rq_clock_clear_update(بنية rq *rq1, بنية rq *rq2)
{
	rq1->clock_update_flags &= (RQCF_REQ_SKIP|RQCF_ACT_SKIP);
	rq2->clock_update_flags &= (RQCF_REQ_SKIP|RQCF_ACT_SKIP);
}

#وصف_ڪريو DEFINE_تالو_GUARD_2(name, type, تالو_2, _unlock, ...)				\
__DEFINE_UNLOCK_GUARD(name, type, _unlock, type *lock2; __VA_ARGS__)			\
ساكن مضمن class_##name##_t class_##name##_constructor(type *تالو, type *lock2)	\
	__no_context_analysis								\
{ class_##name##_t _t = { .تالو = تالو, .lock2 = lock2 }, *_T = &_t;			\
  تالو_2; أرجع _t; }
#وصف_ڪريو DECLARE_تالو_GUARD_2_ATTRS(_name, تالو_2, _unlock1, _unlock2)			\
ساكن مضمن class_##_name##_t class_##_name##_constructor(تالو_3##_name##_t *_T1,	\
							    تالو_3##_name##_t *_T2) تالو_2; \
ساكن __always_inline فراغ __class_##_name##_cleanup_ctx1(class_##_name##_t **_T1)	\
	__no_context_analysis _unlock1 { }						\
ساكن __always_inline فراغ __class_##_name##_cleanup_ctx2(class_##_name##_t **_T2)	\
	__no_context_analysis _unlock2 { }
#وصف_ڪريو WITH_تالو_GUARD_2_ATTRS(_name, _T1, _T2)					\
	class_##_name##_constructor(_T1, _T2),						\
	*__UNIQUE_ID(unlock1) __cleanup(__class_##_name##_cleanup_ctx1) = (فراغ *)(_T1),\
	*__UNIQUE_ID(unlock2) __cleanup(__class_##_name##_cleanup_ctx2) = (فراغ *)(_T2)

ساكن مضمن bool rq_ترتيب_less(بنية rq *rq1, بنية rq *rq2)
{
#جيڪڏهن_وصف_ٿيل CONFIG_SCHED_CORE
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
	إذا (rq1->core->cpu < rq2->core->cpu)
		أرجع true;
	إذا (rq1->core->cpu > rq2->core->cpu)
		أرجع false;

	/*
	 * __sched_core_flip() relies on SMT having cpu-id lock order.
	 */
#جيڪڏهن_پورو /* CONFIG_SCHED_CORE */
	أرجع rq1->cpu < rq2->cpu;
}

خارجي فراغ double_rq_تالو(بنية rq *rq1, بنية rq *rq2)
	__acquires(__rq_lockp(rq1), __rq_lockp(rq2));

#جيڪڏهن_وصف_ٿيل CONFIG_PREEMPTION

/*
 * fair double_lock_balance: Safely acquires both rq->locks in a fair
 * way at the expense of forcing extra atomic operations in all
 * invocations.  This assures that the double_lock is acquired using the
 * same underlying policy as the spinlock_t on this architecture, which
 * reduces latency compared to the unfair variant below.  However, it
 * also adds more overhead and therefore may reduce throughput.
 */
ساكن مضمن صحيح double_تالو_balance_2(بنية rq *this_rq, بنية rq *busiest)
	__must_hold(__rq_lockp(this_rq))
	__acquires(__rq_lockp(busiest))
{
	raw_spin_rq_unlock(this_rq);
	double_rq_تالو(this_rq, busiest);

	أرجع 1;
}

#وإلا /* !CONFIG_PREEMPTION: */
/*
 * Unfair double_lock_balance: Optimizes throughput at the expense of
 * latency by eliminating extra atomic operations when the locks are
 * already in proper order on entry.  This favors lower CPU-ids and will
 * grant the double lock to lower CPUs over higher ids under contention,
 * regardless of entry order into the function.
 */
ساكن مضمن صحيح double_تالو_balance_2(بنية rq *this_rq, بنية rq *busiest)
	__must_hold(__rq_lockp(this_rq))
	__acquires(__rq_lockp(busiest))
{
	إذا (__rq_lockp(this_rq) == __rq_lockp(busiest)) {
		__acquire(__rq_lockp(busiest)); /* already held */
		double_rq_clock_clear_update(this_rq, busiest);
		أرجع 0;
	}

	إذا (likely(raw_spin_rq_trylock(busiest))) {
		double_rq_clock_clear_update(this_rq, busiest);
		أرجع 0;
	}

	إذا (rq_ترتيب_less(this_rq, busiest)) {
		raw_spin_rq_تالو_nested(busiest, SINGLE_اونهائي_NESTING);
		double_rq_clock_clear_update(this_rq, busiest);
		أرجع 0;
	}

	raw_spin_rq_unlock(this_rq);
	double_rq_تالو(this_rq, busiest);

	أرجع 1;
}

#جيڪڏهن_پورو /* !CONFIG_PREEMPTION */

/*
 * double_lock_balance - lock the busiest runqueue, this_rq is locked already.
 */
ساكن مضمن صحيح double_تالو_balance(بنية rq *this_rq, بنية rq *busiest)
	__must_hold(__rq_lockp(this_rq))
	__acquires(__rq_lockp(busiest))
{
	lockdep_assert_irqs_disabled();

	أرجع double_تالو_balance_2(this_rq, busiest);
}

ساكن مضمن فراغ double_unlock_balance(بنية rq *this_rq, بنية rq *busiest)
	__releases(__rq_lockp(busiest))
{
	إذا (__rq_lockp(this_rq) != __rq_lockp(busiest))
		raw_spin_rq_unlock(busiest);
	وإلا
		__release(__rq_lockp(busiest)); /* fake release */
	تالو_سيٽ_subclass(&__rq_lockp(this_rq)->dep_map, 0, _RET_IP_);
}

ساكن مضمن فراغ double_تالو(spinlock_t *l1, spinlock_t *l2)
	__acquires(l1, l2)
{
	إذا (l1 > l2)
		مٽا_سٽا(l1, l2);

	spin_تالو(l1);
	spin_تالو_nested(l2, SINGLE_اونهائي_NESTING);
}

ساكن مضمن فراغ double_تالو_irq(spinlock_t *l1, spinlock_t *l2)
	__acquires(l1, l2)
{
	إذا (l1 > l2)
		مٽا_سٽا(l1, l2);

	spin_تالو_irq(l1);
	spin_تالو_nested(l2, SINGLE_اونهائي_NESTING);
}

ساكن مضمن فراغ double_raw_تالو(raw_spinlock_t *l1, raw_spinlock_t *l2)
	__acquires(l1, l2)
{
	إذا (l1 > l2)
		مٽا_سٽا(l1, l2);

	raw_spin_تالو(l1);
	raw_spin_تالو_nested(l2, SINGLE_اونهائي_NESTING);
}

ساكن مضمن فراغ double_raw_unlock(raw_spinlock_t *l1, raw_spinlock_t *l2)
	__releases(l1, l2)
{
	raw_spin_unlock(l1);
	raw_spin_unlock(l2);
}

DEFINE_تالو_GUARD_2(double_raw_spinlock, raw_spinlock_t,
		    double_raw_تالو(_T->تالو, _T->lock2),
		    double_raw_unlock(_T->تالو, _T->lock2))

DECLARE_تالو_GUARD_2_ATTRS(double_raw_spinlock,
			   __acquires(_T1, _T2),
			   __releases(*(raw_spinlock_t **)_T1),
			   __releases(*(raw_spinlock_t **)_T2));
#وصف_ڪريو class_double_raw_spinlock_constructor(_T1, _T2) \
	WITH_تالو_GUARD_2_ATTRS(double_raw_spinlock, _T1, _T2)

/*
 * double_rq_unlock - safely unlock two runqueues
 *
 * Note this does not restore interrupts like task_rq_unlock,
 * you need to do so manually after calling.
 */
ساكن مضمن فراغ double_rq_unlock(بنية rq *rq1, بنية rq *rq2)
	__releases(__rq_lockp(rq1), __rq_lockp(rq2))
{
	إذا (__rq_lockp(rq1) != __rq_lockp(rq2))
		raw_spin_rq_unlock(rq2);
	وإلا
		__release(__rq_lockp(rq2)); /* fake release */
	raw_spin_rq_unlock(rq1);
}

خارجي فراغ سيٽ_rq_online (بنية rq *rq);
خارجي فراغ سيٽ_rq_offline(بنية rq *rq);

خارجي bool sched_smp_initialized;

DEFINE_تالو_GUARD_2(double_rq_تالو, بنية rq,
		    double_rq_تالو(_T->تالو, _T->lock2),
		    double_rq_unlock(_T->تالو, _T->lock2))

خارجي بنية sched_entity *__pick_root_entity(بنية cfs_rq *cfs_rq);
خارجي بنية sched_entity *__pick_first_entity(بنية cfs_rq *cfs_rq);
خارجي بنية sched_entity *__pick_last_entity(بنية cfs_rq *cfs_rq);

خارجي bool sched_debug_verbose;

خارجي فراغ print_cfs_stats(بنية seq_file *m, صحيح cpu);
خارجي فراغ print_rt_stats(بنية seq_file *m, صحيح cpu);
خارجي فراغ print_dl_stats(بنية seq_file *m, صحيح cpu);
خارجي فراغ print_cfs_rq(بنية seq_file *m, صحيح cpu, بنية cfs_rq *cfs_rq);
خارجي فراغ print_rt_rq(بنية seq_file *m, صحيح cpu, بنية rt_rq *rt_rq);
خارجي فراغ print_dl_rq(بنية seq_file *m, صحيح cpu, بنية dl_rq *dl_rq);

خارجي فراغ resched_latency_warn(صحيح cpu, u64 latency);

#جيڪڏهن_وصف_ٿيل CONFIG_NUMA_BALANCING
خارجي فراغ show_numa_stats(بنية task_struct *p, بنية seq_file *m);
خارجي فراغ
print_numa_stats(بنية seq_file *m, صحيح ڳنڍ, بنانشان ڊگهو tsf,
		 بنانشان ڊگهو tpf, بنانشان ڊگهو gsf, بنانشان ڊگهو gpf);
#جيڪڏهن_پورو /* CONFIG_NUMA_BALANCING */

خارجي فراغ init_cfs_rq(بنية cfs_rq *cfs_rq);
خارجي فراغ init_rt_rq(بنية rt_rq *rt_rq);
خارجي فراغ init_dl_rq(بنية dl_rq *dl_rq);

خارجي فراغ cfs_bandwidth_usage_inc(فراغ);
خارجي فراغ cfs_bandwidth_usage_dec(فراغ);

#جيڪڏهن_وصف_ٿيل CONFIG_NO_HZ_COMMON

#وصف_ڪريو NOHZ_BALANCE_KICK_ٻٽو_انگ	0
#وصف_ڪريو NOHZ_STATS_KICK_ٻٽو_انگ	1
#وصف_ڪريو NOHZ_NEWILB_KICK_ٻٽو_انگ	2
#وصف_ڪريو NOHZ_ايندڙ_KICK_ٻٽو_انگ	3

/* Run sched_balance_domains() */
#وصف_ڪريو NOHZ_BALANCE_KICK	ٻٽو_انگ(NOHZ_BALANCE_KICK_ٻٽو_انگ)
/* Update blocked load */
#وصف_ڪريو NOHZ_STATS_KICK		ٻٽو_انگ(NOHZ_STATS_KICK_ٻٽو_انگ)
/* Update blocked load when entering idle */
#وصف_ڪريو NOHZ_NEWILB_KICK	ٻٽو_انگ(NOHZ_NEWILB_KICK_ٻٽو_انگ)
/* Update nohz.next_balance */
#وصف_ڪريو NOHZ_ايندڙ_KICK		ٻٽو_انگ(NOHZ_ايندڙ_KICK_ٻٽو_انگ)

#وصف_ڪريو NOHZ_KICK_نقاب		(NOHZ_BALANCE_KICK | NOHZ_STATS_KICK | NOHZ_ايندڙ_KICK)

#وصف_ڪريو nohz_flags(cpu)		(&cpu_rq(cpu)->nohz_flags)

خارجي فراغ nohz_balance_exit_idle(بنية rq *rq);
#وإلا /* !CONFIG_NO_HZ_COMMON: */
ساكن مضمن فراغ nohz_balance_exit_idle(بنية rq *rq) { }
#جيڪڏهن_پورو /* !CONFIG_NO_HZ_COMMON */

#جيڪڏهن_وصف_ٿيل CONFIG_NO_HZ_COMMON
خارجي فراغ nohz_لاڳيتو_idle_balance(صحيح cpu);
#وإلا
ساكن مضمن فراغ nohz_لاڳيتو_idle_balance(صحيح cpu) { }
#جيڪڏهن_پورو

#ضمن "stats.h"

#إذا defined(CONFIG_SCHED_CORE) && defined(CONFIG_SCHEDSTATS)

خارجي فراغ __sched_core_account_forceidle(بنية rq *rq);

ساكن مضمن فراغ sched_core_account_forceidle(بنية rq *rq)
{
	إذا (schedstat_enabled())
		__sched_core_account_forceidle(rq);
}

خارجي فراغ __sched_core_tick(بنية rq *rq);

ساكن مضمن فراغ sched_core_tick(بنية rq *rq)
{
	إذا (sched_core_enabled(rq) && schedstat_enabled())
		__sched_core_tick(rq);
}

#وإلا /* !(CONFIG_SCHED_CORE && CONFIG_SCHEDSTATS): */

ساكن مضمن فراغ sched_core_account_forceidle(بنية rq *rq) { }

ساكن مضمن فراغ sched_core_tick(بنية rq *rq) { }

#جيڪڏهن_پورو /* !(CONFIG_SCHED_CORE && CONFIG_SCHEDSTATS) */

#جيڪڏهن_وصف_ٿيل CONFIG_IRQ_وقت_ACCOUNTING

بنية irqtime {
	u64			ڪل;
	u64			tick_delta;
	u64			irq_شروعات_وقت;
	بنية u64_stats_sync	sync;
};

DECLARE_في_CPU(بنية irqtime, cpu_irqtime);
DECLARE_STATIC_ڪنجي_FALSE(sched_clock_irqtime);

ساكن مضمن صحيح irqtime_enabled(فراغ)
{
	أرجع static_branch_likely(&sched_clock_irqtime);
}

/*
 * Returns the irqtime minus the softirq time computed by ksoftirqd.
 * Otherwise ksoftirqd's sum_exec_runtime is subtracted its own runtime
 * and never move forward.
 */
ساكن مضمن u64 irq_وقت_read(صحيح cpu)
{
	بنية irqtime *irqtime = &في_cpu(cpu_irqtime, cpu);
	بنانشان صحيح seq;
	u64 ڪل;

	افعل {
		seq = __u64_stats_fetch_begin(&irqtime->sync);
		ڪل = irqtime->ڪل;
	} طالما (__u64_stats_fetch_retry(&irqtime->sync, seq));

	أرجع ڪل;
}

#وإلا /* !CONFIG_IRQ_TIME_ACCOUNTING: */

ساكن مضمن صحيح irqtime_enabled(فراغ)
{
	أرجع 0;
}

#جيڪڏهن_پورو /* !CONFIG_IRQ_TIME_ACCOUNTING */

#جيڪڏهن_وصف_ٿيل CONFIG_CPU_FREQ

DECLARE_في_CPU(بنية update_util_معلومات __rcu *, cpufreq_update_util_معلومات);

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
ساكن مضمن فراغ cpufreq_update_util(بنية rq *rq, بنانشان صحيح flags)
{
	بنية update_util_معلومات *معلومات;

	معلومات = rcu_dereference_sched(*في_cpu_ptr(&cpufreq_update_util_معلومات,
						  cpu_جو(rq)));
	إذا (معلومات)
		معلومات->func(معلومات, rq_clock(rq), flags);
}
#وإلا /* !CONFIG_CPU_FREQ: */
ساكن مضمن فراغ cpufreq_update_util(بنية rq *rq, بنانشان صحيح flags) { }
#جيڪڏهن_پورو /* !CONFIG_CPU_FREQ */

#جيڪڏهن_وصف_ٿيل arch_scale_freq_صلاحيت
# ifndef arch_scale_freq_invariant
#  define arch_scale_freq_invariant()	true
# endif
#وإلا
# define arch_scale_freq_invariant()	false
#جيڪڏهن_پورو

بنانشان ڊگهو effective_cpu_util(صحيح cpu, بنانشان ڊگهو util_cfs,
				 بنانشان ڊگهو *min,
				 بنانشان ڊگهو *max);

بنانشان ڊگهو sugov_effective_cpu_perf(صحيح cpu, بنانشان ڊگهو actual,
				 بنانشان ڊگهو min,
				 بنانشان ڊگهو max);


/*
 * Verify the fitness of task @p to run on @cpu taking into account the
 * CPU original capacity and the runtime/deadline ratio of the task.
 *
 * The function will return true if the original capacity of @cpu is
 * greater than or equal to task's deadline density right shifted by
 * (BW_SHIFT - SCHED_CAPACITY_SHIFT) and false otherwise.
 */
ساكن مضمن bool dl_task_سمائي_صلاحيت(بنية task_struct *p, صحيح cpu)
{
	بنانشان ڊگهو cap = arch_scale_cpu_صلاحيت(cpu);

	أرجع cap >= p->dl.dl_density >> (BW_سرڪايو - SCHED_صلاحيت_سرڪايو);
}

ساكن مضمن بنانشان ڊگهو cpu_bw_dl(بنية rq *rq)
{
	أرجع (rq->dl.running_bw * SCHED_صلاحيت_SCALE) >> BW_سرڪايو;
}

ساكن مضمن بنانشان ڊگهو cpu_util_dl(بنية rq *rq)
{
	أرجع READ_ONCE(rq->avg_dl.util_avg);
}


خارجي بنانشان ڊگهو cpu_util_cfs(صحيح cpu);
خارجي بنانشان ڊگهو cpu_util_cfs_boost(صحيح cpu);

ساكن مضمن بنانشان ڊگهو cpu_util_rt(بنية rq *rq)
{
	أرجع READ_ONCE(rq->avg_rt.util_avg);
}

#جيڪڏهن_وصف_ٿيل CONFIG_UCLAMP_TASK

بنانشان ڊگهو uclamp_eff_قدر(بنية task_struct *p, تعداد uclamp_id clamp_id);

/*
 * When uclamp is compiled in, the aggregation at rq level is 'turned off'
 * by default in the fast path and only gets turned on once userspace performs
 * an operation that requires it.
 *
 * Returns true if userspace opted-in to use uclamp and aggregation at rq level
 * hence is active.
 */
ساكن مضمن bool uclamp_is_استعمال_ٿيل(فراغ)
{
	أرجع static_branch_likely(&sched_uclamp_استعمال_ٿيل);
}

/*
 * Enabling static branches would get the cpus_read_lock(),
 * check whether uclamp_is_used before enable it to avoid always
 * calling cpus_read_lock(). Because we never disable this
 * static key once enable it.
 */
ساكن مضمن فراغ sched_uclamp_enable(فراغ)
{
	إذا (!uclamp_is_استعمال_ٿيل())
		static_branch_enable(&sched_uclamp_استعمال_ٿيل);
}

ساكن مضمن بنانشان ڊگهو uclamp_rq_get(بنية rq *rq,
					  تعداد uclamp_id clamp_id)
{
	أرجع READ_ONCE(rq->uclamp[clamp_id].قدر);
}

ساكن مضمن فراغ uclamp_rq_سيٽ(بنية rq *rq, تعداد uclamp_id clamp_id,
				 بنانشان صحيح قدر)
{
	WRITE_ONCE(rq->uclamp[clamp_id].قدر, قدر);
}

ساكن مضمن bool uclamp_rq_is_idle(بنية rq *rq)
{
	أرجع rq->uclamp_flags & UCLAMP_FLAG_IDLE;
}

/* Is the rq being capped/throttled by uclamp_max? */
ساكن مضمن bool uclamp_rq_is_capped(بنية rq *rq)
{
	بنانشان ڊگهو rq_util;
	بنانشان ڊگهو max_util;

	إذا (!uclamp_is_استعمال_ٿيل())
		أرجع false;

	rq_util = cpu_util_cfs(cpu_جو(rq)) + cpu_util_rt(rq);
	max_util = READ_ONCE(rq->uclamp[UCLAMP_MAX].قدر);

	أرجع max_util != SCHED_صلاحيت_SCALE && rq_util >= max_util;
}

#وصف_ڪريو for_each_clamp_id(clamp_id) \
	لكل ((clamp_id) = 0; (clamp_id) < UCLAMP_CNT; (clamp_id)++)

خارجي بنانشان صحيح sysctl_sched_uclamp_util_min_rt_default;


ساكن مضمن بنانشان صحيح uclamp_none(تعداد uclamp_id clamp_id)
{
	إذا (clamp_id == UCLAMP_MIN)
		أرجع 0;
	أرجع SCHED_صلاحيت_SCALE;
}

/* Integer rounded range for each bucket */
#وصف_ڪريو UCLAMP_ٻالٽي_DELTA DIV_ڦيرو_CLOSEST(SCHED_صلاحيت_SCALE, UCLAMP_BUCKETS)

ساكن مضمن بنانشان صحيح uclamp_ٻالٽي_id(بنانشان صحيح clamp_قدر)
{
	أرجع min_t(بنانشان صحيح, clamp_قدر / UCLAMP_ٻالٽي_DELTA, UCLAMP_BUCKETS - 1);
}

ساكن مضمن فراغ
uclamp_se_سيٽ(بنية uclamp_se *uc_se, بنانشان صحيح قدر, bool user_defined)
{
	uc_se->قدر = قدر;
	uc_se->ٻالٽي_id = uclamp_ٻالٽي_id(قدر);
	uc_se->user_defined = user_defined;
}

#وإلا /* !CONFIG_UCLAMP_TASK: */

ساكن مضمن بنانشان ڊگهو
uclamp_eff_قدر(بنية task_struct *p, تعداد uclamp_id clamp_id)
{
	إذا (clamp_id == UCLAMP_MIN)
		أرجع 0;

	أرجع SCHED_صلاحيت_SCALE;
}

ساكن مضمن bool uclamp_rq_is_capped(بنية rq *rq) { أرجع false; }

ساكن مضمن bool uclamp_is_استعمال_ٿيل(فراغ)
{
	أرجع false;
}

ساكن مضمن فراغ sched_uclamp_enable(فراغ) {}

ساكن مضمن بنانشان ڊگهو
uclamp_rq_get(بنية rq *rq, تعداد uclamp_id clamp_id)
{
	إذا (clamp_id == UCLAMP_MIN)
		أرجع 0;

	أرجع SCHED_صلاحيت_SCALE;
}

ساكن مضمن فراغ
uclamp_rq_سيٽ(بنية rq *rq, تعداد uclamp_id clamp_id, بنانشان صحيح قدر)
{
}

ساكن مضمن bool uclamp_rq_is_idle(بنية rq *rq)
{
	أرجع false;
}

#جيڪڏهن_پورو /* !CONFIG_UCLAMP_TASK */

#جيڪڏهن_وصف_ٿيل CONFIG_HAVE_SCHED_AVG_IRQ

ساكن مضمن بنانشان ڊگهو cpu_util_irq(بنية rq *rq)
{
	أرجع READ_ONCE(rq->avg_irq.util_avg);
}

ساكن مضمن
بنانشان ڊگهو scale_irq_صلاحيت(بنانشان ڊگهو util, بنانشان ڊگهو irq, بنانشان ڊگهو max)
{
	util *= (max - irq);
	util /= max;

	أرجع util;

}

#وإلا /* !CONFIG_HAVE_SCHED_AVG_IRQ: */

ساكن مضمن بنانشان ڊگهو cpu_util_irq(بنية rq *rq)
{
	أرجع 0;
}

ساكن مضمن
بنانشان ڊگهو scale_irq_صلاحيت(بنانشان ڊگهو util, بنانشان ڊگهو irq, بنانشان ڊگهو max)
{
	أرجع util;
}

#جيڪڏهن_پورو /* !CONFIG_HAVE_SCHED_AVG_IRQ */

خارجي فراغ __setparam_fair(بنية task_struct *p, ثابت بنية sched_attr *attr);

#إذا defined(CONFIG_ENERGY_MODEL) && defined(CONFIG_CPU_FREQ_GOV_SCHEDUTIL)

#وصف_ڪريو perf_domain_وسعت(pd) (منزل_cpumask(((pd)->em_pd->cpus)))

DECLARE_STATIC_ڪنجي_FALSE(sched_energy_present);

ساكن مضمن bool sched_energy_enabled(فراغ)
{
	أرجع static_branch_unlikely(&sched_energy_present);
}

#وإلا /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL): */

#وصف_ڪريو perf_domain_وسعت(pd) عدم

ساكن مضمن bool sched_energy_enabled(فراغ) { أرجع false; }

#جيڪڏهن_پورو /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL) */

#جيڪڏهن_وصف_ٿيل CONFIG_MEMBARRIER

/*
 * The scheduler provides memory barriers required by membarrier between:
 * - prior user-space memory accesses and store to rq->membarrier_state,
 * - store to rq->membarrier_state and following user-space memory accesses.
 * In the same way it provides those guarantees around store to rq->curr.
 */
ساكن مضمن فراغ membarrier_switch_mm(بنية rq *rq,
					بنية mm_struct *prev_mm,
					بنية mm_struct *ايندڙ_mm)
{
	صحيح membarrier_state;

	إذا (prev_mm == ايندڙ_mm)
		أرجع;

	membarrier_state = atomic_read(&ايندڙ_mm->membarrier_state);
	إذا (READ_ONCE(rq->membarrier_state) == membarrier_state)
		أرجع;

	WRITE_ONCE(rq->membarrier_state, membarrier_state);
}

#وإلا /* !CONFIG_MEMBARRIER: */

ساكن مضمن فراغ membarrier_switch_mm(بنية rq *rq,
					بنية mm_struct *prev_mm,
					بنية mm_struct *ايندڙ_mm)
{
}

#جيڪڏهن_پورو /* !CONFIG_MEMBARRIER */

ساكن مضمن bool is_في_cpu_kthread(بنية task_struct *p)
{
	إذا (!(p->flags & PF_KTHREAD))
		أرجع false;

	إذا (p->nr_cpus_allowed != 1)
		أرجع false;

	أرجع true;
}

خارجي فراغ swake_مٿي_all_locked(بنية swait_قطار_سر *q);
خارجي فراغ __prepare_to_swait(بنية swait_قطار_سر *q, بنية swait_قطار *wait);

خارجي صحيح try_منزل_wake_مٿي(بنية task_struct *tsk, بنانشان صحيح state, صحيح wake_flags);

#جيڪڏهن_وصف_ٿيل CONFIG_PREEMPT_DYNAMIC
خارجي صحيح preempt_dynamic_گهڻائي;
خارجي صحيح sched_dynamic_گهڻائي(ثابت حرف *str);
خارجي فراغ sched_dynamic_update(صحيح گهڻائي);
#جيڪڏهن_پورو
خارجي ثابت حرف *preempt_modes[];

#جيڪڏهن_وصف_ٿيل CONFIG_SCHED_MM_CID

ساكن __always_inline bool cid_on_cpu(بنانشان صحيح cid)
{
	أرجع cid & MM_CID_ONCPU;
}

ساكن __always_inline bool cid_اندريون_transit(بنانشان صحيح cid)
{
	أرجع cid & MM_CID_TRANSIT;
}

ساكن __always_inline بنانشان صحيح cpu_cid_منزل_cid(بنانشان صحيح cid)
{
	أرجع cid & ~MM_CID_ONCPU;
}

ساكن __always_inline بنانشان صحيح cid_منزل_cpu_cid(بنانشان صحيح cid)
{
	أرجع cid | MM_CID_ONCPU;
}

ساكن __always_inline بنانشان صحيح cid_منزل_transit_cid(بنانشان صحيح cid)
{
	أرجع cid | MM_CID_TRANSIT;
}

ساكن __always_inline بنانشان صحيح cid_from_transit_cid(بنانشان صحيح cid)
{
	أرجع cid & ~MM_CID_TRANSIT;
}

ساكن __always_inline bool cid_on_task(بنانشان صحيح cid)
{
	/* True if none of the MM_CID_ONCPU, MM_CID_TRANSIT, MM_CID_UNSET bits is set */
	أرجع cid < MM_CID_TRANSIT;
}

ساكن __always_inline فراغ mm_drop_cid(بنية mm_struct *mm, بنانشان صحيح cid)
{
	clear_ٻٽو_انگ(cid, mm_cidmask(mm));
}

ساكن __always_inline فراغ mm_unset_cid_on_task(بنية task_struct *t)
{
	بنانشان صحيح cid = t->mm_cid.cid;

	t->mm_cid.cid = MM_CID_UNSET;
	إذا (cid_on_task(cid))
		mm_drop_cid(t->mm, cid);
}

ساكن __always_inline فراغ mm_drop_cid_on_cpu(بنية mm_struct *mm, بنية mm_cid_pcpu *pcp)
{
	/* Clear the ONCPU bit, but do not set UNSET in the per CPU storage */
	إذا (cid_on_cpu(pcp->cid)) {
		pcp->cid = cpu_cid_منزل_cid(pcp->cid);
		mm_drop_cid(mm, pcp->cid);
	}
}

ساكن مضمن بنانشان صحيح __mm_get_cid(بنية mm_struct *mm, بنانشان صحيح max_cids)
{
	بنانشان صحيح cid = ڳوليو_پهريون_zero_ٻٽو_انگ(mm_cidmask(mm), max_cids);

	إذا (cid >= max_cids)
		أرجع MM_CID_UNSET;
	إذا (test_and_سيٽ_ٻٽو_انگ(cid, mm_cidmask(mm)))
		أرجع MM_CID_UNSET;
	أرجع cid;
}

ساكن مضمن بنانشان صحيح mm_get_cid(بنية mm_struct *mm)
{
	بنانشان صحيح cid = __mm_get_cid(mm, READ_ONCE(mm->mm_cid.max_cids));

	طالما (cid == MM_CID_UNSET) {
		cpu_relax();
		cid = __mm_get_cid(mm, انگ_possible_cpus());
	}
	أرجع cid;
}

ساكن مضمن بنانشان صحيح mm_cid_converge(بنية mm_struct *mm, بنانشان صحيح orig_cid,
					   بنانشان صحيح max_cids)
{
	بنانشان صحيح new_cid, cid = cpu_cid_منزل_cid(orig_cid);

	/* Is it in the optimal CID space? */
	إذا (likely(cid < max_cids))
		أرجع orig_cid;

	/* Try to find one in the optimal space. Otherwise keep the provided. */
	new_cid = __mm_get_cid(mm, max_cids);
	إذا (new_cid != MM_CID_UNSET) {
		mm_drop_cid(mm, cid);
		/* Preserve the ONCPU mode of the original CID */
		أرجع new_cid | (orig_cid & MM_CID_ONCPU);
	}
	أرجع orig_cid;
}

ساكن __always_inline فراغ mm_cid_update_task_cid(بنية task_struct *t, بنانشان صحيح cid)
{
	إذا (t->mm_cid.cid != cid) {
		t->mm_cid.cid = cid;
		rseq_sched_سيٽ_ids_بدليو(t);
	}
}

ساكن __always_inline فراغ mm_cid_update_pcpu_cid(بنية mm_struct *mm, بنانشان صحيح cid)
{
	__this_cpu_write(mm->mm_cid.pcpu->cid, cid);
}

ساكن __always_inline فراغ mm_cid_from_cpu(بنية task_struct *t, بنانشان صحيح cpu_cid,
					    بنانشان صحيح گهڻائي)
{
	بنانشان صحيح max_cids, tcid = t->mm_cid.cid;
	بنية mm_struct *mm = t->mm;

	max_cids = READ_ONCE(mm->mm_cid.max_cids);
	/* Optimize for the common case where both have the ONCPU bit set */
	إذا (likely(cid_on_cpu(cpu_cid & tcid))) {
		إذا (likely(cpu_cid_منزل_cid(cpu_cid) < max_cids)) {
			mm_cid_update_task_cid(t, cpu_cid);
			أرجع;
		}
		/* Try to converge into the optimal CID space */
		cpu_cid = mm_cid_converge(mm, cpu_cid, max_cids);
	} وإلا {
		/* Hand over or drop the task owned CID */
		إذا (cid_on_task(tcid)) {
			إذا (cid_on_cpu(cpu_cid))
				mm_unset_cid_on_task(t);
			وإلا
				cpu_cid = cid_منزل_cpu_cid(tcid);
		}
		/* Still nothing, allocate a new one */
		إذا (!cid_on_cpu(cpu_cid))
			cpu_cid = cid_منزل_cpu_cid(mm_get_cid(mm));

		/* Handle the transition mode flag if required */
		إذا (گهڻائي & MM_CID_TRANSIT)
			cpu_cid = cpu_cid_منزل_cid(cpu_cid) | MM_CID_TRANSIT;
	}
	mm_cid_update_pcpu_cid(mm, cpu_cid);
	mm_cid_update_task_cid(t, cpu_cid);
}

ساكن __always_inline فراغ mm_cid_from_task(بنية task_struct *t, بنانشان صحيح cpu_cid,
					     بنانشان صحيح گهڻائي)
{
	بنانشان صحيح max_cids, tcid = t->mm_cid.cid;
	بنية mm_struct *mm = t->mm;

	max_cids = READ_ONCE(mm->mm_cid.max_cids);
	/* Optimize for the common case, where both have the ONCPU bit clear */
	إذا (likely(cid_on_task(tcid | cpu_cid))) {
		إذا (likely(tcid < max_cids)) {
			mm_cid_update_pcpu_cid(mm, tcid);
			أرجع;
		}
		/* Try to converge into the optimal CID space */
		tcid = mm_cid_converge(mm, tcid, max_cids);
	} وإلا {
		/* Hand over or drop the CPU owned CID */
		إذا (cid_on_cpu(cpu_cid)) {
			إذا (cid_on_task(tcid))
				mm_drop_cid_on_cpu(mm, this_cpu_ptr(mm->mm_cid.pcpu));
			وإلا
				tcid = cpu_cid_منزل_cid(cpu_cid);
		}
		/* Still nothing, allocate a new one */
		إذا (!cid_on_task(tcid))
			tcid = mm_get_cid(mm);
		/* Set the transition mode flag if required */
		tcid |= گهڻائي & MM_CID_TRANSIT;
	}
	mm_cid_update_pcpu_cid(mm, tcid);
	mm_cid_update_task_cid(t, tcid);
}

ساكن __always_inline فراغ mm_cid_schedin(بنية task_struct *ايندڙ)
{
	بنية mm_struct *mm = ايندڙ->mm;
	بنانشان صحيح cpu_cid, گهڻائي;

	إذا (!ايندڙ->mm_cid.سرگرم)
		أرجع;

	cpu_cid = __this_cpu_read(mm->mm_cid.pcpu->cid);
	گهڻائي = READ_ONCE(mm->mm_cid.گهڻائي);
	إذا (likely(!cid_on_cpu(گهڻائي)))
		mm_cid_from_task(ايندڙ, cpu_cid, گهڻائي);
	وإلا
		mm_cid_from_cpu(ايندڙ, cpu_cid, گهڻائي);
}

ساكن __always_inline فراغ mm_cid_schedout(بنية task_struct *prev)
{
	بنية mm_struct *mm = prev->mm;
	بنانشان صحيح گهڻائي, cid;

	/* During mode transitions CIDs are temporary and need to be dropped */
	إذا (likely(!cid_اندريون_transit(prev->mm_cid.cid)))
		أرجع;

	گهڻائي = READ_ONCE(mm->mm_cid.گهڻائي);
	cid = cid_from_transit_cid(prev->mm_cid.cid);

	/*
	 * If transition mode is done, transfer ownership when the CID is
	 * within the convergence range to optimize the next schedule in.
	 */
	إذا (!cid_اندريون_transit(گهڻائي) && cid < READ_ONCE(mm->mm_cid.max_cids)) {
		إذا (cid_on_cpu(گهڻائي))
			cid = cid_منزل_cpu_cid(cid);

		/* Update both so that the next schedule in goes into the fast path */
		mm_cid_update_pcpu_cid(mm, cid);
		prev->mm_cid.cid = cid;
	} وإلا {
		mm_drop_cid(mm, cid);
		prev->mm_cid.cid = MM_CID_UNSET;
	}
}

ساكن مضمن فراغ mm_cid_switch_منزل(بنية task_struct *prev, بنية task_struct *ايندڙ)
{
	mm_cid_schedout(prev);
	mm_cid_schedin(ايندڙ);
}

#وإلا /* !CONFIG_SCHED_MM_CID: */
ساكن مضمن فراغ mm_cid_switch_منزل(بنية task_struct *prev, بنية task_struct *ايندڙ) { }
#جيڪڏهن_پورو /* !CONFIG_SCHED_MM_CID */

#جيڪڏهن_وصف_ٿيل CONFIG_SCHED_CACHE
DECLARE_STATIC_ڪنجي_FALSE(sched_cache_present);
DECLARE_STATIC_ڪنجي_FALSE(sched_cache_سرگرم);
خارجي صحيح sysctl_sched_cache_user;
خارجي بنانشان صحيح llc_aggr_tolerance;
خارجي بنانشان صحيح llc_epoch_period;
خارجي بنانشان صحيح llc_epoch_affinity_timeout;
خارجي بنانشان صحيح llc_imb_pct;
خارجي بنانشان صحيح llc_overaggr_pct;

ساكن مضمن bool sched_cache_enabled(فراغ)
{
	أرجع static_branch_unlikely(&sched_cache_سرگرم);
}

خارجي فراغ sched_cache_سرگرم_سيٽ(فراغ);

#جيڪڏهن_پورو

فراغ sched_domains_free_llc_id(صحيح cpu);

خارجي فراغ init_sched_mm(بنية task_struct *p);

خارجي u64 avg_vruntime(بنية cfs_rq *cfs_rq);
خارجي صحيح entity_eligible(بنية cfs_rq *cfs_rq, بنية sched_entity *se);
ساكن مضمن
فراغ move_queued_task_locked(بنية rq *src_rq, بنية rq *dst_rq, بنية task_struct *task)
{
	lockdep_assert_rq_held(src_rq);
	lockdep_assert_rq_held(dst_rq);

	deactivate_task(src_rq, task, 0);
	سيٽ_task_cpu(task, dst_rq->cpu);
	activate_task(dst_rq, task, 0);
	wakeup_preempt(dst_rq, task, 0);
}

ساكن مضمن
bool task_is_pushable(بنية rq *rq, بنية task_struct *p, صحيح cpu)
{
	إذا (!task_on_cpu(rq, p) &&
	    cpumask_test_cpu(cpu, &p->cpus_نقاب))
		أرجع true;

	أرجع false;
}

#جيڪڏهن_وصف_ٿيل CONFIG_RT_MUTEXES

ساكن مضمن صحيح __rt_effective_prio(بنية task_struct *pi_task, صحيح prio)
{
	إذا (pi_task)
		prio = min(prio, pi_task->prio);

	أرجع prio;
}

ساكن مضمن صحيح rt_effective_prio(بنية task_struct *p, صحيح prio)
{
	بنية task_struct *pi_task = rt_mutex_get_چوٽي_task(p);

	أرجع __rt_effective_prio(pi_task, prio);
}

#وإلا /* !CONFIG_RT_MUTEXES: */

ساكن مضمن صحيح rt_effective_prio(بنية task_struct *p, صحيح prio)
{
	أرجع prio;
}

#جيڪڏهن_پورو /* !CONFIG_RT_MUTEXES */

خارجي صحيح __sched_setscheduler(بنية task_struct *p, ثابت بنية sched_attr *attr, bool user, bool pi);
خارجي صحيح __sched_setaffinity(بنية task_struct *p, بنية affinity_context *ctx);
خارجي ثابت بنية sched_class *__setscheduler_class(صحيح policy, صحيح prio);
خارجي فراغ سيٽ_ڀريو_وزن(بنية task_struct *p, bool update_ڀريو);
خارجي فراغ قطار_اندر_وجهو_task(بنية rq *rq, بنية task_struct *p, صحيح flags);
خارجي bool قطار_مان_ڪڍو_task(بنية rq *rq, بنية task_struct *p, صحيح flags);

خارجي بنية balance_callback *splice_balance_callbacks(بنية rq *rq);

خارجي فراغ __balance_callbacks(بنية rq *rq, بنية rq_flags *rf);
خارجي فراغ balance_callbacks(بنية rq *rq, بنية balance_callback *سر);

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
بنية sched_change_ctx {
	u64			prio;
	بنية task_struct	*p;
	ثابت بنية sched_class *class;
	صحيح			flags;
	bool			queued;
	bool			running;
};

بنية sched_change_ctx *sched_change_begin(بنية task_struct *p, بنانشان صحيح flags);
فراغ sched_change_پڇاڙي(بنية sched_change_ctx *ctx);

DEFINE_CLASS(sched_change, بنية sched_change_ctx *,
	     sched_change_پڇاڙي(_T),
	     sched_change_begin(p, flags),
	     بنية task_struct *p, بنانشان صحيح flags)

DEFINE_CLASS_IS_UNCONDITIONAL(sched_change)

#ضمن "ext/ext.h"

#جيڪڏهن_وصف_ٿيل CONFIG_PREFERRED_CPU
فراغ sched_ڌڪو_current_non_preferred_cpu(بنية rq *rq);
#وإلا	/* !CONFIG_PREFERRED_CPU */
ساكن مضمن فراغ sched_ڌڪو_current_non_preferred_cpu(بنية rq *rq) { }
#جيڪڏهن_پورو

#جيڪڏهن_پورو /* _KERNEL_SCHED_SCHED_H */
