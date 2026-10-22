/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Scheduler internal types and methods:
 */
#که_نه_تعریف_شوی _KERNEL_SCHED_SCHED_H
#تعریف_کړه _KERNEL_SCHED_SCHED_H

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
بنية sched_ډله;
بنية cpuidle_state;

#إذا defined(CONFIG_PARAVIRT) && !defined(CONFIG_HAVE_PV_STEAL_CLOCK_GEN)
# include <asm/paravirt.h>
#د_که_پای

#ضمن <asm/barrier.h>

#ضمن "cpupri.h"
#ضمن "cpudeadline.h"

/* task_struct::on_rq states: */
#تعریف_کړه TASK_ON_RQ_QUEUED	1
#تعریف_کړه TASK_ON_RQ_MIGRATING	2

خارجي __read_mostly صحيح scheduler_running;

خارجي بینښه اوږد calc_بارول_update;
خارجي atomic_long_t calc_بارول_tasks;

خارجي فراغ calc_global_بارول_tick(بنية rq *this_rq);
خارجي اوږد calc_بارول_fold_فعال(بنية rq *this_rq, اوږد adjust);

خارجي فراغ call_trace_sched_update_nr_running(بنية rq *rq, صحيح شمېر);

خارجي صحيح sysctl_sched_rt_period;
خارجي صحيح sysctl_sched_rt_runtime;
خارجي صحيح sched_rr_timeslice;

/*
 * Asymmetric CPU capacity bits
 */
بنية asym_cap_معلومات {
	بنية list_سر link;
	بنية rcu_سر rcu;
	بینښه اوږد ظرفیت;
	بینښه اوږد cpus[];
};

خارجي بنية list_سر asym_cap_list;

#تعریف_کړه cpu_ظرفیت_پراختیا(asym_معلومات) منزل_cpumask((asym_معلومات)->cpus)

/*
 * Helpers for converting nanosecond timing to jiffy resolution
 */
#تعریف_کړه NS_منزل_JIFFIES(وخت)	((بینښه اوږد)(وخت) / (NSEC_فی_SEC/HZ))

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
#که_تعریف_شوی CONFIG_64_بټ
# define NICE_0_بارول_ښويدنه	(SCHED_FIXEDPOINT_ښويدنه + SCHED_FIXEDPOINT_ښويدنه)
# define scale_بارول(w)		((w) << SCHED_FIXEDPOINT_ښويدنه)
# define scale_بارول_ښکته(w)					\
({								\
	بینښه اوږد __w = (w);				\
								\
	إذا (__w)						\
		__w = max(2UL, __w >> SCHED_FIXEDPOINT_ښويدنه);	\
	__w;							\
})
#وإلا
# define NICE_0_بارول_ښويدنه	(SCHED_FIXEDPOINT_ښويدنه)
# define scale_بارول(w)		(w)
# define scale_بارول_ښکته(w)	(w)
#د_که_پای

/*
 * Task weight (visible to users) and its load (invisible to users) have
 * independent resolution, but they should be well calibrated. We use
 * scale_load() and scale_load_down(w) to convert between them. The
 * following must be true:
 *
 *  scale_load(sched_prio_to_weight[NICE_TO_PRIO(0)-MAX_RT_PRIO]) == NICE_0_LOAD
 *
 */
#تعریف_کړه NICE_0_بارول		(1L << NICE_0_بارول_ښويدنه)

/*
 * Single value that decides SCHED_DEADLINE internal math precision.
 * 10 -> just above 1us
 * 9  -> just above 0.5us
 */
#تعریف_کړه DL_SCALE		10

/*
 * Single value that denotes runtime == period, ie unlimited time.
 */
#تعریف_کړه RUNTIME_INF		((u64)~0ULL)

ساكن مضمن صحيح idle_policy(صحيح policy)
{
	أرجع policy == SCHED_IDLE;
}

ساكن مضمن صحيح نورمال_policy(صحيح policy)
{
#که_تعریف_شوی CONFIG_SCHED_CLASS_EXT
	إذا (policy == SCHED_EXT)
		أرجع true;
#د_که_پای
	أرجع policy == SCHED_نورمال;
}

