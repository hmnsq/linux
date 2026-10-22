/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Scheduler internal types and methods:
 */
#اگر_غیر_تعریف_شدہ _KERNEL_SCHED_SCHED_H
#تعریف_کرو _KERNEL_SCHED_SCHED_H

#شامل <linux/prandom.h>
#شامل <linux/sched/affinity.h>
#شامل <linux/sched/autogroup.h>
#شامل <linux/sched/cpufreq.h>
#شامل <linux/sched/deadline.h>
#شامل <linux/sched.h>
#شامل <linux/sched/loadavg.h>
#شامل <linux/sched/mm.h>
#شامل <linux/sched/rseq_api.h>
#شامل <linux/sched/signal.h>
#شامل <linux/sched/smt.h>
#شامل <linux/sched/stat.h>
#شامل <linux/sched/sysctl.h>
#شامل <linux/sched/task_flags.h>
#شامل <linux/sched/task.h>
#شامل <linux/sched/topology.h>
#شامل <linux/atomic.h>
#شامل <linux/bitmap.h>
#شامل <linux/bug.h>
#شامل <linux/capability.h>
#شامل <linux/cgroup_api.h>
#شامل <linux/cgroup.h>
#شامل <linux/context_tracking.h>
#شامل <linux/cpufreq.h>
#شامل <linux/cpumask_api.h>
#شامل <linux/cpuset.h>
#شامل <linux/ctype.h>
#شامل <linux/file.h>
#شامل <linux/fs_api.h>
#شامل <linux/hrtimer_api.h>
#شامل <linux/interrupt.h>
#شامل <linux/irq_work.h>
#شامل <linux/jiffies.h>
#شامل <linux/kref_api.h>
#شامل <linux/kthread.h>
#شامل <linux/ktime_api.h>
#شامل <linux/lockdep_api.h>
#شامل <linux/lockdep.h>
#شامل <linux/memblock.h>
#شامل <linux/memcontrol.h>
#شامل <linux/minmax.h>
#شامل <linux/mm.h>
#شامل <linux/module.h>
#شامل <linux/mutex_api.h>
#شامل <linux/plist.h>
#شامل <linux/poll.h>
#شامل <linux/proc_fs.h>
#شامل <linux/profile.h>
#شامل <linux/psi.h>
#شامل <linux/rcupdate.h>
#شامل <linux/seq_file.h>
#شامل <linux/seqlock.h>
#شامل <linux/softirq.h>
#شامل <linux/spinlock_api.h>
#شامل <linux/static_key.h>
#شامل <linux/stop_machine.h>
#شامل <linux/syscalls_api.h>
#شامل <linux/syscalls.h>
#شامل <linux/tick.h>
#شامل <linux/topology.h>
#شامل <linux/types.h>
#شامل <linux/u64_stats_sync_api.h>
#شامل <linux/uaccess.h>
#شامل <linux/vmstat.h>
#شامل <linux/wait_api.h>
#شامل <linux/wait_bit.h>
#شامل <linux/workqueue_api.h>
#شامل <linux/delayacct.h>
#شامل <linux/mmu_context.h>

#شامل <trace/events/power.h>
#شامل <trace/events/sched.h>

#شامل "../workqueue_internal.h"

ساخت rq;
ساخت cfs_rq;
ساخت rt_rq;
ساخت sched_گروہ;
ساخت cpuidle_state;

#اگر defined(CONFIG_PARAVIRT) && !defined(CONFIG_HAVE_PV_STEAL_CLOCK_GEN)
# include <asm/paravirt.h>
#اگر_ختم

#شامل <asm/barrier.h>

#شامل "cpupri.h"
#شامل "cpudeadline.h"

/* task_struct::on_rq states: */
#تعریف_کرو TASK_ON_RQ_QUEUED	1
#تعریف_کرو TASK_ON_RQ_MIGRATING	2

خارجی __read_mostly صحیح scheduler_running;

خارجی غیرعلامتی طویل calc_بھرو_update;
خارجی atomic_long_t calc_بھرو_tasks;

خارجی خالی calc_global_بھرو_tick(ساخت rq *this_rq);
خارجی طویل calc_بھرو_fold_فعال(ساخت rq *this_rq, طویل adjust);

خارجی خالی call_trace_sched_update_nr_running(ساخت rq *rq, صحیح گنتی);

خارجی صحیح sysctl_sched_rt_period;
خارجی صحیح sysctl_sched_rt_runtime;
خارجی صحیح sched_rr_timeslice;

/*
 * Asymmetric CPU capacity bits
 */
ساخت asym_cap_ڈیٹا {
	ساخت list_سر link;
	ساخت rcu_سر rcu;
	غیرعلامتی طویل استعداد;
	غیرعلامتی طویل cpus[];
};

خارجی ساخت list_سر asym_cap_list;

#تعریف_کرو cpu_استعداد_وسعت(asym_ڈیٹا) مقصد_cpumask((asym_ڈیٹا)->cpus)

/*
 * Helpers for converting nanosecond timing to jiffy resolution
 */
#تعریف_کرو NS_مقصد_JIFFIES(وقت)	((غیرعلامتی طویل)(وقت) / (NSEC_فی_SEC/HZ))

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
#اگر_تعریف_شدہ CONFIG_64_ثنائی_ہندسہ
# define NICE_0_بھرو_سرکاؤ	(SCHED_FIXEDPOINT_سرکاؤ + SCHED_FIXEDPOINT_سرکاؤ)
# define scale_بھرو(w)		((w) << SCHED_FIXEDPOINT_سرکاؤ)
# define scale_بھرو_نیچے_کو(w)					\
({								\
	غیرعلامتی طویل __w = (w);				\
								\
	اگر (__w)						\
		__w = max(2UL, __w >> SCHED_FIXEDPOINT_سرکاؤ);	\
	__w;							\
})
#ورنہ
# define NICE_0_بھرو_سرکاؤ	(SCHED_FIXEDPOINT_سرکاؤ)
# define scale_بھرو(w)		(w)
# define scale_بھرو_نیچے_کو(w)	(w)
#اگر_ختم

/*
 * Task weight (visible to users) and its load (invisible to users) have
 * independent resolution, but they should be well calibrated. We use
 * scale_load() and scale_load_down(w) to convert between them. The
 * following must be true:
 *
 *  scale_load(sched_prio_to_weight[NICE_TO_PRIO(0)-MAX_RT_PRIO]) == NICE_0_LOAD
 *
 */
#تعریف_کرو NICE_0_بھرو		(1L << NICE_0_بھرو_سرکاؤ)

/*
 * Single value that decides SCHED_DEADLINE internal math precision.
 * 10 -> just above 1us
 * 9  -> just above 0.5us
 */
#تعریف_کرو DL_SCALE		10

/*
 * Single value that denotes runtime == period, ie unlimited time.
 */
#تعریف_کرو RUNTIME_INF		((u64)~0ULL)

ساکن اندرونی صحیح idle_policy(صحیح policy)
{
	واپس policy == SCHED_IDLE;
}

ساکن اندرونی صحیح معمول_policy(صحیح policy)
{
#اگر_تعریف_شدہ CONFIG_SCHED_CLASS_EXT
	اگر (policy == SCHED_EXT)
		واپس true;
#اگر_ختم
	واپس policy == SCHED_معمول;
}

ساکن اندرونی صحیح fair_policy(صحیح policy)
{
	واپس معمول_policy(policy) || policy == SCHED_BATCH;
}

ساکن اندرونی صحیح rt_policy(صحیح policy)
{
	واپس policy == SCHED_FIFO || policy == SCHED_RR;
}

ساکن اندرونی صحیح dl_policy(صحیح policy)
{
	واپس policy == SCHED_DEADLINE;
}

ساکن اندرونی bool valid_policy(صحیح policy)
{
	واپس idle_policy(policy) || fair_policy(policy) ||
		rt_policy(policy) || dl_policy(policy);
}

ساکن اندرونی صحیح task_has_idle_policy(ساخت task_struct *p)
{
	واپس idle_policy(p->policy);
}

ساکن اندرونی صحیح task_has_rt_policy(ساخت task_struct *p)
{
	واپس rt_policy(p->policy);
}

ساکن اندرونی صحیح task_has_dl_policy(ساخت task_struct *p)
{
	واپس dl_policy(p->policy);
}

#تعریف_کرو cap_scale(v, s)		((v)*(s) >> SCHED_استعداد_سرکاؤ)

ساکن اندرونی خالی update_avg(u64 *avg, u64 sample)
{
	s64 diff = sample - *avg;

	*avg += diff / 8;
}

/*
 * Shifting a value by an exponent greater *or equal* to the size of said value
 * is UB; cap at size-1.
 */
#تعریف_کرو shr_bound(val, سرکاؤ)							\
	(val >> min_t(typeof(سرکاؤ), سرکاؤ, بٹ_فی_TYPE(typeof(val)) - 1))

/*
 * cgroup weight knobs should use the common MIN, DFL and MAX values which are
 * 1, 100 and 10000 respectively. While it loses a bit of range on both ends, it
 * maps pretty well onto the shares value used by scheduler and the round-trip
 * conversions preserve the original value over the entire range.
 */
ساکن اندرونی غیرعلامتی طویل sched_وزن_from_cgroup(غیرعلامتی طویل cgrp_وزن)
{
	واپس DIV_باری_CLOSEST_ULL(cgrp_وزن * 1024, CGROUP_وزن_DFL);
}

