// SPDX-License-Identifier: GPL-2.0
/*
 * Completely Fair Scheduling (CFS) Class (SCHED_NORMAL/SCHED_BATCH)
 *
 *  Copyright (C) 2007 Red Hat, Inc., Ingo Molnar <mingo@redhat.com>
 *
 *  Interactivity improvements by Mike Galbraith
 *  (C) 2007 Mike Galbraith <efault@gmx.de>
 *
 *  Various enhancements by Dmitry Adamushko.
 *  (C) 2007 Dmitry Adamushko <dmitry.adamushko@gmail.com>
 *
 *  Group scheduling enhancements by Srivatsa Vaddagiri
 *  Copyright IBM Corporation, 2007
 *  Author: Srivatsa Vaddagiri <vatsa@linux.vnet.ibm.com>
 *
 *  Scaled math optimizations by Thomas Gleixner
 *  Copyright (C) 2007, Linutronix GmbH, Thomas Gleixner <tglx@kernel.org>
 *
 *  Adaptive scheduling granularity, math enhancements by Peter Zijlstra
 *  Copyright (C) 2007 Red Hat, Inc., Peter Zijlstra
 */
#شامل <linux/energy_model.h>
#شامل <linux/mmap_lock.h>
#شامل <linux/hugetlb_inline.h>
#شامل <linux/jiffies.h>
#شامل <linux/math.h>
#شامل <linux/mm_api.h>
#شامل <linux/highmem.h>
#شامل <linux/hrtimer.h>
#شامل <linux/hrtimer_bases.h>
#شامل <linux/spinlock_api.h>
#شامل <linux/cpumask_api.h>
#شامل <linux/lockdep_api.h>
#شامل <linux/softirq.h>
#شامل <linux/refcount_api.h>
#شامل <linux/topology.h>
#شامل <linux/sched/clock.h>
#شامل <linux/sched/cond_resched.h>
#شامل <linux/sched/cputime.h>
#شامل <linux/sched/isolation.h>
#شامل <linux/sched/nohz.h>
#شامل <linux/sched/prio.h>
#شامل <linux/static_call.h>

#شامل <linux/cpuidle.h>
#شامل <linux/interrupt.h>
#شامل <linux/memory-tiers.h>
#شامل <linux/mempolicy.h>
#شامل <linux/mutex_api.h>
#شامل <linux/profile.h>
#شامل <linux/psi.h>
#شامل <linux/ratelimit.h>
#شامل <linux/task_work.h>
#شامل <linux/rbtree_augmented.h>

#شامل <asm/switch_to.h>

#شامل <uapi/linux/sched/types.h>

#شامل "sched.h"
#شامل "stats.h"
#شامل "autogroup.h"

/*
 * The initial- and re-scaling of tunables is configurable
 *
 * Options are:
 *
 *   SCHED_TUNABLESCALING_NONE - unscaled, always *1
 *   SCHED_TUNABLESCALING_LOG - scaled logarithmically, *1+ilog(ncpus)
 *   SCHED_TUNABLESCALING_LINEAR - scaled linear, *ncpus
 *
 * (default SCHED_TUNABLESCALING_LOG = *(1+ilog(ncpus))
 */
غیرعلامتی صحیح sysctl_sched_tunable_scaling = SCHED_TUNABLESCALING_LOG;

/*
 * Default base time slice (request size r_i) for SCHED_NORMAL/SCHED_BATCH:
 *
 * Under EEVDF this is the request size used to compute the virtual
 * deadline; see update_deadline().
 *
 * (default: 0.70 msec * (1 + ilog(ncpus)), units: nanoseconds)
 */
غیرعلامتی صحیح sysctl_sched_بنیاد_قاش			= 700000ULL;
ساکن غیرعلامتی صحیح normalized_sysctl_sched_بنیاد_قاش	= 700000ULL;

__read_mostly غیرعلامتی صحیح sysctl_sched_migration_لاگت	= 500000UL;

ساکن صحیح __init تیاری_sched_thermal_decay_سرکاؤ(حرف *str)
{
	pr_warn("Ignoring the deprecated sched_thermal_decay_shift= option\n");
	واپس 1;
}
__setup("sched_thermal_decay_shift=", تیاری_sched_thermal_decay_سرکاؤ);

/*
 * For asym packing, by default the lower numbered CPU has higher priority.
 */
صحیح __weak arch_asym_cpu_ترجیح(صحیح cpu)
{
	واپس -cpu;
}

/*
 * The margin used when comparing utilization with CPU capacity.
 *
 * (default: ~20%)
 */
#تعریف_کرو سماتا_استعداد(cap, max)	((cap) * 1280 < (max) * 1024)

/*
 * The margin used when comparing CPU capacities.
 * is 'cap1' noticeably greater than 'cap2'
 *
 * (default: ~5%)
 */
#تعریف_کرو استعداد_greater(cap1, cap2) ((cap1) * 1024 > (cap2) * 1078)

#اگر_تعریف_شدہ CONFIG_CFS_BANDWIDTH
/*
 * Amount of runtime to allocate from global (tg) to local (per-cfs_rq) pool
 * each time a cfs_rq requests quota.
 *
 * Note: in the case that the slice exceeds the runtime remaining (either due
 * to consumption or the quota being specified to be smaller than the slice)
 * we will always only issue the remaining available time.
 *
 * (default: 5 msec, units: microseconds)
 */
ساکن غیرعلامتی صحیح sysctl_sched_cfs_bandwidth_قاش		= 5000UL;
#اگر_ختم

#اگر_تعریف_شدہ CONFIG_NUMA_BALANCING
/* Restrict the NUMA promotion throughput (MB/s) for each target node. */
ساکن غیرعلامتی صحیح sysctl_numa_balancing_promote_شرح_حد_بندی = 65536;
#اگر_ختم

#اگر_تعریف_شدہ CONFIG_SYSCTL
ساکن ثابت ساخت ctl_جدول sched_fair_sysctls[] = {
#اگر_تعریف_شدہ CONFIG_CFS_BANDWIDTH
	{
		.procname       = "sched_cfs_bandwidth_slice_us",
		.ڈیٹا           = &sysctl_sched_cfs_bandwidth_قاش,
		.maxlen         = حجم(غیرعلامتی صحیح),
		.منوال           = 0644,
		.proc_handler   = proc_dointvec_minmax,
		.extra1         = SYSCTL_ONE,
	},
#اگر_ختم
#اگر_تعریف_شدہ CONFIG_NUMA_BALANCING
	{
		.procname	= "numa_balancing_promote_rate_limit_MBps",
		.ڈیٹا		= &sysctl_numa_balancing_promote_شرح_حد_بندی,
		.maxlen		= حجم(غیرعلامتی صحیح),
		.منوال		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= SYSCTL_ZERO,
	},
#اگر_ختم /* CONFIG_NUMA_BALANCING */
};

ساکن صحیح __init sched_fair_sysctl_init(خالی)
{
	رجسٹر_sysctl_init("kernel", sched_fair_sysctls);
	واپس 0;
}
late_initcall(sched_fair_sysctl_init);
#اگر_ختم /* CONFIG_SYSCTL */

ساکن اندرونی خالی update_بھرو_جوڑو(ساخت بھرو_وزن *lw, غیرعلامتی طویل inc)
{
	lw->وزن += inc;
	lw->inv_وزن = 0;
}

ساکن اندرونی خالی update_بھرو_sub(ساخت بھرو_وزن *lw, غیرعلامتی طویل dec)
{
	lw->وزن -= dec;
	lw->inv_وزن = 0;
}

ساکن اندرونی خالی update_بھرو_سیٹ(ساخت بھرو_وزن *lw, غیرعلامتی طویل w)
{
	lw->وزن = w;
	lw->inv_وزن = 0;
}

/*
 * Increase the granularity value when there are more CPUs,
 * because with more CPUs the 'effective latency' as visible
 * to users decreases. But the relationship is not linear,
 * so pick a second-best guess by going with the log2 of the
 * number of CPUs.
 *
 * This idea comes from the SD scheduler of Con Kolivas:
 */
ساکن غیرعلامتی صحیح get_update_sysctl_ضربی_عدد(خالی)
{
	غیرعلامتی صحیح cpus = min_t(غیرعلامتی صحیح, عدد_online_cpus(), 8);
	غیرعلامتی صحیح ضربی_عدد;

	انتخاب (sysctl_sched_tunable_scaling) {
	حالت SCHED_TUNABLESCALING_NONE:
		ضربی_عدد = 1;
		توڑو;
	حالت SCHED_TUNABLESCALING_خطی:
		ضربی_عدد = cpus;
		توڑو;
	حالت SCHED_TUNABLESCALING_LOG:
	طےشدہ:
		ضربی_عدد = 1 + ilog2(cpus);
		توڑو;
	}

	واپس ضربی_عدد;
}

ساکن خالی update_sysctl(خالی)
{
	غیرعلامتی صحیح ضربی_عدد = get_update_sysctl_ضربی_عدد();

#تعریف_کرو سیٹ_SYSCTL(name) \
	(sysctl_##name = (ضربی_عدد) * normalized_sysctl_##name)
	سیٹ_SYSCTL(sched_بنیاد_قاش);
#تعریف_منسوخ سیٹ_SYSCTL
}

خالی __init sched_init_granularity(خالی)
{
	update_sysctl();
}

#اگر_غیر_تعریف_شدہ CONFIG_64_ثنائی_ہندسہ
#تعریف_کرو WMULT_CONST	(~0U)
#تعریف_کرو WMULT_سرکاؤ	32

ساکن خالی __update_inv_weight(ساخت بھرو_وزن *lw)
{
	غیرعلامتی طویل w;

	اگر (likely(lw->inv_وزن))
		واپس;

	w = scale_بھرو_نیچے_کو(lw->وزن);

	اگر (بٹ_فی_LONG > 32 && unlikely(w >= WMULT_CONST))
		lw->inv_وزن = 1;
	ورنہ اگر (unlikely(!w))
		lw->inv_وزن = WMULT_CONST;
	ورنہ
		lw->inv_وزن = WMULT_CONST / w;
}

/*
 * delta_exec * weight / lw.weight
 *   OR
 * (delta_exec * (weight * lw->inv_weight)) >> WMULT_SHIFT
 *
 * Either weight := NICE_0_LOAD and lw \e sched_prio_to_wmult[], in which case
 * we're guaranteed shift stays positive because inv_weight is guaranteed to
 * fit 32 bits, and NICE_0_LOAD gives another 10 bits; therefore shift >= 22.
 *
 * Or, weight =< lw.weight (because lw.weight is the runqueue weight), thus
 * weight/lw.weight <= 1, and therefore our shift will also be positive.
 */
ساکن u64 __calc_delta(u64 delta_exec, غیرعلامتی طویل وزن, ساخت بھرو_وزن *lw)
{
	u64 fact = scale_بھرو_نیچے_کو(وزن);
	u32 fact_hi = (u32)(fact >> 32);
	صحیح سرکاؤ = WMULT_سرکاؤ;
	صحیح fs;

	__update_inv_weight(lw);

	اگر (unlikely(fact_hi)) {
		fs = fls(fact_hi);
		سرکاؤ -= fs;
		fact >>= fs;
	}

	fact = mul_u32_u32(fact, lw->inv_وزن);

	fact_hi = (u32)(fact >> 32);
	اگر (fact_hi) {
		fs = fls(fact_hi);
		سرکاؤ -= fs;
		fact >>= fs;
	}

	واپس mul_u64_u32_shr(delta_exec, fact, سرکاؤ);
}
#ورنہ
ساکن u64 __calc_delta(u64 delta_exec, غیرعلامتی طویل وزن, ساخت بھرو_وزن *lw)
{
	واپس (delta_exec * وزن) / lw->وزن;
}
#اگر_ختم

/*
 * delta /= w
 */
ساکن اندرونی u64 calc_delta_fair(u64 delta, ساخت sched_entity *se)
{
	اگر (se->h_بھرو.وزن != NICE_0_بھرو)
		delta = __calc_delta(delta, NICE_0_بھرو, &se->h_بھرو);

	واپس delta;
}

ثابت ساخت sched_class fair_sched_class;

/**************************************************************
 * CFS operations on generic schedulable entities:
 */

#اگر_تعریف_شدہ CONFIG_FAIR_گروہ_SCHED

/* Walk up scheduling entities hierarchy */
#تعریف_کرو for_each_sched_entity(se) \
		برائے (; se; se = se->والد)

ساکن اندرونی bool list_جوڑو_برگ_cfs_rq(ساخت cfs_rq *cfs_rq)
{
	ساخت rq *rq = rq_کا(cfs_rq);
	صحیح cpu = cpu_کا(rq);

	اگر (cfs_rq->on_list)
		واپس rq->tmp_alone_branch == &rq->برگ_cfs_rq_list;

	cfs_rq->on_list = 1;

	/*
	 * Ensure we either appear before our parent (if already
	 * enqueued) or force our parent to appear after us when it is
	 * enqueued. The fact that we always enqueue bottom-up
	 * reduces this to two cases and a special case for the root
	 * cfs_rq. Furthermore, it also means that we will always reset
	 * tmp_alone_branch either when the branch is connected
	 * to a tree or when we reach the top of the tree
	 */
	اگر (cfs_rq->tg->والد &&
	    tg_cfs_rq(cfs_rq->tg->والد, cpu)->on_list) {
		/*
		 * If parent is already on the list, we add the child
		 * just before. Thanks to circular linked property of
		 * the list, this means to put the child at the tail
		 * of the list that starts by parent.
		 */
		list_جوڑو_دم_rcu(&cfs_rq->برگ_cfs_rq_list,
			&(tg_cfs_rq(cfs_rq->tg->والد, cpu)->برگ_cfs_rq_list));
		/*
		 * The branch is now connected to its tree so we can
		 * reset tmp_alone_branch to the beginning of the
		 * list.
		 */
		rq->tmp_alone_branch = &rq->برگ_cfs_rq_list;
		واپس true;
	}

	اگر (!cfs_rq->tg->والد) {
		/*
		 * cfs rq without parent should be put
		 * at the tail of the list.
		 */
		list_جوڑو_دم_rcu(&cfs_rq->برگ_cfs_rq_list,
			&rq->برگ_cfs_rq_list);
		/*
		 * We have reach the top of a tree so we can reset
		 * tmp_alone_branch to the beginning of the list.
		 */
		rq->tmp_alone_branch = &rq->برگ_cfs_rq_list;
		واپس true;
	}

	/*
	 * The parent has not already been added so we want to
	 * make sure that it will be put after us.
	 * tmp_alone_branch points to the begin of the branch
	 * where we will add parent.
	 */
	list_جوڑو_rcu(&cfs_rq->برگ_cfs_rq_list, rq->tmp_alone_branch);
	/*
	 * update tmp_alone_branch to points to the new begin
	 * of the branch
	 */
	rq->tmp_alone_branch = &cfs_rq->برگ_cfs_rq_list;
	واپس false;
}

ساکن اندرونی خالی list_del_برگ_cfs_rq(ساخت cfs_rq *cfs_rq)
{
	اگر (cfs_rq->on_list) {
		ساخت rq *rq = rq_کا(cfs_rq);

		/*
		 * With cfs_rq being unthrottled/throttled during an enqueue,
		 * it can happen the tmp_alone_branch points to the leaf that
		 * we finally want to delete. In this case, tmp_alone_branch moves
		 * to the prev element but it will point to rq->leaf_cfs_rq_list
		 * at the end of the enqueue.
		 */
		اگر (rq->tmp_alone_branch == &cfs_rq->برگ_cfs_rq_list)
			rq->tmp_alone_branch = cfs_rq->برگ_cfs_rq_list.پچھلا;

		list_del_rcu(&cfs_rq->برگ_cfs_rq_list);
		cfs_rq->on_list = 0;
	}
}

ساکن اندرونی خالی assert_list_برگ_cfs_rq(ساخت rq *rq)
{
	WARN_ON_ONCE(rq->tmp_alone_branch != &rq->برگ_cfs_rq_list);
}

/* Iterate through all leaf cfs_rq's on a runqueue */
#تعریف_کرو for_each_برگ_cfs_rq_محفوظ(rq, cfs_rq, pos)			\
	list_for_each_اندراج_محفوظ(cfs_rq, pos, &rq->برگ_cfs_rq_list,	\
				 برگ_cfs_rq_list)

/* Do the two (enqueued) entities belong to the same group ? */
ساکن اندرونی ساخت cfs_rq *
is_same_گروہ(ساخت sched_entity *se, ساخت sched_entity *pse)
{
	اگر (se->cfs_rq == pse->cfs_rq)
		واپس se->cfs_rq;

	واپس تہی;
}

ساکن اندرونی ساخت sched_entity *والد_entity(ثابت ساخت sched_entity *se)
{
	واپس se->والد;
}

ساکن صحیح tg_is_idle(ساخت task_گروہ *tg)
{
	واپس tg->idle > 0;
}

ساکن صحیح cfs_rq_is_idle(ساخت cfs_rq *cfs_rq)
{
	واپس cfs_rq->idle > 0;
}

ساکن صحیح se_is_idle(ساخت sched_entity *se)
{
	اگر (entity_is_task(se))
		واپس task_has_idle_policy(task_کا(se));
	واپس cfs_rq_is_idle(گروہ_cfs_rq(se));
}

#ورنہ /* !CONFIG_FAIR_GROUP_SCHED: */

#تعریف_کرو for_each_sched_entity(se) \
		برائے (; se; se = تہی)

ساکن اندرونی bool list_جوڑو_برگ_cfs_rq(ساخت cfs_rq *cfs_rq)
{
	واپس true;
}

ساکن اندرونی خالی list_del_برگ_cfs_rq(ساخت cfs_rq *cfs_rq)
{
}

ساکن اندرونی خالی assert_list_برگ_cfs_rq(ساخت rq *rq)
{
}

#تعریف_کرو for_each_برگ_cfs_rq_محفوظ(rq, cfs_rq, pos)	\
		برائے (cfs_rq = &rq->cfs, pos = تہی; cfs_rq; cfs_rq = pos)

ساکن اندرونی ساخت sched_entity *والد_entity(ساخت sched_entity *se)
{
	واپس تہی;
}

ساکن اندرونی صحیح tg_is_idle(ساخت task_گروہ *tg)
{
	واپس 0;
}

ساکن صحیح cfs_rq_is_idle(ساخت cfs_rq *cfs_rq)
{
	واپس 0;
}

ساکن صحیح se_is_idle(ساخت sched_entity *se)
{
	واپس task_has_idle_policy(task_کا(se));
}

#اگر_ختم /* !CONFIG_FAIR_GROUP_SCHED */

ساکن __always_inline
bool account_cfs_rq_runtime(ساخت cfs_rq *cfs_rq, u64 delta_exec);

/**************************************************************
 * Scheduling class tree data structure manipulation methods:
 */

خارجی خالی __BUILD_BUG_vruntime_cmp(خالی);

/* Use __builtin_strcmp() because of __HAVE_ARCH_STRCMP: */

#تعریف_کرو vruntime_cmp(A, CMP_STR, B) ({				\
	صحیح __res = 0;						\
								\
	اگر (!__builtin_strcmp(CMP_STR, "<")) {			\
		__res = ((s64)((A)-(B)) < 0);			\
	} ورنہ اگر (!__builtin_strcmp(CMP_STR, "<=")) {		\
		__res = ((s64)((A)-(B)) <= 0);			\
	} ورنہ اگر (!__builtin_strcmp(CMP_STR, ">")) {		\
		__res = ((s64)((A)-(B)) > 0);			\
	} ورنہ اگر (!__builtin_strcmp(CMP_STR, ">=")) {		\
		__res = ((s64)((A)-(B)) >= 0);			\
	} ورنہ {						\
		/* Unknown operator throws linker error: */	\
		__BUILD_BUG_vruntime_cmp();			\
	}							\
								\
	__res;							\
})

خارجی خالی __BUILD_BUG_vruntime_op(خالی);

#تعریف_کرو vruntime_op(A, OP_STR, B) ({				\
	s64 __res = 0;						\
								\
	اگر (!__builtin_strcmp(OP_STR, "-")) {			\
		__res = (s64)((A)-(B));				\
	} ورنہ {						\
		/* Unknown operator throws linker error: */	\
		__BUILD_BUG_vruntime_op();			\
	}							\
								\
	__res;						\
})


ساکن اندرونی __maybe_unused u64 max_vruntime(u64 max_vruntime, u64 vruntime)
{
	اگر (vruntime_cmp(vruntime, ">", max_vruntime))
		max_vruntime = vruntime;

	واپس max_vruntime;
}

ساکن اندرونی __maybe_unused u64 min_vruntime(u64 min_vruntime, u64 vruntime)
{
	اگر (vruntime_cmp(vruntime, "<", min_vruntime))
		min_vruntime = vruntime;

	واپس min_vruntime;
}

ساکن اندرونی bool entity_before(ثابت ساخت sched_entity *a,
				 ثابت ساخت sched_entity *b)
{
	/*
	 * Tiebreak on vruntime seems unnecessary since it can
	 * hardly happen.
	 */
	واپس vruntime_cmp(a->deadline, "<", b->deadline);
}

/*
 * Per avg_vruntime() below, cfs_rq::zero_vruntime is only slightly stale
 * and this value should be no more than two lag bounds. Which puts it in the
 * general order of:
 *
 *	(slice + TICK_NSEC) << NICE_0_LOAD_SHIFT
 *
 * which is around 44 bits in size (on 64bit); that is 20 for
 * NICE_0_LOAD_SHIFT, another 20 for NSEC_PER_MSEC and then a handful for
 * however many msec the actual slice+tick ends up begin.
 *
 * (disregarding the actual divide-by-weight part makes for the worst case
 * weight of 2, which nicely cancels vs the fuzz in zero_vruntime not actually
 * being the zero-lag point).
 */
ساکن اندرونی s64 entity_کلید(ساخت cfs_rq *cfs_rq, ساخت sched_entity *se)
{
	واپس vruntime_op(se->vruntime, "-", cfs_rq->zero_vruntime);
}

#تعریف_کرو __node_2_se(گرہ) \
	rb_اندراج((گرہ), ساخت sched_entity, تسلسل_گرہ)

/*
 * Compute virtual time from the per-task service numbers:
 *
 * Fair schedulers conserve lag:
 *
 *   \Sum lag_i = 0
 *
 * Where lag_i is given by:
 *
 *   lag_i = S - s_i = w_i * (V - v_i)
 *
 * Where S is the ideal service time and V is it's virtual time counterpart.
 * Therefore:
 *
 *   \Sum lag_i = 0
 *   \Sum w_i * (V - v_i) = 0
 *   \Sum (w_i * V - w_i * v_i) = 0
 *
 * From which we can solve an expression for V in v_i (which we have in
 * se->vruntime):
 *
 *       \Sum v_i * w_i   \Sum v_i * w_i
 *   V = -------------- = --------------
 *          \Sum w_i            W
 *
 * Specifically, this is the weighted average of all entity virtual runtimes.
 *
 * [[ NOTE: this is only equal to the ideal scheduler under the condition
 *          that join/leave operations happen at lag_i = 0, otherwise the
 *          virtual time has non-contiguous motion equivalent to:
 *
 *	      V +-= lag_i / W
 *
 *	    Also see the comment in place_entity() that deals with this. ]]
 *
 * However, since v_i is u64, and the multiplication could easily overflow
 * transform it into a relative form that uses smaller quantities:
 *
 * Substitute: v_i == (v_i - v0) + v0
 *
 *     \Sum ((v_i - v0) + v0) * w_i   \Sum (v_i - v0) * w_i
 * V = ---------------------------- = --------------------- + v0
 *                  W                            W
 *
 * Which we track using:
 *
 *                    v0 := cfs_rq->zero_vruntime
 * \Sum (v_i - v0) * w_i := cfs_rq->sum_w_vruntime
 *              \Sum w_i := cfs_rq->sum_weight
 *
 * Since zero_vruntime closely tracks the per-task service, these
 * deltas: (v_i - v0), will be in the order of the maximal (virtual) lag
 * induced in the system due to quantisation.
 */
ساکن اندرونی غیرعلامتی طویل avg_vruntime_وزن(ساخت cfs_rq *cfs_rq, غیرعلامتی طویل w)
{
#اگر_تعریف_شدہ CONFIG_64_ثنائی_ہندسہ
	اگر (cfs_rq->مجموع_سرکاؤ)
		w = max(2UL, w >> cfs_rq->مجموع_سرکاؤ);
#اگر_ختم
	واپس w;
}

ساکن اندرونی خالی
__sum_w_vruntime_add(ساخت cfs_rq *cfs_rq, ساخت sched_entity *se)
{
	غیرعلامتی طویل وزن = avg_vruntime_وزن(cfs_rq, se->h_بھرو.وزن);
	s64 w_vruntime, کلید = entity_کلید(cfs_rq, se);

	w_vruntime = کلید * وزن;
	WARN_ON_ONCE((w_vruntime >> 63) != (w_vruntime >> 62));

	cfs_rq->مجموع_w_vruntime += w_vruntime;
	cfs_rq->مجموع_وزن += وزن;
}

ساکن خالی
مجموع_w_vruntime_جوڑو_paranoid(ساخت cfs_rq *cfs_rq, ساخت sched_entity *se)
{
	غیرعلامتی طویل وزن;
	s64 کلید, tmp;

again:
	وزن = avg_vruntime_وزن(cfs_rq, se->h_بھرو.وزن);
	کلید = entity_کلید(cfs_rq, se);

	اگر (check_mul_حد_سے_تجاوز(کلید, وزن, &کلید))
		جاؤ حد_سے_تجاوز;

	اگر (check_جوڑو_حد_سے_تجاوز(cfs_rq->مجموع_w_vruntime, کلید, &tmp))
		جاؤ حد_سے_تجاوز;

	cfs_rq->مجموع_w_vruntime = tmp;
	cfs_rq->مجموع_وزن += وزن;
	واپس;

حد_سے_تجاوز:
	/*
	 * There's gotta be a limit -- if we're still failing at this point
	 * there's really nothing much to be done about things.
	 */
	BUG_ON(cfs_rq->مجموع_سرکاؤ >= 10);
	cfs_rq->مجموع_سرکاؤ++;

	/*
	 * Note: \Sum (k_i * (w_i >> 1)) != (\Sum (k_i * w_i)) >> 1
	 */
	cfs_rq->مجموع_w_vruntime = 0;
	cfs_rq->مجموع_وزن = 0;

	برائے (ساخت rb_گرہ *گرہ = cfs_rq->tasks_timeline.rb_leftmost;
	     گرہ; گرہ = rb_اگلا(گرہ))
		__sum_w_vruntime_add(cfs_rq, __node_2_se(گرہ));

	جاؤ again;
}

ساکن خالی
مجموع_w_vruntime_جوڑو(ساخت cfs_rq *cfs_rq, ساخت sched_entity *se)
{
	اگر (sched_feat(PARANOID_AVG))
		واپس مجموع_w_vruntime_جوڑو_paranoid(cfs_rq, se);

	__sum_w_vruntime_add(cfs_rq, se);
}

ساکن خالی
مجموع_w_vruntime_sub(ساخت cfs_rq *cfs_rq, ساخت sched_entity *se)
{
	غیرعلامتی طویل وزن = avg_vruntime_وزن(cfs_rq, se->h_بھرو.وزن);
	s64 کلید = entity_کلید(cfs_rq, se);

	cfs_rq->مجموع_w_vruntime -= کلید * وزن;
	cfs_rq->مجموع_وزن -= وزن;
}

ساکن اندرونی
خالی update_zero_vruntime(ساخت cfs_rq *cfs_rq, s64 delta)
{
	/*
	 * v' = v + d ==> sum_w_vruntime' = sum_w_vruntime - d*sum_weight
	 */
	cfs_rq->مجموع_w_vruntime -= cfs_rq->مجموع_وزن * delta;
	cfs_rq->zero_vruntime += delta;
}

/*
 * Specifically: avg_vruntime() + 0 must result in entity_eligible() := true
 * For this to be so, the result of this function must have a left bias.
 *
 * Called in:
 *  - place_entity()      -- before enqueue
 *  - update_entity_lag() -- before dequeue
 *  - update_deadline()   -- slice expiration
 *
 * This means it is one entry 'behind' but that puts it close enough to where
 * the bound on entity_key() is at most two lag bounds.
 */
u64 avg_vruntime(ساخت cfs_rq *cfs_rq)
{
	ساخت sched_entity *curr = cfs_rq->curr;
	طویل وزن = cfs_rq->مجموع_وزن;
	s64 delta = 0;

	اگر (curr && !curr->on_rq)
		curr = تہی;

	اگر (وزن) {
		s64 runtime = cfs_rq->مجموع_w_vruntime;

		اگر (curr) {
			غیرعلامتی طویل w = avg_vruntime_وزن(cfs_rq, curr->h_بھرو.وزن);

			runtime += entity_کلید(cfs_rq, curr) * w;
			وزن += w;
		}

		/* sign flips effective floor / ceiling */
		اگر (runtime < 0)
			runtime -= (وزن - 1);

		delta = div64_long(runtime, وزن);
	} ورنہ اگر (curr) {
		/*
		 * When there is but one element, it is the average.
		 */
		delta = curr->vruntime - cfs_rq->zero_vruntime;
	}

	update_zero_vruntime(cfs_rq, delta);

	واپس cfs_rq->zero_vruntime;
}

/*
 *     \Sum (v_i - v0)*w_i
 * V = ------------------- + v0
 *          \Sum w_i
 *
 * Let W = \Sum w_i, and move v_j such that 'v_j == V', thus:
 *
 * V = 1/W * {(v_j - v0)*w_j + \Sum_i!=j (v_i - v0)*w_i} + v0
 *
 * v_j = 1/W * {(v_j - v0)*w_j + \Sum_i!=j (v_i - v0)*w_i} + v0
 *
 * v_j = 1/W * (v_j - v0)*w_j + 1/W * \Sum_i!=j (v_i - v0)*w_i + v0
 *
 * v_j - 1/W * (v_j - v0)*w_j = 1/W * \Sum_i!=j (v_i - v0)*w_i + v0
 *
 * v_j*W - (v_j - v0)*w_j = \Sum_i!=j (v_i - v0)*w_i + v0*W
 *
 * v_j*(W - w_j) + v0*w_j = \Sum_i!=j (v_i - v0)*w_i + v0*W
 *
 * v_j*(W - w_j) = \Sum_i!=j (v_i - v0)*w_i + v0*(W - w_j)
 *
 *       \Sum_i!=j (v_i - v0)*w_i
 * v_j = ------------------------ + v0
 *               W - w_j
 *
 * When v_j happens to be curr, then '\Sum_i!=j (v_i - v0)*w_i'
 * is cfs_rq->sum_w_runtime, and 'W - w_j' is cfs_rq->sum_weight, since curr
 * is not included in the sum.
 */
ساکن u64 ineligible_vruntime(ساخت cfs_rq *cfs_rq)
{
	ساخت sched_entity *curr = cfs_rq->curr;
	طویل وزن = cfs_rq->مجموع_وزن;
	s64 delta = 0;

	اگر (curr && !curr->on_rq)
		curr = تہی;

	اگر (وزن) {
		s64 runtime = cfs_rq->مجموع_w_vruntime;

		/*
		 * Do not add @curr to obtain the effective '- w_j' terms.
		 */

		/* sign flips effective floor / ceiling */
		اگر (runtime < 0)
			runtime -= (وزن - 1);

		delta = div64_long(runtime, وزن);
	}

	واپس cfs_rq->zero_vruntime + delta + 1;
}

ساکن اندرونی u64 cfs_rq_max_قاش(ساخت cfs_rq *cfs_rq);

/*
 * lag_i = S - s_i = w_i * (V - v_i)
 *
 * However, since V is approximated by the weighted average of all entities it
 * is possible -- by addition/removal/reweight to the tree -- to move V around
 * and end up with a larger lag than we started with.
 *
 * Limit this to either double the slice length with a minimum of TICK_NSEC
 * since that is the timing granularity.
 *
 * EEVDF gives the following limit for a steady state system:
 *
 *   -r_max < lag < max(r_max, q)
 */
ساکن s64 entity_lag(ساخت cfs_rq *cfs_rq, ساخت sched_entity *se, u64 avruntime)
{
	u64 max_قاش = cfs_rq_max_قاش(cfs_rq) + TICK_NSEC;
	s64 vlag, حد_بندی;

	vlag = avruntime - se->vruntime;
	حد_بندی = calc_delta_fair(max_قاش, se);

	واپس clamp(vlag, -حد_بندی, حد_بندی);
}

/*
 * Delayed dequeue aims to reduce the negative lag of a dequeued task. While
 * updating the lag of an entity, check that negative lag didn't increase
 * during the delayed dequeue period which would be unfair.
 * Similarly, check that the entity didn't gain positive lag when DELAY_ZERO
 * is set.
 *
 * Return true if the vlag has been modified. Specifically:
 *
 *   se->vlag != avg_vruntime() - se->vruntime
 *
 * This can be due to clamping in entity_lag() or clamping due to
 * sched_delayed. Either way, when vlag is modified and the entity is
 * retained, the tree needs to be adjusted.
 */
ساکن __always_inline
bool update_entity_lag(ساخت cfs_rq *cfs_rq, ساخت sched_entity *se)
{
	u64 avruntime = avg_vruntime(cfs_rq);
	s64 vlag = entity_lag(cfs_rq, se, avruntime);

	اگر (se->sched_delayed) {
		/* previous vlag < 0 otherwise se would not be delayed */
		vlag = max(vlag, se->vlag);
		اگر (sched_feat(DELAY_ZERO))
			vlag = min(vlag, 0);
	}
	se->vlag = vlag;

	واپس avruntime - vlag != se->vruntime;
}

/*
 * Entity is eligible once it received less service than it ought to have,
 * eg. lag >= 0.
 *
 * lag_i = S - s_i = w_i*(V - v_i)
 *
 * lag_i >= 0 -> V >= v_i
 *
 *     \Sum (v_i - v0)*w_i
 * V = ------------------- + v0
 *          \Sum w_i
 *
 * lag_i >= 0 -> \Sum (v_i - v0)*w_i >= (v_i - v0)*(\Sum w_i)
 *
 * Note: using 'avg_vruntime() > se->vruntime' is inaccurate due
 *       to the loss in precision caused by the division.
 */
ساکن صحیح vruntime_eligible(ساخت cfs_rq *cfs_rq, u64 vruntime)
{
	ساخت sched_entity *curr = cfs_rq->curr;
	s64 کلید, avg = cfs_rq->مجموع_w_vruntime;
	طویل بھرو = cfs_rq->مجموع_وزن;

	اگر (curr && curr->on_rq) {
		غیرعلامتی طویل وزن = avg_vruntime_وزن(cfs_rq, curr->h_بھرو.وزن);

		avg += entity_کلید(cfs_rq, curr) * وزن;
		بھرو += وزن;
	}

	کلید = vruntime_op(vruntime, "-", cfs_rq->zero_vruntime);

	/*
	 * The worst case term for @key includes 'NSEC_TICK * NICE_0_LOAD'
	 * and @load obviously includes NICE_0_LOAD. NSEC_TICK is around 24
	 * bits, while NICE_0_LOAD is 20 on 64bit and 10 otherwise.
	 *
	 * This gives that on 64bit the product will be at least 64bit which
	 * overflows s64, while on 32bit it will only be 44bits and should fit
	 * comfortably.
	 */
#اگر_تعریف_شدہ CONFIG_64_ثنائی_ہندسہ
#اگر_تعریف_شدہ CONFIG_ARCH_SUPPORTS_INT128
	/* This often results in simpler code than __builtin_mul_overflow(). */
	واپس avg >= (__int128)کلید * بھرو;
#ورنہ
	s64 rhs;
	/*
	 * On overflow, the sign of key tells us the correct answer: a large
	 * positive key means vruntime >> V, so not eligible; a large negative
	 * key means vruntime << V, so eligible.
	 */
	اگر (check_mul_حد_سے_تجاوز(کلید, بھرو, &rhs))
		واپس کلید <= 0;

	واپس avg >= rhs;
#اگر_ختم
#ورنہ /* 32bit */
	واپس avg >= کلید * بھرو;
#اگر_ختم
}

صحیح entity_eligible(ساخت cfs_rq *cfs_rq, ساخت sched_entity *se)
{
	واپس vruntime_eligible(cfs_rq, se->vruntime);
}

ساکن اندرونی u64 cfs_rq_min_قاش(ساخت cfs_rq *cfs_rq)
{
	ساخت sched_entity *جڑ = __pick_root_entity(cfs_rq);
	ساخت sched_entity *curr = cfs_rq->curr;
	u64 min_قاش = ~0ULL;

	اگر (curr && curr->on_rq)
		min_قاش = curr->قاش;

	اگر (جڑ)
		min_قاش = min(min_قاش, جڑ->min_قاش);

	واپس min_قاش;
}

ساکن اندرونی u64 cfs_rq_max_قاش(ساخت cfs_rq *cfs_rq)
{
	ساخت sched_entity *جڑ = __pick_root_entity(cfs_rq);
	ساخت sched_entity *curr = cfs_rq->curr;
	u64 max_قاش = 0ULL;

	اگر (curr && curr->on_rq)
		max_قاش = curr->قاش;

	اگر (جڑ)
		max_قاش = max(max_قاش, جڑ->max_قاش);

	واپس max_قاش;
}

ساکن اندرونی bool __entity_less(ساخت rb_گرہ *a, ثابت ساخت rb_گرہ *b)
{
	واپس entity_before(__node_2_se(a), __node_2_se(b));
}

/* min() for wrapping vruntimes */
ساکن اندرونی u64 __min_vruntime(u64 a, u64 b)
{
	واپس vruntime_cmp(a, "<", b) ? a : b;
}

RB_DECLARE_CALLBACKS(ساکن, min_vruntime_cb,
		     ساخت sched_entity, تسلسل_گرہ,
		     RB_AUG(vruntime, min_vruntime, __min_vruntime),
		     RB_AUG(قاش, min_قاش, min),
		     RB_AUG(قاش, max_قاش, max));

/*
 * Enqueue an entity into the rb-tree:
 */
ساکن خالی __enqueue_entity(ساخت cfs_rq *cfs_rq, ساخت sched_entity *se)
{
	WARN_ON_ONCE(&rq_کا(cfs_rq)->cfs != cfs_rq);
	WARN_ON_ONCE(!entity_is_task(se));

	مجموع_w_vruntime_جوڑو(cfs_rq, se);
	se->min_vruntime = se->vruntime;
	se->min_قاش = se->قاش;
	se->max_قاش = se->قاش;

	rb_جوڑو_augmented_cached(&se->تسلسل_گرہ, &cfs_rq->tasks_timeline,
				__entity_less, &min_vruntime_cb);
}

ساکن خالی __dequeue_entity(ساخت cfs_rq *cfs_rq, ساخت sched_entity *se)
{
	WARN_ON_ONCE(&rq_کا(cfs_rq)->cfs != cfs_rq);
	WARN_ON_ONCE(!entity_is_task(se));

	rb_erase_augmented_cached(&se->تسلسل_گرہ, &cfs_rq->tasks_timeline,
				  &min_vruntime_cb);
	مجموع_w_vruntime_sub(cfs_rq, se);
}

ساخت sched_entity *__pick_root_entity(ساخت cfs_rq *cfs_rq)
{
	ساخت rb_گرہ *جڑ = cfs_rq->tasks_timeline.rb_جڑ.rb_گرہ;

	اگر (!جڑ)
		واپس تہی;

	واپس __node_2_se(جڑ);
}

ساخت sched_entity *__pick_first_entity(ساخت cfs_rq *cfs_rq)
{
	ساخت rb_گرہ *بایاں = rb_پہلا_cached(&cfs_rq->tasks_timeline);

	اگر (!بایاں)
		واپس تہی;

	واپس __node_2_se(بایاں);
}

/*
 * Set the vruntime up to which an entity can run before looking
 * for another entity to pick.
 * In case of run to parity, we use the shortest slice of the enqueued
 * entities to set the protected period.
 * When run to parity is disabled, we give a minimum quantum to the running
 * entity to ensure progress.
 */
ساکن اندرونی خالی سیٹ_protect_قاش(ساخت cfs_rq *cfs_rq, ساخت sched_entity *se)
{
	u64 قاش = normalized_sysctl_sched_بنیاد_قاش;
	u64 vprot = se->deadline;

	اگر (sched_feat(تسلسل_مقصد_PARITY))
		قاش = cfs_rq_min_قاش(cfs_rq);

	قاش = min(قاش, se->قاش);

	/* If there are shorter slices than se's one */
	اگر (قاش != se->قاش) {
		vprot = min_vruntime(vprot, se->vruntime + calc_delta_fair(قاش, se));
		اگر (sched_feat(PREEMPT_SHORT))
			vprot = min_vruntime(vprot, ineligible_vruntime(cfs_rq));
	}

	se->vprot = vprot;
}

ساکن اندرونی خالی update_protect_قاش(ساخت cfs_rq *cfs_rq, ساخت sched_entity *se)
{
	u64 vruntime = min_vruntime(se->vruntime, avg_vruntime(cfs_rq));
	u64 قاش = cfs_rq_min_قاش(cfs_rq);
	u64 vprot = min_vruntime(se->vprot, vruntime + calc_delta_fair(قاش, se));

	اگر (sched_feat(PREEMPT_SHORT) && قاش != se->قاش)
		vprot = min_vruntime(vprot, ineligible_vruntime(cfs_rq));

	se->vprot = vprot;
}

ساکن اندرونی bool protect_قاش(ساخت sched_entity *se)
{
	واپس vruntime_cmp(se->vruntime, "<", se->vprot);
}

ساکن اندرونی خالی cancel_protect_قاش(ساخت sched_entity *se)
{
	اگر (protect_قاش(se))
		se->vprot = se->vruntime;
}

/*
 * Earliest Eligible Virtual Deadline First
 *
 * In order to provide latency guarantees for different request sizes
 * EEVDF selects the best runnable task from two criteria:
 *
 *  1) the task must be eligible (must be owed service)
 *
 *  2) from those tasks that meet 1), we select the one
 *     with the earliest virtual deadline.
 *
 * We can do this in O(log n) time due to an augmented RB-tree. The
 * tree keeps the entries sorted on deadline, but also functions as a
 * heap based on the vruntime by keeping:
 *
 *  se->min_vruntime = min(se->vruntime, se->{left,right}->min_vruntime)
 *
 * Which allows tree pruning through eligibility.
 */
ساکن ساخت sched_entity *pick_eevdf(ساخت cfs_rq *cfs_rq, bool protect)
{
	ساخت rb_گرہ *گرہ = cfs_rq->tasks_timeline.rb_جڑ.rb_گرہ;
	ساخت sched_entity *se = __pick_first_entity(cfs_rq);
	ساخت sched_entity *curr = cfs_rq->curr;
	ساخت sched_entity *بہترین = تہی;

	/*
	 * We can safely skip eligibility check if there is only one entity
	 * in this cfs_rq, saving some cycles.
	 */
	اگر (cfs_rq->h_nr_queued == 1)
		واپس curr && curr->on_rq ? curr : se;

	/*
	 * Picking the ->next buddy will affect latency but not fairness.
	 */
	اگر (sched_feat(PICK_BUDDY) && protect &&
	    cfs_rq->اگلا && entity_eligible(cfs_rq, cfs_rq->اگلا)) {
		/* ->next will never be delayed */
		WARN_ON_ONCE(cfs_rq->اگلا->sched_delayed);
		واپس cfs_rq->اگلا;
	}

	اگر (curr && (!curr->on_rq || !entity_eligible(cfs_rq, curr)))
		curr = تہی;

	اگر (curr && protect && protect_قاش(curr))
		واپس curr;

	/* Pick the leftmost entity if it's eligible */
	اگر (se && entity_eligible(cfs_rq, se)) {
		بہترین = se;
		جاؤ ملا;
	}

	/* Heap search for the EEVD entity */
	جب (گرہ) {
		ساخت rb_گرہ *بایاں = گرہ->rb_بایاں;

		/*
		 * Eligible entities in left subtree are always better
		 * choices, since they have earlier deadlines.
		 */
		اگر (بایاں && vruntime_eligible(cfs_rq,
					__node_2_se(بایاں)->min_vruntime)) {
			گرہ = بایاں;
			جاری;
		}

		se = __node_2_se(گرہ);

		/*
		 * The left subtree either is empty or has no eligible
		 * entity, so check the current node since it is the one
		 * with earliest deadline that might be eligible.
		 */
		اگر (entity_eligible(cfs_rq, se)) {
			بہترین = se;
			توڑو;
		}

		گرہ = گرہ->rb_دایاں;
	}
ملا:
	اگر (!بہترین || (curr && entity_before(curr, بہترین)))
		بہترین = curr;

	واپس بہترین;
}

ساخت sched_entity *__pick_last_entity(ساخت cfs_rq *cfs_rq)
{
	ساخت rb_گرہ *آخری = rb_آخری(&cfs_rq->tasks_timeline.rb_جڑ);

	اگر (!آخری)
		واپس تہی;

	واپس __node_2_se(آخری);
}

/**************************************************************
 * Scheduling class statistics methods:
 */
صحیح sched_update_scaling(خالی)
{
	غیرعلامتی صحیح ضربی_عدد = get_update_sysctl_ضربی_عدد();

#تعریف_کرو WRT_SYSCTL(name) \
	(normalized_sysctl_##name = sysctl_##name / (ضربی_عدد))
	WRT_SYSCTL(sched_بنیاد_قاش);
#تعریف_منسوخ WRT_SYSCTL

	واپس 0;
}

ساکن خالی clear_buddies(ساخت cfs_rq *cfs_rq, ساخت sched_entity *se);

/*
 * XXX: strictly: vd_i += N*r_i/w_i such that: vd_i > ve_i
 * this is probably good enough.
 */
ساکن bool update_deadline(ساخت cfs_rq *cfs_rq, ساخت sched_entity *se)
{
	اگر (vruntime_cmp(se->vruntime, "<", se->deadline))
		واپس false;

	/*
	 * For EEVDF the virtual time slope is determined by w_i (iow.
	 * nice) while the request time r_i is determined by
	 * sysctl_sched_base_slice.
	 */
	اگر (!se->custom_قاش)
		se->قاش = sysctl_sched_بنیاد_قاش;

	/*
	 * EEVDF: vd_i = ve_i + r_i / w_i
	 */
	se->deadline = se->vruntime + calc_delta_fair(se->قاش, se);
	avg_vruntime(cfs_rq);

	/*
	 * The task has consumed its request, reschedule.
	 */
	واپس true;
}

#شامل "pelt.h"

ساکن صحیح select_idle_sibling(ساخت task_struct *p, صحیح پچھلا_cpu, صحیح cpu);
ساکن غیرعلامتی طویل task_h_بھرو(ساخت task_struct *p);
ساکن غیرعلامتی طویل استعداد_کا(صحیح cpu);

/* Give new sched_entity start runnable values to heavy its load in infant time */
خالی init_entity_runnable_اوسط_قدر(ساخت sched_entity *se)
{
	ساخت sched_avg *sa = &se->avg;

	memset(sa, 0, حجم(*sa));

	/*
	 * Tasks are initialized with full load to be seen as heavy tasks until
	 * they get a chance to stabilize to their real load level.
	 * Group entities are initialized with zero load to reflect the fact that
	 * nothing has been attached to the task group yet.
	 */
	اگر (entity_is_task(se))
		sa->بھرو_avg = scale_بھرو_نیچے_کو(se->بھرو.وزن);

	/* when this task is enqueued, it will contribute to its cfs_rq's load_avg */
}

/*
 * With new tasks being created, their initial util_avgs are extrapolated
 * based on the cfs_rq's current util_avg:
 *
 *   util_avg = cfs_rq->avg.util_avg / (cfs_rq->avg.load_avg + 1)
 *		* se_weight(se)
 *
 * However, in many cases, the above util_avg does not give a desired
 * value. Moreover, the sum of the util_avgs may be divergent, such
 * as when the series is a harmonic series.
 *
 * To solve this problem, we also cap the util_avg of successive tasks to
 * only 1/2 of the left utilization budget:
 *
 *   util_avg_cap = (cpu_scale - cfs_rq->avg.util_avg) / 2^n
 *
 * where n denotes the nth task and cpu_scale the CPU capacity.
 *
 * For example, for a CPU with 1024 of capacity, a simplest series from
 * the beginning would be like:
 *
 *  task  util_avg: 512, 256, 128,  64,  32,   16,    8, ...
 * cfs_rq util_avg: 512, 768, 896, 960, 992, 1008, 1016, ...
 *
 * Finally, that extrapolated util_avg is clamped to the cap (util_avg_cap)
 * if util_avg > util_avg_cap.
 */
خالی post_init_entity_util_avg(ساخت task_struct *p)
{
	ساخت sched_entity *se = &p->se;
	ساخت cfs_rq *cfs_rq = cfs_rq_کا(se);
	ساخت sched_avg *sa = &se->avg;
	طویل cpu_scale = arch_scale_cpu_استعداد(cpu_کا(rq_کا(cfs_rq)));
	طویل cap = (طویل)(cpu_scale - cfs_rq->avg.util_avg) / 2;

	اگر (p->sched_class != &fair_sched_class) {
		/*
		 * For !fair tasks do:
		 *
		update_cfs_rq_load_avg(now, cfs_rq);
		attach_entity_load_avg(cfs_rq, se);
		switched_from_fair(rq, p);
		 *
		 * such that the next switched_to_fair() has the
		 * expected state.
		 */
		se->avg.آخری_update_وقت = cfs_rq_clock_pelt(cfs_rq);
		واپس;
	}

	اگر (cap > 0) {
		اگر (cfs_rq->avg.util_avg != 0) {
			sa->util_avg  = cfs_rq->avg.util_avg * se_وزن(se);
			sa->util_avg /= (cfs_rq->avg.بھرو_avg + 1);

			اگر (sa->util_avg > cap)
				sa->util_avg = cap;
		} ورنہ {
			sa->util_avg = cap;
		}
	}

	sa->runnable_avg = sa->util_avg;
}

ساکن اندرونی خالی account_mm_sched(ساخت rq *rq, ساخت task_struct *p, s64 delta_exec);

ساکن s64 update_se(ساخت rq *rq, ساخت sched_entity *se)
{
	u64 now = rq_clock_task(rq);
	s64 delta_exec;

	delta_exec = now - se->exec_آغاز;
	اگر (unlikely(delta_exec <= 0))
		واپس delta_exec;

	se->exec_آغاز = now;
	اگر (entity_is_task(se)) {
		ساخت task_struct *running = rq->curr;
		/*
		 * If se is a task, we account the time against the running
		 * task, as w/ proxy-exec they may not be the same.
		 */
		running->se.exec_آغاز = now;
		running->se.مجموع_exec_runtime += delta_exec;

		trace_sched_stat_runtime(running, delta_exec);
		account_گروہ_exec_runtime(running, delta_exec);
		account_mm_sched(rq, running, delta_exec);

		cgroup_account_cputime(running, delta_exec);
	} ورنہ {
		/* If not task, account the time against donor se  */
		se->مجموع_exec_runtime += delta_exec;
	}

	اگر (schedstat_enabled()) {
		ساخت sched_statistics *stats;

		stats = __schedstats_from_se(se);
		__schedstat_set(stats->exec_max,
				max(delta_exec, stats->exec_max));
	}

	واپس delta_exec;
}

#اگر_تعریف_شدہ CONFIG_SCHED_CACHE

/*
 * XXX numbers come from a place the sun don't shine -- probably wants to be SD
 * tunable or so.
 */
#تعریف_کرو EPOCH_PERIOD	(HZ / 100)	/* 10 ms */
#تعریف_کرو EPOCH_LLC_AFFINITY_TIMEOUT	5	/* 50 ms */
__read_mostly غیرعلامتی صحیح llc_aggr_tolerance	= 1;
__read_mostly غیرعلامتی صحیح llc_epoch_period	= EPOCH_PERIOD;
__read_mostly غیرعلامتی صحیح llc_epoch_affinity_timeout = EPOCH_LLC_AFFINITY_TIMEOUT;
__read_mostly غیرعلامتی صحیح llc_imb_pct		= 20;
__read_mostly غیرعلامتی صحیح llc_overaggr_pct	= 50;

ساکن صحیح llc_id(صحیح cpu)
{
	اگر (cpu < 0)
		واپس -1;

	واپس فی_cpu(sd_llc_id, cpu);
}

ساکن اندرونی صحیح get_sched_cache_scale(صحیح mul)
{
	غیرعلامتی صحیح tol = READ_ONCE(llc_aggr_tolerance);

	اگر (!tol)
		واپس 0;

	اگر (tol >= 100)
		واپس زیادہ_سے_زیادہ_عدد;

	واپس (1 + (tol - 1) * mul);
}

ساکن bool exceed_llc_استعداد(ساخت sched_cache_گروہ *grp, صحیح cpu)
{
#اگر_تعریف_شدہ CONFIG_NUMA_BALANCING
	غیرعلامتی طویل llc, footprint;
	ساخت sched_domain *sd;
	صحیح scale;

	guard(rcu)();

	sd = rcu_dereference_sched_domain(cpu_rq(cpu)->sd);
	اگر (!sd)
		واپس true;

	اگر (static_branch_likely(&sched_numa_balancing)) {
		/*
		 * TBD: RDT exclusive LLC ways reserved should be
		 * excluded.
		 */
		llc = sd->llc_بائٹیں;
		footprint = READ_ONCE(grp->footprint);

		/*
		 * Scale the LLC size by 256*llc_aggr_tolerance
		 * and compare it to the task's footprint.
		 *
		 * Suppose the L3 size is 32MB. If the
		 * llc_aggr_tolerance is 1:
		 * When the footprint is larger than 32MB, the
		 * process is regarded as exceeding the LLC
		 * capacity. If the llc_aggr_tolerance is 99:
		 * When the footprint is larger than 784GB, the
		 * process is regarded as exceeding the LLC
		 * capacity:
		 * 784GB = (1 + (99 - 1) * 256) * 32MB
		 * If the llc_aggr_tolerance is 100:
		 * ignore the footprint and do the aggregation
		 * anyway.
		 */
		scale = get_sched_cache_scale(256);
		اگر (scale == زیادہ_سے_زیادہ_عدد)
			واپس false;

		واپس ((llc * (u64)scale) < (footprint * صفحہ_سائز));
	}
#اگر_ختم
	واپس false;
}

ساکن bool invalid_llc_nr(ساخت sched_cache_گروہ *grp, ساخت task_struct *p,
			   صحیح cpu)
{
	صحیح scale;

	اگر (get_nr_دھاگے(p) <= 1)
		واپس true;

	/*
	 * Scale the number of 'cores' in a LLC by llc_aggr_tolerance
	 * and compare it to the task's active threads.
	 */
	scale = get_sched_cache_scale(1);
	اگر (scale == زیادہ_سے_زیادہ_عدد)
		واپس false;

	واپس !سماتا_استعداد((READ_ONCE(grp->nr_running_avg) * cpu_smt_عدد_دھاگے),
			(scale * فی_cpu(sd_llc_سائز, cpu)));
}

/*
 * A task counts in nr_pref_llc_running while it is queued on its preferred
 * LLC (pref_llc_queued) and runnable (!sched_delayed), keeping the counter in
 * the runnable domain so alb_break_llc() can compare it with h_nr_runnable.
 */
ساکن bool task_pref_llc_runnable(ساخت task_struct *p)
{
	واپس p->pref_llc_queued && !p->se.sched_delayed;
}

ساکن خالی pref_llc_running_inc(ساخت rq *rq, ساخت task_struct *p)
{
	اگر (task_pref_llc_runnable(p))
		rq->nr_pref_llc_running++;
}

ساکن خالی pref_llc_running_dec(ساخت rq *rq, ساخت task_struct *p)
{
	اگر (task_pref_llc_runnable(p))
		rq->nr_pref_llc_running--;
}

ساکن خالی account_llc_قطار_میں_ڈالو(ساخت rq *rq, ساخت task_struct *p)
{
	صحیح pref_llc, pref_llc_queued;
	ساخت sched_domain *sd;

	pref_llc = p->preferred_llc;
	اگر (pref_llc < 0)
		واپس;

	pref_llc_queued = (pref_llc == task_llc(p));
	rq->nr_llc_running++;

	/*
	 * Record whether p is enqueued on its preferred
	 * LLC, in order to pair with account_llc_dequeue()
	 * to maintain a consistent nr_pref_llc_running per
	 * runqueue.
	 * This is necessary because a race condition exists:
	 * after a task is enqueued on a runqueue, task_llc(p)
	 * may change due to CPU hotplug. Therefore, checking
	 * task_llc(p) to determine whether the task is being
	 * dequeued from its preferred LLC is unreliable and
	 * can cause inconsistent values - checking the
	 * p->pref_llc_queued in account_llc_dequeue() would
	 * be reliable.
	 */
	p->pref_llc_queued = pref_llc_queued;

	/* Skipped while delayed; clear_delayed() adds it back on wake. */
	pref_llc_running_inc(rq, p);

	sd = rcu_dereference_all(rq->sd);
	اگر (sd && (غیرعلامتی صحیح)pref_llc < sd->llc_max)
		sd->llc_counts[pref_llc]++;
}

ساکن خالی account_llc_قطار_سے_نکالو(ساخت rq *rq, ساخت task_struct *p)
{
	ساخت sched_domain *sd;
	صحیح pref_llc;

	pref_llc = p->preferred_llc;
	اگر (pref_llc < 0)
		واپس;

	rq->nr_llc_running--;
	اگر (p->pref_llc_queued) {
		/*
		 * Skipped if still delayed (set_delayed() already removed it);
		 * clearing pref_llc_queued below also stops clear_delayed()
		 * from re-adding it.
		 */
		pref_llc_running_dec(rq, p);
		/*
		 * Update the status in case
		 * other logic might query
		 * this.
		 */
		p->pref_llc_queued = 0;
	}

	sd = rcu_dereference_all(rq->sd);
	اگر (sd && (غیرعلامتی صحیح)pref_llc < sd->llc_max) {
		/*
		 * There is a race condition between dequeue
		 * and CPU hotplug. After a task has been enqueued
		 * on CPUx, a CPU hotplug event occurs, and all online
		 * CPUs (including CPUx) rebuild their sched_domains
		 * and reset statistics to zero(including sd->llc_counts).
		 * This can cause temporary undercount and we have to
		 * check for such underflow in sd->llc_counts.
		 *
		 * This undercount is temporary and accurate accounting
		 * will resume once the rq has a chance to be idle.
		 */
		اگر (sd->llc_counts[pref_llc])
			sd->llc_counts[pref_llc]--;
	}
}

صحیح mm_init_sched(ساخت mm_struct *mm,
		  ساخت sched_cache_وقت __percpu *_pcpu_sched)
{
	ساخت sched_cache_گروہ *grp;
	غیرعلامتی طویل epoch = 0;
	صحیح i;

	grp = kzalloc_obj(*grp);
	اگر (!grp) {
		free_percpu(_pcpu_sched);
		mm->sched_cache_grp = تہی;
		واپس -ENOMEM;
	}

	for_each_possible_cpu(i) {
		ساخت sched_cache_وقت *pcpu_sched = فی_cpu_ptr(_pcpu_sched, i);
		ساخت rq *rq = cpu_rq(i);

		pcpu_sched->runtime = 0;
		/* a slightly stale cpu epoch is acceptible */
		pcpu_sched->epoch = rq->cpu_epoch;
		epoch = rq->cpu_epoch;
	}

	raw_spin_تالا_init(&grp->تالا);
	grp->epoch = epoch;
	grp->cpu = -1;
	grp->اگلا_جائزہ = jiffies;
	grp->nr_running_avg = 0;
	grp->footprint = 0;
	refcount_سیٹ(&grp->refcnt, 1);
	/*
	 * The update to grp->pcpu_sched should not be reordered
	 * before initialization to grp's other fields, in case
	 * the readers may get invalid mm_sched_epoch, etc.
	 */
	smp_store_release(&grp->pcpu_sched, _pcpu_sched);
	/*
	 * Publish the group last.  Not every reader qualifies it by
	 * grp->pcpu_sched - can_migrate_llc_task() only checks that the
	 * pointer is non-NULL before reading grp->footprint and
	 * grp->nr_running_avg - so a reachable group must already be
	 * fully initialized.
	 */
	smp_store_release(&mm->sched_cache_grp, grp);
	واپس 0;
}

ساکن خالی sched_cache_گروہ_free_rcu(ساخت rcu_سر *rcu)
{
	ساخت sched_cache_گروہ *grp =
		container_کا(rcu, ساخت sched_cache_گروہ, rcu);

	free_percpu(grp->pcpu_sched);
	kfree(grp);
}

ساکن خالی sched_cache_گروہ_put(ساخت sched_cache_گروہ *grp)
{
	اگر (!grp || !refcount_dec_and_test(&grp->refcnt))
		واپس;

	call_rcu(&grp->rcu, sched_cache_گروہ_free_rcu);
}

DEFINE_FREE(sched_cache_گروہ_put, ساخت sched_cache_گروہ *,
	    sched_cache_گروہ_put(_T));

#تعریف_کرو rcu_deref_sched_cache_grp(tsk) \
	rcu_dereference_check((tsk)->sched_cache_grp, (tsk) == current)

ساکن ساخت sched_cache_گروہ *sched_cache_replace_grp(ساخت task_struct *p,
							 ساخت sched_cache_گروہ *new)
{
	ساخت sched_cache_گروہ *old;

	old = rcu_deref_sched_cache_grp(p);
	rcu_تفویض_اشارہ_گر(p->sched_cache_grp, new);

	واپس old;
}

ساخت sched_cache_گروہ *sched_cache_گروہ_get(ساخت sched_cache_گروہ *grp)
{
	/*
	 * refcount_inc_not_zero() is the acquire primitive for lockless
	 * (RCU) lookups; plain refcount_inc() would scribble the count if
	 * it already reached zero. Return NULL in that case.
	 */
	اگر (grp && !refcount_inc_not_zero(&grp->refcnt))
		grp = تہی;

	واپس grp;
}

ساخت sched_cache_گروہ *task_cache_گروہ_get(ساخت task_struct *p)
{
	guard(rcu)();
	واپس sched_cache_گروہ_get(rcu_dereference(p->sched_cache_grp));
}

خالی sched_cache_fork(ساخت task_struct *p)
{
	/*
	 * The child takes its own reference on the mm's cache group, separate
	 * from the reference held by the mm. @p is not yet visible to readers,
	 * so a plain initializing store is enough.
	 */
	RCU_INIT_اشارہ_گر(p->sched_cache_grp,
			 sched_cache_گروہ_get(p->mm->sched_cache_grp));
}

خالی sched_cache_fork_cleanup(ساخت task_struct *p)
{
	/*
	 * A fork that fails after sched_cache_fork() never reaches exit_mm(),
	 * so drop the reference here. @p never became visible, so there are no
	 * concurrent readers and the reference we hold keeps the group alive.
	 */
	sched_cache_گروہ_put(rcu_access_اشارہ_گر(p->sched_cache_grp));
	RCU_INIT_اشارہ_گر(p->sched_cache_grp, تہی);
}

خالی sched_cache_exec_mmap(ساخت task_struct *p, ساخت mm_struct *mm)
{
	ساخت sched_cache_گروہ *old;

	/*
	 * Acquire the new reference before publishing the pointer, then drop
	 * the old one. @p is current and the only writer of its own pointer.
	 */
	old = sched_cache_replace_grp(p, sched_cache_گروہ_get(mm->sched_cache_grp));
	sched_cache_گروہ_put(old);
}

خالی sched_cache_exit_mm(ساخت task_struct *p)
{
	ساخت sched_cache_گروہ *grp = sched_cache_replace_grp(p, تہی);

#اگر_تعریف_شدہ CONFIG_NUMA_BALANCING
	/*
	 * Subtract this task's footprint from the group before dropping the
	 * reference, so the group footprint converges as its threads exit.
	 * Unlocked for performance; clamp to avoid underflow.
	 */
	اگر (grp && p->کل_numa_خرابیاں) {
		غیرعلامتی طویل fp = READ_ONCE(grp->footprint);
		غیرعلامتی طویل sub = min(fp, p->کل_numa_خرابیاں);

		WRITE_ONCE(grp->footprint, fp - sub);
	}
#اگر_ختم
	sched_cache_گروہ_put(grp);
}

خالی mm_destroy_sched(ساخت mm_struct *mm)
{
	sched_cache_گروہ_put(mm->sched_cache_grp);
	mm->sched_cache_grp = تہی;
}

/* because why would C be fully specified */
ساکن __always_inline خالی __shr_u64(u64 *val, غیرعلامتی صحیح n)
{
	اگر (n >= 64) {
		*val = 0;
		واپس;
	}
	*val >>= n;
}

ساکن اندرونی خالی __update_mm_sched(ساخت rq *rq,
				     ساخت sched_cache_وقت *pcpu_sched)
{
	lockdep_assert_held(&rq->cpu_epoch_تالا);

	غیرعلامتی صحیح period = max(READ_ONCE(llc_epoch_period), 1U);
	غیرعلامتی طویل n, now = jiffies;
	طویل delta = now - rq->cpu_epoch_اگلا;

	اگر (delta > 0) {
		n = (delta + period - 1) / period;
		rq->cpu_epoch += n;
		rq->cpu_epoch_اگلا += n * period;
		__shr_u64(&rq->cpu_runtime, n);
	}

	n = rq->cpu_epoch - pcpu_sched->epoch;
	اگر (n) {
		pcpu_sched->epoch += n;
		__shr_u64(&pcpu_sched->runtime, n);
	}
}

ساکن غیرعلامتی طویل fraction_mm_sched(ساخت rq *rq,
				       ساخت sched_cache_وقت *pcpu_sched)
{
	guard(raw_spinlock_irqsave)(&rq->cpu_epoch_تالا);

	__update_mm_sched(rq, pcpu_sched);

	/*
	 * Runtime is a geometric series (r=0.5) and as such will sum to twice
	 * the accumulation period, this means the multiplcation here should
	 * not overflow.
	 */
	واپس div64_u64(NICE_0_بھرو * pcpu_sched->runtime, rq->cpu_runtime + 1);
}

ساکن صحیح get_pref_llc(ساخت task_struct *p, ساخت sched_cache_گروہ *grp)
{
	صحیح mm_sched_llc = -1, mm_sched_cpu;

	اگر (!grp)
		واپس -1;

	mm_sched_cpu = READ_ONCE(grp->cpu);
	اگر (mm_sched_cpu != -1) {
		mm_sched_llc = llc_id(mm_sched_cpu);

#اگر_تعریف_شدہ CONFIG_NUMA_BALANCING
		/*
		 * Don't assign preferred LLC if it
		 * conflicts with NUMA balancing.
		 * This can happen when sched_setnuma() gets
		 * called, however it is not much of an issue
		 * because we expect account_mm_sched() to get
		 * called fairly regularly -- at a higher rate
		 * than sched_setnuma() at least -- and thus the
		 * conflict only exists for a short period of time.
		 */
		اگر (static_branch_likely(&sched_numa_balancing) &&
		    p->numa_preferred_nid >= 0 &&
		    cpu_مقصد_گرہ(mm_sched_cpu) != p->numa_preferred_nid)
			mm_sched_llc = -1;
#اگر_ختم
	}

	واپس mm_sched_llc;
}

ساکن غیرعلامتی صحیح task_running_on_cpu(صحیح cpu, ساخت task_struct *p);

ساکن اندرونی
خالی account_mm_sched(ساخت rq *rq, ساخت task_struct *p, s64 delta_exec)
{
	ساخت sched_cache_گروہ *grp = rcu_dereference_all(p->sched_cache_grp);
	ساخت sched_cache_وقت *pcpu_sched;
	صحیح mm_sched_llc = -1;
	غیرعلامتی طویل epoch;

	اگر (!sched_cache_enabled())
		واپس;

	اگر (p->sched_class != &fair_sched_class)
		واپس;
	/*
	 * init_task, kthreads and user thread created
	 * by user_mode_thread() don't have a cache group.
	 * In theory a kernel thread does not have any valid
	 * cache group, because sched_cache_fork() is not
	 * invoked for a kernel thread - !grp should gate the
	 * kernel thread. Use the PF_KTHREAD check explicitly
	 * here for safety reasons, to guard against future
	 * modifications and to pair with task_tick_cache().
	 */
	اگر (p->flags & PF_KTHREAD || !grp || !grp->pcpu_sched)
		واپس;

	pcpu_sched = فی_cpu_ptr(grp->pcpu_sched, cpu_کا(rq));

	scoped_guard (raw_spinlock, &rq->cpu_epoch_تالا) {
		__update_mm_sched(rq, pcpu_sched);
		pcpu_sched->runtime += delta_exec;
		rq->cpu_runtime += delta_exec;
		epoch = rq->cpu_epoch;
	}

	/*
	 * If this process hasn't hit task_cache_work() for a while invalidate
	 * its preferred state.
	 */
	اگر ((طویل)(epoch - READ_ONCE(grp->epoch)) > llc_epoch_affinity_timeout ||
	    invalid_llc_nr(grp, p, cpu_کا(rq)) ||
	    exceed_llc_استعداد(grp, cpu_کا(rq))) {
		اگر (READ_ONCE(grp->cpu) != -1)
			WRITE_ONCE(grp->cpu, -1);
	}

	mm_sched_llc = get_pref_llc(p, grp);

	/* task not on rq accounted later in account_entity_enqueue() */
	اگر (task_running_on_cpu(rq->cpu, p) &&
	    READ_ONCE(p->preferred_llc) != mm_sched_llc) {
		account_llc_قطار_سے_نکالو(rq, p);
		WRITE_ONCE(p->preferred_llc, mm_sched_llc);
		account_llc_قطار_میں_ڈالو(rq, p);
	}
}

ساکن خالی task_tick_cache(ساخت rq *rq, ساخت task_struct *p)
{
	ساخت sched_cache_گروہ *grp = rcu_dereference_all(p->sched_cache_grp);
	ساخت callback_سر *work = &p->cache_work;
	غیرعلامتی طویل epoch;

	اگر (!sched_cache_enabled())
		واپس;

	اگر (!grp || p->flags & PF_KTHREAD ||
	    !grp->pcpu_sched)
		واپس;

	epoch = rq->cpu_epoch;
	/* avoid moving backwards */
	اگر (وقت_after_eq(grp->epoch, epoch))
		واپس;

	guard(raw_spinlock)(&grp->تالا);

	اگر (work->اگلا == work) {
		task_work_جوڑو(p, work, TWA_RESUME);
		WRITE_ONCE(grp->epoch, epoch);
	}
}

ساکن خالی get_جائزہ_cpumasks(cpumask_var_t cpus, ساخت task_struct *p,
			      ساخت sched_cache_گروہ *grp)
{
#اگر_تعریف_شدہ CONFIG_NUMA_BALANCING
	صحیح cpu, curr_cpu, nid, pref_nid;

	اگر (!static_branch_likely(&sched_numa_balancing))
		جاؤ برآمد;

	cpu = READ_ONCE(grp->cpu);
	اگر (cpu != -1)
		nid = cpu_مقصد_گرہ(cpu);
	curr_cpu = task_cpu(p);

	/*
	 * Scanning in the preferred NUMA node is ideal. However, the NUMA
	 * preferred node is per-task rather than per-process. It is possible
	 * for different threads of the process to have distinct preferred
	 * nodes; consequently, the process-wide preferred LLC may bounce
	 * between different nodes. As a workaround, maintain the scan
	 * CPU mask to also cover the process's current preferred LLC and the
	 * current running node to mitigate the bouncing risk.
	 * TBD: numa_group should be considered during task aggregation.
	 */
	pref_nid = p->numa_preferred_nid;
	/* honor the task's preferred node */
	اگر (pref_nid == NUMA_NO_گرہ)
		جاؤ برآمد;

	cpumask_or(cpus, cpus, cpumask_کا_گرہ(pref_nid));

	/* honor the task's preferred LLC CPU */
	اگر (cpu != -1 && !cpumask_test_cpu(cpu, cpus) && nid != NUMA_NO_گرہ)
		cpumask_or(cpus, cpus, cpumask_کا_گرہ(nid));

	/* make sure the task's current running node is included */
	اگر (!cpumask_test_cpu(curr_cpu, cpus))
		cpumask_or(cpus, cpus, cpumask_کا_گرہ(cpu_مقصد_گرہ(curr_cpu)));

	واپس;

برآمد:
#اگر_ختم
	cpumask_copy(cpus, cpu_online_نقاب);
}

ساکن اندرونی خالی update_avg_scale(u64 *avg, u64 sample)
{
	صحیح ضربی_عدد = فی_cpu(sd_llc_سائز, raw_smp_processor_id());
	s64 diff = sample - *avg;
	u32 مقسوم_علیہ;

	/*
	 * Scale the divisor based on the number of CPUs contained
	 * in the LLC. This scaling ensures smaller LLC domains use
	 * a smaller divisor to achieve more precise sensitivity to
	 * changes in nr_running, while larger LLC domains are capped
	 * at a maximum divisor of 8 which is the default smoothing
	 * factor of EWMA in update_avg().
	 */
	مقسوم_علیہ = clamp_t(u32, (ضربی_عدد >> 2), 2, 8);
	*avg += div64_s64(diff, مقسوم_علیہ);
}

ساکن خالی task_cache_work(ساخت callback_سر *work)
{
	ساخت sched_cache_گروہ *grp __free(sched_cache_گروہ_put) = تہی;
	cpumask_var_t cpus __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	صحیح cpu, m_a_cpu = -1, nr_running = 0, curr_cpu;
	غیرعلامتی طویل اگلا_جائزہ, now = jiffies;
	ساخت task_struct *p = current, *cur;
	غیرعلامتی طویل curr_m_a_occ = 0;
	غیرعلامتی طویل m_a_occ = 0;

	WARN_ON_ONCE(work != &p->cache_work);

	work->اگلا = work;

	اگر (p->flags & PF_EXITING)
		واپس;

	/*
	 * A reference makes sure grp is not released by others. The rcu
	 * lock can not be held till after zalloc_cpumask_var() below,
	 * because the latter might sleep.
	 */
	grp = task_cache_گروہ_get(p);
	اگر (!grp)
		واپس;

	اگلا_جائزہ = READ_ONCE(grp->اگلا_جائزہ);
	اگر (وقت_before(now, اگلا_جائزہ))
		واپس;

	/* only 1 thread is allowed to scan */
	اگر (!try_cmpxchg(&grp->اگلا_جائزہ, &اگلا_جائزہ,
			 now + max_t(غیرعلامتی طویل,
				     READ_ONCE(llc_epoch_period), 1)))
		واپس;

	curr_cpu = task_cpu(p);
	اگر (invalid_llc_nr(grp, p, curr_cpu) ||
	    exceed_llc_استعداد(grp, curr_cpu)) {
		اگر (READ_ONCE(grp->cpu) != -1)
			WRITE_ONCE(grp->cpu, -1);

		واپس;
	}

	اگر (!zalloc_cpumask_var(&cpus, GFP_KERNEL))
		واپس;

	scoped_guard (cpus_read_تالا) {
		guard(rcu)();

		get_جائزہ_cpumasks(cpus, p, grp);

		for_each_cpu(cpu, cpus) {
			/* XXX sched_cluster_active */
			ساخت sched_domain *sd = rcu_dereference_all(فی_cpu(sd_llc, cpu));
			غیرعلامتی طویل occ, m_occ = 0, a_occ = 0;
			صحیح m_cpu = -1, i;

			اگر (!sd)
				جاری;

			for_each_cpu(i, sched_domain_وسعت(sd)) {
				occ = fraction_mm_sched(cpu_rq(i),
							فی_cpu_ptr(grp->pcpu_sched, i));
				a_occ += occ;
				اگر (occ > m_occ) {
					m_occ = occ;
					m_cpu = i;
				}

				/*
				 * rcu_access_pointer() is used because the
				 * pointer is only compared, never dereferenced.
				 */
				cur = rcu_dereference_all(cpu_rq(i)->curr);
				اگر (cur && !(cur->flags & (PF_EXITING | PF_KTHREAD)) &&
				    rcu_access_اشارہ_گر(cur->sched_cache_grp) == grp)
					nr_running++;
			}

			/*
			 * Compare the accumulated occupancy of each LLC. The
			 * reason for using accumulated occupancy rather than average
			 * per CPU occupancy is that it works better in asymmetric LLC
			 * scenarios.
			 * For example, if there are 2 threads in a 4CPU LLC and 3
			 * threads in an 8CPU LLC, it might be better to choose the one
			 * with 3 threads. However, this would not be the case if the
			 * occupancy is divided by the number of CPUs in an LLC (i.e.,
			 * if average per CPU occupancy is used).
			 * Besides, NUMA balancing fault statistics behave similarly:
			 * the total number of faults per node is compared rather than
			 * the average number of faults per CPU. This strategy is also
			 * followed here.
			 */
			اگر (a_occ > m_a_occ) {
				m_a_occ = a_occ;
				m_a_cpu = m_cpu;
			}

			اگر (llc_id(cpu) == llc_id(READ_ONCE(grp->cpu)))
				curr_m_a_occ = a_occ;

			cpumask_andnot(cpus, cpus, sched_domain_وسعت(sd));
		}
	}

	اگر (m_a_occ > (2 * curr_m_a_occ)) {
		/*
		 * Avoid switching sched_cache_grp->cpu too fast.
		 * The reason to choose 2X is because:
		 * 1. It is better to keep the preferred LLC stable,
		 *    rather than changing it frequently and cause migrations
		 * 2. 2X means the new preferred LLC has at least 1 more
		 *    busy CPU than the old one(200% vs 100%, eg)
		 * 3. 2X is chosen based on test results, as it delivers
		 *    the optimal performance gain so far.
		 */
		WRITE_ONCE(grp->cpu, m_a_cpu);
	}

	update_avg_scale(&grp->nr_running_avg, nr_running);
}

خالی init_sched_mm(ساخت task_struct *p)
{
	ساخت callback_سر *work = &p->cache_work;

	init_task_work(work, task_cache_work);
	work->اگلا = work;
	/*
	 * dup_task_struct() copies the parent's task_struct, including its
	 * sched_cache_grp, for which the child holds no reference.  Clear it
	 * here - before copy_mm() runs - so the child never carries a
	 * borrowed pointer that the fork error path would put.
	 */
	RCU_INIT_اشارہ_گر(p->sched_cache_grp, تہی);
	/*
	 * Reset new task's preference to avoid
	 * polluting account_llc_enqueue().
	 */
	p->preferred_llc = -1;
	p->pref_llc_queued = 0;
}

#ورنہ /* CONFIG_SCHED_CACHE */

ساکن اندرونی خالی account_mm_sched(ساخت rq *rq, ساخت task_struct *p,
				    s64 delta_exec) { }

خالی init_sched_mm(ساخت task_struct *p) { }

ساکن خالی task_tick_cache(ساخت rq *rq, ساخت task_struct *p) { }

ساکن اندرونی صحیح get_pref_llc(ساخت task_struct *p,
			       ساخت mm_struct *mm)
{
	واپس -1;
}

ساکن خالی account_llc_قطار_میں_ڈالو(ساخت rq *rq, ساخت task_struct *p) {}

ساکن خالی account_llc_قطار_سے_نکالو(ساخت rq *rq, ساخت task_struct *p) {}

ساکن خالی pref_llc_running_inc(ساخت rq *rq, ساخت task_struct *p) {}

ساکن خالی pref_llc_running_dec(ساخت rq *rq, ساخت task_struct *p) {}

#اگر_ختم /* CONFIG_SCHED_CACHE */

/*
 * Used by other classes to account runtime.
 */
s64 update_curr_common(ساخت rq *rq)
{
	واپس update_se(rq, &rq->donor->se);
}

/*
 * Update the current task's runtime statistics.
 */
ساکن خالی update_curr(ساخت cfs_rq *cfs_rq)
{
	/*
	 * Note: cfs_rq->curr corresponds to the task picked to
	 * run (ie: rq->donor.se) which due to proxy-exec may
	 * not necessarily be the actual task running
	 * (rq->curr.se). This is easy to confuse!
	 */
	ساخت sched_entity *curr = cfs_rq->h_curr;
	ساخت rq *rq = rq_کا(cfs_rq);
	s64 delta_exec;
	bool resched;

	اگر (unlikely(!curr))
		واپس;

	delta_exec = update_se(rq, curr);
	اگر (unlikely(delta_exec <= 0))
		واپس;

	account_cfs_rq_runtime(cfs_rq, delta_exec);

	اگر (!entity_is_task(curr))
		واپس;

	cfs_rq = &rq->cfs;

	curr->vruntime += calc_delta_fair(delta_exec, curr);
	resched = update_deadline(cfs_rq, curr);

	/*
	 * If the fair_server is active, we need to account for the
	 * fair_server time whether or not the task is running on
	 * behalf of fair_server or not:
	 *  - If the task is running on behalf of fair_server, we need
	 *    to limit its time based on the assigned runtime.
	 *  - Fair task that runs outside of fair_server should account
	 *    against fair_server such that it can account for this time
	 *    and possibly avoid running this period.
	 */
	dl_server_update(&rq->fair_server, delta_exec);

	اگر (cfs_rq->h_nr_queued == 1)
		واپس;

	اگر (resched || !protect_قاش(curr)) {
		resched_curr_lazy(rq);
		clear_buddies(cfs_rq, curr);
	}
}

ساکن خالی update_curr_fair(ساخت rq *rq)
{
	ساخت sched_entity *se = &rq->donor->se;

	for_each_sched_entity(se)
		update_curr(cfs_rq_کا(se));
}

ساکن اندرونی خالی
update_stats_wait_آغاز_fair(ساخت cfs_rq *cfs_rq, ساخت sched_entity *se)
{
	ساخت sched_statistics *stats;
	ساخت task_struct *p = تہی;

	اگر (!schedstat_enabled())
		واپس;

	stats = __schedstats_from_se(se);

	اگر (entity_is_task(se))
		p = task_کا(se);

	__update_stats_wait_start(rq_کا(cfs_rq), p, stats);
}

ساکن اندرونی خالی
update_stats_wait_انتہا_fair(ساخت cfs_rq *cfs_rq, ساخت sched_entity *se)
{
	ساخت sched_statistics *stats;
	ساخت task_struct *p = تہی;

	اگر (!schedstat_enabled())
		واپس;

	stats = __schedstats_from_se(se);

	/*
	 * When the sched_schedstat changes from 0 to 1, some sched se
	 * maybe already in the runqueue, the se->statistics.wait_start
	 * will be 0.So it will let the delta wrong. We need to avoid this
	 * scenario.
	 */
	اگر (unlikely(!schedstat_val(stats->wait_آغاز)))
		واپس;

	اگر (entity_is_task(se))
		p = task_کا(se);

	__update_stats_wait_end(rq_کا(cfs_rq), p, stats);
}

ساکن اندرونی خالی
update_stats_قطار_میں_ڈالو_sleeper_fair(ساخت cfs_rq *cfs_rq, ساخت sched_entity *se)
{
	ساخت sched_statistics *stats;
	ساخت task_struct *tsk = تہی;

	اگر (!schedstat_enabled())
		واپس;

	stats = __schedstats_from_se(se);

	اگر (entity_is_task(se))
		tsk = task_کا(se);

	__update_stats_enqueue_sleeper(rq_کا(cfs_rq), tsk, stats);
}

/*
 * Task is being enqueued - update stats:
 */
ساکن اندرونی خالی
update_stats_قطار_میں_ڈالو_fair(ساخت cfs_rq *cfs_rq, ساخت sched_entity *se, صحیح flags)
{
	اگر (!schedstat_enabled())
		واپس;

	/*
	 * Are we enqueueing a waiting task? (for current tasks
	 * a dequeue/enqueue event is a NOP)
	 */
	اگر (se != cfs_rq->h_curr)
		update_stats_wait_آغاز_fair(cfs_rq, se);

	اگر (flags & قطار_میں_ڈالو_WAKEUP)
		update_stats_قطار_میں_ڈالو_sleeper_fair(cfs_rq, se);
}

ساکن اندرونی خالی
update_stats_قطار_سے_نکالو_fair(ساخت cfs_rq *cfs_rq, ساخت sched_entity *se, صحیح flags)
{

	اگر (!schedstat_enabled())
		واپس;

	/*
	 * Mark the end of the wait period if dequeueing a
	 * waiting task:
	 */
	اگر (se != cfs_rq->h_curr)
		update_stats_wait_انتہا_fair(cfs_rq, se);

	اگر ((flags & قطار_سے_نکالو_SLEEP) && entity_is_task(se)) {
		ساخت task_struct *tsk = task_کا(se);
		غیرعلامتی صحیح state;

		/* XXX racy against TTWU */
		state = READ_ONCE(tsk->__state);
		اگر (state & TASK_INTERRUPTIBLE)
			__schedstat_set(tsk->stats.sleep_آغاز,
				      rq_clock(rq_کا(cfs_rq)));
		اگر (state & TASK_UNINTERRUPTIBLE)
			__schedstat_set(tsk->stats.بلاک_آغاز,
				      rq_clock(rq_کا(cfs_rq)));
	}
}

/*
 * We are picking a new current task - update its stats:
 */
ساکن اندرونی خالی
update_stats_curr_آغاز(ساخت cfs_rq *cfs_rq, ساخت sched_entity *se)
{
	/*
	 * We are starting a new run period:
	 */
	se->exec_آغاز = rq_clock_task(rq_کا(cfs_rq));
}

/* Check sched_smt_active before calling this to avoid overheads in fastpaths */
ساکن اندرونی bool is_core_idle(صحیح cpu)
{
	صحیح sibling;

	for_each_cpu(sibling, cpu_smt_نقاب(cpu)) {
		اگر (cpu == sibling)
			جاری;

		اگر (!idle_cpu(sibling))
			واپس false;
	}

	واپس true;
}

#اگر_تعریف_شدہ CONFIG_NUMA
#تعریف_کرو NUMA_IMBALANCE_MIN 2

ساکن اندرونی طویل
adjust_numa_imbalance(صحیح imbalance, صحیح dst_running, صحیح imb_numa_nr)
{
	/*
	 * Allow a NUMA imbalance if busy CPUs is less than the maximum
	 * threshold. Above this threshold, individual tasks may be contending
	 * for both memory bandwidth and any shared HT resources.  This is an
	 * approximation as the number of running tasks may not be related to
	 * the number of busy CPUs due to sched_setaffinity.
	 */
	اگر (dst_running > imb_numa_nr)
		واپس imbalance;

	/*
	 * Allow a small imbalance based on a simple pair of communicating
	 * tasks that remain local when the destination is lightly loaded.
	 */
	اگر (imbalance <= NUMA_IMBALANCE_MIN)
		واپس 0;

	واپس imbalance;
}
#اگر_ختم /* CONFIG_NUMA */

#اگر_تعریف_شدہ CONFIG_NUMA_BALANCING
/*
 * Approximate time to scan a full NUMA task in ms. The task scan period is
 * calculated based on the tasks virtual memory size and
 * numa_balancing_scan_size.
 */
غیرعلامتی صحیح sysctl_numa_balancing_جائزہ_period_min = 1000;
غیرعلامتی صحیح sysctl_numa_balancing_جائزہ_period_max = 60000;

/* Portion of address space to scan in MB */
غیرعلامتی صحیح sysctl_numa_balancing_جائزہ_سائز = 256;

/* Scan @scan_size MB every @scan_period after an initial @scan_delay in ms */
غیرعلامتی صحیح sysctl_numa_balancing_جائزہ_delay = 1000;

/* The page with hint page fault latency < threshold in ms is considered hot */
غیرعلامتی صحیح sysctl_numa_balancing_hot_حد = MSEC_فی_SEC;

ساخت numa_گروہ {
	refcount_t refcount;

	spinlock_t تالا; /* nr_tasks, tasks */
	صحیح nr_tasks;
	pid_t gid;
	صحیح فعال_nodes;

	ساخت rcu_سر rcu;
	غیرعلامتی طویل کل_خرابیاں;
	غیرعلامتی طویل max_خرابیاں_cpu;
	/*
	 * faults[] array is split into two regions: faults_mem and faults_cpu.
	 *
	 * Faults_cpu is used to decide whether memory should move
	 * towards the CPU. As a consequence, these stats are weighted
	 * more by CPU use than by memory faults.
	 */
	غیرعلامتی طویل خرابیاں[];
};

/*
 * For functions that can be called in multiple contexts that permit reading
 * ->numa_group (see struct task_struct for locking rules).
 */
ساکن ساخت numa_گروہ *deref_task_numa_گروہ(ساخت task_struct *p)
{
	واپس rcu_dereference_check(p->numa_گروہ, p == current ||
		(lockdep_is_held(__rq_lockp(task_rq(p))) && !READ_ONCE(p->on_cpu)));
}

ساکن ساخت numa_گروہ *deref_curr_numa_گروہ(ساخت task_struct *p)
{
	واپس rcu_dereference_protected(p->numa_گروہ, p == current);
}

ساکن اندرونی غیرعلامتی طویل گروہ_خرابیاں_priv(ساخت numa_گروہ *ng);
ساکن اندرونی غیرعلامتی طویل گروہ_خرابیاں_shared(ساخت numa_گروہ *ng);

ساکن غیرعلامتی صحیح task_nr_جائزہ_windows(ساخت task_struct *p)
{
	غیرعلامتی طویل rss = 0;
	غیرعلامتی طویل nr_جائزہ_صفحات;

	/*
	 * Calculations based on RSS as non-present and empty pages are skipped
	 * by the PTE scanner and NUMA hinting faults should be trapped based
	 * on resident pages
	 */
	nr_جائزہ_صفحات = MB_مقصد_صفحات(sysctl_numa_balancing_جائزہ_سائز);
	rss = get_mm_rss(p->mm);
	اگر (!rss)
		rss = nr_جائزہ_صفحات;

	rss = باری_اوپر(rss, nr_جائزہ_صفحات);
	واپس rss / nr_جائزہ_صفحات;
}

/* For sanity's sake, never scan more PTEs than MAX_SCAN_WINDOW MB/sec. */
#تعریف_کرو MAX_جائزہ_کھڑکی 2560

ساکن غیرعلامتی صحیح task_جائزہ_min(ساخت task_struct *p)
{
	غیرعلامتی صحیح جائزہ_سائز = READ_ONCE(sysctl_numa_balancing_جائزہ_سائز);
	غیرعلامتی صحیح جائزہ, floor;
	غیرعلامتی صحیح windows = 1;

	اگر (جائزہ_سائز < MAX_جائزہ_کھڑکی)
		windows = MAX_جائزہ_کھڑکی / جائزہ_سائز;
	floor = 1000 / windows;

	جائزہ = sysctl_numa_balancing_جائزہ_period_min / task_nr_جائزہ_windows(p);
	واپس max_t(غیرعلامتی صحیح, floor, جائزہ);
}

ساکن غیرعلامتی صحیح task_جائزہ_آغاز(ساخت task_struct *p)
{
	غیرعلامتی طویل smin = task_جائزہ_min(p);
	غیرعلامتی طویل period = smin;
	ساخت numa_گروہ *ng;

	/* Scale the maximum scan period with the amount of shared memory. */
	rcu_read_تالا();
	ng = rcu_dereference_all(p->numa_گروہ);
	اگر (ng) {
		غیرعلامتی طویل shared = گروہ_خرابیاں_shared(ng);
		غیرعلامتی طویل نجی = گروہ_خرابیاں_priv(ng);

		period *= refcount_read(&ng->refcount);
		period *= shared + 1;
		period /= نجی + shared + 1;
	}
	rcu_read_unlock();

	واپس max(smin, period);
}

ساکن غیرعلامتی صحیح task_جائزہ_max(ساخت task_struct *p)
{
	غیرعلامتی طویل smin = task_جائزہ_min(p);
	غیرعلامتی طویل smax;
	ساخت numa_گروہ *ng;

	/* Watch for min being lower than max due to floor calculations */
	smax = sysctl_numa_balancing_جائزہ_period_max / task_nr_جائزہ_windows(p);

	/* Scale the maximum scan period with the amount of shared memory. */
	ng = deref_curr_numa_گروہ(p);
	اگر (ng) {
		غیرعلامتی طویل shared = گروہ_خرابیاں_shared(ng);
		غیرعلامتی طویل نجی = گروہ_خرابیاں_priv(ng);
		غیرعلامتی طویل period = smax;

		period *= refcount_read(&ng->refcount);
		period *= shared + 1;
		period /= نجی + shared + 1;

		smax = max(smax, period);
	}

	واپس max(smin, smax);
}

ساکن خالی account_numa_قطار_میں_ڈالو(ساخت rq *rq, ساخت task_struct *p)
{
	rq->nr_numa_running += (p->numa_preferred_nid != NUMA_NO_گرہ);
	rq->nr_preferred_running += (p->numa_preferred_nid == task_گرہ(p));
}

ساکن خالی account_numa_قطار_سے_نکالو(ساخت rq *rq, ساخت task_struct *p)
{
	rq->nr_numa_running -= (p->numa_preferred_nid != NUMA_NO_گرہ);
	rq->nr_preferred_running -= (p->numa_preferred_nid == task_گرہ(p));
}

/* Shared or private faults. */
#تعریف_کرو NR_NUMA_HINT_FAULT_TYPES 2

/* Memory and CPU locality */
#تعریف_کرو NR_NUMA_HINT_FAULT_STATS (NR_NUMA_HINT_FAULT_TYPES * 2)

/* Averaged statistics, and temporary buffers. */
#تعریف_کرو NR_NUMA_HINT_FAULT_BUCKETS (NR_NUMA_HINT_FAULT_STATS * 2)

pid_t task_numa_گروہ_id(ساخت task_struct *p)
{
	ساخت numa_گروہ *ng;
	pid_t gid = 0;

	rcu_read_تالا();
	ng = rcu_dereference_all(p->numa_گروہ);
	اگر (ng)
		gid = ng->gid;
	rcu_read_unlock();

	واپس gid;
}

/*
 * The averaged statistics, shared & private, memory & CPU,
 * occupy the first half of the array. The second half of the
 * array is for current counters, which are averaged into the
 * first set by task_numa_placement.
 */
ساکن اندرونی صحیح task_خرابیاں_idx(شمار numa_خرابیاں_stats s, صحیح nid, صحیح priv)
{
	واپس NR_NUMA_HINT_FAULT_TYPES * (s * nr_گرہ_ids + nid) + priv;
}

ساکن اندرونی غیرعلامتی طویل task_خرابیاں(ساخت task_struct *p, صحیح nid)
{
	اگر (!p->numa_خرابیاں)
		واپس 0;

	واپس p->numa_خرابیاں[task_خرابیاں_idx(NUMA_MEM, nid, 0)] +
		p->numa_خرابیاں[task_خرابیاں_idx(NUMA_MEM, nid, 1)];
}

ساکن اندرونی غیرعلامتی طویل گروہ_خرابیاں(ساخت task_struct *p, صحیح nid)
{
	ساخت numa_گروہ *ng = deref_task_numa_گروہ(p);

	اگر (!ng)
		واپس 0;

	واپس ng->خرابیاں[task_خرابیاں_idx(NUMA_MEM, nid, 0)] +
		ng->خرابیاں[task_خرابیاں_idx(NUMA_MEM, nid, 1)];
}

ساکن اندرونی غیرعلامتی طویل گروہ_خرابیاں_cpu(ساخت numa_گروہ *گروہ, صحیح nid)
{
	واپس گروہ->خرابیاں[task_خرابیاں_idx(NUMA_CPU, nid, 0)] +
		گروہ->خرابیاں[task_خرابیاں_idx(NUMA_CPU, nid, 1)];
}

ساکن اندرونی غیرعلامتی طویل گروہ_خرابیاں_priv(ساخت numa_گروہ *ng)
{
	غیرعلامتی طویل خرابیاں = 0;
	صحیح گرہ;

	for_each_online_گرہ(گرہ) {
		خرابیاں += ng->خرابیاں[task_خرابیاں_idx(NUMA_MEM, گرہ, 1)];
	}

	واپس خرابیاں;
}

ساکن اندرونی غیرعلامتی طویل گروہ_خرابیاں_shared(ساخت numa_گروہ *ng)
{
	غیرعلامتی طویل خرابیاں = 0;
	صحیح گرہ;

	for_each_online_گرہ(گرہ) {
		خرابیاں += ng->خرابیاں[task_خرابیاں_idx(NUMA_MEM, گرہ, 0)];
	}

	واپس خرابیاں;
}

/*
 * A node triggering more than 1/3 as many NUMA faults as the maximum is
 * considered part of a numa group's pseudo-interleaving set. Migrations
 * between these nodes are slowed down, to allow things to settle down.
 */
#تعریف_کرو فعال_گرہ_FRACTION 3

ساکن bool numa_is_فعال_گرہ(صحیح nid, ساخت numa_گروہ *ng)
{
	واپس گروہ_خرابیاں_cpu(ng, nid) * فعال_گرہ_FRACTION > ng->max_خرابیاں_cpu;
}

/* Handle placement on systems where not all nodes are directly connected. */
ساکن غیرعلامتی طویل score_nearby_nodes(ساخت task_struct *p, صحیح nid,
					صحیح lim_dist, bool task)
{
	غیرعلامتی طویل score = 0;
	صحیح گرہ, max_dist;

	/*
	 * All nodes are directly connected, and the same distance
	 * from each other. No need for fancy placement algorithms.
	 */
	اگر (sched_numa_topology_type == NUMA_براہ_راست)
		واپس 0;

	/* sched_max_numa_distance may be changed in parallel. */
	max_dist = READ_ONCE(sched_max_numa_فاصلہ);
	/*
	 * This code is called for each node, introducing N^2 complexity,
	 * which should be OK given the number of nodes rarely exceeds 8.
	 */
	for_each_online_گرہ(گرہ) {
		غیرعلامتی طویل خرابیاں;
		صحیح dist = گرہ_فاصلہ(nid, گرہ);

		/*
		 * The furthest away nodes in the system are not interesting
		 * for placement; nid was already counted.
		 */
		اگر (dist >= max_dist || گرہ == nid)
			جاری;

		/*
		 * On systems with a backplane NUMA topology, compare groups
		 * of nodes, and move tasks towards the group with the most
		 * memory accesses. When comparing two nodes at distance
		 * "hoplimit", only nodes closer by than "hoplimit" are part
		 * of each group. Skip other nodes.
		 */
		اگر (sched_numa_topology_type == NUMA_BACKPLANE && dist >= lim_dist)
			جاری;

		/* Add up the faults from nearby nodes. */
		اگر (task)
			خرابیاں = task_خرابیاں(p, گرہ);
		ورنہ
			خرابیاں = گروہ_خرابیاں(p, گرہ);

		/*
		 * On systems with a glueless mesh NUMA topology, there are
		 * no fixed "groups of nodes". Instead, nodes that are not
		 * directly connected bounce traffic through intermediate
		 * nodes; a numa_group can occupy any set of nodes.
		 * The further away a node is, the less the faults count.
		 * This seems to result in good task placement.
		 */
		اگر (sched_numa_topology_type == NUMA_GLUELESS_MESH) {
			خرابیاں *= (max_dist - dist);
			خرابیاں /= (max_dist - LOCAL_فاصلہ);
		}

		score += خرابیاں;
	}

	واپس score;
}

/*
 * These return the fraction of accesses done by a particular task, or
 * task group, on a particular numa node.  The group weight is given a
 * larger multiplier, in order to group tasks together that are almost
 * evenly spread out between numa nodes.
 */
ساکن اندرونی غیرعلامتی طویل task_وزن(ساخت task_struct *p, صحیح nid,
					صحیح dist)
{
	غیرعلامتی طویل خرابیاں, کل_خرابیاں;

	اگر (!p->numa_خرابیاں)
		واپس 0;

	کل_خرابیاں = p->کل_numa_خرابیاں;

	اگر (!کل_خرابیاں)
		واپس 0;

	خرابیاں = task_خرابیاں(p, nid);
	خرابیاں += score_nearby_nodes(p, nid, dist, true);

	واپس 1000 * خرابیاں / کل_خرابیاں;
}

ساکن اندرونی غیرعلامتی طویل گروہ_وزن(ساخت task_struct *p, صحیح nid,
					 صحیح dist)
{
	ساخت numa_گروہ *ng = deref_task_numa_گروہ(p);
	غیرعلامتی طویل خرابیاں, کل_خرابیاں;

	اگر (!ng)
		واپس 0;

	کل_خرابیاں = ng->کل_خرابیاں;

	اگر (!کل_خرابیاں)
		واپس 0;

	خرابیاں = گروہ_خرابیاں(p, nid);
	خرابیاں += score_nearby_nodes(p, nid, dist, false);

	واپس 1000 * خرابیاں / کل_خرابیاں;
}

/*
 * If memory tiering mode is enabled, cpupid of slow memory page is
 * used to record scan time instead of CPU and PID.  When tiering mode
 * is disabled at run time, the scan time (in cpupid) will be
 * interpreted as CPU and PID.  So CPU needs to be checked to avoid to
 * access out of array bound.
 */
ساکن اندرونی bool cpupid_valid(صحیح cpupid)
{
	واپس cpupid_مقصد_cpu(cpupid) < nr_cpu_ids;
}

/*
 * For memory tiering mode, if there are enough free pages (more than
 * enough watermark defined here) in fast memory node, to take full
 * advantage of fast memory capacity, all recently accessed slow
 * memory pages will be migrated to fast memory node without
 * considering hot threshold.
 */
ساکن bool pgdat_free_space_enough(ساخت pglist_ڈیٹا *pgdat)
{
	صحیح z;
	غیرعلامتی طویل enough_wmark;

	enough_wmark = max(1UL * 1024 * 1024 * 1024 >> صفحہ_سرکاؤ,
			   pgdat->گرہ_present_صفحات >> 4);
	برائے (z = pgdat->nr_zones - 1; z >= 0; z--) {
		ساخت zone *zone = pgdat->گرہ_zones + z;

		اگر (!populated_zone(zone))
			جاری;

		اگر (zone_watermark_ok(zone, 0,
				      promo_wmark_صفحات(zone) + enough_wmark,
				      ZONE_MOVABLE, 0))
			واپس true;
	}
	واپس false;
}

/*
 * For memory tiering mode, when page tables are scanned, the scan
 * time will be recorded in struct page in addition to make page
 * PROT_NONE for slow memory page.  So when the page is accessed, in
 * hint page fault handler, the hint page fault latency is calculated
 * via,
 *
 *	hint page fault latency = hint page fault time - scan time
 *
 * The smaller the hint page fault latency, the higher the possibility
 * for the page to be hot.
 */
ساکن صحیح numa_hint_fault_latency(ساخت folio *folio)
{
	صحیح آخری_وقت, وقت;

	وقت = jiffies_مقصد_msecs(jiffies);
	آخری_وقت = folio_xchg_access_وقت(folio, وقت);

	واپس (وقت - آخری_وقت) & صفحہ_ACCESS_وقت_نقاب;
}

/*
 * For memory tiering mode, too high promotion/demotion throughput may
 * hurt application latency.  So we provide a mechanism to rate limit
 * the number of pages that are tried to be promoted.
 */
ساکن bool numa_promotion_شرح_حد_بندی(ساخت pglist_ڈیٹا *pgdat,
				      غیرعلامتی طویل شرح_حد_بندی, صحیح nr)
{
	غیرعلامتی طویل nr_cand;
	غیرعلامتی صحیح now, آغاز;

	now = jiffies_مقصد_msecs(jiffies);
	mod_گرہ_صفحہ_state(pgdat, PGPROMOTE_CANDIDATE, nr);
	nr_cand = گرہ_صفحہ_state(pgdat, PGPROMOTE_CANDIDATE);
	آغاز = pgdat->nbp_rl_آغاز;
	اگر (now - آغاز > MSEC_فی_SEC &&
	    cmpxchg(&pgdat->nbp_rl_آغاز, آغاز, now) == آغاز)
		pgdat->nbp_rl_nr_cand = nr_cand;
	اگر (nr_cand - pgdat->nbp_rl_nr_cand >= شرح_حد_بندی)
		واپس true;
	واپس false;
}

#تعریف_کرو NUMA_MIGRATION_ADJUST_اقدام	16

ساکن خالی numa_promotion_adjust_حد(ساخت pglist_ڈیٹا *pgdat,
					    غیرعلامتی طویل شرح_حد_بندی,
					    غیرعلامتی صحیح ref_th)
{
	غیرعلامتی صحیح now, آغاز, th_period, unit_th, th;
	غیرعلامتی طویل nr_cand, ref_cand, diff_cand;

	now = jiffies_مقصد_msecs(jiffies);
	th_period = sysctl_numa_balancing_جائزہ_period_max;
	آغاز = pgdat->nbp_th_آغاز;
	اگر (now - آغاز > th_period &&
	    cmpxchg(&pgdat->nbp_th_آغاز, آغاز, now) == آغاز) {
		ref_cand = شرح_حد_بندی *
			sysctl_numa_balancing_جائزہ_period_max / MSEC_فی_SEC;
		nr_cand = گرہ_صفحہ_state(pgdat, PGPROMOTE_CANDIDATE);
		diff_cand = nr_cand - pgdat->nbp_th_nr_cand;
		unit_th = ref_th * 2 / NUMA_MIGRATION_ADJUST_اقدام;
		th = pgdat->nbp_حد ? : ref_th;
		اگر (diff_cand > ref_cand * 11 / 10)
			th = max(th - unit_th, unit_th);
		ورنہ اگر (diff_cand < ref_cand * 9 / 10)
			th = min(th + unit_th, ref_th * 2);
		pgdat->nbp_th_nr_cand = nr_cand;
		pgdat->nbp_حد = th;
	}
}

bool should_numa_migrate_حافظہ(ساخت task_struct *p, ساخت folio *folio,
				صحیح src_nid, صحیح dst_cpu)
{
	ساخت numa_گروہ *ng = deref_curr_numa_گروہ(p);
	صحیح dst_nid = cpu_مقصد_گرہ(dst_cpu);
	صحیح آخری_cpupid, this_cpupid;

	/*
	 * Cannot migrate to memoryless nodes.
	 */
	اگر (!گرہ_state(dst_nid, N_حافظہ))
		واپس false;

	/*
	 * The pages in slow memory node should be migrated according
	 * to hot/cold instead of private/shared.
	 */
	اگر (folio_use_access_وقت(folio)) {
		ساخت pglist_ڈیٹا *pgdat;
		غیرعلامتی طویل شرح_حد_بندی;
		غیرعلامتی صحیح latency, th, def_th;
		طویل nr = folio_nr_صفحات(folio);

		pgdat = گرہ_ڈیٹا(dst_nid);
		اگر (pgdat_free_space_enough(pgdat)) {
			/* workload changed, reset hot threshold */
			pgdat->nbp_حد = 0;
			mod_گرہ_صفحہ_state(pgdat, PGPROMOTE_CANDIDATE_NRL, nr);
			واپس true;
		}

		def_th = sysctl_numa_balancing_hot_حد;
		شرح_حد_بندی = MB_مقصد_صفحات(sysctl_numa_balancing_promote_شرح_حد_بندی);
		numa_promotion_adjust_حد(pgdat, شرح_حد_بندی, def_th);

		th = pgdat->nbp_حد ? : def_th;
		latency = numa_hint_fault_latency(folio);
		اگر (latency >= th)
			واپس false;

		واپس !numa_promotion_شرح_حد_بندی(pgdat, شرح_حد_بندی, nr);
	}

	this_cpupid = cpu_pid_مقصد_cpupid(dst_cpu, current->pid);
	آخری_cpupid = folio_xchg_آخری_cpupid(folio, this_cpupid);

	اگر (!(sysctl_numa_balancing_منوال & NUMA_BALANCING_حافظہ_TIERING) &&
	    !گرہ_is_toptier(src_nid) && !cpupid_valid(آخری_cpupid))
		واپس false;

	/*
	 * Allow first faults or private faults to migrate immediately early in
	 * the lifetime of a task. The magic number 4 is based on waiting for
	 * two full passes of the "multi-stage node selection" test that is
	 * executed below.
	 */
	اگر ((p->numa_preferred_nid == NUMA_NO_گرہ || p->numa_جائزہ_seq <= 4) &&
	    (cpupid_pid_unset(آخری_cpupid) || cpupid_مطابقت_pid(p, آخری_cpupid)))
		واپس true;

	/*
	 * Multi-stage node selection is used in conjunction with a periodic
	 * migration fault to build a temporal task<->page relation. By using
	 * a two-stage filter we remove short/unlikely relations.
	 *
	 * Using P(p) ~ n_p / n_t as per frequentist probability, we can equate
	 * a task's usage of a particular page (n_p) per total usage of this
	 * page (n_t) (in a given time-span) to a probability.
	 *
	 * Our periodic faults will sample this probability and getting the
	 * same result twice in a row, given these samples are fully
	 * independent, is then given by P(n)^2, provided our sample period
	 * is sufficiently short compared to the usage pattern.
	 *
	 * This quadric squishes small probabilities, making it less likely we
	 * act on an unlikely task<->page relation.
	 */
	اگر (!cpupid_pid_unset(آخری_cpupid) &&
				cpupid_مقصد_nid(آخری_cpupid) != dst_nid)
		واپس false;

	/* Always allow migrate on private faults */
	اگر (cpupid_مطابقت_pid(p, آخری_cpupid))
		واپس true;

	/* A shared fault, but p->numa_group has not been set up yet. */
	اگر (!ng)
		واپس true;

	/*
	 * Destination node is much more heavily used than the source
	 * node? Allow migration.
	 */
	اگر (گروہ_خرابیاں_cpu(ng, dst_nid) > گروہ_خرابیاں_cpu(ng, src_nid) *
					فعال_گرہ_FRACTION)
		واپس true;

	/*
	 * Distribute memory according to CPU & memory use on each node,
	 * with 3/4 hysteresis to avoid unnecessary memory migrations:
	 *
	 * faults_cpu(dst)   3   faults_cpu(src)
	 * --------------- * - > ---------------
	 * faults_mem(dst)   4   faults_mem(src)
	 */
	واپس گروہ_خرابیاں_cpu(ng, dst_nid) * گروہ_خرابیاں(p, src_nid) * 3 >
	       گروہ_خرابیاں_cpu(ng, src_nid) * گروہ_خرابیاں(p, dst_nid) * 4;
}

/*
 * 'numa_type' describes the node at the moment of load balancing.
 */
شمار numa_type {
	/* The node has spare capacity that can be used to run more tasks.  */
	گرہ_has_فالتو = 0,
	/*
	 * The node is fully used and the tasks don't compete for more CPU
	 * cycles. Nevertheless, some tasks might wait before running.
	 */
	گرہ_fully_busy,
	/*
	 * The node is overloaded and can't provide expected CPU cycles to all
	 * tasks.
	 */
	گرہ_overloaded
};

/* Cached statistics for all CPUs within a node */
ساخت numa_stats {
	غیرعلامتی طویل بھرو;
	غیرعلامتی طویل runnable;
	غیرعلامتی طویل util;
	/* Total compute capacity of CPUs on a node */
	غیرعلامتی طویل compute_استعداد;
	غیرعلامتی صحیح nr_running;
	غیرعلامتی صحیح وزن;
	شمار numa_type گرہ_type;
	صحیح idle_cpu;
};

ساخت task_numa_env {
	ساخت task_struct *p;

	صحیح src_cpu, src_nid;
	صحیح dst_cpu, dst_nid;
	صحیح imb_numa_nr;

	ساخت numa_stats src_stats, dst_stats;

	صحیح imbalance_pct;
	صحیح dist;

	ساخت task_struct *بہترین_task;
	طویل بہترین_imp;
	صحیح بہترین_cpu;
};

ساکن غیرعلامتی طویل cpu_بھرو(ساخت rq *rq);
ساکن غیرعلامتی طویل cpu_runnable(ساخت rq *rq);

ساکن اندرونی شمار
numa_type numa_classify(غیرعلامتی صحیح imbalance_pct,
			 ساخت numa_stats *ns)
{
	اگر ((ns->nr_running > ns->وزن) &&
	    (((ns->compute_استعداد * 100) < (ns->util * imbalance_pct)) ||
	     ((ns->compute_استعداد * imbalance_pct) < (ns->runnable * 100))))
		واپس گرہ_overloaded;

	اگر ((ns->nr_running < ns->وزن) ||
	    (((ns->compute_استعداد * 100) > (ns->util * imbalance_pct)) &&
	     ((ns->compute_استعداد * imbalance_pct) > (ns->runnable * 100))))
		واپس گرہ_has_فالتو;

	واپس گرہ_fully_busy;
}

/* Forward declarations of select_idle_sibling helpers */
ساکن اندرونی bool test_idle_cores(صحیح cpu);
ساکن اندرونی صحیح numa_idle_core(صحیح idle_core, صحیح cpu)
{
	اگر (!sched_smt_فعال() ||
	    idle_core >= 0 || !test_idle_cores(cpu))
		واپس idle_core;

	/*
	 * Prefer cores instead of packing HT siblings
	 * and triggering future load balancing.
	 */
	اگر (is_core_idle(cpu))
		idle_core = cpu;

	واپس idle_core;
}

/*
 * Gather all necessary information to make NUMA balancing placement
 * decisions that are compatible with standard load balancer. This
 * borrows code and logic from update_sg_lb_stats but sharing a
 * common implementation is impractical.
 */
ساکن خالی update_numa_stats(ساخت task_numa_env *env,
			      ساخت numa_stats *ns, صحیح nid,
			      bool ڈھونڈو_idle)
{
	صحیح cpu, idle_core = -1;

	memset(ns, 0, حجم(*ns));
	ns->idle_cpu = -1;

	rcu_read_تالا();
	for_each_cpu(cpu, cpumask_کا_گرہ(nid)) {
		ساخت rq *rq = cpu_rq(cpu);

		ns->بھرو += cpu_بھرو(rq);
		ns->runnable += cpu_runnable(rq);
		ns->util += cpu_util_cfs(cpu);
		ns->nr_running += rq->cfs.h_nr_runnable;
		ns->compute_استعداد += استعداد_کا(cpu);

		اگر (ڈھونڈو_idle && idle_core < 0 && !rq->nr_running && idle_cpu(cpu)) {
			اگر (READ_ONCE(rq->numa_migrate_on) ||
			    !cpumask_test_cpu(cpu, env->p->cpus_ptr))
				جاری;

			اگر (ns->idle_cpu == -1)
				ns->idle_cpu = cpu;

			idle_core = numa_idle_core(idle_core, cpu);
		}
	}
	rcu_read_unlock();

	ns->وزن = cpumask_وزن(cpumask_کا_گرہ(nid));

	ns->گرہ_type = numa_classify(env->imbalance_pct, ns);

	اگر (idle_core >= 0)
		ns->idle_cpu = idle_core;
}

ساکن خالی task_numa_تفویض(ساخت task_numa_env *env,
			     ساخت task_struct *p, طویل imp)
{
	ساخت rq *rq = cpu_rq(env->dst_cpu);

	/* Check if run-queue part of active NUMA balance. */
	اگر (env->بہترین_cpu != env->dst_cpu && xchg(&rq->numa_migrate_on, 1)) {
		صحیح cpu;
		صحیح آغاز = env->dst_cpu;

		/* Find alternative idle CPU. */
		for_each_cpu_wrap(cpu, cpumask_کا_گرہ(env->dst_nid), آغاز + 1) {
			اگر (cpu == env->بہترین_cpu || !idle_cpu(cpu) ||
			    !cpumask_test_cpu(cpu, env->p->cpus_ptr)) {
				جاری;
			}

			env->dst_cpu = cpu;
			rq = cpu_rq(env->dst_cpu);
			اگر (!xchg(&rq->numa_migrate_on, 1))
				جاؤ تفویض;
		}

		/* Failed to find an alternative idle CPU */
		واپس;
	}

تفویض:
	/*
	 * Clear previous best_cpu/rq numa-migrate flag, since task now
	 * found a better CPU to move/swap.
	 */
	اگر (env->بہترین_cpu != -1 && env->بہترین_cpu != env->dst_cpu) {
		rq = cpu_rq(env->بہترین_cpu);
		WRITE_ONCE(rq->numa_migrate_on, 0);
	}

	اگر (env->بہترین_task)
		put_task_struct(env->بہترین_task);
	اگر (p)
		get_task_struct(p);

	env->بہترین_task = p;
	env->بہترین_imp = imp;
	env->بہترین_cpu = env->dst_cpu;
}

ساکن bool بھرو_too_imbalanced(طویل src_بھرو, طویل dst_بھرو,
				ساخت task_numa_env *env)
{
	طویل imb, old_imb;
	طویل orig_src_بھرو, orig_dst_بھرو;
	طویل src_استعداد, dst_استعداد;

	/*
	 * The load is corrected for the CPU capacity available on each node.
	 *
	 * src_load        dst_load
	 * ------------ vs ---------
	 * src_capacity    dst_capacity
	 */
	src_استعداد = env->src_stats.compute_استعداد;
	dst_استعداد = env->dst_stats.compute_استعداد;

	imb = مطلق_قدر(dst_بھرو * src_استعداد - src_بھرو * dst_استعداد);

	orig_src_بھرو = env->src_stats.بھرو;
	orig_dst_بھرو = env->dst_stats.بھرو;

	old_imb = مطلق_قدر(orig_dst_بھرو * src_استعداد - orig_src_بھرو * dst_استعداد);

	/* Would this change make things worse? */
	واپس (imb > old_imb);
}

/*
 * Maximum NUMA importance can be 1998 (2*999);
 * SMALLIMP @ 30 would be close to 1998/64.
 * Used to deter task migration.
 */
#تعریف_کرو SMALLIMP	30

/*
 * This checks if the overall compute and NUMA accesses of the system would
 * be improved if the source tasks was migrated to the target dst_cpu taking
 * into account that it might be best if task running on the dst_cpu should
 * be exchanged with the source task
 */
ساکن bool task_numa_compare(ساخت task_numa_env *env,
			      طویل taskimp, طویل groupimp, bool maymove)
{
	ساخت numa_گروہ *cur_ng, *p_ng = deref_curr_numa_گروہ(env->p);
	ساخت rq *dst_rq = cpu_rq(env->dst_cpu);
	طویل imp = p_ng ? groupimp : taskimp;
	ساخت task_struct *cur;
	طویل src_بھرو, dst_بھرو;
	صحیح dist = env->dist;
	طویل moveimp = imp;
	طویل بھرو;
	bool stopsearch = false;

	اگر (READ_ONCE(dst_rq->numa_migrate_on))
		واپس false;

	rcu_read_تالا();
	cur = rcu_dereference_all(dst_rq->curr);
	اگر (cur && ((cur->flags & (PF_EXITING | PF_KTHREAD)) ||
		    !cur->mm))
		cur = تہی;

	/*
	 * Because we have preemption enabled we can get migrated around and
	 * end try selecting ourselves (current == env->p) as a swap candidate.
	 */
	اگر (cur == env->p) {
		stopsearch = true;
		جاؤ unlock;
	}

	اگر (!cur) {
		اگر (maymove && moveimp >= env->بہترین_imp)
			جاؤ تفویض;
		ورنہ
			جاؤ unlock;
	}

	/* Skip this swap candidate if cannot move to the source cpu. */
	اگر (!cpumask_test_cpu(env->src_cpu, cur->cpus_ptr))
		جاؤ unlock;

	/*
	 * Skip this swap candidate if it is not moving to its preferred
	 * node and the best task is.
	 */
	اگر (env->بہترین_task &&
	    env->بہترین_task->numa_preferred_nid == env->src_nid &&
	    cur->numa_preferred_nid != env->src_nid) {
		جاؤ unlock;
	}

	/*
	 * "imp" is the fault differential for the source task between the
	 * source and destination node. Calculate the total differential for
	 * the source task and potential destination task. The more negative
	 * the value is, the more remote accesses that would be expected to
	 * be incurred if the tasks were swapped.
	 *
	 * If dst and source tasks are in the same NUMA group, or not
	 * in any group then look only at task weights.
	 */
	cur_ng = rcu_dereference_all(cur->numa_گروہ);
	اگر (cur_ng == p_ng) {
		/*
		 * Do not swap within a group or between tasks that have
		 * no group if there is spare capacity. Swapping does
		 * not address the load imbalance and helps one task at
		 * the cost of punishing another.
		 */
		اگر (env->dst_stats.گرہ_type == گرہ_has_فالتو)
			جاؤ unlock;

		imp = taskimp + task_وزن(cur, env->src_nid, dist) -
		      task_وزن(cur, env->dst_nid, dist);
		/*
		 * Add some hysteresis to prevent swapping the
		 * tasks within a group over tiny differences.
		 */
		اگر (cur_ng)
			imp -= imp / 16;
	} ورنہ {
		/*
		 * Compare the group weights. If a task is all by itself
		 * (not part of a group), use the task weight instead.
		 */
		اگر (cur_ng && p_ng)
			imp += گروہ_وزن(cur, env->src_nid, dist) -
			       گروہ_وزن(cur, env->dst_nid, dist);
		ورنہ
			imp += task_وزن(cur, env->src_nid, dist) -
			       task_وزن(cur, env->dst_nid, dist);
	}

	/* Discourage picking a task already on its preferred node */
	اگر (cur->numa_preferred_nid == env->dst_nid)
		imp -= imp / 16;

	/*
	 * Encourage picking a task that moves to its preferred node.
	 * This potentially makes imp larger than it's maximum of
	 * 1998 (see SMALLIMP and task_weight for why) but in this
	 * case, it does not matter.
	 */
	اگر (cur->numa_preferred_nid == env->src_nid)
		imp += imp / 8;

	اگر (maymove && moveimp > imp && moveimp > env->بہترین_imp) {
		imp = moveimp;
		cur = تہی;
		جاؤ تفویض;
	}

	/*
	 * Prefer swapping with a task moving to its preferred node over a
	 * task that is not.
	 */
	اگر (env->بہترین_task && cur->numa_preferred_nid == env->src_nid &&
	    env->بہترین_task->numa_preferred_nid != env->src_nid) {
		جاؤ تفویض;
	}

	/*
	 * If the NUMA importance is less than SMALLIMP,
	 * task migration might only result in ping pong
	 * of tasks and also hurt performance due to cache
	 * misses.
	 */
	اگر (imp < SMALLIMP || imp <= env->بہترین_imp + SMALLIMP / 2)
		جاؤ unlock;

	/*
	 * In the overloaded case, try and keep the load balanced.
	 */
	بھرو = task_h_بھرو(env->p) - task_h_بھرو(cur);
	اگر (!بھرو)
		جاؤ تفویض;

	dst_بھرو = env->dst_stats.بھرو + بھرو;
	src_بھرو = env->src_stats.بھرو - بھرو;

	اگر (بھرو_too_imbalanced(src_بھرو, dst_بھرو, env))
		جاؤ unlock;

تفویض:
	/* Evaluate an idle CPU for a task numa move. */
	اگر (!cur) {
		صحیح cpu = env->dst_stats.idle_cpu;

		/* Nothing cached so current CPU went idle since the search. */
		اگر (cpu < 0)
			cpu = env->dst_cpu;

		/*
		 * If the CPU is no longer truly idle and the previous best CPU
		 * is, keep using it.
		 */
		اگر (!idle_cpu(cpu) && env->بہترین_cpu >= 0 &&
		    idle_cpu(env->بہترین_cpu)) {
			cpu = env->بہترین_cpu;
		}

		env->dst_cpu = cpu;
	}

	task_numa_تفویض(env, cur, imp);

	/*
	 * If a move to idle is allowed because there is capacity or load
	 * balance improves then stop the search. While a better swap
	 * candidate may exist, a search is not free.
	 */
	اگر (maymove && !cur && env->بہترین_cpu >= 0 && idle_cpu(env->بہترین_cpu))
		stopsearch = true;

	/*
	 * If a swap candidate must be identified and the current best task
	 * moves its preferred node then stop the search.
	 */
	اگر (!maymove && env->بہترین_task &&
	    env->بہترین_task->numa_preferred_nid == env->src_nid) {
		stopsearch = true;
	}
unlock:
	rcu_read_unlock();

	واپس stopsearch;
}

ساکن خالی task_numa_ڈھونڈو_cpu(ساخت task_numa_env *env,
				طویل taskimp, طویل groupimp)
{
	bool maymove = false;
	صحیح cpu;

	/*
	 * If dst node has spare capacity, then check if there is an
	 * imbalance that would be overruled by the load balancer.
	 */
	اگر (env->dst_stats.گرہ_type == گرہ_has_فالتو) {
		غیرعلامتی صحیح imbalance;
		صحیح src_running, dst_running;

		/*
		 * Would movement cause an imbalance? Note that if src has
		 * more running tasks that the imbalance is ignored as the
		 * move improves the imbalance from the perspective of the
		 * CPU load balancer.
		 * */
		src_running = env->src_stats.nr_running - 1;
		dst_running = env->dst_stats.nr_running + 1;
		imbalance = max(0, dst_running - src_running);
		imbalance = adjust_numa_imbalance(imbalance, dst_running,
						  env->imb_numa_nr);

		/* Use idle CPU if there is no imbalance */
		اگر (!imbalance) {
			maymove = true;
			اگر (env->dst_stats.idle_cpu >= 0) {
				env->dst_cpu = env->dst_stats.idle_cpu;
				task_numa_تفویض(env, تہی, 0);
				واپس;
			}
		}
	} ورنہ {
		طویل src_بھرو, dst_بھرو, بھرو;
		/*
		 * If the improvement from just moving env->p direction is better
		 * than swapping tasks around, check if a move is possible.
		 */
		بھرو = task_h_بھرو(env->p);
		dst_بھرو = env->dst_stats.بھرو + بھرو;
		src_بھرو = env->src_stats.بھرو - بھرو;
		maymove = !بھرو_too_imbalanced(src_بھرو, dst_بھرو, env);
	}

	/* Skip CPUs if the source task cannot migrate */
	for_each_cpu_and(cpu, cpumask_کا_گرہ(env->dst_nid), env->p->cpus_ptr) {
		env->dst_cpu = cpu;
		اگر (task_numa_compare(env, taskimp, groupimp, maymove))
			توڑو;
	}
}

ساکن صحیح task_numa_migrate(ساخت task_struct *p)
{
	ساخت task_numa_env env = {
		.p = p,

		.src_cpu = task_cpu(p),
		.src_nid = task_گرہ(p),

		.imbalance_pct = 112,

		.بہترین_task = تہی,
		.بہترین_imp = 0,
		.بہترین_cpu = -1,
	};
	غیرعلامتی طویل taskweight, groupweight;
	ساخت sched_domain *sd;
	طویل taskimp, groupimp;
	ساخت numa_گروہ *ng;
	ساخت rq *بہترین_rq;
	صحیح nid, ret, dist;

	/*
	 * Pick the lowest SD_NUMA domain, as that would have the smallest
	 * imbalance and would be the first to start moving tasks about.
	 *
	 * And we want to avoid any moving of tasks about, as that would create
	 * random movement of tasks -- counter the numa conditions we're trying
	 * to satisfy here.
	 */
	rcu_read_تالا();
	sd = rcu_dereference_all(فی_cpu(sd_numa, env.src_cpu));
	اگر (sd) {
		env.imbalance_pct = 100 + (sd->imbalance_pct - 100) / 2;
		env.imb_numa_nr = sd->imb_numa_nr;
	}
	rcu_read_unlock();

	/*
	 * Cpusets can break the scheduler domain tree into smaller
	 * balance domains, some of which do not cross NUMA boundaries.
	 * Tasks that are "trapped" in such domains cannot be migrated
	 * elsewhere, so there is no point in (re)trying.
	 */
	اگر (unlikely(!sd)) {
		sched_setnuma(p, task_گرہ(p));
		واپس -EINVAL;
	}

	env.dst_nid = p->numa_preferred_nid;
	dist = env.dist = گرہ_فاصلہ(env.src_nid, env.dst_nid);
	taskweight = task_وزن(p, env.src_nid, dist);
	groupweight = گروہ_وزن(p, env.src_nid, dist);
	update_numa_stats(&env, &env.src_stats, env.src_nid, false);
	taskimp = task_وزن(p, env.dst_nid, dist) - taskweight;
	groupimp = گروہ_وزن(p, env.dst_nid, dist) - groupweight;
	update_numa_stats(&env, &env.dst_stats, env.dst_nid, true);

	/* Try to find a spot on the preferred nid. */
	task_numa_ڈھونڈو_cpu(&env, taskimp, groupimp);

	/*
	 * Look at other nodes in these cases:
	 * - there is no space available on the preferred_nid
	 * - the task is part of a numa_group that is interleaved across
	 *   multiple NUMA nodes; in order to better consolidate the group,
	 *   we need to check other locations.
	 */
	ng = deref_curr_numa_گروہ(p);
	اگر (env.بہترین_cpu == -1 || (ng && ng->فعال_nodes > 1)) {
		for_each_گرہ_state(nid, N_CPU) {
			اگر (nid == env.src_nid || nid == p->numa_preferred_nid)
				جاری;

			dist = گرہ_فاصلہ(env.src_nid, env.dst_nid);
			اگر (sched_numa_topology_type == NUMA_BACKPLANE &&
						dist != env.dist) {
				taskweight = task_وزن(p, env.src_nid, dist);
				groupweight = گروہ_وزن(p, env.src_nid, dist);
			}

			/* Only consider nodes where both task and groups benefit */
			taskimp = task_وزن(p, nid, dist) - taskweight;
			groupimp = گروہ_وزن(p, nid, dist) - groupweight;
			اگر (taskimp < 0 && groupimp < 0)
				جاری;

			env.dist = dist;
			env.dst_nid = nid;
			update_numa_stats(&env, &env.dst_stats, env.dst_nid, true);
			task_numa_ڈھونڈو_cpu(&env, taskimp, groupimp);
		}
	}

	/*
	 * If the task is part of a workload that spans multiple NUMA nodes,
	 * and is migrating into one of the workload's active nodes, remember
	 * this node as the task's preferred numa node, so the workload can
	 * settle down.
	 * A task that migrated to a second choice node will be better off
	 * trying for a better one later. Do not set the preferred node here.
	 */
	اگر (ng) {
		اگر (env.بہترین_cpu == -1)
			nid = env.src_nid;
		ورنہ
			nid = cpu_مقصد_گرہ(env.بہترین_cpu);

		اگر (nid != p->numa_preferred_nid)
			sched_setnuma(p, nid);
	}

	/* No better CPU than the current one was found. */
	اگر (env.بہترین_cpu == -1) {
		trace_sched_stick_numa(p, env.src_cpu, تہی, -1);
		واپس -EAGAIN;
	}

	بہترین_rq = cpu_rq(env.بہترین_cpu);
	اگر (env.بہترین_task == تہی) {
		ret = migrate_task_مقصد(p, env.بہترین_cpu);
		WRITE_ONCE(بہترین_rq->numa_migrate_on, 0);
		اگر (ret != 0)
			trace_sched_stick_numa(p, env.src_cpu, تہی, env.بہترین_cpu);
		واپس ret;
	}

	ret = migrate_تبادلہ(p, env.بہترین_task, env.بہترین_cpu, env.src_cpu);
	WRITE_ONCE(بہترین_rq->numa_migrate_on, 0);

	اگر (ret != 0)
		trace_sched_stick_numa(p, env.src_cpu, env.بہترین_task, env.بہترین_cpu);
	put_task_struct(env.بہترین_task);
	واپس ret;
}

/* Attempt to migrate a task to a CPU on the preferred node. */
ساکن خالی numa_migrate_preferred(ساخت task_struct *p)
{
	غیرعلامتی طویل interval = HZ;

	/* This task has no NUMA fault statistics yet */
	اگر (unlikely(p->numa_preferred_nid == NUMA_NO_گرہ || !p->numa_خرابیاں))
		واپس;

	/* Periodically retry migrating the task to the preferred node */
	interval = min(interval, msecs_مقصد_jiffies(p->numa_جائزہ_period) / 16);
	p->numa_migrate_retry = jiffies + interval;

	/* Success if task is already running on preferred CPU */
	اگر (task_گرہ(p) == p->numa_preferred_nid)
		واپس;

	/* Otherwise, try migrate to a CPU on the preferred node */
	task_numa_migrate(p);
}

/*
 * Find out how many nodes the workload is actively running on. Do this by
 * tracking the nodes from which NUMA hinting faults are triggered. This can
 * be different from the set of nodes where the workload's memory is currently
 * located.
 */
ساکن خالی numa_گروہ_گنتی_فعال_nodes(ساخت numa_گروہ *numa_گروہ)
{
	غیرعلامتی طویل خرابیاں, max_خرابیاں = 0;
	صحیح nid, فعال_nodes = 0;

	for_each_گرہ_state(nid, N_CPU) {
		خرابیاں = گروہ_خرابیاں_cpu(numa_گروہ, nid);
		اگر (خرابیاں > max_خرابیاں)
			max_خرابیاں = خرابیاں;
	}

	for_each_گرہ_state(nid, N_CPU) {
		خرابیاں = گروہ_خرابیاں_cpu(numa_گروہ, nid);
		اگر (خرابیاں * فعال_گرہ_FRACTION > max_خرابیاں)
			فعال_nodes++;
	}

	numa_گروہ->max_خرابیاں_cpu = max_خرابیاں;
	numa_گروہ->فعال_nodes = فعال_nodes;
}

/*
 * When adapting the scan rate, the period is divided into NUMA_PERIOD_SLOTS
 * increments. The more local the fault statistics are, the higher the scan
 * period will be for the next scan window. If local/(local+remote) ratio is
 * below NUMA_PERIOD_THRESHOLD (where range of ratio is 1..NUMA_PERIOD_SLOTS)
 * the scan period will decrease. Aim for 70% local accesses.
 */
#تعریف_کرو NUMA_PERIOD_SLOTS 10
#تعریف_کرو NUMA_PERIOD_حد 7

/*
 * Increase the scan period (slow down scanning) if the majority of
 * our memory is already on our local node, or if the majority of
 * the page accesses are shared with other processes.
 * Otherwise, decrease the scan period.
 */
ساکن خالی update_task_جائزہ_period(ساخت task_struct *p,
			غیرعلامتی طویل shared, غیرعلامتی طویل نجی)
{
	غیرعلامتی صحیح period_خانہ;
	صحیح lr_تناسب, ps_تناسب;
	صحیح diff;

	غیرعلامتی طویل remote = p->numa_خرابیاں_locality[0];
	غیرعلامتی طویل local = p->numa_خرابیاں_locality[1];

	/*
	 * If there were no record hinting faults then either the task is
	 * completely idle or all activity is in areas that are not of interest
	 * to automatic numa balancing. Related to that, if there were failed
	 * migration then it implies we are migrating too quickly or the local
	 * node is overloaded. In either case, scan slower
	 */
	اگر (local + shared == 0 || p->numa_خرابیاں_locality[2]) {
		p->numa_جائزہ_period = min(p->numa_جائزہ_period_max,
			p->numa_جائزہ_period << 1);

		p->mm->numa_اگلا_جائزہ = jiffies +
			msecs_مقصد_jiffies(p->numa_جائزہ_period);

		جاؤ برآمد;
	}

	/*
	 * Prepare to scale scan period relative to the current period.
	 *	 == NUMA_PERIOD_THRESHOLD scan period stays the same
	 *       <  NUMA_PERIOD_THRESHOLD scan period decreases (scan faster)
	 *	 >= NUMA_PERIOD_THRESHOLD scan period increases (scan slower)
	 */
	period_خانہ = DIV_باری_اوپر(p->numa_جائزہ_period, NUMA_PERIOD_SLOTS);
	lr_تناسب = (local * NUMA_PERIOD_SLOTS) / (local + remote);
	ps_تناسب = (نجی * NUMA_PERIOD_SLOTS) / (نجی + shared);

	اگر (ps_تناسب >= NUMA_PERIOD_حد) {
		/*
		 * Most memory accesses are local. There is no need to
		 * do fast NUMA scanning, since memory is already local.
		 */
		صحیح خانہ = ps_تناسب - NUMA_PERIOD_حد;
		اگر (!خانہ)
			خانہ = 1;
		diff = خانہ * period_خانہ;
	} ورنہ اگر (lr_تناسب >= NUMA_PERIOD_حد) {
		/*
		 * Most memory accesses are shared with other tasks.
		 * There is no point in continuing fast NUMA scanning,
		 * since other tasks may just move the memory elsewhere.
		 */
		صحیح خانہ = lr_تناسب - NUMA_PERIOD_حد;
		اگر (!خانہ)
			خانہ = 1;
		diff = خانہ * period_خانہ;
	} ورنہ {
		/*
		 * Private memory faults exceed (SLOTS-THRESHOLD)/SLOTS,
		 * yet they are not on the local NUMA node. Speed up
		 * NUMA scanning to get the memory moved over.
		 */
		صحیح تناسب = max(lr_تناسب, ps_تناسب);
		diff = -(NUMA_PERIOD_حد - تناسب) * period_خانہ;
	}

	p->numa_جائزہ_period = clamp(p->numa_جائزہ_period + diff,
			task_جائزہ_min(p), task_جائزہ_max(p));

برآمد:
	memset(p->numa_خرابیاں_locality, 0,
	       حجم(p->numa_خرابیاں_locality));
}

/*
 * Get the fraction of time the task has been running since the last
 * NUMA placement cycle. The scheduler keeps similar statistics, but
 * decays those on a 32ms period, which is orders of magnitude off
 * from the dozens-of-seconds NUMA balancing period. Use the scheduler
 * stats only if the task is so new there are no NUMA statistics yet.
 */
ساکن u64 numa_get_avg_runtime(ساخت task_struct *p, u64 *period)
{
	u64 runtime, delta, now;
	/* Use the start of this time slice to avoid calculations. */
	now = p->se.exec_آغاز;
	runtime = p->se.مجموع_exec_runtime;

	اگر (p->آخری_task_numa_placement) {
		delta = runtime - p->آخری_مجموع_exec_runtime;
		*period = now - p->آخری_task_numa_placement;

		/* Avoid time going backwards, prevent potential divide error: */
		اگر (unlikely((s64)*period < 0))
			*period = 0;
	} ورنہ {
		delta = p->se.avg.بھرو_مجموع;
		*period = بھرو_AVG_MAX;
	}

	p->آخری_مجموع_exec_runtime = runtime;
	p->آخری_task_numa_placement = now;

	واپس delta;
}

/*
 * Determine the preferred nid for a task in a numa_group. This needs to
 * be done in a way that produces consistent results with group_weight,
 * otherwise workloads might not converge.
 */
ساکن صحیح preferred_گروہ_nid(ساخت task_struct *p, صحیح nid)
{
	nodemask_t nodes;
	صحیح dist;

	/* Direct connections between all NUMA nodes. */
	اگر (sched_numa_topology_type == NUMA_براہ_راست)
		واپس nid;

	/*
	 * On a system with glueless mesh NUMA topology, group_weight
	 * scores nodes according to the number of NUMA hinting faults on
	 * both the node itself, and on nearby nodes.
	 */
	اگر (sched_numa_topology_type == NUMA_GLUELESS_MESH) {
		غیرعلامتی طویل score, max_score = 0;
		صحیح گرہ, max_گرہ = nid;

		dist = sched_max_numa_فاصلہ;

		for_each_گرہ_state(گرہ, N_CPU) {
			score = گروہ_وزن(p, گرہ, dist);
			اگر (score > max_score) {
				max_score = score;
				max_گرہ = گرہ;
			}
		}
		واپس max_گرہ;
	}

	/*
	 * Finding the preferred nid in a system with NUMA backplane
	 * interconnect topology is more involved. The goal is to locate
	 * tasks from numa_groups near each other in the system, and
	 * untangle workloads from different sides of the system. This requires
	 * searching down the hierarchy of node groups, recursively searching
	 * inside the highest scoring group of nodes. The nodemask tricks
	 * keep the complexity of the search down.
	 */
	nodes = گرہ_حالتیں[N_CPU];
	برائے (dist = sched_max_numa_فاصلہ; dist > LOCAL_فاصلہ; dist--) {
		غیرعلامتی طویل max_خرابیاں = 0;
		nodemask_t max_گروہ = گرہ_نقاب_NONE;
		صحیح a, b;

		/* Are there nodes at this distance from each other? */
		اگر (!ڈھونڈو_numa_فاصلہ(dist))
			جاری;

		for_each_گرہ_نقاب(a, nodes) {
			غیرعلامتی طویل خرابیاں = 0;
			nodemask_t this_گروہ;
			nodes_clear(this_گروہ);

			/* Sum group's NUMA faults; includes a==b case. */
			for_each_گرہ_نقاب(b, nodes) {
				اگر (گرہ_فاصلہ(a, b) < dist) {
					خرابیاں += گروہ_خرابیاں(p, b);
					گرہ_سیٹ(b, this_گروہ);
					گرہ_clear(b, nodes);
				}
			}

			/* Remember the top group. */
			اگر (خرابیاں > max_خرابیاں) {
				max_خرابیاں = خرابیاں;
				max_گروہ = this_گروہ;
				/*
				 * subtle: at the smallest distance there is
				 * just one node left in each "group", the
				 * winner is the preferred nid.
				 */
				nid = a;
			}
		}
		/* Next round, evaluate the nodes within max_group. */
		اگر (!max_خرابیاں)
			توڑو;
		nodes = max_گروہ;
	}
	واپس nid;
}

ساکن خالی task_numa_placement(ساخت task_struct *p)
	__context_unsafe(/* conditional locking */)
{
	ساخت sched_cache_گروہ __maybe_unused *grp;
	صحیح seq, nid, max_nid = NUMA_NO_گرہ;
	غیرعلامتی طویل max_خرابیاں = 0;
	غیرعلامتی طویل fault_types[2] = { 0, 0 };
	غیرعلامتی طویل کل_خرابیاں;
	u64 runtime, period;
	spinlock_t *گروہ_تالا = تہی;
	طویل __maybe_unused new_fp;
	ساخت numa_گروہ *ng;

	/*
	 * The p->mm->numa_scan_seq field gets updated without
	 * exclusive access. Use READ_ONCE() here to ensure
	 * that the field is read in a single access:
	 */
	seq = READ_ONCE(p->mm->numa_جائزہ_seq);
	اگر (p->numa_جائزہ_seq == seq)
		واپس;
	p->numa_جائزہ_seq = seq;
	p->numa_جائزہ_period_max = task_جائزہ_max(p);

	کل_خرابیاں = p->numa_خرابیاں_locality[0] +
		       p->numa_خرابیاں_locality[1];
	runtime = numa_get_avg_runtime(p, &period);

	/* If the task is part of a group prevent parallel updates to group stats */
	ng = deref_curr_numa_گروہ(p);
	اگر (ng) {
		گروہ_تالا = &ng->تالا;
		spin_تالا_irq(گروہ_تالا);
	}

	/* Find the node with the highest number of faults */
	for_each_online_گرہ(nid) {
		/* Keep track of the offsets in numa_faults array */
		صحیح mem_idx, membuf_idx, cpu_idx, cpubuf_idx;
		غیرعلامتی طویل خرابیاں = 0, گروہ_خرابیاں = 0;
		صحیح priv;

		برائے (priv = 0; priv < NR_NUMA_HINT_FAULT_TYPES; priv++) {
			طویل diff, f_diff, f_وزن;

			mem_idx = task_خرابیاں_idx(NUMA_MEM, nid, priv);
			membuf_idx = task_خرابیاں_idx(NUMA_MEMBUF, nid, priv);
			cpu_idx = task_خرابیاں_idx(NUMA_CPU, nid, priv);
			cpubuf_idx = task_خرابیاں_idx(NUMA_CPUBUF, nid, priv);

			/* Decay existing window, copy faults since last scan */
			diff = p->numa_خرابیاں[membuf_idx] - p->numa_خرابیاں[mem_idx] / 2;
			fault_types[priv] += p->numa_خرابیاں[membuf_idx];
			p->numa_خرابیاں[membuf_idx] = 0;

			/*
			 * Normalize the faults_from, so all tasks in a group
			 * count according to CPU use, instead of by the raw
			 * number of faults. Tasks with little runtime have
			 * little over-all impact on throughput, and thus their
			 * faults are less important.
			 */
			f_وزن = div64_u64(runtime << 16, period + 1);
			f_وزن = (f_وزن * p->numa_خرابیاں[cpubuf_idx]) /
				   (کل_خرابیاں + 1);
			f_diff = f_وزن - p->numa_خرابیاں[cpu_idx] / 2;
			p->numa_خرابیاں[cpubuf_idx] = 0;

			p->numa_خرابیاں[mem_idx] += diff;
			p->numa_خرابیاں[cpu_idx] += f_diff;
			خرابیاں += p->numa_خرابیاں[mem_idx];
			p->کل_numa_خرابیاں += diff;
			اگر (ng) {
				/*
				 * safe because we can only change our own group
				 *
				 * mem_idx represents the offset for a given
				 * nid and priv in a specific region because it
				 * is at the beginning of the numa_faults array.
				 */
				ng->خرابیاں[mem_idx] += diff;
				ng->خرابیاں[cpu_idx] += f_diff;
				ng->کل_خرابیاں += diff;
				گروہ_خرابیاں += ng->خرابیاں[mem_idx];
			}
#اگر_تعریف_شدہ CONFIG_SCHED_CACHE
			/*
			 * Per task p->numa_faults[mem_idx] converges,
			 * so the accumulation of each task's faults
			 * converges too - Given the number of threads,
			 * it cannot overflow an unsigned long.
			 * Racy with concurrent updates from other threads
			 * sharing this mm. Acceptable since footprint is a
			 * heuristic and occasional lost updates are tolerable.
			 *
			 * If a task exits, its corresponding footprint must
			 * be subtracted from p->sched_cache_grp->footprint,
			 * otherwise the footprint will not converge: the
			 * exiting thread's footprint remains unchanged/undecayed.
			 * See exit_mm().
			 *
			 * Lost updates and unsynchronized subtraction
			 * in exit_mm() can cause footprint + diff to
			 * go negative. Clamp to zero to prevent the
			 * unsigned footprint from wrapping.
			 */
			scoped_guard(rcu) {
				grp = rcu_dereference(p->sched_cache_grp);

				اگر (grp) {
					new_fp = (طویل)READ_ONCE(grp->footprint) + diff;
					WRITE_ONCE(grp->footprint, max(new_fp, 0L));
				}
			}
#اگر_ختم
		}

		اگر (!ng) {
			اگر (خرابیاں > max_خرابیاں) {
				max_خرابیاں = خرابیاں;
				max_nid = nid;
			}
		} ورنہ اگر (گروہ_خرابیاں > max_خرابیاں) {
			max_خرابیاں = گروہ_خرابیاں;
			max_nid = nid;
		}
	}

	/* Cannot migrate task to CPU-less node */
	max_nid = numa_قریب_ترین_گرہ(max_nid, N_CPU);

	اگر (ng) {
		numa_گروہ_گنتی_فعال_nodes(ng);
		spin_unlock_irq(گروہ_تالا);
		max_nid = preferred_گروہ_nid(p, max_nid);
	}

	اگر (max_خرابیاں) {
		/* Set the new preferred node */
		اگر (max_nid != p->numa_preferred_nid)
			sched_setnuma(p, max_nid);
	}

	update_task_جائزہ_period(p, fault_types[0], fault_types[1]);
}

ساکن اندرونی صحیح get_numa_گروہ(ساخت numa_گروہ *grp)
{
	واپس refcount_inc_not_zero(&grp->refcount);
}

ساکن اندرونی خالی put_numa_گروہ(ساخت numa_گروہ *grp)
{
	اگر (refcount_dec_and_test(&grp->refcount))
		kfree_rcu(grp, rcu);
}

ساکن خالی task_numa_گروہ(ساخت task_struct *p, صحیح cpupid, صحیح flags,
			صحیح *priv)
{
	ساخت numa_گروہ *grp, *my_grp;
	ساخت task_struct *tsk;
	bool join = false;
	صحیح cpu = cpupid_مقصد_cpu(cpupid);
	صحیح i;

	اگر (unlikely(!deref_curr_numa_گروہ(p))) {
		غیرعلامتی صحیح سائز = حجم(ساخت numa_گروہ) +
				    NR_NUMA_HINT_FAULT_STATS *
				    nr_گرہ_ids * حجم(غیرعلامتی طویل);

		grp = kzalloc(سائز, GFP_KERNEL | __GFP_NOWARN);
		اگر (!grp)
			واپس;

		refcount_سیٹ(&grp->refcount, 1);
		grp->فعال_nodes = 1;
		grp->max_خرابیاں_cpu = 0;
		spin_تالا_init(&grp->تالا);
		grp->gid = p->pid;

		برائے (i = 0; i < NR_NUMA_HINT_FAULT_STATS * nr_گرہ_ids; i++)
			grp->خرابیاں[i] = p->numa_خرابیاں[i];

		grp->کل_خرابیاں = p->کل_numa_خرابیاں;

		grp->nr_tasks++;
		rcu_تفویض_اشارہ_گر(p->numa_گروہ, grp);
	}

	rcu_read_تالا();
	tsk = READ_ONCE(cpu_rq(cpu)->curr);

	اگر (!cpupid_مطابقت_pid(tsk, cpupid))
		جاؤ no_join;

	grp = rcu_dereference_all(tsk->numa_گروہ);
	اگر (!grp)
		جاؤ no_join;

	my_grp = deref_curr_numa_گروہ(p);
	اگر (grp == my_grp)
		جاؤ no_join;

	/*
	 * Only join the other group if its bigger; if we're the bigger group,
	 * the other task will join us.
	 */
	اگر (my_grp->nr_tasks > grp->nr_tasks)
		جاؤ no_join;

	/*
	 * Tie-break on the grp address.
	 */
	اگر (my_grp->nr_tasks == grp->nr_tasks && my_grp > grp)
		جاؤ no_join;

	/* Always join threads in the same process. */
	اگر (tsk->mm == current->mm)
		join = true;

	/* Simple filter to avoid false positives due to PID collisions */
	اگر (flags & TNF_SHARED)
		join = true;

	/* Update priv based on whether false sharing was detected */
	*priv = !join;

	اگر (join && !get_numa_گروہ(grp))
		جاؤ no_join;

	rcu_read_unlock();

	اگر (!join)
		واپس;

	WARN_ON_ONCE(irqs_disabled());
	double_تالا_irq(&my_grp->تالا, &grp->تالا);

	برائے (i = 0; i < NR_NUMA_HINT_FAULT_STATS * nr_گرہ_ids; i++) {
		my_grp->خرابیاں[i] -= p->numa_خرابیاں[i];
		grp->خرابیاں[i] += p->numa_خرابیاں[i];
	}
	my_grp->کل_خرابیاں -= p->کل_numa_خرابیاں;
	grp->کل_خرابیاں += p->کل_numa_خرابیاں;

	my_grp->nr_tasks--;
	grp->nr_tasks++;

	spin_unlock(&my_grp->تالا);
	spin_unlock_irq(&grp->تالا);

	rcu_تفویض_اشارہ_گر(p->numa_گروہ, grp);

	put_numa_گروہ(my_grp);
	واپس;

no_join:
	rcu_read_unlock();
	واپس;
}

/*
 * Get rid of NUMA statistics associated with a task (either current or dead).
 * If @final is set, the task is dead and has reached refcount zero, so we can
 * safely free all relevant data structures. Otherwise, there might be
 * concurrent reads from places like load balancing and procfs, and we should
 * reset the data back to default state without freeing ->numa_faults.
 */
خالی task_numa_free(ساخت task_struct *p, bool final)
{
	/* safe: p either is current or is being freed by current */
	ساخت numa_گروہ *grp = rcu_dereference_raw(p->numa_گروہ);
	غیرعلامتی طویل *numa_خرابیاں = p->numa_خرابیاں;
	غیرعلامتی طویل flags;
	صحیح i;

	اگر (!numa_خرابیاں)
		واپس;

	اگر (grp) {
		spin_تالا_irqsave(&grp->تالا, flags);
		برائے (i = 0; i < NR_NUMA_HINT_FAULT_STATS * nr_گرہ_ids; i++)
			grp->خرابیاں[i] -= p->numa_خرابیاں[i];
		grp->کل_خرابیاں -= p->کل_numa_خرابیاں;

		grp->nr_tasks--;
		spin_unlock_irqrestore(&grp->تالا, flags);
		RCU_INIT_اشارہ_گر(p->numa_گروہ, تہی);
		put_numa_گروہ(grp);
	}

	اگر (final) {
		p->numa_خرابیاں = تہی;
		kfree(numa_خرابیاں);
	} ورنہ {
		p->کل_numa_خرابیاں = 0;
		برائے (i = 0; i < NR_NUMA_HINT_FAULT_STATS * nr_گرہ_ids; i++)
			numa_خرابیاں[i] = 0;
	}
}

/*
 * Got a PROT_NONE fault for a page on @node.
 */
خالی task_numa_fault(صحیح آخری_cpupid, صحیح mem_گرہ, صحیح صفحات, صحیح flags)
{
	ساخت task_struct *p = current;
	bool migrated = flags & TNF_MIGRATED;
	صحیح cpu_گرہ = task_گرہ(current);
	صحیح local = !!(flags & TNF_FAULT_LOCAL);
	ساخت numa_گروہ *ng;
	صحیح priv;

	اگر (!static_branch_likely(&sched_numa_balancing))
		واپس;

	/* for example, ksmd faulting in a user's mm */
	اگر (!p->mm)
		واپس;

	/*
	 * NUMA faults statistics are unnecessary for the slow memory
	 * node for memory tiering mode.
	 */
	اگر (!گرہ_is_toptier(mem_گرہ) &&
	    (sysctl_numa_balancing_منوال & NUMA_BALANCING_حافظہ_TIERING ||
	     !cpupid_valid(آخری_cpupid)))
		واپس;

	/* Allocate buffer to track faults on a per-node basis */
	اگر (unlikely(!p->numa_خرابیاں)) {
		صحیح سائز = حجم(*p->numa_خرابیاں) *
			   NR_NUMA_HINT_FAULT_BUCKETS * nr_گرہ_ids;

		p->numa_خرابیاں = kzalloc(سائز, GFP_KERNEL|__GFP_NOWARN);
		اگر (!p->numa_خرابیاں)
			واپس;

		p->کل_numa_خرابیاں = 0;
		memset(p->numa_خرابیاں_locality, 0, حجم(p->numa_خرابیاں_locality));
	}

	/*
	 * First accesses are treated as private, otherwise consider accesses
	 * to be private if the accessing pid has not changed
	 */
	اگر (unlikely(آخری_cpupid == (-1 & آخری_CPUPID_نقاب))) {
		priv = 1;
	} ورنہ {
		priv = cpupid_مطابقت_pid(p, آخری_cpupid);
		اگر (!priv && !(flags & TNF_NO_گروہ))
			task_numa_گروہ(p, آخری_cpupid, flags, &priv);
	}

	/*
	 * If a workload spans multiple NUMA nodes, a shared fault that
	 * occurs wholly within the set of nodes that the workload is
	 * actively using should be counted as local. This allows the
	 * scan rate to slow down when a workload has settled down.
	 */
	ng = deref_curr_numa_گروہ(p);
	اگر (!priv && !local && ng && ng->فعال_nodes > 1 &&
				numa_is_فعال_گرہ(cpu_گرہ, ng) &&
				numa_is_فعال_گرہ(mem_گرہ, ng))
		local = 1;

	/*
	 * Retry to migrate task to preferred node periodically, in case it
	 * previously failed, or the scheduler moved us.
	 */
	اگر (وقت_after(jiffies, p->numa_migrate_retry)) {
		task_numa_placement(p);
		numa_migrate_preferred(p);
	}

	اگر (migrated)
		p->numa_صفحات_migrated += صفحات;
	اگر (flags & TNF_MIGRATE_FAIL)
		p->numa_خرابیاں_locality[2] += صفحات;

	p->numa_خرابیاں[task_خرابیاں_idx(NUMA_MEMBUF, mem_گرہ, priv)] += صفحات;
	p->numa_خرابیاں[task_خرابیاں_idx(NUMA_CPUBUF, cpu_گرہ, priv)] += صفحات;
	p->numa_خرابیاں_locality[local] += صفحات;
}

ساکن خالی reset_ptenuma_جائزہ(ساخت task_struct *p)
{
	/*
	 * We only did a read acquisition of the mmap sem, so
	 * p->mm->numa_scan_seq is written to without exclusive access
	 * and the update is not guaranteed to be atomic. That's not
	 * much of an issue though, since this is just used for
	 * statistical sampling. Use READ_ONCE/WRITE_ONCE, which are not
	 * expensive, to avoid any form of compiler optimizations:
	 */
	WRITE_ONCE(p->mm->numa_جائزہ_seq, READ_ONCE(p->mm->numa_جائزہ_seq) + 1);
	p->mm->numa_جائزہ_انحراف = 0;
}

ساکن bool vma_is_accessed(ساخت mm_struct *mm, ساخت vm_area_struct *vma)
{
	غیرعلامتی طویل pids;
	/*
	 * Allow unconditional access first two times, so that all the (pages)
	 * of VMAs get prot_none fault introduced irrespective of accesses.
	 * This is also done to avoid any side effect of task scanning
	 * amplifying the unfairness of disjoint set of VMAs' access.
	 */
	اگر ((READ_ONCE(current->mm->numa_جائزہ_seq) - vma->numab_state->آغاز_جائزہ_seq) < 2)
		واپس true;

	pids = vma->numab_state->pids_فعال[0] | vma->numab_state->pids_فعال[1];
	اگر (test_ثنائی_ہندسہ(hash_32(current->pid, ilog2(بٹ_فی_LONG)), &pids))
		واپس true;

	/*
	 * Complete a scan that has already started regardless of PID access, or
	 * some VMAs may never be scanned in multi-threaded applications:
	 */
	اگر (mm->numa_جائزہ_انحراف > vma->vm_آغاز) {
		trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_IGNORE_PID);
		واپس true;
	}

	/*
	 * This vma has not been accessed for a while, and if the number
	 * the threads in the same process is low, which means no other
	 * threads can help scan this vma, force a vma scan.
	 */
	اگر (READ_ONCE(mm->numa_جائزہ_seq) >
	   (vma->numab_state->پچھلا_جائزہ_seq + get_nr_دھاگے(current)))
		واپس true;

	واپس false;
}

#تعریف_کرو VMA_PID_RESET_PERIOD (4 * sysctl_numa_balancing_جائزہ_delay)

/*
 * The expensive part of numa migration is done from task_work context.
 * Triggered from task_tick_numa().
 */
ساکن خالی task_numa_work(ساخت callback_سر *work)
{
	غیرعلامتی طویل migrate, اگلا_جائزہ, now = jiffies;
	ساخت task_struct *p = current;
	ساخت mm_struct *mm = p->mm;
	u64 runtime = p->se.مجموع_exec_runtime;
	ساخت vm_area_struct *vma;
	غیرعلامتی طویل آغاز, انتہا;
	غیرعلامتی طویل nr_pte_updates = 0;
	طویل صفحات, virtpages;
	ساخت vma_iterator vmi;
	bool vma_pids_skipped;
	bool vma_pids_forced = false;

	WARN_ON_ONCE(p != container_کا(work, ساخت task_struct, numa_work));

	work->اگلا = work;
	/*
	 * Who cares about NUMA placement when they're dying.
	 *
	 * NOTE: make sure not to dereference p->mm before this check,
	 * exit_task_work() happens _after_ exit_mm() so we could be called
	 * without p->mm even though we still had it when we enqueued this
	 * work.
	 */
	اگر (p->flags & PF_EXITING)
		واپس;

	/*
	 * Memory is pinned to only one NUMA node via cpuset.mems, naturally
	 * no page can be migrated.
	 */
	اگر (cpusets_enabled() && nodes_وزن(cpuset_current_mems_allowed) == 1) {
		trace_sched_skip_cpuset_numa(current, &cpuset_current_mems_allowed);
		واپس;
	}

	اگر (!mm->numa_اگلا_جائزہ) {
		mm->numa_اگلا_جائزہ = now +
			msecs_مقصد_jiffies(sysctl_numa_balancing_جائزہ_delay);
	}

	/*
	 * Enforce maximal scan/migration frequency..
	 */
	migrate = mm->numa_اگلا_جائزہ;
	اگر (وقت_before(now, migrate))
		واپس;

	اگر (p->numa_جائزہ_period == 0) {
		p->numa_جائزہ_period_max = task_جائزہ_max(p);
		p->numa_جائزہ_period = task_جائزہ_آغاز(p);
	}

	اگلا_جائزہ = now + msecs_مقصد_jiffies(p->numa_جائزہ_period);
	اگر (!try_cmpxchg(&mm->numa_اگلا_جائزہ, &migrate, اگلا_جائزہ))
		واپس;

	/*
	 * Delay this task enough that another task of this mm will likely win
	 * the next time around.
	 */
	p->گرہ_stamp += 2 * TICK_NSEC;

	صفحات = sysctl_numa_balancing_جائزہ_سائز;
	صفحات <<= 20 - صفحہ_سرکاؤ; /* MB in pages */
	virtpages = صفحات * 8;	   /* Scan up to this much virtual space */
	اگر (!صفحات)
		واپس;


	اگر (!mmap_read_trylock(mm))
		واپس;

	/*
	 * VMAs are skipped if the current PID has not trapped a fault within
	 * the VMA recently. Allow scanning to be forced if there is no
	 * suitable VMA remaining.
	 */
	vma_pids_skipped = false;

retry_pids:
	آغاز = mm->numa_جائزہ_انحراف;
	vma_iter_init(&vmi, mm, آغاز);
	vma = vma_اگلا(&vmi);
	اگر (!vma) {
		reset_ptenuma_جائزہ(p);
		آغاز = 0;
		vma_iter_سیٹ(&vmi, آغاز);
		vma = vma_اگلا(&vmi);
	}

	برائے (; vma; vma = vma_اگلا(&vmi)) {
		اگر (!vma_migratable(vma) || !vma_policy_mof(vma) ||
			is_vm_hugetlb_صفحہ(vma) || (vma->vm_flags & VM_MIXEDMAP)) {
			trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_UNSUITABLE);
			جاری;
		}

		/*
		 * Shared library pages mapped by multiple processes are not
		 * migrated as it is expected they are cache replicated. Avoid
		 * hinting faults in read-only file-backed mappings or the vDSO
		 * as migrating the pages will be of marginal benefit.
		 */
		اگر (!vma->vm_mm ||
		    (vma->vm_file && (vma->vm_flags & (VM_READ|VM_WRITE)) == (VM_READ))) {
			trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_SHARED_RO);
			جاری;
		}

		/*
		 * Skip inaccessible VMAs to avoid any confusion between
		 * PROT_NONE and NUMA hinting PTEs
		 */
		اگر (!vma_is_accessible(vma)) {
			trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_INACCESSIBLE);
			جاری;
		}

		/* Initialise new per-VMA NUMAB state. */
		اگر (!vma->numab_state) {
			ساخت vma_numab_state *ptr;

			ptr = kzalloc_obj(*ptr);
			اگر (!ptr)
				جاری;

			اگر (cmpxchg(&vma->numab_state, تہی, ptr)) {
				kfree(ptr);
				جاری;
			}

			vma->numab_state->آغاز_جائزہ_seq = mm->numa_جائزہ_seq;

			vma->numab_state->اگلا_جائزہ = now +
				msecs_مقصد_jiffies(sysctl_numa_balancing_جائزہ_delay);

			/* Reset happens after 4 times scan delay of scan start */
			vma->numab_state->pids_فعال_reset =  vma->numab_state->اگلا_جائزہ +
				msecs_مقصد_jiffies(VMA_PID_RESET_PERIOD);

			/*
			 * Ensure prev_scan_seq does not match numa_scan_seq,
			 * to prevent VMAs being skipped prematurely on the
			 * first scan:
			 */
			 vma->numab_state->پچھلا_جائزہ_seq = mm->numa_جائزہ_seq - 1;
		}

		/*
		 * Scanning the VMAs of short lived tasks add more overhead. So
		 * delay the scan for new VMAs.
		 */
		اگر (mm->numa_جائزہ_seq && وقت_before(jiffies,
						vma->numab_state->اگلا_جائزہ)) {
			trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_جائزہ_DELAY);
			جاری;
		}

		/* RESET access PIDs regularly for old VMAs. */
		اگر (mm->numa_جائزہ_seq &&
				وقت_after(jiffies, vma->numab_state->pids_فعال_reset)) {
			vma->numab_state->pids_فعال_reset = vma->numab_state->pids_فعال_reset +
				msecs_مقصد_jiffies(VMA_PID_RESET_PERIOD);
			vma->numab_state->pids_فعال[0] = READ_ONCE(vma->numab_state->pids_فعال[1]);
			vma->numab_state->pids_فعال[1] = 0;
		}

		/* Do not rescan VMAs twice within the same sequence. */
		اگر (vma->numab_state->پچھلا_جائزہ_seq == mm->numa_جائزہ_seq) {
			mm->numa_جائزہ_انحراف = vma->vm_انتہا;
			trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_SEQ_COMPLETED);
			جاری;
		}

		/*
		 * Do not scan the VMA if task has not accessed it, unless no other
		 * VMA candidate exists.
		 */
		اگر (!vma_pids_forced && !vma_is_accessed(mm, vma)) {
			vma_pids_skipped = true;
			trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_PID_INACTIVE);
			جاری;
		}

		کرو {
			آغاز = max(آغاز, vma->vm_آغاز);
			انتہا = ALIGN(آغاز + (صفحات << صفحہ_سرکاؤ), HPAGE_سائز);
			انتہا = min(انتہا, vma->vm_انتہا);
			nr_pte_updates = change_prot_numa(vma, آغاز, انتہا);

			/*
			 * Try to scan sysctl_numa_balancing_size worth of
			 * hpages that have at least one present PTE that
			 * is not already PTE-numa. If the VMA contains
			 * areas that are unused or already full of prot_numa
			 * PTEs, scan up to virtpages, to skip through those
			 * areas faster.
			 */
			اگر (nr_pte_updates)
				صفحات -= (انتہا - آغاز) >> صفحہ_سرکاؤ;
			virtpages -= (انتہا - آغاز) >> صفحہ_سرکاؤ;

			آغاز = انتہا;
			اگر (صفحات <= 0 || virtpages <= 0)
				جاؤ برآمد;

			cond_resched();
		} جب (انتہا != vma->vm_انتہا);

		/* VMA scan is complete, do not scan until next sequence. */
		vma->numab_state->پچھلا_جائزہ_seq = mm->numa_جائزہ_seq;

		/*
		 * Only force scan within one VMA at a time, to limit the
		 * cost of scanning a potentially uninteresting VMA.
		 */
		اگر (vma_pids_forced)
			توڑو;
	}

	/*
	 * If no VMAs are remaining and VMAs were skipped due to the PID
	 * not accessing the VMA previously, then force a scan to ensure
	 * forward progress:
	 */
	اگر (!vma && !vma_pids_forced && vma_pids_skipped) {
		vma_pids_forced = true;
		جاؤ retry_pids;
	}

برآمد:
	/*
	 * It is possible to reach the end of the VMA list but the last few
	 * VMAs are not guaranteed to the vma_migratable. If they are not, we
	 * would find the !migratable VMA on the next scan but not reset the
	 * scanner to the start so check it now.
	 */
	اگر (vma)
		mm->numa_جائزہ_انحراف = آغاز;
	ورنہ
		reset_ptenuma_جائزہ(p);
	mmap_read_unlock(mm);

	/*
	 * Make sure tasks use at least 32x as much time to run other code
	 * than they used here, to limit NUMA PTE scanning overhead to 3% max.
	 * Usually update_task_scan_period slows down scanning enough; on an
	 * overloaded system we need to limit overhead on a per task basis.
	 */
	اگر (unlikely(p->se.مجموع_exec_runtime != runtime)) {
		u64 diff = p->se.مجموع_exec_runtime - runtime;
		p->گرہ_stamp += 32 * diff;
	}
}

خالی init_numa_balancing(u64 clone_flags, ساخت task_struct *p)
{
	صحیح mm_users = 0;
	ساخت mm_struct *mm = p->mm;

	اگر (mm) {
		mm_users = atomic_read(&mm->mm_users);
		اگر (mm_users == 1) {
			mm->numa_اگلا_جائزہ = jiffies + msecs_مقصد_jiffies(sysctl_numa_balancing_جائزہ_delay);
			mm->numa_جائزہ_seq = 0;
		}
	}
	p->گرہ_stamp			= 0;
	p->numa_جائزہ_seq		= mm ? mm->numa_جائزہ_seq : 0;
	p->numa_جائزہ_period		= sysctl_numa_balancing_جائزہ_delay;
	p->numa_migrate_retry		= 0;
	/* Protect against double add, see task_tick_numa and task_numa_work */
	p->numa_work.اگلا		= &p->numa_work;
	p->numa_خرابیاں			= تہی;
	p->numa_صفحات_migrated		= 0;
	p->کل_numa_خرابیاں		= 0;
	RCU_INIT_اشارہ_گر(p->numa_گروہ, تہی);
	p->آخری_task_numa_placement	= 0;
	p->آخری_مجموع_exec_runtime	= 0;

	init_task_work(&p->numa_work, task_numa_work);

	/* New address space, reset the preferred nid */
	اگر (!(clone_flags & CLONE_VM)) {
		p->numa_preferred_nid = NUMA_NO_گرہ;
		واپس;
	}

	/*
	 * New thread, keep existing numa_preferred_nid which should be copied
	 * already by arch_dup_task_struct but stagger when scans start.
	 */
	اگر (mm) {
		غیرعلامتی صحیح delay;

		delay = min_t(غیرعلامتی صحیح, task_جائزہ_max(current),
			current->numa_جائزہ_period * mm_users * NSEC_فی_MSEC);
		delay += 2 * TICK_NSEC;
		p->گرہ_stamp = delay;
	}
}

/*
 * Drive the periodic memory faults..
 */
ساکن خالی task_tick_numa(ساخت rq *rq, ساخت task_struct *curr)
{
	ساخت callback_سر *work = &curr->numa_work;
	u64 period, now;

	/*
	 * We don't care about NUMA placement if we don't have memory.
	 */
	اگر (!curr->mm || (curr->flags & (PF_EXITING | PF_KTHREAD)) || work->اگلا != work)
		واپس;

	/*
	 * Using runtime rather than walltime has the dual advantage that
	 * we (mostly) drive the selection from busy threads and that the
	 * task needs to have done some actual work before we bother with
	 * NUMA placement.
	 */
	now = curr->se.مجموع_exec_runtime;
	period = (u64)curr->numa_جائزہ_period * NSEC_فی_MSEC;

	اگر (now > curr->گرہ_stamp + period) {
		اگر (!curr->گرہ_stamp)
			curr->numa_جائزہ_period = task_جائزہ_آغاز(curr);
		curr->گرہ_stamp += period;

		اگر (!وقت_before(jiffies, curr->mm->numa_اگلا_جائزہ))
			task_work_جوڑو(curr, work, TWA_RESUME);
	}
}

ساکن خالی update_جائزہ_period(ساخت task_struct *p, صحیح new_cpu)
{
	صحیح src_nid = cpu_مقصد_گرہ(task_cpu(p));
	صحیح dst_nid = cpu_مقصد_گرہ(new_cpu);

	اگر (!static_branch_likely(&sched_numa_balancing))
		واپس;

	اگر (!p->mm || !p->numa_خرابیاں || (p->flags & PF_EXITING))
		واپس;

	اگر (src_nid == dst_nid)
		واپس;

	/*
	 * Allow resets if faults have been trapped before one scan
	 * has completed. This is most likely due to a new task that
	 * is pulled cross-node due to wakeups or load balancing.
	 */
	اگر (p->numa_جائزہ_seq) {
		/*
		 * Avoid scan adjustments if moving to the preferred
		 * node or if the task was not previously running on
		 * the preferred node.
		 */
		اگر (dst_nid == p->numa_preferred_nid ||
		    (p->numa_preferred_nid != NUMA_NO_گرہ &&
			src_nid != p->numa_preferred_nid))
			واپس;
	}

	p->numa_جائزہ_period = task_جائزہ_آغاز(p);
}

#ورنہ /* !CONFIG_NUMA_BALANCING: */

ساکن خالی task_tick_numa(ساخت rq *rq, ساخت task_struct *curr)
{
}

ساکن اندرونی خالی account_numa_قطار_میں_ڈالو(ساخت rq *rq, ساخت task_struct *p)
{
}

ساکن اندرونی خالی account_numa_قطار_سے_نکالو(ساخت rq *rq, ساخت task_struct *p)
{
}

ساکن اندرونی خالی update_جائزہ_period(ساخت task_struct *p, صحیح new_cpu)
{
}

#اگر_ختم /* !CONFIG_NUMA_BALANCING */

ساکن خالی
account_entity_قطار_میں_ڈالو(ساخت cfs_rq *cfs_rq, ساخت sched_entity *se)
{
	WARN_ON_ONCE(cfs_rq != cfs_rq_کا(se));
	update_بھرو_جوڑو(&cfs_rq->بھرو, se->بھرو.وزن);
	اگر (entity_is_task(se)) {
		ساخت task_struct *p = task_کا(se);
		ساخت rq *rq = rq_کا(cfs_rq);

		account_numa_قطار_میں_ڈالو(rq, p);
		account_llc_قطار_میں_ڈالو(rq, p);
		list_جوڑو(&se->گروہ_گرہ, &rq->cfs_tasks);
	}
	cfs_rq->nr_queued++;
}

ساکن خالی
account_entity_قطار_سے_نکالو(ساخت cfs_rq *cfs_rq, ساخت sched_entity *se)
{
	WARN_ON_ONCE(cfs_rq != cfs_rq_کا(se));
	update_بھرو_sub(&cfs_rq->بھرو, se->بھرو.وزن);
	اگر (entity_is_task(se)) {
		ساخت task_struct *p = task_کا(se);
		ساخت rq *rq = rq_کا(cfs_rq);

		account_numa_قطار_سے_نکالو(rq, p);
		account_llc_قطار_سے_نکالو(rq, p);
		list_del_init(&se->گروہ_گرہ);
	}
	cfs_rq->nr_queued--;
}

/*
 * Signed add and clamp on underflow.
 *
 * Explicitly do a load-store to ensure the intermediate value never hits
 * memory. This allows lockless observations without ever seeing the negative
 * values.
 */
#تعریف_کرو جوڑو_مثبت(_ptr, _val) کرو {                           \
	typeof(_ptr) ptr = (_ptr);                              \
	__signed_scalar_typeof(*ptr) val = (_val);              \
	typeof(*ptr) res, var = READ_ONCE(*ptr);                \
								\
	res = var + val;                                        \
								\
	اگر (val < 0 && res > var)                               \
		res = 0;                                        \
								\
	WRITE_ONCE(*ptr, res);                                  \
} جب (0)

/*
 * Remove and clamp on negative, from a local variable.
 *
 * A variant of sub_positive(), which does not use explicit load-store
 * and is thus optimized for local variable updates.
 */
#تعریف_کرو lsub_مثبت(_ptr, _val) کرو {				\
	typeof(_ptr) ptr = (_ptr);				\
	*ptr -= min_t(typeof(*ptr), *ptr, _val);		\
} جب (0)


/*
 * Because of rounding, se->util_sum might ends up being +1 more than
 * cfs->util_sum. Although this is not a problem by itself, detaching
 * a lot of tasks with the rounding problem between 2 updates of
 * util_avg (~1ms) can make cfs->util_sum becoming null whereas
 * cfs_util_avg is not.
 *
 * Check that util_sum is still above its lower bound for the new
 * util_avg. Given that period_contrib might have moved since the last
 * sync, we are only sure that util_sum must be above or equal to
 *    util_avg * minimum possible divider
 */
#تعریف_کرو __update_sa(sa, name, delta_avg, delta_مجموع) کرو {	\
	جوڑو_مثبت(&(sa)->name##_avg, delta_avg);		\
	جوڑو_مثبت(&(sa)->name##_sum, delta_مجموع);		\
	(sa)->name##_sum = max_t(typeof((sa)->name##_sum),	\
			       (sa)->name##_sum,		\
			       (sa)->name##_avg * PELT_MIN_DIVIDER); \
} جب (0)

ساکن اندرونی خالی
قطار_میں_ڈالو_بھرو_avg(ساخت cfs_rq *cfs_rq, ساخت sched_entity *se)
{
	__update_sa(&cfs_rq->avg, بھرو, se->avg.بھرو_avg,
		    se_وزن(se) * se->avg.بھرو_مجموع);
}

ساکن اندرونی خالی
قطار_سے_نکالو_بھرو_avg(ساخت cfs_rq *cfs_rq, ساخت sched_entity *se)
{
	__update_sa(&cfs_rq->avg, بھرو, -se->avg.بھرو_avg,
		    se_وزن(se) * -se->avg.بھرو_مجموع);
}

ساکن خالی
rescale_entity(ساخت sched_entity *se, غیرعلامتی طویل وزن, bool rel_vprot)
{
	طویل old_وزن = se->h_بھرو.وزن;

	/*
	 * VRUNTIME
	 * --------
	 *
	 * COROLLARY #1: The virtual runtime of the entity needs to be
	 * adjusted if re-weight at !0-lag point.
	 *
	 * Proof: For contradiction assume this is not true, so we can
	 * re-weight without changing vruntime at !0-lag point.
	 *
	 *             Weight	VRuntime   Avg-VRuntime
	 *     before    w          v            V
	 *      after    w'         v'           V'
	 *
	 * Since lag needs to be preserved through re-weight:
	 *
	 *	lag = (V - v)*w = (V'- v')*w', where v = v'
	 *	==>	V' = (V - v)*w/w' + v		(1)
	 *
	 * Let W be the total weight of the entities before reweight,
	 * since V' is the new weighted average of entities:
	 *
	 *	V' = (WV + w'v - wv) / (W + w' - w)	(2)
	 *
	 * by using (1) & (2) we obtain:
	 *
	 *	(WV + w'v - wv) / (W + w' - w) = (V - v)*w/w' + v
	 *	==> (WV-Wv+Wv+w'v-wv)/(W+w'-w) = (V - v)*w/w' + v
	 *	==> (WV - Wv)/(W + w' - w) + v = (V - v)*w/w' + v
	 *	==>	(V - v)*W/(W + w' - w) = (V - v)*w/w' (3)
	 *
	 * Since we are doing at !0-lag point which means V != v, we
	 * can simplify (3):
	 *
	 *	==>	W / (W + w' - w) = w / w'
	 *	==>	Ww' = Ww + ww' - ww
	 *	==>	W * (w' - w) = w * (w' - w)
	 *	==>	W = w	(re-weight indicates w' != w)
	 *
	 * So the cfs_rq contains only one entity, hence vruntime of
	 * the entity @v should always equal to the cfs_rq's weighted
	 * average vruntime @V, which means we will always re-weight
	 * at 0-lag point, thus breach assumption. Proof completed.
	 *
	 *
	 * COROLLARY #2: Re-weight does NOT affect weighted average
	 * vruntime of all the entities.
	 *
	 * Proof: According to corollary #1, Eq. (1) should be:
	 *
	 *	(V - v)*w = (V' - v')*w'
	 *	==>    v' = V' - (V - v)*w/w'		(4)
	 *
	 * According to the weighted average formula, we have:
	 *
	 *	V' = (WV - wv + w'v') / (W - w + w')
	 *	   = (WV - wv + w'(V' - (V - v)w/w')) / (W - w + w')
	 *	   = (WV - wv + w'V' - Vw + wv) / (W - w + w')
	 *	   = (WV + w'V' - Vw) / (W - w + w')
	 *
	 *	==>  V'*(W - w + w') = WV + w'V' - Vw
	 *	==>	V' * (W - w) = (W - w) * V	(5)
	 *
	 * If the entity is the only one in the cfs_rq, then reweight
	 * always occurs at 0-lag point, so V won't change. Or else
	 * there are other entities, hence W != w, then Eq. (5) turns
	 * into V' = V. So V won't change in either case, proof done.
	 *
	 *
	 * So according to corollary #1 & #2, the effect of re-weight
	 * on vruntime should be:
	 *
	 *	v' = V' - (V - v) * w / w'		(4)
	 *	   = V  - (V - v) * w / w'
	 *	   = V  - vl * w / w'
	 *	   = V  - vl'
	 */
	se->vlag = div64_long(se->vlag * old_وزن, وزن);

	/*
	 * DEADLINE
	 * --------
	 *
	 * When the weight changes, the virtual time slope changes and
	 * we should adjust the relative virtual deadline accordingly.
	 *
	 *	d' = v' + (d - v)*w/w'
	 *	   = V' - (V - v)*w/w' + (d - v)*w/w'
	 *	   = V  - (V - v)*w/w' + (d - v)*w/w'
	 *	   = V  + (d - V)*w/w'
	 */
	اگر (se->rel_deadline)
		se->deadline = div64_long(se->deadline * old_وزن, وزن);

	اگر (rel_vprot)
		se->vprot = div64_long(se->vprot * old_وزن, وزن);
}

ساکن خالی reweight_eevdf(ساخت cfs_rq *cfs_rq, ساخت sched_entity *se,
			   غیرعلامتی طویل وزن, bool on_rq)
{
	bool curr = cfs_rq->curr == se;
	bool rel_vprot = false;
	u64 avruntime = 0;

	اگر (se->h_بھرو.وزن == وزن)
		واپس;

	اگر (on_rq) {
		avruntime = avg_vruntime(cfs_rq);
		se->vlag = entity_lag(cfs_rq, se, avruntime);
		se->deadline -= avruntime;
		se->rel_deadline = 1;
		اگر (curr && protect_قاش(se)) {
			se->vprot -= avruntime;
			rel_vprot = true;
		}

		cfs_rq->h_nr_queued--;
		اگر (!curr)
			__dequeue_entity(cfs_rq, se);
	}

	rescale_entity(se, وزن, rel_vprot);

	update_بھرو_سیٹ(&se->h_بھرو, وزن);

	اگر (on_rq) {
		اگر (rel_vprot)
			se->vprot += avruntime;
		se->deadline += avruntime;
		se->rel_deadline = 0;
		se->vruntime = avruntime - se->vlag;

		اگر (!curr)
			__enqueue_entity(cfs_rq, se);
		cfs_rq->h_nr_queued++;
	}
}

ساکن خالی reweight_entity(ساخت cfs_rq *cfs_rq, ساخت sched_entity *se,
			    غیرعلامتی طویل وزن)
{
	اگر (se->بھرو.وزن == وزن)
		واپس;

	اگر (se->on_rq) {
		WARN_ON_ONCE(cfs_rq != cfs_rq_کا(se));
		update_بھرو_sub(&cfs_rq->بھرو, se->بھرو.وزن);
	}
	قطار_سے_نکالو_بھرو_avg(cfs_rq, se);

	update_بھرو_سیٹ(&se->بھرو, وزن);

	کرو {
		u32 divider = get_pelt_divider(&se->avg);
		se->avg.بھرو_avg = div_u64(se_وزن(se) * se->avg.بھرو_مجموع, divider);
	} جب (0);

	قطار_میں_ڈالو_بھرو_avg(cfs_rq, se);

	اگر (se->on_rq)
		update_بھرو_جوڑو(&cfs_rq->بھرو, se->بھرو.وزن);
}

/*
 * weight = NICE_0_LOAD;
 * for_each_entity_se(se)
 *   weight = __calc_prop_weight(cfs_rq_of(se), se, weight);
 */
ساکن __always_inline
غیرعلامتی طویل __calc_prop_weight(ساخت cfs_rq *cfs_rq, ساخت sched_entity *se,
				 غیرعلامتی طویل وزن)
{
	وزن *= se->بھرو.وزن;
	اگر (والد_entity(se))
		وزن /= cfs_rq->بھرو.وزن;
	ورنہ
		وزن /= NICE_0_بھرو;

	واپس max(وزن, MIN_SHARES);
}

ساکن خالی reweight_task_fair(ساخت rq *rq, ساخت task_struct *p,
			       ثابت ساخت بھرو_وزن *lw)
{
	ساخت sched_entity *se = &p->se;
	غیرعلامتی طویل وزن = NICE_0_بھرو;

	اگر (se->on_rq)
		update_curr_fair(rq);

	reweight_entity(cfs_rq_کا(se), se, lw->وزن);
	se->بھرو.inv_وزن = lw->inv_وزن;

	اگر (!se->on_rq)
		واپس;

	for_each_sched_entity(se)
		وزن = __calc_prop_weight(cfs_rq_کا(se), se, وزن);

	reweight_eevdf(&rq->cfs, &p->se, وزن, p->se.on_rq);
}

ساکن اندرونی صحیح throttled_hierarchy(ساخت cfs_rq *cfs_rq);

#اگر_تعریف_شدہ CONFIG_FAIR_گروہ_SCHED
/*
 * All this does is approximate the hierarchical proportion which includes that
 * global sum we all love to hate.
 *
 * That is, the weight of a group entity, is the proportional share of the
 * group weight based on the group runqueue weights. That is:
 *
 *                     tg->weight * grq->load.weight
 *   ge->load.weight = -----------------------------               (1)
 *                       \Sum grq->load.weight
 *
 * Now, because computing that sum is prohibitively expensive to compute (been
 * there, done that) we approximate it with this average stuff. The average
 * moves slower and therefore the approximation is cheaper and more stable.
 *
 * So instead of the above, we substitute:
 *
 *   grq->load.weight -> grq->avg.load_avg                         (2)
 *
 * which yields the following:
 *
 *                     tg->weight * grq->avg.load_avg
 *   ge->load.weight = ------------------------------              (3)
 *                             tg->load_avg
 *
 * Where: tg->load_avg ~= \Sum grq->avg.load_avg
 *
 * That is shares_avg, and it is right (given the approximation (2)).
 *
 * The problem with it is that because the average is slow -- it was designed
 * to be exactly that of course -- this leads to transients in boundary
 * conditions. In specific, the case where the group was idle and we start the
 * one task. It takes time for our CPU's grq->avg.load_avg to build up,
 * yielding bad latency etc..
 *
 * Now, in that special case (1) reduces to:
 *
 *                     tg->weight * grq->load.weight
 *   ge->load.weight = ----------------------------- = tg->weight   (4)
 *                         grp->load.weight
 *
 * That is, the sum collapses because all other CPUs are idle; the UP scenario.
 *
 * So what we do is modify our approximation (3) to approach (4) in the (near)
 * UP case, like:
 *
 *   ge->load.weight =
 *
 *              tg->weight * grq->load.weight
 *     ---------------------------------------------------         (5)
 *     tg->load_avg - grq->avg.load_avg + grq->load.weight
 *
 * But because grq->load.weight can drop to 0, resulting in a divide by zero,
 * we need to use grq->avg.load_avg as its lower bound, which then gives:
 *
 *
 *                     tg->weight * grq->load.weight
 *   ge->load.weight = -----------------------------		   (6)
 *                             tg_load_avg'
 *
 * Where:
 *
 *   tg_load_avg' = tg->load_avg - grq->avg.load_avg +
 *                  max(grq->load.weight, grq->avg.load_avg)
 *
 * And that is shares_weight and is icky. In the (near) UP case it approaches
 * (4) while in the normal case it approaches (3). It consistently
 * overestimates the ge->load.weight and therefore:
 *
 *   \Sum ge->load.weight >= tg->weight
 *
 * hence icky!
 */
ساکن طویل __calc_smp_shares(ساخت cfs_rq *cfs_rq, طویل tg_shares, طویل shares_max)
{
	ساخت task_گروہ *tg = cfs_rq->tg;
	طویل tg_وزن, بھرو, shares;

	بھرو = max(scale_بھرو_نیچے_کو(cfs_rq->بھرو.وزن), cfs_rq->avg.بھرو_avg);

	tg_وزن = atomic_long_read(&tg->بھرو_avg);

	/* Ensure tg_weight >= load */
	tg_وزن -= cfs_rq->tg_بھرو_avg_contrib;
	tg_وزن += بھرو;

	shares = (tg_shares * بھرو);
	اگر (tg_وزن)
		shares /= tg_وزن;

	/*
	 * MIN_SHARES has to be unscaled here to support per-CPU partitioning
	 * of a group with small tg->shares value. It is a floor value which is
	 * assigned as a minimum load.weight to the sched_entity representing
	 * the group on a CPU.
	 *
	 * E.g. on 64-bit for a group with tg->shares of scale_load(15)=15*1024
	 * on an 8-core system with 8 tasks each runnable on one CPU shares has
	 * to be 15*1024*1/8=1920 instead of scale_load(MIN_SHARES)=2*1024. In
	 * case no task is runnable on a CPU MIN_SHARES=2 should be returned
	 * instead of 0.
	 */
	واپس clamp_t(طویل, shares, MIN_SHARES, shares_max);
}

ساکن صحیح tg_cpus(ساخت task_گروہ *tg)
{
	صحیح nr = عدد_online_cpus();

	اگر (cpusets_enabled()) {
		ساخت cgroup *cgrp = tg->css.cgroup;
		اگر (cgrp)
			nr = cpuset_عدد_cpus(cgrp);
	}

	/*
	 * An empty cpuset would propagate a 0 shares_max into
	 * __calc_smp_shares(), where clamp() yields hi when hi < lo and so
	 * defeats the MIN_SHARES floor. Match tg_tasks(), which floors at 1.
	 */
	واپس max(nr, 1);
}

ساکن اندرونی صحیح tg_tasks(ساخت task_گروہ *tg)
{
	واپس max(1, atomic_long_read(&tg->runnable_avg) >> SCHED_استعداد_سرکاؤ);
}

/*
 * Func: fraction(nr_tasks * tg->shares)
 *
 * Scale tg->shares by the number of tasks.
 */
ساکن طویل calc_tasks_shares(ساخت cfs_rq *cfs_rq)
{
	ساخت task_گروہ *tg = cfs_rq->tg;
	صحیح nr = tg_tasks(tg);
	طویل tg_shares = READ_ONCE(tg->shares);
	واپس __calc_smp_shares(cfs_rq, nr * tg_shares, nr * tg_shares);
}

/*
 * Func: min(fraction(nr_cpus * tg->shares), nice -20)
 *
 * Scale tg->shares by the maximal number of CPUs; but clip the max shares at
 * nice -20, otherwise a single spinner on a 512 CPU machine would result in
 * 512*NICE_0_LOAD, which is also crazy.
 */
ساکن طویل calc_max_shares(ساخت cfs_rq *cfs_rq)
{
	ساخت task_گروہ *tg = cfs_rq->tg;
	صحیح nr = tg_cpus(tg);
	طویل tg_shares = READ_ONCE(tg->shares);
	طویل max_shares = scale_بھرو(sched_prio_مقصد_وزن[0]);
	واپس __calc_smp_shares(cfs_rq, tg_shares * nr, max_shares);
}

/*
 * Func: fraction(nr * tg->shares); nr = min(nr_tasks, nr_cpus)
 *
 * Scales between "smp" and "max" in a natural way. No longer needs clipping
 * since there are no unnatural inflations like with "max".
 */
ساکن طویل calc_concur_shares(ساخت cfs_rq *cfs_rq)
{
	ساخت task_گروہ *tg = cfs_rq->tg;
	صحیح nr = min(tg_tasks(tg), tg_cpus(tg));
	طویل tg_shares = READ_ONCE(tg->shares);
	واپس __calc_smp_shares(cfs_rq, nr * tg_shares, nr * tg_shares);
}

/*
 * Func: fraction(tg->shares)
 *
 * This infamously results in tiny shares when you have many CPUs.
 */
ساکن طویل calc_smp_shares(ساخت cfs_rq *cfs_rq)
{
	ساخت task_گروہ *tg = cfs_rq->tg;
	طویل tg_shares = READ_ONCE(tg->shares);
	واپس __calc_smp_shares(cfs_rq, tg_shares, tg_shares);
}

/*
 * Ignore this pesky SMP stuff, use (4).
 */
ساکن طویل calc_اوپر_shares(ساخت cfs_rq *cfs_rq)
{
	ساخت task_گروہ *tg = cfs_rq->tg;
	واپس READ_ONCE(tg->shares);
}

DEFINE_STATIC_CALL(calc_گروہ_shares, calc_concur_shares);

خالی __sched_cgroup_mode_update(صحیح منوال)
{
	طویل (*func)(ساخت cfs_rq *);
	انتخاب (منوال) {
	حالت 0:
		func = &calc_اوپر_shares;
		توڑو;
	حالت 1:
		func = &calc_smp_shares;
		توڑو;
	حالت 2:
	طےشدہ:
		func = &calc_concur_shares;
		توڑو;
	حالت 3:
		func = &calc_max_shares;
		توڑو;
	حالت 4:
		func = &calc_tasks_shares;
		توڑو;
	}
	static_call_update(calc_گروہ_shares, func);
}

/*
 * Recomputes the group entity based on the current state of its group
 * runqueue.
 */
ساکن خالی update_cfs_گروہ(ساخت sched_entity *se)
{
	ساخت cfs_rq *gcfs_rq = گروہ_cfs_rq(se);
	طویل shares;

	/*
	 * When a group becomes empty, preserve its weight. This matters for
	 * DELAY_DEQUEUE.
	 */
	اگر (!gcfs_rq || !gcfs_rq->بھرو.وزن)
		واپس;

	shares = static_call(calc_گروہ_shares)(gcfs_rq);
	reweight_entity(cfs_rq_کا(se), se, shares);
}

#ورنہ /* !CONFIG_FAIR_GROUP_SCHED: */
ساکن اندرونی خالی update_cfs_گروہ(ساخت sched_entity *se)
{
}
#اگر_ختم /* !CONFIG_FAIR_GROUP_SCHED */

ساکن اندرونی خالی cfs_rq_util_change(ساخت cfs_rq *cfs_rq, صحیح flags)
{
	ساخت rq *rq = rq_کا(cfs_rq);

	اگر (&rq->cfs == cfs_rq) {
		/*
		 * There are a few boundary cases this might miss but it should
		 * get called often enough that that should (hopefully) not be
		 * a real problem.
		 *
		 * It will not get called when we go idle, because the idle
		 * thread is a different class (!fair), nor will the utilization
		 * number include things like RT tasks.
		 *
		 * As is, the util number is not freq-invariant (we'd have to
		 * implement arch_scale_freq_capacity() for that).
		 *
		 * See cpu_util_cfs().
		 */
		cpufreq_update_util(rq, flags);
	}
}

ساکن اندرونی bool بھرو_avg_is_decayed(ساخت sched_avg *sa)
{
	اگر (sa->بھرو_مجموع)
		واپس false;

	اگر (sa->util_مجموع)
		واپس false;

	اگر (sa->runnable_مجموع)
		واپس false;

	/*
	 * _avg must be null when _sum are null because _avg = _sum / divider
	 * Make sure that rounding and/or propagation of PELT values never
	 * break this.
	 */
	WARN_ON_ONCE(sa->بھرو_avg ||
		      sa->util_avg ||
		      sa->runnable_avg);

	واپس true;
}

ساکن اندرونی u64 cfs_rq_آخری_update_وقت(ساخت cfs_rq *cfs_rq)
{
	واپس u64_u32_بھرو_copy(cfs_rq->avg.آخری_update_وقت,
				 cfs_rq->آخری_update_وقت_copy);
}
#اگر_تعریف_شدہ CONFIG_FAIR_گروہ_SCHED
/*
 * Because list_add_leaf_cfs_rq always places a child cfs_rq on the list
 * immediately before a parent cfs_rq, and cfs_rqs are removed from the list
 * bottom-up, we only have to test whether the cfs_rq before us on the list
 * is our child.
 * If cfs_rq is not on the list, test whether a child needs its to be added to
 * connect a branch to the tree  * (see list_add_leaf_cfs_rq() for details).
 */
ساکن اندرونی bool بچہ_cfs_rq_on_list(ساخت cfs_rq *cfs_rq)
{
	ساخت cfs_rq *پچھلا_cfs_rq;
	ساخت list_سر *پچھلا;
	ساخت rq *rq = rq_کا(cfs_rq);

	اگر (cfs_rq->on_list) {
		پچھلا = cfs_rq->برگ_cfs_rq_list.پچھلا;
	} ورنہ {
		پچھلا = rq->tmp_alone_branch;
	}

	اگر (پچھلا == &rq->برگ_cfs_rq_list)
		واپس false;

	پچھلا_cfs_rq = container_کا(پچھلا, ساخت cfs_rq, برگ_cfs_rq_list);

	واپس (پچھلا_cfs_rq->tg->والد == cfs_rq->tg);
}

ساکن اندرونی bool cfs_rq_is_decayed(ساخت cfs_rq *cfs_rq)
{
	اگر (cfs_rq->بھرو.وزن)
		واپس false;

	اگر (!بھرو_avg_is_decayed(&cfs_rq->avg))
		واپس false;

	اگر (بچہ_cfs_rq_on_list(cfs_rq))
		واپس false;

	اگر (cfs_rq->tg_بھرو_avg_contrib)
		واپس false;

	واپس true;
}

/**
 * update_tg_load_avg - update the tg's load avg
 * @cfs_rq: the cfs_rq whose avg changed
 *
 * This function 'ensures': tg->load_avg := \Sum tg->cfs_rq[]->avg.load.
 * However, because tg->load_avg is a global value there are performance
 * considerations.
 *
 * In order to avoid having to look at the other cfs_rq's, we use a
 * differential update where we store the last value we propagated. This in
 * turn allows skipping updates if the differential is 'small'.
 *
 * Updating tg's load_avg is necessary before update_cfs_group().
 */
ساکن اندرونی خالی update_tg_بھرو_avg(ساخت cfs_rq *cfs_rq)
{
	طویل dl, dr;
	u64 now;

	/*
	 * No need to update load_avg for root_task_group as it is not used.
	 */
	اگر (cfs_rq->tg == &جڑ_task_گروہ)
		واپس;

	/* rq has been offline and doesn't contribute to the share anymore: */
	اگر (!cpu_فعال(cpu_کا(rq_کا(cfs_rq))))
		واپس;

	/*
	 * For migration heavy workloads, access to tg->load_avg can be
	 * unbound. Limit the update rate to at most once per ms.
	 */
	now = rq_clock(rq_کا(cfs_rq));
	اگر (now - cfs_rq->آخری_update_tg_بھرو_avg < NSEC_فی_MSEC)
		واپس;

	dl = cfs_rq->avg.بھرو_avg - cfs_rq->tg_بھرو_avg_contrib;
	dr = cfs_rq->avg.runnable_avg - cfs_rq->tg_runnable_avg_contrib;
	اگر (مطلق_قدر(dl) > cfs_rq->tg_بھرو_avg_contrib / 64 ||
	    مطلق_قدر(dr) > cfs_rq->tg_runnable_avg_contrib / 64) {
		atomic_long_جوڑو(dl, &cfs_rq->tg->بھرو_avg);
		atomic_long_جوڑو(dr, &cfs_rq->tg->runnable_avg);
		cfs_rq->tg_بھرو_avg_contrib = cfs_rq->avg.بھرو_avg;
		cfs_rq->tg_runnable_avg_contrib = cfs_rq->avg.runnable_avg;
		cfs_rq->آخری_update_tg_بھرو_avg = now;
	}
}

ساکن اندرونی خالی clear_tg_بھرو_avg(ساخت cfs_rq *cfs_rq)
{
	طویل dl, dr;
	u64 now;

	/*
	 * No need to update load_avg for root_task_group, as it is not used.
	 */
	اگر (cfs_rq->tg == &جڑ_task_گروہ)
		واپس;

	now = rq_clock(rq_کا(cfs_rq));
	dl = 0 - cfs_rq->tg_بھرو_avg_contrib;
	dr = 0 - cfs_rq->tg_runnable_avg_contrib;
	atomic_long_جوڑو(dl, &cfs_rq->tg->بھرو_avg);
	atomic_long_جوڑو(dr, &cfs_rq->tg->runnable_avg);
	cfs_rq->tg_بھرو_avg_contrib = 0;
	cfs_rq->tg_runnable_avg_contrib = 0;
	cfs_rq->آخری_update_tg_بھرو_avg = now;
}

/* CPU offline callback: */
ساکن خالی __maybe_unused clear_tg_offline_cfs_rqs(ساخت rq *rq)
{
	ساخت task_گروہ *tg;

	lockdep_assert_rq_held(rq);

	/*
	 * The rq clock has already been updated in
	 * set_rq_offline(), so we should skip updating
	 * the rq clock again in unthrottle_cfs_rq().
	 */
	rq_clock_آغاز_loop_update(rq);

	guard(rcu)();

	list_for_each_اندراج_rcu(tg, &task_groups, list) {
		ساخت cfs_rq *cfs_rq = tg_cfs_rq(tg, cpu_کا(rq));

		clear_tg_بھرو_avg(cfs_rq);
	}

	rq_clock_stop_loop_update(rq);
}

/*
 * Called within set_task_rq() right before setting a task's CPU. The
 * caller only guarantees p->pi_lock is held; no other assumptions,
 * including the state of rq->lock, should be made.
 */
خالی سیٹ_task_rq_fair(ساخت sched_entity *se,
		      ساخت cfs_rq *پچھلا, ساخت cfs_rq *اگلا)
{
	u64 p_آخری_update_وقت;
	u64 n_آخری_update_وقت;

	اگر (!sched_feat(ATTACH_AGE_بھرو))
		واپس;

	/*
	 * We are supposed to update the task to "current" time, then its up to
	 * date and ready to go to new CPU/cfs_rq. But we have difficulty in
	 * getting what current time is, so simply throw away the out-of-date
	 * time. This will result in the wakee task is less decayed, but giving
	 * the wakee more load sounds not bad.
	 */
	اگر (!(se->avg.آخری_update_وقت && پچھلا))
		واپس;

	p_آخری_update_وقت = cfs_rq_آخری_update_وقت(پچھلا);
	n_آخری_update_وقت = cfs_rq_آخری_update_وقت(اگلا);

	__update_load_avg_blocked_se(p_آخری_update_وقت, se);
	se->avg.آخری_update_وقت = n_آخری_update_وقت;
}

/*
 * When on migration a sched_entity joins/leaves the PELT hierarchy, we need to
 * propagate its contribution. The key to this propagation is the invariant
 * that for each group:
 *
 *   ge->avg == grq->avg						(1)
 *
 * _IFF_ we look at the pure running and runnable sums. Because they
 * represent the very same entity, just at different points in the hierarchy.
 *
 * Per the above update_tg_cfs_util() and update_tg_cfs_runnable() are trivial
 * and simply copies the running/runnable sum over (but still wrong, because
 * the group entity and group rq do not have their PELT windows aligned).
 *
 * However, update_tg_cfs_load() is more complex. So we have:
 *
 *   ge->avg.load_avg = ge->load.weight * ge->avg.runnable_avg		(2)
 *
 * And since, like util, the runnable part should be directly transferable,
 * the following would _appear_ to be the straight forward approach:
 *
 *   grq->avg.load_avg = grq->load.weight * grq->avg.runnable_avg	(3)
 *
 * And per (1) we have:
 *
 *   ge->avg.runnable_avg == grq->avg.runnable_avg
 *
 * Which gives:
 *
 *                      ge->load.weight * grq->avg.load_avg
 *   ge->avg.load_avg = -----------------------------------		(4)
 *                               grq->load.weight
 *
 * Except that is wrong!
 *
 * Because while for entities historical weight is not important and we
 * really only care about our future and therefore can consider a pure
 * runnable sum, runqueues can NOT do this.
 *
 * We specifically want runqueues to have a load_avg that includes
 * historical weights. Those represent the blocked load, the load we expect
 * to (shortly) return to us. This only works by keeping the weights as
 * integral part of the sum. We therefore cannot decompose as per (3).
 *
 * Another reason this doesn't work is that runnable isn't a 0-sum entity.
 * Imagine a rq with 2 tasks that each are runnable 2/3 of the time. Then the
 * rq itself is runnable anywhere between 2/3 and 1 depending on how the
 * runnable section of these tasks overlap (or not). If they were to perfectly
 * align the rq as a whole would be runnable 2/3 of the time. If however we
 * always have at least 1 runnable task, the rq as a whole is always runnable.
 *
 * So we'll have to approximate.. :/
 *
 * Given the constraint:
 *
 *   ge->avg.running_sum <= ge->avg.runnable_sum <= LOAD_AVG_MAX
 *
 * We can construct a rule that adds runnable to a rq by assuming minimal
 * overlap.
 *
 * On removal, we'll assume each task is equally runnable; which yields:
 *
 *   grq->avg.runnable_sum = grq->avg.load_sum / grq->load.weight
 *
 * XXX: only do this for the part of runnable > running ?
 *
 */
ساکن اندرونی خالی
update_tg_cfs_util(ساخت cfs_rq *cfs_rq, ساخت sched_entity *se, ساخت cfs_rq *gcfs_rq)
{
	طویل delta_مجموع, delta_avg = gcfs_rq->avg.util_avg - se->avg.util_avg;
	u32 new_مجموع, divider;

	/* Nothing to update */
	اگر (!delta_avg)
		واپس;

	/*
	 * cfs_rq->avg.period_contrib can be used for both cfs_rq and se.
	 * See ___update_load_avg() for details.
	 */
	divider = get_pelt_divider(&cfs_rq->avg);

	/* Set new sched_entity's utilization */
	se->avg.util_avg = gcfs_rq->avg.util_avg;
	new_مجموع = se->avg.util_avg * divider;
	delta_مجموع = (طویل)new_مجموع - (طویل)se->avg.util_مجموع;
	se->avg.util_مجموع = new_مجموع;

	/* Update parent cfs_rq utilization */
	__update_sa(&cfs_rq->avg, util, delta_avg, delta_مجموع);
}

ساکن اندرونی خالی
update_tg_cfs_runnable(ساخت cfs_rq *cfs_rq, ساخت sched_entity *se, ساخت cfs_rq *gcfs_rq)
{
	طویل delta_مجموع, delta_avg = gcfs_rq->avg.runnable_avg - se->avg.runnable_avg;
	u64 new_مجموع;
	u32 divider;

	/* Nothing to update */
	اگر (!delta_avg)
		واپس;

	/*
	 * cfs_rq->avg.period_contrib can be used for both cfs_rq and se.
	 * See ___update_load_avg() for details.
	 */
	divider = get_pelt_divider(&cfs_rq->avg);

	/* Set new sched_entity's runnable */
	se->avg.runnable_avg = gcfs_rq->avg.runnable_avg;
	new_مجموع = (u64)se->avg.runnable_avg * divider;
	delta_مجموع = (طویل)new_مجموع - (طویل)se->avg.runnable_مجموع;
	se->avg.runnable_مجموع = new_مجموع;

	/* Update parent cfs_rq runnable */
	__update_sa(&cfs_rq->avg, runnable, delta_avg, delta_مجموع);
}

ساکن اندرونی خالی
update_tg_cfs_بھرو(ساخت cfs_rq *cfs_rq, ساخت sched_entity *se, ساخت cfs_rq *gcfs_rq)
{
	طویل delta_avg, running_مجموع, runnable_مجموع = gcfs_rq->prop_runnable_مجموع;
	غیرعلامتی طویل بھرو_avg;
	u64 بھرو_مجموع = 0;
	s64 delta_مجموع;
	u32 divider;

	اگر (!runnable_مجموع)
		واپس;

	gcfs_rq->prop_runnable_مجموع = 0;

	/*
	 * cfs_rq->avg.period_contrib can be used for both cfs_rq and se.
	 * See ___update_load_avg() for details.
	 */
	divider = get_pelt_divider(&cfs_rq->avg);

	اگر (runnable_مجموع >= 0) {
		/*
		 * Add runnable; clip at LOAD_AVG_MAX. Reflects that until
		 * the CPU is saturated running == runnable.
		 */
		runnable_مجموع += se->avg.بھرو_مجموع;
		runnable_مجموع = min_t(طویل, runnable_مجموع, divider);
	} ورنہ {
		/*
		 * Estimate the new unweighted runnable_sum of the gcfs_rq by
		 * assuming all tasks are equally runnable.
		 */
		اگر (scale_بھرو_نیچے_کو(gcfs_rq->بھرو.وزن)) {
			بھرو_مجموع = div_u64(gcfs_rq->avg.بھرو_مجموع,
				scale_بھرو_نیچے_کو(gcfs_rq->بھرو.وزن));
		}

		/* But make sure to not inflate se's runnable */
		runnable_مجموع = min(se->avg.بھرو_مجموع, بھرو_مجموع);
	}

	/*
	 * runnable_sum can't be lower than running_sum
	 * Rescale running sum to be in the same range as runnable sum
	 * running_sum is in [0 : LOAD_AVG_MAX <<  SCHED_CAPACITY_SHIFT]
	 * runnable_sum is in [0 : LOAD_AVG_MAX]
	 */
	running_مجموع = se->avg.util_مجموع >> SCHED_استعداد_سرکاؤ;
	runnable_مجموع = max(runnable_مجموع, running_مجموع);

	بھرو_مجموع = se_وزن(se) * runnable_مجموع;
	بھرو_avg = div_u64(بھرو_مجموع, divider);

	delta_avg = بھرو_avg - se->avg.بھرو_avg;
	اگر (!delta_avg)
		واپس;

	delta_مجموع = بھرو_مجموع - (s64)se_وزن(se) * se->avg.بھرو_مجموع;

	se->avg.بھرو_مجموع = runnable_مجموع;
	se->avg.بھرو_avg = بھرو_avg;
	__update_sa(&cfs_rq->avg, بھرو, delta_avg, delta_مجموع);
}

ساکن اندرونی خالی جوڑو_tg_cfs_propagate(ساخت cfs_rq *cfs_rq, طویل runnable_مجموع)
{
	cfs_rq->propagate = 1;
	cfs_rq->prop_runnable_مجموع += runnable_مجموع;
}

/* Update task and its cfs_rq load average */
ساکن اندرونی صحیح propagate_entity_بھرو_avg(ساخت sched_entity *se)
{
	ساخت cfs_rq *cfs_rq, *gcfs_rq;

	اگر (entity_is_task(se))
		واپس 0;

	gcfs_rq = گروہ_cfs_rq(se);
	اگر (!gcfs_rq->propagate)
		واپس 0;

	gcfs_rq->propagate = 0;

	cfs_rq = cfs_rq_کا(se);

	جوڑو_tg_cfs_propagate(cfs_rq, gcfs_rq->prop_runnable_مجموع);

	update_tg_cfs_util(cfs_rq, se, gcfs_rq);
	update_tg_cfs_runnable(cfs_rq, se, gcfs_rq);
	update_tg_cfs_بھرو(cfs_rq, se, gcfs_rq);

	trace_pelt_cfs_tp(cfs_rq);
	trace_pelt_se_tp(se);

	واپس 1;
}

/*
 * Check if we need to update the load and the utilization of a blocked
 * group_entity:
 */
ساکن اندرونی bool skip_blocked_update(ساخت sched_entity *se)
{
	ساخت cfs_rq *gcfs_rq = گروہ_cfs_rq(se);

	/*
	 * If sched_entity still have not zero load or utilization, we have to
	 * decay it:
	 */
	اگر (se->avg.بھرو_avg || se->avg.util_avg)
		واپس false;

	/*
	 * If there is a pending propagation, we have to update the load and
	 * the utilization of the sched_entity:
	 */
	اگر (gcfs_rq->propagate)
		واپس false;

	/*
	 * Otherwise, the load and the utilization of the sched_entity is
	 * already zero and there is no pending propagation, so it will be a
	 * waste of time to try to decay it:
	 */
	واپس true;
}

#ورنہ /* !CONFIG_FAIR_GROUP_SCHED: */

ساکن اندرونی خالی update_tg_بھرو_avg(ساخت cfs_rq *cfs_rq) {}

ساکن اندرونی خالی clear_tg_offline_cfs_rqs(ساخت rq *rq) {}

ساکن اندرونی صحیح propagate_entity_بھرو_avg(ساخت sched_entity *se)
{
	واپس 0;
}

ساکن اندرونی خالی جوڑو_tg_cfs_propagate(ساخت cfs_rq *cfs_rq, طویل runnable_مجموع) {}

#اگر_ختم /* !CONFIG_FAIR_GROUP_SCHED */

#اگر_تعریف_شدہ CONFIG_NO_HZ_COMMON
ساکن اندرونی خالی migrate_se_pelt_lag(ساخت sched_entity *se)
{
	u64 throttled = 0, now, lut;
	ساخت cfs_rq *cfs_rq;
	ساخت rq *rq;
	bool is_idle;

	اگر (بھرو_avg_is_decayed(&se->avg))
		واپس;

	cfs_rq = cfs_rq_کا(se);
	rq = rq_کا(cfs_rq);

	rcu_read_تالا();
	is_idle = is_idle_task(rcu_dereference_all(rq->curr));
	rcu_read_unlock();

	/*
	 * The lag estimation comes with a cost we don't want to pay all the
	 * time. Hence, limiting to the case where the source CPU is idle and
	 * we know we are at the greatest risk to have an outdated clock.
	 */
	اگر (!is_idle)
		واپس;

	/*
	 * Estimated "now" is: last_update_time + cfs_idle_lag + rq_idle_lag, where:
	 *
	 *   last_update_time (the cfs_rq's last_update_time)
	 *	= cfs_rq_clock_pelt()@cfs_rq_idle
	 *      = rq_clock_pelt()@cfs_rq_idle
	 *        - cfs->throttled_clock_pelt_time@cfs_rq_idle
	 *
	 *   cfs_idle_lag (delta between rq's update and cfs_rq's update)
	 *      = rq_clock_pelt()@rq_idle - rq_clock_pelt()@cfs_rq_idle
	 *
	 *   rq_idle_lag (delta between now and rq's update)
	 *      = sched_clock_cpu() - rq_clock()@rq_idle
	 *
	 * We can then write:
	 *
	 *    now = rq_clock_pelt()@rq_idle - cfs->throttled_clock_pelt_time +
	 *          sched_clock_cpu() - rq_clock()@rq_idle
	 * Where:
	 *      rq_clock_pelt()@rq_idle is rq->clock_pelt_idle
	 *      rq_clock()@rq_idle      is rq->clock_idle
	 *      cfs->throttled_clock_pelt_time@cfs_rq_idle
	 *                              is cfs_rq->throttled_pelt_idle
	 */

#اگر_تعریف_شدہ CONFIG_CFS_BANDWIDTH
	throttled = u64_u32_بھرو(cfs_rq->throttled_pelt_idle);
	/* The clock has been stopped for throttling */
	اگر (throttled == U64_MAX)
		واپس;
#اگر_ختم
	now = u64_u32_بھرو(rq->clock_pelt_idle);
	/*
	 * Paired with _update_idle_rq_clock_pelt(). It ensures at the worst case
	 * is observed the old clock_pelt_idle value and the new clock_idle,
	 * which lead to an underestimation. The opposite would lead to an
	 * overestimation.
	 */
	smp_rmb();
	lut = cfs_rq_آخری_update_وقت(cfs_rq);

	now -= throttled;
	اگر (now < lut)
		/*
		 * cfs_rq->avg.last_update_time is more recent than our
		 * estimation, let's use it.
		 */
		now = lut;
	ورنہ
		now += sched_clock_cpu(cpu_کا(rq)) - u64_u32_بھرو(rq->clock_idle);

	__update_load_avg_blocked_se(now, se);
}
#ورنہ /* !CONFIG_NO_HZ_COMMON: */
ساکن خالی migrate_se_pelt_lag(ساخت sched_entity *se) {}
#اگر_ختم /* !CONFIG_NO_HZ_COMMON */

/**
 * update_cfs_rq_load_avg - update the cfs_rq's load/util averages
 * @now: current time, as per cfs_rq_clock_pelt()
 * @cfs_rq: cfs_rq to update
 *
 * The cfs_rq avg is the direct sum of all its entities (blocked and runnable)
 * avg. The immediate corollary is that all (fair) tasks must be attached.
 *
 * cfs_rq->avg is used for task_h_load() and update_cfs_group() for example.
 *
 * Return: true if the load decayed or we removed load.
 *
 * Since both these conditions indicate a changed cfs_rq->avg.load we should
 * call update_tg_load_avg() when this function returns true.
 */
ساکن اندرونی صحیح
update_cfs_rq_بھرو_avg(u64 now, ساخت cfs_rq *cfs_rq)
{
	غیرعلامتی طویل removed_بھرو = 0, removed_util = 0, removed_runnable = 0;
	ساخت sched_avg *sa = &cfs_rq->avg;
	صحیح decayed = 0;

	اگر (cfs_rq->removed.nr) {
		غیرعلامتی طویل r;
		u32 divider = get_pelt_divider(&cfs_rq->avg);

		raw_spin_تالا(&cfs_rq->removed.تالا);
		تبادلہ(cfs_rq->removed.util_avg, removed_util);
		تبادلہ(cfs_rq->removed.بھرو_avg, removed_بھرو);
		تبادلہ(cfs_rq->removed.runnable_avg, removed_runnable);
		cfs_rq->removed.nr = 0;
		raw_spin_unlock(&cfs_rq->removed.تالا);

		r = removed_بھرو;
		__update_sa(sa, بھرو, -r, -r*divider);

		r = removed_util;
		__update_sa(sa, util, -r, -r*divider);

		r = removed_runnable;
		__update_sa(sa, runnable, -r, -r*divider);

		/*
		 * removed_runnable is the unweighted version of removed_load so we
		 * can use it to estimate removed_load_sum.
		 */
		جوڑو_tg_cfs_propagate(cfs_rq,
			-(طویل)(removed_runnable * divider) >> SCHED_استعداد_سرکاؤ);

		decayed = 1;
	}

	decayed |= __update_load_avg_cfs_rq(now, cfs_rq);
	u64_u32_store_copy(sa->آخری_update_وقت,
			   cfs_rq->آخری_update_وقت_copy,
			   sa->آخری_update_وقت);
	واپس decayed;
}

/**
 * attach_entity_load_avg - attach this entity to its cfs_rq load avg
 * @cfs_rq: cfs_rq to attach to
 * @se: sched_entity to attach
 *
 * Must call update_cfs_rq_load_avg() before this, since we rely on
 * cfs_rq->avg.last_update_time being current.
 */
ساکن خالی attach_entity_بھرو_avg(ساخت cfs_rq *cfs_rq, ساخت sched_entity *se)
{
	/*
	 * cfs_rq->avg.period_contrib can be used for both cfs_rq and se.
	 * See ___update_load_avg() for details.
	 */
	u32 divider = get_pelt_divider(&cfs_rq->avg);

	/*
	 * When we attach the @se to the @cfs_rq, we must align the decay
	 * window because without that, really weird and wonderful things can
	 * happen.
	 *
	 * XXX illustrate
	 */
	se->avg.آخری_update_وقت = cfs_rq->avg.آخری_update_وقت;
	se->avg.period_contrib = cfs_rq->avg.period_contrib;

	/*
	 * Hell(o) Nasty stuff.. we need to recompute _sum based on the new
	 * period_contrib. This isn't strictly correct, but since we're
	 * entirely outside of the PELT hierarchy, nobody cares if we truncate
	 * _sum a little.
	 */
	se->avg.util_مجموع = se->avg.util_avg * divider;

	se->avg.runnable_مجموع = se->avg.runnable_avg * divider;

	se->avg.بھرو_مجموع = se->avg.بھرو_avg * divider;
	اگر (se_وزن(se) < se->avg.بھرو_مجموع)
		se->avg.بھرو_مجموع = div_u64(se->avg.بھرو_مجموع, se_وزن(se));
	ورنہ
		se->avg.بھرو_مجموع = 1;

	قطار_میں_ڈالو_بھرو_avg(cfs_rq, se);
	cfs_rq->avg.util_avg += se->avg.util_avg;
	cfs_rq->avg.util_مجموع += se->avg.util_مجموع;
	cfs_rq->avg.runnable_avg += se->avg.runnable_avg;
	cfs_rq->avg.runnable_مجموع += se->avg.runnable_مجموع;

	جوڑو_tg_cfs_propagate(cfs_rq, se->avg.بھرو_مجموع);

	cfs_rq_util_change(cfs_rq, 0);

	trace_pelt_cfs_tp(cfs_rq);
}

/**
 * detach_entity_load_avg - detach this entity from its cfs_rq load avg
 * @cfs_rq: cfs_rq to detach from
 * @se: sched_entity to detach
 *
 * Must call update_cfs_rq_load_avg() before this, since we rely on
 * cfs_rq->avg.last_update_time being current.
 */
ساکن خالی detach_entity_بھرو_avg(ساخت cfs_rq *cfs_rq, ساخت sched_entity *se)
{
	قطار_سے_نکالو_بھرو_avg(cfs_rq, se);
	__update_sa(&cfs_rq->avg, util, -se->avg.util_avg, -se->avg.util_مجموع);
	__update_sa(&cfs_rq->avg, runnable, -se->avg.runnable_avg, -se->avg.runnable_مجموع);

	جوڑو_tg_cfs_propagate(cfs_rq, -se->avg.بھرو_مجموع);

	cfs_rq_util_change(cfs_rq, 0);

	trace_pelt_cfs_tp(cfs_rq);
}

#تعریف_کرو UTIL_EST_MARGIN (SCHED_استعداد_SCALE / 100)

ساکن اندرونی خالی util_est_update(ساخت sched_entity *se)
{
	غیرعلامتی صحیح ewma, dequeued, آخری_ewma_diff;

	اگر (!sched_feat(UTIL_EST))
		واپس;

	/* Get current estimate of utilization */
	ewma = READ_ONCE(se->avg.util_est);

	/*
	 * If the PELT values haven't changed since enqueue time,
	 * skip the util_est update.
	 */
	اگر (ewma & UTIL_AVG_UNCHANGED)
		واپس;

	/* Get utilization at dequeue */
	dequeued = READ_ONCE(se->avg.util_avg);

	/*
	 * Reset EWMA on utilization increases, the moving average is used only
	 * to smooth utilization decreases.
	 */
	اگر (ewma <= dequeued) {
		ewma = dequeued;
		جاؤ مکمل;
	}

	/*
	 * Skip update of task's estimated utilization when its members are
	 * already ~1% close to its last activation value.
	 */
	آخری_ewma_diff = ewma - dequeued;
	اگر (آخری_ewma_diff < UTIL_EST_MARGIN)
		جاؤ مکمل;

	/*
	 * To avoid underestimate of task utilization, skip updates of EWMA if
	 * we cannot grant that thread got all CPU time it wanted.
	 */
	اگر ((dequeued + UTIL_EST_MARGIN) < READ_ONCE(se->avg.runnable_avg))
		جاؤ مکمل;

	/*
	 * Update Task's estimated utilization
	 *
	 * When *p completes an activation we can consolidate another sample
	 * of the task size. This is done by using this value to update the
	 * Exponential Weighted Moving Average (EWMA):
	 *
	 *  ewma(t) = w *  task_util(p) + (1-w) * ewma(t-1)
	 *          = w *  task_util(p) +         ewma(t-1)  - w * ewma(t-1)
	 *          = w * (task_util(p) -         ewma(t-1)) +     ewma(t-1)
	 *          = w * (      -last_ewma_diff           ) +     ewma(t-1)
	 *          = w * (-last_ewma_diff +  ewma(t-1) / w)
	 *
	 * Where 'w' is the weight of new samples, which is configured to be
	 * 0.25, thus making w=1/4 ( >>= UTIL_EST_WEIGHT_SHIFT)
	 */
	ewma <<= UTIL_EST_وزن_سرکاؤ;
	ewma  -= آخری_ewma_diff;
	ewma >>= UTIL_EST_وزن_سرکاؤ;
مکمل:
	ewma |= UTIL_AVG_UNCHANGED;
	WRITE_ONCE(se->avg.util_est, ewma);

	trace_sched_util_est_se_tp(se);
}

/*
 * Optional action to be done while updating the load average
 */
#تعریف_کرو UPDATE_TG	0x01
#تعریف_کرو SKIP_AGE_بھرو	0x02
#تعریف_کرو DO_ATTACH	0x04
#تعریف_کرو DO_DETACH	0x08
#تعریف_کرو UPDATE_UTIL_EST	0x10

/* Update task and its cfs_rq load average */
ساکن اندرونی خالی update_بھرو_avg(ساخت cfs_rq *cfs_rq, ساخت sched_entity *se, صحیح flags)
{
	u64 now = cfs_rq_clock_pelt(cfs_rq);
	صحیح decayed;

	/*
	 * Track task load average for carrying it to new CPU after migrated, and
	 * track group sched_entity load average for task_h_load calculation in migration
	 */
	اگر (se->avg.آخری_update_وقت && !(flags & SKIP_AGE_بھرو))
		__update_load_avg_se(now, cfs_rq, se);

	decayed  = update_cfs_rq_بھرو_avg(now, cfs_rq);
	decayed |= propagate_entity_بھرو_avg(se);

	اگر (!se->avg.آخری_update_وقت && (flags & DO_ATTACH)) {

		/*
		 * DO_ATTACH means we're here from enqueue_entity().
		 * !last_update_time means we've passed through
		 * migrate_task_rq_fair() indicating we migrated.
		 *
		 * IOW we're enqueueing a task on a new CPU.
		 */
		attach_entity_بھرو_avg(cfs_rq, se);
		update_tg_بھرو_avg(cfs_rq);

	} ورنہ اگر (flags & DO_DETACH) {
		/*
		 * DO_DETACH means we're here from dequeue_entity()
		 * and we are migrating task out of the CPU.
		 */
		detach_entity_بھرو_avg(cfs_rq, se);
		update_tg_بھرو_avg(cfs_rq);
	} ورنہ اگر (decayed) {
		cfs_rq_util_change(cfs_rq, 0);

		اگر (flags & UPDATE_TG)
			update_tg_بھرو_avg(cfs_rq);
	}

	اگر (flags & UPDATE_UTIL_EST)
		util_est_update(se);
}

/*
 * Synchronize entity load avg of dequeued entity without locking
 * the previous rq.
 */
ساکن خالی sync_entity_بھرو_avg(ساخت sched_entity *se)
{
	ساخت cfs_rq *cfs_rq = cfs_rq_کا(se);
	u64 آخری_update_وقت;

	آخری_update_وقت = cfs_rq_آخری_update_وقت(cfs_rq);
	__update_load_avg_blocked_se(آخری_update_وقت, se);
}

/*
 * Task first catches up with cfs_rq, and then subtract
 * itself from the cfs_rq (task must be off the queue now).
 */
ساکن خالی remove_entity_بھرو_avg(ساخت sched_entity *se)
{
	ساخت cfs_rq *cfs_rq = cfs_rq_کا(se);
	غیرعلامتی طویل flags;

	/*
	 * tasks cannot exit without having gone through wake_up_new_task() ->
	 * enqueue_task_fair() which will have added things to the cfs_rq,
	 * so we can remove unconditionally.
	 */

	sync_entity_بھرو_avg(se);

	raw_spin_تالا_irqsave(&cfs_rq->removed.تالا, flags);
	++cfs_rq->removed.nr;
	cfs_rq->removed.util_avg	+= se->avg.util_avg;
	cfs_rq->removed.بھرو_avg	+= se->avg.بھرو_avg;
	cfs_rq->removed.runnable_avg	+= se->avg.runnable_avg;
	raw_spin_unlock_irqrestore(&cfs_rq->removed.تالا, flags);
}

ساکن اندرونی غیرعلامتی طویل cfs_rq_runnable_avg(ساخت cfs_rq *cfs_rq)
{
	واپس cfs_rq->avg.runnable_avg;
}

ساکن اندرونی غیرعلامتی طویل cfs_rq_بھرو_avg(ساخت cfs_rq *cfs_rq)
{
	واپس cfs_rq->avg.بھرو_avg;
}

ساکن صحیح sched_balance_newidle(ساخت rq *this_rq, ساخت rq_flags *rf)
	__must_hold(__rq_lockp(this_rq));

ساکن اندرونی غیرعلامتی طویل task_util(ساخت task_struct *p)
{
	واپس READ_ONCE(p->se.avg.util_avg);
}

ساکن اندرونی غیرعلامتی طویل _task_util_est(ساخت task_struct *p)
{
	واپس READ_ONCE(p->se.avg.util_est) & ~UTIL_AVG_UNCHANGED;
}

ساکن اندرونی غیرعلامتی طویل task_util_est(ساخت task_struct *p)
{
	واپس max(task_util(p), _task_util_est(p));
}

ساکن اندرونی خالی util_est_قطار_میں_ڈالو(ساخت cfs_rq *cfs_rq,
				    ساخت task_struct *p)
{
	غیرعلامتی صحیح enqueued;

	اگر (!sched_feat(UTIL_EST))
		واپس;

	/* Update root cfs_rq's estimated utilization */
	enqueued  = cfs_rq->avg.util_est;
	enqueued += _task_util_est(p);
	WRITE_ONCE(cfs_rq->avg.util_est, enqueued);

	trace_sched_util_est_cfs_tp(cfs_rq);
}

ساکن اندرونی خالی util_est_قطار_سے_نکالو(ساخت cfs_rq *cfs_rq,
				    ساخت task_struct *p)
{
	غیرعلامتی صحیح enqueued;

	اگر (!sched_feat(UTIL_EST))
		واپس;

	/* Update root cfs_rq's estimated utilization */
	enqueued  = cfs_rq->avg.util_est;
	enqueued -= min_t(غیرعلامتی صحیح, enqueued, _task_util_est(p));
	WRITE_ONCE(cfs_rq->avg.util_est, enqueued);

	trace_sched_util_est_cfs_tp(cfs_rq);
}

ساکن اندرونی غیرعلامتی طویل get_actual_cpu_استعداد(صحیح cpu)
{
	غیرعلامتی طویل استعداد = arch_scale_cpu_استعداد(cpu);

	استعداد -= max(hw_بھرو_avg(cpu_rq(cpu)), cpufreq_get_pressure(cpu));

	واپس استعداد;
}

ساکن اندرونی صحیح util_سماتا_cpu(غیرعلامتی طویل util,
				غیرعلامتی طویل uclamp_min,
				غیرعلامتی طویل uclamp_max,
				صحیح cpu)
{
	غیرعلامتی طویل استعداد = استعداد_کا(cpu);
	غیرعلامتی طویل استعداد_orig;
	bool سماتا, uclamp_max_سماتا;

	/*
	 * Check if the real util fits without any uclamp boost/cap applied.
	 */
	سماتا = سماتا_استعداد(util, استعداد);

	اگر (!uclamp_is_مستعمل())
		واپس سماتا;

	/*
	 * We must use arch_scale_cpu_capacity() for comparing against uclamp_min and
	 * uclamp_max. We only care about capacity pressure (by using
	 * capacity_of()) for comparing against the real util.
	 *
	 * If a task is boosted to 1024 for example, we don't want a tiny
	 * pressure to skew the check whether it fits a CPU or not.
	 *
	 * Similarly if a task is capped to arch_scale_cpu_capacity(little_cpu), it
	 * should fit a little cpu even if there's some pressure.
	 *
	 * Only exception is for HW or cpufreq pressure since it has a direct impact
	 * on available OPP of the system.
	 *
	 * We honour it for uclamp_min only as a drop in performance level
	 * could result in not getting the requested minimum performance level.
	 *
	 * For uclamp_max, we can tolerate a drop in performance level as the
	 * goal is to cap the task. So it's okay if it's getting less.
	 */
	استعداد_orig = arch_scale_cpu_استعداد(cpu);

	/*
	 * We want to force a task to fit a cpu as implied by uclamp_max.
	 * But we do have some corner cases to cater for..
	 *
	 *
	 *                                 C=z
	 *   |                             ___
	 *   |                  C=y       |   |
	 *   |_ _ _ _ _ _ _ _ _ ___ _ _ _ | _ | _ _ _ _ _  uclamp_max
	 *   |      C=x        |   |      |   |
	 *   |      ___        |   |      |   |
	 *   |     |   |       |   |      |   |    (util somewhere in this region)
	 *   |     |   |       |   |      |   |
	 *   |     |   |       |   |      |   |
	 *   +----------------------------------------
	 *         CPU0        CPU1       CPU2
	 *
	 *   In the above example if a task is capped to a specific performance
	 *   point, y, then when:
	 *
	 *   * util = 80% of x then it does not fit on CPU0 and should migrate
	 *     to CPU1
	 *   * util = 80% of y then it is forced to fit on CPU1 to honour
	 *     uclamp_max request.
	 *
	 *   which is what we're enforcing here. A task always fits if
	 *   uclamp_max <= capacity_orig. But when uclamp_max > capacity_orig,
	 *   the normal upmigration rules should withhold still.
	 *
	 *   Only exception is when we are on max capacity, then we need to be
	 *   careful not to block overutilized state. This is so because:
	 *
	 *     1. There's no concept of capping at max_capacity! We can't go
	 *        beyond this performance level anyway.
	 *     2. The system is being saturated when we're operating near
	 *        max capacity, it doesn't make sense to block overutilized.
	 */
	uclamp_max_سماتا = (استعداد_orig == SCHED_استعداد_SCALE) && (uclamp_max == SCHED_استعداد_SCALE);
	uclamp_max_سماتا = !uclamp_max_سماتا && (uclamp_max <= استعداد_orig);
	سماتا = سماتا || uclamp_max_سماتا;

	/*
	 *
	 *                                 C=z
	 *   |                             ___       (region a, capped, util >= uclamp_max)
	 *   |                  C=y       |   |
	 *   |_ _ _ _ _ _ _ _ _ ___ _ _ _ | _ | _ _ _ _ _ uclamp_max
	 *   |      C=x        |   |      |   |
	 *   |      ___        |   |      |   |      (region b, uclamp_min <= util <= uclamp_max)
	 *   |_ _ _|_ _|_ _ _ _| _ | _ _ _| _ | _ _ _ _ _ uclamp_min
	 *   |     |   |       |   |      |   |
	 *   |     |   |       |   |      |   |      (region c, boosted, util < uclamp_min)
	 *   +----------------------------------------
	 *         CPU0        CPU1       CPU2
	 *
	 * a) If util > uclamp_max, then we're capped, we don't care about
	 *    actual fitness value here. We only care if uclamp_max fits
	 *    capacity without taking margin/pressure into account.
	 *    See comment above.
	 *
	 * b) If uclamp_min <= util <= uclamp_max, then the normal
	 *    fits_capacity() rules apply. Except we need to ensure that we
	 *    enforce we remain within uclamp_max, see comment above.
	 *
	 * c) If util < uclamp_min, then we are boosted. Same as (b) but we
	 *    need to take into account the boosted value fits the CPU without
	 *    taking margin/pressure into account.
	 *
	 * Cases (a) and (b) are handled in the 'fits' variable already. We
	 * just need to consider an extra check for case (c) after ensuring we
	 * handle the case uclamp_min > uclamp_max.
	 */
	uclamp_min = min(uclamp_min, uclamp_max);
	اگر (سماتا && (util < uclamp_min) &&
	    (uclamp_min > get_actual_cpu_استعداد(cpu)))
		واپس -1;

	واپس سماتا;
}

ساکن اندرونی صحیح task_سماتا_cpu(ساخت task_struct *p, صحیح cpu)
{
	غیرعلامتی طویل uclamp_min = uclamp_eff_قدر(p, UCLAMP_MIN);
	غیرعلامتی طویل uclamp_max = uclamp_eff_قدر(p, UCLAMP_MAX);
	غیرعلامتی طویل util = task_util_est(p);
	/*
	 * Return true only if the cpu fully fits the task requirements, which
	 * include the utilization but also the performance hints.
	 */
	واپس (util_سماتا_cpu(util, uclamp_min, uclamp_max, cpu) > 0);
}

ساکن اندرونی خالی update_misfit_حالت(ساخت task_struct *p, ساخت rq *rq)
{
	صحیح cpu = cpu_کا(rq);

	اگر (!sched_asym_cpucap_فعال())
		واپس;

	/*
	 * Affinity allows us to go somewhere higher?  Or are we on biggest
	 * available CPU already? Or do we fit into this CPU ?
	 */
	اگر (!p || (p->nr_cpus_allowed == 1) ||
	    (arch_scale_cpu_استعداد(cpu) == p->max_allowed_استعداد) ||
	    task_سماتا_cpu(p, cpu)) {

		rq->misfit_task_بھرو = 0;
		واپس;
	}

	/*
	 * Make sure that misfit_task_load will not be null even if
	 * task_h_load() returns 0.
	 */
	rq->misfit_task_بھرو = max_t(غیرعلامتی طویل, task_h_بھرو(p), 1);
}

خالی __setparam_fair(ساخت task_struct *p, ثابت ساخت sched_attr *attr)
{
	ساخت sched_entity *se = &p->se;

	p->static_prio = NICE_مقصد_PRIO(attr->sched_nice);
	اگر (attr->sched_runtime) {
		se->custom_قاش = 1;
		se->قاش = clamp_t(u64, attr->sched_runtime,
				      NSEC_فی_MSEC/10,   /* HZ=1000 * 10 */
				      NSEC_فی_MSEC*100); /* HZ=100  / 10 */
	} ورنہ {
		se->custom_قاش = 0;
		se->قاش = sysctl_sched_بنیاد_قاش;
	}
}

ساکن خالی
رکھو_entity(ساخت cfs_rq *cfs_rq, ساخت sched_entity *se, صحیح flags)
{
	u64 vslice, vruntime = avg_vruntime(cfs_rq);
	غیرعلامتی صحیح nr_queued = cfs_rq->h_nr_queued;
	bool update_zero = false;
	s64 lag = 0;

	اگر (!se->custom_قاش)
		se->قاش = sysctl_sched_بنیاد_قاش;
	vslice = calc_delta_fair(se->قاش, se);

	اگر (flags & قطار_میں_ڈالو_QUEUED)
		nr_queued -= 1;

	/*
	 * Due to how V is constructed as the weighted average of entities,
	 * adding tasks with positive lag, or removing tasks with negative lag
	 * will move 'time' backwards, this can screw around with the lag of
	 * other tasks.
	 *
	 * EEVDF: placement strategy #1 / #2
	 */
	اگر (sched_feat(رکھو_LAG) && nr_queued && se->vlag) {
		ساخت sched_entity *curr = cfs_rq->curr;
		طویل بھرو, وزن;

		lag = se->vlag;

		/*
		 * If we want to place a task and preserve lag, we have to
		 * consider the effect of the new entity on the weighted
		 * average and compensate for this, otherwise lag can quickly
		 * evaporate.
		 *
		 * Lag is defined as:
		 *
		 *   lag_i = S - s_i = w_i * (V - v_i)
		 *
		 * To avoid the 'w_i' term all over the place, we only track
		 * the virtual lag:
		 *
		 *   vl_i = V - v_i <=> v_i = V - vl_i
		 *
		 * And we take V to be the weighted average of all v:
		 *
		 *   V = (\Sum w_j*v_j) / W
		 *
		 * Where W is: \Sum w_j
		 *
		 * Then, the weighted average after adding an entity with lag
		 * vl_i is given by:
		 *
		 *   V' = (\Sum w_j*v_j + w_i*v_i) / (W + w_i)
		 *      = (W*V + w_i*(V - vl_i)) / (W + w_i)
		 *      = (W*V + w_i*V - w_i*vl_i) / (W + w_i)
		 *      = (V*(W + w_i) - w_i*vl_i) / (W + w_i)
		 *      = V - w_i*vl_i / (W + w_i)
		 *
		 * And the actual lag after adding an entity with vl_i is:
		 *
		 *   vl'_i = V' - v_i
		 *         = V - w_i*vl_i / (W + w_i) - (V - vl_i)
		 *         = vl_i - w_i*vl_i / (W + w_i)
		 *
		 * Which is strictly less than vl_i. So in order to preserve lag
		 * we should inflate the lag before placement such that the
		 * effective lag after placement comes out right.
		 *
		 * As such, invert the above relation for vl'_i to get the vl_i
		 * we need to use such that the lag after placement is the lag
		 * we computed before dequeue.
		 *
		 *   vl'_i = vl_i - w_i*vl_i / (W + w_i)
		 *         = ((W + w_i)*vl_i - w_i*vl_i) / (W + w_i)
		 *
		 *   (W + w_i)*vl'_i = (W + w_i)*vl_i - w_i*vl_i
		 *                   = W*vl_i
		 *
		 *   vl_i = (W + w_i)*vl'_i / W
		 */
		بھرو = cfs_rq->مجموع_وزن;
		اگر (curr && curr->on_rq)
			بھرو += avg_vruntime_وزن(cfs_rq, curr->h_بھرو.وزن);
		اگر (WARN_ON_ONCE(!بھرو))
			بھرو = 1;

		وزن = avg_vruntime_وزن(cfs_rq, se->h_بھرو.وزن);

		/*
		 * Rewrite, because the product (W + w_i)*vl'_i might
		 * overflow s64:
		 *
		 *   vl_i = (W + w_i)*vl'_i / W
		 *        = (W*vl'_i + w_i*vl'_i) / W
		 *        = vl'_i + w_i*vl'_i/W
		 */
		lag += div64_long(lag * وزن, بھرو);

		/*
		 * A heavy entity (relative to the tree) will pull the
		 * avg_vruntime close to its vruntime position on enqueue. But
		 * the zero_vruntime point is only updated at the next
		 * update_deadline()/place_entity()/update_entity_lag().
		 *
		 * Specifically (see the comment near avg_vruntime_weight()):
		 *
		 *   sum_w_vruntime = \Sum (v_i - v0) * w_i
		 *
		 * Note that if v0 is near a light entity, both terms will be
		 * small for the light entity, while in that case both terms
		 * are large for the heavy entity, leading to risk of
		 * overflow.
		 *
		 * OTOH if v0 is near the heavy entity, then the difference is
		 * larger for the light entity, but the factor is small, while
		 * for the heavy entity the difference is small but the factor
		 * is large. Avoiding the multiplication overflow.
		 */
		اگر (وزن > بھرو)
			update_zero = true;
	}

	se->vruntime = vruntime - lag;

	اگر (update_zero)
		update_zero_vruntime(cfs_rq, -lag);

	اگر (sched_feat(رکھو_REL_DEADLINE) && se->rel_deadline) {
		se->deadline += se->vruntime;
		se->rel_deadline = 0;
		واپس;
	}

	/*
	 * When joining the competition; the existing tasks will be,
	 * on average, halfway through their slice, as such start tasks
	 * off with half a slice to ease into the competition.
	 */
	اگر (sched_feat(رکھو_DEADLINE_INITIAL) && (flags & قطار_میں_ڈالو_INITIAL))
		vslice /= 2;

	/*
	 * EEVDF: vd_i = ve_i + r_i/w_i
	 */
	se->deadline = se->vruntime + vslice;
}

ساکن خالی check_قطار_میں_ڈالو_throttle(ساخت cfs_rq *cfs_rq);
ساکن اندرونی صحیح cfs_rq_throttled(ساخت cfs_rq *cfs_rq);

ساکن خالی
قطار_میں_ڈالو_entity(ساخت cfs_rq *cfs_rq, ساخت sched_entity *se, صحیح flags)
{
	/*
	 * When enqueuing a sched_entity, we must:
	 *   - Update loads to have both entity and cfs_rq synced with now.
	 *   - For group_entity, update its runnable_weight to reflect the new
	 *     h_nr_runnable of its group cfs_rq.
	 *   - For group_entity, update its weight to reflect the new share of
	 *     its group cfs_rq
	 *   - Add its new weight to cfs_rq->load.weight
	 */
	update_بھرو_avg(cfs_rq, se, UPDATE_TG | DO_ATTACH);
	se_update_runnable(se);
	/*
	 * XXX update_load_avg() above will have attached us to the pelt sum;
	 * but update_cfs_group() here will re-adjust the weight and have to
	 * undo/redo all that. Seems wasteful.
	 */
	update_cfs_گروہ(se);

	account_entity_قطار_میں_ڈالو(cfs_rq, se);

	/* Entity has migrated, no longer consider this task hot */
	اگر (flags & قطار_میں_ڈالو_MIGRATED)
		se->exec_آغاز = 0;

	check_schedstat_required();
	update_stats_قطار_میں_ڈالو_fair(cfs_rq, se, flags);
	se->on_rq = 1;

	اگر (cfs_rq->nr_queued == 1) {
		check_قطار_میں_ڈالو_throttle(cfs_rq);
		list_جوڑو_برگ_cfs_rq(cfs_rq);
#اگر_تعریف_شدہ CONFIG_CFS_BANDWIDTH
		اگر (cfs_rq->pelt_clock_throttled) {
			ساخت rq *rq = rq_کا(cfs_rq);

			cfs_rq->throttled_clock_pelt_وقت += rq_clock_pelt(rq) -
				cfs_rq->throttled_clock_pelt;
			cfs_rq->pelt_clock_throttled = 0;
		}
#اگر_ختم
	}
}

ساکن خالی سیٹ_اگلا_buddy(ساخت cfs_rq *cfs_rq, ساخت sched_entity *se)
{
	اگر (WARN_ON_ONCE(!se->on_rq || se->sched_delayed))
		واپس;
	اگر (se_is_idle(se))
		واپس;
	cfs_rq->اگلا = se;
}

ساکن خالی clear_buddies(ساخت cfs_rq *cfs_rq, ساخت sched_entity *se)
{
	اگر (cfs_rq->اگلا == se)
		cfs_rq->اگلا = تہی;
}

ساکن __always_inline خالی return_cfs_rq_runtime(ساخت cfs_rq *cfs_rq);

ساکن خالی سیٹ_delayed(ساخت sched_entity *se)
{
	/*
	 * Delayed se of cfs_rq have no tasks queued on them.
	 * Do not adjust h_nr_runnable since __dequeue_task()
	 * will account it for blocked tasks.
	 *
	 * This check can be removed because when flat pick
	 * patches get merged as only task can get delayed,
	 * same for clear_delayed().
	 */
	اگر (!entity_is_task(se)) {
		se->sched_delayed = 1;
		واپس;
	}

	/*
	 * Drop a task leaving the runnable set.
	 * Needs to be called before sched_delayed is set.
	 * clear_delayed() mirrors this after clearing the flag.
	 */
	pref_llc_running_dec(rq_کا(cfs_rq_کا(se)), task_کا(se));
	se->sched_delayed = 1;

	for_each_sched_entity(se) {
		ساخت cfs_rq *cfs_rq = cfs_rq_کا(se);

		cfs_rq->h_nr_runnable--;
	}
}

ساکن خالی clear_delayed(ساخت sched_entity *se)
{
	se->sched_delayed = 0;

	/*
	 * Delayed se of cfs_rq have no tasks queued on them.
	 * Do not adjust h_nr_runnable since a dequeue has
	 * already accounted for it or an enqueue of a task
	 * below it will account for it in enqueue_task_fair().
	 */
	اگر (!entity_is_task(se))
		واپس;

	/*
	 * Re-add on wake, after sched_delayed is cleared. On a final delayed
	 * dequeue account_llc_dequeue() already cleared pref_llc_queued, so
	 * this does nothing.
	 */
	pref_llc_running_inc(rq_کا(cfs_rq_کا(se)), task_کا(se));

	for_each_sched_entity(se) {
		ساخت cfs_rq *cfs_rq = cfs_rq_کا(se);

		cfs_rq->h_nr_runnable++;
	}
}

ساکن خالی
قطار_سے_نکالو_entity(ساخت cfs_rq *cfs_rq, ساخت sched_entity *se, صحیح flags)
{
	صحیح action = UPDATE_TG;

	اگر (entity_is_task(se)) {
		اگر (task_on_rq_migrating(task_کا(se)))
			action |= DO_DETACH;

		اگر ((flags & قطار_سے_نکالو_SLEEP) && !(flags & قطار_سے_نکالو_DELAYED))
			action |= UPDATE_UTIL_EST;
	}

	/*
	 * When dequeuing a sched_entity, we must:
	 *   - Update loads to have both entity and cfs_rq synced with now.
	 *   - For group_entity, update its runnable_weight to reflect the new
	 *     h_nr_runnable of its group cfs_rq.
	 *   - Subtract its previous weight from cfs_rq->load.weight.
	 *   - For group entity, update its weight to reflect the new share
	 *     of its group cfs_rq.
	 */
	update_بھرو_avg(cfs_rq, se, action);
	se_update_runnable(se);

	update_stats_قطار_سے_نکالو_fair(cfs_rq, se, flags);

	se->on_rq = 0;
	account_entity_قطار_سے_نکالو(cfs_rq, se);

	/* return excess runtime on last dequeue */
	return_cfs_rq_runtime(cfs_rq);

	update_cfs_گروہ(se);

	اگر (cfs_rq->nr_queued == 0) {
		update_idle_cfs_rq_clock_pelt(cfs_rq);
#اگر_تعریف_شدہ CONFIG_CFS_BANDWIDTH
		اگر (throttled_hierarchy(cfs_rq)) {
			ساخت rq *rq = rq_کا(cfs_rq);

			list_del_برگ_cfs_rq(cfs_rq);
			cfs_rq->throttled_clock_pelt = rq_clock_pelt(rq);
			cfs_rq->pelt_clock_throttled = 1;
		}
#اگر_ختم
	}
}

ساکن خالی
سیٹ_اگلا_entity(ساخت cfs_rq *cfs_rq, ساخت sched_entity *se)
{
	/* 'current' is not kept within the tree. */
	اگر (se->on_rq) {
		/*
		 * Any task has to be enqueued before it get to execute on
		 * a CPU. So account for the time it spent waiting on the
		 * runqueue.
		 */
		update_stats_wait_انتہا_fair(cfs_rq, se);
		update_بھرو_avg(cfs_rq, se, UPDATE_TG);
	}

	update_stats_curr_آغاز(cfs_rq, se);
	WARN_ON_ONCE(cfs_rq->h_curr);
	cfs_rq->h_curr = se;

	/*
	 * Track our maximum slice length, if the CPU's load is at
	 * least twice that of our own weight (i.e. don't track it
	 * when there are only lesser-weight tasks around):
	 */
	اگر (schedstat_enabled() &&
	    rq_کا(cfs_rq)->cfs.بھرو.وزن >= 2*se->بھرو.وزن) {
		ساخت sched_statistics *stats;

		stats = __schedstats_from_se(se);
		__schedstat_set(stats->قاش_max,
				max((u64)stats->قاش_max,
				    se->مجموع_exec_runtime - se->پچھلا_مجموع_exec_runtime));
	}

	se->پچھلا_مجموع_exec_runtime = se->مجموع_exec_runtime;
}

ساکن bool __dequeue_task(ساخت rq *rq, ساخت task_struct *p, صحیح flags);

ساکن ساخت sched_entity *
pick_اگلا_entity(ساخت rq *rq, bool protect)
{
	ساخت cfs_rq *cfs_rq = &rq->cfs;
	ساخت sched_entity *se;

	se = pick_eevdf(cfs_rq, protect);
	اگر (se->sched_delayed) {
		__dequeue_task(rq, task_کا(se), قطار_سے_نکالو_SLEEP | قطار_سے_نکالو_DELAYED);
		/*
		 * Must not reference @se again, see __block_task().
		 */
		واپس تہی;
	}
	واپس se;
}

ساکن خالی put_پچھلا_entity(ساخت cfs_rq *cfs_rq, ساخت sched_entity *پچھلا)
{
	/*
	 * If still on the runqueue then deactivate_task()
	 * was not called and update_curr() has to be done:
	 */
	اگر (پچھلا->on_rq)
		update_curr(cfs_rq);

	اگر (پچھلا->on_rq) {
		update_stats_wait_آغاز_fair(cfs_rq, پچھلا);
		/* in !on_rq case, update occurred at dequeue */
		update_بھرو_avg(cfs_rq, پچھلا, 0);
	}
	WARN_ON_ONCE(cfs_rq->h_curr != پچھلا);
	cfs_rq->h_curr = تہی;
}

ساکن خالی
entity_tick(ساخت cfs_rq *cfs_rq, ساخت sched_entity *curr, صحیح queued)
{
	/*
	 * Update run-time statistics of the 'current'.
	 */
	update_curr(cfs_rq);

	/*
	 * Ensure that runnable average is periodically updated.
	 */
	update_بھرو_avg(cfs_rq, curr, UPDATE_TG);
	update_cfs_گروہ(curr);

#اگر_تعریف_شدہ CONFIG_SCHED_HRTICK
	/*
	 * queued ticks are scheduled to match the slice, so don't bother
	 * validating it and just reschedule.
	 */
	اگر (queued) {
		resched_curr(rq_کا(cfs_rq));
		واپس;
	}
#اگر_ختم
}


/**************************************************
 * CFS bandwidth control machinery
 */

#اگر_تعریف_شدہ CONFIG_CFS_BANDWIDTH

#اگر_تعریف_شدہ CONFIG_JUMP_LABEL
ساکن ساخت static_کلید __cfs_bandwidth_used;

ساکن اندرونی bool cfs_bandwidth_مستعمل(خالی)
{
	واپس static_کلید_false(&__cfs_bandwidth_used);
}

خالی cfs_bandwidth_usage_inc(خالی)
{
	static_کلید_slow_inc_cpuslocked(&__cfs_bandwidth_used);
}

خالی cfs_bandwidth_usage_dec(خالی)
{
	static_کلید_slow_dec_cpuslocked(&__cfs_bandwidth_used);
}
#ورنہ /* !CONFIG_JUMP_LABEL: */
ساکن bool cfs_bandwidth_مستعمل(خالی)
{
	واپس true;
}

خالی cfs_bandwidth_usage_inc(خالی) {}
خالی cfs_bandwidth_usage_dec(خالی) {}
#اگر_ختم /* !CONFIG_JUMP_LABEL */

ساکن اندرونی u64 sched_cfs_bandwidth_قاش(خالی)
{
	واپس (u64)sysctl_sched_cfs_bandwidth_قاش * NSEC_فی_USEC;
}

/*
 * Replenish runtime according to assigned quota. We use sched_clock_cpu
 * directly instead of rq->clock to avoid adding additional synchronization
 * around rq->lock.
 *
 * requires cfs_b->lock
 */
خالی __refill_cfs_bandwidth_runtime(ساخت cfs_bandwidth *cfs_b)
{
	s64 runtime;

	اگر (unlikely(cfs_b->quota == RUNTIME_INF))
		واپس;

	cfs_b->runtime += cfs_b->quota;
	runtime = cfs_b->runtime_snap - cfs_b->runtime;
	اگر (runtime > 0) {
		cfs_b->مدت_وقت += runtime;
		cfs_b->nr_مدت++;
	}

	cfs_b->runtime = min(cfs_b->runtime, cfs_b->quota + cfs_b->مدت);
	cfs_b->runtime_snap = cfs_b->runtime;
}

ساکن اندرونی ساخت cfs_bandwidth *tg_cfs_bandwidth(ساخت task_گروہ *tg)
{
	واپس &tg->cfs_bandwidth;
}

/* returns 0 on failure to allocate runtime */
ساکن صحیح __assign_cfs_rq_runtime(ساخت cfs_bandwidth *cfs_b,
				   ساخت cfs_rq *cfs_rq, u64 ہدف_runtime)
{
	u64 min_amount, amount = 0;

	lockdep_assert_held(&cfs_b->تالا);

	/* note: this is a positive sum as runtime_remaining <= 0 */
	min_amount = ہدف_runtime - cfs_rq->runtime_بقایا;

	اگر (cfs_b->quota == RUNTIME_INF)
		amount = min_amount;
	ورنہ {
		آغاز_cfs_bandwidth(cfs_b);

		اگر (cfs_b->runtime > 0) {
			amount = min(cfs_b->runtime, min_amount);
			cfs_b->runtime -= amount;
			cfs_b->idle = 0;
		}
	}

	cfs_rq->runtime_بقایا += amount;

	واپس cfs_rq->runtime_بقایا > 0;
}

ساکن bool throttle_cfs_rq(ساخت cfs_rq *cfs_rq);

ساکن bool __account_cfs_rq_runtime(ساخت cfs_rq *cfs_rq, u64 delta_exec)
{
	/* dock delta_exec before expiring quota (as it could span periods) */
	cfs_rq->runtime_بقایا -= delta_exec;

	اگر (likely(cfs_rq->runtime_بقایا > 0))
		واپس false;

	اگر (cfs_rq->throttled)
		واپس true;
	/*
	 * throttle_cfs_rq() will try to extend the runtime first
	 * before throttling the hierarchy.
	 */
	واپس throttle_cfs_rq(cfs_rq);
}

ساکن __always_inline
bool account_cfs_rq_runtime(ساخت cfs_rq *cfs_rq, u64 delta_exec)
{
	اگر (!cfs_bandwidth_مستعمل() || !cfs_rq->runtime_enabled)
		واپس false;

	واپس __account_cfs_rq_runtime(cfs_rq, delta_exec);
}

ساکن اندرونی صحیح cfs_rq_throttled(ساخت cfs_rq *cfs_rq)
{
	واپس cfs_bandwidth_مستعمل() && cfs_rq->throttled;
}

ساکن اندرونی bool cfs_rq_pelt_clock_throttled(ساخت cfs_rq *cfs_rq)
{
	واپس cfs_bandwidth_مستعمل() && cfs_rq->pelt_clock_throttled;
}

/* check whether cfs_rq, or any parent, is throttled */
ساکن اندرونی صحیح throttled_hierarchy(ساخت cfs_rq *cfs_rq)
{
	واپس cfs_bandwidth_مستعمل() && cfs_rq->throttle_گنتی;
}

ساکن اندرونی صحیح lb_throttled_hierarchy(ساخت task_struct *p, صحیح dst_cpu)
{
	واپس throttled_hierarchy(tg_cfs_rq(task_گروہ(p), dst_cpu));
}

ساکن اندرونی bool task_is_throttled(ساخت task_struct *p)
{
	واپس cfs_bandwidth_مستعمل() && p->throttled;
}

ساکن bool قطار_سے_نکالو_task_fair(ساخت rq *rq, ساخت task_struct *p, صحیح flags);
ساکن خالی throttle_cfs_rq_work(ساخت callback_سر *work)
{
	ساخت task_struct *p = container_کا(work, ساخت task_struct, sched_throttle_work);
	ساخت sched_entity *se;
	ساخت cfs_rq *cfs_rq;
	ساخت rq *rq;

	WARN_ON_ONCE(p != current);
	p->sched_throttle_work.اگلا = &p->sched_throttle_work;

	/*
	 * If task is exiting, then there won't be a return to userspace, so we
	 * don't have to bother with any of this.
	 */
	اگر ((p->flags & PF_EXITING))
		واپس;

	scoped_guard(task_rq_تالا_2, p) {
		se = &p->se;
		cfs_rq = cfs_rq_کا(se);

		/* Raced, forget */
		اگر (p->sched_class != &fair_sched_class)
			واپس;

		/*
		 * If not in limbo, then either replenish has happened or this
		 * task got migrated out of the throttled cfs_rq, move along.
		 */
		اگر (!cfs_rq->throttle_گنتی)
			واپس;
		rq = scope.rq;
		update_rq_clock(rq);
		WARN_ON_ONCE(p->throttled || !list_خالی_خانے(&p->throttle_گرہ));
		قطار_سے_نکالو_task_fair(rq, p, قطار_سے_نکالو_SLEEP | قطار_سے_نکالو_THROTTLE);
		list_جوڑو(&p->throttle_گرہ, &cfs_rq->throttled_limbo_list);
		/*
		 * Must not set throttled before dequeue or dequeue will
		 * mistakenly regard this task as an already throttled one.
		 */
		p->throttled = true;
		resched_curr(rq);
	}
}

خالی init_cfs_throttle_work(ساخت task_struct *p)
{
	init_task_work(&p->sched_throttle_work, throttle_cfs_rq_work);
	/* Protect against double add, see throttle_cfs_rq() and throttle_cfs_rq_work() */
	p->sched_throttle_work.اگلا = &p->sched_throttle_work;
	INIT_LIST_سر(&p->throttle_گرہ);
}

/*
 * Task is throttled and someone wants to dequeue it again:
 * it could be sched/core when core needs to do things like
 * task affinity change, task group change, task sched class
 * change etc. and in these cases, DEQUEUE_SLEEP is not set;
 * or the task is blocked after throttled due to freezer etc.
 * and in these cases, DEQUEUE_SLEEP is set.
 */
ساکن خالی detach_task_cfs_rq(ساخت task_struct *p);
ساکن خالی قطار_سے_نکالو_throttled_task(ساخت task_struct *p, صحیح flags)
{
	WARN_ON_ONCE(p->se.on_rq);
	list_del_init(&p->throttle_گرہ);

	/* task blocked after throttled */
	اگر (flags & قطار_سے_نکالو_SLEEP) {
		p->throttled = false;
		واپس;
	}

	/*
	 * task is migrating off its old cfs_rq, detach
	 * the task's load from its old cfs_rq.
	 */
	اگر (task_on_rq_migrating(p))
		detach_task_cfs_rq(p);
}

ساکن bool قطار_میں_ڈالو_throttled_task(ساخت task_struct *p)
{
	ساخت cfs_rq *cfs_rq = cfs_rq_کا(&p->se);

	/* @p should have gone through dequeue_throttled_task() first */
	WARN_ON_ONCE(!list_خالی_خانے(&p->throttle_گرہ));

	/*
	 * If the throttled task @p is enqueued to a throttled cfs_rq,
	 * take the fast path by directly putting the task on the
	 * target cfs_rq's limbo list.
	 *
	 * Do not do that when @p is current because the following race can
	 * cause @p's group_node to be incorectly re-insterted in its rq's
	 * cfs_tasks list, despite being throttled:
	 *
	 *     cpuX                       cpuY
	 *   p ret2user
	 *  throttle_cfs_rq_work()  sched_move_task(p)
	 *  LOCK task_rq_lock
	 *  dequeue_task_fair(p)
	 *  UNLOCK task_rq_lock
	 *                          LOCK task_rq_lock
	 *                          task_current_donor(p) == true
	 *                          task_on_rq_queued(p) == true
	 *                          dequeue_task(p)
	 *                          put_prev_task(p)
	 *                          sched_change_group()
	 *                          enqueue_task(p) -> p's new cfs_rq
	 *                                             is throttled, go
	 *                                             fast path and skip
	 *                                             actual enqueue
	 *                          set_next_task(p)
	 *                    list_move(&se->group_node, &rq->cfs_tasks); // bug
	 *  schedule()
	 *
	 * In the above race case, @p current cfs_rq is in the same rq as
	 * its previous cfs_rq because sched_move_task() only moves a task
	 * to a different group from the same rq, so we can use its current
	 * cfs_rq to derive rq and test if the task is current.
	 */
	اگر (throttled_hierarchy(cfs_rq) &&
	    !task_current_donor(rq_کا(cfs_rq), p)) {
		list_جوڑو(&p->throttle_گرہ, &cfs_rq->throttled_limbo_list);
		واپس true;
	}

	/* we can't take the fast path, do an actual enqueue*/
	p->throttled = false;
	واپس false;
}

ساکن خالی قطار_میں_ڈالو_task_fair(ساخت rq *rq, ساخت task_struct *p, صحیح flags);
ساکن صحیح tg_unthrottle_اوپر(ساخت task_گروہ *tg, خالی *ڈیٹا)
{
	ساخت rq *rq = ڈیٹا;
	ساخت cfs_rq *cfs_rq = tg_cfs_rq(tg, cpu_کا(rq));
	ساخت task_struct *p, *tmp;
	LIST_سر(throttled_tasks);

	/*
	 * If cfs_rq->curr is set, the cfs_rq might not have caught up
	 * since the last clock update. Do it now before we begin
	 * queueing task onto it to save the need for unnecessarily
	 * unthrottle the hierarchy for this cfs_rq to be throttled
	 * right back again.
	 */
	update_curr(cfs_rq);

	اگر (--cfs_rq->throttle_گنتی)
		واپس 0;

	اگر (cfs_rq->pelt_clock_throttled) {
		cfs_rq->throttled_clock_pelt_وقت += rq_clock_pelt(rq) -
					     cfs_rq->throttled_clock_pelt;
		cfs_rq->pelt_clock_throttled = 0;
	}

	اگر (cfs_rq->throttled_clock_self) {
		u64 delta = rq_clock(rq) - cfs_rq->throttled_clock_self;

		cfs_rq->throttled_clock_self = 0;

		اگر (WARN_ON_ONCE((s64)delta < 0))
			delta = 0;

		cfs_rq->throttled_clock_self_وقت += delta;
	}

	/*
	 * Move the tasks to a local list since an update_curr() during
	 * enqueue_task_fair() can throttle a higher cfs_rq, and it can
	 * see the "throttled_limbo_list" being non-empty in
	 * tg_throttle_down() if throttle_count turned 0 above.
	 */
	list_splice_init(&cfs_rq->throttled_limbo_list, &throttled_tasks);

	/* Re-enqueue the tasks that have been throttled at this level. */
	list_for_each_اندراج_محفوظ(p, tmp, &throttled_tasks, throttle_گرہ) {
		/*
		 * Back to being throttled! Break out and put the remaining
		 * tasks back onto the limbo_list to prevent running them
		 * unnecessarily.
		 */
		اگر (cfs_rq->throttle_گنتی)
			توڑو;

		list_del_init(&p->throttle_گرہ);
		p->throttled = false;
		قطار_میں_ڈالو_task_fair(rq, p, قطار_میں_ڈالو_WAKEUP);
	}

	list_splice(&throttled_tasks, &cfs_rq->throttled_limbo_list);

	/* Add cfs_rq with load or one or more already running entities to the list */
	اگر (!cfs_rq_is_decayed(cfs_rq))
		list_جوڑو_برگ_cfs_rq(cfs_rq);

	واپس 0;
}

ساکن اندرونی bool task_has_throttle_work(ساخت task_struct *p)
{
	واپس p->sched_throttle_work.اگلا != &p->sched_throttle_work;
}

ساکن اندرونی خالی task_throttle_تیاری_work(ساخت task_struct *p)
{
	اگر (task_has_throttle_work(p))
		واپس;

	/*
	 * Kthreads and exiting tasks don't return to userspace, so adding the
	 * work is pointless
	 */
	اگر ((p->flags & (PF_EXITING | PF_KTHREAD)))
		واپس;

	task_work_جوڑو(p, &p->sched_throttle_work, TWA_RESUME);
}

ساکن خالی ریکارڈ_throttle_clock(ساخت cfs_rq *cfs_rq)
{
	ساخت rq *rq = rq_کا(cfs_rq);

	اگر (cfs_rq_throttled(cfs_rq) && !cfs_rq->throttled_clock)
		cfs_rq->throttled_clock = rq_clock(rq);

	اگر (!cfs_rq->throttled_clock_self)
		cfs_rq->throttled_clock_self = rq_clock(rq);
}

ساکن صحیح tg_throttle_نیچے_کو(ساخت task_گروہ *tg, خالی *ڈیٹا)
{
	ساخت rq *rq = ڈیٹا;
	ساخت cfs_rq *cfs_rq = tg_cfs_rq(tg, cpu_کا(rq));

	اگر (cfs_rq->throttle_گنتی++)
		واپس 0;

	/*
	 * For cfs_rqs that still have entities enqueued, PELT clock
	 * stop happens at dequeue time when all entities are dequeued.
	 */
	اگر (!cfs_rq->nr_queued) {
		list_del_برگ_cfs_rq(cfs_rq);
		cfs_rq->throttled_clock_pelt = rq_clock_pelt(rq);
		cfs_rq->pelt_clock_throttled = 1;
	}

	WARN_ON_ONCE(cfs_rq->throttled_clock_self);
	WARN_ON_ONCE(!list_خالی_خانے(&cfs_rq->throttled_limbo_list));
	واپس 0;
}

ساکن bool throttle_cfs_rq(ساخت cfs_rq *cfs_rq)
{
	ساخت cfs_bandwidth *cfs_b = tg_cfs_bandwidth(cfs_rq->tg);
	ساخت sched_entity *curr = cfs_rq->h_curr;
	ساخت rq *rq = rq_کا(cfs_rq);

	scoped_guard(raw_spinlock, &cfs_b->تالا) {
		u64 ہدف_runtime = 1;

		/*
		 * If cfs_rq->h_curr is still runnable, we are here from an
		 * update_curr(). Request sysctl_sched_cfs_bandwidth_slice
		 * worth of bandwidth to continue running.
		 *
		 * If the curr is not runnable, just request enough bandwidth
		 * to be runnable next time the pick selects this cfs_rq.
		 */
		اگر (curr && curr->on_rq)
			ہدف_runtime = sched_cfs_bandwidth_قاش();

		/*
		 * Check if We have raced with bandwidth becoming available. If
		 * we actually throttled the timer might not unthrottle us for
		 * an entire period. We additionally needed to make sure that
		 * any subsequent check_cfs_rq_runtime calls agree not to
		 * throttle us, as we may commit to do cfs put_prev+pick_next,
		 * so we ask for 1ns of runtime rather than just check cfs_b.
		 *
		 * This will start the period timer if necessary.
		 */
		اگر (__assign_cfs_rq_runtime(cfs_b, cfs_rq, ہدف_runtime))
			واپس false;

		/*
		 * No bandwidth available; Add ourselves on the list to be
		 * unthrottled later.
		 */
		list_جوڑو_دم_rcu(&cfs_rq->throttled_list,
				  &cfs_b->throttled_cfs_rq);
	}

	/* freeze hierarchy runnable averages while throttled */
	scoped_guard(rcu)
		walk_tg_درخت_from(cfs_rq->tg, tg_throttle_نیچے_کو, tg_nop, (خالی *)rq);

	/*
	 * Note: distribution will already see us throttled via the
	 * throttled-list.  rq->lock protects completion.
	 */
	cfs_rq->throttled = 1;
	WARN_ON_ONCE(cfs_rq->throttled_clock);

	/*
	 * If current hierarchy was throttled, add throttle work to the
	 * current donor. In case of proxy-execution, the execution
	 * context cannot exit to the userspace while holding a mutex
	 * and the rule of throttle deferral to only throttle the
	 * throttled context at exit to userspace is still preserved.
	 */
	اگر (curr && curr->on_rq)
		task_throttle_تیاری_work(rq->donor);

	واپس true;
}

خالی unthrottle_cfs_rq(ساخت cfs_rq *cfs_rq)
{
	ساخت rq *rq = rq_کا(cfs_rq);
	ساخت cfs_bandwidth *cfs_b = tg_cfs_bandwidth(cfs_rq->tg);
	ساخت sched_entity *se = cfs_rq_se(cfs_rq);

	/*
	 * It's possible we are called with runtime_remaining < 0 due to things
	 * like async unthrottled us with a positive runtime_remaining but other
	 * still running entities consumed those runtime before we reached here.
	 *
	 * We can't unthrottle this cfs_rq without any runtime remaining because
	 * any enqueue in tg_unthrottle_up() will immediately trigger a throttle,
	 * which is not supposed to happen on unthrottle path.
	 *
	 * Catch up on the remaining runtime since last clock update before
	 * checking runtime remaining.
	 */
	update_curr(cfs_rq);
	اگر (cfs_rq->runtime_enabled && cfs_rq->runtime_بقایا <= 0)
		واپس;

	cfs_rq->throttled = 0;

	scoped_guard(raw_spinlock, &cfs_b->تالا) {
		list_del_rcu(&cfs_rq->throttled_list);

		اگر (!cfs_rq->throttled_clock)
			توڑو;

		cfs_b->throttled_وقت += rq_clock(rq) - cfs_rq->throttled_clock;
		cfs_rq->throttled_clock = 0;
	}

	/* update hierarchical throttle state */
	walk_tg_درخت_from(cfs_rq->tg, tg_nop, tg_unthrottle_اوپر, (خالی *)rq);

	اگر (!cfs_rq->بھرو.وزن) {
		اگر (!cfs_rq->on_list)
			واپس;
		/*
		 * Nothing to run but something to decay (on_list)?
		 * Complete the branch.
		 */
		for_each_sched_entity(se) {
			اگر (list_جوڑو_برگ_cfs_rq(cfs_rq_کا(se)))
				توڑو;
		}
	}

	assert_list_برگ_cfs_rq(rq);

	/* Determine whether we need to wake up potentially idle CPU: */
	اگر (rq->curr == rq->idle && rq->cfs.h_nr_queued)
		resched_curr(rq);
}

ساکن خالی __cfsb_csd_unthrottle(خالی *arg)
{
	ساخت cfs_rq *cursor, *tmp;
	ساخت rq *rq = arg;

	guard(rq_تالا)(rq);

	/*
	 * Iterating over the list can trigger several call to
	 * update_rq_clock() in unthrottle_cfs_rq().
	 * Do it once and skip the potential next ones.
	 */
	update_rq_clock(rq);
	rq_clock_آغاز_loop_update(rq);

	/*
	 * Since we hold rq lock we're safe from concurrent manipulation of
	 * the CSD list. However, this RCU critical section annotates the
	 * fact that we pair with sched_free_group_rcu(), so that we cannot
	 * race with group being freed in the window between removing it
	 * from the list and advancing to the next entry in the list.
	 */
	guard(rcu)();

	list_for_each_اندراج_محفوظ(cursor, tmp, &rq->cfsb_csd_list,
				 throttled_csd_list) {
		list_del_init(&cursor->throttled_csd_list);

		اگر (cfs_rq_throttled(cursor))
			unthrottle_cfs_rq(cursor);
	}

	rq_clock_stop_loop_update(rq);
}

ساکن اندرونی خالی __unthrottle_cfs_rq_async(ساخت cfs_rq *cfs_rq)
{
	ساخت rq *rq = rq_کا(cfs_rq);
	bool پہلا;

	اگر (rq == this_rq()) {
		update_rq_clock(rq);
		unthrottle_cfs_rq(cfs_rq);
		واپس;
	}

	/* Already enqueued */
	اگر (WARN_ON_ONCE(!list_خالی_خانے(&cfs_rq->throttled_csd_list)))
		واپس;

	پہلا = list_خالی_خانے(&rq->cfsb_csd_list);
	list_جوڑو_دم(&cfs_rq->throttled_csd_list, &rq->cfsb_csd_list);
	اگر (پہلا)
		smp_call_function_single_async(cpu_کا(rq), &rq->cfsb_csd);
}

ساکن خالی unthrottle_cfs_rq_async(ساخت cfs_rq *cfs_rq)
{
	lockdep_assert_rq_held(rq_کا(cfs_rq));

	اگر (WARN_ON_ONCE(!cfs_rq_throttled(cfs_rq) ||
	    cfs_rq->runtime_بقایا <= 0))
		واپس;

	__unthrottle_cfs_rq_async(cfs_rq);
}

ساکن bool distribute_cfs_runtime(ساخت cfs_bandwidth *cfs_b)
{
	bool throttled = false, unthrottle_local = false;
	صحیح this_cpu = smp_processor_id();
	u64 runtime, بقایا = 1;
	ساخت cfs_rq *cfs_rq;
	ساخت rq *rq;

	guard(rcu)();

	list_for_each_اندراج_rcu(cfs_rq, &cfs_b->throttled_cfs_rq,
				throttled_list) {
		rq = rq_کا(cfs_rq);

		اگر (!بقایا) {
			throttled = true;
			توڑو;
		}

		guard(rq_تالا_irqsave)(rq);

		اگر (!cfs_rq_throttled(cfs_rq))
			جاری;

		/* Already queued for async unthrottle */
		اگر (!list_خالی_خانے(&cfs_rq->throttled_csd_list))
			جاری;

		اگر (cfs_rq->h_curr) {
			update_rq_clock(rq);
			update_curr(cfs_rq);
		}

		/* By the above checks, this should never be true */
		WARN_ON_ONCE(cfs_rq->runtime_بقایا > 0);

		scoped_guard(raw_spinlock, &cfs_b->تالا) {
			runtime = -cfs_rq->runtime_بقایا + 1;
			اگر (runtime > cfs_b->runtime)
				runtime = cfs_b->runtime;
			cfs_b->runtime -= runtime;
			بقایا = cfs_b->runtime;
		}

		cfs_rq->runtime_بقایا += runtime;

		/*
		 * Ran out of bandwidth during distribution!
		 * Indicate throttled entities and break early.
		 */
		اگر (cfs_rq->runtime_بقایا <= 0) {
			throttled = true;
			توڑو;
		}

		/* we check whether we're throttled above */
		اگر (cpu_کا(rq) != this_cpu) {
			unthrottle_cfs_rq_async(cfs_rq);
			جاری;
		}

		/*
		 * Allow a parallel async unthrottle to unthrottle
		 * this cfs_rq too via __cfsb_csd_unthrottle().
		 * If we are first, do it ourselves at the end and
		 * save on an IPI from remote CPUs.
		 */
		unthrottle_local = list_خالی_خانے(&rq->cfsb_csd_list);
		list_جوڑو_دم(&cfs_rq->throttled_csd_list, &rq->cfsb_csd_list);
	}

	اگر (unthrottle_local) {
		/*
		 * Protect against an IPI that is also trying to flush
		 * the unthrottled cfs_rq(s) from this CPU's csd_list.
		 */
		scoped_guard(irqsave)
			__cfsb_csd_unthrottle(cpu_rq(this_cpu));
	}

	واپس throttled;
}

/*
 * Responsible for refilling a task_group's bandwidth and unthrottling its
 * cfs_rqs as appropriate. If there has been no activity within the last
 * period the timer is deactivated until scheduling resumes; cfs_b->idle is
 * used to track this state.
 */
ساکن صحیح do_sched_cfs_period_timer(ساخت cfs_bandwidth *cfs_b, صحیح overrun, غیرعلامتی طویل flags)
	__must_hold(&cfs_b->تالا)
{
	صحیح throttled;

	/* no need to continue the timer with no bandwidth constraint */
	اگر (cfs_b->quota == RUNTIME_INF)
		جاؤ برآمد_deactivate;

	throttled = !list_خالی_خانے(&cfs_b->throttled_cfs_rq);
	cfs_b->nr_periods += overrun;

	/* Refill extra burst quota even if cfs_b->idle */
	__refill_cfs_bandwidth_runtime(cfs_b);

	/*
	 * idle depends on !throttled (for the case of a large deficit), and if
	 * we're going inactive then everything else can be deferred
	 */
	اگر (cfs_b->idle && !throttled)
		جاؤ برآمد_deactivate;

	اگر (!throttled) {
		/* mark as potentially idle for the upcoming period */
		cfs_b->idle = 1;
		واپس 0;
	}

	/* account preceding periods in which throttling occurred */
	cfs_b->nr_throttled += overrun;

	/*
	 * This check is repeated as we release cfs_b->lock while we unthrottle.
	 */
	جب (throttled && cfs_b->runtime > 0) {
		raw_spin_unlock_irqrestore(&cfs_b->تالا, flags);
		/* we can't nest cfs_b->lock while distributing bandwidth */
		throttled = distribute_cfs_runtime(cfs_b);
		raw_spin_تالا_irqsave(&cfs_b->تالا, flags);
	}

	/*
	 * While we are ensured activity in the period following an
	 * unthrottle, this also covers the case in which the new bandwidth is
	 * insufficient to cover the existing bandwidth deficit.  (Forcing the
	 * timer to remain active while there are any throttled entities.)
	 */
	cfs_b->idle = 0;

	واپس 0;

برآمد_deactivate:
	واپس 1;
}

/* a cfs_rq won't donate quota below this amount */
ساکن ثابت u64 min_cfs_rq_runtime = 1 * NSEC_فی_MSEC;
/* minimum remaining period time to redistribute slack quota */
ساکن ثابت u64 min_bandwidth_expiration = 2 * NSEC_فی_MSEC;
/* how long we wait to gather additional slack before distributing */
ساکن ثابت u64 cfs_bandwidth_slack_period = 5 * NSEC_فی_MSEC;

/*
 * Are we near the end of the current quota period?
 *
 * Requires cfs_b->lock for hrtimer_expires_remaining to be safe against the
 * hrtimer base being cleared by hrtimer_start. In the case of
 * migrate_hrtimers, base is never cleared, so we are fine.
 */
ساکن صحیح runtime_refresh_within(ساخت cfs_bandwidth *cfs_b, u64 min_expire)
{
	ساخت hrtimer *refresh_timer = &cfs_b->period_timer;
	s64 بقایا;

	/* if the call-back is running a quota refresh is already occurring */
	اگر (hrtimer_callback_running(refresh_timer))
		واپس 1;

	/* is a quota refresh about to occur? */
	بقایا = ktime_مقصد_ns(hrtimer_expires_بقایا(refresh_timer));
	اگر (بقایا < (s64)min_expire)
		واپس 1;

	واپس 0;
}

ساکن خالی آغاز_cfs_slack_bandwidth(ساخت cfs_bandwidth *cfs_b)
{
	u64 min_بایاں = cfs_bandwidth_slack_period + min_bandwidth_expiration;

	/* if there's a quota refresh soon don't bother with slack */
	اگر (runtime_refresh_within(cfs_b, min_بایاں))
		واپس;

	/* don't push forwards an existing deferred unthrottle */
	اگر (cfs_b->slack_started)
		واپس;
	cfs_b->slack_started = true;

	hrtimer_آغاز(&cfs_b->slack_timer,
			ns_مقصد_ktime(cfs_bandwidth_slack_period),
			HRTIMER_منوال_REL);
}

/* we know any runtime found here is valid as update_curr() precedes return */
ساکن خالی __return_cfs_rq_runtime(ساخت cfs_rq *cfs_rq)
{
	ساخت cfs_bandwidth *cfs_b = tg_cfs_bandwidth(cfs_rq->tg);
	s64 slack_runtime = cfs_rq->runtime_بقایا - min_cfs_rq_runtime;

	اگر (slack_runtime <= 0)
		واپس;

	guard(raw_spinlock)(&cfs_b->تالا);

	اگر (cfs_b->quota != RUNTIME_INF) {
		cfs_b->runtime += slack_runtime;

		/* we are under rq->lock, defer unthrottling using a timer */
		اگر (cfs_b->runtime > sched_cfs_bandwidth_قاش() &&
		    !list_خالی_خانے(&cfs_b->throttled_cfs_rq))
			آغاز_cfs_slack_bandwidth(cfs_b);
	}

	/* even if it's not valid for return we don't want to try again */
	cfs_rq->runtime_بقایا -= slack_runtime;
}

ساکن __always_inline خالی return_cfs_rq_runtime(ساخت cfs_rq *cfs_rq)
{
	اگر (!cfs_bandwidth_مستعمل())
		واپس;

	اگر (!cfs_rq->runtime_enabled || cfs_rq->nr_queued)
		واپس;

	__return_cfs_rq_runtime(cfs_rq);
}

/*
 * This is done with a timer (instead of inline with bandwidth return) since
 * it's necessary to juggle rq->locks to unthrottle their respective cfs_rqs.
 */
ساکن خالی do_sched_cfs_slack_timer(ساخت cfs_bandwidth *cfs_b)
{
	/* confirm we're still not at a refresh boundary */
	scoped_guard(raw_spinlock_irqsave, &cfs_b->تالا) {
		u64 runtime = 0, قاش = sched_cfs_bandwidth_قاش();

		cfs_b->slack_started = false;

		اگر (runtime_refresh_within(cfs_b, min_bandwidth_expiration))
			واپس;

		اگر (cfs_b->quota != RUNTIME_INF && cfs_b->runtime > قاش)
			runtime = cfs_b->runtime;

		اگر (!runtime)
			واپس;
	}

	distribute_cfs_runtime(cfs_b);
}

/*
 * When a group wakes up we want to make sure that its quota is not already
 * expired/exceeded, otherwise it may be allowed to steal additional ticks of
 * runtime as update_curr() throttling can not trigger until it's on-rq.
 */
ساکن خالی check_قطار_میں_ڈالو_throttle(ساخت cfs_rq *cfs_rq)
{
	اگر (!cfs_bandwidth_مستعمل())
		واپس;

	/* an active group must be handled by the update_curr() path */
	اگر (!cfs_rq->runtime_enabled || cfs_rq->h_curr)
		واپس;

	/* ensure the group is not already throttled */
	اگر (cfs_rq_throttled(cfs_rq))
		واپس;

	/* update runtime allocation */
	account_cfs_rq_runtime(cfs_rq, 0);
}

ساکن خالی sync_throttle(ساخت task_گروہ *tg, صحیح cpu)
{
	ساخت cfs_rq *pcfs_rq, *cfs_rq;

	اگر (!cfs_bandwidth_مستعمل())
		واپس;

	اگر (!tg->والد)
		واپس;

	cfs_rq = tg_cfs_rq(tg, cpu);
	pcfs_rq = tg_cfs_rq(tg->والد, cpu);

	cfs_rq->throttle_گنتی = pcfs_rq->throttle_گنتی;
	cfs_rq->throttled_clock_pelt = rq_clock_pelt(cpu_rq(cpu));

	/*
	 * It is not enough to sync the "pelt_clock_throttled" indicator
	 * with the parent cfs_rq when the hierarchy is not queued.
	 * Always join a throttled hierarchy with PELT clock throttled
	 * and leaf it to the first enqueue, or distribution to
	 * unthrottle the PELT clock.
	 */
	اگر (cfs_rq->throttle_گنتی)
		cfs_rq->pelt_clock_throttled = 1;
}

ساکن شمار hrtimer_restart sched_cfs_slack_timer(ساخت hrtimer *timer)
{
	ساخت cfs_bandwidth *cfs_b =
		container_کا(timer, ساخت cfs_bandwidth, slack_timer);

	do_sched_cfs_slack_timer(cfs_b);

	واپس HRTIMER_NORESTART;
}

ساکن شمار hrtimer_restart sched_cfs_period_timer(ساخت hrtimer *timer)
{
	ساخت cfs_bandwidth *cfs_b =
		container_کا(timer, ساخت cfs_bandwidth, period_timer);
	صحیح overrun;
	صحیح idle = 0;
	صحیح گنتی = 0;

	CLASS(raw_spinlock_irqsave, cfsb_guard)(&cfs_b->تالا);

	برائے (;;) {
		overrun = hrtimer_forward_now(timer, cfs_b->period);
		اگر (!overrun)
			توڑو;

		idle = do_sched_cfs_period_timer(cfs_b, overrun, cfsb_guard.flags);

		اگر (++گنتی > 3) {
			u64 new, old = ktime_مقصد_ns(cfs_b->period);

			/*
			 * Grow period by a factor of 2 to avoid losing precision.
			 * Precision loss in the quota/period ratio can cause __cfs_schedulable
			 * to fail.
			 */
			new = old * 2;
			اگر (new < max_bw_quota_period_us * NSEC_فی_USEC) {
				cfs_b->period = ns_مقصد_ktime(new);
				cfs_b->quota *= 2;
				cfs_b->مدت *= 2;

				pr_warn_ratelimited(
	"cfs_period_timer[cpu%d]: period too short, scaling up (new cfs_period_us = %lld, cfs_quota_us = %lld)\n",
					smp_processor_id(),
					div_u64(new, NSEC_فی_USEC),
					div_u64(cfs_b->quota, NSEC_فی_USEC));
			} ورنہ {
				pr_warn_ratelimited(
	"cfs_period_timer[cpu%d]: period too short, but cannot scale up without losing precision (cfs_period_us = %lld, cfs_quota_us = %lld)\n",
					smp_processor_id(),
					div_u64(old, NSEC_فی_USEC),
					div_u64(cfs_b->quota, NSEC_فی_USEC));
			}

			/* reset count so we don't come right back in here */
			گنتی = 0;
		}
	}

	اگر (idle) {
		cfs_b->period_فعال = 0;
		واپس HRTIMER_NORESTART;
	}

	واپس HRTIMER_RESTART;
}

خالی init_cfs_bandwidth(ساخت cfs_bandwidth *cfs_b, ساخت cfs_bandwidth *والد)
{
	raw_spin_تالا_init(&cfs_b->تالا);
	cfs_b->runtime = 0;
	cfs_b->quota = RUNTIME_INF;
	cfs_b->period = us_مقصد_ktime(default_bw_period_us());
	cfs_b->مدت = 0;
	cfs_b->درجہ_وار_quota = والد ? والد->درجہ_وار_quota : RUNTIME_INF;

	INIT_LIST_سر(&cfs_b->throttled_cfs_rq);
	hrtimer_تیاری(&cfs_b->period_timer, sched_cfs_period_timer, CLOCK_MONOTONIC,
		      HRTIMER_منوال_ABS_PINNED);

	/* Add a random offset so that timers interleave */
	hrtimer_سیٹ_expires(&cfs_b->period_timer,
			    get_random_u32_نیچے(cfs_b->period));
	hrtimer_تیاری(&cfs_b->slack_timer, sched_cfs_slack_timer, CLOCK_MONOTONIC,
		      HRTIMER_منوال_REL);
	cfs_b->slack_started = false;
}

ساکن خالی init_cfs_rq_runtime(ساخت cfs_rq *cfs_rq)
{
	cfs_rq->runtime_enabled = 0;
	INIT_LIST_سر(&cfs_rq->throttled_list);
	INIT_LIST_سر(&cfs_rq->throttled_csd_list);
	INIT_LIST_سر(&cfs_rq->throttled_limbo_list);
}

خالی آغاز_cfs_bandwidth(ساخت cfs_bandwidth *cfs_b)
{
	lockdep_assert_held(&cfs_b->تالا);

	اگر (cfs_b->period_فعال)
		واپس;

	cfs_b->period_فعال = 1;
	hrtimer_forward_now(&cfs_b->period_timer, cfs_b->period);
	hrtimer_آغاز_expires(&cfs_b->period_timer, HRTIMER_منوال_ABS_PINNED);
}

ساکن خالی destroy_cfs_bandwidth(ساخت cfs_bandwidth *cfs_b)
{
	صحیح __maybe_unused i;

	/* init_cfs_bandwidth() was not called */
	اگر (!cfs_b->throttled_cfs_rq.اگلا)
		واپس;

	hrtimer_cancel(&cfs_b->period_timer);
	hrtimer_cancel(&cfs_b->slack_timer);

	/*
	 * It is possible that we still have some cfs_rq's pending on a CSD
	 * list, though this race is very rare. In order for this to occur, we
	 * must have raced with the last task leaving the group while there
	 * exist throttled cfs_rq(s), and the period_timer must have queued the
	 * CSD item but the remote cpu has not yet processed it. To handle this,
	 * we can simply flush all pending CSD work inline here. We're
	 * guaranteed at this point that no additional cfs_rq of this group can
	 * join a CSD list.
	 */
	for_each_possible_cpu(i) {
		ساخت rq *rq = cpu_rq(i);

		اگر (list_خالی_خانے(&rq->cfsb_csd_list))
			جاری;

		scoped_guard(irqsave)
			__cfsb_csd_unthrottle(rq);
	}
}

/*
 * Both these CPU hotplug callbacks race against unregister_fair_sched_group()
 *
 * The race is harmless, since modifying bandwidth settings of unhooked group
 * bits doesn't do much.
 */

/* cpu online callback */
ساکن خالی __maybe_unused update_runtime_enabled(ساخت rq *rq)
{
	ساخت task_گروہ *tg;

	lockdep_assert_rq_held(rq);

	guard(rcu)();

	list_for_each_اندراج_rcu(tg, &task_groups, list) {
		ساخت cfs_bandwidth *cfs_b = &tg->cfs_bandwidth;
		ساخت cfs_rq *cfs_rq = tg_cfs_rq(tg, cpu_کا(rq));

		scoped_guard(raw_spinlock, &cfs_b->تالا)
			cfs_rq->runtime_enabled = cfs_b->quota != RUNTIME_INF;
	}
}

/* cpu offline callback */
ساکن خالی __maybe_unused unthrottle_offline_cfs_rqs(ساخت rq *rq)
{
	ساخت task_گروہ *tg;

	lockdep_assert_rq_held(rq);

	// Do not unthrottle for an active CPU
	اگر (cpumask_test_cpu(cpu_کا(rq), cpu_فعال_نقاب))
		واپس;

	/*
	 * The rq clock has already been updated in the
	 * set_rq_offline(), so we should skip updating
	 * the rq clock again in unthrottle_cfs_rq().
	 */
	rq_clock_آغاز_loop_update(rq);

	guard(rcu)();

	list_for_each_اندراج_rcu(tg, &task_groups, list) {
		ساخت cfs_rq *cfs_rq = tg_cfs_rq(tg, cpu_کا(rq));

		اگر (!cfs_rq->runtime_enabled)
			جاری;

		/*
		 * Offline rq is schedulable till CPU is completely disabled
		 * in take_cpu_down(), so we prevent new cfs throttling here.
		 */
		cfs_rq->runtime_enabled = 0;

		اگر (!cfs_rq_throttled(cfs_rq))
			جاری;

		/*
		 * clock_task is not advancing so we just need to make sure
		 * there's some valid quota amount
		 */
		cfs_rq->runtime_بقایا = 1;
		unthrottle_cfs_rq(cfs_rq);
	}

	rq_clock_stop_loop_update(rq);
}

bool cfs_task_bw_constrained(ساخت task_struct *p)
{
	ساخت cfs_rq *cfs_rq = task_cfs_rq(p);

	اگر (!cfs_bandwidth_مستعمل())
		واپس false;

	اگر (cfs_rq->runtime_enabled ||
	    tg_cfs_bandwidth(cfs_rq->tg)->درجہ_وار_quota != RUNTIME_INF)
		واپس true;

	واپس false;
}

#اگر_تعریف_شدہ CONFIG_NO_HZ_بھرا
/* called from pick_next_task_fair() */
ساکن خالی sched_fair_update_stop_tick(ساخت rq *rq, ساخت task_struct *p)
{
	صحیح cpu = cpu_کا(rq);

	اگر (!cfs_bandwidth_مستعمل())
		واپس;

	اگر (!tick_nohz_بھرا_cpu(cpu))
		واپس;

	اگر (rq->nr_running != 1)
		واپس;

	/*
	 *  We know there is only one task runnable and we've just picked it. The
	 *  normal enqueue path will have cleared TICK_DEP_BIT_SCHED if we will
	 *  be otherwise able to stop the tick. Just need to check if we are using
	 *  bandwidth control.
	 */
	اگر (cfs_task_bw_constrained(p))
		tick_nohz_dep_سیٹ_cpu(cpu, TICK_DEP_ثنائی_ہندسہ_SCHED);
}
#اگر_ختم /* CONFIG_NO_HZ_FULL */

#ورنہ /* !CONFIG_CFS_BANDWIDTH: */

ساکن bool account_cfs_rq_runtime(ساخت cfs_rq *cfs_rq, u64 delta_exec) { واپس false; }
ساکن خالی check_قطار_میں_ڈالو_throttle(ساخت cfs_rq *cfs_rq) {}
ساکن اندرونی خالی sync_throttle(ساخت task_گروہ *tg, صحیح cpu) {}
ساکن __always_inline خالی return_cfs_rq_runtime(ساخت cfs_rq *cfs_rq) {}
ساکن خالی task_throttle_تیاری_work(ساخت task_struct *p) {}
ساکن bool task_is_throttled(ساخت task_struct *p) { واپس false; }
ساکن خالی قطار_سے_نکالو_throttled_task(ساخت task_struct *p, صحیح flags) {}
ساکن bool قطار_میں_ڈالو_throttled_task(ساخت task_struct *p) { واپس false; }
ساکن خالی ریکارڈ_throttle_clock(ساخت cfs_rq *cfs_rq) {}

ساکن اندرونی صحیح cfs_rq_throttled(ساخت cfs_rq *cfs_rq)
{
	واپس 0;
}

ساکن اندرونی bool cfs_rq_pelt_clock_throttled(ساخت cfs_rq *cfs_rq)
{
	واپس false;
}

ساکن اندرونی صحیح throttled_hierarchy(ساخت cfs_rq *cfs_rq)
{
	واپس 0;
}

ساکن اندرونی صحیح lb_throttled_hierarchy(ساخت task_struct *p, صحیح dst_cpu)
{
	واپس 0;
}

#اگر_تعریف_شدہ CONFIG_FAIR_گروہ_SCHED
خالی init_cfs_bandwidth(ساخت cfs_bandwidth *cfs_b, ساخت cfs_bandwidth *والد) {}
ساکن خالی init_cfs_rq_runtime(ساخت cfs_rq *cfs_rq) {}
#اگر_ختم

ساکن اندرونی ساخت cfs_bandwidth *tg_cfs_bandwidth(ساخت task_گروہ *tg)
{
	واپس تہی;
}
ساکن اندرونی خالی destroy_cfs_bandwidth(ساخت cfs_bandwidth *cfs_b) {}
ساکن اندرونی خالی update_runtime_enabled(ساخت rq *rq) {}
ساکن اندرونی خالی unthrottle_offline_cfs_rqs(ساخت rq *rq) {}
#اگر_تعریف_شدہ CONFIG_CGROUP_SCHED
bool cfs_task_bw_constrained(ساخت task_struct *p)
{
	واپس false;
}
#اگر_ختم
#اگر_ختم /* !CONFIG_CFS_BANDWIDTH */

#اگر !defined(CONFIG_CFS_BANDWIDTH) || !defined(CONFIG_NO_HZ_بھرا)
ساکن اندرونی خالی sched_fair_update_stop_tick(ساخت rq *rq, ساخت task_struct *p) {}
#اگر_ختم

/**************************************************
 * CFS operations on tasks:
 */

#اگر_تعریف_شدہ CONFIG_SCHED_HRTICK
ساکن خالی hrtick_آغاز_fair(ساخت rq *rq, ساخت task_struct *p)
{
	ساخت sched_entity *se = &p->se;
	غیرعلامتی طویل scale = 1024;
	غیرعلامتی طویل util = 0;
	u64 vdelta;
	u64 delta;

	WARN_ON_ONCE(task_rq(p) != rq);

	اگر (rq->cfs.h_nr_queued <= 1)
		واپس;

	/*
	 * Compute time until virtual deadline
	 */
	vdelta = se->deadline - se->vruntime;
	اگر ((s64)vdelta < 0) {
		اگر (task_current_donor(rq, p))
			resched_curr(rq);
		واپس;
	}
	delta = (se->h_بھرو.وزن * vdelta) / NICE_0_بھرو;

	/*
	 * Correct for instantaneous load of other classes.
	 */
	util += cpu_util_irq(rq);
	اگر (util && util < 1024) {
		scale *= 1024;
		scale /= (1024 - util);
	}

	hrtick_آغاز(rq, (scale * delta) / 1024);
}

/*
 * Called on enqueue to start the hrtick when h_nr_queued becomes more than 1.
 */
ساکن خالی hrtick_update(ساخت rq *rq)
{
	ساخت task_struct *donor = rq->donor;

	اگر (!hrtick_enabled_fair(rq) || donor->sched_class != &fair_sched_class)
		واپس;

	اگر (hrtick_فعال(rq))
		واپس;

	hrtick_آغاز_fair(rq, donor);
}
#ورنہ /* !CONFIG_SCHED_HRTICK: */
ساکن اندرونی خالی
hrtick_آغاز_fair(ساخت rq *rq, ساخت task_struct *p)
{
}

ساکن اندرونی خالی hrtick_update(ساخت rq *rq)
{
}
#اگر_ختم /* !CONFIG_SCHED_HRTICK */

ساکن اندرونی bool cpu_overutilized(صحیح cpu)
{
	غیرعلامتی طویل rq_util_max;

	اگر (!sched_energy_enabled())
		واپس false;

	rq_util_max = uclamp_rq_get(cpu_rq(cpu), UCLAMP_MAX);

	/* Return true only if the utilization doesn't fit CPU's capacity */
	واپس !util_سماتا_cpu(cpu_util_cfs(cpu), 0, rq_util_max, cpu);
}

/*
 * overutilized value make sense only if EAS is enabled
 */
ساکن اندرونی bool is_rd_overutilized(ساخت جڑ_domain *rd)
{
	واپس !sched_energy_enabled() || READ_ONCE(rd->overutilized);
}

ساکن اندرونی خالی سیٹ_rd_overutilized(ساخت جڑ_domain *rd, bool flag)
{
	اگر (!sched_energy_enabled())
		واپس;

	WRITE_ONCE(rd->overutilized, flag);
	trace_sched_overutilized_tp(rd, flag);
}

ساکن اندرونی خالی check_update_overutilized_حالت(ساخت rq *rq)
{
	/*
	 * overutilized field is used for load balancing decisions only
	 * if energy aware scheduler is being used
	 */

	اگر (!is_rd_overutilized(rq->rd) && cpu_overutilized(rq->cpu))
		سیٹ_rd_overutilized(rq->rd, 1);
}

/* Runqueue only has SCHED_IDLE tasks enqueued */
ساکن صحیح sched_idle_rq(ساخت rq *rq)
{
	واپس unlikely(rq->nr_running == rq->cfs.h_nr_idle &&
			rq->nr_running);
}

ساکن صحیح انتخاب_sched_idle_rq(ساخت rq *rq, ساخت task_struct *p)
{
	واپس sched_idle_rq(rq) && !task_has_idle_policy(p);
}

ساکن صحیح انتخاب_idle_cpu(صحیح cpu, ساخت task_struct *p)
{
	واپس دستیاب_idle_cpu(cpu) ||
	       انتخاب_sched_idle_rq(cpu_rq(cpu), p);
}

ساکن خالی
requeue_delayed_entity(ساخت cfs_rq *cfs_rq, ساخت sched_entity *se)
{
	/*
	 * se->sched_delayed should imply: se->on_rq == 1.
	 * Because a delayed entity is one that is still on
	 * the runqueue competing until elegibility.
	 */
	WARN_ON_ONCE(!se->sched_delayed);
	WARN_ON_ONCE(!se->on_rq);

	اگر (update_entity_lag(cfs_rq, se)) {
		cfs_rq->h_nr_queued--;
		اگر (se != cfs_rq->curr)
			__dequeue_entity(cfs_rq, se);
		رکھو_entity(cfs_rq, se, 0);
		اگر (se != cfs_rq->curr)
			__enqueue_entity(cfs_rq, se);
		cfs_rq->h_nr_queued++;
	}

	update_بھرو_avg(cfs_rq, se, 0);
	clear_delayed(se);
}

ساکن غیرعلامتی طویل قطار_میں_ڈالو_hierarchy(ساخت task_struct *p, صحیح flags)
{
	غیرعلامتی طویل وزن = NICE_0_بھرو;
	صحیح task_new = !(flags & قطار_میں_ڈالو_WAKEUP);
	ساخت sched_entity *se = &p->se;
	صحیح h_nr_idle = task_has_idle_policy(p);
	صحیح h_nr_runnable = 1;

	اگر (task_new && se->sched_delayed)
		h_nr_runnable = 0;

	for_each_sched_entity(se) {
		ساخت cfs_rq *cfs_rq = cfs_rq_کا(se);

		update_curr(cfs_rq);

		اگر (!se->on_rq) {
			قطار_میں_ڈالو_entity(cfs_rq, se, flags);
		} ورنہ {
			update_بھرو_avg(cfs_rq, se, UPDATE_TG);
			se_update_runnable(se);
			update_cfs_گروہ(se);
		}

		cfs_rq->h_nr_runnable += h_nr_runnable;
		cfs_rq->h_nr_queued++;
		cfs_rq->h_nr_idle += h_nr_idle;

		اگر (cfs_rq_is_idle(cfs_rq))
			h_nr_idle = 1;

		وزن = __calc_prop_weight(cfs_rq, se, وزن);

		flags = قطار_میں_ڈالو_WAKEUP;
	}

	واپس وزن;
}

/* Update curr's vruntime before placing entity or updating lag */
ساکن اندرونی خالی update_curr_eevdf(ساخت cfs_rq *cfs_rq)
{
	اگر (!cfs_rq->curr)
		واپس;

	update_curr(cfs_rq_کا(cfs_rq->curr));
}

/*
 * The enqueue_task method is called before nr_running is
 * increased. Here we update the fair scheduling stats and
 * then put the task into the rbtree:
 */
ساکن خالی
قطار_میں_ڈالو_task_fair(ساخت rq *rq, ساخت task_struct *p, صحیح flags)
{
	صحیح rq_h_nr_queued = rq->cfs.h_nr_queued;
	صحیح task_new = !(flags & قطار_میں_ڈالو_WAKEUP);
	ساخت sched_entity *se = &p->se;
	ساخت cfs_rq *cfs_rq = &rq->cfs;
	غیرعلامتی طویل وزن;
	bool curr;

	اگر (task_is_throttled(p) && قطار_میں_ڈالو_throttled_task(p))
		واپس;

	/*
	 * The code below (indirectly) updates schedutil which looks at
	 * the cfs_rq utilization to select a frequency.
	 * Let's add the task's estimated utilization to the cfs_rq's
	 * estimated utilization, before we update schedutil.
	 */
	اگر (!p->se.sched_delayed || (flags & قطار_میں_ڈالو_DELAYED))
		util_est_قطار_میں_ڈالو(cfs_rq, p);

	update_curr_eevdf(cfs_rq);

	اگر (flags & قطار_میں_ڈالو_DELAYED) {
		requeue_delayed_entity(cfs_rq, se);
		واپس;
	}

	/*
	 * If in_iowait is set, the code below may not trigger any cpufreq
	 * utilization updates, so do it here explicitly with the IOWAIT flag
	 * passed.
	 */
	اگر (p->اندر_iowait)
		cpufreq_update_util(rq, SCHED_CPUFREQ_IOWAIT);

	curr = (cfs_rq->curr == se);
	/*
	 * This can actually happen with CFS_BANDWIDTH, a task can be throttled
	 * and dequeued, unthrottled from another CPU and then enqueued again
	 * before schedule().
	 */
	اگر (curr)
		رکھو_entity(cfs_rq, se, flags);

	اگر (se->on_rq && se->sched_delayed)
		requeue_delayed_entity(cfs_rq, se);

	وزن = قطار_میں_ڈالو_hierarchy(p, flags);

	اگر (!curr) {
		reweight_eevdf(cfs_rq, se, وزن, false);
		رکھو_entity(cfs_rq, se, flags | قطار_میں_ڈالو_QUEUED);
		__enqueue_entity(cfs_rq, se);
	}

	اگر (!rq_h_nr_queued && rq->cfs.h_nr_queued)
		dl_server_آغاز(&rq->fair_server);

	/* At this point se is NULL and we are at root level*/
	جوڑو_nr_running(rq, 1);

	/*
	 * Since new tasks are assigned an initial util_avg equal to
	 * half of the spare capacity of their CPU, tiny tasks have the
	 * ability to cross the overutilized threshold, which will
	 * result in the load balancer ruining all the task placement
	 * done by EAS. As a way to mitigate that effect, do not account
	 * for the first enqueue operation of new tasks during the
	 * overutilized flag detection.
	 *
	 * A better way of solving this problem would be to wait for
	 * the PELT signals of tasks to converge before taking them
	 * into account, but that is not straightforward to implement,
	 * and the following generally works well enough in practice.
	 */
	اگر (!task_new)
		check_update_overutilized_حالت(rq);

	assert_list_برگ_cfs_rq(rq);

	hrtick_update(rq);
}

ساکن خالی قطار_سے_نکالو_hierarchy(ساخت task_struct *p, صحیح flags)
{
	ساخت sched_entity *se = &p->se;
	bool task_sleep = flags & قطار_سے_نکالو_SLEEP;
	bool task_delayed = flags & قطار_سے_نکالو_DELAYED;
	bool task_throttled = flags & قطار_سے_نکالو_THROTTLE;
	صحیح h_nr_runnable = 0;
	صحیح h_nr_idle = task_has_idle_policy(p);
	bool قطار_سے_نکالو = true;

	اگر (task_sleep || task_delayed || !se->sched_delayed)
		h_nr_runnable = 1;

	for_each_sched_entity(se) {
		ساخت cfs_rq *cfs_rq = cfs_rq_کا(se);

		update_curr(cfs_rq);

		اگر (قطار_سے_نکالو) {
			قطار_سے_نکالو_entity(cfs_rq, se, flags);
			/* Don't dequeue parent if it has other entities besides us */
			اگر (cfs_rq->بھرو.وزن)
				قطار_سے_نکالو = false;
		} ورنہ {
			update_بھرو_avg(cfs_rq, se, UPDATE_TG);
			se_update_runnable(se);
			update_cfs_گروہ(se);
		}

		cfs_rq->h_nr_runnable -= h_nr_runnable;
		cfs_rq->h_nr_queued--;
		cfs_rq->h_nr_idle -= h_nr_idle;

		اگر (cfs_rq_is_idle(cfs_rq))
			h_nr_idle = 1;

		اگر (throttled_hierarchy(cfs_rq) && task_throttled)
			ریکارڈ_throttle_clock(cfs_rq);

		flags |= قطار_سے_نکالو_SLEEP;
		flags &= ~(قطار_سے_نکالو_DELAYED | قطار_سے_نکالو_SPECIAL);
	}
}

/*
 * The part of dequeue_task_fair() that is needed to dequeue delayed tasks.
 *
 * Returns:
 *   true  - dequeued
 *   false - delayed
 */
ساکن bool __dequeue_task(ساخت rq *rq, ساخت task_struct *p, صحیح flags)
{
	ساخت sched_entity *se = &p->se;
	ساخت cfs_rq *cfs_rq = &rq->cfs;
	bool was_sched_idle = sched_idle_rq(rq);
	bool task_sleep = flags & قطار_سے_نکالو_SLEEP;
	bool task_delayed = flags & قطار_سے_نکالو_DELAYED;

	clear_buddies(cfs_rq, se);

	update_curr_eevdf(cfs_rq);
	update_entity_lag(cfs_rq, se);

	اگر (flags & قطار_سے_نکالو_DELAYED) {
		WARN_ON_ONCE(!se->sched_delayed);
	} ورنہ {
		bool delay = task_sleep;
		/*
		 * DELAY_DEQUEUE relies on spurious wakeups, special task
		 * states must not suffer spurious wakeups, excempt them.
		 */
		اگر (flags & (قطار_سے_نکالو_SPECIAL | قطار_سے_نکالو_THROTTLE))
			delay = false;

		WARN_ON_ONCE(delay && se->sched_delayed);

		اگر (sched_feat(DELAY_قطار_سے_نکالو) && delay &&
		    !entity_eligible(cfs_rq, se)) {
			update_بھرو_avg(cfs_rq_کا(se), se, UPDATE_UTIL_EST);
			سیٹ_delayed(se);
			واپس false;
		}
	}

	قطار_سے_نکالو_hierarchy(p, flags);

	اگر (sched_feat(رکھو_REL_DEADLINE) && !task_sleep) {
		se->deadline -= se->vruntime;
		se->rel_deadline = 1;
	}
	اگر (se != cfs_rq->curr)
		__dequeue_entity(cfs_rq, se);

	sub_nr_running(rq, 1);

	/* balance early to pull high priority tasks */
	اگر (unlikely(!was_sched_idle && sched_idle_rq(rq)))
		rq->اگلا_balance = jiffies;

	اگر (task_delayed) {
		clear_delayed(se);

		WARN_ON_ONCE(!task_sleep);
		WARN_ON_ONCE(p->on_rq != 1);

		/*
		 * Fix-up what block_task() skipped.
		 *
		 * Must be last, @p might not be valid after this.
		 */
		__block_task(rq, p);
	}

	واپس true;
}

/*
 * The dequeue_task method is called before nr_running is
 * decreased. We remove the task from the rbtree and
 * update the fair scheduling stats:
 */
ساکن bool قطار_سے_نکالو_task_fair(ساخت rq *rq, ساخت task_struct *p, صحیح flags)
{
	اگر (task_is_throttled(p)) {
		قطار_سے_نکالو_throttled_task(p, flags);
		واپس true;
	}

	اگر (!p->se.sched_delayed)
		util_est_قطار_سے_نکالو(&rq->cfs, p);

	اگر (!__dequeue_task(rq, p, flags))
		واپس false;

	/*
	 * Must not reference @p after __dequeue_task(DEQUEUE_DELAYED).
	 */
	واپس true;
}

ساکن اندرونی غیرعلامتی صحیح cfs_h_nr_delayed(ساخت rq *rq)
{
	واپس (rq->cfs.h_nr_queued - rq->cfs.h_nr_runnable);
}

/* Working cpumask for: sched_balance_rq(), sched_balance_newidle(). */
ساکن DEFINE_فی_CPU(cpumask_var_t, بھرو_balance_نقاب);
ساکن DEFINE_فی_CPU(cpumask_var_t, select_rq_نقاب);
ساکن DEFINE_فی_CPU(cpumask_var_t, should_we_balance_tmpmask);

#اگر_تعریف_شدہ CONFIG_NO_HZ_COMMON

ساکن ساخت {
	cpumask_var_t idle_cpus_نقاب;
	صحیح has_blocked_بھرو;		/* Idle CPUS has blocked load */
	صحیح needs_update;		/* Newly idle CPUs need their next_balance collated */
	غیرعلامتی طویل اگلا_balance;     /* in jiffy units */
	غیرعلامتی طویل اگلا_blocked;	/* Next update of blocked load in jiffies */
} nohz ____cacheline_aligned;

#اگر_ختم /* CONFIG_NO_HZ_COMMON */

ساکن غیرعلامتی طویل cpu_بھرو(ساخت rq *rq)
{
	واپس cfs_rq_بھرو_avg(&rq->cfs);
}

/*
 * cpu_load_without - compute CPU load without any contributions from *p
 * @cpu: the CPU which load is requested
 * @p: the task which load should be discounted
 *
 * The load of a CPU is defined by the load of tasks currently enqueued on that
 * CPU as well as tasks which are currently sleeping after an execution on that
 * CPU.
 *
 * This method returns the load of the specified CPU by discounting the load of
 * the specified task, whenever the task is currently contributing to the CPU
 * load.
 */
ساکن غیرعلامتی طویل cpu_بھرو_without(ساخت rq *rq, ساخت task_struct *p)
{
	ساخت cfs_rq *cfs_rq;
	غیرعلامتی صحیح بھرو;

	/* Task has no contribution or is new */
	اگر (cpu_کا(rq) != task_cpu(p) || !READ_ONCE(p->se.avg.آخری_update_وقت))
		واپس cpu_بھرو(rq);

	cfs_rq = &rq->cfs;
	بھرو = READ_ONCE(cfs_rq->avg.بھرو_avg);

	/* Discount task's util from CPU's util */
	lsub_مثبت(&بھرو, task_h_بھرو(p));

	واپس بھرو;
}

ساکن غیرعلامتی طویل cpu_runnable(ساخت rq *rq)
{
	واپس cfs_rq_runnable_avg(&rq->cfs);
}

ساکن غیرعلامتی طویل cpu_runnable_without(ساخت rq *rq, ساخت task_struct *p)
{
	ساخت cfs_rq *cfs_rq;
	غیرعلامتی صحیح runnable;

	/* Task has no contribution or is new */
	اگر (cpu_کا(rq) != task_cpu(p) || !READ_ONCE(p->se.avg.آخری_update_وقت))
		واپس cpu_runnable(rq);

	cfs_rq = &rq->cfs;
	runnable = READ_ONCE(cfs_rq->avg.runnable_avg);

	/* Discount task's runnable from CPU's runnable */
	lsub_مثبت(&runnable, p->se.avg.runnable_avg);

	واپس runnable;
}

ساکن غیرعلامتی طویل استعداد_کا(صحیح cpu)
{
	واپس cpu_rq(cpu)->cpu_استعداد;
}

ساکن خالی ریکارڈ_wakee(ساخت task_struct *p)
{
	/*
	 * Only decay a single time; tasks that have less then 1 wakeup per
	 * jiffy will not have built up many flips.
	 */
	اگر (وقت_after(jiffies, current->wakee_flip_decay_ts + HZ)) {
		current->wakee_flips >>= 1;
		current->wakee_flip_decay_ts = jiffies;
	}

	اگر (current->آخری_wakee != p) {
		current->آخری_wakee = p;
		current->wakee_flips++;
	}
}

/*
 * Detect M:N waker/wakee relationships via a switching-frequency heuristic.
 *
 * A waker of many should wake a different task than the one last awakened
 * at a frequency roughly N times higher than one of its wakees.
 *
 * In order to determine whether we should let the load spread vs consolidating
 * to shared cache, we look for a minimum 'flip' frequency of llc_size in one
 * partner, and a factor of lls_size higher frequency in the other.
 *
 * With both conditions met, we can be relatively sure that the relationship is
 * non-monogamous, with partner count exceeding socket size.
 *
 * Waker/wakee being client/server, worker/dispatcher, interrupt source or
 * whatever is irrelevant, spread criteria is apparent partner count exceeds
 * socket size.
 */
ساکن صحیح wake_wide(ساخت task_struct *p)
{
	غیرعلامتی صحیح master = current->wakee_flips;
	غیرعلامتی صحیح slave = p->wakee_flips;
	صحیح ضربی_عدد = __this_cpu_read(sd_llc_سائز);

	اگر (master < slave)
		تبادلہ(master, slave);
	اگر (slave < ضربی_عدد || master < slave * ضربی_عدد)
		واپس 0;
	واپس 1;
}

/*
 * The purpose of wake_affine() is to quickly determine on which CPU we can run
 * soonest. For the purpose of speed we only consider the waking and previous
 * CPU.
 *
 * wake_affine_idle() - only considers 'now', it check if the waking CPU is
 *			cache-affine and is (or	will be) idle.
 *
 * wake_affine_weight() - considers the weight to reflect the average
 *			  scheduling latency of the CPUs. This seems to work
 *			  for the overloaded case.
 */
ساکن صحیح
wake_affine_idle(صحیح this_cpu, صحیح پچھلا_cpu, صحیح sync)
{
	/*
	 * If this_cpu is idle, it implies the wakeup is from interrupt
	 * context. Only allow the move if cache is shared. Otherwise an
	 * interrupt intensive workload could force all tasks onto one
	 * node depending on the IO topology or IRQ affinity settings.
	 *
	 * If the prev_cpu is idle and cache affine then avoid a migration.
	 * There is no guarantee that the cache hot data from an interrupt
	 * is more important than cache hot data on the prev_cpu and from
	 * a cpufreq perspective, it's better to have higher utilisation
	 * on one CPU.
	 */
	اگر (دستیاب_idle_cpu(this_cpu) && cpus_share_cache(this_cpu, پچھلا_cpu))
		واپس دستیاب_idle_cpu(پچھلا_cpu) ? پچھلا_cpu : this_cpu;

	اگر (sync) {
		ساخت rq *rq = cpu_rq(this_cpu);

		اگر ((rq->nr_running - cfs_h_nr_delayed(rq)) == 1)
			واپس this_cpu;
	}

	اگر (دستیاب_idle_cpu(پچھلا_cpu))
		واپس پچھلا_cpu;

	واپس nr_cpumask_بٹ;
}

ساکن صحیح
wake_affine_وزن(ساخت sched_domain *sd, ساخت task_struct *p,
		   صحیح this_cpu, صحیح پچھلا_cpu, صحیح sync)
{
	s64 this_eff_بھرو, پچھلا_eff_بھرو;
	غیرعلامتی طویل task_بھرو;

	this_eff_بھرو = cpu_بھرو(cpu_rq(this_cpu));

	اگر (sync) {
		غیرعلامتی طویل current_بھرو = task_h_بھرو(current);

		اگر (current_بھرو > this_eff_بھرو)
			واپس this_cpu;

		this_eff_بھرو -= current_بھرو;
	}

	task_بھرو = task_h_بھرو(p);

	this_eff_بھرو += task_بھرو;
	اگر (sched_feat(WA_BIAS))
		this_eff_بھرو *= 100;
	this_eff_بھرو *= استعداد_کا(پچھلا_cpu);

	پچھلا_eff_بھرو = cpu_بھرو(cpu_rq(پچھلا_cpu));
	پچھلا_eff_بھرو -= task_بھرو;
	اگر (sched_feat(WA_BIAS))
		پچھلا_eff_بھرو *= 100 + (sd->imbalance_pct - 100) / 2;
	پچھلا_eff_بھرو *= استعداد_کا(this_cpu);

	/*
	 * If sync, adjust the weight of prev_eff_load such that if
	 * prev_eff == this_eff that select_idle_sibling() will consider
	 * stacking the wakee on top of the waker if no other CPU is
	 * idle.
	 */
	اگر (sync)
		پچھلا_eff_بھرو += 1;

	واپس this_eff_بھرو < پچھلا_eff_بھرو ? this_cpu : nr_cpumask_بٹ;
}

ساکن صحیح wake_affine(ساخت sched_domain *sd, ساخت task_struct *p,
		       صحیح this_cpu, صحیح پچھلا_cpu, صحیح sync)
{
	صحیح ہدف = nr_cpumask_بٹ;

	اگر (sched_feat(WA_IDLE))
		ہدف = wake_affine_idle(this_cpu, پچھلا_cpu, sync);

	اگر (sched_feat(WA_وزن) && ہدف == nr_cpumask_بٹ)
		ہدف = wake_affine_وزن(sd, p, this_cpu, پچھلا_cpu, sync);

	schedstat_inc(p->stats.nr_wakeups_affine_attempts);
	اگر (ہدف != this_cpu)
		واپس پچھلا_cpu;

	schedstat_inc(sd->ttwu_move_affine);
	schedstat_inc(p->stats.nr_wakeups_affine);
	واپس ہدف;
}

ساکن ساخت sched_گروہ *
sched_balance_ڈھونڈو_dst_گروہ(ساخت sched_domain *sd, ساخت task_struct *p, صحیح this_cpu);

/*
 * sched_balance_find_dst_group_cpu - find the idlest CPU among the CPUs in the group.
 */
ساکن صحیح
sched_balance_ڈھونڈو_dst_گروہ_cpu(ساخت sched_گروہ *گروہ, ساخت task_struct *p, صحیح this_cpu)
{
	غیرعلامتی طویل بھرو, min_بھرو = ULONG_MAX;
	u64 min_exit_latency = U64_MAX;
	غیرعلامتی صحیح nr_candidates = 0;
	صحیح کم_سے_کم_loaded_cpu = this_cpu;
	صحیح shallowest_idle_cpu = -1;
	صحیح i;

	/* Check if we have any choice: */
	اگر (گروہ->گروہ_وزن == 1)
		واپس cpumask_پہلا(sched_گروہ_وسعت(گروہ));

	/* Traverse only the allowed CPUs */
	for_each_cpu_and(i, sched_گروہ_وسعت(گروہ), p->cpus_ptr) {
		ساخت rq *rq = cpu_rq(i);

		اگر (!sched_core_cookie_مطابقت(rq, p))
			جاری;

		اگر (انتخاب_sched_idle_rq(rq, p))
			واپس i;

		اگر (دستیاب_idle_cpu(i)) {
			ساخت cpuidle_state *idle = idle_get_state(rq);
			u64 exit_latency = idle ? idle->exit_latency : U64_MAX;

			اگر (shallowest_idle_cpu == -1 || exit_latency < min_exit_latency) {
				min_exit_latency = exit_latency;
				shallowest_idle_cpu = i;
				nr_candidates = 1;
			} ورنہ اگر (exit_latency == min_exit_latency) {
				nr_candidates++;
				اگر (!reciprocal_scale(sched_rng(), nr_candidates))
					shallowest_idle_cpu = i;
			}
		} ورنہ اگر (shallowest_idle_cpu == -1) {
			بھرو = cpu_بھرو(cpu_rq(i));
			اگر (بھرو < min_بھرو) {
				min_بھرو = بھرو;
				کم_سے_کم_loaded_cpu = i;
			}
		}
	}

	واپس shallowest_idle_cpu != -1 ? shallowest_idle_cpu : کم_سے_کم_loaded_cpu;
}

ساکن اندرونی صحیح sched_balance_ڈھونڈو_dst_cpu(ساخت sched_domain *sd, ساخت task_struct *p,
				  صحیح cpu, صحیح پچھلا_cpu, صحیح sd_flag)
{
	صحیح new_cpu = cpu;

	اگر (!cpumask_intersects(sched_domain_وسعت(sd), p->cpus_ptr))
		واپس پچھلا_cpu;

	/*
	 * We need task's util for cpu_util_without, sync it up to
	 * prev_cpu's last_update_time.
	 */
	اگر (!(sd_flag & SD_BALANCE_FORK))
		sync_entity_بھرو_avg(&p->se);

	جب (sd) {
		ساخت sched_گروہ *گروہ;
		ساخت sched_domain *tmp;
		صحیح وزن;

		اگر (!(sd->flags & sd_flag)) {
			sd = sd->بچہ;
			جاری;
		}

		گروہ = sched_balance_ڈھونڈو_dst_گروہ(sd, p, cpu);
		اگر (!گروہ) {
			sd = sd->بچہ;
			جاری;
		}

		new_cpu = sched_balance_ڈھونڈو_dst_گروہ_cpu(گروہ, p, cpu);
		اگر (new_cpu == cpu) {
			/* Now try balancing at a lower domain level of 'cpu': */
			sd = sd->بچہ;
			جاری;
		}

		/* Now try balancing at a lower domain level of 'new_cpu': */
		cpu = new_cpu;
		وزن = sd->وسعت_وزن;
		sd = تہی;
		for_each_domain(cpu, tmp) {
			اگر (وزن <= tmp->وسعت_وزن)
				توڑو;
			اگر (tmp->flags & sd_flag)
				sd = tmp;
		}
	}

	واپس new_cpu;
}

ساکن اندرونی صحیح __select_idle_cpu(صحیح cpu, ساخت task_struct *p)
{
	اگر (انتخاب_idle_cpu(cpu, p) && sched_cpu_cookie_مطابقت(cpu_rq(cpu), p))
		واپس cpu;

	واپس -1;
}

DEFINE_STATIC_کلید_FALSE(sched_smt_present);
EXPORT_علامت_GPL(sched_smt_present);

ساکن اندرونی خالی سیٹ_idle_cores(صحیح cpu, صحیح val)
{
	ساخت sched_domain_shared *sds;

	sds = rcu_dereference_all(فی_cpu(sd_balance_shared, cpu));
	اگر (sds)
		WRITE_ONCE(sds->has_idle_cores, val);
}

ساکن اندرونی bool test_idle_cores(صحیح cpu)
{
	ساخت sched_domain_shared *sds;

	sds = rcu_dereference_all(فی_cpu(sd_balance_shared, cpu));
	اگر (sds)
		واپس READ_ONCE(sds->has_idle_cores);

	واپس false;
}

/*
 * Redirect a CPU to a higher-priority available sibling in its SMT domain,
 * subject to task affinity.
 */
ساکن اندرونی صحیح select_idle_smt_cpu(ساخت task_struct *p, صحیح cpu)
{
	ساخت sched_domain *sd;
	صحیح بہترین = cpu;
	صحیح sibling;

	اگر (!sched_smt_فعال())
		واپس cpu;

	sd = rcu_dereference_all(cpu_rq(cpu)->sd);
	اگر (!sd || !(sd->flags & SD_SHARE_CPUCAPACITY) ||
	    !(sd->flags & SD_ASYM_PACKING))
		واپس cpu;

	for_each_cpu_and(sibling, sched_domain_وسعت(sd), p->cpus_ptr) {
		اگر (sibling == بہترین || !انتخاب_idle_cpu(sibling, p))
			جاری;

		اگر (sched_asym_prefer(sibling, بہترین))
			بہترین = sibling;
	}

	واپس بہترین;
}

/*
 * Scans the local SMT mask to see if the entire core is idle, and records this
 * information in sd_balance_shared->has_idle_cores.
 *
 * Since SMT siblings share all cache levels, inspecting this limited remote
 * state should be fairly cheap.
 */
خالی __update_idle_core(ساخت rq *rq)
{
	صحیح core = cpu_کا(rq);
	صحیح cpu;

	rcu_read_تالا();
	اگر (test_idle_cores(core))
		جاؤ unlock;

	for_each_cpu(cpu, cpu_smt_نقاب(core)) {
		اگر (cpu == core)
			جاری;

		اگر (!دستیاب_idle_cpu(cpu))
			جاؤ unlock;
	}

	سیٹ_idle_cores(core, 1);
unlock:
	rcu_read_unlock();
}

/*
 * Scan the entire LLC domain for idle cores; this dynamically switches off if
 * there are no idle cores left in the system; tracked through
 * sd_balance_shared->has_idle_cores and enabled through update_idle_core()
 * above.
 */
ساکن صحیح select_idle_core(ساخت task_struct *p, صحیح core, ساخت cpumask *cpus, صحیح *idle_cpu)
{
	bool idle = true;
	صحیح cpu;

	for_each_cpu(cpu, cpu_smt_نقاب(core)) {
		اگر (!دستیاب_idle_cpu(cpu)) {
			idle = false;
			اگر (*idle_cpu == -1) {
				اگر (انتخاب_sched_idle_rq(cpu_rq(cpu), p) &&
				    cpumask_test_cpu(cpu, cpus)) {
					*idle_cpu = cpu;
					توڑو;
				}
				جاری;
			}
			توڑو;
		}
		اگر (*idle_cpu == -1 && cpumask_test_cpu(cpu, cpus))
			*idle_cpu = cpu;
	}

	اگر (idle)
		واپس core;

	cpumask_andnot(cpus, cpus, cpu_smt_نقاب(core));
	واپس -1;
}

/*
 * Scan the local SMT mask for idle CPUs.
 */
ساکن صحیح select_idle_smt(ساخت task_struct *p, ساخت sched_domain *sd, صحیح ہدف)
{
	صحیح cpu;

	for_each_cpu_and(cpu, cpu_smt_نقاب(ہدف), p->cpus_ptr) {
		اگر (cpu == ہدف)
			جاری;
		/*
		 * Check if the CPU is in the LLC scheduling domain of @target.
		 * Due to isolcpus, there is no guarantee that all the siblings are in the domain.
		 */
		اگر (!cpumask_test_cpu(cpu, sched_domain_وسعت(sd)))
			جاری;
		اگر (انتخاب_idle_cpu(cpu, p))
			واپس cpu;
	}

	واپس -1;
}

/*
 * Scan the LLC domain for idle CPUs; this is dynamically regulated by
 * comparing the average scan cost (tracked in sd->avg_scan_cost) against the
 * average idle time for this rq (as found in rq->avg_idle).
 */
ساکن صحیح select_idle_cpu(ساخت task_struct *p, ساخت sched_domain *sd, bool has_idle_core, صحیح ہدف)
{
	ساخت cpumask *cpus = this_cpu_cpumask_var_ptr(select_rq_نقاب);
	صحیح i, cpu, idle_cpu = -1, nr = زیادہ_سے_زیادہ_عدد;

	اگر (sched_feat(SIS_UTIL) && sd->shared) {
		/*
		 * Increment because !--nr is the condition to stop scan.
		 *
		 * Since "sd" is "sd_llc" for target CPU dereferenced in the
		 * caller, it is safe to directly dereference "sd->shared".
		 * Topology bits always ensure it assigned for "sd_llc" abd it
		 * cannot disappear as long as we have a RCU protected
		 * reference to one the associated "sd" here.
		 */
		nr = READ_ONCE(sd->shared->nr_idle_جائزہ) + 1;
		/* overloaded LLC is unlikely to have idle cpu/core */
		اگر (nr == 1)
			واپس -1;
	}

	اگر (!cpumask_and(cpus, sched_domain_وسعت(sd), p->cpus_ptr))
		واپس -1;

	اگر (static_branch_unlikely(&sched_cluster_فعال)) {
		ساخت sched_گروہ *sg = sd->groups;

		اگر (sg->flags & SD_CLUSTER) {
			for_each_cpu_wrap(cpu, sched_گروہ_وسعت(sg), ہدف + 1) {
				اگر (!cpumask_test_cpu(cpu, cpus))
					جاری;

				اگر (has_idle_core) {
					i = select_idle_core(p, cpu, cpus, &idle_cpu);
					اگر ((غیرعلامتی صحیح)i < nr_cpumask_بٹ)
						واپس i;
				} ورنہ {
					اگر (--nr <= 0)
						واپس -1;
					idle_cpu = __select_idle_cpu(cpu, p);
					اگر ((غیرعلامتی صحیح)idle_cpu < nr_cpumask_بٹ)
						واپس idle_cpu;
				}
			}
			cpumask_andnot(cpus, cpus, sched_گروہ_وسعت(sg));
		}
	}

	for_each_cpu_wrap(cpu, cpus, ہدف + 1) {
		اگر (has_idle_core) {
			i = select_idle_core(p, cpu, cpus, &idle_cpu);
			اگر ((غیرعلامتی صحیح)i < nr_cpumask_بٹ)
				واپس i;

		} ورنہ {
			اگر (--nr <= 0)
				واپس -1;
			idle_cpu = __select_idle_cpu(cpu, p);
			اگر ((غیرعلامتی صحیح)idle_cpu < nr_cpumask_بٹ)
				توڑو;
		}
	}

	اگر (has_idle_core)
		سیٹ_idle_cores(ہدف, false);

	واپس idle_cpu;
}

/*
 * Idle-capacity scan converts util_fits_cpu() outcomes into preference ranks,
 * where lower values indicate a better fit - see select_idle_capacity().
 *
 * A CPU that both fits the task and sits on a fully-idle SMT core is returned
 * immediately and is never assigned one of these ranks. On !SMT every CPU is
 * its own "core", so the early return covers all fits-and-idle cases and the
 * core-tier ranks below become unreachable.
 *
 *   Rank                            Val  Tier    Meaning
 *   ------------------------------  ---  ------  ---------------------------
 *   ASYM_IDLE_UCLAMP_MISFIT         -4   core    Idle core; capacity fits
 *                                                util but uclamp_min misses.
 *   ASYM_IDLE_COMPLETE_MISFIT       -3   core    Idle core; capacity does
 *                                                not fit. Still beats every
 *                                                thread-tier rank: a busy
 *                                                sibling cuts effective
 *                                                capacity more than a
 *                                                misfit hurts a quiet core.
 *   ASYM_IDLE_THREAD_FITS           -2   thread  Busy SMT sibling; capacity
 *                                                fits util + uclamp.
 *   ASYM_IDLE_THREAD_UCLAMP_MISFIT  -1   thread  Busy SMT sibling; capacity
 *                                                fits but uclamp_min misses
 *                                                (native util_fits_cpu()
 *                                                return value).
 *   ASYM_IDLE_THREAD_MISFIT          0   thread  Busy SMT sibling; capacity
 *                                                does not fit.
 *
 * ASYM_IDLE_CORE_BIAS (-3) is an offset, not a state. On an idle core,
 * fits += ASYM_IDLE_CORE_BIAS rebases thread-tier ranks into the core tier:
 *
 *   ASYM_IDLE_THREAD_UCLAMP_MISFIT (-1) + BIAS -> ASYM_IDLE_UCLAMP_MISFIT   (-4)
 *   ASYM_IDLE_THREAD_MISFIT         (0) + BIAS -> ASYM_IDLE_COMPLETE_MISFIT (-3)
 *
 * ASYM_IDLE_THREAD_FITS (-2) is never rebased because a fully-fitting idle-core
 * candidate early-returns from select_idle_capacity().
 */
شمار asym_سماتا_state {
	ASYM_IDLE_UCLAMP_MISFIT = -4,
	ASYM_IDLE_COMPLETE_MISFIT,
	ASYM_IDLE_THREAD_سماتا,
	ASYM_IDLE_THREAD_UCLAMP_MISFIT,
	ASYM_IDLE_THREAD_MISFIT,

	/* util_fits_cpu() bias for idle core */
	ASYM_IDLE_CORE_BIAS = -3,
};

/*
 * Scan the asym_capacity domain for idle CPUs; pick the first idle one on which
 * the task fits. If no CPU is big enough, but there are idle ones, try to
 * maximize capacity.
 */
ساکن صحیح
select_idle_استعداد(ساخت task_struct *p, ساخت sched_domain *sd, صحیح ہدف)
{
	/*
	 * On !SMT systems, has_idle_core is always false and preferred_core
	 * is always true (CPU == core), so the SMT preference logic below
	 * collapses to the plain capacity scan.
	 */
	bool has_idle_core = sched_smt_فعال() && test_idle_cores(ہدف);
	غیرعلامتی طویل task_util, util_min, util_max, بہترین_cap = 0;
	صحیح سماتا, بہترین_سماتا = ASYM_IDLE_THREAD_MISFIT;
	صحیح cpu, بہترین_cpu = -1;
	ساخت cpumask *cpus;
	صحیح nr = زیادہ_سے_زیادہ_عدد;

	cpus = this_cpu_cpumask_var_ptr(select_rq_نقاب);
	cpumask_and(cpus, sched_domain_وسعت(sd), p->cpus_ptr);

	task_util = task_util_est(p);
	util_min = uclamp_eff_قدر(p, UCLAMP_MIN);
	util_max = uclamp_eff_قدر(p, UCLAMP_MAX);

	اگر (sched_feat(SIS_UTIL) && sd->shared) {
		/*
		 * Same nr_idle_scan hint as select_idle_cpu(), nr only limits
		 * the scan when not preferring an idle core.
		 */
		nr = READ_ONCE(sd->shared->nr_idle_جائزہ) + 1;
		/* overloaded domain is unlikely to have idle cpu/core */
		اگر (nr == 1)
			واپس -1;
	}

	for_each_cpu_wrap(cpu, cpus, ہدف) {
		bool preferred_core = !has_idle_core || is_core_idle(cpu);
		غیرعلامتی طویل cpu_cap = استعداد_کا(cpu);

		/*
		 * Stop when the nr_idle_scan is exhausted (mirrors
		 * select_idle_cpu() logic).
		 */
		اگر (!has_idle_core && --nr <= 0)
			واپس بہترین_cpu;

		اگر (!انتخاب_idle_cpu(cpu, p))
			جاری;

		سماتا = util_سماتا_cpu(task_util, util_min, util_max, cpu);

		/*
		 * Perfect fit: capacity satisfies util + uclamp and the CPU
		 * sits on a fully-idle SMT core, this is a !SMT system, or
		 * there is no idle core to find.
		 * Short-circuit the rank-based selection and return
		 * immediately.
		 */
		اگر (سماتا > 0 && preferred_core)
			واپس cpu;
		/*
		 * Only the min performance hint (i.e. uclamp_min) doesn't fit.
		 * Look for the CPU with best capacity.
		 */
		ورنہ اگر (سماتا < 0)
			cpu_cap = get_actual_cpu_استعداد(cpu);
		/*
		 * fits > 0 implies we are not on a preferred core, but the util
		 * fits CPU capacity. Set fits to ASYM_IDLE_THREAD_FITS
		 * so the effective range becomes
		 * [ASYM_IDLE_THREAD_FITS, ASYM_IDLE_THREAD_MISFIT], where:
		 *    ASYM_IDLE_THREAD_MISFIT - does not fit
		 *    ASYM_IDLE_THREAD_UCLAMP_MISFIT - fits with the exception of UCLAMP_MIN
		 *    ASYM_IDLE_THREAD_FITS - fits with the exception of preferred_core
		 */
		ورنہ اگر (سماتا > 0)
			سماتا = ASYM_IDLE_THREAD_سماتا;

		/*
		 * If we are on a preferred core, translate the range of fits
		 * of [ASYM_IDLE_THREAD_UCLAMP_MISFIT, ASYM_IDLE_THREAD_MISFIT] to
		 * [ASYM_IDLE_UCLAMP_MISFIT, ASYM_IDLE_COMPLETE_MISFIT].
		 * This ensures that an idle core is always given priority over
		 * (partially) busy core.
		 *
		 * A fully fitting idle core would have returned early and hence
		 * fits > 0 for preferred_core need not be dealt with.
		 */
		اگر (preferred_core)
			سماتا += ASYM_IDLE_CORE_BIAS;

		/*
		 * First, select CPU which fits better (lower is more preferred).
		 * Then, select the one with best capacity at same level.
		 */
		اگر ((سماتا < بہترین_سماتا) ||
		    ((سماتا == بہترین_سماتا) && (cpu_cap > بہترین_cap))) {
			بہترین_cap = cpu_cap;
			بہترین_cpu = cpu;
			بہترین_سماتا = سماتا;
		}
	}

	/*
	 * A value in the [ASYM_IDLE_UCLAMP_MISFIT, ASYM_IDLE_COMPLETE_MISFIT]
	 * range means the chosen CPU is in a fully idle SMT core. Values above
	 * ASYM_IDLE_COMPLETE_MISFIT mean we never ranked such a CPU best.
	 *
	 * The asym-capacity wakeup path returns from select_idle_sibling()
	 * after this function and never runs select_idle_cpu(), so the usual
	 * select_idle_cpu() tail that clears idle cores must live here when the
	 * idle-core preference did not win.
	 */
	اگر (has_idle_core && بہترین_سماتا > ASYM_IDLE_COMPLETE_MISFIT)
		سیٹ_idle_cores(ہدف, false);

	واپس بہترین_cpu;
}

ساکن اندرونی bool asym_سماتا_cpu(غیرعلامتی طویل util,
				 غیرعلامتی طویل util_min,
				 غیرعلامتی طویل util_max,
				 صحیح cpu)
{
	اگر (sched_asym_cpucap_فعال()) {
		/*
		 * Return true only if the cpu fully fits the task requirements
		 * which include the utilization and the performance hints.
		 *
		 * When SMT is active, also require that the core has no busy
		 * siblings.
		 *
		 * Note: gating on is_core_idle() also makes the early-bailout
		 * candidates in select_idle_sibling() (target, prev,
		 * recent_used_cpu) idle-core-aware on ASYM+SMT, which the
		 * NO_ASYM path does not do.
		 */
		واپس (!sched_smt_فعال() || is_core_idle(cpu)) &&
		       (util_سماتا_cpu(util, util_min, util_max, cpu) > 0);
	}

	واپس true;
}

/*
 * Try and locate an idle core/thread in the LLC cache domain.
 */
ساکن صحیح select_idle_sibling(ساخت task_struct *p, صحیح پچھلا, صحیح ہدف)
{
	bool has_idle_core = false;
	ساخت sched_domain *sd;
	غیرعلامتی طویل task_util, util_min, util_max;
	صحیح i, recent_مستعمل_cpu, پچھلا_aff = -1;

	/*
	 * On asymmetric system, update task utilization because we will check
	 * that the task fits with CPU's capacity.
	 */
	اگر (sched_asym_cpucap_فعال()) {
		sync_entity_بھرو_avg(&p->se);
		task_util = task_util_est(p);
		util_min = uclamp_eff_قدر(p, UCLAMP_MIN);
		util_max = uclamp_eff_قدر(p, UCLAMP_MAX);
	}

	/*
	 * per-cpu select_rq_mask usage
	 */
	lockdep_assert_irqs_disabled();

	اگر (انتخاب_idle_cpu(ہدف, p) &&
	    asym_سماتا_cpu(task_util, util_min, util_max, ہدف))
		جاؤ select_smt_ترجیح;

	/*
	 * If the previous CPU is cache affine and idle, don't be stupid:
	 */
	اگر (پچھلا != ہدف && cpus_share_cache(پچھلا, ہدف) &&
	    انتخاب_idle_cpu(پچھلا, p) &&
	    asym_سماتا_cpu(task_util, util_min, util_max, پچھلا)) {

		اگر (!static_branch_unlikely(&sched_cluster_فعال) ||
		    cpus_share_resources(پچھلا, ہدف)) {
			ہدف = پچھلا;
			جاؤ select_smt_ترجیح;
		}

		پچھلا_aff = پچھلا;
	}

	/*
	 * Allow a per-cpu kthread to stack with the wakee if the
	 * kworker thread and the tasks previous CPUs are the same.
	 * The assumption is that the wakee queued work for the
	 * per-cpu kthread that is now complete and the wakeup is
	 * essentially a sync wakeup. An obvious example of this
	 * pattern is IO completions.
	 */
	اگر (is_فی_cpu_kthread(current) &&
	    اندر_task() &&
	    پچھلا == smp_processor_id() &&
	    this_rq()->nr_running <= 1 &&
	    asym_سماتا_cpu(task_util, util_min, util_max, پچھلا)) {
		ہدف = پچھلا;
		جاؤ select_smt_ترجیح;
	}

	/* Check a recently used CPU as a potential idle candidate: */
	recent_مستعمل_cpu = p->recent_مستعمل_cpu;
	p->recent_مستعمل_cpu = پچھلا;
	اگر (recent_مستعمل_cpu != پچھلا &&
	    recent_مستعمل_cpu != ہدف &&
	    cpus_share_cache(recent_مستعمل_cpu, ہدف) &&
	    انتخاب_idle_cpu(recent_مستعمل_cpu, p) &&
	    cpumask_test_cpu(recent_مستعمل_cpu, p->cpus_ptr) &&
	    asym_سماتا_cpu(task_util, util_min, util_max, recent_مستعمل_cpu)) {

		اگر (!static_branch_unlikely(&sched_cluster_فعال) ||
		    cpus_share_resources(recent_مستعمل_cpu, ہدف)) {
			ہدف = recent_مستعمل_cpu;
			جاؤ select_smt_ترجیح;
		}

	} ورنہ {
		recent_مستعمل_cpu = -1;
	}

	/*
	 * For asymmetric CPU capacity systems, our domain of interest is
	 * sd_asym_cpucapacity rather than sd_llc.
	 */
	اگر (sched_asym_cpucap_فعال()) {
		sd = rcu_dereference_all(فی_cpu(sd_asym_cpucapacity, ہدف));
		/*
		 * On an asymmetric CPU capacity system where an exclusive
		 * cpuset defines a symmetric island (i.e. one unique
		 * capacity_orig value through the cpuset), the key will be set
		 * but the CPUs within that cpuset will not have a domain with
		 * SD_ASYM_CPUCAPACITY. These should follow the usual symmetric
		 * capacity path.
		 */
		اگر (sd) {
			i = select_idle_استعداد(p, sd, ہدف);
			اگر ((غیرعلامتی صحیح)i < nr_cpumask_بٹ) {
				ہدف = i;
				جاؤ select_smt_ترجیح;
			}
			واپس ہدف;
		}
	}

	sd = rcu_dereference_all(فی_cpu(sd_llc, ہدف));
	اگر (!sd)
		واپس ہدف;

	اگر (sched_smt_فعال()) {
		has_idle_core = test_idle_cores(ہدف);

		اگر (!has_idle_core && cpus_share_cache(پچھلا, ہدف)) {
			i = select_idle_smt(p, sd, پچھلا);
			اگر ((غیرعلامتی صحیح)i < nr_cpumask_بٹ) {
				ہدف = i;
				جاؤ select_smt_ترجیح;
			}
		}
	}

	i = select_idle_cpu(p, sd, has_idle_core, ہدف);
	اگر ((غیرعلامتی صحیح)i < nr_cpumask_بٹ) {
		ہدف = i;
		جاؤ select_smt_ترجیح;
	}

	/*
	 * For cluster machines which have lower sharing cache like L2 or
	 * LLC Tag, we tend to find an idle CPU in the target's cluster
	 * first. But prev_cpu or recent_used_cpu may also be a good candidate,
	 * use them if possible when no idle CPU found in select_idle_cpu().
	 */
	اگر ((غیرعلامتی صحیح)پچھلا_aff < nr_cpumask_بٹ) {
		ہدف = پچھلا_aff;
		جاؤ select_smt_ترجیح;
	}
	اگر ((غیرعلامتی صحیح)recent_مستعمل_cpu < nr_cpumask_بٹ) {
		ہدف = recent_مستعمل_cpu;
		جاؤ select_smt_ترجیح;
	}

	واپس ہدف;

select_smt_ترجیح:
	واپس select_idle_smt_cpu(p, ہدف);
}

/**
 * cpu_util() - Estimates the amount of CPU capacity used by CFS tasks.
 * @cpu: the CPU to get the utilization for
 * @p: task for which the CPU utilization should be predicted or NULL
 * @dst_cpu: CPU @p migrates to, -1 if @p moves from @cpu or @p == NULL
 * @boost: 1 to enable boosting, otherwise 0
 *
 * The unit of the return value must be the same as the one of CPU capacity
 * so that CPU utilization can be compared with CPU capacity.
 *
 * CPU utilization is the sum of running time of runnable tasks plus the
 * recent utilization of currently non-runnable tasks on that CPU.
 * It represents the amount of CPU capacity currently used by CFS tasks in
 * the range [0..max CPU capacity] with max CPU capacity being the CPU
 * capacity at f_max.
 *
 * The estimated CPU utilization is defined as the maximum between CPU
 * utilization and sum of the estimated utilization of the currently
 * runnable tasks on that CPU. It preserves a utilization "snapshot" of
 * previously-executed tasks, which helps better deduce how busy a CPU will
 * be when a long-sleeping task wakes up. The contribution to CPU utilization
 * of such a task would be significantly decayed at this point of time.
 *
 * Boosted CPU utilization is defined as max(CPU runnable, CPU utilization).
 * CPU contention for CFS tasks can be detected by CPU runnable > CPU
 * utilization. Boosting is implemented in cpu_util() so that internal
 * users (e.g. EAS) can use it next to external users (e.g. schedutil),
 * latter via cpu_util_cfs_boost().
 *
 * CPU utilization can be higher than the current CPU capacity
 * (f_curr/f_max * max CPU capacity) or even the max CPU capacity because
 * of rounding errors as well as task migrations or wakeups of new tasks.
 * CPU utilization has to be capped to fit into the [0..max CPU capacity]
 * range. Otherwise a group of CPUs (CPU0 util = 121% + CPU1 util = 80%)
 * could be seen as over-utilized even though CPU1 has 20% of spare CPU
 * capacity. CPU utilization is allowed to overshoot current CPU capacity
 * though since this is useful for predicting the CPU capacity required
 * after task migrations (scheduler-driven DVFS).
 *
 * Return: (Boosted) (estimated) utilization for the specified CPU.
 */
ساکن غیرعلامتی طویل
cpu_util(صحیح cpu, ساخت task_struct *p, صحیح dst_cpu, صحیح boost)
{
	bool جوڑو_task = p && task_cpu(p) != cpu && dst_cpu == cpu;
	bool sub_task = p && task_cpu(p) == cpu && dst_cpu != cpu;
	ساخت cfs_rq *cfs_rq = &cpu_rq(cpu)->cfs;
	غیرعلامتی طویل util = READ_ONCE(cfs_rq->avg.util_avg);
	غیرعلامتی طویل runnable;

	/*
	 * If @dst_cpu is -1 or @p migrates from @cpu to @dst_cpu remove its
	 * contribution. If @p migrates from another CPU to @cpu add its
	 * contribution. In all the other cases @cpu is not impacted by the
	 * migration so its util_avg is already correct.
	 */
	اگر (جوڑو_task)
		util += task_util(p);
	ورنہ اگر (sub_task)
		lsub_مثبت(&util, task_util(p));

	اگر (boost) {
		runnable = READ_ONCE(cfs_rq->avg.runnable_avg);
		اگر (جوڑو_task)
			runnable += READ_ONCE(p->se.avg.runnable_avg);
		ورنہ اگر (sub_task)
			lsub_مثبت(&runnable,
				      READ_ONCE(p->se.avg.runnable_avg));
		util = max(util, runnable);
	}

	اگر (sched_feat(UTIL_EST)) {
		غیرعلامتی طویل util_est;

		util_est = READ_ONCE(cfs_rq->avg.util_est);

		/*
		 * During wake-up @p isn't enqueued yet and doesn't contribute
		 * to any cpu_rq(cpu)->cfs.avg.util_est.
		 * If @dst_cpu == @cpu add it to "simulate" cpu_util after @p
		 * has been enqueued.
		 *
		 * During exec (@dst_cpu = -1) @p is enqueued and does
		 * contribute to cpu_rq(cpu)->cfs.util_est.
		 * Remove it to "simulate" cpu_util without @p's contribution.
		 *
		 * Despite the task_on_rq_queued(@p) check there is still a
		 * small window for a possible race when an exec
		 * select_task_rq_fair() races with LB's detach_task().
		 *
		 *   detach_task()
		 *     deactivate_task()
		 *       p->on_rq = TASK_ON_RQ_MIGRATING;
		 *       -------------------------------- A
		 *       dequeue_task()                    \
		 *         dequeue_task_fair()              + Race Time
		 *           util_est_dequeue()            /
		 *       -------------------------------- B
		 *
		 * The additional check "current == p" is required to further
		 * reduce the race window.
		 */
		اگر (dst_cpu == cpu)
			util_est += _task_util_est(p);
		ورنہ اگر (p && unlikely(task_on_rq_queued(p) || current == p))
			lsub_مثبت(&util_est, _task_util_est(p));

		util = max(util, util_est);
	}

	واپس min(util, arch_scale_cpu_استعداد(cpu));
}

غیرعلامتی طویل cpu_util_cfs(صحیح cpu)
{
	واپس cpu_util(cpu, تہی, -1, 0);
}

غیرعلامتی طویل cpu_util_cfs_boost(صحیح cpu)
{
	واپس cpu_util(cpu, تہی, -1, 1);
}

/*
 * cpu_util_without: compute cpu utilization without any contributions from *p
 * @cpu: the CPU which utilization is requested
 * @p: the task which utilization should be discounted
 *
 * The utilization of a CPU is defined by the utilization of tasks currently
 * enqueued on that CPU as well as tasks which are currently sleeping after an
 * execution on that CPU.
 *
 * This method returns the utilization of the specified CPU by discounting the
 * utilization of the specified task, whenever the task is currently
 * contributing to the CPU utilization.
 */
ساکن غیرعلامتی طویل cpu_util_without(صحیح cpu, ساخت task_struct *p)
{
	/* Task has no contribution or is new */
	اگر (cpu != task_cpu(p) || !READ_ONCE(p->se.avg.آخری_update_وقت))
		p = تہی;

	واپس cpu_util(cpu, p, -1, 0);
}

/*
 * This function computes an effective utilization for the given CPU, to be
 * used for frequency selection given the linear relation: f = u * f_max.
 *
 * The scheduler tracks the following metrics:
 *
 *   cpu_util_{cfs,rt,dl,irq}()
 *   cpu_bw_dl()
 *
 * Where the cfs,rt and dl util numbers are tracked with the same metric and
 * synchronized windows and are thus directly comparable.
 *
 * The cfs,rt,dl utilization are the running times measured with rq->clock_task
 * which excludes things like IRQ and steal-time. These latter are then accrued
 * in the IRQ utilization.
 *
 * The DL bandwidth number OTOH is not a measured metric but a value computed
 * based on the task model parameters and gives the minimal utilization
 * required to meet deadlines.
 */
غیرعلامتی طویل effective_cpu_util(صحیح cpu, غیرعلامتی طویل util_cfs,
				 غیرعلامتی طویل *min,
				 غیرعلامتی طویل *max)
{
	غیرعلامتی طویل util, irq, scale;
	ساخت rq *rq = cpu_rq(cpu);

	scale = arch_scale_cpu_استعداد(cpu);

	/*
	 * Early check to see if IRQ/steal time saturates the CPU, can be
	 * because of inaccuracies in how we track these -- see
	 * update_irq_load_avg().
	 */
	irq = cpu_util_irq(rq);
	اگر (unlikely(irq >= scale)) {
		اگر (min)
			*min = scale;
		اگر (max)
			*max = scale;
		واپس scale;
	}

	اگر (min) {
		/*
		 * The minimum utilization returns the highest level between:
		 * - the computed DL bandwidth needed with the IRQ pressure which
		 *   steals time to the deadline task.
		 * - The minimum performance requirement for CFS and/or RT.
		 */
		*min = max(irq + cpu_bw_dl(rq), uclamp_rq_get(rq, UCLAMP_MIN));

		/*
		 * When an RT task is runnable and uclamp is not used, we must
		 * ensure that the task will run at maximum compute capacity.
		 */
		اگر (!uclamp_is_مستعمل() && rt_rq_is_runnable(&rq->rt))
			*min = max(*min, scale);
	}

	/*
	 * Because the time spend on RT/DL tasks is visible as 'lost' time to
	 * CFS tasks and we use the same metric to track the effective
	 * utilization (PELT windows are synchronized) we can directly add them
	 * to obtain the CPU's actual utilization.
	 */
	util = util_cfs + cpu_util_rt(rq);
	util += cpu_util_dl(rq);

	/*
	 * The maximum hint is a soft bandwidth requirement, which can be lower
	 * than the actual utilization because of uclamp_max requirements.
	 */
	اگر (max)
		*max = min(scale, uclamp_rq_get(rq, UCLAMP_MAX));

	اگر (util >= scale)
		واپس scale;

	/*
	 * There is still idle time; further improve the number by using the
	 * IRQ metric. Because IRQ/steal time is hidden from the task clock we
	 * need to scale the task numbers:
	 *
	 *              max - irq
	 *   U' = irq + --------- * U
	 *                 max
	 */
	util = scale_irq_استعداد(util, irq, scale);
	util += irq;

	واپس min(scale, util);
}

غیرعلامتی طویل sched_cpu_util(صحیح cpu)
{
	واپس effective_cpu_util(cpu, cpu_util_cfs(cpu), تہی, تہی);
}

/*
 * energy_env - Utilization landscape for energy estimation.
 * @task_busy_time: Utilization contribution by the task for which we test the
 *                  placement. Given by eenv_task_busy_time().
 * @pd_busy_time:   Utilization of the whole perf domain without the task
 *                  contribution. Given by eenv_pd_busy_time().
 * @cpu_cap:        Maximum CPU capacity for the perf domain.
 * @pd_cap:         Entire perf domain capacity. (pd->nr_cpus * cpu_cap).
 */
ساخت energy_env {
	غیرعلامتی طویل task_busy_وقت;
	غیرعلامتی طویل pd_busy_وقت;
	غیرعلامتی طویل cpu_cap;
	غیرعلامتی طویل pd_cap;
};

/*
 * Compute the task busy time for compute_energy(). This time cannot be
 * injected directly into effective_cpu_util() because of the IRQ scaling.
 * The latter only makes sense with the most recent CPUs where the task has
 * run.
 */
ساکن اندرونی خالی eenv_task_busy_وقت(ساخت energy_env *eenv,
				       ساخت task_struct *p, صحیح پچھلا_cpu)
{
	غیرعلامتی طویل busy_وقت, max_cap = arch_scale_cpu_استعداد(پچھلا_cpu);
	غیرعلامتی طویل irq = cpu_util_irq(cpu_rq(پچھلا_cpu));

	اگر (unlikely(irq >= max_cap))
		busy_وقت = max_cap;
	ورنہ
		busy_وقت = scale_irq_استعداد(task_util_est(p), irq, max_cap);

	eenv->task_busy_وقت = busy_وقت;
}

/*
 * Compute the perf_domain (PD) busy time for compute_energy(). Based on the
 * utilization for each @pd_cpus, it however doesn't take into account
 * clamping since the ratio (utilization / cpu_capacity) is already enough to
 * scale the EM reported power consumption at the (eventually clamped)
 * cpu_capacity.
 *
 * The contribution of the task @p for which we want to estimate the
 * energy cost is removed (by cpu_util()) and must be calculated
 * separately (see eenv_task_busy_time). This ensures:
 *
 *   - A stable PD utilization, no matter which CPU of that PD we want to place
 *     the task on.
 *
 *   - A fair comparison between CPUs as the task contribution (task_util())
 *     will always be the same no matter which CPU utilization we rely on
 *     (util_avg or util_est).
 *
 * Set @eenv busy time for the PD that spans @pd_cpus. This busy time can't
 * exceed @eenv->pd_cap.
 */
ساکن اندرونی خالی eenv_pd_busy_وقت(ساخت energy_env *eenv,
				     ساخت cpumask *pd_cpus,
				     ساخت task_struct *p)
{
	غیرعلامتی طویل busy_وقت = 0;
	صحیح cpu;

	for_each_cpu(cpu, pd_cpus) {
		غیرعلامتی طویل util = cpu_util(cpu, p, -1, 0);

		busy_وقت += effective_cpu_util(cpu, util, تہی, تہی);
	}

	eenv->pd_busy_وقت = min(eenv->pd_cap, busy_وقت);
}

/*
 * Compute the maximum utilization for compute_energy() when the task @p
 * is placed on the cpu @dst_cpu.
 *
 * Returns the maximum utilization among @eenv->cpus. This utilization can't
 * exceed @eenv->cpu_cap.
 */
ساکن اندرونی غیرعلامتی طویل
eenv_pd_max_util(ساخت energy_env *eenv, ساخت cpumask *pd_cpus,
		 ساخت task_struct *p, صحیح dst_cpu)
{
	غیرعلامتی طویل max_util = 0;
	صحیح cpu;

	for_each_cpu(cpu, pd_cpus) {
		ساخت task_struct *tsk = (cpu == dst_cpu) ? p : تہی;
		غیرعلامتی طویل util = cpu_util(cpu, p, dst_cpu, 1);
		غیرعلامتی طویل eff_util, min, max;

		/*
		 * Performance domain frequency: utilization clamping
		 * must be considered since it affects the selection
		 * of the performance domain frequency.
		 * NOTE: in case RT tasks are running, by default the min
		 * utilization can be max OPP.
		 */
		eff_util = effective_cpu_util(cpu, util, &min, &max);

		/* Task's uclamp can modify min and max value */
		اگر (tsk && uclamp_is_مستعمل()) {
			min = max(min, uclamp_eff_قدر(p, UCLAMP_MIN));

			/*
			 * If there is no active max uclamp constraint,
			 * directly use task's one, otherwise keep max.
			 */
			اگر (uclamp_rq_is_idle(cpu_rq(cpu)))
				max = uclamp_eff_قدر(p, UCLAMP_MAX);
			ورنہ
				max = max(max, uclamp_eff_قدر(p, UCLAMP_MAX));
		}

		eff_util = sugov_effective_cpu_perf(cpu, eff_util, min, max);
		max_util = max(max_util, eff_util);
	}

	واپس min(max_util, eenv->cpu_cap);
}

/*
 * compute_energy(): Use the Energy Model to estimate the energy that @pd would
 * consume for a given utilization landscape @eenv. When @dst_cpu < 0, the task
 * contribution is ignored.
 */
ساکن اندرونی غیرعلامتی طویل
compute_energy(ساخت energy_env *eenv, ساخت perf_domain *pd,
	       ساخت cpumask *pd_cpus, ساخت task_struct *p, صحیح dst_cpu)
{
	غیرعلامتی طویل max_util = eenv_pd_max_util(eenv, pd_cpus, p, dst_cpu);
	غیرعلامتی طویل busy_وقت = eenv->pd_busy_وقت;
	غیرعلامتی طویل energy;

	اگر (dst_cpu >= 0)
		busy_وقت = min(eenv->pd_cap, busy_وقت + eenv->task_busy_وقت);

	energy = em_cpu_energy(pd->em_pd, max_util, busy_وقت, eenv->cpu_cap);

	trace_sched_compute_energy_tp(p, dst_cpu, energy, max_util, busy_وقت);

	واپس energy;
}

/*
 * find_energy_efficient_cpu(): Find most energy-efficient target CPU for the
 * waking task. find_energy_efficient_cpu() looks for the CPU with maximum
 * spare capacity in each performance domain and uses it as a potential
 * candidate to execute the task. Then, it uses the Energy Model to figure
 * out which of the CPU candidates is the most energy-efficient.
 *
 * The rationale for this heuristic is as follows. In a performance domain,
 * all the most energy efficient CPU candidates (according to the Energy
 * Model) are those for which we'll request a low frequency. When there are
 * several CPUs for which the frequency request will be the same, we don't
 * have enough data to break the tie between them, because the Energy Model
 * only includes active power costs. With this model, if we assume that
 * frequency requests follow utilization (e.g. using schedutil), the CPU with
 * the maximum spare capacity in a performance domain is guaranteed to be among
 * the best candidates of the performance domain.
 *
 * In practice, it could be preferable from an energy standpoint to pack
 * small tasks on a CPU in order to let other CPUs go in deeper idle states,
 * but that could also hurt our chances to go cluster idle, and we have no
 * ways to tell with the current Energy Model if this is actually a good
 * idea or not. So, find_energy_efficient_cpu() basically favors
 * cluster-packing, and spreading inside a cluster. That should at least be
 * a good thing for latency, and this is consistent with the idea that most
 * of the energy savings of EAS come from the asymmetry of the system, and
 * not so much from breaking the tie between identical CPUs. That's also the
 * reason why EAS is enabled in the topology code only for systems where
 * SD_ASYM_CPUCAPACITY is set.
 *
 * NOTE: Forkees are not accepted in the energy-aware wake-up path because
 * they don't have any useful utilization data yet and it's not possible to
 * forecast their impact on energy consumption. Consequently, they will be
 * placed by sched_balance_find_dst_cpu() on the least loaded CPU, which might turn out
 * to be energy-inefficient in some use-cases. The alternative would be to
 * bias new tasks towards specific types of CPUs first, or to try to infer
 * their util_avg from the parent task, but those heuristics could hurt
 * other use-cases too. So, until someone finds a better way to solve this,
 * let's keep things simple by re-using the existing slow path.
 */
ساکن صحیح ڈھونڈو_energy_efficient_cpu(ساخت task_struct *p, صحیح پچھلا_cpu)
{
	ساخت cpumask *cpus = this_cpu_cpumask_var_ptr(select_rq_نقاب);
	غیرعلامتی طویل پچھلا_delta = ULONG_MAX, بہترین_delta = ULONG_MAX;
	غیرعلامتی طویل p_util_min = uclamp_is_مستعمل() ? uclamp_eff_قدر(p, UCLAMP_MIN) : 0;
	غیرعلامتی طویل p_util_max = uclamp_is_مستعمل() ? uclamp_eff_قدر(p, UCLAMP_MAX) : 1024;
	ساخت جڑ_domain *rd = this_rq()->rd;
	صحیح cpu, بہترین_energy_cpu, ہدف = -1;
	صحیح پچھلا_سماتا = -1, بہترین_سماتا = -1;
	غیرعلامتی طویل بہترین_actual_cap = 0;
	غیرعلامتی طویل پچھلا_actual_cap = 0;
	ساخت sched_domain *sd;
	ساخت perf_domain *pd;
	ساخت energy_env eenv;

	pd = rcu_dereference_all(rd->pd);
	اگر (!pd)
		واپس ہدف;

	/*
	 * Energy-aware wake-up happens on the lowest sched_domain starting
	 * from sd_asym_cpucapacity spanning over this_cpu and prev_cpu.
	 */
	sd = rcu_dereference_all(*this_cpu_ptr(&sd_asym_cpucapacity));
	جب (sd && !cpumask_test_cpu(پچھلا_cpu, sched_domain_وسعت(sd)))
		sd = sd->والد;
	اگر (!sd)
		واپس ہدف;

	ہدف = پچھلا_cpu;

	sync_entity_بھرو_avg(&p->se);
	اگر (!task_util_est(p) && p_util_min == 0)
		واپس ہدف;

	eenv_task_busy_وقت(&eenv, p, پچھلا_cpu);

	برائے (; pd; pd = pd->اگلا) {
		غیرعلامتی طویل util_min = p_util_min, util_max = p_util_max;
		غیرعلامتی طویل cpu_cap, cpu_actual_cap, util;
		طویل پچھلا_فالتو_cap = -1, max_فالتو_cap = -1;
		غیرعلامتی طویل rq_util_min, rq_util_max;
		غیرعلامتی طویل cur_delta, بنیاد_energy;
		صحیح max_فالتو_cap_cpu = -1;
		صحیح سماتا, max_سماتا = -1;

		اگر (!cpumask_and(cpus, perf_domain_وسعت(pd), cpu_online_نقاب))
			جاری;

		/* Account external pressure for the energy estimation */
		cpu = cpumask_پہلا(cpus);
		cpu_actual_cap = get_actual_cpu_استعداد(cpu);

		eenv.cpu_cap = cpu_actual_cap;
		eenv.pd_cap = 0;

		for_each_cpu(cpu, cpus) {
			ساخت rq *rq = cpu_rq(cpu);

			eenv.pd_cap += cpu_actual_cap;

			اگر (!cpumask_test_cpu(cpu, sched_domain_وسعت(sd)))
				جاری;

			اگر (!cpumask_test_cpu(cpu, p->cpus_ptr))
				جاری;

			util = cpu_util(cpu, p, cpu, 0);
			cpu_cap = استعداد_کا(cpu);

			/*
			 * Skip CPUs that cannot satisfy the capacity request.
			 * IOW, placing the task there would make the CPU
			 * overutilized. Take uclamp into account to see how
			 * much capacity we can get out of the CPU; this is
			 * aligned with sched_cpu_util().
			 */
			اگر (uclamp_is_مستعمل() && !uclamp_rq_is_idle(rq)) {
				/*
				 * Open code uclamp_rq_util_with() except for
				 * the clamp() part. I.e.: apply max aggregation
				 * only. util_fits_cpu() logic requires to
				 * operate on non clamped util but must use the
				 * max-aggregated uclamp_{min, max}.
				 */
				rq_util_min = uclamp_rq_get(rq, UCLAMP_MIN);
				rq_util_max = uclamp_rq_get(rq, UCLAMP_MAX);

				util_min = max(rq_util_min, p_util_min);
				util_max = max(rq_util_max, p_util_max);
			}

			سماتا = util_سماتا_cpu(util, util_min, util_max, cpu);
			اگر (!سماتا)
				جاری;

			lsub_مثبت(&cpu_cap, util);

			اگر (cpu == پچھلا_cpu) {
				/* Always use prev_cpu as a candidate. */
				پچھلا_فالتو_cap = cpu_cap;
				پچھلا_سماتا = سماتا;
			} ورنہ اگر ((سماتا > max_سماتا) ||
				   ((سماتا == max_سماتا) && ((طویل)cpu_cap > max_فالتو_cap))) {
				/*
				 * Find the CPU with the maximum spare capacity
				 * among the remaining CPUs in the performance
				 * domain.
				 */
				max_فالتو_cap = cpu_cap;
				max_فالتو_cap_cpu = cpu;
				max_سماتا = سماتا;
			}
		}

		اگر (max_فالتو_cap_cpu < 0 && پچھلا_فالتو_cap < 0)
			جاری;

		eenv_pd_busy_وقت(&eenv, cpus, p);
		/* Compute the 'base' energy of the pd, without @p */
		بنیاد_energy = compute_energy(&eenv, pd, cpus, p, -1);

		/* Evaluate the energy impact of using prev_cpu. */
		اگر (پچھلا_فالتو_cap > -1) {
			پچھلا_delta = compute_energy(&eenv, pd, cpus, p,
						    پچھلا_cpu);
			/* CPU utilization has changed */
			اگر (پچھلا_delta < بنیاد_energy)
				واپس ہدف;
			پچھلا_delta -= بنیاد_energy;
			پچھلا_actual_cap = cpu_actual_cap;
			بہترین_delta = min(بہترین_delta, پچھلا_delta);
		}

		/* Evaluate the energy impact of using max_spare_cap_cpu. */
		اگر (max_فالتو_cap_cpu >= 0 && max_فالتو_cap > پچھلا_فالتو_cap) {
			/* Current best energy cpu fits better */
			اگر (max_سماتا < بہترین_سماتا)
				جاری;

			/*
			 * Both don't fit performance hint (i.e. uclamp_min)
			 * but best energy cpu has better capacity.
			 */
			اگر ((max_سماتا < 0) &&
			    (cpu_actual_cap <= بہترین_actual_cap))
				جاری;

			cur_delta = compute_energy(&eenv, pd, cpus, p,
						   max_فالتو_cap_cpu);
			/* CPU utilization has changed */
			اگر (cur_delta < بنیاد_energy)
				واپس ہدف;
			cur_delta -= بنیاد_energy;

			/*
			 * Both fit for the task but best energy cpu has lower
			 * energy impact.
			 */
			اگر ((max_سماتا > 0) && (بہترین_سماتا > 0) &&
			    (cur_delta >= بہترین_delta))
				جاری;

			بہترین_delta = cur_delta;
			بہترین_energy_cpu = max_فالتو_cap_cpu;
			بہترین_سماتا = max_سماتا;
			بہترین_actual_cap = cpu_actual_cap;
		}
	}

	اگر ((بہترین_سماتا > پچھلا_سماتا) ||
	    ((بہترین_سماتا > 0) && (بہترین_delta < پچھلا_delta)) ||
	    ((بہترین_سماتا < 0) && (بہترین_actual_cap > پچھلا_actual_cap)))
		ہدف = بہترین_energy_cpu;

	واپس ہدف;
}

/*
 * select_task_rq_fair: Select target runqueue for the waking task in domains
 * that have the relevant SD flag set. In practice, this is SD_BALANCE_WAKE,
 * SD_BALANCE_FORK, or SD_BALANCE_EXEC.
 *
 * Balances load by selecting the idlest CPU in the idlest group, or under
 * certain conditions an idle sibling CPU if the domain has SD_WAKE_AFFINE set.
 *
 * Returns the target CPU number.
 */
ساکن صحیح
select_task_rq_fair(ساخت task_struct *p, صحیح پچھلا_cpu, صحیح wake_flags)
{
	صحیح sync = (wake_flags & WF_SYNC) && !(current->flags & PF_EXITING);
	ساخت sched_domain *tmp, *sd = تہی;
	صحیح cpu = smp_processor_id();
	صحیح new_cpu = پچھلا_cpu;
	صحیح want_affine = 0;
	/* SD_flags and WF_flags share the first nibble */
	صحیح sd_flag = wake_flags & 0xF;

	/*
	 * required for stable ->cpus_allowed
	 */
	lockdep_assert_held(&p->pi_تالا);
	اگر (wake_flags & WF_TTWU) {
		ریکارڈ_wakee(p);

		اگر ((wake_flags & WF_CURRENT_CPU) &&
		    cpumask_test_cpu(cpu, p->cpus_ptr))
			واپس cpu;

		اگر (!is_rd_overutilized(this_rq()->rd)) {
			new_cpu = ڈھونڈو_energy_efficient_cpu(p, پچھلا_cpu);
			اگر (new_cpu >= 0)
				واپس new_cpu;
			new_cpu = پچھلا_cpu;
		}

		want_affine = !wake_wide(p) && cpumask_test_cpu(cpu, p->cpus_ptr);
	}

	for_each_domain(cpu, tmp) {
		/*
		 * If both 'cpu' and 'prev_cpu' are part of this domain,
		 * cpu is a valid SD_WAKE_AFFINE target.
		 */
		اگر (want_affine && (tmp->flags & SD_WAKE_AFFINE) &&
		    cpumask_test_cpu(پچھلا_cpu, sched_domain_وسعت(tmp))) {
			اگر (cpu != پچھلا_cpu)
				new_cpu = wake_affine(tmp, p, cpu, پچھلا_cpu, sync);

			sd = تہی; /* Prefer wake_affine over balance flags */
			توڑو;
		}

		/*
		 * Usually only true for WF_EXEC and WF_FORK, as sched_domains
		 * usually do not have SD_BALANCE_WAKE set. That means wakeup
		 * will usually go to the fast path.
		 */
		اگر (tmp->flags & sd_flag)
			sd = tmp;
		ورنہ اگر (!want_affine)
			توڑو;
	}

	/* Slow path */
	اگر (unlikely(sd)) {
		new_cpu = sched_balance_ڈھونڈو_dst_cpu(sd, p, cpu, پچھلا_cpu, sd_flag);
		واپس select_idle_smt_cpu(p, new_cpu);
	}

	/* Fast path */
	اگر (wake_flags & WF_TTWU)
		واپس select_idle_sibling(p, پچھلا_cpu, new_cpu);

	واپس new_cpu;
}

/*
 * Called immediately before a task is migrated to a new CPU; task_cpu(p) and
 * cfs_rq_of(p) references at time of call are still valid and identify the
 * previous CPU. The caller guarantees p->pi_lock or task_rq(p)->lock is held.
 */
ساکن خالی migrate_task_rq_fair(ساخت task_struct *p, صحیح new_cpu)
{
	ساخت sched_entity *se = &p->se;

	اگر (!task_on_rq_migrating(p)) {
		remove_entity_بھرو_avg(se);

		/*
		 * Here, the task's PELT values have been updated according to
		 * the current rq's clock. But if that clock hasn't been
		 * updated in a while, a substantial idle time will be missed,
		 * leading to an inflation after wake-up on the new rq.
		 *
		 * Estimate the missing time from the cfs_rq last_update_time
		 * and update sched_avg to improve the PELT continuity after
		 * migration.
		 */
		migrate_se_pelt_lag(se);
	}

	/* Tell new CPU we are migrated */
	se->avg.آخری_update_وقت = 0;

	update_جائزہ_period(p, new_cpu);
}

ساکن خالی task_dead_fair(ساخت task_struct *p)
{
	ساخت sched_entity *se = &p->se;
	remove_entity_بھرو_avg(se);
}

/*
 * Set the max capacity the task is allowed to run at for misfit detection.
 */
ساکن خالی سیٹ_task_max_allowed_استعداد(ساخت task_struct *p)
{
	ساخت asym_cap_ڈیٹا *اندراج_2;

	اگر (!sched_asym_cpucap_فعال())
		واپس;

	rcu_read_تالا();
	list_for_each_اندراج_rcu(اندراج_2, &asym_cap_list, link) {
		cpumask_t *cpumask;

		cpumask = cpu_استعداد_وسعت(اندراج_2);
		اگر (!cpumask_intersects(p->cpus_ptr, cpumask))
			جاری;

		p->max_allowed_استعداد = اندراج_2->استعداد;
		توڑو;
	}
	rcu_read_unlock();
}

ساکن خالی سیٹ_cpus_allowed_fair(ساخت task_struct *p, ساخت affinity_context *ctx)
{
	سیٹ_cpus_allowed_common(p, ctx);
	سیٹ_task_max_allowed_استعداد(p);
}

شمار preempt_wakeup_action {
	PREEMPT_WAKEUP_NONE,	/* No preemption. */
	PREEMPT_WAKEUP_SHORT,	/* Ignore slice protection. */
	PREEMPT_WAKEUP_PICK,	/* Let pick_eevdf() decide. */
	PREEMPT_WAKEUP_RESCHED,	/* Force reschedule. */
};

ساکن اندرونی bool سیٹ_preempt_buddy(ساخت cfs_rq *cfs_rq, ساخت sched_entity *pse)
{
	/*
	 * Keep existing buddy if the deadline is sooner than pse.
	 * The older buddy may be cache cold and completely unrelated
	 * to the current wakeup but that is unpredictable where as
	 * obeying the deadline is more in line with EEVDF objectives.
	 */
	اگر (cfs_rq->اگلا && entity_before(cfs_rq->اگلا, pse))
		واپس false;

	سیٹ_اگلا_buddy(cfs_rq, pse);
	واپس true;
}

ساکن اندرونی bool سیٹ_short_buddy(ساخت cfs_rq *cfs_rq, ساخت sched_entity *pse)
{
	اگر (cfs_rq->اگلا) {
		اگر (cfs_rq->اگلا->قاش < pse->قاش)
			واپس false;

		اگر (cfs_rq->اگلا->قاش == pse->قاش &&
		    entity_before(cfs_rq->اگلا, pse))
			واپس false;
	}

	سیٹ_اگلا_buddy(cfs_rq, pse);
	واپس true;
}

/*
 * WF_SYNC|WF_TTWU indicates the waker expects to sleep but it is not
 * strictly enforced because the hint is either misunderstood or
 * multiple tasks must be woken up.
 */
ساکن اندرونی شمار preempt_wakeup_action
preempt_sync(ساخت rq *rq, صحیح wake_flags,
	     ساخت sched_entity *pse, ساخت sched_entity *se)
{
	u64 حد, delta;

	/*
	 * WF_SYNC without WF_TTWU is not expected so warn if it happens even
	 * though it is likely harmless.
	 */
	WARN_ON_ONCE(!(wake_flags & WF_TTWU));

	حد = sysctl_sched_migration_لاگت;
	delta = rq_clock_task(rq) - se->exec_آغاز;
	اگر ((s64)delta < 0)
		delta = 0;

	/*
	 * WF_RQ_SELECTED implies the tasks are stacking on a CPU when they
	 * could run on other CPUs. Reduce the threshold before preemption is
	 * allowed to an arbitrary lower value as it is more likely (but not
	 * guaranteed) the waker requires the wakee to finish.
	 */
	اگر (wake_flags & WF_RQ_SELECTED)
		حد >>= 2;

	/*
	 * As WF_SYNC is not strictly obeyed, allow some runtime for batch
	 * wakeups to be issued.
	 */
	اگر (entity_before(pse, se) && delta >= حد)
		واپس PREEMPT_WAKEUP_RESCHED;

	واپس PREEMPT_WAKEUP_NONE;
}

/*
 * Preempt the current task with a newly woken task if needed:
 */
ساکن خالی wakeup_preempt_fair(ساخت rq *rq, ساخت task_struct *p, صحیح wake_flags)
{
	شمار preempt_wakeup_action preempt_action = PREEMPT_WAKEUP_PICK;
	ساخت task_struct *donor = rq->donor;
	ساخت sched_entity *nse, *se = &donor->se, *pse = &p->se;
	ساخت cfs_rq *cfs_rq = &rq->cfs;
	صحیح cse_is_idle, pse_is_idle;

	/*
	 * XXX Getting preempted by higher class, try and find idle CPU?
	 */
	اگر (p->sched_class != &fair_sched_class ||
	    donor->sched_class != &fair_sched_class)
		واپس;

	اگر (unlikely(se == pse))
		واپس;

	/*
	 * This is possible from callers such as attach_tasks(), in which we
	 * unconditionally wakeup_preempt() after an enqueue (which may have
	 * lead to a throttle).  This both saves work and prevents false
	 * next-buddy nomination below.
	 */
	اگر (task_is_throttled(p))
		واپس;

	/*
	 * We can come here with TIF_NEED_RESCHED already set from new task
	 * wake up path.
	 *
	 * Note: this also catches the edge-case of curr being in a throttled
	 * group (e.g. via set_curr_task), since update_curr() (in the
	 * enqueue of curr) will have resulted in resched being set.  This
	 * prevents us from potentially nominating it as a false LAST_BUDDY
	 * below.
	 */
	اگر (!sched_feat(PREEMPT_SHORT) && test_tsk_need_resched(rq->curr))
		واپس;

	اگر (!sched_feat(WAKEUP_PREEMPTION))
		واپس;

	WARN_ON_ONCE(!pse);

	cse_is_idle = se_is_idle(se);
	pse_is_idle = se_is_idle(pse);

	nse = se;
	/*
	 * Preempt an idle entity in favor of a non-idle entity (and don't preempt
	 * in the inverse case).
	 */
	اگر (cse_is_idle && !pse_is_idle)
		جاؤ preempt;

	update_curr_fair(rq);

	اگر (cse_is_idle != pse_is_idle)
		جاؤ update;

	/*
	 * BATCH and IDLE tasks do not preempt others.
	 */
	اگر (unlikely(!معمول_policy(p->policy)))
		جاؤ update;

	/*
	 * Do not preempt for tasks that are sched_delayed as it would violate
	 * EEVDF to forcibly queue an ineligible task.
	 */
	اگر (pse->sched_delayed)
		جاؤ update;

	/*
	 * If @p has a shorter slice than current and @p is eligible, override
	 * current's slice protection in order to allow preemption.
	 */
	اگر (sched_feat(PREEMPT_SHORT) && (pse->قاش < se->قاش)) {
		preempt_action = PREEMPT_WAKEUP_SHORT;
		جاؤ pick;
	}

	/*
	 * Ignore wakee preemption on WF_FORK as it is less likely that
	 * there is shared data as exec often follow fork.
	 */
	اگر (wake_flags & WF_FORK)
		جاؤ update;

	/* Prefer picking wakee soon if appropriate. */
	اگر (sched_feat(اگلا_BUDDY) && سیٹ_preempt_buddy(cfs_rq, pse)) {
		/*
		 * Decide whether to obey WF_SYNC hint for a new buddy. Old
		 * buddies are ignored as they may not be relevant to the
		 * waker and less likely to be cache hot.
		 */
		اگر (wake_flags & WF_SYNC)
			preempt_action = preempt_sync(rq, wake_flags, pse, se);
	}

	انتخاب (preempt_action) {
	حالت PREEMPT_WAKEUP_NONE:
		واپس;
	حالت PREEMPT_WAKEUP_RESCHED:
		جاؤ preempt;
	حالت PREEMPT_WAKEUP_SHORT:
		fallthrough;
	حالت PREEMPT_WAKEUP_PICK:
		توڑو;
	}

pick:
	اگر (cfs_rq->h_nr_queued) {
		nse = pick_اگلا_entity(rq, preempt_action != PREEMPT_WAKEUP_SHORT);
		اگر (unlikely(!nse))
			جاؤ pick;

		/* If @p has become the most eligible task, force preemption */
		اگر (nse == pse)
			جاؤ preempt;
	}

	/*
	 * If @p is eligible but not the next task to run then cancel protection
	 * to prevent large scheduling latency
	 */
	اگر (preempt_action == PREEMPT_WAKEUP_SHORT && entity_eligible(cfs_rq, pse))
		جاؤ preempt;
update:
	اگر (sched_feat(تسلسل_مقصد_PARITY))
		update_protect_قاش(cfs_rq, se);

	واپس;

preempt:
	cancel_protect_قاش(se);

	اگر (preempt_action == PREEMPT_WAKEUP_SHORT)
		سیٹ_short_buddy(cfs_rq, pse);

	resched_curr_lazy(rq);
}

ساخت task_struct *pick_task_fair(ساخت rq *rq, ساخت rq_flags *rf)
	__must_hold(__rq_lockp(rq))
{
	ساخت cfs_rq *cfs_rq = &rq->cfs;
	ساخت sched_entity *se;
	ساخت task_struct *p;
	صحیح new_tasks;

again:
	اگر (!cfs_rq->h_nr_queued)
		جاؤ idle;

	/* Might not have done put_prev_entity() */
	اگر (cfs_rq->curr && cfs_rq->curr->on_rq)
		update_curr_eevdf(cfs_rq);

	se = pick_اگلا_entity(rq, true);
	اگر (!se)
		جاؤ again;

	p = task_کا(se);
	واپس p;

idle:
	اگر (sched_core_enabled(rq))
		واپس تہی;

	new_tasks = sched_balance_newidle(rq, rf);
	اگر (new_tasks < 0)
		واپس RETRY_TASK;
	اگر (new_tasks > 0)
		جاؤ again;
	واپس تہی;
}

ساکن ساخت task_struct *
fair_server_pick_task(ساخت sched_dl_entity *dl_se, ساخت rq_flags *rf)
	__must_hold(__rq_lockp(dl_se->rq))
{
	واپس pick_task_fair(dl_se->rq, rf);
}

خالی fair_server_init(ساخت rq *rq)
{
	ساخت sched_dl_entity *dl_se = &rq->fair_server;

	init_dl_entity(dl_se);

	dl_server_init(dl_se, rq, fair_server_pick_task);
}

/*
 * Account for a descheduled task:
 */
ساکن خالی put_پچھلا_task_fair(ساخت rq *rq, ساخت task_struct *پچھلا, ساخت task_struct *اگلا)
{
	ساخت sched_entity *se = &پچھلا->se;
	ساخت cfs_rq *cfs_rq = &rq->cfs;
	ساخت sched_entity *nse = تہی;

#اگر_تعریف_شدہ CONFIG_FAIR_گروہ_SCHED
	اگر (اگلا && اگلا->sched_class == &fair_sched_class)
		nse = &اگلا->se;
#اگر_ختم

	جب (se) {
		cfs_rq = cfs_rq_کا(se);
		اگر (!nse || cfs_rq->h_curr)
			put_پچھلا_entity(cfs_rq, se);
#اگر_تعریف_شدہ CONFIG_FAIR_گروہ_SCHED
		اگر (nse) {
			اگر (is_same_گروہ(se, nse))
				توڑو;

			صحیح d = nse->گہرائی - se->گہرائی;
			اگر (d >= 0) {
				/* nse has equal or greater depth, ascend */
				nse = والد_entity(nse);
				/* if nse is the deeper, do not ascend se */
				اگر (d > 0)
					جاری;
			}
		}
#اگر_ختم
		se = والد_entity(se);
	}

	/* Put 'current' back into the tree. */
	cfs_rq = &rq->cfs;
	se = &پچھلا->se;
	WARN_ON_ONCE(cfs_rq->curr != se);
	cfs_rq->curr = تہی;
	اگر (se->on_rq)
		__enqueue_entity(cfs_rq, se);
}

/*
 * sched_yield() is very simple
 */
ساکن خالی yield_task_fair(ساخت rq *rq)
{
	ساخت task_struct *curr = rq->donor;
	ساخت sched_entity *se = &curr->se;
	ساخت cfs_rq *cfs_rq = &rq->cfs;

	/*
	 * Are we the only task in the tree?
	 */
	اگر (unlikely(rq->nr_running == 1))
		واپس;

	clear_buddies(cfs_rq, se);

	update_rq_clock(rq);
	/*
	 * Update run-time statistics of the 'current'.
	 */
	update_curr_eevdf(cfs_rq);
	/*
	 * Tell update_rq_clock() that we've just updated,
	 * so we don't do microscopic update in schedule()
	 * and double the fastpath cost.
	 */
	rq_clock_skip_update(rq);

	/*
	 * Forfeit the remaining vruntime, only if the entity is eligible. This
	 * condition is necessary because in core scheduling we prefer to run
	 * ineligible tasks rather than force idling. If this happens we may
	 * end up in a loop where the core scheduler picks the yielding task,
	 * which yields immediately again; without the condition the vruntime
	 * ends up quickly running away.
	 */
	اگر (entity_eligible(cfs_rq, se)) {
		se->vruntime = se->deadline;
		update_deadline(cfs_rq, se);
	}
}

ساکن bool yield_مقصد_task_fair(ساخت rq *rq, ساخت task_struct *p)
{
	ساخت sched_entity *se = &p->se;

	/* !se->on_rq also covers throttled task */
	اگر (!se->on_rq || se->sched_delayed)
		واپس false;

	/* Tell the scheduler that we'd really like se to run next. */
	سیٹ_اگلا_buddy(&task_rq(p)->cfs, se);

	yield_task_fair(rq);

	واپس true;
}

/**************************************************
 * Fair scheduling class load-balancing methods.
 *
 * BASICS
 *
 * The purpose of load-balancing is to achieve the same basic fairness the
 * per-CPU scheduler provides, namely provide a proportional amount of compute
 * time to each task. This is expressed in the following equation:
 *
 *   W_i,n/P_i == W_j,n/P_j for all i,j                               (1)
 *
 * Where W_i,n is the n-th weight average for CPU i. The instantaneous weight
 * W_i,0 is defined as:
 *
 *   W_i,0 = \Sum_j w_i,j                                             (2)
 *
 * Where w_i,j is the weight of the j-th runnable task on CPU i. This weight
 * is derived from the nice value as per sched_prio_to_weight[].
 *
 * The weight average is an exponential decay average of the instantaneous
 * weight:
 *
 *   W'_i,n = (2^n - 1) / 2^n * W_i,n + 1 / 2^n * W_i,0               (3)
 *
 * C_i is the compute capacity of CPU i, typically it is the
 * fraction of 'recent' time available for SCHED_OTHER task execution. But it
 * can also include other factors [XXX].
 *
 * To achieve this balance we define a measure of imbalance which follows
 * directly from (1):
 *
 *   imb_i,j = max{ avg(W/C), W_i/C_i } - min{ avg(W/C), W_j/C_j }    (4)
 *
 * We them move tasks around to minimize the imbalance. In the continuous
 * function space it is obvious this converges, in the discrete case we get
 * a few fun cases generally called infeasible weight scenarios.
 *
 * [XXX expand on:
 *     - infeasible weights;
 *     - local vs global optima in the discrete case. ]
 *
 *
 * SCHED DOMAINS
 *
 * In order to solve the imbalance equation (4), and avoid the obvious O(n^2)
 * for all i,j solution, we create a tree of CPUs that follows the hardware
 * topology where each level pairs two lower groups (or better). This results
 * in O(log n) layers. Furthermore we reduce the number of CPUs going up the
 * tree to only the first of the previous level and we decrease the frequency
 * of load-balance at each level inversely proportional to the number of CPUs in
 * the groups.
 *
 * This yields:
 *
 *     log_2 n     1     n
 *   \Sum       { --- * --- * 2^i } = O(n)                            (5)
 *     i = 0      2^i   2^i
 *                               `- size of each group
 *         |         |     `- number of CPUs doing load-balance
 *         |         `- freq
 *         `- sum over all levels
 *
 * Coupled with a limit on how many tasks we can migrate every balance pass,
 * this makes (5) the runtime complexity of the balancer.
 *
 * An important property here is that each CPU is still (indirectly) connected
 * to every other CPU in at most O(log n) steps:
 *
 * The adjacency matrix of the resulting graph is given by:
 *
 *             log_2 n
 *   A_i,j = \Union     (i % 2^k == 0) && i / 2^(k+1) == j / 2^(k+1)  (6)
 *             k = 0
 *
 * And you'll find that:
 *
 *   A^(log_2 n)_i,j != 0  for all i,j                                (7)
 *
 * Showing there's indeed a path between every CPU in at most O(log n) steps.
 * The task movement gives a factor of O(m), giving a convergence complexity
 * of:
 *
 *   O(nm log n),  n := nr_cpus, m := nr_tasks                        (8)
 *
 *
 * WORK CONSERVING
 *
 * In order to avoid CPUs going idle while there's still work to do, new idle
 * balancing is more aggressive and has the newly idle CPU iterate up the domain
 * tree itself instead of relying on other CPUs to bring it work.
 *
 * This adds some complexity to both (5) and (8) but it reduces the total idle
 * time.
 *
 * [XXX more?]
 *
 *
 * CGROUPS
 *
 * Cgroups make a horror show out of (2), instead of a simple sum we get:
 *
 *                                s_k,i
 *   W_i,0 = \Sum_j \Prod_k w_k * -----                               (9)
 *                                 S_k
 *
 * Where
 *
 *   s_k,i = \Sum_j w_i,j,k  and  S_k = \Sum_i s_k,i                 (10)
 *
 * w_i,j,k is the weight of the j-th runnable task in the k-th cgroup on CPU i.
 *
 * The big problem is S_k, its a global sum needed to compute a local (W_i)
 * property.
 *
 * [XXX write more on how we solve this.. _after_ merging pjt's patches that
 *      rewrite all of this once again.]
 */

ساکن غیرعلامتی طویل __read_mostly max_بھرو_balance_interval = HZ/10;

شمار fbq_type { regular, remote, all };

/*
 * 'group_type' describes the group of CPUs at the moment of load balancing.
 *
 * The enum is ordered by pulling priority, with the group with lowest priority
 * first so the group_type can simply be compared when selecting the busiest
 * group. See update_sd_pick_busiest().
 */
شمار گروہ_type {
	/* The group has spare capacity that can be used to run more tasks.  */
	گروہ_has_فالتو = 0,
	/*
	 * The group is fully used and the tasks don't compete for more CPU
	 * cycles. Nevertheless, some tasks might wait before running.
	 */
	گروہ_fully_busy,
	/*
	 * One task doesn't fit with CPU's capacity and must be migrated to a
	 * more powerful CPU.
	 */
	گروہ_misfit_task,
	/*
	 * Balance SMT group that's fully busy. Can benefit from migration
	 * a task on SMT with busy sibling to another CPU on idle core.
	 */
	گروہ_smt_balance,
	/*
	 * SD_ASYM_PACKING only: One local CPU with higher capacity is available,
	 * and the task should be migrated to it instead of running on the
	 * current CPU.
	 */
	گروہ_asym_packing,
	/*
	 * The tasks' affinity constraints previously prevented the scheduler
	 * from balancing the load across the system.
	 */
	گروہ_imbalanced,
	/*
	 * There are tasks running on non-preferred LLC, possible to move
	 * them to their preferred LLC without creating too much imbalance.
	 * The priority of group_llc_balance is lower than that of
	 * group_overloaded and higher than that of all other group types.
	 * This is because group_llc_balance may exacerbate load imbalance.
	 * If the LLC balancing attempt fails, the nr_balance_failed
	 * mechanism will trigger other group types to rebalance the load.
	 */
	گروہ_llc_balance,
	/*
	 * The CPU is overloaded and can't provide expected CPU cycles to all
	 * tasks.
	 */
	گروہ_overloaded
};

شمار migration_type {
	migrate_بھرو = 0,
	migrate_util,
	migrate_task,
	migrate_misfit,
	migrate_llc_task
};

#تعریف_کرو LBF_ALL_PINNED	0x01
#تعریف_کرو LBF_NEED_BREAK	0x02
#تعریف_کرو LBF_DST_PINNED  0x04
#تعریف_کرو LBF_SOME_PINNED	0x08
#تعریف_کرو LBF_فعال_LB	0x10
#تعریف_کرو LBF_LLC_PINNED	0x20
#تعریف_کرو LBF_فعال_LB_LLC	0x40

ساخت lb_env {
	ساخت sched_domain	*sd;

	ساخت rq		*src_rq;
	صحیح			src_cpu;

	صحیح			dst_cpu;
	ساخت rq		*dst_rq;
	bool			dst_core_idle;

	ساخت cpumask		*dst_grpmask;
	صحیح			new_dst_cpu;
	شمار cpu_idle_type	idle;
	طویل			imbalance;
	/* The set of CPUs under consideration for load-balancing */
	ساخت cpumask		*cpus;

	غیرعلامتی صحیح		flags;

	غیرعلامتی صحیح		loop;
	غیرعلامتی صحیح		loop_break;
	غیرعلامتی صحیح		loop_max;

	شمار fbq_type		fbq_type;
	شمار migration_type	migration_type;
	ساخت list_سر	tasks;
};

/*
 * Is this task likely cache-hot:
 */
ساکن صحیح task_hot(ساخت task_struct *p, ساخت lb_env *env)
{
	s64 delta;

	lockdep_assert_rq_held(env->src_rq);

	اگر (p->sched_class != &fair_sched_class)
		واپس 0;

	اگر (unlikely(task_has_idle_policy(p)))
		واپس 0;

	/* SMT siblings share cache */
	اگر (env->sd->flags & SD_SHARE_CPUCAPACITY)
		واپس 0;

	/*
	 * Buddy candidates are cache hot:
	 */
	اگر (sched_feat(CACHE_HOT_BUDDY) && env->dst_rq->nr_running &&
	    (&p->se == cfs_rq_کا(&p->se)->اگلا))
		واپس 1;

	اگر (sysctl_sched_migration_لاگت == -1)
		واپس 1;

	/*
	 * Don't migrate task if the task's cookie does not match
	 * with the destination CPU's core cookie.
	 */
	اگر (!sched_core_cookie_مطابقت(cpu_rq(env->dst_cpu), p))
		واپس 1;

	اگر (sysctl_sched_migration_لاگت == 0)
		واپس 0;

	delta = rq_clock_task(env->src_rq) - p->se.exec_آغاز;

	واپس delta < (s64)sysctl_sched_migration_لاگت;
}

#اگر_تعریف_شدہ CONFIG_NUMA_BALANCING
/*
 * Returns a positive value, if task migration degrades locality.
 * Returns 0, if task migration is not affected by locality.
 * Returns a negative value, if task migration improves locality i.e migration preferred.
 */
ساکن طویل migrate_degrades_locality(ساخت task_struct *p, ساخت lb_env *env)
{
	ساخت numa_گروہ *numa_گروہ = rcu_dereference_all(p->numa_گروہ);
	غیرعلامتی طویل src_وزن, dst_وزن;
	صحیح src_nid, dst_nid, dist;

	اگر (!static_branch_likely(&sched_numa_balancing))
		واپس 0;

	اگر (!p->numa_خرابیاں || !(env->sd->flags & SD_NUMA))
		واپس 0;

	src_nid = cpu_مقصد_گرہ(env->src_cpu);
	dst_nid = cpu_مقصد_گرہ(env->dst_cpu);

	اگر (src_nid == dst_nid)
		واپس 0;

	/* Migrating away from the preferred node is always bad. */
	اگر (src_nid == p->numa_preferred_nid) {
		اگر (env->src_rq->nr_running > env->src_rq->nr_preferred_running)
			واپس 1;
		ورنہ
			واپس 0;
	}

	/* Encourage migration to the preferred node. */
	اگر (dst_nid == p->numa_preferred_nid)
		واپس -1;

	/* Leaving a core idle is often worse than degrading locality. */
	اگر (env->idle == CPU_IDLE)
		واپس 0;

	dist = گرہ_فاصلہ(src_nid, dst_nid);
	اگر (numa_گروہ) {
		src_وزن = گروہ_وزن(p, src_nid, dist);
		dst_وزن = گروہ_وزن(p, dst_nid, dist);
	} ورنہ {
		src_وزن = task_وزن(p, src_nid, dist);
		dst_وزن = task_وزن(p, dst_nid, dist);
	}

	واپس src_وزن - dst_وزن;
}

#ورنہ /* !CONFIG_NUMA_BALANCING: */
ساکن اندرونی طویل migrate_degrades_locality(ساخت task_struct *p,
					     ساخت lb_env *env)
{
	واپس 0;
}
#اگر_ختم /* !CONFIG_NUMA_BALANCING */

/*
 * Check whether the task is ineligible on the destination cpu
 *
 * When the PLACE_LAG scheduling feature is enabled and
 * dst_cfs_rq->nr_queued is greater than 1, if the task
 * is ineligible, it will also be ineligible when
 * it is migrated to the destination cpu.
 */
ساکن اندرونی صحیح task_is_ineligible_on_dst_cpu(ساخت task_struct *p, صحیح dest_cpu)
{
	ساخت cfs_rq *dst_cfs_rq = &cpu_rq(dest_cpu)->cfs;

	اگر (sched_feat(رکھو_LAG) && dst_cfs_rq->h_nr_queued &&
	    !entity_eligible(&task_rq(p)->cfs, &p->se))
		واپس 1;

	واپس 0;
}

#اگر_تعریف_شدہ CONFIG_SCHED_CACHE
/*
 * The margin used when comparing LLC utilization with CPU capacity.
 * It determines the LLC load level where active LLC aggregation is
 * done.
 * Derived from fits_capacity().
 *
 * (default: ~50%, tunable via debugfs)
 */
ساکن bool سماتا_llc_استعداد(غیرعلامتی طویل util, غیرعلامتی طویل max)
{
	u32 aggr_pct = llc_overaggr_pct;

	/*
	 * For single core systems, raise the aggregation
	 * threshold to accommodate more tasks.
	 */
	اگر (cpu_smt_عدد_دھاگے == 1)
		aggr_pct = (aggr_pct * 3 / 2);

	واپس util * 100 < max * aggr_pct;
}

/*
 * The margin used when comparing utilization.
 * is 'util1' noticeably greater than 'util2'
 * Derived from capacity_greater().
 * Bias is in perentage.
 */
/* Allows dst util to be bigger than src util by up to bias percent */
#تعریف_کرو util_greater(util1, util2) \
	((util1) * 100 > (util2) * (100 + llc_imb_pct))

ساکن __maybe_unused bool get_llc_stats(صحیح cpu, غیرعلامتی طویل *util,
					 غیرعلامتی طویل *cap)
{
	ساخت sched_domain_shared *sd_share;

	sd_share = rcu_dereference_all(فی_cpu(sd_llc_shared, cpu));
	اگر (!sd_share)
		واپس false;

	*util = READ_ONCE(sd_share->util_avg);
	*cap = READ_ONCE(sd_share->استعداد);

	واپس true;
}

/*
 * Decision matrix according to the LLC utilization. To
 * decide whether we can do task aggregation across LLC.
 *
 * By default, 50% is the threshold for treating the LLC
 * as busy. The reason for choosing 50% is to avoid saturation
 * of SMT-2, and it is also a safe cutoff for other SMT-n
 * platforms. SMT-1 has higher threshold because it is
 * supposed to accommodate more tasks, see fits_llc_capacity().
 *
 * 20% is the utilization imbalance percentage to decide
 * if the preferred LLC is busier than the non-preferred LLC.
 * 20 is a little higher than the LLC domain's imbalance_pct
 * 17. The hysteresis is used to avoid task bouncing between the
 * preferred LLC and the non-preferred LLC, and it will
 * be turned into tunable debugfs.
 *
 * 1. moving towards the preferred LLC, dst is the preferred
 *    LLC, src is not.
 *
 * src \ dst      30%  40%  50%  60%
 * 30%            Y    Y    Y    N
 * 40%            Y    Y    Y    Y
 * 50%            Y    Y    G    G
 * 60%            Y    Y    G    G
 *
 * 2. moving out of the preferred LLC, src is the preferred
 *    LLC, dst is not:
 *
 * src \ dst      30%  40%  50%  60%
 * 30%            N    N    N    N
 * 40%            N    N    N    N
 * 50%            N    N    G    G
 * 60%            Y    N    G    G
 *
 * src :      src_util
 * dst :      dst_util
 * Y :        Yes, migrate
 * N :        No, do not migrate
 * G :        let the Generic load balance to even the load.
 *
 * The intention is that if both LLCs are quite busy, cache aware
 * load balance should not be performed, and generic load balance
 * should take effect. However, if one is busy and the other is not,
 * the preferred LLC capacity(50%) and imbalance criteria(20%) should
 * be considered to determine whether LLC aggregation should be
 * performed to bias the load towards the preferred LLC.
 */

/* migration decision, 3 states are orthogonal. */
شمار llc_mig {
	mig_forbid = 0,		/* N: Don't migrate task, respect LLC preference */
	mig_llc,		/* Y: Do LLC preference based migration */
	mig_unrestricted	/* G: Don't restrict generic load balance migration */
};

/*
 * Check if task can be moved from the source LLC to the
 * destination LLC without breaking cache aware preferrence.
 * src_cpu and dst_cpu are arbitrary CPUs within the source
 * and destination LLCs, respectively.
 */
ساکن شمار llc_mig can_migrate_llc(صحیح src_cpu, صحیح dst_cpu,
				    غیرعلامتی طویل tsk_util,
				    bool مقصد_pref)
{
	غیرعلامتی طویل src_util, dst_util, src_cap, dst_cap;

	اگر (!get_llc_stats(src_cpu, &src_util, &src_cap) ||
	    !get_llc_stats(dst_cpu, &dst_util, &dst_cap))
		واپس mig_unrestricted;

	src_util = src_util < tsk_util ? 0 : src_util - tsk_util;
	dst_util = dst_util + tsk_util;

	اگر (!سماتا_llc_استعداد(dst_util, dst_cap) &&
	    !سماتا_llc_استعداد(src_util, src_cap))
		واپس mig_unrestricted;

	اگر (مقصد_pref) {
		/*
		 * Don't migrate if we will get preferred LLC too
		 * heavily loaded and if the dest is much busier
		 * than the src, in which case migration will
		 * increase the imbalance too much.
		 */
		اگر (!سماتا_llc_استعداد(dst_util, dst_cap) &&
		    util_greater(dst_util, src_util))
			واپس mig_forbid;
	} ورنہ {
		/*
		 * Don't migrate if we will leave preferred LLC
		 * too idle, or if this migration leads to the
		 * non-preferred LLC falls within sysctl_aggr_imb percent
		 * of preferred LLC, leading to migration again
		 * back to preferred LLC.
		 */
		اگر (سماتا_llc_استعداد(src_util, src_cap) ||
		    !util_greater(src_util, dst_util))
			واپس mig_forbid;
	}
	واپس mig_llc;
}

ساکن اندرونی bool task_misfits_asym_cpu(ساخت lb_env *env, ساخت task_struct *p)
{
	/*
	 * On asymmetric CPU capacity domains, do not let cache-aware
	 * balancing pull the task onto a destination CPU that cannot
	 * accommodate it. Doing so would turn the task into a misfit on
	 * the destination, trading a cache-locality gain for a capacity
	 * loss. If the task already does not fit its source CPU, the move
	 * cannot make things worse, so let the LLC preference decide.
	 */
	اگر ((env->sd->flags & SD_ASYM_CPUCAPACITY) && p &&
	    !task_سماتا_cpu(p, env->dst_cpu) &&
	    task_سماتا_cpu(p, env->src_cpu))
		واپس true;

	واپس false;
}

/*
 * Check if task p can migrate from source LLC to
 * destination LLC in terms of cache aware load balance.
 */
ساکن شمار llc_mig can_migrate_llc_task(ساخت lb_env *env,
					 ساخت task_struct *p)
{
	ساخت sched_cache_گروہ *grp;
	bool مقصد_pref;
	صحیح cpu, src_cpu, dst_cpu;

	اگر (task_misfits_asym_cpu(env, p))
		واپس mig_forbid;

	src_cpu = env->src_cpu;
	dst_cpu = env->dst_cpu;
	grp = rcu_dereference_all(p->sched_cache_grp);
	اگر (!grp)
		واپس mig_unrestricted;

	cpu = READ_ONCE(grp->cpu);
	اگر (cpu < 0 || cpus_share_cache(src_cpu, dst_cpu))
		واپس mig_unrestricted;

	/* skip cache aware load balance for too many threads */
	اگر (invalid_llc_nr(grp, p, dst_cpu) ||
	    exceed_llc_استعداد(grp, dst_cpu)) {
		اگر (READ_ONCE(grp->cpu) != -1)
			WRITE_ONCE(grp->cpu, -1);
		واپس mig_unrestricted;
	}

	اگر (cpus_share_cache(dst_cpu, cpu))
		مقصد_pref = true;
	ورنہ اگر (cpus_share_cache(src_cpu, cpu))
		مقصد_pref = false;
	ورنہ
		واپس mig_unrestricted;

	واپس can_migrate_llc(src_cpu, dst_cpu,
			       task_util(p), مقصد_pref);
}

/*
 * Check if active load balance breaks LLC locality in
 * terms of cache aware load balance. The load level and
 * imbalance do not warrant breaking LLC preference per
 * the can_migrate_llc() policy. Here, the benefit of
 * LLC locality outweighs the power efficiency gained from
 * migrating the only runnable task away.
 */
ساکن اندرونی bool
alb_break_llc(ساخت lb_env *env)
{
	اگر (!sched_cache_enabled())
		واپس false;

	اگر (cpus_share_cache(env->src_cpu, env->dst_cpu))
		واپس false;
	/*
	 * All tasks prefer to stay on their current CPU.
	 * Do not pull a task from its preferred CPU if:
	 * 1. It is the only task running and does not exceed
	 *    imbalance allowance; OR
	 * 2. Migrating it away from its preferred LLC would violate
	 *    the cache-aware scheduling policy.
	 */
	اگر (env->src_rq->nr_pref_llc_running &&
	    env->src_rq->nr_pref_llc_running == env->src_rq->cfs.h_nr_runnable) {
		غیرعلامتی طویل util = 0;
		ساخت task_struct *cur;

		/*
		 * Migrating misfit tasks from current CPU
		 * to CPU with a better fit.
		 * Prioritize that over LLC preference.
		 */
		اگر (env->migration_type == migrate_misfit)
			واپس false;

		اگر (env->src_rq->nr_running <= 1)
			واپس true;

		cur = rcu_dereference_all(env->src_rq->curr);
		اگر (cur && cur->sched_class == &fair_sched_class)
			util = task_util(cur);

		اگر (task_misfits_asym_cpu(env, cur) ||
		    can_migrate_llc(env->src_cpu, env->dst_cpu,
				    util, false) == mig_forbid)
			واپس true;
	}

	واپس false;
}

/*
 * Returns true if p's preferred LLC does not match the destination CPU
 * under migrate_llc_task semantics. Passive LB passes migrate_llc_task
 * in env->migration_type, while active LB carries LBF_ACTIVE_LB_LLC in
 * env->flags to avoid overwriting env->migration_type.
 */
ساکن اندرونی bool
migrate_llc_task_wrong_dst(ساخت task_struct *p, ساخت lb_env *env)
{
	واپس sched_cache_enabled() &&
	       (env->migration_type == migrate_llc_task ||
		env->flags & LBF_فعال_LB_LLC) &&
	       READ_ONCE(p->preferred_llc) != llc_id(env->dst_cpu);
}

/*
 * Check if migrating task p from env->src_cpu to
 * env->dst_cpu breaks LLC localiy.
 */
ساکن bool migrate_degrades_llc(ساخت task_struct *p, ساخت lb_env *env)
{
	اگر (!sched_cache_enabled())
		واپس false;

	اگر (task_has_sched_core(p))
		واپس false;
	/*
	 * Skip over tasks that would degrade LLC locality;
	 * only when nr_balanced_failed is sufficiently high do we
	 * ignore this constraint.
	 *
	 * Threshold of cache_nice_tries is set to 1 higher
	 * than nr_balance_failed to avoid excessive task
	 * migration at the same time.
	 */
	اگر (env->sd->nr_balance_failed >= env->sd->cache_nice_tries + 1)
		واپس false;

	/*
	 * We know the env->src_cpu has some tasks prefer to
	 * run on env->dst_cpu, skip the tasks do not prefer
	 * env->dst_cpu, and find the one that prefers.
	 */
	اگر (migrate_llc_task_wrong_dst(p, env))
		واپس true;

	اگر (can_migrate_llc_task(env, p) != mig_forbid)
		واپس false;

	واپس true;
}

#ورنہ
ساکن اندرونی bool get_llc_stats(صحیح cpu, غیرعلامتی طویل *util,
				 غیرعلامتی طویل *cap)
{
	واپس false;
}

ساکن اندرونی bool
alb_break_llc(ساخت lb_env *env)
{
	واپس false;
}

ساکن اندرونی bool
migrate_llc_task_wrong_dst(ساخت task_struct *p, ساخت lb_env *env)
{
	واپس false;
}

ساکن اندرونی bool
migrate_degrades_llc(ساخت task_struct *p, ساخت lb_env *env)
{
	واپس false;
}
#اگر_ختم
/*
 * can_migrate_task - may task p from runqueue rq be migrated to this_cpu?
 */
ساکن
صحیح can_migrate_task(ساخت task_struct *p, ساخت lb_env *env)
{
	طویل degrades, hot;

	lockdep_assert_rq_held(env->src_rq);
	اگر (p->sched_task_hot)
		p->sched_task_hot = 0;

	/*
	 * We do not migrate tasks that are:
	 * 1) delayed dequeued unless we migrate load, or
	 * 2) target cfs_rq is in throttled hierarchy, or
	 * 3) cannot be migrated to this CPU due to cpus_ptr, or
	 * 4) running (obviously), or
	 * 5) are cache-hot on their current CPU, or
	 * 6) are blocked on mutexes (if SCHED_PROXY_EXEC is enabled)
	 */
	اگر ((p->se.sched_delayed) && (env->migration_type != migrate_بھرو))
		واپس 0;

	اگر (lb_throttled_hierarchy(p, env->dst_cpu))
		واپس 0;

	/*
	 * We want to prioritize the migration of eligible tasks.
	 * For ineligible tasks we soft-limit them and only allow
	 * them to migrate when nr_balance_failed is non-zero to
	 * avoid load-balancing trying very hard to balance the load.
	 */
	اگر (!env->sd->nr_balance_failed &&
	    task_is_ineligible_on_dst_cpu(p, env->dst_cpu))
		واپس 0;

	/* Disregard percpu kthreads; they are where they need to be. */
	اگر (kthread_is_فی_cpu(p))
		واپس 0;

	اگر (task_is_blocked(p))
		واپس 0;

	اگر (!cpumask_test_cpu(env->dst_cpu, p->cpus_ptr)) {
		صحیح cpu;

		schedstat_inc(p->stats.nr_failed_migrations_affine);

		env->flags |= LBF_SOME_PINNED;

		/*
		 * Remember if this task can be migrated to any other CPU in
		 * our sched_group. We may want to revisit it if we couldn't
		 * meet load balance goals by pulling other tasks on src_cpu.
		 *
		 * Avoid computing new_dst_cpu
		 * - for NEWLY_IDLE
		 * - if we have already computed one in current iteration
		 * - if it's an active balance
		 */
		اگر (env->idle == CPU_NEWLY_IDLE ||
		    env->flags & (LBF_DST_PINNED | LBF_فعال_LB))
			واپس 0;

		/* Prevent to re-select dst_cpu via env's CPUs: */
		cpu = cpumask_پہلا_and_and(env->dst_grpmask, env->cpus, p->cpus_ptr);

		اگر (cpu < nr_cpu_ids) {
			env->flags |= LBF_DST_PINNED;
			env->new_dst_cpu = cpu;
		}

		واپس 0;
	}

	/* Record that we found at least one task that could run on dst_cpu */
	env->flags &= ~LBF_ALL_PINNED;

	اگر (task_on_cpu(env->src_rq, p) ||
	    task_current_donor(env->src_rq, p)) {
		schedstat_inc(p->stats.nr_failed_migrations_running);
		واپس 0;
	}

	/*
	 * Aggressive migration if:
	 * 1) active balance
	 * 2) destination numa is preferred
	 * 3) task is cache cold, or
	 * 4) too many balance attempts have failed.
	 */
	اگر (env->flags & LBF_فعال_LB)
		واپس !migrate_llc_task_wrong_dst(p, env);

	degrades = migrate_degrades_locality(p, env);
	اگر (!degrades) {
		/*
		 * If the NUMA locality is not broken,
		 * further check if migration would hurt
		 * LLC locality.
		 */
		اگر (migrate_degrades_llc(p, env)) {
			/*
			 * If regular load balancing fails to pull a task
			 * due to LLC locality, this is expected behavior
			 * and we set LBF_LLC_PINNED so we don't increase
			 * nr_balance_failed unecessarily.
			 */
			اگر (env->migration_type != migrate_llc_task)
				env->flags |= LBF_LLC_PINNED;

			واپس 0;
		}

		hot = task_hot(p, env);
	} ورنہ {
		hot = degrades > 0;
	}

	اگر (!hot || env->sd->nr_balance_failed > env->sd->cache_nice_tries) {
		اگر (hot)
			p->sched_task_hot = 1;
		واپس 1;
	}

	schedstat_inc(p->stats.nr_failed_migrations_hot);
	واپس 0;
}

/*
 * detach_task() -- detach the task for the migration specified in env
 */
ساکن خالی detach_task(ساخت task_struct *p, ساخت lb_env *env)
{
	lockdep_assert_rq_held(env->src_rq);

	اگر (p->sched_task_hot) {
		p->sched_task_hot = 0;
		schedstat_inc(env->sd->lb_hot_gained[env->idle]);
		schedstat_inc(p->stats.nr_forced_migrations);
	}

	WARN_ON(task_current(env->src_rq, p));
	WARN_ON(task_current_donor(env->src_rq, p));

	deactivate_task(env->src_rq, p, قطار_سے_نکالو_NOCLOCK);
	سیٹ_task_cpu(p, env->dst_cpu);
}

/*
 * detach_one_task() -- tries to dequeue exactly one task from env->src_rq, as
 * part of active balancing operations within "domain".
 *
 * Returns a task if successful and NULL otherwise.
 */
ساکن ساخت task_struct *detach_one_task(ساخت lb_env *env)
{
	ساخت task_struct *p;

	lockdep_assert_rq_held(env->src_rq);

	list_for_each_اندراج_الٹ(p,
			&env->src_rq->cfs_tasks, se.گروہ_گرہ) {
		اگر (!can_migrate_task(p, env))
			جاری;

		detach_task(p, env);

		/*
		 * Right now, this is only the second place where
		 * lb_gained[env->idle] is updated (other is detach_tasks)
		 * so we can safely collect stats here rather than
		 * inside detach_tasks().
		 */
		schedstat_inc(env->sd->lb_gained[env->idle]);
		واپس p;
	}
	واپس تہی;
}

/*
 * detach_tasks() -- tries to detach up to imbalance load/util/tasks from
 * busiest_rq, as part of a balancing operation within domain "sd".
 *
 * Returns number of detached tasks if successful and 0 otherwise.
 */
ساکن صحیح detach_tasks(ساخت lb_env *env)
{
	ساخت list_سر *tasks = &env->src_rq->cfs_tasks;
	غیرعلامتی طویل util, بھرو;
	ساخت task_struct *p;
	صحیح detached = 0;

	lockdep_assert_rq_held(env->src_rq);

	/*
	 * Source run queue has been emptied by another CPU, clear
	 * LBF_ALL_PINNED flag as we will not test any task.
	 */
	اگر (env->src_rq->nr_running <= 1) {
		env->flags &= ~LBF_ALL_PINNED;
		واپس 0;
	}

	اگر (env->imbalance <= 0)
		واپس 0;

	جب (!list_خالی_خانے(tasks)) {
		/*
		 * We don't want to steal all, otherwise we may be treated likewise,
		 * which could at worst lead to a livelock crash.
		 */
		اگر (env->idle && env->src_rq->nr_running <= 1)
			توڑو;

		env->loop++;
		/* We've more or less seen every task there is, call it quits */
		اگر (env->loop > env->loop_max)
			توڑو;

		/* take a breather every nr_migrate tasks */
		اگر (env->loop > env->loop_break) {
			env->loop_break += SCHED_NR_MIGRATE_BREAK;
			env->flags |= LBF_NEED_BREAK;
			توڑو;
		}

		p = list_آخری_اندراج(tasks, ساخت task_struct, se.گروہ_گرہ);

		اگر (!can_migrate_task(p, env))
			جاؤ اگلا;

		انتخاب (env->migration_type) {
		حالت migrate_بھرو:
			/*
			 * Depending of the number of CPUs and tasks and the
			 * cgroup hierarchy, task_h_load() can return a null
			 * value. Make sure that env->imbalance decreases
			 * otherwise detach_tasks() will stop only after
			 * detaching up to loop_max tasks.
			 */
			بھرو = max_t(غیرعلامتی طویل, task_h_بھرو(p), 1);

			اگر (sched_feat(LB_MIN) &&
			    بھرو < 16 && !env->sd->nr_balance_failed)
				جاؤ اگلا;

			/*
			 * Make sure that we don't migrate too much load.
			 * Nevertheless, let relax the constraint if
			 * scheduler fails to find a good waiting task to
			 * migrate.
			 */
			اگر (shr_bound(بھرو, env->sd->nr_balance_failed) > env->imbalance)
				جاؤ اگلا;

			env->imbalance -= بھرو;
			توڑو;

		حالت migrate_util:
			util = task_util_est(p);

			اگر (shr_bound(util, env->sd->nr_balance_failed) > env->imbalance)
				جاؤ اگلا;

			env->imbalance -= util;
			توڑو;

		حالت migrate_task:
			env->imbalance--;
			توڑو;

		حالت migrate_misfit:
			/* This is not a misfit task */
			اگر (task_سماتا_cpu(p, env->src_cpu))
				جاؤ اگلا;

			env->imbalance = 0;
			توڑو;

		حالت migrate_llc_task:
			env->imbalance--;
			توڑو;
		}

		detach_task(p, env);
		list_جوڑو(&p->se.گروہ_گرہ, &env->tasks);

		detached++;

#اگر_تعریف_شدہ CONFIG_PREEMPTION
		/*
		 * NEWIDLE balancing is a source of latency, so preemptible
		 * kernels will stop after the first task is detached to minimize
		 * the critical section.
		 */
		اگر (env->idle == CPU_NEWLY_IDLE)
			توڑو;
#اگر_ختم

		/*
		 * We only want to steal up to the prescribed amount of
		 * load/util/tasks.
		 */
		اگر (env->imbalance <= 0)
			توڑو;

		جاری;
اگلا:
		اگر (p->sched_task_hot)
			schedstat_inc(p->stats.nr_failed_migrations_hot);

		list_move(&p->se.گروہ_گرہ, tasks);
	}

	/*
	 * Right now, this is one of only two places we collect this stat
	 * so we can safely collect detach_one_task() stats here rather
	 * than inside detach_one_task().
	 */
	schedstat_جوڑو(env->sd->lb_gained[env->idle], detached);

	واپس detached;
}

/*
 * attach_tasks() -- attaches all tasks detached by detach_tasks() to their
 * new rq.
 */
ساکن خالی attach_tasks(ساخت lb_env *env)
{
	__attach_tasks(env->dst_rq, &env->tasks);
}

#اگر_تعریف_شدہ CONFIG_NO_HZ_COMMON
ساکن اندرونی bool cfs_rq_has_blocked_بھرو(ساخت cfs_rq *cfs_rq)
{
	اگر (cfs_rq->avg.بھرو_avg)
		واپس true;

	اگر (cfs_rq->avg.util_avg)
		واپس true;

	واپس false;
}

ساکن اندرونی bool others_have_blocked(ساخت rq *rq)
{
	اگر (cpu_util_rt(rq))
		واپس true;

	اگر (cpu_util_dl(rq))
		واپس true;

	اگر (hw_بھرو_avg(rq))
		واپس true;

	اگر (cpu_util_irq(rq))
		واپس true;

	واپس false;
}

ساکن اندرونی خالی update_blocked_بھرو_tick(ساخت rq *rq)
{
	WRITE_ONCE(rq->آخری_blocked_بھرو_update_tick, jiffies);
}

ساکن اندرونی خالی update_has_blocked_بھرو_حالت(ساخت rq *rq, bool has_blocked_بھرو)
{
	اگر (!has_blocked_بھرو)
		rq->has_blocked_بھرو = 0;
}
#ورنہ /* !CONFIG_NO_HZ_COMMON: */
ساکن اندرونی bool cfs_rq_has_blocked_بھرو(ساخت cfs_rq *cfs_rq) { واپس false; }
ساکن اندرونی bool others_have_blocked(ساخت rq *rq) { واپس false; }
ساکن اندرونی خالی update_blocked_بھرو_tick(ساخت rq *rq) {}
ساکن اندرونی خالی update_has_blocked_بھرو_حالت(ساخت rq *rq, bool has_blocked_بھرو) {}
#اگر_ختم /* !CONFIG_NO_HZ_COMMON */

ساکن bool __update_blocked_others(ساخت rq *rq, bool *مکمل)
{
	bool updated;

	/*
	 * update_load_avg() can call cpufreq_update_util(). Make sure that RT,
	 * DL and IRQ signals have been updated before updating CFS.
	 */
	updated = update_other_بھرو_avgs(rq);

	اگر (others_have_blocked(rq))
		*مکمل = false;

	واپس updated;
}

#اگر_تعریف_شدہ CONFIG_FAIR_گروہ_SCHED

ساکن bool __update_blocked_fair(ساخت rq *rq, bool *مکمل)
{
	ساخت cfs_rq *cfs_rq, *pos;
	bool decayed = false;

	/*
	 * Iterates the task_group tree in a bottom up fashion, see
	 * list_add_leaf_cfs_rq() for details.
	 */
	for_each_برگ_cfs_rq_محفوظ(rq, cfs_rq, pos) {
		ساخت sched_entity *se;

		اگر (update_cfs_rq_بھرو_avg(cfs_rq_clock_pelt(cfs_rq), cfs_rq)) {
			update_tg_بھرو_avg(cfs_rq);

			اگر (cfs_rq->nr_queued == 0)
				update_idle_cfs_rq_clock_pelt(cfs_rq);

			اگر (cfs_rq == &rq->cfs)
				decayed = true;
		}

		/* Propagate pending load changes to the parent, if any: */
		se = cfs_rq_se(cfs_rq);
		اگر (se && !skip_blocked_update(se))
			update_بھرو_avg(cfs_rq_کا(se), se, UPDATE_TG);

		/*
		 * There can be a lot of idle CPU cgroups.  Don't let fully
		 * decayed cfs_rqs linger on the list.
		 */
		اگر (cfs_rq_is_decayed(cfs_rq))
			list_del_برگ_cfs_rq(cfs_rq);

		/* Don't need periodic decay once load/util_avg are null */
		اگر (cfs_rq_has_blocked_بھرو(cfs_rq))
			*مکمل = false;
	}

	واپس decayed;
}

/*
 * Compute the hierarchical load factor for cfs_rq and all its ascendants.
 * This needs to be done in a top-down fashion because the load of a child
 * group is a fraction of its parents load.
 */
ساکن خالی update_cfs_rq_h_بھرو(ساخت cfs_rq *cfs_rq)
{
	ساخت sched_entity *se = cfs_rq_se(cfs_rq);
	غیرعلامتی طویل now = jiffies;
	غیرعلامتی طویل بھرو;

	اگر (cfs_rq->آخری_h_بھرو_update == now)
		واپس;

	WRITE_ONCE(cfs_rq->h_بھرو_اگلا, تہی);
	for_each_sched_entity(se) {
		cfs_rq = cfs_rq_کا(se);
		WRITE_ONCE(cfs_rq->h_بھرو_اگلا, se);
		اگر (cfs_rq->آخری_h_بھرو_update == now)
			توڑو;
	}

	اگر (!se) {
		cfs_rq->h_بھرو = cfs_rq_بھرو_avg(cfs_rq);
		cfs_rq->آخری_h_بھرو_update = now;
	}

	جب ((se = READ_ONCE(cfs_rq->h_بھرو_اگلا)) != تہی) {
		بھرو = cfs_rq->h_بھرو;
		بھرو = div64_ul(بھرو * se->avg.بھرو_avg,
				cfs_rq_بھرو_avg(cfs_rq) + 1);
		cfs_rq = گروہ_cfs_rq(se);
		cfs_rq->h_بھرو = بھرو;
		cfs_rq->آخری_h_بھرو_update = now;
	}
}

ساکن غیرعلامتی طویل task_h_بھرو(ساخت task_struct *p)
{
	ساخت cfs_rq *cfs_rq = task_cfs_rq(p);

	update_cfs_rq_h_بھرو(cfs_rq);
	واپس div64_ul(p->se.avg.بھرو_avg * cfs_rq->h_بھرو,
			cfs_rq_بھرو_avg(cfs_rq) + 1);
}
#ورنہ /* !CONFIG_FAIR_GROUP_SCHED: */
ساکن bool __update_blocked_fair(ساخت rq *rq, bool *مکمل)
{
	ساخت cfs_rq *cfs_rq = &rq->cfs;
	bool decayed;

	decayed = update_cfs_rq_بھرو_avg(cfs_rq_clock_pelt(cfs_rq), cfs_rq);
	اگر (cfs_rq_has_blocked_بھرو(cfs_rq))
		*مکمل = false;

	واپس decayed;
}

ساکن غیرعلامتی طویل task_h_بھرو(ساخت task_struct *p)
{
	واپس p->se.avg.بھرو_avg;
}
#اگر_ختم /* !CONFIG_FAIR_GROUP_SCHED */

ساکن خالی __sched_balance_update_blocked_averages(ساخت rq *rq)
{
	bool decayed = false, مکمل = true;

	update_blocked_بھرو_tick(rq);

	decayed |= __update_blocked_others(rq, &مکمل);
	decayed |= __update_blocked_fair(rq, &مکمل);

	update_has_blocked_بھرو_حالت(rq, !مکمل);
	اگر (decayed)
		cpufreq_update_util(rq, 0);
}

ساکن خالی sched_balance_update_blocked_averages(صحیح cpu)
{
	ساخت rq *rq = cpu_rq(cpu);

	guard(rq_تالا_irqsave)(rq);
	update_rq_clock(rq);
	__sched_balance_update_blocked_averages(rq);
}

/********** Helpers for sched_balance_find_src_group ************************/

/*
 * sg_lb_stats - stats of a sched_group required for load-balancing:
 */
ساخت sg_lb_stats {
	غیرعلامتی طویل avg_بھرو;			/* Avg load            over the CPUs of the group */
	غیرعلامتی طویل گروہ_بھرو;		/* Total load          over the CPUs of the group */
	غیرعلامتی طویل گروہ_استعداد;		/* Capacity            over the CPUs of the group */
	غیرعلامتی طویل گروہ_util;		/* Total utilization   over the CPUs of the group */
	غیرعلامتی طویل گروہ_runnable;		/* Total runnable time over the CPUs of the group */
	غیرعلامتی صحیح مجموع_nr_running;		/* Nr of all tasks running in the group */
	غیرعلامتی صحیح مجموع_h_nr_running;		/* Nr of CFS tasks running in the group */
	غیرعلامتی صحیح idle_cpus;                 /* Nr of idle CPUs         in the group */
	غیرعلامتی صحیح گروہ_وزن;
	شمار گروہ_type گروہ_type;
	غیرعلامتی صحیح گروہ_asym_packing;	/* Tasks should be moved to preferred CPU */
	غیرعلامتی صحیح گروہ_smt_balance;		/* Task on busy SMT be moved */
	غیرعلامتی صحیح گروہ_llc_balance;		/* Tasks should be moved to preferred LLC */
	غیرعلامتی طویل گروہ_misfit_task_بھرو;	/* A CPU has a task too big for its capacity */
	غیرعلامتی صحیح گروہ_overutilized;	/* At least one CPU is overutilized in the group */
#اگر_تعریف_شدہ CONFIG_NUMA_BALANCING
	غیرعلامتی صحیح nr_numa_running;
	غیرعلامتی صحیح nr_preferred_running;
#اگر_ختم
#اگر_تعریف_شدہ CONFIG_SCHED_CACHE
	غیرعلامتی صحیح nr_pref_dst_llc;
#اگر_ختم
};

/*
 * sd_lb_stats - stats of a sched_domain required for load-balancing:
 */
ساخت sd_lb_stats {
	ساخت sched_گروہ *busiest;		/* Busiest group in this sd */
	ساخت sched_گروہ *local;		/* Local group in this sd */
	غیرعلامتی طویل کل_بھرو;		/* Total load of all groups in sd */
	غیرعلامتی طویل کل_استعداد;		/* Total capacity of all groups in sd */
	غیرعلامتی طویل avg_بھرو;			/* Average load across all groups in sd */
	غیرعلامتی صحیح prefer_sibling;		/* Tasks should go to sibling first */

	ساخت sg_lb_stats busiest_stat;	/* Statistics of the busiest group */
	ساخت sg_lb_stats local_stat;		/* Statistics of the local group */
};

ساکن اندرونی خالی init_sd_lb_stats(ساخت sd_lb_stats *sds)
{
	/*
	 * Skimp on the clearing to avoid duplicate work. We can avoid clearing
	 * local_stat because update_sg_lb_stats() does a full clear/assignment.
	 * We must however set busiest_stat::group_type and
	 * busiest_stat::idle_cpus to the worst busiest group because
	 * update_sd_pick_busiest() reads these before assignment.
	 */
	*sds = (ساخت sd_lb_stats){
		.busiest = تہی,
		.local = تہی,
		.کل_بھرو = 0UL,
		.کل_استعداد = 0UL,
		.busiest_stat = {
			.idle_cpus = UINT_MAX,
			.گروہ_type = گروہ_has_فالتو,
		},
	};
}

ساکن غیرعلامتی طویل scale_rt_استعداد(صحیح cpu)
{
	غیرعلامتی طویل max = get_actual_cpu_استعداد(cpu);
	ساخت rq *rq = cpu_rq(cpu);
	غیرعلامتی طویل مستعمل, آزاد;
	غیرعلامتی طویل irq;

	irq = cpu_util_irq(rq);

	اگر (unlikely(irq >= max))
		واپس 1;

	/*
	 * avg_rt.util_avg and avg_dl.util_avg track binary signals
	 * (running and not running) with weights 0 and 1024 respectively.
	 */
	مستعمل = cpu_util_rt(rq);
	مستعمل += cpu_util_dl(rq);

	اگر (unlikely(مستعمل >= max))
		واپس 1;

	آزاد = max - مستعمل;

	واپس scale_irq_استعداد(آزاد, irq, max);
}

ساکن خالی update_cpu_استعداد(ساخت sched_domain *sd, صحیح cpu)
{
	غیرعلامتی طویل استعداد = scale_rt_استعداد(cpu);
	ساخت sched_گروہ *sdg = sd->groups;

	اگر (!استعداد)
		استعداد = 1;

	cpu_rq(cpu)->cpu_استعداد = استعداد;
	trace_sched_cpu_استعداد_tp(cpu_rq(cpu));

	sdg->sgc->استعداد = استعداد;
	sdg->sgc->min_استعداد = استعداد;
	sdg->sgc->max_استعداد = استعداد;
}

خالی update_گروہ_استعداد(ساخت sched_domain *sd, صحیح cpu)
{
	ساخت sched_domain *بچہ = sd->بچہ;
	ساخت sched_گروہ *گروہ, *sdg = sd->groups;
	غیرعلامتی طویل استعداد, min_استعداد, max_استعداد;
	غیرعلامتی طویل interval;

	interval = msecs_مقصد_jiffies(sd->balance_interval);
	interval = clamp(interval, 1UL, max_بھرو_balance_interval);
	sdg->sgc->اگلا_update = jiffies + interval;

	اگر (!بچہ) {
		update_cpu_استعداد(sd, cpu);
		واپس;
	}

	استعداد = 0;
	min_استعداد = ULONG_MAX;
	max_استعداد = 0;

	اگر (بچہ->flags & SD_NUMA) {
		/*
		 * SD_NUMA domains cannot assume that child groups
		 * span the current group.
		 */

		for_each_cpu(cpu, sched_گروہ_وسعت(sdg)) {
			غیرعلامتی طویل cpu_cap = استعداد_کا(cpu);

			استعداد += cpu_cap;
			min_استعداد = min(cpu_cap, min_استعداد);
			max_استعداد = max(cpu_cap, max_استعداد);
		}
	} ورنہ  {
		/*
		 * !SD_NUMA domains can assume that child groups
		 * span the current group.
		 */

		گروہ = بچہ->groups;
		کرو {
			ساخت sched_گروہ_استعداد *sgc = گروہ->sgc;

			استعداد += sgc->استعداد;
			min_استعداد = min(sgc->min_استعداد, min_استعداد);
			max_استعداد = max(sgc->max_استعداد, max_استعداد);
			گروہ = گروہ->اگلا;
		} جب (گروہ != بچہ->groups);
	}

	sdg->sgc->استعداد = استعداد;
	sdg->sgc->min_استعداد = min_استعداد;
	sdg->sgc->max_استعداد = max_استعداد;
}

/*
 * Check whether the capacity of the rq has been noticeably reduced by side
 * activity. The imbalance_pct is used for the threshold.
 * Return true is the capacity is reduced
 */
ساکن اندرونی صحیح
check_cpu_استعداد(ساخت rq *rq, ساخت sched_domain *sd)
{
	واپس ((rq->cpu_استعداد * sd->imbalance_pct) <
				(arch_scale_cpu_استعداد(cpu_کا(rq)) * 100));
}

/* Check if the rq has a misfit task */
ساکن اندرونی bool check_misfit_حالت(ساخت rq *rq)
{
	واپس rq->misfit_task_بھرو;
}

/*
 * Group imbalance indicates (and tries to solve) the problem where balancing
 * groups is inadequate due to ->cpus_ptr constraints.
 *
 * Imagine a situation of two groups of 4 CPUs each and 4 tasks each with a
 * cpumask covering 1 CPU of the first group and 3 CPUs of the second group.
 * Something like:
 *
 *	{ 0 1 2 3 } { 4 5 6 7 }
 *	        *     * * *
 *
 * If we were to balance group-wise we'd place two tasks in the first group and
 * two tasks in the second group. Clearly this is undesired as it will overload
 * cpu 3 and leave one of the CPUs in the second group unused.
 *
 * The current solution to this issue is detecting the skew in the first group
 * by noticing the lower domain failed to reach balance and had difficulty
 * moving tasks due to affinity constraints.
 *
 * When this is so detected; this group becomes a candidate for busiest; see
 * update_sd_pick_busiest(). And calculate_imbalance() and
 * sched_balance_find_src_group() avoid some of the usual balance conditions to allow it
 * to create an effective group imbalance.
 *
 * This is a somewhat tricky proposition since the next run might not find the
 * group imbalance and decide the groups need to be balanced again. A most
 * subtle and fragile situation.
 */

ساکن اندرونی صحیح sg_imbalanced(ساخت sched_گروہ *گروہ)
{
	واپس گروہ->sgc->imbalance;
}

/*
 * group_has_capacity returns true if the group has spare capacity that could
 * be used by some tasks.
 * We consider that a group has spare capacity if the number of task is
 * smaller than the number of CPUs or if the utilization is lower than the
 * available capacity for CFS tasks.
 * For the latter, we use a threshold to stabilize the state, to take into
 * account the variance of the tasks' load and to return true if the available
 * capacity in meaningful for the load balancer.
 * As an example, an available capacity of 1% can appear but it doesn't make
 * any benefit for the load balance.
 */
ساکن اندرونی bool
گروہ_has_استعداد(غیرعلامتی صحیح imbalance_pct, ساخت sg_lb_stats *sgs)
{
	اگر (sgs->مجموع_nr_running < sgs->گروہ_وزن)
		واپس true;

	اگر ((sgs->گروہ_استعداد * imbalance_pct) <
			(sgs->گروہ_runnable * 100))
		واپس false;

	اگر ((sgs->گروہ_استعداد * 100) >
			(sgs->گروہ_util * imbalance_pct))
		واپس true;

	واپس false;
}

/*
 *  group_is_overloaded returns true if the group has more tasks than it can
 *  handle.
 *  group_is_overloaded is not equals to !group_has_capacity because a group
 *  with the exact right number of tasks, has no more spare capacity but is not
 *  overloaded so both group_has_capacity and group_is_overloaded return
 *  false.
 */
ساکن اندرونی bool
گروہ_is_overloaded(غیرعلامتی صحیح imbalance_pct, ساخت sg_lb_stats *sgs)
{
	/*
	 * With EAS and uclamp, 1 CPU in the group must be overutilized to
	 * consider the group overloaded.
	 */
	اگر (sched_energy_enabled() && !sgs->گروہ_overutilized)
		واپس false;

	اگر (sgs->مجموع_nr_running <= sgs->گروہ_وزن)
		واپس false;

	اگر ((sgs->گروہ_استعداد * 100) <
			(sgs->گروہ_util * imbalance_pct))
		واپس true;

	اگر ((sgs->گروہ_استعداد * imbalance_pct) <
			(sgs->گروہ_runnable * 100))
		واپس true;

	واپس false;
}

ساکن اندرونی شمار
گروہ_type گروہ_classify(غیرعلامتی صحیح imbalance_pct,
			  ساخت sched_گروہ *گروہ,
			  ساخت sg_lb_stats *sgs)
{
	اگر (گروہ_is_overloaded(imbalance_pct, sgs))
		واپس گروہ_overloaded;

	اگر (sgs->گروہ_llc_balance)
		واپس گروہ_llc_balance;

	اگر (sg_imbalanced(گروہ))
		واپس گروہ_imbalanced;

	اگر (sgs->گروہ_asym_packing)
		واپس گروہ_asym_packing;

	اگر (sgs->گروہ_smt_balance)
		واپس گروہ_smt_balance;

	اگر (sgs->گروہ_misfit_task_بھرو)
		واپس گروہ_misfit_task;

	اگر (!گروہ_has_استعداد(imbalance_pct, sgs))
		واپس گروہ_fully_busy;

	واپس گروہ_has_فالتو;
}

/**
 * sched_use_asym_prio - Check whether asym_packing priority must be used
 * @sd:		The scheduling domain of the load balancing
 * @cpu:	A CPU
 *
 * Always use CPU priority when balancing load between SMT siblings. When
 * balancing load between cores, it is not sufficient that @cpu is idle. Only
 * use CPU priority if the whole core is idle.
 *
 * Returns: True if the priority of @cpu must be followed. False otherwise.
 */
ساکن bool sched_use_asym_prio(ساخت sched_domain *sd, صحیح cpu)
{
	اگر (!(sd->flags & SD_ASYM_PACKING))
		واپس false;

	اگر (!sched_smt_فعال())
		واپس true;

	واپس sd->flags & SD_SHARE_CPUCAPACITY || is_core_idle(cpu);
}

ساکن اندرونی bool sched_asym(ساخت sched_domain *sd, صحیح dst_cpu, صحیح src_cpu)
{
	/*
	 * First check if @dst_cpu can do asym_packing load balance. Only do it
	 * if it has higher priority than @src_cpu.
	 */
	واپس sched_use_asym_prio(sd, dst_cpu) &&
		sched_asym_prefer(dst_cpu, src_cpu);
}

/**
 * sched_group_asym - Check if the destination CPU can do asym_packing balance
 * @env:	The load balancing environment
 * @sgs:	Load-balancing statistics of the candidate busiest group
 * @group:	The candidate busiest group
 *
 * @env::dst_cpu can do asym_packing if it has higher priority than the
 * preferred CPU of @group.
 *
 * Return: true if @env::dst_cpu can do with asym_packing load balance. False
 * otherwise.
 */
ساکن اندرونی bool
sched_گروہ_asym(ساخت lb_env *env, ساخت sg_lb_stats *sgs, ساخت sched_گروہ *گروہ)
{
	/*
	 * CPU priorities do not make sense for SMT cores with more than one
	 * busy sibling.
	 */
	اگر ((گروہ->flags & SD_SHARE_CPUCAPACITY) &&
	    (sgs->گروہ_وزن - sgs->idle_cpus != 1))
		واپس false;

	واپس sched_asym(env->sd, env->dst_cpu, READ_ONCE(گروہ->asym_prefer_cpu));
}

/* One group has more than one SMT CPU while the other group does not */
ساکن اندرونی bool smt_vs_nonsmt_groups(ساخت sched_گروہ *sg1,
				    ساخت sched_گروہ *sg2)
{
	اگر (!sg1 || !sg2)
		واپس false;

	واپس (sg1->flags & SD_SHARE_CPUCAPACITY) !=
		(sg2->flags & SD_SHARE_CPUCAPACITY);
}

ساکن اندرونی bool smt_balance(ساخت lb_env *env, ساخت sg_lb_stats *sgs,
			       ساخت sched_گروہ *گروہ)
{
	اگر (!env->idle)
		واپس false;

	/*
	 * For SMT source group, it is better to move a task
	 * to a CPU that doesn't have multiple tasks sharing its CPU capacity.
	 * Note that if a group has a single SMT, SD_SHARE_CPUCAPACITY
	 * will not be on.
	 */
	اگر (گروہ->flags & SD_SHARE_CPUCAPACITY &&
	    sgs->مجموع_h_nr_running > 1)
		واپس true;

	واپس false;
}

ساکن اندرونی طویل sibling_imbalance(ساخت lb_env *env,
				    ساخت sd_lb_stats *sds,
				    ساخت sg_lb_stats *busiest,
				    ساخت sg_lb_stats *local)
{
	صحیح ncores_busiest, ncores_local;
	طویل imbalance;

	اگر (!env->idle || !busiest->مجموع_nr_running)
		واپس 0;

	ncores_busiest = sds->busiest->cores;
	ncores_local = sds->local->cores;

	اگر (ncores_busiest == ncores_local) {
		imbalance = busiest->مجموع_nr_running;
		lsub_مثبت(&imbalance, local->مجموع_nr_running);
		واپس imbalance;
	}

	/* Balance such that nr_running/ncores ratio are same on both groups */
	imbalance = ncores_local * busiest->مجموع_nr_running;
	lsub_مثبت(&imbalance, ncores_busiest * local->مجموع_nr_running);
	/* Normalize imbalance and do rounding on normalization */
	imbalance = 2 * imbalance + ncores_local + ncores_busiest;
	imbalance /= ncores_local + ncores_busiest;

	/* Take advantage of resource in an empty sched group */
	اگر (imbalance <= 1 && local->مجموع_nr_running == 0 &&
	    busiest->مجموع_nr_running > 1)
		imbalance = 2;

	واپس imbalance;
}

ساکن اندرونی bool
sched_reduced_استعداد(ساخت rq *rq, ساخت sched_domain *sd)
{
	/*
	 * When there is more than 1 task, the group_overloaded case already
	 * takes care of cpu with reduced capacity
	 */
	اگر (rq->cfs.h_nr_runnable != 1)
		واپس false;

	واپس check_cpu_استعداد(rq, sd);
}

#اگر_تعریف_شدہ CONFIG_SCHED_CACHE
/*
 * Record the statistics for this scheduler group for later
 * use. These values guide load balancing on aggregating tasks
 * to a LLC.
 */
ساکن خالی ریکارڈ_sg_llc_stats(ساخت lb_env *env,
				ساخت sg_lb_stats *sgs,
				ساخت sched_گروہ *گروہ)
{
	ساخت sched_domain_shared *sd_share;
	صحیح cpu;

	اگر (!sched_cache_enabled() || env->idle == CPU_NEWLY_IDLE)
		واپس;

	/* Only care about sched domain spanning multiple LLCs */
	اگر (env->sd->بچہ != rcu_dereference_all(فی_cpu(sd_llc, env->dst_cpu)))
		واپس;

	/*
	 * A redo repeats stats published on the first pass. Hotplug can
	 * still leave env->cpus partial, so util would not match capacity.
	 */
	اگر ((env->flags & LBF_ALL_PINNED) ||
	    !cpumask_subset(sched_گروہ_وسعت(گروہ), env->cpus))
		واپس;

	/*
	 * At this point we know this group spans a LLC domain.
	 * Record the statistic of this group in its corresponding
	 * shared LLC domain.
	 * Note: sd_share cannot be obtained via sd->child->shared,
	 * because the latter refers to the domain that covers the
	 * local group. Instead, sd_share should be located using
	 * the first CPU of the LLC group.
	 */
	cpu = cpumask_پہلا(sched_گروہ_وسعت(گروہ));
	sd_share = rcu_dereference_all(فی_cpu(sd_llc_shared, cpu));
	اگر (!sd_share)
		واپس;

	اگر (READ_ONCE(sd_share->util_avg) != sgs->گروہ_util)
		WRITE_ONCE(sd_share->util_avg, sgs->گروہ_util);

	اگر (unlikely(READ_ONCE(sd_share->استعداد) != sgs->گروہ_استعداد))
		WRITE_ONCE(sd_share->استعداد, sgs->گروہ_استعداد);
}

/*
 * Do LLC balance on sched group that contains LLC, and have tasks preferring
 * to run on LLC in idle dst_cpu.
 */
ساکن اندرونی bool llc_balance(ساخت lb_env *env, ساخت sg_lb_stats *sgs,
			       ساخت sched_گروہ *گروہ)
{
	اگر (!sched_cache_enabled())
		واپس false;

	اگر (env->sd->flags & SD_SHARE_LLC)
		واپس false;

	/*
	 * On asymmetric domains, group_misfit_task_load
	 * should be prioritized to move tasks to CPU that fit them
	 * over aggregating tasks to their preferred LLC.
	 */
	اگر ((env->sd->flags & SD_ASYM_CPUCAPACITY) &&
	    sgs->گروہ_misfit_task_بھرو)
		واپس false;

	/*
	 * Skip cache aware tagging if nr_balanced_failed is sufficiently high.
	 * Threshold of cache_nice_tries is set to 1 higher than nr_balance_failed
	 * to avoid excessive task migration at the same time.
	 */
	اگر (env->sd->nr_balance_failed >= env->sd->cache_nice_tries + 1)
		واپس false;

	اگر (sgs->nr_pref_dst_llc &&
	    can_migrate_llc(cpumask_پہلا(sched_گروہ_وسعت(گروہ)),
			    env->dst_cpu, 0, true) == mig_llc)
		واپس true;

	واپس false;
}

ساکن bool update_llc_busiest(ساخت lb_env *env,
			       ساخت sg_lb_stats *busiest,
			       ساخت sg_lb_stats *sgs)
{
	/*
	 * There are more tasks that want to run on dst_cpu's LLC.
	 */
	واپس sgs->nr_pref_dst_llc > busiest->nr_pref_dst_llc;
}
#ورنہ
ساکن اندرونی خالی ریکارڈ_sg_llc_stats(ساخت lb_env *env, ساخت sg_lb_stats *sgs,
				       ساخت sched_گروہ *گروہ)
{
}

ساکن اندرونی bool llc_balance(ساخت lb_env *env, ساخت sg_lb_stats *sgs,
			       ساخت sched_گروہ *گروہ)
{
	واپس false;
}

ساکن bool update_llc_busiest(ساخت lb_env *env,
			       ساخت sg_lb_stats *busiest,
			       ساخت sg_lb_stats *sgs)
{
	واپس false;
}
#اگر_ختم

/**
 * update_sg_lb_stats - Update sched_group's statistics for load balancing.
 * @env: The load balancing environment.
 * @sds: Load-balancing data with statistics of the local group.
 * @group: sched_group whose statistics are to be updated.
 * @sgs: variable to hold the statistics for this group.
 * @sg_overloaded: sched_group is overloaded
 */
ساکن اندرونی خالی update_sg_lb_stats(ساخت lb_env *env,
				      ساخت sd_lb_stats *sds,
				      ساخت sched_گروہ *گروہ,
				      ساخت sg_lb_stats *sgs,
				      bool *sg_overloaded)
{
	صحیح i, nr_running, local_گروہ, sd_flags = env->sd->flags;
	bool balancing_at_rd = !env->sd->والد;

	memset(sgs, 0, حجم(*sgs));

	local_گروہ = گروہ == sds->local;

	for_each_cpu_and(i, sched_گروہ_وسعت(گروہ), env->cpus) {
		ساخت rq *rq = cpu_rq(i);
		غیرعلامتی طویل بھرو = cpu_بھرو(rq);

		sgs->گروہ_بھرو += بھرو;
		sgs->گروہ_util += cpu_util_cfs(i);
		sgs->گروہ_runnable += cpu_runnable(rq);
		sgs->مجموع_h_nr_running += rq->cfs.h_nr_runnable;

		nr_running = rq->nr_running;
		sgs->مجموع_nr_running += nr_running;

		اگر (cpu_overutilized(i))
			sgs->گروہ_overutilized = 1;

#اگر_تعریف_شدہ CONFIG_SCHED_CACHE
		اگر (sched_cache_enabled()) {
			ساخت sched_domain *sd_tmp;
			صحیح dst_llc;

			dst_llc = llc_id(env->dst_cpu);
			اگر (llc_id(i) != dst_llc) {
				sd_tmp = rcu_dereference_all(rq->sd);
				اگر (sd_tmp && (غیرعلامتی صحیح)dst_llc < sd_tmp->llc_max)
					sgs->nr_pref_dst_llc += sd_tmp->llc_counts[dst_llc];
			}
		}
#اگر_ختم

		/*
		 * No need to call idle_cpu() if nr_running is not 0
		 */
		اگر (!nr_running && idle_cpu(i)) {
			sgs->idle_cpus++;
			/* Idle cpu can't have misfit task */
			جاری;
		}

		/* Overload indicator is only updated at root domain */
		اگر (balancing_at_rd && nr_running > 1)
			*sg_overloaded = 1;

#اگر_تعریف_شدہ CONFIG_NUMA_BALANCING
		/* Only fbq_classify_group() uses this to classify NUMA groups */
		اگر (sd_flags & SD_NUMA) {
			sgs->nr_numa_running += rq->nr_numa_running;
			sgs->nr_preferred_running += rq->nr_preferred_running;
		}
#اگر_ختم
		اگر (local_گروہ)
			جاری;

		اگر (sd_flags & SD_ASYM_CPUCAPACITY) {
			اگر (rq->misfit_task_بھرو) {
				/*
				 * Always mark the root domain overloaded so big
				 * CPUs can pick up misfit tasks via newly idle
				 * balance.
				 */
				اگر (balancing_at_rd)
					*sg_overloaded = 1;

				/*
				 * Only account misfit load if @dst_cpu can
				 * help; otherwise, the group may be classified
				 * as misfit_task and update_sd_pick_busiest()
				 * will skip it.
				 */
				اگر (استعداد_greater(استعداد_کا(env->dst_cpu),
						     گروہ->sgc->max_استعداد) &&
				    (sgs->گروہ_misfit_task_بھرو < rq->misfit_task_بھرو))
					sgs->گروہ_misfit_task_بھرو = rq->misfit_task_بھرو;
			}
		} ورنہ اگر (env->idle && sched_reduced_استعداد(rq, env->sd)) {
			/* Check for a task running on a CPU with reduced capacity */
			اگر (sgs->گروہ_misfit_task_بھرو < بھرو)
				sgs->گروہ_misfit_task_بھرو = بھرو;
		}
	}

	sgs->گروہ_استعداد = گروہ->sgc->استعداد;

	sgs->گروہ_وزن = گروہ->گروہ_وزن;

	اگر (!local_گروہ) {
		/* Check if dst CPU is idle and preferred to this group */
		اگر (env->idle && sgs->مجموع_h_nr_running &&
		    sched_گروہ_asym(env, sgs, گروہ))
			sgs->گروہ_asym_packing = 1;

		/* Check for loaded SMT group to be balanced to dst CPU */
		اگر (smt_balance(env, sgs, گروہ))
			sgs->گروہ_smt_balance = 1;

		/* Check for tasks in this group can be moved to their preferred LLC */
		اگر (llc_balance(env, sgs, گروہ))
			sgs->گروہ_llc_balance = 1;
	}

	sgs->گروہ_type = گروہ_classify(env->sd->imbalance_pct, گروہ, sgs);

	ریکارڈ_sg_llc_stats(env, sgs, گروہ);
	/* Computing avg_load makes sense only when group is overloaded */
	اگر (sgs->گروہ_type == گروہ_overloaded)
		sgs->avg_بھرو = (sgs->گروہ_بھرو * SCHED_استعداد_SCALE) /
				sgs->گروہ_استعداد;
}

/**
 * update_sd_pick_busiest - return 1 on busiest group
 * @env: The load balancing environment.
 * @sds: sched_domain statistics
 * @sg: sched_group candidate to be checked for being the busiest
 * @sgs: sched_group statistics
 *
 * Determine if @sg is a busier group than the previously selected
 * busiest group.
 *
 * Return: %true if @sg is a busier group than the previously selected
 * busiest group. %false otherwise.
 */
ساکن bool update_sd_pick_busiest(ساخت lb_env *env,
				   ساخت sd_lb_stats *sds,
				   ساخت sched_گروہ *sg,
				   ساخت sg_lb_stats *sgs)
{
	ساخت sg_lb_stats *busiest = &sds->busiest_stat;

	/* Make sure that there is at least one task to pull */
	اگر (!sgs->مجموع_h_nr_running)
		واپس false;

	/*
	 * Don't try to pull misfit tasks we can't help.
	 * We can use max_capacity here as reduction in capacity on some
	 * CPUs in the group should either be possible to resolve
	 * internally or be covered by avg_load imbalance (eventually).
	 *
	 * When SMT is active, only pull a misfit to dst_cpu if it is on a
	 * fully idle core; otherwise the effective capacity of the core is
	 * reduced and we may not actually provide more capacity than the
	 * source.
	 */
	اگر ((env->sd->flags & SD_ASYM_CPUCAPACITY) &&
	    (sgs->گروہ_type == گروہ_misfit_task) &&
	    (!env->dst_core_idle ||
	     !استعداد_greater(استعداد_کا(env->dst_cpu), sg->sgc->max_استعداد) ||
	     sds->local_stat.گروہ_type != گروہ_has_فالتو))
		واپس false;

	/*
	 * Candidate sg has no more than one task per CPU and has higher
	 * per-CPU capacity. Migrating tasks to less capable CPUs may harm
	 * throughput. Maximize throughput, power/energy consequences are not
	 * considered.
	 */
	اگر ((env->sd->flags & SD_ASYM_CPUCAPACITY) &&
	    (sgs->گروہ_type <= گروہ_fully_busy) &&
	    (استعداد_greater(sg->sgc->min_استعداد, استعداد_کا(env->dst_cpu))))
		واپس false;

	اگر (sgs->گروہ_type > busiest->گروہ_type)
		واپس true;

	اگر (sgs->گروہ_type < busiest->گروہ_type)
		واپس false;

	/*
	 * The candidate and the current busiest group are the same type of
	 * group. Let check which one is the busiest according to the type.
	 */

	انتخاب (sgs->گروہ_type) {
	حالت گروہ_overloaded:
		/* Select the overloaded group with highest avg_load. */
		واپس sgs->avg_بھرو > busiest->avg_بھرو;

	حالت گروہ_llc_balance:
		/* Select the group with most tasks preferring dst LLC */
		واپس update_llc_busiest(env, busiest, sgs);

	حالت گروہ_imbalanced:
		/*
		 * Select the 1st imbalanced group as we don't have any way to
		 * choose one more than another.
		 */
		واپس false;

	حالت گروہ_asym_packing:
		/* Prefer to move from lowest priority CPU's work */
		واپس sched_asym_prefer(READ_ONCE(sds->busiest->asym_prefer_cpu),
					 READ_ONCE(sg->asym_prefer_cpu));

	حالت گروہ_misfit_task:
		/*
		 * If we have more than one misfit sg go with the biggest
		 * misfit.
		 */
		واپس sgs->گروہ_misfit_task_بھرو > busiest->گروہ_misfit_task_بھرو;

	حالت گروہ_smt_balance:
		/*
		 * Check if we have spare CPUs on either SMT group to
		 * choose has spare or fully busy handling.
		 */
		اگر (sgs->idle_cpus != 0 || busiest->idle_cpus != 0)
			جاؤ has_فالتو;

		fallthrough;

	حالت گروہ_fully_busy:
		/*
		 * Select the fully busy group with highest avg_load. In
		 * theory, there is no need to pull task from such kind of
		 * group because tasks have all compute capacity that they need
		 * but we can still improve the overall throughput by reducing
		 * contention when accessing shared HW resources.
		 *
		 * XXX for now avg_load is not computed and always 0 so we
		 * select the 1st one, except if @sg is composed of SMT
		 * siblings.
		 */

		اگر (sgs->avg_بھرو < busiest->avg_بھرو)
			واپس false;

		اگر (sgs->avg_بھرو == busiest->avg_بھرو) {
			/*
			 * SMT sched groups need more help than non-SMT groups.
			 * If @sg happens to also be SMT, either choice is good.
			 */
			اگر (sds->busiest->flags & SD_SHARE_CPUCAPACITY)
				واپس false;
		}

		توڑو;

	حالت گروہ_has_فالتو:
		/*
		 * Do not pick sg with SMT CPUs over sg with pure CPUs,
		 * as we do not want to pull task off SMT core with one task
		 * and make the core idle.
		 */
		اگر (smt_vs_nonsmt_groups(sds->busiest, sg)) {
			اگر (sg->flags & SD_SHARE_CPUCAPACITY && sgs->مجموع_h_nr_running <= 1)
				واپس false;
			ورنہ
				واپس true;
		}
has_فالتو:

		/*
		 * Select not overloaded group with lowest number of idle CPUs
		 * and highest number of running tasks. We could also compare
		 * the spare capacity which is more stable but it can end up
		 * that the group has less spare capacity but finally more idle
		 * CPUs which means less opportunity to pull tasks.
		 */
		اگر (sgs->idle_cpus > busiest->idle_cpus)
			واپس false;
		ورنہ اگر ((sgs->idle_cpus == busiest->idle_cpus) &&
			 (sgs->مجموع_nr_running <= busiest->مجموع_nr_running))
			واپس false;

		توڑو;
	}

	واپس true;
}

#اگر_تعریف_شدہ CONFIG_NUMA_BALANCING
ساکن اندرونی شمار fbq_type fbq_classify_گروہ(ساخت sg_lb_stats *sgs)
{
	اگر (sgs->مجموع_h_nr_running > sgs->nr_numa_running)
		واپس regular;
	اگر (sgs->مجموع_h_nr_running > sgs->nr_preferred_running)
		واپس remote;
	واپس all;
}

ساکن اندرونی شمار fbq_type fbq_classify_rq(ساخت rq *rq)
{
	اگر (rq->nr_running > rq->nr_numa_running)
		واپس regular;
	اگر (rq->nr_running > rq->nr_preferred_running)
		واپس remote;
	واپس all;
}
#ورنہ /* !CONFIG_NUMA_BALANCING: */
ساکن اندرونی شمار fbq_type fbq_classify_گروہ(ساخت sg_lb_stats *sgs)
{
	واپس all;
}

ساکن اندرونی شمار fbq_type fbq_classify_rq(ساخت rq *rq)
{
	واپس regular;
}
#اگر_ختم /* !CONFIG_NUMA_BALANCING */


ساخت sg_lb_stats;

/*
 * task_running_on_cpu - return 1 if @p is running on @cpu.
 */

ساکن غیرعلامتی صحیح task_running_on_cpu(صحیح cpu, ساخت task_struct *p)
{
	/* Task has no contribution or is new */
	اگر (cpu != task_cpu(p) || !READ_ONCE(p->se.avg.آخری_update_وقت))
		واپس 0;

	اگر (task_on_rq_queued(p))
		واپس 1;

	واپس 0;
}

/**
 * idle_cpu_without - would a given CPU be idle without p ?
 * @cpu: the processor on which idleness is tested.
 * @p: task which should be ignored.
 *
 * Return: 1 if the CPU would be idle. 0 otherwise.
 */
ساکن صحیح idle_cpu_without(صحیح cpu, ساخت task_struct *p)
{
	ساخت rq *rq = cpu_rq(cpu);

	اگر (rq->curr != rq->idle && rq->curr != p)
		واپس 0;

	/*
	 * rq->nr_running can't be used but an updated version without the
	 * impact of p on cpu must be used instead. The updated nr_running
	 * be computed and tested before calling idle_cpu_without().
	 */

	اگر (rq->ttwu_pending)
		واپس 0;

	واپس 1;
}

/*
 * update_sg_wakeup_stats - Update sched_group's statistics for wakeup.
 * @sd: The sched_domain level to look for idlest group.
 * @group: sched_group whose statistics are to be updated.
 * @sgs: variable to hold the statistics for this group.
 * @p: The task for which we look for the idlest group/CPU.
 */
ساکن اندرونی خالی update_sg_wakeup_stats(ساخت sched_domain *sd,
					  ساخت sched_گروہ *گروہ,
					  ساخت sg_lb_stats *sgs,
					  ساخت task_struct *p)
{
	صحیح i, nr_running;

	memset(sgs, 0, حجم(*sgs));

	/* Assume that task can't fit any CPU of the group */
	اگر (sd->flags & SD_ASYM_CPUCAPACITY)
		sgs->گروہ_misfit_task_بھرو = 1;

	for_each_cpu_and(i, sched_گروہ_وسعت(گروہ), p->cpus_ptr) {
		ساخت rq *rq = cpu_rq(i);
		غیرعلامتی صحیح local;

		sgs->گروہ_بھرو += cpu_بھرو_without(rq, p);
		sgs->گروہ_util += cpu_util_without(i, p);
		sgs->گروہ_runnable += cpu_runnable_without(rq, p);
		local = task_running_on_cpu(i, p);
		sgs->مجموع_h_nr_running += rq->cfs.h_nr_runnable - local;

		nr_running = rq->nr_running - local;
		sgs->مجموع_nr_running += nr_running;

		/*
		 * No need to call idle_cpu_without() if nr_running is not 0
		 */
		اگر (!nr_running && idle_cpu_without(i, p))
			sgs->idle_cpus++;

		/* Check if task fits in the CPU */
		اگر (sd->flags & SD_ASYM_CPUCAPACITY &&
		    sgs->گروہ_misfit_task_بھرو &&
		    task_سماتا_cpu(p, i))
			sgs->گروہ_misfit_task_بھرو = 0;

	}

	sgs->گروہ_استعداد = گروہ->sgc->استعداد;

	sgs->گروہ_وزن = گروہ->گروہ_وزن;

	sgs->گروہ_type = گروہ_classify(sd->imbalance_pct, گروہ, sgs);

	/*
	 * Computing avg_load makes sense only when group is fully busy or
	 * overloaded
	 */
	اگر (sgs->گروہ_type == گروہ_fully_busy ||
		sgs->گروہ_type == گروہ_overloaded)
		sgs->avg_بھرو = (sgs->گروہ_بھرو * SCHED_استعداد_SCALE) /
				sgs->گروہ_استعداد;
}

ساکن bool update_pick_idlest(ساخت sched_گروہ *idlest,
			       ساخت sg_lb_stats *idlest_sgs,
			       ساخت sched_گروہ *گروہ,
			       ساخت sg_lb_stats *sgs)
{
	اگر (sgs->گروہ_type < idlest_sgs->گروہ_type)
		واپس true;

	اگر (sgs->گروہ_type > idlest_sgs->گروہ_type)
		واپس false;

	/*
	 * The candidate and the current idlest group are the same type of
	 * group. Let check which one is the idlest according to the type.
	 */

	انتخاب (sgs->گروہ_type) {
	حالت گروہ_overloaded:
	حالت گروہ_fully_busy:
		/* Select the group with lowest avg_load. */
		اگر (idlest_sgs->avg_بھرو <= sgs->avg_بھرو)
			واپس false;
		توڑو;

	حالت گروہ_llc_balance:
	حالت گروہ_imbalanced:
	حالت گروہ_asym_packing:
	حالت گروہ_smt_balance:
		/* Those types are not used in the slow wakeup path */
		واپس false;

	حالت گروہ_misfit_task:
		/* Select group with the highest max capacity */
		اگر (idlest->sgc->max_استعداد >= گروہ->sgc->max_استعداد)
			واپس false;
		توڑو;

	حالت گروہ_has_فالتو:
		/* Select group with most idle CPUs */
		اگر (idlest_sgs->idle_cpus > sgs->idle_cpus)
			واپس false;

		/* Select group with lowest group_util */
		اگر (idlest_sgs->idle_cpus == sgs->idle_cpus &&
			idlest_sgs->گروہ_util <= sgs->گروہ_util)
			واپس false;

		توڑو;
	}

	واپس true;
}

/*
 * sched_balance_find_dst_group() finds and returns the least busy CPU group within the
 * domain.
 *
 * Assumes p is allowed on at least one CPU in sd.
 */
ساکن ساخت sched_گروہ *
sched_balance_ڈھونڈو_dst_گروہ(ساخت sched_domain *sd, ساخت task_struct *p, صحیح this_cpu)
{
	ساخت sched_گروہ *idlest = تہی, *local = تہی, *گروہ = sd->groups;
	ساخت sg_lb_stats local_sgs, tmp_sgs;
	ساخت sg_lb_stats *sgs;
	غیرعلامتی طویل imbalance;
	ساخت sg_lb_stats idlest_sgs = {
			.avg_بھرو = UINT_MAX,
			.گروہ_type = گروہ_overloaded,
	};

	کرو {
		صحیح local_گروہ;

		/* Skip over this group if it has no CPUs allowed */
		اگر (!cpumask_intersects(sched_گروہ_وسعت(گروہ),
					p->cpus_ptr))
			جاری;

		/* Skip over this group if no cookie matched */
		اگر (!sched_گروہ_cookie_مطابقت(cpu_rq(this_cpu), p, گروہ))
			جاری;

		local_گروہ = cpumask_test_cpu(this_cpu,
					       sched_گروہ_وسعت(گروہ));

		اگر (local_گروہ) {
			sgs = &local_sgs;
			local = گروہ;
		} ورنہ {
			sgs = &tmp_sgs;
		}

		update_sg_wakeup_stats(sd, گروہ, sgs, p);

		اگر (!local_گروہ && update_pick_idlest(idlest, &idlest_sgs, گروہ, sgs)) {
			idlest = گروہ;
			idlest_sgs = *sgs;
		}

	} جب (گروہ = گروہ->اگلا, گروہ != sd->groups);


	/* There is no idlest group to push tasks to */
	اگر (!idlest)
		واپس تہی;

	/* The local group has been skipped because of CPU affinity */
	اگر (!local)
		واپس idlest;

	/*
	 * If the local group is idler than the selected idlest group
	 * don't try and push the task.
	 */
	اگر (local_sgs.گروہ_type < idlest_sgs.گروہ_type)
		واپس تہی;

	/*
	 * If the local group is busier than the selected idlest group
	 * try and push the task.
	 */
	اگر (local_sgs.گروہ_type > idlest_sgs.گروہ_type)
		واپس idlest;

	انتخاب (local_sgs.گروہ_type) {
	حالت گروہ_overloaded:
	حالت گروہ_fully_busy:

		/* Calculate allowed imbalance based on load */
		imbalance = scale_بھرو_نیچے_کو(NICE_0_بھرو) *
				(sd->imbalance_pct-100) / 100;

		/*
		 * When comparing groups across NUMA domains, it's possible for
		 * the local domain to be very lightly loaded relative to the
		 * remote domains but "imbalance" skews the comparison making
		 * remote CPUs look much more favourable. When considering
		 * cross-domain, add imbalance to the load on the remote node
		 * and consider staying local.
		 */

		اگر ((sd->flags & SD_NUMA) &&
		    ((idlest_sgs.avg_بھرو + imbalance) >= local_sgs.avg_بھرو))
			واپس تہی;

		/*
		 * If the local group is less loaded than the selected
		 * idlest group don't try and push any tasks.
		 */
		اگر (idlest_sgs.avg_بھرو >= (local_sgs.avg_بھرو + imbalance))
			واپس تہی;

		اگر (100 * local_sgs.avg_بھرو <= sd->imbalance_pct * idlest_sgs.avg_بھرو)
			واپس تہی;
		توڑو;

	حالت گروہ_llc_balance:
	حالت گروہ_imbalanced:
	حالت گروہ_asym_packing:
	حالت گروہ_smt_balance:
		/* Those type are not used in the slow wakeup path */
		واپس تہی;

	حالت گروہ_misfit_task:
		/* Select group with the highest max capacity */
		اگر (local->sgc->max_استعداد >= idlest->sgc->max_استعداد)
			واپس تہی;
		توڑو;

	حالت گروہ_has_فالتو:
#اگر_تعریف_شدہ CONFIG_NUMA
		اگر (sd->flags & SD_NUMA) {
			صحیح imb_numa_nr = sd->imb_numa_nr;
#اگر_تعریف_شدہ CONFIG_NUMA_BALANCING
			صحیح idlest_cpu;
			/*
			 * If there is spare capacity at NUMA, try to select
			 * the preferred node
			 */
			اگر (cpu_مقصد_گرہ(this_cpu) == p->numa_preferred_nid)
				واپس تہی;

			idlest_cpu = cpumask_پہلا(sched_گروہ_وسعت(idlest));
			اگر (cpu_مقصد_گرہ(idlest_cpu) == p->numa_preferred_nid)
				واپس idlest;
#اگر_ختم /* CONFIG_NUMA_BALANCING */
			/*
			 * Otherwise, keep the task close to the wakeup source
			 * and improve locality if the number of running tasks
			 * would remain below threshold where an imbalance is
			 * allowed while accounting for the possibility the
			 * task is pinned to a subset of CPUs. If there is a
			 * real need of migration, periodic load balance will
			 * take care of it.
			 */
			اگر (p->nr_cpus_allowed != NR_CPUS) {
				غیرعلامتی صحیح w = cpumask_وزن_and(p->cpus_ptr,
								sched_گروہ_وسعت(local));
				imb_numa_nr = min(w, sd->imb_numa_nr);
			}

			imbalance = مطلق_قدر(local_sgs.idle_cpus - idlest_sgs.idle_cpus);
			اگر (!adjust_numa_imbalance(imbalance,
						   local_sgs.مجموع_nr_running + 1,
						   imb_numa_nr)) {
				واپس تہی;
			}
		}
#اگر_ختم /* CONFIG_NUMA */

		/*
		 * Select group with highest number of idle CPUs. We could also
		 * compare the utilization which is more stable but it can end
		 * up that the group has less spare capacity but finally more
		 * idle CPUs which means more opportunity to run task.
		 */
		اگر (local_sgs.idle_cpus >= idlest_sgs.idle_cpus)
			واپس تہی;
		توڑو;
	}

	واپس idlest;
}

ساکن خالی update_idle_cpu_جائزہ(ساخت lb_env *env,
				 غیرعلامتی طویل مجموع_util)
{
	ساخت sched_domain_shared *sd_share;
	ساخت sched_domain *sd = env->sd;
	صحیح llc_وزن, pct;
	u64 x, y, tmp;
	/*
	 * Update the number of CPUs to scan in LLC domain, which could
	 * be used as a hint in select_idle_cpu(). The update of sd_share
	 * could be expensive because it is within a shared cache line.
	 * So the write of this hint only occurs during periodic load
	 * balancing, rather than CPU_NEWLY_IDLE, because the latter
	 * can fire way more frequently than the former.
	 */
	اگر (!sched_feat(SIS_UTIL) || env->idle == CPU_NEWLY_IDLE)
		واپس;

	sd_share = sd->shared;
	اگر (!sd_share)
		واپس;

	/*
	 * The number of CPUs to search drops as sum_util increases, when
	 * sum_util hits 85% or above, the scan stops.
	 * The reason to choose 85% as the threshold is because this is the
	 * imbalance_pct(117) when a LLC sched group is overloaded.
	 *
	 * let y = SCHED_CAPACITY_SCALE - p * x^2                       [1]
	 * and y'= y / SCHED_CAPACITY_SCALE
	 *
	 * x is the ratio of sum_util compared to the CPU capacity:
	 * x = sum_util / (llc_weight * SCHED_CAPACITY_SCALE)
	 * y' is the ratio of CPUs to be scanned in the LLC domain,
	 * and the number of CPUs to scan is calculated by:
	 *
	 * nr_scan = llc_weight * y'                                    [2]
	 *
	 * When x hits the threshold of overloaded, AKA, when
	 * x = 100 / pct, y drops to 0. According to [1],
	 * p should be SCHED_CAPACITY_SCALE * pct^2 / 10000
	 *
	 * Scale x by SCHED_CAPACITY_SCALE:
	 * x' = sum_util / llc_weight;                                  [3]
	 *
	 * and finally [1] becomes:
	 * y = SCHED_CAPACITY_SCALE -
	 *     x'^2 * pct^2 / (10000 * SCHED_CAPACITY_SCALE)            [4]
	 *
	 */
	/* equation [3] */
	x = مجموع_util;
	llc_وزن = sd->وسعت_وزن;
	do_div(x, llc_وزن);

	/* equation [4] */
	pct = sd->imbalance_pct;
	tmp = x * x * pct * pct;
	do_div(tmp, 10000 * SCHED_استعداد_SCALE);
	tmp = min_t(طویل, tmp, SCHED_استعداد_SCALE);
	y = SCHED_استعداد_SCALE - tmp;

	/* equation [2] */
	y *= llc_وزن;
	do_div(y, SCHED_استعداد_SCALE);
	اگر ((صحیح)y != sd_share->nr_idle_جائزہ)
		WRITE_ONCE(sd_share->nr_idle_جائزہ, (صحیح)y);
}

/**
 * update_sd_lb_stats - Update sched_domain's statistics for load balancing.
 * @env: The load balancing environment.
 * @sds: variable to hold the statistics for this sched_domain.
 */

ساکن اندرونی خالی update_sd_lb_stats(ساخت lb_env *env, ساخت sd_lb_stats *sds)
{
	ساخت sched_گروہ *sg = env->sd->groups;
	ساخت sg_lb_stats *local = &sds->local_stat;
	ساخت sg_lb_stats tmp_sgs;
	غیرعلامتی طویل مجموع_util = 0;
	bool sg_overloaded = 0, sg_overutilized = 0;

	env->dst_core_idle = !sched_smt_فعال() || is_core_idle(env->dst_cpu);

	کرو {
		ساخت sg_lb_stats *sgs = &tmp_sgs;
		صحیح local_گروہ;

		local_گروہ = cpumask_test_cpu(env->dst_cpu, sched_گروہ_وسعت(sg));
		اگر (local_گروہ) {
			sds->local = sg;
			sgs = local;

			اگر (env->idle != CPU_NEWLY_IDLE ||
			    وقت_after_eq(jiffies, sg->sgc->اگلا_update))
				update_گروہ_استعداد(env->sd, env->dst_cpu);
		}

		update_sg_lb_stats(env, sds, sg, sgs, &sg_overloaded);

		اگر (!local_گروہ && update_sd_pick_busiest(env, sds, sg, sgs)) {
			sds->busiest = sg;
			sds->busiest_stat = *sgs;
		}

		sg_overutilized |= sgs->گروہ_overutilized;

		/* Now, start updating sd_lb_stats */
		sds->کل_بھرو += sgs->گروہ_بھرو;
		sds->کل_استعداد += sgs->گروہ_استعداد;

		مجموع_util += sgs->گروہ_util;
		sg = sg->اگلا;
	} جب (sg != env->sd->groups);

	/*
	 * Indicate that the child domain of the busiest group prefers tasks
	 * go to a child's sibling domains first. NB the flags of a sched group
	 * are those of the child domain.
	 */
	اگر (sds->busiest)
		sds->prefer_sibling = !!(sds->busiest->flags & SD_PREFER_SIBLING);


	اگر (env->sd->flags & SD_NUMA)
		env->fbq_type = fbq_classify_گروہ(&sds->busiest_stat);

	اگر (!env->sd->والد) {
		/* update overload indicator if we are at root domain */
		سیٹ_rd_overloaded(env->dst_rq->rd, sg_overloaded);

		/* Update over-utilization (tipping point, U >= 0) indicator */
		سیٹ_rd_overutilized(env->dst_rq->rd, sg_overutilized);
	} ورنہ اگر (sg_overutilized) {
		سیٹ_rd_overutilized(env->dst_rq->rd, sg_overutilized);
	}

	update_idle_cpu_جائزہ(env, مجموع_util);
}

/**
 * calculate_imbalance - Calculate the amount of imbalance present within the
 *			 groups of a given sched_domain during load balance.
 * @env: load balance environment
 * @sds: statistics of the sched_domain whose imbalance is to be calculated.
 */
ساکن اندرونی خالی calculate_imbalance(ساخت lb_env *env, ساخت sd_lb_stats *sds)
{
	ساخت sg_lb_stats *local, *busiest;

	local = &sds->local_stat;
	busiest = &sds->busiest_stat;

	اگر (busiest->گروہ_type == گروہ_misfit_task) {
		اگر (env->sd->flags & SD_ASYM_CPUCAPACITY) {
			/* Set imbalance to allow misfit tasks to be balanced. */
			env->migration_type = migrate_misfit;
			env->imbalance = 1;
		} ورنہ {
			/*
			 * Set load imbalance to allow moving task from cpu
			 * with reduced capacity.
			 */
			env->migration_type = migrate_بھرو;
			env->imbalance = busiest->گروہ_misfit_task_بھرو;
		}
		واپس;
	}

	اگر (busiest->گروہ_type == گروہ_asym_packing) {
		/*
		 * In case of asym capacity, we will try to migrate all load to
		 * the preferred CPU.
		 */
		env->migration_type = migrate_task;
		env->imbalance = busiest->مجموع_h_nr_running;
		واپس;
	}

	اگر (busiest->گروہ_type == گروہ_smt_balance) {
		/* Reduce number of tasks sharing CPU capacity */
		env->migration_type = migrate_task;
		env->imbalance = 1;
		واپس;
	}

#اگر_تعریف_شدہ CONFIG_SCHED_CACHE
	اگر (busiest->گروہ_type == گروہ_llc_balance) {
		/* Move a task that prefer local LLC */
		env->migration_type = migrate_llc_task;
		env->imbalance = 1;
		واپس;
	}
#اگر_ختم

	اگر (busiest->گروہ_type == گروہ_imbalanced) {
		/*
		 * In the group_imb case we cannot rely on group-wide averages
		 * to ensure CPU-load equilibrium, try to move any task to fix
		 * the imbalance. The next load balance will take care of
		 * balancing back the system.
		 */
		env->migration_type = migrate_task;
		env->imbalance = 1;
		واپس;
	}

	/*
	 * Try to use spare capacity of local group without overloading it or
	 * emptying busiest.
	 */
	اگر (local->گروہ_type == گروہ_has_فالتو) {
		اگر ((busiest->گروہ_type > گروہ_fully_busy) &&
		    !(env->sd->flags & SD_SHARE_LLC)) {
			/*
			 * If busiest is overloaded, try to fill spare
			 * capacity. This might end up creating spare capacity
			 * in busiest or busiest still being overloaded but
			 * there is no simple way to directly compute the
			 * amount of load to migrate in order to balance the
			 * system.
			 */
			env->migration_type = migrate_util;
			env->imbalance = max(local->گروہ_استعداد, local->گروہ_util) -
					 local->گروہ_util;

			/*
			 * In some cases, the group's utilization is max or even
			 * higher than capacity because of migrations but the
			 * local CPU is (newly) idle. There is at least one
			 * waiting task in this overloaded busiest group. Let's
			 * try to pull it.
			 */
			اگر (env->idle && env->imbalance == 0) {
				env->migration_type = migrate_task;
				env->imbalance = 1;
			}

			واپس;
		}

		اگر (busiest->گروہ_وزن == 1 || sds->prefer_sibling) {
			/*
			 * When prefer sibling, evenly spread running tasks on
			 * groups.
			 */
			env->migration_type = migrate_task;
			env->imbalance = sibling_imbalance(env, sds, busiest, local);
		} ورنہ {

			/*
			 * If there is no overload, we just want to even the number of
			 * idle CPUs.
			 */
			env->migration_type = migrate_task;
			env->imbalance = max_t(طویل, 0,
					       (local->idle_cpus - busiest->idle_cpus));
		}

#اگر_تعریف_شدہ CONFIG_NUMA
		/* Consider allowing a small imbalance between NUMA groups */
		اگر (env->sd->flags & SD_NUMA) {
			env->imbalance = adjust_numa_imbalance(env->imbalance,
							       local->مجموع_nr_running + 1,
							       env->sd->imb_numa_nr);
		}
#اگر_ختم

		/* Number of tasks to move to restore balance */
		env->imbalance >>= 1;

		واپس;
	}

	/*
	 * Local is fully busy but has to take more load to relieve the
	 * busiest group
	 */
	اگر (local->گروہ_type < گروہ_overloaded) {
		/*
		 * Local will become overloaded so the avg_load metrics are
		 * finally needed.
		 */

		local->avg_بھرو = (local->گروہ_بھرو * SCHED_استعداد_SCALE) /
				  local->گروہ_استعداد;

		/*
		 * If the local group is more loaded than the selected
		 * busiest group don't try to pull any tasks.
		 */
		اگر (local->avg_بھرو >= busiest->avg_بھرو) {
			env->imbalance = 0;
			واپس;
		}

		sds->avg_بھرو = (sds->کل_بھرو * SCHED_استعداد_SCALE) /
				sds->کل_استعداد;

		/*
		 * If the local group is more loaded than the average system
		 * load, don't try to pull any tasks.
		 */
		اگر (local->avg_بھرو >= sds->avg_بھرو) {
			env->imbalance = 0;
			واپس;
		}

	}

	/*
	 * Both group are or will become overloaded and we're trying to get all
	 * the CPUs to the average_load, so we don't want to push ourselves
	 * above the average load, nor do we wish to reduce the max loaded CPU
	 * below the average load. At the same time, we also don't want to
	 * reduce the group load below the group capacity. Thus we look for
	 * the minimum possible imbalance.
	 */
	env->migration_type = migrate_بھرو;
	env->imbalance = min(
		(busiest->avg_بھرو - sds->avg_بھرو) * busiest->گروہ_استعداد,
		(sds->avg_بھرو - local->avg_بھرو) * local->گروہ_استعداد
	) / SCHED_استعداد_SCALE;
}

/******* sched_balance_find_src_group() helpers end here *********************/

/*
 * Decision matrix according to the local and busiest group type:
 *
 * busiest \ local has_spare fully_busy misfit asym imbalanced overloaded
 * has_spare        nr_idle   balanced   N/A    N/A  balanced   balanced
 * fully_busy       nr_idle   nr_idle    N/A    N/A  balanced   balanced
 * misfit_task      force     N/A        N/A    N/A  N/A        N/A
 * asym_packing     force     force      N/A    N/A  force      force
 * imbalanced       force     force      N/A    N/A  force      force
 * overloaded       force     force      N/A    N/A  force      avg_load
 *
 * N/A :      Not Applicable because already filtered while updating
 *            statistics.
 * balanced : The system is balanced for these 2 groups.
 * force :    Calculate the imbalance as load migration is probably needed.
 * avg_load : Only if imbalance is significant enough.
 * nr_idle :  dst_cpu is not busy and the number of idle CPUs is quite
 *            different in groups.
 */

/**
 * sched_balance_find_src_group - Returns the busiest group within the sched_domain
 * if there is an imbalance.
 * @env: The load balancing environment.
 *
 * Also calculates the amount of runnable load which should be moved
 * to restore balance.
 *
 * Return:	- The busiest group if imbalance exists.
 */
ساکن ساخت sched_گروہ *sched_balance_ڈھونڈو_src_گروہ(ساخت lb_env *env)
{
	ساخت sg_lb_stats *local, *busiest;
	ساخت sd_lb_stats sds;

	init_sd_lb_stats(&sds);

	/*
	 * Compute the various statistics relevant for load balancing at
	 * this level.
	 */
	update_sd_lb_stats(env, &sds);

	/* There is no busy sibling group to pull tasks from */
	اگر (!sds.busiest)
		جاؤ برآمد_متوازن;

	busiest = &sds.busiest_stat;

	/* Misfit tasks should be dealt with regardless of the avg load */
	اگر (busiest->گروہ_type == گروہ_misfit_task)
		جاؤ force_balance;

	اگر (!is_rd_overutilized(env->dst_rq->rd) &&
	    rcu_dereference_all(env->dst_rq->rd->pd))
		جاؤ برآمد_متوازن;

	/* ASYM feature bypasses nice load balance check */
	اگر (busiest->گروہ_type == گروہ_asym_packing)
		جاؤ force_balance;

	/*
	 * If the busiest group is imbalanced the below checks don't
	 * work because they assume all things are equal, which typically
	 * isn't true due to cpus_ptr constraints and the like.
	 */
	اگر (busiest->گروہ_type == گروہ_imbalanced)
		جاؤ force_balance;

	local = &sds.local_stat;
	/*
	 * If the local group is busier than the selected busiest group
	 * don't try and pull any tasks.
	 */
	اگر (local->گروہ_type > busiest->گروہ_type)
		جاؤ برآمد_متوازن;

	/*
	 * When groups are overloaded, use the avg_load to ensure fairness
	 * between tasks.
	 */
	اگر (local->گروہ_type == گروہ_overloaded) {
		/*
		 * If the local group is more loaded than the selected
		 * busiest group don't try to pull any tasks.
		 */
		اگر (local->avg_بھرو >= busiest->avg_بھرو)
			جاؤ برآمد_متوازن;

		/* XXX broken for overlapping NUMA groups */
		sds.avg_بھرو = (sds.کل_بھرو * SCHED_استعداد_SCALE) /
				sds.کل_استعداد;

		/*
		 * Don't pull any tasks if this group is already above the
		 * domain average load.
		 */
		اگر (local->avg_بھرو >= sds.avg_بھرو)
			جاؤ برآمد_متوازن;

		/*
		 * If the busiest group is more loaded, use imbalance_pct to be
		 * conservative.
		 */
		اگر (100 * busiest->avg_بھرو <=
				env->sd->imbalance_pct * local->avg_بھرو)
			جاؤ برآمد_متوازن;
	}

	/*
	 * Try to move all excess tasks to a sibling domain of the busiest
	 * group's child domain.
	 */
	اگر (sds.prefer_sibling && local->گروہ_type == گروہ_has_فالتو &&
	    (busiest->گروہ_type == گروہ_llc_balance ||
	    sibling_imbalance(env, &sds, busiest, local) > 1))
		جاؤ force_balance;

	اگر (busiest->گروہ_type != گروہ_overloaded) {
		اگر (!env->idle) {
			/*
			 * If the busiest group is not overloaded (and as a
			 * result the local one too) but this CPU is already
			 * busy, let another idle CPU try to pull task.
			 */
			جاؤ برآمد_متوازن;
		}

		اگر (busiest->گروہ_type == گروہ_smt_balance &&
		    smt_vs_nonsmt_groups(sds.local, sds.busiest)) {
			/* Let non SMT CPU pull from SMT CPU sharing with sibling */
			جاؤ force_balance;
		}

		اگر (busiest->گروہ_وزن > 1 &&
		    local->idle_cpus <= (busiest->idle_cpus + 1)) {
			/*
			 * If the busiest group is not overloaded
			 * and there is no imbalance between this and busiest
			 * group wrt idle CPUs, it is balanced. The imbalance
			 * becomes significant if the diff is greater than 1
			 * otherwise we might end up to just move the imbalance
			 * on another group. Of course this applies only if
			 * there is more than 1 CPU per group.
			 */
			جاؤ برآمد_متوازن;
		}

		اگر (busiest->مجموع_h_nr_running == 1) {
			/*
			 * busiest doesn't have any tasks waiting to run
			 */
			جاؤ برآمد_متوازن;
		}
	}

force_balance:
	/* Looks like there is an imbalance. Compute it */
	calculate_imbalance(env, &sds);
	واپس env->imbalance ? sds.busiest : تہی;

برآمد_متوازن:
	env->imbalance = 0;
	واپس تہی;
}

/*
 * sched_balance_find_src_rq - find the busiest runqueue among the CPUs in the group.
 */
ساکن ساخت rq *sched_balance_ڈھونڈو_src_rq(ساخت lb_env *env,
				     ساخت sched_گروہ *گروہ)
{
	ساخت rq *busiest = تہی, *rq;
	غیرعلامتی طویل busiest_util = 0, busiest_بھرو = 0, busiest_استعداد = 1;
	غیرعلامتی صحیح __maybe_unused busiest_pref_llc = 0;
	ساخت sched_domain __maybe_unused *sd_tmp;
	غیرعلامتی صحیح busiest_nr = 0;
	صحیح __maybe_unused dst_llc;
	صحیح i;

	for_each_cpu_and(i, sched_گروہ_وسعت(گروہ), env->cpus) {
		غیرعلامتی طویل استعداد, بھرو, util;
		غیرعلامتی صحیح nr_running;
		شمار fbq_type rt;

		rq = cpu_rq(i);
		rt = fbq_classify_rq(rq);

		/*
		 * We classify groups/runqueues into three groups:
		 *  - regular: there are !numa tasks
		 *  - remote:  there are numa tasks that run on the 'wrong' node
		 *  - all:     there is no distinction
		 *
		 * In order to avoid migrating ideally placed numa tasks,
		 * ignore those when there's better options.
		 *
		 * If we ignore the actual busiest queue to migrate another
		 * task, the next balance pass can still reduce the busiest
		 * queue by moving tasks around inside the node.
		 *
		 * If we cannot move enough load due to this classification
		 * the next pass will adjust the group classification and
		 * allow migration of more tasks.
		 *
		 * Both cases only affect the total convergence complexity.
		 */
		اگر (rt > env->fbq_type)
			جاری;

		nr_running = rq->cfs.h_nr_runnable;
		اگر (!nr_running)
			جاری;

		استعداد = استعداد_کا(i);

		/*
		 * For ASYM_CPUCAPACITY domains, don't pick a CPU that could
		 * eventually lead to active_balancing high->low capacity.
		 * Higher per-CPU capacity is considered better than balancing
		 * average load.
		 */
		اگر (env->sd->flags & SD_ASYM_CPUCAPACITY &&
		    nr_running == 1) {
			bool cluster_equal_cap = static_branch_unlikely(&sched_cluster_فعال) &&
						 (get_actual_cpu_استعداد(env->dst_cpu) ==
						  get_actual_cpu_استعداد(i));
			bool smt_degraded_cap = sched_smt_فعال() && !is_core_idle(i);

			/*
			 * Busy SMT siblings reduce the capacity of CPU @i. Do
			 * not skip it in this case.
			 *
			 * CONFIG_SCHED_CLUSTER requires balancing load across
			 * clusters of identical capacity, accounting for
			 * hardware and cpufreq pressure.
			 */
			اگر (!smt_degraded_cap && !cluster_equal_cap &&
			    !استعداد_greater(استعداد_کا(env->dst_cpu), استعداد))
				جاری;
		}

		/*
		 * Make sure we only pull tasks from a CPU of lower priority
		 * when balancing between SMT siblings.
		 *
		 * If balancing between cores, let lower priority CPUs help
		 * SMT cores with more than one busy sibling.
		 */
		اگر (sched_asym(env->sd, i, env->dst_cpu) && nr_running == 1)
			جاری;

		انتخاب (env->migration_type) {
		حالت migrate_بھرو:
			/*
			 * When comparing with load imbalance, use cpu_load()
			 * which is not scaled with the CPU capacity.
			 */
			بھرو = cpu_بھرو(rq);

			اگر (nr_running == 1 && بھرو > env->imbalance &&
			    !check_cpu_استعداد(rq, env->sd))
				توڑو;

			/*
			 * For the load comparisons with the other CPUs,
			 * consider the cpu_load() scaled with the CPU
			 * capacity, so that the load can be moved away
			 * from the CPU that is potentially running at a
			 * lower capacity.
			 *
			 * Thus we're looking for max(load_i / capacity_i),
			 * crosswise multiplication to rid ourselves of the
			 * division works out to:
			 * load_i * capacity_j > load_j * capacity_i;
			 * where j is our previous maximum.
			 */
			اگر (بھرو * busiest_استعداد > busiest_بھرو * استعداد) {
				busiest_بھرو = بھرو;
				busiest_استعداد = استعداد;
				busiest = rq;
			}
			توڑو;

		حالت migrate_util:
			util = cpu_util_cfs_boost(i);

			/*
			 * Don't try to pull utilization from a CPU with one
			 * running task. Whatever its utilization, we will fail
			 * detach the task.
			 */
			اگر (nr_running <= 1)
				جاری;

			اگر (busiest_util < util) {
				busiest_util = util;
				busiest = rq;
			}
			توڑو;

		حالت migrate_task:
			اگر (busiest_nr < nr_running) {
				busiest_nr = nr_running;
				busiest = rq;
			}
			توڑو;

		حالت migrate_misfit:
			/*
			 * For ASYM_CPUCAPACITY domains with misfit tasks we
			 * simply seek the "biggest" misfit task.
			 */
			اگر (rq->misfit_task_بھرو > busiest_بھرو) {
				busiest_بھرو = rq->misfit_task_بھرو;
				busiest = rq;
			}

			توڑو;

		حالت migrate_llc_task:
#اگر_تعریف_شدہ CONFIG_SCHED_CACHE
			sd_tmp = rcu_dereference_all(rq->sd);
			dst_llc = llc_id(env->dst_cpu);

			اگر (sd_tmp && (غیرعلامتی)dst_llc < sd_tmp->llc_max) {
				غیرعلامتی صحیح this_pref_llc =
					sd_tmp->llc_counts[dst_llc];

				اگر (busiest_pref_llc < this_pref_llc) {
					busiest_pref_llc = this_pref_llc;
					busiest = rq;
				}
			}
#اگر_ختم
			توڑو;

		}
	}

	واپس busiest;
}

/*
 * Max backoff if we encounter pinned tasks. Pretty arbitrary value, but
 * so long as it is large enough.
 */
#تعریف_کرو MAX_PINNED_INTERVAL	512

ساکن اندرونی bool
asym_فعال_balance(ساخت lb_env *env)
{
	/*
	 * ASYM_PACKING needs to force migrate tasks from busy but lower
	 * priority CPUs in order to pack all tasks in the highest priority
	 * CPUs. When done between cores, do it only if the whole core if the
	 * whole core is idle.
	 *
	 * If @env::src_cpu is an SMT core with busy siblings, let
	 * the lower priority @env::dst_cpu help it. Do not follow
	 * CPU priority.
	 */
	واپس env->idle && sched_use_asym_prio(env->sd, env->dst_cpu) &&
	       (sched_asym_prefer(env->dst_cpu, env->src_cpu) ||
		!sched_use_asym_prio(env->sd, env->src_cpu));
}

ساکن اندرونی bool
imbalanced_فعال_balance(ساخت lb_env *env)
{
	ساخت sched_domain *sd = env->sd;

	/*
	 * The imbalanced case includes the case of pinned tasks preventing a fair
	 * distribution of the load on the system but also the even distribution of the
	 * threads on a system with spare capacity
	 */
	اگر ((env->migration_type == migrate_task) &&
	    (sd->nr_balance_failed > sd->cache_nice_tries+2))
		واپس 1;

	واپس 0;
}

ساکن صحیح need_فعال_balance(ساخت lb_env *env)
{
	ساخت sched_domain *sd = env->sd;

	اگر (alb_break_llc(env))
		واپس 0;

	اگر (asym_فعال_balance(env))
		واپس 1;

	اگر (imbalanced_فعال_balance(env))
		واپس 1;

	/*
	 * The dst_cpu is idle and the src_cpu CPU has only 1 CFS task.
	 * It's worth migrating the task if the src_cpu's capacity is reduced
	 * because of other sched_class or IRQs if more capacity stays
	 * available on dst_cpu.
	 */
	اگر (env->idle &&
	    (env->src_rq->cfs.h_nr_runnable == 1)) {
		اگر ((check_cpu_استعداد(env->src_rq, sd)) &&
		    (استعداد_کا(env->src_cpu)*sd->imbalance_pct < استعداد_کا(env->dst_cpu)*100))
			واپس 1;
	}

	اگر (env->migration_type == migrate_misfit ||
	    env->migration_type == migrate_llc_task)
		واپس 1;

	واپس 0;
}

ساکن صحیح فعال_بھرو_balance_cpu_stop(خالی *ڈیٹا);
ساکن صحیح فعال_بھرو_balance_llc_cpu_stop(خالی *ڈیٹا);

/*
 * migration_type is checked elsewhere to decide migration policy, so
 * it shouldn't be repurposed just to flag an LLC-directed active
 * balance across the stopper. Pick the callback here instead.
 */
ساکن اندرونی cpu_stop_fn_t alb_stop_fn(ساخت lb_env *env)
{
	اگر (env->migration_type == migrate_llc_task)
		واپس فعال_بھرو_balance_llc_cpu_stop;

	واپس فعال_بھرو_balance_cpu_stop;
}

ساکن صحیح should_we_balance(ساخت lb_env *env)
{
	ساخت cpumask *swb_cpus = this_cpu_cpumask_var_ptr(should_we_balance_tmpmask);
	ساخت sched_گروہ *sg = env->sd->groups;
	صحیح cpu, idle_smt = -1;

	/*
	 * Ensure the balancing environment is consistent; can happen
	 * when the softirq triggers 'during' hotplug.
	 */
	اگر (!cpumask_test_cpu(env->dst_cpu, env->cpus))
		واپس 0;

	/*
	 * In the newly idle case, we will allow all the CPUs
	 * to do the newly idle load balance.
	 *
	 * However, we bail out if we already have tasks or a wakeup pending,
	 * to optimize wakeup latency.
	 */
	اگر (env->idle == CPU_NEWLY_IDLE) {
		اگر (env->dst_rq->nr_running > 0 || env->dst_rq->ttwu_pending)
			واپس 0;
		واپس 1;
	}

	cpumask_copy(swb_cpus, گروہ_balance_نقاب(sg));
	/* Try to find first idle CPU */
	for_each_cpu_and(cpu, swb_cpus, env->cpus) {
		اگر (!idle_cpu(cpu))
			جاری;

		/*
		 * Don't balance to idle SMT in busy core right away when
		 * balancing cores, but remember the first idle SMT CPU for
		 * later consideration.  Find CPU on an idle core first.
		 */
		اگر (sched_smt_فعال() &&
		    !(env->sd->flags & SD_SHARE_CPUCAPACITY) &&
		    !is_core_idle(cpu)) {
			اگر (idle_smt == -1)
				idle_smt = cpu;
			/*
			 * If the core is not idle, and first SMT sibling which is
			 * idle has been found, then its not needed to check other
			 * SMT siblings for idleness:
			 */
			cpumask_andnot(swb_cpus, swb_cpus, cpu_smt_نقاب(cpu));
			جاری;
		}

		/*
		 * Are we the first idle core in a non-SMT domain or higher,
		 * or the first idle CPU in a SMT domain?
		 */
		واپس cpu == env->dst_cpu;
	}

	/* Are we the first idle CPU with busy siblings? */
	اگر (idle_smt != -1)
		واپس idle_smt == env->dst_cpu;

	/* Are we the first CPU of this group ? */
	واپس گروہ_balance_cpu(sg) == env->dst_cpu;
}

ساکن خالی update_lb_imbalance_stat(ساخت lb_env *env, ساخت sched_domain *sd,
				     شمار cpu_idle_type idle)
{
	اگر (!schedstat_enabled())
		واپس;

	انتخاب (env->migration_type) {
	حالت migrate_بھرو:
		__schedstat_add(sd->lb_imbalance_بھرو[idle], env->imbalance);
		توڑو;
	حالت migrate_util:
		__schedstat_add(sd->lb_imbalance_util[idle], env->imbalance);
		توڑو;
	حالت migrate_task:
		__schedstat_add(sd->lb_imbalance_task[idle], env->imbalance);
		توڑو;
	حالت migrate_misfit:
		__schedstat_add(sd->lb_imbalance_misfit[idle], env->imbalance);
		توڑو;
	حالت migrate_llc_task:
		توڑو;
	}
}

/*
 * This flag serializes load-balancing passes over large domains
 * (above the NODE topology level) - only one load-balancing instance
 * may run at a time, to reduce overhead on very large systems with
 * lots of CPUs and large NUMA distances.
 *
 * - Note that load-balancing passes triggered while another one
 *   is executing are skipped and not re-tried.
 *
 * - Also note that this does not serialize rebalance_domains()
 *   execution, as non-SD_SERIALIZE domains will still be
 *   load-balanced in parallel.
 */
ساکن atomic_t sched_balance_running = ATOMIC_INIT(0);

/*
 * Check this_cpu to ensure it is balanced within domain. Attempt to move
 * tasks if there is an imbalance.
 */
ساکن صحیح sched_balance_rq(صحیح this_cpu, ساخت rq *this_rq,
			ساخت sched_domain *sd, شمار cpu_idle_type idle,
			صحیح *continue_balancing)
{
	صحیح ld_moved, cur_ld_moved, فعال_balance = 0;
	ساخت sched_domain *sd_والد = sd->والد;
	ساخت sched_گروہ *گروہ;
	ساخت rq *busiest;
	ساخت rq_flags rf;
	ساخت cpumask *cpus = this_cpu_cpumask_var_ptr(بھرو_balance_نقاب);
	ساخت lb_env env = {
		.sd		= sd,
		.dst_cpu	= this_cpu,
		.dst_rq		= this_rq,
		.dst_grpmask    = گروہ_balance_نقاب(sd->groups),
		.idle		= idle,
		.loop_break	= SCHED_NR_MIGRATE_BREAK,
		.cpus		= cpus,
		.fbq_type	= all,
		.tasks		= LIST_سر_INIT(env.tasks),
	};
	bool need_unlock = false;

	cpumask_and(cpus, sched_domain_وسعت(sd), cpu_preferred_نقاب);

	schedstat_inc(sd->lb_گنتی[idle]);

redo:
	اگر (!should_we_balance(&env)) {
		*continue_balancing = 0;
		جاؤ برآمد_متوازن;
	}

	اگر (!need_unlock && (sd->flags & SD_SERIALIZE)) {
		صحیح zero = 0;
		اگر (!atomic_try_cmpxchg_acquire(&sched_balance_running, &zero, 1))
			جاؤ برآمد_متوازن;

		need_unlock = true;
	}

	گروہ = sched_balance_ڈھونڈو_src_گروہ(&env);
	اگر (!گروہ) {
		schedstat_inc(sd->lb_nobusyg[idle]);
		جاؤ برآمد_متوازن;
	}

	busiest = sched_balance_ڈھونڈو_src_rq(&env, گروہ);
	اگر (!busiest) {
		schedstat_inc(sd->lb_nobusyq[idle]);
		جاؤ برآمد_متوازن;
	}

	WARN_ON_ONCE(busiest == env.dst_rq);

	update_lb_imbalance_stat(&env, sd, idle);

	env.src_cpu = busiest->cpu;
	env.src_rq = busiest;

	ld_moved = 0;
	/* Clear this flag as soon as we find a pullable task */
	env.flags |= LBF_ALL_PINNED;
	اگر (busiest->nr_running > 1) {
		/*
		 * Attempt to move tasks. If sched_balance_find_src_group has found
		 * an imbalance but busiest->nr_running <= 1, the group is
		 * still unbalanced. ld_moved simply stays zero, so it is
		 * correctly treated as an imbalance.
		 */
		env.loop_max  = min(sysctl_sched_nr_migrate, busiest->nr_running);

مزید_balance:
		rq_تالا_irqsave(busiest, &rf);
		update_rq_clock(busiest);

		/*
		 * cur_ld_moved - load moved in current iteration
		 * ld_moved     - cumulative load moved across iterations
		 */
		cur_ld_moved = detach_tasks(&env);

		/*
		 * We've detached some tasks from busiest_rq. Every
		 * task is masked "TASK_ON_RQ_MIGRATING", so we can safely
		 * unlock busiest->lock, and we are able to be sure
		 * that nobody can manipulate the tasks in parallel.
		 * See task_rq_lock() family for the details.
		 */

		rq_unlock(busiest, &rf);

		اگر (cur_ld_moved) {
			attach_tasks(&env);
			ld_moved += cur_ld_moved;
		}

		local_irq_restore(rf.flags);

		اگر (env.flags & LBF_NEED_BREAK) {
			env.flags &= ~LBF_NEED_BREAK;
			جاؤ مزید_balance;
		}

		/*
		 * Revisit (affine) tasks on src_cpu that couldn't be moved to
		 * us and move them to an alternate dst_cpu in our sched_group
		 * where they can run. The upper limit on how many times we
		 * iterate on same src_cpu is dependent on number of CPUs in our
		 * sched_group.
		 *
		 * This changes load balance semantics a bit on who can move
		 * load to a given_cpu. In addition to the given_cpu itself
		 * (or a ilb_cpu acting on its behalf where given_cpu is
		 * nohz-idle), we now have balance_cpu in a position to move
		 * load to given_cpu. In rare situations, this may cause
		 * conflicts (balance_cpu and given_cpu/ilb_cpu deciding
		 * _independently_ and at _same_ time to move some load to
		 * given_cpu) causing excess load to be moved to given_cpu.
		 * This however should not happen so much in practice and
		 * moreover subsequent load balance cycles should correct the
		 * excess load moved.
		 */
		اگر ((env.flags & LBF_DST_PINNED) && env.imbalance > 0) {

			/* Prevent to re-select dst_cpu via env's CPUs */
			__cpumask_clear_cpu(env.dst_cpu, env.cpus);

			env.dst_rq	 = cpu_rq(env.new_dst_cpu);
			env.dst_cpu	 = env.new_dst_cpu;
			env.flags	&= ~LBF_DST_PINNED;
			env.loop	 = 0;
			env.loop_break	 = SCHED_NR_MIGRATE_BREAK;

			/*
			 * Go back to "more_balance" rather than "redo" since we
			 * need to continue with same src_cpu.
			 */
			جاؤ مزید_balance;
		}

		/*
		 * We failed to reach balance because of affinity.
		 */
		اگر (sd_والد) {
			صحیح *گروہ_imbalance = &sd_والد->groups->sgc->imbalance;

			اگر ((env.flags & LBF_SOME_PINNED) && env.imbalance > 0)
				*گروہ_imbalance = 1;
		}

		/* All tasks on this runqueue were pinned by CPU affinity */
		اگر (unlikely(env.flags & LBF_ALL_PINNED)) {
			__cpumask_clear_cpu(cpu_کا(busiest), cpus);
			/*
			 * Attempting to continue load balancing at the current
			 * sched_domain level only makes sense if there are
			 * active CPUs remaining as possible busiest CPUs to
			 * pull load from which are not contained within the
			 * destination group that is receiving any migrated
			 * load.
			 */
			اگر (!cpumask_subset(cpus, env.dst_grpmask)) {
				env.loop = 0;
				env.loop_break = SCHED_NR_MIGRATE_BREAK;
				جاؤ redo;
			}
			جاؤ برآمد_all_pinned;
		}
	}

	اگر (ld_moved) {
		sd->nr_balance_failed = 0;
		جاؤ برآمد_unbalanced;
	}

	schedstat_inc(sd->lb_failed[idle]);
	/*
	 * Increment the failure counter only on periodic balance.
	 * We do not want newidle balance, which can be very
	 * frequent, pollute the failure counter causing
	 * excessive cache_hot migrations and active balances.
	 *
	 * Similarly for migration_misfit which is not related to
	 * load/util migration, don't pollute nr_balance_failed.
	 *
	 * The same for cache aware scheduling's allowance for
	 * load imbalance. If regular load balance does not
	 * migrate task due to LLC locality, it is a expected
	 * behavior and don't pollute nr_balance_failed.
	 * See can_migrate_task().
	 */
	اگر (idle != CPU_NEWLY_IDLE &&
	    env.migration_type != migrate_misfit &&
	    !(env.flags & LBF_LLC_PINNED))
		sd->nr_balance_failed++;

	اگر (!need_فعال_balance(&env))
		جاؤ برآمد_unbalanced;

	scoped_guard (raw_spin_rq_تالا_irqsave, busiest) {
		/*
		 * Don't kick the active_load_balance_cpu_stop,
		 * if the curr task on busiest CPU can't be
		 * moved to this_cpu:
		 */
		اگر (!cpumask_test_cpu(this_cpu, busiest->curr->cpus_ptr))
			جاؤ برآمد_one_pinned;

		/* Record that we found at least one task that could run on this_cpu */
		env.flags &= ~LBF_ALL_PINNED;

		/*
		 * ->active_balance synchronizes accesses to
		 * ->active_balance_work.  Once set, it's cleared
		 * only after active load balance is finished.
		 */
		اگر (busiest->فعال_balance)
			جاؤ برآمد_unbalanced;

		/*
		 * @busiest dropped its rq_lock in the middle of
		 * scheduling out its ->curr task (->on_rq := 0), no
		 * need to forcefully punt it away with active balance.
		 */
		اگر (!busiest->curr->on_rq)
			جاؤ برآمد_unbalanced;

		busiest->فعال_balance = 1;
		busiest->ڈالو_cpu = this_cpu;
		فعال_balance = 1;
		preempt_disable();
	}
	اگر (فعال_balance) {
		stop_one_cpu_nowait(cpu_کا(busiest),
				    alb_stop_fn(&env), busiest,
				    &busiest->فعال_balance_work);
	}
	preempt_enable();

برآمد_unbalanced:
	/* We were unbalanced, so reset the balancing interval */
	sd->balance_interval = sd->min_interval;
	جاؤ برآمد;

برآمد_متوازن:
	/*
	 * We reach balance although we may have faced some affinity
	 * constraints. Clear the imbalance flag only if other tasks got
	 * a chance to move and fix the imbalance.
	 */
	اگر (sd_والد && !(env.flags & LBF_ALL_PINNED)) {
		صحیح *گروہ_imbalance = &sd_والد->groups->sgc->imbalance;

		اگر (*گروہ_imbalance)
			*گروہ_imbalance = 0;
	}

برآمد_all_pinned:
	/*
	 * We reach balance because all tasks are pinned at this level so
	 * we can't migrate them. Let the imbalance flag set so parent level
	 * can try to migrate them.
	 */
	schedstat_inc(sd->lb_متوازن[idle]);

	sd->nr_balance_failed = 0;

برآمد_one_pinned:
	ld_moved = 0;

	/*
	 * sched_balance_newidle() disregards balance intervals, so we could
	 * repeatedly reach this code, which would lead to balance_interval
	 * skyrocketing in a short amount of time. Skip the balance_interval
	 * increase logic to avoid that.
	 *
	 * Similarly misfit migration which is not necessarily an indication of
	 * the system being busy and requires lb to backoff to let it settle
	 * down.
	 */
	اگر (env.idle == CPU_NEWLY_IDLE ||
	    env.migration_type == migrate_misfit)
		جاؤ برآمد;

	/* tune up the balancing interval */
	اگر ((env.flags & LBF_ALL_PINNED &&
	     sd->balance_interval < MAX_PINNED_INTERVAL) ||
	    sd->balance_interval < sd->max_interval)
		sd->balance_interval *= 2;
برآمد:
	اگر (need_unlock)
		atomic_سیٹ_release(&sched_balance_running, 0);

	واپس ld_moved;
}

ساکن اندرونی غیرعلامتی طویل
get_sd_balance_interval(ساخت sched_domain *sd, صحیح cpu_busy)
{
	غیرعلامتی طویل interval = sd->balance_interval;

	اگر (cpu_busy)
		interval *= sd->busy_ضربی_عدد;

	/* scale ms to jiffies */
	interval = msecs_مقصد_jiffies(interval);

	/*
	 * Reduce likelihood of busy balancing at higher domains racing with
	 * balancing at lower domains by preventing their balancing periods
	 * from being multiples of each other.
	 */
	اگر (cpu_busy)
		interval -= 1;

	interval = clamp(interval, 1UL, max_بھرو_balance_interval);

	واپس interval;
}

ساکن اندرونی خالی
update_اگلا_balance(ساخت sched_domain *sd, غیرعلامتی طویل *اگلا_balance)
{
	غیرعلامتی طویل interval, اگلا;

	/* used by idle balance, so cpu_busy = 0 */
	interval = get_sd_balance_interval(sd, 0);
	اگلا = sd->آخری_balance + interval;

	اگر (وقت_after(*اگلا_balance, اگلا))
		*اگلا_balance = اگلا;
}

/*
 * active_load_balance_cpu_stop is run by the CPU stopper. It pushes
 * running tasks off the busiest CPU onto idle CPUs. It requires at
 * least 1 task to be running on each physical CPU where possible, and
 * avoids physical / logical imbalances.
 */
ساکن صحیح __active_load_balance_cpu_stop(خالی *ڈیٹا, غیرعلامتی صحیح lb_flags)
{
	ساخت rq *busiest_rq = ڈیٹا;
	صحیح busiest_cpu = cpu_کا(busiest_rq);
	صحیح ہدف_cpu = busiest_rq->ڈالو_cpu;
	ساخت rq *ہدف_rq = cpu_rq(ہدف_cpu);
	ساخت sched_domain *sd;
	ساخت task_struct *p = تہی;
	ساخت rq_flags rf;

	rq_تالا_irq(busiest_rq, &rf);
	/*
	 * Between queueing the stop-work and running it is a hole in which
	 * CPUs can become inactive. We should not move tasks from or to
	 * inactive CPUs.
	 */
	اگر (!cpu_فعال(busiest_cpu) || !cpu_فعال(ہدف_cpu))
		جاؤ برآمد_unlock;

	/* Make sure the requested CPU hasn't gone down in the meantime: */
	اگر (unlikely(busiest_cpu != smp_processor_id() ||
		     !busiest_rq->فعال_balance))
		جاؤ برآمد_unlock;

	/* Is there any task to move? */
	اگر (busiest_rq->nr_running <= 1)
		جاؤ برآمد_unlock;

	/*
	 * This condition is "impossible", if it occurs
	 * we need to fix it. Originally reported by
	 * Bjorn Helgaas on a 128-CPU setup.
	 */
	WARN_ON_ONCE(busiest_rq == ہدف_rq);

	/* Search for an sd spanning us and the target CPU. */
	rcu_read_تالا();
	for_each_domain(ہدف_cpu, sd) {
		اگر (cpumask_test_cpu(busiest_cpu, sched_domain_وسعت(sd)))
			توڑو;
	}

	اگر (likely(sd)) {
		ساخت lb_env env = {
			.sd		= sd,
			.dst_cpu	= ہدف_cpu,
			.dst_rq		= ہدف_rq,
			.src_cpu	= busiest_rq->cpu,
			.src_rq		= busiest_rq,
			.idle		= CPU_IDLE,
			.flags		= LBF_فعال_LB | lb_flags,
		};

		schedstat_inc(sd->alb_گنتی);
		update_rq_clock(busiest_rq);

		p = detach_one_task(&env);
		اگر (p) {
			schedstat_inc(sd->alb_pushed);
			/* Active balancing done, reset the failure counter. */
			sd->nr_balance_failed = 0;
		} ورنہ {
			schedstat_inc(sd->alb_failed);
		}
	}
	rcu_read_unlock();
برآمد_unlock:
	busiest_rq->فعال_balance = 0;
	rq_unlock(busiest_rq, &rf);

	اگر (p)
		attach_one_task(ہدف_rq, p);

	local_irq_enable();

	واپس 0;
}

ساکن صحیح فعال_بھرو_balance_cpu_stop(خالی *ڈیٹا)
{
	واپس __active_load_balance_cpu_stop(ڈیٹا, 0);
}

ساکن صحیح فعال_بھرو_balance_llc_cpu_stop(خالی *ڈیٹا)
{
	واپس __active_load_balance_cpu_stop(ڈیٹا, LBF_فعال_LB_LLC);
}

/*
 * Scale the max sched_balance_rq interval with the number of CPUs in the system.
 * This trades load-balance latency on larger machines for less cross talk.
 */
خالی update_max_interval(خالی)
{
	max_بھرو_balance_interval = HZ*عدد_online_cpus()/10;
}

ساکن اندرونی خالی update_newidle_stats(ساخت sched_domain *sd, غیرعلامتی صحیح success)
{
	sd->newidle_call++;
	sd->newidle_success += success;

	اگر (sd->newidle_call >= 1024) {
		u64 now = sched_clock();
		s64 delta = now - sd->newidle_stamp;
		sd->newidle_stamp = now;
		صحیح تناسب = 0;

		اگر (delta < 0)
			delta = 0;

		اگر (sched_feat(NI_شرح)) {
			/*
			 * ratio  delta   freq
			 *
			 * 1024 -  4  s -  128 Hz
			 *  512 -  2  s -  256 Hz
			 *  256 -  1  s -  512 Hz
			 *  128 - .5  s - 1024 Hz
			 *   64 - .25 s - 2048 Hz
			 */
			تناسب = delta >> 22;
		}

		تناسب += sd->newidle_success;

		sd->newidle_تناسب = min(1024, تناسب);
		sd->newidle_call /= 2;
		sd->newidle_success /= 2;
	}
}

ساکن اندرونی bool
update_newidle_لاگت(ساخت sched_domain *sd, u64 لاگت, غیرعلامتی صحیح success)
{
	غیرعلامتی طویل اگلا_decay = sd->آخری_decay_max_lb_لاگت + HZ;
	غیرعلامتی طویل now = jiffies;

	اگر (لاگت)
		update_newidle_stats(sd, success);

	اگر (لاگت > sd->max_newidle_lb_لاگت) {
		/*
		 * Track max cost of a domain to make sure to not delay the
		 * next wakeup on the CPU.
		 */
		sd->max_newidle_lb_لاگت = لاگت;
		sd->آخری_decay_max_lb_لاگت = now;

	} ورنہ اگر (وقت_after(now, اگلا_decay)) {
		/*
		 * Decay the newidle max times by ~1% per second to ensure that
		 * it is not outdated and the current max cost is actually
		 * shorter.
		 */
		sd->max_newidle_lb_لاگت = (sd->max_newidle_lb_لاگت * 253) / 256;
		sd->آخری_decay_max_lb_لاگت = now;
		واپس true;
	}

	واپس false;
}

/*
 * It checks each scheduling domain to see if it is due to be balanced,
 * and initiates a balancing operation if so.
 *
 * Balancing parameters are set up in init_sched_domains.
 */
ساکن خالی sched_balance_domains(ساخت rq *rq, شمار cpu_idle_type idle)
{
	صحیح continue_balancing = 1;
	صحیح cpu = rq->cpu;
	صحیح busy = idle != CPU_IDLE && !sched_idle_rq(rq);
	غیرعلامتی طویل interval;
	ساخت sched_domain *sd;
	/* Earliest time when we have to do rebalance again */
	غیرعلامتی طویل اگلا_balance = jiffies + 60*HZ;
	صحیح update_اگلا_balance = 0;
	صحیح need_decay = 0;
	u64 max_لاگت = 0;

	rcu_read_تالا();
	for_each_domain(cpu, sd) {
		/*
		 * Decay the newidle max times here because this is a regular
		 * visit to all the domains.
		 */
		need_decay = update_newidle_لاگت(sd, 0, 0);
		max_لاگت += sd->max_newidle_lb_لاگت;

		/*
		 * Stop the load balance at this level. There is another
		 * CPU in our sched group which is doing load balancing more
		 * actively.
		 */
		اگر (!continue_balancing) {
			اگر (need_decay)
				جاری;
			توڑو;
		}

		interval = get_sd_balance_interval(sd, busy);
		اگر (وقت_after_eq(jiffies, sd->آخری_balance + interval)) {
			اگر (sched_balance_rq(cpu, rq, sd, idle, &continue_balancing)) {
				/*
				 * The LBF_DST_PINNED logic could have changed
				 * env->dst_cpu, so we can't know our idle
				 * state even if we migrated tasks. Update it.
				 */
				idle = idle_cpu(cpu);
				busy = !idle && !sched_idle_rq(rq);
			}
			sd->آخری_balance = jiffies;
			interval = get_sd_balance_interval(sd, busy);
		}
		اگر (وقت_after(اگلا_balance, sd->آخری_balance + interval)) {
			اگلا_balance = sd->آخری_balance + interval;
			update_اگلا_balance = 1;
		}
	}
	اگر (need_decay) {
		/*
		 * Ensure the rq-wide value also decays but keep it at a
		 * reasonable floor to avoid funnies with rq->avg_idle.
		 */
		rq->max_idle_balance_لاگت =
			max((u64)sysctl_sched_migration_لاگت, max_لاگت);
	}
	rcu_read_unlock();

	/*
	 * next_balance will be updated only when there is a need.
	 * When the cpu is attached to null domain for ex, it will not be
	 * updated.
	 */
	اگر (likely(update_اگلا_balance))
		rq->اگلا_balance = اگلا_balance;

}

ساکن اندرونی صحیح on_null_domain(ساخت rq *rq)
{
	واپس unlikely(!rcu_dereference_sched(rq->sd));
}

#اگر_تعریف_شدہ CONFIG_NO_HZ_COMMON
/*
 * NOHZ idle load balancing (ILB) details:
 *
 * - When one of the busy CPUs notices that there may be an idle rebalancing
 *   needed, they will kick the idle load balancer, which then does idle
 *   load balancing for all the idle CPUs.
 */
ساکن اندرونی صحیح ڈھونڈو_new_ilb(خالی)
{
	ساخت cpumask *ilb_cpus;
	صحیح ilb_cpu, fallback = -1;

	lockdep_assert_irqs_disabled();

	/*
	 * Reuse the per-CPU select_rq_mask, which is protected from concurrent
	 * use on this CPU by having interrupts disabled.
	 */
	ilb_cpus = this_cpu_cpumask_var_ptr(select_rq_نقاب);
	cpumask_and(ilb_cpus, nohz.idle_cpus_نقاب,
		    housekeeping_cpumask(HK_TYPE_KERNEL_NOISE));

	for_each_cpu(ilb_cpu, ilb_cpus) {
		اگر (!idle_cpu(ilb_cpu)) {
			/*
			 * Once an idle fallback exists, a busy CPU proves that
			 * this core cannot be fully idle. Skip its siblings.
			 */
			اگر (sched_smt_فعال() && fallback >= 0)
				cpumask_andnot(ilb_cpus, ilb_cpus, cpu_smt_نقاب(ilb_cpu));
			جاری;
		}

		/*
		 * Running the idle load balancer on an idle sibling of a busy
		 * SMT core can reduce the capacity available to its sibling. Prefer
		 * a CPU whose entire core is idle, but retain the first idle CPU as
		 * a fallback so idle balancing can still make progress when no fully
		 * idle core exists.
		 */
		اگر (sched_smt_فعال() && !is_core_idle(ilb_cpu)) {
			اگر (fallback < 0)
				fallback = ilb_cpu;

			/*
			 * The core is not idle, so there is no need to check
			 * any of its other SMT siblings.
			 */
			cpumask_andnot(ilb_cpus, ilb_cpus,
				       cpu_smt_نقاب(ilb_cpu));
			جاری;
		}

		واپس ilb_cpu;
	}

	واپس fallback;
}

/*
 * Kick a CPU to do the NOHZ balancing, if it is time for it, via a cross-CPU
 * SMP function call (IPI).
 *
 * Prefer a CPU on a fully idle core in the HK_TYPE_KERNEL_NOISE housekeeping
 * set. Fall back to the first idle CPU when no fully idle core exists.
 */
ساکن خالی kick_ilb(غیرعلامتی صحیح flags)
{
	صحیح ilb_cpu;

	/*
	 * Increase nohz.next_balance only when if full ilb is triggered but
	 * not if we only update stats.
	 */
	اگر (flags & NOHZ_BALANCE_KICK)
		nohz.اگلا_balance = jiffies+1;

	ilb_cpu = ڈھونڈو_new_ilb();
	اگر (ilb_cpu < 0)
		واپس;

	/*
	 * Don't bother if no new NOHZ balance work items for ilb_cpu,
	 * i.e. all bits in flags are already set in ilb_cpu.
	 */
	اگر ((atomic_read(nohz_flags(ilb_cpu)) & flags) == flags)
		واپس;

	/*
	 * Access to rq::nohz_csd is serialized by NOHZ_KICK_MASK; he who sets
	 * the first flag owns it; cleared by nohz_csd_func().
	 */
	flags = atomic_fetch_or(flags, nohz_flags(ilb_cpu));
	اگر (flags & NOHZ_KICK_نقاب)
		واپس;

	/*
	 * This way we generate an IPI on the target CPU which
	 * is idle, and the softirq performing NOHZ idle load balancing
	 * will be run before returning from the IPI.
	 */
	smp_call_function_single_async(ilb_cpu, &cpu_rq(ilb_cpu)->nohz_csd);
}

/*
 * Current decision point for kicking the idle load balancer in the presence
 * of idle CPUs in the system.
 */
ساکن خالی nohz_balancer_kick(ساخت rq *rq)
{
	غیرعلامتی طویل now = jiffies;
	ساخت sched_domain_shared *sds;
	ساخت sched_domain *sd;
	صحیح nr_busy, i, cpu = rq->cpu;
	غیرعلامتی صحیح flags = 0;

	اگر (unlikely(rq->idle_balance))
		واپس;

	/*
	 * We may be recently in ticked or tickless idle mode. At the first
	 * busy tick after returning from idle, we will update the busy stats.
	 */
	nohz_balance_exit_idle(rq);

	اگر (READ_ONCE(nohz.has_blocked_بھرو) &&
	    وقت_after(now, READ_ONCE(nohz.اگلا_blocked)))
		flags = NOHZ_STATS_KICK;

	/*
	 * Most of the time system is not 100% busy. i.e nohz.nr_cpus > 0
	 * Skip the read if time is not due.
	 *
	 * If none are in tickless mode, there maybe a narrow window
	 * (28 jiffies, HZ=1000) where flags maybe set and kick_ilb called.
	 * But idle load balancing is not done as find_new_ilb fails.
	 * That's very rare. So read nohz.nr_cpus only if time is due.
	 */
	اگر (وقت_before(now, nohz.اگلا_balance))
		جاؤ برآمد;

	/*
	 * None are in tickless mode and hence no need for NOHZ idle load
	 * balancing
	 */
	اگر (unlikely(cpumask_خالی_خانے(nohz.idle_cpus_نقاب)))
		واپس;

	اگر (rq->nr_running >= 2) {
		flags = NOHZ_STATS_KICK | NOHZ_BALANCE_KICK;
		جاؤ برآمد;
	}

	sd = rcu_dereference_all(rq->sd);
	اگر (sd) {
		/*
		 * If there's a runnable CFS task and the current CPU has reduced
		 * capacity, kick the ILB to see if there's a better CPU to run on:
		 */
		اگر (rq->cfs.h_nr_runnable >= 1 && check_cpu_استعداد(rq, sd)) {
			flags |= NOHZ_STATS_KICK | NOHZ_BALANCE_KICK;
			جاؤ برآمد;
		}
	}

	sd = rcu_dereference_all(فی_cpu(sd_asym_packing, cpu));
	اگر (sd) {
		/*
		 * When ASYM_PACKING; see if there's a more preferred CPU
		 * currently idle; in which case, kick the ILB to move tasks
		 * around.
		 *
		 * When balancing between cores, all the SMT siblings of the
		 * preferred CPU must be idle.
		 */
		for_each_cpu_and(i, sched_domain_وسعت(sd), nohz.idle_cpus_نقاب) {
			اگر (sched_asym(sd, i, cpu)) {
				flags |= NOHZ_STATS_KICK | NOHZ_BALANCE_KICK;
				جاؤ برآمد;
			}
		}
	}

	sd = rcu_dereference_all(فی_cpu(sd_asym_cpucapacity, cpu));
	اگر (sd) {
		/*
		 * When ASYM_CPUCAPACITY; see if there's a higher capacity CPU
		 * to run the misfit task on.
		 */
		اگر (check_misfit_حالت(rq))
			flags |= NOHZ_STATS_KICK | NOHZ_BALANCE_KICK;

		/*
		 * For asymmetric systems, we do not want to nicely balance
		 * cache use, instead we want to embrace asymmetry and only
		 * ensure tasks have enough CPU capacity.
		 *
		 * Skip the LLC logic because it's not relevant in that case.
		 */
		جاؤ برآمد;
	}

	sds = rcu_dereference_all(فی_cpu(sd_balance_shared, cpu));
	اگر (sds) {
		/*
		 * If there is an imbalance between LLC domains (IOW we could
		 * increase the overall cache utilization), we need a less-loaded LLC
		 * domain to pull some load from. Likewise, we may need to spread
		 * load within the current LLC domain (e.g. packed SMT cores but
		 * other CPUs are idle). We can't really know from here how busy
		 * the others are - so just get a NOHZ balance going if it looks
		 * like this LLC domain has tasks we could move.
		 */
		nr_busy = atomic_read(&sds->nr_busy_cpus);
		اگر (nr_busy > 1)
			flags |= NOHZ_STATS_KICK | NOHZ_BALANCE_KICK;
	}
برآمد:
	اگر (READ_ONCE(nohz.needs_update))
		flags |= NOHZ_اگلا_KICK;

	اگر (flags)
		kick_ilb(flags);
}

ساکن خالی سیٹ_cpu_sd_state_busy(صحیح cpu)
{
	ساخت sched_domain *sd;
	sd = rcu_dereference_all(فی_cpu(sd_llc, cpu));

	/*
	 * sd->nohz_idle only pairs with nr_busy_cpus on sd->shared; if this
	 * domain has no shared object there is nothing to clear or account.
	 */
	اگر (!sd || !sd->shared || !sd->nohz_idle)
		واپس;
	sd->nohz_idle = 0;

	atomic_inc(&sd->shared->nr_busy_cpus);
}

خالی nohz_balance_exit_idle(ساخت rq *rq)
{
	WARN_ON_ONCE(rq != this_rq());

	اگر (likely(!rq->nohz_tick_stopped))
		واپس;

	rq->nohz_tick_stopped = 0;
	cpumask_clear_cpu(rq->cpu, nohz.idle_cpus_نقاب);

	سیٹ_cpu_sd_state_busy(rq->cpu);
}

ساکن خالی سیٹ_cpu_sd_state_idle(صحیح cpu)
{
	ساخت sched_domain *sd;
	sd = rcu_dereference_all(فی_cpu(sd_llc, cpu));

	/* See set_cpu_sd_state_busy(): nohz_idle is only used with sd->shared. */
	اگر (!sd || !sd->shared || sd->nohz_idle)
		واپس;
	sd->nohz_idle = 1;

	atomic_dec(&sd->shared->nr_busy_cpus);
}

/*
 * This routine will record that the CPU is going idle with tick stopped.
 * This info will be used in performing idle load balancing in the future.
 */
خالی nohz_balance_enter_idle(صحیح cpu)
{
	ساخت rq *rq = cpu_rq(cpu);

	WARN_ON_ONCE(cpu != smp_processor_id());

	/* If this CPU is going down, then nothing needs to be done: */
	اگر (!cpu_فعال(cpu))
		واپس;

	/*
	 * Can be set safely without rq->lock held
	 * If a clear happens, it will have evaluated last additions because
	 * rq->lock is held during the check and the clear
	 */
	rq->has_blocked_بھرو = 1;

	/*
	 * The tick is still stopped but load could have been added in the
	 * meantime. We set the nohz.has_blocked_load flag to trig a check of the
	 * *_avg. The CPU is already part of nohz.idle_cpus_mask so the clear
	 * of nohz.has_blocked_load can only happen after checking the new load
	 */
	اگر (rq->nohz_tick_stopped)
		جاؤ برآمد;

	/* If we're a completely isolated CPU, we don't play: */
	اگر (on_null_domain(rq))
		واپس;

	rq->nohz_tick_stopped = 1;

	cpumask_سیٹ_cpu(cpu, nohz.idle_cpus_نقاب);

	/*
	 * Ensures that if nohz_idle_balance() fails to observe our
	 * @idle_cpus_mask store, it must observe the @has_blocked_load
	 * and @needs_update stores.
	 */
	smp_mb__after_atomic();

	سیٹ_cpu_sd_state_idle(cpu);

	WRITE_ONCE(nohz.needs_update, 1);
برآمد:
	/*
	 * Each time a cpu enter idle, we assume that it has blocked load and
	 * enable the periodic update of the load of idle CPUs
	 */
	WRITE_ONCE(nohz.has_blocked_بھرو, 1);
}

ساکن bool update_nohz_stats(ساخت rq *rq)
{
	غیرعلامتی صحیح cpu = rq->cpu;

	اگر (!rq->has_blocked_بھرو)
		واپس false;

	اگر (!cpumask_test_cpu(cpu, nohz.idle_cpus_نقاب))
		واپس false;

	اگر (!وقت_after(jiffies, READ_ONCE(rq->آخری_blocked_بھرو_update_tick)))
		واپس true;

	sched_balance_update_blocked_averages(cpu);

	واپس rq->has_blocked_بھرو;
}

/*
 * Internal function that runs load balance for all idle CPUs. The load balance
 * can be a simple update of blocked load or a complete load balance with
 * tasks movement depending of flags.
 */
ساکن خالی _nohz_idle_balance(ساخت rq *this_rq, غیرعلامتی صحیح flags)
{
	/* Earliest time when we have to do rebalance again */
	غیرعلامتی طویل now = jiffies;
	غیرعلامتی طویل اگلا_balance = now + 60*HZ;
	bool has_blocked_بھرو = false;
	صحیح update_اگلا_balance = 0;
	صحیح this_cpu = this_rq->cpu;
	صحیح balance_cpu;
	ساخت rq *rq;

	WARN_ON_ONCE((flags & NOHZ_KICK_نقاب) == NOHZ_BALANCE_KICK);

	/*
	 * We assume there will be no idle load after this update and clear
	 * the has_blocked_load flag. If a cpu enters idle in the mean time, it will
	 * set the has_blocked_load flag and trigger another update of idle load.
	 * Because a cpu that becomes idle, is added to idle_cpus_mask before
	 * setting the flag, we are sure to not clear the state and not
	 * check the load of an idle cpu.
	 *
	 * Same applies to idle_cpus_mask vs needs_update.
	 */
	اگر (flags & NOHZ_STATS_KICK)
		WRITE_ONCE(nohz.has_blocked_بھرو, 0);
	اگر (flags & NOHZ_اگلا_KICK)
		WRITE_ONCE(nohz.needs_update, 0);

	/*
	 * Ensures that if we miss the CPU, we must see the has_blocked_load
	 * store from nohz_balance_enter_idle().
	 */
	smp_mb();

	/*
	 * Start with the next CPU after this_cpu so we will end with this_cpu and let a
	 * chance for other idle cpu to pull load.
	 */
	for_each_cpu_wrap(balance_cpu,  nohz.idle_cpus_نقاب, this_cpu+1) {
		اگر (!idle_cpu(balance_cpu))
			جاری;

		/*
		 * If this CPU gets work to do, stop the load balancing
		 * work being done for other CPUs. Next load
		 * balancing owner will pick it up.
		 */
		اگر (!idle_cpu(this_cpu) && need_resched()) {
			اگر (flags & NOHZ_STATS_KICK)
				has_blocked_بھرو = true;
			اگر (flags & NOHZ_اگلا_KICK)
				WRITE_ONCE(nohz.needs_update, 1);
			جاؤ abort;
		}

		rq = cpu_rq(balance_cpu);

		اگر (flags & NOHZ_STATS_KICK)
			has_blocked_بھرو |= update_nohz_stats(rq);

		/*
		 * If time for next balance is due,
		 * do the balance.
		 */
		اگر (وقت_after_eq(jiffies, rq->اگلا_balance)) {
			ساخت rq_flags rf;

			rq_تالا_irqsave(rq, &rf);
			update_rq_clock(rq);
			rq_unlock_irqrestore(rq, &rf);

			اگر (flags & NOHZ_BALANCE_KICK)
				sched_balance_domains(rq, CPU_IDLE);
		}

		اگر (وقت_after(اگلا_balance, rq->اگلا_balance)) {
			اگلا_balance = rq->اگلا_balance;
			update_اگلا_balance = 1;
		}
	}

	/*
	 * next_balance will be updated only when there is a need.
	 * When the CPU is attached to null domain for ex, it will not be
	 * updated.
	 */
	اگر (likely(update_اگلا_balance))
		nohz.اگلا_balance = اگلا_balance;

	اگر (flags & NOHZ_STATS_KICK)
		WRITE_ONCE(nohz.اگلا_blocked,
			   now + msecs_مقصد_jiffies(بھرو_AVG_PERIOD));

abort:
	/* There is still blocked load, enable periodic update */
	اگر (has_blocked_بھرو)
		WRITE_ONCE(nohz.has_blocked_بھرو, 1);
}

/*
 * In CONFIG_NO_HZ_COMMON case, the idle balance kickee will do the
 * rebalancing for all the CPUs for whom scheduler ticks are stopped.
 */
ساکن bool nohz_idle_balance(ساخت rq *this_rq, شمار cpu_idle_type idle)
{
	غیرعلامتی صحیح flags = this_rq->nohz_idle_balance;

	اگر (!flags)
		واپس false;

	this_rq->nohz_idle_balance = 0;

	اگر (idle != CPU_IDLE)
		واپس false;

	_nohz_idle_balance(this_rq, flags);

	واپس true;
}

/*
 * Check if we need to directly run the ILB for updating blocked load before
 * entering idle state. Here we run ILB directly without issuing IPIs.
 *
 * Note that when this function is called, the tick may not yet be stopped on
 * this CPU yet. nohz.idle_cpus_mask is updated only when tick is stopped and
 * cleared on the next busy tick. In other words, nohz.idle_cpus_mask updates
 * don't align with CPUs enter/exit idle to avoid bottlenecks due to high idle
 * entry/exit rate (usec). So it is possible that _nohz_idle_balance() is
 * called from this function on (this) CPU that's not yet in the mask. That's
 * OK because the goal of nohz_run_idle_balance() is to run ILB only for
 * updating the blocked load of already idle CPUs without waking up one of
 * those idle CPUs and outside the preempt disable / IRQ off phase of the local
 * cpu about to enter idle, because it can take a long time.
 */
خالی nohz_تسلسل_idle_balance(صحیح cpu)
{
	غیرعلامتی صحیح flags;

	flags = atomic_fetch_andnot(NOHZ_NEWILB_KICK, nohz_flags(cpu));

	/*
	 * Update the blocked load only if no SCHED_SOFTIRQ is about to happen
	 * (i.e. NOHZ_STATS_KICK set) and will do the same.
	 */
	اگر ((flags == NOHZ_NEWILB_KICK) && !need_resched())
		_nohz_idle_balance(cpu_rq(cpu), NOHZ_STATS_KICK);
}

ساکن خالی nohz_newidle_balance(ساخت rq *this_rq)
{
	صحیح this_cpu = this_rq->cpu;

	/* Will wake up very soon. No time for doing anything else*/
	اگر (this_rq->avg_idle < sysctl_sched_migration_لاگت)
		واپس;

	/* Don't need to update blocked load of idle CPUs*/
	اگر (!READ_ONCE(nohz.has_blocked_بھرو) ||
	    وقت_before(jiffies, READ_ONCE(nohz.اگلا_blocked)))
		واپس;

	/*
	 * Set the need to trigger ILB in order to update blocked load
	 * before entering idle state.
	 */
	atomic_or(NOHZ_NEWILB_KICK, nohz_flags(this_cpu));
}

#ورنہ /* !CONFIG_NO_HZ_COMMON: */
ساکن اندرونی خالی nohz_balancer_kick(ساخت rq *rq) { }

ساکن اندرونی bool nohz_idle_balance(ساخت rq *this_rq, شمار cpu_idle_type idle)
{
	واپس false;
}

ساکن اندرونی خالی nohz_newidle_balance(ساخت rq *this_rq) { }
#اگر_ختم /* !CONFIG_NO_HZ_COMMON */

/*
 * sched_balance_newidle is called by schedule() if this_cpu is about to become
 * idle. Attempts to pull tasks from other CPUs.
 *
 * Returns:
 *   < 0 - we released the lock and there are !fair tasks present
 *     0 - failed, no new tasks
 *   > 0 - success, new (fair) tasks present
 */
ساکن صحیح sched_balance_newidle(ساخت rq *this_rq, ساخت rq_flags *rf)
	__must_hold(__rq_lockp(this_rq))
{
	غیرعلامتی طویل اگلا_balance = jiffies + HZ;
	صحیح this_cpu = this_rq->cpu;
	صحیح continue_balancing = 1;
	u64 t0, t1, curr_لاگت = 0;
	ساخت sched_domain *sd;
	صحیح pulled_task = 0;

	update_misfit_حالت(تہی, this_rq);

	/*
	 * There is a task waiting to run. No need to search for one.
	 * Return 0; the task will be enqueued when switching to idle.
	 */
	اگر (this_rq->ttwu_pending)
		واپس 0;

	/*
	 * We must set idle_stamp _before_ calling sched_balance_rq()
	 * for CPU_NEWLY_IDLE, such that we measure the this duration
	 * as idle time.
	 */
	this_rq->idle_stamp = rq_clock(this_rq);

	/* Do not pull tasks towards !preferred CPUs */
	اگر (!cpu_preferred(this_cpu))
		واپس 0;

	/*
	 * This is OK, because current is on_cpu, which avoids it being picked
	 * for load-balance and preemption/IRQs are still disabled avoiding
	 * further scheduler activity on it and we're being very careful to
	 * re-start the picking loop.
	 */
	rq_unpin_تالا(this_rq, rf);

	sd = rcu_dereference_sched_domain(this_rq->sd);
	اگر (!sd)
		جاؤ برآمد;

	اگر (!get_rd_overloaded(this_rq->rd) ||
	    this_rq->avg_idle < sd->max_newidle_lb_لاگت) {

		update_اگلا_balance(sd, &اگلا_balance);
		جاؤ برآمد;
	}

	/*
	 * Include sched_balance_update_blocked_averages() in the cost
	 * calculation because it can be quite costly -- this ensures we skip
	 * it when avg_idle gets to be very low.
	 */
	t0 = sched_clock_cpu(this_cpu);
	__sched_balance_update_blocked_averages(this_rq);

	rq_modified_begin(this_rq, &fair_sched_class);
	raw_spin_rq_unlock(this_rq);

	for_each_domain(this_cpu, sd) {
		u64 domain_لاگت;

		update_اگلا_balance(sd, &اگلا_balance);

		اگر (this_rq->avg_idle < curr_لاگت + sd->max_newidle_lb_لاگت)
			توڑو;

		اگر (sd->flags & SD_BALANCE_NEWIDLE) {
			غیرعلامتی صحیح وزن = 1;

			اگر (sched_feat(NI_RANDOM) && sd->newidle_تناسب < 1024) {
				/*
				 * Throw a 1k sided dice; and only run
				 * newidle_balance according to the success
				 * rate.
				 */
				u32 d1k = sched_rng() % 1024;
				وزن = 1 + sd->newidle_تناسب;
				اگر (d1k > وزن) {
					update_newidle_stats(sd, 0);
					جاری;
				}
				وزن = (1024 + وزن/2) / وزن;
			}

			pulled_task = sched_balance_rq(this_cpu, this_rq,
						   sd, CPU_NEWLY_IDLE,
						   &continue_balancing);

			t1 = sched_clock_cpu(this_cpu);
			domain_لاگت = t1 - t0;
			curr_لاگت += domain_لاگت;
			t0 = t1;

			/*
			 * Track max cost of a domain to make sure to not delay the
			 * next wakeup on the CPU.
			 */
			update_newidle_لاگت(sd, domain_لاگت, وزن * !!pulled_task);
		}

		/*
		 * Stop searching for tasks to pull if there are
		 * now runnable tasks on this rq.
		 */
		اگر (pulled_task || !continue_balancing)
			توڑو;
	}

	raw_spin_rq_تالا(this_rq);

	اگر (curr_لاگت > this_rq->max_idle_balance_لاگت)
		this_rq->max_idle_balance_لاگت = curr_لاگت;

	/*
	 * While browsing the domains, we released the rq lock, a task could
	 * have been enqueued in the meantime. Since we're not going idle,
	 * pretend we pulled a task.
	 */
	اگر (this_rq->cfs.h_nr_queued && !pulled_task)
		pulled_task = 1;

	/* If a higher prio class was modified, restart the pick */
	اگر (rq_modified_above(this_rq, &fair_sched_class))
		pulled_task = -1;

برآمد:
	/* Move the next balance forward */
	اگر (وقت_after(this_rq->اگلا_balance, اگلا_balance))
		this_rq->اگلا_balance = اگلا_balance;

	اگر (pulled_task)
		this_rq->idle_stamp = 0;
	ورنہ
		nohz_newidle_balance(this_rq);

	rq_repin_تالا(this_rq, rf);

	واپس pulled_task;
}

/*
 * This softirq handler is triggered via SCHED_SOFTIRQ from two places:
 *
 * - directly from the local sched_tick() for periodic load balancing
 *
 * - indirectly from a remote sched_tick() for NOHZ idle balancing
 *   through the SMP cross-call nohz_csd_func()
 */
ساکن __latent_entropy خالی sched_balance_softirq(خالی)
{
	ساخت rq *this_rq = this_rq();
	شمار cpu_idle_type idle = this_rq->idle_balance;
	/*
	 * If this CPU has a pending NOHZ_BALANCE_KICK, then do the
	 * balancing on behalf of the other idle CPUs whose ticks are
	 * stopped. Do nohz_idle_balance *before* sched_balance_domains to
	 * give the idle CPUs a chance to load balance. Else we may
	 * load balance only within the local sched_domain hierarchy
	 * and abort nohz_idle_balance altogether if we pull some load.
	 */
	اگر (nohz_idle_balance(this_rq, idle))
		واپس;

	/* normal load balance */
	sched_balance_update_blocked_averages(this_rq->cpu);
	sched_balance_domains(this_rq, idle);
}

/*
 * Trigger the SCHED_SOFTIRQ if it is time to do periodic load balancing.
 */
خالی sched_balance_trigger(ساخت rq *rq)
{
	/*
	 * Don't need to rebalance while attached to NULL domain or
	 * runqueue CPU is not active
	 */
	اگر (unlikely(on_null_domain(rq) || !cpu_فعال(cpu_کا(rq))))
		واپس;

	اگر (وقت_after_eq(jiffies, rq->اگلا_balance))
		raise_softirq(SCHED_SOFTIRQ);

	nohz_balancer_kick(rq);
}

ساکن خالی rq_online_fair(ساخت rq *rq)
{
	update_sysctl();

	update_runtime_enabled(rq);
}

ساکن خالی rq_offline_fair(ساخت rq *rq)
{
	update_sysctl();

	/* Ensure any throttled groups are reachable by pick_next_task */
	unthrottle_offline_cfs_rqs(rq);

	/* Ensure that we remove rq contribution to group share: */
	clear_tg_offline_cfs_rqs(rq);
}

#اگر_تعریف_شدہ CONFIG_SCHED_CORE
ساکن اندرونی bool
__entity_slice_used(ساخت sched_entity *se, صحیح min_nr_tasks)
{
	u64 rtime = se->مجموع_exec_runtime - se->پچھلا_مجموع_exec_runtime;
	u64 قاش = se->قاش;

	واپس (rtime * min_nr_tasks > قاش);
}

#تعریف_کرو MIN_NR_TASKS_DURING_FORCEIDLE	2
ساکن اندرونی خالی task_tick_core(ساخت rq *rq, ساخت task_struct *curr)
{
	اگر (!sched_core_enabled(rq))
		واپس;

	/*
	 * If runqueue has only one task which used up its slice and
	 * if the sibling is forced idle, then trigger schedule to
	 * give forced idle task a chance.
	 *
	 * __entity_slice_used() considers only this active rq and it gets the
	 * whole slice. But during force idle, we have siblings acting
	 * like a single runqueue and hence we need to consider runnable
	 * tasks on this CPU and the forced idle CPU. Ideally, we should
	 * go through the forced idle rq, but that would be a perf hit.
	 * We can assume that the forced idle CPU has at least
	 * MIN_NR_TASKS_DURING_FORCEIDLE - 1 tasks and use that to check
	 * if we need to give up the CPU.
	 */
	اگر (rq->core->core_forceidle_گنتی && rq->cfs.h_nr_queued == 1 &&
	    __entity_slice_used(&curr->se, MIN_NR_TASKS_DURING_FORCEIDLE))
		resched_curr(rq);
}

/*
 * Consider any infeasible weight scenario. Take for instance two tasks,
 * each bound to their respective sibling, one with weight 1 and one with
 * weight 2. Then the lower weight task will run ahead of the higher weight
 * task without bound.
 *
 * This utterly destroys the concept of a shared time base.
 *
 * Remember; all this is about a proportionally fair scheduling, where each
 * tasks receives:
 *
 *              w_i
 *   dt_i = ---------- dt                                     (1)
 *          \Sum_j w_j
 *
 * which we do by tracking a virtual time, s_i:
 *
 *          1
 *   s_i = --- d[t]_i                                         (2)
 *         w_i
 *
 * Where d[t] is a delta of discrete time, while dt is an infinitesimal.
 * The immediate corollary is that the ideal schedule S, where (2) to use
 * an infinitesimal delta, is:
 *
 *           1
 *   S = ---------- dt                                        (3)
 *       \Sum_i w_i
 *
 * From which we can define the lag, or deviation from the ideal, as:
 *
 *   lag(i) = S - s_i                                         (4)
 *
 * And since the one and only purpose is to approximate S, we get that:
 *
 *   \Sum_i w_i lag(i) := 0                                   (5)
 *
 * If this were not so, we no longer converge to S, and we can no longer
 * claim our scheduler has any of the properties we derive from S. This is
 * exactly what you did above, you broke it!
 *
 *
 * Let's continue for a while though; to see if there is anything useful to
 * be learned. We can combine (1)-(3) or (4)-(5) and express S in s_i:
 *
 *       \Sum_i w_i s_i
 *   S = --------------                                       (6)
 *         \Sum_i w_i
 *
 * Which gives us a way to compute S, given our s_i. Now, if you've read
 * our code, you know that we do not in fact do this, the reason for this
 * is two-fold. Firstly, computing S in that way requires a 64bit division
 * for every time we'd use it (see 12), and secondly, this only describes
 * the steady-state, it doesn't handle dynamics.
 *
 * Anyway, in (6):  s_i -> x + (s_i - x), to get:
 *
 *           \Sum_i w_i (s_i - x)
 *   S - x = --------------------                             (7)
 *              \Sum_i w_i
 *
 * Which shows that S and s_i transform alike (which makes perfect sense
 * given that S is basically the (weighted) average of s_i).
 *
 * So the thing to remember is that the above is strictly UP. It is
 * possible to generalize to multiple runqueues -- however it gets really
 * yuck when you have to add affinity support, as illustrated by our very
 * first counter-example.
 *
 * Luckily I think we can avoid needing a full multi-queue variant for
 * core-scheduling (or load-balancing). The crucial observation is that we
 * only actually need this comparison in the presence of forced-idle; only
 * then do we need to tell if the stalled rq has higher priority over the
 * other.
 *
 * [XXX assumes SMT2; better consider the more general case, I suspect
 * it'll work out because our comparison is always between 2 rqs and the
 * answer is only interesting if one of them is forced-idle]
 *
 * And (under assumption of SMT2) when there is forced-idle, there is only
 * a single queue, so everything works like normal.
 *
 * Let, for our runqueue 'k':
 *
 *   T_k = \Sum_i w_i s_i
 *   W_k = \Sum_i w_i      ; for all i of k                  (8)
 *
 * Then we can write (6) like:
 *
 *         T_k
 *   S_k = ---                                               (9)
 *         W_k
 *
 * From which immediately follows that:
 *
 *           T_k + T_l
 *   S_k+l = ---------                                       (10)
 *           W_k + W_l
 *
 * On which we can define a combined lag:
 *
 *   lag_k+l(i) := S_k+l - s_i                               (11)
 *
 * And that gives us the tools to compare tasks across a combined runqueue.
 *
 *
 * Combined this gives the following:
 *
 *  a) when a runqueue enters force-idle, sync it against it's sibling rq(s)
 *     using (7); this only requires storing single 'time'-stamps.
 *
 *  b) when comparing tasks between 2 runqueues of which one is forced-idle,
 *     compare the combined lag, per (11).
 *
 * Now, of course cgroups (I so hate them) make this more interesting in
 * that a) seems to suggest we need to iterate all cgroup on a CPU at such
 * boundaries, but I think we can avoid that. The force-idle is for the
 * whole CPU, all it's rqs. So we can mark it in the root and lazily
 * propagate downward on demand.
 */

/*
 * So this sync is basically a relative reset of S to 0.
 *
 * So with 2 queues, when one goes idle, we drop them both to 0 and one
 * then increases due to not being idle, and the idle one builds up lag to
 * get re-elected. So far so simple, right?
 *
 * When there's 3, we can have the situation where 2 run and one is idle,
 * we sync to 0 and let the idle one build up lag to get re-election. Now
 * suppose another one also drops idle. At this point dropping all to 0
 * again would destroy the built-up lag from the queue that was already
 * idle, not good.
 *
 * So instead of syncing everything, we can:
 *
 *   less := !((s64)(s_a - s_b) <= 0)
 *
 *   (v_a - S_a) - (v_b - S_b) == v_a - v_b - S_a + S_b
 *                             == v_a - (v_b - S_a + S_b)
 *
 * IOW, we can recast the (lag) comparison to a one-sided difference.
 * So if then, instead of syncing the whole queue, sync the idle queue
 * against the active queue with S_a + S_b at the point where we sync.
 *
 * (XXX consider the implication of living in a cyclic group: N / 2^n N)
 *
 * This gives us means of syncing single queues against the active queue,
 * and for already idle queues to preserve their build-up lag.
 *
 * Of course, then we get the situation where there's 2 active and one
 * going idle, who do we pick to sync against? Theory would have us sync
 * against the combined S, but as we've already demonstrated, there is no
 * such thing in infeasible weight scenarios.
 *
 * One thing I've considered; and this is where that core_active rudiment
 * came from, is having active queues sync up between themselves after
 * every tick. This limits the observed divergence due to the work
 * conservancy.
 *
 * On top of that, we can improve upon things by employing (10) here.
 */

/*
 * se_fi_update - Update the cfs_rq->zero_vruntime_fi in a CFS hierarchy if needed.
 */
ساکن خالی se_fi_update(ثابت ساخت sched_entity *se, غیرعلامتی صحیح fi_seq,
			 bool forceidle)
{
	for_each_sched_entity(se) {
		ساخت cfs_rq *cfs_rq = cfs_rq_کا(se);

		اگر (forceidle) {
			اگر (cfs_rq->forceidle_seq == fi_seq)
				توڑو;
			cfs_rq->forceidle_seq = fi_seq;
		}

		cfs_rq->zero_vruntime_fi = cfs_rq->zero_vruntime;
	}
}

خالی task_vruntime_update(ساخت rq *rq, ساخت task_struct *p, bool اندر_fi)
{
	ساخت sched_entity *se = &p->se;

	اگر (p->sched_class != &fair_sched_class)
		واپس;

	se_fi_update(se, rq->core->core_forceidle_seq, اندر_fi);
}

bool cfs_prio_less(ثابت ساخت task_struct *a, ثابت ساخت task_struct *b,
			bool اندر_fi)
{
	ساخت rq *rq = task_rq(a);
	ثابت ساخت sched_entity *sea = &a->se;
	ثابت ساخت sched_entity *seb = &b->se;
	ساخت cfs_rq *cfs_rqa;
	ساخت cfs_rq *cfs_rqb;
	s64 delta;

	WARN_ON_ONCE(task_rq(b)->core != rq->core);

	cfs_rqa = &task_rq(a)->cfs;
	cfs_rqb = &task_rq(b)->cfs;

	/*
	 * Find delta after normalizing se's vruntime with its cfs_rq's
	 * zero_vruntime_fi, which would have been updated in prior calls
	 * to se_fi_update().
	 */
	delta = vruntime_op(sea->vruntime, "-", seb->vruntime) +
		vruntime_op(cfs_rqb->zero_vruntime_fi, "-", cfs_rqa->zero_vruntime_fi);

	واپس delta > 0;
}

ساکن صحیح task_is_throttled_fair(ساخت task_struct *p, صحیح cpu)
{
	ساخت cfs_rq *cfs_rq;

#اگر_تعریف_شدہ CONFIG_FAIR_گروہ_SCHED
	cfs_rq = tg_cfs_rq(task_گروہ(p), cpu);
#ورنہ
	cfs_rq = &cpu_rq(cpu)->cfs;
#اگر_ختم
	واپس throttled_hierarchy(cfs_rq);
}
#ورنہ /* !CONFIG_SCHED_CORE: */
ساکن اندرونی خالی task_tick_core(ساخت rq *rq, ساخت task_struct *curr) {}
#اگر_ختم /* !CONFIG_SCHED_CORE */

/*
 * scheduler tick hitting a task of our scheduling class.
 *
 * NOTE: This function can be called remotely by the tick offload that
 * goes along full dynticks. Therefore no local assumption can be made
 * and everything must be accessed through the @rq and @curr passed in
 * parameters.
 */
ساکن خالی task_tick_fair(ساخت rq *rq, ساخت task_struct *curr, صحیح queued)
{
	ساخت sched_entity *se = &curr->se;

	اگر (se->on_rq) {
		غیرعلامتی طویل وزن = NICE_0_بھرو;
		ساخت cfs_rq *cfs_rq;

		for_each_sched_entity(se) {
			cfs_rq = cfs_rq_کا(se);
			entity_tick(cfs_rq, se, queued);

			وزن = __calc_prop_weight(cfs_rq, se, وزن);
		}

		se = &curr->se;
		reweight_eevdf(cfs_rq, se, وزن, se->on_rq);
	}

	اگر (queued)
		واپس;

	اگر (static_branch_unlikely(&sched_numa_balancing))
		task_tick_numa(rq, curr);

	task_tick_cache(rq, curr);

	update_misfit_حالت(curr, rq);
	check_update_overutilized_حالت(task_rq(curr));

	task_tick_core(rq, curr);
}

/*
 * called on fork with the child task as argument from the parent's context
 *  - child not yet on the tasklist
 *  - preemption disabled
 */
ساکن خالی task_fork_fair(ساخت task_struct *p)
{
	سیٹ_task_max_allowed_استعداد(p);
}

/*
 * Priority of the task has changed. Check to see if we preempt
 * the current task.
 */
ساکن خالی
prio_بدلا_fair(ساخت rq *rq, ساخت task_struct *p, u64 oldprio)
{
	اگر (!task_on_rq_queued(p))
		واپس;

	اگر (p->prio == oldprio)
		واپس;

	اگر (rq->cfs.h_nr_queued == 1)
		واپس;

	/*
	 * Reschedule if we are currently running on this runqueue and
	 * our priority decreased, or if we are not currently running on
	 * this runqueue and our priority is higher than the current's
	 */
	اگر (task_current_donor(rq, p)) {
		اگر (p->prio > oldprio)
			resched_curr(rq);
	} ورنہ {
		wakeup_preempt(rq, p, 0);
	}
}

#اگر_تعریف_شدہ CONFIG_FAIR_گروہ_SCHED
/*
 * Propagate the changes of the sched_entity across the tg tree to make it
 * visible to the root
 */
ساکن خالی propagate_entity_cfs_rq(ساخت sched_entity *se)
{
	ساخت cfs_rq *cfs_rq = cfs_rq_کا(se);

	/*
	 * If a task gets attached to this cfs_rq and before being queued,
	 * it gets migrated to another CPU due to reasons like affinity
	 * change, make sure this cfs_rq stays on leaf cfs_rq list to have
	 * that removed load decayed or it can cause faireness problem.
	 */
	اگر (!cfs_rq_pelt_clock_throttled(cfs_rq))
		list_جوڑو_برگ_cfs_rq(cfs_rq);

	/* Start to propagate at parent */
	se = se->والد;

	for_each_sched_entity(se) {
		cfs_rq = cfs_rq_کا(se);

		update_بھرو_avg(cfs_rq, se, UPDATE_TG);

		اگر (!cfs_rq_pelt_clock_throttled(cfs_rq))
			list_جوڑو_برگ_cfs_rq(cfs_rq);
	}

	assert_list_برگ_cfs_rq(rq_کا(cfs_rq));
}
#ورنہ /* !CONFIG_FAIR_GROUP_SCHED: */
ساکن خالی propagate_entity_cfs_rq(ساخت sched_entity *se) { }
#اگر_ختم /* !CONFIG_FAIR_GROUP_SCHED */

ساکن خالی detach_entity_cfs_rq(ساخت sched_entity *se)
{
	ساخت cfs_rq *cfs_rq = cfs_rq_کا(se);

	/*
	 * In case the task sched_avg hasn't been attached:
	 * - A forked task which hasn't been woken up by wake_up_new_task().
	 * - A task which has been woken up by try_to_wake_up() but is
	 *   waiting for actually being woken up by sched_ttwu_pending().
	 */
	اگر (!se->avg.آخری_update_وقت)
		واپس;

	/* Catch up with the cfs_rq and remove our load when we leave */
	update_بھرو_avg(cfs_rq, se, 0);
	detach_entity_بھرو_avg(cfs_rq, se);
	update_tg_بھرو_avg(cfs_rq);
	propagate_entity_cfs_rq(se);
}

ساکن خالی attach_entity_cfs_rq(ساخت sched_entity *se)
{
	ساخت cfs_rq *cfs_rq = cfs_rq_کا(se);

	/* Synchronize entity with its cfs_rq */
	update_بھرو_avg(cfs_rq, se, sched_feat(ATTACH_AGE_بھرو) ? 0 : SKIP_AGE_بھرو);
	attach_entity_بھرو_avg(cfs_rq, se);
	update_tg_بھرو_avg(cfs_rq);
	propagate_entity_cfs_rq(se);
}

ساکن خالی detach_task_cfs_rq(ساخت task_struct *p)
{
	ساخت sched_entity *se = &p->se;

	detach_entity_cfs_rq(se);
}

ساکن خالی attach_task_cfs_rq(ساخت task_struct *p)
{
	ساخت sched_entity *se = &p->se;

	attach_entity_cfs_rq(se);
}

ساکن خالی switching_from_fair(ساخت rq *rq, ساخت task_struct *p)
{
	اگر (p->se.sched_delayed)
		قطار_سے_نکالو_task(rq, p, قطار_سے_نکالو_SLEEP | قطار_سے_نکالو_DELAYED | قطار_سے_نکالو_NOCLOCK);
}

ساکن خالی switched_from_fair(ساخت rq *rq, ساخت task_struct *p)
{
	detach_task_cfs_rq(p);
}

ساکن خالی switched_مقصد_fair(ساخت rq *rq, ساخت task_struct *p)
{
	WARN_ON_ONCE(p->se.sched_delayed);

	attach_task_cfs_rq(p);

	سیٹ_task_max_allowed_استعداد(p);

	اگر (task_on_rq_queued(p)) {
		/*
		 * We were most likely switched from sched_rt, so
		 * kick off the schedule if running, otherwise just see
		 * if we can still preempt the current task.
		 */
		اگر (task_current_donor(rq, p))
			resched_curr(rq);
		ورنہ
			wakeup_preempt(rq, p, 0);
	}
}

ساکن خالی سیٹ_اگلا_task_fair(ساخت rq *rq, ساخت task_struct *p, شمار snt_e type)
{
	ساخت sched_entity *se = &p->se;
	ساخت cfs_rq *cfs_rq = &rq->cfs;
	غیرعلامتی طویل وزن = NICE_0_بھرو;
	bool پہلا = type == SNT_PICK;
	bool throttled = false;
	bool on_rq = se->on_rq;

	اگر (type == SNT_REPICK)
		جاؤ repick;

	clear_buddies(cfs_rq, se);

	اگر (on_rq)
		__dequeue_entity(cfs_rq, se);

	for_each_sched_entity(se) {
		cfs_rq = cfs_rq_کا(se);

		اگر (!IS_ENABLED(CONFIG_FAIR_گروہ_SCHED) ||
		    !پہلا || !cfs_rq->h_curr)
			سیٹ_اگلا_entity(cfs_rq, se);

		/* ensure bandwidth has been allocated on our new cfs_rq */
		throttled |= account_cfs_rq_runtime(cfs_rq, 0);

		اگر (on_rq)
			وزن = __calc_prop_weight(cfs_rq, se, وزن);
	}

	اگر (throttled)
		task_throttle_تیاری_work(p);

	se = &p->se;
	cfs_rq->curr = se;

	اگر (on_rq) {
		reweight_eevdf(cfs_rq, se, وزن, se->on_rq);
		اگر (پہلا)
			سیٹ_protect_قاش(cfs_rq, se);
	}

	اگر (task_on_rq_queued(p)) {
		/*
		 * Move the next running task to the front of the list, so our
		 * cfs_tasks list becomes MRU one.
		 */
		list_move(&se->گروہ_گرہ, &rq->cfs_tasks);
	}
	اگر (!پہلا)
		واپس;

	WARN_ON_ONCE(se->sched_delayed);

	update_misfit_حالت(p, rq);
	sched_fair_update_stop_tick(rq, p);

repick:
	/*
	 * A same-task repick skips put_prev_task_fair(), but
	 * pick_task_fair() refreshed the entity hrtick_start_fair() reads
	 * before selecting it again. rq->cfs.curr identifies that entity,
	 * including with group scheduling.
	 */
	اگر (hrtick_enabled_fair(rq))
		hrtick_آغاز_fair(rq, p);
}

خالی init_cfs_rq(ساخت cfs_rq *cfs_rq)
{
	cfs_rq->tasks_timeline = RB_جڑ_CACHED;
	cfs_rq->zero_vruntime = (u64)(-(1LL << 20));
	raw_spin_تالا_init(&cfs_rq->removed.تالا);
}

#اگر_تعریف_شدہ CONFIG_FAIR_گروہ_SCHED
ساکن خالی task_change_گروہ_fair(ساخت task_struct *p)
{
	/*
	 * We couldn't detach or attach a forked task which
	 * hasn't been woken up by wake_up_new_task().
	 */
	اگر (READ_ONCE(p->__state) == TASK_NEW)
		واپس;

	detach_task_cfs_rq(p);

	/* Tell se's cfs_rq has been changed -- migrated */
	p->se.avg.آخری_update_وقت = 0;
	سیٹ_task_rq(p, task_cpu(p));
	attach_task_cfs_rq(p);
}

خالی free_fair_sched_گروہ(ساخت task_گروہ *tg)
{
	free_percpu(tg->cfs_rq);
}

صحیح alloc_fair_sched_گروہ(ساخت task_گروہ *tg, ساخت task_گروہ *والد)
{
	ساخت cfs_tg_state __percpu *state;
	ساخت sched_entity *se;
	ساخت cfs_rq *cfs_rq;
	صحیح i;

	state = alloc_percpu_gfp(ساخت cfs_tg_state, GFP_KERNEL);
	اگر (!state)
		جاؤ err;

	tg->cfs_rq = &state->cfs_rq;
	tg->shares = NICE_0_بھرو;

	init_cfs_bandwidth(tg_cfs_bandwidth(tg), tg_cfs_bandwidth(والد));

	for_each_possible_cpu(i) {
		cfs_rq = tg_cfs_rq(tg, i);
		اگر (!cfs_rq)
			جاؤ err;

		se = tg_se(tg, i);
		init_cfs_rq(cfs_rq);
		init_tg_cfs_اندراج(tg, cfs_rq, se, i, tg_se(والد, i));
		init_entity_runnable_اوسط_قدر(se);
	}

	واپس 1;

err:
	واپس 0;
}

خالی online_fair_sched_گروہ(ساخت task_گروہ *tg)
{
	ساخت sched_entity *se;
	ساخت rq_flags rf;
	ساخت rq *rq;
	صحیح i;

	for_each_possible_cpu(i) {
		rq = cpu_rq(i);
		se = tg_se(tg, i);
		rq_تالا_irq(rq, &rf);
		update_rq_clock(rq);
		attach_entity_cfs_rq(se);
		sync_throttle(tg, i);
		rq_unlock_irq(rq, &rf);
	}
}

خالی unregister_fair_sched_گروہ(ساخت task_گروہ *tg)
{
	صحیح cpu;

	destroy_cfs_bandwidth(tg_cfs_bandwidth(tg));

	for_each_possible_cpu(cpu) {
		ساخت cfs_rq *cfs_rq = tg_cfs_rq(tg, cpu);
		ساخت sched_entity *se = tg_se(tg, cpu);
		ساخت rq *rq = cpu_rq(cpu);

		اگر (se)
			remove_entity_بھرو_avg(se);

		/*
		 * Only empty task groups can be destroyed; so we can speculatively
		 * check on_list without danger of it being re-added.
		 */
		اگر (cfs_rq->on_list) {
			guard(rq_تالا_irqsave)(rq);
			list_del_برگ_cfs_rq(cfs_rq);
		}
	}
}

خالی init_tg_cfs_اندراج(ساخت task_گروہ *tg, ساخت cfs_rq *cfs_rq,
			ساخت sched_entity *se, صحیح cpu,
			ساخت sched_entity *والد)
{
	ساخت rq *rq = cpu_rq(cpu);

	cfs_rq->tg = tg;
	cfs_rq->rq = rq;
	init_cfs_rq_runtime(cfs_rq);

	/* se could be NULL for root_task_group */
	اگر (!se)
		واپس;

	اگر (!والد) {
		se->cfs_rq = &rq->cfs;
		se->گہرائی = 0;
	} ورنہ {
		se->cfs_rq = والد->my_q;
		se->گہرائی = والد->گہرائی + 1;
	}

	se->my_q = cfs_rq;
	/* guarantee group entities always have weight */
	update_بھرو_سیٹ(&se->بھرو, NICE_0_بھرو);
	se->والد = والد;
}

ساکن DEFINE_MUTEX(shares_mutex);

ساکن صحیح __sched_group_set_shares(ساخت task_گروہ *tg, غیرعلامتی طویل shares)
{
	صحیح i;

	lockdep_assert_held(&shares_mutex);

	/*
	 * We can't change the weight of the root cgroup.
	 */
	اگر (is_جڑ_task_گروہ(tg))
		واپس -EINVAL;

	shares = clamp(shares, scale_بھرو(MIN_SHARES), scale_بھرو(MAX_SHARES));

	اگر (tg->shares == shares)
		واپس 0;

	tg->shares = shares;
	for_each_possible_cpu(i) {
		ساخت rq *rq = cpu_rq(i);
		ساخت sched_entity *se = tg_se(tg, i);
		ساخت rq_flags rf;

		/* Propagate contribution to hierarchy */
		rq_تالا_irqsave(rq, &rf);
		update_rq_clock(rq);
		for_each_sched_entity(se) {
			update_بھرو_avg(cfs_rq_کا(se), se, UPDATE_TG);
			update_cfs_گروہ(se);
		}
		rq_unlock_irqrestore(rq, &rf);
	}

	واپس 0;
}

صحیح sched_گروہ_سیٹ_shares(ساخت task_گروہ *tg, غیرعلامتی طویل shares)
{
	صحیح ret;

	mutex_تالا(&shares_mutex);
	اگر (tg_is_idle(tg))
		ret = -EINVAL;
	ورنہ
		ret = __sched_group_set_shares(tg, shares);
	mutex_unlock(&shares_mutex);

	واپس ret;
}

صحیح sched_گروہ_سیٹ_idle(ساخت task_گروہ *tg, طویل idle)
{
	صحیح i;

	اگر (tg == &جڑ_task_گروہ)
		واپس -EINVAL;

	اگر (idle < 0 || idle > 1)
		واپس -EINVAL;

	mutex_تالا(&shares_mutex);

	اگر (tg->idle == idle) {
		mutex_unlock(&shares_mutex);
		واپس 0;
	}

	tg->idle = idle;

	for_each_possible_cpu(i) {
		ساخت rq *rq = cpu_rq(i);
		ساخت sched_entity *se = tg_se(tg, i);
		ساخت cfs_rq *grp_cfs_rq = tg_cfs_rq(tg, i);
		bool was_idle = cfs_rq_is_idle(grp_cfs_rq);
		طویل idle_task_delta;
		ساخت rq_flags rf;

		rq_تالا_irqsave(rq, &rf);

		grp_cfs_rq->idle = idle;
		اگر (WARN_ON_ONCE(was_idle == cfs_rq_is_idle(grp_cfs_rq)))
			جاؤ اگلا_cpu;

		idle_task_delta = grp_cfs_rq->h_nr_queued -
				  grp_cfs_rq->h_nr_idle;
		اگر (!cfs_rq_is_idle(grp_cfs_rq))
			idle_task_delta *= -1;

		for_each_sched_entity(se) {
			ساخت cfs_rq *cfs_rq = cfs_rq_کا(se);

			اگر (!se->on_rq)
				توڑو;

			cfs_rq->h_nr_idle += idle_task_delta;

			/* Already accounted at parent level and above. */
			اگر (cfs_rq_is_idle(cfs_rq))
				توڑو;
		}

اگلا_cpu:
		rq_unlock_irqrestore(rq, &rf);
	}

	/* Idle groups have minimum weight. */
	اگر (tg_is_idle(tg))
		__sched_group_set_shares(tg, scale_بھرو(وزن_IDLEPRIO));
	ورنہ
		__sched_group_set_shares(tg, NICE_0_بھرو);

	mutex_unlock(&shares_mutex);
	واپس 0;
}

#اگر_ختم /* CONFIG_FAIR_GROUP_SCHED */


ساکن غیرعلامتی صحیح get_rr_interval_fair(ساخت rq *rq, ساخت task_struct *task)
{
	ساخت sched_entity *se = &task->se;
	غیرعلامتی صحیح rr_interval = 0;

	/*
	 * Time slice is 0 for SCHED_OTHER tasks that are on an otherwise
	 * idle runqueue:
	 */
	اگر (rq->cfs.بھرو.وزن)
		rr_interval = NS_مقصد_JIFFIES(se->قاش);

	واپس rr_interval;
}

/*
 * All the scheduling class methods:
 */
DEFINE_SCHED_CLASS(fair) = {
	.قطار_میں_ڈالو_task		= قطار_میں_ڈالو_task_fair,
	.قطار_سے_نکالو_task		= قطار_سے_نکالو_task_fair,
	.yield_task		= yield_task_fair,
	.yield_مقصد_task		= yield_مقصد_task_fair,

	.wakeup_preempt		= wakeup_preempt_fair,

	.pick_task		= pick_task_fair,
	.put_پچھلا_task		= put_پچھلا_task_fair,
	.سیٹ_اگلا_task          = سیٹ_اگلا_task_fair,

	.select_task_rq		= select_task_rq_fair,
	.migrate_task_rq	= migrate_task_rq_fair,

	.rq_online		= rq_online_fair,
	.rq_offline		= rq_offline_fair,

	.task_dead		= task_dead_fair,
	.سیٹ_cpus_allowed	= سیٹ_cpus_allowed_fair,

	.task_tick		= task_tick_fair,
	.task_fork		= task_fork_fair,

	.reweight_task		= reweight_task_fair,
	.prio_بدلا		= prio_بدلا_fair,
	.switching_from		= switching_from_fair,
	.switched_from		= switched_from_fair,
	.switched_مقصد		= switched_مقصد_fair,

	.get_rr_interval	= get_rr_interval_fair,

	.update_curr		= update_curr_fair,

#اگر_تعریف_شدہ CONFIG_FAIR_گروہ_SCHED
	.task_change_گروہ	= task_change_گروہ_fair,
#اگر_ختم

#اگر_تعریف_شدہ CONFIG_SCHED_CORE
	.task_is_throttled	= task_is_throttled_fair,
#اگر_ختم

#اگر_تعریف_شدہ CONFIG_UCLAMP_TASK
	.uclamp_enabled		= 1,
#اگر_ختم
};

خالی print_cfs_stats(ساخت seq_file *m, صحیح cpu)
{
	ساخت cfs_rq *cfs_rq, *pos;

	rcu_read_تالا();
	for_each_برگ_cfs_rq_محفوظ(cpu_rq(cpu), cfs_rq, pos)
		print_cfs_rq(m, cpu, cfs_rq);
	rcu_read_unlock();
}

#اگر_تعریف_شدہ CONFIG_NUMA_BALANCING
خالی show_numa_stats(ساخت task_struct *p, ساخت seq_file *m)
{
	صحیح گرہ;
	غیرعلامتی طویل tsf = 0, tpf = 0, gsf = 0, gpf = 0;
	ساخت numa_گروہ *ng;

	rcu_read_تالا();
	ng = rcu_dereference_all(p->numa_گروہ);
	for_each_online_گرہ(گرہ) {
		اگر (p->numa_خرابیاں) {
			tsf = p->numa_خرابیاں[task_خرابیاں_idx(NUMA_MEM, گرہ, 0)];
			tpf = p->numa_خرابیاں[task_خرابیاں_idx(NUMA_MEM, گرہ, 1)];
		}
		اگر (ng) {
			gsf = ng->خرابیاں[task_خرابیاں_idx(NUMA_MEM, گرہ, 0)];
			gpf = ng->خرابیاں[task_خرابیاں_idx(NUMA_MEM, گرہ, 1)];
		}
		print_numa_stats(m, گرہ, tsf, tpf, gsf, gpf);
	}
	rcu_read_unlock();
}
#اگر_ختم /* CONFIG_NUMA_BALANCING */

__init خالی init_sched_fair_class(خالی)
{
	صحیح i;

	for_each_possible_cpu(i) {
		zalloc_cpumask_var_گرہ(&فی_cpu(بھرو_balance_نقاب, i), GFP_KERNEL, cpu_مقصد_گرہ(i));
		zalloc_cpumask_var_گرہ(&فی_cpu(select_rq_نقاب,    i), GFP_KERNEL, cpu_مقصد_گرہ(i));
		zalloc_cpumask_var_گرہ(&فی_cpu(should_we_balance_tmpmask, i),
					GFP_KERNEL, cpu_مقصد_گرہ(i));

#اگر_تعریف_شدہ CONFIG_CFS_BANDWIDTH
		INIT_CSD(&cpu_rq(i)->cfsb_csd, __cfsb_csd_unthrottle, cpu_rq(i));
		INIT_LIST_سر(&cpu_rq(i)->cfsb_csd_list);
#اگر_ختم
	}

	open_softirq(SCHED_SOFTIRQ, sched_balance_softirq);

#اگر_تعریف_شدہ CONFIG_NO_HZ_COMMON
	nohz.اگلا_balance = jiffies;
	nohz.اگلا_blocked = jiffies;
	zalloc_cpumask_var(&nohz.idle_cpus_نقاب, GFP_NOWAIT);
#اگر_ختم
}