ساكن مضمن صحيح fair_policy(صحيح policy)
{
	أرجع نورمال_policy(policy) || policy == SCHED_BATCH;
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

#تعریف_کړه cap_scale(v, s)		((v)*(s) >> SCHED_ظرفیت_ښويدنه)

ساكن مضمن فراغ update_avg(u64 *avg, u64 sample)
{
	s64 diff = sample - *avg;

	*avg += diff / 8;
}

/*
 * Shifting a value by an exponent greater *or equal* to the size of said value
 * is UB; cap at size-1.
 */
#تعریف_کړه shr_bound(val, ښويدنه)							\
	(val >> min_t(typeof(ښويدنه), ښويدنه, بټونه_فی_TYPE(typeof(val)) - 1))

/*
 * cgroup weight knobs should use the common MIN, DFL and MAX values which are
 * 1, 100 and 10000 respectively. While it loses a bit of range on both ends, it
 * maps pretty well onto the shares value used by scheduler and the round-trip
 * conversions preserve the original value over the entire range.
 */
ساكن مضمن بینښه اوږد sched_وزن_from_cgroup(بینښه اوږد cgrp_وزن)
{
	أرجع DIV_پړاو_CLOSEST_ULL(cgrp_وزن * 1024, CGROUP_وزن_DFL);
}

ساكن مضمن بینښه اوږد sched_وزن_منزل_cgroup(بینښه اوږد وزن)
{
	أرجع clamp_t(بینښه اوږد,
		       DIV_پړاو_CLOSEST_ULL(وزن * CGROUP_وزن_DFL, 1024),
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
#تعریف_کړه SCHED_FLAG_SUGOV	0x10000000

#تعریف_کړه SCHED_DL_FLAGS		(SCHED_FLAG_RECLAIM | SCHED_FLAG_DL_OVERRUN | SCHED_FLAG_SUGOV)

ساكن مضمن bool dl_entity_is_special(ثابت بنية sched_dl_entity *dl_se)
{
#که_تعریف_شوی CONFIG_CPU_FREQ_GOV_SCHEDUTIL
	أرجع unlikely(dl_se->flags & SCHED_FLAG_SUGOV);
#وإلا
	أرجع false;
#د_که_پای
}

/*
 * Tells if entity @a should preempt entity @b.
 */
ساكن مضمن bool dl_entity_preempt(ثابت بنية sched_dl_entity *a,
				     ثابت بنية sched_dl_entity *b)
{
	أرجع dl_entity_is_special(a) ||
	       dl_وخت_before(a->deadline, b->deadline);
}

/*
 * This is the priority-queue data structure of the RT scheduling class:
 */
بنية rt_prio_array {
	DECLARE_BITMAP(bitmap, MAX_RT_PRIO+1); /* include 1 bit for delimiter */
	بنية list_سر کتار[MAX_RT_PRIO];
};

بنية rt_bandwidth {
	/* nests inside the rq lock: */
	raw_spinlock_t		rt_runtime_قلف;
	ktime_t			rt_period;
	u64			rt_runtime;
	بنية hrtimer		rt_period_timer;
	بینښه صحيح		rt_period_فعال;
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
	raw_spinlock_t		قلف;
	u64			bw;
	u64			ټول_bw;
};

خارجي فراغ init_dl_bw(بنية dl_bw *dl_b);
خارجي صحيح  sched_dl_global_validate(فراغ);
خارجي فراغ sched_dl_do_global(فراغ);
خارجي صحيح  sched_dl_ډکېدنه(بنية task_struct *p, صحيح policy, ثابت بنية sched_attr *attr);
خارجي فراغ __setparam_dl(بنية task_struct *p, ثابت بنية sched_attr *attr);
خارجي فراغ __getparam_dl(بنية task_struct *p, بنية sched_attr *attr, بینښه صحيح flags);
خارجي bool __checkparam_dl(ثابت بنية sched_attr *attr);
خارجي bool dl_param_بدل_شوی(بنية task_struct *p, ثابت بنية sched_attr *attr);
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
خارجي فراغ dl_server_پیل(بنية sched_dl_entity *dl_se);
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
خارجي صحيح dl_server_بدلون_bw(بنية sched_dl_entity *detach_se,
			     بنية sched_dl_entity *attach_se);

ساكن مضمن bool dl_server_فعال(بنية sched_dl_entity *dl_se)
{
	أرجع dl_se->dl_server_فعال;
}

#که_تعریف_شوی CONFIG_CGROUP_SCHED

خارجي بنية list_سر task_groups;

#که_تعریف_شوی CONFIG_ډله_SCHED_BANDWIDTH
خارجي ثابت u64 max_bw_quota_period_us;

/*
 * default period for group bandwidth.
 * default: 0.1s, units: microseconds
 */
ساكن مضمن u64 default_bw_period_us(فراغ)
{
	أرجع 100000ULL;
}
#د_که_پای /* CONFIG_GROUP_SCHED_BANDWIDTH */

بنية cfs_bandwidth {
#که_تعریف_شوی CONFIG_CFS_BANDWIDTH
	raw_spinlock_t		قلف;
	ktime_t			period;
	u64			quota;
	u64			runtime;
	u64			موده;
	u64			runtime_snap;
	s64			پوړیز_quota;

	u8			idle;
	u8			period_فعال;
	u8			slack_started;
	بنية hrtimer		period_timer;
	بنية hrtimer		slack_timer;
	بنية list_سر	throttled_cfs_rq;

	/* Statistics: */
	صحيح			nr_periods;
	صحيح			nr_throttled;
	صحيح			nr_موده;
	u64			throttled_وخت;
	u64			موده_وخت;
#د_که_پای /* CONFIG_CFS_BANDWIDTH */
};

/* Task group related information */
بنية task_ډله {
	بنية cgroup_subsys_state css;

#که_تعریف_شوی CONFIG_ډله_SCHED_وزن
	/* A positive value indicates that this is a SCHED_IDLE group. */
	صحيح			idle;
#د_که_پای

#که_تعریف_شوی CONFIG_FAIR_ډله_SCHED
	/* runqueue "owned" by this group on each CPU */
	بنية cfs_rq __percpu	*cfs_rq;
	بینښه اوږد		shares;
	/*
	 * load_avg can be heavily contended at clock tick time, so put
	 * it in its own cache-line separated from the fields above which
	 * will also be accessed at each tick.
	 */
	atomic_long_t		بارول_avg ____cacheline_aligned;
	atomic_long_t		runnable_avg;

#د_که_پای /* CONFIG_FAIR_GROUP_SCHED */

#که_تعریف_شوی CONFIG_RT_ډله_SCHED
	بنية sched_rt_entity	**rt_se;
	بنية rt_rq		**rt_rq;

	بنية rt_bandwidth	rt_bandwidth;
#د_که_پای

	بنية scx_task_ډله	scx;

	بنية rcu_سر		rcu;
	بنية list_سر	list;

	بنية task_ډله	*پلار;
	بنية list_سر	siblings;
	بنية list_سر	children;

#که_تعریف_شوی CONFIG_SCHED_AUTOGROUP
	بنية autogroup	*autogroup;
#د_که_پای

	بنية cfs_bandwidth	cfs_bandwidth;

#که_تعریف_شوی CONFIG_UCLAMP_TASK_ډله
	/* The two decimal precision [%] value requested from user-space */
	بینښه صحيح		uclamp_pct[UCLAMP_CNT];
	/* Clamp values requested for a task group */
	بنية uclamp_se	uclamp_req[UCLAMP_CNT];
	/* Effective clamp values used for a task group */
	بنية uclamp_se	uclamp[UCLAMP_CNT];
#د_که_پای

};

#تعریف_کړه ریښه_TASK_ډله_بارول	NICE_0_بارول

تعريفنوع صحيح (*tg_visitor)(بنية task_ډله *, فراغ *);

خارجي صحيح walk_tg_ونه_from(بنية task_ډله *from,
			     tg_visitor ښکته, tg_visitor پورته, فراغ *معلومات);

/*
 * Iterate the full tree, calling @down when first entering a node and @up when
 * leaving it for the final time.
 *
 * Caller must hold rcu_lock or sufficient equivalent.
 */
ساكن مضمن صحيح walk_tg_ونه(tg_visitor ښکته, tg_visitor پورته, فراغ *معلومات)
{
	أرجع walk_tg_ونه_from(&ریښه_task_ډله, ښکته, پورته, معلومات);
}

ساكن مضمن بنية task_ډله *css_tg(بنية cgroup_subsys_state *css)
{
	أرجع css ? container_د(css, بنية task_ډله, css) : عدم;
}

خارجي صحيح tg_nop(بنية task_ډله *tg, فراغ *معلومات);

#که_تعریف_شوی CONFIG_FAIR_ډله_SCHED
خارجي فراغ free_fair_sched_ډله(بنية task_ډله *tg);
خارجي صحيح alloc_fair_sched_ډله(بنية task_ډله *tg, بنية task_ډله *پلار);
خارجي فراغ online_fair_sched_ډله(بنية task_ډله *tg);
خارجي فراغ unregister_fair_sched_ډله(بنية task_ډله *tg);
خارجي فراغ __sched_cgroup_mode_update(صحيح ډېرپېښ);
#وإلا /* !CONFIG_FAIR_GROUP_SCHED: */
ساكن مضمن فراغ free_fair_sched_ډله(بنية task_ډله *tg) { }
ساكن مضمن صحيح alloc_fair_sched_ډله(بنية task_ډله *tg, بنية task_ډله *پلار)
{
       أرجع 1;
}
ساكن مضمن فراغ online_fair_sched_ډله(بنية task_ډله *tg) { }
ساكن مضمن فراغ unregister_fair_sched_ډله(بنية task_ډله *tg) { }
#د_که_پای /* !CONFIG_FAIR_GROUP_SCHED */

خارجي فراغ init_tg_cfs_ثبت(بنية task_ډله *tg, بنية cfs_rq *cfs_rq,
			بنية sched_entity *se, صحيح cpu,
			بنية sched_entity *پلار);
خارجي فراغ init_cfs_bandwidth(بنية cfs_bandwidth *cfs_b, بنية cfs_bandwidth *پلار);

خارجي فراغ __refill_cfs_bandwidth_runtime(بنية cfs_bandwidth *cfs_b);
خارجي فراغ پیل_cfs_bandwidth(بنية cfs_bandwidth *cfs_b);
خارجي فراغ unthrottle_cfs_rq(بنية cfs_rq *cfs_rq);
خارجي bool cfs_task_bw_constrained(بنية task_struct *p);

خارجي فراغ init_tg_rt_ثبت(بنية task_ډله *tg, بنية rt_rq *rt_rq,
		بنية sched_rt_entity *rt_se, صحيح cpu,
		بنية sched_rt_entity *پلار);
خارجي صحيح sched_ډله_سېټ_rt_runtime(بنية task_ډله *tg, اوږد rt_runtime_us);
خارجي صحيح sched_ډله_سېټ_rt_period(بنية task_ډله *tg, u64 rt_period_us);
خارجي اوږد sched_ډله_rt_runtime(بنية task_ډله *tg);
خارجي اوږد sched_ډله_rt_period(بنية task_ډله *tg);
خارجي صحيح sched_rt_can_attach(بنية task_ډله *tg, بنية task_struct *tsk);

خارجي بنية task_ډله *sched_create_ډله(بنية task_ډله *پلار);
خارجي فراغ sched_online_ډله(بنية task_ډله *tg,
			       بنية task_ډله *پلار);
خارجي فراغ sched_destroy_ډله(بنية task_ډله *tg);
خارجي فراغ sched_release_ډله(بنية task_ډله *tg);

خارجي فراغ sched_move_task(بنية task_struct *tsk, bool for_autogroup);

#که_تعریف_شوی CONFIG_FAIR_ډله_SCHED
خارجي صحيح sched_ډله_سېټ_shares(بنية task_ډله *tg, بینښه اوږد shares);

خارجي صحيح sched_ډله_سېټ_idle(بنية task_ډله *tg, اوږد idle);

خارجي فراغ سېټ_task_rq_fair(بنية sched_entity *se,
			     بنية cfs_rq *prev, بنية cfs_rq *بل);
#وإلا /* !CONFIG_FAIR_GROUP_SCHED: */
ساكن مضمن صحيح sched_ډله_سېټ_shares(بنية task_ډله *tg, بینښه اوږد shares) { أرجع 0; }
ساكن مضمن صحيح sched_ډله_سېټ_idle(بنية task_ډله *tg, اوږد idle) { أرجع 0; }
#د_که_پای /* !CONFIG_FAIR_GROUP_SCHED */

#وإلا /* !CONFIG_CGROUP_SCHED: */

بنية cfs_bandwidth { };

ساكن مضمن bool cfs_task_bw_constrained(بنية task_struct *p) { أرجع false; }

#د_که_پای /* !CONFIG_CGROUP_SCHED */

/*
 * A weight of 0 or 1 can cause arithmetics problems.
 * A weight of a cfs_rq is the sum of weights of which entities
 * are queued on this cfs_rq, so a weight of a entity should not be
 * too large, so as the shares value of a task group.
 * (The default weight is 1024 - so there's no practical
 *  limitation from this.)
 */
#تعریف_کړه MIN_SHARES		(1UL <<  1)
#تعریف_کړه MAX_SHARES		(1UL << 18)

خارجي فراغ unregister_rt_sched_ډله(بنية task_ډله *tg);
خارجي فراغ free_rt_sched_ډله(بنية task_ډله *tg);
خارجي صحيح alloc_rt_sched_ډله(بنية task_ډله *tg, بنية task_ډله *پلار);

/*
 * u64_u32_load/u64_u32_store
 *
 * Use a copy of a u64 value to protect against data race. This is only
 * applicable for 32-bits architectures.
 */
#که_تعریف_شوی CONFIG_64_بټ
# define u64_u32_بارول_copy(var, copy)		var
# define u64_u32_store_copy(var, copy, val)	(var = val)
#وإلا
# define u64_u32_بارول_copy(var, copy)					\
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
#د_که_پای
# define u64_u32_بارول(var)		u64_u32_بارول_copy(var, var##_copy)
# define u64_u32_store(var, val)	u64_u32_store_copy(var, var##_copy, val)

بنية balance_callback {
	بنية balance_callback *بل;
	فراغ (*func)(بنية rq *rq);
};

/* Fair scheduling SCHED_{NORMAL,BATCH,IDLE} related fields in a runqueue: */
بنية cfs_rq {
	بنية بارول_وزن	بارول;
	بینښه صحيح		nr_queued;
	بینښه صحيح		h_nr_queued;		/* SCHED_{NORMAL,BATCH,IDLE} */
	بینښه صحيح		h_nr_runnable;		/* SCHED_{NORMAL,BATCH,IDLE} */
	بینښه صحيح		h_nr_idle;		/* SCHED_IDLE */

	s64			مجموعه_w_vruntime;
	u64			مجموعه_وزن;
	u64			zero_vruntime;
	بینښه صحيح		مجموعه_ښويدنه;

#که_تعریف_شوی CONFIG_SCHED_CORE
	بینښه صحيح		forceidle_seq;
	u64			zero_vruntime_fi;
#د_که_پای

	بنية rb_ریښه_cached	tasks_timeline;

	/*
	 * 'curr' points to the currently running entity on this cfs_rq.
	 * It is set to NULL otherwise (i.e when none are currently running).
	 */
	بنية sched_entity	*curr;
	بنية sched_entity	*بل;

	/*
	 * CFS load tracking
	 */
	بنية sched_entity	*h_curr;
	بنية sched_avg	avg;
#که_نه_تعریف_شوی CONFIG_64_بټ
	u64			وروستی_update_وخت_copy;
#د_که_پای
	بنية {
		raw_spinlock_t	قلف ____cacheline_aligned;
		صحيح		nr;
		بینښه اوږد	بارول_avg;
		بینښه اوږد	util_avg;
		بینښه اوږد	runnable_avg;
	} removed;

#که_تعریف_شوی CONFIG_FAIR_ډله_SCHED
	u64			وروستی_update_tg_بارول_avg;
	بینښه اوږد		tg_بارول_avg_contrib;
	بینښه اوږد		tg_runnable_avg_contrib;
	اوږد			propagate;
	اوږد			prop_runnable_مجموعه;

	/*
	 *   h_load = weight * f(tg)
	 *
	 * Where f(tg) is the recursive weight fraction assigned to
	 * this group.
	 */
	بینښه اوږد		h_بارول;
	u64			وروستی_h_بارول_update;
	بنية sched_entity	*h_بارول_بل;

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
	بنية list_سر	ونې_پاڼه_cfs_rq_list;
	بنية task_ډله	*tg;	/* Group that "owns" this runqueue */

	/* Locally cached copy of our task_group's idle value */
	صحيح			idle;

# ifdef CONFIG_CFS_BANDWIDTH
	صحيح			runtime_enabled;
	s64			runtime_پاتې_شوی;

	u64			throttled_pelt_idle;
#  ifndef CONFIG_64_بټ
	u64                     throttled_pelt_idle_copy;
#  endif
	u64			throttled_clock;
	u64			throttled_clock_pelt;
	u64			throttled_clock_pelt_وخت;
	u64			throttled_clock_self;
	u64			throttled_clock_self_وخت;
	bool			throttled:1;
	bool			pelt_clock_throttled:1;
	صحيح			throttle_شمېر;
	بنية list_سر	throttled_list;
	بنية list_سر	throttled_csd_list;
	بنية list_سر        throttled_limbo_list;
# endif /* CONFIG_CFS_BANDWIDTH */
#د_که_پای /* CONFIG_FAIR_GROUP_SCHED */
};

#که_تعریف_شوی CONFIG_SCHED_CLASS_EXT
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
	SCX_RQ_ریښه_IDLE_RENOTIFY	= 1 << 8, /* the root is owed update_idle() */

	SCX_RQ_دننه_WAKEUP	= 1 << 16,
	SCX_RQ_دننه_DISPATCH	= 1 << 17,
};

/* per-rq rescue execution state, see scx_rescue_timerfn() */
بنية scx_rq_rescue {
	بنية scx_dispatch_q	dsq;			/* stranded tasks awaiting rescue */
	s64			budget;			/* execution token bucket, ns */
	u64			clock;			/* last budget accrual timestamp */
	بنية task_struct	*curr;			/* task being rescued, one at a time */
	s64			ټوټه;			/* curr's admitted slice */
	u64			exec_snap;		/* sum_exec_runtime at admission */
	بنية timer_list	timer;			/* paces admission and escalation */
	u64			kill_at;		/* last ejection, init before any */
};

بنية scx_rq {
	بنية scx_dispatch_q	local_dsq;
#که_تعریف_شوی CONFIG_EXT_SUB_SCHED
	بنية scx_dispatch_q	reject_dsq;		/* staging for cap-rejected tasks */
	بنية scx_rq_rescue	rescue;
#د_که_پای
	بنية list_سر	runnable_list;		/* runnable tasks on this rq */
	بنية list_سر	ddsp_deferred_locals;	/* deferred ddsps from enq */
	/* both stashed across the activate_task() in move_remote_task_to_local_dsq() */
	u64			remote_activate_enq_flags;
	بنية scx_sched	*remote_activate_sch;
	u32			nr_running;
	u32			cpuperf_موخه;		/* [0, SCHED_CAPACITY_SCALE] */
	bool			دننه_select_cpu;
	bool			cpu_released;
	u32			flags;
	u32			nr_immed;		/* ENQ_IMMED tasks on local_dsq */
#که_تعریف_شوی CONFIG_SCHED_CORE
	u32			قلف_drop_seq;	/* nr dispatch lock releases */
#د_که_پای
	u64			clock;			/* current per-rq clock -- see scx_bpf_now() */
#که_تعریف_شوی CONFIG_EXT_SUB_SCHED
	بنية llist_سر	ecaps_منزل_sync;		/* pending ecaps syncs */
	بنية task_struct	*sub_dispatch_prev;
#د_که_پای
	cpumask_var_t		cpus_منزل_sync;
	bool			kick_sync_pending;
	بینښه اوږد		kick_sync;

	بنية list_سر	sched_pcpus_منزل_kick;	/* see kick_cpus_irq_workfn() */

	raw_spinlock_t		deferred_reenq_قلف;
	بنية list_سر	deferred_reenq_locals;	/* scheds requesting reenq of local DSQ */
	بنية list_سر	deferred_reenq_users;	/* user DSQs requesting reenq */
	بنية balance_callback	deferred_bal_cb;
	بنية balance_callback	kick_sync_bal_cb;
	بنية irq_work		deferred_irq_work;
	بنية irq_work		kick_cpus_irq_work;
};
#د_که_پای /* CONFIG_SCHED_CLASS_EXT */

ساكن مضمن صحيح rt_bandwidth_enabled(فراغ)
{
	أرجع sysctl_sched_rt_runtime >= 0;
}

/* RT IPI pull logic requires IRQ_WORK */
#إذا defined(CONFIG_IRQ_WORK) && defined(CONFIG_SMP)
# define HAVE_RT_ټېل_IPI
#د_که_پای

/* Real-Time classes' related field in a runqueue: */
بنية rt_rq {
	بنية rt_prio_array	فعال;
	بینښه صحيح		rt_nr_running;
	بینښه صحيح		rr_nr_running;
	بنية {
		صحيح		curr; /* highest queued rt task prio */
		صحيح		بل; /* next highest */
	} highest_prio;
	bool			overloaded;
	بنية plist_سر	pushable_tasks;

	صحيح			rt_queued;

#که_تعریف_شوی CONFIG_RT_ډله_SCHED
	صحيح			rt_throttled;
	u64			rt_وخت; /* consumed RT time, goes up in update_curr_rt */
	u64			rt_runtime; /* allotted RT time, "slice" from rt_bandwidth, RT sharing/balancing */
	/* Nests inside the rq lock: */
	raw_spinlock_t		rt_runtime_قلف;

	بینښه صحيح		rt_nr_boosted;

	بنية rq		*rq; /* this is always top-level rq, cache? */
#د_که_پای
#که_تعریف_شوی CONFIG_CGROUP_SCHED
	بنية task_ډله	*tg; /* this tg has "this" rt_rq on given CPU for runnable entities */
#د_که_پای
};

ساكن مضمن bool rt_rq_is_runnable(بنية rt_rq *rt_rq)
{
	أرجع rt_rq->rt_queued && rt_rq->rt_nr_running;
}

/* Deadline class' related fields in a runqueue */
بنية dl_rq {
	/* runqueue is an rbtree, ordered by deadline */
	بنية rb_ریښه_cached	ریښه;

	بینښه صحيح		dl_nr_running;

	/*
	 * Deadline values of the currently executing and the
	 * earliest ready task on this rq. Caching these facilitates
	 * the decision whether or not a ready but not running task
	 * should migrate somewhere else.
	 */
	بنية {
		u64		curr;
		u64		بل;
	} earliest_dl;

	bool			overloaded;

	بنية sched_dl_entity	*curr;
	/*
	 * Tasks on this rq that can be pushed away. They are kept in
	 * an rb-tree, ordered by tasks' deadlines, with caching
	 * of the leftmost (earliest deadline) element.
	 */
	بنية rb_ریښه_cached	pushable_dl_tasks_ریښه;

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
	u64			bw_نسبت;
};

#که_تعریف_شوی CONFIG_FAIR_ډله_SCHED
/* Check whether a task group is root tg */
#تعریف_کړه is_ریښه_task_ډله(tg) ((tg) == &ریښه_task_ډله)
/* An entity is a task if it doesn't "own" a runqueue */
#تعریف_کړه entity_is_task(se)	(!se->my_q)

ساكن مضمن فراغ se_update_runnable(بنية sched_entity *se)
{
	إذا (!entity_is_task(se))
		se->runnable_وزن = se->my_q->h_nr_runnable;
}

ساكن مضمن اوږد se_runnable(بنية sched_entity *se)
{
	إذا (se->sched_delayed)
		أرجع false;

	إذا (entity_is_task(se))
		أرجع !!se->on_rq;
	وإلا
		أرجع se->runnable_وزن;
}

#وإلا /* !CONFIG_FAIR_GROUP_SCHED: */

#تعریف_کړه entity_is_task(se)	1

ساكن مضمن فراغ se_update_runnable(بنية sched_entity *se) { }

ساكن مضمن اوږد se_runnable(بنية sched_entity *se)
{
	إذا (se->sched_delayed)
		أرجع false;

	أرجع !!se->on_rq;
}

#د_که_پای /* !CONFIG_FAIR_GROUP_SCHED */

/*
 * XXX we want to get rid of these helpers and use the full load resolution.
 */
ساكن مضمن اوږد se_وزن(بنية sched_entity *se)
{
	أرجع scale_بارول_ښکته(se->بارول.وزن);
}


ساكن مضمن bool sched_asym_prefer(صحيح a, صحيح b)
{
	أرجع arch_asym_cpu_لومړیتوب(a) > arch_asym_cpu_لومړیتوب(b);
}

بنية perf_domain {
	بنية em_perf_domain *em_pd;
	بنية perf_domain *بل;
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
بنية ریښه_domain {
	atomic_t		refcount;
	atomic_t		rto_شمېر;
	بنية rcu_سر		rcu;
	cpumask_var_t		پراختیا;
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
	cpumask_var_t		dlo_ماسک;
	atomic_t		dlo_شمېر;
	بنية dl_bw		dl_bw;
	بنية cpudl		cpudl;

	/*
	 * Indicate whether a root_domain's dl_bw has been checked or
	 * updated. It's monotonously increasing value.
	 *
	 * Also, some corner cases, like 'wrap around' is dangerous, but given
	 * that u64 is 'big enough'. So that shouldn't be a concern.
	 */
	u64 لیدنه_cookie;

#که_تعریف_شوی HAVE_RT_ټېل_IPI
	/*
	 * For IPI pull requests, loop across the rto_mask.
	 */
	بنية irq_work		rto_ټېل_work;
	raw_spinlock_t		rto_قلف;
	/* These are only updated and read within rto_lock */
	صحيح			rto_loop;
	صحيح			rto_cpu;
	/* These atomics are updated outside of a lock */
	atomic_t		rto_loop_بل;
	atomic_t		rto_loop_پیل;
#د_که_پای /* HAVE_RT_PUSH_IPI */
	/*
	 * The "RT overload" flag: it gets set if a CPU has more than
	 * one runnable RT task.
	 */
	cpumask_var_t		rto_ماسک;
	بنية cpupri		cpupri;

	/*
	 * NULL-terminated list of performance domains intersecting with the
	 * CPUs of the rd. Protected by RCU.
	 */
	بنية perf_domain __rcu *pd;
};

خارجي فراغ init_defrootdomain(فراغ);
خارجي صحيح sched_init_domains(ثابت بنية cpumask *cpu_map);
خارجي فراغ rq_attach_ریښه(بنية rq *rq, بنية ریښه_domain *rd);
خارجي فراغ sched_get_rd(بنية ریښه_domain *rd);
خارجي فراغ sched_put_rd(بنية ریښه_domain *rd);

ساكن مضمن صحيح get_rd_overloaded(بنية ریښه_domain *rd)
{
	أرجع READ_ONCE(rd->overloaded);
}

ساكن مضمن فراغ سېټ_rd_overloaded(بنية ریښه_domain *rd, صحيح حالت)
{
	إذا (get_rd_overloaded(rd) != حالت)
		WRITE_ONCE(rd->overloaded, حالت);
}

#که_تعریف_شوی HAVE_RT_ټېل_IPI
خارجي فراغ rto_ټېل_irq_work_func(بنية irq_work *work);
#د_که_پای

#که_تعریف_شوی CONFIG_UCLAMP_TASK
/*
 * struct uclamp_bucket - Utilization clamp bucket
 * @value: utilization clamp value for tasks on this clamp bucket
 * @tasks: number of RUNNABLE tasks on this clamp bucket
 *
 * Keep track of how many tasks are RUNNABLE for a given utilization
 * clamp value.
 */
بنية uclamp_لوښی {
	بینښه اوږد ارزښت : بټونه_فی(SCHED_ظرفیت_SCALE);
	بینښه اوږد tasks : بټونه_فی_LONG - بټونه_فی(SCHED_ظرفیت_SCALE);
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
	بینښه صحيح ارزښت;
	بنية uclamp_لوښی لوښی[UCLAMP_BUCKETS];
};

DECLARE_STATIC_کلی_FALSE(sched_uclamp_کارول_شوی);
#د_که_پای /* CONFIG_UCLAMP_TASK */

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
	بینښه صحيح		nr_running;
#که_تعریف_شوی CONFIG_NUMA_BALANCING
	بینښه صحيح		nr_numa_running;
	بینښه صحيح		nr_preferred_running;
#د_که_پای
	بینښه صحيح		ttwu_pending;
	بینښه اوږد		cpu_ظرفیت;
#که_تعریف_شوی CONFIG_SCHED_PROXY_EXEC
	بنية task_struct __rcu	*donor;  /* Scheduling context */
	بنية task_struct __rcu	*curr;   /* Execution context */
#وإلا
	اتحاد {
		بنية task_struct __rcu *donor; /* Scheduler context */
		بنية task_struct __rcu *curr;  /* Execution context */
	};
#د_که_پای
	بنية task_struct	*idle;
	/* padding left here deliberately */

	/*
	 * The next cacheline holds the (hot) runqueue lock, as well as
	 * some other less performance-critical fields.
	 */
	u64			nr_switches	____cacheline_aligned;

	/* runqueue lock: */
	raw_spinlock_t		__lock;

#که_تعریف_شوی CONFIG_NO_HZ_COMMON
	بینښه صحيح		nohz_tick_stopped;
	atomic_t		nohz_flags;
	بینښه صحيح		has_blocked_بارول;
	بینښه اوږد		وروستی_blocked_بارول_update_tick;
	call_single_معلومات_t	nohz_csd;
#د_که_پای /* CONFIG_NO_HZ_COMMON */

#که_تعریف_شوی CONFIG_UCLAMP_TASK
	/* Utilization clamp values based on CPU's RUNNABLE tasks */
	بنية uclamp_rq	uclamp[UCLAMP_CNT] ____cacheline_aligned;
	بینښه صحيح		uclamp_flags;
#تعریف_کړه UCLAMP_FLAG_IDLE 0x01
#د_که_پای

	بنية cfs_rq		cfs;
	بنية rt_rq		rt;
	بنية dl_rq		dl;
#که_تعریف_شوی CONFIG_SCHED_CLASS_EXT
	بنية scx_rq		scx;
	بنية sched_dl_entity	ext_server;
#د_که_پای
#که_تعریف_شوی CONFIG_SCHED_CACHE
	raw_spinlock_t		cpu_epoch_قلف ____cacheline_aligned;
	u64			cpu_runtime;
	بینښه اوږد		cpu_epoch;
	بینښه اوږد		cpu_epoch_بل;
#د_که_پای

	بنية sched_dl_entity	fair_server;

#که_تعریف_شوی CONFIG_FAIR_ډله_SCHED
	/* list of leaf cfs_rq on this CPU: */
	بنية list_سر	ونې_پاڼه_cfs_rq_list;
	بنية list_سر	*tmp_alone_branch;
#د_که_پای /* CONFIG_FAIR_GROUP_SCHED */

#که_تعریف_شوی CONFIG_NUMA_BALANCING
	بینښه صحيح		numa_migrate_on;
#د_که_پای

#که_تعریف_شوی CONFIG_SCHED_CACHE
	بینښه صحيح		nr_pref_llc_running;
	بینښه صحيح		nr_llc_running;
#د_که_پای

	/*
	 * This is part of a global counter where only the total sum
	 * over all CPUs matters. A task can increase this counter on
	 * one CPU and if it got migrated afterwards it may decrease
	 * it on another CPU. Always updated under the runqueue lock:
	 */
	بینښه اوږد		nr_uninterruptible;

	بنية sched_dl_entity	*dl_server;
	بنية task_struct	*stop;
	ثابت بنية sched_class *بل_class;
	بینښه اوږد		بل_balance;
	بنية mm_struct	*prev_mm;

	/*
	 * The following fields of clock data are frequently referenced
	 * and updated together, and should go on their own cache line.
	 */
	u64			clock_task ____cacheline_aligned;
	u64			clock_pelt;
	u64			clock;
	بینښه اوږد		ورک_idle_وخت;
	بینښه صحيح		clock_update_flags;
	u64			clock_pelt_idle;
	u64			clock_idle;

#که_نه_تعریف_شوی CONFIG_64_بټ
	u64			clock_pelt_idle_copy;
	u64			clock_idle_copy;
#د_که_پای

	u64 وروستی_کتل_شوی_need_resched_ns;
	صحيح ticks_without_resched;

#که_تعریف_شوی CONFIG_MEMBARRIER
	صحيح membarrier_state;
#د_که_پای

	بنية ریښه_domain		*rd;
	بنية sched_domain __rcu	*sd;

	بنية balance_callback *balance_callback;

	بینښه حرف		nohz_idle_balance;
	بینښه حرف		idle_balance;

	بینښه اوږد		misfit_task_بارول;

	/* For active balancing */
	صحيح			فعال_balance;
	صحيح			ټېل_cpu;
	بنية cpu_stop_work	فعال_balance_work;

	/* CPU of this runqueue: */
	صحيح			cpu;
	صحيح			online;

	بنية list_سر cfs_tasks;

	بنية sched_avg	avg_rt;
	بنية sched_avg	avg_dl;
#که_تعریف_شوی CONFIG_HAVE_SCHED_AVG_IRQ
	بنية sched_avg	avg_irq;
#د_که_پای
#که_تعریف_شوی CONFIG_SCHED_HW_PRESSURE
	بنية sched_avg	avg_hw;
#د_که_پای
	u64			idle_stamp;
	u64			avg_idle;

	/* This is used to determine avg_idle's max value */
	u64			max_idle_balance_لګښت;

#که_تعریف_شوی CONFIG_HOTPLUG_CPU
	بنية rcuwait		hotplug_wait;
#د_که_پای

#که_تعریف_شوی CONFIG_IRQ_وخت_ACCOUNTING
	u64			prev_irq_وخت;
	u64			psi_irq_وخت;
#د_که_پای
#که_تعریف_شوی CONFIG_PARAVIRT
	u64			prev_steal_وخت;
#د_که_پای
#که_تعریف_شوی CONFIG_PARAVIRT_وخت_ACCOUNTING
	u64			prev_steal_وخت_rq;
#د_که_پای
#که_تعریف_شوی CONFIG_PREFERRED_CPU
	bool			npc_ټېل_work_pending;
#د_که_پای

	/* calc_load related fields */
	بینښه اوږد		calc_بارول_update;
	اوږد			calc_بارول_فعال;

#که_تعریف_شوی CONFIG_SCHED_HRTICK
	call_single_معلومات_t	hrtick_csd;
	بنية hrtimer		hrtick_timer;
	ktime_t			hrtick_وخت;
	ktime_t			hrtick_delay;
	بینښه صحيح		hrtick_sched;
#د_که_پای

#که_تعریف_شوی CONFIG_SCHEDSTATS
	/* latency stats */
	بنية sched_info	rq_sched_info;
	بینښه اوږد اوږد	rq_cpu_وخت;

	/* sys_sched_yield() stats */
	بینښه صحيح		yld_شمېر;

	/* schedule() stats */
	بینښه صحيح		sched_شمېر;
	بینښه صحيح		sched_goidle;

	/* try_to_wake_up() stats */
	بینښه صحيح		ttwu_شمېر;
	بینښه صحيح		ttwu_local;
#د_که_پای

#که_تعریف_شوی CONFIG_CPU_IDLE
	/* Must be inspected within a RCU lock section */
	بنية cpuidle_state	*idle_state;
#د_که_پای

	بینښه صحيح		nr_pinned;
	بینښه صحيح		ټېل_busy;
	بنية cpu_stop_work	ټېل_work;

#که_تعریف_شوی CONFIG_SCHED_CORE
	/* per rq */
	بنية rq		*core;
	بنية task_struct	*core_pick;
	بنية sched_dl_entity	*core_dl_server;
	بینښه صحيح		core_enabled;
	بنية rb_ریښه		core_ونه;

	/* shared state -- careful with sched_core_cpu_deactivate() */
	بینښه صحيح		core_task_seq;
	بینښه صحيح		core_pick_seq;
	بینښه اوږد		core_cookie;
	بینښه صحيح		core_forceidle_شمېر;
	بینښه صحيح		core_forceidle_seq;
	بینښه صحيح		core_forceidle_occupation;
	u64			core_forceidle_پیل;
	بینښه صحيح		core_pick_دننه_flight;
#د_که_پای /* CONFIG_SCHED_CORE */

	/* Scratch cpumask to be temporarily used under rq_lock */
	cpumask_var_t		scratch_ماسک;

#که_تعریف_شوی CONFIG_CFS_BANDWIDTH
	call_single_معلومات_t	cfsb_csd;
	بنية list_سر	cfsb_csd_list;
#د_که_پای

	atomic_t		nr_iowait;
} __no_randomize_layout;

#که_تعریف_شوی CONFIG_FAIR_ډله_SCHED

/* CPU runqueue to which this cfs_rq is attached */
ساكن مضمن بنية rq *rq_د(بنية cfs_rq *cfs_rq)
{
	أرجع cfs_rq->rq;
}

#وإلا /* !CONFIG_FAIR_GROUP_SCHED: */

ساكن مضمن بنية rq *rq_د(بنية cfs_rq *cfs_rq)
{
	أرجع container_د(cfs_rq, بنية rq, cfs);
}
#د_که_پای /* !CONFIG_FAIR_GROUP_SCHED */

ساكن مضمن صحيح cpu_د(بنية rq *rq)
{
	أرجع rq->cpu;
}

#تعریف_کړه MDF_ټېل		0x01

ساكن مضمن bool is_migration_disabled(بنية task_struct *p)
{
	أرجع p->migration_disabled;
}

DECLARE_فی_CPU_SHARED_ALIGNED(بنية rq, runqueues);
DECLARE_فی_CPU(بنية rnd_state, sched_rnd_state);

ساكن مضمن u32 sched_rng(فراغ)
{
	أرجع prandom_u32_state(this_cpu_ptr(&sched_rnd_state));
}

ساكن __always_inline بنية rq *__this_rq(فراغ)
{
	أرجع this_cpu_ptr(&runqueues);
}

#تعریف_کړه cpu_rq(cpu)		(&فی_cpu(runqueues, (cpu)))
#تعریف_کړه this_rq()		__this_rq()
#تعریف_کړه task_rq(p)		cpu_rq(task_cpu(p))
#تعریف_کړه cpu_curr(cpu)		(cpu_rq(cpu)->curr)
#تعریف_کړه raw_rq()		raw_cpu_ptr(&runqueues)

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
ساكن مضمن bool شته_idle_cpu(صحيح cpu)
{
	إذا (!idle_rq(cpu_rq(cpu)))
		أرجع 0;

	إذا (vcpu_is_preempted(cpu))
		أرجع 0;

	أرجع 1;
}

#که_تعریف_شوی CONFIG_SCHED_PROXY_EXEC
ساكن مضمن فراغ rq_سېټ_donor(بنية rq *rq, بنية task_struct *t)
{
	rcu_ټاکل_اشاره_ګر(rq->donor, t);
}
#وإلا
ساكن مضمن فراغ rq_سېټ_donor(بنية rq *rq, بنية task_struct *t)
{
	/* Do nothing */
}
#د_که_پای

#که_تعریف_شوی CONFIG_SCHED_CORE
ساكن مضمن بنية cpumask *sched_ډله_پراختیا(بنية sched_ډله *sg);

DECLARE_STATIC_کلی_FALSE(__sched_core_enabled);

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

خارجي فراغ task_vruntime_update(بنية rq *rq, بنية task_struct *p, bool دننه_fi);

/*
 * Helpers to check if the CPU's core cookie matches with the task's cookie
 * when core scheduling is enabled.
 * A special case is that the task's cookie always matches with CPU's core
 * cookie if the CPU is in an idle core.
 */
ساكن مضمن bool sched_cpu_cookie_سمون(بنية rq *rq, بنية task_struct *p)
{
	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	إذا (!sched_core_enabled(rq))
		أرجع true;

	أرجع rq->core->core_cookie == p->core_cookie;
}

ساكن مضمن bool sched_core_cookie_سمون(بنية rq *rq, بنية task_struct *p)
{
	bool idle_core = true;
	صحيح cpu;

	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	إذا (!sched_core_enabled(rq))
		أرجع true;

	إذا (rq->core->core_cookie == p->core_cookie)
		أرجع true;

	for_each_cpu(cpu, cpu_smt_ماسک(cpu_د(rq))) {
		إذا (!شته_idle_cpu(cpu)) {
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

ساكن مضمن bool sched_ډله_cookie_سمون(بنية rq *rq,
					    بنية task_struct *p,
					    بنية sched_ډله *ډله)
{
	صحيح cpu;

	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	إذا (!sched_core_enabled(rq))
		أرجع true;

	for_each_cpu_and(cpu, sched_ډله_پراختیا(ډله), p->cpus_ptr) {
		إذا (sched_core_cookie_سمون(cpu_rq(cpu), p))
			أرجع true;
	}
	أرجع false;
}

ساكن مضمن bool sched_core_enqueued(بنية task_struct *p)
{
	أرجع !RB_تشه_غوټه(&p->core_غوټه);
}

خارجي فراغ sched_core_کتار_ته_ورزیاتول(بنية rq *rq, بنية task_struct *p);
خارجي فراغ sched_core_له_کتار_ایستل(بنية rq *rq, بنية task_struct *p, صحيح flags);

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

ساكن مضمن bool sched_cpu_cookie_سمون(بنية rq *rq, بنية task_struct *p)
{
	أرجع true;
}

ساكن مضمن bool sched_core_cookie_سمون(بنية rq *rq, بنية task_struct *p)
{
	أرجع true;
}

ساكن مضمن bool sched_ډله_cookie_سمون(بنية rq *rq,
					    بنية task_struct *p,
					    بنية sched_ډله *ډله)
{
	أرجع true;
}

ساكن مضمن bool task_has_sched_core(بنية task_struct *p)
{
	أرجع false;
}

#د_که_پای /* !CONFIG_SCHED_CORE */

#که_تعریف_شوی CONFIG_RT_ډله_SCHED
# ifdef CONFIG_RT_ډله_SCHED_DEFAULT_DISABLED
DECLARE_STATIC_کلی_FALSE(rt_ډله_sched);
ساكن مضمن bool rt_ډله_sched_enabled(فراغ)
{
	أرجع static_branch_unlikely(&rt_ډله_sched);
}
# وإلا /* !CONFIG_RT_GROUP_SCHED_DEFAULT_DISABLED: */
DECLARE_STATIC_کلی_TRUE(rt_ډله_sched);
ساكن مضمن bool rt_ډله_sched_enabled(فراغ)
{
	أرجع static_branch_likely(&rt_ډله_sched);
}
# endif /* !CONFIG_RT_GROUP_SCHED_DEFAULT_DISABLED */
#وإلا /* !CONFIG_RT_GROUP_SCHED: */
# define rt_ډله_sched_enabled()	false
#د_که_پای /* !CONFIG_RT_GROUP_SCHED */

ساكن مضمن فراغ lockdep_assert_rq_held(بنية rq *rq)
	__assumes_ctx_lock(__rq_lockp(rq))
{
	lockdep_assert_held(__rq_lockp(rq));
}

خارجي فراغ raw_spin_rq_قلف_nested(بنية rq *rq, صحيح subclass)
	__acquires(__rq_lockp(rq));

خارجي bool raw_spin_rq_trylock(بنية rq *rq)
	__cond_acquires(true, __rq_lockp(rq));

ساكن مضمن فراغ raw_spin_rq_قلف(بنية rq *rq)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_قلف_nested(rq, 0);
}

ساكن مضمن فراغ raw_spin_rq_unlock(بنية rq *rq)
	__releases(__rq_lockp(rq))
{
	raw_spin_unlock(rq_lockp(rq));
}

ساكن مضمن فراغ raw_spin_rq_قلف_irq(بنية rq *rq)
	__acquires(__rq_lockp(rq))
{
	local_irq_disable();
	raw_spin_rq_قلف(rq);
}

ساكن مضمن فراغ raw_spin_rq_unlock_irq(بنية rq *rq)
	__releases(__rq_lockp(rq))
{
	raw_spin_rq_unlock(rq);
	local_irq_enable();
}

ساكن مضمن بینښه اوږد raw_spin_rq_قلف_irqsave_2(بنية rq *rq)
	__acquires(__rq_lockp(rq))
{
	بینښه اوږد flags;

	local_irq_save(flags);
	raw_spin_rq_قلف(rq);

	أرجع flags;
}

ساكن مضمن فراغ raw_spin_rq_unlock_irqrestore(بنية rq *rq, بینښه اوږد flags)
	__releases(__rq_lockp(rq))
{
	raw_spin_rq_unlock(rq);
	local_irq_restore(flags);
}

#تعریف_کړه raw_spin_rq_قلف_irqsave(rq, flags)	\
افعل {						\
	flags = raw_spin_rq_قلف_irqsave_2(rq);	\
} طالما (0)

خارجي فراغ __update_idle_core(بنية rq *rq);

ساكن مضمن فراغ update_idle_core(بنية rq *rq)
{
	إذا (sched_smt_فعال())
		__update_idle_core(rq);
}

#که_تعریف_شوی CONFIG_FAIR_ډله_SCHED
ساكن مضمن بنية task_struct *task_د(بنية sched_entity *se)
{
	WARN_ON_ONCE(!entity_is_task(se));
	أرجع container_د(se, بنية task_struct, se);
}

ساكن مضمن بنية cfs_rq *task_cfs_rq(بنية task_struct *p)
{
	أرجع p->se.cfs_rq;
}

/* runqueue on which this entity is (to be) queued */
ساكن مضمن بنية cfs_rq *cfs_rq_د(ثابت بنية sched_entity *se)
{
	أرجع se->cfs_rq;
}

/* runqueue "owned" by this group */
ساكن مضمن بنية cfs_rq *ډله_cfs_rq(بنية sched_entity *grp)
{
	أرجع grp->my_q;
}

#وإلا /* !CONFIG_FAIR_GROUP_SCHED: */

#تعریف_کړه task_د(_se)		container_د(_se, بنية task_struct, se)

ساكن مضمن بنية cfs_rq *task_cfs_rq(ثابت بنية task_struct *p)
{
	أرجع &task_rq(p)->cfs;
}

ساكن مضمن بنية cfs_rq *cfs_rq_د(ثابت بنية sched_entity *se)
{
	ثابت بنية task_struct *p = task_د(se);
	بنية rq *rq = task_rq(p);

	أرجع &rq->cfs;
}

/* runqueue "owned" by this group */
ساكن مضمن بنية cfs_rq *ډله_cfs_rq(بنية sched_entity *grp)
{
	أرجع عدم;
}

#د_که_پای /* !CONFIG_FAIR_GROUP_SCHED */

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
#تعریف_کړه RQCF_REQ_SKIP		0x01
#تعریف_کړه RQCF_ACT_SKIP		0x02
#تعریف_کړه RQCF_UPDATED		0x04

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
ساكن مضمن فراغ rq_clock_پیل_loop_update(بنية rq *rq)
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
	بینښه اوږد flags;
	بنية pin_cookie cookie;
	/*
	 * A copy of (rq::clock_update_flags & RQCF_UPDATED) for the
	 * current pin context is stashed here in case it needs to be
	 * restored in rq_repin_lock().
	 */
	بینښه صحيح clock_update_flags;
};

خارجي بنية balance_callback balance_ټېل_callback;

#که_تعریف_شوی CONFIG_SCHED_CLASS_EXT
خارجي ثابت بنية sched_class ext_sched_class;

DECLARE_STATIC_کلی_FALSE(__scx_enabled);	/* SCX BPF scheduler loaded */
DECLARE_STATIC_کلی_FALSE(__scx_switched_all);	/* all fair class tasks on SCX */

#تعریف_کړه scx_enabled()		static_branch_unlikely(&__scx_enabled)
#تعریف_کړه scx_switched_all()	static_branch_unlikely(&__scx_switched_all)

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
#تعریف_کړه scx_enabled()		false
#تعریف_کړه scx_switched_all()	false

ساكن مضمن فراغ scx_rq_clock_update(بنية rq *rq, u64 clock) {}
ساكن مضمن فراغ scx_rq_clock_invalidate(بنية rq *rq) {}
#د_که_پای /* !CONFIG_SCHED_CLASS_EXT */

ساكن مضمن فراغ assert_balance_callbacks_تشه(بنية rq *rq)
{
	WARN_ON_ONCE(IS_ENABLED(CONFIG_PROVE_LOCKING) &&
		     rq->balance_callback &&
		     rq->balance_callback != &balance_ټېل_callback);
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
ساكن مضمن فراغ rq_pin_قلف(بنية rq *rq, بنية rq_flags *rf)
{
	rf->cookie = lockdep_pin_قلف(__rq_lockp(rq));

	rq->clock_update_flags &= (RQCF_REQ_SKIP|RQCF_ACT_SKIP);
	rf->clock_update_flags = 0;
	assert_balance_callbacks_تشه(rq);
}

ساكن مضمن فراغ rq_unpin_قلف(بنية rq *rq, بنية rq_flags *rf)
{
	إذا (rq->clock_update_flags > RQCF_ACT_SKIP)
		rf->clock_update_flags = RQCF_UPDATED;

	scx_rq_clock_invalidate(rq);
	lockdep_unpin_قلف(__rq_lockp(rq), rf->cookie);
}

ساكن مضمن فراغ rq_repin_قلف(بنية rq *rq, بنية rq_flags *rf)
{
	lockdep_repin_قلف(__rq_lockp(rq), rf->cookie);

	/*
	 * Restore the value we stashed in @rf for this pin context.
	 */
	rq->clock_update_flags |= rf->clock_update_flags;
}

#تعریف_کړه __task_rq_lock(...) __acquire_ret(___task_rq_lock(__VA_ARGS__), __rq_lockp(__ret))
خارجي بنية rq *___task_rq_lock(بنية task_struct *p, بنية rq_flags *rf) __acquires_ret;

#تعریف_کړه task_rq_قلف_2(...) __acquire_ret(task_rq_قلف(__VA_ARGS__), __rq_lockp(__ret))
خارجي بنية rq *task_rq_قلف(بنية task_struct *p, بنية rq_flags *rf)
	__acquires(&p->pi_قلف) __acquires_ret;

ساكن مضمن فراغ
__task_rq_unlock(بنية rq *rq, بنية task_struct *p, بنية rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_قلف(rq, rf);
	raw_spin_rq_unlock(rq);
}

ساكن مضمن فراغ
task_rq_unlock(بنية rq *rq, بنية task_struct *p, بنية rq_flags *rf)
	__releases(__rq_lockp(rq), &p->pi_قلف)
{
	__task_rq_unlock(rq, p, rf);
	raw_spin_unlock_irqrestore(&p->pi_قلف, rf->flags);
}

DEFINE_قلف_GUARD_1(task_rq_قلف_2, بنية task_struct,
		    _T->rq = task_rq_قلف_2(_T->قلف, &_T->rf),
		    task_rq_unlock(_T->rq, _T->قلف, &_T->rf),
		    بنية rq *rq; بنية rq_flags rf)
DECLARE_قلف_GUARD_1_ATTRS(task_rq_قلف_2, __acquires(_T->pi_قلف), __releases((*(بنية task_struct **)_T)->pi_قلف))
#تعریف_کړه class_task_rq_قلف_constructor(_T) WITH_قلف_GUARD_1_ATTRS(task_rq_قلف_2, _T)

DEFINE_قلف_GUARD_1(__task_rq_lock, بنية task_struct,
		    _T->rq = __task_rq_lock(_T->قلف, &_T->rf),
		    __task_rq_unlock(_T->rq, _T->قلف, &_T->rf),
		    بنية rq *rq; بنية rq_flags rf)

ساكن مضمن فراغ rq_قلف_irqsave(بنية rq *rq, بنية rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_قلف_irqsave(rq, rf->flags);
	rq_pin_قلف(rq, rf);
}

ساكن مضمن فراغ rq_قلف_irq(بنية rq *rq, بنية rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_قلف_irq(rq);
	rq_pin_قلف(rq, rf);
}

ساكن مضمن فراغ rq_قلف(بنية rq *rq, بنية rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_قلف(rq);
	rq_pin_قلف(rq, rf);
}

ساكن مضمن فراغ rq_unlock_irqrestore(بنية rq *rq, بنية rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_قلف(rq, rf);
	raw_spin_rq_unlock_irqrestore(rq, rf->flags);
}

ساكن مضمن فراغ rq_unlock_irq(بنية rq *rq, بنية rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_قلف(rq, rf);
	raw_spin_rq_unlock_irq(rq);
}

ساكن مضمن فراغ rq_unlock(بنية rq *rq, بنية rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_قلف(rq, rf);
	raw_spin_rq_unlock(rq);
}

DEFINE_قلف_GUARD_1(rq_قلف, بنية rq,
		    rq_قلف(_T->قلف, &_T->rf),
		    rq_unlock(_T->قلف, &_T->rf),
		    بنية rq_flags rf)

DECLARE_قلف_GUARD_1_ATTRS(rq_قلف, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(بنية rq **)_T)));
#تعریف_کړه class_rq_قلف_constructor(_T) WITH_قلف_GUARD_1_ATTRS(rq_قلف, _T)

DEFINE_قلف_GUARD_1(rq_قلف_irq, بنية rq,
		    rq_قلف_irq(_T->قلف, &_T->rf),
		    rq_unlock_irq(_T->قلف, &_T->rf),
		    بنية rq_flags rf)

DECLARE_قلف_GUARD_1_ATTRS(rq_قلف_irq, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(بنية rq **)_T)));
#تعریف_کړه class_rq_قلف_irq_constructor(_T) WITH_قلف_GUARD_1_ATTRS(rq_قلف_irq, _T)