ساکن اندرونی غیرعلامتی طویل sched_وزن_مقصد_cgroup(غیرعلامتی طویل وزن)
{
	واپس clamp_t(غیرعلامتی طویل,
		       DIV_باری_CLOSEST_ULL(وزن * CGROUP_وزن_DFL, 1024),
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
#تعریف_کرو SCHED_FLAG_SUGOV	0x10000000

#تعریف_کرو SCHED_DL_FLAGS		(SCHED_FLAG_RECLAIM | SCHED_FLAG_DL_OVERRUN | SCHED_FLAG_SUGOV)

ساکن اندرونی bool dl_entity_is_special(ثابت ساخت sched_dl_entity *dl_se)
{
#اگر_تعریف_شدہ CONFIG_CPU_FREQ_GOV_SCHEDUTIL
	واپس unlikely(dl_se->flags & SCHED_FLAG_SUGOV);
#ورنہ
	واپس false;
#اگر_ختم
}

/*
 * Tells if entity @a should preempt entity @b.
 */
ساکن اندرونی bool dl_entity_preempt(ثابت ساخت sched_dl_entity *a,
				     ثابت ساخت sched_dl_entity *b)
{
	واپس dl_entity_is_special(a) ||
	       dl_وقت_before(a->deadline, b->deadline);
}

/*
 * This is the priority-queue data structure of the RT scheduling class:
 */
ساخت rt_prio_array {
	DECLARE_BITMAP(bitmap, MAX_RT_PRIO+1); /* include 1 bit for delimiter */
	ساخت list_سر قطار[MAX_RT_PRIO];
};

ساخت rt_bandwidth {
	/* nests inside the rq lock: */
	raw_spinlock_t		rt_runtime_تالا;
	ktime_t			rt_period;
	u64			rt_runtime;
	ساخت hrtimer		rt_period_timer;
	غیرعلامتی صحیح		rt_period_فعال;
};

ساکن اندرونی صحیح dl_bandwidth_enabled(خالی)
{
	واپس sysctl_sched_rt_runtime >= 0;
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
ساخت dl_bw {
	raw_spinlock_t		تالا;
	u64			bw;
	u64			کل_bw;
};

خارجی خالی init_dl_bw(ساخت dl_bw *dl_b);
خارجی صحیح  sched_dl_global_validate(خالی);
خارجی خالی sched_dl_do_global(خالی);
خارجی صحیح  sched_dl_حد_سے_تجاوز(ساخت task_struct *p, صحیح policy, ثابت ساخت sched_attr *attr);
خارجی خالی __setparam_dl(ساخت task_struct *p, ثابت ساخت sched_attr *attr);
خارجی خالی __getparam_dl(ساخت task_struct *p, ساخت sched_attr *attr, غیرعلامتی صحیح flags);
خارجی bool __checkparam_dl(ثابت ساخت sched_attr *attr);
خارجی bool dl_param_بدلا(ساخت task_struct *p, ثابت ساخت sched_attr *attr);
خارجی صحیح  dl_cpuset_cpumask_can_shrink(ثابت ساخت cpumask *cur, ثابت ساخت cpumask *trial);
خارجی صحیح  dl_bw_deactivate(صحیح cpu);
خارجی s64 dl_scaled_delta_exec(ساخت rq *rq, ساخت sched_dl_entity *dl_se, s64 delta_exec);
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
خارجی خالی dl_server_update_idle(ساخت sched_dl_entity *dl_se, s64 delta_exec);
خارجی خالی dl_server_update(ساخت sched_dl_entity *dl_se, s64 delta_exec);
خارجی خالی dl_server_آغاز(ساخت sched_dl_entity *dl_se);
خارجی خالی dl_server_stop(ساخت sched_dl_entity *dl_se);
خارجی خالی dl_server_init(ساخت sched_dl_entity *dl_se, ساخت rq *rq,
		    dl_server_pick_f pick_task);
خارجی خالی sched_init_dl_servers(خالی);

خارجی خالی fair_server_init(ساخت rq *rq);
خارجی خالی ext_server_init(ساخت rq *rq);
خارجی خالی __dl_server_attach_root(ساخت sched_dl_entity *dl_se, ساخت rq *rq);
خارجی صحیح dl_server_apply_params(ساخت sched_dl_entity *dl_se,
		    u64 runtime, u64 period, bool init);
خارجی صحیح dl_server_attach_bw(ساخت sched_dl_entity *dl_se);
خارجی خالی dl_server_detach_bw(ساخت sched_dl_entity *dl_se);
خارجی صحیح dl_server_تبادلہ_bw(ساخت sched_dl_entity *detach_se,
			     ساخت sched_dl_entity *attach_se);

ساکن اندرونی bool dl_server_فعال(ساخت sched_dl_entity *dl_se)
{
	واپس dl_se->dl_server_فعال;
}

#اگر_تعریف_شدہ CONFIG_CGROUP_SCHED

خارجی ساخت list_سر task_groups;

#اگر_تعریف_شدہ CONFIG_گروہ_SCHED_BANDWIDTH
خارجی ثابت u64 max_bw_quota_period_us;

/*
 * default period for group bandwidth.
 * default: 0.1s, units: microseconds
 */
ساکن اندرونی u64 default_bw_period_us(خالی)
{
	واپس 100000ULL;
}
#اگر_ختم /* CONFIG_GROUP_SCHED_BANDWIDTH */

ساخت cfs_bandwidth {
#اگر_تعریف_شدہ CONFIG_CFS_BANDWIDTH
	raw_spinlock_t		تالا;
	ktime_t			period;
	u64			quota;
	u64			runtime;
	u64			مدت;
	u64			runtime_snap;
	s64			درجہ_وار_quota;

	u8			idle;
	u8			period_فعال;
	u8			slack_started;
	ساخت hrtimer		period_timer;
	ساخت hrtimer		slack_timer;
	ساخت list_سر	throttled_cfs_rq;

	/* Statistics: */
	صحیح			nr_periods;
	صحیح			nr_throttled;
	صحیح			nr_مدت;
	u64			throttled_وقت;
	u64			مدت_وقت;
#اگر_ختم /* CONFIG_CFS_BANDWIDTH */
};

/* Task group related information */
ساخت task_گروہ {
	ساخت cgroup_subsys_state css;

#اگر_تعریف_شدہ CONFIG_گروہ_SCHED_وزن
	/* A positive value indicates that this is a SCHED_IDLE group. */
	صحیح			idle;
#اگر_ختم

#اگر_تعریف_شدہ CONFIG_FAIR_گروہ_SCHED
	/* runqueue "owned" by this group on each CPU */
	ساخت cfs_rq __percpu	*cfs_rq;
	غیرعلامتی طویل		shares;
	/*
	 * load_avg can be heavily contended at clock tick time, so put
	 * it in its own cache-line separated from the fields above which
	 * will also be accessed at each tick.
	 */
	atomic_long_t		بھرو_avg ____cacheline_aligned;
	atomic_long_t		runnable_avg;

#اگر_ختم /* CONFIG_FAIR_GROUP_SCHED */

#اگر_تعریف_شدہ CONFIG_RT_گروہ_SCHED
	ساخت sched_rt_entity	**rt_se;
	ساخت rt_rq		**rt_rq;

	ساخت rt_bandwidth	rt_bandwidth;
#اگر_ختم

	ساخت scx_task_گروہ	scx;

	ساخت rcu_سر		rcu;
	ساخت list_سر	list;

	ساخت task_گروہ	*والد;
	ساخت list_سر	siblings;
	ساخت list_سر	children;

#اگر_تعریف_شدہ CONFIG_SCHED_AUTOGROUP
	ساخت autogroup	*autogroup;
#اگر_ختم

	ساخت cfs_bandwidth	cfs_bandwidth;

#اگر_تعریف_شدہ CONFIG_UCLAMP_TASK_گروہ
	/* The two decimal precision [%] value requested from user-space */
	غیرعلامتی صحیح		uclamp_pct[UCLAMP_CNT];
	/* Clamp values requested for a task group */
	ساخت uclamp_se	uclamp_req[UCLAMP_CNT];
	/* Effective clamp values used for a task group */
	ساخت uclamp_se	uclamp[UCLAMP_CNT];
#اگر_ختم

};

#تعریف_کرو جڑ_TASK_گروہ_بھرو	NICE_0_بھرو

قسمتعریف صحیح (*tg_visitor)(ساخت task_گروہ *, خالی *);

خارجی صحیح walk_tg_درخت_from(ساخت task_گروہ *from,
			     tg_visitor نیچے_کو, tg_visitor اوپر, خالی *ڈیٹا);

/*
 * Iterate the full tree, calling @down when first entering a node and @up when
 * leaving it for the final time.
 *
 * Caller must hold rcu_lock or sufficient equivalent.
 */
ساکن اندرونی صحیح walk_tg_درخت(tg_visitor نیچے_کو, tg_visitor اوپر, خالی *ڈیٹا)
{
	واپس walk_tg_درخت_from(&جڑ_task_گروہ, نیچے_کو, اوپر, ڈیٹا);
}

ساکن اندرونی ساخت task_گروہ *css_tg(ساخت cgroup_subsys_state *css)
{
	واپس css ? container_کا(css, ساخت task_گروہ, css) : تہی;
}

خارجی صحیح tg_nop(ساخت task_گروہ *tg, خالی *ڈیٹا);

#اگر_تعریف_شدہ CONFIG_FAIR_گروہ_SCHED
خارجی خالی free_fair_sched_گروہ(ساخت task_گروہ *tg);
خارجی صحیح alloc_fair_sched_گروہ(ساخت task_گروہ *tg, ساخت task_گروہ *والد);
خارجی خالی online_fair_sched_گروہ(ساخت task_گروہ *tg);
خارجی خالی unregister_fair_sched_گروہ(ساخت task_گروہ *tg);
خارجی خالی __sched_cgroup_mode_update(صحیح منوال);
#ورنہ /* !CONFIG_FAIR_GROUP_SCHED: */
ساکن اندرونی خالی free_fair_sched_گروہ(ساخت task_گروہ *tg) { }
ساکن اندرونی صحیح alloc_fair_sched_گروہ(ساخت task_گروہ *tg, ساخت task_گروہ *والد)
{
       واپس 1;
}
ساکن اندرونی خالی online_fair_sched_گروہ(ساخت task_گروہ *tg) { }
ساکن اندرونی خالی unregister_fair_sched_گروہ(ساخت task_گروہ *tg) { }
#اگر_ختم /* !CONFIG_FAIR_GROUP_SCHED */

خارجی خالی init_tg_cfs_اندراج(ساخت task_گروہ *tg, ساخت cfs_rq *cfs_rq,
			ساخت sched_entity *se, صحیح cpu,
			ساخت sched_entity *والد);
خارجی خالی init_cfs_bandwidth(ساخت cfs_bandwidth *cfs_b, ساخت cfs_bandwidth *والد);

خارجی خالی __refill_cfs_bandwidth_runtime(ساخت cfs_bandwidth *cfs_b);
خارجی خالی آغاز_cfs_bandwidth(ساخت cfs_bandwidth *cfs_b);
خارجی خالی unthrottle_cfs_rq(ساخت cfs_rq *cfs_rq);
خارجی bool cfs_task_bw_constrained(ساخت task_struct *p);

خارجی خالی init_tg_rt_اندراج(ساخت task_گروہ *tg, ساخت rt_rq *rt_rq,
		ساخت sched_rt_entity *rt_se, صحیح cpu,
		ساخت sched_rt_entity *والد);
خارجی صحیح sched_گروہ_سیٹ_rt_runtime(ساخت task_گروہ *tg, طویل rt_runtime_us);
خارجی صحیح sched_گروہ_سیٹ_rt_period(ساخت task_گروہ *tg, u64 rt_period_us);
خارجی طویل sched_گروہ_rt_runtime(ساخت task_گروہ *tg);
خارجی طویل sched_گروہ_rt_period(ساخت task_گروہ *tg);
خارجی صحیح sched_rt_can_attach(ساخت task_گروہ *tg, ساخت task_struct *tsk);

خارجی ساخت task_گروہ *sched_create_گروہ(ساخت task_گروہ *والد);
خارجی خالی sched_online_گروہ(ساخت task_گروہ *tg,
			       ساخت task_گروہ *والد);
خارجی خالی sched_destroy_گروہ(ساخت task_گروہ *tg);
خارجی خالی sched_release_گروہ(ساخت task_گروہ *tg);

خارجی خالی sched_move_task(ساخت task_struct *tsk, bool for_autogroup);

#اگر_تعریف_شدہ CONFIG_FAIR_گروہ_SCHED
خارجی صحیح sched_گروہ_سیٹ_shares(ساخت task_گروہ *tg, غیرعلامتی طویل shares);

خارجی صحیح sched_گروہ_سیٹ_idle(ساخت task_گروہ *tg, طویل idle);

خارجی خالی سیٹ_task_rq_fair(ساخت sched_entity *se,
			     ساخت cfs_rq *پچھلا, ساخت cfs_rq *اگلا);
#ورنہ /* !CONFIG_FAIR_GROUP_SCHED: */
ساکن اندرونی صحیح sched_گروہ_سیٹ_shares(ساخت task_گروہ *tg, غیرعلامتی طویل shares) { واپس 0; }
ساکن اندرونی صحیح sched_گروہ_سیٹ_idle(ساخت task_گروہ *tg, طویل idle) { واپس 0; }
#اگر_ختم /* !CONFIG_FAIR_GROUP_SCHED */

#ورنہ /* !CONFIG_CGROUP_SCHED: */

ساخت cfs_bandwidth { };

ساکن اندرونی bool cfs_task_bw_constrained(ساخت task_struct *p) { واپس false; }

#اگر_ختم /* !CONFIG_CGROUP_SCHED */

/*
 * A weight of 0 or 1 can cause arithmetics problems.
 * A weight of a cfs_rq is the sum of weights of which entities
 * are queued on this cfs_rq, so a weight of a entity should not be
 * too large, so as the shares value of a task group.
 * (The default weight is 1024 - so there's no practical
 *  limitation from this.)
 */
#تعریف_کرو MIN_SHARES		(1UL <<  1)
#تعریف_کرو MAX_SHARES		(1UL << 18)

خارجی خالی unregister_rt_sched_گروہ(ساخت task_گروہ *tg);
خارجی خالی free_rt_sched_گروہ(ساخت task_گروہ *tg);
خارجی صحیح alloc_rt_sched_گروہ(ساخت task_گروہ *tg, ساخت task_گروہ *والد);

/*
 * u64_u32_load/u64_u32_store
 *
 * Use a copy of a u64 value to protect against data race. This is only
 * applicable for 32-bits architectures.
 */
#اگر_تعریف_شدہ CONFIG_64_ثنائی_ہندسہ
# define u64_u32_بھرو_copy(var, copy)		var
# define u64_u32_store_copy(var, copy, val)	(var = val)
#ورنہ
# define u64_u32_بھرو_copy(var, copy)					\
({									\
	u64 __val, __val_copy;						\
	کرو {								\
		__val_copy = copy;					\
		/*							\
		 * paired with u64_u32_store_copy(), ordering access	\
		 * to var and copy.					\
		 */							\
		smp_rmb();						\
		__val = var;						\
	} جب (__val != __val_copy);					\
	__val;								\
})
# define u64_u32_store_copy(var, copy, val)				\
کرو {									\
	typeof(val) __val = (val);					\
	var = __val;							\
	/*								\
	 * paired with u64_u32_load_copy(), ordering access to var and	\
	 * copy.							\
	 */								\
	smp_wmb();							\
	copy = __val;							\
} جب (0)
#اگر_ختم
# define u64_u32_بھرو(var)		u64_u32_بھرو_copy(var, var##_copy)
# define u64_u32_store(var, val)	u64_u32_store_copy(var, var##_copy, val)

ساخت balance_callback {
	ساخت balance_callback *اگلا;
	خالی (*func)(ساخت rq *rq);
};

/* Fair scheduling SCHED_{NORMAL,BATCH,IDLE} related fields in a runqueue: */
ساخت cfs_rq {
	ساخت بھرو_وزن	بھرو;
	غیرعلامتی صحیح		nr_queued;
	غیرعلامتی صحیح		h_nr_queued;		/* SCHED_{NORMAL,BATCH,IDLE} */
	غیرعلامتی صحیح		h_nr_runnable;		/* SCHED_{NORMAL,BATCH,IDLE} */
	غیرعلامتی صحیح		h_nr_idle;		/* SCHED_IDLE */

	s64			مجموع_w_vruntime;
	u64			مجموع_وزن;
	u64			zero_vruntime;
	غیرعلامتی صحیح		مجموع_سرکاؤ;

#اگر_تعریف_شدہ CONFIG_SCHED_CORE
	غیرعلامتی صحیح		forceidle_seq;
	u64			zero_vruntime_fi;
#اگر_ختم

	ساخت rb_جڑ_cached	tasks_timeline;

	/*
	 * 'curr' points to the currently running entity on this cfs_rq.
	 * It is set to NULL otherwise (i.e when none are currently running).
	 */
	ساخت sched_entity	*curr;
	ساخت sched_entity	*اگلا;

	/*
	 * CFS load tracking
	 */
	ساخت sched_entity	*h_curr;
	ساخت sched_avg	avg;
#اگر_غیر_تعریف_شدہ CONFIG_64_ثنائی_ہندسہ
	u64			آخری_update_وقت_copy;
#اگر_ختم
	ساخت {
		raw_spinlock_t	تالا ____cacheline_aligned;
		صحیح		nr;
		غیرعلامتی طویل	بھرو_avg;
		غیرعلامتی طویل	util_avg;
		غیرعلامتی طویل	runnable_avg;
	} removed;

#اگر_تعریف_شدہ CONFIG_FAIR_گروہ_SCHED
	u64			آخری_update_tg_بھرو_avg;
	غیرعلامتی طویل		tg_بھرو_avg_contrib;
	غیرعلامتی طویل		tg_runnable_avg_contrib;
	طویل			propagate;
	طویل			prop_runnable_مجموع;

	/*
	 *   h_load = weight * f(tg)
	 *
	 * Where f(tg) is the recursive weight fraction assigned to
	 * this group.
	 */
	غیرعلامتی طویل		h_بھرو;
	u64			آخری_h_بھرو_update;
	ساخت sched_entity	*h_بھرو_اگلا;

	ساخت rq		*rq;	/* CPU runqueue to which this cfs_rq is attached */

	/*
	 * leaf cfs_rqs are those that hold tasks (lowest schedulable entity in
	 * a hierarchy). Non-leaf lrqs hold other higher schedulable entities
	 * (like users, containers etc.)
	 *
	 * leaf_cfs_rq_list ties together list of leaf cfs_rq's in a CPU.
	 * This list is used during load balance.
	 */
	صحیح			on_list;
	ساخت list_سر	برگ_cfs_rq_list;
	ساخت task_گروہ	*tg;	/* Group that "owns" this runqueue */

	/* Locally cached copy of our task_group's idle value */
	صحیح			idle;

# ifdef CONFIG_CFS_BANDWIDTH
	صحیح			runtime_enabled;
	s64			runtime_بقایا;

	u64			throttled_pelt_idle;
#  ifndef CONFIG_64_ثنائی_ہندسہ
	u64                     throttled_pelt_idle_copy;
#  endif
	u64			throttled_clock;
	u64			throttled_clock_pelt;
	u64			throttled_clock_pelt_وقت;
	u64			throttled_clock_self;
	u64			throttled_clock_self_وقت;
	bool			throttled:1;
	bool			pelt_clock_throttled:1;
	صحیح			throttle_گنتی;
	ساخت list_سر	throttled_list;
	ساخت list_سر	throttled_csd_list;
	ساخت list_سر        throttled_limbo_list;
# endif /* CONFIG_CFS_BANDWIDTH */
#اگر_ختم /* CONFIG_FAIR_GROUP_SCHED */
};

#اگر_تعریف_شدہ CONFIG_SCHED_CLASS_EXT
/* scx_rq->flags, protected by the rq lock */
شمار scx_rq_flags {
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
	SCX_RQ_جڑ_IDLE_RENOTIFY	= 1 << 8, /* the root is owed update_idle() */

	SCX_RQ_اندر_WAKEUP	= 1 << 16,
	SCX_RQ_اندر_DISPATCH	= 1 << 17,
};

/* per-rq rescue execution state, see scx_rescue_timerfn() */
ساخت scx_rq_rescue {
	ساخت scx_dispatch_q	dsq;			/* stranded tasks awaiting rescue */
	s64			budget;			/* execution token bucket, ns */
	u64			clock;			/* last budget accrual timestamp */
	ساخت task_struct	*curr;			/* task being rescued, one at a time */
	s64			قاش;			/* curr's admitted slice */
	u64			exec_snap;		/* sum_exec_runtime at admission */
	ساخت timer_list	timer;			/* paces admission and escalation */
	u64			kill_at;		/* last ejection, init before any */
};

ساخت scx_rq {
	ساخت scx_dispatch_q	local_dsq;
#اگر_تعریف_شدہ CONFIG_EXT_SUB_SCHED
	ساخت scx_dispatch_q	reject_dsq;		/* staging for cap-rejected tasks */
	ساخت scx_rq_rescue	rescue;
#اگر_ختم
	ساخت list_سر	runnable_list;		/* runnable tasks on this rq */
	ساخت list_سر	ddsp_deferred_locals;	/* deferred ddsps from enq */
	/* both stashed across the activate_task() in move_remote_task_to_local_dsq() */
	u64			remote_activate_enq_flags;
	ساخت scx_sched	*remote_activate_sch;
	u32			nr_running;
	u32			cpuperf_ہدف;		/* [0, SCHED_CAPACITY_SCALE] */
	bool			اندر_select_cpu;
	bool			cpu_released;
	u32			flags;
	u32			nr_immed;		/* ENQ_IMMED tasks on local_dsq */
#اگر_تعریف_شدہ CONFIG_SCHED_CORE
	u32			تالا_drop_seq;	/* nr dispatch lock releases */
#اگر_ختم
	u64			clock;			/* current per-rq clock -- see scx_bpf_now() */
#اگر_تعریف_شدہ CONFIG_EXT_SUB_SCHED
	ساخت llist_سر	ecaps_مقصد_sync;		/* pending ecaps syncs */
	ساخت task_struct	*sub_dispatch_پچھلا;
#اگر_ختم
	cpumask_var_t		cpus_مقصد_sync;
	bool			kick_sync_pending;
	غیرعلامتی طویل		kick_sync;

	ساخت list_سر	sched_pcpus_مقصد_kick;	/* see kick_cpus_irq_workfn() */

	raw_spinlock_t		deferred_reenq_تالا;
	ساخت list_سر	deferred_reenq_locals;	/* scheds requesting reenq of local DSQ */
	ساخت list_سر	deferred_reenq_users;	/* user DSQs requesting reenq */
	ساخت balance_callback	deferred_bal_cb;
	ساخت balance_callback	kick_sync_bal_cb;
	ساخت irq_work		deferred_irq_work;
	ساخت irq_work		kick_cpus_irq_work;
};
#اگر_ختم /* CONFIG_SCHED_CLASS_EXT */

ساکن اندرونی صحیح rt_bandwidth_enabled(خالی)
{
	واپس sysctl_sched_rt_runtime >= 0;
}

/* RT IPI pull logic requires IRQ_WORK */
#اگر defined(CONFIG_IRQ_WORK) && defined(CONFIG_SMP)
# define HAVE_RT_ڈالو_IPI
#اگر_ختم

/* Real-Time classes' related field in a runqueue: */
ساخت rt_rq {
	ساخت rt_prio_array	فعال;
	غیرعلامتی صحیح		rt_nr_running;
	غیرعلامتی صحیح		rr_nr_running;
	ساخت {
		صحیح		curr; /* highest queued rt task prio */
		صحیح		اگلا; /* next highest */
	} highest_prio;
	bool			overloaded;
	ساخت plist_سر	pushable_tasks;

	صحیح			rt_queued;

#اگر_تعریف_شدہ CONFIG_RT_گروہ_SCHED
	صحیح			rt_throttled;
	u64			rt_وقت; /* consumed RT time, goes up in update_curr_rt */
	u64			rt_runtime; /* allotted RT time, "slice" from rt_bandwidth, RT sharing/balancing */
	/* Nests inside the rq lock: */
	raw_spinlock_t		rt_runtime_تالا;

	غیرعلامتی صحیح		rt_nr_boosted;

	ساخت rq		*rq; /* this is always top-level rq, cache? */
#اگر_ختم
#اگر_تعریف_شدہ CONFIG_CGROUP_SCHED
	ساخت task_گروہ	*tg; /* this tg has "this" rt_rq on given CPU for runnable entities */
#اگر_ختم
};

ساکن اندرونی bool rt_rq_is_runnable(ساخت rt_rq *rt_rq)
{
	واپس rt_rq->rt_queued && rt_rq->rt_nr_running;
}

/* Deadline class' related fields in a runqueue */
ساخت dl_rq {
	/* runqueue is an rbtree, ordered by deadline */
	ساخت rb_جڑ_cached	جڑ;

	غیرعلامتی صحیح		dl_nr_running;

	/*
	 * Deadline values of the currently executing and the
	 * earliest ready task on this rq. Caching these facilitates
	 * the decision whether or not a ready but not running task
	 * should migrate somewhere else.
	 */
	ساخت {
		u64		curr;
		u64		اگلا;
	} earliest_dl;

	bool			overloaded;

	ساخت sched_dl_entity	*curr;
	/*
	 * Tasks on this rq that can be pushed away. They are kept in
	 * an rb-tree, ordered by tasks' deadlines, with caching
	 * of the leftmost (earliest deadline) element.
	 */
	ساخت rb_جڑ_cached	pushable_dl_tasks_جڑ;

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

#اگر_تعریف_شدہ CONFIG_FAIR_گروہ_SCHED
/* Check whether a task group is root tg */
#تعریف_کرو is_جڑ_task_گروہ(tg) ((tg) == &جڑ_task_گروہ)
/* An entity is a task if it doesn't "own" a runqueue */
#تعریف_کرو entity_is_task(se)	(!se->my_q)

ساکن اندرونی خالی se_update_runnable(ساخت sched_entity *se)
{
	اگر (!entity_is_task(se))
		se->runnable_وزن = se->my_q->h_nr_runnable;
}

ساکن اندرونی طویل se_runnable(ساخت sched_entity *se)
{
	اگر (se->sched_delayed)
		واپس false;

	اگر (entity_is_task(se))
		واپس !!se->on_rq;
	ورنہ
		واپس se->runnable_وزن;
}

#ورنہ /* !CONFIG_FAIR_GROUP_SCHED: */

#تعریف_کرو entity_is_task(se)	1

ساکن اندرونی خالی se_update_runnable(ساخت sched_entity *se) { }

ساکن اندرونی طویل se_runnable(ساخت sched_entity *se)
{
	اگر (se->sched_delayed)
		واپس false;

	واپس !!se->on_rq;
}

#اگر_ختم /* !CONFIG_FAIR_GROUP_SCHED */

/*
 * XXX we want to get rid of these helpers and use the full load resolution.
 */
ساکن اندرونی طویل se_وزن(ساخت sched_entity *se)
{
	واپس scale_بھرو_نیچے_کو(se->بھرو.وزن);
}


ساکن اندرونی bool sched_asym_prefer(صحیح a, صحیح b)
{
	واپس arch_asym_cpu_ترجیح(a) > arch_asym_cpu_ترجیح(b);
}

ساخت perf_domain {
	ساخت em_perf_domain *em_pd;
	ساخت perf_domain *اگلا;
	ساخت rcu_سر rcu;
};

/*
 * We add the notion of a root-domain which will be used to define per-domain
 * variables. Each exclusive cpuset essentially defines an island domain by
 * fully partitioning the member CPUs from any other cpuset. Whenever a new
 * exclusive cpuset is created, we also create and attach a new root-domain
 * object.
 *
 */
ساخت جڑ_domain {
	atomic_t		refcount;
	atomic_t		rto_گنتی;
	ساخت rcu_سر		rcu;
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
	atomic_t		dlo_گنتی;
	ساخت dl_bw		dl_bw;
	ساخت cpudl		cpudl;

	/*
	 * Indicate whether a root_domain's dl_bw has been checked or
	 * updated. It's monotonously increasing value.
	 *
	 * Also, some corner cases, like 'wrap around' is dangerous, but given
	 * that u64 is 'big enough'. So that shouldn't be a concern.
	 */
	u64 دورہ_cookie;

#اگر_تعریف_شدہ HAVE_RT_ڈالو_IPI
	/*
	 * For IPI pull requests, loop across the rto_mask.
	 */
	ساخت irq_work		rto_ڈالو_work;
	raw_spinlock_t		rto_تالا;
	/* These are only updated and read within rto_lock */
	صحیح			rto_loop;
	صحیح			rto_cpu;
	/* These atomics are updated outside of a lock */
	atomic_t		rto_loop_اگلا;
	atomic_t		rto_loop_آغاز;
#اگر_ختم /* HAVE_RT_PUSH_IPI */
	/*
	 * The "RT overload" flag: it gets set if a CPU has more than
	 * one runnable RT task.
	 */
	cpumask_var_t		rto_نقاب;
	ساخت cpupri		cpupri;

	/*
	 * NULL-terminated list of performance domains intersecting with the
	 * CPUs of the rd. Protected by RCU.
	 */
	ساخت perf_domain __rcu *pd;
};

خارجی خالی init_defrootdomain(خالی);
خارجی صحیح sched_init_domains(ثابت ساخت cpumask *cpu_map);
خارجی خالی rq_attach_جڑ(ساخت rq *rq, ساخت جڑ_domain *rd);
خارجی خالی sched_get_rd(ساخت جڑ_domain *rd);
خارجی خالی sched_put_rd(ساخت جڑ_domain *rd);

ساکن اندرونی صحیح get_rd_overloaded(ساخت جڑ_domain *rd)
{
	واپس READ_ONCE(rd->overloaded);
}

ساکن اندرونی خالی سیٹ_rd_overloaded(ساخت جڑ_domain *rd, صحیح حالت_2)
{
	اگر (get_rd_overloaded(rd) != حالت_2)
		WRITE_ONCE(rd->overloaded, حالت_2);
}

#اگر_تعریف_شدہ HAVE_RT_ڈالو_IPI
خارجی خالی rto_ڈالو_irq_work_func(ساخت irq_work *work);
#اگر_ختم

#اگر_تعریف_شدہ CONFIG_UCLAMP_TASK
/*
 * struct uclamp_bucket - Utilization clamp bucket
 * @value: utilization clamp value for tasks on this clamp bucket
 * @tasks: number of RUNNABLE tasks on this clamp bucket
 *
 * Keep track of how many tasks are RUNNABLE for a given utilization
 * clamp value.
 */
ساخت uclamp_بالٹی {
	غیرعلامتی طویل قدر : بٹ_فی(SCHED_استعداد_SCALE);
	غیرعلامتی طویل tasks : بٹ_فی_LONG - بٹ_فی(SCHED_استعداد_SCALE);
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
ساخت uclamp_rq {
	غیرعلامتی صحیح قدر;
	ساخت uclamp_بالٹی بالٹی[UCLAMP_BUCKETS];
};

DECLARE_STATIC_کلید_FALSE(sched_uclamp_مستعمل);
#اگر_ختم /* CONFIG_UCLAMP_TASK */

/*
 * This is the main, per-CPU runqueue data structure.
 *
 * Locking rule: those places that want to lock multiple runqueues
 * (such as the load balancing or the thread migration code), lock
 * acquire operations must be ordered by ascending &runqueue.
 */
ساخت rq {
	/*
	 * The following members are loaded together, without holding the
	 * rq->lock, in an extremely hot loop in update_sg_lb_stats()
	 * (called from pick_next_task()). To reduce cache pollution from
	 * this operation, they are placed together on this dedicated cache
	 * line. Even though some of them are frequently modified, they are
	 * loaded much more frequently than they are stored.
	 */
	غیرعلامتی صحیح		nr_running;
#اگر_تعریف_شدہ CONFIG_NUMA_BALANCING
	غیرعلامتی صحیح		nr_numa_running;
	غیرعلامتی صحیح		nr_preferred_running;
#اگر_ختم
	غیرعلامتی صحیح		ttwu_pending;
	غیرعلامتی طویل		cpu_استعداد;
#اگر_تعریف_شدہ CONFIG_SCHED_PROXY_EXEC
	ساخت task_struct __rcu	*donor;  /* Scheduling context */
	ساخت task_struct __rcu	*curr;   /* Execution context */
#ورنہ
	اتحاد {
		ساخت task_struct __rcu *donor; /* Scheduler context */
		ساخت task_struct __rcu *curr;  /* Execution context */
	};
#اگر_ختم
	ساخت task_struct	*idle;
	/* padding left here deliberately */

	/*
	 * The next cacheline holds the (hot) runqueue lock, as well as
	 * some other less performance-critical fields.
	 */
	u64			nr_switches	____cacheline_aligned;

	/* runqueue lock: */
	raw_spinlock_t		__lock;

#اگر_تعریف_شدہ CONFIG_NO_HZ_COMMON
	غیرعلامتی صحیح		nohz_tick_stopped;
	atomic_t		nohz_flags;
	غیرعلامتی صحیح		has_blocked_بھرو;
	غیرعلامتی طویل		آخری_blocked_بھرو_update_tick;
	call_single_ڈیٹا_t	nohz_csd;
#اگر_ختم /* CONFIG_NO_HZ_COMMON */

#اگر_تعریف_شدہ CONFIG_UCLAMP_TASK
	/* Utilization clamp values based on CPU's RUNNABLE tasks */
	ساخت uclamp_rq	uclamp[UCLAMP_CNT] ____cacheline_aligned;
	غیرعلامتی صحیح		uclamp_flags;
#تعریف_کرو UCLAMP_FLAG_IDLE 0x01
#اگر_ختم

	ساخت cfs_rq		cfs;
	ساخت rt_rq		rt;
	ساخت dl_rq		dl;
#اگر_تعریف_شدہ CONFIG_SCHED_CLASS_EXT
	ساخت scx_rq		scx;
	ساخت sched_dl_entity	ext_server;
#اگر_ختم
#اگر_تعریف_شدہ CONFIG_SCHED_CACHE
	raw_spinlock_t		cpu_epoch_تالا ____cacheline_aligned;
	u64			cpu_runtime;
	غیرعلامتی طویل		cpu_epoch;
	غیرعلامتی طویل		cpu_epoch_اگلا;
#اگر_ختم

	ساخت sched_dl_entity	fair_server;

#اگر_تعریف_شدہ CONFIG_FAIR_گروہ_SCHED
	/* list of leaf cfs_rq on this CPU: */
	ساخت list_سر	برگ_cfs_rq_list;
	ساخت list_سر	*tmp_alone_branch;
#اگر_ختم /* CONFIG_FAIR_GROUP_SCHED */

#اگر_تعریف_شدہ CONFIG_NUMA_BALANCING
	غیرعلامتی صحیح		numa_migrate_on;
#اگر_ختم

#اگر_تعریف_شدہ CONFIG_SCHED_CACHE
	غیرعلامتی صحیح		nr_pref_llc_running;
	غیرعلامتی صحیح		nr_llc_running;
#اگر_ختم

	/*
	 * This is part of a global counter where only the total sum
	 * over all CPUs matters. A task can increase this counter on
	 * one CPU and if it got migrated afterwards it may decrease
	 * it on another CPU. Always updated under the runqueue lock:
	 */
	غیرعلامتی طویل		nr_uninterruptible;

	ساخت sched_dl_entity	*dl_server;
	ساخت task_struct	*stop;
	ثابت ساخت sched_class *اگلا_class;
	غیرعلامتی طویل		اگلا_balance;
	ساخت mm_struct	*پچھلا_mm;

	/*
	 * The following fields of clock data are frequently referenced
	 * and updated together, and should go on their own cache line.
	 */
	u64			clock_task ____cacheline_aligned;
	u64			clock_pelt;
	u64			clock;
	غیرعلامتی طویل		گمشدہ_idle_وقت;
	غیرعلامتی صحیح		clock_update_flags;
	u64			clock_pelt_idle;
	u64			clock_idle;

#اگر_غیر_تعریف_شدہ CONFIG_64_ثنائی_ہندسہ
	u64			clock_pelt_idle_copy;
	u64			clock_idle_copy;
#اگر_ختم

	u64 آخری_نظر_آیا_need_resched_ns;
	صحیح ticks_without_resched;

#اگر_تعریف_شدہ CONFIG_MEMBARRIER
	صحیح membarrier_state;
#اگر_ختم

	ساخت جڑ_domain		*rd;
	ساخت sched_domain __rcu	*sd;

	ساخت balance_callback *balance_callback;

	غیرعلامتی حرف		nohz_idle_balance;
	غیرعلامتی حرف		idle_balance;

	غیرعلامتی طویل		misfit_task_بھرو;

	/* For active balancing */
	صحیح			فعال_balance;
	صحیح			ڈالو_cpu;
	ساخت cpu_stop_work	فعال_balance_work;

	/* CPU of this runqueue: */
	صحیح			cpu;
	صحیح			online;

	ساخت list_سر cfs_tasks;

	ساخت sched_avg	avg_rt;
	ساخت sched_avg	avg_dl;
#اگر_تعریف_شدہ CONFIG_HAVE_SCHED_AVG_IRQ
	ساخت sched_avg	avg_irq;
#اگر_ختم
#اگر_تعریف_شدہ CONFIG_SCHED_HW_PRESSURE
	ساخت sched_avg	avg_hw;
#اگر_ختم
	u64			idle_stamp;
	u64			avg_idle;

	/* This is used to determine avg_idle's max value */
	u64			max_idle_balance_لاگت;

#اگر_تعریف_شدہ CONFIG_HOTPLUG_CPU
	ساخت rcuwait		hotplug_wait;
#اگر_ختم

#اگر_تعریف_شدہ CONFIG_IRQ_وقت_ACCOUNTING
	u64			پچھلا_irq_وقت;
	u64			psi_irq_وقت;
#اگر_ختم
#اگر_تعریف_شدہ CONFIG_PARAVIRT
	u64			پچھلا_steal_وقت;
#اگر_ختم
#اگر_تعریف_شدہ CONFIG_PARAVIRT_وقت_ACCOUNTING
	u64			پچھلا_steal_وقت_rq;
#اگر_ختم
#اگر_تعریف_شدہ CONFIG_PREFERRED_CPU
	bool			npc_ڈالو_work_pending;
#اگر_ختم

	/* calc_load related fields */
	غیرعلامتی طویل		calc_بھرو_update;
	طویل			calc_بھرو_فعال;

#اگر_تعریف_شدہ CONFIG_SCHED_HRTICK
	call_single_ڈیٹا_t	hrtick_csd;
	ساخت hrtimer		hrtick_timer;
	ktime_t			hrtick_وقت;
	ktime_t			hrtick_delay;
	غیرعلامتی صحیح		hrtick_sched;
#اگر_ختم

#اگر_تعریف_شدہ CONFIG_SCHEDSTATS
	/* latency stats */
	ساخت sched_info	rq_sched_info;
	غیرعلامتی طویل طویل	rq_cpu_وقت;

	/* sys_sched_yield() stats */
	غیرعلامتی صحیح		yld_گنتی;

	/* schedule() stats */
	غیرعلامتی صحیح		sched_گنتی;
	غیرعلامتی صحیح		sched_goidle;

	/* try_to_wake_up() stats */
	غیرعلامتی صحیح		ttwu_گنتی;
	غیرعلامتی صحیح		ttwu_local;
#اگر_ختم

#اگر_تعریف_شدہ CONFIG_CPU_IDLE
	/* Must be inspected within a RCU lock section */
	ساخت cpuidle_state	*idle_state;
#اگر_ختم

	غیرعلامتی صحیح		nr_pinned;
	غیرعلامتی صحیح		ڈالو_busy;
	ساخت cpu_stop_work	ڈالو_work;

#اگر_تعریف_شدہ CONFIG_SCHED_CORE
	/* per rq */
	ساخت rq		*core;
	ساخت task_struct	*core_pick;
	ساخت sched_dl_entity	*core_dl_server;
	غیرعلامتی صحیح		core_enabled;
	ساخت rb_جڑ		core_درخت;

	/* shared state -- careful with sched_core_cpu_deactivate() */
	غیرعلامتی صحیح		core_task_seq;
	غیرعلامتی صحیح		core_pick_seq;
	غیرعلامتی طویل		core_cookie;
	غیرعلامتی صحیح		core_forceidle_گنتی;
	غیرعلامتی صحیح		core_forceidle_seq;
	غیرعلامتی صحیح		core_forceidle_occupation;
	u64			core_forceidle_آغاز;
	غیرعلامتی صحیح		core_pick_اندر_flight;
#اگر_ختم /* CONFIG_SCHED_CORE */

	/* Scratch cpumask to be temporarily used under rq_lock */
	cpumask_var_t		scratch_نقاب;

#اگر_تعریف_شدہ CONFIG_CFS_BANDWIDTH
	call_single_ڈیٹا_t	cfsb_csd;
	ساخت list_سر	cfsb_csd_list;
#اگر_ختم

	atomic_t		nr_iowait;
} __no_randomize_layout;

#اگر_تعریف_شدہ CONFIG_FAIR_گروہ_SCHED

/* CPU runqueue to which this cfs_rq is attached */
ساکن اندرونی ساخت rq *rq_کا(ساخت cfs_rq *cfs_rq)
{
	واپس cfs_rq->rq;
}

#ورنہ /* !CONFIG_FAIR_GROUP_SCHED: */

ساکن اندرونی ساخت rq *rq_کا(ساخت cfs_rq *cfs_rq)
{
	واپس container_کا(cfs_rq, ساخت rq, cfs);
}
#اگر_ختم /* !CONFIG_FAIR_GROUP_SCHED */

ساکن اندرونی صحیح cpu_کا(ساخت rq *rq)
{
	واپس rq->cpu;
}

#تعریف_کرو MDF_ڈالو		0x01

ساکن اندرونی bool is_migration_disabled(ساخت task_struct *p)
{
	واپس p->migration_disabled;
}

DECLARE_فی_CPU_SHARED_ALIGNED(ساخت rq, runqueues);
DECLARE_فی_CPU(ساخت rnd_state, sched_rnd_state);

ساکن اندرونی u32 sched_rng(خالی)
{
	واپس prandom_u32_state(this_cpu_ptr(&sched_rnd_state));
}

ساکن __always_inline ساخت rq *__this_rq(خالی)
{
	واپس this_cpu_ptr(&runqueues);
}

#تعریف_کرو cpu_rq(cpu)		(&فی_cpu(runqueues, (cpu)))
#تعریف_کرو this_rq()		__this_rq()
#تعریف_کرو task_rq(p)		cpu_rq(task_cpu(p))
#تعریف_کرو cpu_curr(cpu)		(cpu_rq(cpu)->curr)
#تعریف_کرو raw_rq()		raw_cpu_ptr(&runqueues)

ساکن اندرونی bool idle_rq(ساخت rq *rq)
{
	واپس rq->curr == rq->idle && !rq->nr_running && !rq->ttwu_pending;
}

/**
 * available_idle_cpu - is a given CPU idle for enqueuing work.
 * @cpu: the CPU in question.
 *
 * Return: 1 if the CPU is currently idle. 0 otherwise.
 */
ساکن اندرونی bool دستیاب_idle_cpu(صحیح cpu)
{
	اگر (!idle_rq(cpu_rq(cpu)))
		واپس 0;

	اگر (vcpu_is_preempted(cpu))
		واپس 0;

	واپس 1;
}

#اگر_تعریف_شدہ CONFIG_SCHED_PROXY_EXEC
ساکن اندرونی خالی rq_سیٹ_donor(ساخت rq *rq, ساخت task_struct *t)
{
	rcu_تفویض_اشارہ_گر(rq->donor, t);
}
#ورنہ
ساکن اندرونی خالی rq_سیٹ_donor(ساخت rq *rq, ساخت task_struct *t)
{
	/* Do nothing */
}
#اگر_ختم

#اگر_تعریف_شدہ CONFIG_SCHED_CORE
ساکن اندرونی ساخت cpumask *sched_گروہ_وسعت(ساخت sched_گروہ *sg);

DECLARE_STATIC_کلید_FALSE(__sched_core_enabled);

ساکن اندرونی bool sched_core_enabled(ساخت rq *rq)
{
	واپس static_branch_unlikely(&__sched_core_enabled) && rq->core_enabled;
}

ساکن اندرونی bool sched_core_disabled(خالی)
{
	واپس !static_branch_unlikely(&__sched_core_enabled);
}

/*
 * Be careful with this function; not for general use. The return value isn't
 * stable unless you actually hold a relevant rq->__lock.
 */
ساکن اندرونی raw_spinlock_t *rq_lockp(ساخت rq *rq)
{
	اگر (sched_core_enabled(rq))
		واپس &rq->core->__lock;

	واپس &rq->__lock;
}

ساکن اندرونی raw_spinlock_t *__rq_lockp(ساخت rq *rq)
	__returns_ctx_lock(rq_lockp(rq)) /* alias them */
{
	اگر (rq->core_enabled)
		واپس &rq->core->__lock;

	واپس &rq->__lock;
}

خارجی bool
cfs_prio_less(ثابت ساخت task_struct *a, ثابت ساخت task_struct *b, bool fi);

خارجی خالی task_vruntime_update(ساخت rq *rq, ساخت task_struct *p, bool اندر_fi);

/*
 * Helpers to check if the CPU's core cookie matches with the task's cookie
 * when core scheduling is enabled.
 * A special case is that the task's cookie always matches with CPU's core
 * cookie if the CPU is in an idle core.
 */
ساکن اندرونی bool sched_cpu_cookie_مطابقت(ساخت rq *rq, ساخت task_struct *p)
{
	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	اگر (!sched_core_enabled(rq))
		واپس true;

	واپس rq->core->core_cookie == p->core_cookie;
}

ساکن اندرونی bool sched_core_cookie_مطابقت(ساخت rq *rq, ساخت task_struct *p)
{
	bool idle_core = true;
	صحیح cpu;

	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	اگر (!sched_core_enabled(rq))
		واپس true;

	اگر (rq->core->core_cookie == p->core_cookie)
		واپس true;

	for_each_cpu(cpu, cpu_smt_نقاب(cpu_کا(rq))) {
		اگر (!دستیاب_idle_cpu(cpu)) {
			idle_core = false;
			توڑو;
		}
	}

	/*
	 * A CPU in an idle core is always the best choice for tasks with
	 * cookies.
	 */
	واپس idle_core;
}

ساکن اندرونی bool sched_گروہ_cookie_مطابقت(ساخت rq *rq,
					    ساخت task_struct *p,
					    ساخت sched_گروہ *گروہ)
{
	صحیح cpu;

	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	اگر (!sched_core_enabled(rq))
		واپس true;

	for_each_cpu_and(cpu, sched_گروہ_وسعت(گروہ), p->cpus_ptr) {
		اگر (sched_core_cookie_مطابقت(cpu_rq(cpu), p))
			واپس true;
	}
	واپس false;
}

ساکن اندرونی bool sched_core_enqueued(ساخت task_struct *p)
{
	واپس !RB_خالی_خانے_گرہ(&p->core_گرہ);
}

خارجی خالی sched_core_قطار_میں_ڈالو(ساخت rq *rq, ساخت task_struct *p);
خارجی خالی sched_core_قطار_سے_نکالو(ساخت rq *rq, ساخت task_struct *p, صحیح flags);

خارجی خالی sched_core_get(خالی);
خارجی خالی sched_core_put(خالی);

ساکن اندرونی bool task_has_sched_core(ساخت task_struct *p)
{
	اگر (sched_core_disabled())
		واپس false;

	واپس !!p->core_cookie;
}

#ورنہ /* !CONFIG_SCHED_CORE: */

ساکن اندرونی bool sched_core_enabled(ساخت rq *rq)
{
	واپس false;
}

ساکن اندرونی bool sched_core_disabled(خالی)
{
	واپس true;
}

ساکن اندرونی raw_spinlock_t *rq_lockp(ساخت rq *rq)
{
	واپس &rq->__lock;
}

ساکن اندرونی raw_spinlock_t *__rq_lockp(ساخت rq *rq)
	__returns_ctx_lock(rq_lockp(rq)) /* alias them */
{
	واپس &rq->__lock;
}

ساکن اندرونی bool sched_cpu_cookie_مطابقت(ساخت rq *rq, ساخت task_struct *p)
{
	واپس true;
}

ساکن اندرونی bool sched_core_cookie_مطابقت(ساخت rq *rq, ساخت task_struct *p)
{
	واپس true;
}

ساکن اندرونی bool sched_گروہ_cookie_مطابقت(ساخت rq *rq,
					    ساخت task_struct *p,
					    ساخت sched_گروہ *گروہ)
{
	واپس true;
}

ساکن اندرونی bool task_has_sched_core(ساخت task_struct *p)
{
	واپس false;
}

#اگر_ختم /* !CONFIG_SCHED_CORE */

#اگر_تعریف_شدہ CONFIG_RT_گروہ_SCHED
# ifdef CONFIG_RT_گروہ_SCHED_DEFAULT_DISABLED
DECLARE_STATIC_کلید_FALSE(rt_گروہ_sched);
ساکن اندرونی bool rt_گروہ_sched_enabled(خالی)
{
	واپس static_branch_unlikely(&rt_گروہ_sched);
}
# ورنہ /* !CONFIG_RT_GROUP_SCHED_DEFAULT_DISABLED: */
DECLARE_STATIC_کلید_TRUE(rt_گروہ_sched);
ساکن اندرونی bool rt_گروہ_sched_enabled(خالی)
{
	واپس static_branch_likely(&rt_گروہ_sched);
}
# endif /* !CONFIG_RT_GROUP_SCHED_DEFAULT_DISABLED */
#ورنہ /* !CONFIG_RT_GROUP_SCHED: */
# define rt_گروہ_sched_enabled()	false
#اگر_ختم /* !CONFIG_RT_GROUP_SCHED */

ساکن اندرونی خالی lockdep_assert_rq_held(ساخت rq *rq)
	__assumes_ctx_lock(__rq_lockp(rq))
{
	lockdep_assert_held(__rq_lockp(rq));
}

خارجی خالی raw_spin_rq_تالا_nested(ساخت rq *rq, صحیح subclass)
	__acquires(__rq_lockp(rq));

خارجی bool raw_spin_rq_trylock(ساخت rq *rq)
	__cond_acquires(true, __rq_lockp(rq));

ساکن اندرونی خالی raw_spin_rq_تالا(ساخت rq *rq)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_تالا_nested(rq, 0);
}

ساکن اندرونی خالی raw_spin_rq_unlock(ساخت rq *rq)
	__releases(__rq_lockp(rq))
{
	raw_spin_unlock(rq_lockp(rq));
}

ساکن اندرونی خالی raw_spin_rq_تالا_irq(ساخت rq *rq)
	__acquires(__rq_lockp(rq))
{
	local_irq_disable();
	raw_spin_rq_تالا(rq);
}

ساکن اندرونی خالی raw_spin_rq_unlock_irq(ساخت rq *rq)
	__releases(__rq_lockp(rq))
{
	raw_spin_rq_unlock(rq);
	local_irq_enable();
}

ساکن اندرونی غیرعلامتی طویل raw_spin_rq_تالا_irqsave_2(ساخت rq *rq)
	__acquires(__rq_lockp(rq))
{
	غیرعلامتی طویل flags;

	local_irq_save(flags);
	raw_spin_rq_تالا(rq);

	واپس flags;
}

ساکن اندرونی خالی raw_spin_rq_unlock_irqrestore(ساخت rq *rq, غیرعلامتی طویل flags)
	__releases(__rq_lockp(rq))
{
	raw_spin_rq_unlock(rq);
	local_irq_restore(flags);
}

#تعریف_کرو raw_spin_rq_تالا_irqsave(rq, flags)	\
کرو {						\
	flags = raw_spin_rq_تالا_irqsave_2(rq);	\
} جب (0)

خارجی خالی __update_idle_core(ساخت rq *rq);

ساکن اندرونی خالی update_idle_core(ساخت rq *rq)
{
	اگر (sched_smt_فعال())
		__update_idle_core(rq);
}

#اگر_تعریف_شدہ CONFIG_FAIR_گروہ_SCHED
ساکن اندرونی ساخت task_struct *task_کا(ساخت sched_entity *se)
{
	WARN_ON_ONCE(!entity_is_task(se));
	واپس container_کا(se, ساخت task_struct, se);
}

ساکن اندرونی ساخت cfs_rq *task_cfs_rq(ساخت task_struct *p)
{
	واپس p->se.cfs_rq;
}

/* runqueue on which this entity is (to be) queued */
ساکن اندرونی ساخت cfs_rq *cfs_rq_کا(ثابت ساخت sched_entity *se)
{
	واپس se->cfs_rq;
}

/* runqueue "owned" by this group */
ساکن اندرونی ساخت cfs_rq *گروہ_cfs_rq(ساخت sched_entity *grp)
{
	واپس grp->my_q;
}

#ورنہ /* !CONFIG_FAIR_GROUP_SCHED: */

#تعریف_کرو task_کا(_se)		container_کا(_se, ساخت task_struct, se)

ساکن اندرونی ساخت cfs_rq *task_cfs_rq(ثابت ساخت task_struct *p)
{
	واپس &task_rq(p)->cfs;
}

ساکن اندرونی ساخت cfs_rq *cfs_rq_کا(ثابت ساخت sched_entity *se)
{
	ثابت ساخت task_struct *p = task_کا(se);
	ساخت rq *rq = task_rq(p);

	واپس &rq->cfs;
}

/* runqueue "owned" by this group */
ساکن اندرونی ساخت cfs_rq *گروہ_cfs_rq(ساخت sched_entity *grp)
{
	واپس تہی;
}

#اگر_ختم /* !CONFIG_FAIR_GROUP_SCHED */

خارجی خالی update_rq_avg_idle(ساخت rq *rq);
خارجی خالی update_rq_clock(ساخت rq *rq);

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
#تعریف_کرو RQCF_REQ_SKIP		0x01
#تعریف_کرو RQCF_ACT_SKIP		0x02
#تعریف_کرو RQCF_UPDATED		0x04

ساکن اندرونی خالی assert_clock_updated(ساخت rq *rq)
{
	/*
	 * The only reason for not seeing a clock update since the
	 * last rq_pin_lock() is if we're currently skipping updates.
	 */
	WARN_ON_ONCE(rq->clock_update_flags < RQCF_ACT_SKIP);
}

ساکن اندرونی u64 rq_clock(ساخت rq *rq)
{
	lockdep_assert_rq_held(rq);
	assert_clock_updated(rq);

	واپس rq->clock;
}

ساکن اندرونی u64 rq_clock_task(ساخت rq *rq)
{
	lockdep_assert_rq_held(rq);
	assert_clock_updated(rq);

	واپس rq->clock_task;
}

ساکن اندرونی خالی rq_clock_skip_update(ساخت rq *rq)
{
	lockdep_assert_rq_held(rq);
	rq->clock_update_flags |= RQCF_REQ_SKIP;
}

/*
 * See rt task throttling, which is the only time a skip
 * request is canceled.
 */
ساکن اندرونی خالی rq_clock_cancel_skipupdate(ساخت rq *rq)
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
ساکن اندرونی خالی rq_clock_آغاز_loop_update(ساخت rq *rq)
{
	lockdep_assert_rq_held(rq);
	WARN_ON_ONCE(rq->clock_update_flags & RQCF_ACT_SKIP);
	rq->clock_update_flags |= RQCF_ACT_SKIP;
}

ساکن اندرونی خالی rq_clock_stop_loop_update(ساخت rq *rq)
{
	lockdep_assert_rq_held(rq);
	rq->clock_update_flags &= ~RQCF_ACT_SKIP;
}

ساخت rq_flags {
	غیرعلامتی طویل flags;
	ساخت pin_cookie cookie;
	/*
	 * A copy of (rq::clock_update_flags & RQCF_UPDATED) for the
	 * current pin context is stashed here in case it needs to be
	 * restored in rq_repin_lock().
	 */
	غیرعلامتی صحیح clock_update_flags;
};

خارجی ساخت balance_callback balance_ڈالو_callback;

#اگر_تعریف_شدہ CONFIG_SCHED_CLASS_EXT
خارجی ثابت ساخت sched_class ext_sched_class;

DECLARE_STATIC_کلید_FALSE(__scx_enabled);	/* SCX BPF scheduler loaded */
DECLARE_STATIC_کلید_FALSE(__scx_switched_all);	/* all fair class tasks on SCX */

#تعریف_کرو scx_enabled()		static_branch_unlikely(&__scx_enabled)
#تعریف_کرو scx_switched_all()	static_branch_unlikely(&__scx_switched_all)

ساکن اندرونی خالی scx_rq_clock_update(ساخت rq *rq, u64 clock)
{
	اگر (!scx_enabled())
		واپس;
	WRITE_ONCE(rq->scx.clock, clock);
	smp_store_release(&rq->scx.flags, rq->scx.flags | SCX_RQ_CLK_VALID);
}

ساکن اندرونی خالی scx_rq_clock_invalidate(ساخت rq *rq)
{
	اگر (!scx_enabled())
		واپس;
	WRITE_ONCE(rq->scx.flags, rq->scx.flags & ~SCX_RQ_CLK_VALID);
}

#ورنہ /* !CONFIG_SCHED_CLASS_EXT: */
#تعریف_کرو scx_enabled()		false
#تعریف_کرو scx_switched_all()	false

ساکن اندرونی خالی scx_rq_clock_update(ساخت rq *rq, u64 clock) {}
ساکن اندرونی خالی scx_rq_clock_invalidate(ساخت rq *rq) {}
#اگر_ختم /* !CONFIG_SCHED_CLASS_EXT */

ساکن اندرونی خالی assert_balance_callbacks_خالی_خانے(ساخت rq *rq)
{
	WARN_ON_ONCE(IS_ENABLED(CONFIG_PROVE_LOCKING) &&
		     rq->balance_callback &&
		     rq->balance_callback != &balance_ڈالو_callback);
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
ساکن اندرونی خالی rq_pin_تالا(ساخت rq *rq, ساخت rq_flags *rf)
{
	rf->cookie = lockdep_pin_تالا(__rq_lockp(rq));

	rq->clock_update_flags &= (RQCF_REQ_SKIP|RQCF_ACT_SKIP);
	rf->clock_update_flags = 0;
	assert_balance_callbacks_خالی_خانے(rq);
}

ساکن اندرونی خالی rq_unpin_تالا(ساخت rq *rq, ساخت rq_flags *rf)
{
	اگر (rq->clock_update_flags > RQCF_ACT_SKIP)
		rf->clock_update_flags = RQCF_UPDATED;

	scx_rq_clock_invalidate(rq);
	lockdep_unpin_تالا(__rq_lockp(rq), rf->cookie);
}

ساکن اندرونی خالی rq_repin_تالا(ساخت rq *rq, ساخت rq_flags *rf)
{
	lockdep_repin_تالا(__rq_lockp(rq), rf->cookie);

	/*
	 * Restore the value we stashed in @rf for this pin context.
	 */
	rq->clock_update_flags |= rf->clock_update_flags;
}

#تعریف_کرو __task_rq_lock(...) __acquire_ret(___task_rq_lock(__VA_ARGS__), __rq_lockp(__ret))
خارجی ساخت rq *___task_rq_lock(ساخت task_struct *p, ساخت rq_flags *rf) __acquires_ret;

#تعریف_کرو task_rq_تالا_2(...) __acquire_ret(task_rq_تالا(__VA_ARGS__), __rq_lockp(__ret))
خارجی ساخت rq *task_rq_تالا(ساخت task_struct *p, ساخت rq_flags *rf)
	__acquires(&p->pi_تالا) __acquires_ret;

ساکن اندرونی خالی
__task_rq_unlock(ساخت rq *rq, ساخت task_struct *p, ساخت rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_تالا(rq, rf);
	raw_spin_rq_unlock(rq);
}

ساکن اندرونی خالی
task_rq_unlock(ساخت rq *rq, ساخت task_struct *p, ساخت rq_flags *rf)
	__releases(__rq_lockp(rq), &p->pi_تالا)
{
	__task_rq_unlock(rq, p, rf);
	raw_spin_unlock_irqrestore(&p->pi_تالا, rf->flags);
}

DEFINE_تالا_GUARD_1(task_rq_تالا_2, ساخت task_struct,
		    _T->rq = task_rq_تالا_2(_T->تالا, &_T->rf),
		    task_rq_unlock(_T->rq, _T->تالا, &_T->rf),
		    ساخت rq *rq; ساخت rq_flags rf)
DECLARE_تالا_GUARD_1_ATTRS(task_rq_تالا_2, __acquires(_T->pi_تالا), __releases((*(ساخت task_struct **)_T)->pi_تالا))
#تعریف_کرو class_task_rq_تالا_constructor(_T) WITH_تالا_GUARD_1_ATTRS(task_rq_تالا_2, _T)

DEFINE_تالا_GUARD_1(__task_rq_lock, ساخت task_struct,
		    _T->rq = __task_rq_lock(_T->تالا, &_T->rf),
		    __task_rq_unlock(_T->rq, _T->تالا, &_T->rf),
		    ساخت rq *rq; ساخت rq_flags rf)

ساکن اندرونی خالی rq_تالا_irqsave(ساخت rq *rq, ساخت rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_تالا_irqsave(rq, rf->flags);
	rq_pin_تالا(rq, rf);
}

ساکن اندرونی خالی rq_تالا_irq(ساخت rq *rq, ساخت rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_تالا_irq(rq);
	rq_pin_تالا(rq, rf);
}

ساکن اندرونی خالی rq_تالا(ساخت rq *rq, ساخت rq_flags *rf)
	__acquires(__rq_lockp(rq))
{
	raw_spin_rq_تالا(rq);
	rq_pin_تالا(rq, rf);
}

ساکن اندرونی خالی rq_unlock_irqrestore(ساخت rq *rq, ساخت rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_تالا(rq, rf);
	raw_spin_rq_unlock_irqrestore(rq, rf->flags);
}

ساکن اندرونی خالی rq_unlock_irq(ساخت rq *rq, ساخت rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_تالا(rq, rf);
	raw_spin_rq_unlock_irq(rq);
}

ساکن اندرونی خالی rq_unlock(ساخت rq *rq, ساخت rq_flags *rf)
	__releases(__rq_lockp(rq))
{
	rq_unpin_تالا(rq, rf);
	raw_spin_rq_unlock(rq);
}

DEFINE_تالا_GUARD_1(rq_تالا, ساخت rq,
		    rq_تالا(_T->تالا, &_T->rf),
		    rq_unlock(_T->تالا, &_T->rf),
		    ساخت rq_flags rf)

DECLARE_تالا_GUARD_1_ATTRS(rq_تالا, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(ساخت rq **)_T)));
#تعریف_کرو class_rq_تالا_constructor(_T) WITH_تالا_GUARD_1_ATTRS(rq_تالا, _T)

DEFINE_تالا_GUARD_1(rq_تالا_irq, ساخت rq,
		    rq_تالا_irq(_T->تالا, &_T->rf),
		    rq_unlock_irq(_T->تالا, &_T->rf),
		    ساخت rq_flags rf)

DECLARE_تالا_GUARD_1_ATTRS(rq_تالا_irq, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(ساخت rq **)_T)));
#تعریف_کرو class_rq_تالا_irq_constructor(_T) WITH_تالا_GUARD_1_ATTRS(rq_تالا_irq, _T)

DEFINE_تالا_GUARD_1(rq_تالا_irqsave, ساخت rq,
		    rq_تالا_irqsave(_T->تالا, &_T->rf),
		    rq_unlock_irqrestore(_T->تالا, &_T->rf),
		    ساخت rq_flags rf)

DECLARE_تالا_GUARD_1_ATTRS(rq_تالا_irqsave, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(ساخت rq **)_T)));
#تعریف_کرو class_rq_تالا_irqsave_constructor(_T) WITH_تالا_GUARD_1_ATTRS(rq_تالا_irqsave, _T)

