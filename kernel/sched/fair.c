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
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/energy_model.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/mmap_lock.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/hugetlb_inline.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/jiffies.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/math.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/mm_api.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/highmem.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/hrtimer.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/hrtimer_bases.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/spinlock_api.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/cpumask_api.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/lockdep_api.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/softirq.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/refcount_api.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/topology.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/clock.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/cond_resched.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/cputime.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/isolation.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/nohz.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched/prio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/static_call.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <linux/cpuidle.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/interrupt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/memory-tiers.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/mempolicy.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/mutex_api.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/profile.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/psi.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/ratelimit.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/task_work.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/rbtree_augmented.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <asm/switch_to.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <uapi/linux/sched/types.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "sched.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "stats.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "autogroup.h"

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
ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ sysctl_sched_tunable_scaling = SCHED_TUNABLESCALING_LOG;

/*
 * Default base time slice (request size r_i) for SCHED_NORMAL/SCHED_BATCH:
 *
 * Under EEVDF this is the request size used to compute the virtual
 * deadline; see update_deadline().
 *
 * (default: 0.70 msec * (1 + ilog(ncpus)), units: nanoseconds)
 */
ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ sysctl_sched_ਅਧਾਰ_ਟੁਕੜਾ			= 700000ULL;
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ normalized_sysctl_sched_ਅਧਾਰ_ਟੁਕੜਾ	= 700000ULL;

__read_mostly ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ sysctl_sched_migration_ਲਾਗਤ	= 500000UL;

ਸਥਿਰ ਪੂਰਨ __init ਤਿਆਰੀ_sched_thermal_decay_ਖਿਸਕਾਓ(ਅੱਖਰ *str)
{
	pr_warn("Ignoring the deprecated sched_thermal_decay_shift= option\n");
	ਵਾਪਸ 1;
}
__setup("sched_thermal_decay_shift=", ਤਿਆਰੀ_sched_thermal_decay_ਖਿਸਕਾਓ);

/*
 * For asym packing, by default the lower numbered CPU has higher priority.
 */
ਪੂਰਨ __weak arch_asym_cpu_ਤਰਜੀਹ(ਪੂਰਨ cpu)
{
	ਵਾਪਸ -cpu;
}

/*
 * The margin used when comparing utilization with CPU capacity.
 *
 * (default: ~20%)
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਸਮਾਉਂਦਾ_ਸਮਰੱਥਾ(cap, max)	((cap) * 1280 < (max) * 1024)

/*
 * The margin used when comparing CPU capacities.
 * is 'cap1' noticeably greater than 'cap2'
 *
 * (default: ~5%)
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਸਮਰੱਥਾ_greater(cap1, cap2) ((cap1) * 1024 > (cap2) * 1078)

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_CFS_BANDWIDTH
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
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ sysctl_sched_cfs_bandwidth_ਟੁਕੜਾ		= 5000UL;
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_NUMA_BALANCING
/* Restrict the NUMA promotion throughput (MB/s) for each target node. */
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ sysctl_numa_balancing_promote_ਦਰ_ਹੱਦਬੰਦੀ = 65536;
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SYSCTL
ਸਥਿਰ ਅਚਲ ਬਣਤਰ ctl_ਸਾਰਣੀ sched_fair_sysctls[] = {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_CFS_BANDWIDTH
	{
		.procname       = "sched_cfs_bandwidth_slice_us",
		.ਅੰਕੜੇ           = &sysctl_sched_cfs_bandwidth_ਟੁਕੜਾ,
		.maxlen         = ਆਕਾਰ(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ),
		.ਬਹੁਲਕ           = 0644,
		.proc_handler   = proc_dointvec_minmax,
		.extra1         = SYSCTL_ONE,
	},
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_NUMA_BALANCING
	{
		.procname	= "numa_balancing_promote_rate_limit_MBps",
		.ਅੰਕੜੇ		= &sysctl_numa_balancing_promote_ਦਰ_ਹੱਦਬੰਦੀ,
		.maxlen		= ਆਕਾਰ(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ),
		.ਬਹੁਲਕ		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= SYSCTL_ZERO,
	},
#ਜੇ_ਅੰਤ /* CONFIG_NUMA_BALANCING */
};

ਸਥਿਰ ਪੂਰਨ __init sched_fair_sysctl_init(ਖਾਲੀ)
{
	ਰਜਿਸਟਰ_sysctl_init("kernel", sched_fair_sysctls);
	ਵਾਪਸ 0;
}
late_initcall(sched_fair_sysctl_init);
#ਜੇ_ਅੰਤ /* CONFIG_SYSCTL */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ update_ਲੱਦੋ_ਜੋੜੋ(ਬਣਤਰ ਲੱਦੋ_ਭਾਰ *lw, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ inc)
{
	lw->ਭਾਰ += inc;
	lw->inv_ਭਾਰ = 0;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ update_ਲੱਦੋ_sub(ਬਣਤਰ ਲੱਦੋ_ਭਾਰ *lw, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ dec)
{
	lw->ਭਾਰ -= dec;
	lw->inv_ਭਾਰ = 0;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ update_ਲੱਦੋ_ਸੈੱਟ(ਬਣਤਰ ਲੱਦੋ_ਭਾਰ *lw, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ w)
{
	lw->ਭਾਰ = w;
	lw->inv_ਭਾਰ = 0;
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
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_update_sysctl_ਗੁਣਕ(ਖਾਲੀ)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cpus = min_t(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ, ਅੰਕ_online_cpus(), 8);
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗੁਣਕ;

	ਚੋਣ (sysctl_sched_tunable_scaling) {
	ਹਾਲਤ SCHED_TUNABLESCALING_NONE:
		ਗੁਣਕ = 1;
		ਰੋਕੋ;
	ਹਾਲਤ SCHED_TUNABLESCALING_ਰੇਖਿਕ:
		ਗੁਣਕ = cpus;
		ਰੋਕੋ;
	ਹਾਲਤ SCHED_TUNABLESCALING_LOG:
	ਮੂਲ:
		ਗੁਣਕ = 1 + ilog2(cpus);
		ਰੋਕੋ;
	}

	ਵਾਪਸ ਗੁਣਕ;
}

ਸਥਿਰ ਖਾਲੀ update_sysctl(ਖਾਲੀ)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗੁਣਕ = get_update_sysctl_ਗੁਣਕ();

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਸੈੱਟ_SYSCTL(name) \
	(sysctl_##name = (ਗੁਣਕ) * normalized_sysctl_##name)
	ਸੈੱਟ_SYSCTL(sched_ਅਧਾਰ_ਟੁਕੜਾ);
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ ਸੈੱਟ_SYSCTL
}

ਖਾਲੀ __init sched_init_granularity(ਖਾਲੀ)
{
	update_sysctl();
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CONFIG_64_ਦੋਆਧਾਰੀ_ਅੰਕ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ WMULT_CONST	(~0U)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ WMULT_ਖਿਸਕਾਓ	32

ਸਥਿਰ ਖਾਲੀ __update_inv_weight(ਬਣਤਰ ਲੱਦੋ_ਭਾਰ *lw)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ w;

	ਜੇ (likely(lw->inv_ਭਾਰ))
		ਵਾਪਸ;

	w = scale_ਲੱਦੋ_ਥੱਲੇ(lw->ਭਾਰ);

	ਜੇ (ਬਿੱਟ_ਪ੍ਰਤੀ_LONG > 32 && unlikely(w >= WMULT_CONST))
		lw->inv_ਭਾਰ = 1;
	ਨਹੀਂਤਾਂ ਜੇ (unlikely(!w))
		lw->inv_ਭਾਰ = WMULT_CONST;
	ਨਹੀਂਤਾਂ
		lw->inv_ਭਾਰ = WMULT_CONST / w;
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
ਸਥਿਰ u64 __calc_delta(u64 delta_exec, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਭਾਰ, ਬਣਤਰ ਲੱਦੋ_ਭਾਰ *lw)
{
	u64 fact = scale_ਲੱਦੋ_ਥੱਲੇ(ਭਾਰ);
	u32 fact_hi = (u32)(fact >> 32);
	ਪੂਰਨ ਖਿਸਕਾਓ = WMULT_ਖਿਸਕਾਓ;
	ਪੂਰਨ fs;

	__update_inv_weight(lw);

	ਜੇ (unlikely(fact_hi)) {
		fs = fls(fact_hi);
		ਖਿਸਕਾਓ -= fs;
		fact >>= fs;
	}

	fact = mul_u32_u32(fact, lw->inv_ਭਾਰ);

	fact_hi = (u32)(fact >> 32);
	ਜੇ (fact_hi) {
		fs = fls(fact_hi);
		ਖਿਸਕਾਓ -= fs;
		fact >>= fs;
	}

	ਵਾਪਸ mul_u64_u32_shr(delta_exec, fact, ਖਿਸਕਾਓ);
}
#ਨਹੀਂਤਾਂ
ਸਥਿਰ u64 __calc_delta(u64 delta_exec, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਭਾਰ, ਬਣਤਰ ਲੱਦੋ_ਭਾਰ *lw)
{
	ਵਾਪਸ (delta_exec * ਭਾਰ) / lw->ਭਾਰ;
}
#ਜੇ_ਅੰਤ

/*
 * delta /= w
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ u64 calc_delta_fair(u64 delta, ਬਣਤਰ sched_entity *se)
{
	ਜੇ (se->h_ਲੱਦੋ.ਭਾਰ != NICE_0_ਲੱਦੋ)
		delta = __calc_delta(delta, NICE_0_ਲੱਦੋ, &se->h_ਲੱਦੋ);

	ਵਾਪਸ delta;
}

ਅਚਲ ਬਣਤਰ sched_class fair_sched_class;

/**************************************************************
 * CFS operations on generic schedulable entities:
 */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_FAIR_ਸਮੂਹ_SCHED

/* Walk up scheduling entities hierarchy */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ for_each_sched_entity(se) \
		ਲਈ (; se; se = se->ਮਾਪਾ)

ਸਥਿਰ ਸਤਰਅੰਦਰ bool list_ਜੋੜੋ_ਪੱਤੀ_cfs_rq(ਬਣਤਰ cfs_rq *cfs_rq)
{
	ਬਣਤਰ rq *rq = rq_ਦਾ(cfs_rq);
	ਪੂਰਨ cpu = cpu_ਦਾ(rq);

	ਜੇ (cfs_rq->on_list)
		ਵਾਪਸ rq->tmp_alone_branch == &rq->ਪੱਤੀ_cfs_rq_list;

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
	ਜੇ (cfs_rq->tg->ਮਾਪਾ &&
	    tg_cfs_rq(cfs_rq->tg->ਮਾਪਾ, cpu)->on_list) {
		/*
		 * If parent is already on the list, we add the child
		 * just before. Thanks to circular linked property of
		 * the list, this means to put the child at the tail
		 * of the list that starts by parent.
		 */
		list_ਜੋੜੋ_ਪੂਛ_rcu(&cfs_rq->ਪੱਤੀ_cfs_rq_list,
			&(tg_cfs_rq(cfs_rq->tg->ਮਾਪਾ, cpu)->ਪੱਤੀ_cfs_rq_list));
		/*
		 * The branch is now connected to its tree so we can
		 * reset tmp_alone_branch to the beginning of the
		 * list.
		 */
		rq->tmp_alone_branch = &rq->ਪੱਤੀ_cfs_rq_list;
		ਵਾਪਸ true;
	}

	ਜੇ (!cfs_rq->tg->ਮਾਪਾ) {
		/*
		 * cfs rq without parent should be put
		 * at the tail of the list.
		 */
		list_ਜੋੜੋ_ਪੂਛ_rcu(&cfs_rq->ਪੱਤੀ_cfs_rq_list,
			&rq->ਪੱਤੀ_cfs_rq_list);
		/*
		 * We have reach the top of a tree so we can reset
		 * tmp_alone_branch to the beginning of the list.
		 */
		rq->tmp_alone_branch = &rq->ਪੱਤੀ_cfs_rq_list;
		ਵਾਪਸ true;
	}

	/*
	 * The parent has not already been added so we want to
	 * make sure that it will be put after us.
	 * tmp_alone_branch points to the begin of the branch
	 * where we will add parent.
	 */
	list_ਜੋੜੋ_rcu(&cfs_rq->ਪੱਤੀ_cfs_rq_list, rq->tmp_alone_branch);
	/*
	 * update tmp_alone_branch to points to the new begin
	 * of the branch
	 */
	rq->tmp_alone_branch = &cfs_rq->ਪੱਤੀ_cfs_rq_list;
	ਵਾਪਸ false;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ list_del_ਪੱਤੀ_cfs_rq(ਬਣਤਰ cfs_rq *cfs_rq)
{
	ਜੇ (cfs_rq->on_list) {
		ਬਣਤਰ rq *rq = rq_ਦਾ(cfs_rq);

		/*
		 * With cfs_rq being unthrottled/throttled during an enqueue,
		 * it can happen the tmp_alone_branch points to the leaf that
		 * we finally want to delete. In this case, tmp_alone_branch moves
		 * to the prev element but it will point to rq->leaf_cfs_rq_list
		 * at the end of the enqueue.
		 */
		ਜੇ (rq->tmp_alone_branch == &cfs_rq->ਪੱਤੀ_cfs_rq_list)
			rq->tmp_alone_branch = cfs_rq->ਪੱਤੀ_cfs_rq_list.prev;

		list_del_rcu(&cfs_rq->ਪੱਤੀ_cfs_rq_list);
		cfs_rq->on_list = 0;
	}
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ assert_list_ਪੱਤੀ_cfs_rq(ਬਣਤਰ rq *rq)
{
	WARN_ON_ONCE(rq->tmp_alone_branch != &rq->ਪੱਤੀ_cfs_rq_list);
}

/* Iterate through all leaf cfs_rq's on a runqueue */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ for_each_ਪੱਤੀ_cfs_rq_ਸੁਰੱਖਿਅਤ(rq, cfs_rq, pos)			\
	list_for_each_ਇੰਦਰਾਜ_ਸੁਰੱਖਿਅਤ(cfs_rq, pos, &rq->ਪੱਤੀ_cfs_rq_list,	\
				 ਪੱਤੀ_cfs_rq_list)

/* Do the two (enqueued) entities belong to the same group ? */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ cfs_rq *
is_same_ਸਮੂਹ(ਬਣਤਰ sched_entity *se, ਬਣਤਰ sched_entity *pse)
{
	ਜੇ (se->cfs_rq == pse->cfs_rq)
		ਵਾਪਸ se->cfs_rq;

	ਵਾਪਸ ਨਿਰਾ;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ sched_entity *ਮਾਪਾ_entity(ਅਚਲ ਬਣਤਰ sched_entity *se)
{
	ਵਾਪਸ se->ਮਾਪਾ;
}

ਸਥਿਰ ਪੂਰਨ tg_is_idle(ਬਣਤਰ task_ਸਮੂਹ *tg)
{
	ਵਾਪਸ tg->idle > 0;
}

ਸਥਿਰ ਪੂਰਨ cfs_rq_is_idle(ਬਣਤਰ cfs_rq *cfs_rq)
{
	ਵਾਪਸ cfs_rq->idle > 0;
}

ਸਥਿਰ ਪੂਰਨ se_is_idle(ਬਣਤਰ sched_entity *se)
{
	ਜੇ (entity_is_task(se))
		ਵਾਪਸ task_has_idle_policy(task_ਦਾ(se));
	ਵਾਪਸ cfs_rq_is_idle(ਸਮੂਹ_cfs_rq(se));
}

#ਨਹੀਂਤਾਂ /* !CONFIG_FAIR_GROUP_SCHED: */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ for_each_sched_entity(se) \
		ਲਈ (; se; se = ਨਿਰਾ)

ਸਥਿਰ ਸਤਰਅੰਦਰ bool list_ਜੋੜੋ_ਪੱਤੀ_cfs_rq(ਬਣਤਰ cfs_rq *cfs_rq)
{
	ਵਾਪਸ true;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ list_del_ਪੱਤੀ_cfs_rq(ਬਣਤਰ cfs_rq *cfs_rq)
{
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ assert_list_ਪੱਤੀ_cfs_rq(ਬਣਤਰ rq *rq)
{
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ for_each_ਪੱਤੀ_cfs_rq_ਸੁਰੱਖਿਅਤ(rq, cfs_rq, pos)	\
		ਲਈ (cfs_rq = &rq->cfs, pos = ਨਿਰਾ; cfs_rq; cfs_rq = pos)

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ sched_entity *ਮਾਪਾ_entity(ਬਣਤਰ sched_entity *se)
{
	ਵਾਪਸ ਨਿਰਾ;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ tg_is_idle(ਬਣਤਰ task_ਸਮੂਹ *tg)
{
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ cfs_rq_is_idle(ਬਣਤਰ cfs_rq *cfs_rq)
{
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ se_is_idle(ਬਣਤਰ sched_entity *se)
{
	ਵਾਪਸ task_has_idle_policy(task_ਦਾ(se));
}

#ਜੇ_ਅੰਤ /* !CONFIG_FAIR_GROUP_SCHED */

ਸਥਿਰ __always_inline
bool account_cfs_rq_runtime(ਬਣਤਰ cfs_rq *cfs_rq, u64 delta_exec);

/**************************************************************
 * Scheduling class tree data structure manipulation methods:
 */

ਬਾਹਰੀ ਖਾਲੀ __BUILD_BUG_vruntime_cmp(ਖਾਲੀ);

/* Use __builtin_strcmp() because of __HAVE_ARCH_STRCMP: */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ vruntime_cmp(A, CMP_STR, B) ({				\
	ਪੂਰਨ __res = 0;						\
								\
	ਜੇ (!__builtin_strcmp(CMP_STR, "<")) {			\
		__res = ((s64)((A)-(B)) < 0);			\
	} ਨਹੀਂਤਾਂ ਜੇ (!__builtin_strcmp(CMP_STR, "<=")) {		\
		__res = ((s64)((A)-(B)) <= 0);			\
	} ਨਹੀਂਤਾਂ ਜੇ (!__builtin_strcmp(CMP_STR, ">")) {		\
		__res = ((s64)((A)-(B)) > 0);			\
	} ਨਹੀਂਤਾਂ ਜੇ (!__builtin_strcmp(CMP_STR, ">=")) {		\
		__res = ((s64)((A)-(B)) >= 0);			\
	} ਨਹੀਂਤਾਂ {						\
		/* Unknown operator throws linker error: */	\
		__BUILD_BUG_vruntime_cmp();			\
	}							\
								\
	__res;							\
})

ਬਾਹਰੀ ਖਾਲੀ __BUILD_BUG_vruntime_op(ਖਾਲੀ);

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ vruntime_op(A, OP_STR, B) ({				\
	s64 __res = 0;						\
								\
	ਜੇ (!__builtin_strcmp(OP_STR, "-")) {			\
		__res = (s64)((A)-(B));				\
	} ਨਹੀਂਤਾਂ {						\
		/* Unknown operator throws linker error: */	\
		__BUILD_BUG_vruntime_op();			\
	}							\
								\
	__res;						\
})


ਸਥਿਰ ਸਤਰਅੰਦਰ __maybe_unused u64 max_vruntime(u64 max_vruntime, u64 vruntime)
{
	ਜੇ (vruntime_cmp(vruntime, ">", max_vruntime))
		max_vruntime = vruntime;

	ਵਾਪਸ max_vruntime;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ __maybe_unused u64 min_vruntime(u64 min_vruntime, u64 vruntime)
{
	ਜੇ (vruntime_cmp(vruntime, "<", min_vruntime))
		min_vruntime = vruntime;

	ਵਾਪਸ min_vruntime;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool entity_before(ਅਚਲ ਬਣਤਰ sched_entity *a,
				 ਅਚਲ ਬਣਤਰ sched_entity *b)
{
	/*
	 * Tiebreak on vruntime seems unnecessary since it can
	 * hardly happen.
	 */
	ਵਾਪਸ vruntime_cmp(a->deadline, "<", b->deadline);
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
ਸਥਿਰ ਸਤਰਅੰਦਰ s64 entity_ਕੁੰਜੀ(ਬਣਤਰ cfs_rq *cfs_rq, ਬਣਤਰ sched_entity *se)
{
	ਵਾਪਸ vruntime_op(se->vruntime, "-", cfs_rq->zero_vruntime);
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __node_2_se(ਗੰਢ) \
	rb_ਇੰਦਰਾਜ((ਗੰਢ), ਬਣਤਰ sched_entity, ਲੜੀਵਾਰ_ਗੰਢ)

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
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ avg_vruntime_ਭਾਰ(ਬਣਤਰ cfs_rq *cfs_rq, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ w)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_64_ਦੋਆਧਾਰੀ_ਅੰਕ
	ਜੇ (cfs_rq->ਜੋੜ_ਖਿਸਕਾਓ)
		w = max(2UL, w >> cfs_rq->ਜੋੜ_ਖਿਸਕਾਓ);
#ਜੇ_ਅੰਤ
	ਵਾਪਸ w;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
__sum_w_vruntime_add(ਬਣਤਰ cfs_rq *cfs_rq, ਬਣਤਰ sched_entity *se)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਭਾਰ = avg_vruntime_ਭਾਰ(cfs_rq, se->h_ਲੱਦੋ.ਭਾਰ);
	s64 w_vruntime, ਕੁੰਜੀ = entity_ਕੁੰਜੀ(cfs_rq, se);

	w_vruntime = ਕੁੰਜੀ * ਭਾਰ;
	WARN_ON_ONCE((w_vruntime >> 63) != (w_vruntime >> 62));

	cfs_rq->ਜੋੜ_w_vruntime += w_vruntime;
	cfs_rq->ਜੋੜ_ਭਾਰ += ਭਾਰ;
}

ਸਥਿਰ ਖਾਲੀ
ਜੋੜ_w_vruntime_ਜੋੜੋ_paranoid(ਬਣਤਰ cfs_rq *cfs_rq, ਬਣਤਰ sched_entity *se)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਭਾਰ;
	s64 ਕੁੰਜੀ, tmp;

again:
	ਭਾਰ = avg_vruntime_ਭਾਰ(cfs_rq, se->h_ਲੱਦੋ.ਭਾਰ);
	ਕੁੰਜੀ = entity_ਕੁੰਜੀ(cfs_rq, se);

	ਜੇ (check_mul_ਉਛਾਲ(ਕੁੰਜੀ, ਭਾਰ, &ਕੁੰਜੀ))
		ਜਾਓ ਉਛਾਲ;

	ਜੇ (check_ਜੋੜੋ_ਉਛਾਲ(cfs_rq->ਜੋੜ_w_vruntime, ਕੁੰਜੀ, &tmp))
		ਜਾਓ ਉਛਾਲ;

	cfs_rq->ਜੋੜ_w_vruntime = tmp;
	cfs_rq->ਜੋੜ_ਭਾਰ += ਭਾਰ;
	ਵਾਪਸ;

ਉਛਾਲ:
	/*
	 * There's gotta be a limit -- if we're still failing at this point
	 * there's really nothing much to be done about things.
	 */
	BUG_ON(cfs_rq->ਜੋੜ_ਖਿਸਕਾਓ >= 10);
	cfs_rq->ਜੋੜ_ਖਿਸਕਾਓ++;

	/*
	 * Note: \Sum (k_i * (w_i >> 1)) != (\Sum (k_i * w_i)) >> 1
	 */
	cfs_rq->ਜੋੜ_w_vruntime = 0;
	cfs_rq->ਜੋੜ_ਭਾਰ = 0;

	ਲਈ (ਬਣਤਰ rb_ਗੰਢ *ਗੰਢ = cfs_rq->tasks_timeline.rb_leftmost;
	     ਗੰਢ; ਗੰਢ = rb_ਅਗਲਾ(ਗੰਢ))
		__sum_w_vruntime_add(cfs_rq, __node_2_se(ਗੰਢ));

	ਜਾਓ again;
}

ਸਥਿਰ ਖਾਲੀ
ਜੋੜ_w_vruntime_ਜੋੜੋ(ਬਣਤਰ cfs_rq *cfs_rq, ਬਣਤਰ sched_entity *se)
{
	ਜੇ (sched_feat(PARANOID_AVG))
		ਵਾਪਸ ਜੋੜ_w_vruntime_ਜੋੜੋ_paranoid(cfs_rq, se);

	__sum_w_vruntime_add(cfs_rq, se);
}

ਸਥਿਰ ਖਾਲੀ
ਜੋੜ_w_vruntime_sub(ਬਣਤਰ cfs_rq *cfs_rq, ਬਣਤਰ sched_entity *se)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਭਾਰ = avg_vruntime_ਭਾਰ(cfs_rq, se->h_ਲੱਦੋ.ਭਾਰ);
	s64 ਕੁੰਜੀ = entity_ਕੁੰਜੀ(cfs_rq, se);

	cfs_rq->ਜੋੜ_w_vruntime -= ਕੁੰਜੀ * ਭਾਰ;
	cfs_rq->ਜੋੜ_ਭਾਰ -= ਭਾਰ;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ
ਖਾਲੀ update_zero_vruntime(ਬਣਤਰ cfs_rq *cfs_rq, s64 delta)
{
	/*
	 * v' = v + d ==> sum_w_vruntime' = sum_w_vruntime - d*sum_weight
	 */
	cfs_rq->ਜੋੜ_w_vruntime -= cfs_rq->ਜੋੜ_ਭਾਰ * delta;
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
u64 avg_vruntime(ਬਣਤਰ cfs_rq *cfs_rq)
{
	ਬਣਤਰ sched_entity *curr = cfs_rq->curr;
	ਲੰਮਾ ਭਾਰ = cfs_rq->ਜੋੜ_ਭਾਰ;
	s64 delta = 0;

	ਜੇ (curr && !curr->on_rq)
		curr = ਨਿਰਾ;

	ਜੇ (ਭਾਰ) {
		s64 runtime = cfs_rq->ਜੋੜ_w_vruntime;

		ਜੇ (curr) {
			ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ w = avg_vruntime_ਭਾਰ(cfs_rq, curr->h_ਲੱਦੋ.ਭਾਰ);

			runtime += entity_ਕੁੰਜੀ(cfs_rq, curr) * w;
			ਭਾਰ += w;
		}

		/* sign flips effective floor / ceiling */
		ਜੇ (runtime < 0)
			runtime -= (ਭਾਰ - 1);

		delta = div64_long(runtime, ਭਾਰ);
	} ਨਹੀਂਤਾਂ ਜੇ (curr) {
		/*
		 * When there is but one element, it is the average.
		 */
		delta = curr->vruntime - cfs_rq->zero_vruntime;
	}

	update_zero_vruntime(cfs_rq, delta);

	ਵਾਪਸ cfs_rq->zero_vruntime;
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
ਸਥਿਰ u64 ineligible_vruntime(ਬਣਤਰ cfs_rq *cfs_rq)
{
	ਬਣਤਰ sched_entity *curr = cfs_rq->curr;
	ਲੰਮਾ ਭਾਰ = cfs_rq->ਜੋੜ_ਭਾਰ;
	s64 delta = 0;

	ਜੇ (curr && !curr->on_rq)
		curr = ਨਿਰਾ;

	ਜੇ (ਭਾਰ) {
		s64 runtime = cfs_rq->ਜੋੜ_w_vruntime;

		/*
		 * Do not add @curr to obtain the effective '- w_j' terms.
		 */

		/* sign flips effective floor / ceiling */
		ਜੇ (runtime < 0)
			runtime -= (ਭਾਰ - 1);

		delta = div64_long(runtime, ਭਾਰ);
	}

	ਵਾਪਸ cfs_rq->zero_vruntime + delta + 1;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ u64 cfs_rq_max_ਟੁਕੜਾ(ਬਣਤਰ cfs_rq *cfs_rq);

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
ਸਥਿਰ s64 entity_lag(ਬਣਤਰ cfs_rq *cfs_rq, ਬਣਤਰ sched_entity *se, u64 avruntime)
{
	u64 max_ਟੁਕੜਾ = cfs_rq_max_ਟੁਕੜਾ(cfs_rq) + TICK_NSEC;
	s64 vlag, ਹੱਦਬੰਦੀ;

	vlag = avruntime - se->vruntime;
	ਹੱਦਬੰਦੀ = calc_delta_fair(max_ਟੁਕੜਾ, se);

	ਵਾਪਸ clamp(vlag, -ਹੱਦਬੰਦੀ, ਹੱਦਬੰਦੀ);
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
ਸਥਿਰ __always_inline
bool update_entity_lag(ਬਣਤਰ cfs_rq *cfs_rq, ਬਣਤਰ sched_entity *se)
{
	u64 avruntime = avg_vruntime(cfs_rq);
	s64 vlag = entity_lag(cfs_rq, se, avruntime);

	ਜੇ (se->sched_delayed) {
		/* previous vlag < 0 otherwise se would not be delayed */
		vlag = max(vlag, se->vlag);
		ਜੇ (sched_feat(DELAY_ZERO))
			vlag = min(vlag, 0);
	}
	se->vlag = vlag;

	ਵਾਪਸ avruntime - vlag != se->vruntime;
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
ਸਥਿਰ ਪੂਰਨ vruntime_eligible(ਬਣਤਰ cfs_rq *cfs_rq, u64 vruntime)
{
	ਬਣਤਰ sched_entity *curr = cfs_rq->curr;
	s64 ਕੁੰਜੀ, avg = cfs_rq->ਜੋੜ_w_vruntime;
	ਲੰਮਾ ਲੱਦੋ = cfs_rq->ਜੋੜ_ਭਾਰ;

	ਜੇ (curr && curr->on_rq) {
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਭਾਰ = avg_vruntime_ਭਾਰ(cfs_rq, curr->h_ਲੱਦੋ.ਭਾਰ);

		avg += entity_ਕੁੰਜੀ(cfs_rq, curr) * ਭਾਰ;
		ਲੱਦੋ += ਭਾਰ;
	}

	ਕੁੰਜੀ = vruntime_op(vruntime, "-", cfs_rq->zero_vruntime);

	/*
	 * The worst case term for @key includes 'NSEC_TICK * NICE_0_LOAD'
	 * and @load obviously includes NICE_0_LOAD. NSEC_TICK is around 24
	 * bits, while NICE_0_LOAD is 20 on 64bit and 10 otherwise.
	 *
	 * This gives that on 64bit the product will be at least 64bit which
	 * overflows s64, while on 32bit it will only be 44bits and should fit
	 * comfortably.
	 */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_64_ਦੋਆਧਾਰੀ_ਅੰਕ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_ARCH_SUPPORTS_INT128
	/* This often results in simpler code than __builtin_mul_overflow(). */
	ਵਾਪਸ avg >= (__int128)ਕੁੰਜੀ * ਲੱਦੋ;
#ਨਹੀਂਤਾਂ
	s64 rhs;
	/*
	 * On overflow, the sign of key tells us the correct answer: a large
	 * positive key means vruntime >> V, so not eligible; a large negative
	 * key means vruntime << V, so eligible.
	 */
	ਜੇ (check_mul_ਉਛਾਲ(ਕੁੰਜੀ, ਲੱਦੋ, &rhs))
		ਵਾਪਸ ਕੁੰਜੀ <= 0;

	ਵਾਪਸ avg >= rhs;
#ਜੇ_ਅੰਤ
#ਨਹੀਂਤਾਂ /* 32bit */
	ਵਾਪਸ avg >= ਕੁੰਜੀ * ਲੱਦੋ;
#ਜੇ_ਅੰਤ
}

ਪੂਰਨ entity_eligible(ਬਣਤਰ cfs_rq *cfs_rq, ਬਣਤਰ sched_entity *se)
{
	ਵਾਪਸ vruntime_eligible(cfs_rq, se->vruntime);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ u64 cfs_rq_min_ਟੁਕੜਾ(ਬਣਤਰ cfs_rq *cfs_rq)
{
	ਬਣਤਰ sched_entity *ਜੜ੍ਹ = __pick_root_entity(cfs_rq);
	ਬਣਤਰ sched_entity *curr = cfs_rq->curr;
	u64 min_ਟੁਕੜਾ = ~0ULL;

	ਜੇ (curr && curr->on_rq)
		min_ਟੁਕੜਾ = curr->ਟੁਕੜਾ;

	ਜੇ (ਜੜ੍ਹ)
		min_ਟੁਕੜਾ = min(min_ਟੁਕੜਾ, ਜੜ੍ਹ->min_ਟੁਕੜਾ);

	ਵਾਪਸ min_ਟੁਕੜਾ;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ u64 cfs_rq_max_ਟੁਕੜਾ(ਬਣਤਰ cfs_rq *cfs_rq)
{
	ਬਣਤਰ sched_entity *ਜੜ੍ਹ = __pick_root_entity(cfs_rq);
	ਬਣਤਰ sched_entity *curr = cfs_rq->curr;
	u64 max_ਟੁਕੜਾ = 0ULL;

	ਜੇ (curr && curr->on_rq)
		max_ਟੁਕੜਾ = curr->ਟੁਕੜਾ;

	ਜੇ (ਜੜ੍ਹ)
		max_ਟੁਕੜਾ = max(max_ਟੁਕੜਾ, ਜੜ੍ਹ->max_ਟੁਕੜਾ);

	ਵਾਪਸ max_ਟੁਕੜਾ;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool __entity_less(ਬਣਤਰ rb_ਗੰਢ *a, ਅਚਲ ਬਣਤਰ rb_ਗੰਢ *b)
{
	ਵਾਪਸ entity_before(__node_2_se(a), __node_2_se(b));
}

/* min() for wrapping vruntimes */
ਸਥਿਰ ਸਤਰਅੰਦਰ u64 __min_vruntime(u64 a, u64 b)
{
	ਵਾਪਸ vruntime_cmp(a, "<", b) ? a : b;
}

RB_DECLARE_CALLBACKS(ਸਥਿਰ, min_vruntime_cb,
		     ਬਣਤਰ sched_entity, ਲੜੀਵਾਰ_ਗੰਢ,
		     RB_AUG(vruntime, min_vruntime, __min_vruntime),
		     RB_AUG(ਟੁਕੜਾ, min_ਟੁਕੜਾ, min),
		     RB_AUG(ਟੁਕੜਾ, max_ਟੁਕੜਾ, max));

/*
 * Enqueue an entity into the rb-tree:
 */
ਸਥਿਰ ਖਾਲੀ __enqueue_entity(ਬਣਤਰ cfs_rq *cfs_rq, ਬਣਤਰ sched_entity *se)
{
	WARN_ON_ONCE(&rq_ਦਾ(cfs_rq)->cfs != cfs_rq);
	WARN_ON_ONCE(!entity_is_task(se));

	ਜੋੜ_w_vruntime_ਜੋੜੋ(cfs_rq, se);
	se->min_vruntime = se->vruntime;
	se->min_ਟੁਕੜਾ = se->ਟੁਕੜਾ;
	se->max_ਟੁਕੜਾ = se->ਟੁਕੜਾ;

	rb_ਜੋੜੋ_augmented_cached(&se->ਲੜੀਵਾਰ_ਗੰਢ, &cfs_rq->tasks_timeline,
				__entity_less, &min_vruntime_cb);
}

ਸਥਿਰ ਖਾਲੀ __dequeue_entity(ਬਣਤਰ cfs_rq *cfs_rq, ਬਣਤਰ sched_entity *se)
{
	WARN_ON_ONCE(&rq_ਦਾ(cfs_rq)->cfs != cfs_rq);
	WARN_ON_ONCE(!entity_is_task(se));

	rb_erase_augmented_cached(&se->ਲੜੀਵਾਰ_ਗੰਢ, &cfs_rq->tasks_timeline,
				  &min_vruntime_cb);
	ਜੋੜ_w_vruntime_sub(cfs_rq, se);
}

ਬਣਤਰ sched_entity *__pick_root_entity(ਬਣਤਰ cfs_rq *cfs_rq)
{
	ਬਣਤਰ rb_ਗੰਢ *ਜੜ੍ਹ = cfs_rq->tasks_timeline.rb_ਜੜ੍ਹ.rb_ਗੰਢ;

	ਜੇ (!ਜੜ੍ਹ)
		ਵਾਪਸ ਨਿਰਾ;

	ਵਾਪਸ __node_2_se(ਜੜ੍ਹ);
}

ਬਣਤਰ sched_entity *__pick_first_entity(ਬਣਤਰ cfs_rq *cfs_rq)
{
	ਬਣਤਰ rb_ਗੰਢ *ਖੱਬਾ = rb_ਪਹਿਲਾ_cached(&cfs_rq->tasks_timeline);

	ਜੇ (!ਖੱਬਾ)
		ਵਾਪਸ ਨਿਰਾ;

	ਵਾਪਸ __node_2_se(ਖੱਬਾ);
}

/*
 * Set the vruntime up to which an entity can run before looking
 * for another entity to pick.
 * In case of run to parity, we use the shortest slice of the enqueued
 * entities to set the protected period.
 * When run to parity is disabled, we give a minimum quantum to the running
 * entity to ensure progress.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ ਸੈੱਟ_protect_ਟੁਕੜਾ(ਬਣਤਰ cfs_rq *cfs_rq, ਬਣਤਰ sched_entity *se)
{
	u64 ਟੁਕੜਾ = normalized_sysctl_sched_ਅਧਾਰ_ਟੁਕੜਾ;
	u64 vprot = se->deadline;

	ਜੇ (sched_feat(ਲੜੀਵਾਰ_ਮੰਜ਼ਿਲ_PARITY))
		ਟੁਕੜਾ = cfs_rq_min_ਟੁਕੜਾ(cfs_rq);

	ਟੁਕੜਾ = min(ਟੁਕੜਾ, se->ਟੁਕੜਾ);

	/* If there are shorter slices than se's one */
	ਜੇ (ਟੁਕੜਾ != se->ਟੁਕੜਾ) {
		vprot = min_vruntime(vprot, se->vruntime + calc_delta_fair(ਟੁਕੜਾ, se));
		ਜੇ (sched_feat(PREEMPT_SHORT))
			vprot = min_vruntime(vprot, ineligible_vruntime(cfs_rq));
	}

	se->vprot = vprot;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ update_protect_ਟੁਕੜਾ(ਬਣਤਰ cfs_rq *cfs_rq, ਬਣਤਰ sched_entity *se)
{
	u64 vruntime = min_vruntime(se->vruntime, avg_vruntime(cfs_rq));
	u64 ਟੁਕੜਾ = cfs_rq_min_ਟੁਕੜਾ(cfs_rq);
	u64 vprot = min_vruntime(se->vprot, vruntime + calc_delta_fair(ਟੁਕੜਾ, se));

	ਜੇ (sched_feat(PREEMPT_SHORT) && ਟੁਕੜਾ != se->ਟੁਕੜਾ)
		vprot = min_vruntime(vprot, ineligible_vruntime(cfs_rq));

	se->vprot = vprot;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool protect_ਟੁਕੜਾ(ਬਣਤਰ sched_entity *se)
{
	ਵਾਪਸ vruntime_cmp(se->vruntime, "<", se->vprot);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ cancel_protect_ਟੁਕੜਾ(ਬਣਤਰ sched_entity *se)
{
	ਜੇ (protect_ਟੁਕੜਾ(se))
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
ਸਥਿਰ ਬਣਤਰ sched_entity *pick_eevdf(ਬਣਤਰ cfs_rq *cfs_rq, bool protect)
{
	ਬਣਤਰ rb_ਗੰਢ *ਗੰਢ = cfs_rq->tasks_timeline.rb_ਜੜ੍ਹ.rb_ਗੰਢ;
	ਬਣਤਰ sched_entity *se = __pick_first_entity(cfs_rq);
	ਬਣਤਰ sched_entity *curr = cfs_rq->curr;
	ਬਣਤਰ sched_entity *ਉੱਤਮ = ਨਿਰਾ;

	/*
	 * We can safely skip eligibility check if there is only one entity
	 * in this cfs_rq, saving some cycles.
	 */
	ਜੇ (cfs_rq->h_nr_queued == 1)
		ਵਾਪਸ curr && curr->on_rq ? curr : se;

	/*
	 * Picking the ->next buddy will affect latency but not fairness.
	 */
	ਜੇ (sched_feat(PICK_BUDDY) && protect &&
	    cfs_rq->ਅਗਲਾ && entity_eligible(cfs_rq, cfs_rq->ਅਗਲਾ)) {
		/* ->next will never be delayed */
		WARN_ON_ONCE(cfs_rq->ਅਗਲਾ->sched_delayed);
		ਵਾਪਸ cfs_rq->ਅਗਲਾ;
	}

	ਜੇ (curr && (!curr->on_rq || !entity_eligible(cfs_rq, curr)))
		curr = ਨਿਰਾ;

	ਜੇ (curr && protect && protect_ਟੁਕੜਾ(curr))
		ਵਾਪਸ curr;

	/* Pick the leftmost entity if it's eligible */
	ਜੇ (se && entity_eligible(cfs_rq, se)) {
		ਉੱਤਮ = se;
		ਜਾਓ ਮਿਲਿਆ;
	}

	/* Heap search for the EEVD entity */
	ਜਦੋਂ (ਗੰਢ) {
		ਬਣਤਰ rb_ਗੰਢ *ਖੱਬਾ = ਗੰਢ->rb_ਖੱਬਾ;

		/*
		 * Eligible entities in left subtree are always better
		 * choices, since they have earlier deadlines.
		 */
		ਜੇ (ਖੱਬਾ && vruntime_eligible(cfs_rq,
					__node_2_se(ਖੱਬਾ)->min_vruntime)) {
			ਗੰਢ = ਖੱਬਾ;
			ਜਾਰੀ;
		}

		se = __node_2_se(ਗੰਢ);

		/*
		 * The left subtree either is empty or has no eligible
		 * entity, so check the current node since it is the one
		 * with earliest deadline that might be eligible.
		 */
		ਜੇ (entity_eligible(cfs_rq, se)) {
			ਉੱਤਮ = se;
			ਰੋਕੋ;
		}

		ਗੰਢ = ਗੰਢ->rb_ਸੱਜਾ;
	}
ਮਿਲਿਆ:
	ਜੇ (!ਉੱਤਮ || (curr && entity_before(curr, ਉੱਤਮ)))
		ਉੱਤਮ = curr;

	ਵਾਪਸ ਉੱਤਮ;
}

ਬਣਤਰ sched_entity *__pick_last_entity(ਬਣਤਰ cfs_rq *cfs_rq)
{
	ਬਣਤਰ rb_ਗੰਢ *ਆਖਰੀ = rb_ਆਖਰੀ(&cfs_rq->tasks_timeline.rb_ਜੜ੍ਹ);

	ਜੇ (!ਆਖਰੀ)
		ਵਾਪਸ ਨਿਰਾ;

	ਵਾਪਸ __node_2_se(ਆਖਰੀ);
}

/**************************************************************
 * Scheduling class statistics methods:
 */
ਪੂਰਨ sched_update_scaling(ਖਾਲੀ)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗੁਣਕ = get_update_sysctl_ਗੁਣਕ();

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ WRT_SYSCTL(name) \
	(normalized_sysctl_##name = sysctl_##name / (ਗੁਣਕ))
	WRT_SYSCTL(sched_ਅਧਾਰ_ਟੁਕੜਾ);
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ WRT_SYSCTL

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ clear_buddies(ਬਣਤਰ cfs_rq *cfs_rq, ਬਣਤਰ sched_entity *se);

/*
 * XXX: strictly: vd_i += N*r_i/w_i such that: vd_i > ve_i
 * this is probably good enough.
 */
ਸਥਿਰ bool update_deadline(ਬਣਤਰ cfs_rq *cfs_rq, ਬਣਤਰ sched_entity *se)
{
	ਜੇ (vruntime_cmp(se->vruntime, "<", se->deadline))
		ਵਾਪਸ false;

	/*
	 * For EEVDF the virtual time slope is determined by w_i (iow.
	 * nice) while the request time r_i is determined by
	 * sysctl_sched_base_slice.
	 */
	ਜੇ (!se->custom_ਟੁਕੜਾ)
		se->ਟੁਕੜਾ = sysctl_sched_ਅਧਾਰ_ਟੁਕੜਾ;

	/*
	 * EEVDF: vd_i = ve_i + r_i / w_i
	 */
	se->deadline = se->vruntime + calc_delta_fair(se->ਟੁਕੜਾ, se);
	avg_vruntime(cfs_rq);

	/*
	 * The task has consumed its request, reschedule.
	 */
	ਵਾਪਸ true;
}

#ਅਨ੍ਤਰ੍ਭਾਵ "pelt.h"

ਸਥਿਰ ਪੂਰਨ select_idle_sibling(ਬਣਤਰ task_struct *p, ਪੂਰਨ prev_cpu, ਪੂਰਨ cpu);
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ task_h_ਲੱਦੋ(ਬਣਤਰ task_struct *p);
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਸਮਰੱਥਾ_ਦਾ(ਪੂਰਨ cpu);

/* Give new sched_entity start runnable values to heavy its load in infant time */
ਖਾਲੀ init_entity_runnable_ਔਸਤਨ(ਬਣਤਰ sched_entity *se)
{
	ਬਣਤਰ sched_avg *sa = &se->avg;

	memset(sa, 0, ਆਕਾਰ(*sa));

	/*
	 * Tasks are initialized with full load to be seen as heavy tasks until
	 * they get a chance to stabilize to their real load level.
	 * Group entities are initialized with zero load to reflect the fact that
	 * nothing has been attached to the task group yet.
	 */
	ਜੇ (entity_is_task(se))
		sa->ਲੱਦੋ_avg = scale_ਲੱਦੋ_ਥੱਲੇ(se->ਲੱਦੋ.ਭਾਰ);

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
ਖਾਲੀ post_init_entity_util_avg(ਬਣਤਰ task_struct *p)
{
	ਬਣਤਰ sched_entity *se = &p->se;
	ਬਣਤਰ cfs_rq *cfs_rq = cfs_rq_ਦਾ(se);
	ਬਣਤਰ sched_avg *sa = &se->avg;
	ਲੰਮਾ cpu_scale = arch_scale_cpu_ਸਮਰੱਥਾ(cpu_ਦਾ(rq_ਦਾ(cfs_rq)));
	ਲੰਮਾ cap = (ਲੰਮਾ)(cpu_scale - cfs_rq->avg.util_avg) / 2;

	ਜੇ (p->sched_class != &fair_sched_class) {
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
		se->avg.ਆਖਰੀ_update_ਸਮਾਂ = cfs_rq_clock_pelt(cfs_rq);
		ਵਾਪਸ;
	}

	ਜੇ (cap > 0) {
		ਜੇ (cfs_rq->avg.util_avg != 0) {
			sa->util_avg  = cfs_rq->avg.util_avg * se_ਭਾਰ(se);
			sa->util_avg /= (cfs_rq->avg.ਲੱਦੋ_avg + 1);

			ਜੇ (sa->util_avg > cap)
				sa->util_avg = cap;
		} ਨਹੀਂਤਾਂ {
			sa->util_avg = cap;
		}
	}

	sa->runnable_avg = sa->util_avg;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ account_mm_sched(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, s64 delta_exec);

ਸਥਿਰ s64 update_se(ਬਣਤਰ rq *rq, ਬਣਤਰ sched_entity *se)
{
	u64 now = rq_clock_task(rq);
	s64 delta_exec;

	delta_exec = now - se->exec_ਸ਼ੁਰੂ;
	ਜੇ (unlikely(delta_exec <= 0))
		ਵਾਪਸ delta_exec;

	se->exec_ਸ਼ੁਰੂ = now;
	ਜੇ (entity_is_task(se)) {
		ਬਣਤਰ task_struct *running = rq->curr;
		/*
		 * If se is a task, we account the time against the running
		 * task, as w/ proxy-exec they may not be the same.
		 */
		running->se.exec_ਸ਼ੁਰੂ = now;
		running->se.ਜੋੜ_exec_runtime += delta_exec;

		trace_sched_stat_runtime(running, delta_exec);
		account_ਸਮੂਹ_exec_runtime(running, delta_exec);
		account_mm_sched(rq, running, delta_exec);

		cgroup_account_cputime(running, delta_exec);
	} ਨਹੀਂਤਾਂ {
		/* If not task, account the time against donor se  */
		se->ਜੋੜ_exec_runtime += delta_exec;
	}

	ਜੇ (schedstat_enabled()) {
		ਬਣਤਰ sched_statistics *stats;

		stats = __schedstats_from_se(se);
		__schedstat_set(stats->exec_max,
				max(delta_exec, stats->exec_max));
	}

	ਵਾਪਸ delta_exec;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CACHE

/*
 * XXX numbers come from a place the sun don't shine -- probably wants to be SD
 * tunable or so.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EPOCH_PERIOD	(HZ / 100)	/* 10 ms */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EPOCH_LLC_AFFINITY_TIMEOUT	5	/* 50 ms */
__read_mostly ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ llc_aggr_tolerance	= 1;
__read_mostly ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ llc_epoch_period	= EPOCH_PERIOD;
__read_mostly ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ llc_epoch_affinity_timeout = EPOCH_LLC_AFFINITY_TIMEOUT;
__read_mostly ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ llc_imb_pct		= 20;
__read_mostly ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ llc_overaggr_pct	= 50;

ਸਥਿਰ ਪੂਰਨ llc_id(ਪੂਰਨ cpu)
{
	ਜੇ (cpu < 0)
		ਵਾਪਸ -1;

	ਵਾਪਸ ਪ੍ਰਤੀ_cpu(sd_llc_id, cpu);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ get_sched_cache_scale(ਪੂਰਨ mul)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ tol = READ_ONCE(llc_aggr_tolerance);

	ਜੇ (!tol)
		ਵਾਪਸ 0;

	ਜੇ (tol >= 100)
		ਵਾਪਸ ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ;

	ਵਾਪਸ (1 + (tol - 1) * mul);
}

ਸਥਿਰ bool exceed_llc_ਸਮਰੱਥਾ(ਬਣਤਰ sched_cache_ਸਮੂਹ *grp, ਪੂਰਨ cpu)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_NUMA_BALANCING
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ llc, footprint;
	ਬਣਤਰ sched_domain *sd;
	ਪੂਰਨ scale;

	guard(rcu)();

	sd = rcu_dereference_sched_domain(cpu_rq(cpu)->sd);
	ਜੇ (!sd)
		ਵਾਪਸ true;

	ਜੇ (static_branch_likely(&sched_numa_balancing)) {
		/*
		 * TBD: RDT exclusive LLC ways reserved should be
		 * excluded.
		 */
		llc = sd->llc_ਬਾਈਟਾਂ;
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
		ਜੇ (scale == ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ)
			ਵਾਪਸ false;

		ਵਾਪਸ ((llc * (u64)scale) < (footprint * ਪੰਨਾ_ਮਾਪ));
	}
#ਜੇ_ਅੰਤ
	ਵਾਪਸ false;
}

ਸਥਿਰ bool invalid_llc_nr(ਬਣਤਰ sched_cache_ਸਮੂਹ *grp, ਬਣਤਰ task_struct *p,
			   ਪੂਰਨ cpu)
{
	ਪੂਰਨ scale;

	ਜੇ (get_nr_ਧਾਗੇ(p) <= 1)
		ਵਾਪਸ true;

	/*
	 * Scale the number of 'cores' in a LLC by llc_aggr_tolerance
	 * and compare it to the task's active threads.
	 */
	scale = get_sched_cache_scale(1);
	ਜੇ (scale == ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ)
		ਵਾਪਸ false;

	ਵਾਪਸ !ਸਮਾਉਂਦਾ_ਸਮਰੱਥਾ((READ_ONCE(grp->nr_running_avg) * cpu_smt_ਅੰਕ_ਧਾਗੇ),
			(scale * ਪ੍ਰਤੀ_cpu(sd_llc_ਮਾਪ, cpu)));
}

/*
 * A task counts in nr_pref_llc_running while it is queued on its preferred
 * LLC (pref_llc_queued) and runnable (!sched_delayed), keeping the counter in
 * the runnable domain so alb_break_llc() can compare it with h_nr_runnable.
 */
ਸਥਿਰ bool task_pref_llc_runnable(ਬਣਤਰ task_struct *p)
{
	ਵਾਪਸ p->pref_llc_queued && !p->se.sched_delayed;
}

ਸਥਿਰ ਖਾਲੀ pref_llc_running_inc(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p)
{
	ਜੇ (task_pref_llc_runnable(p))
		rq->nr_pref_llc_running++;
}

ਸਥਿਰ ਖਾਲੀ pref_llc_running_dec(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p)
{
	ਜੇ (task_pref_llc_runnable(p))
		rq->nr_pref_llc_running--;
}

ਸਥਿਰ ਖਾਲੀ account_llc_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p)
{
	ਪੂਰਨ pref_llc, pref_llc_queued;
	ਬਣਤਰ sched_domain *sd;

	pref_llc = p->preferred_llc;
	ਜੇ (pref_llc < 0)
		ਵਾਪਸ;

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
	ਜੇ (sd && (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)pref_llc < sd->llc_max)
		sd->llc_counts[pref_llc]++;
}

ਸਥਿਰ ਖਾਲੀ account_llc_ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p)
{
	ਬਣਤਰ sched_domain *sd;
	ਪੂਰਨ pref_llc;

	pref_llc = p->preferred_llc;
	ਜੇ (pref_llc < 0)
		ਵਾਪਸ;

	rq->nr_llc_running--;
	ਜੇ (p->pref_llc_queued) {
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
	ਜੇ (sd && (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)pref_llc < sd->llc_max) {
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
		ਜੇ (sd->llc_counts[pref_llc])
			sd->llc_counts[pref_llc]--;
	}
}

ਪੂਰਨ mm_init_sched(ਬਣਤਰ mm_struct *mm,
		  ਬਣਤਰ sched_cache_ਸਮਾਂ __percpu *_pcpu_sched)
{
	ਬਣਤਰ sched_cache_ਸਮੂਹ *grp;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ epoch = 0;
	ਪੂਰਨ i;

	grp = kzalloc_obj(*grp);
	ਜੇ (!grp) {
		free_percpu(_pcpu_sched);
		mm->sched_cache_grp = ਨਿਰਾ;
		ਵਾਪਸ -ENOMEM;
	}

	for_each_possible_cpu(i) {
		ਬਣਤਰ sched_cache_ਸਮਾਂ *pcpu_sched = ਪ੍ਰਤੀ_cpu_ptr(_pcpu_sched, i);
		ਬਣਤਰ rq *rq = cpu_rq(i);

		pcpu_sched->runtime = 0;
		/* a slightly stale cpu epoch is acceptible */
		pcpu_sched->epoch = rq->cpu_epoch;
		epoch = rq->cpu_epoch;
	}

	raw_spin_ਤਾਲਾ_init(&grp->ਤਾਲਾ);
	grp->epoch = epoch;
	grp->cpu = -1;
	grp->ਅਗਲਾ_ਜਾਂਚ = jiffies;
	grp->nr_running_avg = 0;
	grp->footprint = 0;
	refcount_ਸੈੱਟ(&grp->refcnt, 1);
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
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ sched_cache_ਸਮੂਹ_free_rcu(ਬਣਤਰ rcu_ਸਿਰ *rcu)
{
	ਬਣਤਰ sched_cache_ਸਮੂਹ *grp =
		container_ਦਾ(rcu, ਬਣਤਰ sched_cache_ਸਮੂਹ, rcu);

	free_percpu(grp->pcpu_sched);
	kfree(grp);
}

ਸਥਿਰ ਖਾਲੀ sched_cache_ਸਮੂਹ_put(ਬਣਤਰ sched_cache_ਸਮੂਹ *grp)
{
	ਜੇ (!grp || !refcount_dec_and_test(&grp->refcnt))
		ਵਾਪਸ;

	call_rcu(&grp->rcu, sched_cache_ਸਮੂਹ_free_rcu);
}

DEFINE_FREE(sched_cache_ਸਮੂਹ_put, ਬਣਤਰ sched_cache_ਸਮੂਹ *,
	    sched_cache_ਸਮੂਹ_put(_T));

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ rcu_deref_sched_cache_grp(tsk) \
	rcu_dereference_check((tsk)->sched_cache_grp, (tsk) == current)

ਸਥਿਰ ਬਣਤਰ sched_cache_ਸਮੂਹ *sched_cache_replace_grp(ਬਣਤਰ task_struct *p,
							 ਬਣਤਰ sched_cache_ਸਮੂਹ *new)
{
	ਬਣਤਰ sched_cache_ਸਮੂਹ *old;

	old = rcu_deref_sched_cache_grp(p);
	rcu_ਸੌਂਪੋ_ਸੂਚਕ(p->sched_cache_grp, new);

	ਵਾਪਸ old;
}

ਬਣਤਰ sched_cache_ਸਮੂਹ *sched_cache_ਸਮੂਹ_get(ਬਣਤਰ sched_cache_ਸਮੂਹ *grp)
{
	/*
	 * refcount_inc_not_zero() is the acquire primitive for lockless
	 * (RCU) lookups; plain refcount_inc() would scribble the count if
	 * it already reached zero. Return NULL in that case.
	 */
	ਜੇ (grp && !refcount_inc_not_zero(&grp->refcnt))
		grp = ਨਿਰਾ;

	ਵਾਪਸ grp;
}

ਬਣਤਰ sched_cache_ਸਮੂਹ *task_cache_ਸਮੂਹ_get(ਬਣਤਰ task_struct *p)
{
	guard(rcu)();
	ਵਾਪਸ sched_cache_ਸਮੂਹ_get(rcu_dereference(p->sched_cache_grp));
}

ਖਾਲੀ sched_cache_fork(ਬਣਤਰ task_struct *p)
{
	/*
	 * The child takes its own reference on the mm's cache group, separate
	 * from the reference held by the mm. @p is not yet visible to readers,
	 * so a plain initializing store is enough.
	 */
	RCU_INIT_ਸੂਚਕ(p->sched_cache_grp,
			 sched_cache_ਸਮੂਹ_get(p->mm->sched_cache_grp));
}

ਖਾਲੀ sched_cache_fork_cleanup(ਬਣਤਰ task_struct *p)
{
	/*
	 * A fork that fails after sched_cache_fork() never reaches exit_mm(),
	 * so drop the reference here. @p never became visible, so there are no
	 * concurrent readers and the reference we hold keeps the group alive.
	 */
	sched_cache_ਸਮੂਹ_put(rcu_access_ਸੂਚਕ(p->sched_cache_grp));
	RCU_INIT_ਸੂਚਕ(p->sched_cache_grp, ਨਿਰਾ);
}

ਖਾਲੀ sched_cache_exec_mmap(ਬਣਤਰ task_struct *p, ਬਣਤਰ mm_struct *mm)
{
	ਬਣਤਰ sched_cache_ਸਮੂਹ *old;

	/*
	 * Acquire the new reference before publishing the pointer, then drop
	 * the old one. @p is current and the only writer of its own pointer.
	 */
	old = sched_cache_replace_grp(p, sched_cache_ਸਮੂਹ_get(mm->sched_cache_grp));
	sched_cache_ਸਮੂਹ_put(old);
}

ਖਾਲੀ sched_cache_exit_mm(ਬਣਤਰ task_struct *p)
{
	ਬਣਤਰ sched_cache_ਸਮੂਹ *grp = sched_cache_replace_grp(p, ਨਿਰਾ);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_NUMA_BALANCING
	/*
	 * Subtract this task's footprint from the group before dropping the
	 * reference, so the group footprint converges as its threads exit.
	 * Unlocked for performance; clamp to avoid underflow.
	 */
	ਜੇ (grp && p->ਕੁੱਲ_numa_ਨੁਕਸ) {
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ fp = READ_ONCE(grp->footprint);
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ sub = min(fp, p->ਕੁੱਲ_numa_ਨੁਕਸ);

		WRITE_ONCE(grp->footprint, fp - sub);
	}
#ਜੇ_ਅੰਤ
	sched_cache_ਸਮੂਹ_put(grp);
}

ਖਾਲੀ mm_destroy_sched(ਬਣਤਰ mm_struct *mm)
{
	sched_cache_ਸਮੂਹ_put(mm->sched_cache_grp);
	mm->sched_cache_grp = ਨਿਰਾ;
}

/* because why would C be fully specified */
ਸਥਿਰ __always_inline ਖਾਲੀ __shr_u64(u64 *val, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ n)
{
	ਜੇ (n >= 64) {
		*val = 0;
		ਵਾਪਸ;
	}
	*val >>= n;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ __update_mm_sched(ਬਣਤਰ rq *rq,
				     ਬਣਤਰ sched_cache_ਸਮਾਂ *pcpu_sched)
{
	lockdep_assert_held(&rq->cpu_epoch_ਤਾਲਾ);

	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ period = max(READ_ONCE(llc_epoch_period), 1U);
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ n, now = jiffies;
	ਲੰਮਾ delta = now - rq->cpu_epoch_ਅਗਲਾ;

	ਜੇ (delta > 0) {
		n = (delta + period - 1) / period;
		rq->cpu_epoch += n;
		rq->cpu_epoch_ਅਗਲਾ += n * period;
		__shr_u64(&rq->cpu_runtime, n);
	}

	n = rq->cpu_epoch - pcpu_sched->epoch;
	ਜੇ (n) {
		pcpu_sched->epoch += n;
		__shr_u64(&pcpu_sched->runtime, n);
	}
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ fraction_mm_sched(ਬਣਤਰ rq *rq,
				       ਬਣਤਰ sched_cache_ਸਮਾਂ *pcpu_sched)
{
	guard(raw_spinlock_irqsave)(&rq->cpu_epoch_ਤਾਲਾ);

	__update_mm_sched(rq, pcpu_sched);

	/*
	 * Runtime is a geometric series (r=0.5) and as such will sum to twice
	 * the accumulation period, this means the multiplcation here should
	 * not overflow.
	 */
	ਵਾਪਸ div64_u64(NICE_0_ਲੱਦੋ * pcpu_sched->runtime, rq->cpu_runtime + 1);
}

ਸਥਿਰ ਪੂਰਨ get_pref_llc(ਬਣਤਰ task_struct *p, ਬਣਤਰ sched_cache_ਸਮੂਹ *grp)
{
	ਪੂਰਨ mm_sched_llc = -1, mm_sched_cpu;

	ਜੇ (!grp)
		ਵਾਪਸ -1;

	mm_sched_cpu = READ_ONCE(grp->cpu);
	ਜੇ (mm_sched_cpu != -1) {
		mm_sched_llc = llc_id(mm_sched_cpu);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_NUMA_BALANCING
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
		ਜੇ (static_branch_likely(&sched_numa_balancing) &&
		    p->numa_preferred_nid >= 0 &&
		    cpu_ਮੰਜ਼ਿਲ_ਗੰਢ(mm_sched_cpu) != p->numa_preferred_nid)
			mm_sched_llc = -1;
#ਜੇ_ਅੰਤ
	}

	ਵਾਪਸ mm_sched_llc;
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ task_running_on_cpu(ਪੂਰਨ cpu, ਬਣਤਰ task_struct *p);

ਸਥਿਰ ਸਤਰਅੰਦਰ
ਖਾਲੀ account_mm_sched(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, s64 delta_exec)
{
	ਬਣਤਰ sched_cache_ਸਮੂਹ *grp = rcu_dereference_all(p->sched_cache_grp);
	ਬਣਤਰ sched_cache_ਸਮਾਂ *pcpu_sched;
	ਪੂਰਨ mm_sched_llc = -1;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ epoch;

	ਜੇ (!sched_cache_enabled())
		ਵਾਪਸ;

	ਜੇ (p->sched_class != &fair_sched_class)
		ਵਾਪਸ;
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
	ਜੇ (p->flags & PF_KTHREAD || !grp || !grp->pcpu_sched)
		ਵਾਪਸ;

	pcpu_sched = ਪ੍ਰਤੀ_cpu_ptr(grp->pcpu_sched, cpu_ਦਾ(rq));

	scoped_guard (raw_spinlock, &rq->cpu_epoch_ਤਾਲਾ) {
		__update_mm_sched(rq, pcpu_sched);
		pcpu_sched->runtime += delta_exec;
		rq->cpu_runtime += delta_exec;
		epoch = rq->cpu_epoch;
	}

	/*
	 * If this process hasn't hit task_cache_work() for a while invalidate
	 * its preferred state.
	 */
	ਜੇ ((ਲੰਮਾ)(epoch - READ_ONCE(grp->epoch)) > llc_epoch_affinity_timeout ||
	    invalid_llc_nr(grp, p, cpu_ਦਾ(rq)) ||
	    exceed_llc_ਸਮਰੱਥਾ(grp, cpu_ਦਾ(rq))) {
		ਜੇ (READ_ONCE(grp->cpu) != -1)
			WRITE_ONCE(grp->cpu, -1);
	}

	mm_sched_llc = get_pref_llc(p, grp);

	/* task not on rq accounted later in account_entity_enqueue() */
	ਜੇ (task_running_on_cpu(rq->cpu, p) &&
	    READ_ONCE(p->preferred_llc) != mm_sched_llc) {
		account_llc_ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ(rq, p);
		WRITE_ONCE(p->preferred_llc, mm_sched_llc);
		account_llc_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ(rq, p);
	}
}

ਸਥਿਰ ਖਾਲੀ task_tick_cache(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p)
{
	ਬਣਤਰ sched_cache_ਸਮੂਹ *grp = rcu_dereference_all(p->sched_cache_grp);
	ਬਣਤਰ callback_ਸਿਰ *work = &p->cache_work;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ epoch;

	ਜੇ (!sched_cache_enabled())
		ਵਾਪਸ;

	ਜੇ (!grp || p->flags & PF_KTHREAD ||
	    !grp->pcpu_sched)
		ਵਾਪਸ;

	epoch = rq->cpu_epoch;
	/* avoid moving backwards */
	ਜੇ (ਸਮਾਂ_after_eq(grp->epoch, epoch))
		ਵਾਪਸ;

	guard(raw_spinlock)(&grp->ਤਾਲਾ);

	ਜੇ (work->ਅਗਲਾ == work) {
		task_work_ਜੋੜੋ(p, work, TWA_RESUME);
		WRITE_ONCE(grp->epoch, epoch);
	}
}

ਸਥਿਰ ਖਾਲੀ get_ਜਾਂਚ_cpumasks(cpumask_var_t cpus, ਬਣਤਰ task_struct *p,
			      ਬਣਤਰ sched_cache_ਸਮੂਹ *grp)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_NUMA_BALANCING
	ਪੂਰਨ cpu, curr_cpu, nid, pref_nid;

	ਜੇ (!static_branch_likely(&sched_numa_balancing))
		ਜਾਓ ਬਾਹਰਲਾ;

	cpu = READ_ONCE(grp->cpu);
	ਜੇ (cpu != -1)
		nid = cpu_ਮੰਜ਼ਿਲ_ਗੰਢ(cpu);
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
	ਜੇ (pref_nid == NUMA_NO_ਗੰਢ)
		ਜਾਓ ਬਾਹਰਲਾ;

	cpumask_or(cpus, cpus, cpumask_ਦਾ_ਗੰਢ(pref_nid));

	/* honor the task's preferred LLC CPU */
	ਜੇ (cpu != -1 && !cpumask_test_cpu(cpu, cpus) && nid != NUMA_NO_ਗੰਢ)
		cpumask_or(cpus, cpus, cpumask_ਦਾ_ਗੰਢ(nid));

	/* make sure the task's current running node is included */
	ਜੇ (!cpumask_test_cpu(curr_cpu, cpus))
		cpumask_or(cpus, cpus, cpumask_ਦਾ_ਗੰਢ(cpu_ਮੰਜ਼ਿਲ_ਗੰਢ(curr_cpu)));

	ਵਾਪਸ;

ਬਾਹਰਲਾ:
#ਜੇ_ਅੰਤ
	cpumask_copy(cpus, cpu_online_ਨਕਾਬ);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ update_avg_scale(u64 *avg, u64 sample)
{
	ਪੂਰਨ ਗੁਣਕ = ਪ੍ਰਤੀ_cpu(sd_llc_ਮਾਪ, raw_smp_processor_id());
	s64 diff = sample - *avg;
	u32 ਭਾਜਕ;

	/*
	 * Scale the divisor based on the number of CPUs contained
	 * in the LLC. This scaling ensures smaller LLC domains use
	 * a smaller divisor to achieve more precise sensitivity to
	 * changes in nr_running, while larger LLC domains are capped
	 * at a maximum divisor of 8 which is the default smoothing
	 * factor of EWMA in update_avg().
	 */
	ਭਾਜਕ = clamp_t(u32, (ਗੁਣਕ >> 2), 2, 8);
	*avg += div64_s64(diff, ਭਾਜਕ);
}

ਸਥਿਰ ਖਾਲੀ task_cache_work(ਬਣਤਰ callback_ਸਿਰ *work)
{
	ਬਣਤਰ sched_cache_ਸਮੂਹ *grp __free(sched_cache_ਸਮੂਹ_put) = ਨਿਰਾ;
	cpumask_var_t cpus __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	ਪੂਰਨ cpu, m_a_cpu = -1, nr_running = 0, curr_cpu;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਅਗਲਾ_ਜਾਂਚ, now = jiffies;
	ਬਣਤਰ task_struct *p = current, *cur;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ curr_m_a_occ = 0;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ m_a_occ = 0;

	WARN_ON_ONCE(work != &p->cache_work);

	work->ਅਗਲਾ = work;

	ਜੇ (p->flags & PF_EXITING)
		ਵਾਪਸ;

	/*
	 * A reference makes sure grp is not released by others. The rcu
	 * lock can not be held till after zalloc_cpumask_var() below,
	 * because the latter might sleep.
	 */
	grp = task_cache_ਸਮੂਹ_get(p);
	ਜੇ (!grp)
		ਵਾਪਸ;

	ਅਗਲਾ_ਜਾਂਚ = READ_ONCE(grp->ਅਗਲਾ_ਜਾਂਚ);
	ਜੇ (ਸਮਾਂ_before(now, ਅਗਲਾ_ਜਾਂਚ))
		ਵਾਪਸ;

	/* only 1 thread is allowed to scan */
	ਜੇ (!try_cmpxchg(&grp->ਅਗਲਾ_ਜਾਂਚ, &ਅਗਲਾ_ਜਾਂਚ,
			 now + max_t(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ,
				     READ_ONCE(llc_epoch_period), 1)))
		ਵਾਪਸ;

	curr_cpu = task_cpu(p);
	ਜੇ (invalid_llc_nr(grp, p, curr_cpu) ||
	    exceed_llc_ਸਮਰੱਥਾ(grp, curr_cpu)) {
		ਜੇ (READ_ONCE(grp->cpu) != -1)
			WRITE_ONCE(grp->cpu, -1);

		ਵਾਪਸ;
	}

	ਜੇ (!zalloc_cpumask_var(&cpus, GFP_KERNEL))
		ਵਾਪਸ;

	scoped_guard (cpus_read_ਤਾਲਾ) {
		guard(rcu)();

		get_ਜਾਂਚ_cpumasks(cpus, p, grp);

		for_each_cpu(cpu, cpus) {
			/* XXX sched_cluster_active */
			ਬਣਤਰ sched_domain *sd = rcu_dereference_all(ਪ੍ਰਤੀ_cpu(sd_llc, cpu));
			ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ occ, m_occ = 0, a_occ = 0;
			ਪੂਰਨ m_cpu = -1, i;

			ਜੇ (!sd)
				ਜਾਰੀ;

			for_each_cpu(i, sched_domain_ਵਿਸਤਾਰ(sd)) {
				occ = fraction_mm_sched(cpu_rq(i),
							ਪ੍ਰਤੀ_cpu_ptr(grp->pcpu_sched, i));
				a_occ += occ;
				ਜੇ (occ > m_occ) {
					m_occ = occ;
					m_cpu = i;
				}

				/*
				 * rcu_access_pointer() is used because the
				 * pointer is only compared, never dereferenced.
				 */
				cur = rcu_dereference_all(cpu_rq(i)->curr);
				ਜੇ (cur && !(cur->flags & (PF_EXITING | PF_KTHREAD)) &&
				    rcu_access_ਸੂਚਕ(cur->sched_cache_grp) == grp)
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
			ਜੇ (a_occ > m_a_occ) {
				m_a_occ = a_occ;
				m_a_cpu = m_cpu;
			}

			ਜੇ (llc_id(cpu) == llc_id(READ_ONCE(grp->cpu)))
				curr_m_a_occ = a_occ;

			cpumask_andnot(cpus, cpus, sched_domain_ਵਿਸਤਾਰ(sd));
		}
	}

	ਜੇ (m_a_occ > (2 * curr_m_a_occ)) {
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

ਖਾਲੀ init_sched_mm(ਬਣਤਰ task_struct *p)
{
	ਬਣਤਰ callback_ਸਿਰ *work = &p->cache_work;

	init_task_work(work, task_cache_work);
	work->ਅਗਲਾ = work;
	/*
	 * dup_task_struct() copies the parent's task_struct, including its
	 * sched_cache_grp, for which the child holds no reference.  Clear it
	 * here - before copy_mm() runs - so the child never carries a
	 * borrowed pointer that the fork error path would put.
	 */
	RCU_INIT_ਸੂਚਕ(p->sched_cache_grp, ਨਿਰਾ);
	/*
	 * Reset new task's preference to avoid
	 * polluting account_llc_enqueue().
	 */
	p->preferred_llc = -1;
	p->pref_llc_queued = 0;
}

#ਨਹੀਂਤਾਂ /* CONFIG_SCHED_CACHE */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ account_mm_sched(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p,
				    s64 delta_exec) { }

ਖਾਲੀ init_sched_mm(ਬਣਤਰ task_struct *p) { }

ਸਥਿਰ ਖਾਲੀ task_tick_cache(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p) { }

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ get_pref_llc(ਬਣਤਰ task_struct *p,
			       ਬਣਤਰ mm_struct *mm)
{
	ਵਾਪਸ -1;
}

ਸਥਿਰ ਖਾਲੀ account_llc_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p) {}

ਸਥਿਰ ਖਾਲੀ account_llc_ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p) {}

ਸਥਿਰ ਖਾਲੀ pref_llc_running_inc(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p) {}

ਸਥਿਰ ਖਾਲੀ pref_llc_running_dec(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p) {}

#ਜੇ_ਅੰਤ /* CONFIG_SCHED_CACHE */

/*
 * Used by other classes to account runtime.
 */
s64 update_curr_common(ਬਣਤਰ rq *rq)
{
	ਵਾਪਸ update_se(rq, &rq->donor->se);
}

/*
 * Update the current task's runtime statistics.
 */
ਸਥਿਰ ਖਾਲੀ update_curr(ਬਣਤਰ cfs_rq *cfs_rq)
{
	/*
	 * Note: cfs_rq->curr corresponds to the task picked to
	 * run (ie: rq->donor.se) which due to proxy-exec may
	 * not necessarily be the actual task running
	 * (rq->curr.se). This is easy to confuse!
	 */
	ਬਣਤਰ sched_entity *curr = cfs_rq->h_curr;
	ਬਣਤਰ rq *rq = rq_ਦਾ(cfs_rq);
	s64 delta_exec;
	bool resched;

	ਜੇ (unlikely(!curr))
		ਵਾਪਸ;

	delta_exec = update_se(rq, curr);
	ਜੇ (unlikely(delta_exec <= 0))
		ਵਾਪਸ;

	account_cfs_rq_runtime(cfs_rq, delta_exec);

	ਜੇ (!entity_is_task(curr))
		ਵਾਪਸ;

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

	ਜੇ (cfs_rq->h_nr_queued == 1)
		ਵਾਪਸ;

	ਜੇ (resched || !protect_ਟੁਕੜਾ(curr)) {
		resched_curr_lazy(rq);
		clear_buddies(cfs_rq, curr);
	}
}

ਸਥਿਰ ਖਾਲੀ update_curr_fair(ਬਣਤਰ rq *rq)
{
	ਬਣਤਰ sched_entity *se = &rq->donor->se;

	for_each_sched_entity(se)
		update_curr(cfs_rq_ਦਾ(se));
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
update_stats_wait_ਸ਼ੁਰੂ_fair(ਬਣਤਰ cfs_rq *cfs_rq, ਬਣਤਰ sched_entity *se)
{
	ਬਣਤਰ sched_statistics *stats;
	ਬਣਤਰ task_struct *p = ਨਿਰਾ;

	ਜੇ (!schedstat_enabled())
		ਵਾਪਸ;

	stats = __schedstats_from_se(se);

	ਜੇ (entity_is_task(se))
		p = task_ਦਾ(se);

	__update_stats_wait_start(rq_ਦਾ(cfs_rq), p, stats);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
update_stats_wait_ਸਿਰਾ_fair(ਬਣਤਰ cfs_rq *cfs_rq, ਬਣਤਰ sched_entity *se)
{
	ਬਣਤਰ sched_statistics *stats;
	ਬਣਤਰ task_struct *p = ਨਿਰਾ;

	ਜੇ (!schedstat_enabled())
		ਵਾਪਸ;

	stats = __schedstats_from_se(se);

	/*
	 * When the sched_schedstat changes from 0 to 1, some sched se
	 * maybe already in the runqueue, the se->statistics.wait_start
	 * will be 0.So it will let the delta wrong. We need to avoid this
	 * scenario.
	 */
	ਜੇ (unlikely(!schedstat_val(stats->wait_ਸ਼ੁਰੂ)))
		ਵਾਪਸ;

	ਜੇ (entity_is_task(se))
		p = task_ਦਾ(se);

	__update_stats_wait_end(rq_ਦਾ(cfs_rq), p, stats);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
update_stats_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_sleeper_fair(ਬਣਤਰ cfs_rq *cfs_rq, ਬਣਤਰ sched_entity *se)
{
	ਬਣਤਰ sched_statistics *stats;
	ਬਣਤਰ task_struct *tsk = ਨਿਰਾ;

	ਜੇ (!schedstat_enabled())
		ਵਾਪਸ;

	stats = __schedstats_from_se(se);

	ਜੇ (entity_is_task(se))
		tsk = task_ਦਾ(se);

	__update_stats_enqueue_sleeper(rq_ਦਾ(cfs_rq), tsk, stats);
}

/*
 * Task is being enqueued - update stats:
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
update_stats_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_fair(ਬਣਤਰ cfs_rq *cfs_rq, ਬਣਤਰ sched_entity *se, ਪੂਰਨ flags)
{
	ਜੇ (!schedstat_enabled())
		ਵਾਪਸ;

	/*
	 * Are we enqueueing a waiting task? (for current tasks
	 * a dequeue/enqueue event is a NOP)
	 */
	ਜੇ (se != cfs_rq->h_curr)
		update_stats_wait_ਸ਼ੁਰੂ_fair(cfs_rq, se);

	ਜੇ (flags & ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_WAKEUP)
		update_stats_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_sleeper_fair(cfs_rq, se);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
update_stats_ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_fair(ਬਣਤਰ cfs_rq *cfs_rq, ਬਣਤਰ sched_entity *se, ਪੂਰਨ flags)
{

	ਜੇ (!schedstat_enabled())
		ਵਾਪਸ;

	/*
	 * Mark the end of the wait period if dequeueing a
	 * waiting task:
	 */
	ਜੇ (se != cfs_rq->h_curr)
		update_stats_wait_ਸਿਰਾ_fair(cfs_rq, se);

	ਜੇ ((flags & ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_SLEEP) && entity_is_task(se)) {
		ਬਣਤਰ task_struct *tsk = task_ਦਾ(se);
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ state;

		/* XXX racy against TTWU */
		state = READ_ONCE(tsk->__state);
		ਜੇ (state & TASK_INTERRUPTIBLE)
			__schedstat_set(tsk->stats.sleep_ਸ਼ੁਰੂ,
				      rq_clock(rq_ਦਾ(cfs_rq)));
		ਜੇ (state & TASK_UNINTERRUPTIBLE)
			__schedstat_set(tsk->stats.ਖੰਡ_ਸ਼ੁਰੂ,
				      rq_clock(rq_ਦਾ(cfs_rq)));
	}
}

/*
 * We are picking a new current task - update its stats:
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
update_stats_curr_ਸ਼ੁਰੂ(ਬਣਤਰ cfs_rq *cfs_rq, ਬਣਤਰ sched_entity *se)
{
	/*
	 * We are starting a new run period:
	 */
	se->exec_ਸ਼ੁਰੂ = rq_clock_task(rq_ਦਾ(cfs_rq));
}

/* Check sched_smt_active before calling this to avoid overheads in fastpaths */
ਸਥਿਰ ਸਤਰਅੰਦਰ bool is_core_idle(ਪੂਰਨ cpu)
{
	ਪੂਰਨ sibling;

	for_each_cpu(sibling, cpu_smt_ਨਕਾਬ(cpu)) {
		ਜੇ (cpu == sibling)
			ਜਾਰੀ;

		ਜੇ (!idle_cpu(sibling))
			ਵਾਪਸ false;
	}

	ਵਾਪਸ true;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_NUMA
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NUMA_IMBALANCE_MIN 2

ਸਥਿਰ ਸਤਰਅੰਦਰ ਲੰਮਾ
adjust_numa_imbalance(ਪੂਰਨ imbalance, ਪੂਰਨ dst_running, ਪੂਰਨ imb_numa_nr)
{
	/*
	 * Allow a NUMA imbalance if busy CPUs is less than the maximum
	 * threshold. Above this threshold, individual tasks may be contending
	 * for both memory bandwidth and any shared HT resources.  This is an
	 * approximation as the number of running tasks may not be related to
	 * the number of busy CPUs due to sched_setaffinity.
	 */
	ਜੇ (dst_running > imb_numa_nr)
		ਵਾਪਸ imbalance;

	/*
	 * Allow a small imbalance based on a simple pair of communicating
	 * tasks that remain local when the destination is lightly loaded.
	 */
	ਜੇ (imbalance <= NUMA_IMBALANCE_MIN)
		ਵਾਪਸ 0;

	ਵਾਪਸ imbalance;
}
#ਜੇ_ਅੰਤ /* CONFIG_NUMA */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_NUMA_BALANCING
/*
 * Approximate time to scan a full NUMA task in ms. The task scan period is
 * calculated based on the tasks virtual memory size and
 * numa_balancing_scan_size.
 */
ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ sysctl_numa_balancing_ਜਾਂਚ_period_min = 1000;
ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ sysctl_numa_balancing_ਜਾਂਚ_period_max = 60000;

/* Portion of address space to scan in MB */
ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ sysctl_numa_balancing_ਜਾਂਚ_ਮਾਪ = 256;

/* Scan @scan_size MB every @scan_period after an initial @scan_delay in ms */
ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ sysctl_numa_balancing_ਜਾਂਚ_delay = 1000;

/* The page with hint page fault latency < threshold in ms is considered hot */
ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ sysctl_numa_balancing_hot_ਹੱਦ = MSEC_ਪ੍ਰਤੀ_SEC;

ਬਣਤਰ numa_ਸਮੂਹ {
	refcount_t refcount;

	spinlock_t ਤਾਲਾ; /* nr_tasks, tasks */
	ਪੂਰਨ nr_tasks;
	pid_t gid;
	ਪੂਰਨ ਸਰਗਰਮ_nodes;

	ਬਣਤਰ rcu_ਸਿਰ rcu;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਕੁੱਲ_ਨੁਕਸ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ max_ਨੁਕਸ_cpu;
	/*
	 * faults[] array is split into two regions: faults_mem and faults_cpu.
	 *
	 * Faults_cpu is used to decide whether memory should move
	 * towards the CPU. As a consequence, these stats are weighted
	 * more by CPU use than by memory faults.
	 */
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਨੁਕਸ[];
};

/*
 * For functions that can be called in multiple contexts that permit reading
 * ->numa_group (see struct task_struct for locking rules).
 */
ਸਥਿਰ ਬਣਤਰ numa_ਸਮੂਹ *deref_task_numa_ਸਮੂਹ(ਬਣਤਰ task_struct *p)
{
	ਵਾਪਸ rcu_dereference_check(p->numa_ਸਮੂਹ, p == current ||
		(lockdep_is_held(__rq_lockp(task_rq(p))) && !READ_ONCE(p->on_cpu)));
}

ਸਥਿਰ ਬਣਤਰ numa_ਸਮੂਹ *deref_curr_numa_ਸਮੂਹ(ਬਣਤਰ task_struct *p)
{
	ਵਾਪਸ rcu_dereference_protected(p->numa_ਸਮੂਹ, p == current);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਸਮੂਹ_ਨੁਕਸ_priv(ਬਣਤਰ numa_ਸਮੂਹ *ng);
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਸਮੂਹ_ਨੁਕਸ_shared(ਬਣਤਰ numa_ਸਮੂਹ *ng);

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ task_nr_ਜਾਂਚ_windows(ਬਣਤਰ task_struct *p)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ rss = 0;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ nr_ਜਾਂਚ_ਪੰਨੇ;

	/*
	 * Calculations based on RSS as non-present and empty pages are skipped
	 * by the PTE scanner and NUMA hinting faults should be trapped based
	 * on resident pages
	 */
	nr_ਜਾਂਚ_ਪੰਨੇ = MB_ਮੰਜ਼ਿਲ_ਪੰਨੇ(sysctl_numa_balancing_ਜਾਂਚ_ਮਾਪ);
	rss = get_mm_rss(p->mm);
	ਜੇ (!rss)
		rss = nr_ਜਾਂਚ_ਪੰਨੇ;

	rss = ਗੇੜਾ_ਉੱਪਰ(rss, nr_ਜਾਂਚ_ਪੰਨੇ);
	ਵਾਪਸ rss / nr_ਜਾਂਚ_ਪੰਨੇ;
}

/* For sanity's sake, never scan more PTEs than MAX_SCAN_WINDOW MB/sec. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MAX_ਜਾਂਚ_ਖਿੜਕੀ 2560

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ task_ਜਾਂਚ_min(ਬਣਤਰ task_struct *p)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਜਾਂਚ_ਮਾਪ = READ_ONCE(sysctl_numa_balancing_ਜਾਂਚ_ਮਾਪ);
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਜਾਂਚ, floor;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ windows = 1;

	ਜੇ (ਜਾਂਚ_ਮਾਪ < MAX_ਜਾਂਚ_ਖਿੜਕੀ)
		windows = MAX_ਜਾਂਚ_ਖਿੜਕੀ / ਜਾਂਚ_ਮਾਪ;
	floor = 1000 / windows;

	ਜਾਂਚ = sysctl_numa_balancing_ਜਾਂਚ_period_min / task_nr_ਜਾਂਚ_windows(p);
	ਵਾਪਸ max_t(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ, floor, ਜਾਂਚ);
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ task_ਜਾਂਚ_ਸ਼ੁਰੂ(ਬਣਤਰ task_struct *p)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ smin = task_ਜਾਂਚ_min(p);
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ period = smin;
	ਬਣਤਰ numa_ਸਮੂਹ *ng;

	/* Scale the maximum scan period with the amount of shared memory. */
	rcu_read_ਤਾਲਾ();
	ng = rcu_dereference_all(p->numa_ਸਮੂਹ);
	ਜੇ (ng) {
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ shared = ਸਮੂਹ_ਨੁਕਸ_shared(ng);
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਨਿਜੀ = ਸਮੂਹ_ਨੁਕਸ_priv(ng);

		period *= refcount_read(&ng->refcount);
		period *= shared + 1;
		period /= ਨਿਜੀ + shared + 1;
	}
	rcu_read_unlock();

	ਵਾਪਸ max(smin, period);
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ task_ਜਾਂਚ_max(ਬਣਤਰ task_struct *p)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ smin = task_ਜਾਂਚ_min(p);
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ smax;
	ਬਣਤਰ numa_ਸਮੂਹ *ng;

	/* Watch for min being lower than max due to floor calculations */
	smax = sysctl_numa_balancing_ਜਾਂਚ_period_max / task_nr_ਜਾਂਚ_windows(p);

	/* Scale the maximum scan period with the amount of shared memory. */
	ng = deref_curr_numa_ਸਮੂਹ(p);
	ਜੇ (ng) {
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ shared = ਸਮੂਹ_ਨੁਕਸ_shared(ng);
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਨਿਜੀ = ਸਮੂਹ_ਨੁਕਸ_priv(ng);
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ period = smax;

		period *= refcount_read(&ng->refcount);
		period *= shared + 1;
		period /= ਨਿਜੀ + shared + 1;

		smax = max(smax, period);
	}

	ਵਾਪਸ max(smin, smax);
}

ਸਥਿਰ ਖਾਲੀ account_numa_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p)
{
	rq->nr_numa_running += (p->numa_preferred_nid != NUMA_NO_ਗੰਢ);
	rq->nr_preferred_running += (p->numa_preferred_nid == task_ਗੰਢ(p));
}

ਸਥਿਰ ਖਾਲੀ account_numa_ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p)
{
	rq->nr_numa_running -= (p->numa_preferred_nid != NUMA_NO_ਗੰਢ);
	rq->nr_preferred_running -= (p->numa_preferred_nid == task_ਗੰਢ(p));
}

/* Shared or private faults. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NR_NUMA_HINT_FAULT_TYPES 2

/* Memory and CPU locality */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NR_NUMA_HINT_FAULT_STATS (NR_NUMA_HINT_FAULT_TYPES * 2)

/* Averaged statistics, and temporary buffers. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NR_NUMA_HINT_FAULT_BUCKETS (NR_NUMA_HINT_FAULT_STATS * 2)

pid_t task_numa_ਸਮੂਹ_id(ਬਣਤਰ task_struct *p)
{
	ਬਣਤਰ numa_ਸਮੂਹ *ng;
	pid_t gid = 0;

	rcu_read_ਤਾਲਾ();
	ng = rcu_dereference_all(p->numa_ਸਮੂਹ);
	ਜੇ (ng)
		gid = ng->gid;
	rcu_read_unlock();

	ਵਾਪਸ gid;
}

/*
 * The averaged statistics, shared & private, memory & CPU,
 * occupy the first half of the array. The second half of the
 * array is for current counters, which are averaged into the
 * first set by task_numa_placement.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ task_ਨੁਕਸ_idx(ਗਿਣਤੀ numa_ਨੁਕਸ_stats s, ਪੂਰਨ nid, ਪੂਰਨ priv)
{
	ਵਾਪਸ NR_NUMA_HINT_FAULT_TYPES * (s * nr_ਗੰਢ_ids + nid) + priv;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ task_ਨੁਕਸ(ਬਣਤਰ task_struct *p, ਪੂਰਨ nid)
{
	ਜੇ (!p->numa_ਨੁਕਸ)
		ਵਾਪਸ 0;

	ਵਾਪਸ p->numa_ਨੁਕਸ[task_ਨੁਕਸ_idx(NUMA_MEM, nid, 0)] +
		p->numa_ਨੁਕਸ[task_ਨੁਕਸ_idx(NUMA_MEM, nid, 1)];
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਸਮੂਹ_ਨੁਕਸ(ਬਣਤਰ task_struct *p, ਪੂਰਨ nid)
{
	ਬਣਤਰ numa_ਸਮੂਹ *ng = deref_task_numa_ਸਮੂਹ(p);

	ਜੇ (!ng)
		ਵਾਪਸ 0;

	ਵਾਪਸ ng->ਨੁਕਸ[task_ਨੁਕਸ_idx(NUMA_MEM, nid, 0)] +
		ng->ਨੁਕਸ[task_ਨੁਕਸ_idx(NUMA_MEM, nid, 1)];
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਸਮੂਹ_ਨੁਕਸ_cpu(ਬਣਤਰ numa_ਸਮੂਹ *ਸਮੂਹ, ਪੂਰਨ nid)
{
	ਵਾਪਸ ਸਮੂਹ->ਨੁਕਸ[task_ਨੁਕਸ_idx(NUMA_CPU, nid, 0)] +
		ਸਮੂਹ->ਨੁਕਸ[task_ਨੁਕਸ_idx(NUMA_CPU, nid, 1)];
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਸਮੂਹ_ਨੁਕਸ_priv(ਬਣਤਰ numa_ਸਮੂਹ *ng)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਨੁਕਸ = 0;
	ਪੂਰਨ ਗੰਢ;

	for_each_online_ਗੰਢ(ਗੰਢ) {
		ਨੁਕਸ += ng->ਨੁਕਸ[task_ਨੁਕਸ_idx(NUMA_MEM, ਗੰਢ, 1)];
	}

	ਵਾਪਸ ਨੁਕਸ;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਸਮੂਹ_ਨੁਕਸ_shared(ਬਣਤਰ numa_ਸਮੂਹ *ng)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਨੁਕਸ = 0;
	ਪੂਰਨ ਗੰਢ;

	for_each_online_ਗੰਢ(ਗੰਢ) {
		ਨੁਕਸ += ng->ਨੁਕਸ[task_ਨੁਕਸ_idx(NUMA_MEM, ਗੰਢ, 0)];
	}

	ਵਾਪਸ ਨੁਕਸ;
}

/*
 * A node triggering more than 1/3 as many NUMA faults as the maximum is
 * considered part of a numa group's pseudo-interleaving set. Migrations
 * between these nodes are slowed down, to allow things to settle down.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਸਰਗਰਮ_ਗੰਢ_FRACTION 3

ਸਥਿਰ bool numa_is_ਸਰਗਰਮ_ਗੰਢ(ਪੂਰਨ nid, ਬਣਤਰ numa_ਸਮੂਹ *ng)
{
	ਵਾਪਸ ਸਮੂਹ_ਨੁਕਸ_cpu(ng, nid) * ਸਰਗਰਮ_ਗੰਢ_FRACTION > ng->max_ਨੁਕਸ_cpu;
}

/* Handle placement on systems where not all nodes are directly connected. */
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ score_nearby_nodes(ਬਣਤਰ task_struct *p, ਪੂਰਨ nid,
					ਪੂਰਨ lim_dist, bool task)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ score = 0;
	ਪੂਰਨ ਗੰਢ, max_dist;

	/*
	 * All nodes are directly connected, and the same distance
	 * from each other. No need for fancy placement algorithms.
	 */
	ਜੇ (sched_numa_topology_type == NUMA_ਸਿੱਧਾ)
		ਵਾਪਸ 0;

	/* sched_max_numa_distance may be changed in parallel. */
	max_dist = READ_ONCE(sched_max_numa_ਦੂਰੀ);
	/*
	 * This code is called for each node, introducing N^2 complexity,
	 * which should be OK given the number of nodes rarely exceeds 8.
	 */
	for_each_online_ਗੰਢ(ਗੰਢ) {
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਨੁਕਸ;
		ਪੂਰਨ dist = ਗੰਢ_ਦੂਰੀ(nid, ਗੰਢ);

		/*
		 * The furthest away nodes in the system are not interesting
		 * for placement; nid was already counted.
		 */
		ਜੇ (dist >= max_dist || ਗੰਢ == nid)
			ਜਾਰੀ;

		/*
		 * On systems with a backplane NUMA topology, compare groups
		 * of nodes, and move tasks towards the group with the most
		 * memory accesses. When comparing two nodes at distance
		 * "hoplimit", only nodes closer by than "hoplimit" are part
		 * of each group. Skip other nodes.
		 */
		ਜੇ (sched_numa_topology_type == NUMA_BACKPLANE && dist >= lim_dist)
			ਜਾਰੀ;

		/* Add up the faults from nearby nodes. */
		ਜੇ (task)
			ਨੁਕਸ = task_ਨੁਕਸ(p, ਗੰਢ);
		ਨਹੀਂਤਾਂ
			ਨੁਕਸ = ਸਮੂਹ_ਨੁਕਸ(p, ਗੰਢ);

		/*
		 * On systems with a glueless mesh NUMA topology, there are
		 * no fixed "groups of nodes". Instead, nodes that are not
		 * directly connected bounce traffic through intermediate
		 * nodes; a numa_group can occupy any set of nodes.
		 * The further away a node is, the less the faults count.
		 * This seems to result in good task placement.
		 */
		ਜੇ (sched_numa_topology_type == NUMA_GLUELESS_MESH) {
			ਨੁਕਸ *= (max_dist - dist);
			ਨੁਕਸ /= (max_dist - LOCAL_ਦੂਰੀ);
		}

		score += ਨੁਕਸ;
	}

	ਵਾਪਸ score;
}

/*
 * These return the fraction of accesses done by a particular task, or
 * task group, on a particular numa node.  The group weight is given a
 * larger multiplier, in order to group tasks together that are almost
 * evenly spread out between numa nodes.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ task_ਭਾਰ(ਬਣਤਰ task_struct *p, ਪੂਰਨ nid,
					ਪੂਰਨ dist)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਨੁਕਸ, ਕੁੱਲ_ਨੁਕਸ;

	ਜੇ (!p->numa_ਨੁਕਸ)
		ਵਾਪਸ 0;

	ਕੁੱਲ_ਨੁਕਸ = p->ਕੁੱਲ_numa_ਨੁਕਸ;

	ਜੇ (!ਕੁੱਲ_ਨੁਕਸ)
		ਵਾਪਸ 0;

	ਨੁਕਸ = task_ਨੁਕਸ(p, nid);
	ਨੁਕਸ += score_nearby_nodes(p, nid, dist, true);

	ਵਾਪਸ 1000 * ਨੁਕਸ / ਕੁੱਲ_ਨੁਕਸ;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਸਮੂਹ_ਭਾਰ(ਬਣਤਰ task_struct *p, ਪੂਰਨ nid,
					 ਪੂਰਨ dist)
{
	ਬਣਤਰ numa_ਸਮੂਹ *ng = deref_task_numa_ਸਮੂਹ(p);
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਨੁਕਸ, ਕੁੱਲ_ਨੁਕਸ;

	ਜੇ (!ng)
		ਵਾਪਸ 0;

	ਕੁੱਲ_ਨੁਕਸ = ng->ਕੁੱਲ_ਨੁਕਸ;

	ਜੇ (!ਕੁੱਲ_ਨੁਕਸ)
		ਵਾਪਸ 0;

	ਨੁਕਸ = ਸਮੂਹ_ਨੁਕਸ(p, nid);
	ਨੁਕਸ += score_nearby_nodes(p, nid, dist, false);

	ਵਾਪਸ 1000 * ਨੁਕਸ / ਕੁੱਲ_ਨੁਕਸ;
}

/*
 * If memory tiering mode is enabled, cpupid of slow memory page is
 * used to record scan time instead of CPU and PID.  When tiering mode
 * is disabled at run time, the scan time (in cpupid) will be
 * interpreted as CPU and PID.  So CPU needs to be checked to avoid to
 * access out of array bound.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ bool cpupid_valid(ਪੂਰਨ cpupid)
{
	ਵਾਪਸ cpupid_ਮੰਜ਼ਿਲ_cpu(cpupid) < nr_cpu_ids;
}

/*
 * For memory tiering mode, if there are enough free pages (more than
 * enough watermark defined here) in fast memory node, to take full
 * advantage of fast memory capacity, all recently accessed slow
 * memory pages will be migrated to fast memory node without
 * considering hot threshold.
 */
ਸਥਿਰ bool pgdat_free_space_enough(ਬਣਤਰ pglist_ਅੰਕੜੇ *pgdat)
{
	ਪੂਰਨ z;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ enough_wmark;

	enough_wmark = max(1UL * 1024 * 1024 * 1024 >> ਪੰਨਾ_ਖਿਸਕਾਓ,
			   pgdat->ਗੰਢ_present_ਪੰਨੇ >> 4);
	ਲਈ (z = pgdat->nr_zones - 1; z >= 0; z--) {
		ਬਣਤਰ zone *zone = pgdat->ਗੰਢ_zones + z;

		ਜੇ (!populated_zone(zone))
			ਜਾਰੀ;

		ਜੇ (zone_watermark_ok(zone, 0,
				      promo_wmark_ਪੰਨੇ(zone) + enough_wmark,
				      ZONE_MOVABLE, 0))
			ਵਾਪਸ true;
	}
	ਵਾਪਸ false;
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
ਸਥਿਰ ਪੂਰਨ numa_hint_fault_latency(ਬਣਤਰ folio *folio)
{
	ਪੂਰਨ ਆਖਰੀ_ਸਮਾਂ, ਸਮਾਂ;

	ਸਮਾਂ = jiffies_ਮੰਜ਼ਿਲ_msecs(jiffies);
	ਆਖਰੀ_ਸਮਾਂ = folio_xchg_access_ਸਮਾਂ(folio, ਸਮਾਂ);

	ਵਾਪਸ (ਸਮਾਂ - ਆਖਰੀ_ਸਮਾਂ) & ਪੰਨਾ_ACCESS_ਸਮਾਂ_ਨਕਾਬ;
}

/*
 * For memory tiering mode, too high promotion/demotion throughput may
 * hurt application latency.  So we provide a mechanism to rate limit
 * the number of pages that are tried to be promoted.
 */
ਸਥਿਰ bool numa_promotion_ਦਰ_ਹੱਦਬੰਦੀ(ਬਣਤਰ pglist_ਅੰਕੜੇ *pgdat,
				      ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਦਰ_ਹੱਦਬੰਦੀ, ਪੂਰਨ nr)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ nr_cand;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ now, ਸ਼ੁਰੂ;

	now = jiffies_ਮੰਜ਼ਿਲ_msecs(jiffies);
	mod_ਗੰਢ_ਪੰਨਾ_state(pgdat, PGPROMOTE_CANDIDATE, nr);
	nr_cand = ਗੰਢ_ਪੰਨਾ_state(pgdat, PGPROMOTE_CANDIDATE);
	ਸ਼ੁਰੂ = pgdat->nbp_rl_ਸ਼ੁਰੂ;
	ਜੇ (now - ਸ਼ੁਰੂ > MSEC_ਪ੍ਰਤੀ_SEC &&
	    cmpxchg(&pgdat->nbp_rl_ਸ਼ੁਰੂ, ਸ਼ੁਰੂ, now) == ਸ਼ੁਰੂ)
		pgdat->nbp_rl_nr_cand = nr_cand;
	ਜੇ (nr_cand - pgdat->nbp_rl_nr_cand >= ਦਰ_ਹੱਦਬੰਦੀ)
		ਵਾਪਸ true;
	ਵਾਪਸ false;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NUMA_MIGRATION_ADJUST_ਕਦਮਾਂ	16

ਸਥਿਰ ਖਾਲੀ numa_promotion_adjust_ਹੱਦ(ਬਣਤਰ pglist_ਅੰਕੜੇ *pgdat,
					    ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਦਰ_ਹੱਦਬੰਦੀ,
					    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ref_th)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ now, ਸ਼ੁਰੂ, th_period, unit_th, th;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ nr_cand, ref_cand, diff_cand;

	now = jiffies_ਮੰਜ਼ਿਲ_msecs(jiffies);
	th_period = sysctl_numa_balancing_ਜਾਂਚ_period_max;
	ਸ਼ੁਰੂ = pgdat->nbp_th_ਸ਼ੁਰੂ;
	ਜੇ (now - ਸ਼ੁਰੂ > th_period &&
	    cmpxchg(&pgdat->nbp_th_ਸ਼ੁਰੂ, ਸ਼ੁਰੂ, now) == ਸ਼ੁਰੂ) {
		ref_cand = ਦਰ_ਹੱਦਬੰਦੀ *
			sysctl_numa_balancing_ਜਾਂਚ_period_max / MSEC_ਪ੍ਰਤੀ_SEC;
		nr_cand = ਗੰਢ_ਪੰਨਾ_state(pgdat, PGPROMOTE_CANDIDATE);
		diff_cand = nr_cand - pgdat->nbp_th_nr_cand;
		unit_th = ref_th * 2 / NUMA_MIGRATION_ADJUST_ਕਦਮਾਂ;
		th = pgdat->nbp_ਹੱਦ ? : ref_th;
		ਜੇ (diff_cand > ref_cand * 11 / 10)
			th = max(th - unit_th, unit_th);
		ਨਹੀਂਤਾਂ ਜੇ (diff_cand < ref_cand * 9 / 10)
			th = min(th + unit_th, ref_th * 2);
		pgdat->nbp_th_nr_cand = nr_cand;
		pgdat->nbp_ਹੱਦ = th;
	}
}

bool should_numa_migrate_ਸਮ੍ਰਿਤੀ(ਬਣਤਰ task_struct *p, ਬਣਤਰ folio *folio,
				ਪੂਰਨ src_nid, ਪੂਰਨ dst_cpu)
{
	ਬਣਤਰ numa_ਸਮੂਹ *ng = deref_curr_numa_ਸਮੂਹ(p);
	ਪੂਰਨ dst_nid = cpu_ਮੰਜ਼ਿਲ_ਗੰਢ(dst_cpu);
	ਪੂਰਨ ਆਖਰੀ_cpupid, this_cpupid;

	/*
	 * Cannot migrate to memoryless nodes.
	 */
	ਜੇ (!ਗੰਢ_state(dst_nid, N_ਸਮ੍ਰਿਤੀ))
		ਵਾਪਸ false;

	/*
	 * The pages in slow memory node should be migrated according
	 * to hot/cold instead of private/shared.
	 */
	ਜੇ (folio_use_access_ਸਮਾਂ(folio)) {
		ਬਣਤਰ pglist_ਅੰਕੜੇ *pgdat;
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਦਰ_ਹੱਦਬੰਦੀ;
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ latency, th, def_th;
		ਲੰਮਾ nr = folio_nr_ਪੰਨੇ(folio);

		pgdat = ਗੰਢ_ਅੰਕੜੇ(dst_nid);
		ਜੇ (pgdat_free_space_enough(pgdat)) {
			/* workload changed, reset hot threshold */
			pgdat->nbp_ਹੱਦ = 0;
			mod_ਗੰਢ_ਪੰਨਾ_state(pgdat, PGPROMOTE_CANDIDATE_NRL, nr);
			ਵਾਪਸ true;
		}

		def_th = sysctl_numa_balancing_hot_ਹੱਦ;
		ਦਰ_ਹੱਦਬੰਦੀ = MB_ਮੰਜ਼ਿਲ_ਪੰਨੇ(sysctl_numa_balancing_promote_ਦਰ_ਹੱਦਬੰਦੀ);
		numa_promotion_adjust_ਹੱਦ(pgdat, ਦਰ_ਹੱਦਬੰਦੀ, def_th);

		th = pgdat->nbp_ਹੱਦ ? : def_th;
		latency = numa_hint_fault_latency(folio);
		ਜੇ (latency >= th)
			ਵਾਪਸ false;

		ਵਾਪਸ !numa_promotion_ਦਰ_ਹੱਦਬੰਦੀ(pgdat, ਦਰ_ਹੱਦਬੰਦੀ, nr);
	}

	this_cpupid = cpu_pid_ਮੰਜ਼ਿਲ_cpupid(dst_cpu, current->pid);
	ਆਖਰੀ_cpupid = folio_xchg_ਆਖਰੀ_cpupid(folio, this_cpupid);

	ਜੇ (!(sysctl_numa_balancing_ਬਹੁਲਕ & NUMA_BALANCING_ਸਮ੍ਰਿਤੀ_TIERING) &&
	    !ਗੰਢ_is_toptier(src_nid) && !cpupid_valid(ਆਖਰੀ_cpupid))
		ਵਾਪਸ false;

	/*
	 * Allow first faults or private faults to migrate immediately early in
	 * the lifetime of a task. The magic number 4 is based on waiting for
	 * two full passes of the "multi-stage node selection" test that is
	 * executed below.
	 */
	ਜੇ ((p->numa_preferred_nid == NUMA_NO_ਗੰਢ || p->numa_ਜਾਂਚ_seq <= 4) &&
	    (cpupid_pid_unset(ਆਖਰੀ_cpupid) || cpupid_ਮੇਲ_pid(p, ਆਖਰੀ_cpupid)))
		ਵਾਪਸ true;

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
	ਜੇ (!cpupid_pid_unset(ਆਖਰੀ_cpupid) &&
				cpupid_ਮੰਜ਼ਿਲ_nid(ਆਖਰੀ_cpupid) != dst_nid)
		ਵਾਪਸ false;

	/* Always allow migrate on private faults */
	ਜੇ (cpupid_ਮੇਲ_pid(p, ਆਖਰੀ_cpupid))
		ਵਾਪਸ true;

	/* A shared fault, but p->numa_group has not been set up yet. */
	ਜੇ (!ng)
		ਵਾਪਸ true;

	/*
	 * Destination node is much more heavily used than the source
	 * node? Allow migration.
	 */
	ਜੇ (ਸਮੂਹ_ਨੁਕਸ_cpu(ng, dst_nid) > ਸਮੂਹ_ਨੁਕਸ_cpu(ng, src_nid) *
					ਸਰਗਰਮ_ਗੰਢ_FRACTION)
		ਵਾਪਸ true;

	/*
	 * Distribute memory according to CPU & memory use on each node,
	 * with 3/4 hysteresis to avoid unnecessary memory migrations:
	 *
	 * faults_cpu(dst)   3   faults_cpu(src)
	 * --------------- * - > ---------------
	 * faults_mem(dst)   4   faults_mem(src)
	 */
	ਵਾਪਸ ਸਮੂਹ_ਨੁਕਸ_cpu(ng, dst_nid) * ਸਮੂਹ_ਨੁਕਸ(p, src_nid) * 3 >
	       ਸਮੂਹ_ਨੁਕਸ_cpu(ng, src_nid) * ਸਮੂਹ_ਨੁਕਸ(p, dst_nid) * 4;
}

/*
 * 'numa_type' describes the node at the moment of load balancing.
 */
ਗਿਣਤੀ numa_type {
	/* The node has spare capacity that can be used to run more tasks.  */
	ਗੰਢ_has_ਵਾਧੂ = 0,
	/*
	 * The node is fully used and the tasks don't compete for more CPU
	 * cycles. Nevertheless, some tasks might wait before running.
	 */
	ਗੰਢ_fully_busy,
	/*
	 * The node is overloaded and can't provide expected CPU cycles to all
	 * tasks.
	 */
	ਗੰਢ_overloaded
};

/* Cached statistics for all CPUs within a node */
ਬਣਤਰ numa_stats {
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੱਦੋ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ runnable;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ util;
	/* Total compute capacity of CPUs on a node */
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ compute_ਸਮਰੱਥਾ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ nr_running;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਭਾਰ;
	ਗਿਣਤੀ numa_type ਗੰਢ_type;
	ਪੂਰਨ idle_cpu;
};

ਬਣਤਰ task_numa_env {
	ਬਣਤਰ task_struct *p;

	ਪੂਰਨ src_cpu, src_nid;
	ਪੂਰਨ dst_cpu, dst_nid;
	ਪੂਰਨ imb_numa_nr;

	ਬਣਤਰ numa_stats src_stats, dst_stats;

	ਪੂਰਨ imbalance_pct;
	ਪੂਰਨ dist;

	ਬਣਤਰ task_struct *ਉੱਤਮ_task;
	ਲੰਮਾ ਉੱਤਮ_imp;
	ਪੂਰਨ ਉੱਤਮ_cpu;
};

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ cpu_ਲੱਦੋ(ਬਣਤਰ rq *rq);
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ cpu_runnable(ਬਣਤਰ rq *rq);

ਸਥਿਰ ਸਤਰਅੰਦਰ ਗਿਣਤੀ
numa_type numa_classify(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ imbalance_pct,
			 ਬਣਤਰ numa_stats *ns)
{
	ਜੇ ((ns->nr_running > ns->ਭਾਰ) &&
	    (((ns->compute_ਸਮਰੱਥਾ * 100) < (ns->util * imbalance_pct)) ||
	     ((ns->compute_ਸਮਰੱਥਾ * imbalance_pct) < (ns->runnable * 100))))
		ਵਾਪਸ ਗੰਢ_overloaded;

	ਜੇ ((ns->nr_running < ns->ਭਾਰ) ||
	    (((ns->compute_ਸਮਰੱਥਾ * 100) > (ns->util * imbalance_pct)) &&
	     ((ns->compute_ਸਮਰੱਥਾ * imbalance_pct) > (ns->runnable * 100))))
		ਵਾਪਸ ਗੰਢ_has_ਵਾਧੂ;

	ਵਾਪਸ ਗੰਢ_fully_busy;
}

/* Forward declarations of select_idle_sibling helpers */
ਸਥਿਰ ਸਤਰਅੰਦਰ bool test_idle_cores(ਪੂਰਨ cpu);
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ numa_idle_core(ਪੂਰਨ idle_core, ਪੂਰਨ cpu)
{
	ਜੇ (!sched_smt_ਸਰਗਰਮ() ||
	    idle_core >= 0 || !test_idle_cores(cpu))
		ਵਾਪਸ idle_core;

	/*
	 * Prefer cores instead of packing HT siblings
	 * and triggering future load balancing.
	 */
	ਜੇ (is_core_idle(cpu))
		idle_core = cpu;

	ਵਾਪਸ idle_core;
}

/*
 * Gather all necessary information to make NUMA balancing placement
 * decisions that are compatible with standard load balancer. This
 * borrows code and logic from update_sg_lb_stats but sharing a
 * common implementation is impractical.
 */
ਸਥਿਰ ਖਾਲੀ update_numa_stats(ਬਣਤਰ task_numa_env *env,
			      ਬਣਤਰ numa_stats *ns, ਪੂਰਨ nid,
			      bool ਲੱਭੋ_idle)
{
	ਪੂਰਨ cpu, idle_core = -1;

	memset(ns, 0, ਆਕਾਰ(*ns));
	ns->idle_cpu = -1;

	rcu_read_ਤਾਲਾ();
	for_each_cpu(cpu, cpumask_ਦਾ_ਗੰਢ(nid)) {
		ਬਣਤਰ rq *rq = cpu_rq(cpu);

		ns->ਲੱਦੋ += cpu_ਲੱਦੋ(rq);
		ns->runnable += cpu_runnable(rq);
		ns->util += cpu_util_cfs(cpu);
		ns->nr_running += rq->cfs.h_nr_runnable;
		ns->compute_ਸਮਰੱਥਾ += ਸਮਰੱਥਾ_ਦਾ(cpu);

		ਜੇ (ਲੱਭੋ_idle && idle_core < 0 && !rq->nr_running && idle_cpu(cpu)) {
			ਜੇ (READ_ONCE(rq->numa_migrate_on) ||
			    !cpumask_test_cpu(cpu, env->p->cpus_ptr))
				ਜਾਰੀ;

			ਜੇ (ns->idle_cpu == -1)
				ns->idle_cpu = cpu;

			idle_core = numa_idle_core(idle_core, cpu);
		}
	}
	rcu_read_unlock();

	ns->ਭਾਰ = cpumask_ਭਾਰ(cpumask_ਦਾ_ਗੰਢ(nid));

	ns->ਗੰਢ_type = numa_classify(env->imbalance_pct, ns);

	ਜੇ (idle_core >= 0)
		ns->idle_cpu = idle_core;
}

ਸਥਿਰ ਖਾਲੀ task_numa_ਸੌਂਪੋ(ਬਣਤਰ task_numa_env *env,
			     ਬਣਤਰ task_struct *p, ਲੰਮਾ imp)
{
	ਬਣਤਰ rq *rq = cpu_rq(env->dst_cpu);

	/* Check if run-queue part of active NUMA balance. */
	ਜੇ (env->ਉੱਤਮ_cpu != env->dst_cpu && xchg(&rq->numa_migrate_on, 1)) {
		ਪੂਰਨ cpu;
		ਪੂਰਨ ਸ਼ੁਰੂ = env->dst_cpu;

		/* Find alternative idle CPU. */
		for_each_cpu_wrap(cpu, cpumask_ਦਾ_ਗੰਢ(env->dst_nid), ਸ਼ੁਰੂ + 1) {
			ਜੇ (cpu == env->ਉੱਤਮ_cpu || !idle_cpu(cpu) ||
			    !cpumask_test_cpu(cpu, env->p->cpus_ptr)) {
				ਜਾਰੀ;
			}

			env->dst_cpu = cpu;
			rq = cpu_rq(env->dst_cpu);
			ਜੇ (!xchg(&rq->numa_migrate_on, 1))
				ਜਾਓ ਸੌਂਪੋ;
		}

		/* Failed to find an alternative idle CPU */
		ਵਾਪਸ;
	}

ਸੌਂਪੋ:
	/*
	 * Clear previous best_cpu/rq numa-migrate flag, since task now
	 * found a better CPU to move/swap.
	 */
	ਜੇ (env->ਉੱਤਮ_cpu != -1 && env->ਉੱਤਮ_cpu != env->dst_cpu) {
		rq = cpu_rq(env->ਉੱਤਮ_cpu);
		WRITE_ONCE(rq->numa_migrate_on, 0);
	}

	ਜੇ (env->ਉੱਤਮ_task)
		put_task_struct(env->ਉੱਤਮ_task);
	ਜੇ (p)
		get_task_struct(p);

	env->ਉੱਤਮ_task = p;
	env->ਉੱਤਮ_imp = imp;
	env->ਉੱਤਮ_cpu = env->dst_cpu;
}

ਸਥਿਰ bool ਲੱਦੋ_too_imbalanced(ਲੰਮਾ src_ਲੱਦੋ, ਲੰਮਾ dst_ਲੱਦੋ,
				ਬਣਤਰ task_numa_env *env)
{
	ਲੰਮਾ imb, old_imb;
	ਲੰਮਾ orig_src_ਲੱਦੋ, orig_dst_ਲੱਦੋ;
	ਲੰਮਾ src_ਸਮਰੱਥਾ, dst_ਸਮਰੱਥਾ;

	/*
	 * The load is corrected for the CPU capacity available on each node.
	 *
	 * src_load        dst_load
	 * ------------ vs ---------
	 * src_capacity    dst_capacity
	 */
	src_ਸਮਰੱਥਾ = env->src_stats.compute_ਸਮਰੱਥਾ;
	dst_ਸਮਰੱਥਾ = env->dst_stats.compute_ਸਮਰੱਥਾ;

	imb = ਨਿਰਪੇਖ(dst_ਲੱਦੋ * src_ਸਮਰੱਥਾ - src_ਲੱਦੋ * dst_ਸਮਰੱਥਾ);

	orig_src_ਲੱਦੋ = env->src_stats.ਲੱਦੋ;
	orig_dst_ਲੱਦੋ = env->dst_stats.ਲੱਦੋ;

	old_imb = ਨਿਰਪੇਖ(orig_dst_ਲੱਦੋ * src_ਸਮਰੱਥਾ - orig_src_ਲੱਦੋ * dst_ਸਮਰੱਥਾ);

	/* Would this change make things worse? */
	ਵਾਪਸ (imb > old_imb);
}

/*
 * Maximum NUMA importance can be 1998 (2*999);
 * SMALLIMP @ 30 would be close to 1998/64.
 * Used to deter task migration.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SMALLIMP	30

/*
 * This checks if the overall compute and NUMA accesses of the system would
 * be improved if the source tasks was migrated to the target dst_cpu taking
 * into account that it might be best if task running on the dst_cpu should
 * be exchanged with the source task
 */
ਸਥਿਰ bool task_numa_compare(ਬਣਤਰ task_numa_env *env,
			      ਲੰਮਾ taskimp, ਲੰਮਾ groupimp, bool maymove)
{
	ਬਣਤਰ numa_ਸਮੂਹ *cur_ng, *p_ng = deref_curr_numa_ਸਮੂਹ(env->p);
	ਬਣਤਰ rq *dst_rq = cpu_rq(env->dst_cpu);
	ਲੰਮਾ imp = p_ng ? groupimp : taskimp;
	ਬਣਤਰ task_struct *cur;
	ਲੰਮਾ src_ਲੱਦੋ, dst_ਲੱਦੋ;
	ਪੂਰਨ dist = env->dist;
	ਲੰਮਾ moveimp = imp;
	ਲੰਮਾ ਲੱਦੋ;
	bool stopsearch = false;

	ਜੇ (READ_ONCE(dst_rq->numa_migrate_on))
		ਵਾਪਸ false;

	rcu_read_ਤਾਲਾ();
	cur = rcu_dereference_all(dst_rq->curr);
	ਜੇ (cur && ((cur->flags & (PF_EXITING | PF_KTHREAD)) ||
		    !cur->mm))
		cur = ਨਿਰਾ;

	/*
	 * Because we have preemption enabled we can get migrated around and
	 * end try selecting ourselves (current == env->p) as a swap candidate.
	 */
	ਜੇ (cur == env->p) {
		stopsearch = true;
		ਜਾਓ unlock;
	}

	ਜੇ (!cur) {
		ਜੇ (maymove && moveimp >= env->ਉੱਤਮ_imp)
			ਜਾਓ ਸੌਂਪੋ;
		ਨਹੀਂਤਾਂ
			ਜਾਓ unlock;
	}

	/* Skip this swap candidate if cannot move to the source cpu. */
	ਜੇ (!cpumask_test_cpu(env->src_cpu, cur->cpus_ptr))
		ਜਾਓ unlock;

	/*
	 * Skip this swap candidate if it is not moving to its preferred
	 * node and the best task is.
	 */
	ਜੇ (env->ਉੱਤਮ_task &&
	    env->ਉੱਤਮ_task->numa_preferred_nid == env->src_nid &&
	    cur->numa_preferred_nid != env->src_nid) {
		ਜਾਓ unlock;
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
	cur_ng = rcu_dereference_all(cur->numa_ਸਮੂਹ);
	ਜੇ (cur_ng == p_ng) {
		/*
		 * Do not swap within a group or between tasks that have
		 * no group if there is spare capacity. Swapping does
		 * not address the load imbalance and helps one task at
		 * the cost of punishing another.
		 */
		ਜੇ (env->dst_stats.ਗੰਢ_type == ਗੰਢ_has_ਵਾਧੂ)
			ਜਾਓ unlock;

		imp = taskimp + task_ਭਾਰ(cur, env->src_nid, dist) -
		      task_ਭਾਰ(cur, env->dst_nid, dist);
		/*
		 * Add some hysteresis to prevent swapping the
		 * tasks within a group over tiny differences.
		 */
		ਜੇ (cur_ng)
			imp -= imp / 16;
	} ਨਹੀਂਤਾਂ {
		/*
		 * Compare the group weights. If a task is all by itself
		 * (not part of a group), use the task weight instead.
		 */
		ਜੇ (cur_ng && p_ng)
			imp += ਸਮੂਹ_ਭਾਰ(cur, env->src_nid, dist) -
			       ਸਮੂਹ_ਭਾਰ(cur, env->dst_nid, dist);
		ਨਹੀਂਤਾਂ
			imp += task_ਭਾਰ(cur, env->src_nid, dist) -
			       task_ਭਾਰ(cur, env->dst_nid, dist);
	}

	/* Discourage picking a task already on its preferred node */
	ਜੇ (cur->numa_preferred_nid == env->dst_nid)
		imp -= imp / 16;

	/*
	 * Encourage picking a task that moves to its preferred node.
	 * This potentially makes imp larger than it's maximum of
	 * 1998 (see SMALLIMP and task_weight for why) but in this
	 * case, it does not matter.
	 */
	ਜੇ (cur->numa_preferred_nid == env->src_nid)
		imp += imp / 8;

	ਜੇ (maymove && moveimp > imp && moveimp > env->ਉੱਤਮ_imp) {
		imp = moveimp;
		cur = ਨਿਰਾ;
		ਜਾਓ ਸੌਂਪੋ;
	}

	/*
	 * Prefer swapping with a task moving to its preferred node over a
	 * task that is not.
	 */
	ਜੇ (env->ਉੱਤਮ_task && cur->numa_preferred_nid == env->src_nid &&
	    env->ਉੱਤਮ_task->numa_preferred_nid != env->src_nid) {
		ਜਾਓ ਸੌਂਪੋ;
	}

	/*
	 * If the NUMA importance is less than SMALLIMP,
	 * task migration might only result in ping pong
	 * of tasks and also hurt performance due to cache
	 * misses.
	 */
	ਜੇ (imp < SMALLIMP || imp <= env->ਉੱਤਮ_imp + SMALLIMP / 2)
		ਜਾਓ unlock;

	/*
	 * In the overloaded case, try and keep the load balanced.
	 */
	ਲੱਦੋ = task_h_ਲੱਦੋ(env->p) - task_h_ਲੱਦੋ(cur);
	ਜੇ (!ਲੱਦੋ)
		ਜਾਓ ਸੌਂਪੋ;

	dst_ਲੱਦੋ = env->dst_stats.ਲੱਦੋ + ਲੱਦੋ;
	src_ਲੱਦੋ = env->src_stats.ਲੱਦੋ - ਲੱਦੋ;

	ਜੇ (ਲੱਦੋ_too_imbalanced(src_ਲੱਦੋ, dst_ਲੱਦੋ, env))
		ਜਾਓ unlock;

ਸੌਂਪੋ:
	/* Evaluate an idle CPU for a task numa move. */
	ਜੇ (!cur) {
		ਪੂਰਨ cpu = env->dst_stats.idle_cpu;

		/* Nothing cached so current CPU went idle since the search. */
		ਜੇ (cpu < 0)
			cpu = env->dst_cpu;

		/*
		 * If the CPU is no longer truly idle and the previous best CPU
		 * is, keep using it.
		 */
		ਜੇ (!idle_cpu(cpu) && env->ਉੱਤਮ_cpu >= 0 &&
		    idle_cpu(env->ਉੱਤਮ_cpu)) {
			cpu = env->ਉੱਤਮ_cpu;
		}

		env->dst_cpu = cpu;
	}

	task_numa_ਸੌਂਪੋ(env, cur, imp);

	/*
	 * If a move to idle is allowed because there is capacity or load
	 * balance improves then stop the search. While a better swap
	 * candidate may exist, a search is not free.
	 */
	ਜੇ (maymove && !cur && env->ਉੱਤਮ_cpu >= 0 && idle_cpu(env->ਉੱਤਮ_cpu))
		stopsearch = true;

	/*
	 * If a swap candidate must be identified and the current best task
	 * moves its preferred node then stop the search.
	 */
	ਜੇ (!maymove && env->ਉੱਤਮ_task &&
	    env->ਉੱਤਮ_task->numa_preferred_nid == env->src_nid) {
		stopsearch = true;
	}
unlock:
	rcu_read_unlock();

	ਵਾਪਸ stopsearch;
}

ਸਥਿਰ ਖਾਲੀ task_numa_ਲੱਭੋ_cpu(ਬਣਤਰ task_numa_env *env,
				ਲੰਮਾ taskimp, ਲੰਮਾ groupimp)
{
	bool maymove = false;
	ਪੂਰਨ cpu;

	/*
	 * If dst node has spare capacity, then check if there is an
	 * imbalance that would be overruled by the load balancer.
	 */
	ਜੇ (env->dst_stats.ਗੰਢ_type == ਗੰਢ_has_ਵਾਧੂ) {
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ imbalance;
		ਪੂਰਨ src_running, dst_running;

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
		ਜੇ (!imbalance) {
			maymove = true;
			ਜੇ (env->dst_stats.idle_cpu >= 0) {
				env->dst_cpu = env->dst_stats.idle_cpu;
				task_numa_ਸੌਂਪੋ(env, ਨਿਰਾ, 0);
				ਵਾਪਸ;
			}
		}
	} ਨਹੀਂਤਾਂ {
		ਲੰਮਾ src_ਲੱਦੋ, dst_ਲੱਦੋ, ਲੱਦੋ;
		/*
		 * If the improvement from just moving env->p direction is better
		 * than swapping tasks around, check if a move is possible.
		 */
		ਲੱਦੋ = task_h_ਲੱਦੋ(env->p);
		dst_ਲੱਦੋ = env->dst_stats.ਲੱਦੋ + ਲੱਦੋ;
		src_ਲੱਦੋ = env->src_stats.ਲੱਦੋ - ਲੱਦੋ;
		maymove = !ਲੱਦੋ_too_imbalanced(src_ਲੱਦੋ, dst_ਲੱਦੋ, env);
	}

	/* Skip CPUs if the source task cannot migrate */
	for_each_cpu_and(cpu, cpumask_ਦਾ_ਗੰਢ(env->dst_nid), env->p->cpus_ptr) {
		env->dst_cpu = cpu;
		ਜੇ (task_numa_compare(env, taskimp, groupimp, maymove))
			ਰੋਕੋ;
	}
}

ਸਥਿਰ ਪੂਰਨ task_numa_migrate(ਬਣਤਰ task_struct *p)
{
	ਬਣਤਰ task_numa_env env = {
		.p = p,

		.src_cpu = task_cpu(p),
		.src_nid = task_ਗੰਢ(p),

		.imbalance_pct = 112,

		.ਉੱਤਮ_task = ਨਿਰਾ,
		.ਉੱਤਮ_imp = 0,
		.ਉੱਤਮ_cpu = -1,
	};
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ taskweight, groupweight;
	ਬਣਤਰ sched_domain *sd;
	ਲੰਮਾ taskimp, groupimp;
	ਬਣਤਰ numa_ਸਮੂਹ *ng;
	ਬਣਤਰ rq *ਉੱਤਮ_rq;
	ਪੂਰਨ nid, ret, dist;

	/*
	 * Pick the lowest SD_NUMA domain, as that would have the smallest
	 * imbalance and would be the first to start moving tasks about.
	 *
	 * And we want to avoid any moving of tasks about, as that would create
	 * random movement of tasks -- counter the numa conditions we're trying
	 * to satisfy here.
	 */
	rcu_read_ਤਾਲਾ();
	sd = rcu_dereference_all(ਪ੍ਰਤੀ_cpu(sd_numa, env.src_cpu));
	ਜੇ (sd) {
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
	ਜੇ (unlikely(!sd)) {
		sched_setnuma(p, task_ਗੰਢ(p));
		ਵਾਪਸ -EINVAL;
	}

	env.dst_nid = p->numa_preferred_nid;
	dist = env.dist = ਗੰਢ_ਦੂਰੀ(env.src_nid, env.dst_nid);
	taskweight = task_ਭਾਰ(p, env.src_nid, dist);
	groupweight = ਸਮੂਹ_ਭਾਰ(p, env.src_nid, dist);
	update_numa_stats(&env, &env.src_stats, env.src_nid, false);
	taskimp = task_ਭਾਰ(p, env.dst_nid, dist) - taskweight;
	groupimp = ਸਮੂਹ_ਭਾਰ(p, env.dst_nid, dist) - groupweight;
	update_numa_stats(&env, &env.dst_stats, env.dst_nid, true);

	/* Try to find a spot on the preferred nid. */
	task_numa_ਲੱਭੋ_cpu(&env, taskimp, groupimp);

	/*
	 * Look at other nodes in these cases:
	 * - there is no space available on the preferred_nid
	 * - the task is part of a numa_group that is interleaved across
	 *   multiple NUMA nodes; in order to better consolidate the group,
	 *   we need to check other locations.
	 */
	ng = deref_curr_numa_ਸਮੂਹ(p);
	ਜੇ (env.ਉੱਤਮ_cpu == -1 || (ng && ng->ਸਰਗਰਮ_nodes > 1)) {
		for_each_ਗੰਢ_state(nid, N_CPU) {
			ਜੇ (nid == env.src_nid || nid == p->numa_preferred_nid)
				ਜਾਰੀ;

			dist = ਗੰਢ_ਦੂਰੀ(env.src_nid, env.dst_nid);
			ਜੇ (sched_numa_topology_type == NUMA_BACKPLANE &&
						dist != env.dist) {
				taskweight = task_ਭਾਰ(p, env.src_nid, dist);
				groupweight = ਸਮੂਹ_ਭਾਰ(p, env.src_nid, dist);
			}

			/* Only consider nodes where both task and groups benefit */
			taskimp = task_ਭਾਰ(p, nid, dist) - taskweight;
			groupimp = ਸਮੂਹ_ਭਾਰ(p, nid, dist) - groupweight;
			ਜੇ (taskimp < 0 && groupimp < 0)
				ਜਾਰੀ;

			env.dist = dist;
			env.dst_nid = nid;
			update_numa_stats(&env, &env.dst_stats, env.dst_nid, true);
			task_numa_ਲੱਭੋ_cpu(&env, taskimp, groupimp);
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
	ਜੇ (ng) {
		ਜੇ (env.ਉੱਤਮ_cpu == -1)
			nid = env.src_nid;
		ਨਹੀਂਤਾਂ
			nid = cpu_ਮੰਜ਼ਿਲ_ਗੰਢ(env.ਉੱਤਮ_cpu);

		ਜੇ (nid != p->numa_preferred_nid)
			sched_setnuma(p, nid);
	}

	/* No better CPU than the current one was found. */
	ਜੇ (env.ਉੱਤਮ_cpu == -1) {
		trace_sched_stick_numa(p, env.src_cpu, ਨਿਰਾ, -1);
		ਵਾਪਸ -EAGAIN;
	}

	ਉੱਤਮ_rq = cpu_rq(env.ਉੱਤਮ_cpu);
	ਜੇ (env.ਉੱਤਮ_task == ਨਿਰਾ) {
		ret = migrate_task_ਮੰਜ਼ਿਲ(p, env.ਉੱਤਮ_cpu);
		WRITE_ONCE(ਉੱਤਮ_rq->numa_migrate_on, 0);
		ਜੇ (ret != 0)
			trace_sched_stick_numa(p, env.src_cpu, ਨਿਰਾ, env.ਉੱਤਮ_cpu);
		ਵਾਪਸ ret;
	}

	ret = migrate_ਅਦਲਾ_ਬਦਲੀ(p, env.ਉੱਤਮ_task, env.ਉੱਤਮ_cpu, env.src_cpu);
	WRITE_ONCE(ਉੱਤਮ_rq->numa_migrate_on, 0);

	ਜੇ (ret != 0)
		trace_sched_stick_numa(p, env.src_cpu, env.ਉੱਤਮ_task, env.ਉੱਤਮ_cpu);
	put_task_struct(env.ਉੱਤਮ_task);
	ਵਾਪਸ ret;
}

/* Attempt to migrate a task to a CPU on the preferred node. */
ਸਥਿਰ ਖਾਲੀ numa_migrate_preferred(ਬਣਤਰ task_struct *p)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ interval = HZ;

	/* This task has no NUMA fault statistics yet */
	ਜੇ (unlikely(p->numa_preferred_nid == NUMA_NO_ਗੰਢ || !p->numa_ਨੁਕਸ))
		ਵਾਪਸ;

	/* Periodically retry migrating the task to the preferred node */
	interval = min(interval, msecs_ਮੰਜ਼ਿਲ_jiffies(p->numa_ਜਾਂਚ_period) / 16);
	p->numa_migrate_retry = jiffies + interval;

	/* Success if task is already running on preferred CPU */
	ਜੇ (task_ਗੰਢ(p) == p->numa_preferred_nid)
		ਵਾਪਸ;

	/* Otherwise, try migrate to a CPU on the preferred node */
	task_numa_migrate(p);
}

/*
 * Find out how many nodes the workload is actively running on. Do this by
 * tracking the nodes from which NUMA hinting faults are triggered. This can
 * be different from the set of nodes where the workload's memory is currently
 * located.
 */
ਸਥਿਰ ਖਾਲੀ numa_ਸਮੂਹ_ਗਿਣਤੀ_ਸਰਗਰਮ_nodes(ਬਣਤਰ numa_ਸਮੂਹ *numa_ਸਮੂਹ)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਨੁਕਸ, max_ਨੁਕਸ = 0;
	ਪੂਰਨ nid, ਸਰਗਰਮ_nodes = 0;

	for_each_ਗੰਢ_state(nid, N_CPU) {
		ਨੁਕਸ = ਸਮੂਹ_ਨੁਕਸ_cpu(numa_ਸਮੂਹ, nid);
		ਜੇ (ਨੁਕਸ > max_ਨੁਕਸ)
			max_ਨੁਕਸ = ਨੁਕਸ;
	}

	for_each_ਗੰਢ_state(nid, N_CPU) {
		ਨੁਕਸ = ਸਮੂਹ_ਨੁਕਸ_cpu(numa_ਸਮੂਹ, nid);
		ਜੇ (ਨੁਕਸ * ਸਰਗਰਮ_ਗੰਢ_FRACTION > max_ਨੁਕਸ)
			ਸਰਗਰਮ_nodes++;
	}

	numa_ਸਮੂਹ->max_ਨੁਕਸ_cpu = max_ਨੁਕਸ;
	numa_ਸਮੂਹ->ਸਰਗਰਮ_nodes = ਸਰਗਰਮ_nodes;
}

/*
 * When adapting the scan rate, the period is divided into NUMA_PERIOD_SLOTS
 * increments. The more local the fault statistics are, the higher the scan
 * period will be for the next scan window. If local/(local+remote) ratio is
 * below NUMA_PERIOD_THRESHOLD (where range of ratio is 1..NUMA_PERIOD_SLOTS)
 * the scan period will decrease. Aim for 70% local accesses.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NUMA_PERIOD_SLOTS 10
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NUMA_PERIOD_ਹੱਦ 7

/*
 * Increase the scan period (slow down scanning) if the majority of
 * our memory is already on our local node, or if the majority of
 * the page accesses are shared with other processes.
 * Otherwise, decrease the scan period.
 */
ਸਥਿਰ ਖਾਲੀ update_task_ਜਾਂਚ_period(ਬਣਤਰ task_struct *p,
			ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ shared, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਨਿਜੀ)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ period_ਥਾਂ;
	ਪੂਰਨ lr_ਅਨੁਪਾਤ, ps_ਅਨੁਪਾਤ;
	ਪੂਰਨ diff;

	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ remote = p->numa_ਨੁਕਸ_locality[0];
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ local = p->numa_ਨੁਕਸ_locality[1];

	/*
	 * If there were no record hinting faults then either the task is
	 * completely idle or all activity is in areas that are not of interest
	 * to automatic numa balancing. Related to that, if there were failed
	 * migration then it implies we are migrating too quickly or the local
	 * node is overloaded. In either case, scan slower
	 */
	ਜੇ (local + shared == 0 || p->numa_ਨੁਕਸ_locality[2]) {
		p->numa_ਜਾਂਚ_period = min(p->numa_ਜਾਂਚ_period_max,
			p->numa_ਜਾਂਚ_period << 1);

		p->mm->numa_ਅਗਲਾ_ਜਾਂਚ = jiffies +
			msecs_ਮੰਜ਼ਿਲ_jiffies(p->numa_ਜਾਂਚ_period);

		ਜਾਓ ਬਾਹਰਲਾ;
	}

	/*
	 * Prepare to scale scan period relative to the current period.
	 *	 == NUMA_PERIOD_THRESHOLD scan period stays the same
	 *       <  NUMA_PERIOD_THRESHOLD scan period decreases (scan faster)
	 *	 >= NUMA_PERIOD_THRESHOLD scan period increases (scan slower)
	 */
	period_ਥਾਂ = DIV_ਗੇੜਾ_ਉੱਪਰ(p->numa_ਜਾਂਚ_period, NUMA_PERIOD_SLOTS);
	lr_ਅਨੁਪਾਤ = (local * NUMA_PERIOD_SLOTS) / (local + remote);
	ps_ਅਨੁਪਾਤ = (ਨਿਜੀ * NUMA_PERIOD_SLOTS) / (ਨਿਜੀ + shared);

	ਜੇ (ps_ਅਨੁਪਾਤ >= NUMA_PERIOD_ਹੱਦ) {
		/*
		 * Most memory accesses are local. There is no need to
		 * do fast NUMA scanning, since memory is already local.
		 */
		ਪੂਰਨ ਥਾਂ = ps_ਅਨੁਪਾਤ - NUMA_PERIOD_ਹੱਦ;
		ਜੇ (!ਥਾਂ)
			ਥਾਂ = 1;
		diff = ਥਾਂ * period_ਥਾਂ;
	} ਨਹੀਂਤਾਂ ਜੇ (lr_ਅਨੁਪਾਤ >= NUMA_PERIOD_ਹੱਦ) {
		/*
		 * Most memory accesses are shared with other tasks.
		 * There is no point in continuing fast NUMA scanning,
		 * since other tasks may just move the memory elsewhere.
		 */
		ਪੂਰਨ ਥਾਂ = lr_ਅਨੁਪਾਤ - NUMA_PERIOD_ਹੱਦ;
		ਜੇ (!ਥਾਂ)
			ਥਾਂ = 1;
		diff = ਥਾਂ * period_ਥਾਂ;
	} ਨਹੀਂਤਾਂ {
		/*
		 * Private memory faults exceed (SLOTS-THRESHOLD)/SLOTS,
		 * yet they are not on the local NUMA node. Speed up
		 * NUMA scanning to get the memory moved over.
		 */
		ਪੂਰਨ ਅਨੁਪਾਤ = max(lr_ਅਨੁਪਾਤ, ps_ਅਨੁਪਾਤ);
		diff = -(NUMA_PERIOD_ਹੱਦ - ਅਨੁਪਾਤ) * period_ਥਾਂ;
	}

	p->numa_ਜਾਂਚ_period = clamp(p->numa_ਜਾਂਚ_period + diff,
			task_ਜਾਂਚ_min(p), task_ਜਾਂਚ_max(p));

ਬਾਹਰਲਾ:
	memset(p->numa_ਨੁਕਸ_locality, 0,
	       ਆਕਾਰ(p->numa_ਨੁਕਸ_locality));
}

/*
 * Get the fraction of time the task has been running since the last
 * NUMA placement cycle. The scheduler keeps similar statistics, but
 * decays those on a 32ms period, which is orders of magnitude off
 * from the dozens-of-seconds NUMA balancing period. Use the scheduler
 * stats only if the task is so new there are no NUMA statistics yet.
 */
ਸਥਿਰ u64 numa_get_avg_runtime(ਬਣਤਰ task_struct *p, u64 *period)
{
	u64 runtime, delta, now;
	/* Use the start of this time slice to avoid calculations. */
	now = p->se.exec_ਸ਼ੁਰੂ;
	runtime = p->se.ਜੋੜ_exec_runtime;

	ਜੇ (p->ਆਖਰੀ_task_numa_placement) {
		delta = runtime - p->ਆਖਰੀ_ਜੋੜ_exec_runtime;
		*period = now - p->ਆਖਰੀ_task_numa_placement;

		/* Avoid time going backwards, prevent potential divide error: */
		ਜੇ (unlikely((s64)*period < 0))
			*period = 0;
	} ਨਹੀਂਤਾਂ {
		delta = p->se.avg.ਲੱਦੋ_ਜੋੜ;
		*period = ਲੱਦੋ_AVG_MAX;
	}

	p->ਆਖਰੀ_ਜੋੜ_exec_runtime = runtime;
	p->ਆਖਰੀ_task_numa_placement = now;

	ਵਾਪਸ delta;
}

/*
 * Determine the preferred nid for a task in a numa_group. This needs to
 * be done in a way that produces consistent results with group_weight,
 * otherwise workloads might not converge.
 */
ਸਥਿਰ ਪੂਰਨ preferred_ਸਮੂਹ_nid(ਬਣਤਰ task_struct *p, ਪੂਰਨ nid)
{
	nodemask_t nodes;
	ਪੂਰਨ dist;

	/* Direct connections between all NUMA nodes. */
	ਜੇ (sched_numa_topology_type == NUMA_ਸਿੱਧਾ)
		ਵਾਪਸ nid;

	/*
	 * On a system with glueless mesh NUMA topology, group_weight
	 * scores nodes according to the number of NUMA hinting faults on
	 * both the node itself, and on nearby nodes.
	 */
	ਜੇ (sched_numa_topology_type == NUMA_GLUELESS_MESH) {
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ score, max_score = 0;
		ਪੂਰਨ ਗੰਢ, max_ਗੰਢ = nid;

		dist = sched_max_numa_ਦੂਰੀ;

		for_each_ਗੰਢ_state(ਗੰਢ, N_CPU) {
			score = ਸਮੂਹ_ਭਾਰ(p, ਗੰਢ, dist);
			ਜੇ (score > max_score) {
				max_score = score;
				max_ਗੰਢ = ਗੰਢ;
			}
		}
		ਵਾਪਸ max_ਗੰਢ;
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
	nodes = ਗੰਢ_ਹਾਲਤਾਂ[N_CPU];
	ਲਈ (dist = sched_max_numa_ਦੂਰੀ; dist > LOCAL_ਦੂਰੀ; dist--) {
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ max_ਨੁਕਸ = 0;
		nodemask_t max_ਸਮੂਹ = ਗੰਢ_ਨਕਾਬ_NONE;
		ਪੂਰਨ a, b;

		/* Are there nodes at this distance from each other? */
		ਜੇ (!ਲੱਭੋ_numa_ਦੂਰੀ(dist))
			ਜਾਰੀ;

		for_each_ਗੰਢ_ਨਕਾਬ(a, nodes) {
			ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਨੁਕਸ = 0;
			nodemask_t this_ਸਮੂਹ;
			nodes_clear(this_ਸਮੂਹ);

			/* Sum group's NUMA faults; includes a==b case. */
			for_each_ਗੰਢ_ਨਕਾਬ(b, nodes) {
				ਜੇ (ਗੰਢ_ਦੂਰੀ(a, b) < dist) {
					ਨੁਕਸ += ਸਮੂਹ_ਨੁਕਸ(p, b);
					ਗੰਢ_ਸੈੱਟ(b, this_ਸਮੂਹ);
					ਗੰਢ_clear(b, nodes);
				}
			}

			/* Remember the top group. */
			ਜੇ (ਨੁਕਸ > max_ਨੁਕਸ) {
				max_ਨੁਕਸ = ਨੁਕਸ;
				max_ਸਮੂਹ = this_ਸਮੂਹ;
				/*
				 * subtle: at the smallest distance there is
				 * just one node left in each "group", the
				 * winner is the preferred nid.
				 */
				nid = a;
			}
		}
		/* Next round, evaluate the nodes within max_group. */
		ਜੇ (!max_ਨੁਕਸ)
			ਰੋਕੋ;
		nodes = max_ਸਮੂਹ;
	}
	ਵਾਪਸ nid;
}

ਸਥਿਰ ਖਾਲੀ task_numa_placement(ਬਣਤਰ task_struct *p)
	__context_unsafe(/* conditional locking */)
{
	ਬਣਤਰ sched_cache_ਸਮੂਹ __maybe_unused *grp;
	ਪੂਰਨ seq, nid, max_nid = NUMA_NO_ਗੰਢ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ max_ਨੁਕਸ = 0;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ fault_types[2] = { 0, 0 };
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਕੁੱਲ_ਨੁਕਸ;
	u64 runtime, period;
	spinlock_t *ਸਮੂਹ_ਤਾਲਾ = ਨਿਰਾ;
	ਲੰਮਾ __maybe_unused new_fp;
	ਬਣਤਰ numa_ਸਮੂਹ *ng;

	/*
	 * The p->mm->numa_scan_seq field gets updated without
	 * exclusive access. Use READ_ONCE() here to ensure
	 * that the field is read in a single access:
	 */
	seq = READ_ONCE(p->mm->numa_ਜਾਂਚ_seq);
	ਜੇ (p->numa_ਜਾਂਚ_seq == seq)
		ਵਾਪਸ;
	p->numa_ਜਾਂਚ_seq = seq;
	p->numa_ਜਾਂਚ_period_max = task_ਜਾਂਚ_max(p);

	ਕੁੱਲ_ਨੁਕਸ = p->numa_ਨੁਕਸ_locality[0] +
		       p->numa_ਨੁਕਸ_locality[1];
	runtime = numa_get_avg_runtime(p, &period);

	/* If the task is part of a group prevent parallel updates to group stats */
	ng = deref_curr_numa_ਸਮੂਹ(p);
	ਜੇ (ng) {
		ਸਮੂਹ_ਤਾਲਾ = &ng->ਤਾਲਾ;
		spin_ਤਾਲਾ_irq(ਸਮੂਹ_ਤਾਲਾ);
	}

	/* Find the node with the highest number of faults */
	for_each_online_ਗੰਢ(nid) {
		/* Keep track of the offsets in numa_faults array */
		ਪੂਰਨ mem_idx, membuf_idx, cpu_idx, cpubuf_idx;
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਨੁਕਸ = 0, ਸਮੂਹ_ਨੁਕਸ = 0;
		ਪੂਰਨ priv;

		ਲਈ (priv = 0; priv < NR_NUMA_HINT_FAULT_TYPES; priv++) {
			ਲੰਮਾ diff, f_diff, f_ਭਾਰ;

			mem_idx = task_ਨੁਕਸ_idx(NUMA_MEM, nid, priv);
			membuf_idx = task_ਨੁਕਸ_idx(NUMA_MEMBUF, nid, priv);
			cpu_idx = task_ਨੁਕਸ_idx(NUMA_CPU, nid, priv);
			cpubuf_idx = task_ਨੁਕਸ_idx(NUMA_CPUBUF, nid, priv);

			/* Decay existing window, copy faults since last scan */
			diff = p->numa_ਨੁਕਸ[membuf_idx] - p->numa_ਨੁਕਸ[mem_idx] / 2;
			fault_types[priv] += p->numa_ਨੁਕਸ[membuf_idx];
			p->numa_ਨੁਕਸ[membuf_idx] = 0;

			/*
			 * Normalize the faults_from, so all tasks in a group
			 * count according to CPU use, instead of by the raw
			 * number of faults. Tasks with little runtime have
			 * little over-all impact on throughput, and thus their
			 * faults are less important.
			 */
			f_ਭਾਰ = div64_u64(runtime << 16, period + 1);
			f_ਭਾਰ = (f_ਭਾਰ * p->numa_ਨੁਕਸ[cpubuf_idx]) /
				   (ਕੁੱਲ_ਨੁਕਸ + 1);
			f_diff = f_ਭਾਰ - p->numa_ਨੁਕਸ[cpu_idx] / 2;
			p->numa_ਨੁਕਸ[cpubuf_idx] = 0;

			p->numa_ਨੁਕਸ[mem_idx] += diff;
			p->numa_ਨੁਕਸ[cpu_idx] += f_diff;
			ਨੁਕਸ += p->numa_ਨੁਕਸ[mem_idx];
			p->ਕੁੱਲ_numa_ਨੁਕਸ += diff;
			ਜੇ (ng) {
				/*
				 * safe because we can only change our own group
				 *
				 * mem_idx represents the offset for a given
				 * nid and priv in a specific region because it
				 * is at the beginning of the numa_faults array.
				 */
				ng->ਨੁਕਸ[mem_idx] += diff;
				ng->ਨੁਕਸ[cpu_idx] += f_diff;
				ng->ਕੁੱਲ_ਨੁਕਸ += diff;
				ਸਮੂਹ_ਨੁਕਸ += ng->ਨੁਕਸ[mem_idx];
			}
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CACHE
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

				ਜੇ (grp) {
					new_fp = (ਲੰਮਾ)READ_ONCE(grp->footprint) + diff;
					WRITE_ONCE(grp->footprint, max(new_fp, 0L));
				}
			}
#ਜੇ_ਅੰਤ
		}

		ਜੇ (!ng) {
			ਜੇ (ਨੁਕਸ > max_ਨੁਕਸ) {
				max_ਨੁਕਸ = ਨੁਕਸ;
				max_nid = nid;
			}
		} ਨਹੀਂਤਾਂ ਜੇ (ਸਮੂਹ_ਨੁਕਸ > max_ਨੁਕਸ) {
			max_ਨੁਕਸ = ਸਮੂਹ_ਨੁਕਸ;
			max_nid = nid;
		}
	}

	/* Cannot migrate task to CPU-less node */
	max_nid = numa_ਸਭ_ਤੋਂ_ਨੇੜੇ_ਗੰਢ(max_nid, N_CPU);

	ਜੇ (ng) {
		numa_ਸਮੂਹ_ਗਿਣਤੀ_ਸਰਗਰਮ_nodes(ng);
		spin_unlock_irq(ਸਮੂਹ_ਤਾਲਾ);
		max_nid = preferred_ਸਮੂਹ_nid(p, max_nid);
	}

	ਜੇ (max_ਨੁਕਸ) {
		/* Set the new preferred node */
		ਜੇ (max_nid != p->numa_preferred_nid)
			sched_setnuma(p, max_nid);
	}

	update_task_ਜਾਂਚ_period(p, fault_types[0], fault_types[1]);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ get_numa_ਸਮੂਹ(ਬਣਤਰ numa_ਸਮੂਹ *grp)
{
	ਵਾਪਸ refcount_inc_not_zero(&grp->refcount);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ put_numa_ਸਮੂਹ(ਬਣਤਰ numa_ਸਮੂਹ *grp)
{
	ਜੇ (refcount_dec_and_test(&grp->refcount))
		kfree_rcu(grp, rcu);
}

ਸਥਿਰ ਖਾਲੀ task_numa_ਸਮੂਹ(ਬਣਤਰ task_struct *p, ਪੂਰਨ cpupid, ਪੂਰਨ flags,
			ਪੂਰਨ *priv)
{
	ਬਣਤਰ numa_ਸਮੂਹ *grp, *my_grp;
	ਬਣਤਰ task_struct *tsk;
	bool join = false;
	ਪੂਰਨ cpu = cpupid_ਮੰਜ਼ਿਲ_cpu(cpupid);
	ਪੂਰਨ i;

	ਜੇ (unlikely(!deref_curr_numa_ਸਮੂਹ(p))) {
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮਾਪ = ਆਕਾਰ(ਬਣਤਰ numa_ਸਮੂਹ) +
				    NR_NUMA_HINT_FAULT_STATS *
				    nr_ਗੰਢ_ids * ਆਕਾਰ(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ);

		grp = kzalloc(ਮਾਪ, GFP_KERNEL | __GFP_NOWARN);
		ਜੇ (!grp)
			ਵਾਪਸ;

		refcount_ਸੈੱਟ(&grp->refcount, 1);
		grp->ਸਰਗਰਮ_nodes = 1;
		grp->max_ਨੁਕਸ_cpu = 0;
		spin_ਤਾਲਾ_init(&grp->ਤਾਲਾ);
		grp->gid = p->pid;

		ਲਈ (i = 0; i < NR_NUMA_HINT_FAULT_STATS * nr_ਗੰਢ_ids; i++)
			grp->ਨੁਕਸ[i] = p->numa_ਨੁਕਸ[i];

		grp->ਕੁੱਲ_ਨੁਕਸ = p->ਕੁੱਲ_numa_ਨੁਕਸ;

		grp->nr_tasks++;
		rcu_ਸੌਂਪੋ_ਸੂਚਕ(p->numa_ਸਮੂਹ, grp);
	}

	rcu_read_ਤਾਲਾ();
	tsk = READ_ONCE(cpu_rq(cpu)->curr);

	ਜੇ (!cpupid_ਮੇਲ_pid(tsk, cpupid))
		ਜਾਓ no_join;

	grp = rcu_dereference_all(tsk->numa_ਸਮੂਹ);
	ਜੇ (!grp)
		ਜਾਓ no_join;

	my_grp = deref_curr_numa_ਸਮੂਹ(p);
	ਜੇ (grp == my_grp)
		ਜਾਓ no_join;

	/*
	 * Only join the other group if its bigger; if we're the bigger group,
	 * the other task will join us.
	 */
	ਜੇ (my_grp->nr_tasks > grp->nr_tasks)
		ਜਾਓ no_join;

	/*
	 * Tie-break on the grp address.
	 */
	ਜੇ (my_grp->nr_tasks == grp->nr_tasks && my_grp > grp)
		ਜਾਓ no_join;

	/* Always join threads in the same process. */
	ਜੇ (tsk->mm == current->mm)
		join = true;

	/* Simple filter to avoid false positives due to PID collisions */
	ਜੇ (flags & TNF_SHARED)
		join = true;

	/* Update priv based on whether false sharing was detected */
	*priv = !join;

	ਜੇ (join && !get_numa_ਸਮੂਹ(grp))
		ਜਾਓ no_join;

	rcu_read_unlock();

	ਜੇ (!join)
		ਵਾਪਸ;

	WARN_ON_ONCE(irqs_disabled());
	double_ਤਾਲਾ_irq(&my_grp->ਤਾਲਾ, &grp->ਤਾਲਾ);

	ਲਈ (i = 0; i < NR_NUMA_HINT_FAULT_STATS * nr_ਗੰਢ_ids; i++) {
		my_grp->ਨੁਕਸ[i] -= p->numa_ਨੁਕਸ[i];
		grp->ਨੁਕਸ[i] += p->numa_ਨੁਕਸ[i];
	}
	my_grp->ਕੁੱਲ_ਨੁਕਸ -= p->ਕੁੱਲ_numa_ਨੁਕਸ;
	grp->ਕੁੱਲ_ਨੁਕਸ += p->ਕੁੱਲ_numa_ਨੁਕਸ;

	my_grp->nr_tasks--;
	grp->nr_tasks++;

	spin_unlock(&my_grp->ਤਾਲਾ);
	spin_unlock_irq(&grp->ਤਾਲਾ);

	rcu_ਸੌਂਪੋ_ਸੂਚਕ(p->numa_ਸਮੂਹ, grp);

	put_numa_ਸਮੂਹ(my_grp);
	ਵਾਪਸ;

no_join:
	rcu_read_unlock();
	ਵਾਪਸ;
}

/*
 * Get rid of NUMA statistics associated with a task (either current or dead).
 * If @final is set, the task is dead and has reached refcount zero, so we can
 * safely free all relevant data structures. Otherwise, there might be
 * concurrent reads from places like load balancing and procfs, and we should
 * reset the data back to default state without freeing ->numa_faults.
 */
ਖਾਲੀ task_numa_free(ਬਣਤਰ task_struct *p, bool final)
{
	/* safe: p either is current or is being freed by current */
	ਬਣਤਰ numa_ਸਮੂਹ *grp = rcu_dereference_raw(p->numa_ਸਮੂਹ);
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ *numa_ਨੁਕਸ = p->numa_ਨੁਕਸ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags;
	ਪੂਰਨ i;

	ਜੇ (!numa_ਨੁਕਸ)
		ਵਾਪਸ;

	ਜੇ (grp) {
		spin_ਤਾਲਾ_irqsave(&grp->ਤਾਲਾ, flags);
		ਲਈ (i = 0; i < NR_NUMA_HINT_FAULT_STATS * nr_ਗੰਢ_ids; i++)
			grp->ਨੁਕਸ[i] -= p->numa_ਨੁਕਸ[i];
		grp->ਕੁੱਲ_ਨੁਕਸ -= p->ਕੁੱਲ_numa_ਨੁਕਸ;

		grp->nr_tasks--;
		spin_unlock_irqrestore(&grp->ਤਾਲਾ, flags);
		RCU_INIT_ਸੂਚਕ(p->numa_ਸਮੂਹ, ਨਿਰਾ);
		put_numa_ਸਮੂਹ(grp);
	}

	ਜੇ (final) {
		p->numa_ਨੁਕਸ = ਨਿਰਾ;
		kfree(numa_ਨੁਕਸ);
	} ਨਹੀਂਤਾਂ {
		p->ਕੁੱਲ_numa_ਨੁਕਸ = 0;
		ਲਈ (i = 0; i < NR_NUMA_HINT_FAULT_STATS * nr_ਗੰਢ_ids; i++)
			numa_ਨੁਕਸ[i] = 0;
	}
}

/*
 * Got a PROT_NONE fault for a page on @node.
 */
ਖਾਲੀ task_numa_fault(ਪੂਰਨ ਆਖਰੀ_cpupid, ਪੂਰਨ mem_ਗੰਢ, ਪੂਰਨ ਪੰਨੇ, ਪੂਰਨ flags)
{
	ਬਣਤਰ task_struct *p = current;
	bool migrated = flags & TNF_MIGRATED;
	ਪੂਰਨ cpu_ਗੰਢ = task_ਗੰਢ(current);
	ਪੂਰਨ local = !!(flags & TNF_FAULT_LOCAL);
	ਬਣਤਰ numa_ਸਮੂਹ *ng;
	ਪੂਰਨ priv;

	ਜੇ (!static_branch_likely(&sched_numa_balancing))
		ਵਾਪਸ;

	/* for example, ksmd faulting in a user's mm */
	ਜੇ (!p->mm)
		ਵਾਪਸ;

	/*
	 * NUMA faults statistics are unnecessary for the slow memory
	 * node for memory tiering mode.
	 */
	ਜੇ (!ਗੰਢ_is_toptier(mem_ਗੰਢ) &&
	    (sysctl_numa_balancing_ਬਹੁਲਕ & NUMA_BALANCING_ਸਮ੍ਰਿਤੀ_TIERING ||
	     !cpupid_valid(ਆਖਰੀ_cpupid)))
		ਵਾਪਸ;

	/* Allocate buffer to track faults on a per-node basis */
	ਜੇ (unlikely(!p->numa_ਨੁਕਸ)) {
		ਪੂਰਨ ਮਾਪ = ਆਕਾਰ(*p->numa_ਨੁਕਸ) *
			   NR_NUMA_HINT_FAULT_BUCKETS * nr_ਗੰਢ_ids;

		p->numa_ਨੁਕਸ = kzalloc(ਮਾਪ, GFP_KERNEL|__GFP_NOWARN);
		ਜੇ (!p->numa_ਨੁਕਸ)
			ਵਾਪਸ;

		p->ਕੁੱਲ_numa_ਨੁਕਸ = 0;
		memset(p->numa_ਨੁਕਸ_locality, 0, ਆਕਾਰ(p->numa_ਨੁਕਸ_locality));
	}

	/*
	 * First accesses are treated as private, otherwise consider accesses
	 * to be private if the accessing pid has not changed
	 */
	ਜੇ (unlikely(ਆਖਰੀ_cpupid == (-1 & ਆਖਰੀ_CPUPID_ਨਕਾਬ))) {
		priv = 1;
	} ਨਹੀਂਤਾਂ {
		priv = cpupid_ਮੇਲ_pid(p, ਆਖਰੀ_cpupid);
		ਜੇ (!priv && !(flags & TNF_NO_ਸਮੂਹ))
			task_numa_ਸਮੂਹ(p, ਆਖਰੀ_cpupid, flags, &priv);
	}

	/*
	 * If a workload spans multiple NUMA nodes, a shared fault that
	 * occurs wholly within the set of nodes that the workload is
	 * actively using should be counted as local. This allows the
	 * scan rate to slow down when a workload has settled down.
	 */
	ng = deref_curr_numa_ਸਮੂਹ(p);
	ਜੇ (!priv && !local && ng && ng->ਸਰਗਰਮ_nodes > 1 &&
				numa_is_ਸਰਗਰਮ_ਗੰਢ(cpu_ਗੰਢ, ng) &&
				numa_is_ਸਰਗਰਮ_ਗੰਢ(mem_ਗੰਢ, ng))
		local = 1;

	/*
	 * Retry to migrate task to preferred node periodically, in case it
	 * previously failed, or the scheduler moved us.
	 */
	ਜੇ (ਸਮਾਂ_after(jiffies, p->numa_migrate_retry)) {
		task_numa_placement(p);
		numa_migrate_preferred(p);
	}

	ਜੇ (migrated)
		p->numa_ਪੰਨੇ_migrated += ਪੰਨੇ;
	ਜੇ (flags & TNF_MIGRATE_FAIL)
		p->numa_ਨੁਕਸ_locality[2] += ਪੰਨੇ;

	p->numa_ਨੁਕਸ[task_ਨੁਕਸ_idx(NUMA_MEMBUF, mem_ਗੰਢ, priv)] += ਪੰਨੇ;
	p->numa_ਨੁਕਸ[task_ਨੁਕਸ_idx(NUMA_CPUBUF, cpu_ਗੰਢ, priv)] += ਪੰਨੇ;
	p->numa_ਨੁਕਸ_locality[local] += ਪੰਨੇ;
}

ਸਥਿਰ ਖਾਲੀ reset_ptenuma_ਜਾਂਚ(ਬਣਤਰ task_struct *p)
{
	/*
	 * We only did a read acquisition of the mmap sem, so
	 * p->mm->numa_scan_seq is written to without exclusive access
	 * and the update is not guaranteed to be atomic. That's not
	 * much of an issue though, since this is just used for
	 * statistical sampling. Use READ_ONCE/WRITE_ONCE, which are not
	 * expensive, to avoid any form of compiler optimizations:
	 */
	WRITE_ONCE(p->mm->numa_ਜਾਂਚ_seq, READ_ONCE(p->mm->numa_ਜਾਂਚ_seq) + 1);
	p->mm->numa_ਜਾਂਚ_ਵਿੱਥ = 0;
}

ਸਥਿਰ bool vma_is_accessed(ਬਣਤਰ mm_struct *mm, ਬਣਤਰ vm_area_struct *vma)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ pids;
	/*
	 * Allow unconditional access first two times, so that all the (pages)
	 * of VMAs get prot_none fault introduced irrespective of accesses.
	 * This is also done to avoid any side effect of task scanning
	 * amplifying the unfairness of disjoint set of VMAs' access.
	 */
	ਜੇ ((READ_ONCE(current->mm->numa_ਜਾਂਚ_seq) - vma->numab_state->ਸ਼ੁਰੂ_ਜਾਂਚ_seq) < 2)
		ਵਾਪਸ true;

	pids = vma->numab_state->pids_ਸਰਗਰਮ[0] | vma->numab_state->pids_ਸਰਗਰਮ[1];
	ਜੇ (test_ਦੋਆਧਾਰੀ_ਅੰਕ(hash_32(current->pid, ilog2(ਬਿੱਟ_ਪ੍ਰਤੀ_LONG)), &pids))
		ਵਾਪਸ true;

	/*
	 * Complete a scan that has already started regardless of PID access, or
	 * some VMAs may never be scanned in multi-threaded applications:
	 */
	ਜੇ (mm->numa_ਜਾਂਚ_ਵਿੱਥ > vma->vm_ਸ਼ੁਰੂ) {
		trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_IGNORE_PID);
		ਵਾਪਸ true;
	}

	/*
	 * This vma has not been accessed for a while, and if the number
	 * the threads in the same process is low, which means no other
	 * threads can help scan this vma, force a vma scan.
	 */
	ਜੇ (READ_ONCE(mm->numa_ਜਾਂਚ_seq) >
	   (vma->numab_state->prev_ਜਾਂਚ_seq + get_nr_ਧਾਗੇ(current)))
		ਵਾਪਸ true;

	ਵਾਪਸ false;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ VMA_PID_RESET_PERIOD (4 * sysctl_numa_balancing_ਜਾਂਚ_delay)

/*
 * The expensive part of numa migration is done from task_work context.
 * Triggered from task_tick_numa().
 */
ਸਥਿਰ ਖਾਲੀ task_numa_work(ਬਣਤਰ callback_ਸਿਰ *work)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ migrate, ਅਗਲਾ_ਜਾਂਚ, now = jiffies;
	ਬਣਤਰ task_struct *p = current;
	ਬਣਤਰ mm_struct *mm = p->mm;
	u64 runtime = p->se.ਜੋੜ_exec_runtime;
	ਬਣਤਰ vm_area_struct *vma;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਸ਼ੁਰੂ, ਸਿਰਾ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ nr_pte_updates = 0;
	ਲੰਮਾ ਪੰਨੇ, virtpages;
	ਬਣਤਰ vma_iterator vmi;
	bool vma_pids_skipped;
	bool vma_pids_forced = false;

	WARN_ON_ONCE(p != container_ਦਾ(work, ਬਣਤਰ task_struct, numa_work));

	work->ਅਗਲਾ = work;
	/*
	 * Who cares about NUMA placement when they're dying.
	 *
	 * NOTE: make sure not to dereference p->mm before this check,
	 * exit_task_work() happens _after_ exit_mm() so we could be called
	 * without p->mm even though we still had it when we enqueued this
	 * work.
	 */
	ਜੇ (p->flags & PF_EXITING)
		ਵਾਪਸ;

	/*
	 * Memory is pinned to only one NUMA node via cpuset.mems, naturally
	 * no page can be migrated.
	 */
	ਜੇ (cpusets_enabled() && nodes_ਭਾਰ(cpuset_current_mems_allowed) == 1) {
		trace_sched_skip_cpuset_numa(current, &cpuset_current_mems_allowed);
		ਵਾਪਸ;
	}

	ਜੇ (!mm->numa_ਅਗਲਾ_ਜਾਂਚ) {
		mm->numa_ਅਗਲਾ_ਜਾਂਚ = now +
			msecs_ਮੰਜ਼ਿਲ_jiffies(sysctl_numa_balancing_ਜਾਂਚ_delay);
	}

	/*
	 * Enforce maximal scan/migration frequency..
	 */
	migrate = mm->numa_ਅਗਲਾ_ਜਾਂਚ;
	ਜੇ (ਸਮਾਂ_before(now, migrate))
		ਵਾਪਸ;

	ਜੇ (p->numa_ਜਾਂਚ_period == 0) {
		p->numa_ਜਾਂਚ_period_max = task_ਜਾਂਚ_max(p);
		p->numa_ਜਾਂਚ_period = task_ਜਾਂਚ_ਸ਼ੁਰੂ(p);
	}

	ਅਗਲਾ_ਜਾਂਚ = now + msecs_ਮੰਜ਼ਿਲ_jiffies(p->numa_ਜਾਂਚ_period);
	ਜੇ (!try_cmpxchg(&mm->numa_ਅਗਲਾ_ਜਾਂਚ, &migrate, ਅਗਲਾ_ਜਾਂਚ))
		ਵਾਪਸ;

	/*
	 * Delay this task enough that another task of this mm will likely win
	 * the next time around.
	 */
	p->ਗੰਢ_stamp += 2 * TICK_NSEC;

	ਪੰਨੇ = sysctl_numa_balancing_ਜਾਂਚ_ਮਾਪ;
	ਪੰਨੇ <<= 20 - ਪੰਨਾ_ਖਿਸਕਾਓ; /* MB in pages */
	virtpages = ਪੰਨੇ * 8;	   /* Scan up to this much virtual space */
	ਜੇ (!ਪੰਨੇ)
		ਵਾਪਸ;


	ਜੇ (!mmap_read_trylock(mm))
		ਵਾਪਸ;

	/*
	 * VMAs are skipped if the current PID has not trapped a fault within
	 * the VMA recently. Allow scanning to be forced if there is no
	 * suitable VMA remaining.
	 */
	vma_pids_skipped = false;

retry_pids:
	ਸ਼ੁਰੂ = mm->numa_ਜਾਂਚ_ਵਿੱਥ;
	vma_iter_init(&vmi, mm, ਸ਼ੁਰੂ);
	vma = vma_ਅਗਲਾ(&vmi);
	ਜੇ (!vma) {
		reset_ptenuma_ਜਾਂਚ(p);
		ਸ਼ੁਰੂ = 0;
		vma_iter_ਸੈੱਟ(&vmi, ਸ਼ੁਰੂ);
		vma = vma_ਅਗਲਾ(&vmi);
	}

	ਲਈ (; vma; vma = vma_ਅਗਲਾ(&vmi)) {
		ਜੇ (!vma_migratable(vma) || !vma_policy_mof(vma) ||
			is_vm_hugetlb_ਪੰਨਾ(vma) || (vma->vm_flags & VM_MIXEDMAP)) {
			trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_UNSUITABLE);
			ਜਾਰੀ;
		}

		/*
		 * Shared library pages mapped by multiple processes are not
		 * migrated as it is expected they are cache replicated. Avoid
		 * hinting faults in read-only file-backed mappings or the vDSO
		 * as migrating the pages will be of marginal benefit.
		 */
		ਜੇ (!vma->vm_mm ||
		    (vma->vm_file && (vma->vm_flags & (VM_READ|VM_WRITE)) == (VM_READ))) {
			trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_SHARED_RO);
			ਜਾਰੀ;
		}

		/*
		 * Skip inaccessible VMAs to avoid any confusion between
		 * PROT_NONE and NUMA hinting PTEs
		 */
		ਜੇ (!vma_is_accessible(vma)) {
			trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_INACCESSIBLE);
			ਜਾਰੀ;
		}

		/* Initialise new per-VMA NUMAB state. */
		ਜੇ (!vma->numab_state) {
			ਬਣਤਰ vma_numab_state *ptr;

			ptr = kzalloc_obj(*ptr);
			ਜੇ (!ptr)
				ਜਾਰੀ;

			ਜੇ (cmpxchg(&vma->numab_state, ਨਿਰਾ, ptr)) {
				kfree(ptr);
				ਜਾਰੀ;
			}

			vma->numab_state->ਸ਼ੁਰੂ_ਜਾਂਚ_seq = mm->numa_ਜਾਂਚ_seq;

			vma->numab_state->ਅਗਲਾ_ਜਾਂਚ = now +
				msecs_ਮੰਜ਼ਿਲ_jiffies(sysctl_numa_balancing_ਜਾਂਚ_delay);

			/* Reset happens after 4 times scan delay of scan start */
			vma->numab_state->pids_ਸਰਗਰਮ_reset =  vma->numab_state->ਅਗਲਾ_ਜਾਂਚ +
				msecs_ਮੰਜ਼ਿਲ_jiffies(VMA_PID_RESET_PERIOD);

			/*
			 * Ensure prev_scan_seq does not match numa_scan_seq,
			 * to prevent VMAs being skipped prematurely on the
			 * first scan:
			 */
			 vma->numab_state->prev_ਜਾਂਚ_seq = mm->numa_ਜਾਂਚ_seq - 1;
		}

		/*
		 * Scanning the VMAs of short lived tasks add more overhead. So
		 * delay the scan for new VMAs.
		 */
		ਜੇ (mm->numa_ਜਾਂਚ_seq && ਸਮਾਂ_before(jiffies,
						vma->numab_state->ਅਗਲਾ_ਜਾਂਚ)) {
			trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_ਜਾਂਚ_DELAY);
			ਜਾਰੀ;
		}

		/* RESET access PIDs regularly for old VMAs. */
		ਜੇ (mm->numa_ਜਾਂਚ_seq &&
				ਸਮਾਂ_after(jiffies, vma->numab_state->pids_ਸਰਗਰਮ_reset)) {
			vma->numab_state->pids_ਸਰਗਰਮ_reset = vma->numab_state->pids_ਸਰਗਰਮ_reset +
				msecs_ਮੰਜ਼ਿਲ_jiffies(VMA_PID_RESET_PERIOD);
			vma->numab_state->pids_ਸਰਗਰਮ[0] = READ_ONCE(vma->numab_state->pids_ਸਰਗਰਮ[1]);
			vma->numab_state->pids_ਸਰਗਰਮ[1] = 0;
		}

		/* Do not rescan VMAs twice within the same sequence. */
		ਜੇ (vma->numab_state->prev_ਜਾਂਚ_seq == mm->numa_ਜਾਂਚ_seq) {
			mm->numa_ਜਾਂਚ_ਵਿੱਥ = vma->vm_ਸਿਰਾ;
			trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_SEQ_COMPLETED);
			ਜਾਰੀ;
		}

		/*
		 * Do not scan the VMA if task has not accessed it, unless no other
		 * VMA candidate exists.
		 */
		ਜੇ (!vma_pids_forced && !vma_is_accessed(mm, vma)) {
			vma_pids_skipped = true;
			trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_PID_INACTIVE);
			ਜਾਰੀ;
		}

		ਕਰੋ {
			ਸ਼ੁਰੂ = max(ਸ਼ੁਰੂ, vma->vm_ਸ਼ੁਰੂ);
			ਸਿਰਾ = ALIGN(ਸ਼ੁਰੂ + (ਪੰਨੇ << ਪੰਨਾ_ਖਿਸਕਾਓ), HPAGE_ਮਾਪ);
			ਸਿਰਾ = min(ਸਿਰਾ, vma->vm_ਸਿਰਾ);
			nr_pte_updates = change_prot_numa(vma, ਸ਼ੁਰੂ, ਸਿਰਾ);

			/*
			 * Try to scan sysctl_numa_balancing_size worth of
			 * hpages that have at least one present PTE that
			 * is not already PTE-numa. If the VMA contains
			 * areas that are unused or already full of prot_numa
			 * PTEs, scan up to virtpages, to skip through those
			 * areas faster.
			 */
			ਜੇ (nr_pte_updates)
				ਪੰਨੇ -= (ਸਿਰਾ - ਸ਼ੁਰੂ) >> ਪੰਨਾ_ਖਿਸਕਾਓ;
			virtpages -= (ਸਿਰਾ - ਸ਼ੁਰੂ) >> ਪੰਨਾ_ਖਿਸਕਾਓ;

			ਸ਼ੁਰੂ = ਸਿਰਾ;
			ਜੇ (ਪੰਨੇ <= 0 || virtpages <= 0)
				ਜਾਓ ਬਾਹਰਲਾ;

			cond_resched();
		} ਜਦੋਂ (ਸਿਰਾ != vma->vm_ਸਿਰਾ);

		/* VMA scan is complete, do not scan until next sequence. */
		vma->numab_state->prev_ਜਾਂਚ_seq = mm->numa_ਜਾਂਚ_seq;

		/*
		 * Only force scan within one VMA at a time, to limit the
		 * cost of scanning a potentially uninteresting VMA.
		 */
		ਜੇ (vma_pids_forced)
			ਰੋਕੋ;
	}

	/*
	 * If no VMAs are remaining and VMAs were skipped due to the PID
	 * not accessing the VMA previously, then force a scan to ensure
	 * forward progress:
	 */
	ਜੇ (!vma && !vma_pids_forced && vma_pids_skipped) {
		vma_pids_forced = true;
		ਜਾਓ retry_pids;
	}

ਬਾਹਰਲਾ:
	/*
	 * It is possible to reach the end of the VMA list but the last few
	 * VMAs are not guaranteed to the vma_migratable. If they are not, we
	 * would find the !migratable VMA on the next scan but not reset the
	 * scanner to the start so check it now.
	 */
	ਜੇ (vma)
		mm->numa_ਜਾਂਚ_ਵਿੱਥ = ਸ਼ੁਰੂ;
	ਨਹੀਂਤਾਂ
		reset_ptenuma_ਜਾਂਚ(p);
	mmap_read_unlock(mm);

	/*
	 * Make sure tasks use at least 32x as much time to run other code
	 * than they used here, to limit NUMA PTE scanning overhead to 3% max.
	 * Usually update_task_scan_period slows down scanning enough; on an
	 * overloaded system we need to limit overhead on a per task basis.
	 */
	ਜੇ (unlikely(p->se.ਜੋੜ_exec_runtime != runtime)) {
		u64 diff = p->se.ਜੋੜ_exec_runtime - runtime;
		p->ਗੰਢ_stamp += 32 * diff;
	}
}

ਖਾਲੀ init_numa_balancing(u64 clone_flags, ਬਣਤਰ task_struct *p)
{
	ਪੂਰਨ mm_users = 0;
	ਬਣਤਰ mm_struct *mm = p->mm;

	ਜੇ (mm) {
		mm_users = atomic_read(&mm->mm_users);
		ਜੇ (mm_users == 1) {
			mm->numa_ਅਗਲਾ_ਜਾਂਚ = jiffies + msecs_ਮੰਜ਼ਿਲ_jiffies(sysctl_numa_balancing_ਜਾਂਚ_delay);
			mm->numa_ਜਾਂਚ_seq = 0;
		}
	}
	p->ਗੰਢ_stamp			= 0;
	p->numa_ਜਾਂਚ_seq		= mm ? mm->numa_ਜਾਂਚ_seq : 0;
	p->numa_ਜਾਂਚ_period		= sysctl_numa_balancing_ਜਾਂਚ_delay;
	p->numa_migrate_retry		= 0;
	/* Protect against double add, see task_tick_numa and task_numa_work */
	p->numa_work.ਅਗਲਾ		= &p->numa_work;
	p->numa_ਨੁਕਸ			= ਨਿਰਾ;
	p->numa_ਪੰਨੇ_migrated		= 0;
	p->ਕੁੱਲ_numa_ਨੁਕਸ		= 0;
	RCU_INIT_ਸੂਚਕ(p->numa_ਸਮੂਹ, ਨਿਰਾ);
	p->ਆਖਰੀ_task_numa_placement	= 0;
	p->ਆਖਰੀ_ਜੋੜ_exec_runtime	= 0;

	init_task_work(&p->numa_work, task_numa_work);

	/* New address space, reset the preferred nid */
	ਜੇ (!(clone_flags & CLONE_VM)) {
		p->numa_preferred_nid = NUMA_NO_ਗੰਢ;
		ਵਾਪਸ;
	}

	/*
	 * New thread, keep existing numa_preferred_nid which should be copied
	 * already by arch_dup_task_struct but stagger when scans start.
	 */
	ਜੇ (mm) {
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ delay;

		delay = min_t(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ, task_ਜਾਂਚ_max(current),
			current->numa_ਜਾਂਚ_period * mm_users * NSEC_ਪ੍ਰਤੀ_MSEC);
		delay += 2 * TICK_NSEC;
		p->ਗੰਢ_stamp = delay;
	}
}

/*
 * Drive the periodic memory faults..
 */
ਸਥਿਰ ਖਾਲੀ task_tick_numa(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *curr)
{
	ਬਣਤਰ callback_ਸਿਰ *work = &curr->numa_work;
	u64 period, now;

	/*
	 * We don't care about NUMA placement if we don't have memory.
	 */
	ਜੇ (!curr->mm || (curr->flags & (PF_EXITING | PF_KTHREAD)) || work->ਅਗਲਾ != work)
		ਵਾਪਸ;

	/*
	 * Using runtime rather than walltime has the dual advantage that
	 * we (mostly) drive the selection from busy threads and that the
	 * task needs to have done some actual work before we bother with
	 * NUMA placement.
	 */
	now = curr->se.ਜੋੜ_exec_runtime;
	period = (u64)curr->numa_ਜਾਂਚ_period * NSEC_ਪ੍ਰਤੀ_MSEC;

	ਜੇ (now > curr->ਗੰਢ_stamp + period) {
		ਜੇ (!curr->ਗੰਢ_stamp)
			curr->numa_ਜਾਂਚ_period = task_ਜਾਂਚ_ਸ਼ੁਰੂ(curr);
		curr->ਗੰਢ_stamp += period;

		ਜੇ (!ਸਮਾਂ_before(jiffies, curr->mm->numa_ਅਗਲਾ_ਜਾਂਚ))
			task_work_ਜੋੜੋ(curr, work, TWA_RESUME);
	}
}

ਸਥਿਰ ਖਾਲੀ update_ਜਾਂਚ_period(ਬਣਤਰ task_struct *p, ਪੂਰਨ new_cpu)
{
	ਪੂਰਨ src_nid = cpu_ਮੰਜ਼ਿਲ_ਗੰਢ(task_cpu(p));
	ਪੂਰਨ dst_nid = cpu_ਮੰਜ਼ਿਲ_ਗੰਢ(new_cpu);

	ਜੇ (!static_branch_likely(&sched_numa_balancing))
		ਵਾਪਸ;

	ਜੇ (!p->mm || !p->numa_ਨੁਕਸ || (p->flags & PF_EXITING))
		ਵਾਪਸ;

	ਜੇ (src_nid == dst_nid)
		ਵਾਪਸ;

	/*
	 * Allow resets if faults have been trapped before one scan
	 * has completed. This is most likely due to a new task that
	 * is pulled cross-node due to wakeups or load balancing.
	 */
	ਜੇ (p->numa_ਜਾਂਚ_seq) {
		/*
		 * Avoid scan adjustments if moving to the preferred
		 * node or if the task was not previously running on
		 * the preferred node.
		 */
		ਜੇ (dst_nid == p->numa_preferred_nid ||
		    (p->numa_preferred_nid != NUMA_NO_ਗੰਢ &&
			src_nid != p->numa_preferred_nid))
			ਵਾਪਸ;
	}

	p->numa_ਜਾਂਚ_period = task_ਜਾਂਚ_ਸ਼ੁਰੂ(p);
}

#ਨਹੀਂਤਾਂ /* !CONFIG_NUMA_BALANCING: */

ਸਥਿਰ ਖਾਲੀ task_tick_numa(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *curr)
{
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ account_numa_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p)
{
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ account_numa_ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p)
{
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ update_ਜਾਂਚ_period(ਬਣਤਰ task_struct *p, ਪੂਰਨ new_cpu)
{
}

#ਜੇ_ਅੰਤ /* !CONFIG_NUMA_BALANCING */

ਸਥਿਰ ਖਾਲੀ
account_entity_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ(ਬਣਤਰ cfs_rq *cfs_rq, ਬਣਤਰ sched_entity *se)
{
	WARN_ON_ONCE(cfs_rq != cfs_rq_ਦਾ(se));
	update_ਲੱਦੋ_ਜੋੜੋ(&cfs_rq->ਲੱਦੋ, se->ਲੱਦੋ.ਭਾਰ);
	ਜੇ (entity_is_task(se)) {
		ਬਣਤਰ task_struct *p = task_ਦਾ(se);
		ਬਣਤਰ rq *rq = rq_ਦਾ(cfs_rq);

		account_numa_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ(rq, p);
		account_llc_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ(rq, p);
		list_ਜੋੜੋ(&se->ਸਮੂਹ_ਗੰਢ, &rq->cfs_tasks);
	}
	cfs_rq->nr_queued++;
}

ਸਥਿਰ ਖਾਲੀ
account_entity_ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ(ਬਣਤਰ cfs_rq *cfs_rq, ਬਣਤਰ sched_entity *se)
{
	WARN_ON_ONCE(cfs_rq != cfs_rq_ਦਾ(se));
	update_ਲੱਦੋ_sub(&cfs_rq->ਲੱਦੋ, se->ਲੱਦੋ.ਭਾਰ);
	ਜੇ (entity_is_task(se)) {
		ਬਣਤਰ task_struct *p = task_ਦਾ(se);
		ਬਣਤਰ rq *rq = rq_ਦਾ(cfs_rq);

		account_numa_ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ(rq, p);
		account_llc_ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ(rq, p);
		list_del_init(&se->ਸਮੂਹ_ਗੰਢ);
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
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਜੋੜੋ_ਸਕਾਰਾਤਮਕ(_ptr, _val) ਕਰੋ {                           \
	typeof(_ptr) ptr = (_ptr);                              \
	__signed_scalar_typeof(*ptr) val = (_val);              \
	typeof(*ptr) res, var = READ_ONCE(*ptr);                \
								\
	res = var + val;                                        \
								\
	ਜੇ (val < 0 && res > var)                               \
		res = 0;                                        \
								\
	WRITE_ONCE(*ptr, res);                                  \
} ਜਦੋਂ (0)

/*
 * Remove and clamp on negative, from a local variable.
 *
 * A variant of sub_positive(), which does not use explicit load-store
 * and is thus optimized for local variable updates.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ lsub_ਸਕਾਰਾਤਮਕ(_ptr, _val) ਕਰੋ {				\
	typeof(_ptr) ptr = (_ptr);				\
	*ptr -= min_t(typeof(*ptr), *ptr, _val);		\
} ਜਦੋਂ (0)


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
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __update_sa(sa, name, delta_avg, delta_ਜੋੜ) ਕਰੋ {	\
	ਜੋੜੋ_ਸਕਾਰਾਤਮਕ(&(sa)->name##_avg, delta_avg);		\
	ਜੋੜੋ_ਸਕਾਰਾਤਮਕ(&(sa)->name##_sum, delta_ਜੋੜ);		\
	(sa)->name##_sum = max_t(typeof((sa)->name##_sum),	\
			       (sa)->name##_sum,		\
			       (sa)->name##_avg * PELT_MIN_DIVIDER); \
} ਜਦੋਂ (0)

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_ਲੱਦੋ_avg(ਬਣਤਰ cfs_rq *cfs_rq, ਬਣਤਰ sched_entity *se)
{
	__update_sa(&cfs_rq->avg, ਲੱਦੋ, se->avg.ਲੱਦੋ_avg,
		    se_ਭਾਰ(se) * se->avg.ਲੱਦੋ_ਜੋੜ);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_ਲੱਦੋ_avg(ਬਣਤਰ cfs_rq *cfs_rq, ਬਣਤਰ sched_entity *se)
{
	__update_sa(&cfs_rq->avg, ਲੱਦੋ, -se->avg.ਲੱਦੋ_avg,
		    se_ਭਾਰ(se) * -se->avg.ਲੱਦੋ_ਜੋੜ);
}

ਸਥਿਰ ਖਾਲੀ
rescale_entity(ਬਣਤਰ sched_entity *se, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਭਾਰ, bool rel_vprot)
{
	ਲੰਮਾ old_ਭਾਰ = se->h_ਲੱਦੋ.ਭਾਰ;

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
	se->vlag = div64_long(se->vlag * old_ਭਾਰ, ਭਾਰ);

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
	ਜੇ (se->rel_deadline)
		se->deadline = div64_long(se->deadline * old_ਭਾਰ, ਭਾਰ);

	ਜੇ (rel_vprot)
		se->vprot = div64_long(se->vprot * old_ਭਾਰ, ਭਾਰ);
}

ਸਥਿਰ ਖਾਲੀ reweight_eevdf(ਬਣਤਰ cfs_rq *cfs_rq, ਬਣਤਰ sched_entity *se,
			   ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਭਾਰ, bool on_rq)
{
	bool curr = cfs_rq->curr == se;
	bool rel_vprot = false;
	u64 avruntime = 0;

	ਜੇ (se->h_ਲੱਦੋ.ਭਾਰ == ਭਾਰ)
		ਵਾਪਸ;

	ਜੇ (on_rq) {
		avruntime = avg_vruntime(cfs_rq);
		se->vlag = entity_lag(cfs_rq, se, avruntime);
		se->deadline -= avruntime;
		se->rel_deadline = 1;
		ਜੇ (curr && protect_ਟੁਕੜਾ(se)) {
			se->vprot -= avruntime;
			rel_vprot = true;
		}

		cfs_rq->h_nr_queued--;
		ਜੇ (!curr)
			__dequeue_entity(cfs_rq, se);
	}

	rescale_entity(se, ਭਾਰ, rel_vprot);

	update_ਲੱਦੋ_ਸੈੱਟ(&se->h_ਲੱਦੋ, ਭਾਰ);

	ਜੇ (on_rq) {
		ਜੇ (rel_vprot)
			se->vprot += avruntime;
		se->deadline += avruntime;
		se->rel_deadline = 0;
		se->vruntime = avruntime - se->vlag;

		ਜੇ (!curr)
			__enqueue_entity(cfs_rq, se);
		cfs_rq->h_nr_queued++;
	}
}

ਸਥਿਰ ਖਾਲੀ reweight_entity(ਬਣਤਰ cfs_rq *cfs_rq, ਬਣਤਰ sched_entity *se,
			    ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਭਾਰ)
{
	ਜੇ (se->ਲੱਦੋ.ਭਾਰ == ਭਾਰ)
		ਵਾਪਸ;

	ਜੇ (se->on_rq) {
		WARN_ON_ONCE(cfs_rq != cfs_rq_ਦਾ(se));
		update_ਲੱਦੋ_sub(&cfs_rq->ਲੱਦੋ, se->ਲੱਦੋ.ਭਾਰ);
	}
	ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_ਲੱਦੋ_avg(cfs_rq, se);

	update_ਲੱਦੋ_ਸੈੱਟ(&se->ਲੱਦੋ, ਭਾਰ);

	ਕਰੋ {
		u32 divider = get_pelt_divider(&se->avg);
		se->avg.ਲੱਦੋ_avg = div_u64(se_ਭਾਰ(se) * se->avg.ਲੱਦੋ_ਜੋੜ, divider);
	} ਜਦੋਂ (0);

	ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_ਲੱਦੋ_avg(cfs_rq, se);

	ਜੇ (se->on_rq)
		update_ਲੱਦੋ_ਜੋੜੋ(&cfs_rq->ਲੱਦੋ, se->ਲੱਦੋ.ਭਾਰ);
}

/*
 * weight = NICE_0_LOAD;
 * for_each_entity_se(se)
 *   weight = __calc_prop_weight(cfs_rq_of(se), se, weight);
 */
ਸਥਿਰ __always_inline
ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ __calc_prop_weight(ਬਣਤਰ cfs_rq *cfs_rq, ਬਣਤਰ sched_entity *se,
				 ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਭਾਰ)
{
	ਭਾਰ *= se->ਲੱਦੋ.ਭਾਰ;
	ਜੇ (ਮਾਪਾ_entity(se))
		ਭਾਰ /= cfs_rq->ਲੱਦੋ.ਭਾਰ;
	ਨਹੀਂਤਾਂ
		ਭਾਰ /= NICE_0_ਲੱਦੋ;

	ਵਾਪਸ max(ਭਾਰ, MIN_SHARES);
}

ਸਥਿਰ ਖਾਲੀ reweight_task_fair(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p,
			       ਅਚਲ ਬਣਤਰ ਲੱਦੋ_ਭਾਰ *lw)
{
	ਬਣਤਰ sched_entity *se = &p->se;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਭਾਰ = NICE_0_ਲੱਦੋ;

	ਜੇ (se->on_rq)
		update_curr_fair(rq);

	reweight_entity(cfs_rq_ਦਾ(se), se, lw->ਭਾਰ);
	se->ਲੱਦੋ.inv_ਭਾਰ = lw->inv_ਭਾਰ;

	ਜੇ (!se->on_rq)
		ਵਾਪਸ;

	for_each_sched_entity(se)
		ਭਾਰ = __calc_prop_weight(cfs_rq_ਦਾ(se), se, ਭਾਰ);

	reweight_eevdf(&rq->cfs, &p->se, ਭਾਰ, p->se.on_rq);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ throttled_hierarchy(ਬਣਤਰ cfs_rq *cfs_rq);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_FAIR_ਸਮੂਹ_SCHED
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
ਸਥਿਰ ਲੰਮਾ __calc_smp_shares(ਬਣਤਰ cfs_rq *cfs_rq, ਲੰਮਾ tg_shares, ਲੰਮਾ shares_max)
{
	ਬਣਤਰ task_ਸਮੂਹ *tg = cfs_rq->tg;
	ਲੰਮਾ tg_ਭਾਰ, ਲੱਦੋ, shares;

	ਲੱਦੋ = max(scale_ਲੱਦੋ_ਥੱਲੇ(cfs_rq->ਲੱਦੋ.ਭਾਰ), cfs_rq->avg.ਲੱਦੋ_avg);

	tg_ਭਾਰ = atomic_long_read(&tg->ਲੱਦੋ_avg);

	/* Ensure tg_weight >= load */
	tg_ਭਾਰ -= cfs_rq->tg_ਲੱਦੋ_avg_contrib;
	tg_ਭਾਰ += ਲੱਦੋ;

	shares = (tg_shares * ਲੱਦੋ);
	ਜੇ (tg_ਭਾਰ)
		shares /= tg_ਭਾਰ;

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
	ਵਾਪਸ clamp_t(ਲੰਮਾ, shares, MIN_SHARES, shares_max);
}

ਸਥਿਰ ਪੂਰਨ tg_cpus(ਬਣਤਰ task_ਸਮੂਹ *tg)
{
	ਪੂਰਨ nr = ਅੰਕ_online_cpus();

	ਜੇ (cpusets_enabled()) {
		ਬਣਤਰ cgroup *cgrp = tg->css.cgroup;
		ਜੇ (cgrp)
			nr = cpuset_ਅੰਕ_cpus(cgrp);
	}

	/*
	 * An empty cpuset would propagate a 0 shares_max into
	 * __calc_smp_shares(), where clamp() yields hi when hi < lo and so
	 * defeats the MIN_SHARES floor. Match tg_tasks(), which floors at 1.
	 */
	ਵਾਪਸ max(nr, 1);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ tg_tasks(ਬਣਤਰ task_ਸਮੂਹ *tg)
{
	ਵਾਪਸ max(1, atomic_long_read(&tg->runnable_avg) >> SCHED_ਸਮਰੱਥਾ_ਖਿਸਕਾਓ);
}

/*
 * Func: fraction(nr_tasks * tg->shares)
 *
 * Scale tg->shares by the number of tasks.
 */
ਸਥਿਰ ਲੰਮਾ calc_tasks_shares(ਬਣਤਰ cfs_rq *cfs_rq)
{
	ਬਣਤਰ task_ਸਮੂਹ *tg = cfs_rq->tg;
	ਪੂਰਨ nr = tg_tasks(tg);
	ਲੰਮਾ tg_shares = READ_ONCE(tg->shares);
	ਵਾਪਸ __calc_smp_shares(cfs_rq, nr * tg_shares, nr * tg_shares);
}

/*
 * Func: min(fraction(nr_cpus * tg->shares), nice -20)
 *
 * Scale tg->shares by the maximal number of CPUs; but clip the max shares at
 * nice -20, otherwise a single spinner on a 512 CPU machine would result in
 * 512*NICE_0_LOAD, which is also crazy.
 */
ਸਥਿਰ ਲੰਮਾ calc_max_shares(ਬਣਤਰ cfs_rq *cfs_rq)
{
	ਬਣਤਰ task_ਸਮੂਹ *tg = cfs_rq->tg;
	ਪੂਰਨ nr = tg_cpus(tg);
	ਲੰਮਾ tg_shares = READ_ONCE(tg->shares);
	ਲੰਮਾ max_shares = scale_ਲੱਦੋ(sched_prio_ਮੰਜ਼ਿਲ_ਭਾਰ[0]);
	ਵਾਪਸ __calc_smp_shares(cfs_rq, tg_shares * nr, max_shares);
}

/*
 * Func: fraction(nr * tg->shares); nr = min(nr_tasks, nr_cpus)
 *
 * Scales between "smp" and "max" in a natural way. No longer needs clipping
 * since there are no unnatural inflations like with "max".
 */
ਸਥਿਰ ਲੰਮਾ calc_concur_shares(ਬਣਤਰ cfs_rq *cfs_rq)
{
	ਬਣਤਰ task_ਸਮੂਹ *tg = cfs_rq->tg;
	ਪੂਰਨ nr = min(tg_tasks(tg), tg_cpus(tg));
	ਲੰਮਾ tg_shares = READ_ONCE(tg->shares);
	ਵਾਪਸ __calc_smp_shares(cfs_rq, nr * tg_shares, nr * tg_shares);
}

/*
 * Func: fraction(tg->shares)
 *
 * This infamously results in tiny shares when you have many CPUs.
 */
ਸਥਿਰ ਲੰਮਾ calc_smp_shares(ਬਣਤਰ cfs_rq *cfs_rq)
{
	ਬਣਤਰ task_ਸਮੂਹ *tg = cfs_rq->tg;
	ਲੰਮਾ tg_shares = READ_ONCE(tg->shares);
	ਵਾਪਸ __calc_smp_shares(cfs_rq, tg_shares, tg_shares);
}

/*
 * Ignore this pesky SMP stuff, use (4).
 */
ਸਥਿਰ ਲੰਮਾ calc_ਉੱਪਰ_shares(ਬਣਤਰ cfs_rq *cfs_rq)
{
	ਬਣਤਰ task_ਸਮੂਹ *tg = cfs_rq->tg;
	ਵਾਪਸ READ_ONCE(tg->shares);
}

DEFINE_STATIC_CALL(calc_ਸਮੂਹ_shares, calc_concur_shares);

ਖਾਲੀ __sched_cgroup_mode_update(ਪੂਰਨ ਬਹੁਲਕ)
{
	ਲੰਮਾ (*func)(ਬਣਤਰ cfs_rq *);
	ਚੋਣ (ਬਹੁਲਕ) {
	ਹਾਲਤ 0:
		func = &calc_ਉੱਪਰ_shares;
		ਰੋਕੋ;
	ਹਾਲਤ 1:
		func = &calc_smp_shares;
		ਰੋਕੋ;
	ਹਾਲਤ 2:
	ਮੂਲ:
		func = &calc_concur_shares;
		ਰੋਕੋ;
	ਹਾਲਤ 3:
		func = &calc_max_shares;
		ਰੋਕੋ;
	ਹਾਲਤ 4:
		func = &calc_tasks_shares;
		ਰੋਕੋ;
	}
	static_call_update(calc_ਸਮੂਹ_shares, func);
}

/*
 * Recomputes the group entity based on the current state of its group
 * runqueue.
 */
ਸਥਿਰ ਖਾਲੀ update_cfs_ਸਮੂਹ(ਬਣਤਰ sched_entity *se)
{
	ਬਣਤਰ cfs_rq *gcfs_rq = ਸਮੂਹ_cfs_rq(se);
	ਲੰਮਾ shares;

	/*
	 * When a group becomes empty, preserve its weight. This matters for
	 * DELAY_DEQUEUE.
	 */
	ਜੇ (!gcfs_rq || !gcfs_rq->ਲੱਦੋ.ਭਾਰ)
		ਵਾਪਸ;

	shares = static_call(calc_ਸਮੂਹ_shares)(gcfs_rq);
	reweight_entity(cfs_rq_ਦਾ(se), se, shares);
}

#ਨਹੀਂਤਾਂ /* !CONFIG_FAIR_GROUP_SCHED: */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ update_cfs_ਸਮੂਹ(ਬਣਤਰ sched_entity *se)
{
}
#ਜੇ_ਅੰਤ /* !CONFIG_FAIR_GROUP_SCHED */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ cfs_rq_util_change(ਬਣਤਰ cfs_rq *cfs_rq, ਪੂਰਨ flags)
{
	ਬਣਤਰ rq *rq = rq_ਦਾ(cfs_rq);

	ਜੇ (&rq->cfs == cfs_rq) {
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

ਸਥਿਰ ਸਤਰਅੰਦਰ bool ਲੱਦੋ_avg_is_decayed(ਬਣਤਰ sched_avg *sa)
{
	ਜੇ (sa->ਲੱਦੋ_ਜੋੜ)
		ਵਾਪਸ false;

	ਜੇ (sa->util_ਜੋੜ)
		ਵਾਪਸ false;

	ਜੇ (sa->runnable_ਜੋੜ)
		ਵਾਪਸ false;

	/*
	 * _avg must be null when _sum are null because _avg = _sum / divider
	 * Make sure that rounding and/or propagation of PELT values never
	 * break this.
	 */
	WARN_ON_ONCE(sa->ਲੱਦੋ_avg ||
		      sa->util_avg ||
		      sa->runnable_avg);

	ਵਾਪਸ true;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ u64 cfs_rq_ਆਖਰੀ_update_ਸਮਾਂ(ਬਣਤਰ cfs_rq *cfs_rq)
{
	ਵਾਪਸ u64_u32_ਲੱਦੋ_copy(cfs_rq->avg.ਆਖਰੀ_update_ਸਮਾਂ,
				 cfs_rq->ਆਖਰੀ_update_ਸਮਾਂ_copy);
}
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_FAIR_ਸਮੂਹ_SCHED
/*
 * Because list_add_leaf_cfs_rq always places a child cfs_rq on the list
 * immediately before a parent cfs_rq, and cfs_rqs are removed from the list
 * bottom-up, we only have to test whether the cfs_rq before us on the list
 * is our child.
 * If cfs_rq is not on the list, test whether a child needs its to be added to
 * connect a branch to the tree  * (see list_add_leaf_cfs_rq() for details).
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ bool ਬੱਚਾ_cfs_rq_on_list(ਬਣਤਰ cfs_rq *cfs_rq)
{
	ਬਣਤਰ cfs_rq *prev_cfs_rq;
	ਬਣਤਰ list_ਸਿਰ *prev;
	ਬਣਤਰ rq *rq = rq_ਦਾ(cfs_rq);

	ਜੇ (cfs_rq->on_list) {
		prev = cfs_rq->ਪੱਤੀ_cfs_rq_list.prev;
	} ਨਹੀਂਤਾਂ {
		prev = rq->tmp_alone_branch;
	}

	ਜੇ (prev == &rq->ਪੱਤੀ_cfs_rq_list)
		ਵਾਪਸ false;

	prev_cfs_rq = container_ਦਾ(prev, ਬਣਤਰ cfs_rq, ਪੱਤੀ_cfs_rq_list);

	ਵਾਪਸ (prev_cfs_rq->tg->ਮਾਪਾ == cfs_rq->tg);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool cfs_rq_is_decayed(ਬਣਤਰ cfs_rq *cfs_rq)
{
	ਜੇ (cfs_rq->ਲੱਦੋ.ਭਾਰ)
		ਵਾਪਸ false;

	ਜੇ (!ਲੱਦੋ_avg_is_decayed(&cfs_rq->avg))
		ਵਾਪਸ false;

	ਜੇ (ਬੱਚਾ_cfs_rq_on_list(cfs_rq))
		ਵਾਪਸ false;

	ਜੇ (cfs_rq->tg_ਲੱਦੋ_avg_contrib)
		ਵਾਪਸ false;

	ਵਾਪਸ true;
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
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ update_tg_ਲੱਦੋ_avg(ਬਣਤਰ cfs_rq *cfs_rq)
{
	ਲੰਮਾ dl, dr;
	u64 now;

	/*
	 * No need to update load_avg for root_task_group as it is not used.
	 */
	ਜੇ (cfs_rq->tg == &ਜੜ੍ਹ_task_ਸਮੂਹ)
		ਵਾਪਸ;

	/* rq has been offline and doesn't contribute to the share anymore: */
	ਜੇ (!cpu_ਸਰਗਰਮ(cpu_ਦਾ(rq_ਦਾ(cfs_rq))))
		ਵਾਪਸ;

	/*
	 * For migration heavy workloads, access to tg->load_avg can be
	 * unbound. Limit the update rate to at most once per ms.
	 */
	now = rq_clock(rq_ਦਾ(cfs_rq));
	ਜੇ (now - cfs_rq->ਆਖਰੀ_update_tg_ਲੱਦੋ_avg < NSEC_ਪ੍ਰਤੀ_MSEC)
		ਵਾਪਸ;

	dl = cfs_rq->avg.ਲੱਦੋ_avg - cfs_rq->tg_ਲੱਦੋ_avg_contrib;
	dr = cfs_rq->avg.runnable_avg - cfs_rq->tg_runnable_avg_contrib;
	ਜੇ (ਨਿਰਪੇਖ(dl) > cfs_rq->tg_ਲੱਦੋ_avg_contrib / 64 ||
	    ਨਿਰਪੇਖ(dr) > cfs_rq->tg_runnable_avg_contrib / 64) {
		atomic_long_ਜੋੜੋ(dl, &cfs_rq->tg->ਲੱਦੋ_avg);
		atomic_long_ਜੋੜੋ(dr, &cfs_rq->tg->runnable_avg);
		cfs_rq->tg_ਲੱਦੋ_avg_contrib = cfs_rq->avg.ਲੱਦੋ_avg;
		cfs_rq->tg_runnable_avg_contrib = cfs_rq->avg.runnable_avg;
		cfs_rq->ਆਖਰੀ_update_tg_ਲੱਦੋ_avg = now;
	}
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ clear_tg_ਲੱਦੋ_avg(ਬਣਤਰ cfs_rq *cfs_rq)
{
	ਲੰਮਾ dl, dr;
	u64 now;

	/*
	 * No need to update load_avg for root_task_group, as it is not used.
	 */
	ਜੇ (cfs_rq->tg == &ਜੜ੍ਹ_task_ਸਮੂਹ)
		ਵਾਪਸ;

	now = rq_clock(rq_ਦਾ(cfs_rq));
	dl = 0 - cfs_rq->tg_ਲੱਦੋ_avg_contrib;
	dr = 0 - cfs_rq->tg_runnable_avg_contrib;
	atomic_long_ਜੋੜੋ(dl, &cfs_rq->tg->ਲੱਦੋ_avg);
	atomic_long_ਜੋੜੋ(dr, &cfs_rq->tg->runnable_avg);
	cfs_rq->tg_ਲੱਦੋ_avg_contrib = 0;
	cfs_rq->tg_runnable_avg_contrib = 0;
	cfs_rq->ਆਖਰੀ_update_tg_ਲੱਦੋ_avg = now;
}

/* CPU offline callback: */
ਸਥਿਰ ਖਾਲੀ __maybe_unused clear_tg_offline_cfs_rqs(ਬਣਤਰ rq *rq)
{
	ਬਣਤਰ task_ਸਮੂਹ *tg;

	lockdep_assert_rq_held(rq);

	/*
	 * The rq clock has already been updated in
	 * set_rq_offline(), so we should skip updating
	 * the rq clock again in unthrottle_cfs_rq().
	 */
	rq_clock_ਸ਼ੁਰੂ_loop_update(rq);

	guard(rcu)();

	list_for_each_ਇੰਦਰਾਜ_rcu(tg, &task_groups, list) {
		ਬਣਤਰ cfs_rq *cfs_rq = tg_cfs_rq(tg, cpu_ਦਾ(rq));

		clear_tg_ਲੱਦੋ_avg(cfs_rq);
	}

	rq_clock_stop_loop_update(rq);
}

/*
 * Called within set_task_rq() right before setting a task's CPU. The
 * caller only guarantees p->pi_lock is held; no other assumptions,
 * including the state of rq->lock, should be made.
 */
ਖਾਲੀ ਸੈੱਟ_task_rq_fair(ਬਣਤਰ sched_entity *se,
		      ਬਣਤਰ cfs_rq *prev, ਬਣਤਰ cfs_rq *ਅਗਲਾ)
{
	u64 p_ਆਖਰੀ_update_ਸਮਾਂ;
	u64 n_ਆਖਰੀ_update_ਸਮਾਂ;

	ਜੇ (!sched_feat(ATTACH_AGE_ਲੱਦੋ))
		ਵਾਪਸ;

	/*
	 * We are supposed to update the task to "current" time, then its up to
	 * date and ready to go to new CPU/cfs_rq. But we have difficulty in
	 * getting what current time is, so simply throw away the out-of-date
	 * time. This will result in the wakee task is less decayed, but giving
	 * the wakee more load sounds not bad.
	 */
	ਜੇ (!(se->avg.ਆਖਰੀ_update_ਸਮਾਂ && prev))
		ਵਾਪਸ;

	p_ਆਖਰੀ_update_ਸਮਾਂ = cfs_rq_ਆਖਰੀ_update_ਸਮਾਂ(prev);
	n_ਆਖਰੀ_update_ਸਮਾਂ = cfs_rq_ਆਖਰੀ_update_ਸਮਾਂ(ਅਗਲਾ);

	__update_load_avg_blocked_se(p_ਆਖਰੀ_update_ਸਮਾਂ, se);
	se->avg.ਆਖਰੀ_update_ਸਮਾਂ = n_ਆਖਰੀ_update_ਸਮਾਂ;
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
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
update_tg_cfs_util(ਬਣਤਰ cfs_rq *cfs_rq, ਬਣਤਰ sched_entity *se, ਬਣਤਰ cfs_rq *gcfs_rq)
{
	ਲੰਮਾ delta_ਜੋੜ, delta_avg = gcfs_rq->avg.util_avg - se->avg.util_avg;
	u32 new_ਜੋੜ, divider;

	/* Nothing to update */
	ਜੇ (!delta_avg)
		ਵਾਪਸ;

	/*
	 * cfs_rq->avg.period_contrib can be used for both cfs_rq and se.
	 * See ___update_load_avg() for details.
	 */
	divider = get_pelt_divider(&cfs_rq->avg);

	/* Set new sched_entity's utilization */
	se->avg.util_avg = gcfs_rq->avg.util_avg;
	new_ਜੋੜ = se->avg.util_avg * divider;
	delta_ਜੋੜ = (ਲੰਮਾ)new_ਜੋੜ - (ਲੰਮਾ)se->avg.util_ਜੋੜ;
	se->avg.util_ਜੋੜ = new_ਜੋੜ;

	/* Update parent cfs_rq utilization */
	__update_sa(&cfs_rq->avg, util, delta_avg, delta_ਜੋੜ);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
update_tg_cfs_runnable(ਬਣਤਰ cfs_rq *cfs_rq, ਬਣਤਰ sched_entity *se, ਬਣਤਰ cfs_rq *gcfs_rq)
{
	ਲੰਮਾ delta_ਜੋੜ, delta_avg = gcfs_rq->avg.runnable_avg - se->avg.runnable_avg;
	u64 new_ਜੋੜ;
	u32 divider;

	/* Nothing to update */
	ਜੇ (!delta_avg)
		ਵਾਪਸ;

	/*
	 * cfs_rq->avg.period_contrib can be used for both cfs_rq and se.
	 * See ___update_load_avg() for details.
	 */
	divider = get_pelt_divider(&cfs_rq->avg);

	/* Set new sched_entity's runnable */
	se->avg.runnable_avg = gcfs_rq->avg.runnable_avg;
	new_ਜੋੜ = (u64)se->avg.runnable_avg * divider;
	delta_ਜੋੜ = (ਲੰਮਾ)new_ਜੋੜ - (ਲੰਮਾ)se->avg.runnable_ਜੋੜ;
	se->avg.runnable_ਜੋੜ = new_ਜੋੜ;

	/* Update parent cfs_rq runnable */
	__update_sa(&cfs_rq->avg, runnable, delta_avg, delta_ਜੋੜ);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
update_tg_cfs_ਲੱਦੋ(ਬਣਤਰ cfs_rq *cfs_rq, ਬਣਤਰ sched_entity *se, ਬਣਤਰ cfs_rq *gcfs_rq)
{
	ਲੰਮਾ delta_avg, running_ਜੋੜ, runnable_ਜੋੜ = gcfs_rq->prop_runnable_ਜੋੜ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੱਦੋ_avg;
	u64 ਲੱਦੋ_ਜੋੜ = 0;
	s64 delta_ਜੋੜ;
	u32 divider;

	ਜੇ (!runnable_ਜੋੜ)
		ਵਾਪਸ;

	gcfs_rq->prop_runnable_ਜੋੜ = 0;

	/*
	 * cfs_rq->avg.period_contrib can be used for both cfs_rq and se.
	 * See ___update_load_avg() for details.
	 */
	divider = get_pelt_divider(&cfs_rq->avg);

	ਜੇ (runnable_ਜੋੜ >= 0) {
		/*
		 * Add runnable; clip at LOAD_AVG_MAX. Reflects that until
		 * the CPU is saturated running == runnable.
		 */
		runnable_ਜੋੜ += se->avg.ਲੱਦੋ_ਜੋੜ;
		runnable_ਜੋੜ = min_t(ਲੰਮਾ, runnable_ਜੋੜ, divider);
	} ਨਹੀਂਤਾਂ {
		/*
		 * Estimate the new unweighted runnable_sum of the gcfs_rq by
		 * assuming all tasks are equally runnable.
		 */
		ਜੇ (scale_ਲੱਦੋ_ਥੱਲੇ(gcfs_rq->ਲੱਦੋ.ਭਾਰ)) {
			ਲੱਦੋ_ਜੋੜ = div_u64(gcfs_rq->avg.ਲੱਦੋ_ਜੋੜ,
				scale_ਲੱਦੋ_ਥੱਲੇ(gcfs_rq->ਲੱਦੋ.ਭਾਰ));
		}

		/* But make sure to not inflate se's runnable */
		runnable_ਜੋੜ = min(se->avg.ਲੱਦੋ_ਜੋੜ, ਲੱਦੋ_ਜੋੜ);
	}

	/*
	 * runnable_sum can't be lower than running_sum
	 * Rescale running sum to be in the same range as runnable sum
	 * running_sum is in [0 : LOAD_AVG_MAX <<  SCHED_CAPACITY_SHIFT]
	 * runnable_sum is in [0 : LOAD_AVG_MAX]
	 */
	running_ਜੋੜ = se->avg.util_ਜੋੜ >> SCHED_ਸਮਰੱਥਾ_ਖਿਸਕਾਓ;
	runnable_ਜੋੜ = max(runnable_ਜੋੜ, running_ਜੋੜ);

	ਲੱਦੋ_ਜੋੜ = se_ਭਾਰ(se) * runnable_ਜੋੜ;
	ਲੱਦੋ_avg = div_u64(ਲੱਦੋ_ਜੋੜ, divider);

	delta_avg = ਲੱਦੋ_avg - se->avg.ਲੱਦੋ_avg;
	ਜੇ (!delta_avg)
		ਵਾਪਸ;

	delta_ਜੋੜ = ਲੱਦੋ_ਜੋੜ - (s64)se_ਭਾਰ(se) * se->avg.ਲੱਦੋ_ਜੋੜ;

	se->avg.ਲੱਦੋ_ਜੋੜ = runnable_ਜੋੜ;
	se->avg.ਲੱਦੋ_avg = ਲੱਦੋ_avg;
	__update_sa(&cfs_rq->avg, ਲੱਦੋ, delta_avg, delta_ਜੋੜ);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ ਜੋੜੋ_tg_cfs_propagate(ਬਣਤਰ cfs_rq *cfs_rq, ਲੰਮਾ runnable_ਜੋੜ)
{
	cfs_rq->propagate = 1;
	cfs_rq->prop_runnable_ਜੋੜ += runnable_ਜੋੜ;
}

/* Update task and its cfs_rq load average */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ propagate_entity_ਲੱਦੋ_avg(ਬਣਤਰ sched_entity *se)
{
	ਬਣਤਰ cfs_rq *cfs_rq, *gcfs_rq;

	ਜੇ (entity_is_task(se))
		ਵਾਪਸ 0;

	gcfs_rq = ਸਮੂਹ_cfs_rq(se);
	ਜੇ (!gcfs_rq->propagate)
		ਵਾਪਸ 0;

	gcfs_rq->propagate = 0;

	cfs_rq = cfs_rq_ਦਾ(se);

	ਜੋੜੋ_tg_cfs_propagate(cfs_rq, gcfs_rq->prop_runnable_ਜੋੜ);

	update_tg_cfs_util(cfs_rq, se, gcfs_rq);
	update_tg_cfs_runnable(cfs_rq, se, gcfs_rq);
	update_tg_cfs_ਲੱਦੋ(cfs_rq, se, gcfs_rq);

	trace_pelt_cfs_tp(cfs_rq);
	trace_pelt_se_tp(se);

	ਵਾਪਸ 1;
}

/*
 * Check if we need to update the load and the utilization of a blocked
 * group_entity:
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ bool skip_blocked_update(ਬਣਤਰ sched_entity *se)
{
	ਬਣਤਰ cfs_rq *gcfs_rq = ਸਮੂਹ_cfs_rq(se);

	/*
	 * If sched_entity still have not zero load or utilization, we have to
	 * decay it:
	 */
	ਜੇ (se->avg.ਲੱਦੋ_avg || se->avg.util_avg)
		ਵਾਪਸ false;

	/*
	 * If there is a pending propagation, we have to update the load and
	 * the utilization of the sched_entity:
	 */
	ਜੇ (gcfs_rq->propagate)
		ਵਾਪਸ false;

	/*
	 * Otherwise, the load and the utilization of the sched_entity is
	 * already zero and there is no pending propagation, so it will be a
	 * waste of time to try to decay it:
	 */
	ਵਾਪਸ true;
}

#ਨਹੀਂਤਾਂ /* !CONFIG_FAIR_GROUP_SCHED: */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ update_tg_ਲੱਦੋ_avg(ਬਣਤਰ cfs_rq *cfs_rq) {}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ clear_tg_offline_cfs_rqs(ਬਣਤਰ rq *rq) {}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ propagate_entity_ਲੱਦੋ_avg(ਬਣਤਰ sched_entity *se)
{
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ ਜੋੜੋ_tg_cfs_propagate(ਬਣਤਰ cfs_rq *cfs_rq, ਲੰਮਾ runnable_ਜੋੜ) {}

#ਜੇ_ਅੰਤ /* !CONFIG_FAIR_GROUP_SCHED */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_NO_HZ_COMMON
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ migrate_se_pelt_lag(ਬਣਤਰ sched_entity *se)
{
	u64 throttled = 0, now, lut;
	ਬਣਤਰ cfs_rq *cfs_rq;
	ਬਣਤਰ rq *rq;
	bool is_idle;

	ਜੇ (ਲੱਦੋ_avg_is_decayed(&se->avg))
		ਵਾਪਸ;

	cfs_rq = cfs_rq_ਦਾ(se);
	rq = rq_ਦਾ(cfs_rq);

	rcu_read_ਤਾਲਾ();
	is_idle = is_idle_task(rcu_dereference_all(rq->curr));
	rcu_read_unlock();

	/*
	 * The lag estimation comes with a cost we don't want to pay all the
	 * time. Hence, limiting to the case where the source CPU is idle and
	 * we know we are at the greatest risk to have an outdated clock.
	 */
	ਜੇ (!is_idle)
		ਵਾਪਸ;

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

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_CFS_BANDWIDTH
	throttled = u64_u32_ਲੱਦੋ(cfs_rq->throttled_pelt_idle);
	/* The clock has been stopped for throttling */
	ਜੇ (throttled == U64_MAX)
		ਵਾਪਸ;
#ਜੇ_ਅੰਤ
	now = u64_u32_ਲੱਦੋ(rq->clock_pelt_idle);
	/*
	 * Paired with _update_idle_rq_clock_pelt(). It ensures at the worst case
	 * is observed the old clock_pelt_idle value and the new clock_idle,
	 * which lead to an underestimation. The opposite would lead to an
	 * overestimation.
	 */
	smp_rmb();
	lut = cfs_rq_ਆਖਰੀ_update_ਸਮਾਂ(cfs_rq);

	now -= throttled;
	ਜੇ (now < lut)
		/*
		 * cfs_rq->avg.last_update_time is more recent than our
		 * estimation, let's use it.
		 */
		now = lut;
	ਨਹੀਂਤਾਂ
		now += sched_clock_cpu(cpu_ਦਾ(rq)) - u64_u32_ਲੱਦੋ(rq->clock_idle);

	__update_load_avg_blocked_se(now, se);
}
#ਨਹੀਂਤਾਂ /* !CONFIG_NO_HZ_COMMON: */
ਸਥਿਰ ਖਾਲੀ migrate_se_pelt_lag(ਬਣਤਰ sched_entity *se) {}
#ਜੇ_ਅੰਤ /* !CONFIG_NO_HZ_COMMON */

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
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
update_cfs_rq_ਲੱਦੋ_avg(u64 now, ਬਣਤਰ cfs_rq *cfs_rq)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ removed_ਲੱਦੋ = 0, removed_util = 0, removed_runnable = 0;
	ਬਣਤਰ sched_avg *sa = &cfs_rq->avg;
	ਪੂਰਨ decayed = 0;

	ਜੇ (cfs_rq->removed.nr) {
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ r;
		u32 divider = get_pelt_divider(&cfs_rq->avg);

		raw_spin_ਤਾਲਾ(&cfs_rq->removed.ਤਾਲਾ);
		ਅਦਲਾ_ਬਦਲੀ(cfs_rq->removed.util_avg, removed_util);
		ਅਦਲਾ_ਬਦਲੀ(cfs_rq->removed.ਲੱਦੋ_avg, removed_ਲੱਦੋ);
		ਅਦਲਾ_ਬਦਲੀ(cfs_rq->removed.runnable_avg, removed_runnable);
		cfs_rq->removed.nr = 0;
		raw_spin_unlock(&cfs_rq->removed.ਤਾਲਾ);

		r = removed_ਲੱਦੋ;
		__update_sa(sa, ਲੱਦੋ, -r, -r*divider);

		r = removed_util;
		__update_sa(sa, util, -r, -r*divider);

		r = removed_runnable;
		__update_sa(sa, runnable, -r, -r*divider);

		/*
		 * removed_runnable is the unweighted version of removed_load so we
		 * can use it to estimate removed_load_sum.
		 */
		ਜੋੜੋ_tg_cfs_propagate(cfs_rq,
			-(ਲੰਮਾ)(removed_runnable * divider) >> SCHED_ਸਮਰੱਥਾ_ਖਿਸਕਾਓ);

		decayed = 1;
	}

	decayed |= __update_load_avg_cfs_rq(now, cfs_rq);
	u64_u32_store_copy(sa->ਆਖਰੀ_update_ਸਮਾਂ,
			   cfs_rq->ਆਖਰੀ_update_ਸਮਾਂ_copy,
			   sa->ਆਖਰੀ_update_ਸਮਾਂ);
	ਵਾਪਸ decayed;
}

/**
 * attach_entity_load_avg - attach this entity to its cfs_rq load avg
 * @cfs_rq: cfs_rq to attach to
 * @se: sched_entity to attach
 *
 * Must call update_cfs_rq_load_avg() before this, since we rely on
 * cfs_rq->avg.last_update_time being current.
 */
ਸਥਿਰ ਖਾਲੀ attach_entity_ਲੱਦੋ_avg(ਬਣਤਰ cfs_rq *cfs_rq, ਬਣਤਰ sched_entity *se)
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
	se->avg.ਆਖਰੀ_update_ਸਮਾਂ = cfs_rq->avg.ਆਖਰੀ_update_ਸਮਾਂ;
	se->avg.period_contrib = cfs_rq->avg.period_contrib;

	/*
	 * Hell(o) Nasty stuff.. we need to recompute _sum based on the new
	 * period_contrib. This isn't strictly correct, but since we're
	 * entirely outside of the PELT hierarchy, nobody cares if we truncate
	 * _sum a little.
	 */
	se->avg.util_ਜੋੜ = se->avg.util_avg * divider;

	se->avg.runnable_ਜੋੜ = se->avg.runnable_avg * divider;

	se->avg.ਲੱਦੋ_ਜੋੜ = se->avg.ਲੱਦੋ_avg * divider;
	ਜੇ (se_ਭਾਰ(se) < se->avg.ਲੱਦੋ_ਜੋੜ)
		se->avg.ਲੱਦੋ_ਜੋੜ = div_u64(se->avg.ਲੱਦੋ_ਜੋੜ, se_ਭਾਰ(se));
	ਨਹੀਂਤਾਂ
		se->avg.ਲੱਦੋ_ਜੋੜ = 1;

	ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_ਲੱਦੋ_avg(cfs_rq, se);
	cfs_rq->avg.util_avg += se->avg.util_avg;
	cfs_rq->avg.util_ਜੋੜ += se->avg.util_ਜੋੜ;
	cfs_rq->avg.runnable_avg += se->avg.runnable_avg;
	cfs_rq->avg.runnable_ਜੋੜ += se->avg.runnable_ਜੋੜ;

	ਜੋੜੋ_tg_cfs_propagate(cfs_rq, se->avg.ਲੱਦੋ_ਜੋੜ);

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
ਸਥਿਰ ਖਾਲੀ detach_entity_ਲੱਦੋ_avg(ਬਣਤਰ cfs_rq *cfs_rq, ਬਣਤਰ sched_entity *se)
{
	ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_ਲੱਦੋ_avg(cfs_rq, se);
	__update_sa(&cfs_rq->avg, util, -se->avg.util_avg, -se->avg.util_ਜੋੜ);
	__update_sa(&cfs_rq->avg, runnable, -se->avg.runnable_avg, -se->avg.runnable_ਜੋੜ);

	ਜੋੜੋ_tg_cfs_propagate(cfs_rq, -se->avg.ਲੱਦੋ_ਜੋੜ);

	cfs_rq_util_change(cfs_rq, 0);

	trace_pelt_cfs_tp(cfs_rq);
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ UTIL_EST_MARGIN (SCHED_ਸਮਰੱਥਾ_SCALE / 100)

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ util_est_update(ਬਣਤਰ sched_entity *se)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ewma, dequeued, ਆਖਰੀ_ewma_diff;

	ਜੇ (!sched_feat(UTIL_EST))
		ਵਾਪਸ;

	/* Get current estimate of utilization */
	ewma = READ_ONCE(se->avg.util_est);

	/*
	 * If the PELT values haven't changed since enqueue time,
	 * skip the util_est update.
	 */
	ਜੇ (ewma & UTIL_AVG_UNCHANGED)
		ਵਾਪਸ;

	/* Get utilization at dequeue */
	dequeued = READ_ONCE(se->avg.util_avg);

	/*
	 * Reset EWMA on utilization increases, the moving average is used only
	 * to smooth utilization decreases.
	 */
	ਜੇ (ewma <= dequeued) {
		ewma = dequeued;
		ਜਾਓ ਪੂਰਾ;
	}

	/*
	 * Skip update of task's estimated utilization when its members are
	 * already ~1% close to its last activation value.
	 */
	ਆਖਰੀ_ewma_diff = ewma - dequeued;
	ਜੇ (ਆਖਰੀ_ewma_diff < UTIL_EST_MARGIN)
		ਜਾਓ ਪੂਰਾ;

	/*
	 * To avoid underestimate of task utilization, skip updates of EWMA if
	 * we cannot grant that thread got all CPU time it wanted.
	 */
	ਜੇ ((dequeued + UTIL_EST_MARGIN) < READ_ONCE(se->avg.runnable_avg))
		ਜਾਓ ਪੂਰਾ;

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
	ewma <<= UTIL_EST_ਭਾਰ_ਖਿਸਕਾਓ;
	ewma  -= ਆਖਰੀ_ewma_diff;
	ewma >>= UTIL_EST_ਭਾਰ_ਖਿਸਕਾਓ;
ਪੂਰਾ:
	ewma |= UTIL_AVG_UNCHANGED;
	WRITE_ONCE(se->avg.util_est, ewma);

	trace_sched_util_est_se_tp(se);
}

/*
 * Optional action to be done while updating the load average
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ UPDATE_TG	0x01
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SKIP_AGE_ਲੱਦੋ	0x02
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DO_ATTACH	0x04
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DO_DETACH	0x08
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ UPDATE_UTIL_EST	0x10

/* Update task and its cfs_rq load average */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ update_ਲੱਦੋ_avg(ਬਣਤਰ cfs_rq *cfs_rq, ਬਣਤਰ sched_entity *se, ਪੂਰਨ flags)
{
	u64 now = cfs_rq_clock_pelt(cfs_rq);
	ਪੂਰਨ decayed;

	/*
	 * Track task load average for carrying it to new CPU after migrated, and
	 * track group sched_entity load average for task_h_load calculation in migration
	 */
	ਜੇ (se->avg.ਆਖਰੀ_update_ਸਮਾਂ && !(flags & SKIP_AGE_ਲੱਦੋ))
		__update_load_avg_se(now, cfs_rq, se);

	decayed  = update_cfs_rq_ਲੱਦੋ_avg(now, cfs_rq);
	decayed |= propagate_entity_ਲੱਦੋ_avg(se);

	ਜੇ (!se->avg.ਆਖਰੀ_update_ਸਮਾਂ && (flags & DO_ATTACH)) {

		/*
		 * DO_ATTACH means we're here from enqueue_entity().
		 * !last_update_time means we've passed through
		 * migrate_task_rq_fair() indicating we migrated.
		 *
		 * IOW we're enqueueing a task on a new CPU.
		 */
		attach_entity_ਲੱਦੋ_avg(cfs_rq, se);
		update_tg_ਲੱਦੋ_avg(cfs_rq);

	} ਨਹੀਂਤਾਂ ਜੇ (flags & DO_DETACH) {
		/*
		 * DO_DETACH means we're here from dequeue_entity()
		 * and we are migrating task out of the CPU.
		 */
		detach_entity_ਲੱਦੋ_avg(cfs_rq, se);
		update_tg_ਲੱਦੋ_avg(cfs_rq);
	} ਨਹੀਂਤਾਂ ਜੇ (decayed) {
		cfs_rq_util_change(cfs_rq, 0);

		ਜੇ (flags & UPDATE_TG)
			update_tg_ਲੱਦੋ_avg(cfs_rq);
	}

	ਜੇ (flags & UPDATE_UTIL_EST)
		util_est_update(se);
}

/*
 * Synchronize entity load avg of dequeued entity without locking
 * the previous rq.
 */
ਸਥਿਰ ਖਾਲੀ sync_entity_ਲੱਦੋ_avg(ਬਣਤਰ sched_entity *se)
{
	ਬਣਤਰ cfs_rq *cfs_rq = cfs_rq_ਦਾ(se);
	u64 ਆਖਰੀ_update_ਸਮਾਂ;

	ਆਖਰੀ_update_ਸਮਾਂ = cfs_rq_ਆਖਰੀ_update_ਸਮਾਂ(cfs_rq);
	__update_load_avg_blocked_se(ਆਖਰੀ_update_ਸਮਾਂ, se);
}

/*
 * Task first catches up with cfs_rq, and then subtract
 * itself from the cfs_rq (task must be off the queue now).
 */
ਸਥਿਰ ਖਾਲੀ remove_entity_ਲੱਦੋ_avg(ਬਣਤਰ sched_entity *se)
{
	ਬਣਤਰ cfs_rq *cfs_rq = cfs_rq_ਦਾ(se);
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags;

	/*
	 * tasks cannot exit without having gone through wake_up_new_task() ->
	 * enqueue_task_fair() which will have added things to the cfs_rq,
	 * so we can remove unconditionally.
	 */

	sync_entity_ਲੱਦੋ_avg(se);

	raw_spin_ਤਾਲਾ_irqsave(&cfs_rq->removed.ਤਾਲਾ, flags);
	++cfs_rq->removed.nr;
	cfs_rq->removed.util_avg	+= se->avg.util_avg;
	cfs_rq->removed.ਲੱਦੋ_avg	+= se->avg.ਲੱਦੋ_avg;
	cfs_rq->removed.runnable_avg	+= se->avg.runnable_avg;
	raw_spin_unlock_irqrestore(&cfs_rq->removed.ਤਾਲਾ, flags);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ cfs_rq_runnable_avg(ਬਣਤਰ cfs_rq *cfs_rq)
{
	ਵਾਪਸ cfs_rq->avg.runnable_avg;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ cfs_rq_ਲੱਦੋ_avg(ਬਣਤਰ cfs_rq *cfs_rq)
{
	ਵਾਪਸ cfs_rq->avg.ਲੱਦੋ_avg;
}

ਸਥਿਰ ਪੂਰਨ sched_balance_newidle(ਬਣਤਰ rq *this_rq, ਬਣਤਰ rq_flags *rf)
	__must_hold(__rq_lockp(this_rq));

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ task_util(ਬਣਤਰ task_struct *p)
{
	ਵਾਪਸ READ_ONCE(p->se.avg.util_avg);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ _task_util_est(ਬਣਤਰ task_struct *p)
{
	ਵਾਪਸ READ_ONCE(p->se.avg.util_est) & ~UTIL_AVG_UNCHANGED;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ task_util_est(ਬਣਤਰ task_struct *p)
{
	ਵਾਪਸ max(task_util(p), _task_util_est(p));
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ util_est_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ(ਬਣਤਰ cfs_rq *cfs_rq,
				    ਬਣਤਰ task_struct *p)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ enqueued;

	ਜੇ (!sched_feat(UTIL_EST))
		ਵਾਪਸ;

	/* Update root cfs_rq's estimated utilization */
	enqueued  = cfs_rq->avg.util_est;
	enqueued += _task_util_est(p);
	WRITE_ONCE(cfs_rq->avg.util_est, enqueued);

	trace_sched_util_est_cfs_tp(cfs_rq);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ util_est_ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ(ਬਣਤਰ cfs_rq *cfs_rq,
				    ਬਣਤਰ task_struct *p)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ enqueued;

	ਜੇ (!sched_feat(UTIL_EST))
		ਵਾਪਸ;

	/* Update root cfs_rq's estimated utilization */
	enqueued  = cfs_rq->avg.util_est;
	enqueued -= min_t(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ, enqueued, _task_util_est(p));
	WRITE_ONCE(cfs_rq->avg.util_est, enqueued);

	trace_sched_util_est_cfs_tp(cfs_rq);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ get_actual_cpu_ਸਮਰੱਥਾ(ਪੂਰਨ cpu)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਸਮਰੱਥਾ = arch_scale_cpu_ਸਮਰੱਥਾ(cpu);

	ਸਮਰੱਥਾ -= max(hw_ਲੱਦੋ_avg(cpu_rq(cpu)), cpufreq_get_pressure(cpu));

	ਵਾਪਸ ਸਮਰੱਥਾ;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ util_ਸਮਾਉਂਦਾ_cpu(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ util,
				ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ uclamp_min,
				ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ uclamp_max,
				ਪੂਰਨ cpu)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਸਮਰੱਥਾ = ਸਮਰੱਥਾ_ਦਾ(cpu);
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਸਮਰੱਥਾ_orig;
	bool ਸਮਾਉਂਦਾ, uclamp_max_ਸਮਾਉਂਦਾ;

	/*
	 * Check if the real util fits without any uclamp boost/cap applied.
	 */
	ਸਮਾਉਂਦਾ = ਸਮਾਉਂਦਾ_ਸਮਰੱਥਾ(util, ਸਮਰੱਥਾ);

	ਜੇ (!uclamp_is_ਵਰਤਿਆ())
		ਵਾਪਸ ਸਮਾਉਂਦਾ;

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
	ਸਮਰੱਥਾ_orig = arch_scale_cpu_ਸਮਰੱਥਾ(cpu);

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
	uclamp_max_ਸਮਾਉਂਦਾ = (ਸਮਰੱਥਾ_orig == SCHED_ਸਮਰੱਥਾ_SCALE) && (uclamp_max == SCHED_ਸਮਰੱਥਾ_SCALE);
	uclamp_max_ਸਮਾਉਂਦਾ = !uclamp_max_ਸਮਾਉਂਦਾ && (uclamp_max <= ਸਮਰੱਥਾ_orig);
	ਸਮਾਉਂਦਾ = ਸਮਾਉਂਦਾ || uclamp_max_ਸਮਾਉਂਦਾ;

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
	ਜੇ (ਸਮਾਉਂਦਾ && (util < uclamp_min) &&
	    (uclamp_min > get_actual_cpu_ਸਮਰੱਥਾ(cpu)))
		ਵਾਪਸ -1;

	ਵਾਪਸ ਸਮਾਉਂਦਾ;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ task_ਸਮਾਉਂਦਾ_cpu(ਬਣਤਰ task_struct *p, ਪੂਰਨ cpu)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ uclamp_min = uclamp_eff_ਮੁੱਲ(p, UCLAMP_MIN);
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ uclamp_max = uclamp_eff_ਮੁੱਲ(p, UCLAMP_MAX);
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ util = task_util_est(p);
	/*
	 * Return true only if the cpu fully fits the task requirements, which
	 * include the utilization but also the performance hints.
	 */
	ਵਾਪਸ (util_ਸਮਾਉਂਦਾ_cpu(util, uclamp_min, uclamp_max, cpu) > 0);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ update_misfit_ਸਥਿਤੀ(ਬਣਤਰ task_struct *p, ਬਣਤਰ rq *rq)
{
	ਪੂਰਨ cpu = cpu_ਦਾ(rq);

	ਜੇ (!sched_asym_cpucap_ਸਰਗਰਮ())
		ਵਾਪਸ;

	/*
	 * Affinity allows us to go somewhere higher?  Or are we on biggest
	 * available CPU already? Or do we fit into this CPU ?
	 */
	ਜੇ (!p || (p->nr_cpus_allowed == 1) ||
	    (arch_scale_cpu_ਸਮਰੱਥਾ(cpu) == p->max_allowed_ਸਮਰੱਥਾ) ||
	    task_ਸਮਾਉਂਦਾ_cpu(p, cpu)) {

		rq->misfit_task_ਲੱਦੋ = 0;
		ਵਾਪਸ;
	}

	/*
	 * Make sure that misfit_task_load will not be null even if
	 * task_h_load() returns 0.
	 */
	rq->misfit_task_ਲੱਦੋ = max_t(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ, task_h_ਲੱਦੋ(p), 1);
}

ਖਾਲੀ __setparam_fair(ਬਣਤਰ task_struct *p, ਅਚਲ ਬਣਤਰ sched_attr *attr)
{
	ਬਣਤਰ sched_entity *se = &p->se;

	p->static_prio = NICE_ਮੰਜ਼ਿਲ_PRIO(attr->sched_nice);
	ਜੇ (attr->sched_runtime) {
		se->custom_ਟੁਕੜਾ = 1;
		se->ਟੁਕੜਾ = clamp_t(u64, attr->sched_runtime,
				      NSEC_ਪ੍ਰਤੀ_MSEC/10,   /* HZ=1000 * 10 */
				      NSEC_ਪ੍ਰਤੀ_MSEC*100); /* HZ=100  / 10 */
	} ਨਹੀਂਤਾਂ {
		se->custom_ਟੁਕੜਾ = 0;
		se->ਟੁਕੜਾ = sysctl_sched_ਅਧਾਰ_ਟੁਕੜਾ;
	}
}

ਸਥਿਰ ਖਾਲੀ
ਰੱਖੋ_entity(ਬਣਤਰ cfs_rq *cfs_rq, ਬਣਤਰ sched_entity *se, ਪੂਰਨ flags)
{
	u64 vslice, vruntime = avg_vruntime(cfs_rq);
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ nr_queued = cfs_rq->h_nr_queued;
	bool update_zero = false;
	s64 lag = 0;

	ਜੇ (!se->custom_ਟੁਕੜਾ)
		se->ਟੁਕੜਾ = sysctl_sched_ਅਧਾਰ_ਟੁਕੜਾ;
	vslice = calc_delta_fair(se->ਟੁਕੜਾ, se);

	ਜੇ (flags & ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_QUEUED)
		nr_queued -= 1;

	/*
	 * Due to how V is constructed as the weighted average of entities,
	 * adding tasks with positive lag, or removing tasks with negative lag
	 * will move 'time' backwards, this can screw around with the lag of
	 * other tasks.
	 *
	 * EEVDF: placement strategy #1 / #2
	 */
	ਜੇ (sched_feat(ਰੱਖੋ_LAG) && nr_queued && se->vlag) {
		ਬਣਤਰ sched_entity *curr = cfs_rq->curr;
		ਲੰਮਾ ਲੱਦੋ, ਭਾਰ;

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
		ਲੱਦੋ = cfs_rq->ਜੋੜ_ਭਾਰ;
		ਜੇ (curr && curr->on_rq)
			ਲੱਦੋ += avg_vruntime_ਭਾਰ(cfs_rq, curr->h_ਲੱਦੋ.ਭਾਰ);
		ਜੇ (WARN_ON_ONCE(!ਲੱਦੋ))
			ਲੱਦੋ = 1;

		ਭਾਰ = avg_vruntime_ਭਾਰ(cfs_rq, se->h_ਲੱਦੋ.ਭਾਰ);

		/*
		 * Rewrite, because the product (W + w_i)*vl'_i might
		 * overflow s64:
		 *
		 *   vl_i = (W + w_i)*vl'_i / W
		 *        = (W*vl'_i + w_i*vl'_i) / W
		 *        = vl'_i + w_i*vl'_i/W
		 */
		lag += div64_long(lag * ਭਾਰ, ਲੱਦੋ);

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
		ਜੇ (ਭਾਰ > ਲੱਦੋ)
			update_zero = true;
	}

	se->vruntime = vruntime - lag;

	ਜੇ (update_zero)
		update_zero_vruntime(cfs_rq, -lag);

	ਜੇ (sched_feat(ਰੱਖੋ_REL_DEADLINE) && se->rel_deadline) {
		se->deadline += se->vruntime;
		se->rel_deadline = 0;
		ਵਾਪਸ;
	}

	/*
	 * When joining the competition; the existing tasks will be,
	 * on average, halfway through their slice, as such start tasks
	 * off with half a slice to ease into the competition.
	 */
	ਜੇ (sched_feat(ਰੱਖੋ_DEADLINE_INITIAL) && (flags & ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_INITIAL))
		vslice /= 2;

	/*
	 * EEVDF: vd_i = ve_i + r_i/w_i
	 */
	se->deadline = se->vruntime + vslice;
}

ਸਥਿਰ ਖਾਲੀ check_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_throttle(ਬਣਤਰ cfs_rq *cfs_rq);
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ cfs_rq_throttled(ਬਣਤਰ cfs_rq *cfs_rq);

ਸਥਿਰ ਖਾਲੀ
ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_entity(ਬਣਤਰ cfs_rq *cfs_rq, ਬਣਤਰ sched_entity *se, ਪੂਰਨ flags)
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
	update_ਲੱਦੋ_avg(cfs_rq, se, UPDATE_TG | DO_ATTACH);
	se_update_runnable(se);
	/*
	 * XXX update_load_avg() above will have attached us to the pelt sum;
	 * but update_cfs_group() here will re-adjust the weight and have to
	 * undo/redo all that. Seems wasteful.
	 */
	update_cfs_ਸਮੂਹ(se);

	account_entity_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ(cfs_rq, se);

	/* Entity has migrated, no longer consider this task hot */
	ਜੇ (flags & ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_MIGRATED)
		se->exec_ਸ਼ੁਰੂ = 0;

	check_schedstat_required();
	update_stats_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_fair(cfs_rq, se, flags);
	se->on_rq = 1;

	ਜੇ (cfs_rq->nr_queued == 1) {
		check_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_throttle(cfs_rq);
		list_ਜੋੜੋ_ਪੱਤੀ_cfs_rq(cfs_rq);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_CFS_BANDWIDTH
		ਜੇ (cfs_rq->pelt_clock_throttled) {
			ਬਣਤਰ rq *rq = rq_ਦਾ(cfs_rq);

			cfs_rq->throttled_clock_pelt_ਸਮਾਂ += rq_clock_pelt(rq) -
				cfs_rq->throttled_clock_pelt;
			cfs_rq->pelt_clock_throttled = 0;
		}
#ਜੇ_ਅੰਤ
	}
}

ਸਥਿਰ ਖਾਲੀ ਸੈੱਟ_ਅਗਲਾ_buddy(ਬਣਤਰ cfs_rq *cfs_rq, ਬਣਤਰ sched_entity *se)
{
	ਜੇ (WARN_ON_ONCE(!se->on_rq || se->sched_delayed))
		ਵਾਪਸ;
	ਜੇ (se_is_idle(se))
		ਵਾਪਸ;
	cfs_rq->ਅਗਲਾ = se;
}

ਸਥਿਰ ਖਾਲੀ clear_buddies(ਬਣਤਰ cfs_rq *cfs_rq, ਬਣਤਰ sched_entity *se)
{
	ਜੇ (cfs_rq->ਅਗਲਾ == se)
		cfs_rq->ਅਗਲਾ = ਨਿਰਾ;
}

ਸਥਿਰ __always_inline ਖਾਲੀ return_cfs_rq_runtime(ਬਣਤਰ cfs_rq *cfs_rq);

ਸਥਿਰ ਖਾਲੀ ਸੈੱਟ_delayed(ਬਣਤਰ sched_entity *se)
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
	ਜੇ (!entity_is_task(se)) {
		se->sched_delayed = 1;
		ਵਾਪਸ;
	}

	/*
	 * Drop a task leaving the runnable set.
	 * Needs to be called before sched_delayed is set.
	 * clear_delayed() mirrors this after clearing the flag.
	 */
	pref_llc_running_dec(rq_ਦਾ(cfs_rq_ਦਾ(se)), task_ਦਾ(se));
	se->sched_delayed = 1;

	for_each_sched_entity(se) {
		ਬਣਤਰ cfs_rq *cfs_rq = cfs_rq_ਦਾ(se);

		cfs_rq->h_nr_runnable--;
	}
}

ਸਥਿਰ ਖਾਲੀ clear_delayed(ਬਣਤਰ sched_entity *se)
{
	se->sched_delayed = 0;

	/*
	 * Delayed se of cfs_rq have no tasks queued on them.
	 * Do not adjust h_nr_runnable since a dequeue has
	 * already accounted for it or an enqueue of a task
	 * below it will account for it in enqueue_task_fair().
	 */
	ਜੇ (!entity_is_task(se))
		ਵਾਪਸ;

	/*
	 * Re-add on wake, after sched_delayed is cleared. On a final delayed
	 * dequeue account_llc_dequeue() already cleared pref_llc_queued, so
	 * this does nothing.
	 */
	pref_llc_running_inc(rq_ਦਾ(cfs_rq_ਦਾ(se)), task_ਦਾ(se));

	for_each_sched_entity(se) {
		ਬਣਤਰ cfs_rq *cfs_rq = cfs_rq_ਦਾ(se);

		cfs_rq->h_nr_runnable++;
	}
}

ਸਥਿਰ ਖਾਲੀ
ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_entity(ਬਣਤਰ cfs_rq *cfs_rq, ਬਣਤਰ sched_entity *se, ਪੂਰਨ flags)
{
	ਪੂਰਨ action = UPDATE_TG;

	ਜੇ (entity_is_task(se)) {
		ਜੇ (task_on_rq_migrating(task_ਦਾ(se)))
			action |= DO_DETACH;

		ਜੇ ((flags & ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_SLEEP) && !(flags & ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_DELAYED))
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
	update_ਲੱਦੋ_avg(cfs_rq, se, action);
	se_update_runnable(se);

	update_stats_ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_fair(cfs_rq, se, flags);

	se->on_rq = 0;
	account_entity_ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ(cfs_rq, se);

	/* return excess runtime on last dequeue */
	return_cfs_rq_runtime(cfs_rq);

	update_cfs_ਸਮੂਹ(se);

	ਜੇ (cfs_rq->nr_queued == 0) {
		update_idle_cfs_rq_clock_pelt(cfs_rq);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_CFS_BANDWIDTH
		ਜੇ (throttled_hierarchy(cfs_rq)) {
			ਬਣਤਰ rq *rq = rq_ਦਾ(cfs_rq);

			list_del_ਪੱਤੀ_cfs_rq(cfs_rq);
			cfs_rq->throttled_clock_pelt = rq_clock_pelt(rq);
			cfs_rq->pelt_clock_throttled = 1;
		}
#ਜੇ_ਅੰਤ
	}
}

ਸਥਿਰ ਖਾਲੀ
ਸੈੱਟ_ਅਗਲਾ_entity(ਬਣਤਰ cfs_rq *cfs_rq, ਬਣਤਰ sched_entity *se)
{
	/* 'current' is not kept within the tree. */
	ਜੇ (se->on_rq) {
		/*
		 * Any task has to be enqueued before it get to execute on
		 * a CPU. So account for the time it spent waiting on the
		 * runqueue.
		 */
		update_stats_wait_ਸਿਰਾ_fair(cfs_rq, se);
		update_ਲੱਦੋ_avg(cfs_rq, se, UPDATE_TG);
	}

	update_stats_curr_ਸ਼ੁਰੂ(cfs_rq, se);
	WARN_ON_ONCE(cfs_rq->h_curr);
	cfs_rq->h_curr = se;

	/*
	 * Track our maximum slice length, if the CPU's load is at
	 * least twice that of our own weight (i.e. don't track it
	 * when there are only lesser-weight tasks around):
	 */
	ਜੇ (schedstat_enabled() &&
	    rq_ਦਾ(cfs_rq)->cfs.ਲੱਦੋ.ਭਾਰ >= 2*se->ਲੱਦੋ.ਭਾਰ) {
		ਬਣਤਰ sched_statistics *stats;

		stats = __schedstats_from_se(se);
		__schedstat_set(stats->ਟੁਕੜਾ_max,
				max((u64)stats->ਟੁਕੜਾ_max,
				    se->ਜੋੜ_exec_runtime - se->prev_ਜੋੜ_exec_runtime));
	}

	se->prev_ਜੋੜ_exec_runtime = se->ਜੋੜ_exec_runtime;
}

ਸਥਿਰ bool __dequeue_task(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, ਪੂਰਨ flags);

ਸਥਿਰ ਬਣਤਰ sched_entity *
pick_ਅਗਲਾ_entity(ਬਣਤਰ rq *rq, bool protect)
{
	ਬਣਤਰ cfs_rq *cfs_rq = &rq->cfs;
	ਬਣਤਰ sched_entity *se;

	se = pick_eevdf(cfs_rq, protect);
	ਜੇ (se->sched_delayed) {
		__dequeue_task(rq, task_ਦਾ(se), ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_SLEEP | ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_DELAYED);
		/*
		 * Must not reference @se again, see __block_task().
		 */
		ਵਾਪਸ ਨਿਰਾ;
	}
	ਵਾਪਸ se;
}

ਸਥਿਰ ਖਾਲੀ put_prev_entity(ਬਣਤਰ cfs_rq *cfs_rq, ਬਣਤਰ sched_entity *prev)
{
	/*
	 * If still on the runqueue then deactivate_task()
	 * was not called and update_curr() has to be done:
	 */
	ਜੇ (prev->on_rq)
		update_curr(cfs_rq);

	ਜੇ (prev->on_rq) {
		update_stats_wait_ਸ਼ੁਰੂ_fair(cfs_rq, prev);
		/* in !on_rq case, update occurred at dequeue */
		update_ਲੱਦੋ_avg(cfs_rq, prev, 0);
	}
	WARN_ON_ONCE(cfs_rq->h_curr != prev);
	cfs_rq->h_curr = ਨਿਰਾ;
}

ਸਥਿਰ ਖਾਲੀ
entity_tick(ਬਣਤਰ cfs_rq *cfs_rq, ਬਣਤਰ sched_entity *curr, ਪੂਰਨ queued)
{
	/*
	 * Update run-time statistics of the 'current'.
	 */
	update_curr(cfs_rq);

	/*
	 * Ensure that runnable average is periodically updated.
	 */
	update_ਲੱਦੋ_avg(cfs_rq, curr, UPDATE_TG);
	update_cfs_ਸਮੂਹ(curr);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_HRTICK
	/*
	 * queued ticks are scheduled to match the slice, so don't bother
	 * validating it and just reschedule.
	 */
	ਜੇ (queued) {
		resched_curr(rq_ਦਾ(cfs_rq));
		ਵਾਪਸ;
	}
#ਜੇ_ਅੰਤ
}


/**************************************************
 * CFS bandwidth control machinery
 */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_CFS_BANDWIDTH

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_JUMP_LABEL
ਸਥਿਰ ਬਣਤਰ static_ਕੁੰਜੀ __cfs_bandwidth_used;

ਸਥਿਰ ਸਤਰਅੰਦਰ bool cfs_bandwidth_ਵਰਤਿਆ(ਖਾਲੀ)
{
	ਵਾਪਸ static_ਕੁੰਜੀ_false(&__cfs_bandwidth_used);
}

ਖਾਲੀ cfs_bandwidth_usage_inc(ਖਾਲੀ)
{
	static_ਕੁੰਜੀ_slow_inc_cpuslocked(&__cfs_bandwidth_used);
}

ਖਾਲੀ cfs_bandwidth_usage_dec(ਖਾਲੀ)
{
	static_ਕੁੰਜੀ_slow_dec_cpuslocked(&__cfs_bandwidth_used);
}
#ਨਹੀਂਤਾਂ /* !CONFIG_JUMP_LABEL: */
ਸਥਿਰ bool cfs_bandwidth_ਵਰਤਿਆ(ਖਾਲੀ)
{
	ਵਾਪਸ true;
}

ਖਾਲੀ cfs_bandwidth_usage_inc(ਖਾਲੀ) {}
ਖਾਲੀ cfs_bandwidth_usage_dec(ਖਾਲੀ) {}
#ਜੇ_ਅੰਤ /* !CONFIG_JUMP_LABEL */

ਸਥਿਰ ਸਤਰਅੰਦਰ u64 sched_cfs_bandwidth_ਟੁਕੜਾ(ਖਾਲੀ)
{
	ਵਾਪਸ (u64)sysctl_sched_cfs_bandwidth_ਟੁਕੜਾ * NSEC_ਪ੍ਰਤੀ_USEC;
}

/*
 * Replenish runtime according to assigned quota. We use sched_clock_cpu
 * directly instead of rq->clock to avoid adding additional synchronization
 * around rq->lock.
 *
 * requires cfs_b->lock
 */
ਖਾਲੀ __refill_cfs_bandwidth_runtime(ਬਣਤਰ cfs_bandwidth *cfs_b)
{
	s64 runtime;

	ਜੇ (unlikely(cfs_b->quota == RUNTIME_INF))
		ਵਾਪਸ;

	cfs_b->runtime += cfs_b->quota;
	runtime = cfs_b->runtime_snap - cfs_b->runtime;
	ਜੇ (runtime > 0) {
		cfs_b->ਮਿਆਦ_ਸਮਾਂ += runtime;
		cfs_b->nr_ਮਿਆਦ++;
	}

	cfs_b->runtime = min(cfs_b->runtime, cfs_b->quota + cfs_b->ਮਿਆਦ);
	cfs_b->runtime_snap = cfs_b->runtime;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ cfs_bandwidth *tg_cfs_bandwidth(ਬਣਤਰ task_ਸਮੂਹ *tg)
{
	ਵਾਪਸ &tg->cfs_bandwidth;
}

/* returns 0 on failure to allocate runtime */
ਸਥਿਰ ਪੂਰਨ __assign_cfs_rq_runtime(ਬਣਤਰ cfs_bandwidth *cfs_b,
				   ਬਣਤਰ cfs_rq *cfs_rq, u64 ਨਿਸ਼ਾਨਾ_runtime)
{
	u64 min_amount, amount = 0;

	lockdep_assert_held(&cfs_b->ਤਾਲਾ);

	/* note: this is a positive sum as runtime_remaining <= 0 */
	min_amount = ਨਿਸ਼ਾਨਾ_runtime - cfs_rq->runtime_ਬਚਿਆ;

	ਜੇ (cfs_b->quota == RUNTIME_INF)
		amount = min_amount;
	ਨਹੀਂਤਾਂ {
		ਸ਼ੁਰੂ_cfs_bandwidth(cfs_b);

		ਜੇ (cfs_b->runtime > 0) {
			amount = min(cfs_b->runtime, min_amount);
			cfs_b->runtime -= amount;
			cfs_b->idle = 0;
		}
	}

	cfs_rq->runtime_ਬਚਿਆ += amount;

	ਵਾਪਸ cfs_rq->runtime_ਬਚਿਆ > 0;
}

ਸਥਿਰ bool throttle_cfs_rq(ਬਣਤਰ cfs_rq *cfs_rq);

ਸਥਿਰ bool __account_cfs_rq_runtime(ਬਣਤਰ cfs_rq *cfs_rq, u64 delta_exec)
{
	/* dock delta_exec before expiring quota (as it could span periods) */
	cfs_rq->runtime_ਬਚਿਆ -= delta_exec;

	ਜੇ (likely(cfs_rq->runtime_ਬਚਿਆ > 0))
		ਵਾਪਸ false;

	ਜੇ (cfs_rq->throttled)
		ਵਾਪਸ true;
	/*
	 * throttle_cfs_rq() will try to extend the runtime first
	 * before throttling the hierarchy.
	 */
	ਵਾਪਸ throttle_cfs_rq(cfs_rq);
}

ਸਥਿਰ __always_inline
bool account_cfs_rq_runtime(ਬਣਤਰ cfs_rq *cfs_rq, u64 delta_exec)
{
	ਜੇ (!cfs_bandwidth_ਵਰਤਿਆ() || !cfs_rq->runtime_enabled)
		ਵਾਪਸ false;

	ਵਾਪਸ __account_cfs_rq_runtime(cfs_rq, delta_exec);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ cfs_rq_throttled(ਬਣਤਰ cfs_rq *cfs_rq)
{
	ਵਾਪਸ cfs_bandwidth_ਵਰਤਿਆ() && cfs_rq->throttled;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool cfs_rq_pelt_clock_throttled(ਬਣਤਰ cfs_rq *cfs_rq)
{
	ਵਾਪਸ cfs_bandwidth_ਵਰਤਿਆ() && cfs_rq->pelt_clock_throttled;
}

/* check whether cfs_rq, or any parent, is throttled */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ throttled_hierarchy(ਬਣਤਰ cfs_rq *cfs_rq)
{
	ਵਾਪਸ cfs_bandwidth_ਵਰਤਿਆ() && cfs_rq->throttle_ਗਿਣਤੀ;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ lb_throttled_hierarchy(ਬਣਤਰ task_struct *p, ਪੂਰਨ dst_cpu)
{
	ਵਾਪਸ throttled_hierarchy(tg_cfs_rq(task_ਸਮੂਹ(p), dst_cpu));
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool task_is_throttled(ਬਣਤਰ task_struct *p)
{
	ਵਾਪਸ cfs_bandwidth_ਵਰਤਿਆ() && p->throttled;
}

ਸਥਿਰ bool ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_task_fair(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, ਪੂਰਨ flags);
ਸਥਿਰ ਖਾਲੀ throttle_cfs_rq_work(ਬਣਤਰ callback_ਸਿਰ *work)
{
	ਬਣਤਰ task_struct *p = container_ਦਾ(work, ਬਣਤਰ task_struct, sched_throttle_work);
	ਬਣਤਰ sched_entity *se;
	ਬਣਤਰ cfs_rq *cfs_rq;
	ਬਣਤਰ rq *rq;

	WARN_ON_ONCE(p != current);
	p->sched_throttle_work.ਅਗਲਾ = &p->sched_throttle_work;

	/*
	 * If task is exiting, then there won't be a return to userspace, so we
	 * don't have to bother with any of this.
	 */
	ਜੇ ((p->flags & PF_EXITING))
		ਵਾਪਸ;

	scoped_guard(task_rq_ਤਾਲਾ_2, p) {
		se = &p->se;
		cfs_rq = cfs_rq_ਦਾ(se);

		/* Raced, forget */
		ਜੇ (p->sched_class != &fair_sched_class)
			ਵਾਪਸ;

		/*
		 * If not in limbo, then either replenish has happened or this
		 * task got migrated out of the throttled cfs_rq, move along.
		 */
		ਜੇ (!cfs_rq->throttle_ਗਿਣਤੀ)
			ਵਾਪਸ;
		rq = scope.rq;
		update_rq_clock(rq);
		WARN_ON_ONCE(p->throttled || !list_ਖਾਲੀ_ਥਾਵਾਂ(&p->throttle_ਗੰਢ));
		ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_task_fair(rq, p, ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_SLEEP | ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_THROTTLE);
		list_ਜੋੜੋ(&p->throttle_ਗੰਢ, &cfs_rq->throttled_limbo_list);
		/*
		 * Must not set throttled before dequeue or dequeue will
		 * mistakenly regard this task as an already throttled one.
		 */
		p->throttled = true;
		resched_curr(rq);
	}
}

ਖਾਲੀ init_cfs_throttle_work(ਬਣਤਰ task_struct *p)
{
	init_task_work(&p->sched_throttle_work, throttle_cfs_rq_work);
	/* Protect against double add, see throttle_cfs_rq() and throttle_cfs_rq_work() */
	p->sched_throttle_work.ਅਗਲਾ = &p->sched_throttle_work;
	INIT_LIST_ਸਿਰ(&p->throttle_ਗੰਢ);
}

/*
 * Task is throttled and someone wants to dequeue it again:
 * it could be sched/core when core needs to do things like
 * task affinity change, task group change, task sched class
 * change etc. and in these cases, DEQUEUE_SLEEP is not set;
 * or the task is blocked after throttled due to freezer etc.
 * and in these cases, DEQUEUE_SLEEP is set.
 */
ਸਥਿਰ ਖਾਲੀ detach_task_cfs_rq(ਬਣਤਰ task_struct *p);
ਸਥਿਰ ਖਾਲੀ ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_throttled_task(ਬਣਤਰ task_struct *p, ਪੂਰਨ flags)
{
	WARN_ON_ONCE(p->se.on_rq);
	list_del_init(&p->throttle_ਗੰਢ);

	/* task blocked after throttled */
	ਜੇ (flags & ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_SLEEP) {
		p->throttled = false;
		ਵਾਪਸ;
	}

	/*
	 * task is migrating off its old cfs_rq, detach
	 * the task's load from its old cfs_rq.
	 */
	ਜੇ (task_on_rq_migrating(p))
		detach_task_cfs_rq(p);
}

ਸਥਿਰ bool ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_throttled_task(ਬਣਤਰ task_struct *p)
{
	ਬਣਤਰ cfs_rq *cfs_rq = cfs_rq_ਦਾ(&p->se);

	/* @p should have gone through dequeue_throttled_task() first */
	WARN_ON_ONCE(!list_ਖਾਲੀ_ਥਾਵਾਂ(&p->throttle_ਗੰਢ));

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
	ਜੇ (throttled_hierarchy(cfs_rq) &&
	    !task_current_donor(rq_ਦਾ(cfs_rq), p)) {
		list_ਜੋੜੋ(&p->throttle_ਗੰਢ, &cfs_rq->throttled_limbo_list);
		ਵਾਪਸ true;
	}

	/* we can't take the fast path, do an actual enqueue*/
	p->throttled = false;
	ਵਾਪਸ false;
}

ਸਥਿਰ ਖਾਲੀ ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_task_fair(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, ਪੂਰਨ flags);
ਸਥਿਰ ਪੂਰਨ tg_unthrottle_ਉੱਪਰ(ਬਣਤਰ task_ਸਮੂਹ *tg, ਖਾਲੀ *ਅੰਕੜੇ)
{
	ਬਣਤਰ rq *rq = ਅੰਕੜੇ;
	ਬਣਤਰ cfs_rq *cfs_rq = tg_cfs_rq(tg, cpu_ਦਾ(rq));
	ਬਣਤਰ task_struct *p, *tmp;
	LIST_ਸਿਰ(throttled_tasks);

	/*
	 * If cfs_rq->curr is set, the cfs_rq might not have caught up
	 * since the last clock update. Do it now before we begin
	 * queueing task onto it to save the need for unnecessarily
	 * unthrottle the hierarchy for this cfs_rq to be throttled
	 * right back again.
	 */
	update_curr(cfs_rq);

	ਜੇ (--cfs_rq->throttle_ਗਿਣਤੀ)
		ਵਾਪਸ 0;

	ਜੇ (cfs_rq->pelt_clock_throttled) {
		cfs_rq->throttled_clock_pelt_ਸਮਾਂ += rq_clock_pelt(rq) -
					     cfs_rq->throttled_clock_pelt;
		cfs_rq->pelt_clock_throttled = 0;
	}

	ਜੇ (cfs_rq->throttled_clock_self) {
		u64 delta = rq_clock(rq) - cfs_rq->throttled_clock_self;

		cfs_rq->throttled_clock_self = 0;

		ਜੇ (WARN_ON_ONCE((s64)delta < 0))
			delta = 0;

		cfs_rq->throttled_clock_self_ਸਮਾਂ += delta;
	}

	/*
	 * Move the tasks to a local list since an update_curr() during
	 * enqueue_task_fair() can throttle a higher cfs_rq, and it can
	 * see the "throttled_limbo_list" being non-empty in
	 * tg_throttle_down() if throttle_count turned 0 above.
	 */
	list_splice_init(&cfs_rq->throttled_limbo_list, &throttled_tasks);

	/* Re-enqueue the tasks that have been throttled at this level. */
	list_for_each_ਇੰਦਰਾਜ_ਸੁਰੱਖਿਅਤ(p, tmp, &throttled_tasks, throttle_ਗੰਢ) {
		/*
		 * Back to being throttled! Break out and put the remaining
		 * tasks back onto the limbo_list to prevent running them
		 * unnecessarily.
		 */
		ਜੇ (cfs_rq->throttle_ਗਿਣਤੀ)
			ਰੋਕੋ;

		list_del_init(&p->throttle_ਗੰਢ);
		p->throttled = false;
		ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_task_fair(rq, p, ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_WAKEUP);
	}

	list_splice(&throttled_tasks, &cfs_rq->throttled_limbo_list);

	/* Add cfs_rq with load or one or more already running entities to the list */
	ਜੇ (!cfs_rq_is_decayed(cfs_rq))
		list_ਜੋੜੋ_ਪੱਤੀ_cfs_rq(cfs_rq);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool task_has_throttle_work(ਬਣਤਰ task_struct *p)
{
	ਵਾਪਸ p->sched_throttle_work.ਅਗਲਾ != &p->sched_throttle_work;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ task_throttle_ਤਿਆਰੀ_work(ਬਣਤਰ task_struct *p)
{
	ਜੇ (task_has_throttle_work(p))
		ਵਾਪਸ;

	/*
	 * Kthreads and exiting tasks don't return to userspace, so adding the
	 * work is pointless
	 */
	ਜੇ ((p->flags & (PF_EXITING | PF_KTHREAD)))
		ਵਾਪਸ;

	task_work_ਜੋੜੋ(p, &p->sched_throttle_work, TWA_RESUME);
}

ਸਥਿਰ ਖਾਲੀ ਰਿਕਾਰਡ_throttle_clock(ਬਣਤਰ cfs_rq *cfs_rq)
{
	ਬਣਤਰ rq *rq = rq_ਦਾ(cfs_rq);

	ਜੇ (cfs_rq_throttled(cfs_rq) && !cfs_rq->throttled_clock)
		cfs_rq->throttled_clock = rq_clock(rq);

	ਜੇ (!cfs_rq->throttled_clock_self)
		cfs_rq->throttled_clock_self = rq_clock(rq);
}

ਸਥਿਰ ਪੂਰਨ tg_throttle_ਥੱਲੇ(ਬਣਤਰ task_ਸਮੂਹ *tg, ਖਾਲੀ *ਅੰਕੜੇ)
{
	ਬਣਤਰ rq *rq = ਅੰਕੜੇ;
	ਬਣਤਰ cfs_rq *cfs_rq = tg_cfs_rq(tg, cpu_ਦਾ(rq));

	ਜੇ (cfs_rq->throttle_ਗਿਣਤੀ++)
		ਵਾਪਸ 0;

	/*
	 * For cfs_rqs that still have entities enqueued, PELT clock
	 * stop happens at dequeue time when all entities are dequeued.
	 */
	ਜੇ (!cfs_rq->nr_queued) {
		list_del_ਪੱਤੀ_cfs_rq(cfs_rq);
		cfs_rq->throttled_clock_pelt = rq_clock_pelt(rq);
		cfs_rq->pelt_clock_throttled = 1;
	}

	WARN_ON_ONCE(cfs_rq->throttled_clock_self);
	WARN_ON_ONCE(!list_ਖਾਲੀ_ਥਾਵਾਂ(&cfs_rq->throttled_limbo_list));
	ਵਾਪਸ 0;
}

ਸਥਿਰ bool throttle_cfs_rq(ਬਣਤਰ cfs_rq *cfs_rq)
{
	ਬਣਤਰ cfs_bandwidth *cfs_b = tg_cfs_bandwidth(cfs_rq->tg);
	ਬਣਤਰ sched_entity *curr = cfs_rq->h_curr;
	ਬਣਤਰ rq *rq = rq_ਦਾ(cfs_rq);

	scoped_guard(raw_spinlock, &cfs_b->ਤਾਲਾ) {
		u64 ਨਿਸ਼ਾਨਾ_runtime = 1;

		/*
		 * If cfs_rq->h_curr is still runnable, we are here from an
		 * update_curr(). Request sysctl_sched_cfs_bandwidth_slice
		 * worth of bandwidth to continue running.
		 *
		 * If the curr is not runnable, just request enough bandwidth
		 * to be runnable next time the pick selects this cfs_rq.
		 */
		ਜੇ (curr && curr->on_rq)
			ਨਿਸ਼ਾਨਾ_runtime = sched_cfs_bandwidth_ਟੁਕੜਾ();

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
		ਜੇ (__assign_cfs_rq_runtime(cfs_b, cfs_rq, ਨਿਸ਼ਾਨਾ_runtime))
			ਵਾਪਸ false;

		/*
		 * No bandwidth available; Add ourselves on the list to be
		 * unthrottled later.
		 */
		list_ਜੋੜੋ_ਪੂਛ_rcu(&cfs_rq->throttled_list,
				  &cfs_b->throttled_cfs_rq);
	}

	/* freeze hierarchy runnable averages while throttled */
	scoped_guard(rcu)
		walk_tg_ਰੁੱਖ_from(cfs_rq->tg, tg_throttle_ਥੱਲੇ, tg_nop, (ਖਾਲੀ *)rq);

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
	ਜੇ (curr && curr->on_rq)
		task_throttle_ਤਿਆਰੀ_work(rq->donor);

	ਵਾਪਸ true;
}

ਖਾਲੀ unthrottle_cfs_rq(ਬਣਤਰ cfs_rq *cfs_rq)
{
	ਬਣਤਰ rq *rq = rq_ਦਾ(cfs_rq);
	ਬਣਤਰ cfs_bandwidth *cfs_b = tg_cfs_bandwidth(cfs_rq->tg);
	ਬਣਤਰ sched_entity *se = cfs_rq_se(cfs_rq);

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
	ਜੇ (cfs_rq->runtime_enabled && cfs_rq->runtime_ਬਚਿਆ <= 0)
		ਵਾਪਸ;

	cfs_rq->throttled = 0;

	scoped_guard(raw_spinlock, &cfs_b->ਤਾਲਾ) {
		list_del_rcu(&cfs_rq->throttled_list);

		ਜੇ (!cfs_rq->throttled_clock)
			ਰੋਕੋ;

		cfs_b->throttled_ਸਮਾਂ += rq_clock(rq) - cfs_rq->throttled_clock;
		cfs_rq->throttled_clock = 0;
	}

	/* update hierarchical throttle state */
	walk_tg_ਰੁੱਖ_from(cfs_rq->tg, tg_nop, tg_unthrottle_ਉੱਪਰ, (ਖਾਲੀ *)rq);

	ਜੇ (!cfs_rq->ਲੱਦੋ.ਭਾਰ) {
		ਜੇ (!cfs_rq->on_list)
			ਵਾਪਸ;
		/*
		 * Nothing to run but something to decay (on_list)?
		 * Complete the branch.
		 */
		for_each_sched_entity(se) {
			ਜੇ (list_ਜੋੜੋ_ਪੱਤੀ_cfs_rq(cfs_rq_ਦਾ(se)))
				ਰੋਕੋ;
		}
	}

	assert_list_ਪੱਤੀ_cfs_rq(rq);

	/* Determine whether we need to wake up potentially idle CPU: */
	ਜੇ (rq->curr == rq->idle && rq->cfs.h_nr_queued)
		resched_curr(rq);
}

ਸਥਿਰ ਖਾਲੀ __cfsb_csd_unthrottle(ਖਾਲੀ *arg)
{
	ਬਣਤਰ cfs_rq *cursor, *tmp;
	ਬਣਤਰ rq *rq = arg;

	guard(rq_ਤਾਲਾ)(rq);

	/*
	 * Iterating over the list can trigger several call to
	 * update_rq_clock() in unthrottle_cfs_rq().
	 * Do it once and skip the potential next ones.
	 */
	update_rq_clock(rq);
	rq_clock_ਸ਼ੁਰੂ_loop_update(rq);

	/*
	 * Since we hold rq lock we're safe from concurrent manipulation of
	 * the CSD list. However, this RCU critical section annotates the
	 * fact that we pair with sched_free_group_rcu(), so that we cannot
	 * race with group being freed in the window between removing it
	 * from the list and advancing to the next entry in the list.
	 */
	guard(rcu)();

	list_for_each_ਇੰਦਰਾਜ_ਸੁਰੱਖਿਅਤ(cursor, tmp, &rq->cfsb_csd_list,
				 throttled_csd_list) {
		list_del_init(&cursor->throttled_csd_list);

		ਜੇ (cfs_rq_throttled(cursor))
			unthrottle_cfs_rq(cursor);
	}

	rq_clock_stop_loop_update(rq);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ __unthrottle_cfs_rq_async(ਬਣਤਰ cfs_rq *cfs_rq)
{
	ਬਣਤਰ rq *rq = rq_ਦਾ(cfs_rq);
	bool ਪਹਿਲਾ;

	ਜੇ (rq == this_rq()) {
		update_rq_clock(rq);
		unthrottle_cfs_rq(cfs_rq);
		ਵਾਪਸ;
	}

	/* Already enqueued */
	ਜੇ (WARN_ON_ONCE(!list_ਖਾਲੀ_ਥਾਵਾਂ(&cfs_rq->throttled_csd_list)))
		ਵਾਪਸ;

	ਪਹਿਲਾ = list_ਖਾਲੀ_ਥਾਵਾਂ(&rq->cfsb_csd_list);
	list_ਜੋੜੋ_ਪੂਛ(&cfs_rq->throttled_csd_list, &rq->cfsb_csd_list);
	ਜੇ (ਪਹਿਲਾ)
		smp_call_function_single_async(cpu_ਦਾ(rq), &rq->cfsb_csd);
}

ਸਥਿਰ ਖਾਲੀ unthrottle_cfs_rq_async(ਬਣਤਰ cfs_rq *cfs_rq)
{
	lockdep_assert_rq_held(rq_ਦਾ(cfs_rq));

	ਜੇ (WARN_ON_ONCE(!cfs_rq_throttled(cfs_rq) ||
	    cfs_rq->runtime_ਬਚਿਆ <= 0))
		ਵਾਪਸ;

	__unthrottle_cfs_rq_async(cfs_rq);
}

ਸਥਿਰ bool distribute_cfs_runtime(ਬਣਤਰ cfs_bandwidth *cfs_b)
{
	bool throttled = false, unthrottle_local = false;
	ਪੂਰਨ this_cpu = smp_processor_id();
	u64 runtime, ਬਚਿਆ = 1;
	ਬਣਤਰ cfs_rq *cfs_rq;
	ਬਣਤਰ rq *rq;

	guard(rcu)();

	list_for_each_ਇੰਦਰਾਜ_rcu(cfs_rq, &cfs_b->throttled_cfs_rq,
				throttled_list) {
		rq = rq_ਦਾ(cfs_rq);

		ਜੇ (!ਬਚਿਆ) {
			throttled = true;
			ਰੋਕੋ;
		}

		guard(rq_ਤਾਲਾ_irqsave)(rq);

		ਜੇ (!cfs_rq_throttled(cfs_rq))
			ਜਾਰੀ;

		/* Already queued for async unthrottle */
		ਜੇ (!list_ਖਾਲੀ_ਥਾਵਾਂ(&cfs_rq->throttled_csd_list))
			ਜਾਰੀ;

		ਜੇ (cfs_rq->h_curr) {
			update_rq_clock(rq);
			update_curr(cfs_rq);
		}

		/* By the above checks, this should never be true */
		WARN_ON_ONCE(cfs_rq->runtime_ਬਚਿਆ > 0);

		scoped_guard(raw_spinlock, &cfs_b->ਤਾਲਾ) {
			runtime = -cfs_rq->runtime_ਬਚਿਆ + 1;
			ਜੇ (runtime > cfs_b->runtime)
				runtime = cfs_b->runtime;
			cfs_b->runtime -= runtime;
			ਬਚਿਆ = cfs_b->runtime;
		}

		cfs_rq->runtime_ਬਚਿਆ += runtime;

		/*
		 * Ran out of bandwidth during distribution!
		 * Indicate throttled entities and break early.
		 */
		ਜੇ (cfs_rq->runtime_ਬਚਿਆ <= 0) {
			throttled = true;
			ਰੋਕੋ;
		}

		/* we check whether we're throttled above */
		ਜੇ (cpu_ਦਾ(rq) != this_cpu) {
			unthrottle_cfs_rq_async(cfs_rq);
			ਜਾਰੀ;
		}

		/*
		 * Allow a parallel async unthrottle to unthrottle
		 * this cfs_rq too via __cfsb_csd_unthrottle().
		 * If we are first, do it ourselves at the end and
		 * save on an IPI from remote CPUs.
		 */
		unthrottle_local = list_ਖਾਲੀ_ਥਾਵਾਂ(&rq->cfsb_csd_list);
		list_ਜੋੜੋ_ਪੂਛ(&cfs_rq->throttled_csd_list, &rq->cfsb_csd_list);
	}

	ਜੇ (unthrottle_local) {
		/*
		 * Protect against an IPI that is also trying to flush
		 * the unthrottled cfs_rq(s) from this CPU's csd_list.
		 */
		scoped_guard(irqsave)
			__cfsb_csd_unthrottle(cpu_rq(this_cpu));
	}

	ਵਾਪਸ throttled;
}

/*
 * Responsible for refilling a task_group's bandwidth and unthrottling its
 * cfs_rqs as appropriate. If there has been no activity within the last
 * period the timer is deactivated until scheduling resumes; cfs_b->idle is
 * used to track this state.
 */
ਸਥਿਰ ਪੂਰਨ do_sched_cfs_period_timer(ਬਣਤਰ cfs_bandwidth *cfs_b, ਪੂਰਨ overrun, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ flags)
	__must_hold(&cfs_b->ਤਾਲਾ)
{
	ਪੂਰਨ throttled;

	/* no need to continue the timer with no bandwidth constraint */
	ਜੇ (cfs_b->quota == RUNTIME_INF)
		ਜਾਓ ਬਾਹਰਲਾ_deactivate;

	throttled = !list_ਖਾਲੀ_ਥਾਵਾਂ(&cfs_b->throttled_cfs_rq);
	cfs_b->nr_periods += overrun;

	/* Refill extra burst quota even if cfs_b->idle */
	__refill_cfs_bandwidth_runtime(cfs_b);

	/*
	 * idle depends on !throttled (for the case of a large deficit), and if
	 * we're going inactive then everything else can be deferred
	 */
	ਜੇ (cfs_b->idle && !throttled)
		ਜਾਓ ਬਾਹਰਲਾ_deactivate;

	ਜੇ (!throttled) {
		/* mark as potentially idle for the upcoming period */
		cfs_b->idle = 1;
		ਵਾਪਸ 0;
	}

	/* account preceding periods in which throttling occurred */
	cfs_b->nr_throttled += overrun;

	/*
	 * This check is repeated as we release cfs_b->lock while we unthrottle.
	 */
	ਜਦੋਂ (throttled && cfs_b->runtime > 0) {
		raw_spin_unlock_irqrestore(&cfs_b->ਤਾਲਾ, flags);
		/* we can't nest cfs_b->lock while distributing bandwidth */
		throttled = distribute_cfs_runtime(cfs_b);
		raw_spin_ਤਾਲਾ_irqsave(&cfs_b->ਤਾਲਾ, flags);
	}

	/*
	 * While we are ensured activity in the period following an
	 * unthrottle, this also covers the case in which the new bandwidth is
	 * insufficient to cover the existing bandwidth deficit.  (Forcing the
	 * timer to remain active while there are any throttled entities.)
	 */
	cfs_b->idle = 0;

	ਵਾਪਸ 0;

ਬਾਹਰਲਾ_deactivate:
	ਵਾਪਸ 1;
}

/* a cfs_rq won't donate quota below this amount */
ਸਥਿਰ ਅਚਲ u64 min_cfs_rq_runtime = 1 * NSEC_ਪ੍ਰਤੀ_MSEC;
/* minimum remaining period time to redistribute slack quota */
ਸਥਿਰ ਅਚਲ u64 min_bandwidth_expiration = 2 * NSEC_ਪ੍ਰਤੀ_MSEC;
/* how long we wait to gather additional slack before distributing */
ਸਥਿਰ ਅਚਲ u64 cfs_bandwidth_slack_period = 5 * NSEC_ਪ੍ਰਤੀ_MSEC;

/*
 * Are we near the end of the current quota period?
 *
 * Requires cfs_b->lock for hrtimer_expires_remaining to be safe against the
 * hrtimer base being cleared by hrtimer_start. In the case of
 * migrate_hrtimers, base is never cleared, so we are fine.
 */
ਸਥਿਰ ਪੂਰਨ runtime_refresh_within(ਬਣਤਰ cfs_bandwidth *cfs_b, u64 min_expire)
{
	ਬਣਤਰ hrtimer *refresh_timer = &cfs_b->period_timer;
	s64 ਬਚਿਆ;

	/* if the call-back is running a quota refresh is already occurring */
	ਜੇ (hrtimer_callback_running(refresh_timer))
		ਵਾਪਸ 1;

	/* is a quota refresh about to occur? */
	ਬਚਿਆ = ktime_ਮੰਜ਼ਿਲ_ns(hrtimer_expires_ਬਚਿਆ(refresh_timer));
	ਜੇ (ਬਚਿਆ < (s64)min_expire)
		ਵਾਪਸ 1;

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ ਸ਼ੁਰੂ_cfs_slack_bandwidth(ਬਣਤਰ cfs_bandwidth *cfs_b)
{
	u64 min_ਖੱਬਾ = cfs_bandwidth_slack_period + min_bandwidth_expiration;

	/* if there's a quota refresh soon don't bother with slack */
	ਜੇ (runtime_refresh_within(cfs_b, min_ਖੱਬਾ))
		ਵਾਪਸ;

	/* don't push forwards an existing deferred unthrottle */
	ਜੇ (cfs_b->slack_started)
		ਵਾਪਸ;
	cfs_b->slack_started = true;

	hrtimer_ਸ਼ੁਰੂ(&cfs_b->slack_timer,
			ns_ਮੰਜ਼ਿਲ_ktime(cfs_bandwidth_slack_period),
			HRTIMER_ਬਹੁਲਕ_REL);
}

/* we know any runtime found here is valid as update_curr() precedes return */
ਸਥਿਰ ਖਾਲੀ __return_cfs_rq_runtime(ਬਣਤਰ cfs_rq *cfs_rq)
{
	ਬਣਤਰ cfs_bandwidth *cfs_b = tg_cfs_bandwidth(cfs_rq->tg);
	s64 slack_runtime = cfs_rq->runtime_ਬਚਿਆ - min_cfs_rq_runtime;

	ਜੇ (slack_runtime <= 0)
		ਵਾਪਸ;

	guard(raw_spinlock)(&cfs_b->ਤਾਲਾ);

	ਜੇ (cfs_b->quota != RUNTIME_INF) {
		cfs_b->runtime += slack_runtime;

		/* we are under rq->lock, defer unthrottling using a timer */
		ਜੇ (cfs_b->runtime > sched_cfs_bandwidth_ਟੁਕੜਾ() &&
		    !list_ਖਾਲੀ_ਥਾਵਾਂ(&cfs_b->throttled_cfs_rq))
			ਸ਼ੁਰੂ_cfs_slack_bandwidth(cfs_b);
	}

	/* even if it's not valid for return we don't want to try again */
	cfs_rq->runtime_ਬਚਿਆ -= slack_runtime;
}

ਸਥਿਰ __always_inline ਖਾਲੀ return_cfs_rq_runtime(ਬਣਤਰ cfs_rq *cfs_rq)
{
	ਜੇ (!cfs_bandwidth_ਵਰਤਿਆ())
		ਵਾਪਸ;

	ਜੇ (!cfs_rq->runtime_enabled || cfs_rq->nr_queued)
		ਵਾਪਸ;

	__return_cfs_rq_runtime(cfs_rq);
}

/*
 * This is done with a timer (instead of inline with bandwidth return) since
 * it's necessary to juggle rq->locks to unthrottle their respective cfs_rqs.
 */
ਸਥਿਰ ਖਾਲੀ do_sched_cfs_slack_timer(ਬਣਤਰ cfs_bandwidth *cfs_b)
{
	/* confirm we're still not at a refresh boundary */
	scoped_guard(raw_spinlock_irqsave, &cfs_b->ਤਾਲਾ) {
		u64 runtime = 0, ਟੁਕੜਾ = sched_cfs_bandwidth_ਟੁਕੜਾ();

		cfs_b->slack_started = false;

		ਜੇ (runtime_refresh_within(cfs_b, min_bandwidth_expiration))
			ਵਾਪਸ;

		ਜੇ (cfs_b->quota != RUNTIME_INF && cfs_b->runtime > ਟੁਕੜਾ)
			runtime = cfs_b->runtime;

		ਜੇ (!runtime)
			ਵਾਪਸ;
	}

	distribute_cfs_runtime(cfs_b);
}

/*
 * When a group wakes up we want to make sure that its quota is not already
 * expired/exceeded, otherwise it may be allowed to steal additional ticks of
 * runtime as update_curr() throttling can not trigger until it's on-rq.
 */
ਸਥਿਰ ਖਾਲੀ check_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_throttle(ਬਣਤਰ cfs_rq *cfs_rq)
{
	ਜੇ (!cfs_bandwidth_ਵਰਤਿਆ())
		ਵਾਪਸ;

	/* an active group must be handled by the update_curr() path */
	ਜੇ (!cfs_rq->runtime_enabled || cfs_rq->h_curr)
		ਵਾਪਸ;

	/* ensure the group is not already throttled */
	ਜੇ (cfs_rq_throttled(cfs_rq))
		ਵਾਪਸ;

	/* update runtime allocation */
	account_cfs_rq_runtime(cfs_rq, 0);
}

ਸਥਿਰ ਖਾਲੀ sync_throttle(ਬਣਤਰ task_ਸਮੂਹ *tg, ਪੂਰਨ cpu)
{
	ਬਣਤਰ cfs_rq *pcfs_rq, *cfs_rq;

	ਜੇ (!cfs_bandwidth_ਵਰਤਿਆ())
		ਵਾਪਸ;

	ਜੇ (!tg->ਮਾਪਾ)
		ਵਾਪਸ;

	cfs_rq = tg_cfs_rq(tg, cpu);
	pcfs_rq = tg_cfs_rq(tg->ਮਾਪਾ, cpu);

	cfs_rq->throttle_ਗਿਣਤੀ = pcfs_rq->throttle_ਗਿਣਤੀ;
	cfs_rq->throttled_clock_pelt = rq_clock_pelt(cpu_rq(cpu));

	/*
	 * It is not enough to sync the "pelt_clock_throttled" indicator
	 * with the parent cfs_rq when the hierarchy is not queued.
	 * Always join a throttled hierarchy with PELT clock throttled
	 * and leaf it to the first enqueue, or distribution to
	 * unthrottle the PELT clock.
	 */
	ਜੇ (cfs_rq->throttle_ਗਿਣਤੀ)
		cfs_rq->pelt_clock_throttled = 1;
}

ਸਥਿਰ ਗਿਣਤੀ hrtimer_restart sched_cfs_slack_timer(ਬਣਤਰ hrtimer *timer)
{
	ਬਣਤਰ cfs_bandwidth *cfs_b =
		container_ਦਾ(timer, ਬਣਤਰ cfs_bandwidth, slack_timer);

	do_sched_cfs_slack_timer(cfs_b);

	ਵਾਪਸ HRTIMER_NORESTART;
}

ਸਥਿਰ ਗਿਣਤੀ hrtimer_restart sched_cfs_period_timer(ਬਣਤਰ hrtimer *timer)
{
	ਬਣਤਰ cfs_bandwidth *cfs_b =
		container_ਦਾ(timer, ਬਣਤਰ cfs_bandwidth, period_timer);
	ਪੂਰਨ overrun;
	ਪੂਰਨ idle = 0;
	ਪੂਰਨ ਗਿਣਤੀ_2 = 0;

	CLASS(raw_spinlock_irqsave, cfsb_guard)(&cfs_b->ਤਾਲਾ);

	ਲਈ (;;) {
		overrun = hrtimer_forward_now(timer, cfs_b->period);
		ਜੇ (!overrun)
			ਰੋਕੋ;

		idle = do_sched_cfs_period_timer(cfs_b, overrun, cfsb_guard.flags);

		ਜੇ (++ਗਿਣਤੀ_2 > 3) {
			u64 new, old = ktime_ਮੰਜ਼ਿਲ_ns(cfs_b->period);

			/*
			 * Grow period by a factor of 2 to avoid losing precision.
			 * Precision loss in the quota/period ratio can cause __cfs_schedulable
			 * to fail.
			 */
			new = old * 2;
			ਜੇ (new < max_bw_quota_period_us * NSEC_ਪ੍ਰਤੀ_USEC) {
				cfs_b->period = ns_ਮੰਜ਼ਿਲ_ktime(new);
				cfs_b->quota *= 2;
				cfs_b->ਮਿਆਦ *= 2;

				pr_warn_ratelimited(
	"cfs_period_timer[cpu%d]: period too short, scaling up (new cfs_period_us = %lld, cfs_quota_us = %lld)\n",
					smp_processor_id(),
					div_u64(new, NSEC_ਪ੍ਰਤੀ_USEC),
					div_u64(cfs_b->quota, NSEC_ਪ੍ਰਤੀ_USEC));
			} ਨਹੀਂਤਾਂ {
				pr_warn_ratelimited(
	"cfs_period_timer[cpu%d]: period too short, but cannot scale up without losing precision (cfs_period_us = %lld, cfs_quota_us = %lld)\n",
					smp_processor_id(),
					div_u64(old, NSEC_ਪ੍ਰਤੀ_USEC),
					div_u64(cfs_b->quota, NSEC_ਪ੍ਰਤੀ_USEC));
			}

			/* reset count so we don't come right back in here */
			ਗਿਣਤੀ_2 = 0;
		}
	}

	ਜੇ (idle) {
		cfs_b->period_ਸਰਗਰਮ = 0;
		ਵਾਪਸ HRTIMER_NORESTART;
	}

	ਵਾਪਸ HRTIMER_RESTART;
}

ਖਾਲੀ init_cfs_bandwidth(ਬਣਤਰ cfs_bandwidth *cfs_b, ਬਣਤਰ cfs_bandwidth *ਮਾਪਾ)
{
	raw_spin_ਤਾਲਾ_init(&cfs_b->ਤਾਲਾ);
	cfs_b->runtime = 0;
	cfs_b->quota = RUNTIME_INF;
	cfs_b->period = us_ਮੰਜ਼ਿਲ_ktime(default_bw_period_us());
	cfs_b->ਮਿਆਦ = 0;
	cfs_b->ਦਰਜਾਵਾਰ_quota = ਮਾਪਾ ? ਮਾਪਾ->ਦਰਜਾਵਾਰ_quota : RUNTIME_INF;

	INIT_LIST_ਸਿਰ(&cfs_b->throttled_cfs_rq);
	hrtimer_ਤਿਆਰੀ(&cfs_b->period_timer, sched_cfs_period_timer, CLOCK_MONOTONIC,
		      HRTIMER_ਬਹੁਲਕ_ABS_PINNED);

	/* Add a random offset so that timers interleave */
	hrtimer_ਸੈੱਟ_expires(&cfs_b->period_timer,
			    get_random_u32_ਹੇਠਾਂ(cfs_b->period));
	hrtimer_ਤਿਆਰੀ(&cfs_b->slack_timer, sched_cfs_slack_timer, CLOCK_MONOTONIC,
		      HRTIMER_ਬਹੁਲਕ_REL);
	cfs_b->slack_started = false;
}

ਸਥਿਰ ਖਾਲੀ init_cfs_rq_runtime(ਬਣਤਰ cfs_rq *cfs_rq)
{
	cfs_rq->runtime_enabled = 0;
	INIT_LIST_ਸਿਰ(&cfs_rq->throttled_list);
	INIT_LIST_ਸਿਰ(&cfs_rq->throttled_csd_list);
	INIT_LIST_ਸਿਰ(&cfs_rq->throttled_limbo_list);
}

ਖਾਲੀ ਸ਼ੁਰੂ_cfs_bandwidth(ਬਣਤਰ cfs_bandwidth *cfs_b)
{
	lockdep_assert_held(&cfs_b->ਤਾਲਾ);

	ਜੇ (cfs_b->period_ਸਰਗਰਮ)
		ਵਾਪਸ;

	cfs_b->period_ਸਰਗਰਮ = 1;
	hrtimer_forward_now(&cfs_b->period_timer, cfs_b->period);
	hrtimer_ਸ਼ੁਰੂ_expires(&cfs_b->period_timer, HRTIMER_ਬਹੁਲਕ_ABS_PINNED);
}

ਸਥਿਰ ਖਾਲੀ destroy_cfs_bandwidth(ਬਣਤਰ cfs_bandwidth *cfs_b)
{
	ਪੂਰਨ __maybe_unused i;

	/* init_cfs_bandwidth() was not called */
	ਜੇ (!cfs_b->throttled_cfs_rq.ਅਗਲਾ)
		ਵਾਪਸ;

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
		ਬਣਤਰ rq *rq = cpu_rq(i);

		ਜੇ (list_ਖਾਲੀ_ਥਾਵਾਂ(&rq->cfsb_csd_list))
			ਜਾਰੀ;

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
ਸਥਿਰ ਖਾਲੀ __maybe_unused update_runtime_enabled(ਬਣਤਰ rq *rq)
{
	ਬਣਤਰ task_ਸਮੂਹ *tg;

	lockdep_assert_rq_held(rq);

	guard(rcu)();

	list_for_each_ਇੰਦਰਾਜ_rcu(tg, &task_groups, list) {
		ਬਣਤਰ cfs_bandwidth *cfs_b = &tg->cfs_bandwidth;
		ਬਣਤਰ cfs_rq *cfs_rq = tg_cfs_rq(tg, cpu_ਦਾ(rq));

		scoped_guard(raw_spinlock, &cfs_b->ਤਾਲਾ)
			cfs_rq->runtime_enabled = cfs_b->quota != RUNTIME_INF;
	}
}

/* cpu offline callback */
ਸਥਿਰ ਖਾਲੀ __maybe_unused unthrottle_offline_cfs_rqs(ਬਣਤਰ rq *rq)
{
	ਬਣਤਰ task_ਸਮੂਹ *tg;

	lockdep_assert_rq_held(rq);

	// Do not unthrottle for an active CPU
	ਜੇ (cpumask_test_cpu(cpu_ਦਾ(rq), cpu_ਸਰਗਰਮ_ਨਕਾਬ))
		ਵਾਪਸ;

	/*
	 * The rq clock has already been updated in the
	 * set_rq_offline(), so we should skip updating
	 * the rq clock again in unthrottle_cfs_rq().
	 */
	rq_clock_ਸ਼ੁਰੂ_loop_update(rq);

	guard(rcu)();

	list_for_each_ਇੰਦਰਾਜ_rcu(tg, &task_groups, list) {
		ਬਣਤਰ cfs_rq *cfs_rq = tg_cfs_rq(tg, cpu_ਦਾ(rq));

		ਜੇ (!cfs_rq->runtime_enabled)
			ਜਾਰੀ;

		/*
		 * Offline rq is schedulable till CPU is completely disabled
		 * in take_cpu_down(), so we prevent new cfs throttling here.
		 */
		cfs_rq->runtime_enabled = 0;

		ਜੇ (!cfs_rq_throttled(cfs_rq))
			ਜਾਰੀ;

		/*
		 * clock_task is not advancing so we just need to make sure
		 * there's some valid quota amount
		 */
		cfs_rq->runtime_ਬਚਿਆ = 1;
		unthrottle_cfs_rq(cfs_rq);
	}

	rq_clock_stop_loop_update(rq);
}

bool cfs_task_bw_constrained(ਬਣਤਰ task_struct *p)
{
	ਬਣਤਰ cfs_rq *cfs_rq = task_cfs_rq(p);

	ਜੇ (!cfs_bandwidth_ਵਰਤਿਆ())
		ਵਾਪਸ false;

	ਜੇ (cfs_rq->runtime_enabled ||
	    tg_cfs_bandwidth(cfs_rq->tg)->ਦਰਜਾਵਾਰ_quota != RUNTIME_INF)
		ਵਾਪਸ true;

	ਵਾਪਸ false;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_NO_HZ_ਭਰਿਆ
/* called from pick_next_task_fair() */
ਸਥਿਰ ਖਾਲੀ sched_fair_update_stop_tick(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p)
{
	ਪੂਰਨ cpu = cpu_ਦਾ(rq);

	ਜੇ (!cfs_bandwidth_ਵਰਤਿਆ())
		ਵਾਪਸ;

	ਜੇ (!tick_nohz_ਭਰਿਆ_cpu(cpu))
		ਵਾਪਸ;

	ਜੇ (rq->nr_running != 1)
		ਵਾਪਸ;

	/*
	 *  We know there is only one task runnable and we've just picked it. The
	 *  normal enqueue path will have cleared TICK_DEP_BIT_SCHED if we will
	 *  be otherwise able to stop the tick. Just need to check if we are using
	 *  bandwidth control.
	 */
	ਜੇ (cfs_task_bw_constrained(p))
		tick_nohz_dep_ਸੈੱਟ_cpu(cpu, TICK_DEP_ਦੋਆਧਾਰੀ_ਅੰਕ_SCHED);
}
#ਜੇ_ਅੰਤ /* CONFIG_NO_HZ_FULL */

#ਨਹੀਂਤਾਂ /* !CONFIG_CFS_BANDWIDTH: */

ਸਥਿਰ bool account_cfs_rq_runtime(ਬਣਤਰ cfs_rq *cfs_rq, u64 delta_exec) { ਵਾਪਸ false; }
ਸਥਿਰ ਖਾਲੀ check_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_throttle(ਬਣਤਰ cfs_rq *cfs_rq) {}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ sync_throttle(ਬਣਤਰ task_ਸਮੂਹ *tg, ਪੂਰਨ cpu) {}
ਸਥਿਰ __always_inline ਖਾਲੀ return_cfs_rq_runtime(ਬਣਤਰ cfs_rq *cfs_rq) {}
ਸਥਿਰ ਖਾਲੀ task_throttle_ਤਿਆਰੀ_work(ਬਣਤਰ task_struct *p) {}
ਸਥਿਰ bool task_is_throttled(ਬਣਤਰ task_struct *p) { ਵਾਪਸ false; }
ਸਥਿਰ ਖਾਲੀ ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_throttled_task(ਬਣਤਰ task_struct *p, ਪੂਰਨ flags) {}
ਸਥਿਰ bool ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_throttled_task(ਬਣਤਰ task_struct *p) { ਵਾਪਸ false; }
ਸਥਿਰ ਖਾਲੀ ਰਿਕਾਰਡ_throttle_clock(ਬਣਤਰ cfs_rq *cfs_rq) {}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ cfs_rq_throttled(ਬਣਤਰ cfs_rq *cfs_rq)
{
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool cfs_rq_pelt_clock_throttled(ਬਣਤਰ cfs_rq *cfs_rq)
{
	ਵਾਪਸ false;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ throttled_hierarchy(ਬਣਤਰ cfs_rq *cfs_rq)
{
	ਵਾਪਸ 0;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ lb_throttled_hierarchy(ਬਣਤਰ task_struct *p, ਪੂਰਨ dst_cpu)
{
	ਵਾਪਸ 0;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_FAIR_ਸਮੂਹ_SCHED
ਖਾਲੀ init_cfs_bandwidth(ਬਣਤਰ cfs_bandwidth *cfs_b, ਬਣਤਰ cfs_bandwidth *ਮਾਪਾ) {}
ਸਥਿਰ ਖਾਲੀ init_cfs_rq_runtime(ਬਣਤਰ cfs_rq *cfs_rq) {}
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਣਤਰ cfs_bandwidth *tg_cfs_bandwidth(ਬਣਤਰ task_ਸਮੂਹ *tg)
{
	ਵਾਪਸ ਨਿਰਾ;
}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ destroy_cfs_bandwidth(ਬਣਤਰ cfs_bandwidth *cfs_b) {}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ update_runtime_enabled(ਬਣਤਰ rq *rq) {}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ unthrottle_offline_cfs_rqs(ਬਣਤਰ rq *rq) {}
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_CGROUP_SCHED
bool cfs_task_bw_constrained(ਬਣਤਰ task_struct *p)
{
	ਵਾਪਸ false;
}
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ /* !CONFIG_CFS_BANDWIDTH */

#ਜੇ !defined(CONFIG_CFS_BANDWIDTH) || !defined(CONFIG_NO_HZ_ਭਰਿਆ)
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ sched_fair_update_stop_tick(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p) {}
#ਜੇ_ਅੰਤ

/**************************************************
 * CFS operations on tasks:
 */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_HRTICK
ਸਥਿਰ ਖਾਲੀ hrtick_ਸ਼ੁਰੂ_fair(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p)
{
	ਬਣਤਰ sched_entity *se = &p->se;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ scale = 1024;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ util = 0;
	u64 vdelta;
	u64 delta;

	WARN_ON_ONCE(task_rq(p) != rq);

	ਜੇ (rq->cfs.h_nr_queued <= 1)
		ਵਾਪਸ;

	/*
	 * Compute time until virtual deadline
	 */
	vdelta = se->deadline - se->vruntime;
	ਜੇ ((s64)vdelta < 0) {
		ਜੇ (task_current_donor(rq, p))
			resched_curr(rq);
		ਵਾਪਸ;
	}
	delta = (se->h_ਲੱਦੋ.ਭਾਰ * vdelta) / NICE_0_ਲੱਦੋ;

	/*
	 * Correct for instantaneous load of other classes.
	 */
	util += cpu_util_irq(rq);
	ਜੇ (util && util < 1024) {
		scale *= 1024;
		scale /= (1024 - util);
	}

	hrtick_ਸ਼ੁਰੂ(rq, (scale * delta) / 1024);
}

/*
 * Called on enqueue to start the hrtick when h_nr_queued becomes more than 1.
 */
ਸਥਿਰ ਖਾਲੀ hrtick_update(ਬਣਤਰ rq *rq)
{
	ਬਣਤਰ task_struct *donor = rq->donor;

	ਜੇ (!hrtick_enabled_fair(rq) || donor->sched_class != &fair_sched_class)
		ਵਾਪਸ;

	ਜੇ (hrtick_ਸਰਗਰਮ(rq))
		ਵਾਪਸ;

	hrtick_ਸ਼ੁਰੂ_fair(rq, donor);
}
#ਨਹੀਂਤਾਂ /* !CONFIG_SCHED_HRTICK: */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hrtick_ਸ਼ੁਰੂ_fair(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p)
{
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ hrtick_update(ਬਣਤਰ rq *rq)
{
}
#ਜੇ_ਅੰਤ /* !CONFIG_SCHED_HRTICK */

ਸਥਿਰ ਸਤਰਅੰਦਰ bool cpu_overutilized(ਪੂਰਨ cpu)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ rq_util_max;

	ਜੇ (!sched_energy_enabled())
		ਵਾਪਸ false;

	rq_util_max = uclamp_rq_get(cpu_rq(cpu), UCLAMP_MAX);

	/* Return true only if the utilization doesn't fit CPU's capacity */
	ਵਾਪਸ !util_ਸਮਾਉਂਦਾ_cpu(cpu_util_cfs(cpu), 0, rq_util_max, cpu);
}

/*
 * overutilized value make sense only if EAS is enabled
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ bool is_rd_overutilized(ਬਣਤਰ ਜੜ੍ਹ_domain *rd)
{
	ਵਾਪਸ !sched_energy_enabled() || READ_ONCE(rd->overutilized);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ ਸੈੱਟ_rd_overutilized(ਬਣਤਰ ਜੜ੍ਹ_domain *rd, bool flag)
{
	ਜੇ (!sched_energy_enabled())
		ਵਾਪਸ;

	WRITE_ONCE(rd->overutilized, flag);
	trace_sched_overutilized_tp(rd, flag);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ check_update_overutilized_ਸਥਿਤੀ(ਬਣਤਰ rq *rq)
{
	/*
	 * overutilized field is used for load balancing decisions only
	 * if energy aware scheduler is being used
	 */

	ਜੇ (!is_rd_overutilized(rq->rd) && cpu_overutilized(rq->cpu))
		ਸੈੱਟ_rd_overutilized(rq->rd, 1);
}

/* Runqueue only has SCHED_IDLE tasks enqueued */
ਸਥਿਰ ਪੂਰਨ sched_idle_rq(ਬਣਤਰ rq *rq)
{
	ਵਾਪਸ unlikely(rq->nr_running == rq->cfs.h_nr_idle &&
			rq->nr_running);
}

ਸਥਿਰ ਪੂਰਨ ਚੁਣੋ_sched_idle_rq(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p)
{
	ਵਾਪਸ sched_idle_rq(rq) && !task_has_idle_policy(p);
}

ਸਥਿਰ ਪੂਰਨ ਚੁਣੋ_idle_cpu(ਪੂਰਨ cpu, ਬਣਤਰ task_struct *p)
{
	ਵਾਪਸ ਉਪਲਬਧ_idle_cpu(cpu) ||
	       ਚੁਣੋ_sched_idle_rq(cpu_rq(cpu), p);
}

ਸਥਿਰ ਖਾਲੀ
requeue_delayed_entity(ਬਣਤਰ cfs_rq *cfs_rq, ਬਣਤਰ sched_entity *se)
{
	/*
	 * se->sched_delayed should imply: se->on_rq == 1.
	 * Because a delayed entity is one that is still on
	 * the runqueue competing until elegibility.
	 */
	WARN_ON_ONCE(!se->sched_delayed);
	WARN_ON_ONCE(!se->on_rq);

	ਜੇ (update_entity_lag(cfs_rq, se)) {
		cfs_rq->h_nr_queued--;
		ਜੇ (se != cfs_rq->curr)
			__dequeue_entity(cfs_rq, se);
		ਰੱਖੋ_entity(cfs_rq, se, 0);
		ਜੇ (se != cfs_rq->curr)
			__enqueue_entity(cfs_rq, se);
		cfs_rq->h_nr_queued++;
	}

	update_ਲੱਦੋ_avg(cfs_rq, se, 0);
	clear_delayed(se);
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_hierarchy(ਬਣਤਰ task_struct *p, ਪੂਰਨ flags)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਭਾਰ = NICE_0_ਲੱਦੋ;
	ਪੂਰਨ task_new = !(flags & ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_WAKEUP);
	ਬਣਤਰ sched_entity *se = &p->se;
	ਪੂਰਨ h_nr_idle = task_has_idle_policy(p);
	ਪੂਰਨ h_nr_runnable = 1;

	ਜੇ (task_new && se->sched_delayed)
		h_nr_runnable = 0;

	for_each_sched_entity(se) {
		ਬਣਤਰ cfs_rq *cfs_rq = cfs_rq_ਦਾ(se);

		update_curr(cfs_rq);

		ਜੇ (!se->on_rq) {
			ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_entity(cfs_rq, se, flags);
		} ਨਹੀਂਤਾਂ {
			update_ਲੱਦੋ_avg(cfs_rq, se, UPDATE_TG);
			se_update_runnable(se);
			update_cfs_ਸਮੂਹ(se);
		}

		cfs_rq->h_nr_runnable += h_nr_runnable;
		cfs_rq->h_nr_queued++;
		cfs_rq->h_nr_idle += h_nr_idle;

		ਜੇ (cfs_rq_is_idle(cfs_rq))
			h_nr_idle = 1;

		ਭਾਰ = __calc_prop_weight(cfs_rq, se, ਭਾਰ);

		flags = ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_WAKEUP;
	}

	ਵਾਪਸ ਭਾਰ;
}

/* Update curr's vruntime before placing entity or updating lag */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ update_curr_eevdf(ਬਣਤਰ cfs_rq *cfs_rq)
{
	ਜੇ (!cfs_rq->curr)
		ਵਾਪਸ;

	update_curr(cfs_rq_ਦਾ(cfs_rq->curr));
}

/*
 * The enqueue_task method is called before nr_running is
 * increased. Here we update the fair scheduling stats and
 * then put the task into the rbtree:
 */
ਸਥਿਰ ਖਾਲੀ
ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_task_fair(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, ਪੂਰਨ flags)
{
	ਪੂਰਨ rq_h_nr_queued = rq->cfs.h_nr_queued;
	ਪੂਰਨ task_new = !(flags & ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_WAKEUP);
	ਬਣਤਰ sched_entity *se = &p->se;
	ਬਣਤਰ cfs_rq *cfs_rq = &rq->cfs;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਭਾਰ;
	bool curr;

	ਜੇ (task_is_throttled(p) && ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_throttled_task(p))
		ਵਾਪਸ;

	/*
	 * The code below (indirectly) updates schedutil which looks at
	 * the cfs_rq utilization to select a frequency.
	 * Let's add the task's estimated utilization to the cfs_rq's
	 * estimated utilization, before we update schedutil.
	 */
	ਜੇ (!p->se.sched_delayed || (flags & ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_DELAYED))
		util_est_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ(cfs_rq, p);

	update_curr_eevdf(cfs_rq);

	ਜੇ (flags & ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_DELAYED) {
		requeue_delayed_entity(cfs_rq, se);
		ਵਾਪਸ;
	}

	/*
	 * If in_iowait is set, the code below may not trigger any cpufreq
	 * utilization updates, so do it here explicitly with the IOWAIT flag
	 * passed.
	 */
	ਜੇ (p->ਵਿੱਚ_iowait)
		cpufreq_update_util(rq, SCHED_CPUFREQ_IOWAIT);

	curr = (cfs_rq->curr == se);
	/*
	 * This can actually happen with CFS_BANDWIDTH, a task can be throttled
	 * and dequeued, unthrottled from another CPU and then enqueued again
	 * before schedule().
	 */
	ਜੇ (curr)
		ਰੱਖੋ_entity(cfs_rq, se, flags);

	ਜੇ (se->on_rq && se->sched_delayed)
		requeue_delayed_entity(cfs_rq, se);

	ਭਾਰ = ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_hierarchy(p, flags);

	ਜੇ (!curr) {
		reweight_eevdf(cfs_rq, se, ਭਾਰ, false);
		ਰੱਖੋ_entity(cfs_rq, se, flags | ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_QUEUED);
		__enqueue_entity(cfs_rq, se);
	}

	ਜੇ (!rq_h_nr_queued && rq->cfs.h_nr_queued)
		dl_server_ਸ਼ੁਰੂ(&rq->fair_server);

	/* At this point se is NULL and we are at root level*/
	ਜੋੜੋ_nr_running(rq, 1);

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
	ਜੇ (!task_new)
		check_update_overutilized_ਸਥਿਤੀ(rq);

	assert_list_ਪੱਤੀ_cfs_rq(rq);

	hrtick_update(rq);
}

ਸਥਿਰ ਖਾਲੀ ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_hierarchy(ਬਣਤਰ task_struct *p, ਪੂਰਨ flags)
{
	ਬਣਤਰ sched_entity *se = &p->se;
	bool task_sleep = flags & ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_SLEEP;
	bool task_delayed = flags & ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_DELAYED;
	bool task_throttled = flags & ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_THROTTLE;
	ਪੂਰਨ h_nr_runnable = 0;
	ਪੂਰਨ h_nr_idle = task_has_idle_policy(p);
	bool ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ = true;

	ਜੇ (task_sleep || task_delayed || !se->sched_delayed)
		h_nr_runnable = 1;

	for_each_sched_entity(se) {
		ਬਣਤਰ cfs_rq *cfs_rq = cfs_rq_ਦਾ(se);

		update_curr(cfs_rq);

		ਜੇ (ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ) {
			ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_entity(cfs_rq, se, flags);
			/* Don't dequeue parent if it has other entities besides us */
			ਜੇ (cfs_rq->ਲੱਦੋ.ਭਾਰ)
				ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ = false;
		} ਨਹੀਂਤਾਂ {
			update_ਲੱਦੋ_avg(cfs_rq, se, UPDATE_TG);
			se_update_runnable(se);
			update_cfs_ਸਮੂਹ(se);
		}

		cfs_rq->h_nr_runnable -= h_nr_runnable;
		cfs_rq->h_nr_queued--;
		cfs_rq->h_nr_idle -= h_nr_idle;

		ਜੇ (cfs_rq_is_idle(cfs_rq))
			h_nr_idle = 1;

		ਜੇ (throttled_hierarchy(cfs_rq) && task_throttled)
			ਰਿਕਾਰਡ_throttle_clock(cfs_rq);

		flags |= ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_SLEEP;
		flags &= ~(ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_DELAYED | ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_SPECIAL);
	}
}

/*
 * The part of dequeue_task_fair() that is needed to dequeue delayed tasks.
 *
 * Returns:
 *   true  - dequeued
 *   false - delayed
 */
ਸਥਿਰ bool __dequeue_task(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, ਪੂਰਨ flags)
{
	ਬਣਤਰ sched_entity *se = &p->se;
	ਬਣਤਰ cfs_rq *cfs_rq = &rq->cfs;
	bool was_sched_idle = sched_idle_rq(rq);
	bool task_sleep = flags & ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_SLEEP;
	bool task_delayed = flags & ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_DELAYED;

	clear_buddies(cfs_rq, se);

	update_curr_eevdf(cfs_rq);
	update_entity_lag(cfs_rq, se);

	ਜੇ (flags & ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_DELAYED) {
		WARN_ON_ONCE(!se->sched_delayed);
	} ਨਹੀਂਤਾਂ {
		bool delay = task_sleep;
		/*
		 * DELAY_DEQUEUE relies on spurious wakeups, special task
		 * states must not suffer spurious wakeups, excempt them.
		 */
		ਜੇ (flags & (ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_SPECIAL | ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_THROTTLE))
			delay = false;

		WARN_ON_ONCE(delay && se->sched_delayed);

		ਜੇ (sched_feat(DELAY_ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ) && delay &&
		    !entity_eligible(cfs_rq, se)) {
			update_ਲੱਦੋ_avg(cfs_rq_ਦਾ(se), se, UPDATE_UTIL_EST);
			ਸੈੱਟ_delayed(se);
			ਵਾਪਸ false;
		}
	}

	ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_hierarchy(p, flags);

	ਜੇ (sched_feat(ਰੱਖੋ_REL_DEADLINE) && !task_sleep) {
		se->deadline -= se->vruntime;
		se->rel_deadline = 1;
	}
	ਜੇ (se != cfs_rq->curr)
		__dequeue_entity(cfs_rq, se);

	sub_nr_running(rq, 1);

	/* balance early to pull high priority tasks */
	ਜੇ (unlikely(!was_sched_idle && sched_idle_rq(rq)))
		rq->ਅਗਲਾ_balance = jiffies;

	ਜੇ (task_delayed) {
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

	ਵਾਪਸ true;
}

/*
 * The dequeue_task method is called before nr_running is
 * decreased. We remove the task from the rbtree and
 * update the fair scheduling stats:
 */
ਸਥਿਰ bool ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_task_fair(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, ਪੂਰਨ flags)
{
	ਜੇ (task_is_throttled(p)) {
		ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_throttled_task(p, flags);
		ਵਾਪਸ true;
	}

	ਜੇ (!p->se.sched_delayed)
		util_est_ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ(&rq->cfs, p);

	ਜੇ (!__dequeue_task(rq, p, flags))
		ਵਾਪਸ false;

	/*
	 * Must not reference @p after __dequeue_task(DEQUEUE_DELAYED).
	 */
	ਵਾਪਸ true;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cfs_h_nr_delayed(ਬਣਤਰ rq *rq)
{
	ਵਾਪਸ (rq->cfs.h_nr_queued - rq->cfs.h_nr_runnable);
}

/* Working cpumask for: sched_balance_rq(), sched_balance_newidle(). */
ਸਥਿਰ DEFINE_ਪ੍ਰਤੀ_CPU(cpumask_var_t, ਲੱਦੋ_balance_ਨਕਾਬ);
ਸਥਿਰ DEFINE_ਪ੍ਰਤੀ_CPU(cpumask_var_t, select_rq_ਨਕਾਬ);
ਸਥਿਰ DEFINE_ਪ੍ਰਤੀ_CPU(cpumask_var_t, should_we_balance_tmpmask);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_NO_HZ_COMMON

ਸਥਿਰ ਬਣਤਰ {
	cpumask_var_t idle_cpus_ਨਕਾਬ;
	ਪੂਰਨ has_blocked_ਲੱਦੋ;		/* Idle CPUS has blocked load */
	ਪੂਰਨ needs_update;		/* Newly idle CPUs need their next_balance collated */
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਅਗਲਾ_balance;     /* in jiffy units */
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਅਗਲਾ_blocked;	/* Next update of blocked load in jiffies */
} nohz ____cacheline_aligned;

#ਜੇ_ਅੰਤ /* CONFIG_NO_HZ_COMMON */

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ cpu_ਲੱਦੋ(ਬਣਤਰ rq *rq)
{
	ਵਾਪਸ cfs_rq_ਲੱਦੋ_avg(&rq->cfs);
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
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ cpu_ਲੱਦੋ_without(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p)
{
	ਬਣਤਰ cfs_rq *cfs_rq;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਲੱਦੋ;

	/* Task has no contribution or is new */
	ਜੇ (cpu_ਦਾ(rq) != task_cpu(p) || !READ_ONCE(p->se.avg.ਆਖਰੀ_update_ਸਮਾਂ))
		ਵਾਪਸ cpu_ਲੱਦੋ(rq);

	cfs_rq = &rq->cfs;
	ਲੱਦੋ = READ_ONCE(cfs_rq->avg.ਲੱਦੋ_avg);

	/* Discount task's util from CPU's util */
	lsub_ਸਕਾਰਾਤਮਕ(&ਲੱਦੋ, task_h_ਲੱਦੋ(p));

	ਵਾਪਸ ਲੱਦੋ;
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ cpu_runnable(ਬਣਤਰ rq *rq)
{
	ਵਾਪਸ cfs_rq_runnable_avg(&rq->cfs);
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ cpu_runnable_without(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p)
{
	ਬਣਤਰ cfs_rq *cfs_rq;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ runnable;

	/* Task has no contribution or is new */
	ਜੇ (cpu_ਦਾ(rq) != task_cpu(p) || !READ_ONCE(p->se.avg.ਆਖਰੀ_update_ਸਮਾਂ))
		ਵਾਪਸ cpu_runnable(rq);

	cfs_rq = &rq->cfs;
	runnable = READ_ONCE(cfs_rq->avg.runnable_avg);

	/* Discount task's runnable from CPU's runnable */
	lsub_ਸਕਾਰਾਤਮਕ(&runnable, p->se.avg.runnable_avg);

	ਵਾਪਸ runnable;
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਸਮਰੱਥਾ_ਦਾ(ਪੂਰਨ cpu)
{
	ਵਾਪਸ cpu_rq(cpu)->cpu_ਸਮਰੱਥਾ;
}

ਸਥਿਰ ਖਾਲੀ ਰਿਕਾਰਡ_wakee(ਬਣਤਰ task_struct *p)
{
	/*
	 * Only decay a single time; tasks that have less then 1 wakeup per
	 * jiffy will not have built up many flips.
	 */
	ਜੇ (ਸਮਾਂ_after(jiffies, current->wakee_flip_decay_ts + HZ)) {
		current->wakee_flips >>= 1;
		current->wakee_flip_decay_ts = jiffies;
	}

	ਜੇ (current->ਆਖਰੀ_wakee != p) {
		current->ਆਖਰੀ_wakee = p;
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
ਸਥਿਰ ਪੂਰਨ wake_wide(ਬਣਤਰ task_struct *p)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ master = current->wakee_flips;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ slave = p->wakee_flips;
	ਪੂਰਨ ਗੁਣਕ = __this_cpu_read(sd_llc_ਮਾਪ);

	ਜੇ (master < slave)
		ਅਦਲਾ_ਬਦਲੀ(master, slave);
	ਜੇ (slave < ਗੁਣਕ || master < slave * ਗੁਣਕ)
		ਵਾਪਸ 0;
	ਵਾਪਸ 1;
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
ਸਥਿਰ ਪੂਰਨ
wake_affine_idle(ਪੂਰਨ this_cpu, ਪੂਰਨ prev_cpu, ਪੂਰਨ sync)
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
	ਜੇ (ਉਪਲਬਧ_idle_cpu(this_cpu) && cpus_share_cache(this_cpu, prev_cpu))
		ਵਾਪਸ ਉਪਲਬਧ_idle_cpu(prev_cpu) ? prev_cpu : this_cpu;

	ਜੇ (sync) {
		ਬਣਤਰ rq *rq = cpu_rq(this_cpu);

		ਜੇ ((rq->nr_running - cfs_h_nr_delayed(rq)) == 1)
			ਵਾਪਸ this_cpu;
	}

	ਜੇ (ਉਪਲਬਧ_idle_cpu(prev_cpu))
		ਵਾਪਸ prev_cpu;

	ਵਾਪਸ nr_cpumask_ਬਿੱਟ;
}

ਸਥਿਰ ਪੂਰਨ
wake_affine_ਭਾਰ(ਬਣਤਰ sched_domain *sd, ਬਣਤਰ task_struct *p,
		   ਪੂਰਨ this_cpu, ਪੂਰਨ prev_cpu, ਪੂਰਨ sync)
{
	s64 this_eff_ਲੱਦੋ, prev_eff_ਲੱਦੋ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ task_ਲੱਦੋ;

	this_eff_ਲੱਦੋ = cpu_ਲੱਦੋ(cpu_rq(this_cpu));

	ਜੇ (sync) {
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ current_ਲੱਦੋ = task_h_ਲੱਦੋ(current);

		ਜੇ (current_ਲੱਦੋ > this_eff_ਲੱਦੋ)
			ਵਾਪਸ this_cpu;

		this_eff_ਲੱਦੋ -= current_ਲੱਦੋ;
	}

	task_ਲੱਦੋ = task_h_ਲੱਦੋ(p);

	this_eff_ਲੱਦੋ += task_ਲੱਦੋ;
	ਜੇ (sched_feat(WA_BIAS))
		this_eff_ਲੱਦੋ *= 100;
	this_eff_ਲੱਦੋ *= ਸਮਰੱਥਾ_ਦਾ(prev_cpu);

	prev_eff_ਲੱਦੋ = cpu_ਲੱਦੋ(cpu_rq(prev_cpu));
	prev_eff_ਲੱਦੋ -= task_ਲੱਦੋ;
	ਜੇ (sched_feat(WA_BIAS))
		prev_eff_ਲੱਦੋ *= 100 + (sd->imbalance_pct - 100) / 2;
	prev_eff_ਲੱਦੋ *= ਸਮਰੱਥਾ_ਦਾ(this_cpu);

	/*
	 * If sync, adjust the weight of prev_eff_load such that if
	 * prev_eff == this_eff that select_idle_sibling() will consider
	 * stacking the wakee on top of the waker if no other CPU is
	 * idle.
	 */
	ਜੇ (sync)
		prev_eff_ਲੱਦੋ += 1;

	ਵਾਪਸ this_eff_ਲੱਦੋ < prev_eff_ਲੱਦੋ ? this_cpu : nr_cpumask_ਬਿੱਟ;
}

ਸਥਿਰ ਪੂਰਨ wake_affine(ਬਣਤਰ sched_domain *sd, ਬਣਤਰ task_struct *p,
		       ਪੂਰਨ this_cpu, ਪੂਰਨ prev_cpu, ਪੂਰਨ sync)
{
	ਪੂਰਨ ਨਿਸ਼ਾਨਾ = nr_cpumask_ਬਿੱਟ;

	ਜੇ (sched_feat(WA_IDLE))
		ਨਿਸ਼ਾਨਾ = wake_affine_idle(this_cpu, prev_cpu, sync);

	ਜੇ (sched_feat(WA_ਭਾਰ) && ਨਿਸ਼ਾਨਾ == nr_cpumask_ਬਿੱਟ)
		ਨਿਸ਼ਾਨਾ = wake_affine_ਭਾਰ(sd, p, this_cpu, prev_cpu, sync);

	schedstat_inc(p->stats.nr_wakeups_affine_attempts);
	ਜੇ (ਨਿਸ਼ਾਨਾ != this_cpu)
		ਵਾਪਸ prev_cpu;

	schedstat_inc(sd->ttwu_move_affine);
	schedstat_inc(p->stats.nr_wakeups_affine);
	ਵਾਪਸ ਨਿਸ਼ਾਨਾ;
}

ਸਥਿਰ ਬਣਤਰ sched_ਸਮੂਹ *
sched_balance_ਲੱਭੋ_dst_ਸਮੂਹ(ਬਣਤਰ sched_domain *sd, ਬਣਤਰ task_struct *p, ਪੂਰਨ this_cpu);

/*
 * sched_balance_find_dst_group_cpu - find the idlest CPU among the CPUs in the group.
 */
ਸਥਿਰ ਪੂਰਨ
sched_balance_ਲੱਭੋ_dst_ਸਮੂਹ_cpu(ਬਣਤਰ sched_ਸਮੂਹ *ਸਮੂਹ, ਬਣਤਰ task_struct *p, ਪੂਰਨ this_cpu)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੱਦੋ, min_ਲੱਦੋ = ULONG_MAX;
	u64 min_exit_latency = U64_MAX;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ nr_candidates = 0;
	ਪੂਰਨ ਘੱਟ_ਤੋਂ_ਘੱਟ_loaded_cpu = this_cpu;
	ਪੂਰਨ shallowest_idle_cpu = -1;
	ਪੂਰਨ i;

	/* Check if we have any choice: */
	ਜੇ (ਸਮੂਹ->ਸਮੂਹ_ਭਾਰ == 1)
		ਵਾਪਸ cpumask_ਪਹਿਲਾ(sched_ਸਮੂਹ_ਵਿਸਤਾਰ(ਸਮੂਹ));

	/* Traverse only the allowed CPUs */
	for_each_cpu_and(i, sched_ਸਮੂਹ_ਵਿਸਤਾਰ(ਸਮੂਹ), p->cpus_ptr) {
		ਬਣਤਰ rq *rq = cpu_rq(i);

		ਜੇ (!sched_core_cookie_ਮੇਲ(rq, p))
			ਜਾਰੀ;

		ਜੇ (ਚੁਣੋ_sched_idle_rq(rq, p))
			ਵਾਪਸ i;

		ਜੇ (ਉਪਲਬਧ_idle_cpu(i)) {
			ਬਣਤਰ cpuidle_state *idle = idle_get_state(rq);
			u64 exit_latency = idle ? idle->exit_latency : U64_MAX;

			ਜੇ (shallowest_idle_cpu == -1 || exit_latency < min_exit_latency) {
				min_exit_latency = exit_latency;
				shallowest_idle_cpu = i;
				nr_candidates = 1;
			} ਨਹੀਂਤਾਂ ਜੇ (exit_latency == min_exit_latency) {
				nr_candidates++;
				ਜੇ (!reciprocal_scale(sched_rng(), nr_candidates))
					shallowest_idle_cpu = i;
			}
		} ਨਹੀਂਤਾਂ ਜੇ (shallowest_idle_cpu == -1) {
			ਲੱਦੋ = cpu_ਲੱਦੋ(cpu_rq(i));
			ਜੇ (ਲੱਦੋ < min_ਲੱਦੋ) {
				min_ਲੱਦੋ = ਲੱਦੋ;
				ਘੱਟ_ਤੋਂ_ਘੱਟ_loaded_cpu = i;
			}
		}
	}

	ਵਾਪਸ shallowest_idle_cpu != -1 ? shallowest_idle_cpu : ਘੱਟ_ਤੋਂ_ਘੱਟ_loaded_cpu;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ sched_balance_ਲੱਭੋ_dst_cpu(ਬਣਤਰ sched_domain *sd, ਬਣਤਰ task_struct *p,
				  ਪੂਰਨ cpu, ਪੂਰਨ prev_cpu, ਪੂਰਨ sd_flag)
{
	ਪੂਰਨ new_cpu = cpu;

	ਜੇ (!cpumask_intersects(sched_domain_ਵਿਸਤਾਰ(sd), p->cpus_ptr))
		ਵਾਪਸ prev_cpu;

	/*
	 * We need task's util for cpu_util_without, sync it up to
	 * prev_cpu's last_update_time.
	 */
	ਜੇ (!(sd_flag & SD_BALANCE_FORK))
		sync_entity_ਲੱਦੋ_avg(&p->se);

	ਜਦੋਂ (sd) {
		ਬਣਤਰ sched_ਸਮੂਹ *ਸਮੂਹ;
		ਬਣਤਰ sched_domain *tmp;
		ਪੂਰਨ ਭਾਰ;

		ਜੇ (!(sd->flags & sd_flag)) {
			sd = sd->ਬੱਚਾ;
			ਜਾਰੀ;
		}

		ਸਮੂਹ = sched_balance_ਲੱਭੋ_dst_ਸਮੂਹ(sd, p, cpu);
		ਜੇ (!ਸਮੂਹ) {
			sd = sd->ਬੱਚਾ;
			ਜਾਰੀ;
		}

		new_cpu = sched_balance_ਲੱਭੋ_dst_ਸਮੂਹ_cpu(ਸਮੂਹ, p, cpu);
		ਜੇ (new_cpu == cpu) {
			/* Now try balancing at a lower domain level of 'cpu': */
			sd = sd->ਬੱਚਾ;
			ਜਾਰੀ;
		}

		/* Now try balancing at a lower domain level of 'new_cpu': */
		cpu = new_cpu;
		ਭਾਰ = sd->ਵਿਸਤਾਰ_ਭਾਰ;
		sd = ਨਿਰਾ;
		for_each_domain(cpu, tmp) {
			ਜੇ (ਭਾਰ <= tmp->ਵਿਸਤਾਰ_ਭਾਰ)
				ਰੋਕੋ;
			ਜੇ (tmp->flags & sd_flag)
				sd = tmp;
		}
	}

	ਵਾਪਸ new_cpu;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ __select_idle_cpu(ਪੂਰਨ cpu, ਬਣਤਰ task_struct *p)
{
	ਜੇ (ਚੁਣੋ_idle_cpu(cpu, p) && sched_cpu_cookie_ਮੇਲ(cpu_rq(cpu), p))
		ਵਾਪਸ cpu;

	ਵਾਪਸ -1;
}

DEFINE_STATIC_ਕੁੰਜੀ_FALSE(sched_smt_present);
EXPORT_ਚਿੰਨ੍ਹ_GPL(sched_smt_present);

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ ਸੈੱਟ_idle_cores(ਪੂਰਨ cpu, ਪੂਰਨ val)
{
	ਬਣਤਰ sched_domain_shared *sds;

	sds = rcu_dereference_all(ਪ੍ਰਤੀ_cpu(sd_balance_shared, cpu));
	ਜੇ (sds)
		WRITE_ONCE(sds->has_idle_cores, val);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool test_idle_cores(ਪੂਰਨ cpu)
{
	ਬਣਤਰ sched_domain_shared *sds;

	sds = rcu_dereference_all(ਪ੍ਰਤੀ_cpu(sd_balance_shared, cpu));
	ਜੇ (sds)
		ਵਾਪਸ READ_ONCE(sds->has_idle_cores);

	ਵਾਪਸ false;
}

/*
 * Redirect a CPU to a higher-priority available sibling in its SMT domain,
 * subject to task affinity.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ select_idle_smt_cpu(ਬਣਤਰ task_struct *p, ਪੂਰਨ cpu)
{
	ਬਣਤਰ sched_domain *sd;
	ਪੂਰਨ ਉੱਤਮ = cpu;
	ਪੂਰਨ sibling;

	ਜੇ (!sched_smt_ਸਰਗਰਮ())
		ਵਾਪਸ cpu;

	sd = rcu_dereference_all(cpu_rq(cpu)->sd);
	ਜੇ (!sd || !(sd->flags & SD_SHARE_CPUCAPACITY) ||
	    !(sd->flags & SD_ASYM_PACKING))
		ਵਾਪਸ cpu;

	for_each_cpu_and(sibling, sched_domain_ਵਿਸਤਾਰ(sd), p->cpus_ptr) {
		ਜੇ (sibling == ਉੱਤਮ || !ਚੁਣੋ_idle_cpu(sibling, p))
			ਜਾਰੀ;

		ਜੇ (sched_asym_prefer(sibling, ਉੱਤਮ))
			ਉੱਤਮ = sibling;
	}

	ਵਾਪਸ ਉੱਤਮ;
}

/*
 * Scans the local SMT mask to see if the entire core is idle, and records this
 * information in sd_balance_shared->has_idle_cores.
 *
 * Since SMT siblings share all cache levels, inspecting this limited remote
 * state should be fairly cheap.
 */
ਖਾਲੀ __update_idle_core(ਬਣਤਰ rq *rq)
{
	ਪੂਰਨ core = cpu_ਦਾ(rq);
	ਪੂਰਨ cpu;

	rcu_read_ਤਾਲਾ();
	ਜੇ (test_idle_cores(core))
		ਜਾਓ unlock;

	for_each_cpu(cpu, cpu_smt_ਨਕਾਬ(core)) {
		ਜੇ (cpu == core)
			ਜਾਰੀ;

		ਜੇ (!ਉਪਲਬਧ_idle_cpu(cpu))
			ਜਾਓ unlock;
	}

	ਸੈੱਟ_idle_cores(core, 1);
unlock:
	rcu_read_unlock();
}

/*
 * Scan the entire LLC domain for idle cores; this dynamically switches off if
 * there are no idle cores left in the system; tracked through
 * sd_balance_shared->has_idle_cores and enabled through update_idle_core()
 * above.
 */
ਸਥਿਰ ਪੂਰਨ select_idle_core(ਬਣਤਰ task_struct *p, ਪੂਰਨ core, ਬਣਤਰ cpumask *cpus, ਪੂਰਨ *idle_cpu)
{
	bool idle = true;
	ਪੂਰਨ cpu;

	for_each_cpu(cpu, cpu_smt_ਨਕਾਬ(core)) {
		ਜੇ (!ਉਪਲਬਧ_idle_cpu(cpu)) {
			idle = false;
			ਜੇ (*idle_cpu == -1) {
				ਜੇ (ਚੁਣੋ_sched_idle_rq(cpu_rq(cpu), p) &&
				    cpumask_test_cpu(cpu, cpus)) {
					*idle_cpu = cpu;
					ਰੋਕੋ;
				}
				ਜਾਰੀ;
			}
			ਰੋਕੋ;
		}
		ਜੇ (*idle_cpu == -1 && cpumask_test_cpu(cpu, cpus))
			*idle_cpu = cpu;
	}

	ਜੇ (idle)
		ਵਾਪਸ core;

	cpumask_andnot(cpus, cpus, cpu_smt_ਨਕਾਬ(core));
	ਵਾਪਸ -1;
}

/*
 * Scan the local SMT mask for idle CPUs.
 */
ਸਥਿਰ ਪੂਰਨ select_idle_smt(ਬਣਤਰ task_struct *p, ਬਣਤਰ sched_domain *sd, ਪੂਰਨ ਨਿਸ਼ਾਨਾ)
{
	ਪੂਰਨ cpu;

	for_each_cpu_and(cpu, cpu_smt_ਨਕਾਬ(ਨਿਸ਼ਾਨਾ), p->cpus_ptr) {
		ਜੇ (cpu == ਨਿਸ਼ਾਨਾ)
			ਜਾਰੀ;
		/*
		 * Check if the CPU is in the LLC scheduling domain of @target.
		 * Due to isolcpus, there is no guarantee that all the siblings are in the domain.
		 */
		ਜੇ (!cpumask_test_cpu(cpu, sched_domain_ਵਿਸਤਾਰ(sd)))
			ਜਾਰੀ;
		ਜੇ (ਚੁਣੋ_idle_cpu(cpu, p))
			ਵਾਪਸ cpu;
	}

	ਵਾਪਸ -1;
}

/*
 * Scan the LLC domain for idle CPUs; this is dynamically regulated by
 * comparing the average scan cost (tracked in sd->avg_scan_cost) against the
 * average idle time for this rq (as found in rq->avg_idle).
 */
ਸਥਿਰ ਪੂਰਨ select_idle_cpu(ਬਣਤਰ task_struct *p, ਬਣਤਰ sched_domain *sd, bool has_idle_core, ਪੂਰਨ ਨਿਸ਼ਾਨਾ)
{
	ਬਣਤਰ cpumask *cpus = this_cpu_cpumask_var_ptr(select_rq_ਨਕਾਬ);
	ਪੂਰਨ i, cpu, idle_cpu = -1, nr = ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ;

	ਜੇ (sched_feat(SIS_UTIL) && sd->shared) {
		/*
		 * Increment because !--nr is the condition to stop scan.
		 *
		 * Since "sd" is "sd_llc" for target CPU dereferenced in the
		 * caller, it is safe to directly dereference "sd->shared".
		 * Topology bits always ensure it assigned for "sd_llc" abd it
		 * cannot disappear as long as we have a RCU protected
		 * reference to one the associated "sd" here.
		 */
		nr = READ_ONCE(sd->shared->nr_idle_ਜਾਂਚ) + 1;
		/* overloaded LLC is unlikely to have idle cpu/core */
		ਜੇ (nr == 1)
			ਵਾਪਸ -1;
	}

	ਜੇ (!cpumask_and(cpus, sched_domain_ਵਿਸਤਾਰ(sd), p->cpus_ptr))
		ਵਾਪਸ -1;

	ਜੇ (static_branch_unlikely(&sched_cluster_ਸਰਗਰਮ)) {
		ਬਣਤਰ sched_ਸਮੂਹ *sg = sd->groups;

		ਜੇ (sg->flags & SD_CLUSTER) {
			for_each_cpu_wrap(cpu, sched_ਸਮੂਹ_ਵਿਸਤਾਰ(sg), ਨਿਸ਼ਾਨਾ + 1) {
				ਜੇ (!cpumask_test_cpu(cpu, cpus))
					ਜਾਰੀ;

				ਜੇ (has_idle_core) {
					i = select_idle_core(p, cpu, cpus, &idle_cpu);
					ਜੇ ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)i < nr_cpumask_ਬਿੱਟ)
						ਵਾਪਸ i;
				} ਨਹੀਂਤਾਂ {
					ਜੇ (--nr <= 0)
						ਵਾਪਸ -1;
					idle_cpu = __select_idle_cpu(cpu, p);
					ਜੇ ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)idle_cpu < nr_cpumask_ਬਿੱਟ)
						ਵਾਪਸ idle_cpu;
				}
			}
			cpumask_andnot(cpus, cpus, sched_ਸਮੂਹ_ਵਿਸਤਾਰ(sg));
		}
	}

	for_each_cpu_wrap(cpu, cpus, ਨਿਸ਼ਾਨਾ + 1) {
		ਜੇ (has_idle_core) {
			i = select_idle_core(p, cpu, cpus, &idle_cpu);
			ਜੇ ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)i < nr_cpumask_ਬਿੱਟ)
				ਵਾਪਸ i;

		} ਨਹੀਂਤਾਂ {
			ਜੇ (--nr <= 0)
				ਵਾਪਸ -1;
			idle_cpu = __select_idle_cpu(cpu, p);
			ਜੇ ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)idle_cpu < nr_cpumask_ਬਿੱਟ)
				ਰੋਕੋ;
		}
	}

	ਜੇ (has_idle_core)
		ਸੈੱਟ_idle_cores(ਨਿਸ਼ਾਨਾ, false);

	ਵਾਪਸ idle_cpu;
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
ਗਿਣਤੀ asym_ਸਮਾਉਂਦਾ_state {
	ASYM_IDLE_UCLAMP_MISFIT = -4,
	ASYM_IDLE_COMPLETE_MISFIT,
	ASYM_IDLE_THREAD_ਸਮਾਉਂਦਾ,
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
ਸਥਿਰ ਪੂਰਨ
select_idle_ਸਮਰੱਥਾ(ਬਣਤਰ task_struct *p, ਬਣਤਰ sched_domain *sd, ਪੂਰਨ ਨਿਸ਼ਾਨਾ)
{
	/*
	 * On !SMT systems, has_idle_core is always false and preferred_core
	 * is always true (CPU == core), so the SMT preference logic below
	 * collapses to the plain capacity scan.
	 */
	bool has_idle_core = sched_smt_ਸਰਗਰਮ() && test_idle_cores(ਨਿਸ਼ਾਨਾ);
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ task_util, util_min, util_max, ਉੱਤਮ_cap = 0;
	ਪੂਰਨ ਸਮਾਉਂਦਾ, ਉੱਤਮ_ਸਮਾਉਂਦਾ = ASYM_IDLE_THREAD_MISFIT;
	ਪੂਰਨ cpu, ਉੱਤਮ_cpu = -1;
	ਬਣਤਰ cpumask *cpus;
	ਪੂਰਨ nr = ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ;

	cpus = this_cpu_cpumask_var_ptr(select_rq_ਨਕਾਬ);
	cpumask_and(cpus, sched_domain_ਵਿਸਤਾਰ(sd), p->cpus_ptr);

	task_util = task_util_est(p);
	util_min = uclamp_eff_ਮੁੱਲ(p, UCLAMP_MIN);
	util_max = uclamp_eff_ਮੁੱਲ(p, UCLAMP_MAX);

	ਜੇ (sched_feat(SIS_UTIL) && sd->shared) {
		/*
		 * Same nr_idle_scan hint as select_idle_cpu(), nr only limits
		 * the scan when not preferring an idle core.
		 */
		nr = READ_ONCE(sd->shared->nr_idle_ਜਾਂਚ) + 1;
		/* overloaded domain is unlikely to have idle cpu/core */
		ਜੇ (nr == 1)
			ਵਾਪਸ -1;
	}

	for_each_cpu_wrap(cpu, cpus, ਨਿਸ਼ਾਨਾ) {
		bool preferred_core = !has_idle_core || is_core_idle(cpu);
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ cpu_cap = ਸਮਰੱਥਾ_ਦਾ(cpu);

		/*
		 * Stop when the nr_idle_scan is exhausted (mirrors
		 * select_idle_cpu() logic).
		 */
		ਜੇ (!has_idle_core && --nr <= 0)
			ਵਾਪਸ ਉੱਤਮ_cpu;

		ਜੇ (!ਚੁਣੋ_idle_cpu(cpu, p))
			ਜਾਰੀ;

		ਸਮਾਉਂਦਾ = util_ਸਮਾਉਂਦਾ_cpu(task_util, util_min, util_max, cpu);

		/*
		 * Perfect fit: capacity satisfies util + uclamp and the CPU
		 * sits on a fully-idle SMT core, this is a !SMT system, or
		 * there is no idle core to find.
		 * Short-circuit the rank-based selection and return
		 * immediately.
		 */
		ਜੇ (ਸਮਾਉਂਦਾ > 0 && preferred_core)
			ਵਾਪਸ cpu;
		/*
		 * Only the min performance hint (i.e. uclamp_min) doesn't fit.
		 * Look for the CPU with best capacity.
		 */
		ਨਹੀਂਤਾਂ ਜੇ (ਸਮਾਉਂਦਾ < 0)
			cpu_cap = get_actual_cpu_ਸਮਰੱਥਾ(cpu);
		/*
		 * fits > 0 implies we are not on a preferred core, but the util
		 * fits CPU capacity. Set fits to ASYM_IDLE_THREAD_FITS
		 * so the effective range becomes
		 * [ASYM_IDLE_THREAD_FITS, ASYM_IDLE_THREAD_MISFIT], where:
		 *    ASYM_IDLE_THREAD_MISFIT - does not fit
		 *    ASYM_IDLE_THREAD_UCLAMP_MISFIT - fits with the exception of UCLAMP_MIN
		 *    ASYM_IDLE_THREAD_FITS - fits with the exception of preferred_core
		 */
		ਨਹੀਂਤਾਂ ਜੇ (ਸਮਾਉਂਦਾ > 0)
			ਸਮਾਉਂਦਾ = ASYM_IDLE_THREAD_ਸਮਾਉਂਦਾ;

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
		ਜੇ (preferred_core)
			ਸਮਾਉਂਦਾ += ASYM_IDLE_CORE_BIAS;

		/*
		 * First, select CPU which fits better (lower is more preferred).
		 * Then, select the one with best capacity at same level.
		 */
		ਜੇ ((ਸਮਾਉਂਦਾ < ਉੱਤਮ_ਸਮਾਉਂਦਾ) ||
		    ((ਸਮਾਉਂਦਾ == ਉੱਤਮ_ਸਮਾਉਂਦਾ) && (cpu_cap > ਉੱਤਮ_cap))) {
			ਉੱਤਮ_cap = cpu_cap;
			ਉੱਤਮ_cpu = cpu;
			ਉੱਤਮ_ਸਮਾਉਂਦਾ = ਸਮਾਉਂਦਾ;
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
	ਜੇ (has_idle_core && ਉੱਤਮ_ਸਮਾਉਂਦਾ > ASYM_IDLE_COMPLETE_MISFIT)
		ਸੈੱਟ_idle_cores(ਨਿਸ਼ਾਨਾ, false);

	ਵਾਪਸ ਉੱਤਮ_cpu;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool asym_ਸਮਾਉਂਦਾ_cpu(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ util,
				 ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ util_min,
				 ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ util_max,
				 ਪੂਰਨ cpu)
{
	ਜੇ (sched_asym_cpucap_ਸਰਗਰਮ()) {
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
		ਵਾਪਸ (!sched_smt_ਸਰਗਰਮ() || is_core_idle(cpu)) &&
		       (util_ਸਮਾਉਂਦਾ_cpu(util, util_min, util_max, cpu) > 0);
	}

	ਵਾਪਸ true;
}

/*
 * Try and locate an idle core/thread in the LLC cache domain.
 */
ਸਥਿਰ ਪੂਰਨ select_idle_sibling(ਬਣਤਰ task_struct *p, ਪੂਰਨ prev, ਪੂਰਨ ਨਿਸ਼ਾਨਾ)
{
	bool has_idle_core = false;
	ਬਣਤਰ sched_domain *sd;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ task_util, util_min, util_max;
	ਪੂਰਨ i, recent_ਵਰਤਿਆ_cpu, prev_aff = -1;

	/*
	 * On asymmetric system, update task utilization because we will check
	 * that the task fits with CPU's capacity.
	 */
	ਜੇ (sched_asym_cpucap_ਸਰਗਰਮ()) {
		sync_entity_ਲੱਦੋ_avg(&p->se);
		task_util = task_util_est(p);
		util_min = uclamp_eff_ਮੁੱਲ(p, UCLAMP_MIN);
		util_max = uclamp_eff_ਮੁੱਲ(p, UCLAMP_MAX);
	}

	/*
	 * per-cpu select_rq_mask usage
	 */
	lockdep_assert_irqs_disabled();

	ਜੇ (ਚੁਣੋ_idle_cpu(ਨਿਸ਼ਾਨਾ, p) &&
	    asym_ਸਮਾਉਂਦਾ_cpu(task_util, util_min, util_max, ਨਿਸ਼ਾਨਾ))
		ਜਾਓ select_smt_ਤਰਜੀਹ;

	/*
	 * If the previous CPU is cache affine and idle, don't be stupid:
	 */
	ਜੇ (prev != ਨਿਸ਼ਾਨਾ && cpus_share_cache(prev, ਨਿਸ਼ਾਨਾ) &&
	    ਚੁਣੋ_idle_cpu(prev, p) &&
	    asym_ਸਮਾਉਂਦਾ_cpu(task_util, util_min, util_max, prev)) {

		ਜੇ (!static_branch_unlikely(&sched_cluster_ਸਰਗਰਮ) ||
		    cpus_share_resources(prev, ਨਿਸ਼ਾਨਾ)) {
			ਨਿਸ਼ਾਨਾ = prev;
			ਜਾਓ select_smt_ਤਰਜੀਹ;
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
	ਜੇ (is_ਪ੍ਰਤੀ_cpu_kthread(current) &&
	    ਵਿੱਚ_task() &&
	    prev == smp_processor_id() &&
	    this_rq()->nr_running <= 1 &&
	    asym_ਸਮਾਉਂਦਾ_cpu(task_util, util_min, util_max, prev)) {
		ਨਿਸ਼ਾਨਾ = prev;
		ਜਾਓ select_smt_ਤਰਜੀਹ;
	}

	/* Check a recently used CPU as a potential idle candidate: */
	recent_ਵਰਤਿਆ_cpu = p->recent_ਵਰਤਿਆ_cpu;
	p->recent_ਵਰਤਿਆ_cpu = prev;
	ਜੇ (recent_ਵਰਤਿਆ_cpu != prev &&
	    recent_ਵਰਤਿਆ_cpu != ਨਿਸ਼ਾਨਾ &&
	    cpus_share_cache(recent_ਵਰਤਿਆ_cpu, ਨਿਸ਼ਾਨਾ) &&
	    ਚੁਣੋ_idle_cpu(recent_ਵਰਤਿਆ_cpu, p) &&
	    cpumask_test_cpu(recent_ਵਰਤਿਆ_cpu, p->cpus_ptr) &&
	    asym_ਸਮਾਉਂਦਾ_cpu(task_util, util_min, util_max, recent_ਵਰਤਿਆ_cpu)) {

		ਜੇ (!static_branch_unlikely(&sched_cluster_ਸਰਗਰਮ) ||
		    cpus_share_resources(recent_ਵਰਤਿਆ_cpu, ਨਿਸ਼ਾਨਾ)) {
			ਨਿਸ਼ਾਨਾ = recent_ਵਰਤਿਆ_cpu;
			ਜਾਓ select_smt_ਤਰਜੀਹ;
		}

	} ਨਹੀਂਤਾਂ {
		recent_ਵਰਤਿਆ_cpu = -1;
	}

	/*
	 * For asymmetric CPU capacity systems, our domain of interest is
	 * sd_asym_cpucapacity rather than sd_llc.
	 */
	ਜੇ (sched_asym_cpucap_ਸਰਗਰਮ()) {
		sd = rcu_dereference_all(ਪ੍ਰਤੀ_cpu(sd_asym_cpucapacity, ਨਿਸ਼ਾਨਾ));
		/*
		 * On an asymmetric CPU capacity system where an exclusive
		 * cpuset defines a symmetric island (i.e. one unique
		 * capacity_orig value through the cpuset), the key will be set
		 * but the CPUs within that cpuset will not have a domain with
		 * SD_ASYM_CPUCAPACITY. These should follow the usual symmetric
		 * capacity path.
		 */
		ਜੇ (sd) {
			i = select_idle_ਸਮਰੱਥਾ(p, sd, ਨਿਸ਼ਾਨਾ);
			ਜੇ ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)i < nr_cpumask_ਬਿੱਟ) {
				ਨਿਸ਼ਾਨਾ = i;
				ਜਾਓ select_smt_ਤਰਜੀਹ;
			}
			ਵਾਪਸ ਨਿਸ਼ਾਨਾ;
		}
	}

	sd = rcu_dereference_all(ਪ੍ਰਤੀ_cpu(sd_llc, ਨਿਸ਼ਾਨਾ));
	ਜੇ (!sd)
		ਵਾਪਸ ਨਿਸ਼ਾਨਾ;

	ਜੇ (sched_smt_ਸਰਗਰਮ()) {
		has_idle_core = test_idle_cores(ਨਿਸ਼ਾਨਾ);

		ਜੇ (!has_idle_core && cpus_share_cache(prev, ਨਿਸ਼ਾਨਾ)) {
			i = select_idle_smt(p, sd, prev);
			ਜੇ ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)i < nr_cpumask_ਬਿੱਟ) {
				ਨਿਸ਼ਾਨਾ = i;
				ਜਾਓ select_smt_ਤਰਜੀਹ;
			}
		}
	}

	i = select_idle_cpu(p, sd, has_idle_core, ਨਿਸ਼ਾਨਾ);
	ਜੇ ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)i < nr_cpumask_ਬਿੱਟ) {
		ਨਿਸ਼ਾਨਾ = i;
		ਜਾਓ select_smt_ਤਰਜੀਹ;
	}

	/*
	 * For cluster machines which have lower sharing cache like L2 or
	 * LLC Tag, we tend to find an idle CPU in the target's cluster
	 * first. But prev_cpu or recent_used_cpu may also be a good candidate,
	 * use them if possible when no idle CPU found in select_idle_cpu().
	 */
	ਜੇ ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)prev_aff < nr_cpumask_ਬਿੱਟ) {
		ਨਿਸ਼ਾਨਾ = prev_aff;
		ਜਾਓ select_smt_ਤਰਜੀਹ;
	}
	ਜੇ ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)recent_ਵਰਤਿਆ_cpu < nr_cpumask_ਬਿੱਟ) {
		ਨਿਸ਼ਾਨਾ = recent_ਵਰਤਿਆ_cpu;
		ਜਾਓ select_smt_ਤਰਜੀਹ;
	}

	ਵਾਪਸ ਨਿਸ਼ਾਨਾ;

select_smt_ਤਰਜੀਹ:
	ਵਾਪਸ select_idle_smt_cpu(p, ਨਿਸ਼ਾਨਾ);
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
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ
cpu_util(ਪੂਰਨ cpu, ਬਣਤਰ task_struct *p, ਪੂਰਨ dst_cpu, ਪੂਰਨ boost)
{
	bool ਜੋੜੋ_task = p && task_cpu(p) != cpu && dst_cpu == cpu;
	bool sub_task = p && task_cpu(p) == cpu && dst_cpu != cpu;
	ਬਣਤਰ cfs_rq *cfs_rq = &cpu_rq(cpu)->cfs;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ util = READ_ONCE(cfs_rq->avg.util_avg);
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ runnable;

	/*
	 * If @dst_cpu is -1 or @p migrates from @cpu to @dst_cpu remove its
	 * contribution. If @p migrates from another CPU to @cpu add its
	 * contribution. In all the other cases @cpu is not impacted by the
	 * migration so its util_avg is already correct.
	 */
	ਜੇ (ਜੋੜੋ_task)
		util += task_util(p);
	ਨਹੀਂਤਾਂ ਜੇ (sub_task)
		lsub_ਸਕਾਰਾਤਮਕ(&util, task_util(p));

	ਜੇ (boost) {
		runnable = READ_ONCE(cfs_rq->avg.runnable_avg);
		ਜੇ (ਜੋੜੋ_task)
			runnable += READ_ONCE(p->se.avg.runnable_avg);
		ਨਹੀਂਤਾਂ ਜੇ (sub_task)
			lsub_ਸਕਾਰਾਤਮਕ(&runnable,
				      READ_ONCE(p->se.avg.runnable_avg));
		util = max(util, runnable);
	}

	ਜੇ (sched_feat(UTIL_EST)) {
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ util_est;

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
		ਜੇ (dst_cpu == cpu)
			util_est += _task_util_est(p);
		ਨਹੀਂਤਾਂ ਜੇ (p && unlikely(task_on_rq_queued(p) || current == p))
			lsub_ਸਕਾਰਾਤਮਕ(&util_est, _task_util_est(p));

		util = max(util, util_est);
	}

	ਵਾਪਸ min(util, arch_scale_cpu_ਸਮਰੱਥਾ(cpu));
}

ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ cpu_util_cfs(ਪੂਰਨ cpu)
{
	ਵਾਪਸ cpu_util(cpu, ਨਿਰਾ, -1, 0);
}

ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ cpu_util_cfs_boost(ਪੂਰਨ cpu)
{
	ਵਾਪਸ cpu_util(cpu, ਨਿਰਾ, -1, 1);
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
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ cpu_util_without(ਪੂਰਨ cpu, ਬਣਤਰ task_struct *p)
{
	/* Task has no contribution or is new */
	ਜੇ (cpu != task_cpu(p) || !READ_ONCE(p->se.avg.ਆਖਰੀ_update_ਸਮਾਂ))
		p = ਨਿਰਾ;

	ਵਾਪਸ cpu_util(cpu, p, -1, 0);
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
ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ effective_cpu_util(ਪੂਰਨ cpu, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ util_cfs,
				 ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ *min,
				 ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ *max)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ util, irq, scale;
	ਬਣਤਰ rq *rq = cpu_rq(cpu);

	scale = arch_scale_cpu_ਸਮਰੱਥਾ(cpu);

	/*
	 * Early check to see if IRQ/steal time saturates the CPU, can be
	 * because of inaccuracies in how we track these -- see
	 * update_irq_load_avg().
	 */
	irq = cpu_util_irq(rq);
	ਜੇ (unlikely(irq >= scale)) {
		ਜੇ (min)
			*min = scale;
		ਜੇ (max)
			*max = scale;
		ਵਾਪਸ scale;
	}

	ਜੇ (min) {
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
		ਜੇ (!uclamp_is_ਵਰਤਿਆ() && rt_rq_is_runnable(&rq->rt))
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
	ਜੇ (max)
		*max = min(scale, uclamp_rq_get(rq, UCLAMP_MAX));

	ਜੇ (util >= scale)
		ਵਾਪਸ scale;

	/*
	 * There is still idle time; further improve the number by using the
	 * IRQ metric. Because IRQ/steal time is hidden from the task clock we
	 * need to scale the task numbers:
	 *
	 *              max - irq
	 *   U' = irq + --------- * U
	 *                 max
	 */
	util = scale_irq_ਸਮਰੱਥਾ(util, irq, scale);
	util += irq;

	ਵਾਪਸ min(scale, util);
}

ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ sched_cpu_util(ਪੂਰਨ cpu)
{
	ਵਾਪਸ effective_cpu_util(cpu, cpu_util_cfs(cpu), ਨਿਰਾ, ਨਿਰਾ);
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
ਬਣਤਰ energy_env {
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ task_busy_ਸਮਾਂ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ pd_busy_ਸਮਾਂ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ cpu_cap;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ pd_cap;
};

/*
 * Compute the task busy time for compute_energy(). This time cannot be
 * injected directly into effective_cpu_util() because of the IRQ scaling.
 * The latter only makes sense with the most recent CPUs where the task has
 * run.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ eenv_task_busy_ਸਮਾਂ(ਬਣਤਰ energy_env *eenv,
				       ਬਣਤਰ task_struct *p, ਪੂਰਨ prev_cpu)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ busy_ਸਮਾਂ, max_cap = arch_scale_cpu_ਸਮਰੱਥਾ(prev_cpu);
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ irq = cpu_util_irq(cpu_rq(prev_cpu));

	ਜੇ (unlikely(irq >= max_cap))
		busy_ਸਮਾਂ = max_cap;
	ਨਹੀਂਤਾਂ
		busy_ਸਮਾਂ = scale_irq_ਸਮਰੱਥਾ(task_util_est(p), irq, max_cap);

	eenv->task_busy_ਸਮਾਂ = busy_ਸਮਾਂ;
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
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ eenv_pd_busy_ਸਮਾਂ(ਬਣਤਰ energy_env *eenv,
				     ਬਣਤਰ cpumask *pd_cpus,
				     ਬਣਤਰ task_struct *p)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ busy_ਸਮਾਂ = 0;
	ਪੂਰਨ cpu;

	for_each_cpu(cpu, pd_cpus) {
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ util = cpu_util(cpu, p, -1, 0);

		busy_ਸਮਾਂ += effective_cpu_util(cpu, util, ਨਿਰਾ, ਨਿਰਾ);
	}

	eenv->pd_busy_ਸਮਾਂ = min(eenv->pd_cap, busy_ਸਮਾਂ);
}

/*
 * Compute the maximum utilization for compute_energy() when the task @p
 * is placed on the cpu @dst_cpu.
 *
 * Returns the maximum utilization among @eenv->cpus. This utilization can't
 * exceed @eenv->cpu_cap.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ
eenv_pd_max_util(ਬਣਤਰ energy_env *eenv, ਬਣਤਰ cpumask *pd_cpus,
		 ਬਣਤਰ task_struct *p, ਪੂਰਨ dst_cpu)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ max_util = 0;
	ਪੂਰਨ cpu;

	for_each_cpu(cpu, pd_cpus) {
		ਬਣਤਰ task_struct *tsk = (cpu == dst_cpu) ? p : ਨਿਰਾ;
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ util = cpu_util(cpu, p, dst_cpu, 1);
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ eff_util, min, max;

		/*
		 * Performance domain frequency: utilization clamping
		 * must be considered since it affects the selection
		 * of the performance domain frequency.
		 * NOTE: in case RT tasks are running, by default the min
		 * utilization can be max OPP.
		 */
		eff_util = effective_cpu_util(cpu, util, &min, &max);

		/* Task's uclamp can modify min and max value */
		ਜੇ (tsk && uclamp_is_ਵਰਤਿਆ()) {
			min = max(min, uclamp_eff_ਮੁੱਲ(p, UCLAMP_MIN));

			/*
			 * If there is no active max uclamp constraint,
			 * directly use task's one, otherwise keep max.
			 */
			ਜੇ (uclamp_rq_is_idle(cpu_rq(cpu)))
				max = uclamp_eff_ਮੁੱਲ(p, UCLAMP_MAX);
			ਨਹੀਂਤਾਂ
				max = max(max, uclamp_eff_ਮੁੱਲ(p, UCLAMP_MAX));
		}

		eff_util = sugov_effective_cpu_perf(cpu, eff_util, min, max);
		max_util = max(max_util, eff_util);
	}

	ਵਾਪਸ min(max_util, eenv->cpu_cap);
}

/*
 * compute_energy(): Use the Energy Model to estimate the energy that @pd would
 * consume for a given utilization landscape @eenv. When @dst_cpu < 0, the task
 * contribution is ignored.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ
compute_energy(ਬਣਤਰ energy_env *eenv, ਬਣਤਰ perf_domain *pd,
	       ਬਣਤਰ cpumask *pd_cpus, ਬਣਤਰ task_struct *p, ਪੂਰਨ dst_cpu)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ max_util = eenv_pd_max_util(eenv, pd_cpus, p, dst_cpu);
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ busy_ਸਮਾਂ = eenv->pd_busy_ਸਮਾਂ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ energy;

	ਜੇ (dst_cpu >= 0)
		busy_ਸਮਾਂ = min(eenv->pd_cap, busy_ਸਮਾਂ + eenv->task_busy_ਸਮਾਂ);

	energy = em_cpu_energy(pd->em_pd, max_util, busy_ਸਮਾਂ, eenv->cpu_cap);

	trace_sched_compute_energy_tp(p, dst_cpu, energy, max_util, busy_ਸਮਾਂ);

	ਵਾਪਸ energy;
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
ਸਥਿਰ ਪੂਰਨ ਲੱਭੋ_energy_efficient_cpu(ਬਣਤਰ task_struct *p, ਪੂਰਨ prev_cpu)
{
	ਬਣਤਰ cpumask *cpus = this_cpu_cpumask_var_ptr(select_rq_ਨਕਾਬ);
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ prev_delta = ULONG_MAX, ਉੱਤਮ_delta = ULONG_MAX;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ p_util_min = uclamp_is_ਵਰਤਿਆ() ? uclamp_eff_ਮੁੱਲ(p, UCLAMP_MIN) : 0;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ p_util_max = uclamp_is_ਵਰਤਿਆ() ? uclamp_eff_ਮੁੱਲ(p, UCLAMP_MAX) : 1024;
	ਬਣਤਰ ਜੜ੍ਹ_domain *rd = this_rq()->rd;
	ਪੂਰਨ cpu, ਉੱਤਮ_energy_cpu, ਨਿਸ਼ਾਨਾ = -1;
	ਪੂਰਨ prev_ਸਮਾਉਂਦਾ = -1, ਉੱਤਮ_ਸਮਾਉਂਦਾ = -1;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਉੱਤਮ_actual_cap = 0;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ prev_actual_cap = 0;
	ਬਣਤਰ sched_domain *sd;
	ਬਣਤਰ perf_domain *pd;
	ਬਣਤਰ energy_env eenv;

	pd = rcu_dereference_all(rd->pd);
	ਜੇ (!pd)
		ਵਾਪਸ ਨਿਸ਼ਾਨਾ;

	/*
	 * Energy-aware wake-up happens on the lowest sched_domain starting
	 * from sd_asym_cpucapacity spanning over this_cpu and prev_cpu.
	 */
	sd = rcu_dereference_all(*this_cpu_ptr(&sd_asym_cpucapacity));
	ਜਦੋਂ (sd && !cpumask_test_cpu(prev_cpu, sched_domain_ਵਿਸਤਾਰ(sd)))
		sd = sd->ਮਾਪਾ;
	ਜੇ (!sd)
		ਵਾਪਸ ਨਿਸ਼ਾਨਾ;

	ਨਿਸ਼ਾਨਾ = prev_cpu;

	sync_entity_ਲੱਦੋ_avg(&p->se);
	ਜੇ (!task_util_est(p) && p_util_min == 0)
		ਵਾਪਸ ਨਿਸ਼ਾਨਾ;

	eenv_task_busy_ਸਮਾਂ(&eenv, p, prev_cpu);

	ਲਈ (; pd; pd = pd->ਅਗਲਾ) {
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ util_min = p_util_min, util_max = p_util_max;
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ cpu_cap, cpu_actual_cap, util;
		ਲੰਮਾ prev_ਵਾਧੂ_cap = -1, max_ਵਾਧੂ_cap = -1;
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ rq_util_min, rq_util_max;
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ cur_delta, ਅਧਾਰ_energy;
		ਪੂਰਨ max_ਵਾਧੂ_cap_cpu = -1;
		ਪੂਰਨ ਸਮਾਉਂਦਾ, max_ਸਮਾਉਂਦਾ = -1;

		ਜੇ (!cpumask_and(cpus, perf_domain_ਵਿਸਤਾਰ(pd), cpu_online_ਨਕਾਬ))
			ਜਾਰੀ;

		/* Account external pressure for the energy estimation */
		cpu = cpumask_ਪਹਿਲਾ(cpus);
		cpu_actual_cap = get_actual_cpu_ਸਮਰੱਥਾ(cpu);

		eenv.cpu_cap = cpu_actual_cap;
		eenv.pd_cap = 0;

		for_each_cpu(cpu, cpus) {
			ਬਣਤਰ rq *rq = cpu_rq(cpu);

			eenv.pd_cap += cpu_actual_cap;

			ਜੇ (!cpumask_test_cpu(cpu, sched_domain_ਵਿਸਤਾਰ(sd)))
				ਜਾਰੀ;

			ਜੇ (!cpumask_test_cpu(cpu, p->cpus_ptr))
				ਜਾਰੀ;

			util = cpu_util(cpu, p, cpu, 0);
			cpu_cap = ਸਮਰੱਥਾ_ਦਾ(cpu);

			/*
			 * Skip CPUs that cannot satisfy the capacity request.
			 * IOW, placing the task there would make the CPU
			 * overutilized. Take uclamp into account to see how
			 * much capacity we can get out of the CPU; this is
			 * aligned with sched_cpu_util().
			 */
			ਜੇ (uclamp_is_ਵਰਤਿਆ() && !uclamp_rq_is_idle(rq)) {
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

			ਸਮਾਉਂਦਾ = util_ਸਮਾਉਂਦਾ_cpu(util, util_min, util_max, cpu);
			ਜੇ (!ਸਮਾਉਂਦਾ)
				ਜਾਰੀ;

			lsub_ਸਕਾਰਾਤਮਕ(&cpu_cap, util);

			ਜੇ (cpu == prev_cpu) {
				/* Always use prev_cpu as a candidate. */
				prev_ਵਾਧੂ_cap = cpu_cap;
				prev_ਸਮਾਉਂਦਾ = ਸਮਾਉਂਦਾ;
			} ਨਹੀਂਤਾਂ ਜੇ ((ਸਮਾਉਂਦਾ > max_ਸਮਾਉਂਦਾ) ||
				   ((ਸਮਾਉਂਦਾ == max_ਸਮਾਉਂਦਾ) && ((ਲੰਮਾ)cpu_cap > max_ਵਾਧੂ_cap))) {
				/*
				 * Find the CPU with the maximum spare capacity
				 * among the remaining CPUs in the performance
				 * domain.
				 */
				max_ਵਾਧੂ_cap = cpu_cap;
				max_ਵਾਧੂ_cap_cpu = cpu;
				max_ਸਮਾਉਂਦਾ = ਸਮਾਉਂਦਾ;
			}
		}

		ਜੇ (max_ਵਾਧੂ_cap_cpu < 0 && prev_ਵਾਧੂ_cap < 0)
			ਜਾਰੀ;

		eenv_pd_busy_ਸਮਾਂ(&eenv, cpus, p);
		/* Compute the 'base' energy of the pd, without @p */
		ਅਧਾਰ_energy = compute_energy(&eenv, pd, cpus, p, -1);

		/* Evaluate the energy impact of using prev_cpu. */
		ਜੇ (prev_ਵਾਧੂ_cap > -1) {
			prev_delta = compute_energy(&eenv, pd, cpus, p,
						    prev_cpu);
			/* CPU utilization has changed */
			ਜੇ (prev_delta < ਅਧਾਰ_energy)
				ਵਾਪਸ ਨਿਸ਼ਾਨਾ;
			prev_delta -= ਅਧਾਰ_energy;
			prev_actual_cap = cpu_actual_cap;
			ਉੱਤਮ_delta = min(ਉੱਤਮ_delta, prev_delta);
		}

		/* Evaluate the energy impact of using max_spare_cap_cpu. */
		ਜੇ (max_ਵਾਧੂ_cap_cpu >= 0 && max_ਵਾਧੂ_cap > prev_ਵਾਧੂ_cap) {
			/* Current best energy cpu fits better */
			ਜੇ (max_ਸਮਾਉਂਦਾ < ਉੱਤਮ_ਸਮਾਉਂਦਾ)
				ਜਾਰੀ;

			/*
			 * Both don't fit performance hint (i.e. uclamp_min)
			 * but best energy cpu has better capacity.
			 */
			ਜੇ ((max_ਸਮਾਉਂਦਾ < 0) &&
			    (cpu_actual_cap <= ਉੱਤਮ_actual_cap))
				ਜਾਰੀ;

			cur_delta = compute_energy(&eenv, pd, cpus, p,
						   max_ਵਾਧੂ_cap_cpu);
			/* CPU utilization has changed */
			ਜੇ (cur_delta < ਅਧਾਰ_energy)
				ਵਾਪਸ ਨਿਸ਼ਾਨਾ;
			cur_delta -= ਅਧਾਰ_energy;

			/*
			 * Both fit for the task but best energy cpu has lower
			 * energy impact.
			 */
			ਜੇ ((max_ਸਮਾਉਂਦਾ > 0) && (ਉੱਤਮ_ਸਮਾਉਂਦਾ > 0) &&
			    (cur_delta >= ਉੱਤਮ_delta))
				ਜਾਰੀ;

			ਉੱਤਮ_delta = cur_delta;
			ਉੱਤਮ_energy_cpu = max_ਵਾਧੂ_cap_cpu;
			ਉੱਤਮ_ਸਮਾਉਂਦਾ = max_ਸਮਾਉਂਦਾ;
			ਉੱਤਮ_actual_cap = cpu_actual_cap;
		}
	}

	ਜੇ ((ਉੱਤਮ_ਸਮਾਉਂਦਾ > prev_ਸਮਾਉਂਦਾ) ||
	    ((ਉੱਤਮ_ਸਮਾਉਂਦਾ > 0) && (ਉੱਤਮ_delta < prev_delta)) ||
	    ((ਉੱਤਮ_ਸਮਾਉਂਦਾ < 0) && (ਉੱਤਮ_actual_cap > prev_actual_cap)))
		ਨਿਸ਼ਾਨਾ = ਉੱਤਮ_energy_cpu;

	ਵਾਪਸ ਨਿਸ਼ਾਨਾ;
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
ਸਥਿਰ ਪੂਰਨ
select_task_rq_fair(ਬਣਤਰ task_struct *p, ਪੂਰਨ prev_cpu, ਪੂਰਨ wake_flags)
{
	ਪੂਰਨ sync = (wake_flags & WF_SYNC) && !(current->flags & PF_EXITING);
	ਬਣਤਰ sched_domain *tmp, *sd = ਨਿਰਾ;
	ਪੂਰਨ cpu = smp_processor_id();
	ਪੂਰਨ new_cpu = prev_cpu;
	ਪੂਰਨ want_affine = 0;
	/* SD_flags and WF_flags share the first nibble */
	ਪੂਰਨ sd_flag = wake_flags & 0xF;

	/*
	 * required for stable ->cpus_allowed
	 */
	lockdep_assert_held(&p->pi_ਤਾਲਾ);
	ਜੇ (wake_flags & WF_TTWU) {
		ਰਿਕਾਰਡ_wakee(p);

		ਜੇ ((wake_flags & WF_CURRENT_CPU) &&
		    cpumask_test_cpu(cpu, p->cpus_ptr))
			ਵਾਪਸ cpu;

		ਜੇ (!is_rd_overutilized(this_rq()->rd)) {
			new_cpu = ਲੱਭੋ_energy_efficient_cpu(p, prev_cpu);
			ਜੇ (new_cpu >= 0)
				ਵਾਪਸ new_cpu;
			new_cpu = prev_cpu;
		}

		want_affine = !wake_wide(p) && cpumask_test_cpu(cpu, p->cpus_ptr);
	}

	for_each_domain(cpu, tmp) {
		/*
		 * If both 'cpu' and 'prev_cpu' are part of this domain,
		 * cpu is a valid SD_WAKE_AFFINE target.
		 */
		ਜੇ (want_affine && (tmp->flags & SD_WAKE_AFFINE) &&
		    cpumask_test_cpu(prev_cpu, sched_domain_ਵਿਸਤਾਰ(tmp))) {
			ਜੇ (cpu != prev_cpu)
				new_cpu = wake_affine(tmp, p, cpu, prev_cpu, sync);

			sd = ਨਿਰਾ; /* Prefer wake_affine over balance flags */
			ਰੋਕੋ;
		}

		/*
		 * Usually only true for WF_EXEC and WF_FORK, as sched_domains
		 * usually do not have SD_BALANCE_WAKE set. That means wakeup
		 * will usually go to the fast path.
		 */
		ਜੇ (tmp->flags & sd_flag)
			sd = tmp;
		ਨਹੀਂਤਾਂ ਜੇ (!want_affine)
			ਰੋਕੋ;
	}

	/* Slow path */
	ਜੇ (unlikely(sd)) {
		new_cpu = sched_balance_ਲੱਭੋ_dst_cpu(sd, p, cpu, prev_cpu, sd_flag);
		ਵਾਪਸ select_idle_smt_cpu(p, new_cpu);
	}

	/* Fast path */
	ਜੇ (wake_flags & WF_TTWU)
		ਵਾਪਸ select_idle_sibling(p, prev_cpu, new_cpu);

	ਵਾਪਸ new_cpu;
}

/*
 * Called immediately before a task is migrated to a new CPU; task_cpu(p) and
 * cfs_rq_of(p) references at time of call are still valid and identify the
 * previous CPU. The caller guarantees p->pi_lock or task_rq(p)->lock is held.
 */
ਸਥਿਰ ਖਾਲੀ migrate_task_rq_fair(ਬਣਤਰ task_struct *p, ਪੂਰਨ new_cpu)
{
	ਬਣਤਰ sched_entity *se = &p->se;

	ਜੇ (!task_on_rq_migrating(p)) {
		remove_entity_ਲੱਦੋ_avg(se);

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
	se->avg.ਆਖਰੀ_update_ਸਮਾਂ = 0;

	update_ਜਾਂਚ_period(p, new_cpu);
}

ਸਥਿਰ ਖਾਲੀ task_dead_fair(ਬਣਤਰ task_struct *p)
{
	ਬਣਤਰ sched_entity *se = &p->se;
	remove_entity_ਲੱਦੋ_avg(se);
}

/*
 * Set the max capacity the task is allowed to run at for misfit detection.
 */
ਸਥਿਰ ਖਾਲੀ ਸੈੱਟ_task_max_allowed_ਸਮਰੱਥਾ(ਬਣਤਰ task_struct *p)
{
	ਬਣਤਰ asym_cap_ਅੰਕੜੇ *ਇੰਦਰਾਜ;

	ਜੇ (!sched_asym_cpucap_ਸਰਗਰਮ())
		ਵਾਪਸ;

	rcu_read_ਤਾਲਾ();
	list_for_each_ਇੰਦਰਾਜ_rcu(ਇੰਦਰਾਜ, &asym_cap_list, link) {
		cpumask_t *cpumask;

		cpumask = cpu_ਸਮਰੱਥਾ_ਵਿਸਤਾਰ(ਇੰਦਰਾਜ);
		ਜੇ (!cpumask_intersects(p->cpus_ptr, cpumask))
			ਜਾਰੀ;

		p->max_allowed_ਸਮਰੱਥਾ = ਇੰਦਰਾਜ->ਸਮਰੱਥਾ;
		ਰੋਕੋ;
	}
	rcu_read_unlock();
}

ਸਥਿਰ ਖਾਲੀ ਸੈੱਟ_cpus_allowed_fair(ਬਣਤਰ task_struct *p, ਬਣਤਰ affinity_context *ctx)
{
	ਸੈੱਟ_cpus_allowed_common(p, ctx);
	ਸੈੱਟ_task_max_allowed_ਸਮਰੱਥਾ(p);
}

ਗਿਣਤੀ preempt_wakeup_action {
	PREEMPT_WAKEUP_NONE,	/* No preemption. */
	PREEMPT_WAKEUP_SHORT,	/* Ignore slice protection. */
	PREEMPT_WAKEUP_PICK,	/* Let pick_eevdf() decide. */
	PREEMPT_WAKEUP_RESCHED,	/* Force reschedule. */
};

ਸਥਿਰ ਸਤਰਅੰਦਰ bool ਸੈੱਟ_preempt_buddy(ਬਣਤਰ cfs_rq *cfs_rq, ਬਣਤਰ sched_entity *pse)
{
	/*
	 * Keep existing buddy if the deadline is sooner than pse.
	 * The older buddy may be cache cold and completely unrelated
	 * to the current wakeup but that is unpredictable where as
	 * obeying the deadline is more in line with EEVDF objectives.
	 */
	ਜੇ (cfs_rq->ਅਗਲਾ && entity_before(cfs_rq->ਅਗਲਾ, pse))
		ਵਾਪਸ false;

	ਸੈੱਟ_ਅਗਲਾ_buddy(cfs_rq, pse);
	ਵਾਪਸ true;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool ਸੈੱਟ_short_buddy(ਬਣਤਰ cfs_rq *cfs_rq, ਬਣਤਰ sched_entity *pse)
{
	ਜੇ (cfs_rq->ਅਗਲਾ) {
		ਜੇ (cfs_rq->ਅਗਲਾ->ਟੁਕੜਾ < pse->ਟੁਕੜਾ)
			ਵਾਪਸ false;

		ਜੇ (cfs_rq->ਅਗਲਾ->ਟੁਕੜਾ == pse->ਟੁਕੜਾ &&
		    entity_before(cfs_rq->ਅਗਲਾ, pse))
			ਵਾਪਸ false;
	}

	ਸੈੱਟ_ਅਗਲਾ_buddy(cfs_rq, pse);
	ਵਾਪਸ true;
}

/*
 * WF_SYNC|WF_TTWU indicates the waker expects to sleep but it is not
 * strictly enforced because the hint is either misunderstood or
 * multiple tasks must be woken up.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਗਿਣਤੀ preempt_wakeup_action
preempt_sync(ਬਣਤਰ rq *rq, ਪੂਰਨ wake_flags,
	     ਬਣਤਰ sched_entity *pse, ਬਣਤਰ sched_entity *se)
{
	u64 ਹੱਦ, delta;

	/*
	 * WF_SYNC without WF_TTWU is not expected so warn if it happens even
	 * though it is likely harmless.
	 */
	WARN_ON_ONCE(!(wake_flags & WF_TTWU));

	ਹੱਦ = sysctl_sched_migration_ਲਾਗਤ;
	delta = rq_clock_task(rq) - se->exec_ਸ਼ੁਰੂ;
	ਜੇ ((s64)delta < 0)
		delta = 0;

	/*
	 * WF_RQ_SELECTED implies the tasks are stacking on a CPU when they
	 * could run on other CPUs. Reduce the threshold before preemption is
	 * allowed to an arbitrary lower value as it is more likely (but not
	 * guaranteed) the waker requires the wakee to finish.
	 */
	ਜੇ (wake_flags & WF_RQ_SELECTED)
		ਹੱਦ >>= 2;

	/*
	 * As WF_SYNC is not strictly obeyed, allow some runtime for batch
	 * wakeups to be issued.
	 */
	ਜੇ (entity_before(pse, se) && delta >= ਹੱਦ)
		ਵਾਪਸ PREEMPT_WAKEUP_RESCHED;

	ਵਾਪਸ PREEMPT_WAKEUP_NONE;
}

/*
 * Preempt the current task with a newly woken task if needed:
 */
ਸਥਿਰ ਖਾਲੀ wakeup_preempt_fair(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, ਪੂਰਨ wake_flags)
{
	ਗਿਣਤੀ preempt_wakeup_action preempt_action = PREEMPT_WAKEUP_PICK;
	ਬਣਤਰ task_struct *donor = rq->donor;
	ਬਣਤਰ sched_entity *nse, *se = &donor->se, *pse = &p->se;
	ਬਣਤਰ cfs_rq *cfs_rq = &rq->cfs;
	ਪੂਰਨ cse_is_idle, pse_is_idle;

	/*
	 * XXX Getting preempted by higher class, try and find idle CPU?
	 */
	ਜੇ (p->sched_class != &fair_sched_class ||
	    donor->sched_class != &fair_sched_class)
		ਵਾਪਸ;

	ਜੇ (unlikely(se == pse))
		ਵਾਪਸ;

	/*
	 * This is possible from callers such as attach_tasks(), in which we
	 * unconditionally wakeup_preempt() after an enqueue (which may have
	 * lead to a throttle).  This both saves work and prevents false
	 * next-buddy nomination below.
	 */
	ਜੇ (task_is_throttled(p))
		ਵਾਪਸ;

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
	ਜੇ (!sched_feat(PREEMPT_SHORT) && test_tsk_need_resched(rq->curr))
		ਵਾਪਸ;

	ਜੇ (!sched_feat(WAKEUP_PREEMPTION))
		ਵਾਪਸ;

	WARN_ON_ONCE(!pse);

	cse_is_idle = se_is_idle(se);
	pse_is_idle = se_is_idle(pse);

	nse = se;
	/*
	 * Preempt an idle entity in favor of a non-idle entity (and don't preempt
	 * in the inverse case).
	 */
	ਜੇ (cse_is_idle && !pse_is_idle)
		ਜਾਓ preempt;

	update_curr_fair(rq);

	ਜੇ (cse_is_idle != pse_is_idle)
		ਜਾਓ update;

	/*
	 * BATCH and IDLE tasks do not preempt others.
	 */
	ਜੇ (unlikely(!ਸਾਧਾਰਨ_policy(p->policy)))
		ਜਾਓ update;

	/*
	 * Do not preempt for tasks that are sched_delayed as it would violate
	 * EEVDF to forcibly queue an ineligible task.
	 */
	ਜੇ (pse->sched_delayed)
		ਜਾਓ update;

	/*
	 * If @p has a shorter slice than current and @p is eligible, override
	 * current's slice protection in order to allow preemption.
	 */
	ਜੇ (sched_feat(PREEMPT_SHORT) && (pse->ਟੁਕੜਾ < se->ਟੁਕੜਾ)) {
		preempt_action = PREEMPT_WAKEUP_SHORT;
		ਜਾਓ pick;
	}

	/*
	 * Ignore wakee preemption on WF_FORK as it is less likely that
	 * there is shared data as exec often follow fork.
	 */
	ਜੇ (wake_flags & WF_FORK)
		ਜਾਓ update;

	/* Prefer picking wakee soon if appropriate. */
	ਜੇ (sched_feat(ਅਗਲਾ_BUDDY) && ਸੈੱਟ_preempt_buddy(cfs_rq, pse)) {
		/*
		 * Decide whether to obey WF_SYNC hint for a new buddy. Old
		 * buddies are ignored as they may not be relevant to the
		 * waker and less likely to be cache hot.
		 */
		ਜੇ (wake_flags & WF_SYNC)
			preempt_action = preempt_sync(rq, wake_flags, pse, se);
	}

	ਚੋਣ (preempt_action) {
	ਹਾਲਤ PREEMPT_WAKEUP_NONE:
		ਵਾਪਸ;
	ਹਾਲਤ PREEMPT_WAKEUP_RESCHED:
		ਜਾਓ preempt;
	ਹਾਲਤ PREEMPT_WAKEUP_SHORT:
		fallthrough;
	ਹਾਲਤ PREEMPT_WAKEUP_PICK:
		ਰੋਕੋ;
	}

pick:
	ਜੇ (cfs_rq->h_nr_queued) {
		nse = pick_ਅਗਲਾ_entity(rq, preempt_action != PREEMPT_WAKEUP_SHORT);
		ਜੇ (unlikely(!nse))
			ਜਾਓ pick;

		/* If @p has become the most eligible task, force preemption */
		ਜੇ (nse == pse)
			ਜਾਓ preempt;
	}

	/*
	 * If @p is eligible but not the next task to run then cancel protection
	 * to prevent large scheduling latency
	 */
	ਜੇ (preempt_action == PREEMPT_WAKEUP_SHORT && entity_eligible(cfs_rq, pse))
		ਜਾਓ preempt;
update:
	ਜੇ (sched_feat(ਲੜੀਵਾਰ_ਮੰਜ਼ਿਲ_PARITY))
		update_protect_ਟੁਕੜਾ(cfs_rq, se);

	ਵਾਪਸ;

preempt:
	cancel_protect_ਟੁਕੜਾ(se);

	ਜੇ (preempt_action == PREEMPT_WAKEUP_SHORT)
		ਸੈੱਟ_short_buddy(cfs_rq, pse);

	resched_curr_lazy(rq);
}

ਬਣਤਰ task_struct *pick_task_fair(ਬਣਤਰ rq *rq, ਬਣਤਰ rq_flags *rf)
	__must_hold(__rq_lockp(rq))
{
	ਬਣਤਰ cfs_rq *cfs_rq = &rq->cfs;
	ਬਣਤਰ sched_entity *se;
	ਬਣਤਰ task_struct *p;
	ਪੂਰਨ new_tasks;

again:
	ਜੇ (!cfs_rq->h_nr_queued)
		ਜਾਓ idle;

	/* Might not have done put_prev_entity() */
	ਜੇ (cfs_rq->curr && cfs_rq->curr->on_rq)
		update_curr_eevdf(cfs_rq);

	se = pick_ਅਗਲਾ_entity(rq, true);
	ਜੇ (!se)
		ਜਾਓ again;

	p = task_ਦਾ(se);
	ਵਾਪਸ p;

idle:
	ਜੇ (sched_core_enabled(rq))
		ਵਾਪਸ ਨਿਰਾ;

	new_tasks = sched_balance_newidle(rq, rf);
	ਜੇ (new_tasks < 0)
		ਵਾਪਸ RETRY_TASK;
	ਜੇ (new_tasks > 0)
		ਜਾਓ again;
	ਵਾਪਸ ਨਿਰਾ;
}

ਸਥਿਰ ਬਣਤਰ task_struct *
fair_server_pick_task(ਬਣਤਰ sched_dl_entity *dl_se, ਬਣਤਰ rq_flags *rf)
	__must_hold(__rq_lockp(dl_se->rq))
{
	ਵਾਪਸ pick_task_fair(dl_se->rq, rf);
}

ਖਾਲੀ fair_server_init(ਬਣਤਰ rq *rq)
{
	ਬਣਤਰ sched_dl_entity *dl_se = &rq->fair_server;

	init_dl_entity(dl_se);

	dl_server_init(dl_se, rq, fair_server_pick_task);
}

/*
 * Account for a descheduled task:
 */
ਸਥਿਰ ਖਾਲੀ put_prev_task_fair(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *prev, ਬਣਤਰ task_struct *ਅਗਲਾ)
{
	ਬਣਤਰ sched_entity *se = &prev->se;
	ਬਣਤਰ cfs_rq *cfs_rq = &rq->cfs;
	ਬਣਤਰ sched_entity *nse = ਨਿਰਾ;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_FAIR_ਸਮੂਹ_SCHED
	ਜੇ (ਅਗਲਾ && ਅਗਲਾ->sched_class == &fair_sched_class)
		nse = &ਅਗਲਾ->se;
#ਜੇ_ਅੰਤ

	ਜਦੋਂ (se) {
		cfs_rq = cfs_rq_ਦਾ(se);
		ਜੇ (!nse || cfs_rq->h_curr)
			put_prev_entity(cfs_rq, se);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_FAIR_ਸਮੂਹ_SCHED
		ਜੇ (nse) {
			ਜੇ (is_same_ਸਮੂਹ(se, nse))
				ਰੋਕੋ;

			ਪੂਰਨ d = nse->ਡੂੰਘਾਈ - se->ਡੂੰਘਾਈ;
			ਜੇ (d >= 0) {
				/* nse has equal or greater depth, ascend */
				nse = ਮਾਪਾ_entity(nse);
				/* if nse is the deeper, do not ascend se */
				ਜੇ (d > 0)
					ਜਾਰੀ;
			}
		}
#ਜੇ_ਅੰਤ
		se = ਮਾਪਾ_entity(se);
	}

	/* Put 'current' back into the tree. */
	cfs_rq = &rq->cfs;
	se = &prev->se;
	WARN_ON_ONCE(cfs_rq->curr != se);
	cfs_rq->curr = ਨਿਰਾ;
	ਜੇ (se->on_rq)
		__enqueue_entity(cfs_rq, se);
}

/*
 * sched_yield() is very simple
 */
ਸਥਿਰ ਖਾਲੀ yield_task_fair(ਬਣਤਰ rq *rq)
{
	ਬਣਤਰ task_struct *curr = rq->donor;
	ਬਣਤਰ sched_entity *se = &curr->se;
	ਬਣਤਰ cfs_rq *cfs_rq = &rq->cfs;

	/*
	 * Are we the only task in the tree?
	 */
	ਜੇ (unlikely(rq->nr_running == 1))
		ਵਾਪਸ;

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
	ਜੇ (entity_eligible(cfs_rq, se)) {
		se->vruntime = se->deadline;
		update_deadline(cfs_rq, se);
	}
}

ਸਥਿਰ bool yield_ਮੰਜ਼ਿਲ_task_fair(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p)
{
	ਬਣਤਰ sched_entity *se = &p->se;

	/* !se->on_rq also covers throttled task */
	ਜੇ (!se->on_rq || se->sched_delayed)
		ਵਾਪਸ false;

	/* Tell the scheduler that we'd really like se to run next. */
	ਸੈੱਟ_ਅਗਲਾ_buddy(&task_rq(p)->cfs, se);

	yield_task_fair(rq);

	ਵਾਪਸ true;
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

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ __read_mostly max_ਲੱਦੋ_balance_interval = HZ/10;

ਗਿਣਤੀ fbq_type { regular, remote, all };

/*
 * 'group_type' describes the group of CPUs at the moment of load balancing.
 *
 * The enum is ordered by pulling priority, with the group with lowest priority
 * first so the group_type can simply be compared when selecting the busiest
 * group. See update_sd_pick_busiest().
 */
ਗਿਣਤੀ ਸਮੂਹ_type {
	/* The group has spare capacity that can be used to run more tasks.  */
	ਸਮੂਹ_has_ਵਾਧੂ = 0,
	/*
	 * The group is fully used and the tasks don't compete for more CPU
	 * cycles. Nevertheless, some tasks might wait before running.
	 */
	ਸਮੂਹ_fully_busy,
	/*
	 * One task doesn't fit with CPU's capacity and must be migrated to a
	 * more powerful CPU.
	 */
	ਸਮੂਹ_misfit_task,
	/*
	 * Balance SMT group that's fully busy. Can benefit from migration
	 * a task on SMT with busy sibling to another CPU on idle core.
	 */
	ਸਮੂਹ_smt_balance,
	/*
	 * SD_ASYM_PACKING only: One local CPU with higher capacity is available,
	 * and the task should be migrated to it instead of running on the
	 * current CPU.
	 */
	ਸਮੂਹ_asym_packing,
	/*
	 * The tasks' affinity constraints previously prevented the scheduler
	 * from balancing the load across the system.
	 */
	ਸਮੂਹ_imbalanced,
	/*
	 * There are tasks running on non-preferred LLC, possible to move
	 * them to their preferred LLC without creating too much imbalance.
	 * The priority of group_llc_balance is lower than that of
	 * group_overloaded and higher than that of all other group types.
	 * This is because group_llc_balance may exacerbate load imbalance.
	 * If the LLC balancing attempt fails, the nr_balance_failed
	 * mechanism will trigger other group types to rebalance the load.
	 */
	ਸਮੂਹ_llc_balance,
	/*
	 * The CPU is overloaded and can't provide expected CPU cycles to all
	 * tasks.
	 */
	ਸਮੂਹ_overloaded
};

ਗਿਣਤੀ migration_type {
	migrate_ਲੱਦੋ = 0,
	migrate_util,
	migrate_task,
	migrate_misfit,
	migrate_llc_task
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LBF_ALL_PINNED	0x01
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LBF_NEED_BREAK	0x02
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LBF_DST_PINNED  0x04
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LBF_SOME_PINNED	0x08
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LBF_ਸਰਗਰਮ_LB	0x10
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LBF_LLC_PINNED	0x20
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LBF_ਸਰਗਰਮ_LB_LLC	0x40

ਬਣਤਰ lb_env {
	ਬਣਤਰ sched_domain	*sd;

	ਬਣਤਰ rq		*src_rq;
	ਪੂਰਨ			src_cpu;

	ਪੂਰਨ			dst_cpu;
	ਬਣਤਰ rq		*dst_rq;
	bool			dst_core_idle;

	ਬਣਤਰ cpumask		*dst_grpmask;
	ਪੂਰਨ			new_dst_cpu;
	ਗਿਣਤੀ cpu_idle_type	idle;
	ਲੰਮਾ			imbalance;
	/* The set of CPUs under consideration for load-balancing */
	ਬਣਤਰ cpumask		*cpus;

	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		flags;

	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		loop;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		loop_break;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ		loop_max;

	ਗਿਣਤੀ fbq_type		fbq_type;
	ਗਿਣਤੀ migration_type	migration_type;
	ਬਣਤਰ list_ਸਿਰ	tasks;
};

/*
 * Is this task likely cache-hot:
 */
ਸਥਿਰ ਪੂਰਨ task_hot(ਬਣਤਰ task_struct *p, ਬਣਤਰ lb_env *env)
{
	s64 delta;

	lockdep_assert_rq_held(env->src_rq);

	ਜੇ (p->sched_class != &fair_sched_class)
		ਵਾਪਸ 0;

	ਜੇ (unlikely(task_has_idle_policy(p)))
		ਵਾਪਸ 0;

	/* SMT siblings share cache */
	ਜੇ (env->sd->flags & SD_SHARE_CPUCAPACITY)
		ਵਾਪਸ 0;

	/*
	 * Buddy candidates are cache hot:
	 */
	ਜੇ (sched_feat(CACHE_HOT_BUDDY) && env->dst_rq->nr_running &&
	    (&p->se == cfs_rq_ਦਾ(&p->se)->ਅਗਲਾ))
		ਵਾਪਸ 1;

	ਜੇ (sysctl_sched_migration_ਲਾਗਤ == -1)
		ਵਾਪਸ 1;

	/*
	 * Don't migrate task if the task's cookie does not match
	 * with the destination CPU's core cookie.
	 */
	ਜੇ (!sched_core_cookie_ਮੇਲ(cpu_rq(env->dst_cpu), p))
		ਵਾਪਸ 1;

	ਜੇ (sysctl_sched_migration_ਲਾਗਤ == 0)
		ਵਾਪਸ 0;

	delta = rq_clock_task(env->src_rq) - p->se.exec_ਸ਼ੁਰੂ;

	ਵਾਪਸ delta < (s64)sysctl_sched_migration_ਲਾਗਤ;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_NUMA_BALANCING
/*
 * Returns a positive value, if task migration degrades locality.
 * Returns 0, if task migration is not affected by locality.
 * Returns a negative value, if task migration improves locality i.e migration preferred.
 */
ਸਥਿਰ ਲੰਮਾ migrate_degrades_locality(ਬਣਤਰ task_struct *p, ਬਣਤਰ lb_env *env)
{
	ਬਣਤਰ numa_ਸਮੂਹ *numa_ਸਮੂਹ = rcu_dereference_all(p->numa_ਸਮੂਹ);
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ src_ਭਾਰ, dst_ਭਾਰ;
	ਪੂਰਨ src_nid, dst_nid, dist;

	ਜੇ (!static_branch_likely(&sched_numa_balancing))
		ਵਾਪਸ 0;

	ਜੇ (!p->numa_ਨੁਕਸ || !(env->sd->flags & SD_NUMA))
		ਵਾਪਸ 0;

	src_nid = cpu_ਮੰਜ਼ਿਲ_ਗੰਢ(env->src_cpu);
	dst_nid = cpu_ਮੰਜ਼ਿਲ_ਗੰਢ(env->dst_cpu);

	ਜੇ (src_nid == dst_nid)
		ਵਾਪਸ 0;

	/* Migrating away from the preferred node is always bad. */
	ਜੇ (src_nid == p->numa_preferred_nid) {
		ਜੇ (env->src_rq->nr_running > env->src_rq->nr_preferred_running)
			ਵਾਪਸ 1;
		ਨਹੀਂਤਾਂ
			ਵਾਪਸ 0;
	}

	/* Encourage migration to the preferred node. */
	ਜੇ (dst_nid == p->numa_preferred_nid)
		ਵਾਪਸ -1;

	/* Leaving a core idle is often worse than degrading locality. */
	ਜੇ (env->idle == CPU_IDLE)
		ਵਾਪਸ 0;

	dist = ਗੰਢ_ਦੂਰੀ(src_nid, dst_nid);
	ਜੇ (numa_ਸਮੂਹ) {
		src_ਭਾਰ = ਸਮੂਹ_ਭਾਰ(p, src_nid, dist);
		dst_ਭਾਰ = ਸਮੂਹ_ਭਾਰ(p, dst_nid, dist);
	} ਨਹੀਂਤਾਂ {
		src_ਭਾਰ = task_ਭਾਰ(p, src_nid, dist);
		dst_ਭਾਰ = task_ਭਾਰ(p, dst_nid, dist);
	}

	ਵਾਪਸ src_ਭਾਰ - dst_ਭਾਰ;
}

#ਨਹੀਂਤਾਂ /* !CONFIG_NUMA_BALANCING: */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਲੰਮਾ migrate_degrades_locality(ਬਣਤਰ task_struct *p,
					     ਬਣਤਰ lb_env *env)
{
	ਵਾਪਸ 0;
}
#ਜੇ_ਅੰਤ /* !CONFIG_NUMA_BALANCING */

/*
 * Check whether the task is ineligible on the destination cpu
 *
 * When the PLACE_LAG scheduling feature is enabled and
 * dst_cfs_rq->nr_queued is greater than 1, if the task
 * is ineligible, it will also be ineligible when
 * it is migrated to the destination cpu.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ task_is_ineligible_on_dst_cpu(ਬਣਤਰ task_struct *p, ਪੂਰਨ dest_cpu)
{
	ਬਣਤਰ cfs_rq *dst_cfs_rq = &cpu_rq(dest_cpu)->cfs;

	ਜੇ (sched_feat(ਰੱਖੋ_LAG) && dst_cfs_rq->h_nr_queued &&
	    !entity_eligible(&task_rq(p)->cfs, &p->se))
		ਵਾਪਸ 1;

	ਵਾਪਸ 0;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CACHE
/*
 * The margin used when comparing LLC utilization with CPU capacity.
 * It determines the LLC load level where active LLC aggregation is
 * done.
 * Derived from fits_capacity().
 *
 * (default: ~50%, tunable via debugfs)
 */
ਸਥਿਰ bool ਸਮਾਉਂਦਾ_llc_ਸਮਰੱਥਾ(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ util, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ max)
{
	u32 aggr_pct = llc_overaggr_pct;

	/*
	 * For single core systems, raise the aggregation
	 * threshold to accommodate more tasks.
	 */
	ਜੇ (cpu_smt_ਅੰਕ_ਧਾਗੇ == 1)
		aggr_pct = (aggr_pct * 3 / 2);

	ਵਾਪਸ util * 100 < max * aggr_pct;
}

/*
 * The margin used when comparing utilization.
 * is 'util1' noticeably greater than 'util2'
 * Derived from capacity_greater().
 * Bias is in perentage.
 */
/* Allows dst util to be bigger than src util by up to bias percent */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ util_greater(util1, util2) \
	((util1) * 100 > (util2) * (100 + llc_imb_pct))

ਸਥਿਰ __maybe_unused bool get_llc_stats(ਪੂਰਨ cpu, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ *util,
					 ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ *cap)
{
	ਬਣਤਰ sched_domain_shared *sd_share;

	sd_share = rcu_dereference_all(ਪ੍ਰਤੀ_cpu(sd_llc_shared, cpu));
	ਜੇ (!sd_share)
		ਵਾਪਸ false;

	*util = READ_ONCE(sd_share->util_avg);
	*cap = READ_ONCE(sd_share->ਸਮਰੱਥਾ);

	ਵਾਪਸ true;
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
ਗਿਣਤੀ llc_mig {
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
ਸਥਿਰ ਗਿਣਤੀ llc_mig can_migrate_llc(ਪੂਰਨ src_cpu, ਪੂਰਨ dst_cpu,
				    ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ tsk_util,
				    bool ਮੰਜ਼ਿਲ_pref)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ src_util, dst_util, src_cap, dst_cap;

	ਜੇ (!get_llc_stats(src_cpu, &src_util, &src_cap) ||
	    !get_llc_stats(dst_cpu, &dst_util, &dst_cap))
		ਵਾਪਸ mig_unrestricted;

	src_util = src_util < tsk_util ? 0 : src_util - tsk_util;
	dst_util = dst_util + tsk_util;

	ਜੇ (!ਸਮਾਉਂਦਾ_llc_ਸਮਰੱਥਾ(dst_util, dst_cap) &&
	    !ਸਮਾਉਂਦਾ_llc_ਸਮਰੱਥਾ(src_util, src_cap))
		ਵਾਪਸ mig_unrestricted;

	ਜੇ (ਮੰਜ਼ਿਲ_pref) {
		/*
		 * Don't migrate if we will get preferred LLC too
		 * heavily loaded and if the dest is much busier
		 * than the src, in which case migration will
		 * increase the imbalance too much.
		 */
		ਜੇ (!ਸਮਾਉਂਦਾ_llc_ਸਮਰੱਥਾ(dst_util, dst_cap) &&
		    util_greater(dst_util, src_util))
			ਵਾਪਸ mig_forbid;
	} ਨਹੀਂਤਾਂ {
		/*
		 * Don't migrate if we will leave preferred LLC
		 * too idle, or if this migration leads to the
		 * non-preferred LLC falls within sysctl_aggr_imb percent
		 * of preferred LLC, leading to migration again
		 * back to preferred LLC.
		 */
		ਜੇ (ਸਮਾਉਂਦਾ_llc_ਸਮਰੱਥਾ(src_util, src_cap) ||
		    !util_greater(src_util, dst_util))
			ਵਾਪਸ mig_forbid;
	}
	ਵਾਪਸ mig_llc;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool task_misfits_asym_cpu(ਬਣਤਰ lb_env *env, ਬਣਤਰ task_struct *p)
{
	/*
	 * On asymmetric CPU capacity domains, do not let cache-aware
	 * balancing pull the task onto a destination CPU that cannot
	 * accommodate it. Doing so would turn the task into a misfit on
	 * the destination, trading a cache-locality gain for a capacity
	 * loss. If the task already does not fit its source CPU, the move
	 * cannot make things worse, so let the LLC preference decide.
	 */
	ਜੇ ((env->sd->flags & SD_ASYM_CPUCAPACITY) && p &&
	    !task_ਸਮਾਉਂਦਾ_cpu(p, env->dst_cpu) &&
	    task_ਸਮਾਉਂਦਾ_cpu(p, env->src_cpu))
		ਵਾਪਸ true;

	ਵਾਪਸ false;
}

/*
 * Check if task p can migrate from source LLC to
 * destination LLC in terms of cache aware load balance.
 */
ਸਥਿਰ ਗਿਣਤੀ llc_mig can_migrate_llc_task(ਬਣਤਰ lb_env *env,
					 ਬਣਤਰ task_struct *p)
{
	ਬਣਤਰ sched_cache_ਸਮੂਹ *grp;
	bool ਮੰਜ਼ਿਲ_pref;
	ਪੂਰਨ cpu, src_cpu, dst_cpu;

	ਜੇ (task_misfits_asym_cpu(env, p))
		ਵਾਪਸ mig_forbid;

	src_cpu = env->src_cpu;
	dst_cpu = env->dst_cpu;
	grp = rcu_dereference_all(p->sched_cache_grp);
	ਜੇ (!grp)
		ਵਾਪਸ mig_unrestricted;

	cpu = READ_ONCE(grp->cpu);
	ਜੇ (cpu < 0 || cpus_share_cache(src_cpu, dst_cpu))
		ਵਾਪਸ mig_unrestricted;

	/* skip cache aware load balance for too many threads */
	ਜੇ (invalid_llc_nr(grp, p, dst_cpu) ||
	    exceed_llc_ਸਮਰੱਥਾ(grp, dst_cpu)) {
		ਜੇ (READ_ONCE(grp->cpu) != -1)
			WRITE_ONCE(grp->cpu, -1);
		ਵਾਪਸ mig_unrestricted;
	}

	ਜੇ (cpus_share_cache(dst_cpu, cpu))
		ਮੰਜ਼ਿਲ_pref = true;
	ਨਹੀਂਤਾਂ ਜੇ (cpus_share_cache(src_cpu, cpu))
		ਮੰਜ਼ਿਲ_pref = false;
	ਨਹੀਂਤਾਂ
		ਵਾਪਸ mig_unrestricted;

	ਵਾਪਸ can_migrate_llc(src_cpu, dst_cpu,
			       task_util(p), ਮੰਜ਼ਿਲ_pref);
}

/*
 * Check if active load balance breaks LLC locality in
 * terms of cache aware load balance. The load level and
 * imbalance do not warrant breaking LLC preference per
 * the can_migrate_llc() policy. Here, the benefit of
 * LLC locality outweighs the power efficiency gained from
 * migrating the only runnable task away.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ bool
alb_break_llc(ਬਣਤਰ lb_env *env)
{
	ਜੇ (!sched_cache_enabled())
		ਵਾਪਸ false;

	ਜੇ (cpus_share_cache(env->src_cpu, env->dst_cpu))
		ਵਾਪਸ false;
	/*
	 * All tasks prefer to stay on their current CPU.
	 * Do not pull a task from its preferred CPU if:
	 * 1. It is the only task running and does not exceed
	 *    imbalance allowance; OR
	 * 2. Migrating it away from its preferred LLC would violate
	 *    the cache-aware scheduling policy.
	 */
	ਜੇ (env->src_rq->nr_pref_llc_running &&
	    env->src_rq->nr_pref_llc_running == env->src_rq->cfs.h_nr_runnable) {
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ util = 0;
		ਬਣਤਰ task_struct *cur;

		/*
		 * Migrating misfit tasks from current CPU
		 * to CPU with a better fit.
		 * Prioritize that over LLC preference.
		 */
		ਜੇ (env->migration_type == migrate_misfit)
			ਵਾਪਸ false;

		ਜੇ (env->src_rq->nr_running <= 1)
			ਵਾਪਸ true;

		cur = rcu_dereference_all(env->src_rq->curr);
		ਜੇ (cur && cur->sched_class == &fair_sched_class)
			util = task_util(cur);

		ਜੇ (task_misfits_asym_cpu(env, cur) ||
		    can_migrate_llc(env->src_cpu, env->dst_cpu,
				    util, false) == mig_forbid)
			ਵਾਪਸ true;
	}

	ਵਾਪਸ false;
}

/*
 * Returns true if p's preferred LLC does not match the destination CPU
 * under migrate_llc_task semantics. Passive LB passes migrate_llc_task
 * in env->migration_type, while active LB carries LBF_ACTIVE_LB_LLC in
 * env->flags to avoid overwriting env->migration_type.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ bool
migrate_llc_task_wrong_dst(ਬਣਤਰ task_struct *p, ਬਣਤਰ lb_env *env)
{
	ਵਾਪਸ sched_cache_enabled() &&
	       (env->migration_type == migrate_llc_task ||
		env->flags & LBF_ਸਰਗਰਮ_LB_LLC) &&
	       READ_ONCE(p->preferred_llc) != llc_id(env->dst_cpu);
}

/*
 * Check if migrating task p from env->src_cpu to
 * env->dst_cpu breaks LLC localiy.
 */
ਸਥਿਰ bool migrate_degrades_llc(ਬਣਤਰ task_struct *p, ਬਣਤਰ lb_env *env)
{
	ਜੇ (!sched_cache_enabled())
		ਵਾਪਸ false;

	ਜੇ (task_has_sched_core(p))
		ਵਾਪਸ false;
	/*
	 * Skip over tasks that would degrade LLC locality;
	 * only when nr_balanced_failed is sufficiently high do we
	 * ignore this constraint.
	 *
	 * Threshold of cache_nice_tries is set to 1 higher
	 * than nr_balance_failed to avoid excessive task
	 * migration at the same time.
	 */
	ਜੇ (env->sd->nr_balance_failed >= env->sd->cache_nice_tries + 1)
		ਵਾਪਸ false;

	/*
	 * We know the env->src_cpu has some tasks prefer to
	 * run on env->dst_cpu, skip the tasks do not prefer
	 * env->dst_cpu, and find the one that prefers.
	 */
	ਜੇ (migrate_llc_task_wrong_dst(p, env))
		ਵਾਪਸ true;

	ਜੇ (can_migrate_llc_task(env, p) != mig_forbid)
		ਵਾਪਸ false;

	ਵਾਪਸ true;
}

#ਨਹੀਂਤਾਂ
ਸਥਿਰ ਸਤਰਅੰਦਰ bool get_llc_stats(ਪੂਰਨ cpu, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ *util,
				 ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ *cap)
{
	ਵਾਪਸ false;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
alb_break_llc(ਬਣਤਰ lb_env *env)
{
	ਵਾਪਸ false;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
migrate_llc_task_wrong_dst(ਬਣਤਰ task_struct *p, ਬਣਤਰ lb_env *env)
{
	ਵਾਪਸ false;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
migrate_degrades_llc(ਬਣਤਰ task_struct *p, ਬਣਤਰ lb_env *env)
{
	ਵਾਪਸ false;
}
#ਜੇ_ਅੰਤ
/*
 * can_migrate_task - may task p from runqueue rq be migrated to this_cpu?
 */
ਸਥਿਰ
ਪੂਰਨ can_migrate_task(ਬਣਤਰ task_struct *p, ਬਣਤਰ lb_env *env)
{
	ਲੰਮਾ degrades, hot;

	lockdep_assert_rq_held(env->src_rq);
	ਜੇ (p->sched_task_hot)
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
	ਜੇ ((p->se.sched_delayed) && (env->migration_type != migrate_ਲੱਦੋ))
		ਵਾਪਸ 0;

	ਜੇ (lb_throttled_hierarchy(p, env->dst_cpu))
		ਵਾਪਸ 0;

	/*
	 * We want to prioritize the migration of eligible tasks.
	 * For ineligible tasks we soft-limit them and only allow
	 * them to migrate when nr_balance_failed is non-zero to
	 * avoid load-balancing trying very hard to balance the load.
	 */
	ਜੇ (!env->sd->nr_balance_failed &&
	    task_is_ineligible_on_dst_cpu(p, env->dst_cpu))
		ਵਾਪਸ 0;

	/* Disregard percpu kthreads; they are where they need to be. */
	ਜੇ (kthread_is_ਪ੍ਰਤੀ_cpu(p))
		ਵਾਪਸ 0;

	ਜੇ (task_is_blocked(p))
		ਵਾਪਸ 0;

	ਜੇ (!cpumask_test_cpu(env->dst_cpu, p->cpus_ptr)) {
		ਪੂਰਨ cpu;

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
		ਜੇ (env->idle == CPU_NEWLY_IDLE ||
		    env->flags & (LBF_DST_PINNED | LBF_ਸਰਗਰਮ_LB))
			ਵਾਪਸ 0;

		/* Prevent to re-select dst_cpu via env's CPUs: */
		cpu = cpumask_ਪਹਿਲਾ_and_and(env->dst_grpmask, env->cpus, p->cpus_ptr);

		ਜੇ (cpu < nr_cpu_ids) {
			env->flags |= LBF_DST_PINNED;
			env->new_dst_cpu = cpu;
		}

		ਵਾਪਸ 0;
	}

	/* Record that we found at least one task that could run on dst_cpu */
	env->flags &= ~LBF_ALL_PINNED;

	ਜੇ (task_on_cpu(env->src_rq, p) ||
	    task_current_donor(env->src_rq, p)) {
		schedstat_inc(p->stats.nr_failed_migrations_running);
		ਵਾਪਸ 0;
	}

	/*
	 * Aggressive migration if:
	 * 1) active balance
	 * 2) destination numa is preferred
	 * 3) task is cache cold, or
	 * 4) too many balance attempts have failed.
	 */
	ਜੇ (env->flags & LBF_ਸਰਗਰਮ_LB)
		ਵਾਪਸ !migrate_llc_task_wrong_dst(p, env);

	degrades = migrate_degrades_locality(p, env);
	ਜੇ (!degrades) {
		/*
		 * If the NUMA locality is not broken,
		 * further check if migration would hurt
		 * LLC locality.
		 */
		ਜੇ (migrate_degrades_llc(p, env)) {
			/*
			 * If regular load balancing fails to pull a task
			 * due to LLC locality, this is expected behavior
			 * and we set LBF_LLC_PINNED so we don't increase
			 * nr_balance_failed unecessarily.
			 */
			ਜੇ (env->migration_type != migrate_llc_task)
				env->flags |= LBF_LLC_PINNED;

			ਵਾਪਸ 0;
		}

		hot = task_hot(p, env);
	} ਨਹੀਂਤਾਂ {
		hot = degrades > 0;
	}

	ਜੇ (!hot || env->sd->nr_balance_failed > env->sd->cache_nice_tries) {
		ਜੇ (hot)
			p->sched_task_hot = 1;
		ਵਾਪਸ 1;
	}

	schedstat_inc(p->stats.nr_failed_migrations_hot);
	ਵਾਪਸ 0;
}

/*
 * detach_task() -- detach the task for the migration specified in env
 */
ਸਥਿਰ ਖਾਲੀ detach_task(ਬਣਤਰ task_struct *p, ਬਣਤਰ lb_env *env)
{
	lockdep_assert_rq_held(env->src_rq);

	ਜੇ (p->sched_task_hot) {
		p->sched_task_hot = 0;
		schedstat_inc(env->sd->lb_hot_gained[env->idle]);
		schedstat_inc(p->stats.nr_forced_migrations);
	}

	WARN_ON(task_current(env->src_rq, p));
	WARN_ON(task_current_donor(env->src_rq, p));

	deactivate_task(env->src_rq, p, ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_NOCLOCK);
	ਸੈੱਟ_task_cpu(p, env->dst_cpu);
}

/*
 * detach_one_task() -- tries to dequeue exactly one task from env->src_rq, as
 * part of active balancing operations within "domain".
 *
 * Returns a task if successful and NULL otherwise.
 */
ਸਥਿਰ ਬਣਤਰ task_struct *detach_one_task(ਬਣਤਰ lb_env *env)
{
	ਬਣਤਰ task_struct *p;

	lockdep_assert_rq_held(env->src_rq);

	list_for_each_ਇੰਦਰਾਜ_ਉਲਟ(p,
			&env->src_rq->cfs_tasks, se.ਸਮੂਹ_ਗੰਢ) {
		ਜੇ (!can_migrate_task(p, env))
			ਜਾਰੀ;

		detach_task(p, env);

		/*
		 * Right now, this is only the second place where
		 * lb_gained[env->idle] is updated (other is detach_tasks)
		 * so we can safely collect stats here rather than
		 * inside detach_tasks().
		 */
		schedstat_inc(env->sd->lb_gained[env->idle]);
		ਵਾਪਸ p;
	}
	ਵਾਪਸ ਨਿਰਾ;
}

/*
 * detach_tasks() -- tries to detach up to imbalance load/util/tasks from
 * busiest_rq, as part of a balancing operation within domain "sd".
 *
 * Returns number of detached tasks if successful and 0 otherwise.
 */
ਸਥਿਰ ਪੂਰਨ detach_tasks(ਬਣਤਰ lb_env *env)
{
	ਬਣਤਰ list_ਸਿਰ *tasks = &env->src_rq->cfs_tasks;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ util, ਲੱਦੋ;
	ਬਣਤਰ task_struct *p;
	ਪੂਰਨ detached = 0;

	lockdep_assert_rq_held(env->src_rq);

	/*
	 * Source run queue has been emptied by another CPU, clear
	 * LBF_ALL_PINNED flag as we will not test any task.
	 */
	ਜੇ (env->src_rq->nr_running <= 1) {
		env->flags &= ~LBF_ALL_PINNED;
		ਵਾਪਸ 0;
	}

	ਜੇ (env->imbalance <= 0)
		ਵਾਪਸ 0;

	ਜਦੋਂ (!list_ਖਾਲੀ_ਥਾਵਾਂ(tasks)) {
		/*
		 * We don't want to steal all, otherwise we may be treated likewise,
		 * which could at worst lead to a livelock crash.
		 */
		ਜੇ (env->idle && env->src_rq->nr_running <= 1)
			ਰੋਕੋ;

		env->loop++;
		/* We've more or less seen every task there is, call it quits */
		ਜੇ (env->loop > env->loop_max)
			ਰੋਕੋ;

		/* take a breather every nr_migrate tasks */
		ਜੇ (env->loop > env->loop_break) {
			env->loop_break += SCHED_NR_MIGRATE_BREAK;
			env->flags |= LBF_NEED_BREAK;
			ਰੋਕੋ;
		}

		p = list_ਆਖਰੀ_ਇੰਦਰਾਜ(tasks, ਬਣਤਰ task_struct, se.ਸਮੂਹ_ਗੰਢ);

		ਜੇ (!can_migrate_task(p, env))
			ਜਾਓ ਅਗਲਾ;

		ਚੋਣ (env->migration_type) {
		ਹਾਲਤ migrate_ਲੱਦੋ:
			/*
			 * Depending of the number of CPUs and tasks and the
			 * cgroup hierarchy, task_h_load() can return a null
			 * value. Make sure that env->imbalance decreases
			 * otherwise detach_tasks() will stop only after
			 * detaching up to loop_max tasks.
			 */
			ਲੱਦੋ = max_t(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ, task_h_ਲੱਦੋ(p), 1);

			ਜੇ (sched_feat(LB_MIN) &&
			    ਲੱਦੋ < 16 && !env->sd->nr_balance_failed)
				ਜਾਓ ਅਗਲਾ;

			/*
			 * Make sure that we don't migrate too much load.
			 * Nevertheless, let relax the constraint if
			 * scheduler fails to find a good waiting task to
			 * migrate.
			 */
			ਜੇ (shr_bound(ਲੱਦੋ, env->sd->nr_balance_failed) > env->imbalance)
				ਜਾਓ ਅਗਲਾ;

			env->imbalance -= ਲੱਦੋ;
			ਰੋਕੋ;

		ਹਾਲਤ migrate_util:
			util = task_util_est(p);

			ਜੇ (shr_bound(util, env->sd->nr_balance_failed) > env->imbalance)
				ਜਾਓ ਅਗਲਾ;

			env->imbalance -= util;
			ਰੋਕੋ;

		ਹਾਲਤ migrate_task:
			env->imbalance--;
			ਰੋਕੋ;

		ਹਾਲਤ migrate_misfit:
			/* This is not a misfit task */
			ਜੇ (task_ਸਮਾਉਂਦਾ_cpu(p, env->src_cpu))
				ਜਾਓ ਅਗਲਾ;

			env->imbalance = 0;
			ਰੋਕੋ;

		ਹਾਲਤ migrate_llc_task:
			env->imbalance--;
			ਰੋਕੋ;
		}

		detach_task(p, env);
		list_ਜੋੜੋ(&p->se.ਸਮੂਹ_ਗੰਢ, &env->tasks);

		detached++;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_PREEMPTION
		/*
		 * NEWIDLE balancing is a source of latency, so preemptible
		 * kernels will stop after the first task is detached to minimize
		 * the critical section.
		 */
		ਜੇ (env->idle == CPU_NEWLY_IDLE)
			ਰੋਕੋ;
#ਜੇ_ਅੰਤ

		/*
		 * We only want to steal up to the prescribed amount of
		 * load/util/tasks.
		 */
		ਜੇ (env->imbalance <= 0)
			ਰੋਕੋ;

		ਜਾਰੀ;
ਅਗਲਾ:
		ਜੇ (p->sched_task_hot)
			schedstat_inc(p->stats.nr_failed_migrations_hot);

		list_move(&p->se.ਸਮੂਹ_ਗੰਢ, tasks);
	}

	/*
	 * Right now, this is one of only two places we collect this stat
	 * so we can safely collect detach_one_task() stats here rather
	 * than inside detach_one_task().
	 */
	schedstat_ਜੋੜੋ(env->sd->lb_gained[env->idle], detached);

	ਵਾਪਸ detached;
}

/*
 * attach_tasks() -- attaches all tasks detached by detach_tasks() to their
 * new rq.
 */
ਸਥਿਰ ਖਾਲੀ attach_tasks(ਬਣਤਰ lb_env *env)
{
	__attach_tasks(env->dst_rq, &env->tasks);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_NO_HZ_COMMON
ਸਥਿਰ ਸਤਰਅੰਦਰ bool cfs_rq_has_blocked_ਲੱਦੋ(ਬਣਤਰ cfs_rq *cfs_rq)
{
	ਜੇ (cfs_rq->avg.ਲੱਦੋ_avg)
		ਵਾਪਸ true;

	ਜੇ (cfs_rq->avg.util_avg)
		ਵਾਪਸ true;

	ਵਾਪਸ false;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool others_have_blocked(ਬਣਤਰ rq *rq)
{
	ਜੇ (cpu_util_rt(rq))
		ਵਾਪਸ true;

	ਜੇ (cpu_util_dl(rq))
		ਵਾਪਸ true;

	ਜੇ (hw_ਲੱਦੋ_avg(rq))
		ਵਾਪਸ true;

	ਜੇ (cpu_util_irq(rq))
		ਵਾਪਸ true;

	ਵਾਪਸ false;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ update_blocked_ਲੱਦੋ_tick(ਬਣਤਰ rq *rq)
{
	WRITE_ONCE(rq->ਆਖਰੀ_blocked_ਲੱਦੋ_update_tick, jiffies);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ update_has_blocked_ਲੱਦੋ_ਸਥਿਤੀ(ਬਣਤਰ rq *rq, bool has_blocked_ਲੱਦੋ)
{
	ਜੇ (!has_blocked_ਲੱਦੋ)
		rq->has_blocked_ਲੱਦੋ = 0;
}
#ਨਹੀਂਤਾਂ /* !CONFIG_NO_HZ_COMMON: */
ਸਥਿਰ ਸਤਰਅੰਦਰ bool cfs_rq_has_blocked_ਲੱਦੋ(ਬਣਤਰ cfs_rq *cfs_rq) { ਵਾਪਸ false; }
ਸਥਿਰ ਸਤਰਅੰਦਰ bool others_have_blocked(ਬਣਤਰ rq *rq) { ਵਾਪਸ false; }
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ update_blocked_ਲੱਦੋ_tick(ਬਣਤਰ rq *rq) {}
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ update_has_blocked_ਲੱਦੋ_ਸਥਿਤੀ(ਬਣਤਰ rq *rq, bool has_blocked_ਲੱਦੋ) {}
#ਜੇ_ਅੰਤ /* !CONFIG_NO_HZ_COMMON */

ਸਥਿਰ bool __update_blocked_others(ਬਣਤਰ rq *rq, bool *ਪੂਰਾ)
{
	bool updated;

	/*
	 * update_load_avg() can call cpufreq_update_util(). Make sure that RT,
	 * DL and IRQ signals have been updated before updating CFS.
	 */
	updated = update_other_ਲੱਦੋ_avgs(rq);

	ਜੇ (others_have_blocked(rq))
		*ਪੂਰਾ = false;

	ਵਾਪਸ updated;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_FAIR_ਸਮੂਹ_SCHED

ਸਥਿਰ bool __update_blocked_fair(ਬਣਤਰ rq *rq, bool *ਪੂਰਾ)
{
	ਬਣਤਰ cfs_rq *cfs_rq, *pos;
	bool decayed = false;

	/*
	 * Iterates the task_group tree in a bottom up fashion, see
	 * list_add_leaf_cfs_rq() for details.
	 */
	for_each_ਪੱਤੀ_cfs_rq_ਸੁਰੱਖਿਅਤ(rq, cfs_rq, pos) {
		ਬਣਤਰ sched_entity *se;

		ਜੇ (update_cfs_rq_ਲੱਦੋ_avg(cfs_rq_clock_pelt(cfs_rq), cfs_rq)) {
			update_tg_ਲੱਦੋ_avg(cfs_rq);

			ਜੇ (cfs_rq->nr_queued == 0)
				update_idle_cfs_rq_clock_pelt(cfs_rq);

			ਜੇ (cfs_rq == &rq->cfs)
				decayed = true;
		}

		/* Propagate pending load changes to the parent, if any: */
		se = cfs_rq_se(cfs_rq);
		ਜੇ (se && !skip_blocked_update(se))
			update_ਲੱਦੋ_avg(cfs_rq_ਦਾ(se), se, UPDATE_TG);

		/*
		 * There can be a lot of idle CPU cgroups.  Don't let fully
		 * decayed cfs_rqs linger on the list.
		 */
		ਜੇ (cfs_rq_is_decayed(cfs_rq))
			list_del_ਪੱਤੀ_cfs_rq(cfs_rq);

		/* Don't need periodic decay once load/util_avg are null */
		ਜੇ (cfs_rq_has_blocked_ਲੱਦੋ(cfs_rq))
			*ਪੂਰਾ = false;
	}

	ਵਾਪਸ decayed;
}

/*
 * Compute the hierarchical load factor for cfs_rq and all its ascendants.
 * This needs to be done in a top-down fashion because the load of a child
 * group is a fraction of its parents load.
 */
ਸਥਿਰ ਖਾਲੀ update_cfs_rq_h_ਲੱਦੋ(ਬਣਤਰ cfs_rq *cfs_rq)
{
	ਬਣਤਰ sched_entity *se = cfs_rq_se(cfs_rq);
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ now = jiffies;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੱਦੋ;

	ਜੇ (cfs_rq->ਆਖਰੀ_h_ਲੱਦੋ_update == now)
		ਵਾਪਸ;

	WRITE_ONCE(cfs_rq->h_ਲੱਦੋ_ਅਗਲਾ, ਨਿਰਾ);
	for_each_sched_entity(se) {
		cfs_rq = cfs_rq_ਦਾ(se);
		WRITE_ONCE(cfs_rq->h_ਲੱਦੋ_ਅਗਲਾ, se);
		ਜੇ (cfs_rq->ਆਖਰੀ_h_ਲੱਦੋ_update == now)
			ਰੋਕੋ;
	}

	ਜੇ (!se) {
		cfs_rq->h_ਲੱਦੋ = cfs_rq_ਲੱਦੋ_avg(cfs_rq);
		cfs_rq->ਆਖਰੀ_h_ਲੱਦੋ_update = now;
	}

	ਜਦੋਂ ((se = READ_ONCE(cfs_rq->h_ਲੱਦੋ_ਅਗਲਾ)) != ਨਿਰਾ) {
		ਲੱਦੋ = cfs_rq->h_ਲੱਦੋ;
		ਲੱਦੋ = div64_ul(ਲੱਦੋ * se->avg.ਲੱਦੋ_avg,
				cfs_rq_ਲੱਦੋ_avg(cfs_rq) + 1);
		cfs_rq = ਸਮੂਹ_cfs_rq(se);
		cfs_rq->h_ਲੱਦੋ = ਲੱਦੋ;
		cfs_rq->ਆਖਰੀ_h_ਲੱਦੋ_update = now;
	}
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ task_h_ਲੱਦੋ(ਬਣਤਰ task_struct *p)
{
	ਬਣਤਰ cfs_rq *cfs_rq = task_cfs_rq(p);

	update_cfs_rq_h_ਲੱਦੋ(cfs_rq);
	ਵਾਪਸ div64_ul(p->se.avg.ਲੱਦੋ_avg * cfs_rq->h_ਲੱਦੋ,
			cfs_rq_ਲੱਦੋ_avg(cfs_rq) + 1);
}
#ਨਹੀਂਤਾਂ /* !CONFIG_FAIR_GROUP_SCHED: */
ਸਥਿਰ bool __update_blocked_fair(ਬਣਤਰ rq *rq, bool *ਪੂਰਾ)
{
	ਬਣਤਰ cfs_rq *cfs_rq = &rq->cfs;
	bool decayed;

	decayed = update_cfs_rq_ਲੱਦੋ_avg(cfs_rq_clock_pelt(cfs_rq), cfs_rq);
	ਜੇ (cfs_rq_has_blocked_ਲੱਦੋ(cfs_rq))
		*ਪੂਰਾ = false;

	ਵਾਪਸ decayed;
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ task_h_ਲੱਦੋ(ਬਣਤਰ task_struct *p)
{
	ਵਾਪਸ p->se.avg.ਲੱਦੋ_avg;
}
#ਜੇ_ਅੰਤ /* !CONFIG_FAIR_GROUP_SCHED */

ਸਥਿਰ ਖਾਲੀ __sched_balance_update_blocked_averages(ਬਣਤਰ rq *rq)
{
	bool decayed = false, ਪੂਰਾ = true;

	update_blocked_ਲੱਦੋ_tick(rq);

	decayed |= __update_blocked_others(rq, &ਪੂਰਾ);
	decayed |= __update_blocked_fair(rq, &ਪੂਰਾ);

	update_has_blocked_ਲੱਦੋ_ਸਥਿਤੀ(rq, !ਪੂਰਾ);
	ਜੇ (decayed)
		cpufreq_update_util(rq, 0);
}

ਸਥਿਰ ਖਾਲੀ sched_balance_update_blocked_averages(ਪੂਰਨ cpu)
{
	ਬਣਤਰ rq *rq = cpu_rq(cpu);

	guard(rq_ਤਾਲਾ_irqsave)(rq);
	update_rq_clock(rq);
	__sched_balance_update_blocked_averages(rq);
}

/********** Helpers for sched_balance_find_src_group ************************/

/*
 * sg_lb_stats - stats of a sched_group required for load-balancing:
 */
ਬਣਤਰ sg_lb_stats {
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ avg_ਲੱਦੋ;			/* Avg load            over the CPUs of the group */
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਸਮੂਹ_ਲੱਦੋ;		/* Total load          over the CPUs of the group */
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਸਮੂਹ_ਸਮਰੱਥਾ;		/* Capacity            over the CPUs of the group */
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਸਮੂਹ_util;		/* Total utilization   over the CPUs of the group */
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਸਮੂਹ_runnable;		/* Total runnable time over the CPUs of the group */
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਜੋੜ_nr_running;		/* Nr of all tasks running in the group */
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਜੋੜ_h_nr_running;		/* Nr of CFS tasks running in the group */
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ idle_cpus;                 /* Nr of idle CPUs         in the group */
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਮੂਹ_ਭਾਰ;
	ਗਿਣਤੀ ਸਮੂਹ_type ਸਮੂਹ_type;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਮੂਹ_asym_packing;	/* Tasks should be moved to preferred CPU */
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਮੂਹ_smt_balance;		/* Task on busy SMT be moved */
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਮੂਹ_llc_balance;		/* Tasks should be moved to preferred LLC */
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਸਮੂਹ_misfit_task_ਲੱਦੋ;	/* A CPU has a task too big for its capacity */
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਮੂਹ_overutilized;	/* At least one CPU is overutilized in the group */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_NUMA_BALANCING
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ nr_numa_running;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ nr_preferred_running;
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CACHE
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ nr_pref_dst_llc;
#ਜੇ_ਅੰਤ
};

/*
 * sd_lb_stats - stats of a sched_domain required for load-balancing:
 */
ਬਣਤਰ sd_lb_stats {
	ਬਣਤਰ sched_ਸਮੂਹ *busiest;		/* Busiest group in this sd */
	ਬਣਤਰ sched_ਸਮੂਹ *local;		/* Local group in this sd */
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਕੁੱਲ_ਲੱਦੋ;		/* Total load of all groups in sd */
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਕੁੱਲ_ਸਮਰੱਥਾ;		/* Total capacity of all groups in sd */
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ avg_ਲੱਦੋ;			/* Average load across all groups in sd */
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ prefer_sibling;		/* Tasks should go to sibling first */

	ਬਣਤਰ sg_lb_stats busiest_stat;	/* Statistics of the busiest group */
	ਬਣਤਰ sg_lb_stats local_stat;		/* Statistics of the local group */
};

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ init_sd_lb_stats(ਬਣਤਰ sd_lb_stats *sds)
{
	/*
	 * Skimp on the clearing to avoid duplicate work. We can avoid clearing
	 * local_stat because update_sg_lb_stats() does a full clear/assignment.
	 * We must however set busiest_stat::group_type and
	 * busiest_stat::idle_cpus to the worst busiest group because
	 * update_sd_pick_busiest() reads these before assignment.
	 */
	*sds = (ਬਣਤਰ sd_lb_stats){
		.busiest = ਨਿਰਾ,
		.local = ਨਿਰਾ,
		.ਕੁੱਲ_ਲੱਦੋ = 0UL,
		.ਕੁੱਲ_ਸਮਰੱਥਾ = 0UL,
		.busiest_stat = {
			.idle_cpus = UINT_MAX,
			.ਸਮੂਹ_type = ਸਮੂਹ_has_ਵਾਧੂ,
		},
	};
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ scale_rt_ਸਮਰੱਥਾ(ਪੂਰਨ cpu)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ max = get_actual_cpu_ਸਮਰੱਥਾ(cpu);
	ਬਣਤਰ rq *rq = cpu_rq(cpu);
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਵਰਤਿਆ, ਮੁਕਤ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ irq;

	irq = cpu_util_irq(rq);

	ਜੇ (unlikely(irq >= max))
		ਵਾਪਸ 1;

	/*
	 * avg_rt.util_avg and avg_dl.util_avg track binary signals
	 * (running and not running) with weights 0 and 1024 respectively.
	 */
	ਵਰਤਿਆ = cpu_util_rt(rq);
	ਵਰਤਿਆ += cpu_util_dl(rq);

	ਜੇ (unlikely(ਵਰਤਿਆ >= max))
		ਵਾਪਸ 1;

	ਮੁਕਤ = max - ਵਰਤਿਆ;

	ਵਾਪਸ scale_irq_ਸਮਰੱਥਾ(ਮੁਕਤ, irq, max);
}

ਸਥਿਰ ਖਾਲੀ update_cpu_ਸਮਰੱਥਾ(ਬਣਤਰ sched_domain *sd, ਪੂਰਨ cpu)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਸਮਰੱਥਾ = scale_rt_ਸਮਰੱਥਾ(cpu);
	ਬਣਤਰ sched_ਸਮੂਹ *sdg = sd->groups;

	ਜੇ (!ਸਮਰੱਥਾ)
		ਸਮਰੱਥਾ = 1;

	cpu_rq(cpu)->cpu_ਸਮਰੱਥਾ = ਸਮਰੱਥਾ;
	trace_sched_cpu_ਸਮਰੱਥਾ_tp(cpu_rq(cpu));

	sdg->sgc->ਸਮਰੱਥਾ = ਸਮਰੱਥਾ;
	sdg->sgc->min_ਸਮਰੱਥਾ = ਸਮਰੱਥਾ;
	sdg->sgc->max_ਸਮਰੱਥਾ = ਸਮਰੱਥਾ;
}

ਖਾਲੀ update_ਸਮੂਹ_ਸਮਰੱਥਾ(ਬਣਤਰ sched_domain *sd, ਪੂਰਨ cpu)
{
	ਬਣਤਰ sched_domain *ਬੱਚਾ = sd->ਬੱਚਾ;
	ਬਣਤਰ sched_ਸਮੂਹ *ਸਮੂਹ, *sdg = sd->groups;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਸਮਰੱਥਾ, min_ਸਮਰੱਥਾ, max_ਸਮਰੱਥਾ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ interval;

	interval = msecs_ਮੰਜ਼ਿਲ_jiffies(sd->balance_interval);
	interval = clamp(interval, 1UL, max_ਲੱਦੋ_balance_interval);
	sdg->sgc->ਅਗਲਾ_update = jiffies + interval;

	ਜੇ (!ਬੱਚਾ) {
		update_cpu_ਸਮਰੱਥਾ(sd, cpu);
		ਵਾਪਸ;
	}

	ਸਮਰੱਥਾ = 0;
	min_ਸਮਰੱਥਾ = ULONG_MAX;
	max_ਸਮਰੱਥਾ = 0;

	ਜੇ (ਬੱਚਾ->flags & SD_NUMA) {
		/*
		 * SD_NUMA domains cannot assume that child groups
		 * span the current group.
		 */

		for_each_cpu(cpu, sched_ਸਮੂਹ_ਵਿਸਤਾਰ(sdg)) {
			ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ cpu_cap = ਸਮਰੱਥਾ_ਦਾ(cpu);

			ਸਮਰੱਥਾ += cpu_cap;
			min_ਸਮਰੱਥਾ = min(cpu_cap, min_ਸਮਰੱਥਾ);
			max_ਸਮਰੱਥਾ = max(cpu_cap, max_ਸਮਰੱਥਾ);
		}
	} ਨਹੀਂਤਾਂ  {
		/*
		 * !SD_NUMA domains can assume that child groups
		 * span the current group.
		 */

		ਸਮੂਹ = ਬੱਚਾ->groups;
		ਕਰੋ {
			ਬਣਤਰ sched_ਸਮੂਹ_ਸਮਰੱਥਾ *sgc = ਸਮੂਹ->sgc;

			ਸਮਰੱਥਾ += sgc->ਸਮਰੱਥਾ;
			min_ਸਮਰੱਥਾ = min(sgc->min_ਸਮਰੱਥਾ, min_ਸਮਰੱਥਾ);
			max_ਸਮਰੱਥਾ = max(sgc->max_ਸਮਰੱਥਾ, max_ਸਮਰੱਥਾ);
			ਸਮੂਹ = ਸਮੂਹ->ਅਗਲਾ;
		} ਜਦੋਂ (ਸਮੂਹ != ਬੱਚਾ->groups);
	}

	sdg->sgc->ਸਮਰੱਥਾ = ਸਮਰੱਥਾ;
	sdg->sgc->min_ਸਮਰੱਥਾ = min_ਸਮਰੱਥਾ;
	sdg->sgc->max_ਸਮਰੱਥਾ = max_ਸਮਰੱਥਾ;
}

/*
 * Check whether the capacity of the rq has been noticeably reduced by side
 * activity. The imbalance_pct is used for the threshold.
 * Return true is the capacity is reduced
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ
check_cpu_ਸਮਰੱਥਾ(ਬਣਤਰ rq *rq, ਬਣਤਰ sched_domain *sd)
{
	ਵਾਪਸ ((rq->cpu_ਸਮਰੱਥਾ * sd->imbalance_pct) <
				(arch_scale_cpu_ਸਮਰੱਥਾ(cpu_ਦਾ(rq)) * 100));
}

/* Check if the rq has a misfit task */
ਸਥਿਰ ਸਤਰਅੰਦਰ bool check_misfit_ਸਥਿਤੀ(ਬਣਤਰ rq *rq)
{
	ਵਾਪਸ rq->misfit_task_ਲੱਦੋ;
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

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ sg_imbalanced(ਬਣਤਰ sched_ਸਮੂਹ *ਸਮੂਹ)
{
	ਵਾਪਸ ਸਮੂਹ->sgc->imbalance;
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
ਸਥਿਰ ਸਤਰਅੰਦਰ bool
ਸਮੂਹ_has_ਸਮਰੱਥਾ(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ imbalance_pct, ਬਣਤਰ sg_lb_stats *sgs)
{
	ਜੇ (sgs->ਜੋੜ_nr_running < sgs->ਸਮੂਹ_ਭਾਰ)
		ਵਾਪਸ true;

	ਜੇ ((sgs->ਸਮੂਹ_ਸਮਰੱਥਾ * imbalance_pct) <
			(sgs->ਸਮੂਹ_runnable * 100))
		ਵਾਪਸ false;

	ਜੇ ((sgs->ਸਮੂਹ_ਸਮਰੱਥਾ * 100) >
			(sgs->ਸਮੂਹ_util * imbalance_pct))
		ਵਾਪਸ true;

	ਵਾਪਸ false;
}

/*
 *  group_is_overloaded returns true if the group has more tasks than it can
 *  handle.
 *  group_is_overloaded is not equals to !group_has_capacity because a group
 *  with the exact right number of tasks, has no more spare capacity but is not
 *  overloaded so both group_has_capacity and group_is_overloaded return
 *  false.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ bool
ਸਮੂਹ_is_overloaded(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ imbalance_pct, ਬਣਤਰ sg_lb_stats *sgs)
{
	/*
	 * With EAS and uclamp, 1 CPU in the group must be overutilized to
	 * consider the group overloaded.
	 */
	ਜੇ (sched_energy_enabled() && !sgs->ਸਮੂਹ_overutilized)
		ਵਾਪਸ false;

	ਜੇ (sgs->ਜੋੜ_nr_running <= sgs->ਸਮੂਹ_ਭਾਰ)
		ਵਾਪਸ false;

	ਜੇ ((sgs->ਸਮੂਹ_ਸਮਰੱਥਾ * 100) <
			(sgs->ਸਮੂਹ_util * imbalance_pct))
		ਵਾਪਸ true;

	ਜੇ ((sgs->ਸਮੂਹ_ਸਮਰੱਥਾ * imbalance_pct) <
			(sgs->ਸਮੂਹ_runnable * 100))
		ਵਾਪਸ true;

	ਵਾਪਸ false;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਗਿਣਤੀ
ਸਮੂਹ_type ਸਮੂਹ_classify(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ imbalance_pct,
			  ਬਣਤਰ sched_ਸਮੂਹ *ਸਮੂਹ,
			  ਬਣਤਰ sg_lb_stats *sgs)
{
	ਜੇ (ਸਮੂਹ_is_overloaded(imbalance_pct, sgs))
		ਵਾਪਸ ਸਮੂਹ_overloaded;

	ਜੇ (sgs->ਸਮੂਹ_llc_balance)
		ਵਾਪਸ ਸਮੂਹ_llc_balance;

	ਜੇ (sg_imbalanced(ਸਮੂਹ))
		ਵਾਪਸ ਸਮੂਹ_imbalanced;

	ਜੇ (sgs->ਸਮੂਹ_asym_packing)
		ਵਾਪਸ ਸਮੂਹ_asym_packing;

	ਜੇ (sgs->ਸਮੂਹ_smt_balance)
		ਵਾਪਸ ਸਮੂਹ_smt_balance;

	ਜੇ (sgs->ਸਮੂਹ_misfit_task_ਲੱਦੋ)
		ਵਾਪਸ ਸਮੂਹ_misfit_task;

	ਜੇ (!ਸਮੂਹ_has_ਸਮਰੱਥਾ(imbalance_pct, sgs))
		ਵਾਪਸ ਸਮੂਹ_fully_busy;

	ਵਾਪਸ ਸਮੂਹ_has_ਵਾਧੂ;
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
ਸਥਿਰ bool sched_use_asym_prio(ਬਣਤਰ sched_domain *sd, ਪੂਰਨ cpu)
{
	ਜੇ (!(sd->flags & SD_ASYM_PACKING))
		ਵਾਪਸ false;

	ਜੇ (!sched_smt_ਸਰਗਰਮ())
		ਵਾਪਸ true;

	ਵਾਪਸ sd->flags & SD_SHARE_CPUCAPACITY || is_core_idle(cpu);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool sched_asym(ਬਣਤਰ sched_domain *sd, ਪੂਰਨ dst_cpu, ਪੂਰਨ src_cpu)
{
	/*
	 * First check if @dst_cpu can do asym_packing load balance. Only do it
	 * if it has higher priority than @src_cpu.
	 */
	ਵਾਪਸ sched_use_asym_prio(sd, dst_cpu) &&
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
ਸਥਿਰ ਸਤਰਅੰਦਰ bool
sched_ਸਮੂਹ_asym(ਬਣਤਰ lb_env *env, ਬਣਤਰ sg_lb_stats *sgs, ਬਣਤਰ sched_ਸਮੂਹ *ਸਮੂਹ)
{
	/*
	 * CPU priorities do not make sense for SMT cores with more than one
	 * busy sibling.
	 */
	ਜੇ ((ਸਮੂਹ->flags & SD_SHARE_CPUCAPACITY) &&
	    (sgs->ਸਮੂਹ_ਭਾਰ - sgs->idle_cpus != 1))
		ਵਾਪਸ false;

	ਵਾਪਸ sched_asym(env->sd, env->dst_cpu, READ_ONCE(ਸਮੂਹ->asym_prefer_cpu));
}

/* One group has more than one SMT CPU while the other group does not */
ਸਥਿਰ ਸਤਰਅੰਦਰ bool smt_vs_nonsmt_groups(ਬਣਤਰ sched_ਸਮੂਹ *sg1,
				    ਬਣਤਰ sched_ਸਮੂਹ *sg2)
{
	ਜੇ (!sg1 || !sg2)
		ਵਾਪਸ false;

	ਵਾਪਸ (sg1->flags & SD_SHARE_CPUCAPACITY) !=
		(sg2->flags & SD_SHARE_CPUCAPACITY);
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool smt_balance(ਬਣਤਰ lb_env *env, ਬਣਤਰ sg_lb_stats *sgs,
			       ਬਣਤਰ sched_ਸਮੂਹ *ਸਮੂਹ)
{
	ਜੇ (!env->idle)
		ਵਾਪਸ false;

	/*
	 * For SMT source group, it is better to move a task
	 * to a CPU that doesn't have multiple tasks sharing its CPU capacity.
	 * Note that if a group has a single SMT, SD_SHARE_CPUCAPACITY
	 * will not be on.
	 */
	ਜੇ (ਸਮੂਹ->flags & SD_SHARE_CPUCAPACITY &&
	    sgs->ਜੋੜ_h_nr_running > 1)
		ਵਾਪਸ true;

	ਵਾਪਸ false;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਲੰਮਾ sibling_imbalance(ਬਣਤਰ lb_env *env,
				    ਬਣਤਰ sd_lb_stats *sds,
				    ਬਣਤਰ sg_lb_stats *busiest,
				    ਬਣਤਰ sg_lb_stats *local)
{
	ਪੂਰਨ ncores_busiest, ncores_local;
	ਲੰਮਾ imbalance;

	ਜੇ (!env->idle || !busiest->ਜੋੜ_nr_running)
		ਵਾਪਸ 0;

	ncores_busiest = sds->busiest->cores;
	ncores_local = sds->local->cores;

	ਜੇ (ncores_busiest == ncores_local) {
		imbalance = busiest->ਜੋੜ_nr_running;
		lsub_ਸਕਾਰਾਤਮਕ(&imbalance, local->ਜੋੜ_nr_running);
		ਵਾਪਸ imbalance;
	}

	/* Balance such that nr_running/ncores ratio are same on both groups */
	imbalance = ncores_local * busiest->ਜੋੜ_nr_running;
	lsub_ਸਕਾਰਾਤਮਕ(&imbalance, ncores_busiest * local->ਜੋੜ_nr_running);
	/* Normalize imbalance and do rounding on normalization */
	imbalance = 2 * imbalance + ncores_local + ncores_busiest;
	imbalance /= ncores_local + ncores_busiest;

	/* Take advantage of resource in an empty sched group */
	ਜੇ (imbalance <= 1 && local->ਜੋੜ_nr_running == 0 &&
	    busiest->ਜੋੜ_nr_running > 1)
		imbalance = 2;

	ਵਾਪਸ imbalance;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
sched_reduced_ਸਮਰੱਥਾ(ਬਣਤਰ rq *rq, ਬਣਤਰ sched_domain *sd)
{
	/*
	 * When there is more than 1 task, the group_overloaded case already
	 * takes care of cpu with reduced capacity
	 */
	ਜੇ (rq->cfs.h_nr_runnable != 1)
		ਵਾਪਸ false;

	ਵਾਪਸ check_cpu_ਸਮਰੱਥਾ(rq, sd);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CACHE
/*
 * Record the statistics for this scheduler group for later
 * use. These values guide load balancing on aggregating tasks
 * to a LLC.
 */
ਸਥਿਰ ਖਾਲੀ ਰਿਕਾਰਡ_sg_llc_stats(ਬਣਤਰ lb_env *env,
				ਬਣਤਰ sg_lb_stats *sgs,
				ਬਣਤਰ sched_ਸਮੂਹ *ਸਮੂਹ)
{
	ਬਣਤਰ sched_domain_shared *sd_share;
	ਪੂਰਨ cpu;

	ਜੇ (!sched_cache_enabled() || env->idle == CPU_NEWLY_IDLE)
		ਵਾਪਸ;

	/* Only care about sched domain spanning multiple LLCs */
	ਜੇ (env->sd->ਬੱਚਾ != rcu_dereference_all(ਪ੍ਰਤੀ_cpu(sd_llc, env->dst_cpu)))
		ਵਾਪਸ;

	/*
	 * A redo repeats stats published on the first pass. Hotplug can
	 * still leave env->cpus partial, so util would not match capacity.
	 */
	ਜੇ ((env->flags & LBF_ALL_PINNED) ||
	    !cpumask_subset(sched_ਸਮੂਹ_ਵਿਸਤਾਰ(ਸਮੂਹ), env->cpus))
		ਵਾਪਸ;

	/*
	 * At this point we know this group spans a LLC domain.
	 * Record the statistic of this group in its corresponding
	 * shared LLC domain.
	 * Note: sd_share cannot be obtained via sd->child->shared,
	 * because the latter refers to the domain that covers the
	 * local group. Instead, sd_share should be located using
	 * the first CPU of the LLC group.
	 */
	cpu = cpumask_ਪਹਿਲਾ(sched_ਸਮੂਹ_ਵਿਸਤਾਰ(ਸਮੂਹ));
	sd_share = rcu_dereference_all(ਪ੍ਰਤੀ_cpu(sd_llc_shared, cpu));
	ਜੇ (!sd_share)
		ਵਾਪਸ;

	ਜੇ (READ_ONCE(sd_share->util_avg) != sgs->ਸਮੂਹ_util)
		WRITE_ONCE(sd_share->util_avg, sgs->ਸਮੂਹ_util);

	ਜੇ (unlikely(READ_ONCE(sd_share->ਸਮਰੱਥਾ) != sgs->ਸਮੂਹ_ਸਮਰੱਥਾ))
		WRITE_ONCE(sd_share->ਸਮਰੱਥਾ, sgs->ਸਮੂਹ_ਸਮਰੱਥਾ);
}

/*
 * Do LLC balance on sched group that contains LLC, and have tasks preferring
 * to run on LLC in idle dst_cpu.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ bool llc_balance(ਬਣਤਰ lb_env *env, ਬਣਤਰ sg_lb_stats *sgs,
			       ਬਣਤਰ sched_ਸਮੂਹ *ਸਮੂਹ)
{
	ਜੇ (!sched_cache_enabled())
		ਵਾਪਸ false;

	ਜੇ (env->sd->flags & SD_SHARE_LLC)
		ਵਾਪਸ false;

	/*
	 * On asymmetric domains, group_misfit_task_load
	 * should be prioritized to move tasks to CPU that fit them
	 * over aggregating tasks to their preferred LLC.
	 */
	ਜੇ ((env->sd->flags & SD_ASYM_CPUCAPACITY) &&
	    sgs->ਸਮੂਹ_misfit_task_ਲੱਦੋ)
		ਵਾਪਸ false;

	/*
	 * Skip cache aware tagging if nr_balanced_failed is sufficiently high.
	 * Threshold of cache_nice_tries is set to 1 higher than nr_balance_failed
	 * to avoid excessive task migration at the same time.
	 */
	ਜੇ (env->sd->nr_balance_failed >= env->sd->cache_nice_tries + 1)
		ਵਾਪਸ false;

	ਜੇ (sgs->nr_pref_dst_llc &&
	    can_migrate_llc(cpumask_ਪਹਿਲਾ(sched_ਸਮੂਹ_ਵਿਸਤਾਰ(ਸਮੂਹ)),
			    env->dst_cpu, 0, true) == mig_llc)
		ਵਾਪਸ true;

	ਵਾਪਸ false;
}

ਸਥਿਰ bool update_llc_busiest(ਬਣਤਰ lb_env *env,
			       ਬਣਤਰ sg_lb_stats *busiest,
			       ਬਣਤਰ sg_lb_stats *sgs)
{
	/*
	 * There are more tasks that want to run on dst_cpu's LLC.
	 */
	ਵਾਪਸ sgs->nr_pref_dst_llc > busiest->nr_pref_dst_llc;
}
#ਨਹੀਂਤਾਂ
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ ਰਿਕਾਰਡ_sg_llc_stats(ਬਣਤਰ lb_env *env, ਬਣਤਰ sg_lb_stats *sgs,
				       ਬਣਤਰ sched_ਸਮੂਹ *ਸਮੂਹ)
{
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool llc_balance(ਬਣਤਰ lb_env *env, ਬਣਤਰ sg_lb_stats *sgs,
			       ਬਣਤਰ sched_ਸਮੂਹ *ਸਮੂਹ)
{
	ਵਾਪਸ false;
}

ਸਥਿਰ bool update_llc_busiest(ਬਣਤਰ lb_env *env,
			       ਬਣਤਰ sg_lb_stats *busiest,
			       ਬਣਤਰ sg_lb_stats *sgs)
{
	ਵਾਪਸ false;
}
#ਜੇ_ਅੰਤ

/**
 * update_sg_lb_stats - Update sched_group's statistics for load balancing.
 * @env: The load balancing environment.
 * @sds: Load-balancing data with statistics of the local group.
 * @group: sched_group whose statistics are to be updated.
 * @sgs: variable to hold the statistics for this group.
 * @sg_overloaded: sched_group is overloaded
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ update_sg_lb_stats(ਬਣਤਰ lb_env *env,
				      ਬਣਤਰ sd_lb_stats *sds,
				      ਬਣਤਰ sched_ਸਮੂਹ *ਸਮੂਹ,
				      ਬਣਤਰ sg_lb_stats *sgs,
				      bool *sg_overloaded)
{
	ਪੂਰਨ i, nr_running, local_ਸਮੂਹ, sd_flags = env->sd->flags;
	bool balancing_at_rd = !env->sd->ਮਾਪਾ;

	memset(sgs, 0, ਆਕਾਰ(*sgs));

	local_ਸਮੂਹ = ਸਮੂਹ == sds->local;

	for_each_cpu_and(i, sched_ਸਮੂਹ_ਵਿਸਤਾਰ(ਸਮੂਹ), env->cpus) {
		ਬਣਤਰ rq *rq = cpu_rq(i);
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੱਦੋ = cpu_ਲੱਦੋ(rq);

		sgs->ਸਮੂਹ_ਲੱਦੋ += ਲੱਦੋ;
		sgs->ਸਮੂਹ_util += cpu_util_cfs(i);
		sgs->ਸਮੂਹ_runnable += cpu_runnable(rq);
		sgs->ਜੋੜ_h_nr_running += rq->cfs.h_nr_runnable;

		nr_running = rq->nr_running;
		sgs->ਜੋੜ_nr_running += nr_running;

		ਜੇ (cpu_overutilized(i))
			sgs->ਸਮੂਹ_overutilized = 1;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CACHE
		ਜੇ (sched_cache_enabled()) {
			ਬਣਤਰ sched_domain *sd_tmp;
			ਪੂਰਨ dst_llc;

			dst_llc = llc_id(env->dst_cpu);
			ਜੇ (llc_id(i) != dst_llc) {
				sd_tmp = rcu_dereference_all(rq->sd);
				ਜੇ (sd_tmp && (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)dst_llc < sd_tmp->llc_max)
					sgs->nr_pref_dst_llc += sd_tmp->llc_counts[dst_llc];
			}
		}
#ਜੇ_ਅੰਤ

		/*
		 * No need to call idle_cpu() if nr_running is not 0
		 */
		ਜੇ (!nr_running && idle_cpu(i)) {
			sgs->idle_cpus++;
			/* Idle cpu can't have misfit task */
			ਜਾਰੀ;
		}

		/* Overload indicator is only updated at root domain */
		ਜੇ (balancing_at_rd && nr_running > 1)
			*sg_overloaded = 1;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_NUMA_BALANCING
		/* Only fbq_classify_group() uses this to classify NUMA groups */
		ਜੇ (sd_flags & SD_NUMA) {
			sgs->nr_numa_running += rq->nr_numa_running;
			sgs->nr_preferred_running += rq->nr_preferred_running;
		}
#ਜੇ_ਅੰਤ
		ਜੇ (local_ਸਮੂਹ)
			ਜਾਰੀ;

		ਜੇ (sd_flags & SD_ASYM_CPUCAPACITY) {
			ਜੇ (rq->misfit_task_ਲੱਦੋ) {
				/*
				 * Always mark the root domain overloaded so big
				 * CPUs can pick up misfit tasks via newly idle
				 * balance.
				 */
				ਜੇ (balancing_at_rd)
					*sg_overloaded = 1;

				/*
				 * Only account misfit load if @dst_cpu can
				 * help; otherwise, the group may be classified
				 * as misfit_task and update_sd_pick_busiest()
				 * will skip it.
				 */
				ਜੇ (ਸਮਰੱਥਾ_greater(ਸਮਰੱਥਾ_ਦਾ(env->dst_cpu),
						     ਸਮੂਹ->sgc->max_ਸਮਰੱਥਾ) &&
				    (sgs->ਸਮੂਹ_misfit_task_ਲੱਦੋ < rq->misfit_task_ਲੱਦੋ))
					sgs->ਸਮੂਹ_misfit_task_ਲੱਦੋ = rq->misfit_task_ਲੱਦੋ;
			}
		} ਨਹੀਂਤਾਂ ਜੇ (env->idle && sched_reduced_ਸਮਰੱਥਾ(rq, env->sd)) {
			/* Check for a task running on a CPU with reduced capacity */
			ਜੇ (sgs->ਸਮੂਹ_misfit_task_ਲੱਦੋ < ਲੱਦੋ)
				sgs->ਸਮੂਹ_misfit_task_ਲੱਦੋ = ਲੱਦੋ;
		}
	}

	sgs->ਸਮੂਹ_ਸਮਰੱਥਾ = ਸਮੂਹ->sgc->ਸਮਰੱਥਾ;

	sgs->ਸਮੂਹ_ਭਾਰ = ਸਮੂਹ->ਸਮੂਹ_ਭਾਰ;

	ਜੇ (!local_ਸਮੂਹ) {
		/* Check if dst CPU is idle and preferred to this group */
		ਜੇ (env->idle && sgs->ਜੋੜ_h_nr_running &&
		    sched_ਸਮੂਹ_asym(env, sgs, ਸਮੂਹ))
			sgs->ਸਮੂਹ_asym_packing = 1;

		/* Check for loaded SMT group to be balanced to dst CPU */
		ਜੇ (smt_balance(env, sgs, ਸਮੂਹ))
			sgs->ਸਮੂਹ_smt_balance = 1;

		/* Check for tasks in this group can be moved to their preferred LLC */
		ਜੇ (llc_balance(env, sgs, ਸਮੂਹ))
			sgs->ਸਮੂਹ_llc_balance = 1;
	}

	sgs->ਸਮੂਹ_type = ਸਮੂਹ_classify(env->sd->imbalance_pct, ਸਮੂਹ, sgs);

	ਰਿਕਾਰਡ_sg_llc_stats(env, sgs, ਸਮੂਹ);
	/* Computing avg_load makes sense only when group is overloaded */
	ਜੇ (sgs->ਸਮੂਹ_type == ਸਮੂਹ_overloaded)
		sgs->avg_ਲੱਦੋ = (sgs->ਸਮੂਹ_ਲੱਦੋ * SCHED_ਸਮਰੱਥਾ_SCALE) /
				sgs->ਸਮੂਹ_ਸਮਰੱਥਾ;
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
ਸਥਿਰ bool update_sd_pick_busiest(ਬਣਤਰ lb_env *env,
				   ਬਣਤਰ sd_lb_stats *sds,
				   ਬਣਤਰ sched_ਸਮੂਹ *sg,
				   ਬਣਤਰ sg_lb_stats *sgs)
{
	ਬਣਤਰ sg_lb_stats *busiest = &sds->busiest_stat;

	/* Make sure that there is at least one task to pull */
	ਜੇ (!sgs->ਜੋੜ_h_nr_running)
		ਵਾਪਸ false;

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
	ਜੇ ((env->sd->flags & SD_ASYM_CPUCAPACITY) &&
	    (sgs->ਸਮੂਹ_type == ਸਮੂਹ_misfit_task) &&
	    (!env->dst_core_idle ||
	     !ਸਮਰੱਥਾ_greater(ਸਮਰੱਥਾ_ਦਾ(env->dst_cpu), sg->sgc->max_ਸਮਰੱਥਾ) ||
	     sds->local_stat.ਸਮੂਹ_type != ਸਮੂਹ_has_ਵਾਧੂ))
		ਵਾਪਸ false;

	/*
	 * Candidate sg has no more than one task per CPU and has higher
	 * per-CPU capacity. Migrating tasks to less capable CPUs may harm
	 * throughput. Maximize throughput, power/energy consequences are not
	 * considered.
	 */
	ਜੇ ((env->sd->flags & SD_ASYM_CPUCAPACITY) &&
	    (sgs->ਸਮੂਹ_type <= ਸਮੂਹ_fully_busy) &&
	    (ਸਮਰੱਥਾ_greater(sg->sgc->min_ਸਮਰੱਥਾ, ਸਮਰੱਥਾ_ਦਾ(env->dst_cpu))))
		ਵਾਪਸ false;

	ਜੇ (sgs->ਸਮੂਹ_type > busiest->ਸਮੂਹ_type)
		ਵਾਪਸ true;

	ਜੇ (sgs->ਸਮੂਹ_type < busiest->ਸਮੂਹ_type)
		ਵਾਪਸ false;

	/*
	 * The candidate and the current busiest group are the same type of
	 * group. Let check which one is the busiest according to the type.
	 */

	ਚੋਣ (sgs->ਸਮੂਹ_type) {
	ਹਾਲਤ ਸਮੂਹ_overloaded:
		/* Select the overloaded group with highest avg_load. */
		ਵਾਪਸ sgs->avg_ਲੱਦੋ > busiest->avg_ਲੱਦੋ;

	ਹਾਲਤ ਸਮੂਹ_llc_balance:
		/* Select the group with most tasks preferring dst LLC */
		ਵਾਪਸ update_llc_busiest(env, busiest, sgs);

	ਹਾਲਤ ਸਮੂਹ_imbalanced:
		/*
		 * Select the 1st imbalanced group as we don't have any way to
		 * choose one more than another.
		 */
		ਵਾਪਸ false;

	ਹਾਲਤ ਸਮੂਹ_asym_packing:
		/* Prefer to move from lowest priority CPU's work */
		ਵਾਪਸ sched_asym_prefer(READ_ONCE(sds->busiest->asym_prefer_cpu),
					 READ_ONCE(sg->asym_prefer_cpu));

	ਹਾਲਤ ਸਮੂਹ_misfit_task:
		/*
		 * If we have more than one misfit sg go with the biggest
		 * misfit.
		 */
		ਵਾਪਸ sgs->ਸਮੂਹ_misfit_task_ਲੱਦੋ > busiest->ਸਮੂਹ_misfit_task_ਲੱਦੋ;

	ਹਾਲਤ ਸਮੂਹ_smt_balance:
		/*
		 * Check if we have spare CPUs on either SMT group to
		 * choose has spare or fully busy handling.
		 */
		ਜੇ (sgs->idle_cpus != 0 || busiest->idle_cpus != 0)
			ਜਾਓ has_ਵਾਧੂ;

		fallthrough;

	ਹਾਲਤ ਸਮੂਹ_fully_busy:
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

		ਜੇ (sgs->avg_ਲੱਦੋ < busiest->avg_ਲੱਦੋ)
			ਵਾਪਸ false;

		ਜੇ (sgs->avg_ਲੱਦੋ == busiest->avg_ਲੱਦੋ) {
			/*
			 * SMT sched groups need more help than non-SMT groups.
			 * If @sg happens to also be SMT, either choice is good.
			 */
			ਜੇ (sds->busiest->flags & SD_SHARE_CPUCAPACITY)
				ਵਾਪਸ false;
		}

		ਰੋਕੋ;

	ਹਾਲਤ ਸਮੂਹ_has_ਵਾਧੂ:
		/*
		 * Do not pick sg with SMT CPUs over sg with pure CPUs,
		 * as we do not want to pull task off SMT core with one task
		 * and make the core idle.
		 */
		ਜੇ (smt_vs_nonsmt_groups(sds->busiest, sg)) {
			ਜੇ (sg->flags & SD_SHARE_CPUCAPACITY && sgs->ਜੋੜ_h_nr_running <= 1)
				ਵਾਪਸ false;
			ਨਹੀਂਤਾਂ
				ਵਾਪਸ true;
		}
has_ਵਾਧੂ:

		/*
		 * Select not overloaded group with lowest number of idle CPUs
		 * and highest number of running tasks. We could also compare
		 * the spare capacity which is more stable but it can end up
		 * that the group has less spare capacity but finally more idle
		 * CPUs which means less opportunity to pull tasks.
		 */
		ਜੇ (sgs->idle_cpus > busiest->idle_cpus)
			ਵਾਪਸ false;
		ਨਹੀਂਤਾਂ ਜੇ ((sgs->idle_cpus == busiest->idle_cpus) &&
			 (sgs->ਜੋੜ_nr_running <= busiest->ਜੋੜ_nr_running))
			ਵਾਪਸ false;

		ਰੋਕੋ;
	}

	ਵਾਪਸ true;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_NUMA_BALANCING
ਸਥਿਰ ਸਤਰਅੰਦਰ ਗਿਣਤੀ fbq_type fbq_classify_ਸਮੂਹ(ਬਣਤਰ sg_lb_stats *sgs)
{
	ਜੇ (sgs->ਜੋੜ_h_nr_running > sgs->nr_numa_running)
		ਵਾਪਸ regular;
	ਜੇ (sgs->ਜੋੜ_h_nr_running > sgs->nr_preferred_running)
		ਵਾਪਸ remote;
	ਵਾਪਸ all;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਗਿਣਤੀ fbq_type fbq_classify_rq(ਬਣਤਰ rq *rq)
{
	ਜੇ (rq->nr_running > rq->nr_numa_running)
		ਵਾਪਸ regular;
	ਜੇ (rq->nr_running > rq->nr_preferred_running)
		ਵਾਪਸ remote;
	ਵਾਪਸ all;
}
#ਨਹੀਂਤਾਂ /* !CONFIG_NUMA_BALANCING: */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਗਿਣਤੀ fbq_type fbq_classify_ਸਮੂਹ(ਬਣਤਰ sg_lb_stats *sgs)
{
	ਵਾਪਸ all;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਗਿਣਤੀ fbq_type fbq_classify_rq(ਬਣਤਰ rq *rq)
{
	ਵਾਪਸ regular;
}
#ਜੇ_ਅੰਤ /* !CONFIG_NUMA_BALANCING */


ਬਣਤਰ sg_lb_stats;

/*
 * task_running_on_cpu - return 1 if @p is running on @cpu.
 */

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ task_running_on_cpu(ਪੂਰਨ cpu, ਬਣਤਰ task_struct *p)
{
	/* Task has no contribution or is new */
	ਜੇ (cpu != task_cpu(p) || !READ_ONCE(p->se.avg.ਆਖਰੀ_update_ਸਮਾਂ))
		ਵਾਪਸ 0;

	ਜੇ (task_on_rq_queued(p))
		ਵਾਪਸ 1;

	ਵਾਪਸ 0;
}

/**
 * idle_cpu_without - would a given CPU be idle without p ?
 * @cpu: the processor on which idleness is tested.
 * @p: task which should be ignored.
 *
 * Return: 1 if the CPU would be idle. 0 otherwise.
 */
ਸਥਿਰ ਪੂਰਨ idle_cpu_without(ਪੂਰਨ cpu, ਬਣਤਰ task_struct *p)
{
	ਬਣਤਰ rq *rq = cpu_rq(cpu);

	ਜੇ (rq->curr != rq->idle && rq->curr != p)
		ਵਾਪਸ 0;

	/*
	 * rq->nr_running can't be used but an updated version without the
	 * impact of p on cpu must be used instead. The updated nr_running
	 * be computed and tested before calling idle_cpu_without().
	 */

	ਜੇ (rq->ttwu_pending)
		ਵਾਪਸ 0;

	ਵਾਪਸ 1;
}

/*
 * update_sg_wakeup_stats - Update sched_group's statistics for wakeup.
 * @sd: The sched_domain level to look for idlest group.
 * @group: sched_group whose statistics are to be updated.
 * @sgs: variable to hold the statistics for this group.
 * @p: The task for which we look for the idlest group/CPU.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ update_sg_wakeup_stats(ਬਣਤਰ sched_domain *sd,
					  ਬਣਤਰ sched_ਸਮੂਹ *ਸਮੂਹ,
					  ਬਣਤਰ sg_lb_stats *sgs,
					  ਬਣਤਰ task_struct *p)
{
	ਪੂਰਨ i, nr_running;

	memset(sgs, 0, ਆਕਾਰ(*sgs));

	/* Assume that task can't fit any CPU of the group */
	ਜੇ (sd->flags & SD_ASYM_CPUCAPACITY)
		sgs->ਸਮੂਹ_misfit_task_ਲੱਦੋ = 1;

	for_each_cpu_and(i, sched_ਸਮੂਹ_ਵਿਸਤਾਰ(ਸਮੂਹ), p->cpus_ptr) {
		ਬਣਤਰ rq *rq = cpu_rq(i);
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ local;

		sgs->ਸਮੂਹ_ਲੱਦੋ += cpu_ਲੱਦੋ_without(rq, p);
		sgs->ਸਮੂਹ_util += cpu_util_without(i, p);
		sgs->ਸਮੂਹ_runnable += cpu_runnable_without(rq, p);
		local = task_running_on_cpu(i, p);
		sgs->ਜੋੜ_h_nr_running += rq->cfs.h_nr_runnable - local;

		nr_running = rq->nr_running - local;
		sgs->ਜੋੜ_nr_running += nr_running;

		/*
		 * No need to call idle_cpu_without() if nr_running is not 0
		 */
		ਜੇ (!nr_running && idle_cpu_without(i, p))
			sgs->idle_cpus++;

		/* Check if task fits in the CPU */
		ਜੇ (sd->flags & SD_ASYM_CPUCAPACITY &&
		    sgs->ਸਮੂਹ_misfit_task_ਲੱਦੋ &&
		    task_ਸਮਾਉਂਦਾ_cpu(p, i))
			sgs->ਸਮੂਹ_misfit_task_ਲੱਦੋ = 0;

	}

	sgs->ਸਮੂਹ_ਸਮਰੱਥਾ = ਸਮੂਹ->sgc->ਸਮਰੱਥਾ;

	sgs->ਸਮੂਹ_ਭਾਰ = ਸਮੂਹ->ਸਮੂਹ_ਭਾਰ;

	sgs->ਸਮੂਹ_type = ਸਮੂਹ_classify(sd->imbalance_pct, ਸਮੂਹ, sgs);

	/*
	 * Computing avg_load makes sense only when group is fully busy or
	 * overloaded
	 */
	ਜੇ (sgs->ਸਮੂਹ_type == ਸਮੂਹ_fully_busy ||
		sgs->ਸਮੂਹ_type == ਸਮੂਹ_overloaded)
		sgs->avg_ਲੱਦੋ = (sgs->ਸਮੂਹ_ਲੱਦੋ * SCHED_ਸਮਰੱਥਾ_SCALE) /
				sgs->ਸਮੂਹ_ਸਮਰੱਥਾ;
}

ਸਥਿਰ bool update_pick_idlest(ਬਣਤਰ sched_ਸਮੂਹ *idlest,
			       ਬਣਤਰ sg_lb_stats *idlest_sgs,
			       ਬਣਤਰ sched_ਸਮੂਹ *ਸਮੂਹ,
			       ਬਣਤਰ sg_lb_stats *sgs)
{
	ਜੇ (sgs->ਸਮੂਹ_type < idlest_sgs->ਸਮੂਹ_type)
		ਵਾਪਸ true;

	ਜੇ (sgs->ਸਮੂਹ_type > idlest_sgs->ਸਮੂਹ_type)
		ਵਾਪਸ false;

	/*
	 * The candidate and the current idlest group are the same type of
	 * group. Let check which one is the idlest according to the type.
	 */

	ਚੋਣ (sgs->ਸਮੂਹ_type) {
	ਹਾਲਤ ਸਮੂਹ_overloaded:
	ਹਾਲਤ ਸਮੂਹ_fully_busy:
		/* Select the group with lowest avg_load. */
		ਜੇ (idlest_sgs->avg_ਲੱਦੋ <= sgs->avg_ਲੱਦੋ)
			ਵਾਪਸ false;
		ਰੋਕੋ;

	ਹਾਲਤ ਸਮੂਹ_llc_balance:
	ਹਾਲਤ ਸਮੂਹ_imbalanced:
	ਹਾਲਤ ਸਮੂਹ_asym_packing:
	ਹਾਲਤ ਸਮੂਹ_smt_balance:
		/* Those types are not used in the slow wakeup path */
		ਵਾਪਸ false;

	ਹਾਲਤ ਸਮੂਹ_misfit_task:
		/* Select group with the highest max capacity */
		ਜੇ (idlest->sgc->max_ਸਮਰੱਥਾ >= ਸਮੂਹ->sgc->max_ਸਮਰੱਥਾ)
			ਵਾਪਸ false;
		ਰੋਕੋ;

	ਹਾਲਤ ਸਮੂਹ_has_ਵਾਧੂ:
		/* Select group with most idle CPUs */
		ਜੇ (idlest_sgs->idle_cpus > sgs->idle_cpus)
			ਵਾਪਸ false;

		/* Select group with lowest group_util */
		ਜੇ (idlest_sgs->idle_cpus == sgs->idle_cpus &&
			idlest_sgs->ਸਮੂਹ_util <= sgs->ਸਮੂਹ_util)
			ਵਾਪਸ false;

		ਰੋਕੋ;
	}

	ਵਾਪਸ true;
}

/*
 * sched_balance_find_dst_group() finds and returns the least busy CPU group within the
 * domain.
 *
 * Assumes p is allowed on at least one CPU in sd.
 */
ਸਥਿਰ ਬਣਤਰ sched_ਸਮੂਹ *
sched_balance_ਲੱਭੋ_dst_ਸਮੂਹ(ਬਣਤਰ sched_domain *sd, ਬਣਤਰ task_struct *p, ਪੂਰਨ this_cpu)
{
	ਬਣਤਰ sched_ਸਮੂਹ *idlest = ਨਿਰਾ, *local = ਨਿਰਾ, *ਸਮੂਹ = sd->groups;
	ਬਣਤਰ sg_lb_stats local_sgs, tmp_sgs;
	ਬਣਤਰ sg_lb_stats *sgs;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ imbalance;
	ਬਣਤਰ sg_lb_stats idlest_sgs = {
			.avg_ਲੱਦੋ = UINT_MAX,
			.ਸਮੂਹ_type = ਸਮੂਹ_overloaded,
	};

	ਕਰੋ {
		ਪੂਰਨ local_ਸਮੂਹ;

		/* Skip over this group if it has no CPUs allowed */
		ਜੇ (!cpumask_intersects(sched_ਸਮੂਹ_ਵਿਸਤਾਰ(ਸਮੂਹ),
					p->cpus_ptr))
			ਜਾਰੀ;

		/* Skip over this group if no cookie matched */
		ਜੇ (!sched_ਸਮੂਹ_cookie_ਮੇਲ(cpu_rq(this_cpu), p, ਸਮੂਹ))
			ਜਾਰੀ;

		local_ਸਮੂਹ = cpumask_test_cpu(this_cpu,
					       sched_ਸਮੂਹ_ਵਿਸਤਾਰ(ਸਮੂਹ));

		ਜੇ (local_ਸਮੂਹ) {
			sgs = &local_sgs;
			local = ਸਮੂਹ;
		} ਨਹੀਂਤਾਂ {
			sgs = &tmp_sgs;
		}

		update_sg_wakeup_stats(sd, ਸਮੂਹ, sgs, p);

		ਜੇ (!local_ਸਮੂਹ && update_pick_idlest(idlest, &idlest_sgs, ਸਮੂਹ, sgs)) {
			idlest = ਸਮੂਹ;
			idlest_sgs = *sgs;
		}

	} ਜਦੋਂ (ਸਮੂਹ = ਸਮੂਹ->ਅਗਲਾ, ਸਮੂਹ != sd->groups);


	/* There is no idlest group to push tasks to */
	ਜੇ (!idlest)
		ਵਾਪਸ ਨਿਰਾ;

	/* The local group has been skipped because of CPU affinity */
	ਜੇ (!local)
		ਵਾਪਸ idlest;

	/*
	 * If the local group is idler than the selected idlest group
	 * don't try and push the task.
	 */
	ਜੇ (local_sgs.ਸਮੂਹ_type < idlest_sgs.ਸਮੂਹ_type)
		ਵਾਪਸ ਨਿਰਾ;

	/*
	 * If the local group is busier than the selected idlest group
	 * try and push the task.
	 */
	ਜੇ (local_sgs.ਸਮੂਹ_type > idlest_sgs.ਸਮੂਹ_type)
		ਵਾਪਸ idlest;

	ਚੋਣ (local_sgs.ਸਮੂਹ_type) {
	ਹਾਲਤ ਸਮੂਹ_overloaded:
	ਹਾਲਤ ਸਮੂਹ_fully_busy:

		/* Calculate allowed imbalance based on load */
		imbalance = scale_ਲੱਦੋ_ਥੱਲੇ(NICE_0_ਲੱਦੋ) *
				(sd->imbalance_pct-100) / 100;

		/*
		 * When comparing groups across NUMA domains, it's possible for
		 * the local domain to be very lightly loaded relative to the
		 * remote domains but "imbalance" skews the comparison making
		 * remote CPUs look much more favourable. When considering
		 * cross-domain, add imbalance to the load on the remote node
		 * and consider staying local.
		 */

		ਜੇ ((sd->flags & SD_NUMA) &&
		    ((idlest_sgs.avg_ਲੱਦੋ + imbalance) >= local_sgs.avg_ਲੱਦੋ))
			ਵਾਪਸ ਨਿਰਾ;

		/*
		 * If the local group is less loaded than the selected
		 * idlest group don't try and push any tasks.
		 */
		ਜੇ (idlest_sgs.avg_ਲੱਦੋ >= (local_sgs.avg_ਲੱਦੋ + imbalance))
			ਵਾਪਸ ਨਿਰਾ;

		ਜੇ (100 * local_sgs.avg_ਲੱਦੋ <= sd->imbalance_pct * idlest_sgs.avg_ਲੱਦੋ)
			ਵਾਪਸ ਨਿਰਾ;
		ਰੋਕੋ;

	ਹਾਲਤ ਸਮੂਹ_llc_balance:
	ਹਾਲਤ ਸਮੂਹ_imbalanced:
	ਹਾਲਤ ਸਮੂਹ_asym_packing:
	ਹਾਲਤ ਸਮੂਹ_smt_balance:
		/* Those type are not used in the slow wakeup path */
		ਵਾਪਸ ਨਿਰਾ;

	ਹਾਲਤ ਸਮੂਹ_misfit_task:
		/* Select group with the highest max capacity */
		ਜੇ (local->sgc->max_ਸਮਰੱਥਾ >= idlest->sgc->max_ਸਮਰੱਥਾ)
			ਵਾਪਸ ਨਿਰਾ;
		ਰੋਕੋ;

	ਹਾਲਤ ਸਮੂਹ_has_ਵਾਧੂ:
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_NUMA
		ਜੇ (sd->flags & SD_NUMA) {
			ਪੂਰਨ imb_numa_nr = sd->imb_numa_nr;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_NUMA_BALANCING
			ਪੂਰਨ idlest_cpu;
			/*
			 * If there is spare capacity at NUMA, try to select
			 * the preferred node
			 */
			ਜੇ (cpu_ਮੰਜ਼ਿਲ_ਗੰਢ(this_cpu) == p->numa_preferred_nid)
				ਵਾਪਸ ਨਿਰਾ;

			idlest_cpu = cpumask_ਪਹਿਲਾ(sched_ਸਮੂਹ_ਵਿਸਤਾਰ(idlest));
			ਜੇ (cpu_ਮੰਜ਼ਿਲ_ਗੰਢ(idlest_cpu) == p->numa_preferred_nid)
				ਵਾਪਸ idlest;
#ਜੇ_ਅੰਤ /* CONFIG_NUMA_BALANCING */
			/*
			 * Otherwise, keep the task close to the wakeup source
			 * and improve locality if the number of running tasks
			 * would remain below threshold where an imbalance is
			 * allowed while accounting for the possibility the
			 * task is pinned to a subset of CPUs. If there is a
			 * real need of migration, periodic load balance will
			 * take care of it.
			 */
			ਜੇ (p->nr_cpus_allowed != NR_CPUS) {
				ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ w = cpumask_ਭਾਰ_and(p->cpus_ptr,
								sched_ਸਮੂਹ_ਵਿਸਤਾਰ(local));
				imb_numa_nr = min(w, sd->imb_numa_nr);
			}

			imbalance = ਨਿਰਪੇਖ(local_sgs.idle_cpus - idlest_sgs.idle_cpus);
			ਜੇ (!adjust_numa_imbalance(imbalance,
						   local_sgs.ਜੋੜ_nr_running + 1,
						   imb_numa_nr)) {
				ਵਾਪਸ ਨਿਰਾ;
			}
		}
#ਜੇ_ਅੰਤ /* CONFIG_NUMA */

		/*
		 * Select group with highest number of idle CPUs. We could also
		 * compare the utilization which is more stable but it can end
		 * up that the group has less spare capacity but finally more
		 * idle CPUs which means more opportunity to run task.
		 */
		ਜੇ (local_sgs.idle_cpus >= idlest_sgs.idle_cpus)
			ਵਾਪਸ ਨਿਰਾ;
		ਰੋਕੋ;
	}

	ਵਾਪਸ idlest;
}

ਸਥਿਰ ਖਾਲੀ update_idle_cpu_ਜਾਂਚ(ਬਣਤਰ lb_env *env,
				 ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਜੋੜ_util)
{
	ਬਣਤਰ sched_domain_shared *sd_share;
	ਬਣਤਰ sched_domain *sd = env->sd;
	ਪੂਰਨ llc_ਭਾਰ, pct;
	u64 x, y, tmp;
	/*
	 * Update the number of CPUs to scan in LLC domain, which could
	 * be used as a hint in select_idle_cpu(). The update of sd_share
	 * could be expensive because it is within a shared cache line.
	 * So the write of this hint only occurs during periodic load
	 * balancing, rather than CPU_NEWLY_IDLE, because the latter
	 * can fire way more frequently than the former.
	 */
	ਜੇ (!sched_feat(SIS_UTIL) || env->idle == CPU_NEWLY_IDLE)
		ਵਾਪਸ;

	sd_share = sd->shared;
	ਜੇ (!sd_share)
		ਵਾਪਸ;

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
	x = ਜੋੜ_util;
	llc_ਭਾਰ = sd->ਵਿਸਤਾਰ_ਭਾਰ;
	do_div(x, llc_ਭਾਰ);

	/* equation [4] */
	pct = sd->imbalance_pct;
	tmp = x * x * pct * pct;
	do_div(tmp, 10000 * SCHED_ਸਮਰੱਥਾ_SCALE);
	tmp = min_t(ਲੰਮਾ, tmp, SCHED_ਸਮਰੱਥਾ_SCALE);
	y = SCHED_ਸਮਰੱਥਾ_SCALE - tmp;

	/* equation [2] */
	y *= llc_ਭਾਰ;
	do_div(y, SCHED_ਸਮਰੱਥਾ_SCALE);
	ਜੇ ((ਪੂਰਨ)y != sd_share->nr_idle_ਜਾਂਚ)
		WRITE_ONCE(sd_share->nr_idle_ਜਾਂਚ, (ਪੂਰਨ)y);
}

/**
 * update_sd_lb_stats - Update sched_domain's statistics for load balancing.
 * @env: The load balancing environment.
 * @sds: variable to hold the statistics for this sched_domain.
 */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ update_sd_lb_stats(ਬਣਤਰ lb_env *env, ਬਣਤਰ sd_lb_stats *sds)
{
	ਬਣਤਰ sched_ਸਮੂਹ *sg = env->sd->groups;
	ਬਣਤਰ sg_lb_stats *local = &sds->local_stat;
	ਬਣਤਰ sg_lb_stats tmp_sgs;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਜੋੜ_util = 0;
	bool sg_overloaded = 0, sg_overutilized = 0;

	env->dst_core_idle = !sched_smt_ਸਰਗਰਮ() || is_core_idle(env->dst_cpu);

	ਕਰੋ {
		ਬਣਤਰ sg_lb_stats *sgs = &tmp_sgs;
		ਪੂਰਨ local_ਸਮੂਹ;

		local_ਸਮੂਹ = cpumask_test_cpu(env->dst_cpu, sched_ਸਮੂਹ_ਵਿਸਤਾਰ(sg));
		ਜੇ (local_ਸਮੂਹ) {
			sds->local = sg;
			sgs = local;

			ਜੇ (env->idle != CPU_NEWLY_IDLE ||
			    ਸਮਾਂ_after_eq(jiffies, sg->sgc->ਅਗਲਾ_update))
				update_ਸਮੂਹ_ਸਮਰੱਥਾ(env->sd, env->dst_cpu);
		}

		update_sg_lb_stats(env, sds, sg, sgs, &sg_overloaded);

		ਜੇ (!local_ਸਮੂਹ && update_sd_pick_busiest(env, sds, sg, sgs)) {
			sds->busiest = sg;
			sds->busiest_stat = *sgs;
		}

		sg_overutilized |= sgs->ਸਮੂਹ_overutilized;

		/* Now, start updating sd_lb_stats */
		sds->ਕੁੱਲ_ਲੱਦੋ += sgs->ਸਮੂਹ_ਲੱਦੋ;
		sds->ਕੁੱਲ_ਸਮਰੱਥਾ += sgs->ਸਮੂਹ_ਸਮਰੱਥਾ;

		ਜੋੜ_util += sgs->ਸਮੂਹ_util;
		sg = sg->ਅਗਲਾ;
	} ਜਦੋਂ (sg != env->sd->groups);

	/*
	 * Indicate that the child domain of the busiest group prefers tasks
	 * go to a child's sibling domains first. NB the flags of a sched group
	 * are those of the child domain.
	 */
	ਜੇ (sds->busiest)
		sds->prefer_sibling = !!(sds->busiest->flags & SD_PREFER_SIBLING);


	ਜੇ (env->sd->flags & SD_NUMA)
		env->fbq_type = fbq_classify_ਸਮੂਹ(&sds->busiest_stat);

	ਜੇ (!env->sd->ਮਾਪਾ) {
		/* update overload indicator if we are at root domain */
		ਸੈੱਟ_rd_overloaded(env->dst_rq->rd, sg_overloaded);

		/* Update over-utilization (tipping point, U >= 0) indicator */
		ਸੈੱਟ_rd_overutilized(env->dst_rq->rd, sg_overutilized);
	} ਨਹੀਂਤਾਂ ਜੇ (sg_overutilized) {
		ਸੈੱਟ_rd_overutilized(env->dst_rq->rd, sg_overutilized);
	}

	update_idle_cpu_ਜਾਂਚ(env, ਜੋੜ_util);
}

/**
 * calculate_imbalance - Calculate the amount of imbalance present within the
 *			 groups of a given sched_domain during load balance.
 * @env: load balance environment
 * @sds: statistics of the sched_domain whose imbalance is to be calculated.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ calculate_imbalance(ਬਣਤਰ lb_env *env, ਬਣਤਰ sd_lb_stats *sds)
{
	ਬਣਤਰ sg_lb_stats *local, *busiest;

	local = &sds->local_stat;
	busiest = &sds->busiest_stat;

	ਜੇ (busiest->ਸਮੂਹ_type == ਸਮੂਹ_misfit_task) {
		ਜੇ (env->sd->flags & SD_ASYM_CPUCAPACITY) {
			/* Set imbalance to allow misfit tasks to be balanced. */
			env->migration_type = migrate_misfit;
			env->imbalance = 1;
		} ਨਹੀਂਤਾਂ {
			/*
			 * Set load imbalance to allow moving task from cpu
			 * with reduced capacity.
			 */
			env->migration_type = migrate_ਲੱਦੋ;
			env->imbalance = busiest->ਸਮੂਹ_misfit_task_ਲੱਦੋ;
		}
		ਵਾਪਸ;
	}

	ਜੇ (busiest->ਸਮੂਹ_type == ਸਮੂਹ_asym_packing) {
		/*
		 * In case of asym capacity, we will try to migrate all load to
		 * the preferred CPU.
		 */
		env->migration_type = migrate_task;
		env->imbalance = busiest->ਜੋੜ_h_nr_running;
		ਵਾਪਸ;
	}

	ਜੇ (busiest->ਸਮੂਹ_type == ਸਮੂਹ_smt_balance) {
		/* Reduce number of tasks sharing CPU capacity */
		env->migration_type = migrate_task;
		env->imbalance = 1;
		ਵਾਪਸ;
	}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CACHE
	ਜੇ (busiest->ਸਮੂਹ_type == ਸਮੂਹ_llc_balance) {
		/* Move a task that prefer local LLC */
		env->migration_type = migrate_llc_task;
		env->imbalance = 1;
		ਵਾਪਸ;
	}
#ਜੇ_ਅੰਤ

	ਜੇ (busiest->ਸਮੂਹ_type == ਸਮੂਹ_imbalanced) {
		/*
		 * In the group_imb case we cannot rely on group-wide averages
		 * to ensure CPU-load equilibrium, try to move any task to fix
		 * the imbalance. The next load balance will take care of
		 * balancing back the system.
		 */
		env->migration_type = migrate_task;
		env->imbalance = 1;
		ਵਾਪਸ;
	}

	/*
	 * Try to use spare capacity of local group without overloading it or
	 * emptying busiest.
	 */
	ਜੇ (local->ਸਮੂਹ_type == ਸਮੂਹ_has_ਵਾਧੂ) {
		ਜੇ ((busiest->ਸਮੂਹ_type > ਸਮੂਹ_fully_busy) &&
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
			env->imbalance = max(local->ਸਮੂਹ_ਸਮਰੱਥਾ, local->ਸਮੂਹ_util) -
					 local->ਸਮੂਹ_util;

			/*
			 * In some cases, the group's utilization is max or even
			 * higher than capacity because of migrations but the
			 * local CPU is (newly) idle. There is at least one
			 * waiting task in this overloaded busiest group. Let's
			 * try to pull it.
			 */
			ਜੇ (env->idle && env->imbalance == 0) {
				env->migration_type = migrate_task;
				env->imbalance = 1;
			}

			ਵਾਪਸ;
		}

		ਜੇ (busiest->ਸਮੂਹ_ਭਾਰ == 1 || sds->prefer_sibling) {
			/*
			 * When prefer sibling, evenly spread running tasks on
			 * groups.
			 */
			env->migration_type = migrate_task;
			env->imbalance = sibling_imbalance(env, sds, busiest, local);
		} ਨਹੀਂਤਾਂ {

			/*
			 * If there is no overload, we just want to even the number of
			 * idle CPUs.
			 */
			env->migration_type = migrate_task;
			env->imbalance = max_t(ਲੰਮਾ, 0,
					       (local->idle_cpus - busiest->idle_cpus));
		}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_NUMA
		/* Consider allowing a small imbalance between NUMA groups */
		ਜੇ (env->sd->flags & SD_NUMA) {
			env->imbalance = adjust_numa_imbalance(env->imbalance,
							       local->ਜੋੜ_nr_running + 1,
							       env->sd->imb_numa_nr);
		}
#ਜੇ_ਅੰਤ

		/* Number of tasks to move to restore balance */
		env->imbalance >>= 1;

		ਵਾਪਸ;
	}

	/*
	 * Local is fully busy but has to take more load to relieve the
	 * busiest group
	 */
	ਜੇ (local->ਸਮੂਹ_type < ਸਮੂਹ_overloaded) {
		/*
		 * Local will become overloaded so the avg_load metrics are
		 * finally needed.
		 */

		local->avg_ਲੱਦੋ = (local->ਸਮੂਹ_ਲੱਦੋ * SCHED_ਸਮਰੱਥਾ_SCALE) /
				  local->ਸਮੂਹ_ਸਮਰੱਥਾ;

		/*
		 * If the local group is more loaded than the selected
		 * busiest group don't try to pull any tasks.
		 */
		ਜੇ (local->avg_ਲੱਦੋ >= busiest->avg_ਲੱਦੋ) {
			env->imbalance = 0;
			ਵਾਪਸ;
		}

		sds->avg_ਲੱਦੋ = (sds->ਕੁੱਲ_ਲੱਦੋ * SCHED_ਸਮਰੱਥਾ_SCALE) /
				sds->ਕੁੱਲ_ਸਮਰੱਥਾ;

		/*
		 * If the local group is more loaded than the average system
		 * load, don't try to pull any tasks.
		 */
		ਜੇ (local->avg_ਲੱਦੋ >= sds->avg_ਲੱਦੋ) {
			env->imbalance = 0;
			ਵਾਪਸ;
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
	env->migration_type = migrate_ਲੱਦੋ;
	env->imbalance = min(
		(busiest->avg_ਲੱਦੋ - sds->avg_ਲੱਦੋ) * busiest->ਸਮੂਹ_ਸਮਰੱਥਾ,
		(sds->avg_ਲੱਦੋ - local->avg_ਲੱਦੋ) * local->ਸਮੂਹ_ਸਮਰੱਥਾ
	) / SCHED_ਸਮਰੱਥਾ_SCALE;
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
ਸਥਿਰ ਬਣਤਰ sched_ਸਮੂਹ *sched_balance_ਲੱਭੋ_src_ਸਮੂਹ(ਬਣਤਰ lb_env *env)
{
	ਬਣਤਰ sg_lb_stats *local, *busiest;
	ਬਣਤਰ sd_lb_stats sds;

	init_sd_lb_stats(&sds);

	/*
	 * Compute the various statistics relevant for load balancing at
	 * this level.
	 */
	update_sd_lb_stats(env, &sds);

	/* There is no busy sibling group to pull tasks from */
	ਜੇ (!sds.busiest)
		ਜਾਓ ਬਾਹਰਲਾ_ਸੰਤੁਲਿਤ;

	busiest = &sds.busiest_stat;

	/* Misfit tasks should be dealt with regardless of the avg load */
	ਜੇ (busiest->ਸਮੂਹ_type == ਸਮੂਹ_misfit_task)
		ਜਾਓ force_balance;

	ਜੇ (!is_rd_overutilized(env->dst_rq->rd) &&
	    rcu_dereference_all(env->dst_rq->rd->pd))
		ਜਾਓ ਬਾਹਰਲਾ_ਸੰਤੁਲਿਤ;

	/* ASYM feature bypasses nice load balance check */
	ਜੇ (busiest->ਸਮੂਹ_type == ਸਮੂਹ_asym_packing)
		ਜਾਓ force_balance;

	/*
	 * If the busiest group is imbalanced the below checks don't
	 * work because they assume all things are equal, which typically
	 * isn't true due to cpus_ptr constraints and the like.
	 */
	ਜੇ (busiest->ਸਮੂਹ_type == ਸਮੂਹ_imbalanced)
		ਜਾਓ force_balance;

	local = &sds.local_stat;
	/*
	 * If the local group is busier than the selected busiest group
	 * don't try and pull any tasks.
	 */
	ਜੇ (local->ਸਮੂਹ_type > busiest->ਸਮੂਹ_type)
		ਜਾਓ ਬਾਹਰਲਾ_ਸੰਤੁਲਿਤ;

	/*
	 * When groups are overloaded, use the avg_load to ensure fairness
	 * between tasks.
	 */
	ਜੇ (local->ਸਮੂਹ_type == ਸਮੂਹ_overloaded) {
		/*
		 * If the local group is more loaded than the selected
		 * busiest group don't try to pull any tasks.
		 */
		ਜੇ (local->avg_ਲੱਦੋ >= busiest->avg_ਲੱਦੋ)
			ਜਾਓ ਬਾਹਰਲਾ_ਸੰਤੁਲਿਤ;

		/* XXX broken for overlapping NUMA groups */
		sds.avg_ਲੱਦੋ = (sds.ਕੁੱਲ_ਲੱਦੋ * SCHED_ਸਮਰੱਥਾ_SCALE) /
				sds.ਕੁੱਲ_ਸਮਰੱਥਾ;

		/*
		 * Don't pull any tasks if this group is already above the
		 * domain average load.
		 */
		ਜੇ (local->avg_ਲੱਦੋ >= sds.avg_ਲੱਦੋ)
			ਜਾਓ ਬਾਹਰਲਾ_ਸੰਤੁਲਿਤ;

		/*
		 * If the busiest group is more loaded, use imbalance_pct to be
		 * conservative.
		 */
		ਜੇ (100 * busiest->avg_ਲੱਦੋ <=
				env->sd->imbalance_pct * local->avg_ਲੱਦੋ)
			ਜਾਓ ਬਾਹਰਲਾ_ਸੰਤੁਲਿਤ;
	}

	/*
	 * Try to move all excess tasks to a sibling domain of the busiest
	 * group's child domain.
	 */
	ਜੇ (sds.prefer_sibling && local->ਸਮੂਹ_type == ਸਮੂਹ_has_ਵਾਧੂ &&
	    (busiest->ਸਮੂਹ_type == ਸਮੂਹ_llc_balance ||
	    sibling_imbalance(env, &sds, busiest, local) > 1))
		ਜਾਓ force_balance;

	ਜੇ (busiest->ਸਮੂਹ_type != ਸਮੂਹ_overloaded) {
		ਜੇ (!env->idle) {
			/*
			 * If the busiest group is not overloaded (and as a
			 * result the local one too) but this CPU is already
			 * busy, let another idle CPU try to pull task.
			 */
			ਜਾਓ ਬਾਹਰਲਾ_ਸੰਤੁਲਿਤ;
		}

		ਜੇ (busiest->ਸਮੂਹ_type == ਸਮੂਹ_smt_balance &&
		    smt_vs_nonsmt_groups(sds.local, sds.busiest)) {
			/* Let non SMT CPU pull from SMT CPU sharing with sibling */
			ਜਾਓ force_balance;
		}

		ਜੇ (busiest->ਸਮੂਹ_ਭਾਰ > 1 &&
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
			ਜਾਓ ਬਾਹਰਲਾ_ਸੰਤੁਲਿਤ;
		}

		ਜੇ (busiest->ਜੋੜ_h_nr_running == 1) {
			/*
			 * busiest doesn't have any tasks waiting to run
			 */
			ਜਾਓ ਬਾਹਰਲਾ_ਸੰਤੁਲਿਤ;
		}
	}

force_balance:
	/* Looks like there is an imbalance. Compute it */
	calculate_imbalance(env, &sds);
	ਵਾਪਸ env->imbalance ? sds.busiest : ਨਿਰਾ;

ਬਾਹਰਲਾ_ਸੰਤੁਲਿਤ:
	env->imbalance = 0;
	ਵਾਪਸ ਨਿਰਾ;
}

/*
 * sched_balance_find_src_rq - find the busiest runqueue among the CPUs in the group.
 */
ਸਥਿਰ ਬਣਤਰ rq *sched_balance_ਲੱਭੋ_src_rq(ਬਣਤਰ lb_env *env,
				     ਬਣਤਰ sched_ਸਮੂਹ *ਸਮੂਹ)
{
	ਬਣਤਰ rq *busiest = ਨਿਰਾ, *rq;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ busiest_util = 0, busiest_ਲੱਦੋ = 0, busiest_ਸਮਰੱਥਾ = 1;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ __maybe_unused busiest_pref_llc = 0;
	ਬਣਤਰ sched_domain __maybe_unused *sd_tmp;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ busiest_nr = 0;
	ਪੂਰਨ __maybe_unused dst_llc;
	ਪੂਰਨ i;

	for_each_cpu_and(i, sched_ਸਮੂਹ_ਵਿਸਤਾਰ(ਸਮੂਹ), env->cpus) {
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਸਮਰੱਥਾ, ਲੱਦੋ, util;
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ nr_running;
		ਗਿਣਤੀ fbq_type rt;

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
		ਜੇ (rt > env->fbq_type)
			ਜਾਰੀ;

		nr_running = rq->cfs.h_nr_runnable;
		ਜੇ (!nr_running)
			ਜਾਰੀ;

		ਸਮਰੱਥਾ = ਸਮਰੱਥਾ_ਦਾ(i);

		/*
		 * For ASYM_CPUCAPACITY domains, don't pick a CPU that could
		 * eventually lead to active_balancing high->low capacity.
		 * Higher per-CPU capacity is considered better than balancing
		 * average load.
		 */
		ਜੇ (env->sd->flags & SD_ASYM_CPUCAPACITY &&
		    nr_running == 1) {
			bool cluster_equal_cap = static_branch_unlikely(&sched_cluster_ਸਰਗਰਮ) &&
						 (get_actual_cpu_ਸਮਰੱਥਾ(env->dst_cpu) ==
						  get_actual_cpu_ਸਮਰੱਥਾ(i));
			bool smt_degraded_cap = sched_smt_ਸਰਗਰਮ() && !is_core_idle(i);

			/*
			 * Busy SMT siblings reduce the capacity of CPU @i. Do
			 * not skip it in this case.
			 *
			 * CONFIG_SCHED_CLUSTER requires balancing load across
			 * clusters of identical capacity, accounting for
			 * hardware and cpufreq pressure.
			 */
			ਜੇ (!smt_degraded_cap && !cluster_equal_cap &&
			    !ਸਮਰੱਥਾ_greater(ਸਮਰੱਥਾ_ਦਾ(env->dst_cpu), ਸਮਰੱਥਾ))
				ਜਾਰੀ;
		}

		/*
		 * Make sure we only pull tasks from a CPU of lower priority
		 * when balancing between SMT siblings.
		 *
		 * If balancing between cores, let lower priority CPUs help
		 * SMT cores with more than one busy sibling.
		 */
		ਜੇ (sched_asym(env->sd, i, env->dst_cpu) && nr_running == 1)
			ਜਾਰੀ;

		ਚੋਣ (env->migration_type) {
		ਹਾਲਤ migrate_ਲੱਦੋ:
			/*
			 * When comparing with load imbalance, use cpu_load()
			 * which is not scaled with the CPU capacity.
			 */
			ਲੱਦੋ = cpu_ਲੱਦੋ(rq);

			ਜੇ (nr_running == 1 && ਲੱਦੋ > env->imbalance &&
			    !check_cpu_ਸਮਰੱਥਾ(rq, env->sd))
				ਰੋਕੋ;

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
			ਜੇ (ਲੱਦੋ * busiest_ਸਮਰੱਥਾ > busiest_ਲੱਦੋ * ਸਮਰੱਥਾ) {
				busiest_ਲੱਦੋ = ਲੱਦੋ;
				busiest_ਸਮਰੱਥਾ = ਸਮਰੱਥਾ;
				busiest = rq;
			}
			ਰੋਕੋ;

		ਹਾਲਤ migrate_util:
			util = cpu_util_cfs_boost(i);

			/*
			 * Don't try to pull utilization from a CPU with one
			 * running task. Whatever its utilization, we will fail
			 * detach the task.
			 */
			ਜੇ (nr_running <= 1)
				ਜਾਰੀ;

			ਜੇ (busiest_util < util) {
				busiest_util = util;
				busiest = rq;
			}
			ਰੋਕੋ;

		ਹਾਲਤ migrate_task:
			ਜੇ (busiest_nr < nr_running) {
				busiest_nr = nr_running;
				busiest = rq;
			}
			ਰੋਕੋ;

		ਹਾਲਤ migrate_misfit:
			/*
			 * For ASYM_CPUCAPACITY domains with misfit tasks we
			 * simply seek the "biggest" misfit task.
			 */
			ਜੇ (rq->misfit_task_ਲੱਦੋ > busiest_ਲੱਦੋ) {
				busiest_ਲੱਦੋ = rq->misfit_task_ਲੱਦੋ;
				busiest = rq;
			}

			ਰੋਕੋ;

		ਹਾਲਤ migrate_llc_task:
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CACHE
			sd_tmp = rcu_dereference_all(rq->sd);
			dst_llc = llc_id(env->dst_cpu);

			ਜੇ (sd_tmp && (ਬਿਨਾਨਿਸ਼ਾਨੀ)dst_llc < sd_tmp->llc_max) {
				ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ this_pref_llc =
					sd_tmp->llc_counts[dst_llc];

				ਜੇ (busiest_pref_llc < this_pref_llc) {
					busiest_pref_llc = this_pref_llc;
					busiest = rq;
				}
			}
#ਜੇ_ਅੰਤ
			ਰੋਕੋ;

		}
	}

	ਵਾਪਸ busiest;
}

/*
 * Max backoff if we encounter pinned tasks. Pretty arbitrary value, but
 * so long as it is large enough.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MAX_PINNED_INTERVAL	512

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
asym_ਸਰਗਰਮ_balance(ਬਣਤਰ lb_env *env)
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
	ਵਾਪਸ env->idle && sched_use_asym_prio(env->sd, env->dst_cpu) &&
	       (sched_asym_prefer(env->dst_cpu, env->src_cpu) ||
		!sched_use_asym_prio(env->sd, env->src_cpu));
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
imbalanced_ਸਰਗਰਮ_balance(ਬਣਤਰ lb_env *env)
{
	ਬਣਤਰ sched_domain *sd = env->sd;

	/*
	 * The imbalanced case includes the case of pinned tasks preventing a fair
	 * distribution of the load on the system but also the even distribution of the
	 * threads on a system with spare capacity
	 */
	ਜੇ ((env->migration_type == migrate_task) &&
	    (sd->nr_balance_failed > sd->cache_nice_tries+2))
		ਵਾਪਸ 1;

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ need_ਸਰਗਰਮ_balance(ਬਣਤਰ lb_env *env)
{
	ਬਣਤਰ sched_domain *sd = env->sd;

	ਜੇ (alb_break_llc(env))
		ਵਾਪਸ 0;

	ਜੇ (asym_ਸਰਗਰਮ_balance(env))
		ਵਾਪਸ 1;

	ਜੇ (imbalanced_ਸਰਗਰਮ_balance(env))
		ਵਾਪਸ 1;

	/*
	 * The dst_cpu is idle and the src_cpu CPU has only 1 CFS task.
	 * It's worth migrating the task if the src_cpu's capacity is reduced
	 * because of other sched_class or IRQs if more capacity stays
	 * available on dst_cpu.
	 */
	ਜੇ (env->idle &&
	    (env->src_rq->cfs.h_nr_runnable == 1)) {
		ਜੇ ((check_cpu_ਸਮਰੱਥਾ(env->src_rq, sd)) &&
		    (ਸਮਰੱਥਾ_ਦਾ(env->src_cpu)*sd->imbalance_pct < ਸਮਰੱਥਾ_ਦਾ(env->dst_cpu)*100))
			ਵਾਪਸ 1;
	}

	ਜੇ (env->migration_type == migrate_misfit ||
	    env->migration_type == migrate_llc_task)
		ਵਾਪਸ 1;

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ ਸਰਗਰਮ_ਲੱਦੋ_balance_cpu_stop(ਖਾਲੀ *ਅੰਕੜੇ);
ਸਥਿਰ ਪੂਰਨ ਸਰਗਰਮ_ਲੱਦੋ_balance_llc_cpu_stop(ਖਾਲੀ *ਅੰਕੜੇ);

/*
 * migration_type is checked elsewhere to decide migration policy, so
 * it shouldn't be repurposed just to flag an LLC-directed active
 * balance across the stopper. Pick the callback here instead.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ cpu_stop_fn_t alb_stop_fn(ਬਣਤਰ lb_env *env)
{
	ਜੇ (env->migration_type == migrate_llc_task)
		ਵਾਪਸ ਸਰਗਰਮ_ਲੱਦੋ_balance_llc_cpu_stop;

	ਵਾਪਸ ਸਰਗਰਮ_ਲੱਦੋ_balance_cpu_stop;
}

ਸਥਿਰ ਪੂਰਨ should_we_balance(ਬਣਤਰ lb_env *env)
{
	ਬਣਤਰ cpumask *swb_cpus = this_cpu_cpumask_var_ptr(should_we_balance_tmpmask);
	ਬਣਤਰ sched_ਸਮੂਹ *sg = env->sd->groups;
	ਪੂਰਨ cpu, idle_smt = -1;

	/*
	 * Ensure the balancing environment is consistent; can happen
	 * when the softirq triggers 'during' hotplug.
	 */
	ਜੇ (!cpumask_test_cpu(env->dst_cpu, env->cpus))
		ਵਾਪਸ 0;

	/*
	 * In the newly idle case, we will allow all the CPUs
	 * to do the newly idle load balance.
	 *
	 * However, we bail out if we already have tasks or a wakeup pending,
	 * to optimize wakeup latency.
	 */
	ਜੇ (env->idle == CPU_NEWLY_IDLE) {
		ਜੇ (env->dst_rq->nr_running > 0 || env->dst_rq->ttwu_pending)
			ਵਾਪਸ 0;
		ਵਾਪਸ 1;
	}

	cpumask_copy(swb_cpus, ਸਮੂਹ_balance_ਨਕਾਬ(sg));
	/* Try to find first idle CPU */
	for_each_cpu_and(cpu, swb_cpus, env->cpus) {
		ਜੇ (!idle_cpu(cpu))
			ਜਾਰੀ;

		/*
		 * Don't balance to idle SMT in busy core right away when
		 * balancing cores, but remember the first idle SMT CPU for
		 * later consideration.  Find CPU on an idle core first.
		 */
		ਜੇ (sched_smt_ਸਰਗਰਮ() &&
		    !(env->sd->flags & SD_SHARE_CPUCAPACITY) &&
		    !is_core_idle(cpu)) {
			ਜੇ (idle_smt == -1)
				idle_smt = cpu;
			/*
			 * If the core is not idle, and first SMT sibling which is
			 * idle has been found, then its not needed to check other
			 * SMT siblings for idleness:
			 */
			cpumask_andnot(swb_cpus, swb_cpus, cpu_smt_ਨਕਾਬ(cpu));
			ਜਾਰੀ;
		}

		/*
		 * Are we the first idle core in a non-SMT domain or higher,
		 * or the first idle CPU in a SMT domain?
		 */
		ਵਾਪਸ cpu == env->dst_cpu;
	}

	/* Are we the first idle CPU with busy siblings? */
	ਜੇ (idle_smt != -1)
		ਵਾਪਸ idle_smt == env->dst_cpu;

	/* Are we the first CPU of this group ? */
	ਵਾਪਸ ਸਮੂਹ_balance_cpu(sg) == env->dst_cpu;
}

ਸਥਿਰ ਖਾਲੀ update_lb_imbalance_stat(ਬਣਤਰ lb_env *env, ਬਣਤਰ sched_domain *sd,
				     ਗਿਣਤੀ cpu_idle_type idle)
{
	ਜੇ (!schedstat_enabled())
		ਵਾਪਸ;

	ਚੋਣ (env->migration_type) {
	ਹਾਲਤ migrate_ਲੱਦੋ:
		__schedstat_add(sd->lb_imbalance_ਲੱਦੋ[idle], env->imbalance);
		ਰੋਕੋ;
	ਹਾਲਤ migrate_util:
		__schedstat_add(sd->lb_imbalance_util[idle], env->imbalance);
		ਰੋਕੋ;
	ਹਾਲਤ migrate_task:
		__schedstat_add(sd->lb_imbalance_task[idle], env->imbalance);
		ਰੋਕੋ;
	ਹਾਲਤ migrate_misfit:
		__schedstat_add(sd->lb_imbalance_misfit[idle], env->imbalance);
		ਰੋਕੋ;
	ਹਾਲਤ migrate_llc_task:
		ਰੋਕੋ;
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
ਸਥਿਰ atomic_t sched_balance_running = ATOMIC_INIT(0);

/*
 * Check this_cpu to ensure it is balanced within domain. Attempt to move
 * tasks if there is an imbalance.
 */
ਸਥਿਰ ਪੂਰਨ sched_balance_rq(ਪੂਰਨ this_cpu, ਬਣਤਰ rq *this_rq,
			ਬਣਤਰ sched_domain *sd, ਗਿਣਤੀ cpu_idle_type idle,
			ਪੂਰਨ *continue_balancing)
{
	ਪੂਰਨ ld_moved, cur_ld_moved, ਸਰਗਰਮ_balance = 0;
	ਬਣਤਰ sched_domain *sd_ਮਾਪਾ = sd->ਮਾਪਾ;
	ਬਣਤਰ sched_ਸਮੂਹ *ਸਮੂਹ;
	ਬਣਤਰ rq *busiest;
	ਬਣਤਰ rq_flags rf;
	ਬਣਤਰ cpumask *cpus = this_cpu_cpumask_var_ptr(ਲੱਦੋ_balance_ਨਕਾਬ);
	ਬਣਤਰ lb_env env = {
		.sd		= sd,
		.dst_cpu	= this_cpu,
		.dst_rq		= this_rq,
		.dst_grpmask    = ਸਮੂਹ_balance_ਨਕਾਬ(sd->groups),
		.idle		= idle,
		.loop_break	= SCHED_NR_MIGRATE_BREAK,
		.cpus		= cpus,
		.fbq_type	= all,
		.tasks		= LIST_ਸਿਰ_INIT(env.tasks),
	};
	bool need_unlock = false;

	cpumask_and(cpus, sched_domain_ਵਿਸਤਾਰ(sd), cpu_preferred_ਨਕਾਬ);

	schedstat_inc(sd->lb_ਗਿਣਤੀ[idle]);

redo:
	ਜੇ (!should_we_balance(&env)) {
		*continue_balancing = 0;
		ਜਾਓ ਬਾਹਰਲਾ_ਸੰਤੁਲਿਤ;
	}

	ਜੇ (!need_unlock && (sd->flags & SD_SERIALIZE)) {
		ਪੂਰਨ zero = 0;
		ਜੇ (!atomic_try_cmpxchg_acquire(&sched_balance_running, &zero, 1))
			ਜਾਓ ਬਾਹਰਲਾ_ਸੰਤੁਲਿਤ;

		need_unlock = true;
	}

	ਸਮੂਹ = sched_balance_ਲੱਭੋ_src_ਸਮੂਹ(&env);
	ਜੇ (!ਸਮੂਹ) {
		schedstat_inc(sd->lb_nobusyg[idle]);
		ਜਾਓ ਬਾਹਰਲਾ_ਸੰਤੁਲਿਤ;
	}

	busiest = sched_balance_ਲੱਭੋ_src_rq(&env, ਸਮੂਹ);
	ਜੇ (!busiest) {
		schedstat_inc(sd->lb_nobusyq[idle]);
		ਜਾਓ ਬਾਹਰਲਾ_ਸੰਤੁਲਿਤ;
	}

	WARN_ON_ONCE(busiest == env.dst_rq);

	update_lb_imbalance_stat(&env, sd, idle);

	env.src_cpu = busiest->cpu;
	env.src_rq = busiest;

	ld_moved = 0;
	/* Clear this flag as soon as we find a pullable task */
	env.flags |= LBF_ALL_PINNED;
	ਜੇ (busiest->nr_running > 1) {
		/*
		 * Attempt to move tasks. If sched_balance_find_src_group has found
		 * an imbalance but busiest->nr_running <= 1, the group is
		 * still unbalanced. ld_moved simply stays zero, so it is
		 * correctly treated as an imbalance.
		 */
		env.loop_max  = min(sysctl_sched_nr_migrate, busiest->nr_running);

ਹੋਰ_balance:
		rq_ਤਾਲਾ_irqsave(busiest, &rf);
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

		ਜੇ (cur_ld_moved) {
			attach_tasks(&env);
			ld_moved += cur_ld_moved;
		}

		local_irq_restore(rf.flags);

		ਜੇ (env.flags & LBF_NEED_BREAK) {
			env.flags &= ~LBF_NEED_BREAK;
			ਜਾਓ ਹੋਰ_balance;
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
		ਜੇ ((env.flags & LBF_DST_PINNED) && env.imbalance > 0) {

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
			ਜਾਓ ਹੋਰ_balance;
		}

		/*
		 * We failed to reach balance because of affinity.
		 */
		ਜੇ (sd_ਮਾਪਾ) {
			ਪੂਰਨ *ਸਮੂਹ_imbalance = &sd_ਮਾਪਾ->groups->sgc->imbalance;

			ਜੇ ((env.flags & LBF_SOME_PINNED) && env.imbalance > 0)
				*ਸਮੂਹ_imbalance = 1;
		}

		/* All tasks on this runqueue were pinned by CPU affinity */
		ਜੇ (unlikely(env.flags & LBF_ALL_PINNED)) {
			__cpumask_clear_cpu(cpu_ਦਾ(busiest), cpus);
			/*
			 * Attempting to continue load balancing at the current
			 * sched_domain level only makes sense if there are
			 * active CPUs remaining as possible busiest CPUs to
			 * pull load from which are not contained within the
			 * destination group that is receiving any migrated
			 * load.
			 */
			ਜੇ (!cpumask_subset(cpus, env.dst_grpmask)) {
				env.loop = 0;
				env.loop_break = SCHED_NR_MIGRATE_BREAK;
				ਜਾਓ redo;
			}
			ਜਾਓ ਬਾਹਰਲਾ_all_pinned;
		}
	}

	ਜੇ (ld_moved) {
		sd->nr_balance_failed = 0;
		ਜਾਓ ਬਾਹਰਲਾ_unbalanced;
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
	ਜੇ (idle != CPU_NEWLY_IDLE &&
	    env.migration_type != migrate_misfit &&
	    !(env.flags & LBF_LLC_PINNED))
		sd->nr_balance_failed++;

	ਜੇ (!need_ਸਰਗਰਮ_balance(&env))
		ਜਾਓ ਬਾਹਰਲਾ_unbalanced;

	scoped_guard (raw_spin_rq_ਤਾਲਾ_irqsave, busiest) {
		/*
		 * Don't kick the active_load_balance_cpu_stop,
		 * if the curr task on busiest CPU can't be
		 * moved to this_cpu:
		 */
		ਜੇ (!cpumask_test_cpu(this_cpu, busiest->curr->cpus_ptr))
			ਜਾਓ ਬਾਹਰਲਾ_one_pinned;

		/* Record that we found at least one task that could run on this_cpu */
		env.flags &= ~LBF_ALL_PINNED;

		/*
		 * ->active_balance synchronizes accesses to
		 * ->active_balance_work.  Once set, it's cleared
		 * only after active load balance is finished.
		 */
		ਜੇ (busiest->ਸਰਗਰਮ_balance)
			ਜਾਓ ਬਾਹਰਲਾ_unbalanced;

		/*
		 * @busiest dropped its rq_lock in the middle of
		 * scheduling out its ->curr task (->on_rq := 0), no
		 * need to forcefully punt it away with active balance.
		 */
		ਜੇ (!busiest->curr->on_rq)
			ਜਾਓ ਬਾਹਰਲਾ_unbalanced;

		busiest->ਸਰਗਰਮ_balance = 1;
		busiest->ਧੱਕੋ_cpu = this_cpu;
		ਸਰਗਰਮ_balance = 1;
		preempt_disable();
	}
	ਜੇ (ਸਰਗਰਮ_balance) {
		stop_one_cpu_nowait(cpu_ਦਾ(busiest),
				    alb_stop_fn(&env), busiest,
				    &busiest->ਸਰਗਰਮ_balance_work);
	}
	preempt_enable();

ਬਾਹਰਲਾ_unbalanced:
	/* We were unbalanced, so reset the balancing interval */
	sd->balance_interval = sd->min_interval;
	ਜਾਓ ਬਾਹਰਲਾ;

ਬਾਹਰਲਾ_ਸੰਤੁਲਿਤ:
	/*
	 * We reach balance although we may have faced some affinity
	 * constraints. Clear the imbalance flag only if other tasks got
	 * a chance to move and fix the imbalance.
	 */
	ਜੇ (sd_ਮਾਪਾ && !(env.flags & LBF_ALL_PINNED)) {
		ਪੂਰਨ *ਸਮੂਹ_imbalance = &sd_ਮਾਪਾ->groups->sgc->imbalance;

		ਜੇ (*ਸਮੂਹ_imbalance)
			*ਸਮੂਹ_imbalance = 0;
	}

ਬਾਹਰਲਾ_all_pinned:
	/*
	 * We reach balance because all tasks are pinned at this level so
	 * we can't migrate them. Let the imbalance flag set so parent level
	 * can try to migrate them.
	 */
	schedstat_inc(sd->lb_ਸੰਤੁਲਿਤ[idle]);

	sd->nr_balance_failed = 0;

ਬਾਹਰਲਾ_one_pinned:
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
	ਜੇ (env.idle == CPU_NEWLY_IDLE ||
	    env.migration_type == migrate_misfit)
		ਜਾਓ ਬਾਹਰਲਾ;

	/* tune up the balancing interval */
	ਜੇ ((env.flags & LBF_ALL_PINNED &&
	     sd->balance_interval < MAX_PINNED_INTERVAL) ||
	    sd->balance_interval < sd->max_interval)
		sd->balance_interval *= 2;
ਬਾਹਰਲਾ:
	ਜੇ (need_unlock)
		atomic_ਸੈੱਟ_release(&sched_balance_running, 0);

	ਵਾਪਸ ld_moved;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ
get_sd_balance_interval(ਬਣਤਰ sched_domain *sd, ਪੂਰਨ cpu_busy)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ interval = sd->balance_interval;

	ਜੇ (cpu_busy)
		interval *= sd->busy_ਗੁਣਕ;

	/* scale ms to jiffies */
	interval = msecs_ਮੰਜ਼ਿਲ_jiffies(interval);

	/*
	 * Reduce likelihood of busy balancing at higher domains racing with
	 * balancing at lower domains by preventing their balancing periods
	 * from being multiples of each other.
	 */
	ਜੇ (cpu_busy)
		interval -= 1;

	interval = clamp(interval, 1UL, max_ਲੱਦੋ_balance_interval);

	ਵਾਪਸ interval;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
update_ਅਗਲਾ_balance(ਬਣਤਰ sched_domain *sd, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ *ਅਗਲਾ_balance)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ interval, ਅਗਲਾ;

	/* used by idle balance, so cpu_busy = 0 */
	interval = get_sd_balance_interval(sd, 0);
	ਅਗਲਾ = sd->ਆਖਰੀ_balance + interval;

	ਜੇ (ਸਮਾਂ_after(*ਅਗਲਾ_balance, ਅਗਲਾ))
		*ਅਗਲਾ_balance = ਅਗਲਾ;
}

/*
 * active_load_balance_cpu_stop is run by the CPU stopper. It pushes
 * running tasks off the busiest CPU onto idle CPUs. It requires at
 * least 1 task to be running on each physical CPU where possible, and
 * avoids physical / logical imbalances.
 */
ਸਥਿਰ ਪੂਰਨ __active_load_balance_cpu_stop(ਖਾਲੀ *ਅੰਕੜੇ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lb_flags)
{
	ਬਣਤਰ rq *busiest_rq = ਅੰਕੜੇ;
	ਪੂਰਨ busiest_cpu = cpu_ਦਾ(busiest_rq);
	ਪੂਰਨ ਨਿਸ਼ਾਨਾ_cpu = busiest_rq->ਧੱਕੋ_cpu;
	ਬਣਤਰ rq *ਨਿਸ਼ਾਨਾ_rq = cpu_rq(ਨਿਸ਼ਾਨਾ_cpu);
	ਬਣਤਰ sched_domain *sd;
	ਬਣਤਰ task_struct *p = ਨਿਰਾ;
	ਬਣਤਰ rq_flags rf;

	rq_ਤਾਲਾ_irq(busiest_rq, &rf);
	/*
	 * Between queueing the stop-work and running it is a hole in which
	 * CPUs can become inactive. We should not move tasks from or to
	 * inactive CPUs.
	 */
	ਜੇ (!cpu_ਸਰਗਰਮ(busiest_cpu) || !cpu_ਸਰਗਰਮ(ਨਿਸ਼ਾਨਾ_cpu))
		ਜਾਓ ਬਾਹਰਲਾ_unlock;

	/* Make sure the requested CPU hasn't gone down in the meantime: */
	ਜੇ (unlikely(busiest_cpu != smp_processor_id() ||
		     !busiest_rq->ਸਰਗਰਮ_balance))
		ਜਾਓ ਬਾਹਰਲਾ_unlock;

	/* Is there any task to move? */
	ਜੇ (busiest_rq->nr_running <= 1)
		ਜਾਓ ਬਾਹਰਲਾ_unlock;

	/*
	 * This condition is "impossible", if it occurs
	 * we need to fix it. Originally reported by
	 * Bjorn Helgaas on a 128-CPU setup.
	 */
	WARN_ON_ONCE(busiest_rq == ਨਿਸ਼ਾਨਾ_rq);

	/* Search for an sd spanning us and the target CPU. */
	rcu_read_ਤਾਲਾ();
	for_each_domain(ਨਿਸ਼ਾਨਾ_cpu, sd) {
		ਜੇ (cpumask_test_cpu(busiest_cpu, sched_domain_ਵਿਸਤਾਰ(sd)))
			ਰੋਕੋ;
	}

	ਜੇ (likely(sd)) {
		ਬਣਤਰ lb_env env = {
			.sd		= sd,
			.dst_cpu	= ਨਿਸ਼ਾਨਾ_cpu,
			.dst_rq		= ਨਿਸ਼ਾਨਾ_rq,
			.src_cpu	= busiest_rq->cpu,
			.src_rq		= busiest_rq,
			.idle		= CPU_IDLE,
			.flags		= LBF_ਸਰਗਰਮ_LB | lb_flags,
		};

		schedstat_inc(sd->alb_ਗਿਣਤੀ);
		update_rq_clock(busiest_rq);

		p = detach_one_task(&env);
		ਜੇ (p) {
			schedstat_inc(sd->alb_pushed);
			/* Active balancing done, reset the failure counter. */
			sd->nr_balance_failed = 0;
		} ਨਹੀਂਤਾਂ {
			schedstat_inc(sd->alb_failed);
		}
	}
	rcu_read_unlock();
ਬਾਹਰਲਾ_unlock:
	busiest_rq->ਸਰਗਰਮ_balance = 0;
	rq_unlock(busiest_rq, &rf);

	ਜੇ (p)
		attach_one_task(ਨਿਸ਼ਾਨਾ_rq, p);

	local_irq_enable();

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ ਸਰਗਰਮ_ਲੱਦੋ_balance_cpu_stop(ਖਾਲੀ *ਅੰਕੜੇ)
{
	ਵਾਪਸ __active_load_balance_cpu_stop(ਅੰਕੜੇ, 0);
}

ਸਥਿਰ ਪੂਰਨ ਸਰਗਰਮ_ਲੱਦੋ_balance_llc_cpu_stop(ਖਾਲੀ *ਅੰਕੜੇ)
{
	ਵਾਪਸ __active_load_balance_cpu_stop(ਅੰਕੜੇ, LBF_ਸਰਗਰਮ_LB_LLC);
}

/*
 * Scale the max sched_balance_rq interval with the number of CPUs in the system.
 * This trades load-balance latency on larger machines for less cross talk.
 */
ਖਾਲੀ update_max_interval(ਖਾਲੀ)
{
	max_ਲੱਦੋ_balance_interval = HZ*ਅੰਕ_online_cpus()/10;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ update_newidle_stats(ਬਣਤਰ sched_domain *sd, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ success)
{
	sd->newidle_call++;
	sd->newidle_success += success;

	ਜੇ (sd->newidle_call >= 1024) {
		u64 now = sched_clock();
		s64 delta = now - sd->newidle_stamp;
		sd->newidle_stamp = now;
		ਪੂਰਨ ਅਨੁਪਾਤ = 0;

		ਜੇ (delta < 0)
			delta = 0;

		ਜੇ (sched_feat(NI_ਦਰ)) {
			/*
			 * ratio  delta   freq
			 *
			 * 1024 -  4  s -  128 Hz
			 *  512 -  2  s -  256 Hz
			 *  256 -  1  s -  512 Hz
			 *  128 - .5  s - 1024 Hz
			 *   64 - .25 s - 2048 Hz
			 */
			ਅਨੁਪਾਤ = delta >> 22;
		}

		ਅਨੁਪਾਤ += sd->newidle_success;

		sd->newidle_ਅਨੁਪਾਤ = min(1024, ਅਨੁਪਾਤ);
		sd->newidle_call /= 2;
		sd->newidle_success /= 2;
	}
}

ਸਥਿਰ ਸਤਰਅੰਦਰ bool
update_newidle_ਲਾਗਤ(ਬਣਤਰ sched_domain *sd, u64 ਲਾਗਤ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ success)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਅਗਲਾ_decay = sd->ਆਖਰੀ_decay_max_lb_ਲਾਗਤ + HZ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ now = jiffies;

	ਜੇ (ਲਾਗਤ)
		update_newidle_stats(sd, success);

	ਜੇ (ਲਾਗਤ > sd->max_newidle_lb_ਲਾਗਤ) {
		/*
		 * Track max cost of a domain to make sure to not delay the
		 * next wakeup on the CPU.
		 */
		sd->max_newidle_lb_ਲਾਗਤ = ਲਾਗਤ;
		sd->ਆਖਰੀ_decay_max_lb_ਲਾਗਤ = now;

	} ਨਹੀਂਤਾਂ ਜੇ (ਸਮਾਂ_after(now, ਅਗਲਾ_decay)) {
		/*
		 * Decay the newidle max times by ~1% per second to ensure that
		 * it is not outdated and the current max cost is actually
		 * shorter.
		 */
		sd->max_newidle_lb_ਲਾਗਤ = (sd->max_newidle_lb_ਲਾਗਤ * 253) / 256;
		sd->ਆਖਰੀ_decay_max_lb_ਲਾਗਤ = now;
		ਵਾਪਸ true;
	}

	ਵਾਪਸ false;
}

/*
 * It checks each scheduling domain to see if it is due to be balanced,
 * and initiates a balancing operation if so.
 *
 * Balancing parameters are set up in init_sched_domains.
 */
ਸਥਿਰ ਖਾਲੀ sched_balance_domains(ਬਣਤਰ rq *rq, ਗਿਣਤੀ cpu_idle_type idle)
{
	ਪੂਰਨ continue_balancing = 1;
	ਪੂਰਨ cpu = rq->cpu;
	ਪੂਰਨ busy = idle != CPU_IDLE && !sched_idle_rq(rq);
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ interval;
	ਬਣਤਰ sched_domain *sd;
	/* Earliest time when we have to do rebalance again */
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਅਗਲਾ_balance = jiffies + 60*HZ;
	ਪੂਰਨ update_ਅਗਲਾ_balance = 0;
	ਪੂਰਨ need_decay = 0;
	u64 max_ਲਾਗਤ = 0;

	rcu_read_ਤਾਲਾ();
	for_each_domain(cpu, sd) {
		/*
		 * Decay the newidle max times here because this is a regular
		 * visit to all the domains.
		 */
		need_decay = update_newidle_ਲਾਗਤ(sd, 0, 0);
		max_ਲਾਗਤ += sd->max_newidle_lb_ਲਾਗਤ;

		/*
		 * Stop the load balance at this level. There is another
		 * CPU in our sched group which is doing load balancing more
		 * actively.
		 */
		ਜੇ (!continue_balancing) {
			ਜੇ (need_decay)
				ਜਾਰੀ;
			ਰੋਕੋ;
		}

		interval = get_sd_balance_interval(sd, busy);
		ਜੇ (ਸਮਾਂ_after_eq(jiffies, sd->ਆਖਰੀ_balance + interval)) {
			ਜੇ (sched_balance_rq(cpu, rq, sd, idle, &continue_balancing)) {
				/*
				 * The LBF_DST_PINNED logic could have changed
				 * env->dst_cpu, so we can't know our idle
				 * state even if we migrated tasks. Update it.
				 */
				idle = idle_cpu(cpu);
				busy = !idle && !sched_idle_rq(rq);
			}
			sd->ਆਖਰੀ_balance = jiffies;
			interval = get_sd_balance_interval(sd, busy);
		}
		ਜੇ (ਸਮਾਂ_after(ਅਗਲਾ_balance, sd->ਆਖਰੀ_balance + interval)) {
			ਅਗਲਾ_balance = sd->ਆਖਰੀ_balance + interval;
			update_ਅਗਲਾ_balance = 1;
		}
	}
	ਜੇ (need_decay) {
		/*
		 * Ensure the rq-wide value also decays but keep it at a
		 * reasonable floor to avoid funnies with rq->avg_idle.
		 */
		rq->max_idle_balance_ਲਾਗਤ =
			max((u64)sysctl_sched_migration_ਲਾਗਤ, max_ਲਾਗਤ);
	}
	rcu_read_unlock();

	/*
	 * next_balance will be updated only when there is a need.
	 * When the cpu is attached to null domain for ex, it will not be
	 * updated.
	 */
	ਜੇ (likely(update_ਅਗਲਾ_balance))
		rq->ਅਗਲਾ_balance = ਅਗਲਾ_balance;

}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ on_null_domain(ਬਣਤਰ rq *rq)
{
	ਵਾਪਸ unlikely(!rcu_dereference_sched(rq->sd));
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_NO_HZ_COMMON
/*
 * NOHZ idle load balancing (ILB) details:
 *
 * - When one of the busy CPUs notices that there may be an idle rebalancing
 *   needed, they will kick the idle load balancer, which then does idle
 *   load balancing for all the idle CPUs.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ ਲੱਭੋ_new_ilb(ਖਾਲੀ)
{
	ਬਣਤਰ cpumask *ilb_cpus;
	ਪੂਰਨ ilb_cpu, fallback = -1;

	lockdep_assert_irqs_disabled();

	/*
	 * Reuse the per-CPU select_rq_mask, which is protected from concurrent
	 * use on this CPU by having interrupts disabled.
	 */
	ilb_cpus = this_cpu_cpumask_var_ptr(select_rq_ਨਕਾਬ);
	cpumask_and(ilb_cpus, nohz.idle_cpus_ਨਕਾਬ,
		    housekeeping_cpumask(HK_TYPE_KERNEL_NOISE));

	for_each_cpu(ilb_cpu, ilb_cpus) {
		ਜੇ (!idle_cpu(ilb_cpu)) {
			/*
			 * Once an idle fallback exists, a busy CPU proves that
			 * this core cannot be fully idle. Skip its siblings.
			 */
			ਜੇ (sched_smt_ਸਰਗਰਮ() && fallback >= 0)
				cpumask_andnot(ilb_cpus, ilb_cpus, cpu_smt_ਨਕਾਬ(ilb_cpu));
			ਜਾਰੀ;
		}

		/*
		 * Running the idle load balancer on an idle sibling of a busy
		 * SMT core can reduce the capacity available to its sibling. Prefer
		 * a CPU whose entire core is idle, but retain the first idle CPU as
		 * a fallback so idle balancing can still make progress when no fully
		 * idle core exists.
		 */
		ਜੇ (sched_smt_ਸਰਗਰਮ() && !is_core_idle(ilb_cpu)) {
			ਜੇ (fallback < 0)
				fallback = ilb_cpu;

			/*
			 * The core is not idle, so there is no need to check
			 * any of its other SMT siblings.
			 */
			cpumask_andnot(ilb_cpus, ilb_cpus,
				       cpu_smt_ਨਕਾਬ(ilb_cpu));
			ਜਾਰੀ;
		}

		ਵਾਪਸ ilb_cpu;
	}

	ਵਾਪਸ fallback;
}

/*
 * Kick a CPU to do the NOHZ balancing, if it is time for it, via a cross-CPU
 * SMP function call (IPI).
 *
 * Prefer a CPU on a fully idle core in the HK_TYPE_KERNEL_NOISE housekeeping
 * set. Fall back to the first idle CPU when no fully idle core exists.
 */
ਸਥਿਰ ਖਾਲੀ kick_ilb(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags)
{
	ਪੂਰਨ ilb_cpu;

	/*
	 * Increase nohz.next_balance only when if full ilb is triggered but
	 * not if we only update stats.
	 */
	ਜੇ (flags & NOHZ_BALANCE_KICK)
		nohz.ਅਗਲਾ_balance = jiffies+1;

	ilb_cpu = ਲੱਭੋ_new_ilb();
	ਜੇ (ilb_cpu < 0)
		ਵਾਪਸ;

	/*
	 * Don't bother if no new NOHZ balance work items for ilb_cpu,
	 * i.e. all bits in flags are already set in ilb_cpu.
	 */
	ਜੇ ((atomic_read(nohz_flags(ilb_cpu)) & flags) == flags)
		ਵਾਪਸ;

	/*
	 * Access to rq::nohz_csd is serialized by NOHZ_KICK_MASK; he who sets
	 * the first flag owns it; cleared by nohz_csd_func().
	 */
	flags = atomic_fetch_or(flags, nohz_flags(ilb_cpu));
	ਜੇ (flags & NOHZ_KICK_ਨਕਾਬ)
		ਵਾਪਸ;

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
ਸਥਿਰ ਖਾਲੀ nohz_balancer_kick(ਬਣਤਰ rq *rq)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ now = jiffies;
	ਬਣਤਰ sched_domain_shared *sds;
	ਬਣਤਰ sched_domain *sd;
	ਪੂਰਨ nr_busy, i, cpu = rq->cpu;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags = 0;

	ਜੇ (unlikely(rq->idle_balance))
		ਵਾਪਸ;

	/*
	 * We may be recently in ticked or tickless idle mode. At the first
	 * busy tick after returning from idle, we will update the busy stats.
	 */
	nohz_balance_exit_idle(rq);

	ਜੇ (READ_ONCE(nohz.has_blocked_ਲੱਦੋ) &&
	    ਸਮਾਂ_after(now, READ_ONCE(nohz.ਅਗਲਾ_blocked)))
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
	ਜੇ (ਸਮਾਂ_before(now, nohz.ਅਗਲਾ_balance))
		ਜਾਓ ਬਾਹਰਲਾ;

	/*
	 * None are in tickless mode and hence no need for NOHZ idle load
	 * balancing
	 */
	ਜੇ (unlikely(cpumask_ਖਾਲੀ_ਥਾਵਾਂ(nohz.idle_cpus_ਨਕਾਬ)))
		ਵਾਪਸ;

	ਜੇ (rq->nr_running >= 2) {
		flags = NOHZ_STATS_KICK | NOHZ_BALANCE_KICK;
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	sd = rcu_dereference_all(rq->sd);
	ਜੇ (sd) {
		/*
		 * If there's a runnable CFS task and the current CPU has reduced
		 * capacity, kick the ILB to see if there's a better CPU to run on:
		 */
		ਜੇ (rq->cfs.h_nr_runnable >= 1 && check_cpu_ਸਮਰੱਥਾ(rq, sd)) {
			flags |= NOHZ_STATS_KICK | NOHZ_BALANCE_KICK;
			ਜਾਓ ਬਾਹਰਲਾ;
		}
	}

	sd = rcu_dereference_all(ਪ੍ਰਤੀ_cpu(sd_asym_packing, cpu));
	ਜੇ (sd) {
		/*
		 * When ASYM_PACKING; see if there's a more preferred CPU
		 * currently idle; in which case, kick the ILB to move tasks
		 * around.
		 *
		 * When balancing between cores, all the SMT siblings of the
		 * preferred CPU must be idle.
		 */
		for_each_cpu_and(i, sched_domain_ਵਿਸਤਾਰ(sd), nohz.idle_cpus_ਨਕਾਬ) {
			ਜੇ (sched_asym(sd, i, cpu)) {
				flags |= NOHZ_STATS_KICK | NOHZ_BALANCE_KICK;
				ਜਾਓ ਬਾਹਰਲਾ;
			}
		}
	}

	sd = rcu_dereference_all(ਪ੍ਰਤੀ_cpu(sd_asym_cpucapacity, cpu));
	ਜੇ (sd) {
		/*
		 * When ASYM_CPUCAPACITY; see if there's a higher capacity CPU
		 * to run the misfit task on.
		 */
		ਜੇ (check_misfit_ਸਥਿਤੀ(rq))
			flags |= NOHZ_STATS_KICK | NOHZ_BALANCE_KICK;

		/*
		 * For asymmetric systems, we do not want to nicely balance
		 * cache use, instead we want to embrace asymmetry and only
		 * ensure tasks have enough CPU capacity.
		 *
		 * Skip the LLC logic because it's not relevant in that case.
		 */
		ਜਾਓ ਬਾਹਰਲਾ;
	}

	sds = rcu_dereference_all(ਪ੍ਰਤੀ_cpu(sd_balance_shared, cpu));
	ਜੇ (sds) {
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
		ਜੇ (nr_busy > 1)
			flags |= NOHZ_STATS_KICK | NOHZ_BALANCE_KICK;
	}
ਬਾਹਰਲਾ:
	ਜੇ (READ_ONCE(nohz.needs_update))
		flags |= NOHZ_ਅਗਲਾ_KICK;

	ਜੇ (flags)
		kick_ilb(flags);
}

ਸਥਿਰ ਖਾਲੀ ਸੈੱਟ_cpu_sd_state_busy(ਪੂਰਨ cpu)
{
	ਬਣਤਰ sched_domain *sd;
	sd = rcu_dereference_all(ਪ੍ਰਤੀ_cpu(sd_llc, cpu));

	/*
	 * sd->nohz_idle only pairs with nr_busy_cpus on sd->shared; if this
	 * domain has no shared object there is nothing to clear or account.
	 */
	ਜੇ (!sd || !sd->shared || !sd->nohz_idle)
		ਵਾਪਸ;
	sd->nohz_idle = 0;

	atomic_inc(&sd->shared->nr_busy_cpus);
}

ਖਾਲੀ nohz_balance_exit_idle(ਬਣਤਰ rq *rq)
{
	WARN_ON_ONCE(rq != this_rq());

	ਜੇ (likely(!rq->nohz_tick_stopped))
		ਵਾਪਸ;

	rq->nohz_tick_stopped = 0;
	cpumask_clear_cpu(rq->cpu, nohz.idle_cpus_ਨਕਾਬ);

	ਸੈੱਟ_cpu_sd_state_busy(rq->cpu);
}

ਸਥਿਰ ਖਾਲੀ ਸੈੱਟ_cpu_sd_state_idle(ਪੂਰਨ cpu)
{
	ਬਣਤਰ sched_domain *sd;
	sd = rcu_dereference_all(ਪ੍ਰਤੀ_cpu(sd_llc, cpu));

	/* See set_cpu_sd_state_busy(): nohz_idle is only used with sd->shared. */
	ਜੇ (!sd || !sd->shared || sd->nohz_idle)
		ਵਾਪਸ;
	sd->nohz_idle = 1;

	atomic_dec(&sd->shared->nr_busy_cpus);
}

/*
 * This routine will record that the CPU is going idle with tick stopped.
 * This info will be used in performing idle load balancing in the future.
 */
ਖਾਲੀ nohz_balance_enter_idle(ਪੂਰਨ cpu)
{
	ਬਣਤਰ rq *rq = cpu_rq(cpu);

	WARN_ON_ONCE(cpu != smp_processor_id());

	/* If this CPU is going down, then nothing needs to be done: */
	ਜੇ (!cpu_ਸਰਗਰਮ(cpu))
		ਵਾਪਸ;

	/*
	 * Can be set safely without rq->lock held
	 * If a clear happens, it will have evaluated last additions because
	 * rq->lock is held during the check and the clear
	 */
	rq->has_blocked_ਲੱਦੋ = 1;

	/*
	 * The tick is still stopped but load could have been added in the
	 * meantime. We set the nohz.has_blocked_load flag to trig a check of the
	 * *_avg. The CPU is already part of nohz.idle_cpus_mask so the clear
	 * of nohz.has_blocked_load can only happen after checking the new load
	 */
	ਜੇ (rq->nohz_tick_stopped)
		ਜਾਓ ਬਾਹਰਲਾ;

	/* If we're a completely isolated CPU, we don't play: */
	ਜੇ (on_null_domain(rq))
		ਵਾਪਸ;

	rq->nohz_tick_stopped = 1;

	cpumask_ਸੈੱਟ_cpu(cpu, nohz.idle_cpus_ਨਕਾਬ);

	/*
	 * Ensures that if nohz_idle_balance() fails to observe our
	 * @idle_cpus_mask store, it must observe the @has_blocked_load
	 * and @needs_update stores.
	 */
	smp_mb__after_atomic();

	ਸੈੱਟ_cpu_sd_state_idle(cpu);

	WRITE_ONCE(nohz.needs_update, 1);
ਬਾਹਰਲਾ:
	/*
	 * Each time a cpu enter idle, we assume that it has blocked load and
	 * enable the periodic update of the load of idle CPUs
	 */
	WRITE_ONCE(nohz.has_blocked_ਲੱਦੋ, 1);
}

ਸਥਿਰ bool update_nohz_stats(ਬਣਤਰ rq *rq)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ cpu = rq->cpu;

	ਜੇ (!rq->has_blocked_ਲੱਦੋ)
		ਵਾਪਸ false;

	ਜੇ (!cpumask_test_cpu(cpu, nohz.idle_cpus_ਨਕਾਬ))
		ਵਾਪਸ false;

	ਜੇ (!ਸਮਾਂ_after(jiffies, READ_ONCE(rq->ਆਖਰੀ_blocked_ਲੱਦੋ_update_tick)))
		ਵਾਪਸ true;

	sched_balance_update_blocked_averages(cpu);

	ਵਾਪਸ rq->has_blocked_ਲੱਦੋ;
}

/*
 * Internal function that runs load balance for all idle CPUs. The load balance
 * can be a simple update of blocked load or a complete load balance with
 * tasks movement depending of flags.
 */
ਸਥਿਰ ਖਾਲੀ _nohz_idle_balance(ਬਣਤਰ rq *this_rq, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags)
{
	/* Earliest time when we have to do rebalance again */
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ now = jiffies;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਅਗਲਾ_balance = now + 60*HZ;
	bool has_blocked_ਲੱਦੋ = false;
	ਪੂਰਨ update_ਅਗਲਾ_balance = 0;
	ਪੂਰਨ this_cpu = this_rq->cpu;
	ਪੂਰਨ balance_cpu;
	ਬਣਤਰ rq *rq;

	WARN_ON_ONCE((flags & NOHZ_KICK_ਨਕਾਬ) == NOHZ_BALANCE_KICK);

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
	ਜੇ (flags & NOHZ_STATS_KICK)
		WRITE_ONCE(nohz.has_blocked_ਲੱਦੋ, 0);
	ਜੇ (flags & NOHZ_ਅਗਲਾ_KICK)
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
	for_each_cpu_wrap(balance_cpu,  nohz.idle_cpus_ਨਕਾਬ, this_cpu+1) {
		ਜੇ (!idle_cpu(balance_cpu))
			ਜਾਰੀ;

		/*
		 * If this CPU gets work to do, stop the load balancing
		 * work being done for other CPUs. Next load
		 * balancing owner will pick it up.
		 */
		ਜੇ (!idle_cpu(this_cpu) && need_resched()) {
			ਜੇ (flags & NOHZ_STATS_KICK)
				has_blocked_ਲੱਦੋ = true;
			ਜੇ (flags & NOHZ_ਅਗਲਾ_KICK)
				WRITE_ONCE(nohz.needs_update, 1);
			ਜਾਓ abort;
		}

		rq = cpu_rq(balance_cpu);

		ਜੇ (flags & NOHZ_STATS_KICK)
			has_blocked_ਲੱਦੋ |= update_nohz_stats(rq);

		/*
		 * If time for next balance is due,
		 * do the balance.
		 */
		ਜੇ (ਸਮਾਂ_after_eq(jiffies, rq->ਅਗਲਾ_balance)) {
			ਬਣਤਰ rq_flags rf;

			rq_ਤਾਲਾ_irqsave(rq, &rf);
			update_rq_clock(rq);
			rq_unlock_irqrestore(rq, &rf);

			ਜੇ (flags & NOHZ_BALANCE_KICK)
				sched_balance_domains(rq, CPU_IDLE);
		}

		ਜੇ (ਸਮਾਂ_after(ਅਗਲਾ_balance, rq->ਅਗਲਾ_balance)) {
			ਅਗਲਾ_balance = rq->ਅਗਲਾ_balance;
			update_ਅਗਲਾ_balance = 1;
		}
	}

	/*
	 * next_balance will be updated only when there is a need.
	 * When the CPU is attached to null domain for ex, it will not be
	 * updated.
	 */
	ਜੇ (likely(update_ਅਗਲਾ_balance))
		nohz.ਅਗਲਾ_balance = ਅਗਲਾ_balance;

	ਜੇ (flags & NOHZ_STATS_KICK)
		WRITE_ONCE(nohz.ਅਗਲਾ_blocked,
			   now + msecs_ਮੰਜ਼ਿਲ_jiffies(ਲੱਦੋ_AVG_PERIOD));

abort:
	/* There is still blocked load, enable periodic update */
	ਜੇ (has_blocked_ਲੱਦੋ)
		WRITE_ONCE(nohz.has_blocked_ਲੱਦੋ, 1);
}

/*
 * In CONFIG_NO_HZ_COMMON case, the idle balance kickee will do the
 * rebalancing for all the CPUs for whom scheduler ticks are stopped.
 */
ਸਥਿਰ bool nohz_idle_balance(ਬਣਤਰ rq *this_rq, ਗਿਣਤੀ cpu_idle_type idle)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags = this_rq->nohz_idle_balance;

	ਜੇ (!flags)
		ਵਾਪਸ false;

	this_rq->nohz_idle_balance = 0;

	ਜੇ (idle != CPU_IDLE)
		ਵਾਪਸ false;

	_nohz_idle_balance(this_rq, flags);

	ਵਾਪਸ true;
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
ਖਾਲੀ nohz_ਲੜੀਵਾਰ_idle_balance(ਪੂਰਨ cpu)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags;

	flags = atomic_fetch_andnot(NOHZ_NEWILB_KICK, nohz_flags(cpu));

	/*
	 * Update the blocked load only if no SCHED_SOFTIRQ is about to happen
	 * (i.e. NOHZ_STATS_KICK set) and will do the same.
	 */
	ਜੇ ((flags == NOHZ_NEWILB_KICK) && !need_resched())
		_nohz_idle_balance(cpu_rq(cpu), NOHZ_STATS_KICK);
}

ਸਥਿਰ ਖਾਲੀ nohz_newidle_balance(ਬਣਤਰ rq *this_rq)
{
	ਪੂਰਨ this_cpu = this_rq->cpu;

	/* Will wake up very soon. No time for doing anything else*/
	ਜੇ (this_rq->avg_idle < sysctl_sched_migration_ਲਾਗਤ)
		ਵਾਪਸ;

	/* Don't need to update blocked load of idle CPUs*/
	ਜੇ (!READ_ONCE(nohz.has_blocked_ਲੱਦੋ) ||
	    ਸਮਾਂ_before(jiffies, READ_ONCE(nohz.ਅਗਲਾ_blocked)))
		ਵਾਪਸ;

	/*
	 * Set the need to trigger ILB in order to update blocked load
	 * before entering idle state.
	 */
	atomic_or(NOHZ_NEWILB_KICK, nohz_flags(this_cpu));
}

#ਨਹੀਂਤਾਂ /* !CONFIG_NO_HZ_COMMON: */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ nohz_balancer_kick(ਬਣਤਰ rq *rq) { }

ਸਥਿਰ ਸਤਰਅੰਦਰ bool nohz_idle_balance(ਬਣਤਰ rq *this_rq, ਗਿਣਤੀ cpu_idle_type idle)
{
	ਵਾਪਸ false;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ nohz_newidle_balance(ਬਣਤਰ rq *this_rq) { }
#ਜੇ_ਅੰਤ /* !CONFIG_NO_HZ_COMMON */

/*
 * sched_balance_newidle is called by schedule() if this_cpu is about to become
 * idle. Attempts to pull tasks from other CPUs.
 *
 * Returns:
 *   < 0 - we released the lock and there are !fair tasks present
 *     0 - failed, no new tasks
 *   > 0 - success, new (fair) tasks present
 */
ਸਥਿਰ ਪੂਰਨ sched_balance_newidle(ਬਣਤਰ rq *this_rq, ਬਣਤਰ rq_flags *rf)
	__must_hold(__rq_lockp(this_rq))
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਅਗਲਾ_balance = jiffies + HZ;
	ਪੂਰਨ this_cpu = this_rq->cpu;
	ਪੂਰਨ continue_balancing = 1;
	u64 t0, t1, curr_ਲਾਗਤ = 0;
	ਬਣਤਰ sched_domain *sd;
	ਪੂਰਨ pulled_task = 0;

	update_misfit_ਸਥਿਤੀ(ਨਿਰਾ, this_rq);

	/*
	 * There is a task waiting to run. No need to search for one.
	 * Return 0; the task will be enqueued when switching to idle.
	 */
	ਜੇ (this_rq->ttwu_pending)
		ਵਾਪਸ 0;

	/*
	 * We must set idle_stamp _before_ calling sched_balance_rq()
	 * for CPU_NEWLY_IDLE, such that we measure the this duration
	 * as idle time.
	 */
	this_rq->idle_stamp = rq_clock(this_rq);

	/* Do not pull tasks towards !preferred CPUs */
	ਜੇ (!cpu_preferred(this_cpu))
		ਵਾਪਸ 0;

	/*
	 * This is OK, because current is on_cpu, which avoids it being picked
	 * for load-balance and preemption/IRQs are still disabled avoiding
	 * further scheduler activity on it and we're being very careful to
	 * re-start the picking loop.
	 */
	rq_unpin_ਤਾਲਾ(this_rq, rf);

	sd = rcu_dereference_sched_domain(this_rq->sd);
	ਜੇ (!sd)
		ਜਾਓ ਬਾਹਰਲਾ;

	ਜੇ (!get_rd_overloaded(this_rq->rd) ||
	    this_rq->avg_idle < sd->max_newidle_lb_ਲਾਗਤ) {

		update_ਅਗਲਾ_balance(sd, &ਅਗਲਾ_balance);
		ਜਾਓ ਬਾਹਰਲਾ;
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
		u64 domain_ਲਾਗਤ;

		update_ਅਗਲਾ_balance(sd, &ਅਗਲਾ_balance);

		ਜੇ (this_rq->avg_idle < curr_ਲਾਗਤ + sd->max_newidle_lb_ਲਾਗਤ)
			ਰੋਕੋ;

		ਜੇ (sd->flags & SD_BALANCE_NEWIDLE) {
			ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਭਾਰ = 1;

			ਜੇ (sched_feat(NI_RANDOM) && sd->newidle_ਅਨੁਪਾਤ < 1024) {
				/*
				 * Throw a 1k sided dice; and only run
				 * newidle_balance according to the success
				 * rate.
				 */
				u32 d1k = sched_rng() % 1024;
				ਭਾਰ = 1 + sd->newidle_ਅਨੁਪਾਤ;
				ਜੇ (d1k > ਭਾਰ) {
					update_newidle_stats(sd, 0);
					ਜਾਰੀ;
				}
				ਭਾਰ = (1024 + ਭਾਰ/2) / ਭਾਰ;
			}

			pulled_task = sched_balance_rq(this_cpu, this_rq,
						   sd, CPU_NEWLY_IDLE,
						   &continue_balancing);

			t1 = sched_clock_cpu(this_cpu);
			domain_ਲਾਗਤ = t1 - t0;
			curr_ਲਾਗਤ += domain_ਲਾਗਤ;
			t0 = t1;

			/*
			 * Track max cost of a domain to make sure to not delay the
			 * next wakeup on the CPU.
			 */
			update_newidle_ਲਾਗਤ(sd, domain_ਲਾਗਤ, ਭਾਰ * !!pulled_task);
		}

		/*
		 * Stop searching for tasks to pull if there are
		 * now runnable tasks on this rq.
		 */
		ਜੇ (pulled_task || !continue_balancing)
			ਰੋਕੋ;
	}

	raw_spin_rq_ਤਾਲਾ(this_rq);

	ਜੇ (curr_ਲਾਗਤ > this_rq->max_idle_balance_ਲਾਗਤ)
		this_rq->max_idle_balance_ਲਾਗਤ = curr_ਲਾਗਤ;

	/*
	 * While browsing the domains, we released the rq lock, a task could
	 * have been enqueued in the meantime. Since we're not going idle,
	 * pretend we pulled a task.
	 */
	ਜੇ (this_rq->cfs.h_nr_queued && !pulled_task)
		pulled_task = 1;

	/* If a higher prio class was modified, restart the pick */
	ਜੇ (rq_modified_above(this_rq, &fair_sched_class))
		pulled_task = -1;

ਬਾਹਰਲਾ:
	/* Move the next balance forward */
	ਜੇ (ਸਮਾਂ_after(this_rq->ਅਗਲਾ_balance, ਅਗਲਾ_balance))
		this_rq->ਅਗਲਾ_balance = ਅਗਲਾ_balance;

	ਜੇ (pulled_task)
		this_rq->idle_stamp = 0;
	ਨਹੀਂਤਾਂ
		nohz_newidle_balance(this_rq);

	rq_repin_ਤਾਲਾ(this_rq, rf);

	ਵਾਪਸ pulled_task;
}

/*
 * This softirq handler is triggered via SCHED_SOFTIRQ from two places:
 *
 * - directly from the local sched_tick() for periodic load balancing
 *
 * - indirectly from a remote sched_tick() for NOHZ idle balancing
 *   through the SMP cross-call nohz_csd_func()
 */
ਸਥਿਰ __latent_entropy ਖਾਲੀ sched_balance_softirq(ਖਾਲੀ)
{
	ਬਣਤਰ rq *this_rq = this_rq();
	ਗਿਣਤੀ cpu_idle_type idle = this_rq->idle_balance;
	/*
	 * If this CPU has a pending NOHZ_BALANCE_KICK, then do the
	 * balancing on behalf of the other idle CPUs whose ticks are
	 * stopped. Do nohz_idle_balance *before* sched_balance_domains to
	 * give the idle CPUs a chance to load balance. Else we may
	 * load balance only within the local sched_domain hierarchy
	 * and abort nohz_idle_balance altogether if we pull some load.
	 */
	ਜੇ (nohz_idle_balance(this_rq, idle))
		ਵਾਪਸ;

	/* normal load balance */
	sched_balance_update_blocked_averages(this_rq->cpu);
	sched_balance_domains(this_rq, idle);
}

/*
 * Trigger the SCHED_SOFTIRQ if it is time to do periodic load balancing.
 */
ਖਾਲੀ sched_balance_trigger(ਬਣਤਰ rq *rq)
{
	/*
	 * Don't need to rebalance while attached to NULL domain or
	 * runqueue CPU is not active
	 */
	ਜੇ (unlikely(on_null_domain(rq) || !cpu_ਸਰਗਰਮ(cpu_ਦਾ(rq))))
		ਵਾਪਸ;

	ਜੇ (ਸਮਾਂ_after_eq(jiffies, rq->ਅਗਲਾ_balance))
		raise_softirq(SCHED_SOFTIRQ);

	nohz_balancer_kick(rq);
}

ਸਥਿਰ ਖਾਲੀ rq_online_fair(ਬਣਤਰ rq *rq)
{
	update_sysctl();

	update_runtime_enabled(rq);
}

ਸਥਿਰ ਖਾਲੀ rq_offline_fair(ਬਣਤਰ rq *rq)
{
	update_sysctl();

	/* Ensure any throttled groups are reachable by pick_next_task */
	unthrottle_offline_cfs_rqs(rq);

	/* Ensure that we remove rq contribution to group share: */
	clear_tg_offline_cfs_rqs(rq);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CORE
ਸਥਿਰ ਸਤਰਅੰਦਰ bool
__entity_slice_used(ਬਣਤਰ sched_entity *se, ਪੂਰਨ min_nr_tasks)
{
	u64 rtime = se->ਜੋੜ_exec_runtime - se->prev_ਜੋੜ_exec_runtime;
	u64 ਟੁਕੜਾ = se->ਟੁਕੜਾ;

	ਵਾਪਸ (rtime * min_nr_tasks > ਟੁਕੜਾ);
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MIN_NR_TASKS_DURING_FORCEIDLE	2
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ task_tick_core(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *curr)
{
	ਜੇ (!sched_core_enabled(rq))
		ਵਾਪਸ;

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
	ਜੇ (rq->core->core_forceidle_ਗਿਣਤੀ && rq->cfs.h_nr_queued == 1 &&
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
ਸਥਿਰ ਖਾਲੀ se_fi_update(ਅਚਲ ਬਣਤਰ sched_entity *se, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ fi_seq,
			 bool forceidle)
{
	for_each_sched_entity(se) {
		ਬਣਤਰ cfs_rq *cfs_rq = cfs_rq_ਦਾ(se);

		ਜੇ (forceidle) {
			ਜੇ (cfs_rq->forceidle_seq == fi_seq)
				ਰੋਕੋ;
			cfs_rq->forceidle_seq = fi_seq;
		}

		cfs_rq->zero_vruntime_fi = cfs_rq->zero_vruntime;
	}
}

ਖਾਲੀ task_vruntime_update(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, bool ਵਿੱਚ_fi)
{
	ਬਣਤਰ sched_entity *se = &p->se;

	ਜੇ (p->sched_class != &fair_sched_class)
		ਵਾਪਸ;

	se_fi_update(se, rq->core->core_forceidle_seq, ਵਿੱਚ_fi);
}

bool cfs_prio_less(ਅਚਲ ਬਣਤਰ task_struct *a, ਅਚਲ ਬਣਤਰ task_struct *b,
			bool ਵਿੱਚ_fi)
{
	ਬਣਤਰ rq *rq = task_rq(a);
	ਅਚਲ ਬਣਤਰ sched_entity *sea = &a->se;
	ਅਚਲ ਬਣਤਰ sched_entity *seb = &b->se;
	ਬਣਤਰ cfs_rq *cfs_rqa;
	ਬਣਤਰ cfs_rq *cfs_rqb;
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

	ਵਾਪਸ delta > 0;
}

ਸਥਿਰ ਪੂਰਨ task_is_throttled_fair(ਬਣਤਰ task_struct *p, ਪੂਰਨ cpu)
{
	ਬਣਤਰ cfs_rq *cfs_rq;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_FAIR_ਸਮੂਹ_SCHED
	cfs_rq = tg_cfs_rq(task_ਸਮੂਹ(p), cpu);
#ਨਹੀਂਤਾਂ
	cfs_rq = &cpu_rq(cpu)->cfs;
#ਜੇ_ਅੰਤ
	ਵਾਪਸ throttled_hierarchy(cfs_rq);
}
#ਨਹੀਂਤਾਂ /* !CONFIG_SCHED_CORE: */
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ task_tick_core(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *curr) {}
#ਜੇ_ਅੰਤ /* !CONFIG_SCHED_CORE */

/*
 * scheduler tick hitting a task of our scheduling class.
 *
 * NOTE: This function can be called remotely by the tick offload that
 * goes along full dynticks. Therefore no local assumption can be made
 * and everything must be accessed through the @rq and @curr passed in
 * parameters.
 */
ਸਥਿਰ ਖਾਲੀ task_tick_fair(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *curr, ਪੂਰਨ queued)
{
	ਬਣਤਰ sched_entity *se = &curr->se;

	ਜੇ (se->on_rq) {
		ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਭਾਰ = NICE_0_ਲੱਦੋ;
		ਬਣਤਰ cfs_rq *cfs_rq;

		for_each_sched_entity(se) {
			cfs_rq = cfs_rq_ਦਾ(se);
			entity_tick(cfs_rq, se, queued);

			ਭਾਰ = __calc_prop_weight(cfs_rq, se, ਭਾਰ);
		}

		se = &curr->se;
		reweight_eevdf(cfs_rq, se, ਭਾਰ, se->on_rq);
	}

	ਜੇ (queued)
		ਵਾਪਸ;

	ਜੇ (static_branch_unlikely(&sched_numa_balancing))
		task_tick_numa(rq, curr);

	task_tick_cache(rq, curr);

	update_misfit_ਸਥਿਤੀ(curr, rq);
	check_update_overutilized_ਸਥਿਤੀ(task_rq(curr));

	task_tick_core(rq, curr);
}

/*
 * called on fork with the child task as argument from the parent's context
 *  - child not yet on the tasklist
 *  - preemption disabled
 */
ਸਥਿਰ ਖਾਲੀ task_fork_fair(ਬਣਤਰ task_struct *p)
{
	ਸੈੱਟ_task_max_allowed_ਸਮਰੱਥਾ(p);
}

/*
 * Priority of the task has changed. Check to see if we preempt
 * the current task.
 */
ਸਥਿਰ ਖਾਲੀ
prio_ਬਦਲਿਆ_fair(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, u64 oldprio)
{
	ਜੇ (!task_on_rq_queued(p))
		ਵਾਪਸ;

	ਜੇ (p->prio == oldprio)
		ਵਾਪਸ;

	ਜੇ (rq->cfs.h_nr_queued == 1)
		ਵਾਪਸ;

	/*
	 * Reschedule if we are currently running on this runqueue and
	 * our priority decreased, or if we are not currently running on
	 * this runqueue and our priority is higher than the current's
	 */
	ਜੇ (task_current_donor(rq, p)) {
		ਜੇ (p->prio > oldprio)
			resched_curr(rq);
	} ਨਹੀਂਤਾਂ {
		wakeup_preempt(rq, p, 0);
	}
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_FAIR_ਸਮੂਹ_SCHED
/*
 * Propagate the changes of the sched_entity across the tg tree to make it
 * visible to the root
 */
ਸਥਿਰ ਖਾਲੀ propagate_entity_cfs_rq(ਬਣਤਰ sched_entity *se)
{
	ਬਣਤਰ cfs_rq *cfs_rq = cfs_rq_ਦਾ(se);

	/*
	 * If a task gets attached to this cfs_rq and before being queued,
	 * it gets migrated to another CPU due to reasons like affinity
	 * change, make sure this cfs_rq stays on leaf cfs_rq list to have
	 * that removed load decayed or it can cause faireness problem.
	 */
	ਜੇ (!cfs_rq_pelt_clock_throttled(cfs_rq))
		list_ਜੋੜੋ_ਪੱਤੀ_cfs_rq(cfs_rq);

	/* Start to propagate at parent */
	se = se->ਮਾਪਾ;

	for_each_sched_entity(se) {
		cfs_rq = cfs_rq_ਦਾ(se);

		update_ਲੱਦੋ_avg(cfs_rq, se, UPDATE_TG);

		ਜੇ (!cfs_rq_pelt_clock_throttled(cfs_rq))
			list_ਜੋੜੋ_ਪੱਤੀ_cfs_rq(cfs_rq);
	}

	assert_list_ਪੱਤੀ_cfs_rq(rq_ਦਾ(cfs_rq));
}
#ਨਹੀਂਤਾਂ /* !CONFIG_FAIR_GROUP_SCHED: */
ਸਥਿਰ ਖਾਲੀ propagate_entity_cfs_rq(ਬਣਤਰ sched_entity *se) { }
#ਜੇ_ਅੰਤ /* !CONFIG_FAIR_GROUP_SCHED */

ਸਥਿਰ ਖਾਲੀ detach_entity_cfs_rq(ਬਣਤਰ sched_entity *se)
{
	ਬਣਤਰ cfs_rq *cfs_rq = cfs_rq_ਦਾ(se);

	/*
	 * In case the task sched_avg hasn't been attached:
	 * - A forked task which hasn't been woken up by wake_up_new_task().
	 * - A task which has been woken up by try_to_wake_up() but is
	 *   waiting for actually being woken up by sched_ttwu_pending().
	 */
	ਜੇ (!se->avg.ਆਖਰੀ_update_ਸਮਾਂ)
		ਵਾਪਸ;

	/* Catch up with the cfs_rq and remove our load when we leave */
	update_ਲੱਦੋ_avg(cfs_rq, se, 0);
	detach_entity_ਲੱਦੋ_avg(cfs_rq, se);
	update_tg_ਲੱਦੋ_avg(cfs_rq);
	propagate_entity_cfs_rq(se);
}

ਸਥਿਰ ਖਾਲੀ attach_entity_cfs_rq(ਬਣਤਰ sched_entity *se)
{
	ਬਣਤਰ cfs_rq *cfs_rq = cfs_rq_ਦਾ(se);

	/* Synchronize entity with its cfs_rq */
	update_ਲੱਦੋ_avg(cfs_rq, se, sched_feat(ATTACH_AGE_ਲੱਦੋ) ? 0 : SKIP_AGE_ਲੱਦੋ);
	attach_entity_ਲੱਦੋ_avg(cfs_rq, se);
	update_tg_ਲੱਦੋ_avg(cfs_rq);
	propagate_entity_cfs_rq(se);
}

ਸਥਿਰ ਖਾਲੀ detach_task_cfs_rq(ਬਣਤਰ task_struct *p)
{
	ਬਣਤਰ sched_entity *se = &p->se;

	detach_entity_cfs_rq(se);
}

ਸਥਿਰ ਖਾਲੀ attach_task_cfs_rq(ਬਣਤਰ task_struct *p)
{
	ਬਣਤਰ sched_entity *se = &p->se;

	attach_entity_cfs_rq(se);
}

ਸਥਿਰ ਖਾਲੀ switching_from_fair(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p)
{
	ਜੇ (p->se.sched_delayed)
		ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_task(rq, p, ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_SLEEP | ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_DELAYED | ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_NOCLOCK);
}

ਸਥਿਰ ਖਾਲੀ switched_from_fair(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p)
{
	detach_task_cfs_rq(p);
}

ਸਥਿਰ ਖਾਲੀ switched_ਮੰਜ਼ਿਲ_fair(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p)
{
	WARN_ON_ONCE(p->se.sched_delayed);

	attach_task_cfs_rq(p);

	ਸੈੱਟ_task_max_allowed_ਸਮਰੱਥਾ(p);

	ਜੇ (task_on_rq_queued(p)) {
		/*
		 * We were most likely switched from sched_rt, so
		 * kick off the schedule if running, otherwise just see
		 * if we can still preempt the current task.
		 */
		ਜੇ (task_current_donor(rq, p))
			resched_curr(rq);
		ਨਹੀਂਤਾਂ
			wakeup_preempt(rq, p, 0);
	}
}

ਸਥਿਰ ਖਾਲੀ ਸੈੱਟ_ਅਗਲਾ_task_fair(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *p, ਗਿਣਤੀ snt_e type)
{
	ਬਣਤਰ sched_entity *se = &p->se;
	ਬਣਤਰ cfs_rq *cfs_rq = &rq->cfs;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਭਾਰ = NICE_0_ਲੱਦੋ;
	bool ਪਹਿਲਾ = type == SNT_PICK;
	bool throttled = false;
	bool on_rq = se->on_rq;

	ਜੇ (type == SNT_REPICK)
		ਜਾਓ repick;

	clear_buddies(cfs_rq, se);

	ਜੇ (on_rq)
		__dequeue_entity(cfs_rq, se);

	for_each_sched_entity(se) {
		cfs_rq = cfs_rq_ਦਾ(se);

		ਜੇ (!IS_ENABLED(CONFIG_FAIR_ਸਮੂਹ_SCHED) ||
		    !ਪਹਿਲਾ || !cfs_rq->h_curr)
			ਸੈੱਟ_ਅਗਲਾ_entity(cfs_rq, se);

		/* ensure bandwidth has been allocated on our new cfs_rq */
		throttled |= account_cfs_rq_runtime(cfs_rq, 0);

		ਜੇ (on_rq)
			ਭਾਰ = __calc_prop_weight(cfs_rq, se, ਭਾਰ);
	}

	ਜੇ (throttled)
		task_throttle_ਤਿਆਰੀ_work(p);

	se = &p->se;
	cfs_rq->curr = se;

	ਜੇ (on_rq) {
		reweight_eevdf(cfs_rq, se, ਭਾਰ, se->on_rq);
		ਜੇ (ਪਹਿਲਾ)
			ਸੈੱਟ_protect_ਟੁਕੜਾ(cfs_rq, se);
	}

	ਜੇ (task_on_rq_queued(p)) {
		/*
		 * Move the next running task to the front of the list, so our
		 * cfs_tasks list becomes MRU one.
		 */
		list_move(&se->ਸਮੂਹ_ਗੰਢ, &rq->cfs_tasks);
	}
	ਜੇ (!ਪਹਿਲਾ)
		ਵਾਪਸ;

	WARN_ON_ONCE(se->sched_delayed);

	update_misfit_ਸਥਿਤੀ(p, rq);
	sched_fair_update_stop_tick(rq, p);

repick:
	/*
	 * A same-task repick skips put_prev_task_fair(), but
	 * pick_task_fair() refreshed the entity hrtick_start_fair() reads
	 * before selecting it again. rq->cfs.curr identifies that entity,
	 * including with group scheduling.
	 */
	ਜੇ (hrtick_enabled_fair(rq))
		hrtick_ਸ਼ੁਰੂ_fair(rq, p);
}

ਖਾਲੀ init_cfs_rq(ਬਣਤਰ cfs_rq *cfs_rq)
{
	cfs_rq->tasks_timeline = RB_ਜੜ੍ਹ_CACHED;
	cfs_rq->zero_vruntime = (u64)(-(1LL << 20));
	raw_spin_ਤਾਲਾ_init(&cfs_rq->removed.ਤਾਲਾ);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_FAIR_ਸਮੂਹ_SCHED
ਸਥਿਰ ਖਾਲੀ task_change_ਸਮੂਹ_fair(ਬਣਤਰ task_struct *p)
{
	/*
	 * We couldn't detach or attach a forked task which
	 * hasn't been woken up by wake_up_new_task().
	 */
	ਜੇ (READ_ONCE(p->__state) == TASK_NEW)
		ਵਾਪਸ;

	detach_task_cfs_rq(p);

	/* Tell se's cfs_rq has been changed -- migrated */
	p->se.avg.ਆਖਰੀ_update_ਸਮਾਂ = 0;
	ਸੈੱਟ_task_rq(p, task_cpu(p));
	attach_task_cfs_rq(p);
}

ਖਾਲੀ free_fair_sched_ਸਮੂਹ(ਬਣਤਰ task_ਸਮੂਹ *tg)
{
	free_percpu(tg->cfs_rq);
}

ਪੂਰਨ alloc_fair_sched_ਸਮੂਹ(ਬਣਤਰ task_ਸਮੂਹ *tg, ਬਣਤਰ task_ਸਮੂਹ *ਮਾਪਾ)
{
	ਬਣਤਰ cfs_tg_state __percpu *state;
	ਬਣਤਰ sched_entity *se;
	ਬਣਤਰ cfs_rq *cfs_rq;
	ਪੂਰਨ i;

	state = alloc_percpu_gfp(ਬਣਤਰ cfs_tg_state, GFP_KERNEL);
	ਜੇ (!state)
		ਜਾਓ err;

	tg->cfs_rq = &state->cfs_rq;
	tg->shares = NICE_0_ਲੱਦੋ;

	init_cfs_bandwidth(tg_cfs_bandwidth(tg), tg_cfs_bandwidth(ਮਾਪਾ));

	for_each_possible_cpu(i) {
		cfs_rq = tg_cfs_rq(tg, i);
		ਜੇ (!cfs_rq)
			ਜਾਓ err;

		se = tg_se(tg, i);
		init_cfs_rq(cfs_rq);
		init_tg_cfs_ਇੰਦਰਾਜ(tg, cfs_rq, se, i, tg_se(ਮਾਪਾ, i));
		init_entity_runnable_ਔਸਤਨ(se);
	}

	ਵਾਪਸ 1;

err:
	ਵਾਪਸ 0;
}

ਖਾਲੀ online_fair_sched_ਸਮੂਹ(ਬਣਤਰ task_ਸਮੂਹ *tg)
{
	ਬਣਤਰ sched_entity *se;
	ਬਣਤਰ rq_flags rf;
	ਬਣਤਰ rq *rq;
	ਪੂਰਨ i;

	for_each_possible_cpu(i) {
		rq = cpu_rq(i);
		se = tg_se(tg, i);
		rq_ਤਾਲਾ_irq(rq, &rf);
		update_rq_clock(rq);
		attach_entity_cfs_rq(se);
		sync_throttle(tg, i);
		rq_unlock_irq(rq, &rf);
	}
}

ਖਾਲੀ unregister_fair_sched_ਸਮੂਹ(ਬਣਤਰ task_ਸਮੂਹ *tg)
{
	ਪੂਰਨ cpu;

	destroy_cfs_bandwidth(tg_cfs_bandwidth(tg));

	for_each_possible_cpu(cpu) {
		ਬਣਤਰ cfs_rq *cfs_rq = tg_cfs_rq(tg, cpu);
		ਬਣਤਰ sched_entity *se = tg_se(tg, cpu);
		ਬਣਤਰ rq *rq = cpu_rq(cpu);

		ਜੇ (se)
			remove_entity_ਲੱਦੋ_avg(se);

		/*
		 * Only empty task groups can be destroyed; so we can speculatively
		 * check on_list without danger of it being re-added.
		 */
		ਜੇ (cfs_rq->on_list) {
			guard(rq_ਤਾਲਾ_irqsave)(rq);
			list_del_ਪੱਤੀ_cfs_rq(cfs_rq);
		}
	}
}

ਖਾਲੀ init_tg_cfs_ਇੰਦਰਾਜ(ਬਣਤਰ task_ਸਮੂਹ *tg, ਬਣਤਰ cfs_rq *cfs_rq,
			ਬਣਤਰ sched_entity *se, ਪੂਰਨ cpu,
			ਬਣਤਰ sched_entity *ਮਾਪਾ)
{
	ਬਣਤਰ rq *rq = cpu_rq(cpu);

	cfs_rq->tg = tg;
	cfs_rq->rq = rq;
	init_cfs_rq_runtime(cfs_rq);

	/* se could be NULL for root_task_group */
	ਜੇ (!se)
		ਵਾਪਸ;

	ਜੇ (!ਮਾਪਾ) {
		se->cfs_rq = &rq->cfs;
		se->ਡੂੰਘਾਈ = 0;
	} ਨਹੀਂਤਾਂ {
		se->cfs_rq = ਮਾਪਾ->my_q;
		se->ਡੂੰਘਾਈ = ਮਾਪਾ->ਡੂੰਘਾਈ + 1;
	}

	se->my_q = cfs_rq;
	/* guarantee group entities always have weight */
	update_ਲੱਦੋ_ਸੈੱਟ(&se->ਲੱਦੋ, NICE_0_ਲੱਦੋ);
	se->ਮਾਪਾ = ਮਾਪਾ;
}

ਸਥਿਰ DEFINE_MUTEX(shares_mutex);

ਸਥਿਰ ਪੂਰਨ __sched_group_set_shares(ਬਣਤਰ task_ਸਮੂਹ *tg, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ shares)
{
	ਪੂਰਨ i;

	lockdep_assert_held(&shares_mutex);

	/*
	 * We can't change the weight of the root cgroup.
	 */
	ਜੇ (is_ਜੜ੍ਹ_task_ਸਮੂਹ(tg))
		ਵਾਪਸ -EINVAL;

	shares = clamp(shares, scale_ਲੱਦੋ(MIN_SHARES), scale_ਲੱਦੋ(MAX_SHARES));

	ਜੇ (tg->shares == shares)
		ਵਾਪਸ 0;

	tg->shares = shares;
	for_each_possible_cpu(i) {
		ਬਣਤਰ rq *rq = cpu_rq(i);
		ਬਣਤਰ sched_entity *se = tg_se(tg, i);
		ਬਣਤਰ rq_flags rf;

		/* Propagate contribution to hierarchy */
		rq_ਤਾਲਾ_irqsave(rq, &rf);
		update_rq_clock(rq);
		for_each_sched_entity(se) {
			update_ਲੱਦੋ_avg(cfs_rq_ਦਾ(se), se, UPDATE_TG);
			update_cfs_ਸਮੂਹ(se);
		}
		rq_unlock_irqrestore(rq, &rf);
	}

	ਵਾਪਸ 0;
}

ਪੂਰਨ sched_ਸਮੂਹ_ਸੈੱਟ_shares(ਬਣਤਰ task_ਸਮੂਹ *tg, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ shares)
{
	ਪੂਰਨ ret;

	mutex_ਤਾਲਾ(&shares_mutex);
	ਜੇ (tg_is_idle(tg))
		ret = -EINVAL;
	ਨਹੀਂਤਾਂ
		ret = __sched_group_set_shares(tg, shares);
	mutex_unlock(&shares_mutex);

	ਵਾਪਸ ret;
}

ਪੂਰਨ sched_ਸਮੂਹ_ਸੈੱਟ_idle(ਬਣਤਰ task_ਸਮੂਹ *tg, ਲੰਮਾ idle)
{
	ਪੂਰਨ i;

	ਜੇ (tg == &ਜੜ੍ਹ_task_ਸਮੂਹ)
		ਵਾਪਸ -EINVAL;

	ਜੇ (idle < 0 || idle > 1)
		ਵਾਪਸ -EINVAL;

	mutex_ਤਾਲਾ(&shares_mutex);

	ਜੇ (tg->idle == idle) {
		mutex_unlock(&shares_mutex);
		ਵਾਪਸ 0;
	}

	tg->idle = idle;

	for_each_possible_cpu(i) {
		ਬਣਤਰ rq *rq = cpu_rq(i);
		ਬਣਤਰ sched_entity *se = tg_se(tg, i);
		ਬਣਤਰ cfs_rq *grp_cfs_rq = tg_cfs_rq(tg, i);
		bool was_idle = cfs_rq_is_idle(grp_cfs_rq);
		ਲੰਮਾ idle_task_delta;
		ਬਣਤਰ rq_flags rf;

		rq_ਤਾਲਾ_irqsave(rq, &rf);

		grp_cfs_rq->idle = idle;
		ਜੇ (WARN_ON_ONCE(was_idle == cfs_rq_is_idle(grp_cfs_rq)))
			ਜਾਓ ਅਗਲਾ_cpu;

		idle_task_delta = grp_cfs_rq->h_nr_queued -
				  grp_cfs_rq->h_nr_idle;
		ਜੇ (!cfs_rq_is_idle(grp_cfs_rq))
			idle_task_delta *= -1;

		for_each_sched_entity(se) {
			ਬਣਤਰ cfs_rq *cfs_rq = cfs_rq_ਦਾ(se);

			ਜੇ (!se->on_rq)
				ਰੋਕੋ;

			cfs_rq->h_nr_idle += idle_task_delta;

			/* Already accounted at parent level and above. */
			ਜੇ (cfs_rq_is_idle(cfs_rq))
				ਰੋਕੋ;
		}

ਅਗਲਾ_cpu:
		rq_unlock_irqrestore(rq, &rf);
	}

	/* Idle groups have minimum weight. */
	ਜੇ (tg_is_idle(tg))
		__sched_group_set_shares(tg, scale_ਲੱਦੋ(ਭਾਰ_IDLEPRIO));
	ਨਹੀਂਤਾਂ
		__sched_group_set_shares(tg, NICE_0_ਲੱਦੋ);

	mutex_unlock(&shares_mutex);
	ਵਾਪਸ 0;
}

#ਜੇ_ਅੰਤ /* CONFIG_FAIR_GROUP_SCHED */


ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_rr_interval_fair(ਬਣਤਰ rq *rq, ਬਣਤਰ task_struct *task)
{
	ਬਣਤਰ sched_entity *se = &task->se;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ rr_interval = 0;

	/*
	 * Time slice is 0 for SCHED_OTHER tasks that are on an otherwise
	 * idle runqueue:
	 */
	ਜੇ (rq->cfs.ਲੱਦੋ.ਭਾਰ)
		rr_interval = NS_ਮੰਜ਼ਿਲ_JIFFIES(se->ਟੁਕੜਾ);

	ਵਾਪਸ rr_interval;
}

/*
 * All the scheduling class methods:
 */
DEFINE_SCHED_CLASS(fair) = {
	.ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_task		= ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_task_fair,
	.ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_task		= ਕਤਾਰ_ਤੋਂ_ਕੱਢੋ_task_fair,
	.yield_task		= yield_task_fair,
	.yield_ਮੰਜ਼ਿਲ_task		= yield_ਮੰਜ਼ਿਲ_task_fair,

	.wakeup_preempt		= wakeup_preempt_fair,

	.pick_task		= pick_task_fair,
	.put_prev_task		= put_prev_task_fair,
	.ਸੈੱਟ_ਅਗਲਾ_task          = ਸੈੱਟ_ਅਗਲਾ_task_fair,

	.select_task_rq		= select_task_rq_fair,
	.migrate_task_rq	= migrate_task_rq_fair,

	.rq_online		= rq_online_fair,
	.rq_offline		= rq_offline_fair,

	.task_dead		= task_dead_fair,
	.ਸੈੱਟ_cpus_allowed	= ਸੈੱਟ_cpus_allowed_fair,

	.task_tick		= task_tick_fair,
	.task_fork		= task_fork_fair,

	.reweight_task		= reweight_task_fair,
	.prio_ਬਦਲਿਆ		= prio_ਬਦਲਿਆ_fair,
	.switching_from		= switching_from_fair,
	.switched_from		= switched_from_fair,
	.switched_ਮੰਜ਼ਿਲ		= switched_ਮੰਜ਼ਿਲ_fair,

	.get_rr_interval	= get_rr_interval_fair,

	.update_curr		= update_curr_fair,

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_FAIR_ਸਮੂਹ_SCHED
	.task_change_ਸਮੂਹ	= task_change_ਸਮੂਹ_fair,
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_SCHED_CORE
	.task_is_throttled	= task_is_throttled_fair,
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_UCLAMP_TASK
	.uclamp_enabled		= 1,
#ਜੇ_ਅੰਤ
};

ਖਾਲੀ print_cfs_stats(ਬਣਤਰ seq_file *m, ਪੂਰਨ cpu)
{
	ਬਣਤਰ cfs_rq *cfs_rq, *pos;

	rcu_read_ਤਾਲਾ();
	for_each_ਪੱਤੀ_cfs_rq_ਸੁਰੱਖਿਅਤ(cpu_rq(cpu), cfs_rq, pos)
		print_cfs_rq(m, cpu, cfs_rq);
	rcu_read_unlock();
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_NUMA_BALANCING
ਖਾਲੀ show_numa_stats(ਬਣਤਰ task_struct *p, ਬਣਤਰ seq_file *m)
{
	ਪੂਰਨ ਗੰਢ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ tsf = 0, tpf = 0, gsf = 0, gpf = 0;
	ਬਣਤਰ numa_ਸਮੂਹ *ng;

	rcu_read_ਤਾਲਾ();
	ng = rcu_dereference_all(p->numa_ਸਮੂਹ);
	for_each_online_ਗੰਢ(ਗੰਢ) {
		ਜੇ (p->numa_ਨੁਕਸ) {
			tsf = p->numa_ਨੁਕਸ[task_ਨੁਕਸ_idx(NUMA_MEM, ਗੰਢ, 0)];
			tpf = p->numa_ਨੁਕਸ[task_ਨੁਕਸ_idx(NUMA_MEM, ਗੰਢ, 1)];
		}
		ਜੇ (ng) {
			gsf = ng->ਨੁਕਸ[task_ਨੁਕਸ_idx(NUMA_MEM, ਗੰਢ, 0)];
			gpf = ng->ਨੁਕਸ[task_ਨੁਕਸ_idx(NUMA_MEM, ਗੰਢ, 1)];
		}
		print_numa_stats(m, ਗੰਢ, tsf, tpf, gsf, gpf);
	}
	rcu_read_unlock();
}
#ਜੇ_ਅੰਤ /* CONFIG_NUMA_BALANCING */

__init ਖਾਲੀ init_sched_fair_class(ਖਾਲੀ)
{
	ਪੂਰਨ i;

	for_each_possible_cpu(i) {
		zalloc_cpumask_var_ਗੰਢ(&ਪ੍ਰਤੀ_cpu(ਲੱਦੋ_balance_ਨਕਾਬ, i), GFP_KERNEL, cpu_ਮੰਜ਼ਿਲ_ਗੰਢ(i));
		zalloc_cpumask_var_ਗੰਢ(&ਪ੍ਰਤੀ_cpu(select_rq_ਨਕਾਬ,    i), GFP_KERNEL, cpu_ਮੰਜ਼ਿਲ_ਗੰਢ(i));
		zalloc_cpumask_var_ਗੰਢ(&ਪ੍ਰਤੀ_cpu(should_we_balance_tmpmask, i),
					GFP_KERNEL, cpu_ਮੰਜ਼ਿਲ_ਗੰਢ(i));

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_CFS_BANDWIDTH
		INIT_CSD(&cpu_rq(i)->cfsb_csd, __cfsb_csd_unthrottle, cpu_rq(i));
		INIT_LIST_ਸਿਰ(&cpu_rq(i)->cfsb_csd_list);
#ਜੇ_ਅੰਤ
	}

	open_softirq(SCHED_SOFTIRQ, sched_balance_softirq);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_NO_HZ_COMMON
	nohz.ਅਗਲਾ_balance = jiffies;
	nohz.ਅਗਲਾ_blocked = jiffies;
	zalloc_cpumask_var(&nohz.idle_cpus_ਨਕਾਬ, GFP_NOWAIT);
#ਜੇ_ਅੰਤ
}