DEFINE_قلف_GUARD_1(rq_قلف_irqsave, بنية rq,
		    rq_قلف_irqsave(_T->قلف, &_T->rf),
		    rq_unlock_irqrestore(_T->قلف, &_T->rf),
		    بنية rq_flags rf)

DECLARE_قلف_GUARD_1_ATTRS(rq_قلف_irqsave, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(بنية rq **)_T)));
#تعریف_کړه class_rq_قلف_irqsave_constructor(_T) WITH_قلف_GUARD_1_ATTRS(rq_قلف_irqsave, _T)

DEFINE_قلف_GUARD_1(raw_spin_rq_قلف_irqsave, بنية rq,
		    raw_spin_rq_قلف_irqsave(_T->قلف, _T->flags),
		    raw_spin_rq_unlock_irqrestore(_T->قلف, _T->flags),
		    بینښه اوږد flags)

DECLARE_قلف_GUARD_1_ATTRS(raw_spin_rq_قلف_irqsave, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(بنية rq **)_T)));
#تعریف_کړه class_raw_spin_rq_قلف_irqsave_constructor(_T) \
	WITH_قلف_GUARD_1_ATTRS(raw_spin_rq_قلف_irqsave, _T)

#تعریف_کړه this_rq_قلف_irq(...) __acquire_ret(this_rq_قلف_irq_2(__VA_ARGS__), __rq_lockp(__ret))
ساكن مضمن بنية rq *this_rq_قلف_irq_2(بنية rq_flags *rf) __acquires_ret
{
	بنية rq *rq;

	local_irq_disable();
	rq = this_rq();
	rq_قلف(rq, rf);

	أرجع rq;
}