DEFINE_تالا_GUARD_1(raw_spin_rq_تالا_irqsave, ساخت rq,
		    raw_spin_rq_تالا_irqsave(_T->تالا, _T->flags),
		    raw_spin_rq_unlock_irqrestore(_T->تالا, _T->flags),
		    غیرعلامتی طویل flags)

DECLARE_تالا_GUARD_1_ATTRS(raw_spin_rq_تالا_irqsave, __acquires(__rq_lockp(_T)),
			   __releases(__rq_lockp(*(ساخت rq **)_T)));
#تعریف_کرو class_raw_spin_rq_تالا_irqsave_constructor(_T) \
	WITH_تالا_GUARD_1_ATTRS(raw_spin_rq_تالا_irqsave, _T)

#تعریف_کرو this_rq_تالا_irq(...) __acquire_ret(this_rq_تالا_irq_2(__VA_ARGS__), __rq_lockp(__ret))
ساکن اندرونی ساخت rq *this_rq_تالا_irq_2(ساخت rq_flags *rf) __acquires_ret
{
	ساخت rq *rq;

	local_irq_disable();
	rq = this_rq();
	rq_تالا(rq, rf);

	واپس rq;
}

#اگر_تعریف_شدہ CONFIG_NUMA

شمار numa_topology_type {
	NUMA_براہ_راست,
	NUMA_GLUELESS_MESH,
	NUMA_BACKPLANE,
};

