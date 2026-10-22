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
#ಅನ್ತರ್ಭಾವ <linux/energy_model.h>
#ಅನ್ತರ್ಭಾವ <linux/mmap_lock.h>
#ಅನ್ತರ್ಭಾವ <linux/hugetlb_inline.h>
#ಅನ್ತರ್ಭಾವ <linux/jiffies.h>
#ಅನ್ತರ್ಭಾವ <linux/math.h>
#ಅನ್ತರ್ಭಾವ <linux/mm_api.h>
#ಅನ್ತರ್ಭಾವ <linux/highmem.h>
#ಅನ್ತರ್ಭಾವ <linux/hrtimer.h>
#ಅನ್ತರ್ಭಾವ <linux/hrtimer_bases.h>
#ಅನ್ತರ್ಭಾವ <linux/spinlock_api.h>
#ಅನ್ತರ್ಭಾವ <linux/cpumask_api.h>
#ಅನ್ತರ್ಭಾವ <linux/lockdep_api.h>
#ಅನ್ತರ್ಭಾವ <linux/softirq.h>
#ಅನ್ತರ್ಭಾವ <linux/refcount_api.h>
#ಅನ್ತರ್ಭಾವ <linux/topology.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/clock.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/cond_resched.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/cputime.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/isolation.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/nohz.h>
#ಅನ್ತರ್ಭಾವ <linux/sched/prio.h>
#ಅನ್ತರ್ಭಾವ <linux/static_call.h>

#ಅನ್ತರ್ಭಾವ <linux/cpuidle.h>
#ಅನ್ತರ್ಭಾವ <linux/interrupt.h>
#ಅನ್ತರ್ಭಾವ <linux/memory-tiers.h>
#ಅನ್ತರ್ಭಾವ <linux/mempolicy.h>
#ಅನ್ತರ್ಭಾವ <linux/mutex_api.h>
#ಅನ್ತರ್ಭಾವ <linux/profile.h>
#ಅನ್ತರ್ಭಾವ <linux/psi.h>
#ಅನ್ತರ್ಭಾವ <linux/ratelimit.h>
#ಅನ್ತರ್ಭಾವ <linux/task_work.h>
#ಅನ್ತರ್ಭಾವ <linux/rbtree_augmented.h>

#ಅನ್ತರ್ಭಾವ <asm/switch_to.h>

#ಅನ್ತರ್ಭಾವ <uapi/linux/sched/types.h>

#ಅನ್ತರ್ಭಾವ "sched.h"
#ಅನ್ತರ್ಭಾವ "stats.h"
#ಅನ್ತರ್ಭಾವ "autogroup.h"

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
ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ sysctl_sched_tunable_scaling = SCHED_TUNABLESCALING_LOG;

/*
 * Default base time slice (request size r_i) for SCHED_NORMAL/SCHED_BATCH:
 *
 * Under EEVDF this is the request size used to compute the virtual
 * deadline; see update_deadline().
 *
 * (default: 0.70 msec * (1 + ilog(ncpus)), units: nanoseconds)
 */
ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ sysctl_sched_ತಳ_ತುಂಡು			= 700000ULL;
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ normalized_sysctl_sched_ತಳ_ತುಂಡು	= 700000ULL;

__read_mostly ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ sysctl_sched_migration_ವೆಚ್ಚ	= 500000UL;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ __init ಸಿದ್ಧತೆ_sched_thermal_decay_ಸರಿಸು(ಅಕ್ಷರ *str)
{
	pr_warn("Ignoring the deprecated sched_thermal_decay_shift= option\n");
	ಹಿಂತಿರುಗಿ 1;
}
__setup("sched_thermal_decay_shift=", ಸಿದ್ಧತೆ_sched_thermal_decay_ಸರಿಸು);

/*
 * For asym packing, by default the lower numbered CPU has higher priority.
 */
ಪೂರ್ಣಾಂಕ __weak arch_asym_cpu_ಆದ್ಯತೆ(ಪೂರ್ಣಾಂಕ cpu)
{
	ಹಿಂತಿರುಗಿ -cpu;
}

/*
 * The margin used when comparing utilization with CPU capacity.
 *
 * (default: ~20%)
 */
#ವ್ಯಾಖ್ಯಾನಿಸು ಹೊಂದುತ್ತದೆ_ಸಾಮರ್ಥ್ಯ(cap, max)	((cap) * 1280 < (max) * 1024)

/*
 * The margin used when comparing CPU capacities.
 * is 'cap1' noticeably greater than 'cap2'
 *
 * (default: ~5%)
 */
#ವ್ಯಾಖ್ಯಾನಿಸು ಸಾಮರ್ಥ್ಯ_greater(cap1, cap2) ((cap1) * 1024 > (cap2) * 1078)

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_CFS_BANDWIDTH
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
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ sysctl_sched_cfs_bandwidth_ತುಂಡು		= 5000UL;
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_NUMA_BALANCING
/* Restrict the NUMA promotion throughput (MB/s) for each target node. */
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ sysctl_numa_balancing_promote_ದರ_ಎಲ್ಲೆ = 65536;
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SYSCTL
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ ctl_ಕೋಷ್ಟಕ sched_fair_sysctls[] = {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_CFS_BANDWIDTH
	{
		.procname       = "sched_cfs_bandwidth_slice_us",
		.ದತ್ತಾಂಶ           = &sysctl_sched_cfs_bandwidth_ತುಂಡು,
		.maxlen         = ಗಾತ್ರ(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ),
		.ಬಹುಲಕ           = 0644,
		.proc_handler   = proc_dointvec_minmax,
		.extra1         = SYSCTL_ONE,
	},
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_NUMA_BALANCING
	{
		.procname	= "numa_balancing_promote_rate_limit_MBps",
		.ದತ್ತಾಂಶ		= &sysctl_numa_balancing_promote_ದರ_ಎಲ್ಲೆ,
		.maxlen		= ಗಾತ್ರ(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ),
		.ಬಹುಲಕ		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= SYSCTL_ZERO,
	},
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_NUMA_BALANCING */
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ __init sched_fair_sysctl_init(ಶೂನ್ಯ)
{
	ರಿಜಿಸ್ಟರ್_sysctl_init("kernel", sched_fair_sysctls);
	ಹಿಂತಿರುಗಿ 0;
}
late_initcall(sched_fair_sysctl_init);
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_SYSCTL */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ update_ತುಂಬು_ಕೂಡಿಸು(ರಚನೆ ತುಂಬು_ತೂಕ *lw, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ inc)
{
	lw->ತೂಕ += inc;
	lw->inv_ತೂಕ = 0;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ update_ತುಂಬು_sub(ರಚನೆ ತುಂಬು_ತೂಕ *lw, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ dec)
{
	lw->ತೂಕ -= dec;
	lw->inv_ತೂಕ = 0;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ update_ತುಂಬು_ಗಣ(ರಚನೆ ತುಂಬು_ತೂಕ *lw, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ w)
{
	lw->ತೂಕ = w;
	lw->inv_ತೂಕ = 0;
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
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ get_update_sysctl_ಗುಣಕ(ಶೂನ್ಯ)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cpus = min_t(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ, ಅಂಕೆ_online_cpus(), 8);
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಗುಣಕ;

	ಆಯ್ಕೆ (sysctl_sched_tunable_scaling) {
	ಸ್ಥಿತಿ SCHED_TUNABLESCALING_NONE:
		ಗುಣಕ = 1;
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ SCHED_TUNABLESCALING_ರೇಖೀಯ:
		ಗುಣಕ = cpus;
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ SCHED_TUNABLESCALING_LOG:
	ಪೂರ್ವನಿಯೋಜಿತ:
		ಗುಣಕ = 1 + ilog2(cpus);
		ನಿಲ್ಲಿಸು;
	}

	ಹಿಂತಿರುಗಿ ಗುಣಕ;
}

ಸ್ಥಿರ ಶೂನ್ಯ update_sysctl(ಶೂನ್ಯ)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಗುಣಕ = get_update_sysctl_ಗುಣಕ();

#ವ್ಯಾಖ್ಯಾನಿಸು ಗಣ_SYSCTL(name) \
	(sysctl_##name = (ಗುಣಕ) * normalized_sysctl_##name)
	ಗಣ_SYSCTL(sched_ತಳ_ತುಂಡು);
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು ಗಣ_SYSCTL
}

ಶೂನ್ಯ __init sched_init_granularity(ಶೂನ್ಯ)
{
	update_sysctl();
}

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CONFIG_64_ಬಿಟ್
#ವ್ಯಾಖ್ಯಾನಿಸು WMULT_CONST	(~0U)
#ವ್ಯಾಖ್ಯಾನಿಸು WMULT_ಸರಿಸು	32

ಸ್ಥಿರ ಶೂನ್ಯ __update_inv_weight(ರಚನೆ ತುಂಬು_ತೂಕ *lw)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ w;

	ಆದರೆ (likely(lw->inv_ತೂಕ))
		ಹಿಂತಿರುಗಿ;

	w = scale_ತುಂಬು_ಕೆಳಕ್ಕೆ(lw->ತೂಕ);

	ಆದರೆ (ಬಿಟ್ಟುಗಳು_ಪ್ರತಿ_LONG > 32 && unlikely(w >= WMULT_CONST))
		lw->inv_ತೂಕ = 1;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (unlikely(!w))
		lw->inv_ತೂಕ = WMULT_CONST;
	ಇಲ್ಲದಿದ್ದರೆ
		lw->inv_ತೂಕ = WMULT_CONST / w;
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
ಸ್ಥಿರ u64 __calc_delta(u64 delta_exec, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ತೂಕ, ರಚನೆ ತುಂಬು_ತೂಕ *lw)
{
	u64 fact = scale_ತುಂಬು_ಕೆಳಕ್ಕೆ(ತೂಕ);
	u32 fact_hi = (u32)(fact >> 32);
	ಪೂರ್ಣಾಂಕ ಸರಿಸು = WMULT_ಸರಿಸು;
	ಪೂರ್ಣಾಂಕ fs;

	__update_inv_weight(lw);

	ಆದರೆ (unlikely(fact_hi)) {
		fs = fls(fact_hi);
		ಸರಿಸು -= fs;
		fact >>= fs;
	}

	fact = mul_u32_u32(fact, lw->inv_ತೂಕ);

	fact_hi = (u32)(fact >> 32);
	ಆದರೆ (fact_hi) {
		fs = fls(fact_hi);
		ಸರಿಸು -= fs;
		fact >>= fs;
	}

	ಹಿಂತಿರುಗಿ mul_u64_u32_shr(delta_exec, fact, ಸರಿಸು);
}
#ಇಲ್ಲದಿದ್ದರೆ
ಸ್ಥಿರ u64 __calc_delta(u64 delta_exec, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ತೂಕ, ರಚನೆ ತುಂಬು_ತೂಕ *lw)
{
	ಹಿಂತಿರುಗಿ (delta_exec * ತೂಕ) / lw->ತೂಕ;
}
#ಆದರೆ_ಅಂತ್ಯ

/*
 * delta /= w
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ u64 calc_delta_fair(u64 delta, ರಚನೆ sched_entity *se)
{
	ಆದರೆ (se->h_ತುಂಬು.ತೂಕ != NICE_0_ತುಂಬು)
		delta = __calc_delta(delta, NICE_0_ತುಂಬು, &se->h_ತುಂಬು);

	ಹಿಂತಿರುಗಿ delta;
}

ಸ್ಥಿರಾಂಕ ರಚನೆ sched_class fair_sched_class;

/**************************************************************
 * CFS operations on generic schedulable entities:
 */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_FAIR_ಗುಂಪು_SCHED

/* Walk up scheduling entities hierarchy */
#ವ್ಯಾಖ್ಯಾನಿಸು for_each_sched_entity(se) \
		ಗಾಗಿ (; se; se = se->ಪೋಷಕ)

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool list_ಕೂಡಿಸು_ಎಲೆ_cfs_rq(ರಚನೆ cfs_rq *cfs_rq)
{
	ರಚನೆ rq *rq = rq_ದ(cfs_rq);
	ಪೂರ್ಣಾಂಕ cpu = cpu_ದ(rq);

	ಆದರೆ (cfs_rq->on_list)
		ಹಿಂತಿರುಗಿ rq->tmp_alone_branch == &rq->ಎಲೆ_cfs_rq_list;

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
	ಆದರೆ (cfs_rq->tg->ಪೋಷಕ &&
	    tg_cfs_rq(cfs_rq->tg->ಪೋಷಕ, cpu)->on_list) {
		/*
		 * If parent is already on the list, we add the child
		 * just before. Thanks to circular linked property of
		 * the list, this means to put the child at the tail
		 * of the list that starts by parent.
		 */
		list_ಕೂಡಿಸು_ಬಾಲ_rcu(&cfs_rq->ಎಲೆ_cfs_rq_list,
			&(tg_cfs_rq(cfs_rq->tg->ಪೋಷಕ, cpu)->ಎಲೆ_cfs_rq_list));
		/*
		 * The branch is now connected to its tree so we can
		 * reset tmp_alone_branch to the beginning of the
		 * list.
		 */
		rq->tmp_alone_branch = &rq->ಎಲೆ_cfs_rq_list;
		ಹಿಂತಿರುಗಿ true;
	}

	ಆದರೆ (!cfs_rq->tg->ಪೋಷಕ) {
		/*
		 * cfs rq without parent should be put
		 * at the tail of the list.
		 */
		list_ಕೂಡಿಸು_ಬಾಲ_rcu(&cfs_rq->ಎಲೆ_cfs_rq_list,
			&rq->ಎಲೆ_cfs_rq_list);
		/*
		 * We have reach the top of a tree so we can reset
		 * tmp_alone_branch to the beginning of the list.
		 */
		rq->tmp_alone_branch = &rq->ಎಲೆ_cfs_rq_list;
		ಹಿಂತಿರುಗಿ true;
	}

	/*
	 * The parent has not already been added so we want to
	 * make sure that it will be put after us.
	 * tmp_alone_branch points to the begin of the branch
	 * where we will add parent.
	 */
	list_ಕೂಡಿಸು_rcu(&cfs_rq->ಎಲೆ_cfs_rq_list, rq->tmp_alone_branch);
	/*
	 * update tmp_alone_branch to points to the new begin
	 * of the branch
	 */
	rq->tmp_alone_branch = &cfs_rq->ಎಲೆ_cfs_rq_list;
	ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ list_del_ಎಲೆ_cfs_rq(ರಚನೆ cfs_rq *cfs_rq)
{
	ಆದರೆ (cfs_rq->on_list) {
		ರಚನೆ rq *rq = rq_ದ(cfs_rq);

		/*
		 * With cfs_rq being unthrottled/throttled during an enqueue,
		 * it can happen the tmp_alone_branch points to the leaf that
		 * we finally want to delete. In this case, tmp_alone_branch moves
		 * to the prev element but it will point to rq->leaf_cfs_rq_list
		 * at the end of the enqueue.
		 */
		ಆದರೆ (rq->tmp_alone_branch == &cfs_rq->ಎಲೆ_cfs_rq_list)
			rq->tmp_alone_branch = cfs_rq->ಎಲೆ_cfs_rq_list.prev;

		list_del_rcu(&cfs_rq->ಎಲೆ_cfs_rq_list);
		cfs_rq->on_list = 0;
	}
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ assert_list_ಎಲೆ_cfs_rq(ರಚನೆ rq *rq)
{
	WARN_ON_ONCE(rq->tmp_alone_branch != &rq->ಎಲೆ_cfs_rq_list);
}

/* Iterate through all leaf cfs_rq's on a runqueue */
#ವ್ಯಾಖ್ಯಾನಿಸು for_each_ಎಲೆ_cfs_rq_ಸುರಕ್ಷಿತ(rq, cfs_rq, pos)			\
	list_for_each_ನಮೂದು_ಸುರಕ್ಷಿತ(cfs_rq, pos, &rq->ಎಲೆ_cfs_rq_list,	\
				 ಎಲೆ_cfs_rq_list)

/* Do the two (enqueued) entities belong to the same group ? */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ cfs_rq *
is_same_ಗುಂಪು(ರಚನೆ sched_entity *se, ರಚನೆ sched_entity *pse)
{
	ಆದರೆ (se->cfs_rq == pse->cfs_rq)
		ಹಿಂತಿರುಗಿ se->cfs_rq;

	ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ sched_entity *ಪೋಷಕ_entity(ಸ್ಥಿರಾಂಕ ರಚನೆ sched_entity *se)
{
	ಹಿಂತಿರುಗಿ se->ಪೋಷಕ;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ tg_is_idle(ರಚನೆ task_ಗುಂಪು *tg)
{
	ಹಿಂತಿರುಗಿ tg->idle > 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cfs_rq_is_idle(ರಚನೆ cfs_rq *cfs_rq)
{
	ಹಿಂತಿರುಗಿ cfs_rq->idle > 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ se_is_idle(ರಚನೆ sched_entity *se)
{
	ಆದರೆ (entity_is_task(se))
		ಹಿಂತಿರುಗಿ task_has_idle_policy(task_ದ(se));
	ಹಿಂತಿರುಗಿ cfs_rq_is_idle(ಗುಂಪು_cfs_rq(se));
}

#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_FAIR_GROUP_SCHED: */

#ವ್ಯಾಖ್ಯಾನಿಸು for_each_sched_entity(se) \
		ಗಾಗಿ (; se; se = ಖಾಲಿ)

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool list_ಕೂಡಿಸು_ಎಲೆ_cfs_rq(ರಚನೆ cfs_rq *cfs_rq)
{
	ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ list_del_ಎಲೆ_cfs_rq(ರಚನೆ cfs_rq *cfs_rq)
{
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ assert_list_ಎಲೆ_cfs_rq(ರಚನೆ rq *rq)
{
}

#ವ್ಯಾಖ್ಯಾನಿಸು for_each_ಎಲೆ_cfs_rq_ಸುರಕ್ಷಿತ(rq, cfs_rq, pos)	\
		ಗಾಗಿ (cfs_rq = &rq->cfs, pos = ಖಾಲಿ; cfs_rq; cfs_rq = pos)

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ sched_entity *ಪೋಷಕ_entity(ರಚನೆ sched_entity *se)
{
	ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ tg_is_idle(ರಚನೆ task_ಗುಂಪು *tg)
{
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ cfs_rq_is_idle(ರಚನೆ cfs_rq *cfs_rq)
{
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ se_is_idle(ರಚನೆ sched_entity *se)
{
	ಹಿಂತಿರುಗಿ task_has_idle_policy(task_ದ(se));
}

#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_FAIR_GROUP_SCHED */

ಸ್ಥಿರ __always_inline
bool account_cfs_rq_runtime(ರಚನೆ cfs_rq *cfs_rq, u64 delta_exec);

/**************************************************************
 * Scheduling class tree data structure manipulation methods:
 */

ಬಾಹ್ಯ ಶೂನ್ಯ __BUILD_BUG_vruntime_cmp(ಶೂನ್ಯ);

/* Use __builtin_strcmp() because of __HAVE_ARCH_STRCMP: */

#ವ್ಯಾಖ್ಯಾನಿಸು vruntime_cmp(A, CMP_STR, B) ({				\
	ಪೂರ್ಣಾಂಕ __res = 0;						\
								\
	ಆದರೆ (!__builtin_strcmp(CMP_STR, "<")) {			\
		__res = ((s64)((A)-(B)) < 0);			\
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!__builtin_strcmp(CMP_STR, "<=")) {		\
		__res = ((s64)((A)-(B)) <= 0);			\
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!__builtin_strcmp(CMP_STR, ">")) {		\
		__res = ((s64)((A)-(B)) > 0);			\
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!__builtin_strcmp(CMP_STR, ">=")) {		\
		__res = ((s64)((A)-(B)) >= 0);			\
	} ಇಲ್ಲದಿದ್ದರೆ {						\
		/* Unknown operator throws linker error: */	\
		__BUILD_BUG_vruntime_cmp();			\
	}							\
								\
	__res;							\
})

ಬಾಹ್ಯ ಶೂನ್ಯ __BUILD_BUG_vruntime_op(ಶೂನ್ಯ);

#ವ್ಯಾಖ್ಯಾನಿಸು vruntime_op(A, OP_STR, B) ({				\
	s64 __res = 0;						\
								\
	ಆದರೆ (!__builtin_strcmp(OP_STR, "-")) {			\
		__res = (s64)((A)-(B));				\
	} ಇಲ್ಲದಿದ್ದರೆ {						\
		/* Unknown operator throws linker error: */	\
		__BUILD_BUG_vruntime_op();			\
	}							\
								\
	__res;						\
})


ಸ್ಥಿರ ಸಾಲೊಳಗೆ __maybe_unused u64 max_vruntime(u64 max_vruntime, u64 vruntime)
{
	ಆದರೆ (vruntime_cmp(vruntime, ">", max_vruntime))
		max_vruntime = vruntime;

	ಹಿಂತಿರುಗಿ max_vruntime;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ __maybe_unused u64 min_vruntime(u64 min_vruntime, u64 vruntime)
{
	ಆದರೆ (vruntime_cmp(vruntime, "<", min_vruntime))
		min_vruntime = vruntime;

	ಹಿಂತಿರುಗಿ min_vruntime;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool entity_before(ಸ್ಥಿರಾಂಕ ರಚನೆ sched_entity *a,
				 ಸ್ಥಿರಾಂಕ ರಚನೆ sched_entity *b)
{
	/*
	 * Tiebreak on vruntime seems unnecessary since it can
	 * hardly happen.
	 */
	ಹಿಂತಿರುಗಿ vruntime_cmp(a->deadline, "<", b->deadline);
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
ಸ್ಥಿರ ಸಾಲೊಳಗೆ s64 entity_ಕೀಲಿ(ರಚನೆ cfs_rq *cfs_rq, ರಚನೆ sched_entity *se)
{
	ಹಿಂತಿರುಗಿ vruntime_op(se->vruntime, "-", cfs_rq->zero_vruntime);
}

#ವ್ಯಾಖ್ಯಾನಿಸು __node_2_se(ಗಂಟು) \
	rb_ನಮೂದು((ಗಂಟು), ರಚನೆ sched_entity, ಸತತ_ಗಂಟು)

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
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ avg_vruntime_ತೂಕ(ರಚನೆ cfs_rq *cfs_rq, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ w)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_64_ಬಿಟ್
	ಆದರೆ (cfs_rq->ಮೊತ್ತ_ಸರಿಸು)
		w = max(2UL, w >> cfs_rq->ಮೊತ್ತ_ಸರಿಸು);
#ಆದರೆ_ಅಂತ್ಯ
	ಹಿಂತಿರುಗಿ w;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
__sum_w_vruntime_add(ರಚನೆ cfs_rq *cfs_rq, ರಚನೆ sched_entity *se)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ತೂಕ = avg_vruntime_ತೂಕ(cfs_rq, se->h_ತುಂಬು.ತೂಕ);
	s64 w_vruntime, ಕೀಲಿ = entity_ಕೀಲಿ(cfs_rq, se);

	w_vruntime = ಕೀಲಿ * ತೂಕ;
	WARN_ON_ONCE((w_vruntime >> 63) != (w_vruntime >> 62));

	cfs_rq->ಮೊತ್ತ_w_vruntime += w_vruntime;
	cfs_rq->ಮೊತ್ತ_ತೂಕ += ತೂಕ;
}

ಸ್ಥಿರ ಶೂನ್ಯ
ಮೊತ್ತ_w_vruntime_ಕೂಡಿಸು_paranoid(ರಚನೆ cfs_rq *cfs_rq, ರಚನೆ sched_entity *se)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ತೂಕ;
	s64 ಕೀಲಿ, tmp;

again:
	ತೂಕ = avg_vruntime_ತೂಕ(cfs_rq, se->h_ತುಂಬು.ತೂಕ);
	ಕೀಲಿ = entity_ಕೀಲಿ(cfs_rq, se);

	ಆದರೆ (check_mul_ಅತಿಪ್ರವಾಹ(ಕೀಲಿ, ತೂಕ, &ಕೀಲಿ))
		ಹೋಗು ಅತಿಪ್ರವಾಹ;

	ಆದರೆ (check_ಕೂಡಿಸು_ಅತಿಪ್ರವಾಹ(cfs_rq->ಮೊತ್ತ_w_vruntime, ಕೀಲಿ, &tmp))
		ಹೋಗು ಅತಿಪ್ರವಾಹ;

	cfs_rq->ಮೊತ್ತ_w_vruntime = tmp;
	cfs_rq->ಮೊತ್ತ_ತೂಕ += ತೂಕ;
	ಹಿಂತಿರುಗಿ;

ಅತಿಪ್ರವಾಹ:
	/*
	 * There's gotta be a limit -- if we're still failing at this point
	 * there's really nothing much to be done about things.
	 */
	BUG_ON(cfs_rq->ಮೊತ್ತ_ಸರಿಸು >= 10);
	cfs_rq->ಮೊತ್ತ_ಸರಿಸು++;

	/*
	 * Note: \Sum (k_i * (w_i >> 1)) != (\Sum (k_i * w_i)) >> 1
	 */
	cfs_rq->ಮೊತ್ತ_w_vruntime = 0;
	cfs_rq->ಮೊತ್ತ_ತೂಕ = 0;

	ಗಾಗಿ (ರಚನೆ rb_ಗಂಟು *ಗಂಟು = cfs_rq->tasks_timeline.rb_leftmost;
	     ಗಂಟು; ಗಂಟು = rb_ಮುಂದಿನ(ಗಂಟು))
		__sum_w_vruntime_add(cfs_rq, __node_2_se(ಗಂಟು));

	ಹೋಗು again;
}

ಸ್ಥಿರ ಶೂನ್ಯ
ಮೊತ್ತ_w_vruntime_ಕೂಡಿಸು(ರಚನೆ cfs_rq *cfs_rq, ರಚನೆ sched_entity *se)
{
	ಆದರೆ (sched_feat(PARANOID_AVG))
		ಹಿಂತಿರುಗಿ ಮೊತ್ತ_w_vruntime_ಕೂಡಿಸು_paranoid(cfs_rq, se);

	__sum_w_vruntime_add(cfs_rq, se);
}

ಸ್ಥಿರ ಶೂನ್ಯ
ಮೊತ್ತ_w_vruntime_sub(ರಚನೆ cfs_rq *cfs_rq, ರಚನೆ sched_entity *se)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ತೂಕ = avg_vruntime_ತೂಕ(cfs_rq, se->h_ತುಂಬು.ತೂಕ);
	s64 ಕೀಲಿ = entity_ಕೀಲಿ(cfs_rq, se);

	cfs_rq->ಮೊತ್ತ_w_vruntime -= ಕೀಲಿ * ತೂಕ;
	cfs_rq->ಮೊತ್ತ_ತೂಕ -= ತೂಕ;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ
ಶೂನ್ಯ update_zero_vruntime(ರಚನೆ cfs_rq *cfs_rq, s64 delta)
{
	/*
	 * v' = v + d ==> sum_w_vruntime' = sum_w_vruntime - d*sum_weight
	 */
	cfs_rq->ಮೊತ್ತ_w_vruntime -= cfs_rq->ಮೊತ್ತ_ತೂಕ * delta;
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
u64 avg_vruntime(ರಚನೆ cfs_rq *cfs_rq)
{
	ರಚನೆ sched_entity *curr = cfs_rq->curr;
	ದೀರ್ಘ ತೂಕ = cfs_rq->ಮೊತ್ತ_ತೂಕ;
	s64 delta = 0;

	ಆದರೆ (curr && !curr->on_rq)
		curr = ಖಾಲಿ;

	ಆದರೆ (ತೂಕ) {
		s64 runtime = cfs_rq->ಮೊತ್ತ_w_vruntime;

		ಆದರೆ (curr) {
			ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ w = avg_vruntime_ತೂಕ(cfs_rq, curr->h_ತುಂಬು.ತೂಕ);

			runtime += entity_ಕೀಲಿ(cfs_rq, curr) * w;
			ತೂಕ += w;
		}

		/* sign flips effective floor / ceiling */
		ಆದರೆ (runtime < 0)
			runtime -= (ತೂಕ - 1);

		delta = div64_long(runtime, ತೂಕ);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (curr) {
		/*
		 * When there is but one element, it is the average.
		 */
		delta = curr->vruntime - cfs_rq->zero_vruntime;
	}

	update_zero_vruntime(cfs_rq, delta);

	ಹಿಂತಿರುಗಿ cfs_rq->zero_vruntime;
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
ಸ್ಥಿರ u64 ineligible_vruntime(ರಚನೆ cfs_rq *cfs_rq)
{
	ರಚನೆ sched_entity *curr = cfs_rq->curr;
	ದೀರ್ಘ ತೂಕ = cfs_rq->ಮೊತ್ತ_ತೂಕ;
	s64 delta = 0;

	ಆದರೆ (curr && !curr->on_rq)
		curr = ಖಾಲಿ;

	ಆದರೆ (ತೂಕ) {
		s64 runtime = cfs_rq->ಮೊತ್ತ_w_vruntime;

		/*
		 * Do not add @curr to obtain the effective '- w_j' terms.
		 */

		/* sign flips effective floor / ceiling */
		ಆದರೆ (runtime < 0)
			runtime -= (ತೂಕ - 1);

		delta = div64_long(runtime, ತೂಕ);
	}

	ಹಿಂತಿರುಗಿ cfs_rq->zero_vruntime + delta + 1;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ u64 cfs_rq_max_ತುಂಡು(ರಚನೆ cfs_rq *cfs_rq);

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
ಸ್ಥಿರ s64 entity_lag(ರಚನೆ cfs_rq *cfs_rq, ರಚನೆ sched_entity *se, u64 avruntime)
{
	u64 max_ತುಂಡು = cfs_rq_max_ತುಂಡು(cfs_rq) + TICK_NSEC;
	s64 vlag, ಎಲ್ಲೆ;

	vlag = avruntime - se->vruntime;
	ಎಲ್ಲೆ = calc_delta_fair(max_ತುಂಡು, se);

	ಹಿಂತಿರುಗಿ clamp(vlag, -ಎಲ್ಲೆ, ಎಲ್ಲೆ);
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
ಸ್ಥಿರ __always_inline
bool update_entity_lag(ರಚನೆ cfs_rq *cfs_rq, ರಚನೆ sched_entity *se)
{
	u64 avruntime = avg_vruntime(cfs_rq);
	s64 vlag = entity_lag(cfs_rq, se, avruntime);

	ಆದರೆ (se->sched_delayed) {
		/* previous vlag < 0 otherwise se would not be delayed */
		vlag = max(vlag, se->vlag);
		ಆದರೆ (sched_feat(DELAY_ZERO))
			vlag = min(vlag, 0);
	}
	se->vlag = vlag;

	ಹಿಂತಿರುಗಿ avruntime - vlag != se->vruntime;
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
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ vruntime_eligible(ರಚನೆ cfs_rq *cfs_rq, u64 vruntime)
{
	ರಚನೆ sched_entity *curr = cfs_rq->curr;
	s64 ಕೀಲಿ, avg = cfs_rq->ಮೊತ್ತ_w_vruntime;
	ದೀರ್ಘ ತುಂಬು = cfs_rq->ಮೊತ್ತ_ತೂಕ;

	ಆದರೆ (curr && curr->on_rq) {
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ತೂಕ = avg_vruntime_ತೂಕ(cfs_rq, curr->h_ತುಂಬು.ತೂಕ);

		avg += entity_ಕೀಲಿ(cfs_rq, curr) * ತೂಕ;
		ತುಂಬು += ತೂಕ;
	}

	ಕೀಲಿ = vruntime_op(vruntime, "-", cfs_rq->zero_vruntime);

	/*
	 * The worst case term for @key includes 'NSEC_TICK * NICE_0_LOAD'
	 * and @load obviously includes NICE_0_LOAD. NSEC_TICK is around 24
	 * bits, while NICE_0_LOAD is 20 on 64bit and 10 otherwise.
	 *
	 * This gives that on 64bit the product will be at least 64bit which
	 * overflows s64, while on 32bit it will only be 44bits and should fit
	 * comfortably.
	 */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_64_ಬಿಟ್
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_ARCH_SUPPORTS_INT128
	/* This often results in simpler code than __builtin_mul_overflow(). */
	ಹಿಂತಿರುಗಿ avg >= (__int128)ಕೀಲಿ * ತುಂಬು;
#ಇಲ್ಲದಿದ್ದರೆ
	s64 rhs;
	/*
	 * On overflow, the sign of key tells us the correct answer: a large
	 * positive key means vruntime >> V, so not eligible; a large negative
	 * key means vruntime << V, so eligible.
	 */
	ಆದರೆ (check_mul_ಅತಿಪ್ರವಾಹ(ಕೀಲಿ, ತುಂಬು, &rhs))
		ಹಿಂತಿರುಗಿ ಕೀಲಿ <= 0;

	ಹಿಂತಿರುಗಿ avg >= rhs;
#ಆದರೆ_ಅಂತ್ಯ
#ಇಲ್ಲದಿದ್ದರೆ /* 32bit */
	ಹಿಂತಿರುಗಿ avg >= ಕೀಲಿ * ತುಂಬು;
#ಆದರೆ_ಅಂತ್ಯ
}

ಪೂರ್ಣಾಂಕ entity_eligible(ರಚನೆ cfs_rq *cfs_rq, ರಚನೆ sched_entity *se)
{
	ಹಿಂತಿರುಗಿ vruntime_eligible(cfs_rq, se->vruntime);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ u64 cfs_rq_min_ತುಂಡು(ರಚನೆ cfs_rq *cfs_rq)
{
	ರಚನೆ sched_entity *ಬೇರು = __pick_root_entity(cfs_rq);
	ರಚನೆ sched_entity *curr = cfs_rq->curr;
	u64 min_ತುಂಡು = ~0ULL;

	ಆದರೆ (curr && curr->on_rq)
		min_ತುಂಡು = curr->ತುಂಡು;

	ಆದರೆ (ಬೇರು)
		min_ತುಂಡು = min(min_ತುಂಡು, ಬೇರು->min_ತುಂಡು);

	ಹಿಂತಿರುಗಿ min_ತುಂಡು;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ u64 cfs_rq_max_ತುಂಡು(ರಚನೆ cfs_rq *cfs_rq)
{
	ರಚನೆ sched_entity *ಬೇರು = __pick_root_entity(cfs_rq);
	ರಚನೆ sched_entity *curr = cfs_rq->curr;
	u64 max_ತುಂಡು = 0ULL;

	ಆದರೆ (curr && curr->on_rq)
		max_ತುಂಡು = curr->ತುಂಡು;

	ಆದರೆ (ಬೇರು)
		max_ತುಂಡು = max(max_ತುಂಡು, ಬೇರು->max_ತುಂಡು);

	ಹಿಂತಿರುಗಿ max_ತುಂಡು;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool __entity_less(ರಚನೆ rb_ಗಂಟು *a, ಸ್ಥಿರಾಂಕ ರಚನೆ rb_ಗಂಟು *b)
{
	ಹಿಂತಿರುಗಿ entity_before(__node_2_se(a), __node_2_se(b));
}

/* min() for wrapping vruntimes */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ u64 __min_vruntime(u64 a, u64 b)
{
	ಹಿಂತಿರುಗಿ vruntime_cmp(a, "<", b) ? a : b;
}

RB_DECLARE_CALLBACKS(ಸ್ಥಿರ, min_vruntime_cb,
		     ರಚನೆ sched_entity, ಸತತ_ಗಂಟು,
		     RB_AUG(vruntime, min_vruntime, __min_vruntime),
		     RB_AUG(ತುಂಡು, min_ತುಂಡು, min),
		     RB_AUG(ತುಂಡು, max_ತುಂಡು, max));

/*
 * Enqueue an entity into the rb-tree:
 */
ಸ್ಥಿರ ಶೂನ್ಯ __enqueue_entity(ರಚನೆ cfs_rq *cfs_rq, ರಚನೆ sched_entity *se)
{
	WARN_ON_ONCE(&rq_ದ(cfs_rq)->cfs != cfs_rq);
	WARN_ON_ONCE(!entity_is_task(se));

	ಮೊತ್ತ_w_vruntime_ಕೂಡಿಸು(cfs_rq, se);
	se->min_vruntime = se->vruntime;
	se->min_ತುಂಡು = se->ತುಂಡು;
	se->max_ತುಂಡು = se->ತುಂಡು;

	rb_ಕೂಡಿಸು_augmented_cached(&se->ಸತತ_ಗಂಟು, &cfs_rq->tasks_timeline,
				__entity_less, &min_vruntime_cb);
}

ಸ್ಥಿರ ಶೂನ್ಯ __dequeue_entity(ರಚನೆ cfs_rq *cfs_rq, ರಚನೆ sched_entity *se)
{
	WARN_ON_ONCE(&rq_ದ(cfs_rq)->cfs != cfs_rq);
	WARN_ON_ONCE(!entity_is_task(se));

	rb_erase_augmented_cached(&se->ಸತತ_ಗಂಟು, &cfs_rq->tasks_timeline,
				  &min_vruntime_cb);
	ಮೊತ್ತ_w_vruntime_sub(cfs_rq, se);
}

ರಚನೆ sched_entity *__pick_root_entity(ರಚನೆ cfs_rq *cfs_rq)
{
	ರಚನೆ rb_ಗಂಟು *ಬೇರು = cfs_rq->tasks_timeline.rb_ಬೇರು.rb_ಗಂಟು;

	ಆದರೆ (!ಬೇರು)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	ಹಿಂತಿರುಗಿ __node_2_se(ಬೇರು);
}

ರಚನೆ sched_entity *__pick_first_entity(ರಚನೆ cfs_rq *cfs_rq)
{
	ರಚನೆ rb_ಗಂಟು *ಎಡ = rb_ಮೊದಲ_cached(&cfs_rq->tasks_timeline);

	ಆದರೆ (!ಎಡ)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	ಹಿಂತಿರುಗಿ __node_2_se(ಎಡ);
}

/*
 * Set the vruntime up to which an entity can run before looking
 * for another entity to pick.
 * In case of run to parity, we use the shortest slice of the enqueued
 * entities to set the protected period.
 * When run to parity is disabled, we give a minimum quantum to the running
 * entity to ensure progress.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ ಗಣ_protect_ತುಂಡು(ರಚನೆ cfs_rq *cfs_rq, ರಚನೆ sched_entity *se)
{
	u64 ತುಂಡು = normalized_sysctl_sched_ತಳ_ತುಂಡು;
	u64 vprot = se->deadline;

	ಆದರೆ (sched_feat(ಸತತ_ಗಮ್ಯ_PARITY))
		ತುಂಡು = cfs_rq_min_ತುಂಡು(cfs_rq);

	ತುಂಡು = min(ತುಂಡು, se->ತುಂಡು);

	/* If there are shorter slices than se's one */
	ಆದರೆ (ತುಂಡು != se->ತುಂಡು) {
		vprot = min_vruntime(vprot, se->vruntime + calc_delta_fair(ತುಂಡು, se));
		ಆದರೆ (sched_feat(PREEMPT_SHORT))
			vprot = min_vruntime(vprot, ineligible_vruntime(cfs_rq));
	}

	se->vprot = vprot;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ update_protect_ತುಂಡು(ರಚನೆ cfs_rq *cfs_rq, ರಚನೆ sched_entity *se)
{
	u64 vruntime = min_vruntime(se->vruntime, avg_vruntime(cfs_rq));
	u64 ತುಂಡು = cfs_rq_min_ತುಂಡು(cfs_rq);
	u64 vprot = min_vruntime(se->vprot, vruntime + calc_delta_fair(ತುಂಡು, se));

	ಆದರೆ (sched_feat(PREEMPT_SHORT) && ತುಂಡು != se->ತುಂಡು)
		vprot = min_vruntime(vprot, ineligible_vruntime(cfs_rq));

	se->vprot = vprot;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool protect_ತುಂಡು(ರಚನೆ sched_entity *se)
{
	ಹಿಂತಿರುಗಿ vruntime_cmp(se->vruntime, "<", se->vprot);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ cancel_protect_ತುಂಡು(ರಚನೆ sched_entity *se)
{
	ಆದರೆ (protect_ತುಂಡು(se))
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
ಸ್ಥಿರ ರಚನೆ sched_entity *pick_eevdf(ರಚನೆ cfs_rq *cfs_rq, bool protect)
{
	ರಚನೆ rb_ಗಂಟು *ಗಂಟು = cfs_rq->tasks_timeline.rb_ಬೇರು.rb_ಗಂಟು;
	ರಚನೆ sched_entity *se = __pick_first_entity(cfs_rq);
	ರಚನೆ sched_entity *curr = cfs_rq->curr;
	ರಚನೆ sched_entity *ಉತ್ತಮ = ಖಾಲಿ;

	/*
	 * We can safely skip eligibility check if there is only one entity
	 * in this cfs_rq, saving some cycles.
	 */
	ಆದರೆ (cfs_rq->h_nr_queued == 1)
		ಹಿಂತಿರುಗಿ curr && curr->on_rq ? curr : se;

	/*
	 * Picking the ->next buddy will affect latency but not fairness.
	 */
	ಆದರೆ (sched_feat(PICK_BUDDY) && protect &&
	    cfs_rq->ಮುಂದಿನ && entity_eligible(cfs_rq, cfs_rq->ಮುಂದಿನ)) {
		/* ->next will never be delayed */
		WARN_ON_ONCE(cfs_rq->ಮುಂದಿನ->sched_delayed);
		ಹಿಂತಿರುಗಿ cfs_rq->ಮುಂದಿನ;
	}

	ಆದರೆ (curr && (!curr->on_rq || !entity_eligible(cfs_rq, curr)))
		curr = ಖಾಲಿ;

	ಆದರೆ (curr && protect && protect_ತುಂಡು(curr))
		ಹಿಂತಿರುಗಿ curr;

	/* Pick the leftmost entity if it's eligible */
	ಆದರೆ (se && entity_eligible(cfs_rq, se)) {
		ಉತ್ತಮ = se;
		ಹೋಗು ಸಿಕ್ಕಿತು;
	}

	/* Heap search for the EEVD entity */
	ಆಗ (ಗಂಟು) {
		ರಚನೆ rb_ಗಂಟು *ಎಡ = ಗಂಟು->rb_ಎಡ;

		/*
		 * Eligible entities in left subtree are always better
		 * choices, since they have earlier deadlines.
		 */
		ಆದರೆ (ಎಡ && vruntime_eligible(cfs_rq,
					__node_2_se(ಎಡ)->min_vruntime)) {
			ಗಂಟು = ಎಡ;
			ಮುಂದುವರಿ;
		}

		se = __node_2_se(ಗಂಟು);

		/*
		 * The left subtree either is empty or has no eligible
		 * entity, so check the current node since it is the one
		 * with earliest deadline that might be eligible.
		 */
		ಆದರೆ (entity_eligible(cfs_rq, se)) {
			ಉತ್ತಮ = se;
			ನಿಲ್ಲಿಸು;
		}

		ಗಂಟು = ಗಂಟು->rb_ಬಲ;
	}
ಸಿಕ್ಕಿತು:
	ಆದರೆ (!ಉತ್ತಮ || (curr && entity_before(curr, ಉತ್ತಮ)))
		ಉತ್ತಮ = curr;

	ಹಿಂತಿರುಗಿ ಉತ್ತಮ;
}

ರಚನೆ sched_entity *__pick_last_entity(ರಚನೆ cfs_rq *cfs_rq)
{
	ರಚನೆ rb_ಗಂಟು *ಕೊನೆಯ = rb_ಕೊನೆಯ(&cfs_rq->tasks_timeline.rb_ಬೇರು);

	ಆದರೆ (!ಕೊನೆಯ)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	ಹಿಂತಿರುಗಿ __node_2_se(ಕೊನೆಯ);
}

/**************************************************************
 * Scheduling class statistics methods:
 */
ಪೂರ್ಣಾಂಕ sched_update_scaling(ಶೂನ್ಯ)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಗುಣಕ = get_update_sysctl_ಗುಣಕ();

#ವ್ಯಾಖ್ಯಾನಿಸು WRT_SYSCTL(name) \
	(normalized_sysctl_##name = sysctl_##name / (ಗುಣಕ))
	WRT_SYSCTL(sched_ತಳ_ತುಂಡು);
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು WRT_SYSCTL

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ clear_buddies(ರಚನೆ cfs_rq *cfs_rq, ರಚನೆ sched_entity *se);

/*
 * XXX: strictly: vd_i += N*r_i/w_i such that: vd_i > ve_i
 * this is probably good enough.
 */
ಸ್ಥಿರ bool update_deadline(ರಚನೆ cfs_rq *cfs_rq, ರಚನೆ sched_entity *se)
{
	ಆದರೆ (vruntime_cmp(se->vruntime, "<", se->deadline))
		ಹಿಂತಿರುಗಿ false;

	/*
	 * For EEVDF the virtual time slope is determined by w_i (iow.
	 * nice) while the request time r_i is determined by
	 * sysctl_sched_base_slice.
	 */
	ಆದರೆ (!se->custom_ತುಂಡು)
		se->ತುಂಡು = sysctl_sched_ತಳ_ತುಂಡು;

	/*
	 * EEVDF: vd_i = ve_i + r_i / w_i
	 */
	se->deadline = se->vruntime + calc_delta_fair(se->ತುಂಡು, se);
	avg_vruntime(cfs_rq);

	/*
	 * The task has consumed its request, reschedule.
	 */
	ಹಿಂತಿರುಗಿ true;
}

#ಅನ್ತರ್ಭಾವ "pelt.h"

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ select_idle_sibling(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ prev_cpu, ಪೂರ್ಣಾಂಕ cpu);
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ task_h_ತುಂಬು(ರಚನೆ task_struct *p);
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಸಾಮರ್ಥ್ಯ_ದ(ಪೂರ್ಣಾಂಕ cpu);

/* Give new sched_entity start runnable values to heavy its load in infant time */
ಶೂನ್ಯ init_entity_runnable_ಸರಾಸರಿ_ಮೌಲ್ಯ(ರಚನೆ sched_entity *se)
{
	ರಚನೆ sched_avg *sa = &se->avg;

	memset(sa, 0, ಗಾತ್ರ(*sa));

	/*
	 * Tasks are initialized with full load to be seen as heavy tasks until
	 * they get a chance to stabilize to their real load level.
	 * Group entities are initialized with zero load to reflect the fact that
	 * nothing has been attached to the task group yet.
	 */
	ಆದರೆ (entity_is_task(se))
		sa->ತುಂಬು_avg = scale_ತುಂಬು_ಕೆಳಕ್ಕೆ(se->ತುಂಬು.ತೂಕ);

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
ಶೂನ್ಯ post_init_entity_util_avg(ರಚನೆ task_struct *p)
{
	ರಚನೆ sched_entity *se = &p->se;
	ರಚನೆ cfs_rq *cfs_rq = cfs_rq_ದ(se);
	ರಚನೆ sched_avg *sa = &se->avg;
	ದೀರ್ಘ cpu_scale = arch_scale_cpu_ಸಾಮರ್ಥ್ಯ(cpu_ದ(rq_ದ(cfs_rq)));
	ದೀರ್ಘ cap = (ದೀರ್ಘ)(cpu_scale - cfs_rq->avg.util_avg) / 2;

	ಆದರೆ (p->sched_class != &fair_sched_class) {
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
		se->avg.ಕೊನೆಯ_update_ಸಮಯ = cfs_rq_clock_pelt(cfs_rq);
		ಹಿಂತಿರುಗಿ;
	}

	ಆದರೆ (cap > 0) {
		ಆದರೆ (cfs_rq->avg.util_avg != 0) {
			sa->util_avg  = cfs_rq->avg.util_avg * se_ತೂಕ(se);
			sa->util_avg /= (cfs_rq->avg.ತುಂಬು_avg + 1);

			ಆದರೆ (sa->util_avg > cap)
				sa->util_avg = cap;
		} ಇಲ್ಲದಿದ್ದರೆ {
			sa->util_avg = cap;
		}
	}

	sa->runnable_avg = sa->util_avg;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ account_mm_sched(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, s64 delta_exec);

ಸ್ಥಿರ s64 update_se(ರಚನೆ rq *rq, ರಚನೆ sched_entity *se)
{
	u64 now = rq_clock_task(rq);
	s64 delta_exec;

	delta_exec = now - se->exec_ಆರಂಭ;
	ಆದರೆ (unlikely(delta_exec <= 0))
		ಹಿಂತಿರುಗಿ delta_exec;

	se->exec_ಆರಂಭ = now;
	ಆದರೆ (entity_is_task(se)) {
		ರಚನೆ task_struct *running = rq->curr;
		/*
		 * If se is a task, we account the time against the running
		 * task, as w/ proxy-exec they may not be the same.
		 */
		running->se.exec_ಆರಂಭ = now;
		running->se.ಮೊತ್ತ_exec_runtime += delta_exec;

		trace_sched_stat_runtime(running, delta_exec);
		account_ಗುಂಪು_exec_runtime(running, delta_exec);
		account_mm_sched(rq, running, delta_exec);

		cgroup_account_cputime(running, delta_exec);
	} ಇಲ್ಲದಿದ್ದರೆ {
		/* If not task, account the time against donor se  */
		se->ಮೊತ್ತ_exec_runtime += delta_exec;
	}

	ಆದರೆ (schedstat_enabled()) {
		ರಚನೆ sched_statistics *stats;

		stats = __schedstats_from_se(se);
		__schedstat_set(stats->exec_max,
				max(delta_exec, stats->exec_max));
	}

	ಹಿಂತಿರುಗಿ delta_exec;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CACHE

/*
 * XXX numbers come from a place the sun don't shine -- probably wants to be SD
 * tunable or so.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು EPOCH_PERIOD	(HZ / 100)	/* 10 ms */
#ವ್ಯಾಖ್ಯಾನಿಸು EPOCH_LLC_AFFINITY_TIMEOUT	5	/* 50 ms */
__read_mostly ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ llc_aggr_tolerance	= 1;
__read_mostly ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ llc_epoch_period	= EPOCH_PERIOD;
__read_mostly ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ llc_epoch_affinity_timeout = EPOCH_LLC_AFFINITY_TIMEOUT;
__read_mostly ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ llc_imb_pct		= 20;
__read_mostly ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ llc_overaggr_pct	= 50;

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ llc_id(ಪೂರ್ಣಾಂಕ cpu)
{
	ಆದರೆ (cpu < 0)
		ಹಿಂತಿರುಗಿ -1;

	ಹಿಂತಿರುಗಿ ಪ್ರತಿ_cpu(sd_llc_id, cpu);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ get_sched_cache_scale(ಪೂರ್ಣಾಂಕ mul)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ tol = READ_ONCE(llc_aggr_tolerance);

	ಆದರೆ (!tol)
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (tol >= 100)
		ಹಿಂತಿರುಗಿ ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ;

	ಹಿಂತಿರುಗಿ (1 + (tol - 1) * mul);
}

ಸ್ಥಿರ bool exceed_llc_ಸಾಮರ್ಥ್ಯ(ರಚನೆ sched_cache_ಗುಂಪು *grp, ಪೂರ್ಣಾಂಕ cpu)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_NUMA_BALANCING
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ llc, footprint;
	ರಚನೆ sched_domain *sd;
	ಪೂರ್ಣಾಂಕ scale;

	guard(rcu)();

	sd = rcu_dereference_sched_domain(cpu_rq(cpu)->sd);
	ಆದರೆ (!sd)
		ಹಿಂತಿರುಗಿ true;

	ಆದರೆ (static_branch_likely(&sched_numa_balancing)) {
		/*
		 * TBD: RDT exclusive LLC ways reserved should be
		 * excluded.
		 */
		llc = sd->llc_ಬೈಟುಗಳು;
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
		ಆದರೆ (scale == ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ)
			ಹಿಂತಿರುಗಿ false;

		ಹಿಂತಿರುಗಿ ((llc * (u64)scale) < (footprint * ಪುಟ_ಗಾತ್ರ));
	}
#ಆದರೆ_ಅಂತ್ಯ
	ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ bool invalid_llc_nr(ರಚನೆ sched_cache_ಗುಂಪು *grp, ರಚನೆ task_struct *p,
			   ಪೂರ್ಣಾಂಕ cpu)
{
	ಪೂರ್ಣಾಂಕ scale;

	ಆದರೆ (get_nr_ಎಳೆಗಳು(p) <= 1)
		ಹಿಂತಿರುಗಿ true;

	/*
	 * Scale the number of 'cores' in a LLC by llc_aggr_tolerance
	 * and compare it to the task's active threads.
	 */
	scale = get_sched_cache_scale(1);
	ಆದರೆ (scale == ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ)
		ಹಿಂತಿರುಗಿ false;

	ಹಿಂತಿರುಗಿ !ಹೊಂದುತ್ತದೆ_ಸಾಮರ್ಥ್ಯ((READ_ONCE(grp->nr_running_avg) * cpu_smt_ಅಂಕೆ_ಎಳೆಗಳು),
			(scale * ಪ್ರತಿ_cpu(sd_llc_ಗಾತ್ರ, cpu)));
}

/*
 * A task counts in nr_pref_llc_running while it is queued on its preferred
 * LLC (pref_llc_queued) and runnable (!sched_delayed), keeping the counter in
 * the runnable domain so alb_break_llc() can compare it with h_nr_runnable.
 */
ಸ್ಥಿರ bool task_pref_llc_runnable(ರಚನೆ task_struct *p)
{
	ಹಿಂತಿರುಗಿ p->pref_llc_queued && !p->se.sched_delayed;
}

ಸ್ಥಿರ ಶೂನ್ಯ pref_llc_running_inc(ರಚನೆ rq *rq, ರಚನೆ task_struct *p)
{
	ಆದರೆ (task_pref_llc_runnable(p))
		rq->nr_pref_llc_running++;
}

ಸ್ಥಿರ ಶೂನ್ಯ pref_llc_running_dec(ರಚನೆ rq *rq, ರಚನೆ task_struct *p)
{
	ಆದರೆ (task_pref_llc_runnable(p))
		rq->nr_pref_llc_running--;
}

ಸ್ಥಿರ ಶೂನ್ಯ account_llc_ಸರತಿಗೆ_ಸೇರಿಸು(ರಚನೆ rq *rq, ರಚನೆ task_struct *p)
{
	ಪೂರ್ಣಾಂಕ pref_llc, pref_llc_queued;
	ರಚನೆ sched_domain *sd;

	pref_llc = p->preferred_llc;
	ಆದರೆ (pref_llc < 0)
		ಹಿಂತಿರುಗಿ;

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
	ಆದರೆ (sd && (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)pref_llc < sd->llc_max)
		sd->llc_counts[pref_llc]++;
}

ಸ್ಥಿರ ಶೂನ್ಯ account_llc_ಸರತಿಯಿಂದ_ತೆಗೆ(ರಚನೆ rq *rq, ರಚನೆ task_struct *p)
{
	ರಚನೆ sched_domain *sd;
	ಪೂರ್ಣಾಂಕ pref_llc;

	pref_llc = p->preferred_llc;
	ಆದರೆ (pref_llc < 0)
		ಹಿಂತಿರುಗಿ;

	rq->nr_llc_running--;
	ಆದರೆ (p->pref_llc_queued) {
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
	ಆದರೆ (sd && (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)pref_llc < sd->llc_max) {
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
		ಆದರೆ (sd->llc_counts[pref_llc])
			sd->llc_counts[pref_llc]--;
	}
}

ಪೂರ್ಣಾಂಕ mm_init_sched(ರಚನೆ mm_struct *mm,
		  ರಚನೆ sched_cache_ಸಮಯ __percpu *_pcpu_sched)
{
	ರಚನೆ sched_cache_ಗುಂಪು *grp;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ epoch = 0;
	ಪೂರ್ಣಾಂಕ i;

	grp = kzalloc_obj(*grp);
	ಆದರೆ (!grp) {
		free_percpu(_pcpu_sched);
		mm->sched_cache_grp = ಖಾಲಿ;
		ಹಿಂತಿರುಗಿ -ENOMEM;
	}

	for_each_possible_cpu(i) {
		ರಚನೆ sched_cache_ಸಮಯ *pcpu_sched = ಪ್ರತಿ_cpu_ptr(_pcpu_sched, i);
		ರಚನೆ rq *rq = cpu_rq(i);

		pcpu_sched->runtime = 0;
		/* a slightly stale cpu epoch is acceptible */
		pcpu_sched->epoch = rq->cpu_epoch;
		epoch = rq->cpu_epoch;
	}

	raw_spin_ಬೀಗ_init(&grp->ಬೀಗ);
	grp->epoch = epoch;
	grp->cpu = -1;
	grp->ಮುಂದಿನ_ಪರಿಶೀಲನೆ = jiffies;
	grp->nr_running_avg = 0;
	grp->footprint = 0;
	refcount_ಗಣ(&grp->refcnt, 1);
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
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ sched_cache_ಗುಂಪು_free_rcu(ರಚನೆ rcu_ತಲೆ *rcu)
{
	ರಚನೆ sched_cache_ಗುಂಪು *grp =
		container_ದ(rcu, ರಚನೆ sched_cache_ಗುಂಪು, rcu);

	free_percpu(grp->pcpu_sched);
	kfree(grp);
}

ಸ್ಥಿರ ಶೂನ್ಯ sched_cache_ಗುಂಪು_put(ರಚನೆ sched_cache_ಗುಂಪು *grp)
{
	ಆದರೆ (!grp || !refcount_dec_and_test(&grp->refcnt))
		ಹಿಂತಿರುಗಿ;

	call_rcu(&grp->rcu, sched_cache_ಗುಂಪು_free_rcu);
}

DEFINE_FREE(sched_cache_ಗುಂಪು_put, ರಚನೆ sched_cache_ಗುಂಪು *,
	    sched_cache_ಗುಂಪು_put(_T));

#ವ್ಯಾಖ್ಯಾನಿಸು rcu_deref_sched_cache_grp(tsk) \
	rcu_dereference_check((tsk)->sched_cache_grp, (tsk) == current)

ಸ್ಥಿರ ರಚನೆ sched_cache_ಗುಂಪು *sched_cache_replace_grp(ರಚನೆ task_struct *p,
							 ರಚನೆ sched_cache_ಗುಂಪು *new)
{
	ರಚನೆ sched_cache_ಗುಂಪು *old;

	old = rcu_deref_sched_cache_grp(p);
	rcu_ನಿಯೋಜಿಸು_ಸೂಚಕ(p->sched_cache_grp, new);

	ಹಿಂತಿರುಗಿ old;
}

ರಚನೆ sched_cache_ಗುಂಪು *sched_cache_ಗುಂಪು_get(ರಚನೆ sched_cache_ಗುಂಪು *grp)
{
	/*
	 * refcount_inc_not_zero() is the acquire primitive for lockless
	 * (RCU) lookups; plain refcount_inc() would scribble the count if
	 * it already reached zero. Return NULL in that case.
	 */
	ಆದರೆ (grp && !refcount_inc_not_zero(&grp->refcnt))
		grp = ಖಾಲಿ;

	ಹಿಂತಿರುಗಿ grp;
}

ರಚನೆ sched_cache_ಗುಂಪು *task_cache_ಗುಂಪು_get(ರಚನೆ task_struct *p)
{
	guard(rcu)();
	ಹಿಂತಿರುಗಿ sched_cache_ಗುಂಪು_get(rcu_dereference(p->sched_cache_grp));
}

ಶೂನ್ಯ sched_cache_fork(ರಚನೆ task_struct *p)
{
	/*
	 * The child takes its own reference on the mm's cache group, separate
	 * from the reference held by the mm. @p is not yet visible to readers,
	 * so a plain initializing store is enough.
	 */
	RCU_INIT_ಸೂಚಕ(p->sched_cache_grp,
			 sched_cache_ಗುಂಪು_get(p->mm->sched_cache_grp));
}

ಶೂನ್ಯ sched_cache_fork_cleanup(ರಚನೆ task_struct *p)
{
	/*
	 * A fork that fails after sched_cache_fork() never reaches exit_mm(),
	 * so drop the reference here. @p never became visible, so there are no
	 * concurrent readers and the reference we hold keeps the group alive.
	 */
	sched_cache_ಗುಂಪು_put(rcu_access_ಸೂಚಕ(p->sched_cache_grp));
	RCU_INIT_ಸೂಚಕ(p->sched_cache_grp, ಖಾಲಿ);
}

ಶೂನ್ಯ sched_cache_exec_mmap(ರಚನೆ task_struct *p, ರಚನೆ mm_struct *mm)
{
	ರಚನೆ sched_cache_ಗುಂಪು *old;

	/*
	 * Acquire the new reference before publishing the pointer, then drop
	 * the old one. @p is current and the only writer of its own pointer.
	 */
	old = sched_cache_replace_grp(p, sched_cache_ಗುಂಪು_get(mm->sched_cache_grp));
	sched_cache_ಗುಂಪು_put(old);
}

ಶೂನ್ಯ sched_cache_exit_mm(ರಚನೆ task_struct *p)
{
	ರಚನೆ sched_cache_ಗುಂಪು *grp = sched_cache_replace_grp(p, ಖಾಲಿ);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_NUMA_BALANCING
	/*
	 * Subtract this task's footprint from the group before dropping the
	 * reference, so the group footprint converges as its threads exit.
	 * Unlocked for performance; clamp to avoid underflow.
	 */
	ಆದರೆ (grp && p->ಒಟ್ಟು_numa_ದೋಷಗಳು) {
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ fp = READ_ONCE(grp->footprint);
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ sub = min(fp, p->ಒಟ್ಟು_numa_ದೋಷಗಳು);

		WRITE_ONCE(grp->footprint, fp - sub);
	}
#ಆದರೆ_ಅಂತ್ಯ
	sched_cache_ಗುಂಪು_put(grp);
}

ಶೂನ್ಯ mm_destroy_sched(ರಚನೆ mm_struct *mm)
{
	sched_cache_ಗುಂಪು_put(mm->sched_cache_grp);
	mm->sched_cache_grp = ಖಾಲಿ;
}

/* because why would C be fully specified */
ಸ್ಥಿರ __always_inline ಶೂನ್ಯ __shr_u64(u64 *val, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ n)
{
	ಆದರೆ (n >= 64) {
		*val = 0;
		ಹಿಂತಿರುಗಿ;
	}
	*val >>= n;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ __update_mm_sched(ರಚನೆ rq *rq,
				     ರಚನೆ sched_cache_ಸಮಯ *pcpu_sched)
{
	lockdep_assert_held(&rq->cpu_epoch_ಬೀಗ);

	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ period = max(READ_ONCE(llc_epoch_period), 1U);
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ n, now = jiffies;
	ದೀರ್ಘ delta = now - rq->cpu_epoch_ಮುಂದಿನ;

	ಆದರೆ (delta > 0) {
		n = (delta + period - 1) / period;
		rq->cpu_epoch += n;
		rq->cpu_epoch_ಮುಂದಿನ += n * period;
		__shr_u64(&rq->cpu_runtime, n);
	}

	n = rq->cpu_epoch - pcpu_sched->epoch;
	ಆದರೆ (n) {
		pcpu_sched->epoch += n;
		__shr_u64(&pcpu_sched->runtime, n);
	}
}

ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ fraction_mm_sched(ರಚನೆ rq *rq,
				       ರಚನೆ sched_cache_ಸಮಯ *pcpu_sched)
{
	guard(raw_spinlock_irqsave)(&rq->cpu_epoch_ಬೀಗ);

	__update_mm_sched(rq, pcpu_sched);

	/*
	 * Runtime is a geometric series (r=0.5) and as such will sum to twice
	 * the accumulation period, this means the multiplcation here should
	 * not overflow.
	 */
	ಹಿಂತಿರುಗಿ div64_u64(NICE_0_ತುಂಬು * pcpu_sched->runtime, rq->cpu_runtime + 1);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ get_pref_llc(ರಚನೆ task_struct *p, ರಚನೆ sched_cache_ಗುಂಪು *grp)
{
	ಪೂರ್ಣಾಂಕ mm_sched_llc = -1, mm_sched_cpu;

	ಆದರೆ (!grp)
		ಹಿಂತಿರುಗಿ -1;

	mm_sched_cpu = READ_ONCE(grp->cpu);
	ಆದರೆ (mm_sched_cpu != -1) {
		mm_sched_llc = llc_id(mm_sched_cpu);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_NUMA_BALANCING
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
		ಆದರೆ (static_branch_likely(&sched_numa_balancing) &&
		    p->numa_preferred_nid >= 0 &&
		    cpu_ಗಮ್ಯ_ಗಂಟು(mm_sched_cpu) != p->numa_preferred_nid)
			mm_sched_llc = -1;
#ಆದರೆ_ಅಂತ್ಯ
	}

	ಹಿಂತಿರುಗಿ mm_sched_llc;
}

ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ task_running_on_cpu(ಪೂರ್ಣಾಂಕ cpu, ರಚನೆ task_struct *p);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ
ಶೂನ್ಯ account_mm_sched(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, s64 delta_exec)
{
	ರಚನೆ sched_cache_ಗುಂಪು *grp = rcu_dereference_all(p->sched_cache_grp);
	ರಚನೆ sched_cache_ಸಮಯ *pcpu_sched;
	ಪೂರ್ಣಾಂಕ mm_sched_llc = -1;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ epoch;

	ಆದರೆ (!sched_cache_enabled())
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (p->sched_class != &fair_sched_class)
		ಹಿಂತಿರುಗಿ;
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
	ಆದರೆ (p->flags & PF_KTHREAD || !grp || !grp->pcpu_sched)
		ಹಿಂತಿರುಗಿ;

	pcpu_sched = ಪ್ರತಿ_cpu_ptr(grp->pcpu_sched, cpu_ದ(rq));

	scoped_guard (raw_spinlock, &rq->cpu_epoch_ಬೀಗ) {
		__update_mm_sched(rq, pcpu_sched);
		pcpu_sched->runtime += delta_exec;
		rq->cpu_runtime += delta_exec;
		epoch = rq->cpu_epoch;
	}

	/*
	 * If this process hasn't hit task_cache_work() for a while invalidate
	 * its preferred state.
	 */
	ಆದರೆ ((ದೀರ್ಘ)(epoch - READ_ONCE(grp->epoch)) > llc_epoch_affinity_timeout ||
	    invalid_llc_nr(grp, p, cpu_ದ(rq)) ||
	    exceed_llc_ಸಾಮರ್ಥ್ಯ(grp, cpu_ದ(rq))) {
		ಆದರೆ (READ_ONCE(grp->cpu) != -1)
			WRITE_ONCE(grp->cpu, -1);
	}

	mm_sched_llc = get_pref_llc(p, grp);

	/* task not on rq accounted later in account_entity_enqueue() */
	ಆದರೆ (task_running_on_cpu(rq->cpu, p) &&
	    READ_ONCE(p->preferred_llc) != mm_sched_llc) {
		account_llc_ಸರತಿಯಿಂದ_ತೆಗೆ(rq, p);
		WRITE_ONCE(p->preferred_llc, mm_sched_llc);
		account_llc_ಸರತಿಗೆ_ಸೇರಿಸು(rq, p);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ task_tick_cache(ರಚನೆ rq *rq, ರಚನೆ task_struct *p)
{
	ರಚನೆ sched_cache_ಗುಂಪು *grp = rcu_dereference_all(p->sched_cache_grp);
	ರಚನೆ callback_ತಲೆ *work = &p->cache_work;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ epoch;

	ಆದರೆ (!sched_cache_enabled())
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (!grp || p->flags & PF_KTHREAD ||
	    !grp->pcpu_sched)
		ಹಿಂತಿರುಗಿ;

	epoch = rq->cpu_epoch;
	/* avoid moving backwards */
	ಆದರೆ (ಸಮಯ_after_eq(grp->epoch, epoch))
		ಹಿಂತಿರುಗಿ;

	guard(raw_spinlock)(&grp->ಬೀಗ);

	ಆದರೆ (work->ಮುಂದಿನ == work) {
		task_work_ಕೂಡಿಸು(p, work, TWA_RESUME);
		WRITE_ONCE(grp->epoch, epoch);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ get_ಪರಿಶೀಲನೆ_cpumasks(cpumask_var_t cpus, ರಚನೆ task_struct *p,
			      ರಚನೆ sched_cache_ಗುಂಪು *grp)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_NUMA_BALANCING
	ಪೂರ್ಣಾಂಕ cpu, curr_cpu, nid, pref_nid;

	ಆದರೆ (!static_branch_likely(&sched_numa_balancing))
		ಹೋಗು ಹೊರಹೋಗು;

	cpu = READ_ONCE(grp->cpu);
	ಆದರೆ (cpu != -1)
		nid = cpu_ಗಮ್ಯ_ಗಂಟು(cpu);
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
	ಆದರೆ (pref_nid == NUMA_NO_ಗಂಟು)
		ಹೋಗು ಹೊರಹೋಗು;

	cpumask_or(cpus, cpus, cpumask_ದ_ಗಂಟು(pref_nid));

	/* honor the task's preferred LLC CPU */
	ಆದರೆ (cpu != -1 && !cpumask_test_cpu(cpu, cpus) && nid != NUMA_NO_ಗಂಟು)
		cpumask_or(cpus, cpus, cpumask_ದ_ಗಂಟು(nid));

	/* make sure the task's current running node is included */
	ಆದರೆ (!cpumask_test_cpu(curr_cpu, cpus))
		cpumask_or(cpus, cpus, cpumask_ದ_ಗಂಟು(cpu_ಗಮ್ಯ_ಗಂಟು(curr_cpu)));

	ಹಿಂತಿರುಗಿ;

ಹೊರಹೋಗು:
#ಆದರೆ_ಅಂತ್ಯ
	cpumask_copy(cpus, cpu_online_ಮುಖವಾಡ);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ update_avg_scale(u64 *avg, u64 sample)
{
	ಪೂರ್ಣಾಂಕ ಗುಣಕ = ಪ್ರತಿ_cpu(sd_llc_ಗಾತ್ರ, raw_smp_processor_id());
	s64 diff = sample - *avg;
	u32 ಭಾಜಕ;

	/*
	 * Scale the divisor based on the number of CPUs contained
	 * in the LLC. This scaling ensures smaller LLC domains use
	 * a smaller divisor to achieve more precise sensitivity to
	 * changes in nr_running, while larger LLC domains are capped
	 * at a maximum divisor of 8 which is the default smoothing
	 * factor of EWMA in update_avg().
	 */
	ಭಾಜಕ = clamp_t(u32, (ಗುಣಕ >> 2), 2, 8);
	*avg += div64_s64(diff, ಭಾಜಕ);
}

ಸ್ಥಿರ ಶೂನ್ಯ task_cache_work(ರಚನೆ callback_ತಲೆ *work)
{
	ರಚನೆ sched_cache_ಗುಂಪು *grp __free(sched_cache_ಗುಂಪು_put) = ಖಾಲಿ;
	cpumask_var_t cpus __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	ಪೂರ್ಣಾಂಕ cpu, m_a_cpu = -1, nr_running = 0, curr_cpu;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಮುಂದಿನ_ಪರಿಶೀಲನೆ, now = jiffies;
	ರಚನೆ task_struct *p = current, *cur;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ curr_m_a_occ = 0;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ m_a_occ = 0;

	WARN_ON_ONCE(work != &p->cache_work);

	work->ಮುಂದಿನ = work;

	ಆದರೆ (p->flags & PF_EXITING)
		ಹಿಂತಿರುಗಿ;

	/*
	 * A reference makes sure grp is not released by others. The rcu
	 * lock can not be held till after zalloc_cpumask_var() below,
	 * because the latter might sleep.
	 */
	grp = task_cache_ಗುಂಪು_get(p);
	ಆದರೆ (!grp)
		ಹಿಂತಿರುಗಿ;

	ಮುಂದಿನ_ಪರಿಶೀಲನೆ = READ_ONCE(grp->ಮುಂದಿನ_ಪರಿಶೀಲನೆ);
	ಆದರೆ (ಸಮಯ_before(now, ಮುಂದಿನ_ಪರಿಶೀಲನೆ))
		ಹಿಂತಿರುಗಿ;

	/* only 1 thread is allowed to scan */
	ಆದರೆ (!try_cmpxchg(&grp->ಮುಂದಿನ_ಪರಿಶೀಲನೆ, &ಮುಂದಿನ_ಪರಿಶೀಲನೆ,
			 now + max_t(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ,
				     READ_ONCE(llc_epoch_period), 1)))
		ಹಿಂತಿರುಗಿ;

	curr_cpu = task_cpu(p);
	ಆದರೆ (invalid_llc_nr(grp, p, curr_cpu) ||
	    exceed_llc_ಸಾಮರ್ಥ್ಯ(grp, curr_cpu)) {
		ಆದರೆ (READ_ONCE(grp->cpu) != -1)
			WRITE_ONCE(grp->cpu, -1);

		ಹಿಂತಿರುಗಿ;
	}

	ಆದರೆ (!zalloc_cpumask_var(&cpus, GFP_KERNEL))
		ಹಿಂತಿರುಗಿ;

	scoped_guard (cpus_read_ಬೀಗ) {
		guard(rcu)();

		get_ಪರಿಶೀಲನೆ_cpumasks(cpus, p, grp);

		for_each_cpu(cpu, cpus) {
			/* XXX sched_cluster_active */
			ರಚನೆ sched_domain *sd = rcu_dereference_all(ಪ್ರತಿ_cpu(sd_llc, cpu));
			ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ occ, m_occ = 0, a_occ = 0;
			ಪೂರ್ಣಾಂಕ m_cpu = -1, i;

			ಆದರೆ (!sd)
				ಮುಂದುವರಿ;

			for_each_cpu(i, sched_domain_ಹರವು(sd)) {
				occ = fraction_mm_sched(cpu_rq(i),
							ಪ್ರತಿ_cpu_ptr(grp->pcpu_sched, i));
				a_occ += occ;
				ಆದರೆ (occ > m_occ) {
					m_occ = occ;
					m_cpu = i;
				}

				/*
				 * rcu_access_pointer() is used because the
				 * pointer is only compared, never dereferenced.
				 */
				cur = rcu_dereference_all(cpu_rq(i)->curr);
				ಆದರೆ (cur && !(cur->flags & (PF_EXITING | PF_KTHREAD)) &&
				    rcu_access_ಸೂಚಕ(cur->sched_cache_grp) == grp)
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
			ಆದರೆ (a_occ > m_a_occ) {
				m_a_occ = a_occ;
				m_a_cpu = m_cpu;
			}

			ಆದರೆ (llc_id(cpu) == llc_id(READ_ONCE(grp->cpu)))
				curr_m_a_occ = a_occ;

			cpumask_andnot(cpus, cpus, sched_domain_ಹರವು(sd));
		}
	}

	ಆದರೆ (m_a_occ > (2 * curr_m_a_occ)) {
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

ಶೂನ್ಯ init_sched_mm(ರಚನೆ task_struct *p)
{
	ರಚನೆ callback_ತಲೆ *work = &p->cache_work;

	init_task_work(work, task_cache_work);
	work->ಮುಂದಿನ = work;
	/*
	 * dup_task_struct() copies the parent's task_struct, including its
	 * sched_cache_grp, for which the child holds no reference.  Clear it
	 * here - before copy_mm() runs - so the child never carries a
	 * borrowed pointer that the fork error path would put.
	 */
	RCU_INIT_ಸೂಚಕ(p->sched_cache_grp, ಖಾಲಿ);
	/*
	 * Reset new task's preference to avoid
	 * polluting account_llc_enqueue().
	 */
	p->preferred_llc = -1;
	p->pref_llc_queued = 0;
}

#ಇಲ್ಲದಿದ್ದರೆ /* CONFIG_SCHED_CACHE */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ account_mm_sched(ರಚನೆ rq *rq, ರಚನೆ task_struct *p,
				    s64 delta_exec) { }

ಶೂನ್ಯ init_sched_mm(ರಚನೆ task_struct *p) { }

ಸ್ಥಿರ ಶೂನ್ಯ task_tick_cache(ರಚನೆ rq *rq, ರಚನೆ task_struct *p) { }

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ get_pref_llc(ರಚನೆ task_struct *p,
			       ರಚನೆ mm_struct *mm)
{
	ಹಿಂತಿರುಗಿ -1;
}

ಸ್ಥಿರ ಶೂನ್ಯ account_llc_ಸರತಿಗೆ_ಸೇರಿಸು(ರಚನೆ rq *rq, ರಚನೆ task_struct *p) {}

ಸ್ಥಿರ ಶೂನ್ಯ account_llc_ಸರತಿಯಿಂದ_ತೆಗೆ(ರಚನೆ rq *rq, ರಚನೆ task_struct *p) {}

ಸ್ಥಿರ ಶೂನ್ಯ pref_llc_running_inc(ರಚನೆ rq *rq, ರಚನೆ task_struct *p) {}

ಸ್ಥಿರ ಶೂನ್ಯ pref_llc_running_dec(ರಚನೆ rq *rq, ರಚನೆ task_struct *p) {}

#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_SCHED_CACHE */

/*
 * Used by other classes to account runtime.
 */
s64 update_curr_common(ರಚನೆ rq *rq)
{
	ಹಿಂತಿರುಗಿ update_se(rq, &rq->donor->se);
}

/*
 * Update the current task's runtime statistics.
 */
ಸ್ಥಿರ ಶೂನ್ಯ update_curr(ರಚನೆ cfs_rq *cfs_rq)
{
	/*
	 * Note: cfs_rq->curr corresponds to the task picked to
	 * run (ie: rq->donor.se) which due to proxy-exec may
	 * not necessarily be the actual task running
	 * (rq->curr.se). This is easy to confuse!
	 */
	ರಚನೆ sched_entity *curr = cfs_rq->h_curr;
	ರಚನೆ rq *rq = rq_ದ(cfs_rq);
	s64 delta_exec;
	bool resched;

	ಆದರೆ (unlikely(!curr))
		ಹಿಂತಿರುಗಿ;

	delta_exec = update_se(rq, curr);
	ಆದರೆ (unlikely(delta_exec <= 0))
		ಹಿಂತಿರುಗಿ;

	account_cfs_rq_runtime(cfs_rq, delta_exec);

	ಆದರೆ (!entity_is_task(curr))
		ಹಿಂತಿರುಗಿ;

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

	ಆದರೆ (cfs_rq->h_nr_queued == 1)
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (resched || !protect_ತುಂಡು(curr)) {
		resched_curr_lazy(rq);
		clear_buddies(cfs_rq, curr);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ update_curr_fair(ರಚನೆ rq *rq)
{
	ರಚನೆ sched_entity *se = &rq->donor->se;

	for_each_sched_entity(se)
		update_curr(cfs_rq_ದ(se));
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
update_stats_wait_ಆರಂಭ_fair(ರಚನೆ cfs_rq *cfs_rq, ರಚನೆ sched_entity *se)
{
	ರಚನೆ sched_statistics *stats;
	ರಚನೆ task_struct *p = ಖಾಲಿ;

	ಆದರೆ (!schedstat_enabled())
		ಹಿಂತಿರುಗಿ;

	stats = __schedstats_from_se(se);

	ಆದರೆ (entity_is_task(se))
		p = task_ದ(se);

	__update_stats_wait_start(rq_ದ(cfs_rq), p, stats);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
update_stats_wait_ಕೊನೆ_fair(ರಚನೆ cfs_rq *cfs_rq, ರಚನೆ sched_entity *se)
{
	ರಚನೆ sched_statistics *stats;
	ರಚನೆ task_struct *p = ಖಾಲಿ;

	ಆದರೆ (!schedstat_enabled())
		ಹಿಂತಿರುಗಿ;

	stats = __schedstats_from_se(se);

	/*
	 * When the sched_schedstat changes from 0 to 1, some sched se
	 * maybe already in the runqueue, the se->statistics.wait_start
	 * will be 0.So it will let the delta wrong. We need to avoid this
	 * scenario.
	 */
	ಆದರೆ (unlikely(!schedstat_val(stats->wait_ಆರಂಭ)))
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (entity_is_task(se))
		p = task_ದ(se);

	__update_stats_wait_end(rq_ದ(cfs_rq), p, stats);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
update_stats_ಸರತಿಗೆ_ಸೇರಿಸು_sleeper_fair(ರಚನೆ cfs_rq *cfs_rq, ರಚನೆ sched_entity *se)
{
	ರಚನೆ sched_statistics *stats;
	ರಚನೆ task_struct *tsk = ಖಾಲಿ;

	ಆದರೆ (!schedstat_enabled())
		ಹಿಂತಿರುಗಿ;

	stats = __schedstats_from_se(se);

	ಆದರೆ (entity_is_task(se))
		tsk = task_ದ(se);

	__update_stats_enqueue_sleeper(rq_ದ(cfs_rq), tsk, stats);
}

/*
 * Task is being enqueued - update stats:
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
update_stats_ಸರತಿಗೆ_ಸೇರಿಸು_fair(ರಚನೆ cfs_rq *cfs_rq, ರಚನೆ sched_entity *se, ಪೂರ್ಣಾಂಕ flags)
{
	ಆದರೆ (!schedstat_enabled())
		ಹಿಂತಿರುಗಿ;

	/*
	 * Are we enqueueing a waiting task? (for current tasks
	 * a dequeue/enqueue event is a NOP)
	 */
	ಆದರೆ (se != cfs_rq->h_curr)
		update_stats_wait_ಆರಂಭ_fair(cfs_rq, se);

	ಆದರೆ (flags & ಸರತಿಗೆ_ಸೇರಿಸು_WAKEUP)
		update_stats_ಸರತಿಗೆ_ಸೇರಿಸು_sleeper_fair(cfs_rq, se);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
update_stats_ಸರತಿಯಿಂದ_ತೆಗೆ_fair(ರಚನೆ cfs_rq *cfs_rq, ರಚನೆ sched_entity *se, ಪೂರ್ಣಾಂಕ flags)
{

	ಆದರೆ (!schedstat_enabled())
		ಹಿಂತಿರುಗಿ;

	/*
	 * Mark the end of the wait period if dequeueing a
	 * waiting task:
	 */
	ಆದರೆ (se != cfs_rq->h_curr)
		update_stats_wait_ಕೊನೆ_fair(cfs_rq, se);

	ಆದರೆ ((flags & ಸರತಿಯಿಂದ_ತೆಗೆ_SLEEP) && entity_is_task(se)) {
		ರಚನೆ task_struct *tsk = task_ದ(se);
		ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ state;

		/* XXX racy against TTWU */
		state = READ_ONCE(tsk->__state);
		ಆದರೆ (state & TASK_INTERRUPTIBLE)
			__schedstat_set(tsk->stats.sleep_ಆರಂಭ,
				      rq_clock(rq_ದ(cfs_rq)));
		ಆದರೆ (state & TASK_UNINTERRUPTIBLE)
			__schedstat_set(tsk->stats.ಖಂಡ_ಆರಂಭ,
				      rq_clock(rq_ದ(cfs_rq)));
	}
}

/*
 * We are picking a new current task - update its stats:
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
update_stats_curr_ಆರಂಭ(ರಚನೆ cfs_rq *cfs_rq, ರಚನೆ sched_entity *se)
{
	/*
	 * We are starting a new run period:
	 */
	se->exec_ಆರಂಭ = rq_clock_task(rq_ದ(cfs_rq));
}

/* Check sched_smt_active before calling this to avoid overheads in fastpaths */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool is_core_idle(ಪೂರ್ಣಾಂಕ cpu)
{
	ಪೂರ್ಣಾಂಕ sibling;

	for_each_cpu(sibling, cpu_smt_ಮುಖವಾಡ(cpu)) {
		ಆದರೆ (cpu == sibling)
			ಮುಂದುವರಿ;

		ಆದರೆ (!idle_cpu(sibling))
			ಹಿಂತಿರುಗಿ false;
	}

	ಹಿಂತಿರುಗಿ true;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_NUMA
#ವ್ಯಾಖ್ಯಾನಿಸು NUMA_IMBALANCE_MIN 2

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ದೀರ್ಘ
adjust_numa_imbalance(ಪೂರ್ಣಾಂಕ imbalance, ಪೂರ್ಣಾಂಕ dst_running, ಪೂರ್ಣಾಂಕ imb_numa_nr)
{
	/*
	 * Allow a NUMA imbalance if busy CPUs is less than the maximum
	 * threshold. Above this threshold, individual tasks may be contending
	 * for both memory bandwidth and any shared HT resources.  This is an
	 * approximation as the number of running tasks may not be related to
	 * the number of busy CPUs due to sched_setaffinity.
	 */
	ಆದರೆ (dst_running > imb_numa_nr)
		ಹಿಂತಿರುಗಿ imbalance;

	/*
	 * Allow a small imbalance based on a simple pair of communicating
	 * tasks that remain local when the destination is lightly loaded.
	 */
	ಆದರೆ (imbalance <= NUMA_IMBALANCE_MIN)
		ಹಿಂತಿರುಗಿ 0;

	ಹಿಂತಿರುಗಿ imbalance;
}
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_NUMA */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_NUMA_BALANCING
/*
 * Approximate time to scan a full NUMA task in ms. The task scan period is
 * calculated based on the tasks virtual memory size and
 * numa_balancing_scan_size.
 */
ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ sysctl_numa_balancing_ಪರಿಶೀಲನೆ_period_min = 1000;
ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ sysctl_numa_balancing_ಪರಿಶೀಲನೆ_period_max = 60000;

/* Portion of address space to scan in MB */
ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ sysctl_numa_balancing_ಪರಿಶೀಲನೆ_ಗಾತ್ರ = 256;

/* Scan @scan_size MB every @scan_period after an initial @scan_delay in ms */
ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ sysctl_numa_balancing_ಪರಿಶೀಲನೆ_delay = 1000;

/* The page with hint page fault latency < threshold in ms is considered hot */
ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ sysctl_numa_balancing_hot_ಮಿತಿ = MSEC_ಪ್ರತಿ_SEC;

ರಚನೆ numa_ಗುಂಪು {
	refcount_t refcount;

	spinlock_t ಬೀಗ; /* nr_tasks, tasks */
	ಪೂರ್ಣಾಂಕ nr_tasks;
	pid_t gid;
	ಪೂರ್ಣಾಂಕ ಸಕ್ರಿಯ_nodes;

	ರಚನೆ rcu_ತಲೆ rcu;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಒಟ್ಟು_ದೋಷಗಳು;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ max_ದೋಷಗಳು_cpu;
	/*
	 * faults[] array is split into two regions: faults_mem and faults_cpu.
	 *
	 * Faults_cpu is used to decide whether memory should move
	 * towards the CPU. As a consequence, these stats are weighted
	 * more by CPU use than by memory faults.
	 */
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೋಷಗಳು[];
};

/*
 * For functions that can be called in multiple contexts that permit reading
 * ->numa_group (see struct task_struct for locking rules).
 */
ಸ್ಥಿರ ರಚನೆ numa_ಗುಂಪು *deref_task_numa_ಗುಂಪು(ರಚನೆ task_struct *p)
{
	ಹಿಂತಿರುಗಿ rcu_dereference_check(p->numa_ಗುಂಪು, p == current ||
		(lockdep_is_held(__rq_lockp(task_rq(p))) && !READ_ONCE(p->on_cpu)));
}

ಸ್ಥಿರ ರಚನೆ numa_ಗುಂಪು *deref_curr_numa_ಗುಂಪು(ರಚನೆ task_struct *p)
{
	ಹಿಂತಿರುಗಿ rcu_dereference_protected(p->numa_ಗುಂಪು, p == current);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗುಂಪು_ದೋಷಗಳು_priv(ರಚನೆ numa_ಗುಂಪು *ng);
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗುಂಪು_ದೋಷಗಳು_shared(ರಚನೆ numa_ಗುಂಪು *ng);

ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ task_nr_ಪರಿಶೀಲನೆ_windows(ರಚನೆ task_struct *p)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ rss = 0;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ nr_ಪರಿಶೀಲನೆ_ಪುಟಗಳು;

	/*
	 * Calculations based on RSS as non-present and empty pages are skipped
	 * by the PTE scanner and NUMA hinting faults should be trapped based
	 * on resident pages
	 */
	nr_ಪರಿಶೀಲನೆ_ಪುಟಗಳು = MB_ಗಮ್ಯ_ಪುಟಗಳು(sysctl_numa_balancing_ಪರಿಶೀಲನೆ_ಗಾತ್ರ);
	rss = get_mm_rss(p->mm);
	ಆದರೆ (!rss)
		rss = nr_ಪರಿಶೀಲನೆ_ಪುಟಗಳು;

	rss = ಸುತ್ತು_ಮೇಲೆ(rss, nr_ಪರಿಶೀಲನೆ_ಪುಟಗಳು);
	ಹಿಂತಿರುಗಿ rss / nr_ಪರಿಶೀಲನೆ_ಪುಟಗಳು;
}

/* For sanity's sake, never scan more PTEs than MAX_SCAN_WINDOW MB/sec. */
#ವ್ಯಾಖ್ಯಾನಿಸು MAX_ಪರಿಶೀಲನೆ_ಕಿಟಕಿ 2560

ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ task_ಪರಿಶೀಲನೆ_min(ರಚನೆ task_struct *p)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಪರಿಶೀಲನೆ_ಗಾತ್ರ = READ_ONCE(sysctl_numa_balancing_ಪರಿಶೀಲನೆ_ಗಾತ್ರ);
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಪರಿಶೀಲನೆ, floor;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ windows = 1;

	ಆದರೆ (ಪರಿಶೀಲನೆ_ಗಾತ್ರ < MAX_ಪರಿಶೀಲನೆ_ಕಿಟಕಿ)
		windows = MAX_ಪರಿಶೀಲನೆ_ಕಿಟಕಿ / ಪರಿಶೀಲನೆ_ಗಾತ್ರ;
	floor = 1000 / windows;

	ಪರಿಶೀಲನೆ = sysctl_numa_balancing_ಪರಿಶೀಲನೆ_period_min / task_nr_ಪರಿಶೀಲನೆ_windows(p);
	ಹಿಂತಿರುಗಿ max_t(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ, floor, ಪರಿಶೀಲನೆ);
}

ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ task_ಪರಿಶೀಲನೆ_ಆರಂಭ(ರಚನೆ task_struct *p)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ smin = task_ಪರಿಶೀಲನೆ_min(p);
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ period = smin;
	ರಚನೆ numa_ಗುಂಪು *ng;

	/* Scale the maximum scan period with the amount of shared memory. */
	rcu_read_ಬೀಗ();
	ng = rcu_dereference_all(p->numa_ಗುಂಪು);
	ಆದರೆ (ng) {
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ shared = ಗುಂಪು_ದೋಷಗಳು_shared(ng);
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಖಾಸಗಿ = ಗುಂಪು_ದೋಷಗಳು_priv(ng);

		period *= refcount_read(&ng->refcount);
		period *= shared + 1;
		period /= ಖಾಸಗಿ + shared + 1;
	}
	rcu_read_unlock();

	ಹಿಂತಿರುಗಿ max(smin, period);
}

ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ task_ಪರಿಶೀಲನೆ_max(ರಚನೆ task_struct *p)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ smin = task_ಪರಿಶೀಲನೆ_min(p);
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ smax;
	ರಚನೆ numa_ಗುಂಪು *ng;

	/* Watch for min being lower than max due to floor calculations */
	smax = sysctl_numa_balancing_ಪರಿಶೀಲನೆ_period_max / task_nr_ಪರಿಶೀಲನೆ_windows(p);

	/* Scale the maximum scan period with the amount of shared memory. */
	ng = deref_curr_numa_ಗುಂಪು(p);
	ಆದರೆ (ng) {
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ shared = ಗುಂಪು_ದೋಷಗಳು_shared(ng);
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಖಾಸಗಿ = ಗುಂಪು_ದೋಷಗಳು_priv(ng);
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ period = smax;

		period *= refcount_read(&ng->refcount);
		period *= shared + 1;
		period /= ಖಾಸಗಿ + shared + 1;

		smax = max(smax, period);
	}

	ಹಿಂತಿರುಗಿ max(smin, smax);
}

ಸ್ಥಿರ ಶೂನ್ಯ account_numa_ಸರತಿಗೆ_ಸೇರಿಸು(ರಚನೆ rq *rq, ರಚನೆ task_struct *p)
{
	rq->nr_numa_running += (p->numa_preferred_nid != NUMA_NO_ಗಂಟು);
	rq->nr_preferred_running += (p->numa_preferred_nid == task_ಗಂಟು(p));
}

ಸ್ಥಿರ ಶೂನ್ಯ account_numa_ಸರತಿಯಿಂದ_ತೆಗೆ(ರಚನೆ rq *rq, ರಚನೆ task_struct *p)
{
	rq->nr_numa_running -= (p->numa_preferred_nid != NUMA_NO_ಗಂಟು);
	rq->nr_preferred_running -= (p->numa_preferred_nid == task_ಗಂಟು(p));
}

/* Shared or private faults. */
#ವ್ಯಾಖ್ಯಾನಿಸು NR_NUMA_HINT_FAULT_TYPES 2

/* Memory and CPU locality */
#ವ್ಯಾಖ್ಯಾನಿಸು NR_NUMA_HINT_FAULT_STATS (NR_NUMA_HINT_FAULT_TYPES * 2)

/* Averaged statistics, and temporary buffers. */
#ವ್ಯಾಖ್ಯಾನಿಸು NR_NUMA_HINT_FAULT_BUCKETS (NR_NUMA_HINT_FAULT_STATS * 2)

pid_t task_numa_ಗುಂಪು_id(ರಚನೆ task_struct *p)
{
	ರಚನೆ numa_ಗುಂಪು *ng;
	pid_t gid = 0;

	rcu_read_ಬೀಗ();
	ng = rcu_dereference_all(p->numa_ಗುಂಪು);
	ಆದರೆ (ng)
		gid = ng->gid;
	rcu_read_unlock();

	ಹಿಂತಿರುಗಿ gid;
}

/*
 * The averaged statistics, shared & private, memory & CPU,
 * occupy the first half of the array. The second half of the
 * array is for current counters, which are averaged into the
 * first set by task_numa_placement.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ task_ದೋಷಗಳು_idx(ಎಣಿಕೆ numa_ದೋಷಗಳು_stats s, ಪೂರ್ಣಾಂಕ nid, ಪೂರ್ಣಾಂಕ priv)
{
	ಹಿಂತಿರುಗಿ NR_NUMA_HINT_FAULT_TYPES * (s * nr_ಗಂಟು_ids + nid) + priv;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ task_ದೋಷಗಳು(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ nid)
{
	ಆದರೆ (!p->numa_ದೋಷಗಳು)
		ಹಿಂತಿರುಗಿ 0;

	ಹಿಂತಿರುಗಿ p->numa_ದೋಷಗಳು[task_ದೋಷಗಳು_idx(NUMA_MEM, nid, 0)] +
		p->numa_ದೋಷಗಳು[task_ದೋಷಗಳು_idx(NUMA_MEM, nid, 1)];
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗುಂಪು_ದೋಷಗಳು(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ nid)
{
	ರಚನೆ numa_ಗುಂಪು *ng = deref_task_numa_ಗುಂಪು(p);

	ಆದರೆ (!ng)
		ಹಿಂತಿರುಗಿ 0;

	ಹಿಂತಿರುಗಿ ng->ದೋಷಗಳು[task_ದೋಷಗಳು_idx(NUMA_MEM, nid, 0)] +
		ng->ದೋಷಗಳು[task_ದೋಷಗಳು_idx(NUMA_MEM, nid, 1)];
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗುಂಪು_ದೋಷಗಳು_cpu(ರಚನೆ numa_ಗುಂಪು *ಗುಂಪು, ಪೂರ್ಣಾಂಕ nid)
{
	ಹಿಂತಿರುಗಿ ಗುಂಪು->ದೋಷಗಳು[task_ದೋಷಗಳು_idx(NUMA_CPU, nid, 0)] +
		ಗುಂಪು->ದೋಷಗಳು[task_ದೋಷಗಳು_idx(NUMA_CPU, nid, 1)];
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗುಂಪು_ದೋಷಗಳು_priv(ರಚನೆ numa_ಗುಂಪು *ng)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೋಷಗಳು = 0;
	ಪೂರ್ಣಾಂಕ ಗಂಟು;

	for_each_online_ಗಂಟು(ಗಂಟು) {
		ದೋಷಗಳು += ng->ದೋಷಗಳು[task_ದೋಷಗಳು_idx(NUMA_MEM, ಗಂಟು, 1)];
	}

	ಹಿಂತಿರುಗಿ ದೋಷಗಳು;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗುಂಪು_ದೋಷಗಳು_shared(ರಚನೆ numa_ಗುಂಪು *ng)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೋಷಗಳು = 0;
	ಪೂರ್ಣಾಂಕ ಗಂಟು;

	for_each_online_ಗಂಟು(ಗಂಟು) {
		ದೋಷಗಳು += ng->ದೋಷಗಳು[task_ದೋಷಗಳು_idx(NUMA_MEM, ಗಂಟು, 0)];
	}

	ಹಿಂತಿರುಗಿ ದೋಷಗಳು;
}

/*
 * A node triggering more than 1/3 as many NUMA faults as the maximum is
 * considered part of a numa group's pseudo-interleaving set. Migrations
 * between these nodes are slowed down, to allow things to settle down.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು ಸಕ್ರಿಯ_ಗಂಟು_FRACTION 3

ಸ್ಥಿರ bool numa_is_ಸಕ್ರಿಯ_ಗಂಟು(ಪೂರ್ಣಾಂಕ nid, ರಚನೆ numa_ಗುಂಪು *ng)
{
	ಹಿಂತಿರುಗಿ ಗುಂಪು_ದೋಷಗಳು_cpu(ng, nid) * ಸಕ್ರಿಯ_ಗಂಟು_FRACTION > ng->max_ದೋಷಗಳು_cpu;
}

/* Handle placement on systems where not all nodes are directly connected. */
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ score_nearby_nodes(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ nid,
					ಪೂರ್ಣಾಂಕ lim_dist, bool task)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ score = 0;
	ಪೂರ್ಣಾಂಕ ಗಂಟು, max_dist;

	/*
	 * All nodes are directly connected, and the same distance
	 * from each other. No need for fancy placement algorithms.
	 */
	ಆದರೆ (sched_numa_topology_type == NUMA_ನೇರ)
		ಹಿಂತಿರುಗಿ 0;

	/* sched_max_numa_distance may be changed in parallel. */
	max_dist = READ_ONCE(sched_max_numa_ದೂರ);
	/*
	 * This code is called for each node, introducing N^2 complexity,
	 * which should be OK given the number of nodes rarely exceeds 8.
	 */
	for_each_online_ಗಂಟು(ಗಂಟು) {
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೋಷಗಳು;
		ಪೂರ್ಣಾಂಕ dist = ಗಂಟು_ದೂರ(nid, ಗಂಟು);

		/*
		 * The furthest away nodes in the system are not interesting
		 * for placement; nid was already counted.
		 */
		ಆದರೆ (dist >= max_dist || ಗಂಟು == nid)
			ಮುಂದುವರಿ;

		/*
		 * On systems with a backplane NUMA topology, compare groups
		 * of nodes, and move tasks towards the group with the most
		 * memory accesses. When comparing two nodes at distance
		 * "hoplimit", only nodes closer by than "hoplimit" are part
		 * of each group. Skip other nodes.
		 */
		ಆದರೆ (sched_numa_topology_type == NUMA_BACKPLANE && dist >= lim_dist)
			ಮುಂದುವರಿ;

		/* Add up the faults from nearby nodes. */
		ಆದರೆ (task)
			ದೋಷಗಳು = task_ದೋಷಗಳು(p, ಗಂಟು);
		ಇಲ್ಲದಿದ್ದರೆ
			ದೋಷಗಳು = ಗುಂಪು_ದೋಷಗಳು(p, ಗಂಟು);

		/*
		 * On systems with a glueless mesh NUMA topology, there are
		 * no fixed "groups of nodes". Instead, nodes that are not
		 * directly connected bounce traffic through intermediate
		 * nodes; a numa_group can occupy any set of nodes.
		 * The further away a node is, the less the faults count.
		 * This seems to result in good task placement.
		 */
		ಆದರೆ (sched_numa_topology_type == NUMA_GLUELESS_MESH) {
			ದೋಷಗಳು *= (max_dist - dist);
			ದೋಷಗಳು /= (max_dist - LOCAL_ದೂರ);
		}

		score += ದೋಷಗಳು;
	}

	ಹಿಂತಿರುಗಿ score;
}

/*
 * These return the fraction of accesses done by a particular task, or
 * task group, on a particular numa node.  The group weight is given a
 * larger multiplier, in order to group tasks together that are almost
 * evenly spread out between numa nodes.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ task_ತೂಕ(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ nid,
					ಪೂರ್ಣಾಂಕ dist)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೋಷಗಳು, ಒಟ್ಟು_ದೋಷಗಳು;

	ಆದರೆ (!p->numa_ದೋಷಗಳು)
		ಹಿಂತಿರುಗಿ 0;

	ಒಟ್ಟು_ದೋಷಗಳು = p->ಒಟ್ಟು_numa_ದೋಷಗಳು;

	ಆದರೆ (!ಒಟ್ಟು_ದೋಷಗಳು)
		ಹಿಂತಿರುಗಿ 0;

	ದೋಷಗಳು = task_ದೋಷಗಳು(p, nid);
	ದೋಷಗಳು += score_nearby_nodes(p, nid, dist, true);

	ಹಿಂತಿರುಗಿ 1000 * ದೋಷಗಳು / ಒಟ್ಟು_ದೋಷಗಳು;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗುಂಪು_ತೂಕ(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ nid,
					 ಪೂರ್ಣಾಂಕ dist)
{
	ರಚನೆ numa_ಗುಂಪು *ng = deref_task_numa_ಗುಂಪು(p);
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೋಷಗಳು, ಒಟ್ಟು_ದೋಷಗಳು;

	ಆದರೆ (!ng)
		ಹಿಂತಿರುಗಿ 0;

	ಒಟ್ಟು_ದೋಷಗಳು = ng->ಒಟ್ಟು_ದೋಷಗಳು;

	ಆದರೆ (!ಒಟ್ಟು_ದೋಷಗಳು)
		ಹಿಂತಿರುಗಿ 0;

	ದೋಷಗಳು = ಗುಂಪು_ದೋಷಗಳು(p, nid);
	ದೋಷಗಳು += score_nearby_nodes(p, nid, dist, false);

	ಹಿಂತಿರುಗಿ 1000 * ದೋಷಗಳು / ಒಟ್ಟು_ದೋಷಗಳು;
}

/*
 * If memory tiering mode is enabled, cpupid of slow memory page is
 * used to record scan time instead of CPU and PID.  When tiering mode
 * is disabled at run time, the scan time (in cpupid) will be
 * interpreted as CPU and PID.  So CPU needs to be checked to avoid to
 * access out of array bound.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool cpupid_valid(ಪೂರ್ಣಾಂಕ cpupid)
{
	ಹಿಂತಿರುಗಿ cpupid_ಗಮ್ಯ_cpu(cpupid) < nr_cpu_ids;
}

/*
 * For memory tiering mode, if there are enough free pages (more than
 * enough watermark defined here) in fast memory node, to take full
 * advantage of fast memory capacity, all recently accessed slow
 * memory pages will be migrated to fast memory node without
 * considering hot threshold.
 */
ಸ್ಥಿರ bool pgdat_free_space_enough(ರಚನೆ pglist_ದತ್ತಾಂಶ *pgdat)
{
	ಪೂರ್ಣಾಂಕ z;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ enough_wmark;

	enough_wmark = max(1UL * 1024 * 1024 * 1024 >> ಪುಟ_ಸರಿಸು,
			   pgdat->ಗಂಟು_present_ಪುಟಗಳು >> 4);
	ಗಾಗಿ (z = pgdat->nr_zones - 1; z >= 0; z--) {
		ರಚನೆ zone *zone = pgdat->ಗಂಟು_zones + z;

		ಆದರೆ (!populated_zone(zone))
			ಮುಂದುವರಿ;

		ಆದರೆ (zone_watermark_ok(zone, 0,
				      promo_wmark_ಪುಟಗಳು(zone) + enough_wmark,
				      ZONE_MOVABLE, 0))
			ಹಿಂತಿರುಗಿ true;
	}
	ಹಿಂತಿರುಗಿ false;
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
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ numa_hint_fault_latency(ರಚನೆ folio *folio)
{
	ಪೂರ್ಣಾಂಕ ಕೊನೆಯ_ಸಮಯ, ಸಮಯ;

	ಸಮಯ = jiffies_ಗಮ್ಯ_msecs(jiffies);
	ಕೊನೆಯ_ಸಮಯ = folio_xchg_access_ಸಮಯ(folio, ಸಮಯ);

	ಹಿಂತಿರುಗಿ (ಸಮಯ - ಕೊನೆಯ_ಸಮಯ) & ಪುಟ_ACCESS_ಸಮಯ_ಮುಖವಾಡ;
}

/*
 * For memory tiering mode, too high promotion/demotion throughput may
 * hurt application latency.  So we provide a mechanism to rate limit
 * the number of pages that are tried to be promoted.
 */
ಸ್ಥಿರ bool numa_promotion_ದರ_ಎಲ್ಲೆ(ರಚನೆ pglist_ದತ್ತಾಂಶ *pgdat,
				      ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದರ_ಎಲ್ಲೆ, ಪೂರ್ಣಾಂಕ nr)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ nr_cand;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ now, ಆರಂಭ;

	now = jiffies_ಗಮ್ಯ_msecs(jiffies);
	mod_ಗಂಟು_ಪುಟ_state(pgdat, PGPROMOTE_CANDIDATE, nr);
	nr_cand = ಗಂಟು_ಪುಟ_state(pgdat, PGPROMOTE_CANDIDATE);
	ಆರಂಭ = pgdat->nbp_rl_ಆರಂಭ;
	ಆದರೆ (now - ಆರಂಭ > MSEC_ಪ್ರತಿ_SEC &&
	    cmpxchg(&pgdat->nbp_rl_ಆರಂಭ, ಆರಂಭ, now) == ಆರಂಭ)
		pgdat->nbp_rl_nr_cand = nr_cand;
	ಆದರೆ (nr_cand - pgdat->nbp_rl_nr_cand >= ದರ_ಎಲ್ಲೆ)
		ಹಿಂತಿರುಗಿ true;
	ಹಿಂತಿರುಗಿ false;
}

#ವ್ಯಾಖ್ಯಾನಿಸು NUMA_MIGRATION_ADJUST_ಹೆಜ್ಜೆಗಳು	16

ಸ್ಥಿರ ಶೂನ್ಯ numa_promotion_adjust_ಮಿತಿ(ರಚನೆ pglist_ದತ್ತಾಂಶ *pgdat,
					    ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದರ_ಎಲ್ಲೆ,
					    ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ref_th)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ now, ಆರಂಭ, th_period, unit_th, th;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ nr_cand, ref_cand, diff_cand;

	now = jiffies_ಗಮ್ಯ_msecs(jiffies);
	th_period = sysctl_numa_balancing_ಪರಿಶೀಲನೆ_period_max;
	ಆರಂಭ = pgdat->nbp_th_ಆರಂಭ;
	ಆದರೆ (now - ಆರಂಭ > th_period &&
	    cmpxchg(&pgdat->nbp_th_ಆರಂಭ, ಆರಂಭ, now) == ಆರಂಭ) {
		ref_cand = ದರ_ಎಲ್ಲೆ *
			sysctl_numa_balancing_ಪರಿಶೀಲನೆ_period_max / MSEC_ಪ್ರತಿ_SEC;
		nr_cand = ಗಂಟು_ಪುಟ_state(pgdat, PGPROMOTE_CANDIDATE);
		diff_cand = nr_cand - pgdat->nbp_th_nr_cand;
		unit_th = ref_th * 2 / NUMA_MIGRATION_ADJUST_ಹೆಜ್ಜೆಗಳು;
		th = pgdat->nbp_ಮಿತಿ ? : ref_th;
		ಆದರೆ (diff_cand > ref_cand * 11 / 10)
			th = max(th - unit_th, unit_th);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (diff_cand < ref_cand * 9 / 10)
			th = min(th + unit_th, ref_th * 2);
		pgdat->nbp_th_nr_cand = nr_cand;
		pgdat->nbp_ಮಿತಿ = th;
	}
}

bool should_numa_migrate_ಸ್ಮರಣೆ(ರಚನೆ task_struct *p, ರಚನೆ folio *folio,
				ಪೂರ್ಣಾಂಕ src_nid, ಪೂರ್ಣಾಂಕ dst_cpu)
{
	ರಚನೆ numa_ಗುಂಪು *ng = deref_curr_numa_ಗುಂಪು(p);
	ಪೂರ್ಣಾಂಕ dst_nid = cpu_ಗಮ್ಯ_ಗಂಟು(dst_cpu);
	ಪೂರ್ಣಾಂಕ ಕೊನೆಯ_cpupid, this_cpupid;

	/*
	 * Cannot migrate to memoryless nodes.
	 */
	ಆದರೆ (!ಗಂಟು_state(dst_nid, N_ಸ್ಮರಣೆ))
		ಹಿಂತಿರುಗಿ false;

	/*
	 * The pages in slow memory node should be migrated according
	 * to hot/cold instead of private/shared.
	 */
	ಆದರೆ (folio_use_access_ಸಮಯ(folio)) {
		ರಚನೆ pglist_ದತ್ತಾಂಶ *pgdat;
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದರ_ಎಲ್ಲೆ;
		ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ latency, th, def_th;
		ದೀರ್ಘ nr = folio_nr_ಪುಟಗಳು(folio);

		pgdat = ಗಂಟು_ದತ್ತಾಂಶ(dst_nid);
		ಆದರೆ (pgdat_free_space_enough(pgdat)) {
			/* workload changed, reset hot threshold */
			pgdat->nbp_ಮಿತಿ = 0;
			mod_ಗಂಟು_ಪುಟ_state(pgdat, PGPROMOTE_CANDIDATE_NRL, nr);
			ಹಿಂತಿರುಗಿ true;
		}

		def_th = sysctl_numa_balancing_hot_ಮಿತಿ;
		ದರ_ಎಲ್ಲೆ = MB_ಗಮ್ಯ_ಪುಟಗಳು(sysctl_numa_balancing_promote_ದರ_ಎಲ್ಲೆ);
		numa_promotion_adjust_ಮಿತಿ(pgdat, ದರ_ಎಲ್ಲೆ, def_th);

		th = pgdat->nbp_ಮಿತಿ ? : def_th;
		latency = numa_hint_fault_latency(folio);
		ಆದರೆ (latency >= th)
			ಹಿಂತಿರುಗಿ false;

		ಹಿಂತಿರುಗಿ !numa_promotion_ದರ_ಎಲ್ಲೆ(pgdat, ದರ_ಎಲ್ಲೆ, nr);
	}

	this_cpupid = cpu_pid_ಗಮ್ಯ_cpupid(dst_cpu, current->pid);
	ಕೊನೆಯ_cpupid = folio_xchg_ಕೊನೆಯ_cpupid(folio, this_cpupid);

	ಆದರೆ (!(sysctl_numa_balancing_ಬಹುಲಕ & NUMA_BALANCING_ಸ್ಮರಣೆ_TIERING) &&
	    !ಗಂಟು_is_toptier(src_nid) && !cpupid_valid(ಕೊನೆಯ_cpupid))
		ಹಿಂತಿರುಗಿ false;

	/*
	 * Allow first faults or private faults to migrate immediately early in
	 * the lifetime of a task. The magic number 4 is based on waiting for
	 * two full passes of the "multi-stage node selection" test that is
	 * executed below.
	 */
	ಆದರೆ ((p->numa_preferred_nid == NUMA_NO_ಗಂಟು || p->numa_ಪರಿಶೀಲನೆ_seq <= 4) &&
	    (cpupid_pid_unset(ಕೊನೆಯ_cpupid) || cpupid_ಹೊಂದಿಕೆ_pid(p, ಕೊನೆಯ_cpupid)))
		ಹಿಂತಿರುಗಿ true;

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
	ಆದರೆ (!cpupid_pid_unset(ಕೊನೆಯ_cpupid) &&
				cpupid_ಗಮ್ಯ_nid(ಕೊನೆಯ_cpupid) != dst_nid)
		ಹಿಂತಿರುಗಿ false;

	/* Always allow migrate on private faults */
	ಆದರೆ (cpupid_ಹೊಂದಿಕೆ_pid(p, ಕೊನೆಯ_cpupid))
		ಹಿಂತಿರುಗಿ true;

	/* A shared fault, but p->numa_group has not been set up yet. */
	ಆದರೆ (!ng)
		ಹಿಂತಿರುಗಿ true;

	/*
	 * Destination node is much more heavily used than the source
	 * node? Allow migration.
	 */
	ಆದರೆ (ಗುಂಪು_ದೋಷಗಳು_cpu(ng, dst_nid) > ಗುಂಪು_ದೋಷಗಳು_cpu(ng, src_nid) *
					ಸಕ್ರಿಯ_ಗಂಟು_FRACTION)
		ಹಿಂತಿರುಗಿ true;

	/*
	 * Distribute memory according to CPU & memory use on each node,
	 * with 3/4 hysteresis to avoid unnecessary memory migrations:
	 *
	 * faults_cpu(dst)   3   faults_cpu(src)
	 * --------------- * - > ---------------
	 * faults_mem(dst)   4   faults_mem(src)
	 */
	ಹಿಂತಿರುಗಿ ಗುಂಪು_ದೋಷಗಳು_cpu(ng, dst_nid) * ಗುಂಪು_ದೋಷಗಳು(p, src_nid) * 3 >
	       ಗುಂಪು_ದೋಷಗಳು_cpu(ng, src_nid) * ಗುಂಪು_ದೋಷಗಳು(p, dst_nid) * 4;
}

/*
 * 'numa_type' describes the node at the moment of load balancing.
 */
ಎಣಿಕೆ numa_type {
	/* The node has spare capacity that can be used to run more tasks.  */
	ಗಂಟು_has_ಹೆಚ್ಚುವರಿ = 0,
	/*
	 * The node is fully used and the tasks don't compete for more CPU
	 * cycles. Nevertheless, some tasks might wait before running.
	 */
	ಗಂಟು_fully_busy,
	/*
	 * The node is overloaded and can't provide expected CPU cycles to all
	 * tasks.
	 */
	ಗಂಟು_overloaded
};

/* Cached statistics for all CPUs within a node */
ರಚನೆ numa_stats {
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ತುಂಬು;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ runnable;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ util;
	/* Total compute capacity of CPUs on a node */
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ compute_ಸಾಮರ್ಥ್ಯ;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ nr_running;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ತೂಕ;
	ಎಣಿಕೆ numa_type ಗಂಟು_type;
	ಪೂರ್ಣಾಂಕ idle_cpu;
};

ರಚನೆ task_numa_env {
	ರಚನೆ task_struct *p;

	ಪೂರ್ಣಾಂಕ src_cpu, src_nid;
	ಪೂರ್ಣಾಂಕ dst_cpu, dst_nid;
	ಪೂರ್ಣಾಂಕ imb_numa_nr;

	ರಚನೆ numa_stats src_stats, dst_stats;

	ಪೂರ್ಣಾಂಕ imbalance_pct;
	ಪೂರ್ಣಾಂಕ dist;

	ರಚನೆ task_struct *ಉತ್ತಮ_task;
	ದೀರ್ಘ ಉತ್ತಮ_imp;
	ಪೂರ್ಣಾಂಕ ಉತ್ತಮ_cpu;
};

ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ cpu_ತುಂಬು(ರಚನೆ rq *rq);
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ cpu_runnable(ರಚನೆ rq *rq);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಎಣಿಕೆ
numa_type numa_classify(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ imbalance_pct,
			 ರಚನೆ numa_stats *ns)
{
	ಆದರೆ ((ns->nr_running > ns->ತೂಕ) &&
	    (((ns->compute_ಸಾಮರ್ಥ್ಯ * 100) < (ns->util * imbalance_pct)) ||
	     ((ns->compute_ಸಾಮರ್ಥ್ಯ * imbalance_pct) < (ns->runnable * 100))))
		ಹಿಂತಿರುಗಿ ಗಂಟು_overloaded;

	ಆದರೆ ((ns->nr_running < ns->ತೂಕ) ||
	    (((ns->compute_ಸಾಮರ್ಥ್ಯ * 100) > (ns->util * imbalance_pct)) &&
	     ((ns->compute_ಸಾಮರ್ಥ್ಯ * imbalance_pct) > (ns->runnable * 100))))
		ಹಿಂತಿರುಗಿ ಗಂಟು_has_ಹೆಚ್ಚುವರಿ;

	ಹಿಂತಿರುಗಿ ಗಂಟು_fully_busy;
}

/* Forward declarations of select_idle_sibling helpers */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool test_idle_cores(ಪೂರ್ಣಾಂಕ cpu);
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ numa_idle_core(ಪೂರ್ಣಾಂಕ idle_core, ಪೂರ್ಣಾಂಕ cpu)
{
	ಆದರೆ (!sched_smt_ಸಕ್ರಿಯ() ||
	    idle_core >= 0 || !test_idle_cores(cpu))
		ಹಿಂತಿರುಗಿ idle_core;

	/*
	 * Prefer cores instead of packing HT siblings
	 * and triggering future load balancing.
	 */
	ಆದರೆ (is_core_idle(cpu))
		idle_core = cpu;

	ಹಿಂತಿರುಗಿ idle_core;
}

/*
 * Gather all necessary information to make NUMA balancing placement
 * decisions that are compatible with standard load balancer. This
 * borrows code and logic from update_sg_lb_stats but sharing a
 * common implementation is impractical.
 */
ಸ್ಥಿರ ಶೂನ್ಯ update_numa_stats(ರಚನೆ task_numa_env *env,
			      ರಚನೆ numa_stats *ns, ಪೂರ್ಣಾಂಕ nid,
			      bool ಕಂಡುಹಿಡಿ_idle)
{
	ಪೂರ್ಣಾಂಕ cpu, idle_core = -1;

	memset(ns, 0, ಗಾತ್ರ(*ns));
	ns->idle_cpu = -1;

	rcu_read_ಬೀಗ();
	for_each_cpu(cpu, cpumask_ದ_ಗಂಟು(nid)) {
		ರಚನೆ rq *rq = cpu_rq(cpu);

		ns->ತುಂಬು += cpu_ತುಂಬು(rq);
		ns->runnable += cpu_runnable(rq);
		ns->util += cpu_util_cfs(cpu);
		ns->nr_running += rq->cfs.h_nr_runnable;
		ns->compute_ಸಾಮರ್ಥ್ಯ += ಸಾಮರ್ಥ್ಯ_ದ(cpu);

		ಆದರೆ (ಕಂಡುಹಿಡಿ_idle && idle_core < 0 && !rq->nr_running && idle_cpu(cpu)) {
			ಆದರೆ (READ_ONCE(rq->numa_migrate_on) ||
			    !cpumask_test_cpu(cpu, env->p->cpus_ptr))
				ಮುಂದುವರಿ;

			ಆದರೆ (ns->idle_cpu == -1)
				ns->idle_cpu = cpu;

			idle_core = numa_idle_core(idle_core, cpu);
		}
	}
	rcu_read_unlock();

	ns->ತೂಕ = cpumask_ತೂಕ(cpumask_ದ_ಗಂಟು(nid));

	ns->ಗಂಟು_type = numa_classify(env->imbalance_pct, ns);

	ಆದರೆ (idle_core >= 0)
		ns->idle_cpu = idle_core;
}

ಸ್ಥಿರ ಶೂನ್ಯ task_numa_ನಿಯೋಜಿಸು(ರಚನೆ task_numa_env *env,
			     ರಚನೆ task_struct *p, ದೀರ್ಘ imp)
{
	ರಚನೆ rq *rq = cpu_rq(env->dst_cpu);

	/* Check if run-queue part of active NUMA balance. */
	ಆದರೆ (env->ಉತ್ತಮ_cpu != env->dst_cpu && xchg(&rq->numa_migrate_on, 1)) {
		ಪೂರ್ಣಾಂಕ cpu;
		ಪೂರ್ಣಾಂಕ ಆರಂಭ = env->dst_cpu;

		/* Find alternative idle CPU. */
		for_each_cpu_wrap(cpu, cpumask_ದ_ಗಂಟು(env->dst_nid), ಆರಂಭ + 1) {
			ಆದರೆ (cpu == env->ಉತ್ತಮ_cpu || !idle_cpu(cpu) ||
			    !cpumask_test_cpu(cpu, env->p->cpus_ptr)) {
				ಮುಂದುವರಿ;
			}

			env->dst_cpu = cpu;
			rq = cpu_rq(env->dst_cpu);
			ಆದರೆ (!xchg(&rq->numa_migrate_on, 1))
				ಹೋಗು ನಿಯೋಜಿಸು;
		}

		/* Failed to find an alternative idle CPU */
		ಹಿಂತಿರುಗಿ;
	}

ನಿಯೋಜಿಸು:
	/*
	 * Clear previous best_cpu/rq numa-migrate flag, since task now
	 * found a better CPU to move/swap.
	 */
	ಆದರೆ (env->ಉತ್ತಮ_cpu != -1 && env->ಉತ್ತಮ_cpu != env->dst_cpu) {
		rq = cpu_rq(env->ಉತ್ತಮ_cpu);
		WRITE_ONCE(rq->numa_migrate_on, 0);
	}

	ಆದರೆ (env->ಉತ್ತಮ_task)
		put_task_struct(env->ಉತ್ತಮ_task);
	ಆದರೆ (p)
		get_task_struct(p);

	env->ಉತ್ತಮ_task = p;
	env->ಉತ್ತಮ_imp = imp;
	env->ಉತ್ತಮ_cpu = env->dst_cpu;
}

ಸ್ಥಿರ bool ತುಂಬು_too_imbalanced(ದೀರ್ಘ src_ತುಂಬು, ದೀರ್ಘ dst_ತುಂಬು,
				ರಚನೆ task_numa_env *env)
{
	ದೀರ್ಘ imb, old_imb;
	ದೀರ್ಘ orig_src_ತುಂಬು, orig_dst_ತುಂಬು;
	ದೀರ್ಘ src_ಸಾಮರ್ಥ್ಯ, dst_ಸಾಮರ್ಥ್ಯ;

	/*
	 * The load is corrected for the CPU capacity available on each node.
	 *
	 * src_load        dst_load
	 * ------------ vs ---------
	 * src_capacity    dst_capacity
	 */
	src_ಸಾಮರ್ಥ್ಯ = env->src_stats.compute_ಸಾಮರ್ಥ್ಯ;
	dst_ಸಾಮರ್ಥ್ಯ = env->dst_stats.compute_ಸಾಮರ್ಥ್ಯ;

	imb = ನಿರಪೇಕ್ಷ(dst_ತುಂಬು * src_ಸಾಮರ್ಥ್ಯ - src_ತುಂಬು * dst_ಸಾಮರ್ಥ್ಯ);

	orig_src_ತುಂಬು = env->src_stats.ತುಂಬು;
	orig_dst_ತುಂಬು = env->dst_stats.ತುಂಬು;

	old_imb = ನಿರಪೇಕ್ಷ(orig_dst_ತುಂಬು * src_ಸಾಮರ್ಥ್ಯ - orig_src_ತುಂಬು * dst_ಸಾಮರ್ಥ್ಯ);

	/* Would this change make things worse? */
	ಹಿಂತಿರುಗಿ (imb > old_imb);
}

/*
 * Maximum NUMA importance can be 1998 (2*999);
 * SMALLIMP @ 30 would be close to 1998/64.
 * Used to deter task migration.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು SMALLIMP	30

/*
 * This checks if the overall compute and NUMA accesses of the system would
 * be improved if the source tasks was migrated to the target dst_cpu taking
 * into account that it might be best if task running on the dst_cpu should
 * be exchanged with the source task
 */
ಸ್ಥಿರ bool task_numa_compare(ರಚನೆ task_numa_env *env,
			      ದೀರ್ಘ taskimp, ದೀರ್ಘ groupimp, bool maymove)
{
	ರಚನೆ numa_ಗುಂಪು *cur_ng, *p_ng = deref_curr_numa_ಗುಂಪು(env->p);
	ರಚನೆ rq *dst_rq = cpu_rq(env->dst_cpu);
	ದೀರ್ಘ imp = p_ng ? groupimp : taskimp;
	ರಚನೆ task_struct *cur;
	ದೀರ್ಘ src_ತುಂಬು, dst_ತುಂಬು;
	ಪೂರ್ಣಾಂಕ dist = env->dist;
	ದೀರ್ಘ moveimp = imp;
	ದೀರ್ಘ ತುಂಬು;
	bool stopsearch = false;

	ಆದರೆ (READ_ONCE(dst_rq->numa_migrate_on))
		ಹಿಂತಿರುಗಿ false;

	rcu_read_ಬೀಗ();
	cur = rcu_dereference_all(dst_rq->curr);
	ಆದರೆ (cur && ((cur->flags & (PF_EXITING | PF_KTHREAD)) ||
		    !cur->mm))
		cur = ಖಾಲಿ;

	/*
	 * Because we have preemption enabled we can get migrated around and
	 * end try selecting ourselves (current == env->p) as a swap candidate.
	 */
	ಆದರೆ (cur == env->p) {
		stopsearch = true;
		ಹೋಗು unlock;
	}

	ಆದರೆ (!cur) {
		ಆದರೆ (maymove && moveimp >= env->ಉತ್ತಮ_imp)
			ಹೋಗು ನಿಯೋಜಿಸು;
		ಇಲ್ಲದಿದ್ದರೆ
			ಹೋಗು unlock;
	}

	/* Skip this swap candidate if cannot move to the source cpu. */
	ಆದರೆ (!cpumask_test_cpu(env->src_cpu, cur->cpus_ptr))
		ಹೋಗು unlock;

	/*
	 * Skip this swap candidate if it is not moving to its preferred
	 * node and the best task is.
	 */
	ಆದರೆ (env->ಉತ್ತಮ_task &&
	    env->ಉತ್ತಮ_task->numa_preferred_nid == env->src_nid &&
	    cur->numa_preferred_nid != env->src_nid) {
		ಹೋಗು unlock;
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
	cur_ng = rcu_dereference_all(cur->numa_ಗುಂಪು);
	ಆದರೆ (cur_ng == p_ng) {
		/*
		 * Do not swap within a group or between tasks that have
		 * no group if there is spare capacity. Swapping does
		 * not address the load imbalance and helps one task at
		 * the cost of punishing another.
		 */
		ಆದರೆ (env->dst_stats.ಗಂಟು_type == ಗಂಟು_has_ಹೆಚ್ಚುವರಿ)
			ಹೋಗು unlock;

		imp = taskimp + task_ತೂಕ(cur, env->src_nid, dist) -
		      task_ತೂಕ(cur, env->dst_nid, dist);
		/*
		 * Add some hysteresis to prevent swapping the
		 * tasks within a group over tiny differences.
		 */
		ಆದರೆ (cur_ng)
			imp -= imp / 16;
	} ಇಲ್ಲದಿದ್ದರೆ {
		/*
		 * Compare the group weights. If a task is all by itself
		 * (not part of a group), use the task weight instead.
		 */
		ಆದರೆ (cur_ng && p_ng)
			imp += ಗುಂಪು_ತೂಕ(cur, env->src_nid, dist) -
			       ಗುಂಪು_ತೂಕ(cur, env->dst_nid, dist);
		ಇಲ್ಲದಿದ್ದರೆ
			imp += task_ತೂಕ(cur, env->src_nid, dist) -
			       task_ತೂಕ(cur, env->dst_nid, dist);
	}

	/* Discourage picking a task already on its preferred node */
	ಆದರೆ (cur->numa_preferred_nid == env->dst_nid)
		imp -= imp / 16;

	/*
	 * Encourage picking a task that moves to its preferred node.
	 * This potentially makes imp larger than it's maximum of
	 * 1998 (see SMALLIMP and task_weight for why) but in this
	 * case, it does not matter.
	 */
	ಆದರೆ (cur->numa_preferred_nid == env->src_nid)
		imp += imp / 8;

	ಆದರೆ (maymove && moveimp > imp && moveimp > env->ಉತ್ತಮ_imp) {
		imp = moveimp;
		cur = ಖಾಲಿ;
		ಹೋಗು ನಿಯೋಜಿಸು;
	}

	/*
	 * Prefer swapping with a task moving to its preferred node over a
	 * task that is not.
	 */
	ಆದರೆ (env->ಉತ್ತಮ_task && cur->numa_preferred_nid == env->src_nid &&
	    env->ಉತ್ತಮ_task->numa_preferred_nid != env->src_nid) {
		ಹೋಗು ನಿಯೋಜಿಸು;
	}

	/*
	 * If the NUMA importance is less than SMALLIMP,
	 * task migration might only result in ping pong
	 * of tasks and also hurt performance due to cache
	 * misses.
	 */
	ಆದರೆ (imp < SMALLIMP || imp <= env->ಉತ್ತಮ_imp + SMALLIMP / 2)
		ಹೋಗು unlock;

	/*
	 * In the overloaded case, try and keep the load balanced.
	 */
	ತುಂಬು = task_h_ತುಂಬು(env->p) - task_h_ತುಂಬು(cur);
	ಆದರೆ (!ತುಂಬು)
		ಹೋಗು ನಿಯೋಜಿಸು;

	dst_ತುಂಬು = env->dst_stats.ತುಂಬು + ತುಂಬು;
	src_ತುಂಬು = env->src_stats.ತುಂಬು - ತುಂಬು;

	ಆದರೆ (ತುಂಬು_too_imbalanced(src_ತುಂಬು, dst_ತುಂಬು, env))
		ಹೋಗು unlock;

ನಿಯೋಜಿಸು:
	/* Evaluate an idle CPU for a task numa move. */
	ಆದರೆ (!cur) {
		ಪೂರ್ಣಾಂಕ cpu = env->dst_stats.idle_cpu;

		/* Nothing cached so current CPU went idle since the search. */
		ಆದರೆ (cpu < 0)
			cpu = env->dst_cpu;

		/*
		 * If the CPU is no longer truly idle and the previous best CPU
		 * is, keep using it.
		 */
		ಆದರೆ (!idle_cpu(cpu) && env->ಉತ್ತಮ_cpu >= 0 &&
		    idle_cpu(env->ಉತ್ತಮ_cpu)) {
			cpu = env->ಉತ್ತಮ_cpu;
		}

		env->dst_cpu = cpu;
	}

	task_numa_ನಿಯೋಜಿಸು(env, cur, imp);

	/*
	 * If a move to idle is allowed because there is capacity or load
	 * balance improves then stop the search. While a better swap
	 * candidate may exist, a search is not free.
	 */
	ಆದರೆ (maymove && !cur && env->ಉತ್ತಮ_cpu >= 0 && idle_cpu(env->ಉತ್ತಮ_cpu))
		stopsearch = true;

	/*
	 * If a swap candidate must be identified and the current best task
	 * moves its preferred node then stop the search.
	 */
	ಆದರೆ (!maymove && env->ಉತ್ತಮ_task &&
	    env->ಉತ್ತಮ_task->numa_preferred_nid == env->src_nid) {
		stopsearch = true;
	}
unlock:
	rcu_read_unlock();

	ಹಿಂತಿರುಗಿ stopsearch;
}

ಸ್ಥಿರ ಶೂನ್ಯ task_numa_ಕಂಡುಹಿಡಿ_cpu(ರಚನೆ task_numa_env *env,
				ದೀರ್ಘ taskimp, ದೀರ್ಘ groupimp)
{
	bool maymove = false;
	ಪೂರ್ಣಾಂಕ cpu;

	/*
	 * If dst node has spare capacity, then check if there is an
	 * imbalance that would be overruled by the load balancer.
	 */
	ಆದರೆ (env->dst_stats.ಗಂಟು_type == ಗಂಟು_has_ಹೆಚ್ಚುವರಿ) {
		ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ imbalance;
		ಪೂರ್ಣಾಂಕ src_running, dst_running;

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
		ಆದರೆ (!imbalance) {
			maymove = true;
			ಆದರೆ (env->dst_stats.idle_cpu >= 0) {
				env->dst_cpu = env->dst_stats.idle_cpu;
				task_numa_ನಿಯೋಜಿಸು(env, ಖಾಲಿ, 0);
				ಹಿಂತಿರುಗಿ;
			}
		}
	} ಇಲ್ಲದಿದ್ದರೆ {
		ದೀರ್ಘ src_ತುಂಬು, dst_ತುಂಬು, ತುಂಬು;
		/*
		 * If the improvement from just moving env->p direction is better
		 * than swapping tasks around, check if a move is possible.
		 */
		ತುಂಬು = task_h_ತುಂಬು(env->p);
		dst_ತುಂಬು = env->dst_stats.ತುಂಬು + ತುಂಬು;
		src_ತುಂಬು = env->src_stats.ತುಂಬು - ತುಂಬು;
		maymove = !ತುಂಬು_too_imbalanced(src_ತುಂಬು, dst_ತುಂಬು, env);
	}

	/* Skip CPUs if the source task cannot migrate */
	for_each_cpu_and(cpu, cpumask_ದ_ಗಂಟು(env->dst_nid), env->p->cpus_ptr) {
		env->dst_cpu = cpu;
		ಆದರೆ (task_numa_compare(env, taskimp, groupimp, maymove))
			ನಿಲ್ಲಿಸು;
	}
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ task_numa_migrate(ರಚನೆ task_struct *p)
{
	ರಚನೆ task_numa_env env = {
		.p = p,

		.src_cpu = task_cpu(p),
		.src_nid = task_ಗಂಟು(p),

		.imbalance_pct = 112,

		.ಉತ್ತಮ_task = ಖಾಲಿ,
		.ಉತ್ತಮ_imp = 0,
		.ಉತ್ತಮ_cpu = -1,
	};
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ taskweight, groupweight;
	ರಚನೆ sched_domain *sd;
	ದೀರ್ಘ taskimp, groupimp;
	ರಚನೆ numa_ಗುಂಪು *ng;
	ರಚನೆ rq *ಉತ್ತಮ_rq;
	ಪೂರ್ಣಾಂಕ nid, ret, dist;

	/*
	 * Pick the lowest SD_NUMA domain, as that would have the smallest
	 * imbalance and would be the first to start moving tasks about.
	 *
	 * And we want to avoid any moving of tasks about, as that would create
	 * random movement of tasks -- counter the numa conditions we're trying
	 * to satisfy here.
	 */
	rcu_read_ಬೀಗ();
	sd = rcu_dereference_all(ಪ್ರತಿ_cpu(sd_numa, env.src_cpu));
	ಆದರೆ (sd) {
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
	ಆದರೆ (unlikely(!sd)) {
		sched_setnuma(p, task_ಗಂಟು(p));
		ಹಿಂತಿರುಗಿ -EINVAL;
	}

	env.dst_nid = p->numa_preferred_nid;
	dist = env.dist = ಗಂಟು_ದೂರ(env.src_nid, env.dst_nid);
	taskweight = task_ತೂಕ(p, env.src_nid, dist);
	groupweight = ಗುಂಪು_ತೂಕ(p, env.src_nid, dist);
	update_numa_stats(&env, &env.src_stats, env.src_nid, false);
	taskimp = task_ತೂಕ(p, env.dst_nid, dist) - taskweight;
	groupimp = ಗುಂಪು_ತೂಕ(p, env.dst_nid, dist) - groupweight;
	update_numa_stats(&env, &env.dst_stats, env.dst_nid, true);

	/* Try to find a spot on the preferred nid. */
	task_numa_ಕಂಡುಹಿಡಿ_cpu(&env, taskimp, groupimp);

	/*
	 * Look at other nodes in these cases:
	 * - there is no space available on the preferred_nid
	 * - the task is part of a numa_group that is interleaved across
	 *   multiple NUMA nodes; in order to better consolidate the group,
	 *   we need to check other locations.
	 */
	ng = deref_curr_numa_ಗುಂಪು(p);
	ಆದರೆ (env.ಉತ್ತಮ_cpu == -1 || (ng && ng->ಸಕ್ರಿಯ_nodes > 1)) {
		for_each_ಗಂಟು_state(nid, N_CPU) {
			ಆದರೆ (nid == env.src_nid || nid == p->numa_preferred_nid)
				ಮುಂದುವರಿ;

			dist = ಗಂಟು_ದೂರ(env.src_nid, env.dst_nid);
			ಆದರೆ (sched_numa_topology_type == NUMA_BACKPLANE &&
						dist != env.dist) {
				taskweight = task_ತೂಕ(p, env.src_nid, dist);
				groupweight = ಗುಂಪು_ತೂಕ(p, env.src_nid, dist);
			}

			/* Only consider nodes where both task and groups benefit */
			taskimp = task_ತೂಕ(p, nid, dist) - taskweight;
			groupimp = ಗುಂಪು_ತೂಕ(p, nid, dist) - groupweight;
			ಆದರೆ (taskimp < 0 && groupimp < 0)
				ಮುಂದುವರಿ;

			env.dist = dist;
			env.dst_nid = nid;
			update_numa_stats(&env, &env.dst_stats, env.dst_nid, true);
			task_numa_ಕಂಡುಹಿಡಿ_cpu(&env, taskimp, groupimp);
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
	ಆದರೆ (ng) {
		ಆದರೆ (env.ಉತ್ತಮ_cpu == -1)
			nid = env.src_nid;
		ಇಲ್ಲದಿದ್ದರೆ
			nid = cpu_ಗಮ್ಯ_ಗಂಟು(env.ಉತ್ತಮ_cpu);

		ಆದರೆ (nid != p->numa_preferred_nid)
			sched_setnuma(p, nid);
	}

	/* No better CPU than the current one was found. */
	ಆದರೆ (env.ಉತ್ತಮ_cpu == -1) {
		trace_sched_stick_numa(p, env.src_cpu, ಖಾಲಿ, -1);
		ಹಿಂತಿರುಗಿ -EAGAIN;
	}

	ಉತ್ತಮ_rq = cpu_rq(env.ಉತ್ತಮ_cpu);
	ಆದರೆ (env.ಉತ್ತಮ_task == ಖಾಲಿ) {
		ret = migrate_task_ಗಮ್ಯ(p, env.ಉತ್ತಮ_cpu);
		WRITE_ONCE(ಉತ್ತಮ_rq->numa_migrate_on, 0);
		ಆದರೆ (ret != 0)
			trace_sched_stick_numa(p, env.src_cpu, ಖಾಲಿ, env.ಉತ್ತಮ_cpu);
		ಹಿಂತಿರುಗಿ ret;
	}

	ret = migrate_ಅದಲುಬದಲು(p, env.ಉತ್ತಮ_task, env.ಉತ್ತಮ_cpu, env.src_cpu);
	WRITE_ONCE(ಉತ್ತಮ_rq->numa_migrate_on, 0);

	ಆದರೆ (ret != 0)
		trace_sched_stick_numa(p, env.src_cpu, env.ಉತ್ತಮ_task, env.ಉತ್ತಮ_cpu);
	put_task_struct(env.ಉತ್ತಮ_task);
	ಹಿಂತಿರುಗಿ ret;
}

/* Attempt to migrate a task to a CPU on the preferred node. */
ಸ್ಥಿರ ಶೂನ್ಯ numa_migrate_preferred(ರಚನೆ task_struct *p)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ interval = HZ;

	/* This task has no NUMA fault statistics yet */
	ಆದರೆ (unlikely(p->numa_preferred_nid == NUMA_NO_ಗಂಟು || !p->numa_ದೋಷಗಳು))
		ಹಿಂತಿರುಗಿ;

	/* Periodically retry migrating the task to the preferred node */
	interval = min(interval, msecs_ಗಮ್ಯ_jiffies(p->numa_ಪರಿಶೀಲನೆ_period) / 16);
	p->numa_migrate_retry = jiffies + interval;

	/* Success if task is already running on preferred CPU */
	ಆದರೆ (task_ಗಂಟು(p) == p->numa_preferred_nid)
		ಹಿಂತಿರುಗಿ;

	/* Otherwise, try migrate to a CPU on the preferred node */
	task_numa_migrate(p);
}

/*
 * Find out how many nodes the workload is actively running on. Do this by
 * tracking the nodes from which NUMA hinting faults are triggered. This can
 * be different from the set of nodes where the workload's memory is currently
 * located.
 */
ಸ್ಥಿರ ಶೂನ್ಯ numa_ಗುಂಪು_ಎಣಿಕೆ_ಸಕ್ರಿಯ_nodes(ರಚನೆ numa_ಗುಂಪು *numa_ಗುಂಪು)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೋಷಗಳು, max_ದೋಷಗಳು = 0;
	ಪೂರ್ಣಾಂಕ nid, ಸಕ್ರಿಯ_nodes = 0;

	for_each_ಗಂಟು_state(nid, N_CPU) {
		ದೋಷಗಳು = ಗುಂಪು_ದೋಷಗಳು_cpu(numa_ಗುಂಪು, nid);
		ಆದರೆ (ದೋಷಗಳು > max_ದೋಷಗಳು)
			max_ದೋಷಗಳು = ದೋಷಗಳು;
	}

	for_each_ಗಂಟು_state(nid, N_CPU) {
		ದೋಷಗಳು = ಗುಂಪು_ದೋಷಗಳು_cpu(numa_ಗುಂಪು, nid);
		ಆದರೆ (ದೋಷಗಳು * ಸಕ್ರಿಯ_ಗಂಟು_FRACTION > max_ದೋಷಗಳು)
			ಸಕ್ರಿಯ_nodes++;
	}

	numa_ಗುಂಪು->max_ದೋಷಗಳು_cpu = max_ದೋಷಗಳು;
	numa_ಗುಂಪು->ಸಕ್ರಿಯ_nodes = ಸಕ್ರಿಯ_nodes;
}

/*
 * When adapting the scan rate, the period is divided into NUMA_PERIOD_SLOTS
 * increments. The more local the fault statistics are, the higher the scan
 * period will be for the next scan window. If local/(local+remote) ratio is
 * below NUMA_PERIOD_THRESHOLD (where range of ratio is 1..NUMA_PERIOD_SLOTS)
 * the scan period will decrease. Aim for 70% local accesses.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು NUMA_PERIOD_SLOTS 10
#ವ್ಯಾಖ್ಯಾನಿಸು NUMA_PERIOD_ಮಿತಿ 7

/*
 * Increase the scan period (slow down scanning) if the majority of
 * our memory is already on our local node, or if the majority of
 * the page accesses are shared with other processes.
 * Otherwise, decrease the scan period.
 */
ಸ್ಥಿರ ಶೂನ್ಯ update_task_ಪರಿಶೀಲನೆ_period(ರಚನೆ task_struct *p,
			ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ shared, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಖಾಸಗಿ)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ period_ಸ್ಥಾನ;
	ಪೂರ್ಣಾಂಕ lr_ಅನುಪಾತ, ps_ಅನುಪಾತ;
	ಪೂರ್ಣಾಂಕ diff;

	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ remote = p->numa_ದೋಷಗಳು_locality[0];
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ local = p->numa_ದೋಷಗಳು_locality[1];

	/*
	 * If there were no record hinting faults then either the task is
	 * completely idle or all activity is in areas that are not of interest
	 * to automatic numa balancing. Related to that, if there were failed
	 * migration then it implies we are migrating too quickly or the local
	 * node is overloaded. In either case, scan slower
	 */
	ಆದರೆ (local + shared == 0 || p->numa_ದೋಷಗಳು_locality[2]) {
		p->numa_ಪರಿಶೀಲನೆ_period = min(p->numa_ಪರಿಶೀಲನೆ_period_max,
			p->numa_ಪರಿಶೀಲನೆ_period << 1);

		p->mm->numa_ಮುಂದಿನ_ಪರಿಶೀಲನೆ = jiffies +
			msecs_ಗಮ್ಯ_jiffies(p->numa_ಪರಿಶೀಲನೆ_period);

		ಹೋಗು ಹೊರಹೋಗು;
	}

	/*
	 * Prepare to scale scan period relative to the current period.
	 *	 == NUMA_PERIOD_THRESHOLD scan period stays the same
	 *       <  NUMA_PERIOD_THRESHOLD scan period decreases (scan faster)
	 *	 >= NUMA_PERIOD_THRESHOLD scan period increases (scan slower)
	 */
	period_ಸ್ಥಾನ = DIV_ಸುತ್ತು_ಮೇಲೆ(p->numa_ಪರಿಶೀಲನೆ_period, NUMA_PERIOD_SLOTS);
	lr_ಅನುಪಾತ = (local * NUMA_PERIOD_SLOTS) / (local + remote);
	ps_ಅನುಪಾತ = (ಖಾಸಗಿ * NUMA_PERIOD_SLOTS) / (ಖಾಸಗಿ + shared);

	ಆದರೆ (ps_ಅನುಪಾತ >= NUMA_PERIOD_ಮಿತಿ) {
		/*
		 * Most memory accesses are local. There is no need to
		 * do fast NUMA scanning, since memory is already local.
		 */
		ಪೂರ್ಣಾಂಕ ಸ್ಥಾನ = ps_ಅನುಪಾತ - NUMA_PERIOD_ಮಿತಿ;
		ಆದರೆ (!ಸ್ಥಾನ)
			ಸ್ಥಾನ = 1;
		diff = ಸ್ಥಾನ * period_ಸ್ಥಾನ;
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (lr_ಅನುಪಾತ >= NUMA_PERIOD_ಮಿತಿ) {
		/*
		 * Most memory accesses are shared with other tasks.
		 * There is no point in continuing fast NUMA scanning,
		 * since other tasks may just move the memory elsewhere.
		 */
		ಪೂರ್ಣಾಂಕ ಸ್ಥಾನ = lr_ಅನುಪಾತ - NUMA_PERIOD_ಮಿತಿ;
		ಆದರೆ (!ಸ್ಥಾನ)
			ಸ್ಥಾನ = 1;
		diff = ಸ್ಥಾನ * period_ಸ್ಥಾನ;
	} ಇಲ್ಲದಿದ್ದರೆ {
		/*
		 * Private memory faults exceed (SLOTS-THRESHOLD)/SLOTS,
		 * yet they are not on the local NUMA node. Speed up
		 * NUMA scanning to get the memory moved over.
		 */
		ಪೂರ್ಣಾಂಕ ಅನುಪಾತ = max(lr_ಅನುಪಾತ, ps_ಅನುಪಾತ);
		diff = -(NUMA_PERIOD_ಮಿತಿ - ಅನುಪಾತ) * period_ಸ್ಥಾನ;
	}

	p->numa_ಪರಿಶೀಲನೆ_period = clamp(p->numa_ಪರಿಶೀಲನೆ_period + diff,
			task_ಪರಿಶೀಲನೆ_min(p), task_ಪರಿಶೀಲನೆ_max(p));

ಹೊರಹೋಗು:
	memset(p->numa_ದೋಷಗಳು_locality, 0,
	       ಗಾತ್ರ(p->numa_ದೋಷಗಳು_locality));
}

/*
 * Get the fraction of time the task has been running since the last
 * NUMA placement cycle. The scheduler keeps similar statistics, but
 * decays those on a 32ms period, which is orders of magnitude off
 * from the dozens-of-seconds NUMA balancing period. Use the scheduler
 * stats only if the task is so new there are no NUMA statistics yet.
 */
ಸ್ಥಿರ u64 numa_get_avg_runtime(ರಚನೆ task_struct *p, u64 *period)
{
	u64 runtime, delta, now;
	/* Use the start of this time slice to avoid calculations. */
	now = p->se.exec_ಆರಂಭ;
	runtime = p->se.ಮೊತ್ತ_exec_runtime;

	ಆದರೆ (p->ಕೊನೆಯ_task_numa_placement) {
		delta = runtime - p->ಕೊನೆಯ_ಮೊತ್ತ_exec_runtime;
		*period = now - p->ಕೊನೆಯ_task_numa_placement;

		/* Avoid time going backwards, prevent potential divide error: */
		ಆದರೆ (unlikely((s64)*period < 0))
			*period = 0;
	} ಇಲ್ಲದಿದ್ದರೆ {
		delta = p->se.avg.ತುಂಬು_ಮೊತ್ತ;
		*period = ತುಂಬು_AVG_MAX;
	}

	p->ಕೊನೆಯ_ಮೊತ್ತ_exec_runtime = runtime;
	p->ಕೊನೆಯ_task_numa_placement = now;

	ಹಿಂತಿರುಗಿ delta;
}

/*
 * Determine the preferred nid for a task in a numa_group. This needs to
 * be done in a way that produces consistent results with group_weight,
 * otherwise workloads might not converge.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ preferred_ಗುಂಪು_nid(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ nid)
{
	nodemask_t nodes;
	ಪೂರ್ಣಾಂಕ dist;

	/* Direct connections between all NUMA nodes. */
	ಆದರೆ (sched_numa_topology_type == NUMA_ನೇರ)
		ಹಿಂತಿರುಗಿ nid;

	/*
	 * On a system with glueless mesh NUMA topology, group_weight
	 * scores nodes according to the number of NUMA hinting faults on
	 * both the node itself, and on nearby nodes.
	 */
	ಆದರೆ (sched_numa_topology_type == NUMA_GLUELESS_MESH) {
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ score, max_score = 0;
		ಪೂರ್ಣಾಂಕ ಗಂಟು, max_ಗಂಟು = nid;

		dist = sched_max_numa_ದೂರ;

		for_each_ಗಂಟು_state(ಗಂಟು, N_CPU) {
			score = ಗುಂಪು_ತೂಕ(p, ಗಂಟು, dist);
			ಆದರೆ (score > max_score) {
				max_score = score;
				max_ಗಂಟು = ಗಂಟು;
			}
		}
		ಹಿಂತಿರುಗಿ max_ಗಂಟು;
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
	nodes = ಗಂಟು_ಸ್ಥಿತಿಗಳು[N_CPU];
	ಗಾಗಿ (dist = sched_max_numa_ದೂರ; dist > LOCAL_ದೂರ; dist--) {
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ max_ದೋಷಗಳು = 0;
		nodemask_t max_ಗುಂಪು = ಗಂಟು_ಮುಖವಾಡ_NONE;
		ಪೂರ್ಣಾಂಕ a, b;

		/* Are there nodes at this distance from each other? */
		ಆದರೆ (!ಕಂಡುಹಿಡಿ_numa_ದೂರ(dist))
			ಮುಂದುವರಿ;

		for_each_ಗಂಟು_ಮುಖವಾಡ(a, nodes) {
			ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೋಷಗಳು = 0;
			nodemask_t this_ಗುಂಪು;
			nodes_clear(this_ಗುಂಪು);

			/* Sum group's NUMA faults; includes a==b case. */
			for_each_ಗಂಟು_ಮುಖವಾಡ(b, nodes) {
				ಆದರೆ (ಗಂಟು_ದೂರ(a, b) < dist) {
					ದೋಷಗಳು += ಗುಂಪು_ದೋಷಗಳು(p, b);
					ಗಂಟು_ಗಣ(b, this_ಗುಂಪು);
					ಗಂಟು_clear(b, nodes);
				}
			}

			/* Remember the top group. */
			ಆದರೆ (ದೋಷಗಳು > max_ದೋಷಗಳು) {
				max_ದೋಷಗಳು = ದೋಷಗಳು;
				max_ಗುಂಪು = this_ಗುಂಪು;
				/*
				 * subtle: at the smallest distance there is
				 * just one node left in each "group", the
				 * winner is the preferred nid.
				 */
				nid = a;
			}
		}
		/* Next round, evaluate the nodes within max_group. */
		ಆದರೆ (!max_ದೋಷಗಳು)
			ನಿಲ್ಲಿಸು;
		nodes = max_ಗುಂಪು;
	}
	ಹಿಂತಿರುಗಿ nid;
}

ಸ್ಥಿರ ಶೂನ್ಯ task_numa_placement(ರಚನೆ task_struct *p)
	__context_unsafe(/* conditional locking */)
{
	ರಚನೆ sched_cache_ಗುಂಪು __maybe_unused *grp;
	ಪೂರ್ಣಾಂಕ seq, nid, max_nid = NUMA_NO_ಗಂಟು;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ max_ದೋಷಗಳು = 0;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ fault_types[2] = { 0, 0 };
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಒಟ್ಟು_ದೋಷಗಳು;
	u64 runtime, period;
	spinlock_t *ಗುಂಪು_ಬೀಗ = ಖಾಲಿ;
	ದೀರ್ಘ __maybe_unused new_fp;
	ರಚನೆ numa_ಗುಂಪು *ng;

	/*
	 * The p->mm->numa_scan_seq field gets updated without
	 * exclusive access. Use READ_ONCE() here to ensure
	 * that the field is read in a single access:
	 */
	seq = READ_ONCE(p->mm->numa_ಪರಿಶೀಲನೆ_seq);
	ಆದರೆ (p->numa_ಪರಿಶೀಲನೆ_seq == seq)
		ಹಿಂತಿರುಗಿ;
	p->numa_ಪರಿಶೀಲನೆ_seq = seq;
	p->numa_ಪರಿಶೀಲನೆ_period_max = task_ಪರಿಶೀಲನೆ_max(p);

	ಒಟ್ಟು_ದೋಷಗಳು = p->numa_ದೋಷಗಳು_locality[0] +
		       p->numa_ದೋಷಗಳು_locality[1];
	runtime = numa_get_avg_runtime(p, &period);

	/* If the task is part of a group prevent parallel updates to group stats */
	ng = deref_curr_numa_ಗುಂಪು(p);
	ಆದರೆ (ng) {
		ಗುಂಪು_ಬೀಗ = &ng->ಬೀಗ;
		spin_ಬೀಗ_irq(ಗುಂಪು_ಬೀಗ);
	}

	/* Find the node with the highest number of faults */
	for_each_online_ಗಂಟು(nid) {
		/* Keep track of the offsets in numa_faults array */
		ಪೂರ್ಣಾಂಕ mem_idx, membuf_idx, cpu_idx, cpubuf_idx;
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೋಷಗಳು = 0, ಗುಂಪು_ದೋಷಗಳು = 0;
		ಪೂರ್ಣಾಂಕ priv;

		ಗಾಗಿ (priv = 0; priv < NR_NUMA_HINT_FAULT_TYPES; priv++) {
			ದೀರ್ಘ diff, f_diff, f_ತೂಕ;

			mem_idx = task_ದೋಷಗಳು_idx(NUMA_MEM, nid, priv);
			membuf_idx = task_ದೋಷಗಳು_idx(NUMA_MEMBUF, nid, priv);
			cpu_idx = task_ದೋಷಗಳು_idx(NUMA_CPU, nid, priv);
			cpubuf_idx = task_ದೋಷಗಳು_idx(NUMA_CPUBUF, nid, priv);

			/* Decay existing window, copy faults since last scan */
			diff = p->numa_ದೋಷಗಳು[membuf_idx] - p->numa_ದೋಷಗಳು[mem_idx] / 2;
			fault_types[priv] += p->numa_ದೋಷಗಳು[membuf_idx];
			p->numa_ದೋಷಗಳು[membuf_idx] = 0;

			/*
			 * Normalize the faults_from, so all tasks in a group
			 * count according to CPU use, instead of by the raw
			 * number of faults. Tasks with little runtime have
			 * little over-all impact on throughput, and thus their
			 * faults are less important.
			 */
			f_ತೂಕ = div64_u64(runtime << 16, period + 1);
			f_ತೂಕ = (f_ತೂಕ * p->numa_ದೋಷಗಳು[cpubuf_idx]) /
				   (ಒಟ್ಟು_ದೋಷಗಳು + 1);
			f_diff = f_ತೂಕ - p->numa_ದೋಷಗಳು[cpu_idx] / 2;
			p->numa_ದೋಷಗಳು[cpubuf_idx] = 0;

			p->numa_ದೋಷಗಳು[mem_idx] += diff;
			p->numa_ದೋಷಗಳು[cpu_idx] += f_diff;
			ದೋಷಗಳು += p->numa_ದೋಷಗಳು[mem_idx];
			p->ಒಟ್ಟು_numa_ದೋಷಗಳು += diff;
			ಆದರೆ (ng) {
				/*
				 * safe because we can only change our own group
				 *
				 * mem_idx represents the offset for a given
				 * nid and priv in a specific region because it
				 * is at the beginning of the numa_faults array.
				 */
				ng->ದೋಷಗಳು[mem_idx] += diff;
				ng->ದೋಷಗಳು[cpu_idx] += f_diff;
				ng->ಒಟ್ಟು_ದೋಷಗಳು += diff;
				ಗುಂಪು_ದೋಷಗಳು += ng->ದೋಷಗಳು[mem_idx];
			}
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CACHE
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

				ಆದರೆ (grp) {
					new_fp = (ದೀರ್ಘ)READ_ONCE(grp->footprint) + diff;
					WRITE_ONCE(grp->footprint, max(new_fp, 0L));
				}
			}
#ಆದರೆ_ಅಂತ್ಯ
		}

		ಆದರೆ (!ng) {
			ಆದರೆ (ದೋಷಗಳು > max_ದೋಷಗಳು) {
				max_ದೋಷಗಳು = ದೋಷಗಳು;
				max_nid = nid;
			}
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಗುಂಪು_ದೋಷಗಳು > max_ದೋಷಗಳು) {
			max_ದೋಷಗಳು = ಗುಂಪು_ದೋಷಗಳು;
			max_nid = nid;
		}
	}

	/* Cannot migrate task to CPU-less node */
	max_nid = numa_ಹತ್ತಿರದ_ಗಂಟು(max_nid, N_CPU);

	ಆದರೆ (ng) {
		numa_ಗುಂಪು_ಎಣಿಕೆ_ಸಕ್ರಿಯ_nodes(ng);
		spin_unlock_irq(ಗುಂಪು_ಬೀಗ);
		max_nid = preferred_ಗುಂಪು_nid(p, max_nid);
	}

	ಆದರೆ (max_ದೋಷಗಳು) {
		/* Set the new preferred node */
		ಆದರೆ (max_nid != p->numa_preferred_nid)
			sched_setnuma(p, max_nid);
	}

	update_task_ಪರಿಶೀಲನೆ_period(p, fault_types[0], fault_types[1]);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ get_numa_ಗುಂಪು(ರಚನೆ numa_ಗುಂಪು *grp)
{
	ಹಿಂತಿರುಗಿ refcount_inc_not_zero(&grp->refcount);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ put_numa_ಗುಂಪು(ರಚನೆ numa_ಗುಂಪು *grp)
{
	ಆದರೆ (refcount_dec_and_test(&grp->refcount))
		kfree_rcu(grp, rcu);
}

ಸ್ಥಿರ ಶೂನ್ಯ task_numa_ಗುಂಪು(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ cpupid, ಪೂರ್ಣಾಂಕ flags,
			ಪೂರ್ಣಾಂಕ *priv)
{
	ರಚನೆ numa_ಗುಂಪು *grp, *my_grp;
	ರಚನೆ task_struct *tsk;
	bool join = false;
	ಪೂರ್ಣಾಂಕ cpu = cpupid_ಗಮ್ಯ_cpu(cpupid);
	ಪೂರ್ಣಾಂಕ i;

	ಆದರೆ (unlikely(!deref_curr_numa_ಗುಂಪು(p))) {
		ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2 = ಗಾತ್ರ(ರಚನೆ numa_ಗುಂಪು) +
				    NR_NUMA_HINT_FAULT_STATS *
				    nr_ಗಂಟು_ids * ಗಾತ್ರ(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ);

		grp = kzalloc(ಗಾತ್ರ_2, GFP_KERNEL | __GFP_NOWARN);
		ಆದರೆ (!grp)
			ಹಿಂತಿರುಗಿ;

		refcount_ಗಣ(&grp->refcount, 1);
		grp->ಸಕ್ರಿಯ_nodes = 1;
		grp->max_ದೋಷಗಳು_cpu = 0;
		spin_ಬೀಗ_init(&grp->ಬೀಗ);
		grp->gid = p->pid;

		ಗಾಗಿ (i = 0; i < NR_NUMA_HINT_FAULT_STATS * nr_ಗಂಟು_ids; i++)
			grp->ದೋಷಗಳು[i] = p->numa_ದೋಷಗಳು[i];

		grp->ಒಟ್ಟು_ದೋಷಗಳು = p->ಒಟ್ಟು_numa_ದೋಷಗಳು;

		grp->nr_tasks++;
		rcu_ನಿಯೋಜಿಸು_ಸೂಚಕ(p->numa_ಗುಂಪು, grp);
	}

	rcu_read_ಬೀಗ();
	tsk = READ_ONCE(cpu_rq(cpu)->curr);

	ಆದರೆ (!cpupid_ಹೊಂದಿಕೆ_pid(tsk, cpupid))
		ಹೋಗು no_join;

	grp = rcu_dereference_all(tsk->numa_ಗುಂಪು);
	ಆದರೆ (!grp)
		ಹೋಗು no_join;

	my_grp = deref_curr_numa_ಗುಂಪು(p);
	ಆದರೆ (grp == my_grp)
		ಹೋಗು no_join;

	/*
	 * Only join the other group if its bigger; if we're the bigger group,
	 * the other task will join us.
	 */
	ಆದರೆ (my_grp->nr_tasks > grp->nr_tasks)
		ಹೋಗು no_join;

	/*
	 * Tie-break on the grp address.
	 */
	ಆದರೆ (my_grp->nr_tasks == grp->nr_tasks && my_grp > grp)
		ಹೋಗು no_join;

	/* Always join threads in the same process. */
	ಆದರೆ (tsk->mm == current->mm)
		join = true;

	/* Simple filter to avoid false positives due to PID collisions */
	ಆದರೆ (flags & TNF_SHARED)
		join = true;

	/* Update priv based on whether false sharing was detected */
	*priv = !join;

	ಆದರೆ (join && !get_numa_ಗುಂಪು(grp))
		ಹೋಗು no_join;

	rcu_read_unlock();

	ಆದರೆ (!join)
		ಹಿಂತಿರುಗಿ;

	WARN_ON_ONCE(irqs_disabled());
	double_ಬೀಗ_irq(&my_grp->ಬೀಗ, &grp->ಬೀಗ);

	ಗಾಗಿ (i = 0; i < NR_NUMA_HINT_FAULT_STATS * nr_ಗಂಟು_ids; i++) {
		my_grp->ದೋಷಗಳು[i] -= p->numa_ದೋಷಗಳು[i];
		grp->ದೋಷಗಳು[i] += p->numa_ದೋಷಗಳು[i];
	}
	my_grp->ಒಟ್ಟು_ದೋಷಗಳು -= p->ಒಟ್ಟು_numa_ದೋಷಗಳು;
	grp->ಒಟ್ಟು_ದೋಷಗಳು += p->ಒಟ್ಟು_numa_ದೋಷಗಳು;

	my_grp->nr_tasks--;
	grp->nr_tasks++;

	spin_unlock(&my_grp->ಬೀಗ);
	spin_unlock_irq(&grp->ಬೀಗ);

	rcu_ನಿಯೋಜಿಸು_ಸೂಚಕ(p->numa_ಗುಂಪು, grp);

	put_numa_ಗುಂಪು(my_grp);
	ಹಿಂತಿರುಗಿ;

no_join:
	rcu_read_unlock();
	ಹಿಂತಿರುಗಿ;
}

/*
 * Get rid of NUMA statistics associated with a task (either current or dead).
 * If @final is set, the task is dead and has reached refcount zero, so we can
 * safely free all relevant data structures. Otherwise, there might be
 * concurrent reads from places like load balancing and procfs, and we should
 * reset the data back to default state without freeing ->numa_faults.
 */
ಶೂನ್ಯ task_numa_free(ರಚನೆ task_struct *p, bool final)
{
	/* safe: p either is current or is being freed by current */
	ರಚನೆ numa_ಗುಂಪು *grp = rcu_dereference_raw(p->numa_ಗುಂಪು);
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ *numa_ದೋಷಗಳು = p->numa_ದೋಷಗಳು;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags;
	ಪೂರ್ಣಾಂಕ i;

	ಆದರೆ (!numa_ದೋಷಗಳು)
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (grp) {
		spin_ಬೀಗ_irqsave(&grp->ಬೀಗ, flags);
		ಗಾಗಿ (i = 0; i < NR_NUMA_HINT_FAULT_STATS * nr_ಗಂಟು_ids; i++)
			grp->ದೋಷಗಳು[i] -= p->numa_ದೋಷಗಳು[i];
		grp->ಒಟ್ಟು_ದೋಷಗಳು -= p->ಒಟ್ಟು_numa_ದೋಷಗಳು;

		grp->nr_tasks--;
		spin_unlock_irqrestore(&grp->ಬೀಗ, flags);
		RCU_INIT_ಸೂಚಕ(p->numa_ಗುಂಪು, ಖಾಲಿ);
		put_numa_ಗುಂಪು(grp);
	}

	ಆದರೆ (final) {
		p->numa_ದೋಷಗಳು = ಖಾಲಿ;
		kfree(numa_ದೋಷಗಳು);
	} ಇಲ್ಲದಿದ್ದರೆ {
		p->ಒಟ್ಟು_numa_ದೋಷಗಳು = 0;
		ಗಾಗಿ (i = 0; i < NR_NUMA_HINT_FAULT_STATS * nr_ಗಂಟು_ids; i++)
			numa_ದೋಷಗಳು[i] = 0;
	}
}

/*
 * Got a PROT_NONE fault for a page on @node.
 */
ಶೂನ್ಯ task_numa_fault(ಪೂರ್ಣಾಂಕ ಕೊನೆಯ_cpupid, ಪೂರ್ಣಾಂಕ mem_ಗಂಟು, ಪೂರ್ಣಾಂಕ ಪುಟಗಳು, ಪೂರ್ಣಾಂಕ flags)
{
	ರಚನೆ task_struct *p = current;
	bool migrated = flags & TNF_MIGRATED;
	ಪೂರ್ಣಾಂಕ cpu_ಗಂಟು = task_ಗಂಟು(current);
	ಪೂರ್ಣಾಂಕ local = !!(flags & TNF_FAULT_LOCAL);
	ರಚನೆ numa_ಗುಂಪು *ng;
	ಪೂರ್ಣಾಂಕ priv;

	ಆದರೆ (!static_branch_likely(&sched_numa_balancing))
		ಹಿಂತಿರುಗಿ;

	/* for example, ksmd faulting in a user's mm */
	ಆದರೆ (!p->mm)
		ಹಿಂತಿರುಗಿ;

	/*
	 * NUMA faults statistics are unnecessary for the slow memory
	 * node for memory tiering mode.
	 */
	ಆದರೆ (!ಗಂಟು_is_toptier(mem_ಗಂಟು) &&
	    (sysctl_numa_balancing_ಬಹುಲಕ & NUMA_BALANCING_ಸ್ಮರಣೆ_TIERING ||
	     !cpupid_valid(ಕೊನೆಯ_cpupid)))
		ಹಿಂತಿರುಗಿ;

	/* Allocate buffer to track faults on a per-node basis */
	ಆದರೆ (unlikely(!p->numa_ದೋಷಗಳು)) {
		ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2 = ಗಾತ್ರ(*p->numa_ದೋಷಗಳು) *
			   NR_NUMA_HINT_FAULT_BUCKETS * nr_ಗಂಟು_ids;

		p->numa_ದೋಷಗಳು = kzalloc(ಗಾತ್ರ_2, GFP_KERNEL|__GFP_NOWARN);
		ಆದರೆ (!p->numa_ದೋಷಗಳು)
			ಹಿಂತಿರುಗಿ;

		p->ಒಟ್ಟು_numa_ದೋಷಗಳು = 0;
		memset(p->numa_ದೋಷಗಳು_locality, 0, ಗಾತ್ರ(p->numa_ದೋಷಗಳು_locality));
	}

	/*
	 * First accesses are treated as private, otherwise consider accesses
	 * to be private if the accessing pid has not changed
	 */
	ಆದರೆ (unlikely(ಕೊನೆಯ_cpupid == (-1 & ಕೊನೆಯ_CPUPID_ಮುಖವಾಡ))) {
		priv = 1;
	} ಇಲ್ಲದಿದ್ದರೆ {
		priv = cpupid_ಹೊಂದಿಕೆ_pid(p, ಕೊನೆಯ_cpupid);
		ಆದರೆ (!priv && !(flags & TNF_NO_ಗುಂಪು))
			task_numa_ಗುಂಪು(p, ಕೊನೆಯ_cpupid, flags, &priv);
	}

	/*
	 * If a workload spans multiple NUMA nodes, a shared fault that
	 * occurs wholly within the set of nodes that the workload is
	 * actively using should be counted as local. This allows the
	 * scan rate to slow down when a workload has settled down.
	 */
	ng = deref_curr_numa_ಗುಂಪು(p);
	ಆದರೆ (!priv && !local && ng && ng->ಸಕ್ರಿಯ_nodes > 1 &&
				numa_is_ಸಕ್ರಿಯ_ಗಂಟು(cpu_ಗಂಟು, ng) &&
				numa_is_ಸಕ್ರಿಯ_ಗಂಟು(mem_ಗಂಟು, ng))
		local = 1;

	/*
	 * Retry to migrate task to preferred node periodically, in case it
	 * previously failed, or the scheduler moved us.
	 */
	ಆದರೆ (ಸಮಯ_after(jiffies, p->numa_migrate_retry)) {
		task_numa_placement(p);
		numa_migrate_preferred(p);
	}

	ಆದರೆ (migrated)
		p->numa_ಪುಟಗಳು_migrated += ಪುಟಗಳು;
	ಆದರೆ (flags & TNF_MIGRATE_FAIL)
		p->numa_ದೋಷಗಳು_locality[2] += ಪುಟಗಳು;

	p->numa_ದೋಷಗಳು[task_ದೋಷಗಳು_idx(NUMA_MEMBUF, mem_ಗಂಟು, priv)] += ಪುಟಗಳು;
	p->numa_ದೋಷಗಳು[task_ದೋಷಗಳು_idx(NUMA_CPUBUF, cpu_ಗಂಟು, priv)] += ಪುಟಗಳು;
	p->numa_ದೋಷಗಳು_locality[local] += ಪುಟಗಳು;
}

ಸ್ಥಿರ ಶೂನ್ಯ reset_ptenuma_ಪರಿಶೀಲನೆ(ರಚನೆ task_struct *p)
{
	/*
	 * We only did a read acquisition of the mmap sem, so
	 * p->mm->numa_scan_seq is written to without exclusive access
	 * and the update is not guaranteed to be atomic. That's not
	 * much of an issue though, since this is just used for
	 * statistical sampling. Use READ_ONCE/WRITE_ONCE, which are not
	 * expensive, to avoid any form of compiler optimizations:
	 */
	WRITE_ONCE(p->mm->numa_ಪರಿಶೀಲನೆ_seq, READ_ONCE(p->mm->numa_ಪರಿಶೀಲನೆ_seq) + 1);
	p->mm->numa_ಪರಿಶೀಲನೆ_ಸ್ಥಾನಾಂತರ = 0;
}

ಸ್ಥಿರ bool vma_is_accessed(ರಚನೆ mm_struct *mm, ರಚನೆ vm_area_struct *vma)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ pids;
	/*
	 * Allow unconditional access first two times, so that all the (pages)
	 * of VMAs get prot_none fault introduced irrespective of accesses.
	 * This is also done to avoid any side effect of task scanning
	 * amplifying the unfairness of disjoint set of VMAs' access.
	 */
	ಆದರೆ ((READ_ONCE(current->mm->numa_ಪರಿಶೀಲನೆ_seq) - vma->numab_state->ಆರಂಭ_ಪರಿಶೀಲನೆ_seq) < 2)
		ಹಿಂತಿರುಗಿ true;

	pids = vma->numab_state->pids_ಸಕ್ರಿಯ[0] | vma->numab_state->pids_ಸಕ್ರಿಯ[1];
	ಆದರೆ (test_ಬಿಟ್(hash_32(current->pid, ilog2(ಬಿಟ್ಟುಗಳು_ಪ್ರತಿ_LONG)), &pids))
		ಹಿಂತಿರುಗಿ true;

	/*
	 * Complete a scan that has already started regardless of PID access, or
	 * some VMAs may never be scanned in multi-threaded applications:
	 */
	ಆದರೆ (mm->numa_ಪರಿಶೀಲನೆ_ಸ್ಥಾನಾಂತರ > vma->vm_ಆರಂಭ) {
		trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_IGNORE_PID);
		ಹಿಂತಿರುಗಿ true;
	}

	/*
	 * This vma has not been accessed for a while, and if the number
	 * the threads in the same process is low, which means no other
	 * threads can help scan this vma, force a vma scan.
	 */
	ಆದರೆ (READ_ONCE(mm->numa_ಪರಿಶೀಲನೆ_seq) >
	   (vma->numab_state->prev_ಪರಿಶೀಲನೆ_seq + get_nr_ಎಳೆಗಳು(current)))
		ಹಿಂತಿರುಗಿ true;

	ಹಿಂತಿರುಗಿ false;
}

#ವ್ಯಾಖ್ಯಾನಿಸು VMA_PID_RESET_PERIOD (4 * sysctl_numa_balancing_ಪರಿಶೀಲನೆ_delay)

/*
 * The expensive part of numa migration is done from task_work context.
 * Triggered from task_tick_numa().
 */
ಸ್ಥಿರ ಶೂನ್ಯ task_numa_work(ರಚನೆ callback_ತಲೆ *work)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ migrate, ಮುಂದಿನ_ಪರಿಶೀಲನೆ, now = jiffies;
	ರಚನೆ task_struct *p = current;
	ರಚನೆ mm_struct *mm = p->mm;
	u64 runtime = p->se.ಮೊತ್ತ_exec_runtime;
	ರಚನೆ vm_area_struct *vma;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಆರಂಭ, ಕೊನೆ;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ nr_pte_updates = 0;
	ದೀರ್ಘ ಪುಟಗಳು, virtpages;
	ರಚನೆ vma_iterator vmi;
	bool vma_pids_skipped;
	bool vma_pids_forced = false;

	WARN_ON_ONCE(p != container_ದ(work, ರಚನೆ task_struct, numa_work));

	work->ಮುಂದಿನ = work;
	/*
	 * Who cares about NUMA placement when they're dying.
	 *
	 * NOTE: make sure not to dereference p->mm before this check,
	 * exit_task_work() happens _after_ exit_mm() so we could be called
	 * without p->mm even though we still had it when we enqueued this
	 * work.
	 */
	ಆದರೆ (p->flags & PF_EXITING)
		ಹಿಂತಿರುಗಿ;

	/*
	 * Memory is pinned to only one NUMA node via cpuset.mems, naturally
	 * no page can be migrated.
	 */
	ಆದರೆ (cpusets_enabled() && nodes_ತೂಕ(cpuset_current_mems_allowed) == 1) {
		trace_sched_skip_cpuset_numa(current, &cpuset_current_mems_allowed);
		ಹಿಂತಿರುಗಿ;
	}

	ಆದರೆ (!mm->numa_ಮುಂದಿನ_ಪರಿಶೀಲನೆ) {
		mm->numa_ಮುಂದಿನ_ಪರಿಶೀಲನೆ = now +
			msecs_ಗಮ್ಯ_jiffies(sysctl_numa_balancing_ಪರಿಶೀಲನೆ_delay);
	}

	/*
	 * Enforce maximal scan/migration frequency..
	 */
	migrate = mm->numa_ಮುಂದಿನ_ಪರಿಶೀಲನೆ;
	ಆದರೆ (ಸಮಯ_before(now, migrate))
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (p->numa_ಪರಿಶೀಲನೆ_period == 0) {
		p->numa_ಪರಿಶೀಲನೆ_period_max = task_ಪರಿಶೀಲನೆ_max(p);
		p->numa_ಪರಿಶೀಲನೆ_period = task_ಪರಿಶೀಲನೆ_ಆರಂಭ(p);
	}

	ಮುಂದಿನ_ಪರಿಶೀಲನೆ = now + msecs_ಗಮ್ಯ_jiffies(p->numa_ಪರಿಶೀಲನೆ_period);
	ಆದರೆ (!try_cmpxchg(&mm->numa_ಮುಂದಿನ_ಪರಿಶೀಲನೆ, &migrate, ಮುಂದಿನ_ಪರಿಶೀಲನೆ))
		ಹಿಂತಿರುಗಿ;

	/*
	 * Delay this task enough that another task of this mm will likely win
	 * the next time around.
	 */
	p->ಗಂಟು_stamp += 2 * TICK_NSEC;

	ಪುಟಗಳು = sysctl_numa_balancing_ಪರಿಶೀಲನೆ_ಗಾತ್ರ;
	ಪುಟಗಳು <<= 20 - ಪುಟ_ಸರಿಸು; /* MB in pages */
	virtpages = ಪುಟಗಳು * 8;	   /* Scan up to this much virtual space */
	ಆದರೆ (!ಪುಟಗಳು)
		ಹಿಂತಿರುಗಿ;


	ಆದರೆ (!mmap_read_trylock(mm))
		ಹಿಂತಿರುಗಿ;

	/*
	 * VMAs are skipped if the current PID has not trapped a fault within
	 * the VMA recently. Allow scanning to be forced if there is no
	 * suitable VMA remaining.
	 */
	vma_pids_skipped = false;

retry_pids:
	ಆರಂಭ = mm->numa_ಪರಿಶೀಲನೆ_ಸ್ಥಾನಾಂತರ;
	vma_iter_init(&vmi, mm, ಆರಂಭ);
	vma = vma_ಮುಂದಿನ(&vmi);
	ಆದರೆ (!vma) {
		reset_ptenuma_ಪರಿಶೀಲನೆ(p);
		ಆರಂಭ = 0;
		vma_iter_ಗಣ(&vmi, ಆರಂಭ);
		vma = vma_ಮುಂದಿನ(&vmi);
	}

	ಗಾಗಿ (; vma; vma = vma_ಮುಂದಿನ(&vmi)) {
		ಆದರೆ (!vma_migratable(vma) || !vma_policy_mof(vma) ||
			is_vm_hugetlb_ಪುಟ(vma) || (vma->vm_flags & VM_MIXEDMAP)) {
			trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_UNSUITABLE);
			ಮುಂದುವರಿ;
		}

		/*
		 * Shared library pages mapped by multiple processes are not
		 * migrated as it is expected they are cache replicated. Avoid
		 * hinting faults in read-only file-backed mappings or the vDSO
		 * as migrating the pages will be of marginal benefit.
		 */
		ಆದರೆ (!vma->vm_mm ||
		    (vma->vm_file && (vma->vm_flags & (VM_READ|VM_WRITE)) == (VM_READ))) {
			trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_SHARED_RO);
			ಮುಂದುವರಿ;
		}

		/*
		 * Skip inaccessible VMAs to avoid any confusion between
		 * PROT_NONE and NUMA hinting PTEs
		 */
		ಆದರೆ (!vma_is_accessible(vma)) {
			trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_INACCESSIBLE);
			ಮುಂದುವರಿ;
		}

		/* Initialise new per-VMA NUMAB state. */
		ಆದರೆ (!vma->numab_state) {
			ರಚನೆ vma_numab_state *ptr;

			ptr = kzalloc_obj(*ptr);
			ಆದರೆ (!ptr)
				ಮುಂದುವರಿ;

			ಆದರೆ (cmpxchg(&vma->numab_state, ಖಾಲಿ, ptr)) {
				kfree(ptr);
				ಮುಂದುವರಿ;
			}

			vma->numab_state->ಆರಂಭ_ಪರಿಶೀಲನೆ_seq = mm->numa_ಪರಿಶೀಲನೆ_seq;

			vma->numab_state->ಮುಂದಿನ_ಪರಿಶೀಲನೆ = now +
				msecs_ಗಮ್ಯ_jiffies(sysctl_numa_balancing_ಪರಿಶೀಲನೆ_delay);

			/* Reset happens after 4 times scan delay of scan start */
			vma->numab_state->pids_ಸಕ್ರಿಯ_reset =  vma->numab_state->ಮುಂದಿನ_ಪರಿಶೀಲನೆ +
				msecs_ಗಮ್ಯ_jiffies(VMA_PID_RESET_PERIOD);

			/*
			 * Ensure prev_scan_seq does not match numa_scan_seq,
			 * to prevent VMAs being skipped prematurely on the
			 * first scan:
			 */
			 vma->numab_state->prev_ಪರಿಶೀಲನೆ_seq = mm->numa_ಪರಿಶೀಲನೆ_seq - 1;
		}

		/*
		 * Scanning the VMAs of short lived tasks add more overhead. So
		 * delay the scan for new VMAs.
		 */
		ಆದರೆ (mm->numa_ಪರಿಶೀಲನೆ_seq && ಸಮಯ_before(jiffies,
						vma->numab_state->ಮುಂದಿನ_ಪರಿಶೀಲನೆ)) {
			trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_ಪರಿಶೀಲನೆ_DELAY);
			ಮುಂದುವರಿ;
		}

		/* RESET access PIDs regularly for old VMAs. */
		ಆದರೆ (mm->numa_ಪರಿಶೀಲನೆ_seq &&
				ಸಮಯ_after(jiffies, vma->numab_state->pids_ಸಕ್ರಿಯ_reset)) {
			vma->numab_state->pids_ಸಕ್ರಿಯ_reset = vma->numab_state->pids_ಸಕ್ರಿಯ_reset +
				msecs_ಗಮ್ಯ_jiffies(VMA_PID_RESET_PERIOD);
			vma->numab_state->pids_ಸಕ್ರಿಯ[0] = READ_ONCE(vma->numab_state->pids_ಸಕ್ರಿಯ[1]);
			vma->numab_state->pids_ಸಕ್ರಿಯ[1] = 0;
		}

		/* Do not rescan VMAs twice within the same sequence. */
		ಆದರೆ (vma->numab_state->prev_ಪರಿಶೀಲನೆ_seq == mm->numa_ಪರಿಶೀಲನೆ_seq) {
			mm->numa_ಪರಿಶೀಲನೆ_ಸ್ಥಾನಾಂತರ = vma->vm_ಕೊನೆ;
			trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_SEQ_COMPLETED);
			ಮುಂದುವರಿ;
		}

		/*
		 * Do not scan the VMA if task has not accessed it, unless no other
		 * VMA candidate exists.
		 */
		ಆದರೆ (!vma_pids_forced && !vma_is_accessed(mm, vma)) {
			vma_pids_skipped = true;
			trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_PID_INACTIVE);
			ಮುಂದುವರಿ;
		}

		ಮಾಡು {
			ಆರಂಭ = max(ಆರಂಭ, vma->vm_ಆರಂಭ);
			ಕೊನೆ = ALIGN(ಆರಂಭ + (ಪುಟಗಳು << ಪುಟ_ಸರಿಸು), HPAGE_ಗಾತ್ರ);
			ಕೊನೆ = min(ಕೊನೆ, vma->vm_ಕೊನೆ);
			nr_pte_updates = change_prot_numa(vma, ಆರಂಭ, ಕೊನೆ);

			/*
			 * Try to scan sysctl_numa_balancing_size worth of
			 * hpages that have at least one present PTE that
			 * is not already PTE-numa. If the VMA contains
			 * areas that are unused or already full of prot_numa
			 * PTEs, scan up to virtpages, to skip through those
			 * areas faster.
			 */
			ಆದರೆ (nr_pte_updates)
				ಪುಟಗಳು -= (ಕೊನೆ - ಆರಂಭ) >> ಪುಟ_ಸರಿಸು;
			virtpages -= (ಕೊನೆ - ಆರಂಭ) >> ಪುಟ_ಸರಿಸು;

			ಆರಂಭ = ಕೊನೆ;
			ಆದರೆ (ಪುಟಗಳು <= 0 || virtpages <= 0)
				ಹೋಗು ಹೊರಹೋಗು;

			cond_resched();
		} ಆಗ (ಕೊನೆ != vma->vm_ಕೊನೆ);

		/* VMA scan is complete, do not scan until next sequence. */
		vma->numab_state->prev_ಪರಿಶೀಲನೆ_seq = mm->numa_ಪರಿಶೀಲನೆ_seq;

		/*
		 * Only force scan within one VMA at a time, to limit the
		 * cost of scanning a potentially uninteresting VMA.
		 */
		ಆದರೆ (vma_pids_forced)
			ನಿಲ್ಲಿಸು;
	}

	/*
	 * If no VMAs are remaining and VMAs were skipped due to the PID
	 * not accessing the VMA previously, then force a scan to ensure
	 * forward progress:
	 */
	ಆದರೆ (!vma && !vma_pids_forced && vma_pids_skipped) {
		vma_pids_forced = true;
		ಹೋಗು retry_pids;
	}

ಹೊರಹೋಗು:
	/*
	 * It is possible to reach the end of the VMA list but the last few
	 * VMAs are not guaranteed to the vma_migratable. If they are not, we
	 * would find the !migratable VMA on the next scan but not reset the
	 * scanner to the start so check it now.
	 */
	ಆದರೆ (vma)
		mm->numa_ಪರಿಶೀಲನೆ_ಸ್ಥಾನಾಂತರ = ಆರಂಭ;
	ಇಲ್ಲದಿದ್ದರೆ
		reset_ptenuma_ಪರಿಶೀಲನೆ(p);
	mmap_read_unlock(mm);

	/*
	 * Make sure tasks use at least 32x as much time to run other code
	 * than they used here, to limit NUMA PTE scanning overhead to 3% max.
	 * Usually update_task_scan_period slows down scanning enough; on an
	 * overloaded system we need to limit overhead on a per task basis.
	 */
	ಆದರೆ (unlikely(p->se.ಮೊತ್ತ_exec_runtime != runtime)) {
		u64 diff = p->se.ಮೊತ್ತ_exec_runtime - runtime;
		p->ಗಂಟು_stamp += 32 * diff;
	}
}

ಶೂನ್ಯ init_numa_balancing(u64 clone_flags, ರಚನೆ task_struct *p)
{
	ಪೂರ್ಣಾಂಕ mm_users = 0;
	ರಚನೆ mm_struct *mm = p->mm;

	ಆದರೆ (mm) {
		mm_users = atomic_read(&mm->mm_users);
		ಆದರೆ (mm_users == 1) {
			mm->numa_ಮುಂದಿನ_ಪರಿಶೀಲನೆ = jiffies + msecs_ಗಮ್ಯ_jiffies(sysctl_numa_balancing_ಪರಿಶೀಲನೆ_delay);
			mm->numa_ಪರಿಶೀಲನೆ_seq = 0;
		}
	}
	p->ಗಂಟು_stamp			= 0;
	p->numa_ಪರಿಶೀಲನೆ_seq		= mm ? mm->numa_ಪರಿಶೀಲನೆ_seq : 0;
	p->numa_ಪರಿಶೀಲನೆ_period		= sysctl_numa_balancing_ಪರಿಶೀಲನೆ_delay;
	p->numa_migrate_retry		= 0;
	/* Protect against double add, see task_tick_numa and task_numa_work */
	p->numa_work.ಮುಂದಿನ		= &p->numa_work;
	p->numa_ದೋಷಗಳು			= ಖಾಲಿ;
	p->numa_ಪುಟಗಳು_migrated		= 0;
	p->ಒಟ್ಟು_numa_ದೋಷಗಳು		= 0;
	RCU_INIT_ಸೂಚಕ(p->numa_ಗುಂಪು, ಖಾಲಿ);
	p->ಕೊನೆಯ_task_numa_placement	= 0;
	p->ಕೊನೆಯ_ಮೊತ್ತ_exec_runtime	= 0;

	init_task_work(&p->numa_work, task_numa_work);

	/* New address space, reset the preferred nid */
	ಆದರೆ (!(clone_flags & CLONE_VM)) {
		p->numa_preferred_nid = NUMA_NO_ಗಂಟು;
		ಹಿಂತಿರುಗಿ;
	}

	/*
	 * New thread, keep existing numa_preferred_nid which should be copied
	 * already by arch_dup_task_struct but stagger when scans start.
	 */
	ಆದರೆ (mm) {
		ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ delay;

		delay = min_t(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ, task_ಪರಿಶೀಲನೆ_max(current),
			current->numa_ಪರಿಶೀಲನೆ_period * mm_users * NSEC_ಪ್ರತಿ_MSEC);
		delay += 2 * TICK_NSEC;
		p->ಗಂಟು_stamp = delay;
	}
}

/*
 * Drive the periodic memory faults..
 */
ಸ್ಥಿರ ಶೂನ್ಯ task_tick_numa(ರಚನೆ rq *rq, ರಚನೆ task_struct *curr)
{
	ರಚನೆ callback_ತಲೆ *work = &curr->numa_work;
	u64 period, now;

	/*
	 * We don't care about NUMA placement if we don't have memory.
	 */
	ಆದರೆ (!curr->mm || (curr->flags & (PF_EXITING | PF_KTHREAD)) || work->ಮುಂದಿನ != work)
		ಹಿಂತಿರುಗಿ;

	/*
	 * Using runtime rather than walltime has the dual advantage that
	 * we (mostly) drive the selection from busy threads and that the
	 * task needs to have done some actual work before we bother with
	 * NUMA placement.
	 */
	now = curr->se.ಮೊತ್ತ_exec_runtime;
	period = (u64)curr->numa_ಪರಿಶೀಲನೆ_period * NSEC_ಪ್ರತಿ_MSEC;

	ಆದರೆ (now > curr->ಗಂಟು_stamp + period) {
		ಆದರೆ (!curr->ಗಂಟು_stamp)
			curr->numa_ಪರಿಶೀಲನೆ_period = task_ಪರಿಶೀಲನೆ_ಆರಂಭ(curr);
		curr->ಗಂಟು_stamp += period;

		ಆದರೆ (!ಸಮಯ_before(jiffies, curr->mm->numa_ಮುಂದಿನ_ಪರಿಶೀಲನೆ))
			task_work_ಕೂಡಿಸು(curr, work, TWA_RESUME);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ update_ಪರಿಶೀಲನೆ_period(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ new_cpu)
{
	ಪೂರ್ಣಾಂಕ src_nid = cpu_ಗಮ್ಯ_ಗಂಟು(task_cpu(p));
	ಪೂರ್ಣಾಂಕ dst_nid = cpu_ಗಮ್ಯ_ಗಂಟು(new_cpu);

	ಆದರೆ (!static_branch_likely(&sched_numa_balancing))
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (!p->mm || !p->numa_ದೋಷಗಳು || (p->flags & PF_EXITING))
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (src_nid == dst_nid)
		ಹಿಂತಿರುಗಿ;

	/*
	 * Allow resets if faults have been trapped before one scan
	 * has completed. This is most likely due to a new task that
	 * is pulled cross-node due to wakeups or load balancing.
	 */
	ಆದರೆ (p->numa_ಪರಿಶೀಲನೆ_seq) {
		/*
		 * Avoid scan adjustments if moving to the preferred
		 * node or if the task was not previously running on
		 * the preferred node.
		 */
		ಆದರೆ (dst_nid == p->numa_preferred_nid ||
		    (p->numa_preferred_nid != NUMA_NO_ಗಂಟು &&
			src_nid != p->numa_preferred_nid))
			ಹಿಂತಿರುಗಿ;
	}

	p->numa_ಪರಿಶೀಲನೆ_period = task_ಪರಿಶೀಲನೆ_ಆರಂಭ(p);
}

#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_NUMA_BALANCING: */

ಸ್ಥಿರ ಶೂನ್ಯ task_tick_numa(ರಚನೆ rq *rq, ರಚನೆ task_struct *curr)
{
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ account_numa_ಸರತಿಗೆ_ಸೇರಿಸು(ರಚನೆ rq *rq, ರಚನೆ task_struct *p)
{
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ account_numa_ಸರತಿಯಿಂದ_ತೆಗೆ(ರಚನೆ rq *rq, ರಚನೆ task_struct *p)
{
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ update_ಪರಿಶೀಲನೆ_period(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ new_cpu)
{
}

#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_NUMA_BALANCING */

ಸ್ಥಿರ ಶೂನ್ಯ
account_entity_ಸರತಿಗೆ_ಸೇರಿಸು(ರಚನೆ cfs_rq *cfs_rq, ರಚನೆ sched_entity *se)
{
	WARN_ON_ONCE(cfs_rq != cfs_rq_ದ(se));
	update_ತುಂಬು_ಕೂಡಿಸು(&cfs_rq->ತುಂಬು, se->ತುಂಬು.ತೂಕ);
	ಆದರೆ (entity_is_task(se)) {
		ರಚನೆ task_struct *p = task_ದ(se);
		ರಚನೆ rq *rq = rq_ದ(cfs_rq);

		account_numa_ಸರತಿಗೆ_ಸೇರಿಸು(rq, p);
		account_llc_ಸರತಿಗೆ_ಸೇರಿಸು(rq, p);
		list_ಕೂಡಿಸು(&se->ಗುಂಪು_ಗಂಟು, &rq->cfs_tasks);
	}
	cfs_rq->nr_queued++;
}

ಸ್ಥಿರ ಶೂನ್ಯ
account_entity_ಸರತಿಯಿಂದ_ತೆಗೆ(ರಚನೆ cfs_rq *cfs_rq, ರಚನೆ sched_entity *se)
{
	WARN_ON_ONCE(cfs_rq != cfs_rq_ದ(se));
	update_ತುಂಬು_sub(&cfs_rq->ತುಂಬು, se->ತುಂಬು.ತೂಕ);
	ಆದರೆ (entity_is_task(se)) {
		ರಚನೆ task_struct *p = task_ದ(se);
		ರಚನೆ rq *rq = rq_ದ(cfs_rq);

		account_numa_ಸರತಿಯಿಂದ_ತೆಗೆ(rq, p);
		account_llc_ಸರತಿಯಿಂದ_ತೆಗೆ(rq, p);
		list_del_init(&se->ಗುಂಪು_ಗಂಟು);
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
#ವ್ಯಾಖ್ಯಾನಿಸು ಕೂಡಿಸು_ಧನಾತ್ಮಕ(_ptr, _val) ಮಾಡು {                           \
	typeof(_ptr) ptr = (_ptr);                              \
	__signed_scalar_typeof(*ptr) val = (_val);              \
	typeof(*ptr) res, var = READ_ONCE(*ptr);                \
								\
	res = var + val;                                        \
								\
	ಆದರೆ (val < 0 && res > var)                               \
		res = 0;                                        \
								\
	WRITE_ONCE(*ptr, res);                                  \
} ಆಗ (0)

/*
 * Remove and clamp on negative, from a local variable.
 *
 * A variant of sub_positive(), which does not use explicit load-store
 * and is thus optimized for local variable updates.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು lsub_ಧನಾತ್ಮಕ(_ptr, _val) ಮಾಡು {				\
	typeof(_ptr) ptr = (_ptr);				\
	*ptr -= min_t(typeof(*ptr), *ptr, _val);		\
} ಆಗ (0)


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
#ವ್ಯಾಖ್ಯಾನಿಸು __update_sa(sa, name, delta_avg, delta_ಮೊತ್ತ) ಮಾಡು {	\
	ಕೂಡಿಸು_ಧನಾತ್ಮಕ(&(sa)->name##_avg, delta_avg);		\
	ಕೂಡಿಸು_ಧನಾತ್ಮಕ(&(sa)->name##_sum, delta_ಮೊತ್ತ);		\
	(sa)->name##_sum = max_t(typeof((sa)->name##_sum),	\
			       (sa)->name##_sum,		\
			       (sa)->name##_avg * PELT_MIN_DIVIDER); \
} ಆಗ (0)

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
ಸರತಿಗೆ_ಸೇರಿಸು_ತುಂಬು_avg(ರಚನೆ cfs_rq *cfs_rq, ರಚನೆ sched_entity *se)
{
	__update_sa(&cfs_rq->avg, ತುಂಬು, se->avg.ತುಂಬು_avg,
		    se_ತೂಕ(se) * se->avg.ತುಂಬು_ಮೊತ್ತ);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
ಸರತಿಯಿಂದ_ತೆಗೆ_ತುಂಬು_avg(ರಚನೆ cfs_rq *cfs_rq, ರಚನೆ sched_entity *se)
{
	__update_sa(&cfs_rq->avg, ತುಂಬು, -se->avg.ತುಂಬು_avg,
		    se_ತೂಕ(se) * -se->avg.ತುಂಬು_ಮೊತ್ತ);
}

ಸ್ಥಿರ ಶೂನ್ಯ
rescale_entity(ರಚನೆ sched_entity *se, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ತೂಕ, bool rel_vprot)
{
	ದೀರ್ಘ old_ತೂಕ = se->h_ತುಂಬು.ತೂಕ;

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
	se->vlag = div64_long(se->vlag * old_ತೂಕ, ತೂಕ);

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
	ಆದರೆ (se->rel_deadline)
		se->deadline = div64_long(se->deadline * old_ತೂಕ, ತೂಕ);

	ಆದರೆ (rel_vprot)
		se->vprot = div64_long(se->vprot * old_ತೂಕ, ತೂಕ);
}

ಸ್ಥಿರ ಶೂನ್ಯ reweight_eevdf(ರಚನೆ cfs_rq *cfs_rq, ರಚನೆ sched_entity *se,
			   ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ತೂಕ, bool on_rq)
{
	bool curr = cfs_rq->curr == se;
	bool rel_vprot = false;
	u64 avruntime = 0;

	ಆದರೆ (se->h_ತುಂಬು.ತೂಕ == ತೂಕ)
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (on_rq) {
		avruntime = avg_vruntime(cfs_rq);
		se->vlag = entity_lag(cfs_rq, se, avruntime);
		se->deadline -= avruntime;
		se->rel_deadline = 1;
		ಆದರೆ (curr && protect_ತುಂಡು(se)) {
			se->vprot -= avruntime;
			rel_vprot = true;
		}

		cfs_rq->h_nr_queued--;
		ಆದರೆ (!curr)
			__dequeue_entity(cfs_rq, se);
	}

	rescale_entity(se, ತೂಕ, rel_vprot);

	update_ತುಂಬು_ಗಣ(&se->h_ತುಂಬು, ತೂಕ);

	ಆದರೆ (on_rq) {
		ಆದರೆ (rel_vprot)
			se->vprot += avruntime;
		se->deadline += avruntime;
		se->rel_deadline = 0;
		se->vruntime = avruntime - se->vlag;

		ಆದರೆ (!curr)
			__enqueue_entity(cfs_rq, se);
		cfs_rq->h_nr_queued++;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ reweight_entity(ರಚನೆ cfs_rq *cfs_rq, ರಚನೆ sched_entity *se,
			    ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ತೂಕ)
{
	ಆದರೆ (se->ತುಂಬು.ತೂಕ == ತೂಕ)
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (se->on_rq) {
		WARN_ON_ONCE(cfs_rq != cfs_rq_ದ(se));
		update_ತುಂಬು_sub(&cfs_rq->ತುಂಬು, se->ತುಂಬು.ತೂಕ);
	}
	ಸರತಿಯಿಂದ_ತೆಗೆ_ತುಂಬು_avg(cfs_rq, se);

	update_ತುಂಬು_ಗಣ(&se->ತುಂಬು, ತೂಕ);

	ಮಾಡು {
		u32 divider = get_pelt_divider(&se->avg);
		se->avg.ತುಂಬು_avg = div_u64(se_ತೂಕ(se) * se->avg.ತುಂಬು_ಮೊತ್ತ, divider);
	} ಆಗ (0);

	ಸರತಿಗೆ_ಸೇರಿಸು_ತುಂಬು_avg(cfs_rq, se);

	ಆದರೆ (se->on_rq)
		update_ತುಂಬು_ಕೂಡಿಸು(&cfs_rq->ತುಂಬು, se->ತುಂಬು.ತೂಕ);
}

/*
 * weight = NICE_0_LOAD;
 * for_each_entity_se(se)
 *   weight = __calc_prop_weight(cfs_rq_of(se), se, weight);
 */
ಸ್ಥಿರ __always_inline
ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ __calc_prop_weight(ರಚನೆ cfs_rq *cfs_rq, ರಚನೆ sched_entity *se,
				 ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ತೂಕ)
{
	ತೂಕ *= se->ತುಂಬು.ತೂಕ;
	ಆದರೆ (ಪೋಷಕ_entity(se))
		ತೂಕ /= cfs_rq->ತುಂಬು.ತೂಕ;
	ಇಲ್ಲದಿದ್ದರೆ
		ತೂಕ /= NICE_0_ತುಂಬು;

	ಹಿಂತಿರುಗಿ max(ತೂಕ, MIN_SHARES);
}

ಸ್ಥಿರ ಶೂನ್ಯ reweight_task_fair(ರಚನೆ rq *rq, ರಚನೆ task_struct *p,
			       ಸ್ಥಿರಾಂಕ ರಚನೆ ತುಂಬು_ತೂಕ *lw)
{
	ರಚನೆ sched_entity *se = &p->se;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ತೂಕ = NICE_0_ತುಂಬು;

	ಆದರೆ (se->on_rq)
		update_curr_fair(rq);

	reweight_entity(cfs_rq_ದ(se), se, lw->ತೂಕ);
	se->ತುಂಬು.inv_ತೂಕ = lw->inv_ತೂಕ;

	ಆದರೆ (!se->on_rq)
		ಹಿಂತಿರುಗಿ;

	for_each_sched_entity(se)
		ತೂಕ = __calc_prop_weight(cfs_rq_ದ(se), se, ತೂಕ);

	reweight_eevdf(&rq->cfs, &p->se, ತೂಕ, p->se.on_rq);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ throttled_hierarchy(ರಚನೆ cfs_rq *cfs_rq);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_FAIR_ಗುಂಪು_SCHED
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
ಸ್ಥಿರ ದೀರ್ಘ __calc_smp_shares(ರಚನೆ cfs_rq *cfs_rq, ದೀರ್ಘ tg_shares, ದೀರ್ಘ shares_max)
{
	ರಚನೆ task_ಗುಂಪು *tg = cfs_rq->tg;
	ದೀರ್ಘ tg_ತೂಕ, ತುಂಬು, shares;

	ತುಂಬು = max(scale_ತುಂಬು_ಕೆಳಕ್ಕೆ(cfs_rq->ತುಂಬು.ತೂಕ), cfs_rq->avg.ತುಂಬು_avg);

	tg_ತೂಕ = atomic_long_read(&tg->ತುಂಬು_avg);

	/* Ensure tg_weight >= load */
	tg_ತೂಕ -= cfs_rq->tg_ತುಂಬು_avg_contrib;
	tg_ತೂಕ += ತುಂಬು;

	shares = (tg_shares * ತುಂಬು);
	ಆದರೆ (tg_ತೂಕ)
		shares /= tg_ತೂಕ;

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
	ಹಿಂತಿರುಗಿ clamp_t(ದೀರ್ಘ, shares, MIN_SHARES, shares_max);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ tg_cpus(ರಚನೆ task_ಗುಂಪು *tg)
{
	ಪೂರ್ಣಾಂಕ nr = ಅಂಕೆ_online_cpus();

	ಆದರೆ (cpusets_enabled()) {
		ರಚನೆ cgroup *cgrp = tg->css.cgroup;
		ಆದರೆ (cgrp)
			nr = cpuset_ಅಂಕೆ_cpus(cgrp);
	}

	/*
	 * An empty cpuset would propagate a 0 shares_max into
	 * __calc_smp_shares(), where clamp() yields hi when hi < lo and so
	 * defeats the MIN_SHARES floor. Match tg_tasks(), which floors at 1.
	 */
	ಹಿಂತಿರುಗಿ max(nr, 1);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ tg_tasks(ರಚನೆ task_ಗುಂಪು *tg)
{
	ಹಿಂತಿರುಗಿ max(1, atomic_long_read(&tg->runnable_avg) >> SCHED_ಸಾಮರ್ಥ್ಯ_ಸರಿಸು);
}

/*
 * Func: fraction(nr_tasks * tg->shares)
 *
 * Scale tg->shares by the number of tasks.
 */
ಸ್ಥಿರ ದೀರ್ಘ calc_tasks_shares(ರಚನೆ cfs_rq *cfs_rq)
{
	ರಚನೆ task_ಗುಂಪು *tg = cfs_rq->tg;
	ಪೂರ್ಣಾಂಕ nr = tg_tasks(tg);
	ದೀರ್ಘ tg_shares = READ_ONCE(tg->shares);
	ಹಿಂತಿರುಗಿ __calc_smp_shares(cfs_rq, nr * tg_shares, nr * tg_shares);
}

/*
 * Func: min(fraction(nr_cpus * tg->shares), nice -20)
 *
 * Scale tg->shares by the maximal number of CPUs; but clip the max shares at
 * nice -20, otherwise a single spinner on a 512 CPU machine would result in
 * 512*NICE_0_LOAD, which is also crazy.
 */
ಸ್ಥಿರ ದೀರ್ಘ calc_max_shares(ರಚನೆ cfs_rq *cfs_rq)
{
	ರಚನೆ task_ಗುಂಪು *tg = cfs_rq->tg;
	ಪೂರ್ಣಾಂಕ nr = tg_cpus(tg);
	ದೀರ್ಘ tg_shares = READ_ONCE(tg->shares);
	ದೀರ್ಘ max_shares = scale_ತುಂಬು(sched_prio_ಗಮ್ಯ_ತೂಕ[0]);
	ಹಿಂತಿರುಗಿ __calc_smp_shares(cfs_rq, tg_shares * nr, max_shares);
}

/*
 * Func: fraction(nr * tg->shares); nr = min(nr_tasks, nr_cpus)
 *
 * Scales between "smp" and "max" in a natural way. No longer needs clipping
 * since there are no unnatural inflations like with "max".
 */
ಸ್ಥಿರ ದೀರ್ಘ calc_concur_shares(ರಚನೆ cfs_rq *cfs_rq)
{
	ರಚನೆ task_ಗುಂಪು *tg = cfs_rq->tg;
	ಪೂರ್ಣಾಂಕ nr = min(tg_tasks(tg), tg_cpus(tg));
	ದೀರ್ಘ tg_shares = READ_ONCE(tg->shares);
	ಹಿಂತಿರುಗಿ __calc_smp_shares(cfs_rq, nr * tg_shares, nr * tg_shares);
}

/*
 * Func: fraction(tg->shares)
 *
 * This infamously results in tiny shares when you have many CPUs.
 */
ಸ್ಥಿರ ದೀರ್ಘ calc_smp_shares(ರಚನೆ cfs_rq *cfs_rq)
{
	ರಚನೆ task_ಗುಂಪು *tg = cfs_rq->tg;
	ದೀರ್ಘ tg_shares = READ_ONCE(tg->shares);
	ಹಿಂತಿರುಗಿ __calc_smp_shares(cfs_rq, tg_shares, tg_shares);
}

/*
 * Ignore this pesky SMP stuff, use (4).
 */
ಸ್ಥಿರ ದೀರ್ಘ calc_ಮೇಲೆ_shares(ರಚನೆ cfs_rq *cfs_rq)
{
	ರಚನೆ task_ಗುಂಪು *tg = cfs_rq->tg;
	ಹಿಂತಿರುಗಿ READ_ONCE(tg->shares);
}

DEFINE_STATIC_CALL(calc_ಗುಂಪು_shares, calc_concur_shares);

ಶೂನ್ಯ __sched_cgroup_mode_update(ಪೂರ್ಣಾಂಕ ಬಹುಲಕ)
{
	ದೀರ್ಘ (*func)(ರಚನೆ cfs_rq *);
	ಆಯ್ಕೆ (ಬಹುಲಕ) {
	ಸ್ಥಿತಿ 0:
		func = &calc_ಮೇಲೆ_shares;
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ 1:
		func = &calc_smp_shares;
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ 2:
	ಪೂರ್ವನಿಯೋಜಿತ:
		func = &calc_concur_shares;
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ 3:
		func = &calc_max_shares;
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ 4:
		func = &calc_tasks_shares;
		ನಿಲ್ಲಿಸು;
	}
	static_call_update(calc_ಗುಂಪು_shares, func);
}

/*
 * Recomputes the group entity based on the current state of its group
 * runqueue.
 */
ಸ್ಥಿರ ಶೂನ್ಯ update_cfs_ಗುಂಪು(ರಚನೆ sched_entity *se)
{
	ರಚನೆ cfs_rq *gcfs_rq = ಗುಂಪು_cfs_rq(se);
	ದೀರ್ಘ shares;

	/*
	 * When a group becomes empty, preserve its weight. This matters for
	 * DELAY_DEQUEUE.
	 */
	ಆದರೆ (!gcfs_rq || !gcfs_rq->ತುಂಬು.ತೂಕ)
		ಹಿಂತಿರುಗಿ;

	shares = static_call(calc_ಗುಂಪು_shares)(gcfs_rq);
	reweight_entity(cfs_rq_ದ(se), se, shares);
}

#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_FAIR_GROUP_SCHED: */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ update_cfs_ಗುಂಪು(ರಚನೆ sched_entity *se)
{
}
#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_FAIR_GROUP_SCHED */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ cfs_rq_util_change(ರಚನೆ cfs_rq *cfs_rq, ಪೂರ್ಣಾಂಕ flags)
{
	ರಚನೆ rq *rq = rq_ದ(cfs_rq);

	ಆದರೆ (&rq->cfs == cfs_rq) {
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

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool ತುಂಬು_avg_is_decayed(ರಚನೆ sched_avg *sa)
{
	ಆದರೆ (sa->ತುಂಬು_ಮೊತ್ತ)
		ಹಿಂತಿರುಗಿ false;

	ಆದರೆ (sa->util_ಮೊತ್ತ)
		ಹಿಂತಿರುಗಿ false;

	ಆದರೆ (sa->runnable_ಮೊತ್ತ)
		ಹಿಂತಿರುಗಿ false;

	/*
	 * _avg must be null when _sum are null because _avg = _sum / divider
	 * Make sure that rounding and/or propagation of PELT values never
	 * break this.
	 */
	WARN_ON_ONCE(sa->ತುಂಬು_avg ||
		      sa->util_avg ||
		      sa->runnable_avg);

	ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ u64 cfs_rq_ಕೊನೆಯ_update_ಸಮಯ(ರಚನೆ cfs_rq *cfs_rq)
{
	ಹಿಂತಿರುಗಿ u64_u32_ತುಂಬು_copy(cfs_rq->avg.ಕೊನೆಯ_update_ಸಮಯ,
				 cfs_rq->ಕೊನೆಯ_update_ಸಮಯ_copy);
}
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_FAIR_ಗುಂಪು_SCHED
/*
 * Because list_add_leaf_cfs_rq always places a child cfs_rq on the list
 * immediately before a parent cfs_rq, and cfs_rqs are removed from the list
 * bottom-up, we only have to test whether the cfs_rq before us on the list
 * is our child.
 * If cfs_rq is not on the list, test whether a child needs its to be added to
 * connect a branch to the tree  * (see list_add_leaf_cfs_rq() for details).
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool ಮಗು_cfs_rq_on_list(ರಚನೆ cfs_rq *cfs_rq)
{
	ರಚನೆ cfs_rq *prev_cfs_rq;
	ರಚನೆ list_ತಲೆ *prev;
	ರಚನೆ rq *rq = rq_ದ(cfs_rq);

	ಆದರೆ (cfs_rq->on_list) {
		prev = cfs_rq->ಎಲೆ_cfs_rq_list.prev;
	} ಇಲ್ಲದಿದ್ದರೆ {
		prev = rq->tmp_alone_branch;
	}

	ಆದರೆ (prev == &rq->ಎಲೆ_cfs_rq_list)
		ಹಿಂತಿರುಗಿ false;

	prev_cfs_rq = container_ದ(prev, ರಚನೆ cfs_rq, ಎಲೆ_cfs_rq_list);

	ಹಿಂತಿರುಗಿ (prev_cfs_rq->tg->ಪೋಷಕ == cfs_rq->tg);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool cfs_rq_is_decayed(ರಚನೆ cfs_rq *cfs_rq)
{
	ಆದರೆ (cfs_rq->ತುಂಬು.ತೂಕ)
		ಹಿಂತಿರುಗಿ false;

	ಆದರೆ (!ತುಂಬು_avg_is_decayed(&cfs_rq->avg))
		ಹಿಂತಿರುಗಿ false;

	ಆದರೆ (ಮಗು_cfs_rq_on_list(cfs_rq))
		ಹಿಂತಿರುಗಿ false;

	ಆದರೆ (cfs_rq->tg_ತುಂಬು_avg_contrib)
		ಹಿಂತಿರುಗಿ false;

	ಹಿಂತಿರುಗಿ true;
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
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ update_tg_ತುಂಬು_avg(ರಚನೆ cfs_rq *cfs_rq)
{
	ದೀರ್ಘ dl, dr;
	u64 now;

	/*
	 * No need to update load_avg for root_task_group as it is not used.
	 */
	ಆದರೆ (cfs_rq->tg == &ಬೇರು_task_ಗುಂಪು)
		ಹಿಂತಿರುಗಿ;

	/* rq has been offline and doesn't contribute to the share anymore: */
	ಆದರೆ (!cpu_ಸಕ್ರಿಯ(cpu_ದ(rq_ದ(cfs_rq))))
		ಹಿಂತಿರುಗಿ;

	/*
	 * For migration heavy workloads, access to tg->load_avg can be
	 * unbound. Limit the update rate to at most once per ms.
	 */
	now = rq_clock(rq_ದ(cfs_rq));
	ಆದರೆ (now - cfs_rq->ಕೊನೆಯ_update_tg_ತುಂಬು_avg < NSEC_ಪ್ರತಿ_MSEC)
		ಹಿಂತಿರುಗಿ;

	dl = cfs_rq->avg.ತುಂಬು_avg - cfs_rq->tg_ತುಂಬು_avg_contrib;
	dr = cfs_rq->avg.runnable_avg - cfs_rq->tg_runnable_avg_contrib;
	ಆದರೆ (ನಿರಪೇಕ್ಷ(dl) > cfs_rq->tg_ತುಂಬು_avg_contrib / 64 ||
	    ನಿರಪೇಕ್ಷ(dr) > cfs_rq->tg_runnable_avg_contrib / 64) {
		atomic_long_ಕೂಡಿಸು(dl, &cfs_rq->tg->ತುಂಬು_avg);
		atomic_long_ಕೂಡಿಸು(dr, &cfs_rq->tg->runnable_avg);
		cfs_rq->tg_ತುಂಬು_avg_contrib = cfs_rq->avg.ತುಂಬು_avg;
		cfs_rq->tg_runnable_avg_contrib = cfs_rq->avg.runnable_avg;
		cfs_rq->ಕೊನೆಯ_update_tg_ತುಂಬು_avg = now;
	}
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ clear_tg_ತುಂಬು_avg(ರಚನೆ cfs_rq *cfs_rq)
{
	ದೀರ್ಘ dl, dr;
	u64 now;

	/*
	 * No need to update load_avg for root_task_group, as it is not used.
	 */
	ಆದರೆ (cfs_rq->tg == &ಬೇರು_task_ಗುಂಪು)
		ಹಿಂತಿರುಗಿ;

	now = rq_clock(rq_ದ(cfs_rq));
	dl = 0 - cfs_rq->tg_ತುಂಬು_avg_contrib;
	dr = 0 - cfs_rq->tg_runnable_avg_contrib;
	atomic_long_ಕೂಡಿಸು(dl, &cfs_rq->tg->ತುಂಬು_avg);
	atomic_long_ಕೂಡಿಸು(dr, &cfs_rq->tg->runnable_avg);
	cfs_rq->tg_ತುಂಬು_avg_contrib = 0;
	cfs_rq->tg_runnable_avg_contrib = 0;
	cfs_rq->ಕೊನೆಯ_update_tg_ತುಂಬು_avg = now;
}

/* CPU offline callback: */
ಸ್ಥಿರ ಶೂನ್ಯ __maybe_unused clear_tg_offline_cfs_rqs(ರಚನೆ rq *rq)
{
	ರಚನೆ task_ಗುಂಪು *tg;

	lockdep_assert_rq_held(rq);

	/*
	 * The rq clock has already been updated in
	 * set_rq_offline(), so we should skip updating
	 * the rq clock again in unthrottle_cfs_rq().
	 */
	rq_clock_ಆರಂಭ_loop_update(rq);

	guard(rcu)();

	list_for_each_ನಮೂದು_rcu(tg, &task_groups, list) {
		ರಚನೆ cfs_rq *cfs_rq = tg_cfs_rq(tg, cpu_ದ(rq));

		clear_tg_ತುಂಬು_avg(cfs_rq);
	}

	rq_clock_stop_loop_update(rq);
}

/*
 * Called within set_task_rq() right before setting a task's CPU. The
 * caller only guarantees p->pi_lock is held; no other assumptions,
 * including the state of rq->lock, should be made.
 */
ಶೂನ್ಯ ಗಣ_task_rq_fair(ರಚನೆ sched_entity *se,
		      ರಚನೆ cfs_rq *prev, ರಚನೆ cfs_rq *ಮುಂದಿನ)
{
	u64 p_ಕೊನೆಯ_update_ಸಮಯ;
	u64 n_ಕೊನೆಯ_update_ಸಮಯ;

	ಆದರೆ (!sched_feat(ATTACH_AGE_ತುಂಬು))
		ಹಿಂತಿರುಗಿ;

	/*
	 * We are supposed to update the task to "current" time, then its up to
	 * date and ready to go to new CPU/cfs_rq. But we have difficulty in
	 * getting what current time is, so simply throw away the out-of-date
	 * time. This will result in the wakee task is less decayed, but giving
	 * the wakee more load sounds not bad.
	 */
	ಆದರೆ (!(se->avg.ಕೊನೆಯ_update_ಸಮಯ && prev))
		ಹಿಂತಿರುಗಿ;

	p_ಕೊನೆಯ_update_ಸಮಯ = cfs_rq_ಕೊನೆಯ_update_ಸಮಯ(prev);
	n_ಕೊನೆಯ_update_ಸಮಯ = cfs_rq_ಕೊನೆಯ_update_ಸಮಯ(ಮುಂದಿನ);

	__update_load_avg_blocked_se(p_ಕೊನೆಯ_update_ಸಮಯ, se);
	se->avg.ಕೊನೆಯ_update_ಸಮಯ = n_ಕೊನೆಯ_update_ಸಮಯ;
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
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
update_tg_cfs_util(ರಚನೆ cfs_rq *cfs_rq, ರಚನೆ sched_entity *se, ರಚನೆ cfs_rq *gcfs_rq)
{
	ದೀರ್ಘ delta_ಮೊತ್ತ, delta_avg = gcfs_rq->avg.util_avg - se->avg.util_avg;
	u32 new_ಮೊತ್ತ, divider;

	/* Nothing to update */
	ಆದರೆ (!delta_avg)
		ಹಿಂತಿರುಗಿ;

	/*
	 * cfs_rq->avg.period_contrib can be used for both cfs_rq and se.
	 * See ___update_load_avg() for details.
	 */
	divider = get_pelt_divider(&cfs_rq->avg);

	/* Set new sched_entity's utilization */
	se->avg.util_avg = gcfs_rq->avg.util_avg;
	new_ಮೊತ್ತ = se->avg.util_avg * divider;
	delta_ಮೊತ್ತ = (ದೀರ್ಘ)new_ಮೊತ್ತ - (ದೀರ್ಘ)se->avg.util_ಮೊತ್ತ;
	se->avg.util_ಮೊತ್ತ = new_ಮೊತ್ತ;

	/* Update parent cfs_rq utilization */
	__update_sa(&cfs_rq->avg, util, delta_avg, delta_ಮೊತ್ತ);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
update_tg_cfs_runnable(ರಚನೆ cfs_rq *cfs_rq, ರಚನೆ sched_entity *se, ರಚನೆ cfs_rq *gcfs_rq)
{
	ದೀರ್ಘ delta_ಮೊತ್ತ, delta_avg = gcfs_rq->avg.runnable_avg - se->avg.runnable_avg;
	u64 new_ಮೊತ್ತ;
	u32 divider;

	/* Nothing to update */
	ಆದರೆ (!delta_avg)
		ಹಿಂತಿರುಗಿ;

	/*
	 * cfs_rq->avg.period_contrib can be used for both cfs_rq and se.
	 * See ___update_load_avg() for details.
	 */
	divider = get_pelt_divider(&cfs_rq->avg);

	/* Set new sched_entity's runnable */
	se->avg.runnable_avg = gcfs_rq->avg.runnable_avg;
	new_ಮೊತ್ತ = (u64)se->avg.runnable_avg * divider;
	delta_ಮೊತ್ತ = (ದೀರ್ಘ)new_ಮೊತ್ತ - (ದೀರ್ಘ)se->avg.runnable_ಮೊತ್ತ;
	se->avg.runnable_ಮೊತ್ತ = new_ಮೊತ್ತ;

	/* Update parent cfs_rq runnable */
	__update_sa(&cfs_rq->avg, runnable, delta_avg, delta_ಮೊತ್ತ);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
update_tg_cfs_ತುಂಬು(ರಚನೆ cfs_rq *cfs_rq, ರಚನೆ sched_entity *se, ರಚನೆ cfs_rq *gcfs_rq)
{
	ದೀರ್ಘ delta_avg, running_ಮೊತ್ತ, runnable_ಮೊತ್ತ = gcfs_rq->prop_runnable_ಮೊತ್ತ;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ತುಂಬು_avg;
	u64 ತುಂಬು_ಮೊತ್ತ = 0;
	s64 delta_ಮೊತ್ತ;
	u32 divider;

	ಆದರೆ (!runnable_ಮೊತ್ತ)
		ಹಿಂತಿರುಗಿ;

	gcfs_rq->prop_runnable_ಮೊತ್ತ = 0;

	/*
	 * cfs_rq->avg.period_contrib can be used for both cfs_rq and se.
	 * See ___update_load_avg() for details.
	 */
	divider = get_pelt_divider(&cfs_rq->avg);

	ಆದರೆ (runnable_ಮೊತ್ತ >= 0) {
		/*
		 * Add runnable; clip at LOAD_AVG_MAX. Reflects that until
		 * the CPU is saturated running == runnable.
		 */
		runnable_ಮೊತ್ತ += se->avg.ತುಂಬು_ಮೊತ್ತ;
		runnable_ಮೊತ್ತ = min_t(ದೀರ್ಘ, runnable_ಮೊತ್ತ, divider);
	} ಇಲ್ಲದಿದ್ದರೆ {
		/*
		 * Estimate the new unweighted runnable_sum of the gcfs_rq by
		 * assuming all tasks are equally runnable.
		 */
		ಆದರೆ (scale_ತುಂಬು_ಕೆಳಕ್ಕೆ(gcfs_rq->ತುಂಬು.ತೂಕ)) {
			ತುಂಬು_ಮೊತ್ತ = div_u64(gcfs_rq->avg.ತುಂಬು_ಮೊತ್ತ,
				scale_ತುಂಬು_ಕೆಳಕ್ಕೆ(gcfs_rq->ತುಂಬು.ತೂಕ));
		}

		/* But make sure to not inflate se's runnable */
		runnable_ಮೊತ್ತ = min(se->avg.ತುಂಬು_ಮೊತ್ತ, ತುಂಬು_ಮೊತ್ತ);
	}

	/*
	 * runnable_sum can't be lower than running_sum
	 * Rescale running sum to be in the same range as runnable sum
	 * running_sum is in [0 : LOAD_AVG_MAX <<  SCHED_CAPACITY_SHIFT]
	 * runnable_sum is in [0 : LOAD_AVG_MAX]
	 */
	running_ಮೊತ್ತ = se->avg.util_ಮೊತ್ತ >> SCHED_ಸಾಮರ್ಥ್ಯ_ಸರಿಸು;
	runnable_ಮೊತ್ತ = max(runnable_ಮೊತ್ತ, running_ಮೊತ್ತ);

	ತುಂಬು_ಮೊತ್ತ = se_ತೂಕ(se) * runnable_ಮೊತ್ತ;
	ತುಂಬು_avg = div_u64(ತುಂಬು_ಮೊತ್ತ, divider);

	delta_avg = ತುಂಬು_avg - se->avg.ತುಂಬು_avg;
	ಆದರೆ (!delta_avg)
		ಹಿಂತಿರುಗಿ;

	delta_ಮೊತ್ತ = ತುಂಬು_ಮೊತ್ತ - (s64)se_ತೂಕ(se) * se->avg.ತುಂಬು_ಮೊತ್ತ;

	se->avg.ತುಂಬು_ಮೊತ್ತ = runnable_ಮೊತ್ತ;
	se->avg.ತುಂಬು_avg = ತುಂಬು_avg;
	__update_sa(&cfs_rq->avg, ತುಂಬು, delta_avg, delta_ಮೊತ್ತ);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ ಕೂಡಿಸು_tg_cfs_propagate(ರಚನೆ cfs_rq *cfs_rq, ದೀರ್ಘ runnable_ಮೊತ್ತ)
{
	cfs_rq->propagate = 1;
	cfs_rq->prop_runnable_ಮೊತ್ತ += runnable_ಮೊತ್ತ;
}

/* Update task and its cfs_rq load average */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ propagate_entity_ತುಂಬು_avg(ರಚನೆ sched_entity *se)
{
	ರಚನೆ cfs_rq *cfs_rq, *gcfs_rq;

	ಆದರೆ (entity_is_task(se))
		ಹಿಂತಿರುಗಿ 0;

	gcfs_rq = ಗುಂಪು_cfs_rq(se);
	ಆದರೆ (!gcfs_rq->propagate)
		ಹಿಂತಿರುಗಿ 0;

	gcfs_rq->propagate = 0;

	cfs_rq = cfs_rq_ದ(se);

	ಕೂಡಿಸು_tg_cfs_propagate(cfs_rq, gcfs_rq->prop_runnable_ಮೊತ್ತ);

	update_tg_cfs_util(cfs_rq, se, gcfs_rq);
	update_tg_cfs_runnable(cfs_rq, se, gcfs_rq);
	update_tg_cfs_ತುಂಬು(cfs_rq, se, gcfs_rq);

	trace_pelt_cfs_tp(cfs_rq);
	trace_pelt_se_tp(se);

	ಹಿಂತಿರುಗಿ 1;
}

/*
 * Check if we need to update the load and the utilization of a blocked
 * group_entity:
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool skip_blocked_update(ರಚನೆ sched_entity *se)
{
	ರಚನೆ cfs_rq *gcfs_rq = ಗುಂಪು_cfs_rq(se);

	/*
	 * If sched_entity still have not zero load or utilization, we have to
	 * decay it:
	 */
	ಆದರೆ (se->avg.ತುಂಬು_avg || se->avg.util_avg)
		ಹಿಂತಿರುಗಿ false;

	/*
	 * If there is a pending propagation, we have to update the load and
	 * the utilization of the sched_entity:
	 */
	ಆದರೆ (gcfs_rq->propagate)
		ಹಿಂತಿರುಗಿ false;

	/*
	 * Otherwise, the load and the utilization of the sched_entity is
	 * already zero and there is no pending propagation, so it will be a
	 * waste of time to try to decay it:
	 */
	ಹಿಂತಿರುಗಿ true;
}

#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_FAIR_GROUP_SCHED: */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ update_tg_ತುಂಬು_avg(ರಚನೆ cfs_rq *cfs_rq) {}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ clear_tg_offline_cfs_rqs(ರಚನೆ rq *rq) {}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ propagate_entity_ತುಂಬು_avg(ರಚನೆ sched_entity *se)
{
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ ಕೂಡಿಸು_tg_cfs_propagate(ರಚನೆ cfs_rq *cfs_rq, ದೀರ್ಘ runnable_ಮೊತ್ತ) {}

#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_FAIR_GROUP_SCHED */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_NO_HZ_COMMON
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ migrate_se_pelt_lag(ರಚನೆ sched_entity *se)
{
	u64 throttled = 0, now, lut;
	ರಚನೆ cfs_rq *cfs_rq;
	ರಚನೆ rq *rq;
	bool is_idle;

	ಆದರೆ (ತುಂಬು_avg_is_decayed(&se->avg))
		ಹಿಂತಿರುಗಿ;

	cfs_rq = cfs_rq_ದ(se);
	rq = rq_ದ(cfs_rq);

	rcu_read_ಬೀಗ();
	is_idle = is_idle_task(rcu_dereference_all(rq->curr));
	rcu_read_unlock();

	/*
	 * The lag estimation comes with a cost we don't want to pay all the
	 * time. Hence, limiting to the case where the source CPU is idle and
	 * we know we are at the greatest risk to have an outdated clock.
	 */
	ಆದರೆ (!is_idle)
		ಹಿಂತಿರುಗಿ;

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

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_CFS_BANDWIDTH
	throttled = u64_u32_ತುಂಬು(cfs_rq->throttled_pelt_idle);
	/* The clock has been stopped for throttling */
	ಆದರೆ (throttled == U64_MAX)
		ಹಿಂತಿರುಗಿ;
#ಆದರೆ_ಅಂತ್ಯ
	now = u64_u32_ತುಂಬು(rq->clock_pelt_idle);
	/*
	 * Paired with _update_idle_rq_clock_pelt(). It ensures at the worst case
	 * is observed the old clock_pelt_idle value and the new clock_idle,
	 * which lead to an underestimation. The opposite would lead to an
	 * overestimation.
	 */
	smp_rmb();
	lut = cfs_rq_ಕೊನೆಯ_update_ಸಮಯ(cfs_rq);

	now -= throttled;
	ಆದರೆ (now < lut)
		/*
		 * cfs_rq->avg.last_update_time is more recent than our
		 * estimation, let's use it.
		 */
		now = lut;
	ಇಲ್ಲದಿದ್ದರೆ
		now += sched_clock_cpu(cpu_ದ(rq)) - u64_u32_ತುಂಬು(rq->clock_idle);

	__update_load_avg_blocked_se(now, se);
}
#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_NO_HZ_COMMON: */
ಸ್ಥಿರ ಶೂನ್ಯ migrate_se_pelt_lag(ರಚನೆ sched_entity *se) {}
#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_NO_HZ_COMMON */

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
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
update_cfs_rq_ತುಂಬು_avg(u64 now, ರಚನೆ cfs_rq *cfs_rq)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ removed_ತುಂಬು = 0, removed_util = 0, removed_runnable = 0;
	ರಚನೆ sched_avg *sa = &cfs_rq->avg;
	ಪೂರ್ಣಾಂಕ decayed = 0;

	ಆದರೆ (cfs_rq->removed.nr) {
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ r;
		u32 divider = get_pelt_divider(&cfs_rq->avg);

		raw_spin_ಬೀಗ(&cfs_rq->removed.ಬೀಗ);
		ಅದಲುಬದಲು(cfs_rq->removed.util_avg, removed_util);
		ಅದಲುಬದಲು(cfs_rq->removed.ತುಂಬು_avg, removed_ತುಂಬು);
		ಅದಲುಬದಲು(cfs_rq->removed.runnable_avg, removed_runnable);
		cfs_rq->removed.nr = 0;
		raw_spin_unlock(&cfs_rq->removed.ಬೀಗ);

		r = removed_ತುಂಬು;
		__update_sa(sa, ತುಂಬು, -r, -r*divider);

		r = removed_util;
		__update_sa(sa, util, -r, -r*divider);

		r = removed_runnable;
		__update_sa(sa, runnable, -r, -r*divider);

		/*
		 * removed_runnable is the unweighted version of removed_load so we
		 * can use it to estimate removed_load_sum.
		 */
		ಕೂಡಿಸು_tg_cfs_propagate(cfs_rq,
			-(ದೀರ್ಘ)(removed_runnable * divider) >> SCHED_ಸಾಮರ್ಥ್ಯ_ಸರಿಸು);

		decayed = 1;
	}

	decayed |= __update_load_avg_cfs_rq(now, cfs_rq);
	u64_u32_store_copy(sa->ಕೊನೆಯ_update_ಸಮಯ,
			   cfs_rq->ಕೊನೆಯ_update_ಸಮಯ_copy,
			   sa->ಕೊನೆಯ_update_ಸಮಯ);
	ಹಿಂತಿರುಗಿ decayed;
}

/**
 * attach_entity_load_avg - attach this entity to its cfs_rq load avg
 * @cfs_rq: cfs_rq to attach to
 * @se: sched_entity to attach
 *
 * Must call update_cfs_rq_load_avg() before this, since we rely on
 * cfs_rq->avg.last_update_time being current.
 */
ಸ್ಥಿರ ಶೂನ್ಯ attach_entity_ತುಂಬು_avg(ರಚನೆ cfs_rq *cfs_rq, ರಚನೆ sched_entity *se)
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
	se->avg.ಕೊನೆಯ_update_ಸಮಯ = cfs_rq->avg.ಕೊನೆಯ_update_ಸಮಯ;
	se->avg.period_contrib = cfs_rq->avg.period_contrib;

	/*
	 * Hell(o) Nasty stuff.. we need to recompute _sum based on the new
	 * period_contrib. This isn't strictly correct, but since we're
	 * entirely outside of the PELT hierarchy, nobody cares if we truncate
	 * _sum a little.
	 */
	se->avg.util_ಮೊತ್ತ = se->avg.util_avg * divider;

	se->avg.runnable_ಮೊತ್ತ = se->avg.runnable_avg * divider;

	se->avg.ತುಂಬು_ಮೊತ್ತ = se->avg.ತುಂಬು_avg * divider;
	ಆದರೆ (se_ತೂಕ(se) < se->avg.ತುಂಬು_ಮೊತ್ತ)
		se->avg.ತುಂಬು_ಮೊತ್ತ = div_u64(se->avg.ತುಂಬು_ಮೊತ್ತ, se_ತೂಕ(se));
	ಇಲ್ಲದಿದ್ದರೆ
		se->avg.ತುಂಬು_ಮೊತ್ತ = 1;

	ಸರತಿಗೆ_ಸೇರಿಸು_ತುಂಬು_avg(cfs_rq, se);
	cfs_rq->avg.util_avg += se->avg.util_avg;
	cfs_rq->avg.util_ಮೊತ್ತ += se->avg.util_ಮೊತ್ತ;
	cfs_rq->avg.runnable_avg += se->avg.runnable_avg;
	cfs_rq->avg.runnable_ಮೊತ್ತ += se->avg.runnable_ಮೊತ್ತ;

	ಕೂಡಿಸು_tg_cfs_propagate(cfs_rq, se->avg.ತುಂಬು_ಮೊತ್ತ);

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
ಸ್ಥಿರ ಶೂನ್ಯ detach_entity_ತುಂಬು_avg(ರಚನೆ cfs_rq *cfs_rq, ರಚನೆ sched_entity *se)
{
	ಸರತಿಯಿಂದ_ತೆಗೆ_ತುಂಬು_avg(cfs_rq, se);
	__update_sa(&cfs_rq->avg, util, -se->avg.util_avg, -se->avg.util_ಮೊತ್ತ);
	__update_sa(&cfs_rq->avg, runnable, -se->avg.runnable_avg, -se->avg.runnable_ಮೊತ್ತ);

	ಕೂಡಿಸು_tg_cfs_propagate(cfs_rq, -se->avg.ತುಂಬು_ಮೊತ್ತ);

	cfs_rq_util_change(cfs_rq, 0);

	trace_pelt_cfs_tp(cfs_rq);
}

#ವ್ಯಾಖ್ಯಾನಿಸು UTIL_EST_MARGIN (SCHED_ಸಾಮರ್ಥ್ಯ_SCALE / 100)

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ util_est_update(ರಚನೆ sched_entity *se)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ewma, dequeued, ಕೊನೆಯ_ewma_diff;

	ಆದರೆ (!sched_feat(UTIL_EST))
		ಹಿಂತಿರುಗಿ;

	/* Get current estimate of utilization */
	ewma = READ_ONCE(se->avg.util_est);

	/*
	 * If the PELT values haven't changed since enqueue time,
	 * skip the util_est update.
	 */
	ಆದರೆ (ewma & UTIL_AVG_UNCHANGED)
		ಹಿಂತಿರುಗಿ;

	/* Get utilization at dequeue */
	dequeued = READ_ONCE(se->avg.util_avg);

	/*
	 * Reset EWMA on utilization increases, the moving average is used only
	 * to smooth utilization decreases.
	 */
	ಆದರೆ (ewma <= dequeued) {
		ewma = dequeued;
		ಹೋಗು ಮುಗಿದ;
	}

	/*
	 * Skip update of task's estimated utilization when its members are
	 * already ~1% close to its last activation value.
	 */
	ಕೊನೆಯ_ewma_diff = ewma - dequeued;
	ಆದರೆ (ಕೊನೆಯ_ewma_diff < UTIL_EST_MARGIN)
		ಹೋಗು ಮುಗಿದ;

	/*
	 * To avoid underestimate of task utilization, skip updates of EWMA if
	 * we cannot grant that thread got all CPU time it wanted.
	 */
	ಆದರೆ ((dequeued + UTIL_EST_MARGIN) < READ_ONCE(se->avg.runnable_avg))
		ಹೋಗು ಮುಗಿದ;

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
	ewma <<= UTIL_EST_ತೂಕ_ಸರಿಸು;
	ewma  -= ಕೊನೆಯ_ewma_diff;
	ewma >>= UTIL_EST_ತೂಕ_ಸರಿಸು;
ಮುಗಿದ:
	ewma |= UTIL_AVG_UNCHANGED;
	WRITE_ONCE(se->avg.util_est, ewma);

	trace_sched_util_est_se_tp(se);
}

/*
 * Optional action to be done while updating the load average
 */
#ವ್ಯಾಖ್ಯಾನಿಸು UPDATE_TG	0x01
#ವ್ಯಾಖ್ಯಾನಿಸು SKIP_AGE_ತುಂಬು	0x02
#ವ್ಯಾಖ್ಯಾನಿಸು DO_ATTACH	0x04
#ವ್ಯಾಖ್ಯಾನಿಸು DO_DETACH	0x08
#ವ್ಯಾಖ್ಯಾನಿಸು UPDATE_UTIL_EST	0x10

/* Update task and its cfs_rq load average */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ update_ತುಂಬು_avg(ರಚನೆ cfs_rq *cfs_rq, ರಚನೆ sched_entity *se, ಪೂರ್ಣಾಂಕ flags)
{
	u64 now = cfs_rq_clock_pelt(cfs_rq);
	ಪೂರ್ಣಾಂಕ decayed;

	/*
	 * Track task load average for carrying it to new CPU after migrated, and
	 * track group sched_entity load average for task_h_load calculation in migration
	 */
	ಆದರೆ (se->avg.ಕೊನೆಯ_update_ಸಮಯ && !(flags & SKIP_AGE_ತುಂಬು))
		__update_load_avg_se(now, cfs_rq, se);

	decayed  = update_cfs_rq_ತುಂಬು_avg(now, cfs_rq);
	decayed |= propagate_entity_ತುಂಬು_avg(se);

	ಆದರೆ (!se->avg.ಕೊನೆಯ_update_ಸಮಯ && (flags & DO_ATTACH)) {

		/*
		 * DO_ATTACH means we're here from enqueue_entity().
		 * !last_update_time means we've passed through
		 * migrate_task_rq_fair() indicating we migrated.
		 *
		 * IOW we're enqueueing a task on a new CPU.
		 */
		attach_entity_ತುಂಬು_avg(cfs_rq, se);
		update_tg_ತುಂಬು_avg(cfs_rq);

	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (flags & DO_DETACH) {
		/*
		 * DO_DETACH means we're here from dequeue_entity()
		 * and we are migrating task out of the CPU.
		 */
		detach_entity_ತುಂಬು_avg(cfs_rq, se);
		update_tg_ತುಂಬು_avg(cfs_rq);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (decayed) {
		cfs_rq_util_change(cfs_rq, 0);

		ಆದರೆ (flags & UPDATE_TG)
			update_tg_ತುಂಬು_avg(cfs_rq);
	}

	ಆದರೆ (flags & UPDATE_UTIL_EST)
		util_est_update(se);
}

/*
 * Synchronize entity load avg of dequeued entity without locking
 * the previous rq.
 */
ಸ್ಥಿರ ಶೂನ್ಯ sync_entity_ತುಂಬು_avg(ರಚನೆ sched_entity *se)
{
	ರಚನೆ cfs_rq *cfs_rq = cfs_rq_ದ(se);
	u64 ಕೊನೆಯ_update_ಸಮಯ;

	ಕೊನೆಯ_update_ಸಮಯ = cfs_rq_ಕೊನೆಯ_update_ಸಮಯ(cfs_rq);
	__update_load_avg_blocked_se(ಕೊನೆಯ_update_ಸಮಯ, se);
}

/*
 * Task first catches up with cfs_rq, and then subtract
 * itself from the cfs_rq (task must be off the queue now).
 */
ಸ್ಥಿರ ಶೂನ್ಯ remove_entity_ತುಂಬು_avg(ರಚನೆ sched_entity *se)
{
	ರಚನೆ cfs_rq *cfs_rq = cfs_rq_ದ(se);
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags;

	/*
	 * tasks cannot exit without having gone through wake_up_new_task() ->
	 * enqueue_task_fair() which will have added things to the cfs_rq,
	 * so we can remove unconditionally.
	 */

	sync_entity_ತುಂಬು_avg(se);

	raw_spin_ಬೀಗ_irqsave(&cfs_rq->removed.ಬೀಗ, flags);
	++cfs_rq->removed.nr;
	cfs_rq->removed.util_avg	+= se->avg.util_avg;
	cfs_rq->removed.ತುಂಬು_avg	+= se->avg.ತುಂಬು_avg;
	cfs_rq->removed.runnable_avg	+= se->avg.runnable_avg;
	raw_spin_unlock_irqrestore(&cfs_rq->removed.ಬೀಗ, flags);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ cfs_rq_runnable_avg(ರಚನೆ cfs_rq *cfs_rq)
{
	ಹಿಂತಿರುಗಿ cfs_rq->avg.runnable_avg;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ cfs_rq_ತುಂಬು_avg(ರಚನೆ cfs_rq *cfs_rq)
{
	ಹಿಂತಿರುಗಿ cfs_rq->avg.ತುಂಬು_avg;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sched_balance_newidle(ರಚನೆ rq *this_rq, ರಚನೆ rq_flags *rf)
	__must_hold(__rq_lockp(this_rq));

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ task_util(ರಚನೆ task_struct *p)
{
	ಹಿಂತಿರುಗಿ READ_ONCE(p->se.avg.util_avg);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ _task_util_est(ರಚನೆ task_struct *p)
{
	ಹಿಂತಿರುಗಿ READ_ONCE(p->se.avg.util_est) & ~UTIL_AVG_UNCHANGED;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ task_util_est(ರಚನೆ task_struct *p)
{
	ಹಿಂತಿರುಗಿ max(task_util(p), _task_util_est(p));
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ util_est_ಸರತಿಗೆ_ಸೇರಿಸು(ರಚನೆ cfs_rq *cfs_rq,
				    ರಚನೆ task_struct *p)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ enqueued;

	ಆದರೆ (!sched_feat(UTIL_EST))
		ಹಿಂತಿರುಗಿ;

	/* Update root cfs_rq's estimated utilization */
	enqueued  = cfs_rq->avg.util_est;
	enqueued += _task_util_est(p);
	WRITE_ONCE(cfs_rq->avg.util_est, enqueued);

	trace_sched_util_est_cfs_tp(cfs_rq);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ util_est_ಸರತಿಯಿಂದ_ತೆಗೆ(ರಚನೆ cfs_rq *cfs_rq,
				    ರಚನೆ task_struct *p)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ enqueued;

	ಆದರೆ (!sched_feat(UTIL_EST))
		ಹಿಂತಿರುಗಿ;

	/* Update root cfs_rq's estimated utilization */
	enqueued  = cfs_rq->avg.util_est;
	enqueued -= min_t(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ, enqueued, _task_util_est(p));
	WRITE_ONCE(cfs_rq->avg.util_est, enqueued);

	trace_sched_util_est_cfs_tp(cfs_rq);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ get_actual_cpu_ಸಾಮರ್ಥ್ಯ(ಪೂರ್ಣಾಂಕ cpu)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಸಾಮರ್ಥ್ಯ = arch_scale_cpu_ಸಾಮರ್ಥ್ಯ(cpu);

	ಸಾಮರ್ಥ್ಯ -= max(hw_ತುಂಬು_avg(cpu_rq(cpu)), cpufreq_get_pressure(cpu));

	ಹಿಂತಿರುಗಿ ಸಾಮರ್ಥ್ಯ;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ util_ಹೊಂದುತ್ತದೆ_cpu(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ util,
				ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ uclamp_min,
				ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ uclamp_max,
				ಪೂರ್ಣಾಂಕ cpu)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಸಾಮರ್ಥ್ಯ = ಸಾಮರ್ಥ್ಯ_ದ(cpu);
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಸಾಮರ್ಥ್ಯ_orig;
	bool ಹೊಂದುತ್ತದೆ, uclamp_max_ಹೊಂದುತ್ತದೆ;

	/*
	 * Check if the real util fits without any uclamp boost/cap applied.
	 */
	ಹೊಂದುತ್ತದೆ = ಹೊಂದುತ್ತದೆ_ಸಾಮರ್ಥ್ಯ(util, ಸಾಮರ್ಥ್ಯ);

	ಆದರೆ (!uclamp_is_ಬಳಸಿದ())
		ಹಿಂತಿರುಗಿ ಹೊಂದುತ್ತದೆ;

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
	ಸಾಮರ್ಥ್ಯ_orig = arch_scale_cpu_ಸಾಮರ್ಥ್ಯ(cpu);

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
	uclamp_max_ಹೊಂದುತ್ತದೆ = (ಸಾಮರ್ಥ್ಯ_orig == SCHED_ಸಾಮರ್ಥ್ಯ_SCALE) && (uclamp_max == SCHED_ಸಾಮರ್ಥ್ಯ_SCALE);
	uclamp_max_ಹೊಂದುತ್ತದೆ = !uclamp_max_ಹೊಂದುತ್ತದೆ && (uclamp_max <= ಸಾಮರ್ಥ್ಯ_orig);
	ಹೊಂದುತ್ತದೆ = ಹೊಂದುತ್ತದೆ || uclamp_max_ಹೊಂದುತ್ತದೆ;

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
	ಆದರೆ (ಹೊಂದುತ್ತದೆ && (util < uclamp_min) &&
	    (uclamp_min > get_actual_cpu_ಸಾಮರ್ಥ್ಯ(cpu)))
		ಹಿಂತಿರುಗಿ -1;

	ಹಿಂತಿರುಗಿ ಹೊಂದುತ್ತದೆ;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ task_ಹೊಂದುತ್ತದೆ_cpu(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ cpu)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ uclamp_min = uclamp_eff_ಮೌಲ್ಯ(p, UCLAMP_MIN);
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ uclamp_max = uclamp_eff_ಮೌಲ್ಯ(p, UCLAMP_MAX);
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ util = task_util_est(p);
	/*
	 * Return true only if the cpu fully fits the task requirements, which
	 * include the utilization but also the performance hints.
	 */
	ಹಿಂತಿರುಗಿ (util_ಹೊಂದುತ್ತದೆ_cpu(util, uclamp_min, uclamp_max, cpu) > 0);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ update_misfit_ಸ್ಥಿತಿ(ರಚನೆ task_struct *p, ರಚನೆ rq *rq)
{
	ಪೂರ್ಣಾಂಕ cpu = cpu_ದ(rq);

	ಆದರೆ (!sched_asym_cpucap_ಸಕ್ರಿಯ())
		ಹಿಂತಿರುಗಿ;

	/*
	 * Affinity allows us to go somewhere higher?  Or are we on biggest
	 * available CPU already? Or do we fit into this CPU ?
	 */
	ಆದರೆ (!p || (p->nr_cpus_allowed == 1) ||
	    (arch_scale_cpu_ಸಾಮರ್ಥ್ಯ(cpu) == p->max_allowed_ಸಾಮರ್ಥ್ಯ) ||
	    task_ಹೊಂದುತ್ತದೆ_cpu(p, cpu)) {

		rq->misfit_task_ತುಂಬು = 0;
		ಹಿಂತಿರುಗಿ;
	}

	/*
	 * Make sure that misfit_task_load will not be null even if
	 * task_h_load() returns 0.
	 */
	rq->misfit_task_ತುಂಬು = max_t(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ, task_h_ತುಂಬು(p), 1);
}

ಶೂನ್ಯ __setparam_fair(ರಚನೆ task_struct *p, ಸ್ಥಿರಾಂಕ ರಚನೆ sched_attr *attr)
{
	ರಚನೆ sched_entity *se = &p->se;

	p->static_prio = NICE_ಗಮ್ಯ_PRIO(attr->sched_nice);
	ಆದರೆ (attr->sched_runtime) {
		se->custom_ತುಂಡು = 1;
		se->ತುಂಡು = clamp_t(u64, attr->sched_runtime,
				      NSEC_ಪ್ರತಿ_MSEC/10,   /* HZ=1000 * 10 */
				      NSEC_ಪ್ರತಿ_MSEC*100); /* HZ=100  / 10 */
	} ಇಲ್ಲದಿದ್ದರೆ {
		se->custom_ತುಂಡು = 0;
		se->ತುಂಡು = sysctl_sched_ತಳ_ತುಂಡು;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ
ಇರಿಸು_entity(ರಚನೆ cfs_rq *cfs_rq, ರಚನೆ sched_entity *se, ಪೂರ್ಣಾಂಕ flags)
{
	u64 vslice, vruntime = avg_vruntime(cfs_rq);
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ nr_queued = cfs_rq->h_nr_queued;
	bool update_zero = false;
	s64 lag = 0;

	ಆದರೆ (!se->custom_ತುಂಡು)
		se->ತುಂಡು = sysctl_sched_ತಳ_ತುಂಡು;
	vslice = calc_delta_fair(se->ತುಂಡು, se);

	ಆದರೆ (flags & ಸರತಿಗೆ_ಸೇರಿಸು_QUEUED)
		nr_queued -= 1;

	/*
	 * Due to how V is constructed as the weighted average of entities,
	 * adding tasks with positive lag, or removing tasks with negative lag
	 * will move 'time' backwards, this can screw around with the lag of
	 * other tasks.
	 *
	 * EEVDF: placement strategy #1 / #2
	 */
	ಆದರೆ (sched_feat(ಇರಿಸು_LAG) && nr_queued && se->vlag) {
		ರಚನೆ sched_entity *curr = cfs_rq->curr;
		ದೀರ್ಘ ತುಂಬು, ತೂಕ;

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
		ತುಂಬು = cfs_rq->ಮೊತ್ತ_ತೂಕ;
		ಆದರೆ (curr && curr->on_rq)
			ತುಂಬು += avg_vruntime_ತೂಕ(cfs_rq, curr->h_ತುಂಬು.ತೂಕ);
		ಆದರೆ (WARN_ON_ONCE(!ತುಂಬು))
			ತುಂಬು = 1;

		ತೂಕ = avg_vruntime_ತೂಕ(cfs_rq, se->h_ತುಂಬು.ತೂಕ);

		/*
		 * Rewrite, because the product (W + w_i)*vl'_i might
		 * overflow s64:
		 *
		 *   vl_i = (W + w_i)*vl'_i / W
		 *        = (W*vl'_i + w_i*vl'_i) / W
		 *        = vl'_i + w_i*vl'_i/W
		 */
		lag += div64_long(lag * ತೂಕ, ತುಂಬು);

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
		ಆದರೆ (ತೂಕ > ತುಂಬು)
			update_zero = true;
	}

	se->vruntime = vruntime - lag;

	ಆದರೆ (update_zero)
		update_zero_vruntime(cfs_rq, -lag);

	ಆದರೆ (sched_feat(ಇರಿಸು_REL_DEADLINE) && se->rel_deadline) {
		se->deadline += se->vruntime;
		se->rel_deadline = 0;
		ಹಿಂತಿರುಗಿ;
	}

	/*
	 * When joining the competition; the existing tasks will be,
	 * on average, halfway through their slice, as such start tasks
	 * off with half a slice to ease into the competition.
	 */
	ಆದರೆ (sched_feat(ಇರಿಸು_DEADLINE_INITIAL) && (flags & ಸರತಿಗೆ_ಸೇರಿಸು_INITIAL))
		vslice /= 2;

	/*
	 * EEVDF: vd_i = ve_i + r_i/w_i
	 */
	se->deadline = se->vruntime + vslice;
}

ಸ್ಥಿರ ಶೂನ್ಯ check_ಸರತಿಗೆ_ಸೇರಿಸು_throttle(ರಚನೆ cfs_rq *cfs_rq);
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ cfs_rq_throttled(ರಚನೆ cfs_rq *cfs_rq);

ಸ್ಥಿರ ಶೂನ್ಯ
ಸರತಿಗೆ_ಸೇರಿಸು_entity(ರಚನೆ cfs_rq *cfs_rq, ರಚನೆ sched_entity *se, ಪೂರ್ಣಾಂಕ flags)
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
	update_ತುಂಬು_avg(cfs_rq, se, UPDATE_TG | DO_ATTACH);
	se_update_runnable(se);
	/*
	 * XXX update_load_avg() above will have attached us to the pelt sum;
	 * but update_cfs_group() here will re-adjust the weight and have to
	 * undo/redo all that. Seems wasteful.
	 */
	update_cfs_ಗುಂಪು(se);

	account_entity_ಸರತಿಗೆ_ಸೇರಿಸು(cfs_rq, se);

	/* Entity has migrated, no longer consider this task hot */
	ಆದರೆ (flags & ಸರತಿಗೆ_ಸೇರಿಸು_MIGRATED)
		se->exec_ಆರಂಭ = 0;

	check_schedstat_required();
	update_stats_ಸರತಿಗೆ_ಸೇರಿಸು_fair(cfs_rq, se, flags);
	se->on_rq = 1;

	ಆದರೆ (cfs_rq->nr_queued == 1) {
		check_ಸರತಿಗೆ_ಸೇರಿಸು_throttle(cfs_rq);
		list_ಕೂಡಿಸು_ಎಲೆ_cfs_rq(cfs_rq);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_CFS_BANDWIDTH
		ಆದರೆ (cfs_rq->pelt_clock_throttled) {
			ರಚನೆ rq *rq = rq_ದ(cfs_rq);

			cfs_rq->throttled_clock_pelt_ಸಮಯ += rq_clock_pelt(rq) -
				cfs_rq->throttled_clock_pelt;
			cfs_rq->pelt_clock_throttled = 0;
		}
#ಆದರೆ_ಅಂತ್ಯ
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ ಗಣ_ಮುಂದಿನ_buddy(ರಚನೆ cfs_rq *cfs_rq, ರಚನೆ sched_entity *se)
{
	ಆದರೆ (WARN_ON_ONCE(!se->on_rq || se->sched_delayed))
		ಹಿಂತಿರುಗಿ;
	ಆದರೆ (se_is_idle(se))
		ಹಿಂತಿರುಗಿ;
	cfs_rq->ಮುಂದಿನ = se;
}

ಸ್ಥಿರ ಶೂನ್ಯ clear_buddies(ರಚನೆ cfs_rq *cfs_rq, ರಚನೆ sched_entity *se)
{
	ಆದರೆ (cfs_rq->ಮುಂದಿನ == se)
		cfs_rq->ಮುಂದಿನ = ಖಾಲಿ;
}

ಸ್ಥಿರ __always_inline ಶೂನ್ಯ return_cfs_rq_runtime(ರಚನೆ cfs_rq *cfs_rq);

ಸ್ಥಿರ ಶೂನ್ಯ ಗಣ_delayed(ರಚನೆ sched_entity *se)
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
	ಆದರೆ (!entity_is_task(se)) {
		se->sched_delayed = 1;
		ಹಿಂತಿರುಗಿ;
	}

	/*
	 * Drop a task leaving the runnable set.
	 * Needs to be called before sched_delayed is set.
	 * clear_delayed() mirrors this after clearing the flag.
	 */
	pref_llc_running_dec(rq_ದ(cfs_rq_ದ(se)), task_ದ(se));
	se->sched_delayed = 1;

	for_each_sched_entity(se) {
		ರಚನೆ cfs_rq *cfs_rq = cfs_rq_ದ(se);

		cfs_rq->h_nr_runnable--;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ clear_delayed(ರಚನೆ sched_entity *se)
{
	se->sched_delayed = 0;

	/*
	 * Delayed se of cfs_rq have no tasks queued on them.
	 * Do not adjust h_nr_runnable since a dequeue has
	 * already accounted for it or an enqueue of a task
	 * below it will account for it in enqueue_task_fair().
	 */
	ಆದರೆ (!entity_is_task(se))
		ಹಿಂತಿರುಗಿ;

	/*
	 * Re-add on wake, after sched_delayed is cleared. On a final delayed
	 * dequeue account_llc_dequeue() already cleared pref_llc_queued, so
	 * this does nothing.
	 */
	pref_llc_running_inc(rq_ದ(cfs_rq_ದ(se)), task_ದ(se));

	for_each_sched_entity(se) {
		ರಚನೆ cfs_rq *cfs_rq = cfs_rq_ದ(se);

		cfs_rq->h_nr_runnable++;
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ
ಸರತಿಯಿಂದ_ತೆಗೆ_entity(ರಚನೆ cfs_rq *cfs_rq, ರಚನೆ sched_entity *se, ಪೂರ್ಣಾಂಕ flags)
{
	ಪೂರ್ಣಾಂಕ action = UPDATE_TG;

	ಆದರೆ (entity_is_task(se)) {
		ಆದರೆ (task_on_rq_migrating(task_ದ(se)))
			action |= DO_DETACH;

		ಆದರೆ ((flags & ಸರತಿಯಿಂದ_ತೆಗೆ_SLEEP) && !(flags & ಸರತಿಯಿಂದ_ತೆಗೆ_DELAYED))
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
	update_ತುಂಬು_avg(cfs_rq, se, action);
	se_update_runnable(se);

	update_stats_ಸರತಿಯಿಂದ_ತೆಗೆ_fair(cfs_rq, se, flags);

	se->on_rq = 0;
	account_entity_ಸರತಿಯಿಂದ_ತೆಗೆ(cfs_rq, se);

	/* return excess runtime on last dequeue */
	return_cfs_rq_runtime(cfs_rq);

	update_cfs_ಗುಂಪು(se);

	ಆದರೆ (cfs_rq->nr_queued == 0) {
		update_idle_cfs_rq_clock_pelt(cfs_rq);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_CFS_BANDWIDTH
		ಆದರೆ (throttled_hierarchy(cfs_rq)) {
			ರಚನೆ rq *rq = rq_ದ(cfs_rq);

			list_del_ಎಲೆ_cfs_rq(cfs_rq);
			cfs_rq->throttled_clock_pelt = rq_clock_pelt(rq);
			cfs_rq->pelt_clock_throttled = 1;
		}
#ಆದರೆ_ಅಂತ್ಯ
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ
ಗಣ_ಮುಂದಿನ_entity(ರಚನೆ cfs_rq *cfs_rq, ರಚನೆ sched_entity *se)
{
	/* 'current' is not kept within the tree. */
	ಆದರೆ (se->on_rq) {
		/*
		 * Any task has to be enqueued before it get to execute on
		 * a CPU. So account for the time it spent waiting on the
		 * runqueue.
		 */
		update_stats_wait_ಕೊನೆ_fair(cfs_rq, se);
		update_ತುಂಬು_avg(cfs_rq, se, UPDATE_TG);
	}

	update_stats_curr_ಆರಂಭ(cfs_rq, se);
	WARN_ON_ONCE(cfs_rq->h_curr);
	cfs_rq->h_curr = se;

	/*
	 * Track our maximum slice length, if the CPU's load is at
	 * least twice that of our own weight (i.e. don't track it
	 * when there are only lesser-weight tasks around):
	 */
	ಆದರೆ (schedstat_enabled() &&
	    rq_ದ(cfs_rq)->cfs.ತುಂಬು.ತೂಕ >= 2*se->ತುಂಬು.ತೂಕ) {
		ರಚನೆ sched_statistics *stats;

		stats = __schedstats_from_se(se);
		__schedstat_set(stats->ತುಂಡು_max,
				max((u64)stats->ತುಂಡು_max,
				    se->ಮೊತ್ತ_exec_runtime - se->prev_ಮೊತ್ತ_exec_runtime));
	}

	se->prev_ಮೊತ್ತ_exec_runtime = se->ಮೊತ್ತ_exec_runtime;
}

ಸ್ಥಿರ bool __dequeue_task(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ flags);

ಸ್ಥಿರ ರಚನೆ sched_entity *
pick_ಮುಂದಿನ_entity(ರಚನೆ rq *rq, bool protect)
{
	ರಚನೆ cfs_rq *cfs_rq = &rq->cfs;
	ರಚನೆ sched_entity *se;

	se = pick_eevdf(cfs_rq, protect);
	ಆದರೆ (se->sched_delayed) {
		__dequeue_task(rq, task_ದ(se), ಸರತಿಯಿಂದ_ತೆಗೆ_SLEEP | ಸರತಿಯಿಂದ_ತೆಗೆ_DELAYED);
		/*
		 * Must not reference @se again, see __block_task().
		 */
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;
	}
	ಹಿಂತಿರುಗಿ se;
}

ಸ್ಥಿರ ಶೂನ್ಯ put_prev_entity(ರಚನೆ cfs_rq *cfs_rq, ರಚನೆ sched_entity *prev)
{
	/*
	 * If still on the runqueue then deactivate_task()
	 * was not called and update_curr() has to be done:
	 */
	ಆದರೆ (prev->on_rq)
		update_curr(cfs_rq);

	ಆದರೆ (prev->on_rq) {
		update_stats_wait_ಆರಂಭ_fair(cfs_rq, prev);
		/* in !on_rq case, update occurred at dequeue */
		update_ತುಂಬು_avg(cfs_rq, prev, 0);
	}
	WARN_ON_ONCE(cfs_rq->h_curr != prev);
	cfs_rq->h_curr = ಖಾಲಿ;
}

ಸ್ಥಿರ ಶೂನ್ಯ
entity_tick(ರಚನೆ cfs_rq *cfs_rq, ರಚನೆ sched_entity *curr, ಪೂರ್ಣಾಂಕ queued)
{
	/*
	 * Update run-time statistics of the 'current'.
	 */
	update_curr(cfs_rq);

	/*
	 * Ensure that runnable average is periodically updated.
	 */
	update_ತುಂಬು_avg(cfs_rq, curr, UPDATE_TG);
	update_cfs_ಗುಂಪು(curr);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_HRTICK
	/*
	 * queued ticks are scheduled to match the slice, so don't bother
	 * validating it and just reschedule.
	 */
	ಆದರೆ (queued) {
		resched_curr(rq_ದ(cfs_rq));
		ಹಿಂತಿರುಗಿ;
	}
#ಆದರೆ_ಅಂತ್ಯ
}


/**************************************************
 * CFS bandwidth control machinery
 */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_CFS_BANDWIDTH

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_JUMP_LABEL
ಸ್ಥಿರ ರಚನೆ static_ಕೀಲಿ __cfs_bandwidth_used;

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool cfs_bandwidth_ಬಳಸಿದ(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ static_ಕೀಲಿ_false(&__cfs_bandwidth_used);
}

ಶೂನ್ಯ cfs_bandwidth_usage_inc(ಶೂನ್ಯ)
{
	static_ಕೀಲಿ_slow_inc_cpuslocked(&__cfs_bandwidth_used);
}

ಶೂನ್ಯ cfs_bandwidth_usage_dec(ಶೂನ್ಯ)
{
	static_ಕೀಲಿ_slow_dec_cpuslocked(&__cfs_bandwidth_used);
}
#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_JUMP_LABEL: */
ಸ್ಥಿರ bool cfs_bandwidth_ಬಳಸಿದ(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ true;
}

ಶೂನ್ಯ cfs_bandwidth_usage_inc(ಶೂನ್ಯ) {}
ಶೂನ್ಯ cfs_bandwidth_usage_dec(ಶೂನ್ಯ) {}
#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_JUMP_LABEL */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ u64 sched_cfs_bandwidth_ತುಂಡು(ಶೂನ್ಯ)
{
	ಹಿಂತಿರುಗಿ (u64)sysctl_sched_cfs_bandwidth_ತುಂಡು * NSEC_ಪ್ರತಿ_USEC;
}

/*
 * Replenish runtime according to assigned quota. We use sched_clock_cpu
 * directly instead of rq->clock to avoid adding additional synchronization
 * around rq->lock.
 *
 * requires cfs_b->lock
 */
ಶೂನ್ಯ __refill_cfs_bandwidth_runtime(ರಚನೆ cfs_bandwidth *cfs_b)
{
	s64 runtime;

	ಆದರೆ (unlikely(cfs_b->quota == RUNTIME_INF))
		ಹಿಂತಿರುಗಿ;

	cfs_b->runtime += cfs_b->quota;
	runtime = cfs_b->runtime_snap - cfs_b->runtime;
	ಆದರೆ (runtime > 0) {
		cfs_b->ಅವಧಿ_ಸಮಯ += runtime;
		cfs_b->nr_ಅವಧಿ++;
	}

	cfs_b->runtime = min(cfs_b->runtime, cfs_b->quota + cfs_b->ಅವಧಿ);
	cfs_b->runtime_snap = cfs_b->runtime;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ cfs_bandwidth *tg_cfs_bandwidth(ರಚನೆ task_ಗುಂಪು *tg)
{
	ಹಿಂತಿರುಗಿ &tg->cfs_bandwidth;
}

/* returns 0 on failure to allocate runtime */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ __assign_cfs_rq_runtime(ರಚನೆ cfs_bandwidth *cfs_b,
				   ರಚನೆ cfs_rq *cfs_rq, u64 ಗುರಿ_runtime)
{
	u64 min_amount, amount = 0;

	lockdep_assert_held(&cfs_b->ಬೀಗ);

	/* note: this is a positive sum as runtime_remaining <= 0 */
	min_amount = ಗುರಿ_runtime - cfs_rq->runtime_ಉಳಿದ;

	ಆದರೆ (cfs_b->quota == RUNTIME_INF)
		amount = min_amount;
	ಇಲ್ಲದಿದ್ದರೆ {
		ಆರಂಭ_cfs_bandwidth(cfs_b);

		ಆದರೆ (cfs_b->runtime > 0) {
			amount = min(cfs_b->runtime, min_amount);
			cfs_b->runtime -= amount;
			cfs_b->idle = 0;
		}
	}

	cfs_rq->runtime_ಉಳಿದ += amount;

	ಹಿಂತಿರುಗಿ cfs_rq->runtime_ಉಳಿದ > 0;
}

ಸ್ಥಿರ bool throttle_cfs_rq(ರಚನೆ cfs_rq *cfs_rq);

ಸ್ಥಿರ bool __account_cfs_rq_runtime(ರಚನೆ cfs_rq *cfs_rq, u64 delta_exec)
{
	/* dock delta_exec before expiring quota (as it could span periods) */
	cfs_rq->runtime_ಉಳಿದ -= delta_exec;

	ಆದರೆ (likely(cfs_rq->runtime_ಉಳಿದ > 0))
		ಹಿಂತಿರುಗಿ false;

	ಆದರೆ (cfs_rq->throttled)
		ಹಿಂತಿರುಗಿ true;
	/*
	 * throttle_cfs_rq() will try to extend the runtime first
	 * before throttling the hierarchy.
	 */
	ಹಿಂತಿರುಗಿ throttle_cfs_rq(cfs_rq);
}

ಸ್ಥಿರ __always_inline
bool account_cfs_rq_runtime(ರಚನೆ cfs_rq *cfs_rq, u64 delta_exec)
{
	ಆದರೆ (!cfs_bandwidth_ಬಳಸಿದ() || !cfs_rq->runtime_enabled)
		ಹಿಂತಿರುಗಿ false;

	ಹಿಂತಿರುಗಿ __account_cfs_rq_runtime(cfs_rq, delta_exec);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ cfs_rq_throttled(ರಚನೆ cfs_rq *cfs_rq)
{
	ಹಿಂತಿರುಗಿ cfs_bandwidth_ಬಳಸಿದ() && cfs_rq->throttled;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool cfs_rq_pelt_clock_throttled(ರಚನೆ cfs_rq *cfs_rq)
{
	ಹಿಂತಿರುಗಿ cfs_bandwidth_ಬಳಸಿದ() && cfs_rq->pelt_clock_throttled;
}

/* check whether cfs_rq, or any parent, is throttled */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ throttled_hierarchy(ರಚನೆ cfs_rq *cfs_rq)
{
	ಹಿಂತಿರುಗಿ cfs_bandwidth_ಬಳಸಿದ() && cfs_rq->throttle_ಎಣಿಕೆ;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ lb_throttled_hierarchy(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ dst_cpu)
{
	ಹಿಂತಿರುಗಿ throttled_hierarchy(tg_cfs_rq(task_ಗುಂಪು(p), dst_cpu));
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool task_is_throttled(ರಚನೆ task_struct *p)
{
	ಹಿಂತಿರುಗಿ cfs_bandwidth_ಬಳಸಿದ() && p->throttled;
}

ಸ್ಥಿರ bool ಸರತಿಯಿಂದ_ತೆಗೆ_task_fair(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ flags);
ಸ್ಥಿರ ಶೂನ್ಯ throttle_cfs_rq_work(ರಚನೆ callback_ತಲೆ *work)
{
	ರಚನೆ task_struct *p = container_ದ(work, ರಚನೆ task_struct, sched_throttle_work);
	ರಚನೆ sched_entity *se;
	ರಚನೆ cfs_rq *cfs_rq;
	ರಚನೆ rq *rq;

	WARN_ON_ONCE(p != current);
	p->sched_throttle_work.ಮುಂದಿನ = &p->sched_throttle_work;

	/*
	 * If task is exiting, then there won't be a return to userspace, so we
	 * don't have to bother with any of this.
	 */
	ಆದರೆ ((p->flags & PF_EXITING))
		ಹಿಂತಿರುಗಿ;

	scoped_guard(task_rq_ಬೀಗ_2, p) {
		se = &p->se;
		cfs_rq = cfs_rq_ದ(se);

		/* Raced, forget */
		ಆದರೆ (p->sched_class != &fair_sched_class)
			ಹಿಂತಿರುಗಿ;

		/*
		 * If not in limbo, then either replenish has happened or this
		 * task got migrated out of the throttled cfs_rq, move along.
		 */
		ಆದರೆ (!cfs_rq->throttle_ಎಣಿಕೆ)
			ಹಿಂತಿರುಗಿ;
		rq = scope.rq;
		update_rq_clock(rq);
		WARN_ON_ONCE(p->throttled || !list_ಖಾಲಿಗಳು(&p->throttle_ಗಂಟು));
		ಸರತಿಯಿಂದ_ತೆಗೆ_task_fair(rq, p, ಸರತಿಯಿಂದ_ತೆಗೆ_SLEEP | ಸರತಿಯಿಂದ_ತೆಗೆ_THROTTLE);
		list_ಕೂಡಿಸು(&p->throttle_ಗಂಟು, &cfs_rq->throttled_limbo_list);
		/*
		 * Must not set throttled before dequeue or dequeue will
		 * mistakenly regard this task as an already throttled one.
		 */
		p->throttled = true;
		resched_curr(rq);
	}
}

ಶೂನ್ಯ init_cfs_throttle_work(ರಚನೆ task_struct *p)
{
	init_task_work(&p->sched_throttle_work, throttle_cfs_rq_work);
	/* Protect against double add, see throttle_cfs_rq() and throttle_cfs_rq_work() */
	p->sched_throttle_work.ಮುಂದಿನ = &p->sched_throttle_work;
	INIT_LIST_ತಲೆ(&p->throttle_ಗಂಟು);
}

/*
 * Task is throttled and someone wants to dequeue it again:
 * it could be sched/core when core needs to do things like
 * task affinity change, task group change, task sched class
 * change etc. and in these cases, DEQUEUE_SLEEP is not set;
 * or the task is blocked after throttled due to freezer etc.
 * and in these cases, DEQUEUE_SLEEP is set.
 */
ಸ್ಥಿರ ಶೂನ್ಯ detach_task_cfs_rq(ರಚನೆ task_struct *p);
ಸ್ಥಿರ ಶೂನ್ಯ ಸರತಿಯಿಂದ_ತೆಗೆ_throttled_task(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ flags)
{
	WARN_ON_ONCE(p->se.on_rq);
	list_del_init(&p->throttle_ಗಂಟು);

	/* task blocked after throttled */
	ಆದರೆ (flags & ಸರತಿಯಿಂದ_ತೆಗೆ_SLEEP) {
		p->throttled = false;
		ಹಿಂತಿರುಗಿ;
	}

	/*
	 * task is migrating off its old cfs_rq, detach
	 * the task's load from its old cfs_rq.
	 */
	ಆದರೆ (task_on_rq_migrating(p))
		detach_task_cfs_rq(p);
}

ಸ್ಥಿರ bool ಸರತಿಗೆ_ಸೇರಿಸು_throttled_task(ರಚನೆ task_struct *p)
{
	ರಚನೆ cfs_rq *cfs_rq = cfs_rq_ದ(&p->se);

	/* @p should have gone through dequeue_throttled_task() first */
	WARN_ON_ONCE(!list_ಖಾಲಿಗಳು(&p->throttle_ಗಂಟು));

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
	ಆದರೆ (throttled_hierarchy(cfs_rq) &&
	    !task_current_donor(rq_ದ(cfs_rq), p)) {
		list_ಕೂಡಿಸು(&p->throttle_ಗಂಟು, &cfs_rq->throttled_limbo_list);
		ಹಿಂತಿರುಗಿ true;
	}

	/* we can't take the fast path, do an actual enqueue*/
	p->throttled = false;
	ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ ಶೂನ್ಯ ಸರತಿಗೆ_ಸೇರಿಸು_task_fair(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ flags);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ tg_unthrottle_ಮೇಲೆ(ರಚನೆ task_ಗುಂಪು *tg, ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	ರಚನೆ rq *rq = ದತ್ತಾಂಶ;
	ರಚನೆ cfs_rq *cfs_rq = tg_cfs_rq(tg, cpu_ದ(rq));
	ರಚನೆ task_struct *p, *tmp;
	LIST_ತಲೆ(throttled_tasks);

	/*
	 * If cfs_rq->curr is set, the cfs_rq might not have caught up
	 * since the last clock update. Do it now before we begin
	 * queueing task onto it to save the need for unnecessarily
	 * unthrottle the hierarchy for this cfs_rq to be throttled
	 * right back again.
	 */
	update_curr(cfs_rq);

	ಆದರೆ (--cfs_rq->throttle_ಎಣಿಕೆ)
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (cfs_rq->pelt_clock_throttled) {
		cfs_rq->throttled_clock_pelt_ಸಮಯ += rq_clock_pelt(rq) -
					     cfs_rq->throttled_clock_pelt;
		cfs_rq->pelt_clock_throttled = 0;
	}

	ಆದರೆ (cfs_rq->throttled_clock_self) {
		u64 delta = rq_clock(rq) - cfs_rq->throttled_clock_self;

		cfs_rq->throttled_clock_self = 0;

		ಆದರೆ (WARN_ON_ONCE((s64)delta < 0))
			delta = 0;

		cfs_rq->throttled_clock_self_ಸಮಯ += delta;
	}

	/*
	 * Move the tasks to a local list since an update_curr() during
	 * enqueue_task_fair() can throttle a higher cfs_rq, and it can
	 * see the "throttled_limbo_list" being non-empty in
	 * tg_throttle_down() if throttle_count turned 0 above.
	 */
	list_splice_init(&cfs_rq->throttled_limbo_list, &throttled_tasks);

	/* Re-enqueue the tasks that have been throttled at this level. */
	list_for_each_ನಮೂದು_ಸುರಕ್ಷಿತ(p, tmp, &throttled_tasks, throttle_ಗಂಟು) {
		/*
		 * Back to being throttled! Break out and put the remaining
		 * tasks back onto the limbo_list to prevent running them
		 * unnecessarily.
		 */
		ಆದರೆ (cfs_rq->throttle_ಎಣಿಕೆ)
			ನಿಲ್ಲಿಸು;

		list_del_init(&p->throttle_ಗಂಟು);
		p->throttled = false;
		ಸರತಿಗೆ_ಸೇರಿಸು_task_fair(rq, p, ಸರತಿಗೆ_ಸೇರಿಸು_WAKEUP);
	}

	list_splice(&throttled_tasks, &cfs_rq->throttled_limbo_list);

	/* Add cfs_rq with load or one or more already running entities to the list */
	ಆದರೆ (!cfs_rq_is_decayed(cfs_rq))
		list_ಕೂಡಿಸು_ಎಲೆ_cfs_rq(cfs_rq);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool task_has_throttle_work(ರಚನೆ task_struct *p)
{
	ಹಿಂತಿರುಗಿ p->sched_throttle_work.ಮುಂದಿನ != &p->sched_throttle_work;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ task_throttle_ಸಿದ್ಧತೆ_work(ರಚನೆ task_struct *p)
{
	ಆದರೆ (task_has_throttle_work(p))
		ಹಿಂತಿರುಗಿ;

	/*
	 * Kthreads and exiting tasks don't return to userspace, so adding the
	 * work is pointless
	 */
	ಆದರೆ ((p->flags & (PF_EXITING | PF_KTHREAD)))
		ಹಿಂತಿರುಗಿ;

	task_work_ಕೂಡಿಸು(p, &p->sched_throttle_work, TWA_RESUME);
}

ಸ್ಥಿರ ಶೂನ್ಯ ದಾಖಲೆ_throttle_clock(ರಚನೆ cfs_rq *cfs_rq)
{
	ರಚನೆ rq *rq = rq_ದ(cfs_rq);

	ಆದರೆ (cfs_rq_throttled(cfs_rq) && !cfs_rq->throttled_clock)
		cfs_rq->throttled_clock = rq_clock(rq);

	ಆದರೆ (!cfs_rq->throttled_clock_self)
		cfs_rq->throttled_clock_self = rq_clock(rq);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ tg_throttle_ಕೆಳಕ್ಕೆ(ರಚನೆ task_ಗುಂಪು *tg, ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	ರಚನೆ rq *rq = ದತ್ತಾಂಶ;
	ರಚನೆ cfs_rq *cfs_rq = tg_cfs_rq(tg, cpu_ದ(rq));

	ಆದರೆ (cfs_rq->throttle_ಎಣಿಕೆ++)
		ಹಿಂತಿರುಗಿ 0;

	/*
	 * For cfs_rqs that still have entities enqueued, PELT clock
	 * stop happens at dequeue time when all entities are dequeued.
	 */
	ಆದರೆ (!cfs_rq->nr_queued) {
		list_del_ಎಲೆ_cfs_rq(cfs_rq);
		cfs_rq->throttled_clock_pelt = rq_clock_pelt(rq);
		cfs_rq->pelt_clock_throttled = 1;
	}

	WARN_ON_ONCE(cfs_rq->throttled_clock_self);
	WARN_ON_ONCE(!list_ಖಾಲಿಗಳು(&cfs_rq->throttled_limbo_list));
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ bool throttle_cfs_rq(ರಚನೆ cfs_rq *cfs_rq)
{
	ರಚನೆ cfs_bandwidth *cfs_b = tg_cfs_bandwidth(cfs_rq->tg);
	ರಚನೆ sched_entity *curr = cfs_rq->h_curr;
	ರಚನೆ rq *rq = rq_ದ(cfs_rq);

	scoped_guard(raw_spinlock, &cfs_b->ಬೀಗ) {
		u64 ಗುರಿ_runtime = 1;

		/*
		 * If cfs_rq->h_curr is still runnable, we are here from an
		 * update_curr(). Request sysctl_sched_cfs_bandwidth_slice
		 * worth of bandwidth to continue running.
		 *
		 * If the curr is not runnable, just request enough bandwidth
		 * to be runnable next time the pick selects this cfs_rq.
		 */
		ಆದರೆ (curr && curr->on_rq)
			ಗುರಿ_runtime = sched_cfs_bandwidth_ತುಂಡು();

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
		ಆದರೆ (__assign_cfs_rq_runtime(cfs_b, cfs_rq, ಗುರಿ_runtime))
			ಹಿಂತಿರುಗಿ false;

		/*
		 * No bandwidth available; Add ourselves on the list to be
		 * unthrottled later.
		 */
		list_ಕೂಡಿಸು_ಬಾಲ_rcu(&cfs_rq->throttled_list,
				  &cfs_b->throttled_cfs_rq);
	}

	/* freeze hierarchy runnable averages while throttled */
	scoped_guard(rcu)
		walk_tg_ವೃಕ್ಷ_from(cfs_rq->tg, tg_throttle_ಕೆಳಕ್ಕೆ, tg_nop, (ಶೂನ್ಯ *)rq);

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
	ಆದರೆ (curr && curr->on_rq)
		task_throttle_ಸಿದ್ಧತೆ_work(rq->donor);

	ಹಿಂತಿರುಗಿ true;
}

ಶೂನ್ಯ unthrottle_cfs_rq(ರಚನೆ cfs_rq *cfs_rq)
{
	ರಚನೆ rq *rq = rq_ದ(cfs_rq);
	ರಚನೆ cfs_bandwidth *cfs_b = tg_cfs_bandwidth(cfs_rq->tg);
	ರಚನೆ sched_entity *se = cfs_rq_se(cfs_rq);

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
	ಆದರೆ (cfs_rq->runtime_enabled && cfs_rq->runtime_ಉಳಿದ <= 0)
		ಹಿಂತಿರುಗಿ;

	cfs_rq->throttled = 0;

	scoped_guard(raw_spinlock, &cfs_b->ಬೀಗ) {
		list_del_rcu(&cfs_rq->throttled_list);

		ಆದರೆ (!cfs_rq->throttled_clock)
			ನಿಲ್ಲಿಸು;

		cfs_b->throttled_ಸಮಯ += rq_clock(rq) - cfs_rq->throttled_clock;
		cfs_rq->throttled_clock = 0;
	}

	/* update hierarchical throttle state */
	walk_tg_ವೃಕ್ಷ_from(cfs_rq->tg, tg_nop, tg_unthrottle_ಮೇಲೆ, (ಶೂನ್ಯ *)rq);

	ಆದರೆ (!cfs_rq->ತುಂಬು.ತೂಕ) {
		ಆದರೆ (!cfs_rq->on_list)
			ಹಿಂತಿರುಗಿ;
		/*
		 * Nothing to run but something to decay (on_list)?
		 * Complete the branch.
		 */
		for_each_sched_entity(se) {
			ಆದರೆ (list_ಕೂಡಿಸು_ಎಲೆ_cfs_rq(cfs_rq_ದ(se)))
				ನಿಲ್ಲಿಸು;
		}
	}

	assert_list_ಎಲೆ_cfs_rq(rq);

	/* Determine whether we need to wake up potentially idle CPU: */
	ಆದರೆ (rq->curr == rq->idle && rq->cfs.h_nr_queued)
		resched_curr(rq);
}

ಸ್ಥಿರ ಶೂನ್ಯ __cfsb_csd_unthrottle(ಶೂನ್ಯ *arg)
{
	ರಚನೆ cfs_rq *cursor, *tmp;
	ರಚನೆ rq *rq = arg;

	guard(rq_ಬೀಗ)(rq);

	/*
	 * Iterating over the list can trigger several call to
	 * update_rq_clock() in unthrottle_cfs_rq().
	 * Do it once and skip the potential next ones.
	 */
	update_rq_clock(rq);
	rq_clock_ಆರಂಭ_loop_update(rq);

	/*
	 * Since we hold rq lock we're safe from concurrent manipulation of
	 * the CSD list. However, this RCU critical section annotates the
	 * fact that we pair with sched_free_group_rcu(), so that we cannot
	 * race with group being freed in the window between removing it
	 * from the list and advancing to the next entry in the list.
	 */
	guard(rcu)();

	list_for_each_ನಮೂದು_ಸುರಕ್ಷಿತ(cursor, tmp, &rq->cfsb_csd_list,
				 throttled_csd_list) {
		list_del_init(&cursor->throttled_csd_list);

		ಆದರೆ (cfs_rq_throttled(cursor))
			unthrottle_cfs_rq(cursor);
	}

	rq_clock_stop_loop_update(rq);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ __unthrottle_cfs_rq_async(ರಚನೆ cfs_rq *cfs_rq)
{
	ರಚನೆ rq *rq = rq_ದ(cfs_rq);
	bool ಮೊದಲ;

	ಆದರೆ (rq == this_rq()) {
		update_rq_clock(rq);
		unthrottle_cfs_rq(cfs_rq);
		ಹಿಂತಿರುಗಿ;
	}

	/* Already enqueued */
	ಆದರೆ (WARN_ON_ONCE(!list_ಖಾಲಿಗಳು(&cfs_rq->throttled_csd_list)))
		ಹಿಂತಿರುಗಿ;

	ಮೊದಲ = list_ಖಾಲಿಗಳು(&rq->cfsb_csd_list);
	list_ಕೂಡಿಸು_ಬಾಲ(&cfs_rq->throttled_csd_list, &rq->cfsb_csd_list);
	ಆದರೆ (ಮೊದಲ)
		smp_call_function_single_async(cpu_ದ(rq), &rq->cfsb_csd);
}

ಸ್ಥಿರ ಶೂನ್ಯ unthrottle_cfs_rq_async(ರಚನೆ cfs_rq *cfs_rq)
{
	lockdep_assert_rq_held(rq_ದ(cfs_rq));

	ಆದರೆ (WARN_ON_ONCE(!cfs_rq_throttled(cfs_rq) ||
	    cfs_rq->runtime_ಉಳಿದ <= 0))
		ಹಿಂತಿರುಗಿ;

	__unthrottle_cfs_rq_async(cfs_rq);
}

ಸ್ಥಿರ bool distribute_cfs_runtime(ರಚನೆ cfs_bandwidth *cfs_b)
{
	bool throttled = false, unthrottle_local = false;
	ಪೂರ್ಣಾಂಕ this_cpu = smp_processor_id();
	u64 runtime, ಉಳಿದ = 1;
	ರಚನೆ cfs_rq *cfs_rq;
	ರಚನೆ rq *rq;

	guard(rcu)();

	list_for_each_ನಮೂದು_rcu(cfs_rq, &cfs_b->throttled_cfs_rq,
				throttled_list) {
		rq = rq_ದ(cfs_rq);

		ಆದರೆ (!ಉಳಿದ) {
			throttled = true;
			ನಿಲ್ಲಿಸು;
		}

		guard(rq_ಬೀಗ_irqsave)(rq);

		ಆದರೆ (!cfs_rq_throttled(cfs_rq))
			ಮುಂದುವರಿ;

		/* Already queued for async unthrottle */
		ಆದರೆ (!list_ಖಾಲಿಗಳು(&cfs_rq->throttled_csd_list))
			ಮುಂದುವರಿ;

		ಆದರೆ (cfs_rq->h_curr) {
			update_rq_clock(rq);
			update_curr(cfs_rq);
		}

		/* By the above checks, this should never be true */
		WARN_ON_ONCE(cfs_rq->runtime_ಉಳಿದ > 0);

		scoped_guard(raw_spinlock, &cfs_b->ಬೀಗ) {
			runtime = -cfs_rq->runtime_ಉಳಿದ + 1;
			ಆದರೆ (runtime > cfs_b->runtime)
				runtime = cfs_b->runtime;
			cfs_b->runtime -= runtime;
			ಉಳಿದ = cfs_b->runtime;
		}

		cfs_rq->runtime_ಉಳಿದ += runtime;

		/*
		 * Ran out of bandwidth during distribution!
		 * Indicate throttled entities and break early.
		 */
		ಆದರೆ (cfs_rq->runtime_ಉಳಿದ <= 0) {
			throttled = true;
			ನಿಲ್ಲಿಸು;
		}

		/* we check whether we're throttled above */
		ಆದರೆ (cpu_ದ(rq) != this_cpu) {
			unthrottle_cfs_rq_async(cfs_rq);
			ಮುಂದುವರಿ;
		}

		/*
		 * Allow a parallel async unthrottle to unthrottle
		 * this cfs_rq too via __cfsb_csd_unthrottle().
		 * If we are first, do it ourselves at the end and
		 * save on an IPI from remote CPUs.
		 */
		unthrottle_local = list_ಖಾಲಿಗಳು(&rq->cfsb_csd_list);
		list_ಕೂಡಿಸು_ಬಾಲ(&cfs_rq->throttled_csd_list, &rq->cfsb_csd_list);
	}

	ಆದರೆ (unthrottle_local) {
		/*
		 * Protect against an IPI that is also trying to flush
		 * the unthrottled cfs_rq(s) from this CPU's csd_list.
		 */
		scoped_guard(irqsave)
			__cfsb_csd_unthrottle(cpu_rq(this_cpu));
	}

	ಹಿಂತಿರುಗಿ throttled;
}

/*
 * Responsible for refilling a task_group's bandwidth and unthrottling its
 * cfs_rqs as appropriate. If there has been no activity within the last
 * period the timer is deactivated until scheduling resumes; cfs_b->idle is
 * used to track this state.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ do_sched_cfs_period_timer(ರಚನೆ cfs_bandwidth *cfs_b, ಪೂರ್ಣಾಂಕ overrun, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ flags)
	__must_hold(&cfs_b->ಬೀಗ)
{
	ಪೂರ್ಣಾಂಕ throttled;

	/* no need to continue the timer with no bandwidth constraint */
	ಆದರೆ (cfs_b->quota == RUNTIME_INF)
		ಹೋಗು ಹೊರಹೋಗು_deactivate;

	throttled = !list_ಖಾಲಿಗಳು(&cfs_b->throttled_cfs_rq);
	cfs_b->nr_periods += overrun;

	/* Refill extra burst quota even if cfs_b->idle */
	__refill_cfs_bandwidth_runtime(cfs_b);

	/*
	 * idle depends on !throttled (for the case of a large deficit), and if
	 * we're going inactive then everything else can be deferred
	 */
	ಆದರೆ (cfs_b->idle && !throttled)
		ಹೋಗು ಹೊರಹೋಗು_deactivate;

	ಆದರೆ (!throttled) {
		/* mark as potentially idle for the upcoming period */
		cfs_b->idle = 1;
		ಹಿಂತಿರುಗಿ 0;
	}

	/* account preceding periods in which throttling occurred */
	cfs_b->nr_throttled += overrun;

	/*
	 * This check is repeated as we release cfs_b->lock while we unthrottle.
	 */
	ಆಗ (throttled && cfs_b->runtime > 0) {
		raw_spin_unlock_irqrestore(&cfs_b->ಬೀಗ, flags);
		/* we can't nest cfs_b->lock while distributing bandwidth */
		throttled = distribute_cfs_runtime(cfs_b);
		raw_spin_ಬೀಗ_irqsave(&cfs_b->ಬೀಗ, flags);
	}

	/*
	 * While we are ensured activity in the period following an
	 * unthrottle, this also covers the case in which the new bandwidth is
	 * insufficient to cover the existing bandwidth deficit.  (Forcing the
	 * timer to remain active while there are any throttled entities.)
	 */
	cfs_b->idle = 0;

	ಹಿಂತಿರುಗಿ 0;

ಹೊರಹೋಗು_deactivate:
	ಹಿಂತಿರುಗಿ 1;
}

/* a cfs_rq won't donate quota below this amount */
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ u64 min_cfs_rq_runtime = 1 * NSEC_ಪ್ರತಿ_MSEC;
/* minimum remaining period time to redistribute slack quota */
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ u64 min_bandwidth_expiration = 2 * NSEC_ಪ್ರತಿ_MSEC;
/* how long we wait to gather additional slack before distributing */
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ u64 cfs_bandwidth_slack_period = 5 * NSEC_ಪ್ರತಿ_MSEC;

/*
 * Are we near the end of the current quota period?
 *
 * Requires cfs_b->lock for hrtimer_expires_remaining to be safe against the
 * hrtimer base being cleared by hrtimer_start. In the case of
 * migrate_hrtimers, base is never cleared, so we are fine.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ runtime_refresh_within(ರಚನೆ cfs_bandwidth *cfs_b, u64 min_expire)
{
	ರಚನೆ hrtimer *refresh_timer = &cfs_b->period_timer;
	s64 ಉಳಿದ;

	/* if the call-back is running a quota refresh is already occurring */
	ಆದರೆ (hrtimer_callback_running(refresh_timer))
		ಹಿಂತಿರುಗಿ 1;

	/* is a quota refresh about to occur? */
	ಉಳಿದ = ktime_ಗಮ್ಯ_ns(hrtimer_expires_ಉಳಿದ(refresh_timer));
	ಆದರೆ (ಉಳಿದ < (s64)min_expire)
		ಹಿಂತಿರುಗಿ 1;

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ ಆರಂಭ_cfs_slack_bandwidth(ರಚನೆ cfs_bandwidth *cfs_b)
{
	u64 min_ಎಡ = cfs_bandwidth_slack_period + min_bandwidth_expiration;

	/* if there's a quota refresh soon don't bother with slack */
	ಆದರೆ (runtime_refresh_within(cfs_b, min_ಎಡ))
		ಹಿಂತಿರುಗಿ;

	/* don't push forwards an existing deferred unthrottle */
	ಆದರೆ (cfs_b->slack_started)
		ಹಿಂತಿರುಗಿ;
	cfs_b->slack_started = true;

	hrtimer_ಆರಂಭ(&cfs_b->slack_timer,
			ns_ಗಮ್ಯ_ktime(cfs_bandwidth_slack_period),
			HRTIMER_ಬಹುಲಕ_REL);
}

/* we know any runtime found here is valid as update_curr() precedes return */
ಸ್ಥಿರ ಶೂನ್ಯ __return_cfs_rq_runtime(ರಚನೆ cfs_rq *cfs_rq)
{
	ರಚನೆ cfs_bandwidth *cfs_b = tg_cfs_bandwidth(cfs_rq->tg);
	s64 slack_runtime = cfs_rq->runtime_ಉಳಿದ - min_cfs_rq_runtime;

	ಆದರೆ (slack_runtime <= 0)
		ಹಿಂತಿರುಗಿ;

	guard(raw_spinlock)(&cfs_b->ಬೀಗ);

	ಆದರೆ (cfs_b->quota != RUNTIME_INF) {
		cfs_b->runtime += slack_runtime;

		/* we are under rq->lock, defer unthrottling using a timer */
		ಆದರೆ (cfs_b->runtime > sched_cfs_bandwidth_ತುಂಡು() &&
		    !list_ಖಾಲಿಗಳು(&cfs_b->throttled_cfs_rq))
			ಆರಂಭ_cfs_slack_bandwidth(cfs_b);
	}

	/* even if it's not valid for return we don't want to try again */
	cfs_rq->runtime_ಉಳಿದ -= slack_runtime;
}

ಸ್ಥಿರ __always_inline ಶೂನ್ಯ return_cfs_rq_runtime(ರಚನೆ cfs_rq *cfs_rq)
{
	ಆದರೆ (!cfs_bandwidth_ಬಳಸಿದ())
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (!cfs_rq->runtime_enabled || cfs_rq->nr_queued)
		ಹಿಂತಿರುಗಿ;

	__return_cfs_rq_runtime(cfs_rq);
}

/*
 * This is done with a timer (instead of inline with bandwidth return) since
 * it's necessary to juggle rq->locks to unthrottle their respective cfs_rqs.
 */
ಸ್ಥಿರ ಶೂನ್ಯ do_sched_cfs_slack_timer(ರಚನೆ cfs_bandwidth *cfs_b)
{
	/* confirm we're still not at a refresh boundary */
	scoped_guard(raw_spinlock_irqsave, &cfs_b->ಬೀಗ) {
		u64 runtime = 0, ತುಂಡು = sched_cfs_bandwidth_ತುಂಡು();

		cfs_b->slack_started = false;

		ಆದರೆ (runtime_refresh_within(cfs_b, min_bandwidth_expiration))
			ಹಿಂತಿರುಗಿ;

		ಆದರೆ (cfs_b->quota != RUNTIME_INF && cfs_b->runtime > ತುಂಡು)
			runtime = cfs_b->runtime;

		ಆದರೆ (!runtime)
			ಹಿಂತಿರುಗಿ;
	}

	distribute_cfs_runtime(cfs_b);
}

/*
 * When a group wakes up we want to make sure that its quota is not already
 * expired/exceeded, otherwise it may be allowed to steal additional ticks of
 * runtime as update_curr() throttling can not trigger until it's on-rq.
 */
ಸ್ಥಿರ ಶೂನ್ಯ check_ಸರತಿಗೆ_ಸೇರಿಸು_throttle(ರಚನೆ cfs_rq *cfs_rq)
{
	ಆದರೆ (!cfs_bandwidth_ಬಳಸಿದ())
		ಹಿಂತಿರುಗಿ;

	/* an active group must be handled by the update_curr() path */
	ಆದರೆ (!cfs_rq->runtime_enabled || cfs_rq->h_curr)
		ಹಿಂತಿರುಗಿ;

	/* ensure the group is not already throttled */
	ಆದರೆ (cfs_rq_throttled(cfs_rq))
		ಹಿಂತಿರುಗಿ;

	/* update runtime allocation */
	account_cfs_rq_runtime(cfs_rq, 0);
}

ಸ್ಥಿರ ಶೂನ್ಯ sync_throttle(ರಚನೆ task_ಗುಂಪು *tg, ಪೂರ್ಣಾಂಕ cpu)
{
	ರಚನೆ cfs_rq *pcfs_rq, *cfs_rq;

	ಆದರೆ (!cfs_bandwidth_ಬಳಸಿದ())
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (!tg->ಪೋಷಕ)
		ಹಿಂತಿರುಗಿ;

	cfs_rq = tg_cfs_rq(tg, cpu);
	pcfs_rq = tg_cfs_rq(tg->ಪೋಷಕ, cpu);

	cfs_rq->throttle_ಎಣಿಕೆ = pcfs_rq->throttle_ಎಣಿಕೆ;
	cfs_rq->throttled_clock_pelt = rq_clock_pelt(cpu_rq(cpu));

	/*
	 * It is not enough to sync the "pelt_clock_throttled" indicator
	 * with the parent cfs_rq when the hierarchy is not queued.
	 * Always join a throttled hierarchy with PELT clock throttled
	 * and leaf it to the first enqueue, or distribution to
	 * unthrottle the PELT clock.
	 */
	ಆದರೆ (cfs_rq->throttle_ಎಣಿಕೆ)
		cfs_rq->pelt_clock_throttled = 1;
}

ಸ್ಥಿರ ಎಣಿಕೆ hrtimer_restart sched_cfs_slack_timer(ರಚನೆ hrtimer *timer)
{
	ರಚನೆ cfs_bandwidth *cfs_b =
		container_ದ(timer, ರಚನೆ cfs_bandwidth, slack_timer);

	do_sched_cfs_slack_timer(cfs_b);

	ಹಿಂತಿರುಗಿ HRTIMER_NORESTART;
}

ಸ್ಥಿರ ಎಣಿಕೆ hrtimer_restart sched_cfs_period_timer(ರಚನೆ hrtimer *timer)
{
	ರಚನೆ cfs_bandwidth *cfs_b =
		container_ದ(timer, ರಚನೆ cfs_bandwidth, period_timer);
	ಪೂರ್ಣಾಂಕ overrun;
	ಪೂರ್ಣಾಂಕ idle = 0;
	ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = 0;

	CLASS(raw_spinlock_irqsave, cfsb_guard)(&cfs_b->ಬೀಗ);

	ಗಾಗಿ (;;) {
		overrun = hrtimer_forward_now(timer, cfs_b->period);
		ಆದರೆ (!overrun)
			ನಿಲ್ಲಿಸು;

		idle = do_sched_cfs_period_timer(cfs_b, overrun, cfsb_guard.flags);

		ಆದರೆ (++ಎಣಿಕೆ_2 > 3) {
			u64 new, old = ktime_ಗಮ್ಯ_ns(cfs_b->period);

			/*
			 * Grow period by a factor of 2 to avoid losing precision.
			 * Precision loss in the quota/period ratio can cause __cfs_schedulable
			 * to fail.
			 */
			new = old * 2;
			ಆದರೆ (new < max_bw_quota_period_us * NSEC_ಪ್ರತಿ_USEC) {
				cfs_b->period = ns_ಗಮ್ಯ_ktime(new);
				cfs_b->quota *= 2;
				cfs_b->ಅವಧಿ *= 2;

				pr_warn_ratelimited(
	"cfs_period_timer[cpu%d]: period too short, scaling up (new cfs_period_us = %lld, cfs_quota_us = %lld)\n",
					smp_processor_id(),
					div_u64(new, NSEC_ಪ್ರತಿ_USEC),
					div_u64(cfs_b->quota, NSEC_ಪ್ರತಿ_USEC));
			} ಇಲ್ಲದಿದ್ದರೆ {
				pr_warn_ratelimited(
	"cfs_period_timer[cpu%d]: period too short, but cannot scale up without losing precision (cfs_period_us = %lld, cfs_quota_us = %lld)\n",
					smp_processor_id(),
					div_u64(old, NSEC_ಪ್ರತಿ_USEC),
					div_u64(cfs_b->quota, NSEC_ಪ್ರತಿ_USEC));
			}

			/* reset count so we don't come right back in here */
			ಎಣಿಕೆ_2 = 0;
		}
	}

	ಆದರೆ (idle) {
		cfs_b->period_ಸಕ್ರಿಯ = 0;
		ಹಿಂತಿರುಗಿ HRTIMER_NORESTART;
	}

	ಹಿಂತಿರುಗಿ HRTIMER_RESTART;
}

ಶೂನ್ಯ init_cfs_bandwidth(ರಚನೆ cfs_bandwidth *cfs_b, ರಚನೆ cfs_bandwidth *ಪೋಷಕ)
{
	raw_spin_ಬೀಗ_init(&cfs_b->ಬೀಗ);
	cfs_b->runtime = 0;
	cfs_b->quota = RUNTIME_INF;
	cfs_b->period = us_ಗಮ್ಯ_ktime(default_bw_period_us());
	cfs_b->ಅವಧಿ = 0;
	cfs_b->ಶ್ರೇಣೀಕೃತ_quota = ಪೋಷಕ ? ಪೋಷಕ->ಶ್ರೇಣೀಕೃತ_quota : RUNTIME_INF;

	INIT_LIST_ತಲೆ(&cfs_b->throttled_cfs_rq);
	hrtimer_ಸಿದ್ಧತೆ(&cfs_b->period_timer, sched_cfs_period_timer, CLOCK_MONOTONIC,
		      HRTIMER_ಬಹುಲಕ_ABS_PINNED);

	/* Add a random offset so that timers interleave */
	hrtimer_ಗಣ_expires(&cfs_b->period_timer,
			    get_random_u32_ಕೆಳಗೆ(cfs_b->period));
	hrtimer_ಸಿದ್ಧತೆ(&cfs_b->slack_timer, sched_cfs_slack_timer, CLOCK_MONOTONIC,
		      HRTIMER_ಬಹುಲಕ_REL);
	cfs_b->slack_started = false;
}

ಸ್ಥಿರ ಶೂನ್ಯ init_cfs_rq_runtime(ರಚನೆ cfs_rq *cfs_rq)
{
	cfs_rq->runtime_enabled = 0;
	INIT_LIST_ತಲೆ(&cfs_rq->throttled_list);
	INIT_LIST_ತಲೆ(&cfs_rq->throttled_csd_list);
	INIT_LIST_ತಲೆ(&cfs_rq->throttled_limbo_list);
}

ಶೂನ್ಯ ಆರಂಭ_cfs_bandwidth(ರಚನೆ cfs_bandwidth *cfs_b)
{
	lockdep_assert_held(&cfs_b->ಬೀಗ);

	ಆದರೆ (cfs_b->period_ಸಕ್ರಿಯ)
		ಹಿಂತಿರುಗಿ;

	cfs_b->period_ಸಕ್ರಿಯ = 1;
	hrtimer_forward_now(&cfs_b->period_timer, cfs_b->period);
	hrtimer_ಆರಂಭ_expires(&cfs_b->period_timer, HRTIMER_ಬಹುಲಕ_ABS_PINNED);
}

ಸ್ಥಿರ ಶೂನ್ಯ destroy_cfs_bandwidth(ರಚನೆ cfs_bandwidth *cfs_b)
{
	ಪೂರ್ಣಾಂಕ __maybe_unused i;

	/* init_cfs_bandwidth() was not called */
	ಆದರೆ (!cfs_b->throttled_cfs_rq.ಮುಂದಿನ)
		ಹಿಂತಿರುಗಿ;

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
		ರಚನೆ rq *rq = cpu_rq(i);

		ಆದರೆ (list_ಖಾಲಿಗಳು(&rq->cfsb_csd_list))
			ಮುಂದುವರಿ;

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
ಸ್ಥಿರ ಶೂನ್ಯ __maybe_unused update_runtime_enabled(ರಚನೆ rq *rq)
{
	ರಚನೆ task_ಗುಂಪು *tg;

	lockdep_assert_rq_held(rq);

	guard(rcu)();

	list_for_each_ನಮೂದು_rcu(tg, &task_groups, list) {
		ರಚನೆ cfs_bandwidth *cfs_b = &tg->cfs_bandwidth;
		ರಚನೆ cfs_rq *cfs_rq = tg_cfs_rq(tg, cpu_ದ(rq));

		scoped_guard(raw_spinlock, &cfs_b->ಬೀಗ)
			cfs_rq->runtime_enabled = cfs_b->quota != RUNTIME_INF;
	}
}

/* cpu offline callback */
ಸ್ಥಿರ ಶೂನ್ಯ __maybe_unused unthrottle_offline_cfs_rqs(ರಚನೆ rq *rq)
{
	ರಚನೆ task_ಗುಂಪು *tg;

	lockdep_assert_rq_held(rq);

	// Do not unthrottle for an active CPU
	ಆದರೆ (cpumask_test_cpu(cpu_ದ(rq), cpu_ಸಕ್ರಿಯ_ಮುಖವಾಡ))
		ಹಿಂತಿರುಗಿ;

	/*
	 * The rq clock has already been updated in the
	 * set_rq_offline(), so we should skip updating
	 * the rq clock again in unthrottle_cfs_rq().
	 */
	rq_clock_ಆರಂಭ_loop_update(rq);

	guard(rcu)();

	list_for_each_ನಮೂದು_rcu(tg, &task_groups, list) {
		ರಚನೆ cfs_rq *cfs_rq = tg_cfs_rq(tg, cpu_ದ(rq));

		ಆದರೆ (!cfs_rq->runtime_enabled)
			ಮುಂದುವರಿ;

		/*
		 * Offline rq is schedulable till CPU is completely disabled
		 * in take_cpu_down(), so we prevent new cfs throttling here.
		 */
		cfs_rq->runtime_enabled = 0;

		ಆದರೆ (!cfs_rq_throttled(cfs_rq))
			ಮುಂದುವರಿ;

		/*
		 * clock_task is not advancing so we just need to make sure
		 * there's some valid quota amount
		 */
		cfs_rq->runtime_ಉಳಿದ = 1;
		unthrottle_cfs_rq(cfs_rq);
	}

	rq_clock_stop_loop_update(rq);
}

bool cfs_task_bw_constrained(ರಚನೆ task_struct *p)
{
	ರಚನೆ cfs_rq *cfs_rq = task_cfs_rq(p);

	ಆದರೆ (!cfs_bandwidth_ಬಳಸಿದ())
		ಹಿಂತಿರುಗಿ false;

	ಆದರೆ (cfs_rq->runtime_enabled ||
	    tg_cfs_bandwidth(cfs_rq->tg)->ಶ್ರೇಣೀಕೃತ_quota != RUNTIME_INF)
		ಹಿಂತಿರುಗಿ true;

	ಹಿಂತಿರುಗಿ false;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_NO_HZ_ತುಂಬಿದ
/* called from pick_next_task_fair() */
ಸ್ಥಿರ ಶೂನ್ಯ sched_fair_update_stop_tick(ರಚನೆ rq *rq, ರಚನೆ task_struct *p)
{
	ಪೂರ್ಣಾಂಕ cpu = cpu_ದ(rq);

	ಆದರೆ (!cfs_bandwidth_ಬಳಸಿದ())
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (!tick_nohz_ತುಂಬಿದ_cpu(cpu))
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (rq->nr_running != 1)
		ಹಿಂತಿರುಗಿ;

	/*
	 *  We know there is only one task runnable and we've just picked it. The
	 *  normal enqueue path will have cleared TICK_DEP_BIT_SCHED if we will
	 *  be otherwise able to stop the tick. Just need to check if we are using
	 *  bandwidth control.
	 */
	ಆದರೆ (cfs_task_bw_constrained(p))
		tick_nohz_dep_ಗಣ_cpu(cpu, TICK_DEP_ಬಿಟ್_SCHED);
}
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_NO_HZ_FULL */

#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_CFS_BANDWIDTH: */

ಸ್ಥಿರ bool account_cfs_rq_runtime(ರಚನೆ cfs_rq *cfs_rq, u64 delta_exec) { ಹಿಂತಿರುಗಿ false; }
ಸ್ಥಿರ ಶೂನ್ಯ check_ಸರತಿಗೆ_ಸೇರಿಸು_throttle(ರಚನೆ cfs_rq *cfs_rq) {}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ sync_throttle(ರಚನೆ task_ಗುಂಪು *tg, ಪೂರ್ಣಾಂಕ cpu) {}
ಸ್ಥಿರ __always_inline ಶೂನ್ಯ return_cfs_rq_runtime(ರಚನೆ cfs_rq *cfs_rq) {}
ಸ್ಥಿರ ಶೂನ್ಯ task_throttle_ಸಿದ್ಧತೆ_work(ರಚನೆ task_struct *p) {}
ಸ್ಥಿರ bool task_is_throttled(ರಚನೆ task_struct *p) { ಹಿಂತಿರುಗಿ false; }
ಸ್ಥಿರ ಶೂನ್ಯ ಸರತಿಯಿಂದ_ತೆಗೆ_throttled_task(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ flags) {}
ಸ್ಥಿರ bool ಸರತಿಗೆ_ಸೇರಿಸು_throttled_task(ರಚನೆ task_struct *p) { ಹಿಂತಿರುಗಿ false; }
ಸ್ಥಿರ ಶೂನ್ಯ ದಾಖಲೆ_throttle_clock(ರಚನೆ cfs_rq *cfs_rq) {}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ cfs_rq_throttled(ರಚನೆ cfs_rq *cfs_rq)
{
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool cfs_rq_pelt_clock_throttled(ರಚನೆ cfs_rq *cfs_rq)
{
	ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ throttled_hierarchy(ರಚನೆ cfs_rq *cfs_rq)
{
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ lb_throttled_hierarchy(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ dst_cpu)
{
	ಹಿಂತಿರುಗಿ 0;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_FAIR_ಗುಂಪು_SCHED
ಶೂನ್ಯ init_cfs_bandwidth(ರಚನೆ cfs_bandwidth *cfs_b, ರಚನೆ cfs_bandwidth *ಪೋಷಕ) {}
ಸ್ಥಿರ ಶೂನ್ಯ init_cfs_rq_runtime(ರಚನೆ cfs_rq *cfs_rq) {}
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ರಚನೆ cfs_bandwidth *tg_cfs_bandwidth(ರಚನೆ task_ಗುಂಪು *tg)
{
	ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ destroy_cfs_bandwidth(ರಚನೆ cfs_bandwidth *cfs_b) {}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ update_runtime_enabled(ರಚನೆ rq *rq) {}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ unthrottle_offline_cfs_rqs(ರಚನೆ rq *rq) {}
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_CGROUP_SCHED
bool cfs_task_bw_constrained(ರಚನೆ task_struct *p)
{
	ಹಿಂತಿರುಗಿ false;
}
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_CFS_BANDWIDTH */

#ಆದರೆ !defined(CONFIG_CFS_BANDWIDTH) || !defined(CONFIG_NO_HZ_ತುಂಬಿದ)
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ sched_fair_update_stop_tick(ರಚನೆ rq *rq, ರಚನೆ task_struct *p) {}
#ಆದರೆ_ಅಂತ್ಯ

/**************************************************
 * CFS operations on tasks:
 */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_HRTICK
ಸ್ಥಿರ ಶೂನ್ಯ hrtick_ಆರಂಭ_fair(ರಚನೆ rq *rq, ರಚನೆ task_struct *p)
{
	ರಚನೆ sched_entity *se = &p->se;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ scale = 1024;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ util = 0;
	u64 vdelta;
	u64 delta;

	WARN_ON_ONCE(task_rq(p) != rq);

	ಆದರೆ (rq->cfs.h_nr_queued <= 1)
		ಹಿಂತಿರುಗಿ;

	/*
	 * Compute time until virtual deadline
	 */
	vdelta = se->deadline - se->vruntime;
	ಆದರೆ ((s64)vdelta < 0) {
		ಆದರೆ (task_current_donor(rq, p))
			resched_curr(rq);
		ಹಿಂತಿರುಗಿ;
	}
	delta = (se->h_ತುಂಬು.ತೂಕ * vdelta) / NICE_0_ತುಂಬು;

	/*
	 * Correct for instantaneous load of other classes.
	 */
	util += cpu_util_irq(rq);
	ಆದರೆ (util && util < 1024) {
		scale *= 1024;
		scale /= (1024 - util);
	}

	hrtick_ಆರಂಭ(rq, (scale * delta) / 1024);
}

/*
 * Called on enqueue to start the hrtick when h_nr_queued becomes more than 1.
 */
ಸ್ಥಿರ ಶೂನ್ಯ hrtick_update(ರಚನೆ rq *rq)
{
	ರಚನೆ task_struct *donor = rq->donor;

	ಆದರೆ (!hrtick_enabled_fair(rq) || donor->sched_class != &fair_sched_class)
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (hrtick_ಸಕ್ರಿಯ(rq))
		ಹಿಂತಿರುಗಿ;

	hrtick_ಆರಂಭ_fair(rq, donor);
}
#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_SCHED_HRTICK: */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
hrtick_ಆರಂಭ_fair(ರಚನೆ rq *rq, ರಚನೆ task_struct *p)
{
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ hrtick_update(ರಚನೆ rq *rq)
{
}
#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_SCHED_HRTICK */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool cpu_overutilized(ಪೂರ್ಣಾಂಕ cpu)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ rq_util_max;

	ಆದರೆ (!sched_energy_enabled())
		ಹಿಂತಿರುಗಿ false;

	rq_util_max = uclamp_rq_get(cpu_rq(cpu), UCLAMP_MAX);

	/* Return true only if the utilization doesn't fit CPU's capacity */
	ಹಿಂತಿರುಗಿ !util_ಹೊಂದುತ್ತದೆ_cpu(cpu_util_cfs(cpu), 0, rq_util_max, cpu);
}

/*
 * overutilized value make sense only if EAS is enabled
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool is_rd_overutilized(ರಚನೆ ಬೇರು_domain *rd)
{
	ಹಿಂತಿರುಗಿ !sched_energy_enabled() || READ_ONCE(rd->overutilized);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ ಗಣ_rd_overutilized(ರಚನೆ ಬೇರು_domain *rd, bool flag)
{
	ಆದರೆ (!sched_energy_enabled())
		ಹಿಂತಿರುಗಿ;

	WRITE_ONCE(rd->overutilized, flag);
	trace_sched_overutilized_tp(rd, flag);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ check_update_overutilized_ಸ್ಥಿತಿ(ರಚನೆ rq *rq)
{
	/*
	 * overutilized field is used for load balancing decisions only
	 * if energy aware scheduler is being used
	 */

	ಆದರೆ (!is_rd_overutilized(rq->rd) && cpu_overutilized(rq->cpu))
		ಗಣ_rd_overutilized(rq->rd, 1);
}

/* Runqueue only has SCHED_IDLE tasks enqueued */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sched_idle_rq(ರಚನೆ rq *rq)
{
	ಹಿಂತಿರುಗಿ unlikely(rq->nr_running == rq->cfs.h_nr_idle &&
			rq->nr_running);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಆರಿಸು_sched_idle_rq(ರಚನೆ rq *rq, ರಚನೆ task_struct *p)
{
	ಹಿಂತಿರುಗಿ sched_idle_rq(rq) && !task_has_idle_policy(p);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಆರಿಸು_idle_cpu(ಪೂರ್ಣಾಂಕ cpu, ರಚನೆ task_struct *p)
{
	ಹಿಂತಿರುಗಿ ಲಭ್ಯ_idle_cpu(cpu) ||
	       ಆರಿಸು_sched_idle_rq(cpu_rq(cpu), p);
}

ಸ್ಥಿರ ಶೂನ್ಯ
requeue_delayed_entity(ರಚನೆ cfs_rq *cfs_rq, ರಚನೆ sched_entity *se)
{
	/*
	 * se->sched_delayed should imply: se->on_rq == 1.
	 * Because a delayed entity is one that is still on
	 * the runqueue competing until elegibility.
	 */
	WARN_ON_ONCE(!se->sched_delayed);
	WARN_ON_ONCE(!se->on_rq);

	ಆದರೆ (update_entity_lag(cfs_rq, se)) {
		cfs_rq->h_nr_queued--;
		ಆದರೆ (se != cfs_rq->curr)
			__dequeue_entity(cfs_rq, se);
		ಇರಿಸು_entity(cfs_rq, se, 0);
		ಆದರೆ (se != cfs_rq->curr)
			__enqueue_entity(cfs_rq, se);
		cfs_rq->h_nr_queued++;
	}

	update_ತುಂಬು_avg(cfs_rq, se, 0);
	clear_delayed(se);
}

ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಸರತಿಗೆ_ಸೇರಿಸು_hierarchy(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ flags)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ತೂಕ = NICE_0_ತುಂಬು;
	ಪೂರ್ಣಾಂಕ task_new = !(flags & ಸರತಿಗೆ_ಸೇರಿಸು_WAKEUP);
	ರಚನೆ sched_entity *se = &p->se;
	ಪೂರ್ಣಾಂಕ h_nr_idle = task_has_idle_policy(p);
	ಪೂರ್ಣಾಂಕ h_nr_runnable = 1;

	ಆದರೆ (task_new && se->sched_delayed)
		h_nr_runnable = 0;

	for_each_sched_entity(se) {
		ರಚನೆ cfs_rq *cfs_rq = cfs_rq_ದ(se);

		update_curr(cfs_rq);

		ಆದರೆ (!se->on_rq) {
			ಸರತಿಗೆ_ಸೇರಿಸು_entity(cfs_rq, se, flags);
		} ಇಲ್ಲದಿದ್ದರೆ {
			update_ತುಂಬು_avg(cfs_rq, se, UPDATE_TG);
			se_update_runnable(se);
			update_cfs_ಗುಂಪು(se);
		}

		cfs_rq->h_nr_runnable += h_nr_runnable;
		cfs_rq->h_nr_queued++;
		cfs_rq->h_nr_idle += h_nr_idle;

		ಆದರೆ (cfs_rq_is_idle(cfs_rq))
			h_nr_idle = 1;

		ತೂಕ = __calc_prop_weight(cfs_rq, se, ತೂಕ);

		flags = ಸರತಿಗೆ_ಸೇರಿಸು_WAKEUP;
	}

	ಹಿಂತಿರುಗಿ ತೂಕ;
}

/* Update curr's vruntime before placing entity or updating lag */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ update_curr_eevdf(ರಚನೆ cfs_rq *cfs_rq)
{
	ಆದರೆ (!cfs_rq->curr)
		ಹಿಂತಿರುಗಿ;

	update_curr(cfs_rq_ದ(cfs_rq->curr));
}

/*
 * The enqueue_task method is called before nr_running is
 * increased. Here we update the fair scheduling stats and
 * then put the task into the rbtree:
 */
ಸ್ಥಿರ ಶೂನ್ಯ
ಸರತಿಗೆ_ಸೇರಿಸು_task_fair(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ flags)
{
	ಪೂರ್ಣಾಂಕ rq_h_nr_queued = rq->cfs.h_nr_queued;
	ಪೂರ್ಣಾಂಕ task_new = !(flags & ಸರತಿಗೆ_ಸೇರಿಸು_WAKEUP);
	ರಚನೆ sched_entity *se = &p->se;
	ರಚನೆ cfs_rq *cfs_rq = &rq->cfs;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ತೂಕ;
	bool curr;

	ಆದರೆ (task_is_throttled(p) && ಸರತಿಗೆ_ಸೇರಿಸು_throttled_task(p))
		ಹಿಂತಿರುಗಿ;

	/*
	 * The code below (indirectly) updates schedutil which looks at
	 * the cfs_rq utilization to select a frequency.
	 * Let's add the task's estimated utilization to the cfs_rq's
	 * estimated utilization, before we update schedutil.
	 */
	ಆದರೆ (!p->se.sched_delayed || (flags & ಸರತಿಗೆ_ಸೇರಿಸು_DELAYED))
		util_est_ಸರತಿಗೆ_ಸೇರಿಸು(cfs_rq, p);

	update_curr_eevdf(cfs_rq);

	ಆದರೆ (flags & ಸರತಿಗೆ_ಸೇರಿಸು_DELAYED) {
		requeue_delayed_entity(cfs_rq, se);
		ಹಿಂತಿರುಗಿ;
	}

	/*
	 * If in_iowait is set, the code below may not trigger any cpufreq
	 * utilization updates, so do it here explicitly with the IOWAIT flag
	 * passed.
	 */
	ಆದರೆ (p->ಒಳ_iowait)
		cpufreq_update_util(rq, SCHED_CPUFREQ_IOWAIT);

	curr = (cfs_rq->curr == se);
	/*
	 * This can actually happen with CFS_BANDWIDTH, a task can be throttled
	 * and dequeued, unthrottled from another CPU and then enqueued again
	 * before schedule().
	 */
	ಆದರೆ (curr)
		ಇರಿಸು_entity(cfs_rq, se, flags);

	ಆದರೆ (se->on_rq && se->sched_delayed)
		requeue_delayed_entity(cfs_rq, se);

	ತೂಕ = ಸರತಿಗೆ_ಸೇರಿಸು_hierarchy(p, flags);

	ಆದರೆ (!curr) {
		reweight_eevdf(cfs_rq, se, ತೂಕ, false);
		ಇರಿಸು_entity(cfs_rq, se, flags | ಸರತಿಗೆ_ಸೇರಿಸು_QUEUED);
		__enqueue_entity(cfs_rq, se);
	}

	ಆದರೆ (!rq_h_nr_queued && rq->cfs.h_nr_queued)
		dl_server_ಆರಂಭ(&rq->fair_server);

	/* At this point se is NULL and we are at root level*/
	ಕೂಡಿಸು_nr_running(rq, 1);

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
	ಆದರೆ (!task_new)
		check_update_overutilized_ಸ್ಥಿತಿ(rq);

	assert_list_ಎಲೆ_cfs_rq(rq);

	hrtick_update(rq);
}

ಸ್ಥಿರ ಶೂನ್ಯ ಸರತಿಯಿಂದ_ತೆಗೆ_hierarchy(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ flags)
{
	ರಚನೆ sched_entity *se = &p->se;
	bool task_sleep = flags & ಸರತಿಯಿಂದ_ತೆಗೆ_SLEEP;
	bool task_delayed = flags & ಸರತಿಯಿಂದ_ತೆಗೆ_DELAYED;
	bool task_throttled = flags & ಸರತಿಯಿಂದ_ತೆಗೆ_THROTTLE;
	ಪೂರ್ಣಾಂಕ h_nr_runnable = 0;
	ಪೂರ್ಣಾಂಕ h_nr_idle = task_has_idle_policy(p);
	bool ಸರತಿಯಿಂದ_ತೆಗೆ = true;

	ಆದರೆ (task_sleep || task_delayed || !se->sched_delayed)
		h_nr_runnable = 1;

	for_each_sched_entity(se) {
		ರಚನೆ cfs_rq *cfs_rq = cfs_rq_ದ(se);

		update_curr(cfs_rq);

		ಆದರೆ (ಸರತಿಯಿಂದ_ತೆಗೆ) {
			ಸರತಿಯಿಂದ_ತೆಗೆ_entity(cfs_rq, se, flags);
			/* Don't dequeue parent if it has other entities besides us */
			ಆದರೆ (cfs_rq->ತುಂಬು.ತೂಕ)
				ಸರತಿಯಿಂದ_ತೆಗೆ = false;
		} ಇಲ್ಲದಿದ್ದರೆ {
			update_ತುಂಬು_avg(cfs_rq, se, UPDATE_TG);
			se_update_runnable(se);
			update_cfs_ಗುಂಪು(se);
		}

		cfs_rq->h_nr_runnable -= h_nr_runnable;
		cfs_rq->h_nr_queued--;
		cfs_rq->h_nr_idle -= h_nr_idle;

		ಆದರೆ (cfs_rq_is_idle(cfs_rq))
			h_nr_idle = 1;

		ಆದರೆ (throttled_hierarchy(cfs_rq) && task_throttled)
			ದಾಖಲೆ_throttle_clock(cfs_rq);

		flags |= ಸರತಿಯಿಂದ_ತೆಗೆ_SLEEP;
		flags &= ~(ಸರತಿಯಿಂದ_ತೆಗೆ_DELAYED | ಸರತಿಯಿಂದ_ತೆಗೆ_SPECIAL);
	}
}

/*
 * The part of dequeue_task_fair() that is needed to dequeue delayed tasks.
 *
 * Returns:
 *   true  - dequeued
 *   false - delayed
 */
ಸ್ಥಿರ bool __dequeue_task(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ flags)
{
	ರಚನೆ sched_entity *se = &p->se;
	ರಚನೆ cfs_rq *cfs_rq = &rq->cfs;
	bool was_sched_idle = sched_idle_rq(rq);
	bool task_sleep = flags & ಸರತಿಯಿಂದ_ತೆಗೆ_SLEEP;
	bool task_delayed = flags & ಸರತಿಯಿಂದ_ತೆಗೆ_DELAYED;

	clear_buddies(cfs_rq, se);

	update_curr_eevdf(cfs_rq);
	update_entity_lag(cfs_rq, se);

	ಆದರೆ (flags & ಸರತಿಯಿಂದ_ತೆಗೆ_DELAYED) {
		WARN_ON_ONCE(!se->sched_delayed);
	} ಇಲ್ಲದಿದ್ದರೆ {
		bool delay = task_sleep;
		/*
		 * DELAY_DEQUEUE relies on spurious wakeups, special task
		 * states must not suffer spurious wakeups, excempt them.
		 */
		ಆದರೆ (flags & (ಸರತಿಯಿಂದ_ತೆಗೆ_SPECIAL | ಸರತಿಯಿಂದ_ತೆಗೆ_THROTTLE))
			delay = false;

		WARN_ON_ONCE(delay && se->sched_delayed);

		ಆದರೆ (sched_feat(DELAY_ಸರತಿಯಿಂದ_ತೆಗೆ) && delay &&
		    !entity_eligible(cfs_rq, se)) {
			update_ತುಂಬು_avg(cfs_rq_ದ(se), se, UPDATE_UTIL_EST);
			ಗಣ_delayed(se);
			ಹಿಂತಿರುಗಿ false;
		}
	}

	ಸರತಿಯಿಂದ_ತೆಗೆ_hierarchy(p, flags);

	ಆದರೆ (sched_feat(ಇರಿಸು_REL_DEADLINE) && !task_sleep) {
		se->deadline -= se->vruntime;
		se->rel_deadline = 1;
	}
	ಆದರೆ (se != cfs_rq->curr)
		__dequeue_entity(cfs_rq, se);

	sub_nr_running(rq, 1);

	/* balance early to pull high priority tasks */
	ಆದರೆ (unlikely(!was_sched_idle && sched_idle_rq(rq)))
		rq->ಮುಂದಿನ_balance = jiffies;

	ಆದರೆ (task_delayed) {
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

	ಹಿಂತಿರುಗಿ true;
}

/*
 * The dequeue_task method is called before nr_running is
 * decreased. We remove the task from the rbtree and
 * update the fair scheduling stats:
 */
ಸ್ಥಿರ bool ಸರತಿಯಿಂದ_ತೆಗೆ_task_fair(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ flags)
{
	ಆದರೆ (task_is_throttled(p)) {
		ಸರತಿಯಿಂದ_ತೆಗೆ_throttled_task(p, flags);
		ಹಿಂತಿರುಗಿ true;
	}

	ಆದರೆ (!p->se.sched_delayed)
		util_est_ಸರತಿಯಿಂದ_ತೆಗೆ(&rq->cfs, p);

	ಆದರೆ (!__dequeue_task(rq, p, flags))
		ಹಿಂತಿರುಗಿ false;

	/*
	 * Must not reference @p after __dequeue_task(DEQUEUE_DELAYED).
	 */
	ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cfs_h_nr_delayed(ರಚನೆ rq *rq)
{
	ಹಿಂತಿರುಗಿ (rq->cfs.h_nr_queued - rq->cfs.h_nr_runnable);
}

/* Working cpumask for: sched_balance_rq(), sched_balance_newidle(). */
ಸ್ಥಿರ DEFINE_ಪ್ರತಿ_CPU(cpumask_var_t, ತುಂಬು_balance_ಮುಖವಾಡ);
ಸ್ಥಿರ DEFINE_ಪ್ರತಿ_CPU(cpumask_var_t, select_rq_ಮುಖವಾಡ);
ಸ್ಥಿರ DEFINE_ಪ್ರತಿ_CPU(cpumask_var_t, should_we_balance_tmpmask);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_NO_HZ_COMMON

ಸ್ಥಿರ ರಚನೆ {
	cpumask_var_t idle_cpus_ಮುಖವಾಡ;
	ಪೂರ್ಣಾಂಕ has_blocked_ತುಂಬು;		/* Idle CPUS has blocked load */
	ಪೂರ್ಣಾಂಕ needs_update;		/* Newly idle CPUs need their next_balance collated */
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಮುಂದಿನ_balance;     /* in jiffy units */
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಮುಂದಿನ_blocked;	/* Next update of blocked load in jiffies */
} nohz ____cacheline_aligned;

#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_NO_HZ_COMMON */

ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ cpu_ತುಂಬು(ರಚನೆ rq *rq)
{
	ಹಿಂತಿರುಗಿ cfs_rq_ತುಂಬು_avg(&rq->cfs);
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
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ cpu_ತುಂಬು_without(ರಚನೆ rq *rq, ರಚನೆ task_struct *p)
{
	ರಚನೆ cfs_rq *cfs_rq;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ತುಂಬು;

	/* Task has no contribution or is new */
	ಆದರೆ (cpu_ದ(rq) != task_cpu(p) || !READ_ONCE(p->se.avg.ಕೊನೆಯ_update_ಸಮಯ))
		ಹಿಂತಿರುಗಿ cpu_ತುಂಬು(rq);

	cfs_rq = &rq->cfs;
	ತುಂಬು = READ_ONCE(cfs_rq->avg.ತುಂಬು_avg);

	/* Discount task's util from CPU's util */
	lsub_ಧನಾತ್ಮಕ(&ತುಂಬು, task_h_ತುಂಬು(p));

	ಹಿಂತಿರುಗಿ ತುಂಬು;
}

ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ cpu_runnable(ರಚನೆ rq *rq)
{
	ಹಿಂತಿರುಗಿ cfs_rq_runnable_avg(&rq->cfs);
}

ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ cpu_runnable_without(ರಚನೆ rq *rq, ರಚನೆ task_struct *p)
{
	ರಚನೆ cfs_rq *cfs_rq;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ runnable;

	/* Task has no contribution or is new */
	ಆದರೆ (cpu_ದ(rq) != task_cpu(p) || !READ_ONCE(p->se.avg.ಕೊನೆಯ_update_ಸಮಯ))
		ಹಿಂತಿರುಗಿ cpu_runnable(rq);

	cfs_rq = &rq->cfs;
	runnable = READ_ONCE(cfs_rq->avg.runnable_avg);

	/* Discount task's runnable from CPU's runnable */
	lsub_ಧನಾತ್ಮಕ(&runnable, p->se.avg.runnable_avg);

	ಹಿಂತಿರುಗಿ runnable;
}

ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಸಾಮರ್ಥ್ಯ_ದ(ಪೂರ್ಣಾಂಕ cpu)
{
	ಹಿಂತಿರುಗಿ cpu_rq(cpu)->cpu_ಸಾಮರ್ಥ್ಯ;
}

ಸ್ಥಿರ ಶೂನ್ಯ ದಾಖಲೆ_wakee(ರಚನೆ task_struct *p)
{
	/*
	 * Only decay a single time; tasks that have less then 1 wakeup per
	 * jiffy will not have built up many flips.
	 */
	ಆದರೆ (ಸಮಯ_after(jiffies, current->wakee_flip_decay_ts + HZ)) {
		current->wakee_flips >>= 1;
		current->wakee_flip_decay_ts = jiffies;
	}

	ಆದರೆ (current->ಕೊನೆಯ_wakee != p) {
		current->ಕೊನೆಯ_wakee = p;
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
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ wake_wide(ರಚನೆ task_struct *p)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ master = current->wakee_flips;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ slave = p->wakee_flips;
	ಪೂರ್ಣಾಂಕ ಗುಣಕ = __this_cpu_read(sd_llc_ಗಾತ್ರ);

	ಆದರೆ (master < slave)
		ಅದಲುಬದಲು(master, slave);
	ಆದರೆ (slave < ಗುಣಕ || master < slave * ಗುಣಕ)
		ಹಿಂತಿರುಗಿ 0;
	ಹಿಂತಿರುಗಿ 1;
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
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
wake_affine_idle(ಪೂರ್ಣಾಂಕ this_cpu, ಪೂರ್ಣಾಂಕ prev_cpu, ಪೂರ್ಣಾಂಕ sync)
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
	ಆದರೆ (ಲಭ್ಯ_idle_cpu(this_cpu) && cpus_share_cache(this_cpu, prev_cpu))
		ಹಿಂತಿರುಗಿ ಲಭ್ಯ_idle_cpu(prev_cpu) ? prev_cpu : this_cpu;

	ಆದರೆ (sync) {
		ರಚನೆ rq *rq = cpu_rq(this_cpu);

		ಆದರೆ ((rq->nr_running - cfs_h_nr_delayed(rq)) == 1)
			ಹಿಂತಿರುಗಿ this_cpu;
	}

	ಆದರೆ (ಲಭ್ಯ_idle_cpu(prev_cpu))
		ಹಿಂತಿರುಗಿ prev_cpu;

	ಹಿಂತಿರುಗಿ nr_cpumask_ಬಿಟ್ಟುಗಳು;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
wake_affine_ತೂಕ(ರಚನೆ sched_domain *sd, ರಚನೆ task_struct *p,
		   ಪೂರ್ಣಾಂಕ this_cpu, ಪೂರ್ಣಾಂಕ prev_cpu, ಪೂರ್ಣಾಂಕ sync)
{
	s64 this_eff_ತುಂಬು, prev_eff_ತುಂಬು;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ task_ತುಂಬು;

	this_eff_ತುಂಬು = cpu_ತುಂಬು(cpu_rq(this_cpu));

	ಆದರೆ (sync) {
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ current_ತುಂಬು = task_h_ತುಂಬು(current);

		ಆದರೆ (current_ತುಂಬು > this_eff_ತುಂಬು)
			ಹಿಂತಿರುಗಿ this_cpu;

		this_eff_ತುಂಬು -= current_ತುಂಬು;
	}

	task_ತುಂಬು = task_h_ತುಂಬು(p);

	this_eff_ತುಂಬು += task_ತುಂಬು;
	ಆದರೆ (sched_feat(WA_BIAS))
		this_eff_ತುಂಬು *= 100;
	this_eff_ತುಂಬು *= ಸಾಮರ್ಥ್ಯ_ದ(prev_cpu);

	prev_eff_ತುಂಬು = cpu_ತುಂಬು(cpu_rq(prev_cpu));
	prev_eff_ತುಂಬು -= task_ತುಂಬು;
	ಆದರೆ (sched_feat(WA_BIAS))
		prev_eff_ತುಂಬು *= 100 + (sd->imbalance_pct - 100) / 2;
	prev_eff_ತುಂಬು *= ಸಾಮರ್ಥ್ಯ_ದ(this_cpu);

	/*
	 * If sync, adjust the weight of prev_eff_load such that if
	 * prev_eff == this_eff that select_idle_sibling() will consider
	 * stacking the wakee on top of the waker if no other CPU is
	 * idle.
	 */
	ಆದರೆ (sync)
		prev_eff_ತುಂಬು += 1;

	ಹಿಂತಿರುಗಿ this_eff_ತುಂಬು < prev_eff_ತುಂಬು ? this_cpu : nr_cpumask_ಬಿಟ್ಟುಗಳು;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ wake_affine(ರಚನೆ sched_domain *sd, ರಚನೆ task_struct *p,
		       ಪೂರ್ಣಾಂಕ this_cpu, ಪೂರ್ಣಾಂಕ prev_cpu, ಪೂರ್ಣಾಂಕ sync)
{
	ಪೂರ್ಣಾಂಕ ಗುರಿ = nr_cpumask_ಬಿಟ್ಟುಗಳು;

	ಆದರೆ (sched_feat(WA_IDLE))
		ಗುರಿ = wake_affine_idle(this_cpu, prev_cpu, sync);

	ಆದರೆ (sched_feat(WA_ತೂಕ) && ಗುರಿ == nr_cpumask_ಬಿಟ್ಟುಗಳು)
		ಗುರಿ = wake_affine_ತೂಕ(sd, p, this_cpu, prev_cpu, sync);

	schedstat_inc(p->stats.nr_wakeups_affine_attempts);
	ಆದರೆ (ಗುರಿ != this_cpu)
		ಹಿಂತಿರುಗಿ prev_cpu;

	schedstat_inc(sd->ttwu_move_affine);
	schedstat_inc(p->stats.nr_wakeups_affine);
	ಹಿಂತಿರುಗಿ ಗುರಿ;
}

ಸ್ಥಿರ ರಚನೆ sched_ಗುಂಪು *
sched_balance_ಕಂಡುಹಿಡಿ_dst_ಗುಂಪು(ರಚನೆ sched_domain *sd, ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ this_cpu);

/*
 * sched_balance_find_dst_group_cpu - find the idlest CPU among the CPUs in the group.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
sched_balance_ಕಂಡುಹಿಡಿ_dst_ಗುಂಪು_cpu(ರಚನೆ sched_ಗುಂಪು *ಗುಂಪು, ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ this_cpu)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ತುಂಬು, min_ತುಂಬು = ULONG_MAX;
	u64 min_exit_latency = U64_MAX;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ nr_candidates = 0;
	ಪೂರ್ಣಾಂಕ ಕನಿಷ್ಠ_loaded_cpu = this_cpu;
	ಪೂರ್ಣಾಂಕ shallowest_idle_cpu = -1;
	ಪೂರ್ಣಾಂಕ i;

	/* Check if we have any choice: */
	ಆದರೆ (ಗುಂಪು->ಗುಂಪು_ತೂಕ == 1)
		ಹಿಂತಿರುಗಿ cpumask_ಮೊದಲ(sched_ಗುಂಪು_ಹರವು(ಗುಂಪು));

	/* Traverse only the allowed CPUs */
	for_each_cpu_and(i, sched_ಗುಂಪು_ಹರವು(ಗುಂಪು), p->cpus_ptr) {
		ರಚನೆ rq *rq = cpu_rq(i);

		ಆದರೆ (!sched_core_cookie_ಹೊಂದಿಕೆ(rq, p))
			ಮುಂದುವರಿ;

		ಆದರೆ (ಆರಿಸು_sched_idle_rq(rq, p))
			ಹಿಂತಿರುಗಿ i;

		ಆದರೆ (ಲಭ್ಯ_idle_cpu(i)) {
			ರಚನೆ cpuidle_state *idle = idle_get_state(rq);
			u64 exit_latency = idle ? idle->exit_latency : U64_MAX;

			ಆದರೆ (shallowest_idle_cpu == -1 || exit_latency < min_exit_latency) {
				min_exit_latency = exit_latency;
				shallowest_idle_cpu = i;
				nr_candidates = 1;
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (exit_latency == min_exit_latency) {
				nr_candidates++;
				ಆದರೆ (!reciprocal_scale(sched_rng(), nr_candidates))
					shallowest_idle_cpu = i;
			}
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (shallowest_idle_cpu == -1) {
			ತುಂಬು = cpu_ತುಂಬು(cpu_rq(i));
			ಆದರೆ (ತುಂಬು < min_ತುಂಬು) {
				min_ತುಂಬು = ತುಂಬು;
				ಕನಿಷ್ಠ_loaded_cpu = i;
			}
		}
	}

	ಹಿಂತಿರುಗಿ shallowest_idle_cpu != -1 ? shallowest_idle_cpu : ಕನಿಷ್ಠ_loaded_cpu;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ sched_balance_ಕಂಡುಹಿಡಿ_dst_cpu(ರಚನೆ sched_domain *sd, ರಚನೆ task_struct *p,
				  ಪೂರ್ಣಾಂಕ cpu, ಪೂರ್ಣಾಂಕ prev_cpu, ಪೂರ್ಣಾಂಕ sd_flag)
{
	ಪೂರ್ಣಾಂಕ new_cpu = cpu;

	ಆದರೆ (!cpumask_intersects(sched_domain_ಹರವು(sd), p->cpus_ptr))
		ಹಿಂತಿರುಗಿ prev_cpu;

	/*
	 * We need task's util for cpu_util_without, sync it up to
	 * prev_cpu's last_update_time.
	 */
	ಆದರೆ (!(sd_flag & SD_BALANCE_FORK))
		sync_entity_ತುಂಬು_avg(&p->se);

	ಆಗ (sd) {
		ರಚನೆ sched_ಗುಂಪು *ಗುಂಪು;
		ರಚನೆ sched_domain *tmp;
		ಪೂರ್ಣಾಂಕ ತೂಕ;

		ಆದರೆ (!(sd->flags & sd_flag)) {
			sd = sd->ಮಗು;
			ಮುಂದುವರಿ;
		}

		ಗುಂಪು = sched_balance_ಕಂಡುಹಿಡಿ_dst_ಗುಂಪು(sd, p, cpu);
		ಆದರೆ (!ಗುಂಪು) {
			sd = sd->ಮಗು;
			ಮುಂದುವರಿ;
		}

		new_cpu = sched_balance_ಕಂಡುಹಿಡಿ_dst_ಗುಂಪು_cpu(ಗುಂಪು, p, cpu);
		ಆದರೆ (new_cpu == cpu) {
			/* Now try balancing at a lower domain level of 'cpu': */
			sd = sd->ಮಗು;
			ಮುಂದುವರಿ;
		}

		/* Now try balancing at a lower domain level of 'new_cpu': */
		cpu = new_cpu;
		ತೂಕ = sd->ಹರವು_ತೂಕ;
		sd = ಖಾಲಿ;
		for_each_domain(cpu, tmp) {
			ಆದರೆ (ತೂಕ <= tmp->ಹರವು_ತೂಕ)
				ನಿಲ್ಲಿಸು;
			ಆದರೆ (tmp->flags & sd_flag)
				sd = tmp;
		}
	}

	ಹಿಂತಿರುಗಿ new_cpu;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ __select_idle_cpu(ಪೂರ್ಣಾಂಕ cpu, ರಚನೆ task_struct *p)
{
	ಆದರೆ (ಆರಿಸು_idle_cpu(cpu, p) && sched_cpu_cookie_ಹೊಂದಿಕೆ(cpu_rq(cpu), p))
		ಹಿಂತಿರುಗಿ cpu;

	ಹಿಂತಿರುಗಿ -1;
}

DEFINE_STATIC_ಕೀಲಿ_FALSE(sched_smt_present);
EXPORT_ಚಿಹ್ನೆ_GPL(sched_smt_present);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ ಗಣ_idle_cores(ಪೂರ್ಣಾಂಕ cpu, ಪೂರ್ಣಾಂಕ val)
{
	ರಚನೆ sched_domain_shared *sds;

	sds = rcu_dereference_all(ಪ್ರತಿ_cpu(sd_balance_shared, cpu));
	ಆದರೆ (sds)
		WRITE_ONCE(sds->has_idle_cores, val);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool test_idle_cores(ಪೂರ್ಣಾಂಕ cpu)
{
	ರಚನೆ sched_domain_shared *sds;

	sds = rcu_dereference_all(ಪ್ರತಿ_cpu(sd_balance_shared, cpu));
	ಆದರೆ (sds)
		ಹಿಂತಿರುಗಿ READ_ONCE(sds->has_idle_cores);

	ಹಿಂತಿರುಗಿ false;
}

/*
 * Redirect a CPU to a higher-priority available sibling in its SMT domain,
 * subject to task affinity.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ select_idle_smt_cpu(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ cpu)
{
	ರಚನೆ sched_domain *sd;
	ಪೂರ್ಣಾಂಕ ಉತ್ತಮ = cpu;
	ಪೂರ್ಣಾಂಕ sibling;

	ಆದರೆ (!sched_smt_ಸಕ್ರಿಯ())
		ಹಿಂತಿರುಗಿ cpu;

	sd = rcu_dereference_all(cpu_rq(cpu)->sd);
	ಆದರೆ (!sd || !(sd->flags & SD_SHARE_CPUCAPACITY) ||
	    !(sd->flags & SD_ASYM_PACKING))
		ಹಿಂತಿರುಗಿ cpu;

	for_each_cpu_and(sibling, sched_domain_ಹರವು(sd), p->cpus_ptr) {
		ಆದರೆ (sibling == ಉತ್ತಮ || !ಆರಿಸು_idle_cpu(sibling, p))
			ಮುಂದುವರಿ;

		ಆದರೆ (sched_asym_prefer(sibling, ಉತ್ತಮ))
			ಉತ್ತಮ = sibling;
	}

	ಹಿಂತಿರುಗಿ ಉತ್ತಮ;
}

/*
 * Scans the local SMT mask to see if the entire core is idle, and records this
 * information in sd_balance_shared->has_idle_cores.
 *
 * Since SMT siblings share all cache levels, inspecting this limited remote
 * state should be fairly cheap.
 */
ಶೂನ್ಯ __update_idle_core(ರಚನೆ rq *rq)
{
	ಪೂರ್ಣಾಂಕ core = cpu_ದ(rq);
	ಪೂರ್ಣಾಂಕ cpu;

	rcu_read_ಬೀಗ();
	ಆದರೆ (test_idle_cores(core))
		ಹೋಗು unlock;

	for_each_cpu(cpu, cpu_smt_ಮುಖವಾಡ(core)) {
		ಆದರೆ (cpu == core)
			ಮುಂದುವರಿ;

		ಆದರೆ (!ಲಭ್ಯ_idle_cpu(cpu))
			ಹೋಗು unlock;
	}

	ಗಣ_idle_cores(core, 1);
unlock:
	rcu_read_unlock();
}

/*
 * Scan the entire LLC domain for idle cores; this dynamically switches off if
 * there are no idle cores left in the system; tracked through
 * sd_balance_shared->has_idle_cores and enabled through update_idle_core()
 * above.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ select_idle_core(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ core, ರಚನೆ cpumask *cpus, ಪೂರ್ಣಾಂಕ *idle_cpu)
{
	bool idle = true;
	ಪೂರ್ಣಾಂಕ cpu;

	for_each_cpu(cpu, cpu_smt_ಮುಖವಾಡ(core)) {
		ಆದರೆ (!ಲಭ್ಯ_idle_cpu(cpu)) {
			idle = false;
			ಆದರೆ (*idle_cpu == -1) {
				ಆದರೆ (ಆರಿಸು_sched_idle_rq(cpu_rq(cpu), p) &&
				    cpumask_test_cpu(cpu, cpus)) {
					*idle_cpu = cpu;
					ನಿಲ್ಲಿಸು;
				}
				ಮುಂದುವರಿ;
			}
			ನಿಲ್ಲಿಸು;
		}
		ಆದರೆ (*idle_cpu == -1 && cpumask_test_cpu(cpu, cpus))
			*idle_cpu = cpu;
	}

	ಆದರೆ (idle)
		ಹಿಂತಿರುಗಿ core;

	cpumask_andnot(cpus, cpus, cpu_smt_ಮುಖವಾಡ(core));
	ಹಿಂತಿರುಗಿ -1;
}

/*
 * Scan the local SMT mask for idle CPUs.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ select_idle_smt(ರಚನೆ task_struct *p, ರಚನೆ sched_domain *sd, ಪೂರ್ಣಾಂಕ ಗುರಿ)
{
	ಪೂರ್ಣಾಂಕ cpu;

	for_each_cpu_and(cpu, cpu_smt_ಮುಖವಾಡ(ಗುರಿ), p->cpus_ptr) {
		ಆದರೆ (cpu == ಗುರಿ)
			ಮುಂದುವರಿ;
		/*
		 * Check if the CPU is in the LLC scheduling domain of @target.
		 * Due to isolcpus, there is no guarantee that all the siblings are in the domain.
		 */
		ಆದರೆ (!cpumask_test_cpu(cpu, sched_domain_ಹರವು(sd)))
			ಮುಂದುವರಿ;
		ಆದರೆ (ಆರಿಸು_idle_cpu(cpu, p))
			ಹಿಂತಿರುಗಿ cpu;
	}

	ಹಿಂತಿರುಗಿ -1;
}

/*
 * Scan the LLC domain for idle CPUs; this is dynamically regulated by
 * comparing the average scan cost (tracked in sd->avg_scan_cost) against the
 * average idle time for this rq (as found in rq->avg_idle).
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ select_idle_cpu(ರಚನೆ task_struct *p, ರಚನೆ sched_domain *sd, bool has_idle_core, ಪೂರ್ಣಾಂಕ ಗುರಿ)
{
	ರಚನೆ cpumask *cpus = this_cpu_cpumask_var_ptr(select_rq_ಮುಖವಾಡ);
	ಪೂರ್ಣಾಂಕ i, cpu, idle_cpu = -1, nr = ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ;

	ಆದರೆ (sched_feat(SIS_UTIL) && sd->shared) {
		/*
		 * Increment because !--nr is the condition to stop scan.
		 *
		 * Since "sd" is "sd_llc" for target CPU dereferenced in the
		 * caller, it is safe to directly dereference "sd->shared".
		 * Topology bits always ensure it assigned for "sd_llc" abd it
		 * cannot disappear as long as we have a RCU protected
		 * reference to one the associated "sd" here.
		 */
		nr = READ_ONCE(sd->shared->nr_idle_ಪರಿಶೀಲನೆ) + 1;
		/* overloaded LLC is unlikely to have idle cpu/core */
		ಆದರೆ (nr == 1)
			ಹಿಂತಿರುಗಿ -1;
	}

	ಆದರೆ (!cpumask_and(cpus, sched_domain_ಹರವು(sd), p->cpus_ptr))
		ಹಿಂತಿರುಗಿ -1;

	ಆದರೆ (static_branch_unlikely(&sched_cluster_ಸಕ್ರಿಯ)) {
		ರಚನೆ sched_ಗುಂಪು *sg = sd->groups;

		ಆದರೆ (sg->flags & SD_CLUSTER) {
			for_each_cpu_wrap(cpu, sched_ಗುಂಪು_ಹರವು(sg), ಗುರಿ + 1) {
				ಆದರೆ (!cpumask_test_cpu(cpu, cpus))
					ಮುಂದುವರಿ;

				ಆದರೆ (has_idle_core) {
					i = select_idle_core(p, cpu, cpus, &idle_cpu);
					ಆದರೆ ((ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)i < nr_cpumask_ಬಿಟ್ಟುಗಳು)
						ಹಿಂತಿರುಗಿ i;
				} ಇಲ್ಲದಿದ್ದರೆ {
					ಆದರೆ (--nr <= 0)
						ಹಿಂತಿರುಗಿ -1;
					idle_cpu = __select_idle_cpu(cpu, p);
					ಆದರೆ ((ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)idle_cpu < nr_cpumask_ಬಿಟ್ಟುಗಳು)
						ಹಿಂತಿರುಗಿ idle_cpu;
				}
			}
			cpumask_andnot(cpus, cpus, sched_ಗುಂಪು_ಹರವು(sg));
		}
	}

	for_each_cpu_wrap(cpu, cpus, ಗುರಿ + 1) {
		ಆದರೆ (has_idle_core) {
			i = select_idle_core(p, cpu, cpus, &idle_cpu);
			ಆದರೆ ((ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)i < nr_cpumask_ಬಿಟ್ಟುಗಳು)
				ಹಿಂತಿರುಗಿ i;

		} ಇಲ್ಲದಿದ್ದರೆ {
			ಆದರೆ (--nr <= 0)
				ಹಿಂತಿರುಗಿ -1;
			idle_cpu = __select_idle_cpu(cpu, p);
			ಆದರೆ ((ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)idle_cpu < nr_cpumask_ಬಿಟ್ಟುಗಳು)
				ನಿಲ್ಲಿಸು;
		}
	}

	ಆದರೆ (has_idle_core)
		ಗಣ_idle_cores(ಗುರಿ, false);

	ಹಿಂತಿರುಗಿ idle_cpu;
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
ಎಣಿಕೆ asym_ಹೊಂದುತ್ತದೆ_state {
	ASYM_IDLE_UCLAMP_MISFIT = -4,
	ASYM_IDLE_COMPLETE_MISFIT,
	ASYM_IDLE_THREAD_ಹೊಂದುತ್ತದೆ,
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
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
select_idle_ಸಾಮರ್ಥ್ಯ(ರಚನೆ task_struct *p, ರಚನೆ sched_domain *sd, ಪೂರ್ಣಾಂಕ ಗುರಿ)
{
	/*
	 * On !SMT systems, has_idle_core is always false and preferred_core
	 * is always true (CPU == core), so the SMT preference logic below
	 * collapses to the plain capacity scan.
	 */
	bool has_idle_core = sched_smt_ಸಕ್ರಿಯ() && test_idle_cores(ಗುರಿ);
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ task_util, util_min, util_max, ಉತ್ತಮ_cap = 0;
	ಪೂರ್ಣಾಂಕ ಹೊಂದುತ್ತದೆ, ಉತ್ತಮ_ಹೊಂದುತ್ತದೆ = ASYM_IDLE_THREAD_MISFIT;
	ಪೂರ್ಣಾಂಕ cpu, ಉತ್ತಮ_cpu = -1;
	ರಚನೆ cpumask *cpus;
	ಪೂರ್ಣಾಂಕ nr = ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ;

	cpus = this_cpu_cpumask_var_ptr(select_rq_ಮುಖವಾಡ);
	cpumask_and(cpus, sched_domain_ಹರವು(sd), p->cpus_ptr);

	task_util = task_util_est(p);
	util_min = uclamp_eff_ಮೌಲ್ಯ(p, UCLAMP_MIN);
	util_max = uclamp_eff_ಮೌಲ್ಯ(p, UCLAMP_MAX);

	ಆದರೆ (sched_feat(SIS_UTIL) && sd->shared) {
		/*
		 * Same nr_idle_scan hint as select_idle_cpu(), nr only limits
		 * the scan when not preferring an idle core.
		 */
		nr = READ_ONCE(sd->shared->nr_idle_ಪರಿಶೀಲನೆ) + 1;
		/* overloaded domain is unlikely to have idle cpu/core */
		ಆದರೆ (nr == 1)
			ಹಿಂತಿರುಗಿ -1;
	}

	for_each_cpu_wrap(cpu, cpus, ಗುರಿ) {
		bool preferred_core = !has_idle_core || is_core_idle(cpu);
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ cpu_cap = ಸಾಮರ್ಥ್ಯ_ದ(cpu);

		/*
		 * Stop when the nr_idle_scan is exhausted (mirrors
		 * select_idle_cpu() logic).
		 */
		ಆದರೆ (!has_idle_core && --nr <= 0)
			ಹಿಂತಿರುಗಿ ಉತ್ತಮ_cpu;

		ಆದರೆ (!ಆರಿಸು_idle_cpu(cpu, p))
			ಮುಂದುವರಿ;

		ಹೊಂದುತ್ತದೆ = util_ಹೊಂದುತ್ತದೆ_cpu(task_util, util_min, util_max, cpu);

		/*
		 * Perfect fit: capacity satisfies util + uclamp and the CPU
		 * sits on a fully-idle SMT core, this is a !SMT system, or
		 * there is no idle core to find.
		 * Short-circuit the rank-based selection and return
		 * immediately.
		 */
		ಆದರೆ (ಹೊಂದುತ್ತದೆ > 0 && preferred_core)
			ಹಿಂತಿರುಗಿ cpu;
		/*
		 * Only the min performance hint (i.e. uclamp_min) doesn't fit.
		 * Look for the CPU with best capacity.
		 */
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಹೊಂದುತ್ತದೆ < 0)
			cpu_cap = get_actual_cpu_ಸಾಮರ್ಥ್ಯ(cpu);
		/*
		 * fits > 0 implies we are not on a preferred core, but the util
		 * fits CPU capacity. Set fits to ASYM_IDLE_THREAD_FITS
		 * so the effective range becomes
		 * [ASYM_IDLE_THREAD_FITS, ASYM_IDLE_THREAD_MISFIT], where:
		 *    ASYM_IDLE_THREAD_MISFIT - does not fit
		 *    ASYM_IDLE_THREAD_UCLAMP_MISFIT - fits with the exception of UCLAMP_MIN
		 *    ASYM_IDLE_THREAD_FITS - fits with the exception of preferred_core
		 */
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಹೊಂದುತ್ತದೆ > 0)
			ಹೊಂದುತ್ತದೆ = ASYM_IDLE_THREAD_ಹೊಂದುತ್ತದೆ;

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
		ಆದರೆ (preferred_core)
			ಹೊಂದುತ್ತದೆ += ASYM_IDLE_CORE_BIAS;

		/*
		 * First, select CPU which fits better (lower is more preferred).
		 * Then, select the one with best capacity at same level.
		 */
		ಆದರೆ ((ಹೊಂದುತ್ತದೆ < ಉತ್ತಮ_ಹೊಂದುತ್ತದೆ) ||
		    ((ಹೊಂದುತ್ತದೆ == ಉತ್ತಮ_ಹೊಂದುತ್ತದೆ) && (cpu_cap > ಉತ್ತಮ_cap))) {
			ಉತ್ತಮ_cap = cpu_cap;
			ಉತ್ತಮ_cpu = cpu;
			ಉತ್ತಮ_ಹೊಂದುತ್ತದೆ = ಹೊಂದುತ್ತದೆ;
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
	ಆದರೆ (has_idle_core && ಉತ್ತಮ_ಹೊಂದುತ್ತದೆ > ASYM_IDLE_COMPLETE_MISFIT)
		ಗಣ_idle_cores(ಗುರಿ, false);

	ಹಿಂತಿರುಗಿ ಉತ್ತಮ_cpu;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool asym_ಹೊಂದುತ್ತದೆ_cpu(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ util,
				 ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ util_min,
				 ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ util_max,
				 ಪೂರ್ಣಾಂಕ cpu)
{
	ಆದರೆ (sched_asym_cpucap_ಸಕ್ರಿಯ()) {
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
		ಹಿಂತಿರುಗಿ (!sched_smt_ಸಕ್ರಿಯ() || is_core_idle(cpu)) &&
		       (util_ಹೊಂದುತ್ತದೆ_cpu(util, util_min, util_max, cpu) > 0);
	}

	ಹಿಂತಿರುಗಿ true;
}

/*
 * Try and locate an idle core/thread in the LLC cache domain.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ select_idle_sibling(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ prev, ಪೂರ್ಣಾಂಕ ಗುರಿ)
{
	bool has_idle_core = false;
	ರಚನೆ sched_domain *sd;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ task_util, util_min, util_max;
	ಪೂರ್ಣಾಂಕ i, recent_ಬಳಸಿದ_cpu, prev_aff = -1;

	/*
	 * On asymmetric system, update task utilization because we will check
	 * that the task fits with CPU's capacity.
	 */
	ಆದರೆ (sched_asym_cpucap_ಸಕ್ರಿಯ()) {
		sync_entity_ತುಂಬು_avg(&p->se);
		task_util = task_util_est(p);
		util_min = uclamp_eff_ಮೌಲ್ಯ(p, UCLAMP_MIN);
		util_max = uclamp_eff_ಮೌಲ್ಯ(p, UCLAMP_MAX);
	}

	/*
	 * per-cpu select_rq_mask usage
	 */
	lockdep_assert_irqs_disabled();

	ಆದರೆ (ಆರಿಸು_idle_cpu(ಗುರಿ, p) &&
	    asym_ಹೊಂದುತ್ತದೆ_cpu(task_util, util_min, util_max, ಗುರಿ))
		ಹೋಗು select_smt_ಆದ್ಯತೆ;

	/*
	 * If the previous CPU is cache affine and idle, don't be stupid:
	 */
	ಆದರೆ (prev != ಗುರಿ && cpus_share_cache(prev, ಗುರಿ) &&
	    ಆರಿಸು_idle_cpu(prev, p) &&
	    asym_ಹೊಂದುತ್ತದೆ_cpu(task_util, util_min, util_max, prev)) {

		ಆದರೆ (!static_branch_unlikely(&sched_cluster_ಸಕ್ರಿಯ) ||
		    cpus_share_resources(prev, ಗುರಿ)) {
			ಗುರಿ = prev;
			ಹೋಗು select_smt_ಆದ್ಯತೆ;
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
	ಆದರೆ (is_ಪ್ರತಿ_cpu_kthread(current) &&
	    ಒಳ_task() &&
	    prev == smp_processor_id() &&
	    this_rq()->nr_running <= 1 &&
	    asym_ಹೊಂದುತ್ತದೆ_cpu(task_util, util_min, util_max, prev)) {
		ಗುರಿ = prev;
		ಹೋಗು select_smt_ಆದ್ಯತೆ;
	}

	/* Check a recently used CPU as a potential idle candidate: */
	recent_ಬಳಸಿದ_cpu = p->recent_ಬಳಸಿದ_cpu;
	p->recent_ಬಳಸಿದ_cpu = prev;
	ಆದರೆ (recent_ಬಳಸಿದ_cpu != prev &&
	    recent_ಬಳಸಿದ_cpu != ಗುರಿ &&
	    cpus_share_cache(recent_ಬಳಸಿದ_cpu, ಗುರಿ) &&
	    ಆರಿಸು_idle_cpu(recent_ಬಳಸಿದ_cpu, p) &&
	    cpumask_test_cpu(recent_ಬಳಸಿದ_cpu, p->cpus_ptr) &&
	    asym_ಹೊಂದುತ್ತದೆ_cpu(task_util, util_min, util_max, recent_ಬಳಸಿದ_cpu)) {

		ಆದರೆ (!static_branch_unlikely(&sched_cluster_ಸಕ್ರಿಯ) ||
		    cpus_share_resources(recent_ಬಳಸಿದ_cpu, ಗುರಿ)) {
			ಗುರಿ = recent_ಬಳಸಿದ_cpu;
			ಹೋಗು select_smt_ಆದ್ಯತೆ;
		}

	} ಇಲ್ಲದಿದ್ದರೆ {
		recent_ಬಳಸಿದ_cpu = -1;
	}

	/*
	 * For asymmetric CPU capacity systems, our domain of interest is
	 * sd_asym_cpucapacity rather than sd_llc.
	 */
	ಆದರೆ (sched_asym_cpucap_ಸಕ್ರಿಯ()) {
		sd = rcu_dereference_all(ಪ್ರತಿ_cpu(sd_asym_cpucapacity, ಗುರಿ));
		/*
		 * On an asymmetric CPU capacity system where an exclusive
		 * cpuset defines a symmetric island (i.e. one unique
		 * capacity_orig value through the cpuset), the key will be set
		 * but the CPUs within that cpuset will not have a domain with
		 * SD_ASYM_CPUCAPACITY. These should follow the usual symmetric
		 * capacity path.
		 */
		ಆದರೆ (sd) {
			i = select_idle_ಸಾಮರ್ಥ್ಯ(p, sd, ಗುರಿ);
			ಆದರೆ ((ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)i < nr_cpumask_ಬಿಟ್ಟುಗಳು) {
				ಗುರಿ = i;
				ಹೋಗು select_smt_ಆದ್ಯತೆ;
			}
			ಹಿಂತಿರುಗಿ ಗುರಿ;
		}
	}

	sd = rcu_dereference_all(ಪ್ರತಿ_cpu(sd_llc, ಗುರಿ));
	ಆದರೆ (!sd)
		ಹಿಂತಿರುಗಿ ಗುರಿ;

	ಆದರೆ (sched_smt_ಸಕ್ರಿಯ()) {
		has_idle_core = test_idle_cores(ಗುರಿ);

		ಆದರೆ (!has_idle_core && cpus_share_cache(prev, ಗುರಿ)) {
			i = select_idle_smt(p, sd, prev);
			ಆದರೆ ((ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)i < nr_cpumask_ಬಿಟ್ಟುಗಳು) {
				ಗುರಿ = i;
				ಹೋಗು select_smt_ಆದ್ಯತೆ;
			}
		}
	}

	i = select_idle_cpu(p, sd, has_idle_core, ಗುರಿ);
	ಆದರೆ ((ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)i < nr_cpumask_ಬಿಟ್ಟುಗಳು) {
		ಗುರಿ = i;
		ಹೋಗು select_smt_ಆದ್ಯತೆ;
	}

	/*
	 * For cluster machines which have lower sharing cache like L2 or
	 * LLC Tag, we tend to find an idle CPU in the target's cluster
	 * first. But prev_cpu or recent_used_cpu may also be a good candidate,
	 * use them if possible when no idle CPU found in select_idle_cpu().
	 */
	ಆದರೆ ((ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)prev_aff < nr_cpumask_ಬಿಟ್ಟುಗಳು) {
		ಗುರಿ = prev_aff;
		ಹೋಗು select_smt_ಆದ್ಯತೆ;
	}
	ಆದರೆ ((ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)recent_ಬಳಸಿದ_cpu < nr_cpumask_ಬಿಟ್ಟುಗಳು) {
		ಗುರಿ = recent_ಬಳಸಿದ_cpu;
		ಹೋಗು select_smt_ಆದ್ಯತೆ;
	}

	ಹಿಂತಿರುಗಿ ಗುರಿ;

select_smt_ಆದ್ಯತೆ:
	ಹಿಂತಿರುಗಿ select_idle_smt_cpu(p, ಗುರಿ);
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
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ
cpu_util(ಪೂರ್ಣಾಂಕ cpu, ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ dst_cpu, ಪೂರ್ಣಾಂಕ boost)
{
	bool ಕೂಡಿಸು_task = p && task_cpu(p) != cpu && dst_cpu == cpu;
	bool sub_task = p && task_cpu(p) == cpu && dst_cpu != cpu;
	ರಚನೆ cfs_rq *cfs_rq = &cpu_rq(cpu)->cfs;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ util = READ_ONCE(cfs_rq->avg.util_avg);
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ runnable;

	/*
	 * If @dst_cpu is -1 or @p migrates from @cpu to @dst_cpu remove its
	 * contribution. If @p migrates from another CPU to @cpu add its
	 * contribution. In all the other cases @cpu is not impacted by the
	 * migration so its util_avg is already correct.
	 */
	ಆದರೆ (ಕೂಡಿಸು_task)
		util += task_util(p);
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (sub_task)
		lsub_ಧನಾತ್ಮಕ(&util, task_util(p));

	ಆದರೆ (boost) {
		runnable = READ_ONCE(cfs_rq->avg.runnable_avg);
		ಆದರೆ (ಕೂಡಿಸು_task)
			runnable += READ_ONCE(p->se.avg.runnable_avg);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (sub_task)
			lsub_ಧನಾತ್ಮಕ(&runnable,
				      READ_ONCE(p->se.avg.runnable_avg));
		util = max(util, runnable);
	}

	ಆದರೆ (sched_feat(UTIL_EST)) {
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ util_est;

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
		ಆದರೆ (dst_cpu == cpu)
			util_est += _task_util_est(p);
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (p && unlikely(task_on_rq_queued(p) || current == p))
			lsub_ಧನಾತ್ಮಕ(&util_est, _task_util_est(p));

		util = max(util, util_est);
	}

	ಹಿಂತಿರುಗಿ min(util, arch_scale_cpu_ಸಾಮರ್ಥ್ಯ(cpu));
}

ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ cpu_util_cfs(ಪೂರ್ಣಾಂಕ cpu)
{
	ಹಿಂತಿರುಗಿ cpu_util(cpu, ಖಾಲಿ, -1, 0);
}

ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ cpu_util_cfs_boost(ಪೂರ್ಣಾಂಕ cpu)
{
	ಹಿಂತಿರುಗಿ cpu_util(cpu, ಖಾಲಿ, -1, 1);
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
ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ cpu_util_without(ಪೂರ್ಣಾಂಕ cpu, ರಚನೆ task_struct *p)
{
	/* Task has no contribution or is new */
	ಆದರೆ (cpu != task_cpu(p) || !READ_ONCE(p->se.avg.ಕೊನೆಯ_update_ಸಮಯ))
		p = ಖಾಲಿ;

	ಹಿಂತಿರುಗಿ cpu_util(cpu, p, -1, 0);
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
ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ effective_cpu_util(ಪೂರ್ಣಾಂಕ cpu, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ util_cfs,
				 ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ *min,
				 ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ *max)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ util, irq, scale;
	ರಚನೆ rq *rq = cpu_rq(cpu);

	scale = arch_scale_cpu_ಸಾಮರ್ಥ್ಯ(cpu);

	/*
	 * Early check to see if IRQ/steal time saturates the CPU, can be
	 * because of inaccuracies in how we track these -- see
	 * update_irq_load_avg().
	 */
	irq = cpu_util_irq(rq);
	ಆದರೆ (unlikely(irq >= scale)) {
		ಆದರೆ (min)
			*min = scale;
		ಆದರೆ (max)
			*max = scale;
		ಹಿಂತಿರುಗಿ scale;
	}

	ಆದರೆ (min) {
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
		ಆದರೆ (!uclamp_is_ಬಳಸಿದ() && rt_rq_is_runnable(&rq->rt))
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
	ಆದರೆ (max)
		*max = min(scale, uclamp_rq_get(rq, UCLAMP_MAX));

	ಆದರೆ (util >= scale)
		ಹಿಂತಿರುಗಿ scale;

	/*
	 * There is still idle time; further improve the number by using the
	 * IRQ metric. Because IRQ/steal time is hidden from the task clock we
	 * need to scale the task numbers:
	 *
	 *              max - irq
	 *   U' = irq + --------- * U
	 *                 max
	 */
	util = scale_irq_ಸಾಮರ್ಥ್ಯ(util, irq, scale);
	util += irq;

	ಹಿಂತಿರುಗಿ min(scale, util);
}

ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ sched_cpu_util(ಪೂರ್ಣಾಂಕ cpu)
{
	ಹಿಂತಿರುಗಿ effective_cpu_util(cpu, cpu_util_cfs(cpu), ಖಾಲಿ, ಖಾಲಿ);
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
ರಚನೆ energy_env {
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ task_busy_ಸಮಯ;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ pd_busy_ಸಮಯ;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ cpu_cap;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ pd_cap;
};

/*
 * Compute the task busy time for compute_energy(). This time cannot be
 * injected directly into effective_cpu_util() because of the IRQ scaling.
 * The latter only makes sense with the most recent CPUs where the task has
 * run.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ eenv_task_busy_ಸಮಯ(ರಚನೆ energy_env *eenv,
				       ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ prev_cpu)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ busy_ಸಮಯ, max_cap = arch_scale_cpu_ಸಾಮರ್ಥ್ಯ(prev_cpu);
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ irq = cpu_util_irq(cpu_rq(prev_cpu));

	ಆದರೆ (unlikely(irq >= max_cap))
		busy_ಸಮಯ = max_cap;
	ಇಲ್ಲದಿದ್ದರೆ
		busy_ಸಮಯ = scale_irq_ಸಾಮರ್ಥ್ಯ(task_util_est(p), irq, max_cap);

	eenv->task_busy_ಸಮಯ = busy_ಸಮಯ;
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
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ eenv_pd_busy_ಸಮಯ(ರಚನೆ energy_env *eenv,
				     ರಚನೆ cpumask *pd_cpus,
				     ರಚನೆ task_struct *p)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ busy_ಸಮಯ = 0;
	ಪೂರ್ಣಾಂಕ cpu;

	for_each_cpu(cpu, pd_cpus) {
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ util = cpu_util(cpu, p, -1, 0);

		busy_ಸಮಯ += effective_cpu_util(cpu, util, ಖಾಲಿ, ಖಾಲಿ);
	}

	eenv->pd_busy_ಸಮಯ = min(eenv->pd_cap, busy_ಸಮಯ);
}

/*
 * Compute the maximum utilization for compute_energy() when the task @p
 * is placed on the cpu @dst_cpu.
 *
 * Returns the maximum utilization among @eenv->cpus. This utilization can't
 * exceed @eenv->cpu_cap.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ
eenv_pd_max_util(ರಚನೆ energy_env *eenv, ರಚನೆ cpumask *pd_cpus,
		 ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ dst_cpu)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ max_util = 0;
	ಪೂರ್ಣಾಂಕ cpu;

	for_each_cpu(cpu, pd_cpus) {
		ರಚನೆ task_struct *tsk = (cpu == dst_cpu) ? p : ಖಾಲಿ;
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ util = cpu_util(cpu, p, dst_cpu, 1);
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ eff_util, min, max;

		/*
		 * Performance domain frequency: utilization clamping
		 * must be considered since it affects the selection
		 * of the performance domain frequency.
		 * NOTE: in case RT tasks are running, by default the min
		 * utilization can be max OPP.
		 */
		eff_util = effective_cpu_util(cpu, util, &min, &max);

		/* Task's uclamp can modify min and max value */
		ಆದರೆ (tsk && uclamp_is_ಬಳಸಿದ()) {
			min = max(min, uclamp_eff_ಮೌಲ್ಯ(p, UCLAMP_MIN));

			/*
			 * If there is no active max uclamp constraint,
			 * directly use task's one, otherwise keep max.
			 */
			ಆದರೆ (uclamp_rq_is_idle(cpu_rq(cpu)))
				max = uclamp_eff_ಮೌಲ್ಯ(p, UCLAMP_MAX);
			ಇಲ್ಲದಿದ್ದರೆ
				max = max(max, uclamp_eff_ಮೌಲ್ಯ(p, UCLAMP_MAX));
		}

		eff_util = sugov_effective_cpu_perf(cpu, eff_util, min, max);
		max_util = max(max_util, eff_util);
	}

	ಹಿಂತಿರುಗಿ min(max_util, eenv->cpu_cap);
}

/*
 * compute_energy(): Use the Energy Model to estimate the energy that @pd would
 * consume for a given utilization landscape @eenv. When @dst_cpu < 0, the task
 * contribution is ignored.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ
compute_energy(ರಚನೆ energy_env *eenv, ರಚನೆ perf_domain *pd,
	       ರಚನೆ cpumask *pd_cpus, ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ dst_cpu)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ max_util = eenv_pd_max_util(eenv, pd_cpus, p, dst_cpu);
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ busy_ಸಮಯ = eenv->pd_busy_ಸಮಯ;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ energy;

	ಆದರೆ (dst_cpu >= 0)
		busy_ಸಮಯ = min(eenv->pd_cap, busy_ಸಮಯ + eenv->task_busy_ಸಮಯ);

	energy = em_cpu_energy(pd->em_pd, max_util, busy_ಸಮಯ, eenv->cpu_cap);

	trace_sched_compute_energy_tp(p, dst_cpu, energy, max_util, busy_ಸಮಯ);

	ಹಿಂತಿರುಗಿ energy;
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
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಕಂಡುಹಿಡಿ_energy_efficient_cpu(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ prev_cpu)
{
	ರಚನೆ cpumask *cpus = this_cpu_cpumask_var_ptr(select_rq_ಮುಖವಾಡ);
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ prev_delta = ULONG_MAX, ಉತ್ತಮ_delta = ULONG_MAX;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ p_util_min = uclamp_is_ಬಳಸಿದ() ? uclamp_eff_ಮೌಲ್ಯ(p, UCLAMP_MIN) : 0;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ p_util_max = uclamp_is_ಬಳಸಿದ() ? uclamp_eff_ಮೌಲ್ಯ(p, UCLAMP_MAX) : 1024;
	ರಚನೆ ಬೇರು_domain *rd = this_rq()->rd;
	ಪೂರ್ಣಾಂಕ cpu, ಉತ್ತಮ_energy_cpu, ಗುರಿ = -1;
	ಪೂರ್ಣಾಂಕ prev_ಹೊಂದುತ್ತದೆ = -1, ಉತ್ತಮ_ಹೊಂದುತ್ತದೆ = -1;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಉತ್ತಮ_actual_cap = 0;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ prev_actual_cap = 0;
	ರಚನೆ sched_domain *sd;
	ರಚನೆ perf_domain *pd;
	ರಚನೆ energy_env eenv;

	pd = rcu_dereference_all(rd->pd);
	ಆದರೆ (!pd)
		ಹಿಂತಿರುಗಿ ಗುರಿ;

	/*
	 * Energy-aware wake-up happens on the lowest sched_domain starting
	 * from sd_asym_cpucapacity spanning over this_cpu and prev_cpu.
	 */
	sd = rcu_dereference_all(*this_cpu_ptr(&sd_asym_cpucapacity));
	ಆಗ (sd && !cpumask_test_cpu(prev_cpu, sched_domain_ಹರವು(sd)))
		sd = sd->ಪೋಷಕ;
	ಆದರೆ (!sd)
		ಹಿಂತಿರುಗಿ ಗುರಿ;

	ಗುರಿ = prev_cpu;

	sync_entity_ತುಂಬು_avg(&p->se);
	ಆದರೆ (!task_util_est(p) && p_util_min == 0)
		ಹಿಂತಿರುಗಿ ಗುರಿ;

	eenv_task_busy_ಸಮಯ(&eenv, p, prev_cpu);

	ಗಾಗಿ (; pd; pd = pd->ಮುಂದಿನ) {
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ util_min = p_util_min, util_max = p_util_max;
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ cpu_cap, cpu_actual_cap, util;
		ದೀರ್ಘ prev_ಹೆಚ್ಚುವರಿ_cap = -1, max_ಹೆಚ್ಚುವರಿ_cap = -1;
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ rq_util_min, rq_util_max;
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ cur_delta, ತಳ_energy;
		ಪೂರ್ಣಾಂಕ max_ಹೆಚ್ಚುವರಿ_cap_cpu = -1;
		ಪೂರ್ಣಾಂಕ ಹೊಂದುತ್ತದೆ, max_ಹೊಂದುತ್ತದೆ = -1;

		ಆದರೆ (!cpumask_and(cpus, perf_domain_ಹರವು(pd), cpu_online_ಮುಖವಾಡ))
			ಮುಂದುವರಿ;

		/* Account external pressure for the energy estimation */
		cpu = cpumask_ಮೊದಲ(cpus);
		cpu_actual_cap = get_actual_cpu_ಸಾಮರ್ಥ್ಯ(cpu);

		eenv.cpu_cap = cpu_actual_cap;
		eenv.pd_cap = 0;

		for_each_cpu(cpu, cpus) {
			ರಚನೆ rq *rq = cpu_rq(cpu);

			eenv.pd_cap += cpu_actual_cap;

			ಆದರೆ (!cpumask_test_cpu(cpu, sched_domain_ಹರವು(sd)))
				ಮುಂದುವರಿ;

			ಆದರೆ (!cpumask_test_cpu(cpu, p->cpus_ptr))
				ಮುಂದುವರಿ;

			util = cpu_util(cpu, p, cpu, 0);
			cpu_cap = ಸಾಮರ್ಥ್ಯ_ದ(cpu);

			/*
			 * Skip CPUs that cannot satisfy the capacity request.
			 * IOW, placing the task there would make the CPU
			 * overutilized. Take uclamp into account to see how
			 * much capacity we can get out of the CPU; this is
			 * aligned with sched_cpu_util().
			 */
			ಆದರೆ (uclamp_is_ಬಳಸಿದ() && !uclamp_rq_is_idle(rq)) {
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

			ಹೊಂದುತ್ತದೆ = util_ಹೊಂದುತ್ತದೆ_cpu(util, util_min, util_max, cpu);
			ಆದರೆ (!ಹೊಂದುತ್ತದೆ)
				ಮುಂದುವರಿ;

			lsub_ಧನಾತ್ಮಕ(&cpu_cap, util);

			ಆದರೆ (cpu == prev_cpu) {
				/* Always use prev_cpu as a candidate. */
				prev_ಹೆಚ್ಚುವರಿ_cap = cpu_cap;
				prev_ಹೊಂದುತ್ತದೆ = ಹೊಂದುತ್ತದೆ;
			} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ((ಹೊಂದುತ್ತದೆ > max_ಹೊಂದುತ್ತದೆ) ||
				   ((ಹೊಂದುತ್ತದೆ == max_ಹೊಂದುತ್ತದೆ) && ((ದೀರ್ಘ)cpu_cap > max_ಹೆಚ್ಚುವರಿ_cap))) {
				/*
				 * Find the CPU with the maximum spare capacity
				 * among the remaining CPUs in the performance
				 * domain.
				 */
				max_ಹೆಚ್ಚುವರಿ_cap = cpu_cap;
				max_ಹೆಚ್ಚುವರಿ_cap_cpu = cpu;
				max_ಹೊಂದುತ್ತದೆ = ಹೊಂದುತ್ತದೆ;
			}
		}

		ಆದರೆ (max_ಹೆಚ್ಚುವರಿ_cap_cpu < 0 && prev_ಹೆಚ್ಚುವರಿ_cap < 0)
			ಮುಂದುವರಿ;

		eenv_pd_busy_ಸಮಯ(&eenv, cpus, p);
		/* Compute the 'base' energy of the pd, without @p */
		ತಳ_energy = compute_energy(&eenv, pd, cpus, p, -1);

		/* Evaluate the energy impact of using prev_cpu. */
		ಆದರೆ (prev_ಹೆಚ್ಚುವರಿ_cap > -1) {
			prev_delta = compute_energy(&eenv, pd, cpus, p,
						    prev_cpu);
			/* CPU utilization has changed */
			ಆದರೆ (prev_delta < ತಳ_energy)
				ಹಿಂತಿರುಗಿ ಗುರಿ;
			prev_delta -= ತಳ_energy;
			prev_actual_cap = cpu_actual_cap;
			ಉತ್ತಮ_delta = min(ಉತ್ತಮ_delta, prev_delta);
		}

		/* Evaluate the energy impact of using max_spare_cap_cpu. */
		ಆದರೆ (max_ಹೆಚ್ಚುವರಿ_cap_cpu >= 0 && max_ಹೆಚ್ಚುವರಿ_cap > prev_ಹೆಚ್ಚುವರಿ_cap) {
			/* Current best energy cpu fits better */
			ಆದರೆ (max_ಹೊಂದುತ್ತದೆ < ಉತ್ತಮ_ಹೊಂದುತ್ತದೆ)
				ಮುಂದುವರಿ;

			/*
			 * Both don't fit performance hint (i.e. uclamp_min)
			 * but best energy cpu has better capacity.
			 */
			ಆದರೆ ((max_ಹೊಂದುತ್ತದೆ < 0) &&
			    (cpu_actual_cap <= ಉತ್ತಮ_actual_cap))
				ಮುಂದುವರಿ;

			cur_delta = compute_energy(&eenv, pd, cpus, p,
						   max_ಹೆಚ್ಚುವರಿ_cap_cpu);
			/* CPU utilization has changed */
			ಆದರೆ (cur_delta < ತಳ_energy)
				ಹಿಂತಿರುಗಿ ಗುರಿ;
			cur_delta -= ತಳ_energy;

			/*
			 * Both fit for the task but best energy cpu has lower
			 * energy impact.
			 */
			ಆದರೆ ((max_ಹೊಂದುತ್ತದೆ > 0) && (ಉತ್ತಮ_ಹೊಂದುತ್ತದೆ > 0) &&
			    (cur_delta >= ಉತ್ತಮ_delta))
				ಮುಂದುವರಿ;

			ಉತ್ತಮ_delta = cur_delta;
			ಉತ್ತಮ_energy_cpu = max_ಹೆಚ್ಚುವರಿ_cap_cpu;
			ಉತ್ತಮ_ಹೊಂದುತ್ತದೆ = max_ಹೊಂದುತ್ತದೆ;
			ಉತ್ತಮ_actual_cap = cpu_actual_cap;
		}
	}

	ಆದರೆ ((ಉತ್ತಮ_ಹೊಂದುತ್ತದೆ > prev_ಹೊಂದುತ್ತದೆ) ||
	    ((ಉತ್ತಮ_ಹೊಂದುತ್ತದೆ > 0) && (ಉತ್ತಮ_delta < prev_delta)) ||
	    ((ಉತ್ತಮ_ಹೊಂದುತ್ತದೆ < 0) && (ಉತ್ತಮ_actual_cap > prev_actual_cap)))
		ಗುರಿ = ಉತ್ತಮ_energy_cpu;

	ಹಿಂತಿರುಗಿ ಗುರಿ;
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
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
select_task_rq_fair(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ prev_cpu, ಪೂರ್ಣಾಂಕ wake_flags)
{
	ಪೂರ್ಣಾಂಕ sync = (wake_flags & WF_SYNC) && !(current->flags & PF_EXITING);
	ರಚನೆ sched_domain *tmp, *sd = ಖಾಲಿ;
	ಪೂರ್ಣಾಂಕ cpu = smp_processor_id();
	ಪೂರ್ಣಾಂಕ new_cpu = prev_cpu;
	ಪೂರ್ಣಾಂಕ want_affine = 0;
	/* SD_flags and WF_flags share the first nibble */
	ಪೂರ್ಣಾಂಕ sd_flag = wake_flags & 0xF;

	/*
	 * required for stable ->cpus_allowed
	 */
	lockdep_assert_held(&p->pi_ಬೀಗ);
	ಆದರೆ (wake_flags & WF_TTWU) {
		ದಾಖಲೆ_wakee(p);

		ಆದರೆ ((wake_flags & WF_CURRENT_CPU) &&
		    cpumask_test_cpu(cpu, p->cpus_ptr))
			ಹಿಂತಿರುಗಿ cpu;

		ಆದರೆ (!is_rd_overutilized(this_rq()->rd)) {
			new_cpu = ಕಂಡುಹಿಡಿ_energy_efficient_cpu(p, prev_cpu);
			ಆದರೆ (new_cpu >= 0)
				ಹಿಂತಿರುಗಿ new_cpu;
			new_cpu = prev_cpu;
		}

		want_affine = !wake_wide(p) && cpumask_test_cpu(cpu, p->cpus_ptr);
	}

	for_each_domain(cpu, tmp) {
		/*
		 * If both 'cpu' and 'prev_cpu' are part of this domain,
		 * cpu is a valid SD_WAKE_AFFINE target.
		 */
		ಆದರೆ (want_affine && (tmp->flags & SD_WAKE_AFFINE) &&
		    cpumask_test_cpu(prev_cpu, sched_domain_ಹರವು(tmp))) {
			ಆದರೆ (cpu != prev_cpu)
				new_cpu = wake_affine(tmp, p, cpu, prev_cpu, sync);

			sd = ಖಾಲಿ; /* Prefer wake_affine over balance flags */
			ನಿಲ್ಲಿಸು;
		}

		/*
		 * Usually only true for WF_EXEC and WF_FORK, as sched_domains
		 * usually do not have SD_BALANCE_WAKE set. That means wakeup
		 * will usually go to the fast path.
		 */
		ಆದರೆ (tmp->flags & sd_flag)
			sd = tmp;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (!want_affine)
			ನಿಲ್ಲಿಸು;
	}

	/* Slow path */
	ಆದರೆ (unlikely(sd)) {
		new_cpu = sched_balance_ಕಂಡುಹಿಡಿ_dst_cpu(sd, p, cpu, prev_cpu, sd_flag);
		ಹಿಂತಿರುಗಿ select_idle_smt_cpu(p, new_cpu);
	}

	/* Fast path */
	ಆದರೆ (wake_flags & WF_TTWU)
		ಹಿಂತಿರುಗಿ select_idle_sibling(p, prev_cpu, new_cpu);

	ಹಿಂತಿರುಗಿ new_cpu;
}

/*
 * Called immediately before a task is migrated to a new CPU; task_cpu(p) and
 * cfs_rq_of(p) references at time of call are still valid and identify the
 * previous CPU. The caller guarantees p->pi_lock or task_rq(p)->lock is held.
 */
ಸ್ಥಿರ ಶೂನ್ಯ migrate_task_rq_fair(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ new_cpu)
{
	ರಚನೆ sched_entity *se = &p->se;

	ಆದರೆ (!task_on_rq_migrating(p)) {
		remove_entity_ತುಂಬು_avg(se);

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
	se->avg.ಕೊನೆಯ_update_ಸಮಯ = 0;

	update_ಪರಿಶೀಲನೆ_period(p, new_cpu);
}

ಸ್ಥಿರ ಶೂನ್ಯ task_dead_fair(ರಚನೆ task_struct *p)
{
	ರಚನೆ sched_entity *se = &p->se;
	remove_entity_ತುಂಬು_avg(se);
}

/*
 * Set the max capacity the task is allowed to run at for misfit detection.
 */
ಸ್ಥಿರ ಶೂನ್ಯ ಗಣ_task_max_allowed_ಸಾಮರ್ಥ್ಯ(ರಚನೆ task_struct *p)
{
	ರಚನೆ asym_cap_ದತ್ತಾಂಶ *ನಮೂದು;

	ಆದರೆ (!sched_asym_cpucap_ಸಕ್ರಿಯ())
		ಹಿಂತಿರುಗಿ;

	rcu_read_ಬೀಗ();
	list_for_each_ನಮೂದು_rcu(ನಮೂದು, &asym_cap_list, link) {
		cpumask_t *cpumask;

		cpumask = cpu_ಸಾಮರ್ಥ್ಯ_ಹರವು(ನಮೂದು);
		ಆದರೆ (!cpumask_intersects(p->cpus_ptr, cpumask))
			ಮುಂದುವರಿ;

		p->max_allowed_ಸಾಮರ್ಥ್ಯ = ನಮೂದು->ಸಾಮರ್ಥ್ಯ;
		ನಿಲ್ಲಿಸು;
	}
	rcu_read_unlock();
}

ಸ್ಥಿರ ಶೂನ್ಯ ಗಣ_cpus_allowed_fair(ರಚನೆ task_struct *p, ರಚನೆ affinity_context *ctx)
{
	ಗಣ_cpus_allowed_common(p, ctx);
	ಗಣ_task_max_allowed_ಸಾಮರ್ಥ್ಯ(p);
}

ಎಣಿಕೆ preempt_wakeup_action {
	PREEMPT_WAKEUP_NONE,	/* No preemption. */
	PREEMPT_WAKEUP_SHORT,	/* Ignore slice protection. */
	PREEMPT_WAKEUP_PICK,	/* Let pick_eevdf() decide. */
	PREEMPT_WAKEUP_RESCHED,	/* Force reschedule. */
};

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool ಗಣ_preempt_buddy(ರಚನೆ cfs_rq *cfs_rq, ರಚನೆ sched_entity *pse)
{
	/*
	 * Keep existing buddy if the deadline is sooner than pse.
	 * The older buddy may be cache cold and completely unrelated
	 * to the current wakeup but that is unpredictable where as
	 * obeying the deadline is more in line with EEVDF objectives.
	 */
	ಆದರೆ (cfs_rq->ಮುಂದಿನ && entity_before(cfs_rq->ಮುಂದಿನ, pse))
		ಹಿಂತಿರುಗಿ false;

	ಗಣ_ಮುಂದಿನ_buddy(cfs_rq, pse);
	ಹಿಂತಿರುಗಿ true;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool ಗಣ_short_buddy(ರಚನೆ cfs_rq *cfs_rq, ರಚನೆ sched_entity *pse)
{
	ಆದರೆ (cfs_rq->ಮುಂದಿನ) {
		ಆದರೆ (cfs_rq->ಮುಂದಿನ->ತುಂಡು < pse->ತುಂಡು)
			ಹಿಂತಿರುಗಿ false;

		ಆದರೆ (cfs_rq->ಮುಂದಿನ->ತುಂಡು == pse->ತುಂಡು &&
		    entity_before(cfs_rq->ಮುಂದಿನ, pse))
			ಹಿಂತಿರುಗಿ false;
	}

	ಗಣ_ಮುಂದಿನ_buddy(cfs_rq, pse);
	ಹಿಂತಿರುಗಿ true;
}

/*
 * WF_SYNC|WF_TTWU indicates the waker expects to sleep but it is not
 * strictly enforced because the hint is either misunderstood or
 * multiple tasks must be woken up.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಎಣಿಕೆ preempt_wakeup_action
preempt_sync(ರಚನೆ rq *rq, ಪೂರ್ಣಾಂಕ wake_flags,
	     ರಚನೆ sched_entity *pse, ರಚನೆ sched_entity *se)
{
	u64 ಮಿತಿ, delta;

	/*
	 * WF_SYNC without WF_TTWU is not expected so warn if it happens even
	 * though it is likely harmless.
	 */
	WARN_ON_ONCE(!(wake_flags & WF_TTWU));

	ಮಿತಿ = sysctl_sched_migration_ವೆಚ್ಚ;
	delta = rq_clock_task(rq) - se->exec_ಆರಂಭ;
	ಆದರೆ ((s64)delta < 0)
		delta = 0;

	/*
	 * WF_RQ_SELECTED implies the tasks are stacking on a CPU when they
	 * could run on other CPUs. Reduce the threshold before preemption is
	 * allowed to an arbitrary lower value as it is more likely (but not
	 * guaranteed) the waker requires the wakee to finish.
	 */
	ಆದರೆ (wake_flags & WF_RQ_SELECTED)
		ಮಿತಿ >>= 2;

	/*
	 * As WF_SYNC is not strictly obeyed, allow some runtime for batch
	 * wakeups to be issued.
	 */
	ಆದರೆ (entity_before(pse, se) && delta >= ಮಿತಿ)
		ಹಿಂತಿರುಗಿ PREEMPT_WAKEUP_RESCHED;

	ಹಿಂತಿರುಗಿ PREEMPT_WAKEUP_NONE;
}

/*
 * Preempt the current task with a newly woken task if needed:
 */
ಸ್ಥಿರ ಶೂನ್ಯ wakeup_preempt_fair(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ wake_flags)
{
	ಎಣಿಕೆ preempt_wakeup_action preempt_action = PREEMPT_WAKEUP_PICK;
	ರಚನೆ task_struct *donor = rq->donor;
	ರಚನೆ sched_entity *nse, *se = &donor->se, *pse = &p->se;
	ರಚನೆ cfs_rq *cfs_rq = &rq->cfs;
	ಪೂರ್ಣಾಂಕ cse_is_idle, pse_is_idle;

	/*
	 * XXX Getting preempted by higher class, try and find idle CPU?
	 */
	ಆದರೆ (p->sched_class != &fair_sched_class ||
	    donor->sched_class != &fair_sched_class)
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (unlikely(se == pse))
		ಹಿಂತಿರುಗಿ;

	/*
	 * This is possible from callers such as attach_tasks(), in which we
	 * unconditionally wakeup_preempt() after an enqueue (which may have
	 * lead to a throttle).  This both saves work and prevents false
	 * next-buddy nomination below.
	 */
	ಆದರೆ (task_is_throttled(p))
		ಹಿಂತಿರುಗಿ;

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
	ಆದರೆ (!sched_feat(PREEMPT_SHORT) && test_tsk_need_resched(rq->curr))
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (!sched_feat(WAKEUP_PREEMPTION))
		ಹಿಂತಿರುಗಿ;

	WARN_ON_ONCE(!pse);

	cse_is_idle = se_is_idle(se);
	pse_is_idle = se_is_idle(pse);

	nse = se;
	/*
	 * Preempt an idle entity in favor of a non-idle entity (and don't preempt
	 * in the inverse case).
	 */
	ಆದರೆ (cse_is_idle && !pse_is_idle)
		ಹೋಗು preempt;

	update_curr_fair(rq);

	ಆದರೆ (cse_is_idle != pse_is_idle)
		ಹೋಗು update;

	/*
	 * BATCH and IDLE tasks do not preempt others.
	 */
	ಆದರೆ (unlikely(!ಸಾಮಾನ್ಯ_policy(p->policy)))
		ಹೋಗು update;

	/*
	 * Do not preempt for tasks that are sched_delayed as it would violate
	 * EEVDF to forcibly queue an ineligible task.
	 */
	ಆದರೆ (pse->sched_delayed)
		ಹೋಗು update;

	/*
	 * If @p has a shorter slice than current and @p is eligible, override
	 * current's slice protection in order to allow preemption.
	 */
	ಆದರೆ (sched_feat(PREEMPT_SHORT) && (pse->ತುಂಡು < se->ತುಂಡು)) {
		preempt_action = PREEMPT_WAKEUP_SHORT;
		ಹೋಗು pick;
	}

	/*
	 * Ignore wakee preemption on WF_FORK as it is less likely that
	 * there is shared data as exec often follow fork.
	 */
	ಆದರೆ (wake_flags & WF_FORK)
		ಹೋಗು update;

	/* Prefer picking wakee soon if appropriate. */
	ಆದರೆ (sched_feat(ಮುಂದಿನ_BUDDY) && ಗಣ_preempt_buddy(cfs_rq, pse)) {
		/*
		 * Decide whether to obey WF_SYNC hint for a new buddy. Old
		 * buddies are ignored as they may not be relevant to the
		 * waker and less likely to be cache hot.
		 */
		ಆದರೆ (wake_flags & WF_SYNC)
			preempt_action = preempt_sync(rq, wake_flags, pse, se);
	}

	ಆಯ್ಕೆ (preempt_action) {
	ಸ್ಥಿತಿ PREEMPT_WAKEUP_NONE:
		ಹಿಂತಿರುಗಿ;
	ಸ್ಥಿತಿ PREEMPT_WAKEUP_RESCHED:
		ಹೋಗು preempt;
	ಸ್ಥಿತಿ PREEMPT_WAKEUP_SHORT:
		fallthrough;
	ಸ್ಥಿತಿ PREEMPT_WAKEUP_PICK:
		ನಿಲ್ಲಿಸು;
	}

pick:
	ಆದರೆ (cfs_rq->h_nr_queued) {
		nse = pick_ಮುಂದಿನ_entity(rq, preempt_action != PREEMPT_WAKEUP_SHORT);
		ಆದರೆ (unlikely(!nse))
			ಹೋಗು pick;

		/* If @p has become the most eligible task, force preemption */
		ಆದರೆ (nse == pse)
			ಹೋಗು preempt;
	}

	/*
	 * If @p is eligible but not the next task to run then cancel protection
	 * to prevent large scheduling latency
	 */
	ಆದರೆ (preempt_action == PREEMPT_WAKEUP_SHORT && entity_eligible(cfs_rq, pse))
		ಹೋಗು preempt;
update:
	ಆದರೆ (sched_feat(ಸತತ_ಗಮ್ಯ_PARITY))
		update_protect_ತುಂಡು(cfs_rq, se);

	ಹಿಂತಿರುಗಿ;

preempt:
	cancel_protect_ತುಂಡು(se);

	ಆದರೆ (preempt_action == PREEMPT_WAKEUP_SHORT)
		ಗಣ_short_buddy(cfs_rq, pse);

	resched_curr_lazy(rq);
}

ರಚನೆ task_struct *pick_task_fair(ರಚನೆ rq *rq, ರಚನೆ rq_flags *rf)
	__must_hold(__rq_lockp(rq))
{
	ರಚನೆ cfs_rq *cfs_rq = &rq->cfs;
	ರಚನೆ sched_entity *se;
	ರಚನೆ task_struct *p;
	ಪೂರ್ಣಾಂಕ new_tasks;

again:
	ಆದರೆ (!cfs_rq->h_nr_queued)
		ಹೋಗು idle;

	/* Might not have done put_prev_entity() */
	ಆದರೆ (cfs_rq->curr && cfs_rq->curr->on_rq)
		update_curr_eevdf(cfs_rq);

	se = pick_ಮುಂದಿನ_entity(rq, true);
	ಆದರೆ (!se)
		ಹೋಗು again;

	p = task_ದ(se);
	ಹಿಂತಿರುಗಿ p;

idle:
	ಆದರೆ (sched_core_enabled(rq))
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	new_tasks = sched_balance_newidle(rq, rf);
	ಆದರೆ (new_tasks < 0)
		ಹಿಂತಿರುಗಿ RETRY_TASK;
	ಆದರೆ (new_tasks > 0)
		ಹೋಗು again;
	ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಸ್ಥಿರ ರಚನೆ task_struct *
fair_server_pick_task(ರಚನೆ sched_dl_entity *dl_se, ರಚನೆ rq_flags *rf)
	__must_hold(__rq_lockp(dl_se->rq))
{
	ಹಿಂತಿರುಗಿ pick_task_fair(dl_se->rq, rf);
}

ಶೂನ್ಯ fair_server_init(ರಚನೆ rq *rq)
{
	ರಚನೆ sched_dl_entity *dl_se = &rq->fair_server;

	init_dl_entity(dl_se);

	dl_server_init(dl_se, rq, fair_server_pick_task);
}

/*
 * Account for a descheduled task:
 */
ಸ್ಥಿರ ಶೂನ್ಯ put_prev_task_fair(ರಚನೆ rq *rq, ರಚನೆ task_struct *prev, ರಚನೆ task_struct *ಮುಂದಿನ)
{
	ರಚನೆ sched_entity *se = &prev->se;
	ರಚನೆ cfs_rq *cfs_rq = &rq->cfs;
	ರಚನೆ sched_entity *nse = ಖಾಲಿ;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_FAIR_ಗುಂಪು_SCHED
	ಆದರೆ (ಮುಂದಿನ && ಮುಂದಿನ->sched_class == &fair_sched_class)
		nse = &ಮುಂದಿನ->se;
#ಆದರೆ_ಅಂತ್ಯ

	ಆಗ (se) {
		cfs_rq = cfs_rq_ದ(se);
		ಆದರೆ (!nse || cfs_rq->h_curr)
			put_prev_entity(cfs_rq, se);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_FAIR_ಗುಂಪು_SCHED
		ಆದರೆ (nse) {
			ಆದರೆ (is_same_ಗುಂಪು(se, nse))
				ನಿಲ್ಲಿಸು;

			ಪೂರ್ಣಾಂಕ d = nse->ಆಳ - se->ಆಳ;
			ಆದರೆ (d >= 0) {
				/* nse has equal or greater depth, ascend */
				nse = ಪೋಷಕ_entity(nse);
				/* if nse is the deeper, do not ascend se */
				ಆದರೆ (d > 0)
					ಮುಂದುವರಿ;
			}
		}
#ಆದರೆ_ಅಂತ್ಯ
		se = ಪೋಷಕ_entity(se);
	}

	/* Put 'current' back into the tree. */
	cfs_rq = &rq->cfs;
	se = &prev->se;
	WARN_ON_ONCE(cfs_rq->curr != se);
	cfs_rq->curr = ಖಾಲಿ;
	ಆದರೆ (se->on_rq)
		__enqueue_entity(cfs_rq, se);
}

/*
 * sched_yield() is very simple
 */
ಸ್ಥಿರ ಶೂನ್ಯ yield_task_fair(ರಚನೆ rq *rq)
{
	ರಚನೆ task_struct *curr = rq->donor;
	ರಚನೆ sched_entity *se = &curr->se;
	ರಚನೆ cfs_rq *cfs_rq = &rq->cfs;

	/*
	 * Are we the only task in the tree?
	 */
	ಆದರೆ (unlikely(rq->nr_running == 1))
		ಹಿಂತಿರುಗಿ;

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
	ಆದರೆ (entity_eligible(cfs_rq, se)) {
		se->vruntime = se->deadline;
		update_deadline(cfs_rq, se);
	}
}

ಸ್ಥಿರ bool yield_ಗಮ್ಯ_task_fair(ರಚನೆ rq *rq, ರಚನೆ task_struct *p)
{
	ರಚನೆ sched_entity *se = &p->se;

	/* !se->on_rq also covers throttled task */
	ಆದರೆ (!se->on_rq || se->sched_delayed)
		ಹಿಂತಿರುಗಿ false;

	/* Tell the scheduler that we'd really like se to run next. */
	ಗಣ_ಮುಂದಿನ_buddy(&task_rq(p)->cfs, se);

	yield_task_fair(rq);

	ಹಿಂತಿರುಗಿ true;
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

ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ __read_mostly max_ತುಂಬು_balance_interval = HZ/10;

ಎಣಿಕೆ fbq_type { regular, remote, all };

/*
 * 'group_type' describes the group of CPUs at the moment of load balancing.
 *
 * The enum is ordered by pulling priority, with the group with lowest priority
 * first so the group_type can simply be compared when selecting the busiest
 * group. See update_sd_pick_busiest().
 */
ಎಣಿಕೆ ಗುಂಪು_type {
	/* The group has spare capacity that can be used to run more tasks.  */
	ಗುಂಪು_has_ಹೆಚ್ಚುವರಿ = 0,
	/*
	 * The group is fully used and the tasks don't compete for more CPU
	 * cycles. Nevertheless, some tasks might wait before running.
	 */
	ಗುಂಪು_fully_busy,
	/*
	 * One task doesn't fit with CPU's capacity and must be migrated to a
	 * more powerful CPU.
	 */
	ಗುಂಪು_misfit_task,
	/*
	 * Balance SMT group that's fully busy. Can benefit from migration
	 * a task on SMT with busy sibling to another CPU on idle core.
	 */
	ಗುಂಪು_smt_balance,
	/*
	 * SD_ASYM_PACKING only: One local CPU with higher capacity is available,
	 * and the task should be migrated to it instead of running on the
	 * current CPU.
	 */
	ಗುಂಪು_asym_packing,
	/*
	 * The tasks' affinity constraints previously prevented the scheduler
	 * from balancing the load across the system.
	 */
	ಗುಂಪು_imbalanced,
	/*
	 * There are tasks running on non-preferred LLC, possible to move
	 * them to their preferred LLC without creating too much imbalance.
	 * The priority of group_llc_balance is lower than that of
	 * group_overloaded and higher than that of all other group types.
	 * This is because group_llc_balance may exacerbate load imbalance.
	 * If the LLC balancing attempt fails, the nr_balance_failed
	 * mechanism will trigger other group types to rebalance the load.
	 */
	ಗುಂಪು_llc_balance,
	/*
	 * The CPU is overloaded and can't provide expected CPU cycles to all
	 * tasks.
	 */
	ಗುಂಪು_overloaded
};

ಎಣಿಕೆ migration_type {
	migrate_ತುಂಬು = 0,
	migrate_util,
	migrate_task,
	migrate_misfit,
	migrate_llc_task
};

#ವ್ಯಾಖ್ಯಾನಿಸು LBF_ALL_PINNED	0x01
#ವ್ಯಾಖ್ಯಾನಿಸು LBF_NEED_BREAK	0x02
#ವ್ಯಾಖ್ಯಾನಿಸು LBF_DST_PINNED  0x04
#ವ್ಯಾಖ್ಯಾನಿಸು LBF_SOME_PINNED	0x08
#ವ್ಯಾಖ್ಯಾನಿಸು LBF_ಸಕ್ರಿಯ_LB	0x10
#ವ್ಯಾಖ್ಯಾನಿಸು LBF_LLC_PINNED	0x20
#ವ್ಯಾಖ್ಯಾನಿಸು LBF_ಸಕ್ರಿಯ_LB_LLC	0x40

ರಚನೆ lb_env {
	ರಚನೆ sched_domain	*sd;

	ರಚನೆ rq		*src_rq;
	ಪೂರ್ಣಾಂಕ			src_cpu;

	ಪೂರ್ಣಾಂಕ			dst_cpu;
	ರಚನೆ rq		*dst_rq;
	bool			dst_core_idle;

	ರಚನೆ cpumask		*dst_grpmask;
	ಪೂರ್ಣಾಂಕ			new_dst_cpu;
	ಎಣಿಕೆ cpu_idle_type	idle;
	ದೀರ್ಘ			imbalance;
	/* The set of CPUs under consideration for load-balancing */
	ರಚನೆ cpumask		*cpus;

	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		flags;

	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		loop;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		loop_break;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ		loop_max;

	ಎಣಿಕೆ fbq_type		fbq_type;
	ಎಣಿಕೆ migration_type	migration_type;
	ರಚನೆ list_ತಲೆ	tasks;
};

/*
 * Is this task likely cache-hot:
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ task_hot(ರಚನೆ task_struct *p, ರಚನೆ lb_env *env)
{
	s64 delta;

	lockdep_assert_rq_held(env->src_rq);

	ಆದರೆ (p->sched_class != &fair_sched_class)
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (unlikely(task_has_idle_policy(p)))
		ಹಿಂತಿರುಗಿ 0;

	/* SMT siblings share cache */
	ಆದರೆ (env->sd->flags & SD_SHARE_CPUCAPACITY)
		ಹಿಂತಿರುಗಿ 0;

	/*
	 * Buddy candidates are cache hot:
	 */
	ಆದರೆ (sched_feat(CACHE_HOT_BUDDY) && env->dst_rq->nr_running &&
	    (&p->se == cfs_rq_ದ(&p->se)->ಮುಂದಿನ))
		ಹಿಂತಿರುಗಿ 1;

	ಆದರೆ (sysctl_sched_migration_ವೆಚ್ಚ == -1)
		ಹಿಂತಿರುಗಿ 1;

	/*
	 * Don't migrate task if the task's cookie does not match
	 * with the destination CPU's core cookie.
	 */
	ಆದರೆ (!sched_core_cookie_ಹೊಂದಿಕೆ(cpu_rq(env->dst_cpu), p))
		ಹಿಂತಿರುಗಿ 1;

	ಆದರೆ (sysctl_sched_migration_ವೆಚ್ಚ == 0)
		ಹಿಂತಿರುಗಿ 0;

	delta = rq_clock_task(env->src_rq) - p->se.exec_ಆರಂಭ;

	ಹಿಂತಿರುಗಿ delta < (s64)sysctl_sched_migration_ವೆಚ್ಚ;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_NUMA_BALANCING
/*
 * Returns a positive value, if task migration degrades locality.
 * Returns 0, if task migration is not affected by locality.
 * Returns a negative value, if task migration improves locality i.e migration preferred.
 */
ಸ್ಥಿರ ದೀರ್ಘ migrate_degrades_locality(ರಚನೆ task_struct *p, ರಚನೆ lb_env *env)
{
	ರಚನೆ numa_ಗುಂಪು *numa_ಗುಂಪು = rcu_dereference_all(p->numa_ಗುಂಪು);
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ src_ತೂಕ, dst_ತೂಕ;
	ಪೂರ್ಣಾಂಕ src_nid, dst_nid, dist;

	ಆದರೆ (!static_branch_likely(&sched_numa_balancing))
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (!p->numa_ದೋಷಗಳು || !(env->sd->flags & SD_NUMA))
		ಹಿಂತಿರುಗಿ 0;

	src_nid = cpu_ಗಮ್ಯ_ಗಂಟು(env->src_cpu);
	dst_nid = cpu_ಗಮ್ಯ_ಗಂಟು(env->dst_cpu);

	ಆದರೆ (src_nid == dst_nid)
		ಹಿಂತಿರುಗಿ 0;

	/* Migrating away from the preferred node is always bad. */
	ಆದರೆ (src_nid == p->numa_preferred_nid) {
		ಆದರೆ (env->src_rq->nr_running > env->src_rq->nr_preferred_running)
			ಹಿಂತಿರುಗಿ 1;
		ಇಲ್ಲದಿದ್ದರೆ
			ಹಿಂತಿರುಗಿ 0;
	}

	/* Encourage migration to the preferred node. */
	ಆದರೆ (dst_nid == p->numa_preferred_nid)
		ಹಿಂತಿರುಗಿ -1;

	/* Leaving a core idle is often worse than degrading locality. */
	ಆದರೆ (env->idle == CPU_IDLE)
		ಹಿಂತಿರುಗಿ 0;

	dist = ಗಂಟು_ದೂರ(src_nid, dst_nid);
	ಆದರೆ (numa_ಗುಂಪು) {
		src_ತೂಕ = ಗುಂಪು_ತೂಕ(p, src_nid, dist);
		dst_ತೂಕ = ಗುಂಪು_ತೂಕ(p, dst_nid, dist);
	} ಇಲ್ಲದಿದ್ದರೆ {
		src_ತೂಕ = task_ತೂಕ(p, src_nid, dist);
		dst_ತೂಕ = task_ತೂಕ(p, dst_nid, dist);
	}

	ಹಿಂತಿರುಗಿ src_ತೂಕ - dst_ತೂಕ;
}

#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_NUMA_BALANCING: */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ದೀರ್ಘ migrate_degrades_locality(ರಚನೆ task_struct *p,
					     ರಚನೆ lb_env *env)
{
	ಹಿಂತಿರುಗಿ 0;
}
#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_NUMA_BALANCING */

/*
 * Check whether the task is ineligible on the destination cpu
 *
 * When the PLACE_LAG scheduling feature is enabled and
 * dst_cfs_rq->nr_queued is greater than 1, if the task
 * is ineligible, it will also be ineligible when
 * it is migrated to the destination cpu.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ task_is_ineligible_on_dst_cpu(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ dest_cpu)
{
	ರಚನೆ cfs_rq *dst_cfs_rq = &cpu_rq(dest_cpu)->cfs;

	ಆದರೆ (sched_feat(ಇರಿಸು_LAG) && dst_cfs_rq->h_nr_queued &&
	    !entity_eligible(&task_rq(p)->cfs, &p->se))
		ಹಿಂತಿರುಗಿ 1;

	ಹಿಂತಿರುಗಿ 0;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CACHE
/*
 * The margin used when comparing LLC utilization with CPU capacity.
 * It determines the LLC load level where active LLC aggregation is
 * done.
 * Derived from fits_capacity().
 *
 * (default: ~50%, tunable via debugfs)
 */
ಸ್ಥಿರ bool ಹೊಂದುತ್ತದೆ_llc_ಸಾಮರ್ಥ್ಯ(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ util, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ max)
{
	u32 aggr_pct = llc_overaggr_pct;

	/*
	 * For single core systems, raise the aggregation
	 * threshold to accommodate more tasks.
	 */
	ಆದರೆ (cpu_smt_ಅಂಕೆ_ಎಳೆಗಳು == 1)
		aggr_pct = (aggr_pct * 3 / 2);

	ಹಿಂತಿರುಗಿ util * 100 < max * aggr_pct;
}

/*
 * The margin used when comparing utilization.
 * is 'util1' noticeably greater than 'util2'
 * Derived from capacity_greater().
 * Bias is in perentage.
 */
/* Allows dst util to be bigger than src util by up to bias percent */
#ವ್ಯಾಖ್ಯಾನಿಸು util_greater(util1, util2) \
	((util1) * 100 > (util2) * (100 + llc_imb_pct))

ಸ್ಥಿರ __maybe_unused bool get_llc_stats(ಪೂರ್ಣಾಂಕ cpu, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ *util,
					 ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ *cap)
{
	ರಚನೆ sched_domain_shared *sd_share;

	sd_share = rcu_dereference_all(ಪ್ರತಿ_cpu(sd_llc_shared, cpu));
	ಆದರೆ (!sd_share)
		ಹಿಂತಿರುಗಿ false;

	*util = READ_ONCE(sd_share->util_avg);
	*cap = READ_ONCE(sd_share->ಸಾಮರ್ಥ್ಯ);

	ಹಿಂತಿರುಗಿ true;
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
ಎಣಿಕೆ llc_mig {
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
ಸ್ಥಿರ ಎಣಿಕೆ llc_mig can_migrate_llc(ಪೂರ್ಣಾಂಕ src_cpu, ಪೂರ್ಣಾಂಕ dst_cpu,
				    ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ tsk_util,
				    bool ಗಮ್ಯ_pref)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ src_util, dst_util, src_cap, dst_cap;

	ಆದರೆ (!get_llc_stats(src_cpu, &src_util, &src_cap) ||
	    !get_llc_stats(dst_cpu, &dst_util, &dst_cap))
		ಹಿಂತಿರುಗಿ mig_unrestricted;

	src_util = src_util < tsk_util ? 0 : src_util - tsk_util;
	dst_util = dst_util + tsk_util;

	ಆದರೆ (!ಹೊಂದುತ್ತದೆ_llc_ಸಾಮರ್ಥ್ಯ(dst_util, dst_cap) &&
	    !ಹೊಂದುತ್ತದೆ_llc_ಸಾಮರ್ಥ್ಯ(src_util, src_cap))
		ಹಿಂತಿರುಗಿ mig_unrestricted;

	ಆದರೆ (ಗಮ್ಯ_pref) {
		/*
		 * Don't migrate if we will get preferred LLC too
		 * heavily loaded and if the dest is much busier
		 * than the src, in which case migration will
		 * increase the imbalance too much.
		 */
		ಆದರೆ (!ಹೊಂದುತ್ತದೆ_llc_ಸಾಮರ್ಥ್ಯ(dst_util, dst_cap) &&
		    util_greater(dst_util, src_util))
			ಹಿಂತಿರುಗಿ mig_forbid;
	} ಇಲ್ಲದಿದ್ದರೆ {
		/*
		 * Don't migrate if we will leave preferred LLC
		 * too idle, or if this migration leads to the
		 * non-preferred LLC falls within sysctl_aggr_imb percent
		 * of preferred LLC, leading to migration again
		 * back to preferred LLC.
		 */
		ಆದರೆ (ಹೊಂದುತ್ತದೆ_llc_ಸಾಮರ್ಥ್ಯ(src_util, src_cap) ||
		    !util_greater(src_util, dst_util))
			ಹಿಂತಿರುಗಿ mig_forbid;
	}
	ಹಿಂತಿರುಗಿ mig_llc;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool task_misfits_asym_cpu(ರಚನೆ lb_env *env, ರಚನೆ task_struct *p)
{
	/*
	 * On asymmetric CPU capacity domains, do not let cache-aware
	 * balancing pull the task onto a destination CPU that cannot
	 * accommodate it. Doing so would turn the task into a misfit on
	 * the destination, trading a cache-locality gain for a capacity
	 * loss. If the task already does not fit its source CPU, the move
	 * cannot make things worse, so let the LLC preference decide.
	 */
	ಆದರೆ ((env->sd->flags & SD_ASYM_CPUCAPACITY) && p &&
	    !task_ಹೊಂದುತ್ತದೆ_cpu(p, env->dst_cpu) &&
	    task_ಹೊಂದುತ್ತದೆ_cpu(p, env->src_cpu))
		ಹಿಂತಿರುಗಿ true;

	ಹಿಂತಿರುಗಿ false;
}

/*
 * Check if task p can migrate from source LLC to
 * destination LLC in terms of cache aware load balance.
 */
ಸ್ಥಿರ ಎಣಿಕೆ llc_mig can_migrate_llc_task(ರಚನೆ lb_env *env,
					 ರಚನೆ task_struct *p)
{
	ರಚನೆ sched_cache_ಗುಂಪು *grp;
	bool ಗಮ್ಯ_pref;
	ಪೂರ್ಣಾಂಕ cpu, src_cpu, dst_cpu;

	ಆದರೆ (task_misfits_asym_cpu(env, p))
		ಹಿಂತಿರುಗಿ mig_forbid;

	src_cpu = env->src_cpu;
	dst_cpu = env->dst_cpu;
	grp = rcu_dereference_all(p->sched_cache_grp);
	ಆದರೆ (!grp)
		ಹಿಂತಿರುಗಿ mig_unrestricted;

	cpu = READ_ONCE(grp->cpu);
	ಆದರೆ (cpu < 0 || cpus_share_cache(src_cpu, dst_cpu))
		ಹಿಂತಿರುಗಿ mig_unrestricted;

	/* skip cache aware load balance for too many threads */
	ಆದರೆ (invalid_llc_nr(grp, p, dst_cpu) ||
	    exceed_llc_ಸಾಮರ್ಥ್ಯ(grp, dst_cpu)) {
		ಆದರೆ (READ_ONCE(grp->cpu) != -1)
			WRITE_ONCE(grp->cpu, -1);
		ಹಿಂತಿರುಗಿ mig_unrestricted;
	}

	ಆದರೆ (cpus_share_cache(dst_cpu, cpu))
		ಗಮ್ಯ_pref = true;
	ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (cpus_share_cache(src_cpu, cpu))
		ಗಮ್ಯ_pref = false;
	ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ mig_unrestricted;

	ಹಿಂತಿರುಗಿ can_migrate_llc(src_cpu, dst_cpu,
			       task_util(p), ಗಮ್ಯ_pref);
}

/*
 * Check if active load balance breaks LLC locality in
 * terms of cache aware load balance. The load level and
 * imbalance do not warrant breaking LLC preference per
 * the can_migrate_llc() policy. Here, the benefit of
 * LLC locality outweighs the power efficiency gained from
 * migrating the only runnable task away.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
alb_break_llc(ರಚನೆ lb_env *env)
{
	ಆದರೆ (!sched_cache_enabled())
		ಹಿಂತಿರುಗಿ false;

	ಆದರೆ (cpus_share_cache(env->src_cpu, env->dst_cpu))
		ಹಿಂತಿರುಗಿ false;
	/*
	 * All tasks prefer to stay on their current CPU.
	 * Do not pull a task from its preferred CPU if:
	 * 1. It is the only task running and does not exceed
	 *    imbalance allowance; OR
	 * 2. Migrating it away from its preferred LLC would violate
	 *    the cache-aware scheduling policy.
	 */
	ಆದರೆ (env->src_rq->nr_pref_llc_running &&
	    env->src_rq->nr_pref_llc_running == env->src_rq->cfs.h_nr_runnable) {
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ util = 0;
		ರಚನೆ task_struct *cur;

		/*
		 * Migrating misfit tasks from current CPU
		 * to CPU with a better fit.
		 * Prioritize that over LLC preference.
		 */
		ಆದರೆ (env->migration_type == migrate_misfit)
			ಹಿಂತಿರುಗಿ false;

		ಆದರೆ (env->src_rq->nr_running <= 1)
			ಹಿಂತಿರುಗಿ true;

		cur = rcu_dereference_all(env->src_rq->curr);
		ಆದರೆ (cur && cur->sched_class == &fair_sched_class)
			util = task_util(cur);

		ಆದರೆ (task_misfits_asym_cpu(env, cur) ||
		    can_migrate_llc(env->src_cpu, env->dst_cpu,
				    util, false) == mig_forbid)
			ಹಿಂತಿರುಗಿ true;
	}

	ಹಿಂತಿರುಗಿ false;
}

/*
 * Returns true if p's preferred LLC does not match the destination CPU
 * under migrate_llc_task semantics. Passive LB passes migrate_llc_task
 * in env->migration_type, while active LB carries LBF_ACTIVE_LB_LLC in
 * env->flags to avoid overwriting env->migration_type.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
migrate_llc_task_wrong_dst(ರಚನೆ task_struct *p, ರಚನೆ lb_env *env)
{
	ಹಿಂತಿರುಗಿ sched_cache_enabled() &&
	       (env->migration_type == migrate_llc_task ||
		env->flags & LBF_ಸಕ್ರಿಯ_LB_LLC) &&
	       READ_ONCE(p->preferred_llc) != llc_id(env->dst_cpu);
}

/*
 * Check if migrating task p from env->src_cpu to
 * env->dst_cpu breaks LLC localiy.
 */
ಸ್ಥಿರ bool migrate_degrades_llc(ರಚನೆ task_struct *p, ರಚನೆ lb_env *env)
{
	ಆದರೆ (!sched_cache_enabled())
		ಹಿಂತಿರುಗಿ false;

	ಆದರೆ (task_has_sched_core(p))
		ಹಿಂತಿರುಗಿ false;
	/*
	 * Skip over tasks that would degrade LLC locality;
	 * only when nr_balanced_failed is sufficiently high do we
	 * ignore this constraint.
	 *
	 * Threshold of cache_nice_tries is set to 1 higher
	 * than nr_balance_failed to avoid excessive task
	 * migration at the same time.
	 */
	ಆದರೆ (env->sd->nr_balance_failed >= env->sd->cache_nice_tries + 1)
		ಹಿಂತಿರುಗಿ false;

	/*
	 * We know the env->src_cpu has some tasks prefer to
	 * run on env->dst_cpu, skip the tasks do not prefer
	 * env->dst_cpu, and find the one that prefers.
	 */
	ಆದರೆ (migrate_llc_task_wrong_dst(p, env))
		ಹಿಂತಿರುಗಿ true;

	ಆದರೆ (can_migrate_llc_task(env, p) != mig_forbid)
		ಹಿಂತಿರುಗಿ false;

	ಹಿಂತಿರುಗಿ true;
}

#ಇಲ್ಲದಿದ್ದರೆ
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool get_llc_stats(ಪೂರ್ಣಾಂಕ cpu, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ *util,
				 ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ *cap)
{
	ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
alb_break_llc(ರಚನೆ lb_env *env)
{
	ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
migrate_llc_task_wrong_dst(ರಚನೆ task_struct *p, ರಚನೆ lb_env *env)
{
	ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
migrate_degrades_llc(ರಚನೆ task_struct *p, ರಚನೆ lb_env *env)
{
	ಹಿಂತಿರುಗಿ false;
}
#ಆದರೆ_ಅಂತ್ಯ
/*
 * can_migrate_task - may task p from runqueue rq be migrated to this_cpu?
 */
ಸ್ಥಿರ
ಪೂರ್ಣಾಂಕ can_migrate_task(ರಚನೆ task_struct *p, ರಚನೆ lb_env *env)
{
	ದೀರ್ಘ degrades, hot;

	lockdep_assert_rq_held(env->src_rq);
	ಆದರೆ (p->sched_task_hot)
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
	ಆದರೆ ((p->se.sched_delayed) && (env->migration_type != migrate_ತುಂಬು))
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (lb_throttled_hierarchy(p, env->dst_cpu))
		ಹಿಂತಿರುಗಿ 0;

	/*
	 * We want to prioritize the migration of eligible tasks.
	 * For ineligible tasks we soft-limit them and only allow
	 * them to migrate when nr_balance_failed is non-zero to
	 * avoid load-balancing trying very hard to balance the load.
	 */
	ಆದರೆ (!env->sd->nr_balance_failed &&
	    task_is_ineligible_on_dst_cpu(p, env->dst_cpu))
		ಹಿಂತಿರುಗಿ 0;

	/* Disregard percpu kthreads; they are where they need to be. */
	ಆದರೆ (kthread_is_ಪ್ರತಿ_cpu(p))
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (task_is_blocked(p))
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (!cpumask_test_cpu(env->dst_cpu, p->cpus_ptr)) {
		ಪೂರ್ಣಾಂಕ cpu;

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
		ಆದರೆ (env->idle == CPU_NEWLY_IDLE ||
		    env->flags & (LBF_DST_PINNED | LBF_ಸಕ್ರಿಯ_LB))
			ಹಿಂತಿರುಗಿ 0;

		/* Prevent to re-select dst_cpu via env's CPUs: */
		cpu = cpumask_ಮೊದಲ_and_and(env->dst_grpmask, env->cpus, p->cpus_ptr);

		ಆದರೆ (cpu < nr_cpu_ids) {
			env->flags |= LBF_DST_PINNED;
			env->new_dst_cpu = cpu;
		}

		ಹಿಂತಿರುಗಿ 0;
	}

	/* Record that we found at least one task that could run on dst_cpu */
	env->flags &= ~LBF_ALL_PINNED;

	ಆದರೆ (task_on_cpu(env->src_rq, p) ||
	    task_current_donor(env->src_rq, p)) {
		schedstat_inc(p->stats.nr_failed_migrations_running);
		ಹಿಂತಿರುಗಿ 0;
	}

	/*
	 * Aggressive migration if:
	 * 1) active balance
	 * 2) destination numa is preferred
	 * 3) task is cache cold, or
	 * 4) too many balance attempts have failed.
	 */
	ಆದರೆ (env->flags & LBF_ಸಕ್ರಿಯ_LB)
		ಹಿಂತಿರುಗಿ !migrate_llc_task_wrong_dst(p, env);

	degrades = migrate_degrades_locality(p, env);
	ಆದರೆ (!degrades) {
		/*
		 * If the NUMA locality is not broken,
		 * further check if migration would hurt
		 * LLC locality.
		 */
		ಆದರೆ (migrate_degrades_llc(p, env)) {
			/*
			 * If regular load balancing fails to pull a task
			 * due to LLC locality, this is expected behavior
			 * and we set LBF_LLC_PINNED so we don't increase
			 * nr_balance_failed unecessarily.
			 */
			ಆದರೆ (env->migration_type != migrate_llc_task)
				env->flags |= LBF_LLC_PINNED;

			ಹಿಂತಿರುಗಿ 0;
		}

		hot = task_hot(p, env);
	} ಇಲ್ಲದಿದ್ದರೆ {
		hot = degrades > 0;
	}

	ಆದರೆ (!hot || env->sd->nr_balance_failed > env->sd->cache_nice_tries) {
		ಆದರೆ (hot)
			p->sched_task_hot = 1;
		ಹಿಂತಿರುಗಿ 1;
	}

	schedstat_inc(p->stats.nr_failed_migrations_hot);
	ಹಿಂತಿರುಗಿ 0;
}

/*
 * detach_task() -- detach the task for the migration specified in env
 */
ಸ್ಥಿರ ಶೂನ್ಯ detach_task(ರಚನೆ task_struct *p, ರಚನೆ lb_env *env)
{
	lockdep_assert_rq_held(env->src_rq);

	ಆದರೆ (p->sched_task_hot) {
		p->sched_task_hot = 0;
		schedstat_inc(env->sd->lb_hot_gained[env->idle]);
		schedstat_inc(p->stats.nr_forced_migrations);
	}

	WARN_ON(task_current(env->src_rq, p));
	WARN_ON(task_current_donor(env->src_rq, p));

	deactivate_task(env->src_rq, p, ಸರತಿಯಿಂದ_ತೆಗೆ_NOCLOCK);
	ಗಣ_task_cpu(p, env->dst_cpu);
}

/*
 * detach_one_task() -- tries to dequeue exactly one task from env->src_rq, as
 * part of active balancing operations within "domain".
 *
 * Returns a task if successful and NULL otherwise.
 */
ಸ್ಥಿರ ರಚನೆ task_struct *detach_one_task(ರಚನೆ lb_env *env)
{
	ರಚನೆ task_struct *p;

	lockdep_assert_rq_held(env->src_rq);

	list_for_each_ನಮೂದು_ಹಿಮ್ಮುಖ(p,
			&env->src_rq->cfs_tasks, se.ಗುಂಪು_ಗಂಟು) {
		ಆದರೆ (!can_migrate_task(p, env))
			ಮುಂದುವರಿ;

		detach_task(p, env);

		/*
		 * Right now, this is only the second place where
		 * lb_gained[env->idle] is updated (other is detach_tasks)
		 * so we can safely collect stats here rather than
		 * inside detach_tasks().
		 */
		schedstat_inc(env->sd->lb_gained[env->idle]);
		ಹಿಂತಿರುಗಿ p;
	}
	ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

/*
 * detach_tasks() -- tries to detach up to imbalance load/util/tasks from
 * busiest_rq, as part of a balancing operation within domain "sd".
 *
 * Returns number of detached tasks if successful and 0 otherwise.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ detach_tasks(ರಚನೆ lb_env *env)
{
	ರಚನೆ list_ತಲೆ *tasks = &env->src_rq->cfs_tasks;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ util, ತುಂಬು;
	ರಚನೆ task_struct *p;
	ಪೂರ್ಣಾಂಕ detached = 0;

	lockdep_assert_rq_held(env->src_rq);

	/*
	 * Source run queue has been emptied by another CPU, clear
	 * LBF_ALL_PINNED flag as we will not test any task.
	 */
	ಆದರೆ (env->src_rq->nr_running <= 1) {
		env->flags &= ~LBF_ALL_PINNED;
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (env->imbalance <= 0)
		ಹಿಂತಿರುಗಿ 0;

	ಆಗ (!list_ಖಾಲಿಗಳು(tasks)) {
		/*
		 * We don't want to steal all, otherwise we may be treated likewise,
		 * which could at worst lead to a livelock crash.
		 */
		ಆದರೆ (env->idle && env->src_rq->nr_running <= 1)
			ನಿಲ್ಲಿಸು;

		env->loop++;
		/* We've more or less seen every task there is, call it quits */
		ಆದರೆ (env->loop > env->loop_max)
			ನಿಲ್ಲಿಸು;

		/* take a breather every nr_migrate tasks */
		ಆದರೆ (env->loop > env->loop_break) {
			env->loop_break += SCHED_NR_MIGRATE_BREAK;
			env->flags |= LBF_NEED_BREAK;
			ನಿಲ್ಲಿಸು;
		}

		p = list_ಕೊನೆಯ_ನಮೂದು(tasks, ರಚನೆ task_struct, se.ಗುಂಪು_ಗಂಟು);

		ಆದರೆ (!can_migrate_task(p, env))
			ಹೋಗು ಮುಂದಿನ;

		ಆಯ್ಕೆ (env->migration_type) {
		ಸ್ಥಿತಿ migrate_ತುಂಬು:
			/*
			 * Depending of the number of CPUs and tasks and the
			 * cgroup hierarchy, task_h_load() can return a null
			 * value. Make sure that env->imbalance decreases
			 * otherwise detach_tasks() will stop only after
			 * detaching up to loop_max tasks.
			 */
			ತುಂಬು = max_t(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ, task_h_ತುಂಬು(p), 1);

			ಆದರೆ (sched_feat(LB_MIN) &&
			    ತುಂಬು < 16 && !env->sd->nr_balance_failed)
				ಹೋಗು ಮುಂದಿನ;

			/*
			 * Make sure that we don't migrate too much load.
			 * Nevertheless, let relax the constraint if
			 * scheduler fails to find a good waiting task to
			 * migrate.
			 */
			ಆದರೆ (shr_bound(ತುಂಬು, env->sd->nr_balance_failed) > env->imbalance)
				ಹೋಗು ಮುಂದಿನ;

			env->imbalance -= ತುಂಬು;
			ನಿಲ್ಲಿಸು;

		ಸ್ಥಿತಿ migrate_util:
			util = task_util_est(p);

			ಆದರೆ (shr_bound(util, env->sd->nr_balance_failed) > env->imbalance)
				ಹೋಗು ಮುಂದಿನ;

			env->imbalance -= util;
			ನಿಲ್ಲಿಸು;

		ಸ್ಥಿತಿ migrate_task:
			env->imbalance--;
			ನಿಲ್ಲಿಸು;

		ಸ್ಥಿತಿ migrate_misfit:
			/* This is not a misfit task */
			ಆದರೆ (task_ಹೊಂದುತ್ತದೆ_cpu(p, env->src_cpu))
				ಹೋಗು ಮುಂದಿನ;

			env->imbalance = 0;
			ನಿಲ್ಲಿಸು;

		ಸ್ಥಿತಿ migrate_llc_task:
			env->imbalance--;
			ನಿಲ್ಲಿಸು;
		}

		detach_task(p, env);
		list_ಕೂಡಿಸು(&p->se.ಗುಂಪು_ಗಂಟು, &env->tasks);

		detached++;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_PREEMPTION
		/*
		 * NEWIDLE balancing is a source of latency, so preemptible
		 * kernels will stop after the first task is detached to minimize
		 * the critical section.
		 */
		ಆದರೆ (env->idle == CPU_NEWLY_IDLE)
			ನಿಲ್ಲಿಸು;
#ಆದರೆ_ಅಂತ್ಯ

		/*
		 * We only want to steal up to the prescribed amount of
		 * load/util/tasks.
		 */
		ಆದರೆ (env->imbalance <= 0)
			ನಿಲ್ಲಿಸು;

		ಮುಂದುವರಿ;
ಮುಂದಿನ:
		ಆದರೆ (p->sched_task_hot)
			schedstat_inc(p->stats.nr_failed_migrations_hot);

		list_move(&p->se.ಗುಂಪು_ಗಂಟು, tasks);
	}

	/*
	 * Right now, this is one of only two places we collect this stat
	 * so we can safely collect detach_one_task() stats here rather
	 * than inside detach_one_task().
	 */
	schedstat_ಕೂಡಿಸು(env->sd->lb_gained[env->idle], detached);

	ಹಿಂತಿರುಗಿ detached;
}

/*
 * attach_tasks() -- attaches all tasks detached by detach_tasks() to their
 * new rq.
 */
ಸ್ಥಿರ ಶೂನ್ಯ attach_tasks(ರಚನೆ lb_env *env)
{
	__attach_tasks(env->dst_rq, &env->tasks);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_NO_HZ_COMMON
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool cfs_rq_has_blocked_ತುಂಬು(ರಚನೆ cfs_rq *cfs_rq)
{
	ಆದರೆ (cfs_rq->avg.ತುಂಬು_avg)
		ಹಿಂತಿರುಗಿ true;

	ಆದರೆ (cfs_rq->avg.util_avg)
		ಹಿಂತಿರುಗಿ true;

	ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool others_have_blocked(ರಚನೆ rq *rq)
{
	ಆದರೆ (cpu_util_rt(rq))
		ಹಿಂತಿರುಗಿ true;

	ಆದರೆ (cpu_util_dl(rq))
		ಹಿಂತಿರುಗಿ true;

	ಆದರೆ (hw_ತುಂಬು_avg(rq))
		ಹಿಂತಿರುಗಿ true;

	ಆದರೆ (cpu_util_irq(rq))
		ಹಿಂತಿರುಗಿ true;

	ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ update_blocked_ತುಂಬು_tick(ರಚನೆ rq *rq)
{
	WRITE_ONCE(rq->ಕೊನೆಯ_blocked_ತುಂಬು_update_tick, jiffies);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ update_has_blocked_ತುಂಬು_ಸ್ಥಿತಿ(ರಚನೆ rq *rq, bool has_blocked_ತುಂಬು)
{
	ಆದರೆ (!has_blocked_ತುಂಬು)
		rq->has_blocked_ತುಂಬು = 0;
}
#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_NO_HZ_COMMON: */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool cfs_rq_has_blocked_ತುಂಬು(ರಚನೆ cfs_rq *cfs_rq) { ಹಿಂತಿರುಗಿ false; }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool others_have_blocked(ರಚನೆ rq *rq) { ಹಿಂತಿರುಗಿ false; }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ update_blocked_ತುಂಬು_tick(ರಚನೆ rq *rq) {}
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ update_has_blocked_ತುಂಬು_ಸ್ಥಿತಿ(ರಚನೆ rq *rq, bool has_blocked_ತುಂಬು) {}
#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_NO_HZ_COMMON */

ಸ್ಥಿರ bool __update_blocked_others(ರಚನೆ rq *rq, bool *ಮುಗಿದ)
{
	bool updated;

	/*
	 * update_load_avg() can call cpufreq_update_util(). Make sure that RT,
	 * DL and IRQ signals have been updated before updating CFS.
	 */
	updated = update_other_ತುಂಬು_avgs(rq);

	ಆದರೆ (others_have_blocked(rq))
		*ಮುಗಿದ = false;

	ಹಿಂತಿರುಗಿ updated;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_FAIR_ಗುಂಪು_SCHED

ಸ್ಥಿರ bool __update_blocked_fair(ರಚನೆ rq *rq, bool *ಮುಗಿದ)
{
	ರಚನೆ cfs_rq *cfs_rq, *pos;
	bool decayed = false;

	/*
	 * Iterates the task_group tree in a bottom up fashion, see
	 * list_add_leaf_cfs_rq() for details.
	 */
	for_each_ಎಲೆ_cfs_rq_ಸುರಕ್ಷಿತ(rq, cfs_rq, pos) {
		ರಚನೆ sched_entity *se;

		ಆದರೆ (update_cfs_rq_ತುಂಬು_avg(cfs_rq_clock_pelt(cfs_rq), cfs_rq)) {
			update_tg_ತುಂಬು_avg(cfs_rq);

			ಆದರೆ (cfs_rq->nr_queued == 0)
				update_idle_cfs_rq_clock_pelt(cfs_rq);

			ಆದರೆ (cfs_rq == &rq->cfs)
				decayed = true;
		}

		/* Propagate pending load changes to the parent, if any: */
		se = cfs_rq_se(cfs_rq);
		ಆದರೆ (se && !skip_blocked_update(se))
			update_ತುಂಬು_avg(cfs_rq_ದ(se), se, UPDATE_TG);

		/*
		 * There can be a lot of idle CPU cgroups.  Don't let fully
		 * decayed cfs_rqs linger on the list.
		 */
		ಆದರೆ (cfs_rq_is_decayed(cfs_rq))
			list_del_ಎಲೆ_cfs_rq(cfs_rq);

		/* Don't need periodic decay once load/util_avg are null */
		ಆದರೆ (cfs_rq_has_blocked_ತುಂಬು(cfs_rq))
			*ಮುಗಿದ = false;
	}

	ಹಿಂತಿರುಗಿ decayed;
}

/*
 * Compute the hierarchical load factor for cfs_rq and all its ascendants.
 * This needs to be done in a top-down fashion because the load of a child
 * group is a fraction of its parents load.
 */
ಸ್ಥಿರ ಶೂನ್ಯ update_cfs_rq_h_ತುಂಬು(ರಚನೆ cfs_rq *cfs_rq)
{
	ರಚನೆ sched_entity *se = cfs_rq_se(cfs_rq);
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ now = jiffies;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ತುಂಬು;

	ಆದರೆ (cfs_rq->ಕೊನೆಯ_h_ತುಂಬು_update == now)
		ಹಿಂತಿರುಗಿ;

	WRITE_ONCE(cfs_rq->h_ತುಂಬು_ಮುಂದಿನ, ಖಾಲಿ);
	for_each_sched_entity(se) {
		cfs_rq = cfs_rq_ದ(se);
		WRITE_ONCE(cfs_rq->h_ತುಂಬು_ಮುಂದಿನ, se);
		ಆದರೆ (cfs_rq->ಕೊನೆಯ_h_ತುಂಬು_update == now)
			ನಿಲ್ಲಿಸು;
	}

	ಆದರೆ (!se) {
		cfs_rq->h_ತುಂಬು = cfs_rq_ತುಂಬು_avg(cfs_rq);
		cfs_rq->ಕೊನೆಯ_h_ತುಂಬು_update = now;
	}

	ಆಗ ((se = READ_ONCE(cfs_rq->h_ತುಂಬು_ಮುಂದಿನ)) != ಖಾಲಿ) {
		ತುಂಬು = cfs_rq->h_ತುಂಬು;
		ತುಂಬು = div64_ul(ತುಂಬು * se->avg.ತುಂಬು_avg,
				cfs_rq_ತುಂಬು_avg(cfs_rq) + 1);
		cfs_rq = ಗುಂಪು_cfs_rq(se);
		cfs_rq->h_ತುಂಬು = ತುಂಬು;
		cfs_rq->ಕೊನೆಯ_h_ತುಂಬು_update = now;
	}
}

ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ task_h_ತುಂಬು(ರಚನೆ task_struct *p)
{
	ರಚನೆ cfs_rq *cfs_rq = task_cfs_rq(p);

	update_cfs_rq_h_ತುಂಬು(cfs_rq);
	ಹಿಂತಿರುಗಿ div64_ul(p->se.avg.ತುಂಬು_avg * cfs_rq->h_ತುಂಬು,
			cfs_rq_ತುಂಬು_avg(cfs_rq) + 1);
}
#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_FAIR_GROUP_SCHED: */
ಸ್ಥಿರ bool __update_blocked_fair(ರಚನೆ rq *rq, bool *ಮುಗಿದ)
{
	ರಚನೆ cfs_rq *cfs_rq = &rq->cfs;
	bool decayed;

	decayed = update_cfs_rq_ತುಂಬು_avg(cfs_rq_clock_pelt(cfs_rq), cfs_rq);
	ಆದರೆ (cfs_rq_has_blocked_ತುಂಬು(cfs_rq))
		*ಮುಗಿದ = false;

	ಹಿಂತಿರುಗಿ decayed;
}

ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ task_h_ತುಂಬು(ರಚನೆ task_struct *p)
{
	ಹಿಂತಿರುಗಿ p->se.avg.ತುಂಬು_avg;
}
#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_FAIR_GROUP_SCHED */

ಸ್ಥಿರ ಶೂನ್ಯ __sched_balance_update_blocked_averages(ರಚನೆ rq *rq)
{
	bool decayed = false, ಮುಗಿದ = true;

	update_blocked_ತುಂಬು_tick(rq);

	decayed |= __update_blocked_others(rq, &ಮುಗಿದ);
	decayed |= __update_blocked_fair(rq, &ಮುಗಿದ);

	update_has_blocked_ತುಂಬು_ಸ್ಥಿತಿ(rq, !ಮುಗಿದ);
	ಆದರೆ (decayed)
		cpufreq_update_util(rq, 0);
}

ಸ್ಥಿರ ಶೂನ್ಯ sched_balance_update_blocked_averages(ಪೂರ್ಣಾಂಕ cpu)
{
	ರಚನೆ rq *rq = cpu_rq(cpu);

	guard(rq_ಬೀಗ_irqsave)(rq);
	update_rq_clock(rq);
	__sched_balance_update_blocked_averages(rq);
}

/********** Helpers for sched_balance_find_src_group ************************/

/*
 * sg_lb_stats - stats of a sched_group required for load-balancing:
 */
ರಚನೆ sg_lb_stats {
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ avg_ತುಂಬು;			/* Avg load            over the CPUs of the group */
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗುಂಪು_ತುಂಬು;		/* Total load          over the CPUs of the group */
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗುಂಪು_ಸಾಮರ್ಥ್ಯ;		/* Capacity            over the CPUs of the group */
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗುಂಪು_util;		/* Total utilization   over the CPUs of the group */
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗುಂಪು_runnable;		/* Total runnable time over the CPUs of the group */
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಮೊತ್ತ_nr_running;		/* Nr of all tasks running in the group */
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಮೊತ್ತ_h_nr_running;		/* Nr of CFS tasks running in the group */
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ idle_cpus;                 /* Nr of idle CPUs         in the group */
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಗುಂಪು_ತೂಕ;
	ಎಣಿಕೆ ಗುಂಪು_type ಗುಂಪು_type;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಗುಂಪು_asym_packing;	/* Tasks should be moved to preferred CPU */
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಗುಂಪು_smt_balance;		/* Task on busy SMT be moved */
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಗುಂಪು_llc_balance;		/* Tasks should be moved to preferred LLC */
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗುಂಪು_misfit_task_ತುಂಬು;	/* A CPU has a task too big for its capacity */
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಗುಂಪು_overutilized;	/* At least one CPU is overutilized in the group */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_NUMA_BALANCING
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ nr_numa_running;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ nr_preferred_running;
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CACHE
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ nr_pref_dst_llc;
#ಆದರೆ_ಅಂತ್ಯ
};

/*
 * sd_lb_stats - stats of a sched_domain required for load-balancing:
 */
ರಚನೆ sd_lb_stats {
	ರಚನೆ sched_ಗುಂಪು *busiest;		/* Busiest group in this sd */
	ರಚನೆ sched_ಗುಂಪು *local;		/* Local group in this sd */
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಒಟ್ಟು_ತುಂಬು;		/* Total load of all groups in sd */
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಒಟ್ಟು_ಸಾಮರ್ಥ್ಯ;		/* Total capacity of all groups in sd */
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ avg_ತುಂಬು;			/* Average load across all groups in sd */
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ prefer_sibling;		/* Tasks should go to sibling first */

	ರಚನೆ sg_lb_stats busiest_stat;	/* Statistics of the busiest group */
	ರಚನೆ sg_lb_stats local_stat;		/* Statistics of the local group */
};

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ init_sd_lb_stats(ರಚನೆ sd_lb_stats *sds)
{
	/*
	 * Skimp on the clearing to avoid duplicate work. We can avoid clearing
	 * local_stat because update_sg_lb_stats() does a full clear/assignment.
	 * We must however set busiest_stat::group_type and
	 * busiest_stat::idle_cpus to the worst busiest group because
	 * update_sd_pick_busiest() reads these before assignment.
	 */
	*sds = (ರಚನೆ sd_lb_stats){
		.busiest = ಖಾಲಿ,
		.local = ಖಾಲಿ,
		.ಒಟ್ಟು_ತುಂಬು = 0UL,
		.ಒಟ್ಟು_ಸಾಮರ್ಥ್ಯ = 0UL,
		.busiest_stat = {
			.idle_cpus = UINT_MAX,
			.ಗುಂಪು_type = ಗುಂಪು_has_ಹೆಚ್ಚುವರಿ,
		},
	};
}

ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ scale_rt_ಸಾಮರ್ಥ್ಯ(ಪೂರ್ಣಾಂಕ cpu)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ max = get_actual_cpu_ಸಾಮರ್ಥ್ಯ(cpu);
	ರಚನೆ rq *rq = cpu_rq(cpu);
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಬಳಸಿದ, ಬಿಡುಗಡೆ;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ irq;

	irq = cpu_util_irq(rq);

	ಆದರೆ (unlikely(irq >= max))
		ಹಿಂತಿರುಗಿ 1;

	/*
	 * avg_rt.util_avg and avg_dl.util_avg track binary signals
	 * (running and not running) with weights 0 and 1024 respectively.
	 */
	ಬಳಸಿದ = cpu_util_rt(rq);
	ಬಳಸಿದ += cpu_util_dl(rq);

	ಆದರೆ (unlikely(ಬಳಸಿದ >= max))
		ಹಿಂತಿರುಗಿ 1;

	ಬಿಡುಗಡೆ = max - ಬಳಸಿದ;

	ಹಿಂತಿರುಗಿ scale_irq_ಸಾಮರ್ಥ್ಯ(ಬಿಡುಗಡೆ, irq, max);
}

ಸ್ಥಿರ ಶೂನ್ಯ update_cpu_ಸಾಮರ್ಥ್ಯ(ರಚನೆ sched_domain *sd, ಪೂರ್ಣಾಂಕ cpu)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಸಾಮರ್ಥ್ಯ = scale_rt_ಸಾಮರ್ಥ್ಯ(cpu);
	ರಚನೆ sched_ಗುಂಪು *sdg = sd->groups;

	ಆದರೆ (!ಸಾಮರ್ಥ್ಯ)
		ಸಾಮರ್ಥ್ಯ = 1;

	cpu_rq(cpu)->cpu_ಸಾಮರ್ಥ್ಯ = ಸಾಮರ್ಥ್ಯ;
	trace_sched_cpu_ಸಾಮರ್ಥ್ಯ_tp(cpu_rq(cpu));

	sdg->sgc->ಸಾಮರ್ಥ್ಯ = ಸಾಮರ್ಥ್ಯ;
	sdg->sgc->min_ಸಾಮರ್ಥ್ಯ = ಸಾಮರ್ಥ್ಯ;
	sdg->sgc->max_ಸಾಮರ್ಥ್ಯ = ಸಾಮರ್ಥ್ಯ;
}

ಶೂನ್ಯ update_ಗುಂಪು_ಸಾಮರ್ಥ್ಯ(ರಚನೆ sched_domain *sd, ಪೂರ್ಣಾಂಕ cpu)
{
	ರಚನೆ sched_domain *ಮಗು = sd->ಮಗು;
	ರಚನೆ sched_ಗುಂಪು *ಗುಂಪು, *sdg = sd->groups;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಸಾಮರ್ಥ್ಯ, min_ಸಾಮರ್ಥ್ಯ, max_ಸಾಮರ್ಥ್ಯ;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ interval;

	interval = msecs_ಗಮ್ಯ_jiffies(sd->balance_interval);
	interval = clamp(interval, 1UL, max_ತುಂಬು_balance_interval);
	sdg->sgc->ಮುಂದಿನ_update = jiffies + interval;

	ಆದರೆ (!ಮಗು) {
		update_cpu_ಸಾಮರ್ಥ್ಯ(sd, cpu);
		ಹಿಂತಿರುಗಿ;
	}

	ಸಾಮರ್ಥ್ಯ = 0;
	min_ಸಾಮರ್ಥ್ಯ = ULONG_MAX;
	max_ಸಾಮರ್ಥ್ಯ = 0;

	ಆದರೆ (ಮಗು->flags & SD_NUMA) {
		/*
		 * SD_NUMA domains cannot assume that child groups
		 * span the current group.
		 */

		for_each_cpu(cpu, sched_ಗುಂಪು_ಹರವು(sdg)) {
			ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ cpu_cap = ಸಾಮರ್ಥ್ಯ_ದ(cpu);

			ಸಾಮರ್ಥ್ಯ += cpu_cap;
			min_ಸಾಮರ್ಥ್ಯ = min(cpu_cap, min_ಸಾಮರ್ಥ್ಯ);
			max_ಸಾಮರ್ಥ್ಯ = max(cpu_cap, max_ಸಾಮರ್ಥ್ಯ);
		}
	} ಇಲ್ಲದಿದ್ದರೆ  {
		/*
		 * !SD_NUMA domains can assume that child groups
		 * span the current group.
		 */

		ಗುಂಪು = ಮಗು->groups;
		ಮಾಡು {
			ರಚನೆ sched_ಗುಂಪು_ಸಾಮರ್ಥ್ಯ *sgc = ಗುಂಪು->sgc;

			ಸಾಮರ್ಥ್ಯ += sgc->ಸಾಮರ್ಥ್ಯ;
			min_ಸಾಮರ್ಥ್ಯ = min(sgc->min_ಸಾಮರ್ಥ್ಯ, min_ಸಾಮರ್ಥ್ಯ);
			max_ಸಾಮರ್ಥ್ಯ = max(sgc->max_ಸಾಮರ್ಥ್ಯ, max_ಸಾಮರ್ಥ್ಯ);
			ಗುಂಪು = ಗುಂಪು->ಮುಂದಿನ;
		} ಆಗ (ಗುಂಪು != ಮಗು->groups);
	}

	sdg->sgc->ಸಾಮರ್ಥ್ಯ = ಸಾಮರ್ಥ್ಯ;
	sdg->sgc->min_ಸಾಮರ್ಥ್ಯ = min_ಸಾಮರ್ಥ್ಯ;
	sdg->sgc->max_ಸಾಮರ್ಥ್ಯ = max_ಸಾಮರ್ಥ್ಯ;
}

/*
 * Check whether the capacity of the rq has been noticeably reduced by side
 * activity. The imbalance_pct is used for the threshold.
 * Return true is the capacity is reduced
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
check_cpu_ಸಾಮರ್ಥ್ಯ(ರಚನೆ rq *rq, ರಚನೆ sched_domain *sd)
{
	ಹಿಂತಿರುಗಿ ((rq->cpu_ಸಾಮರ್ಥ್ಯ * sd->imbalance_pct) <
				(arch_scale_cpu_ಸಾಮರ್ಥ್ಯ(cpu_ದ(rq)) * 100));
}

/* Check if the rq has a misfit task */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool check_misfit_ಸ್ಥಿತಿ(ರಚನೆ rq *rq)
{
	ಹಿಂತಿರುಗಿ rq->misfit_task_ತುಂಬು;
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

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ sg_imbalanced(ರಚನೆ sched_ಗುಂಪು *ಗುಂಪು)
{
	ಹಿಂತಿರುಗಿ ಗುಂಪು->sgc->imbalance;
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
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
ಗುಂಪು_has_ಸಾಮರ್ಥ್ಯ(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ imbalance_pct, ರಚನೆ sg_lb_stats *sgs)
{
	ಆದರೆ (sgs->ಮೊತ್ತ_nr_running < sgs->ಗುಂಪು_ತೂಕ)
		ಹಿಂತಿರುಗಿ true;

	ಆದರೆ ((sgs->ಗುಂಪು_ಸಾಮರ್ಥ್ಯ * imbalance_pct) <
			(sgs->ಗುಂಪು_runnable * 100))
		ಹಿಂತಿರುಗಿ false;

	ಆದರೆ ((sgs->ಗುಂಪು_ಸಾಮರ್ಥ್ಯ * 100) >
			(sgs->ಗುಂಪು_util * imbalance_pct))
		ಹಿಂತಿರುಗಿ true;

	ಹಿಂತಿರುಗಿ false;
}

/*
 *  group_is_overloaded returns true if the group has more tasks than it can
 *  handle.
 *  group_is_overloaded is not equals to !group_has_capacity because a group
 *  with the exact right number of tasks, has no more spare capacity but is not
 *  overloaded so both group_has_capacity and group_is_overloaded return
 *  false.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
ಗುಂಪು_is_overloaded(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ imbalance_pct, ರಚನೆ sg_lb_stats *sgs)
{
	/*
	 * With EAS and uclamp, 1 CPU in the group must be overutilized to
	 * consider the group overloaded.
	 */
	ಆದರೆ (sched_energy_enabled() && !sgs->ಗುಂಪು_overutilized)
		ಹಿಂತಿರುಗಿ false;

	ಆದರೆ (sgs->ಮೊತ್ತ_nr_running <= sgs->ಗುಂಪು_ತೂಕ)
		ಹಿಂತಿರುಗಿ false;

	ಆದರೆ ((sgs->ಗುಂಪು_ಸಾಮರ್ಥ್ಯ * 100) <
			(sgs->ಗುಂಪು_util * imbalance_pct))
		ಹಿಂತಿರುಗಿ true;

	ಆದರೆ ((sgs->ಗುಂಪು_ಸಾಮರ್ಥ್ಯ * imbalance_pct) <
			(sgs->ಗುಂಪು_runnable * 100))
		ಹಿಂತಿರುಗಿ true;

	ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಎಣಿಕೆ
ಗುಂಪು_type ಗುಂಪು_classify(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ imbalance_pct,
			  ರಚನೆ sched_ಗುಂಪು *ಗುಂಪು,
			  ರಚನೆ sg_lb_stats *sgs)
{
	ಆದರೆ (ಗುಂಪು_is_overloaded(imbalance_pct, sgs))
		ಹಿಂತಿರುಗಿ ಗುಂಪು_overloaded;

	ಆದರೆ (sgs->ಗುಂಪು_llc_balance)
		ಹಿಂತಿರುಗಿ ಗುಂಪು_llc_balance;

	ಆದರೆ (sg_imbalanced(ಗುಂಪು))
		ಹಿಂತಿರುಗಿ ಗುಂಪು_imbalanced;

	ಆದರೆ (sgs->ಗುಂಪು_asym_packing)
		ಹಿಂತಿರುಗಿ ಗುಂಪು_asym_packing;

	ಆದರೆ (sgs->ಗುಂಪು_smt_balance)
		ಹಿಂತಿರುಗಿ ಗುಂಪು_smt_balance;

	ಆದರೆ (sgs->ಗುಂಪು_misfit_task_ತುಂಬು)
		ಹಿಂತಿರುಗಿ ಗುಂಪು_misfit_task;

	ಆದರೆ (!ಗುಂಪು_has_ಸಾಮರ್ಥ್ಯ(imbalance_pct, sgs))
		ಹಿಂತಿರುಗಿ ಗುಂಪು_fully_busy;

	ಹಿಂತಿರುಗಿ ಗುಂಪು_has_ಹೆಚ್ಚುವರಿ;
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
ಸ್ಥಿರ bool sched_use_asym_prio(ರಚನೆ sched_domain *sd, ಪೂರ್ಣಾಂಕ cpu)
{
	ಆದರೆ (!(sd->flags & SD_ASYM_PACKING))
		ಹಿಂತಿರುಗಿ false;

	ಆದರೆ (!sched_smt_ಸಕ್ರಿಯ())
		ಹಿಂತಿರುಗಿ true;

	ಹಿಂತಿರುಗಿ sd->flags & SD_SHARE_CPUCAPACITY || is_core_idle(cpu);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool sched_asym(ರಚನೆ sched_domain *sd, ಪೂರ್ಣಾಂಕ dst_cpu, ಪೂರ್ಣಾಂಕ src_cpu)
{
	/*
	 * First check if @dst_cpu can do asym_packing load balance. Only do it
	 * if it has higher priority than @src_cpu.
	 */
	ಹಿಂತಿರುಗಿ sched_use_asym_prio(sd, dst_cpu) &&
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
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
sched_ಗುಂಪು_asym(ರಚನೆ lb_env *env, ರಚನೆ sg_lb_stats *sgs, ರಚನೆ sched_ಗುಂಪು *ಗುಂಪು)
{
	/*
	 * CPU priorities do not make sense for SMT cores with more than one
	 * busy sibling.
	 */
	ಆದರೆ ((ಗುಂಪು->flags & SD_SHARE_CPUCAPACITY) &&
	    (sgs->ಗುಂಪು_ತೂಕ - sgs->idle_cpus != 1))
		ಹಿಂತಿರುಗಿ false;

	ಹಿಂತಿರುಗಿ sched_asym(env->sd, env->dst_cpu, READ_ONCE(ಗುಂಪು->asym_prefer_cpu));
}

/* One group has more than one SMT CPU while the other group does not */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool smt_vs_nonsmt_groups(ರಚನೆ sched_ಗುಂಪು *sg1,
				    ರಚನೆ sched_ಗುಂಪು *sg2)
{
	ಆದರೆ (!sg1 || !sg2)
		ಹಿಂತಿರುಗಿ false;

	ಹಿಂತಿರುಗಿ (sg1->flags & SD_SHARE_CPUCAPACITY) !=
		(sg2->flags & SD_SHARE_CPUCAPACITY);
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool smt_balance(ರಚನೆ lb_env *env, ರಚನೆ sg_lb_stats *sgs,
			       ರಚನೆ sched_ಗುಂಪು *ಗುಂಪು)
{
	ಆದರೆ (!env->idle)
		ಹಿಂತಿರುಗಿ false;

	/*
	 * For SMT source group, it is better to move a task
	 * to a CPU that doesn't have multiple tasks sharing its CPU capacity.
	 * Note that if a group has a single SMT, SD_SHARE_CPUCAPACITY
	 * will not be on.
	 */
	ಆದರೆ (ಗುಂಪು->flags & SD_SHARE_CPUCAPACITY &&
	    sgs->ಮೊತ್ತ_h_nr_running > 1)
		ಹಿಂತಿರುಗಿ true;

	ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ದೀರ್ಘ sibling_imbalance(ರಚನೆ lb_env *env,
				    ರಚನೆ sd_lb_stats *sds,
				    ರಚನೆ sg_lb_stats *busiest,
				    ರಚನೆ sg_lb_stats *local)
{
	ಪೂರ್ಣಾಂಕ ncores_busiest, ncores_local;
	ದೀರ್ಘ imbalance;

	ಆದರೆ (!env->idle || !busiest->ಮೊತ್ತ_nr_running)
		ಹಿಂತಿರುಗಿ 0;

	ncores_busiest = sds->busiest->cores;
	ncores_local = sds->local->cores;

	ಆದರೆ (ncores_busiest == ncores_local) {
		imbalance = busiest->ಮೊತ್ತ_nr_running;
		lsub_ಧನಾತ್ಮಕ(&imbalance, local->ಮೊತ್ತ_nr_running);
		ಹಿಂತಿರುಗಿ imbalance;
	}

	/* Balance such that nr_running/ncores ratio are same on both groups */
	imbalance = ncores_local * busiest->ಮೊತ್ತ_nr_running;
	lsub_ಧನಾತ್ಮಕ(&imbalance, ncores_busiest * local->ಮೊತ್ತ_nr_running);
	/* Normalize imbalance and do rounding on normalization */
	imbalance = 2 * imbalance + ncores_local + ncores_busiest;
	imbalance /= ncores_local + ncores_busiest;

	/* Take advantage of resource in an empty sched group */
	ಆದರೆ (imbalance <= 1 && local->ಮೊತ್ತ_nr_running == 0 &&
	    busiest->ಮೊತ್ತ_nr_running > 1)
		imbalance = 2;

	ಹಿಂತಿರುಗಿ imbalance;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
sched_reduced_ಸಾಮರ್ಥ್ಯ(ರಚನೆ rq *rq, ರಚನೆ sched_domain *sd)
{
	/*
	 * When there is more than 1 task, the group_overloaded case already
	 * takes care of cpu with reduced capacity
	 */
	ಆದರೆ (rq->cfs.h_nr_runnable != 1)
		ಹಿಂತಿರುಗಿ false;

	ಹಿಂತಿರುಗಿ check_cpu_ಸಾಮರ್ಥ್ಯ(rq, sd);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CACHE
/*
 * Record the statistics for this scheduler group for later
 * use. These values guide load balancing on aggregating tasks
 * to a LLC.
 */
ಸ್ಥಿರ ಶೂನ್ಯ ದಾಖಲೆ_sg_llc_stats(ರಚನೆ lb_env *env,
				ರಚನೆ sg_lb_stats *sgs,
				ರಚನೆ sched_ಗುಂಪು *ಗುಂಪು)
{
	ರಚನೆ sched_domain_shared *sd_share;
	ಪೂರ್ಣಾಂಕ cpu;

	ಆದರೆ (!sched_cache_enabled() || env->idle == CPU_NEWLY_IDLE)
		ಹಿಂತಿರುಗಿ;

	/* Only care about sched domain spanning multiple LLCs */
	ಆದರೆ (env->sd->ಮಗು != rcu_dereference_all(ಪ್ರತಿ_cpu(sd_llc, env->dst_cpu)))
		ಹಿಂತಿರುಗಿ;

	/*
	 * A redo repeats stats published on the first pass. Hotplug can
	 * still leave env->cpus partial, so util would not match capacity.
	 */
	ಆದರೆ ((env->flags & LBF_ALL_PINNED) ||
	    !cpumask_subset(sched_ಗುಂಪು_ಹರವು(ಗುಂಪು), env->cpus))
		ಹಿಂತಿರುಗಿ;

	/*
	 * At this point we know this group spans a LLC domain.
	 * Record the statistic of this group in its corresponding
	 * shared LLC domain.
	 * Note: sd_share cannot be obtained via sd->child->shared,
	 * because the latter refers to the domain that covers the
	 * local group. Instead, sd_share should be located using
	 * the first CPU of the LLC group.
	 */
	cpu = cpumask_ಮೊದಲ(sched_ಗುಂಪು_ಹರವು(ಗುಂಪು));
	sd_share = rcu_dereference_all(ಪ್ರತಿ_cpu(sd_llc_shared, cpu));
	ಆದರೆ (!sd_share)
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (READ_ONCE(sd_share->util_avg) != sgs->ಗುಂಪು_util)
		WRITE_ONCE(sd_share->util_avg, sgs->ಗುಂಪು_util);

	ಆದರೆ (unlikely(READ_ONCE(sd_share->ಸಾಮರ್ಥ್ಯ) != sgs->ಗುಂಪು_ಸಾಮರ್ಥ್ಯ))
		WRITE_ONCE(sd_share->ಸಾಮರ್ಥ್ಯ, sgs->ಗುಂಪು_ಸಾಮರ್ಥ್ಯ);
}

/*
 * Do LLC balance on sched group that contains LLC, and have tasks preferring
 * to run on LLC in idle dst_cpu.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool llc_balance(ರಚನೆ lb_env *env, ರಚನೆ sg_lb_stats *sgs,
			       ರಚನೆ sched_ಗುಂಪು *ಗುಂಪು)
{
	ಆದರೆ (!sched_cache_enabled())
		ಹಿಂತಿರುಗಿ false;

	ಆದರೆ (env->sd->flags & SD_SHARE_LLC)
		ಹಿಂತಿರುಗಿ false;

	/*
	 * On asymmetric domains, group_misfit_task_load
	 * should be prioritized to move tasks to CPU that fit them
	 * over aggregating tasks to their preferred LLC.
	 */
	ಆದರೆ ((env->sd->flags & SD_ASYM_CPUCAPACITY) &&
	    sgs->ಗುಂಪು_misfit_task_ತುಂಬು)
		ಹಿಂತಿರುಗಿ false;

	/*
	 * Skip cache aware tagging if nr_balanced_failed is sufficiently high.
	 * Threshold of cache_nice_tries is set to 1 higher than nr_balance_failed
	 * to avoid excessive task migration at the same time.
	 */
	ಆದರೆ (env->sd->nr_balance_failed >= env->sd->cache_nice_tries + 1)
		ಹಿಂತಿರುಗಿ false;

	ಆದರೆ (sgs->nr_pref_dst_llc &&
	    can_migrate_llc(cpumask_ಮೊದಲ(sched_ಗುಂಪು_ಹರವು(ಗುಂಪು)),
			    env->dst_cpu, 0, true) == mig_llc)
		ಹಿಂತಿರುಗಿ true;

	ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ bool update_llc_busiest(ರಚನೆ lb_env *env,
			       ರಚನೆ sg_lb_stats *busiest,
			       ರಚನೆ sg_lb_stats *sgs)
{
	/*
	 * There are more tasks that want to run on dst_cpu's LLC.
	 */
	ಹಿಂತಿರುಗಿ sgs->nr_pref_dst_llc > busiest->nr_pref_dst_llc;
}
#ಇಲ್ಲದಿದ್ದರೆ
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ ದಾಖಲೆ_sg_llc_stats(ರಚನೆ lb_env *env, ರಚನೆ sg_lb_stats *sgs,
				       ರಚನೆ sched_ಗುಂಪು *ಗುಂಪು)
{
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool llc_balance(ರಚನೆ lb_env *env, ರಚನೆ sg_lb_stats *sgs,
			       ರಚನೆ sched_ಗುಂಪು *ಗುಂಪು)
{
	ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ bool update_llc_busiest(ರಚನೆ lb_env *env,
			       ರಚನೆ sg_lb_stats *busiest,
			       ರಚನೆ sg_lb_stats *sgs)
{
	ಹಿಂತಿರುಗಿ false;
}
#ಆದರೆ_ಅಂತ್ಯ

/**
 * update_sg_lb_stats - Update sched_group's statistics for load balancing.
 * @env: The load balancing environment.
 * @sds: Load-balancing data with statistics of the local group.
 * @group: sched_group whose statistics are to be updated.
 * @sgs: variable to hold the statistics for this group.
 * @sg_overloaded: sched_group is overloaded
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ update_sg_lb_stats(ರಚನೆ lb_env *env,
				      ರಚನೆ sd_lb_stats *sds,
				      ರಚನೆ sched_ಗುಂಪು *ಗುಂಪು,
				      ರಚನೆ sg_lb_stats *sgs,
				      bool *sg_overloaded)
{
	ಪೂರ್ಣಾಂಕ i, nr_running, local_ಗುಂಪು, sd_flags = env->sd->flags;
	bool balancing_at_rd = !env->sd->ಪೋಷಕ;

	memset(sgs, 0, ಗಾತ್ರ(*sgs));

	local_ಗುಂಪು = ಗುಂಪು == sds->local;

	for_each_cpu_and(i, sched_ಗುಂಪು_ಹರವು(ಗುಂಪು), env->cpus) {
		ರಚನೆ rq *rq = cpu_rq(i);
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ತುಂಬು = cpu_ತುಂಬು(rq);

		sgs->ಗುಂಪು_ತುಂಬು += ತುಂಬು;
		sgs->ಗುಂಪು_util += cpu_util_cfs(i);
		sgs->ಗುಂಪು_runnable += cpu_runnable(rq);
		sgs->ಮೊತ್ತ_h_nr_running += rq->cfs.h_nr_runnable;

		nr_running = rq->nr_running;
		sgs->ಮೊತ್ತ_nr_running += nr_running;

		ಆದರೆ (cpu_overutilized(i))
			sgs->ಗುಂಪು_overutilized = 1;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CACHE
		ಆದರೆ (sched_cache_enabled()) {
			ರಚನೆ sched_domain *sd_tmp;
			ಪೂರ್ಣಾಂಕ dst_llc;

			dst_llc = llc_id(env->dst_cpu);
			ಆದರೆ (llc_id(i) != dst_llc) {
				sd_tmp = rcu_dereference_all(rq->sd);
				ಆದರೆ (sd_tmp && (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)dst_llc < sd_tmp->llc_max)
					sgs->nr_pref_dst_llc += sd_tmp->llc_counts[dst_llc];
			}
		}
#ಆದರೆ_ಅಂತ್ಯ

		/*
		 * No need to call idle_cpu() if nr_running is not 0
		 */
		ಆದರೆ (!nr_running && idle_cpu(i)) {
			sgs->idle_cpus++;
			/* Idle cpu can't have misfit task */
			ಮುಂದುವರಿ;
		}

		/* Overload indicator is only updated at root domain */
		ಆದರೆ (balancing_at_rd && nr_running > 1)
			*sg_overloaded = 1;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_NUMA_BALANCING
		/* Only fbq_classify_group() uses this to classify NUMA groups */
		ಆದರೆ (sd_flags & SD_NUMA) {
			sgs->nr_numa_running += rq->nr_numa_running;
			sgs->nr_preferred_running += rq->nr_preferred_running;
		}
#ಆದರೆ_ಅಂತ್ಯ
		ಆದರೆ (local_ಗುಂಪು)
			ಮುಂದುವರಿ;

		ಆದರೆ (sd_flags & SD_ASYM_CPUCAPACITY) {
			ಆದರೆ (rq->misfit_task_ತುಂಬು) {
				/*
				 * Always mark the root domain overloaded so big
				 * CPUs can pick up misfit tasks via newly idle
				 * balance.
				 */
				ಆದರೆ (balancing_at_rd)
					*sg_overloaded = 1;

				/*
				 * Only account misfit load if @dst_cpu can
				 * help; otherwise, the group may be classified
				 * as misfit_task and update_sd_pick_busiest()
				 * will skip it.
				 */
				ಆದರೆ (ಸಾಮರ್ಥ್ಯ_greater(ಸಾಮರ್ಥ್ಯ_ದ(env->dst_cpu),
						     ಗುಂಪು->sgc->max_ಸಾಮರ್ಥ್ಯ) &&
				    (sgs->ಗುಂಪು_misfit_task_ತುಂಬು < rq->misfit_task_ತುಂಬು))
					sgs->ಗುಂಪು_misfit_task_ತುಂಬು = rq->misfit_task_ತುಂಬು;
			}
		} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (env->idle && sched_reduced_ಸಾಮರ್ಥ್ಯ(rq, env->sd)) {
			/* Check for a task running on a CPU with reduced capacity */
			ಆದರೆ (sgs->ಗುಂಪು_misfit_task_ತುಂಬು < ತುಂಬು)
				sgs->ಗುಂಪು_misfit_task_ತುಂಬು = ತುಂಬು;
		}
	}

	sgs->ಗುಂಪು_ಸಾಮರ್ಥ್ಯ = ಗುಂಪು->sgc->ಸಾಮರ್ಥ್ಯ;

	sgs->ಗುಂಪು_ತೂಕ = ಗುಂಪು->ಗುಂಪು_ತೂಕ;

	ಆದರೆ (!local_ಗುಂಪು) {
		/* Check if dst CPU is idle and preferred to this group */
		ಆದರೆ (env->idle && sgs->ಮೊತ್ತ_h_nr_running &&
		    sched_ಗುಂಪು_asym(env, sgs, ಗುಂಪು))
			sgs->ಗುಂಪು_asym_packing = 1;

		/* Check for loaded SMT group to be balanced to dst CPU */
		ಆದರೆ (smt_balance(env, sgs, ಗುಂಪು))
			sgs->ಗುಂಪು_smt_balance = 1;

		/* Check for tasks in this group can be moved to their preferred LLC */
		ಆದರೆ (llc_balance(env, sgs, ಗುಂಪು))
			sgs->ಗುಂಪು_llc_balance = 1;
	}

	sgs->ಗುಂಪು_type = ಗುಂಪು_classify(env->sd->imbalance_pct, ಗುಂಪು, sgs);

	ದಾಖಲೆ_sg_llc_stats(env, sgs, ಗುಂಪು);
	/* Computing avg_load makes sense only when group is overloaded */
	ಆದರೆ (sgs->ಗುಂಪು_type == ಗುಂಪು_overloaded)
		sgs->avg_ತುಂಬು = (sgs->ಗುಂಪು_ತುಂಬು * SCHED_ಸಾಮರ್ಥ್ಯ_SCALE) /
				sgs->ಗುಂಪು_ಸಾಮರ್ಥ್ಯ;
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
ಸ್ಥಿರ bool update_sd_pick_busiest(ರಚನೆ lb_env *env,
				   ರಚನೆ sd_lb_stats *sds,
				   ರಚನೆ sched_ಗುಂಪು *sg,
				   ರಚನೆ sg_lb_stats *sgs)
{
	ರಚನೆ sg_lb_stats *busiest = &sds->busiest_stat;

	/* Make sure that there is at least one task to pull */
	ಆದರೆ (!sgs->ಮೊತ್ತ_h_nr_running)
		ಹಿಂತಿರುಗಿ false;

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
	ಆದರೆ ((env->sd->flags & SD_ASYM_CPUCAPACITY) &&
	    (sgs->ಗುಂಪು_type == ಗುಂಪು_misfit_task) &&
	    (!env->dst_core_idle ||
	     !ಸಾಮರ್ಥ್ಯ_greater(ಸಾಮರ್ಥ್ಯ_ದ(env->dst_cpu), sg->sgc->max_ಸಾಮರ್ಥ್ಯ) ||
	     sds->local_stat.ಗುಂಪು_type != ಗುಂಪು_has_ಹೆಚ್ಚುವರಿ))
		ಹಿಂತಿರುಗಿ false;

	/*
	 * Candidate sg has no more than one task per CPU and has higher
	 * per-CPU capacity. Migrating tasks to less capable CPUs may harm
	 * throughput. Maximize throughput, power/energy consequences are not
	 * considered.
	 */
	ಆದರೆ ((env->sd->flags & SD_ASYM_CPUCAPACITY) &&
	    (sgs->ಗುಂಪು_type <= ಗುಂಪು_fully_busy) &&
	    (ಸಾಮರ್ಥ್ಯ_greater(sg->sgc->min_ಸಾಮರ್ಥ್ಯ, ಸಾಮರ್ಥ್ಯ_ದ(env->dst_cpu))))
		ಹಿಂತಿರುಗಿ false;

	ಆದರೆ (sgs->ಗುಂಪು_type > busiest->ಗುಂಪು_type)
		ಹಿಂತಿರುಗಿ true;

	ಆದರೆ (sgs->ಗುಂಪು_type < busiest->ಗುಂಪು_type)
		ಹಿಂತಿರುಗಿ false;

	/*
	 * The candidate and the current busiest group are the same type of
	 * group. Let check which one is the busiest according to the type.
	 */

	ಆಯ್ಕೆ (sgs->ಗುಂಪು_type) {
	ಸ್ಥಿತಿ ಗುಂಪು_overloaded:
		/* Select the overloaded group with highest avg_load. */
		ಹಿಂತಿರುಗಿ sgs->avg_ತುಂಬು > busiest->avg_ತುಂಬು;

	ಸ್ಥಿತಿ ಗುಂಪು_llc_balance:
		/* Select the group with most tasks preferring dst LLC */
		ಹಿಂತಿರುಗಿ update_llc_busiest(env, busiest, sgs);

	ಸ್ಥಿತಿ ಗುಂಪು_imbalanced:
		/*
		 * Select the 1st imbalanced group as we don't have any way to
		 * choose one more than another.
		 */
		ಹಿಂತಿರುಗಿ false;

	ಸ್ಥಿತಿ ಗುಂಪು_asym_packing:
		/* Prefer to move from lowest priority CPU's work */
		ಹಿಂತಿರುಗಿ sched_asym_prefer(READ_ONCE(sds->busiest->asym_prefer_cpu),
					 READ_ONCE(sg->asym_prefer_cpu));

	ಸ್ಥಿತಿ ಗುಂಪು_misfit_task:
		/*
		 * If we have more than one misfit sg go with the biggest
		 * misfit.
		 */
		ಹಿಂತಿರುಗಿ sgs->ಗುಂಪು_misfit_task_ತುಂಬು > busiest->ಗುಂಪು_misfit_task_ತುಂಬು;

	ಸ್ಥಿತಿ ಗುಂಪು_smt_balance:
		/*
		 * Check if we have spare CPUs on either SMT group to
		 * choose has spare or fully busy handling.
		 */
		ಆದರೆ (sgs->idle_cpus != 0 || busiest->idle_cpus != 0)
			ಹೋಗು has_ಹೆಚ್ಚುವರಿ;

		fallthrough;

	ಸ್ಥಿತಿ ಗುಂಪು_fully_busy:
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

		ಆದರೆ (sgs->avg_ತುಂಬು < busiest->avg_ತುಂಬು)
			ಹಿಂತಿರುಗಿ false;

		ಆದರೆ (sgs->avg_ತುಂಬು == busiest->avg_ತುಂಬು) {
			/*
			 * SMT sched groups need more help than non-SMT groups.
			 * If @sg happens to also be SMT, either choice is good.
			 */
			ಆದರೆ (sds->busiest->flags & SD_SHARE_CPUCAPACITY)
				ಹಿಂತಿರುಗಿ false;
		}

		ನಿಲ್ಲಿಸು;

	ಸ್ಥಿತಿ ಗುಂಪು_has_ಹೆಚ್ಚುವರಿ:
		/*
		 * Do not pick sg with SMT CPUs over sg with pure CPUs,
		 * as we do not want to pull task off SMT core with one task
		 * and make the core idle.
		 */
		ಆದರೆ (smt_vs_nonsmt_groups(sds->busiest, sg)) {
			ಆದರೆ (sg->flags & SD_SHARE_CPUCAPACITY && sgs->ಮೊತ್ತ_h_nr_running <= 1)
				ಹಿಂತಿರುಗಿ false;
			ಇಲ್ಲದಿದ್ದರೆ
				ಹಿಂತಿರುಗಿ true;
		}
has_ಹೆಚ್ಚುವರಿ:

		/*
		 * Select not overloaded group with lowest number of idle CPUs
		 * and highest number of running tasks. We could also compare
		 * the spare capacity which is more stable but it can end up
		 * that the group has less spare capacity but finally more idle
		 * CPUs which means less opportunity to pull tasks.
		 */
		ಆದರೆ (sgs->idle_cpus > busiest->idle_cpus)
			ಹಿಂತಿರುಗಿ false;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ ((sgs->idle_cpus == busiest->idle_cpus) &&
			 (sgs->ಮೊತ್ತ_nr_running <= busiest->ಮೊತ್ತ_nr_running))
			ಹಿಂತಿರುಗಿ false;

		ನಿಲ್ಲಿಸು;
	}

	ಹಿಂತಿರುಗಿ true;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_NUMA_BALANCING
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಎಣಿಕೆ fbq_type fbq_classify_ಗುಂಪು(ರಚನೆ sg_lb_stats *sgs)
{
	ಆದರೆ (sgs->ಮೊತ್ತ_h_nr_running > sgs->nr_numa_running)
		ಹಿಂತಿರುಗಿ regular;
	ಆದರೆ (sgs->ಮೊತ್ತ_h_nr_running > sgs->nr_preferred_running)
		ಹಿಂತಿರುಗಿ remote;
	ಹಿಂತಿರುಗಿ all;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಎಣಿಕೆ fbq_type fbq_classify_rq(ರಚನೆ rq *rq)
{
	ಆದರೆ (rq->nr_running > rq->nr_numa_running)
		ಹಿಂತಿರುಗಿ regular;
	ಆದರೆ (rq->nr_running > rq->nr_preferred_running)
		ಹಿಂತಿರುಗಿ remote;
	ಹಿಂತಿರುಗಿ all;
}
#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_NUMA_BALANCING: */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಎಣಿಕೆ fbq_type fbq_classify_ಗುಂಪು(ರಚನೆ sg_lb_stats *sgs)
{
	ಹಿಂತಿರುಗಿ all;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಎಣಿಕೆ fbq_type fbq_classify_rq(ರಚನೆ rq *rq)
{
	ಹಿಂತಿರುಗಿ regular;
}
#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_NUMA_BALANCING */


ರಚನೆ sg_lb_stats;

/*
 * task_running_on_cpu - return 1 if @p is running on @cpu.
 */

ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ task_running_on_cpu(ಪೂರ್ಣಾಂಕ cpu, ರಚನೆ task_struct *p)
{
	/* Task has no contribution or is new */
	ಆದರೆ (cpu != task_cpu(p) || !READ_ONCE(p->se.avg.ಕೊನೆಯ_update_ಸಮಯ))
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (task_on_rq_queued(p))
		ಹಿಂತಿರುಗಿ 1;

	ಹಿಂತಿರುಗಿ 0;
}

/**
 * idle_cpu_without - would a given CPU be idle without p ?
 * @cpu: the processor on which idleness is tested.
 * @p: task which should be ignored.
 *
 * Return: 1 if the CPU would be idle. 0 otherwise.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ idle_cpu_without(ಪೂರ್ಣಾಂಕ cpu, ರಚನೆ task_struct *p)
{
	ರಚನೆ rq *rq = cpu_rq(cpu);

	ಆದರೆ (rq->curr != rq->idle && rq->curr != p)
		ಹಿಂತಿರುಗಿ 0;

	/*
	 * rq->nr_running can't be used but an updated version without the
	 * impact of p on cpu must be used instead. The updated nr_running
	 * be computed and tested before calling idle_cpu_without().
	 */

	ಆದರೆ (rq->ttwu_pending)
		ಹಿಂತಿರುಗಿ 0;

	ಹಿಂತಿರುಗಿ 1;
}

/*
 * update_sg_wakeup_stats - Update sched_group's statistics for wakeup.
 * @sd: The sched_domain level to look for idlest group.
 * @group: sched_group whose statistics are to be updated.
 * @sgs: variable to hold the statistics for this group.
 * @p: The task for which we look for the idlest group/CPU.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ update_sg_wakeup_stats(ರಚನೆ sched_domain *sd,
					  ರಚನೆ sched_ಗುಂಪು *ಗುಂಪು,
					  ರಚನೆ sg_lb_stats *sgs,
					  ರಚನೆ task_struct *p)
{
	ಪೂರ್ಣಾಂಕ i, nr_running;

	memset(sgs, 0, ಗಾತ್ರ(*sgs));

	/* Assume that task can't fit any CPU of the group */
	ಆದರೆ (sd->flags & SD_ASYM_CPUCAPACITY)
		sgs->ಗುಂಪು_misfit_task_ತುಂಬು = 1;

	for_each_cpu_and(i, sched_ಗುಂಪು_ಹರವು(ಗುಂಪು), p->cpus_ptr) {
		ರಚನೆ rq *rq = cpu_rq(i);
		ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ local;

		sgs->ಗುಂಪು_ತುಂಬು += cpu_ತುಂಬು_without(rq, p);
		sgs->ಗುಂಪು_util += cpu_util_without(i, p);
		sgs->ಗುಂಪು_runnable += cpu_runnable_without(rq, p);
		local = task_running_on_cpu(i, p);
		sgs->ಮೊತ್ತ_h_nr_running += rq->cfs.h_nr_runnable - local;

		nr_running = rq->nr_running - local;
		sgs->ಮೊತ್ತ_nr_running += nr_running;

		/*
		 * No need to call idle_cpu_without() if nr_running is not 0
		 */
		ಆದರೆ (!nr_running && idle_cpu_without(i, p))
			sgs->idle_cpus++;

		/* Check if task fits in the CPU */
		ಆದರೆ (sd->flags & SD_ASYM_CPUCAPACITY &&
		    sgs->ಗುಂಪು_misfit_task_ತುಂಬು &&
		    task_ಹೊಂದುತ್ತದೆ_cpu(p, i))
			sgs->ಗುಂಪು_misfit_task_ತುಂಬು = 0;

	}

	sgs->ಗುಂಪು_ಸಾಮರ್ಥ್ಯ = ಗುಂಪು->sgc->ಸಾಮರ್ಥ್ಯ;

	sgs->ಗುಂಪು_ತೂಕ = ಗುಂಪು->ಗುಂಪು_ತೂಕ;

	sgs->ಗುಂಪು_type = ಗುಂಪು_classify(sd->imbalance_pct, ಗುಂಪು, sgs);

	/*
	 * Computing avg_load makes sense only when group is fully busy or
	 * overloaded
	 */
	ಆದರೆ (sgs->ಗುಂಪು_type == ಗುಂಪು_fully_busy ||
		sgs->ಗುಂಪು_type == ಗುಂಪು_overloaded)
		sgs->avg_ತುಂಬು = (sgs->ಗುಂಪು_ತುಂಬು * SCHED_ಸಾಮರ್ಥ್ಯ_SCALE) /
				sgs->ಗುಂಪು_ಸಾಮರ್ಥ್ಯ;
}

ಸ್ಥಿರ bool update_pick_idlest(ರಚನೆ sched_ಗುಂಪು *idlest,
			       ರಚನೆ sg_lb_stats *idlest_sgs,
			       ರಚನೆ sched_ಗುಂಪು *ಗುಂಪು,
			       ರಚನೆ sg_lb_stats *sgs)
{
	ಆದರೆ (sgs->ಗುಂಪು_type < idlest_sgs->ಗುಂಪು_type)
		ಹಿಂತಿರುಗಿ true;

	ಆದರೆ (sgs->ಗುಂಪು_type > idlest_sgs->ಗುಂಪು_type)
		ಹಿಂತಿರುಗಿ false;

	/*
	 * The candidate and the current idlest group are the same type of
	 * group. Let check which one is the idlest according to the type.
	 */

	ಆಯ್ಕೆ (sgs->ಗುಂಪು_type) {
	ಸ್ಥಿತಿ ಗುಂಪು_overloaded:
	ಸ್ಥಿತಿ ಗುಂಪು_fully_busy:
		/* Select the group with lowest avg_load. */
		ಆದರೆ (idlest_sgs->avg_ತುಂಬು <= sgs->avg_ತುಂಬು)
			ಹಿಂತಿರುಗಿ false;
		ನಿಲ್ಲಿಸು;

	ಸ್ಥಿತಿ ಗುಂಪು_llc_balance:
	ಸ್ಥಿತಿ ಗುಂಪು_imbalanced:
	ಸ್ಥಿತಿ ಗುಂಪು_asym_packing:
	ಸ್ಥಿತಿ ಗುಂಪು_smt_balance:
		/* Those types are not used in the slow wakeup path */
		ಹಿಂತಿರುಗಿ false;

	ಸ್ಥಿತಿ ಗುಂಪು_misfit_task:
		/* Select group with the highest max capacity */
		ಆದರೆ (idlest->sgc->max_ಸಾಮರ್ಥ್ಯ >= ಗುಂಪು->sgc->max_ಸಾಮರ್ಥ್ಯ)
			ಹಿಂತಿರುಗಿ false;
		ನಿಲ್ಲಿಸು;

	ಸ್ಥಿತಿ ಗುಂಪು_has_ಹೆಚ್ಚುವರಿ:
		/* Select group with most idle CPUs */
		ಆದರೆ (idlest_sgs->idle_cpus > sgs->idle_cpus)
			ಹಿಂತಿರುಗಿ false;

		/* Select group with lowest group_util */
		ಆದರೆ (idlest_sgs->idle_cpus == sgs->idle_cpus &&
			idlest_sgs->ಗುಂಪು_util <= sgs->ಗುಂಪು_util)
			ಹಿಂತಿರುಗಿ false;

		ನಿಲ್ಲಿಸು;
	}

	ಹಿಂತಿರುಗಿ true;
}

/*
 * sched_balance_find_dst_group() finds and returns the least busy CPU group within the
 * domain.
 *
 * Assumes p is allowed on at least one CPU in sd.
 */
ಸ್ಥಿರ ರಚನೆ sched_ಗುಂಪು *
sched_balance_ಕಂಡುಹಿಡಿ_dst_ಗುಂಪು(ರಚನೆ sched_domain *sd, ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ this_cpu)
{
	ರಚನೆ sched_ಗುಂಪು *idlest = ಖಾಲಿ, *local = ಖಾಲಿ, *ಗುಂಪು = sd->groups;
	ರಚನೆ sg_lb_stats local_sgs, tmp_sgs;
	ರಚನೆ sg_lb_stats *sgs;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ imbalance;
	ರಚನೆ sg_lb_stats idlest_sgs = {
			.avg_ತುಂಬು = UINT_MAX,
			.ಗುಂಪು_type = ಗುಂಪು_overloaded,
	};

	ಮಾಡು {
		ಪೂರ್ಣಾಂಕ local_ಗುಂಪು;

		/* Skip over this group if it has no CPUs allowed */
		ಆದರೆ (!cpumask_intersects(sched_ಗುಂಪು_ಹರವು(ಗುಂಪು),
					p->cpus_ptr))
			ಮುಂದುವರಿ;

		/* Skip over this group if no cookie matched */
		ಆದರೆ (!sched_ಗುಂಪು_cookie_ಹೊಂದಿಕೆ(cpu_rq(this_cpu), p, ಗುಂಪು))
			ಮುಂದುವರಿ;

		local_ಗುಂಪು = cpumask_test_cpu(this_cpu,
					       sched_ಗುಂಪು_ಹರವು(ಗುಂಪು));

		ಆದರೆ (local_ಗುಂಪು) {
			sgs = &local_sgs;
			local = ಗುಂಪು;
		} ಇಲ್ಲದಿದ್ದರೆ {
			sgs = &tmp_sgs;
		}

		update_sg_wakeup_stats(sd, ಗುಂಪು, sgs, p);

		ಆದರೆ (!local_ಗುಂಪು && update_pick_idlest(idlest, &idlest_sgs, ಗುಂಪು, sgs)) {
			idlest = ಗುಂಪು;
			idlest_sgs = *sgs;
		}

	} ಆಗ (ಗುಂಪು = ಗುಂಪು->ಮುಂದಿನ, ಗುಂಪು != sd->groups);


	/* There is no idlest group to push tasks to */
	ಆದರೆ (!idlest)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	/* The local group has been skipped because of CPU affinity */
	ಆದರೆ (!local)
		ಹಿಂತಿರುಗಿ idlest;

	/*
	 * If the local group is idler than the selected idlest group
	 * don't try and push the task.
	 */
	ಆದರೆ (local_sgs.ಗುಂಪು_type < idlest_sgs.ಗುಂಪು_type)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	/*
	 * If the local group is busier than the selected idlest group
	 * try and push the task.
	 */
	ಆದರೆ (local_sgs.ಗುಂಪು_type > idlest_sgs.ಗುಂಪು_type)
		ಹಿಂತಿರುಗಿ idlest;

	ಆಯ್ಕೆ (local_sgs.ಗುಂಪು_type) {
	ಸ್ಥಿತಿ ಗುಂಪು_overloaded:
	ಸ್ಥಿತಿ ಗುಂಪು_fully_busy:

		/* Calculate allowed imbalance based on load */
		imbalance = scale_ತುಂಬು_ಕೆಳಕ್ಕೆ(NICE_0_ತುಂಬು) *
				(sd->imbalance_pct-100) / 100;

		/*
		 * When comparing groups across NUMA domains, it's possible for
		 * the local domain to be very lightly loaded relative to the
		 * remote domains but "imbalance" skews the comparison making
		 * remote CPUs look much more favourable. When considering
		 * cross-domain, add imbalance to the load on the remote node
		 * and consider staying local.
		 */

		ಆದರೆ ((sd->flags & SD_NUMA) &&
		    ((idlest_sgs.avg_ತುಂಬು + imbalance) >= local_sgs.avg_ತುಂಬು))
			ಹಿಂತಿರುಗಿ ಖಾಲಿ;

		/*
		 * If the local group is less loaded than the selected
		 * idlest group don't try and push any tasks.
		 */
		ಆದರೆ (idlest_sgs.avg_ತುಂಬು >= (local_sgs.avg_ತುಂಬು + imbalance))
			ಹಿಂತಿರುಗಿ ಖಾಲಿ;

		ಆದರೆ (100 * local_sgs.avg_ತುಂಬು <= sd->imbalance_pct * idlest_sgs.avg_ತುಂಬು)
			ಹಿಂತಿರುಗಿ ಖಾಲಿ;
		ನಿಲ್ಲಿಸು;

	ಸ್ಥಿತಿ ಗುಂಪು_llc_balance:
	ಸ್ಥಿತಿ ಗುಂಪು_imbalanced:
	ಸ್ಥಿತಿ ಗುಂಪು_asym_packing:
	ಸ್ಥಿತಿ ಗುಂಪು_smt_balance:
		/* Those type are not used in the slow wakeup path */
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;

	ಸ್ಥಿತಿ ಗುಂಪು_misfit_task:
		/* Select group with the highest max capacity */
		ಆದರೆ (local->sgc->max_ಸಾಮರ್ಥ್ಯ >= idlest->sgc->max_ಸಾಮರ್ಥ್ಯ)
			ಹಿಂತಿರುಗಿ ಖಾಲಿ;
		ನಿಲ್ಲಿಸು;

	ಸ್ಥಿತಿ ಗುಂಪು_has_ಹೆಚ್ಚುವರಿ:
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_NUMA
		ಆದರೆ (sd->flags & SD_NUMA) {
			ಪೂರ್ಣಾಂಕ imb_numa_nr = sd->imb_numa_nr;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_NUMA_BALANCING
			ಪೂರ್ಣಾಂಕ idlest_cpu;
			/*
			 * If there is spare capacity at NUMA, try to select
			 * the preferred node
			 */
			ಆದರೆ (cpu_ಗಮ್ಯ_ಗಂಟು(this_cpu) == p->numa_preferred_nid)
				ಹಿಂತಿರುಗಿ ಖಾಲಿ;

			idlest_cpu = cpumask_ಮೊದಲ(sched_ಗುಂಪು_ಹರವು(idlest));
			ಆದರೆ (cpu_ಗಮ್ಯ_ಗಂಟು(idlest_cpu) == p->numa_preferred_nid)
				ಹಿಂತಿರುಗಿ idlest;
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_NUMA_BALANCING */
			/*
			 * Otherwise, keep the task close to the wakeup source
			 * and improve locality if the number of running tasks
			 * would remain below threshold where an imbalance is
			 * allowed while accounting for the possibility the
			 * task is pinned to a subset of CPUs. If there is a
			 * real need of migration, periodic load balance will
			 * take care of it.
			 */
			ಆದರೆ (p->nr_cpus_allowed != NR_CPUS) {
				ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ w = cpumask_ತೂಕ_and(p->cpus_ptr,
								sched_ಗುಂಪು_ಹರವು(local));
				imb_numa_nr = min(w, sd->imb_numa_nr);
			}

			imbalance = ನಿರಪೇಕ್ಷ(local_sgs.idle_cpus - idlest_sgs.idle_cpus);
			ಆದರೆ (!adjust_numa_imbalance(imbalance,
						   local_sgs.ಮೊತ್ತ_nr_running + 1,
						   imb_numa_nr)) {
				ಹಿಂತಿರುಗಿ ಖಾಲಿ;
			}
		}
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_NUMA */

		/*
		 * Select group with highest number of idle CPUs. We could also
		 * compare the utilization which is more stable but it can end
		 * up that the group has less spare capacity but finally more
		 * idle CPUs which means more opportunity to run task.
		 */
		ಆದರೆ (local_sgs.idle_cpus >= idlest_sgs.idle_cpus)
			ಹಿಂತಿರುಗಿ ಖಾಲಿ;
		ನಿಲ್ಲಿಸು;
	}

	ಹಿಂತಿರುಗಿ idlest;
}

ಸ್ಥಿರ ಶೂನ್ಯ update_idle_cpu_ಪರಿಶೀಲನೆ(ರಚನೆ lb_env *env,
				 ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಮೊತ್ತ_util)
{
	ರಚನೆ sched_domain_shared *sd_share;
	ರಚನೆ sched_domain *sd = env->sd;
	ಪೂರ್ಣಾಂಕ llc_ತೂಕ, pct;
	u64 x, y, tmp;
	/*
	 * Update the number of CPUs to scan in LLC domain, which could
	 * be used as a hint in select_idle_cpu(). The update of sd_share
	 * could be expensive because it is within a shared cache line.
	 * So the write of this hint only occurs during periodic load
	 * balancing, rather than CPU_NEWLY_IDLE, because the latter
	 * can fire way more frequently than the former.
	 */
	ಆದರೆ (!sched_feat(SIS_UTIL) || env->idle == CPU_NEWLY_IDLE)
		ಹಿಂತಿರುಗಿ;

	sd_share = sd->shared;
	ಆದರೆ (!sd_share)
		ಹಿಂತಿರುಗಿ;

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
	x = ಮೊತ್ತ_util;
	llc_ತೂಕ = sd->ಹರವು_ತೂಕ;
	do_div(x, llc_ತೂಕ);

	/* equation [4] */
	pct = sd->imbalance_pct;
	tmp = x * x * pct * pct;
	do_div(tmp, 10000 * SCHED_ಸಾಮರ್ಥ್ಯ_SCALE);
	tmp = min_t(ದೀರ್ಘ, tmp, SCHED_ಸಾಮರ್ಥ್ಯ_SCALE);
	y = SCHED_ಸಾಮರ್ಥ್ಯ_SCALE - tmp;

	/* equation [2] */
	y *= llc_ತೂಕ;
	do_div(y, SCHED_ಸಾಮರ್ಥ್ಯ_SCALE);
	ಆದರೆ ((ಪೂರ್ಣಾಂಕ)y != sd_share->nr_idle_ಪರಿಶೀಲನೆ)
		WRITE_ONCE(sd_share->nr_idle_ಪರಿಶೀಲನೆ, (ಪೂರ್ಣಾಂಕ)y);
}

/**
 * update_sd_lb_stats - Update sched_domain's statistics for load balancing.
 * @env: The load balancing environment.
 * @sds: variable to hold the statistics for this sched_domain.
 */

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ update_sd_lb_stats(ರಚನೆ lb_env *env, ರಚನೆ sd_lb_stats *sds)
{
	ರಚನೆ sched_ಗುಂಪು *sg = env->sd->groups;
	ರಚನೆ sg_lb_stats *local = &sds->local_stat;
	ರಚನೆ sg_lb_stats tmp_sgs;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಮೊತ್ತ_util = 0;
	bool sg_overloaded = 0, sg_overutilized = 0;

	env->dst_core_idle = !sched_smt_ಸಕ್ರಿಯ() || is_core_idle(env->dst_cpu);

	ಮಾಡು {
		ರಚನೆ sg_lb_stats *sgs = &tmp_sgs;
		ಪೂರ್ಣಾಂಕ local_ಗುಂಪು;

		local_ಗುಂಪು = cpumask_test_cpu(env->dst_cpu, sched_ಗುಂಪು_ಹರವು(sg));
		ಆದರೆ (local_ಗುಂಪು) {
			sds->local = sg;
			sgs = local;

			ಆದರೆ (env->idle != CPU_NEWLY_IDLE ||
			    ಸಮಯ_after_eq(jiffies, sg->sgc->ಮುಂದಿನ_update))
				update_ಗುಂಪು_ಸಾಮರ್ಥ್ಯ(env->sd, env->dst_cpu);
		}

		update_sg_lb_stats(env, sds, sg, sgs, &sg_overloaded);

		ಆದರೆ (!local_ಗುಂಪು && update_sd_pick_busiest(env, sds, sg, sgs)) {
			sds->busiest = sg;
			sds->busiest_stat = *sgs;
		}

		sg_overutilized |= sgs->ಗುಂಪು_overutilized;

		/* Now, start updating sd_lb_stats */
		sds->ಒಟ್ಟು_ತುಂಬು += sgs->ಗುಂಪು_ತುಂಬು;
		sds->ಒಟ್ಟು_ಸಾಮರ್ಥ್ಯ += sgs->ಗುಂಪು_ಸಾಮರ್ಥ್ಯ;

		ಮೊತ್ತ_util += sgs->ಗುಂಪು_util;
		sg = sg->ಮುಂದಿನ;
	} ಆಗ (sg != env->sd->groups);

	/*
	 * Indicate that the child domain of the busiest group prefers tasks
	 * go to a child's sibling domains first. NB the flags of a sched group
	 * are those of the child domain.
	 */
	ಆದರೆ (sds->busiest)
		sds->prefer_sibling = !!(sds->busiest->flags & SD_PREFER_SIBLING);


	ಆದರೆ (env->sd->flags & SD_NUMA)
		env->fbq_type = fbq_classify_ಗುಂಪು(&sds->busiest_stat);

	ಆದರೆ (!env->sd->ಪೋಷಕ) {
		/* update overload indicator if we are at root domain */
		ಗಣ_rd_overloaded(env->dst_rq->rd, sg_overloaded);

		/* Update over-utilization (tipping point, U >= 0) indicator */
		ಗಣ_rd_overutilized(env->dst_rq->rd, sg_overutilized);
	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (sg_overutilized) {
		ಗಣ_rd_overutilized(env->dst_rq->rd, sg_overutilized);
	}

	update_idle_cpu_ಪರಿಶೀಲನೆ(env, ಮೊತ್ತ_util);
}

/**
 * calculate_imbalance - Calculate the amount of imbalance present within the
 *			 groups of a given sched_domain during load balance.
 * @env: load balance environment
 * @sds: statistics of the sched_domain whose imbalance is to be calculated.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ calculate_imbalance(ರಚನೆ lb_env *env, ರಚನೆ sd_lb_stats *sds)
{
	ರಚನೆ sg_lb_stats *local, *busiest;

	local = &sds->local_stat;
	busiest = &sds->busiest_stat;

	ಆದರೆ (busiest->ಗುಂಪು_type == ಗುಂಪು_misfit_task) {
		ಆದರೆ (env->sd->flags & SD_ASYM_CPUCAPACITY) {
			/* Set imbalance to allow misfit tasks to be balanced. */
			env->migration_type = migrate_misfit;
			env->imbalance = 1;
		} ಇಲ್ಲದಿದ್ದರೆ {
			/*
			 * Set load imbalance to allow moving task from cpu
			 * with reduced capacity.
			 */
			env->migration_type = migrate_ತುಂಬು;
			env->imbalance = busiest->ಗುಂಪು_misfit_task_ತುಂಬು;
		}
		ಹಿಂತಿರುಗಿ;
	}

	ಆದರೆ (busiest->ಗುಂಪು_type == ಗುಂಪು_asym_packing) {
		/*
		 * In case of asym capacity, we will try to migrate all load to
		 * the preferred CPU.
		 */
		env->migration_type = migrate_task;
		env->imbalance = busiest->ಮೊತ್ತ_h_nr_running;
		ಹಿಂತಿರುಗಿ;
	}

	ಆದರೆ (busiest->ಗುಂಪು_type == ಗುಂಪು_smt_balance) {
		/* Reduce number of tasks sharing CPU capacity */
		env->migration_type = migrate_task;
		env->imbalance = 1;
		ಹಿಂತಿರುಗಿ;
	}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CACHE
	ಆದರೆ (busiest->ಗುಂಪು_type == ಗುಂಪು_llc_balance) {
		/* Move a task that prefer local LLC */
		env->migration_type = migrate_llc_task;
		env->imbalance = 1;
		ಹಿಂತಿರುಗಿ;
	}
#ಆದರೆ_ಅಂತ್ಯ

	ಆದರೆ (busiest->ಗುಂಪು_type == ಗುಂಪು_imbalanced) {
		/*
		 * In the group_imb case we cannot rely on group-wide averages
		 * to ensure CPU-load equilibrium, try to move any task to fix
		 * the imbalance. The next load balance will take care of
		 * balancing back the system.
		 */
		env->migration_type = migrate_task;
		env->imbalance = 1;
		ಹಿಂತಿರುಗಿ;
	}

	/*
	 * Try to use spare capacity of local group without overloading it or
	 * emptying busiest.
	 */
	ಆದರೆ (local->ಗುಂಪು_type == ಗುಂಪು_has_ಹೆಚ್ಚುವರಿ) {
		ಆದರೆ ((busiest->ಗುಂಪು_type > ಗುಂಪು_fully_busy) &&
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
			env->imbalance = max(local->ಗುಂಪು_ಸಾಮರ್ಥ್ಯ, local->ಗುಂಪು_util) -
					 local->ಗುಂಪು_util;

			/*
			 * In some cases, the group's utilization is max or even
			 * higher than capacity because of migrations but the
			 * local CPU is (newly) idle. There is at least one
			 * waiting task in this overloaded busiest group. Let's
			 * try to pull it.
			 */
			ಆದರೆ (env->idle && env->imbalance == 0) {
				env->migration_type = migrate_task;
				env->imbalance = 1;
			}

			ಹಿಂತಿರುಗಿ;
		}

		ಆದರೆ (busiest->ಗುಂಪು_ತೂಕ == 1 || sds->prefer_sibling) {
			/*
			 * When prefer sibling, evenly spread running tasks on
			 * groups.
			 */
			env->migration_type = migrate_task;
			env->imbalance = sibling_imbalance(env, sds, busiest, local);
		} ಇಲ್ಲದಿದ್ದರೆ {

			/*
			 * If there is no overload, we just want to even the number of
			 * idle CPUs.
			 */
			env->migration_type = migrate_task;
			env->imbalance = max_t(ದೀರ್ಘ, 0,
					       (local->idle_cpus - busiest->idle_cpus));
		}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_NUMA
		/* Consider allowing a small imbalance between NUMA groups */
		ಆದರೆ (env->sd->flags & SD_NUMA) {
			env->imbalance = adjust_numa_imbalance(env->imbalance,
							       local->ಮೊತ್ತ_nr_running + 1,
							       env->sd->imb_numa_nr);
		}
#ಆದರೆ_ಅಂತ್ಯ

		/* Number of tasks to move to restore balance */
		env->imbalance >>= 1;

		ಹಿಂತಿರುಗಿ;
	}

	/*
	 * Local is fully busy but has to take more load to relieve the
	 * busiest group
	 */
	ಆದರೆ (local->ಗುಂಪು_type < ಗುಂಪು_overloaded) {
		/*
		 * Local will become overloaded so the avg_load metrics are
		 * finally needed.
		 */

		local->avg_ತುಂಬು = (local->ಗುಂಪು_ತುಂಬು * SCHED_ಸಾಮರ್ಥ್ಯ_SCALE) /
				  local->ಗುಂಪು_ಸಾಮರ್ಥ್ಯ;

		/*
		 * If the local group is more loaded than the selected
		 * busiest group don't try to pull any tasks.
		 */
		ಆದರೆ (local->avg_ತುಂಬು >= busiest->avg_ತುಂಬು) {
			env->imbalance = 0;
			ಹಿಂತಿರುಗಿ;
		}

		sds->avg_ತುಂಬು = (sds->ಒಟ್ಟು_ತುಂಬು * SCHED_ಸಾಮರ್ಥ್ಯ_SCALE) /
				sds->ಒಟ್ಟು_ಸಾಮರ್ಥ್ಯ;

		/*
		 * If the local group is more loaded than the average system
		 * load, don't try to pull any tasks.
		 */
		ಆದರೆ (local->avg_ತುಂಬು >= sds->avg_ತುಂಬು) {
			env->imbalance = 0;
			ಹಿಂತಿರುಗಿ;
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
	env->migration_type = migrate_ತುಂಬು;
	env->imbalance = min(
		(busiest->avg_ತುಂಬು - sds->avg_ತುಂಬು) * busiest->ಗುಂಪು_ಸಾಮರ್ಥ್ಯ,
		(sds->avg_ತುಂಬು - local->avg_ತುಂಬು) * local->ಗುಂಪು_ಸಾಮರ್ಥ್ಯ
	) / SCHED_ಸಾಮರ್ಥ್ಯ_SCALE;
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
ಸ್ಥಿರ ರಚನೆ sched_ಗುಂಪು *sched_balance_ಕಂಡುಹಿಡಿ_src_ಗುಂಪು(ರಚನೆ lb_env *env)
{
	ರಚನೆ sg_lb_stats *local, *busiest;
	ರಚನೆ sd_lb_stats sds;

	init_sd_lb_stats(&sds);

	/*
	 * Compute the various statistics relevant for load balancing at
	 * this level.
	 */
	update_sd_lb_stats(env, &sds);

	/* There is no busy sibling group to pull tasks from */
	ಆದರೆ (!sds.busiest)
		ಹೋಗು ಹೊರಹೋಗು_ಸಮತೋಲಿತ;

	busiest = &sds.busiest_stat;

	/* Misfit tasks should be dealt with regardless of the avg load */
	ಆದರೆ (busiest->ಗುಂಪು_type == ಗುಂಪು_misfit_task)
		ಹೋಗು force_balance;

	ಆದರೆ (!is_rd_overutilized(env->dst_rq->rd) &&
	    rcu_dereference_all(env->dst_rq->rd->pd))
		ಹೋಗು ಹೊರಹೋಗು_ಸಮತೋಲಿತ;

	/* ASYM feature bypasses nice load balance check */
	ಆದರೆ (busiest->ಗುಂಪು_type == ಗುಂಪು_asym_packing)
		ಹೋಗು force_balance;

	/*
	 * If the busiest group is imbalanced the below checks don't
	 * work because they assume all things are equal, which typically
	 * isn't true due to cpus_ptr constraints and the like.
	 */
	ಆದರೆ (busiest->ಗುಂಪು_type == ಗುಂಪು_imbalanced)
		ಹೋಗು force_balance;

	local = &sds.local_stat;
	/*
	 * If the local group is busier than the selected busiest group
	 * don't try and pull any tasks.
	 */
	ಆದರೆ (local->ಗುಂಪು_type > busiest->ಗುಂಪು_type)
		ಹೋಗು ಹೊರಹೋಗು_ಸಮತೋಲಿತ;

	/*
	 * When groups are overloaded, use the avg_load to ensure fairness
	 * between tasks.
	 */
	ಆದರೆ (local->ಗುಂಪು_type == ಗುಂಪು_overloaded) {
		/*
		 * If the local group is more loaded than the selected
		 * busiest group don't try to pull any tasks.
		 */
		ಆದರೆ (local->avg_ತುಂಬು >= busiest->avg_ತುಂಬು)
			ಹೋಗು ಹೊರಹೋಗು_ಸಮತೋಲಿತ;

		/* XXX broken for overlapping NUMA groups */
		sds.avg_ತುಂಬು = (sds.ಒಟ್ಟು_ತುಂಬು * SCHED_ಸಾಮರ್ಥ್ಯ_SCALE) /
				sds.ಒಟ್ಟು_ಸಾಮರ್ಥ್ಯ;

		/*
		 * Don't pull any tasks if this group is already above the
		 * domain average load.
		 */
		ಆದರೆ (local->avg_ತುಂಬು >= sds.avg_ತುಂಬು)
			ಹೋಗು ಹೊರಹೋಗು_ಸಮತೋಲಿತ;

		/*
		 * If the busiest group is more loaded, use imbalance_pct to be
		 * conservative.
		 */
		ಆದರೆ (100 * busiest->avg_ತುಂಬು <=
				env->sd->imbalance_pct * local->avg_ತುಂಬು)
			ಹೋಗು ಹೊರಹೋಗು_ಸಮತೋಲಿತ;
	}

	/*
	 * Try to move all excess tasks to a sibling domain of the busiest
	 * group's child domain.
	 */
	ಆದರೆ (sds.prefer_sibling && local->ಗುಂಪು_type == ಗುಂಪು_has_ಹೆಚ್ಚುವರಿ &&
	    (busiest->ಗುಂಪು_type == ಗುಂಪು_llc_balance ||
	    sibling_imbalance(env, &sds, busiest, local) > 1))
		ಹೋಗು force_balance;

	ಆದರೆ (busiest->ಗುಂಪು_type != ಗುಂಪು_overloaded) {
		ಆದರೆ (!env->idle) {
			/*
			 * If the busiest group is not overloaded (and as a
			 * result the local one too) but this CPU is already
			 * busy, let another idle CPU try to pull task.
			 */
			ಹೋಗು ಹೊರಹೋಗು_ಸಮತೋಲಿತ;
		}

		ಆದರೆ (busiest->ಗುಂಪು_type == ಗುಂಪು_smt_balance &&
		    smt_vs_nonsmt_groups(sds.local, sds.busiest)) {
			/* Let non SMT CPU pull from SMT CPU sharing with sibling */
			ಹೋಗು force_balance;
		}

		ಆದರೆ (busiest->ಗುಂಪು_ತೂಕ > 1 &&
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
			ಹೋಗು ಹೊರಹೋಗು_ಸಮತೋಲಿತ;
		}

		ಆದರೆ (busiest->ಮೊತ್ತ_h_nr_running == 1) {
			/*
			 * busiest doesn't have any tasks waiting to run
			 */
			ಹೋಗು ಹೊರಹೋಗು_ಸಮತೋಲಿತ;
		}
	}

force_balance:
	/* Looks like there is an imbalance. Compute it */
	calculate_imbalance(env, &sds);
	ಹಿಂತಿರುಗಿ env->imbalance ? sds.busiest : ಖಾಲಿ;

ಹೊರಹೋಗು_ಸಮತೋಲಿತ:
	env->imbalance = 0;
	ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

/*
 * sched_balance_find_src_rq - find the busiest runqueue among the CPUs in the group.
 */
ಸ್ಥಿರ ರಚನೆ rq *sched_balance_ಕಂಡುಹಿಡಿ_src_rq(ರಚನೆ lb_env *env,
				     ರಚನೆ sched_ಗುಂಪು *ಗುಂಪು)
{
	ರಚನೆ rq *busiest = ಖಾಲಿ, *rq;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ busiest_util = 0, busiest_ತುಂಬು = 0, busiest_ಸಾಮರ್ಥ್ಯ = 1;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ __maybe_unused busiest_pref_llc = 0;
	ರಚನೆ sched_domain __maybe_unused *sd_tmp;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ busiest_nr = 0;
	ಪೂರ್ಣಾಂಕ __maybe_unused dst_llc;
	ಪೂರ್ಣಾಂಕ i;

	for_each_cpu_and(i, sched_ಗುಂಪು_ಹರವು(ಗುಂಪು), env->cpus) {
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಸಾಮರ್ಥ್ಯ, ತುಂಬು, util;
		ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ nr_running;
		ಎಣಿಕೆ fbq_type rt;

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
		ಆದರೆ (rt > env->fbq_type)
			ಮುಂದುವರಿ;

		nr_running = rq->cfs.h_nr_runnable;
		ಆದರೆ (!nr_running)
			ಮುಂದುವರಿ;

		ಸಾಮರ್ಥ್ಯ = ಸಾಮರ್ಥ್ಯ_ದ(i);

		/*
		 * For ASYM_CPUCAPACITY domains, don't pick a CPU that could
		 * eventually lead to active_balancing high->low capacity.
		 * Higher per-CPU capacity is considered better than balancing
		 * average load.
		 */
		ಆದರೆ (env->sd->flags & SD_ASYM_CPUCAPACITY &&
		    nr_running == 1) {
			bool cluster_equal_cap = static_branch_unlikely(&sched_cluster_ಸಕ್ರಿಯ) &&
						 (get_actual_cpu_ಸಾಮರ್ಥ್ಯ(env->dst_cpu) ==
						  get_actual_cpu_ಸಾಮರ್ಥ್ಯ(i));
			bool smt_degraded_cap = sched_smt_ಸಕ್ರಿಯ() && !is_core_idle(i);

			/*
			 * Busy SMT siblings reduce the capacity of CPU @i. Do
			 * not skip it in this case.
			 *
			 * CONFIG_SCHED_CLUSTER requires balancing load across
			 * clusters of identical capacity, accounting for
			 * hardware and cpufreq pressure.
			 */
			ಆದರೆ (!smt_degraded_cap && !cluster_equal_cap &&
			    !ಸಾಮರ್ಥ್ಯ_greater(ಸಾಮರ್ಥ್ಯ_ದ(env->dst_cpu), ಸಾಮರ್ಥ್ಯ))
				ಮುಂದುವರಿ;
		}

		/*
		 * Make sure we only pull tasks from a CPU of lower priority
		 * when balancing between SMT siblings.
		 *
		 * If balancing between cores, let lower priority CPUs help
		 * SMT cores with more than one busy sibling.
		 */
		ಆದರೆ (sched_asym(env->sd, i, env->dst_cpu) && nr_running == 1)
			ಮುಂದುವರಿ;

		ಆಯ್ಕೆ (env->migration_type) {
		ಸ್ಥಿತಿ migrate_ತುಂಬು:
			/*
			 * When comparing with load imbalance, use cpu_load()
			 * which is not scaled with the CPU capacity.
			 */
			ತುಂಬು = cpu_ತುಂಬು(rq);

			ಆದರೆ (nr_running == 1 && ತುಂಬು > env->imbalance &&
			    !check_cpu_ಸಾಮರ್ಥ್ಯ(rq, env->sd))
				ನಿಲ್ಲಿಸು;

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
			ಆದರೆ (ತುಂಬು * busiest_ಸಾಮರ್ಥ್ಯ > busiest_ತುಂಬು * ಸಾಮರ್ಥ್ಯ) {
				busiest_ತುಂಬು = ತುಂಬು;
				busiest_ಸಾಮರ್ಥ್ಯ = ಸಾಮರ್ಥ್ಯ;
				busiest = rq;
			}
			ನಿಲ್ಲಿಸು;

		ಸ್ಥಿತಿ migrate_util:
			util = cpu_util_cfs_boost(i);

			/*
			 * Don't try to pull utilization from a CPU with one
			 * running task. Whatever its utilization, we will fail
			 * detach the task.
			 */
			ಆದರೆ (nr_running <= 1)
				ಮುಂದುವರಿ;

			ಆದರೆ (busiest_util < util) {
				busiest_util = util;
				busiest = rq;
			}
			ನಿಲ್ಲಿಸು;

		ಸ್ಥಿತಿ migrate_task:
			ಆದರೆ (busiest_nr < nr_running) {
				busiest_nr = nr_running;
				busiest = rq;
			}
			ನಿಲ್ಲಿಸು;

		ಸ್ಥಿತಿ migrate_misfit:
			/*
			 * For ASYM_CPUCAPACITY domains with misfit tasks we
			 * simply seek the "biggest" misfit task.
			 */
			ಆದರೆ (rq->misfit_task_ತುಂಬು > busiest_ತುಂಬು) {
				busiest_ತುಂಬು = rq->misfit_task_ತುಂಬು;
				busiest = rq;
			}

			ನಿಲ್ಲಿಸು;

		ಸ್ಥಿತಿ migrate_llc_task:
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CACHE
			sd_tmp = rcu_dereference_all(rq->sd);
			dst_llc = llc_id(env->dst_cpu);

			ಆದರೆ (sd_tmp && (ಚಿಹ್ನೆರಹಿತ)dst_llc < sd_tmp->llc_max) {
				ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ this_pref_llc =
					sd_tmp->llc_counts[dst_llc];

				ಆದರೆ (busiest_pref_llc < this_pref_llc) {
					busiest_pref_llc = this_pref_llc;
					busiest = rq;
				}
			}
#ಆದರೆ_ಅಂತ್ಯ
			ನಿಲ್ಲಿಸು;

		}
	}

	ಹಿಂತಿರುಗಿ busiest;
}

/*
 * Max backoff if we encounter pinned tasks. Pretty arbitrary value, but
 * so long as it is large enough.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು MAX_PINNED_INTERVAL	512

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
asym_ಸಕ್ರಿಯ_balance(ರಚನೆ lb_env *env)
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
	ಹಿಂತಿರುಗಿ env->idle && sched_use_asym_prio(env->sd, env->dst_cpu) &&
	       (sched_asym_prefer(env->dst_cpu, env->src_cpu) ||
		!sched_use_asym_prio(env->sd, env->src_cpu));
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
imbalanced_ಸಕ್ರಿಯ_balance(ರಚನೆ lb_env *env)
{
	ರಚನೆ sched_domain *sd = env->sd;

	/*
	 * The imbalanced case includes the case of pinned tasks preventing a fair
	 * distribution of the load on the system but also the even distribution of the
	 * threads on a system with spare capacity
	 */
	ಆದರೆ ((env->migration_type == migrate_task) &&
	    (sd->nr_balance_failed > sd->cache_nice_tries+2))
		ಹಿಂತಿರುಗಿ 1;

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ need_ಸಕ್ರಿಯ_balance(ರಚನೆ lb_env *env)
{
	ರಚನೆ sched_domain *sd = env->sd;

	ಆದರೆ (alb_break_llc(env))
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ (asym_ಸಕ್ರಿಯ_balance(env))
		ಹಿಂತಿರುಗಿ 1;

	ಆದರೆ (imbalanced_ಸಕ್ರಿಯ_balance(env))
		ಹಿಂತಿರುಗಿ 1;

	/*
	 * The dst_cpu is idle and the src_cpu CPU has only 1 CFS task.
	 * It's worth migrating the task if the src_cpu's capacity is reduced
	 * because of other sched_class or IRQs if more capacity stays
	 * available on dst_cpu.
	 */
	ಆದರೆ (env->idle &&
	    (env->src_rq->cfs.h_nr_runnable == 1)) {
		ಆದರೆ ((check_cpu_ಸಾಮರ್ಥ್ಯ(env->src_rq, sd)) &&
		    (ಸಾಮರ್ಥ್ಯ_ದ(env->src_cpu)*sd->imbalance_pct < ಸಾಮರ್ಥ್ಯ_ದ(env->dst_cpu)*100))
			ಹಿಂತಿರುಗಿ 1;
	}

	ಆದರೆ (env->migration_type == migrate_misfit ||
	    env->migration_type == migrate_llc_task)
		ಹಿಂತಿರುಗಿ 1;

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಸಕ್ರಿಯ_ತುಂಬು_balance_cpu_stop(ಶೂನ್ಯ *ದತ್ತಾಂಶ);
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಸಕ್ರಿಯ_ತುಂಬು_balance_llc_cpu_stop(ಶೂನ್ಯ *ದತ್ತಾಂಶ);

/*
 * migration_type is checked elsewhere to decide migration policy, so
 * it shouldn't be repurposed just to flag an LLC-directed active
 * balance across the stopper. Pick the callback here instead.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ cpu_stop_fn_t alb_stop_fn(ರಚನೆ lb_env *env)
{
	ಆದರೆ (env->migration_type == migrate_llc_task)
		ಹಿಂತಿರುಗಿ ಸಕ್ರಿಯ_ತುಂಬು_balance_llc_cpu_stop;

	ಹಿಂತಿರುಗಿ ಸಕ್ರಿಯ_ತುಂಬು_balance_cpu_stop;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ should_we_balance(ರಚನೆ lb_env *env)
{
	ರಚನೆ cpumask *swb_cpus = this_cpu_cpumask_var_ptr(should_we_balance_tmpmask);
	ರಚನೆ sched_ಗುಂಪು *sg = env->sd->groups;
	ಪೂರ್ಣಾಂಕ cpu, idle_smt = -1;

	/*
	 * Ensure the balancing environment is consistent; can happen
	 * when the softirq triggers 'during' hotplug.
	 */
	ಆದರೆ (!cpumask_test_cpu(env->dst_cpu, env->cpus))
		ಹಿಂತಿರುಗಿ 0;

	/*
	 * In the newly idle case, we will allow all the CPUs
	 * to do the newly idle load balance.
	 *
	 * However, we bail out if we already have tasks or a wakeup pending,
	 * to optimize wakeup latency.
	 */
	ಆದರೆ (env->idle == CPU_NEWLY_IDLE) {
		ಆದರೆ (env->dst_rq->nr_running > 0 || env->dst_rq->ttwu_pending)
			ಹಿಂತಿರುಗಿ 0;
		ಹಿಂತಿರುಗಿ 1;
	}

	cpumask_copy(swb_cpus, ಗುಂಪು_balance_ಮುಖವಾಡ(sg));
	/* Try to find first idle CPU */
	for_each_cpu_and(cpu, swb_cpus, env->cpus) {
		ಆದರೆ (!idle_cpu(cpu))
			ಮುಂದುವರಿ;

		/*
		 * Don't balance to idle SMT in busy core right away when
		 * balancing cores, but remember the first idle SMT CPU for
		 * later consideration.  Find CPU on an idle core first.
		 */
		ಆದರೆ (sched_smt_ಸಕ್ರಿಯ() &&
		    !(env->sd->flags & SD_SHARE_CPUCAPACITY) &&
		    !is_core_idle(cpu)) {
			ಆದರೆ (idle_smt == -1)
				idle_smt = cpu;
			/*
			 * If the core is not idle, and first SMT sibling which is
			 * idle has been found, then its not needed to check other
			 * SMT siblings for idleness:
			 */
			cpumask_andnot(swb_cpus, swb_cpus, cpu_smt_ಮುಖವಾಡ(cpu));
			ಮುಂದುವರಿ;
		}

		/*
		 * Are we the first idle core in a non-SMT domain or higher,
		 * or the first idle CPU in a SMT domain?
		 */
		ಹಿಂತಿರುಗಿ cpu == env->dst_cpu;
	}

	/* Are we the first idle CPU with busy siblings? */
	ಆದರೆ (idle_smt != -1)
		ಹಿಂತಿರುಗಿ idle_smt == env->dst_cpu;

	/* Are we the first CPU of this group ? */
	ಹಿಂತಿರುಗಿ ಗುಂಪು_balance_cpu(sg) == env->dst_cpu;
}

ಸ್ಥಿರ ಶೂನ್ಯ update_lb_imbalance_stat(ರಚನೆ lb_env *env, ರಚನೆ sched_domain *sd,
				     ಎಣಿಕೆ cpu_idle_type idle)
{
	ಆದರೆ (!schedstat_enabled())
		ಹಿಂತಿರುಗಿ;

	ಆಯ್ಕೆ (env->migration_type) {
	ಸ್ಥಿತಿ migrate_ತುಂಬು:
		__schedstat_add(sd->lb_imbalance_ತುಂಬು[idle], env->imbalance);
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ migrate_util:
		__schedstat_add(sd->lb_imbalance_util[idle], env->imbalance);
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ migrate_task:
		__schedstat_add(sd->lb_imbalance_task[idle], env->imbalance);
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ migrate_misfit:
		__schedstat_add(sd->lb_imbalance_misfit[idle], env->imbalance);
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ migrate_llc_task:
		ನಿಲ್ಲಿಸು;
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
ಸ್ಥಿರ atomic_t sched_balance_running = ATOMIC_INIT(0);

/*
 * Check this_cpu to ensure it is balanced within domain. Attempt to move
 * tasks if there is an imbalance.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sched_balance_rq(ಪೂರ್ಣಾಂಕ this_cpu, ರಚನೆ rq *this_rq,
			ರಚನೆ sched_domain *sd, ಎಣಿಕೆ cpu_idle_type idle,
			ಪೂರ್ಣಾಂಕ *continue_balancing)
{
	ಪೂರ್ಣಾಂಕ ld_moved, cur_ld_moved, ಸಕ್ರಿಯ_balance = 0;
	ರಚನೆ sched_domain *sd_ಪೋಷಕ = sd->ಪೋಷಕ;
	ರಚನೆ sched_ಗುಂಪು *ಗುಂಪು;
	ರಚನೆ rq *busiest;
	ರಚನೆ rq_flags rf;
	ರಚನೆ cpumask *cpus = this_cpu_cpumask_var_ptr(ತುಂಬು_balance_ಮುಖವಾಡ);
	ರಚನೆ lb_env env = {
		.sd		= sd,
		.dst_cpu	= this_cpu,
		.dst_rq		= this_rq,
		.dst_grpmask    = ಗುಂಪು_balance_ಮುಖವಾಡ(sd->groups),
		.idle		= idle,
		.loop_break	= SCHED_NR_MIGRATE_BREAK,
		.cpus		= cpus,
		.fbq_type	= all,
		.tasks		= LIST_ತಲೆ_INIT(env.tasks),
	};
	bool need_unlock = false;

	cpumask_and(cpus, sched_domain_ಹರವು(sd), cpu_preferred_ಮುಖವಾಡ);

	schedstat_inc(sd->lb_ಎಣಿಕೆ[idle]);

redo:
	ಆದರೆ (!should_we_balance(&env)) {
		*continue_balancing = 0;
		ಹೋಗು ಹೊರಹೋಗು_ಸಮತೋಲಿತ;
	}

	ಆದರೆ (!need_unlock && (sd->flags & SD_SERIALIZE)) {
		ಪೂರ್ಣಾಂಕ zero = 0;
		ಆದರೆ (!atomic_try_cmpxchg_acquire(&sched_balance_running, &zero, 1))
			ಹೋಗು ಹೊರಹೋಗು_ಸಮತೋಲಿತ;

		need_unlock = true;
	}

	ಗುಂಪು = sched_balance_ಕಂಡುಹಿಡಿ_src_ಗುಂಪು(&env);
	ಆದರೆ (!ಗುಂಪು) {
		schedstat_inc(sd->lb_nobusyg[idle]);
		ಹೋಗು ಹೊರಹೋಗು_ಸಮತೋಲಿತ;
	}

	busiest = sched_balance_ಕಂಡುಹಿಡಿ_src_rq(&env, ಗುಂಪು);
	ಆದರೆ (!busiest) {
		schedstat_inc(sd->lb_nobusyq[idle]);
		ಹೋಗು ಹೊರಹೋಗು_ಸಮತೋಲಿತ;
	}

	WARN_ON_ONCE(busiest == env.dst_rq);

	update_lb_imbalance_stat(&env, sd, idle);

	env.src_cpu = busiest->cpu;
	env.src_rq = busiest;

	ld_moved = 0;
	/* Clear this flag as soon as we find a pullable task */
	env.flags |= LBF_ALL_PINNED;
	ಆದರೆ (busiest->nr_running > 1) {
		/*
		 * Attempt to move tasks. If sched_balance_find_src_group has found
		 * an imbalance but busiest->nr_running <= 1, the group is
		 * still unbalanced. ld_moved simply stays zero, so it is
		 * correctly treated as an imbalance.
		 */
		env.loop_max  = min(sysctl_sched_nr_migrate, busiest->nr_running);

ಇನ್ನಷ್ಟು_balance:
		rq_ಬೀಗ_irqsave(busiest, &rf);
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

		ಆದರೆ (cur_ld_moved) {
			attach_tasks(&env);
			ld_moved += cur_ld_moved;
		}

		local_irq_restore(rf.flags);

		ಆದರೆ (env.flags & LBF_NEED_BREAK) {
			env.flags &= ~LBF_NEED_BREAK;
			ಹೋಗು ಇನ್ನಷ್ಟು_balance;
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
		ಆದರೆ ((env.flags & LBF_DST_PINNED) && env.imbalance > 0) {

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
			ಹೋಗು ಇನ್ನಷ್ಟು_balance;
		}

		/*
		 * We failed to reach balance because of affinity.
		 */
		ಆದರೆ (sd_ಪೋಷಕ) {
			ಪೂರ್ಣಾಂಕ *ಗುಂಪು_imbalance = &sd_ಪೋಷಕ->groups->sgc->imbalance;

			ಆದರೆ ((env.flags & LBF_SOME_PINNED) && env.imbalance > 0)
				*ಗುಂಪು_imbalance = 1;
		}

		/* All tasks on this runqueue were pinned by CPU affinity */
		ಆದರೆ (unlikely(env.flags & LBF_ALL_PINNED)) {
			__cpumask_clear_cpu(cpu_ದ(busiest), cpus);
			/*
			 * Attempting to continue load balancing at the current
			 * sched_domain level only makes sense if there are
			 * active CPUs remaining as possible busiest CPUs to
			 * pull load from which are not contained within the
			 * destination group that is receiving any migrated
			 * load.
			 */
			ಆದರೆ (!cpumask_subset(cpus, env.dst_grpmask)) {
				env.loop = 0;
				env.loop_break = SCHED_NR_MIGRATE_BREAK;
				ಹೋಗು redo;
			}
			ಹೋಗು ಹೊರಹೋಗು_all_pinned;
		}
	}

	ಆದರೆ (ld_moved) {
		sd->nr_balance_failed = 0;
		ಹೋಗು ಹೊರಹೋಗು_unbalanced;
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
	ಆದರೆ (idle != CPU_NEWLY_IDLE &&
	    env.migration_type != migrate_misfit &&
	    !(env.flags & LBF_LLC_PINNED))
		sd->nr_balance_failed++;

	ಆದರೆ (!need_ಸಕ್ರಿಯ_balance(&env))
		ಹೋಗು ಹೊರಹೋಗು_unbalanced;

	scoped_guard (raw_spin_rq_ಬೀಗ_irqsave, busiest) {
		/*
		 * Don't kick the active_load_balance_cpu_stop,
		 * if the curr task on busiest CPU can't be
		 * moved to this_cpu:
		 */
		ಆದರೆ (!cpumask_test_cpu(this_cpu, busiest->curr->cpus_ptr))
			ಹೋಗು ಹೊರಹೋಗು_one_pinned;

		/* Record that we found at least one task that could run on this_cpu */
		env.flags &= ~LBF_ALL_PINNED;

		/*
		 * ->active_balance synchronizes accesses to
		 * ->active_balance_work.  Once set, it's cleared
		 * only after active load balance is finished.
		 */
		ಆದರೆ (busiest->ಸಕ್ರಿಯ_balance)
			ಹೋಗು ಹೊರಹೋಗು_unbalanced;

		/*
		 * @busiest dropped its rq_lock in the middle of
		 * scheduling out its ->curr task (->on_rq := 0), no
		 * need to forcefully punt it away with active balance.
		 */
		ಆದರೆ (!busiest->curr->on_rq)
			ಹೋಗು ಹೊರಹೋಗು_unbalanced;

		busiest->ಸಕ್ರಿಯ_balance = 1;
		busiest->ತಳ್ಳು_cpu = this_cpu;
		ಸಕ್ರಿಯ_balance = 1;
		preempt_disable();
	}
	ಆದರೆ (ಸಕ್ರಿಯ_balance) {
		stop_one_cpu_nowait(cpu_ದ(busiest),
				    alb_stop_fn(&env), busiest,
				    &busiest->ಸಕ್ರಿಯ_balance_work);
	}
	preempt_enable();

ಹೊರಹೋಗು_unbalanced:
	/* We were unbalanced, so reset the balancing interval */
	sd->balance_interval = sd->min_interval;
	ಹೋಗು ಹೊರಹೋಗು;

ಹೊರಹೋಗು_ಸಮತೋಲಿತ:
	/*
	 * We reach balance although we may have faced some affinity
	 * constraints. Clear the imbalance flag only if other tasks got
	 * a chance to move and fix the imbalance.
	 */
	ಆದರೆ (sd_ಪೋಷಕ && !(env.flags & LBF_ALL_PINNED)) {
		ಪೂರ್ಣಾಂಕ *ಗುಂಪು_imbalance = &sd_ಪೋಷಕ->groups->sgc->imbalance;

		ಆದರೆ (*ಗುಂಪು_imbalance)
			*ಗುಂಪು_imbalance = 0;
	}

ಹೊರಹೋಗು_all_pinned:
	/*
	 * We reach balance because all tasks are pinned at this level so
	 * we can't migrate them. Let the imbalance flag set so parent level
	 * can try to migrate them.
	 */
	schedstat_inc(sd->lb_ಸಮತೋಲಿತ[idle]);

	sd->nr_balance_failed = 0;

ಹೊರಹೋಗು_one_pinned:
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
	ಆದರೆ (env.idle == CPU_NEWLY_IDLE ||
	    env.migration_type == migrate_misfit)
		ಹೋಗು ಹೊರಹೋಗು;

	/* tune up the balancing interval */
	ಆದರೆ ((env.flags & LBF_ALL_PINNED &&
	     sd->balance_interval < MAX_PINNED_INTERVAL) ||
	    sd->balance_interval < sd->max_interval)
		sd->balance_interval *= 2;
ಹೊರಹೋಗು:
	ಆದರೆ (need_unlock)
		atomic_ಗಣ_release(&sched_balance_running, 0);

	ಹಿಂತಿರುಗಿ ld_moved;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ
get_sd_balance_interval(ರಚನೆ sched_domain *sd, ಪೂರ್ಣಾಂಕ cpu_busy)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ interval = sd->balance_interval;

	ಆದರೆ (cpu_busy)
		interval *= sd->busy_ಗುಣಕ;

	/* scale ms to jiffies */
	interval = msecs_ಗಮ್ಯ_jiffies(interval);

	/*
	 * Reduce likelihood of busy balancing at higher domains racing with
	 * balancing at lower domains by preventing their balancing periods
	 * from being multiples of each other.
	 */
	ಆದರೆ (cpu_busy)
		interval -= 1;

	interval = clamp(interval, 1UL, max_ತುಂಬು_balance_interval);

	ಹಿಂತಿರುಗಿ interval;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
update_ಮುಂದಿನ_balance(ರಚನೆ sched_domain *sd, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ *ಮುಂದಿನ_balance)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ interval, ಮುಂದಿನ;

	/* used by idle balance, so cpu_busy = 0 */
	interval = get_sd_balance_interval(sd, 0);
	ಮುಂದಿನ = sd->ಕೊನೆಯ_balance + interval;

	ಆದರೆ (ಸಮಯ_after(*ಮುಂದಿನ_balance, ಮುಂದಿನ))
		*ಮುಂದಿನ_balance = ಮುಂದಿನ;
}

/*
 * active_load_balance_cpu_stop is run by the CPU stopper. It pushes
 * running tasks off the busiest CPU onto idle CPUs. It requires at
 * least 1 task to be running on each physical CPU where possible, and
 * avoids physical / logical imbalances.
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ __active_load_balance_cpu_stop(ಶೂನ್ಯ *ದತ್ತಾಂಶ, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ lb_flags)
{
	ರಚನೆ rq *busiest_rq = ದತ್ತಾಂಶ;
	ಪೂರ್ಣಾಂಕ busiest_cpu = cpu_ದ(busiest_rq);
	ಪೂರ್ಣಾಂಕ ಗುರಿ_cpu = busiest_rq->ತಳ್ಳು_cpu;
	ರಚನೆ rq *ಗುರಿ_rq = cpu_rq(ಗುರಿ_cpu);
	ರಚನೆ sched_domain *sd;
	ರಚನೆ task_struct *p = ಖಾಲಿ;
	ರಚನೆ rq_flags rf;

	rq_ಬೀಗ_irq(busiest_rq, &rf);
	/*
	 * Between queueing the stop-work and running it is a hole in which
	 * CPUs can become inactive. We should not move tasks from or to
	 * inactive CPUs.
	 */
	ಆದರೆ (!cpu_ಸಕ್ರಿಯ(busiest_cpu) || !cpu_ಸಕ್ರಿಯ(ಗುರಿ_cpu))
		ಹೋಗು ಹೊರಹೋಗು_unlock;

	/* Make sure the requested CPU hasn't gone down in the meantime: */
	ಆದರೆ (unlikely(busiest_cpu != smp_processor_id() ||
		     !busiest_rq->ಸಕ್ರಿಯ_balance))
		ಹೋಗು ಹೊರಹೋಗು_unlock;

	/* Is there any task to move? */
	ಆದರೆ (busiest_rq->nr_running <= 1)
		ಹೋಗು ಹೊರಹೋಗು_unlock;

	/*
	 * This condition is "impossible", if it occurs
	 * we need to fix it. Originally reported by
	 * Bjorn Helgaas on a 128-CPU setup.
	 */
	WARN_ON_ONCE(busiest_rq == ಗುರಿ_rq);

	/* Search for an sd spanning us and the target CPU. */
	rcu_read_ಬೀಗ();
	for_each_domain(ಗುರಿ_cpu, sd) {
		ಆದರೆ (cpumask_test_cpu(busiest_cpu, sched_domain_ಹರವು(sd)))
			ನಿಲ್ಲಿಸು;
	}

	ಆದರೆ (likely(sd)) {
		ರಚನೆ lb_env env = {
			.sd		= sd,
			.dst_cpu	= ಗುರಿ_cpu,
			.dst_rq		= ಗುರಿ_rq,
			.src_cpu	= busiest_rq->cpu,
			.src_rq		= busiest_rq,
			.idle		= CPU_IDLE,
			.flags		= LBF_ಸಕ್ರಿಯ_LB | lb_flags,
		};

		schedstat_inc(sd->alb_ಎಣಿಕೆ);
		update_rq_clock(busiest_rq);

		p = detach_one_task(&env);
		ಆದರೆ (p) {
			schedstat_inc(sd->alb_pushed);
			/* Active balancing done, reset the failure counter. */
			sd->nr_balance_failed = 0;
		} ಇಲ್ಲದಿದ್ದರೆ {
			schedstat_inc(sd->alb_failed);
		}
	}
	rcu_read_unlock();
ಹೊರಹೋಗು_unlock:
	busiest_rq->ಸಕ್ರಿಯ_balance = 0;
	rq_unlock(busiest_rq, &rf);

	ಆದರೆ (p)
		attach_one_task(ಗುರಿ_rq, p);

	local_irq_enable();

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಸಕ್ರಿಯ_ತುಂಬು_balance_cpu_stop(ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	ಹಿಂತಿರುಗಿ __active_load_balance_cpu_stop(ದತ್ತಾಂಶ, 0);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಸಕ್ರಿಯ_ತುಂಬು_balance_llc_cpu_stop(ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	ಹಿಂತಿರುಗಿ __active_load_balance_cpu_stop(ದತ್ತಾಂಶ, LBF_ಸಕ್ರಿಯ_LB_LLC);
}

/*
 * Scale the max sched_balance_rq interval with the number of CPUs in the system.
 * This trades load-balance latency on larger machines for less cross talk.
 */
ಶೂನ್ಯ update_max_interval(ಶೂನ್ಯ)
{
	max_ತುಂಬು_balance_interval = HZ*ಅಂಕೆ_online_cpus()/10;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ update_newidle_stats(ರಚನೆ sched_domain *sd, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ success)
{
	sd->newidle_call++;
	sd->newidle_success += success;

	ಆದರೆ (sd->newidle_call >= 1024) {
		u64 now = sched_clock();
		s64 delta = now - sd->newidle_stamp;
		sd->newidle_stamp = now;
		ಪೂರ್ಣಾಂಕ ಅನುಪಾತ = 0;

		ಆದರೆ (delta < 0)
			delta = 0;

		ಆದರೆ (sched_feat(NI_ದರ)) {
			/*
			 * ratio  delta   freq
			 *
			 * 1024 -  4  s -  128 Hz
			 *  512 -  2  s -  256 Hz
			 *  256 -  1  s -  512 Hz
			 *  128 - .5  s - 1024 Hz
			 *   64 - .25 s - 2048 Hz
			 */
			ಅನುಪಾತ = delta >> 22;
		}

		ಅನುಪಾತ += sd->newidle_success;

		sd->newidle_ಅನುಪಾತ = min(1024, ಅನುಪಾತ);
		sd->newidle_call /= 2;
		sd->newidle_success /= 2;
	}
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
update_newidle_ವೆಚ್ಚ(ರಚನೆ sched_domain *sd, u64 ವೆಚ್ಚ, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ success)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಮುಂದಿನ_decay = sd->ಕೊನೆಯ_decay_max_lb_ವೆಚ್ಚ + HZ;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ now = jiffies;

	ಆದರೆ (ವೆಚ್ಚ)
		update_newidle_stats(sd, success);

	ಆದರೆ (ವೆಚ್ಚ > sd->max_newidle_lb_ವೆಚ್ಚ) {
		/*
		 * Track max cost of a domain to make sure to not delay the
		 * next wakeup on the CPU.
		 */
		sd->max_newidle_lb_ವೆಚ್ಚ = ವೆಚ್ಚ;
		sd->ಕೊನೆಯ_decay_max_lb_ವೆಚ್ಚ = now;

	} ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಸಮಯ_after(now, ಮುಂದಿನ_decay)) {
		/*
		 * Decay the newidle max times by ~1% per second to ensure that
		 * it is not outdated and the current max cost is actually
		 * shorter.
		 */
		sd->max_newidle_lb_ವೆಚ್ಚ = (sd->max_newidle_lb_ವೆಚ್ಚ * 253) / 256;
		sd->ಕೊನೆಯ_decay_max_lb_ವೆಚ್ಚ = now;
		ಹಿಂತಿರುಗಿ true;
	}

	ಹಿಂತಿರುಗಿ false;
}

/*
 * It checks each scheduling domain to see if it is due to be balanced,
 * and initiates a balancing operation if so.
 *
 * Balancing parameters are set up in init_sched_domains.
 */
ಸ್ಥಿರ ಶೂನ್ಯ sched_balance_domains(ರಚನೆ rq *rq, ಎಣಿಕೆ cpu_idle_type idle)
{
	ಪೂರ್ಣಾಂಕ continue_balancing = 1;
	ಪೂರ್ಣಾಂಕ cpu = rq->cpu;
	ಪೂರ್ಣಾಂಕ busy = idle != CPU_IDLE && !sched_idle_rq(rq);
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ interval;
	ರಚನೆ sched_domain *sd;
	/* Earliest time when we have to do rebalance again */
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಮುಂದಿನ_balance = jiffies + 60*HZ;
	ಪೂರ್ಣಾಂಕ update_ಮುಂದಿನ_balance = 0;
	ಪೂರ್ಣಾಂಕ need_decay = 0;
	u64 max_ವೆಚ್ಚ = 0;

	rcu_read_ಬೀಗ();
	for_each_domain(cpu, sd) {
		/*
		 * Decay the newidle max times here because this is a regular
		 * visit to all the domains.
		 */
		need_decay = update_newidle_ವೆಚ್ಚ(sd, 0, 0);
		max_ವೆಚ್ಚ += sd->max_newidle_lb_ವೆಚ್ಚ;

		/*
		 * Stop the load balance at this level. There is another
		 * CPU in our sched group which is doing load balancing more
		 * actively.
		 */
		ಆದರೆ (!continue_balancing) {
			ಆದರೆ (need_decay)
				ಮುಂದುವರಿ;
			ನಿಲ್ಲಿಸು;
		}

		interval = get_sd_balance_interval(sd, busy);
		ಆದರೆ (ಸಮಯ_after_eq(jiffies, sd->ಕೊನೆಯ_balance + interval)) {
			ಆದರೆ (sched_balance_rq(cpu, rq, sd, idle, &continue_balancing)) {
				/*
				 * The LBF_DST_PINNED logic could have changed
				 * env->dst_cpu, so we can't know our idle
				 * state even if we migrated tasks. Update it.
				 */
				idle = idle_cpu(cpu);
				busy = !idle && !sched_idle_rq(rq);
			}
			sd->ಕೊನೆಯ_balance = jiffies;
			interval = get_sd_balance_interval(sd, busy);
		}
		ಆದರೆ (ಸಮಯ_after(ಮುಂದಿನ_balance, sd->ಕೊನೆಯ_balance + interval)) {
			ಮುಂದಿನ_balance = sd->ಕೊನೆಯ_balance + interval;
			update_ಮುಂದಿನ_balance = 1;
		}
	}
	ಆದರೆ (need_decay) {
		/*
		 * Ensure the rq-wide value also decays but keep it at a
		 * reasonable floor to avoid funnies with rq->avg_idle.
		 */
		rq->max_idle_balance_ವೆಚ್ಚ =
			max((u64)sysctl_sched_migration_ವೆಚ್ಚ, max_ವೆಚ್ಚ);
	}
	rcu_read_unlock();

	/*
	 * next_balance will be updated only when there is a need.
	 * When the cpu is attached to null domain for ex, it will not be
	 * updated.
	 */
	ಆದರೆ (likely(update_ಮುಂದಿನ_balance))
		rq->ಮುಂದಿನ_balance = ಮುಂದಿನ_balance;

}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ on_null_domain(ರಚನೆ rq *rq)
{
	ಹಿಂತಿರುಗಿ unlikely(!rcu_dereference_sched(rq->sd));
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_NO_HZ_COMMON
/*
 * NOHZ idle load balancing (ILB) details:
 *
 * - When one of the busy CPUs notices that there may be an idle rebalancing
 *   needed, they will kick the idle load balancer, which then does idle
 *   load balancing for all the idle CPUs.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ ಕಂಡುಹಿಡಿ_new_ilb(ಶೂನ್ಯ)
{
	ರಚನೆ cpumask *ilb_cpus;
	ಪೂರ್ಣಾಂಕ ilb_cpu, fallback = -1;

	lockdep_assert_irqs_disabled();

	/*
	 * Reuse the per-CPU select_rq_mask, which is protected from concurrent
	 * use on this CPU by having interrupts disabled.
	 */
	ilb_cpus = this_cpu_cpumask_var_ptr(select_rq_ಮುಖವಾಡ);
	cpumask_and(ilb_cpus, nohz.idle_cpus_ಮುಖವಾಡ,
		    housekeeping_cpumask(HK_TYPE_KERNEL_NOISE));

	for_each_cpu(ilb_cpu, ilb_cpus) {
		ಆದರೆ (!idle_cpu(ilb_cpu)) {
			/*
			 * Once an idle fallback exists, a busy CPU proves that
			 * this core cannot be fully idle. Skip its siblings.
			 */
			ಆದರೆ (sched_smt_ಸಕ್ರಿಯ() && fallback >= 0)
				cpumask_andnot(ilb_cpus, ilb_cpus, cpu_smt_ಮುಖವಾಡ(ilb_cpu));
			ಮುಂದುವರಿ;
		}

		/*
		 * Running the idle load balancer on an idle sibling of a busy
		 * SMT core can reduce the capacity available to its sibling. Prefer
		 * a CPU whose entire core is idle, but retain the first idle CPU as
		 * a fallback so idle balancing can still make progress when no fully
		 * idle core exists.
		 */
		ಆದರೆ (sched_smt_ಸಕ್ರಿಯ() && !is_core_idle(ilb_cpu)) {
			ಆದರೆ (fallback < 0)
				fallback = ilb_cpu;

			/*
			 * The core is not idle, so there is no need to check
			 * any of its other SMT siblings.
			 */
			cpumask_andnot(ilb_cpus, ilb_cpus,
				       cpu_smt_ಮುಖವಾಡ(ilb_cpu));
			ಮುಂದುವರಿ;
		}

		ಹಿಂತಿರುಗಿ ilb_cpu;
	}

	ಹಿಂತಿರುಗಿ fallback;
}

/*
 * Kick a CPU to do the NOHZ balancing, if it is time for it, via a cross-CPU
 * SMP function call (IPI).
 *
 * Prefer a CPU on a fully idle core in the HK_TYPE_KERNEL_NOISE housekeeping
 * set. Fall back to the first idle CPU when no fully idle core exists.
 */
ಸ್ಥಿರ ಶೂನ್ಯ kick_ilb(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags)
{
	ಪೂರ್ಣಾಂಕ ilb_cpu;

	/*
	 * Increase nohz.next_balance only when if full ilb is triggered but
	 * not if we only update stats.
	 */
	ಆದರೆ (flags & NOHZ_BALANCE_KICK)
		nohz.ಮುಂದಿನ_balance = jiffies+1;

	ilb_cpu = ಕಂಡುಹಿಡಿ_new_ilb();
	ಆದರೆ (ilb_cpu < 0)
		ಹಿಂತಿರುಗಿ;

	/*
	 * Don't bother if no new NOHZ balance work items for ilb_cpu,
	 * i.e. all bits in flags are already set in ilb_cpu.
	 */
	ಆದರೆ ((atomic_read(nohz_flags(ilb_cpu)) & flags) == flags)
		ಹಿಂತಿರುಗಿ;

	/*
	 * Access to rq::nohz_csd is serialized by NOHZ_KICK_MASK; he who sets
	 * the first flag owns it; cleared by nohz_csd_func().
	 */
	flags = atomic_fetch_or(flags, nohz_flags(ilb_cpu));
	ಆದರೆ (flags & NOHZ_KICK_ಮುಖವಾಡ)
		ಹಿಂತಿರುಗಿ;

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
ಸ್ಥಿರ ಶೂನ್ಯ nohz_balancer_kick(ರಚನೆ rq *rq)
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ now = jiffies;
	ರಚನೆ sched_domain_shared *sds;
	ರಚನೆ sched_domain *sd;
	ಪೂರ್ಣಾಂಕ nr_busy, i, cpu = rq->cpu;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags = 0;

	ಆದರೆ (unlikely(rq->idle_balance))
		ಹಿಂತಿರುಗಿ;

	/*
	 * We may be recently in ticked or tickless idle mode. At the first
	 * busy tick after returning from idle, we will update the busy stats.
	 */
	nohz_balance_exit_idle(rq);

	ಆದರೆ (READ_ONCE(nohz.has_blocked_ತುಂಬು) &&
	    ಸಮಯ_after(now, READ_ONCE(nohz.ಮುಂದಿನ_blocked)))
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
	ಆದರೆ (ಸಮಯ_before(now, nohz.ಮುಂದಿನ_balance))
		ಹೋಗು ಹೊರಹೋಗು;

	/*
	 * None are in tickless mode and hence no need for NOHZ idle load
	 * balancing
	 */
	ಆದರೆ (unlikely(cpumask_ಖಾಲಿಗಳು(nohz.idle_cpus_ಮುಖವಾಡ)))
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (rq->nr_running >= 2) {
		flags = NOHZ_STATS_KICK | NOHZ_BALANCE_KICK;
		ಹೋಗು ಹೊರಹೋಗು;
	}

	sd = rcu_dereference_all(rq->sd);
	ಆದರೆ (sd) {
		/*
		 * If there's a runnable CFS task and the current CPU has reduced
		 * capacity, kick the ILB to see if there's a better CPU to run on:
		 */
		ಆದರೆ (rq->cfs.h_nr_runnable >= 1 && check_cpu_ಸಾಮರ್ಥ್ಯ(rq, sd)) {
			flags |= NOHZ_STATS_KICK | NOHZ_BALANCE_KICK;
			ಹೋಗು ಹೊರಹೋಗು;
		}
	}

	sd = rcu_dereference_all(ಪ್ರತಿ_cpu(sd_asym_packing, cpu));
	ಆದರೆ (sd) {
		/*
		 * When ASYM_PACKING; see if there's a more preferred CPU
		 * currently idle; in which case, kick the ILB to move tasks
		 * around.
		 *
		 * When balancing between cores, all the SMT siblings of the
		 * preferred CPU must be idle.
		 */
		for_each_cpu_and(i, sched_domain_ಹರವು(sd), nohz.idle_cpus_ಮುಖವಾಡ) {
			ಆದರೆ (sched_asym(sd, i, cpu)) {
				flags |= NOHZ_STATS_KICK | NOHZ_BALANCE_KICK;
				ಹೋಗು ಹೊರಹೋಗು;
			}
		}
	}

	sd = rcu_dereference_all(ಪ್ರತಿ_cpu(sd_asym_cpucapacity, cpu));
	ಆದರೆ (sd) {
		/*
		 * When ASYM_CPUCAPACITY; see if there's a higher capacity CPU
		 * to run the misfit task on.
		 */
		ಆದರೆ (check_misfit_ಸ್ಥಿತಿ(rq))
			flags |= NOHZ_STATS_KICK | NOHZ_BALANCE_KICK;

		/*
		 * For asymmetric systems, we do not want to nicely balance
		 * cache use, instead we want to embrace asymmetry and only
		 * ensure tasks have enough CPU capacity.
		 *
		 * Skip the LLC logic because it's not relevant in that case.
		 */
		ಹೋಗು ಹೊರಹೋಗು;
	}

	sds = rcu_dereference_all(ಪ್ರತಿ_cpu(sd_balance_shared, cpu));
	ಆದರೆ (sds) {
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
		ಆದರೆ (nr_busy > 1)
			flags |= NOHZ_STATS_KICK | NOHZ_BALANCE_KICK;
	}
ಹೊರಹೋಗು:
	ಆದರೆ (READ_ONCE(nohz.needs_update))
		flags |= NOHZ_ಮುಂದಿನ_KICK;

	ಆದರೆ (flags)
		kick_ilb(flags);
}

ಸ್ಥಿರ ಶೂನ್ಯ ಗಣ_cpu_sd_state_busy(ಪೂರ್ಣಾಂಕ cpu)
{
	ರಚನೆ sched_domain *sd;
	sd = rcu_dereference_all(ಪ್ರತಿ_cpu(sd_llc, cpu));

	/*
	 * sd->nohz_idle only pairs with nr_busy_cpus on sd->shared; if this
	 * domain has no shared object there is nothing to clear or account.
	 */
	ಆದರೆ (!sd || !sd->shared || !sd->nohz_idle)
		ಹಿಂತಿರುಗಿ;
	sd->nohz_idle = 0;

	atomic_inc(&sd->shared->nr_busy_cpus);
}

ಶೂನ್ಯ nohz_balance_exit_idle(ರಚನೆ rq *rq)
{
	WARN_ON_ONCE(rq != this_rq());

	ಆದರೆ (likely(!rq->nohz_tick_stopped))
		ಹಿಂತಿರುಗಿ;

	rq->nohz_tick_stopped = 0;
	cpumask_clear_cpu(rq->cpu, nohz.idle_cpus_ಮುಖವಾಡ);

	ಗಣ_cpu_sd_state_busy(rq->cpu);
}

ಸ್ಥಿರ ಶೂನ್ಯ ಗಣ_cpu_sd_state_idle(ಪೂರ್ಣಾಂಕ cpu)
{
	ರಚನೆ sched_domain *sd;
	sd = rcu_dereference_all(ಪ್ರತಿ_cpu(sd_llc, cpu));

	/* See set_cpu_sd_state_busy(): nohz_idle is only used with sd->shared. */
	ಆದರೆ (!sd || !sd->shared || sd->nohz_idle)
		ಹಿಂತಿರುಗಿ;
	sd->nohz_idle = 1;

	atomic_dec(&sd->shared->nr_busy_cpus);
}

/*
 * This routine will record that the CPU is going idle with tick stopped.
 * This info will be used in performing idle load balancing in the future.
 */
ಶೂನ್ಯ nohz_balance_enter_idle(ಪೂರ್ಣಾಂಕ cpu)
{
	ರಚನೆ rq *rq = cpu_rq(cpu);

	WARN_ON_ONCE(cpu != smp_processor_id());

	/* If this CPU is going down, then nothing needs to be done: */
	ಆದರೆ (!cpu_ಸಕ್ರಿಯ(cpu))
		ಹಿಂತಿರುಗಿ;

	/*
	 * Can be set safely without rq->lock held
	 * If a clear happens, it will have evaluated last additions because
	 * rq->lock is held during the check and the clear
	 */
	rq->has_blocked_ತುಂಬು = 1;

	/*
	 * The tick is still stopped but load could have been added in the
	 * meantime. We set the nohz.has_blocked_load flag to trig a check of the
	 * *_avg. The CPU is already part of nohz.idle_cpus_mask so the clear
	 * of nohz.has_blocked_load can only happen after checking the new load
	 */
	ಆದರೆ (rq->nohz_tick_stopped)
		ಹೋಗು ಹೊರಹೋಗು;

	/* If we're a completely isolated CPU, we don't play: */
	ಆದರೆ (on_null_domain(rq))
		ಹಿಂತಿರುಗಿ;

	rq->nohz_tick_stopped = 1;

	cpumask_ಗಣ_cpu(cpu, nohz.idle_cpus_ಮುಖವಾಡ);

	/*
	 * Ensures that if nohz_idle_balance() fails to observe our
	 * @idle_cpus_mask store, it must observe the @has_blocked_load
	 * and @needs_update stores.
	 */
	smp_mb__after_atomic();

	ಗಣ_cpu_sd_state_idle(cpu);

	WRITE_ONCE(nohz.needs_update, 1);
ಹೊರಹೋಗು:
	/*
	 * Each time a cpu enter idle, we assume that it has blocked load and
	 * enable the periodic update of the load of idle CPUs
	 */
	WRITE_ONCE(nohz.has_blocked_ತುಂಬು, 1);
}

ಸ್ಥಿರ bool update_nohz_stats(ರಚನೆ rq *rq)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ cpu = rq->cpu;

	ಆದರೆ (!rq->has_blocked_ತುಂಬು)
		ಹಿಂತಿರುಗಿ false;

	ಆದರೆ (!cpumask_test_cpu(cpu, nohz.idle_cpus_ಮುಖವಾಡ))
		ಹಿಂತಿರುಗಿ false;

	ಆದರೆ (!ಸಮಯ_after(jiffies, READ_ONCE(rq->ಕೊನೆಯ_blocked_ತುಂಬು_update_tick)))
		ಹಿಂತಿರುಗಿ true;

	sched_balance_update_blocked_averages(cpu);

	ಹಿಂತಿರುಗಿ rq->has_blocked_ತುಂಬು;
}

/*
 * Internal function that runs load balance for all idle CPUs. The load balance
 * can be a simple update of blocked load or a complete load balance with
 * tasks movement depending of flags.
 */
ಸ್ಥಿರ ಶೂನ್ಯ _nohz_idle_balance(ರಚನೆ rq *this_rq, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags)
{
	/* Earliest time when we have to do rebalance again */
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ now = jiffies;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಮುಂದಿನ_balance = now + 60*HZ;
	bool has_blocked_ತುಂಬು = false;
	ಪೂರ್ಣಾಂಕ update_ಮುಂದಿನ_balance = 0;
	ಪೂರ್ಣಾಂಕ this_cpu = this_rq->cpu;
	ಪೂರ್ಣಾಂಕ balance_cpu;
	ರಚನೆ rq *rq;

	WARN_ON_ONCE((flags & NOHZ_KICK_ಮುಖವಾಡ) == NOHZ_BALANCE_KICK);

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
	ಆದರೆ (flags & NOHZ_STATS_KICK)
		WRITE_ONCE(nohz.has_blocked_ತುಂಬು, 0);
	ಆದರೆ (flags & NOHZ_ಮುಂದಿನ_KICK)
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
	for_each_cpu_wrap(balance_cpu,  nohz.idle_cpus_ಮುಖವಾಡ, this_cpu+1) {
		ಆದರೆ (!idle_cpu(balance_cpu))
			ಮುಂದುವರಿ;

		/*
		 * If this CPU gets work to do, stop the load balancing
		 * work being done for other CPUs. Next load
		 * balancing owner will pick it up.
		 */
		ಆದರೆ (!idle_cpu(this_cpu) && need_resched()) {
			ಆದರೆ (flags & NOHZ_STATS_KICK)
				has_blocked_ತುಂಬು = true;
			ಆದರೆ (flags & NOHZ_ಮುಂದಿನ_KICK)
				WRITE_ONCE(nohz.needs_update, 1);
			ಹೋಗು abort;
		}

		rq = cpu_rq(balance_cpu);

		ಆದರೆ (flags & NOHZ_STATS_KICK)
			has_blocked_ತುಂಬು |= update_nohz_stats(rq);

		/*
		 * If time for next balance is due,
		 * do the balance.
		 */
		ಆದರೆ (ಸಮಯ_after_eq(jiffies, rq->ಮುಂದಿನ_balance)) {
			ರಚನೆ rq_flags rf;

			rq_ಬೀಗ_irqsave(rq, &rf);
			update_rq_clock(rq);
			rq_unlock_irqrestore(rq, &rf);

			ಆದರೆ (flags & NOHZ_BALANCE_KICK)
				sched_balance_domains(rq, CPU_IDLE);
		}

		ಆದರೆ (ಸಮಯ_after(ಮುಂದಿನ_balance, rq->ಮುಂದಿನ_balance)) {
			ಮುಂದಿನ_balance = rq->ಮುಂದಿನ_balance;
			update_ಮುಂದಿನ_balance = 1;
		}
	}

	/*
	 * next_balance will be updated only when there is a need.
	 * When the CPU is attached to null domain for ex, it will not be
	 * updated.
	 */
	ಆದರೆ (likely(update_ಮುಂದಿನ_balance))
		nohz.ಮುಂದಿನ_balance = ಮುಂದಿನ_balance;

	ಆದರೆ (flags & NOHZ_STATS_KICK)
		WRITE_ONCE(nohz.ಮುಂದಿನ_blocked,
			   now + msecs_ಗಮ್ಯ_jiffies(ತುಂಬು_AVG_PERIOD));

abort:
	/* There is still blocked load, enable periodic update */
	ಆದರೆ (has_blocked_ತುಂಬು)
		WRITE_ONCE(nohz.has_blocked_ತುಂಬು, 1);
}

/*
 * In CONFIG_NO_HZ_COMMON case, the idle balance kickee will do the
 * rebalancing for all the CPUs for whom scheduler ticks are stopped.
 */
ಸ್ಥಿರ bool nohz_idle_balance(ರಚನೆ rq *this_rq, ಎಣಿಕೆ cpu_idle_type idle)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags = this_rq->nohz_idle_balance;

	ಆದರೆ (!flags)
		ಹಿಂತಿರುಗಿ false;

	this_rq->nohz_idle_balance = 0;

	ಆದರೆ (idle != CPU_IDLE)
		ಹಿಂತಿರುಗಿ false;

	_nohz_idle_balance(this_rq, flags);

	ಹಿಂತಿರುಗಿ true;
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
ಶೂನ್ಯ nohz_ಸತತ_idle_balance(ಪೂರ್ಣಾಂಕ cpu)
{
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags;

	flags = atomic_fetch_andnot(NOHZ_NEWILB_KICK, nohz_flags(cpu));

	/*
	 * Update the blocked load only if no SCHED_SOFTIRQ is about to happen
	 * (i.e. NOHZ_STATS_KICK set) and will do the same.
	 */
	ಆದರೆ ((flags == NOHZ_NEWILB_KICK) && !need_resched())
		_nohz_idle_balance(cpu_rq(cpu), NOHZ_STATS_KICK);
}

ಸ್ಥಿರ ಶೂನ್ಯ nohz_newidle_balance(ರಚನೆ rq *this_rq)
{
	ಪೂರ್ಣಾಂಕ this_cpu = this_rq->cpu;

	/* Will wake up very soon. No time for doing anything else*/
	ಆದರೆ (this_rq->avg_idle < sysctl_sched_migration_ವೆಚ್ಚ)
		ಹಿಂತಿರುಗಿ;

	/* Don't need to update blocked load of idle CPUs*/
	ಆದರೆ (!READ_ONCE(nohz.has_blocked_ತುಂಬು) ||
	    ಸಮಯ_before(jiffies, READ_ONCE(nohz.ಮುಂದಿನ_blocked)))
		ಹಿಂತಿರುಗಿ;

	/*
	 * Set the need to trigger ILB in order to update blocked load
	 * before entering idle state.
	 */
	atomic_or(NOHZ_NEWILB_KICK, nohz_flags(this_cpu));
}

#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_NO_HZ_COMMON: */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ nohz_balancer_kick(ರಚನೆ rq *rq) { }

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool nohz_idle_balance(ರಚನೆ rq *this_rq, ಎಣಿಕೆ cpu_idle_type idle)
{
	ಹಿಂತಿರುಗಿ false;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ nohz_newidle_balance(ರಚನೆ rq *this_rq) { }
#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_NO_HZ_COMMON */

/*
 * sched_balance_newidle is called by schedule() if this_cpu is about to become
 * idle. Attempts to pull tasks from other CPUs.
 *
 * Returns:
 *   < 0 - we released the lock and there are !fair tasks present
 *     0 - failed, no new tasks
 *   > 0 - success, new (fair) tasks present
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ sched_balance_newidle(ರಚನೆ rq *this_rq, ರಚನೆ rq_flags *rf)
	__must_hold(__rq_lockp(this_rq))
{
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಮುಂದಿನ_balance = jiffies + HZ;
	ಪೂರ್ಣಾಂಕ this_cpu = this_rq->cpu;
	ಪೂರ್ಣಾಂಕ continue_balancing = 1;
	u64 t0, t1, curr_ವೆಚ್ಚ = 0;
	ರಚನೆ sched_domain *sd;
	ಪೂರ್ಣಾಂಕ pulled_task = 0;

	update_misfit_ಸ್ಥಿತಿ(ಖಾಲಿ, this_rq);

	/*
	 * There is a task waiting to run. No need to search for one.
	 * Return 0; the task will be enqueued when switching to idle.
	 */
	ಆದರೆ (this_rq->ttwu_pending)
		ಹಿಂತಿರುಗಿ 0;

	/*
	 * We must set idle_stamp _before_ calling sched_balance_rq()
	 * for CPU_NEWLY_IDLE, such that we measure the this duration
	 * as idle time.
	 */
	this_rq->idle_stamp = rq_clock(this_rq);

	/* Do not pull tasks towards !preferred CPUs */
	ಆದರೆ (!cpu_preferred(this_cpu))
		ಹಿಂತಿರುಗಿ 0;

	/*
	 * This is OK, because current is on_cpu, which avoids it being picked
	 * for load-balance and preemption/IRQs are still disabled avoiding
	 * further scheduler activity on it and we're being very careful to
	 * re-start the picking loop.
	 */
	rq_unpin_ಬೀಗ(this_rq, rf);

	sd = rcu_dereference_sched_domain(this_rq->sd);
	ಆದರೆ (!sd)
		ಹೋಗು ಹೊರಹೋಗು;

	ಆದರೆ (!get_rd_overloaded(this_rq->rd) ||
	    this_rq->avg_idle < sd->max_newidle_lb_ವೆಚ್ಚ) {

		update_ಮುಂದಿನ_balance(sd, &ಮುಂದಿನ_balance);
		ಹೋಗು ಹೊರಹೋಗು;
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
		u64 domain_ವೆಚ್ಚ;

		update_ಮುಂದಿನ_balance(sd, &ಮುಂದಿನ_balance);

		ಆದರೆ (this_rq->avg_idle < curr_ವೆಚ್ಚ + sd->max_newidle_lb_ವೆಚ್ಚ)
			ನಿಲ್ಲಿಸು;

		ಆದರೆ (sd->flags & SD_BALANCE_NEWIDLE) {
			ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ತೂಕ = 1;

			ಆದರೆ (sched_feat(NI_RANDOM) && sd->newidle_ಅನುಪಾತ < 1024) {
				/*
				 * Throw a 1k sided dice; and only run
				 * newidle_balance according to the success
				 * rate.
				 */
				u32 d1k = sched_rng() % 1024;
				ತೂಕ = 1 + sd->newidle_ಅನುಪಾತ;
				ಆದರೆ (d1k > ತೂಕ) {
					update_newidle_stats(sd, 0);
					ಮುಂದುವರಿ;
				}
				ತೂಕ = (1024 + ತೂಕ/2) / ತೂಕ;
			}

			pulled_task = sched_balance_rq(this_cpu, this_rq,
						   sd, CPU_NEWLY_IDLE,
						   &continue_balancing);

			t1 = sched_clock_cpu(this_cpu);
			domain_ವೆಚ್ಚ = t1 - t0;
			curr_ವೆಚ್ಚ += domain_ವೆಚ್ಚ;
			t0 = t1;

			/*
			 * Track max cost of a domain to make sure to not delay the
			 * next wakeup on the CPU.
			 */
			update_newidle_ವೆಚ್ಚ(sd, domain_ವೆಚ್ಚ, ತೂಕ * !!pulled_task);
		}

		/*
		 * Stop searching for tasks to pull if there are
		 * now runnable tasks on this rq.
		 */
		ಆದರೆ (pulled_task || !continue_balancing)
			ನಿಲ್ಲಿಸು;
	}

	raw_spin_rq_ಬೀಗ(this_rq);

	ಆದರೆ (curr_ವೆಚ್ಚ > this_rq->max_idle_balance_ವೆಚ್ಚ)
		this_rq->max_idle_balance_ವೆಚ್ಚ = curr_ವೆಚ್ಚ;

	/*
	 * While browsing the domains, we released the rq lock, a task could
	 * have been enqueued in the meantime. Since we're not going idle,
	 * pretend we pulled a task.
	 */
	ಆದರೆ (this_rq->cfs.h_nr_queued && !pulled_task)
		pulled_task = 1;

	/* If a higher prio class was modified, restart the pick */
	ಆದರೆ (rq_modified_above(this_rq, &fair_sched_class))
		pulled_task = -1;

ಹೊರಹೋಗು:
	/* Move the next balance forward */
	ಆದರೆ (ಸಮಯ_after(this_rq->ಮುಂದಿನ_balance, ಮುಂದಿನ_balance))
		this_rq->ಮುಂದಿನ_balance = ಮುಂದಿನ_balance;

	ಆದರೆ (pulled_task)
		this_rq->idle_stamp = 0;
	ಇಲ್ಲದಿದ್ದರೆ
		nohz_newidle_balance(this_rq);

	rq_repin_ಬೀಗ(this_rq, rf);

	ಹಿಂತಿರುಗಿ pulled_task;
}

/*
 * This softirq handler is triggered via SCHED_SOFTIRQ from two places:
 *
 * - directly from the local sched_tick() for periodic load balancing
 *
 * - indirectly from a remote sched_tick() for NOHZ idle balancing
 *   through the SMP cross-call nohz_csd_func()
 */
ಸ್ಥಿರ __latent_entropy ಶೂನ್ಯ sched_balance_softirq(ಶೂನ್ಯ)
{
	ರಚನೆ rq *this_rq = this_rq();
	ಎಣಿಕೆ cpu_idle_type idle = this_rq->idle_balance;
	/*
	 * If this CPU has a pending NOHZ_BALANCE_KICK, then do the
	 * balancing on behalf of the other idle CPUs whose ticks are
	 * stopped. Do nohz_idle_balance *before* sched_balance_domains to
	 * give the idle CPUs a chance to load balance. Else we may
	 * load balance only within the local sched_domain hierarchy
	 * and abort nohz_idle_balance altogether if we pull some load.
	 */
	ಆದರೆ (nohz_idle_balance(this_rq, idle))
		ಹಿಂತಿರುಗಿ;

	/* normal load balance */
	sched_balance_update_blocked_averages(this_rq->cpu);
	sched_balance_domains(this_rq, idle);
}

/*
 * Trigger the SCHED_SOFTIRQ if it is time to do periodic load balancing.
 */
ಶೂನ್ಯ sched_balance_trigger(ರಚನೆ rq *rq)
{
	/*
	 * Don't need to rebalance while attached to NULL domain or
	 * runqueue CPU is not active
	 */
	ಆದರೆ (unlikely(on_null_domain(rq) || !cpu_ಸಕ್ರಿಯ(cpu_ದ(rq))))
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (ಸಮಯ_after_eq(jiffies, rq->ಮುಂದಿನ_balance))
		raise_softirq(SCHED_SOFTIRQ);

	nohz_balancer_kick(rq);
}

ಸ್ಥಿರ ಶೂನ್ಯ rq_online_fair(ರಚನೆ rq *rq)
{
	update_sysctl();

	update_runtime_enabled(rq);
}

ಸ್ಥಿರ ಶೂನ್ಯ rq_offline_fair(ರಚನೆ rq *rq)
{
	update_sysctl();

	/* Ensure any throttled groups are reachable by pick_next_task */
	unthrottle_offline_cfs_rqs(rq);

	/* Ensure that we remove rq contribution to group share: */
	clear_tg_offline_cfs_rqs(rq);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CORE
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool
__entity_slice_used(ರಚನೆ sched_entity *se, ಪೂರ್ಣಾಂಕ min_nr_tasks)
{
	u64 rtime = se->ಮೊತ್ತ_exec_runtime - se->prev_ಮೊತ್ತ_exec_runtime;
	u64 ತುಂಡು = se->ತುಂಡು;

	ಹಿಂತಿರುಗಿ (rtime * min_nr_tasks > ತುಂಡು);
}

#ವ್ಯಾಖ್ಯಾನಿಸು MIN_NR_TASKS_DURING_FORCEIDLE	2
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ task_tick_core(ರಚನೆ rq *rq, ರಚನೆ task_struct *curr)
{
	ಆದರೆ (!sched_core_enabled(rq))
		ಹಿಂತಿರುಗಿ;

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
	ಆದರೆ (rq->core->core_forceidle_ಎಣಿಕೆ && rq->cfs.h_nr_queued == 1 &&
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
ಸ್ಥಿರ ಶೂನ್ಯ se_fi_update(ಸ್ಥಿರಾಂಕ ರಚನೆ sched_entity *se, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ fi_seq,
			 bool forceidle)
{
	for_each_sched_entity(se) {
		ರಚನೆ cfs_rq *cfs_rq = cfs_rq_ದ(se);

		ಆದರೆ (forceidle) {
			ಆದರೆ (cfs_rq->forceidle_seq == fi_seq)
				ನಿಲ್ಲಿಸು;
			cfs_rq->forceidle_seq = fi_seq;
		}

		cfs_rq->zero_vruntime_fi = cfs_rq->zero_vruntime;
	}
}

ಶೂನ್ಯ task_vruntime_update(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, bool ಒಳ_fi)
{
	ರಚನೆ sched_entity *se = &p->se;

	ಆದರೆ (p->sched_class != &fair_sched_class)
		ಹಿಂತಿರುಗಿ;

	se_fi_update(se, rq->core->core_forceidle_seq, ಒಳ_fi);
}

bool cfs_prio_less(ಸ್ಥಿರಾಂಕ ರಚನೆ task_struct *a, ಸ್ಥಿರಾಂಕ ರಚನೆ task_struct *b,
			bool ಒಳ_fi)
{
	ರಚನೆ rq *rq = task_rq(a);
	ಸ್ಥಿರಾಂಕ ರಚನೆ sched_entity *sea = &a->se;
	ಸ್ಥಿರಾಂಕ ರಚನೆ sched_entity *seb = &b->se;
	ರಚನೆ cfs_rq *cfs_rqa;
	ರಚನೆ cfs_rq *cfs_rqb;
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

	ಹಿಂತಿರುಗಿ delta > 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ task_is_throttled_fair(ರಚನೆ task_struct *p, ಪೂರ್ಣಾಂಕ cpu)
{
	ರಚನೆ cfs_rq *cfs_rq;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_FAIR_ಗುಂಪು_SCHED
	cfs_rq = tg_cfs_rq(task_ಗುಂಪು(p), cpu);
#ಇಲ್ಲದಿದ್ದರೆ
	cfs_rq = &cpu_rq(cpu)->cfs;
#ಆದರೆ_ಅಂತ್ಯ
	ಹಿಂತಿರುಗಿ throttled_hierarchy(cfs_rq);
}
#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_SCHED_CORE: */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ task_tick_core(ರಚನೆ rq *rq, ರಚನೆ task_struct *curr) {}
#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_SCHED_CORE */

/*
 * scheduler tick hitting a task of our scheduling class.
 *
 * NOTE: This function can be called remotely by the tick offload that
 * goes along full dynticks. Therefore no local assumption can be made
 * and everything must be accessed through the @rq and @curr passed in
 * parameters.
 */
ಸ್ಥಿರ ಶೂನ್ಯ task_tick_fair(ರಚನೆ rq *rq, ರಚನೆ task_struct *curr, ಪೂರ್ಣಾಂಕ queued)
{
	ರಚನೆ sched_entity *se = &curr->se;

	ಆದರೆ (se->on_rq) {
		ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ತೂಕ = NICE_0_ತುಂಬು;
		ರಚನೆ cfs_rq *cfs_rq;

		for_each_sched_entity(se) {
			cfs_rq = cfs_rq_ದ(se);
			entity_tick(cfs_rq, se, queued);

			ತೂಕ = __calc_prop_weight(cfs_rq, se, ತೂಕ);
		}

		se = &curr->se;
		reweight_eevdf(cfs_rq, se, ತೂಕ, se->on_rq);
	}

	ಆದರೆ (queued)
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (static_branch_unlikely(&sched_numa_balancing))
		task_tick_numa(rq, curr);

	task_tick_cache(rq, curr);

	update_misfit_ಸ್ಥಿತಿ(curr, rq);
	check_update_overutilized_ಸ್ಥಿತಿ(task_rq(curr));

	task_tick_core(rq, curr);
}

/*
 * called on fork with the child task as argument from the parent's context
 *  - child not yet on the tasklist
 *  - preemption disabled
 */
ಸ್ಥಿರ ಶೂನ್ಯ task_fork_fair(ರಚನೆ task_struct *p)
{
	ಗಣ_task_max_allowed_ಸಾಮರ್ಥ್ಯ(p);
}

/*
 * Priority of the task has changed. Check to see if we preempt
 * the current task.
 */
ಸ್ಥಿರ ಶೂನ್ಯ
prio_ಬದಲಾಯಿತು_fair(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, u64 oldprio)
{
	ಆದರೆ (!task_on_rq_queued(p))
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (p->prio == oldprio)
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (rq->cfs.h_nr_queued == 1)
		ಹಿಂತಿರುಗಿ;

	/*
	 * Reschedule if we are currently running on this runqueue and
	 * our priority decreased, or if we are not currently running on
	 * this runqueue and our priority is higher than the current's
	 */
	ಆದರೆ (task_current_donor(rq, p)) {
		ಆದರೆ (p->prio > oldprio)
			resched_curr(rq);
	} ಇಲ್ಲದಿದ್ದರೆ {
		wakeup_preempt(rq, p, 0);
	}
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_FAIR_ಗುಂಪು_SCHED
/*
 * Propagate the changes of the sched_entity across the tg tree to make it
 * visible to the root
 */
ಸ್ಥಿರ ಶೂನ್ಯ propagate_entity_cfs_rq(ರಚನೆ sched_entity *se)
{
	ರಚನೆ cfs_rq *cfs_rq = cfs_rq_ದ(se);

	/*
	 * If a task gets attached to this cfs_rq and before being queued,
	 * it gets migrated to another CPU due to reasons like affinity
	 * change, make sure this cfs_rq stays on leaf cfs_rq list to have
	 * that removed load decayed or it can cause faireness problem.
	 */
	ಆದರೆ (!cfs_rq_pelt_clock_throttled(cfs_rq))
		list_ಕೂಡಿಸು_ಎಲೆ_cfs_rq(cfs_rq);

	/* Start to propagate at parent */
	se = se->ಪೋಷಕ;

	for_each_sched_entity(se) {
		cfs_rq = cfs_rq_ದ(se);

		update_ತುಂಬು_avg(cfs_rq, se, UPDATE_TG);

		ಆದರೆ (!cfs_rq_pelt_clock_throttled(cfs_rq))
			list_ಕೂಡಿಸು_ಎಲೆ_cfs_rq(cfs_rq);
	}

	assert_list_ಎಲೆ_cfs_rq(rq_ದ(cfs_rq));
}
#ಇಲ್ಲದಿದ್ದರೆ /* !CONFIG_FAIR_GROUP_SCHED: */
ಸ್ಥಿರ ಶೂನ್ಯ propagate_entity_cfs_rq(ರಚನೆ sched_entity *se) { }
#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_FAIR_GROUP_SCHED */

ಸ್ಥಿರ ಶೂನ್ಯ detach_entity_cfs_rq(ರಚನೆ sched_entity *se)
{
	ರಚನೆ cfs_rq *cfs_rq = cfs_rq_ದ(se);

	/*
	 * In case the task sched_avg hasn't been attached:
	 * - A forked task which hasn't been woken up by wake_up_new_task().
	 * - A task which has been woken up by try_to_wake_up() but is
	 *   waiting for actually being woken up by sched_ttwu_pending().
	 */
	ಆದರೆ (!se->avg.ಕೊನೆಯ_update_ಸಮಯ)
		ಹಿಂತಿರುಗಿ;

	/* Catch up with the cfs_rq and remove our load when we leave */
	update_ತುಂಬು_avg(cfs_rq, se, 0);
	detach_entity_ತುಂಬು_avg(cfs_rq, se);
	update_tg_ತುಂಬು_avg(cfs_rq);
	propagate_entity_cfs_rq(se);
}

ಸ್ಥಿರ ಶೂನ್ಯ attach_entity_cfs_rq(ರಚನೆ sched_entity *se)
{
	ರಚನೆ cfs_rq *cfs_rq = cfs_rq_ದ(se);

	/* Synchronize entity with its cfs_rq */
	update_ತುಂಬು_avg(cfs_rq, se, sched_feat(ATTACH_AGE_ತುಂಬು) ? 0 : SKIP_AGE_ತುಂಬು);
	attach_entity_ತುಂಬು_avg(cfs_rq, se);
	update_tg_ತುಂಬು_avg(cfs_rq);
	propagate_entity_cfs_rq(se);
}

ಸ್ಥಿರ ಶೂನ್ಯ detach_task_cfs_rq(ರಚನೆ task_struct *p)
{
	ರಚನೆ sched_entity *se = &p->se;

	detach_entity_cfs_rq(se);
}

ಸ್ಥಿರ ಶೂನ್ಯ attach_task_cfs_rq(ರಚನೆ task_struct *p)
{
	ರಚನೆ sched_entity *se = &p->se;

	attach_entity_cfs_rq(se);
}

ಸ್ಥಿರ ಶೂನ್ಯ switching_from_fair(ರಚನೆ rq *rq, ರಚನೆ task_struct *p)
{
	ಆದರೆ (p->se.sched_delayed)
		ಸರತಿಯಿಂದ_ತೆಗೆ_task(rq, p, ಸರತಿಯಿಂದ_ತೆಗೆ_SLEEP | ಸರತಿಯಿಂದ_ತೆಗೆ_DELAYED | ಸರತಿಯಿಂದ_ತೆಗೆ_NOCLOCK);
}

ಸ್ಥಿರ ಶೂನ್ಯ switched_from_fair(ರಚನೆ rq *rq, ರಚನೆ task_struct *p)
{
	detach_task_cfs_rq(p);
}

ಸ್ಥಿರ ಶೂನ್ಯ switched_ಗಮ್ಯ_fair(ರಚನೆ rq *rq, ರಚನೆ task_struct *p)
{
	WARN_ON_ONCE(p->se.sched_delayed);

	attach_task_cfs_rq(p);

	ಗಣ_task_max_allowed_ಸಾಮರ್ಥ್ಯ(p);

	ಆದರೆ (task_on_rq_queued(p)) {
		/*
		 * We were most likely switched from sched_rt, so
		 * kick off the schedule if running, otherwise just see
		 * if we can still preempt the current task.
		 */
		ಆದರೆ (task_current_donor(rq, p))
			resched_curr(rq);
		ಇಲ್ಲದಿದ್ದರೆ
			wakeup_preempt(rq, p, 0);
	}
}

ಸ್ಥಿರ ಶೂನ್ಯ ಗಣ_ಮುಂದಿನ_task_fair(ರಚನೆ rq *rq, ರಚನೆ task_struct *p, ಎಣಿಕೆ snt_e type)
{
	ರಚನೆ sched_entity *se = &p->se;
	ರಚನೆ cfs_rq *cfs_rq = &rq->cfs;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ತೂಕ = NICE_0_ತುಂಬು;
	bool ಮೊದಲ = type == SNT_PICK;
	bool throttled = false;
	bool on_rq = se->on_rq;

	ಆದರೆ (type == SNT_REPICK)
		ಹೋಗು repick;

	clear_buddies(cfs_rq, se);

	ಆದರೆ (on_rq)
		__dequeue_entity(cfs_rq, se);

	for_each_sched_entity(se) {
		cfs_rq = cfs_rq_ದ(se);

		ಆದರೆ (!IS_ENABLED(CONFIG_FAIR_ಗುಂಪು_SCHED) ||
		    !ಮೊದಲ || !cfs_rq->h_curr)
			ಗಣ_ಮುಂದಿನ_entity(cfs_rq, se);

		/* ensure bandwidth has been allocated on our new cfs_rq */
		throttled |= account_cfs_rq_runtime(cfs_rq, 0);

		ಆದರೆ (on_rq)
			ತೂಕ = __calc_prop_weight(cfs_rq, se, ತೂಕ);
	}

	ಆದರೆ (throttled)
		task_throttle_ಸಿದ್ಧತೆ_work(p);

	se = &p->se;
	cfs_rq->curr = se;

	ಆದರೆ (on_rq) {
		reweight_eevdf(cfs_rq, se, ತೂಕ, se->on_rq);
		ಆದರೆ (ಮೊದಲ)
			ಗಣ_protect_ತುಂಡು(cfs_rq, se);
	}

	ಆದರೆ (task_on_rq_queued(p)) {
		/*
		 * Move the next running task to the front of the list, so our
		 * cfs_tasks list becomes MRU one.
		 */
		list_move(&se->ಗುಂಪು_ಗಂಟು, &rq->cfs_tasks);
	}
	ಆದರೆ (!ಮೊದಲ)
		ಹಿಂತಿರುಗಿ;

	WARN_ON_ONCE(se->sched_delayed);

	update_misfit_ಸ್ಥಿತಿ(p, rq);
	sched_fair_update_stop_tick(rq, p);

repick:
	/*
	 * A same-task repick skips put_prev_task_fair(), but
	 * pick_task_fair() refreshed the entity hrtick_start_fair() reads
	 * before selecting it again. rq->cfs.curr identifies that entity,
	 * including with group scheduling.
	 */
	ಆದರೆ (hrtick_enabled_fair(rq))
		hrtick_ಆರಂಭ_fair(rq, p);
}

ಶೂನ್ಯ init_cfs_rq(ರಚನೆ cfs_rq *cfs_rq)
{
	cfs_rq->tasks_timeline = RB_ಬೇರು_CACHED;
	cfs_rq->zero_vruntime = (u64)(-(1LL << 20));
	raw_spin_ಬೀಗ_init(&cfs_rq->removed.ಬೀಗ);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_FAIR_ಗುಂಪು_SCHED
ಸ್ಥಿರ ಶೂನ್ಯ task_change_ಗುಂಪು_fair(ರಚನೆ task_struct *p)
{
	/*
	 * We couldn't detach or attach a forked task which
	 * hasn't been woken up by wake_up_new_task().
	 */
	ಆದರೆ (READ_ONCE(p->__state) == TASK_NEW)
		ಹಿಂತಿರುಗಿ;

	detach_task_cfs_rq(p);

	/* Tell se's cfs_rq has been changed -- migrated */
	p->se.avg.ಕೊನೆಯ_update_ಸಮಯ = 0;
	ಗಣ_task_rq(p, task_cpu(p));
	attach_task_cfs_rq(p);
}

ಶೂನ್ಯ free_fair_sched_ಗುಂಪು(ರಚನೆ task_ಗುಂಪು *tg)
{
	free_percpu(tg->cfs_rq);
}

ಪೂರ್ಣಾಂಕ alloc_fair_sched_ಗುಂಪು(ರಚನೆ task_ಗುಂಪು *tg, ರಚನೆ task_ಗುಂಪು *ಪೋಷಕ)
{
	ರಚನೆ cfs_tg_state __percpu *state;
	ರಚನೆ sched_entity *se;
	ರಚನೆ cfs_rq *cfs_rq;
	ಪೂರ್ಣಾಂಕ i;

	state = alloc_percpu_gfp(ರಚನೆ cfs_tg_state, GFP_KERNEL);
	ಆದರೆ (!state)
		ಹೋಗು err;

	tg->cfs_rq = &state->cfs_rq;
	tg->shares = NICE_0_ತುಂಬು;

	init_cfs_bandwidth(tg_cfs_bandwidth(tg), tg_cfs_bandwidth(ಪೋಷಕ));

	for_each_possible_cpu(i) {
		cfs_rq = tg_cfs_rq(tg, i);
		ಆದರೆ (!cfs_rq)
			ಹೋಗು err;

		se = tg_se(tg, i);
		init_cfs_rq(cfs_rq);
		init_tg_cfs_ನಮೂದು(tg, cfs_rq, se, i, tg_se(ಪೋಷಕ, i));
		init_entity_runnable_ಸರಾಸರಿ_ಮೌಲ್ಯ(se);
	}

	ಹಿಂತಿರುಗಿ 1;

err:
	ಹಿಂತಿರುಗಿ 0;
}

ಶೂನ್ಯ online_fair_sched_ಗುಂಪು(ರಚನೆ task_ಗುಂಪು *tg)
{
	ರಚನೆ sched_entity *se;
	ರಚನೆ rq_flags rf;
	ರಚನೆ rq *rq;
	ಪೂರ್ಣಾಂಕ i;

	for_each_possible_cpu(i) {
		rq = cpu_rq(i);
		se = tg_se(tg, i);
		rq_ಬೀಗ_irq(rq, &rf);
		update_rq_clock(rq);
		attach_entity_cfs_rq(se);
		sync_throttle(tg, i);
		rq_unlock_irq(rq, &rf);
	}
}

ಶೂನ್ಯ unregister_fair_sched_ಗುಂಪು(ರಚನೆ task_ಗುಂಪು *tg)
{
	ಪೂರ್ಣಾಂಕ cpu;

	destroy_cfs_bandwidth(tg_cfs_bandwidth(tg));

	for_each_possible_cpu(cpu) {
		ರಚನೆ cfs_rq *cfs_rq = tg_cfs_rq(tg, cpu);
		ರಚನೆ sched_entity *se = tg_se(tg, cpu);
		ರಚನೆ rq *rq = cpu_rq(cpu);

		ಆದರೆ (se)
			remove_entity_ತುಂಬು_avg(se);

		/*
		 * Only empty task groups can be destroyed; so we can speculatively
		 * check on_list without danger of it being re-added.
		 */
		ಆದರೆ (cfs_rq->on_list) {
			guard(rq_ಬೀಗ_irqsave)(rq);
			list_del_ಎಲೆ_cfs_rq(cfs_rq);
		}
	}
}

ಶೂನ್ಯ init_tg_cfs_ನಮೂದು(ರಚನೆ task_ಗುಂಪು *tg, ರಚನೆ cfs_rq *cfs_rq,
			ರಚನೆ sched_entity *se, ಪೂರ್ಣಾಂಕ cpu,
			ರಚನೆ sched_entity *ಪೋಷಕ)
{
	ರಚನೆ rq *rq = cpu_rq(cpu);

	cfs_rq->tg = tg;
	cfs_rq->rq = rq;
	init_cfs_rq_runtime(cfs_rq);

	/* se could be NULL for root_task_group */
	ಆದರೆ (!se)
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (!ಪೋಷಕ) {
		se->cfs_rq = &rq->cfs;
		se->ಆಳ = 0;
	} ಇಲ್ಲದಿದ್ದರೆ {
		se->cfs_rq = ಪೋಷಕ->my_q;
		se->ಆಳ = ಪೋಷಕ->ಆಳ + 1;
	}

	se->my_q = cfs_rq;
	/* guarantee group entities always have weight */
	update_ತುಂಬು_ಗಣ(&se->ತುಂಬು, NICE_0_ತುಂಬು);
	se->ಪೋಷಕ = ಪೋಷಕ;
}

ಸ್ಥಿರ DEFINE_MUTEX(shares_mutex);

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ __sched_group_set_shares(ರಚನೆ task_ಗುಂಪು *tg, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ shares)
{
	ಪೂರ್ಣಾಂಕ i;

	lockdep_assert_held(&shares_mutex);

	/*
	 * We can't change the weight of the root cgroup.
	 */
	ಆದರೆ (is_ಬೇರು_task_ಗುಂಪು(tg))
		ಹಿಂತಿರುಗಿ -EINVAL;

	shares = clamp(shares, scale_ತುಂಬು(MIN_SHARES), scale_ತುಂಬು(MAX_SHARES));

	ಆದರೆ (tg->shares == shares)
		ಹಿಂತಿರುಗಿ 0;

	tg->shares = shares;
	for_each_possible_cpu(i) {
		ರಚನೆ rq *rq = cpu_rq(i);
		ರಚನೆ sched_entity *se = tg_se(tg, i);
		ರಚನೆ rq_flags rf;

		/* Propagate contribution to hierarchy */
		rq_ಬೀಗ_irqsave(rq, &rf);
		update_rq_clock(rq);
		for_each_sched_entity(se) {
			update_ತುಂಬು_avg(cfs_rq_ದ(se), se, UPDATE_TG);
			update_cfs_ಗುಂಪು(se);
		}
		rq_unlock_irqrestore(rq, &rf);
	}

	ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ sched_ಗುಂಪು_ಗಣ_shares(ರಚನೆ task_ಗುಂಪು *tg, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ shares)
{
	ಪೂರ್ಣಾಂಕ ret;

	mutex_ಬೀಗ(&shares_mutex);
	ಆದರೆ (tg_is_idle(tg))
		ret = -EINVAL;
	ಇಲ್ಲದಿದ್ದರೆ
		ret = __sched_group_set_shares(tg, shares);
	mutex_unlock(&shares_mutex);

	ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ sched_ಗುಂಪು_ಗಣ_idle(ರಚನೆ task_ಗುಂಪು *tg, ದೀರ್ಘ idle)
{
	ಪೂರ್ಣಾಂಕ i;

	ಆದರೆ (tg == &ಬೇರು_task_ಗುಂಪು)
		ಹಿಂತಿರುಗಿ -EINVAL;

	ಆದರೆ (idle < 0 || idle > 1)
		ಹಿಂತಿರುಗಿ -EINVAL;

	mutex_ಬೀಗ(&shares_mutex);

	ಆದರೆ (tg->idle == idle) {
		mutex_unlock(&shares_mutex);
		ಹಿಂತಿರುಗಿ 0;
	}

	tg->idle = idle;

	for_each_possible_cpu(i) {
		ರಚನೆ rq *rq = cpu_rq(i);
		ರಚನೆ sched_entity *se = tg_se(tg, i);
		ರಚನೆ cfs_rq *grp_cfs_rq = tg_cfs_rq(tg, i);
		bool was_idle = cfs_rq_is_idle(grp_cfs_rq);
		ದೀರ್ಘ idle_task_delta;
		ರಚನೆ rq_flags rf;

		rq_ಬೀಗ_irqsave(rq, &rf);

		grp_cfs_rq->idle = idle;
		ಆದರೆ (WARN_ON_ONCE(was_idle == cfs_rq_is_idle(grp_cfs_rq)))
			ಹೋಗು ಮುಂದಿನ_cpu;

		idle_task_delta = grp_cfs_rq->h_nr_queued -
				  grp_cfs_rq->h_nr_idle;
		ಆದರೆ (!cfs_rq_is_idle(grp_cfs_rq))
			idle_task_delta *= -1;

		for_each_sched_entity(se) {
			ರಚನೆ cfs_rq *cfs_rq = cfs_rq_ದ(se);

			ಆದರೆ (!se->on_rq)
				ನಿಲ್ಲಿಸು;

			cfs_rq->h_nr_idle += idle_task_delta;

			/* Already accounted at parent level and above. */
			ಆದರೆ (cfs_rq_is_idle(cfs_rq))
				ನಿಲ್ಲಿಸು;
		}

ಮುಂದಿನ_cpu:
		rq_unlock_irqrestore(rq, &rf);
	}

	/* Idle groups have minimum weight. */
	ಆದರೆ (tg_is_idle(tg))
		__sched_group_set_shares(tg, scale_ತುಂಬು(ತೂಕ_IDLEPRIO));
	ಇಲ್ಲದಿದ್ದರೆ
		__sched_group_set_shares(tg, NICE_0_ತುಂಬು);

	mutex_unlock(&shares_mutex);
	ಹಿಂತಿರುಗಿ 0;
}

#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FAIR_GROUP_SCHED */


ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ get_rr_interval_fair(ರಚನೆ rq *rq, ರಚನೆ task_struct *task)
{
	ರಚನೆ sched_entity *se = &task->se;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ rr_interval = 0;

	/*
	 * Time slice is 0 for SCHED_OTHER tasks that are on an otherwise
	 * idle runqueue:
	 */
	ಆದರೆ (rq->cfs.ತುಂಬು.ತೂಕ)
		rr_interval = NS_ಗಮ್ಯ_JIFFIES(se->ತುಂಡು);

	ಹಿಂತಿರುಗಿ rr_interval;
}

/*
 * All the scheduling class methods:
 */
DEFINE_SCHED_CLASS(fair) = {
	.ಸರತಿಗೆ_ಸೇರಿಸು_task		= ಸರತಿಗೆ_ಸೇರಿಸು_task_fair,
	.ಸರತಿಯಿಂದ_ತೆಗೆ_task		= ಸರತಿಯಿಂದ_ತೆಗೆ_task_fair,
	.yield_task		= yield_task_fair,
	.yield_ಗಮ್ಯ_task		= yield_ಗಮ್ಯ_task_fair,

	.wakeup_preempt		= wakeup_preempt_fair,

	.pick_task		= pick_task_fair,
	.put_prev_task		= put_prev_task_fair,
	.ಗಣ_ಮುಂದಿನ_task          = ಗಣ_ಮುಂದಿನ_task_fair,

	.select_task_rq		= select_task_rq_fair,
	.migrate_task_rq	= migrate_task_rq_fair,

	.rq_online		= rq_online_fair,
	.rq_offline		= rq_offline_fair,

	.task_dead		= task_dead_fair,
	.ಗಣ_cpus_allowed	= ಗಣ_cpus_allowed_fair,

	.task_tick		= task_tick_fair,
	.task_fork		= task_fork_fair,

	.reweight_task		= reweight_task_fair,
	.prio_ಬದಲಾಯಿತು		= prio_ಬದಲಾಯಿತು_fair,
	.switching_from		= switching_from_fair,
	.switched_from		= switched_from_fair,
	.switched_ಗಮ್ಯ		= switched_ಗಮ್ಯ_fair,

	.get_rr_interval	= get_rr_interval_fair,

	.update_curr		= update_curr_fair,

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_FAIR_ಗುಂಪು_SCHED
	.task_change_ಗುಂಪು	= task_change_ಗುಂಪು_fair,
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_SCHED_CORE
	.task_is_throttled	= task_is_throttled_fair,
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_UCLAMP_TASK
	.uclamp_enabled		= 1,
#ಆದರೆ_ಅಂತ್ಯ
};

ಶೂನ್ಯ print_cfs_stats(ರಚನೆ seq_file *m, ಪೂರ್ಣಾಂಕ cpu)
{
	ರಚನೆ cfs_rq *cfs_rq, *pos;

	rcu_read_ಬೀಗ();
	for_each_ಎಲೆ_cfs_rq_ಸುರಕ್ಷಿತ(cpu_rq(cpu), cfs_rq, pos)
		print_cfs_rq(m, cpu, cfs_rq);
	rcu_read_unlock();
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_NUMA_BALANCING
ಶೂನ್ಯ show_numa_stats(ರಚನೆ task_struct *p, ರಚನೆ seq_file *m)
{
	ಪೂರ್ಣಾಂಕ ಗಂಟು;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ tsf = 0, tpf = 0, gsf = 0, gpf = 0;
	ರಚನೆ numa_ಗುಂಪು *ng;

	rcu_read_ಬೀಗ();
	ng = rcu_dereference_all(p->numa_ಗುಂಪು);
	for_each_online_ಗಂಟು(ಗಂಟು) {
		ಆದರೆ (p->numa_ದೋಷಗಳು) {
			tsf = p->numa_ದೋಷಗಳು[task_ದೋಷಗಳು_idx(NUMA_MEM, ಗಂಟು, 0)];
			tpf = p->numa_ದೋಷಗಳು[task_ದೋಷಗಳು_idx(NUMA_MEM, ಗಂಟು, 1)];
		}
		ಆದರೆ (ng) {
			gsf = ng->ದೋಷಗಳು[task_ದೋಷಗಳು_idx(NUMA_MEM, ಗಂಟು, 0)];
			gpf = ng->ದೋಷಗಳು[task_ದೋಷಗಳು_idx(NUMA_MEM, ಗಂಟು, 1)];
		}
		print_numa_stats(m, ಗಂಟು, tsf, tpf, gsf, gpf);
	}
	rcu_read_unlock();
}
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_NUMA_BALANCING */

__init ಶೂನ್ಯ init_sched_fair_class(ಶೂನ್ಯ)
{
	ಪೂರ್ಣಾಂಕ i;

	for_each_possible_cpu(i) {
		zalloc_cpumask_var_ಗಂಟು(&ಪ್ರತಿ_cpu(ತುಂಬು_balance_ಮುಖವಾಡ, i), GFP_KERNEL, cpu_ಗಮ್ಯ_ಗಂಟು(i));
		zalloc_cpumask_var_ಗಂಟು(&ಪ್ರತಿ_cpu(select_rq_ಮುಖವಾಡ,    i), GFP_KERNEL, cpu_ಗಮ್ಯ_ಗಂಟು(i));
		zalloc_cpumask_var_ಗಂಟು(&ಪ್ರತಿ_cpu(should_we_balance_tmpmask, i),
					GFP_KERNEL, cpu_ಗಮ್ಯ_ಗಂಟು(i));

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_CFS_BANDWIDTH
		INIT_CSD(&cpu_rq(i)->cfsb_csd, __cfsb_csd_unthrottle, cpu_rq(i));
		INIT_LIST_ತಲೆ(&cpu_rq(i)->cfsb_csd_list);
#ಆದರೆ_ಅಂತ್ಯ
	}

	open_softirq(SCHED_SOFTIRQ, sched_balance_softirq);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_NO_HZ_COMMON
	nohz.ಮುಂದಿನ_balance = jiffies;
	nohz.ಮುಂದಿನ_blocked = jiffies;
	zalloc_cpumask_var(&nohz.idle_cpus_ಮುಖವಾಡ, GFP_NOWAIT);
#ಆದರೆ_ಅಂತ್ಯ
}