#که_تعریف_شوی CONFIG_NUMA

تعداد numa_topology_type {
	NUMA_مستقیم,
	NUMA_GLUELESS_MESH,
	NUMA_BACKPLANE,
};

خارجي تعداد numa_topology_type sched_numa_topology_type;
خارجي صحيح sched_max_numa_واټن;
خارجي bool پیدا_کړه_numa_واټن(صحيح واټن);
خارجي فراغ sched_init_numa(صحيح offline_غوټه);
خارجي فراغ sched_update_numa(صحيح cpu, bool online);
خارجي فراغ sched_domains_numa_masks_سېټ(بینښه صحيح cpu);
خارجي فراغ sched_domains_numa_masks_clear(بینښه صحيح cpu);
خارجي صحيح sched_numa_پیدا_کړه_closest(ثابت بنية cpumask *cpus, صحيح cpu);

#وإلا /* !CONFIG_NUMA: */

ساكن مضمن فراغ sched_init_numa(صحيح offline_غوټه) { }
ساكن مضمن فراغ sched_update_numa(صحيح cpu, bool online) { }
ساكن مضمن فراغ sched_domains_numa_masks_سېټ(بینښه صحيح cpu) { }
ساكن مضمن فراغ sched_domains_numa_masks_clear(بینښه صحيح cpu) { }

ساكن مضمن صحيح sched_numa_پیدا_کړه_closest(ثابت بنية cpumask *cpus, صحيح cpu)
{
	أرجع nr_cpu_ids;
}

#د_که_پای /* !CONFIG_NUMA */

#که_تعریف_شوی CONFIG_NUMA_BALANCING

/* The regions in numa_faults array from task_struct */
تعداد numa_تېروتنې_stats {
	NUMA_MEM = 0,
	NUMA_CPU,
	NUMA_MEMBUF,
	NUMA_CPUBUF
};

خارجي فراغ sched_setnuma(بنية task_struct *p, صحيح غوټه);
خارجي صحيح migrate_task_منزل(بنية task_struct *p, صحيح cpu);
خارجي صحيح migrate_بدلون(بنية task_struct *p, بنية task_struct *t,
			صحيح cpu, صحيح scpu);
خارجي فراغ init_numa_balancing(u64 clone_flags, بنية task_struct *p);

#وإلا /* !CONFIG_NUMA_BALANCING: */

ساكن مضمن فراغ
init_numa_balancing(u64 clone_flags, بنية task_struct *p)
{
}

#د_که_پای /* !CONFIG_NUMA_BALANCING */

صحيح task_llc(ثابت بنية task_struct *p);