خارجی شمار numa_topology_type sched_numa_topology_type;
خارجی صحیح sched_max_numa_فاصلہ;
خارجی bool ڈھونڈو_numa_فاصلہ(صحیح فاصلہ);
خارجی خالی sched_init_numa(صحیح offline_گرہ);
خارجی خالی sched_update_numa(صحیح cpu, bool online);
خارجی خالی sched_domains_numa_masks_سیٹ(غیرعلامتی صحیح cpu);
خارجی خالی sched_domains_numa_masks_clear(غیرعلامتی صحیح cpu);
خارجی صحیح sched_numa_ڈھونڈو_closest(ثابت ساخت cpumask *cpus, صحیح cpu);

#ورنہ /* !CONFIG_NUMA: */

ساکن اندرونی خالی sched_init_numa(صحیح offline_گرہ) { }
ساکن اندرونی خالی sched_update_numa(صحیح cpu, bool online) { }
ساکن اندرونی خالی sched_domains_numa_masks_سیٹ(غیرعلامتی صحیح cpu) { }
ساکن اندرونی خالی sched_domains_numa_masks_clear(غیرعلامتی صحیح cpu) { }

ساکن اندرونی صحیح sched_numa_ڈھونڈو_closest(ثابت ساخت cpumask *cpus, صحیح cpu)
{
	واپس nr_cpu_ids;
}

#اگر_ختم /* !CONFIG_NUMA */

#اگر_تعریف_شدہ CONFIG_NUMA_BALANCING

/* The regions in numa_faults array from task_struct */
شمار numa_خرابیاں_stats {
	NUMA_MEM = 0,
	NUMA_CPU,
	NUMA_MEMBUF,
	NUMA_CPUBUF
};

خارجی خالی sched_setnuma(ساخت task_struct *p, صحیح گرہ);
خارجی صحیح migrate_task_مقصد(ساخت task_struct *p, صحیح cpu);
خارجی صحیح migrate_تبادلہ(ساخت task_struct *p, ساخت task_struct *t,
			صحیح cpu, صحیح scpu);
خارجی خالی init_numa_balancing(u64 clone_flags, ساخت task_struct *p);

#ورنہ /* !CONFIG_NUMA_BALANCING: */

ساکن اندرونی خالی
init_numa_balancing(u64 clone_flags, ساخت task_struct *p)
{
}

#اگر_ختم /* !CONFIG_NUMA_BALANCING */

صحیح task_llc(ثابت ساخت task_struct *p);

ساکن اندرونی خالی
قطار_balance_callback(ساخت rq *rq,
		       ساخت balance_callback *سر,
		       خالی (*func)(ساخت rq *rq))
{
	lockdep_assert_rq_held(rq);

	/*
	 * Don't (re)queue an already queued item; nor queue anything when
	 * balance_push() is active, see the comment with
	 * balance_push_callback.
	 */
	اگر (unlikely(سر->اگلا || rq->balance_callback == &balance_ڈالو_callback))
		واپس;

	سر->func = func;
	سر->اگلا = rq->balance_callback;
	rq->balance_callback = سر;
}

#تعریف_کرو rcu_dereference_sched_domain(p) \
	rcu_dereference_all_check((p), lockdep_is_held(&sched_domains_mutex))

/*
 * The domain tree (rq->sd) is protected by RCU's quiescent state transition.
 * See destroy_sched_domains: call_rcu for details.
 *
 * The domain tree of any CPU may only be accessed from within
 * preempt-disabled sections.
 */
#تعریف_کرو for_each_domain(cpu, __sd) \
	برائے (__sd = rcu_dereference_sched_domain(cpu_rq(cpu)->sd); \
			__sd; __sd = __sd->والد)

/* A mask of all the SD flags that have the SDF_SHARED_CHILD metaflag */
#تعریف_کرو SD_FLAG(name, mflags) (name * !!((mflags) & SDF_SHARED_بچہ)) |
ساکن ثابت غیرعلامتی صحیح SD_SHARED_بچہ_نقاب =
#شامل <linux/sched/sd_flags.h>
0;
#تعریف_منسوخ SD_FLAG

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
ساکن اندرونی ساخت sched_domain *highest_flag_domain(صحیح cpu, صحیح flag)
{
	ساخت sched_domain *sd, *hsd = تہی;

	for_each_domain(cpu, sd) {
		اگر (sd->flags & flag) {
			hsd = sd;
			جاری;
		}

		/*
		 * Stop the search if @flag is known to be shared at lower
		 * levels. It will not be found further up.
		 */
		اگر (flag & SD_SHARED_بچہ_نقاب)
			توڑو;
	}

	واپس hsd;
}

