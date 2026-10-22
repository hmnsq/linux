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
#അന്തര്ഭാവ <linux/energy_model.h>
#അന്തര്ഭാവ <linux/mmap_lock.h>
#അന്തര്ഭാവ <linux/hugetlb_inline.h>
#അന്തര്ഭാവ <linux/jiffies.h>
#അന്തര്ഭാവ <linux/math.h>
#അന്തര്ഭാവ <linux/mm_api.h>
#അന്തര്ഭാവ <linux/highmem.h>
#അന്തര്ഭാവ <linux/hrtimer.h>
#അന്തര്ഭാവ <linux/hrtimer_bases.h>
#അന്തര്ഭാവ <linux/spinlock_api.h>
#അന്തര്ഭാവ <linux/cpumask_api.h>
#അന്തര്ഭാവ <linux/lockdep_api.h>
#അന്തര്ഭാവ <linux/softirq.h>
#അന്തര്ഭാവ <linux/refcount_api.h>
#അന്തര്ഭാവ <linux/topology.h>
#അന്തര്ഭാവ <linux/sched/clock.h>
#അന്തര്ഭാവ <linux/sched/cond_resched.h>
#അന്തര്ഭാവ <linux/sched/cputime.h>
#അന്തര്ഭാവ <linux/sched/isolation.h>
#അന്തര്ഭാവ <linux/sched/nohz.h>
#അന്തര്ഭാവ <linux/sched/prio.h>
#അന്തര്ഭാവ <linux/static_call.h>

#അന്തര്ഭാവ <linux/cpuidle.h>
#അന്തര്ഭാവ <linux/interrupt.h>
#അന്തര്ഭാവ <linux/memory-tiers.h>
#അന്തര്ഭാവ <linux/mempolicy.h>
#അന്തര്ഭാവ <linux/mutex_api.h>
#അന്തര്ഭാവ <linux/profile.h>
#അന്തര്ഭാവ <linux/psi.h>
#അന്തര്ഭാവ <linux/ratelimit.h>
#അന്തര്ഭാവ <linux/task_work.h>
#അന്തര്ഭാവ <linux/rbtree_augmented.h>

#അന്തര്ഭാവ <asm/switch_to.h>

#അന്തര്ഭാവ <uapi/linux/sched/types.h>

#അന്തര്ഭാവ "sched.h"
#അന്തര്ഭാവ "stats.h"
#അന്തര്ഭാവ "autogroup.h"

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
unsigned പൂർണ്ണസംഖ്യ sysctl_sched_tunable_scaling = SCHED_TUNABLESCALING_LOG;

/*
 * Default base time slice (request size r_i) for SCHED_NORMAL/SCHED_BATCH:
 *
 * Under EEVDF this is the request size used to compute the virtual
 * deadline; see update_deadline().
 *
 * (default: 0.70 msec * (1 + ilog(ncpus)), units: nanoseconds)
 */
unsigned പൂർണ്ണസംഖ്യ sysctl_sched_base_slice			= 700000ULL;
സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ normalized_sysctl_sched_base_slice	= 700000ULL;

__read_mostly unsigned പൂർണ്ണസംഖ്യ sysctl_sched_migration_cost	= 500000UL;

സ്ഥിതം പൂർണ്ണസംഖ്യ __init setup_sched_thermal_decay_shift(അക്ഷരം *str)
{
	pr_warn("Ignoring the deprecated sched_thermal_decay_shift= option\n");
	തിരികെ 1;
}
__setup("sched_thermal_decay_shift=", setup_sched_thermal_decay_shift);

/*
 * For asym packing, by default the lower numbered CPU has higher priority.
 */
പൂർണ്ണസംഖ്യ __weak arch_asym_cpu_priority(പൂർണ്ണസംഖ്യ cpu)
{
	തിരികെ -cpu;
}

/*
 * The margin used when comparing utilization with CPU capacity.
 *
 * (default: ~20%)
 */
#define fits_capacity(cap, max)	((cap) * 1280 < (max) * 1024)

/*
 * The margin used when comparing CPU capacities.
 * is 'cap1' noticeably greater than 'cap2'
 *
 * (default: ~5%)
 */
#define capacity_greater(cap1, cap2) ((cap1) * 1024 > (cap2) * 1078)

#ifdef CONFIG_CFS_BANDWIDTH
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
സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ sysctl_sched_cfs_bandwidth_slice		= 5000UL;
#endif

#ifdef CONFIG_NUMA_BALANCING
/* Restrict the NUMA promotion throughput (MB/s) for each target node. */
സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ sysctl_numa_balancing_promote_rate_limit = 65536;
#endif

#ifdef CONFIG_SYSCTL
സ്ഥിതം സ്ഥിരം ഘടന ctl_table sched_fair_sysctls[] = {
#ifdef CONFIG_CFS_BANDWIDTH
	{
		.procname       = "sched_cfs_bandwidth_slice_us",
		.data           = &sysctl_sched_cfs_bandwidth_slice,
		.maxlen         = വലുപ്പം(unsigned പൂർണ്ണസംഖ്യ),
		.mode           = 0644,
		.proc_handler   = proc_dointvec_minmax,
		.extra1         = SYSCTL_ONE,
	},
#endif
#ifdef CONFIG_NUMA_BALANCING
	{
		.procname	= "numa_balancing_promote_rate_limit_MBps",
		.data		= &sysctl_numa_balancing_promote_rate_limit,
		.maxlen		= വലുപ്പം(unsigned പൂർണ്ണസംഖ്യ),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= SYSCTL_ZERO,
	},
#endif /* CONFIG_NUMA_BALANCING */
};

സ്ഥിതം പൂർണ്ണസംഖ്യ __init sched_fair_sysctl_init(ശൂന്യം)
{
	register_sysctl_init("kernel", sched_fair_sysctls);
	തിരികെ 0;
}
late_initcall(sched_fair_sysctl_init);
#endif /* CONFIG_SYSCTL */

സ്ഥിതം വരിയിൽ ശൂന്യം update_load_add(ഘടന load_weight *lw, unsigned long inc)
{
	lw->weight += inc;
	lw->inv_weight = 0;
}

സ്ഥിതം വരിയിൽ ശൂന്യം update_load_sub(ഘടന load_weight *lw, unsigned long dec)
{
	lw->weight -= dec;
	lw->inv_weight = 0;
}

സ്ഥിതം വരിയിൽ ശൂന്യം update_load_set(ഘടന load_weight *lw, unsigned long w)
{
	lw->weight = w;
	lw->inv_weight = 0;
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
സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ get_update_sysctl_factor(ശൂന്യം)
{
	unsigned പൂർണ്ണസംഖ്യ cpus = min_t(unsigned പൂർണ്ണസംഖ്യ, അക്കം_online_cpus(), 8);
	unsigned പൂർണ്ണസംഖ്യ factor;

	തിരഞ്ഞെടുപ്പ് (sysctl_sched_tunable_scaling) {
	അവസ്ഥ SCHED_TUNABLESCALING_NONE:
		factor = 1;
		നിർത്തുക;
	അവസ്ഥ SCHED_TUNABLESCALING_LINEAR:
		factor = cpus;
		നിർത്തുക;
	അവസ്ഥ SCHED_TUNABLESCALING_LOG:
	സ്ഥിരസ്ഥിതി:
		factor = 1 + ilog2(cpus);
		നിർത്തുക;
	}

	തിരികെ factor;
}

സ്ഥിതം ശൂന്യം update_sysctl(ശൂന്യം)
{
	unsigned പൂർണ്ണസംഖ്യ factor = get_update_sysctl_factor();

#define SET_SYSCTL(name) \
	(sysctl_##name = (factor) * normalized_sysctl_##name)
	SET_SYSCTL(sched_base_slice);
#undef SET_SYSCTL
}

ശൂന്യം __init sched_init_granularity(ശൂന്യം)
{
	update_sysctl();
}

#ifndef CONFIG_64BIT
#define WMULT_CONST	(~0U)
#define WMULT_SHIFT	32

സ്ഥിതം ശൂന്യം __update_inv_weight(ഘടന load_weight *lw)
{
	unsigned long w;

	എങ്കിൽ (likely(lw->inv_weight))
		തിരികെ;

	w = scale_load_down(lw->weight);

	എങ്കിൽ (BITS_PER_LONG > 32 && unlikely(w >= WMULT_CONST))
		lw->inv_weight = 1;
	അല്ലെങ്കിൽ എങ്കിൽ (unlikely(!w))
		lw->inv_weight = WMULT_CONST;
	അല്ലെങ്കിൽ
		lw->inv_weight = WMULT_CONST / w;
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
സ്ഥിതം u64 __calc_delta(u64 delta_exec, unsigned long weight, ഘടന load_weight *lw)
{
	u64 fact = scale_load_down(weight);
	u32 fact_hi = (u32)(fact >> 32);
	പൂർണ്ണസംഖ്യ shift = WMULT_SHIFT;
	പൂർണ്ണസംഖ്യ fs;

	__update_inv_weight(lw);

	എങ്കിൽ (unlikely(fact_hi)) {
		fs = fls(fact_hi);
		shift -= fs;
		fact >>= fs;
	}

	fact = mul_u32_u32(fact, lw->inv_weight);

	fact_hi = (u32)(fact >> 32);
	എങ്കിൽ (fact_hi) {
		fs = fls(fact_hi);
		shift -= fs;
		fact >>= fs;
	}

	തിരികെ mul_u64_u32_shr(delta_exec, fact, shift);
}
#അല്ലെങ്കിൽ
സ്ഥിതം u64 __calc_delta(u64 delta_exec, unsigned long weight, ഘടന load_weight *lw)
{
	തിരികെ (delta_exec * weight) / lw->weight;
}
#endif

/*
 * delta /= w
 */
സ്ഥിതം വരിയിൽ u64 calc_delta_fair(u64 delta, ഘടന sched_entity *se)
{
	എങ്കിൽ (se->h_load.weight != NICE_0_LOAD)
		delta = __calc_delta(delta, NICE_0_LOAD, &se->h_load);

	തിരികെ delta;
}

സ്ഥിരം ഘടന sched_class fair_sched_class;

/**************************************************************
 * CFS operations on generic schedulable entities:
 */

#ifdef CONFIG_FAIR_GROUP_SCHED

/* Walk up scheduling entities hierarchy */
#define for_each_sched_entity(se) \
		വേണ്ടി (; se; se = se->parent)

സ്ഥിതം വരിയിൽ bool list_add_leaf_cfs_rq(ഘടന cfs_rq *cfs_rq)
{
	ഘടന rq *rq = rq_of(cfs_rq);
	പൂർണ്ണസംഖ്യ cpu = cpu_of(rq);

	എങ്കിൽ (cfs_rq->on_list)
		തിരികെ rq->tmp_alone_branch == &rq->leaf_cfs_rq_list;

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
	എങ്കിൽ (cfs_rq->tg->parent &&
	    tg_cfs_rq(cfs_rq->tg->parent, cpu)->on_list) {
		/*
		 * If parent is already on the list, we add the child
		 * just before. Thanks to circular linked property of
		 * the list, this means to put the child at the tail
		 * of the list that starts by parent.
		 */
		list_add_tail_rcu(&cfs_rq->leaf_cfs_rq_list,
			&(tg_cfs_rq(cfs_rq->tg->parent, cpu)->leaf_cfs_rq_list));
		/*
		 * The branch is now connected to its tree so we can
		 * reset tmp_alone_branch to the beginning of the
		 * list.
		 */
		rq->tmp_alone_branch = &rq->leaf_cfs_rq_list;
		തിരികെ true;
	}

	എങ്കിൽ (!cfs_rq->tg->parent) {
		/*
		 * cfs rq without parent should be put
		 * at the tail of the list.
		 */
		list_add_tail_rcu(&cfs_rq->leaf_cfs_rq_list,
			&rq->leaf_cfs_rq_list);
		/*
		 * We have reach the top of a tree so we can reset
		 * tmp_alone_branch to the beginning of the list.
		 */
		rq->tmp_alone_branch = &rq->leaf_cfs_rq_list;
		തിരികെ true;
	}

	/*
	 * The parent has not already been added so we want to
	 * make sure that it will be put after us.
	 * tmp_alone_branch points to the begin of the branch
	 * where we will add parent.
	 */
	list_add_rcu(&cfs_rq->leaf_cfs_rq_list, rq->tmp_alone_branch);
	/*
	 * update tmp_alone_branch to points to the new begin
	 * of the branch
	 */
	rq->tmp_alone_branch = &cfs_rq->leaf_cfs_rq_list;
	തിരികെ false;
}

സ്ഥിതം വരിയിൽ ശൂന്യം list_del_leaf_cfs_rq(ഘടന cfs_rq *cfs_rq)
{
	എങ്കിൽ (cfs_rq->on_list) {
		ഘടന rq *rq = rq_of(cfs_rq);

		/*
		 * With cfs_rq being unthrottled/throttled during an enqueue,
		 * it can happen the tmp_alone_branch points to the leaf that
		 * we finally want to delete. In this case, tmp_alone_branch moves
		 * to the prev element but it will point to rq->leaf_cfs_rq_list
		 * at the end of the enqueue.
		 */
		എങ്കിൽ (rq->tmp_alone_branch == &cfs_rq->leaf_cfs_rq_list)
			rq->tmp_alone_branch = cfs_rq->leaf_cfs_rq_list.prev;

		list_del_rcu(&cfs_rq->leaf_cfs_rq_list);
		cfs_rq->on_list = 0;
	}
}

സ്ഥിതം വരിയിൽ ശൂന്യം assert_list_leaf_cfs_rq(ഘടന rq *rq)
{
	WARN_ON_ONCE(rq->tmp_alone_branch != &rq->leaf_cfs_rq_list);
}

/* Iterate through all leaf cfs_rq's on a runqueue */
#define for_each_leaf_cfs_rq_safe(rq, cfs_rq, pos)			\
	list_for_each_entry_safe(cfs_rq, pos, &rq->leaf_cfs_rq_list,	\
				 leaf_cfs_rq_list)

/* Do the two (enqueued) entities belong to the same group ? */
സ്ഥിതം വരിയിൽ ഘടന cfs_rq *
is_same_group(ഘടന sched_entity *se, ഘടന sched_entity *pse)
{
	എങ്കിൽ (se->cfs_rq == pse->cfs_rq)
		തിരികെ se->cfs_rq;

	തിരികെ ശൂന്യ;
}

സ്ഥിതം വരിയിൽ ഘടന sched_entity *parent_entity(സ്ഥിരം ഘടന sched_entity *se)
{
	തിരികെ se->parent;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ tg_is_idle(ഘടന task_group *tg)
{
	തിരികെ tg->idle > 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cfs_rq_is_idle(ഘടന cfs_rq *cfs_rq)
{
	തിരികെ cfs_rq->idle > 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ se_is_idle(ഘടന sched_entity *se)
{
	എങ്കിൽ (entity_is_task(se))
		തിരികെ task_has_idle_policy(task_of(se));
	തിരികെ cfs_rq_is_idle(group_cfs_rq(se));
}

#അല്ലെങ്കിൽ /* !CONFIG_FAIR_GROUP_SCHED: */

#define for_each_sched_entity(se) \
		വേണ്ടി (; se; se = ശൂന്യ)

സ്ഥിതം വരിയിൽ bool list_add_leaf_cfs_rq(ഘടന cfs_rq *cfs_rq)
{
	തിരികെ true;
}

സ്ഥിതം വരിയിൽ ശൂന്യം list_del_leaf_cfs_rq(ഘടന cfs_rq *cfs_rq)
{
}

സ്ഥിതം വരിയിൽ ശൂന്യം assert_list_leaf_cfs_rq(ഘടന rq *rq)
{
}

#define for_each_leaf_cfs_rq_safe(rq, cfs_rq, pos)	\
		വേണ്ടി (cfs_rq = &rq->cfs, pos = ശൂന്യ; cfs_rq; cfs_rq = pos)

സ്ഥിതം വരിയിൽ ഘടന sched_entity *parent_entity(ഘടന sched_entity *se)
{
	തിരികെ ശൂന്യ;
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ tg_is_idle(ഘടന task_group *tg)
{
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ cfs_rq_is_idle(ഘടന cfs_rq *cfs_rq)
{
	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ se_is_idle(ഘടന sched_entity *se)
{
	തിരികെ task_has_idle_policy(task_of(se));
}

#endif /* !CONFIG_FAIR_GROUP_SCHED */

സ്ഥിതം __always_inline
bool account_cfs_rq_runtime(ഘടന cfs_rq *cfs_rq, u64 delta_exec);

/**************************************************************
 * Scheduling class tree data structure manipulation methods:
 */

ബാഹ്യ ശൂന്യം __BUILD_BUG_vruntime_cmp(ശൂന്യം);

/* Use __builtin_strcmp() because of __HAVE_ARCH_STRCMP: */

#define vruntime_cmp(A, CMP_STR, B) ({				\
	പൂർണ്ണസംഖ്യ __res = 0;						\
								\
	എങ്കിൽ (!__builtin_strcmp(CMP_STR, "<")) {			\
		__res = ((s64)((A)-(B)) < 0);			\
	} അല്ലെങ്കിൽ എങ്കിൽ (!__builtin_strcmp(CMP_STR, "<=")) {		\
		__res = ((s64)((A)-(B)) <= 0);			\
	} അല്ലെങ്കിൽ എങ്കിൽ (!__builtin_strcmp(CMP_STR, ">")) {		\
		__res = ((s64)((A)-(B)) > 0);			\
	} അല്ലെങ്കിൽ എങ്കിൽ (!__builtin_strcmp(CMP_STR, ">=")) {		\
		__res = ((s64)((A)-(B)) >= 0);			\
	} അല്ലെങ്കിൽ {						\
		/* Unknown operator throws linker error: */	\
		__BUILD_BUG_vruntime_cmp();			\
	}							\
								\
	__res;							\
})

ബാഹ്യ ശൂന്യം __BUILD_BUG_vruntime_op(ശൂന്യം);

#define vruntime_op(A, OP_STR, B) ({				\
	s64 __res = 0;						\
								\
	എങ്കിൽ (!__builtin_strcmp(OP_STR, "-")) {			\
		__res = (s64)((A)-(B));				\
	} അല്ലെങ്കിൽ {						\
		/* Unknown operator throws linker error: */	\
		__BUILD_BUG_vruntime_op();			\
	}							\
								\
	__res;						\
})


സ്ഥിതം വരിയിൽ __maybe_unused u64 max_vruntime(u64 max_vruntime, u64 vruntime)
{
	എങ്കിൽ (vruntime_cmp(vruntime, ">", max_vruntime))
		max_vruntime = vruntime;

	തിരികെ max_vruntime;
}

സ്ഥിതം വരിയിൽ __maybe_unused u64 min_vruntime(u64 min_vruntime, u64 vruntime)
{
	എങ്കിൽ (vruntime_cmp(vruntime, "<", min_vruntime))
		min_vruntime = vruntime;

	തിരികെ min_vruntime;
}

സ്ഥിതം വരിയിൽ bool entity_before(സ്ഥിരം ഘടന sched_entity *a,
				 സ്ഥിരം ഘടന sched_entity *b)
{
	/*
	 * Tiebreak on vruntime seems unnecessary since it can
	 * hardly happen.
	 */
	തിരികെ vruntime_cmp(a->deadline, "<", b->deadline);
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
സ്ഥിതം വരിയിൽ s64 entity_താക്കോൽ(ഘടന cfs_rq *cfs_rq, ഘടന sched_entity *se)
{
	തിരികെ vruntime_op(se->vruntime, "-", cfs_rq->zero_vruntime);
}

#define __node_2_se(കണ്ണി) \
	rb_entry((കണ്ണി), ഘടന sched_entity, run_കണ്ണി)

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
സ്ഥിതം വരിയിൽ unsigned long avg_vruntime_weight(ഘടന cfs_rq *cfs_rq, unsigned long w)
{
#ifdef CONFIG_64BIT
	എങ്കിൽ (cfs_rq->sum_shift)
		w = max(2UL, w >> cfs_rq->sum_shift);
#endif
	തിരികെ w;
}

സ്ഥിതം വരിയിൽ ശൂന്യം
__sum_w_vruntime_add(ഘടന cfs_rq *cfs_rq, ഘടന sched_entity *se)
{
	unsigned long weight = avg_vruntime_weight(cfs_rq, se->h_load.weight);
	s64 w_vruntime, താക്കോൽ = entity_താക്കോൽ(cfs_rq, se);

	w_vruntime = താക്കോൽ * weight;
	WARN_ON_ONCE((w_vruntime >> 63) != (w_vruntime >> 62));

	cfs_rq->sum_w_vruntime += w_vruntime;
	cfs_rq->sum_weight += weight;
}

സ്ഥിതം ശൂന്യം
sum_w_vruntime_add_paranoid(ഘടന cfs_rq *cfs_rq, ഘടന sched_entity *se)
{
	unsigned long weight;
	s64 താക്കോൽ, tmp;

again:
	weight = avg_vruntime_weight(cfs_rq, se->h_load.weight);
	താക്കോൽ = entity_താക്കോൽ(cfs_rq, se);

	എങ്കിൽ (check_mul_overflow(താക്കോൽ, weight, &താക്കോൽ))
		പോകുക overflow;

	എങ്കിൽ (check_add_overflow(cfs_rq->sum_w_vruntime, താക്കോൽ, &tmp))
		പോകുക overflow;

	cfs_rq->sum_w_vruntime = tmp;
	cfs_rq->sum_weight += weight;
	തിരികെ;

overflow:
	/*
	 * There's gotta be a limit -- if we're still failing at this point
	 * there's really nothing much to be done about things.
	 */
	BUG_ON(cfs_rq->sum_shift >= 10);
	cfs_rq->sum_shift++;

	/*
	 * Note: \Sum (k_i * (w_i >> 1)) != (\Sum (k_i * w_i)) >> 1
	 */
	cfs_rq->sum_w_vruntime = 0;
	cfs_rq->sum_weight = 0;

	വേണ്ടി (ഘടന rb_കണ്ണി *കണ്ണി = cfs_rq->tasks_timeline.rb_leftmost;
	     കണ്ണി; കണ്ണി = rb_next(കണ്ണി))
		__sum_w_vruntime_add(cfs_rq, __node_2_se(കണ്ണി));

	പോകുക again;
}

സ്ഥിതം ശൂന്യം
sum_w_vruntime_add(ഘടന cfs_rq *cfs_rq, ഘടന sched_entity *se)
{
	എങ്കിൽ (sched_feat(PARANOID_AVG))
		തിരികെ sum_w_vruntime_add_paranoid(cfs_rq, se);

	__sum_w_vruntime_add(cfs_rq, se);
}

സ്ഥിതം ശൂന്യം
sum_w_vruntime_sub(ഘടന cfs_rq *cfs_rq, ഘടന sched_entity *se)
{
	unsigned long weight = avg_vruntime_weight(cfs_rq, se->h_load.weight);
	s64 താക്കോൽ = entity_താക്കോൽ(cfs_rq, se);

	cfs_rq->sum_w_vruntime -= താക്കോൽ * weight;
	cfs_rq->sum_weight -= weight;
}

സ്ഥിതം വരിയിൽ
ശൂന്യം update_zero_vruntime(ഘടന cfs_rq *cfs_rq, s64 delta)
{
	/*
	 * v' = v + d ==> sum_w_vruntime' = sum_w_vruntime - d*sum_weight
	 */
	cfs_rq->sum_w_vruntime -= cfs_rq->sum_weight * delta;
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
u64 avg_vruntime(ഘടന cfs_rq *cfs_rq)
{
	ഘടന sched_entity *curr = cfs_rq->curr;
	long weight = cfs_rq->sum_weight;
	s64 delta = 0;

	എങ്കിൽ (curr && !curr->on_rq)
		curr = ശൂന്യ;

	എങ്കിൽ (weight) {
		s64 runtime = cfs_rq->sum_w_vruntime;

		എങ്കിൽ (curr) {
			unsigned long w = avg_vruntime_weight(cfs_rq, curr->h_load.weight);

			runtime += entity_താക്കോൽ(cfs_rq, curr) * w;
			weight += w;
		}

		/* sign flips effective floor / ceiling */
		എങ്കിൽ (runtime < 0)
			runtime -= (weight - 1);

		delta = div64_long(runtime, weight);
	} അല്ലെങ്കിൽ എങ്കിൽ (curr) {
		/*
		 * When there is but one element, it is the average.
		 */
		delta = curr->vruntime - cfs_rq->zero_vruntime;
	}

	update_zero_vruntime(cfs_rq, delta);

	തിരികെ cfs_rq->zero_vruntime;
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
സ്ഥിതം u64 ineligible_vruntime(ഘടന cfs_rq *cfs_rq)
{
	ഘടന sched_entity *curr = cfs_rq->curr;
	long weight = cfs_rq->sum_weight;
	s64 delta = 0;

	എങ്കിൽ (curr && !curr->on_rq)
		curr = ശൂന്യ;

	എങ്കിൽ (weight) {
		s64 runtime = cfs_rq->sum_w_vruntime;

		/*
		 * Do not add @curr to obtain the effective '- w_j' terms.
		 */

		/* sign flips effective floor / ceiling */
		എങ്കിൽ (runtime < 0)
			runtime -= (weight - 1);

		delta = div64_long(runtime, weight);
	}

	തിരികെ cfs_rq->zero_vruntime + delta + 1;
}

സ്ഥിതം വരിയിൽ u64 cfs_rq_max_slice(ഘടന cfs_rq *cfs_rq);

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
സ്ഥിതം s64 entity_lag(ഘടന cfs_rq *cfs_rq, ഘടന sched_entity *se, u64 avruntime)
{
	u64 max_slice = cfs_rq_max_slice(cfs_rq) + TICK_NSEC;
	s64 vlag, limit;

	vlag = avruntime - se->vruntime;
	limit = calc_delta_fair(max_slice, se);

	തിരികെ clamp(vlag, -limit, limit);
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
സ്ഥിതം __always_inline
bool update_entity_lag(ഘടന cfs_rq *cfs_rq, ഘടന sched_entity *se)
{
	u64 avruntime = avg_vruntime(cfs_rq);
	s64 vlag = entity_lag(cfs_rq, se, avruntime);

	എങ്കിൽ (se->sched_delayed) {
		/* previous vlag < 0 otherwise se would not be delayed */
		vlag = max(vlag, se->vlag);
		എങ്കിൽ (sched_feat(DELAY_ZERO))
			vlag = min(vlag, 0);
	}
	se->vlag = vlag;

	തിരികെ avruntime - vlag != se->vruntime;
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
സ്ഥിതം പൂർണ്ണസംഖ്യ vruntime_eligible(ഘടന cfs_rq *cfs_rq, u64 vruntime)
{
	ഘടന sched_entity *curr = cfs_rq->curr;
	s64 താക്കോൽ, avg = cfs_rq->sum_w_vruntime;
	long load = cfs_rq->sum_weight;

	എങ്കിൽ (curr && curr->on_rq) {
		unsigned long weight = avg_vruntime_weight(cfs_rq, curr->h_load.weight);

		avg += entity_താക്കോൽ(cfs_rq, curr) * weight;
		load += weight;
	}

	താക്കോൽ = vruntime_op(vruntime, "-", cfs_rq->zero_vruntime);

	/*
	 * The worst case term for @key includes 'NSEC_TICK * NICE_0_LOAD'
	 * and @load obviously includes NICE_0_LOAD. NSEC_TICK is around 24
	 * bits, while NICE_0_LOAD is 20 on 64bit and 10 otherwise.
	 *
	 * This gives that on 64bit the product will be at least 64bit which
	 * overflows s64, while on 32bit it will only be 44bits and should fit
	 * comfortably.
	 */
#ifdef CONFIG_64BIT
#ifdef CONFIG_ARCH_SUPPORTS_INT128
	/* This often results in simpler code than __builtin_mul_overflow(). */
	തിരികെ avg >= (__int128)താക്കോൽ * load;
#അല്ലെങ്കിൽ
	s64 rhs;
	/*
	 * On overflow, the sign of key tells us the correct answer: a large
	 * positive key means vruntime >> V, so not eligible; a large negative
	 * key means vruntime << V, so eligible.
	 */
	എങ്കിൽ (check_mul_overflow(താക്കോൽ, load, &rhs))
		തിരികെ താക്കോൽ <= 0;

	തിരികെ avg >= rhs;
#endif
#അല്ലെങ്കിൽ /* 32bit */
	തിരികെ avg >= താക്കോൽ * load;
#endif
}

പൂർണ്ണസംഖ്യ entity_eligible(ഘടന cfs_rq *cfs_rq, ഘടന sched_entity *se)
{
	തിരികെ vruntime_eligible(cfs_rq, se->vruntime);
}

സ്ഥിതം വരിയിൽ u64 cfs_rq_min_slice(ഘടന cfs_rq *cfs_rq)
{
	ഘടന sched_entity *വേര് = __pick_root_entity(cfs_rq);
	ഘടന sched_entity *curr = cfs_rq->curr;
	u64 min_slice = ~0ULL;

	എങ്കിൽ (curr && curr->on_rq)
		min_slice = curr->slice;

	എങ്കിൽ (വേര്)
		min_slice = min(min_slice, വേര്->min_slice);

	തിരികെ min_slice;
}

സ്ഥിതം വരിയിൽ u64 cfs_rq_max_slice(ഘടന cfs_rq *cfs_rq)
{
	ഘടന sched_entity *വേര് = __pick_root_entity(cfs_rq);
	ഘടന sched_entity *curr = cfs_rq->curr;
	u64 max_slice = 0ULL;

	എങ്കിൽ (curr && curr->on_rq)
		max_slice = curr->slice;

	എങ്കിൽ (വേര്)
		max_slice = max(max_slice, വേര്->max_slice);

	തിരികെ max_slice;
}

സ്ഥിതം വരിയിൽ bool __entity_less(ഘടന rb_കണ്ണി *a, സ്ഥിരം ഘടന rb_കണ്ണി *b)
{
	തിരികെ entity_before(__node_2_se(a), __node_2_se(b));
}

/* min() for wrapping vruntimes */
സ്ഥിതം വരിയിൽ u64 __min_vruntime(u64 a, u64 b)
{
	തിരികെ vruntime_cmp(a, "<", b) ? a : b;
}

RB_DECLARE_CALLBACKS(സ്ഥിതം, min_vruntime_cb,
		     ഘടന sched_entity, run_കണ്ണി,
		     RB_AUG(vruntime, min_vruntime, __min_vruntime),
		     RB_AUG(slice, min_slice, min),
		     RB_AUG(slice, max_slice, max));

/*
 * Enqueue an entity into the rb-tree:
 */
സ്ഥിതം ശൂന്യം __enqueue_entity(ഘടന cfs_rq *cfs_rq, ഘടന sched_entity *se)
{
	WARN_ON_ONCE(&rq_of(cfs_rq)->cfs != cfs_rq);
	WARN_ON_ONCE(!entity_is_task(se));

	sum_w_vruntime_add(cfs_rq, se);
	se->min_vruntime = se->vruntime;
	se->min_slice = se->slice;
	se->max_slice = se->slice;

	rb_add_augmented_cached(&se->run_കണ്ണി, &cfs_rq->tasks_timeline,
				__entity_less, &min_vruntime_cb);
}

സ്ഥിതം ശൂന്യം __dequeue_entity(ഘടന cfs_rq *cfs_rq, ഘടന sched_entity *se)
{
	WARN_ON_ONCE(&rq_of(cfs_rq)->cfs != cfs_rq);
	WARN_ON_ONCE(!entity_is_task(se));

	rb_erase_augmented_cached(&se->run_കണ്ണി, &cfs_rq->tasks_timeline,
				  &min_vruntime_cb);
	sum_w_vruntime_sub(cfs_rq, se);
}

ഘടന sched_entity *__pick_root_entity(ഘടന cfs_rq *cfs_rq)
{
	ഘടന rb_കണ്ണി *വേര് = cfs_rq->tasks_timeline.rb_വേര്.rb_കണ്ണി;

	എങ്കിൽ (!വേര്)
		തിരികെ ശൂന്യ;

	തിരികെ __node_2_se(വേര്);
}

ഘടന sched_entity *__pick_first_entity(ഘടന cfs_rq *cfs_rq)
{
	ഘടന rb_കണ്ണി *ഇടത് = rb_first_cached(&cfs_rq->tasks_timeline);

	എങ്കിൽ (!ഇടത്)
		തിരികെ ശൂന്യ;

	തിരികെ __node_2_se(ഇടത്);
}

/*
 * Set the vruntime up to which an entity can run before looking
 * for another entity to pick.
 * In case of run to parity, we use the shortest slice of the enqueued
 * entities to set the protected period.
 * When run to parity is disabled, we give a minimum quantum to the running
 * entity to ensure progress.
 */
സ്ഥിതം വരിയിൽ ശൂന്യം set_protect_slice(ഘടന cfs_rq *cfs_rq, ഘടന sched_entity *se)
{
	u64 slice = normalized_sysctl_sched_base_slice;
	u64 vprot = se->deadline;

	എങ്കിൽ (sched_feat(RUN_TO_PARITY))
		slice = cfs_rq_min_slice(cfs_rq);

	slice = min(slice, se->slice);

	/* If there are shorter slices than se's one */
	എങ്കിൽ (slice != se->slice) {
		vprot = min_vruntime(vprot, se->vruntime + calc_delta_fair(slice, se));
		എങ്കിൽ (sched_feat(PREEMPT_SHORT))
			vprot = min_vruntime(vprot, ineligible_vruntime(cfs_rq));
	}

	se->vprot = vprot;
}

സ്ഥിതം വരിയിൽ ശൂന്യം update_protect_slice(ഘടന cfs_rq *cfs_rq, ഘടന sched_entity *se)
{
	u64 vruntime = min_vruntime(se->vruntime, avg_vruntime(cfs_rq));
	u64 slice = cfs_rq_min_slice(cfs_rq);
	u64 vprot = min_vruntime(se->vprot, vruntime + calc_delta_fair(slice, se));

	എങ്കിൽ (sched_feat(PREEMPT_SHORT) && slice != se->slice)
		vprot = min_vruntime(vprot, ineligible_vruntime(cfs_rq));

	se->vprot = vprot;
}

സ്ഥിതം വരിയിൽ bool protect_slice(ഘടന sched_entity *se)
{
	തിരികെ vruntime_cmp(se->vruntime, "<", se->vprot);
}

സ്ഥിതം വരിയിൽ ശൂന്യം cancel_protect_slice(ഘടന sched_entity *se)
{
	എങ്കിൽ (protect_slice(se))
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
സ്ഥിതം ഘടന sched_entity *pick_eevdf(ഘടന cfs_rq *cfs_rq, bool protect)
{
	ഘടന rb_കണ്ണി *കണ്ണി = cfs_rq->tasks_timeline.rb_വേര്.rb_കണ്ണി;
	ഘടന sched_entity *se = __pick_first_entity(cfs_rq);
	ഘടന sched_entity *curr = cfs_rq->curr;
	ഘടന sched_entity *best = ശൂന്യ;

	/*
	 * We can safely skip eligibility check if there is only one entity
	 * in this cfs_rq, saving some cycles.
	 */
	എങ്കിൽ (cfs_rq->h_nr_queued == 1)
		തിരികെ curr && curr->on_rq ? curr : se;

	/*
	 * Picking the ->next buddy will affect latency but not fairness.
	 */
	എങ്കിൽ (sched_feat(PICK_BUDDY) && protect &&
	    cfs_rq->next && entity_eligible(cfs_rq, cfs_rq->next)) {
		/* ->next will never be delayed */
		WARN_ON_ONCE(cfs_rq->next->sched_delayed);
		തിരികെ cfs_rq->next;
	}

	എങ്കിൽ (curr && (!curr->on_rq || !entity_eligible(cfs_rq, curr)))
		curr = ശൂന്യ;

	എങ്കിൽ (curr && protect && protect_slice(curr))
		തിരികെ curr;

	/* Pick the leftmost entity if it's eligible */
	എങ്കിൽ (se && entity_eligible(cfs_rq, se)) {
		best = se;
		പോകുക found;
	}

	/* Heap search for the EEVD entity */
	ആകുമ്പോൾ (കണ്ണി) {
		ഘടന rb_കണ്ണി *ഇടത് = കണ്ണി->rb_ഇടത്;

		/*
		 * Eligible entities in left subtree are always better
		 * choices, since they have earlier deadlines.
		 */
		എങ്കിൽ (ഇടത് && vruntime_eligible(cfs_rq,
					__node_2_se(ഇടത്)->min_vruntime)) {
			കണ്ണി = ഇടത്;
			തുടരുക;
		}

		se = __node_2_se(കണ്ണി);

		/*
		 * The left subtree either is empty or has no eligible
		 * entity, so check the current node since it is the one
		 * with earliest deadline that might be eligible.
		 */
		എങ്കിൽ (entity_eligible(cfs_rq, se)) {
			best = se;
			നിർത്തുക;
		}

		കണ്ണി = കണ്ണി->rb_വലത്;
	}
found:
	എങ്കിൽ (!best || (curr && entity_before(curr, best)))
		best = curr;

	തിരികെ best;
}

ഘടന sched_entity *__pick_last_entity(ഘടന cfs_rq *cfs_rq)
{
	ഘടന rb_കണ്ണി *last = rb_last(&cfs_rq->tasks_timeline.rb_വേര്);

	എങ്കിൽ (!last)
		തിരികെ ശൂന്യ;

	തിരികെ __node_2_se(last);
}

/**************************************************************
 * Scheduling class statistics methods:
 */
പൂർണ്ണസംഖ്യ sched_update_scaling(ശൂന്യം)
{
	unsigned പൂർണ്ണസംഖ്യ factor = get_update_sysctl_factor();

#define WRT_SYSCTL(name) \
	(normalized_sysctl_##name = sysctl_##name / (factor))
	WRT_SYSCTL(sched_base_slice);
#undef WRT_SYSCTL

	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം clear_buddies(ഘടന cfs_rq *cfs_rq, ഘടന sched_entity *se);

/*
 * XXX: strictly: vd_i += N*r_i/w_i such that: vd_i > ve_i
 * this is probably good enough.
 */
സ്ഥിതം bool update_deadline(ഘടന cfs_rq *cfs_rq, ഘടന sched_entity *se)
{
	എങ്കിൽ (vruntime_cmp(se->vruntime, "<", se->deadline))
		തിരികെ false;

	/*
	 * For EEVDF the virtual time slope is determined by w_i (iow.
	 * nice) while the request time r_i is determined by
	 * sysctl_sched_base_slice.
	 */
	എങ്കിൽ (!se->custom_slice)
		se->slice = sysctl_sched_base_slice;

	/*
	 * EEVDF: vd_i = ve_i + r_i / w_i
	 */
	se->deadline = se->vruntime + calc_delta_fair(se->slice, se);
	avg_vruntime(cfs_rq);

	/*
	 * The task has consumed its request, reschedule.
	 */
	തിരികെ true;
}

#അന്തര്ഭാവ "pelt.h"

സ്ഥിതം പൂർണ്ണസംഖ്യ select_idle_sibling(ഘടന task_struct *p, പൂർണ്ണസംഖ്യ prev_cpu, പൂർണ്ണസംഖ്യ cpu);
സ്ഥിതം unsigned long task_h_load(ഘടന task_struct *p);
സ്ഥിതം unsigned long capacity_of(പൂർണ്ണസംഖ്യ cpu);

/* Give new sched_entity start runnable values to heavy its load in infant time */
ശൂന്യം init_entity_runnable_average(ഘടന sched_entity *se)
{
	ഘടന sched_avg *sa = &se->avg;

	memset(sa, 0, വലുപ്പം(*sa));

	/*
	 * Tasks are initialized with full load to be seen as heavy tasks until
	 * they get a chance to stabilize to their real load level.
	 * Group entities are initialized with zero load to reflect the fact that
	 * nothing has been attached to the task group yet.
	 */
	എങ്കിൽ (entity_is_task(se))
		sa->load_avg = scale_load_down(se->load.weight);

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
ശൂന്യം post_init_entity_util_avg(ഘടന task_struct *p)
{
	ഘടന sched_entity *se = &p->se;
	ഘടന cfs_rq *cfs_rq = cfs_rq_of(se);
	ഘടന sched_avg *sa = &se->avg;
	long cpu_scale = arch_scale_cpu_capacity(cpu_of(rq_of(cfs_rq)));
	long cap = (long)(cpu_scale - cfs_rq->avg.util_avg) / 2;

	എങ്കിൽ (p->sched_class != &fair_sched_class) {
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
		se->avg.last_update_time = cfs_rq_clock_pelt(cfs_rq);
		തിരികെ;
	}

	എങ്കിൽ (cap > 0) {
		എങ്കിൽ (cfs_rq->avg.util_avg != 0) {
			sa->util_avg  = cfs_rq->avg.util_avg * se_weight(se);
			sa->util_avg /= (cfs_rq->avg.load_avg + 1);

			എങ്കിൽ (sa->util_avg > cap)
				sa->util_avg = cap;
		} അല്ലെങ്കിൽ {
			sa->util_avg = cap;
		}
	}

	sa->runnable_avg = sa->util_avg;
}

സ്ഥിതം വരിയിൽ ശൂന്യം account_mm_sched(ഘടന rq *rq, ഘടന task_struct *p, s64 delta_exec);

സ്ഥിതം s64 update_se(ഘടന rq *rq, ഘടന sched_entity *se)
{
	u64 now = rq_clock_task(rq);
	s64 delta_exec;

	delta_exec = now - se->exec_start;
	എങ്കിൽ (unlikely(delta_exec <= 0))
		തിരികെ delta_exec;

	se->exec_start = now;
	എങ്കിൽ (entity_is_task(se)) {
		ഘടന task_struct *running = rq->curr;
		/*
		 * If se is a task, we account the time against the running
		 * task, as w/ proxy-exec they may not be the same.
		 */
		running->se.exec_start = now;
		running->se.sum_exec_runtime += delta_exec;

		trace_sched_stat_runtime(running, delta_exec);
		account_group_exec_runtime(running, delta_exec);
		account_mm_sched(rq, running, delta_exec);

		cgroup_account_cputime(running, delta_exec);
	} അല്ലെങ്കിൽ {
		/* If not task, account the time against donor se  */
		se->sum_exec_runtime += delta_exec;
	}

	എങ്കിൽ (schedstat_enabled()) {
		ഘടന sched_statistics *stats;

		stats = __schedstats_from_se(se);
		__schedstat_set(stats->exec_max,
				max(delta_exec, stats->exec_max));
	}

	തിരികെ delta_exec;
}

#ifdef CONFIG_SCHED_CACHE

/*
 * XXX numbers come from a place the sun don't shine -- probably wants to be SD
 * tunable or so.
 */
#define EPOCH_PERIOD	(HZ / 100)	/* 10 ms */
#define EPOCH_LLC_AFFINITY_TIMEOUT	5	/* 50 ms */
__read_mostly unsigned പൂർണ്ണസംഖ്യ llc_aggr_tolerance	= 1;
__read_mostly unsigned പൂർണ്ണസംഖ്യ llc_epoch_period	= EPOCH_PERIOD;
__read_mostly unsigned പൂർണ്ണസംഖ്യ llc_epoch_affinity_timeout = EPOCH_LLC_AFFINITY_TIMEOUT;
__read_mostly unsigned പൂർണ്ണസംഖ്യ llc_imb_pct		= 20;
__read_mostly unsigned പൂർണ്ണസംഖ്യ llc_overaggr_pct	= 50;

സ്ഥിതം പൂർണ്ണസംഖ്യ llc_id(പൂർണ്ണസംഖ്യ cpu)
{
	എങ്കിൽ (cpu < 0)
		തിരികെ -1;

	തിരികെ per_cpu(sd_llc_id, cpu);
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ get_sched_cache_scale(പൂർണ്ണസംഖ്യ mul)
{
	unsigned പൂർണ്ണസംഖ്യ tol = READ_ONCE(llc_aggr_tolerance);

	എങ്കിൽ (!tol)
		തിരികെ 0;

	എങ്കിൽ (tol >= 100)
		തിരികെ INT_MAX;

	തിരികെ (1 + (tol - 1) * mul);
}

സ്ഥിതം bool exceed_llc_capacity(ഘടന sched_cache_group *grp, പൂർണ്ണസംഖ്യ cpu)
{
#ifdef CONFIG_NUMA_BALANCING
	unsigned long llc, footprint;
	ഘടന sched_domain *sd;
	പൂർണ്ണസംഖ്യ scale;

	guard(rcu)();

	sd = rcu_dereference_sched_domain(cpu_rq(cpu)->sd);
	എങ്കിൽ (!sd)
		തിരികെ true;

	എങ്കിൽ (static_branch_likely(&sched_numa_balancing)) {
		/*
		 * TBD: RDT exclusive LLC ways reserved should be
		 * excluded.
		 */
		llc = sd->llc_bytes;
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
		എങ്കിൽ (scale == INT_MAX)
			തിരികെ false;

		തിരികെ ((llc * (u64)scale) < (footprint * PAGE_SIZE));
	}
#endif
	തിരികെ false;
}

സ്ഥിതം bool invalid_llc_nr(ഘടന sched_cache_group *grp, ഘടന task_struct *p,
			   പൂർണ്ണസംഖ്യ cpu)
{
	പൂർണ്ണസംഖ്യ scale;

	എങ്കിൽ (get_nr_threads(p) <= 1)
		തിരികെ true;

	/*
	 * Scale the number of 'cores' in a LLC by llc_aggr_tolerance
	 * and compare it to the task's active threads.
	 */
	scale = get_sched_cache_scale(1);
	എങ്കിൽ (scale == INT_MAX)
		തിരികെ false;

	തിരികെ !fits_capacity((READ_ONCE(grp->nr_running_avg) * cpu_smt_അക്കം_threads),
			(scale * per_cpu(sd_llc_size, cpu)));
}

/*
 * A task counts in nr_pref_llc_running while it is queued on its preferred
 * LLC (pref_llc_queued) and runnable (!sched_delayed), keeping the counter in
 * the runnable domain so alb_break_llc() can compare it with h_nr_runnable.
 */
സ്ഥിതം bool task_pref_llc_runnable(ഘടന task_struct *p)
{
	തിരികെ p->pref_llc_queued && !p->se.sched_delayed;
}

സ്ഥിതം ശൂന്യം pref_llc_running_inc(ഘടന rq *rq, ഘടന task_struct *p)
{
	എങ്കിൽ (task_pref_llc_runnable(p))
		rq->nr_pref_llc_running++;
}

സ്ഥിതം ശൂന്യം pref_llc_running_dec(ഘടന rq *rq, ഘടന task_struct *p)
{
	എങ്കിൽ (task_pref_llc_runnable(p))
		rq->nr_pref_llc_running--;
}

സ്ഥിതം ശൂന്യം account_llc_enqueue(ഘടന rq *rq, ഘടന task_struct *p)
{
	പൂർണ്ണസംഖ്യ pref_llc, pref_llc_queued;
	ഘടന sched_domain *sd;

	pref_llc = p->preferred_llc;
	എങ്കിൽ (pref_llc < 0)
		തിരികെ;

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
	എങ്കിൽ (sd && (unsigned പൂർണ്ണസംഖ്യ)pref_llc < sd->llc_max)
		sd->llc_counts[pref_llc]++;
}

സ്ഥിതം ശൂന്യം account_llc_dequeue(ഘടന rq *rq, ഘടന task_struct *p)
{
	ഘടന sched_domain *sd;
	പൂർണ്ണസംഖ്യ pref_llc;

	pref_llc = p->preferred_llc;
	എങ്കിൽ (pref_llc < 0)
		തിരികെ;

	rq->nr_llc_running--;
	എങ്കിൽ (p->pref_llc_queued) {
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
	എങ്കിൽ (sd && (unsigned പൂർണ്ണസംഖ്യ)pref_llc < sd->llc_max) {
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
		എങ്കിൽ (sd->llc_counts[pref_llc])
			sd->llc_counts[pref_llc]--;
	}
}

പൂർണ്ണസംഖ്യ mm_init_sched(ഘടന mm_struct *mm,
		  ഘടന sched_cache_time __percpu *_pcpu_sched)
{
	ഘടന sched_cache_group *grp;
	unsigned long epoch = 0;
	പൂർണ്ണസംഖ്യ i;

	grp = kzalloc_obj(*grp);
	എങ്കിൽ (!grp) {
		free_percpu(_pcpu_sched);
		mm->sched_cache_grp = ശൂന്യ;
		തിരികെ -ENOMEM;
	}

	for_each_possible_cpu(i) {
		ഘടന sched_cache_time *pcpu_sched = per_cpu_ptr(_pcpu_sched, i);
		ഘടന rq *rq = cpu_rq(i);

		pcpu_sched->runtime = 0;
		/* a slightly stale cpu epoch is acceptible */
		pcpu_sched->epoch = rq->cpu_epoch;
		epoch = rq->cpu_epoch;
	}

	raw_spin_lock_init(&grp->lock);
	grp->epoch = epoch;
	grp->cpu = -1;
	grp->next_scan = jiffies;
	grp->nr_running_avg = 0;
	grp->footprint = 0;
	refcount_set(&grp->refcnt, 1);
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
	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം sched_cache_group_free_rcu(ഘടന rcu_head *rcu)
{
	ഘടന sched_cache_group *grp =
		container_of(rcu, ഘടന sched_cache_group, rcu);

	free_percpu(grp->pcpu_sched);
	kfree(grp);
}

സ്ഥിതം ശൂന്യം sched_cache_group_put(ഘടന sched_cache_group *grp)
{
	എങ്കിൽ (!grp || !refcount_dec_and_test(&grp->refcnt))
		തിരികെ;

	call_rcu(&grp->rcu, sched_cache_group_free_rcu);
}

DEFINE_FREE(sched_cache_group_put, ഘടന sched_cache_group *,
	    sched_cache_group_put(_T));

#define rcu_deref_sched_cache_grp(tsk) \
	rcu_dereference_check((tsk)->sched_cache_grp, (tsk) == current)

സ്ഥിതം ഘടന sched_cache_group *sched_cache_replace_grp(ഘടന task_struct *p,
							 ഘടന sched_cache_group *new)
{
	ഘടന sched_cache_group *old;

	old = rcu_deref_sched_cache_grp(p);
	rcu_assign_pointer(p->sched_cache_grp, new);

	തിരികെ old;
}

ഘടന sched_cache_group *sched_cache_group_get(ഘടന sched_cache_group *grp)
{
	/*
	 * refcount_inc_not_zero() is the acquire primitive for lockless
	 * (RCU) lookups; plain refcount_inc() would scribble the count if
	 * it already reached zero. Return NULL in that case.
	 */
	എങ്കിൽ (grp && !refcount_inc_not_zero(&grp->refcnt))
		grp = ശൂന്യ;

	തിരികെ grp;
}

ഘടന sched_cache_group *task_cache_group_get(ഘടന task_struct *p)
{
	guard(rcu)();
	തിരികെ sched_cache_group_get(rcu_dereference(p->sched_cache_grp));
}

ശൂന്യം sched_cache_fork(ഘടന task_struct *p)
{
	/*
	 * The child takes its own reference on the mm's cache group, separate
	 * from the reference held by the mm. @p is not yet visible to readers,
	 * so a plain initializing store is enough.
	 */
	RCU_INIT_POINTER(p->sched_cache_grp,
			 sched_cache_group_get(p->mm->sched_cache_grp));
}

ശൂന്യം sched_cache_fork_cleanup(ഘടന task_struct *p)
{
	/*
	 * A fork that fails after sched_cache_fork() never reaches exit_mm(),
	 * so drop the reference here. @p never became visible, so there are no
	 * concurrent readers and the reference we hold keeps the group alive.
	 */
	sched_cache_group_put(rcu_access_pointer(p->sched_cache_grp));
	RCU_INIT_POINTER(p->sched_cache_grp, ശൂന്യ);
}

ശൂന്യം sched_cache_exec_mmap(ഘടന task_struct *p, ഘടന mm_struct *mm)
{
	ഘടന sched_cache_group *old;

	/*
	 * Acquire the new reference before publishing the pointer, then drop
	 * the old one. @p is current and the only writer of its own pointer.
	 */
	old = sched_cache_replace_grp(p, sched_cache_group_get(mm->sched_cache_grp));
	sched_cache_group_put(old);
}

ശൂന്യം sched_cache_exit_mm(ഘടന task_struct *p)
{
	ഘടന sched_cache_group *grp = sched_cache_replace_grp(p, ശൂന്യ);

#ifdef CONFIG_NUMA_BALANCING
	/*
	 * Subtract this task's footprint from the group before dropping the
	 * reference, so the group footprint converges as its threads exit.
	 * Unlocked for performance; clamp to avoid underflow.
	 */
	എങ്കിൽ (grp && p->total_numa_faults) {
		unsigned long fp = READ_ONCE(grp->footprint);
		unsigned long sub = min(fp, p->total_numa_faults);

		WRITE_ONCE(grp->footprint, fp - sub);
	}
#endif
	sched_cache_group_put(grp);
}

ശൂന്യം mm_destroy_sched(ഘടന mm_struct *mm)
{
	sched_cache_group_put(mm->sched_cache_grp);
	mm->sched_cache_grp = ശൂന്യ;
}

/* because why would C be fully specified */
സ്ഥിതം __always_inline ശൂന്യം __shr_u64(u64 *val, unsigned പൂർണ്ണസംഖ്യ n)
{
	എങ്കിൽ (n >= 64) {
		*val = 0;
		തിരികെ;
	}
	*val >>= n;
}

സ്ഥിതം വരിയിൽ ശൂന്യം __update_mm_sched(ഘടന rq *rq,
				     ഘടന sched_cache_time *pcpu_sched)
{
	lockdep_assert_held(&rq->cpu_epoch_lock);

	unsigned പൂർണ്ണസംഖ്യ period = max(READ_ONCE(llc_epoch_period), 1U);
	unsigned long n, now = jiffies;
	long delta = now - rq->cpu_epoch_next;

	എങ്കിൽ (delta > 0) {
		n = (delta + period - 1) / period;
		rq->cpu_epoch += n;
		rq->cpu_epoch_next += n * period;
		__shr_u64(&rq->cpu_runtime, n);
	}

	n = rq->cpu_epoch - pcpu_sched->epoch;
	എങ്കിൽ (n) {
		pcpu_sched->epoch += n;
		__shr_u64(&pcpu_sched->runtime, n);
	}
}

സ്ഥിതം unsigned long fraction_mm_sched(ഘടന rq *rq,
				       ഘടന sched_cache_time *pcpu_sched)
{
	guard(raw_spinlock_irqsave)(&rq->cpu_epoch_lock);

	__update_mm_sched(rq, pcpu_sched);

	/*
	 * Runtime is a geometric series (r=0.5) and as such will sum to twice
	 * the accumulation period, this means the multiplcation here should
	 * not overflow.
	 */
	തിരികെ div64_u64(NICE_0_LOAD * pcpu_sched->runtime, rq->cpu_runtime + 1);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ get_pref_llc(ഘടന task_struct *p, ഘടന sched_cache_group *grp)
{
	പൂർണ്ണസംഖ്യ mm_sched_llc = -1, mm_sched_cpu;

	എങ്കിൽ (!grp)
		തിരികെ -1;

	mm_sched_cpu = READ_ONCE(grp->cpu);
	എങ്കിൽ (mm_sched_cpu != -1) {
		mm_sched_llc = llc_id(mm_sched_cpu);

#ifdef CONFIG_NUMA_BALANCING
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
		എങ്കിൽ (static_branch_likely(&sched_numa_balancing) &&
		    p->numa_preferred_nid >= 0 &&
		    cpu_to_കണ്ണി(mm_sched_cpu) != p->numa_preferred_nid)
			mm_sched_llc = -1;
#endif
	}

	തിരികെ mm_sched_llc;
}

സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ task_running_on_cpu(പൂർണ്ണസംഖ്യ cpu, ഘടന task_struct *p);

സ്ഥിതം വരിയിൽ
ശൂന്യം account_mm_sched(ഘടന rq *rq, ഘടന task_struct *p, s64 delta_exec)
{
	ഘടന sched_cache_group *grp = rcu_dereference_all(p->sched_cache_grp);
	ഘടന sched_cache_time *pcpu_sched;
	പൂർണ്ണസംഖ്യ mm_sched_llc = -1;
	unsigned long epoch;

	എങ്കിൽ (!sched_cache_enabled())
		തിരികെ;

	എങ്കിൽ (p->sched_class != &fair_sched_class)
		തിരികെ;
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
	എങ്കിൽ (p->flags & PF_KTHREAD || !grp || !grp->pcpu_sched)
		തിരികെ;

	pcpu_sched = per_cpu_ptr(grp->pcpu_sched, cpu_of(rq));

	scoped_guard (raw_spinlock, &rq->cpu_epoch_lock) {
		__update_mm_sched(rq, pcpu_sched);
		pcpu_sched->runtime += delta_exec;
		rq->cpu_runtime += delta_exec;
		epoch = rq->cpu_epoch;
	}

	/*
	 * If this process hasn't hit task_cache_work() for a while invalidate
	 * its preferred state.
	 */
	എങ്കിൽ ((long)(epoch - READ_ONCE(grp->epoch)) > llc_epoch_affinity_timeout ||
	    invalid_llc_nr(grp, p, cpu_of(rq)) ||
	    exceed_llc_capacity(grp, cpu_of(rq))) {
		എങ്കിൽ (READ_ONCE(grp->cpu) != -1)
			WRITE_ONCE(grp->cpu, -1);
	}

	mm_sched_llc = get_pref_llc(p, grp);

	/* task not on rq accounted later in account_entity_enqueue() */
	എങ്കിൽ (task_running_on_cpu(rq->cpu, p) &&
	    READ_ONCE(p->preferred_llc) != mm_sched_llc) {
		account_llc_dequeue(rq, p);
		WRITE_ONCE(p->preferred_llc, mm_sched_llc);
		account_llc_enqueue(rq, p);
	}
}

സ്ഥിതം ശൂന്യം task_tick_cache(ഘടന rq *rq, ഘടന task_struct *p)
{
	ഘടന sched_cache_group *grp = rcu_dereference_all(p->sched_cache_grp);
	ഘടന callback_head *work = &p->cache_work;
	unsigned long epoch;

	എങ്കിൽ (!sched_cache_enabled())
		തിരികെ;

	എങ്കിൽ (!grp || p->flags & PF_KTHREAD ||
	    !grp->pcpu_sched)
		തിരികെ;

	epoch = rq->cpu_epoch;
	/* avoid moving backwards */
	എങ്കിൽ (time_after_eq(grp->epoch, epoch))
		തിരികെ;

	guard(raw_spinlock)(&grp->lock);

	എങ്കിൽ (work->next == work) {
		task_work_add(p, work, TWA_RESUME);
		WRITE_ONCE(grp->epoch, epoch);
	}
}

സ്ഥിതം ശൂന്യം get_scan_cpumasks(cpumask_var_t cpus, ഘടന task_struct *p,
			      ഘടന sched_cache_group *grp)
{
#ifdef CONFIG_NUMA_BALANCING
	പൂർണ്ണസംഖ്യ cpu, curr_cpu, nid, pref_nid;

	എങ്കിൽ (!static_branch_likely(&sched_numa_balancing))
		പോകുക out;

	cpu = READ_ONCE(grp->cpu);
	എങ്കിൽ (cpu != -1)
		nid = cpu_to_കണ്ണി(cpu);
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
	എങ്കിൽ (pref_nid == NUMA_NO_കണ്ണി)
		പോകുക out;

	cpumask_or(cpus, cpus, cpumask_of_കണ്ണി(pref_nid));

	/* honor the task's preferred LLC CPU */
	എങ്കിൽ (cpu != -1 && !cpumask_test_cpu(cpu, cpus) && nid != NUMA_NO_കണ്ണി)
		cpumask_or(cpus, cpus, cpumask_of_കണ്ണി(nid));

	/* make sure the task's current running node is included */
	എങ്കിൽ (!cpumask_test_cpu(curr_cpu, cpus))
		cpumask_or(cpus, cpus, cpumask_of_കണ്ണി(cpu_to_കണ്ണി(curr_cpu)));

	തിരികെ;

out:
#endif
	cpumask_copy(cpus, cpu_online_mask);
}

സ്ഥിതം വരിയിൽ ശൂന്യം update_avg_scale(u64 *avg, u64 sample)
{
	പൂർണ്ണസംഖ്യ factor = per_cpu(sd_llc_size, raw_smp_processor_id());
	s64 diff = sample - *avg;
	u32 ഹാരകം;

	/*
	 * Scale the divisor based on the number of CPUs contained
	 * in the LLC. This scaling ensures smaller LLC domains use
	 * a smaller divisor to achieve more precise sensitivity to
	 * changes in nr_running, while larger LLC domains are capped
	 * at a maximum divisor of 8 which is the default smoothing
	 * factor of EWMA in update_avg().
	 */
	ഹാരകം = clamp_t(u32, (factor >> 2), 2, 8);
	*avg += div64_s64(diff, ഹാരകം);
}

സ്ഥിതം ശൂന്യം task_cache_work(ഘടന callback_head *work)
{
	ഘടന sched_cache_group *grp __free(sched_cache_group_put) = ശൂന്യ;
	cpumask_var_t cpus __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	പൂർണ്ണസംഖ്യ cpu, m_a_cpu = -1, nr_running = 0, curr_cpu;
	unsigned long next_scan, now = jiffies;
	ഘടന task_struct *p = current, *cur;
	unsigned long curr_m_a_occ = 0;
	unsigned long m_a_occ = 0;

	WARN_ON_ONCE(work != &p->cache_work);

	work->next = work;

	എങ്കിൽ (p->flags & PF_EXITING)
		തിരികെ;

	/*
	 * A reference makes sure grp is not released by others. The rcu
	 * lock can not be held till after zalloc_cpumask_var() below,
	 * because the latter might sleep.
	 */
	grp = task_cache_group_get(p);
	എങ്കിൽ (!grp)
		തിരികെ;

	next_scan = READ_ONCE(grp->next_scan);
	എങ്കിൽ (time_before(now, next_scan))
		തിരികെ;

	/* only 1 thread is allowed to scan */
	എങ്കിൽ (!try_cmpxchg(&grp->next_scan, &next_scan,
			 now + max_t(unsigned long,
				     READ_ONCE(llc_epoch_period), 1)))
		തിരികെ;

	curr_cpu = task_cpu(p);
	എങ്കിൽ (invalid_llc_nr(grp, p, curr_cpu) ||
	    exceed_llc_capacity(grp, curr_cpu)) {
		എങ്കിൽ (READ_ONCE(grp->cpu) != -1)
			WRITE_ONCE(grp->cpu, -1);

		തിരികെ;
	}

	എങ്കിൽ (!zalloc_cpumask_var(&cpus, GFP_KERNEL))
		തിരികെ;

	scoped_guard (cpus_read_lock) {
		guard(rcu)();

		get_scan_cpumasks(cpus, p, grp);

		for_each_cpu(cpu, cpus) {
			/* XXX sched_cluster_active */
			ഘടന sched_domain *sd = rcu_dereference_all(per_cpu(sd_llc, cpu));
			unsigned long occ, m_occ = 0, a_occ = 0;
			പൂർണ്ണസംഖ്യ m_cpu = -1, i;

			എങ്കിൽ (!sd)
				തുടരുക;

			for_each_cpu(i, sched_domain_span(sd)) {
				occ = fraction_mm_sched(cpu_rq(i),
							per_cpu_ptr(grp->pcpu_sched, i));
				a_occ += occ;
				എങ്കിൽ (occ > m_occ) {
					m_occ = occ;
					m_cpu = i;
				}

				/*
				 * rcu_access_pointer() is used because the
				 * pointer is only compared, never dereferenced.
				 */
				cur = rcu_dereference_all(cpu_rq(i)->curr);
				എങ്കിൽ (cur && !(cur->flags & (PF_EXITING | PF_KTHREAD)) &&
				    rcu_access_pointer(cur->sched_cache_grp) == grp)
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
			എങ്കിൽ (a_occ > m_a_occ) {
				m_a_occ = a_occ;
				m_a_cpu = m_cpu;
			}

			എങ്കിൽ (llc_id(cpu) == llc_id(READ_ONCE(grp->cpu)))
				curr_m_a_occ = a_occ;

			cpumask_andnot(cpus, cpus, sched_domain_span(sd));
		}
	}

	എങ്കിൽ (m_a_occ > (2 * curr_m_a_occ)) {
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

ശൂന്യം init_sched_mm(ഘടന task_struct *p)
{
	ഘടന callback_head *work = &p->cache_work;

	init_task_work(work, task_cache_work);
	work->next = work;
	/*
	 * dup_task_struct() copies the parent's task_struct, including its
	 * sched_cache_grp, for which the child holds no reference.  Clear it
	 * here - before copy_mm() runs - so the child never carries a
	 * borrowed pointer that the fork error path would put.
	 */
	RCU_INIT_POINTER(p->sched_cache_grp, ശൂന്യ);
	/*
	 * Reset new task's preference to avoid
	 * polluting account_llc_enqueue().
	 */
	p->preferred_llc = -1;
	p->pref_llc_queued = 0;
}

#അല്ലെങ്കിൽ /* CONFIG_SCHED_CACHE */

സ്ഥിതം വരിയിൽ ശൂന്യം account_mm_sched(ഘടന rq *rq, ഘടന task_struct *p,
				    s64 delta_exec) { }

ശൂന്യം init_sched_mm(ഘടന task_struct *p) { }

സ്ഥിതം ശൂന്യം task_tick_cache(ഘടന rq *rq, ഘടന task_struct *p) { }

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ get_pref_llc(ഘടന task_struct *p,
			       ഘടന mm_struct *mm)
{
	തിരികെ -1;
}

സ്ഥിതം ശൂന്യം account_llc_enqueue(ഘടന rq *rq, ഘടന task_struct *p) {}

സ്ഥിതം ശൂന്യം account_llc_dequeue(ഘടന rq *rq, ഘടന task_struct *p) {}

സ്ഥിതം ശൂന്യം pref_llc_running_inc(ഘടന rq *rq, ഘടന task_struct *p) {}

സ്ഥിതം ശൂന്യം pref_llc_running_dec(ഘടന rq *rq, ഘടന task_struct *p) {}

#endif /* CONFIG_SCHED_CACHE */

/*
 * Used by other classes to account runtime.
 */
s64 update_curr_common(ഘടന rq *rq)
{
	തിരികെ update_se(rq, &rq->donor->se);
}

/*
 * Update the current task's runtime statistics.
 */
സ്ഥിതം ശൂന്യം update_curr(ഘടന cfs_rq *cfs_rq)
{
	/*
	 * Note: cfs_rq->curr corresponds to the task picked to
	 * run (ie: rq->donor.se) which due to proxy-exec may
	 * not necessarily be the actual task running
	 * (rq->curr.se). This is easy to confuse!
	 */
	ഘടന sched_entity *curr = cfs_rq->h_curr;
	ഘടന rq *rq = rq_of(cfs_rq);
	s64 delta_exec;
	bool resched;

	എങ്കിൽ (unlikely(!curr))
		തിരികെ;

	delta_exec = update_se(rq, curr);
	എങ്കിൽ (unlikely(delta_exec <= 0))
		തിരികെ;

	account_cfs_rq_runtime(cfs_rq, delta_exec);

	എങ്കിൽ (!entity_is_task(curr))
		തിരികെ;

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

	എങ്കിൽ (cfs_rq->h_nr_queued == 1)
		തിരികെ;

	എങ്കിൽ (resched || !protect_slice(curr)) {
		resched_curr_lazy(rq);
		clear_buddies(cfs_rq, curr);
	}
}

സ്ഥിതം ശൂന്യം update_curr_fair(ഘടന rq *rq)
{
	ഘടന sched_entity *se = &rq->donor->se;

	for_each_sched_entity(se)
		update_curr(cfs_rq_of(se));
}

സ്ഥിതം വരിയിൽ ശൂന്യം
update_stats_wait_start_fair(ഘടന cfs_rq *cfs_rq, ഘടന sched_entity *se)
{
	ഘടന sched_statistics *stats;
	ഘടന task_struct *p = ശൂന്യ;

	എങ്കിൽ (!schedstat_enabled())
		തിരികെ;

	stats = __schedstats_from_se(se);

	എങ്കിൽ (entity_is_task(se))
		p = task_of(se);

	__update_stats_wait_start(rq_of(cfs_rq), p, stats);
}

സ്ഥിതം വരിയിൽ ശൂന്യം
update_stats_wait_end_fair(ഘടന cfs_rq *cfs_rq, ഘടന sched_entity *se)
{
	ഘടന sched_statistics *stats;
	ഘടന task_struct *p = ശൂന്യ;

	എങ്കിൽ (!schedstat_enabled())
		തിരികെ;

	stats = __schedstats_from_se(se);

	/*
	 * When the sched_schedstat changes from 0 to 1, some sched se
	 * maybe already in the runqueue, the se->statistics.wait_start
	 * will be 0.So it will let the delta wrong. We need to avoid this
	 * scenario.
	 */
	എങ്കിൽ (unlikely(!schedstat_val(stats->wait_start)))
		തിരികെ;

	എങ്കിൽ (entity_is_task(se))
		p = task_of(se);

	__update_stats_wait_end(rq_of(cfs_rq), p, stats);
}

സ്ഥിതം വരിയിൽ ശൂന്യം
update_stats_enqueue_sleeper_fair(ഘടന cfs_rq *cfs_rq, ഘടന sched_entity *se)
{
	ഘടന sched_statistics *stats;
	ഘടന task_struct *tsk = ശൂന്യ;

	എങ്കിൽ (!schedstat_enabled())
		തിരികെ;

	stats = __schedstats_from_se(se);

	എങ്കിൽ (entity_is_task(se))
		tsk = task_of(se);

	__update_stats_enqueue_sleeper(rq_of(cfs_rq), tsk, stats);
}

/*
 * Task is being enqueued - update stats:
 */
സ്ഥിതം വരിയിൽ ശൂന്യം
update_stats_enqueue_fair(ഘടന cfs_rq *cfs_rq, ഘടന sched_entity *se, പൂർണ്ണസംഖ്യ flags)
{
	എങ്കിൽ (!schedstat_enabled())
		തിരികെ;

	/*
	 * Are we enqueueing a waiting task? (for current tasks
	 * a dequeue/enqueue event is a NOP)
	 */
	എങ്കിൽ (se != cfs_rq->h_curr)
		update_stats_wait_start_fair(cfs_rq, se);

	എങ്കിൽ (flags & ENQUEUE_WAKEUP)
		update_stats_enqueue_sleeper_fair(cfs_rq, se);
}

സ്ഥിതം വരിയിൽ ശൂന്യം
update_stats_dequeue_fair(ഘടന cfs_rq *cfs_rq, ഘടന sched_entity *se, പൂർണ്ണസംഖ്യ flags)
{

	എങ്കിൽ (!schedstat_enabled())
		തിരികെ;

	/*
	 * Mark the end of the wait period if dequeueing a
	 * waiting task:
	 */
	എങ്കിൽ (se != cfs_rq->h_curr)
		update_stats_wait_end_fair(cfs_rq, se);

	എങ്കിൽ ((flags & DEQUEUE_SLEEP) && entity_is_task(se)) {
		ഘടന task_struct *tsk = task_of(se);
		unsigned പൂർണ്ണസംഖ്യ state;

		/* XXX racy against TTWU */
		state = READ_ONCE(tsk->__state);
		എങ്കിൽ (state & TASK_INTERRUPTIBLE)
			__schedstat_set(tsk->stats.sleep_start,
				      rq_clock(rq_of(cfs_rq)));
		എങ്കിൽ (state & TASK_UNINTERRUPTIBLE)
			__schedstat_set(tsk->stats.block_start,
				      rq_clock(rq_of(cfs_rq)));
	}
}

/*
 * We are picking a new current task - update its stats:
 */
സ്ഥിതം വരിയിൽ ശൂന്യം
update_stats_curr_start(ഘടന cfs_rq *cfs_rq, ഘടന sched_entity *se)
{
	/*
	 * We are starting a new run period:
	 */
	se->exec_start = rq_clock_task(rq_of(cfs_rq));
}

/* Check sched_smt_active before calling this to avoid overheads in fastpaths */
സ്ഥിതം വരിയിൽ bool is_core_idle(പൂർണ്ണസംഖ്യ cpu)
{
	പൂർണ്ണസംഖ്യ sibling;

	for_each_cpu(sibling, cpu_smt_mask(cpu)) {
		എങ്കിൽ (cpu == sibling)
			തുടരുക;

		എങ്കിൽ (!idle_cpu(sibling))
			തിരികെ false;
	}

	തിരികെ true;
}

#ifdef CONFIG_NUMA
#define NUMA_IMBALANCE_MIN 2

സ്ഥിതം വരിയിൽ long
adjust_numa_imbalance(പൂർണ്ണസംഖ്യ imbalance, പൂർണ്ണസംഖ്യ dst_running, പൂർണ്ണസംഖ്യ imb_numa_nr)
{
	/*
	 * Allow a NUMA imbalance if busy CPUs is less than the maximum
	 * threshold. Above this threshold, individual tasks may be contending
	 * for both memory bandwidth and any shared HT resources.  This is an
	 * approximation as the number of running tasks may not be related to
	 * the number of busy CPUs due to sched_setaffinity.
	 */
	എങ്കിൽ (dst_running > imb_numa_nr)
		തിരികെ imbalance;

	/*
	 * Allow a small imbalance based on a simple pair of communicating
	 * tasks that remain local when the destination is lightly loaded.
	 */
	എങ്കിൽ (imbalance <= NUMA_IMBALANCE_MIN)
		തിരികെ 0;

	തിരികെ imbalance;
}
#endif /* CONFIG_NUMA */

#ifdef CONFIG_NUMA_BALANCING
/*
 * Approximate time to scan a full NUMA task in ms. The task scan period is
 * calculated based on the tasks virtual memory size and
 * numa_balancing_scan_size.
 */
unsigned പൂർണ്ണസംഖ്യ sysctl_numa_balancing_scan_period_min = 1000;
unsigned പൂർണ്ണസംഖ്യ sysctl_numa_balancing_scan_period_max = 60000;

/* Portion of address space to scan in MB */
unsigned പൂർണ്ണസംഖ്യ sysctl_numa_balancing_scan_size = 256;

/* Scan @scan_size MB every @scan_period after an initial @scan_delay in ms */
unsigned പൂർണ്ണസംഖ്യ sysctl_numa_balancing_scan_delay = 1000;

/* The page with hint page fault latency < threshold in ms is considered hot */
unsigned പൂർണ്ണസംഖ്യ sysctl_numa_balancing_hot_threshold = MSEC_PER_SEC;

ഘടന numa_group {
	refcount_t refcount;

	spinlock_t lock; /* nr_tasks, tasks */
	പൂർണ്ണസംഖ്യ nr_tasks;
	pid_t gid;
	പൂർണ്ണസംഖ്യ active_nodes;

	ഘടന rcu_head rcu;
	unsigned long total_faults;
	unsigned long max_faults_cpu;
	/*
	 * faults[] array is split into two regions: faults_mem and faults_cpu.
	 *
	 * Faults_cpu is used to decide whether memory should move
	 * towards the CPU. As a consequence, these stats are weighted
	 * more by CPU use than by memory faults.
	 */
	unsigned long faults[];
};

/*
 * For functions that can be called in multiple contexts that permit reading
 * ->numa_group (see struct task_struct for locking rules).
 */
സ്ഥിതം ഘടന numa_group *deref_task_numa_group(ഘടന task_struct *p)
{
	തിരികെ rcu_dereference_check(p->numa_group, p == current ||
		(lockdep_is_held(__rq_lockp(task_rq(p))) && !READ_ONCE(p->on_cpu)));
}

സ്ഥിതം ഘടന numa_group *deref_curr_numa_group(ഘടന task_struct *p)
{
	തിരികെ rcu_dereference_protected(p->numa_group, p == current);
}

സ്ഥിതം വരിയിൽ unsigned long group_faults_priv(ഘടന numa_group *ng);
സ്ഥിതം വരിയിൽ unsigned long group_faults_shared(ഘടന numa_group *ng);

സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ task_nr_scan_windows(ഘടന task_struct *p)
{
	unsigned long rss = 0;
	unsigned long nr_scan_pages;

	/*
	 * Calculations based on RSS as non-present and empty pages are skipped
	 * by the PTE scanner and NUMA hinting faults should be trapped based
	 * on resident pages
	 */
	nr_scan_pages = MB_TO_PAGES(sysctl_numa_balancing_scan_size);
	rss = get_mm_rss(p->mm);
	എങ്കിൽ (!rss)
		rss = nr_scan_pages;

	rss = round_up(rss, nr_scan_pages);
	തിരികെ rss / nr_scan_pages;
}

/* For sanity's sake, never scan more PTEs than MAX_SCAN_WINDOW MB/sec. */
#define MAX_SCAN_WINDOW 2560

സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ task_scan_min(ഘടന task_struct *p)
{
	unsigned പൂർണ്ണസംഖ്യ scan_size = READ_ONCE(sysctl_numa_balancing_scan_size);
	unsigned പൂർണ്ണസംഖ്യ scan, floor;
	unsigned പൂർണ്ണസംഖ്യ windows = 1;

	എങ്കിൽ (scan_size < MAX_SCAN_WINDOW)
		windows = MAX_SCAN_WINDOW / scan_size;
	floor = 1000 / windows;

	scan = sysctl_numa_balancing_scan_period_min / task_nr_scan_windows(p);
	തിരികെ max_t(unsigned പൂർണ്ണസംഖ്യ, floor, scan);
}

സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ task_scan_start(ഘടന task_struct *p)
{
	unsigned long smin = task_scan_min(p);
	unsigned long period = smin;
	ഘടന numa_group *ng;

	/* Scale the maximum scan period with the amount of shared memory. */
	rcu_read_lock();
	ng = rcu_dereference_all(p->numa_group);
	എങ്കിൽ (ng) {
		unsigned long shared = group_faults_shared(ng);
		unsigned long സ്വകാര്യ = group_faults_priv(ng);

		period *= refcount_read(&ng->refcount);
		period *= shared + 1;
		period /= സ്വകാര്യ + shared + 1;
	}
	rcu_read_unlock();

	തിരികെ max(smin, period);
}

സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ task_scan_max(ഘടന task_struct *p)
{
	unsigned long smin = task_scan_min(p);
	unsigned long smax;
	ഘടന numa_group *ng;

	/* Watch for min being lower than max due to floor calculations */
	smax = sysctl_numa_balancing_scan_period_max / task_nr_scan_windows(p);

	/* Scale the maximum scan period with the amount of shared memory. */
	ng = deref_curr_numa_group(p);
	എങ്കിൽ (ng) {
		unsigned long shared = group_faults_shared(ng);
		unsigned long സ്വകാര്യ = group_faults_priv(ng);
		unsigned long period = smax;

		period *= refcount_read(&ng->refcount);
		period *= shared + 1;
		period /= സ്വകാര്യ + shared + 1;

		smax = max(smax, period);
	}

	തിരികെ max(smin, smax);
}

സ്ഥിതം ശൂന്യം account_numa_enqueue(ഘടന rq *rq, ഘടന task_struct *p)
{
	rq->nr_numa_running += (p->numa_preferred_nid != NUMA_NO_കണ്ണി);
	rq->nr_preferred_running += (p->numa_preferred_nid == task_കണ്ണി(p));
}

സ്ഥിതം ശൂന്യം account_numa_dequeue(ഘടന rq *rq, ഘടന task_struct *p)
{
	rq->nr_numa_running -= (p->numa_preferred_nid != NUMA_NO_കണ്ണി);
	rq->nr_preferred_running -= (p->numa_preferred_nid == task_കണ്ണി(p));
}

/* Shared or private faults. */
#define NR_NUMA_HINT_FAULT_TYPES 2

/* Memory and CPU locality */
#define NR_NUMA_HINT_FAULT_STATS (NR_NUMA_HINT_FAULT_TYPES * 2)

/* Averaged statistics, and temporary buffers. */
#define NR_NUMA_HINT_FAULT_BUCKETS (NR_NUMA_HINT_FAULT_STATS * 2)

pid_t task_numa_group_id(ഘടന task_struct *p)
{
	ഘടന numa_group *ng;
	pid_t gid = 0;

	rcu_read_lock();
	ng = rcu_dereference_all(p->numa_group);
	എങ്കിൽ (ng)
		gid = ng->gid;
	rcu_read_unlock();

	തിരികെ gid;
}

/*
 * The averaged statistics, shared & private, memory & CPU,
 * occupy the first half of the array. The second half of the
 * array is for current counters, which are averaged into the
 * first set by task_numa_placement.
 */
സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ task_faults_idx(എണ്ണൽ numa_faults_stats s, പൂർണ്ണസംഖ്യ nid, പൂർണ്ണസംഖ്യ priv)
{
	തിരികെ NR_NUMA_HINT_FAULT_TYPES * (s * nr_കണ്ണി_ids + nid) + priv;
}

സ്ഥിതം വരിയിൽ unsigned long task_faults(ഘടന task_struct *p, പൂർണ്ണസംഖ്യ nid)
{
	എങ്കിൽ (!p->numa_faults)
		തിരികെ 0;

	തിരികെ p->numa_faults[task_faults_idx(NUMA_MEM, nid, 0)] +
		p->numa_faults[task_faults_idx(NUMA_MEM, nid, 1)];
}

സ്ഥിതം വരിയിൽ unsigned long group_faults(ഘടന task_struct *p, പൂർണ്ണസംഖ്യ nid)
{
	ഘടന numa_group *ng = deref_task_numa_group(p);

	എങ്കിൽ (!ng)
		തിരികെ 0;

	തിരികെ ng->faults[task_faults_idx(NUMA_MEM, nid, 0)] +
		ng->faults[task_faults_idx(NUMA_MEM, nid, 1)];
}

സ്ഥിതം വരിയിൽ unsigned long group_faults_cpu(ഘടന numa_group *group, പൂർണ്ണസംഖ്യ nid)
{
	തിരികെ group->faults[task_faults_idx(NUMA_CPU, nid, 0)] +
		group->faults[task_faults_idx(NUMA_CPU, nid, 1)];
}

സ്ഥിതം വരിയിൽ unsigned long group_faults_priv(ഘടന numa_group *ng)
{
	unsigned long faults = 0;
	പൂർണ്ണസംഖ്യ കണ്ണി;

	for_each_online_കണ്ണി(കണ്ണി) {
		faults += ng->faults[task_faults_idx(NUMA_MEM, കണ്ണി, 1)];
	}

	തിരികെ faults;
}

സ്ഥിതം വരിയിൽ unsigned long group_faults_shared(ഘടന numa_group *ng)
{
	unsigned long faults = 0;
	പൂർണ്ണസംഖ്യ കണ്ണി;

	for_each_online_കണ്ണി(കണ്ണി) {
		faults += ng->faults[task_faults_idx(NUMA_MEM, കണ്ണി, 0)];
	}

	തിരികെ faults;
}

/*
 * A node triggering more than 1/3 as many NUMA faults as the maximum is
 * considered part of a numa group's pseudo-interleaving set. Migrations
 * between these nodes are slowed down, to allow things to settle down.
 */
#define ACTIVE_കണ്ണി_FRACTION 3

സ്ഥിതം bool numa_is_active_കണ്ണി(പൂർണ്ണസംഖ്യ nid, ഘടന numa_group *ng)
{
	തിരികെ group_faults_cpu(ng, nid) * ACTIVE_കണ്ണി_FRACTION > ng->max_faults_cpu;
}

/* Handle placement on systems where not all nodes are directly connected. */
സ്ഥിതം unsigned long score_nearby_nodes(ഘടന task_struct *p, പൂർണ്ണസംഖ്യ nid,
					പൂർണ്ണസംഖ്യ lim_dist, bool task)
{
	unsigned long score = 0;
	പൂർണ്ണസംഖ്യ കണ്ണി, max_dist;

	/*
	 * All nodes are directly connected, and the same distance
	 * from each other. No need for fancy placement algorithms.
	 */
	എങ്കിൽ (sched_numa_topology_type == NUMA_DIRECT)
		തിരികെ 0;

	/* sched_max_numa_distance may be changed in parallel. */
	max_dist = READ_ONCE(sched_max_numa_distance);
	/*
	 * This code is called for each node, introducing N^2 complexity,
	 * which should be OK given the number of nodes rarely exceeds 8.
	 */
	for_each_online_കണ്ണി(കണ്ണി) {
		unsigned long faults;
		പൂർണ്ണസംഖ്യ dist = കണ്ണി_distance(nid, കണ്ണി);

		/*
		 * The furthest away nodes in the system are not interesting
		 * for placement; nid was already counted.
		 */
		എങ്കിൽ (dist >= max_dist || കണ്ണി == nid)
			തുടരുക;

		/*
		 * On systems with a backplane NUMA topology, compare groups
		 * of nodes, and move tasks towards the group with the most
		 * memory accesses. When comparing two nodes at distance
		 * "hoplimit", only nodes closer by than "hoplimit" are part
		 * of each group. Skip other nodes.
		 */
		എങ്കിൽ (sched_numa_topology_type == NUMA_BACKPLANE && dist >= lim_dist)
			തുടരുക;

		/* Add up the faults from nearby nodes. */
		എങ്കിൽ (task)
			faults = task_faults(p, കണ്ണി);
		അല്ലെങ്കിൽ
			faults = group_faults(p, കണ്ണി);

		/*
		 * On systems with a glueless mesh NUMA topology, there are
		 * no fixed "groups of nodes". Instead, nodes that are not
		 * directly connected bounce traffic through intermediate
		 * nodes; a numa_group can occupy any set of nodes.
		 * The further away a node is, the less the faults count.
		 * This seems to result in good task placement.
		 */
		എങ്കിൽ (sched_numa_topology_type == NUMA_GLUELESS_MESH) {
			faults *= (max_dist - dist);
			faults /= (max_dist - LOCAL_DISTANCE);
		}

		score += faults;
	}

	തിരികെ score;
}

/*
 * These return the fraction of accesses done by a particular task, or
 * task group, on a particular numa node.  The group weight is given a
 * larger multiplier, in order to group tasks together that are almost
 * evenly spread out between numa nodes.
 */
സ്ഥിതം വരിയിൽ unsigned long task_weight(ഘടന task_struct *p, പൂർണ്ണസംഖ്യ nid,
					പൂർണ്ണസംഖ്യ dist)
{
	unsigned long faults, total_faults;

	എങ്കിൽ (!p->numa_faults)
		തിരികെ 0;

	total_faults = p->total_numa_faults;

	എങ്കിൽ (!total_faults)
		തിരികെ 0;

	faults = task_faults(p, nid);
	faults += score_nearby_nodes(p, nid, dist, true);

	തിരികെ 1000 * faults / total_faults;
}

സ്ഥിതം വരിയിൽ unsigned long group_weight(ഘടന task_struct *p, പൂർണ്ണസംഖ്യ nid,
					 പൂർണ്ണസംഖ്യ dist)
{
	ഘടന numa_group *ng = deref_task_numa_group(p);
	unsigned long faults, total_faults;

	എങ്കിൽ (!ng)
		തിരികെ 0;

	total_faults = ng->total_faults;

	എങ്കിൽ (!total_faults)
		തിരികെ 0;

	faults = group_faults(p, nid);
	faults += score_nearby_nodes(p, nid, dist, false);

	തിരികെ 1000 * faults / total_faults;
}

/*
 * If memory tiering mode is enabled, cpupid of slow memory page is
 * used to record scan time instead of CPU and PID.  When tiering mode
 * is disabled at run time, the scan time (in cpupid) will be
 * interpreted as CPU and PID.  So CPU needs to be checked to avoid to
 * access out of array bound.
 */
സ്ഥിതം വരിയിൽ bool cpupid_valid(പൂർണ്ണസംഖ്യ cpupid)
{
	തിരികെ cpupid_to_cpu(cpupid) < nr_cpu_ids;
}

/*
 * For memory tiering mode, if there are enough free pages (more than
 * enough watermark defined here) in fast memory node, to take full
 * advantage of fast memory capacity, all recently accessed slow
 * memory pages will be migrated to fast memory node without
 * considering hot threshold.
 */
സ്ഥിതം bool pgdat_free_space_enough(ഘടന pglist_data *pgdat)
{
	പൂർണ്ണസംഖ്യ z;
	unsigned long enough_wmark;

	enough_wmark = max(1UL * 1024 * 1024 * 1024 >> PAGE_SHIFT,
			   pgdat->കണ്ണി_present_pages >> 4);
	വേണ്ടി (z = pgdat->nr_zones - 1; z >= 0; z--) {
		ഘടന zone *zone = pgdat->കണ്ണി_zones + z;

		എങ്കിൽ (!populated_zone(zone))
			തുടരുക;

		എങ്കിൽ (zone_watermark_ok(zone, 0,
				      promo_wmark_pages(zone) + enough_wmark,
				      ZONE_MOVABLE, 0))
			തിരികെ true;
	}
	തിരികെ false;
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
സ്ഥിതം പൂർണ്ണസംഖ്യ numa_hint_fault_latency(ഘടന folio *folio)
{
	പൂർണ്ണസംഖ്യ last_time, time;

	time = jiffies_to_msecs(jiffies);
	last_time = folio_xchg_access_time(folio, time);

	തിരികെ (time - last_time) & PAGE_ACCESS_TIME_MASK;
}

/*
 * For memory tiering mode, too high promotion/demotion throughput may
 * hurt application latency.  So we provide a mechanism to rate limit
 * the number of pages that are tried to be promoted.
 */
സ്ഥിതം bool numa_promotion_rate_limit(ഘടന pglist_data *pgdat,
				      unsigned long rate_limit, പൂർണ്ണസംഖ്യ nr)
{
	unsigned long nr_cand;
	unsigned പൂർണ്ണസംഖ്യ now, start;

	now = jiffies_to_msecs(jiffies);
	mod_കണ്ണി_page_state(pgdat, PGPROMOTE_CANDIDATE, nr);
	nr_cand = കണ്ണി_page_state(pgdat, PGPROMOTE_CANDIDATE);
	start = pgdat->nbp_rl_start;
	എങ്കിൽ (now - start > MSEC_PER_SEC &&
	    cmpxchg(&pgdat->nbp_rl_start, start, now) == start)
		pgdat->nbp_rl_nr_cand = nr_cand;
	എങ്കിൽ (nr_cand - pgdat->nbp_rl_nr_cand >= rate_limit)
		തിരികെ true;
	തിരികെ false;
}

#define NUMA_MIGRATION_ADJUST_STEPS	16

സ്ഥിതം ശൂന്യം numa_promotion_adjust_threshold(ഘടന pglist_data *pgdat,
					    unsigned long rate_limit,
					    unsigned പൂർണ്ണസംഖ്യ ref_th)
{
	unsigned പൂർണ്ണസംഖ്യ now, start, th_period, unit_th, th;
	unsigned long nr_cand, ref_cand, diff_cand;

	now = jiffies_to_msecs(jiffies);
	th_period = sysctl_numa_balancing_scan_period_max;
	start = pgdat->nbp_th_start;
	എങ്കിൽ (now - start > th_period &&
	    cmpxchg(&pgdat->nbp_th_start, start, now) == start) {
		ref_cand = rate_limit *
			sysctl_numa_balancing_scan_period_max / MSEC_PER_SEC;
		nr_cand = കണ്ണി_page_state(pgdat, PGPROMOTE_CANDIDATE);
		diff_cand = nr_cand - pgdat->nbp_th_nr_cand;
		unit_th = ref_th * 2 / NUMA_MIGRATION_ADJUST_STEPS;
		th = pgdat->nbp_threshold ? : ref_th;
		എങ്കിൽ (diff_cand > ref_cand * 11 / 10)
			th = max(th - unit_th, unit_th);
		അല്ലെങ്കിൽ എങ്കിൽ (diff_cand < ref_cand * 9 / 10)
			th = min(th + unit_th, ref_th * 2);
		pgdat->nbp_th_nr_cand = nr_cand;
		pgdat->nbp_threshold = th;
	}
}

bool should_numa_migrate_memory(ഘടന task_struct *p, ഘടന folio *folio,
				പൂർണ്ണസംഖ്യ src_nid, പൂർണ്ണസംഖ്യ dst_cpu)
{
	ഘടന numa_group *ng = deref_curr_numa_group(p);
	പൂർണ്ണസംഖ്യ dst_nid = cpu_to_കണ്ണി(dst_cpu);
	പൂർണ്ണസംഖ്യ last_cpupid, this_cpupid;

	/*
	 * Cannot migrate to memoryless nodes.
	 */
	എങ്കിൽ (!കണ്ണി_state(dst_nid, N_MEMORY))
		തിരികെ false;

	/*
	 * The pages in slow memory node should be migrated according
	 * to hot/cold instead of private/shared.
	 */
	എങ്കിൽ (folio_use_access_time(folio)) {
		ഘടന pglist_data *pgdat;
		unsigned long rate_limit;
		unsigned പൂർണ്ണസംഖ്യ latency, th, def_th;
		long nr = folio_nr_pages(folio);

		pgdat = കണ്ണി_DATA(dst_nid);
		എങ്കിൽ (pgdat_free_space_enough(pgdat)) {
			/* workload changed, reset hot threshold */
			pgdat->nbp_threshold = 0;
			mod_കണ്ണി_page_state(pgdat, PGPROMOTE_CANDIDATE_NRL, nr);
			തിരികെ true;
		}

		def_th = sysctl_numa_balancing_hot_threshold;
		rate_limit = MB_TO_PAGES(sysctl_numa_balancing_promote_rate_limit);
		numa_promotion_adjust_threshold(pgdat, rate_limit, def_th);

		th = pgdat->nbp_threshold ? : def_th;
		latency = numa_hint_fault_latency(folio);
		എങ്കിൽ (latency >= th)
			തിരികെ false;

		തിരികെ !numa_promotion_rate_limit(pgdat, rate_limit, nr);
	}

	this_cpupid = cpu_pid_to_cpupid(dst_cpu, current->pid);
	last_cpupid = folio_xchg_last_cpupid(folio, this_cpupid);

	എങ്കിൽ (!(sysctl_numa_balancing_mode & NUMA_BALANCING_MEMORY_TIERING) &&
	    !കണ്ണി_is_toptier(src_nid) && !cpupid_valid(last_cpupid))
		തിരികെ false;

	/*
	 * Allow first faults or private faults to migrate immediately early in
	 * the lifetime of a task. The magic number 4 is based on waiting for
	 * two full passes of the "multi-stage node selection" test that is
	 * executed below.
	 */
	എങ്കിൽ ((p->numa_preferred_nid == NUMA_NO_കണ്ണി || p->numa_scan_seq <= 4) &&
	    (cpupid_pid_unset(last_cpupid) || cpupid_match_pid(p, last_cpupid)))
		തിരികെ true;

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
	എങ്കിൽ (!cpupid_pid_unset(last_cpupid) &&
				cpupid_to_nid(last_cpupid) != dst_nid)
		തിരികെ false;

	/* Always allow migrate on private faults */
	എങ്കിൽ (cpupid_match_pid(p, last_cpupid))
		തിരികെ true;

	/* A shared fault, but p->numa_group has not been set up yet. */
	എങ്കിൽ (!ng)
		തിരികെ true;

	/*
	 * Destination node is much more heavily used than the source
	 * node? Allow migration.
	 */
	എങ്കിൽ (group_faults_cpu(ng, dst_nid) > group_faults_cpu(ng, src_nid) *
					ACTIVE_കണ്ണി_FRACTION)
		തിരികെ true;

	/*
	 * Distribute memory according to CPU & memory use on each node,
	 * with 3/4 hysteresis to avoid unnecessary memory migrations:
	 *
	 * faults_cpu(dst)   3   faults_cpu(src)
	 * --------------- * - > ---------------
	 * faults_mem(dst)   4   faults_mem(src)
	 */
	തിരികെ group_faults_cpu(ng, dst_nid) * group_faults(p, src_nid) * 3 >
	       group_faults_cpu(ng, src_nid) * group_faults(p, dst_nid) * 4;
}

/*
 * 'numa_type' describes the node at the moment of load balancing.
 */
എണ്ണൽ numa_type {
	/* The node has spare capacity that can be used to run more tasks.  */
	കണ്ണി_has_spare = 0,
	/*
	 * The node is fully used and the tasks don't compete for more CPU
	 * cycles. Nevertheless, some tasks might wait before running.
	 */
	കണ്ണി_fully_busy,
	/*
	 * The node is overloaded and can't provide expected CPU cycles to all
	 * tasks.
	 */
	കണ്ണി_overloaded
};

/* Cached statistics for all CPUs within a node */
ഘടന numa_stats {
	unsigned long load;
	unsigned long runnable;
	unsigned long util;
	/* Total compute capacity of CPUs on a node */
	unsigned long compute_capacity;
	unsigned പൂർണ്ണസംഖ്യ nr_running;
	unsigned പൂർണ്ണസംഖ്യ weight;
	എണ്ണൽ numa_type കണ്ണി_type;
	പൂർണ്ണസംഖ്യ idle_cpu;
};

ഘടന task_numa_env {
	ഘടന task_struct *p;

	പൂർണ്ണസംഖ്യ src_cpu, src_nid;
	പൂർണ്ണസംഖ്യ dst_cpu, dst_nid;
	പൂർണ്ണസംഖ്യ imb_numa_nr;

	ഘടന numa_stats src_stats, dst_stats;

	പൂർണ്ണസംഖ്യ imbalance_pct;
	പൂർണ്ണസംഖ്യ dist;

	ഘടന task_struct *best_task;
	long best_imp;
	പൂർണ്ണസംഖ്യ best_cpu;
};

സ്ഥിതം unsigned long cpu_load(ഘടന rq *rq);
സ്ഥിതം unsigned long cpu_runnable(ഘടന rq *rq);

സ്ഥിതം വരിയിൽ എണ്ണൽ
numa_type numa_classify(unsigned പൂർണ്ണസംഖ്യ imbalance_pct,
			 ഘടന numa_stats *ns)
{
	എങ്കിൽ ((ns->nr_running > ns->weight) &&
	    (((ns->compute_capacity * 100) < (ns->util * imbalance_pct)) ||
	     ((ns->compute_capacity * imbalance_pct) < (ns->runnable * 100))))
		തിരികെ കണ്ണി_overloaded;

	എങ്കിൽ ((ns->nr_running < ns->weight) ||
	    (((ns->compute_capacity * 100) > (ns->util * imbalance_pct)) &&
	     ((ns->compute_capacity * imbalance_pct) > (ns->runnable * 100))))
		തിരികെ കണ്ണി_has_spare;

	തിരികെ കണ്ണി_fully_busy;
}

/* Forward declarations of select_idle_sibling helpers */
സ്ഥിതം വരിയിൽ bool test_idle_cores(പൂർണ്ണസംഖ്യ cpu);
സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ numa_idle_core(പൂർണ്ണസംഖ്യ idle_core, പൂർണ്ണസംഖ്യ cpu)
{
	എങ്കിൽ (!sched_smt_active() ||
	    idle_core >= 0 || !test_idle_cores(cpu))
		തിരികെ idle_core;

	/*
	 * Prefer cores instead of packing HT siblings
	 * and triggering future load balancing.
	 */
	എങ്കിൽ (is_core_idle(cpu))
		idle_core = cpu;

	തിരികെ idle_core;
}

/*
 * Gather all necessary information to make NUMA balancing placement
 * decisions that are compatible with standard load balancer. This
 * borrows code and logic from update_sg_lb_stats but sharing a
 * common implementation is impractical.
 */
സ്ഥിതം ശൂന്യം update_numa_stats(ഘടന task_numa_env *env,
			      ഘടന numa_stats *ns, പൂർണ്ണസംഖ്യ nid,
			      bool find_idle)
{
	പൂർണ്ണസംഖ്യ cpu, idle_core = -1;

	memset(ns, 0, വലുപ്പം(*ns));
	ns->idle_cpu = -1;

	rcu_read_lock();
	for_each_cpu(cpu, cpumask_of_കണ്ണി(nid)) {
		ഘടന rq *rq = cpu_rq(cpu);

		ns->load += cpu_load(rq);
		ns->runnable += cpu_runnable(rq);
		ns->util += cpu_util_cfs(cpu);
		ns->nr_running += rq->cfs.h_nr_runnable;
		ns->compute_capacity += capacity_of(cpu);

		എങ്കിൽ (find_idle && idle_core < 0 && !rq->nr_running && idle_cpu(cpu)) {
			എങ്കിൽ (READ_ONCE(rq->numa_migrate_on) ||
			    !cpumask_test_cpu(cpu, env->p->cpus_ptr))
				തുടരുക;

			എങ്കിൽ (ns->idle_cpu == -1)
				ns->idle_cpu = cpu;

			idle_core = numa_idle_core(idle_core, cpu);
		}
	}
	rcu_read_unlock();

	ns->weight = cpumask_weight(cpumask_of_കണ്ണി(nid));

	ns->കണ്ണി_type = numa_classify(env->imbalance_pct, ns);

	എങ്കിൽ (idle_core >= 0)
		ns->idle_cpu = idle_core;
}

സ്ഥിതം ശൂന്യം task_numa_assign(ഘടന task_numa_env *env,
			     ഘടന task_struct *p, long imp)
{
	ഘടന rq *rq = cpu_rq(env->dst_cpu);

	/* Check if run-queue part of active NUMA balance. */
	എങ്കിൽ (env->best_cpu != env->dst_cpu && xchg(&rq->numa_migrate_on, 1)) {
		പൂർണ്ണസംഖ്യ cpu;
		പൂർണ്ണസംഖ്യ start = env->dst_cpu;

		/* Find alternative idle CPU. */
		for_each_cpu_wrap(cpu, cpumask_of_കണ്ണി(env->dst_nid), start + 1) {
			എങ്കിൽ (cpu == env->best_cpu || !idle_cpu(cpu) ||
			    !cpumask_test_cpu(cpu, env->p->cpus_ptr)) {
				തുടരുക;
			}

			env->dst_cpu = cpu;
			rq = cpu_rq(env->dst_cpu);
			എങ്കിൽ (!xchg(&rq->numa_migrate_on, 1))
				പോകുക assign;
		}

		/* Failed to find an alternative idle CPU */
		തിരികെ;
	}

assign:
	/*
	 * Clear previous best_cpu/rq numa-migrate flag, since task now
	 * found a better CPU to move/swap.
	 */
	എങ്കിൽ (env->best_cpu != -1 && env->best_cpu != env->dst_cpu) {
		rq = cpu_rq(env->best_cpu);
		WRITE_ONCE(rq->numa_migrate_on, 0);
	}

	എങ്കിൽ (env->best_task)
		put_task_struct(env->best_task);
	എങ്കിൽ (p)
		get_task_struct(p);

	env->best_task = p;
	env->best_imp = imp;
	env->best_cpu = env->dst_cpu;
}

സ്ഥിതം bool load_too_imbalanced(long src_load, long dst_load,
				ഘടന task_numa_env *env)
{
	long imb, old_imb;
	long orig_src_load, orig_dst_load;
	long src_capacity, dst_capacity;

	/*
	 * The load is corrected for the CPU capacity available on each node.
	 *
	 * src_load        dst_load
	 * ------------ vs ---------
	 * src_capacity    dst_capacity
	 */
	src_capacity = env->src_stats.compute_capacity;
	dst_capacity = env->dst_stats.compute_capacity;

	imb = abs(dst_load * src_capacity - src_load * dst_capacity);

	orig_src_load = env->src_stats.load;
	orig_dst_load = env->dst_stats.load;

	old_imb = abs(orig_dst_load * src_capacity - orig_src_load * dst_capacity);

	/* Would this change make things worse? */
	തിരികെ (imb > old_imb);
}

/*
 * Maximum NUMA importance can be 1998 (2*999);
 * SMALLIMP @ 30 would be close to 1998/64.
 * Used to deter task migration.
 */
#define SMALLIMP	30

/*
 * This checks if the overall compute and NUMA accesses of the system would
 * be improved if the source tasks was migrated to the target dst_cpu taking
 * into account that it might be best if task running on the dst_cpu should
 * be exchanged with the source task
 */
സ്ഥിതം bool task_numa_compare(ഘടന task_numa_env *env,
			      long taskimp, long groupimp, bool maymove)
{
	ഘടന numa_group *cur_ng, *p_ng = deref_curr_numa_group(env->p);
	ഘടന rq *dst_rq = cpu_rq(env->dst_cpu);
	long imp = p_ng ? groupimp : taskimp;
	ഘടന task_struct *cur;
	long src_load, dst_load;
	പൂർണ്ണസംഖ്യ dist = env->dist;
	long moveimp = imp;
	long load;
	bool stopsearch = false;

	എങ്കിൽ (READ_ONCE(dst_rq->numa_migrate_on))
		തിരികെ false;

	rcu_read_lock();
	cur = rcu_dereference_all(dst_rq->curr);
	എങ്കിൽ (cur && ((cur->flags & (PF_EXITING | PF_KTHREAD)) ||
		    !cur->mm))
		cur = ശൂന്യ;

	/*
	 * Because we have preemption enabled we can get migrated around and
	 * end try selecting ourselves (current == env->p) as a swap candidate.
	 */
	എങ്കിൽ (cur == env->p) {
		stopsearch = true;
		പോകുക unlock;
	}

	എങ്കിൽ (!cur) {
		എങ്കിൽ (maymove && moveimp >= env->best_imp)
			പോകുക assign;
		അല്ലെങ്കിൽ
			പോകുക unlock;
	}

	/* Skip this swap candidate if cannot move to the source cpu. */
	എങ്കിൽ (!cpumask_test_cpu(env->src_cpu, cur->cpus_ptr))
		പോകുക unlock;

	/*
	 * Skip this swap candidate if it is not moving to its preferred
	 * node and the best task is.
	 */
	എങ്കിൽ (env->best_task &&
	    env->best_task->numa_preferred_nid == env->src_nid &&
	    cur->numa_preferred_nid != env->src_nid) {
		പോകുക unlock;
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
	cur_ng = rcu_dereference_all(cur->numa_group);
	എങ്കിൽ (cur_ng == p_ng) {
		/*
		 * Do not swap within a group or between tasks that have
		 * no group if there is spare capacity. Swapping does
		 * not address the load imbalance and helps one task at
		 * the cost of punishing another.
		 */
		എങ്കിൽ (env->dst_stats.കണ്ണി_type == കണ്ണി_has_spare)
			പോകുക unlock;

		imp = taskimp + task_weight(cur, env->src_nid, dist) -
		      task_weight(cur, env->dst_nid, dist);
		/*
		 * Add some hysteresis to prevent swapping the
		 * tasks within a group over tiny differences.
		 */
		എങ്കിൽ (cur_ng)
			imp -= imp / 16;
	} അല്ലെങ്കിൽ {
		/*
		 * Compare the group weights. If a task is all by itself
		 * (not part of a group), use the task weight instead.
		 */
		എങ്കിൽ (cur_ng && p_ng)
			imp += group_weight(cur, env->src_nid, dist) -
			       group_weight(cur, env->dst_nid, dist);
		അല്ലെങ്കിൽ
			imp += task_weight(cur, env->src_nid, dist) -
			       task_weight(cur, env->dst_nid, dist);
	}

	/* Discourage picking a task already on its preferred node */
	എങ്കിൽ (cur->numa_preferred_nid == env->dst_nid)
		imp -= imp / 16;

	/*
	 * Encourage picking a task that moves to its preferred node.
	 * This potentially makes imp larger than it's maximum of
	 * 1998 (see SMALLIMP and task_weight for why) but in this
	 * case, it does not matter.
	 */
	എങ്കിൽ (cur->numa_preferred_nid == env->src_nid)
		imp += imp / 8;

	എങ്കിൽ (maymove && moveimp > imp && moveimp > env->best_imp) {
		imp = moveimp;
		cur = ശൂന്യ;
		പോകുക assign;
	}

	/*
	 * Prefer swapping with a task moving to its preferred node over a
	 * task that is not.
	 */
	എങ്കിൽ (env->best_task && cur->numa_preferred_nid == env->src_nid &&
	    env->best_task->numa_preferred_nid != env->src_nid) {
		പോകുക assign;
	}

	/*
	 * If the NUMA importance is less than SMALLIMP,
	 * task migration might only result in ping pong
	 * of tasks and also hurt performance due to cache
	 * misses.
	 */
	എങ്കിൽ (imp < SMALLIMP || imp <= env->best_imp + SMALLIMP / 2)
		പോകുക unlock;

	/*
	 * In the overloaded case, try and keep the load balanced.
	 */
	load = task_h_load(env->p) - task_h_load(cur);
	എങ്കിൽ (!load)
		പോകുക assign;

	dst_load = env->dst_stats.load + load;
	src_load = env->src_stats.load - load;

	എങ്കിൽ (load_too_imbalanced(src_load, dst_load, env))
		പോകുക unlock;

assign:
	/* Evaluate an idle CPU for a task numa move. */
	എങ്കിൽ (!cur) {
		പൂർണ്ണസംഖ്യ cpu = env->dst_stats.idle_cpu;

		/* Nothing cached so current CPU went idle since the search. */
		എങ്കിൽ (cpu < 0)
			cpu = env->dst_cpu;

		/*
		 * If the CPU is no longer truly idle and the previous best CPU
		 * is, keep using it.
		 */
		എങ്കിൽ (!idle_cpu(cpu) && env->best_cpu >= 0 &&
		    idle_cpu(env->best_cpu)) {
			cpu = env->best_cpu;
		}

		env->dst_cpu = cpu;
	}

	task_numa_assign(env, cur, imp);

	/*
	 * If a move to idle is allowed because there is capacity or load
	 * balance improves then stop the search. While a better swap
	 * candidate may exist, a search is not free.
	 */
	എങ്കിൽ (maymove && !cur && env->best_cpu >= 0 && idle_cpu(env->best_cpu))
		stopsearch = true;

	/*
	 * If a swap candidate must be identified and the current best task
	 * moves its preferred node then stop the search.
	 */
	എങ്കിൽ (!maymove && env->best_task &&
	    env->best_task->numa_preferred_nid == env->src_nid) {
		stopsearch = true;
	}
unlock:
	rcu_read_unlock();

	തിരികെ stopsearch;
}

സ്ഥിതം ശൂന്യം task_numa_find_cpu(ഘടന task_numa_env *env,
				long taskimp, long groupimp)
{
	bool maymove = false;
	പൂർണ്ണസംഖ്യ cpu;

	/*
	 * If dst node has spare capacity, then check if there is an
	 * imbalance that would be overruled by the load balancer.
	 */
	എങ്കിൽ (env->dst_stats.കണ്ണി_type == കണ്ണി_has_spare) {
		unsigned പൂർണ്ണസംഖ്യ imbalance;
		പൂർണ്ണസംഖ്യ src_running, dst_running;

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
		എങ്കിൽ (!imbalance) {
			maymove = true;
			എങ്കിൽ (env->dst_stats.idle_cpu >= 0) {
				env->dst_cpu = env->dst_stats.idle_cpu;
				task_numa_assign(env, ശൂന്യ, 0);
				തിരികെ;
			}
		}
	} അല്ലെങ്കിൽ {
		long src_load, dst_load, load;
		/*
		 * If the improvement from just moving env->p direction is better
		 * than swapping tasks around, check if a move is possible.
		 */
		load = task_h_load(env->p);
		dst_load = env->dst_stats.load + load;
		src_load = env->src_stats.load - load;
		maymove = !load_too_imbalanced(src_load, dst_load, env);
	}

	/* Skip CPUs if the source task cannot migrate */
	for_each_cpu_and(cpu, cpumask_of_കണ്ണി(env->dst_nid), env->p->cpus_ptr) {
		env->dst_cpu = cpu;
		എങ്കിൽ (task_numa_compare(env, taskimp, groupimp, maymove))
			നിർത്തുക;
	}
}

സ്ഥിതം പൂർണ്ണസംഖ്യ task_numa_migrate(ഘടന task_struct *p)
{
	ഘടന task_numa_env env = {
		.p = p,

		.src_cpu = task_cpu(p),
		.src_nid = task_കണ്ണി(p),

		.imbalance_pct = 112,

		.best_task = ശൂന്യ,
		.best_imp = 0,
		.best_cpu = -1,
	};
	unsigned long taskweight, groupweight;
	ഘടന sched_domain *sd;
	long taskimp, groupimp;
	ഘടന numa_group *ng;
	ഘടന rq *best_rq;
	പൂർണ്ണസംഖ്യ nid, ret, dist;

	/*
	 * Pick the lowest SD_NUMA domain, as that would have the smallest
	 * imbalance and would be the first to start moving tasks about.
	 *
	 * And we want to avoid any moving of tasks about, as that would create
	 * random movement of tasks -- counter the numa conditions we're trying
	 * to satisfy here.
	 */
	rcu_read_lock();
	sd = rcu_dereference_all(per_cpu(sd_numa, env.src_cpu));
	എങ്കിൽ (sd) {
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
	എങ്കിൽ (unlikely(!sd)) {
		sched_setnuma(p, task_കണ്ണി(p));
		തിരികെ -EINVAL;
	}

	env.dst_nid = p->numa_preferred_nid;
	dist = env.dist = കണ്ണി_distance(env.src_nid, env.dst_nid);
	taskweight = task_weight(p, env.src_nid, dist);
	groupweight = group_weight(p, env.src_nid, dist);
	update_numa_stats(&env, &env.src_stats, env.src_nid, false);
	taskimp = task_weight(p, env.dst_nid, dist) - taskweight;
	groupimp = group_weight(p, env.dst_nid, dist) - groupweight;
	update_numa_stats(&env, &env.dst_stats, env.dst_nid, true);

	/* Try to find a spot on the preferred nid. */
	task_numa_find_cpu(&env, taskimp, groupimp);

	/*
	 * Look at other nodes in these cases:
	 * - there is no space available on the preferred_nid
	 * - the task is part of a numa_group that is interleaved across
	 *   multiple NUMA nodes; in order to better consolidate the group,
	 *   we need to check other locations.
	 */
	ng = deref_curr_numa_group(p);
	എങ്കിൽ (env.best_cpu == -1 || (ng && ng->active_nodes > 1)) {
		for_each_കണ്ണി_state(nid, N_CPU) {
			എങ്കിൽ (nid == env.src_nid || nid == p->numa_preferred_nid)
				തുടരുക;

			dist = കണ്ണി_distance(env.src_nid, env.dst_nid);
			എങ്കിൽ (sched_numa_topology_type == NUMA_BACKPLANE &&
						dist != env.dist) {
				taskweight = task_weight(p, env.src_nid, dist);
				groupweight = group_weight(p, env.src_nid, dist);
			}

			/* Only consider nodes where both task and groups benefit */
			taskimp = task_weight(p, nid, dist) - taskweight;
			groupimp = group_weight(p, nid, dist) - groupweight;
			എങ്കിൽ (taskimp < 0 && groupimp < 0)
				തുടരുക;

			env.dist = dist;
			env.dst_nid = nid;
			update_numa_stats(&env, &env.dst_stats, env.dst_nid, true);
			task_numa_find_cpu(&env, taskimp, groupimp);
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
	എങ്കിൽ (ng) {
		എങ്കിൽ (env.best_cpu == -1)
			nid = env.src_nid;
		അല്ലെങ്കിൽ
			nid = cpu_to_കണ്ണി(env.best_cpu);

		എങ്കിൽ (nid != p->numa_preferred_nid)
			sched_setnuma(p, nid);
	}

	/* No better CPU than the current one was found. */
	എങ്കിൽ (env.best_cpu == -1) {
		trace_sched_stick_numa(p, env.src_cpu, ശൂന്യ, -1);
		തിരികെ -EAGAIN;
	}

	best_rq = cpu_rq(env.best_cpu);
	എങ്കിൽ (env.best_task == ശൂന്യ) {
		ret = migrate_task_to(p, env.best_cpu);
		WRITE_ONCE(best_rq->numa_migrate_on, 0);
		എങ്കിൽ (ret != 0)
			trace_sched_stick_numa(p, env.src_cpu, ശൂന്യ, env.best_cpu);
		തിരികെ ret;
	}

	ret = migrate_swap(p, env.best_task, env.best_cpu, env.src_cpu);
	WRITE_ONCE(best_rq->numa_migrate_on, 0);

	എങ്കിൽ (ret != 0)
		trace_sched_stick_numa(p, env.src_cpu, env.best_task, env.best_cpu);
	put_task_struct(env.best_task);
	തിരികെ ret;
}

/* Attempt to migrate a task to a CPU on the preferred node. */
സ്ഥിതം ശൂന്യം numa_migrate_preferred(ഘടന task_struct *p)
{
	unsigned long interval = HZ;

	/* This task has no NUMA fault statistics yet */
	എങ്കിൽ (unlikely(p->numa_preferred_nid == NUMA_NO_കണ്ണി || !p->numa_faults))
		തിരികെ;

	/* Periodically retry migrating the task to the preferred node */
	interval = min(interval, msecs_to_jiffies(p->numa_scan_period) / 16);
	p->numa_migrate_retry = jiffies + interval;

	/* Success if task is already running on preferred CPU */
	എങ്കിൽ (task_കണ്ണി(p) == p->numa_preferred_nid)
		തിരികെ;

	/* Otherwise, try migrate to a CPU on the preferred node */
	task_numa_migrate(p);
}

/*
 * Find out how many nodes the workload is actively running on. Do this by
 * tracking the nodes from which NUMA hinting faults are triggered. This can
 * be different from the set of nodes where the workload's memory is currently
 * located.
 */
സ്ഥിതം ശൂന്യം numa_group_എണ്ണം_active_nodes(ഘടന numa_group *numa_group)
{
	unsigned long faults, max_faults = 0;
	പൂർണ്ണസംഖ്യ nid, active_nodes = 0;

	for_each_കണ്ണി_state(nid, N_CPU) {
		faults = group_faults_cpu(numa_group, nid);
		എങ്കിൽ (faults > max_faults)
			max_faults = faults;
	}

	for_each_കണ്ണി_state(nid, N_CPU) {
		faults = group_faults_cpu(numa_group, nid);
		എങ്കിൽ (faults * ACTIVE_കണ്ണി_FRACTION > max_faults)
			active_nodes++;
	}

	numa_group->max_faults_cpu = max_faults;
	numa_group->active_nodes = active_nodes;
}

/*
 * When adapting the scan rate, the period is divided into NUMA_PERIOD_SLOTS
 * increments. The more local the fault statistics are, the higher the scan
 * period will be for the next scan window. If local/(local+remote) ratio is
 * below NUMA_PERIOD_THRESHOLD (where range of ratio is 1..NUMA_PERIOD_SLOTS)
 * the scan period will decrease. Aim for 70% local accesses.
 */
#define NUMA_PERIOD_SLOTS 10
#define NUMA_PERIOD_THRESHOLD 7

/*
 * Increase the scan period (slow down scanning) if the majority of
 * our memory is already on our local node, or if the majority of
 * the page accesses are shared with other processes.
 * Otherwise, decrease the scan period.
 */
സ്ഥിതം ശൂന്യം update_task_scan_period(ഘടന task_struct *p,
			unsigned long shared, unsigned long സ്വകാര്യ)
{
	unsigned പൂർണ്ണസംഖ്യ period_slot;
	പൂർണ്ണസംഖ്യ lr_ratio, ps_ratio;
	പൂർണ്ണസംഖ്യ diff;

	unsigned long remote = p->numa_faults_locality[0];
	unsigned long local = p->numa_faults_locality[1];

	/*
	 * If there were no record hinting faults then either the task is
	 * completely idle or all activity is in areas that are not of interest
	 * to automatic numa balancing. Related to that, if there were failed
	 * migration then it implies we are migrating too quickly or the local
	 * node is overloaded. In either case, scan slower
	 */
	എങ്കിൽ (local + shared == 0 || p->numa_faults_locality[2]) {
		p->numa_scan_period = min(p->numa_scan_period_max,
			p->numa_scan_period << 1);

		p->mm->numa_next_scan = jiffies +
			msecs_to_jiffies(p->numa_scan_period);

		പോകുക out;
	}

	/*
	 * Prepare to scale scan period relative to the current period.
	 *	 == NUMA_PERIOD_THRESHOLD scan period stays the same
	 *       <  NUMA_PERIOD_THRESHOLD scan period decreases (scan faster)
	 *	 >= NUMA_PERIOD_THRESHOLD scan period increases (scan slower)
	 */
	period_slot = DIV_ROUND_UP(p->numa_scan_period, NUMA_PERIOD_SLOTS);
	lr_ratio = (local * NUMA_PERIOD_SLOTS) / (local + remote);
	ps_ratio = (സ്വകാര്യ * NUMA_PERIOD_SLOTS) / (സ്വകാര്യ + shared);

	എങ്കിൽ (ps_ratio >= NUMA_PERIOD_THRESHOLD) {
		/*
		 * Most memory accesses are local. There is no need to
		 * do fast NUMA scanning, since memory is already local.
		 */
		പൂർണ്ണസംഖ്യ slot = ps_ratio - NUMA_PERIOD_THRESHOLD;
		എങ്കിൽ (!slot)
			slot = 1;
		diff = slot * period_slot;
	} അല്ലെങ്കിൽ എങ്കിൽ (lr_ratio >= NUMA_PERIOD_THRESHOLD) {
		/*
		 * Most memory accesses are shared with other tasks.
		 * There is no point in continuing fast NUMA scanning,
		 * since other tasks may just move the memory elsewhere.
		 */
		പൂർണ്ണസംഖ്യ slot = lr_ratio - NUMA_PERIOD_THRESHOLD;
		എങ്കിൽ (!slot)
			slot = 1;
		diff = slot * period_slot;
	} അല്ലെങ്കിൽ {
		/*
		 * Private memory faults exceed (SLOTS-THRESHOLD)/SLOTS,
		 * yet they are not on the local NUMA node. Speed up
		 * NUMA scanning to get the memory moved over.
		 */
		പൂർണ്ണസംഖ്യ ratio = max(lr_ratio, ps_ratio);
		diff = -(NUMA_PERIOD_THRESHOLD - ratio) * period_slot;
	}

	p->numa_scan_period = clamp(p->numa_scan_period + diff,
			task_scan_min(p), task_scan_max(p));

out:
	memset(p->numa_faults_locality, 0,
	       വലുപ്പം(p->numa_faults_locality));
}

/*
 * Get the fraction of time the task has been running since the last
 * NUMA placement cycle. The scheduler keeps similar statistics, but
 * decays those on a 32ms period, which is orders of magnitude off
 * from the dozens-of-seconds NUMA balancing period. Use the scheduler
 * stats only if the task is so new there are no NUMA statistics yet.
 */
സ്ഥിതം u64 numa_get_avg_runtime(ഘടന task_struct *p, u64 *period)
{
	u64 runtime, delta, now;
	/* Use the start of this time slice to avoid calculations. */
	now = p->se.exec_start;
	runtime = p->se.sum_exec_runtime;

	എങ്കിൽ (p->last_task_numa_placement) {
		delta = runtime - p->last_sum_exec_runtime;
		*period = now - p->last_task_numa_placement;

		/* Avoid time going backwards, prevent potential divide error: */
		എങ്കിൽ (unlikely((s64)*period < 0))
			*period = 0;
	} അല്ലെങ്കിൽ {
		delta = p->se.avg.load_sum;
		*period = LOAD_AVG_MAX;
	}

	p->last_sum_exec_runtime = runtime;
	p->last_task_numa_placement = now;

	തിരികെ delta;
}

/*
 * Determine the preferred nid for a task in a numa_group. This needs to
 * be done in a way that produces consistent results with group_weight,
 * otherwise workloads might not converge.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ preferred_group_nid(ഘടന task_struct *p, പൂർണ്ണസംഖ്യ nid)
{
	nodemask_t nodes;
	പൂർണ്ണസംഖ്യ dist;

	/* Direct connections between all NUMA nodes. */
	എങ്കിൽ (sched_numa_topology_type == NUMA_DIRECT)
		തിരികെ nid;

	/*
	 * On a system with glueless mesh NUMA topology, group_weight
	 * scores nodes according to the number of NUMA hinting faults on
	 * both the node itself, and on nearby nodes.
	 */
	എങ്കിൽ (sched_numa_topology_type == NUMA_GLUELESS_MESH) {
		unsigned long score, max_score = 0;
		പൂർണ്ണസംഖ്യ കണ്ണി, max_കണ്ണി = nid;

		dist = sched_max_numa_distance;

		for_each_കണ്ണി_state(കണ്ണി, N_CPU) {
			score = group_weight(p, കണ്ണി, dist);
			എങ്കിൽ (score > max_score) {
				max_score = score;
				max_കണ്ണി = കണ്ണി;
			}
		}
		തിരികെ max_കണ്ണി;
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
	nodes = കണ്ണി_states[N_CPU];
	വേണ്ടി (dist = sched_max_numa_distance; dist > LOCAL_DISTANCE; dist--) {
		unsigned long max_faults = 0;
		nodemask_t max_group = കണ്ണി_MASK_NONE;
		പൂർണ്ണസംഖ്യ a, b;

		/* Are there nodes at this distance from each other? */
		എങ്കിൽ (!find_numa_distance(dist))
			തുടരുക;

		for_each_കണ്ണി_mask(a, nodes) {
			unsigned long faults = 0;
			nodemask_t this_group;
			nodes_clear(this_group);

			/* Sum group's NUMA faults; includes a==b case. */
			for_each_കണ്ണി_mask(b, nodes) {
				എങ്കിൽ (കണ്ണി_distance(a, b) < dist) {
					faults += group_faults(p, b);
					കണ്ണി_set(b, this_group);
					കണ്ണി_clear(b, nodes);
				}
			}

			/* Remember the top group. */
			എങ്കിൽ (faults > max_faults) {
				max_faults = faults;
				max_group = this_group;
				/*
				 * subtle: at the smallest distance there is
				 * just one node left in each "group", the
				 * winner is the preferred nid.
				 */
				nid = a;
			}
		}
		/* Next round, evaluate the nodes within max_group. */
		എങ്കിൽ (!max_faults)
			നിർത്തുക;
		nodes = max_group;
	}
	തിരികെ nid;
}

സ്ഥിതം ശൂന്യം task_numa_placement(ഘടന task_struct *p)
	__context_unsafe(/* conditional locking */)
{
	ഘടന sched_cache_group __maybe_unused *grp;
	പൂർണ്ണസംഖ്യ seq, nid, max_nid = NUMA_NO_കണ്ണി;
	unsigned long max_faults = 0;
	unsigned long fault_types[2] = { 0, 0 };
	unsigned long total_faults;
	u64 runtime, period;
	spinlock_t *group_lock = ശൂന്യ;
	long __maybe_unused new_fp;
	ഘടന numa_group *ng;

	/*
	 * The p->mm->numa_scan_seq field gets updated without
	 * exclusive access. Use READ_ONCE() here to ensure
	 * that the field is read in a single access:
	 */
	seq = READ_ONCE(p->mm->numa_scan_seq);
	എങ്കിൽ (p->numa_scan_seq == seq)
		തിരികെ;
	p->numa_scan_seq = seq;
	p->numa_scan_period_max = task_scan_max(p);

	total_faults = p->numa_faults_locality[0] +
		       p->numa_faults_locality[1];
	runtime = numa_get_avg_runtime(p, &period);

	/* If the task is part of a group prevent parallel updates to group stats */
	ng = deref_curr_numa_group(p);
	എങ്കിൽ (ng) {
		group_lock = &ng->lock;
		spin_lock_irq(group_lock);
	}

	/* Find the node with the highest number of faults */
	for_each_online_കണ്ണി(nid) {
		/* Keep track of the offsets in numa_faults array */
		പൂർണ്ണസംഖ്യ mem_idx, membuf_idx, cpu_idx, cpubuf_idx;
		unsigned long faults = 0, group_faults = 0;
		പൂർണ്ണസംഖ്യ priv;

		വേണ്ടി (priv = 0; priv < NR_NUMA_HINT_FAULT_TYPES; priv++) {
			long diff, f_diff, f_weight;

			mem_idx = task_faults_idx(NUMA_MEM, nid, priv);
			membuf_idx = task_faults_idx(NUMA_MEMBUF, nid, priv);
			cpu_idx = task_faults_idx(NUMA_CPU, nid, priv);
			cpubuf_idx = task_faults_idx(NUMA_CPUBUF, nid, priv);

			/* Decay existing window, copy faults since last scan */
			diff = p->numa_faults[membuf_idx] - p->numa_faults[mem_idx] / 2;
			fault_types[priv] += p->numa_faults[membuf_idx];
			p->numa_faults[membuf_idx] = 0;

			/*
			 * Normalize the faults_from, so all tasks in a group
			 * count according to CPU use, instead of by the raw
			 * number of faults. Tasks with little runtime have
			 * little over-all impact on throughput, and thus their
			 * faults are less important.
			 */
			f_weight = div64_u64(runtime << 16, period + 1);
			f_weight = (f_weight * p->numa_faults[cpubuf_idx]) /
				   (total_faults + 1);
			f_diff = f_weight - p->numa_faults[cpu_idx] / 2;
			p->numa_faults[cpubuf_idx] = 0;

			p->numa_faults[mem_idx] += diff;
			p->numa_faults[cpu_idx] += f_diff;
			faults += p->numa_faults[mem_idx];
			p->total_numa_faults += diff;
			എങ്കിൽ (ng) {
				/*
				 * safe because we can only change our own group
				 *
				 * mem_idx represents the offset for a given
				 * nid and priv in a specific region because it
				 * is at the beginning of the numa_faults array.
				 */
				ng->faults[mem_idx] += diff;
				ng->faults[cpu_idx] += f_diff;
				ng->total_faults += diff;
				group_faults += ng->faults[mem_idx];
			}
#ifdef CONFIG_SCHED_CACHE
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

				എങ്കിൽ (grp) {
					new_fp = (long)READ_ONCE(grp->footprint) + diff;
					WRITE_ONCE(grp->footprint, max(new_fp, 0L));
				}
			}
#endif
		}

		എങ്കിൽ (!ng) {
			എങ്കിൽ (faults > max_faults) {
				max_faults = faults;
				max_nid = nid;
			}
		} അല്ലെങ്കിൽ എങ്കിൽ (group_faults > max_faults) {
			max_faults = group_faults;
			max_nid = nid;
		}
	}

	/* Cannot migrate task to CPU-less node */
	max_nid = numa_nearest_കണ്ണി(max_nid, N_CPU);

	എങ്കിൽ (ng) {
		numa_group_എണ്ണം_active_nodes(ng);
		spin_unlock_irq(group_lock);
		max_nid = preferred_group_nid(p, max_nid);
	}

	എങ്കിൽ (max_faults) {
		/* Set the new preferred node */
		എങ്കിൽ (max_nid != p->numa_preferred_nid)
			sched_setnuma(p, max_nid);
	}

	update_task_scan_period(p, fault_types[0], fault_types[1]);
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ get_numa_group(ഘടന numa_group *grp)
{
	തിരികെ refcount_inc_not_zero(&grp->refcount);
}

സ്ഥിതം വരിയിൽ ശൂന്യം put_numa_group(ഘടന numa_group *grp)
{
	എങ്കിൽ (refcount_dec_and_test(&grp->refcount))
		kfree_rcu(grp, rcu);
}

സ്ഥിതം ശൂന്യം task_numa_group(ഘടന task_struct *p, പൂർണ്ണസംഖ്യ cpupid, പൂർണ്ണസംഖ്യ flags,
			പൂർണ്ണസംഖ്യ *priv)
{
	ഘടന numa_group *grp, *my_grp;
	ഘടന task_struct *tsk;
	bool join = false;
	പൂർണ്ണസംഖ്യ cpu = cpupid_to_cpu(cpupid);
	പൂർണ്ണസംഖ്യ i;

	എങ്കിൽ (unlikely(!deref_curr_numa_group(p))) {
		unsigned പൂർണ്ണസംഖ്യ size = വലുപ്പം(ഘടന numa_group) +
				    NR_NUMA_HINT_FAULT_STATS *
				    nr_കണ്ണി_ids * വലുപ്പം(unsigned long);

		grp = kzalloc(size, GFP_KERNEL | __GFP_NOWARN);
		എങ്കിൽ (!grp)
			തിരികെ;

		refcount_set(&grp->refcount, 1);
		grp->active_nodes = 1;
		grp->max_faults_cpu = 0;
		spin_lock_init(&grp->lock);
		grp->gid = p->pid;

		വേണ്ടി (i = 0; i < NR_NUMA_HINT_FAULT_STATS * nr_കണ്ണി_ids; i++)
			grp->faults[i] = p->numa_faults[i];

		grp->total_faults = p->total_numa_faults;

		grp->nr_tasks++;
		rcu_assign_pointer(p->numa_group, grp);
	}

	rcu_read_lock();
	tsk = READ_ONCE(cpu_rq(cpu)->curr);

	എങ്കിൽ (!cpupid_match_pid(tsk, cpupid))
		പോകുക no_join;

	grp = rcu_dereference_all(tsk->numa_group);
	എങ്കിൽ (!grp)
		പോകുക no_join;

	my_grp = deref_curr_numa_group(p);
	എങ്കിൽ (grp == my_grp)
		പോകുക no_join;

	/*
	 * Only join the other group if its bigger; if we're the bigger group,
	 * the other task will join us.
	 */
	എങ്കിൽ (my_grp->nr_tasks > grp->nr_tasks)
		പോകുക no_join;

	/*
	 * Tie-break on the grp address.
	 */
	എങ്കിൽ (my_grp->nr_tasks == grp->nr_tasks && my_grp > grp)
		പോകുക no_join;

	/* Always join threads in the same process. */
	എങ്കിൽ (tsk->mm == current->mm)
		join = true;

	/* Simple filter to avoid false positives due to PID collisions */
	എങ്കിൽ (flags & TNF_SHARED)
		join = true;

	/* Update priv based on whether false sharing was detected */
	*priv = !join;

	എങ്കിൽ (join && !get_numa_group(grp))
		പോകുക no_join;

	rcu_read_unlock();

	എങ്കിൽ (!join)
		തിരികെ;

	WARN_ON_ONCE(irqs_disabled());
	double_lock_irq(&my_grp->lock, &grp->lock);

	വേണ്ടി (i = 0; i < NR_NUMA_HINT_FAULT_STATS * nr_കണ്ണി_ids; i++) {
		my_grp->faults[i] -= p->numa_faults[i];
		grp->faults[i] += p->numa_faults[i];
	}
	my_grp->total_faults -= p->total_numa_faults;
	grp->total_faults += p->total_numa_faults;

	my_grp->nr_tasks--;
	grp->nr_tasks++;

	spin_unlock(&my_grp->lock);
	spin_unlock_irq(&grp->lock);

	rcu_assign_pointer(p->numa_group, grp);

	put_numa_group(my_grp);
	തിരികെ;

no_join:
	rcu_read_unlock();
	തിരികെ;
}

/*
 * Get rid of NUMA statistics associated with a task (either current or dead).
 * If @final is set, the task is dead and has reached refcount zero, so we can
 * safely free all relevant data structures. Otherwise, there might be
 * concurrent reads from places like load balancing and procfs, and we should
 * reset the data back to default state without freeing ->numa_faults.
 */
ശൂന്യം task_numa_free(ഘടന task_struct *p, bool final)
{
	/* safe: p either is current or is being freed by current */
	ഘടന numa_group *grp = rcu_dereference_raw(p->numa_group);
	unsigned long *numa_faults = p->numa_faults;
	unsigned long flags;
	പൂർണ്ണസംഖ്യ i;

	എങ്കിൽ (!numa_faults)
		തിരികെ;

	എങ്കിൽ (grp) {
		spin_lock_irqsave(&grp->lock, flags);
		വേണ്ടി (i = 0; i < NR_NUMA_HINT_FAULT_STATS * nr_കണ്ണി_ids; i++)
			grp->faults[i] -= p->numa_faults[i];
		grp->total_faults -= p->total_numa_faults;

		grp->nr_tasks--;
		spin_unlock_irqrestore(&grp->lock, flags);
		RCU_INIT_POINTER(p->numa_group, ശൂന്യ);
		put_numa_group(grp);
	}

	എങ്കിൽ (final) {
		p->numa_faults = ശൂന്യ;
		kfree(numa_faults);
	} അല്ലെങ്കിൽ {
		p->total_numa_faults = 0;
		വേണ്ടി (i = 0; i < NR_NUMA_HINT_FAULT_STATS * nr_കണ്ണി_ids; i++)
			numa_faults[i] = 0;
	}
}

/*
 * Got a PROT_NONE fault for a page on @node.
 */
ശൂന്യം task_numa_fault(പൂർണ്ണസംഖ്യ last_cpupid, പൂർണ്ണസംഖ്യ mem_കണ്ണി, പൂർണ്ണസംഖ്യ pages, പൂർണ്ണസംഖ്യ flags)
{
	ഘടന task_struct *p = current;
	bool migrated = flags & TNF_MIGRATED;
	പൂർണ്ണസംഖ്യ cpu_കണ്ണി = task_കണ്ണി(current);
	പൂർണ്ണസംഖ്യ local = !!(flags & TNF_FAULT_LOCAL);
	ഘടന numa_group *ng;
	പൂർണ്ണസംഖ്യ priv;

	എങ്കിൽ (!static_branch_likely(&sched_numa_balancing))
		തിരികെ;

	/* for example, ksmd faulting in a user's mm */
	എങ്കിൽ (!p->mm)
		തിരികെ;

	/*
	 * NUMA faults statistics are unnecessary for the slow memory
	 * node for memory tiering mode.
	 */
	എങ്കിൽ (!കണ്ണി_is_toptier(mem_കണ്ണി) &&
	    (sysctl_numa_balancing_mode & NUMA_BALANCING_MEMORY_TIERING ||
	     !cpupid_valid(last_cpupid)))
		തിരികെ;

	/* Allocate buffer to track faults on a per-node basis */
	എങ്കിൽ (unlikely(!p->numa_faults)) {
		പൂർണ്ണസംഖ്യ size = വലുപ്പം(*p->numa_faults) *
			   NR_NUMA_HINT_FAULT_BUCKETS * nr_കണ്ണി_ids;

		p->numa_faults = kzalloc(size, GFP_KERNEL|__GFP_NOWARN);
		എങ്കിൽ (!p->numa_faults)
			തിരികെ;

		p->total_numa_faults = 0;
		memset(p->numa_faults_locality, 0, വലുപ്പം(p->numa_faults_locality));
	}

	/*
	 * First accesses are treated as private, otherwise consider accesses
	 * to be private if the accessing pid has not changed
	 */
	എങ്കിൽ (unlikely(last_cpupid == (-1 & LAST_CPUPID_MASK))) {
		priv = 1;
	} അല്ലെങ്കിൽ {
		priv = cpupid_match_pid(p, last_cpupid);
		എങ്കിൽ (!priv && !(flags & TNF_NO_GROUP))
			task_numa_group(p, last_cpupid, flags, &priv);
	}

	/*
	 * If a workload spans multiple NUMA nodes, a shared fault that
	 * occurs wholly within the set of nodes that the workload is
	 * actively using should be counted as local. This allows the
	 * scan rate to slow down when a workload has settled down.
	 */
	ng = deref_curr_numa_group(p);
	എങ്കിൽ (!priv && !local && ng && ng->active_nodes > 1 &&
				numa_is_active_കണ്ണി(cpu_കണ്ണി, ng) &&
				numa_is_active_കണ്ണി(mem_കണ്ണി, ng))
		local = 1;

	/*
	 * Retry to migrate task to preferred node periodically, in case it
	 * previously failed, or the scheduler moved us.
	 */
	എങ്കിൽ (time_after(jiffies, p->numa_migrate_retry)) {
		task_numa_placement(p);
		numa_migrate_preferred(p);
	}

	എങ്കിൽ (migrated)
		p->numa_pages_migrated += pages;
	എങ്കിൽ (flags & TNF_MIGRATE_FAIL)
		p->numa_faults_locality[2] += pages;

	p->numa_faults[task_faults_idx(NUMA_MEMBUF, mem_കണ്ണി, priv)] += pages;
	p->numa_faults[task_faults_idx(NUMA_CPUBUF, cpu_കണ്ണി, priv)] += pages;
	p->numa_faults_locality[local] += pages;
}

സ്ഥിതം ശൂന്യം reset_ptenuma_scan(ഘടന task_struct *p)
{
	/*
	 * We only did a read acquisition of the mmap sem, so
	 * p->mm->numa_scan_seq is written to without exclusive access
	 * and the update is not guaranteed to be atomic. That's not
	 * much of an issue though, since this is just used for
	 * statistical sampling. Use READ_ONCE/WRITE_ONCE, which are not
	 * expensive, to avoid any form of compiler optimizations:
	 */
	WRITE_ONCE(p->mm->numa_scan_seq, READ_ONCE(p->mm->numa_scan_seq) + 1);
	p->mm->numa_scan_offset = 0;
}

സ്ഥിതം bool vma_is_accessed(ഘടന mm_struct *mm, ഘടന vm_area_struct *vma)
{
	unsigned long pids;
	/*
	 * Allow unconditional access first two times, so that all the (pages)
	 * of VMAs get prot_none fault introduced irrespective of accesses.
	 * This is also done to avoid any side effect of task scanning
	 * amplifying the unfairness of disjoint set of VMAs' access.
	 */
	എങ്കിൽ ((READ_ONCE(current->mm->numa_scan_seq) - vma->numab_state->start_scan_seq) < 2)
		തിരികെ true;

	pids = vma->numab_state->pids_active[0] | vma->numab_state->pids_active[1];
	എങ്കിൽ (test_bit(hash_32(current->pid, ilog2(BITS_PER_LONG)), &pids))
		തിരികെ true;

	/*
	 * Complete a scan that has already started regardless of PID access, or
	 * some VMAs may never be scanned in multi-threaded applications:
	 */
	എങ്കിൽ (mm->numa_scan_offset > vma->vm_start) {
		trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_IGNORE_PID);
		തിരികെ true;
	}

	/*
	 * This vma has not been accessed for a while, and if the number
	 * the threads in the same process is low, which means no other
	 * threads can help scan this vma, force a vma scan.
	 */
	എങ്കിൽ (READ_ONCE(mm->numa_scan_seq) >
	   (vma->numab_state->prev_scan_seq + get_nr_threads(current)))
		തിരികെ true;

	തിരികെ false;
}

#define VMA_PID_RESET_PERIOD (4 * sysctl_numa_balancing_scan_delay)

/*
 * The expensive part of numa migration is done from task_work context.
 * Triggered from task_tick_numa().
 */
സ്ഥിതം ശൂന്യം task_numa_work(ഘടന callback_head *work)
{
	unsigned long migrate, next_scan, now = jiffies;
	ഘടന task_struct *p = current;
	ഘടന mm_struct *mm = p->mm;
	u64 runtime = p->se.sum_exec_runtime;
	ഘടന vm_area_struct *vma;
	unsigned long start, end;
	unsigned long nr_pte_updates = 0;
	long pages, virtpages;
	ഘടന vma_iterator vmi;
	bool vma_pids_skipped;
	bool vma_pids_forced = false;

	WARN_ON_ONCE(p != container_of(work, ഘടന task_struct, numa_work));

	work->next = work;
	/*
	 * Who cares about NUMA placement when they're dying.
	 *
	 * NOTE: make sure not to dereference p->mm before this check,
	 * exit_task_work() happens _after_ exit_mm() so we could be called
	 * without p->mm even though we still had it when we enqueued this
	 * work.
	 */
	എങ്കിൽ (p->flags & PF_EXITING)
		തിരികെ;

	/*
	 * Memory is pinned to only one NUMA node via cpuset.mems, naturally
	 * no page can be migrated.
	 */
	എങ്കിൽ (cpusets_enabled() && nodes_weight(cpuset_current_mems_allowed) == 1) {
		trace_sched_skip_cpuset_numa(current, &cpuset_current_mems_allowed);
		തിരികെ;
	}

	എങ്കിൽ (!mm->numa_next_scan) {
		mm->numa_next_scan = now +
			msecs_to_jiffies(sysctl_numa_balancing_scan_delay);
	}

	/*
	 * Enforce maximal scan/migration frequency..
	 */
	migrate = mm->numa_next_scan;
	എങ്കിൽ (time_before(now, migrate))
		തിരികെ;

	എങ്കിൽ (p->numa_scan_period == 0) {
		p->numa_scan_period_max = task_scan_max(p);
		p->numa_scan_period = task_scan_start(p);
	}

	next_scan = now + msecs_to_jiffies(p->numa_scan_period);
	എങ്കിൽ (!try_cmpxchg(&mm->numa_next_scan, &migrate, next_scan))
		തിരികെ;

	/*
	 * Delay this task enough that another task of this mm will likely win
	 * the next time around.
	 */
	p->കണ്ണി_stamp += 2 * TICK_NSEC;

	pages = sysctl_numa_balancing_scan_size;
	pages <<= 20 - PAGE_SHIFT; /* MB in pages */
	virtpages = pages * 8;	   /* Scan up to this much virtual space */
	എങ്കിൽ (!pages)
		തിരികെ;


	എങ്കിൽ (!mmap_read_trylock(mm))
		തിരികെ;

	/*
	 * VMAs are skipped if the current PID has not trapped a fault within
	 * the VMA recently. Allow scanning to be forced if there is no
	 * suitable VMA remaining.
	 */
	vma_pids_skipped = false;

retry_pids:
	start = mm->numa_scan_offset;
	vma_iter_init(&vmi, mm, start);
	vma = vma_next(&vmi);
	എങ്കിൽ (!vma) {
		reset_ptenuma_scan(p);
		start = 0;
		vma_iter_set(&vmi, start);
		vma = vma_next(&vmi);
	}

	വേണ്ടി (; vma; vma = vma_next(&vmi)) {
		എങ്കിൽ (!vma_migratable(vma) || !vma_policy_mof(vma) ||
			is_vm_hugetlb_page(vma) || (vma->vm_flags & VM_MIXEDMAP)) {
			trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_UNSUITABLE);
			തുടരുക;
		}

		/*
		 * Shared library pages mapped by multiple processes are not
		 * migrated as it is expected they are cache replicated. Avoid
		 * hinting faults in read-only file-backed mappings or the vDSO
		 * as migrating the pages will be of marginal benefit.
		 */
		എങ്കിൽ (!vma->vm_mm ||
		    (vma->vm_file && (vma->vm_flags & (VM_READ|VM_WRITE)) == (VM_READ))) {
			trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_SHARED_RO);
			തുടരുക;
		}

		/*
		 * Skip inaccessible VMAs to avoid any confusion between
		 * PROT_NONE and NUMA hinting PTEs
		 */
		എങ്കിൽ (!vma_is_accessible(vma)) {
			trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_INACCESSIBLE);
			തുടരുക;
		}

		/* Initialise new per-VMA NUMAB state. */
		എങ്കിൽ (!vma->numab_state) {
			ഘടന vma_numab_state *ptr;

			ptr = kzalloc_obj(*ptr);
			എങ്കിൽ (!ptr)
				തുടരുക;

			എങ്കിൽ (cmpxchg(&vma->numab_state, ശൂന്യ, ptr)) {
				kfree(ptr);
				തുടരുക;
			}

			vma->numab_state->start_scan_seq = mm->numa_scan_seq;

			vma->numab_state->next_scan = now +
				msecs_to_jiffies(sysctl_numa_balancing_scan_delay);

			/* Reset happens after 4 times scan delay of scan start */
			vma->numab_state->pids_active_reset =  vma->numab_state->next_scan +
				msecs_to_jiffies(VMA_PID_RESET_PERIOD);

			/*
			 * Ensure prev_scan_seq does not match numa_scan_seq,
			 * to prevent VMAs being skipped prematurely on the
			 * first scan:
			 */
			 vma->numab_state->prev_scan_seq = mm->numa_scan_seq - 1;
		}

		/*
		 * Scanning the VMAs of short lived tasks add more overhead. So
		 * delay the scan for new VMAs.
		 */
		എങ്കിൽ (mm->numa_scan_seq && time_before(jiffies,
						vma->numab_state->next_scan)) {
			trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_SCAN_DELAY);
			തുടരുക;
		}

		/* RESET access PIDs regularly for old VMAs. */
		എങ്കിൽ (mm->numa_scan_seq &&
				time_after(jiffies, vma->numab_state->pids_active_reset)) {
			vma->numab_state->pids_active_reset = vma->numab_state->pids_active_reset +
				msecs_to_jiffies(VMA_PID_RESET_PERIOD);
			vma->numab_state->pids_active[0] = READ_ONCE(vma->numab_state->pids_active[1]);
			vma->numab_state->pids_active[1] = 0;
		}

		/* Do not rescan VMAs twice within the same sequence. */
		എങ്കിൽ (vma->numab_state->prev_scan_seq == mm->numa_scan_seq) {
			mm->numa_scan_offset = vma->vm_end;
			trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_SEQ_COMPLETED);
			തുടരുക;
		}

		/*
		 * Do not scan the VMA if task has not accessed it, unless no other
		 * VMA candidate exists.
		 */
		എങ്കിൽ (!vma_pids_forced && !vma_is_accessed(mm, vma)) {
			vma_pids_skipped = true;
			trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_PID_INACTIVE);
			തുടരുക;
		}

		ചെയ്യുക {
			start = max(start, vma->vm_start);
			end = ALIGN(start + (pages << PAGE_SHIFT), HPAGE_SIZE);
			end = min(end, vma->vm_end);
			nr_pte_updates = change_prot_numa(vma, start, end);

			/*
			 * Try to scan sysctl_numa_balancing_size worth of
			 * hpages that have at least one present PTE that
			 * is not already PTE-numa. If the VMA contains
			 * areas that are unused or already full of prot_numa
			 * PTEs, scan up to virtpages, to skip through those
			 * areas faster.
			 */
			എങ്കിൽ (nr_pte_updates)
				pages -= (end - start) >> PAGE_SHIFT;
			virtpages -= (end - start) >> PAGE_SHIFT;

			start = end;
			എങ്കിൽ (pages <= 0 || virtpages <= 0)
				പോകുക out;

			cond_resched();
		} ആകുമ്പോൾ (end != vma->vm_end);

		/* VMA scan is complete, do not scan until next sequence. */
		vma->numab_state->prev_scan_seq = mm->numa_scan_seq;

		/*
		 * Only force scan within one VMA at a time, to limit the
		 * cost of scanning a potentially uninteresting VMA.
		 */
		എങ്കിൽ (vma_pids_forced)
			നിർത്തുക;
	}

	/*
	 * If no VMAs are remaining and VMAs were skipped due to the PID
	 * not accessing the VMA previously, then force a scan to ensure
	 * forward progress:
	 */
	എങ്കിൽ (!vma && !vma_pids_forced && vma_pids_skipped) {
		vma_pids_forced = true;
		പോകുക retry_pids;
	}

out:
	/*
	 * It is possible to reach the end of the VMA list but the last few
	 * VMAs are not guaranteed to the vma_migratable. If they are not, we
	 * would find the !migratable VMA on the next scan but not reset the
	 * scanner to the start so check it now.
	 */
	എങ്കിൽ (vma)
		mm->numa_scan_offset = start;
	അല്ലെങ്കിൽ
		reset_ptenuma_scan(p);
	mmap_read_unlock(mm);

	/*
	 * Make sure tasks use at least 32x as much time to run other code
	 * than they used here, to limit NUMA PTE scanning overhead to 3% max.
	 * Usually update_task_scan_period slows down scanning enough; on an
	 * overloaded system we need to limit overhead on a per task basis.
	 */
	എങ്കിൽ (unlikely(p->se.sum_exec_runtime != runtime)) {
		u64 diff = p->se.sum_exec_runtime - runtime;
		p->കണ്ണി_stamp += 32 * diff;
	}
}

ശൂന്യം init_numa_balancing(u64 clone_flags, ഘടന task_struct *p)
{
	പൂർണ്ണസംഖ്യ mm_users = 0;
	ഘടന mm_struct *mm = p->mm;

	എങ്കിൽ (mm) {
		mm_users = atomic_read(&mm->mm_users);
		എങ്കിൽ (mm_users == 1) {
			mm->numa_next_scan = jiffies + msecs_to_jiffies(sysctl_numa_balancing_scan_delay);
			mm->numa_scan_seq = 0;
		}
	}
	p->കണ്ണി_stamp			= 0;
	p->numa_scan_seq		= mm ? mm->numa_scan_seq : 0;
	p->numa_scan_period		= sysctl_numa_balancing_scan_delay;
	p->numa_migrate_retry		= 0;
	/* Protect against double add, see task_tick_numa and task_numa_work */
	p->numa_work.next		= &p->numa_work;
	p->numa_faults			= ശൂന്യ;
	p->numa_pages_migrated		= 0;
	p->total_numa_faults		= 0;
	RCU_INIT_POINTER(p->numa_group, ശൂന്യ);
	p->last_task_numa_placement	= 0;
	p->last_sum_exec_runtime	= 0;

	init_task_work(&p->numa_work, task_numa_work);

	/* New address space, reset the preferred nid */
	എങ്കിൽ (!(clone_flags & CLONE_VM)) {
		p->numa_preferred_nid = NUMA_NO_കണ്ണി;
		തിരികെ;
	}

	/*
	 * New thread, keep existing numa_preferred_nid which should be copied
	 * already by arch_dup_task_struct but stagger when scans start.
	 */
	എങ്കിൽ (mm) {
		unsigned പൂർണ്ണസംഖ്യ delay;

		delay = min_t(unsigned പൂർണ്ണസംഖ്യ, task_scan_max(current),
			current->numa_scan_period * mm_users * NSEC_PER_MSEC);
		delay += 2 * TICK_NSEC;
		p->കണ്ണി_stamp = delay;
	}
}

/*
 * Drive the periodic memory faults..
 */
സ്ഥിതം ശൂന്യം task_tick_numa(ഘടന rq *rq, ഘടന task_struct *curr)
{
	ഘടന callback_head *work = &curr->numa_work;
	u64 period, now;

	/*
	 * We don't care about NUMA placement if we don't have memory.
	 */
	എങ്കിൽ (!curr->mm || (curr->flags & (PF_EXITING | PF_KTHREAD)) || work->next != work)
		തിരികെ;

	/*
	 * Using runtime rather than walltime has the dual advantage that
	 * we (mostly) drive the selection from busy threads and that the
	 * task needs to have done some actual work before we bother with
	 * NUMA placement.
	 */
	now = curr->se.sum_exec_runtime;
	period = (u64)curr->numa_scan_period * NSEC_PER_MSEC;

	എങ്കിൽ (now > curr->കണ്ണി_stamp + period) {
		എങ്കിൽ (!curr->കണ്ണി_stamp)
			curr->numa_scan_period = task_scan_start(curr);
		curr->കണ്ണി_stamp += period;

		എങ്കിൽ (!time_before(jiffies, curr->mm->numa_next_scan))
			task_work_add(curr, work, TWA_RESUME);
	}
}

സ്ഥിതം ശൂന്യം update_scan_period(ഘടന task_struct *p, പൂർണ്ണസംഖ്യ new_cpu)
{
	പൂർണ്ണസംഖ്യ src_nid = cpu_to_കണ്ണി(task_cpu(p));
	പൂർണ്ണസംഖ്യ dst_nid = cpu_to_കണ്ണി(new_cpu);

	എങ്കിൽ (!static_branch_likely(&sched_numa_balancing))
		തിരികെ;

	എങ്കിൽ (!p->mm || !p->numa_faults || (p->flags & PF_EXITING))
		തിരികെ;

	എങ്കിൽ (src_nid == dst_nid)
		തിരികെ;

	/*
	 * Allow resets if faults have been trapped before one scan
	 * has completed. This is most likely due to a new task that
	 * is pulled cross-node due to wakeups or load balancing.
	 */
	എങ്കിൽ (p->numa_scan_seq) {
		/*
		 * Avoid scan adjustments if moving to the preferred
		 * node or if the task was not previously running on
		 * the preferred node.
		 */
		എങ്കിൽ (dst_nid == p->numa_preferred_nid ||
		    (p->numa_preferred_nid != NUMA_NO_കണ്ണി &&
			src_nid != p->numa_preferred_nid))
			തിരികെ;
	}

	p->numa_scan_period = task_scan_start(p);
}

#അല്ലെങ്കിൽ /* !CONFIG_NUMA_BALANCING: */

സ്ഥിതം ശൂന്യം task_tick_numa(ഘടന rq *rq, ഘടന task_struct *curr)
{
}

സ്ഥിതം വരിയിൽ ശൂന്യം account_numa_enqueue(ഘടന rq *rq, ഘടന task_struct *p)
{
}

സ്ഥിതം വരിയിൽ ശൂന്യം account_numa_dequeue(ഘടന rq *rq, ഘടന task_struct *p)
{
}

സ്ഥിതം വരിയിൽ ശൂന്യം update_scan_period(ഘടന task_struct *p, പൂർണ്ണസംഖ്യ new_cpu)
{
}

#endif /* !CONFIG_NUMA_BALANCING */

സ്ഥിതം ശൂന്യം
account_entity_enqueue(ഘടന cfs_rq *cfs_rq, ഘടന sched_entity *se)
{
	WARN_ON_ONCE(cfs_rq != cfs_rq_of(se));
	update_load_add(&cfs_rq->load, se->load.weight);
	എങ്കിൽ (entity_is_task(se)) {
		ഘടന task_struct *p = task_of(se);
		ഘടന rq *rq = rq_of(cfs_rq);

		account_numa_enqueue(rq, p);
		account_llc_enqueue(rq, p);
		list_add(&se->group_കണ്ണി, &rq->cfs_tasks);
	}
	cfs_rq->nr_queued++;
}

സ്ഥിതം ശൂന്യം
account_entity_dequeue(ഘടന cfs_rq *cfs_rq, ഘടന sched_entity *se)
{
	WARN_ON_ONCE(cfs_rq != cfs_rq_of(se));
	update_load_sub(&cfs_rq->load, se->load.weight);
	എങ്കിൽ (entity_is_task(se)) {
		ഘടന task_struct *p = task_of(se);
		ഘടന rq *rq = rq_of(cfs_rq);

		account_numa_dequeue(rq, p);
		account_llc_dequeue(rq, p);
		list_del_init(&se->group_കണ്ണി);
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
#define add_positive(_ptr, _val) ചെയ്യുക {                           \
	typeof(_ptr) ptr = (_ptr);                              \
	__signed_scalar_typeof(*ptr) val = (_val);              \
	typeof(*ptr) res, var = READ_ONCE(*ptr);                \
								\
	res = var + val;                                        \
								\
	എങ്കിൽ (val < 0 && res > var)                               \
		res = 0;                                        \
								\
	WRITE_ONCE(*ptr, res);                                  \
} ആകുമ്പോൾ (0)

/*
 * Remove and clamp on negative, from a local variable.
 *
 * A variant of sub_positive(), which does not use explicit load-store
 * and is thus optimized for local variable updates.
 */
#define lsub_positive(_ptr, _val) ചെയ്യുക {				\
	typeof(_ptr) ptr = (_ptr);				\
	*ptr -= min_t(typeof(*ptr), *ptr, _val);		\
} ആകുമ്പോൾ (0)


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
#define __update_sa(sa, name, delta_avg, delta_sum) ചെയ്യുക {	\
	add_positive(&(sa)->name##_avg, delta_avg);		\
	add_positive(&(sa)->name##_sum, delta_sum);		\
	(sa)->name##_sum = max_t(typeof((sa)->name##_sum),	\
			       (sa)->name##_sum,		\
			       (sa)->name##_avg * PELT_MIN_DIVIDER); \
} ആകുമ്പോൾ (0)

സ്ഥിതം വരിയിൽ ശൂന്യം
enqueue_load_avg(ഘടന cfs_rq *cfs_rq, ഘടന sched_entity *se)
{
	__update_sa(&cfs_rq->avg, load, se->avg.load_avg,
		    se_weight(se) * se->avg.load_sum);
}

സ്ഥിതം വരിയിൽ ശൂന്യം
dequeue_load_avg(ഘടന cfs_rq *cfs_rq, ഘടന sched_entity *se)
{
	__update_sa(&cfs_rq->avg, load, -se->avg.load_avg,
		    se_weight(se) * -se->avg.load_sum);
}

സ്ഥിതം ശൂന്യം
rescale_entity(ഘടന sched_entity *se, unsigned long weight, bool rel_vprot)
{
	long old_weight = se->h_load.weight;

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
	se->vlag = div64_long(se->vlag * old_weight, weight);

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
	എങ്കിൽ (se->rel_deadline)
		se->deadline = div64_long(se->deadline * old_weight, weight);

	എങ്കിൽ (rel_vprot)
		se->vprot = div64_long(se->vprot * old_weight, weight);
}

സ്ഥിതം ശൂന്യം reweight_eevdf(ഘടന cfs_rq *cfs_rq, ഘടന sched_entity *se,
			   unsigned long weight, bool on_rq)
{
	bool curr = cfs_rq->curr == se;
	bool rel_vprot = false;
	u64 avruntime = 0;

	എങ്കിൽ (se->h_load.weight == weight)
		തിരികെ;

	എങ്കിൽ (on_rq) {
		avruntime = avg_vruntime(cfs_rq);
		se->vlag = entity_lag(cfs_rq, se, avruntime);
		se->deadline -= avruntime;
		se->rel_deadline = 1;
		എങ്കിൽ (curr && protect_slice(se)) {
			se->vprot -= avruntime;
			rel_vprot = true;
		}

		cfs_rq->h_nr_queued--;
		എങ്കിൽ (!curr)
			__dequeue_entity(cfs_rq, se);
	}

	rescale_entity(se, weight, rel_vprot);

	update_load_set(&se->h_load, weight);

	എങ്കിൽ (on_rq) {
		എങ്കിൽ (rel_vprot)
			se->vprot += avruntime;
		se->deadline += avruntime;
		se->rel_deadline = 0;
		se->vruntime = avruntime - se->vlag;

		എങ്കിൽ (!curr)
			__enqueue_entity(cfs_rq, se);
		cfs_rq->h_nr_queued++;
	}
}

സ്ഥിതം ശൂന്യം reweight_entity(ഘടന cfs_rq *cfs_rq, ഘടന sched_entity *se,
			    unsigned long weight)
{
	എങ്കിൽ (se->load.weight == weight)
		തിരികെ;

	എങ്കിൽ (se->on_rq) {
		WARN_ON_ONCE(cfs_rq != cfs_rq_of(se));
		update_load_sub(&cfs_rq->load, se->load.weight);
	}
	dequeue_load_avg(cfs_rq, se);

	update_load_set(&se->load, weight);

	ചെയ്യുക {
		u32 divider = get_pelt_divider(&se->avg);
		se->avg.load_avg = div_u64(se_weight(se) * se->avg.load_sum, divider);
	} ആകുമ്പോൾ (0);

	enqueue_load_avg(cfs_rq, se);

	എങ്കിൽ (se->on_rq)
		update_load_add(&cfs_rq->load, se->load.weight);
}

/*
 * weight = NICE_0_LOAD;
 * for_each_entity_se(se)
 *   weight = __calc_prop_weight(cfs_rq_of(se), se, weight);
 */
സ്ഥിതം __always_inline
unsigned long __calc_prop_weight(ഘടന cfs_rq *cfs_rq, ഘടന sched_entity *se,
				 unsigned long weight)
{
	weight *= se->load.weight;
	എങ്കിൽ (parent_entity(se))
		weight /= cfs_rq->load.weight;
	അല്ലെങ്കിൽ
		weight /= NICE_0_LOAD;

	തിരികെ max(weight, MIN_SHARES);
}

സ്ഥിതം ശൂന്യം reweight_task_fair(ഘടന rq *rq, ഘടന task_struct *p,
			       സ്ഥിരം ഘടന load_weight *lw)
{
	ഘടന sched_entity *se = &p->se;
	unsigned long weight = NICE_0_LOAD;

	എങ്കിൽ (se->on_rq)
		update_curr_fair(rq);

	reweight_entity(cfs_rq_of(se), se, lw->weight);
	se->load.inv_weight = lw->inv_weight;

	എങ്കിൽ (!se->on_rq)
		തിരികെ;

	for_each_sched_entity(se)
		weight = __calc_prop_weight(cfs_rq_of(se), se, weight);

	reweight_eevdf(&rq->cfs, &p->se, weight, p->se.on_rq);
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ throttled_hierarchy(ഘടന cfs_rq *cfs_rq);

#ifdef CONFIG_FAIR_GROUP_SCHED
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
സ്ഥിതം long __calc_smp_shares(ഘടന cfs_rq *cfs_rq, long tg_shares, long shares_max)
{
	ഘടന task_group *tg = cfs_rq->tg;
	long tg_weight, load, shares;

	load = max(scale_load_down(cfs_rq->load.weight), cfs_rq->avg.load_avg);

	tg_weight = atomic_long_read(&tg->load_avg);

	/* Ensure tg_weight >= load */
	tg_weight -= cfs_rq->tg_load_avg_contrib;
	tg_weight += load;

	shares = (tg_shares * load);
	എങ്കിൽ (tg_weight)
		shares /= tg_weight;

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
	തിരികെ clamp_t(long, shares, MIN_SHARES, shares_max);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ tg_cpus(ഘടന task_group *tg)
{
	പൂർണ്ണസംഖ്യ nr = അക്കം_online_cpus();

	എങ്കിൽ (cpusets_enabled()) {
		ഘടന cgroup *cgrp = tg->css.cgroup;
		എങ്കിൽ (cgrp)
			nr = cpuset_അക്കം_cpus(cgrp);
	}

	/*
	 * An empty cpuset would propagate a 0 shares_max into
	 * __calc_smp_shares(), where clamp() yields hi when hi < lo and so
	 * defeats the MIN_SHARES floor. Match tg_tasks(), which floors at 1.
	 */
	തിരികെ max(nr, 1);
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ tg_tasks(ഘടന task_group *tg)
{
	തിരികെ max(1, atomic_long_read(&tg->runnable_avg) >> SCHED_CAPACITY_SHIFT);
}

/*
 * Func: fraction(nr_tasks * tg->shares)
 *
 * Scale tg->shares by the number of tasks.
 */
സ്ഥിതം long calc_tasks_shares(ഘടന cfs_rq *cfs_rq)
{
	ഘടന task_group *tg = cfs_rq->tg;
	പൂർണ്ണസംഖ്യ nr = tg_tasks(tg);
	long tg_shares = READ_ONCE(tg->shares);
	തിരികെ __calc_smp_shares(cfs_rq, nr * tg_shares, nr * tg_shares);
}

/*
 * Func: min(fraction(nr_cpus * tg->shares), nice -20)
 *
 * Scale tg->shares by the maximal number of CPUs; but clip the max shares at
 * nice -20, otherwise a single spinner on a 512 CPU machine would result in
 * 512*NICE_0_LOAD, which is also crazy.
 */
സ്ഥിതം long calc_max_shares(ഘടന cfs_rq *cfs_rq)
{
	ഘടന task_group *tg = cfs_rq->tg;
	പൂർണ്ണസംഖ്യ nr = tg_cpus(tg);
	long tg_shares = READ_ONCE(tg->shares);
	long max_shares = scale_load(sched_prio_to_weight[0]);
	തിരികെ __calc_smp_shares(cfs_rq, tg_shares * nr, max_shares);
}

/*
 * Func: fraction(nr * tg->shares); nr = min(nr_tasks, nr_cpus)
 *
 * Scales between "smp" and "max" in a natural way. No longer needs clipping
 * since there are no unnatural inflations like with "max".
 */
സ്ഥിതം long calc_concur_shares(ഘടന cfs_rq *cfs_rq)
{
	ഘടന task_group *tg = cfs_rq->tg;
	പൂർണ്ണസംഖ്യ nr = min(tg_tasks(tg), tg_cpus(tg));
	long tg_shares = READ_ONCE(tg->shares);
	തിരികെ __calc_smp_shares(cfs_rq, nr * tg_shares, nr * tg_shares);
}

/*
 * Func: fraction(tg->shares)
 *
 * This infamously results in tiny shares when you have many CPUs.
 */
സ്ഥിതം long calc_smp_shares(ഘടന cfs_rq *cfs_rq)
{
	ഘടന task_group *tg = cfs_rq->tg;
	long tg_shares = READ_ONCE(tg->shares);
	തിരികെ __calc_smp_shares(cfs_rq, tg_shares, tg_shares);
}

/*
 * Ignore this pesky SMP stuff, use (4).
 */
സ്ഥിതം long calc_up_shares(ഘടന cfs_rq *cfs_rq)
{
	ഘടന task_group *tg = cfs_rq->tg;
	തിരികെ READ_ONCE(tg->shares);
}

DEFINE_STATIC_CALL(calc_group_shares, calc_concur_shares);

ശൂന്യം __sched_cgroup_mode_update(പൂർണ്ണസംഖ്യ mode)
{
	long (*func)(ഘടന cfs_rq *);
	തിരഞ്ഞെടുപ്പ് (mode) {
	അവസ്ഥ 0:
		func = &calc_up_shares;
		നിർത്തുക;
	അവസ്ഥ 1:
		func = &calc_smp_shares;
		നിർത്തുക;
	അവസ്ഥ 2:
	സ്ഥിരസ്ഥിതി:
		func = &calc_concur_shares;
		നിർത്തുക;
	അവസ്ഥ 3:
		func = &calc_max_shares;
		നിർത്തുക;
	അവസ്ഥ 4:
		func = &calc_tasks_shares;
		നിർത്തുക;
	}
	static_call_update(calc_group_shares, func);
}

/*
 * Recomputes the group entity based on the current state of its group
 * runqueue.
 */
സ്ഥിതം ശൂന്യം update_cfs_group(ഘടന sched_entity *se)
{
	ഘടന cfs_rq *gcfs_rq = group_cfs_rq(se);
	long shares;

	/*
	 * When a group becomes empty, preserve its weight. This matters for
	 * DELAY_DEQUEUE.
	 */
	എങ്കിൽ (!gcfs_rq || !gcfs_rq->load.weight)
		തിരികെ;

	shares = static_call(calc_group_shares)(gcfs_rq);
	reweight_entity(cfs_rq_of(se), se, shares);
}

#അല്ലെങ്കിൽ /* !CONFIG_FAIR_GROUP_SCHED: */
സ്ഥിതം വരിയിൽ ശൂന്യം update_cfs_group(ഘടന sched_entity *se)
{
}
#endif /* !CONFIG_FAIR_GROUP_SCHED */

സ്ഥിതം വരിയിൽ ശൂന്യം cfs_rq_util_change(ഘടന cfs_rq *cfs_rq, പൂർണ്ണസംഖ്യ flags)
{
	ഘടന rq *rq = rq_of(cfs_rq);

	എങ്കിൽ (&rq->cfs == cfs_rq) {
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

സ്ഥിതം വരിയിൽ bool load_avg_is_decayed(ഘടന sched_avg *sa)
{
	എങ്കിൽ (sa->load_sum)
		തിരികെ false;

	എങ്കിൽ (sa->util_sum)
		തിരികെ false;

	എങ്കിൽ (sa->runnable_sum)
		തിരികെ false;

	/*
	 * _avg must be null when _sum are null because _avg = _sum / divider
	 * Make sure that rounding and/or propagation of PELT values never
	 * break this.
	 */
	WARN_ON_ONCE(sa->load_avg ||
		      sa->util_avg ||
		      sa->runnable_avg);

	തിരികെ true;
}

സ്ഥിതം വരിയിൽ u64 cfs_rq_last_update_time(ഘടന cfs_rq *cfs_rq)
{
	തിരികെ u64_u32_load_copy(cfs_rq->avg.last_update_time,
				 cfs_rq->last_update_time_copy);
}
#ifdef CONFIG_FAIR_GROUP_SCHED
/*
 * Because list_add_leaf_cfs_rq always places a child cfs_rq on the list
 * immediately before a parent cfs_rq, and cfs_rqs are removed from the list
 * bottom-up, we only have to test whether the cfs_rq before us on the list
 * is our child.
 * If cfs_rq is not on the list, test whether a child needs its to be added to
 * connect a branch to the tree  * (see list_add_leaf_cfs_rq() for details).
 */
സ്ഥിതം വരിയിൽ bool child_cfs_rq_on_list(ഘടന cfs_rq *cfs_rq)
{
	ഘടന cfs_rq *prev_cfs_rq;
	ഘടന list_head *prev;
	ഘടന rq *rq = rq_of(cfs_rq);

	എങ്കിൽ (cfs_rq->on_list) {
		prev = cfs_rq->leaf_cfs_rq_list.prev;
	} അല്ലെങ്കിൽ {
		prev = rq->tmp_alone_branch;
	}

	എങ്കിൽ (prev == &rq->leaf_cfs_rq_list)
		തിരികെ false;

	prev_cfs_rq = container_of(prev, ഘടന cfs_rq, leaf_cfs_rq_list);

	തിരികെ (prev_cfs_rq->tg->parent == cfs_rq->tg);
}

സ്ഥിതം വരിയിൽ bool cfs_rq_is_decayed(ഘടന cfs_rq *cfs_rq)
{
	എങ്കിൽ (cfs_rq->load.weight)
		തിരികെ false;

	എങ്കിൽ (!load_avg_is_decayed(&cfs_rq->avg))
		തിരികെ false;

	എങ്കിൽ (child_cfs_rq_on_list(cfs_rq))
		തിരികെ false;

	എങ്കിൽ (cfs_rq->tg_load_avg_contrib)
		തിരികെ false;

	തിരികെ true;
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
സ്ഥിതം വരിയിൽ ശൂന്യം update_tg_load_avg(ഘടന cfs_rq *cfs_rq)
{
	long dl, dr;
	u64 now;

	/*
	 * No need to update load_avg for root_task_group as it is not used.
	 */
	എങ്കിൽ (cfs_rq->tg == &വേര്_task_group)
		തിരികെ;

	/* rq has been offline and doesn't contribute to the share anymore: */
	എങ്കിൽ (!cpu_active(cpu_of(rq_of(cfs_rq))))
		തിരികെ;

	/*
	 * For migration heavy workloads, access to tg->load_avg can be
	 * unbound. Limit the update rate to at most once per ms.
	 */
	now = rq_clock(rq_of(cfs_rq));
	എങ്കിൽ (now - cfs_rq->last_update_tg_load_avg < NSEC_PER_MSEC)
		തിരികെ;

	dl = cfs_rq->avg.load_avg - cfs_rq->tg_load_avg_contrib;
	dr = cfs_rq->avg.runnable_avg - cfs_rq->tg_runnable_avg_contrib;
	എങ്കിൽ (abs(dl) > cfs_rq->tg_load_avg_contrib / 64 ||
	    abs(dr) > cfs_rq->tg_runnable_avg_contrib / 64) {
		atomic_long_add(dl, &cfs_rq->tg->load_avg);
		atomic_long_add(dr, &cfs_rq->tg->runnable_avg);
		cfs_rq->tg_load_avg_contrib = cfs_rq->avg.load_avg;
		cfs_rq->tg_runnable_avg_contrib = cfs_rq->avg.runnable_avg;
		cfs_rq->last_update_tg_load_avg = now;
	}
}

സ്ഥിതം വരിയിൽ ശൂന്യം clear_tg_load_avg(ഘടന cfs_rq *cfs_rq)
{
	long dl, dr;
	u64 now;

	/*
	 * No need to update load_avg for root_task_group, as it is not used.
	 */
	എങ്കിൽ (cfs_rq->tg == &വേര്_task_group)
		തിരികെ;

	now = rq_clock(rq_of(cfs_rq));
	dl = 0 - cfs_rq->tg_load_avg_contrib;
	dr = 0 - cfs_rq->tg_runnable_avg_contrib;
	atomic_long_add(dl, &cfs_rq->tg->load_avg);
	atomic_long_add(dr, &cfs_rq->tg->runnable_avg);
	cfs_rq->tg_load_avg_contrib = 0;
	cfs_rq->tg_runnable_avg_contrib = 0;
	cfs_rq->last_update_tg_load_avg = now;
}

/* CPU offline callback: */
സ്ഥിതം ശൂന്യം __maybe_unused clear_tg_offline_cfs_rqs(ഘടന rq *rq)
{
	ഘടന task_group *tg;

	lockdep_assert_rq_held(rq);

	/*
	 * The rq clock has already been updated in
	 * set_rq_offline(), so we should skip updating
	 * the rq clock again in unthrottle_cfs_rq().
	 */
	rq_clock_start_loop_update(rq);

	guard(rcu)();

	list_for_each_entry_rcu(tg, &task_groups, list) {
		ഘടന cfs_rq *cfs_rq = tg_cfs_rq(tg, cpu_of(rq));

		clear_tg_load_avg(cfs_rq);
	}

	rq_clock_stop_loop_update(rq);
}

/*
 * Called within set_task_rq() right before setting a task's CPU. The
 * caller only guarantees p->pi_lock is held; no other assumptions,
 * including the state of rq->lock, should be made.
 */
ശൂന്യം set_task_rq_fair(ഘടന sched_entity *se,
		      ഘടന cfs_rq *prev, ഘടന cfs_rq *next)
{
	u64 p_last_update_time;
	u64 n_last_update_time;

	എങ്കിൽ (!sched_feat(ATTACH_AGE_LOAD))
		തിരികെ;

	/*
	 * We are supposed to update the task to "current" time, then its up to
	 * date and ready to go to new CPU/cfs_rq. But we have difficulty in
	 * getting what current time is, so simply throw away the out-of-date
	 * time. This will result in the wakee task is less decayed, but giving
	 * the wakee more load sounds not bad.
	 */
	എങ്കിൽ (!(se->avg.last_update_time && prev))
		തിരികെ;

	p_last_update_time = cfs_rq_last_update_time(prev);
	n_last_update_time = cfs_rq_last_update_time(next);

	__update_load_avg_blocked_se(p_last_update_time, se);
	se->avg.last_update_time = n_last_update_time;
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
സ്ഥിതം വരിയിൽ ശൂന്യം
update_tg_cfs_util(ഘടന cfs_rq *cfs_rq, ഘടന sched_entity *se, ഘടന cfs_rq *gcfs_rq)
{
	long delta_sum, delta_avg = gcfs_rq->avg.util_avg - se->avg.util_avg;
	u32 new_sum, divider;

	/* Nothing to update */
	എങ്കിൽ (!delta_avg)
		തിരികെ;

	/*
	 * cfs_rq->avg.period_contrib can be used for both cfs_rq and se.
	 * See ___update_load_avg() for details.
	 */
	divider = get_pelt_divider(&cfs_rq->avg);

	/* Set new sched_entity's utilization */
	se->avg.util_avg = gcfs_rq->avg.util_avg;
	new_sum = se->avg.util_avg * divider;
	delta_sum = (long)new_sum - (long)se->avg.util_sum;
	se->avg.util_sum = new_sum;

	/* Update parent cfs_rq utilization */
	__update_sa(&cfs_rq->avg, util, delta_avg, delta_sum);
}

സ്ഥിതം വരിയിൽ ശൂന്യം
update_tg_cfs_runnable(ഘടന cfs_rq *cfs_rq, ഘടന sched_entity *se, ഘടന cfs_rq *gcfs_rq)
{
	long delta_sum, delta_avg = gcfs_rq->avg.runnable_avg - se->avg.runnable_avg;
	u64 new_sum;
	u32 divider;

	/* Nothing to update */
	എങ്കിൽ (!delta_avg)
		തിരികെ;

	/*
	 * cfs_rq->avg.period_contrib can be used for both cfs_rq and se.
	 * See ___update_load_avg() for details.
	 */
	divider = get_pelt_divider(&cfs_rq->avg);

	/* Set new sched_entity's runnable */
	se->avg.runnable_avg = gcfs_rq->avg.runnable_avg;
	new_sum = (u64)se->avg.runnable_avg * divider;
	delta_sum = (long)new_sum - (long)se->avg.runnable_sum;
	se->avg.runnable_sum = new_sum;

	/* Update parent cfs_rq runnable */
	__update_sa(&cfs_rq->avg, runnable, delta_avg, delta_sum);
}

സ്ഥിതം വരിയിൽ ശൂന്യം
update_tg_cfs_load(ഘടന cfs_rq *cfs_rq, ഘടന sched_entity *se, ഘടന cfs_rq *gcfs_rq)
{
	long delta_avg, running_sum, runnable_sum = gcfs_rq->prop_runnable_sum;
	unsigned long load_avg;
	u64 load_sum = 0;
	s64 delta_sum;
	u32 divider;

	എങ്കിൽ (!runnable_sum)
		തിരികെ;

	gcfs_rq->prop_runnable_sum = 0;

	/*
	 * cfs_rq->avg.period_contrib can be used for both cfs_rq and se.
	 * See ___update_load_avg() for details.
	 */
	divider = get_pelt_divider(&cfs_rq->avg);

	എങ്കിൽ (runnable_sum >= 0) {
		/*
		 * Add runnable; clip at LOAD_AVG_MAX. Reflects that until
		 * the CPU is saturated running == runnable.
		 */
		runnable_sum += se->avg.load_sum;
		runnable_sum = min_t(long, runnable_sum, divider);
	} അല്ലെങ്കിൽ {
		/*
		 * Estimate the new unweighted runnable_sum of the gcfs_rq by
		 * assuming all tasks are equally runnable.
		 */
		എങ്കിൽ (scale_load_down(gcfs_rq->load.weight)) {
			load_sum = div_u64(gcfs_rq->avg.load_sum,
				scale_load_down(gcfs_rq->load.weight));
		}

		/* But make sure to not inflate se's runnable */
		runnable_sum = min(se->avg.load_sum, load_sum);
	}

	/*
	 * runnable_sum can't be lower than running_sum
	 * Rescale running sum to be in the same range as runnable sum
	 * running_sum is in [0 : LOAD_AVG_MAX <<  SCHED_CAPACITY_SHIFT]
	 * runnable_sum is in [0 : LOAD_AVG_MAX]
	 */
	running_sum = se->avg.util_sum >> SCHED_CAPACITY_SHIFT;
	runnable_sum = max(runnable_sum, running_sum);

	load_sum = se_weight(se) * runnable_sum;
	load_avg = div_u64(load_sum, divider);

	delta_avg = load_avg - se->avg.load_avg;
	എങ്കിൽ (!delta_avg)
		തിരികെ;

	delta_sum = load_sum - (s64)se_weight(se) * se->avg.load_sum;

	se->avg.load_sum = runnable_sum;
	se->avg.load_avg = load_avg;
	__update_sa(&cfs_rq->avg, load, delta_avg, delta_sum);
}

സ്ഥിതം വരിയിൽ ശൂന്യം add_tg_cfs_propagate(ഘടന cfs_rq *cfs_rq, long runnable_sum)
{
	cfs_rq->propagate = 1;
	cfs_rq->prop_runnable_sum += runnable_sum;
}

/* Update task and its cfs_rq load average */
സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ propagate_entity_load_avg(ഘടന sched_entity *se)
{
	ഘടന cfs_rq *cfs_rq, *gcfs_rq;

	എങ്കിൽ (entity_is_task(se))
		തിരികെ 0;

	gcfs_rq = group_cfs_rq(se);
	എങ്കിൽ (!gcfs_rq->propagate)
		തിരികെ 0;

	gcfs_rq->propagate = 0;

	cfs_rq = cfs_rq_of(se);

	add_tg_cfs_propagate(cfs_rq, gcfs_rq->prop_runnable_sum);

	update_tg_cfs_util(cfs_rq, se, gcfs_rq);
	update_tg_cfs_runnable(cfs_rq, se, gcfs_rq);
	update_tg_cfs_load(cfs_rq, se, gcfs_rq);

	trace_pelt_cfs_tp(cfs_rq);
	trace_pelt_se_tp(se);

	തിരികെ 1;
}

/*
 * Check if we need to update the load and the utilization of a blocked
 * group_entity:
 */
സ്ഥിതം വരിയിൽ bool skip_blocked_update(ഘടന sched_entity *se)
{
	ഘടന cfs_rq *gcfs_rq = group_cfs_rq(se);

	/*
	 * If sched_entity still have not zero load or utilization, we have to
	 * decay it:
	 */
	എങ്കിൽ (se->avg.load_avg || se->avg.util_avg)
		തിരികെ false;

	/*
	 * If there is a pending propagation, we have to update the load and
	 * the utilization of the sched_entity:
	 */
	എങ്കിൽ (gcfs_rq->propagate)
		തിരികെ false;

	/*
	 * Otherwise, the load and the utilization of the sched_entity is
	 * already zero and there is no pending propagation, so it will be a
	 * waste of time to try to decay it:
	 */
	തിരികെ true;
}

#അല്ലെങ്കിൽ /* !CONFIG_FAIR_GROUP_SCHED: */

സ്ഥിതം വരിയിൽ ശൂന്യം update_tg_load_avg(ഘടന cfs_rq *cfs_rq) {}

സ്ഥിതം വരിയിൽ ശൂന്യം clear_tg_offline_cfs_rqs(ഘടന rq *rq) {}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ propagate_entity_load_avg(ഘടന sched_entity *se)
{
	തിരികെ 0;
}

സ്ഥിതം വരിയിൽ ശൂന്യം add_tg_cfs_propagate(ഘടന cfs_rq *cfs_rq, long runnable_sum) {}

#endif /* !CONFIG_FAIR_GROUP_SCHED */

#ifdef CONFIG_NO_HZ_COMMON
സ്ഥിതം വരിയിൽ ശൂന്യം migrate_se_pelt_lag(ഘടന sched_entity *se)
{
	u64 throttled = 0, now, lut;
	ഘടന cfs_rq *cfs_rq;
	ഘടന rq *rq;
	bool is_idle;

	എങ്കിൽ (load_avg_is_decayed(&se->avg))
		തിരികെ;

	cfs_rq = cfs_rq_of(se);
	rq = rq_of(cfs_rq);

	rcu_read_lock();
	is_idle = is_idle_task(rcu_dereference_all(rq->curr));
	rcu_read_unlock();

	/*
	 * The lag estimation comes with a cost we don't want to pay all the
	 * time. Hence, limiting to the case where the source CPU is idle and
	 * we know we are at the greatest risk to have an outdated clock.
	 */
	എങ്കിൽ (!is_idle)
		തിരികെ;

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

#ifdef CONFIG_CFS_BANDWIDTH
	throttled = u64_u32_load(cfs_rq->throttled_pelt_idle);
	/* The clock has been stopped for throttling */
	എങ്കിൽ (throttled == U64_MAX)
		തിരികെ;
#endif
	now = u64_u32_load(rq->clock_pelt_idle);
	/*
	 * Paired with _update_idle_rq_clock_pelt(). It ensures at the worst case
	 * is observed the old clock_pelt_idle value and the new clock_idle,
	 * which lead to an underestimation. The opposite would lead to an
	 * overestimation.
	 */
	smp_rmb();
	lut = cfs_rq_last_update_time(cfs_rq);

	now -= throttled;
	എങ്കിൽ (now < lut)
		/*
		 * cfs_rq->avg.last_update_time is more recent than our
		 * estimation, let's use it.
		 */
		now = lut;
	അല്ലെങ്കിൽ
		now += sched_clock_cpu(cpu_of(rq)) - u64_u32_load(rq->clock_idle);

	__update_load_avg_blocked_se(now, se);
}
#അല്ലെങ്കിൽ /* !CONFIG_NO_HZ_COMMON: */
സ്ഥിതം ശൂന്യം migrate_se_pelt_lag(ഘടന sched_entity *se) {}
#endif /* !CONFIG_NO_HZ_COMMON */

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
സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
update_cfs_rq_load_avg(u64 now, ഘടന cfs_rq *cfs_rq)
{
	unsigned long removed_load = 0, removed_util = 0, removed_runnable = 0;
	ഘടന sched_avg *sa = &cfs_rq->avg;
	പൂർണ്ണസംഖ്യ decayed = 0;

	എങ്കിൽ (cfs_rq->removed.nr) {
		unsigned long r;
		u32 divider = get_pelt_divider(&cfs_rq->avg);

		raw_spin_lock(&cfs_rq->removed.lock);
		swap(cfs_rq->removed.util_avg, removed_util);
		swap(cfs_rq->removed.load_avg, removed_load);
		swap(cfs_rq->removed.runnable_avg, removed_runnable);
		cfs_rq->removed.nr = 0;
		raw_spin_unlock(&cfs_rq->removed.lock);

		r = removed_load;
		__update_sa(sa, load, -r, -r*divider);

		r = removed_util;
		__update_sa(sa, util, -r, -r*divider);

		r = removed_runnable;
		__update_sa(sa, runnable, -r, -r*divider);

		/*
		 * removed_runnable is the unweighted version of removed_load so we
		 * can use it to estimate removed_load_sum.
		 */
		add_tg_cfs_propagate(cfs_rq,
			-(long)(removed_runnable * divider) >> SCHED_CAPACITY_SHIFT);

		decayed = 1;
	}

	decayed |= __update_load_avg_cfs_rq(now, cfs_rq);
	u64_u32_store_copy(sa->last_update_time,
			   cfs_rq->last_update_time_copy,
			   sa->last_update_time);
	തിരികെ decayed;
}

/**
 * attach_entity_load_avg - attach this entity to its cfs_rq load avg
 * @cfs_rq: cfs_rq to attach to
 * @se: sched_entity to attach
 *
 * Must call update_cfs_rq_load_avg() before this, since we rely on
 * cfs_rq->avg.last_update_time being current.
 */
സ്ഥിതം ശൂന്യം attach_entity_load_avg(ഘടന cfs_rq *cfs_rq, ഘടന sched_entity *se)
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
	se->avg.last_update_time = cfs_rq->avg.last_update_time;
	se->avg.period_contrib = cfs_rq->avg.period_contrib;

	/*
	 * Hell(o) Nasty stuff.. we need to recompute _sum based on the new
	 * period_contrib. This isn't strictly correct, but since we're
	 * entirely outside of the PELT hierarchy, nobody cares if we truncate
	 * _sum a little.
	 */
	se->avg.util_sum = se->avg.util_avg * divider;

	se->avg.runnable_sum = se->avg.runnable_avg * divider;

	se->avg.load_sum = se->avg.load_avg * divider;
	എങ്കിൽ (se_weight(se) < se->avg.load_sum)
		se->avg.load_sum = div_u64(se->avg.load_sum, se_weight(se));
	അല്ലെങ്കിൽ
		se->avg.load_sum = 1;

	enqueue_load_avg(cfs_rq, se);
	cfs_rq->avg.util_avg += se->avg.util_avg;
	cfs_rq->avg.util_sum += se->avg.util_sum;
	cfs_rq->avg.runnable_avg += se->avg.runnable_avg;
	cfs_rq->avg.runnable_sum += se->avg.runnable_sum;

	add_tg_cfs_propagate(cfs_rq, se->avg.load_sum);

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
സ്ഥിതം ശൂന്യം detach_entity_load_avg(ഘടന cfs_rq *cfs_rq, ഘടന sched_entity *se)
{
	dequeue_load_avg(cfs_rq, se);
	__update_sa(&cfs_rq->avg, util, -se->avg.util_avg, -se->avg.util_sum);
	__update_sa(&cfs_rq->avg, runnable, -se->avg.runnable_avg, -se->avg.runnable_sum);

	add_tg_cfs_propagate(cfs_rq, -se->avg.load_sum);

	cfs_rq_util_change(cfs_rq, 0);

	trace_pelt_cfs_tp(cfs_rq);
}

#define UTIL_EST_MARGIN (SCHED_CAPACITY_SCALE / 100)

സ്ഥിതം വരിയിൽ ശൂന്യം util_est_update(ഘടന sched_entity *se)
{
	unsigned പൂർണ്ണസംഖ്യ ewma, dequeued, last_ewma_diff;

	എങ്കിൽ (!sched_feat(UTIL_EST))
		തിരികെ;

	/* Get current estimate of utilization */
	ewma = READ_ONCE(se->avg.util_est);

	/*
	 * If the PELT values haven't changed since enqueue time,
	 * skip the util_est update.
	 */
	എങ്കിൽ (ewma & UTIL_AVG_UNCHANGED)
		തിരികെ;

	/* Get utilization at dequeue */
	dequeued = READ_ONCE(se->avg.util_avg);

	/*
	 * Reset EWMA on utilization increases, the moving average is used only
	 * to smooth utilization decreases.
	 */
	എങ്കിൽ (ewma <= dequeued) {
		ewma = dequeued;
		പോകുക done;
	}

	/*
	 * Skip update of task's estimated utilization when its members are
	 * already ~1% close to its last activation value.
	 */
	last_ewma_diff = ewma - dequeued;
	എങ്കിൽ (last_ewma_diff < UTIL_EST_MARGIN)
		പോകുക done;

	/*
	 * To avoid underestimate of task utilization, skip updates of EWMA if
	 * we cannot grant that thread got all CPU time it wanted.
	 */
	എങ്കിൽ ((dequeued + UTIL_EST_MARGIN) < READ_ONCE(se->avg.runnable_avg))
		പോകുക done;

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
	ewma <<= UTIL_EST_WEIGHT_SHIFT;
	ewma  -= last_ewma_diff;
	ewma >>= UTIL_EST_WEIGHT_SHIFT;
done:
	ewma |= UTIL_AVG_UNCHANGED;
	WRITE_ONCE(se->avg.util_est, ewma);

	trace_sched_util_est_se_tp(se);
}

/*
 * Optional action to be done while updating the load average
 */
#define UPDATE_TG	0x01
#define SKIP_AGE_LOAD	0x02
#define DO_ATTACH	0x04
#define DO_DETACH	0x08
#define UPDATE_UTIL_EST	0x10

/* Update task and its cfs_rq load average */
സ്ഥിതം വരിയിൽ ശൂന്യം update_load_avg(ഘടന cfs_rq *cfs_rq, ഘടന sched_entity *se, പൂർണ്ണസംഖ്യ flags)
{
	u64 now = cfs_rq_clock_pelt(cfs_rq);
	പൂർണ്ണസംഖ്യ decayed;

	/*
	 * Track task load average for carrying it to new CPU after migrated, and
	 * track group sched_entity load average for task_h_load calculation in migration
	 */
	എങ്കിൽ (se->avg.last_update_time && !(flags & SKIP_AGE_LOAD))
		__update_load_avg_se(now, cfs_rq, se);

	decayed  = update_cfs_rq_load_avg(now, cfs_rq);
	decayed |= propagate_entity_load_avg(se);

	എങ്കിൽ (!se->avg.last_update_time && (flags & DO_ATTACH)) {

		/*
		 * DO_ATTACH means we're here from enqueue_entity().
		 * !last_update_time means we've passed through
		 * migrate_task_rq_fair() indicating we migrated.
		 *
		 * IOW we're enqueueing a task on a new CPU.
		 */
		attach_entity_load_avg(cfs_rq, se);
		update_tg_load_avg(cfs_rq);

	} അല്ലെങ്കിൽ എങ്കിൽ (flags & DO_DETACH) {
		/*
		 * DO_DETACH means we're here from dequeue_entity()
		 * and we are migrating task out of the CPU.
		 */
		detach_entity_load_avg(cfs_rq, se);
		update_tg_load_avg(cfs_rq);
	} അല്ലെങ്കിൽ എങ്കിൽ (decayed) {
		cfs_rq_util_change(cfs_rq, 0);

		എങ്കിൽ (flags & UPDATE_TG)
			update_tg_load_avg(cfs_rq);
	}

	എങ്കിൽ (flags & UPDATE_UTIL_EST)
		util_est_update(se);
}

/*
 * Synchronize entity load avg of dequeued entity without locking
 * the previous rq.
 */
സ്ഥിതം ശൂന്യം sync_entity_load_avg(ഘടന sched_entity *se)
{
	ഘടന cfs_rq *cfs_rq = cfs_rq_of(se);
	u64 last_update_time;

	last_update_time = cfs_rq_last_update_time(cfs_rq);
	__update_load_avg_blocked_se(last_update_time, se);
}

/*
 * Task first catches up with cfs_rq, and then subtract
 * itself from the cfs_rq (task must be off the queue now).
 */
സ്ഥിതം ശൂന്യം remove_entity_load_avg(ഘടന sched_entity *se)
{
	ഘടന cfs_rq *cfs_rq = cfs_rq_of(se);
	unsigned long flags;

	/*
	 * tasks cannot exit without having gone through wake_up_new_task() ->
	 * enqueue_task_fair() which will have added things to the cfs_rq,
	 * so we can remove unconditionally.
	 */

	sync_entity_load_avg(se);

	raw_spin_lock_irqsave(&cfs_rq->removed.lock, flags);
	++cfs_rq->removed.nr;
	cfs_rq->removed.util_avg	+= se->avg.util_avg;
	cfs_rq->removed.load_avg	+= se->avg.load_avg;
	cfs_rq->removed.runnable_avg	+= se->avg.runnable_avg;
	raw_spin_unlock_irqrestore(&cfs_rq->removed.lock, flags);
}

സ്ഥിതം വരിയിൽ unsigned long cfs_rq_runnable_avg(ഘടന cfs_rq *cfs_rq)
{
	തിരികെ cfs_rq->avg.runnable_avg;
}

സ്ഥിതം വരിയിൽ unsigned long cfs_rq_load_avg(ഘടന cfs_rq *cfs_rq)
{
	തിരികെ cfs_rq->avg.load_avg;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ sched_balance_newidle(ഘടന rq *this_rq, ഘടന rq_flags *rf)
	__must_hold(__rq_lockp(this_rq));

സ്ഥിതം വരിയിൽ unsigned long task_util(ഘടന task_struct *p)
{
	തിരികെ READ_ONCE(p->se.avg.util_avg);
}

സ്ഥിതം വരിയിൽ unsigned long _task_util_est(ഘടന task_struct *p)
{
	തിരികെ READ_ONCE(p->se.avg.util_est) & ~UTIL_AVG_UNCHANGED;
}

സ്ഥിതം വരിയിൽ unsigned long task_util_est(ഘടന task_struct *p)
{
	തിരികെ max(task_util(p), _task_util_est(p));
}

സ്ഥിതം വരിയിൽ ശൂന്യം util_est_enqueue(ഘടന cfs_rq *cfs_rq,
				    ഘടന task_struct *p)
{
	unsigned പൂർണ്ണസംഖ്യ enqueued;

	എങ്കിൽ (!sched_feat(UTIL_EST))
		തിരികെ;

	/* Update root cfs_rq's estimated utilization */
	enqueued  = cfs_rq->avg.util_est;
	enqueued += _task_util_est(p);
	WRITE_ONCE(cfs_rq->avg.util_est, enqueued);

	trace_sched_util_est_cfs_tp(cfs_rq);
}

സ്ഥിതം വരിയിൽ ശൂന്യം util_est_dequeue(ഘടന cfs_rq *cfs_rq,
				    ഘടന task_struct *p)
{
	unsigned പൂർണ്ണസംഖ്യ enqueued;

	എങ്കിൽ (!sched_feat(UTIL_EST))
		തിരികെ;

	/* Update root cfs_rq's estimated utilization */
	enqueued  = cfs_rq->avg.util_est;
	enqueued -= min_t(unsigned പൂർണ്ണസംഖ്യ, enqueued, _task_util_est(p));
	WRITE_ONCE(cfs_rq->avg.util_est, enqueued);

	trace_sched_util_est_cfs_tp(cfs_rq);
}

സ്ഥിതം വരിയിൽ unsigned long get_actual_cpu_capacity(പൂർണ്ണസംഖ്യ cpu)
{
	unsigned long capacity = arch_scale_cpu_capacity(cpu);

	capacity -= max(hw_load_avg(cpu_rq(cpu)), cpufreq_get_pressure(cpu));

	തിരികെ capacity;
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ util_fits_cpu(unsigned long util,
				unsigned long uclamp_min,
				unsigned long uclamp_max,
				പൂർണ്ണസംഖ്യ cpu)
{
	unsigned long capacity = capacity_of(cpu);
	unsigned long capacity_orig;
	bool fits, uclamp_max_fits;

	/*
	 * Check if the real util fits without any uclamp boost/cap applied.
	 */
	fits = fits_capacity(util, capacity);

	എങ്കിൽ (!uclamp_is_used())
		തിരികെ fits;

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
	capacity_orig = arch_scale_cpu_capacity(cpu);

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
	uclamp_max_fits = (capacity_orig == SCHED_CAPACITY_SCALE) && (uclamp_max == SCHED_CAPACITY_SCALE);
	uclamp_max_fits = !uclamp_max_fits && (uclamp_max <= capacity_orig);
	fits = fits || uclamp_max_fits;

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
	എങ്കിൽ (fits && (util < uclamp_min) &&
	    (uclamp_min > get_actual_cpu_capacity(cpu)))
		തിരികെ -1;

	തിരികെ fits;
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ task_fits_cpu(ഘടന task_struct *p, പൂർണ്ണസംഖ്യ cpu)
{
	unsigned long uclamp_min = uclamp_eff_മൂല്യം(p, UCLAMP_MIN);
	unsigned long uclamp_max = uclamp_eff_മൂല്യം(p, UCLAMP_MAX);
	unsigned long util = task_util_est(p);
	/*
	 * Return true only if the cpu fully fits the task requirements, which
	 * include the utilization but also the performance hints.
	 */
	തിരികെ (util_fits_cpu(util, uclamp_min, uclamp_max, cpu) > 0);
}

സ്ഥിതം വരിയിൽ ശൂന്യം update_misfit_status(ഘടന task_struct *p, ഘടന rq *rq)
{
	പൂർണ്ണസംഖ്യ cpu = cpu_of(rq);

	എങ്കിൽ (!sched_asym_cpucap_active())
		തിരികെ;

	/*
	 * Affinity allows us to go somewhere higher?  Or are we on biggest
	 * available CPU already? Or do we fit into this CPU ?
	 */
	എങ്കിൽ (!p || (p->nr_cpus_allowed == 1) ||
	    (arch_scale_cpu_capacity(cpu) == p->max_allowed_capacity) ||
	    task_fits_cpu(p, cpu)) {

		rq->misfit_task_load = 0;
		തിരികെ;
	}

	/*
	 * Make sure that misfit_task_load will not be null even if
	 * task_h_load() returns 0.
	 */
	rq->misfit_task_load = max_t(unsigned long, task_h_load(p), 1);
}

ശൂന്യം __setparam_fair(ഘടന task_struct *p, സ്ഥിരം ഘടന sched_attr *attr)
{
	ഘടന sched_entity *se = &p->se;

	p->static_prio = NICE_TO_PRIO(attr->sched_nice);
	എങ്കിൽ (attr->sched_runtime) {
		se->custom_slice = 1;
		se->slice = clamp_t(u64, attr->sched_runtime,
				      NSEC_PER_MSEC/10,   /* HZ=1000 * 10 */
				      NSEC_PER_MSEC*100); /* HZ=100  / 10 */
	} അല്ലെങ്കിൽ {
		se->custom_slice = 0;
		se->slice = sysctl_sched_base_slice;
	}
}

സ്ഥിതം ശൂന്യം
place_entity(ഘടന cfs_rq *cfs_rq, ഘടന sched_entity *se, പൂർണ്ണസംഖ്യ flags)
{
	u64 vslice, vruntime = avg_vruntime(cfs_rq);
	unsigned പൂർണ്ണസംഖ്യ nr_queued = cfs_rq->h_nr_queued;
	bool update_zero = false;
	s64 lag = 0;

	എങ്കിൽ (!se->custom_slice)
		se->slice = sysctl_sched_base_slice;
	vslice = calc_delta_fair(se->slice, se);

	എങ്കിൽ (flags & ENQUEUE_QUEUED)
		nr_queued -= 1;

	/*
	 * Due to how V is constructed as the weighted average of entities,
	 * adding tasks with positive lag, or removing tasks with negative lag
	 * will move 'time' backwards, this can screw around with the lag of
	 * other tasks.
	 *
	 * EEVDF: placement strategy #1 / #2
	 */
	എങ്കിൽ (sched_feat(PLACE_LAG) && nr_queued && se->vlag) {
		ഘടന sched_entity *curr = cfs_rq->curr;
		long load, weight;

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
		load = cfs_rq->sum_weight;
		എങ്കിൽ (curr && curr->on_rq)
			load += avg_vruntime_weight(cfs_rq, curr->h_load.weight);
		എങ്കിൽ (WARN_ON_ONCE(!load))
			load = 1;

		weight = avg_vruntime_weight(cfs_rq, se->h_load.weight);

		/*
		 * Rewrite, because the product (W + w_i)*vl'_i might
		 * overflow s64:
		 *
		 *   vl_i = (W + w_i)*vl'_i / W
		 *        = (W*vl'_i + w_i*vl'_i) / W
		 *        = vl'_i + w_i*vl'_i/W
		 */
		lag += div64_long(lag * weight, load);

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
		എങ്കിൽ (weight > load)
			update_zero = true;
	}

	se->vruntime = vruntime - lag;

	എങ്കിൽ (update_zero)
		update_zero_vruntime(cfs_rq, -lag);

	എങ്കിൽ (sched_feat(PLACE_REL_DEADLINE) && se->rel_deadline) {
		se->deadline += se->vruntime;
		se->rel_deadline = 0;
		തിരികെ;
	}

	/*
	 * When joining the competition; the existing tasks will be,
	 * on average, halfway through their slice, as such start tasks
	 * off with half a slice to ease into the competition.
	 */
	എങ്കിൽ (sched_feat(PLACE_DEADLINE_INITIAL) && (flags & ENQUEUE_INITIAL))
		vslice /= 2;

	/*
	 * EEVDF: vd_i = ve_i + r_i/w_i
	 */
	se->deadline = se->vruntime + vslice;
}

സ്ഥിതം ശൂന്യം check_enqueue_throttle(ഘടന cfs_rq *cfs_rq);
സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ cfs_rq_throttled(ഘടന cfs_rq *cfs_rq);

സ്ഥിതം ശൂന്യം
enqueue_entity(ഘടന cfs_rq *cfs_rq, ഘടന sched_entity *se, പൂർണ്ണസംഖ്യ flags)
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
	update_load_avg(cfs_rq, se, UPDATE_TG | DO_ATTACH);
	se_update_runnable(se);
	/*
	 * XXX update_load_avg() above will have attached us to the pelt sum;
	 * but update_cfs_group() here will re-adjust the weight and have to
	 * undo/redo all that. Seems wasteful.
	 */
	update_cfs_group(se);

	account_entity_enqueue(cfs_rq, se);

	/* Entity has migrated, no longer consider this task hot */
	എങ്കിൽ (flags & ENQUEUE_MIGRATED)
		se->exec_start = 0;

	check_schedstat_required();
	update_stats_enqueue_fair(cfs_rq, se, flags);
	se->on_rq = 1;

	എങ്കിൽ (cfs_rq->nr_queued == 1) {
		check_enqueue_throttle(cfs_rq);
		list_add_leaf_cfs_rq(cfs_rq);
#ifdef CONFIG_CFS_BANDWIDTH
		എങ്കിൽ (cfs_rq->pelt_clock_throttled) {
			ഘടന rq *rq = rq_of(cfs_rq);

			cfs_rq->throttled_clock_pelt_time += rq_clock_pelt(rq) -
				cfs_rq->throttled_clock_pelt;
			cfs_rq->pelt_clock_throttled = 0;
		}
#endif
	}
}

സ്ഥിതം ശൂന്യം set_next_buddy(ഘടന cfs_rq *cfs_rq, ഘടന sched_entity *se)
{
	എങ്കിൽ (WARN_ON_ONCE(!se->on_rq || se->sched_delayed))
		തിരികെ;
	എങ്കിൽ (se_is_idle(se))
		തിരികെ;
	cfs_rq->next = se;
}

സ്ഥിതം ശൂന്യം clear_buddies(ഘടന cfs_rq *cfs_rq, ഘടന sched_entity *se)
{
	എങ്കിൽ (cfs_rq->next == se)
		cfs_rq->next = ശൂന്യ;
}

സ്ഥിതം __always_inline ശൂന്യം return_cfs_rq_runtime(ഘടന cfs_rq *cfs_rq);

സ്ഥിതം ശൂന്യം set_delayed(ഘടന sched_entity *se)
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
	എങ്കിൽ (!entity_is_task(se)) {
		se->sched_delayed = 1;
		തിരികെ;
	}

	/*
	 * Drop a task leaving the runnable set.
	 * Needs to be called before sched_delayed is set.
	 * clear_delayed() mirrors this after clearing the flag.
	 */
	pref_llc_running_dec(rq_of(cfs_rq_of(se)), task_of(se));
	se->sched_delayed = 1;

	for_each_sched_entity(se) {
		ഘടന cfs_rq *cfs_rq = cfs_rq_of(se);

		cfs_rq->h_nr_runnable--;
	}
}

സ്ഥിതം ശൂന്യം clear_delayed(ഘടന sched_entity *se)
{
	se->sched_delayed = 0;

	/*
	 * Delayed se of cfs_rq have no tasks queued on them.
	 * Do not adjust h_nr_runnable since a dequeue has
	 * already accounted for it or an enqueue of a task
	 * below it will account for it in enqueue_task_fair().
	 */
	എങ്കിൽ (!entity_is_task(se))
		തിരികെ;

	/*
	 * Re-add on wake, after sched_delayed is cleared. On a final delayed
	 * dequeue account_llc_dequeue() already cleared pref_llc_queued, so
	 * this does nothing.
	 */
	pref_llc_running_inc(rq_of(cfs_rq_of(se)), task_of(se));

	for_each_sched_entity(se) {
		ഘടന cfs_rq *cfs_rq = cfs_rq_of(se);

		cfs_rq->h_nr_runnable++;
	}
}

സ്ഥിതം ശൂന്യം
dequeue_entity(ഘടന cfs_rq *cfs_rq, ഘടന sched_entity *se, പൂർണ്ണസംഖ്യ flags)
{
	പൂർണ്ണസംഖ്യ action = UPDATE_TG;

	എങ്കിൽ (entity_is_task(se)) {
		എങ്കിൽ (task_on_rq_migrating(task_of(se)))
			action |= DO_DETACH;

		എങ്കിൽ ((flags & DEQUEUE_SLEEP) && !(flags & DEQUEUE_DELAYED))
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
	update_load_avg(cfs_rq, se, action);
	se_update_runnable(se);

	update_stats_dequeue_fair(cfs_rq, se, flags);

	se->on_rq = 0;
	account_entity_dequeue(cfs_rq, se);

	/* return excess runtime on last dequeue */
	return_cfs_rq_runtime(cfs_rq);

	update_cfs_group(se);

	എങ്കിൽ (cfs_rq->nr_queued == 0) {
		update_idle_cfs_rq_clock_pelt(cfs_rq);
#ifdef CONFIG_CFS_BANDWIDTH
		എങ്കിൽ (throttled_hierarchy(cfs_rq)) {
			ഘടന rq *rq = rq_of(cfs_rq);

			list_del_leaf_cfs_rq(cfs_rq);
			cfs_rq->throttled_clock_pelt = rq_clock_pelt(rq);
			cfs_rq->pelt_clock_throttled = 1;
		}
#endif
	}
}

സ്ഥിതം ശൂന്യം
set_next_entity(ഘടന cfs_rq *cfs_rq, ഘടന sched_entity *se)
{
	/* 'current' is not kept within the tree. */
	എങ്കിൽ (se->on_rq) {
		/*
		 * Any task has to be enqueued before it get to execute on
		 * a CPU. So account for the time it spent waiting on the
		 * runqueue.
		 */
		update_stats_wait_end_fair(cfs_rq, se);
		update_load_avg(cfs_rq, se, UPDATE_TG);
	}

	update_stats_curr_start(cfs_rq, se);
	WARN_ON_ONCE(cfs_rq->h_curr);
	cfs_rq->h_curr = se;

	/*
	 * Track our maximum slice length, if the CPU's load is at
	 * least twice that of our own weight (i.e. don't track it
	 * when there are only lesser-weight tasks around):
	 */
	എങ്കിൽ (schedstat_enabled() &&
	    rq_of(cfs_rq)->cfs.load.weight >= 2*se->load.weight) {
		ഘടന sched_statistics *stats;

		stats = __schedstats_from_se(se);
		__schedstat_set(stats->slice_max,
				max((u64)stats->slice_max,
				    se->sum_exec_runtime - se->prev_sum_exec_runtime));
	}

	se->prev_sum_exec_runtime = se->sum_exec_runtime;
}

സ്ഥിതം bool __dequeue_task(ഘടന rq *rq, ഘടന task_struct *p, പൂർണ്ണസംഖ്യ flags);

സ്ഥിതം ഘടന sched_entity *
pick_next_entity(ഘടന rq *rq, bool protect)
{
	ഘടന cfs_rq *cfs_rq = &rq->cfs;
	ഘടന sched_entity *se;

	se = pick_eevdf(cfs_rq, protect);
	എങ്കിൽ (se->sched_delayed) {
		__dequeue_task(rq, task_of(se), DEQUEUE_SLEEP | DEQUEUE_DELAYED);
		/*
		 * Must not reference @se again, see __block_task().
		 */
		തിരികെ ശൂന്യ;
	}
	തിരികെ se;
}

സ്ഥിതം ശൂന്യം put_prev_entity(ഘടന cfs_rq *cfs_rq, ഘടന sched_entity *prev)
{
	/*
	 * If still on the runqueue then deactivate_task()
	 * was not called and update_curr() has to be done:
	 */
	എങ്കിൽ (prev->on_rq)
		update_curr(cfs_rq);

	എങ്കിൽ (prev->on_rq) {
		update_stats_wait_start_fair(cfs_rq, prev);
		/* in !on_rq case, update occurred at dequeue */
		update_load_avg(cfs_rq, prev, 0);
	}
	WARN_ON_ONCE(cfs_rq->h_curr != prev);
	cfs_rq->h_curr = ശൂന്യ;
}

സ്ഥിതം ശൂന്യം
entity_tick(ഘടന cfs_rq *cfs_rq, ഘടന sched_entity *curr, പൂർണ്ണസംഖ്യ queued)
{
	/*
	 * Update run-time statistics of the 'current'.
	 */
	update_curr(cfs_rq);

	/*
	 * Ensure that runnable average is periodically updated.
	 */
	update_load_avg(cfs_rq, curr, UPDATE_TG);
	update_cfs_group(curr);

#ifdef CONFIG_SCHED_HRTICK
	/*
	 * queued ticks are scheduled to match the slice, so don't bother
	 * validating it and just reschedule.
	 */
	എങ്കിൽ (queued) {
		resched_curr(rq_of(cfs_rq));
		തിരികെ;
	}
#endif
}


/**************************************************
 * CFS bandwidth control machinery
 */

#ifdef CONFIG_CFS_BANDWIDTH

#ifdef CONFIG_JUMP_LABEL
സ്ഥിതം ഘടന static_താക്കോൽ __cfs_bandwidth_used;

സ്ഥിതം വരിയിൽ bool cfs_bandwidth_used(ശൂന്യം)
{
	തിരികെ static_താക്കോൽ_false(&__cfs_bandwidth_used);
}

ശൂന്യം cfs_bandwidth_usage_inc(ശൂന്യം)
{
	static_താക്കോൽ_slow_inc_cpuslocked(&__cfs_bandwidth_used);
}

ശൂന്യം cfs_bandwidth_usage_dec(ശൂന്യം)
{
	static_താക്കോൽ_slow_dec_cpuslocked(&__cfs_bandwidth_used);
}
#അല്ലെങ്കിൽ /* !CONFIG_JUMP_LABEL: */
സ്ഥിതം bool cfs_bandwidth_used(ശൂന്യം)
{
	തിരികെ true;
}

ശൂന്യം cfs_bandwidth_usage_inc(ശൂന്യം) {}
ശൂന്യം cfs_bandwidth_usage_dec(ശൂന്യം) {}
#endif /* !CONFIG_JUMP_LABEL */

സ്ഥിതം വരിയിൽ u64 sched_cfs_bandwidth_slice(ശൂന്യം)
{
	തിരികെ (u64)sysctl_sched_cfs_bandwidth_slice * NSEC_PER_USEC;
}

/*
 * Replenish runtime according to assigned quota. We use sched_clock_cpu
 * directly instead of rq->clock to avoid adding additional synchronization
 * around rq->lock.
 *
 * requires cfs_b->lock
 */
ശൂന്യം __refill_cfs_bandwidth_runtime(ഘടന cfs_bandwidth *cfs_b)
{
	s64 runtime;

	എങ്കിൽ (unlikely(cfs_b->quota == RUNTIME_INF))
		തിരികെ;

	cfs_b->runtime += cfs_b->quota;
	runtime = cfs_b->runtime_snap - cfs_b->runtime;
	എങ്കിൽ (runtime > 0) {
		cfs_b->burst_time += runtime;
		cfs_b->nr_burst++;
	}

	cfs_b->runtime = min(cfs_b->runtime, cfs_b->quota + cfs_b->burst);
	cfs_b->runtime_snap = cfs_b->runtime;
}

സ്ഥിതം വരിയിൽ ഘടന cfs_bandwidth *tg_cfs_bandwidth(ഘടന task_group *tg)
{
	തിരികെ &tg->cfs_bandwidth;
}

/* returns 0 on failure to allocate runtime */
സ്ഥിതം പൂർണ്ണസംഖ്യ __assign_cfs_rq_runtime(ഘടന cfs_bandwidth *cfs_b,
				   ഘടന cfs_rq *cfs_rq, u64 target_runtime)
{
	u64 min_amount, amount = 0;

	lockdep_assert_held(&cfs_b->lock);

	/* note: this is a positive sum as runtime_remaining <= 0 */
	min_amount = target_runtime - cfs_rq->runtime_remaining;

	എങ്കിൽ (cfs_b->quota == RUNTIME_INF)
		amount = min_amount;
	അല്ലെങ്കിൽ {
		start_cfs_bandwidth(cfs_b);

		എങ്കിൽ (cfs_b->runtime > 0) {
			amount = min(cfs_b->runtime, min_amount);
			cfs_b->runtime -= amount;
			cfs_b->idle = 0;
		}
	}

	cfs_rq->runtime_remaining += amount;

	തിരികെ cfs_rq->runtime_remaining > 0;
}

സ്ഥിതം bool throttle_cfs_rq(ഘടന cfs_rq *cfs_rq);

സ്ഥിതം bool __account_cfs_rq_runtime(ഘടന cfs_rq *cfs_rq, u64 delta_exec)
{
	/* dock delta_exec before expiring quota (as it could span periods) */
	cfs_rq->runtime_remaining -= delta_exec;

	എങ്കിൽ (likely(cfs_rq->runtime_remaining > 0))
		തിരികെ false;

	എങ്കിൽ (cfs_rq->throttled)
		തിരികെ true;
	/*
	 * throttle_cfs_rq() will try to extend the runtime first
	 * before throttling the hierarchy.
	 */
	തിരികെ throttle_cfs_rq(cfs_rq);
}

സ്ഥിതം __always_inline
bool account_cfs_rq_runtime(ഘടന cfs_rq *cfs_rq, u64 delta_exec)
{
	എങ്കിൽ (!cfs_bandwidth_used() || !cfs_rq->runtime_enabled)
		തിരികെ false;

	തിരികെ __account_cfs_rq_runtime(cfs_rq, delta_exec);
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ cfs_rq_throttled(ഘടന cfs_rq *cfs_rq)
{
	തിരികെ cfs_bandwidth_used() && cfs_rq->throttled;
}

സ്ഥിതം വരിയിൽ bool cfs_rq_pelt_clock_throttled(ഘടന cfs_rq *cfs_rq)
{
	തിരികെ cfs_bandwidth_used() && cfs_rq->pelt_clock_throttled;
}

/* check whether cfs_rq, or any parent, is throttled */
സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ throttled_hierarchy(ഘടന cfs_rq *cfs_rq)
{
	തിരികെ cfs_bandwidth_used() && cfs_rq->throttle_എണ്ണം;
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ lb_throttled_hierarchy(ഘടന task_struct *p, പൂർണ്ണസംഖ്യ dst_cpu)
{
	തിരികെ throttled_hierarchy(tg_cfs_rq(task_group(p), dst_cpu));
}

സ്ഥിതം വരിയിൽ bool task_is_throttled(ഘടന task_struct *p)
{
	തിരികെ cfs_bandwidth_used() && p->throttled;
}

സ്ഥിതം bool dequeue_task_fair(ഘടന rq *rq, ഘടന task_struct *p, പൂർണ്ണസംഖ്യ flags);
സ്ഥിതം ശൂന്യം throttle_cfs_rq_work(ഘടന callback_head *work)
{
	ഘടന task_struct *p = container_of(work, ഘടന task_struct, sched_throttle_work);
	ഘടന sched_entity *se;
	ഘടന cfs_rq *cfs_rq;
	ഘടന rq *rq;

	WARN_ON_ONCE(p != current);
	p->sched_throttle_work.next = &p->sched_throttle_work;

	/*
	 * If task is exiting, then there won't be a return to userspace, so we
	 * don't have to bother with any of this.
	 */
	എങ്കിൽ ((p->flags & PF_EXITING))
		തിരികെ;

	scoped_guard(task_rq_lock, p) {
		se = &p->se;
		cfs_rq = cfs_rq_of(se);

		/* Raced, forget */
		എങ്കിൽ (p->sched_class != &fair_sched_class)
			തിരികെ;

		/*
		 * If not in limbo, then either replenish has happened or this
		 * task got migrated out of the throttled cfs_rq, move along.
		 */
		എങ്കിൽ (!cfs_rq->throttle_എണ്ണം)
			തിരികെ;
		rq = scope.rq;
		update_rq_clock(rq);
		WARN_ON_ONCE(p->throttled || !list_empty(&p->throttle_കണ്ണി));
		dequeue_task_fair(rq, p, DEQUEUE_SLEEP | DEQUEUE_THROTTLE);
		list_add(&p->throttle_കണ്ണി, &cfs_rq->throttled_limbo_list);
		/*
		 * Must not set throttled before dequeue or dequeue will
		 * mistakenly regard this task as an already throttled one.
		 */
		p->throttled = true;
		resched_curr(rq);
	}
}

ശൂന്യം init_cfs_throttle_work(ഘടന task_struct *p)
{
	init_task_work(&p->sched_throttle_work, throttle_cfs_rq_work);
	/* Protect against double add, see throttle_cfs_rq() and throttle_cfs_rq_work() */
	p->sched_throttle_work.next = &p->sched_throttle_work;
	INIT_LIST_HEAD(&p->throttle_കണ്ണി);
}

/*
 * Task is throttled and someone wants to dequeue it again:
 * it could be sched/core when core needs to do things like
 * task affinity change, task group change, task sched class
 * change etc. and in these cases, DEQUEUE_SLEEP is not set;
 * or the task is blocked after throttled due to freezer etc.
 * and in these cases, DEQUEUE_SLEEP is set.
 */
സ്ഥിതം ശൂന്യം detach_task_cfs_rq(ഘടന task_struct *p);
സ്ഥിതം ശൂന്യം dequeue_throttled_task(ഘടന task_struct *p, പൂർണ്ണസംഖ്യ flags)
{
	WARN_ON_ONCE(p->se.on_rq);
	list_del_init(&p->throttle_കണ്ണി);

	/* task blocked after throttled */
	എങ്കിൽ (flags & DEQUEUE_SLEEP) {
		p->throttled = false;
		തിരികെ;
	}

	/*
	 * task is migrating off its old cfs_rq, detach
	 * the task's load from its old cfs_rq.
	 */
	എങ്കിൽ (task_on_rq_migrating(p))
		detach_task_cfs_rq(p);
}

സ്ഥിതം bool enqueue_throttled_task(ഘടന task_struct *p)
{
	ഘടന cfs_rq *cfs_rq = cfs_rq_of(&p->se);

	/* @p should have gone through dequeue_throttled_task() first */
	WARN_ON_ONCE(!list_empty(&p->throttle_കണ്ണി));

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
	എങ്കിൽ (throttled_hierarchy(cfs_rq) &&
	    !task_current_donor(rq_of(cfs_rq), p)) {
		list_add(&p->throttle_കണ്ണി, &cfs_rq->throttled_limbo_list);
		തിരികെ true;
	}

	/* we can't take the fast path, do an actual enqueue*/
	p->throttled = false;
	തിരികെ false;
}

സ്ഥിതം ശൂന്യം enqueue_task_fair(ഘടന rq *rq, ഘടന task_struct *p, പൂർണ്ണസംഖ്യ flags);
സ്ഥിതം പൂർണ്ണസംഖ്യ tg_unthrottle_up(ഘടന task_group *tg, ശൂന്യം *data)
{
	ഘടന rq *rq = data;
	ഘടന cfs_rq *cfs_rq = tg_cfs_rq(tg, cpu_of(rq));
	ഘടന task_struct *p, *tmp;
	LIST_HEAD(throttled_tasks);

	/*
	 * If cfs_rq->curr is set, the cfs_rq might not have caught up
	 * since the last clock update. Do it now before we begin
	 * queueing task onto it to save the need for unnecessarily
	 * unthrottle the hierarchy for this cfs_rq to be throttled
	 * right back again.
	 */
	update_curr(cfs_rq);

	എങ്കിൽ (--cfs_rq->throttle_എണ്ണം)
		തിരികെ 0;

	എങ്കിൽ (cfs_rq->pelt_clock_throttled) {
		cfs_rq->throttled_clock_pelt_time += rq_clock_pelt(rq) -
					     cfs_rq->throttled_clock_pelt;
		cfs_rq->pelt_clock_throttled = 0;
	}

	എങ്കിൽ (cfs_rq->throttled_clock_self) {
		u64 delta = rq_clock(rq) - cfs_rq->throttled_clock_self;

		cfs_rq->throttled_clock_self = 0;

		എങ്കിൽ (WARN_ON_ONCE((s64)delta < 0))
			delta = 0;

		cfs_rq->throttled_clock_self_time += delta;
	}

	/*
	 * Move the tasks to a local list since an update_curr() during
	 * enqueue_task_fair() can throttle a higher cfs_rq, and it can
	 * see the "throttled_limbo_list" being non-empty in
	 * tg_throttle_down() if throttle_count turned 0 above.
	 */
	list_splice_init(&cfs_rq->throttled_limbo_list, &throttled_tasks);

	/* Re-enqueue the tasks that have been throttled at this level. */
	list_for_each_entry_safe(p, tmp, &throttled_tasks, throttle_കണ്ണി) {
		/*
		 * Back to being throttled! Break out and put the remaining
		 * tasks back onto the limbo_list to prevent running them
		 * unnecessarily.
		 */
		എങ്കിൽ (cfs_rq->throttle_എണ്ണം)
			നിർത്തുക;

		list_del_init(&p->throttle_കണ്ണി);
		p->throttled = false;
		enqueue_task_fair(rq, p, ENQUEUE_WAKEUP);
	}

	list_splice(&throttled_tasks, &cfs_rq->throttled_limbo_list);

	/* Add cfs_rq with load or one or more already running entities to the list */
	എങ്കിൽ (!cfs_rq_is_decayed(cfs_rq))
		list_add_leaf_cfs_rq(cfs_rq);

	തിരികെ 0;
}

സ്ഥിതം വരിയിൽ bool task_has_throttle_work(ഘടന task_struct *p)
{
	തിരികെ p->sched_throttle_work.next != &p->sched_throttle_work;
}

സ്ഥിതം വരിയിൽ ശൂന്യം task_throttle_setup_work(ഘടന task_struct *p)
{
	എങ്കിൽ (task_has_throttle_work(p))
		തിരികെ;

	/*
	 * Kthreads and exiting tasks don't return to userspace, so adding the
	 * work is pointless
	 */
	എങ്കിൽ ((p->flags & (PF_EXITING | PF_KTHREAD)))
		തിരികെ;

	task_work_add(p, &p->sched_throttle_work, TWA_RESUME);
}

സ്ഥിതം ശൂന്യം record_throttle_clock(ഘടന cfs_rq *cfs_rq)
{
	ഘടന rq *rq = rq_of(cfs_rq);

	എങ്കിൽ (cfs_rq_throttled(cfs_rq) && !cfs_rq->throttled_clock)
		cfs_rq->throttled_clock = rq_clock(rq);

	എങ്കിൽ (!cfs_rq->throttled_clock_self)
		cfs_rq->throttled_clock_self = rq_clock(rq);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ tg_throttle_down(ഘടന task_group *tg, ശൂന്യം *data)
{
	ഘടന rq *rq = data;
	ഘടന cfs_rq *cfs_rq = tg_cfs_rq(tg, cpu_of(rq));

	എങ്കിൽ (cfs_rq->throttle_എണ്ണം++)
		തിരികെ 0;

	/*
	 * For cfs_rqs that still have entities enqueued, PELT clock
	 * stop happens at dequeue time when all entities are dequeued.
	 */
	എങ്കിൽ (!cfs_rq->nr_queued) {
		list_del_leaf_cfs_rq(cfs_rq);
		cfs_rq->throttled_clock_pelt = rq_clock_pelt(rq);
		cfs_rq->pelt_clock_throttled = 1;
	}

	WARN_ON_ONCE(cfs_rq->throttled_clock_self);
	WARN_ON_ONCE(!list_empty(&cfs_rq->throttled_limbo_list));
	തിരികെ 0;
}

സ്ഥിതം bool throttle_cfs_rq(ഘടന cfs_rq *cfs_rq)
{
	ഘടന cfs_bandwidth *cfs_b = tg_cfs_bandwidth(cfs_rq->tg);
	ഘടന sched_entity *curr = cfs_rq->h_curr;
	ഘടന rq *rq = rq_of(cfs_rq);

	scoped_guard(raw_spinlock, &cfs_b->lock) {
		u64 target_runtime = 1;

		/*
		 * If cfs_rq->h_curr is still runnable, we are here from an
		 * update_curr(). Request sysctl_sched_cfs_bandwidth_slice
		 * worth of bandwidth to continue running.
		 *
		 * If the curr is not runnable, just request enough bandwidth
		 * to be runnable next time the pick selects this cfs_rq.
		 */
		എങ്കിൽ (curr && curr->on_rq)
			target_runtime = sched_cfs_bandwidth_slice();

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
		എങ്കിൽ (__assign_cfs_rq_runtime(cfs_b, cfs_rq, target_runtime))
			തിരികെ false;

		/*
		 * No bandwidth available; Add ourselves on the list to be
		 * unthrottled later.
		 */
		list_add_tail_rcu(&cfs_rq->throttled_list,
				  &cfs_b->throttled_cfs_rq);
	}

	/* freeze hierarchy runnable averages while throttled */
	scoped_guard(rcu)
		walk_tg_വൃക്ഷം_from(cfs_rq->tg, tg_throttle_down, tg_nop, (ശൂന്യം *)rq);

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
	എങ്കിൽ (curr && curr->on_rq)
		task_throttle_setup_work(rq->donor);

	തിരികെ true;
}

ശൂന്യം unthrottle_cfs_rq(ഘടന cfs_rq *cfs_rq)
{
	ഘടന rq *rq = rq_of(cfs_rq);
	ഘടന cfs_bandwidth *cfs_b = tg_cfs_bandwidth(cfs_rq->tg);
	ഘടന sched_entity *se = cfs_rq_se(cfs_rq);

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
	എങ്കിൽ (cfs_rq->runtime_enabled && cfs_rq->runtime_remaining <= 0)
		തിരികെ;

	cfs_rq->throttled = 0;

	scoped_guard(raw_spinlock, &cfs_b->lock) {
		list_del_rcu(&cfs_rq->throttled_list);

		എങ്കിൽ (!cfs_rq->throttled_clock)
			നിർത്തുക;

		cfs_b->throttled_time += rq_clock(rq) - cfs_rq->throttled_clock;
		cfs_rq->throttled_clock = 0;
	}

	/* update hierarchical throttle state */
	walk_tg_വൃക്ഷം_from(cfs_rq->tg, tg_nop, tg_unthrottle_up, (ശൂന്യം *)rq);

	എങ്കിൽ (!cfs_rq->load.weight) {
		എങ്കിൽ (!cfs_rq->on_list)
			തിരികെ;
		/*
		 * Nothing to run but something to decay (on_list)?
		 * Complete the branch.
		 */
		for_each_sched_entity(se) {
			എങ്കിൽ (list_add_leaf_cfs_rq(cfs_rq_of(se)))
				നിർത്തുക;
		}
	}

	assert_list_leaf_cfs_rq(rq);

	/* Determine whether we need to wake up potentially idle CPU: */
	എങ്കിൽ (rq->curr == rq->idle && rq->cfs.h_nr_queued)
		resched_curr(rq);
}

സ്ഥിതം ശൂന്യം __cfsb_csd_unthrottle(ശൂന്യം *arg)
{
	ഘടന cfs_rq *cursor, *tmp;
	ഘടന rq *rq = arg;

	guard(rq_lock)(rq);

	/*
	 * Iterating over the list can trigger several call to
	 * update_rq_clock() in unthrottle_cfs_rq().
	 * Do it once and skip the potential next ones.
	 */
	update_rq_clock(rq);
	rq_clock_start_loop_update(rq);

	/*
	 * Since we hold rq lock we're safe from concurrent manipulation of
	 * the CSD list. However, this RCU critical section annotates the
	 * fact that we pair with sched_free_group_rcu(), so that we cannot
	 * race with group being freed in the window between removing it
	 * from the list and advancing to the next entry in the list.
	 */
	guard(rcu)();

	list_for_each_entry_safe(cursor, tmp, &rq->cfsb_csd_list,
				 throttled_csd_list) {
		list_del_init(&cursor->throttled_csd_list);

		എങ്കിൽ (cfs_rq_throttled(cursor))
			unthrottle_cfs_rq(cursor);
	}

	rq_clock_stop_loop_update(rq);
}

സ്ഥിതം വരിയിൽ ശൂന്യം __unthrottle_cfs_rq_async(ഘടന cfs_rq *cfs_rq)
{
	ഘടന rq *rq = rq_of(cfs_rq);
	bool first;

	എങ്കിൽ (rq == this_rq()) {
		update_rq_clock(rq);
		unthrottle_cfs_rq(cfs_rq);
		തിരികെ;
	}

	/* Already enqueued */
	എങ്കിൽ (WARN_ON_ONCE(!list_empty(&cfs_rq->throttled_csd_list)))
		തിരികെ;

	first = list_empty(&rq->cfsb_csd_list);
	list_add_tail(&cfs_rq->throttled_csd_list, &rq->cfsb_csd_list);
	എങ്കിൽ (first)
		smp_call_function_single_async(cpu_of(rq), &rq->cfsb_csd);
}

സ്ഥിതം ശൂന്യം unthrottle_cfs_rq_async(ഘടന cfs_rq *cfs_rq)
{
	lockdep_assert_rq_held(rq_of(cfs_rq));

	എങ്കിൽ (WARN_ON_ONCE(!cfs_rq_throttled(cfs_rq) ||
	    cfs_rq->runtime_remaining <= 0))
		തിരികെ;

	__unthrottle_cfs_rq_async(cfs_rq);
}

സ്ഥിതം bool distribute_cfs_runtime(ഘടന cfs_bandwidth *cfs_b)
{
	bool throttled = false, unthrottle_local = false;
	പൂർണ്ണസംഖ്യ this_cpu = smp_processor_id();
	u64 runtime, remaining = 1;
	ഘടന cfs_rq *cfs_rq;
	ഘടന rq *rq;

	guard(rcu)();

	list_for_each_entry_rcu(cfs_rq, &cfs_b->throttled_cfs_rq,
				throttled_list) {
		rq = rq_of(cfs_rq);

		എങ്കിൽ (!remaining) {
			throttled = true;
			നിർത്തുക;
		}

		guard(rq_lock_irqsave)(rq);

		എങ്കിൽ (!cfs_rq_throttled(cfs_rq))
			തുടരുക;

		/* Already queued for async unthrottle */
		എങ്കിൽ (!list_empty(&cfs_rq->throttled_csd_list))
			തുടരുക;

		എങ്കിൽ (cfs_rq->h_curr) {
			update_rq_clock(rq);
			update_curr(cfs_rq);
		}

		/* By the above checks, this should never be true */
		WARN_ON_ONCE(cfs_rq->runtime_remaining > 0);

		scoped_guard(raw_spinlock, &cfs_b->lock) {
			runtime = -cfs_rq->runtime_remaining + 1;
			എങ്കിൽ (runtime > cfs_b->runtime)
				runtime = cfs_b->runtime;
			cfs_b->runtime -= runtime;
			remaining = cfs_b->runtime;
		}

		cfs_rq->runtime_remaining += runtime;

		/*
		 * Ran out of bandwidth during distribution!
		 * Indicate throttled entities and break early.
		 */
		എങ്കിൽ (cfs_rq->runtime_remaining <= 0) {
			throttled = true;
			നിർത്തുക;
		}

		/* we check whether we're throttled above */
		എങ്കിൽ (cpu_of(rq) != this_cpu) {
			unthrottle_cfs_rq_async(cfs_rq);
			തുടരുക;
		}

		/*
		 * Allow a parallel async unthrottle to unthrottle
		 * this cfs_rq too via __cfsb_csd_unthrottle().
		 * If we are first, do it ourselves at the end and
		 * save on an IPI from remote CPUs.
		 */
		unthrottle_local = list_empty(&rq->cfsb_csd_list);
		list_add_tail(&cfs_rq->throttled_csd_list, &rq->cfsb_csd_list);
	}

	എങ്കിൽ (unthrottle_local) {
		/*
		 * Protect against an IPI that is also trying to flush
		 * the unthrottled cfs_rq(s) from this CPU's csd_list.
		 */
		scoped_guard(irqsave)
			__cfsb_csd_unthrottle(cpu_rq(this_cpu));
	}

	തിരികെ throttled;
}

/*
 * Responsible for refilling a task_group's bandwidth and unthrottling its
 * cfs_rqs as appropriate. If there has been no activity within the last
 * period the timer is deactivated until scheduling resumes; cfs_b->idle is
 * used to track this state.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ do_sched_cfs_period_timer(ഘടന cfs_bandwidth *cfs_b, പൂർണ്ണസംഖ്യ overrun, unsigned long flags)
	__must_hold(&cfs_b->lock)
{
	പൂർണ്ണസംഖ്യ throttled;

	/* no need to continue the timer with no bandwidth constraint */
	എങ്കിൽ (cfs_b->quota == RUNTIME_INF)
		പോകുക out_deactivate;

	throttled = !list_empty(&cfs_b->throttled_cfs_rq);
	cfs_b->nr_periods += overrun;

	/* Refill extra burst quota even if cfs_b->idle */
	__refill_cfs_bandwidth_runtime(cfs_b);

	/*
	 * idle depends on !throttled (for the case of a large deficit), and if
	 * we're going inactive then everything else can be deferred
	 */
	എങ്കിൽ (cfs_b->idle && !throttled)
		പോകുക out_deactivate;

	എങ്കിൽ (!throttled) {
		/* mark as potentially idle for the upcoming period */
		cfs_b->idle = 1;
		തിരികെ 0;
	}

	/* account preceding periods in which throttling occurred */
	cfs_b->nr_throttled += overrun;

	/*
	 * This check is repeated as we release cfs_b->lock while we unthrottle.
	 */
	ആകുമ്പോൾ (throttled && cfs_b->runtime > 0) {
		raw_spin_unlock_irqrestore(&cfs_b->lock, flags);
		/* we can't nest cfs_b->lock while distributing bandwidth */
		throttled = distribute_cfs_runtime(cfs_b);
		raw_spin_lock_irqsave(&cfs_b->lock, flags);
	}

	/*
	 * While we are ensured activity in the period following an
	 * unthrottle, this also covers the case in which the new bandwidth is
	 * insufficient to cover the existing bandwidth deficit.  (Forcing the
	 * timer to remain active while there are any throttled entities.)
	 */
	cfs_b->idle = 0;

	തിരികെ 0;

out_deactivate:
	തിരികെ 1;
}

/* a cfs_rq won't donate quota below this amount */
സ്ഥിതം സ്ഥിരം u64 min_cfs_rq_runtime = 1 * NSEC_PER_MSEC;
/* minimum remaining period time to redistribute slack quota */
സ്ഥിതം സ്ഥിരം u64 min_bandwidth_expiration = 2 * NSEC_PER_MSEC;
/* how long we wait to gather additional slack before distributing */
സ്ഥിതം സ്ഥിരം u64 cfs_bandwidth_slack_period = 5 * NSEC_PER_MSEC;

/*
 * Are we near the end of the current quota period?
 *
 * Requires cfs_b->lock for hrtimer_expires_remaining to be safe against the
 * hrtimer base being cleared by hrtimer_start. In the case of
 * migrate_hrtimers, base is never cleared, so we are fine.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ runtime_refresh_within(ഘടന cfs_bandwidth *cfs_b, u64 min_expire)
{
	ഘടന hrtimer *refresh_timer = &cfs_b->period_timer;
	s64 remaining;

	/* if the call-back is running a quota refresh is already occurring */
	എങ്കിൽ (hrtimer_callback_running(refresh_timer))
		തിരികെ 1;

	/* is a quota refresh about to occur? */
	remaining = ktime_to_ns(hrtimer_expires_remaining(refresh_timer));
	എങ്കിൽ (remaining < (s64)min_expire)
		തിരികെ 1;

	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം start_cfs_slack_bandwidth(ഘടന cfs_bandwidth *cfs_b)
{
	u64 min_ഇടത് = cfs_bandwidth_slack_period + min_bandwidth_expiration;

	/* if there's a quota refresh soon don't bother with slack */
	എങ്കിൽ (runtime_refresh_within(cfs_b, min_ഇടത്))
		തിരികെ;

	/* don't push forwards an existing deferred unthrottle */
	എങ്കിൽ (cfs_b->slack_started)
		തിരികെ;
	cfs_b->slack_started = true;

	hrtimer_start(&cfs_b->slack_timer,
			ns_to_ktime(cfs_bandwidth_slack_period),
			HRTIMER_MODE_REL);
}

/* we know any runtime found here is valid as update_curr() precedes return */
സ്ഥിതം ശൂന്യം __return_cfs_rq_runtime(ഘടന cfs_rq *cfs_rq)
{
	ഘടന cfs_bandwidth *cfs_b = tg_cfs_bandwidth(cfs_rq->tg);
	s64 slack_runtime = cfs_rq->runtime_remaining - min_cfs_rq_runtime;

	എങ്കിൽ (slack_runtime <= 0)
		തിരികെ;

	guard(raw_spinlock)(&cfs_b->lock);

	എങ്കിൽ (cfs_b->quota != RUNTIME_INF) {
		cfs_b->runtime += slack_runtime;

		/* we are under rq->lock, defer unthrottling using a timer */
		എങ്കിൽ (cfs_b->runtime > sched_cfs_bandwidth_slice() &&
		    !list_empty(&cfs_b->throttled_cfs_rq))
			start_cfs_slack_bandwidth(cfs_b);
	}

	/* even if it's not valid for return we don't want to try again */
	cfs_rq->runtime_remaining -= slack_runtime;
}

സ്ഥിതം __always_inline ശൂന്യം return_cfs_rq_runtime(ഘടന cfs_rq *cfs_rq)
{
	എങ്കിൽ (!cfs_bandwidth_used())
		തിരികെ;

	എങ്കിൽ (!cfs_rq->runtime_enabled || cfs_rq->nr_queued)
		തിരികെ;

	__return_cfs_rq_runtime(cfs_rq);
}

/*
 * This is done with a timer (instead of inline with bandwidth return) since
 * it's necessary to juggle rq->locks to unthrottle their respective cfs_rqs.
 */
സ്ഥിതം ശൂന്യം do_sched_cfs_slack_timer(ഘടന cfs_bandwidth *cfs_b)
{
	/* confirm we're still not at a refresh boundary */
	scoped_guard(raw_spinlock_irqsave, &cfs_b->lock) {
		u64 runtime = 0, slice = sched_cfs_bandwidth_slice();

		cfs_b->slack_started = false;

		എങ്കിൽ (runtime_refresh_within(cfs_b, min_bandwidth_expiration))
			തിരികെ;

		എങ്കിൽ (cfs_b->quota != RUNTIME_INF && cfs_b->runtime > slice)
			runtime = cfs_b->runtime;

		എങ്കിൽ (!runtime)
			തിരികെ;
	}

	distribute_cfs_runtime(cfs_b);
}

/*
 * When a group wakes up we want to make sure that its quota is not already
 * expired/exceeded, otherwise it may be allowed to steal additional ticks of
 * runtime as update_curr() throttling can not trigger until it's on-rq.
 */
സ്ഥിതം ശൂന്യം check_enqueue_throttle(ഘടന cfs_rq *cfs_rq)
{
	എങ്കിൽ (!cfs_bandwidth_used())
		തിരികെ;

	/* an active group must be handled by the update_curr() path */
	എങ്കിൽ (!cfs_rq->runtime_enabled || cfs_rq->h_curr)
		തിരികെ;

	/* ensure the group is not already throttled */
	എങ്കിൽ (cfs_rq_throttled(cfs_rq))
		തിരികെ;

	/* update runtime allocation */
	account_cfs_rq_runtime(cfs_rq, 0);
}

സ്ഥിതം ശൂന്യം sync_throttle(ഘടന task_group *tg, പൂർണ്ണസംഖ്യ cpu)
{
	ഘടന cfs_rq *pcfs_rq, *cfs_rq;

	എങ്കിൽ (!cfs_bandwidth_used())
		തിരികെ;

	എങ്കിൽ (!tg->parent)
		തിരികെ;

	cfs_rq = tg_cfs_rq(tg, cpu);
	pcfs_rq = tg_cfs_rq(tg->parent, cpu);

	cfs_rq->throttle_എണ്ണം = pcfs_rq->throttle_എണ്ണം;
	cfs_rq->throttled_clock_pelt = rq_clock_pelt(cpu_rq(cpu));

	/*
	 * It is not enough to sync the "pelt_clock_throttled" indicator
	 * with the parent cfs_rq when the hierarchy is not queued.
	 * Always join a throttled hierarchy with PELT clock throttled
	 * and leaf it to the first enqueue, or distribution to
	 * unthrottle the PELT clock.
	 */
	എങ്കിൽ (cfs_rq->throttle_എണ്ണം)
		cfs_rq->pelt_clock_throttled = 1;
}

സ്ഥിതം എണ്ണൽ hrtimer_restart sched_cfs_slack_timer(ഘടന hrtimer *timer)
{
	ഘടന cfs_bandwidth *cfs_b =
		container_of(timer, ഘടന cfs_bandwidth, slack_timer);

	do_sched_cfs_slack_timer(cfs_b);

	തിരികെ HRTIMER_NORESTART;
}

സ്ഥിതം എണ്ണൽ hrtimer_restart sched_cfs_period_timer(ഘടന hrtimer *timer)
{
	ഘടന cfs_bandwidth *cfs_b =
		container_of(timer, ഘടന cfs_bandwidth, period_timer);
	പൂർണ്ണസംഖ്യ overrun;
	പൂർണ്ണസംഖ്യ idle = 0;
	പൂർണ്ണസംഖ്യ എണ്ണം = 0;

	CLASS(raw_spinlock_irqsave, cfsb_guard)(&cfs_b->lock);

	വേണ്ടി (;;) {
		overrun = hrtimer_forward_now(timer, cfs_b->period);
		എങ്കിൽ (!overrun)
			നിർത്തുക;

		idle = do_sched_cfs_period_timer(cfs_b, overrun, cfsb_guard.flags);

		എങ്കിൽ (++എണ്ണം > 3) {
			u64 new, old = ktime_to_ns(cfs_b->period);

			/*
			 * Grow period by a factor of 2 to avoid losing precision.
			 * Precision loss in the quota/period ratio can cause __cfs_schedulable
			 * to fail.
			 */
			new = old * 2;
			എങ്കിൽ (new < max_bw_quota_period_us * NSEC_PER_USEC) {
				cfs_b->period = ns_to_ktime(new);
				cfs_b->quota *= 2;
				cfs_b->burst *= 2;

				pr_warn_ratelimited(
	"cfs_period_timer[cpu%d]: period too short, scaling up (new cfs_period_us = %lld, cfs_quota_us = %lld)\n",
					smp_processor_id(),
					div_u64(new, NSEC_PER_USEC),
					div_u64(cfs_b->quota, NSEC_PER_USEC));
			} അല്ലെങ്കിൽ {
				pr_warn_ratelimited(
	"cfs_period_timer[cpu%d]: period too short, but cannot scale up without losing precision (cfs_period_us = %lld, cfs_quota_us = %lld)\n",
					smp_processor_id(),
					div_u64(old, NSEC_PER_USEC),
					div_u64(cfs_b->quota, NSEC_PER_USEC));
			}

			/* reset count so we don't come right back in here */
			എണ്ണം = 0;
		}
	}

	എങ്കിൽ (idle) {
		cfs_b->period_active = 0;
		തിരികെ HRTIMER_NORESTART;
	}

	തിരികെ HRTIMER_RESTART;
}

ശൂന്യം init_cfs_bandwidth(ഘടന cfs_bandwidth *cfs_b, ഘടന cfs_bandwidth *parent)
{
	raw_spin_lock_init(&cfs_b->lock);
	cfs_b->runtime = 0;
	cfs_b->quota = RUNTIME_INF;
	cfs_b->period = us_to_ktime(default_bw_period_us());
	cfs_b->burst = 0;
	cfs_b->hierarchical_quota = parent ? parent->hierarchical_quota : RUNTIME_INF;

	INIT_LIST_HEAD(&cfs_b->throttled_cfs_rq);
	hrtimer_setup(&cfs_b->period_timer, sched_cfs_period_timer, CLOCK_MONOTONIC,
		      HRTIMER_MODE_ABS_PINNED);

	/* Add a random offset so that timers interleave */
	hrtimer_set_expires(&cfs_b->period_timer,
			    get_random_u32_below(cfs_b->period));
	hrtimer_setup(&cfs_b->slack_timer, sched_cfs_slack_timer, CLOCK_MONOTONIC,
		      HRTIMER_MODE_REL);
	cfs_b->slack_started = false;
}

സ്ഥിതം ശൂന്യം init_cfs_rq_runtime(ഘടന cfs_rq *cfs_rq)
{
	cfs_rq->runtime_enabled = 0;
	INIT_LIST_HEAD(&cfs_rq->throttled_list);
	INIT_LIST_HEAD(&cfs_rq->throttled_csd_list);
	INIT_LIST_HEAD(&cfs_rq->throttled_limbo_list);
}

ശൂന്യം start_cfs_bandwidth(ഘടന cfs_bandwidth *cfs_b)
{
	lockdep_assert_held(&cfs_b->lock);

	എങ്കിൽ (cfs_b->period_active)
		തിരികെ;

	cfs_b->period_active = 1;
	hrtimer_forward_now(&cfs_b->period_timer, cfs_b->period);
	hrtimer_start_expires(&cfs_b->period_timer, HRTIMER_MODE_ABS_PINNED);
}

സ്ഥിതം ശൂന്യം destroy_cfs_bandwidth(ഘടന cfs_bandwidth *cfs_b)
{
	പൂർണ്ണസംഖ്യ __maybe_unused i;

	/* init_cfs_bandwidth() was not called */
	എങ്കിൽ (!cfs_b->throttled_cfs_rq.next)
		തിരികെ;

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
		ഘടന rq *rq = cpu_rq(i);

		എങ്കിൽ (list_empty(&rq->cfsb_csd_list))
			തുടരുക;

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
സ്ഥിതം ശൂന്യം __maybe_unused update_runtime_enabled(ഘടന rq *rq)
{
	ഘടന task_group *tg;

	lockdep_assert_rq_held(rq);

	guard(rcu)();

	list_for_each_entry_rcu(tg, &task_groups, list) {
		ഘടന cfs_bandwidth *cfs_b = &tg->cfs_bandwidth;
		ഘടന cfs_rq *cfs_rq = tg_cfs_rq(tg, cpu_of(rq));

		scoped_guard(raw_spinlock, &cfs_b->lock)
			cfs_rq->runtime_enabled = cfs_b->quota != RUNTIME_INF;
	}
}

/* cpu offline callback */
സ്ഥിതം ശൂന്യം __maybe_unused unthrottle_offline_cfs_rqs(ഘടന rq *rq)
{
	ഘടന task_group *tg;

	lockdep_assert_rq_held(rq);

	// Do not unthrottle for an active CPU
	എങ്കിൽ (cpumask_test_cpu(cpu_of(rq), cpu_active_mask))
		തിരികെ;

	/*
	 * The rq clock has already been updated in the
	 * set_rq_offline(), so we should skip updating
	 * the rq clock again in unthrottle_cfs_rq().
	 */
	rq_clock_start_loop_update(rq);

	guard(rcu)();

	list_for_each_entry_rcu(tg, &task_groups, list) {
		ഘടന cfs_rq *cfs_rq = tg_cfs_rq(tg, cpu_of(rq));

		എങ്കിൽ (!cfs_rq->runtime_enabled)
			തുടരുക;

		/*
		 * Offline rq is schedulable till CPU is completely disabled
		 * in take_cpu_down(), so we prevent new cfs throttling here.
		 */
		cfs_rq->runtime_enabled = 0;

		എങ്കിൽ (!cfs_rq_throttled(cfs_rq))
			തുടരുക;

		/*
		 * clock_task is not advancing so we just need to make sure
		 * there's some valid quota amount
		 */
		cfs_rq->runtime_remaining = 1;
		unthrottle_cfs_rq(cfs_rq);
	}

	rq_clock_stop_loop_update(rq);
}

bool cfs_task_bw_constrained(ഘടന task_struct *p)
{
	ഘടന cfs_rq *cfs_rq = task_cfs_rq(p);

	എങ്കിൽ (!cfs_bandwidth_used())
		തിരികെ false;

	എങ്കിൽ (cfs_rq->runtime_enabled ||
	    tg_cfs_bandwidth(cfs_rq->tg)->hierarchical_quota != RUNTIME_INF)
		തിരികെ true;

	തിരികെ false;
}

#ifdef CONFIG_NO_HZ_FULL
/* called from pick_next_task_fair() */
സ്ഥിതം ശൂന്യം sched_fair_update_stop_tick(ഘടന rq *rq, ഘടന task_struct *p)
{
	പൂർണ്ണസംഖ്യ cpu = cpu_of(rq);

	എങ്കിൽ (!cfs_bandwidth_used())
		തിരികെ;

	എങ്കിൽ (!tick_nohz_full_cpu(cpu))
		തിരികെ;

	എങ്കിൽ (rq->nr_running != 1)
		തിരികെ;

	/*
	 *  We know there is only one task runnable and we've just picked it. The
	 *  normal enqueue path will have cleared TICK_DEP_BIT_SCHED if we will
	 *  be otherwise able to stop the tick. Just need to check if we are using
	 *  bandwidth control.
	 */
	എങ്കിൽ (cfs_task_bw_constrained(p))
		tick_nohz_dep_set_cpu(cpu, TICK_DEP_BIT_SCHED);
}
#endif /* CONFIG_NO_HZ_FULL */

#അല്ലെങ്കിൽ /* !CONFIG_CFS_BANDWIDTH: */

സ്ഥിതം bool account_cfs_rq_runtime(ഘടന cfs_rq *cfs_rq, u64 delta_exec) { തിരികെ false; }
സ്ഥിതം ശൂന്യം check_enqueue_throttle(ഘടന cfs_rq *cfs_rq) {}
സ്ഥിതം വരിയിൽ ശൂന്യം sync_throttle(ഘടന task_group *tg, പൂർണ്ണസംഖ്യ cpu) {}
സ്ഥിതം __always_inline ശൂന്യം return_cfs_rq_runtime(ഘടന cfs_rq *cfs_rq) {}
സ്ഥിതം ശൂന്യം task_throttle_setup_work(ഘടന task_struct *p) {}
സ്ഥിതം bool task_is_throttled(ഘടന task_struct *p) { തിരികെ false; }
സ്ഥിതം ശൂന്യം dequeue_throttled_task(ഘടന task_struct *p, പൂർണ്ണസംഖ്യ flags) {}
സ്ഥിതം bool enqueue_throttled_task(ഘടന task_struct *p) { തിരികെ false; }
സ്ഥിതം ശൂന്യം record_throttle_clock(ഘടന cfs_rq *cfs_rq) {}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ cfs_rq_throttled(ഘടന cfs_rq *cfs_rq)
{
	തിരികെ 0;
}

സ്ഥിതം വരിയിൽ bool cfs_rq_pelt_clock_throttled(ഘടന cfs_rq *cfs_rq)
{
	തിരികെ false;
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ throttled_hierarchy(ഘടന cfs_rq *cfs_rq)
{
	തിരികെ 0;
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ lb_throttled_hierarchy(ഘടന task_struct *p, പൂർണ്ണസംഖ്യ dst_cpu)
{
	തിരികെ 0;
}

#ifdef CONFIG_FAIR_GROUP_SCHED
ശൂന്യം init_cfs_bandwidth(ഘടന cfs_bandwidth *cfs_b, ഘടന cfs_bandwidth *parent) {}
സ്ഥിതം ശൂന്യം init_cfs_rq_runtime(ഘടന cfs_rq *cfs_rq) {}
#endif

സ്ഥിതം വരിയിൽ ഘടന cfs_bandwidth *tg_cfs_bandwidth(ഘടന task_group *tg)
{
	തിരികെ ശൂന്യ;
}
സ്ഥിതം വരിയിൽ ശൂന്യം destroy_cfs_bandwidth(ഘടന cfs_bandwidth *cfs_b) {}
സ്ഥിതം വരിയിൽ ശൂന്യം update_runtime_enabled(ഘടന rq *rq) {}
സ്ഥിതം വരിയിൽ ശൂന്യം unthrottle_offline_cfs_rqs(ഘടന rq *rq) {}
#ifdef CONFIG_CGROUP_SCHED
bool cfs_task_bw_constrained(ഘടന task_struct *p)
{
	തിരികെ false;
}
#endif
#endif /* !CONFIG_CFS_BANDWIDTH */

#എങ്കിൽ !defined(CONFIG_CFS_BANDWIDTH) || !defined(CONFIG_NO_HZ_FULL)
സ്ഥിതം വരിയിൽ ശൂന്യം sched_fair_update_stop_tick(ഘടന rq *rq, ഘടന task_struct *p) {}
#endif

/**************************************************
 * CFS operations on tasks:
 */

#ifdef CONFIG_SCHED_HRTICK
സ്ഥിതം ശൂന്യം hrtick_start_fair(ഘടന rq *rq, ഘടന task_struct *p)
{
	ഘടന sched_entity *se = &p->se;
	unsigned long scale = 1024;
	unsigned long util = 0;
	u64 vdelta;
	u64 delta;

	WARN_ON_ONCE(task_rq(p) != rq);

	എങ്കിൽ (rq->cfs.h_nr_queued <= 1)
		തിരികെ;

	/*
	 * Compute time until virtual deadline
	 */
	vdelta = se->deadline - se->vruntime;
	എങ്കിൽ ((s64)vdelta < 0) {
		എങ്കിൽ (task_current_donor(rq, p))
			resched_curr(rq);
		തിരികെ;
	}
	delta = (se->h_load.weight * vdelta) / NICE_0_LOAD;

	/*
	 * Correct for instantaneous load of other classes.
	 */
	util += cpu_util_irq(rq);
	എങ്കിൽ (util && util < 1024) {
		scale *= 1024;
		scale /= (1024 - util);
	}

	hrtick_start(rq, (scale * delta) / 1024);
}

/*
 * Called on enqueue to start the hrtick when h_nr_queued becomes more than 1.
 */
സ്ഥിതം ശൂന്യം hrtick_update(ഘടന rq *rq)
{
	ഘടന task_struct *donor = rq->donor;

	എങ്കിൽ (!hrtick_enabled_fair(rq) || donor->sched_class != &fair_sched_class)
		തിരികെ;

	എങ്കിൽ (hrtick_active(rq))
		തിരികെ;

	hrtick_start_fair(rq, donor);
}
#അല്ലെങ്കിൽ /* !CONFIG_SCHED_HRTICK: */
സ്ഥിതം വരിയിൽ ശൂന്യം
hrtick_start_fair(ഘടന rq *rq, ഘടന task_struct *p)
{
}

സ്ഥിതം വരിയിൽ ശൂന്യം hrtick_update(ഘടന rq *rq)
{
}
#endif /* !CONFIG_SCHED_HRTICK */

സ്ഥിതം വരിയിൽ bool cpu_overutilized(പൂർണ്ണസംഖ്യ cpu)
{
	unsigned long rq_util_max;

	എങ്കിൽ (!sched_energy_enabled())
		തിരികെ false;

	rq_util_max = uclamp_rq_get(cpu_rq(cpu), UCLAMP_MAX);

	/* Return true only if the utilization doesn't fit CPU's capacity */
	തിരികെ !util_fits_cpu(cpu_util_cfs(cpu), 0, rq_util_max, cpu);
}

/*
 * overutilized value make sense only if EAS is enabled
 */
സ്ഥിതം വരിയിൽ bool is_rd_overutilized(ഘടന വേര്_domain *rd)
{
	തിരികെ !sched_energy_enabled() || READ_ONCE(rd->overutilized);
}

സ്ഥിതം വരിയിൽ ശൂന്യം set_rd_overutilized(ഘടന വേര്_domain *rd, bool flag)
{
	എങ്കിൽ (!sched_energy_enabled())
		തിരികെ;

	WRITE_ONCE(rd->overutilized, flag);
	trace_sched_overutilized_tp(rd, flag);
}

സ്ഥിതം വരിയിൽ ശൂന്യം check_update_overutilized_status(ഘടന rq *rq)
{
	/*
	 * overutilized field is used for load balancing decisions only
	 * if energy aware scheduler is being used
	 */

	എങ്കിൽ (!is_rd_overutilized(rq->rd) && cpu_overutilized(rq->cpu))
		set_rd_overutilized(rq->rd, 1);
}

/* Runqueue only has SCHED_IDLE tasks enqueued */
സ്ഥിതം പൂർണ്ണസംഖ്യ sched_idle_rq(ഘടന rq *rq)
{
	തിരികെ unlikely(rq->nr_running == rq->cfs.h_nr_idle &&
			rq->nr_running);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ choose_sched_idle_rq(ഘടന rq *rq, ഘടന task_struct *p)
{
	തിരികെ sched_idle_rq(rq) && !task_has_idle_policy(p);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ choose_idle_cpu(പൂർണ്ണസംഖ്യ cpu, ഘടന task_struct *p)
{
	തിരികെ available_idle_cpu(cpu) ||
	       choose_sched_idle_rq(cpu_rq(cpu), p);
}

സ്ഥിതം ശൂന്യം
requeue_delayed_entity(ഘടന cfs_rq *cfs_rq, ഘടന sched_entity *se)
{
	/*
	 * se->sched_delayed should imply: se->on_rq == 1.
	 * Because a delayed entity is one that is still on
	 * the runqueue competing until elegibility.
	 */
	WARN_ON_ONCE(!se->sched_delayed);
	WARN_ON_ONCE(!se->on_rq);

	എങ്കിൽ (update_entity_lag(cfs_rq, se)) {
		cfs_rq->h_nr_queued--;
		എങ്കിൽ (se != cfs_rq->curr)
			__dequeue_entity(cfs_rq, se);
		place_entity(cfs_rq, se, 0);
		എങ്കിൽ (se != cfs_rq->curr)
			__enqueue_entity(cfs_rq, se);
		cfs_rq->h_nr_queued++;
	}

	update_load_avg(cfs_rq, se, 0);
	clear_delayed(se);
}

സ്ഥിതം unsigned long enqueue_hierarchy(ഘടന task_struct *p, പൂർണ്ണസംഖ്യ flags)
{
	unsigned long weight = NICE_0_LOAD;
	പൂർണ്ണസംഖ്യ task_new = !(flags & ENQUEUE_WAKEUP);
	ഘടന sched_entity *se = &p->se;
	പൂർണ്ണസംഖ്യ h_nr_idle = task_has_idle_policy(p);
	പൂർണ്ണസംഖ്യ h_nr_runnable = 1;

	എങ്കിൽ (task_new && se->sched_delayed)
		h_nr_runnable = 0;

	for_each_sched_entity(se) {
		ഘടന cfs_rq *cfs_rq = cfs_rq_of(se);

		update_curr(cfs_rq);

		എങ്കിൽ (!se->on_rq) {
			enqueue_entity(cfs_rq, se, flags);
		} അല്ലെങ്കിൽ {
			update_load_avg(cfs_rq, se, UPDATE_TG);
			se_update_runnable(se);
			update_cfs_group(se);
		}

		cfs_rq->h_nr_runnable += h_nr_runnable;
		cfs_rq->h_nr_queued++;
		cfs_rq->h_nr_idle += h_nr_idle;

		എങ്കിൽ (cfs_rq_is_idle(cfs_rq))
			h_nr_idle = 1;

		weight = __calc_prop_weight(cfs_rq, se, weight);

		flags = ENQUEUE_WAKEUP;
	}

	തിരികെ weight;
}

/* Update curr's vruntime before placing entity or updating lag */
സ്ഥിതം വരിയിൽ ശൂന്യം update_curr_eevdf(ഘടന cfs_rq *cfs_rq)
{
	എങ്കിൽ (!cfs_rq->curr)
		തിരികെ;

	update_curr(cfs_rq_of(cfs_rq->curr));
}

/*
 * The enqueue_task method is called before nr_running is
 * increased. Here we update the fair scheduling stats and
 * then put the task into the rbtree:
 */
സ്ഥിതം ശൂന്യം
enqueue_task_fair(ഘടന rq *rq, ഘടന task_struct *p, പൂർണ്ണസംഖ്യ flags)
{
	പൂർണ്ണസംഖ്യ rq_h_nr_queued = rq->cfs.h_nr_queued;
	പൂർണ്ണസംഖ്യ task_new = !(flags & ENQUEUE_WAKEUP);
	ഘടന sched_entity *se = &p->se;
	ഘടന cfs_rq *cfs_rq = &rq->cfs;
	unsigned long weight;
	bool curr;

	എങ്കിൽ (task_is_throttled(p) && enqueue_throttled_task(p))
		തിരികെ;

	/*
	 * The code below (indirectly) updates schedutil which looks at
	 * the cfs_rq utilization to select a frequency.
	 * Let's add the task's estimated utilization to the cfs_rq's
	 * estimated utilization, before we update schedutil.
	 */
	എങ്കിൽ (!p->se.sched_delayed || (flags & ENQUEUE_DELAYED))
		util_est_enqueue(cfs_rq, p);

	update_curr_eevdf(cfs_rq);

	എങ്കിൽ (flags & ENQUEUE_DELAYED) {
		requeue_delayed_entity(cfs_rq, se);
		തിരികെ;
	}

	/*
	 * If in_iowait is set, the code below may not trigger any cpufreq
	 * utilization updates, so do it here explicitly with the IOWAIT flag
	 * passed.
	 */
	എങ്കിൽ (p->in_iowait)
		cpufreq_update_util(rq, SCHED_CPUFREQ_IOWAIT);

	curr = (cfs_rq->curr == se);
	/*
	 * This can actually happen with CFS_BANDWIDTH, a task can be throttled
	 * and dequeued, unthrottled from another CPU and then enqueued again
	 * before schedule().
	 */
	എങ്കിൽ (curr)
		place_entity(cfs_rq, se, flags);

	എങ്കിൽ (se->on_rq && se->sched_delayed)
		requeue_delayed_entity(cfs_rq, se);

	weight = enqueue_hierarchy(p, flags);

	എങ്കിൽ (!curr) {
		reweight_eevdf(cfs_rq, se, weight, false);
		place_entity(cfs_rq, se, flags | ENQUEUE_QUEUED);
		__enqueue_entity(cfs_rq, se);
	}

	എങ്കിൽ (!rq_h_nr_queued && rq->cfs.h_nr_queued)
		dl_server_start(&rq->fair_server);

	/* At this point se is NULL and we are at root level*/
	add_nr_running(rq, 1);

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
	എങ്കിൽ (!task_new)
		check_update_overutilized_status(rq);

	assert_list_leaf_cfs_rq(rq);

	hrtick_update(rq);
}

സ്ഥിതം ശൂന്യം dequeue_hierarchy(ഘടന task_struct *p, പൂർണ്ണസംഖ്യ flags)
{
	ഘടന sched_entity *se = &p->se;
	bool task_sleep = flags & DEQUEUE_SLEEP;
	bool task_delayed = flags & DEQUEUE_DELAYED;
	bool task_throttled = flags & DEQUEUE_THROTTLE;
	പൂർണ്ണസംഖ്യ h_nr_runnable = 0;
	പൂർണ്ണസംഖ്യ h_nr_idle = task_has_idle_policy(p);
	bool dequeue = true;

	എങ്കിൽ (task_sleep || task_delayed || !se->sched_delayed)
		h_nr_runnable = 1;

	for_each_sched_entity(se) {
		ഘടന cfs_rq *cfs_rq = cfs_rq_of(se);

		update_curr(cfs_rq);

		എങ്കിൽ (dequeue) {
			dequeue_entity(cfs_rq, se, flags);
			/* Don't dequeue parent if it has other entities besides us */
			എങ്കിൽ (cfs_rq->load.weight)
				dequeue = false;
		} അല്ലെങ്കിൽ {
			update_load_avg(cfs_rq, se, UPDATE_TG);
			se_update_runnable(se);
			update_cfs_group(se);
		}

		cfs_rq->h_nr_runnable -= h_nr_runnable;
		cfs_rq->h_nr_queued--;
		cfs_rq->h_nr_idle -= h_nr_idle;

		എങ്കിൽ (cfs_rq_is_idle(cfs_rq))
			h_nr_idle = 1;

		എങ്കിൽ (throttled_hierarchy(cfs_rq) && task_throttled)
			record_throttle_clock(cfs_rq);

		flags |= DEQUEUE_SLEEP;
		flags &= ~(DEQUEUE_DELAYED | DEQUEUE_SPECIAL);
	}
}

/*
 * The part of dequeue_task_fair() that is needed to dequeue delayed tasks.
 *
 * Returns:
 *   true  - dequeued
 *   false - delayed
 */
സ്ഥിതം bool __dequeue_task(ഘടന rq *rq, ഘടന task_struct *p, പൂർണ്ണസംഖ്യ flags)
{
	ഘടന sched_entity *se = &p->se;
	ഘടന cfs_rq *cfs_rq = &rq->cfs;
	bool was_sched_idle = sched_idle_rq(rq);
	bool task_sleep = flags & DEQUEUE_SLEEP;
	bool task_delayed = flags & DEQUEUE_DELAYED;

	clear_buddies(cfs_rq, se);

	update_curr_eevdf(cfs_rq);
	update_entity_lag(cfs_rq, se);

	എങ്കിൽ (flags & DEQUEUE_DELAYED) {
		WARN_ON_ONCE(!se->sched_delayed);
	} അല്ലെങ്കിൽ {
		bool delay = task_sleep;
		/*
		 * DELAY_DEQUEUE relies on spurious wakeups, special task
		 * states must not suffer spurious wakeups, excempt them.
		 */
		എങ്കിൽ (flags & (DEQUEUE_SPECIAL | DEQUEUE_THROTTLE))
			delay = false;

		WARN_ON_ONCE(delay && se->sched_delayed);

		എങ്കിൽ (sched_feat(DELAY_DEQUEUE) && delay &&
		    !entity_eligible(cfs_rq, se)) {
			update_load_avg(cfs_rq_of(se), se, UPDATE_UTIL_EST);
			set_delayed(se);
			തിരികെ false;
		}
	}

	dequeue_hierarchy(p, flags);

	എങ്കിൽ (sched_feat(PLACE_REL_DEADLINE) && !task_sleep) {
		se->deadline -= se->vruntime;
		se->rel_deadline = 1;
	}
	എങ്കിൽ (se != cfs_rq->curr)
		__dequeue_entity(cfs_rq, se);

	sub_nr_running(rq, 1);

	/* balance early to pull high priority tasks */
	എങ്കിൽ (unlikely(!was_sched_idle && sched_idle_rq(rq)))
		rq->next_balance = jiffies;

	എങ്കിൽ (task_delayed) {
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

	തിരികെ true;
}

/*
 * The dequeue_task method is called before nr_running is
 * decreased. We remove the task from the rbtree and
 * update the fair scheduling stats:
 */
സ്ഥിതം bool dequeue_task_fair(ഘടന rq *rq, ഘടന task_struct *p, പൂർണ്ണസംഖ്യ flags)
{
	എങ്കിൽ (task_is_throttled(p)) {
		dequeue_throttled_task(p, flags);
		തിരികെ true;
	}

	എങ്കിൽ (!p->se.sched_delayed)
		util_est_dequeue(&rq->cfs, p);

	എങ്കിൽ (!__dequeue_task(rq, p, flags))
		തിരികെ false;

	/*
	 * Must not reference @p after __dequeue_task(DEQUEUE_DELAYED).
	 */
	തിരികെ true;
}

സ്ഥിതം വരിയിൽ unsigned പൂർണ്ണസംഖ്യ cfs_h_nr_delayed(ഘടന rq *rq)
{
	തിരികെ (rq->cfs.h_nr_queued - rq->cfs.h_nr_runnable);
}

/* Working cpumask for: sched_balance_rq(), sched_balance_newidle(). */
സ്ഥിതം DEFINE_PER_CPU(cpumask_var_t, load_balance_mask);
സ്ഥിതം DEFINE_PER_CPU(cpumask_var_t, select_rq_mask);
സ്ഥിതം DEFINE_PER_CPU(cpumask_var_t, should_we_balance_tmpmask);

#ifdef CONFIG_NO_HZ_COMMON

സ്ഥിതം ഘടന {
	cpumask_var_t idle_cpus_mask;
	പൂർണ്ണസംഖ്യ has_blocked_load;		/* Idle CPUS has blocked load */
	പൂർണ്ണസംഖ്യ needs_update;		/* Newly idle CPUs need their next_balance collated */
	unsigned long next_balance;     /* in jiffy units */
	unsigned long next_blocked;	/* Next update of blocked load in jiffies */
} nohz ____cacheline_aligned;

#endif /* CONFIG_NO_HZ_COMMON */

സ്ഥിതം unsigned long cpu_load(ഘടന rq *rq)
{
	തിരികെ cfs_rq_load_avg(&rq->cfs);
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
സ്ഥിതം unsigned long cpu_load_without(ഘടന rq *rq, ഘടന task_struct *p)
{
	ഘടന cfs_rq *cfs_rq;
	unsigned പൂർണ്ണസംഖ്യ load;

	/* Task has no contribution or is new */
	എങ്കിൽ (cpu_of(rq) != task_cpu(p) || !READ_ONCE(p->se.avg.last_update_time))
		തിരികെ cpu_load(rq);

	cfs_rq = &rq->cfs;
	load = READ_ONCE(cfs_rq->avg.load_avg);

	/* Discount task's util from CPU's util */
	lsub_positive(&load, task_h_load(p));

	തിരികെ load;
}

സ്ഥിതം unsigned long cpu_runnable(ഘടന rq *rq)
{
	തിരികെ cfs_rq_runnable_avg(&rq->cfs);
}

സ്ഥിതം unsigned long cpu_runnable_without(ഘടന rq *rq, ഘടന task_struct *p)
{
	ഘടന cfs_rq *cfs_rq;
	unsigned പൂർണ്ണസംഖ്യ runnable;

	/* Task has no contribution or is new */
	എങ്കിൽ (cpu_of(rq) != task_cpu(p) || !READ_ONCE(p->se.avg.last_update_time))
		തിരികെ cpu_runnable(rq);

	cfs_rq = &rq->cfs;
	runnable = READ_ONCE(cfs_rq->avg.runnable_avg);

	/* Discount task's runnable from CPU's runnable */
	lsub_positive(&runnable, p->se.avg.runnable_avg);

	തിരികെ runnable;
}

സ്ഥിതം unsigned long capacity_of(പൂർണ്ണസംഖ്യ cpu)
{
	തിരികെ cpu_rq(cpu)->cpu_capacity;
}

സ്ഥിതം ശൂന്യം record_wakee(ഘടന task_struct *p)
{
	/*
	 * Only decay a single time; tasks that have less then 1 wakeup per
	 * jiffy will not have built up many flips.
	 */
	എങ്കിൽ (time_after(jiffies, current->wakee_flip_decay_ts + HZ)) {
		current->wakee_flips >>= 1;
		current->wakee_flip_decay_ts = jiffies;
	}

	എങ്കിൽ (current->last_wakee != p) {
		current->last_wakee = p;
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
സ്ഥിതം പൂർണ്ണസംഖ്യ wake_wide(ഘടന task_struct *p)
{
	unsigned പൂർണ്ണസംഖ്യ master = current->wakee_flips;
	unsigned പൂർണ്ണസംഖ്യ slave = p->wakee_flips;
	പൂർണ്ണസംഖ്യ factor = __this_cpu_read(sd_llc_size);

	എങ്കിൽ (master < slave)
		swap(master, slave);
	എങ്കിൽ (slave < factor || master < slave * factor)
		തിരികെ 0;
	തിരികെ 1;
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
സ്ഥിതം പൂർണ്ണസംഖ്യ
wake_affine_idle(പൂർണ്ണസംഖ്യ this_cpu, പൂർണ്ണസംഖ്യ prev_cpu, പൂർണ്ണസംഖ്യ sync)
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
	എങ്കിൽ (available_idle_cpu(this_cpu) && cpus_share_cache(this_cpu, prev_cpu))
		തിരികെ available_idle_cpu(prev_cpu) ? prev_cpu : this_cpu;

	എങ്കിൽ (sync) {
		ഘടന rq *rq = cpu_rq(this_cpu);

		എങ്കിൽ ((rq->nr_running - cfs_h_nr_delayed(rq)) == 1)
			തിരികെ this_cpu;
	}

	എങ്കിൽ (available_idle_cpu(prev_cpu))
		തിരികെ prev_cpu;

	തിരികെ nr_cpumask_bits;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
wake_affine_weight(ഘടന sched_domain *sd, ഘടന task_struct *p,
		   പൂർണ്ണസംഖ്യ this_cpu, പൂർണ്ണസംഖ്യ prev_cpu, പൂർണ്ണസംഖ്യ sync)
{
	s64 this_eff_load, prev_eff_load;
	unsigned long task_load;

	this_eff_load = cpu_load(cpu_rq(this_cpu));

	എങ്കിൽ (sync) {
		unsigned long current_load = task_h_load(current);

		എങ്കിൽ (current_load > this_eff_load)
			തിരികെ this_cpu;

		this_eff_load -= current_load;
	}

	task_load = task_h_load(p);

	this_eff_load += task_load;
	എങ്കിൽ (sched_feat(WA_BIAS))
		this_eff_load *= 100;
	this_eff_load *= capacity_of(prev_cpu);

	prev_eff_load = cpu_load(cpu_rq(prev_cpu));
	prev_eff_load -= task_load;
	എങ്കിൽ (sched_feat(WA_BIAS))
		prev_eff_load *= 100 + (sd->imbalance_pct - 100) / 2;
	prev_eff_load *= capacity_of(this_cpu);

	/*
	 * If sync, adjust the weight of prev_eff_load such that if
	 * prev_eff == this_eff that select_idle_sibling() will consider
	 * stacking the wakee on top of the waker if no other CPU is
	 * idle.
	 */
	എങ്കിൽ (sync)
		prev_eff_load += 1;

	തിരികെ this_eff_load < prev_eff_load ? this_cpu : nr_cpumask_bits;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ wake_affine(ഘടന sched_domain *sd, ഘടന task_struct *p,
		       പൂർണ്ണസംഖ്യ this_cpu, പൂർണ്ണസംഖ്യ prev_cpu, പൂർണ്ണസംഖ്യ sync)
{
	പൂർണ്ണസംഖ്യ target = nr_cpumask_bits;

	എങ്കിൽ (sched_feat(WA_IDLE))
		target = wake_affine_idle(this_cpu, prev_cpu, sync);

	എങ്കിൽ (sched_feat(WA_WEIGHT) && target == nr_cpumask_bits)
		target = wake_affine_weight(sd, p, this_cpu, prev_cpu, sync);

	schedstat_inc(p->stats.nr_wakeups_affine_attempts);
	എങ്കിൽ (target != this_cpu)
		തിരികെ prev_cpu;

	schedstat_inc(sd->ttwu_move_affine);
	schedstat_inc(p->stats.nr_wakeups_affine);
	തിരികെ target;
}

സ്ഥിതം ഘടന sched_group *
sched_balance_find_dst_group(ഘടന sched_domain *sd, ഘടന task_struct *p, പൂർണ്ണസംഖ്യ this_cpu);

/*
 * sched_balance_find_dst_group_cpu - find the idlest CPU among the CPUs in the group.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ
sched_balance_find_dst_group_cpu(ഘടന sched_group *group, ഘടന task_struct *p, പൂർണ്ണസംഖ്യ this_cpu)
{
	unsigned long load, min_load = ULONG_MAX;
	u64 min_exit_latency = U64_MAX;
	unsigned പൂർണ്ണസംഖ്യ nr_candidates = 0;
	പൂർണ്ണസംഖ്യ least_loaded_cpu = this_cpu;
	പൂർണ്ണസംഖ്യ shallowest_idle_cpu = -1;
	പൂർണ്ണസംഖ്യ i;

	/* Check if we have any choice: */
	എങ്കിൽ (group->group_weight == 1)
		തിരികെ cpumask_first(sched_group_span(group));

	/* Traverse only the allowed CPUs */
	for_each_cpu_and(i, sched_group_span(group), p->cpus_ptr) {
		ഘടന rq *rq = cpu_rq(i);

		എങ്കിൽ (!sched_core_cookie_match(rq, p))
			തുടരുക;

		എങ്കിൽ (choose_sched_idle_rq(rq, p))
			തിരികെ i;

		എങ്കിൽ (available_idle_cpu(i)) {
			ഘടന cpuidle_state *idle = idle_get_state(rq);
			u64 exit_latency = idle ? idle->exit_latency : U64_MAX;

			എങ്കിൽ (shallowest_idle_cpu == -1 || exit_latency < min_exit_latency) {
				min_exit_latency = exit_latency;
				shallowest_idle_cpu = i;
				nr_candidates = 1;
			} അല്ലെങ്കിൽ എങ്കിൽ (exit_latency == min_exit_latency) {
				nr_candidates++;
				എങ്കിൽ (!reciprocal_scale(sched_rng(), nr_candidates))
					shallowest_idle_cpu = i;
			}
		} അല്ലെങ്കിൽ എങ്കിൽ (shallowest_idle_cpu == -1) {
			load = cpu_load(cpu_rq(i));
			എങ്കിൽ (load < min_load) {
				min_load = load;
				least_loaded_cpu = i;
			}
		}
	}

	തിരികെ shallowest_idle_cpu != -1 ? shallowest_idle_cpu : least_loaded_cpu;
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ sched_balance_find_dst_cpu(ഘടന sched_domain *sd, ഘടന task_struct *p,
				  പൂർണ്ണസംഖ്യ cpu, പൂർണ്ണസംഖ്യ prev_cpu, പൂർണ്ണസംഖ്യ sd_flag)
{
	പൂർണ്ണസംഖ്യ new_cpu = cpu;

	എങ്കിൽ (!cpumask_intersects(sched_domain_span(sd), p->cpus_ptr))
		തിരികെ prev_cpu;

	/*
	 * We need task's util for cpu_util_without, sync it up to
	 * prev_cpu's last_update_time.
	 */
	എങ്കിൽ (!(sd_flag & SD_BALANCE_FORK))
		sync_entity_load_avg(&p->se);

	ആകുമ്പോൾ (sd) {
		ഘടന sched_group *group;
		ഘടന sched_domain *tmp;
		പൂർണ്ണസംഖ്യ weight;

		എങ്കിൽ (!(sd->flags & sd_flag)) {
			sd = sd->child;
			തുടരുക;
		}

		group = sched_balance_find_dst_group(sd, p, cpu);
		എങ്കിൽ (!group) {
			sd = sd->child;
			തുടരുക;
		}

		new_cpu = sched_balance_find_dst_group_cpu(group, p, cpu);
		എങ്കിൽ (new_cpu == cpu) {
			/* Now try balancing at a lower domain level of 'cpu': */
			sd = sd->child;
			തുടരുക;
		}

		/* Now try balancing at a lower domain level of 'new_cpu': */
		cpu = new_cpu;
		weight = sd->span_weight;
		sd = ശൂന്യ;
		for_each_domain(cpu, tmp) {
			എങ്കിൽ (weight <= tmp->span_weight)
				നിർത്തുക;
			എങ്കിൽ (tmp->flags & sd_flag)
				sd = tmp;
		}
	}

	തിരികെ new_cpu;
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ __select_idle_cpu(പൂർണ്ണസംഖ്യ cpu, ഘടന task_struct *p)
{
	എങ്കിൽ (choose_idle_cpu(cpu, p) && sched_cpu_cookie_match(cpu_rq(cpu), p))
		തിരികെ cpu;

	തിരികെ -1;
}

DEFINE_STATIC_താക്കോൽ_FALSE(sched_smt_present);
EXPORT_ചിഹ്നം_GPL(sched_smt_present);

സ്ഥിതം വരിയിൽ ശൂന്യം set_idle_cores(പൂർണ്ണസംഖ്യ cpu, പൂർണ്ണസംഖ്യ val)
{
	ഘടന sched_domain_shared *sds;

	sds = rcu_dereference_all(per_cpu(sd_balance_shared, cpu));
	എങ്കിൽ (sds)
		WRITE_ONCE(sds->has_idle_cores, val);
}

സ്ഥിതം വരിയിൽ bool test_idle_cores(പൂർണ്ണസംഖ്യ cpu)
{
	ഘടന sched_domain_shared *sds;

	sds = rcu_dereference_all(per_cpu(sd_balance_shared, cpu));
	എങ്കിൽ (sds)
		തിരികെ READ_ONCE(sds->has_idle_cores);

	തിരികെ false;
}

/*
 * Redirect a CPU to a higher-priority available sibling in its SMT domain,
 * subject to task affinity.
 */
സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ select_idle_smt_cpu(ഘടന task_struct *p, പൂർണ്ണസംഖ്യ cpu)
{
	ഘടന sched_domain *sd;
	പൂർണ്ണസംഖ്യ best = cpu;
	പൂർണ്ണസംഖ്യ sibling;

	എങ്കിൽ (!sched_smt_active())
		തിരികെ cpu;

	sd = rcu_dereference_all(cpu_rq(cpu)->sd);
	എങ്കിൽ (!sd || !(sd->flags & SD_SHARE_CPUCAPACITY) ||
	    !(sd->flags & SD_ASYM_PACKING))
		തിരികെ cpu;

	for_each_cpu_and(sibling, sched_domain_span(sd), p->cpus_ptr) {
		എങ്കിൽ (sibling == best || !choose_idle_cpu(sibling, p))
			തുടരുക;

		എങ്കിൽ (sched_asym_prefer(sibling, best))
			best = sibling;
	}

	തിരികെ best;
}

/*
 * Scans the local SMT mask to see if the entire core is idle, and records this
 * information in sd_balance_shared->has_idle_cores.
 *
 * Since SMT siblings share all cache levels, inspecting this limited remote
 * state should be fairly cheap.
 */
ശൂന്യം __update_idle_core(ഘടന rq *rq)
{
	പൂർണ്ണസംഖ്യ core = cpu_of(rq);
	പൂർണ്ണസംഖ്യ cpu;

	rcu_read_lock();
	എങ്കിൽ (test_idle_cores(core))
		പോകുക unlock;

	for_each_cpu(cpu, cpu_smt_mask(core)) {
		എങ്കിൽ (cpu == core)
			തുടരുക;

		എങ്കിൽ (!available_idle_cpu(cpu))
			പോകുക unlock;
	}

	set_idle_cores(core, 1);
unlock:
	rcu_read_unlock();
}

/*
 * Scan the entire LLC domain for idle cores; this dynamically switches off if
 * there are no idle cores left in the system; tracked through
 * sd_balance_shared->has_idle_cores and enabled through update_idle_core()
 * above.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ select_idle_core(ഘടന task_struct *p, പൂർണ്ണസംഖ്യ core, ഘടന cpumask *cpus, പൂർണ്ണസംഖ്യ *idle_cpu)
{
	bool idle = true;
	പൂർണ്ണസംഖ്യ cpu;

	for_each_cpu(cpu, cpu_smt_mask(core)) {
		എങ്കിൽ (!available_idle_cpu(cpu)) {
			idle = false;
			എങ്കിൽ (*idle_cpu == -1) {
				എങ്കിൽ (choose_sched_idle_rq(cpu_rq(cpu), p) &&
				    cpumask_test_cpu(cpu, cpus)) {
					*idle_cpu = cpu;
					നിർത്തുക;
				}
				തുടരുക;
			}
			നിർത്തുക;
		}
		എങ്കിൽ (*idle_cpu == -1 && cpumask_test_cpu(cpu, cpus))
			*idle_cpu = cpu;
	}

	എങ്കിൽ (idle)
		തിരികെ core;

	cpumask_andnot(cpus, cpus, cpu_smt_mask(core));
	തിരികെ -1;
}

/*
 * Scan the local SMT mask for idle CPUs.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ select_idle_smt(ഘടന task_struct *p, ഘടന sched_domain *sd, പൂർണ്ണസംഖ്യ target)
{
	പൂർണ്ണസംഖ്യ cpu;

	for_each_cpu_and(cpu, cpu_smt_mask(target), p->cpus_ptr) {
		എങ്കിൽ (cpu == target)
			തുടരുക;
		/*
		 * Check if the CPU is in the LLC scheduling domain of @target.
		 * Due to isolcpus, there is no guarantee that all the siblings are in the domain.
		 */
		എങ്കിൽ (!cpumask_test_cpu(cpu, sched_domain_span(sd)))
			തുടരുക;
		എങ്കിൽ (choose_idle_cpu(cpu, p))
			തിരികെ cpu;
	}

	തിരികെ -1;
}

/*
 * Scan the LLC domain for idle CPUs; this is dynamically regulated by
 * comparing the average scan cost (tracked in sd->avg_scan_cost) against the
 * average idle time for this rq (as found in rq->avg_idle).
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ select_idle_cpu(ഘടന task_struct *p, ഘടന sched_domain *sd, bool has_idle_core, പൂർണ്ണസംഖ്യ target)
{
	ഘടന cpumask *cpus = this_cpu_cpumask_var_ptr(select_rq_mask);
	പൂർണ്ണസംഖ്യ i, cpu, idle_cpu = -1, nr = INT_MAX;

	എങ്കിൽ (sched_feat(SIS_UTIL) && sd->shared) {
		/*
		 * Increment because !--nr is the condition to stop scan.
		 *
		 * Since "sd" is "sd_llc" for target CPU dereferenced in the
		 * caller, it is safe to directly dereference "sd->shared".
		 * Topology bits always ensure it assigned for "sd_llc" abd it
		 * cannot disappear as long as we have a RCU protected
		 * reference to one the associated "sd" here.
		 */
		nr = READ_ONCE(sd->shared->nr_idle_scan) + 1;
		/* overloaded LLC is unlikely to have idle cpu/core */
		എങ്കിൽ (nr == 1)
			തിരികെ -1;
	}

	എങ്കിൽ (!cpumask_and(cpus, sched_domain_span(sd), p->cpus_ptr))
		തിരികെ -1;

	എങ്കിൽ (static_branch_unlikely(&sched_cluster_active)) {
		ഘടന sched_group *sg = sd->groups;

		എങ്കിൽ (sg->flags & SD_CLUSTER) {
			for_each_cpu_wrap(cpu, sched_group_span(sg), target + 1) {
				എങ്കിൽ (!cpumask_test_cpu(cpu, cpus))
					തുടരുക;

				എങ്കിൽ (has_idle_core) {
					i = select_idle_core(p, cpu, cpus, &idle_cpu);
					എങ്കിൽ ((unsigned പൂർണ്ണസംഖ്യ)i < nr_cpumask_bits)
						തിരികെ i;
				} അല്ലെങ്കിൽ {
					എങ്കിൽ (--nr <= 0)
						തിരികെ -1;
					idle_cpu = __select_idle_cpu(cpu, p);
					എങ്കിൽ ((unsigned പൂർണ്ണസംഖ്യ)idle_cpu < nr_cpumask_bits)
						തിരികെ idle_cpu;
				}
			}
			cpumask_andnot(cpus, cpus, sched_group_span(sg));
		}
	}

	for_each_cpu_wrap(cpu, cpus, target + 1) {
		എങ്കിൽ (has_idle_core) {
			i = select_idle_core(p, cpu, cpus, &idle_cpu);
			എങ്കിൽ ((unsigned പൂർണ്ണസംഖ്യ)i < nr_cpumask_bits)
				തിരികെ i;

		} അല്ലെങ്കിൽ {
			എങ്കിൽ (--nr <= 0)
				തിരികെ -1;
			idle_cpu = __select_idle_cpu(cpu, p);
			എങ്കിൽ ((unsigned പൂർണ്ണസംഖ്യ)idle_cpu < nr_cpumask_bits)
				നിർത്തുക;
		}
	}

	എങ്കിൽ (has_idle_core)
		set_idle_cores(target, false);

	തിരികെ idle_cpu;
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
എണ്ണൽ asym_fits_state {
	ASYM_IDLE_UCLAMP_MISFIT = -4,
	ASYM_IDLE_COMPLETE_MISFIT,
	ASYM_IDLE_THREAD_FITS,
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
സ്ഥിതം പൂർണ്ണസംഖ്യ
select_idle_capacity(ഘടന task_struct *p, ഘടന sched_domain *sd, പൂർണ്ണസംഖ്യ target)
{
	/*
	 * On !SMT systems, has_idle_core is always false and preferred_core
	 * is always true (CPU == core), so the SMT preference logic below
	 * collapses to the plain capacity scan.
	 */
	bool has_idle_core = sched_smt_active() && test_idle_cores(target);
	unsigned long task_util, util_min, util_max, best_cap = 0;
	പൂർണ്ണസംഖ്യ fits, best_fits = ASYM_IDLE_THREAD_MISFIT;
	പൂർണ്ണസംഖ്യ cpu, best_cpu = -1;
	ഘടന cpumask *cpus;
	പൂർണ്ണസംഖ്യ nr = INT_MAX;

	cpus = this_cpu_cpumask_var_ptr(select_rq_mask);
	cpumask_and(cpus, sched_domain_span(sd), p->cpus_ptr);

	task_util = task_util_est(p);
	util_min = uclamp_eff_മൂല്യം(p, UCLAMP_MIN);
	util_max = uclamp_eff_മൂല്യം(p, UCLAMP_MAX);

	എങ്കിൽ (sched_feat(SIS_UTIL) && sd->shared) {
		/*
		 * Same nr_idle_scan hint as select_idle_cpu(), nr only limits
		 * the scan when not preferring an idle core.
		 */
		nr = READ_ONCE(sd->shared->nr_idle_scan) + 1;
		/* overloaded domain is unlikely to have idle cpu/core */
		എങ്കിൽ (nr == 1)
			തിരികെ -1;
	}

	for_each_cpu_wrap(cpu, cpus, target) {
		bool preferred_core = !has_idle_core || is_core_idle(cpu);
		unsigned long cpu_cap = capacity_of(cpu);

		/*
		 * Stop when the nr_idle_scan is exhausted (mirrors
		 * select_idle_cpu() logic).
		 */
		എങ്കിൽ (!has_idle_core && --nr <= 0)
			തിരികെ best_cpu;

		എങ്കിൽ (!choose_idle_cpu(cpu, p))
			തുടരുക;

		fits = util_fits_cpu(task_util, util_min, util_max, cpu);

		/*
		 * Perfect fit: capacity satisfies util + uclamp and the CPU
		 * sits on a fully-idle SMT core, this is a !SMT system, or
		 * there is no idle core to find.
		 * Short-circuit the rank-based selection and return
		 * immediately.
		 */
		എങ്കിൽ (fits > 0 && preferred_core)
			തിരികെ cpu;
		/*
		 * Only the min performance hint (i.e. uclamp_min) doesn't fit.
		 * Look for the CPU with best capacity.
		 */
		അല്ലെങ്കിൽ എങ്കിൽ (fits < 0)
			cpu_cap = get_actual_cpu_capacity(cpu);
		/*
		 * fits > 0 implies we are not on a preferred core, but the util
		 * fits CPU capacity. Set fits to ASYM_IDLE_THREAD_FITS
		 * so the effective range becomes
		 * [ASYM_IDLE_THREAD_FITS, ASYM_IDLE_THREAD_MISFIT], where:
		 *    ASYM_IDLE_THREAD_MISFIT - does not fit
		 *    ASYM_IDLE_THREAD_UCLAMP_MISFIT - fits with the exception of UCLAMP_MIN
		 *    ASYM_IDLE_THREAD_FITS - fits with the exception of preferred_core
		 */
		അല്ലെങ്കിൽ എങ്കിൽ (fits > 0)
			fits = ASYM_IDLE_THREAD_FITS;

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
		എങ്കിൽ (preferred_core)
			fits += ASYM_IDLE_CORE_BIAS;

		/*
		 * First, select CPU which fits better (lower is more preferred).
		 * Then, select the one with best capacity at same level.
		 */
		എങ്കിൽ ((fits < best_fits) ||
		    ((fits == best_fits) && (cpu_cap > best_cap))) {
			best_cap = cpu_cap;
			best_cpu = cpu;
			best_fits = fits;
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
	എങ്കിൽ (has_idle_core && best_fits > ASYM_IDLE_COMPLETE_MISFIT)
		set_idle_cores(target, false);

	തിരികെ best_cpu;
}

സ്ഥിതം വരിയിൽ bool asym_fits_cpu(unsigned long util,
				 unsigned long util_min,
				 unsigned long util_max,
				 പൂർണ്ണസംഖ്യ cpu)
{
	എങ്കിൽ (sched_asym_cpucap_active()) {
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
		തിരികെ (!sched_smt_active() || is_core_idle(cpu)) &&
		       (util_fits_cpu(util, util_min, util_max, cpu) > 0);
	}

	തിരികെ true;
}

/*
 * Try and locate an idle core/thread in the LLC cache domain.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ select_idle_sibling(ഘടന task_struct *p, പൂർണ്ണസംഖ്യ prev, പൂർണ്ണസംഖ്യ target)
{
	bool has_idle_core = false;
	ഘടന sched_domain *sd;
	unsigned long task_util, util_min, util_max;
	പൂർണ്ണസംഖ്യ i, recent_used_cpu, prev_aff = -1;

	/*
	 * On asymmetric system, update task utilization because we will check
	 * that the task fits with CPU's capacity.
	 */
	എങ്കിൽ (sched_asym_cpucap_active()) {
		sync_entity_load_avg(&p->se);
		task_util = task_util_est(p);
		util_min = uclamp_eff_മൂല്യം(p, UCLAMP_MIN);
		util_max = uclamp_eff_മൂല്യം(p, UCLAMP_MAX);
	}

	/*
	 * per-cpu select_rq_mask usage
	 */
	lockdep_assert_irqs_disabled();

	എങ്കിൽ (choose_idle_cpu(target, p) &&
	    asym_fits_cpu(task_util, util_min, util_max, target))
		പോകുക select_smt_priority;

	/*
	 * If the previous CPU is cache affine and idle, don't be stupid:
	 */
	എങ്കിൽ (prev != target && cpus_share_cache(prev, target) &&
	    choose_idle_cpu(prev, p) &&
	    asym_fits_cpu(task_util, util_min, util_max, prev)) {

		എങ്കിൽ (!static_branch_unlikely(&sched_cluster_active) ||
		    cpus_share_resources(prev, target)) {
			target = prev;
			പോകുക select_smt_priority;
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
	എങ്കിൽ (is_per_cpu_kthread(current) &&
	    in_task() &&
	    prev == smp_processor_id() &&
	    this_rq()->nr_running <= 1 &&
	    asym_fits_cpu(task_util, util_min, util_max, prev)) {
		target = prev;
		പോകുക select_smt_priority;
	}

	/* Check a recently used CPU as a potential idle candidate: */
	recent_used_cpu = p->recent_used_cpu;
	p->recent_used_cpu = prev;
	എങ്കിൽ (recent_used_cpu != prev &&
	    recent_used_cpu != target &&
	    cpus_share_cache(recent_used_cpu, target) &&
	    choose_idle_cpu(recent_used_cpu, p) &&
	    cpumask_test_cpu(recent_used_cpu, p->cpus_ptr) &&
	    asym_fits_cpu(task_util, util_min, util_max, recent_used_cpu)) {

		എങ്കിൽ (!static_branch_unlikely(&sched_cluster_active) ||
		    cpus_share_resources(recent_used_cpu, target)) {
			target = recent_used_cpu;
			പോകുക select_smt_priority;
		}

	} അല്ലെങ്കിൽ {
		recent_used_cpu = -1;
	}

	/*
	 * For asymmetric CPU capacity systems, our domain of interest is
	 * sd_asym_cpucapacity rather than sd_llc.
	 */
	എങ്കിൽ (sched_asym_cpucap_active()) {
		sd = rcu_dereference_all(per_cpu(sd_asym_cpucapacity, target));
		/*
		 * On an asymmetric CPU capacity system where an exclusive
		 * cpuset defines a symmetric island (i.e. one unique
		 * capacity_orig value through the cpuset), the key will be set
		 * but the CPUs within that cpuset will not have a domain with
		 * SD_ASYM_CPUCAPACITY. These should follow the usual symmetric
		 * capacity path.
		 */
		എങ്കിൽ (sd) {
			i = select_idle_capacity(p, sd, target);
			എങ്കിൽ ((unsigned പൂർണ്ണസംഖ്യ)i < nr_cpumask_bits) {
				target = i;
				പോകുക select_smt_priority;
			}
			തിരികെ target;
		}
	}

	sd = rcu_dereference_all(per_cpu(sd_llc, target));
	എങ്കിൽ (!sd)
		തിരികെ target;

	എങ്കിൽ (sched_smt_active()) {
		has_idle_core = test_idle_cores(target);

		എങ്കിൽ (!has_idle_core && cpus_share_cache(prev, target)) {
			i = select_idle_smt(p, sd, prev);
			എങ്കിൽ ((unsigned പൂർണ്ണസംഖ്യ)i < nr_cpumask_bits) {
				target = i;
				പോകുക select_smt_priority;
			}
		}
	}

	i = select_idle_cpu(p, sd, has_idle_core, target);
	എങ്കിൽ ((unsigned പൂർണ്ണസംഖ്യ)i < nr_cpumask_bits) {
		target = i;
		പോകുക select_smt_priority;
	}

	/*
	 * For cluster machines which have lower sharing cache like L2 or
	 * LLC Tag, we tend to find an idle CPU in the target's cluster
	 * first. But prev_cpu or recent_used_cpu may also be a good candidate,
	 * use them if possible when no idle CPU found in select_idle_cpu().
	 */
	എങ്കിൽ ((unsigned പൂർണ്ണസംഖ്യ)prev_aff < nr_cpumask_bits) {
		target = prev_aff;
		പോകുക select_smt_priority;
	}
	എങ്കിൽ ((unsigned പൂർണ്ണസംഖ്യ)recent_used_cpu < nr_cpumask_bits) {
		target = recent_used_cpu;
		പോകുക select_smt_priority;
	}

	തിരികെ target;

select_smt_priority:
	തിരികെ select_idle_smt_cpu(p, target);
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
സ്ഥിതം unsigned long
cpu_util(പൂർണ്ണസംഖ്യ cpu, ഘടന task_struct *p, പൂർണ്ണസംഖ്യ dst_cpu, പൂർണ്ണസംഖ്യ boost)
{
	bool add_task = p && task_cpu(p) != cpu && dst_cpu == cpu;
	bool sub_task = p && task_cpu(p) == cpu && dst_cpu != cpu;
	ഘടന cfs_rq *cfs_rq = &cpu_rq(cpu)->cfs;
	unsigned long util = READ_ONCE(cfs_rq->avg.util_avg);
	unsigned long runnable;

	/*
	 * If @dst_cpu is -1 or @p migrates from @cpu to @dst_cpu remove its
	 * contribution. If @p migrates from another CPU to @cpu add its
	 * contribution. In all the other cases @cpu is not impacted by the
	 * migration so its util_avg is already correct.
	 */
	എങ്കിൽ (add_task)
		util += task_util(p);
	അല്ലെങ്കിൽ എങ്കിൽ (sub_task)
		lsub_positive(&util, task_util(p));

	എങ്കിൽ (boost) {
		runnable = READ_ONCE(cfs_rq->avg.runnable_avg);
		എങ്കിൽ (add_task)
			runnable += READ_ONCE(p->se.avg.runnable_avg);
		അല്ലെങ്കിൽ എങ്കിൽ (sub_task)
			lsub_positive(&runnable,
				      READ_ONCE(p->se.avg.runnable_avg));
		util = max(util, runnable);
	}

	എങ്കിൽ (sched_feat(UTIL_EST)) {
		unsigned long util_est;

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
		എങ്കിൽ (dst_cpu == cpu)
			util_est += _task_util_est(p);
		അല്ലെങ്കിൽ എങ്കിൽ (p && unlikely(task_on_rq_queued(p) || current == p))
			lsub_positive(&util_est, _task_util_est(p));

		util = max(util, util_est);
	}

	തിരികെ min(util, arch_scale_cpu_capacity(cpu));
}

unsigned long cpu_util_cfs(പൂർണ്ണസംഖ്യ cpu)
{
	തിരികെ cpu_util(cpu, ശൂന്യ, -1, 0);
}

unsigned long cpu_util_cfs_boost(പൂർണ്ണസംഖ്യ cpu)
{
	തിരികെ cpu_util(cpu, ശൂന്യ, -1, 1);
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
സ്ഥിതം unsigned long cpu_util_without(പൂർണ്ണസംഖ്യ cpu, ഘടന task_struct *p)
{
	/* Task has no contribution or is new */
	എങ്കിൽ (cpu != task_cpu(p) || !READ_ONCE(p->se.avg.last_update_time))
		p = ശൂന്യ;

	തിരികെ cpu_util(cpu, p, -1, 0);
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
unsigned long effective_cpu_util(പൂർണ്ണസംഖ്യ cpu, unsigned long util_cfs,
				 unsigned long *min,
				 unsigned long *max)
{
	unsigned long util, irq, scale;
	ഘടന rq *rq = cpu_rq(cpu);

	scale = arch_scale_cpu_capacity(cpu);

	/*
	 * Early check to see if IRQ/steal time saturates the CPU, can be
	 * because of inaccuracies in how we track these -- see
	 * update_irq_load_avg().
	 */
	irq = cpu_util_irq(rq);
	എങ്കിൽ (unlikely(irq >= scale)) {
		എങ്കിൽ (min)
			*min = scale;
		എങ്കിൽ (max)
			*max = scale;
		തിരികെ scale;
	}

	എങ്കിൽ (min) {
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
		എങ്കിൽ (!uclamp_is_used() && rt_rq_is_runnable(&rq->rt))
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
	എങ്കിൽ (max)
		*max = min(scale, uclamp_rq_get(rq, UCLAMP_MAX));

	എങ്കിൽ (util >= scale)
		തിരികെ scale;

	/*
	 * There is still idle time; further improve the number by using the
	 * IRQ metric. Because IRQ/steal time is hidden from the task clock we
	 * need to scale the task numbers:
	 *
	 *              max - irq
	 *   U' = irq + --------- * U
	 *                 max
	 */
	util = scale_irq_capacity(util, irq, scale);
	util += irq;

	തിരികെ min(scale, util);
}

unsigned long sched_cpu_util(പൂർണ്ണസംഖ്യ cpu)
{
	തിരികെ effective_cpu_util(cpu, cpu_util_cfs(cpu), ശൂന്യ, ശൂന്യ);
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
ഘടന energy_env {
	unsigned long task_busy_time;
	unsigned long pd_busy_time;
	unsigned long cpu_cap;
	unsigned long pd_cap;
};

/*
 * Compute the task busy time for compute_energy(). This time cannot be
 * injected directly into effective_cpu_util() because of the IRQ scaling.
 * The latter only makes sense with the most recent CPUs where the task has
 * run.
 */
സ്ഥിതം വരിയിൽ ശൂന്യം eenv_task_busy_time(ഘടന energy_env *eenv,
				       ഘടന task_struct *p, പൂർണ്ണസംഖ്യ prev_cpu)
{
	unsigned long busy_time, max_cap = arch_scale_cpu_capacity(prev_cpu);
	unsigned long irq = cpu_util_irq(cpu_rq(prev_cpu));

	എങ്കിൽ (unlikely(irq >= max_cap))
		busy_time = max_cap;
	അല്ലെങ്കിൽ
		busy_time = scale_irq_capacity(task_util_est(p), irq, max_cap);

	eenv->task_busy_time = busy_time;
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
സ്ഥിതം വരിയിൽ ശൂന്യം eenv_pd_busy_time(ഘടന energy_env *eenv,
				     ഘടന cpumask *pd_cpus,
				     ഘടന task_struct *p)
{
	unsigned long busy_time = 0;
	പൂർണ്ണസംഖ്യ cpu;

	for_each_cpu(cpu, pd_cpus) {
		unsigned long util = cpu_util(cpu, p, -1, 0);

		busy_time += effective_cpu_util(cpu, util, ശൂന്യ, ശൂന്യ);
	}

	eenv->pd_busy_time = min(eenv->pd_cap, busy_time);
}

/*
 * Compute the maximum utilization for compute_energy() when the task @p
 * is placed on the cpu @dst_cpu.
 *
 * Returns the maximum utilization among @eenv->cpus. This utilization can't
 * exceed @eenv->cpu_cap.
 */
സ്ഥിതം വരിയിൽ unsigned long
eenv_pd_max_util(ഘടന energy_env *eenv, ഘടന cpumask *pd_cpus,
		 ഘടന task_struct *p, പൂർണ്ണസംഖ്യ dst_cpu)
{
	unsigned long max_util = 0;
	പൂർണ്ണസംഖ്യ cpu;

	for_each_cpu(cpu, pd_cpus) {
		ഘടന task_struct *tsk = (cpu == dst_cpu) ? p : ശൂന്യ;
		unsigned long util = cpu_util(cpu, p, dst_cpu, 1);
		unsigned long eff_util, min, max;

		/*
		 * Performance domain frequency: utilization clamping
		 * must be considered since it affects the selection
		 * of the performance domain frequency.
		 * NOTE: in case RT tasks are running, by default the min
		 * utilization can be max OPP.
		 */
		eff_util = effective_cpu_util(cpu, util, &min, &max);

		/* Task's uclamp can modify min and max value */
		എങ്കിൽ (tsk && uclamp_is_used()) {
			min = max(min, uclamp_eff_മൂല്യം(p, UCLAMP_MIN));

			/*
			 * If there is no active max uclamp constraint,
			 * directly use task's one, otherwise keep max.
			 */
			എങ്കിൽ (uclamp_rq_is_idle(cpu_rq(cpu)))
				max = uclamp_eff_മൂല്യം(p, UCLAMP_MAX);
			അല്ലെങ്കിൽ
				max = max(max, uclamp_eff_മൂല്യം(p, UCLAMP_MAX));
		}

		eff_util = sugov_effective_cpu_perf(cpu, eff_util, min, max);
		max_util = max(max_util, eff_util);
	}

	തിരികെ min(max_util, eenv->cpu_cap);
}

/*
 * compute_energy(): Use the Energy Model to estimate the energy that @pd would
 * consume for a given utilization landscape @eenv. When @dst_cpu < 0, the task
 * contribution is ignored.
 */
സ്ഥിതം വരിയിൽ unsigned long
compute_energy(ഘടന energy_env *eenv, ഘടന perf_domain *pd,
	       ഘടന cpumask *pd_cpus, ഘടന task_struct *p, പൂർണ്ണസംഖ്യ dst_cpu)
{
	unsigned long max_util = eenv_pd_max_util(eenv, pd_cpus, p, dst_cpu);
	unsigned long busy_time = eenv->pd_busy_time;
	unsigned long energy;

	എങ്കിൽ (dst_cpu >= 0)
		busy_time = min(eenv->pd_cap, busy_time + eenv->task_busy_time);

	energy = em_cpu_energy(pd->em_pd, max_util, busy_time, eenv->cpu_cap);

	trace_sched_compute_energy_tp(p, dst_cpu, energy, max_util, busy_time);

	തിരികെ energy;
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
സ്ഥിതം പൂർണ്ണസംഖ്യ find_energy_efficient_cpu(ഘടന task_struct *p, പൂർണ്ണസംഖ്യ prev_cpu)
{
	ഘടന cpumask *cpus = this_cpu_cpumask_var_ptr(select_rq_mask);
	unsigned long prev_delta = ULONG_MAX, best_delta = ULONG_MAX;
	unsigned long p_util_min = uclamp_is_used() ? uclamp_eff_മൂല്യം(p, UCLAMP_MIN) : 0;
	unsigned long p_util_max = uclamp_is_used() ? uclamp_eff_മൂല്യം(p, UCLAMP_MAX) : 1024;
	ഘടന വേര്_domain *rd = this_rq()->rd;
	പൂർണ്ണസംഖ്യ cpu, best_energy_cpu, target = -1;
	പൂർണ്ണസംഖ്യ prev_fits = -1, best_fits = -1;
	unsigned long best_actual_cap = 0;
	unsigned long prev_actual_cap = 0;
	ഘടന sched_domain *sd;
	ഘടന perf_domain *pd;
	ഘടന energy_env eenv;

	pd = rcu_dereference_all(rd->pd);
	എങ്കിൽ (!pd)
		തിരികെ target;

	/*
	 * Energy-aware wake-up happens on the lowest sched_domain starting
	 * from sd_asym_cpucapacity spanning over this_cpu and prev_cpu.
	 */
	sd = rcu_dereference_all(*this_cpu_ptr(&sd_asym_cpucapacity));
	ആകുമ്പോൾ (sd && !cpumask_test_cpu(prev_cpu, sched_domain_span(sd)))
		sd = sd->parent;
	എങ്കിൽ (!sd)
		തിരികെ target;

	target = prev_cpu;

	sync_entity_load_avg(&p->se);
	എങ്കിൽ (!task_util_est(p) && p_util_min == 0)
		തിരികെ target;

	eenv_task_busy_time(&eenv, p, prev_cpu);

	വേണ്ടി (; pd; pd = pd->next) {
		unsigned long util_min = p_util_min, util_max = p_util_max;
		unsigned long cpu_cap, cpu_actual_cap, util;
		long prev_spare_cap = -1, max_spare_cap = -1;
		unsigned long rq_util_min, rq_util_max;
		unsigned long cur_delta, base_energy;
		പൂർണ്ണസംഖ്യ max_spare_cap_cpu = -1;
		പൂർണ്ണസംഖ്യ fits, max_fits = -1;

		എങ്കിൽ (!cpumask_and(cpus, perf_domain_span(pd), cpu_online_mask))
			തുടരുക;

		/* Account external pressure for the energy estimation */
		cpu = cpumask_first(cpus);
		cpu_actual_cap = get_actual_cpu_capacity(cpu);

		eenv.cpu_cap = cpu_actual_cap;
		eenv.pd_cap = 0;

		for_each_cpu(cpu, cpus) {
			ഘടന rq *rq = cpu_rq(cpu);

			eenv.pd_cap += cpu_actual_cap;

			എങ്കിൽ (!cpumask_test_cpu(cpu, sched_domain_span(sd)))
				തുടരുക;

			എങ്കിൽ (!cpumask_test_cpu(cpu, p->cpus_ptr))
				തുടരുക;

			util = cpu_util(cpu, p, cpu, 0);
			cpu_cap = capacity_of(cpu);

			/*
			 * Skip CPUs that cannot satisfy the capacity request.
			 * IOW, placing the task there would make the CPU
			 * overutilized. Take uclamp into account to see how
			 * much capacity we can get out of the CPU; this is
			 * aligned with sched_cpu_util().
			 */
			എങ്കിൽ (uclamp_is_used() && !uclamp_rq_is_idle(rq)) {
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

			fits = util_fits_cpu(util, util_min, util_max, cpu);
			എങ്കിൽ (!fits)
				തുടരുക;

			lsub_positive(&cpu_cap, util);

			എങ്കിൽ (cpu == prev_cpu) {
				/* Always use prev_cpu as a candidate. */
				prev_spare_cap = cpu_cap;
				prev_fits = fits;
			} അല്ലെങ്കിൽ എങ്കിൽ ((fits > max_fits) ||
				   ((fits == max_fits) && ((long)cpu_cap > max_spare_cap))) {
				/*
				 * Find the CPU with the maximum spare capacity
				 * among the remaining CPUs in the performance
				 * domain.
				 */
				max_spare_cap = cpu_cap;
				max_spare_cap_cpu = cpu;
				max_fits = fits;
			}
		}

		എങ്കിൽ (max_spare_cap_cpu < 0 && prev_spare_cap < 0)
			തുടരുക;

		eenv_pd_busy_time(&eenv, cpus, p);
		/* Compute the 'base' energy of the pd, without @p */
		base_energy = compute_energy(&eenv, pd, cpus, p, -1);

		/* Evaluate the energy impact of using prev_cpu. */
		എങ്കിൽ (prev_spare_cap > -1) {
			prev_delta = compute_energy(&eenv, pd, cpus, p,
						    prev_cpu);
			/* CPU utilization has changed */
			എങ്കിൽ (prev_delta < base_energy)
				തിരികെ target;
			prev_delta -= base_energy;
			prev_actual_cap = cpu_actual_cap;
			best_delta = min(best_delta, prev_delta);
		}

		/* Evaluate the energy impact of using max_spare_cap_cpu. */
		എങ്കിൽ (max_spare_cap_cpu >= 0 && max_spare_cap > prev_spare_cap) {
			/* Current best energy cpu fits better */
			എങ്കിൽ (max_fits < best_fits)
				തുടരുക;

			/*
			 * Both don't fit performance hint (i.e. uclamp_min)
			 * but best energy cpu has better capacity.
			 */
			എങ്കിൽ ((max_fits < 0) &&
			    (cpu_actual_cap <= best_actual_cap))
				തുടരുക;

			cur_delta = compute_energy(&eenv, pd, cpus, p,
						   max_spare_cap_cpu);
			/* CPU utilization has changed */
			എങ്കിൽ (cur_delta < base_energy)
				തിരികെ target;
			cur_delta -= base_energy;

			/*
			 * Both fit for the task but best energy cpu has lower
			 * energy impact.
			 */
			എങ്കിൽ ((max_fits > 0) && (best_fits > 0) &&
			    (cur_delta >= best_delta))
				തുടരുക;

			best_delta = cur_delta;
			best_energy_cpu = max_spare_cap_cpu;
			best_fits = max_fits;
			best_actual_cap = cpu_actual_cap;
		}
	}

	എങ്കിൽ ((best_fits > prev_fits) ||
	    ((best_fits > 0) && (best_delta < prev_delta)) ||
	    ((best_fits < 0) && (best_actual_cap > prev_actual_cap)))
		target = best_energy_cpu;

	തിരികെ target;
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
സ്ഥിതം പൂർണ്ണസംഖ്യ
select_task_rq_fair(ഘടന task_struct *p, പൂർണ്ണസംഖ്യ prev_cpu, പൂർണ്ണസംഖ്യ wake_flags)
{
	പൂർണ്ണസംഖ്യ sync = (wake_flags & WF_SYNC) && !(current->flags & PF_EXITING);
	ഘടന sched_domain *tmp, *sd = ശൂന്യ;
	പൂർണ്ണസംഖ്യ cpu = smp_processor_id();
	പൂർണ്ണസംഖ്യ new_cpu = prev_cpu;
	പൂർണ്ണസംഖ്യ want_affine = 0;
	/* SD_flags and WF_flags share the first nibble */
	പൂർണ്ണസംഖ്യ sd_flag = wake_flags & 0xF;

	/*
	 * required for stable ->cpus_allowed
	 */
	lockdep_assert_held(&p->pi_lock);
	എങ്കിൽ (wake_flags & WF_TTWU) {
		record_wakee(p);

		എങ്കിൽ ((wake_flags & WF_CURRENT_CPU) &&
		    cpumask_test_cpu(cpu, p->cpus_ptr))
			തിരികെ cpu;

		എങ്കിൽ (!is_rd_overutilized(this_rq()->rd)) {
			new_cpu = find_energy_efficient_cpu(p, prev_cpu);
			എങ്കിൽ (new_cpu >= 0)
				തിരികെ new_cpu;
			new_cpu = prev_cpu;
		}

		want_affine = !wake_wide(p) && cpumask_test_cpu(cpu, p->cpus_ptr);
	}

	for_each_domain(cpu, tmp) {
		/*
		 * If both 'cpu' and 'prev_cpu' are part of this domain,
		 * cpu is a valid SD_WAKE_AFFINE target.
		 */
		എങ്കിൽ (want_affine && (tmp->flags & SD_WAKE_AFFINE) &&
		    cpumask_test_cpu(prev_cpu, sched_domain_span(tmp))) {
			എങ്കിൽ (cpu != prev_cpu)
				new_cpu = wake_affine(tmp, p, cpu, prev_cpu, sync);

			sd = ശൂന്യ; /* Prefer wake_affine over balance flags */
			നിർത്തുക;
		}

		/*
		 * Usually only true for WF_EXEC and WF_FORK, as sched_domains
		 * usually do not have SD_BALANCE_WAKE set. That means wakeup
		 * will usually go to the fast path.
		 */
		എങ്കിൽ (tmp->flags & sd_flag)
			sd = tmp;
		അല്ലെങ്കിൽ എങ്കിൽ (!want_affine)
			നിർത്തുക;
	}

	/* Slow path */
	എങ്കിൽ (unlikely(sd)) {
		new_cpu = sched_balance_find_dst_cpu(sd, p, cpu, prev_cpu, sd_flag);
		തിരികെ select_idle_smt_cpu(p, new_cpu);
	}

	/* Fast path */
	എങ്കിൽ (wake_flags & WF_TTWU)
		തിരികെ select_idle_sibling(p, prev_cpu, new_cpu);

	തിരികെ new_cpu;
}

/*
 * Called immediately before a task is migrated to a new CPU; task_cpu(p) and
 * cfs_rq_of(p) references at time of call are still valid and identify the
 * previous CPU. The caller guarantees p->pi_lock or task_rq(p)->lock is held.
 */
സ്ഥിതം ശൂന്യം migrate_task_rq_fair(ഘടന task_struct *p, പൂർണ്ണസംഖ്യ new_cpu)
{
	ഘടന sched_entity *se = &p->se;

	എങ്കിൽ (!task_on_rq_migrating(p)) {
		remove_entity_load_avg(se);

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
	se->avg.last_update_time = 0;

	update_scan_period(p, new_cpu);
}

സ്ഥിതം ശൂന്യം task_dead_fair(ഘടന task_struct *p)
{
	ഘടന sched_entity *se = &p->se;
	remove_entity_load_avg(se);
}

/*
 * Set the max capacity the task is allowed to run at for misfit detection.
 */
സ്ഥിതം ശൂന്യം set_task_max_allowed_capacity(ഘടന task_struct *p)
{
	ഘടന asym_cap_data *entry;

	എങ്കിൽ (!sched_asym_cpucap_active())
		തിരികെ;

	rcu_read_lock();
	list_for_each_entry_rcu(entry, &asym_cap_list, link) {
		cpumask_t *cpumask;

		cpumask = cpu_capacity_span(entry);
		എങ്കിൽ (!cpumask_intersects(p->cpus_ptr, cpumask))
			തുടരുക;

		p->max_allowed_capacity = entry->capacity;
		നിർത്തുക;
	}
	rcu_read_unlock();
}

സ്ഥിതം ശൂന്യം set_cpus_allowed_fair(ഘടന task_struct *p, ഘടന affinity_context *ctx)
{
	set_cpus_allowed_common(p, ctx);
	set_task_max_allowed_capacity(p);
}

എണ്ണൽ preempt_wakeup_action {
	PREEMPT_WAKEUP_NONE,	/* No preemption. */
	PREEMPT_WAKEUP_SHORT,	/* Ignore slice protection. */
	PREEMPT_WAKEUP_PICK,	/* Let pick_eevdf() decide. */
	PREEMPT_WAKEUP_RESCHED,	/* Force reschedule. */
};

സ്ഥിതം വരിയിൽ bool set_preempt_buddy(ഘടന cfs_rq *cfs_rq, ഘടന sched_entity *pse)
{
	/*
	 * Keep existing buddy if the deadline is sooner than pse.
	 * The older buddy may be cache cold and completely unrelated
	 * to the current wakeup but that is unpredictable where as
	 * obeying the deadline is more in line with EEVDF objectives.
	 */
	എങ്കിൽ (cfs_rq->next && entity_before(cfs_rq->next, pse))
		തിരികെ false;

	set_next_buddy(cfs_rq, pse);
	തിരികെ true;
}

സ്ഥിതം വരിയിൽ bool set_short_buddy(ഘടന cfs_rq *cfs_rq, ഘടന sched_entity *pse)
{
	എങ്കിൽ (cfs_rq->next) {
		എങ്കിൽ (cfs_rq->next->slice < pse->slice)
			തിരികെ false;

		എങ്കിൽ (cfs_rq->next->slice == pse->slice &&
		    entity_before(cfs_rq->next, pse))
			തിരികെ false;
	}

	set_next_buddy(cfs_rq, pse);
	തിരികെ true;
}

/*
 * WF_SYNC|WF_TTWU indicates the waker expects to sleep but it is not
 * strictly enforced because the hint is either misunderstood or
 * multiple tasks must be woken up.
 */
സ്ഥിതം വരിയിൽ എണ്ണൽ preempt_wakeup_action
preempt_sync(ഘടന rq *rq, പൂർണ്ണസംഖ്യ wake_flags,
	     ഘടന sched_entity *pse, ഘടന sched_entity *se)
{
	u64 threshold, delta;

	/*
	 * WF_SYNC without WF_TTWU is not expected so warn if it happens even
	 * though it is likely harmless.
	 */
	WARN_ON_ONCE(!(wake_flags & WF_TTWU));

	threshold = sysctl_sched_migration_cost;
	delta = rq_clock_task(rq) - se->exec_start;
	എങ്കിൽ ((s64)delta < 0)
		delta = 0;

	/*
	 * WF_RQ_SELECTED implies the tasks are stacking on a CPU when they
	 * could run on other CPUs. Reduce the threshold before preemption is
	 * allowed to an arbitrary lower value as it is more likely (but not
	 * guaranteed) the waker requires the wakee to finish.
	 */
	എങ്കിൽ (wake_flags & WF_RQ_SELECTED)
		threshold >>= 2;

	/*
	 * As WF_SYNC is not strictly obeyed, allow some runtime for batch
	 * wakeups to be issued.
	 */
	എങ്കിൽ (entity_before(pse, se) && delta >= threshold)
		തിരികെ PREEMPT_WAKEUP_RESCHED;

	തിരികെ PREEMPT_WAKEUP_NONE;
}

/*
 * Preempt the current task with a newly woken task if needed:
 */
സ്ഥിതം ശൂന്യം wakeup_preempt_fair(ഘടന rq *rq, ഘടന task_struct *p, പൂർണ്ണസംഖ്യ wake_flags)
{
	എണ്ണൽ preempt_wakeup_action preempt_action = PREEMPT_WAKEUP_PICK;
	ഘടന task_struct *donor = rq->donor;
	ഘടന sched_entity *nse, *se = &donor->se, *pse = &p->se;
	ഘടന cfs_rq *cfs_rq = &rq->cfs;
	പൂർണ്ണസംഖ്യ cse_is_idle, pse_is_idle;

	/*
	 * XXX Getting preempted by higher class, try and find idle CPU?
	 */
	എങ്കിൽ (p->sched_class != &fair_sched_class ||
	    donor->sched_class != &fair_sched_class)
		തിരികെ;

	എങ്കിൽ (unlikely(se == pse))
		തിരികെ;

	/*
	 * This is possible from callers such as attach_tasks(), in which we
	 * unconditionally wakeup_preempt() after an enqueue (which may have
	 * lead to a throttle).  This both saves work and prevents false
	 * next-buddy nomination below.
	 */
	എങ്കിൽ (task_is_throttled(p))
		തിരികെ;

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
	എങ്കിൽ (!sched_feat(PREEMPT_SHORT) && test_tsk_need_resched(rq->curr))
		തിരികെ;

	എങ്കിൽ (!sched_feat(WAKEUP_PREEMPTION))
		തിരികെ;

	WARN_ON_ONCE(!pse);

	cse_is_idle = se_is_idle(se);
	pse_is_idle = se_is_idle(pse);

	nse = se;
	/*
	 * Preempt an idle entity in favor of a non-idle entity (and don't preempt
	 * in the inverse case).
	 */
	എങ്കിൽ (cse_is_idle && !pse_is_idle)
		പോകുക preempt;

	update_curr_fair(rq);

	എങ്കിൽ (cse_is_idle != pse_is_idle)
		പോകുക update;

	/*
	 * BATCH and IDLE tasks do not preempt others.
	 */
	എങ്കിൽ (unlikely(!normal_policy(p->policy)))
		പോകുക update;

	/*
	 * Do not preempt for tasks that are sched_delayed as it would violate
	 * EEVDF to forcibly queue an ineligible task.
	 */
	എങ്കിൽ (pse->sched_delayed)
		പോകുക update;

	/*
	 * If @p has a shorter slice than current and @p is eligible, override
	 * current's slice protection in order to allow preemption.
	 */
	എങ്കിൽ (sched_feat(PREEMPT_SHORT) && (pse->slice < se->slice)) {
		preempt_action = PREEMPT_WAKEUP_SHORT;
		പോകുക pick;
	}

	/*
	 * Ignore wakee preemption on WF_FORK as it is less likely that
	 * there is shared data as exec often follow fork.
	 */
	എങ്കിൽ (wake_flags & WF_FORK)
		പോകുക update;

	/* Prefer picking wakee soon if appropriate. */
	എങ്കിൽ (sched_feat(NEXT_BUDDY) && set_preempt_buddy(cfs_rq, pse)) {
		/*
		 * Decide whether to obey WF_SYNC hint for a new buddy. Old
		 * buddies are ignored as they may not be relevant to the
		 * waker and less likely to be cache hot.
		 */
		എങ്കിൽ (wake_flags & WF_SYNC)
			preempt_action = preempt_sync(rq, wake_flags, pse, se);
	}

	തിരഞ്ഞെടുപ്പ് (preempt_action) {
	അവസ്ഥ PREEMPT_WAKEUP_NONE:
		തിരികെ;
	അവസ്ഥ PREEMPT_WAKEUP_RESCHED:
		പോകുക preempt;
	അവസ്ഥ PREEMPT_WAKEUP_SHORT:
		fallthrough;
	അവസ്ഥ PREEMPT_WAKEUP_PICK:
		നിർത്തുക;
	}

pick:
	എങ്കിൽ (cfs_rq->h_nr_queued) {
		nse = pick_next_entity(rq, preempt_action != PREEMPT_WAKEUP_SHORT);
		എങ്കിൽ (unlikely(!nse))
			പോകുക pick;

		/* If @p has become the most eligible task, force preemption */
		എങ്കിൽ (nse == pse)
			പോകുക preempt;
	}

	/*
	 * If @p is eligible but not the next task to run then cancel protection
	 * to prevent large scheduling latency
	 */
	എങ്കിൽ (preempt_action == PREEMPT_WAKEUP_SHORT && entity_eligible(cfs_rq, pse))
		പോകുക preempt;
update:
	എങ്കിൽ (sched_feat(RUN_TO_PARITY))
		update_protect_slice(cfs_rq, se);

	തിരികെ;

preempt:
	cancel_protect_slice(se);

	എങ്കിൽ (preempt_action == PREEMPT_WAKEUP_SHORT)
		set_short_buddy(cfs_rq, pse);

	resched_curr_lazy(rq);
}

ഘടന task_struct *pick_task_fair(ഘടന rq *rq, ഘടന rq_flags *rf)
	__must_hold(__rq_lockp(rq))
{
	ഘടന cfs_rq *cfs_rq = &rq->cfs;
	ഘടന sched_entity *se;
	ഘടന task_struct *p;
	പൂർണ്ണസംഖ്യ new_tasks;

again:
	എങ്കിൽ (!cfs_rq->h_nr_queued)
		പോകുക idle;

	/* Might not have done put_prev_entity() */
	എങ്കിൽ (cfs_rq->curr && cfs_rq->curr->on_rq)
		update_curr_eevdf(cfs_rq);

	se = pick_next_entity(rq, true);
	എങ്കിൽ (!se)
		പോകുക again;

	p = task_of(se);
	തിരികെ p;

idle:
	എങ്കിൽ (sched_core_enabled(rq))
		തിരികെ ശൂന്യ;

	new_tasks = sched_balance_newidle(rq, rf);
	എങ്കിൽ (new_tasks < 0)
		തിരികെ RETRY_TASK;
	എങ്കിൽ (new_tasks > 0)
		പോകുക again;
	തിരികെ ശൂന്യ;
}

സ്ഥിതം ഘടന task_struct *
fair_server_pick_task(ഘടന sched_dl_entity *dl_se, ഘടന rq_flags *rf)
	__must_hold(__rq_lockp(dl_se->rq))
{
	തിരികെ pick_task_fair(dl_se->rq, rf);
}

ശൂന്യം fair_server_init(ഘടന rq *rq)
{
	ഘടന sched_dl_entity *dl_se = &rq->fair_server;

	init_dl_entity(dl_se);

	dl_server_init(dl_se, rq, fair_server_pick_task);
}

/*
 * Account for a descheduled task:
 */
സ്ഥിതം ശൂന്യം put_prev_task_fair(ഘടന rq *rq, ഘടന task_struct *prev, ഘടന task_struct *next)
{
	ഘടന sched_entity *se = &prev->se;
	ഘടന cfs_rq *cfs_rq = &rq->cfs;
	ഘടന sched_entity *nse = ശൂന്യ;

#ifdef CONFIG_FAIR_GROUP_SCHED
	എങ്കിൽ (next && next->sched_class == &fair_sched_class)
		nse = &next->se;
#endif

	ആകുമ്പോൾ (se) {
		cfs_rq = cfs_rq_of(se);
		എങ്കിൽ (!nse || cfs_rq->h_curr)
			put_prev_entity(cfs_rq, se);
#ifdef CONFIG_FAIR_GROUP_SCHED
		എങ്കിൽ (nse) {
			എങ്കിൽ (is_same_group(se, nse))
				നിർത്തുക;

			പൂർണ്ണസംഖ്യ d = nse->depth - se->depth;
			എങ്കിൽ (d >= 0) {
				/* nse has equal or greater depth, ascend */
				nse = parent_entity(nse);
				/* if nse is the deeper, do not ascend se */
				എങ്കിൽ (d > 0)
					തുടരുക;
			}
		}
#endif
		se = parent_entity(se);
	}

	/* Put 'current' back into the tree. */
	cfs_rq = &rq->cfs;
	se = &prev->se;
	WARN_ON_ONCE(cfs_rq->curr != se);
	cfs_rq->curr = ശൂന്യ;
	എങ്കിൽ (se->on_rq)
		__enqueue_entity(cfs_rq, se);
}

/*
 * sched_yield() is very simple
 */
സ്ഥിതം ശൂന്യം yield_task_fair(ഘടന rq *rq)
{
	ഘടന task_struct *curr = rq->donor;
	ഘടന sched_entity *se = &curr->se;
	ഘടന cfs_rq *cfs_rq = &rq->cfs;

	/*
	 * Are we the only task in the tree?
	 */
	എങ്കിൽ (unlikely(rq->nr_running == 1))
		തിരികെ;

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
	എങ്കിൽ (entity_eligible(cfs_rq, se)) {
		se->vruntime = se->deadline;
		update_deadline(cfs_rq, se);
	}
}

സ്ഥിതം bool yield_to_task_fair(ഘടന rq *rq, ഘടന task_struct *p)
{
	ഘടന sched_entity *se = &p->se;

	/* !se->on_rq also covers throttled task */
	എങ്കിൽ (!se->on_rq || se->sched_delayed)
		തിരികെ false;

	/* Tell the scheduler that we'd really like se to run next. */
	set_next_buddy(&task_rq(p)->cfs, se);

	yield_task_fair(rq);

	തിരികെ true;
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

സ്ഥിതം unsigned long __read_mostly max_load_balance_interval = HZ/10;

എണ്ണൽ fbq_type { regular, remote, all };

/*
 * 'group_type' describes the group of CPUs at the moment of load balancing.
 *
 * The enum is ordered by pulling priority, with the group with lowest priority
 * first so the group_type can simply be compared when selecting the busiest
 * group. See update_sd_pick_busiest().
 */
എണ്ണൽ group_type {
	/* The group has spare capacity that can be used to run more tasks.  */
	group_has_spare = 0,
	/*
	 * The group is fully used and the tasks don't compete for more CPU
	 * cycles. Nevertheless, some tasks might wait before running.
	 */
	group_fully_busy,
	/*
	 * One task doesn't fit with CPU's capacity and must be migrated to a
	 * more powerful CPU.
	 */
	group_misfit_task,
	/*
	 * Balance SMT group that's fully busy. Can benefit from migration
	 * a task on SMT with busy sibling to another CPU on idle core.
	 */
	group_smt_balance,
	/*
	 * SD_ASYM_PACKING only: One local CPU with higher capacity is available,
	 * and the task should be migrated to it instead of running on the
	 * current CPU.
	 */
	group_asym_packing,
	/*
	 * The tasks' affinity constraints previously prevented the scheduler
	 * from balancing the load across the system.
	 */
	group_imbalanced,
	/*
	 * There are tasks running on non-preferred LLC, possible to move
	 * them to their preferred LLC without creating too much imbalance.
	 * The priority of group_llc_balance is lower than that of
	 * group_overloaded and higher than that of all other group types.
	 * This is because group_llc_balance may exacerbate load imbalance.
	 * If the LLC balancing attempt fails, the nr_balance_failed
	 * mechanism will trigger other group types to rebalance the load.
	 */
	group_llc_balance,
	/*
	 * The CPU is overloaded and can't provide expected CPU cycles to all
	 * tasks.
	 */
	group_overloaded
};

എണ്ണൽ migration_type {
	migrate_load = 0,
	migrate_util,
	migrate_task,
	migrate_misfit,
	migrate_llc_task
};

#define LBF_ALL_PINNED	0x01
#define LBF_NEED_BREAK	0x02
#define LBF_DST_PINNED  0x04
#define LBF_SOME_PINNED	0x08
#define LBF_ACTIVE_LB	0x10
#define LBF_LLC_PINNED	0x20
#define LBF_ACTIVE_LB_LLC	0x40

ഘടന lb_env {
	ഘടന sched_domain	*sd;

	ഘടന rq		*src_rq;
	പൂർണ്ണസംഖ്യ			src_cpu;

	പൂർണ്ണസംഖ്യ			dst_cpu;
	ഘടന rq		*dst_rq;
	bool			dst_core_idle;

	ഘടന cpumask		*dst_grpmask;
	പൂർണ്ണസംഖ്യ			new_dst_cpu;
	എണ്ണൽ cpu_idle_type	idle;
	long			imbalance;
	/* The set of CPUs under consideration for load-balancing */
	ഘടന cpumask		*cpus;

	unsigned പൂർണ്ണസംഖ്യ		flags;

	unsigned പൂർണ്ണസംഖ്യ		loop;
	unsigned പൂർണ്ണസംഖ്യ		loop_break;
	unsigned പൂർണ്ണസംഖ്യ		loop_max;

	എണ്ണൽ fbq_type		fbq_type;
	എണ്ണൽ migration_type	migration_type;
	ഘടന list_head	tasks;
};

/*
 * Is this task likely cache-hot:
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ task_hot(ഘടന task_struct *p, ഘടന lb_env *env)
{
	s64 delta;

	lockdep_assert_rq_held(env->src_rq);

	എങ്കിൽ (p->sched_class != &fair_sched_class)
		തിരികെ 0;

	എങ്കിൽ (unlikely(task_has_idle_policy(p)))
		തിരികെ 0;

	/* SMT siblings share cache */
	എങ്കിൽ (env->sd->flags & SD_SHARE_CPUCAPACITY)
		തിരികെ 0;

	/*
	 * Buddy candidates are cache hot:
	 */
	എങ്കിൽ (sched_feat(CACHE_HOT_BUDDY) && env->dst_rq->nr_running &&
	    (&p->se == cfs_rq_of(&p->se)->next))
		തിരികെ 1;

	എങ്കിൽ (sysctl_sched_migration_cost == -1)
		തിരികെ 1;

	/*
	 * Don't migrate task if the task's cookie does not match
	 * with the destination CPU's core cookie.
	 */
	എങ്കിൽ (!sched_core_cookie_match(cpu_rq(env->dst_cpu), p))
		തിരികെ 1;

	എങ്കിൽ (sysctl_sched_migration_cost == 0)
		തിരികെ 0;

	delta = rq_clock_task(env->src_rq) - p->se.exec_start;

	തിരികെ delta < (s64)sysctl_sched_migration_cost;
}

#ifdef CONFIG_NUMA_BALANCING
/*
 * Returns a positive value, if task migration degrades locality.
 * Returns 0, if task migration is not affected by locality.
 * Returns a negative value, if task migration improves locality i.e migration preferred.
 */
സ്ഥിതം long migrate_degrades_locality(ഘടന task_struct *p, ഘടന lb_env *env)
{
	ഘടന numa_group *numa_group = rcu_dereference_all(p->numa_group);
	unsigned long src_weight, dst_weight;
	പൂർണ്ണസംഖ്യ src_nid, dst_nid, dist;

	എങ്കിൽ (!static_branch_likely(&sched_numa_balancing))
		തിരികെ 0;

	എങ്കിൽ (!p->numa_faults || !(env->sd->flags & SD_NUMA))
		തിരികെ 0;

	src_nid = cpu_to_കണ്ണി(env->src_cpu);
	dst_nid = cpu_to_കണ്ണി(env->dst_cpu);

	എങ്കിൽ (src_nid == dst_nid)
		തിരികെ 0;

	/* Migrating away from the preferred node is always bad. */
	എങ്കിൽ (src_nid == p->numa_preferred_nid) {
		എങ്കിൽ (env->src_rq->nr_running > env->src_rq->nr_preferred_running)
			തിരികെ 1;
		അല്ലെങ്കിൽ
			തിരികെ 0;
	}

	/* Encourage migration to the preferred node. */
	എങ്കിൽ (dst_nid == p->numa_preferred_nid)
		തിരികെ -1;

	/* Leaving a core idle is often worse than degrading locality. */
	എങ്കിൽ (env->idle == CPU_IDLE)
		തിരികെ 0;

	dist = കണ്ണി_distance(src_nid, dst_nid);
	എങ്കിൽ (numa_group) {
		src_weight = group_weight(p, src_nid, dist);
		dst_weight = group_weight(p, dst_nid, dist);
	} അല്ലെങ്കിൽ {
		src_weight = task_weight(p, src_nid, dist);
		dst_weight = task_weight(p, dst_nid, dist);
	}

	തിരികെ src_weight - dst_weight;
}

#അല്ലെങ്കിൽ /* !CONFIG_NUMA_BALANCING: */
സ്ഥിതം വരിയിൽ long migrate_degrades_locality(ഘടന task_struct *p,
					     ഘടന lb_env *env)
{
	തിരികെ 0;
}
#endif /* !CONFIG_NUMA_BALANCING */

/*
 * Check whether the task is ineligible on the destination cpu
 *
 * When the PLACE_LAG scheduling feature is enabled and
 * dst_cfs_rq->nr_queued is greater than 1, if the task
 * is ineligible, it will also be ineligible when
 * it is migrated to the destination cpu.
 */
സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ task_is_ineligible_on_dst_cpu(ഘടന task_struct *p, പൂർണ്ണസംഖ്യ dest_cpu)
{
	ഘടന cfs_rq *dst_cfs_rq = &cpu_rq(dest_cpu)->cfs;

	എങ്കിൽ (sched_feat(PLACE_LAG) && dst_cfs_rq->h_nr_queued &&
	    !entity_eligible(&task_rq(p)->cfs, &p->se))
		തിരികെ 1;

	തിരികെ 0;
}

#ifdef CONFIG_SCHED_CACHE
/*
 * The margin used when comparing LLC utilization with CPU capacity.
 * It determines the LLC load level where active LLC aggregation is
 * done.
 * Derived from fits_capacity().
 *
 * (default: ~50%, tunable via debugfs)
 */
സ്ഥിതം bool fits_llc_capacity(unsigned long util, unsigned long max)
{
	u32 aggr_pct = llc_overaggr_pct;

	/*
	 * For single core systems, raise the aggregation
	 * threshold to accommodate more tasks.
	 */
	എങ്കിൽ (cpu_smt_അക്കം_threads == 1)
		aggr_pct = (aggr_pct * 3 / 2);

	തിരികെ util * 100 < max * aggr_pct;
}

/*
 * The margin used when comparing utilization.
 * is 'util1' noticeably greater than 'util2'
 * Derived from capacity_greater().
 * Bias is in perentage.
 */
/* Allows dst util to be bigger than src util by up to bias percent */
#define util_greater(util1, util2) \
	((util1) * 100 > (util2) * (100 + llc_imb_pct))

സ്ഥിതം __maybe_unused bool get_llc_stats(പൂർണ്ണസംഖ്യ cpu, unsigned long *util,
					 unsigned long *cap)
{
	ഘടന sched_domain_shared *sd_share;

	sd_share = rcu_dereference_all(per_cpu(sd_llc_shared, cpu));
	എങ്കിൽ (!sd_share)
		തിരികെ false;

	*util = READ_ONCE(sd_share->util_avg);
	*cap = READ_ONCE(sd_share->capacity);

	തിരികെ true;
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
എണ്ണൽ llc_mig {
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
സ്ഥിതം എണ്ണൽ llc_mig can_migrate_llc(പൂർണ്ണസംഖ്യ src_cpu, പൂർണ്ണസംഖ്യ dst_cpu,
				    unsigned long tsk_util,
				    bool to_pref)
{
	unsigned long src_util, dst_util, src_cap, dst_cap;

	എങ്കിൽ (!get_llc_stats(src_cpu, &src_util, &src_cap) ||
	    !get_llc_stats(dst_cpu, &dst_util, &dst_cap))
		തിരികെ mig_unrestricted;

	src_util = src_util < tsk_util ? 0 : src_util - tsk_util;
	dst_util = dst_util + tsk_util;

	എങ്കിൽ (!fits_llc_capacity(dst_util, dst_cap) &&
	    !fits_llc_capacity(src_util, src_cap))
		തിരികെ mig_unrestricted;

	എങ്കിൽ (to_pref) {
		/*
		 * Don't migrate if we will get preferred LLC too
		 * heavily loaded and if the dest is much busier
		 * than the src, in which case migration will
		 * increase the imbalance too much.
		 */
		എങ്കിൽ (!fits_llc_capacity(dst_util, dst_cap) &&
		    util_greater(dst_util, src_util))
			തിരികെ mig_forbid;
	} അല്ലെങ്കിൽ {
		/*
		 * Don't migrate if we will leave preferred LLC
		 * too idle, or if this migration leads to the
		 * non-preferred LLC falls within sysctl_aggr_imb percent
		 * of preferred LLC, leading to migration again
		 * back to preferred LLC.
		 */
		എങ്കിൽ (fits_llc_capacity(src_util, src_cap) ||
		    !util_greater(src_util, dst_util))
			തിരികെ mig_forbid;
	}
	തിരികെ mig_llc;
}

സ്ഥിതം വരിയിൽ bool task_misfits_asym_cpu(ഘടന lb_env *env, ഘടന task_struct *p)
{
	/*
	 * On asymmetric CPU capacity domains, do not let cache-aware
	 * balancing pull the task onto a destination CPU that cannot
	 * accommodate it. Doing so would turn the task into a misfit on
	 * the destination, trading a cache-locality gain for a capacity
	 * loss. If the task already does not fit its source CPU, the move
	 * cannot make things worse, so let the LLC preference decide.
	 */
	എങ്കിൽ ((env->sd->flags & SD_ASYM_CPUCAPACITY) && p &&
	    !task_fits_cpu(p, env->dst_cpu) &&
	    task_fits_cpu(p, env->src_cpu))
		തിരികെ true;

	തിരികെ false;
}

/*
 * Check if task p can migrate from source LLC to
 * destination LLC in terms of cache aware load balance.
 */
സ്ഥിതം എണ്ണൽ llc_mig can_migrate_llc_task(ഘടന lb_env *env,
					 ഘടന task_struct *p)
{
	ഘടന sched_cache_group *grp;
	bool to_pref;
	പൂർണ്ണസംഖ്യ cpu, src_cpu, dst_cpu;

	എങ്കിൽ (task_misfits_asym_cpu(env, p))
		തിരികെ mig_forbid;

	src_cpu = env->src_cpu;
	dst_cpu = env->dst_cpu;
	grp = rcu_dereference_all(p->sched_cache_grp);
	എങ്കിൽ (!grp)
		തിരികെ mig_unrestricted;

	cpu = READ_ONCE(grp->cpu);
	എങ്കിൽ (cpu < 0 || cpus_share_cache(src_cpu, dst_cpu))
		തിരികെ mig_unrestricted;

	/* skip cache aware load balance for too many threads */
	എങ്കിൽ (invalid_llc_nr(grp, p, dst_cpu) ||
	    exceed_llc_capacity(grp, dst_cpu)) {
		എങ്കിൽ (READ_ONCE(grp->cpu) != -1)
			WRITE_ONCE(grp->cpu, -1);
		തിരികെ mig_unrestricted;
	}

	എങ്കിൽ (cpus_share_cache(dst_cpu, cpu))
		to_pref = true;
	അല്ലെങ്കിൽ എങ്കിൽ (cpus_share_cache(src_cpu, cpu))
		to_pref = false;
	അല്ലെങ്കിൽ
		തിരികെ mig_unrestricted;

	തിരികെ can_migrate_llc(src_cpu, dst_cpu,
			       task_util(p), to_pref);
}

/*
 * Check if active load balance breaks LLC locality in
 * terms of cache aware load balance. The load level and
 * imbalance do not warrant breaking LLC preference per
 * the can_migrate_llc() policy. Here, the benefit of
 * LLC locality outweighs the power efficiency gained from
 * migrating the only runnable task away.
 */
സ്ഥിതം വരിയിൽ bool
alb_break_llc(ഘടന lb_env *env)
{
	എങ്കിൽ (!sched_cache_enabled())
		തിരികെ false;

	എങ്കിൽ (cpus_share_cache(env->src_cpu, env->dst_cpu))
		തിരികെ false;
	/*
	 * All tasks prefer to stay on their current CPU.
	 * Do not pull a task from its preferred CPU if:
	 * 1. It is the only task running and does not exceed
	 *    imbalance allowance; OR
	 * 2. Migrating it away from its preferred LLC would violate
	 *    the cache-aware scheduling policy.
	 */
	എങ്കിൽ (env->src_rq->nr_pref_llc_running &&
	    env->src_rq->nr_pref_llc_running == env->src_rq->cfs.h_nr_runnable) {
		unsigned long util = 0;
		ഘടന task_struct *cur;

		/*
		 * Migrating misfit tasks from current CPU
		 * to CPU with a better fit.
		 * Prioritize that over LLC preference.
		 */
		എങ്കിൽ (env->migration_type == migrate_misfit)
			തിരികെ false;

		എങ്കിൽ (env->src_rq->nr_running <= 1)
			തിരികെ true;

		cur = rcu_dereference_all(env->src_rq->curr);
		എങ്കിൽ (cur && cur->sched_class == &fair_sched_class)
			util = task_util(cur);

		എങ്കിൽ (task_misfits_asym_cpu(env, cur) ||
		    can_migrate_llc(env->src_cpu, env->dst_cpu,
				    util, false) == mig_forbid)
			തിരികെ true;
	}

	തിരികെ false;
}

/*
 * Returns true if p's preferred LLC does not match the destination CPU
 * under migrate_llc_task semantics. Passive LB passes migrate_llc_task
 * in env->migration_type, while active LB carries LBF_ACTIVE_LB_LLC in
 * env->flags to avoid overwriting env->migration_type.
 */
സ്ഥിതം വരിയിൽ bool
migrate_llc_task_wrong_dst(ഘടന task_struct *p, ഘടന lb_env *env)
{
	തിരികെ sched_cache_enabled() &&
	       (env->migration_type == migrate_llc_task ||
		env->flags & LBF_ACTIVE_LB_LLC) &&
	       READ_ONCE(p->preferred_llc) != llc_id(env->dst_cpu);
}

/*
 * Check if migrating task p from env->src_cpu to
 * env->dst_cpu breaks LLC localiy.
 */
സ്ഥിതം bool migrate_degrades_llc(ഘടന task_struct *p, ഘടന lb_env *env)
{
	എങ്കിൽ (!sched_cache_enabled())
		തിരികെ false;

	എങ്കിൽ (task_has_sched_core(p))
		തിരികെ false;
	/*
	 * Skip over tasks that would degrade LLC locality;
	 * only when nr_balanced_failed is sufficiently high do we
	 * ignore this constraint.
	 *
	 * Threshold of cache_nice_tries is set to 1 higher
	 * than nr_balance_failed to avoid excessive task
	 * migration at the same time.
	 */
	എങ്കിൽ (env->sd->nr_balance_failed >= env->sd->cache_nice_tries + 1)
		തിരികെ false;

	/*
	 * We know the env->src_cpu has some tasks prefer to
	 * run on env->dst_cpu, skip the tasks do not prefer
	 * env->dst_cpu, and find the one that prefers.
	 */
	എങ്കിൽ (migrate_llc_task_wrong_dst(p, env))
		തിരികെ true;

	എങ്കിൽ (can_migrate_llc_task(env, p) != mig_forbid)
		തിരികെ false;

	തിരികെ true;
}

#അല്ലെങ്കിൽ
സ്ഥിതം വരിയിൽ bool get_llc_stats(പൂർണ്ണസംഖ്യ cpu, unsigned long *util,
				 unsigned long *cap)
{
	തിരികെ false;
}

സ്ഥിതം വരിയിൽ bool
alb_break_llc(ഘടന lb_env *env)
{
	തിരികെ false;
}

സ്ഥിതം വരിയിൽ bool
migrate_llc_task_wrong_dst(ഘടന task_struct *p, ഘടന lb_env *env)
{
	തിരികെ false;
}

സ്ഥിതം വരിയിൽ bool
migrate_degrades_llc(ഘടന task_struct *p, ഘടന lb_env *env)
{
	തിരികെ false;
}
#endif
/*
 * can_migrate_task - may task p from runqueue rq be migrated to this_cpu?
 */
സ്ഥിതം
പൂർണ്ണസംഖ്യ can_migrate_task(ഘടന task_struct *p, ഘടന lb_env *env)
{
	long degrades, hot;

	lockdep_assert_rq_held(env->src_rq);
	എങ്കിൽ (p->sched_task_hot)
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
	എങ്കിൽ ((p->se.sched_delayed) && (env->migration_type != migrate_load))
		തിരികെ 0;

	എങ്കിൽ (lb_throttled_hierarchy(p, env->dst_cpu))
		തിരികെ 0;

	/*
	 * We want to prioritize the migration of eligible tasks.
	 * For ineligible tasks we soft-limit them and only allow
	 * them to migrate when nr_balance_failed is non-zero to
	 * avoid load-balancing trying very hard to balance the load.
	 */
	എങ്കിൽ (!env->sd->nr_balance_failed &&
	    task_is_ineligible_on_dst_cpu(p, env->dst_cpu))
		തിരികെ 0;

	/* Disregard percpu kthreads; they are where they need to be. */
	എങ്കിൽ (kthread_is_per_cpu(p))
		തിരികെ 0;

	എങ്കിൽ (task_is_blocked(p))
		തിരികെ 0;

	എങ്കിൽ (!cpumask_test_cpu(env->dst_cpu, p->cpus_ptr)) {
		പൂർണ്ണസംഖ്യ cpu;

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
		എങ്കിൽ (env->idle == CPU_NEWLY_IDLE ||
		    env->flags & (LBF_DST_PINNED | LBF_ACTIVE_LB))
			തിരികെ 0;

		/* Prevent to re-select dst_cpu via env's CPUs: */
		cpu = cpumask_first_and_and(env->dst_grpmask, env->cpus, p->cpus_ptr);

		എങ്കിൽ (cpu < nr_cpu_ids) {
			env->flags |= LBF_DST_PINNED;
			env->new_dst_cpu = cpu;
		}

		തിരികെ 0;
	}

	/* Record that we found at least one task that could run on dst_cpu */
	env->flags &= ~LBF_ALL_PINNED;

	എങ്കിൽ (task_on_cpu(env->src_rq, p) ||
	    task_current_donor(env->src_rq, p)) {
		schedstat_inc(p->stats.nr_failed_migrations_running);
		തിരികെ 0;
	}

	/*
	 * Aggressive migration if:
	 * 1) active balance
	 * 2) destination numa is preferred
	 * 3) task is cache cold, or
	 * 4) too many balance attempts have failed.
	 */
	എങ്കിൽ (env->flags & LBF_ACTIVE_LB)
		തിരികെ !migrate_llc_task_wrong_dst(p, env);

	degrades = migrate_degrades_locality(p, env);
	എങ്കിൽ (!degrades) {
		/*
		 * If the NUMA locality is not broken,
		 * further check if migration would hurt
		 * LLC locality.
		 */
		എങ്കിൽ (migrate_degrades_llc(p, env)) {
			/*
			 * If regular load balancing fails to pull a task
			 * due to LLC locality, this is expected behavior
			 * and we set LBF_LLC_PINNED so we don't increase
			 * nr_balance_failed unecessarily.
			 */
			എങ്കിൽ (env->migration_type != migrate_llc_task)
				env->flags |= LBF_LLC_PINNED;

			തിരികെ 0;
		}

		hot = task_hot(p, env);
	} അല്ലെങ്കിൽ {
		hot = degrades > 0;
	}

	എങ്കിൽ (!hot || env->sd->nr_balance_failed > env->sd->cache_nice_tries) {
		എങ്കിൽ (hot)
			p->sched_task_hot = 1;
		തിരികെ 1;
	}

	schedstat_inc(p->stats.nr_failed_migrations_hot);
	തിരികെ 0;
}

/*
 * detach_task() -- detach the task for the migration specified in env
 */
സ്ഥിതം ശൂന്യം detach_task(ഘടന task_struct *p, ഘടന lb_env *env)
{
	lockdep_assert_rq_held(env->src_rq);

	എങ്കിൽ (p->sched_task_hot) {
		p->sched_task_hot = 0;
		schedstat_inc(env->sd->lb_hot_gained[env->idle]);
		schedstat_inc(p->stats.nr_forced_migrations);
	}

	WARN_ON(task_current(env->src_rq, p));
	WARN_ON(task_current_donor(env->src_rq, p));

	deactivate_task(env->src_rq, p, DEQUEUE_NOCLOCK);
	set_task_cpu(p, env->dst_cpu);
}

/*
 * detach_one_task() -- tries to dequeue exactly one task from env->src_rq, as
 * part of active balancing operations within "domain".
 *
 * Returns a task if successful and NULL otherwise.
 */
സ്ഥിതം ഘടന task_struct *detach_one_task(ഘടന lb_env *env)
{
	ഘടന task_struct *p;

	lockdep_assert_rq_held(env->src_rq);

	list_for_each_entry_reverse(p,
			&env->src_rq->cfs_tasks, se.group_കണ്ണി) {
		എങ്കിൽ (!can_migrate_task(p, env))
			തുടരുക;

		detach_task(p, env);

		/*
		 * Right now, this is only the second place where
		 * lb_gained[env->idle] is updated (other is detach_tasks)
		 * so we can safely collect stats here rather than
		 * inside detach_tasks().
		 */
		schedstat_inc(env->sd->lb_gained[env->idle]);
		തിരികെ p;
	}
	തിരികെ ശൂന്യ;
}

/*
 * detach_tasks() -- tries to detach up to imbalance load/util/tasks from
 * busiest_rq, as part of a balancing operation within domain "sd".
 *
 * Returns number of detached tasks if successful and 0 otherwise.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ detach_tasks(ഘടന lb_env *env)
{
	ഘടന list_head *tasks = &env->src_rq->cfs_tasks;
	unsigned long util, load;
	ഘടന task_struct *p;
	പൂർണ്ണസംഖ്യ detached = 0;

	lockdep_assert_rq_held(env->src_rq);

	/*
	 * Source run queue has been emptied by another CPU, clear
	 * LBF_ALL_PINNED flag as we will not test any task.
	 */
	എങ്കിൽ (env->src_rq->nr_running <= 1) {
		env->flags &= ~LBF_ALL_PINNED;
		തിരികെ 0;
	}

	എങ്കിൽ (env->imbalance <= 0)
		തിരികെ 0;

	ആകുമ്പോൾ (!list_empty(tasks)) {
		/*
		 * We don't want to steal all, otherwise we may be treated likewise,
		 * which could at worst lead to a livelock crash.
		 */
		എങ്കിൽ (env->idle && env->src_rq->nr_running <= 1)
			നിർത്തുക;

		env->loop++;
		/* We've more or less seen every task there is, call it quits */
		എങ്കിൽ (env->loop > env->loop_max)
			നിർത്തുക;

		/* take a breather every nr_migrate tasks */
		എങ്കിൽ (env->loop > env->loop_break) {
			env->loop_break += SCHED_NR_MIGRATE_BREAK;
			env->flags |= LBF_NEED_BREAK;
			നിർത്തുക;
		}

		p = list_last_entry(tasks, ഘടന task_struct, se.group_കണ്ണി);

		എങ്കിൽ (!can_migrate_task(p, env))
			പോകുക next;

		തിരഞ്ഞെടുപ്പ് (env->migration_type) {
		അവസ്ഥ migrate_load:
			/*
			 * Depending of the number of CPUs and tasks and the
			 * cgroup hierarchy, task_h_load() can return a null
			 * value. Make sure that env->imbalance decreases
			 * otherwise detach_tasks() will stop only after
			 * detaching up to loop_max tasks.
			 */
			load = max_t(unsigned long, task_h_load(p), 1);

			എങ്കിൽ (sched_feat(LB_MIN) &&
			    load < 16 && !env->sd->nr_balance_failed)
				പോകുക next;

			/*
			 * Make sure that we don't migrate too much load.
			 * Nevertheless, let relax the constraint if
			 * scheduler fails to find a good waiting task to
			 * migrate.
			 */
			എങ്കിൽ (shr_bound(load, env->sd->nr_balance_failed) > env->imbalance)
				പോകുക next;

			env->imbalance -= load;
			നിർത്തുക;

		അവസ്ഥ migrate_util:
			util = task_util_est(p);

			എങ്കിൽ (shr_bound(util, env->sd->nr_balance_failed) > env->imbalance)
				പോകുക next;

			env->imbalance -= util;
			നിർത്തുക;

		അവസ്ഥ migrate_task:
			env->imbalance--;
			നിർത്തുക;

		അവസ്ഥ migrate_misfit:
			/* This is not a misfit task */
			എങ്കിൽ (task_fits_cpu(p, env->src_cpu))
				പോകുക next;

			env->imbalance = 0;
			നിർത്തുക;

		അവസ്ഥ migrate_llc_task:
			env->imbalance--;
			നിർത്തുക;
		}

		detach_task(p, env);
		list_add(&p->se.group_കണ്ണി, &env->tasks);

		detached++;

#ifdef CONFIG_PREEMPTION
		/*
		 * NEWIDLE balancing is a source of latency, so preemptible
		 * kernels will stop after the first task is detached to minimize
		 * the critical section.
		 */
		എങ്കിൽ (env->idle == CPU_NEWLY_IDLE)
			നിർത്തുക;
#endif

		/*
		 * We only want to steal up to the prescribed amount of
		 * load/util/tasks.
		 */
		എങ്കിൽ (env->imbalance <= 0)
			നിർത്തുക;

		തുടരുക;
next:
		എങ്കിൽ (p->sched_task_hot)
			schedstat_inc(p->stats.nr_failed_migrations_hot);

		list_move(&p->se.group_കണ്ണി, tasks);
	}

	/*
	 * Right now, this is one of only two places we collect this stat
	 * so we can safely collect detach_one_task() stats here rather
	 * than inside detach_one_task().
	 */
	schedstat_add(env->sd->lb_gained[env->idle], detached);

	തിരികെ detached;
}

/*
 * attach_tasks() -- attaches all tasks detached by detach_tasks() to their
 * new rq.
 */
സ്ഥിതം ശൂന്യം attach_tasks(ഘടന lb_env *env)
{
	__attach_tasks(env->dst_rq, &env->tasks);
}

#ifdef CONFIG_NO_HZ_COMMON
സ്ഥിതം വരിയിൽ bool cfs_rq_has_blocked_load(ഘടന cfs_rq *cfs_rq)
{
	എങ്കിൽ (cfs_rq->avg.load_avg)
		തിരികെ true;

	എങ്കിൽ (cfs_rq->avg.util_avg)
		തിരികെ true;

	തിരികെ false;
}

സ്ഥിതം വരിയിൽ bool others_have_blocked(ഘടന rq *rq)
{
	എങ്കിൽ (cpu_util_rt(rq))
		തിരികെ true;

	എങ്കിൽ (cpu_util_dl(rq))
		തിരികെ true;

	എങ്കിൽ (hw_load_avg(rq))
		തിരികെ true;

	എങ്കിൽ (cpu_util_irq(rq))
		തിരികെ true;

	തിരികെ false;
}

സ്ഥിതം വരിയിൽ ശൂന്യം update_blocked_load_tick(ഘടന rq *rq)
{
	WRITE_ONCE(rq->last_blocked_load_update_tick, jiffies);
}

സ്ഥിതം വരിയിൽ ശൂന്യം update_has_blocked_load_status(ഘടന rq *rq, bool has_blocked_load)
{
	എങ്കിൽ (!has_blocked_load)
		rq->has_blocked_load = 0;
}
#അല്ലെങ്കിൽ /* !CONFIG_NO_HZ_COMMON: */
സ്ഥിതം വരിയിൽ bool cfs_rq_has_blocked_load(ഘടന cfs_rq *cfs_rq) { തിരികെ false; }
സ്ഥിതം വരിയിൽ bool others_have_blocked(ഘടന rq *rq) { തിരികെ false; }
സ്ഥിതം വരിയിൽ ശൂന്യം update_blocked_load_tick(ഘടന rq *rq) {}
സ്ഥിതം വരിയിൽ ശൂന്യം update_has_blocked_load_status(ഘടന rq *rq, bool has_blocked_load) {}
#endif /* !CONFIG_NO_HZ_COMMON */

സ്ഥിതം bool __update_blocked_others(ഘടന rq *rq, bool *done)
{
	bool updated;

	/*
	 * update_load_avg() can call cpufreq_update_util(). Make sure that RT,
	 * DL and IRQ signals have been updated before updating CFS.
	 */
	updated = update_other_load_avgs(rq);

	എങ്കിൽ (others_have_blocked(rq))
		*done = false;

	തിരികെ updated;
}

#ifdef CONFIG_FAIR_GROUP_SCHED

സ്ഥിതം bool __update_blocked_fair(ഘടന rq *rq, bool *done)
{
	ഘടന cfs_rq *cfs_rq, *pos;
	bool decayed = false;

	/*
	 * Iterates the task_group tree in a bottom up fashion, see
	 * list_add_leaf_cfs_rq() for details.
	 */
	for_each_leaf_cfs_rq_safe(rq, cfs_rq, pos) {
		ഘടന sched_entity *se;

		എങ്കിൽ (update_cfs_rq_load_avg(cfs_rq_clock_pelt(cfs_rq), cfs_rq)) {
			update_tg_load_avg(cfs_rq);

			എങ്കിൽ (cfs_rq->nr_queued == 0)
				update_idle_cfs_rq_clock_pelt(cfs_rq);

			എങ്കിൽ (cfs_rq == &rq->cfs)
				decayed = true;
		}

		/* Propagate pending load changes to the parent, if any: */
		se = cfs_rq_se(cfs_rq);
		എങ്കിൽ (se && !skip_blocked_update(se))
			update_load_avg(cfs_rq_of(se), se, UPDATE_TG);

		/*
		 * There can be a lot of idle CPU cgroups.  Don't let fully
		 * decayed cfs_rqs linger on the list.
		 */
		എങ്കിൽ (cfs_rq_is_decayed(cfs_rq))
			list_del_leaf_cfs_rq(cfs_rq);

		/* Don't need periodic decay once load/util_avg are null */
		എങ്കിൽ (cfs_rq_has_blocked_load(cfs_rq))
			*done = false;
	}

	തിരികെ decayed;
}

/*
 * Compute the hierarchical load factor for cfs_rq and all its ascendants.
 * This needs to be done in a top-down fashion because the load of a child
 * group is a fraction of its parents load.
 */
സ്ഥിതം ശൂന്യം update_cfs_rq_h_load(ഘടന cfs_rq *cfs_rq)
{
	ഘടന sched_entity *se = cfs_rq_se(cfs_rq);
	unsigned long now = jiffies;
	unsigned long load;

	എങ്കിൽ (cfs_rq->last_h_load_update == now)
		തിരികെ;

	WRITE_ONCE(cfs_rq->h_load_next, ശൂന്യ);
	for_each_sched_entity(se) {
		cfs_rq = cfs_rq_of(se);
		WRITE_ONCE(cfs_rq->h_load_next, se);
		എങ്കിൽ (cfs_rq->last_h_load_update == now)
			നിർത്തുക;
	}

	എങ്കിൽ (!se) {
		cfs_rq->h_load = cfs_rq_load_avg(cfs_rq);
		cfs_rq->last_h_load_update = now;
	}

	ആകുമ്പോൾ ((se = READ_ONCE(cfs_rq->h_load_next)) != ശൂന്യ) {
		load = cfs_rq->h_load;
		load = div64_ul(load * se->avg.load_avg,
				cfs_rq_load_avg(cfs_rq) + 1);
		cfs_rq = group_cfs_rq(se);
		cfs_rq->h_load = load;
		cfs_rq->last_h_load_update = now;
	}
}

സ്ഥിതം unsigned long task_h_load(ഘടന task_struct *p)
{
	ഘടന cfs_rq *cfs_rq = task_cfs_rq(p);

	update_cfs_rq_h_load(cfs_rq);
	തിരികെ div64_ul(p->se.avg.load_avg * cfs_rq->h_load,
			cfs_rq_load_avg(cfs_rq) + 1);
}
#അല്ലെങ്കിൽ /* !CONFIG_FAIR_GROUP_SCHED: */
സ്ഥിതം bool __update_blocked_fair(ഘടന rq *rq, bool *done)
{
	ഘടന cfs_rq *cfs_rq = &rq->cfs;
	bool decayed;

	decayed = update_cfs_rq_load_avg(cfs_rq_clock_pelt(cfs_rq), cfs_rq);
	എങ്കിൽ (cfs_rq_has_blocked_load(cfs_rq))
		*done = false;

	തിരികെ decayed;
}

സ്ഥിതം unsigned long task_h_load(ഘടന task_struct *p)
{
	തിരികെ p->se.avg.load_avg;
}
#endif /* !CONFIG_FAIR_GROUP_SCHED */

സ്ഥിതം ശൂന്യം __sched_balance_update_blocked_averages(ഘടന rq *rq)
{
	bool decayed = false, done = true;

	update_blocked_load_tick(rq);

	decayed |= __update_blocked_others(rq, &done);
	decayed |= __update_blocked_fair(rq, &done);

	update_has_blocked_load_status(rq, !done);
	എങ്കിൽ (decayed)
		cpufreq_update_util(rq, 0);
}

സ്ഥിതം ശൂന്യം sched_balance_update_blocked_averages(പൂർണ്ണസംഖ്യ cpu)
{
	ഘടന rq *rq = cpu_rq(cpu);

	guard(rq_lock_irqsave)(rq);
	update_rq_clock(rq);
	__sched_balance_update_blocked_averages(rq);
}

/********** Helpers for sched_balance_find_src_group ************************/

/*
 * sg_lb_stats - stats of a sched_group required for load-balancing:
 */
ഘടന sg_lb_stats {
	unsigned long avg_load;			/* Avg load            over the CPUs of the group */
	unsigned long group_load;		/* Total load          over the CPUs of the group */
	unsigned long group_capacity;		/* Capacity            over the CPUs of the group */
	unsigned long group_util;		/* Total utilization   over the CPUs of the group */
	unsigned long group_runnable;		/* Total runnable time over the CPUs of the group */
	unsigned പൂർണ്ണസംഖ്യ sum_nr_running;		/* Nr of all tasks running in the group */
	unsigned പൂർണ്ണസംഖ്യ sum_h_nr_running;		/* Nr of CFS tasks running in the group */
	unsigned പൂർണ്ണസംഖ്യ idle_cpus;                 /* Nr of idle CPUs         in the group */
	unsigned പൂർണ്ണസംഖ്യ group_weight;
	എണ്ണൽ group_type group_type;
	unsigned പൂർണ്ണസംഖ്യ group_asym_packing;	/* Tasks should be moved to preferred CPU */
	unsigned പൂർണ്ണസംഖ്യ group_smt_balance;		/* Task on busy SMT be moved */
	unsigned പൂർണ്ണസംഖ്യ group_llc_balance;		/* Tasks should be moved to preferred LLC */
	unsigned long group_misfit_task_load;	/* A CPU has a task too big for its capacity */
	unsigned പൂർണ്ണസംഖ്യ group_overutilized;	/* At least one CPU is overutilized in the group */
#ifdef CONFIG_NUMA_BALANCING
	unsigned പൂർണ്ണസംഖ്യ nr_numa_running;
	unsigned പൂർണ്ണസംഖ്യ nr_preferred_running;
#endif
#ifdef CONFIG_SCHED_CACHE
	unsigned പൂർണ്ണസംഖ്യ nr_pref_dst_llc;
#endif
};

/*
 * sd_lb_stats - stats of a sched_domain required for load-balancing:
 */
ഘടന sd_lb_stats {
	ഘടന sched_group *busiest;		/* Busiest group in this sd */
	ഘടന sched_group *local;		/* Local group in this sd */
	unsigned long total_load;		/* Total load of all groups in sd */
	unsigned long total_capacity;		/* Total capacity of all groups in sd */
	unsigned long avg_load;			/* Average load across all groups in sd */
	unsigned പൂർണ്ണസംഖ്യ prefer_sibling;		/* Tasks should go to sibling first */

	ഘടന sg_lb_stats busiest_stat;	/* Statistics of the busiest group */
	ഘടന sg_lb_stats local_stat;		/* Statistics of the local group */
};

സ്ഥിതം വരിയിൽ ശൂന്യം init_sd_lb_stats(ഘടന sd_lb_stats *sds)
{
	/*
	 * Skimp on the clearing to avoid duplicate work. We can avoid clearing
	 * local_stat because update_sg_lb_stats() does a full clear/assignment.
	 * We must however set busiest_stat::group_type and
	 * busiest_stat::idle_cpus to the worst busiest group because
	 * update_sd_pick_busiest() reads these before assignment.
	 */
	*sds = (ഘടന sd_lb_stats){
		.busiest = ശൂന്യ,
		.local = ശൂന്യ,
		.total_load = 0UL,
		.total_capacity = 0UL,
		.busiest_stat = {
			.idle_cpus = UINT_MAX,
			.group_type = group_has_spare,
		},
	};
}

സ്ഥിതം unsigned long scale_rt_capacity(പൂർണ്ണസംഖ്യ cpu)
{
	unsigned long max = get_actual_cpu_capacity(cpu);
	ഘടന rq *rq = cpu_rq(cpu);
	unsigned long used, സ്വതന്ത്രം;
	unsigned long irq;

	irq = cpu_util_irq(rq);

	എങ്കിൽ (unlikely(irq >= max))
		തിരികെ 1;

	/*
	 * avg_rt.util_avg and avg_dl.util_avg track binary signals
	 * (running and not running) with weights 0 and 1024 respectively.
	 */
	used = cpu_util_rt(rq);
	used += cpu_util_dl(rq);

	എങ്കിൽ (unlikely(used >= max))
		തിരികെ 1;

	സ്വതന്ത്രം = max - used;

	തിരികെ scale_irq_capacity(സ്വതന്ത്രം, irq, max);
}

സ്ഥിതം ശൂന്യം update_cpu_capacity(ഘടന sched_domain *sd, പൂർണ്ണസംഖ്യ cpu)
{
	unsigned long capacity = scale_rt_capacity(cpu);
	ഘടന sched_group *sdg = sd->groups;

	എങ്കിൽ (!capacity)
		capacity = 1;

	cpu_rq(cpu)->cpu_capacity = capacity;
	trace_sched_cpu_capacity_tp(cpu_rq(cpu));

	sdg->sgc->capacity = capacity;
	sdg->sgc->min_capacity = capacity;
	sdg->sgc->max_capacity = capacity;
}

ശൂന്യം update_group_capacity(ഘടന sched_domain *sd, പൂർണ്ണസംഖ്യ cpu)
{
	ഘടന sched_domain *child = sd->child;
	ഘടന sched_group *group, *sdg = sd->groups;
	unsigned long capacity, min_capacity, max_capacity;
	unsigned long interval;

	interval = msecs_to_jiffies(sd->balance_interval);
	interval = clamp(interval, 1UL, max_load_balance_interval);
	sdg->sgc->next_update = jiffies + interval;

	എങ്കിൽ (!child) {
		update_cpu_capacity(sd, cpu);
		തിരികെ;
	}

	capacity = 0;
	min_capacity = ULONG_MAX;
	max_capacity = 0;

	എങ്കിൽ (child->flags & SD_NUMA) {
		/*
		 * SD_NUMA domains cannot assume that child groups
		 * span the current group.
		 */

		for_each_cpu(cpu, sched_group_span(sdg)) {
			unsigned long cpu_cap = capacity_of(cpu);

			capacity += cpu_cap;
			min_capacity = min(cpu_cap, min_capacity);
			max_capacity = max(cpu_cap, max_capacity);
		}
	} അല്ലെങ്കിൽ  {
		/*
		 * !SD_NUMA domains can assume that child groups
		 * span the current group.
		 */

		group = child->groups;
		ചെയ്യുക {
			ഘടന sched_group_capacity *sgc = group->sgc;

			capacity += sgc->capacity;
			min_capacity = min(sgc->min_capacity, min_capacity);
			max_capacity = max(sgc->max_capacity, max_capacity);
			group = group->next;
		} ആകുമ്പോൾ (group != child->groups);
	}

	sdg->sgc->capacity = capacity;
	sdg->sgc->min_capacity = min_capacity;
	sdg->sgc->max_capacity = max_capacity;
}

/*
 * Check whether the capacity of the rq has been noticeably reduced by side
 * activity. The imbalance_pct is used for the threshold.
 * Return true is the capacity is reduced
 */
സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
check_cpu_capacity(ഘടന rq *rq, ഘടന sched_domain *sd)
{
	തിരികെ ((rq->cpu_capacity * sd->imbalance_pct) <
				(arch_scale_cpu_capacity(cpu_of(rq)) * 100));
}

/* Check if the rq has a misfit task */
സ്ഥിതം വരിയിൽ bool check_misfit_status(ഘടന rq *rq)
{
	തിരികെ rq->misfit_task_load;
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

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ sg_imbalanced(ഘടന sched_group *group)
{
	തിരികെ group->sgc->imbalance;
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
സ്ഥിതം വരിയിൽ bool
group_has_capacity(unsigned പൂർണ്ണസംഖ്യ imbalance_pct, ഘടന sg_lb_stats *sgs)
{
	എങ്കിൽ (sgs->sum_nr_running < sgs->group_weight)
		തിരികെ true;

	എങ്കിൽ ((sgs->group_capacity * imbalance_pct) <
			(sgs->group_runnable * 100))
		തിരികെ false;

	എങ്കിൽ ((sgs->group_capacity * 100) >
			(sgs->group_util * imbalance_pct))
		തിരികെ true;

	തിരികെ false;
}

/*
 *  group_is_overloaded returns true if the group has more tasks than it can
 *  handle.
 *  group_is_overloaded is not equals to !group_has_capacity because a group
 *  with the exact right number of tasks, has no more spare capacity but is not
 *  overloaded so both group_has_capacity and group_is_overloaded return
 *  false.
 */
സ്ഥിതം വരിയിൽ bool
group_is_overloaded(unsigned പൂർണ്ണസംഖ്യ imbalance_pct, ഘടന sg_lb_stats *sgs)
{
	/*
	 * With EAS and uclamp, 1 CPU in the group must be overutilized to
	 * consider the group overloaded.
	 */
	എങ്കിൽ (sched_energy_enabled() && !sgs->group_overutilized)
		തിരികെ false;

	എങ്കിൽ (sgs->sum_nr_running <= sgs->group_weight)
		തിരികെ false;

	എങ്കിൽ ((sgs->group_capacity * 100) <
			(sgs->group_util * imbalance_pct))
		തിരികെ true;

	എങ്കിൽ ((sgs->group_capacity * imbalance_pct) <
			(sgs->group_runnable * 100))
		തിരികെ true;

	തിരികെ false;
}

സ്ഥിതം വരിയിൽ എണ്ണൽ
group_type group_classify(unsigned പൂർണ്ണസംഖ്യ imbalance_pct,
			  ഘടന sched_group *group,
			  ഘടന sg_lb_stats *sgs)
{
	എങ്കിൽ (group_is_overloaded(imbalance_pct, sgs))
		തിരികെ group_overloaded;

	എങ്കിൽ (sgs->group_llc_balance)
		തിരികെ group_llc_balance;

	എങ്കിൽ (sg_imbalanced(group))
		തിരികെ group_imbalanced;

	എങ്കിൽ (sgs->group_asym_packing)
		തിരികെ group_asym_packing;

	എങ്കിൽ (sgs->group_smt_balance)
		തിരികെ group_smt_balance;

	എങ്കിൽ (sgs->group_misfit_task_load)
		തിരികെ group_misfit_task;

	എങ്കിൽ (!group_has_capacity(imbalance_pct, sgs))
		തിരികെ group_fully_busy;

	തിരികെ group_has_spare;
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
സ്ഥിതം bool sched_use_asym_prio(ഘടന sched_domain *sd, പൂർണ്ണസംഖ്യ cpu)
{
	എങ്കിൽ (!(sd->flags & SD_ASYM_PACKING))
		തിരികെ false;

	എങ്കിൽ (!sched_smt_active())
		തിരികെ true;

	തിരികെ sd->flags & SD_SHARE_CPUCAPACITY || is_core_idle(cpu);
}

സ്ഥിതം വരിയിൽ bool sched_asym(ഘടന sched_domain *sd, പൂർണ്ണസംഖ്യ dst_cpu, പൂർണ്ണസംഖ്യ src_cpu)
{
	/*
	 * First check if @dst_cpu can do asym_packing load balance. Only do it
	 * if it has higher priority than @src_cpu.
	 */
	തിരികെ sched_use_asym_prio(sd, dst_cpu) &&
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
സ്ഥിതം വരിയിൽ bool
sched_group_asym(ഘടന lb_env *env, ഘടന sg_lb_stats *sgs, ഘടന sched_group *group)
{
	/*
	 * CPU priorities do not make sense for SMT cores with more than one
	 * busy sibling.
	 */
	എങ്കിൽ ((group->flags & SD_SHARE_CPUCAPACITY) &&
	    (sgs->group_weight - sgs->idle_cpus != 1))
		തിരികെ false;

	തിരികെ sched_asym(env->sd, env->dst_cpu, READ_ONCE(group->asym_prefer_cpu));
}

/* One group has more than one SMT CPU while the other group does not */
സ്ഥിതം വരിയിൽ bool smt_vs_nonsmt_groups(ഘടന sched_group *sg1,
				    ഘടന sched_group *sg2)
{
	എങ്കിൽ (!sg1 || !sg2)
		തിരികെ false;

	തിരികെ (sg1->flags & SD_SHARE_CPUCAPACITY) !=
		(sg2->flags & SD_SHARE_CPUCAPACITY);
}

സ്ഥിതം വരിയിൽ bool smt_balance(ഘടന lb_env *env, ഘടന sg_lb_stats *sgs,
			       ഘടന sched_group *group)
{
	എങ്കിൽ (!env->idle)
		തിരികെ false;

	/*
	 * For SMT source group, it is better to move a task
	 * to a CPU that doesn't have multiple tasks sharing its CPU capacity.
	 * Note that if a group has a single SMT, SD_SHARE_CPUCAPACITY
	 * will not be on.
	 */
	എങ്കിൽ (group->flags & SD_SHARE_CPUCAPACITY &&
	    sgs->sum_h_nr_running > 1)
		തിരികെ true;

	തിരികെ false;
}

സ്ഥിതം വരിയിൽ long sibling_imbalance(ഘടന lb_env *env,
				    ഘടന sd_lb_stats *sds,
				    ഘടന sg_lb_stats *busiest,
				    ഘടന sg_lb_stats *local)
{
	പൂർണ്ണസംഖ്യ ncores_busiest, ncores_local;
	long imbalance;

	എങ്കിൽ (!env->idle || !busiest->sum_nr_running)
		തിരികെ 0;

	ncores_busiest = sds->busiest->cores;
	ncores_local = sds->local->cores;

	എങ്കിൽ (ncores_busiest == ncores_local) {
		imbalance = busiest->sum_nr_running;
		lsub_positive(&imbalance, local->sum_nr_running);
		തിരികെ imbalance;
	}

	/* Balance such that nr_running/ncores ratio are same on both groups */
	imbalance = ncores_local * busiest->sum_nr_running;
	lsub_positive(&imbalance, ncores_busiest * local->sum_nr_running);
	/* Normalize imbalance and do rounding on normalization */
	imbalance = 2 * imbalance + ncores_local + ncores_busiest;
	imbalance /= ncores_local + ncores_busiest;

	/* Take advantage of resource in an empty sched group */
	എങ്കിൽ (imbalance <= 1 && local->sum_nr_running == 0 &&
	    busiest->sum_nr_running > 1)
		imbalance = 2;

	തിരികെ imbalance;
}

സ്ഥിതം വരിയിൽ bool
sched_reduced_capacity(ഘടന rq *rq, ഘടന sched_domain *sd)
{
	/*
	 * When there is more than 1 task, the group_overloaded case already
	 * takes care of cpu with reduced capacity
	 */
	എങ്കിൽ (rq->cfs.h_nr_runnable != 1)
		തിരികെ false;

	തിരികെ check_cpu_capacity(rq, sd);
}

#ifdef CONFIG_SCHED_CACHE
/*
 * Record the statistics for this scheduler group for later
 * use. These values guide load balancing on aggregating tasks
 * to a LLC.
 */
സ്ഥിതം ശൂന്യം record_sg_llc_stats(ഘടന lb_env *env,
				ഘടന sg_lb_stats *sgs,
				ഘടന sched_group *group)
{
	ഘടന sched_domain_shared *sd_share;
	പൂർണ്ണസംഖ്യ cpu;

	എങ്കിൽ (!sched_cache_enabled() || env->idle == CPU_NEWLY_IDLE)
		തിരികെ;

	/* Only care about sched domain spanning multiple LLCs */
	എങ്കിൽ (env->sd->child != rcu_dereference_all(per_cpu(sd_llc, env->dst_cpu)))
		തിരികെ;

	/*
	 * A redo repeats stats published on the first pass. Hotplug can
	 * still leave env->cpus partial, so util would not match capacity.
	 */
	എങ്കിൽ ((env->flags & LBF_ALL_PINNED) ||
	    !cpumask_subset(sched_group_span(group), env->cpus))
		തിരികെ;

	/*
	 * At this point we know this group spans a LLC domain.
	 * Record the statistic of this group in its corresponding
	 * shared LLC domain.
	 * Note: sd_share cannot be obtained via sd->child->shared,
	 * because the latter refers to the domain that covers the
	 * local group. Instead, sd_share should be located using
	 * the first CPU of the LLC group.
	 */
	cpu = cpumask_first(sched_group_span(group));
	sd_share = rcu_dereference_all(per_cpu(sd_llc_shared, cpu));
	എങ്കിൽ (!sd_share)
		തിരികെ;

	എങ്കിൽ (READ_ONCE(sd_share->util_avg) != sgs->group_util)
		WRITE_ONCE(sd_share->util_avg, sgs->group_util);

	എങ്കിൽ (unlikely(READ_ONCE(sd_share->capacity) != sgs->group_capacity))
		WRITE_ONCE(sd_share->capacity, sgs->group_capacity);
}

/*
 * Do LLC balance on sched group that contains LLC, and have tasks preferring
 * to run on LLC in idle dst_cpu.
 */
സ്ഥിതം വരിയിൽ bool llc_balance(ഘടന lb_env *env, ഘടന sg_lb_stats *sgs,
			       ഘടന sched_group *group)
{
	എങ്കിൽ (!sched_cache_enabled())
		തിരികെ false;

	എങ്കിൽ (env->sd->flags & SD_SHARE_LLC)
		തിരികെ false;

	/*
	 * On asymmetric domains, group_misfit_task_load
	 * should be prioritized to move tasks to CPU that fit them
	 * over aggregating tasks to their preferred LLC.
	 */
	എങ്കിൽ ((env->sd->flags & SD_ASYM_CPUCAPACITY) &&
	    sgs->group_misfit_task_load)
		തിരികെ false;

	/*
	 * Skip cache aware tagging if nr_balanced_failed is sufficiently high.
	 * Threshold of cache_nice_tries is set to 1 higher than nr_balance_failed
	 * to avoid excessive task migration at the same time.
	 */
	എങ്കിൽ (env->sd->nr_balance_failed >= env->sd->cache_nice_tries + 1)
		തിരികെ false;

	എങ്കിൽ (sgs->nr_pref_dst_llc &&
	    can_migrate_llc(cpumask_first(sched_group_span(group)),
			    env->dst_cpu, 0, true) == mig_llc)
		തിരികെ true;

	തിരികെ false;
}

സ്ഥിതം bool update_llc_busiest(ഘടന lb_env *env,
			       ഘടന sg_lb_stats *busiest,
			       ഘടന sg_lb_stats *sgs)
{
	/*
	 * There are more tasks that want to run on dst_cpu's LLC.
	 */
	തിരികെ sgs->nr_pref_dst_llc > busiest->nr_pref_dst_llc;
}
#അല്ലെങ്കിൽ
സ്ഥിതം വരിയിൽ ശൂന്യം record_sg_llc_stats(ഘടന lb_env *env, ഘടന sg_lb_stats *sgs,
				       ഘടന sched_group *group)
{
}

സ്ഥിതം വരിയിൽ bool llc_balance(ഘടന lb_env *env, ഘടന sg_lb_stats *sgs,
			       ഘടന sched_group *group)
{
	തിരികെ false;
}

സ്ഥിതം bool update_llc_busiest(ഘടന lb_env *env,
			       ഘടന sg_lb_stats *busiest,
			       ഘടന sg_lb_stats *sgs)
{
	തിരികെ false;
}
#endif

/**
 * update_sg_lb_stats - Update sched_group's statistics for load balancing.
 * @env: The load balancing environment.
 * @sds: Load-balancing data with statistics of the local group.
 * @group: sched_group whose statistics are to be updated.
 * @sgs: variable to hold the statistics for this group.
 * @sg_overloaded: sched_group is overloaded
 */
സ്ഥിതം വരിയിൽ ശൂന്യം update_sg_lb_stats(ഘടന lb_env *env,
				      ഘടന sd_lb_stats *sds,
				      ഘടന sched_group *group,
				      ഘടന sg_lb_stats *sgs,
				      bool *sg_overloaded)
{
	പൂർണ്ണസംഖ്യ i, nr_running, local_group, sd_flags = env->sd->flags;
	bool balancing_at_rd = !env->sd->parent;

	memset(sgs, 0, വലുപ്പം(*sgs));

	local_group = group == sds->local;

	for_each_cpu_and(i, sched_group_span(group), env->cpus) {
		ഘടന rq *rq = cpu_rq(i);
		unsigned long load = cpu_load(rq);

		sgs->group_load += load;
		sgs->group_util += cpu_util_cfs(i);
		sgs->group_runnable += cpu_runnable(rq);
		sgs->sum_h_nr_running += rq->cfs.h_nr_runnable;

		nr_running = rq->nr_running;
		sgs->sum_nr_running += nr_running;

		എങ്കിൽ (cpu_overutilized(i))
			sgs->group_overutilized = 1;

#ifdef CONFIG_SCHED_CACHE
		എങ്കിൽ (sched_cache_enabled()) {
			ഘടന sched_domain *sd_tmp;
			പൂർണ്ണസംഖ്യ dst_llc;

			dst_llc = llc_id(env->dst_cpu);
			എങ്കിൽ (llc_id(i) != dst_llc) {
				sd_tmp = rcu_dereference_all(rq->sd);
				എങ്കിൽ (sd_tmp && (unsigned പൂർണ്ണസംഖ്യ)dst_llc < sd_tmp->llc_max)
					sgs->nr_pref_dst_llc += sd_tmp->llc_counts[dst_llc];
			}
		}
#endif

		/*
		 * No need to call idle_cpu() if nr_running is not 0
		 */
		എങ്കിൽ (!nr_running && idle_cpu(i)) {
			sgs->idle_cpus++;
			/* Idle cpu can't have misfit task */
			തുടരുക;
		}

		/* Overload indicator is only updated at root domain */
		എങ്കിൽ (balancing_at_rd && nr_running > 1)
			*sg_overloaded = 1;

#ifdef CONFIG_NUMA_BALANCING
		/* Only fbq_classify_group() uses this to classify NUMA groups */
		എങ്കിൽ (sd_flags & SD_NUMA) {
			sgs->nr_numa_running += rq->nr_numa_running;
			sgs->nr_preferred_running += rq->nr_preferred_running;
		}
#endif
		എങ്കിൽ (local_group)
			തുടരുക;

		എങ്കിൽ (sd_flags & SD_ASYM_CPUCAPACITY) {
			എങ്കിൽ (rq->misfit_task_load) {
				/*
				 * Always mark the root domain overloaded so big
				 * CPUs can pick up misfit tasks via newly idle
				 * balance.
				 */
				എങ്കിൽ (balancing_at_rd)
					*sg_overloaded = 1;

				/*
				 * Only account misfit load if @dst_cpu can
				 * help; otherwise, the group may be classified
				 * as misfit_task and update_sd_pick_busiest()
				 * will skip it.
				 */
				എങ്കിൽ (capacity_greater(capacity_of(env->dst_cpu),
						     group->sgc->max_capacity) &&
				    (sgs->group_misfit_task_load < rq->misfit_task_load))
					sgs->group_misfit_task_load = rq->misfit_task_load;
			}
		} അല്ലെങ്കിൽ എങ്കിൽ (env->idle && sched_reduced_capacity(rq, env->sd)) {
			/* Check for a task running on a CPU with reduced capacity */
			എങ്കിൽ (sgs->group_misfit_task_load < load)
				sgs->group_misfit_task_load = load;
		}
	}

	sgs->group_capacity = group->sgc->capacity;

	sgs->group_weight = group->group_weight;

	എങ്കിൽ (!local_group) {
		/* Check if dst CPU is idle and preferred to this group */
		എങ്കിൽ (env->idle && sgs->sum_h_nr_running &&
		    sched_group_asym(env, sgs, group))
			sgs->group_asym_packing = 1;

		/* Check for loaded SMT group to be balanced to dst CPU */
		എങ്കിൽ (smt_balance(env, sgs, group))
			sgs->group_smt_balance = 1;

		/* Check for tasks in this group can be moved to their preferred LLC */
		എങ്കിൽ (llc_balance(env, sgs, group))
			sgs->group_llc_balance = 1;
	}

	sgs->group_type = group_classify(env->sd->imbalance_pct, group, sgs);

	record_sg_llc_stats(env, sgs, group);
	/* Computing avg_load makes sense only when group is overloaded */
	എങ്കിൽ (sgs->group_type == group_overloaded)
		sgs->avg_load = (sgs->group_load * SCHED_CAPACITY_SCALE) /
				sgs->group_capacity;
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
സ്ഥിതം bool update_sd_pick_busiest(ഘടന lb_env *env,
				   ഘടന sd_lb_stats *sds,
				   ഘടന sched_group *sg,
				   ഘടന sg_lb_stats *sgs)
{
	ഘടന sg_lb_stats *busiest = &sds->busiest_stat;

	/* Make sure that there is at least one task to pull */
	എങ്കിൽ (!sgs->sum_h_nr_running)
		തിരികെ false;

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
	എങ്കിൽ ((env->sd->flags & SD_ASYM_CPUCAPACITY) &&
	    (sgs->group_type == group_misfit_task) &&
	    (!env->dst_core_idle ||
	     !capacity_greater(capacity_of(env->dst_cpu), sg->sgc->max_capacity) ||
	     sds->local_stat.group_type != group_has_spare))
		തിരികെ false;

	/*
	 * Candidate sg has no more than one task per CPU and has higher
	 * per-CPU capacity. Migrating tasks to less capable CPUs may harm
	 * throughput. Maximize throughput, power/energy consequences are not
	 * considered.
	 */
	എങ്കിൽ ((env->sd->flags & SD_ASYM_CPUCAPACITY) &&
	    (sgs->group_type <= group_fully_busy) &&
	    (capacity_greater(sg->sgc->min_capacity, capacity_of(env->dst_cpu))))
		തിരികെ false;

	എങ്കിൽ (sgs->group_type > busiest->group_type)
		തിരികെ true;

	എങ്കിൽ (sgs->group_type < busiest->group_type)
		തിരികെ false;

	/*
	 * The candidate and the current busiest group are the same type of
	 * group. Let check which one is the busiest according to the type.
	 */

	തിരഞ്ഞെടുപ്പ് (sgs->group_type) {
	അവസ്ഥ group_overloaded:
		/* Select the overloaded group with highest avg_load. */
		തിരികെ sgs->avg_load > busiest->avg_load;

	അവസ്ഥ group_llc_balance:
		/* Select the group with most tasks preferring dst LLC */
		തിരികെ update_llc_busiest(env, busiest, sgs);

	അവസ്ഥ group_imbalanced:
		/*
		 * Select the 1st imbalanced group as we don't have any way to
		 * choose one more than another.
		 */
		തിരികെ false;

	അവസ്ഥ group_asym_packing:
		/* Prefer to move from lowest priority CPU's work */
		തിരികെ sched_asym_prefer(READ_ONCE(sds->busiest->asym_prefer_cpu),
					 READ_ONCE(sg->asym_prefer_cpu));

	അവസ്ഥ group_misfit_task:
		/*
		 * If we have more than one misfit sg go with the biggest
		 * misfit.
		 */
		തിരികെ sgs->group_misfit_task_load > busiest->group_misfit_task_load;

	അവസ്ഥ group_smt_balance:
		/*
		 * Check if we have spare CPUs on either SMT group to
		 * choose has spare or fully busy handling.
		 */
		എങ്കിൽ (sgs->idle_cpus != 0 || busiest->idle_cpus != 0)
			പോകുക has_spare;

		fallthrough;

	അവസ്ഥ group_fully_busy:
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

		എങ്കിൽ (sgs->avg_load < busiest->avg_load)
			തിരികെ false;

		എങ്കിൽ (sgs->avg_load == busiest->avg_load) {
			/*
			 * SMT sched groups need more help than non-SMT groups.
			 * If @sg happens to also be SMT, either choice is good.
			 */
			എങ്കിൽ (sds->busiest->flags & SD_SHARE_CPUCAPACITY)
				തിരികെ false;
		}

		നിർത്തുക;

	അവസ്ഥ group_has_spare:
		/*
		 * Do not pick sg with SMT CPUs over sg with pure CPUs,
		 * as we do not want to pull task off SMT core with one task
		 * and make the core idle.
		 */
		എങ്കിൽ (smt_vs_nonsmt_groups(sds->busiest, sg)) {
			എങ്കിൽ (sg->flags & SD_SHARE_CPUCAPACITY && sgs->sum_h_nr_running <= 1)
				തിരികെ false;
			അല്ലെങ്കിൽ
				തിരികെ true;
		}
has_spare:

		/*
		 * Select not overloaded group with lowest number of idle CPUs
		 * and highest number of running tasks. We could also compare
		 * the spare capacity which is more stable but it can end up
		 * that the group has less spare capacity but finally more idle
		 * CPUs which means less opportunity to pull tasks.
		 */
		എങ്കിൽ (sgs->idle_cpus > busiest->idle_cpus)
			തിരികെ false;
		അല്ലെങ്കിൽ എങ്കിൽ ((sgs->idle_cpus == busiest->idle_cpus) &&
			 (sgs->sum_nr_running <= busiest->sum_nr_running))
			തിരികെ false;

		നിർത്തുക;
	}

	തിരികെ true;
}

#ifdef CONFIG_NUMA_BALANCING
സ്ഥിതം വരിയിൽ എണ്ണൽ fbq_type fbq_classify_group(ഘടന sg_lb_stats *sgs)
{
	എങ്കിൽ (sgs->sum_h_nr_running > sgs->nr_numa_running)
		തിരികെ regular;
	എങ്കിൽ (sgs->sum_h_nr_running > sgs->nr_preferred_running)
		തിരികെ remote;
	തിരികെ all;
}

സ്ഥിതം വരിയിൽ എണ്ണൽ fbq_type fbq_classify_rq(ഘടന rq *rq)
{
	എങ്കിൽ (rq->nr_running > rq->nr_numa_running)
		തിരികെ regular;
	എങ്കിൽ (rq->nr_running > rq->nr_preferred_running)
		തിരികെ remote;
	തിരികെ all;
}
#അല്ലെങ്കിൽ /* !CONFIG_NUMA_BALANCING: */
സ്ഥിതം വരിയിൽ എണ്ണൽ fbq_type fbq_classify_group(ഘടന sg_lb_stats *sgs)
{
	തിരികെ all;
}

സ്ഥിതം വരിയിൽ എണ്ണൽ fbq_type fbq_classify_rq(ഘടന rq *rq)
{
	തിരികെ regular;
}
#endif /* !CONFIG_NUMA_BALANCING */


ഘടന sg_lb_stats;

/*
 * task_running_on_cpu - return 1 if @p is running on @cpu.
 */

സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ task_running_on_cpu(പൂർണ്ണസംഖ്യ cpu, ഘടന task_struct *p)
{
	/* Task has no contribution or is new */
	എങ്കിൽ (cpu != task_cpu(p) || !READ_ONCE(p->se.avg.last_update_time))
		തിരികെ 0;

	എങ്കിൽ (task_on_rq_queued(p))
		തിരികെ 1;

	തിരികെ 0;
}

/**
 * idle_cpu_without - would a given CPU be idle without p ?
 * @cpu: the processor on which idleness is tested.
 * @p: task which should be ignored.
 *
 * Return: 1 if the CPU would be idle. 0 otherwise.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ idle_cpu_without(പൂർണ്ണസംഖ്യ cpu, ഘടന task_struct *p)
{
	ഘടന rq *rq = cpu_rq(cpu);

	എങ്കിൽ (rq->curr != rq->idle && rq->curr != p)
		തിരികെ 0;

	/*
	 * rq->nr_running can't be used but an updated version without the
	 * impact of p on cpu must be used instead. The updated nr_running
	 * be computed and tested before calling idle_cpu_without().
	 */

	എങ്കിൽ (rq->ttwu_pending)
		തിരികെ 0;

	തിരികെ 1;
}

/*
 * update_sg_wakeup_stats - Update sched_group's statistics for wakeup.
 * @sd: The sched_domain level to look for idlest group.
 * @group: sched_group whose statistics are to be updated.
 * @sgs: variable to hold the statistics for this group.
 * @p: The task for which we look for the idlest group/CPU.
 */
സ്ഥിതം വരിയിൽ ശൂന്യം update_sg_wakeup_stats(ഘടന sched_domain *sd,
					  ഘടന sched_group *group,
					  ഘടന sg_lb_stats *sgs,
					  ഘടന task_struct *p)
{
	പൂർണ്ണസംഖ്യ i, nr_running;

	memset(sgs, 0, വലുപ്പം(*sgs));

	/* Assume that task can't fit any CPU of the group */
	എങ്കിൽ (sd->flags & SD_ASYM_CPUCAPACITY)
		sgs->group_misfit_task_load = 1;

	for_each_cpu_and(i, sched_group_span(group), p->cpus_ptr) {
		ഘടന rq *rq = cpu_rq(i);
		unsigned പൂർണ്ണസംഖ്യ local;

		sgs->group_load += cpu_load_without(rq, p);
		sgs->group_util += cpu_util_without(i, p);
		sgs->group_runnable += cpu_runnable_without(rq, p);
		local = task_running_on_cpu(i, p);
		sgs->sum_h_nr_running += rq->cfs.h_nr_runnable - local;

		nr_running = rq->nr_running - local;
		sgs->sum_nr_running += nr_running;

		/*
		 * No need to call idle_cpu_without() if nr_running is not 0
		 */
		എങ്കിൽ (!nr_running && idle_cpu_without(i, p))
			sgs->idle_cpus++;

		/* Check if task fits in the CPU */
		എങ്കിൽ (sd->flags & SD_ASYM_CPUCAPACITY &&
		    sgs->group_misfit_task_load &&
		    task_fits_cpu(p, i))
			sgs->group_misfit_task_load = 0;

	}

	sgs->group_capacity = group->sgc->capacity;

	sgs->group_weight = group->group_weight;

	sgs->group_type = group_classify(sd->imbalance_pct, group, sgs);

	/*
	 * Computing avg_load makes sense only when group is fully busy or
	 * overloaded
	 */
	എങ്കിൽ (sgs->group_type == group_fully_busy ||
		sgs->group_type == group_overloaded)
		sgs->avg_load = (sgs->group_load * SCHED_CAPACITY_SCALE) /
				sgs->group_capacity;
}

സ്ഥിതം bool update_pick_idlest(ഘടന sched_group *idlest,
			       ഘടന sg_lb_stats *idlest_sgs,
			       ഘടന sched_group *group,
			       ഘടന sg_lb_stats *sgs)
{
	എങ്കിൽ (sgs->group_type < idlest_sgs->group_type)
		തിരികെ true;

	എങ്കിൽ (sgs->group_type > idlest_sgs->group_type)
		തിരികെ false;

	/*
	 * The candidate and the current idlest group are the same type of
	 * group. Let check which one is the idlest according to the type.
	 */

	തിരഞ്ഞെടുപ്പ് (sgs->group_type) {
	അവസ്ഥ group_overloaded:
	അവസ്ഥ group_fully_busy:
		/* Select the group with lowest avg_load. */
		എങ്കിൽ (idlest_sgs->avg_load <= sgs->avg_load)
			തിരികെ false;
		നിർത്തുക;

	അവസ്ഥ group_llc_balance:
	അവസ്ഥ group_imbalanced:
	അവസ്ഥ group_asym_packing:
	അവസ്ഥ group_smt_balance:
		/* Those types are not used in the slow wakeup path */
		തിരികെ false;

	അവസ്ഥ group_misfit_task:
		/* Select group with the highest max capacity */
		എങ്കിൽ (idlest->sgc->max_capacity >= group->sgc->max_capacity)
			തിരികെ false;
		നിർത്തുക;

	അവസ്ഥ group_has_spare:
		/* Select group with most idle CPUs */
		എങ്കിൽ (idlest_sgs->idle_cpus > sgs->idle_cpus)
			തിരികെ false;

		/* Select group with lowest group_util */
		എങ്കിൽ (idlest_sgs->idle_cpus == sgs->idle_cpus &&
			idlest_sgs->group_util <= sgs->group_util)
			തിരികെ false;

		നിർത്തുക;
	}

	തിരികെ true;
}

/*
 * sched_balance_find_dst_group() finds and returns the least busy CPU group within the
 * domain.
 *
 * Assumes p is allowed on at least one CPU in sd.
 */
സ്ഥിതം ഘടന sched_group *
sched_balance_find_dst_group(ഘടന sched_domain *sd, ഘടന task_struct *p, പൂർണ്ണസംഖ്യ this_cpu)
{
	ഘടന sched_group *idlest = ശൂന്യ, *local = ശൂന്യ, *group = sd->groups;
	ഘടന sg_lb_stats local_sgs, tmp_sgs;
	ഘടന sg_lb_stats *sgs;
	unsigned long imbalance;
	ഘടന sg_lb_stats idlest_sgs = {
			.avg_load = UINT_MAX,
			.group_type = group_overloaded,
	};

	ചെയ്യുക {
		പൂർണ്ണസംഖ്യ local_group;

		/* Skip over this group if it has no CPUs allowed */
		എങ്കിൽ (!cpumask_intersects(sched_group_span(group),
					p->cpus_ptr))
			തുടരുക;

		/* Skip over this group if no cookie matched */
		എങ്കിൽ (!sched_group_cookie_match(cpu_rq(this_cpu), p, group))
			തുടരുക;

		local_group = cpumask_test_cpu(this_cpu,
					       sched_group_span(group));

		എങ്കിൽ (local_group) {
			sgs = &local_sgs;
			local = group;
		} അല്ലെങ്കിൽ {
			sgs = &tmp_sgs;
		}

		update_sg_wakeup_stats(sd, group, sgs, p);

		എങ്കിൽ (!local_group && update_pick_idlest(idlest, &idlest_sgs, group, sgs)) {
			idlest = group;
			idlest_sgs = *sgs;
		}

	} ആകുമ്പോൾ (group = group->next, group != sd->groups);


	/* There is no idlest group to push tasks to */
	എങ്കിൽ (!idlest)
		തിരികെ ശൂന്യ;

	/* The local group has been skipped because of CPU affinity */
	എങ്കിൽ (!local)
		തിരികെ idlest;

	/*
	 * If the local group is idler than the selected idlest group
	 * don't try and push the task.
	 */
	എങ്കിൽ (local_sgs.group_type < idlest_sgs.group_type)
		തിരികെ ശൂന്യ;

	/*
	 * If the local group is busier than the selected idlest group
	 * try and push the task.
	 */
	എങ്കിൽ (local_sgs.group_type > idlest_sgs.group_type)
		തിരികെ idlest;

	തിരഞ്ഞെടുപ്പ് (local_sgs.group_type) {
	അവസ്ഥ group_overloaded:
	അവസ്ഥ group_fully_busy:

		/* Calculate allowed imbalance based on load */
		imbalance = scale_load_down(NICE_0_LOAD) *
				(sd->imbalance_pct-100) / 100;

		/*
		 * When comparing groups across NUMA domains, it's possible for
		 * the local domain to be very lightly loaded relative to the
		 * remote domains but "imbalance" skews the comparison making
		 * remote CPUs look much more favourable. When considering
		 * cross-domain, add imbalance to the load on the remote node
		 * and consider staying local.
		 */

		എങ്കിൽ ((sd->flags & SD_NUMA) &&
		    ((idlest_sgs.avg_load + imbalance) >= local_sgs.avg_load))
			തിരികെ ശൂന്യ;

		/*
		 * If the local group is less loaded than the selected
		 * idlest group don't try and push any tasks.
		 */
		എങ്കിൽ (idlest_sgs.avg_load >= (local_sgs.avg_load + imbalance))
			തിരികെ ശൂന്യ;

		എങ്കിൽ (100 * local_sgs.avg_load <= sd->imbalance_pct * idlest_sgs.avg_load)
			തിരികെ ശൂന്യ;
		നിർത്തുക;

	അവസ്ഥ group_llc_balance:
	അവസ്ഥ group_imbalanced:
	അവസ്ഥ group_asym_packing:
	അവസ്ഥ group_smt_balance:
		/* Those type are not used in the slow wakeup path */
		തിരികെ ശൂന്യ;

	അവസ്ഥ group_misfit_task:
		/* Select group with the highest max capacity */
		എങ്കിൽ (local->sgc->max_capacity >= idlest->sgc->max_capacity)
			തിരികെ ശൂന്യ;
		നിർത്തുക;

	അവസ്ഥ group_has_spare:
#ifdef CONFIG_NUMA
		എങ്കിൽ (sd->flags & SD_NUMA) {
			പൂർണ്ണസംഖ്യ imb_numa_nr = sd->imb_numa_nr;
#ifdef CONFIG_NUMA_BALANCING
			പൂർണ്ണസംഖ്യ idlest_cpu;
			/*
			 * If there is spare capacity at NUMA, try to select
			 * the preferred node
			 */
			എങ്കിൽ (cpu_to_കണ്ണി(this_cpu) == p->numa_preferred_nid)
				തിരികെ ശൂന്യ;

			idlest_cpu = cpumask_first(sched_group_span(idlest));
			എങ്കിൽ (cpu_to_കണ്ണി(idlest_cpu) == p->numa_preferred_nid)
				തിരികെ idlest;
#endif /* CONFIG_NUMA_BALANCING */
			/*
			 * Otherwise, keep the task close to the wakeup source
			 * and improve locality if the number of running tasks
			 * would remain below threshold where an imbalance is
			 * allowed while accounting for the possibility the
			 * task is pinned to a subset of CPUs. If there is a
			 * real need of migration, periodic load balance will
			 * take care of it.
			 */
			എങ്കിൽ (p->nr_cpus_allowed != NR_CPUS) {
				unsigned പൂർണ്ണസംഖ്യ w = cpumask_weight_and(p->cpus_ptr,
								sched_group_span(local));
				imb_numa_nr = min(w, sd->imb_numa_nr);
			}

			imbalance = abs(local_sgs.idle_cpus - idlest_sgs.idle_cpus);
			എങ്കിൽ (!adjust_numa_imbalance(imbalance,
						   local_sgs.sum_nr_running + 1,
						   imb_numa_nr)) {
				തിരികെ ശൂന്യ;
			}
		}
#endif /* CONFIG_NUMA */

		/*
		 * Select group with highest number of idle CPUs. We could also
		 * compare the utilization which is more stable but it can end
		 * up that the group has less spare capacity but finally more
		 * idle CPUs which means more opportunity to run task.
		 */
		എങ്കിൽ (local_sgs.idle_cpus >= idlest_sgs.idle_cpus)
			തിരികെ ശൂന്യ;
		നിർത്തുക;
	}

	തിരികെ idlest;
}

സ്ഥിതം ശൂന്യം update_idle_cpu_scan(ഘടന lb_env *env,
				 unsigned long sum_util)
{
	ഘടന sched_domain_shared *sd_share;
	ഘടന sched_domain *sd = env->sd;
	പൂർണ്ണസംഖ്യ llc_weight, pct;
	u64 x, y, tmp;
	/*
	 * Update the number of CPUs to scan in LLC domain, which could
	 * be used as a hint in select_idle_cpu(). The update of sd_share
	 * could be expensive because it is within a shared cache line.
	 * So the write of this hint only occurs during periodic load
	 * balancing, rather than CPU_NEWLY_IDLE, because the latter
	 * can fire way more frequently than the former.
	 */
	എങ്കിൽ (!sched_feat(SIS_UTIL) || env->idle == CPU_NEWLY_IDLE)
		തിരികെ;

	sd_share = sd->shared;
	എങ്കിൽ (!sd_share)
		തിരികെ;

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
	x = sum_util;
	llc_weight = sd->span_weight;
	do_div(x, llc_weight);

	/* equation [4] */
	pct = sd->imbalance_pct;
	tmp = x * x * pct * pct;
	do_div(tmp, 10000 * SCHED_CAPACITY_SCALE);
	tmp = min_t(long, tmp, SCHED_CAPACITY_SCALE);
	y = SCHED_CAPACITY_SCALE - tmp;

	/* equation [2] */
	y *= llc_weight;
	do_div(y, SCHED_CAPACITY_SCALE);
	എങ്കിൽ ((പൂർണ്ണസംഖ്യ)y != sd_share->nr_idle_scan)
		WRITE_ONCE(sd_share->nr_idle_scan, (പൂർണ്ണസംഖ്യ)y);
}

/**
 * update_sd_lb_stats - Update sched_domain's statistics for load balancing.
 * @env: The load balancing environment.
 * @sds: variable to hold the statistics for this sched_domain.
 */

സ്ഥിതം വരിയിൽ ശൂന്യം update_sd_lb_stats(ഘടന lb_env *env, ഘടന sd_lb_stats *sds)
{
	ഘടന sched_group *sg = env->sd->groups;
	ഘടന sg_lb_stats *local = &sds->local_stat;
	ഘടന sg_lb_stats tmp_sgs;
	unsigned long sum_util = 0;
	bool sg_overloaded = 0, sg_overutilized = 0;

	env->dst_core_idle = !sched_smt_active() || is_core_idle(env->dst_cpu);

	ചെയ്യുക {
		ഘടന sg_lb_stats *sgs = &tmp_sgs;
		പൂർണ്ണസംഖ്യ local_group;

		local_group = cpumask_test_cpu(env->dst_cpu, sched_group_span(sg));
		എങ്കിൽ (local_group) {
			sds->local = sg;
			sgs = local;

			എങ്കിൽ (env->idle != CPU_NEWLY_IDLE ||
			    time_after_eq(jiffies, sg->sgc->next_update))
				update_group_capacity(env->sd, env->dst_cpu);
		}

		update_sg_lb_stats(env, sds, sg, sgs, &sg_overloaded);

		എങ്കിൽ (!local_group && update_sd_pick_busiest(env, sds, sg, sgs)) {
			sds->busiest = sg;
			sds->busiest_stat = *sgs;
		}

		sg_overutilized |= sgs->group_overutilized;

		/* Now, start updating sd_lb_stats */
		sds->total_load += sgs->group_load;
		sds->total_capacity += sgs->group_capacity;

		sum_util += sgs->group_util;
		sg = sg->next;
	} ആകുമ്പോൾ (sg != env->sd->groups);

	/*
	 * Indicate that the child domain of the busiest group prefers tasks
	 * go to a child's sibling domains first. NB the flags of a sched group
	 * are those of the child domain.
	 */
	എങ്കിൽ (sds->busiest)
		sds->prefer_sibling = !!(sds->busiest->flags & SD_PREFER_SIBLING);


	എങ്കിൽ (env->sd->flags & SD_NUMA)
		env->fbq_type = fbq_classify_group(&sds->busiest_stat);

	എങ്കിൽ (!env->sd->parent) {
		/* update overload indicator if we are at root domain */
		set_rd_overloaded(env->dst_rq->rd, sg_overloaded);

		/* Update over-utilization (tipping point, U >= 0) indicator */
		set_rd_overutilized(env->dst_rq->rd, sg_overutilized);
	} അല്ലെങ്കിൽ എങ്കിൽ (sg_overutilized) {
		set_rd_overutilized(env->dst_rq->rd, sg_overutilized);
	}

	update_idle_cpu_scan(env, sum_util);
}

/**
 * calculate_imbalance - Calculate the amount of imbalance present within the
 *			 groups of a given sched_domain during load balance.
 * @env: load balance environment
 * @sds: statistics of the sched_domain whose imbalance is to be calculated.
 */
സ്ഥിതം വരിയിൽ ശൂന്യം calculate_imbalance(ഘടന lb_env *env, ഘടന sd_lb_stats *sds)
{
	ഘടന sg_lb_stats *local, *busiest;

	local = &sds->local_stat;
	busiest = &sds->busiest_stat;

	എങ്കിൽ (busiest->group_type == group_misfit_task) {
		എങ്കിൽ (env->sd->flags & SD_ASYM_CPUCAPACITY) {
			/* Set imbalance to allow misfit tasks to be balanced. */
			env->migration_type = migrate_misfit;
			env->imbalance = 1;
		} അല്ലെങ്കിൽ {
			/*
			 * Set load imbalance to allow moving task from cpu
			 * with reduced capacity.
			 */
			env->migration_type = migrate_load;
			env->imbalance = busiest->group_misfit_task_load;
		}
		തിരികെ;
	}

	എങ്കിൽ (busiest->group_type == group_asym_packing) {
		/*
		 * In case of asym capacity, we will try to migrate all load to
		 * the preferred CPU.
		 */
		env->migration_type = migrate_task;
		env->imbalance = busiest->sum_h_nr_running;
		തിരികെ;
	}

	എങ്കിൽ (busiest->group_type == group_smt_balance) {
		/* Reduce number of tasks sharing CPU capacity */
		env->migration_type = migrate_task;
		env->imbalance = 1;
		തിരികെ;
	}

#ifdef CONFIG_SCHED_CACHE
	എങ്കിൽ (busiest->group_type == group_llc_balance) {
		/* Move a task that prefer local LLC */
		env->migration_type = migrate_llc_task;
		env->imbalance = 1;
		തിരികെ;
	}
#endif

	എങ്കിൽ (busiest->group_type == group_imbalanced) {
		/*
		 * In the group_imb case we cannot rely on group-wide averages
		 * to ensure CPU-load equilibrium, try to move any task to fix
		 * the imbalance. The next load balance will take care of
		 * balancing back the system.
		 */
		env->migration_type = migrate_task;
		env->imbalance = 1;
		തിരികെ;
	}

	/*
	 * Try to use spare capacity of local group without overloading it or
	 * emptying busiest.
	 */
	എങ്കിൽ (local->group_type == group_has_spare) {
		എങ്കിൽ ((busiest->group_type > group_fully_busy) &&
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
			env->imbalance = max(local->group_capacity, local->group_util) -
					 local->group_util;

			/*
			 * In some cases, the group's utilization is max or even
			 * higher than capacity because of migrations but the
			 * local CPU is (newly) idle. There is at least one
			 * waiting task in this overloaded busiest group. Let's
			 * try to pull it.
			 */
			എങ്കിൽ (env->idle && env->imbalance == 0) {
				env->migration_type = migrate_task;
				env->imbalance = 1;
			}

			തിരികെ;
		}

		എങ്കിൽ (busiest->group_weight == 1 || sds->prefer_sibling) {
			/*
			 * When prefer sibling, evenly spread running tasks on
			 * groups.
			 */
			env->migration_type = migrate_task;
			env->imbalance = sibling_imbalance(env, sds, busiest, local);
		} അല്ലെങ്കിൽ {

			/*
			 * If there is no overload, we just want to even the number of
			 * idle CPUs.
			 */
			env->migration_type = migrate_task;
			env->imbalance = max_t(long, 0,
					       (local->idle_cpus - busiest->idle_cpus));
		}

#ifdef CONFIG_NUMA
		/* Consider allowing a small imbalance between NUMA groups */
		എങ്കിൽ (env->sd->flags & SD_NUMA) {
			env->imbalance = adjust_numa_imbalance(env->imbalance,
							       local->sum_nr_running + 1,
							       env->sd->imb_numa_nr);
		}
#endif

		/* Number of tasks to move to restore balance */
		env->imbalance >>= 1;

		തിരികെ;
	}

	/*
	 * Local is fully busy but has to take more load to relieve the
	 * busiest group
	 */
	എങ്കിൽ (local->group_type < group_overloaded) {
		/*
		 * Local will become overloaded so the avg_load metrics are
		 * finally needed.
		 */

		local->avg_load = (local->group_load * SCHED_CAPACITY_SCALE) /
				  local->group_capacity;

		/*
		 * If the local group is more loaded than the selected
		 * busiest group don't try to pull any tasks.
		 */
		എങ്കിൽ (local->avg_load >= busiest->avg_load) {
			env->imbalance = 0;
			തിരികെ;
		}

		sds->avg_load = (sds->total_load * SCHED_CAPACITY_SCALE) /
				sds->total_capacity;

		/*
		 * If the local group is more loaded than the average system
		 * load, don't try to pull any tasks.
		 */
		എങ്കിൽ (local->avg_load >= sds->avg_load) {
			env->imbalance = 0;
			തിരികെ;
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
	env->migration_type = migrate_load;
	env->imbalance = min(
		(busiest->avg_load - sds->avg_load) * busiest->group_capacity,
		(sds->avg_load - local->avg_load) * local->group_capacity
	) / SCHED_CAPACITY_SCALE;
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
സ്ഥിതം ഘടന sched_group *sched_balance_find_src_group(ഘടന lb_env *env)
{
	ഘടന sg_lb_stats *local, *busiest;
	ഘടന sd_lb_stats sds;

	init_sd_lb_stats(&sds);

	/*
	 * Compute the various statistics relevant for load balancing at
	 * this level.
	 */
	update_sd_lb_stats(env, &sds);

	/* There is no busy sibling group to pull tasks from */
	എങ്കിൽ (!sds.busiest)
		പോകുക out_balanced;

	busiest = &sds.busiest_stat;

	/* Misfit tasks should be dealt with regardless of the avg load */
	എങ്കിൽ (busiest->group_type == group_misfit_task)
		പോകുക force_balance;

	എങ്കിൽ (!is_rd_overutilized(env->dst_rq->rd) &&
	    rcu_dereference_all(env->dst_rq->rd->pd))
		പോകുക out_balanced;

	/* ASYM feature bypasses nice load balance check */
	എങ്കിൽ (busiest->group_type == group_asym_packing)
		പോകുക force_balance;

	/*
	 * If the busiest group is imbalanced the below checks don't
	 * work because they assume all things are equal, which typically
	 * isn't true due to cpus_ptr constraints and the like.
	 */
	എങ്കിൽ (busiest->group_type == group_imbalanced)
		പോകുക force_balance;

	local = &sds.local_stat;
	/*
	 * If the local group is busier than the selected busiest group
	 * don't try and pull any tasks.
	 */
	എങ്കിൽ (local->group_type > busiest->group_type)
		പോകുക out_balanced;

	/*
	 * When groups are overloaded, use the avg_load to ensure fairness
	 * between tasks.
	 */
	എങ്കിൽ (local->group_type == group_overloaded) {
		/*
		 * If the local group is more loaded than the selected
		 * busiest group don't try to pull any tasks.
		 */
		എങ്കിൽ (local->avg_load >= busiest->avg_load)
			പോകുക out_balanced;

		/* XXX broken for overlapping NUMA groups */
		sds.avg_load = (sds.total_load * SCHED_CAPACITY_SCALE) /
				sds.total_capacity;

		/*
		 * Don't pull any tasks if this group is already above the
		 * domain average load.
		 */
		എങ്കിൽ (local->avg_load >= sds.avg_load)
			പോകുക out_balanced;

		/*
		 * If the busiest group is more loaded, use imbalance_pct to be
		 * conservative.
		 */
		എങ്കിൽ (100 * busiest->avg_load <=
				env->sd->imbalance_pct * local->avg_load)
			പോകുക out_balanced;
	}

	/*
	 * Try to move all excess tasks to a sibling domain of the busiest
	 * group's child domain.
	 */
	എങ്കിൽ (sds.prefer_sibling && local->group_type == group_has_spare &&
	    (busiest->group_type == group_llc_balance ||
	    sibling_imbalance(env, &sds, busiest, local) > 1))
		പോകുക force_balance;

	എങ്കിൽ (busiest->group_type != group_overloaded) {
		എങ്കിൽ (!env->idle) {
			/*
			 * If the busiest group is not overloaded (and as a
			 * result the local one too) but this CPU is already
			 * busy, let another idle CPU try to pull task.
			 */
			പോകുക out_balanced;
		}

		എങ്കിൽ (busiest->group_type == group_smt_balance &&
		    smt_vs_nonsmt_groups(sds.local, sds.busiest)) {
			/* Let non SMT CPU pull from SMT CPU sharing with sibling */
			പോകുക force_balance;
		}

		എങ്കിൽ (busiest->group_weight > 1 &&
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
			പോകുക out_balanced;
		}

		എങ്കിൽ (busiest->sum_h_nr_running == 1) {
			/*
			 * busiest doesn't have any tasks waiting to run
			 */
			പോകുക out_balanced;
		}
	}

force_balance:
	/* Looks like there is an imbalance. Compute it */
	calculate_imbalance(env, &sds);
	തിരികെ env->imbalance ? sds.busiest : ശൂന്യ;

out_balanced:
	env->imbalance = 0;
	തിരികെ ശൂന്യ;
}

/*
 * sched_balance_find_src_rq - find the busiest runqueue among the CPUs in the group.
 */
സ്ഥിതം ഘടന rq *sched_balance_find_src_rq(ഘടന lb_env *env,
				     ഘടന sched_group *group)
{
	ഘടന rq *busiest = ശൂന്യ, *rq;
	unsigned long busiest_util = 0, busiest_load = 0, busiest_capacity = 1;
	unsigned പൂർണ്ണസംഖ്യ __maybe_unused busiest_pref_llc = 0;
	ഘടന sched_domain __maybe_unused *sd_tmp;
	unsigned പൂർണ്ണസംഖ്യ busiest_nr = 0;
	പൂർണ്ണസംഖ്യ __maybe_unused dst_llc;
	പൂർണ്ണസംഖ്യ i;

	for_each_cpu_and(i, sched_group_span(group), env->cpus) {
		unsigned long capacity, load, util;
		unsigned പൂർണ്ണസംഖ്യ nr_running;
		എണ്ണൽ fbq_type rt;

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
		എങ്കിൽ (rt > env->fbq_type)
			തുടരുക;

		nr_running = rq->cfs.h_nr_runnable;
		എങ്കിൽ (!nr_running)
			തുടരുക;

		capacity = capacity_of(i);

		/*
		 * For ASYM_CPUCAPACITY domains, don't pick a CPU that could
		 * eventually lead to active_balancing high->low capacity.
		 * Higher per-CPU capacity is considered better than balancing
		 * average load.
		 */
		എങ്കിൽ (env->sd->flags & SD_ASYM_CPUCAPACITY &&
		    nr_running == 1) {
			bool cluster_equal_cap = static_branch_unlikely(&sched_cluster_active) &&
						 (get_actual_cpu_capacity(env->dst_cpu) ==
						  get_actual_cpu_capacity(i));
			bool smt_degraded_cap = sched_smt_active() && !is_core_idle(i);

			/*
			 * Busy SMT siblings reduce the capacity of CPU @i. Do
			 * not skip it in this case.
			 *
			 * CONFIG_SCHED_CLUSTER requires balancing load across
			 * clusters of identical capacity, accounting for
			 * hardware and cpufreq pressure.
			 */
			എങ്കിൽ (!smt_degraded_cap && !cluster_equal_cap &&
			    !capacity_greater(capacity_of(env->dst_cpu), capacity))
				തുടരുക;
		}

		/*
		 * Make sure we only pull tasks from a CPU of lower priority
		 * when balancing between SMT siblings.
		 *
		 * If balancing between cores, let lower priority CPUs help
		 * SMT cores with more than one busy sibling.
		 */
		എങ്കിൽ (sched_asym(env->sd, i, env->dst_cpu) && nr_running == 1)
			തുടരുക;

		തിരഞ്ഞെടുപ്പ് (env->migration_type) {
		അവസ്ഥ migrate_load:
			/*
			 * When comparing with load imbalance, use cpu_load()
			 * which is not scaled with the CPU capacity.
			 */
			load = cpu_load(rq);

			എങ്കിൽ (nr_running == 1 && load > env->imbalance &&
			    !check_cpu_capacity(rq, env->sd))
				നിർത്തുക;

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
			എങ്കിൽ (load * busiest_capacity > busiest_load * capacity) {
				busiest_load = load;
				busiest_capacity = capacity;
				busiest = rq;
			}
			നിർത്തുക;

		അവസ്ഥ migrate_util:
			util = cpu_util_cfs_boost(i);

			/*
			 * Don't try to pull utilization from a CPU with one
			 * running task. Whatever its utilization, we will fail
			 * detach the task.
			 */
			എങ്കിൽ (nr_running <= 1)
				തുടരുക;

			എങ്കിൽ (busiest_util < util) {
				busiest_util = util;
				busiest = rq;
			}
			നിർത്തുക;

		അവസ്ഥ migrate_task:
			എങ്കിൽ (busiest_nr < nr_running) {
				busiest_nr = nr_running;
				busiest = rq;
			}
			നിർത്തുക;

		അവസ്ഥ migrate_misfit:
			/*
			 * For ASYM_CPUCAPACITY domains with misfit tasks we
			 * simply seek the "biggest" misfit task.
			 */
			എങ്കിൽ (rq->misfit_task_load > busiest_load) {
				busiest_load = rq->misfit_task_load;
				busiest = rq;
			}

			നിർത്തുക;

		അവസ്ഥ migrate_llc_task:
#ifdef CONFIG_SCHED_CACHE
			sd_tmp = rcu_dereference_all(rq->sd);
			dst_llc = llc_id(env->dst_cpu);

			എങ്കിൽ (sd_tmp && (unsigned)dst_llc < sd_tmp->llc_max) {
				unsigned പൂർണ്ണസംഖ്യ this_pref_llc =
					sd_tmp->llc_counts[dst_llc];

				എങ്കിൽ (busiest_pref_llc < this_pref_llc) {
					busiest_pref_llc = this_pref_llc;
					busiest = rq;
				}
			}
#endif
			നിർത്തുക;

		}
	}

	തിരികെ busiest;
}

/*
 * Max backoff if we encounter pinned tasks. Pretty arbitrary value, but
 * so long as it is large enough.
 */
#define MAX_PINNED_INTERVAL	512

സ്ഥിതം വരിയിൽ bool
asym_active_balance(ഘടന lb_env *env)
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
	തിരികെ env->idle && sched_use_asym_prio(env->sd, env->dst_cpu) &&
	       (sched_asym_prefer(env->dst_cpu, env->src_cpu) ||
		!sched_use_asym_prio(env->sd, env->src_cpu));
}

സ്ഥിതം വരിയിൽ bool
imbalanced_active_balance(ഘടന lb_env *env)
{
	ഘടന sched_domain *sd = env->sd;

	/*
	 * The imbalanced case includes the case of pinned tasks preventing a fair
	 * distribution of the load on the system but also the even distribution of the
	 * threads on a system with spare capacity
	 */
	എങ്കിൽ ((env->migration_type == migrate_task) &&
	    (sd->nr_balance_failed > sd->cache_nice_tries+2))
		തിരികെ 1;

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ need_active_balance(ഘടന lb_env *env)
{
	ഘടന sched_domain *sd = env->sd;

	എങ്കിൽ (alb_break_llc(env))
		തിരികെ 0;

	എങ്കിൽ (asym_active_balance(env))
		തിരികെ 1;

	എങ്കിൽ (imbalanced_active_balance(env))
		തിരികെ 1;

	/*
	 * The dst_cpu is idle and the src_cpu CPU has only 1 CFS task.
	 * It's worth migrating the task if the src_cpu's capacity is reduced
	 * because of other sched_class or IRQs if more capacity stays
	 * available on dst_cpu.
	 */
	എങ്കിൽ (env->idle &&
	    (env->src_rq->cfs.h_nr_runnable == 1)) {
		എങ്കിൽ ((check_cpu_capacity(env->src_rq, sd)) &&
		    (capacity_of(env->src_cpu)*sd->imbalance_pct < capacity_of(env->dst_cpu)*100))
			തിരികെ 1;
	}

	എങ്കിൽ (env->migration_type == migrate_misfit ||
	    env->migration_type == migrate_llc_task)
		തിരികെ 1;

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ active_load_balance_cpu_stop(ശൂന്യം *data);
സ്ഥിതം പൂർണ്ണസംഖ്യ active_load_balance_llc_cpu_stop(ശൂന്യം *data);

/*
 * migration_type is checked elsewhere to decide migration policy, so
 * it shouldn't be repurposed just to flag an LLC-directed active
 * balance across the stopper. Pick the callback here instead.
 */
സ്ഥിതം വരിയിൽ cpu_stop_fn_t alb_stop_fn(ഘടന lb_env *env)
{
	എങ്കിൽ (env->migration_type == migrate_llc_task)
		തിരികെ active_load_balance_llc_cpu_stop;

	തിരികെ active_load_balance_cpu_stop;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ should_we_balance(ഘടന lb_env *env)
{
	ഘടന cpumask *swb_cpus = this_cpu_cpumask_var_ptr(should_we_balance_tmpmask);
	ഘടന sched_group *sg = env->sd->groups;
	പൂർണ്ണസംഖ്യ cpu, idle_smt = -1;

	/*
	 * Ensure the balancing environment is consistent; can happen
	 * when the softirq triggers 'during' hotplug.
	 */
	എങ്കിൽ (!cpumask_test_cpu(env->dst_cpu, env->cpus))
		തിരികെ 0;

	/*
	 * In the newly idle case, we will allow all the CPUs
	 * to do the newly idle load balance.
	 *
	 * However, we bail out if we already have tasks or a wakeup pending,
	 * to optimize wakeup latency.
	 */
	എങ്കിൽ (env->idle == CPU_NEWLY_IDLE) {
		എങ്കിൽ (env->dst_rq->nr_running > 0 || env->dst_rq->ttwu_pending)
			തിരികെ 0;
		തിരികെ 1;
	}

	cpumask_copy(swb_cpus, group_balance_mask(sg));
	/* Try to find first idle CPU */
	for_each_cpu_and(cpu, swb_cpus, env->cpus) {
		എങ്കിൽ (!idle_cpu(cpu))
			തുടരുക;

		/*
		 * Don't balance to idle SMT in busy core right away when
		 * balancing cores, but remember the first idle SMT CPU for
		 * later consideration.  Find CPU on an idle core first.
		 */
		എങ്കിൽ (sched_smt_active() &&
		    !(env->sd->flags & SD_SHARE_CPUCAPACITY) &&
		    !is_core_idle(cpu)) {
			എങ്കിൽ (idle_smt == -1)
				idle_smt = cpu;
			/*
			 * If the core is not idle, and first SMT sibling which is
			 * idle has been found, then its not needed to check other
			 * SMT siblings for idleness:
			 */
			cpumask_andnot(swb_cpus, swb_cpus, cpu_smt_mask(cpu));
			തുടരുക;
		}

		/*
		 * Are we the first idle core in a non-SMT domain or higher,
		 * or the first idle CPU in a SMT domain?
		 */
		തിരികെ cpu == env->dst_cpu;
	}

	/* Are we the first idle CPU with busy siblings? */
	എങ്കിൽ (idle_smt != -1)
		തിരികെ idle_smt == env->dst_cpu;

	/* Are we the first CPU of this group ? */
	തിരികെ group_balance_cpu(sg) == env->dst_cpu;
}

സ്ഥിതം ശൂന്യം update_lb_imbalance_stat(ഘടന lb_env *env, ഘടന sched_domain *sd,
				     എണ്ണൽ cpu_idle_type idle)
{
	എങ്കിൽ (!schedstat_enabled())
		തിരികെ;

	തിരഞ്ഞെടുപ്പ് (env->migration_type) {
	അവസ്ഥ migrate_load:
		__schedstat_add(sd->lb_imbalance_load[idle], env->imbalance);
		നിർത്തുക;
	അവസ്ഥ migrate_util:
		__schedstat_add(sd->lb_imbalance_util[idle], env->imbalance);
		നിർത്തുക;
	അവസ്ഥ migrate_task:
		__schedstat_add(sd->lb_imbalance_task[idle], env->imbalance);
		നിർത്തുക;
	അവസ്ഥ migrate_misfit:
		__schedstat_add(sd->lb_imbalance_misfit[idle], env->imbalance);
		നിർത്തുക;
	അവസ്ഥ migrate_llc_task:
		നിർത്തുക;
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
സ്ഥിതം atomic_t sched_balance_running = ATOMIC_INIT(0);

/*
 * Check this_cpu to ensure it is balanced within domain. Attempt to move
 * tasks if there is an imbalance.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ sched_balance_rq(പൂർണ്ണസംഖ്യ this_cpu, ഘടന rq *this_rq,
			ഘടന sched_domain *sd, എണ്ണൽ cpu_idle_type idle,
			പൂർണ്ണസംഖ്യ *continue_balancing)
{
	പൂർണ്ണസംഖ്യ ld_moved, cur_ld_moved, active_balance = 0;
	ഘടന sched_domain *sd_parent = sd->parent;
	ഘടന sched_group *group;
	ഘടന rq *busiest;
	ഘടന rq_flags rf;
	ഘടന cpumask *cpus = this_cpu_cpumask_var_ptr(load_balance_mask);
	ഘടന lb_env env = {
		.sd		= sd,
		.dst_cpu	= this_cpu,
		.dst_rq		= this_rq,
		.dst_grpmask    = group_balance_mask(sd->groups),
		.idle		= idle,
		.loop_break	= SCHED_NR_MIGRATE_BREAK,
		.cpus		= cpus,
		.fbq_type	= all,
		.tasks		= LIST_HEAD_INIT(env.tasks),
	};
	bool need_unlock = false;

	cpumask_and(cpus, sched_domain_span(sd), cpu_preferred_mask);

	schedstat_inc(sd->lb_എണ്ണം[idle]);

redo:
	എങ്കിൽ (!should_we_balance(&env)) {
		*continue_balancing = 0;
		പോകുക out_balanced;
	}

	എങ്കിൽ (!need_unlock && (sd->flags & SD_SERIALIZE)) {
		പൂർണ്ണസംഖ്യ zero = 0;
		എങ്കിൽ (!atomic_try_cmpxchg_acquire(&sched_balance_running, &zero, 1))
			പോകുക out_balanced;

		need_unlock = true;
	}

	group = sched_balance_find_src_group(&env);
	എങ്കിൽ (!group) {
		schedstat_inc(sd->lb_nobusyg[idle]);
		പോകുക out_balanced;
	}

	busiest = sched_balance_find_src_rq(&env, group);
	എങ്കിൽ (!busiest) {
		schedstat_inc(sd->lb_nobusyq[idle]);
		പോകുക out_balanced;
	}

	WARN_ON_ONCE(busiest == env.dst_rq);

	update_lb_imbalance_stat(&env, sd, idle);

	env.src_cpu = busiest->cpu;
	env.src_rq = busiest;

	ld_moved = 0;
	/* Clear this flag as soon as we find a pullable task */
	env.flags |= LBF_ALL_PINNED;
	എങ്കിൽ (busiest->nr_running > 1) {
		/*
		 * Attempt to move tasks. If sched_balance_find_src_group has found
		 * an imbalance but busiest->nr_running <= 1, the group is
		 * still unbalanced. ld_moved simply stays zero, so it is
		 * correctly treated as an imbalance.
		 */
		env.loop_max  = min(sysctl_sched_nr_migrate, busiest->nr_running);

more_balance:
		rq_lock_irqsave(busiest, &rf);
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

		എങ്കിൽ (cur_ld_moved) {
			attach_tasks(&env);
			ld_moved += cur_ld_moved;
		}

		local_irq_restore(rf.flags);

		എങ്കിൽ (env.flags & LBF_NEED_BREAK) {
			env.flags &= ~LBF_NEED_BREAK;
			പോകുക more_balance;
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
		എങ്കിൽ ((env.flags & LBF_DST_PINNED) && env.imbalance > 0) {

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
			പോകുക more_balance;
		}

		/*
		 * We failed to reach balance because of affinity.
		 */
		എങ്കിൽ (sd_parent) {
			പൂർണ്ണസംഖ്യ *group_imbalance = &sd_parent->groups->sgc->imbalance;

			എങ്കിൽ ((env.flags & LBF_SOME_PINNED) && env.imbalance > 0)
				*group_imbalance = 1;
		}

		/* All tasks on this runqueue were pinned by CPU affinity */
		എങ്കിൽ (unlikely(env.flags & LBF_ALL_PINNED)) {
			__cpumask_clear_cpu(cpu_of(busiest), cpus);
			/*
			 * Attempting to continue load balancing at the current
			 * sched_domain level only makes sense if there are
			 * active CPUs remaining as possible busiest CPUs to
			 * pull load from which are not contained within the
			 * destination group that is receiving any migrated
			 * load.
			 */
			എങ്കിൽ (!cpumask_subset(cpus, env.dst_grpmask)) {
				env.loop = 0;
				env.loop_break = SCHED_NR_MIGRATE_BREAK;
				പോകുക redo;
			}
			പോകുക out_all_pinned;
		}
	}

	എങ്കിൽ (ld_moved) {
		sd->nr_balance_failed = 0;
		പോകുക out_unbalanced;
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
	എങ്കിൽ (idle != CPU_NEWLY_IDLE &&
	    env.migration_type != migrate_misfit &&
	    !(env.flags & LBF_LLC_PINNED))
		sd->nr_balance_failed++;

	എങ്കിൽ (!need_active_balance(&env))
		പോകുക out_unbalanced;

	scoped_guard (raw_spin_rq_lock_irqsave, busiest) {
		/*
		 * Don't kick the active_load_balance_cpu_stop,
		 * if the curr task on busiest CPU can't be
		 * moved to this_cpu:
		 */
		എങ്കിൽ (!cpumask_test_cpu(this_cpu, busiest->curr->cpus_ptr))
			പോകുക out_one_pinned;

		/* Record that we found at least one task that could run on this_cpu */
		env.flags &= ~LBF_ALL_PINNED;

		/*
		 * ->active_balance synchronizes accesses to
		 * ->active_balance_work.  Once set, it's cleared
		 * only after active load balance is finished.
		 */
		എങ്കിൽ (busiest->active_balance)
			പോകുക out_unbalanced;

		/*
		 * @busiest dropped its rq_lock in the middle of
		 * scheduling out its ->curr task (->on_rq := 0), no
		 * need to forcefully punt it away with active balance.
		 */
		എങ്കിൽ (!busiest->curr->on_rq)
			പോകുക out_unbalanced;

		busiest->active_balance = 1;
		busiest->തള്ളുക_cpu = this_cpu;
		active_balance = 1;
		preempt_disable();
	}
	എങ്കിൽ (active_balance) {
		stop_one_cpu_nowait(cpu_of(busiest),
				    alb_stop_fn(&env), busiest,
				    &busiest->active_balance_work);
	}
	preempt_enable();

out_unbalanced:
	/* We were unbalanced, so reset the balancing interval */
	sd->balance_interval = sd->min_interval;
	പോകുക out;

out_balanced:
	/*
	 * We reach balance although we may have faced some affinity
	 * constraints. Clear the imbalance flag only if other tasks got
	 * a chance to move and fix the imbalance.
	 */
	എങ്കിൽ (sd_parent && !(env.flags & LBF_ALL_PINNED)) {
		പൂർണ്ണസംഖ്യ *group_imbalance = &sd_parent->groups->sgc->imbalance;

		എങ്കിൽ (*group_imbalance)
			*group_imbalance = 0;
	}

out_all_pinned:
	/*
	 * We reach balance because all tasks are pinned at this level so
	 * we can't migrate them. Let the imbalance flag set so parent level
	 * can try to migrate them.
	 */
	schedstat_inc(sd->lb_balanced[idle]);

	sd->nr_balance_failed = 0;

out_one_pinned:
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
	എങ്കിൽ (env.idle == CPU_NEWLY_IDLE ||
	    env.migration_type == migrate_misfit)
		പോകുക out;

	/* tune up the balancing interval */
	എങ്കിൽ ((env.flags & LBF_ALL_PINNED &&
	     sd->balance_interval < MAX_PINNED_INTERVAL) ||
	    sd->balance_interval < sd->max_interval)
		sd->balance_interval *= 2;
out:
	എങ്കിൽ (need_unlock)
		atomic_set_release(&sched_balance_running, 0);

	തിരികെ ld_moved;
}

സ്ഥിതം വരിയിൽ unsigned long
get_sd_balance_interval(ഘടന sched_domain *sd, പൂർണ്ണസംഖ്യ cpu_busy)
{
	unsigned long interval = sd->balance_interval;

	എങ്കിൽ (cpu_busy)
		interval *= sd->busy_factor;

	/* scale ms to jiffies */
	interval = msecs_to_jiffies(interval);

	/*
	 * Reduce likelihood of busy balancing at higher domains racing with
	 * balancing at lower domains by preventing their balancing periods
	 * from being multiples of each other.
	 */
	എങ്കിൽ (cpu_busy)
		interval -= 1;

	interval = clamp(interval, 1UL, max_load_balance_interval);

	തിരികെ interval;
}

സ്ഥിതം വരിയിൽ ശൂന്യം
update_next_balance(ഘടന sched_domain *sd, unsigned long *next_balance)
{
	unsigned long interval, next;

	/* used by idle balance, so cpu_busy = 0 */
	interval = get_sd_balance_interval(sd, 0);
	next = sd->last_balance + interval;

	എങ്കിൽ (time_after(*next_balance, next))
		*next_balance = next;
}

/*
 * active_load_balance_cpu_stop is run by the CPU stopper. It pushes
 * running tasks off the busiest CPU onto idle CPUs. It requires at
 * least 1 task to be running on each physical CPU where possible, and
 * avoids physical / logical imbalances.
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ __active_load_balance_cpu_stop(ശൂന്യം *data, unsigned പൂർണ്ണസംഖ്യ lb_flags)
{
	ഘടന rq *busiest_rq = data;
	പൂർണ്ണസംഖ്യ busiest_cpu = cpu_of(busiest_rq);
	പൂർണ്ണസംഖ്യ target_cpu = busiest_rq->തള്ളുക_cpu;
	ഘടന rq *target_rq = cpu_rq(target_cpu);
	ഘടന sched_domain *sd;
	ഘടന task_struct *p = ശൂന്യ;
	ഘടന rq_flags rf;

	rq_lock_irq(busiest_rq, &rf);
	/*
	 * Between queueing the stop-work and running it is a hole in which
	 * CPUs can become inactive. We should not move tasks from or to
	 * inactive CPUs.
	 */
	എങ്കിൽ (!cpu_active(busiest_cpu) || !cpu_active(target_cpu))
		പോകുക out_unlock;

	/* Make sure the requested CPU hasn't gone down in the meantime: */
	എങ്കിൽ (unlikely(busiest_cpu != smp_processor_id() ||
		     !busiest_rq->active_balance))
		പോകുക out_unlock;

	/* Is there any task to move? */
	എങ്കിൽ (busiest_rq->nr_running <= 1)
		പോകുക out_unlock;

	/*
	 * This condition is "impossible", if it occurs
	 * we need to fix it. Originally reported by
	 * Bjorn Helgaas on a 128-CPU setup.
	 */
	WARN_ON_ONCE(busiest_rq == target_rq);

	/* Search for an sd spanning us and the target CPU. */
	rcu_read_lock();
	for_each_domain(target_cpu, sd) {
		എങ്കിൽ (cpumask_test_cpu(busiest_cpu, sched_domain_span(sd)))
			നിർത്തുക;
	}

	എങ്കിൽ (likely(sd)) {
		ഘടന lb_env env = {
			.sd		= sd,
			.dst_cpu	= target_cpu,
			.dst_rq		= target_rq,
			.src_cpu	= busiest_rq->cpu,
			.src_rq		= busiest_rq,
			.idle		= CPU_IDLE,
			.flags		= LBF_ACTIVE_LB | lb_flags,
		};

		schedstat_inc(sd->alb_എണ്ണം);
		update_rq_clock(busiest_rq);

		p = detach_one_task(&env);
		എങ്കിൽ (p) {
			schedstat_inc(sd->alb_pushed);
			/* Active balancing done, reset the failure counter. */
			sd->nr_balance_failed = 0;
		} അല്ലെങ്കിൽ {
			schedstat_inc(sd->alb_failed);
		}
	}
	rcu_read_unlock();
out_unlock:
	busiest_rq->active_balance = 0;
	rq_unlock(busiest_rq, &rf);

	എങ്കിൽ (p)
		attach_one_task(target_rq, p);

	local_irq_enable();

	തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ active_load_balance_cpu_stop(ശൂന്യം *data)
{
	തിരികെ __active_load_balance_cpu_stop(data, 0);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ active_load_balance_llc_cpu_stop(ശൂന്യം *data)
{
	തിരികെ __active_load_balance_cpu_stop(data, LBF_ACTIVE_LB_LLC);
}

/*
 * Scale the max sched_balance_rq interval with the number of CPUs in the system.
 * This trades load-balance latency on larger machines for less cross talk.
 */
ശൂന്യം update_max_interval(ശൂന്യം)
{
	max_load_balance_interval = HZ*അക്കം_online_cpus()/10;
}

സ്ഥിതം വരിയിൽ ശൂന്യം update_newidle_stats(ഘടന sched_domain *sd, unsigned പൂർണ്ണസംഖ്യ success)
{
	sd->newidle_call++;
	sd->newidle_success += success;

	എങ്കിൽ (sd->newidle_call >= 1024) {
		u64 now = sched_clock();
		s64 delta = now - sd->newidle_stamp;
		sd->newidle_stamp = now;
		പൂർണ്ണസംഖ്യ ratio = 0;

		എങ്കിൽ (delta < 0)
			delta = 0;

		എങ്കിൽ (sched_feat(NI_RATE)) {
			/*
			 * ratio  delta   freq
			 *
			 * 1024 -  4  s -  128 Hz
			 *  512 -  2  s -  256 Hz
			 *  256 -  1  s -  512 Hz
			 *  128 - .5  s - 1024 Hz
			 *   64 - .25 s - 2048 Hz
			 */
			ratio = delta >> 22;
		}

		ratio += sd->newidle_success;

		sd->newidle_ratio = min(1024, ratio);
		sd->newidle_call /= 2;
		sd->newidle_success /= 2;
	}
}

സ്ഥിതം വരിയിൽ bool
update_newidle_cost(ഘടന sched_domain *sd, u64 cost, unsigned പൂർണ്ണസംഖ്യ success)
{
	unsigned long next_decay = sd->last_decay_max_lb_cost + HZ;
	unsigned long now = jiffies;

	എങ്കിൽ (cost)
		update_newidle_stats(sd, success);

	എങ്കിൽ (cost > sd->max_newidle_lb_cost) {
		/*
		 * Track max cost of a domain to make sure to not delay the
		 * next wakeup on the CPU.
		 */
		sd->max_newidle_lb_cost = cost;
		sd->last_decay_max_lb_cost = now;

	} അല്ലെങ്കിൽ എങ്കിൽ (time_after(now, next_decay)) {
		/*
		 * Decay the newidle max times by ~1% per second to ensure that
		 * it is not outdated and the current max cost is actually
		 * shorter.
		 */
		sd->max_newidle_lb_cost = (sd->max_newidle_lb_cost * 253) / 256;
		sd->last_decay_max_lb_cost = now;
		തിരികെ true;
	}

	തിരികെ false;
}

/*
 * It checks each scheduling domain to see if it is due to be balanced,
 * and initiates a balancing operation if so.
 *
 * Balancing parameters are set up in init_sched_domains.
 */
സ്ഥിതം ശൂന്യം sched_balance_domains(ഘടന rq *rq, എണ്ണൽ cpu_idle_type idle)
{
	പൂർണ്ണസംഖ്യ continue_balancing = 1;
	പൂർണ്ണസംഖ്യ cpu = rq->cpu;
	പൂർണ്ണസംഖ്യ busy = idle != CPU_IDLE && !sched_idle_rq(rq);
	unsigned long interval;
	ഘടന sched_domain *sd;
	/* Earliest time when we have to do rebalance again */
	unsigned long next_balance = jiffies + 60*HZ;
	പൂർണ്ണസംഖ്യ update_next_balance = 0;
	പൂർണ്ണസംഖ്യ need_decay = 0;
	u64 max_cost = 0;

	rcu_read_lock();
	for_each_domain(cpu, sd) {
		/*
		 * Decay the newidle max times here because this is a regular
		 * visit to all the domains.
		 */
		need_decay = update_newidle_cost(sd, 0, 0);
		max_cost += sd->max_newidle_lb_cost;

		/*
		 * Stop the load balance at this level. There is another
		 * CPU in our sched group which is doing load balancing more
		 * actively.
		 */
		എങ്കിൽ (!continue_balancing) {
			എങ്കിൽ (need_decay)
				തുടരുക;
			നിർത്തുക;
		}

		interval = get_sd_balance_interval(sd, busy);
		എങ്കിൽ (time_after_eq(jiffies, sd->last_balance + interval)) {
			എങ്കിൽ (sched_balance_rq(cpu, rq, sd, idle, &continue_balancing)) {
				/*
				 * The LBF_DST_PINNED logic could have changed
				 * env->dst_cpu, so we can't know our idle
				 * state even if we migrated tasks. Update it.
				 */
				idle = idle_cpu(cpu);
				busy = !idle && !sched_idle_rq(rq);
			}
			sd->last_balance = jiffies;
			interval = get_sd_balance_interval(sd, busy);
		}
		എങ്കിൽ (time_after(next_balance, sd->last_balance + interval)) {
			next_balance = sd->last_balance + interval;
			update_next_balance = 1;
		}
	}
	എങ്കിൽ (need_decay) {
		/*
		 * Ensure the rq-wide value also decays but keep it at a
		 * reasonable floor to avoid funnies with rq->avg_idle.
		 */
		rq->max_idle_balance_cost =
			max((u64)sysctl_sched_migration_cost, max_cost);
	}
	rcu_read_unlock();

	/*
	 * next_balance will be updated only when there is a need.
	 * When the cpu is attached to null domain for ex, it will not be
	 * updated.
	 */
	എങ്കിൽ (likely(update_next_balance))
		rq->next_balance = next_balance;

}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ on_null_domain(ഘടന rq *rq)
{
	തിരികെ unlikely(!rcu_dereference_sched(rq->sd));
}

#ifdef CONFIG_NO_HZ_COMMON
/*
 * NOHZ idle load balancing (ILB) details:
 *
 * - When one of the busy CPUs notices that there may be an idle rebalancing
 *   needed, they will kick the idle load balancer, which then does idle
 *   load balancing for all the idle CPUs.
 */
സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ find_new_ilb(ശൂന്യം)
{
	ഘടന cpumask *ilb_cpus;
	പൂർണ്ണസംഖ്യ ilb_cpu, fallback = -1;

	lockdep_assert_irqs_disabled();

	/*
	 * Reuse the per-CPU select_rq_mask, which is protected from concurrent
	 * use on this CPU by having interrupts disabled.
	 */
	ilb_cpus = this_cpu_cpumask_var_ptr(select_rq_mask);
	cpumask_and(ilb_cpus, nohz.idle_cpus_mask,
		    housekeeping_cpumask(HK_TYPE_KERNEL_NOISE));

	for_each_cpu(ilb_cpu, ilb_cpus) {
		എങ്കിൽ (!idle_cpu(ilb_cpu)) {
			/*
			 * Once an idle fallback exists, a busy CPU proves that
			 * this core cannot be fully idle. Skip its siblings.
			 */
			എങ്കിൽ (sched_smt_active() && fallback >= 0)
				cpumask_andnot(ilb_cpus, ilb_cpus, cpu_smt_mask(ilb_cpu));
			തുടരുക;
		}

		/*
		 * Running the idle load balancer on an idle sibling of a busy
		 * SMT core can reduce the capacity available to its sibling. Prefer
		 * a CPU whose entire core is idle, but retain the first idle CPU as
		 * a fallback so idle balancing can still make progress when no fully
		 * idle core exists.
		 */
		എങ്കിൽ (sched_smt_active() && !is_core_idle(ilb_cpu)) {
			എങ്കിൽ (fallback < 0)
				fallback = ilb_cpu;

			/*
			 * The core is not idle, so there is no need to check
			 * any of its other SMT siblings.
			 */
			cpumask_andnot(ilb_cpus, ilb_cpus,
				       cpu_smt_mask(ilb_cpu));
			തുടരുക;
		}

		തിരികെ ilb_cpu;
	}

	തിരികെ fallback;
}

/*
 * Kick a CPU to do the NOHZ balancing, if it is time for it, via a cross-CPU
 * SMP function call (IPI).
 *
 * Prefer a CPU on a fully idle core in the HK_TYPE_KERNEL_NOISE housekeeping
 * set. Fall back to the first idle CPU when no fully idle core exists.
 */
സ്ഥിതം ശൂന്യം kick_ilb(unsigned പൂർണ്ണസംഖ്യ flags)
{
	പൂർണ്ണസംഖ്യ ilb_cpu;

	/*
	 * Increase nohz.next_balance only when if full ilb is triggered but
	 * not if we only update stats.
	 */
	എങ്കിൽ (flags & NOHZ_BALANCE_KICK)
		nohz.next_balance = jiffies+1;

	ilb_cpu = find_new_ilb();
	എങ്കിൽ (ilb_cpu < 0)
		തിരികെ;

	/*
	 * Don't bother if no new NOHZ balance work items for ilb_cpu,
	 * i.e. all bits in flags are already set in ilb_cpu.
	 */
	എങ്കിൽ ((atomic_read(nohz_flags(ilb_cpu)) & flags) == flags)
		തിരികെ;

	/*
	 * Access to rq::nohz_csd is serialized by NOHZ_KICK_MASK; he who sets
	 * the first flag owns it; cleared by nohz_csd_func().
	 */
	flags = atomic_fetch_or(flags, nohz_flags(ilb_cpu));
	എങ്കിൽ (flags & NOHZ_KICK_MASK)
		തിരികെ;

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
സ്ഥിതം ശൂന്യം nohz_balancer_kick(ഘടന rq *rq)
{
	unsigned long now = jiffies;
	ഘടന sched_domain_shared *sds;
	ഘടന sched_domain *sd;
	പൂർണ്ണസംഖ്യ nr_busy, i, cpu = rq->cpu;
	unsigned പൂർണ്ണസംഖ്യ flags = 0;

	എങ്കിൽ (unlikely(rq->idle_balance))
		തിരികെ;

	/*
	 * We may be recently in ticked or tickless idle mode. At the first
	 * busy tick after returning from idle, we will update the busy stats.
	 */
	nohz_balance_exit_idle(rq);

	എങ്കിൽ (READ_ONCE(nohz.has_blocked_load) &&
	    time_after(now, READ_ONCE(nohz.next_blocked)))
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
	എങ്കിൽ (time_before(now, nohz.next_balance))
		പോകുക out;

	/*
	 * None are in tickless mode and hence no need for NOHZ idle load
	 * balancing
	 */
	എങ്കിൽ (unlikely(cpumask_empty(nohz.idle_cpus_mask)))
		തിരികെ;

	എങ്കിൽ (rq->nr_running >= 2) {
		flags = NOHZ_STATS_KICK | NOHZ_BALANCE_KICK;
		പോകുക out;
	}

	sd = rcu_dereference_all(rq->sd);
	എങ്കിൽ (sd) {
		/*
		 * If there's a runnable CFS task and the current CPU has reduced
		 * capacity, kick the ILB to see if there's a better CPU to run on:
		 */
		എങ്കിൽ (rq->cfs.h_nr_runnable >= 1 && check_cpu_capacity(rq, sd)) {
			flags |= NOHZ_STATS_KICK | NOHZ_BALANCE_KICK;
			പോകുക out;
		}
	}

	sd = rcu_dereference_all(per_cpu(sd_asym_packing, cpu));
	എങ്കിൽ (sd) {
		/*
		 * When ASYM_PACKING; see if there's a more preferred CPU
		 * currently idle; in which case, kick the ILB to move tasks
		 * around.
		 *
		 * When balancing between cores, all the SMT siblings of the
		 * preferred CPU must be idle.
		 */
		for_each_cpu_and(i, sched_domain_span(sd), nohz.idle_cpus_mask) {
			എങ്കിൽ (sched_asym(sd, i, cpu)) {
				flags |= NOHZ_STATS_KICK | NOHZ_BALANCE_KICK;
				പോകുക out;
			}
		}
	}

	sd = rcu_dereference_all(per_cpu(sd_asym_cpucapacity, cpu));
	എങ്കിൽ (sd) {
		/*
		 * When ASYM_CPUCAPACITY; see if there's a higher capacity CPU
		 * to run the misfit task on.
		 */
		എങ്കിൽ (check_misfit_status(rq))
			flags |= NOHZ_STATS_KICK | NOHZ_BALANCE_KICK;

		/*
		 * For asymmetric systems, we do not want to nicely balance
		 * cache use, instead we want to embrace asymmetry and only
		 * ensure tasks have enough CPU capacity.
		 *
		 * Skip the LLC logic because it's not relevant in that case.
		 */
		പോകുക out;
	}

	sds = rcu_dereference_all(per_cpu(sd_balance_shared, cpu));
	എങ്കിൽ (sds) {
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
		എങ്കിൽ (nr_busy > 1)
			flags |= NOHZ_STATS_KICK | NOHZ_BALANCE_KICK;
	}
out:
	എങ്കിൽ (READ_ONCE(nohz.needs_update))
		flags |= NOHZ_NEXT_KICK;

	എങ്കിൽ (flags)
		kick_ilb(flags);
}

സ്ഥിതം ശൂന്യം set_cpu_sd_state_busy(പൂർണ്ണസംഖ്യ cpu)
{
	ഘടന sched_domain *sd;
	sd = rcu_dereference_all(per_cpu(sd_llc, cpu));

	/*
	 * sd->nohz_idle only pairs with nr_busy_cpus on sd->shared; if this
	 * domain has no shared object there is nothing to clear or account.
	 */
	എങ്കിൽ (!sd || !sd->shared || !sd->nohz_idle)
		തിരികെ;
	sd->nohz_idle = 0;

	atomic_inc(&sd->shared->nr_busy_cpus);
}

ശൂന്യം nohz_balance_exit_idle(ഘടന rq *rq)
{
	WARN_ON_ONCE(rq != this_rq());

	എങ്കിൽ (likely(!rq->nohz_tick_stopped))
		തിരികെ;

	rq->nohz_tick_stopped = 0;
	cpumask_clear_cpu(rq->cpu, nohz.idle_cpus_mask);

	set_cpu_sd_state_busy(rq->cpu);
}

സ്ഥിതം ശൂന്യം set_cpu_sd_state_idle(പൂർണ്ണസംഖ്യ cpu)
{
	ഘടന sched_domain *sd;
	sd = rcu_dereference_all(per_cpu(sd_llc, cpu));

	/* See set_cpu_sd_state_busy(): nohz_idle is only used with sd->shared. */
	എങ്കിൽ (!sd || !sd->shared || sd->nohz_idle)
		തിരികെ;
	sd->nohz_idle = 1;

	atomic_dec(&sd->shared->nr_busy_cpus);
}

/*
 * This routine will record that the CPU is going idle with tick stopped.
 * This info will be used in performing idle load balancing in the future.
 */
ശൂന്യം nohz_balance_enter_idle(പൂർണ്ണസംഖ്യ cpu)
{
	ഘടന rq *rq = cpu_rq(cpu);

	WARN_ON_ONCE(cpu != smp_processor_id());

	/* If this CPU is going down, then nothing needs to be done: */
	എങ്കിൽ (!cpu_active(cpu))
		തിരികെ;

	/*
	 * Can be set safely without rq->lock held
	 * If a clear happens, it will have evaluated last additions because
	 * rq->lock is held during the check and the clear
	 */
	rq->has_blocked_load = 1;

	/*
	 * The tick is still stopped but load could have been added in the
	 * meantime. We set the nohz.has_blocked_load flag to trig a check of the
	 * *_avg. The CPU is already part of nohz.idle_cpus_mask so the clear
	 * of nohz.has_blocked_load can only happen after checking the new load
	 */
	എങ്കിൽ (rq->nohz_tick_stopped)
		പോകുക out;

	/* If we're a completely isolated CPU, we don't play: */
	എങ്കിൽ (on_null_domain(rq))
		തിരികെ;

	rq->nohz_tick_stopped = 1;

	cpumask_set_cpu(cpu, nohz.idle_cpus_mask);

	/*
	 * Ensures that if nohz_idle_balance() fails to observe our
	 * @idle_cpus_mask store, it must observe the @has_blocked_load
	 * and @needs_update stores.
	 */
	smp_mb__after_atomic();

	set_cpu_sd_state_idle(cpu);

	WRITE_ONCE(nohz.needs_update, 1);
out:
	/*
	 * Each time a cpu enter idle, we assume that it has blocked load and
	 * enable the periodic update of the load of idle CPUs
	 */
	WRITE_ONCE(nohz.has_blocked_load, 1);
}

സ്ഥിതം bool update_nohz_stats(ഘടന rq *rq)
{
	unsigned പൂർണ്ണസംഖ്യ cpu = rq->cpu;

	എങ്കിൽ (!rq->has_blocked_load)
		തിരികെ false;

	എങ്കിൽ (!cpumask_test_cpu(cpu, nohz.idle_cpus_mask))
		തിരികെ false;

	എങ്കിൽ (!time_after(jiffies, READ_ONCE(rq->last_blocked_load_update_tick)))
		തിരികെ true;

	sched_balance_update_blocked_averages(cpu);

	തിരികെ rq->has_blocked_load;
}

/*
 * Internal function that runs load balance for all idle CPUs. The load balance
 * can be a simple update of blocked load or a complete load balance with
 * tasks movement depending of flags.
 */
സ്ഥിതം ശൂന്യം _nohz_idle_balance(ഘടന rq *this_rq, unsigned പൂർണ്ണസംഖ്യ flags)
{
	/* Earliest time when we have to do rebalance again */
	unsigned long now = jiffies;
	unsigned long next_balance = now + 60*HZ;
	bool has_blocked_load = false;
	പൂർണ്ണസംഖ്യ update_next_balance = 0;
	പൂർണ്ണസംഖ്യ this_cpu = this_rq->cpu;
	പൂർണ്ണസംഖ്യ balance_cpu;
	ഘടന rq *rq;

	WARN_ON_ONCE((flags & NOHZ_KICK_MASK) == NOHZ_BALANCE_KICK);

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
	എങ്കിൽ (flags & NOHZ_STATS_KICK)
		WRITE_ONCE(nohz.has_blocked_load, 0);
	എങ്കിൽ (flags & NOHZ_NEXT_KICK)
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
	for_each_cpu_wrap(balance_cpu,  nohz.idle_cpus_mask, this_cpu+1) {
		എങ്കിൽ (!idle_cpu(balance_cpu))
			തുടരുക;

		/*
		 * If this CPU gets work to do, stop the load balancing
		 * work being done for other CPUs. Next load
		 * balancing owner will pick it up.
		 */
		എങ്കിൽ (!idle_cpu(this_cpu) && need_resched()) {
			എങ്കിൽ (flags & NOHZ_STATS_KICK)
				has_blocked_load = true;
			എങ്കിൽ (flags & NOHZ_NEXT_KICK)
				WRITE_ONCE(nohz.needs_update, 1);
			പോകുക abort;
		}

		rq = cpu_rq(balance_cpu);

		എങ്കിൽ (flags & NOHZ_STATS_KICK)
			has_blocked_load |= update_nohz_stats(rq);

		/*
		 * If time for next balance is due,
		 * do the balance.
		 */
		എങ്കിൽ (time_after_eq(jiffies, rq->next_balance)) {
			ഘടന rq_flags rf;

			rq_lock_irqsave(rq, &rf);
			update_rq_clock(rq);
			rq_unlock_irqrestore(rq, &rf);

			എങ്കിൽ (flags & NOHZ_BALANCE_KICK)
				sched_balance_domains(rq, CPU_IDLE);
		}

		എങ്കിൽ (time_after(next_balance, rq->next_balance)) {
			next_balance = rq->next_balance;
			update_next_balance = 1;
		}
	}

	/*
	 * next_balance will be updated only when there is a need.
	 * When the CPU is attached to null domain for ex, it will not be
	 * updated.
	 */
	എങ്കിൽ (likely(update_next_balance))
		nohz.next_balance = next_balance;

	എങ്കിൽ (flags & NOHZ_STATS_KICK)
		WRITE_ONCE(nohz.next_blocked,
			   now + msecs_to_jiffies(LOAD_AVG_PERIOD));

abort:
	/* There is still blocked load, enable periodic update */
	എങ്കിൽ (has_blocked_load)
		WRITE_ONCE(nohz.has_blocked_load, 1);
}

/*
 * In CONFIG_NO_HZ_COMMON case, the idle balance kickee will do the
 * rebalancing for all the CPUs for whom scheduler ticks are stopped.
 */
സ്ഥിതം bool nohz_idle_balance(ഘടന rq *this_rq, എണ്ണൽ cpu_idle_type idle)
{
	unsigned പൂർണ്ണസംഖ്യ flags = this_rq->nohz_idle_balance;

	എങ്കിൽ (!flags)
		തിരികെ false;

	this_rq->nohz_idle_balance = 0;

	എങ്കിൽ (idle != CPU_IDLE)
		തിരികെ false;

	_nohz_idle_balance(this_rq, flags);

	തിരികെ true;
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
ശൂന്യം nohz_run_idle_balance(പൂർണ്ണസംഖ്യ cpu)
{
	unsigned പൂർണ്ണസംഖ്യ flags;

	flags = atomic_fetch_andnot(NOHZ_NEWILB_KICK, nohz_flags(cpu));

	/*
	 * Update the blocked load only if no SCHED_SOFTIRQ is about to happen
	 * (i.e. NOHZ_STATS_KICK set) and will do the same.
	 */
	എങ്കിൽ ((flags == NOHZ_NEWILB_KICK) && !need_resched())
		_nohz_idle_balance(cpu_rq(cpu), NOHZ_STATS_KICK);
}

സ്ഥിതം ശൂന്യം nohz_newidle_balance(ഘടന rq *this_rq)
{
	പൂർണ്ണസംഖ്യ this_cpu = this_rq->cpu;

	/* Will wake up very soon. No time for doing anything else*/
	എങ്കിൽ (this_rq->avg_idle < sysctl_sched_migration_cost)
		തിരികെ;

	/* Don't need to update blocked load of idle CPUs*/
	എങ്കിൽ (!READ_ONCE(nohz.has_blocked_load) ||
	    time_before(jiffies, READ_ONCE(nohz.next_blocked)))
		തിരികെ;

	/*
	 * Set the need to trigger ILB in order to update blocked load
	 * before entering idle state.
	 */
	atomic_or(NOHZ_NEWILB_KICK, nohz_flags(this_cpu));
}

#അല്ലെങ്കിൽ /* !CONFIG_NO_HZ_COMMON: */
സ്ഥിതം വരിയിൽ ശൂന്യം nohz_balancer_kick(ഘടന rq *rq) { }

സ്ഥിതം വരിയിൽ bool nohz_idle_balance(ഘടന rq *this_rq, എണ്ണൽ cpu_idle_type idle)
{
	തിരികെ false;
}

സ്ഥിതം വരിയിൽ ശൂന്യം nohz_newidle_balance(ഘടന rq *this_rq) { }
#endif /* !CONFIG_NO_HZ_COMMON */

/*
 * sched_balance_newidle is called by schedule() if this_cpu is about to become
 * idle. Attempts to pull tasks from other CPUs.
 *
 * Returns:
 *   < 0 - we released the lock and there are !fair tasks present
 *     0 - failed, no new tasks
 *   > 0 - success, new (fair) tasks present
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ sched_balance_newidle(ഘടന rq *this_rq, ഘടന rq_flags *rf)
	__must_hold(__rq_lockp(this_rq))
{
	unsigned long next_balance = jiffies + HZ;
	പൂർണ്ണസംഖ്യ this_cpu = this_rq->cpu;
	പൂർണ്ണസംഖ്യ continue_balancing = 1;
	u64 t0, t1, curr_cost = 0;
	ഘടന sched_domain *sd;
	പൂർണ്ണസംഖ്യ pulled_task = 0;

	update_misfit_status(ശൂന്യ, this_rq);

	/*
	 * There is a task waiting to run. No need to search for one.
	 * Return 0; the task will be enqueued when switching to idle.
	 */
	എങ്കിൽ (this_rq->ttwu_pending)
		തിരികെ 0;

	/*
	 * We must set idle_stamp _before_ calling sched_balance_rq()
	 * for CPU_NEWLY_IDLE, such that we measure the this duration
	 * as idle time.
	 */
	this_rq->idle_stamp = rq_clock(this_rq);

	/* Do not pull tasks towards !preferred CPUs */
	എങ്കിൽ (!cpu_preferred(this_cpu))
		തിരികെ 0;

	/*
	 * This is OK, because current is on_cpu, which avoids it being picked
	 * for load-balance and preemption/IRQs are still disabled avoiding
	 * further scheduler activity on it and we're being very careful to
	 * re-start the picking loop.
	 */
	rq_unpin_lock(this_rq, rf);

	sd = rcu_dereference_sched_domain(this_rq->sd);
	എങ്കിൽ (!sd)
		പോകുക out;

	എങ്കിൽ (!get_rd_overloaded(this_rq->rd) ||
	    this_rq->avg_idle < sd->max_newidle_lb_cost) {

		update_next_balance(sd, &next_balance);
		പോകുക out;
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
		u64 domain_cost;

		update_next_balance(sd, &next_balance);

		എങ്കിൽ (this_rq->avg_idle < curr_cost + sd->max_newidle_lb_cost)
			നിർത്തുക;

		എങ്കിൽ (sd->flags & SD_BALANCE_NEWIDLE) {
			unsigned പൂർണ്ണസംഖ്യ weight = 1;

			എങ്കിൽ (sched_feat(NI_RANDOM) && sd->newidle_ratio < 1024) {
				/*
				 * Throw a 1k sided dice; and only run
				 * newidle_balance according to the success
				 * rate.
				 */
				u32 d1k = sched_rng() % 1024;
				weight = 1 + sd->newidle_ratio;
				എങ്കിൽ (d1k > weight) {
					update_newidle_stats(sd, 0);
					തുടരുക;
				}
				weight = (1024 + weight/2) / weight;
			}

			pulled_task = sched_balance_rq(this_cpu, this_rq,
						   sd, CPU_NEWLY_IDLE,
						   &continue_balancing);

			t1 = sched_clock_cpu(this_cpu);
			domain_cost = t1 - t0;
			curr_cost += domain_cost;
			t0 = t1;

			/*
			 * Track max cost of a domain to make sure to not delay the
			 * next wakeup on the CPU.
			 */
			update_newidle_cost(sd, domain_cost, weight * !!pulled_task);
		}

		/*
		 * Stop searching for tasks to pull if there are
		 * now runnable tasks on this rq.
		 */
		എങ്കിൽ (pulled_task || !continue_balancing)
			നിർത്തുക;
	}

	raw_spin_rq_lock(this_rq);

	എങ്കിൽ (curr_cost > this_rq->max_idle_balance_cost)
		this_rq->max_idle_balance_cost = curr_cost;

	/*
	 * While browsing the domains, we released the rq lock, a task could
	 * have been enqueued in the meantime. Since we're not going idle,
	 * pretend we pulled a task.
	 */
	എങ്കിൽ (this_rq->cfs.h_nr_queued && !pulled_task)
		pulled_task = 1;

	/* If a higher prio class was modified, restart the pick */
	എങ്കിൽ (rq_modified_above(this_rq, &fair_sched_class))
		pulled_task = -1;

out:
	/* Move the next balance forward */
	എങ്കിൽ (time_after(this_rq->next_balance, next_balance))
		this_rq->next_balance = next_balance;

	എങ്കിൽ (pulled_task)
		this_rq->idle_stamp = 0;
	അല്ലെങ്കിൽ
		nohz_newidle_balance(this_rq);

	rq_repin_lock(this_rq, rf);

	തിരികെ pulled_task;
}

/*
 * This softirq handler is triggered via SCHED_SOFTIRQ from two places:
 *
 * - directly from the local sched_tick() for periodic load balancing
 *
 * - indirectly from a remote sched_tick() for NOHZ idle balancing
 *   through the SMP cross-call nohz_csd_func()
 */
സ്ഥിതം __latent_entropy ശൂന്യം sched_balance_softirq(ശൂന്യം)
{
	ഘടന rq *this_rq = this_rq();
	എണ്ണൽ cpu_idle_type idle = this_rq->idle_balance;
	/*
	 * If this CPU has a pending NOHZ_BALANCE_KICK, then do the
	 * balancing on behalf of the other idle CPUs whose ticks are
	 * stopped. Do nohz_idle_balance *before* sched_balance_domains to
	 * give the idle CPUs a chance to load balance. Else we may
	 * load balance only within the local sched_domain hierarchy
	 * and abort nohz_idle_balance altogether if we pull some load.
	 */
	എങ്കിൽ (nohz_idle_balance(this_rq, idle))
		തിരികെ;

	/* normal load balance */
	sched_balance_update_blocked_averages(this_rq->cpu);
	sched_balance_domains(this_rq, idle);
}

/*
 * Trigger the SCHED_SOFTIRQ if it is time to do periodic load balancing.
 */
ശൂന്യം sched_balance_trigger(ഘടന rq *rq)
{
	/*
	 * Don't need to rebalance while attached to NULL domain or
	 * runqueue CPU is not active
	 */
	എങ്കിൽ (unlikely(on_null_domain(rq) || !cpu_active(cpu_of(rq))))
		തിരികെ;

	എങ്കിൽ (time_after_eq(jiffies, rq->next_balance))
		raise_softirq(SCHED_SOFTIRQ);

	nohz_balancer_kick(rq);
}

സ്ഥിതം ശൂന്യം rq_online_fair(ഘടന rq *rq)
{
	update_sysctl();

	update_runtime_enabled(rq);
}

സ്ഥിതം ശൂന്യം rq_offline_fair(ഘടന rq *rq)
{
	update_sysctl();

	/* Ensure any throttled groups are reachable by pick_next_task */
	unthrottle_offline_cfs_rqs(rq);

	/* Ensure that we remove rq contribution to group share: */
	clear_tg_offline_cfs_rqs(rq);
}

#ifdef CONFIG_SCHED_CORE
സ്ഥിതം വരിയിൽ bool
__entity_slice_used(ഘടന sched_entity *se, പൂർണ്ണസംഖ്യ min_nr_tasks)
{
	u64 rtime = se->sum_exec_runtime - se->prev_sum_exec_runtime;
	u64 slice = se->slice;

	തിരികെ (rtime * min_nr_tasks > slice);
}

#define MIN_NR_TASKS_DURING_FORCEIDLE	2
സ്ഥിതം വരിയിൽ ശൂന്യം task_tick_core(ഘടന rq *rq, ഘടന task_struct *curr)
{
	എങ്കിൽ (!sched_core_enabled(rq))
		തിരികെ;

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
	എങ്കിൽ (rq->core->core_forceidle_എണ്ണം && rq->cfs.h_nr_queued == 1 &&
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
സ്ഥിതം ശൂന്യം se_fi_update(സ്ഥിരം ഘടന sched_entity *se, unsigned പൂർണ്ണസംഖ്യ fi_seq,
			 bool forceidle)
{
	for_each_sched_entity(se) {
		ഘടന cfs_rq *cfs_rq = cfs_rq_of(se);

		എങ്കിൽ (forceidle) {
			എങ്കിൽ (cfs_rq->forceidle_seq == fi_seq)
				നിർത്തുക;
			cfs_rq->forceidle_seq = fi_seq;
		}

		cfs_rq->zero_vruntime_fi = cfs_rq->zero_vruntime;
	}
}

ശൂന്യം task_vruntime_update(ഘടന rq *rq, ഘടന task_struct *p, bool in_fi)
{
	ഘടന sched_entity *se = &p->se;

	എങ്കിൽ (p->sched_class != &fair_sched_class)
		തിരികെ;

	se_fi_update(se, rq->core->core_forceidle_seq, in_fi);
}

bool cfs_prio_less(സ്ഥിരം ഘടന task_struct *a, സ്ഥിരം ഘടന task_struct *b,
			bool in_fi)
{
	ഘടന rq *rq = task_rq(a);
	സ്ഥിരം ഘടന sched_entity *sea = &a->se;
	സ്ഥിരം ഘടന sched_entity *seb = &b->se;
	ഘടന cfs_rq *cfs_rqa;
	ഘടന cfs_rq *cfs_rqb;
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

	തിരികെ delta > 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ task_is_throttled_fair(ഘടന task_struct *p, പൂർണ്ണസംഖ്യ cpu)
{
	ഘടന cfs_rq *cfs_rq;

#ifdef CONFIG_FAIR_GROUP_SCHED
	cfs_rq = tg_cfs_rq(task_group(p), cpu);
#അല്ലെങ്കിൽ
	cfs_rq = &cpu_rq(cpu)->cfs;
#endif
	തിരികെ throttled_hierarchy(cfs_rq);
}
#അല്ലെങ്കിൽ /* !CONFIG_SCHED_CORE: */
സ്ഥിതം വരിയിൽ ശൂന്യം task_tick_core(ഘടന rq *rq, ഘടന task_struct *curr) {}
#endif /* !CONFIG_SCHED_CORE */

/*
 * scheduler tick hitting a task of our scheduling class.
 *
 * NOTE: This function can be called remotely by the tick offload that
 * goes along full dynticks. Therefore no local assumption can be made
 * and everything must be accessed through the @rq and @curr passed in
 * parameters.
 */
സ്ഥിതം ശൂന്യം task_tick_fair(ഘടന rq *rq, ഘടന task_struct *curr, പൂർണ്ണസംഖ്യ queued)
{
	ഘടന sched_entity *se = &curr->se;

	എങ്കിൽ (se->on_rq) {
		unsigned long weight = NICE_0_LOAD;
		ഘടന cfs_rq *cfs_rq;

		for_each_sched_entity(se) {
			cfs_rq = cfs_rq_of(se);
			entity_tick(cfs_rq, se, queued);

			weight = __calc_prop_weight(cfs_rq, se, weight);
		}

		se = &curr->se;
		reweight_eevdf(cfs_rq, se, weight, se->on_rq);
	}

	എങ്കിൽ (queued)
		തിരികെ;

	എങ്കിൽ (static_branch_unlikely(&sched_numa_balancing))
		task_tick_numa(rq, curr);

	task_tick_cache(rq, curr);

	update_misfit_status(curr, rq);
	check_update_overutilized_status(task_rq(curr));

	task_tick_core(rq, curr);
}

/*
 * called on fork with the child task as argument from the parent's context
 *  - child not yet on the tasklist
 *  - preemption disabled
 */
സ്ഥിതം ശൂന്യം task_fork_fair(ഘടന task_struct *p)
{
	set_task_max_allowed_capacity(p);
}

/*
 * Priority of the task has changed. Check to see if we preempt
 * the current task.
 */
സ്ഥിതം ശൂന്യം
prio_changed_fair(ഘടന rq *rq, ഘടന task_struct *p, u64 oldprio)
{
	എങ്കിൽ (!task_on_rq_queued(p))
		തിരികെ;

	എങ്കിൽ (p->prio == oldprio)
		തിരികെ;

	എങ്കിൽ (rq->cfs.h_nr_queued == 1)
		തിരികെ;

	/*
	 * Reschedule if we are currently running on this runqueue and
	 * our priority decreased, or if we are not currently running on
	 * this runqueue and our priority is higher than the current's
	 */
	എങ്കിൽ (task_current_donor(rq, p)) {
		എങ്കിൽ (p->prio > oldprio)
			resched_curr(rq);
	} അല്ലെങ്കിൽ {
		wakeup_preempt(rq, p, 0);
	}
}

#ifdef CONFIG_FAIR_GROUP_SCHED
/*
 * Propagate the changes of the sched_entity across the tg tree to make it
 * visible to the root
 */
സ്ഥിതം ശൂന്യം propagate_entity_cfs_rq(ഘടന sched_entity *se)
{
	ഘടന cfs_rq *cfs_rq = cfs_rq_of(se);

	/*
	 * If a task gets attached to this cfs_rq and before being queued,
	 * it gets migrated to another CPU due to reasons like affinity
	 * change, make sure this cfs_rq stays on leaf cfs_rq list to have
	 * that removed load decayed or it can cause faireness problem.
	 */
	എങ്കിൽ (!cfs_rq_pelt_clock_throttled(cfs_rq))
		list_add_leaf_cfs_rq(cfs_rq);

	/* Start to propagate at parent */
	se = se->parent;

	for_each_sched_entity(se) {
		cfs_rq = cfs_rq_of(se);

		update_load_avg(cfs_rq, se, UPDATE_TG);

		എങ്കിൽ (!cfs_rq_pelt_clock_throttled(cfs_rq))
			list_add_leaf_cfs_rq(cfs_rq);
	}

	assert_list_leaf_cfs_rq(rq_of(cfs_rq));
}
#അല്ലെങ്കിൽ /* !CONFIG_FAIR_GROUP_SCHED: */
സ്ഥിതം ശൂന്യം propagate_entity_cfs_rq(ഘടന sched_entity *se) { }
#endif /* !CONFIG_FAIR_GROUP_SCHED */

സ്ഥിതം ശൂന്യം detach_entity_cfs_rq(ഘടന sched_entity *se)
{
	ഘടന cfs_rq *cfs_rq = cfs_rq_of(se);

	/*
	 * In case the task sched_avg hasn't been attached:
	 * - A forked task which hasn't been woken up by wake_up_new_task().
	 * - A task which has been woken up by try_to_wake_up() but is
	 *   waiting for actually being woken up by sched_ttwu_pending().
	 */
	എങ്കിൽ (!se->avg.last_update_time)
		തിരികെ;

	/* Catch up with the cfs_rq and remove our load when we leave */
	update_load_avg(cfs_rq, se, 0);
	detach_entity_load_avg(cfs_rq, se);
	update_tg_load_avg(cfs_rq);
	propagate_entity_cfs_rq(se);
}

സ്ഥിതം ശൂന്യം attach_entity_cfs_rq(ഘടന sched_entity *se)
{
	ഘടന cfs_rq *cfs_rq = cfs_rq_of(se);

	/* Synchronize entity with its cfs_rq */
	update_load_avg(cfs_rq, se, sched_feat(ATTACH_AGE_LOAD) ? 0 : SKIP_AGE_LOAD);
	attach_entity_load_avg(cfs_rq, se);
	update_tg_load_avg(cfs_rq);
	propagate_entity_cfs_rq(se);
}

സ്ഥിതം ശൂന്യം detach_task_cfs_rq(ഘടന task_struct *p)
{
	ഘടന sched_entity *se = &p->se;

	detach_entity_cfs_rq(se);
}

സ്ഥിതം ശൂന്യം attach_task_cfs_rq(ഘടന task_struct *p)
{
	ഘടന sched_entity *se = &p->se;

	attach_entity_cfs_rq(se);
}

സ്ഥിതം ശൂന്യം switching_from_fair(ഘടന rq *rq, ഘടന task_struct *p)
{
	എങ്കിൽ (p->se.sched_delayed)
		dequeue_task(rq, p, DEQUEUE_SLEEP | DEQUEUE_DELAYED | DEQUEUE_NOCLOCK);
}

സ്ഥിതം ശൂന്യം switched_from_fair(ഘടന rq *rq, ഘടന task_struct *p)
{
	detach_task_cfs_rq(p);
}

സ്ഥിതം ശൂന്യം switched_to_fair(ഘടന rq *rq, ഘടന task_struct *p)
{
	WARN_ON_ONCE(p->se.sched_delayed);

	attach_task_cfs_rq(p);

	set_task_max_allowed_capacity(p);

	എങ്കിൽ (task_on_rq_queued(p)) {
		/*
		 * We were most likely switched from sched_rt, so
		 * kick off the schedule if running, otherwise just see
		 * if we can still preempt the current task.
		 */
		എങ്കിൽ (task_current_donor(rq, p))
			resched_curr(rq);
		അല്ലെങ്കിൽ
			wakeup_preempt(rq, p, 0);
	}
}

സ്ഥിതം ശൂന്യം set_next_task_fair(ഘടന rq *rq, ഘടന task_struct *p, എണ്ണൽ snt_e type)
{
	ഘടന sched_entity *se = &p->se;
	ഘടന cfs_rq *cfs_rq = &rq->cfs;
	unsigned long weight = NICE_0_LOAD;
	bool first = type == SNT_PICK;
	bool throttled = false;
	bool on_rq = se->on_rq;

	എങ്കിൽ (type == SNT_REPICK)
		പോകുക repick;

	clear_buddies(cfs_rq, se);

	എങ്കിൽ (on_rq)
		__dequeue_entity(cfs_rq, se);

	for_each_sched_entity(se) {
		cfs_rq = cfs_rq_of(se);

		എങ്കിൽ (!IS_ENABLED(CONFIG_FAIR_GROUP_SCHED) ||
		    !first || !cfs_rq->h_curr)
			set_next_entity(cfs_rq, se);

		/* ensure bandwidth has been allocated on our new cfs_rq */
		throttled |= account_cfs_rq_runtime(cfs_rq, 0);

		എങ്കിൽ (on_rq)
			weight = __calc_prop_weight(cfs_rq, se, weight);
	}

	എങ്കിൽ (throttled)
		task_throttle_setup_work(p);

	se = &p->se;
	cfs_rq->curr = se;

	എങ്കിൽ (on_rq) {
		reweight_eevdf(cfs_rq, se, weight, se->on_rq);
		എങ്കിൽ (first)
			set_protect_slice(cfs_rq, se);
	}

	എങ്കിൽ (task_on_rq_queued(p)) {
		/*
		 * Move the next running task to the front of the list, so our
		 * cfs_tasks list becomes MRU one.
		 */
		list_move(&se->group_കണ്ണി, &rq->cfs_tasks);
	}
	എങ്കിൽ (!first)
		തിരികെ;

	WARN_ON_ONCE(se->sched_delayed);

	update_misfit_status(p, rq);
	sched_fair_update_stop_tick(rq, p);

repick:
	/*
	 * A same-task repick skips put_prev_task_fair(), but
	 * pick_task_fair() refreshed the entity hrtick_start_fair() reads
	 * before selecting it again. rq->cfs.curr identifies that entity,
	 * including with group scheduling.
	 */
	എങ്കിൽ (hrtick_enabled_fair(rq))
		hrtick_start_fair(rq, p);
}

ശൂന്യം init_cfs_rq(ഘടന cfs_rq *cfs_rq)
{
	cfs_rq->tasks_timeline = RB_വേര്_CACHED;
	cfs_rq->zero_vruntime = (u64)(-(1LL << 20));
	raw_spin_lock_init(&cfs_rq->removed.lock);
}

#ifdef CONFIG_FAIR_GROUP_SCHED
സ്ഥിതം ശൂന്യം task_change_group_fair(ഘടന task_struct *p)
{
	/*
	 * We couldn't detach or attach a forked task which
	 * hasn't been woken up by wake_up_new_task().
	 */
	എങ്കിൽ (READ_ONCE(p->__state) == TASK_NEW)
		തിരികെ;

	detach_task_cfs_rq(p);

	/* Tell se's cfs_rq has been changed -- migrated */
	p->se.avg.last_update_time = 0;
	set_task_rq(p, task_cpu(p));
	attach_task_cfs_rq(p);
}

ശൂന്യം free_fair_sched_group(ഘടന task_group *tg)
{
	free_percpu(tg->cfs_rq);
}

പൂർണ്ണസംഖ്യ alloc_fair_sched_group(ഘടന task_group *tg, ഘടന task_group *parent)
{
	ഘടന cfs_tg_state __percpu *state;
	ഘടന sched_entity *se;
	ഘടന cfs_rq *cfs_rq;
	പൂർണ്ണസംഖ്യ i;

	state = alloc_percpu_gfp(ഘടന cfs_tg_state, GFP_KERNEL);
	എങ്കിൽ (!state)
		പോകുക err;

	tg->cfs_rq = &state->cfs_rq;
	tg->shares = NICE_0_LOAD;

	init_cfs_bandwidth(tg_cfs_bandwidth(tg), tg_cfs_bandwidth(parent));

	for_each_possible_cpu(i) {
		cfs_rq = tg_cfs_rq(tg, i);
		എങ്കിൽ (!cfs_rq)
			പോകുക err;

		se = tg_se(tg, i);
		init_cfs_rq(cfs_rq);
		init_tg_cfs_entry(tg, cfs_rq, se, i, tg_se(parent, i));
		init_entity_runnable_average(se);
	}

	തിരികെ 1;

err:
	തിരികെ 0;
}

ശൂന്യം online_fair_sched_group(ഘടന task_group *tg)
{
	ഘടന sched_entity *se;
	ഘടന rq_flags rf;
	ഘടന rq *rq;
	പൂർണ്ണസംഖ്യ i;

	for_each_possible_cpu(i) {
		rq = cpu_rq(i);
		se = tg_se(tg, i);
		rq_lock_irq(rq, &rf);
		update_rq_clock(rq);
		attach_entity_cfs_rq(se);
		sync_throttle(tg, i);
		rq_unlock_irq(rq, &rf);
	}
}

ശൂന്യം unregister_fair_sched_group(ഘടന task_group *tg)
{
	പൂർണ്ണസംഖ്യ cpu;

	destroy_cfs_bandwidth(tg_cfs_bandwidth(tg));

	for_each_possible_cpu(cpu) {
		ഘടന cfs_rq *cfs_rq = tg_cfs_rq(tg, cpu);
		ഘടന sched_entity *se = tg_se(tg, cpu);
		ഘടന rq *rq = cpu_rq(cpu);

		എങ്കിൽ (se)
			remove_entity_load_avg(se);

		/*
		 * Only empty task groups can be destroyed; so we can speculatively
		 * check on_list without danger of it being re-added.
		 */
		എങ്കിൽ (cfs_rq->on_list) {
			guard(rq_lock_irqsave)(rq);
			list_del_leaf_cfs_rq(cfs_rq);
		}
	}
}

ശൂന്യം init_tg_cfs_entry(ഘടന task_group *tg, ഘടന cfs_rq *cfs_rq,
			ഘടന sched_entity *se, പൂർണ്ണസംഖ്യ cpu,
			ഘടന sched_entity *parent)
{
	ഘടന rq *rq = cpu_rq(cpu);

	cfs_rq->tg = tg;
	cfs_rq->rq = rq;
	init_cfs_rq_runtime(cfs_rq);

	/* se could be NULL for root_task_group */
	എങ്കിൽ (!se)
		തിരികെ;

	എങ്കിൽ (!parent) {
		se->cfs_rq = &rq->cfs;
		se->depth = 0;
	} അല്ലെങ്കിൽ {
		se->cfs_rq = parent->my_q;
		se->depth = parent->depth + 1;
	}

	se->my_q = cfs_rq;
	/* guarantee group entities always have weight */
	update_load_set(&se->load, NICE_0_LOAD);
	se->parent = parent;
}

സ്ഥിതം DEFINE_MUTEX(shares_mutex);

സ്ഥിതം പൂർണ്ണസംഖ്യ __sched_group_set_shares(ഘടന task_group *tg, unsigned long shares)
{
	പൂർണ്ണസംഖ്യ i;

	lockdep_assert_held(&shares_mutex);

	/*
	 * We can't change the weight of the root cgroup.
	 */
	എങ്കിൽ (is_വേര്_task_group(tg))
		തിരികെ -EINVAL;

	shares = clamp(shares, scale_load(MIN_SHARES), scale_load(MAX_SHARES));

	എങ്കിൽ (tg->shares == shares)
		തിരികെ 0;

	tg->shares = shares;
	for_each_possible_cpu(i) {
		ഘടന rq *rq = cpu_rq(i);
		ഘടന sched_entity *se = tg_se(tg, i);
		ഘടന rq_flags rf;

		/* Propagate contribution to hierarchy */
		rq_lock_irqsave(rq, &rf);
		update_rq_clock(rq);
		for_each_sched_entity(se) {
			update_load_avg(cfs_rq_of(se), se, UPDATE_TG);
			update_cfs_group(se);
		}
		rq_unlock_irqrestore(rq, &rf);
	}

	തിരികെ 0;
}

പൂർണ്ണസംഖ്യ sched_group_set_shares(ഘടന task_group *tg, unsigned long shares)
{
	പൂർണ്ണസംഖ്യ ret;

	mutex_lock(&shares_mutex);
	എങ്കിൽ (tg_is_idle(tg))
		ret = -EINVAL;
	അല്ലെങ്കിൽ
		ret = __sched_group_set_shares(tg, shares);
	mutex_unlock(&shares_mutex);

	തിരികെ ret;
}

പൂർണ്ണസംഖ്യ sched_group_set_idle(ഘടന task_group *tg, long idle)
{
	പൂർണ്ണസംഖ്യ i;

	എങ്കിൽ (tg == &വേര്_task_group)
		തിരികെ -EINVAL;

	എങ്കിൽ (idle < 0 || idle > 1)
		തിരികെ -EINVAL;

	mutex_lock(&shares_mutex);

	എങ്കിൽ (tg->idle == idle) {
		mutex_unlock(&shares_mutex);
		തിരികെ 0;
	}

	tg->idle = idle;

	for_each_possible_cpu(i) {
		ഘടന rq *rq = cpu_rq(i);
		ഘടന sched_entity *se = tg_se(tg, i);
		ഘടന cfs_rq *grp_cfs_rq = tg_cfs_rq(tg, i);
		bool was_idle = cfs_rq_is_idle(grp_cfs_rq);
		long idle_task_delta;
		ഘടന rq_flags rf;

		rq_lock_irqsave(rq, &rf);

		grp_cfs_rq->idle = idle;
		എങ്കിൽ (WARN_ON_ONCE(was_idle == cfs_rq_is_idle(grp_cfs_rq)))
			പോകുക next_cpu;

		idle_task_delta = grp_cfs_rq->h_nr_queued -
				  grp_cfs_rq->h_nr_idle;
		എങ്കിൽ (!cfs_rq_is_idle(grp_cfs_rq))
			idle_task_delta *= -1;

		for_each_sched_entity(se) {
			ഘടന cfs_rq *cfs_rq = cfs_rq_of(se);

			എങ്കിൽ (!se->on_rq)
				നിർത്തുക;

			cfs_rq->h_nr_idle += idle_task_delta;

			/* Already accounted at parent level and above. */
			എങ്കിൽ (cfs_rq_is_idle(cfs_rq))
				നിർത്തുക;
		}

next_cpu:
		rq_unlock_irqrestore(rq, &rf);
	}

	/* Idle groups have minimum weight. */
	എങ്കിൽ (tg_is_idle(tg))
		__sched_group_set_shares(tg, scale_load(WEIGHT_IDLEPRIO));
	അല്ലെങ്കിൽ
		__sched_group_set_shares(tg, NICE_0_LOAD);

	mutex_unlock(&shares_mutex);
	തിരികെ 0;
}

#endif /* CONFIG_FAIR_GROUP_SCHED */


സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ get_rr_interval_fair(ഘടന rq *rq, ഘടന task_struct *task)
{
	ഘടന sched_entity *se = &task->se;
	unsigned പൂർണ്ണസംഖ്യ rr_interval = 0;

	/*
	 * Time slice is 0 for SCHED_OTHER tasks that are on an otherwise
	 * idle runqueue:
	 */
	എങ്കിൽ (rq->cfs.load.weight)
		rr_interval = NS_TO_JIFFIES(se->slice);

	തിരികെ rr_interval;
}

/*
 * All the scheduling class methods:
 */
DEFINE_SCHED_CLASS(fair) = {
	.enqueue_task		= enqueue_task_fair,
	.dequeue_task		= dequeue_task_fair,
	.yield_task		= yield_task_fair,
	.yield_to_task		= yield_to_task_fair,

	.wakeup_preempt		= wakeup_preempt_fair,

	.pick_task		= pick_task_fair,
	.put_prev_task		= put_prev_task_fair,
	.set_next_task          = set_next_task_fair,

	.select_task_rq		= select_task_rq_fair,
	.migrate_task_rq	= migrate_task_rq_fair,

	.rq_online		= rq_online_fair,
	.rq_offline		= rq_offline_fair,

	.task_dead		= task_dead_fair,
	.set_cpus_allowed	= set_cpus_allowed_fair,

	.task_tick		= task_tick_fair,
	.task_fork		= task_fork_fair,

	.reweight_task		= reweight_task_fair,
	.prio_changed		= prio_changed_fair,
	.switching_from		= switching_from_fair,
	.switched_from		= switched_from_fair,
	.switched_to		= switched_to_fair,

	.get_rr_interval	= get_rr_interval_fair,

	.update_curr		= update_curr_fair,

#ifdef CONFIG_FAIR_GROUP_SCHED
	.task_change_group	= task_change_group_fair,
#endif

#ifdef CONFIG_SCHED_CORE
	.task_is_throttled	= task_is_throttled_fair,
#endif

#ifdef CONFIG_UCLAMP_TASK
	.uclamp_enabled		= 1,
#endif
};

ശൂന്യം print_cfs_stats(ഘടന seq_file *m, പൂർണ്ണസംഖ്യ cpu)
{
	ഘടന cfs_rq *cfs_rq, *pos;

	rcu_read_lock();
	for_each_leaf_cfs_rq_safe(cpu_rq(cpu), cfs_rq, pos)
		print_cfs_rq(m, cpu, cfs_rq);
	rcu_read_unlock();
}

#ifdef CONFIG_NUMA_BALANCING
ശൂന്യം show_numa_stats(ഘടന task_struct *p, ഘടന seq_file *m)
{
	പൂർണ്ണസംഖ്യ കണ്ണി;
	unsigned long tsf = 0, tpf = 0, gsf = 0, gpf = 0;
	ഘടന numa_group *ng;

	rcu_read_lock();
	ng = rcu_dereference_all(p->numa_group);
	for_each_online_കണ്ണി(കണ്ണി) {
		എങ്കിൽ (p->numa_faults) {
			tsf = p->numa_faults[task_faults_idx(NUMA_MEM, കണ്ണി, 0)];
			tpf = p->numa_faults[task_faults_idx(NUMA_MEM, കണ്ണി, 1)];
		}
		എങ്കിൽ (ng) {
			gsf = ng->faults[task_faults_idx(NUMA_MEM, കണ്ണി, 0)];
			gpf = ng->faults[task_faults_idx(NUMA_MEM, കണ്ണി, 1)];
		}
		print_numa_stats(m, കണ്ണി, tsf, tpf, gsf, gpf);
	}
	rcu_read_unlock();
}
#endif /* CONFIG_NUMA_BALANCING */

__init ശൂന്യം init_sched_fair_class(ശൂന്യം)
{
	പൂർണ്ണസംഖ്യ i;

	for_each_possible_cpu(i) {
		zalloc_cpumask_var_കണ്ണി(&per_cpu(load_balance_mask, i), GFP_KERNEL, cpu_to_കണ്ണി(i));
		zalloc_cpumask_var_കണ്ണി(&per_cpu(select_rq_mask,    i), GFP_KERNEL, cpu_to_കണ്ണി(i));
		zalloc_cpumask_var_കണ്ണി(&per_cpu(should_we_balance_tmpmask, i),
					GFP_KERNEL, cpu_to_കണ്ണി(i));

#ifdef CONFIG_CFS_BANDWIDTH
		INIT_CSD(&cpu_rq(i)->cfsb_csd, __cfsb_csd_unthrottle, cpu_rq(i));
		INIT_LIST_HEAD(&cpu_rq(i)->cfsb_csd_list);
#endif
	}

	open_softirq(SCHED_SOFTIRQ, sched_balance_softirq);

#ifdef CONFIG_NO_HZ_COMMON
	nohz.next_balance = jiffies;
	nohz.next_blocked = jiffies;
	zalloc_cpumask_var(&nohz.idle_cpus_mask, GFP_NOWAIT);
#endif
}