ساكن مضمن فراغ
کتار_balance_callback(بنية rq *rq,
		       بنية balance_callback *سر,
		       فراغ (*func)(بنية rq *rq))
{
	lockdep_assert_rq_held(rq);

	/*
	 * Don't (re)queue an already queued item; nor queue anything when
	 * balance_push() is active, see the comment with
	 * balance_push_callback.
	 */
	إذا (unlikely(سر->بل || rq->balance_callback == &balance_ټېل_callback))
		أرجع;

	سر->func = func;
	سر->بل = rq->balance_callback;
	rq->balance_callback = سر;
}

#تعریف_کړه rcu_dereference_sched_domain(p) \
	rcu_dereference_all_check((p), lockdep_is_held(&sched_domains_mutex))

/*
 * The domain tree (rq->sd) is protected by RCU's quiescent state transition.
 * See destroy_sched_domains: call_rcu for details.
 *
 * The domain tree of any CPU may only be accessed from within
 * preempt-disabled sections.
 */
#تعریف_کړه for_each_domain(cpu, __sd) \
	لكل (__sd = rcu_dereference_sched_domain(cpu_rq(cpu)->sd); \
			__sd; __sd = __sd->پلار)

/* A mask of all the SD flags that have the SDF_SHARED_CHILD metaflag */
#تعریف_کړه SD_FLAG(name, mflags) (name * !!((mflags) & SDF_SHARED_ماشوم)) |
ساكن ثابت بینښه صحيح SD_SHARED_ماشوم_ماسک =
#ضمن <linux/sched/sd_flags.h>
0;
#تعریف_لغوه SD_FLAG

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
		إذا (flag & SD_SHARED_ماشوم_ماسک)
			اكسر;
	}

	أرجع hsd;
}

ساكن مضمن بنية sched_domain *ټیټ_ترین_flag_domain(صحيح cpu, صحيح flag)
{
	بنية sched_domain *sd;

	for_each_domain(cpu, sd) {
		إذا (sd->flags & flag)
			اكسر;
	}

	أرجع sd;
}

DECLARE_فی_CPU(بنية sched_domain __rcu *, sd_llc);
DECLARE_فی_CPU(صحيح, sd_llc_اندازه);
DECLARE_فی_CPU(صحيح, sd_llc_id);
DECLARE_فی_CPU(صحيح, sd_share_id);
DECLARE_فی_CPU(بنية sched_domain_shared __rcu *, sd_llc_shared);
DECLARE_فی_CPU(بنية sched_domain_shared __rcu *, sd_balance_shared);
DECLARE_فی_CPU(بنية sched_domain __rcu *, sd_numa);
DECLARE_فی_CPU(بنية sched_domain __rcu *, sd_asym_packing);
DECLARE_فی_CPU(بنية sched_domain __rcu *, sd_asym_cpucapacity);

خارجي بنية static_کلی_false sched_asym_cpucapacity;
خارجي بنية static_کلی_false sched_cluster_فعال;

ساكن __always_inline bool sched_asym_cpucap_فعال(فراغ)
{
	أرجع static_branch_unlikely(&sched_asym_cpucapacity);
}

بنية sched_ډله_ظرفیت {
	atomic_t		ref;
	/*
	 * CPU capacity of this group, SCHED_CAPACITY_SCALE being max capacity
	 * for a single CPU.
	 */
	بینښه اوږد		ظرفیت;
	بینښه اوږد		min_ظرفیت;		/* Min per-CPU capacity in group */
	بینښه اوږد		max_ظرفیت;		/* Max per-CPU capacity in group */
	بینښه اوږد		بل_update;
	صحيح			imbalance;		/* XXX unrelated to capacity but shared group state */

	صحيح			id;

	بینښه اوږد		cpumask[];		/* Balance mask */
};

بنية sched_ډله {
	بنية sched_ډله	*بل;			/* Must be a circular list */
	atomic_t		ref;

	بینښه صحيح		ډله_وزن;
	بینښه صحيح		cores;
	بنية sched_ډله_ظرفیت *sgc;
	صحيح			asym_prefer_cpu;	/* CPU of highest priority in group */
	صحيح			flags;

	/*
	 * The CPUs this group covers.
	 *
	 * NOTE: this field is variable length. (Allocated dynamically
	 * by attaching extra space to the end of the structure,
	 * depending on how many CPUs the kernel has booted up with)
	 */
	بینښه اوږد		cpumask[];
};

ساكن مضمن بنية cpumask *sched_ډله_پراختیا(بنية sched_ډله *sg)
{
	أرجع منزل_cpumask(sg->cpumask);
}

/*
 * See build_balance_mask().
 */
ساكن مضمن بنية cpumask *ډله_balance_ماسک(بنية sched_ډله *sg)
{
	أرجع منزل_cpumask(sg->sgc->cpumask);
}

خارجي صحيح ډله_balance_cpu(بنية sched_ډله *sg);

خارجي فراغ update_sched_domain_debugfs(فراغ);
خارجي فراغ dirty_sched_domain_sysctl(صحيح cpu);

خارجي صحيح sched_update_scaling(فراغ);

ساكن مضمن ثابت بنية cpumask *task_user_cpus(بنية task_struct *p)
{
	إذا (!p->user_cpus_ptr)
		أرجع cpu_possible_ماسک; /* &init_task.cpus_mask */
	أرجع p->user_cpus_ptr;
}

#که_تعریف_شوی CONFIG_CGROUP_SCHED

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
ساكن مضمن بنية task_ډله *task_ډله(بنية task_struct *p)
{
	أرجع p->sched_task_ډله;
}

#که_تعریف_شوی CONFIG_FAIR_ډله_SCHED
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
ساكن مضمن بنية cfs_rq *tg_cfs_rq(بنية task_ډله *tg, صحيح cpu)
{
	أرجع فی_cpu_ptr(tg->cfs_rq, cpu);
}

ساكن مضمن بنية sched_entity *tg_se(بنية task_ډله *tg, صحيح cpu)
{
	بنية cfs_tg_state *state;

	إذا (is_ریښه_task_ډله(tg))
		أرجع عدم;

	state = container_د(tg_cfs_rq(tg, cpu), بنية cfs_tg_state, cfs_rq);
	أرجع &state->se;
}

ساكن مضمن بنية sched_entity *cfs_rq_se(بنية cfs_rq *cfs_rq)
{
	بنية cfs_tg_state *state;

	إذا (is_ریښه_task_ډله(cfs_rq->tg))
		أرجع عدم;

	state = container_د(cfs_rq, بنية cfs_tg_state, cfs_rq);
	أرجع &state->se;
}
#د_که_پای

/* Change a task's cfs_rq and parent entity if it moves across CPUs/groups */
ساكن مضمن فراغ سېټ_task_rq(بنية task_struct *p, بینښه صحيح cpu)
{
#إذا defined(CONFIG_FAIR_ډله_SCHED) || defined(CONFIG_RT_ډله_SCHED)
	بنية task_ډله *tg = task_ډله(p);
#د_که_پای

#که_تعریف_شوی CONFIG_FAIR_ډله_SCHED
	سېټ_task_rq_fair(&p->se, p->se.cfs_rq, tg_cfs_rq(tg, cpu));
	p->se.cfs_rq = tg_cfs_rq(tg, cpu);
	p->se.پلار = tg_se(tg, cpu);
	p->se.ژوروالی = p->se.پلار ? p->se.پلار->ژوروالی + 1 : 0;
#د_که_پای

#که_تعریف_شوی CONFIG_RT_ډله_SCHED
	/*
	 * p->rt.rt_rq is NULL initially and it is easier to assign
	 * root_task_group's rt_rq than switching in rt_rq_of_se()
	 * Clobbers tg(!)
	 */
	إذا (!rt_ډله_sched_enabled())
		tg = &ریښه_task_ډله;
	p->rt.rt_rq  = tg->rt_rq[cpu];
	p->rt.پلار = tg->rt_se[cpu];
#د_که_پای /* CONFIG_RT_GROUP_SCHED */
}

#وإلا /* !CONFIG_CGROUP_SCHED: */

ساكن مضمن فراغ سېټ_task_rq(بنية task_struct *p, بینښه صحيح cpu) { }

ساكن مضمن بنية task_ډله *task_ډله(بنية task_struct *p)
{
	أرجع عدم;
}

#د_که_پای /* !CONFIG_CGROUP_SCHED */

ساكن مضمن فراغ __set_task_cpu(بنية task_struct *p, بینښه صحيح cpu)
{
	سېټ_task_rq(p, cpu);
#که_تعریف_شوی CONFIG_SMP
	/*
	 * After ->cpu is set up to a new value, task_rq_lock(p, ...) can be
	 * successfully executed on another CPU. We must ensure that updates of
	 * per-task data have been completed by this moment.
	 */
	smp_wmb();
	WRITE_ONCE(task_thread_info(p)->cpu, cpu);
	p->wake_cpu = cpu;
	rseq_sched_سېټ_ids_بدل_شوی(p);
#د_که_پای /* CONFIG_SMP */
}

/*
 * Tunables:
 */

#تعریف_کړه SCHED_FEAT(name, enabled)	\
	__SCHED_FEAT_##name ,

تعداد {
#ضمن "features.h"
	__SCHED_FEAT_NR,
};

#تعریف_لغوه SCHED_FEAT

/*
 * To support run-time toggling of sched features, all the translation units
 * (but core.c) reference the sysctl_sched_features defined in core.c.
 */
خارجي __read_mostly بینښه صحيح sysctl_sched_features;

#که_تعریف_شوی CONFIG_JUMP_LABEL

اتحاد sched_feat_کلی {
	بنية static_کلی_true	کلی_true;
	بنية static_کلی_false	کلی_false;
};

#تعریف_کړه sched_feat_branch_true(کلی)	static_branch_likely(&(کلی)->کلی_true)
#تعریف_کړه sched_feat_branch_false(کلی)	static_branch_unlikely(&(کلی)->کلی_false)

#تعریف_کړه SCHED_FEAT(name, enabled)			\
ساكن __always_inline bool				\
static_branch_##name(اتحاد sched_feat_کلی *کلی) 	\
{							\
	أرجع sched_feat_branch_##enabled(کلی);	\
}

#ضمن "features.h"
#تعریف_لغوه SCHED_FEAT

خارجي اتحاد sched_feat_کلی sched_feat_کلیګانې[__SCHED_FEAT_NR];
#تعریف_کړه sched_feat(x) (static_branch_##x(&sched_feat_کلیګانې[__SCHED_FEAT_##x]))

#وإلا /* !CONFIG_JUMP_LABEL: */

#تعریف_کړه sched_feat(x) (sysctl_sched_features & (1UL << __SCHED_FEAT_##x))

#د_که_پای /* !CONFIG_JUMP_LABEL */

خارجي بنية static_کلی_false sched_numa_balancing;
خارجي بنية static_کلی_false sched_schedstats;

ساكن مضمن u64 global_rt_period(فراغ)
{
	أرجع (u64)sysctl_sched_rt_period * NSEC_فی_USEC;
}