ساکن اندرونی ساخت sched_domain *سب_سے_نیچا_flag_domain(صحیح cpu, صحیح flag)
{
	ساخت sched_domain *sd;

	for_each_domain(cpu, sd) {
		اگر (sd->flags & flag)
			توڑو;
	}

	واپس sd;
}

DECLARE_فی_CPU(ساخت sched_domain __rcu *, sd_llc);
DECLARE_فی_CPU(صحیح, sd_llc_سائز);
DECLARE_فی_CPU(صحیح, sd_llc_id);
DECLARE_فی_CPU(صحیح, sd_share_id);
DECLARE_فی_CPU(ساخت sched_domain_shared __rcu *, sd_llc_shared);
DECLARE_فی_CPU(ساخت sched_domain_shared __rcu *, sd_balance_shared);
DECLARE_فی_CPU(ساخت sched_domain __rcu *, sd_numa);
DECLARE_فی_CPU(ساخت sched_domain __rcu *, sd_asym_packing);
DECLARE_فی_CPU(ساخت sched_domain __rcu *, sd_asym_cpucapacity);

خارجی ساخت static_کلید_false sched_asym_cpucapacity;
خارجی ساخت static_کلید_false sched_cluster_فعال;

ساکن __always_inline bool sched_asym_cpucap_فعال(خالی)
{
	واپس static_branch_unlikely(&sched_asym_cpucapacity);
}

ساخت sched_گروہ_استعداد {
	atomic_t		ref;
	/*
	 * CPU capacity of this group, SCHED_CAPACITY_SCALE being max capacity
	 * for a single CPU.
	 */
	غیرعلامتی طویل		استعداد;
	غیرعلامتی طویل		min_استعداد;		/* Min per-CPU capacity in group */
	غیرعلامتی طویل		max_استعداد;		/* Max per-CPU capacity in group */
	غیرعلامتی طویل		اگلا_update;
	صحیح			imbalance;		/* XXX unrelated to capacity but shared group state */

	صحیح			id;

	غیرعلامتی طویل		cpumask[];		/* Balance mask */
};

ساخت sched_گروہ {
	ساخت sched_گروہ	*اگلا;			/* Must be a circular list */
	atomic_t		ref;

	غیرعلامتی صحیح		گروہ_وزن;
	غیرعلامتی صحیح		cores;
	ساخت sched_گروہ_استعداد *sgc;
	صحیح			asym_prefer_cpu;	/* CPU of highest priority in group */
	صحیح			flags;

	/*
	 * The CPUs this group covers.
	 *
	 * NOTE: this field is variable length. (Allocated dynamically
	 * by attaching extra space to the end of the structure,
	 * depending on how many CPUs the kernel has booted up with)
	 */
	غیرعلامتی طویل		cpumask[];
};

ساکن اندرونی ساخت cpumask *sched_گروہ_وسعت(ساخت sched_گروہ *sg)
{
	واپس مقصد_cpumask(sg->cpumask);
}

/*
 * See build_balance_mask().
 */
ساکن اندرونی ساخت cpumask *گروہ_balance_نقاب(ساخت sched_گروہ *sg)
{
	واپس مقصد_cpumask(sg->sgc->cpumask);
}

خارجی صحیح گروہ_balance_cpu(ساخت sched_گروہ *sg);

خارجی خالی update_sched_domain_debugfs(خالی);
خارجی خالی dirty_sched_domain_sysctl(صحیح cpu);

خارجی صحیح sched_update_scaling(خالی);

ساکن اندرونی ثابت ساخت cpumask *task_user_cpus(ساخت task_struct *p)
{
	اگر (!p->user_cpus_ptr)
		واپس cpu_possible_نقاب; /* &init_task.cpus_mask */
	واپس p->user_cpus_ptr;
}

#اگر_تعریف_شدہ CONFIG_CGROUP_SCHED

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
ساکن اندرونی ساخت task_گروہ *task_گروہ(ساخت task_struct *p)
{
	واپس p->sched_task_گروہ;
}

#اگر_تعریف_شدہ CONFIG_FAIR_گروہ_SCHED
/*
 * Defined here to be available before stats.h is included, since
 * stats.h has dependencies on things defined later in this file.
 */
ساخت cfs_tg_state {
	ساخت cfs_rq		cfs_rq;
	ساخت sched_entity	se;
	ساخت sched_statistics	stats;
} __no_randomize_layout;

/* Access a specific CPU's cfs_rq from a task group */
ساکن اندرونی ساخت cfs_rq *tg_cfs_rq(ساخت task_گروہ *tg, صحیح cpu)
{
	واپس فی_cpu_ptr(tg->cfs_rq, cpu);
}

ساکن اندرونی ساخت sched_entity *tg_se(ساخت task_گروہ *tg, صحیح cpu)
{
	ساخت cfs_tg_state *state;

	اگر (is_جڑ_task_گروہ(tg))
		واپس تہی;

	state = container_کا(tg_cfs_rq(tg, cpu), ساخت cfs_tg_state, cfs_rq);
	واپس &state->se;
}

ساکن اندرونی ساخت sched_entity *cfs_rq_se(ساخت cfs_rq *cfs_rq)
{
	ساخت cfs_tg_state *state;

	اگر (is_جڑ_task_گروہ(cfs_rq->tg))
		واپس تہی;

	state = container_کا(cfs_rq, ساخت cfs_tg_state, cfs_rq);
	واپس &state->se;
}
#اگر_ختم

/* Change a task's cfs_rq and parent entity if it moves across CPUs/groups */
ساکن اندرونی خالی سیٹ_task_rq(ساخت task_struct *p, غیرعلامتی صحیح cpu)
{
#اگر defined(CONFIG_FAIR_گروہ_SCHED) || defined(CONFIG_RT_گروہ_SCHED)
	ساخت task_گروہ *tg = task_گروہ(p);
#اگر_ختم

#اگر_تعریف_شدہ CONFIG_FAIR_گروہ_SCHED
	سیٹ_task_rq_fair(&p->se, p->se.cfs_rq, tg_cfs_rq(tg, cpu));
	p->se.cfs_rq = tg_cfs_rq(tg, cpu);
	p->se.والد = tg_se(tg, cpu);
	p->se.گہرائی = p->se.والد ? p->se.والد->گہرائی + 1 : 0;
#اگر_ختم

#اگر_تعریف_شدہ CONFIG_RT_گروہ_SCHED
	/*
	 * p->rt.rt_rq is NULL initially and it is easier to assign
	 * root_task_group's rt_rq than switching in rt_rq_of_se()
	 * Clobbers tg(!)
	 */
	اگر (!rt_گروہ_sched_enabled())
		tg = &جڑ_task_گروہ;
	p->rt.rt_rq  = tg->rt_rq[cpu];
	p->rt.والد = tg->rt_se[cpu];
#اگر_ختم /* CONFIG_RT_GROUP_SCHED */
}

#ورنہ /* !CONFIG_CGROUP_SCHED: */

ساکن اندرونی خالی سیٹ_task_rq(ساخت task_struct *p, غیرعلامتی صحیح cpu) { }

ساکن اندرونی ساخت task_گروہ *task_گروہ(ساخت task_struct *p)
{
	واپس تہی;
}

#اگر_ختم /* !CONFIG_CGROUP_SCHED */

ساکن اندرونی خالی __set_task_cpu(ساخت task_struct *p, غیرعلامتی صحیح cpu)
{
	سیٹ_task_rq(p, cpu);
#اگر_تعریف_شدہ CONFIG_SMP
	/*
	 * After ->cpu is set up to a new value, task_rq_lock(p, ...) can be
	 * successfully executed on another CPU. We must ensure that updates of
	 * per-task data have been completed by this moment.
	 */
	smp_wmb();
	WRITE_ONCE(task_thread_info(p)->cpu, cpu);
	p->wake_cpu = cpu;
	rseq_sched_سیٹ_ids_بدلا(p);
#اگر_ختم /* CONFIG_SMP */
}

/*
 * Tunables:
 */

#تعریف_کرو SCHED_FEAT(name, enabled)	\
	__SCHED_FEAT_##name ,

شمار {
#شامل "features.h"
	__SCHED_FEAT_NR,
};

#تعریف_منسوخ SCHED_FEAT

/*
 * To support run-time toggling of sched features, all the translation units
 * (but core.c) reference the sysctl_sched_features defined in core.c.
 */
خارجی __read_mostly غیرعلامتی صحیح sysctl_sched_features;

#اگر_تعریف_شدہ CONFIG_JUMP_LABEL

اتحاد sched_feat_کلید {
	ساخت static_کلید_true	کلید_true;
	ساخت static_کلید_false	کلید_false;
};

#تعریف_کرو sched_feat_branch_true(کلید)	static_branch_likely(&(کلید)->کلید_true)
#تعریف_کرو sched_feat_branch_false(کلید)	static_branch_unlikely(&(کلید)->کلید_false)

#تعریف_کرو SCHED_FEAT(name, enabled)			\
ساکن __always_inline bool				\
static_branch_##name(اتحاد sched_feat_کلید *کلید) 	\
{							\
	واپس sched_feat_branch_##enabled(کلید);	\
}

#شامل "features.h"
#تعریف_منسوخ SCHED_FEAT

خارجی اتحاد sched_feat_کلید sched_feat_کلیدیں[__SCHED_FEAT_NR];
#تعریف_کرو sched_feat(x) (static_branch_##x(&sched_feat_کلیدیں[__SCHED_FEAT_##x]))

#ورنہ /* !CONFIG_JUMP_LABEL: */

#تعریف_کرو sched_feat(x) (sysctl_sched_features & (1UL << __SCHED_FEAT_##x))

#اگر_ختم /* !CONFIG_JUMP_LABEL */

خارجی ساخت static_کلید_false sched_numa_balancing;
خارجی ساخت static_کلید_false sched_schedstats;

ساکن اندرونی u64 global_rt_period(خالی)
{
	واپس (u64)sysctl_sched_rt_period * NSEC_فی_USEC;
}

ساکن اندرونی u64 global_rt_runtime(خالی)
{
	اگر (sysctl_sched_rt_runtime < 0)
		واپس RUNTIME_INF;

	واپس (u64)sysctl_sched_rt_runtime * NSEC_فی_USEC;
}

/*
 * Is p the current execution context?
 */
ساکن اندرونی صحیح task_current(ساخت rq *rq, ساخت task_struct *p)
{
	واپس rq->curr == p;
}

/*
 * Is p the current scheduling context?
 *
 * Note that it might be the current execution context at the same time if
 * rq->curr == rq->donor == p.
 */
ساکن اندرونی صحیح task_current_donor(ساخت rq *rq, ساخت task_struct *p)
{
	واپس rq->donor == p;
}

ساکن اندرونی bool task_is_blocked(ساخت task_struct *p)
{
	اگر (!sched_proxy_exec())
		واپس false;

	واپس !!p->blocked_on;
}

#اگر_تعریف_شدہ CONFIG_SCHED_PROXY_EXEC
خالی sched_proxy_بلاک_task(ساخت rq *rq, ساخت task_struct *p);
#ورنہ
ساکن اندرونی خالی sched_proxy_بلاک_task(ساخت rq *rq, ساخت task_struct *p) {}
#اگر_ختم

ساکن اندرونی صحیح task_on_cpu(ساخت rq *rq, ساخت task_struct *p)
{
	واپس p->on_cpu;
}

ساکن اندرونی صحیح task_on_rq_queued(ساخت task_struct *p)
{
	واپس READ_ONCE(p->on_rq) == TASK_ON_RQ_QUEUED;
}

ساکن اندرونی صحیح task_on_rq_migrating(ساخت task_struct *p)
{
	واپس READ_ONCE(p->on_rq) == TASK_ON_RQ_MIGRATING;
}

/* Wake flags. The first three directly map to some SD flag value */
#تعریف_کرو WF_EXEC			0x02 /* Wakeup after exec; maps to SD_BALANCE_EXEC */
#تعریف_کرو WF_FORK			0x04 /* Wakeup after fork; maps to SD_BALANCE_FORK */
#تعریف_کرو WF_TTWU			0x08 /* Wakeup;            maps to SD_BALANCE_WAKE */
/*
 * Hint that the caller expects the waker to sleep soon.
 * Scheduler classes may use it for placement or preemption.
 * Callers must not rely on it to prevent migration,
 * preserve CPU locality or make the wakee run next.
 */
#تعریف_کرو WF_SYNC			0x10
#تعریف_کرو WF_MIGRATED		0x20 /* Internal use, task got migrated */
#تعریف_کرو WF_CURRENT_CPU		0x40 /* Prefer to move the wakee to the current CPU. */
#تعریف_کرو WF_RQ_SELECTED		0x80 /* ->select_task_rq() was called */
#تعریف_کرو WF_TTWU_RQ		0x100 /* Wakeup completed through ttwu_runnable() */

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

#تعریف_کرو وزن_IDLEPRIO		3
#تعریف_کرو WMULT_IDLEPRIO		1431655765

خارجی ثابت صحیح		sched_prio_مقصد_وزن[40];
خارجی ثابت u32		sched_prio_مقصد_wmult[40];

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

#تعریف_کرو قطار_سے_نکالو_SLEEP		0x0001 /* Matches ENQUEUE_WAKEUP */
#تعریف_کرو قطار_سے_نکالو_SAVE		0x0002 /* Matches ENQUEUE_RESTORE */
#تعریف_کرو قطار_سے_نکالو_MOVE		0x0004 /* Matches ENQUEUE_MOVE */
#تعریف_کرو قطار_سے_نکالو_NOCLOCK		0x0008 /* Matches ENQUEUE_NOCLOCK */

#تعریف_کرو قطار_سے_نکالو_MIGRATING	0x0010 /* Matches ENQUEUE_MIGRATING */
#تعریف_کرو قطار_سے_نکالو_DELAYED		0x0020 /* Matches ENQUEUE_DELAYED */
#تعریف_کرو قطار_سے_نکالو_CLASS		0x0040 /* Matches ENQUEUE_CLASS */

#تعریف_کرو قطار_سے_نکالو_SPECIAL		0x00010000
#تعریف_کرو قطار_سے_نکالو_THROTTLE	0x00020000

#تعریف_کرو قطار_میں_ڈالو_WAKEUP		0x0001
#تعریف_کرو قطار_میں_ڈالو_RESTORE		0x0002
#تعریف_کرو قطار_میں_ڈالو_MOVE		0x0004
#تعریف_کرو قطار_میں_ڈالو_NOCLOCK		0x0008

#تعریف_کرو قطار_میں_ڈالو_MIGRATING	0x0010
#تعریف_کرو قطار_میں_ڈالو_DELAYED		0x0020
#تعریف_کرو قطار_میں_ڈالو_CLASS		0x0040

#تعریف_کرو قطار_میں_ڈالو_سر		0x00010000
#تعریف_کرو قطار_میں_ڈالو_REPLENISH	0x00020000
#تعریف_کرو قطار_میں_ڈالو_MIGRATED	0x00040000
#تعریف_کرو قطار_میں_ڈالو_INITIAL		0x00080000
#تعریف_کرو قطار_میں_ڈالو_RQ_SELECTED	0x00100000
#تعریف_کرو قطار_میں_ڈالو_QUEUED		0x00200000

#تعریف_کرو RETRY_TASK		((خالی *)-1UL)

ساخت affinity_context {
	ثابت ساخت cpumask	*new_نقاب;
	ساخت cpumask		*user_نقاب;
	غیرعلامتی صحیح		flags;
};

خارجی s64 update_curr_common(ساخت rq *rq);

شمار snt_e {
	SNT_معمول,	/* set_next_task() */
	SNT_PICK,	/* put_prev_set_next_task(): prev != next */
	SNT_REPICK,	/* put_prev_set_next_task(): prev == next */
};

ساخت sched_class {

#اگر_تعریف_شدہ CONFIG_UCLAMP_TASK
	صحیح uclamp_enabled;
#اگر_ختم

	/*
	 * move_queued_task/activate_task/enqueue_task: rq->lock
	 * ttwu_do_activate/activate_task/enqueue_task: rq->lock
	 * wake_up_new_task/activate_task/enqueue_task: task_rq_lock
	 * ttwu_runnable/enqueue_task: task_rq_lock
	 * proxy_task_current: rq->lock
	 * sched_change_end
	 */
	خالی (*قطار_میں_ڈالو_task) (ساخت rq *rq, ساخت task_struct *p, صحیح flags);
	/*
	 * move_queued_task/deactivate_task/dequeue_task: rq->lock
	 * __schedule/block_task/dequeue_task: rq->lock
	 * proxy_task_current: rq->lock
	 * wait_task_inactive: task_rq_lock
	 * sched_change_begin
	 */
	bool (*قطار_سے_نکالو_task) (ساخت rq *rq, ساخت task_struct *p, صحیح flags);

	/*
	 * do_sched_yield: rq->lock
	 */
	خالی (*yield_task)   (ساخت rq *rq);
	/*
	 * yield_to: rq->lock (double)
	 */
	bool (*yield_مقصد_task)(ساخت rq *rq, ساخت task_struct *p);

	/*
	 * move_queued_task: rq->lock
	 * __migrate_swap_task: rq->lock
	 * ttwu_do_activate: rq->lock
	 * ttwu_runnable: task_rq_lock
	 * wake_up_new_task: task_rq_lock
	 */
	خالی (*wakeup_preempt)(ساخت rq *rq, ساخت task_struct *p, صحیح flags);

	/*
	 * schedule/pick_next_task/prev_balance: rq->lock
	 */
	صحیح (*balance)(ساخت rq *rq, ساخت rq_flags *rf);

	/*
	 * schedule/pick_next_task: rq->lock
	 */
	ساخت task_struct *(*pick_task)(ساخت rq *rq, ساخت rq_flags *rf);

	/*
	 * sched_change:
	 * __schedule: rq->lock
	 */
	خالی (*put_پچھلا_task)(ساخت rq *rq, ساخت task_struct *p, ساخت task_struct *اگلا);
	خالی (*سیٹ_اگلا_task)(ساخت rq *rq, ساخت task_struct *p, شمار snt_e type);

	/*
	 * select_task_rq: p->pi_lock
	 * sched_exec: p->pi_lock
	 */
	صحیح  (*select_task_rq)(ساخت task_struct *p, صحیح task_cpu, صحیح flags);

	/*
	 * set_task_cpu: p->pi_lock || rq->lock (ttwu like)
	 */
	خالی (*migrate_task_rq)(ساخت task_struct *p, صحیح new_cpu);

	/*
	 * ttwu_do_activate: rq->lock
	 * wake_up_new_task: task_rq_lock
	 */
	خالی (*task_woken)(ساخت rq *this_rq, ساخت task_struct *task);

	/*
	 * do_set_cpus_allowed: task_rq_lock + sched_change
	 */
	خالی (*سیٹ_cpus_allowed)(ساخت task_struct *p, ساخت affinity_context *ctx);

	/*
	 * sched_set_rq_{on,off}line: rq->lock
	 */
	خالی (*rq_online)(ساخت rq *rq);
	خالی (*rq_offline)(ساخت rq *rq);

	/*
	 * push_cpu_stop: p->pi_lock && rq->lock
	 */
	ساخت rq *(*ڈھونڈو_تالا_rq)(ساخت task_struct *p, ساخت rq *rq);

	/*
	 * hrtick: rq->lock
	 * sched_tick: rq->lock
	 * sched_tick_remote: rq->lock
	 */
	خالی (*task_tick)(ساخت rq *rq, ساخت task_struct *p, صحیح queued);
	/*
	 * sched_cgroup_fork: p->pi_lock
	 */
	خالی (*task_fork)(ساخت task_struct *p);
	/*
	 * finish_task_switch: no locks
	 */
	خالی (*task_dead)(ساخت task_struct *p);

	/*
	 * sched_change
	 */
	خالی (*switching_from)(ساخت rq *this_rq, ساخت task_struct *task);
	خالی (*switched_from) (ساخت rq *this_rq, ساخت task_struct *task);
	خالی (*switching_مقصد)  (ساخت rq *this_rq, ساخت task_struct *task);
	خالی (*switched_مقصد)   (ساخت rq *this_rq, ساخت task_struct *task);
	u64  (*get_prio)     (ساخت rq *this_rq, ساخت task_struct *task);
	خالی (*prio_بدلا) (ساخت rq *this_rq, ساخت task_struct *task,
			      u64 oldprio);

	/*
	 * set_load_weight: task_rq_lock + sched_change
	 * __setscheduler_parms: task_rq_lock + sched_change
	 */
	خالی (*reweight_task)(ساخت rq *this_rq, ساخت task_struct *task,
			      ثابت ساخت بھرو_وزن *lw);

	/*
	 * sched_rr_get_interval: task_rq_lock
	 */
	غیرعلامتی صحیح (*get_rr_interval)(ساخت rq *rq,
					ساخت task_struct *task);

	/*
	 * task_sched_runtime: task_rq_lock
	 */
	خالی (*update_curr)(ساخت rq *rq);

#اگر_تعریف_شدہ CONFIG_FAIR_گروہ_SCHED
	/*
	 * sched_change_group: task_rq_lock + sched_change
	 */
	خالی (*task_change_گروہ)(ساخت task_struct *p);
#اگر_ختم

#اگر_تعریف_شدہ CONFIG_SCHED_CORE
	/*
	 * pick_next_task: rq->lock
	 * try_steal_cookie: rq->lock (double)
	 */
	صحیح (*task_is_throttled)(ساخت task_struct *p, صحیح cpu);
#اگر_ختم
};

