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
#అన్తర్భావ <linux/energy_model.h>
#అన్తర్భావ <linux/mmap_lock.h>
#అన్తర్భావ <linux/hugetlb_inline.h>
#అన్తర్భావ <linux/jiffies.h>
#అన్తర్భావ <linux/math.h>
#అన్తర్భావ <linux/mm_api.h>
#అన్తర్భావ <linux/highmem.h>
#అన్తర్భావ <linux/hrtimer.h>
#అన్తర్భావ <linux/hrtimer_bases.h>
#అన్తర్భావ <linux/spinlock_api.h>
#అన్తర్భావ <linux/cpumask_api.h>
#అన్తర్భావ <linux/lockdep_api.h>
#అన్తర్భావ <linux/softirq.h>
#అన్తర్భావ <linux/refcount_api.h>
#అన్తర్భావ <linux/topology.h>
#అన్తర్భావ <linux/sched/clock.h>
#అన్తర్భావ <linux/sched/cond_resched.h>
#అన్తర్భావ <linux/sched/cputime.h>
#అన్తర్భావ <linux/sched/isolation.h>
#అన్తర్భావ <linux/sched/nohz.h>
#అన్తర్భావ <linux/sched/prio.h>
#అన్తర్భావ <linux/static_call.h>

#అన్తర్భావ <linux/cpuidle.h>
#అన్తర్భావ <linux/interrupt.h>
#అన్తర్భావ <linux/memory-tiers.h>
#అన్తర్భావ <linux/mempolicy.h>
#అన్తర్భావ <linux/mutex_api.h>
#అన్తర్భావ <linux/profile.h>
#అన్తర్భావ <linux/psi.h>
#అన్తర్భావ <linux/ratelimit.h>
#అన్తర్భావ <linux/task_work.h>
#అన్తర్భావ <linux/rbtree_augmented.h>

#అన్తర్భావ <asm/switch_to.h>

#అన్తర్భావ <uapi/linux/sched/types.h>

#అన్తర్భావ "sched.h"
#అన్తర్భావ "stats.h"
#అన్తర్భావ "autogroup.h"

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
గుర్తులేని పూర్ణాంకం sysctl_sched_tunable_scaling = SCHED_TUNABLESCALING_LOG;

/*
 * Default base time slice (request size r_i) for SCHED_NORMAL/SCHED_BATCH:
 *
 * Under EEVDF this is the request size used to compute the virtual
 * deadline; see update_deadline().
 *
 * (default: 0.70 msec * (1 + ilog(ncpus)), units: nanoseconds)
 */
గుర్తులేని పూర్ణాంకం sysctl_sched_పునాది_ముక్క			= 700000ULL;
స్థిర గుర్తులేని పూర్ణాంకం normalized_sysctl_sched_పునాది_ముక్క	= 700000ULL;

__read_mostly గుర్తులేని పూర్ణాంకం sysctl_sched_migration_ఖర్చు	= 500000UL;

స్థిర పూర్ణాంకం __init అమరిక_sched_thermal_decay_జరుపు(అక్షరం *str)
{
	pr_warn("Ignoring the deprecated sched_thermal_decay_shift= option\n");
	తిరిగి 1;
}
__setup("sched_thermal_decay_shift=", అమరిక_sched_thermal_decay_జరుపు);

/*
 * For asym packing, by default the lower numbered CPU has higher priority.
 */
పూర్ణాంకం __weak arch_asym_cpu_ప్రాధాన్యత(పూర్ణాంకం cpu)
{
	తిరిగి -cpu;
}

/*
 * The margin used when comparing utilization with CPU capacity.
 *
 * (default: ~20%)
 */
#నిర్వచించు సరిపోతుంది_సామర్థ్యం(cap, max)	((cap) * 1280 < (max) * 1024)

/*
 * The margin used when comparing CPU capacities.
 * is 'cap1' noticeably greater than 'cap2'
 *
 * (default: ~5%)
 */
#నిర్వచించు సామర్థ్యం_greater(cap1, cap2) ((cap1) * 1024 > (cap2) * 1078)

#నిర్వచితమైతే CONFIG_CFS_BANDWIDTH
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
స్థిర గుర్తులేని పూర్ణాంకం sysctl_sched_cfs_bandwidth_ముక్క		= 5000UL;
#అయితే_ముగింపు

#నిర్వచితమైతే CONFIG_NUMA_BALANCING
/* Restrict the NUMA promotion throughput (MB/s) for each target node. */
స్థిర గుర్తులేని పూర్ణాంకం sysctl_numa_balancing_promote_రేటు_హద్దు = 65536;
#అయితే_ముగింపు

#నిర్వచితమైతే CONFIG_SYSCTL
స్థిర స్థిరరాశి నిర్మాణం ctl_పట్టిక sched_fair_sysctls[] = {
#నిర్వచితమైతే CONFIG_CFS_BANDWIDTH
	{
		.procname       = "sched_cfs_bandwidth_slice_us",
		.దత్తాంశం           = &sysctl_sched_cfs_bandwidth_ముక్క,
		.maxlen         = పరిమాణం(గుర్తులేని పూర్ణాంకం),
		.బహుళకం           = 0644,
		.proc_handler   = proc_dointvec_minmax,
		.extra1         = SYSCTL_ONE,
	},
#అయితే_ముగింపు
#నిర్వచితమైతే CONFIG_NUMA_BALANCING
	{
		.procname	= "numa_balancing_promote_rate_limit_MBps",
		.దత్తాంశం		= &sysctl_numa_balancing_promote_రేటు_హద్దు,
		.maxlen		= పరిమాణం(గుర్తులేని పూర్ణాంకం),
		.బహుళకం		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= SYSCTL_ZERO,
	},
#అయితే_ముగింపు /* CONFIG_NUMA_BALANCING */
};

స్థిర పూర్ణాంకం __init sched_fair_sysctl_init(శూన్య)
{
	రిజిస్టరు_sysctl_init("kernel", sched_fair_sysctls);
	తిరిగి 0;
}
late_initcall(sched_fair_sysctl_init);
#అయితే_ముగింపు /* CONFIG_SYSCTL */

స్థిర అంతఃవరుస శూన్య update_ఎక్కించు_కూడు(నిర్మాణం ఎక్కించు_బరువు *lw, గుర్తులేని దీర్ఘ inc)
{
	lw->బరువు += inc;
	lw->inv_బరువు = 0;
}

స్థిర అంతఃవరుస శూన్య update_ఎక్కించు_sub(నిర్మాణం ఎక్కించు_బరువు *lw, గుర్తులేని దీర్ఘ dec)
{
	lw->బరువు -= dec;
	lw->inv_బరువు = 0;
}

స్థిర అంతఃవరుస శూన్య update_ఎక్కించు_సమితి(నిర్మాణం ఎక్కించు_బరువు *lw, గుర్తులేని దీర్ఘ w)
{
	lw->బరువు = w;
	lw->inv_బరువు = 0;
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
స్థిర గుర్తులేని పూర్ణాంకం get_update_sysctl_గుణకం(శూన్య)
{
	గుర్తులేని పూర్ణాంకం cpus = min_t(గుర్తులేని పూర్ణాంకం, అంకె_online_cpus(), 8);
	గుర్తులేని పూర్ణాంకం గుణకం;

	ఎంపిక (sysctl_sched_tunable_scaling) {
	స్థితి SCHED_TUNABLESCALING_NONE:
		గుణకం = 1;
		ఆపు;
	స్థితి SCHED_TUNABLESCALING_రేఖీయ:
		గుణకం = cpus;
		ఆపు;
	స్థితి SCHED_TUNABLESCALING_LOG:
	అప్రమేయ:
		గుణకం = 1 + ilog2(cpus);
		ఆపు;
	}

	తిరిగి గుణకం;
}

స్థిర శూన్య update_sysctl(శూన్య)
{
	గుర్తులేని పూర్ణాంకం గుణకం = get_update_sysctl_గుణకం();

#నిర్వచించు సమితి_SYSCTL(name) \
	(sysctl_##name = (గుణకం) * normalized_sysctl_##name)
	సమితి_SYSCTL(sched_పునాది_ముక్క);
#నిర్వచనం_రద్దు సమితి_SYSCTL
}

శూన్య __init sched_init_granularity(శూన్య)
{
	update_sysctl();
}

#నిర్వచితం_కాకపోతే CONFIG_64_బిట్టు
#నిర్వచించు WMULT_CONST	(~0U)
#నిర్వచించు WMULT_జరుపు	32

స్థిర శూన్య __update_inv_weight(నిర్మాణం ఎక్కించు_బరువు *lw)
{
	గుర్తులేని దీర్ఘ w;

	ఒకవేళ (likely(lw->inv_బరువు))
		తిరిగి;

	w = scale_ఎక్కించు_కిందికి(lw->బరువు);

	ఒకవేళ (బిట్లు_చొప్పున_LONG > 32 && unlikely(w >= WMULT_CONST))
		lw->inv_బరువు = 1;
	లేకపోతే ఒకవేళ (unlikely(!w))
		lw->inv_బరువు = WMULT_CONST;
	లేకపోతే
		lw->inv_బరువు = WMULT_CONST / w;
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
స్థిర u64 __calc_delta(u64 delta_exec, గుర్తులేని దీర్ఘ బరువు, నిర్మాణం ఎక్కించు_బరువు *lw)
{
	u64 fact = scale_ఎక్కించు_కిందికి(బరువు);
	u32 fact_hi = (u32)(fact >> 32);
	పూర్ణాంకం జరుపు = WMULT_జరుపు;
	పూర్ణాంకం fs;

	__update_inv_weight(lw);

	ఒకవేళ (unlikely(fact_hi)) {
		fs = fls(fact_hi);
		జరుపు -= fs;
		fact >>= fs;
	}

	fact = mul_u32_u32(fact, lw->inv_బరువు);

	fact_hi = (u32)(fact >> 32);
	ఒకవేళ (fact_hi) {
		fs = fls(fact_hi);
		జరుపు -= fs;
		fact >>= fs;
	}

	తిరిగి mul_u64_u32_shr(delta_exec, fact, జరుపు);
}
#లేకపోతే
స్థిర u64 __calc_delta(u64 delta_exec, గుర్తులేని దీర్ఘ బరువు, నిర్మాణం ఎక్కించు_బరువు *lw)
{
	తిరిగి (delta_exec * బరువు) / lw->బరువు;
}
#అయితే_ముగింపు

/*
 * delta /= w
 */
స్థిర అంతఃవరుస u64 calc_delta_fair(u64 delta, నిర్మాణం sched_entity *se)
{
	ఒకవేళ (se->h_ఎక్కించు.బరువు != NICE_0_ఎక్కించు)
		delta = __calc_delta(delta, NICE_0_ఎక్కించు, &se->h_ఎక్కించు);

	తిరిగి delta;
}

స్థిరరాశి నిర్మాణం sched_class fair_sched_class;

/**************************************************************
 * CFS operations on generic schedulable entities:
 */

#నిర్వచితమైతే CONFIG_FAIR_సమూహం_SCHED

/* Walk up scheduling entities hierarchy */
#నిర్వచించు for_each_sched_entity(se) \
		కొరకు (; se; se = se->మాతృ)

స్థిర అంతఃవరుస bool list_కూడు_ఆకు_cfs_rq(నిర్మాణం cfs_rq *cfs_rq)
{
	నిర్మాణం rq *rq = rq_యొక్క(cfs_rq);
	పూర్ణాంకం cpu = cpu_యొక్క(rq);

	ఒకవేళ (cfs_rq->on_list)
		తిరిగి rq->tmp_alone_branch == &rq->ఆకు_cfs_rq_list;

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
	ఒకవేళ (cfs_rq->tg->మాతృ &&
	    tg_cfs_rq(cfs_rq->tg->మాతృ, cpu)->on_list) {
		/*
		 * If parent is already on the list, we add the child
		 * just before. Thanks to circular linked property of
		 * the list, this means to put the child at the tail
		 * of the list that starts by parent.
		 */
		list_కూడు_తోక_rcu(&cfs_rq->ఆకు_cfs_rq_list,
			&(tg_cfs_rq(cfs_rq->tg->మాతృ, cpu)->ఆకు_cfs_rq_list));
		/*
		 * The branch is now connected to its tree so we can
		 * reset tmp_alone_branch to the beginning of the
		 * list.
		 */
		rq->tmp_alone_branch = &rq->ఆకు_cfs_rq_list;
		తిరిగి true;
	}

	ఒకవేళ (!cfs_rq->tg->మాతృ) {
		/*
		 * cfs rq without parent should be put
		 * at the tail of the list.
		 */
		list_కూడు_తోక_rcu(&cfs_rq->ఆకు_cfs_rq_list,
			&rq->ఆకు_cfs_rq_list);
		/*
		 * We have reach the top of a tree so we can reset
		 * tmp_alone_branch to the beginning of the list.
		 */
		rq->tmp_alone_branch = &rq->ఆకు_cfs_rq_list;
		తిరిగి true;
	}

	/*
	 * The parent has not already been added so we want to
	 * make sure that it will be put after us.
	 * tmp_alone_branch points to the begin of the branch
	 * where we will add parent.
	 */
	list_కూడు_rcu(&cfs_rq->ఆకు_cfs_rq_list, rq->tmp_alone_branch);
	/*
	 * update tmp_alone_branch to points to the new begin
	 * of the branch
	 */
	rq->tmp_alone_branch = &cfs_rq->ఆకు_cfs_rq_list;
	తిరిగి false;
}

స్థిర అంతఃవరుస శూన్య list_del_ఆకు_cfs_rq(నిర్మాణం cfs_rq *cfs_rq)
{
	ఒకవేళ (cfs_rq->on_list) {
		నిర్మాణం rq *rq = rq_యొక్క(cfs_rq);

		/*
		 * With cfs_rq being unthrottled/throttled during an enqueue,
		 * it can happen the tmp_alone_branch points to the leaf that
		 * we finally want to delete. In this case, tmp_alone_branch moves
		 * to the prev element but it will point to rq->leaf_cfs_rq_list
		 * at the end of the enqueue.
		 */
		ఒకవేళ (rq->tmp_alone_branch == &cfs_rq->ఆకు_cfs_rq_list)
			rq->tmp_alone_branch = cfs_rq->ఆకు_cfs_rq_list.prev;

		list_del_rcu(&cfs_rq->ఆకు_cfs_rq_list);
		cfs_rq->on_list = 0;
	}
}

స్థిర అంతఃవరుస శూన్య assert_list_ఆకు_cfs_rq(నిర్మాణం rq *rq)
{
	WARN_ON_ONCE(rq->tmp_alone_branch != &rq->ఆకు_cfs_rq_list);
}

/* Iterate through all leaf cfs_rq's on a runqueue */
#నిర్వచించు for_each_ఆకు_cfs_rq_సురక్షిత(rq, cfs_rq, pos)			\
	list_for_each_నమోదు_సురక్షిత(cfs_rq, pos, &rq->ఆకు_cfs_rq_list,	\
				 ఆకు_cfs_rq_list)

/* Do the two (enqueued) entities belong to the same group ? */
స్థిర అంతఃవరుస నిర్మాణం cfs_rq *
is_same_సమూహం(నిర్మాణం sched_entity *se, నిర్మాణం sched_entity *pse)
{
	ఒకవేళ (se->cfs_rq == pse->cfs_rq)
		తిరిగి se->cfs_rq;

	తిరిగి ఖాళీ;
}

స్థిర అంతఃవరుస నిర్మాణం sched_entity *మాతృ_entity(స్థిరరాశి నిర్మాణం sched_entity *se)
{
	తిరిగి se->మాతృ;
}

స్థిర పూర్ణాంకం tg_is_idle(నిర్మాణం task_సమూహం *tg)
{
	తిరిగి tg->idle > 0;
}

స్థిర పూర్ణాంకం cfs_rq_is_idle(నిర్మాణం cfs_rq *cfs_rq)
{
	తిరిగి cfs_rq->idle > 0;
}

స్థిర పూర్ణాంకం se_is_idle(నిర్మాణం sched_entity *se)
{
	ఒకవేళ (entity_is_task(se))
		తిరిగి task_has_idle_policy(task_యొక్క(se));
	తిరిగి cfs_rq_is_idle(సమూహం_cfs_rq(se));
}

#లేకపోతే /* !CONFIG_FAIR_GROUP_SCHED: */

#నిర్వచించు for_each_sched_entity(se) \
		కొరకు (; se; se = ఖాళీ)

స్థిర అంతఃవరుస bool list_కూడు_ఆకు_cfs_rq(నిర్మాణం cfs_rq *cfs_rq)
{
	తిరిగి true;
}

స్థిర అంతఃవరుస శూన్య list_del_ఆకు_cfs_rq(నిర్మాణం cfs_rq *cfs_rq)
{
}

స్థిర అంతఃవరుస శూన్య assert_list_ఆకు_cfs_rq(నిర్మాణం rq *rq)
{
}

#నిర్వచించు for_each_ఆకు_cfs_rq_సురక్షిత(rq, cfs_rq, pos)	\
		కొరకు (cfs_rq = &rq->cfs, pos = ఖాళీ; cfs_rq; cfs_rq = pos)

స్థిర అంతఃవరుస నిర్మాణం sched_entity *మాతృ_entity(నిర్మాణం sched_entity *se)
{
	తిరిగి ఖాళీ;
}

స్థిర అంతఃవరుస పూర్ణాంకం tg_is_idle(నిర్మాణం task_సమూహం *tg)
{
	తిరిగి 0;
}

స్థిర పూర్ణాంకం cfs_rq_is_idle(నిర్మాణం cfs_rq *cfs_rq)
{
	తిరిగి 0;
}

స్థిర పూర్ణాంకం se_is_idle(నిర్మాణం sched_entity *se)
{
	తిరిగి task_has_idle_policy(task_యొక్క(se));
}

#అయితే_ముగింపు /* !CONFIG_FAIR_GROUP_SCHED */

స్థిర __always_inline
bool account_cfs_rq_runtime(నిర్మాణం cfs_rq *cfs_rq, u64 delta_exec);

/**************************************************************
 * Scheduling class tree data structure manipulation methods:
 */

బాహ్య శూన్య __BUILD_BUG_vruntime_cmp(శూన్య);

/* Use __builtin_strcmp() because of __HAVE_ARCH_STRCMP: */

#నిర్వచించు vruntime_cmp(A, CMP_STR, B) ({				\
	పూర్ణాంకం __res = 0;						\
								\
	ఒకవేళ (!__builtin_strcmp(CMP_STR, "<")) {			\
		__res = ((s64)((A)-(B)) < 0);			\
	} లేకపోతే ఒకవేళ (!__builtin_strcmp(CMP_STR, "<=")) {		\
		__res = ((s64)((A)-(B)) <= 0);			\
	} లేకపోతే ఒకవేళ (!__builtin_strcmp(CMP_STR, ">")) {		\
		__res = ((s64)((A)-(B)) > 0);			\
	} లేకపోతే ఒకవేళ (!__builtin_strcmp(CMP_STR, ">=")) {		\
		__res = ((s64)((A)-(B)) >= 0);			\
	} లేకపోతే {						\
		/* Unknown operator throws linker error: */	\
		__BUILD_BUG_vruntime_cmp();			\
	}							\
								\
	__res;							\
})

బాహ్య శూన్య __BUILD_BUG_vruntime_op(శూన్య);

#నిర్వచించు vruntime_op(A, OP_STR, B) ({				\
	s64 __res = 0;						\
								\
	ఒకవేళ (!__builtin_strcmp(OP_STR, "-")) {			\
		__res = (s64)((A)-(B));				\
	} లేకపోతే {						\
		/* Unknown operator throws linker error: */	\
		__BUILD_BUG_vruntime_op();			\
	}							\
								\
	__res;						\
})


స్థిర అంతఃవరుస __maybe_unused u64 max_vruntime(u64 max_vruntime, u64 vruntime)
{
	ఒకవేళ (vruntime_cmp(vruntime, ">", max_vruntime))
		max_vruntime = vruntime;

	తిరిగి max_vruntime;
}

స్థిర అంతఃవరుస __maybe_unused u64 min_vruntime(u64 min_vruntime, u64 vruntime)
{
	ఒకవేళ (vruntime_cmp(vruntime, "<", min_vruntime))
		min_vruntime = vruntime;

	తిరిగి min_vruntime;
}

స్థిర అంతఃవరుస bool entity_before(స్థిరరాశి నిర్మాణం sched_entity *a,
				 స్థిరరాశి నిర్మాణం sched_entity *b)
{
	/*
	 * Tiebreak on vruntime seems unnecessary since it can
	 * hardly happen.
	 */
	తిరిగి vruntime_cmp(a->deadline, "<", b->deadline);
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
స్థిర అంతఃవరుస s64 entity_కీలకం(నిర్మాణం cfs_rq *cfs_rq, నిర్మాణం sched_entity *se)
{
	తిరిగి vruntime_op(se->vruntime, "-", cfs_rq->zero_vruntime);
}

#నిర్వచించు __node_2_se(కణుపు) \
	rb_నమోదు((కణుపు), నిర్మాణం sched_entity, నిరంతరం_కణుపు)

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
స్థిర అంతఃవరుస గుర్తులేని దీర్ఘ avg_vruntime_బరువు(నిర్మాణం cfs_rq *cfs_rq, గుర్తులేని దీర్ఘ w)
{
#నిర్వచితమైతే CONFIG_64_బిట్టు
	ఒకవేళ (cfs_rq->కూడిక_జరుపు)
		w = max(2UL, w >> cfs_rq->కూడిక_జరుపు);
#అయితే_ముగింపు
	తిరిగి w;
}

స్థిర అంతఃవరుస శూన్య
__sum_w_vruntime_add(నిర్మాణం cfs_rq *cfs_rq, నిర్మాణం sched_entity *se)
{
	గుర్తులేని దీర్ఘ బరువు = avg_vruntime_బరువు(cfs_rq, se->h_ఎక్కించు.బరువు);
	s64 w_vruntime, కీలకం = entity_కీలకం(cfs_rq, se);

	w_vruntime = కీలకం * బరువు;
	WARN_ON_ONCE((w_vruntime >> 63) != (w_vruntime >> 62));

	cfs_rq->కూడిక_w_vruntime += w_vruntime;
	cfs_rq->కూడిక_బరువు += బరువు;
}

స్థిర శూన్య
కూడిక_w_vruntime_కూడు_paranoid(నిర్మాణం cfs_rq *cfs_rq, నిర్మాణం sched_entity *se)
{
	గుర్తులేని దీర్ఘ బరువు;
	s64 కీలకం, tmp;

again:
	బరువు = avg_vruntime_బరువు(cfs_rq, se->h_ఎక్కించు.బరువు);
	కీలకం = entity_కీలకం(cfs_rq, se);

	ఒకవేళ (check_mul_అధికప్రవాహం(కీలకం, బరువు, &కీలకం))
		వెళ్ళు అధికప్రవాహం;

	ఒకవేళ (check_కూడు_అధికప్రవాహం(cfs_rq->కూడిక_w_vruntime, కీలకం, &tmp))
		వెళ్ళు అధికప్రవాహం;

	cfs_rq->కూడిక_w_vruntime = tmp;
	cfs_rq->కూడిక_బరువు += బరువు;
	తిరిగి;

అధికప్రవాహం:
	/*
	 * There's gotta be a limit -- if we're still failing at this point
	 * there's really nothing much to be done about things.
	 */
	BUG_ON(cfs_rq->కూడిక_జరుపు >= 10);
	cfs_rq->కూడిక_జరుపు++;

	/*
	 * Note: \Sum (k_i * (w_i >> 1)) != (\Sum (k_i * w_i)) >> 1
	 */
	cfs_rq->కూడిక_w_vruntime = 0;
	cfs_rq->కూడిక_బరువు = 0;

	కొరకు (నిర్మాణం rb_కణుపు *కణుపు = cfs_rq->tasks_timeline.rb_leftmost;
	     కణుపు; కణుపు = rb_తదుపరి(కణుపు))
		__sum_w_vruntime_add(cfs_rq, __node_2_se(కణుపు));

	వెళ్ళు again;
}

స్థిర శూన్య
కూడిక_w_vruntime_కూడు(నిర్మాణం cfs_rq *cfs_rq, నిర్మాణం sched_entity *se)
{
	ఒకవేళ (sched_feat(PARANOID_AVG))
		తిరిగి కూడిక_w_vruntime_కూడు_paranoid(cfs_rq, se);

	__sum_w_vruntime_add(cfs_rq, se);
}

స్థిర శూన్య
కూడిక_w_vruntime_sub(నిర్మాణం cfs_rq *cfs_rq, నిర్మాణం sched_entity *se)
{
	గుర్తులేని దీర్ఘ బరువు = avg_vruntime_బరువు(cfs_rq, se->h_ఎక్కించు.బరువు);
	s64 కీలకం = entity_కీలకం(cfs_rq, se);

	cfs_rq->కూడిక_w_vruntime -= కీలకం * బరువు;
	cfs_rq->కూడిక_బరువు -= బరువు;
}

స్థిర అంతఃవరుస
శూన్య update_zero_vruntime(నిర్మాణం cfs_rq *cfs_rq, s64 delta)
{
	/*
	 * v' = v + d ==> sum_w_vruntime' = sum_w_vruntime - d*sum_weight
	 */
	cfs_rq->కూడిక_w_vruntime -= cfs_rq->కూడిక_బరువు * delta;
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
u64 avg_vruntime(నిర్మాణం cfs_rq *cfs_rq)
{
	నిర్మాణం sched_entity *curr = cfs_rq->curr;
	దీర్ఘ బరువు = cfs_rq->కూడిక_బరువు;
	s64 delta = 0;

	ఒకవేళ (curr && !curr->on_rq)
		curr = ఖాళీ;

	ఒకవేళ (బరువు) {
		s64 runtime = cfs_rq->కూడిక_w_vruntime;

		ఒకవేళ (curr) {
			గుర్తులేని దీర్ఘ w = avg_vruntime_బరువు(cfs_rq, curr->h_ఎక్కించు.బరువు);

			runtime += entity_కీలకం(cfs_rq, curr) * w;
			బరువు += w;
		}

		/* sign flips effective floor / ceiling */
		ఒకవేళ (runtime < 0)
			runtime -= (బరువు - 1);

		delta = div64_long(runtime, బరువు);
	} లేకపోతే ఒకవేళ (curr) {
		/*
		 * When there is but one element, it is the average.
		 */
		delta = curr->vruntime - cfs_rq->zero_vruntime;
	}

	update_zero_vruntime(cfs_rq, delta);

	తిరిగి cfs_rq->zero_vruntime;
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
స్థిర u64 ineligible_vruntime(నిర్మాణం cfs_rq *cfs_rq)
{
	నిర్మాణం sched_entity *curr = cfs_rq->curr;
	దీర్ఘ బరువు = cfs_rq->కూడిక_బరువు;
	s64 delta = 0;

	ఒకవేళ (curr && !curr->on_rq)
		curr = ఖాళీ;

	ఒకవేళ (బరువు) {
		s64 runtime = cfs_rq->కూడిక_w_vruntime;

		/*
		 * Do not add @curr to obtain the effective '- w_j' terms.
		 */

		/* sign flips effective floor / ceiling */
		ఒకవేళ (runtime < 0)
			runtime -= (బరువు - 1);

		delta = div64_long(runtime, బరువు);
	}

	తిరిగి cfs_rq->zero_vruntime + delta + 1;
}

స్థిర అంతఃవరుస u64 cfs_rq_max_ముక్క(నిర్మాణం cfs_rq *cfs_rq);

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
స్థిర s64 entity_lag(నిర్మాణం cfs_rq *cfs_rq, నిర్మాణం sched_entity *se, u64 avruntime)
{
	u64 max_ముక్క = cfs_rq_max_ముక్క(cfs_rq) + TICK_NSEC;
	s64 vlag, హద్దు;

	vlag = avruntime - se->vruntime;
	హద్దు = calc_delta_fair(max_ముక్క, se);

	తిరిగి clamp(vlag, -హద్దు, హద్దు);
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
స్థిర __always_inline
bool update_entity_lag(నిర్మాణం cfs_rq *cfs_rq, నిర్మాణం sched_entity *se)
{
	u64 avruntime = avg_vruntime(cfs_rq);
	s64 vlag = entity_lag(cfs_rq, se, avruntime);

	ఒకవేళ (se->sched_delayed) {
		/* previous vlag < 0 otherwise se would not be delayed */
		vlag = max(vlag, se->vlag);
		ఒకవేళ (sched_feat(DELAY_ZERO))
			vlag = min(vlag, 0);
	}
	se->vlag = vlag;

	తిరిగి avruntime - vlag != se->vruntime;
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
స్థిర పూర్ణాంకం vruntime_eligible(నిర్మాణం cfs_rq *cfs_rq, u64 vruntime)
{
	నిర్మాణం sched_entity *curr = cfs_rq->curr;
	s64 కీలకం, avg = cfs_rq->కూడిక_w_vruntime;
	దీర్ఘ ఎక్కించు = cfs_rq->కూడిక_బరువు;

	ఒకవేళ (curr && curr->on_rq) {
		గుర్తులేని దీర్ఘ బరువు = avg_vruntime_బరువు(cfs_rq, curr->h_ఎక్కించు.బరువు);

		avg += entity_కీలకం(cfs_rq, curr) * బరువు;
		ఎక్కించు += బరువు;
	}

	కీలకం = vruntime_op(vruntime, "-", cfs_rq->zero_vruntime);

	/*
	 * The worst case term for @key includes 'NSEC_TICK * NICE_0_LOAD'
	 * and @load obviously includes NICE_0_LOAD. NSEC_TICK is around 24
	 * bits, while NICE_0_LOAD is 20 on 64bit and 10 otherwise.
	 *
	 * This gives that on 64bit the product will be at least 64bit which
	 * overflows s64, while on 32bit it will only be 44bits and should fit
	 * comfortably.
	 */
#నిర్వచితమైతే CONFIG_64_బిట్టు
#నిర్వచితమైతే CONFIG_ARCH_SUPPORTS_INT128
	/* This often results in simpler code than __builtin_mul_overflow(). */
	తిరిగి avg >= (__int128)కీలకం * ఎక్కించు;
#లేకపోతే
	s64 rhs;
	/*
	 * On overflow, the sign of key tells us the correct answer: a large
	 * positive key means vruntime >> V, so not eligible; a large negative
	 * key means vruntime << V, so eligible.
	 */
	ఒకవేళ (check_mul_అధికప్రవాహం(కీలకం, ఎక్కించు, &rhs))
		తిరిగి కీలకం <= 0;

	తిరిగి avg >= rhs;
#అయితే_ముగింపు
#లేకపోతే /* 32bit */
	తిరిగి avg >= కీలకం * ఎక్కించు;
#అయితే_ముగింపు
}

పూర్ణాంకం entity_eligible(నిర్మాణం cfs_rq *cfs_rq, నిర్మాణం sched_entity *se)
{
	తిరిగి vruntime_eligible(cfs_rq, se->vruntime);
}

స్థిర అంతఃవరుస u64 cfs_rq_min_ముక్క(నిర్మాణం cfs_rq *cfs_rq)
{
	నిర్మాణం sched_entity *మూలం = __pick_root_entity(cfs_rq);
	నిర్మాణం sched_entity *curr = cfs_rq->curr;
	u64 min_ముక్క = ~0ULL;

	ఒకవేళ (curr && curr->on_rq)
		min_ముక్క = curr->ముక్క;

	ఒకవేళ (మూలం)
		min_ముక్క = min(min_ముక్క, మూలం->min_ముక్క);

	తిరిగి min_ముక్క;
}

స్థిర అంతఃవరుస u64 cfs_rq_max_ముక్క(నిర్మాణం cfs_rq *cfs_rq)
{
	నిర్మాణం sched_entity *మూలం = __pick_root_entity(cfs_rq);
	నిర్మాణం sched_entity *curr = cfs_rq->curr;
	u64 max_ముక్క = 0ULL;

	ఒకవేళ (curr && curr->on_rq)
		max_ముక్క = curr->ముక్క;

	ఒకవేళ (మూలం)
		max_ముక్క = max(max_ముక్క, మూలం->max_ముక్క);

	తిరిగి max_ముక్క;
}

స్థిర అంతఃవరుస bool __entity_less(నిర్మాణం rb_కణుపు *a, స్థిరరాశి నిర్మాణం rb_కణుపు *b)
{
	తిరిగి entity_before(__node_2_se(a), __node_2_se(b));
}

/* min() for wrapping vruntimes */
స్థిర అంతఃవరుస u64 __min_vruntime(u64 a, u64 b)
{
	తిరిగి vruntime_cmp(a, "<", b) ? a : b;
}

RB_DECLARE_CALLBACKS(స్థిర, min_vruntime_cb,
		     నిర్మాణం sched_entity, నిరంతరం_కణుపు,
		     RB_AUG(vruntime, min_vruntime, __min_vruntime),
		     RB_AUG(ముక్క, min_ముక్క, min),
		     RB_AUG(ముక్క, max_ముక్క, max));

/*
 * Enqueue an entity into the rb-tree:
 */
స్థిర శూన్య __enqueue_entity(నిర్మాణం cfs_rq *cfs_rq, నిర్మాణం sched_entity *se)
{
	WARN_ON_ONCE(&rq_యొక్క(cfs_rq)->cfs != cfs_rq);
	WARN_ON_ONCE(!entity_is_task(se));

	కూడిక_w_vruntime_కూడు(cfs_rq, se);
	se->min_vruntime = se->vruntime;
	se->min_ముక్క = se->ముక్క;
	se->max_ముక్క = se->ముక్క;

	rb_కూడు_augmented_cached(&se->నిరంతరం_కణుపు, &cfs_rq->tasks_timeline,
				__entity_less, &min_vruntime_cb);
}

స్థిర శూన్య __dequeue_entity(నిర్మాణం cfs_rq *cfs_rq, నిర్మాణం sched_entity *se)
{
	WARN_ON_ONCE(&rq_యొక్క(cfs_rq)->cfs != cfs_rq);
	WARN_ON_ONCE(!entity_is_task(se));

	rb_erase_augmented_cached(&se->నిరంతరం_కణుపు, &cfs_rq->tasks_timeline,
				  &min_vruntime_cb);
	కూడిక_w_vruntime_sub(cfs_rq, se);
}

నిర్మాణం sched_entity *__pick_root_entity(నిర్మాణం cfs_rq *cfs_rq)
{
	నిర్మాణం rb_కణుపు *మూలం = cfs_rq->tasks_timeline.rb_మూలం.rb_కణుపు;

	ఒకవేళ (!మూలం)
		తిరిగి ఖాళీ;

	తిరిగి __node_2_se(మూలం);
}

నిర్మాణం sched_entity *__pick_first_entity(నిర్మాణం cfs_rq *cfs_rq)
{
	నిర్మాణం rb_కణుపు *ఎడమ = rb_మొదటి_cached(&cfs_rq->tasks_timeline);

	ఒకవేళ (!ఎడమ)
		తిరిగి ఖాళీ;

	తిరిగి __node_2_se(ఎడమ);
}

/*
 * Set the vruntime up to which an entity can run before looking
 * for another entity to pick.
 * In case of run to parity, we use the shortest slice of the enqueued
 * entities to set the protected period.
 * When run to parity is disabled, we give a minimum quantum to the running
 * entity to ensure progress.
 */
స్థిర అంతఃవరుస శూన్య సమితి_protect_ముక్క(నిర్మాణం cfs_rq *cfs_rq, నిర్మాణం sched_entity *se)
{
	u64 ముక్క = normalized_sysctl_sched_పునాది_ముక్క;
	u64 vprot = se->deadline;

	ఒకవేళ (sched_feat(నిరంతరం_గమ్యం_PARITY))
		ముక్క = cfs_rq_min_ముక్క(cfs_rq);

	ముక్క = min(ముక్క, se->ముక్క);

	/* If there are shorter slices than se's one */
	ఒకవేళ (ముక్క != se->ముక్క) {
		vprot = min_vruntime(vprot, se->vruntime + calc_delta_fair(ముక్క, se));
		ఒకవేళ (sched_feat(PREEMPT_SHORT))
			vprot = min_vruntime(vprot, ineligible_vruntime(cfs_rq));
	}

	se->vprot = vprot;
}

స్థిర అంతఃవరుస శూన్య update_protect_ముక్క(నిర్మాణం cfs_rq *cfs_rq, నిర్మాణం sched_entity *se)
{
	u64 vruntime = min_vruntime(se->vruntime, avg_vruntime(cfs_rq));
	u64 ముక్క = cfs_rq_min_ముక్క(cfs_rq);
	u64 vprot = min_vruntime(se->vprot, vruntime + calc_delta_fair(ముక్క, se));

	ఒకవేళ (sched_feat(PREEMPT_SHORT) && ముక్క != se->ముక్క)
		vprot = min_vruntime(vprot, ineligible_vruntime(cfs_rq));

	se->vprot = vprot;
}

స్థిర అంతఃవరుస bool protect_ముక్క(నిర్మాణం sched_entity *se)
{
	తిరిగి vruntime_cmp(se->vruntime, "<", se->vprot);
}

స్థిర అంతఃవరుస శూన్య cancel_protect_ముక్క(నిర్మాణం sched_entity *se)
{
	ఒకవేళ (protect_ముక్క(se))
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
స్థిర నిర్మాణం sched_entity *pick_eevdf(నిర్మాణం cfs_rq *cfs_rq, bool protect)
{
	నిర్మాణం rb_కణుపు *కణుపు = cfs_rq->tasks_timeline.rb_మూలం.rb_కణుపు;
	నిర్మాణం sched_entity *se = __pick_first_entity(cfs_rq);
	నిర్మాణం sched_entity *curr = cfs_rq->curr;
	నిర్మాణం sched_entity *ఉత్తమ = ఖాళీ;

	/*
	 * We can safely skip eligibility check if there is only one entity
	 * in this cfs_rq, saving some cycles.
	 */
	ఒకవేళ (cfs_rq->h_nr_queued == 1)
		తిరిగి curr && curr->on_rq ? curr : se;

	/*
	 * Picking the ->next buddy will affect latency but not fairness.
	 */
	ఒకవేళ (sched_feat(PICK_BUDDY) && protect &&
	    cfs_rq->తదుపరి && entity_eligible(cfs_rq, cfs_rq->తదుపరి)) {
		/* ->next will never be delayed */
		WARN_ON_ONCE(cfs_rq->తదుపరి->sched_delayed);
		తిరిగి cfs_rq->తదుపరి;
	}

	ఒకవేళ (curr && (!curr->on_rq || !entity_eligible(cfs_rq, curr)))
		curr = ఖాళీ;

	ఒకవేళ (curr && protect && protect_ముక్క(curr))
		తిరిగి curr;

	/* Pick the leftmost entity if it's eligible */
	ఒకవేళ (se && entity_eligible(cfs_rq, se)) {
		ఉత్తమ = se;
		వెళ్ళు దొరికింది;
	}

	/* Heap search for the EEVD entity */
	అయితే (కణుపు) {
		నిర్మాణం rb_కణుపు *ఎడమ = కణుపు->rb_ఎడమ;

		/*
		 * Eligible entities in left subtree are always better
		 * choices, since they have earlier deadlines.
		 */
		ఒకవేళ (ఎడమ && vruntime_eligible(cfs_rq,
					__node_2_se(ఎడమ)->min_vruntime)) {
			కణుపు = ఎడమ;
			కొనసాగు;
		}

		se = __node_2_se(కణుపు);

		/*
		 * The left subtree either is empty or has no eligible
		 * entity, so check the current node since it is the one
		 * with earliest deadline that might be eligible.
		 */
		ఒకవేళ (entity_eligible(cfs_rq, se)) {
			ఉత్తమ = se;
			ఆపు;
		}

		కణుపు = కణుపు->rb_కుడి;
	}
దొరికింది:
	ఒకవేళ (!ఉత్తమ || (curr && entity_before(curr, ఉత్తమ)))
		ఉత్తమ = curr;

	తిరిగి ఉత్తమ;
}

నిర్మాణం sched_entity *__pick_last_entity(నిర్మాణం cfs_rq *cfs_rq)
{
	నిర్మాణం rb_కణుపు *చివరి = rb_చివరి(&cfs_rq->tasks_timeline.rb_మూలం);

	ఒకవేళ (!చివరి)
		తిరిగి ఖాళీ;

	తిరిగి __node_2_se(చివరి);
}

/**************************************************************
 * Scheduling class statistics methods:
 */
పూర్ణాంకం sched_update_scaling(శూన్య)
{
	గుర్తులేని పూర్ణాంకం గుణకం = get_update_sysctl_గుణకం();

#నిర్వచించు WRT_SYSCTL(name) \
	(normalized_sysctl_##name = sysctl_##name / (గుణకం))
	WRT_SYSCTL(sched_పునాది_ముక్క);
#నిర్వచనం_రద్దు WRT_SYSCTL

	తిరిగి 0;
}

స్థిర శూన్య clear_buddies(నిర్మాణం cfs_rq *cfs_rq, నిర్మాణం sched_entity *se);

/*
 * XXX: strictly: vd_i += N*r_i/w_i such that: vd_i > ve_i
 * this is probably good enough.
 */
స్థిర bool update_deadline(నిర్మాణం cfs_rq *cfs_rq, నిర్మాణం sched_entity *se)
{
	ఒకవేళ (vruntime_cmp(se->vruntime, "<", se->deadline))
		తిరిగి false;

	/*
	 * For EEVDF the virtual time slope is determined by w_i (iow.
	 * nice) while the request time r_i is determined by
	 * sysctl_sched_base_slice.
	 */
	ఒకవేళ (!se->custom_ముక్క)
		se->ముక్క = sysctl_sched_పునాది_ముక్క;

	/*
	 * EEVDF: vd_i = ve_i + r_i / w_i
	 */
	se->deadline = se->vruntime + calc_delta_fair(se->ముక్క, se);
	avg_vruntime(cfs_rq);

	/*
	 * The task has consumed its request, reschedule.
	 */
	తిరిగి true;
}

#అన్తర్భావ "pelt.h"

స్థిర పూర్ణాంకం select_idle_sibling(నిర్మాణం task_struct *p, పూర్ణాంకం prev_cpu, పూర్ణాంకం cpu);
స్థిర గుర్తులేని దీర్ఘ task_h_ఎక్కించు(నిర్మాణం task_struct *p);
స్థిర గుర్తులేని దీర్ఘ సామర్థ్యం_యొక్క(పూర్ణాంకం cpu);

/* Give new sched_entity start runnable values to heavy its load in infant time */
శూన్య init_entity_runnable_సరాసరి(నిర్మాణం sched_entity *se)
{
	నిర్మాణం sched_avg *sa = &se->avg;

	memset(sa, 0, పరిమాణం(*sa));

	/*
	 * Tasks are initialized with full load to be seen as heavy tasks until
	 * they get a chance to stabilize to their real load level.
	 * Group entities are initialized with zero load to reflect the fact that
	 * nothing has been attached to the task group yet.
	 */
	ఒకవేళ (entity_is_task(se))
		sa->ఎక్కించు_avg = scale_ఎక్కించు_కిందికి(se->ఎక్కించు.బరువు);

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
శూన్య post_init_entity_util_avg(నిర్మాణం task_struct *p)
{
	నిర్మాణం sched_entity *se = &p->se;
	నిర్మాణం cfs_rq *cfs_rq = cfs_rq_యొక్క(se);
	నిర్మాణం sched_avg *sa = &se->avg;
	దీర్ఘ cpu_scale = arch_scale_cpu_సామర్థ్యం(cpu_యొక్క(rq_యొక్క(cfs_rq)));
	దీర్ఘ cap = (దీర్ఘ)(cpu_scale - cfs_rq->avg.util_avg) / 2;

	ఒకవేళ (p->sched_class != &fair_sched_class) {
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
		se->avg.చివరి_update_సమయం = cfs_rq_clock_pelt(cfs_rq);
		తిరిగి;
	}

	ఒకవేళ (cap > 0) {
		ఒకవేళ (cfs_rq->avg.util_avg != 0) {
			sa->util_avg  = cfs_rq->avg.util_avg * se_బరువు(se);
			sa->util_avg /= (cfs_rq->avg.ఎక్కించు_avg + 1);

			ఒకవేళ (sa->util_avg > cap)
				sa->util_avg = cap;
		} లేకపోతే {
			sa->util_avg = cap;
		}
	}

	sa->runnable_avg = sa->util_avg;
}

స్థిర అంతఃవరుస శూన్య account_mm_sched(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, s64 delta_exec);

స్థిర s64 update_se(నిర్మాణం rq *rq, నిర్మాణం sched_entity *se)
{
	u64 now = rq_clock_task(rq);
	s64 delta_exec;

	delta_exec = now - se->exec_ప్రారంభం;
	ఒకవేళ (unlikely(delta_exec <= 0))
		తిరిగి delta_exec;

	se->exec_ప్రారంభం = now;
	ఒకవేళ (entity_is_task(se)) {
		నిర్మాణం task_struct *running = rq->curr;
		/*
		 * If se is a task, we account the time against the running
		 * task, as w/ proxy-exec they may not be the same.
		 */
		running->se.exec_ప్రారంభం = now;
		running->se.కూడిక_exec_runtime += delta_exec;

		trace_sched_stat_runtime(running, delta_exec);
		account_సమూహం_exec_runtime(running, delta_exec);
		account_mm_sched(rq, running, delta_exec);

		cgroup_account_cputime(running, delta_exec);
	} లేకపోతే {
		/* If not task, account the time against donor se  */
		se->కూడిక_exec_runtime += delta_exec;
	}

	ఒకవేళ (schedstat_enabled()) {
		నిర్మాణం sched_statistics *stats;

		stats = __schedstats_from_se(se);
		__schedstat_set(stats->exec_max,
				max(delta_exec, stats->exec_max));
	}

	తిరిగి delta_exec;
}

#నిర్వచితమైతే CONFIG_SCHED_CACHE

/*
 * XXX numbers come from a place the sun don't shine -- probably wants to be SD
 * tunable or so.
 */
#నిర్వచించు EPOCH_PERIOD	(HZ / 100)	/* 10 ms */
#నిర్వచించు EPOCH_LLC_AFFINITY_TIMEOUT	5	/* 50 ms */
__read_mostly గుర్తులేని పూర్ణాంకం llc_aggr_tolerance	= 1;
__read_mostly గుర్తులేని పూర్ణాంకం llc_epoch_period	= EPOCH_PERIOD;
__read_mostly గుర్తులేని పూర్ణాంకం llc_epoch_affinity_timeout = EPOCH_LLC_AFFINITY_TIMEOUT;
__read_mostly గుర్తులేని పూర్ణాంకం llc_imb_pct		= 20;
__read_mostly గుర్తులేని పూర్ణాంకం llc_overaggr_pct	= 50;

స్థిర పూర్ణాంకం llc_id(పూర్ణాంకం cpu)
{
	ఒకవేళ (cpu < 0)
		తిరిగి -1;

	తిరిగి చొప్పున_cpu(sd_llc_id, cpu);
}

స్థిర అంతఃవరుస పూర్ణాంకం get_sched_cache_scale(పూర్ణాంకం mul)
{
	గుర్తులేని పూర్ణాంకం tol = READ_ONCE(llc_aggr_tolerance);

	ఒకవేళ (!tol)
		తిరిగి 0;

	ఒకవేళ (tol >= 100)
		తిరిగి గరిష్ఠ_పూర్ణాంకం;

	తిరిగి (1 + (tol - 1) * mul);
}

స్థిర bool exceed_llc_సామర్థ్యం(నిర్మాణం sched_cache_సమూహం *grp, పూర్ణాంకం cpu)
{
#నిర్వచితమైతే CONFIG_NUMA_BALANCING
	గుర్తులేని దీర్ఘ llc, footprint;
	నిర్మాణం sched_domain *sd;
	పూర్ణాంకం scale;

	guard(rcu)();

	sd = rcu_dereference_sched_domain(cpu_rq(cpu)->sd);
	ఒకవేళ (!sd)
		తిరిగి true;

	ఒకవేళ (static_branch_likely(&sched_numa_balancing)) {
		/*
		 * TBD: RDT exclusive LLC ways reserved should be
		 * excluded.
		 */
		llc = sd->llc_బైట్లు;
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
		ఒకవేళ (scale == గరిష్ఠ_పూర్ణాంకం)
			తిరిగి false;

		తిరిగి ((llc * (u64)scale) < (footprint * పుట_కొలత));
	}
#అయితే_ముగింపు
	తిరిగి false;
}

స్థిర bool invalid_llc_nr(నిర్మాణం sched_cache_సమూహం *grp, నిర్మాణం task_struct *p,
			   పూర్ణాంకం cpu)
{
	పూర్ణాంకం scale;

	ఒకవేళ (get_nr_దారాలు(p) <= 1)
		తిరిగి true;

	/*
	 * Scale the number of 'cores' in a LLC by llc_aggr_tolerance
	 * and compare it to the task's active threads.
	 */
	scale = get_sched_cache_scale(1);
	ఒకవేళ (scale == గరిష్ఠ_పూర్ణాంకం)
		తిరిగి false;

	తిరిగి !సరిపోతుంది_సామర్థ్యం((READ_ONCE(grp->nr_running_avg) * cpu_smt_అంకె_దారాలు),
			(scale * చొప్పున_cpu(sd_llc_కొలత, cpu)));
}

/*
 * A task counts in nr_pref_llc_running while it is queued on its preferred
 * LLC (pref_llc_queued) and runnable (!sched_delayed), keeping the counter in
 * the runnable domain so alb_break_llc() can compare it with h_nr_runnable.
 */
స్థిర bool task_pref_llc_runnable(నిర్మాణం task_struct *p)
{
	తిరిగి p->pref_llc_queued && !p->se.sched_delayed;
}

స్థిర శూన్య pref_llc_running_inc(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
	ఒకవేళ (task_pref_llc_runnable(p))
		rq->nr_pref_llc_running++;
}

స్థిర శూన్య pref_llc_running_dec(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
	ఒకవేళ (task_pref_llc_runnable(p))
		rq->nr_pref_llc_running--;
}

స్థిర శూన్య account_llc_వరుసలో_చేర్చు(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
	పూర్ణాంకం pref_llc, pref_llc_queued;
	నిర్మాణం sched_domain *sd;

	pref_llc = p->preferred_llc;
	ఒకవేళ (pref_llc < 0)
		తిరిగి;

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
	ఒకవేళ (sd && (గుర్తులేని పూర్ణాంకం)pref_llc < sd->llc_max)
		sd->llc_counts[pref_llc]++;
}

స్థిర శూన్య account_llc_వరుస_నుండి_తీయి(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
	నిర్మాణం sched_domain *sd;
	పూర్ణాంకం pref_llc;

	pref_llc = p->preferred_llc;
	ఒకవేళ (pref_llc < 0)
		తిరిగి;

	rq->nr_llc_running--;
	ఒకవేళ (p->pref_llc_queued) {
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
	ఒకవేళ (sd && (గుర్తులేని పూర్ణాంకం)pref_llc < sd->llc_max) {
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
		ఒకవేళ (sd->llc_counts[pref_llc])
			sd->llc_counts[pref_llc]--;
	}
}

పూర్ణాంకం mm_init_sched(నిర్మాణం mm_struct *mm,
		  నిర్మాణం sched_cache_సమయం __percpu *_pcpu_sched)
{
	నిర్మాణం sched_cache_సమూహం *grp;
	గుర్తులేని దీర్ఘ epoch = 0;
	పూర్ణాంకం i;

	grp = kzalloc_obj(*grp);
	ఒకవేళ (!grp) {
		free_percpu(_pcpu_sched);
		mm->sched_cache_grp = ఖాళీ;
		తిరిగి -ENOMEM;
	}

	for_each_possible_cpu(i) {
		నిర్మాణం sched_cache_సమయం *pcpu_sched = చొప్పున_cpu_ptr(_pcpu_sched, i);
		నిర్మాణం rq *rq = cpu_rq(i);

		pcpu_sched->runtime = 0;
		/* a slightly stale cpu epoch is acceptible */
		pcpu_sched->epoch = rq->cpu_epoch;
		epoch = rq->cpu_epoch;
	}

	raw_spin_తాళం_init(&grp->తాళం);
	grp->epoch = epoch;
	grp->cpu = -1;
	grp->తదుపరి_పరిశీలన = jiffies;
	grp->nr_running_avg = 0;
	grp->footprint = 0;
	refcount_సమితి(&grp->refcnt, 1);
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
	తిరిగి 0;
}

స్థిర శూన్య sched_cache_సమూహం_free_rcu(నిర్మాణం rcu_తల *rcu)
{
	నిర్మాణం sched_cache_సమూహం *grp =
		container_యొక్క(rcu, నిర్మాణం sched_cache_సమూహం, rcu);

	free_percpu(grp->pcpu_sched);
	kfree(grp);
}

స్థిర శూన్య sched_cache_సమూహం_put(నిర్మాణం sched_cache_సమూహం *grp)
{
	ఒకవేళ (!grp || !refcount_dec_and_test(&grp->refcnt))
		తిరిగి;

	call_rcu(&grp->rcu, sched_cache_సమూహం_free_rcu);
}

DEFINE_FREE(sched_cache_సమూహం_put, నిర్మాణం sched_cache_సమూహం *,
	    sched_cache_సమూహం_put(_T));

#నిర్వచించు rcu_deref_sched_cache_grp(tsk) \
	rcu_dereference_check((tsk)->sched_cache_grp, (tsk) == current)

స్థిర నిర్మాణం sched_cache_సమూహం *sched_cache_replace_grp(నిర్మాణం task_struct *p,
							 నిర్మాణం sched_cache_సమూహం *new)
{
	నిర్మాణం sched_cache_సమూహం *old;

	old = rcu_deref_sched_cache_grp(p);
	rcu_కేటాయించు_సూచిక(p->sched_cache_grp, new);

	తిరిగి old;
}

నిర్మాణం sched_cache_సమూహం *sched_cache_సమూహం_get(నిర్మాణం sched_cache_సమూహం *grp)
{
	/*
	 * refcount_inc_not_zero() is the acquire primitive for lockless
	 * (RCU) lookups; plain refcount_inc() would scribble the count if
	 * it already reached zero. Return NULL in that case.
	 */
	ఒకవేళ (grp && !refcount_inc_not_zero(&grp->refcnt))
		grp = ఖాళీ;

	తిరిగి grp;
}

నిర్మాణం sched_cache_సమూహం *task_cache_సమూహం_get(నిర్మాణం task_struct *p)
{
	guard(rcu)();
	తిరిగి sched_cache_సమూహం_get(rcu_dereference(p->sched_cache_grp));
}

శూన్య sched_cache_fork(నిర్మాణం task_struct *p)
{
	/*
	 * The child takes its own reference on the mm's cache group, separate
	 * from the reference held by the mm. @p is not yet visible to readers,
	 * so a plain initializing store is enough.
	 */
	RCU_INIT_సూచిక(p->sched_cache_grp,
			 sched_cache_సమూహం_get(p->mm->sched_cache_grp));
}

శూన్య sched_cache_fork_cleanup(నిర్మాణం task_struct *p)
{
	/*
	 * A fork that fails after sched_cache_fork() never reaches exit_mm(),
	 * so drop the reference here. @p never became visible, so there are no
	 * concurrent readers and the reference we hold keeps the group alive.
	 */
	sched_cache_సమూహం_put(rcu_access_సూచిక(p->sched_cache_grp));
	RCU_INIT_సూచిక(p->sched_cache_grp, ఖాళీ);
}

శూన్య sched_cache_exec_mmap(నిర్మాణం task_struct *p, నిర్మాణం mm_struct *mm)
{
	నిర్మాణం sched_cache_సమూహం *old;

	/*
	 * Acquire the new reference before publishing the pointer, then drop
	 * the old one. @p is current and the only writer of its own pointer.
	 */
	old = sched_cache_replace_grp(p, sched_cache_సమూహం_get(mm->sched_cache_grp));
	sched_cache_సమూహం_put(old);
}

శూన్య sched_cache_exit_mm(నిర్మాణం task_struct *p)
{
	నిర్మాణం sched_cache_సమూహం *grp = sched_cache_replace_grp(p, ఖాళీ);

#నిర్వచితమైతే CONFIG_NUMA_BALANCING
	/*
	 * Subtract this task's footprint from the group before dropping the
	 * reference, so the group footprint converges as its threads exit.
	 * Unlocked for performance; clamp to avoid underflow.
	 */
	ఒకవేళ (grp && p->మొత్తం_numa_దోషాలు) {
		గుర్తులేని దీర్ఘ fp = READ_ONCE(grp->footprint);
		గుర్తులేని దీర్ఘ sub = min(fp, p->మొత్తం_numa_దోషాలు);

		WRITE_ONCE(grp->footprint, fp - sub);
	}
#అయితే_ముగింపు
	sched_cache_సమూహం_put(grp);
}

శూన్య mm_destroy_sched(నిర్మాణం mm_struct *mm)
{
	sched_cache_సమూహం_put(mm->sched_cache_grp);
	mm->sched_cache_grp = ఖాళీ;
}

/* because why would C be fully specified */
స్థిర __always_inline శూన్య __shr_u64(u64 *val, గుర్తులేని పూర్ణాంకం n)
{
	ఒకవేళ (n >= 64) {
		*val = 0;
		తిరిగి;
	}
	*val >>= n;
}

స్థిర అంతఃవరుస శూన్య __update_mm_sched(నిర్మాణం rq *rq,
				     నిర్మాణం sched_cache_సమయం *pcpu_sched)
{
	lockdep_assert_held(&rq->cpu_epoch_తాళం);

	గుర్తులేని పూర్ణాంకం period = max(READ_ONCE(llc_epoch_period), 1U);
	గుర్తులేని దీర్ఘ n, now = jiffies;
	దీర్ఘ delta = now - rq->cpu_epoch_తదుపరి;

	ఒకవేళ (delta > 0) {
		n = (delta + period - 1) / period;
		rq->cpu_epoch += n;
		rq->cpu_epoch_తదుపరి += n * period;
		__shr_u64(&rq->cpu_runtime, n);
	}

	n = rq->cpu_epoch - pcpu_sched->epoch;
	ఒకవేళ (n) {
		pcpu_sched->epoch += n;
		__shr_u64(&pcpu_sched->runtime, n);
	}
}

స్థిర గుర్తులేని దీర్ఘ fraction_mm_sched(నిర్మాణం rq *rq,
				       నిర్మాణం sched_cache_సమయం *pcpu_sched)
{
	guard(raw_spinlock_irqsave)(&rq->cpu_epoch_తాళం);

	__update_mm_sched(rq, pcpu_sched);

	/*
	 * Runtime is a geometric series (r=0.5) and as such will sum to twice
	 * the accumulation period, this means the multiplcation here should
	 * not overflow.
	 */
	తిరిగి div64_u64(NICE_0_ఎక్కించు * pcpu_sched->runtime, rq->cpu_runtime + 1);
}

స్థిర పూర్ణాంకం get_pref_llc(నిర్మాణం task_struct *p, నిర్మాణం sched_cache_సమూహం *grp)
{
	పూర్ణాంకం mm_sched_llc = -1, mm_sched_cpu;

	ఒకవేళ (!grp)
		తిరిగి -1;

	mm_sched_cpu = READ_ONCE(grp->cpu);
	ఒకవేళ (mm_sched_cpu != -1) {
		mm_sched_llc = llc_id(mm_sched_cpu);

#నిర్వచితమైతే CONFIG_NUMA_BALANCING
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
		ఒకవేళ (static_branch_likely(&sched_numa_balancing) &&
		    p->numa_preferred_nid >= 0 &&
		    cpu_గమ్యం_కణుపు(mm_sched_cpu) != p->numa_preferred_nid)
			mm_sched_llc = -1;
#అయితే_ముగింపు
	}

	తిరిగి mm_sched_llc;
}

స్థిర గుర్తులేని పూర్ణాంకం task_running_on_cpu(పూర్ణాంకం cpu, నిర్మాణం task_struct *p);

స్థిర అంతఃవరుస
శూన్య account_mm_sched(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, s64 delta_exec)
{
	నిర్మాణం sched_cache_సమూహం *grp = rcu_dereference_all(p->sched_cache_grp);
	నిర్మాణం sched_cache_సమయం *pcpu_sched;
	పూర్ణాంకం mm_sched_llc = -1;
	గుర్తులేని దీర్ఘ epoch;

	ఒకవేళ (!sched_cache_enabled())
		తిరిగి;

	ఒకవేళ (p->sched_class != &fair_sched_class)
		తిరిగి;
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
	ఒకవేళ (p->flags & PF_KTHREAD || !grp || !grp->pcpu_sched)
		తిరిగి;

	pcpu_sched = చొప్పున_cpu_ptr(grp->pcpu_sched, cpu_యొక్క(rq));

	scoped_guard (raw_spinlock, &rq->cpu_epoch_తాళం) {
		__update_mm_sched(rq, pcpu_sched);
		pcpu_sched->runtime += delta_exec;
		rq->cpu_runtime += delta_exec;
		epoch = rq->cpu_epoch;
	}

	/*
	 * If this process hasn't hit task_cache_work() for a while invalidate
	 * its preferred state.
	 */
	ఒకవేళ ((దీర్ఘ)(epoch - READ_ONCE(grp->epoch)) > llc_epoch_affinity_timeout ||
	    invalid_llc_nr(grp, p, cpu_యొక్క(rq)) ||
	    exceed_llc_సామర్థ్యం(grp, cpu_యొక్క(rq))) {
		ఒకవేళ (READ_ONCE(grp->cpu) != -1)
			WRITE_ONCE(grp->cpu, -1);
	}

	mm_sched_llc = get_pref_llc(p, grp);

	/* task not on rq accounted later in account_entity_enqueue() */
	ఒకవేళ (task_running_on_cpu(rq->cpu, p) &&
	    READ_ONCE(p->preferred_llc) != mm_sched_llc) {
		account_llc_వరుస_నుండి_తీయి(rq, p);
		WRITE_ONCE(p->preferred_llc, mm_sched_llc);
		account_llc_వరుసలో_చేర్చు(rq, p);
	}
}

స్థిర శూన్య task_tick_cache(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
	నిర్మాణం sched_cache_సమూహం *grp = rcu_dereference_all(p->sched_cache_grp);
	నిర్మాణం callback_తల *work = &p->cache_work;
	గుర్తులేని దీర్ఘ epoch;

	ఒకవేళ (!sched_cache_enabled())
		తిరిగి;

	ఒకవేళ (!grp || p->flags & PF_KTHREAD ||
	    !grp->pcpu_sched)
		తిరిగి;

	epoch = rq->cpu_epoch;
	/* avoid moving backwards */
	ఒకవేళ (సమయం_after_eq(grp->epoch, epoch))
		తిరిగి;

	guard(raw_spinlock)(&grp->తాళం);

	ఒకవేళ (work->తదుపరి == work) {
		task_work_కూడు(p, work, TWA_RESUME);
		WRITE_ONCE(grp->epoch, epoch);
	}
}

స్థిర శూన్య get_పరిశీలన_cpumasks(cpumask_var_t cpus, నిర్మాణం task_struct *p,
			      నిర్మాణం sched_cache_సమూహం *grp)
{
#నిర్వచితమైతే CONFIG_NUMA_BALANCING
	పూర్ణాంకం cpu, curr_cpu, nid, pref_nid;

	ఒకవేళ (!static_branch_likely(&sched_numa_balancing))
		వెళ్ళు బహిర్గతం;

	cpu = READ_ONCE(grp->cpu);
	ఒకవేళ (cpu != -1)
		nid = cpu_గమ్యం_కణుపు(cpu);
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
	ఒకవేళ (pref_nid == NUMA_NO_కణుపు)
		వెళ్ళు బహిర్గతం;

	cpumask_or(cpus, cpus, cpumask_యొక్క_కణుపు(pref_nid));

	/* honor the task's preferred LLC CPU */
	ఒకవేళ (cpu != -1 && !cpumask_test_cpu(cpu, cpus) && nid != NUMA_NO_కణుపు)
		cpumask_or(cpus, cpus, cpumask_యొక్క_కణుపు(nid));

	/* make sure the task's current running node is included */
	ఒకవేళ (!cpumask_test_cpu(curr_cpu, cpus))
		cpumask_or(cpus, cpus, cpumask_యొక్క_కణుపు(cpu_గమ్యం_కణుపు(curr_cpu)));

	తిరిగి;

బహిర్గతం:
#అయితే_ముగింపు
	cpumask_copy(cpus, cpu_online_ముసుగు);
}

స్థిర అంతఃవరుస శూన్య update_avg_scale(u64 *avg, u64 sample)
{
	పూర్ణాంకం గుణకం = చొప్పున_cpu(sd_llc_కొలత, raw_smp_processor_id());
	s64 diff = sample - *avg;
	u32 భాజకం;

	/*
	 * Scale the divisor based on the number of CPUs contained
	 * in the LLC. This scaling ensures smaller LLC domains use
	 * a smaller divisor to achieve more precise sensitivity to
	 * changes in nr_running, while larger LLC domains are capped
	 * at a maximum divisor of 8 which is the default smoothing
	 * factor of EWMA in update_avg().
	 */
	భాజకం = clamp_t(u32, (గుణకం >> 2), 2, 8);
	*avg += div64_s64(diff, భాజకం);
}

స్థిర శూన్య task_cache_work(నిర్మాణం callback_తల *work)
{
	నిర్మాణం sched_cache_సమూహం *grp __free(sched_cache_సమూహం_put) = ఖాళీ;
	cpumask_var_t cpus __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	పూర్ణాంకం cpu, m_a_cpu = -1, nr_running = 0, curr_cpu;
	గుర్తులేని దీర్ఘ తదుపరి_పరిశీలన, now = jiffies;
	నిర్మాణం task_struct *p = current, *cur;
	గుర్తులేని దీర్ఘ curr_m_a_occ = 0;
	గుర్తులేని దీర్ఘ m_a_occ = 0;

	WARN_ON_ONCE(work != &p->cache_work);

	work->తదుపరి = work;

	ఒకవేళ (p->flags & PF_EXITING)
		తిరిగి;

	/*
	 * A reference makes sure grp is not released by others. The rcu
	 * lock can not be held till after zalloc_cpumask_var() below,
	 * because the latter might sleep.
	 */
	grp = task_cache_సమూహం_get(p);
	ఒకవేళ (!grp)
		తిరిగి;

	తదుపరి_పరిశీలన = READ_ONCE(grp->తదుపరి_పరిశీలన);
	ఒకవేళ (సమయం_before(now, తదుపరి_పరిశీలన))
		తిరిగి;

	/* only 1 thread is allowed to scan */
	ఒకవేళ (!try_cmpxchg(&grp->తదుపరి_పరిశీలన, &తదుపరి_పరిశీలన,
			 now + max_t(గుర్తులేని దీర్ఘ,
				     READ_ONCE(llc_epoch_period), 1)))
		తిరిగి;

	curr_cpu = task_cpu(p);
	ఒకవేళ (invalid_llc_nr(grp, p, curr_cpu) ||
	    exceed_llc_సామర్థ్యం(grp, curr_cpu)) {
		ఒకవేళ (READ_ONCE(grp->cpu) != -1)
			WRITE_ONCE(grp->cpu, -1);

		తిరిగి;
	}

	ఒకవేళ (!zalloc_cpumask_var(&cpus, GFP_KERNEL))
		తిరిగి;

	scoped_guard (cpus_read_తాళం) {
		guard(rcu)();

		get_పరిశీలన_cpumasks(cpus, p, grp);

		for_each_cpu(cpu, cpus) {
			/* XXX sched_cluster_active */
			నిర్మాణం sched_domain *sd = rcu_dereference_all(చొప్పున_cpu(sd_llc, cpu));
			గుర్తులేని దీర్ఘ occ, m_occ = 0, a_occ = 0;
			పూర్ణాంకం m_cpu = -1, i;

			ఒకవేళ (!sd)
				కొనసాగు;

			for_each_cpu(i, sched_domain_పరిధి(sd)) {
				occ = fraction_mm_sched(cpu_rq(i),
							చొప్పున_cpu_ptr(grp->pcpu_sched, i));
				a_occ += occ;
				ఒకవేళ (occ > m_occ) {
					m_occ = occ;
					m_cpu = i;
				}

				/*
				 * rcu_access_pointer() is used because the
				 * pointer is only compared, never dereferenced.
				 */
				cur = rcu_dereference_all(cpu_rq(i)->curr);
				ఒకవేళ (cur && !(cur->flags & (PF_EXITING | PF_KTHREAD)) &&
				    rcu_access_సూచిక(cur->sched_cache_grp) == grp)
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
			ఒకవేళ (a_occ > m_a_occ) {
				m_a_occ = a_occ;
				m_a_cpu = m_cpu;
			}

			ఒకవేళ (llc_id(cpu) == llc_id(READ_ONCE(grp->cpu)))
				curr_m_a_occ = a_occ;

			cpumask_andnot(cpus, cpus, sched_domain_పరిధి(sd));
		}
	}

	ఒకవేళ (m_a_occ > (2 * curr_m_a_occ)) {
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

శూన్య init_sched_mm(నిర్మాణం task_struct *p)
{
	నిర్మాణం callback_తల *work = &p->cache_work;

	init_task_work(work, task_cache_work);
	work->తదుపరి = work;
	/*
	 * dup_task_struct() copies the parent's task_struct, including its
	 * sched_cache_grp, for which the child holds no reference.  Clear it
	 * here - before copy_mm() runs - so the child never carries a
	 * borrowed pointer that the fork error path would put.
	 */
	RCU_INIT_సూచిక(p->sched_cache_grp, ఖాళీ);
	/*
	 * Reset new task's preference to avoid
	 * polluting account_llc_enqueue().
	 */
	p->preferred_llc = -1;
	p->pref_llc_queued = 0;
}

#లేకపోతే /* CONFIG_SCHED_CACHE */

స్థిర అంతఃవరుస శూన్య account_mm_sched(నిర్మాణం rq *rq, నిర్మాణం task_struct *p,
				    s64 delta_exec) { }

శూన్య init_sched_mm(నిర్మాణం task_struct *p) { }

స్థిర శూన్య task_tick_cache(నిర్మాణం rq *rq, నిర్మాణం task_struct *p) { }

స్థిర అంతఃవరుస పూర్ణాంకం get_pref_llc(నిర్మాణం task_struct *p,
			       నిర్మాణం mm_struct *mm)
{
	తిరిగి -1;
}

స్థిర శూన్య account_llc_వరుసలో_చేర్చు(నిర్మాణం rq *rq, నిర్మాణం task_struct *p) {}

స్థిర శూన్య account_llc_వరుస_నుండి_తీయి(నిర్మాణం rq *rq, నిర్మాణం task_struct *p) {}

స్థిర శూన్య pref_llc_running_inc(నిర్మాణం rq *rq, నిర్మాణం task_struct *p) {}

స్థిర శూన్య pref_llc_running_dec(నిర్మాణం rq *rq, నిర్మాణం task_struct *p) {}

#అయితే_ముగింపు /* CONFIG_SCHED_CACHE */

/*
 * Used by other classes to account runtime.
 */
s64 update_curr_common(నిర్మాణం rq *rq)
{
	తిరిగి update_se(rq, &rq->donor->se);
}

/*
 * Update the current task's runtime statistics.
 */
స్థిర శూన్య update_curr(నిర్మాణం cfs_rq *cfs_rq)
{
	/*
	 * Note: cfs_rq->curr corresponds to the task picked to
	 * run (ie: rq->donor.se) which due to proxy-exec may
	 * not necessarily be the actual task running
	 * (rq->curr.se). This is easy to confuse!
	 */
	నిర్మాణం sched_entity *curr = cfs_rq->h_curr;
	నిర్మాణం rq *rq = rq_యొక్క(cfs_rq);
	s64 delta_exec;
	bool resched;

	ఒకవేళ (unlikely(!curr))
		తిరిగి;

	delta_exec = update_se(rq, curr);
	ఒకవేళ (unlikely(delta_exec <= 0))
		తిరిగి;

	account_cfs_rq_runtime(cfs_rq, delta_exec);

	ఒకవేళ (!entity_is_task(curr))
		తిరిగి;

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

	ఒకవేళ (cfs_rq->h_nr_queued == 1)
		తిరిగి;

	ఒకవేళ (resched || !protect_ముక్క(curr)) {
		resched_curr_lazy(rq);
		clear_buddies(cfs_rq, curr);
	}
}

స్థిర శూన్య update_curr_fair(నిర్మాణం rq *rq)
{
	నిర్మాణం sched_entity *se = &rq->donor->se;

	for_each_sched_entity(se)
		update_curr(cfs_rq_యొక్క(se));
}

స్థిర అంతఃవరుస శూన్య
update_stats_wait_ప్రారంభం_fair(నిర్మాణం cfs_rq *cfs_rq, నిర్మాణం sched_entity *se)
{
	నిర్మాణం sched_statistics *stats;
	నిర్మాణం task_struct *p = ఖాళీ;

	ఒకవేళ (!schedstat_enabled())
		తిరిగి;

	stats = __schedstats_from_se(se);

	ఒకవేళ (entity_is_task(se))
		p = task_యొక్క(se);

	__update_stats_wait_start(rq_యొక్క(cfs_rq), p, stats);
}

స్థిర అంతఃవరుస శూన్య
update_stats_wait_చివర_fair(నిర్మాణం cfs_rq *cfs_rq, నిర్మాణం sched_entity *se)
{
	నిర్మాణం sched_statistics *stats;
	నిర్మాణం task_struct *p = ఖాళీ;

	ఒకవేళ (!schedstat_enabled())
		తిరిగి;

	stats = __schedstats_from_se(se);

	/*
	 * When the sched_schedstat changes from 0 to 1, some sched se
	 * maybe already in the runqueue, the se->statistics.wait_start
	 * will be 0.So it will let the delta wrong. We need to avoid this
	 * scenario.
	 */
	ఒకవేళ (unlikely(!schedstat_val(stats->wait_ప్రారంభం)))
		తిరిగి;

	ఒకవేళ (entity_is_task(se))
		p = task_యొక్క(se);

	__update_stats_wait_end(rq_యొక్క(cfs_rq), p, stats);
}

స్థిర అంతఃవరుస శూన్య
update_stats_వరుసలో_చేర్చు_sleeper_fair(నిర్మాణం cfs_rq *cfs_rq, నిర్మాణం sched_entity *se)
{
	నిర్మాణం sched_statistics *stats;
	నిర్మాణం task_struct *tsk = ఖాళీ;

	ఒకవేళ (!schedstat_enabled())
		తిరిగి;

	stats = __schedstats_from_se(se);

	ఒకవేళ (entity_is_task(se))
		tsk = task_యొక్క(se);

	__update_stats_enqueue_sleeper(rq_యొక్క(cfs_rq), tsk, stats);
}

/*
 * Task is being enqueued - update stats:
 */
స్థిర అంతఃవరుస శూన్య
update_stats_వరుసలో_చేర్చు_fair(నిర్మాణం cfs_rq *cfs_rq, నిర్మాణం sched_entity *se, పూర్ణాంకం flags)
{
	ఒకవేళ (!schedstat_enabled())
		తిరిగి;

	/*
	 * Are we enqueueing a waiting task? (for current tasks
	 * a dequeue/enqueue event is a NOP)
	 */
	ఒకవేళ (se != cfs_rq->h_curr)
		update_stats_wait_ప్రారంభం_fair(cfs_rq, se);

	ఒకవేళ (flags & వరుసలో_చేర్చు_WAKEUP)
		update_stats_వరుసలో_చేర్చు_sleeper_fair(cfs_rq, se);
}

స్థిర అంతఃవరుస శూన్య
update_stats_వరుస_నుండి_తీయి_fair(నిర్మాణం cfs_rq *cfs_rq, నిర్మాణం sched_entity *se, పూర్ణాంకం flags)
{

	ఒకవేళ (!schedstat_enabled())
		తిరిగి;

	/*
	 * Mark the end of the wait period if dequeueing a
	 * waiting task:
	 */
	ఒకవేళ (se != cfs_rq->h_curr)
		update_stats_wait_చివర_fair(cfs_rq, se);

	ఒకవేళ ((flags & వరుస_నుండి_తీయి_SLEEP) && entity_is_task(se)) {
		నిర్మాణం task_struct *tsk = task_యొక్క(se);
		గుర్తులేని పూర్ణాంకం state;

		/* XXX racy against TTWU */
		state = READ_ONCE(tsk->__state);
		ఒకవేళ (state & TASK_INTERRUPTIBLE)
			__schedstat_set(tsk->stats.sleep_ప్రారంభం,
				      rq_clock(rq_యొక్క(cfs_rq)));
		ఒకవేళ (state & TASK_UNINTERRUPTIBLE)
			__schedstat_set(tsk->stats.ఖండం_ప్రారంభం,
				      rq_clock(rq_యొక్క(cfs_rq)));
	}
}

/*
 * We are picking a new current task - update its stats:
 */
స్థిర అంతఃవరుస శూన్య
update_stats_curr_ప్రారంభం(నిర్మాణం cfs_rq *cfs_rq, నిర్మాణం sched_entity *se)
{
	/*
	 * We are starting a new run period:
	 */
	se->exec_ప్రారంభం = rq_clock_task(rq_యొక్క(cfs_rq));
}

/* Check sched_smt_active before calling this to avoid overheads in fastpaths */
స్థిర అంతఃవరుస bool is_core_idle(పూర్ణాంకం cpu)
{
	పూర్ణాంకం sibling;

	for_each_cpu(sibling, cpu_smt_ముసుగు(cpu)) {
		ఒకవేళ (cpu == sibling)
			కొనసాగు;

		ఒకవేళ (!idle_cpu(sibling))
			తిరిగి false;
	}

	తిరిగి true;
}

#నిర్వచితమైతే CONFIG_NUMA
#నిర్వచించు NUMA_IMBALANCE_MIN 2

స్థిర అంతఃవరుస దీర్ఘ
adjust_numa_imbalance(పూర్ణాంకం imbalance, పూర్ణాంకం dst_running, పూర్ణాంకం imb_numa_nr)
{
	/*
	 * Allow a NUMA imbalance if busy CPUs is less than the maximum
	 * threshold. Above this threshold, individual tasks may be contending
	 * for both memory bandwidth and any shared HT resources.  This is an
	 * approximation as the number of running tasks may not be related to
	 * the number of busy CPUs due to sched_setaffinity.
	 */
	ఒకవేళ (dst_running > imb_numa_nr)
		తిరిగి imbalance;

	/*
	 * Allow a small imbalance based on a simple pair of communicating
	 * tasks that remain local when the destination is lightly loaded.
	 */
	ఒకవేళ (imbalance <= NUMA_IMBALANCE_MIN)
		తిరిగి 0;

	తిరిగి imbalance;
}
#అయితే_ముగింపు /* CONFIG_NUMA */

#నిర్వచితమైతే CONFIG_NUMA_BALANCING
/*
 * Approximate time to scan a full NUMA task in ms. The task scan period is
 * calculated based on the tasks virtual memory size and
 * numa_balancing_scan_size.
 */
గుర్తులేని పూర్ణాంకం sysctl_numa_balancing_పరిశీలన_period_min = 1000;
గుర్తులేని పూర్ణాంకం sysctl_numa_balancing_పరిశీలన_period_max = 60000;

/* Portion of address space to scan in MB */
గుర్తులేని పూర్ణాంకం sysctl_numa_balancing_పరిశీలన_కొలత = 256;

/* Scan @scan_size MB every @scan_period after an initial @scan_delay in ms */
గుర్తులేని పూర్ణాంకం sysctl_numa_balancing_పరిశీలన_delay = 1000;

/* The page with hint page fault latency < threshold in ms is considered hot */
గుర్తులేని పూర్ణాంకం sysctl_numa_balancing_hot_పరిమితి = MSEC_చొప్పున_SEC;

నిర్మాణం numa_సమూహం {
	refcount_t refcount;

	spinlock_t తాళం; /* nr_tasks, tasks */
	పూర్ణాంకం nr_tasks;
	pid_t gid;
	పూర్ణాంకం చురుకైన_nodes;

	నిర్మాణం rcu_తల rcu;
	గుర్తులేని దీర్ఘ మొత్తం_దోషాలు;
	గుర్తులేని దీర్ఘ max_దోషాలు_cpu;
	/*
	 * faults[] array is split into two regions: faults_mem and faults_cpu.
	 *
	 * Faults_cpu is used to decide whether memory should move
	 * towards the CPU. As a consequence, these stats are weighted
	 * more by CPU use than by memory faults.
	 */
	గుర్తులేని దీర్ఘ దోషాలు[];
};

/*
 * For functions that can be called in multiple contexts that permit reading
 * ->numa_group (see struct task_struct for locking rules).
 */
స్థిర నిర్మాణం numa_సమూహం *deref_task_numa_సమూహం(నిర్మాణం task_struct *p)
{
	తిరిగి rcu_dereference_check(p->numa_సమూహం, p == current ||
		(lockdep_is_held(__rq_lockp(task_rq(p))) && !READ_ONCE(p->on_cpu)));
}

స్థిర నిర్మాణం numa_సమూహం *deref_curr_numa_సమూహం(నిర్మాణం task_struct *p)
{
	తిరిగి rcu_dereference_protected(p->numa_సమూహం, p == current);
}

స్థిర అంతఃవరుస గుర్తులేని దీర్ఘ సమూహం_దోషాలు_priv(నిర్మాణం numa_సమూహం *ng);
స్థిర అంతఃవరుస గుర్తులేని దీర్ఘ సమూహం_దోషాలు_shared(నిర్మాణం numa_సమూహం *ng);

స్థిర గుర్తులేని పూర్ణాంకం task_nr_పరిశీలన_windows(నిర్మాణం task_struct *p)
{
	గుర్తులేని దీర్ఘ rss = 0;
	గుర్తులేని దీర్ఘ nr_పరిశీలన_పుటలు;

	/*
	 * Calculations based on RSS as non-present and empty pages are skipped
	 * by the PTE scanner and NUMA hinting faults should be trapped based
	 * on resident pages
	 */
	nr_పరిశీలన_పుటలు = MB_గమ్యం_పుటలు(sysctl_numa_balancing_పరిశీలన_కొలత);
	rss = get_mm_rss(p->mm);
	ఒకవేళ (!rss)
		rss = nr_పరిశీలన_పుటలు;

	rss = విడత_పైకి(rss, nr_పరిశీలన_పుటలు);
	తిరిగి rss / nr_పరిశీలన_పుటలు;
}

/* For sanity's sake, never scan more PTEs than MAX_SCAN_WINDOW MB/sec. */
#నిర్వచించు MAX_పరిశీలన_కిటికీ 2560

స్థిర గుర్తులేని పూర్ణాంకం task_పరిశీలన_min(నిర్మాణం task_struct *p)
{
	గుర్తులేని పూర్ణాంకం పరిశీలన_కొలత = READ_ONCE(sysctl_numa_balancing_పరిశీలన_కొలత);
	గుర్తులేని పూర్ణాంకం పరిశీలన, floor;
	గుర్తులేని పూర్ణాంకం windows = 1;

	ఒకవేళ (పరిశీలన_కొలత < MAX_పరిశీలన_కిటికీ)
		windows = MAX_పరిశీలన_కిటికీ / పరిశీలన_కొలత;
	floor = 1000 / windows;

	పరిశీలన = sysctl_numa_balancing_పరిశీలన_period_min / task_nr_పరిశీలన_windows(p);
	తిరిగి max_t(గుర్తులేని పూర్ణాంకం, floor, పరిశీలన);
}

స్థిర గుర్తులేని పూర్ణాంకం task_పరిశీలన_ప్రారంభం(నిర్మాణం task_struct *p)
{
	గుర్తులేని దీర్ఘ smin = task_పరిశీలన_min(p);
	గుర్తులేని దీర్ఘ period = smin;
	నిర్మాణం numa_సమూహం *ng;

	/* Scale the maximum scan period with the amount of shared memory. */
	rcu_read_తాళం();
	ng = rcu_dereference_all(p->numa_సమూహం);
	ఒకవేళ (ng) {
		గుర్తులేని దీర్ఘ shared = సమూహం_దోషాలు_shared(ng);
		గుర్తులేని దీర్ఘ అంతరంగ = సమూహం_దోషాలు_priv(ng);

		period *= refcount_read(&ng->refcount);
		period *= shared + 1;
		period /= అంతరంగ + shared + 1;
	}
	rcu_read_unlock();

	తిరిగి max(smin, period);
}

స్థిర గుర్తులేని పూర్ణాంకం task_పరిశీలన_max(నిర్మాణం task_struct *p)
{
	గుర్తులేని దీర్ఘ smin = task_పరిశీలన_min(p);
	గుర్తులేని దీర్ఘ smax;
	నిర్మాణం numa_సమూహం *ng;

	/* Watch for min being lower than max due to floor calculations */
	smax = sysctl_numa_balancing_పరిశీలన_period_max / task_nr_పరిశీలన_windows(p);

	/* Scale the maximum scan period with the amount of shared memory. */
	ng = deref_curr_numa_సమూహం(p);
	ఒకవేళ (ng) {
		గుర్తులేని దీర్ఘ shared = సమూహం_దోషాలు_shared(ng);
		గుర్తులేని దీర్ఘ అంతరంగ = సమూహం_దోషాలు_priv(ng);
		గుర్తులేని దీర్ఘ period = smax;

		period *= refcount_read(&ng->refcount);
		period *= shared + 1;
		period /= అంతరంగ + shared + 1;

		smax = max(smax, period);
	}

	తిరిగి max(smin, smax);
}

స్థిర శూన్య account_numa_వరుసలో_చేర్చు(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
	rq->nr_numa_running += (p->numa_preferred_nid != NUMA_NO_కణుపు);
	rq->nr_preferred_running += (p->numa_preferred_nid == task_కణుపు(p));
}

స్థిర శూన్య account_numa_వరుస_నుండి_తీయి(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
	rq->nr_numa_running -= (p->numa_preferred_nid != NUMA_NO_కణుపు);
	rq->nr_preferred_running -= (p->numa_preferred_nid == task_కణుపు(p));
}

/* Shared or private faults. */
#నిర్వచించు NR_NUMA_HINT_FAULT_TYPES 2

/* Memory and CPU locality */
#నిర్వచించు NR_NUMA_HINT_FAULT_STATS (NR_NUMA_HINT_FAULT_TYPES * 2)

/* Averaged statistics, and temporary buffers. */
#నిర్వచించు NR_NUMA_HINT_FAULT_BUCKETS (NR_NUMA_HINT_FAULT_STATS * 2)

pid_t task_numa_సమూహం_id(నిర్మాణం task_struct *p)
{
	నిర్మాణం numa_సమూహం *ng;
	pid_t gid = 0;

	rcu_read_తాళం();
	ng = rcu_dereference_all(p->numa_సమూహం);
	ఒకవేళ (ng)
		gid = ng->gid;
	rcu_read_unlock();

	తిరిగి gid;
}

/*
 * The averaged statistics, shared & private, memory & CPU,
 * occupy the first half of the array. The second half of the
 * array is for current counters, which are averaged into the
 * first set by task_numa_placement.
 */
స్థిర అంతఃవరుస పూర్ణాంకం task_దోషాలు_idx(గణన numa_దోషాలు_stats s, పూర్ణాంకం nid, పూర్ణాంకం priv)
{
	తిరిగి NR_NUMA_HINT_FAULT_TYPES * (s * nr_కణుపు_ids + nid) + priv;
}

స్థిర అంతఃవరుస గుర్తులేని దీర్ఘ task_దోషాలు(నిర్మాణం task_struct *p, పూర్ణాంకం nid)
{
	ఒకవేళ (!p->numa_దోషాలు)
		తిరిగి 0;

	తిరిగి p->numa_దోషాలు[task_దోషాలు_idx(NUMA_MEM, nid, 0)] +
		p->numa_దోషాలు[task_దోషాలు_idx(NUMA_MEM, nid, 1)];
}

స్థిర అంతఃవరుస గుర్తులేని దీర్ఘ సమూహం_దోషాలు(నిర్మాణం task_struct *p, పూర్ణాంకం nid)
{
	నిర్మాణం numa_సమూహం *ng = deref_task_numa_సమూహం(p);

	ఒకవేళ (!ng)
		తిరిగి 0;

	తిరిగి ng->దోషాలు[task_దోషాలు_idx(NUMA_MEM, nid, 0)] +
		ng->దోషాలు[task_దోషాలు_idx(NUMA_MEM, nid, 1)];
}

స్థిర అంతఃవరుస గుర్తులేని దీర్ఘ సమూహం_దోషాలు_cpu(నిర్మాణం numa_సమూహం *సమూహం, పూర్ణాంకం nid)
{
	తిరిగి సమూహం->దోషాలు[task_దోషాలు_idx(NUMA_CPU, nid, 0)] +
		సమూహం->దోషాలు[task_దోషాలు_idx(NUMA_CPU, nid, 1)];
}

స్థిర అంతఃవరుస గుర్తులేని దీర్ఘ సమూహం_దోషాలు_priv(నిర్మాణం numa_సమూహం *ng)
{
	గుర్తులేని దీర్ఘ దోషాలు = 0;
	పూర్ణాంకం కణుపు;

	for_each_online_కణుపు(కణుపు) {
		దోషాలు += ng->దోషాలు[task_దోషాలు_idx(NUMA_MEM, కణుపు, 1)];
	}

	తిరిగి దోషాలు;
}

స్థిర అంతఃవరుస గుర్తులేని దీర్ఘ సమూహం_దోషాలు_shared(నిర్మాణం numa_సమూహం *ng)
{
	గుర్తులేని దీర్ఘ దోషాలు = 0;
	పూర్ణాంకం కణుపు;

	for_each_online_కణుపు(కణుపు) {
		దోషాలు += ng->దోషాలు[task_దోషాలు_idx(NUMA_MEM, కణుపు, 0)];
	}

	తిరిగి దోషాలు;
}

/*
 * A node triggering more than 1/3 as many NUMA faults as the maximum is
 * considered part of a numa group's pseudo-interleaving set. Migrations
 * between these nodes are slowed down, to allow things to settle down.
 */
#నిర్వచించు చురుకైన_కణుపు_FRACTION 3

స్థిర bool numa_is_చురుకైన_కణుపు(పూర్ణాంకం nid, నిర్మాణం numa_సమూహం *ng)
{
	తిరిగి సమూహం_దోషాలు_cpu(ng, nid) * చురుకైన_కణుపు_FRACTION > ng->max_దోషాలు_cpu;
}

/* Handle placement on systems where not all nodes are directly connected. */
స్థిర గుర్తులేని దీర్ఘ score_nearby_nodes(నిర్మాణం task_struct *p, పూర్ణాంకం nid,
					పూర్ణాంకం lim_dist, bool task)
{
	గుర్తులేని దీర్ఘ score = 0;
	పూర్ణాంకం కణుపు, max_dist;

	/*
	 * All nodes are directly connected, and the same distance
	 * from each other. No need for fancy placement algorithms.
	 */
	ఒకవేళ (sched_numa_topology_type == NUMA_ప్రత్యక్ష)
		తిరిగి 0;

	/* sched_max_numa_distance may be changed in parallel. */
	max_dist = READ_ONCE(sched_max_numa_దూరం);
	/*
	 * This code is called for each node, introducing N^2 complexity,
	 * which should be OK given the number of nodes rarely exceeds 8.
	 */
	for_each_online_కణుపు(కణుపు) {
		గుర్తులేని దీర్ఘ దోషాలు;
		పూర్ణాంకం dist = కణుపు_దూరం(nid, కణుపు);

		/*
		 * The furthest away nodes in the system are not interesting
		 * for placement; nid was already counted.
		 */
		ఒకవేళ (dist >= max_dist || కణుపు == nid)
			కొనసాగు;

		/*
		 * On systems with a backplane NUMA topology, compare groups
		 * of nodes, and move tasks towards the group with the most
		 * memory accesses. When comparing two nodes at distance
		 * "hoplimit", only nodes closer by than "hoplimit" are part
		 * of each group. Skip other nodes.
		 */
		ఒకవేళ (sched_numa_topology_type == NUMA_BACKPLANE && dist >= lim_dist)
			కొనసాగు;

		/* Add up the faults from nearby nodes. */
		ఒకవేళ (task)
			దోషాలు = task_దోషాలు(p, కణుపు);
		లేకపోతే
			దోషాలు = సమూహం_దోషాలు(p, కణుపు);

		/*
		 * On systems with a glueless mesh NUMA topology, there are
		 * no fixed "groups of nodes". Instead, nodes that are not
		 * directly connected bounce traffic through intermediate
		 * nodes; a numa_group can occupy any set of nodes.
		 * The further away a node is, the less the faults count.
		 * This seems to result in good task placement.
		 */
		ఒకవేళ (sched_numa_topology_type == NUMA_GLUELESS_MESH) {
			దోషాలు *= (max_dist - dist);
			దోషాలు /= (max_dist - LOCAL_దూరం);
		}

		score += దోషాలు;
	}

	తిరిగి score;
}

/*
 * These return the fraction of accesses done by a particular task, or
 * task group, on a particular numa node.  The group weight is given a
 * larger multiplier, in order to group tasks together that are almost
 * evenly spread out between numa nodes.
 */
స్థిర అంతఃవరుస గుర్తులేని దీర్ఘ task_బరువు(నిర్మాణం task_struct *p, పూర్ణాంకం nid,
					పూర్ణాంకం dist)
{
	గుర్తులేని దీర్ఘ దోషాలు, మొత్తం_దోషాలు;

	ఒకవేళ (!p->numa_దోషాలు)
		తిరిగి 0;

	మొత్తం_దోషాలు = p->మొత్తం_numa_దోషాలు;

	ఒకవేళ (!మొత్తం_దోషాలు)
		తిరిగి 0;

	దోషాలు = task_దోషాలు(p, nid);
	దోషాలు += score_nearby_nodes(p, nid, dist, true);

	తిరిగి 1000 * దోషాలు / మొత్తం_దోషాలు;
}

స్థిర అంతఃవరుస గుర్తులేని దీర్ఘ సమూహం_బరువు(నిర్మాణం task_struct *p, పూర్ణాంకం nid,
					 పూర్ణాంకం dist)
{
	నిర్మాణం numa_సమూహం *ng = deref_task_numa_సమూహం(p);
	గుర్తులేని దీర్ఘ దోషాలు, మొత్తం_దోషాలు;

	ఒకవేళ (!ng)
		తిరిగి 0;

	మొత్తం_దోషాలు = ng->మొత్తం_దోషాలు;

	ఒకవేళ (!మొత్తం_దోషాలు)
		తిరిగి 0;

	దోషాలు = సమూహం_దోషాలు(p, nid);
	దోషాలు += score_nearby_nodes(p, nid, dist, false);

	తిరిగి 1000 * దోషాలు / మొత్తం_దోషాలు;
}

/*
 * If memory tiering mode is enabled, cpupid of slow memory page is
 * used to record scan time instead of CPU and PID.  When tiering mode
 * is disabled at run time, the scan time (in cpupid) will be
 * interpreted as CPU and PID.  So CPU needs to be checked to avoid to
 * access out of array bound.
 */
స్థిర అంతఃవరుస bool cpupid_valid(పూర్ణాంకం cpupid)
{
	తిరిగి cpupid_గమ్యం_cpu(cpupid) < nr_cpu_ids;
}

/*
 * For memory tiering mode, if there are enough free pages (more than
 * enough watermark defined here) in fast memory node, to take full
 * advantage of fast memory capacity, all recently accessed slow
 * memory pages will be migrated to fast memory node without
 * considering hot threshold.
 */
స్థిర bool pgdat_free_space_enough(నిర్మాణం pglist_దత్తాంశం *pgdat)
{
	పూర్ణాంకం z;
	గుర్తులేని దీర్ఘ enough_wmark;

	enough_wmark = max(1UL * 1024 * 1024 * 1024 >> పుట_జరుపు,
			   pgdat->కణుపు_present_పుటలు >> 4);
	కొరకు (z = pgdat->nr_zones - 1; z >= 0; z--) {
		నిర్మాణం zone *zone = pgdat->కణుపు_zones + z;

		ఒకవేళ (!populated_zone(zone))
			కొనసాగు;

		ఒకవేళ (zone_watermark_ok(zone, 0,
				      promo_wmark_పుటలు(zone) + enough_wmark,
				      ZONE_MOVABLE, 0))
			తిరిగి true;
	}
	తిరిగి false;
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
స్థిర పూర్ణాంకం numa_hint_fault_latency(నిర్మాణం folio *folio)
{
	పూర్ణాంకం చివరి_సమయం, సమయం;

	సమయం = jiffies_గమ్యం_msecs(jiffies);
	చివరి_సమయం = folio_xchg_access_సమయం(folio, సమయం);

	తిరిగి (సమయం - చివరి_సమయం) & పుట_ACCESS_సమయం_ముసుగు;
}

/*
 * For memory tiering mode, too high promotion/demotion throughput may
 * hurt application latency.  So we provide a mechanism to rate limit
 * the number of pages that are tried to be promoted.
 */
స్థిర bool numa_promotion_రేటు_హద్దు(నిర్మాణం pglist_దత్తాంశం *pgdat,
				      గుర్తులేని దీర్ఘ రేటు_హద్దు, పూర్ణాంకం nr)
{
	గుర్తులేని దీర్ఘ nr_cand;
	గుర్తులేని పూర్ణాంకం now, ప్రారంభం;

	now = jiffies_గమ్యం_msecs(jiffies);
	mod_కణుపు_పుట_state(pgdat, PGPROMOTE_CANDIDATE, nr);
	nr_cand = కణుపు_పుట_state(pgdat, PGPROMOTE_CANDIDATE);
	ప్రారంభం = pgdat->nbp_rl_ప్రారంభం;
	ఒకవేళ (now - ప్రారంభం > MSEC_చొప్పున_SEC &&
	    cmpxchg(&pgdat->nbp_rl_ప్రారంభం, ప్రారంభం, now) == ప్రారంభం)
		pgdat->nbp_rl_nr_cand = nr_cand;
	ఒకవేళ (nr_cand - pgdat->nbp_rl_nr_cand >= రేటు_హద్దు)
		తిరిగి true;
	తిరిగి false;
}

#నిర్వచించు NUMA_MIGRATION_ADJUST_అడుగులు	16

స్థిర శూన్య numa_promotion_adjust_పరిమితి(నిర్మాణం pglist_దత్తాంశం *pgdat,
					    గుర్తులేని దీర్ఘ రేటు_హద్దు,
					    గుర్తులేని పూర్ణాంకం ref_th)
{
	గుర్తులేని పూర్ణాంకం now, ప్రారంభం, th_period, unit_th, th;
	గుర్తులేని దీర్ఘ nr_cand, ref_cand, diff_cand;

	now = jiffies_గమ్యం_msecs(jiffies);
	th_period = sysctl_numa_balancing_పరిశీలన_period_max;
	ప్రారంభం = pgdat->nbp_th_ప్రారంభం;
	ఒకవేళ (now - ప్రారంభం > th_period &&
	    cmpxchg(&pgdat->nbp_th_ప్రారంభం, ప్రారంభం, now) == ప్రారంభం) {
		ref_cand = రేటు_హద్దు *
			sysctl_numa_balancing_పరిశీలన_period_max / MSEC_చొప్పున_SEC;
		nr_cand = కణుపు_పుట_state(pgdat, PGPROMOTE_CANDIDATE);
		diff_cand = nr_cand - pgdat->nbp_th_nr_cand;
		unit_th = ref_th * 2 / NUMA_MIGRATION_ADJUST_అడుగులు;
		th = pgdat->nbp_పరిమితి ? : ref_th;
		ఒకవేళ (diff_cand > ref_cand * 11 / 10)
			th = max(th - unit_th, unit_th);
		లేకపోతే ఒకవేళ (diff_cand < ref_cand * 9 / 10)
			th = min(th + unit_th, ref_th * 2);
		pgdat->nbp_th_nr_cand = nr_cand;
		pgdat->nbp_పరిమితి = th;
	}
}

bool should_numa_migrate_జ్ఞాపకం(నిర్మాణం task_struct *p, నిర్మాణం folio *folio,
				పూర్ణాంకం src_nid, పూర్ణాంకం dst_cpu)
{
	నిర్మాణం numa_సమూహం *ng = deref_curr_numa_సమూహం(p);
	పూర్ణాంకం dst_nid = cpu_గమ్యం_కణుపు(dst_cpu);
	పూర్ణాంకం చివరి_cpupid, this_cpupid;

	/*
	 * Cannot migrate to memoryless nodes.
	 */
	ఒకవేళ (!కణుపు_state(dst_nid, N_జ్ఞాపకం))
		తిరిగి false;

	/*
	 * The pages in slow memory node should be migrated according
	 * to hot/cold instead of private/shared.
	 */
	ఒకవేళ (folio_use_access_సమయం(folio)) {
		నిర్మాణం pglist_దత్తాంశం *pgdat;
		గుర్తులేని దీర్ఘ రేటు_హద్దు;
		గుర్తులేని పూర్ణాంకం latency, th, def_th;
		దీర్ఘ nr = folio_nr_పుటలు(folio);

		pgdat = కణుపు_దత్తాంశం(dst_nid);
		ఒకవేళ (pgdat_free_space_enough(pgdat)) {
			/* workload changed, reset hot threshold */
			pgdat->nbp_పరిమితి = 0;
			mod_కణుపు_పుట_state(pgdat, PGPROMOTE_CANDIDATE_NRL, nr);
			తిరిగి true;
		}

		def_th = sysctl_numa_balancing_hot_పరిమితి;
		రేటు_హద్దు = MB_గమ్యం_పుటలు(sysctl_numa_balancing_promote_రేటు_హద్దు);
		numa_promotion_adjust_పరిమితి(pgdat, రేటు_హద్దు, def_th);

		th = pgdat->nbp_పరిమితి ? : def_th;
		latency = numa_hint_fault_latency(folio);
		ఒకవేళ (latency >= th)
			తిరిగి false;

		తిరిగి !numa_promotion_రేటు_హద్దు(pgdat, రేటు_హద్దు, nr);
	}

	this_cpupid = cpu_pid_గమ్యం_cpupid(dst_cpu, current->pid);
	చివరి_cpupid = folio_xchg_చివరి_cpupid(folio, this_cpupid);

	ఒకవేళ (!(sysctl_numa_balancing_బహుళకం & NUMA_BALANCING_జ్ఞాపకం_TIERING) &&
	    !కణుపు_is_toptier(src_nid) && !cpupid_valid(చివరి_cpupid))
		తిరిగి false;

	/*
	 * Allow first faults or private faults to migrate immediately early in
	 * the lifetime of a task. The magic number 4 is based on waiting for
	 * two full passes of the "multi-stage node selection" test that is
	 * executed below.
	 */
	ఒకవేళ ((p->numa_preferred_nid == NUMA_NO_కణుపు || p->numa_పరిశీలన_seq <= 4) &&
	    (cpupid_pid_unset(చివరి_cpupid) || cpupid_సరిపోలిక_pid(p, చివరి_cpupid)))
		తిరిగి true;

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
	ఒకవేళ (!cpupid_pid_unset(చివరి_cpupid) &&
				cpupid_గమ్యం_nid(చివరి_cpupid) != dst_nid)
		తిరిగి false;

	/* Always allow migrate on private faults */
	ఒకవేళ (cpupid_సరిపోలిక_pid(p, చివరి_cpupid))
		తిరిగి true;

	/* A shared fault, but p->numa_group has not been set up yet. */
	ఒకవేళ (!ng)
		తిరిగి true;

	/*
	 * Destination node is much more heavily used than the source
	 * node? Allow migration.
	 */
	ఒకవేళ (సమూహం_దోషాలు_cpu(ng, dst_nid) > సమూహం_దోషాలు_cpu(ng, src_nid) *
					చురుకైన_కణుపు_FRACTION)
		తిరిగి true;

	/*
	 * Distribute memory according to CPU & memory use on each node,
	 * with 3/4 hysteresis to avoid unnecessary memory migrations:
	 *
	 * faults_cpu(dst)   3   faults_cpu(src)
	 * --------------- * - > ---------------
	 * faults_mem(dst)   4   faults_mem(src)
	 */
	తిరిగి సమూహం_దోషాలు_cpu(ng, dst_nid) * సమూహం_దోషాలు(p, src_nid) * 3 >
	       సమూహం_దోషాలు_cpu(ng, src_nid) * సమూహం_దోషాలు(p, dst_nid) * 4;
}

/*
 * 'numa_type' describes the node at the moment of load balancing.
 */
గణన numa_type {
	/* The node has spare capacity that can be used to run more tasks.  */
	కణుపు_has_అదనపు = 0,
	/*
	 * The node is fully used and the tasks don't compete for more CPU
	 * cycles. Nevertheless, some tasks might wait before running.
	 */
	కణుపు_fully_busy,
	/*
	 * The node is overloaded and can't provide expected CPU cycles to all
	 * tasks.
	 */
	కణుపు_overloaded
};

/* Cached statistics for all CPUs within a node */
నిర్మాణం numa_stats {
	గుర్తులేని దీర్ఘ ఎక్కించు;
	గుర్తులేని దీర్ఘ runnable;
	గుర్తులేని దీర్ఘ util;
	/* Total compute capacity of CPUs on a node */
	గుర్తులేని దీర్ఘ compute_సామర్థ్యం;
	గుర్తులేని పూర్ణాంకం nr_running;
	గుర్తులేని పూర్ణాంకం బరువు;
	గణన numa_type కణుపు_type;
	పూర్ణాంకం idle_cpu;
};

నిర్మాణం task_numa_env {
	నిర్మాణం task_struct *p;

	పూర్ణాంకం src_cpu, src_nid;
	పూర్ణాంకం dst_cpu, dst_nid;
	పూర్ణాంకం imb_numa_nr;

	నిర్మాణం numa_stats src_stats, dst_stats;

	పూర్ణాంకం imbalance_pct;
	పూర్ణాంకం dist;

	నిర్మాణం task_struct *ఉత్తమ_task;
	దీర్ఘ ఉత్తమ_imp;
	పూర్ణాంకం ఉత్తమ_cpu;
};

స్థిర గుర్తులేని దీర్ఘ cpu_ఎక్కించు(నిర్మాణం rq *rq);
స్థిర గుర్తులేని దీర్ఘ cpu_runnable(నిర్మాణం rq *rq);

స్థిర అంతఃవరుస గణన
numa_type numa_classify(గుర్తులేని పూర్ణాంకం imbalance_pct,
			 నిర్మాణం numa_stats *ns)
{
	ఒకవేళ ((ns->nr_running > ns->బరువు) &&
	    (((ns->compute_సామర్థ్యం * 100) < (ns->util * imbalance_pct)) ||
	     ((ns->compute_సామర్థ్యం * imbalance_pct) < (ns->runnable * 100))))
		తిరిగి కణుపు_overloaded;

	ఒకవేళ ((ns->nr_running < ns->బరువు) ||
	    (((ns->compute_సామర్థ్యం * 100) > (ns->util * imbalance_pct)) &&
	     ((ns->compute_సామర్థ్యం * imbalance_pct) > (ns->runnable * 100))))
		తిరిగి కణుపు_has_అదనపు;

	తిరిగి కణుపు_fully_busy;
}

/* Forward declarations of select_idle_sibling helpers */
స్థిర అంతఃవరుస bool test_idle_cores(పూర్ణాంకం cpu);
స్థిర అంతఃవరుస పూర్ణాంకం numa_idle_core(పూర్ణాంకం idle_core, పూర్ణాంకం cpu)
{
	ఒకవేళ (!sched_smt_చురుకైన() ||
	    idle_core >= 0 || !test_idle_cores(cpu))
		తిరిగి idle_core;

	/*
	 * Prefer cores instead of packing HT siblings
	 * and triggering future load balancing.
	 */
	ఒకవేళ (is_core_idle(cpu))
		idle_core = cpu;

	తిరిగి idle_core;
}

/*
 * Gather all necessary information to make NUMA balancing placement
 * decisions that are compatible with standard load balancer. This
 * borrows code and logic from update_sg_lb_stats but sharing a
 * common implementation is impractical.
 */
స్థిర శూన్య update_numa_stats(నిర్మాణం task_numa_env *env,
			      నిర్మాణం numa_stats *ns, పూర్ణాంకం nid,
			      bool కనుగొను_idle)
{
	పూర్ణాంకం cpu, idle_core = -1;

	memset(ns, 0, పరిమాణం(*ns));
	ns->idle_cpu = -1;

	rcu_read_తాళం();
	for_each_cpu(cpu, cpumask_యొక్క_కణుపు(nid)) {
		నిర్మాణం rq *rq = cpu_rq(cpu);

		ns->ఎక్కించు += cpu_ఎక్కించు(rq);
		ns->runnable += cpu_runnable(rq);
		ns->util += cpu_util_cfs(cpu);
		ns->nr_running += rq->cfs.h_nr_runnable;
		ns->compute_సామర్థ్యం += సామర్థ్యం_యొక్క(cpu);

		ఒకవేళ (కనుగొను_idle && idle_core < 0 && !rq->nr_running && idle_cpu(cpu)) {
			ఒకవేళ (READ_ONCE(rq->numa_migrate_on) ||
			    !cpumask_test_cpu(cpu, env->p->cpus_ptr))
				కొనసాగు;

			ఒకవేళ (ns->idle_cpu == -1)
				ns->idle_cpu = cpu;

			idle_core = numa_idle_core(idle_core, cpu);
		}
	}
	rcu_read_unlock();

	ns->బరువు = cpumask_బరువు(cpumask_యొక్క_కణుపు(nid));

	ns->కణుపు_type = numa_classify(env->imbalance_pct, ns);

	ఒకవేళ (idle_core >= 0)
		ns->idle_cpu = idle_core;
}

స్థిర శూన్య task_numa_కేటాయించు(నిర్మాణం task_numa_env *env,
			     నిర్మాణం task_struct *p, దీర్ఘ imp)
{
	నిర్మాణం rq *rq = cpu_rq(env->dst_cpu);

	/* Check if run-queue part of active NUMA balance. */
	ఒకవేళ (env->ఉత్తమ_cpu != env->dst_cpu && xchg(&rq->numa_migrate_on, 1)) {
		పూర్ణాంకం cpu;
		పూర్ణాంకం ప్రారంభం = env->dst_cpu;

		/* Find alternative idle CPU. */
		for_each_cpu_wrap(cpu, cpumask_యొక్క_కణుపు(env->dst_nid), ప్రారంభం + 1) {
			ఒకవేళ (cpu == env->ఉత్తమ_cpu || !idle_cpu(cpu) ||
			    !cpumask_test_cpu(cpu, env->p->cpus_ptr)) {
				కొనసాగు;
			}

			env->dst_cpu = cpu;
			rq = cpu_rq(env->dst_cpu);
			ఒకవేళ (!xchg(&rq->numa_migrate_on, 1))
				వెళ్ళు కేటాయించు;
		}

		/* Failed to find an alternative idle CPU */
		తిరిగి;
	}

కేటాయించు:
	/*
	 * Clear previous best_cpu/rq numa-migrate flag, since task now
	 * found a better CPU to move/swap.
	 */
	ఒకవేళ (env->ఉత్తమ_cpu != -1 && env->ఉత్తమ_cpu != env->dst_cpu) {
		rq = cpu_rq(env->ఉత్తమ_cpu);
		WRITE_ONCE(rq->numa_migrate_on, 0);
	}

	ఒకవేళ (env->ఉత్తమ_task)
		put_task_struct(env->ఉత్తమ_task);
	ఒకవేళ (p)
		get_task_struct(p);

	env->ఉత్తమ_task = p;
	env->ఉత్తమ_imp = imp;
	env->ఉత్తమ_cpu = env->dst_cpu;
}

స్థిర bool ఎక్కించు_too_imbalanced(దీర్ఘ src_ఎక్కించు, దీర్ఘ dst_ఎక్కించు,
				నిర్మాణం task_numa_env *env)
{
	దీర్ఘ imb, old_imb;
	దీర్ఘ orig_src_ఎక్కించు, orig_dst_ఎక్కించు;
	దీర్ఘ src_సామర్థ్యం, dst_సామర్థ్యం;

	/*
	 * The load is corrected for the CPU capacity available on each node.
	 *
	 * src_load        dst_load
	 * ------------ vs ---------
	 * src_capacity    dst_capacity
	 */
	src_సామర్థ్యం = env->src_stats.compute_సామర్థ్యం;
	dst_సామర్థ్యం = env->dst_stats.compute_సామర్థ్యం;

	imb = నిరపేక్ష(dst_ఎక్కించు * src_సామర్థ్యం - src_ఎక్కించు * dst_సామర్థ్యం);

	orig_src_ఎక్కించు = env->src_stats.ఎక్కించు;
	orig_dst_ఎక్కించు = env->dst_stats.ఎక్కించు;

	old_imb = నిరపేక్ష(orig_dst_ఎక్కించు * src_సామర్థ్యం - orig_src_ఎక్కించు * dst_సామర్థ్యం);

	/* Would this change make things worse? */
	తిరిగి (imb > old_imb);
}

/*
 * Maximum NUMA importance can be 1998 (2*999);
 * SMALLIMP @ 30 would be close to 1998/64.
 * Used to deter task migration.
 */
#నిర్వచించు SMALLIMP	30

/*
 * This checks if the overall compute and NUMA accesses of the system would
 * be improved if the source tasks was migrated to the target dst_cpu taking
 * into account that it might be best if task running on the dst_cpu should
 * be exchanged with the source task
 */
స్థిర bool task_numa_compare(నిర్మాణం task_numa_env *env,
			      దీర్ఘ taskimp, దీర్ఘ groupimp, bool maymove)
{
	నిర్మాణం numa_సమూహం *cur_ng, *p_ng = deref_curr_numa_సమూహం(env->p);
	నిర్మాణం rq *dst_rq = cpu_rq(env->dst_cpu);
	దీర్ఘ imp = p_ng ? groupimp : taskimp;
	నిర్మాణం task_struct *cur;
	దీర్ఘ src_ఎక్కించు, dst_ఎక్కించు;
	పూర్ణాంకం dist = env->dist;
	దీర్ఘ moveimp = imp;
	దీర్ఘ ఎక్కించు;
	bool stopsearch = false;

	ఒకవేళ (READ_ONCE(dst_rq->numa_migrate_on))
		తిరిగి false;

	rcu_read_తాళం();
	cur = rcu_dereference_all(dst_rq->curr);
	ఒకవేళ (cur && ((cur->flags & (PF_EXITING | PF_KTHREAD)) ||
		    !cur->mm))
		cur = ఖాళీ;

	/*
	 * Because we have preemption enabled we can get migrated around and
	 * end try selecting ourselves (current == env->p) as a swap candidate.
	 */
	ఒకవేళ (cur == env->p) {
		stopsearch = true;
		వెళ్ళు unlock;
	}

	ఒకవేళ (!cur) {
		ఒకవేళ (maymove && moveimp >= env->ఉత్తమ_imp)
			వెళ్ళు కేటాయించు;
		లేకపోతే
			వెళ్ళు unlock;
	}

	/* Skip this swap candidate if cannot move to the source cpu. */
	ఒకవేళ (!cpumask_test_cpu(env->src_cpu, cur->cpus_ptr))
		వెళ్ళు unlock;

	/*
	 * Skip this swap candidate if it is not moving to its preferred
	 * node and the best task is.
	 */
	ఒకవేళ (env->ఉత్తమ_task &&
	    env->ఉత్తమ_task->numa_preferred_nid == env->src_nid &&
	    cur->numa_preferred_nid != env->src_nid) {
		వెళ్ళు unlock;
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
	cur_ng = rcu_dereference_all(cur->numa_సమూహం);
	ఒకవేళ (cur_ng == p_ng) {
		/*
		 * Do not swap within a group or between tasks that have
		 * no group if there is spare capacity. Swapping does
		 * not address the load imbalance and helps one task at
		 * the cost of punishing another.
		 */
		ఒకవేళ (env->dst_stats.కణుపు_type == కణుపు_has_అదనపు)
			వెళ్ళు unlock;

		imp = taskimp + task_బరువు(cur, env->src_nid, dist) -
		      task_బరువు(cur, env->dst_nid, dist);
		/*
		 * Add some hysteresis to prevent swapping the
		 * tasks within a group over tiny differences.
		 */
		ఒకవేళ (cur_ng)
			imp -= imp / 16;
	} లేకపోతే {
		/*
		 * Compare the group weights. If a task is all by itself
		 * (not part of a group), use the task weight instead.
		 */
		ఒకవేళ (cur_ng && p_ng)
			imp += సమూహం_బరువు(cur, env->src_nid, dist) -
			       సమూహం_బరువు(cur, env->dst_nid, dist);
		లేకపోతే
			imp += task_బరువు(cur, env->src_nid, dist) -
			       task_బరువు(cur, env->dst_nid, dist);
	}

	/* Discourage picking a task already on its preferred node */
	ఒకవేళ (cur->numa_preferred_nid == env->dst_nid)
		imp -= imp / 16;

	/*
	 * Encourage picking a task that moves to its preferred node.
	 * This potentially makes imp larger than it's maximum of
	 * 1998 (see SMALLIMP and task_weight for why) but in this
	 * case, it does not matter.
	 */
	ఒకవేళ (cur->numa_preferred_nid == env->src_nid)
		imp += imp / 8;

	ఒకవేళ (maymove && moveimp > imp && moveimp > env->ఉత్తమ_imp) {
		imp = moveimp;
		cur = ఖాళీ;
		వెళ్ళు కేటాయించు;
	}

	/*
	 * Prefer swapping with a task moving to its preferred node over a
	 * task that is not.
	 */
	ఒకవేళ (env->ఉత్తమ_task && cur->numa_preferred_nid == env->src_nid &&
	    env->ఉత్తమ_task->numa_preferred_nid != env->src_nid) {
		వెళ్ళు కేటాయించు;
	}

	/*
	 * If the NUMA importance is less than SMALLIMP,
	 * task migration might only result in ping pong
	 * of tasks and also hurt performance due to cache
	 * misses.
	 */
	ఒకవేళ (imp < SMALLIMP || imp <= env->ఉత్తమ_imp + SMALLIMP / 2)
		వెళ్ళు unlock;

	/*
	 * In the overloaded case, try and keep the load balanced.
	 */
	ఎక్కించు = task_h_ఎక్కించు(env->p) - task_h_ఎక్కించు(cur);
	ఒకవేళ (!ఎక్కించు)
		వెళ్ళు కేటాయించు;

	dst_ఎక్కించు = env->dst_stats.ఎక్కించు + ఎక్కించు;
	src_ఎక్కించు = env->src_stats.ఎక్కించు - ఎక్కించు;

	ఒకవేళ (ఎక్కించు_too_imbalanced(src_ఎక్కించు, dst_ఎక్కించు, env))
		వెళ్ళు unlock;

కేటాయించు:
	/* Evaluate an idle CPU for a task numa move. */
	ఒకవేళ (!cur) {
		పూర్ణాంకం cpu = env->dst_stats.idle_cpu;

		/* Nothing cached so current CPU went idle since the search. */
		ఒకవేళ (cpu < 0)
			cpu = env->dst_cpu;

		/*
		 * If the CPU is no longer truly idle and the previous best CPU
		 * is, keep using it.
		 */
		ఒకవేళ (!idle_cpu(cpu) && env->ఉత్తమ_cpu >= 0 &&
		    idle_cpu(env->ఉత్తమ_cpu)) {
			cpu = env->ఉత్తమ_cpu;
		}

		env->dst_cpu = cpu;
	}

	task_numa_కేటాయించు(env, cur, imp);

	/*
	 * If a move to idle is allowed because there is capacity or load
	 * balance improves then stop the search. While a better swap
	 * candidate may exist, a search is not free.
	 */
	ఒకవేళ (maymove && !cur && env->ఉత్తమ_cpu >= 0 && idle_cpu(env->ఉత్తమ_cpu))
		stopsearch = true;

	/*
	 * If a swap candidate must be identified and the current best task
	 * moves its preferred node then stop the search.
	 */
	ఒకవేళ (!maymove && env->ఉత్తమ_task &&
	    env->ఉత్తమ_task->numa_preferred_nid == env->src_nid) {
		stopsearch = true;
	}
unlock:
	rcu_read_unlock();

	తిరిగి stopsearch;
}

స్థిర శూన్య task_numa_కనుగొను_cpu(నిర్మాణం task_numa_env *env,
				దీర్ఘ taskimp, దీర్ఘ groupimp)
{
	bool maymove = false;
	పూర్ణాంకం cpu;

	/*
	 * If dst node has spare capacity, then check if there is an
	 * imbalance that would be overruled by the load balancer.
	 */
	ఒకవేళ (env->dst_stats.కణుపు_type == కణుపు_has_అదనపు) {
		గుర్తులేని పూర్ణాంకం imbalance;
		పూర్ణాంకం src_running, dst_running;

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
		ఒకవేళ (!imbalance) {
			maymove = true;
			ఒకవేళ (env->dst_stats.idle_cpu >= 0) {
				env->dst_cpu = env->dst_stats.idle_cpu;
				task_numa_కేటాయించు(env, ఖాళీ, 0);
				తిరిగి;
			}
		}
	} లేకపోతే {
		దీర్ఘ src_ఎక్కించు, dst_ఎక్కించు, ఎక్కించు;
		/*
		 * If the improvement from just moving env->p direction is better
		 * than swapping tasks around, check if a move is possible.
		 */
		ఎక్కించు = task_h_ఎక్కించు(env->p);
		dst_ఎక్కించు = env->dst_stats.ఎక్కించు + ఎక్కించు;
		src_ఎక్కించు = env->src_stats.ఎక్కించు - ఎక్కించు;
		maymove = !ఎక్కించు_too_imbalanced(src_ఎక్కించు, dst_ఎక్కించు, env);
	}

	/* Skip CPUs if the source task cannot migrate */
	for_each_cpu_and(cpu, cpumask_యొక్క_కణుపు(env->dst_nid), env->p->cpus_ptr) {
		env->dst_cpu = cpu;
		ఒకవేళ (task_numa_compare(env, taskimp, groupimp, maymove))
			ఆపు;
	}
}

స్థిర పూర్ణాంకం task_numa_migrate(నిర్మాణం task_struct *p)
{
	నిర్మాణం task_numa_env env = {
		.p = p,

		.src_cpu = task_cpu(p),
		.src_nid = task_కణుపు(p),

		.imbalance_pct = 112,

		.ఉత్తమ_task = ఖాళీ,
		.ఉత్తమ_imp = 0,
		.ఉత్తమ_cpu = -1,
	};
	గుర్తులేని దీర్ఘ taskweight, groupweight;
	నిర్మాణం sched_domain *sd;
	దీర్ఘ taskimp, groupimp;
	నిర్మాణం numa_సమూహం *ng;
	నిర్మాణం rq *ఉత్తమ_rq;
	పూర్ణాంకం nid, ret, dist;

	/*
	 * Pick the lowest SD_NUMA domain, as that would have the smallest
	 * imbalance and would be the first to start moving tasks about.
	 *
	 * And we want to avoid any moving of tasks about, as that would create
	 * random movement of tasks -- counter the numa conditions we're trying
	 * to satisfy here.
	 */
	rcu_read_తాళం();
	sd = rcu_dereference_all(చొప్పున_cpu(sd_numa, env.src_cpu));
	ఒకవేళ (sd) {
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
	ఒకవేళ (unlikely(!sd)) {
		sched_setnuma(p, task_కణుపు(p));
		తిరిగి -EINVAL;
	}

	env.dst_nid = p->numa_preferred_nid;
	dist = env.dist = కణుపు_దూరం(env.src_nid, env.dst_nid);
	taskweight = task_బరువు(p, env.src_nid, dist);
	groupweight = సమూహం_బరువు(p, env.src_nid, dist);
	update_numa_stats(&env, &env.src_stats, env.src_nid, false);
	taskimp = task_బరువు(p, env.dst_nid, dist) - taskweight;
	groupimp = సమూహం_బరువు(p, env.dst_nid, dist) - groupweight;
	update_numa_stats(&env, &env.dst_stats, env.dst_nid, true);

	/* Try to find a spot on the preferred nid. */
	task_numa_కనుగొను_cpu(&env, taskimp, groupimp);

	/*
	 * Look at other nodes in these cases:
	 * - there is no space available on the preferred_nid
	 * - the task is part of a numa_group that is interleaved across
	 *   multiple NUMA nodes; in order to better consolidate the group,
	 *   we need to check other locations.
	 */
	ng = deref_curr_numa_సమూహం(p);
	ఒకవేళ (env.ఉత్తమ_cpu == -1 || (ng && ng->చురుకైన_nodes > 1)) {
		for_each_కణుపు_state(nid, N_CPU) {
			ఒకవేళ (nid == env.src_nid || nid == p->numa_preferred_nid)
				కొనసాగు;

			dist = కణుపు_దూరం(env.src_nid, env.dst_nid);
			ఒకవేళ (sched_numa_topology_type == NUMA_BACKPLANE &&
						dist != env.dist) {
				taskweight = task_బరువు(p, env.src_nid, dist);
				groupweight = సమూహం_బరువు(p, env.src_nid, dist);
			}

			/* Only consider nodes where both task and groups benefit */
			taskimp = task_బరువు(p, nid, dist) - taskweight;
			groupimp = సమూహం_బరువు(p, nid, dist) - groupweight;
			ఒకవేళ (taskimp < 0 && groupimp < 0)
				కొనసాగు;

			env.dist = dist;
			env.dst_nid = nid;
			update_numa_stats(&env, &env.dst_stats, env.dst_nid, true);
			task_numa_కనుగొను_cpu(&env, taskimp, groupimp);
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
	ఒకవేళ (ng) {
		ఒకవేళ (env.ఉత్తమ_cpu == -1)
			nid = env.src_nid;
		లేకపోతే
			nid = cpu_గమ్యం_కణుపు(env.ఉత్తమ_cpu);

		ఒకవేళ (nid != p->numa_preferred_nid)
			sched_setnuma(p, nid);
	}

	/* No better CPU than the current one was found. */
	ఒకవేళ (env.ఉత్తమ_cpu == -1) {
		trace_sched_stick_numa(p, env.src_cpu, ఖాళీ, -1);
		తిరిగి -EAGAIN;
	}

	ఉత్తమ_rq = cpu_rq(env.ఉత్తమ_cpu);
	ఒకవేళ (env.ఉత్తమ_task == ఖాళీ) {
		ret = migrate_task_గమ్యం(p, env.ఉత్తమ_cpu);
		WRITE_ONCE(ఉత్తమ_rq->numa_migrate_on, 0);
		ఒకవేళ (ret != 0)
			trace_sched_stick_numa(p, env.src_cpu, ఖాళీ, env.ఉత్తమ_cpu);
		తిరిగి ret;
	}

	ret = migrate_మార్పిడి(p, env.ఉత్తమ_task, env.ఉత్తమ_cpu, env.src_cpu);
	WRITE_ONCE(ఉత్తమ_rq->numa_migrate_on, 0);

	ఒకవేళ (ret != 0)
		trace_sched_stick_numa(p, env.src_cpu, env.ఉత్తమ_task, env.ఉత్తమ_cpu);
	put_task_struct(env.ఉత్తమ_task);
	తిరిగి ret;
}

/* Attempt to migrate a task to a CPU on the preferred node. */
స్థిర శూన్య numa_migrate_preferred(నిర్మాణం task_struct *p)
{
	గుర్తులేని దీర్ఘ interval = HZ;

	/* This task has no NUMA fault statistics yet */
	ఒకవేళ (unlikely(p->numa_preferred_nid == NUMA_NO_కణుపు || !p->numa_దోషాలు))
		తిరిగి;

	/* Periodically retry migrating the task to the preferred node */
	interval = min(interval, msecs_గమ్యం_jiffies(p->numa_పరిశీలన_period) / 16);
	p->numa_migrate_retry = jiffies + interval;

	/* Success if task is already running on preferred CPU */
	ఒకవేళ (task_కణుపు(p) == p->numa_preferred_nid)
		తిరిగి;

	/* Otherwise, try migrate to a CPU on the preferred node */
	task_numa_migrate(p);
}

/*
 * Find out how many nodes the workload is actively running on. Do this by
 * tracking the nodes from which NUMA hinting faults are triggered. This can
 * be different from the set of nodes where the workload's memory is currently
 * located.
 */
స్థిర శూన్య numa_సమూహం_లెక్క_చురుకైన_nodes(నిర్మాణం numa_సమూహం *numa_సమూహం)
{
	గుర్తులేని దీర్ఘ దోషాలు, max_దోషాలు = 0;
	పూర్ణాంకం nid, చురుకైన_nodes = 0;

	for_each_కణుపు_state(nid, N_CPU) {
		దోషాలు = సమూహం_దోషాలు_cpu(numa_సమూహం, nid);
		ఒకవేళ (దోషాలు > max_దోషాలు)
			max_దోషాలు = దోషాలు;
	}

	for_each_కణుపు_state(nid, N_CPU) {
		దోషాలు = సమూహం_దోషాలు_cpu(numa_సమూహం, nid);
		ఒకవేళ (దోషాలు * చురుకైన_కణుపు_FRACTION > max_దోషాలు)
			చురుకైన_nodes++;
	}

	numa_సమూహం->max_దోషాలు_cpu = max_దోషాలు;
	numa_సమూహం->చురుకైన_nodes = చురుకైన_nodes;
}

/*
 * When adapting the scan rate, the period is divided into NUMA_PERIOD_SLOTS
 * increments. The more local the fault statistics are, the higher the scan
 * period will be for the next scan window. If local/(local+remote) ratio is
 * below NUMA_PERIOD_THRESHOLD (where range of ratio is 1..NUMA_PERIOD_SLOTS)
 * the scan period will decrease. Aim for 70% local accesses.
 */
#నిర్వచించు NUMA_PERIOD_SLOTS 10
#నిర్వచించు NUMA_PERIOD_పరిమితి 7

/*
 * Increase the scan period (slow down scanning) if the majority of
 * our memory is already on our local node, or if the majority of
 * the page accesses are shared with other processes.
 * Otherwise, decrease the scan period.
 */
స్థిర శూన్య update_task_పరిశీలన_period(నిర్మాణం task_struct *p,
			గుర్తులేని దీర్ఘ shared, గుర్తులేని దీర్ఘ అంతరంగ)
{
	గుర్తులేని పూర్ణాంకం period_స్థానం;
	పూర్ణాంకం lr_నిష్పత్తి, ps_నిష్పత్తి;
	పూర్ణాంకం diff;

	గుర్తులేని దీర్ఘ remote = p->numa_దోషాలు_locality[0];
	గుర్తులేని దీర్ఘ local = p->numa_దోషాలు_locality[1];

	/*
	 * If there were no record hinting faults then either the task is
	 * completely idle or all activity is in areas that are not of interest
	 * to automatic numa balancing. Related to that, if there were failed
	 * migration then it implies we are migrating too quickly or the local
	 * node is overloaded. In either case, scan slower
	 */
	ఒకవేళ (local + shared == 0 || p->numa_దోషాలు_locality[2]) {
		p->numa_పరిశీలన_period = min(p->numa_పరిశీలన_period_max,
			p->numa_పరిశీలన_period << 1);

		p->mm->numa_తదుపరి_పరిశీలన = jiffies +
			msecs_గమ్యం_jiffies(p->numa_పరిశీలన_period);

		వెళ్ళు బహిర్గతం;
	}

	/*
	 * Prepare to scale scan period relative to the current period.
	 *	 == NUMA_PERIOD_THRESHOLD scan period stays the same
	 *       <  NUMA_PERIOD_THRESHOLD scan period decreases (scan faster)
	 *	 >= NUMA_PERIOD_THRESHOLD scan period increases (scan slower)
	 */
	period_స్థానం = DIV_విడత_పైకి(p->numa_పరిశీలన_period, NUMA_PERIOD_SLOTS);
	lr_నిష్పత్తి = (local * NUMA_PERIOD_SLOTS) / (local + remote);
	ps_నిష్పత్తి = (అంతరంగ * NUMA_PERIOD_SLOTS) / (అంతరంగ + shared);

	ఒకవేళ (ps_నిష్పత్తి >= NUMA_PERIOD_పరిమితి) {
		/*
		 * Most memory accesses are local. There is no need to
		 * do fast NUMA scanning, since memory is already local.
		 */
		పూర్ణాంకం స్థానం = ps_నిష్పత్తి - NUMA_PERIOD_పరిమితి;
		ఒకవేళ (!స్థానం)
			స్థానం = 1;
		diff = స్థానం * period_స్థానం;
	} లేకపోతే ఒకవేళ (lr_నిష్పత్తి >= NUMA_PERIOD_పరిమితి) {
		/*
		 * Most memory accesses are shared with other tasks.
		 * There is no point in continuing fast NUMA scanning,
		 * since other tasks may just move the memory elsewhere.
		 */
		పూర్ణాంకం స్థానం = lr_నిష్పత్తి - NUMA_PERIOD_పరిమితి;
		ఒకవేళ (!స్థానం)
			స్థానం = 1;
		diff = స్థానం * period_స్థానం;
	} లేకపోతే {
		/*
		 * Private memory faults exceed (SLOTS-THRESHOLD)/SLOTS,
		 * yet they are not on the local NUMA node. Speed up
		 * NUMA scanning to get the memory moved over.
		 */
		పూర్ణాంకం నిష్పత్తి = max(lr_నిష్పత్తి, ps_నిష్పత్తి);
		diff = -(NUMA_PERIOD_పరిమితి - నిష్పత్తి) * period_స్థానం;
	}

	p->numa_పరిశీలన_period = clamp(p->numa_పరిశీలన_period + diff,
			task_పరిశీలన_min(p), task_పరిశీలన_max(p));

బహిర్గతం:
	memset(p->numa_దోషాలు_locality, 0,
	       పరిమాణం(p->numa_దోషాలు_locality));
}

/*
 * Get the fraction of time the task has been running since the last
 * NUMA placement cycle. The scheduler keeps similar statistics, but
 * decays those on a 32ms period, which is orders of magnitude off
 * from the dozens-of-seconds NUMA balancing period. Use the scheduler
 * stats only if the task is so new there are no NUMA statistics yet.
 */
స్థిర u64 numa_get_avg_runtime(నిర్మాణం task_struct *p, u64 *period)
{
	u64 runtime, delta, now;
	/* Use the start of this time slice to avoid calculations. */
	now = p->se.exec_ప్రారంభం;
	runtime = p->se.కూడిక_exec_runtime;

	ఒకవేళ (p->చివరి_task_numa_placement) {
		delta = runtime - p->చివరి_కూడిక_exec_runtime;
		*period = now - p->చివరి_task_numa_placement;

		/* Avoid time going backwards, prevent potential divide error: */
		ఒకవేళ (unlikely((s64)*period < 0))
			*period = 0;
	} లేకపోతే {
		delta = p->se.avg.ఎక్కించు_కూడిక;
		*period = ఎక్కించు_AVG_MAX;
	}

	p->చివరి_కూడిక_exec_runtime = runtime;
	p->చివరి_task_numa_placement = now;

	తిరిగి delta;
}

/*
 * Determine the preferred nid for a task in a numa_group. This needs to
 * be done in a way that produces consistent results with group_weight,
 * otherwise workloads might not converge.
 */
స్థిర పూర్ణాంకం preferred_సమూహం_nid(నిర్మాణం task_struct *p, పూర్ణాంకం nid)
{
	nodemask_t nodes;
	పూర్ణాంకం dist;

	/* Direct connections between all NUMA nodes. */
	ఒకవేళ (sched_numa_topology_type == NUMA_ప్రత్యక్ష)
		తిరిగి nid;

	/*
	 * On a system with glueless mesh NUMA topology, group_weight
	 * scores nodes according to the number of NUMA hinting faults on
	 * both the node itself, and on nearby nodes.
	 */
	ఒకవేళ (sched_numa_topology_type == NUMA_GLUELESS_MESH) {
		గుర్తులేని దీర్ఘ score, max_score = 0;
		పూర్ణాంకం కణుపు, max_కణుపు = nid;

		dist = sched_max_numa_దూరం;

		for_each_కణుపు_state(కణుపు, N_CPU) {
			score = సమూహం_బరువు(p, కణుపు, dist);
			ఒకవేళ (score > max_score) {
				max_score = score;
				max_కణుపు = కణుపు;
			}
		}
		తిరిగి max_కణుపు;
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
	nodes = కణుపు_స్థితులు[N_CPU];
	కొరకు (dist = sched_max_numa_దూరం; dist > LOCAL_దూరం; dist--) {
		గుర్తులేని దీర్ఘ max_దోషాలు = 0;
		nodemask_t max_సమూహం = కణుపు_ముసుగు_NONE;
		పూర్ణాంకం a, b;

		/* Are there nodes at this distance from each other? */
		ఒకవేళ (!కనుగొను_numa_దూరం(dist))
			కొనసాగు;

		for_each_కణుపు_ముసుగు(a, nodes) {
			గుర్తులేని దీర్ఘ దోషాలు = 0;
			nodemask_t this_సమూహం;
			nodes_clear(this_సమూహం);

			/* Sum group's NUMA faults; includes a==b case. */
			for_each_కణుపు_ముసుగు(b, nodes) {
				ఒకవేళ (కణుపు_దూరం(a, b) < dist) {
					దోషాలు += సమూహం_దోషాలు(p, b);
					కణుపు_సమితి(b, this_సమూహం);
					కణుపు_clear(b, nodes);
				}
			}

			/* Remember the top group. */
			ఒకవేళ (దోషాలు > max_దోషాలు) {
				max_దోషాలు = దోషాలు;
				max_సమూహం = this_సమూహం;
				/*
				 * subtle: at the smallest distance there is
				 * just one node left in each "group", the
				 * winner is the preferred nid.
				 */
				nid = a;
			}
		}
		/* Next round, evaluate the nodes within max_group. */
		ఒకవేళ (!max_దోషాలు)
			ఆపు;
		nodes = max_సమూహం;
	}
	తిరిగి nid;
}

స్థిర శూన్య task_numa_placement(నిర్మాణం task_struct *p)
	__context_unsafe(/* conditional locking */)
{
	నిర్మాణం sched_cache_సమూహం __maybe_unused *grp;
	పూర్ణాంకం seq, nid, max_nid = NUMA_NO_కణుపు;
	గుర్తులేని దీర్ఘ max_దోషాలు = 0;
	గుర్తులేని దీర్ఘ fault_types[2] = { 0, 0 };
	గుర్తులేని దీర్ఘ మొత్తం_దోషాలు;
	u64 runtime, period;
	spinlock_t *సమూహం_తాళం = ఖాళీ;
	దీర్ఘ __maybe_unused new_fp;
	నిర్మాణం numa_సమూహం *ng;

	/*
	 * The p->mm->numa_scan_seq field gets updated without
	 * exclusive access. Use READ_ONCE() here to ensure
	 * that the field is read in a single access:
	 */
	seq = READ_ONCE(p->mm->numa_పరిశీలన_seq);
	ఒకవేళ (p->numa_పరిశీలన_seq == seq)
		తిరిగి;
	p->numa_పరిశీలన_seq = seq;
	p->numa_పరిశీలన_period_max = task_పరిశీలన_max(p);

	మొత్తం_దోషాలు = p->numa_దోషాలు_locality[0] +
		       p->numa_దోషాలు_locality[1];
	runtime = numa_get_avg_runtime(p, &period);

	/* If the task is part of a group prevent parallel updates to group stats */
	ng = deref_curr_numa_సమూహం(p);
	ఒకవేళ (ng) {
		సమూహం_తాళం = &ng->తాళం;
		spin_తాళం_irq(సమూహం_తాళం);
	}

	/* Find the node with the highest number of faults */
	for_each_online_కణుపు(nid) {
		/* Keep track of the offsets in numa_faults array */
		పూర్ణాంకం mem_idx, membuf_idx, cpu_idx, cpubuf_idx;
		గుర్తులేని దీర్ఘ దోషాలు = 0, సమూహం_దోషాలు = 0;
		పూర్ణాంకం priv;

		కొరకు (priv = 0; priv < NR_NUMA_HINT_FAULT_TYPES; priv++) {
			దీర్ఘ diff, f_diff, f_బరువు;

			mem_idx = task_దోషాలు_idx(NUMA_MEM, nid, priv);
			membuf_idx = task_దోషాలు_idx(NUMA_MEMBUF, nid, priv);
			cpu_idx = task_దోషాలు_idx(NUMA_CPU, nid, priv);
			cpubuf_idx = task_దోషాలు_idx(NUMA_CPUBUF, nid, priv);

			/* Decay existing window, copy faults since last scan */
			diff = p->numa_దోషాలు[membuf_idx] - p->numa_దోషాలు[mem_idx] / 2;
			fault_types[priv] += p->numa_దోషాలు[membuf_idx];
			p->numa_దోషాలు[membuf_idx] = 0;

			/*
			 * Normalize the faults_from, so all tasks in a group
			 * count according to CPU use, instead of by the raw
			 * number of faults. Tasks with little runtime have
			 * little over-all impact on throughput, and thus their
			 * faults are less important.
			 */
			f_బరువు = div64_u64(runtime << 16, period + 1);
			f_బరువు = (f_బరువు * p->numa_దోషాలు[cpubuf_idx]) /
				   (మొత్తం_దోషాలు + 1);
			f_diff = f_బరువు - p->numa_దోషాలు[cpu_idx] / 2;
			p->numa_దోషాలు[cpubuf_idx] = 0;

			p->numa_దోషాలు[mem_idx] += diff;
			p->numa_దోషాలు[cpu_idx] += f_diff;
			దోషాలు += p->numa_దోషాలు[mem_idx];
			p->మొత్తం_numa_దోషాలు += diff;
			ఒకవేళ (ng) {
				/*
				 * safe because we can only change our own group
				 *
				 * mem_idx represents the offset for a given
				 * nid and priv in a specific region because it
				 * is at the beginning of the numa_faults array.
				 */
				ng->దోషాలు[mem_idx] += diff;
				ng->దోషాలు[cpu_idx] += f_diff;
				ng->మొత్తం_దోషాలు += diff;
				సమూహం_దోషాలు += ng->దోషాలు[mem_idx];
			}
#నిర్వచితమైతే CONFIG_SCHED_CACHE
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

				ఒకవేళ (grp) {
					new_fp = (దీర్ఘ)READ_ONCE(grp->footprint) + diff;
					WRITE_ONCE(grp->footprint, max(new_fp, 0L));
				}
			}
#అయితే_ముగింపు
		}

		ఒకవేళ (!ng) {
			ఒకవేళ (దోషాలు > max_దోషాలు) {
				max_దోషాలు = దోషాలు;
				max_nid = nid;
			}
		} లేకపోతే ఒకవేళ (సమూహం_దోషాలు > max_దోషాలు) {
			max_దోషాలు = సమూహం_దోషాలు;
			max_nid = nid;
		}
	}

	/* Cannot migrate task to CPU-less node */
	max_nid = numa_సమీప_కణుపు(max_nid, N_CPU);

	ఒకవేళ (ng) {
		numa_సమూహం_లెక్క_చురుకైన_nodes(ng);
		spin_unlock_irq(సమూహం_తాళం);
		max_nid = preferred_సమూహం_nid(p, max_nid);
	}

	ఒకవేళ (max_దోషాలు) {
		/* Set the new preferred node */
		ఒకవేళ (max_nid != p->numa_preferred_nid)
			sched_setnuma(p, max_nid);
	}

	update_task_పరిశీలన_period(p, fault_types[0], fault_types[1]);
}

స్థిర అంతఃవరుస పూర్ణాంకం get_numa_సమూహం(నిర్మాణం numa_సమూహం *grp)
{
	తిరిగి refcount_inc_not_zero(&grp->refcount);
}

స్థిర అంతఃవరుస శూన్య put_numa_సమూహం(నిర్మాణం numa_సమూహం *grp)
{
	ఒకవేళ (refcount_dec_and_test(&grp->refcount))
		kfree_rcu(grp, rcu);
}

స్థిర శూన్య task_numa_సమూహం(నిర్మాణం task_struct *p, పూర్ణాంకం cpupid, పూర్ణాంకం flags,
			పూర్ణాంకం *priv)
{
	నిర్మాణం numa_సమూహం *grp, *my_grp;
	నిర్మాణం task_struct *tsk;
	bool join = false;
	పూర్ణాంకం cpu = cpupid_గమ్యం_cpu(cpupid);
	పూర్ణాంకం i;

	ఒకవేళ (unlikely(!deref_curr_numa_సమూహం(p))) {
		గుర్తులేని పూర్ణాంకం కొలత = పరిమాణం(నిర్మాణం numa_సమూహం) +
				    NR_NUMA_HINT_FAULT_STATS *
				    nr_కణుపు_ids * పరిమాణం(గుర్తులేని దీర్ఘ);

		grp = kzalloc(కొలత, GFP_KERNEL | __GFP_NOWARN);
		ఒకవేళ (!grp)
			తిరిగి;

		refcount_సమితి(&grp->refcount, 1);
		grp->చురుకైన_nodes = 1;
		grp->max_దోషాలు_cpu = 0;
		spin_తాళం_init(&grp->తాళం);
		grp->gid = p->pid;

		కొరకు (i = 0; i < NR_NUMA_HINT_FAULT_STATS * nr_కణుపు_ids; i++)
			grp->దోషాలు[i] = p->numa_దోషాలు[i];

		grp->మొత్తం_దోషాలు = p->మొత్తం_numa_దోషాలు;

		grp->nr_tasks++;
		rcu_కేటాయించు_సూచిక(p->numa_సమూహం, grp);
	}

	rcu_read_తాళం();
	tsk = READ_ONCE(cpu_rq(cpu)->curr);

	ఒకవేళ (!cpupid_సరిపోలిక_pid(tsk, cpupid))
		వెళ్ళు no_join;

	grp = rcu_dereference_all(tsk->numa_సమూహం);
	ఒకవేళ (!grp)
		వెళ్ళు no_join;

	my_grp = deref_curr_numa_సమూహం(p);
	ఒకవేళ (grp == my_grp)
		వెళ్ళు no_join;

	/*
	 * Only join the other group if its bigger; if we're the bigger group,
	 * the other task will join us.
	 */
	ఒకవేళ (my_grp->nr_tasks > grp->nr_tasks)
		వెళ్ళు no_join;

	/*
	 * Tie-break on the grp address.
	 */
	ఒకవేళ (my_grp->nr_tasks == grp->nr_tasks && my_grp > grp)
		వెళ్ళు no_join;

	/* Always join threads in the same process. */
	ఒకవేళ (tsk->mm == current->mm)
		join = true;

	/* Simple filter to avoid false positives due to PID collisions */
	ఒకవేళ (flags & TNF_SHARED)
		join = true;

	/* Update priv based on whether false sharing was detected */
	*priv = !join;

	ఒకవేళ (join && !get_numa_సమూహం(grp))
		వెళ్ళు no_join;

	rcu_read_unlock();

	ఒకవేళ (!join)
		తిరిగి;

	WARN_ON_ONCE(irqs_disabled());
	double_తాళం_irq(&my_grp->తాళం, &grp->తాళం);

	కొరకు (i = 0; i < NR_NUMA_HINT_FAULT_STATS * nr_కణుపు_ids; i++) {
		my_grp->దోషాలు[i] -= p->numa_దోషాలు[i];
		grp->దోషాలు[i] += p->numa_దోషాలు[i];
	}
	my_grp->మొత్తం_దోషాలు -= p->మొత్తం_numa_దోషాలు;
	grp->మొత్తం_దోషాలు += p->మొత్తం_numa_దోషాలు;

	my_grp->nr_tasks--;
	grp->nr_tasks++;

	spin_unlock(&my_grp->తాళం);
	spin_unlock_irq(&grp->తాళం);

	rcu_కేటాయించు_సూచిక(p->numa_సమూహం, grp);

	put_numa_సమూహం(my_grp);
	తిరిగి;

no_join:
	rcu_read_unlock();
	తిరిగి;
}

/*
 * Get rid of NUMA statistics associated with a task (either current or dead).
 * If @final is set, the task is dead and has reached refcount zero, so we can
 * safely free all relevant data structures. Otherwise, there might be
 * concurrent reads from places like load balancing and procfs, and we should
 * reset the data back to default state without freeing ->numa_faults.
 */
శూన్య task_numa_free(నిర్మాణం task_struct *p, bool final)
{
	/* safe: p either is current or is being freed by current */
	నిర్మాణం numa_సమూహం *grp = rcu_dereference_raw(p->numa_సమూహం);
	గుర్తులేని దీర్ఘ *numa_దోషాలు = p->numa_దోషాలు;
	గుర్తులేని దీర్ఘ flags;
	పూర్ణాంకం i;

	ఒకవేళ (!numa_దోషాలు)
		తిరిగి;

	ఒకవేళ (grp) {
		spin_తాళం_irqsave(&grp->తాళం, flags);
		కొరకు (i = 0; i < NR_NUMA_HINT_FAULT_STATS * nr_కణుపు_ids; i++)
			grp->దోషాలు[i] -= p->numa_దోషాలు[i];
		grp->మొత్తం_దోషాలు -= p->మొత్తం_numa_దోషాలు;

		grp->nr_tasks--;
		spin_unlock_irqrestore(&grp->తాళం, flags);
		RCU_INIT_సూచిక(p->numa_సమూహం, ఖాళీ);
		put_numa_సమూహం(grp);
	}

	ఒకవేళ (final) {
		p->numa_దోషాలు = ఖాళీ;
		kfree(numa_దోషాలు);
	} లేకపోతే {
		p->మొత్తం_numa_దోషాలు = 0;
		కొరకు (i = 0; i < NR_NUMA_HINT_FAULT_STATS * nr_కణుపు_ids; i++)
			numa_దోషాలు[i] = 0;
	}
}

/*
 * Got a PROT_NONE fault for a page on @node.
 */
శూన్య task_numa_fault(పూర్ణాంకం చివరి_cpupid, పూర్ణాంకం mem_కణుపు, పూర్ణాంకం పుటలు, పూర్ణాంకం flags)
{
	నిర్మాణం task_struct *p = current;
	bool migrated = flags & TNF_MIGRATED;
	పూర్ణాంకం cpu_కణుపు = task_కణుపు(current);
	పూర్ణాంకం local = !!(flags & TNF_FAULT_LOCAL);
	నిర్మాణం numa_సమూహం *ng;
	పూర్ణాంకం priv;

	ఒకవేళ (!static_branch_likely(&sched_numa_balancing))
		తిరిగి;

	/* for example, ksmd faulting in a user's mm */
	ఒకవేళ (!p->mm)
		తిరిగి;

	/*
	 * NUMA faults statistics are unnecessary for the slow memory
	 * node for memory tiering mode.
	 */
	ఒకవేళ (!కణుపు_is_toptier(mem_కణుపు) &&
	    (sysctl_numa_balancing_బహుళకం & NUMA_BALANCING_జ్ఞాపకం_TIERING ||
	     !cpupid_valid(చివరి_cpupid)))
		తిరిగి;

	/* Allocate buffer to track faults on a per-node basis */
	ఒకవేళ (unlikely(!p->numa_దోషాలు)) {
		పూర్ణాంకం కొలత = పరిమాణం(*p->numa_దోషాలు) *
			   NR_NUMA_HINT_FAULT_BUCKETS * nr_కణుపు_ids;

		p->numa_దోషాలు = kzalloc(కొలత, GFP_KERNEL|__GFP_NOWARN);
		ఒకవేళ (!p->numa_దోషాలు)
			తిరిగి;

		p->మొత్తం_numa_దోషాలు = 0;
		memset(p->numa_దోషాలు_locality, 0, పరిమాణం(p->numa_దోషాలు_locality));
	}

	/*
	 * First accesses are treated as private, otherwise consider accesses
	 * to be private if the accessing pid has not changed
	 */
	ఒకవేళ (unlikely(చివరి_cpupid == (-1 & చివరి_CPUPID_ముసుగు))) {
		priv = 1;
	} లేకపోతే {
		priv = cpupid_సరిపోలిక_pid(p, చివరి_cpupid);
		ఒకవేళ (!priv && !(flags & TNF_NO_సమూహం))
			task_numa_సమూహం(p, చివరి_cpupid, flags, &priv);
	}

	/*
	 * If a workload spans multiple NUMA nodes, a shared fault that
	 * occurs wholly within the set of nodes that the workload is
	 * actively using should be counted as local. This allows the
	 * scan rate to slow down when a workload has settled down.
	 */
	ng = deref_curr_numa_సమూహం(p);
	ఒకవేళ (!priv && !local && ng && ng->చురుకైన_nodes > 1 &&
				numa_is_చురుకైన_కణుపు(cpu_కణుపు, ng) &&
				numa_is_చురుకైన_కణుపు(mem_కణుపు, ng))
		local = 1;

	/*
	 * Retry to migrate task to preferred node periodically, in case it
	 * previously failed, or the scheduler moved us.
	 */
	ఒకవేళ (సమయం_after(jiffies, p->numa_migrate_retry)) {
		task_numa_placement(p);
		numa_migrate_preferred(p);
	}

	ఒకవేళ (migrated)
		p->numa_పుటలు_migrated += పుటలు;
	ఒకవేళ (flags & TNF_MIGRATE_FAIL)
		p->numa_దోషాలు_locality[2] += పుటలు;

	p->numa_దోషాలు[task_దోషాలు_idx(NUMA_MEMBUF, mem_కణుపు, priv)] += పుటలు;
	p->numa_దోషాలు[task_దోషాలు_idx(NUMA_CPUBUF, cpu_కణుపు, priv)] += పుటలు;
	p->numa_దోషాలు_locality[local] += పుటలు;
}

స్థిర శూన్య reset_ptenuma_పరిశీలన(నిర్మాణం task_struct *p)
{
	/*
	 * We only did a read acquisition of the mmap sem, so
	 * p->mm->numa_scan_seq is written to without exclusive access
	 * and the update is not guaranteed to be atomic. That's not
	 * much of an issue though, since this is just used for
	 * statistical sampling. Use READ_ONCE/WRITE_ONCE, which are not
	 * expensive, to avoid any form of compiler optimizations:
	 */
	WRITE_ONCE(p->mm->numa_పరిశీలన_seq, READ_ONCE(p->mm->numa_పరిశీలన_seq) + 1);
	p->mm->numa_పరిశీలన_స్థానభ్రంశం = 0;
}

స్థిర bool vma_is_accessed(నిర్మాణం mm_struct *mm, నిర్మాణం vm_area_struct *vma)
{
	గుర్తులేని దీర్ఘ pids;
	/*
	 * Allow unconditional access first two times, so that all the (pages)
	 * of VMAs get prot_none fault introduced irrespective of accesses.
	 * This is also done to avoid any side effect of task scanning
	 * amplifying the unfairness of disjoint set of VMAs' access.
	 */
	ఒకవేళ ((READ_ONCE(current->mm->numa_పరిశీలన_seq) - vma->numab_state->ప్రారంభం_పరిశీలన_seq) < 2)
		తిరిగి true;

	pids = vma->numab_state->pids_చురుకైన[0] | vma->numab_state->pids_చురుకైన[1];
	ఒకవేళ (test_బిట్టు(hash_32(current->pid, ilog2(బిట్లు_చొప్పున_LONG)), &pids))
		తిరిగి true;

	/*
	 * Complete a scan that has already started regardless of PID access, or
	 * some VMAs may never be scanned in multi-threaded applications:
	 */
	ఒకవేళ (mm->numa_పరిశీలన_స్థానభ్రంశం > vma->vm_ప్రారంభం) {
		trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_IGNORE_PID);
		తిరిగి true;
	}

	/*
	 * This vma has not been accessed for a while, and if the number
	 * the threads in the same process is low, which means no other
	 * threads can help scan this vma, force a vma scan.
	 */
	ఒకవేళ (READ_ONCE(mm->numa_పరిశీలన_seq) >
	   (vma->numab_state->prev_పరిశీలన_seq + get_nr_దారాలు(current)))
		తిరిగి true;

	తిరిగి false;
}

#నిర్వచించు VMA_PID_RESET_PERIOD (4 * sysctl_numa_balancing_పరిశీలన_delay)

/*
 * The expensive part of numa migration is done from task_work context.
 * Triggered from task_tick_numa().
 */
స్థిర శూన్య task_numa_work(నిర్మాణం callback_తల *work)
{
	గుర్తులేని దీర్ఘ migrate, తదుపరి_పరిశీలన, now = jiffies;
	నిర్మాణం task_struct *p = current;
	నిర్మాణం mm_struct *mm = p->mm;
	u64 runtime = p->se.కూడిక_exec_runtime;
	నిర్మాణం vm_area_struct *vma;
	గుర్తులేని దీర్ఘ ప్రారంభం, చివర;
	గుర్తులేని దీర్ఘ nr_pte_updates = 0;
	దీర్ఘ పుటలు, virtpages;
	నిర్మాణం vma_iterator vmi;
	bool vma_pids_skipped;
	bool vma_pids_forced = false;

	WARN_ON_ONCE(p != container_యొక్క(work, నిర్మాణం task_struct, numa_work));

	work->తదుపరి = work;
	/*
	 * Who cares about NUMA placement when they're dying.
	 *
	 * NOTE: make sure not to dereference p->mm before this check,
	 * exit_task_work() happens _after_ exit_mm() so we could be called
	 * without p->mm even though we still had it when we enqueued this
	 * work.
	 */
	ఒకవేళ (p->flags & PF_EXITING)
		తిరిగి;

	/*
	 * Memory is pinned to only one NUMA node via cpuset.mems, naturally
	 * no page can be migrated.
	 */
	ఒకవేళ (cpusets_enabled() && nodes_బరువు(cpuset_current_mems_allowed) == 1) {
		trace_sched_skip_cpuset_numa(current, &cpuset_current_mems_allowed);
		తిరిగి;
	}

	ఒకవేళ (!mm->numa_తదుపరి_పరిశీలన) {
		mm->numa_తదుపరి_పరిశీలన = now +
			msecs_గమ్యం_jiffies(sysctl_numa_balancing_పరిశీలన_delay);
	}

	/*
	 * Enforce maximal scan/migration frequency..
	 */
	migrate = mm->numa_తదుపరి_పరిశీలన;
	ఒకవేళ (సమయం_before(now, migrate))
		తిరిగి;

	ఒకవేళ (p->numa_పరిశీలన_period == 0) {
		p->numa_పరిశీలన_period_max = task_పరిశీలన_max(p);
		p->numa_పరిశీలన_period = task_పరిశీలన_ప్రారంభం(p);
	}

	తదుపరి_పరిశీలన = now + msecs_గమ్యం_jiffies(p->numa_పరిశీలన_period);
	ఒకవేళ (!try_cmpxchg(&mm->numa_తదుపరి_పరిశీలన, &migrate, తదుపరి_పరిశీలన))
		తిరిగి;

	/*
	 * Delay this task enough that another task of this mm will likely win
	 * the next time around.
	 */
	p->కణుపు_stamp += 2 * TICK_NSEC;

	పుటలు = sysctl_numa_balancing_పరిశీలన_కొలత;
	పుటలు <<= 20 - పుట_జరుపు; /* MB in pages */
	virtpages = పుటలు * 8;	   /* Scan up to this much virtual space */
	ఒకవేళ (!పుటలు)
		తిరిగి;


	ఒకవేళ (!mmap_read_trylock(mm))
		తిరిగి;

	/*
	 * VMAs are skipped if the current PID has not trapped a fault within
	 * the VMA recently. Allow scanning to be forced if there is no
	 * suitable VMA remaining.
	 */
	vma_pids_skipped = false;

retry_pids:
	ప్రారంభం = mm->numa_పరిశీలన_స్థానభ్రంశం;
	vma_iter_init(&vmi, mm, ప్రారంభం);
	vma = vma_తదుపరి(&vmi);
	ఒకవేళ (!vma) {
		reset_ptenuma_పరిశీలన(p);
		ప్రారంభం = 0;
		vma_iter_సమితి(&vmi, ప్రారంభం);
		vma = vma_తదుపరి(&vmi);
	}

	కొరకు (; vma; vma = vma_తదుపరి(&vmi)) {
		ఒకవేళ (!vma_migratable(vma) || !vma_policy_mof(vma) ||
			is_vm_hugetlb_పుట(vma) || (vma->vm_flags & VM_MIXEDMAP)) {
			trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_UNSUITABLE);
			కొనసాగు;
		}

		/*
		 * Shared library pages mapped by multiple processes are not
		 * migrated as it is expected they are cache replicated. Avoid
		 * hinting faults in read-only file-backed mappings or the vDSO
		 * as migrating the pages will be of marginal benefit.
		 */
		ఒకవేళ (!vma->vm_mm ||
		    (vma->vm_file && (vma->vm_flags & (VM_READ|VM_WRITE)) == (VM_READ))) {
			trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_SHARED_RO);
			కొనసాగు;
		}

		/*
		 * Skip inaccessible VMAs to avoid any confusion between
		 * PROT_NONE and NUMA hinting PTEs
		 */
		ఒకవేళ (!vma_is_accessible(vma)) {
			trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_INACCESSIBLE);
			కొనసాగు;
		}

		/* Initialise new per-VMA NUMAB state. */
		ఒకవేళ (!vma->numab_state) {
			నిర్మాణం vma_numab_state *ptr;

			ptr = kzalloc_obj(*ptr);
			ఒకవేళ (!ptr)
				కొనసాగు;

			ఒకవేళ (cmpxchg(&vma->numab_state, ఖాళీ, ptr)) {
				kfree(ptr);
				కొనసాగు;
			}

			vma->numab_state->ప్రారంభం_పరిశీలన_seq = mm->numa_పరిశీలన_seq;

			vma->numab_state->తదుపరి_పరిశీలన = now +
				msecs_గమ్యం_jiffies(sysctl_numa_balancing_పరిశీలన_delay);

			/* Reset happens after 4 times scan delay of scan start */
			vma->numab_state->pids_చురుకైన_reset =  vma->numab_state->తదుపరి_పరిశీలన +
				msecs_గమ్యం_jiffies(VMA_PID_RESET_PERIOD);

			/*
			 * Ensure prev_scan_seq does not match numa_scan_seq,
			 * to prevent VMAs being skipped prematurely on the
			 * first scan:
			 */
			 vma->numab_state->prev_పరిశీలన_seq = mm->numa_పరిశీలన_seq - 1;
		}

		/*
		 * Scanning the VMAs of short lived tasks add more overhead. So
		 * delay the scan for new VMAs.
		 */
		ఒకవేళ (mm->numa_పరిశీలన_seq && సమయం_before(jiffies,
						vma->numab_state->తదుపరి_పరిశీలన)) {
			trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_పరిశీలన_DELAY);
			కొనసాగు;
		}

		/* RESET access PIDs regularly for old VMAs. */
		ఒకవేళ (mm->numa_పరిశీలన_seq &&
				సమయం_after(jiffies, vma->numab_state->pids_చురుకైన_reset)) {
			vma->numab_state->pids_చురుకైన_reset = vma->numab_state->pids_చురుకైన_reset +
				msecs_గమ్యం_jiffies(VMA_PID_RESET_PERIOD);
			vma->numab_state->pids_చురుకైన[0] = READ_ONCE(vma->numab_state->pids_చురుకైన[1]);
			vma->numab_state->pids_చురుకైన[1] = 0;
		}

		/* Do not rescan VMAs twice within the same sequence. */
		ఒకవేళ (vma->numab_state->prev_పరిశీలన_seq == mm->numa_పరిశీలన_seq) {
			mm->numa_పరిశీలన_స్థానభ్రంశం = vma->vm_చివర;
			trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_SEQ_COMPLETED);
			కొనసాగు;
		}

		/*
		 * Do not scan the VMA if task has not accessed it, unless no other
		 * VMA candidate exists.
		 */
		ఒకవేళ (!vma_pids_forced && !vma_is_accessed(mm, vma)) {
			vma_pids_skipped = true;
			trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_PID_INACTIVE);
			కొనసాగు;
		}

		చేయి {
			ప్రారంభం = max(ప్రారంభం, vma->vm_ప్రారంభం);
			చివర = ALIGN(ప్రారంభం + (పుటలు << పుట_జరుపు), HPAGE_కొలత);
			చివర = min(చివర, vma->vm_చివర);
			nr_pte_updates = change_prot_numa(vma, ప్రారంభం, చివర);

			/*
			 * Try to scan sysctl_numa_balancing_size worth of
			 * hpages that have at least one present PTE that
			 * is not already PTE-numa. If the VMA contains
			 * areas that are unused or already full of prot_numa
			 * PTEs, scan up to virtpages, to skip through those
			 * areas faster.
			 */
			ఒకవేళ (nr_pte_updates)
				పుటలు -= (చివర - ప్రారంభం) >> పుట_జరుపు;
			virtpages -= (చివర - ప్రారంభం) >> పుట_జరుపు;

			ప్రారంభం = చివర;
			ఒకవేళ (పుటలు <= 0 || virtpages <= 0)
				వెళ్ళు బహిర్గతం;

			cond_resched();
		} అయితే (చివర != vma->vm_చివర);

		/* VMA scan is complete, do not scan until next sequence. */
		vma->numab_state->prev_పరిశీలన_seq = mm->numa_పరిశీలన_seq;

		/*
		 * Only force scan within one VMA at a time, to limit the
		 * cost of scanning a potentially uninteresting VMA.
		 */
		ఒకవేళ (vma_pids_forced)
			ఆపు;
	}

	/*
	 * If no VMAs are remaining and VMAs were skipped due to the PID
	 * not accessing the VMA previously, then force a scan to ensure
	 * forward progress:
	 */
	ఒకవేళ (!vma && !vma_pids_forced && vma_pids_skipped) {
		vma_pids_forced = true;
		వెళ్ళు retry_pids;
	}

బహిర్గతం:
	/*
	 * It is possible to reach the end of the VMA list but the last few
	 * VMAs are not guaranteed to the vma_migratable. If they are not, we
	 * would find the !migratable VMA on the next scan but not reset the
	 * scanner to the start so check it now.
	 */
	ఒకవేళ (vma)
		mm->numa_పరిశీలన_స్థానభ్రంశం = ప్రారంభం;
	లేకపోతే
		reset_ptenuma_పరిశీలన(p);
	mmap_read_unlock(mm);

	/*
	 * Make sure tasks use at least 32x as much time to run other code
	 * than they used here, to limit NUMA PTE scanning overhead to 3% max.
	 * Usually update_task_scan_period slows down scanning enough; on an
	 * overloaded system we need to limit overhead on a per task basis.
	 */
	ఒకవేళ (unlikely(p->se.కూడిక_exec_runtime != runtime)) {
		u64 diff = p->se.కూడిక_exec_runtime - runtime;
		p->కణుపు_stamp += 32 * diff;
	}
}

శూన్య init_numa_balancing(u64 clone_flags, నిర్మాణం task_struct *p)
{
	పూర్ణాంకం mm_users = 0;
	నిర్మాణం mm_struct *mm = p->mm;

	ఒకవేళ (mm) {
		mm_users = atomic_read(&mm->mm_users);
		ఒకవేళ (mm_users == 1) {
			mm->numa_తదుపరి_పరిశీలన = jiffies + msecs_గమ్యం_jiffies(sysctl_numa_balancing_పరిశీలన_delay);
			mm->numa_పరిశీలన_seq = 0;
		}
	}
	p->కణుపు_stamp			= 0;
	p->numa_పరిశీలన_seq		= mm ? mm->numa_పరిశీలన_seq : 0;
	p->numa_పరిశీలన_period		= sysctl_numa_balancing_పరిశీలన_delay;
	p->numa_migrate_retry		= 0;
	/* Protect against double add, see task_tick_numa and task_numa_work */
	p->numa_work.తదుపరి		= &p->numa_work;
	p->numa_దోషాలు			= ఖాళీ;
	p->numa_పుటలు_migrated		= 0;
	p->మొత్తం_numa_దోషాలు		= 0;
	RCU_INIT_సూచిక(p->numa_సమూహం, ఖాళీ);
	p->చివరి_task_numa_placement	= 0;
	p->చివరి_కూడిక_exec_runtime	= 0;

	init_task_work(&p->numa_work, task_numa_work);

	/* New address space, reset the preferred nid */
	ఒకవేళ (!(clone_flags & CLONE_VM)) {
		p->numa_preferred_nid = NUMA_NO_కణుపు;
		తిరిగి;
	}

	/*
	 * New thread, keep existing numa_preferred_nid which should be copied
	 * already by arch_dup_task_struct but stagger when scans start.
	 */
	ఒకవేళ (mm) {
		గుర్తులేని పూర్ణాంకం delay;

		delay = min_t(గుర్తులేని పూర్ణాంకం, task_పరిశీలన_max(current),
			current->numa_పరిశీలన_period * mm_users * NSEC_చొప్పున_MSEC);
		delay += 2 * TICK_NSEC;
		p->కణుపు_stamp = delay;
	}
}

/*
 * Drive the periodic memory faults..
 */
స్థిర శూన్య task_tick_numa(నిర్మాణం rq *rq, నిర్మాణం task_struct *curr)
{
	నిర్మాణం callback_తల *work = &curr->numa_work;
	u64 period, now;

	/*
	 * We don't care about NUMA placement if we don't have memory.
	 */
	ఒకవేళ (!curr->mm || (curr->flags & (PF_EXITING | PF_KTHREAD)) || work->తదుపరి != work)
		తిరిగి;

	/*
	 * Using runtime rather than walltime has the dual advantage that
	 * we (mostly) drive the selection from busy threads and that the
	 * task needs to have done some actual work before we bother with
	 * NUMA placement.
	 */
	now = curr->se.కూడిక_exec_runtime;
	period = (u64)curr->numa_పరిశీలన_period * NSEC_చొప్పున_MSEC;

	ఒకవేళ (now > curr->కణుపు_stamp + period) {
		ఒకవేళ (!curr->కణుపు_stamp)
			curr->numa_పరిశీలన_period = task_పరిశీలన_ప్రారంభం(curr);
		curr->కణుపు_stamp += period;

		ఒకవేళ (!సమయం_before(jiffies, curr->mm->numa_తదుపరి_పరిశీలన))
			task_work_కూడు(curr, work, TWA_RESUME);
	}
}

స్థిర శూన్య update_పరిశీలన_period(నిర్మాణం task_struct *p, పూర్ణాంకం new_cpu)
{
	పూర్ణాంకం src_nid = cpu_గమ్యం_కణుపు(task_cpu(p));
	పూర్ణాంకం dst_nid = cpu_గమ్యం_కణుపు(new_cpu);

	ఒకవేళ (!static_branch_likely(&sched_numa_balancing))
		తిరిగి;

	ఒకవేళ (!p->mm || !p->numa_దోషాలు || (p->flags & PF_EXITING))
		తిరిగి;

	ఒకవేళ (src_nid == dst_nid)
		తిరిగి;

	/*
	 * Allow resets if faults have been trapped before one scan
	 * has completed. This is most likely due to a new task that
	 * is pulled cross-node due to wakeups or load balancing.
	 */
	ఒకవేళ (p->numa_పరిశీలన_seq) {
		/*
		 * Avoid scan adjustments if moving to the preferred
		 * node or if the task was not previously running on
		 * the preferred node.
		 */
		ఒకవేళ (dst_nid == p->numa_preferred_nid ||
		    (p->numa_preferred_nid != NUMA_NO_కణుపు &&
			src_nid != p->numa_preferred_nid))
			తిరిగి;
	}

	p->numa_పరిశీలన_period = task_పరిశీలన_ప్రారంభం(p);
}

#లేకపోతే /* !CONFIG_NUMA_BALANCING: */

స్థిర శూన్య task_tick_numa(నిర్మాణం rq *rq, నిర్మాణం task_struct *curr)
{
}

స్థిర అంతఃవరుస శూన్య account_numa_వరుసలో_చేర్చు(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
}

స్థిర అంతఃవరుస శూన్య account_numa_వరుస_నుండి_తీయి(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
}

స్థిర అంతఃవరుస శూన్య update_పరిశీలన_period(నిర్మాణం task_struct *p, పూర్ణాంకం new_cpu)
{
}

#అయితే_ముగింపు /* !CONFIG_NUMA_BALANCING */

స్థిర శూన్య
account_entity_వరుసలో_చేర్చు(నిర్మాణం cfs_rq *cfs_rq, నిర్మాణం sched_entity *se)
{
	WARN_ON_ONCE(cfs_rq != cfs_rq_యొక్క(se));
	update_ఎక్కించు_కూడు(&cfs_rq->ఎక్కించు, se->ఎక్కించు.బరువు);
	ఒకవేళ (entity_is_task(se)) {
		నిర్మాణం task_struct *p = task_యొక్క(se);
		నిర్మాణం rq *rq = rq_యొక్క(cfs_rq);

		account_numa_వరుసలో_చేర్చు(rq, p);
		account_llc_వరుసలో_చేర్చు(rq, p);
		list_కూడు(&se->సమూహం_కణుపు, &rq->cfs_tasks);
	}
	cfs_rq->nr_queued++;
}

స్థిర శూన్య
account_entity_వరుస_నుండి_తీయి(నిర్మాణం cfs_rq *cfs_rq, నిర్మాణం sched_entity *se)
{
	WARN_ON_ONCE(cfs_rq != cfs_rq_యొక్క(se));
	update_ఎక్కించు_sub(&cfs_rq->ఎక్కించు, se->ఎక్కించు.బరువు);
	ఒకవేళ (entity_is_task(se)) {
		నిర్మాణం task_struct *p = task_యొక్క(se);
		నిర్మాణం rq *rq = rq_యొక్క(cfs_rq);

		account_numa_వరుస_నుండి_తీయి(rq, p);
		account_llc_వరుస_నుండి_తీయి(rq, p);
		list_del_init(&se->సమూహం_కణుపు);
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
#నిర్వచించు కూడు_ధనాత్మక(_ptr, _val) చేయి {                           \
	typeof(_ptr) ptr = (_ptr);                              \
	__signed_scalar_typeof(*ptr) val = (_val);              \
	typeof(*ptr) res, var = READ_ONCE(*ptr);                \
								\
	res = var + val;                                        \
								\
	ఒకవేళ (val < 0 && res > var)                               \
		res = 0;                                        \
								\
	WRITE_ONCE(*ptr, res);                                  \
} అయితే (0)

/*
 * Remove and clamp on negative, from a local variable.
 *
 * A variant of sub_positive(), which does not use explicit load-store
 * and is thus optimized for local variable updates.
 */
#నిర్వచించు lsub_ధనాత్మక(_ptr, _val) చేయి {				\
	typeof(_ptr) ptr = (_ptr);				\
	*ptr -= min_t(typeof(*ptr), *ptr, _val);		\
} అయితే (0)


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
#నిర్వచించు __update_sa(sa, name, delta_avg, delta_కూడిక) చేయి {	\
	కూడు_ధనాత్మక(&(sa)->name##_avg, delta_avg);		\
	కూడు_ధనాత్మక(&(sa)->name##_sum, delta_కూడిక);		\
	(sa)->name##_sum = max_t(typeof((sa)->name##_sum),	\
			       (sa)->name##_sum,		\
			       (sa)->name##_avg * PELT_MIN_DIVIDER); \
} అయితే (0)

స్థిర అంతఃవరుస శూన్య
వరుసలో_చేర్చు_ఎక్కించు_avg(నిర్మాణం cfs_rq *cfs_rq, నిర్మాణం sched_entity *se)
{
	__update_sa(&cfs_rq->avg, ఎక్కించు, se->avg.ఎక్కించు_avg,
		    se_బరువు(se) * se->avg.ఎక్కించు_కూడిక);
}

స్థిర అంతఃవరుస శూన్య
వరుస_నుండి_తీయి_ఎక్కించు_avg(నిర్మాణం cfs_rq *cfs_rq, నిర్మాణం sched_entity *se)
{
	__update_sa(&cfs_rq->avg, ఎక్కించు, -se->avg.ఎక్కించు_avg,
		    se_బరువు(se) * -se->avg.ఎక్కించు_కూడిక);
}

స్థిర శూన్య
rescale_entity(నిర్మాణం sched_entity *se, గుర్తులేని దీర్ఘ బరువు, bool rel_vprot)
{
	దీర్ఘ old_బరువు = se->h_ఎక్కించు.బరువు;

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
	se->vlag = div64_long(se->vlag * old_బరువు, బరువు);

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
	ఒకవేళ (se->rel_deadline)
		se->deadline = div64_long(se->deadline * old_బరువు, బరువు);

	ఒకవేళ (rel_vprot)
		se->vprot = div64_long(se->vprot * old_బరువు, బరువు);
}

స్థిర శూన్య reweight_eevdf(నిర్మాణం cfs_rq *cfs_rq, నిర్మాణం sched_entity *se,
			   గుర్తులేని దీర్ఘ బరువు, bool on_rq)
{
	bool curr = cfs_rq->curr == se;
	bool rel_vprot = false;
	u64 avruntime = 0;

	ఒకవేళ (se->h_ఎక్కించు.బరువు == బరువు)
		తిరిగి;

	ఒకవేళ (on_rq) {
		avruntime = avg_vruntime(cfs_rq);
		se->vlag = entity_lag(cfs_rq, se, avruntime);
		se->deadline -= avruntime;
		se->rel_deadline = 1;
		ఒకవేళ (curr && protect_ముక్క(se)) {
			se->vprot -= avruntime;
			rel_vprot = true;
		}

		cfs_rq->h_nr_queued--;
		ఒకవేళ (!curr)
			__dequeue_entity(cfs_rq, se);
	}

	rescale_entity(se, బరువు, rel_vprot);

	update_ఎక్కించు_సమితి(&se->h_ఎక్కించు, బరువు);

	ఒకవేళ (on_rq) {
		ఒకవేళ (rel_vprot)
			se->vprot += avruntime;
		se->deadline += avruntime;
		se->rel_deadline = 0;
		se->vruntime = avruntime - se->vlag;

		ఒకవేళ (!curr)
			__enqueue_entity(cfs_rq, se);
		cfs_rq->h_nr_queued++;
	}
}

స్థిర శూన్య reweight_entity(నిర్మాణం cfs_rq *cfs_rq, నిర్మాణం sched_entity *se,
			    గుర్తులేని దీర్ఘ బరువు)
{
	ఒకవేళ (se->ఎక్కించు.బరువు == బరువు)
		తిరిగి;

	ఒకవేళ (se->on_rq) {
		WARN_ON_ONCE(cfs_rq != cfs_rq_యొక్క(se));
		update_ఎక్కించు_sub(&cfs_rq->ఎక్కించు, se->ఎక్కించు.బరువు);
	}
	వరుస_నుండి_తీయి_ఎక్కించు_avg(cfs_rq, se);

	update_ఎక్కించు_సమితి(&se->ఎక్కించు, బరువు);

	చేయి {
		u32 divider = get_pelt_divider(&se->avg);
		se->avg.ఎక్కించు_avg = div_u64(se_బరువు(se) * se->avg.ఎక్కించు_కూడిక, divider);
	} అయితే (0);

	వరుసలో_చేర్చు_ఎక్కించు_avg(cfs_rq, se);

	ఒకవేళ (se->on_rq)
		update_ఎక్కించు_కూడు(&cfs_rq->ఎక్కించు, se->ఎక్కించు.బరువు);
}

/*
 * weight = NICE_0_LOAD;
 * for_each_entity_se(se)
 *   weight = __calc_prop_weight(cfs_rq_of(se), se, weight);
 */
స్థిర __always_inline
గుర్తులేని దీర్ఘ __calc_prop_weight(నిర్మాణం cfs_rq *cfs_rq, నిర్మాణం sched_entity *se,
				 గుర్తులేని దీర్ఘ బరువు)
{
	బరువు *= se->ఎక్కించు.బరువు;
	ఒకవేళ (మాతృ_entity(se))
		బరువు /= cfs_rq->ఎక్కించు.బరువు;
	లేకపోతే
		బరువు /= NICE_0_ఎక్కించు;

	తిరిగి max(బరువు, MIN_SHARES);
}

స్థిర శూన్య reweight_task_fair(నిర్మాణం rq *rq, నిర్మాణం task_struct *p,
			       స్థిరరాశి నిర్మాణం ఎక్కించు_బరువు *lw)
{
	నిర్మాణం sched_entity *se = &p->se;
	గుర్తులేని దీర్ఘ బరువు = NICE_0_ఎక్కించు;

	ఒకవేళ (se->on_rq)
		update_curr_fair(rq);

	reweight_entity(cfs_rq_యొక్క(se), se, lw->బరువు);
	se->ఎక్కించు.inv_బరువు = lw->inv_బరువు;

	ఒకవేళ (!se->on_rq)
		తిరిగి;

	for_each_sched_entity(se)
		బరువు = __calc_prop_weight(cfs_rq_యొక్క(se), se, బరువు);

	reweight_eevdf(&rq->cfs, &p->se, బరువు, p->se.on_rq);
}

స్థిర అంతఃవరుస పూర్ణాంకం throttled_hierarchy(నిర్మాణం cfs_rq *cfs_rq);

#నిర్వచితమైతే CONFIG_FAIR_సమూహం_SCHED
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
స్థిర దీర్ఘ __calc_smp_shares(నిర్మాణం cfs_rq *cfs_rq, దీర్ఘ tg_shares, దీర్ఘ shares_max)
{
	నిర్మాణం task_సమూహం *tg = cfs_rq->tg;
	దీర్ఘ tg_బరువు, ఎక్కించు, shares;

	ఎక్కించు = max(scale_ఎక్కించు_కిందికి(cfs_rq->ఎక్కించు.బరువు), cfs_rq->avg.ఎక్కించు_avg);

	tg_బరువు = atomic_long_read(&tg->ఎక్కించు_avg);

	/* Ensure tg_weight >= load */
	tg_బరువు -= cfs_rq->tg_ఎక్కించు_avg_contrib;
	tg_బరువు += ఎక్కించు;

	shares = (tg_shares * ఎక్కించు);
	ఒకవేళ (tg_బరువు)
		shares /= tg_బరువు;

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
	తిరిగి clamp_t(దీర్ఘ, shares, MIN_SHARES, shares_max);
}

స్థిర పూర్ణాంకం tg_cpus(నిర్మాణం task_సమూహం *tg)
{
	పూర్ణాంకం nr = అంకె_online_cpus();

	ఒకవేళ (cpusets_enabled()) {
		నిర్మాణం cgroup *cgrp = tg->css.cgroup;
		ఒకవేళ (cgrp)
			nr = cpuset_అంకె_cpus(cgrp);
	}

	/*
	 * An empty cpuset would propagate a 0 shares_max into
	 * __calc_smp_shares(), where clamp() yields hi when hi < lo and so
	 * defeats the MIN_SHARES floor. Match tg_tasks(), which floors at 1.
	 */
	తిరిగి max(nr, 1);
}

స్థిర అంతఃవరుస పూర్ణాంకం tg_tasks(నిర్మాణం task_సమూహం *tg)
{
	తిరిగి max(1, atomic_long_read(&tg->runnable_avg) >> SCHED_సామర్థ్యం_జరుపు);
}

/*
 * Func: fraction(nr_tasks * tg->shares)
 *
 * Scale tg->shares by the number of tasks.
 */
స్థిర దీర్ఘ calc_tasks_shares(నిర్మాణం cfs_rq *cfs_rq)
{
	నిర్మాణం task_సమూహం *tg = cfs_rq->tg;
	పూర్ణాంకం nr = tg_tasks(tg);
	దీర్ఘ tg_shares = READ_ONCE(tg->shares);
	తిరిగి __calc_smp_shares(cfs_rq, nr * tg_shares, nr * tg_shares);
}

/*
 * Func: min(fraction(nr_cpus * tg->shares), nice -20)
 *
 * Scale tg->shares by the maximal number of CPUs; but clip the max shares at
 * nice -20, otherwise a single spinner on a 512 CPU machine would result in
 * 512*NICE_0_LOAD, which is also crazy.
 */
స్థిర దీర్ఘ calc_max_shares(నిర్మాణం cfs_rq *cfs_rq)
{
	నిర్మాణం task_సమూహం *tg = cfs_rq->tg;
	పూర్ణాంకం nr = tg_cpus(tg);
	దీర్ఘ tg_shares = READ_ONCE(tg->shares);
	దీర్ఘ max_shares = scale_ఎక్కించు(sched_prio_గమ్యం_బరువు[0]);
	తిరిగి __calc_smp_shares(cfs_rq, tg_shares * nr, max_shares);
}

/*
 * Func: fraction(nr * tg->shares); nr = min(nr_tasks, nr_cpus)
 *
 * Scales between "smp" and "max" in a natural way. No longer needs clipping
 * since there are no unnatural inflations like with "max".
 */
స్థిర దీర్ఘ calc_concur_shares(నిర్మాణం cfs_rq *cfs_rq)
{
	నిర్మాణం task_సమూహం *tg = cfs_rq->tg;
	పూర్ణాంకం nr = min(tg_tasks(tg), tg_cpus(tg));
	దీర్ఘ tg_shares = READ_ONCE(tg->shares);
	తిరిగి __calc_smp_shares(cfs_rq, nr * tg_shares, nr * tg_shares);
}

/*
 * Func: fraction(tg->shares)
 *
 * This infamously results in tiny shares when you have many CPUs.
 */
స్థిర దీర్ఘ calc_smp_shares(నిర్మాణం cfs_rq *cfs_rq)
{
	నిర్మాణం task_సమూహం *tg = cfs_rq->tg;
	దీర్ఘ tg_shares = READ_ONCE(tg->shares);
	తిరిగి __calc_smp_shares(cfs_rq, tg_shares, tg_shares);
}

/*
 * Ignore this pesky SMP stuff, use (4).
 */
స్థిర దీర్ఘ calc_పైకి_shares(నిర్మాణం cfs_rq *cfs_rq)
{
	నిర్మాణం task_సమూహం *tg = cfs_rq->tg;
	తిరిగి READ_ONCE(tg->shares);
}

DEFINE_STATIC_CALL(calc_సమూహం_shares, calc_concur_shares);

శూన్య __sched_cgroup_mode_update(పూర్ణాంకం బహుళకం)
{
	దీర్ఘ (*func)(నిర్మాణం cfs_rq *);
	ఎంపిక (బహుళకం) {
	స్థితి 0:
		func = &calc_పైకి_shares;
		ఆపు;
	స్థితి 1:
		func = &calc_smp_shares;
		ఆపు;
	స్థితి 2:
	అప్రమేయ:
		func = &calc_concur_shares;
		ఆపు;
	స్థితి 3:
		func = &calc_max_shares;
		ఆపు;
	స్థితి 4:
		func = &calc_tasks_shares;
		ఆపు;
	}
	static_call_update(calc_సమూహం_shares, func);
}

/*
 * Recomputes the group entity based on the current state of its group
 * runqueue.
 */
స్థిర శూన్య update_cfs_సమూహం(నిర్మాణం sched_entity *se)
{
	నిర్మాణం cfs_rq *gcfs_rq = సమూహం_cfs_rq(se);
	దీర్ఘ shares;

	/*
	 * When a group becomes empty, preserve its weight. This matters for
	 * DELAY_DEQUEUE.
	 */
	ఒకవేళ (!gcfs_rq || !gcfs_rq->ఎక్కించు.బరువు)
		తిరిగి;

	shares = static_call(calc_సమూహం_shares)(gcfs_rq);
	reweight_entity(cfs_rq_యొక్క(se), se, shares);
}

#లేకపోతే /* !CONFIG_FAIR_GROUP_SCHED: */
స్థిర అంతఃవరుస శూన్య update_cfs_సమూహం(నిర్మాణం sched_entity *se)
{
}
#అయితే_ముగింపు /* !CONFIG_FAIR_GROUP_SCHED */

స్థిర అంతఃవరుస శూన్య cfs_rq_util_change(నిర్మాణం cfs_rq *cfs_rq, పూర్ణాంకం flags)
{
	నిర్మాణం rq *rq = rq_యొక్క(cfs_rq);

	ఒకవేళ (&rq->cfs == cfs_rq) {
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

స్థిర అంతఃవరుస bool ఎక్కించు_avg_is_decayed(నిర్మాణం sched_avg *sa)
{
	ఒకవేళ (sa->ఎక్కించు_కూడిక)
		తిరిగి false;

	ఒకవేళ (sa->util_కూడిక)
		తిరిగి false;

	ఒకవేళ (sa->runnable_కూడిక)
		తిరిగి false;

	/*
	 * _avg must be null when _sum are null because _avg = _sum / divider
	 * Make sure that rounding and/or propagation of PELT values never
	 * break this.
	 */
	WARN_ON_ONCE(sa->ఎక్కించు_avg ||
		      sa->util_avg ||
		      sa->runnable_avg);

	తిరిగి true;
}

స్థిర అంతఃవరుస u64 cfs_rq_చివరి_update_సమయం(నిర్మాణం cfs_rq *cfs_rq)
{
	తిరిగి u64_u32_ఎక్కించు_copy(cfs_rq->avg.చివరి_update_సమయం,
				 cfs_rq->చివరి_update_సమయం_copy);
}
#నిర్వచితమైతే CONFIG_FAIR_సమూహం_SCHED
/*
 * Because list_add_leaf_cfs_rq always places a child cfs_rq on the list
 * immediately before a parent cfs_rq, and cfs_rqs are removed from the list
 * bottom-up, we only have to test whether the cfs_rq before us on the list
 * is our child.
 * If cfs_rq is not on the list, test whether a child needs its to be added to
 * connect a branch to the tree  * (see list_add_leaf_cfs_rq() for details).
 */
స్థిర అంతఃవరుస bool బిడ్డ_cfs_rq_on_list(నిర్మాణం cfs_rq *cfs_rq)
{
	నిర్మాణం cfs_rq *prev_cfs_rq;
	నిర్మాణం list_తల *prev;
	నిర్మాణం rq *rq = rq_యొక్క(cfs_rq);

	ఒకవేళ (cfs_rq->on_list) {
		prev = cfs_rq->ఆకు_cfs_rq_list.prev;
	} లేకపోతే {
		prev = rq->tmp_alone_branch;
	}

	ఒకవేళ (prev == &rq->ఆకు_cfs_rq_list)
		తిరిగి false;

	prev_cfs_rq = container_యొక్క(prev, నిర్మాణం cfs_rq, ఆకు_cfs_rq_list);

	తిరిగి (prev_cfs_rq->tg->మాతృ == cfs_rq->tg);
}

స్థిర అంతఃవరుస bool cfs_rq_is_decayed(నిర్మాణం cfs_rq *cfs_rq)
{
	ఒకవేళ (cfs_rq->ఎక్కించు.బరువు)
		తిరిగి false;

	ఒకవేళ (!ఎక్కించు_avg_is_decayed(&cfs_rq->avg))
		తిరిగి false;

	ఒకవేళ (బిడ్డ_cfs_rq_on_list(cfs_rq))
		తిరిగి false;

	ఒకవేళ (cfs_rq->tg_ఎక్కించు_avg_contrib)
		తిరిగి false;

	తిరిగి true;
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
స్థిర అంతఃవరుస శూన్య update_tg_ఎక్కించు_avg(నిర్మాణం cfs_rq *cfs_rq)
{
	దీర్ఘ dl, dr;
	u64 now;

	/*
	 * No need to update load_avg for root_task_group as it is not used.
	 */
	ఒకవేళ (cfs_rq->tg == &మూలం_task_సమూహం)
		తిరిగి;

	/* rq has been offline and doesn't contribute to the share anymore: */
	ఒకవేళ (!cpu_చురుకైన(cpu_యొక్క(rq_యొక్క(cfs_rq))))
		తిరిగి;

	/*
	 * For migration heavy workloads, access to tg->load_avg can be
	 * unbound. Limit the update rate to at most once per ms.
	 */
	now = rq_clock(rq_యొక్క(cfs_rq));
	ఒకవేళ (now - cfs_rq->చివరి_update_tg_ఎక్కించు_avg < NSEC_చొప్పున_MSEC)
		తిరిగి;

	dl = cfs_rq->avg.ఎక్కించు_avg - cfs_rq->tg_ఎక్కించు_avg_contrib;
	dr = cfs_rq->avg.runnable_avg - cfs_rq->tg_runnable_avg_contrib;
	ఒకవేళ (నిరపేక్ష(dl) > cfs_rq->tg_ఎక్కించు_avg_contrib / 64 ||
	    నిరపేక్ష(dr) > cfs_rq->tg_runnable_avg_contrib / 64) {
		atomic_long_కూడు(dl, &cfs_rq->tg->ఎక్కించు_avg);
		atomic_long_కూడు(dr, &cfs_rq->tg->runnable_avg);
		cfs_rq->tg_ఎక్కించు_avg_contrib = cfs_rq->avg.ఎక్కించు_avg;
		cfs_rq->tg_runnable_avg_contrib = cfs_rq->avg.runnable_avg;
		cfs_rq->చివరి_update_tg_ఎక్కించు_avg = now;
	}
}

స్థిర అంతఃవరుస శూన్య clear_tg_ఎక్కించు_avg(నిర్మాణం cfs_rq *cfs_rq)
{
	దీర్ఘ dl, dr;
	u64 now;

	/*
	 * No need to update load_avg for root_task_group, as it is not used.
	 */
	ఒకవేళ (cfs_rq->tg == &మూలం_task_సమూహం)
		తిరిగి;

	now = rq_clock(rq_యొక్క(cfs_rq));
	dl = 0 - cfs_rq->tg_ఎక్కించు_avg_contrib;
	dr = 0 - cfs_rq->tg_runnable_avg_contrib;
	atomic_long_కూడు(dl, &cfs_rq->tg->ఎక్కించు_avg);
	atomic_long_కూడు(dr, &cfs_rq->tg->runnable_avg);
	cfs_rq->tg_ఎక్కించు_avg_contrib = 0;
	cfs_rq->tg_runnable_avg_contrib = 0;
	cfs_rq->చివరి_update_tg_ఎక్కించు_avg = now;
}

/* CPU offline callback: */
స్థిర శూన్య __maybe_unused clear_tg_offline_cfs_rqs(నిర్మాణం rq *rq)
{
	నిర్మాణం task_సమూహం *tg;

	lockdep_assert_rq_held(rq);

	/*
	 * The rq clock has already been updated in
	 * set_rq_offline(), so we should skip updating
	 * the rq clock again in unthrottle_cfs_rq().
	 */
	rq_clock_ప్రారంభం_loop_update(rq);

	guard(rcu)();

	list_for_each_నమోదు_rcu(tg, &task_groups, list) {
		నిర్మాణం cfs_rq *cfs_rq = tg_cfs_rq(tg, cpu_యొక్క(rq));

		clear_tg_ఎక్కించు_avg(cfs_rq);
	}

	rq_clock_stop_loop_update(rq);
}

/*
 * Called within set_task_rq() right before setting a task's CPU. The
 * caller only guarantees p->pi_lock is held; no other assumptions,
 * including the state of rq->lock, should be made.
 */
శూన్య సమితి_task_rq_fair(నిర్మాణం sched_entity *se,
		      నిర్మాణం cfs_rq *prev, నిర్మాణం cfs_rq *తదుపరి)
{
	u64 p_చివరి_update_సమయం;
	u64 n_చివరి_update_సమయం;

	ఒకవేళ (!sched_feat(ATTACH_AGE_ఎక్కించు))
		తిరిగి;

	/*
	 * We are supposed to update the task to "current" time, then its up to
	 * date and ready to go to new CPU/cfs_rq. But we have difficulty in
	 * getting what current time is, so simply throw away the out-of-date
	 * time. This will result in the wakee task is less decayed, but giving
	 * the wakee more load sounds not bad.
	 */
	ఒకవేళ (!(se->avg.చివరి_update_సమయం && prev))
		తిరిగి;

	p_చివరి_update_సమయం = cfs_rq_చివరి_update_సమయం(prev);
	n_చివరి_update_సమయం = cfs_rq_చివరి_update_సమయం(తదుపరి);

	__update_load_avg_blocked_se(p_చివరి_update_సమయం, se);
	se->avg.చివరి_update_సమయం = n_చివరి_update_సమయం;
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
స్థిర అంతఃవరుస శూన్య
update_tg_cfs_util(నిర్మాణం cfs_rq *cfs_rq, నిర్మాణం sched_entity *se, నిర్మాణం cfs_rq *gcfs_rq)
{
	దీర్ఘ delta_కూడిక, delta_avg = gcfs_rq->avg.util_avg - se->avg.util_avg;
	u32 new_కూడిక, divider;

	/* Nothing to update */
	ఒకవేళ (!delta_avg)
		తిరిగి;

	/*
	 * cfs_rq->avg.period_contrib can be used for both cfs_rq and se.
	 * See ___update_load_avg() for details.
	 */
	divider = get_pelt_divider(&cfs_rq->avg);

	/* Set new sched_entity's utilization */
	se->avg.util_avg = gcfs_rq->avg.util_avg;
	new_కూడిక = se->avg.util_avg * divider;
	delta_కూడిక = (దీర్ఘ)new_కూడిక - (దీర్ఘ)se->avg.util_కూడిక;
	se->avg.util_కూడిక = new_కూడిక;

	/* Update parent cfs_rq utilization */
	__update_sa(&cfs_rq->avg, util, delta_avg, delta_కూడిక);
}

స్థిర అంతఃవరుస శూన్య
update_tg_cfs_runnable(నిర్మాణం cfs_rq *cfs_rq, నిర్మాణం sched_entity *se, నిర్మాణం cfs_rq *gcfs_rq)
{
	దీర్ఘ delta_కూడిక, delta_avg = gcfs_rq->avg.runnable_avg - se->avg.runnable_avg;
	u64 new_కూడిక;
	u32 divider;

	/* Nothing to update */
	ఒకవేళ (!delta_avg)
		తిరిగి;

	/*
	 * cfs_rq->avg.period_contrib can be used for both cfs_rq and se.
	 * See ___update_load_avg() for details.
	 */
	divider = get_pelt_divider(&cfs_rq->avg);

	/* Set new sched_entity's runnable */
	se->avg.runnable_avg = gcfs_rq->avg.runnable_avg;
	new_కూడిక = (u64)se->avg.runnable_avg * divider;
	delta_కూడిక = (దీర్ఘ)new_కూడిక - (దీర్ఘ)se->avg.runnable_కూడిక;
	se->avg.runnable_కూడిక = new_కూడిక;

	/* Update parent cfs_rq runnable */
	__update_sa(&cfs_rq->avg, runnable, delta_avg, delta_కూడిక);
}

స్థిర అంతఃవరుస శూన్య
update_tg_cfs_ఎక్కించు(నిర్మాణం cfs_rq *cfs_rq, నిర్మాణం sched_entity *se, నిర్మాణం cfs_rq *gcfs_rq)
{
	దీర్ఘ delta_avg, running_కూడిక, runnable_కూడిక = gcfs_rq->prop_runnable_కూడిక;
	గుర్తులేని దీర్ఘ ఎక్కించు_avg;
	u64 ఎక్కించు_కూడిక = 0;
	s64 delta_కూడిక;
	u32 divider;

	ఒకవేళ (!runnable_కూడిక)
		తిరిగి;

	gcfs_rq->prop_runnable_కూడిక = 0;

	/*
	 * cfs_rq->avg.period_contrib can be used for both cfs_rq and se.
	 * See ___update_load_avg() for details.
	 */
	divider = get_pelt_divider(&cfs_rq->avg);

	ఒకవేళ (runnable_కూడిక >= 0) {
		/*
		 * Add runnable; clip at LOAD_AVG_MAX. Reflects that until
		 * the CPU is saturated running == runnable.
		 */
		runnable_కూడిక += se->avg.ఎక్కించు_కూడిక;
		runnable_కూడిక = min_t(దీర్ఘ, runnable_కూడిక, divider);
	} లేకపోతే {
		/*
		 * Estimate the new unweighted runnable_sum of the gcfs_rq by
		 * assuming all tasks are equally runnable.
		 */
		ఒకవేళ (scale_ఎక్కించు_కిందికి(gcfs_rq->ఎక్కించు.బరువు)) {
			ఎక్కించు_కూడిక = div_u64(gcfs_rq->avg.ఎక్కించు_కూడిక,
				scale_ఎక్కించు_కిందికి(gcfs_rq->ఎక్కించు.బరువు));
		}

		/* But make sure to not inflate se's runnable */
		runnable_కూడిక = min(se->avg.ఎక్కించు_కూడిక, ఎక్కించు_కూడిక);
	}

	/*
	 * runnable_sum can't be lower than running_sum
	 * Rescale running sum to be in the same range as runnable sum
	 * running_sum is in [0 : LOAD_AVG_MAX <<  SCHED_CAPACITY_SHIFT]
	 * runnable_sum is in [0 : LOAD_AVG_MAX]
	 */
	running_కూడిక = se->avg.util_కూడిక >> SCHED_సామర్థ్యం_జరుపు;
	runnable_కూడిక = max(runnable_కూడిక, running_కూడిక);

	ఎక్కించు_కూడిక = se_బరువు(se) * runnable_కూడిక;
	ఎక్కించు_avg = div_u64(ఎక్కించు_కూడిక, divider);

	delta_avg = ఎక్కించు_avg - se->avg.ఎక్కించు_avg;
	ఒకవేళ (!delta_avg)
		తిరిగి;

	delta_కూడిక = ఎక్కించు_కూడిక - (s64)se_బరువు(se) * se->avg.ఎక్కించు_కూడిక;

	se->avg.ఎక్కించు_కూడిక = runnable_కూడిక;
	se->avg.ఎక్కించు_avg = ఎక్కించు_avg;
	__update_sa(&cfs_rq->avg, ఎక్కించు, delta_avg, delta_కూడిక);
}

స్థిర అంతఃవరుస శూన్య కూడు_tg_cfs_propagate(నిర్మాణం cfs_rq *cfs_rq, దీర్ఘ runnable_కూడిక)
{
	cfs_rq->propagate = 1;
	cfs_rq->prop_runnable_కూడిక += runnable_కూడిక;
}

/* Update task and its cfs_rq load average */
స్థిర అంతఃవరుస పూర్ణాంకం propagate_entity_ఎక్కించు_avg(నిర్మాణం sched_entity *se)
{
	నిర్మాణం cfs_rq *cfs_rq, *gcfs_rq;

	ఒకవేళ (entity_is_task(se))
		తిరిగి 0;

	gcfs_rq = సమూహం_cfs_rq(se);
	ఒకవేళ (!gcfs_rq->propagate)
		తిరిగి 0;

	gcfs_rq->propagate = 0;

	cfs_rq = cfs_rq_యొక్క(se);

	కూడు_tg_cfs_propagate(cfs_rq, gcfs_rq->prop_runnable_కూడిక);

	update_tg_cfs_util(cfs_rq, se, gcfs_rq);
	update_tg_cfs_runnable(cfs_rq, se, gcfs_rq);
	update_tg_cfs_ఎక్కించు(cfs_rq, se, gcfs_rq);

	trace_pelt_cfs_tp(cfs_rq);
	trace_pelt_se_tp(se);

	తిరిగి 1;
}

/*
 * Check if we need to update the load and the utilization of a blocked
 * group_entity:
 */
స్థిర అంతఃవరుస bool skip_blocked_update(నిర్మాణం sched_entity *se)
{
	నిర్మాణం cfs_rq *gcfs_rq = సమూహం_cfs_rq(se);

	/*
	 * If sched_entity still have not zero load or utilization, we have to
	 * decay it:
	 */
	ఒకవేళ (se->avg.ఎక్కించు_avg || se->avg.util_avg)
		తిరిగి false;

	/*
	 * If there is a pending propagation, we have to update the load and
	 * the utilization of the sched_entity:
	 */
	ఒకవేళ (gcfs_rq->propagate)
		తిరిగి false;

	/*
	 * Otherwise, the load and the utilization of the sched_entity is
	 * already zero and there is no pending propagation, so it will be a
	 * waste of time to try to decay it:
	 */
	తిరిగి true;
}

#లేకపోతే /* !CONFIG_FAIR_GROUP_SCHED: */

స్థిర అంతఃవరుస శూన్య update_tg_ఎక్కించు_avg(నిర్మాణం cfs_rq *cfs_rq) {}

స్థిర అంతఃవరుస శూన్య clear_tg_offline_cfs_rqs(నిర్మాణం rq *rq) {}

స్థిర అంతఃవరుస పూర్ణాంకం propagate_entity_ఎక్కించు_avg(నిర్మాణం sched_entity *se)
{
	తిరిగి 0;
}

స్థిర అంతఃవరుస శూన్య కూడు_tg_cfs_propagate(నిర్మాణం cfs_rq *cfs_rq, దీర్ఘ runnable_కూడిక) {}

#అయితే_ముగింపు /* !CONFIG_FAIR_GROUP_SCHED */

#నిర్వచితమైతే CONFIG_NO_HZ_COMMON
స్థిర అంతఃవరుస శూన్య migrate_se_pelt_lag(నిర్మాణం sched_entity *se)
{
	u64 throttled = 0, now, lut;
	నిర్మాణం cfs_rq *cfs_rq;
	నిర్మాణం rq *rq;
	bool is_idle;

	ఒకవేళ (ఎక్కించు_avg_is_decayed(&se->avg))
		తిరిగి;

	cfs_rq = cfs_rq_యొక్క(se);
	rq = rq_యొక్క(cfs_rq);

	rcu_read_తాళం();
	is_idle = is_idle_task(rcu_dereference_all(rq->curr));
	rcu_read_unlock();

	/*
	 * The lag estimation comes with a cost we don't want to pay all the
	 * time. Hence, limiting to the case where the source CPU is idle and
	 * we know we are at the greatest risk to have an outdated clock.
	 */
	ఒకవేళ (!is_idle)
		తిరిగి;

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

#నిర్వచితమైతే CONFIG_CFS_BANDWIDTH
	throttled = u64_u32_ఎక్కించు(cfs_rq->throttled_pelt_idle);
	/* The clock has been stopped for throttling */
	ఒకవేళ (throttled == U64_MAX)
		తిరిగి;
#అయితే_ముగింపు
	now = u64_u32_ఎక్కించు(rq->clock_pelt_idle);
	/*
	 * Paired with _update_idle_rq_clock_pelt(). It ensures at the worst case
	 * is observed the old clock_pelt_idle value and the new clock_idle,
	 * which lead to an underestimation. The opposite would lead to an
	 * overestimation.
	 */
	smp_rmb();
	lut = cfs_rq_చివరి_update_సమయం(cfs_rq);

	now -= throttled;
	ఒకవేళ (now < lut)
		/*
		 * cfs_rq->avg.last_update_time is more recent than our
		 * estimation, let's use it.
		 */
		now = lut;
	లేకపోతే
		now += sched_clock_cpu(cpu_యొక్క(rq)) - u64_u32_ఎక్కించు(rq->clock_idle);

	__update_load_avg_blocked_se(now, se);
}
#లేకపోతే /* !CONFIG_NO_HZ_COMMON: */
స్థిర శూన్య migrate_se_pelt_lag(నిర్మాణం sched_entity *se) {}
#అయితే_ముగింపు /* !CONFIG_NO_HZ_COMMON */

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
స్థిర అంతఃవరుస పూర్ణాంకం
update_cfs_rq_ఎక్కించు_avg(u64 now, నిర్మాణం cfs_rq *cfs_rq)
{
	గుర్తులేని దీర్ఘ removed_ఎక్కించు = 0, removed_util = 0, removed_runnable = 0;
	నిర్మాణం sched_avg *sa = &cfs_rq->avg;
	పూర్ణాంకం decayed = 0;

	ఒకవేళ (cfs_rq->removed.nr) {
		గుర్తులేని దీర్ఘ r;
		u32 divider = get_pelt_divider(&cfs_rq->avg);

		raw_spin_తాళం(&cfs_rq->removed.తాళం);
		మార్పిడి(cfs_rq->removed.util_avg, removed_util);
		మార్పిడి(cfs_rq->removed.ఎక్కించు_avg, removed_ఎక్కించు);
		మార్పిడి(cfs_rq->removed.runnable_avg, removed_runnable);
		cfs_rq->removed.nr = 0;
		raw_spin_unlock(&cfs_rq->removed.తాళం);

		r = removed_ఎక్కించు;
		__update_sa(sa, ఎక్కించు, -r, -r*divider);

		r = removed_util;
		__update_sa(sa, util, -r, -r*divider);

		r = removed_runnable;
		__update_sa(sa, runnable, -r, -r*divider);

		/*
		 * removed_runnable is the unweighted version of removed_load so we
		 * can use it to estimate removed_load_sum.
		 */
		కూడు_tg_cfs_propagate(cfs_rq,
			-(దీర్ఘ)(removed_runnable * divider) >> SCHED_సామర్థ్యం_జరుపు);

		decayed = 1;
	}

	decayed |= __update_load_avg_cfs_rq(now, cfs_rq);
	u64_u32_store_copy(sa->చివరి_update_సమయం,
			   cfs_rq->చివరి_update_సమయం_copy,
			   sa->చివరి_update_సమయం);
	తిరిగి decayed;
}

/**
 * attach_entity_load_avg - attach this entity to its cfs_rq load avg
 * @cfs_rq: cfs_rq to attach to
 * @se: sched_entity to attach
 *
 * Must call update_cfs_rq_load_avg() before this, since we rely on
 * cfs_rq->avg.last_update_time being current.
 */
స్థిర శూన్య attach_entity_ఎక్కించు_avg(నిర్మాణం cfs_rq *cfs_rq, నిర్మాణం sched_entity *se)
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
	se->avg.చివరి_update_సమయం = cfs_rq->avg.చివరి_update_సమయం;
	se->avg.period_contrib = cfs_rq->avg.period_contrib;

	/*
	 * Hell(o) Nasty stuff.. we need to recompute _sum based on the new
	 * period_contrib. This isn't strictly correct, but since we're
	 * entirely outside of the PELT hierarchy, nobody cares if we truncate
	 * _sum a little.
	 */
	se->avg.util_కూడిక = se->avg.util_avg * divider;

	se->avg.runnable_కూడిక = se->avg.runnable_avg * divider;

	se->avg.ఎక్కించు_కూడిక = se->avg.ఎక్కించు_avg * divider;
	ఒకవేళ (se_బరువు(se) < se->avg.ఎక్కించు_కూడిక)
		se->avg.ఎక్కించు_కూడిక = div_u64(se->avg.ఎక్కించు_కూడిక, se_బరువు(se));
	లేకపోతే
		se->avg.ఎక్కించు_కూడిక = 1;

	వరుసలో_చేర్చు_ఎక్కించు_avg(cfs_rq, se);
	cfs_rq->avg.util_avg += se->avg.util_avg;
	cfs_rq->avg.util_కూడిక += se->avg.util_కూడిక;
	cfs_rq->avg.runnable_avg += se->avg.runnable_avg;
	cfs_rq->avg.runnable_కూడిక += se->avg.runnable_కూడిక;

	కూడు_tg_cfs_propagate(cfs_rq, se->avg.ఎక్కించు_కూడిక);

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
స్థిర శూన్య detach_entity_ఎక్కించు_avg(నిర్మాణం cfs_rq *cfs_rq, నిర్మాణం sched_entity *se)
{
	వరుస_నుండి_తీయి_ఎక్కించు_avg(cfs_rq, se);
	__update_sa(&cfs_rq->avg, util, -se->avg.util_avg, -se->avg.util_కూడిక);
	__update_sa(&cfs_rq->avg, runnable, -se->avg.runnable_avg, -se->avg.runnable_కూడిక);

	కూడు_tg_cfs_propagate(cfs_rq, -se->avg.ఎక్కించు_కూడిక);

	cfs_rq_util_change(cfs_rq, 0);

	trace_pelt_cfs_tp(cfs_rq);
}

#నిర్వచించు UTIL_EST_MARGIN (SCHED_సామర్థ్యం_SCALE / 100)

స్థిర అంతఃవరుస శూన్య util_est_update(నిర్మాణం sched_entity *se)
{
	గుర్తులేని పూర్ణాంకం ewma, dequeued, చివరి_ewma_diff;

	ఒకవేళ (!sched_feat(UTIL_EST))
		తిరిగి;

	/* Get current estimate of utilization */
	ewma = READ_ONCE(se->avg.util_est);

	/*
	 * If the PELT values haven't changed since enqueue time,
	 * skip the util_est update.
	 */
	ఒకవేళ (ewma & UTIL_AVG_UNCHANGED)
		తిరిగి;

	/* Get utilization at dequeue */
	dequeued = READ_ONCE(se->avg.util_avg);

	/*
	 * Reset EWMA on utilization increases, the moving average is used only
	 * to smooth utilization decreases.
	 */
	ఒకవేళ (ewma <= dequeued) {
		ewma = dequeued;
		వెళ్ళు పూర్తి;
	}

	/*
	 * Skip update of task's estimated utilization when its members are
	 * already ~1% close to its last activation value.
	 */
	చివరి_ewma_diff = ewma - dequeued;
	ఒకవేళ (చివరి_ewma_diff < UTIL_EST_MARGIN)
		వెళ్ళు పూర్తి;

	/*
	 * To avoid underestimate of task utilization, skip updates of EWMA if
	 * we cannot grant that thread got all CPU time it wanted.
	 */
	ఒకవేళ ((dequeued + UTIL_EST_MARGIN) < READ_ONCE(se->avg.runnable_avg))
		వెళ్ళు పూర్తి;

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
	ewma <<= UTIL_EST_బరువు_జరుపు;
	ewma  -= చివరి_ewma_diff;
	ewma >>= UTIL_EST_బరువు_జరుపు;
పూర్తి:
	ewma |= UTIL_AVG_UNCHANGED;
	WRITE_ONCE(se->avg.util_est, ewma);

	trace_sched_util_est_se_tp(se);
}

/*
 * Optional action to be done while updating the load average
 */
#నిర్వచించు UPDATE_TG	0x01
#నిర్వచించు SKIP_AGE_ఎక్కించు	0x02
#నిర్వచించు DO_ATTACH	0x04
#నిర్వచించు DO_DETACH	0x08
#నిర్వచించు UPDATE_UTIL_EST	0x10

/* Update task and its cfs_rq load average */
స్థిర అంతఃవరుస శూన్య update_ఎక్కించు_avg(నిర్మాణం cfs_rq *cfs_rq, నిర్మాణం sched_entity *se, పూర్ణాంకం flags)
{
	u64 now = cfs_rq_clock_pelt(cfs_rq);
	పూర్ణాంకం decayed;

	/*
	 * Track task load average for carrying it to new CPU after migrated, and
	 * track group sched_entity load average for task_h_load calculation in migration
	 */
	ఒకవేళ (se->avg.చివరి_update_సమయం && !(flags & SKIP_AGE_ఎక్కించు))
		__update_load_avg_se(now, cfs_rq, se);

	decayed  = update_cfs_rq_ఎక్కించు_avg(now, cfs_rq);
	decayed |= propagate_entity_ఎక్కించు_avg(se);

	ఒకవేళ (!se->avg.చివరి_update_సమయం && (flags & DO_ATTACH)) {

		/*
		 * DO_ATTACH means we're here from enqueue_entity().
		 * !last_update_time means we've passed through
		 * migrate_task_rq_fair() indicating we migrated.
		 *
		 * IOW we're enqueueing a task on a new CPU.
		 */
		attach_entity_ఎక్కించు_avg(cfs_rq, se);
		update_tg_ఎక్కించు_avg(cfs_rq);

	} లేకపోతే ఒకవేళ (flags & DO_DETACH) {
		/*
		 * DO_DETACH means we're here from dequeue_entity()
		 * and we are migrating task out of the CPU.
		 */
		detach_entity_ఎక్కించు_avg(cfs_rq, se);
		update_tg_ఎక్కించు_avg(cfs_rq);
	} లేకపోతే ఒకవేళ (decayed) {
		cfs_rq_util_change(cfs_rq, 0);

		ఒకవేళ (flags & UPDATE_TG)
			update_tg_ఎక్కించు_avg(cfs_rq);
	}

	ఒకవేళ (flags & UPDATE_UTIL_EST)
		util_est_update(se);
}

/*
 * Synchronize entity load avg of dequeued entity without locking
 * the previous rq.
 */
స్థిర శూన్య sync_entity_ఎక్కించు_avg(నిర్మాణం sched_entity *se)
{
	నిర్మాణం cfs_rq *cfs_rq = cfs_rq_యొక్క(se);
	u64 చివరి_update_సమయం;

	చివరి_update_సమయం = cfs_rq_చివరి_update_సమయం(cfs_rq);
	__update_load_avg_blocked_se(చివరి_update_సమయం, se);
}

/*
 * Task first catches up with cfs_rq, and then subtract
 * itself from the cfs_rq (task must be off the queue now).
 */
స్థిర శూన్య remove_entity_ఎక్కించు_avg(నిర్మాణం sched_entity *se)
{
	నిర్మాణం cfs_rq *cfs_rq = cfs_rq_యొక్క(se);
	గుర్తులేని దీర్ఘ flags;

	/*
	 * tasks cannot exit without having gone through wake_up_new_task() ->
	 * enqueue_task_fair() which will have added things to the cfs_rq,
	 * so we can remove unconditionally.
	 */

	sync_entity_ఎక్కించు_avg(se);

	raw_spin_తాళం_irqsave(&cfs_rq->removed.తాళం, flags);
	++cfs_rq->removed.nr;
	cfs_rq->removed.util_avg	+= se->avg.util_avg;
	cfs_rq->removed.ఎక్కించు_avg	+= se->avg.ఎక్కించు_avg;
	cfs_rq->removed.runnable_avg	+= se->avg.runnable_avg;
	raw_spin_unlock_irqrestore(&cfs_rq->removed.తాళం, flags);
}

స్థిర అంతఃవరుస గుర్తులేని దీర్ఘ cfs_rq_runnable_avg(నిర్మాణం cfs_rq *cfs_rq)
{
	తిరిగి cfs_rq->avg.runnable_avg;
}

స్థిర అంతఃవరుస గుర్తులేని దీర్ఘ cfs_rq_ఎక్కించు_avg(నిర్మాణం cfs_rq *cfs_rq)
{
	తిరిగి cfs_rq->avg.ఎక్కించు_avg;
}

స్థిర పూర్ణాంకం sched_balance_newidle(నిర్మాణం rq *this_rq, నిర్మాణం rq_flags *rf)
	__must_hold(__rq_lockp(this_rq));

స్థిర అంతఃవరుస గుర్తులేని దీర్ఘ task_util(నిర్మాణం task_struct *p)
{
	తిరిగి READ_ONCE(p->se.avg.util_avg);
}

స్థిర అంతఃవరుస గుర్తులేని దీర్ఘ _task_util_est(నిర్మాణం task_struct *p)
{
	తిరిగి READ_ONCE(p->se.avg.util_est) & ~UTIL_AVG_UNCHANGED;
}

స్థిర అంతఃవరుస గుర్తులేని దీర్ఘ task_util_est(నిర్మాణం task_struct *p)
{
	తిరిగి max(task_util(p), _task_util_est(p));
}

స్థిర అంతఃవరుస శూన్య util_est_వరుసలో_చేర్చు(నిర్మాణం cfs_rq *cfs_rq,
				    నిర్మాణం task_struct *p)
{
	గుర్తులేని పూర్ణాంకం enqueued;

	ఒకవేళ (!sched_feat(UTIL_EST))
		తిరిగి;

	/* Update root cfs_rq's estimated utilization */
	enqueued  = cfs_rq->avg.util_est;
	enqueued += _task_util_est(p);
	WRITE_ONCE(cfs_rq->avg.util_est, enqueued);

	trace_sched_util_est_cfs_tp(cfs_rq);
}

స్థిర అంతఃవరుస శూన్య util_est_వరుస_నుండి_తీయి(నిర్మాణం cfs_rq *cfs_rq,
				    నిర్మాణం task_struct *p)
{
	గుర్తులేని పూర్ణాంకం enqueued;

	ఒకవేళ (!sched_feat(UTIL_EST))
		తిరిగి;

	/* Update root cfs_rq's estimated utilization */
	enqueued  = cfs_rq->avg.util_est;
	enqueued -= min_t(గుర్తులేని పూర్ణాంకం, enqueued, _task_util_est(p));
	WRITE_ONCE(cfs_rq->avg.util_est, enqueued);

	trace_sched_util_est_cfs_tp(cfs_rq);
}

స్థిర అంతఃవరుస గుర్తులేని దీర్ఘ get_actual_cpu_సామర్థ్యం(పూర్ణాంకం cpu)
{
	గుర్తులేని దీర్ఘ సామర్థ్యం = arch_scale_cpu_సామర్థ్యం(cpu);

	సామర్థ్యం -= max(hw_ఎక్కించు_avg(cpu_rq(cpu)), cpufreq_get_pressure(cpu));

	తిరిగి సామర్థ్యం;
}

స్థిర అంతఃవరుస పూర్ణాంకం util_సరిపోతుంది_cpu(గుర్తులేని దీర్ఘ util,
				గుర్తులేని దీర్ఘ uclamp_min,
				గుర్తులేని దీర్ఘ uclamp_max,
				పూర్ణాంకం cpu)
{
	గుర్తులేని దీర్ఘ సామర్థ్యం = సామర్థ్యం_యొక్క(cpu);
	గుర్తులేని దీర్ఘ సామర్థ్యం_orig;
	bool సరిపోతుంది, uclamp_max_సరిపోతుంది;

	/*
	 * Check if the real util fits without any uclamp boost/cap applied.
	 */
	సరిపోతుంది = సరిపోతుంది_సామర్థ్యం(util, సామర్థ్యం);

	ఒకవేళ (!uclamp_is_వాడిన())
		తిరిగి సరిపోతుంది;

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
	సామర్థ్యం_orig = arch_scale_cpu_సామర్థ్యం(cpu);

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
	uclamp_max_సరిపోతుంది = (సామర్థ్యం_orig == SCHED_సామర్థ్యం_SCALE) && (uclamp_max == SCHED_సామర్థ్యం_SCALE);
	uclamp_max_సరిపోతుంది = !uclamp_max_సరిపోతుంది && (uclamp_max <= సామర్థ్యం_orig);
	సరిపోతుంది = సరిపోతుంది || uclamp_max_సరిపోతుంది;

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
	ఒకవేళ (సరిపోతుంది && (util < uclamp_min) &&
	    (uclamp_min > get_actual_cpu_సామర్థ్యం(cpu)))
		తిరిగి -1;

	తిరిగి సరిపోతుంది;
}

స్థిర అంతఃవరుస పూర్ణాంకం task_సరిపోతుంది_cpu(నిర్మాణం task_struct *p, పూర్ణాంకం cpu)
{
	గుర్తులేని దీర్ఘ uclamp_min = uclamp_eff_విలువ(p, UCLAMP_MIN);
	గుర్తులేని దీర్ఘ uclamp_max = uclamp_eff_విలువ(p, UCLAMP_MAX);
	గుర్తులేని దీర్ఘ util = task_util_est(p);
	/*
	 * Return true only if the cpu fully fits the task requirements, which
	 * include the utilization but also the performance hints.
	 */
	తిరిగి (util_సరిపోతుంది_cpu(util, uclamp_min, uclamp_max, cpu) > 0);
}

స్థిర అంతఃవరుస శూన్య update_misfit_స్థితి(నిర్మాణం task_struct *p, నిర్మాణం rq *rq)
{
	పూర్ణాంకం cpu = cpu_యొక్క(rq);

	ఒకవేళ (!sched_asym_cpucap_చురుకైన())
		తిరిగి;

	/*
	 * Affinity allows us to go somewhere higher?  Or are we on biggest
	 * available CPU already? Or do we fit into this CPU ?
	 */
	ఒకవేళ (!p || (p->nr_cpus_allowed == 1) ||
	    (arch_scale_cpu_సామర్థ్యం(cpu) == p->max_allowed_సామర్థ్యం) ||
	    task_సరిపోతుంది_cpu(p, cpu)) {

		rq->misfit_task_ఎక్కించు = 0;
		తిరిగి;
	}

	/*
	 * Make sure that misfit_task_load will not be null even if
	 * task_h_load() returns 0.
	 */
	rq->misfit_task_ఎక్కించు = max_t(గుర్తులేని దీర్ఘ, task_h_ఎక్కించు(p), 1);
}

శూన్య __setparam_fair(నిర్మాణం task_struct *p, స్థిరరాశి నిర్మాణం sched_attr *attr)
{
	నిర్మాణం sched_entity *se = &p->se;

	p->static_prio = NICE_గమ్యం_PRIO(attr->sched_nice);
	ఒకవేళ (attr->sched_runtime) {
		se->custom_ముక్క = 1;
		se->ముక్క = clamp_t(u64, attr->sched_runtime,
				      NSEC_చొప్పున_MSEC/10,   /* HZ=1000 * 10 */
				      NSEC_చొప్పున_MSEC*100); /* HZ=100  / 10 */
	} లేకపోతే {
		se->custom_ముక్క = 0;
		se->ముక్క = sysctl_sched_పునాది_ముక్క;
	}
}

స్థిర శూన్య
ఉంచు_entity(నిర్మాణం cfs_rq *cfs_rq, నిర్మాణం sched_entity *se, పూర్ణాంకం flags)
{
	u64 vslice, vruntime = avg_vruntime(cfs_rq);
	గుర్తులేని పూర్ణాంకం nr_queued = cfs_rq->h_nr_queued;
	bool update_zero = false;
	s64 lag = 0;

	ఒకవేళ (!se->custom_ముక్క)
		se->ముక్క = sysctl_sched_పునాది_ముక్క;
	vslice = calc_delta_fair(se->ముక్క, se);

	ఒకవేళ (flags & వరుసలో_చేర్చు_QUEUED)
		nr_queued -= 1;

	/*
	 * Due to how V is constructed as the weighted average of entities,
	 * adding tasks with positive lag, or removing tasks with negative lag
	 * will move 'time' backwards, this can screw around with the lag of
	 * other tasks.
	 *
	 * EEVDF: placement strategy #1 / #2
	 */
	ఒకవేళ (sched_feat(ఉంచు_LAG) && nr_queued && se->vlag) {
		నిర్మాణం sched_entity *curr = cfs_rq->curr;
		దీర్ఘ ఎక్కించు, బరువు;

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
		ఎక్కించు = cfs_rq->కూడిక_బరువు;
		ఒకవేళ (curr && curr->on_rq)
			ఎక్కించు += avg_vruntime_బరువు(cfs_rq, curr->h_ఎక్కించు.బరువు);
		ఒకవేళ (WARN_ON_ONCE(!ఎక్కించు))
			ఎక్కించు = 1;

		బరువు = avg_vruntime_బరువు(cfs_rq, se->h_ఎక్కించు.బరువు);

		/*
		 * Rewrite, because the product (W + w_i)*vl'_i might
		 * overflow s64:
		 *
		 *   vl_i = (W + w_i)*vl'_i / W
		 *        = (W*vl'_i + w_i*vl'_i) / W
		 *        = vl'_i + w_i*vl'_i/W
		 */
		lag += div64_long(lag * బరువు, ఎక్కించు);

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
		ఒకవేళ (బరువు > ఎక్కించు)
			update_zero = true;
	}

	se->vruntime = vruntime - lag;

	ఒకవేళ (update_zero)
		update_zero_vruntime(cfs_rq, -lag);

	ఒకవేళ (sched_feat(ఉంచు_REL_DEADLINE) && se->rel_deadline) {
		se->deadline += se->vruntime;
		se->rel_deadline = 0;
		తిరిగి;
	}

	/*
	 * When joining the competition; the existing tasks will be,
	 * on average, halfway through their slice, as such start tasks
	 * off with half a slice to ease into the competition.
	 */
	ఒకవేళ (sched_feat(ఉంచు_DEADLINE_INITIAL) && (flags & వరుసలో_చేర్చు_INITIAL))
		vslice /= 2;

	/*
	 * EEVDF: vd_i = ve_i + r_i/w_i
	 */
	se->deadline = se->vruntime + vslice;
}

స్థిర శూన్య check_వరుసలో_చేర్చు_throttle(నిర్మాణం cfs_rq *cfs_rq);
స్థిర అంతఃవరుస పూర్ణాంకం cfs_rq_throttled(నిర్మాణం cfs_rq *cfs_rq);

స్థిర శూన్య
వరుసలో_చేర్చు_entity(నిర్మాణం cfs_rq *cfs_rq, నిర్మాణం sched_entity *se, పూర్ణాంకం flags)
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
	update_ఎక్కించు_avg(cfs_rq, se, UPDATE_TG | DO_ATTACH);
	se_update_runnable(se);
	/*
	 * XXX update_load_avg() above will have attached us to the pelt sum;
	 * but update_cfs_group() here will re-adjust the weight and have to
	 * undo/redo all that. Seems wasteful.
	 */
	update_cfs_సమూహం(se);

	account_entity_వరుసలో_చేర్చు(cfs_rq, se);

	/* Entity has migrated, no longer consider this task hot */
	ఒకవేళ (flags & వరుసలో_చేర్చు_MIGRATED)
		se->exec_ప్రారంభం = 0;

	check_schedstat_required();
	update_stats_వరుసలో_చేర్చు_fair(cfs_rq, se, flags);
	se->on_rq = 1;

	ఒకవేళ (cfs_rq->nr_queued == 1) {
		check_వరుసలో_చేర్చు_throttle(cfs_rq);
		list_కూడు_ఆకు_cfs_rq(cfs_rq);
#నిర్వచితమైతే CONFIG_CFS_BANDWIDTH
		ఒకవేళ (cfs_rq->pelt_clock_throttled) {
			నిర్మాణం rq *rq = rq_యొక్క(cfs_rq);

			cfs_rq->throttled_clock_pelt_సమయం += rq_clock_pelt(rq) -
				cfs_rq->throttled_clock_pelt;
			cfs_rq->pelt_clock_throttled = 0;
		}
#అయితే_ముగింపు
	}
}

స్థిర శూన్య సమితి_తదుపరి_buddy(నిర్మాణం cfs_rq *cfs_rq, నిర్మాణం sched_entity *se)
{
	ఒకవేళ (WARN_ON_ONCE(!se->on_rq || se->sched_delayed))
		తిరిగి;
	ఒకవేళ (se_is_idle(se))
		తిరిగి;
	cfs_rq->తదుపరి = se;
}

స్థిర శూన్య clear_buddies(నిర్మాణం cfs_rq *cfs_rq, నిర్మాణం sched_entity *se)
{
	ఒకవేళ (cfs_rq->తదుపరి == se)
		cfs_rq->తదుపరి = ఖాళీ;
}

స్థిర __always_inline శూన్య return_cfs_rq_runtime(నిర్మాణం cfs_rq *cfs_rq);

స్థిర శూన్య సమితి_delayed(నిర్మాణం sched_entity *se)
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
	ఒకవేళ (!entity_is_task(se)) {
		se->sched_delayed = 1;
		తిరిగి;
	}

	/*
	 * Drop a task leaving the runnable set.
	 * Needs to be called before sched_delayed is set.
	 * clear_delayed() mirrors this after clearing the flag.
	 */
	pref_llc_running_dec(rq_యొక్క(cfs_rq_యొక్క(se)), task_యొక్క(se));
	se->sched_delayed = 1;

	for_each_sched_entity(se) {
		నిర్మాణం cfs_rq *cfs_rq = cfs_rq_యొక్క(se);

		cfs_rq->h_nr_runnable--;
	}
}

స్థిర శూన్య clear_delayed(నిర్మాణం sched_entity *se)
{
	se->sched_delayed = 0;

	/*
	 * Delayed se of cfs_rq have no tasks queued on them.
	 * Do not adjust h_nr_runnable since a dequeue has
	 * already accounted for it or an enqueue of a task
	 * below it will account for it in enqueue_task_fair().
	 */
	ఒకవేళ (!entity_is_task(se))
		తిరిగి;

	/*
	 * Re-add on wake, after sched_delayed is cleared. On a final delayed
	 * dequeue account_llc_dequeue() already cleared pref_llc_queued, so
	 * this does nothing.
	 */
	pref_llc_running_inc(rq_యొక్క(cfs_rq_యొక్క(se)), task_యొక్క(se));

	for_each_sched_entity(se) {
		నిర్మాణం cfs_rq *cfs_rq = cfs_rq_యొక్క(se);

		cfs_rq->h_nr_runnable++;
	}
}

స్థిర శూన్య
వరుస_నుండి_తీయి_entity(నిర్మాణం cfs_rq *cfs_rq, నిర్మాణం sched_entity *se, పూర్ణాంకం flags)
{
	పూర్ణాంకం action = UPDATE_TG;

	ఒకవేళ (entity_is_task(se)) {
		ఒకవేళ (task_on_rq_migrating(task_యొక్క(se)))
			action |= DO_DETACH;

		ఒకవేళ ((flags & వరుస_నుండి_తీయి_SLEEP) && !(flags & వరుస_నుండి_తీయి_DELAYED))
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
	update_ఎక్కించు_avg(cfs_rq, se, action);
	se_update_runnable(se);

	update_stats_వరుస_నుండి_తీయి_fair(cfs_rq, se, flags);

	se->on_rq = 0;
	account_entity_వరుస_నుండి_తీయి(cfs_rq, se);

	/* return excess runtime on last dequeue */
	return_cfs_rq_runtime(cfs_rq);

	update_cfs_సమూహం(se);

	ఒకవేళ (cfs_rq->nr_queued == 0) {
		update_idle_cfs_rq_clock_pelt(cfs_rq);
#నిర్వచితమైతే CONFIG_CFS_BANDWIDTH
		ఒకవేళ (throttled_hierarchy(cfs_rq)) {
			నిర్మాణం rq *rq = rq_యొక్క(cfs_rq);

			list_del_ఆకు_cfs_rq(cfs_rq);
			cfs_rq->throttled_clock_pelt = rq_clock_pelt(rq);
			cfs_rq->pelt_clock_throttled = 1;
		}
#అయితే_ముగింపు
	}
}

స్థిర శూన్య
సమితి_తదుపరి_entity(నిర్మాణం cfs_rq *cfs_rq, నిర్మాణం sched_entity *se)
{
	/* 'current' is not kept within the tree. */
	ఒకవేళ (se->on_rq) {
		/*
		 * Any task has to be enqueued before it get to execute on
		 * a CPU. So account for the time it spent waiting on the
		 * runqueue.
		 */
		update_stats_wait_చివర_fair(cfs_rq, se);
		update_ఎక్కించు_avg(cfs_rq, se, UPDATE_TG);
	}

	update_stats_curr_ప్రారంభం(cfs_rq, se);
	WARN_ON_ONCE(cfs_rq->h_curr);
	cfs_rq->h_curr = se;

	/*
	 * Track our maximum slice length, if the CPU's load is at
	 * least twice that of our own weight (i.e. don't track it
	 * when there are only lesser-weight tasks around):
	 */
	ఒకవేళ (schedstat_enabled() &&
	    rq_యొక్క(cfs_rq)->cfs.ఎక్కించు.బరువు >= 2*se->ఎక్కించు.బరువు) {
		నిర్మాణం sched_statistics *stats;

		stats = __schedstats_from_se(se);
		__schedstat_set(stats->ముక్క_max,
				max((u64)stats->ముక్క_max,
				    se->కూడిక_exec_runtime - se->prev_కూడిక_exec_runtime));
	}

	se->prev_కూడిక_exec_runtime = se->కూడిక_exec_runtime;
}

స్థిర bool __dequeue_task(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, పూర్ణాంకం flags);

స్థిర నిర్మాణం sched_entity *
pick_తదుపరి_entity(నిర్మాణం rq *rq, bool protect)
{
	నిర్మాణం cfs_rq *cfs_rq = &rq->cfs;
	నిర్మాణం sched_entity *se;

	se = pick_eevdf(cfs_rq, protect);
	ఒకవేళ (se->sched_delayed) {
		__dequeue_task(rq, task_యొక్క(se), వరుస_నుండి_తీయి_SLEEP | వరుస_నుండి_తీయి_DELAYED);
		/*
		 * Must not reference @se again, see __block_task().
		 */
		తిరిగి ఖాళీ;
	}
	తిరిగి se;
}

స్థిర శూన్య put_prev_entity(నిర్మాణం cfs_rq *cfs_rq, నిర్మాణం sched_entity *prev)
{
	/*
	 * If still on the runqueue then deactivate_task()
	 * was not called and update_curr() has to be done:
	 */
	ఒకవేళ (prev->on_rq)
		update_curr(cfs_rq);

	ఒకవేళ (prev->on_rq) {
		update_stats_wait_ప్రారంభం_fair(cfs_rq, prev);
		/* in !on_rq case, update occurred at dequeue */
		update_ఎక్కించు_avg(cfs_rq, prev, 0);
	}
	WARN_ON_ONCE(cfs_rq->h_curr != prev);
	cfs_rq->h_curr = ఖాళీ;
}

స్థిర శూన్య
entity_tick(నిర్మాణం cfs_rq *cfs_rq, నిర్మాణం sched_entity *curr, పూర్ణాంకం queued)
{
	/*
	 * Update run-time statistics of the 'current'.
	 */
	update_curr(cfs_rq);

	/*
	 * Ensure that runnable average is periodically updated.
	 */
	update_ఎక్కించు_avg(cfs_rq, curr, UPDATE_TG);
	update_cfs_సమూహం(curr);

#నిర్వచితమైతే CONFIG_SCHED_HRTICK
	/*
	 * queued ticks are scheduled to match the slice, so don't bother
	 * validating it and just reschedule.
	 */
	ఒకవేళ (queued) {
		resched_curr(rq_యొక్క(cfs_rq));
		తిరిగి;
	}
#అయితే_ముగింపు
}


/**************************************************
 * CFS bandwidth control machinery
 */

#నిర్వచితమైతే CONFIG_CFS_BANDWIDTH

#నిర్వచితమైతే CONFIG_JUMP_LABEL
స్థిర నిర్మాణం static_కీలకం __cfs_bandwidth_used;

స్థిర అంతఃవరుస bool cfs_bandwidth_వాడిన(శూన్య)
{
	తిరిగి static_కీలకం_false(&__cfs_bandwidth_used);
}

శూన్య cfs_bandwidth_usage_inc(శూన్య)
{
	static_కీలకం_slow_inc_cpuslocked(&__cfs_bandwidth_used);
}

శూన్య cfs_bandwidth_usage_dec(శూన్య)
{
	static_కీలకం_slow_dec_cpuslocked(&__cfs_bandwidth_used);
}
#లేకపోతే /* !CONFIG_JUMP_LABEL: */
స్థిర bool cfs_bandwidth_వాడిన(శూన్య)
{
	తిరిగి true;
}

శూన్య cfs_bandwidth_usage_inc(శూన్య) {}
శూన్య cfs_bandwidth_usage_dec(శూన్య) {}
#అయితే_ముగింపు /* !CONFIG_JUMP_LABEL */

స్థిర అంతఃవరుస u64 sched_cfs_bandwidth_ముక్క(శూన్య)
{
	తిరిగి (u64)sysctl_sched_cfs_bandwidth_ముక్క * NSEC_చొప్పున_USEC;
}

/*
 * Replenish runtime according to assigned quota. We use sched_clock_cpu
 * directly instead of rq->clock to avoid adding additional synchronization
 * around rq->lock.
 *
 * requires cfs_b->lock
 */
శూన్య __refill_cfs_bandwidth_runtime(నిర్మాణం cfs_bandwidth *cfs_b)
{
	s64 runtime;

	ఒకవేళ (unlikely(cfs_b->quota == RUNTIME_INF))
		తిరిగి;

	cfs_b->runtime += cfs_b->quota;
	runtime = cfs_b->runtime_snap - cfs_b->runtime;
	ఒకవేళ (runtime > 0) {
		cfs_b->వ్యవధి_సమయం += runtime;
		cfs_b->nr_వ్యవధి++;
	}

	cfs_b->runtime = min(cfs_b->runtime, cfs_b->quota + cfs_b->వ్యవధి);
	cfs_b->runtime_snap = cfs_b->runtime;
}

స్థిర అంతఃవరుస నిర్మాణం cfs_bandwidth *tg_cfs_bandwidth(నిర్మాణం task_సమూహం *tg)
{
	తిరిగి &tg->cfs_bandwidth;
}

/* returns 0 on failure to allocate runtime */
స్థిర పూర్ణాంకం __assign_cfs_rq_runtime(నిర్మాణం cfs_bandwidth *cfs_b,
				   నిర్మాణం cfs_rq *cfs_rq, u64 లక్ష్యం_runtime)
{
	u64 min_amount, amount = 0;

	lockdep_assert_held(&cfs_b->తాళం);

	/* note: this is a positive sum as runtime_remaining <= 0 */
	min_amount = లక్ష్యం_runtime - cfs_rq->runtime_మిగిలిన;

	ఒకవేళ (cfs_b->quota == RUNTIME_INF)
		amount = min_amount;
	లేకపోతే {
		ప్రారంభం_cfs_bandwidth(cfs_b);

		ఒకవేళ (cfs_b->runtime > 0) {
			amount = min(cfs_b->runtime, min_amount);
			cfs_b->runtime -= amount;
			cfs_b->idle = 0;
		}
	}

	cfs_rq->runtime_మిగిలిన += amount;

	తిరిగి cfs_rq->runtime_మిగిలిన > 0;
}

స్థిర bool throttle_cfs_rq(నిర్మాణం cfs_rq *cfs_rq);

స్థిర bool __account_cfs_rq_runtime(నిర్మాణం cfs_rq *cfs_rq, u64 delta_exec)
{
	/* dock delta_exec before expiring quota (as it could span periods) */
	cfs_rq->runtime_మిగిలిన -= delta_exec;

	ఒకవేళ (likely(cfs_rq->runtime_మిగిలిన > 0))
		తిరిగి false;

	ఒకవేళ (cfs_rq->throttled)
		తిరిగి true;
	/*
	 * throttle_cfs_rq() will try to extend the runtime first
	 * before throttling the hierarchy.
	 */
	తిరిగి throttle_cfs_rq(cfs_rq);
}

స్థిర __always_inline
bool account_cfs_rq_runtime(నిర్మాణం cfs_rq *cfs_rq, u64 delta_exec)
{
	ఒకవేళ (!cfs_bandwidth_వాడిన() || !cfs_rq->runtime_enabled)
		తిరిగి false;

	తిరిగి __account_cfs_rq_runtime(cfs_rq, delta_exec);
}

స్థిర అంతఃవరుస పూర్ణాంకం cfs_rq_throttled(నిర్మాణం cfs_rq *cfs_rq)
{
	తిరిగి cfs_bandwidth_వాడిన() && cfs_rq->throttled;
}

స్థిర అంతఃవరుస bool cfs_rq_pelt_clock_throttled(నిర్మాణం cfs_rq *cfs_rq)
{
	తిరిగి cfs_bandwidth_వాడిన() && cfs_rq->pelt_clock_throttled;
}

/* check whether cfs_rq, or any parent, is throttled */
స్థిర అంతఃవరుస పూర్ణాంకం throttled_hierarchy(నిర్మాణం cfs_rq *cfs_rq)
{
	తిరిగి cfs_bandwidth_వాడిన() && cfs_rq->throttle_లెక్క;
}

స్థిర అంతఃవరుస పూర్ణాంకం lb_throttled_hierarchy(నిర్మాణం task_struct *p, పూర్ణాంకం dst_cpu)
{
	తిరిగి throttled_hierarchy(tg_cfs_rq(task_సమూహం(p), dst_cpu));
}

స్థిర అంతఃవరుస bool task_is_throttled(నిర్మాణం task_struct *p)
{
	తిరిగి cfs_bandwidth_వాడిన() && p->throttled;
}

స్థిర bool వరుస_నుండి_తీయి_task_fair(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, పూర్ణాంకం flags);
స్థిర శూన్య throttle_cfs_rq_work(నిర్మాణం callback_తల *work)
{
	నిర్మాణం task_struct *p = container_యొక్క(work, నిర్మాణం task_struct, sched_throttle_work);
	నిర్మాణం sched_entity *se;
	నిర్మాణం cfs_rq *cfs_rq;
	నిర్మాణం rq *rq;

	WARN_ON_ONCE(p != current);
	p->sched_throttle_work.తదుపరి = &p->sched_throttle_work;

	/*
	 * If task is exiting, then there won't be a return to userspace, so we
	 * don't have to bother with any of this.
	 */
	ఒకవేళ ((p->flags & PF_EXITING))
		తిరిగి;

	scoped_guard(task_rq_తాళం_2, p) {
		se = &p->se;
		cfs_rq = cfs_rq_యొక్క(se);

		/* Raced, forget */
		ఒకవేళ (p->sched_class != &fair_sched_class)
			తిరిగి;

		/*
		 * If not in limbo, then either replenish has happened or this
		 * task got migrated out of the throttled cfs_rq, move along.
		 */
		ఒకవేళ (!cfs_rq->throttle_లెక్క)
			తిరిగి;
		rq = scope.rq;
		update_rq_clock(rq);
		WARN_ON_ONCE(p->throttled || !list_ఖాళీలు(&p->throttle_కణుపు));
		వరుస_నుండి_తీయి_task_fair(rq, p, వరుస_నుండి_తీయి_SLEEP | వరుస_నుండి_తీయి_THROTTLE);
		list_కూడు(&p->throttle_కణుపు, &cfs_rq->throttled_limbo_list);
		/*
		 * Must not set throttled before dequeue or dequeue will
		 * mistakenly regard this task as an already throttled one.
		 */
		p->throttled = true;
		resched_curr(rq);
	}
}

శూన్య init_cfs_throttle_work(నిర్మాణం task_struct *p)
{
	init_task_work(&p->sched_throttle_work, throttle_cfs_rq_work);
	/* Protect against double add, see throttle_cfs_rq() and throttle_cfs_rq_work() */
	p->sched_throttle_work.తదుపరి = &p->sched_throttle_work;
	INIT_LIST_తల(&p->throttle_కణుపు);
}

/*
 * Task is throttled and someone wants to dequeue it again:
 * it could be sched/core when core needs to do things like
 * task affinity change, task group change, task sched class
 * change etc. and in these cases, DEQUEUE_SLEEP is not set;
 * or the task is blocked after throttled due to freezer etc.
 * and in these cases, DEQUEUE_SLEEP is set.
 */
స్థిర శూన్య detach_task_cfs_rq(నిర్మాణం task_struct *p);
స్థిర శూన్య వరుస_నుండి_తీయి_throttled_task(నిర్మాణం task_struct *p, పూర్ణాంకం flags)
{
	WARN_ON_ONCE(p->se.on_rq);
	list_del_init(&p->throttle_కణుపు);

	/* task blocked after throttled */
	ఒకవేళ (flags & వరుస_నుండి_తీయి_SLEEP) {
		p->throttled = false;
		తిరిగి;
	}

	/*
	 * task is migrating off its old cfs_rq, detach
	 * the task's load from its old cfs_rq.
	 */
	ఒకవేళ (task_on_rq_migrating(p))
		detach_task_cfs_rq(p);
}

స్థిర bool వరుసలో_చేర్చు_throttled_task(నిర్మాణం task_struct *p)
{
	నిర్మాణం cfs_rq *cfs_rq = cfs_rq_యొక్క(&p->se);

	/* @p should have gone through dequeue_throttled_task() first */
	WARN_ON_ONCE(!list_ఖాళీలు(&p->throttle_కణుపు));

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
	ఒకవేళ (throttled_hierarchy(cfs_rq) &&
	    !task_current_donor(rq_యొక్క(cfs_rq), p)) {
		list_కూడు(&p->throttle_కణుపు, &cfs_rq->throttled_limbo_list);
		తిరిగి true;
	}

	/* we can't take the fast path, do an actual enqueue*/
	p->throttled = false;
	తిరిగి false;
}

స్థిర శూన్య వరుసలో_చేర్చు_task_fair(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, పూర్ణాంకం flags);
స్థిర పూర్ణాంకం tg_unthrottle_పైకి(నిర్మాణం task_సమూహం *tg, శూన్య *దత్తాంశం)
{
	నిర్మాణం rq *rq = దత్తాంశం;
	నిర్మాణం cfs_rq *cfs_rq = tg_cfs_rq(tg, cpu_యొక్క(rq));
	నిర్మాణం task_struct *p, *tmp;
	LIST_తల(throttled_tasks);

	/*
	 * If cfs_rq->curr is set, the cfs_rq might not have caught up
	 * since the last clock update. Do it now before we begin
	 * queueing task onto it to save the need for unnecessarily
	 * unthrottle the hierarchy for this cfs_rq to be throttled
	 * right back again.
	 */
	update_curr(cfs_rq);

	ఒకవేళ (--cfs_rq->throttle_లెక్క)
		తిరిగి 0;

	ఒకవేళ (cfs_rq->pelt_clock_throttled) {
		cfs_rq->throttled_clock_pelt_సమయం += rq_clock_pelt(rq) -
					     cfs_rq->throttled_clock_pelt;
		cfs_rq->pelt_clock_throttled = 0;
	}

	ఒకవేళ (cfs_rq->throttled_clock_self) {
		u64 delta = rq_clock(rq) - cfs_rq->throttled_clock_self;

		cfs_rq->throttled_clock_self = 0;

		ఒకవేళ (WARN_ON_ONCE((s64)delta < 0))
			delta = 0;

		cfs_rq->throttled_clock_self_సమయం += delta;
	}

	/*
	 * Move the tasks to a local list since an update_curr() during
	 * enqueue_task_fair() can throttle a higher cfs_rq, and it can
	 * see the "throttled_limbo_list" being non-empty in
	 * tg_throttle_down() if throttle_count turned 0 above.
	 */
	list_splice_init(&cfs_rq->throttled_limbo_list, &throttled_tasks);

	/* Re-enqueue the tasks that have been throttled at this level. */
	list_for_each_నమోదు_సురక్షిత(p, tmp, &throttled_tasks, throttle_కణుపు) {
		/*
		 * Back to being throttled! Break out and put the remaining
		 * tasks back onto the limbo_list to prevent running them
		 * unnecessarily.
		 */
		ఒకవేళ (cfs_rq->throttle_లెక్క)
			ఆపు;

		list_del_init(&p->throttle_కణుపు);
		p->throttled = false;
		వరుసలో_చేర్చు_task_fair(rq, p, వరుసలో_చేర్చు_WAKEUP);
	}

	list_splice(&throttled_tasks, &cfs_rq->throttled_limbo_list);

	/* Add cfs_rq with load or one or more already running entities to the list */
	ఒకవేళ (!cfs_rq_is_decayed(cfs_rq))
		list_కూడు_ఆకు_cfs_rq(cfs_rq);

	తిరిగి 0;
}

స్థిర అంతఃవరుస bool task_has_throttle_work(నిర్మాణం task_struct *p)
{
	తిరిగి p->sched_throttle_work.తదుపరి != &p->sched_throttle_work;
}

స్థిర అంతఃవరుస శూన్య task_throttle_అమరిక_work(నిర్మాణం task_struct *p)
{
	ఒకవేళ (task_has_throttle_work(p))
		తిరిగి;

	/*
	 * Kthreads and exiting tasks don't return to userspace, so adding the
	 * work is pointless
	 */
	ఒకవేళ ((p->flags & (PF_EXITING | PF_KTHREAD)))
		తిరిగి;

	task_work_కూడు(p, &p->sched_throttle_work, TWA_RESUME);
}

స్థిర శూన్య రికార్డు_throttle_clock(నిర్మాణం cfs_rq *cfs_rq)
{
	నిర్మాణం rq *rq = rq_యొక్క(cfs_rq);

	ఒకవేళ (cfs_rq_throttled(cfs_rq) && !cfs_rq->throttled_clock)
		cfs_rq->throttled_clock = rq_clock(rq);

	ఒకవేళ (!cfs_rq->throttled_clock_self)
		cfs_rq->throttled_clock_self = rq_clock(rq);
}

స్థిర పూర్ణాంకం tg_throttle_కిందికి(నిర్మాణం task_సమూహం *tg, శూన్య *దత్తాంశం)
{
	నిర్మాణం rq *rq = దత్తాంశం;
	నిర్మాణం cfs_rq *cfs_rq = tg_cfs_rq(tg, cpu_యొక్క(rq));

	ఒకవేళ (cfs_rq->throttle_లెక్క++)
		తిరిగి 0;

	/*
	 * For cfs_rqs that still have entities enqueued, PELT clock
	 * stop happens at dequeue time when all entities are dequeued.
	 */
	ఒకవేళ (!cfs_rq->nr_queued) {
		list_del_ఆకు_cfs_rq(cfs_rq);
		cfs_rq->throttled_clock_pelt = rq_clock_pelt(rq);
		cfs_rq->pelt_clock_throttled = 1;
	}

	WARN_ON_ONCE(cfs_rq->throttled_clock_self);
	WARN_ON_ONCE(!list_ఖాళీలు(&cfs_rq->throttled_limbo_list));
	తిరిగి 0;
}

స్థిర bool throttle_cfs_rq(నిర్మాణం cfs_rq *cfs_rq)
{
	నిర్మాణం cfs_bandwidth *cfs_b = tg_cfs_bandwidth(cfs_rq->tg);
	నిర్మాణం sched_entity *curr = cfs_rq->h_curr;
	నిర్మాణం rq *rq = rq_యొక్క(cfs_rq);

	scoped_guard(raw_spinlock, &cfs_b->తాళం) {
		u64 లక్ష్యం_runtime = 1;

		/*
		 * If cfs_rq->h_curr is still runnable, we are here from an
		 * update_curr(). Request sysctl_sched_cfs_bandwidth_slice
		 * worth of bandwidth to continue running.
		 *
		 * If the curr is not runnable, just request enough bandwidth
		 * to be runnable next time the pick selects this cfs_rq.
		 */
		ఒకవేళ (curr && curr->on_rq)
			లక్ష్యం_runtime = sched_cfs_bandwidth_ముక్క();

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
		ఒకవేళ (__assign_cfs_rq_runtime(cfs_b, cfs_rq, లక్ష్యం_runtime))
			తిరిగి false;

		/*
		 * No bandwidth available; Add ourselves on the list to be
		 * unthrottled later.
		 */
		list_కూడు_తోక_rcu(&cfs_rq->throttled_list,
				  &cfs_b->throttled_cfs_rq);
	}

	/* freeze hierarchy runnable averages while throttled */
	scoped_guard(rcu)
		walk_tg_వృక్షం_from(cfs_rq->tg, tg_throttle_కిందికి, tg_nop, (శూన్య *)rq);

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
	ఒకవేళ (curr && curr->on_rq)
		task_throttle_అమరిక_work(rq->donor);

	తిరిగి true;
}

శూన్య unthrottle_cfs_rq(నిర్మాణం cfs_rq *cfs_rq)
{
	నిర్మాణం rq *rq = rq_యొక్క(cfs_rq);
	నిర్మాణం cfs_bandwidth *cfs_b = tg_cfs_bandwidth(cfs_rq->tg);
	నిర్మాణం sched_entity *se = cfs_rq_se(cfs_rq);

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
	ఒకవేళ (cfs_rq->runtime_enabled && cfs_rq->runtime_మిగిలిన <= 0)
		తిరిగి;

	cfs_rq->throttled = 0;

	scoped_guard(raw_spinlock, &cfs_b->తాళం) {
		list_del_rcu(&cfs_rq->throttled_list);

		ఒకవేళ (!cfs_rq->throttled_clock)
			ఆపు;

		cfs_b->throttled_సమయం += rq_clock(rq) - cfs_rq->throttled_clock;
		cfs_rq->throttled_clock = 0;
	}

	/* update hierarchical throttle state */
	walk_tg_వృక్షం_from(cfs_rq->tg, tg_nop, tg_unthrottle_పైకి, (శూన్య *)rq);

	ఒకవేళ (!cfs_rq->ఎక్కించు.బరువు) {
		ఒకవేళ (!cfs_rq->on_list)
			తిరిగి;
		/*
		 * Nothing to run but something to decay (on_list)?
		 * Complete the branch.
		 */
		for_each_sched_entity(se) {
			ఒకవేళ (list_కూడు_ఆకు_cfs_rq(cfs_rq_యొక్క(se)))
				ఆపు;
		}
	}

	assert_list_ఆకు_cfs_rq(rq);

	/* Determine whether we need to wake up potentially idle CPU: */
	ఒకవేళ (rq->curr == rq->idle && rq->cfs.h_nr_queued)
		resched_curr(rq);
}

స్థిర శూన్య __cfsb_csd_unthrottle(శూన్య *arg)
{
	నిర్మాణం cfs_rq *cursor, *tmp;
	నిర్మాణం rq *rq = arg;

	guard(rq_తాళం)(rq);

	/*
	 * Iterating over the list can trigger several call to
	 * update_rq_clock() in unthrottle_cfs_rq().
	 * Do it once and skip the potential next ones.
	 */
	update_rq_clock(rq);
	rq_clock_ప్రారంభం_loop_update(rq);

	/*
	 * Since we hold rq lock we're safe from concurrent manipulation of
	 * the CSD list. However, this RCU critical section annotates the
	 * fact that we pair with sched_free_group_rcu(), so that we cannot
	 * race with group being freed in the window between removing it
	 * from the list and advancing to the next entry in the list.
	 */
	guard(rcu)();

	list_for_each_నమోదు_సురక్షిత(cursor, tmp, &rq->cfsb_csd_list,
				 throttled_csd_list) {
		list_del_init(&cursor->throttled_csd_list);

		ఒకవేళ (cfs_rq_throttled(cursor))
			unthrottle_cfs_rq(cursor);
	}

	rq_clock_stop_loop_update(rq);
}

స్థిర అంతఃవరుస శూన్య __unthrottle_cfs_rq_async(నిర్మాణం cfs_rq *cfs_rq)
{
	నిర్మాణం rq *rq = rq_యొక్క(cfs_rq);
	bool మొదటి;

	ఒకవేళ (rq == this_rq()) {
		update_rq_clock(rq);
		unthrottle_cfs_rq(cfs_rq);
		తిరిగి;
	}

	/* Already enqueued */
	ఒకవేళ (WARN_ON_ONCE(!list_ఖాళీలు(&cfs_rq->throttled_csd_list)))
		తిరిగి;

	మొదటి = list_ఖాళీలు(&rq->cfsb_csd_list);
	list_కూడు_తోక(&cfs_rq->throttled_csd_list, &rq->cfsb_csd_list);
	ఒకవేళ (మొదటి)
		smp_call_function_single_async(cpu_యొక్క(rq), &rq->cfsb_csd);
}

స్థిర శూన్య unthrottle_cfs_rq_async(నిర్మాణం cfs_rq *cfs_rq)
{
	lockdep_assert_rq_held(rq_యొక్క(cfs_rq));

	ఒకవేళ (WARN_ON_ONCE(!cfs_rq_throttled(cfs_rq) ||
	    cfs_rq->runtime_మిగిలిన <= 0))
		తిరిగి;

	__unthrottle_cfs_rq_async(cfs_rq);
}

స్థిర bool distribute_cfs_runtime(నిర్మాణం cfs_bandwidth *cfs_b)
{
	bool throttled = false, unthrottle_local = false;
	పూర్ణాంకం this_cpu = smp_processor_id();
	u64 runtime, మిగిలిన = 1;
	నిర్మాణం cfs_rq *cfs_rq;
	నిర్మాణం rq *rq;

	guard(rcu)();

	list_for_each_నమోదు_rcu(cfs_rq, &cfs_b->throttled_cfs_rq,
				throttled_list) {
		rq = rq_యొక్క(cfs_rq);

		ఒకవేళ (!మిగిలిన) {
			throttled = true;
			ఆపు;
		}

		guard(rq_తాళం_irqsave)(rq);

		ఒకవేళ (!cfs_rq_throttled(cfs_rq))
			కొనసాగు;

		/* Already queued for async unthrottle */
		ఒకవేళ (!list_ఖాళీలు(&cfs_rq->throttled_csd_list))
			కొనసాగు;

		ఒకవేళ (cfs_rq->h_curr) {
			update_rq_clock(rq);
			update_curr(cfs_rq);
		}

		/* By the above checks, this should never be true */
		WARN_ON_ONCE(cfs_rq->runtime_మిగిలిన > 0);

		scoped_guard(raw_spinlock, &cfs_b->తాళం) {
			runtime = -cfs_rq->runtime_మిగిలిన + 1;
			ఒకవేళ (runtime > cfs_b->runtime)
				runtime = cfs_b->runtime;
			cfs_b->runtime -= runtime;
			మిగిలిన = cfs_b->runtime;
		}

		cfs_rq->runtime_మిగిలిన += runtime;

		/*
		 * Ran out of bandwidth during distribution!
		 * Indicate throttled entities and break early.
		 */
		ఒకవేళ (cfs_rq->runtime_మిగిలిన <= 0) {
			throttled = true;
			ఆపు;
		}

		/* we check whether we're throttled above */
		ఒకవేళ (cpu_యొక్క(rq) != this_cpu) {
			unthrottle_cfs_rq_async(cfs_rq);
			కొనసాగు;
		}

		/*
		 * Allow a parallel async unthrottle to unthrottle
		 * this cfs_rq too via __cfsb_csd_unthrottle().
		 * If we are first, do it ourselves at the end and
		 * save on an IPI from remote CPUs.
		 */
		unthrottle_local = list_ఖాళీలు(&rq->cfsb_csd_list);
		list_కూడు_తోక(&cfs_rq->throttled_csd_list, &rq->cfsb_csd_list);
	}

	ఒకవేళ (unthrottle_local) {
		/*
		 * Protect against an IPI that is also trying to flush
		 * the unthrottled cfs_rq(s) from this CPU's csd_list.
		 */
		scoped_guard(irqsave)
			__cfsb_csd_unthrottle(cpu_rq(this_cpu));
	}

	తిరిగి throttled;
}

/*
 * Responsible for refilling a task_group's bandwidth and unthrottling its
 * cfs_rqs as appropriate. If there has been no activity within the last
 * period the timer is deactivated until scheduling resumes; cfs_b->idle is
 * used to track this state.
 */
స్థిర పూర్ణాంకం do_sched_cfs_period_timer(నిర్మాణం cfs_bandwidth *cfs_b, పూర్ణాంకం overrun, గుర్తులేని దీర్ఘ flags)
	__must_hold(&cfs_b->తాళం)
{
	పూర్ణాంకం throttled;

	/* no need to continue the timer with no bandwidth constraint */
	ఒకవేళ (cfs_b->quota == RUNTIME_INF)
		వెళ్ళు బహిర్గతం_deactivate;

	throttled = !list_ఖాళీలు(&cfs_b->throttled_cfs_rq);
	cfs_b->nr_periods += overrun;

	/* Refill extra burst quota even if cfs_b->idle */
	__refill_cfs_bandwidth_runtime(cfs_b);

	/*
	 * idle depends on !throttled (for the case of a large deficit), and if
	 * we're going inactive then everything else can be deferred
	 */
	ఒకవేళ (cfs_b->idle && !throttled)
		వెళ్ళు బహిర్గతం_deactivate;

	ఒకవేళ (!throttled) {
		/* mark as potentially idle for the upcoming period */
		cfs_b->idle = 1;
		తిరిగి 0;
	}

	/* account preceding periods in which throttling occurred */
	cfs_b->nr_throttled += overrun;

	/*
	 * This check is repeated as we release cfs_b->lock while we unthrottle.
	 */
	అయితే (throttled && cfs_b->runtime > 0) {
		raw_spin_unlock_irqrestore(&cfs_b->తాళం, flags);
		/* we can't nest cfs_b->lock while distributing bandwidth */
		throttled = distribute_cfs_runtime(cfs_b);
		raw_spin_తాళం_irqsave(&cfs_b->తాళం, flags);
	}

	/*
	 * While we are ensured activity in the period following an
	 * unthrottle, this also covers the case in which the new bandwidth is
	 * insufficient to cover the existing bandwidth deficit.  (Forcing the
	 * timer to remain active while there are any throttled entities.)
	 */
	cfs_b->idle = 0;

	తిరిగి 0;

బహిర్గతం_deactivate:
	తిరిగి 1;
}

/* a cfs_rq won't donate quota below this amount */
స్థిర స్థిరరాశి u64 min_cfs_rq_runtime = 1 * NSEC_చొప్పున_MSEC;
/* minimum remaining period time to redistribute slack quota */
స్థిర స్థిరరాశి u64 min_bandwidth_expiration = 2 * NSEC_చొప్పున_MSEC;
/* how long we wait to gather additional slack before distributing */
స్థిర స్థిరరాశి u64 cfs_bandwidth_slack_period = 5 * NSEC_చొప్పున_MSEC;

/*
 * Are we near the end of the current quota period?
 *
 * Requires cfs_b->lock for hrtimer_expires_remaining to be safe against the
 * hrtimer base being cleared by hrtimer_start. In the case of
 * migrate_hrtimers, base is never cleared, so we are fine.
 */
స్థిర పూర్ణాంకం runtime_refresh_within(నిర్మాణం cfs_bandwidth *cfs_b, u64 min_expire)
{
	నిర్మాణం hrtimer *refresh_timer = &cfs_b->period_timer;
	s64 మిగిలిన;

	/* if the call-back is running a quota refresh is already occurring */
	ఒకవేళ (hrtimer_callback_running(refresh_timer))
		తిరిగి 1;

	/* is a quota refresh about to occur? */
	మిగిలిన = ktime_గమ్యం_ns(hrtimer_expires_మిగిలిన(refresh_timer));
	ఒకవేళ (మిగిలిన < (s64)min_expire)
		తిరిగి 1;

	తిరిగి 0;
}

స్థిర శూన్య ప్రారంభం_cfs_slack_bandwidth(నిర్మాణం cfs_bandwidth *cfs_b)
{
	u64 min_ఎడమ = cfs_bandwidth_slack_period + min_bandwidth_expiration;

	/* if there's a quota refresh soon don't bother with slack */
	ఒకవేళ (runtime_refresh_within(cfs_b, min_ఎడమ))
		తిరిగి;

	/* don't push forwards an existing deferred unthrottle */
	ఒకవేళ (cfs_b->slack_started)
		తిరిగి;
	cfs_b->slack_started = true;

	hrtimer_ప్రారంభం(&cfs_b->slack_timer,
			ns_గమ్యం_ktime(cfs_bandwidth_slack_period),
			HRTIMER_బహుళకం_REL);
}

/* we know any runtime found here is valid as update_curr() precedes return */
స్థిర శూన్య __return_cfs_rq_runtime(నిర్మాణం cfs_rq *cfs_rq)
{
	నిర్మాణం cfs_bandwidth *cfs_b = tg_cfs_bandwidth(cfs_rq->tg);
	s64 slack_runtime = cfs_rq->runtime_మిగిలిన - min_cfs_rq_runtime;

	ఒకవేళ (slack_runtime <= 0)
		తిరిగి;

	guard(raw_spinlock)(&cfs_b->తాళం);

	ఒకవేళ (cfs_b->quota != RUNTIME_INF) {
		cfs_b->runtime += slack_runtime;

		/* we are under rq->lock, defer unthrottling using a timer */
		ఒకవేళ (cfs_b->runtime > sched_cfs_bandwidth_ముక్క() &&
		    !list_ఖాళీలు(&cfs_b->throttled_cfs_rq))
			ప్రారంభం_cfs_slack_bandwidth(cfs_b);
	}

	/* even if it's not valid for return we don't want to try again */
	cfs_rq->runtime_మిగిలిన -= slack_runtime;
}

స్థిర __always_inline శూన్య return_cfs_rq_runtime(నిర్మాణం cfs_rq *cfs_rq)
{
	ఒకవేళ (!cfs_bandwidth_వాడిన())
		తిరిగి;

	ఒకవేళ (!cfs_rq->runtime_enabled || cfs_rq->nr_queued)
		తిరిగి;

	__return_cfs_rq_runtime(cfs_rq);
}

/*
 * This is done with a timer (instead of inline with bandwidth return) since
 * it's necessary to juggle rq->locks to unthrottle their respective cfs_rqs.
 */
స్థిర శూన్య do_sched_cfs_slack_timer(నిర్మాణం cfs_bandwidth *cfs_b)
{
	/* confirm we're still not at a refresh boundary */
	scoped_guard(raw_spinlock_irqsave, &cfs_b->తాళం) {
		u64 runtime = 0, ముక్క = sched_cfs_bandwidth_ముక్క();

		cfs_b->slack_started = false;

		ఒకవేళ (runtime_refresh_within(cfs_b, min_bandwidth_expiration))
			తిరిగి;

		ఒకవేళ (cfs_b->quota != RUNTIME_INF && cfs_b->runtime > ముక్క)
			runtime = cfs_b->runtime;

		ఒకవేళ (!runtime)
			తిరిగి;
	}

	distribute_cfs_runtime(cfs_b);
}

/*
 * When a group wakes up we want to make sure that its quota is not already
 * expired/exceeded, otherwise it may be allowed to steal additional ticks of
 * runtime as update_curr() throttling can not trigger until it's on-rq.
 */
స్థిర శూన్య check_వరుసలో_చేర్చు_throttle(నిర్మాణం cfs_rq *cfs_rq)
{
	ఒకవేళ (!cfs_bandwidth_వాడిన())
		తిరిగి;

	/* an active group must be handled by the update_curr() path */
	ఒకవేళ (!cfs_rq->runtime_enabled || cfs_rq->h_curr)
		తిరిగి;

	/* ensure the group is not already throttled */
	ఒకవేళ (cfs_rq_throttled(cfs_rq))
		తిరిగి;

	/* update runtime allocation */
	account_cfs_rq_runtime(cfs_rq, 0);
}

స్థిర శూన్య sync_throttle(నిర్మాణం task_సమూహం *tg, పూర్ణాంకం cpu)
{
	నిర్మాణం cfs_rq *pcfs_rq, *cfs_rq;

	ఒకవేళ (!cfs_bandwidth_వాడిన())
		తిరిగి;

	ఒకవేళ (!tg->మాతృ)
		తిరిగి;

	cfs_rq = tg_cfs_rq(tg, cpu);
	pcfs_rq = tg_cfs_rq(tg->మాతృ, cpu);

	cfs_rq->throttle_లెక్క = pcfs_rq->throttle_లెక్క;
	cfs_rq->throttled_clock_pelt = rq_clock_pelt(cpu_rq(cpu));

	/*
	 * It is not enough to sync the "pelt_clock_throttled" indicator
	 * with the parent cfs_rq when the hierarchy is not queued.
	 * Always join a throttled hierarchy with PELT clock throttled
	 * and leaf it to the first enqueue, or distribution to
	 * unthrottle the PELT clock.
	 */
	ఒకవేళ (cfs_rq->throttle_లెక్క)
		cfs_rq->pelt_clock_throttled = 1;
}

స్థిర గణన hrtimer_restart sched_cfs_slack_timer(నిర్మాణం hrtimer *timer)
{
	నిర్మాణం cfs_bandwidth *cfs_b =
		container_యొక్క(timer, నిర్మాణం cfs_bandwidth, slack_timer);

	do_sched_cfs_slack_timer(cfs_b);

	తిరిగి HRTIMER_NORESTART;
}

స్థిర గణన hrtimer_restart sched_cfs_period_timer(నిర్మాణం hrtimer *timer)
{
	నిర్మాణం cfs_bandwidth *cfs_b =
		container_యొక్క(timer, నిర్మాణం cfs_bandwidth, period_timer);
	పూర్ణాంకం overrun;
	పూర్ణాంకం idle = 0;
	పూర్ణాంకం లెక్క = 0;

	CLASS(raw_spinlock_irqsave, cfsb_guard)(&cfs_b->తాళం);

	కొరకు (;;) {
		overrun = hrtimer_forward_now(timer, cfs_b->period);
		ఒకవేళ (!overrun)
			ఆపు;

		idle = do_sched_cfs_period_timer(cfs_b, overrun, cfsb_guard.flags);

		ఒకవేళ (++లెక్క > 3) {
			u64 new, old = ktime_గమ్యం_ns(cfs_b->period);

			/*
			 * Grow period by a factor of 2 to avoid losing precision.
			 * Precision loss in the quota/period ratio can cause __cfs_schedulable
			 * to fail.
			 */
			new = old * 2;
			ఒకవేళ (new < max_bw_quota_period_us * NSEC_చొప్పున_USEC) {
				cfs_b->period = ns_గమ్యం_ktime(new);
				cfs_b->quota *= 2;
				cfs_b->వ్యవధి *= 2;

				pr_warn_ratelimited(
	"cfs_period_timer[cpu%d]: period too short, scaling up (new cfs_period_us = %lld, cfs_quota_us = %lld)\n",
					smp_processor_id(),
					div_u64(new, NSEC_చొప్పున_USEC),
					div_u64(cfs_b->quota, NSEC_చొప్పున_USEC));
			} లేకపోతే {
				pr_warn_ratelimited(
	"cfs_period_timer[cpu%d]: period too short, but cannot scale up without losing precision (cfs_period_us = %lld, cfs_quota_us = %lld)\n",
					smp_processor_id(),
					div_u64(old, NSEC_చొప్పున_USEC),
					div_u64(cfs_b->quota, NSEC_చొప్పున_USEC));
			}

			/* reset count so we don't come right back in here */
			లెక్క = 0;
		}
	}

	ఒకవేళ (idle) {
		cfs_b->period_చురుకైన = 0;
		తిరిగి HRTIMER_NORESTART;
	}

	తిరిగి HRTIMER_RESTART;
}

శూన్య init_cfs_bandwidth(నిర్మాణం cfs_bandwidth *cfs_b, నిర్మాణం cfs_bandwidth *మాతృ)
{
	raw_spin_తాళం_init(&cfs_b->తాళం);
	cfs_b->runtime = 0;
	cfs_b->quota = RUNTIME_INF;
	cfs_b->period = us_గమ్యం_ktime(default_bw_period_us());
	cfs_b->వ్యవధి = 0;
	cfs_b->క్రమానుగత_quota = మాతృ ? మాతృ->క్రమానుగత_quota : RUNTIME_INF;

	INIT_LIST_తల(&cfs_b->throttled_cfs_rq);
	hrtimer_అమరిక(&cfs_b->period_timer, sched_cfs_period_timer, CLOCK_MONOTONIC,
		      HRTIMER_బహుళకం_ABS_PINNED);

	/* Add a random offset so that timers interleave */
	hrtimer_సమితి_expires(&cfs_b->period_timer,
			    get_random_u32_క్రింద(cfs_b->period));
	hrtimer_అమరిక(&cfs_b->slack_timer, sched_cfs_slack_timer, CLOCK_MONOTONIC,
		      HRTIMER_బహుళకం_REL);
	cfs_b->slack_started = false;
}

స్థిర శూన్య init_cfs_rq_runtime(నిర్మాణం cfs_rq *cfs_rq)
{
	cfs_rq->runtime_enabled = 0;
	INIT_LIST_తల(&cfs_rq->throttled_list);
	INIT_LIST_తల(&cfs_rq->throttled_csd_list);
	INIT_LIST_తల(&cfs_rq->throttled_limbo_list);
}

శూన్య ప్రారంభం_cfs_bandwidth(నిర్మాణం cfs_bandwidth *cfs_b)
{
	lockdep_assert_held(&cfs_b->తాళం);

	ఒకవేళ (cfs_b->period_చురుకైన)
		తిరిగి;

	cfs_b->period_చురుకైన = 1;
	hrtimer_forward_now(&cfs_b->period_timer, cfs_b->period);
	hrtimer_ప్రారంభం_expires(&cfs_b->period_timer, HRTIMER_బహుళకం_ABS_PINNED);
}

స్థిర శూన్య destroy_cfs_bandwidth(నిర్మాణం cfs_bandwidth *cfs_b)
{
	పూర్ణాంకం __maybe_unused i;

	/* init_cfs_bandwidth() was not called */
	ఒకవేళ (!cfs_b->throttled_cfs_rq.తదుపరి)
		తిరిగి;

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
		నిర్మాణం rq *rq = cpu_rq(i);

		ఒకవేళ (list_ఖాళీలు(&rq->cfsb_csd_list))
			కొనసాగు;

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
స్థిర శూన్య __maybe_unused update_runtime_enabled(నిర్మాణం rq *rq)
{
	నిర్మాణం task_సమూహం *tg;

	lockdep_assert_rq_held(rq);

	guard(rcu)();

	list_for_each_నమోదు_rcu(tg, &task_groups, list) {
		నిర్మాణం cfs_bandwidth *cfs_b = &tg->cfs_bandwidth;
		నిర్మాణం cfs_rq *cfs_rq = tg_cfs_rq(tg, cpu_యొక్క(rq));

		scoped_guard(raw_spinlock, &cfs_b->తాళం)
			cfs_rq->runtime_enabled = cfs_b->quota != RUNTIME_INF;
	}
}

/* cpu offline callback */
స్థిర శూన్య __maybe_unused unthrottle_offline_cfs_rqs(నిర్మాణం rq *rq)
{
	నిర్మాణం task_సమూహం *tg;

	lockdep_assert_rq_held(rq);

	// Do not unthrottle for an active CPU
	ఒకవేళ (cpumask_test_cpu(cpu_యొక్క(rq), cpu_చురుకైన_ముసుగు))
		తిరిగి;

	/*
	 * The rq clock has already been updated in the
	 * set_rq_offline(), so we should skip updating
	 * the rq clock again in unthrottle_cfs_rq().
	 */
	rq_clock_ప్రారంభం_loop_update(rq);

	guard(rcu)();

	list_for_each_నమోదు_rcu(tg, &task_groups, list) {
		నిర్మాణం cfs_rq *cfs_rq = tg_cfs_rq(tg, cpu_యొక్క(rq));

		ఒకవేళ (!cfs_rq->runtime_enabled)
			కొనసాగు;

		/*
		 * Offline rq is schedulable till CPU is completely disabled
		 * in take_cpu_down(), so we prevent new cfs throttling here.
		 */
		cfs_rq->runtime_enabled = 0;

		ఒకవేళ (!cfs_rq_throttled(cfs_rq))
			కొనసాగు;

		/*
		 * clock_task is not advancing so we just need to make sure
		 * there's some valid quota amount
		 */
		cfs_rq->runtime_మిగిలిన = 1;
		unthrottle_cfs_rq(cfs_rq);
	}

	rq_clock_stop_loop_update(rq);
}

bool cfs_task_bw_constrained(నిర్మాణం task_struct *p)
{
	నిర్మాణం cfs_rq *cfs_rq = task_cfs_rq(p);

	ఒకవేళ (!cfs_bandwidth_వాడిన())
		తిరిగి false;

	ఒకవేళ (cfs_rq->runtime_enabled ||
	    tg_cfs_bandwidth(cfs_rq->tg)->క్రమానుగత_quota != RUNTIME_INF)
		తిరిగి true;

	తిరిగి false;
}

#నిర్వచితమైతే CONFIG_NO_HZ_నిండిన
/* called from pick_next_task_fair() */
స్థిర శూన్య sched_fair_update_stop_tick(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
	పూర్ణాంకం cpu = cpu_యొక్క(rq);

	ఒకవేళ (!cfs_bandwidth_వాడిన())
		తిరిగి;

	ఒకవేళ (!tick_nohz_నిండిన_cpu(cpu))
		తిరిగి;

	ఒకవేళ (rq->nr_running != 1)
		తిరిగి;

	/*
	 *  We know there is only one task runnable and we've just picked it. The
	 *  normal enqueue path will have cleared TICK_DEP_BIT_SCHED if we will
	 *  be otherwise able to stop the tick. Just need to check if we are using
	 *  bandwidth control.
	 */
	ఒకవేళ (cfs_task_bw_constrained(p))
		tick_nohz_dep_సమితి_cpu(cpu, TICK_DEP_బిట్టు_SCHED);
}
#అయితే_ముగింపు /* CONFIG_NO_HZ_FULL */

#లేకపోతే /* !CONFIG_CFS_BANDWIDTH: */

స్థిర bool account_cfs_rq_runtime(నిర్మాణం cfs_rq *cfs_rq, u64 delta_exec) { తిరిగి false; }
స్థిర శూన్య check_వరుసలో_చేర్చు_throttle(నిర్మాణం cfs_rq *cfs_rq) {}
స్థిర అంతఃవరుస శూన్య sync_throttle(నిర్మాణం task_సమూహం *tg, పూర్ణాంకం cpu) {}
స్థిర __always_inline శూన్య return_cfs_rq_runtime(నిర్మాణం cfs_rq *cfs_rq) {}
స్థిర శూన్య task_throttle_అమరిక_work(నిర్మాణం task_struct *p) {}
స్థిర bool task_is_throttled(నిర్మాణం task_struct *p) { తిరిగి false; }
స్థిర శూన్య వరుస_నుండి_తీయి_throttled_task(నిర్మాణం task_struct *p, పూర్ణాంకం flags) {}
స్థిర bool వరుసలో_చేర్చు_throttled_task(నిర్మాణం task_struct *p) { తిరిగి false; }
స్థిర శూన్య రికార్డు_throttle_clock(నిర్మాణం cfs_rq *cfs_rq) {}

స్థిర అంతఃవరుస పూర్ణాంకం cfs_rq_throttled(నిర్మాణం cfs_rq *cfs_rq)
{
	తిరిగి 0;
}

స్థిర అంతఃవరుస bool cfs_rq_pelt_clock_throttled(నిర్మాణం cfs_rq *cfs_rq)
{
	తిరిగి false;
}

స్థిర అంతఃవరుస పూర్ణాంకం throttled_hierarchy(నిర్మాణం cfs_rq *cfs_rq)
{
	తిరిగి 0;
}

స్థిర అంతఃవరుస పూర్ణాంకం lb_throttled_hierarchy(నిర్మాణం task_struct *p, పూర్ణాంకం dst_cpu)
{
	తిరిగి 0;
}

#నిర్వచితమైతే CONFIG_FAIR_సమూహం_SCHED
శూన్య init_cfs_bandwidth(నిర్మాణం cfs_bandwidth *cfs_b, నిర్మాణం cfs_bandwidth *మాతృ) {}
స్థిర శూన్య init_cfs_rq_runtime(నిర్మాణం cfs_rq *cfs_rq) {}
#అయితే_ముగింపు

స్థిర అంతఃవరుస నిర్మాణం cfs_bandwidth *tg_cfs_bandwidth(నిర్మాణం task_సమూహం *tg)
{
	తిరిగి ఖాళీ;
}
స్థిర అంతఃవరుస శూన్య destroy_cfs_bandwidth(నిర్మాణం cfs_bandwidth *cfs_b) {}
స్థిర అంతఃవరుస శూన్య update_runtime_enabled(నిర్మాణం rq *rq) {}
స్థిర అంతఃవరుస శూన్య unthrottle_offline_cfs_rqs(నిర్మాణం rq *rq) {}
#నిర్వచితమైతే CONFIG_CGROUP_SCHED
bool cfs_task_bw_constrained(నిర్మాణం task_struct *p)
{
	తిరిగి false;
}
#అయితే_ముగింపు
#అయితే_ముగింపు /* !CONFIG_CFS_BANDWIDTH */

#ఒకవేళ !defined(CONFIG_CFS_BANDWIDTH) || !defined(CONFIG_NO_HZ_నిండిన)
స్థిర అంతఃవరుస శూన్య sched_fair_update_stop_tick(నిర్మాణం rq *rq, నిర్మాణం task_struct *p) {}
#అయితే_ముగింపు

/**************************************************
 * CFS operations on tasks:
 */

#నిర్వచితమైతే CONFIG_SCHED_HRTICK
స్థిర శూన్య hrtick_ప్రారంభం_fair(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
	నిర్మాణం sched_entity *se = &p->se;
	గుర్తులేని దీర్ఘ scale = 1024;
	గుర్తులేని దీర్ఘ util = 0;
	u64 vdelta;
	u64 delta;

	WARN_ON_ONCE(task_rq(p) != rq);

	ఒకవేళ (rq->cfs.h_nr_queued <= 1)
		తిరిగి;

	/*
	 * Compute time until virtual deadline
	 */
	vdelta = se->deadline - se->vruntime;
	ఒకవేళ ((s64)vdelta < 0) {
		ఒకవేళ (task_current_donor(rq, p))
			resched_curr(rq);
		తిరిగి;
	}
	delta = (se->h_ఎక్కించు.బరువు * vdelta) / NICE_0_ఎక్కించు;

	/*
	 * Correct for instantaneous load of other classes.
	 */
	util += cpu_util_irq(rq);
	ఒకవేళ (util && util < 1024) {
		scale *= 1024;
		scale /= (1024 - util);
	}

	hrtick_ప్రారంభం(rq, (scale * delta) / 1024);
}

/*
 * Called on enqueue to start the hrtick when h_nr_queued becomes more than 1.
 */
స్థిర శూన్య hrtick_update(నిర్మాణం rq *rq)
{
	నిర్మాణం task_struct *donor = rq->donor;

	ఒకవేళ (!hrtick_enabled_fair(rq) || donor->sched_class != &fair_sched_class)
		తిరిగి;

	ఒకవేళ (hrtick_చురుకైన(rq))
		తిరిగి;

	hrtick_ప్రారంభం_fair(rq, donor);
}
#లేకపోతే /* !CONFIG_SCHED_HRTICK: */
స్థిర అంతఃవరుస శూన్య
hrtick_ప్రారంభం_fair(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
}

స్థిర అంతఃవరుస శూన్య hrtick_update(నిర్మాణం rq *rq)
{
}
#అయితే_ముగింపు /* !CONFIG_SCHED_HRTICK */

స్థిర అంతఃవరుస bool cpu_overutilized(పూర్ణాంకం cpu)
{
	గుర్తులేని దీర్ఘ rq_util_max;

	ఒకవేళ (!sched_energy_enabled())
		తిరిగి false;

	rq_util_max = uclamp_rq_get(cpu_rq(cpu), UCLAMP_MAX);

	/* Return true only if the utilization doesn't fit CPU's capacity */
	తిరిగి !util_సరిపోతుంది_cpu(cpu_util_cfs(cpu), 0, rq_util_max, cpu);
}

/*
 * overutilized value make sense only if EAS is enabled
 */
స్థిర అంతఃవరుస bool is_rd_overutilized(నిర్మాణం మూలం_domain *rd)
{
	తిరిగి !sched_energy_enabled() || READ_ONCE(rd->overutilized);
}

స్థిర అంతఃవరుస శూన్య సమితి_rd_overutilized(నిర్మాణం మూలం_domain *rd, bool flag)
{
	ఒకవేళ (!sched_energy_enabled())
		తిరిగి;

	WRITE_ONCE(rd->overutilized, flag);
	trace_sched_overutilized_tp(rd, flag);
}

స్థిర అంతఃవరుస శూన్య check_update_overutilized_స్థితి(నిర్మాణం rq *rq)
{
	/*
	 * overutilized field is used for load balancing decisions only
	 * if energy aware scheduler is being used
	 */

	ఒకవేళ (!is_rd_overutilized(rq->rd) && cpu_overutilized(rq->cpu))
		సమితి_rd_overutilized(rq->rd, 1);
}

/* Runqueue only has SCHED_IDLE tasks enqueued */
స్థిర పూర్ణాంకం sched_idle_rq(నిర్మాణం rq *rq)
{
	తిరిగి unlikely(rq->nr_running == rq->cfs.h_nr_idle &&
			rq->nr_running);
}

స్థిర పూర్ణాంకం ఎంచుకో_sched_idle_rq(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
	తిరిగి sched_idle_rq(rq) && !task_has_idle_policy(p);
}

స్థిర పూర్ణాంకం ఎంచుకో_idle_cpu(పూర్ణాంకం cpu, నిర్మాణం task_struct *p)
{
	తిరిగి అందుబాటు_idle_cpu(cpu) ||
	       ఎంచుకో_sched_idle_rq(cpu_rq(cpu), p);
}

స్థిర శూన్య
requeue_delayed_entity(నిర్మాణం cfs_rq *cfs_rq, నిర్మాణం sched_entity *se)
{
	/*
	 * se->sched_delayed should imply: se->on_rq == 1.
	 * Because a delayed entity is one that is still on
	 * the runqueue competing until elegibility.
	 */
	WARN_ON_ONCE(!se->sched_delayed);
	WARN_ON_ONCE(!se->on_rq);

	ఒకవేళ (update_entity_lag(cfs_rq, se)) {
		cfs_rq->h_nr_queued--;
		ఒకవేళ (se != cfs_rq->curr)
			__dequeue_entity(cfs_rq, se);
		ఉంచు_entity(cfs_rq, se, 0);
		ఒకవేళ (se != cfs_rq->curr)
			__enqueue_entity(cfs_rq, se);
		cfs_rq->h_nr_queued++;
	}

	update_ఎక్కించు_avg(cfs_rq, se, 0);
	clear_delayed(se);
}

స్థిర గుర్తులేని దీర్ఘ వరుసలో_చేర్చు_hierarchy(నిర్మాణం task_struct *p, పూర్ణాంకం flags)
{
	గుర్తులేని దీర్ఘ బరువు = NICE_0_ఎక్కించు;
	పూర్ణాంకం task_new = !(flags & వరుసలో_చేర్చు_WAKEUP);
	నిర్మాణం sched_entity *se = &p->se;
	పూర్ణాంకం h_nr_idle = task_has_idle_policy(p);
	పూర్ణాంకం h_nr_runnable = 1;

	ఒకవేళ (task_new && se->sched_delayed)
		h_nr_runnable = 0;

	for_each_sched_entity(se) {
		నిర్మాణం cfs_rq *cfs_rq = cfs_rq_యొక్క(se);

		update_curr(cfs_rq);

		ఒకవేళ (!se->on_rq) {
			వరుసలో_చేర్చు_entity(cfs_rq, se, flags);
		} లేకపోతే {
			update_ఎక్కించు_avg(cfs_rq, se, UPDATE_TG);
			se_update_runnable(se);
			update_cfs_సమూహం(se);
		}

		cfs_rq->h_nr_runnable += h_nr_runnable;
		cfs_rq->h_nr_queued++;
		cfs_rq->h_nr_idle += h_nr_idle;

		ఒకవేళ (cfs_rq_is_idle(cfs_rq))
			h_nr_idle = 1;

		బరువు = __calc_prop_weight(cfs_rq, se, బరువు);

		flags = వరుసలో_చేర్చు_WAKEUP;
	}

	తిరిగి బరువు;
}

/* Update curr's vruntime before placing entity or updating lag */
స్థిర అంతఃవరుస శూన్య update_curr_eevdf(నిర్మాణం cfs_rq *cfs_rq)
{
	ఒకవేళ (!cfs_rq->curr)
		తిరిగి;

	update_curr(cfs_rq_యొక్క(cfs_rq->curr));
}

/*
 * The enqueue_task method is called before nr_running is
 * increased. Here we update the fair scheduling stats and
 * then put the task into the rbtree:
 */
స్థిర శూన్య
వరుసలో_చేర్చు_task_fair(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, పూర్ణాంకం flags)
{
	పూర్ణాంకం rq_h_nr_queued = rq->cfs.h_nr_queued;
	పూర్ణాంకం task_new = !(flags & వరుసలో_చేర్చు_WAKEUP);
	నిర్మాణం sched_entity *se = &p->se;
	నిర్మాణం cfs_rq *cfs_rq = &rq->cfs;
	గుర్తులేని దీర్ఘ బరువు;
	bool curr;

	ఒకవేళ (task_is_throttled(p) && వరుసలో_చేర్చు_throttled_task(p))
		తిరిగి;

	/*
	 * The code below (indirectly) updates schedutil which looks at
	 * the cfs_rq utilization to select a frequency.
	 * Let's add the task's estimated utilization to the cfs_rq's
	 * estimated utilization, before we update schedutil.
	 */
	ఒకవేళ (!p->se.sched_delayed || (flags & వరుసలో_చేర్చు_DELAYED))
		util_est_వరుసలో_చేర్చు(cfs_rq, p);

	update_curr_eevdf(cfs_rq);

	ఒకవేళ (flags & వరుసలో_చేర్చు_DELAYED) {
		requeue_delayed_entity(cfs_rq, se);
		తిరిగి;
	}

	/*
	 * If in_iowait is set, the code below may not trigger any cpufreq
	 * utilization updates, so do it here explicitly with the IOWAIT flag
	 * passed.
	 */
	ఒకవేళ (p->లోపల_iowait)
		cpufreq_update_util(rq, SCHED_CPUFREQ_IOWAIT);

	curr = (cfs_rq->curr == se);
	/*
	 * This can actually happen with CFS_BANDWIDTH, a task can be throttled
	 * and dequeued, unthrottled from another CPU and then enqueued again
	 * before schedule().
	 */
	ఒకవేళ (curr)
		ఉంచు_entity(cfs_rq, se, flags);

	ఒకవేళ (se->on_rq && se->sched_delayed)
		requeue_delayed_entity(cfs_rq, se);

	బరువు = వరుసలో_చేర్చు_hierarchy(p, flags);

	ఒకవేళ (!curr) {
		reweight_eevdf(cfs_rq, se, బరువు, false);
		ఉంచు_entity(cfs_rq, se, flags | వరుసలో_చేర్చు_QUEUED);
		__enqueue_entity(cfs_rq, se);
	}

	ఒకవేళ (!rq_h_nr_queued && rq->cfs.h_nr_queued)
		dl_server_ప్రారంభం(&rq->fair_server);

	/* At this point se is NULL and we are at root level*/
	కూడు_nr_running(rq, 1);

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
	ఒకవేళ (!task_new)
		check_update_overutilized_స్థితి(rq);

	assert_list_ఆకు_cfs_rq(rq);

	hrtick_update(rq);
}

స్థిర శూన్య వరుస_నుండి_తీయి_hierarchy(నిర్మాణం task_struct *p, పూర్ణాంకం flags)
{
	నిర్మాణం sched_entity *se = &p->se;
	bool task_sleep = flags & వరుస_నుండి_తీయి_SLEEP;
	bool task_delayed = flags & వరుస_నుండి_తీయి_DELAYED;
	bool task_throttled = flags & వరుస_నుండి_తీయి_THROTTLE;
	పూర్ణాంకం h_nr_runnable = 0;
	పూర్ణాంకం h_nr_idle = task_has_idle_policy(p);
	bool వరుస_నుండి_తీయి = true;

	ఒకవేళ (task_sleep || task_delayed || !se->sched_delayed)
		h_nr_runnable = 1;

	for_each_sched_entity(se) {
		నిర్మాణం cfs_rq *cfs_rq = cfs_rq_యొక్క(se);

		update_curr(cfs_rq);

		ఒకవేళ (వరుస_నుండి_తీయి) {
			వరుస_నుండి_తీయి_entity(cfs_rq, se, flags);
			/* Don't dequeue parent if it has other entities besides us */
			ఒకవేళ (cfs_rq->ఎక్కించు.బరువు)
				వరుస_నుండి_తీయి = false;
		} లేకపోతే {
			update_ఎక్కించు_avg(cfs_rq, se, UPDATE_TG);
			se_update_runnable(se);
			update_cfs_సమూహం(se);
		}

		cfs_rq->h_nr_runnable -= h_nr_runnable;
		cfs_rq->h_nr_queued--;
		cfs_rq->h_nr_idle -= h_nr_idle;

		ఒకవేళ (cfs_rq_is_idle(cfs_rq))
			h_nr_idle = 1;

		ఒకవేళ (throttled_hierarchy(cfs_rq) && task_throttled)
			రికార్డు_throttle_clock(cfs_rq);

		flags |= వరుస_నుండి_తీయి_SLEEP;
		flags &= ~(వరుస_నుండి_తీయి_DELAYED | వరుస_నుండి_తీయి_SPECIAL);
	}
}

/*
 * The part of dequeue_task_fair() that is needed to dequeue delayed tasks.
 *
 * Returns:
 *   true  - dequeued
 *   false - delayed
 */
స్థిర bool __dequeue_task(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, పూర్ణాంకం flags)
{
	నిర్మాణం sched_entity *se = &p->se;
	నిర్మాణం cfs_rq *cfs_rq = &rq->cfs;
	bool was_sched_idle = sched_idle_rq(rq);
	bool task_sleep = flags & వరుస_నుండి_తీయి_SLEEP;
	bool task_delayed = flags & వరుస_నుండి_తీయి_DELAYED;

	clear_buddies(cfs_rq, se);

	update_curr_eevdf(cfs_rq);
	update_entity_lag(cfs_rq, se);

	ఒకవేళ (flags & వరుస_నుండి_తీయి_DELAYED) {
		WARN_ON_ONCE(!se->sched_delayed);
	} లేకపోతే {
		bool delay = task_sleep;
		/*
		 * DELAY_DEQUEUE relies on spurious wakeups, special task
		 * states must not suffer spurious wakeups, excempt them.
		 */
		ఒకవేళ (flags & (వరుస_నుండి_తీయి_SPECIAL | వరుస_నుండి_తీయి_THROTTLE))
			delay = false;

		WARN_ON_ONCE(delay && se->sched_delayed);

		ఒకవేళ (sched_feat(DELAY_వరుస_నుండి_తీయి) && delay &&
		    !entity_eligible(cfs_rq, se)) {
			update_ఎక్కించు_avg(cfs_rq_యొక్క(se), se, UPDATE_UTIL_EST);
			సమితి_delayed(se);
			తిరిగి false;
		}
	}

	వరుస_నుండి_తీయి_hierarchy(p, flags);

	ఒకవేళ (sched_feat(ఉంచు_REL_DEADLINE) && !task_sleep) {
		se->deadline -= se->vruntime;
		se->rel_deadline = 1;
	}
	ఒకవేళ (se != cfs_rq->curr)
		__dequeue_entity(cfs_rq, se);

	sub_nr_running(rq, 1);

	/* balance early to pull high priority tasks */
	ఒకవేళ (unlikely(!was_sched_idle && sched_idle_rq(rq)))
		rq->తదుపరి_balance = jiffies;

	ఒకవేళ (task_delayed) {
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

	తిరిగి true;
}

/*
 * The dequeue_task method is called before nr_running is
 * decreased. We remove the task from the rbtree and
 * update the fair scheduling stats:
 */
స్థిర bool వరుస_నుండి_తీయి_task_fair(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, పూర్ణాంకం flags)
{
	ఒకవేళ (task_is_throttled(p)) {
		వరుస_నుండి_తీయి_throttled_task(p, flags);
		తిరిగి true;
	}

	ఒకవేళ (!p->se.sched_delayed)
		util_est_వరుస_నుండి_తీయి(&rq->cfs, p);

	ఒకవేళ (!__dequeue_task(rq, p, flags))
		తిరిగి false;

	/*
	 * Must not reference @p after __dequeue_task(DEQUEUE_DELAYED).
	 */
	తిరిగి true;
}

స్థిర అంతఃవరుస గుర్తులేని పూర్ణాంకం cfs_h_nr_delayed(నిర్మాణం rq *rq)
{
	తిరిగి (rq->cfs.h_nr_queued - rq->cfs.h_nr_runnable);
}

/* Working cpumask for: sched_balance_rq(), sched_balance_newidle(). */
స్థిర DEFINE_చొప్పున_CPU(cpumask_var_t, ఎక్కించు_balance_ముసుగు);
స్థిర DEFINE_చొప్పున_CPU(cpumask_var_t, select_rq_ముసుగు);
స్థిర DEFINE_చొప్పున_CPU(cpumask_var_t, should_we_balance_tmpmask);

#నిర్వచితమైతే CONFIG_NO_HZ_COMMON

స్థిర నిర్మాణం {
	cpumask_var_t idle_cpus_ముసుగు;
	పూర్ణాంకం has_blocked_ఎక్కించు;		/* Idle CPUS has blocked load */
	పూర్ణాంకం needs_update;		/* Newly idle CPUs need their next_balance collated */
	గుర్తులేని దీర్ఘ తదుపరి_balance;     /* in jiffy units */
	గుర్తులేని దీర్ఘ తదుపరి_blocked;	/* Next update of blocked load in jiffies */
} nohz ____cacheline_aligned;

#అయితే_ముగింపు /* CONFIG_NO_HZ_COMMON */

స్థిర గుర్తులేని దీర్ఘ cpu_ఎక్కించు(నిర్మాణం rq *rq)
{
	తిరిగి cfs_rq_ఎక్కించు_avg(&rq->cfs);
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
స్థిర గుర్తులేని దీర్ఘ cpu_ఎక్కించు_without(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
	నిర్మాణం cfs_rq *cfs_rq;
	గుర్తులేని పూర్ణాంకం ఎక్కించు;

	/* Task has no contribution or is new */
	ఒకవేళ (cpu_యొక్క(rq) != task_cpu(p) || !READ_ONCE(p->se.avg.చివరి_update_సమయం))
		తిరిగి cpu_ఎక్కించు(rq);

	cfs_rq = &rq->cfs;
	ఎక్కించు = READ_ONCE(cfs_rq->avg.ఎక్కించు_avg);

	/* Discount task's util from CPU's util */
	lsub_ధనాత్మక(&ఎక్కించు, task_h_ఎక్కించు(p));

	తిరిగి ఎక్కించు;
}

స్థిర గుర్తులేని దీర్ఘ cpu_runnable(నిర్మాణం rq *rq)
{
	తిరిగి cfs_rq_runnable_avg(&rq->cfs);
}

స్థిర గుర్తులేని దీర్ఘ cpu_runnable_without(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
	నిర్మాణం cfs_rq *cfs_rq;
	గుర్తులేని పూర్ణాంకం runnable;

	/* Task has no contribution or is new */
	ఒకవేళ (cpu_యొక్క(rq) != task_cpu(p) || !READ_ONCE(p->se.avg.చివరి_update_సమయం))
		తిరిగి cpu_runnable(rq);

	cfs_rq = &rq->cfs;
	runnable = READ_ONCE(cfs_rq->avg.runnable_avg);

	/* Discount task's runnable from CPU's runnable */
	lsub_ధనాత్మక(&runnable, p->se.avg.runnable_avg);

	తిరిగి runnable;
}

స్థిర గుర్తులేని దీర్ఘ సామర్థ్యం_యొక్క(పూర్ణాంకం cpu)
{
	తిరిగి cpu_rq(cpu)->cpu_సామర్థ్యం;
}

స్థిర శూన్య రికార్డు_wakee(నిర్మాణం task_struct *p)
{
	/*
	 * Only decay a single time; tasks that have less then 1 wakeup per
	 * jiffy will not have built up many flips.
	 */
	ఒకవేళ (సమయం_after(jiffies, current->wakee_flip_decay_ts + HZ)) {
		current->wakee_flips >>= 1;
		current->wakee_flip_decay_ts = jiffies;
	}

	ఒకవేళ (current->చివరి_wakee != p) {
		current->చివరి_wakee = p;
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
స్థిర పూర్ణాంకం wake_wide(నిర్మాణం task_struct *p)
{
	గుర్తులేని పూర్ణాంకం master = current->wakee_flips;
	గుర్తులేని పూర్ణాంకం slave = p->wakee_flips;
	పూర్ణాంకం గుణకం = __this_cpu_read(sd_llc_కొలత);

	ఒకవేళ (master < slave)
		మార్పిడి(master, slave);
	ఒకవేళ (slave < గుణకం || master < slave * గుణకం)
		తిరిగి 0;
	తిరిగి 1;
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
స్థిర పూర్ణాంకం
wake_affine_idle(పూర్ణాంకం this_cpu, పూర్ణాంకం prev_cpu, పూర్ణాంకం sync)
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
	ఒకవేళ (అందుబాటు_idle_cpu(this_cpu) && cpus_share_cache(this_cpu, prev_cpu))
		తిరిగి అందుబాటు_idle_cpu(prev_cpu) ? prev_cpu : this_cpu;

	ఒకవేళ (sync) {
		నిర్మాణం rq *rq = cpu_rq(this_cpu);

		ఒకవేళ ((rq->nr_running - cfs_h_nr_delayed(rq)) == 1)
			తిరిగి this_cpu;
	}

	ఒకవేళ (అందుబాటు_idle_cpu(prev_cpu))
		తిరిగి prev_cpu;

	తిరిగి nr_cpumask_బిట్లు;
}

స్థిర పూర్ణాంకం
wake_affine_బరువు(నిర్మాణం sched_domain *sd, నిర్మాణం task_struct *p,
		   పూర్ణాంకం this_cpu, పూర్ణాంకం prev_cpu, పూర్ణాంకం sync)
{
	s64 this_eff_ఎక్కించు, prev_eff_ఎక్కించు;
	గుర్తులేని దీర్ఘ task_ఎక్కించు;

	this_eff_ఎక్కించు = cpu_ఎక్కించు(cpu_rq(this_cpu));

	ఒకవేళ (sync) {
		గుర్తులేని దీర్ఘ current_ఎక్కించు = task_h_ఎక్కించు(current);

		ఒకవేళ (current_ఎక్కించు > this_eff_ఎక్కించు)
			తిరిగి this_cpu;

		this_eff_ఎక్కించు -= current_ఎక్కించు;
	}

	task_ఎక్కించు = task_h_ఎక్కించు(p);

	this_eff_ఎక్కించు += task_ఎక్కించు;
	ఒకవేళ (sched_feat(WA_BIAS))
		this_eff_ఎక్కించు *= 100;
	this_eff_ఎక్కించు *= సామర్థ్యం_యొక్క(prev_cpu);

	prev_eff_ఎక్కించు = cpu_ఎక్కించు(cpu_rq(prev_cpu));
	prev_eff_ఎక్కించు -= task_ఎక్కించు;
	ఒకవేళ (sched_feat(WA_BIAS))
		prev_eff_ఎక్కించు *= 100 + (sd->imbalance_pct - 100) / 2;
	prev_eff_ఎక్కించు *= సామర్థ్యం_యొక్క(this_cpu);

	/*
	 * If sync, adjust the weight of prev_eff_load such that if
	 * prev_eff == this_eff that select_idle_sibling() will consider
	 * stacking the wakee on top of the waker if no other CPU is
	 * idle.
	 */
	ఒకవేళ (sync)
		prev_eff_ఎక్కించు += 1;

	తిరిగి this_eff_ఎక్కించు < prev_eff_ఎక్కించు ? this_cpu : nr_cpumask_బిట్లు;
}

స్థిర పూర్ణాంకం wake_affine(నిర్మాణం sched_domain *sd, నిర్మాణం task_struct *p,
		       పూర్ణాంకం this_cpu, పూర్ణాంకం prev_cpu, పూర్ణాంకం sync)
{
	పూర్ణాంకం లక్ష్యం = nr_cpumask_బిట్లు;

	ఒకవేళ (sched_feat(WA_IDLE))
		లక్ష్యం = wake_affine_idle(this_cpu, prev_cpu, sync);

	ఒకవేళ (sched_feat(WA_బరువు) && లక్ష్యం == nr_cpumask_బిట్లు)
		లక్ష్యం = wake_affine_బరువు(sd, p, this_cpu, prev_cpu, sync);

	schedstat_inc(p->stats.nr_wakeups_affine_attempts);
	ఒకవేళ (లక్ష్యం != this_cpu)
		తిరిగి prev_cpu;

	schedstat_inc(sd->ttwu_move_affine);
	schedstat_inc(p->stats.nr_wakeups_affine);
	తిరిగి లక్ష్యం;
}

స్థిర నిర్మాణం sched_సమూహం *
sched_balance_కనుగొను_dst_సమూహం(నిర్మాణం sched_domain *sd, నిర్మాణం task_struct *p, పూర్ణాంకం this_cpu);

/*
 * sched_balance_find_dst_group_cpu - find the idlest CPU among the CPUs in the group.
 */
స్థిర పూర్ణాంకం
sched_balance_కనుగొను_dst_సమూహం_cpu(నిర్మాణం sched_సమూహం *సమూహం, నిర్మాణం task_struct *p, పూర్ణాంకం this_cpu)
{
	గుర్తులేని దీర్ఘ ఎక్కించు, min_ఎక్కించు = ULONG_MAX;
	u64 min_exit_latency = U64_MAX;
	గుర్తులేని పూర్ణాంకం nr_candidates = 0;
	పూర్ణాంకం కనిష్ఠ_loaded_cpu = this_cpu;
	పూర్ణాంకం shallowest_idle_cpu = -1;
	పూర్ణాంకం i;

	/* Check if we have any choice: */
	ఒకవేళ (సమూహం->సమూహం_బరువు == 1)
		తిరిగి cpumask_మొదటి(sched_సమూహం_పరిధి(సమూహం));

	/* Traverse only the allowed CPUs */
	for_each_cpu_and(i, sched_సమూహం_పరిధి(సమూహం), p->cpus_ptr) {
		నిర్మాణం rq *rq = cpu_rq(i);

		ఒకవేళ (!sched_core_cookie_సరిపోలిక(rq, p))
			కొనసాగు;

		ఒకవేళ (ఎంచుకో_sched_idle_rq(rq, p))
			తిరిగి i;

		ఒకవేళ (అందుబాటు_idle_cpu(i)) {
			నిర్మాణం cpuidle_state *idle = idle_get_state(rq);
			u64 exit_latency = idle ? idle->exit_latency : U64_MAX;

			ఒకవేళ (shallowest_idle_cpu == -1 || exit_latency < min_exit_latency) {
				min_exit_latency = exit_latency;
				shallowest_idle_cpu = i;
				nr_candidates = 1;
			} లేకపోతే ఒకవేళ (exit_latency == min_exit_latency) {
				nr_candidates++;
				ఒకవేళ (!reciprocal_scale(sched_rng(), nr_candidates))
					shallowest_idle_cpu = i;
			}
		} లేకపోతే ఒకవేళ (shallowest_idle_cpu == -1) {
			ఎక్కించు = cpu_ఎక్కించు(cpu_rq(i));
			ఒకవేళ (ఎక్కించు < min_ఎక్కించు) {
				min_ఎక్కించు = ఎక్కించు;
				కనిష్ఠ_loaded_cpu = i;
			}
		}
	}

	తిరిగి shallowest_idle_cpu != -1 ? shallowest_idle_cpu : కనిష్ఠ_loaded_cpu;
}

స్థిర అంతఃవరుస పూర్ణాంకం sched_balance_కనుగొను_dst_cpu(నిర్మాణం sched_domain *sd, నిర్మాణం task_struct *p,
				  పూర్ణాంకం cpu, పూర్ణాంకం prev_cpu, పూర్ణాంకం sd_flag)
{
	పూర్ణాంకం new_cpu = cpu;

	ఒకవేళ (!cpumask_intersects(sched_domain_పరిధి(sd), p->cpus_ptr))
		తిరిగి prev_cpu;

	/*
	 * We need task's util for cpu_util_without, sync it up to
	 * prev_cpu's last_update_time.
	 */
	ఒకవేళ (!(sd_flag & SD_BALANCE_FORK))
		sync_entity_ఎక్కించు_avg(&p->se);

	అయితే (sd) {
		నిర్మాణం sched_సమూహం *సమూహం;
		నిర్మాణం sched_domain *tmp;
		పూర్ణాంకం బరువు;

		ఒకవేళ (!(sd->flags & sd_flag)) {
			sd = sd->బిడ్డ;
			కొనసాగు;
		}

		సమూహం = sched_balance_కనుగొను_dst_సమూహం(sd, p, cpu);
		ఒకవేళ (!సమూహం) {
			sd = sd->బిడ్డ;
			కొనసాగు;
		}

		new_cpu = sched_balance_కనుగొను_dst_సమూహం_cpu(సమూహం, p, cpu);
		ఒకవేళ (new_cpu == cpu) {
			/* Now try balancing at a lower domain level of 'cpu': */
			sd = sd->బిడ్డ;
			కొనసాగు;
		}

		/* Now try balancing at a lower domain level of 'new_cpu': */
		cpu = new_cpu;
		బరువు = sd->పరిధి_బరువు;
		sd = ఖాళీ;
		for_each_domain(cpu, tmp) {
			ఒకవేళ (బరువు <= tmp->పరిధి_బరువు)
				ఆపు;
			ఒకవేళ (tmp->flags & sd_flag)
				sd = tmp;
		}
	}

	తిరిగి new_cpu;
}

స్థిర అంతఃవరుస పూర్ణాంకం __select_idle_cpu(పూర్ణాంకం cpu, నిర్మాణం task_struct *p)
{
	ఒకవేళ (ఎంచుకో_idle_cpu(cpu, p) && sched_cpu_cookie_సరిపోలిక(cpu_rq(cpu), p))
		తిరిగి cpu;

	తిరిగి -1;
}

DEFINE_STATIC_కీలకం_FALSE(sched_smt_present);
EXPORT_చిహ్నం_GPL(sched_smt_present);

స్థిర అంతఃవరుస శూన్య సమితి_idle_cores(పూర్ణాంకం cpu, పూర్ణాంకం val)
{
	నిర్మాణం sched_domain_shared *sds;

	sds = rcu_dereference_all(చొప్పున_cpu(sd_balance_shared, cpu));
	ఒకవేళ (sds)
		WRITE_ONCE(sds->has_idle_cores, val);
}

స్థిర అంతఃవరుస bool test_idle_cores(పూర్ణాంకం cpu)
{
	నిర్మాణం sched_domain_shared *sds;

	sds = rcu_dereference_all(చొప్పున_cpu(sd_balance_shared, cpu));
	ఒకవేళ (sds)
		తిరిగి READ_ONCE(sds->has_idle_cores);

	తిరిగి false;
}

/*
 * Redirect a CPU to a higher-priority available sibling in its SMT domain,
 * subject to task affinity.
 */
స్థిర అంతఃవరుస పూర్ణాంకం select_idle_smt_cpu(నిర్మాణం task_struct *p, పూర్ణాంకం cpu)
{
	నిర్మాణం sched_domain *sd;
	పూర్ణాంకం ఉత్తమ = cpu;
	పూర్ణాంకం sibling;

	ఒకవేళ (!sched_smt_చురుకైన())
		తిరిగి cpu;

	sd = rcu_dereference_all(cpu_rq(cpu)->sd);
	ఒకవేళ (!sd || !(sd->flags & SD_SHARE_CPUCAPACITY) ||
	    !(sd->flags & SD_ASYM_PACKING))
		తిరిగి cpu;

	for_each_cpu_and(sibling, sched_domain_పరిధి(sd), p->cpus_ptr) {
		ఒకవేళ (sibling == ఉత్తమ || !ఎంచుకో_idle_cpu(sibling, p))
			కొనసాగు;

		ఒకవేళ (sched_asym_prefer(sibling, ఉత్తమ))
			ఉత్తమ = sibling;
	}

	తిరిగి ఉత్తమ;
}

/*
 * Scans the local SMT mask to see if the entire core is idle, and records this
 * information in sd_balance_shared->has_idle_cores.
 *
 * Since SMT siblings share all cache levels, inspecting this limited remote
 * state should be fairly cheap.
 */
శూన్య __update_idle_core(నిర్మాణం rq *rq)
{
	పూర్ణాంకం core = cpu_యొక్క(rq);
	పూర్ణాంకం cpu;

	rcu_read_తాళం();
	ఒకవేళ (test_idle_cores(core))
		వెళ్ళు unlock;

	for_each_cpu(cpu, cpu_smt_ముసుగు(core)) {
		ఒకవేళ (cpu == core)
			కొనసాగు;

		ఒకవేళ (!అందుబాటు_idle_cpu(cpu))
			వెళ్ళు unlock;
	}

	సమితి_idle_cores(core, 1);
unlock:
	rcu_read_unlock();
}

/*
 * Scan the entire LLC domain for idle cores; this dynamically switches off if
 * there are no idle cores left in the system; tracked through
 * sd_balance_shared->has_idle_cores and enabled through update_idle_core()
 * above.
 */
స్థిర పూర్ణాంకం select_idle_core(నిర్మాణం task_struct *p, పూర్ణాంకం core, నిర్మాణం cpumask *cpus, పూర్ణాంకం *idle_cpu)
{
	bool idle = true;
	పూర్ణాంకం cpu;

	for_each_cpu(cpu, cpu_smt_ముసుగు(core)) {
		ఒకవేళ (!అందుబాటు_idle_cpu(cpu)) {
			idle = false;
			ఒకవేళ (*idle_cpu == -1) {
				ఒకవేళ (ఎంచుకో_sched_idle_rq(cpu_rq(cpu), p) &&
				    cpumask_test_cpu(cpu, cpus)) {
					*idle_cpu = cpu;
					ఆపు;
				}
				కొనసాగు;
			}
			ఆపు;
		}
		ఒకవేళ (*idle_cpu == -1 && cpumask_test_cpu(cpu, cpus))
			*idle_cpu = cpu;
	}

	ఒకవేళ (idle)
		తిరిగి core;

	cpumask_andnot(cpus, cpus, cpu_smt_ముసుగు(core));
	తిరిగి -1;
}

/*
 * Scan the local SMT mask for idle CPUs.
 */
స్థిర పూర్ణాంకం select_idle_smt(నిర్మాణం task_struct *p, నిర్మాణం sched_domain *sd, పూర్ణాంకం లక్ష్యం)
{
	పూర్ణాంకం cpu;

	for_each_cpu_and(cpu, cpu_smt_ముసుగు(లక్ష్యం), p->cpus_ptr) {
		ఒకవేళ (cpu == లక్ష్యం)
			కొనసాగు;
		/*
		 * Check if the CPU is in the LLC scheduling domain of @target.
		 * Due to isolcpus, there is no guarantee that all the siblings are in the domain.
		 */
		ఒకవేళ (!cpumask_test_cpu(cpu, sched_domain_పరిధి(sd)))
			కొనసాగు;
		ఒకవేళ (ఎంచుకో_idle_cpu(cpu, p))
			తిరిగి cpu;
	}

	తిరిగి -1;
}

/*
 * Scan the LLC domain for idle CPUs; this is dynamically regulated by
 * comparing the average scan cost (tracked in sd->avg_scan_cost) against the
 * average idle time for this rq (as found in rq->avg_idle).
 */
స్థిర పూర్ణాంకం select_idle_cpu(నిర్మాణం task_struct *p, నిర్మాణం sched_domain *sd, bool has_idle_core, పూర్ణాంకం లక్ష్యం)
{
	నిర్మాణం cpumask *cpus = this_cpu_cpumask_var_ptr(select_rq_ముసుగు);
	పూర్ణాంకం i, cpu, idle_cpu = -1, nr = గరిష్ఠ_పూర్ణాంకం;

	ఒకవేళ (sched_feat(SIS_UTIL) && sd->shared) {
		/*
		 * Increment because !--nr is the condition to stop scan.
		 *
		 * Since "sd" is "sd_llc" for target CPU dereferenced in the
		 * caller, it is safe to directly dereference "sd->shared".
		 * Topology bits always ensure it assigned for "sd_llc" abd it
		 * cannot disappear as long as we have a RCU protected
		 * reference to one the associated "sd" here.
		 */
		nr = READ_ONCE(sd->shared->nr_idle_పరిశీలన) + 1;
		/* overloaded LLC is unlikely to have idle cpu/core */
		ఒకవేళ (nr == 1)
			తిరిగి -1;
	}

	ఒకవేళ (!cpumask_and(cpus, sched_domain_పరిధి(sd), p->cpus_ptr))
		తిరిగి -1;

	ఒకవేళ (static_branch_unlikely(&sched_cluster_చురుకైన)) {
		నిర్మాణం sched_సమూహం *sg = sd->groups;

		ఒకవేళ (sg->flags & SD_CLUSTER) {
			for_each_cpu_wrap(cpu, sched_సమూహం_పరిధి(sg), లక్ష్యం + 1) {
				ఒకవేళ (!cpumask_test_cpu(cpu, cpus))
					కొనసాగు;

				ఒకవేళ (has_idle_core) {
					i = select_idle_core(p, cpu, cpus, &idle_cpu);
					ఒకవేళ ((గుర్తులేని పూర్ణాంకం)i < nr_cpumask_బిట్లు)
						తిరిగి i;
				} లేకపోతే {
					ఒకవేళ (--nr <= 0)
						తిరిగి -1;
					idle_cpu = __select_idle_cpu(cpu, p);
					ఒకవేళ ((గుర్తులేని పూర్ణాంకం)idle_cpu < nr_cpumask_బిట్లు)
						తిరిగి idle_cpu;
				}
			}
			cpumask_andnot(cpus, cpus, sched_సమూహం_పరిధి(sg));
		}
	}

	for_each_cpu_wrap(cpu, cpus, లక్ష్యం + 1) {
		ఒకవేళ (has_idle_core) {
			i = select_idle_core(p, cpu, cpus, &idle_cpu);
			ఒకవేళ ((గుర్తులేని పూర్ణాంకం)i < nr_cpumask_బిట్లు)
				తిరిగి i;

		} లేకపోతే {
			ఒకవేళ (--nr <= 0)
				తిరిగి -1;
			idle_cpu = __select_idle_cpu(cpu, p);
			ఒకవేళ ((గుర్తులేని పూర్ణాంకం)idle_cpu < nr_cpumask_బిట్లు)
				ఆపు;
		}
	}

	ఒకవేళ (has_idle_core)
		సమితి_idle_cores(లక్ష్యం, false);

	తిరిగి idle_cpu;
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
గణన asym_సరిపోతుంది_state {
	ASYM_IDLE_UCLAMP_MISFIT = -4,
	ASYM_IDLE_COMPLETE_MISFIT,
	ASYM_IDLE_THREAD_సరిపోతుంది,
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
స్థిర పూర్ణాంకం
select_idle_సామర్థ్యం(నిర్మాణం task_struct *p, నిర్మాణం sched_domain *sd, పూర్ణాంకం లక్ష్యం)
{
	/*
	 * On !SMT systems, has_idle_core is always false and preferred_core
	 * is always true (CPU == core), so the SMT preference logic below
	 * collapses to the plain capacity scan.
	 */
	bool has_idle_core = sched_smt_చురుకైన() && test_idle_cores(లక్ష్యం);
	గుర్తులేని దీర్ఘ task_util, util_min, util_max, ఉత్తమ_cap = 0;
	పూర్ణాంకం సరిపోతుంది, ఉత్తమ_సరిపోతుంది = ASYM_IDLE_THREAD_MISFIT;
	పూర్ణాంకం cpu, ఉత్తమ_cpu = -1;
	నిర్మాణం cpumask *cpus;
	పూర్ణాంకం nr = గరిష్ఠ_పూర్ణాంకం;

	cpus = this_cpu_cpumask_var_ptr(select_rq_ముసుగు);
	cpumask_and(cpus, sched_domain_పరిధి(sd), p->cpus_ptr);

	task_util = task_util_est(p);
	util_min = uclamp_eff_విలువ(p, UCLAMP_MIN);
	util_max = uclamp_eff_విలువ(p, UCLAMP_MAX);

	ఒకవేళ (sched_feat(SIS_UTIL) && sd->shared) {
		/*
		 * Same nr_idle_scan hint as select_idle_cpu(), nr only limits
		 * the scan when not preferring an idle core.
		 */
		nr = READ_ONCE(sd->shared->nr_idle_పరిశీలన) + 1;
		/* overloaded domain is unlikely to have idle cpu/core */
		ఒకవేళ (nr == 1)
			తిరిగి -1;
	}

	for_each_cpu_wrap(cpu, cpus, లక్ష్యం) {
		bool preferred_core = !has_idle_core || is_core_idle(cpu);
		గుర్తులేని దీర్ఘ cpu_cap = సామర్థ్యం_యొక్క(cpu);

		/*
		 * Stop when the nr_idle_scan is exhausted (mirrors
		 * select_idle_cpu() logic).
		 */
		ఒకవేళ (!has_idle_core && --nr <= 0)
			తిరిగి ఉత్తమ_cpu;

		ఒకవేళ (!ఎంచుకో_idle_cpu(cpu, p))
			కొనసాగు;

		సరిపోతుంది = util_సరిపోతుంది_cpu(task_util, util_min, util_max, cpu);

		/*
		 * Perfect fit: capacity satisfies util + uclamp and the CPU
		 * sits on a fully-idle SMT core, this is a !SMT system, or
		 * there is no idle core to find.
		 * Short-circuit the rank-based selection and return
		 * immediately.
		 */
		ఒకవేళ (సరిపోతుంది > 0 && preferred_core)
			తిరిగి cpu;
		/*
		 * Only the min performance hint (i.e. uclamp_min) doesn't fit.
		 * Look for the CPU with best capacity.
		 */
		లేకపోతే ఒకవేళ (సరిపోతుంది < 0)
			cpu_cap = get_actual_cpu_సామర్థ్యం(cpu);
		/*
		 * fits > 0 implies we are not on a preferred core, but the util
		 * fits CPU capacity. Set fits to ASYM_IDLE_THREAD_FITS
		 * so the effective range becomes
		 * [ASYM_IDLE_THREAD_FITS, ASYM_IDLE_THREAD_MISFIT], where:
		 *    ASYM_IDLE_THREAD_MISFIT - does not fit
		 *    ASYM_IDLE_THREAD_UCLAMP_MISFIT - fits with the exception of UCLAMP_MIN
		 *    ASYM_IDLE_THREAD_FITS - fits with the exception of preferred_core
		 */
		లేకపోతే ఒకవేళ (సరిపోతుంది > 0)
			సరిపోతుంది = ASYM_IDLE_THREAD_సరిపోతుంది;

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
		ఒకవేళ (preferred_core)
			సరిపోతుంది += ASYM_IDLE_CORE_BIAS;

		/*
		 * First, select CPU which fits better (lower is more preferred).
		 * Then, select the one with best capacity at same level.
		 */
		ఒకవేళ ((సరిపోతుంది < ఉత్తమ_సరిపోతుంది) ||
		    ((సరిపోతుంది == ఉత్తమ_సరిపోతుంది) && (cpu_cap > ఉత్తమ_cap))) {
			ఉత్తమ_cap = cpu_cap;
			ఉత్తమ_cpu = cpu;
			ఉత్తమ_సరిపోతుంది = సరిపోతుంది;
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
	ఒకవేళ (has_idle_core && ఉత్తమ_సరిపోతుంది > ASYM_IDLE_COMPLETE_MISFIT)
		సమితి_idle_cores(లక్ష్యం, false);

	తిరిగి ఉత్తమ_cpu;
}

స్థిర అంతఃవరుస bool asym_సరిపోతుంది_cpu(గుర్తులేని దీర్ఘ util,
				 గుర్తులేని దీర్ఘ util_min,
				 గుర్తులేని దీర్ఘ util_max,
				 పూర్ణాంకం cpu)
{
	ఒకవేళ (sched_asym_cpucap_చురుకైన()) {
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
		తిరిగి (!sched_smt_చురుకైన() || is_core_idle(cpu)) &&
		       (util_సరిపోతుంది_cpu(util, util_min, util_max, cpu) > 0);
	}

	తిరిగి true;
}

/*
 * Try and locate an idle core/thread in the LLC cache domain.
 */
స్థిర పూర్ణాంకం select_idle_sibling(నిర్మాణం task_struct *p, పూర్ణాంకం prev, పూర్ణాంకం లక్ష్యం)
{
	bool has_idle_core = false;
	నిర్మాణం sched_domain *sd;
	గుర్తులేని దీర్ఘ task_util, util_min, util_max;
	పూర్ణాంకం i, recent_వాడిన_cpu, prev_aff = -1;

	/*
	 * On asymmetric system, update task utilization because we will check
	 * that the task fits with CPU's capacity.
	 */
	ఒకవేళ (sched_asym_cpucap_చురుకైన()) {
		sync_entity_ఎక్కించు_avg(&p->se);
		task_util = task_util_est(p);
		util_min = uclamp_eff_విలువ(p, UCLAMP_MIN);
		util_max = uclamp_eff_విలువ(p, UCLAMP_MAX);
	}

	/*
	 * per-cpu select_rq_mask usage
	 */
	lockdep_assert_irqs_disabled();

	ఒకవేళ (ఎంచుకో_idle_cpu(లక్ష్యం, p) &&
	    asym_సరిపోతుంది_cpu(task_util, util_min, util_max, లక్ష్యం))
		వెళ్ళు select_smt_ప్రాధాన్యత;

	/*
	 * If the previous CPU is cache affine and idle, don't be stupid:
	 */
	ఒకవేళ (prev != లక్ష్యం && cpus_share_cache(prev, లక్ష్యం) &&
	    ఎంచుకో_idle_cpu(prev, p) &&
	    asym_సరిపోతుంది_cpu(task_util, util_min, util_max, prev)) {

		ఒకవేళ (!static_branch_unlikely(&sched_cluster_చురుకైన) ||
		    cpus_share_resources(prev, లక్ష్యం)) {
			లక్ష్యం = prev;
			వెళ్ళు select_smt_ప్రాధాన్యత;
		}

		prev_aff = prev;
	}

	/*
	 * Allow a per-cpu kthread to stack with the wakee if the
	 * kworker thread and the tasks previous CPUs are the same.
	 * The assumption is that the wakee queued work for the
	 * per-cpu kthread that is now complete and the wakeup is
	 * essentially a sync wakeup. An obvious example of this
	 * pattern is IO completions.
	 */
	ఒకవేళ (is_చొప్పున_cpu_kthread(current) &&
	    లోపల_task() &&
	    prev == smp_processor_id() &&
	    this_rq()->nr_running <= 1 &&
	    asym_సరిపోతుంది_cpu(task_util, util_min, util_max, prev)) {
		లక్ష్యం = prev;
		వెళ్ళు select_smt_ప్రాధాన్యత;
	}

	/* Check a recently used CPU as a potential idle candidate: */
	recent_వాడిన_cpu = p->recent_వాడిన_cpu;
	p->recent_వాడిన_cpu = prev;
	ఒకవేళ (recent_వాడిన_cpu != prev &&
	    recent_వాడిన_cpu != లక్ష్యం &&
	    cpus_share_cache(recent_వాడిన_cpu, లక్ష్యం) &&
	    ఎంచుకో_idle_cpu(recent_వాడిన_cpu, p) &&
	    cpumask_test_cpu(recent_వాడిన_cpu, p->cpus_ptr) &&
	    asym_సరిపోతుంది_cpu(task_util, util_min, util_max, recent_వాడిన_cpu)) {

		ఒకవేళ (!static_branch_unlikely(&sched_cluster_చురుకైన) ||
		    cpus_share_resources(recent_వాడిన_cpu, లక్ష్యం)) {
			లక్ష్యం = recent_వాడిన_cpu;
			వెళ్ళు select_smt_ప్రాధాన్యత;
		}

	} లేకపోతే {
		recent_వాడిన_cpu = -1;
	}

	/*
	 * For asymmetric CPU capacity systems, our domain of interest is
	 * sd_asym_cpucapacity rather than sd_llc.
	 */
	ఒకవేళ (sched_asym_cpucap_చురుకైన()) {
		sd = rcu_dereference_all(చొప్పున_cpu(sd_asym_cpucapacity, లక్ష్యం));
		/*
		 * On an asymmetric CPU capacity system where an exclusive
		 * cpuset defines a symmetric island (i.e. one unique
		 * capacity_orig value through the cpuset), the key will be set
		 * but the CPUs within that cpuset will not have a domain with
		 * SD_ASYM_CPUCAPACITY. These should follow the usual symmetric
		 * capacity path.
		 */
		ఒకవేళ (sd) {
			i = select_idle_సామర్థ్యం(p, sd, లక్ష్యం);
			ఒకవేళ ((గుర్తులేని పూర్ణాంకం)i < nr_cpumask_బిట్లు) {
				లక్ష్యం = i;
				వెళ్ళు select_smt_ప్రాధాన్యత;
			}
			తిరిగి లక్ష్యం;
		}
	}

	sd = rcu_dereference_all(చొప్పున_cpu(sd_llc, లక్ష్యం));
	ఒకవేళ (!sd)
		తిరిగి లక్ష్యం;

	ఒకవేళ (sched_smt_చురుకైన()) {
		has_idle_core = test_idle_cores(లక్ష్యం);

		ఒకవేళ (!has_idle_core && cpus_share_cache(prev, లక్ష్యం)) {
			i = select_idle_smt(p, sd, prev);
			ఒకవేళ ((గుర్తులేని పూర్ణాంకం)i < nr_cpumask_బిట్లు) {
				లక్ష్యం = i;
				వెళ్ళు select_smt_ప్రాధాన్యత;
			}
		}
	}

	i = select_idle_cpu(p, sd, has_idle_core, లక్ష్యం);
	ఒకవేళ ((గుర్తులేని పూర్ణాంకం)i < nr_cpumask_బిట్లు) {
		లక్ష్యం = i;
		వెళ్ళు select_smt_ప్రాధాన్యత;
	}

	/*
	 * For cluster machines which have lower sharing cache like L2 or
	 * LLC Tag, we tend to find an idle CPU in the target's cluster
	 * first. But prev_cpu or recent_used_cpu may also be a good candidate,
	 * use them if possible when no idle CPU found in select_idle_cpu().
	 */
	ఒకవేళ ((గుర్తులేని పూర్ణాంకం)prev_aff < nr_cpumask_బిట్లు) {
		లక్ష్యం = prev_aff;
		వెళ్ళు select_smt_ప్రాధాన్యత;
	}
	ఒకవేళ ((గుర్తులేని పూర్ణాంకం)recent_వాడిన_cpu < nr_cpumask_బిట్లు) {
		లక్ష్యం = recent_వాడిన_cpu;
		వెళ్ళు select_smt_ప్రాధాన్యత;
	}

	తిరిగి లక్ష్యం;

select_smt_ప్రాధాన్యత:
	తిరిగి select_idle_smt_cpu(p, లక్ష్యం);
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
స్థిర గుర్తులేని దీర్ఘ
cpu_util(పూర్ణాంకం cpu, నిర్మాణం task_struct *p, పూర్ణాంకం dst_cpu, పూర్ణాంకం boost)
{
	bool కూడు_task = p && task_cpu(p) != cpu && dst_cpu == cpu;
	bool sub_task = p && task_cpu(p) == cpu && dst_cpu != cpu;
	నిర్మాణం cfs_rq *cfs_rq = &cpu_rq(cpu)->cfs;
	గుర్తులేని దీర్ఘ util = READ_ONCE(cfs_rq->avg.util_avg);
	గుర్తులేని దీర్ఘ runnable;

	/*
	 * If @dst_cpu is -1 or @p migrates from @cpu to @dst_cpu remove its
	 * contribution. If @p migrates from another CPU to @cpu add its
	 * contribution. In all the other cases @cpu is not impacted by the
	 * migration so its util_avg is already correct.
	 */
	ఒకవేళ (కూడు_task)
		util += task_util(p);
	లేకపోతే ఒకవేళ (sub_task)
		lsub_ధనాత్మక(&util, task_util(p));

	ఒకవేళ (boost) {
		runnable = READ_ONCE(cfs_rq->avg.runnable_avg);
		ఒకవేళ (కూడు_task)
			runnable += READ_ONCE(p->se.avg.runnable_avg);
		లేకపోతే ఒకవేళ (sub_task)
			lsub_ధనాత్మక(&runnable,
				      READ_ONCE(p->se.avg.runnable_avg));
		util = max(util, runnable);
	}

	ఒకవేళ (sched_feat(UTIL_EST)) {
		గుర్తులేని దీర్ఘ util_est;

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
		ఒకవేళ (dst_cpu == cpu)
			util_est += _task_util_est(p);
		లేకపోతే ఒకవేళ (p && unlikely(task_on_rq_queued(p) || current == p))
			lsub_ధనాత్మక(&util_est, _task_util_est(p));

		util = max(util, util_est);
	}

	తిరిగి min(util, arch_scale_cpu_సామర్థ్యం(cpu));
}

గుర్తులేని దీర్ఘ cpu_util_cfs(పూర్ణాంకం cpu)
{
	తిరిగి cpu_util(cpu, ఖాళీ, -1, 0);
}

గుర్తులేని దీర్ఘ cpu_util_cfs_boost(పూర్ణాంకం cpu)
{
	తిరిగి cpu_util(cpu, ఖాళీ, -1, 1);
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
స్థిర గుర్తులేని దీర్ఘ cpu_util_without(పూర్ణాంకం cpu, నిర్మాణం task_struct *p)
{
	/* Task has no contribution or is new */
	ఒకవేళ (cpu != task_cpu(p) || !READ_ONCE(p->se.avg.చివరి_update_సమయం))
		p = ఖాళీ;

	తిరిగి cpu_util(cpu, p, -1, 0);
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
గుర్తులేని దీర్ఘ effective_cpu_util(పూర్ణాంకం cpu, గుర్తులేని దీర్ఘ util_cfs,
				 గుర్తులేని దీర్ఘ *min,
				 గుర్తులేని దీర్ఘ *max)
{
	గుర్తులేని దీర్ఘ util, irq, scale;
	నిర్మాణం rq *rq = cpu_rq(cpu);

	scale = arch_scale_cpu_సామర్థ్యం(cpu);

	/*
	 * Early check to see if IRQ/steal time saturates the CPU, can be
	 * because of inaccuracies in how we track these -- see
	 * update_irq_load_avg().
	 */
	irq = cpu_util_irq(rq);
	ఒకవేళ (unlikely(irq >= scale)) {
		ఒకవేళ (min)
			*min = scale;
		ఒకవేళ (max)
			*max = scale;
		తిరిగి scale;
	}

	ఒకవేళ (min) {
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
		ఒకవేళ (!uclamp_is_వాడిన() && rt_rq_is_runnable(&rq->rt))
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
	ఒకవేళ (max)
		*max = min(scale, uclamp_rq_get(rq, UCLAMP_MAX));

	ఒకవేళ (util >= scale)
		తిరిగి scale;

	/*
	 * There is still idle time; further improve the number by using the
	 * IRQ metric. Because IRQ/steal time is hidden from the task clock we
	 * need to scale the task numbers:
	 *
	 *              max - irq
	 *   U' = irq + --------- * U
	 *                 max
	 */
	util = scale_irq_సామర్థ్యం(util, irq, scale);
	util += irq;

	తిరిగి min(scale, util);
}

గుర్తులేని దీర్ఘ sched_cpu_util(పూర్ణాంకం cpu)
{
	తిరిగి effective_cpu_util(cpu, cpu_util_cfs(cpu), ఖాళీ, ఖాళీ);
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
నిర్మాణం energy_env {
	గుర్తులేని దీర్ఘ task_busy_సమయం;
	గుర్తులేని దీర్ఘ pd_busy_సమయం;
	గుర్తులేని దీర్ఘ cpu_cap;
	గుర్తులేని దీర్ఘ pd_cap;
};

/*
 * Compute the task busy time for compute_energy(). This time cannot be
 * injected directly into effective_cpu_util() because of the IRQ scaling.
 * The latter only makes sense with the most recent CPUs where the task has
 * run.
 */
స్థిర అంతఃవరుస శూన్య eenv_task_busy_సమయం(నిర్మాణం energy_env *eenv,
				       నిర్మాణం task_struct *p, పూర్ణాంకం prev_cpu)
{
	గుర్తులేని దీర్ఘ busy_సమయం, max_cap = arch_scale_cpu_సామర్థ్యం(prev_cpu);
	గుర్తులేని దీర్ఘ irq = cpu_util_irq(cpu_rq(prev_cpu));

	ఒకవేళ (unlikely(irq >= max_cap))
		busy_సమయం = max_cap;
	లేకపోతే
		busy_సమయం = scale_irq_సామర్థ్యం(task_util_est(p), irq, max_cap);

	eenv->task_busy_సమయం = busy_సమయం;
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
స్థిర అంతఃవరుస శూన్య eenv_pd_busy_సమయం(నిర్మాణం energy_env *eenv,
				     నిర్మాణం cpumask *pd_cpus,
				     నిర్మాణం task_struct *p)
{
	గుర్తులేని దీర్ఘ busy_సమయం = 0;
	పూర్ణాంకం cpu;

	for_each_cpu(cpu, pd_cpus) {
		గుర్తులేని దీర్ఘ util = cpu_util(cpu, p, -1, 0);

		busy_సమయం += effective_cpu_util(cpu, util, ఖాళీ, ఖాళీ);
	}

	eenv->pd_busy_సమయం = min(eenv->pd_cap, busy_సమయం);
}

/*
 * Compute the maximum utilization for compute_energy() when the task @p
 * is placed on the cpu @dst_cpu.
 *
 * Returns the maximum utilization among @eenv->cpus. This utilization can't
 * exceed @eenv->cpu_cap.
 */
స్థిర అంతఃవరుస గుర్తులేని దీర్ఘ
eenv_pd_max_util(నిర్మాణం energy_env *eenv, నిర్మాణం cpumask *pd_cpus,
		 నిర్మాణం task_struct *p, పూర్ణాంకం dst_cpu)
{
	గుర్తులేని దీర్ఘ max_util = 0;
	పూర్ణాంకం cpu;

	for_each_cpu(cpu, pd_cpus) {
		నిర్మాణం task_struct *tsk = (cpu == dst_cpu) ? p : ఖాళీ;
		గుర్తులేని దీర్ఘ util = cpu_util(cpu, p, dst_cpu, 1);
		గుర్తులేని దీర్ఘ eff_util, min, max;

		/*
		 * Performance domain frequency: utilization clamping
		 * must be considered since it affects the selection
		 * of the performance domain frequency.
		 * NOTE: in case RT tasks are running, by default the min
		 * utilization can be max OPP.
		 */
		eff_util = effective_cpu_util(cpu, util, &min, &max);

		/* Task's uclamp can modify min and max value */
		ఒకవేళ (tsk && uclamp_is_వాడిన()) {
			min = max(min, uclamp_eff_విలువ(p, UCLAMP_MIN));

			/*
			 * If there is no active max uclamp constraint,
			 * directly use task's one, otherwise keep max.
			 */
			ఒకవేళ (uclamp_rq_is_idle(cpu_rq(cpu)))
				max = uclamp_eff_విలువ(p, UCLAMP_MAX);
			లేకపోతే
				max = max(max, uclamp_eff_విలువ(p, UCLAMP_MAX));
		}

		eff_util = sugov_effective_cpu_perf(cpu, eff_util, min, max);
		max_util = max(max_util, eff_util);
	}

	తిరిగి min(max_util, eenv->cpu_cap);
}

/*
 * compute_energy(): Use the Energy Model to estimate the energy that @pd would
 * consume for a given utilization landscape @eenv. When @dst_cpu < 0, the task
 * contribution is ignored.
 */
స్థిర అంతఃవరుస గుర్తులేని దీర్ఘ
compute_energy(నిర్మాణం energy_env *eenv, నిర్మాణం perf_domain *pd,
	       నిర్మాణం cpumask *pd_cpus, నిర్మాణం task_struct *p, పూర్ణాంకం dst_cpu)
{
	గుర్తులేని దీర్ఘ max_util = eenv_pd_max_util(eenv, pd_cpus, p, dst_cpu);
	గుర్తులేని దీర్ఘ busy_సమయం = eenv->pd_busy_సమయం;
	గుర్తులేని దీర్ఘ energy;

	ఒకవేళ (dst_cpu >= 0)
		busy_సమయం = min(eenv->pd_cap, busy_సమయం + eenv->task_busy_సమయం);

	energy = em_cpu_energy(pd->em_pd, max_util, busy_సమయం, eenv->cpu_cap);

	trace_sched_compute_energy_tp(p, dst_cpu, energy, max_util, busy_సమయం);

	తిరిగి energy;
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
స్థిర పూర్ణాంకం కనుగొను_energy_efficient_cpu(నిర్మాణం task_struct *p, పూర్ణాంకం prev_cpu)
{
	నిర్మాణం cpumask *cpus = this_cpu_cpumask_var_ptr(select_rq_ముసుగు);
	గుర్తులేని దీర్ఘ prev_delta = ULONG_MAX, ఉత్తమ_delta = ULONG_MAX;
	గుర్తులేని దీర్ఘ p_util_min = uclamp_is_వాడిన() ? uclamp_eff_విలువ(p, UCLAMP_MIN) : 0;
	గుర్తులేని దీర్ఘ p_util_max = uclamp_is_వాడిన() ? uclamp_eff_విలువ(p, UCLAMP_MAX) : 1024;
	నిర్మాణం మూలం_domain *rd = this_rq()->rd;
	పూర్ణాంకం cpu, ఉత్తమ_energy_cpu, లక్ష్యం = -1;
	పూర్ణాంకం prev_సరిపోతుంది = -1, ఉత్తమ_సరిపోతుంది = -1;
	గుర్తులేని దీర్ఘ ఉత్తమ_actual_cap = 0;
	గుర్తులేని దీర్ఘ prev_actual_cap = 0;
	నిర్మాణం sched_domain *sd;
	నిర్మాణం perf_domain *pd;
	నిర్మాణం energy_env eenv;

	pd = rcu_dereference_all(rd->pd);
	ఒకవేళ (!pd)
		తిరిగి లక్ష్యం;

	/*
	 * Energy-aware wake-up happens on the lowest sched_domain starting
	 * from sd_asym_cpucapacity spanning over this_cpu and prev_cpu.
	 */
	sd = rcu_dereference_all(*this_cpu_ptr(&sd_asym_cpucapacity));
	అయితే (sd && !cpumask_test_cpu(prev_cpu, sched_domain_పరిధి(sd)))
		sd = sd->మాతృ;
	ఒకవేళ (!sd)
		తిరిగి లక్ష్యం;

	లక్ష్యం = prev_cpu;

	sync_entity_ఎక్కించు_avg(&p->se);
	ఒకవేళ (!task_util_est(p) && p_util_min == 0)
		తిరిగి లక్ష్యం;

	eenv_task_busy_సమయం(&eenv, p, prev_cpu);

	కొరకు (; pd; pd = pd->తదుపరి) {
		గుర్తులేని దీర్ఘ util_min = p_util_min, util_max = p_util_max;
		గుర్తులేని దీర్ఘ cpu_cap, cpu_actual_cap, util;
		దీర్ఘ prev_అదనపు_cap = -1, max_అదనపు_cap = -1;
		గుర్తులేని దీర్ఘ rq_util_min, rq_util_max;
		గుర్తులేని దీర్ఘ cur_delta, పునాది_energy;
		పూర్ణాంకం max_అదనపు_cap_cpu = -1;
		పూర్ణాంకం సరిపోతుంది, max_సరిపోతుంది = -1;

		ఒకవేళ (!cpumask_and(cpus, perf_domain_పరిధి(pd), cpu_online_ముసుగు))
			కొనసాగు;

		/* Account external pressure for the energy estimation */
		cpu = cpumask_మొదటి(cpus);
		cpu_actual_cap = get_actual_cpu_సామర్థ్యం(cpu);

		eenv.cpu_cap = cpu_actual_cap;
		eenv.pd_cap = 0;

		for_each_cpu(cpu, cpus) {
			నిర్మాణం rq *rq = cpu_rq(cpu);

			eenv.pd_cap += cpu_actual_cap;

			ఒకవేళ (!cpumask_test_cpu(cpu, sched_domain_పరిధి(sd)))
				కొనసాగు;

			ఒకవేళ (!cpumask_test_cpu(cpu, p->cpus_ptr))
				కొనసాగు;

			util = cpu_util(cpu, p, cpu, 0);
			cpu_cap = సామర్థ్యం_యొక్క(cpu);

			/*
			 * Skip CPUs that cannot satisfy the capacity request.
			 * IOW, placing the task there would make the CPU
			 * overutilized. Take uclamp into account to see how
			 * much capacity we can get out of the CPU; this is
			 * aligned with sched_cpu_util().
			 */
			ఒకవేళ (uclamp_is_వాడిన() && !uclamp_rq_is_idle(rq)) {
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

			సరిపోతుంది = util_సరిపోతుంది_cpu(util, util_min, util_max, cpu);
			ఒకవేళ (!సరిపోతుంది)
				కొనసాగు;

			lsub_ధనాత్మక(&cpu_cap, util);

			ఒకవేళ (cpu == prev_cpu) {
				/* Always use prev_cpu as a candidate. */
				prev_అదనపు_cap = cpu_cap;
				prev_సరిపోతుంది = సరిపోతుంది;
			} లేకపోతే ఒకవేళ ((సరిపోతుంది > max_సరిపోతుంది) ||
				   ((సరిపోతుంది == max_సరిపోతుంది) && ((దీర్ఘ)cpu_cap > max_అదనపు_cap))) {
				/*
				 * Find the CPU with the maximum spare capacity
				 * among the remaining CPUs in the performance
				 * domain.
				 */
				max_అదనపు_cap = cpu_cap;
				max_అదనపు_cap_cpu = cpu;
				max_సరిపోతుంది = సరిపోతుంది;
			}
		}

		ఒకవేళ (max_అదనపు_cap_cpu < 0 && prev_అదనపు_cap < 0)
			కొనసాగు;

		eenv_pd_busy_సమయం(&eenv, cpus, p);
		/* Compute the 'base' energy of the pd, without @p */
		పునాది_energy = compute_energy(&eenv, pd, cpus, p, -1);

		/* Evaluate the energy impact of using prev_cpu. */
		ఒకవేళ (prev_అదనపు_cap > -1) {
			prev_delta = compute_energy(&eenv, pd, cpus, p,
						    prev_cpu);
			/* CPU utilization has changed */
			ఒకవేళ (prev_delta < పునాది_energy)
				తిరిగి లక్ష్యం;
			prev_delta -= పునాది_energy;
			prev_actual_cap = cpu_actual_cap;
			ఉత్తమ_delta = min(ఉత్తమ_delta, prev_delta);
		}

		/* Evaluate the energy impact of using max_spare_cap_cpu. */
		ఒకవేళ (max_అదనపు_cap_cpu >= 0 && max_అదనపు_cap > prev_అదనపు_cap) {
			/* Current best energy cpu fits better */
			ఒకవేళ (max_సరిపోతుంది < ఉత్తమ_సరిపోతుంది)
				కొనసాగు;

			/*
			 * Both don't fit performance hint (i.e. uclamp_min)
			 * but best energy cpu has better capacity.
			 */
			ఒకవేళ ((max_సరిపోతుంది < 0) &&
			    (cpu_actual_cap <= ఉత్తమ_actual_cap))
				కొనసాగు;

			cur_delta = compute_energy(&eenv, pd, cpus, p,
						   max_అదనపు_cap_cpu);
			/* CPU utilization has changed */
			ఒకవేళ (cur_delta < పునాది_energy)
				తిరిగి లక్ష్యం;
			cur_delta -= పునాది_energy;

			/*
			 * Both fit for the task but best energy cpu has lower
			 * energy impact.
			 */
			ఒకవేళ ((max_సరిపోతుంది > 0) && (ఉత్తమ_సరిపోతుంది > 0) &&
			    (cur_delta >= ఉత్తమ_delta))
				కొనసాగు;

			ఉత్తమ_delta = cur_delta;
			ఉత్తమ_energy_cpu = max_అదనపు_cap_cpu;
			ఉత్తమ_సరిపోతుంది = max_సరిపోతుంది;
			ఉత్తమ_actual_cap = cpu_actual_cap;
		}
	}

	ఒకవేళ ((ఉత్తమ_సరిపోతుంది > prev_సరిపోతుంది) ||
	    ((ఉత్తమ_సరిపోతుంది > 0) && (ఉత్తమ_delta < prev_delta)) ||
	    ((ఉత్తమ_సరిపోతుంది < 0) && (ఉత్తమ_actual_cap > prev_actual_cap)))
		లక్ష్యం = ఉత్తమ_energy_cpu;

	తిరిగి లక్ష్యం;
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
స్థిర పూర్ణాంకం
select_task_rq_fair(నిర్మాణం task_struct *p, పూర్ణాంకం prev_cpu, పూర్ణాంకం wake_flags)
{
	పూర్ణాంకం sync = (wake_flags & WF_SYNC) && !(current->flags & PF_EXITING);
	నిర్మాణం sched_domain *tmp, *sd = ఖాళీ;
	పూర్ణాంకం cpu = smp_processor_id();
	పూర్ణాంకం new_cpu = prev_cpu;
	పూర్ణాంకం want_affine = 0;
	/* SD_flags and WF_flags share the first nibble */
	పూర్ణాంకం sd_flag = wake_flags & 0xF;

	/*
	 * required for stable ->cpus_allowed
	 */
	lockdep_assert_held(&p->pi_తాళం);
	ఒకవేళ (wake_flags & WF_TTWU) {
		రికార్డు_wakee(p);

		ఒకవేళ ((wake_flags & WF_CURRENT_CPU) &&
		    cpumask_test_cpu(cpu, p->cpus_ptr))
			తిరిగి cpu;

		ఒకవేళ (!is_rd_overutilized(this_rq()->rd)) {
			new_cpu = కనుగొను_energy_efficient_cpu(p, prev_cpu);
			ఒకవేళ (new_cpu >= 0)
				తిరిగి new_cpu;
			new_cpu = prev_cpu;
		}

		want_affine = !wake_wide(p) && cpumask_test_cpu(cpu, p->cpus_ptr);
	}

	for_each_domain(cpu, tmp) {
		/*
		 * If both 'cpu' and 'prev_cpu' are part of this domain,
		 * cpu is a valid SD_WAKE_AFFINE target.
		 */
		ఒకవేళ (want_affine && (tmp->flags & SD_WAKE_AFFINE) &&
		    cpumask_test_cpu(prev_cpu, sched_domain_పరిధి(tmp))) {
			ఒకవేళ (cpu != prev_cpu)
				new_cpu = wake_affine(tmp, p, cpu, prev_cpu, sync);

			sd = ఖాళీ; /* Prefer wake_affine over balance flags */
			ఆపు;
		}

		/*
		 * Usually only true for WF_EXEC and WF_FORK, as sched_domains
		 * usually do not have SD_BALANCE_WAKE set. That means wakeup
		 * will usually go to the fast path.
		 */
		ఒకవేళ (tmp->flags & sd_flag)
			sd = tmp;
		లేకపోతే ఒకవేళ (!want_affine)
			ఆపు;
	}

	/* Slow path */
	ఒకవేళ (unlikely(sd)) {
		new_cpu = sched_balance_కనుగొను_dst_cpu(sd, p, cpu, prev_cpu, sd_flag);
		తిరిగి select_idle_smt_cpu(p, new_cpu);
	}

	/* Fast path */
	ఒకవేళ (wake_flags & WF_TTWU)
		తిరిగి select_idle_sibling(p, prev_cpu, new_cpu);

	తిరిగి new_cpu;
}

/*
 * Called immediately before a task is migrated to a new CPU; task_cpu(p) and
 * cfs_rq_of(p) references at time of call are still valid and identify the
 * previous CPU. The caller guarantees p->pi_lock or task_rq(p)->lock is held.
 */
స్థిర శూన్య migrate_task_rq_fair(నిర్మాణం task_struct *p, పూర్ణాంకం new_cpu)
{
	నిర్మాణం sched_entity *se = &p->se;

	ఒకవేళ (!task_on_rq_migrating(p)) {
		remove_entity_ఎక్కించు_avg(se);

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
	se->avg.చివరి_update_సమయం = 0;

	update_పరిశీలన_period(p, new_cpu);
}

స్థిర శూన్య task_dead_fair(నిర్మాణం task_struct *p)
{
	నిర్మాణం sched_entity *se = &p->se;
	remove_entity_ఎక్కించు_avg(se);
}

/*
 * Set the max capacity the task is allowed to run at for misfit detection.
 */
స్థిర శూన్య సమితి_task_max_allowed_సామర్థ్యం(నిర్మాణం task_struct *p)
{
	నిర్మాణం asym_cap_దత్తాంశం *నమోదు_2;

	ఒకవేళ (!sched_asym_cpucap_చురుకైన())
		తిరిగి;

	rcu_read_తాళం();
	list_for_each_నమోదు_rcu(నమోదు_2, &asym_cap_list, link) {
		cpumask_t *cpumask;

		cpumask = cpu_సామర్థ్యం_పరిధి(నమోదు_2);
		ఒకవేళ (!cpumask_intersects(p->cpus_ptr, cpumask))
			కొనసాగు;

		p->max_allowed_సామర్థ్యం = నమోదు_2->సామర్థ్యం;
		ఆపు;
	}
	rcu_read_unlock();
}

స్థిర శూన్య సమితి_cpus_allowed_fair(నిర్మాణం task_struct *p, నిర్మాణం affinity_context *ctx)
{
	సమితి_cpus_allowed_common(p, ctx);
	సమితి_task_max_allowed_సామర్థ్యం(p);
}

గణన preempt_wakeup_action {
	PREEMPT_WAKEUP_NONE,	/* No preemption. */
	PREEMPT_WAKEUP_SHORT,	/* Ignore slice protection. */
	PREEMPT_WAKEUP_PICK,	/* Let pick_eevdf() decide. */
	PREEMPT_WAKEUP_RESCHED,	/* Force reschedule. */
};

స్థిర అంతఃవరుస bool సమితి_preempt_buddy(నిర్మాణం cfs_rq *cfs_rq, నిర్మాణం sched_entity *pse)
{
	/*
	 * Keep existing buddy if the deadline is sooner than pse.
	 * The older buddy may be cache cold and completely unrelated
	 * to the current wakeup but that is unpredictable where as
	 * obeying the deadline is more in line with EEVDF objectives.
	 */
	ఒకవేళ (cfs_rq->తదుపరి && entity_before(cfs_rq->తదుపరి, pse))
		తిరిగి false;

	సమితి_తదుపరి_buddy(cfs_rq, pse);
	తిరిగి true;
}

స్థిర అంతఃవరుస bool సమితి_short_buddy(నిర్మాణం cfs_rq *cfs_rq, నిర్మాణం sched_entity *pse)
{
	ఒకవేళ (cfs_rq->తదుపరి) {
		ఒకవేళ (cfs_rq->తదుపరి->ముక్క < pse->ముక్క)
			తిరిగి false;

		ఒకవేళ (cfs_rq->తదుపరి->ముక్క == pse->ముక్క &&
		    entity_before(cfs_rq->తదుపరి, pse))
			తిరిగి false;
	}

	సమితి_తదుపరి_buddy(cfs_rq, pse);
	తిరిగి true;
}

/*
 * WF_SYNC|WF_TTWU indicates the waker expects to sleep but it is not
 * strictly enforced because the hint is either misunderstood or
 * multiple tasks must be woken up.
 */
స్థిర అంతఃవరుస గణన preempt_wakeup_action
preempt_sync(నిర్మాణం rq *rq, పూర్ణాంకం wake_flags,
	     నిర్మాణం sched_entity *pse, నిర్మాణం sched_entity *se)
{
	u64 పరిమితి, delta;

	/*
	 * WF_SYNC without WF_TTWU is not expected so warn if it happens even
	 * though it is likely harmless.
	 */
	WARN_ON_ONCE(!(wake_flags & WF_TTWU));

	పరిమితి = sysctl_sched_migration_ఖర్చు;
	delta = rq_clock_task(rq) - se->exec_ప్రారంభం;
	ఒకవేళ ((s64)delta < 0)
		delta = 0;

	/*
	 * WF_RQ_SELECTED implies the tasks are stacking on a CPU when they
	 * could run on other CPUs. Reduce the threshold before preemption is
	 * allowed to an arbitrary lower value as it is more likely (but not
	 * guaranteed) the waker requires the wakee to finish.
	 */
	ఒకవేళ (wake_flags & WF_RQ_SELECTED)
		పరిమితి >>= 2;

	/*
	 * As WF_SYNC is not strictly obeyed, allow some runtime for batch
	 * wakeups to be issued.
	 */
	ఒకవేళ (entity_before(pse, se) && delta >= పరిమితి)
		తిరిగి PREEMPT_WAKEUP_RESCHED;

	తిరిగి PREEMPT_WAKEUP_NONE;
}

/*
 * Preempt the current task with a newly woken task if needed:
 */
స్థిర శూన్య wakeup_preempt_fair(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, పూర్ణాంకం wake_flags)
{
	గణన preempt_wakeup_action preempt_action = PREEMPT_WAKEUP_PICK;
	నిర్మాణం task_struct *donor = rq->donor;
	నిర్మాణం sched_entity *nse, *se = &donor->se, *pse = &p->se;
	నిర్మాణం cfs_rq *cfs_rq = &rq->cfs;
	పూర్ణాంకం cse_is_idle, pse_is_idle;

	/*
	 * XXX Getting preempted by higher class, try and find idle CPU?
	 */
	ఒకవేళ (p->sched_class != &fair_sched_class ||
	    donor->sched_class != &fair_sched_class)
		తిరిగి;

	ఒకవేళ (unlikely(se == pse))
		తిరిగి;

	/*
	 * This is possible from callers such as attach_tasks(), in which we
	 * unconditionally wakeup_preempt() after an enqueue (which may have
	 * lead to a throttle).  This both saves work and prevents false
	 * next-buddy nomination below.
	 */
	ఒకవేళ (task_is_throttled(p))
		తిరిగి;

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
	ఒకవేళ (!sched_feat(PREEMPT_SHORT) && test_tsk_need_resched(rq->curr))
		తిరిగి;

	ఒకవేళ (!sched_feat(WAKEUP_PREEMPTION))
		తిరిగి;

	WARN_ON_ONCE(!pse);

	cse_is_idle = se_is_idle(se);
	pse_is_idle = se_is_idle(pse);

	nse = se;
	/*
	 * Preempt an idle entity in favor of a non-idle entity (and don't preempt
	 * in the inverse case).
	 */
	ఒకవేళ (cse_is_idle && !pse_is_idle)
		వెళ్ళు preempt;

	update_curr_fair(rq);

	ఒకవేళ (cse_is_idle != pse_is_idle)
		వెళ్ళు update;

	/*
	 * BATCH and IDLE tasks do not preempt others.
	 */
	ఒకవేళ (unlikely(!సాధారణ_policy(p->policy)))
		వెళ్ళు update;

	/*
	 * Do not preempt for tasks that are sched_delayed as it would violate
	 * EEVDF to forcibly queue an ineligible task.
	 */
	ఒకవేళ (pse->sched_delayed)
		వెళ్ళు update;

	/*
	 * If @p has a shorter slice than current and @p is eligible, override
	 * current's slice protection in order to allow preemption.
	 */
	ఒకవేళ (sched_feat(PREEMPT_SHORT) && (pse->ముక్క < se->ముక్క)) {
		preempt_action = PREEMPT_WAKEUP_SHORT;
		వెళ్ళు pick;
	}

	/*
	 * Ignore wakee preemption on WF_FORK as it is less likely that
	 * there is shared data as exec often follow fork.
	 */
	ఒకవేళ (wake_flags & WF_FORK)
		వెళ్ళు update;

	/* Prefer picking wakee soon if appropriate. */
	ఒకవేళ (sched_feat(తదుపరి_BUDDY) && సమితి_preempt_buddy(cfs_rq, pse)) {
		/*
		 * Decide whether to obey WF_SYNC hint for a new buddy. Old
		 * buddies are ignored as they may not be relevant to the
		 * waker and less likely to be cache hot.
		 */
		ఒకవేళ (wake_flags & WF_SYNC)
			preempt_action = preempt_sync(rq, wake_flags, pse, se);
	}

	ఎంపిక (preempt_action) {
	స్థితి PREEMPT_WAKEUP_NONE:
		తిరిగి;
	స్థితి PREEMPT_WAKEUP_RESCHED:
		వెళ్ళు preempt;
	స్థితి PREEMPT_WAKEUP_SHORT:
		fallthrough;
	స్థితి PREEMPT_WAKEUP_PICK:
		ఆపు;
	}

pick:
	ఒకవేళ (cfs_rq->h_nr_queued) {
		nse = pick_తదుపరి_entity(rq, preempt_action != PREEMPT_WAKEUP_SHORT);
		ఒకవేళ (unlikely(!nse))
			వెళ్ళు pick;

		/* If @p has become the most eligible task, force preemption */
		ఒకవేళ (nse == pse)
			వెళ్ళు preempt;
	}

	/*
	 * If @p is eligible but not the next task to run then cancel protection
	 * to prevent large scheduling latency
	 */
	ఒకవేళ (preempt_action == PREEMPT_WAKEUP_SHORT && entity_eligible(cfs_rq, pse))
		వెళ్ళు preempt;
update:
	ఒకవేళ (sched_feat(నిరంతరం_గమ్యం_PARITY))
		update_protect_ముక్క(cfs_rq, se);

	తిరిగి;

preempt:
	cancel_protect_ముక్క(se);

	ఒకవేళ (preempt_action == PREEMPT_WAKEUP_SHORT)
		సమితి_short_buddy(cfs_rq, pse);

	resched_curr_lazy(rq);
}

నిర్మాణం task_struct *pick_task_fair(నిర్మాణం rq *rq, నిర్మాణం rq_flags *rf)
	__must_hold(__rq_lockp(rq))
{
	నిర్మాణం cfs_rq *cfs_rq = &rq->cfs;
	నిర్మాణం sched_entity *se;
	నిర్మాణం task_struct *p;
	పూర్ణాంకం new_tasks;

again:
	ఒకవేళ (!cfs_rq->h_nr_queued)
		వెళ్ళు idle;

	/* Might not have done put_prev_entity() */
	ఒకవేళ (cfs_rq->curr && cfs_rq->curr->on_rq)
		update_curr_eevdf(cfs_rq);

	se = pick_తదుపరి_entity(rq, true);
	ఒకవేళ (!se)
		వెళ్ళు again;

	p = task_యొక్క(se);
	తిరిగి p;

idle:
	ఒకవేళ (sched_core_enabled(rq))
		తిరిగి ఖాళీ;

	new_tasks = sched_balance_newidle(rq, rf);
	ఒకవేళ (new_tasks < 0)
		తిరిగి RETRY_TASK;
	ఒకవేళ (new_tasks > 0)
		వెళ్ళు again;
	తిరిగి ఖాళీ;
}

స్థిర నిర్మాణం task_struct *
fair_server_pick_task(నిర్మాణం sched_dl_entity *dl_se, నిర్మాణం rq_flags *rf)
	__must_hold(__rq_lockp(dl_se->rq))
{
	తిరిగి pick_task_fair(dl_se->rq, rf);
}

శూన్య fair_server_init(నిర్మాణం rq *rq)
{
	నిర్మాణం sched_dl_entity *dl_se = &rq->fair_server;

	init_dl_entity(dl_se);

	dl_server_init(dl_se, rq, fair_server_pick_task);
}

/*
 * Account for a descheduled task:
 */
స్థిర శూన్య put_prev_task_fair(నిర్మాణం rq *rq, నిర్మాణం task_struct *prev, నిర్మాణం task_struct *తదుపరి)
{
	నిర్మాణం sched_entity *se = &prev->se;
	నిర్మాణం cfs_rq *cfs_rq = &rq->cfs;
	నిర్మాణం sched_entity *nse = ఖాళీ;

#నిర్వచితమైతే CONFIG_FAIR_సమూహం_SCHED
	ఒకవేళ (తదుపరి && తదుపరి->sched_class == &fair_sched_class)
		nse = &తదుపరి->se;
#అయితే_ముగింపు

	అయితే (se) {
		cfs_rq = cfs_rq_యొక్క(se);
		ఒకవేళ (!nse || cfs_rq->h_curr)
			put_prev_entity(cfs_rq, se);
#నిర్వచితమైతే CONFIG_FAIR_సమూహం_SCHED
		ఒకవేళ (nse) {
			ఒకవేళ (is_same_సమూహం(se, nse))
				ఆపు;

			పూర్ణాంకం d = nse->లోతు - se->లోతు;
			ఒకవేళ (d >= 0) {
				/* nse has equal or greater depth, ascend */
				nse = మాతృ_entity(nse);
				/* if nse is the deeper, do not ascend se */
				ఒకవేళ (d > 0)
					కొనసాగు;
			}
		}
#అయితే_ముగింపు
		se = మాతృ_entity(se);
	}

	/* Put 'current' back into the tree. */
	cfs_rq = &rq->cfs;
	se = &prev->se;
	WARN_ON_ONCE(cfs_rq->curr != se);
	cfs_rq->curr = ఖాళీ;
	ఒకవేళ (se->on_rq)
		__enqueue_entity(cfs_rq, se);
}

/*
 * sched_yield() is very simple
 */
స్థిర శూన్య yield_task_fair(నిర్మాణం rq *rq)
{
	నిర్మాణం task_struct *curr = rq->donor;
	నిర్మాణం sched_entity *se = &curr->se;
	నిర్మాణం cfs_rq *cfs_rq = &rq->cfs;

	/*
	 * Are we the only task in the tree?
	 */
	ఒకవేళ (unlikely(rq->nr_running == 1))
		తిరిగి;

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
	ఒకవేళ (entity_eligible(cfs_rq, se)) {
		se->vruntime = se->deadline;
		update_deadline(cfs_rq, se);
	}
}

స్థిర bool yield_గమ్యం_task_fair(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
	నిర్మాణం sched_entity *se = &p->se;

	/* !se->on_rq also covers throttled task */
	ఒకవేళ (!se->on_rq || se->sched_delayed)
		తిరిగి false;

	/* Tell the scheduler that we'd really like se to run next. */
	సమితి_తదుపరి_buddy(&task_rq(p)->cfs, se);

	yield_task_fair(rq);

	తిరిగి true;
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

స్థిర గుర్తులేని దీర్ఘ __read_mostly max_ఎక్కించు_balance_interval = HZ/10;

గణన fbq_type { regular, remote, all };

/*
 * 'group_type' describes the group of CPUs at the moment of load balancing.
 *
 * The enum is ordered by pulling priority, with the group with lowest priority
 * first so the group_type can simply be compared when selecting the busiest
 * group. See update_sd_pick_busiest().
 */
గణన సమూహం_type {
	/* The group has spare capacity that can be used to run more tasks.  */
	సమూహం_has_అదనపు = 0,
	/*
	 * The group is fully used and the tasks don't compete for more CPU
	 * cycles. Nevertheless, some tasks might wait before running.
	 */
	సమూహం_fully_busy,
	/*
	 * One task doesn't fit with CPU's capacity and must be migrated to a
	 * more powerful CPU.
	 */
	సమూహం_misfit_task,
	/*
	 * Balance SMT group that's fully busy. Can benefit from migration
	 * a task on SMT with busy sibling to another CPU on idle core.
	 */
	సమూహం_smt_balance,
	/*
	 * SD_ASYM_PACKING only: One local CPU with higher capacity is available,
	 * and the task should be migrated to it instead of running on the
	 * current CPU.
	 */
	సమూహం_asym_packing,
	/*
	 * The tasks' affinity constraints previously prevented the scheduler
	 * from balancing the load across the system.
	 */
	సమూహం_imbalanced,
	/*
	 * There are tasks running on non-preferred LLC, possible to move
	 * them to their preferred LLC without creating too much imbalance.
	 * The priority of group_llc_balance is lower than that of
	 * group_overloaded and higher than that of all other group types.
	 * This is because group_llc_balance may exacerbate load imbalance.
	 * If the LLC balancing attempt fails, the nr_balance_failed
	 * mechanism will trigger other group types to rebalance the load.
	 */
	సమూహం_llc_balance,
	/*
	 * The CPU is overloaded and can't provide expected CPU cycles to all
	 * tasks.
	 */
	సమూహం_overloaded
};

గణన migration_type {
	migrate_ఎక్కించు = 0,
	migrate_util,
	migrate_task,
	migrate_misfit,
	migrate_llc_task
};

#నిర్వచించు LBF_ALL_PINNED	0x01
#నిర్వచించు LBF_NEED_BREAK	0x02
#నిర్వచించు LBF_DST_PINNED  0x04
#నిర్వచించు LBF_SOME_PINNED	0x08
#నిర్వచించు LBF_చురుకైన_LB	0x10
#నిర్వచించు LBF_LLC_PINNED	0x20
#నిర్వచించు LBF_చురుకైన_LB_LLC	0x40

నిర్మాణం lb_env {
	నిర్మాణం sched_domain	*sd;

	నిర్మాణం rq		*src_rq;
	పూర్ణాంకం			src_cpu;

	పూర్ణాంకం			dst_cpu;
	నిర్మాణం rq		*dst_rq;
	bool			dst_core_idle;

	నిర్మాణం cpumask		*dst_grpmask;
	పూర్ణాంకం			new_dst_cpu;
	గణన cpu_idle_type	idle;
	దీర్ఘ			imbalance;
	/* The set of CPUs under consideration for load-balancing */
	నిర్మాణం cpumask		*cpus;

	గుర్తులేని పూర్ణాంకం		flags;

	గుర్తులేని పూర్ణాంకం		loop;
	గుర్తులేని పూర్ణాంకం		loop_break;
	గుర్తులేని పూర్ణాంకం		loop_max;

	గణన fbq_type		fbq_type;
	గణన migration_type	migration_type;
	నిర్మాణం list_తల	tasks;
};

/*
 * Is this task likely cache-hot:
 */
స్థిర పూర్ణాంకం task_hot(నిర్మాణం task_struct *p, నిర్మాణం lb_env *env)
{
	s64 delta;

	lockdep_assert_rq_held(env->src_rq);

	ఒకవేళ (p->sched_class != &fair_sched_class)
		తిరిగి 0;

	ఒకవేళ (unlikely(task_has_idle_policy(p)))
		తిరిగి 0;

	/* SMT siblings share cache */
	ఒకవేళ (env->sd->flags & SD_SHARE_CPUCAPACITY)
		తిరిగి 0;

	/*
	 * Buddy candidates are cache hot:
	 */
	ఒకవేళ (sched_feat(CACHE_HOT_BUDDY) && env->dst_rq->nr_running &&
	    (&p->se == cfs_rq_యొక్క(&p->se)->తదుపరి))
		తిరిగి 1;

	ఒకవేళ (sysctl_sched_migration_ఖర్చు == -1)
		తిరిగి 1;

	/*
	 * Don't migrate task if the task's cookie does not match
	 * with the destination CPU's core cookie.
	 */
	ఒకవేళ (!sched_core_cookie_సరిపోలిక(cpu_rq(env->dst_cpu), p))
		తిరిగి 1;

	ఒకవేళ (sysctl_sched_migration_ఖర్చు == 0)
		తిరిగి 0;

	delta = rq_clock_task(env->src_rq) - p->se.exec_ప్రారంభం;

	తిరిగి delta < (s64)sysctl_sched_migration_ఖర్చు;
}

#నిర్వచితమైతే CONFIG_NUMA_BALANCING
/*
 * Returns a positive value, if task migration degrades locality.
 * Returns 0, if task migration is not affected by locality.
 * Returns a negative value, if task migration improves locality i.e migration preferred.
 */
స్థిర దీర్ఘ migrate_degrades_locality(నిర్మాణం task_struct *p, నిర్మాణం lb_env *env)
{
	నిర్మాణం numa_సమూహం *numa_సమూహం = rcu_dereference_all(p->numa_సమూహం);
	గుర్తులేని దీర్ఘ src_బరువు, dst_బరువు;
	పూర్ణాంకం src_nid, dst_nid, dist;

	ఒకవేళ (!static_branch_likely(&sched_numa_balancing))
		తిరిగి 0;

	ఒకవేళ (!p->numa_దోషాలు || !(env->sd->flags & SD_NUMA))
		తిరిగి 0;

	src_nid = cpu_గమ్యం_కణుపు(env->src_cpu);
	dst_nid = cpu_గమ్యం_కణుపు(env->dst_cpu);

	ఒకవేళ (src_nid == dst_nid)
		తిరిగి 0;

	/* Migrating away from the preferred node is always bad. */
	ఒకవేళ (src_nid == p->numa_preferred_nid) {
		ఒకవేళ (env->src_rq->nr_running > env->src_rq->nr_preferred_running)
			తిరిగి 1;
		లేకపోతే
			తిరిగి 0;
	}

	/* Encourage migration to the preferred node. */
	ఒకవేళ (dst_nid == p->numa_preferred_nid)
		తిరిగి -1;

	/* Leaving a core idle is often worse than degrading locality. */
	ఒకవేళ (env->idle == CPU_IDLE)
		తిరిగి 0;

	dist = కణుపు_దూరం(src_nid, dst_nid);
	ఒకవేళ (numa_సమూహం) {
		src_బరువు = సమూహం_బరువు(p, src_nid, dist);
		dst_బరువు = సమూహం_బరువు(p, dst_nid, dist);
	} లేకపోతే {
		src_బరువు = task_బరువు(p, src_nid, dist);
		dst_బరువు = task_బరువు(p, dst_nid, dist);
	}

	తిరిగి src_బరువు - dst_బరువు;
}

#లేకపోతే /* !CONFIG_NUMA_BALANCING: */
స్థిర అంతఃవరుస దీర్ఘ migrate_degrades_locality(నిర్మాణం task_struct *p,
					     నిర్మాణం lb_env *env)
{
	తిరిగి 0;
}
#అయితే_ముగింపు /* !CONFIG_NUMA_BALANCING */

/*
 * Check whether the task is ineligible on the destination cpu
 *
 * When the PLACE_LAG scheduling feature is enabled and
 * dst_cfs_rq->nr_queued is greater than 1, if the task
 * is ineligible, it will also be ineligible when
 * it is migrated to the destination cpu.
 */
స్థిర అంతఃవరుస పూర్ణాంకం task_is_ineligible_on_dst_cpu(నిర్మాణం task_struct *p, పూర్ణాంకం dest_cpu)
{
	నిర్మాణం cfs_rq *dst_cfs_rq = &cpu_rq(dest_cpu)->cfs;

	ఒకవేళ (sched_feat(ఉంచు_LAG) && dst_cfs_rq->h_nr_queued &&
	    !entity_eligible(&task_rq(p)->cfs, &p->se))
		తిరిగి 1;

	తిరిగి 0;
}

#నిర్వచితమైతే CONFIG_SCHED_CACHE
/*
 * The margin used when comparing LLC utilization with CPU capacity.
 * It determines the LLC load level where active LLC aggregation is
 * done.
 * Derived from fits_capacity().
 *
 * (default: ~50%, tunable via debugfs)
 */
స్థిర bool సరిపోతుంది_llc_సామర్థ్యం(గుర్తులేని దీర్ఘ util, గుర్తులేని దీర్ఘ max)
{
	u32 aggr_pct = llc_overaggr_pct;

	/*
	 * For single core systems, raise the aggregation
	 * threshold to accommodate more tasks.
	 */
	ఒకవేళ (cpu_smt_అంకె_దారాలు == 1)
		aggr_pct = (aggr_pct * 3 / 2);

	తిరిగి util * 100 < max * aggr_pct;
}

/*
 * The margin used when comparing utilization.
 * is 'util1' noticeably greater than 'util2'
 * Derived from capacity_greater().
 * Bias is in perentage.
 */
/* Allows dst util to be bigger than src util by up to bias percent */
#నిర్వచించు util_greater(util1, util2) \
	((util1) * 100 > (util2) * (100 + llc_imb_pct))

స్థిర __maybe_unused bool get_llc_stats(పూర్ణాంకం cpu, గుర్తులేని దీర్ఘ *util,
					 గుర్తులేని దీర్ఘ *cap)
{
	నిర్మాణం sched_domain_shared *sd_share;

	sd_share = rcu_dereference_all(చొప్పున_cpu(sd_llc_shared, cpu));
	ఒకవేళ (!sd_share)
		తిరిగి false;

	*util = READ_ONCE(sd_share->util_avg);
	*cap = READ_ONCE(sd_share->సామర్థ్యం);

	తిరిగి true;
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
గణన llc_mig {
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
స్థిర గణన llc_mig can_migrate_llc(పూర్ణాంకం src_cpu, పూర్ణాంకం dst_cpu,
				    గుర్తులేని దీర్ఘ tsk_util,
				    bool గమ్యం_pref)
{
	గుర్తులేని దీర్ఘ src_util, dst_util, src_cap, dst_cap;

	ఒకవేళ (!get_llc_stats(src_cpu, &src_util, &src_cap) ||
	    !get_llc_stats(dst_cpu, &dst_util, &dst_cap))
		తిరిగి mig_unrestricted;

	src_util = src_util < tsk_util ? 0 : src_util - tsk_util;
	dst_util = dst_util + tsk_util;

	ఒకవేళ (!సరిపోతుంది_llc_సామర్థ్యం(dst_util, dst_cap) &&
	    !సరిపోతుంది_llc_సామర్థ్యం(src_util, src_cap))
		తిరిగి mig_unrestricted;

	ఒకవేళ (గమ్యం_pref) {
		/*
		 * Don't migrate if we will get preferred LLC too
		 * heavily loaded and if the dest is much busier
		 * than the src, in which case migration will
		 * increase the imbalance too much.
		 */
		ఒకవేళ (!సరిపోతుంది_llc_సామర్థ్యం(dst_util, dst_cap) &&
		    util_greater(dst_util, src_util))
			తిరిగి mig_forbid;
	} లేకపోతే {
		/*
		 * Don't migrate if we will leave preferred LLC
		 * too idle, or if this migration leads to the
		 * non-preferred LLC falls within sysctl_aggr_imb percent
		 * of preferred LLC, leading to migration again
		 * back to preferred LLC.
		 */
		ఒకవేళ (సరిపోతుంది_llc_సామర్థ్యం(src_util, src_cap) ||
		    !util_greater(src_util, dst_util))
			తిరిగి mig_forbid;
	}
	తిరిగి mig_llc;
}

స్థిర అంతఃవరుస bool task_misfits_asym_cpu(నిర్మాణం lb_env *env, నిర్మాణం task_struct *p)
{
	/*
	 * On asymmetric CPU capacity domains, do not let cache-aware
	 * balancing pull the task onto a destination CPU that cannot
	 * accommodate it. Doing so would turn the task into a misfit on
	 * the destination, trading a cache-locality gain for a capacity
	 * loss. If the task already does not fit its source CPU, the move
	 * cannot make things worse, so let the LLC preference decide.
	 */
	ఒకవేళ ((env->sd->flags & SD_ASYM_CPUCAPACITY) && p &&
	    !task_సరిపోతుంది_cpu(p, env->dst_cpu) &&
	    task_సరిపోతుంది_cpu(p, env->src_cpu))
		తిరిగి true;

	తిరిగి false;
}

/*
 * Check if task p can migrate from source LLC to
 * destination LLC in terms of cache aware load balance.
 */
స్థిర గణన llc_mig can_migrate_llc_task(నిర్మాణం lb_env *env,
					 నిర్మాణం task_struct *p)
{
	నిర్మాణం sched_cache_సమూహం *grp;
	bool గమ్యం_pref;
	పూర్ణాంకం cpu, src_cpu, dst_cpu;

	ఒకవేళ (task_misfits_asym_cpu(env, p))
		తిరిగి mig_forbid;

	src_cpu = env->src_cpu;
	dst_cpu = env->dst_cpu;
	grp = rcu_dereference_all(p->sched_cache_grp);
	ఒకవేళ (!grp)
		తిరిగి mig_unrestricted;

	cpu = READ_ONCE(grp->cpu);
	ఒకవేళ (cpu < 0 || cpus_share_cache(src_cpu, dst_cpu))
		తిరిగి mig_unrestricted;

	/* skip cache aware load balance for too many threads */
	ఒకవేళ (invalid_llc_nr(grp, p, dst_cpu) ||
	    exceed_llc_సామర్థ్యం(grp, dst_cpu)) {
		ఒకవేళ (READ_ONCE(grp->cpu) != -1)
			WRITE_ONCE(grp->cpu, -1);
		తిరిగి mig_unrestricted;
	}

	ఒకవేళ (cpus_share_cache(dst_cpu, cpu))
		గమ్యం_pref = true;
	లేకపోతే ఒకవేళ (cpus_share_cache(src_cpu, cpu))
		గమ్యం_pref = false;
	లేకపోతే
		తిరిగి mig_unrestricted;

	తిరిగి can_migrate_llc(src_cpu, dst_cpu,
			       task_util(p), గమ్యం_pref);
}

/*
 * Check if active load balance breaks LLC locality in
 * terms of cache aware load balance. The load level and
 * imbalance do not warrant breaking LLC preference per
 * the can_migrate_llc() policy. Here, the benefit of
 * LLC locality outweighs the power efficiency gained from
 * migrating the only runnable task away.
 */
స్థిర అంతఃవరుస bool
alb_break_llc(నిర్మాణం lb_env *env)
{
	ఒకవేళ (!sched_cache_enabled())
		తిరిగి false;

	ఒకవేళ (cpus_share_cache(env->src_cpu, env->dst_cpu))
		తిరిగి false;
	/*
	 * All tasks prefer to stay on their current CPU.
	 * Do not pull a task from its preferred CPU if:
	 * 1. It is the only task running and does not exceed
	 *    imbalance allowance; OR
	 * 2. Migrating it away from its preferred LLC would violate
	 *    the cache-aware scheduling policy.
	 */
	ఒకవేళ (env->src_rq->nr_pref_llc_running &&
	    env->src_rq->nr_pref_llc_running == env->src_rq->cfs.h_nr_runnable) {
		గుర్తులేని దీర్ఘ util = 0;
		నిర్మాణం task_struct *cur;

		/*
		 * Migrating misfit tasks from current CPU
		 * to CPU with a better fit.
		 * Prioritize that over LLC preference.
		 */
		ఒకవేళ (env->migration_type == migrate_misfit)
			తిరిగి false;

		ఒకవేళ (env->src_rq->nr_running <= 1)
			తిరిగి true;

		cur = rcu_dereference_all(env->src_rq->curr);
		ఒకవేళ (cur && cur->sched_class == &fair_sched_class)
			util = task_util(cur);

		ఒకవేళ (task_misfits_asym_cpu(env, cur) ||
		    can_migrate_llc(env->src_cpu, env->dst_cpu,
				    util, false) == mig_forbid)
			తిరిగి true;
	}

	తిరిగి false;
}

/*
 * Returns true if p's preferred LLC does not match the destination CPU
 * under migrate_llc_task semantics. Passive LB passes migrate_llc_task
 * in env->migration_type, while active LB carries LBF_ACTIVE_LB_LLC in
 * env->flags to avoid overwriting env->migration_type.
 */
స్థిర అంతఃవరుస bool
migrate_llc_task_wrong_dst(నిర్మాణం task_struct *p, నిర్మాణం lb_env *env)
{
	తిరిగి sched_cache_enabled() &&
	       (env->migration_type == migrate_llc_task ||
		env->flags & LBF_చురుకైన_LB_LLC) &&
	       READ_ONCE(p->preferred_llc) != llc_id(env->dst_cpu);
}

/*
 * Check if migrating task p from env->src_cpu to
 * env->dst_cpu breaks LLC localiy.
 */
స్థిర bool migrate_degrades_llc(నిర్మాణం task_struct *p, నిర్మాణం lb_env *env)
{
	ఒకవేళ (!sched_cache_enabled())
		తిరిగి false;

	ఒకవేళ (task_has_sched_core(p))
		తిరిగి false;
	/*
	 * Skip over tasks that would degrade LLC locality;
	 * only when nr_balanced_failed is sufficiently high do we
	 * ignore this constraint.
	 *
	 * Threshold of cache_nice_tries is set to 1 higher
	 * than nr_balance_failed to avoid excessive task
	 * migration at the same time.
	 */
	ఒకవేళ (env->sd->nr_balance_failed >= env->sd->cache_nice_tries + 1)
		తిరిగి false;

	/*
	 * We know the env->src_cpu has some tasks prefer to
	 * run on env->dst_cpu, skip the tasks do not prefer
	 * env->dst_cpu, and find the one that prefers.
	 */
	ఒకవేళ (migrate_llc_task_wrong_dst(p, env))
		తిరిగి true;

	ఒకవేళ (can_migrate_llc_task(env, p) != mig_forbid)
		తిరిగి false;

	తిరిగి true;
}

#లేకపోతే
స్థిర అంతఃవరుస bool get_llc_stats(పూర్ణాంకం cpu, గుర్తులేని దీర్ఘ *util,
				 గుర్తులేని దీర్ఘ *cap)
{
	తిరిగి false;
}

స్థిర అంతఃవరుస bool
alb_break_llc(నిర్మాణం lb_env *env)
{
	తిరిగి false;
}

స్థిర అంతఃవరుస bool
migrate_llc_task_wrong_dst(నిర్మాణం task_struct *p, నిర్మాణం lb_env *env)
{
	తిరిగి false;
}

స్థిర అంతఃవరుస bool
migrate_degrades_llc(నిర్మాణం task_struct *p, నిర్మాణం lb_env *env)
{
	తిరిగి false;
}
#అయితే_ముగింపు
/*
 * can_migrate_task - may task p from runqueue rq be migrated to this_cpu?
 */
స్థిర
పూర్ణాంకం can_migrate_task(నిర్మాణం task_struct *p, నిర్మాణం lb_env *env)
{
	దీర్ఘ degrades, hot;

	lockdep_assert_rq_held(env->src_rq);
	ఒకవేళ (p->sched_task_hot)
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
	ఒకవేళ ((p->se.sched_delayed) && (env->migration_type != migrate_ఎక్కించు))
		తిరిగి 0;

	ఒకవేళ (lb_throttled_hierarchy(p, env->dst_cpu))
		తిరిగి 0;

	/*
	 * We want to prioritize the migration of eligible tasks.
	 * For ineligible tasks we soft-limit them and only allow
	 * them to migrate when nr_balance_failed is non-zero to
	 * avoid load-balancing trying very hard to balance the load.
	 */
	ఒకవేళ (!env->sd->nr_balance_failed &&
	    task_is_ineligible_on_dst_cpu(p, env->dst_cpu))
		తిరిగి 0;

	/* Disregard percpu kthreads; they are where they need to be. */
	ఒకవేళ (kthread_is_చొప్పున_cpu(p))
		తిరిగి 0;

	ఒకవేళ (task_is_blocked(p))
		తిరిగి 0;

	ఒకవేళ (!cpumask_test_cpu(env->dst_cpu, p->cpus_ptr)) {
		పూర్ణాంకం cpu;

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
		ఒకవేళ (env->idle == CPU_NEWLY_IDLE ||
		    env->flags & (LBF_DST_PINNED | LBF_చురుకైన_LB))
			తిరిగి 0;

		/* Prevent to re-select dst_cpu via env's CPUs: */
		cpu = cpumask_మొదటి_and_and(env->dst_grpmask, env->cpus, p->cpus_ptr);

		ఒకవేళ (cpu < nr_cpu_ids) {
			env->flags |= LBF_DST_PINNED;
			env->new_dst_cpu = cpu;
		}

		తిరిగి 0;
	}

	/* Record that we found at least one task that could run on dst_cpu */
	env->flags &= ~LBF_ALL_PINNED;

	ఒకవేళ (task_on_cpu(env->src_rq, p) ||
	    task_current_donor(env->src_rq, p)) {
		schedstat_inc(p->stats.nr_failed_migrations_running);
		తిరిగి 0;
	}

	/*
	 * Aggressive migration if:
	 * 1) active balance
	 * 2) destination numa is preferred
	 * 3) task is cache cold, or
	 * 4) too many balance attempts have failed.
	 */
	ఒకవేళ (env->flags & LBF_చురుకైన_LB)
		తిరిగి !migrate_llc_task_wrong_dst(p, env);

	degrades = migrate_degrades_locality(p, env);
	ఒకవేళ (!degrades) {
		/*
		 * If the NUMA locality is not broken,
		 * further check if migration would hurt
		 * LLC locality.
		 */
		ఒకవేళ (migrate_degrades_llc(p, env)) {
			/*
			 * If regular load balancing fails to pull a task
			 * due to LLC locality, this is expected behavior
			 * and we set LBF_LLC_PINNED so we don't increase
			 * nr_balance_failed unecessarily.
			 */
			ఒకవేళ (env->migration_type != migrate_llc_task)
				env->flags |= LBF_LLC_PINNED;

			తిరిగి 0;
		}

		hot = task_hot(p, env);
	} లేకపోతే {
		hot = degrades > 0;
	}

	ఒకవేళ (!hot || env->sd->nr_balance_failed > env->sd->cache_nice_tries) {
		ఒకవేళ (hot)
			p->sched_task_hot = 1;
		తిరిగి 1;
	}

	schedstat_inc(p->stats.nr_failed_migrations_hot);
	తిరిగి 0;
}

/*
 * detach_task() -- detach the task for the migration specified in env
 */
స్థిర శూన్య detach_task(నిర్మాణం task_struct *p, నిర్మాణం lb_env *env)
{
	lockdep_assert_rq_held(env->src_rq);

	ఒకవేళ (p->sched_task_hot) {
		p->sched_task_hot = 0;
		schedstat_inc(env->sd->lb_hot_gained[env->idle]);
		schedstat_inc(p->stats.nr_forced_migrations);
	}

	WARN_ON(task_current(env->src_rq, p));
	WARN_ON(task_current_donor(env->src_rq, p));

	deactivate_task(env->src_rq, p, వరుస_నుండి_తీయి_NOCLOCK);
	సమితి_task_cpu(p, env->dst_cpu);
}

/*
 * detach_one_task() -- tries to dequeue exactly one task from env->src_rq, as
 * part of active balancing operations within "domain".
 *
 * Returns a task if successful and NULL otherwise.
 */
స్థిర నిర్మాణం task_struct *detach_one_task(నిర్మాణం lb_env *env)
{
	నిర్మాణం task_struct *p;

	lockdep_assert_rq_held(env->src_rq);

	list_for_each_నమోదు_తిరగేయి(p,
			&env->src_rq->cfs_tasks, se.సమూహం_కణుపు) {
		ఒకవేళ (!can_migrate_task(p, env))
			కొనసాగు;

		detach_task(p, env);

		/*
		 * Right now, this is only the second place where
		 * lb_gained[env->idle] is updated (other is detach_tasks)
		 * so we can safely collect stats here rather than
		 * inside detach_tasks().
		 */
		schedstat_inc(env->sd->lb_gained[env->idle]);
		తిరిగి p;
	}
	తిరిగి ఖాళీ;
}

/*
 * detach_tasks() -- tries to detach up to imbalance load/util/tasks from
 * busiest_rq, as part of a balancing operation within domain "sd".
 *
 * Returns number of detached tasks if successful and 0 otherwise.
 */
స్థిర పూర్ణాంకం detach_tasks(నిర్మాణం lb_env *env)
{
	నిర్మాణం list_తల *tasks = &env->src_rq->cfs_tasks;
	గుర్తులేని దీర్ఘ util, ఎక్కించు;
	నిర్మాణం task_struct *p;
	పూర్ణాంకం detached = 0;

	lockdep_assert_rq_held(env->src_rq);

	/*
	 * Source run queue has been emptied by another CPU, clear
	 * LBF_ALL_PINNED flag as we will not test any task.
	 */
	ఒకవేళ (env->src_rq->nr_running <= 1) {
		env->flags &= ~LBF_ALL_PINNED;
		తిరిగి 0;
	}

	ఒకవేళ (env->imbalance <= 0)
		తిరిగి 0;

	అయితే (!list_ఖాళీలు(tasks)) {
		/*
		 * We don't want to steal all, otherwise we may be treated likewise,
		 * which could at worst lead to a livelock crash.
		 */
		ఒకవేళ (env->idle && env->src_rq->nr_running <= 1)
			ఆపు;

		env->loop++;
		/* We've more or less seen every task there is, call it quits */
		ఒకవేళ (env->loop > env->loop_max)
			ఆపు;

		/* take a breather every nr_migrate tasks */
		ఒకవేళ (env->loop > env->loop_break) {
			env->loop_break += SCHED_NR_MIGRATE_BREAK;
			env->flags |= LBF_NEED_BREAK;
			ఆపు;
		}

		p = list_చివరి_నమోదు(tasks, నిర్మాణం task_struct, se.సమూహం_కణుపు);

		ఒకవేళ (!can_migrate_task(p, env))
			వెళ్ళు తదుపరి;

		ఎంపిక (env->migration_type) {
		స్థితి migrate_ఎక్కించు:
			/*
			 * Depending of the number of CPUs and tasks and the
			 * cgroup hierarchy, task_h_load() can return a null
			 * value. Make sure that env->imbalance decreases
			 * otherwise detach_tasks() will stop only after
			 * detaching up to loop_max tasks.
			 */
			ఎక్కించు = max_t(గుర్తులేని దీర్ఘ, task_h_ఎక్కించు(p), 1);

			ఒకవేళ (sched_feat(LB_MIN) &&
			    ఎక్కించు < 16 && !env->sd->nr_balance_failed)
				వెళ్ళు తదుపరి;

			/*
			 * Make sure that we don't migrate too much load.
			 * Nevertheless, let relax the constraint if
			 * scheduler fails to find a good waiting task to
			 * migrate.
			 */
			ఒకవేళ (shr_bound(ఎక్కించు, env->sd->nr_balance_failed) > env->imbalance)
				వెళ్ళు తదుపరి;

			env->imbalance -= ఎక్కించు;
			ఆపు;

		స్థితి migrate_util:
			util = task_util_est(p);

			ఒకవేళ (shr_bound(util, env->sd->nr_balance_failed) > env->imbalance)
				వెళ్ళు తదుపరి;

			env->imbalance -= util;
			ఆపు;

		స్థితి migrate_task:
			env->imbalance--;
			ఆపు;

		స్థితి migrate_misfit:
			/* This is not a misfit task */
			ఒకవేళ (task_సరిపోతుంది_cpu(p, env->src_cpu))
				వెళ్ళు తదుపరి;

			env->imbalance = 0;
			ఆపు;

		స్థితి migrate_llc_task:
			env->imbalance--;
			ఆపు;
		}

		detach_task(p, env);
		list_కూడు(&p->se.సమూహం_కణుపు, &env->tasks);

		detached++;

#నిర్వచితమైతే CONFIG_PREEMPTION
		/*
		 * NEWIDLE balancing is a source of latency, so preemptible
		 * kernels will stop after the first task is detached to minimize
		 * the critical section.
		 */
		ఒకవేళ (env->idle == CPU_NEWLY_IDLE)
			ఆపు;
#అయితే_ముగింపు

		/*
		 * We only want to steal up to the prescribed amount of
		 * load/util/tasks.
		 */
		ఒకవేళ (env->imbalance <= 0)
			ఆపు;

		కొనసాగు;
తదుపరి:
		ఒకవేళ (p->sched_task_hot)
			schedstat_inc(p->stats.nr_failed_migrations_hot);

		list_move(&p->se.సమూహం_కణుపు, tasks);
	}

	/*
	 * Right now, this is one of only two places we collect this stat
	 * so we can safely collect detach_one_task() stats here rather
	 * than inside detach_one_task().
	 */
	schedstat_కూడు(env->sd->lb_gained[env->idle], detached);

	తిరిగి detached;
}

/*
 * attach_tasks() -- attaches all tasks detached by detach_tasks() to their
 * new rq.
 */
స్థిర శూన్య attach_tasks(నిర్మాణం lb_env *env)
{
	__attach_tasks(env->dst_rq, &env->tasks);
}

#నిర్వచితమైతే CONFIG_NO_HZ_COMMON
స్థిర అంతఃవరుస bool cfs_rq_has_blocked_ఎక్కించు(నిర్మాణం cfs_rq *cfs_rq)
{
	ఒకవేళ (cfs_rq->avg.ఎక్కించు_avg)
		తిరిగి true;

	ఒకవేళ (cfs_rq->avg.util_avg)
		తిరిగి true;

	తిరిగి false;
}

స్థిర అంతఃవరుస bool others_have_blocked(నిర్మాణం rq *rq)
{
	ఒకవేళ (cpu_util_rt(rq))
		తిరిగి true;

	ఒకవేళ (cpu_util_dl(rq))
		తిరిగి true;

	ఒకవేళ (hw_ఎక్కించు_avg(rq))
		తిరిగి true;

	ఒకవేళ (cpu_util_irq(rq))
		తిరిగి true;

	తిరిగి false;
}

స్థిర అంతఃవరుస శూన్య update_blocked_ఎక్కించు_tick(నిర్మాణం rq *rq)
{
	WRITE_ONCE(rq->చివరి_blocked_ఎక్కించు_update_tick, jiffies);
}

స్థిర అంతఃవరుస శూన్య update_has_blocked_ఎక్కించు_స్థితి(నిర్మాణం rq *rq, bool has_blocked_ఎక్కించు)
{
	ఒకవేళ (!has_blocked_ఎక్కించు)
		rq->has_blocked_ఎక్కించు = 0;
}
#లేకపోతే /* !CONFIG_NO_HZ_COMMON: */
స్థిర అంతఃవరుస bool cfs_rq_has_blocked_ఎక్కించు(నిర్మాణం cfs_rq *cfs_rq) { తిరిగి false; }
స్థిర అంతఃవరుస bool others_have_blocked(నిర్మాణం rq *rq) { తిరిగి false; }
స్థిర అంతఃవరుస శూన్య update_blocked_ఎక్కించు_tick(నిర్మాణం rq *rq) {}
స్థిర అంతఃవరుస శూన్య update_has_blocked_ఎక్కించు_స్థితి(నిర్మాణం rq *rq, bool has_blocked_ఎక్కించు) {}
#అయితే_ముగింపు /* !CONFIG_NO_HZ_COMMON */

స్థిర bool __update_blocked_others(నిర్మాణం rq *rq, bool *పూర్తి)
{
	bool updated;

	/*
	 * update_load_avg() can call cpufreq_update_util(). Make sure that RT,
	 * DL and IRQ signals have been updated before updating CFS.
	 */
	updated = update_other_ఎక్కించు_avgs(rq);

	ఒకవేళ (others_have_blocked(rq))
		*పూర్తి = false;

	తిరిగి updated;
}

#నిర్వచితమైతే CONFIG_FAIR_సమూహం_SCHED

స్థిర bool __update_blocked_fair(నిర్మాణం rq *rq, bool *పూర్తి)
{
	నిర్మాణం cfs_rq *cfs_rq, *pos;
	bool decayed = false;

	/*
	 * Iterates the task_group tree in a bottom up fashion, see
	 * list_add_leaf_cfs_rq() for details.
	 */
	for_each_ఆకు_cfs_rq_సురక్షిత(rq, cfs_rq, pos) {
		నిర్మాణం sched_entity *se;

		ఒకవేళ (update_cfs_rq_ఎక్కించు_avg(cfs_rq_clock_pelt(cfs_rq), cfs_rq)) {
			update_tg_ఎక్కించు_avg(cfs_rq);

			ఒకవేళ (cfs_rq->nr_queued == 0)
				update_idle_cfs_rq_clock_pelt(cfs_rq);

			ఒకవేళ (cfs_rq == &rq->cfs)
				decayed = true;
		}

		/* Propagate pending load changes to the parent, if any: */
		se = cfs_rq_se(cfs_rq);
		ఒకవేళ (se && !skip_blocked_update(se))
			update_ఎక్కించు_avg(cfs_rq_యొక్క(se), se, UPDATE_TG);

		/*
		 * There can be a lot of idle CPU cgroups.  Don't let fully
		 * decayed cfs_rqs linger on the list.
		 */
		ఒకవేళ (cfs_rq_is_decayed(cfs_rq))
			list_del_ఆకు_cfs_rq(cfs_rq);

		/* Don't need periodic decay once load/util_avg are null */
		ఒకవేళ (cfs_rq_has_blocked_ఎక్కించు(cfs_rq))
			*పూర్తి = false;
	}

	తిరిగి decayed;
}

/*
 * Compute the hierarchical load factor for cfs_rq and all its ascendants.
 * This needs to be done in a top-down fashion because the load of a child
 * group is a fraction of its parents load.
 */
స్థిర శూన్య update_cfs_rq_h_ఎక్కించు(నిర్మాణం cfs_rq *cfs_rq)
{
	నిర్మాణం sched_entity *se = cfs_rq_se(cfs_rq);
	గుర్తులేని దీర్ఘ now = jiffies;
	గుర్తులేని దీర్ఘ ఎక్కించు;

	ఒకవేళ (cfs_rq->చివరి_h_ఎక్కించు_update == now)
		తిరిగి;

	WRITE_ONCE(cfs_rq->h_ఎక్కించు_తదుపరి, ఖాళీ);
	for_each_sched_entity(se) {
		cfs_rq = cfs_rq_యొక్క(se);
		WRITE_ONCE(cfs_rq->h_ఎక్కించు_తదుపరి, se);
		ఒకవేళ (cfs_rq->చివరి_h_ఎక్కించు_update == now)
			ఆపు;
	}

	ఒకవేళ (!se) {
		cfs_rq->h_ఎక్కించు = cfs_rq_ఎక్కించు_avg(cfs_rq);
		cfs_rq->చివరి_h_ఎక్కించు_update = now;
	}

	అయితే ((se = READ_ONCE(cfs_rq->h_ఎక్కించు_తదుపరి)) != ఖాళీ) {
		ఎక్కించు = cfs_rq->h_ఎక్కించు;
		ఎక్కించు = div64_ul(ఎక్కించు * se->avg.ఎక్కించు_avg,
				cfs_rq_ఎక్కించు_avg(cfs_rq) + 1);
		cfs_rq = సమూహం_cfs_rq(se);
		cfs_rq->h_ఎక్కించు = ఎక్కించు;
		cfs_rq->చివరి_h_ఎక్కించు_update = now;
	}
}

స్థిర గుర్తులేని దీర్ఘ task_h_ఎక్కించు(నిర్మాణం task_struct *p)
{
	నిర్మాణం cfs_rq *cfs_rq = task_cfs_rq(p);

	update_cfs_rq_h_ఎక్కించు(cfs_rq);
	తిరిగి div64_ul(p->se.avg.ఎక్కించు_avg * cfs_rq->h_ఎక్కించు,
			cfs_rq_ఎక్కించు_avg(cfs_rq) + 1);
}
#లేకపోతే /* !CONFIG_FAIR_GROUP_SCHED: */
స్థిర bool __update_blocked_fair(నిర్మాణం rq *rq, bool *పూర్తి)
{
	నిర్మాణం cfs_rq *cfs_rq = &rq->cfs;
	bool decayed;

	decayed = update_cfs_rq_ఎక్కించు_avg(cfs_rq_clock_pelt(cfs_rq), cfs_rq);
	ఒకవేళ (cfs_rq_has_blocked_ఎక్కించు(cfs_rq))
		*పూర్తి = false;

	తిరిగి decayed;
}

స్థిర గుర్తులేని దీర్ఘ task_h_ఎక్కించు(నిర్మాణం task_struct *p)
{
	తిరిగి p->se.avg.ఎక్కించు_avg;
}
#అయితే_ముగింపు /* !CONFIG_FAIR_GROUP_SCHED */

స్థిర శూన్య __sched_balance_update_blocked_averages(నిర్మాణం rq *rq)
{
	bool decayed = false, పూర్తి = true;

	update_blocked_ఎక్కించు_tick(rq);

	decayed |= __update_blocked_others(rq, &పూర్తి);
	decayed |= __update_blocked_fair(rq, &పూర్తి);

	update_has_blocked_ఎక్కించు_స్థితి(rq, !పూర్తి);
	ఒకవేళ (decayed)
		cpufreq_update_util(rq, 0);
}

స్థిర శూన్య sched_balance_update_blocked_averages(పూర్ణాంకం cpu)
{
	నిర్మాణం rq *rq = cpu_rq(cpu);

	guard(rq_తాళం_irqsave)(rq);
	update_rq_clock(rq);
	__sched_balance_update_blocked_averages(rq);
}

/********** Helpers for sched_balance_find_src_group ************************/

/*
 * sg_lb_stats - stats of a sched_group required for load-balancing:
 */
నిర్మాణం sg_lb_stats {
	గుర్తులేని దీర్ఘ avg_ఎక్కించు;			/* Avg load            over the CPUs of the group */
	గుర్తులేని దీర్ఘ సమూహం_ఎక్కించు;		/* Total load          over the CPUs of the group */
	గుర్తులేని దీర్ఘ సమూహం_సామర్థ్యం;		/* Capacity            over the CPUs of the group */
	గుర్తులేని దీర్ఘ సమూహం_util;		/* Total utilization   over the CPUs of the group */
	గుర్తులేని దీర్ఘ సమూహం_runnable;		/* Total runnable time over the CPUs of the group */
	గుర్తులేని పూర్ణాంకం కూడిక_nr_running;		/* Nr of all tasks running in the group */
	గుర్తులేని పూర్ణాంకం కూడిక_h_nr_running;		/* Nr of CFS tasks running in the group */
	గుర్తులేని పూర్ణాంకం idle_cpus;                 /* Nr of idle CPUs         in the group */
	గుర్తులేని పూర్ణాంకం సమూహం_బరువు;
	గణన సమూహం_type సమూహం_type;
	గుర్తులేని పూర్ణాంకం సమూహం_asym_packing;	/* Tasks should be moved to preferred CPU */
	గుర్తులేని పూర్ణాంకం సమూహం_smt_balance;		/* Task on busy SMT be moved */
	గుర్తులేని పూర్ణాంకం సమూహం_llc_balance;		/* Tasks should be moved to preferred LLC */
	గుర్తులేని దీర్ఘ సమూహం_misfit_task_ఎక్కించు;	/* A CPU has a task too big for its capacity */
	గుర్తులేని పూర్ణాంకం సమూహం_overutilized;	/* At least one CPU is overutilized in the group */
#నిర్వచితమైతే CONFIG_NUMA_BALANCING
	గుర్తులేని పూర్ణాంకం nr_numa_running;
	గుర్తులేని పూర్ణాంకం nr_preferred_running;
#అయితే_ముగింపు
#నిర్వచితమైతే CONFIG_SCHED_CACHE
	గుర్తులేని పూర్ణాంకం nr_pref_dst_llc;
#అయితే_ముగింపు
};

/*
 * sd_lb_stats - stats of a sched_domain required for load-balancing:
 */
నిర్మాణం sd_lb_stats {
	నిర్మాణం sched_సమూహం *busiest;		/* Busiest group in this sd */
	నిర్మాణం sched_సమూహం *local;		/* Local group in this sd */
	గుర్తులేని దీర్ఘ మొత్తం_ఎక్కించు;		/* Total load of all groups in sd */
	గుర్తులేని దీర్ఘ మొత్తం_సామర్థ్యం;		/* Total capacity of all groups in sd */
	గుర్తులేని దీర్ఘ avg_ఎక్కించు;			/* Average load across all groups in sd */
	గుర్తులేని పూర్ణాంకం prefer_sibling;		/* Tasks should go to sibling first */

	నిర్మాణం sg_lb_stats busiest_stat;	/* Statistics of the busiest group */
	నిర్మాణం sg_lb_stats local_stat;		/* Statistics of the local group */
};

స్థిర అంతఃవరుస శూన్య init_sd_lb_stats(నిర్మాణం sd_lb_stats *sds)
{
	/*
	 * Skimp on the clearing to avoid duplicate work. We can avoid clearing
	 * local_stat because update_sg_lb_stats() does a full clear/assignment.
	 * We must however set busiest_stat::group_type and
	 * busiest_stat::idle_cpus to the worst busiest group because
	 * update_sd_pick_busiest() reads these before assignment.
	 */
	*sds = (నిర్మాణం sd_lb_stats){
		.busiest = ఖాళీ,
		.local = ఖాళీ,
		.మొత్తం_ఎక్కించు = 0UL,
		.మొత్తం_సామర్థ్యం = 0UL,
		.busiest_stat = {
			.idle_cpus = UINT_MAX,
			.సమూహం_type = సమూహం_has_అదనపు,
		},
	};
}

స్థిర గుర్తులేని దీర్ఘ scale_rt_సామర్థ్యం(పూర్ణాంకం cpu)
{
	గుర్తులేని దీర్ఘ max = get_actual_cpu_సామర్థ్యం(cpu);
	నిర్మాణం rq *rq = cpu_rq(cpu);
	గుర్తులేని దీర్ఘ వాడిన, విడుదల;
	గుర్తులేని దీర్ఘ irq;

	irq = cpu_util_irq(rq);

	ఒకవేళ (unlikely(irq >= max))
		తిరిగి 1;

	/*
	 * avg_rt.util_avg and avg_dl.util_avg track binary signals
	 * (running and not running) with weights 0 and 1024 respectively.
	 */
	వాడిన = cpu_util_rt(rq);
	వాడిన += cpu_util_dl(rq);

	ఒకవేళ (unlikely(వాడిన >= max))
		తిరిగి 1;

	విడుదల = max - వాడిన;

	తిరిగి scale_irq_సామర్థ్యం(విడుదల, irq, max);
}

స్థిర శూన్య update_cpu_సామర్థ్యం(నిర్మాణం sched_domain *sd, పూర్ణాంకం cpu)
{
	గుర్తులేని దీర్ఘ సామర్థ్యం = scale_rt_సామర్థ్యం(cpu);
	నిర్మాణం sched_సమూహం *sdg = sd->groups;

	ఒకవేళ (!సామర్థ్యం)
		సామర్థ్యం = 1;

	cpu_rq(cpu)->cpu_సామర్థ్యం = సామర్థ్యం;
	trace_sched_cpu_సామర్థ్యం_tp(cpu_rq(cpu));

	sdg->sgc->సామర్థ్యం = సామర్థ్యం;
	sdg->sgc->min_సామర్థ్యం = సామర్థ్యం;
	sdg->sgc->max_సామర్థ్యం = సామర్థ్యం;
}

శూన్య update_సమూహం_సామర్థ్యం(నిర్మాణం sched_domain *sd, పూర్ణాంకం cpu)
{
	నిర్మాణం sched_domain *బిడ్డ = sd->బిడ్డ;
	నిర్మాణం sched_సమూహం *సమూహం, *sdg = sd->groups;
	గుర్తులేని దీర్ఘ సామర్థ్యం, min_సామర్థ్యం, max_సామర్థ్యం;
	గుర్తులేని దీర్ఘ interval;

	interval = msecs_గమ్యం_jiffies(sd->balance_interval);
	interval = clamp(interval, 1UL, max_ఎక్కించు_balance_interval);
	sdg->sgc->తదుపరి_update = jiffies + interval;

	ఒకవేళ (!బిడ్డ) {
		update_cpu_సామర్థ్యం(sd, cpu);
		తిరిగి;
	}

	సామర్థ్యం = 0;
	min_సామర్థ్యం = ULONG_MAX;
	max_సామర్థ్యం = 0;

	ఒకవేళ (బిడ్డ->flags & SD_NUMA) {
		/*
		 * SD_NUMA domains cannot assume that child groups
		 * span the current group.
		 */

		for_each_cpu(cpu, sched_సమూహం_పరిధి(sdg)) {
			గుర్తులేని దీర్ఘ cpu_cap = సామర్థ్యం_యొక్క(cpu);

			సామర్థ్యం += cpu_cap;
			min_సామర్థ్యం = min(cpu_cap, min_సామర్థ్యం);
			max_సామర్థ్యం = max(cpu_cap, max_సామర్థ్యం);
		}
	} లేకపోతే  {
		/*
		 * !SD_NUMA domains can assume that child groups
		 * span the current group.
		 */

		సమూహం = బిడ్డ->groups;
		చేయి {
			నిర్మాణం sched_సమూహం_సామర్థ్యం *sgc = సమూహం->sgc;

			సామర్థ్యం += sgc->సామర్థ్యం;
			min_సామర్థ్యం = min(sgc->min_సామర్థ్యం, min_సామర్థ్యం);
			max_సామర్థ్యం = max(sgc->max_సామర్థ్యం, max_సామర్థ్యం);
			సమూహం = సమూహం->తదుపరి;
		} అయితే (సమూహం != బిడ్డ->groups);
	}

	sdg->sgc->సామర్థ్యం = సామర్థ్యం;
	sdg->sgc->min_సామర్థ్యం = min_సామర్థ్యం;
	sdg->sgc->max_సామర్థ్యం = max_సామర్థ్యం;
}

/*
 * Check whether the capacity of the rq has been noticeably reduced by side
 * activity. The imbalance_pct is used for the threshold.
 * Return true is the capacity is reduced
 */
స్థిర అంతఃవరుస పూర్ణాంకం
check_cpu_సామర్థ్యం(నిర్మాణం rq *rq, నిర్మాణం sched_domain *sd)
{
	తిరిగి ((rq->cpu_సామర్థ్యం * sd->imbalance_pct) <
				(arch_scale_cpu_సామర్థ్యం(cpu_యొక్క(rq)) * 100));
}

/* Check if the rq has a misfit task */
స్థిర అంతఃవరుస bool check_misfit_స్థితి(నిర్మాణం rq *rq)
{
	తిరిగి rq->misfit_task_ఎక్కించు;
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

స్థిర అంతఃవరుస పూర్ణాంకం sg_imbalanced(నిర్మాణం sched_సమూహం *సమూహం)
{
	తిరిగి సమూహం->sgc->imbalance;
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
స్థిర అంతఃవరుస bool
సమూహం_has_సామర్థ్యం(గుర్తులేని పూర్ణాంకం imbalance_pct, నిర్మాణం sg_lb_stats *sgs)
{
	ఒకవేళ (sgs->కూడిక_nr_running < sgs->సమూహం_బరువు)
		తిరిగి true;

	ఒకవేళ ((sgs->సమూహం_సామర్థ్యం * imbalance_pct) <
			(sgs->సమూహం_runnable * 100))
		తిరిగి false;

	ఒకవేళ ((sgs->సమూహం_సామర్థ్యం * 100) >
			(sgs->సమూహం_util * imbalance_pct))
		తిరిగి true;

	తిరిగి false;
}

/*
 *  group_is_overloaded returns true if the group has more tasks than it can
 *  handle.
 *  group_is_overloaded is not equals to !group_has_capacity because a group
 *  with the exact right number of tasks, has no more spare capacity but is not
 *  overloaded so both group_has_capacity and group_is_overloaded return
 *  false.
 */
స్థిర అంతఃవరుస bool
సమూహం_is_overloaded(గుర్తులేని పూర్ణాంకం imbalance_pct, నిర్మాణం sg_lb_stats *sgs)
{
	/*
	 * With EAS and uclamp, 1 CPU in the group must be overutilized to
	 * consider the group overloaded.
	 */
	ఒకవేళ (sched_energy_enabled() && !sgs->సమూహం_overutilized)
		తిరిగి false;

	ఒకవేళ (sgs->కూడిక_nr_running <= sgs->సమూహం_బరువు)
		తిరిగి false;

	ఒకవేళ ((sgs->సమూహం_సామర్థ్యం * 100) <
			(sgs->సమూహం_util * imbalance_pct))
		తిరిగి true;

	ఒకవేళ ((sgs->సమూహం_సామర్థ్యం * imbalance_pct) <
			(sgs->సమూహం_runnable * 100))
		తిరిగి true;

	తిరిగి false;
}

స్థిర అంతఃవరుస గణన
సమూహం_type సమూహం_classify(గుర్తులేని పూర్ణాంకం imbalance_pct,
			  నిర్మాణం sched_సమూహం *సమూహం,
			  నిర్మాణం sg_lb_stats *sgs)
{
	ఒకవేళ (సమూహం_is_overloaded(imbalance_pct, sgs))
		తిరిగి సమూహం_overloaded;

	ఒకవేళ (sgs->సమూహం_llc_balance)
		తిరిగి సమూహం_llc_balance;

	ఒకవేళ (sg_imbalanced(సమూహం))
		తిరిగి సమూహం_imbalanced;

	ఒకవేళ (sgs->సమూహం_asym_packing)
		తిరిగి సమూహం_asym_packing;

	ఒకవేళ (sgs->సమూహం_smt_balance)
		తిరిగి సమూహం_smt_balance;

	ఒకవేళ (sgs->సమూహం_misfit_task_ఎక్కించు)
		తిరిగి సమూహం_misfit_task;

	ఒకవేళ (!సమూహం_has_సామర్థ్యం(imbalance_pct, sgs))
		తిరిగి సమూహం_fully_busy;

	తిరిగి సమూహం_has_అదనపు;
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
స్థిర bool sched_use_asym_prio(నిర్మాణం sched_domain *sd, పూర్ణాంకం cpu)
{
	ఒకవేళ (!(sd->flags & SD_ASYM_PACKING))
		తిరిగి false;

	ఒకవేళ (!sched_smt_చురుకైన())
		తిరిగి true;

	తిరిగి sd->flags & SD_SHARE_CPUCAPACITY || is_core_idle(cpu);
}

స్థిర అంతఃవరుస bool sched_asym(నిర్మాణం sched_domain *sd, పూర్ణాంకం dst_cpu, పూర్ణాంకం src_cpu)
{
	/*
	 * First check if @dst_cpu can do asym_packing load balance. Only do it
	 * if it has higher priority than @src_cpu.
	 */
	తిరిగి sched_use_asym_prio(sd, dst_cpu) &&
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
స్థిర అంతఃవరుస bool
sched_సమూహం_asym(నిర్మాణం lb_env *env, నిర్మాణం sg_lb_stats *sgs, నిర్మాణం sched_సమూహం *సమూహం)
{
	/*
	 * CPU priorities do not make sense for SMT cores with more than one
	 * busy sibling.
	 */
	ఒకవేళ ((సమూహం->flags & SD_SHARE_CPUCAPACITY) &&
	    (sgs->సమూహం_బరువు - sgs->idle_cpus != 1))
		తిరిగి false;

	తిరిగి sched_asym(env->sd, env->dst_cpu, READ_ONCE(సమూహం->asym_prefer_cpu));
}

/* One group has more than one SMT CPU while the other group does not */
స్థిర అంతఃవరుస bool smt_vs_nonsmt_groups(నిర్మాణం sched_సమూహం *sg1,
				    నిర్మాణం sched_సమూహం *sg2)
{
	ఒకవేళ (!sg1 || !sg2)
		తిరిగి false;

	తిరిగి (sg1->flags & SD_SHARE_CPUCAPACITY) !=
		(sg2->flags & SD_SHARE_CPUCAPACITY);
}

స్థిర అంతఃవరుస bool smt_balance(నిర్మాణం lb_env *env, నిర్మాణం sg_lb_stats *sgs,
			       నిర్మాణం sched_సమూహం *సమూహం)
{
	ఒకవేళ (!env->idle)
		తిరిగి false;

	/*
	 * For SMT source group, it is better to move a task
	 * to a CPU that doesn't have multiple tasks sharing its CPU capacity.
	 * Note that if a group has a single SMT, SD_SHARE_CPUCAPACITY
	 * will not be on.
	 */
	ఒకవేళ (సమూహం->flags & SD_SHARE_CPUCAPACITY &&
	    sgs->కూడిక_h_nr_running > 1)
		తిరిగి true;

	తిరిగి false;
}

స్థిర అంతఃవరుస దీర్ఘ sibling_imbalance(నిర్మాణం lb_env *env,
				    నిర్మాణం sd_lb_stats *sds,
				    నిర్మాణం sg_lb_stats *busiest,
				    నిర్మాణం sg_lb_stats *local)
{
	పూర్ణాంకం ncores_busiest, ncores_local;
	దీర్ఘ imbalance;

	ఒకవేళ (!env->idle || !busiest->కూడిక_nr_running)
		తిరిగి 0;

	ncores_busiest = sds->busiest->cores;
	ncores_local = sds->local->cores;

	ఒకవేళ (ncores_busiest == ncores_local) {
		imbalance = busiest->కూడిక_nr_running;
		lsub_ధనాత్మక(&imbalance, local->కూడిక_nr_running);
		తిరిగి imbalance;
	}

	/* Balance such that nr_running/ncores ratio are same on both groups */
	imbalance = ncores_local * busiest->కూడిక_nr_running;
	lsub_ధనాత్మక(&imbalance, ncores_busiest * local->కూడిక_nr_running);
	/* Normalize imbalance and do rounding on normalization */
	imbalance = 2 * imbalance + ncores_local + ncores_busiest;
	imbalance /= ncores_local + ncores_busiest;

	/* Take advantage of resource in an empty sched group */
	ఒకవేళ (imbalance <= 1 && local->కూడిక_nr_running == 0 &&
	    busiest->కూడిక_nr_running > 1)
		imbalance = 2;

	తిరిగి imbalance;
}

స్థిర అంతఃవరుస bool
sched_reduced_సామర్థ్యం(నిర్మాణం rq *rq, నిర్మాణం sched_domain *sd)
{
	/*
	 * When there is more than 1 task, the group_overloaded case already
	 * takes care of cpu with reduced capacity
	 */
	ఒకవేళ (rq->cfs.h_nr_runnable != 1)
		తిరిగి false;

	తిరిగి check_cpu_సామర్థ్యం(rq, sd);
}

#నిర్వచితమైతే CONFIG_SCHED_CACHE
/*
 * Record the statistics for this scheduler group for later
 * use. These values guide load balancing on aggregating tasks
 * to a LLC.
 */
స్థిర శూన్య రికార్డు_sg_llc_stats(నిర్మాణం lb_env *env,
				నిర్మాణం sg_lb_stats *sgs,
				నిర్మాణం sched_సమూహం *సమూహం)
{
	నిర్మాణం sched_domain_shared *sd_share;
	పూర్ణాంకం cpu;

	ఒకవేళ (!sched_cache_enabled() || env->idle == CPU_NEWLY_IDLE)
		తిరిగి;

	/* Only care about sched domain spanning multiple LLCs */
	ఒకవేళ (env->sd->బిడ్డ != rcu_dereference_all(చొప్పున_cpu(sd_llc, env->dst_cpu)))
		తిరిగి;

	/*
	 * A redo repeats stats published on the first pass. Hotplug can
	 * still leave env->cpus partial, so util would not match capacity.
	 */
	ఒకవేళ ((env->flags & LBF_ALL_PINNED) ||
	    !cpumask_subset(sched_సమూహం_పరిధి(సమూహం), env->cpus))
		తిరిగి;

	/*
	 * At this point we know this group spans a LLC domain.
	 * Record the statistic of this group in its corresponding
	 * shared LLC domain.
	 * Note: sd_share cannot be obtained via sd->child->shared,
	 * because the latter refers to the domain that covers the
	 * local group. Instead, sd_share should be located using
	 * the first CPU of the LLC group.
	 */
	cpu = cpumask_మొదటి(sched_సమూహం_పరిధి(సమూహం));
	sd_share = rcu_dereference_all(చొప్పున_cpu(sd_llc_shared, cpu));
	ఒకవేళ (!sd_share)
		తిరిగి;

	ఒకవేళ (READ_ONCE(sd_share->util_avg) != sgs->సమూహం_util)
		WRITE_ONCE(sd_share->util_avg, sgs->సమూహం_util);

	ఒకవేళ (unlikely(READ_ONCE(sd_share->సామర్థ్యం) != sgs->సమూహం_సామర్థ్యం))
		WRITE_ONCE(sd_share->సామర్థ్యం, sgs->సమూహం_సామర్థ్యం);
}

/*
 * Do LLC balance on sched group that contains LLC, and have tasks preferring
 * to run on LLC in idle dst_cpu.
 */
స్థిర అంతఃవరుస bool llc_balance(నిర్మాణం lb_env *env, నిర్మాణం sg_lb_stats *sgs,
			       నిర్మాణం sched_సమూహం *సమూహం)
{
	ఒకవేళ (!sched_cache_enabled())
		తిరిగి false;

	ఒకవేళ (env->sd->flags & SD_SHARE_LLC)
		తిరిగి false;

	/*
	 * On asymmetric domains, group_misfit_task_load
	 * should be prioritized to move tasks to CPU that fit them
	 * over aggregating tasks to their preferred LLC.
	 */
	ఒకవేళ ((env->sd->flags & SD_ASYM_CPUCAPACITY) &&
	    sgs->సమూహం_misfit_task_ఎక్కించు)
		తిరిగి false;

	/*
	 * Skip cache aware tagging if nr_balanced_failed is sufficiently high.
	 * Threshold of cache_nice_tries is set to 1 higher than nr_balance_failed
	 * to avoid excessive task migration at the same time.
	 */
	ఒకవేళ (env->sd->nr_balance_failed >= env->sd->cache_nice_tries + 1)
		తిరిగి false;

	ఒకవేళ (sgs->nr_pref_dst_llc &&
	    can_migrate_llc(cpumask_మొదటి(sched_సమూహం_పరిధి(సమూహం)),
			    env->dst_cpu, 0, true) == mig_llc)
		తిరిగి true;

	తిరిగి false;
}

స్థిర bool update_llc_busiest(నిర్మాణం lb_env *env,
			       నిర్మాణం sg_lb_stats *busiest,
			       నిర్మాణం sg_lb_stats *sgs)
{
	/*
	 * There are more tasks that want to run on dst_cpu's LLC.
	 */
	తిరిగి sgs->nr_pref_dst_llc > busiest->nr_pref_dst_llc;
}
#లేకపోతే
స్థిర అంతఃవరుస శూన్య రికార్డు_sg_llc_stats(నిర్మాణం lb_env *env, నిర్మాణం sg_lb_stats *sgs,
				       నిర్మాణం sched_సమూహం *సమూహం)
{
}

స్థిర అంతఃవరుస bool llc_balance(నిర్మాణం lb_env *env, నిర్మాణం sg_lb_stats *sgs,
			       నిర్మాణం sched_సమూహం *సమూహం)
{
	తిరిగి false;
}

స్థిర bool update_llc_busiest(నిర్మాణం lb_env *env,
			       నిర్మాణం sg_lb_stats *busiest,
			       నిర్మాణం sg_lb_stats *sgs)
{
	తిరిగి false;
}
#అయితే_ముగింపు

/**
 * update_sg_lb_stats - Update sched_group's statistics for load balancing.
 * @env: The load balancing environment.
 * @sds: Load-balancing data with statistics of the local group.
 * @group: sched_group whose statistics are to be updated.
 * @sgs: variable to hold the statistics for this group.
 * @sg_overloaded: sched_group is overloaded
 */
స్థిర అంతఃవరుస శూన్య update_sg_lb_stats(నిర్మాణం lb_env *env,
				      నిర్మాణం sd_lb_stats *sds,
				      నిర్మాణం sched_సమూహం *సమూహం,
				      నిర్మాణం sg_lb_stats *sgs,
				      bool *sg_overloaded)
{
	పూర్ణాంకం i, nr_running, local_సమూహం, sd_flags = env->sd->flags;
	bool balancing_at_rd = !env->sd->మాతృ;

	memset(sgs, 0, పరిమాణం(*sgs));

	local_సమూహం = సమూహం == sds->local;

	for_each_cpu_and(i, sched_సమూహం_పరిధి(సమూహం), env->cpus) {
		నిర్మాణం rq *rq = cpu_rq(i);
		గుర్తులేని దీర్ఘ ఎక్కించు = cpu_ఎక్కించు(rq);

		sgs->సమూహం_ఎక్కించు += ఎక్కించు;
		sgs->సమూహం_util += cpu_util_cfs(i);
		sgs->సమూహం_runnable += cpu_runnable(rq);
		sgs->కూడిక_h_nr_running += rq->cfs.h_nr_runnable;

		nr_running = rq->nr_running;
		sgs->కూడిక_nr_running += nr_running;

		ఒకవేళ (cpu_overutilized(i))
			sgs->సమూహం_overutilized = 1;

#నిర్వచితమైతే CONFIG_SCHED_CACHE
		ఒకవేళ (sched_cache_enabled()) {
			నిర్మాణం sched_domain *sd_tmp;
			పూర్ణాంకం dst_llc;

			dst_llc = llc_id(env->dst_cpu);
			ఒకవేళ (llc_id(i) != dst_llc) {
				sd_tmp = rcu_dereference_all(rq->sd);
				ఒకవేళ (sd_tmp && (గుర్తులేని పూర్ణాంకం)dst_llc < sd_tmp->llc_max)
					sgs->nr_pref_dst_llc += sd_tmp->llc_counts[dst_llc];
			}
		}
#అయితే_ముగింపు

		/*
		 * No need to call idle_cpu() if nr_running is not 0
		 */
		ఒకవేళ (!nr_running && idle_cpu(i)) {
			sgs->idle_cpus++;
			/* Idle cpu can't have misfit task */
			కొనసాగు;
		}

		/* Overload indicator is only updated at root domain */
		ఒకవేళ (balancing_at_rd && nr_running > 1)
			*sg_overloaded = 1;

#నిర్వచితమైతే CONFIG_NUMA_BALANCING
		/* Only fbq_classify_group() uses this to classify NUMA groups */
		ఒకవేళ (sd_flags & SD_NUMA) {
			sgs->nr_numa_running += rq->nr_numa_running;
			sgs->nr_preferred_running += rq->nr_preferred_running;
		}
#అయితే_ముగింపు
		ఒకవేళ (local_సమూహం)
			కొనసాగు;

		ఒకవేళ (sd_flags & SD_ASYM_CPUCAPACITY) {
			ఒకవేళ (rq->misfit_task_ఎక్కించు) {
				/*
				 * Always mark the root domain overloaded so big
				 * CPUs can pick up misfit tasks via newly idle
				 * balance.
				 */
				ఒకవేళ (balancing_at_rd)
					*sg_overloaded = 1;

				/*
				 * Only account misfit load if @dst_cpu can
				 * help; otherwise, the group may be classified
				 * as misfit_task and update_sd_pick_busiest()
				 * will skip it.
				 */
				ఒకవేళ (సామర్థ్యం_greater(సామర్థ్యం_యొక్క(env->dst_cpu),
						     సమూహం->sgc->max_సామర్థ్యం) &&
				    (sgs->సమూహం_misfit_task_ఎక్కించు < rq->misfit_task_ఎక్కించు))
					sgs->సమూహం_misfit_task_ఎక్కించు = rq->misfit_task_ఎక్కించు;
			}
		} లేకపోతే ఒకవేళ (env->idle && sched_reduced_సామర్థ్యం(rq, env->sd)) {
			/* Check for a task running on a CPU with reduced capacity */
			ఒకవేళ (sgs->సమూహం_misfit_task_ఎక్కించు < ఎక్కించు)
				sgs->సమూహం_misfit_task_ఎక్కించు = ఎక్కించు;
		}
	}

	sgs->సమూహం_సామర్థ్యం = సమూహం->sgc->సామర్థ్యం;

	sgs->సమూహం_బరువు = సమూహం->సమూహం_బరువు;

	ఒకవేళ (!local_సమూహం) {
		/* Check if dst CPU is idle and preferred to this group */
		ఒకవేళ (env->idle && sgs->కూడిక_h_nr_running &&
		    sched_సమూహం_asym(env, sgs, సమూహం))
			sgs->సమూహం_asym_packing = 1;

		/* Check for loaded SMT group to be balanced to dst CPU */
		ఒకవేళ (smt_balance(env, sgs, సమూహం))
			sgs->సమూహం_smt_balance = 1;

		/* Check for tasks in this group can be moved to their preferred LLC */
		ఒకవేళ (llc_balance(env, sgs, సమూహం))
			sgs->సమూహం_llc_balance = 1;
	}

	sgs->సమూహం_type = సమూహం_classify(env->sd->imbalance_pct, సమూహం, sgs);

	రికార్డు_sg_llc_stats(env, sgs, సమూహం);
	/* Computing avg_load makes sense only when group is overloaded */
	ఒకవేళ (sgs->సమూహం_type == సమూహం_overloaded)
		sgs->avg_ఎక్కించు = (sgs->సమూహం_ఎక్కించు * SCHED_సామర్థ్యం_SCALE) /
				sgs->సమూహం_సామర్థ్యం;
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
స్థిర bool update_sd_pick_busiest(నిర్మాణం lb_env *env,
				   నిర్మాణం sd_lb_stats *sds,
				   నిర్మాణం sched_సమూహం *sg,
				   నిర్మాణం sg_lb_stats *sgs)
{
	నిర్మాణం sg_lb_stats *busiest = &sds->busiest_stat;

	/* Make sure that there is at least one task to pull */
	ఒకవేళ (!sgs->కూడిక_h_nr_running)
		తిరిగి false;

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
	ఒకవేళ ((env->sd->flags & SD_ASYM_CPUCAPACITY) &&
	    (sgs->సమూహం_type == సమూహం_misfit_task) &&
	    (!env->dst_core_idle ||
	     !సామర్థ్యం_greater(సామర్థ్యం_యొక్క(env->dst_cpu), sg->sgc->max_సామర్థ్యం) ||
	     sds->local_stat.సమూహం_type != సమూహం_has_అదనపు))
		తిరిగి false;

	/*
	 * Candidate sg has no more than one task per CPU and has higher
	 * per-CPU capacity. Migrating tasks to less capable CPUs may harm
	 * throughput. Maximize throughput, power/energy consequences are not
	 * considered.
	 */
	ఒకవేళ ((env->sd->flags & SD_ASYM_CPUCAPACITY) &&
	    (sgs->సమూహం_type <= సమూహం_fully_busy) &&
	    (సామర్థ్యం_greater(sg->sgc->min_సామర్థ్యం, సామర్థ్యం_యొక్క(env->dst_cpu))))
		తిరిగి false;

	ఒకవేళ (sgs->సమూహం_type > busiest->సమూహం_type)
		తిరిగి true;

	ఒకవేళ (sgs->సమూహం_type < busiest->సమూహం_type)
		తిరిగి false;

	/*
	 * The candidate and the current busiest group are the same type of
	 * group. Let check which one is the busiest according to the type.
	 */

	ఎంపిక (sgs->సమూహం_type) {
	స్థితి సమూహం_overloaded:
		/* Select the overloaded group with highest avg_load. */
		తిరిగి sgs->avg_ఎక్కించు > busiest->avg_ఎక్కించు;

	స్థితి సమూహం_llc_balance:
		/* Select the group with most tasks preferring dst LLC */
		తిరిగి update_llc_busiest(env, busiest, sgs);

	స్థితి సమూహం_imbalanced:
		/*
		 * Select the 1st imbalanced group as we don't have any way to
		 * choose one more than another.
		 */
		తిరిగి false;

	స్థితి సమూహం_asym_packing:
		/* Prefer to move from lowest priority CPU's work */
		తిరిగి sched_asym_prefer(READ_ONCE(sds->busiest->asym_prefer_cpu),
					 READ_ONCE(sg->asym_prefer_cpu));

	స్థితి సమూహం_misfit_task:
		/*
		 * If we have more than one misfit sg go with the biggest
		 * misfit.
		 */
		తిరిగి sgs->సమూహం_misfit_task_ఎక్కించు > busiest->సమూహం_misfit_task_ఎక్కించు;

	స్థితి సమూహం_smt_balance:
		/*
		 * Check if we have spare CPUs on either SMT group to
		 * choose has spare or fully busy handling.
		 */
		ఒకవేళ (sgs->idle_cpus != 0 || busiest->idle_cpus != 0)
			వెళ్ళు has_అదనపు;

		fallthrough;

	స్థితి సమూహం_fully_busy:
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

		ఒకవేళ (sgs->avg_ఎక్కించు < busiest->avg_ఎక్కించు)
			తిరిగి false;

		ఒకవేళ (sgs->avg_ఎక్కించు == busiest->avg_ఎక్కించు) {
			/*
			 * SMT sched groups need more help than non-SMT groups.
			 * If @sg happens to also be SMT, either choice is good.
			 */
			ఒకవేళ (sds->busiest->flags & SD_SHARE_CPUCAPACITY)
				తిరిగి false;
		}

		ఆపు;

	స్థితి సమూహం_has_అదనపు:
		/*
		 * Do not pick sg with SMT CPUs over sg with pure CPUs,
		 * as we do not want to pull task off SMT core with one task
		 * and make the core idle.
		 */
		ఒకవేళ (smt_vs_nonsmt_groups(sds->busiest, sg)) {
			ఒకవేళ (sg->flags & SD_SHARE_CPUCAPACITY && sgs->కూడిక_h_nr_running <= 1)
				తిరిగి false;
			లేకపోతే
				తిరిగి true;
		}
has_అదనపు:

		/*
		 * Select not overloaded group with lowest number of idle CPUs
		 * and highest number of running tasks. We could also compare
		 * the spare capacity which is more stable but it can end up
		 * that the group has less spare capacity but finally more idle
		 * CPUs which means less opportunity to pull tasks.
		 */
		ఒకవేళ (sgs->idle_cpus > busiest->idle_cpus)
			తిరిగి false;
		లేకపోతే ఒకవేళ ((sgs->idle_cpus == busiest->idle_cpus) &&
			 (sgs->కూడిక_nr_running <= busiest->కూడిక_nr_running))
			తిరిగి false;

		ఆపు;
	}

	తిరిగి true;
}

#నిర్వచితమైతే CONFIG_NUMA_BALANCING
స్థిర అంతఃవరుస గణన fbq_type fbq_classify_సమూహం(నిర్మాణం sg_lb_stats *sgs)
{
	ఒకవేళ (sgs->కూడిక_h_nr_running > sgs->nr_numa_running)
		తిరిగి regular;
	ఒకవేళ (sgs->కూడిక_h_nr_running > sgs->nr_preferred_running)
		తిరిగి remote;
	తిరిగి all;
}

స్థిర అంతఃవరుస గణన fbq_type fbq_classify_rq(నిర్మాణం rq *rq)
{
	ఒకవేళ (rq->nr_running > rq->nr_numa_running)
		తిరిగి regular;
	ఒకవేళ (rq->nr_running > rq->nr_preferred_running)
		తిరిగి remote;
	తిరిగి all;
}
#లేకపోతే /* !CONFIG_NUMA_BALANCING: */
స్థిర అంతఃవరుస గణన fbq_type fbq_classify_సమూహం(నిర్మాణం sg_lb_stats *sgs)
{
	తిరిగి all;
}

స్థిర అంతఃవరుస గణన fbq_type fbq_classify_rq(నిర్మాణం rq *rq)
{
	తిరిగి regular;
}
#అయితే_ముగింపు /* !CONFIG_NUMA_BALANCING */


నిర్మాణం sg_lb_stats;

/*
 * task_running_on_cpu - return 1 if @p is running on @cpu.
 */

స్థిర గుర్తులేని పూర్ణాంకం task_running_on_cpu(పూర్ణాంకం cpu, నిర్మాణం task_struct *p)
{
	/* Task has no contribution or is new */
	ఒకవేళ (cpu != task_cpu(p) || !READ_ONCE(p->se.avg.చివరి_update_సమయం))
		తిరిగి 0;

	ఒకవేళ (task_on_rq_queued(p))
		తిరిగి 1;

	తిరిగి 0;
}

/**
 * idle_cpu_without - would a given CPU be idle without p ?
 * @cpu: the processor on which idleness is tested.
 * @p: task which should be ignored.
 *
 * Return: 1 if the CPU would be idle. 0 otherwise.
 */
స్థిర పూర్ణాంకం idle_cpu_without(పూర్ణాంకం cpu, నిర్మాణం task_struct *p)
{
	నిర్మాణం rq *rq = cpu_rq(cpu);

	ఒకవేళ (rq->curr != rq->idle && rq->curr != p)
		తిరిగి 0;

	/*
	 * rq->nr_running can't be used but an updated version without the
	 * impact of p on cpu must be used instead. The updated nr_running
	 * be computed and tested before calling idle_cpu_without().
	 */

	ఒకవేళ (rq->ttwu_pending)
		తిరిగి 0;

	తిరిగి 1;
}

/*
 * update_sg_wakeup_stats - Update sched_group's statistics for wakeup.
 * @sd: The sched_domain level to look for idlest group.
 * @group: sched_group whose statistics are to be updated.
 * @sgs: variable to hold the statistics for this group.
 * @p: The task for which we look for the idlest group/CPU.
 */
స్థిర అంతఃవరుస శూన్య update_sg_wakeup_stats(నిర్మాణం sched_domain *sd,
					  నిర్మాణం sched_సమూహం *సమూహం,
					  నిర్మాణం sg_lb_stats *sgs,
					  నిర్మాణం task_struct *p)
{
	పూర్ణాంకం i, nr_running;

	memset(sgs, 0, పరిమాణం(*sgs));

	/* Assume that task can't fit any CPU of the group */
	ఒకవేళ (sd->flags & SD_ASYM_CPUCAPACITY)
		sgs->సమూహం_misfit_task_ఎక్కించు = 1;

	for_each_cpu_and(i, sched_సమూహం_పరిధి(సమూహం), p->cpus_ptr) {
		నిర్మాణం rq *rq = cpu_rq(i);
		గుర్తులేని పూర్ణాంకం local;

		sgs->సమూహం_ఎక్కించు += cpu_ఎక్కించు_without(rq, p);
		sgs->సమూహం_util += cpu_util_without(i, p);
		sgs->సమూహం_runnable += cpu_runnable_without(rq, p);
		local = task_running_on_cpu(i, p);
		sgs->కూడిక_h_nr_running += rq->cfs.h_nr_runnable - local;

		nr_running = rq->nr_running - local;
		sgs->కూడిక_nr_running += nr_running;

		/*
		 * No need to call idle_cpu_without() if nr_running is not 0
		 */
		ఒకవేళ (!nr_running && idle_cpu_without(i, p))
			sgs->idle_cpus++;

		/* Check if task fits in the CPU */
		ఒకవేళ (sd->flags & SD_ASYM_CPUCAPACITY &&
		    sgs->సమూహం_misfit_task_ఎక్కించు &&
		    task_సరిపోతుంది_cpu(p, i))
			sgs->సమూహం_misfit_task_ఎక్కించు = 0;

	}

	sgs->సమూహం_సామర్థ్యం = సమూహం->sgc->సామర్థ్యం;

	sgs->సమూహం_బరువు = సమూహం->సమూహం_బరువు;

	sgs->సమూహం_type = సమూహం_classify(sd->imbalance_pct, సమూహం, sgs);

	/*
	 * Computing avg_load makes sense only when group is fully busy or
	 * overloaded
	 */
	ఒకవేళ (sgs->సమూహం_type == సమూహం_fully_busy ||
		sgs->సమూహం_type == సమూహం_overloaded)
		sgs->avg_ఎక్కించు = (sgs->సమూహం_ఎక్కించు * SCHED_సామర్థ్యం_SCALE) /
				sgs->సమూహం_సామర్థ్యం;
}

స్థిర bool update_pick_idlest(నిర్మాణం sched_సమూహం *idlest,
			       నిర్మాణం sg_lb_stats *idlest_sgs,
			       నిర్మాణం sched_సమూహం *సమూహం,
			       నిర్మాణం sg_lb_stats *sgs)
{
	ఒకవేళ (sgs->సమూహం_type < idlest_sgs->సమూహం_type)
		తిరిగి true;

	ఒకవేళ (sgs->సమూహం_type > idlest_sgs->సమూహం_type)
		తిరిగి false;

	/*
	 * The candidate and the current idlest group are the same type of
	 * group. Let check which one is the idlest according to the type.
	 */

	ఎంపిక (sgs->సమూహం_type) {
	స్థితి సమూహం_overloaded:
	స్థితి సమూహం_fully_busy:
		/* Select the group with lowest avg_load. */
		ఒకవేళ (idlest_sgs->avg_ఎక్కించు <= sgs->avg_ఎక్కించు)
			తిరిగి false;
		ఆపు;

	స్థితి సమూహం_llc_balance:
	స్థితి సమూహం_imbalanced:
	స్థితి సమూహం_asym_packing:
	స్థితి సమూహం_smt_balance:
		/* Those types are not used in the slow wakeup path */
		తిరిగి false;

	స్థితి సమూహం_misfit_task:
		/* Select group with the highest max capacity */
		ఒకవేళ (idlest->sgc->max_సామర్థ్యం >= సమూహం->sgc->max_సామర్థ్యం)
			తిరిగి false;
		ఆపు;

	స్థితి సమూహం_has_అదనపు:
		/* Select group with most idle CPUs */
		ఒకవేళ (idlest_sgs->idle_cpus > sgs->idle_cpus)
			తిరిగి false;

		/* Select group with lowest group_util */
		ఒకవేళ (idlest_sgs->idle_cpus == sgs->idle_cpus &&
			idlest_sgs->సమూహం_util <= sgs->సమూహం_util)
			తిరిగి false;

		ఆపు;
	}

	తిరిగి true;
}

/*
 * sched_balance_find_dst_group() finds and returns the least busy CPU group within the
 * domain.
 *
 * Assumes p is allowed on at least one CPU in sd.
 */
స్థిర నిర్మాణం sched_సమూహం *
sched_balance_కనుగొను_dst_సమూహం(నిర్మాణం sched_domain *sd, నిర్మాణం task_struct *p, పూర్ణాంకం this_cpu)
{
	నిర్మాణం sched_సమూహం *idlest = ఖాళీ, *local = ఖాళీ, *సమూహం = sd->groups;
	నిర్మాణం sg_lb_stats local_sgs, tmp_sgs;
	నిర్మాణం sg_lb_stats *sgs;
	గుర్తులేని దీర్ఘ imbalance;
	నిర్మాణం sg_lb_stats idlest_sgs = {
			.avg_ఎక్కించు = UINT_MAX,
			.సమూహం_type = సమూహం_overloaded,
	};

	చేయి {
		పూర్ణాంకం local_సమూహం;

		/* Skip over this group if it has no CPUs allowed */
		ఒకవేళ (!cpumask_intersects(sched_సమూహం_పరిధి(సమూహం),
					p->cpus_ptr))
			కొనసాగు;

		/* Skip over this group if no cookie matched */
		ఒకవేళ (!sched_సమూహం_cookie_సరిపోలిక(cpu_rq(this_cpu), p, సమూహం))
			కొనసాగు;

		local_సమూహం = cpumask_test_cpu(this_cpu,
					       sched_సమూహం_పరిధి(సమూహం));

		ఒకవేళ (local_సమూహం) {
			sgs = &local_sgs;
			local = సమూహం;
		} లేకపోతే {
			sgs = &tmp_sgs;
		}

		update_sg_wakeup_stats(sd, సమూహం, sgs, p);

		ఒకవేళ (!local_సమూహం && update_pick_idlest(idlest, &idlest_sgs, సమూహం, sgs)) {
			idlest = సమూహం;
			idlest_sgs = *sgs;
		}

	} అయితే (సమూహం = సమూహం->తదుపరి, సమూహం != sd->groups);


	/* There is no idlest group to push tasks to */
	ఒకవేళ (!idlest)
		తిరిగి ఖాళీ;

	/* The local group has been skipped because of CPU affinity */
	ఒకవేళ (!local)
		తిరిగి idlest;

	/*
	 * If the local group is idler than the selected idlest group
	 * don't try and push the task.
	 */
	ఒకవేళ (local_sgs.సమూహం_type < idlest_sgs.సమూహం_type)
		తిరిగి ఖాళీ;

	/*
	 * If the local group is busier than the selected idlest group
	 * try and push the task.
	 */
	ఒకవేళ (local_sgs.సమూహం_type > idlest_sgs.సమూహం_type)
		తిరిగి idlest;

	ఎంపిక (local_sgs.సమూహం_type) {
	స్థితి సమూహం_overloaded:
	స్థితి సమూహం_fully_busy:

		/* Calculate allowed imbalance based on load */
		imbalance = scale_ఎక్కించు_కిందికి(NICE_0_ఎక్కించు) *
				(sd->imbalance_pct-100) / 100;

		/*
		 * When comparing groups across NUMA domains, it's possible for
		 * the local domain to be very lightly loaded relative to the
		 * remote domains but "imbalance" skews the comparison making
		 * remote CPUs look much more favourable. When considering
		 * cross-domain, add imbalance to the load on the remote node
		 * and consider staying local.
		 */

		ఒకవేళ ((sd->flags & SD_NUMA) &&
		    ((idlest_sgs.avg_ఎక్కించు + imbalance) >= local_sgs.avg_ఎక్కించు))
			తిరిగి ఖాళీ;

		/*
		 * If the local group is less loaded than the selected
		 * idlest group don't try and push any tasks.
		 */
		ఒకవేళ (idlest_sgs.avg_ఎక్కించు >= (local_sgs.avg_ఎక్కించు + imbalance))
			తిరిగి ఖాళీ;

		ఒకవేళ (100 * local_sgs.avg_ఎక్కించు <= sd->imbalance_pct * idlest_sgs.avg_ఎక్కించు)
			తిరిగి ఖాళీ;
		ఆపు;

	స్థితి సమూహం_llc_balance:
	స్థితి సమూహం_imbalanced:
	స్థితి సమూహం_asym_packing:
	స్థితి సమూహం_smt_balance:
		/* Those type are not used in the slow wakeup path */
		తిరిగి ఖాళీ;

	స్థితి సమూహం_misfit_task:
		/* Select group with the highest max capacity */
		ఒకవేళ (local->sgc->max_సామర్థ్యం >= idlest->sgc->max_సామర్థ్యం)
			తిరిగి ఖాళీ;
		ఆపు;

	స్థితి సమూహం_has_అదనపు:
#నిర్వచితమైతే CONFIG_NUMA
		ఒకవేళ (sd->flags & SD_NUMA) {
			పూర్ణాంకం imb_numa_nr = sd->imb_numa_nr;
#నిర్వచితమైతే CONFIG_NUMA_BALANCING
			పూర్ణాంకం idlest_cpu;
			/*
			 * If there is spare capacity at NUMA, try to select
			 * the preferred node
			 */
			ఒకవేళ (cpu_గమ్యం_కణుపు(this_cpu) == p->numa_preferred_nid)
				తిరిగి ఖాళీ;

			idlest_cpu = cpumask_మొదటి(sched_సమూహం_పరిధి(idlest));
			ఒకవేళ (cpu_గమ్యం_కణుపు(idlest_cpu) == p->numa_preferred_nid)
				తిరిగి idlest;
#అయితే_ముగింపు /* CONFIG_NUMA_BALANCING */
			/*
			 * Otherwise, keep the task close to the wakeup source
			 * and improve locality if the number of running tasks
			 * would remain below threshold where an imbalance is
			 * allowed while accounting for the possibility the
			 * task is pinned to a subset of CPUs. If there is a
			 * real need of migration, periodic load balance will
			 * take care of it.
			 */
			ఒకవేళ (p->nr_cpus_allowed != NR_CPUS) {
				గుర్తులేని పూర్ణాంకం w = cpumask_బరువు_and(p->cpus_ptr,
								sched_సమూహం_పరిధి(local));
				imb_numa_nr = min(w, sd->imb_numa_nr);
			}

			imbalance = నిరపేక్ష(local_sgs.idle_cpus - idlest_sgs.idle_cpus);
			ఒకవేళ (!adjust_numa_imbalance(imbalance,
						   local_sgs.కూడిక_nr_running + 1,
						   imb_numa_nr)) {
				తిరిగి ఖాళీ;
			}
		}
#అయితే_ముగింపు /* CONFIG_NUMA */

		/*
		 * Select group with highest number of idle CPUs. We could also
		 * compare the utilization which is more stable but it can end
		 * up that the group has less spare capacity but finally more
		 * idle CPUs which means more opportunity to run task.
		 */
		ఒకవేళ (local_sgs.idle_cpus >= idlest_sgs.idle_cpus)
			తిరిగి ఖాళీ;
		ఆపు;
	}

	తిరిగి idlest;
}

స్థిర శూన్య update_idle_cpu_పరిశీలన(నిర్మాణం lb_env *env,
				 గుర్తులేని దీర్ఘ కూడిక_util)
{
	నిర్మాణం sched_domain_shared *sd_share;
	నిర్మాణం sched_domain *sd = env->sd;
	పూర్ణాంకం llc_బరువు, pct;
	u64 x, y, tmp;
	/*
	 * Update the number of CPUs to scan in LLC domain, which could
	 * be used as a hint in select_idle_cpu(). The update of sd_share
	 * could be expensive because it is within a shared cache line.
	 * So the write of this hint only occurs during periodic load
	 * balancing, rather than CPU_NEWLY_IDLE, because the latter
	 * can fire way more frequently than the former.
	 */
	ఒకవేళ (!sched_feat(SIS_UTIL) || env->idle == CPU_NEWLY_IDLE)
		తిరిగి;

	sd_share = sd->shared;
	ఒకవేళ (!sd_share)
		తిరిగి;

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
	x = కూడిక_util;
	llc_బరువు = sd->పరిధి_బరువు;
	do_div(x, llc_బరువు);

	/* equation [4] */
	pct = sd->imbalance_pct;
	tmp = x * x * pct * pct;
	do_div(tmp, 10000 * SCHED_సామర్థ్యం_SCALE);
	tmp = min_t(దీర్ఘ, tmp, SCHED_సామర్థ్యం_SCALE);
	y = SCHED_సామర్థ్యం_SCALE - tmp;

	/* equation [2] */
	y *= llc_బరువు;
	do_div(y, SCHED_సామర్థ్యం_SCALE);
	ఒకవేళ ((పూర్ణాంకం)y != sd_share->nr_idle_పరిశీలన)
		WRITE_ONCE(sd_share->nr_idle_పరిశీలన, (పూర్ణాంకం)y);
}

/**
 * update_sd_lb_stats - Update sched_domain's statistics for load balancing.
 * @env: The load balancing environment.
 * @sds: variable to hold the statistics for this sched_domain.
 */

స్థిర అంతఃవరుస శూన్య update_sd_lb_stats(నిర్మాణం lb_env *env, నిర్మాణం sd_lb_stats *sds)
{
	నిర్మాణం sched_సమూహం *sg = env->sd->groups;
	నిర్మాణం sg_lb_stats *local = &sds->local_stat;
	నిర్మాణం sg_lb_stats tmp_sgs;
	గుర్తులేని దీర్ఘ కూడిక_util = 0;
	bool sg_overloaded = 0, sg_overutilized = 0;

	env->dst_core_idle = !sched_smt_చురుకైన() || is_core_idle(env->dst_cpu);

	చేయి {
		నిర్మాణం sg_lb_stats *sgs = &tmp_sgs;
		పూర్ణాంకం local_సమూహం;

		local_సమూహం = cpumask_test_cpu(env->dst_cpu, sched_సమూహం_పరిధి(sg));
		ఒకవేళ (local_సమూహం) {
			sds->local = sg;
			sgs = local;

			ఒకవేళ (env->idle != CPU_NEWLY_IDLE ||
			    సమయం_after_eq(jiffies, sg->sgc->తదుపరి_update))
				update_సమూహం_సామర్థ్యం(env->sd, env->dst_cpu);
		}

		update_sg_lb_stats(env, sds, sg, sgs, &sg_overloaded);

		ఒకవేళ (!local_సమూహం && update_sd_pick_busiest(env, sds, sg, sgs)) {
			sds->busiest = sg;
			sds->busiest_stat = *sgs;
		}

		sg_overutilized |= sgs->సమూహం_overutilized;

		/* Now, start updating sd_lb_stats */
		sds->మొత్తం_ఎక్కించు += sgs->సమూహం_ఎక్కించు;
		sds->మొత్తం_సామర్థ్యం += sgs->సమూహం_సామర్థ్యం;

		కూడిక_util += sgs->సమూహం_util;
		sg = sg->తదుపరి;
	} అయితే (sg != env->sd->groups);

	/*
	 * Indicate that the child domain of the busiest group prefers tasks
	 * go to a child's sibling domains first. NB the flags of a sched group
	 * are those of the child domain.
	 */
	ఒకవేళ (sds->busiest)
		sds->prefer_sibling = !!(sds->busiest->flags & SD_PREFER_SIBLING);


	ఒకవేళ (env->sd->flags & SD_NUMA)
		env->fbq_type = fbq_classify_సమూహం(&sds->busiest_stat);

	ఒకవేళ (!env->sd->మాతృ) {
		/* update overload indicator if we are at root domain */
		సమితి_rd_overloaded(env->dst_rq->rd, sg_overloaded);

		/* Update over-utilization (tipping point, U >= 0) indicator */
		సమితి_rd_overutilized(env->dst_rq->rd, sg_overutilized);
	} లేకపోతే ఒకవేళ (sg_overutilized) {
		సమితి_rd_overutilized(env->dst_rq->rd, sg_overutilized);
	}

	update_idle_cpu_పరిశీలన(env, కూడిక_util);
}

/**
 * calculate_imbalance - Calculate the amount of imbalance present within the
 *			 groups of a given sched_domain during load balance.
 * @env: load balance environment
 * @sds: statistics of the sched_domain whose imbalance is to be calculated.
 */
స్థిర అంతఃవరుస శూన్య calculate_imbalance(నిర్మాణం lb_env *env, నిర్మాణం sd_lb_stats *sds)
{
	నిర్మాణం sg_lb_stats *local, *busiest;

	local = &sds->local_stat;
	busiest = &sds->busiest_stat;

	ఒకవేళ (busiest->సమూహం_type == సమూహం_misfit_task) {
		ఒకవేళ (env->sd->flags & SD_ASYM_CPUCAPACITY) {
			/* Set imbalance to allow misfit tasks to be balanced. */
			env->migration_type = migrate_misfit;
			env->imbalance = 1;
		} లేకపోతే {
			/*
			 * Set load imbalance to allow moving task from cpu
			 * with reduced capacity.
			 */
			env->migration_type = migrate_ఎక్కించు;
			env->imbalance = busiest->సమూహం_misfit_task_ఎక్కించు;
		}
		తిరిగి;
	}

	ఒకవేళ (busiest->సమూహం_type == సమూహం_asym_packing) {
		/*
		 * In case of asym capacity, we will try to migrate all load to
		 * the preferred CPU.
		 */
		env->migration_type = migrate_task;
		env->imbalance = busiest->కూడిక_h_nr_running;
		తిరిగి;
	}

	ఒకవేళ (busiest->సమూహం_type == సమూహం_smt_balance) {
		/* Reduce number of tasks sharing CPU capacity */
		env->migration_type = migrate_task;
		env->imbalance = 1;
		తిరిగి;
	}

#నిర్వచితమైతే CONFIG_SCHED_CACHE
	ఒకవేళ (busiest->సమూహం_type == సమూహం_llc_balance) {
		/* Move a task that prefer local LLC */
		env->migration_type = migrate_llc_task;
		env->imbalance = 1;
		తిరిగి;
	}
#అయితే_ముగింపు

	ఒకవేళ (busiest->సమూహం_type == సమూహం_imbalanced) {
		/*
		 * In the group_imb case we cannot rely on group-wide averages
		 * to ensure CPU-load equilibrium, try to move any task to fix
		 * the imbalance. The next load balance will take care of
		 * balancing back the system.
		 */
		env->migration_type = migrate_task;
		env->imbalance = 1;
		తిరిగి;
	}

	/*
	 * Try to use spare capacity of local group without overloading it or
	 * emptying busiest.
	 */
	ఒకవేళ (local->సమూహం_type == సమూహం_has_అదనపు) {
		ఒకవేళ ((busiest->సమూహం_type > సమూహం_fully_busy) &&
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
			env->imbalance = max(local->సమూహం_సామర్థ్యం, local->సమూహం_util) -
					 local->సమూహం_util;

			/*
			 * In some cases, the group's utilization is max or even
			 * higher than capacity because of migrations but the
			 * local CPU is (newly) idle. There is at least one
			 * waiting task in this overloaded busiest group. Let's
			 * try to pull it.
			 */
			ఒకవేళ (env->idle && env->imbalance == 0) {
				env->migration_type = migrate_task;
				env->imbalance = 1;
			}

			తిరిగి;
		}

		ఒకవేళ (busiest->సమూహం_బరువు == 1 || sds->prefer_sibling) {
			/*
			 * When prefer sibling, evenly spread running tasks on
			 * groups.
			 */
			env->migration_type = migrate_task;
			env->imbalance = sibling_imbalance(env, sds, busiest, local);
		} లేకపోతే {

			/*
			 * If there is no overload, we just want to even the number of
			 * idle CPUs.
			 */
			env->migration_type = migrate_task;
			env->imbalance = max_t(దీర్ఘ, 0,
					       (local->idle_cpus - busiest->idle_cpus));
		}

#నిర్వచితమైతే CONFIG_NUMA
		/* Consider allowing a small imbalance between NUMA groups */
		ఒకవేళ (env->sd->flags & SD_NUMA) {
			env->imbalance = adjust_numa_imbalance(env->imbalance,
							       local->కూడిక_nr_running + 1,
							       env->sd->imb_numa_nr);
		}
#అయితే_ముగింపు

		/* Number of tasks to move to restore balance */
		env->imbalance >>= 1;

		తిరిగి;
	}

	/*
	 * Local is fully busy but has to take more load to relieve the
	 * busiest group
	 */
	ఒకవేళ (local->సమూహం_type < సమూహం_overloaded) {
		/*
		 * Local will become overloaded so the avg_load metrics are
		 * finally needed.
		 */

		local->avg_ఎక్కించు = (local->సమూహం_ఎక్కించు * SCHED_సామర్థ్యం_SCALE) /
				  local->సమూహం_సామర్థ్యం;

		/*
		 * If the local group is more loaded than the selected
		 * busiest group don't try to pull any tasks.
		 */
		ఒకవేళ (local->avg_ఎక్కించు >= busiest->avg_ఎక్కించు) {
			env->imbalance = 0;
			తిరిగి;
		}

		sds->avg_ఎక్కించు = (sds->మొత్తం_ఎక్కించు * SCHED_సామర్థ్యం_SCALE) /
				sds->మొత్తం_సామర్థ్యం;

		/*
		 * If the local group is more loaded than the average system
		 * load, don't try to pull any tasks.
		 */
		ఒకవేళ (local->avg_ఎక్కించు >= sds->avg_ఎక్కించు) {
			env->imbalance = 0;
			తిరిగి;
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
	env->migration_type = migrate_ఎక్కించు;
	env->imbalance = min(
		(busiest->avg_ఎక్కించు - sds->avg_ఎక్కించు) * busiest->సమూహం_సామర్థ్యం,
		(sds->avg_ఎక్కించు - local->avg_ఎక్కించు) * local->సమూహం_సామర్థ్యం
	) / SCHED_సామర్థ్యం_SCALE;
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
స్థిర నిర్మాణం sched_సమూహం *sched_balance_కనుగొను_src_సమూహం(నిర్మాణం lb_env *env)
{
	నిర్మాణం sg_lb_stats *local, *busiest;
	నిర్మాణం sd_lb_stats sds;

	init_sd_lb_stats(&sds);

	/*
	 * Compute the various statistics relevant for load balancing at
	 * this level.
	 */
	update_sd_lb_stats(env, &sds);

	/* There is no busy sibling group to pull tasks from */
	ఒకవేళ (!sds.busiest)
		వెళ్ళు బహిర్గతం_సమతుల్య;

	busiest = &sds.busiest_stat;

	/* Misfit tasks should be dealt with regardless of the avg load */
	ఒకవేళ (busiest->సమూహం_type == సమూహం_misfit_task)
		వెళ్ళు force_balance;

	ఒకవేళ (!is_rd_overutilized(env->dst_rq->rd) &&
	    rcu_dereference_all(env->dst_rq->rd->pd))
		వెళ్ళు బహిర్గతం_సమతుల్య;

	/* ASYM feature bypasses nice load balance check */
	ఒకవేళ (busiest->సమూహం_type == సమూహం_asym_packing)
		వెళ్ళు force_balance;

	/*
	 * If the busiest group is imbalanced the below checks don't
	 * work because they assume all things are equal, which typically
	 * isn't true due to cpus_ptr constraints and the like.
	 */
	ఒకవేళ (busiest->సమూహం_type == సమూహం_imbalanced)
		వెళ్ళు force_balance;

	local = &sds.local_stat;
	/*
	 * If the local group is busier than the selected busiest group
	 * don't try and pull any tasks.
	 */
	ఒకవేళ (local->సమూహం_type > busiest->సమూహం_type)
		వెళ్ళు బహిర్గతం_సమతుల్య;

	/*
	 * When groups are overloaded, use the avg_load to ensure fairness
	 * between tasks.
	 */
	ఒకవేళ (local->సమూహం_type == సమూహం_overloaded) {
		/*
		 * If the local group is more loaded than the selected
		 * busiest group don't try to pull any tasks.
		 */
		ఒకవేళ (local->avg_ఎక్కించు >= busiest->avg_ఎక్కించు)
			వెళ్ళు బహిర్గతం_సమతుల్య;

		/* XXX broken for overlapping NUMA groups */
		sds.avg_ఎక్కించు = (sds.మొత్తం_ఎక్కించు * SCHED_సామర్థ్యం_SCALE) /
				sds.మొత్తం_సామర్థ్యం;

		/*
		 * Don't pull any tasks if this group is already above the
		 * domain average load.
		 */
		ఒకవేళ (local->avg_ఎక్కించు >= sds.avg_ఎక్కించు)
			వెళ్ళు బహిర్గతం_సమతుల్య;

		/*
		 * If the busiest group is more loaded, use imbalance_pct to be
		 * conservative.
		 */
		ఒకవేళ (100 * busiest->avg_ఎక్కించు <=
				env->sd->imbalance_pct * local->avg_ఎక్కించు)
			వెళ్ళు బహిర్గతం_సమతుల్య;
	}

	/*
	 * Try to move all excess tasks to a sibling domain of the busiest
	 * group's child domain.
	 */
	ఒకవేళ (sds.prefer_sibling && local->సమూహం_type == సమూహం_has_అదనపు &&
	    (busiest->సమూహం_type == సమూహం_llc_balance ||
	    sibling_imbalance(env, &sds, busiest, local) > 1))
		వెళ్ళు force_balance;

	ఒకవేళ (busiest->సమూహం_type != సమూహం_overloaded) {
		ఒకవేళ (!env->idle) {
			/*
			 * If the busiest group is not overloaded (and as a
			 * result the local one too) but this CPU is already
			 * busy, let another idle CPU try to pull task.
			 */
			వెళ్ళు బహిర్గతం_సమతుల్య;
		}

		ఒకవేళ (busiest->సమూహం_type == సమూహం_smt_balance &&
		    smt_vs_nonsmt_groups(sds.local, sds.busiest)) {
			/* Let non SMT CPU pull from SMT CPU sharing with sibling */
			వెళ్ళు force_balance;
		}

		ఒకవేళ (busiest->సమూహం_బరువు > 1 &&
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
			వెళ్ళు బహిర్గతం_సమతుల్య;
		}

		ఒకవేళ (busiest->కూడిక_h_nr_running == 1) {
			/*
			 * busiest doesn't have any tasks waiting to run
			 */
			వెళ్ళు బహిర్గతం_సమతుల్య;
		}
	}

force_balance:
	/* Looks like there is an imbalance. Compute it */
	calculate_imbalance(env, &sds);
	తిరిగి env->imbalance ? sds.busiest : ఖాళీ;

బహిర్గతం_సమతుల్య:
	env->imbalance = 0;
	తిరిగి ఖాళీ;
}

/*
 * sched_balance_find_src_rq - find the busiest runqueue among the CPUs in the group.
 */
స్థిర నిర్మాణం rq *sched_balance_కనుగొను_src_rq(నిర్మాణం lb_env *env,
				     నిర్మాణం sched_సమూహం *సమూహం)
{
	నిర్మాణం rq *busiest = ఖాళీ, *rq;
	గుర్తులేని దీర్ఘ busiest_util = 0, busiest_ఎక్కించు = 0, busiest_సామర్థ్యం = 1;
	గుర్తులేని పూర్ణాంకం __maybe_unused busiest_pref_llc = 0;
	నిర్మాణం sched_domain __maybe_unused *sd_tmp;
	గుర్తులేని పూర్ణాంకం busiest_nr = 0;
	పూర్ణాంకం __maybe_unused dst_llc;
	పూర్ణాంకం i;

	for_each_cpu_and(i, sched_సమూహం_పరిధి(సమూహం), env->cpus) {
		గుర్తులేని దీర్ఘ సామర్థ్యం, ఎక్కించు, util;
		గుర్తులేని పూర్ణాంకం nr_running;
		గణన fbq_type rt;

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
		ఒకవేళ (rt > env->fbq_type)
			కొనసాగు;

		nr_running = rq->cfs.h_nr_runnable;
		ఒకవేళ (!nr_running)
			కొనసాగు;

		సామర్థ్యం = సామర్థ్యం_యొక్క(i);

		/*
		 * For ASYM_CPUCAPACITY domains, don't pick a CPU that could
		 * eventually lead to active_balancing high->low capacity.
		 * Higher per-CPU capacity is considered better than balancing
		 * average load.
		 */
		ఒకవేళ (env->sd->flags & SD_ASYM_CPUCAPACITY &&
		    nr_running == 1) {
			bool cluster_equal_cap = static_branch_unlikely(&sched_cluster_చురుకైన) &&
						 (get_actual_cpu_సామర్థ్యం(env->dst_cpu) ==
						  get_actual_cpu_సామర్థ్యం(i));
			bool smt_degraded_cap = sched_smt_చురుకైన() && !is_core_idle(i);

			/*
			 * Busy SMT siblings reduce the capacity of CPU @i. Do
			 * not skip it in this case.
			 *
			 * CONFIG_SCHED_CLUSTER requires balancing load across
			 * clusters of identical capacity, accounting for
			 * hardware and cpufreq pressure.
			 */
			ఒకవేళ (!smt_degraded_cap && !cluster_equal_cap &&
			    !సామర్థ్యం_greater(సామర్థ్యం_యొక్క(env->dst_cpu), సామర్థ్యం))
				కొనసాగు;
		}

		/*
		 * Make sure we only pull tasks from a CPU of lower priority
		 * when balancing between SMT siblings.
		 *
		 * If balancing between cores, let lower priority CPUs help
		 * SMT cores with more than one busy sibling.
		 */
		ఒకవేళ (sched_asym(env->sd, i, env->dst_cpu) && nr_running == 1)
			కొనసాగు;

		ఎంపిక (env->migration_type) {
		స్థితి migrate_ఎక్కించు:
			/*
			 * When comparing with load imbalance, use cpu_load()
			 * which is not scaled with the CPU capacity.
			 */
			ఎక్కించు = cpu_ఎక్కించు(rq);

			ఒకవేళ (nr_running == 1 && ఎక్కించు > env->imbalance &&
			    !check_cpu_సామర్థ్యం(rq, env->sd))
				ఆపు;

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
			ఒకవేళ (ఎక్కించు * busiest_సామర్థ్యం > busiest_ఎక్కించు * సామర్థ్యం) {
				busiest_ఎక్కించు = ఎక్కించు;
				busiest_సామర్థ్యం = సామర్థ్యం;
				busiest = rq;
			}
			ఆపు;

		స్థితి migrate_util:
			util = cpu_util_cfs_boost(i);

			/*
			 * Don't try to pull utilization from a CPU with one
			 * running task. Whatever its utilization, we will fail
			 * detach the task.
			 */
			ఒకవేళ (nr_running <= 1)
				కొనసాగు;

			ఒకవేళ (busiest_util < util) {
				busiest_util = util;
				busiest = rq;
			}
			ఆపు;

		స్థితి migrate_task:
			ఒకవేళ (busiest_nr < nr_running) {
				busiest_nr = nr_running;
				busiest = rq;
			}
			ఆపు;

		స్థితి migrate_misfit:
			/*
			 * For ASYM_CPUCAPACITY domains with misfit tasks we
			 * simply seek the "biggest" misfit task.
			 */
			ఒకవేళ (rq->misfit_task_ఎక్కించు > busiest_ఎక్కించు) {
				busiest_ఎక్కించు = rq->misfit_task_ఎక్కించు;
				busiest = rq;
			}

			ఆపు;

		స్థితి migrate_llc_task:
#నిర్వచితమైతే CONFIG_SCHED_CACHE
			sd_tmp = rcu_dereference_all(rq->sd);
			dst_llc = llc_id(env->dst_cpu);

			ఒకవేళ (sd_tmp && (గుర్తులేని)dst_llc < sd_tmp->llc_max) {
				గుర్తులేని పూర్ణాంకం this_pref_llc =
					sd_tmp->llc_counts[dst_llc];

				ఒకవేళ (busiest_pref_llc < this_pref_llc) {
					busiest_pref_llc = this_pref_llc;
					busiest = rq;
				}
			}
#అయితే_ముగింపు
			ఆపు;

		}
	}

	తిరిగి busiest;
}

/*
 * Max backoff if we encounter pinned tasks. Pretty arbitrary value, but
 * so long as it is large enough.
 */
#నిర్వచించు MAX_PINNED_INTERVAL	512

స్థిర అంతఃవరుస bool
asym_చురుకైన_balance(నిర్మాణం lb_env *env)
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
	తిరిగి env->idle && sched_use_asym_prio(env->sd, env->dst_cpu) &&
	       (sched_asym_prefer(env->dst_cpu, env->src_cpu) ||
		!sched_use_asym_prio(env->sd, env->src_cpu));
}

స్థిర అంతఃవరుస bool
imbalanced_చురుకైన_balance(నిర్మాణం lb_env *env)
{
	నిర్మాణం sched_domain *sd = env->sd;

	/*
	 * The imbalanced case includes the case of pinned tasks preventing a fair
	 * distribution of the load on the system but also the even distribution of the
	 * threads on a system with spare capacity
	 */
	ఒకవేళ ((env->migration_type == migrate_task) &&
	    (sd->nr_balance_failed > sd->cache_nice_tries+2))
		తిరిగి 1;

	తిరిగి 0;
}

స్థిర పూర్ణాంకం need_చురుకైన_balance(నిర్మాణం lb_env *env)
{
	నిర్మాణం sched_domain *sd = env->sd;

	ఒకవేళ (alb_break_llc(env))
		తిరిగి 0;

	ఒకవేళ (asym_చురుకైన_balance(env))
		తిరిగి 1;

	ఒకవేళ (imbalanced_చురుకైన_balance(env))
		తిరిగి 1;

	/*
	 * The dst_cpu is idle and the src_cpu CPU has only 1 CFS task.
	 * It's worth migrating the task if the src_cpu's capacity is reduced
	 * because of other sched_class or IRQs if more capacity stays
	 * available on dst_cpu.
	 */
	ఒకవేళ (env->idle &&
	    (env->src_rq->cfs.h_nr_runnable == 1)) {
		ఒకవేళ ((check_cpu_సామర్థ్యం(env->src_rq, sd)) &&
		    (సామర్థ్యం_యొక్క(env->src_cpu)*sd->imbalance_pct < సామర్థ్యం_యొక్క(env->dst_cpu)*100))
			తిరిగి 1;
	}

	ఒకవేళ (env->migration_type == migrate_misfit ||
	    env->migration_type == migrate_llc_task)
		తిరిగి 1;

	తిరిగి 0;
}

స్థిర పూర్ణాంకం చురుకైన_ఎక్కించు_balance_cpu_stop(శూన్య *దత్తాంశం);
స్థిర పూర్ణాంకం చురుకైన_ఎక్కించు_balance_llc_cpu_stop(శూన్య *దత్తాంశం);

/*
 * migration_type is checked elsewhere to decide migration policy, so
 * it shouldn't be repurposed just to flag an LLC-directed active
 * balance across the stopper. Pick the callback here instead.
 */
స్థిర అంతఃవరుస cpu_stop_fn_t alb_stop_fn(నిర్మాణం lb_env *env)
{
	ఒకవేళ (env->migration_type == migrate_llc_task)
		తిరిగి చురుకైన_ఎక్కించు_balance_llc_cpu_stop;

	తిరిగి చురుకైన_ఎక్కించు_balance_cpu_stop;
}

స్థిర పూర్ణాంకం should_we_balance(నిర్మాణం lb_env *env)
{
	నిర్మాణం cpumask *swb_cpus = this_cpu_cpumask_var_ptr(should_we_balance_tmpmask);
	నిర్మాణం sched_సమూహం *sg = env->sd->groups;
	పూర్ణాంకం cpu, idle_smt = -1;

	/*
	 * Ensure the balancing environment is consistent; can happen
	 * when the softirq triggers 'during' hotplug.
	 */
	ఒకవేళ (!cpumask_test_cpu(env->dst_cpu, env->cpus))
		తిరిగి 0;

	/*
	 * In the newly idle case, we will allow all the CPUs
	 * to do the newly idle load balance.
	 *
	 * However, we bail out if we already have tasks or a wakeup pending,
	 * to optimize wakeup latency.
	 */
	ఒకవేళ (env->idle == CPU_NEWLY_IDLE) {
		ఒకవేళ (env->dst_rq->nr_running > 0 || env->dst_rq->ttwu_pending)
			తిరిగి 0;
		తిరిగి 1;
	}

	cpumask_copy(swb_cpus, సమూహం_balance_ముసుగు(sg));
	/* Try to find first idle CPU */
	for_each_cpu_and(cpu, swb_cpus, env->cpus) {
		ఒకవేళ (!idle_cpu(cpu))
			కొనసాగు;

		/*
		 * Don't balance to idle SMT in busy core right away when
		 * balancing cores, but remember the first idle SMT CPU for
		 * later consideration.  Find CPU on an idle core first.
		 */
		ఒకవేళ (sched_smt_చురుకైన() &&
		    !(env->sd->flags & SD_SHARE_CPUCAPACITY) &&
		    !is_core_idle(cpu)) {
			ఒకవేళ (idle_smt == -1)
				idle_smt = cpu;
			/*
			 * If the core is not idle, and first SMT sibling which is
			 * idle has been found, then its not needed to check other
			 * SMT siblings for idleness:
			 */
			cpumask_andnot(swb_cpus, swb_cpus, cpu_smt_ముసుగు(cpu));
			కొనసాగు;
		}

		/*
		 * Are we the first idle core in a non-SMT domain or higher,
		 * or the first idle CPU in a SMT domain?
		 */
		తిరిగి cpu == env->dst_cpu;
	}

	/* Are we the first idle CPU with busy siblings? */
	ఒకవేళ (idle_smt != -1)
		తిరిగి idle_smt == env->dst_cpu;

	/* Are we the first CPU of this group ? */
	తిరిగి సమూహం_balance_cpu(sg) == env->dst_cpu;
}

స్థిర శూన్య update_lb_imbalance_stat(నిర్మాణం lb_env *env, నిర్మాణం sched_domain *sd,
				     గణన cpu_idle_type idle)
{
	ఒకవేళ (!schedstat_enabled())
		తిరిగి;

	ఎంపిక (env->migration_type) {
	స్థితి migrate_ఎక్కించు:
		__schedstat_add(sd->lb_imbalance_ఎక్కించు[idle], env->imbalance);
		ఆపు;
	స్థితి migrate_util:
		__schedstat_add(sd->lb_imbalance_util[idle], env->imbalance);
		ఆపు;
	స్థితి migrate_task:
		__schedstat_add(sd->lb_imbalance_task[idle], env->imbalance);
		ఆపు;
	స్థితి migrate_misfit:
		__schedstat_add(sd->lb_imbalance_misfit[idle], env->imbalance);
		ఆపు;
	స్థితి migrate_llc_task:
		ఆపు;
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
స్థిర atomic_t sched_balance_running = ATOMIC_INIT(0);

/*
 * Check this_cpu to ensure it is balanced within domain. Attempt to move
 * tasks if there is an imbalance.
 */
స్థిర పూర్ణాంకం sched_balance_rq(పూర్ణాంకం this_cpu, నిర్మాణం rq *this_rq,
			నిర్మాణం sched_domain *sd, గణన cpu_idle_type idle,
			పూర్ణాంకం *continue_balancing)
{
	పూర్ణాంకం ld_moved, cur_ld_moved, చురుకైన_balance = 0;
	నిర్మాణం sched_domain *sd_మాతృ = sd->మాతృ;
	నిర్మాణం sched_సమూహం *సమూహం;
	నిర్మాణం rq *busiest;
	నిర్మాణం rq_flags rf;
	నిర్మాణం cpumask *cpus = this_cpu_cpumask_var_ptr(ఎక్కించు_balance_ముసుగు);
	నిర్మాణం lb_env env = {
		.sd		= sd,
		.dst_cpu	= this_cpu,
		.dst_rq		= this_rq,
		.dst_grpmask    = సమూహం_balance_ముసుగు(sd->groups),
		.idle		= idle,
		.loop_break	= SCHED_NR_MIGRATE_BREAK,
		.cpus		= cpus,
		.fbq_type	= all,
		.tasks		= LIST_తల_INIT(env.tasks),
	};
	bool need_unlock = false;

	cpumask_and(cpus, sched_domain_పరిధి(sd), cpu_preferred_ముసుగు);

	schedstat_inc(sd->lb_లెక్క[idle]);

redo:
	ఒకవేళ (!should_we_balance(&env)) {
		*continue_balancing = 0;
		వెళ్ళు బహిర్గతం_సమతుల్య;
	}

	ఒకవేళ (!need_unlock && (sd->flags & SD_SERIALIZE)) {
		పూర్ణాంకం zero = 0;
		ఒకవేళ (!atomic_try_cmpxchg_acquire(&sched_balance_running, &zero, 1))
			వెళ్ళు బహిర్గతం_సమతుల్య;

		need_unlock = true;
	}

	సమూహం = sched_balance_కనుగొను_src_సమూహం(&env);
	ఒకవేళ (!సమూహం) {
		schedstat_inc(sd->lb_nobusyg[idle]);
		వెళ్ళు బహిర్గతం_సమతుల్య;
	}

	busiest = sched_balance_కనుగొను_src_rq(&env, సమూహం);
	ఒకవేళ (!busiest) {
		schedstat_inc(sd->lb_nobusyq[idle]);
		వెళ్ళు బహిర్గతం_సమతుల్య;
	}

	WARN_ON_ONCE(busiest == env.dst_rq);

	update_lb_imbalance_stat(&env, sd, idle);

	env.src_cpu = busiest->cpu;
	env.src_rq = busiest;

	ld_moved = 0;
	/* Clear this flag as soon as we find a pullable task */
	env.flags |= LBF_ALL_PINNED;
	ఒకవేళ (busiest->nr_running > 1) {
		/*
		 * Attempt to move tasks. If sched_balance_find_src_group has found
		 * an imbalance but busiest->nr_running <= 1, the group is
		 * still unbalanced. ld_moved simply stays zero, so it is
		 * correctly treated as an imbalance.
		 */
		env.loop_max  = min(sysctl_sched_nr_migrate, busiest->nr_running);

మరిన్ని_balance:
		rq_తాళం_irqsave(busiest, &rf);
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

		ఒకవేళ (cur_ld_moved) {
			attach_tasks(&env);
			ld_moved += cur_ld_moved;
		}

		local_irq_restore(rf.flags);

		ఒకవేళ (env.flags & LBF_NEED_BREAK) {
			env.flags &= ~LBF_NEED_BREAK;
			వెళ్ళు మరిన్ని_balance;
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
		ఒకవేళ ((env.flags & LBF_DST_PINNED) && env.imbalance > 0) {

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
			వెళ్ళు మరిన్ని_balance;
		}

		/*
		 * We failed to reach balance because of affinity.
		 */
		ఒకవేళ (sd_మాతృ) {
			పూర్ణాంకం *సమూహం_imbalance = &sd_మాతృ->groups->sgc->imbalance;

			ఒకవేళ ((env.flags & LBF_SOME_PINNED) && env.imbalance > 0)
				*సమూహం_imbalance = 1;
		}

		/* All tasks on this runqueue were pinned by CPU affinity */
		ఒకవేళ (unlikely(env.flags & LBF_ALL_PINNED)) {
			__cpumask_clear_cpu(cpu_యొక్క(busiest), cpus);
			/*
			 * Attempting to continue load balancing at the current
			 * sched_domain level only makes sense if there are
			 * active CPUs remaining as possible busiest CPUs to
			 * pull load from which are not contained within the
			 * destination group that is receiving any migrated
			 * load.
			 */
			ఒకవేళ (!cpumask_subset(cpus, env.dst_grpmask)) {
				env.loop = 0;
				env.loop_break = SCHED_NR_MIGRATE_BREAK;
				వెళ్ళు redo;
			}
			వెళ్ళు బహిర్గతం_all_pinned;
		}
	}

	ఒకవేళ (ld_moved) {
		sd->nr_balance_failed = 0;
		వెళ్ళు బహిర్గతం_unbalanced;
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
	ఒకవేళ (idle != CPU_NEWLY_IDLE &&
	    env.migration_type != migrate_misfit &&
	    !(env.flags & LBF_LLC_PINNED))
		sd->nr_balance_failed++;

	ఒకవేళ (!need_చురుకైన_balance(&env))
		వెళ్ళు బహిర్గతం_unbalanced;

	scoped_guard (raw_spin_rq_తాళం_irqsave, busiest) {
		/*
		 * Don't kick the active_load_balance_cpu_stop,
		 * if the curr task on busiest CPU can't be
		 * moved to this_cpu:
		 */
		ఒకవేళ (!cpumask_test_cpu(this_cpu, busiest->curr->cpus_ptr))
			వెళ్ళు బహిర్గతం_one_pinned;

		/* Record that we found at least one task that could run on this_cpu */
		env.flags &= ~LBF_ALL_PINNED;

		/*
		 * ->active_balance synchronizes accesses to
		 * ->active_balance_work.  Once set, it's cleared
		 * only after active load balance is finished.
		 */
		ఒకవేళ (busiest->చురుకైన_balance)
			వెళ్ళు బహిర్గతం_unbalanced;

		/*
		 * @busiest dropped its rq_lock in the middle of
		 * scheduling out its ->curr task (->on_rq := 0), no
		 * need to forcefully punt it away with active balance.
		 */
		ఒకవేళ (!busiest->curr->on_rq)
			వెళ్ళు బహిర్గతం_unbalanced;

		busiest->చురుకైన_balance = 1;
		busiest->నెట్టు_cpu = this_cpu;
		చురుకైన_balance = 1;
		preempt_disable();
	}
	ఒకవేళ (చురుకైన_balance) {
		stop_one_cpu_nowait(cpu_యొక్క(busiest),
				    alb_stop_fn(&env), busiest,
				    &busiest->చురుకైన_balance_work);
	}
	preempt_enable();

బహిర్గతం_unbalanced:
	/* We were unbalanced, so reset the balancing interval */
	sd->balance_interval = sd->min_interval;
	వెళ్ళు బహిర్గతం;

బహిర్గతం_సమతుల్య:
	/*
	 * We reach balance although we may have faced some affinity
	 * constraints. Clear the imbalance flag only if other tasks got
	 * a chance to move and fix the imbalance.
	 */
	ఒకవేళ (sd_మాతృ && !(env.flags & LBF_ALL_PINNED)) {
		పూర్ణాంకం *సమూహం_imbalance = &sd_మాతృ->groups->sgc->imbalance;

		ఒకవేళ (*సమూహం_imbalance)
			*సమూహం_imbalance = 0;
	}

బహిర్గతం_all_pinned:
	/*
	 * We reach balance because all tasks are pinned at this level so
	 * we can't migrate them. Let the imbalance flag set so parent level
	 * can try to migrate them.
	 */
	schedstat_inc(sd->lb_సమతుల్య[idle]);

	sd->nr_balance_failed = 0;

బహిర్గతం_one_pinned:
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
	ఒకవేళ (env.idle == CPU_NEWLY_IDLE ||
	    env.migration_type == migrate_misfit)
		వెళ్ళు బహిర్గతం;

	/* tune up the balancing interval */
	ఒకవేళ ((env.flags & LBF_ALL_PINNED &&
	     sd->balance_interval < MAX_PINNED_INTERVAL) ||
	    sd->balance_interval < sd->max_interval)
		sd->balance_interval *= 2;
బహిర్గతం:
	ఒకవేళ (need_unlock)
		atomic_సమితి_release(&sched_balance_running, 0);

	తిరిగి ld_moved;
}

స్థిర అంతఃవరుస గుర్తులేని దీర్ఘ
get_sd_balance_interval(నిర్మాణం sched_domain *sd, పూర్ణాంకం cpu_busy)
{
	గుర్తులేని దీర్ఘ interval = sd->balance_interval;

	ఒకవేళ (cpu_busy)
		interval *= sd->busy_గుణకం;

	/* scale ms to jiffies */
	interval = msecs_గమ్యం_jiffies(interval);

	/*
	 * Reduce likelihood of busy balancing at higher domains racing with
	 * balancing at lower domains by preventing their balancing periods
	 * from being multiples of each other.
	 */
	ఒకవేళ (cpu_busy)
		interval -= 1;

	interval = clamp(interval, 1UL, max_ఎక్కించు_balance_interval);

	తిరిగి interval;
}

స్థిర అంతఃవరుస శూన్య
update_తదుపరి_balance(నిర్మాణం sched_domain *sd, గుర్తులేని దీర్ఘ *తదుపరి_balance)
{
	గుర్తులేని దీర్ఘ interval, తదుపరి;

	/* used by idle balance, so cpu_busy = 0 */
	interval = get_sd_balance_interval(sd, 0);
	తదుపరి = sd->చివరి_balance + interval;

	ఒకవేళ (సమయం_after(*తదుపరి_balance, తదుపరి))
		*తదుపరి_balance = తదుపరి;
}

/*
 * active_load_balance_cpu_stop is run by the CPU stopper. It pushes
 * running tasks off the busiest CPU onto idle CPUs. It requires at
 * least 1 task to be running on each physical CPU where possible, and
 * avoids physical / logical imbalances.
 */
స్థిర పూర్ణాంకం __active_load_balance_cpu_stop(శూన్య *దత్తాంశం, గుర్తులేని పూర్ణాంకం lb_flags)
{
	నిర్మాణం rq *busiest_rq = దత్తాంశం;
	పూర్ణాంకం busiest_cpu = cpu_యొక్క(busiest_rq);
	పూర్ణాంకం లక్ష్యం_cpu = busiest_rq->నెట్టు_cpu;
	నిర్మాణం rq *లక్ష్యం_rq = cpu_rq(లక్ష్యం_cpu);
	నిర్మాణం sched_domain *sd;
	నిర్మాణం task_struct *p = ఖాళీ;
	నిర్మాణం rq_flags rf;

	rq_తాళం_irq(busiest_rq, &rf);
	/*
	 * Between queueing the stop-work and running it is a hole in which
	 * CPUs can become inactive. We should not move tasks from or to
	 * inactive CPUs.
	 */
	ఒకవేళ (!cpu_చురుకైన(busiest_cpu) || !cpu_చురుకైన(లక్ష్యం_cpu))
		వెళ్ళు బహిర్గతం_unlock;

	/* Make sure the requested CPU hasn't gone down in the meantime: */
	ఒకవేళ (unlikely(busiest_cpu != smp_processor_id() ||
		     !busiest_rq->చురుకైన_balance))
		వెళ్ళు బహిర్గతం_unlock;

	/* Is there any task to move? */
	ఒకవేళ (busiest_rq->nr_running <= 1)
		వెళ్ళు బహిర్గతం_unlock;

	/*
	 * This condition is "impossible", if it occurs
	 * we need to fix it. Originally reported by
	 * Bjorn Helgaas on a 128-CPU setup.
	 */
	WARN_ON_ONCE(busiest_rq == లక్ష్యం_rq);

	/* Search for an sd spanning us and the target CPU. */
	rcu_read_తాళం();
	for_each_domain(లక్ష్యం_cpu, sd) {
		ఒకవేళ (cpumask_test_cpu(busiest_cpu, sched_domain_పరిధి(sd)))
			ఆపు;
	}

	ఒకవేళ (likely(sd)) {
		నిర్మాణం lb_env env = {
			.sd		= sd,
			.dst_cpu	= లక్ష్యం_cpu,
			.dst_rq		= లక్ష్యం_rq,
			.src_cpu	= busiest_rq->cpu,
			.src_rq		= busiest_rq,
			.idle		= CPU_IDLE,
			.flags		= LBF_చురుకైన_LB | lb_flags,
		};

		schedstat_inc(sd->alb_లెక్క);
		update_rq_clock(busiest_rq);

		p = detach_one_task(&env);
		ఒకవేళ (p) {
			schedstat_inc(sd->alb_pushed);
			/* Active balancing done, reset the failure counter. */
			sd->nr_balance_failed = 0;
		} లేకపోతే {
			schedstat_inc(sd->alb_failed);
		}
	}
	rcu_read_unlock();
బహిర్గతం_unlock:
	busiest_rq->చురుకైన_balance = 0;
	rq_unlock(busiest_rq, &rf);

	ఒకవేళ (p)
		attach_one_task(లక్ష్యం_rq, p);

	local_irq_enable();

	తిరిగి 0;
}

స్థిర పూర్ణాంకం చురుకైన_ఎక్కించు_balance_cpu_stop(శూన్య *దత్తాంశం)
{
	తిరిగి __active_load_balance_cpu_stop(దత్తాంశం, 0);
}

స్థిర పూర్ణాంకం చురుకైన_ఎక్కించు_balance_llc_cpu_stop(శూన్య *దత్తాంశం)
{
	తిరిగి __active_load_balance_cpu_stop(దత్తాంశం, LBF_చురుకైన_LB_LLC);
}

/*
 * Scale the max sched_balance_rq interval with the number of CPUs in the system.
 * This trades load-balance latency on larger machines for less cross talk.
 */
శూన్య update_max_interval(శూన్య)
{
	max_ఎక్కించు_balance_interval = HZ*అంకె_online_cpus()/10;
}

స్థిర అంతఃవరుస శూన్య update_newidle_stats(నిర్మాణం sched_domain *sd, గుర్తులేని పూర్ణాంకం success)
{
	sd->newidle_call++;
	sd->newidle_success += success;

	ఒకవేళ (sd->newidle_call >= 1024) {
		u64 now = sched_clock();
		s64 delta = now - sd->newidle_stamp;
		sd->newidle_stamp = now;
		పూర్ణాంకం నిష్పత్తి = 0;

		ఒకవేళ (delta < 0)
			delta = 0;

		ఒకవేళ (sched_feat(NI_రేటు)) {
			/*
			 * ratio  delta   freq
			 *
			 * 1024 -  4  s -  128 Hz
			 *  512 -  2  s -  256 Hz
			 *  256 -  1  s -  512 Hz
			 *  128 - .5  s - 1024 Hz
			 *   64 - .25 s - 2048 Hz
			 */
			నిష్పత్తి = delta >> 22;
		}

		నిష్పత్తి += sd->newidle_success;

		sd->newidle_నిష్పత్తి = min(1024, నిష్పత్తి);
		sd->newidle_call /= 2;
		sd->newidle_success /= 2;
	}
}

స్థిర అంతఃవరుస bool
update_newidle_ఖర్చు(నిర్మాణం sched_domain *sd, u64 ఖర్చు, గుర్తులేని పూర్ణాంకం success)
{
	గుర్తులేని దీర్ఘ తదుపరి_decay = sd->చివరి_decay_max_lb_ఖర్చు + HZ;
	గుర్తులేని దీర్ఘ now = jiffies;

	ఒకవేళ (ఖర్చు)
		update_newidle_stats(sd, success);

	ఒకవేళ (ఖర్చు > sd->max_newidle_lb_ఖర్చు) {
		/*
		 * Track max cost of a domain to make sure to not delay the
		 * next wakeup on the CPU.
		 */
		sd->max_newidle_lb_ఖర్చు = ఖర్చు;
		sd->చివరి_decay_max_lb_ఖర్చు = now;

	} లేకపోతే ఒకవేళ (సమయం_after(now, తదుపరి_decay)) {
		/*
		 * Decay the newidle max times by ~1% per second to ensure that
		 * it is not outdated and the current max cost is actually
		 * shorter.
		 */
		sd->max_newidle_lb_ఖర్చు = (sd->max_newidle_lb_ఖర్చు * 253) / 256;
		sd->చివరి_decay_max_lb_ఖర్చు = now;
		తిరిగి true;
	}

	తిరిగి false;
}

/*
 * It checks each scheduling domain to see if it is due to be balanced,
 * and initiates a balancing operation if so.
 *
 * Balancing parameters are set up in init_sched_domains.
 */
స్థిర శూన్య sched_balance_domains(నిర్మాణం rq *rq, గణన cpu_idle_type idle)
{
	పూర్ణాంకం continue_balancing = 1;
	పూర్ణాంకం cpu = rq->cpu;
	పూర్ణాంకం busy = idle != CPU_IDLE && !sched_idle_rq(rq);
	గుర్తులేని దీర్ఘ interval;
	నిర్మాణం sched_domain *sd;
	/* Earliest time when we have to do rebalance again */
	గుర్తులేని దీర్ఘ తదుపరి_balance = jiffies + 60*HZ;
	పూర్ణాంకం update_తదుపరి_balance = 0;
	పూర్ణాంకం need_decay = 0;
	u64 max_ఖర్చు = 0;

	rcu_read_తాళం();
	for_each_domain(cpu, sd) {
		/*
		 * Decay the newidle max times here because this is a regular
		 * visit to all the domains.
		 */
		need_decay = update_newidle_ఖర్చు(sd, 0, 0);
		max_ఖర్చు += sd->max_newidle_lb_ఖర్చు;

		/*
		 * Stop the load balance at this level. There is another
		 * CPU in our sched group which is doing load balancing more
		 * actively.
		 */
		ఒకవేళ (!continue_balancing) {
			ఒకవేళ (need_decay)
				కొనసాగు;
			ఆపు;
		}

		interval = get_sd_balance_interval(sd, busy);
		ఒకవేళ (సమయం_after_eq(jiffies, sd->చివరి_balance + interval)) {
			ఒకవేళ (sched_balance_rq(cpu, rq, sd, idle, &continue_balancing)) {
				/*
				 * The LBF_DST_PINNED logic could have changed
				 * env->dst_cpu, so we can't know our idle
				 * state even if we migrated tasks. Update it.
				 */
				idle = idle_cpu(cpu);
				busy = !idle && !sched_idle_rq(rq);
			}
			sd->చివరి_balance = jiffies;
			interval = get_sd_balance_interval(sd, busy);
		}
		ఒకవేళ (సమయం_after(తదుపరి_balance, sd->చివరి_balance + interval)) {
			తదుపరి_balance = sd->చివరి_balance + interval;
			update_తదుపరి_balance = 1;
		}
	}
	ఒకవేళ (need_decay) {
		/*
		 * Ensure the rq-wide value also decays but keep it at a
		 * reasonable floor to avoid funnies with rq->avg_idle.
		 */
		rq->max_idle_balance_ఖర్చు =
			max((u64)sysctl_sched_migration_ఖర్చు, max_ఖర్చు);
	}
	rcu_read_unlock();

	/*
	 * next_balance will be updated only when there is a need.
	 * When the cpu is attached to null domain for ex, it will not be
	 * updated.
	 */
	ఒకవేళ (likely(update_తదుపరి_balance))
		rq->తదుపరి_balance = తదుపరి_balance;

}

స్థిర అంతఃవరుస పూర్ణాంకం on_null_domain(నిర్మాణం rq *rq)
{
	తిరిగి unlikely(!rcu_dereference_sched(rq->sd));
}

#నిర్వచితమైతే CONFIG_NO_HZ_COMMON
/*
 * NOHZ idle load balancing (ILB) details:
 *
 * - When one of the busy CPUs notices that there may be an idle rebalancing
 *   needed, they will kick the idle load balancer, which then does idle
 *   load balancing for all the idle CPUs.
 */
స్థిర అంతఃవరుస పూర్ణాంకం కనుగొను_new_ilb(శూన్య)
{
	నిర్మాణం cpumask *ilb_cpus;
	పూర్ణాంకం ilb_cpu, fallback = -1;

	lockdep_assert_irqs_disabled();

	/*
	 * Reuse the per-CPU select_rq_mask, which is protected from concurrent
	 * use on this CPU by having interrupts disabled.
	 */
	ilb_cpus = this_cpu_cpumask_var_ptr(select_rq_ముసుగు);
	cpumask_and(ilb_cpus, nohz.idle_cpus_ముసుగు,
		    housekeeping_cpumask(HK_TYPE_KERNEL_NOISE));

	for_each_cpu(ilb_cpu, ilb_cpus) {
		ఒకవేళ (!idle_cpu(ilb_cpu)) {
			/*
			 * Once an idle fallback exists, a busy CPU proves that
			 * this core cannot be fully idle. Skip its siblings.
			 */
			ఒకవేళ (sched_smt_చురుకైన() && fallback >= 0)
				cpumask_andnot(ilb_cpus, ilb_cpus, cpu_smt_ముసుగు(ilb_cpu));
			కొనసాగు;
		}

		/*
		 * Running the idle load balancer on an idle sibling of a busy
		 * SMT core can reduce the capacity available to its sibling. Prefer
		 * a CPU whose entire core is idle, but retain the first idle CPU as
		 * a fallback so idle balancing can still make progress when no fully
		 * idle core exists.
		 */
		ఒకవేళ (sched_smt_చురుకైన() && !is_core_idle(ilb_cpu)) {
			ఒకవేళ (fallback < 0)
				fallback = ilb_cpu;

			/*
			 * The core is not idle, so there is no need to check
			 * any of its other SMT siblings.
			 */
			cpumask_andnot(ilb_cpus, ilb_cpus,
				       cpu_smt_ముసుగు(ilb_cpu));
			కొనసాగు;
		}

		తిరిగి ilb_cpu;
	}

	తిరిగి fallback;
}

/*
 * Kick a CPU to do the NOHZ balancing, if it is time for it, via a cross-CPU
 * SMP function call (IPI).
 *
 * Prefer a CPU on a fully idle core in the HK_TYPE_KERNEL_NOISE housekeeping
 * set. Fall back to the first idle CPU when no fully idle core exists.
 */
స్థిర శూన్య kick_ilb(గుర్తులేని పూర్ణాంకం flags)
{
	పూర్ణాంకం ilb_cpu;

	/*
	 * Increase nohz.next_balance only when if full ilb is triggered but
	 * not if we only update stats.
	 */
	ఒకవేళ (flags & NOHZ_BALANCE_KICK)
		nohz.తదుపరి_balance = jiffies+1;

	ilb_cpu = కనుగొను_new_ilb();
	ఒకవేళ (ilb_cpu < 0)
		తిరిగి;

	/*
	 * Don't bother if no new NOHZ balance work items for ilb_cpu,
	 * i.e. all bits in flags are already set in ilb_cpu.
	 */
	ఒకవేళ ((atomic_read(nohz_flags(ilb_cpu)) & flags) == flags)
		తిరిగి;

	/*
	 * Access to rq::nohz_csd is serialized by NOHZ_KICK_MASK; he who sets
	 * the first flag owns it; cleared by nohz_csd_func().
	 */
	flags = atomic_fetch_or(flags, nohz_flags(ilb_cpu));
	ఒకవేళ (flags & NOHZ_KICK_ముసుగు)
		తిరిగి;

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
స్థిర శూన్య nohz_balancer_kick(నిర్మాణం rq *rq)
{
	గుర్తులేని దీర్ఘ now = jiffies;
	నిర్మాణం sched_domain_shared *sds;
	నిర్మాణం sched_domain *sd;
	పూర్ణాంకం nr_busy, i, cpu = rq->cpu;
	గుర్తులేని పూర్ణాంకం flags = 0;

	ఒకవేళ (unlikely(rq->idle_balance))
		తిరిగి;

	/*
	 * We may be recently in ticked or tickless idle mode. At the first
	 * busy tick after returning from idle, we will update the busy stats.
	 */
	nohz_balance_exit_idle(rq);

	ఒకవేళ (READ_ONCE(nohz.has_blocked_ఎక్కించు) &&
	    సమయం_after(now, READ_ONCE(nohz.తదుపరి_blocked)))
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
	ఒకవేళ (సమయం_before(now, nohz.తదుపరి_balance))
		వెళ్ళు బహిర్గతం;

	/*
	 * None are in tickless mode and hence no need for NOHZ idle load
	 * balancing
	 */
	ఒకవేళ (unlikely(cpumask_ఖాళీలు(nohz.idle_cpus_ముసుగు)))
		తిరిగి;

	ఒకవేళ (rq->nr_running >= 2) {
		flags = NOHZ_STATS_KICK | NOHZ_BALANCE_KICK;
		వెళ్ళు బహిర్గతం;
	}

	sd = rcu_dereference_all(rq->sd);
	ఒకవేళ (sd) {
		/*
		 * If there's a runnable CFS task and the current CPU has reduced
		 * capacity, kick the ILB to see if there's a better CPU to run on:
		 */
		ఒకవేళ (rq->cfs.h_nr_runnable >= 1 && check_cpu_సామర్థ్యం(rq, sd)) {
			flags |= NOHZ_STATS_KICK | NOHZ_BALANCE_KICK;
			వెళ్ళు బహిర్గతం;
		}
	}

	sd = rcu_dereference_all(చొప్పున_cpu(sd_asym_packing, cpu));
	ఒకవేళ (sd) {
		/*
		 * When ASYM_PACKING; see if there's a more preferred CPU
		 * currently idle; in which case, kick the ILB to move tasks
		 * around.
		 *
		 * When balancing between cores, all the SMT siblings of the
		 * preferred CPU must be idle.
		 */
		for_each_cpu_and(i, sched_domain_పరిధి(sd), nohz.idle_cpus_ముసుగు) {
			ఒకవేళ (sched_asym(sd, i, cpu)) {
				flags |= NOHZ_STATS_KICK | NOHZ_BALANCE_KICK;
				వెళ్ళు బహిర్గతం;
			}
		}
	}

	sd = rcu_dereference_all(చొప్పున_cpu(sd_asym_cpucapacity, cpu));
	ఒకవేళ (sd) {
		/*
		 * When ASYM_CPUCAPACITY; see if there's a higher capacity CPU
		 * to run the misfit task on.
		 */
		ఒకవేళ (check_misfit_స్థితి(rq))
			flags |= NOHZ_STATS_KICK | NOHZ_BALANCE_KICK;

		/*
		 * For asymmetric systems, we do not want to nicely balance
		 * cache use, instead we want to embrace asymmetry and only
		 * ensure tasks have enough CPU capacity.
		 *
		 * Skip the LLC logic because it's not relevant in that case.
		 */
		వెళ్ళు బహిర్గతం;
	}

	sds = rcu_dereference_all(చొప్పున_cpu(sd_balance_shared, cpu));
	ఒకవేళ (sds) {
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
		ఒకవేళ (nr_busy > 1)
			flags |= NOHZ_STATS_KICK | NOHZ_BALANCE_KICK;
	}
బహిర్గతం:
	ఒకవేళ (READ_ONCE(nohz.needs_update))
		flags |= NOHZ_తదుపరి_KICK;

	ఒకవేళ (flags)
		kick_ilb(flags);
}

స్థిర శూన్య సమితి_cpu_sd_state_busy(పూర్ణాంకం cpu)
{
	నిర్మాణం sched_domain *sd;
	sd = rcu_dereference_all(చొప్పున_cpu(sd_llc, cpu));

	/*
	 * sd->nohz_idle only pairs with nr_busy_cpus on sd->shared; if this
	 * domain has no shared object there is nothing to clear or account.
	 */
	ఒకవేళ (!sd || !sd->shared || !sd->nohz_idle)
		తిరిగి;
	sd->nohz_idle = 0;

	atomic_inc(&sd->shared->nr_busy_cpus);
}

శూన్య nohz_balance_exit_idle(నిర్మాణం rq *rq)
{
	WARN_ON_ONCE(rq != this_rq());

	ఒకవేళ (likely(!rq->nohz_tick_stopped))
		తిరిగి;

	rq->nohz_tick_stopped = 0;
	cpumask_clear_cpu(rq->cpu, nohz.idle_cpus_ముసుగు);

	సమితి_cpu_sd_state_busy(rq->cpu);
}

స్థిర శూన్య సమితి_cpu_sd_state_idle(పూర్ణాంకం cpu)
{
	నిర్మాణం sched_domain *sd;
	sd = rcu_dereference_all(చొప్పున_cpu(sd_llc, cpu));

	/* See set_cpu_sd_state_busy(): nohz_idle is only used with sd->shared. */
	ఒకవేళ (!sd || !sd->shared || sd->nohz_idle)
		తిరిగి;
	sd->nohz_idle = 1;

	atomic_dec(&sd->shared->nr_busy_cpus);
}

/*
 * This routine will record that the CPU is going idle with tick stopped.
 * This info will be used in performing idle load balancing in the future.
 */
శూన్య nohz_balance_enter_idle(పూర్ణాంకం cpu)
{
	నిర్మాణం rq *rq = cpu_rq(cpu);

	WARN_ON_ONCE(cpu != smp_processor_id());

	/* If this CPU is going down, then nothing needs to be done: */
	ఒకవేళ (!cpu_చురుకైన(cpu))
		తిరిగి;

	/*
	 * Can be set safely without rq->lock held
	 * If a clear happens, it will have evaluated last additions because
	 * rq->lock is held during the check and the clear
	 */
	rq->has_blocked_ఎక్కించు = 1;

	/*
	 * The tick is still stopped but load could have been added in the
	 * meantime. We set the nohz.has_blocked_load flag to trig a check of the
	 * *_avg. The CPU is already part of nohz.idle_cpus_mask so the clear
	 * of nohz.has_blocked_load can only happen after checking the new load
	 */
	ఒకవేళ (rq->nohz_tick_stopped)
		వెళ్ళు బహిర్గతం;

	/* If we're a completely isolated CPU, we don't play: */
	ఒకవేళ (on_null_domain(rq))
		తిరిగి;

	rq->nohz_tick_stopped = 1;

	cpumask_సమితి_cpu(cpu, nohz.idle_cpus_ముసుగు);

	/*
	 * Ensures that if nohz_idle_balance() fails to observe our
	 * @idle_cpus_mask store, it must observe the @has_blocked_load
	 * and @needs_update stores.
	 */
	smp_mb__after_atomic();

	సమితి_cpu_sd_state_idle(cpu);

	WRITE_ONCE(nohz.needs_update, 1);
బహిర్గతం:
	/*
	 * Each time a cpu enter idle, we assume that it has blocked load and
	 * enable the periodic update of the load of idle CPUs
	 */
	WRITE_ONCE(nohz.has_blocked_ఎక్కించు, 1);
}

స్థిర bool update_nohz_stats(నిర్మాణం rq *rq)
{
	గుర్తులేని పూర్ణాంకం cpu = rq->cpu;

	ఒకవేళ (!rq->has_blocked_ఎక్కించు)
		తిరిగి false;

	ఒకవేళ (!cpumask_test_cpu(cpu, nohz.idle_cpus_ముసుగు))
		తిరిగి false;

	ఒకవేళ (!సమయం_after(jiffies, READ_ONCE(rq->చివరి_blocked_ఎక్కించు_update_tick)))
		తిరిగి true;

	sched_balance_update_blocked_averages(cpu);

	తిరిగి rq->has_blocked_ఎక్కించు;
}

/*
 * Internal function that runs load balance for all idle CPUs. The load balance
 * can be a simple update of blocked load or a complete load balance with
 * tasks movement depending of flags.
 */
స్థిర శూన్య _nohz_idle_balance(నిర్మాణం rq *this_rq, గుర్తులేని పూర్ణాంకం flags)
{
	/* Earliest time when we have to do rebalance again */
	గుర్తులేని దీర్ఘ now = jiffies;
	గుర్తులేని దీర్ఘ తదుపరి_balance = now + 60*HZ;
	bool has_blocked_ఎక్కించు = false;
	పూర్ణాంకం update_తదుపరి_balance = 0;
	పూర్ణాంకం this_cpu = this_rq->cpu;
	పూర్ణాంకం balance_cpu;
	నిర్మాణం rq *rq;

	WARN_ON_ONCE((flags & NOHZ_KICK_ముసుగు) == NOHZ_BALANCE_KICK);

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
	ఒకవేళ (flags & NOHZ_STATS_KICK)
		WRITE_ONCE(nohz.has_blocked_ఎక్కించు, 0);
	ఒకవేళ (flags & NOHZ_తదుపరి_KICK)
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
	for_each_cpu_wrap(balance_cpu,  nohz.idle_cpus_ముసుగు, this_cpu+1) {
		ఒకవేళ (!idle_cpu(balance_cpu))
			కొనసాగు;

		/*
		 * If this CPU gets work to do, stop the load balancing
		 * work being done for other CPUs. Next load
		 * balancing owner will pick it up.
		 */
		ఒకవేళ (!idle_cpu(this_cpu) && need_resched()) {
			ఒకవేళ (flags & NOHZ_STATS_KICK)
				has_blocked_ఎక్కించు = true;
			ఒకవేళ (flags & NOHZ_తదుపరి_KICK)
				WRITE_ONCE(nohz.needs_update, 1);
			వెళ్ళు abort;
		}

		rq = cpu_rq(balance_cpu);

		ఒకవేళ (flags & NOHZ_STATS_KICK)
			has_blocked_ఎక్కించు |= update_nohz_stats(rq);

		/*
		 * If time for next balance is due,
		 * do the balance.
		 */
		ఒకవేళ (సమయం_after_eq(jiffies, rq->తదుపరి_balance)) {
			నిర్మాణం rq_flags rf;

			rq_తాళం_irqsave(rq, &rf);
			update_rq_clock(rq);
			rq_unlock_irqrestore(rq, &rf);

			ఒకవేళ (flags & NOHZ_BALANCE_KICK)
				sched_balance_domains(rq, CPU_IDLE);
		}

		ఒకవేళ (సమయం_after(తదుపరి_balance, rq->తదుపరి_balance)) {
			తదుపరి_balance = rq->తదుపరి_balance;
			update_తదుపరి_balance = 1;
		}
	}

	/*
	 * next_balance will be updated only when there is a need.
	 * When the CPU is attached to null domain for ex, it will not be
	 * updated.
	 */
	ఒకవేళ (likely(update_తదుపరి_balance))
		nohz.తదుపరి_balance = తదుపరి_balance;

	ఒకవేళ (flags & NOHZ_STATS_KICK)
		WRITE_ONCE(nohz.తదుపరి_blocked,
			   now + msecs_గమ్యం_jiffies(ఎక్కించు_AVG_PERIOD));

abort:
	/* There is still blocked load, enable periodic update */
	ఒకవేళ (has_blocked_ఎక్కించు)
		WRITE_ONCE(nohz.has_blocked_ఎక్కించు, 1);
}

/*
 * In CONFIG_NO_HZ_COMMON case, the idle balance kickee will do the
 * rebalancing for all the CPUs for whom scheduler ticks are stopped.
 */
స్థిర bool nohz_idle_balance(నిర్మాణం rq *this_rq, గణన cpu_idle_type idle)
{
	గుర్తులేని పూర్ణాంకం flags = this_rq->nohz_idle_balance;

	ఒకవేళ (!flags)
		తిరిగి false;

	this_rq->nohz_idle_balance = 0;

	ఒకవేళ (idle != CPU_IDLE)
		తిరిగి false;

	_nohz_idle_balance(this_rq, flags);

	తిరిగి true;
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
శూన్య nohz_నిరంతరం_idle_balance(పూర్ణాంకం cpu)
{
	గుర్తులేని పూర్ణాంకం flags;

	flags = atomic_fetch_andnot(NOHZ_NEWILB_KICK, nohz_flags(cpu));

	/*
	 * Update the blocked load only if no SCHED_SOFTIRQ is about to happen
	 * (i.e. NOHZ_STATS_KICK set) and will do the same.
	 */
	ఒకవేళ ((flags == NOHZ_NEWILB_KICK) && !need_resched())
		_nohz_idle_balance(cpu_rq(cpu), NOHZ_STATS_KICK);
}

స్థిర శూన్య nohz_newidle_balance(నిర్మాణం rq *this_rq)
{
	పూర్ణాంకం this_cpu = this_rq->cpu;

	/* Will wake up very soon. No time for doing anything else*/
	ఒకవేళ (this_rq->avg_idle < sysctl_sched_migration_ఖర్చు)
		తిరిగి;

	/* Don't need to update blocked load of idle CPUs*/
	ఒకవేళ (!READ_ONCE(nohz.has_blocked_ఎక్కించు) ||
	    సమయం_before(jiffies, READ_ONCE(nohz.తదుపరి_blocked)))
		తిరిగి;

	/*
	 * Set the need to trigger ILB in order to update blocked load
	 * before entering idle state.
	 */
	atomic_or(NOHZ_NEWILB_KICK, nohz_flags(this_cpu));
}

#లేకపోతే /* !CONFIG_NO_HZ_COMMON: */
స్థిర అంతఃవరుస శూన్య nohz_balancer_kick(నిర్మాణం rq *rq) { }

స్థిర అంతఃవరుస bool nohz_idle_balance(నిర్మాణం rq *this_rq, గణన cpu_idle_type idle)
{
	తిరిగి false;
}

స్థిర అంతఃవరుస శూన్య nohz_newidle_balance(నిర్మాణం rq *this_rq) { }
#అయితే_ముగింపు /* !CONFIG_NO_HZ_COMMON */

/*
 * sched_balance_newidle is called by schedule() if this_cpu is about to become
 * idle. Attempts to pull tasks from other CPUs.
 *
 * Returns:
 *   < 0 - we released the lock and there are !fair tasks present
 *     0 - failed, no new tasks
 *   > 0 - success, new (fair) tasks present
 */
స్థిర పూర్ణాంకం sched_balance_newidle(నిర్మాణం rq *this_rq, నిర్మాణం rq_flags *rf)
	__must_hold(__rq_lockp(this_rq))
{
	గుర్తులేని దీర్ఘ తదుపరి_balance = jiffies + HZ;
	పూర్ణాంకం this_cpu = this_rq->cpu;
	పూర్ణాంకం continue_balancing = 1;
	u64 t0, t1, curr_ఖర్చు = 0;
	నిర్మాణం sched_domain *sd;
	పూర్ణాంకం pulled_task = 0;

	update_misfit_స్థితి(ఖాళీ, this_rq);

	/*
	 * There is a task waiting to run. No need to search for one.
	 * Return 0; the task will be enqueued when switching to idle.
	 */
	ఒకవేళ (this_rq->ttwu_pending)
		తిరిగి 0;

	/*
	 * We must set idle_stamp _before_ calling sched_balance_rq()
	 * for CPU_NEWLY_IDLE, such that we measure the this duration
	 * as idle time.
	 */
	this_rq->idle_stamp = rq_clock(this_rq);

	/* Do not pull tasks towards !preferred CPUs */
	ఒకవేళ (!cpu_preferred(this_cpu))
		తిరిగి 0;

	/*
	 * This is OK, because current is on_cpu, which avoids it being picked
	 * for load-balance and preemption/IRQs are still disabled avoiding
	 * further scheduler activity on it and we're being very careful to
	 * re-start the picking loop.
	 */
	rq_unpin_తాళం(this_rq, rf);

	sd = rcu_dereference_sched_domain(this_rq->sd);
	ఒకవేళ (!sd)
		వెళ్ళు బహిర్గతం;

	ఒకవేళ (!get_rd_overloaded(this_rq->rd) ||
	    this_rq->avg_idle < sd->max_newidle_lb_ఖర్చు) {

		update_తదుపరి_balance(sd, &తదుపరి_balance);
		వెళ్ళు బహిర్గతం;
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
		u64 domain_ఖర్చు;

		update_తదుపరి_balance(sd, &తదుపరి_balance);

		ఒకవేళ (this_rq->avg_idle < curr_ఖర్చు + sd->max_newidle_lb_ఖర్చు)
			ఆపు;

		ఒకవేళ (sd->flags & SD_BALANCE_NEWIDLE) {
			గుర్తులేని పూర్ణాంకం బరువు = 1;

			ఒకవేళ (sched_feat(NI_RANDOM) && sd->newidle_నిష్పత్తి < 1024) {
				/*
				 * Throw a 1k sided dice; and only run
				 * newidle_balance according to the success
				 * rate.
				 */
				u32 d1k = sched_rng() % 1024;
				బరువు = 1 + sd->newidle_నిష్పత్తి;
				ఒకవేళ (d1k > బరువు) {
					update_newidle_stats(sd, 0);
					కొనసాగు;
				}
				బరువు = (1024 + బరువు/2) / బరువు;
			}

			pulled_task = sched_balance_rq(this_cpu, this_rq,
						   sd, CPU_NEWLY_IDLE,
						   &continue_balancing);

			t1 = sched_clock_cpu(this_cpu);
			domain_ఖర్చు = t1 - t0;
			curr_ఖర్చు += domain_ఖర్చు;
			t0 = t1;

			/*
			 * Track max cost of a domain to make sure to not delay the
			 * next wakeup on the CPU.
			 */
			update_newidle_ఖర్చు(sd, domain_ఖర్చు, బరువు * !!pulled_task);
		}

		/*
		 * Stop searching for tasks to pull if there are
		 * now runnable tasks on this rq.
		 */
		ఒకవేళ (pulled_task || !continue_balancing)
			ఆపు;
	}

	raw_spin_rq_తాళం(this_rq);

	ఒకవేళ (curr_ఖర్చు > this_rq->max_idle_balance_ఖర్చు)
		this_rq->max_idle_balance_ఖర్చు = curr_ఖర్చు;

	/*
	 * While browsing the domains, we released the rq lock, a task could
	 * have been enqueued in the meantime. Since we're not going idle,
	 * pretend we pulled a task.
	 */
	ఒకవేళ (this_rq->cfs.h_nr_queued && !pulled_task)
		pulled_task = 1;

	/* If a higher prio class was modified, restart the pick */
	ఒకవేళ (rq_modified_above(this_rq, &fair_sched_class))
		pulled_task = -1;

బహిర్గతం:
	/* Move the next balance forward */
	ఒకవేళ (సమయం_after(this_rq->తదుపరి_balance, తదుపరి_balance))
		this_rq->తదుపరి_balance = తదుపరి_balance;

	ఒకవేళ (pulled_task)
		this_rq->idle_stamp = 0;
	లేకపోతే
		nohz_newidle_balance(this_rq);

	rq_repin_తాళం(this_rq, rf);

	తిరిగి pulled_task;
}

/*
 * This softirq handler is triggered via SCHED_SOFTIRQ from two places:
 *
 * - directly from the local sched_tick() for periodic load balancing
 *
 * - indirectly from a remote sched_tick() for NOHZ idle balancing
 *   through the SMP cross-call nohz_csd_func()
 */
స్థిర __latent_entropy శూన్య sched_balance_softirq(శూన్య)
{
	నిర్మాణం rq *this_rq = this_rq();
	గణన cpu_idle_type idle = this_rq->idle_balance;
	/*
	 * If this CPU has a pending NOHZ_BALANCE_KICK, then do the
	 * balancing on behalf of the other idle CPUs whose ticks are
	 * stopped. Do nohz_idle_balance *before* sched_balance_domains to
	 * give the idle CPUs a chance to load balance. Else we may
	 * load balance only within the local sched_domain hierarchy
	 * and abort nohz_idle_balance altogether if we pull some load.
	 */
	ఒకవేళ (nohz_idle_balance(this_rq, idle))
		తిరిగి;

	/* normal load balance */
	sched_balance_update_blocked_averages(this_rq->cpu);
	sched_balance_domains(this_rq, idle);
}

/*
 * Trigger the SCHED_SOFTIRQ if it is time to do periodic load balancing.
 */
శూన్య sched_balance_trigger(నిర్మాణం rq *rq)
{
	/*
	 * Don't need to rebalance while attached to NULL domain or
	 * runqueue CPU is not active
	 */
	ఒకవేళ (unlikely(on_null_domain(rq) || !cpu_చురుకైన(cpu_యొక్క(rq))))
		తిరిగి;

	ఒకవేళ (సమయం_after_eq(jiffies, rq->తదుపరి_balance))
		raise_softirq(SCHED_SOFTIRQ);

	nohz_balancer_kick(rq);
}

స్థిర శూన్య rq_online_fair(నిర్మాణం rq *rq)
{
	update_sysctl();

	update_runtime_enabled(rq);
}

స్థిర శూన్య rq_offline_fair(నిర్మాణం rq *rq)
{
	update_sysctl();

	/* Ensure any throttled groups are reachable by pick_next_task */
	unthrottle_offline_cfs_rqs(rq);

	/* Ensure that we remove rq contribution to group share: */
	clear_tg_offline_cfs_rqs(rq);
}

#నిర్వచితమైతే CONFIG_SCHED_CORE
స్థిర అంతఃవరుస bool
__entity_slice_used(నిర్మాణం sched_entity *se, పూర్ణాంకం min_nr_tasks)
{
	u64 rtime = se->కూడిక_exec_runtime - se->prev_కూడిక_exec_runtime;
	u64 ముక్క = se->ముక్క;

	తిరిగి (rtime * min_nr_tasks > ముక్క);
}

#నిర్వచించు MIN_NR_TASKS_DURING_FORCEIDLE	2
స్థిర అంతఃవరుస శూన్య task_tick_core(నిర్మాణం rq *rq, నిర్మాణం task_struct *curr)
{
	ఒకవేళ (!sched_core_enabled(rq))
		తిరిగి;

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
	ఒకవేళ (rq->core->core_forceidle_లెక్క && rq->cfs.h_nr_queued == 1 &&
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
స్థిర శూన్య se_fi_update(స్థిరరాశి నిర్మాణం sched_entity *se, గుర్తులేని పూర్ణాంకం fi_seq,
			 bool forceidle)
{
	for_each_sched_entity(se) {
		నిర్మాణం cfs_rq *cfs_rq = cfs_rq_యొక్క(se);

		ఒకవేళ (forceidle) {
			ఒకవేళ (cfs_rq->forceidle_seq == fi_seq)
				ఆపు;
			cfs_rq->forceidle_seq = fi_seq;
		}

		cfs_rq->zero_vruntime_fi = cfs_rq->zero_vruntime;
	}
}

శూన్య task_vruntime_update(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, bool లోపల_fi)
{
	నిర్మాణం sched_entity *se = &p->se;

	ఒకవేళ (p->sched_class != &fair_sched_class)
		తిరిగి;

	se_fi_update(se, rq->core->core_forceidle_seq, లోపల_fi);
}

bool cfs_prio_less(స్థిరరాశి నిర్మాణం task_struct *a, స్థిరరాశి నిర్మాణం task_struct *b,
			bool లోపల_fi)
{
	నిర్మాణం rq *rq = task_rq(a);
	స్థిరరాశి నిర్మాణం sched_entity *sea = &a->se;
	స్థిరరాశి నిర్మాణం sched_entity *seb = &b->se;
	నిర్మాణం cfs_rq *cfs_rqa;
	నిర్మాణం cfs_rq *cfs_rqb;
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

	తిరిగి delta > 0;
}

స్థిర పూర్ణాంకం task_is_throttled_fair(నిర్మాణం task_struct *p, పూర్ణాంకం cpu)
{
	నిర్మాణం cfs_rq *cfs_rq;

#నిర్వచితమైతే CONFIG_FAIR_సమూహం_SCHED
	cfs_rq = tg_cfs_rq(task_సమూహం(p), cpu);
#లేకపోతే
	cfs_rq = &cpu_rq(cpu)->cfs;
#అయితే_ముగింపు
	తిరిగి throttled_hierarchy(cfs_rq);
}
#లేకపోతే /* !CONFIG_SCHED_CORE: */
స్థిర అంతఃవరుస శూన్య task_tick_core(నిర్మాణం rq *rq, నిర్మాణం task_struct *curr) {}
#అయితే_ముగింపు /* !CONFIG_SCHED_CORE */

/*
 * scheduler tick hitting a task of our scheduling class.
 *
 * NOTE: This function can be called remotely by the tick offload that
 * goes along full dynticks. Therefore no local assumption can be made
 * and everything must be accessed through the @rq and @curr passed in
 * parameters.
 */
స్థిర శూన్య task_tick_fair(నిర్మాణం rq *rq, నిర్మాణం task_struct *curr, పూర్ణాంకం queued)
{
	నిర్మాణం sched_entity *se = &curr->se;

	ఒకవేళ (se->on_rq) {
		గుర్తులేని దీర్ఘ బరువు = NICE_0_ఎక్కించు;
		నిర్మాణం cfs_rq *cfs_rq;

		for_each_sched_entity(se) {
			cfs_rq = cfs_rq_యొక్క(se);
			entity_tick(cfs_rq, se, queued);

			బరువు = __calc_prop_weight(cfs_rq, se, బరువు);
		}

		se = &curr->se;
		reweight_eevdf(cfs_rq, se, బరువు, se->on_rq);
	}

	ఒకవేళ (queued)
		తిరిగి;

	ఒకవేళ (static_branch_unlikely(&sched_numa_balancing))
		task_tick_numa(rq, curr);

	task_tick_cache(rq, curr);

	update_misfit_స్థితి(curr, rq);
	check_update_overutilized_స్థితి(task_rq(curr));

	task_tick_core(rq, curr);
}

/*
 * called on fork with the child task as argument from the parent's context
 *  - child not yet on the tasklist
 *  - preemption disabled
 */
స్థిర శూన్య task_fork_fair(నిర్మాణం task_struct *p)
{
	సమితి_task_max_allowed_సామర్థ్యం(p);
}

/*
 * Priority of the task has changed. Check to see if we preempt
 * the current task.
 */
స్థిర శూన్య
prio_మారింది_fair(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, u64 oldprio)
{
	ఒకవేళ (!task_on_rq_queued(p))
		తిరిగి;

	ఒకవేళ (p->prio == oldprio)
		తిరిగి;

	ఒకవేళ (rq->cfs.h_nr_queued == 1)
		తిరిగి;

	/*
	 * Reschedule if we are currently running on this runqueue and
	 * our priority decreased, or if we are not currently running on
	 * this runqueue and our priority is higher than the current's
	 */
	ఒకవేళ (task_current_donor(rq, p)) {
		ఒకవేళ (p->prio > oldprio)
			resched_curr(rq);
	} లేకపోతే {
		wakeup_preempt(rq, p, 0);
	}
}

#నిర్వచితమైతే CONFIG_FAIR_సమూహం_SCHED
/*
 * Propagate the changes of the sched_entity across the tg tree to make it
 * visible to the root
 */
స్థిర శూన్య propagate_entity_cfs_rq(నిర్మాణం sched_entity *se)
{
	నిర్మాణం cfs_rq *cfs_rq = cfs_rq_యొక్క(se);

	/*
	 * If a task gets attached to this cfs_rq and before being queued,
	 * it gets migrated to another CPU due to reasons like affinity
	 * change, make sure this cfs_rq stays on leaf cfs_rq list to have
	 * that removed load decayed or it can cause faireness problem.
	 */
	ఒకవేళ (!cfs_rq_pelt_clock_throttled(cfs_rq))
		list_కూడు_ఆకు_cfs_rq(cfs_rq);

	/* Start to propagate at parent */
	se = se->మాతృ;

	for_each_sched_entity(se) {
		cfs_rq = cfs_rq_యొక్క(se);

		update_ఎక్కించు_avg(cfs_rq, se, UPDATE_TG);

		ఒకవేళ (!cfs_rq_pelt_clock_throttled(cfs_rq))
			list_కూడు_ఆకు_cfs_rq(cfs_rq);
	}

	assert_list_ఆకు_cfs_rq(rq_యొక్క(cfs_rq));
}
#లేకపోతే /* !CONFIG_FAIR_GROUP_SCHED: */
స్థిర శూన్య propagate_entity_cfs_rq(నిర్మాణం sched_entity *se) { }
#అయితే_ముగింపు /* !CONFIG_FAIR_GROUP_SCHED */

స్థిర శూన్య detach_entity_cfs_rq(నిర్మాణం sched_entity *se)
{
	నిర్మాణం cfs_rq *cfs_rq = cfs_rq_యొక్క(se);

	/*
	 * In case the task sched_avg hasn't been attached:
	 * - A forked task which hasn't been woken up by wake_up_new_task().
	 * - A task which has been woken up by try_to_wake_up() but is
	 *   waiting for actually being woken up by sched_ttwu_pending().
	 */
	ఒకవేళ (!se->avg.చివరి_update_సమయం)
		తిరిగి;

	/* Catch up with the cfs_rq and remove our load when we leave */
	update_ఎక్కించు_avg(cfs_rq, se, 0);
	detach_entity_ఎక్కించు_avg(cfs_rq, se);
	update_tg_ఎక్కించు_avg(cfs_rq);
	propagate_entity_cfs_rq(se);
}

స్థిర శూన్య attach_entity_cfs_rq(నిర్మాణం sched_entity *se)
{
	నిర్మాణం cfs_rq *cfs_rq = cfs_rq_యొక్క(se);

	/* Synchronize entity with its cfs_rq */
	update_ఎక్కించు_avg(cfs_rq, se, sched_feat(ATTACH_AGE_ఎక్కించు) ? 0 : SKIP_AGE_ఎక్కించు);
	attach_entity_ఎక్కించు_avg(cfs_rq, se);
	update_tg_ఎక్కించు_avg(cfs_rq);
	propagate_entity_cfs_rq(se);
}

స్థిర శూన్య detach_task_cfs_rq(నిర్మాణం task_struct *p)
{
	నిర్మాణం sched_entity *se = &p->se;

	detach_entity_cfs_rq(se);
}

స్థిర శూన్య attach_task_cfs_rq(నిర్మాణం task_struct *p)
{
	నిర్మాణం sched_entity *se = &p->se;

	attach_entity_cfs_rq(se);
}

స్థిర శూన్య switching_from_fair(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
	ఒకవేళ (p->se.sched_delayed)
		వరుస_నుండి_తీయి_task(rq, p, వరుస_నుండి_తీయి_SLEEP | వరుస_నుండి_తీయి_DELAYED | వరుస_నుండి_తీయి_NOCLOCK);
}

స్థిర శూన్య switched_from_fair(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
	detach_task_cfs_rq(p);
}

స్థిర శూన్య switched_గమ్యం_fair(నిర్మాణం rq *rq, నిర్మాణం task_struct *p)
{
	WARN_ON_ONCE(p->se.sched_delayed);

	attach_task_cfs_rq(p);

	సమితి_task_max_allowed_సామర్థ్యం(p);

	ఒకవేళ (task_on_rq_queued(p)) {
		/*
		 * We were most likely switched from sched_rt, so
		 * kick off the schedule if running, otherwise just see
		 * if we can still preempt the current task.
		 */
		ఒకవేళ (task_current_donor(rq, p))
			resched_curr(rq);
		లేకపోతే
			wakeup_preempt(rq, p, 0);
	}
}

స్థిర శూన్య సమితి_తదుపరి_task_fair(నిర్మాణం rq *rq, నిర్మాణం task_struct *p, గణన snt_e type)
{
	నిర్మాణం sched_entity *se = &p->se;
	నిర్మాణం cfs_rq *cfs_rq = &rq->cfs;
	గుర్తులేని దీర్ఘ బరువు = NICE_0_ఎక్కించు;
	bool మొదటి = type == SNT_PICK;
	bool throttled = false;
	bool on_rq = se->on_rq;

	ఒకవేళ (type == SNT_REPICK)
		వెళ్ళు repick;

	clear_buddies(cfs_rq, se);

	ఒకవేళ (on_rq)
		__dequeue_entity(cfs_rq, se);

	for_each_sched_entity(se) {
		cfs_rq = cfs_rq_యొక్క(se);

		ఒకవేళ (!IS_ENABLED(CONFIG_FAIR_సమూహం_SCHED) ||
		    !మొదటి || !cfs_rq->h_curr)
			సమితి_తదుపరి_entity(cfs_rq, se);

		/* ensure bandwidth has been allocated on our new cfs_rq */
		throttled |= account_cfs_rq_runtime(cfs_rq, 0);

		ఒకవేళ (on_rq)
			బరువు = __calc_prop_weight(cfs_rq, se, బరువు);
	}

	ఒకవేళ (throttled)
		task_throttle_అమరిక_work(p);

	se = &p->se;
	cfs_rq->curr = se;

	ఒకవేళ (on_rq) {
		reweight_eevdf(cfs_rq, se, బరువు, se->on_rq);
		ఒకవేళ (మొదటి)
			సమితి_protect_ముక్క(cfs_rq, se);
	}

	ఒకవేళ (task_on_rq_queued(p)) {
		/*
		 * Move the next running task to the front of the list, so our
		 * cfs_tasks list becomes MRU one.
		 */
		list_move(&se->సమూహం_కణుపు, &rq->cfs_tasks);
	}
	ఒకవేళ (!మొదటి)
		తిరిగి;

	WARN_ON_ONCE(se->sched_delayed);

	update_misfit_స్థితి(p, rq);
	sched_fair_update_stop_tick(rq, p);

repick:
	/*
	 * A same-task repick skips put_prev_task_fair(), but
	 * pick_task_fair() refreshed the entity hrtick_start_fair() reads
	 * before selecting it again. rq->cfs.curr identifies that entity,
	 * including with group scheduling.
	 */
	ఒకవేళ (hrtick_enabled_fair(rq))
		hrtick_ప్రారంభం_fair(rq, p);
}

శూన్య init_cfs_rq(నిర్మాణం cfs_rq *cfs_rq)
{
	cfs_rq->tasks_timeline = RB_మూలం_CACHED;
	cfs_rq->zero_vruntime = (u64)(-(1LL << 20));
	raw_spin_తాళం_init(&cfs_rq->removed.తాళం);
}

#నిర్వచితమైతే CONFIG_FAIR_సమూహం_SCHED
స్థిర శూన్య task_change_సమూహం_fair(నిర్మాణం task_struct *p)
{
	/*
	 * We couldn't detach or attach a forked task which
	 * hasn't been woken up by wake_up_new_task().
	 */
	ఒకవేళ (READ_ONCE(p->__state) == TASK_NEW)
		తిరిగి;

	detach_task_cfs_rq(p);

	/* Tell se's cfs_rq has been changed -- migrated */
	p->se.avg.చివరి_update_సమయం = 0;
	సమితి_task_rq(p, task_cpu(p));
	attach_task_cfs_rq(p);
}

శూన్య free_fair_sched_సమూహం(నిర్మాణం task_సమూహం *tg)
{
	free_percpu(tg->cfs_rq);
}

పూర్ణాంకం alloc_fair_sched_సమూహం(నిర్మాణం task_సమూహం *tg, నిర్మాణం task_సమూహం *మాతృ)
{
	నిర్మాణం cfs_tg_state __percpu *state;
	నిర్మాణం sched_entity *se;
	నిర్మాణం cfs_rq *cfs_rq;
	పూర్ణాంకం i;

	state = alloc_percpu_gfp(నిర్మాణం cfs_tg_state, GFP_KERNEL);
	ఒకవేళ (!state)
		వెళ్ళు err;

	tg->cfs_rq = &state->cfs_rq;
	tg->shares = NICE_0_ఎక్కించు;

	init_cfs_bandwidth(tg_cfs_bandwidth(tg), tg_cfs_bandwidth(మాతృ));

	for_each_possible_cpu(i) {
		cfs_rq = tg_cfs_rq(tg, i);
		ఒకవేళ (!cfs_rq)
			వెళ్ళు err;

		se = tg_se(tg, i);
		init_cfs_rq(cfs_rq);
		init_tg_cfs_నమోదు(tg, cfs_rq, se, i, tg_se(మాతృ, i));
		init_entity_runnable_సరాసరి(se);
	}

	తిరిగి 1;

err:
	తిరిగి 0;
}

శూన్య online_fair_sched_సమూహం(నిర్మాణం task_సమూహం *tg)
{
	నిర్మాణం sched_entity *se;
	నిర్మాణం rq_flags rf;
	నిర్మాణం rq *rq;
	పూర్ణాంకం i;

	for_each_possible_cpu(i) {
		rq = cpu_rq(i);
		se = tg_se(tg, i);
		rq_తాళం_irq(rq, &rf);
		update_rq_clock(rq);
		attach_entity_cfs_rq(se);
		sync_throttle(tg, i);
		rq_unlock_irq(rq, &rf);
	}
}

శూన్య unregister_fair_sched_సమూహం(నిర్మాణం task_సమూహం *tg)
{
	పూర్ణాంకం cpu;

	destroy_cfs_bandwidth(tg_cfs_bandwidth(tg));

	for_each_possible_cpu(cpu) {
		నిర్మాణం cfs_rq *cfs_rq = tg_cfs_rq(tg, cpu);
		నిర్మాణం sched_entity *se = tg_se(tg, cpu);
		నిర్మాణం rq *rq = cpu_rq(cpu);

		ఒకవేళ (se)
			remove_entity_ఎక్కించు_avg(se);

		/*
		 * Only empty task groups can be destroyed; so we can speculatively
		 * check on_list without danger of it being re-added.
		 */
		ఒకవేళ (cfs_rq->on_list) {
			guard(rq_తాళం_irqsave)(rq);
			list_del_ఆకు_cfs_rq(cfs_rq);
		}
	}
}

శూన్య init_tg_cfs_నమోదు(నిర్మాణం task_సమూహం *tg, నిర్మాణం cfs_rq *cfs_rq,
			నిర్మాణం sched_entity *se, పూర్ణాంకం cpu,
			నిర్మాణం sched_entity *మాతృ)
{
	నిర్మాణం rq *rq = cpu_rq(cpu);

	cfs_rq->tg = tg;
	cfs_rq->rq = rq;
	init_cfs_rq_runtime(cfs_rq);

	/* se could be NULL for root_task_group */
	ఒకవేళ (!se)
		తిరిగి;

	ఒకవేళ (!మాతృ) {
		se->cfs_rq = &rq->cfs;
		se->లోతు = 0;
	} లేకపోతే {
		se->cfs_rq = మాతృ->my_q;
		se->లోతు = మాతృ->లోతు + 1;
	}

	se->my_q = cfs_rq;
	/* guarantee group entities always have weight */
	update_ఎక్కించు_సమితి(&se->ఎక్కించు, NICE_0_ఎక్కించు);
	se->మాతృ = మాతృ;
}

స్థిర DEFINE_MUTEX(shares_mutex);

స్థిర పూర్ణాంకం __sched_group_set_shares(నిర్మాణం task_సమూహం *tg, గుర్తులేని దీర్ఘ shares)
{
	పూర్ణాంకం i;

	lockdep_assert_held(&shares_mutex);

	/*
	 * We can't change the weight of the root cgroup.
	 */
	ఒకవేళ (is_మూలం_task_సమూహం(tg))
		తిరిగి -EINVAL;

	shares = clamp(shares, scale_ఎక్కించు(MIN_SHARES), scale_ఎక్కించు(MAX_SHARES));

	ఒకవేళ (tg->shares == shares)
		తిరిగి 0;

	tg->shares = shares;
	for_each_possible_cpu(i) {
		నిర్మాణం rq *rq = cpu_rq(i);
		నిర్మాణం sched_entity *se = tg_se(tg, i);
		నిర్మాణం rq_flags rf;

		/* Propagate contribution to hierarchy */
		rq_తాళం_irqsave(rq, &rf);
		update_rq_clock(rq);
		for_each_sched_entity(se) {
			update_ఎక్కించు_avg(cfs_rq_యొక్క(se), se, UPDATE_TG);
			update_cfs_సమూహం(se);
		}
		rq_unlock_irqrestore(rq, &rf);
	}

	తిరిగి 0;
}

పూర్ణాంకం sched_సమూహం_సమితి_shares(నిర్మాణం task_సమూహం *tg, గుర్తులేని దీర్ఘ shares)
{
	పూర్ణాంకం ret;

	mutex_తాళం(&shares_mutex);
	ఒకవేళ (tg_is_idle(tg))
		ret = -EINVAL;
	లేకపోతే
		ret = __sched_group_set_shares(tg, shares);
	mutex_unlock(&shares_mutex);

	తిరిగి ret;
}

పూర్ణాంకం sched_సమూహం_సమితి_idle(నిర్మాణం task_సమూహం *tg, దీర్ఘ idle)
{
	పూర్ణాంకం i;

	ఒకవేళ (tg == &మూలం_task_సమూహం)
		తిరిగి -EINVAL;

	ఒకవేళ (idle < 0 || idle > 1)
		తిరిగి -EINVAL;

	mutex_తాళం(&shares_mutex);

	ఒకవేళ (tg->idle == idle) {
		mutex_unlock(&shares_mutex);
		తిరిగి 0;
	}

	tg->idle = idle;

	for_each_possible_cpu(i) {
		నిర్మాణం rq *rq = cpu_rq(i);
		నిర్మాణం sched_entity *se = tg_se(tg, i);
		నిర్మాణం cfs_rq *grp_cfs_rq = tg_cfs_rq(tg, i);
		bool was_idle = cfs_rq_is_idle(grp_cfs_rq);
		దీర్ఘ idle_task_delta;
		నిర్మాణం rq_flags rf;

		rq_తాళం_irqsave(rq, &rf);

		grp_cfs_rq->idle = idle;
		ఒకవేళ (WARN_ON_ONCE(was_idle == cfs_rq_is_idle(grp_cfs_rq)))
			వెళ్ళు తదుపరి_cpu;

		idle_task_delta = grp_cfs_rq->h_nr_queued -
				  grp_cfs_rq->h_nr_idle;
		ఒకవేళ (!cfs_rq_is_idle(grp_cfs_rq))
			idle_task_delta *= -1;

		for_each_sched_entity(se) {
			నిర్మాణం cfs_rq *cfs_rq = cfs_rq_యొక్క(se);

			ఒకవేళ (!se->on_rq)
				ఆపు;

			cfs_rq->h_nr_idle += idle_task_delta;

			/* Already accounted at parent level and above. */
			ఒకవేళ (cfs_rq_is_idle(cfs_rq))
				ఆపు;
		}

తదుపరి_cpu:
		rq_unlock_irqrestore(rq, &rf);
	}

	/* Idle groups have minimum weight. */
	ఒకవేళ (tg_is_idle(tg))
		__sched_group_set_shares(tg, scale_ఎక్కించు(బరువు_IDLEPRIO));
	లేకపోతే
		__sched_group_set_shares(tg, NICE_0_ఎక్కించు);

	mutex_unlock(&shares_mutex);
	తిరిగి 0;
}

#అయితే_ముగింపు /* CONFIG_FAIR_GROUP_SCHED */


స్థిర గుర్తులేని పూర్ణాంకం get_rr_interval_fair(నిర్మాణం rq *rq, నిర్మాణం task_struct *task)
{
	నిర్మాణం sched_entity *se = &task->se;
	గుర్తులేని పూర్ణాంకం rr_interval = 0;

	/*
	 * Time slice is 0 for SCHED_OTHER tasks that are on an otherwise
	 * idle runqueue:
	 */
	ఒకవేళ (rq->cfs.ఎక్కించు.బరువు)
		rr_interval = NS_గమ్యం_JIFFIES(se->ముక్క);

	తిరిగి rr_interval;
}

/*
 * All the scheduling class methods:
 */
DEFINE_SCHED_CLASS(fair) = {
	.వరుసలో_చేర్చు_task		= వరుసలో_చేర్చు_task_fair,
	.వరుస_నుండి_తీయి_task		= వరుస_నుండి_తీయి_task_fair,
	.yield_task		= yield_task_fair,
	.yield_గమ్యం_task		= yield_గమ్యం_task_fair,

	.wakeup_preempt		= wakeup_preempt_fair,

	.pick_task		= pick_task_fair,
	.put_prev_task		= put_prev_task_fair,
	.సమితి_తదుపరి_task          = సమితి_తదుపరి_task_fair,

	.select_task_rq		= select_task_rq_fair,
	.migrate_task_rq	= migrate_task_rq_fair,

	.rq_online		= rq_online_fair,
	.rq_offline		= rq_offline_fair,

	.task_dead		= task_dead_fair,
	.సమితి_cpus_allowed	= సమితి_cpus_allowed_fair,

	.task_tick		= task_tick_fair,
	.task_fork		= task_fork_fair,

	.reweight_task		= reweight_task_fair,
	.prio_మారింది		= prio_మారింది_fair,
	.switching_from		= switching_from_fair,
	.switched_from		= switched_from_fair,
	.switched_గమ్యం		= switched_గమ్యం_fair,

	.get_rr_interval	= get_rr_interval_fair,

	.update_curr		= update_curr_fair,

#నిర్వచితమైతే CONFIG_FAIR_సమూహం_SCHED
	.task_change_సమూహం	= task_change_సమూహం_fair,
#అయితే_ముగింపు

#నిర్వచితమైతే CONFIG_SCHED_CORE
	.task_is_throttled	= task_is_throttled_fair,
#అయితే_ముగింపు

#నిర్వచితమైతే CONFIG_UCLAMP_TASK
	.uclamp_enabled		= 1,
#అయితే_ముగింపు
};

శూన్య print_cfs_stats(నిర్మాణం seq_file *m, పూర్ణాంకం cpu)
{
	నిర్మాణం cfs_rq *cfs_rq, *pos;

	rcu_read_తాళం();
	for_each_ఆకు_cfs_rq_సురక్షిత(cpu_rq(cpu), cfs_rq, pos)
		print_cfs_rq(m, cpu, cfs_rq);
	rcu_read_unlock();
}

#నిర్వచితమైతే CONFIG_NUMA_BALANCING
శూన్య show_numa_stats(నిర్మాణం task_struct *p, నిర్మాణం seq_file *m)
{
	పూర్ణాంకం కణుపు;
	గుర్తులేని దీర్ఘ tsf = 0, tpf = 0, gsf = 0, gpf = 0;
	నిర్మాణం numa_సమూహం *ng;

	rcu_read_తాళం();
	ng = rcu_dereference_all(p->numa_సమూహం);
	for_each_online_కణుపు(కణుపు) {
		ఒకవేళ (p->numa_దోషాలు) {
			tsf = p->numa_దోషాలు[task_దోషాలు_idx(NUMA_MEM, కణుపు, 0)];
			tpf = p->numa_దోషాలు[task_దోషాలు_idx(NUMA_MEM, కణుపు, 1)];
		}
		ఒకవేళ (ng) {
			gsf = ng->దోషాలు[task_దోషాలు_idx(NUMA_MEM, కణుపు, 0)];
			gpf = ng->దోషాలు[task_దోషాలు_idx(NUMA_MEM, కణుపు, 1)];
		}
		print_numa_stats(m, కణుపు, tsf, tpf, gsf, gpf);
	}
	rcu_read_unlock();
}
#అయితే_ముగింపు /* CONFIG_NUMA_BALANCING */

__init శూన్య init_sched_fair_class(శూన్య)
{
	పూర్ణాంకం i;

	for_each_possible_cpu(i) {
		zalloc_cpumask_var_కణుపు(&చొప్పున_cpu(ఎక్కించు_balance_ముసుగు, i), GFP_KERNEL, cpu_గమ్యం_కణుపు(i));
		zalloc_cpumask_var_కణుపు(&చొప్పున_cpu(select_rq_ముసుగు,    i), GFP_KERNEL, cpu_గమ్యం_కణుపు(i));
		zalloc_cpumask_var_కణుపు(&చొప్పున_cpu(should_we_balance_tmpmask, i),
					GFP_KERNEL, cpu_గమ్యం_కణుపు(i));

#నిర్వచితమైతే CONFIG_CFS_BANDWIDTH
		INIT_CSD(&cpu_rq(i)->cfsb_csd, __cfsb_csd_unthrottle, cpu_rq(i));
		INIT_LIST_తల(&cpu_rq(i)->cfsb_csd_list);
#అయితే_ముగింపు
	}

	open_softirq(SCHED_SOFTIRQ, sched_balance_softirq);

#నిర్వచితమైతే CONFIG_NO_HZ_COMMON
	nohz.తదుపరి_balance = jiffies;
	nohz.తదుపరి_blocked = jiffies;
	zalloc_cpumask_var(&nohz.idle_cpus_ముసుగు, GFP_NOWAIT);
#అయితే_ముగింపు
}