ساكن مضمن u64 global_rt_runtime(فراغ)
{
	إذا (sysctl_sched_rt_runtime < 0)
		أرجع RUNTIME_INF;

	أرجع (u64)sysctl_sched_rt_runtime * NSEC_فی_USEC;
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

#که_تعریف_شوی CONFIG_SCHED_PROXY_EXEC
فراغ sched_proxy_بلاک_task(بنية rq *rq, بنية task_struct *p);
#وإلا
ساكن مضمن فراغ sched_proxy_بلاک_task(بنية rq *rq, بنية task_struct *p) {}
#د_که_پای

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
#تعریف_کړه WF_EXEC			0x02 /* Wakeup after exec; maps to SD_BALANCE_EXEC */
#تعریف_کړه WF_FORK			0x04 /* Wakeup after fork; maps to SD_BALANCE_FORK */
#تعریف_کړه WF_TTWU			0x08 /* Wakeup;            maps to SD_BALANCE_WAKE */
/*
 * Hint that the caller expects the waker to sleep soon.
 * Scheduler classes may use it for placement or preemption.
 * Callers must not rely on it to prevent migration,
 * preserve CPU locality or make the wakee run next.
 */
#تعریف_کړه WF_SYNC			0x10
#تعریف_کړه WF_MIGRATED		0x20 /* Internal use, task got migrated */
#تعریف_کړه WF_CURRENT_CPU		0x40 /* Prefer to move the wakee to the current CPU. */
#تعریف_کړه WF_RQ_SELECTED		0x80 /* ->select_task_rq() was called */
#تعریف_کړه WF_TTWU_RQ		0x100 /* Wakeup completed through ttwu_runnable() */

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

#تعریف_کړه وزن_IDLEPRIO		3
#تعریف_کړه WMULT_IDLEPRIO		1431655765

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

#تعریف_کړه له_کتار_ایستل_SLEEP		0x0001 /* Matches ENQUEUE_WAKEUP */
#تعریف_کړه له_کتار_ایستل_SAVE		0x0002 /* Matches ENQUEUE_RESTORE */
#تعریف_کړه له_کتار_ایستل_MOVE		0x0004 /* Matches ENQUEUE_MOVE */
#تعریف_کړه له_کتار_ایستل_NOCLOCK		0x0008 /* Matches ENQUEUE_NOCLOCK */

#تعریف_کړه له_کتار_ایستل_MIGRATING	0x0010 /* Matches ENQUEUE_MIGRATING */
#تعریف_کړه له_کتار_ایستل_DELAYED		0x0020 /* Matches ENQUEUE_DELAYED */
#تعریف_کړه له_کتار_ایستل_CLASS		0x0040 /* Matches ENQUEUE_CLASS */

#تعریف_کړه له_کتار_ایستل_SPECIAL		0x00010000
#تعریف_کړه له_کتار_ایستل_THROTTLE	0x00020000

#تعریف_کړه کتار_ته_ورزیاتول_WAKEUP		0x0001
#تعریف_کړه کتار_ته_ورزیاتول_RESTORE		0x0002
#تعریف_کړه کتار_ته_ورزیاتول_MOVE		0x0004
#تعریف_کړه کتار_ته_ورزیاتول_NOCLOCK		0x0008

#تعریف_کړه کتار_ته_ورزیاتول_MIGRATING	0x0010
#تعریف_کړه کتار_ته_ورزیاتول_DELAYED		0x0020
#تعریف_کړه کتار_ته_ورزیاتول_CLASS		0x0040

#تعریف_کړه کتار_ته_ورزیاتول_سر		0x00010000
#تعریف_کړه کتار_ته_ورزیاتول_REPLENISH	0x00020000
#تعریف_کړه کتار_ته_ورزیاتول_MIGRATED	0x00040000
#تعریف_کړه کتار_ته_ورزیاتول_INITIAL		0x00080000
#تعریف_کړه کتار_ته_ورزیاتول_RQ_SELECTED	0x00100000
#تعریف_کړه کتار_ته_ورزیاتول_QUEUED		0x00200000

#تعریف_کړه RETRY_TASK		((فراغ *)-1UL)

بنية affinity_context {
	ثابت بنية cpumask	*new_ماسک;
	بنية cpumask		*user_ماسک;
	بینښه صحيح		flags;
};

خارجي s64 update_curr_common(بنية rq *rq);

تعداد snt_e {
	SNT_نورمال,	/* set_next_task() */
	SNT_PICK,	/* put_prev_set_next_task(): prev != next */
	SNT_REPICK,	/* put_prev_set_next_task(): prev == next */
};

بنية sched_class {

#که_تعریف_شوی CONFIG_UCLAMP_TASK
	صحيح uclamp_enabled;
#د_که_پای

	/*
	 * move_queued_task/activate_task/enqueue_task: rq->lock
	 * ttwu_do_activate/activate_task/enqueue_task: rq->lock
	 * wake_up_new_task/activate_task/enqueue_task: task_rq_lock
	 * ttwu_runnable/enqueue_task: task_rq_lock
	 * proxy_task_current: rq->lock
	 * sched_change_end
	 */
	فراغ (*کتار_ته_ورزیاتول_task) (بنية rq *rq, بنية task_struct *p, صحيح flags);
	/*
	 * move_queued_task/deactivate_task/dequeue_task: rq->lock
	 * __schedule/block_task/dequeue_task: rq->lock
	 * proxy_task_current: rq->lock
	 * wait_task_inactive: task_rq_lock
	 * sched_change_begin
	 */
	bool (*له_کتار_ایستل_task) (بنية rq *rq, بنية task_struct *p, صحيح flags);

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
	فراغ (*put_prev_task)(بنية rq *rq, بنية task_struct *p, بنية task_struct *بل);
	فراغ (*سېټ_بل_task)(بنية rq *rq, بنية task_struct *p, تعداد snt_e type);

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
	فراغ (*سېټ_cpus_allowed)(بنية task_struct *p, بنية affinity_context *ctx);

	/*
	 * sched_set_rq_{on,off}line: rq->lock
	 */
	فراغ (*rq_online)(بنية rq *rq);
	فراغ (*rq_offline)(بنية rq *rq);

	/*
	 * push_cpu_stop: p->pi_lock && rq->lock
	 */
	بنية rq *(*پیدا_کړه_قلف_rq)(بنية task_struct *p, بنية rq *rq);

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
	فراغ (*prio_بدل_شوی) (بنية rq *this_rq, بنية task_struct *task,
			      u64 oldprio);

	/*
	 * set_load_weight: task_rq_lock + sched_change
	 * __setscheduler_parms: task_rq_lock + sched_change
	 */
	فراغ (*reweight_task)(بنية rq *this_rq, بنية task_struct *task,
			      ثابت بنية بارول_وزن *lw);

	/*
	 * sched_rr_get_interval: task_rq_lock
	 */
	بینښه صحيح (*get_rr_interval)(بنية rq *rq,
					بنية task_struct *task);

	/*
	 * task_sched_runtime: task_rq_lock
	 */
	فراغ (*update_curr)(بنية rq *rq);

#که_تعریف_شوی CONFIG_FAIR_ډله_SCHED
	/*
	 * sched_change_group: task_rq_lock + sched_change
	 */
	فراغ (*task_change_ډله)(بنية task_struct *p);
#د_که_پای

#که_تعریف_شوی CONFIG_SCHED_CORE
	/*
	 * pick_next_task: rq->lock
	 * try_steal_cookie: rq->lock (double)
	 */
	صحيح (*task_is_throttled)(بنية task_struct *p, صحيح cpu);
#د_که_پای
};

ساكن مضمن فراغ put_prev_task(بنية rq *rq, بنية task_struct *prev)
{
	WARN_ON_ONCE(rq->donor != prev);
	prev->sched_class->put_prev_task(rq, prev, عدم);
}

ساكن مضمن فراغ سېټ_بل_task(بنية rq *rq, بنية task_struct *بل)
{
	بل->sched_class->سېټ_بل_task(rq, بل, SNT_نورمال);
}

ساكن مضمن فراغ
__put_prev_set_next_dl_server(بنية rq *rq,
			      بنية task_struct *prev,
			      بنية task_struct *بل)
{
	prev->dl_server = عدم;
	بل->dl_server = rq->dl_server;
	rq->dl_server = عدم;
}

ساكن مضمن فراغ put_prev_سېټ_بل_task(بنية rq *rq,
					  بنية task_struct *prev,
					  بنية task_struct *بل)
{
	WARN_ON_ONCE(rq->donor != prev);

	__put_prev_set_next_dl_server(rq, prev, بل);

	إذا (بل == prev) {
		بل->sched_class->سېټ_بل_task(rq, بل, SNT_REPICK);
		أرجع;
	}

	prev->sched_class->put_prev_task(rq, prev, بل);
	بل->sched_class->سېټ_بل_task(rq, بل, SNT_PICK);
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
#تعریف_کړه DEFINE_SCHED_CLASS(name) \
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
ساكن مضمن ثابت بنية sched_class *بل_فعال_class(ثابت بنية sched_class *class)
{
	class++;
#که_تعریف_شوی CONFIG_SCHED_CLASS_EXT
	إذا (scx_switched_all() && class == &fair_sched_class)
		class++;
	إذا (!scx_enabled() && class == &ext_sched_class)
		class++;
#د_که_پای
	أرجع class;
}

#تعریف_کړه for_class_range(class, _from, منزل_2) \
	لكل (class = (_from); class < (منزل_2); class++)

#تعریف_کړه for_each_class(class) \
	for_class_range(class, __sched_class_highest, __sched_class_lowest)

#تعریف_کړه for_فعال_class_range(class, _from, منزل_2)				\
	لكل (class = (_from); class != (منزل_2); class = بل_فعال_class(class))

#تعریف_کړه for_each_فعال_class(class)						\
	for_فعال_class_range(class, __sched_class_highest, __sched_class_lowest)

#تعریف_کړه sched_class_above(_a, _b)	((_a) < (_b))

ساكن مضمن فراغ rq_modified_begin(بنية rq *rq, ثابت بنية sched_class *class)
{
	إذا (sched_class_above(rq->بل_class, class))
		rq->بل_class = class;
}

ساكن مضمن bool rq_modified_above(بنية rq *rq, ثابت بنية sched_class *class)
{
	أرجع sched_class_above(rq->بل_class, class);
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

#تعریف_کړه SCA_CHECK		0x01
#تعریف_کړه SCA_MIGRATE_DISABLE	0x02
#تعریف_کړه SCA_MIGRATE_ENABLE	0x04
#تعریف_کړه SCA_USER		0x08

خارجي فراغ update_ډله_ظرفیت(بنية sched_domain *sd, صحيح cpu);

خارجي فراغ sched_balance_trigger(بنية rq *rq);

خارجي صحيح __set_cpus_allowed_ptr(بنية task_struct *p, بنية affinity_context *ctx);
خارجي فراغ سېټ_cpus_allowed_common(بنية task_struct *p, بنية affinity_context *ctx);

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

ساكن مضمن cpumask_t *alloc_user_cpus_ptr(صحيح غوټه)
{
	/*
	 * See set_cpus_allowed_force() above for the rcu_head usage.
	 */
	صحيح اندازه = max_t(صحيح, cpumask_اندازه(), حجم(بنية rcu_سر));

	أرجع kmalloc_غوټه(اندازه, GFP_KERNEL, غوټه);
}

ساكن مضمن بنية task_struct *get_ټېل_task(بنية rq *rq)
{
	بنية task_struct *p = rq->donor;

	lockdep_assert_rq_held(rq);

	إذا (rq->ټېل_busy)
		أرجع عدم;

	إذا (p->nr_cpus_allowed == 1)
		أرجع عدم;

	إذا (p->migration_disabled)
		أرجع عدم;

	rq->ټېل_busy = true;
	أرجع get_task_struct(p);
}

خارجي صحيح ټېل_cpu_stop(فراغ *arg);

#که_تعریف_شوی CONFIG_CPU_IDLE

ساكن مضمن فراغ idle_سېټ_state(بنية rq *rq,
				  بنية cpuidle_state *idle_state)
{
	rq->idle_state = idle_state;
}

ساكن مضمن بنية cpuidle_state *idle_get_state(بنية rq *rq)
{
	lockdep_assert(rcu_read_قلف_any_held());

	أرجع rq->idle_state;
}

#وإلا /* !CONFIG_CPU_IDLE: */

ساكن مضمن فراغ idle_سېټ_state(بنية rq *rq,
				  بنية cpuidle_state *idle_state)
{
}

ساكن مضمن بنية cpuidle_state *idle_get_state(بنية rq *rq)
{
	أرجع عدم;
}

#د_که_پای /* !CONFIG_CPU_IDLE */

خارجي فراغ مهالوېش_idle(فراغ);
asmlinkage فراغ مهالوېش_user(فراغ);

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

#تعریف_کړه BW_ښويدنه		20
#تعریف_کړه BW_UNIT			(1 << BW_ښويدنه)
#تعریف_کړه نسبت_ښويدنه		8
#تعریف_کړه MAX_BW_بټونه		(64 - BW_ښويدنه)
#تعریف_کړه MAX_BW			((1ULL << MAX_BW_بټونه) - 1)

خارجي u64 منزل_نسبت(u64 period, u64 runtime);

خارجي فراغ init_entity_runnable_اوسط(بنية sched_entity *se);
خارجي فراغ post_init_entity_util_avg(بنية task_struct *p);

#که_تعریف_شوی CONFIG_NO_HZ_ډک
خارجي bool sched_can_stop_tick(بنية rq *rq);
خارجي صحيح __init sched_tick_offload_init(فراغ);

/*
 * Tick may be needed by tasks in the runqueue depending on their policy and
 * requirements. If tick is needed, lets send the target an IPI to kick it out of
 * nohz mode if necessary.
 */
ساكن مضمن فراغ sched_update_tick_dependency(بنية rq *rq)
{
	صحيح cpu = cpu_د(rq);

	إذا (!tick_nohz_ډک_cpu(cpu))
		أرجع;

	إذا (sched_can_stop_tick(rq))
		tick_nohz_dep_clear_cpu(cpu, TICK_DEP_بټ_SCHED);
	وإلا
		tick_nohz_dep_سېټ_cpu(cpu, TICK_DEP_بټ_SCHED);
}
#وإلا /* !CONFIG_NO_HZ_FULL: */
ساكن مضمن صحيح sched_tick_offload_init(فراغ) { أرجع 0; }
ساكن مضمن فراغ sched_update_tick_dependency(بنية rq *rq) { }
#د_که_پای /* !CONFIG_NO_HZ_FULL */

ساكن مضمن فراغ جمع_کړه_nr_running(بنية rq *rq, بینښه شمېر)
{
	بینښه prev_nr = rq->nr_running;

	rq->nr_running = prev_nr + شمېر;
	إذا (trace_sched_update_nr_running_tp_enabled()) {
		call_trace_sched_update_nr_running(rq, شمېر);
	}

	إذا (prev_nr < 2 && rq->nr_running >= 2)
		سېټ_rd_overloaded(rq->rd, 1);

	sched_update_tick_dependency(rq);
}

ساكن مضمن فراغ sub_nr_running(بنية rq *rq, بینښه شمېر)
{
	rq->nr_running -= شمېر;
	إذا (trace_sched_update_nr_running_tp_enabled()) {
		call_trace_sched_update_nr_running(rq, -شمېر);
	}

	/* Check if we still need preemption */
	sched_update_tick_dependency(rq);
}

ساكن مضمن فراغ __block_task(بنية rq *rq, بنية task_struct *p)
{
	إذا (p->sched_contributes_منزل_بارول)
		rq->nr_uninterruptible++;

	إذا (p->دننه_iowait) {
		atomic_inc(&rq->nr_iowait);
		delayacct_blkio_پیل();
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
	activate_task(rq, p, کتار_ته_ورزیاتول_NOCLOCK);
	wakeup_preempt(rq, p, 0);
}

/*
 * attach_one_task() -- attaches the task returned from detach_one_task() to
 * its new rq.
 */
ساكن مضمن فراغ attach_one_task(بنية rq *rq, بنية task_struct *p)
{
	guard(rq_قلف)(rq);
	update_rq_clock(rq);
	attach_task(rq, p);
}

/*
 * __attach_tasks() - attaches a list of tasks (using se.group_node) to
 * the new rq
 */
ساكن مضمن فراغ __attach_tasks(بنية rq *rq, بنية list_سر *tasks)
{
	guard(rq_قلف)(rq);
	update_rq_clock(rq);

	طالما (!list_تشه(tasks)) {
		بنية task_struct *p;

		p = list_لومړی_ثبت(tasks, بنية task_struct, se.ډله_غوټه);
		list_del_init(&p->se.ډله_غوټه);

		attach_task(rq, p);
	}
}

#که_تعریف_شوی CONFIG_PREEMPT_RT
# define SCHED_NR_MIGRATE_BREAK 8
#وإلا
# define SCHED_NR_MIGRATE_BREAK 32
#د_که_پای

خارجي __read_mostly بینښه صحيح sysctl_sched_nr_migrate;
خارجي __read_mostly بینښه صحيح sysctl_sched_migration_لګښت;

خارجي بینښه صحيح sysctl_sched_بنسټ_ټوټه;

خارجي صحيح sysctl_resched_latency_warn_ms;
خارجي صحيح sysctl_resched_latency_warn_once;

خارجي بینښه صحيح sysctl_sched_tunable_scaling;

خارجي بینښه صحيح sysctl_numa_balancing_لټون_delay;
خارجي بینښه صحيح sysctl_numa_balancing_لټون_period_min;
خارجي بینښه صحيح sysctl_numa_balancing_لټون_period_max;
خارجي بینښه صحيح sysctl_numa_balancing_لټون_اندازه;
خارجي بینښه صحيح sysctl_numa_balancing_hot_پوله;

#که_تعریف_شوی CONFIG_SCHED_HRTICK

/*
 * Use hrtick when:
 *  - enabled by features
 *  - hrtimer is actually high res
 */
ساكن مضمن bool hrtick_enabled(بنية rq *rq)
{
	أرجع cpu_فعال(cpu_د(rq)) && hrtimer_highres_enabled();
}

ساكن مضمن bool hrtick_enabled_fair(بنية rq *rq)
{
	أرجع sched_feat(HRTICK) && hrtick_enabled(rq);
}

ساكن مضمن bool hrtick_enabled_dl(بنية rq *rq)
{
	أرجع sched_feat(HRTICK_DL) && hrtick_enabled(rq);
}

خارجي فراغ hrtick_پیل(بنية rq *rq, u64 delay);
ساكن مضمن bool hrtick_فعال(بنية rq *rq)
{
	أرجع hrtimer_فعال(&rq->hrtick_timer);
}

#وإلا /* !CONFIG_SCHED_HRTICK: */
ساكن مضمن bool hrtick_enabled_fair(بنية rq *rq) { أرجع false; }
ساكن مضمن bool hrtick_enabled_dl(بنية rq *rq) { أرجع false; }
ساكن مضمن bool hrtick_enabled(بنية rq *rq) { أرجع false; }
#د_که_پای /* !CONFIG_SCHED_HRTICK */

#که_نه_تعریف_شوی arch_scale_freq_tick
ساكن __always_inline فراغ arch_scale_freq_tick(فراغ) { }
#د_که_پای

#که_نه_تعریف_شوی arch_scale_freq_ظرفیت
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
بینښه اوږد arch_scale_freq_ظرفیت(صحيح cpu)
{
	أرجع SCHED_ظرفیت_SCALE;
}
#د_که_پای

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

#تعریف_کړه DEFINE_قلف_GUARD_2(name, type, قلف_2, _unlock, ...)				\
__DEFINE_UNLOCK_GUARD(name, type, _unlock, type *lock2; __VA_ARGS__)			\
ساكن مضمن class_##name##_t class_##name##_constructor(type *قلف, type *lock2)	\
	__no_context_analysis								\
{ class_##name##_t _t = { .قلف = قلف, .lock2 = lock2 }, *_T = &_t;			\
  قلف_2; أرجع _t; }
#تعریف_کړه DECLARE_قلف_GUARD_2_ATTRS(_name, قلف_2, _unlock1, _unlock2)			\
ساكن مضمن class_##_name##_t class_##_name##_constructor(قلف_3##_name##_t *_T1,	\
							    قلف_3##_name##_t *_T2) قلف_2; \
ساكن __always_inline فراغ __class_##_name##_cleanup_ctx1(class_##_name##_t **_T1)	\
	__no_context_analysis _unlock1 { }						\
ساكن __always_inline فراغ __class_##_name##_cleanup_ctx2(class_##_name##_t **_T2)	\
	__no_context_analysis _unlock2 { }
#تعریف_کړه WITH_قلف_GUARD_2_ATTRS(_name, _T1, _T2)					\
	class_##_name##_constructor(_T1, _T2),						\
	*__UNIQUE_ID(unlock1) __cleanup(__class_##_name##_cleanup_ctx1) = (فراغ *)(_T1),\
	*__UNIQUE_ID(unlock2) __cleanup(__class_##_name##_cleanup_ctx2) = (فراغ *)(_T2)

ساكن مضمن bool rq_ترتیب_less(بنية rq *rq1, بنية rq *rq2)
{
#که_تعریف_شوی CONFIG_SCHED_CORE
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
#د_که_پای /* CONFIG_SCHED_CORE */
	أرجع rq1->cpu < rq2->cpu;
}

خارجي فراغ double_rq_قلف(بنية rq *rq1, بنية rq *rq2)
	__acquires(__rq_lockp(rq1), __rq_lockp(rq2));

#که_تعریف_شوی CONFIG_PREEMPTION

/*
 * fair double_lock_balance: Safely acquires both rq->locks in a fair
 * way at the expense of forcing extra atomic operations in all
 * invocations.  This assures that the double_lock is acquired using the
 * same underlying policy as the spinlock_t on this architecture, which
 * reduces latency compared to the unfair variant below.  However, it
 * also adds more overhead and therefore may reduce throughput.
 */
ساكن مضمن صحيح double_قلف_balance_2(بنية rq *this_rq, بنية rq *busiest)
	__must_hold(__rq_lockp(this_rq))
	__acquires(__rq_lockp(busiest))
{
	raw_spin_rq_unlock(this_rq);
	double_rq_قلف(this_rq, busiest);

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
ساكن مضمن صحيح double_قلف_balance_2(بنية rq *this_rq, بنية rq *busiest)
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

	إذا (rq_ترتیب_less(this_rq, busiest)) {
		raw_spin_rq_قلف_nested(busiest, SINGLE_ژوروالی_NESTING);
		double_rq_clock_clear_update(this_rq, busiest);
		أرجع 0;
	}

	raw_spin_rq_unlock(this_rq);
	double_rq_قلف(this_rq, busiest);

	أرجع 1;
}

#د_که_پای /* !CONFIG_PREEMPTION */

/*
 * double_lock_balance - lock the busiest runqueue, this_rq is locked already.
 */
ساكن مضمن صحيح double_قلف_balance(بنية rq *this_rq, بنية rq *busiest)
	__must_hold(__rq_lockp(this_rq))
	__acquires(__rq_lockp(busiest))
{
	lockdep_assert_irqs_disabled();

	أرجع double_قلف_balance_2(this_rq, busiest);
}

ساكن مضمن فراغ double_unlock_balance(بنية rq *this_rq, بنية rq *busiest)
	__releases(__rq_lockp(busiest))
{
	إذا (__rq_lockp(this_rq) != __rq_lockp(busiest))
		raw_spin_rq_unlock(busiest);
	وإلا
		__release(__rq_lockp(busiest)); /* fake release */
	قلف_سېټ_subclass(&__rq_lockp(this_rq)->dep_map, 0, _RET_IP_);
}

ساكن مضمن فراغ double_قلف(spinlock_t *l1, spinlock_t *l2)
	__acquires(l1, l2)
{
	إذا (l1 > l2)
		بدلون(l1, l2);

	spin_قلف(l1);
	spin_قلف_nested(l2, SINGLE_ژوروالی_NESTING);
}

ساكن مضمن فراغ double_قلف_irq(spinlock_t *l1, spinlock_t *l2)
	__acquires(l1, l2)
{
	إذا (l1 > l2)
		بدلون(l1, l2);

	spin_قلف_irq(l1);
	spin_قلف_nested(l2, SINGLE_ژوروالی_NESTING);
}

ساكن مضمن فراغ double_raw_قلف(raw_spinlock_t *l1, raw_spinlock_t *l2)
	__acquires(l1, l2)
{
	إذا (l1 > l2)
		بدلون(l1, l2);

	raw_spin_قلف(l1);
	raw_spin_قلف_nested(l2, SINGLE_ژوروالی_NESTING);
}

ساكن مضمن فراغ double_raw_unlock(raw_spinlock_t *l1, raw_spinlock_t *l2)
	__releases(l1, l2)
{
	raw_spin_unlock(l1);
	raw_spin_unlock(l2);
}

DEFINE_قلف_GUARD_2(double_raw_spinlock, raw_spinlock_t,
		    double_raw_قلف(_T->قلف, _T->lock2),
		    double_raw_unlock(_T->قلف, _T->lock2))

DECLARE_قلف_GUARD_2_ATTRS(double_raw_spinlock,
			   __acquires(_T1, _T2),
			   __releases(*(raw_spinlock_t **)_T1),
			   __releases(*(raw_spinlock_t **)_T2));
#تعریف_کړه class_double_raw_spinlock_constructor(_T1, _T2) \
	WITH_قلف_GUARD_2_ATTRS(double_raw_spinlock, _T1, _T2)

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

خارجي فراغ سېټ_rq_online (بنية rq *rq);
خارجي فراغ سېټ_rq_offline(بنية rq *rq);

خارجي bool sched_smp_initialized;

DEFINE_قلف_GUARD_2(double_rq_قلف, بنية rq,
		    double_rq_قلف(_T->قلف, _T->lock2),
		    double_rq_unlock(_T->قلف, _T->lock2))

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

#که_تعریف_شوی CONFIG_NUMA_BALANCING
خارجي فراغ show_numa_stats(بنية task_struct *p, بنية seq_file *m);
خارجي فراغ
print_numa_stats(بنية seq_file *m, صحيح غوټه, بینښه اوږد tsf,
		 بینښه اوږد tpf, بینښه اوږد gsf, بینښه اوږد gpf);
#د_که_پای /* CONFIG_NUMA_BALANCING */

خارجي فراغ init_cfs_rq(بنية cfs_rq *cfs_rq);
خارجي فراغ init_rt_rq(بنية rt_rq *rt_rq);
خارجي فراغ init_dl_rq(بنية dl_rq *dl_rq);

خارجي فراغ cfs_bandwidth_usage_inc(فراغ);
خارجي فراغ cfs_bandwidth_usage_dec(فراغ);

#که_تعریف_شوی CONFIG_NO_HZ_COMMON

#تعریف_کړه NOHZ_BALANCE_KICK_بټ	0
#تعریف_کړه NOHZ_STATS_KICK_بټ	1
#تعریف_کړه NOHZ_NEWILB_KICK_بټ	2
#تعریف_کړه NOHZ_بل_KICK_بټ	3

/* Run sched_balance_domains() */
#تعریف_کړه NOHZ_BALANCE_KICK	بټ(NOHZ_BALANCE_KICK_بټ)
/* Update blocked load */
#تعریف_کړه NOHZ_STATS_KICK		بټ(NOHZ_STATS_KICK_بټ)
/* Update blocked load when entering idle */
#تعریف_کړه NOHZ_NEWILB_KICK	بټ(NOHZ_NEWILB_KICK_بټ)
/* Update nohz.next_balance */
#تعریف_کړه NOHZ_بل_KICK		بټ(NOHZ_بل_KICK_بټ)

#تعریف_کړه NOHZ_KICK_ماسک		(NOHZ_BALANCE_KICK | NOHZ_STATS_KICK | NOHZ_بل_KICK)

#تعریف_کړه nohz_flags(cpu)		(&cpu_rq(cpu)->nohz_flags)

خارجي فراغ nohz_balance_exit_idle(بنية rq *rq);
#وإلا /* !CONFIG_NO_HZ_COMMON: */
ساكن مضمن فراغ nohz_balance_exit_idle(بنية rq *rq) { }
#د_که_پای /* !CONFIG_NO_HZ_COMMON */

#که_تعریف_شوی CONFIG_NO_HZ_COMMON
خارجي فراغ nohz_پرلپسې_idle_balance(صحيح cpu);
#وإلا
ساكن مضمن فراغ nohz_پرلپسې_idle_balance(صحيح cpu) { }
#د_که_پای

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

#د_که_پای /* !(CONFIG_SCHED_CORE && CONFIG_SCHEDSTATS) */

#که_تعریف_شوی CONFIG_IRQ_وخت_ACCOUNTING

بنية irqtime {
	u64			ټول;
	u64			tick_delta;
	u64			irq_پیل_وخت;
	بنية u64_stats_sync	sync;
};

DECLARE_فی_CPU(بنية irqtime, cpu_irqtime);
DECLARE_STATIC_کلی_FALSE(sched_clock_irqtime);

ساكن مضمن صحيح irqtime_enabled(فراغ)
{
	أرجع static_branch_likely(&sched_clock_irqtime);
}

/*
 * Returns the irqtime minus the softirq time computed by ksoftirqd.
 * Otherwise ksoftirqd's sum_exec_runtime is subtracted its own runtime
 * and never move forward.
 */
ساكن مضمن u64 irq_وخت_read(صحيح cpu)
{
	بنية irqtime *irqtime = &فی_cpu(cpu_irqtime, cpu);
	بینښه صحيح seq;
	u64 ټول;

	افعل {
		seq = __u64_stats_fetch_begin(&irqtime->sync);
		ټول = irqtime->ټول;
	} طالما (__u64_stats_fetch_retry(&irqtime->sync, seq));

	أرجع ټول;
}

#وإلا /* !CONFIG_IRQ_TIME_ACCOUNTING: */

ساكن مضمن صحيح irqtime_enabled(فراغ)
{
	أرجع 0;
}

#د_که_پای /* !CONFIG_IRQ_TIME_ACCOUNTING */

#که_تعریف_شوی CONFIG_CPU_FREQ

DECLARE_فی_CPU(بنية update_util_معلومات __rcu *, cpufreq_update_util_معلومات);

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
ساكن مضمن فراغ cpufreq_update_util(بنية rq *rq, بینښه صحيح flags)
{
	بنية update_util_معلومات *معلومات;

	معلومات = rcu_dereference_sched(*فی_cpu_ptr(&cpufreq_update_util_معلومات,
						  cpu_د(rq)));
	إذا (معلومات)
		معلومات->func(معلومات, rq_clock(rq), flags);
}
#وإلا /* !CONFIG_CPU_FREQ: */
ساكن مضمن فراغ cpufreq_update_util(بنية rq *rq, بینښه صحيح flags) { }
#د_که_پای /* !CONFIG_CPU_FREQ */

#که_تعریف_شوی arch_scale_freq_ظرفیت
# ifndef arch_scale_freq_invariant
#  define arch_scale_freq_invariant()	true
# endif
#وإلا
# define arch_scale_freq_invariant()	false
#د_که_پای

بینښه اوږد effective_cpu_util(صحيح cpu, بینښه اوږد util_cfs,
				 بینښه اوږد *min,
				 بینښه اوږد *max);

بینښه اوږد sugov_effective_cpu_perf(صحيح cpu, بینښه اوږد actual,
				 بینښه اوږد min,
				 بینښه اوږد max);


/*
 * Verify the fitness of task @p to run on @cpu taking into account the
 * CPU original capacity and the runtime/deadline ratio of the task.
 *
 * The function will return true if the original capacity of @cpu is
 * greater than or equal to task's deadline density right shifted by
 * (BW_SHIFT - SCHED_CAPACITY_SHIFT) and false otherwise.
 */
ساكن مضمن bool dl_task_ځاییږي_ظرفیت(بنية task_struct *p, صحيح cpu)
{
	بینښه اوږد cap = arch_scale_cpu_ظرفیت(cpu);

	أرجع cap >= p->dl.dl_density >> (BW_ښويدنه - SCHED_ظرفیت_ښويدنه);
}

ساكن مضمن بینښه اوږد cpu_bw_dl(بنية rq *rq)
{
	أرجع (rq->dl.running_bw * SCHED_ظرفیت_SCALE) >> BW_ښويدنه;
}

ساكن مضمن بینښه اوږد cpu_util_dl(بنية rq *rq)
{
	أرجع READ_ONCE(rq->avg_dl.util_avg);
}


خارجي بینښه اوږد cpu_util_cfs(صحيح cpu);
خارجي بینښه اوږد cpu_util_cfs_boost(صحيح cpu);

ساكن مضمن بینښه اوږد cpu_util_rt(بنية rq *rq)
{
	أرجع READ_ONCE(rq->avg_rt.util_avg);
}

#که_تعریف_شوی CONFIG_UCLAMP_TASK

بینښه اوږد uclamp_eff_ارزښت(بنية task_struct *p, تعداد uclamp_id clamp_id);

/*
 * When uclamp is compiled in, the aggregation at rq level is 'turned off'
 * by default in the fast path and only gets turned on once userspace performs
 * an operation that requires it.
 *
 * Returns true if userspace opted-in to use uclamp and aggregation at rq level
 * hence is active.
 */
ساكن مضمن bool uclamp_is_کارول_شوی(فراغ)
{
	أرجع static_branch_likely(&sched_uclamp_کارول_شوی);
}

/*
 * Enabling static branches would get the cpus_read_lock(),
 * check whether uclamp_is_used before enable it to avoid always
 * calling cpus_read_lock(). Because we never disable this
 * static key once enable it.
 */
ساكن مضمن فراغ sched_uclamp_enable(فراغ)
{
	إذا (!uclamp_is_کارول_شوی())
		static_branch_enable(&sched_uclamp_کارول_شوی);
}

ساكن مضمن بینښه اوږد uclamp_rq_get(بنية rq *rq,
					  تعداد uclamp_id clamp_id)
{
	أرجع READ_ONCE(rq->uclamp[clamp_id].ارزښت);
}

ساكن مضمن فراغ uclamp_rq_سېټ(بنية rq *rq, تعداد uclamp_id clamp_id,
				 بینښه صحيح ارزښت)
{
	WRITE_ONCE(rq->uclamp[clamp_id].ارزښت, ارزښت);
}

ساكن مضمن bool uclamp_rq_is_idle(بنية rq *rq)
{
	أرجع rq->uclamp_flags & UCLAMP_FLAG_IDLE;
}

/* Is the rq being capped/throttled by uclamp_max? */
ساكن مضمن bool uclamp_rq_is_capped(بنية rq *rq)
{
	بینښه اوږد rq_util;
	بینښه اوږد max_util;

	إذا (!uclamp_is_کارول_شوی())
		أرجع false;

	rq_util = cpu_util_cfs(cpu_د(rq)) + cpu_util_rt(rq);
	max_util = READ_ONCE(rq->uclamp[UCLAMP_MAX].ارزښت);

	أرجع max_util != SCHED_ظرفیت_SCALE && rq_util >= max_util;
}

#تعریف_کړه for_each_clamp_id(clamp_id) \
	لكل ((clamp_id) = 0; (clamp_id) < UCLAMP_CNT; (clamp_id)++)

خارجي بینښه صحيح sysctl_sched_uclamp_util_min_rt_default;


ساكن مضمن بینښه صحيح uclamp_none(تعداد uclamp_id clamp_id)
{
	إذا (clamp_id == UCLAMP_MIN)
		أرجع 0;
	أرجع SCHED_ظرفیت_SCALE;
}

/* Integer rounded range for each bucket */
#تعریف_کړه UCLAMP_لوښی_DELTA DIV_پړاو_CLOSEST(SCHED_ظرفیت_SCALE, UCLAMP_BUCKETS)

ساكن مضمن بینښه صحيح uclamp_لوښی_id(بینښه صحيح clamp_ارزښت)
{
	أرجع min_t(بینښه صحيح, clamp_ارزښت / UCLAMP_لوښی_DELTA, UCLAMP_BUCKETS - 1);
}

ساكن مضمن فراغ
uclamp_se_سېټ(بنية uclamp_se *uc_se, بینښه صحيح ارزښت, bool user_defined)
{
	uc_se->ارزښت = ارزښت;
	uc_se->لوښی_id = uclamp_لوښی_id(ارزښت);
	uc_se->user_defined = user_defined;
}

#وإلا /* !CONFIG_UCLAMP_TASK: */

ساكن مضمن بینښه اوږد
uclamp_eff_ارزښت(بنية task_struct *p, تعداد uclamp_id clamp_id)
{
	إذا (clamp_id == UCLAMP_MIN)
		أرجع 0;

	أرجع SCHED_ظرفیت_SCALE;
}

ساكن مضمن bool uclamp_rq_is_capped(بنية rq *rq) { أرجع false; }

ساكن مضمن bool uclamp_is_کارول_شوی(فراغ)
{
	أرجع false;
}

ساكن مضمن فراغ sched_uclamp_enable(فراغ) {}

ساكن مضمن بینښه اوږد
uclamp_rq_get(بنية rq *rq, تعداد uclamp_id clamp_id)
{
	إذا (clamp_id == UCLAMP_MIN)
		أرجع 0;

	أرجع SCHED_ظرفیت_SCALE;
}

ساكن مضمن فراغ
uclamp_rq_سېټ(بنية rq *rq, تعداد uclamp_id clamp_id, بینښه صحيح ارزښت)
{
}

ساكن مضمن bool uclamp_rq_is_idle(بنية rq *rq)
{
	أرجع false;
}

#د_که_پای /* !CONFIG_UCLAMP_TASK */

#که_تعریف_شوی CONFIG_HAVE_SCHED_AVG_IRQ

ساكن مضمن بینښه اوږد cpu_util_irq(بنية rq *rq)
{
	أرجع READ_ONCE(rq->avg_irq.util_avg);
}

ساكن مضمن
بینښه اوږد scale_irq_ظرفیت(بینښه اوږد util, بینښه اوږد irq, بینښه اوږد max)
{
	util *= (max - irq);
	util /= max;

	أرجع util;

}

#وإلا /* !CONFIG_HAVE_SCHED_AVG_IRQ: */

ساكن مضمن بینښه اوږد cpu_util_irq(بنية rq *rq)
{
	أرجع 0;
}

ساكن مضمن
بینښه اوږد scale_irq_ظرفیت(بینښه اوږد util, بینښه اوږد irq, بینښه اوږد max)
{
	أرجع util;
}

#د_که_پای /* !CONFIG_HAVE_SCHED_AVG_IRQ */

خارجي فراغ __setparam_fair(بنية task_struct *p, ثابت بنية sched_attr *attr);

#إذا defined(CONFIG_ENERGY_MODEL) && defined(CONFIG_CPU_FREQ_GOV_SCHEDUTIL)

#تعریف_کړه perf_domain_پراختیا(pd) (منزل_cpumask(((pd)->em_pd->cpus)))

DECLARE_STATIC_کلی_FALSE(sched_energy_present);

ساكن مضمن bool sched_energy_enabled(فراغ)
{
	أرجع static_branch_unlikely(&sched_energy_present);
}

#وإلا /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL): */

#تعریف_کړه perf_domain_پراختیا(pd) عدم

ساكن مضمن bool sched_energy_enabled(فراغ) { أرجع false; }

#د_که_پای /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL) */

#که_تعریف_شوی CONFIG_MEMBARRIER

/*
 * The scheduler provides memory barriers required by membarrier between:
 * - prior user-space memory accesses and store to rq->membarrier_state,
 * - store to rq->membarrier_state and following user-space memory accesses.
 * In the same way it provides those guarantees around store to rq->curr.
 */
ساكن مضمن فراغ membarrier_switch_mm(بنية rq *rq,
					بنية mm_struct *prev_mm,
					بنية mm_struct *بل_mm)
{
	صحيح membarrier_state;

	إذا (prev_mm == بل_mm)
		أرجع;

	membarrier_state = atomic_read(&بل_mm->membarrier_state);
	إذا (READ_ONCE(rq->membarrier_state) == membarrier_state)
		أرجع;

	WRITE_ONCE(rq->membarrier_state, membarrier_state);
}

#وإلا /* !CONFIG_MEMBARRIER: */

ساكن مضمن فراغ membarrier_switch_mm(بنية rq *rq,
					بنية mm_struct *prev_mm,
					بنية mm_struct *بل_mm)
{
}

#د_که_پای /* !CONFIG_MEMBARRIER */

ساكن مضمن bool is_فی_cpu_kthread(بنية task_struct *p)
{
	إذا (!(p->flags & PF_KTHREAD))
		أرجع false;

	إذا (p->nr_cpus_allowed != 1)
		أرجع false;

	أرجع true;
}

خارجي فراغ swake_پورته_all_locked(بنية swait_کتار_سر *q);
خارجي فراغ __prepare_to_swait(بنية swait_کتار_سر *q, بنية swait_کتار *wait);

خارجي صحيح try_منزل_wake_پورته(بنية task_struct *tsk, بینښه صحيح state, صحيح wake_flags);

#که_تعریف_شوی CONFIG_PREEMPT_DYNAMIC
خارجي صحيح preempt_dynamic_ډېرپېښ;
خارجي صحيح sched_dynamic_ډېرپېښ(ثابت حرف *str);
خارجي فراغ sched_dynamic_update(صحيح ډېرپېښ);
#د_که_پای
خارجي ثابت حرف *preempt_modes[];

#که_تعریف_شوی CONFIG_SCHED_MM_CID

ساكن __always_inline bool cid_on_cpu(بینښه صحيح cid)
{
	أرجع cid & MM_CID_ONCPU;
}

ساكن __always_inline bool cid_دننه_transit(بینښه صحيح cid)
{
	أرجع cid & MM_CID_TRANSIT;
}

ساكن __always_inline بینښه صحيح cpu_cid_منزل_cid(بینښه صحيح cid)
{
	أرجع cid & ~MM_CID_ONCPU;
}

ساكن __always_inline بینښه صحيح cid_منزل_cpu_cid(بینښه صحيح cid)
{
	أرجع cid | MM_CID_ONCPU;
}

ساكن __always_inline بینښه صحيح cid_منزل_transit_cid(بینښه صحيح cid)
{
	أرجع cid | MM_CID_TRANSIT;
}

ساكن __always_inline بینښه صحيح cid_from_transit_cid(بینښه صحيح cid)
{
	أرجع cid & ~MM_CID_TRANSIT;
}

ساكن __always_inline bool cid_on_task(بینښه صحيح cid)
{
	/* True if none of the MM_CID_ONCPU, MM_CID_TRANSIT, MM_CID_UNSET bits is set */
	أرجع cid < MM_CID_TRANSIT;
}

ساكن __always_inline فراغ mm_drop_cid(بنية mm_struct *mm, بینښه صحيح cid)
{
	clear_بټ(cid, mm_cidmask(mm));
}

ساكن __always_inline فراغ mm_unset_cid_on_task(بنية task_struct *t)
{
	بینښه صحيح cid = t->mm_cid.cid;

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

ساكن مضمن بینښه صحيح __mm_get_cid(بنية mm_struct *mm, بینښه صحيح max_cids)
{
	بینښه صحيح cid = پیدا_کړه_لومړی_zero_بټ(mm_cidmask(mm), max_cids);

	إذا (cid >= max_cids)
		أرجع MM_CID_UNSET;
	إذا (test_and_سېټ_بټ(cid, mm_cidmask(mm)))
		أرجع MM_CID_UNSET;
	أرجع cid;
}

ساكن مضمن بینښه صحيح mm_get_cid(بنية mm_struct *mm)
{
	بینښه صحيح cid = __mm_get_cid(mm, READ_ONCE(mm->mm_cid.max_cids));

	طالما (cid == MM_CID_UNSET) {
		cpu_relax();
		cid = __mm_get_cid(mm, عدد_possible_cpus());
	}
	أرجع cid;
}

ساكن مضمن بینښه صحيح mm_cid_converge(بنية mm_struct *mm, بینښه صحيح orig_cid,
					   بینښه صحيح max_cids)
{
	بینښه صحيح new_cid, cid = cpu_cid_منزل_cid(orig_cid);

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

ساكن __always_inline فراغ mm_cid_update_task_cid(بنية task_struct *t, بینښه صحيح cid)
{
	إذا (t->mm_cid.cid != cid) {
		t->mm_cid.cid = cid;
		rseq_sched_سېټ_ids_بدل_شوی(t);
	}
}

ساكن __always_inline فراغ mm_cid_update_pcpu_cid(بنية mm_struct *mm, بینښه صحيح cid)
{
	__this_cpu_write(mm->mm_cid.pcpu->cid, cid);
}

ساكن __always_inline فراغ mm_cid_from_cpu(بنية task_struct *t, بینښه صحيح cpu_cid,
					    بینښه صحيح ډېرپېښ)
{
	بینښه صحيح max_cids, tcid = t->mm_cid.cid;
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
		إذا (ډېرپېښ & MM_CID_TRANSIT)
			cpu_cid = cpu_cid_منزل_cid(cpu_cid) | MM_CID_TRANSIT;
	}
	mm_cid_update_pcpu_cid(mm, cpu_cid);
	mm_cid_update_task_cid(t, cpu_cid);
}

ساكن __always_inline فراغ mm_cid_from_task(بنية task_struct *t, بینښه صحيح cpu_cid,
					     بینښه صحيح ډېرپېښ)
{
	بینښه صحيح max_cids, tcid = t->mm_cid.cid;
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
		tcid |= ډېرپېښ & MM_CID_TRANSIT;
	}
	mm_cid_update_pcpu_cid(mm, tcid);
	mm_cid_update_task_cid(t, tcid);
}

ساكن __always_inline فراغ mm_cid_schedin(بنية task_struct *بل)
{
	بنية mm_struct *mm = بل->mm;
	بینښه صحيح cpu_cid, ډېرپېښ;

	إذا (!بل->mm_cid.فعال)
		أرجع;

	cpu_cid = __this_cpu_read(mm->mm_cid.pcpu->cid);
	ډېرپېښ = READ_ONCE(mm->mm_cid.ډېرپېښ);
	إذا (likely(!cid_on_cpu(ډېرپېښ)))
		mm_cid_from_task(بل, cpu_cid, ډېرپېښ);
	وإلا
		mm_cid_from_cpu(بل, cpu_cid, ډېرپېښ);
}

ساكن __always_inline فراغ mm_cid_schedout(بنية task_struct *prev)
{
	بنية mm_struct *mm = prev->mm;
	بینښه صحيح ډېرپېښ, cid;

	/* During mode transitions CIDs are temporary and need to be dropped */
	إذا (likely(!cid_دننه_transit(prev->mm_cid.cid)))
		أرجع;

	ډېرپېښ = READ_ONCE(mm->mm_cid.ډېرپېښ);
	cid = cid_from_transit_cid(prev->mm_cid.cid);

	/*
	 * If transition mode is done, transfer ownership when the CID is
	 * within the convergence range to optimize the next schedule in.
	 */
	إذا (!cid_دننه_transit(ډېرپېښ) && cid < READ_ONCE(mm->mm_cid.max_cids)) {
		إذا (cid_on_cpu(ډېرپېښ))
			cid = cid_منزل_cpu_cid(cid);

		/* Update both so that the next schedule in goes into the fast path */
		mm_cid_update_pcpu_cid(mm, cid);
		prev->mm_cid.cid = cid;
	} وإلا {
		mm_drop_cid(mm, cid);
		prev->mm_cid.cid = MM_CID_UNSET;
	}
}

ساكن مضمن فراغ mm_cid_switch_منزل(بنية task_struct *prev, بنية task_struct *بل)
{
	mm_cid_schedout(prev);
	mm_cid_schedin(بل);
}

#وإلا /* !CONFIG_SCHED_MM_CID: */
ساكن مضمن فراغ mm_cid_switch_منزل(بنية task_struct *prev, بنية task_struct *بل) { }
#د_که_پای /* !CONFIG_SCHED_MM_CID */

#که_تعریف_شوی CONFIG_SCHED_CACHE
DECLARE_STATIC_کلی_FALSE(sched_cache_present);
DECLARE_STATIC_کلی_FALSE(sched_cache_فعال);
خارجي صحيح sysctl_sched_cache_user;
خارجي بینښه صحيح llc_aggr_tolerance;
خارجي بینښه صحيح llc_epoch_period;
خارجي بینښه صحيح llc_epoch_affinity_timeout;
خارجي بینښه صحيح llc_imb_pct;
خارجي بینښه صحيح llc_overaggr_pct;

ساكن مضمن bool sched_cache_enabled(فراغ)
{
	أرجع static_branch_unlikely(&sched_cache_فعال);
}

خارجي فراغ sched_cache_فعال_سېټ(فراغ);

#د_که_پای

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
	سېټ_task_cpu(task, dst_rq->cpu);
	activate_task(dst_rq, task, 0);
	wakeup_preempt(dst_rq, task, 0);
}

ساكن مضمن
bool task_is_pushable(بنية rq *rq, بنية task_struct *p, صحيح cpu)
{
	إذا (!task_on_cpu(rq, p) &&
	    cpumask_test_cpu(cpu, &p->cpus_ماسک))
		أرجع true;

	أرجع false;
}

#که_تعریف_شوی CONFIG_RT_MUTEXES

ساكن مضمن صحيح __rt_effective_prio(بنية task_struct *pi_task, صحيح prio)
{
	إذا (pi_task)
		prio = min(prio, pi_task->prio);

	أرجع prio;
}

ساكن مضمن صحيح rt_effective_prio(بنية task_struct *p, صحيح prio)
{
	بنية task_struct *pi_task = rt_mutex_get_سر_task(p);

	أرجع __rt_effective_prio(pi_task, prio);
}

#وإلا /* !CONFIG_RT_MUTEXES: */

ساكن مضمن صحيح rt_effective_prio(بنية task_struct *p, صحيح prio)
{
	أرجع prio;
}

#د_که_پای /* !CONFIG_RT_MUTEXES */

خارجي صحيح __sched_setscheduler(بنية task_struct *p, ثابت بنية sched_attr *attr, bool user, bool pi);
خارجي صحيح __sched_setaffinity(بنية task_struct *p, بنية affinity_context *ctx);
خارجي ثابت بنية sched_class *__setscheduler_class(صحيح policy, صحيح prio);
خارجي فراغ سېټ_بارول_وزن(بنية task_struct *p, bool update_بارول);
خارجي فراغ کتار_ته_ورزیاتول_task(بنية rq *rq, بنية task_struct *p, صحيح flags);
خارجي bool له_کتار_ایستل_task(بنية rq *rq, بنية task_struct *p, صحيح flags);

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

بنية sched_change_ctx *sched_change_begin(بنية task_struct *p, بینښه صحيح flags);
فراغ sched_change_پای_ټکی(بنية sched_change_ctx *ctx);

DEFINE_CLASS(sched_change, بنية sched_change_ctx *,
	     sched_change_پای_ټکی(_T),
	     sched_change_begin(p, flags),
	     بنية task_struct *p, بینښه صحيح flags)

DEFINE_CLASS_IS_UNCONDITIONAL(sched_change)

#ضمن "ext/ext.h"

#که_تعریف_شوی CONFIG_PREFERRED_CPU
فراغ sched_ټېل_current_non_preferred_cpu(بنية rq *rq);
#وإلا	/* !CONFIG_PREFERRED_CPU */
ساكن مضمن فراغ sched_ټېل_current_non_preferred_cpu(بنية rq *rq) { }
#د_که_پای

#د_که_پای /* _KERNEL_SCHED_SCHED_H */