ساکن اندرونی خالی put_پچھلا_task(ساخت rq *rq, ساخت task_struct *پچھلا)
{
	WARN_ON_ONCE(rq->donor != پچھلا);
	پچھلا->sched_class->put_پچھلا_task(rq, پچھلا, تہی);
}

ساکن اندرونی خالی سیٹ_اگلا_task(ساخت rq *rq, ساخت task_struct *اگلا)
{
	اگلا->sched_class->سیٹ_اگلا_task(rq, اگلا, SNT_معمول);
}

ساکن اندرونی خالی
__put_prev_set_next_dl_server(ساخت rq *rq,
			      ساخت task_struct *پچھلا,
			      ساخت task_struct *اگلا)
{
	پچھلا->dl_server = تہی;
	اگلا->dl_server = rq->dl_server;
	rq->dl_server = تہی;
}

ساکن اندرونی خالی put_پچھلا_سیٹ_اگلا_task(ساخت rq *rq,
					  ساخت task_struct *پچھلا,
					  ساخت task_struct *اگلا)
{
	WARN_ON_ONCE(rq->donor != پچھلا);

	__put_prev_set_next_dl_server(rq, پچھلا, اگلا);

	اگر (اگلا == پچھلا) {
		اگلا->sched_class->سیٹ_اگلا_task(rq, اگلا, SNT_REPICK);
		واپس;
	}

	پچھلا->sched_class->put_پچھلا_task(rq, پچھلا, اگلا);
	اگلا->sched_class->سیٹ_اگلا_task(rq, اگلا, SNT_PICK);
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
#تعریف_کرو DEFINE_SCHED_CLASS(name) \
ثابت ساخت sched_class name##_sched_class \
	__aligned(__alignof__(ساخت sched_class)) \
	__section("__" #name "_sched_class")

/* Defined in include/asm-generic/vmlinux.lds.h */
خارجی ساخت sched_class __sched_class_highest[];
خارجی ساخت sched_class __sched_class_lowest[];

خارجی ثابت ساخت sched_class stop_sched_class;
خارجی ثابت ساخت sched_class dl_sched_class;
خارجی ثابت ساخت sched_class rt_sched_class;
خارجی ثابت ساخت sched_class fair_sched_class;
خارجی ثابت ساخت sched_class idle_sched_class;

/*
 * Iterate only active classes. SCX can take over all fair tasks or be
 * completely disabled. If the former, skip fair. If the latter, skip SCX.
 */
ساکن اندرونی ثابت ساخت sched_class *اگلا_فعال_class(ثابت ساخت sched_class *class)
{
	class++;
#اگر_تعریف_شدہ CONFIG_SCHED_CLASS_EXT
	اگر (scx_switched_all() && class == &fair_sched_class)
		class++;
	اگر (!scx_enabled() && class == &ext_sched_class)
		class++;
#اگر_ختم
	واپس class;
}

#تعریف_کرو for_class_range(class, _from, مقصد_2) \
	برائے (class = (_from); class < (مقصد_2); class++)

#تعریف_کرو for_each_class(class) \
	for_class_range(class, __sched_class_highest, __sched_class_lowest)

#تعریف_کرو for_فعال_class_range(class, _from, مقصد_2)				\
	برائے (class = (_from); class != (مقصد_2); class = اگلا_فعال_class(class))

#تعریف_کرو for_each_فعال_class(class)						\
	for_فعال_class_range(class, __sched_class_highest, __sched_class_lowest)

#تعریف_کرو sched_class_above(_a, _b)	((_a) < (_b))

ساکن اندرونی خالی rq_modified_begin(ساخت rq *rq, ثابت ساخت sched_class *class)
{
	اگر (sched_class_above(rq->اگلا_class, class))
		rq->اگلا_class = class;
}

ساکن اندرونی bool rq_modified_above(ساخت rq *rq, ثابت ساخت sched_class *class)
{
	واپس sched_class_above(rq->اگلا_class, class);
}

ساکن اندرونی bool sched_stop_runnable(ساخت rq *rq)
{
	واپس rq->stop && task_on_rq_queued(rq->stop);
}

ساکن اندرونی bool sched_dl_runnable(ساخت rq *rq)
{
	واپس rq->dl.dl_nr_running > 0;
}

ساکن اندرونی bool sched_rt_runnable(ساخت rq *rq)
{
	واپس rq->rt.rt_queued > 0;
}

ساکن اندرونی bool sched_fair_runnable(ساخت rq *rq)
{
	واپس rq->cfs.nr_queued > 0;
}

خارجی ساخت task_struct *pick_task_fair(ساخت rq *rq, ساخت rq_flags *rf);
خارجی ساخت task_struct *pick_task_idle(ساخت rq *rq, ساخت rq_flags *rf);

#تعریف_کرو SCA_CHECK		0x01
#تعریف_کرو SCA_MIGRATE_DISABLE	0x02
#تعریف_کرو SCA_MIGRATE_ENABLE	0x04
#تعریف_کرو SCA_USER		0x08

خارجی خالی update_گروہ_استعداد(ساخت sched_domain *sd, صحیح cpu);

خارجی خالی sched_balance_trigger(ساخت rq *rq);

خارجی صحیح __set_cpus_allowed_ptr(ساخت task_struct *p, ساخت affinity_context *ctx);
خارجی خالی سیٹ_cpus_allowed_common(ساخت task_struct *p, ساخت affinity_context *ctx);

ساکن اندرونی bool task_allowed_on_cpu(ساخت task_struct *p, صحیح cpu)
{
	/* When not in the task's cpumask, no point in looking further. */
	اگر (!cpumask_test_cpu(cpu, p->cpus_ptr))
		واپس false;

	/* Can @cpu run a user thread? */
	اگر (!(p->flags & PF_KTHREAD) && !task_cpu_possible(cpu, p))
		واپس false;

	واپس true;
}

ساکن اندرونی cpumask_t *alloc_user_cpus_ptr(صحیح گرہ)
{
	/*
	 * See set_cpus_allowed_force() above for the rcu_head usage.
	 */
	صحیح سائز = max_t(صحیح, cpumask_سائز(), حجم(ساخت rcu_سر));

	واپس kmalloc_گرہ(سائز, GFP_KERNEL, گرہ);
}

ساکن اندرونی ساخت task_struct *get_ڈالو_task(ساخت rq *rq)
{
	ساخت task_struct *p = rq->donor;

	lockdep_assert_rq_held(rq);

	اگر (rq->ڈالو_busy)
		واپس تہی;

	اگر (p->nr_cpus_allowed == 1)
		واپس تہی;

	اگر (p->migration_disabled)
		واپس تہی;

	rq->ڈالو_busy = true;
	واپس get_task_struct(p);
}

خارجی صحیح ڈالو_cpu_stop(خالی *arg);

#اگر_تعریف_شدہ CONFIG_CPU_IDLE

ساکن اندرونی خالی idle_سیٹ_state(ساخت rq *rq,
				  ساخت cpuidle_state *idle_state)
{
	rq->idle_state = idle_state;
}

ساکن اندرونی ساخت cpuidle_state *idle_get_state(ساخت rq *rq)
{
	lockdep_assert(rcu_read_تالا_any_held());

	واپس rq->idle_state;
}

#ورنہ /* !CONFIG_CPU_IDLE: */

ساکن اندرونی خالی idle_سیٹ_state(ساخت rq *rq,
				  ساخت cpuidle_state *idle_state)
{
}

ساکن اندرونی ساخت cpuidle_state *idle_get_state(ساخت rq *rq)
{
	واپس تہی;
}

#اگر_ختم /* !CONFIG_CPU_IDLE */

خارجی خالی جدول_وقت_idle(خالی);
asmlinkage خالی جدول_وقت_user(خالی);

خارجی خالی sysrq_sched_debug_show(خالی);
خارجی خالی sched_init_granularity(خالی);
خارجی خالی update_max_interval(خالی);

خارجی خالی init_sched_dl_class(خالی);
خارجی خالی init_sched_rt_class(خالی);
خارجی خالی init_sched_fair_class(خالی);

خارجی خالی resched_curr(ساخت rq *rq);
خارجی خالی resched_curr_lazy(ساخت rq *rq);
خارجی خالی resched_cpu(صحیح cpu);

خارجی خالی init_rt_bandwidth(ساخت rt_bandwidth *rt_b, u64 period, u64 runtime);
خارجی bool sched_rt_bandwidth_account(ساخت rt_rq *rt_rq);

خارجی خالی init_dl_entity(ساخت sched_dl_entity *dl_se);

خارجی خالی init_cfs_throttle_work(ساخت task_struct *p);

#تعریف_کرو BW_سرکاؤ		20
#تعریف_کرو BW_UNIT			(1 << BW_سرکاؤ)
#تعریف_کرو تناسب_سرکاؤ		8
#تعریف_کرو MAX_BW_بٹ		(64 - BW_سرکاؤ)
#تعریف_کرو MAX_BW			((1ULL << MAX_BW_بٹ) - 1)

خارجی u64 مقصد_تناسب(u64 period, u64 runtime);

خارجی خالی init_entity_runnable_اوسط_قدر(ساخت sched_entity *se);
خارجی خالی post_init_entity_util_avg(ساخت task_struct *p);

#اگر_تعریف_شدہ CONFIG_NO_HZ_بھرا
خارجی bool sched_can_stop_tick(ساخت rq *rq);
خارجی صحیح __init sched_tick_offload_init(خالی);

/*
 * Tick may be needed by tasks in the runqueue depending on their policy and
 * requirements. If tick is needed, lets send the target an IPI to kick it out of
 * nohz mode if necessary.
 */
ساکن اندرونی خالی sched_update_tick_dependency(ساخت rq *rq)
{
	صحیح cpu = cpu_کا(rq);

	اگر (!tick_nohz_بھرا_cpu(cpu))
		واپس;

	اگر (sched_can_stop_tick(rq))
		tick_nohz_dep_clear_cpu(cpu, TICK_DEP_ثنائی_ہندسہ_SCHED);
	ورنہ
		tick_nohz_dep_سیٹ_cpu(cpu, TICK_DEP_ثنائی_ہندسہ_SCHED);
}
#ورنہ /* !CONFIG_NO_HZ_FULL: */
ساکن اندرونی صحیح sched_tick_offload_init(خالی) { واپس 0; }
ساکن اندرونی خالی sched_update_tick_dependency(ساخت rq *rq) { }
#اگر_ختم /* !CONFIG_NO_HZ_FULL */

ساکن اندرونی خالی جوڑو_nr_running(ساخت rq *rq, غیرعلامتی گنتی)
{
	غیرعلامتی پچھلا_nr = rq->nr_running;

	rq->nr_running = پچھلا_nr + گنتی;
	اگر (trace_sched_update_nr_running_tp_enabled()) {
		call_trace_sched_update_nr_running(rq, گنتی);
	}

	اگر (پچھلا_nr < 2 && rq->nr_running >= 2)
		سیٹ_rd_overloaded(rq->rd, 1);

	sched_update_tick_dependency(rq);
}

ساکن اندرونی خالی sub_nr_running(ساخت rq *rq, غیرعلامتی گنتی)
{
	rq->nr_running -= گنتی;
	اگر (trace_sched_update_nr_running_tp_enabled()) {
		call_trace_sched_update_nr_running(rq, -گنتی);
	}

	/* Check if we still need preemption */
	sched_update_tick_dependency(rq);
}

ساکن اندرونی خالی __block_task(ساخت rq *rq, ساخت task_struct *p)
{
	اگر (p->sched_contributes_مقصد_بھرو)
		rq->nr_uninterruptible++;

	اگر (p->اندر_iowait) {
		atomic_inc(&rq->nr_iowait);
		delayacct_blkio_آغاز();
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

خارجی خالی activate_task(ساخت rq *rq, ساخت task_struct *p, صحیح flags);
خارجی خالی deactivate_task(ساخت rq *rq, ساخت task_struct *p, صحیح flags);

خارجی خالی wakeup_preempt(ساخت rq *rq, ساخت task_struct *p, صحیح flags);

/*
 * attach_task() -- attach the task detached by detach_task() to its new rq.
 */
ساکن اندرونی خالی attach_task(ساخت rq *rq, ساخت task_struct *p)
{
	lockdep_assert_rq_held(rq);

	WARN_ON_ONCE(task_rq(p) != rq);
	activate_task(rq, p, قطار_میں_ڈالو_NOCLOCK);
	wakeup_preempt(rq, p, 0);
}

/*
 * attach_one_task() -- attaches the task returned from detach_one_task() to
 * its new rq.
 */
ساکن اندرونی خالی attach_one_task(ساخت rq *rq, ساخت task_struct *p)
{
	guard(rq_تالا)(rq);
	update_rq_clock(rq);
	attach_task(rq, p);
}

/*
 * __attach_tasks() - attaches a list of tasks (using se.group_node) to
 * the new rq
 */
ساکن اندرونی خالی __attach_tasks(ساخت rq *rq, ساخت list_سر *tasks)
{
	guard(rq_تالا)(rq);
	update_rq_clock(rq);

	جب (!list_خالی_خانے(tasks)) {
		ساخت task_struct *p;

		p = list_پہلا_اندراج(tasks, ساخت task_struct, se.گروہ_گرہ);
		list_del_init(&p->se.گروہ_گرہ);

		attach_task(rq, p);
	}
}

#اگر_تعریف_شدہ CONFIG_PREEMPT_RT
# define SCHED_NR_MIGRATE_BREAK 8
#ورنہ
# define SCHED_NR_MIGRATE_BREAK 32
#اگر_ختم

خارجی __read_mostly غیرعلامتی صحیح sysctl_sched_nr_migrate;
خارجی __read_mostly غیرعلامتی صحیح sysctl_sched_migration_لاگت;

خارجی غیرعلامتی صحیح sysctl_sched_بنیاد_قاش;

خارجی صحیح sysctl_resched_latency_warn_ms;
خارجی صحیح sysctl_resched_latency_warn_once;

خارجی غیرعلامتی صحیح sysctl_sched_tunable_scaling;

خارجی غیرعلامتی صحیح sysctl_numa_balancing_جائزہ_delay;
خارجی غیرعلامتی صحیح sysctl_numa_balancing_جائزہ_period_min;
خارجی غیرعلامتی صحیح sysctl_numa_balancing_جائزہ_period_max;
خارجی غیرعلامتی صحیح sysctl_numa_balancing_جائزہ_سائز;
خارجی غیرعلامتی صحیح sysctl_numa_balancing_hot_حد;

#اگر_تعریف_شدہ CONFIG_SCHED_HRTICK

/*
 * Use hrtick when:
 *  - enabled by features
 *  - hrtimer is actually high res
 */
ساکن اندرونی bool hrtick_enabled(ساخت rq *rq)
{
	واپس cpu_فعال(cpu_کا(rq)) && hrtimer_highres_enabled();
}

ساکن اندرونی bool hrtick_enabled_fair(ساخت rq *rq)
{
	واپس sched_feat(HRTICK) && hrtick_enabled(rq);
}

ساکن اندرونی bool hrtick_enabled_dl(ساخت rq *rq)
{
	واپس sched_feat(HRTICK_DL) && hrtick_enabled(rq);
}

خارجی خالی hrtick_آغاز(ساخت rq *rq, u64 delay);
ساکن اندرونی bool hrtick_فعال(ساخت rq *rq)
{
	واپس hrtimer_فعال(&rq->hrtick_timer);
}

#ورنہ /* !CONFIG_SCHED_HRTICK: */
ساکن اندرونی bool hrtick_enabled_fair(ساخت rq *rq) { واپس false; }
ساکن اندرونی bool hrtick_enabled_dl(ساخت rq *rq) { واپس false; }
ساکن اندرونی bool hrtick_enabled(ساخت rq *rq) { واپس false; }
#اگر_ختم /* !CONFIG_SCHED_HRTICK */

#اگر_غیر_تعریف_شدہ arch_scale_freq_tick
ساکن __always_inline خالی arch_scale_freq_tick(خالی) { }
#اگر_ختم

#اگر_غیر_تعریف_شدہ arch_scale_freq_استعداد
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
ساکن __always_inline
غیرعلامتی طویل arch_scale_freq_استعداد(صحیح cpu)
{
	واپس SCHED_استعداد_SCALE;
}
#اگر_ختم

/*
 * In double_lock_balance()/double_rq_lock(), we use raw_spin_rq_lock() to
 * acquire rq lock instead of rq_lock(). So at the end of these two functions
 * we need to call double_rq_clock_clear_update() to clear RQCF_UPDATED of
 * rq->clock_update_flags to avoid the WARN_DOUBLE_CLOCK warning.
 */
ساکن اندرونی خالی double_rq_clock_clear_update(ساخت rq *rq1, ساخت rq *rq2)
{
	rq1->clock_update_flags &= (RQCF_REQ_SKIP|RQCF_ACT_SKIP);
	rq2->clock_update_flags &= (RQCF_REQ_SKIP|RQCF_ACT_SKIP);
}

#تعریف_کرو DEFINE_تالا_GUARD_2(name, type, تالا_2, _unlock, ...)				\
__DEFINE_UNLOCK_GUARD(name, type, _unlock, type *lock2; __VA_ARGS__)			\
ساکن اندرونی class_##name##_t class_##name##_constructor(type *تالا, type *lock2)	\
	__no_context_analysis								\
{ class_##name##_t _t = { .تالا = تالا, .lock2 = lock2 }, *_T = &_t;			\
  تالا_2; واپس _t; }
#تعریف_کرو DECLARE_تالا_GUARD_2_ATTRS(_name, تالا_2, _unlock1, _unlock2)			\
ساکن اندرونی class_##_name##_t class_##_name##_constructor(تالا_3##_name##_t *_T1,	\
							    تالا_3##_name##_t *_T2) تالا_2; \
ساکن __always_inline خالی __class_##_name##_cleanup_ctx1(class_##_name##_t **_T1)	\
	__no_context_analysis _unlock1 { }						\
ساکن __always_inline خالی __class_##_name##_cleanup_ctx2(class_##_name##_t **_T2)	\
	__no_context_analysis _unlock2 { }
#تعریف_کرو WITH_تالا_GUARD_2_ATTRS(_name, _T1, _T2)					\
	class_##_name##_constructor(_T1, _T2),						\
	*__UNIQUE_ID(unlock1) __cleanup(__class_##_name##_cleanup_ctx1) = (خالی *)(_T1),\
	*__UNIQUE_ID(unlock2) __cleanup(__class_##_name##_cleanup_ctx2) = (خالی *)(_T2)

ساکن اندرونی bool rq_ترتیب_less(ساخت rq *rq1, ساخت rq *rq2)
{
#اگر_تعریف_شدہ CONFIG_SCHED_CORE
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
	اگر (rq1->core->cpu < rq2->core->cpu)
		واپس true;
	اگر (rq1->core->cpu > rq2->core->cpu)
		واپس false;

	/*
	 * __sched_core_flip() relies on SMT having cpu-id lock order.
	 */
#اگر_ختم /* CONFIG_SCHED_CORE */
	واپس rq1->cpu < rq2->cpu;
}

خارجی خالی double_rq_تالا(ساخت rq *rq1, ساخت rq *rq2)
	__acquires(__rq_lockp(rq1), __rq_lockp(rq2));

#اگر_تعریف_شدہ CONFIG_PREEMPTION

/*
 * fair double_lock_balance: Safely acquires both rq->locks in a fair
 * way at the expense of forcing extra atomic operations in all
 * invocations.  This assures that the double_lock is acquired using the
 * same underlying policy as the spinlock_t on this architecture, which
 * reduces latency compared to the unfair variant below.  However, it
 * also adds more overhead and therefore may reduce throughput.
 */
ساکن اندرونی صحیح double_تالا_balance_2(ساخت rq *this_rq, ساخت rq *busiest)
	__must_hold(__rq_lockp(this_rq))
	__acquires(__rq_lockp(busiest))
{
	raw_spin_rq_unlock(this_rq);
	double_rq_تالا(this_rq, busiest);

	واپس 1;
}

#ورنہ /* !CONFIG_PREEMPTION: */
/*
 * Unfair double_lock_balance: Optimizes throughput at the expense of
 * latency by eliminating extra atomic operations when the locks are
 * already in proper order on entry.  This favors lower CPU-ids and will
 * grant the double lock to lower CPUs over higher ids under contention,
 * regardless of entry order into the function.
 */
ساکن اندرونی صحیح double_تالا_balance_2(ساخت rq *this_rq, ساخت rq *busiest)
	__must_hold(__rq_lockp(this_rq))
	__acquires(__rq_lockp(busiest))
{
	اگر (__rq_lockp(this_rq) == __rq_lockp(busiest)) {
		__acquire(__rq_lockp(busiest)); /* already held */
		double_rq_clock_clear_update(this_rq, busiest);
		واپس 0;
	}

	اگر (likely(raw_spin_rq_trylock(busiest))) {
		double_rq_clock_clear_update(this_rq, busiest);
		واپس 0;
	}

	اگر (rq_ترتیب_less(this_rq, busiest)) {
		raw_spin_rq_تالا_nested(busiest, SINGLE_گہرائی_NESTING);
		double_rq_clock_clear_update(this_rq, busiest);
		واپس 0;
	}

	raw_spin_rq_unlock(this_rq);
	double_rq_تالا(this_rq, busiest);

	واپس 1;
}

#اگر_ختم /* !CONFIG_PREEMPTION */

/*
 * double_lock_balance - lock the busiest runqueue, this_rq is locked already.
 */
ساکن اندرونی صحیح double_تالا_balance(ساخت rq *this_rq, ساخت rq *busiest)
	__must_hold(__rq_lockp(this_rq))
	__acquires(__rq_lockp(busiest))
{
	lockdep_assert_irqs_disabled();

	واپس double_تالا_balance_2(this_rq, busiest);
}

ساکن اندرونی خالی double_unlock_balance(ساخت rq *this_rq, ساخت rq *busiest)
	__releases(__rq_lockp(busiest))
{
	اگر (__rq_lockp(this_rq) != __rq_lockp(busiest))
		raw_spin_rq_unlock(busiest);
	ورنہ
		__release(__rq_lockp(busiest)); /* fake release */
	تالا_سیٹ_subclass(&__rq_lockp(this_rq)->dep_map, 0, _RET_IP_);
}

ساکن اندرونی خالی double_تالا(spinlock_t *l1, spinlock_t *l2)
	__acquires(l1, l2)
{
	اگر (l1 > l2)
		تبادلہ(l1, l2);

	spin_تالا(l1);
	spin_تالا_nested(l2, SINGLE_گہرائی_NESTING);
}

ساکن اندرونی خالی double_تالا_irq(spinlock_t *l1, spinlock_t *l2)
	__acquires(l1, l2)
{
	اگر (l1 > l2)
		تبادلہ(l1, l2);

	spin_تالا_irq(l1);
	spin_تالا_nested(l2, SINGLE_گہرائی_NESTING);
}

ساکن اندرونی خالی double_raw_تالا(raw_spinlock_t *l1, raw_spinlock_t *l2)
	__acquires(l1, l2)
{
	اگر (l1 > l2)
		تبادلہ(l1, l2);

	raw_spin_تالا(l1);
	raw_spin_تالا_nested(l2, SINGLE_گہرائی_NESTING);
}

ساکن اندرونی خالی double_raw_unlock(raw_spinlock_t *l1, raw_spinlock_t *l2)
	__releases(l1, l2)
{
	raw_spin_unlock(l1);
	raw_spin_unlock(l2);
}

DEFINE_تالا_GUARD_2(double_raw_spinlock, raw_spinlock_t,
		    double_raw_تالا(_T->تالا, _T->lock2),
		    double_raw_unlock(_T->تالا, _T->lock2))

DECLARE_تالا_GUARD_2_ATTRS(double_raw_spinlock,
			   __acquires(_T1, _T2),
			   __releases(*(raw_spinlock_t **)_T1),
			   __releases(*(raw_spinlock_t **)_T2));
#تعریف_کرو class_double_raw_spinlock_constructor(_T1, _T2) \
	WITH_تالا_GUARD_2_ATTRS(double_raw_spinlock, _T1, _T2)

/*
 * double_rq_unlock - safely unlock two runqueues
 *
 * Note this does not restore interrupts like task_rq_unlock,
 * you need to do so manually after calling.
 */
ساکن اندرونی خالی double_rq_unlock(ساخت rq *rq1, ساخت rq *rq2)
	__releases(__rq_lockp(rq1), __rq_lockp(rq2))
{
	اگر (__rq_lockp(rq1) != __rq_lockp(rq2))
		raw_spin_rq_unlock(rq2);
	ورنہ
		__release(__rq_lockp(rq2)); /* fake release */
	raw_spin_rq_unlock(rq1);
}

خارجی خالی سیٹ_rq_online (ساخت rq *rq);
خارجی خالی سیٹ_rq_offline(ساخت rq *rq);

خارجی bool sched_smp_initialized;

DEFINE_تالا_GUARD_2(double_rq_تالا, ساخت rq,
		    double_rq_تالا(_T->تالا, _T->lock2),
		    double_rq_unlock(_T->تالا, _T->lock2))

خارجی ساخت sched_entity *__pick_root_entity(ساخت cfs_rq *cfs_rq);
خارجی ساخت sched_entity *__pick_first_entity(ساخت cfs_rq *cfs_rq);
خارجی ساخت sched_entity *__pick_last_entity(ساخت cfs_rq *cfs_rq);

خارجی bool sched_debug_verbose;

خارجی خالی print_cfs_stats(ساخت seq_file *m, صحیح cpu);
خارجی خالی print_rt_stats(ساخت seq_file *m, صحیح cpu);
خارجی خالی print_dl_stats(ساخت seq_file *m, صحیح cpu);
خارجی خالی print_cfs_rq(ساخت seq_file *m, صحیح cpu, ساخت cfs_rq *cfs_rq);
خارجی خالی print_rt_rq(ساخت seq_file *m, صحیح cpu, ساخت rt_rq *rt_rq);
خارجی خالی print_dl_rq(ساخت seq_file *m, صحیح cpu, ساخت dl_rq *dl_rq);

خارجی خالی resched_latency_warn(صحیح cpu, u64 latency);

#اگر_تعریف_شدہ CONFIG_NUMA_BALANCING
خارجی خالی show_numa_stats(ساخت task_struct *p, ساخت seq_file *m);
خارجی خالی
print_numa_stats(ساخت seq_file *m, صحیح گرہ, غیرعلامتی طویل tsf,
		 غیرعلامتی طویل tpf, غیرعلامتی طویل gsf, غیرعلامتی طویل gpf);
#اگر_ختم /* CONFIG_NUMA_BALANCING */

خارجی خالی init_cfs_rq(ساخت cfs_rq *cfs_rq);
خارجی خالی init_rt_rq(ساخت rt_rq *rt_rq);
خارجی خالی init_dl_rq(ساخت dl_rq *dl_rq);

خارجی خالی cfs_bandwidth_usage_inc(خالی);
خارجی خالی cfs_bandwidth_usage_dec(خالی);

#اگر_تعریف_شدہ CONFIG_NO_HZ_COMMON

#تعریف_کرو NOHZ_BALANCE_KICK_ثنائی_ہندسہ	0
#تعریف_کرو NOHZ_STATS_KICK_ثنائی_ہندسہ	1
#تعریف_کرو NOHZ_NEWILB_KICK_ثنائی_ہندسہ	2
#تعریف_کرو NOHZ_اگلا_KICK_ثنائی_ہندسہ	3

/* Run sched_balance_domains() */
#تعریف_کرو NOHZ_BALANCE_KICK	ثنائی_ہندسہ(NOHZ_BALANCE_KICK_ثنائی_ہندسہ)
/* Update blocked load */
#تعریف_کرو NOHZ_STATS_KICK		ثنائی_ہندسہ(NOHZ_STATS_KICK_ثنائی_ہندسہ)
/* Update blocked load when entering idle */
#تعریف_کرو NOHZ_NEWILB_KICK	ثنائی_ہندسہ(NOHZ_NEWILB_KICK_ثنائی_ہندسہ)
/* Update nohz.next_balance */
#تعریف_کرو NOHZ_اگلا_KICK		ثنائی_ہندسہ(NOHZ_اگلا_KICK_ثنائی_ہندسہ)

#تعریف_کرو NOHZ_KICK_نقاب		(NOHZ_BALANCE_KICK | NOHZ_STATS_KICK | NOHZ_اگلا_KICK)

#تعریف_کرو nohz_flags(cpu)		(&cpu_rq(cpu)->nohz_flags)

خارجی خالی nohz_balance_exit_idle(ساخت rq *rq);
#ورنہ /* !CONFIG_NO_HZ_COMMON: */
ساکن اندرونی خالی nohz_balance_exit_idle(ساخت rq *rq) { }
#اگر_ختم /* !CONFIG_NO_HZ_COMMON */

#اگر_تعریف_شدہ CONFIG_NO_HZ_COMMON
خارجی خالی nohz_تسلسل_idle_balance(صحیح cpu);
#ورنہ
ساکن اندرونی خالی nohz_تسلسل_idle_balance(صحیح cpu) { }
#اگر_ختم

#شامل "stats.h"

#اگر defined(CONFIG_SCHED_CORE) && defined(CONFIG_SCHEDSTATS)

خارجی خالی __sched_core_account_forceidle(ساخت rq *rq);

ساکن اندرونی خالی sched_core_account_forceidle(ساخت rq *rq)
{
	اگر (schedstat_enabled())
		__sched_core_account_forceidle(rq);
}

خارجی خالی __sched_core_tick(ساخت rq *rq);

ساکن اندرونی خالی sched_core_tick(ساخت rq *rq)
{
	اگر (sched_core_enabled(rq) && schedstat_enabled())
		__sched_core_tick(rq);
}

#ورنہ /* !(CONFIG_SCHED_CORE && CONFIG_SCHEDSTATS): */

ساکن اندرونی خالی sched_core_account_forceidle(ساخت rq *rq) { }

ساکن اندرونی خالی sched_core_tick(ساخت rq *rq) { }

#اگر_ختم /* !(CONFIG_SCHED_CORE && CONFIG_SCHEDSTATS) */

#اگر_تعریف_شدہ CONFIG_IRQ_وقت_ACCOUNTING

ساخت irqtime {
	u64			کل;
	u64			tick_delta;
	u64			irq_آغاز_وقت;
	ساخت u64_stats_sync	sync;
};

DECLARE_فی_CPU(ساخت irqtime, cpu_irqtime);
DECLARE_STATIC_کلید_FALSE(sched_clock_irqtime);

ساکن اندرونی صحیح irqtime_enabled(خالی)
{
	واپس static_branch_likely(&sched_clock_irqtime);
}

/*
 * Returns the irqtime minus the softirq time computed by ksoftirqd.
 * Otherwise ksoftirqd's sum_exec_runtime is subtracted its own runtime
 * and never move forward.
 */
ساکن اندرونی u64 irq_وقت_read(صحیح cpu)
{
	ساخت irqtime *irqtime = &فی_cpu(cpu_irqtime, cpu);
	غیرعلامتی صحیح seq;
	u64 کل;

	کرو {
		seq = __u64_stats_fetch_begin(&irqtime->sync);
		کل = irqtime->کل;
	} جب (__u64_stats_fetch_retry(&irqtime->sync, seq));

	واپس کل;
}

#ورنہ /* !CONFIG_IRQ_TIME_ACCOUNTING: */

ساکن اندرونی صحیح irqtime_enabled(خالی)
{
	واپس 0;
}

#اگر_ختم /* !CONFIG_IRQ_TIME_ACCOUNTING */

#اگر_تعریف_شدہ CONFIG_CPU_FREQ

DECLARE_فی_CPU(ساخت update_util_ڈیٹا __rcu *, cpufreq_update_util_ڈیٹا);

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
ساکن اندرونی خالی cpufreq_update_util(ساخت rq *rq, غیرعلامتی صحیح flags)
{
	ساخت update_util_ڈیٹا *ڈیٹا;

	ڈیٹا = rcu_dereference_sched(*فی_cpu_ptr(&cpufreq_update_util_ڈیٹا,
						  cpu_کا(rq)));
	اگر (ڈیٹا)
		ڈیٹا->func(ڈیٹا, rq_clock(rq), flags);
}
#ورنہ /* !CONFIG_CPU_FREQ: */
ساکن اندرونی خالی cpufreq_update_util(ساخت rq *rq, غیرعلامتی صحیح flags) { }
#اگر_ختم /* !CONFIG_CPU_FREQ */

#اگر_تعریف_شدہ arch_scale_freq_استعداد
# ifndef arch_scale_freq_invariant
#  define arch_scale_freq_invariant()	true
# endif
#ورنہ
# define arch_scale_freq_invariant()	false
#اگر_ختم

غیرعلامتی طویل effective_cpu_util(صحیح cpu, غیرعلامتی طویل util_cfs,
				 غیرعلامتی طویل *min,
				 غیرعلامتی طویل *max);

غیرعلامتی طویل sugov_effective_cpu_perf(صحیح cpu, غیرعلامتی طویل actual,
				 غیرعلامتی طویل min,
				 غیرعلامتی طویل max);


/*
 * Verify the fitness of task @p to run on @cpu taking into account the
 * CPU original capacity and the runtime/deadline ratio of the task.
 *
 * The function will return true if the original capacity of @cpu is
 * greater than or equal to task's deadline density right shifted by
 * (BW_SHIFT - SCHED_CAPACITY_SHIFT) and false otherwise.
 */
ساکن اندرونی bool dl_task_سماتا_استعداد(ساخت task_struct *p, صحیح cpu)
{
	غیرعلامتی طویل cap = arch_scale_cpu_استعداد(cpu);

	واپس cap >= p->dl.dl_density >> (BW_سرکاؤ - SCHED_استعداد_سرکاؤ);
}

ساکن اندرونی غیرعلامتی طویل cpu_bw_dl(ساخت rq *rq)
{
	واپس (rq->dl.running_bw * SCHED_استعداد_SCALE) >> BW_سرکاؤ;
}

ساکن اندرونی غیرعلامتی طویل cpu_util_dl(ساخت rq *rq)
{
	واپس READ_ONCE(rq->avg_dl.util_avg);
}


خارجی غیرعلامتی طویل cpu_util_cfs(صحیح cpu);
خارجی غیرعلامتی طویل cpu_util_cfs_boost(صحیح cpu);

ساکن اندرونی غیرعلامتی طویل cpu_util_rt(ساخت rq *rq)
{
	واپس READ_ONCE(rq->avg_rt.util_avg);
}

#اگر_تعریف_شدہ CONFIG_UCLAMP_TASK

غیرعلامتی طویل uclamp_eff_قدر(ساخت task_struct *p, شمار uclamp_id clamp_id);

/*
 * When uclamp is compiled in, the aggregation at rq level is 'turned off'
 * by default in the fast path and only gets turned on once userspace performs
 * an operation that requires it.
 *
 * Returns true if userspace opted-in to use uclamp and aggregation at rq level
 * hence is active.
 */
ساکن اندرونی bool uclamp_is_مستعمل(خالی)
{
	واپس static_branch_likely(&sched_uclamp_مستعمل);
}

/*
 * Enabling static branches would get the cpus_read_lock(),
 * check whether uclamp_is_used before enable it to avoid always
 * calling cpus_read_lock(). Because we never disable this
 * static key once enable it.
 */
ساکن اندرونی خالی sched_uclamp_enable(خالی)
{
	اگر (!uclamp_is_مستعمل())
		static_branch_enable(&sched_uclamp_مستعمل);
}

ساکن اندرونی غیرعلامتی طویل uclamp_rq_get(ساخت rq *rq,
					  شمار uclamp_id clamp_id)
{
	واپس READ_ONCE(rq->uclamp[clamp_id].قدر);
}

ساکن اندرونی خالی uclamp_rq_سیٹ(ساخت rq *rq, شمار uclamp_id clamp_id,
				 غیرعلامتی صحیح قدر)
{
	WRITE_ONCE(rq->uclamp[clamp_id].قدر, قدر);
}

ساکن اندرونی bool uclamp_rq_is_idle(ساخت rq *rq)
{
	واپس rq->uclamp_flags & UCLAMP_FLAG_IDLE;
}

/* Is the rq being capped/throttled by uclamp_max? */
ساکن اندرونی bool uclamp_rq_is_capped(ساخت rq *rq)
{
	غیرعلامتی طویل rq_util;
	غیرعلامتی طویل max_util;

	اگر (!uclamp_is_مستعمل())
		واپس false;

	rq_util = cpu_util_cfs(cpu_کا(rq)) + cpu_util_rt(rq);
	max_util = READ_ONCE(rq->uclamp[UCLAMP_MAX].قدر);

	واپس max_util != SCHED_استعداد_SCALE && rq_util >= max_util;
}

#تعریف_کرو for_each_clamp_id(clamp_id) \
	برائے ((clamp_id) = 0; (clamp_id) < UCLAMP_CNT; (clamp_id)++)

خارجی غیرعلامتی صحیح sysctl_sched_uclamp_util_min_rt_default;


ساکن اندرونی غیرعلامتی صحیح uclamp_none(شمار uclamp_id clamp_id)
{
	اگر (clamp_id == UCLAMP_MIN)
		واپس 0;
	واپس SCHED_استعداد_SCALE;
}

/* Integer rounded range for each bucket */
#تعریف_کرو UCLAMP_بالٹی_DELTA DIV_باری_CLOSEST(SCHED_استعداد_SCALE, UCLAMP_BUCKETS)

ساکن اندرونی غیرعلامتی صحیح uclamp_بالٹی_id(غیرعلامتی صحیح clamp_قدر)
{
	واپس min_t(غیرعلامتی صحیح, clamp_قدر / UCLAMP_بالٹی_DELTA, UCLAMP_BUCKETS - 1);
}

ساکن اندرونی خالی
uclamp_se_سیٹ(ساخت uclamp_se *uc_se, غیرعلامتی صحیح قدر, bool user_defined)
{
	uc_se->قدر = قدر;
	uc_se->بالٹی_id = uclamp_بالٹی_id(قدر);
	uc_se->user_defined = user_defined;
}

#ورنہ /* !CONFIG_UCLAMP_TASK: */

ساکن اندرونی غیرعلامتی طویل
uclamp_eff_قدر(ساخت task_struct *p, شمار uclamp_id clamp_id)
{
	اگر (clamp_id == UCLAMP_MIN)
		واپس 0;

	واپس SCHED_استعداد_SCALE;
}

ساکن اندرونی bool uclamp_rq_is_capped(ساخت rq *rq) { واپس false; }

ساکن اندرونی bool uclamp_is_مستعمل(خالی)
{
	واپس false;
}

ساکن اندرونی خالی sched_uclamp_enable(خالی) {}

ساکن اندرونی غیرعلامتی طویل
uclamp_rq_get(ساخت rq *rq, شمار uclamp_id clamp_id)
{
	اگر (clamp_id == UCLAMP_MIN)
		واپس 0;

	واپس SCHED_استعداد_SCALE;
}

ساکن اندرونی خالی
uclamp_rq_سیٹ(ساخت rq *rq, شمار uclamp_id clamp_id, غیرعلامتی صحیح قدر)
{
}

ساکن اندرونی bool uclamp_rq_is_idle(ساخت rq *rq)
{
	واپس false;
}

#اگر_ختم /* !CONFIG_UCLAMP_TASK */

#اگر_تعریف_شدہ CONFIG_HAVE_SCHED_AVG_IRQ

ساکن اندرونی غیرعلامتی طویل cpu_util_irq(ساخت rq *rq)
{
	واپس READ_ONCE(rq->avg_irq.util_avg);
}

ساکن اندرونی
غیرعلامتی طویل scale_irq_استعداد(غیرعلامتی طویل util, غیرعلامتی طویل irq, غیرعلامتی طویل max)
{
	util *= (max - irq);
	util /= max;

	واپس util;

}

#ورنہ /* !CONFIG_HAVE_SCHED_AVG_IRQ: */

ساکن اندرونی غیرعلامتی طویل cpu_util_irq(ساخت rq *rq)
{
	واپس 0;
}

ساکن اندرونی
غیرعلامتی طویل scale_irq_استعداد(غیرعلامتی طویل util, غیرعلامتی طویل irq, غیرعلامتی طویل max)
{
	واپس util;
}

#اگر_ختم /* !CONFIG_HAVE_SCHED_AVG_IRQ */

خارجی خالی __setparam_fair(ساخت task_struct *p, ثابت ساخت sched_attr *attr);

#اگر defined(CONFIG_ENERGY_MODEL) && defined(CONFIG_CPU_FREQ_GOV_SCHEDUTIL)

#تعریف_کرو perf_domain_وسعت(pd) (مقصد_cpumask(((pd)->em_pd->cpus)))

DECLARE_STATIC_کلید_FALSE(sched_energy_present);

ساکن اندرونی bool sched_energy_enabled(خالی)
{
	واپس static_branch_unlikely(&sched_energy_present);
}

#ورنہ /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL): */

#تعریف_کرو perf_domain_وسعت(pd) تہی

ساکن اندرونی bool sched_energy_enabled(خالی) { واپس false; }

#اگر_ختم /* !(CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL) */

#اگر_تعریف_شدہ CONFIG_MEMBARRIER

/*
 * The scheduler provides memory barriers required by membarrier between:
 * - prior user-space memory accesses and store to rq->membarrier_state,
 * - store to rq->membarrier_state and following user-space memory accesses.
 * In the same way it provides those guarantees around store to rq->curr.
 */
ساکن اندرونی خالی membarrier_switch_mm(ساخت rq *rq,
					ساخت mm_struct *پچھلا_mm,
					ساخت mm_struct *اگلا_mm)
{
	صحیح membarrier_state;

	اگر (پچھلا_mm == اگلا_mm)
		واپس;

	membarrier_state = atomic_read(&اگلا_mm->membarrier_state);
	اگر (READ_ONCE(rq->membarrier_state) == membarrier_state)
		واپس;

	WRITE_ONCE(rq->membarrier_state, membarrier_state);
}

#ورنہ /* !CONFIG_MEMBARRIER: */

ساکن اندرونی خالی membarrier_switch_mm(ساخت rq *rq,
					ساخت mm_struct *پچھلا_mm,
					ساخت mm_struct *اگلا_mm)
{
}

#اگر_ختم /* !CONFIG_MEMBARRIER */

ساکن اندرونی bool is_فی_cpu_kthread(ساخت task_struct *p)
{
	اگر (!(p->flags & PF_KTHREAD))
		واپس false;

	اگر (p->nr_cpus_allowed != 1)
		واپس false;

	واپس true;
}

خارجی خالی swake_اوپر_all_locked(ساخت swait_قطار_سر *q);
خارجی خالی __prepare_to_swait(ساخت swait_قطار_سر *q, ساخت swait_قطار *wait);

خارجی صحیح try_مقصد_wake_اوپر(ساخت task_struct *tsk, غیرعلامتی صحیح state, صحیح wake_flags);

#اگر_تعریف_شدہ CONFIG_PREEMPT_DYNAMIC
خارجی صحیح preempt_dynamic_منوال;
خارجی صحیح sched_dynamic_منوال(ثابت حرف *str);
خارجی خالی sched_dynamic_update(صحیح منوال);
#اگر_ختم
خارجی ثابت حرف *preempt_modes[];

#اگر_تعریف_شدہ CONFIG_SCHED_MM_CID

ساکن __always_inline bool cid_on_cpu(غیرعلامتی صحیح cid)
{
	واپس cid & MM_CID_ONCPU;
}

ساکن __always_inline bool cid_اندر_transit(غیرعلامتی صحیح cid)
{
	واپس cid & MM_CID_TRANSIT;
}

ساکن __always_inline غیرعلامتی صحیح cpu_cid_مقصد_cid(غیرعلامتی صحیح cid)
{
	واپس cid & ~MM_CID_ONCPU;
}

ساکن __always_inline غیرعلامتی صحیح cid_مقصد_cpu_cid(غیرعلامتی صحیح cid)
{
	واپس cid | MM_CID_ONCPU;
}

ساکن __always_inline غیرعلامتی صحیح cid_مقصد_transit_cid(غیرعلامتی صحیح cid)
{
	واپس cid | MM_CID_TRANSIT;
}

ساکن __always_inline غیرعلامتی صحیح cid_from_transit_cid(غیرعلامتی صحیح cid)
{
	واپس cid & ~MM_CID_TRANSIT;
}

ساکن __always_inline bool cid_on_task(غیرعلامتی صحیح cid)
{
	/* True if none of the MM_CID_ONCPU, MM_CID_TRANSIT, MM_CID_UNSET bits is set */
	واپس cid < MM_CID_TRANSIT;
}

ساکن __always_inline خالی mm_drop_cid(ساخت mm_struct *mm, غیرعلامتی صحیح cid)
{
	clear_ثنائی_ہندسہ(cid, mm_cidmask(mm));
}

ساکن __always_inline خالی mm_unset_cid_on_task(ساخت task_struct *t)
{
	غیرعلامتی صحیح cid = t->mm_cid.cid;

	t->mm_cid.cid = MM_CID_UNSET;
	اگر (cid_on_task(cid))
		mm_drop_cid(t->mm, cid);
}

ساکن __always_inline خالی mm_drop_cid_on_cpu(ساخت mm_struct *mm, ساخت mm_cid_pcpu *pcp)
{
	/* Clear the ONCPU bit, but do not set UNSET in the per CPU storage */
	اگر (cid_on_cpu(pcp->cid)) {
		pcp->cid = cpu_cid_مقصد_cid(pcp->cid);
		mm_drop_cid(mm, pcp->cid);
	}
}

ساکن اندرونی غیرعلامتی صحیح __mm_get_cid(ساخت mm_struct *mm, غیرعلامتی صحیح max_cids)
{
	غیرعلامتی صحیح cid = ڈھونڈو_پہلا_zero_ثنائی_ہندسہ(mm_cidmask(mm), max_cids);

	اگر (cid >= max_cids)
		واپس MM_CID_UNSET;
	اگر (test_and_سیٹ_ثنائی_ہندسہ(cid, mm_cidmask(mm)))
		واپس MM_CID_UNSET;
	واپس cid;
}

ساکن اندرونی غیرعلامتی صحیح mm_get_cid(ساخت mm_struct *mm)
{
	غیرعلامتی صحیح cid = __mm_get_cid(mm, READ_ONCE(mm->mm_cid.max_cids));

	جب (cid == MM_CID_UNSET) {
		cpu_relax();
		cid = __mm_get_cid(mm, عدد_possible_cpus());
	}
	واپس cid;
}

ساکن اندرونی غیرعلامتی صحیح mm_cid_converge(ساخت mm_struct *mm, غیرعلامتی صحیح orig_cid,
					   غیرعلامتی صحیح max_cids)
{
	غیرعلامتی صحیح new_cid, cid = cpu_cid_مقصد_cid(orig_cid);

	/* Is it in the optimal CID space? */
	اگر (likely(cid < max_cids))
		واپس orig_cid;

	/* Try to find one in the optimal space. Otherwise keep the provided. */
	new_cid = __mm_get_cid(mm, max_cids);
	اگر (new_cid != MM_CID_UNSET) {
		mm_drop_cid(mm, cid);
		/* Preserve the ONCPU mode of the original CID */
		واپس new_cid | (orig_cid & MM_CID_ONCPU);
	}
	واپس orig_cid;
}

ساکن __always_inline خالی mm_cid_update_task_cid(ساخت task_struct *t, غیرعلامتی صحیح cid)
{
	اگر (t->mm_cid.cid != cid) {
		t->mm_cid.cid = cid;
		rseq_sched_سیٹ_ids_بدلا(t);
	}
}

ساکن __always_inline خالی mm_cid_update_pcpu_cid(ساخت mm_struct *mm, غیرعلامتی صحیح cid)
{
	__this_cpu_write(mm->mm_cid.pcpu->cid, cid);
}

ساکن __always_inline خالی mm_cid_from_cpu(ساخت task_struct *t, غیرعلامتی صحیح cpu_cid,
					    غیرعلامتی صحیح منوال)
{
	غیرعلامتی صحیح max_cids, tcid = t->mm_cid.cid;
	ساخت mm_struct *mm = t->mm;

	max_cids = READ_ONCE(mm->mm_cid.max_cids);
	/* Optimize for the common case where both have the ONCPU bit set */
	اگر (likely(cid_on_cpu(cpu_cid & tcid))) {
		اگر (likely(cpu_cid_مقصد_cid(cpu_cid) < max_cids)) {
			mm_cid_update_task_cid(t, cpu_cid);
			واپس;
		}
		/* Try to converge into the optimal CID space */
		cpu_cid = mm_cid_converge(mm, cpu_cid, max_cids);
	} ورنہ {
		/* Hand over or drop the task owned CID */
		اگر (cid_on_task(tcid)) {
			اگر (cid_on_cpu(cpu_cid))
				mm_unset_cid_on_task(t);
			ورنہ
				cpu_cid = cid_مقصد_cpu_cid(tcid);
		}
		/* Still nothing, allocate a new one */
		اگر (!cid_on_cpu(cpu_cid))
			cpu_cid = cid_مقصد_cpu_cid(mm_get_cid(mm));

		/* Handle the transition mode flag if required */
		اگر (منوال & MM_CID_TRANSIT)
			cpu_cid = cpu_cid_مقصد_cid(cpu_cid) | MM_CID_TRANSIT;
	}
	mm_cid_update_pcpu_cid(mm, cpu_cid);
	mm_cid_update_task_cid(t, cpu_cid);
}

ساکن __always_inline خالی mm_cid_from_task(ساخت task_struct *t, غیرعلامتی صحیح cpu_cid,
					     غیرعلامتی صحیح منوال)
{
	غیرعلامتی صحیح max_cids, tcid = t->mm_cid.cid;
	ساخت mm_struct *mm = t->mm;

	max_cids = READ_ONCE(mm->mm_cid.max_cids);
	/* Optimize for the common case, where both have the ONCPU bit clear */
	اگر (likely(cid_on_task(tcid | cpu_cid))) {
		اگر (likely(tcid < max_cids)) {
			mm_cid_update_pcpu_cid(mm, tcid);
			واپس;
		}
		/* Try to converge into the optimal CID space */
		tcid = mm_cid_converge(mm, tcid, max_cids);
	} ورنہ {
		/* Hand over or drop the CPU owned CID */
		اگر (cid_on_cpu(cpu_cid)) {
			اگر (cid_on_task(tcid))
				mm_drop_cid_on_cpu(mm, this_cpu_ptr(mm->mm_cid.pcpu));
			ورنہ
				tcid = cpu_cid_مقصد_cid(cpu_cid);
		}
		/* Still nothing, allocate a new one */
		اگر (!cid_on_task(tcid))
			tcid = mm_get_cid(mm);
		/* Set the transition mode flag if required */
		tcid |= منوال & MM_CID_TRANSIT;
	}
	mm_cid_update_pcpu_cid(mm, tcid);
	mm_cid_update_task_cid(t, tcid);
}

ساکن __always_inline خالی mm_cid_schedin(ساخت task_struct *اگلا)
{
	ساخت mm_struct *mm = اگلا->mm;
	غیرعلامتی صحیح cpu_cid, منوال;

	اگر (!اگلا->mm_cid.فعال)
		واپس;

	cpu_cid = __this_cpu_read(mm->mm_cid.pcpu->cid);
	منوال = READ_ONCE(mm->mm_cid.منوال);
	اگر (likely(!cid_on_cpu(منوال)))
		mm_cid_from_task(اگلا, cpu_cid, منوال);
	ورنہ
		mm_cid_from_cpu(اگلا, cpu_cid, منوال);
}

ساکن __always_inline خالی mm_cid_schedout(ساخت task_struct *پچھلا)
{
	ساخت mm_struct *mm = پچھلا->mm;
	غیرعلامتی صحیح منوال, cid;

	/* During mode transitions CIDs are temporary and need to be dropped */
	اگر (likely(!cid_اندر_transit(پچھلا->mm_cid.cid)))
		واپس;

	منوال = READ_ONCE(mm->mm_cid.منوال);
	cid = cid_from_transit_cid(پچھلا->mm_cid.cid);

	/*
	 * If transition mode is done, transfer ownership when the CID is
	 * within the convergence range to optimize the next schedule in.
	 */
	اگر (!cid_اندر_transit(منوال) && cid < READ_ONCE(mm->mm_cid.max_cids)) {
		اگر (cid_on_cpu(منوال))
			cid = cid_مقصد_cpu_cid(cid);

		/* Update both so that the next schedule in goes into the fast path */
		mm_cid_update_pcpu_cid(mm, cid);
		پچھلا->mm_cid.cid = cid;
	} ورنہ {
		mm_drop_cid(mm, cid);
		پچھلا->mm_cid.cid = MM_CID_UNSET;
	}
}

ساکن اندرونی خالی mm_cid_switch_مقصد(ساخت task_struct *پچھلا, ساخت task_struct *اگلا)
{
	mm_cid_schedout(پچھلا);
	mm_cid_schedin(اگلا);
}

#ورنہ /* !CONFIG_SCHED_MM_CID: */
ساکن اندرونی خالی mm_cid_switch_مقصد(ساخت task_struct *پچھلا, ساخت task_struct *اگلا) { }
#اگر_ختم /* !CONFIG_SCHED_MM_CID */

#اگر_تعریف_شدہ CONFIG_SCHED_CACHE
DECLARE_STATIC_کلید_FALSE(sched_cache_present);
DECLARE_STATIC_کلید_FALSE(sched_cache_فعال);
خارجی صحیح sysctl_sched_cache_user;
خارجی غیرعلامتی صحیح llc_aggr_tolerance;
خارجی غیرعلامتی صحیح llc_epoch_period;
خارجی غیرعلامتی صحیح llc_epoch_affinity_timeout;
خارجی غیرعلامتی صحیح llc_imb_pct;
خارجی غیرعلامتی صحیح llc_overaggr_pct;

ساکن اندرونی bool sched_cache_enabled(خالی)
{
	واپس static_branch_unlikely(&sched_cache_فعال);
}

خارجی خالی sched_cache_فعال_سیٹ(خالی);

#اگر_ختم

خالی sched_domains_free_llc_id(صحیح cpu);

خارجی خالی init_sched_mm(ساخت task_struct *p);

خارجی u64 avg_vruntime(ساخت cfs_rq *cfs_rq);
خارجی صحیح entity_eligible(ساخت cfs_rq *cfs_rq, ساخت sched_entity *se);
ساکن اندرونی
خالی move_queued_task_locked(ساخت rq *src_rq, ساخت rq *dst_rq, ساخت task_struct *task)
{
	lockdep_assert_rq_held(src_rq);
	lockdep_assert_rq_held(dst_rq);

	deactivate_task(src_rq, task, 0);
	سیٹ_task_cpu(task, dst_rq->cpu);
	activate_task(dst_rq, task, 0);
	wakeup_preempt(dst_rq, task, 0);
}

ساکن اندرونی
bool task_is_pushable(ساخت rq *rq, ساخت task_struct *p, صحیح cpu)
{
	اگر (!task_on_cpu(rq, p) &&
	    cpumask_test_cpu(cpu, &p->cpus_نقاب))
		واپس true;

	واپس false;
}

#اگر_تعریف_شدہ CONFIG_RT_MUTEXES

ساکن اندرونی صحیح __rt_effective_prio(ساخت task_struct *pi_task, صحیح prio)
{
	اگر (pi_task)
		prio = min(prio, pi_task->prio);

	واپس prio;
}

ساکن اندرونی صحیح rt_effective_prio(ساخت task_struct *p, صحیح prio)
{
	ساخت task_struct *pi_task = rt_mutex_get_چوٹی_task(p);

	واپس __rt_effective_prio(pi_task, prio);
}

#ورنہ /* !CONFIG_RT_MUTEXES: */

ساکن اندرونی صحیح rt_effective_prio(ساخت task_struct *p, صحیح prio)
{
	واپس prio;
}

#اگر_ختم /* !CONFIG_RT_MUTEXES */

خارجی صحیح __sched_setscheduler(ساخت task_struct *p, ثابت ساخت sched_attr *attr, bool user, bool pi);
خارجی صحیح __sched_setaffinity(ساخت task_struct *p, ساخت affinity_context *ctx);
خارجی ثابت ساخت sched_class *__setscheduler_class(صحیح policy, صحیح prio);
خارجی خالی سیٹ_بھرو_وزن(ساخت task_struct *p, bool update_بھرو);
خارجی خالی قطار_میں_ڈالو_task(ساخت rq *rq, ساخت task_struct *p, صحیح flags);
خارجی bool قطار_سے_نکالو_task(ساخت rq *rq, ساخت task_struct *p, صحیح flags);

خارجی ساخت balance_callback *splice_balance_callbacks(ساخت rq *rq);

خارجی خالی __balance_callbacks(ساخت rq *rq, ساخت rq_flags *rf);
خارجی خالی balance_callbacks(ساخت rq *rq, ساخت balance_callback *سر);

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
ساخت sched_change_ctx {
	u64			prio;
	ساخت task_struct	*p;
	ثابت ساخت sched_class *class;
	صحیح			flags;
	bool			queued;
	bool			running;
};

ساخت sched_change_ctx *sched_change_begin(ساخت task_struct *p, غیرعلامتی صحیح flags);
خالی sched_change_انتہا(ساخت sched_change_ctx *ctx);

DEFINE_CLASS(sched_change, ساخت sched_change_ctx *,
	     sched_change_انتہا(_T),
	     sched_change_begin(p, flags),
	     ساخت task_struct *p, غیرعلامتی صحیح flags)

DEFINE_CLASS_IS_UNCONDITIONAL(sched_change)

#شامل "ext/ext.h"

#اگر_تعریف_شدہ CONFIG_PREFERRED_CPU
خالی sched_ڈالو_current_non_preferred_cpu(ساخت rq *rq);
#ورنہ	/* !CONFIG_PREFERRED_CPU */
ساکن اندرونی خالی sched_ڈالو_current_non_preferred_cpu(ساخت rq *rq) { }
#اگر_ختم

#اگر_ختم /* _KERNEL_SCHED_SCHED_H */
