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
#अन्तर्भाव <linux/energy_model.h>
#अन्तर्भाव <linux/mmap_lock.h>
#अन्तर्भाव <linux/hugetlb_inline.h>
#अन्तर्भाव <linux/jiffies.h>
#अन्तर्भाव <linux/math.h>
#अन्तर्भाव <linux/mm_api.h>
#अन्तर्भाव <linux/highmem.h>
#अन्तर्भाव <linux/hrtimer.h>
#अन्तर्भाव <linux/hrtimer_bases.h>
#अन्तर्भाव <linux/spinlock_api.h>
#अन्तर्भाव <linux/cpumask_api.h>
#अन्तर्भाव <linux/lockdep_api.h>
#अन्तर्भाव <linux/softirq.h>
#अन्तर्भाव <linux/refcount_api.h>
#अन्तर्भाव <linux/topology.h>
#अन्तर्भाव <linux/sched/clock.h>
#अन्तर्भाव <linux/sched/cond_resched.h>
#अन्तर्भाव <linux/sched/cputime.h>
#अन्तर्भाव <linux/sched/isolation.h>
#अन्तर्भाव <linux/sched/nohz.h>
#अन्तर्भाव <linux/sched/prio.h>
#अन्तर्भाव <linux/static_call.h>

#अन्तर्भाव <linux/cpuidle.h>
#अन्तर्भाव <linux/interrupt.h>
#अन्तर्भाव <linux/memory-tiers.h>
#अन्तर्भाव <linux/mempolicy.h>
#अन्तर्भाव <linux/mutex_api.h>
#अन्तर्भाव <linux/profile.h>
#अन्तर्भाव <linux/psi.h>
#अन्तर्भाव <linux/ratelimit.h>
#अन्तर्भाव <linux/task_work.h>
#अन्तर्भाव <linux/rbtree_augmented.h>

#अन्तर्भाव <asm/switch_to.h>

#अन्तर्भाव <uapi/linux/sched/types.h>

#अन्तर्भाव "sched.h"
#अन्तर्भाव "stats.h"
#अन्तर्भाव "autogroup.h"

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
अचिह्नित पूर्णाङ्क sysctl_sched_tunable_scaling = SCHED_TUNABLESCALING_LOG;

/*
 * Default base time slice (request size r_i) for SCHED_NORMAL/SCHED_BATCH:
 *
 * Under EEVDF this is the request size used to compute the virtual
 * deadline; see update_deadline().
 *
 * (default: 0.70 msec * (1 + ilog(ncpus)), units: nanoseconds)
 */
अचिह्नित पूर्णाङ्क sysctl_sched_आधारः_खण्डिका			= 700000ULL;
स्थिर अचिह्नित पूर्णाङ्क normalized_sysctl_sched_आधारः_खण्डिका	= 700000ULL;

__read_mostly अचिह्नित पूर्णाङ्क sysctl_sched_migration_cost	= 500000UL;

स्थिर पूर्णाङ्क __init सज्जता_sched_thermal_decay_सरणम्(वर्ण *str)
{
	pr_warn("Ignoring the deprecated sched_thermal_decay_shift= option\n");
	प्रत्यागम 1;
}
__setup("sched_thermal_decay_shift=", सज्जता_sched_thermal_decay_सरणम्);

/*
 * For asym packing, by default the lower numbered CPU has higher priority.
 */
पूर्णाङ्क __weak arch_asym_cpu_प्राथमिकता(पूर्णाङ्क cpu)
{
	प्रत्यागम -cpu;
}

/*
 * The margin used when comparing utilization with CPU capacity.
 *
 * (default: ~20%)
 */
#परिभाषय माति_क्षमता(cap, max)	((cap) * 1280 < (max) * 1024)

/*
 * The margin used when comparing CPU capacities.
 * is 'cap1' noticeably greater than 'cap2'
 *
 * (default: ~5%)
 */
#परिभाषय क्षमता_greater(cap1, cap2) ((cap1) * 1024 > (cap2) * 1078)

#यदि_परिभाषितम् CONFIG_CFS_BANDWIDTH
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
स्थिर अचिह्नित पूर्णाङ्क sysctl_sched_cfs_bandwidth_खण्डिका		= 5000UL;
#यदि_अन्तः

#यदि_परिभाषितम् CONFIG_NUMA_BALANCING
/* Restrict the NUMA promotion throughput (MB/s) for each target node. */
स्थिर अचिह्नित पूर्णाङ्क sysctl_numa_balancing_promote_दरः_सीमा = 65536;
#यदि_अन्तः

#यदि_परिभाषितम् CONFIG_SYSCTL
स्थिर ध्रुव संरचना ctl_सारणी sched_fair_sysctls[] = {
#यदि_परिभाषितम् CONFIG_CFS_BANDWIDTH
	{
		.procname       = "sched_cfs_bandwidth_slice_us",
		.दत्त           = &sysctl_sched_cfs_bandwidth_खण्डिका,
		.maxlen         = परिमाण(अचिह्नित पूर्णाङ्क),
		.बहुलकम्           = 0644,
		.proc_handler   = proc_dointvec_minmax,
		.extra1         = SYSCTL_ONE,
	},
#यदि_अन्तः
#यदि_परिभाषितम् CONFIG_NUMA_BALANCING
	{
		.procname	= "numa_balancing_promote_rate_limit_MBps",
		.दत्त		= &sysctl_numa_balancing_promote_दरः_सीमा,
		.maxlen		= परिमाण(अचिह्नित पूर्णाङ्क),
		.बहुलकम्		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= SYSCTL_ZERO,
	},
#यदि_अन्तः /* CONFIG_NUMA_BALANCING */
};

स्थिर पूर्णाङ्क __init sched_fair_sysctl_init(शून्य)
{
	पञ्जिकाकोशः_sysctl_init("kernel", sched_fair_sysctls);
	प्रत्यागम 0;
}
late_initcall(sched_fair_sysctl_init);
#यदि_अन्तः /* CONFIG_SYSCTL */

स्थिर अन्तःस्थ शून्य update_आरोपय_सङ्कलय(संरचना आरोपय_weight *lw, अचिह्नित दीर्घ inc)
{
	lw->weight += inc;
	lw->inv_weight = 0;
}

स्थिर अन्तःस्थ शून्य update_आरोपय_sub(संरचना आरोपय_weight *lw, अचिह्नित दीर्घ dec)
{
	lw->weight -= dec;
	lw->inv_weight = 0;
}

स्थिर अन्तःस्थ शून्य update_आरोपय_समुच्चयः(संरचना आरोपय_weight *lw, अचिह्नित दीर्घ w)
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
स्थिर अचिह्नित पूर्णाङ्क get_update_sysctl_गुणकः(शून्य)
{
	अचिह्नित पूर्णाङ्क cpus = min_t(अचिह्नित पूर्णाङ्क, अङ्कः_online_cpus(), 8);
	अचिह्नित पूर्णाङ्क गुणकः;

	विकल्प (sysctl_sched_tunable_scaling) {
	पक्ष SCHED_TUNABLESCALING_NONE:
		गुणकः = 1;
		भङ्ग;
	पक्ष SCHED_TUNABLESCALING_रैखिकम्:
		गुणकः = cpus;
		भङ्ग;
	पक्ष SCHED_TUNABLESCALING_LOG:
	स्वभाव:
		गुणकः = 1 + ilog2(cpus);
		भङ्ग;
	}

	प्रत्यागम गुणकः;
}

स्थिर शून्य update_sysctl(शून्य)
{
	अचिह्नित पूर्णाङ्क गुणकः = get_update_sysctl_गुणकः();

#परिभाषय समुच्चयः_SYSCTL(name) \
	(sysctl_##name = (गुणकः) * normalized_sysctl_##name)
	समुच्चयः_SYSCTL(sched_आधारः_खण्डिका);
#अपरिभाषय समुच्चयः_SYSCTL
}

शून्य __init sched_init_granularity(शून्य)
{
	update_sysctl();
}

#यदि_अपरिभाषितम् CONFIG_64BIT
#परिभाषय WMULT_CONST	(~0U)
#परिभाषय WMULT_सरणम्	32

स्थिर शून्य __update_inv_weight(संरचना आरोपय_weight *lw)
{
	अचिह्नित दीर्घ w;

	यदि (likely(lw->inv_weight))
		प्रत्यागम;

	w = scale_आरोपय_अधस्तात्(lw->weight);

	यदि (द्वयङ्काः_प्रत्येकम्_LONG > 32 && unlikely(w >= WMULT_CONST))
		lw->inv_weight = 1;
	अन्यथा यदि (unlikely(!w))
		lw->inv_weight = WMULT_CONST;
	अन्यथा
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
स्थिर u64 __calc_delta(u64 delta_exec, अचिह्नित दीर्घ weight, संरचना आरोपय_weight *lw)
{
	u64 fact = scale_आरोपय_अधस्तात्(weight);
	u32 fact_hi = (u32)(fact >> 32);
	पूर्णाङ्क सरणम् = WMULT_सरणम्;
	पूर्णाङ्क fs;

	__update_inv_weight(lw);

	यदि (unlikely(fact_hi)) {
		fs = fls(fact_hi);
		सरणम् -= fs;
		fact >>= fs;
	}

	fact = mul_u32_u32(fact, lw->inv_weight);

	fact_hi = (u32)(fact >> 32);
	यदि (fact_hi) {
		fs = fls(fact_hi);
		सरणम् -= fs;
		fact >>= fs;
	}

	प्रत्यागम mul_u64_u32_shr(delta_exec, fact, सरणम्);
}
#अन्यथा
स्थिर u64 __calc_delta(u64 delta_exec, अचिह्नित दीर्घ weight, संरचना आरोपय_weight *lw)
{
	प्रत्यागम (delta_exec * weight) / lw->weight;
}
#यदि_अन्तः

/*
 * delta /= w
 */
स्थिर अन्तःस्थ u64 calc_delta_fair(u64 delta, संरचना sched_entity *se)
{
	यदि (se->h_आरोपय.weight != NICE_0_आरोपय)
		delta = __calc_delta(delta, NICE_0_आरोपय, &se->h_आरोपय);

	प्रत्यागम delta;
}

ध्रुव संरचना sched_class fair_sched_class;

/**************************************************************
 * CFS operations on generic schedulable entities:
 */

#यदि_परिभाषितम् CONFIG_FAIR_गणः_SCHED

/* Walk up scheduling entities hierarchy */
#परिभाषय for_each_sched_entity(se) \
		प्रति (; se; se = se->parent)

स्थिर अन्तःस्थ bool list_सङ्कलय_पर्णम्_cfs_rq(संरचना cfs_rq *cfs_rq)
{
	संरचना rq *rq = rq_सम्बन्धि(cfs_rq);
	पूर्णाङ्क cpu = cpu_सम्बन्धि(rq);

	यदि (cfs_rq->on_list)
		प्रत्यागम rq->tmp_alone_branch == &rq->पर्णम्_cfs_rq_list;

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
	यदि (cfs_rq->tg->parent &&
	    tg_cfs_rq(cfs_rq->tg->parent, cpu)->on_list) {
		/*
		 * If parent is already on the list, we add the child
		 * just before. Thanks to circular linked property of
		 * the list, this means to put the child at the tail
		 * of the list that starts by parent.
		 */
		list_सङ्कलय_पुच्छ_rcu(&cfs_rq->पर्णम्_cfs_rq_list,
			&(tg_cfs_rq(cfs_rq->tg->parent, cpu)->पर्णम्_cfs_rq_list));
		/*
		 * The branch is now connected to its tree so we can
		 * reset tmp_alone_branch to the beginning of the
		 * list.
		 */
		rq->tmp_alone_branch = &rq->पर्णम्_cfs_rq_list;
		प्रत्यागम true;
	}

	यदि (!cfs_rq->tg->parent) {
		/*
		 * cfs rq without parent should be put
		 * at the tail of the list.
		 */
		list_सङ्कलय_पुच्छ_rcu(&cfs_rq->पर्णम्_cfs_rq_list,
			&rq->पर्णम्_cfs_rq_list);
		/*
		 * We have reach the top of a tree so we can reset
		 * tmp_alone_branch to the beginning of the list.
		 */
		rq->tmp_alone_branch = &rq->पर्णम्_cfs_rq_list;
		प्रत्यागम true;
	}

	/*
	 * The parent has not already been added so we want to
	 * make sure that it will be put after us.
	 * tmp_alone_branch points to the begin of the branch
	 * where we will add parent.
	 */
	list_सङ्कलय_rcu(&cfs_rq->पर्णम्_cfs_rq_list, rq->tmp_alone_branch);
	/*
	 * update tmp_alone_branch to points to the new begin
	 * of the branch
	 */
	rq->tmp_alone_branch = &cfs_rq->पर्णम्_cfs_rq_list;
	प्रत्यागम false;
}

स्थिर अन्तःस्थ शून्य list_del_पर्णम्_cfs_rq(संरचना cfs_rq *cfs_rq)
{
	यदि (cfs_rq->on_list) {
		संरचना rq *rq = rq_सम्बन्धि(cfs_rq);

		/*
		 * With cfs_rq being unthrottled/throttled during an enqueue,
		 * it can happen the tmp_alone_branch points to the leaf that
		 * we finally want to delete. In this case, tmp_alone_branch moves
		 * to the prev element but it will point to rq->leaf_cfs_rq_list
		 * at the end of the enqueue.
		 */
		यदि (rq->tmp_alone_branch == &cfs_rq->पर्णम्_cfs_rq_list)
			rq->tmp_alone_branch = cfs_rq->पर्णम्_cfs_rq_list.पूर्व;

		list_del_rcu(&cfs_rq->पर्णम्_cfs_rq_list);
		cfs_rq->on_list = 0;
	}
}

स्थिर अन्तःस्थ शून्य assert_list_पर्णम्_cfs_rq(संरचना rq *rq)
{
	WARN_ON_ONCE(rq->tmp_alone_branch != &rq->पर्णम्_cfs_rq_list);
}

/* Iterate through all leaf cfs_rq's on a runqueue */
#परिभाषय for_each_पर्णम्_cfs_rq_safe(rq, cfs_rq, pos)			\
	list_for_each_प्रविष्टिः_safe(cfs_rq, pos, &rq->पर्णम्_cfs_rq_list,	\
				 पर्णम्_cfs_rq_list)

/* Do the two (enqueued) entities belong to the same group ? */
स्थिर अन्तःस्थ संरचना cfs_rq *
is_same_गणः(संरचना sched_entity *se, संरचना sched_entity *pse)
{
	यदि (se->cfs_rq == pse->cfs_rq)
		प्रत्यागम se->cfs_rq;

	प्रत्यागम रिक्त;
}

स्थिर अन्तःस्थ संरचना sched_entity *parent_entity(ध्रुव संरचना sched_entity *se)
{
	प्रत्यागम se->parent;
}

स्थिर पूर्णाङ्क tg_is_idle(संरचना task_गणः *tg)
{
	प्रत्यागम tg->idle > 0;
}

स्थिर पूर्णाङ्क cfs_rq_is_idle(संरचना cfs_rq *cfs_rq)
{
	प्रत्यागम cfs_rq->idle > 0;
}

स्थिर पूर्णाङ्क se_is_idle(संरचना sched_entity *se)
{
	यदि (entity_is_task(se))
		प्रत्यागम task_has_idle_policy(task_सम्बन्धि(se));
	प्रत्यागम cfs_rq_is_idle(गणः_cfs_rq(se));
}

#अन्यथा /* !CONFIG_FAIR_GROUP_SCHED: */

#परिभाषय for_each_sched_entity(se) \
		प्रति (; se; se = रिक्त)

स्थिर अन्तःस्थ bool list_सङ्कलय_पर्णम्_cfs_rq(संरचना cfs_rq *cfs_rq)
{
	प्रत्यागम true;
}

स्थिर अन्तःस्थ शून्य list_del_पर्णम्_cfs_rq(संरचना cfs_rq *cfs_rq)
{
}

स्थिर अन्तःस्थ शून्य assert_list_पर्णम्_cfs_rq(संरचना rq *rq)
{
}

#परिभाषय for_each_पर्णम्_cfs_rq_safe(rq, cfs_rq, pos)	\
		प्रति (cfs_rq = &rq->cfs, pos = रिक्त; cfs_rq; cfs_rq = pos)

स्थिर अन्तःस्थ संरचना sched_entity *parent_entity(संरचना sched_entity *se)
{
	प्रत्यागम रिक्त;
}

स्थिर अन्तःस्थ पूर्णाङ्क tg_is_idle(संरचना task_गणः *tg)
{
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क cfs_rq_is_idle(संरचना cfs_rq *cfs_rq)
{
	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क se_is_idle(संरचना sched_entity *se)
{
	प्रत्यागम task_has_idle_policy(task_सम्बन्धि(se));
}

#यदि_अन्तः /* !CONFIG_FAIR_GROUP_SCHED */

स्थिर __always_inline
bool account_cfs_rq_runtime(संरचना cfs_rq *cfs_rq, u64 delta_exec);

/**************************************************************
 * Scheduling class tree data structure manipulation methods:
 */

बाह्य शून्य __BUILD_BUG_vruntime_cmp(शून्य);

/* Use __builtin_strcmp() because of __HAVE_ARCH_STRCMP: */

#परिभाषय vruntime_cmp(A, CMP_STR, B) ({				\
	पूर्णाङ्क __res = 0;						\
								\
	यदि (!__builtin_strcmp(CMP_STR, "<")) {			\
		__res = ((s64)((A)-(B)) < 0);			\
	} अन्यथा यदि (!__builtin_strcmp(CMP_STR, "<=")) {		\
		__res = ((s64)((A)-(B)) <= 0);			\
	} अन्यथा यदि (!__builtin_strcmp(CMP_STR, ">")) {		\
		__res = ((s64)((A)-(B)) > 0);			\
	} अन्यथा यदि (!__builtin_strcmp(CMP_STR, ">=")) {		\
		__res = ((s64)((A)-(B)) >= 0);			\
	} अन्यथा {						\
		/* Unknown operator throws linker error: */	\
		__BUILD_BUG_vruntime_cmp();			\
	}							\
								\
	__res;							\
})

बाह्य शून्य __BUILD_BUG_vruntime_op(शून्य);

#परिभाषय vruntime_op(A, OP_STR, B) ({				\
	s64 __res = 0;						\
								\
	यदि (!__builtin_strcmp(OP_STR, "-")) {			\
		__res = (s64)((A)-(B));				\
	} अन्यथा {						\
		/* Unknown operator throws linker error: */	\
		__BUILD_BUG_vruntime_op();			\
	}							\
								\
	__res;						\
})


स्थिर अन्तःस्थ __maybe_unused u64 max_vruntime(u64 max_vruntime, u64 vruntime)
{
	यदि (vruntime_cmp(vruntime, ">", max_vruntime))
		max_vruntime = vruntime;

	प्रत्यागम max_vruntime;
}

स्थिर अन्तःस्थ __maybe_unused u64 min_vruntime(u64 min_vruntime, u64 vruntime)
{
	यदि (vruntime_cmp(vruntime, "<", min_vruntime))
		min_vruntime = vruntime;

	प्रत्यागम min_vruntime;
}

स्थिर अन्तःस्थ bool entity_before(ध्रुव संरचना sched_entity *a,
				 ध्रुव संरचना sched_entity *b)
{
	/*
	 * Tiebreak on vruntime seems unnecessary since it can
	 * hardly happen.
	 */
	प्रत्यागम vruntime_cmp(a->deadline, "<", b->deadline);
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
स्थिर अन्तःस्थ s64 entity_कुञ्चिका(संरचना cfs_rq *cfs_rq, संरचना sched_entity *se)
{
	प्रत्यागम vruntime_op(se->vruntime, "-", cfs_rq->zero_vruntime);
}

#परिभाषय __node_2_se(ग्रन्थिः) \
	rb_प्रविष्टिः((ग्रन्थिः), संरचना sched_entity, सततम्_ग्रन्थिः)

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
स्थिर अन्तःस्थ अचिह्नित दीर्घ avg_vruntime_weight(संरचना cfs_rq *cfs_rq, अचिह्नित दीर्घ w)
{
#यदि_परिभाषितम् CONFIG_64BIT
	यदि (cfs_rq->योग_सरणम्)
		w = max(2UL, w >> cfs_rq->योग_सरणम्);
#यदि_अन्तः
	प्रत्यागम w;
}

स्थिर अन्तःस्थ शून्य
__sum_w_vruntime_add(संरचना cfs_rq *cfs_rq, संरचना sched_entity *se)
{
	अचिह्नित दीर्घ weight = avg_vruntime_weight(cfs_rq, se->h_आरोपय.weight);
	s64 w_vruntime, कुञ्चिका = entity_कुञ्चिका(cfs_rq, se);

	w_vruntime = कुञ्चिका * weight;
	WARN_ON_ONCE((w_vruntime >> 63) != (w_vruntime >> 62));

	cfs_rq->योग_w_vruntime += w_vruntime;
	cfs_rq->योग_weight += weight;
}

स्थिर शून्य
योग_w_vruntime_सङ्कलय_paranoid(संरचना cfs_rq *cfs_rq, संरचना sched_entity *se)
{
	अचिह्नित दीर्घ weight;
	s64 कुञ्चिका, tmp;

again:
	weight = avg_vruntime_weight(cfs_rq, se->h_आरोपय.weight);
	कुञ्चिका = entity_कुञ्चिका(cfs_rq, se);

	यदि (check_mul_अतिप्रवाहः(कुञ्चिका, weight, &कुञ्चिका))
		गच्छ अतिप्रवाहः;

	यदि (check_सङ्कलय_अतिप्रवाहः(cfs_rq->योग_w_vruntime, कुञ्चिका, &tmp))
		गच्छ अतिप्रवाहः;

	cfs_rq->योग_w_vruntime = tmp;
	cfs_rq->योग_weight += weight;
	प्रत्यागम;

अतिप्रवाहः:
	/*
	 * There's gotta be a limit -- if we're still failing at this point
	 * there's really nothing much to be done about things.
	 */
	BUG_ON(cfs_rq->योग_सरणम् >= 10);
	cfs_rq->योग_सरणम्++;

	/*
	 * Note: \Sum (k_i * (w_i >> 1)) != (\Sum (k_i * w_i)) >> 1
	 */
	cfs_rq->योग_w_vruntime = 0;
	cfs_rq->योग_weight = 0;

	प्रति (संरचना rb_ग्रन्थिः *ग्रन्थिः = cfs_rq->tasks_timeline.rb_leftmost;
	     ग्रन्थिः; ग्रन्थिः = rb_अग्र(ग्रन्थिः))
		__sum_w_vruntime_add(cfs_rq, __node_2_se(ग्रन्थिः));

	गच्छ again;
}

स्थिर शून्य
योग_w_vruntime_सङ्कलय(संरचना cfs_rq *cfs_rq, संरचना sched_entity *se)
{
	यदि (sched_feat(PARANOID_AVG))
		प्रत्यागम योग_w_vruntime_सङ्कलय_paranoid(cfs_rq, se);

	__sum_w_vruntime_add(cfs_rq, se);
}

स्थिर शून्य
योग_w_vruntime_sub(संरचना cfs_rq *cfs_rq, संरचना sched_entity *se)
{
	अचिह्नित दीर्घ weight = avg_vruntime_weight(cfs_rq, se->h_आरोपय.weight);
	s64 कुञ्चिका = entity_कुञ्चिका(cfs_rq, se);

	cfs_rq->योग_w_vruntime -= कुञ्चिका * weight;
	cfs_rq->योग_weight -= weight;
}

स्थिर अन्तःस्थ
शून्य update_zero_vruntime(संरचना cfs_rq *cfs_rq, s64 delta)
{
	/*
	 * v' = v + d ==> sum_w_vruntime' = sum_w_vruntime - d*sum_weight
	 */
	cfs_rq->योग_w_vruntime -= cfs_rq->योग_weight * delta;
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
u64 avg_vruntime(संरचना cfs_rq *cfs_rq)
{
	संरचना sched_entity *curr = cfs_rq->curr;
	दीर्घ weight = cfs_rq->योग_weight;
	s64 delta = 0;

	यदि (curr && !curr->on_rq)
		curr = रिक्त;

	यदि (weight) {
		s64 runtime = cfs_rq->योग_w_vruntime;

		यदि (curr) {
			अचिह्नित दीर्घ w = avg_vruntime_weight(cfs_rq, curr->h_आरोपय.weight);

			runtime += entity_कुञ्चिका(cfs_rq, curr) * w;
			weight += w;
		}

		/* sign flips effective floor / ceiling */
		यदि (runtime < 0)
			runtime -= (weight - 1);

		delta = div64_long(runtime, weight);
	} अन्यथा यदि (curr) {
		/*
		 * When there is but one element, it is the average.
		 */
		delta = curr->vruntime - cfs_rq->zero_vruntime;
	}

	update_zero_vruntime(cfs_rq, delta);

	प्रत्यागम cfs_rq->zero_vruntime;
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
स्थिर u64 ineligible_vruntime(संरचना cfs_rq *cfs_rq)
{
	संरचना sched_entity *curr = cfs_rq->curr;
	दीर्घ weight = cfs_rq->योग_weight;
	s64 delta = 0;

	यदि (curr && !curr->on_rq)
		curr = रिक्त;

	यदि (weight) {
		s64 runtime = cfs_rq->योग_w_vruntime;

		/*
		 * Do not add @curr to obtain the effective '- w_j' terms.
		 */

		/* sign flips effective floor / ceiling */
		यदि (runtime < 0)
			runtime -= (weight - 1);

		delta = div64_long(runtime, weight);
	}

	प्रत्यागम cfs_rq->zero_vruntime + delta + 1;
}

स्थिर अन्तःस्थ u64 cfs_rq_max_खण्डिका(संरचना cfs_rq *cfs_rq);

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
स्थिर s64 entity_lag(संरचना cfs_rq *cfs_rq, संरचना sched_entity *se, u64 avruntime)
{
	u64 max_खण्डिका = cfs_rq_max_खण्डिका(cfs_rq) + TICK_NSEC;
	s64 vlag, सीमा;

	vlag = avruntime - se->vruntime;
	सीमा = calc_delta_fair(max_खण्डिका, se);

	प्रत्यागम clamp(vlag, -सीमा, सीमा);
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
स्थिर __always_inline
bool update_entity_lag(संरचना cfs_rq *cfs_rq, संरचना sched_entity *se)
{
	u64 avruntime = avg_vruntime(cfs_rq);
	s64 vlag = entity_lag(cfs_rq, se, avruntime);

	यदि (se->sched_delayed) {
		/* previous vlag < 0 otherwise se would not be delayed */
		vlag = max(vlag, se->vlag);
		यदि (sched_feat(DELAY_ZERO))
			vlag = min(vlag, 0);
	}
	se->vlag = vlag;

	प्रत्यागम avruntime - vlag != se->vruntime;
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
स्थिर पूर्णाङ्क vruntime_eligible(संरचना cfs_rq *cfs_rq, u64 vruntime)
{
	संरचना sched_entity *curr = cfs_rq->curr;
	s64 कुञ्चिका, avg = cfs_rq->योग_w_vruntime;
	दीर्घ आरोपय = cfs_rq->योग_weight;

	यदि (curr && curr->on_rq) {
		अचिह्नित दीर्घ weight = avg_vruntime_weight(cfs_rq, curr->h_आरोपय.weight);

		avg += entity_कुञ्चिका(cfs_rq, curr) * weight;
		आरोपय += weight;
	}

	कुञ्चिका = vruntime_op(vruntime, "-", cfs_rq->zero_vruntime);

	/*
	 * The worst case term for @key includes 'NSEC_TICK * NICE_0_LOAD'
	 * and @load obviously includes NICE_0_LOAD. NSEC_TICK is around 24
	 * bits, while NICE_0_LOAD is 20 on 64bit and 10 otherwise.
	 *
	 * This gives that on 64bit the product will be at least 64bit which
	 * overflows s64, while on 32bit it will only be 44bits and should fit
	 * comfortably.
	 */
#यदि_परिभाषितम् CONFIG_64BIT
#यदि_परिभाषितम् CONFIG_ARCH_SUPPORTS_INT128
	/* This often results in simpler code than __builtin_mul_overflow(). */
	प्रत्यागम avg >= (__int128)कुञ्चिका * आरोपय;
#अन्यथा
	s64 rhs;
	/*
	 * On overflow, the sign of key tells us the correct answer: a large
	 * positive key means vruntime >> V, so not eligible; a large negative
	 * key means vruntime << V, so eligible.
	 */
	यदि (check_mul_अतिप्रवाहः(कुञ्चिका, आरोपय, &rhs))
		प्रत्यागम कुञ्चिका <= 0;

	प्रत्यागम avg >= rhs;
#यदि_अन्तः
#अन्यथा /* 32bit */
	प्रत्यागम avg >= कुञ्चिका * आरोपय;
#यदि_अन्तः
}

पूर्णाङ्क entity_eligible(संरचना cfs_rq *cfs_rq, संरचना sched_entity *se)
{
	प्रत्यागम vruntime_eligible(cfs_rq, se->vruntime);
}

स्थिर अन्तःस्थ u64 cfs_rq_min_खण्डिका(संरचना cfs_rq *cfs_rq)
{
	संरचना sched_entity *मूलम् = __pick_root_entity(cfs_rq);
	संरचना sched_entity *curr = cfs_rq->curr;
	u64 min_खण्डिका = ~0ULL;

	यदि (curr && curr->on_rq)
		min_खण्डिका = curr->खण्डिका;

	यदि (मूलम्)
		min_खण्डिका = min(min_खण्डिका, मूलम्->min_खण्डिका);

	प्रत्यागम min_खण्डिका;
}

स्थिर अन्तःस्थ u64 cfs_rq_max_खण्डिका(संरचना cfs_rq *cfs_rq)
{
	संरचना sched_entity *मूलम् = __pick_root_entity(cfs_rq);
	संरचना sched_entity *curr = cfs_rq->curr;
	u64 max_खण्डिका = 0ULL;

	यदि (curr && curr->on_rq)
		max_खण्डिका = curr->खण्डिका;

	यदि (मूलम्)
		max_खण्डिका = max(max_खण्डिका, मूलम्->max_खण्डिका);

	प्रत्यागम max_खण्डिका;
}

स्थिर अन्तःस्थ bool __entity_less(संरचना rb_ग्रन्थिः *a, ध्रुव संरचना rb_ग्रन्थिः *b)
{
	प्रत्यागम entity_before(__node_2_se(a), __node_2_se(b));
}

/* min() for wrapping vruntimes */
स्थिर अन्तःस्थ u64 __min_vruntime(u64 a, u64 b)
{
	प्रत्यागम vruntime_cmp(a, "<", b) ? a : b;
}

RB_DECLARE_CALLBACKS(स्थिर, min_vruntime_cb,
		     संरचना sched_entity, सततम्_ग्रन्थिः,
		     RB_AUG(vruntime, min_vruntime, __min_vruntime),
		     RB_AUG(खण्डिका, min_खण्डिका, min),
		     RB_AUG(खण्डिका, max_खण्डिका, max));

/*
 * Enqueue an entity into the rb-tree:
 */
स्थिर शून्य __enqueue_entity(संरचना cfs_rq *cfs_rq, संरचना sched_entity *se)
{
	WARN_ON_ONCE(&rq_सम्बन्धि(cfs_rq)->cfs != cfs_rq);
	WARN_ON_ONCE(!entity_is_task(se));

	योग_w_vruntime_सङ्कलय(cfs_rq, se);
	se->min_vruntime = se->vruntime;
	se->min_खण्डिका = se->खण्डिका;
	se->max_खण्डिका = se->खण्डिका;

	rb_सङ्कलय_augmented_cached(&se->सततम्_ग्रन्थिः, &cfs_rq->tasks_timeline,
				__entity_less, &min_vruntime_cb);
}

स्थिर शून्य __dequeue_entity(संरचना cfs_rq *cfs_rq, संरचना sched_entity *se)
{
	WARN_ON_ONCE(&rq_सम्बन्धि(cfs_rq)->cfs != cfs_rq);
	WARN_ON_ONCE(!entity_is_task(se));

	rb_erase_augmented_cached(&se->सततम्_ग्रन्थिः, &cfs_rq->tasks_timeline,
				  &min_vruntime_cb);
	योग_w_vruntime_sub(cfs_rq, se);
}

संरचना sched_entity *__pick_root_entity(संरचना cfs_rq *cfs_rq)
{
	संरचना rb_ग्रन्थिः *मूलम् = cfs_rq->tasks_timeline.rb_मूलम्.rb_ग्रन्थिः;

	यदि (!मूलम्)
		प्रत्यागम रिक्त;

	प्रत्यागम __node_2_se(मूलम्);
}

संरचना sched_entity *__pick_first_entity(संरचना cfs_rq *cfs_rq)
{
	संरचना rb_ग्रन्थिः *वामः = rb_first_cached(&cfs_rq->tasks_timeline);

	यदि (!वामः)
		प्रत्यागम रिक्त;

	प्रत्यागम __node_2_se(वामः);
}

/*
 * Set the vruntime up to which an entity can run before looking
 * for another entity to pick.
 * In case of run to parity, we use the shortest slice of the enqueued
 * entities to set the protected period.
 * When run to parity is disabled, we give a minimum quantum to the running
 * entity to ensure progress.
 */
स्थिर अन्तःस्थ शून्य समुच्चयः_protect_खण्डिका(संरचना cfs_rq *cfs_rq, संरचना sched_entity *se)
{
	u64 खण्डिका = normalized_sysctl_sched_आधारः_खण्डिका;
	u64 vprot = se->deadline;

	यदि (sched_feat(सततम्_गन्तव्यम्_PARITY))
		खण्डिका = cfs_rq_min_खण्डिका(cfs_rq);

	खण्डिका = min(खण्डिका, se->खण्डिका);

	/* If there are shorter slices than se's one */
	यदि (खण्डिका != se->खण्डिका) {
		vprot = min_vruntime(vprot, se->vruntime + calc_delta_fair(खण्डिका, se));
		यदि (sched_feat(PREEMPT_SHORT))
			vprot = min_vruntime(vprot, ineligible_vruntime(cfs_rq));
	}

	se->vprot = vprot;
}

स्थिर अन्तःस्थ शून्य update_protect_खण्डिका(संरचना cfs_rq *cfs_rq, संरचना sched_entity *se)
{
	u64 vruntime = min_vruntime(se->vruntime, avg_vruntime(cfs_rq));
	u64 खण्डिका = cfs_rq_min_खण्डिका(cfs_rq);
	u64 vprot = min_vruntime(se->vprot, vruntime + calc_delta_fair(खण्डिका, se));

	यदि (sched_feat(PREEMPT_SHORT) && खण्डिका != se->खण्डिका)
		vprot = min_vruntime(vprot, ineligible_vruntime(cfs_rq));

	se->vprot = vprot;
}

स्थिर अन्तःस्थ bool protect_खण्डिका(संरचना sched_entity *se)
{
	प्रत्यागम vruntime_cmp(se->vruntime, "<", se->vprot);
}

स्थिर अन्तःस्थ शून्य cancel_protect_खण्डिका(संरचना sched_entity *se)
{
	यदि (protect_खण्डिका(se))
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
स्थिर संरचना sched_entity *pick_eevdf(संरचना cfs_rq *cfs_rq, bool protect)
{
	संरचना rb_ग्रन्थिः *ग्रन्थिः = cfs_rq->tasks_timeline.rb_मूलम्.rb_ग्रन्थिः;
	संरचना sched_entity *se = __pick_first_entity(cfs_rq);
	संरचना sched_entity *curr = cfs_rq->curr;
	संरचना sched_entity *best = रिक्त;

	/*
	 * We can safely skip eligibility check if there is only one entity
	 * in this cfs_rq, saving some cycles.
	 */
	यदि (cfs_rq->h_nr_queued == 1)
		प्रत्यागम curr && curr->on_rq ? curr : se;

	/*
	 * Picking the ->next buddy will affect latency but not fairness.
	 */
	यदि (sched_feat(PICK_BUDDY) && protect &&
	    cfs_rq->अग्र && entity_eligible(cfs_rq, cfs_rq->अग्र)) {
		/* ->next will never be delayed */
		WARN_ON_ONCE(cfs_rq->अग्र->sched_delayed);
		प्रत्यागम cfs_rq->अग्र;
	}

	यदि (curr && (!curr->on_rq || !entity_eligible(cfs_rq, curr)))
		curr = रिक्त;

	यदि (curr && protect && protect_खण्डिका(curr))
		प्रत्यागम curr;

	/* Pick the leftmost entity if it's eligible */
	यदि (se && entity_eligible(cfs_rq, se)) {
		best = se;
		गच्छ प्राप्तम्;
	}

	/* Heap search for the EEVD entity */
	यावत् (ग्रन्थिः) {
		संरचना rb_ग्रन्थिः *वामः = ग्रन्थिः->rb_वामः;

		/*
		 * Eligible entities in left subtree are always better
		 * choices, since they have earlier deadlines.
		 */
		यदि (वामः && vruntime_eligible(cfs_rq,
					__node_2_se(वामः)->min_vruntime)) {
			ग्रन्थिः = वामः;
			अनुवर्त;
		}

		se = __node_2_se(ग्रन्थिः);

		/*
		 * The left subtree either is empty or has no eligible
		 * entity, so check the current node since it is the one
		 * with earliest deadline that might be eligible.
		 */
		यदि (entity_eligible(cfs_rq, se)) {
			best = se;
			भङ्ग;
		}

		ग्रन्थिः = ग्रन्थिः->rb_दक्षिणः;
	}
प्राप्तम्:
	यदि (!best || (curr && entity_before(curr, best)))
		best = curr;

	प्रत्यागम best;
}

संरचना sched_entity *__pick_last_entity(संरचना cfs_rq *cfs_rq)
{
	संरचना rb_ग्रन्थिः *चरमः = rb_चरमः(&cfs_rq->tasks_timeline.rb_मूलम्);

	यदि (!चरमः)
		प्रत्यागम रिक्त;

	प्रत्यागम __node_2_se(चरमः);
}

/**************************************************************
 * Scheduling class statistics methods:
 */
पूर्णाङ्क sched_update_scaling(शून्य)
{
	अचिह्नित पूर्णाङ्क गुणकः = get_update_sysctl_गुणकः();

#परिभाषय WRT_SYSCTL(name) \
	(normalized_sysctl_##name = sysctl_##name / (गुणकः))
	WRT_SYSCTL(sched_आधारः_खण्डिका);
#अपरिभाषय WRT_SYSCTL

	प्रत्यागम 0;
}

स्थिर शून्य clear_buddies(संरचना cfs_rq *cfs_rq, संरचना sched_entity *se);

/*
 * XXX: strictly: vd_i += N*r_i/w_i such that: vd_i > ve_i
 * this is probably good enough.
 */
स्थिर bool update_deadline(संरचना cfs_rq *cfs_rq, संरचना sched_entity *se)
{
	यदि (vruntime_cmp(se->vruntime, "<", se->deadline))
		प्रत्यागम false;

	/*
	 * For EEVDF the virtual time slope is determined by w_i (iow.
	 * nice) while the request time r_i is determined by
	 * sysctl_sched_base_slice.
	 */
	यदि (!se->custom_खण्डिका)
		se->खण्डिका = sysctl_sched_आधारः_खण्डिका;

	/*
	 * EEVDF: vd_i = ve_i + r_i / w_i
	 */
	se->deadline = se->vruntime + calc_delta_fair(se->खण्डिका, se);
	avg_vruntime(cfs_rq);

	/*
	 * The task has consumed its request, reschedule.
	 */
	प्रत्यागम true;
}

#अन्तर्भाव "pelt.h"

स्थिर पूर्णाङ्क select_idle_sibling(संरचना task_struct *p, पूर्णाङ्क पूर्व_cpu, पूर्णाङ्क cpu);
स्थिर अचिह्नित दीर्घ task_h_आरोपय(संरचना task_struct *p);
स्थिर अचिह्नित दीर्घ क्षमता_सम्बन्धि(पूर्णाङ्क cpu);

/* Give new sched_entity start runnable values to heavy its load in infant time */
शून्य init_entity_runnable_मध्यमानम्(संरचना sched_entity *se)
{
	संरचना sched_avg *sa = &se->avg;

	स्मृतिं_पूरय(sa, 0, परिमाण(*sa));

	/*
	 * Tasks are initialized with full load to be seen as heavy tasks until
	 * they get a chance to stabilize to their real load level.
	 * Group entities are initialized with zero load to reflect the fact that
	 * nothing has been attached to the task group yet.
	 */
	यदि (entity_is_task(se))
		sa->आरोपय_avg = scale_आरोपय_अधस्तात्(se->आरोपय.weight);

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
शून्य post_init_entity_util_avg(संरचना task_struct *p)
{
	संरचना sched_entity *se = &p->se;
	संरचना cfs_rq *cfs_rq = cfs_rq_सम्बन्धि(se);
	संरचना sched_avg *sa = &se->avg;
	दीर्घ cpu_scale = arch_scale_cpu_क्षमता(cpu_सम्बन्धि(rq_सम्बन्धि(cfs_rq)));
	दीर्घ cap = (दीर्घ)(cpu_scale - cfs_rq->avg.util_avg) / 2;

	यदि (p->sched_class != &fair_sched_class) {
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
		se->avg.चरमः_update_time = cfs_rq_clock_pelt(cfs_rq);
		प्रत्यागम;
	}

	यदि (cap > 0) {
		यदि (cfs_rq->avg.util_avg != 0) {
			sa->util_avg  = cfs_rq->avg.util_avg * se_weight(se);
			sa->util_avg /= (cfs_rq->avg.आरोपय_avg + 1);

			यदि (sa->util_avg > cap)
				sa->util_avg = cap;
		} अन्यथा {
			sa->util_avg = cap;
		}
	}

	sa->runnable_avg = sa->util_avg;
}

स्थिर अन्तःस्थ शून्य account_mm_sched(संरचना rq *rq, संरचना task_struct *p, s64 delta_exec);

स्थिर s64 update_se(संरचना rq *rq, संरचना sched_entity *se)
{
	u64 now = rq_clock_task(rq);
	s64 delta_exec;

	delta_exec = now - se->exec_start;
	यदि (unlikely(delta_exec <= 0))
		प्रत्यागम delta_exec;

	se->exec_start = now;
	यदि (entity_is_task(se)) {
		संरचना task_struct *running = rq->curr;
		/*
		 * If se is a task, we account the time against the running
		 * task, as w/ proxy-exec they may not be the same.
		 */
		running->se.exec_start = now;
		running->se.योग_exec_runtime += delta_exec;

		trace_sched_stat_runtime(running, delta_exec);
		account_गणः_exec_runtime(running, delta_exec);
		account_mm_sched(rq, running, delta_exec);

		cgroup_account_cputime(running, delta_exec);
	} अन्यथा {
		/* If not task, account the time against donor se  */
		se->योग_exec_runtime += delta_exec;
	}

	यदि (schedstat_enabled()) {
		संरचना sched_statistics *stats;

		stats = __schedstats_from_se(se);
		__schedstat_set(stats->exec_max,
				max(delta_exec, stats->exec_max));
	}

	प्रत्यागम delta_exec;
}

#यदि_परिभाषितम् CONFIG_SCHED_CACHE

/*
 * XXX numbers come from a place the sun don't shine -- probably wants to be SD
 * tunable or so.
 */
#परिभाषय EPOCH_PERIOD	(HZ / 100)	/* 10 ms */
#परिभाषय EPOCH_LLC_AFFINITY_TIMEOUT	5	/* 50 ms */
__read_mostly अचिह्नित पूर्णाङ्क llc_aggr_tolerance	= 1;
__read_mostly अचिह्नित पूर्णाङ्क llc_epoch_period	= EPOCH_PERIOD;
__read_mostly अचिह्नित पूर्णाङ्क llc_epoch_affinity_timeout = EPOCH_LLC_AFFINITY_TIMEOUT;
__read_mostly अचिह्नित पूर्णाङ्क llc_imb_pct		= 20;
__read_mostly अचिह्नित पूर्णाङ्क llc_overaggr_pct	= 50;

स्थिर पूर्णाङ्क llc_id(पूर्णाङ्क cpu)
{
	यदि (cpu < 0)
		प्रत्यागम -1;

	प्रत्यागम प्रत्येकम्_cpu(sd_llc_id, cpu);
}

स्थिर अन्तःस्थ पूर्णाङ्क get_sched_cache_scale(पूर्णाङ्क mul)
{
	अचिह्नित पूर्णाङ्क tol = READ_ONCE(llc_aggr_tolerance);

	यदि (!tol)
		प्रत्यागम 0;

	यदि (tol >= 100)
		प्रत्यागम अधिकतम_पूर्णाङ्कः;

	प्रत्यागम (1 + (tol - 1) * mul);
}

स्थिर bool exceed_llc_क्षमता(संरचना sched_cache_गणः *grp, पूर्णाङ्क cpu)
{
#यदि_परिभाषितम् CONFIG_NUMA_BALANCING
	अचिह्नित दीर्घ llc, footprint;
	संरचना sched_domain *sd;
	पूर्णाङ्क scale;

	guard(rcu)();

	sd = rcu_dereference_sched_domain(cpu_rq(cpu)->sd);
	यदि (!sd)
		प्रत्यागम true;

	यदि (static_branch_likely(&sched_numa_balancing)) {
		/*
		 * TBD: RDT exclusive LLC ways reserved should be
		 * excluded.
		 */
		llc = sd->llc_अष्टकानि;
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
		यदि (scale == अधिकतम_पूर्णाङ्कः)
			प्रत्यागम false;

		प्रत्यागम ((llc * (u64)scale) < (footprint * PAGE_आकार));
	}
#यदि_अन्तः
	प्रत्यागम false;
}

स्थिर bool invalid_llc_nr(संरचना sched_cache_गणः *grp, संरचना task_struct *p,
			   पूर्णाङ्क cpu)
{
	पूर्णाङ्क scale;

	यदि (get_nr_तन्तवः(p) <= 1)
		प्रत्यागम true;

	/*
	 * Scale the number of 'cores' in a LLC by llc_aggr_tolerance
	 * and compare it to the task's active threads.
	 */
	scale = get_sched_cache_scale(1);
	यदि (scale == अधिकतम_पूर्णाङ्कः)
		प्रत्यागम false;

	प्रत्यागम !माति_क्षमता((READ_ONCE(grp->nr_running_avg) * cpu_smt_अङ्कः_तन्तवः),
			(scale * प्रत्येकम्_cpu(sd_llc_आकार, cpu)));
}

/*
 * A task counts in nr_pref_llc_running while it is queued on its preferred
 * LLC (pref_llc_queued) and runnable (!sched_delayed), keeping the counter in
 * the runnable domain so alb_break_llc() can compare it with h_nr_runnable.
 */
स्थिर bool task_pref_llc_runnable(संरचना task_struct *p)
{
	प्रत्यागम p->pref_llc_queued && !p->se.sched_delayed;
}

स्थिर शून्य pref_llc_running_inc(संरचना rq *rq, संरचना task_struct *p)
{
	यदि (task_pref_llc_runnable(p))
		rq->nr_pref_llc_running++;
}

स्थिर शून्य pref_llc_running_dec(संरचना rq *rq, संरचना task_struct *p)
{
	यदि (task_pref_llc_runnable(p))
		rq->nr_pref_llc_running--;
}

स्थिर शून्य account_llc_पङ्क्तौ_निक्षिप(संरचना rq *rq, संरचना task_struct *p)
{
	पूर्णाङ्क pref_llc, pref_llc_queued;
	संरचना sched_domain *sd;

	pref_llc = p->preferred_llc;
	यदि (pref_llc < 0)
		प्रत्यागम;

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
	यदि (sd && (अचिह्नित पूर्णाङ्क)pref_llc < sd->llc_max)
		sd->llc_counts[pref_llc]++;
}

स्थिर शून्य account_llc_पङ्क्तेः_निष्कासय(संरचना rq *rq, संरचना task_struct *p)
{
	संरचना sched_domain *sd;
	पूर्णाङ्क pref_llc;

	pref_llc = p->preferred_llc;
	यदि (pref_llc < 0)
		प्रत्यागम;

	rq->nr_llc_running--;
	यदि (p->pref_llc_queued) {
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
	यदि (sd && (अचिह्नित पूर्णाङ्क)pref_llc < sd->llc_max) {
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
		यदि (sd->llc_counts[pref_llc])
			sd->llc_counts[pref_llc]--;
	}
}

पूर्णाङ्क mm_init_sched(संरचना mm_struct *mm,
		  संरचना sched_cache_time __percpu *_pcpu_sched)
{
	संरचना sched_cache_गणः *grp;
	अचिह्नित दीर्घ epoch = 0;
	पूर्णाङ्क i;

	grp = kzalloc_obj(*grp);
	यदि (!grp) {
		free_percpu(_pcpu_sched);
		mm->sched_cache_grp = रिक्त;
		प्रत्यागम -ENOMEM;
	}

	for_each_possible_cpu(i) {
		संरचना sched_cache_time *pcpu_sched = प्रत्येकम्_cpu_ptr(_pcpu_sched, i);
		संरचना rq *rq = cpu_rq(i);

		pcpu_sched->runtime = 0;
		/* a slightly stale cpu epoch is acceptible */
		pcpu_sched->epoch = rq->cpu_epoch;
		epoch = rq->cpu_epoch;
	}

	raw_spin_तालकम्_init(&grp->तालकम्);
	grp->epoch = epoch;
	grp->cpu = -1;
	grp->अग्र_अवलोकनम् = jiffies;
	grp->nr_running_avg = 0;
	grp->footprint = 0;
	refcount_समुच्चयः(&grp->refcnt, 1);
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
	प्रत्यागम 0;
}

स्थिर शून्य sched_cache_गणः_free_rcu(संरचना rcu_शीर्ष *rcu)
{
	संरचना sched_cache_गणः *grp =
		container_सम्बन्धि(rcu, संरचना sched_cache_गणः, rcu);

	free_percpu(grp->pcpu_sched);
	kfree(grp);
}

स्थिर शून्य sched_cache_गणः_put(संरचना sched_cache_गणः *grp)
{
	यदि (!grp || !refcount_dec_and_test(&grp->refcnt))
		प्रत्यागम;

	call_rcu(&grp->rcu, sched_cache_गणः_free_rcu);
}

DEFINE_FREE(sched_cache_गणः_put, संरचना sched_cache_गणः *,
	    sched_cache_गणः_put(_T));

#परिभाषय rcu_deref_sched_cache_grp(tsk) \
	rcu_dereference_check((tsk)->sched_cache_grp, (tsk) == current)

स्थिर संरचना sched_cache_गणः *sched_cache_replace_grp(संरचना task_struct *p,
							 संरचना sched_cache_गणः *new)
{
	संरचना sched_cache_गणः *old;

	old = rcu_deref_sched_cache_grp(p);
	rcu_नियोजय_pointer(p->sched_cache_grp, new);

	प्रत्यागम old;
}

संरचना sched_cache_गणः *sched_cache_गणः_get(संरचना sched_cache_गणः *grp)
{
	/*
	 * refcount_inc_not_zero() is the acquire primitive for lockless
	 * (RCU) lookups; plain refcount_inc() would scribble the count if
	 * it already reached zero. Return NULL in that case.
	 */
	यदि (grp && !refcount_inc_not_zero(&grp->refcnt))
		grp = रिक्त;

	प्रत्यागम grp;
}

संरचना sched_cache_गणः *task_cache_गणः_get(संरचना task_struct *p)
{
	guard(rcu)();
	प्रत्यागम sched_cache_गणः_get(rcu_dereference(p->sched_cache_grp));
}

शून्य sched_cache_fork(संरचना task_struct *p)
{
	/*
	 * The child takes its own reference on the mm's cache group, separate
	 * from the reference held by the mm. @p is not yet visible to readers,
	 * so a plain initializing store is enough.
	 */
	RCU_INIT_POINTER(p->sched_cache_grp,
			 sched_cache_गणः_get(p->mm->sched_cache_grp));
}

शून्य sched_cache_fork_cleanup(संरचना task_struct *p)
{
	/*
	 * A fork that fails after sched_cache_fork() never reaches exit_mm(),
	 * so drop the reference here. @p never became visible, so there are no
	 * concurrent readers and the reference we hold keeps the group alive.
	 */
	sched_cache_गणः_put(rcu_access_pointer(p->sched_cache_grp));
	RCU_INIT_POINTER(p->sched_cache_grp, रिक्त);
}

शून्य sched_cache_exec_mmap(संरचना task_struct *p, संरचना mm_struct *mm)
{
	संरचना sched_cache_गणः *old;

	/*
	 * Acquire the new reference before publishing the pointer, then drop
	 * the old one. @p is current and the only writer of its own pointer.
	 */
	old = sched_cache_replace_grp(p, sched_cache_गणः_get(mm->sched_cache_grp));
	sched_cache_गणः_put(old);
}

शून्य sched_cache_exit_mm(संरचना task_struct *p)
{
	संरचना sched_cache_गणः *grp = sched_cache_replace_grp(p, रिक्त);

#यदि_परिभाषितम् CONFIG_NUMA_BALANCING
	/*
	 * Subtract this task's footprint from the group before dropping the
	 * reference, so the group footprint converges as its threads exit.
	 * Unlocked for performance; clamp to avoid underflow.
	 */
	यदि (grp && p->योग_numa_दोषाः) {
		अचिह्नित दीर्घ fp = READ_ONCE(grp->footprint);
		अचिह्नित दीर्घ sub = min(fp, p->योग_numa_दोषाः);

		WRITE_ONCE(grp->footprint, fp - sub);
	}
#यदि_अन्तः
	sched_cache_गणः_put(grp);
}

शून्य mm_destroy_sched(संरचना mm_struct *mm)
{
	sched_cache_गणः_put(mm->sched_cache_grp);
	mm->sched_cache_grp = रिक्त;
}

/* because why would C be fully specified */
स्थिर __always_inline शून्य __shr_u64(u64 *val, अचिह्नित पूर्णाङ्क n)
{
	यदि (n >= 64) {
		*val = 0;
		प्रत्यागम;
	}
	*val >>= n;
}

स्थिर अन्तःस्थ शून्य __update_mm_sched(संरचना rq *rq,
				     संरचना sched_cache_time *pcpu_sched)
{
	lockdep_assert_held(&rq->cpu_epoch_तालकम्);

	अचिह्नित पूर्णाङ्क period = max(READ_ONCE(llc_epoch_period), 1U);
	अचिह्नित दीर्घ n, now = jiffies;
	दीर्घ delta = now - rq->cpu_epoch_अग्र;

	यदि (delta > 0) {
		n = (delta + period - 1) / period;
		rq->cpu_epoch += n;
		rq->cpu_epoch_अग्र += n * period;
		__shr_u64(&rq->cpu_runtime, n);
	}

	n = rq->cpu_epoch - pcpu_sched->epoch;
	यदि (n) {
		pcpu_sched->epoch += n;
		__shr_u64(&pcpu_sched->runtime, n);
	}
}

स्थिर अचिह्नित दीर्घ fraction_mm_sched(संरचना rq *rq,
				       संरचना sched_cache_time *pcpu_sched)
{
	guard(raw_spinlock_irqsave)(&rq->cpu_epoch_तालकम्);

	__update_mm_sched(rq, pcpu_sched);

	/*
	 * Runtime is a geometric series (r=0.5) and as such will sum to twice
	 * the accumulation period, this means the multiplcation here should
	 * not overflow.
	 */
	प्रत्यागम div64_u64(NICE_0_आरोपय * pcpu_sched->runtime, rq->cpu_runtime + 1);
}

स्थिर पूर्णाङ्क get_pref_llc(संरचना task_struct *p, संरचना sched_cache_गणः *grp)
{
	पूर्णाङ्क mm_sched_llc = -1, mm_sched_cpu;

	यदि (!grp)
		प्रत्यागम -1;

	mm_sched_cpu = READ_ONCE(grp->cpu);
	यदि (mm_sched_cpu != -1) {
		mm_sched_llc = llc_id(mm_sched_cpu);

#यदि_परिभाषितम् CONFIG_NUMA_BALANCING
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
		यदि (static_branch_likely(&sched_numa_balancing) &&
		    p->numa_preferred_nid >= 0 &&
		    cpu_गन्तव्यम्_ग्रन्थिः(mm_sched_cpu) != p->numa_preferred_nid)
			mm_sched_llc = -1;
#यदि_अन्तः
	}

	प्रत्यागम mm_sched_llc;
}

स्थिर अचिह्नित पूर्णाङ्क task_running_on_cpu(पूर्णाङ्क cpu, संरचना task_struct *p);

स्थिर अन्तःस्थ
शून्य account_mm_sched(संरचना rq *rq, संरचना task_struct *p, s64 delta_exec)
{
	संरचना sched_cache_गणः *grp = rcu_dereference_all(p->sched_cache_grp);
	संरचना sched_cache_time *pcpu_sched;
	पूर्णाङ्क mm_sched_llc = -1;
	अचिह्नित दीर्घ epoch;

	यदि (!sched_cache_enabled())
		प्रत्यागम;

	यदि (p->sched_class != &fair_sched_class)
		प्रत्यागम;
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
	यदि (p->flags & PF_KTHREAD || !grp || !grp->pcpu_sched)
		प्रत्यागम;

	pcpu_sched = प्रत्येकम्_cpu_ptr(grp->pcpu_sched, cpu_सम्बन्धि(rq));

	scoped_guard (raw_spinlock, &rq->cpu_epoch_तालकम्) {
		__update_mm_sched(rq, pcpu_sched);
		pcpu_sched->runtime += delta_exec;
		rq->cpu_runtime += delta_exec;
		epoch = rq->cpu_epoch;
	}

	/*
	 * If this process hasn't hit task_cache_work() for a while invalidate
	 * its preferred state.
	 */
	यदि ((दीर्घ)(epoch - READ_ONCE(grp->epoch)) > llc_epoch_affinity_timeout ||
	    invalid_llc_nr(grp, p, cpu_सम्बन्धि(rq)) ||
	    exceed_llc_क्षमता(grp, cpu_सम्बन्धि(rq))) {
		यदि (READ_ONCE(grp->cpu) != -1)
			WRITE_ONCE(grp->cpu, -1);
	}

	mm_sched_llc = get_pref_llc(p, grp);

	/* task not on rq accounted later in account_entity_enqueue() */
	यदि (task_running_on_cpu(rq->cpu, p) &&
	    READ_ONCE(p->preferred_llc) != mm_sched_llc) {
		account_llc_पङ्क्तेः_निष्कासय(rq, p);
		WRITE_ONCE(p->preferred_llc, mm_sched_llc);
		account_llc_पङ्क्तौ_निक्षिप(rq, p);
	}
}

स्थिर शून्य task_tick_cache(संरचना rq *rq, संरचना task_struct *p)
{
	संरचना sched_cache_गणः *grp = rcu_dereference_all(p->sched_cache_grp);
	संरचना callback_शीर्ष *work = &p->cache_work;
	अचिह्नित दीर्घ epoch;

	यदि (!sched_cache_enabled())
		प्रत्यागम;

	यदि (!grp || p->flags & PF_KTHREAD ||
	    !grp->pcpu_sched)
		प्रत्यागम;

	epoch = rq->cpu_epoch;
	/* avoid moving backwards */
	यदि (time_after_eq(grp->epoch, epoch))
		प्रत्यागम;

	guard(raw_spinlock)(&grp->तालकम्);

	यदि (work->अग्र == work) {
		task_work_सङ्कलय(p, work, TWA_RESUME);
		WRITE_ONCE(grp->epoch, epoch);
	}
}

स्थिर शून्य get_अवलोकनम्_cpumasks(cpumask_var_t cpus, संरचना task_struct *p,
			      संरचना sched_cache_गणः *grp)
{
#यदि_परिभाषितम् CONFIG_NUMA_BALANCING
	पूर्णाङ्क cpu, curr_cpu, nid, pref_nid;

	यदि (!static_branch_likely(&sched_numa_balancing))
		गच्छ बहिर्गमनम्;

	cpu = READ_ONCE(grp->cpu);
	यदि (cpu != -1)
		nid = cpu_गन्तव्यम्_ग्रन्थिः(cpu);
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
	यदि (pref_nid == NUMA_NO_ग्रन्थिः)
		गच्छ बहिर्गमनम्;

	cpumask_or(cpus, cpus, cpumask_सम्बन्धि_ग्रन्थिः(pref_nid));

	/* honor the task's preferred LLC CPU */
	यदि (cpu != -1 && !cpumask_test_cpu(cpu, cpus) && nid != NUMA_NO_ग्रन्थिः)
		cpumask_or(cpus, cpus, cpumask_सम्बन्धि_ग्रन्थिः(nid));

	/* make sure the task's current running node is included */
	यदि (!cpumask_test_cpu(curr_cpu, cpus))
		cpumask_or(cpus, cpus, cpumask_सम्बन्धि_ग्रन्थिः(cpu_गन्तव्यम्_ग्रन्थिः(curr_cpu)));

	प्रत्यागम;

बहिर्गमनम्:
#यदि_अन्तः
	cpumask_copy(cpus, cpu_online_आवरणम्);
}

स्थिर अन्तःस्थ शून्य update_avg_scale(u64 *avg, u64 sample)
{
	पूर्णाङ्क गुणकः = प्रत्येकम्_cpu(sd_llc_आकार, raw_smp_processor_id());
	s64 diff = sample - *avg;
	u32 भाजकः;

	/*
	 * Scale the divisor based on the number of CPUs contained
	 * in the LLC. This scaling ensures smaller LLC domains use
	 * a smaller divisor to achieve more precise sensitivity to
	 * changes in nr_running, while larger LLC domains are capped
	 * at a maximum divisor of 8 which is the default smoothing
	 * factor of EWMA in update_avg().
	 */
	भाजकः = clamp_t(u32, (गुणकः >> 2), 2, 8);
	*avg += div64_s64(diff, भाजकः);
}

स्थिर शून्य task_cache_work(संरचना callback_शीर्ष *work)
{
	संरचना sched_cache_गणः *grp __free(sched_cache_गणः_put) = रिक्त;
	cpumask_var_t cpus __free(free_cpumask_var) = CPUMASK_VAR_NULL;
	पूर्णाङ्क cpu, m_a_cpu = -1, nr_running = 0, curr_cpu;
	अचिह्नित दीर्घ अग्र_अवलोकनम्, now = jiffies;
	संरचना task_struct *p = current, *cur;
	अचिह्नित दीर्घ curr_m_a_occ = 0;
	अचिह्नित दीर्घ m_a_occ = 0;

	WARN_ON_ONCE(work != &p->cache_work);

	work->अग्र = work;

	यदि (p->flags & PF_EXITING)
		प्रत्यागम;

	/*
	 * A reference makes sure grp is not released by others. The rcu
	 * lock can not be held till after zalloc_cpumask_var() below,
	 * because the latter might sleep.
	 */
	grp = task_cache_गणः_get(p);
	यदि (!grp)
		प्रत्यागम;

	अग्र_अवलोकनम् = READ_ONCE(grp->अग्र_अवलोकनम्);
	यदि (time_before(now, अग्र_अवलोकनम्))
		प्रत्यागम;

	/* only 1 thread is allowed to scan */
	यदि (!try_cmpxchg(&grp->अग्र_अवलोकनम्, &अग्र_अवलोकनम्,
			 now + max_t(अचिह्नित दीर्घ,
				     READ_ONCE(llc_epoch_period), 1)))
		प्रत्यागम;

	curr_cpu = task_cpu(p);
	यदि (invalid_llc_nr(grp, p, curr_cpu) ||
	    exceed_llc_क्षमता(grp, curr_cpu)) {
		यदि (READ_ONCE(grp->cpu) != -1)
			WRITE_ONCE(grp->cpu, -1);

		प्रत्यागम;
	}

	यदि (!zalloc_cpumask_var(&cpus, GFP_KERNEL))
		प्रत्यागम;

	scoped_guard (cpus_read_तालकम्) {
		guard(rcu)();

		get_अवलोकनम्_cpumasks(cpus, p, grp);

		for_each_cpu(cpu, cpus) {
			/* XXX sched_cluster_active */
			संरचना sched_domain *sd = rcu_dereference_all(प्रत्येकम्_cpu(sd_llc, cpu));
			अचिह्नित दीर्घ occ, m_occ = 0, a_occ = 0;
			पूर्णाङ्क m_cpu = -1, i;

			यदि (!sd)
				अनुवर्त;

			for_each_cpu(i, sched_domain_विस्तृतिः(sd)) {
				occ = fraction_mm_sched(cpu_rq(i),
							प्रत्येकम्_cpu_ptr(grp->pcpu_sched, i));
				a_occ += occ;
				यदि (occ > m_occ) {
					m_occ = occ;
					m_cpu = i;
				}

				/*
				 * rcu_access_pointer() is used because the
				 * pointer is only compared, never dereferenced.
				 */
				cur = rcu_dereference_all(cpu_rq(i)->curr);
				यदि (cur && !(cur->flags & (PF_EXITING | PF_KTHREAD)) &&
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
			यदि (a_occ > m_a_occ) {
				m_a_occ = a_occ;
				m_a_cpu = m_cpu;
			}

			यदि (llc_id(cpu) == llc_id(READ_ONCE(grp->cpu)))
				curr_m_a_occ = a_occ;

			cpumask_andnot(cpus, cpus, sched_domain_विस्तृतिः(sd));
		}
	}

	यदि (m_a_occ > (2 * curr_m_a_occ)) {
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

शून्य init_sched_mm(संरचना task_struct *p)
{
	संरचना callback_शीर्ष *work = &p->cache_work;

	init_task_work(work, task_cache_work);
	work->अग्र = work;
	/*
	 * dup_task_struct() copies the parent's task_struct, including its
	 * sched_cache_grp, for which the child holds no reference.  Clear it
	 * here - before copy_mm() runs - so the child never carries a
	 * borrowed pointer that the fork error path would put.
	 */
	RCU_INIT_POINTER(p->sched_cache_grp, रिक्त);
	/*
	 * Reset new task's preference to avoid
	 * polluting account_llc_enqueue().
	 */
	p->preferred_llc = -1;
	p->pref_llc_queued = 0;
}

#अन्यथा /* CONFIG_SCHED_CACHE */

स्थिर अन्तःस्थ शून्य account_mm_sched(संरचना rq *rq, संरचना task_struct *p,
				    s64 delta_exec) { }

शून्य init_sched_mm(संरचना task_struct *p) { }

स्थिर शून्य task_tick_cache(संरचना rq *rq, संरचना task_struct *p) { }

स्थिर अन्तःस्थ पूर्णाङ्क get_pref_llc(संरचना task_struct *p,
			       संरचना mm_struct *mm)
{
	प्रत्यागम -1;
}

स्थिर शून्य account_llc_पङ्क्तौ_निक्षिप(संरचना rq *rq, संरचना task_struct *p) {}

स्थिर शून्य account_llc_पङ्क्तेः_निष्कासय(संरचना rq *rq, संरचना task_struct *p) {}

स्थिर शून्य pref_llc_running_inc(संरचना rq *rq, संरचना task_struct *p) {}

स्थिर शून्य pref_llc_running_dec(संरचना rq *rq, संरचना task_struct *p) {}

#यदि_अन्तः /* CONFIG_SCHED_CACHE */

/*
 * Used by other classes to account runtime.
 */
s64 update_curr_common(संरचना rq *rq)
{
	प्रत्यागम update_se(rq, &rq->donor->se);
}

/*
 * Update the current task's runtime statistics.
 */
स्थिर शून्य update_curr(संरचना cfs_rq *cfs_rq)
{
	/*
	 * Note: cfs_rq->curr corresponds to the task picked to
	 * run (ie: rq->donor.se) which due to proxy-exec may
	 * not necessarily be the actual task running
	 * (rq->curr.se). This is easy to confuse!
	 */
	संरचना sched_entity *curr = cfs_rq->h_curr;
	संरचना rq *rq = rq_सम्बन्धि(cfs_rq);
	s64 delta_exec;
	bool resched;

	यदि (unlikely(!curr))
		प्रत्यागम;

	delta_exec = update_se(rq, curr);
	यदि (unlikely(delta_exec <= 0))
		प्रत्यागम;

	account_cfs_rq_runtime(cfs_rq, delta_exec);

	यदि (!entity_is_task(curr))
		प्रत्यागम;

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

	यदि (cfs_rq->h_nr_queued == 1)
		प्रत्यागम;

	यदि (resched || !protect_खण्डिका(curr)) {
		resched_curr_lazy(rq);
		clear_buddies(cfs_rq, curr);
	}
}

स्थिर शून्य update_curr_fair(संरचना rq *rq)
{
	संरचना sched_entity *se = &rq->donor->se;

	for_each_sched_entity(se)
		update_curr(cfs_rq_सम्बन्धि(se));
}

स्थिर अन्तःस्थ शून्य
update_stats_wait_start_fair(संरचना cfs_rq *cfs_rq, संरचना sched_entity *se)
{
	संरचना sched_statistics *stats;
	संरचना task_struct *p = रिक्त;

	यदि (!schedstat_enabled())
		प्रत्यागम;

	stats = __schedstats_from_se(se);

	यदि (entity_is_task(se))
		p = task_सम्बन्धि(se);

	__update_stats_wait_start(rq_सम्बन्धि(cfs_rq), p, stats);
}

स्थिर अन्तःस्थ शून्य
update_stats_wait_अन्तः_fair(संरचना cfs_rq *cfs_rq, संरचना sched_entity *se)
{
	संरचना sched_statistics *stats;
	संरचना task_struct *p = रिक्त;

	यदि (!schedstat_enabled())
		प्रत्यागम;

	stats = __schedstats_from_se(se);

	/*
	 * When the sched_schedstat changes from 0 to 1, some sched se
	 * maybe already in the runqueue, the se->statistics.wait_start
	 * will be 0.So it will let the delta wrong. We need to avoid this
	 * scenario.
	 */
	यदि (unlikely(!schedstat_val(stats->wait_start)))
		प्रत्यागम;

	यदि (entity_is_task(se))
		p = task_सम्बन्धि(se);

	__update_stats_wait_end(rq_सम्बन्धि(cfs_rq), p, stats);
}

स्थिर अन्तःस्थ शून्य
update_stats_पङ्क्तौ_निक्षिप_sleeper_fair(संरचना cfs_rq *cfs_rq, संरचना sched_entity *se)
{
	संरचना sched_statistics *stats;
	संरचना task_struct *tsk = रिक्त;

	यदि (!schedstat_enabled())
		प्रत्यागम;

	stats = __schedstats_from_se(se);

	यदि (entity_is_task(se))
		tsk = task_सम्बन्धि(se);

	__update_stats_enqueue_sleeper(rq_सम्बन्धि(cfs_rq), tsk, stats);
}

/*
 * Task is being enqueued - update stats:
 */
स्थिर अन्तःस्थ शून्य
update_stats_पङ्क्तौ_निक्षिप_fair(संरचना cfs_rq *cfs_rq, संरचना sched_entity *se, पूर्णाङ्क flags)
{
	यदि (!schedstat_enabled())
		प्रत्यागम;

	/*
	 * Are we enqueueing a waiting task? (for current tasks
	 * a dequeue/enqueue event is a NOP)
	 */
	यदि (se != cfs_rq->h_curr)
		update_stats_wait_start_fair(cfs_rq, se);

	यदि (flags & पङ्क्तौ_निक्षिप_WAKEUP)
		update_stats_पङ्क्तौ_निक्षिप_sleeper_fair(cfs_rq, se);
}

स्थिर अन्तःस्थ शून्य
update_stats_पङ्क्तेः_निष्कासय_fair(संरचना cfs_rq *cfs_rq, संरचना sched_entity *se, पूर्णाङ्क flags)
{

	यदि (!schedstat_enabled())
		प्रत्यागम;

	/*
	 * Mark the end of the wait period if dequeueing a
	 * waiting task:
	 */
	यदि (se != cfs_rq->h_curr)
		update_stats_wait_अन्तः_fair(cfs_rq, se);

	यदि ((flags & पङ्क्तेः_निष्कासय_SLEEP) && entity_is_task(se)) {
		संरचना task_struct *tsk = task_सम्बन्धि(se);
		अचिह्नित पूर्णाङ्क state;

		/* XXX racy against TTWU */
		state = READ_ONCE(tsk->__state);
		यदि (state & TASK_INTERRUPTIBLE)
			__schedstat_set(tsk->stats.sleep_start,
				      rq_clock(rq_सम्बन्धि(cfs_rq)));
		यदि (state & TASK_UNINTERRUPTIBLE)
			__schedstat_set(tsk->stats.खण्डम्_start,
				      rq_clock(rq_सम्बन्धि(cfs_rq)));
	}
}

/*
 * We are picking a new current task - update its stats:
 */
स्थिर अन्तःस्थ शून्य
update_stats_curr_start(संरचना cfs_rq *cfs_rq, संरचना sched_entity *se)
{
	/*
	 * We are starting a new run period:
	 */
	se->exec_start = rq_clock_task(rq_सम्बन्धि(cfs_rq));
}

/* Check sched_smt_active before calling this to avoid overheads in fastpaths */
स्थिर अन्तःस्थ bool is_core_idle(पूर्णाङ्क cpu)
{
	पूर्णाङ्क sibling;

	for_each_cpu(sibling, cpu_smt_आवरणम्(cpu)) {
		यदि (cpu == sibling)
			अनुवर्त;

		यदि (!idle_cpu(sibling))
			प्रत्यागम false;
	}

	प्रत्यागम true;
}

#यदि_परिभाषितम् CONFIG_NUMA
#परिभाषय NUMA_IMBALANCE_MIN 2

स्थिर अन्तःस्थ दीर्घ
adjust_numa_imbalance(पूर्णाङ्क imbalance, पूर्णाङ्क dst_running, पूर्णाङ्क imb_numa_nr)
{
	/*
	 * Allow a NUMA imbalance if busy CPUs is less than the maximum
	 * threshold. Above this threshold, individual tasks may be contending
	 * for both memory bandwidth and any shared HT resources.  This is an
	 * approximation as the number of running tasks may not be related to
	 * the number of busy CPUs due to sched_setaffinity.
	 */
	यदि (dst_running > imb_numa_nr)
		प्रत्यागम imbalance;

	/*
	 * Allow a small imbalance based on a simple pair of communicating
	 * tasks that remain local when the destination is lightly loaded.
	 */
	यदि (imbalance <= NUMA_IMBALANCE_MIN)
		प्रत्यागम 0;

	प्रत्यागम imbalance;
}
#यदि_अन्तः /* CONFIG_NUMA */

#यदि_परिभाषितम् CONFIG_NUMA_BALANCING
/*
 * Approximate time to scan a full NUMA task in ms. The task scan period is
 * calculated based on the tasks virtual memory size and
 * numa_balancing_scan_size.
 */
अचिह्नित पूर्णाङ्क sysctl_numa_balancing_अवलोकनम्_period_min = 1000;
अचिह्नित पूर्णाङ्क sysctl_numa_balancing_अवलोकनम्_period_max = 60000;

/* Portion of address space to scan in MB */
अचिह्नित पूर्णाङ्क sysctl_numa_balancing_अवलोकनम्_आकार = 256;

/* Scan @scan_size MB every @scan_period after an initial @scan_delay in ms */
अचिह्नित पूर्णाङ्क sysctl_numa_balancing_अवलोकनम्_delay = 1000;

/* The page with hint page fault latency < threshold in ms is considered hot */
अचिह्नित पूर्णाङ्क sysctl_numa_balancing_hot_देहली = MSEC_प्रत्येकम्_SEC;

संरचना numa_गणः {
	refcount_t refcount;

	spinlock_t तालकम्; /* nr_tasks, tasks */
	पूर्णाङ्क nr_tasks;
	pid_t gid;
	पूर्णाङ्क सक्रिय_nodes;

	संरचना rcu_शीर्ष rcu;
	अचिह्नित दीर्घ योग_दोषाः;
	अचिह्नित दीर्घ max_दोषाः_cpu;
	/*
	 * faults[] array is split into two regions: faults_mem and faults_cpu.
	 *
	 * Faults_cpu is used to decide whether memory should move
	 * towards the CPU. As a consequence, these stats are weighted
	 * more by CPU use than by memory faults.
	 */
	अचिह्नित दीर्घ दोषाः[];
};

/*
 * For functions that can be called in multiple contexts that permit reading
 * ->numa_group (see struct task_struct for locking rules).
 */
स्थिर संरचना numa_गणः *deref_task_numa_गणः(संरचना task_struct *p)
{
	प्रत्यागम rcu_dereference_check(p->numa_गणः, p == current ||
		(lockdep_is_held(__rq_lockp(task_rq(p))) && !READ_ONCE(p->on_cpu)));
}

स्थिर संरचना numa_गणः *deref_curr_numa_गणः(संरचना task_struct *p)
{
	प्रत्यागम rcu_dereference_protected(p->numa_गणः, p == current);
}

स्थिर अन्तःस्थ अचिह्नित दीर्घ गणः_दोषाः_priv(संरचना numa_गणः *ng);
स्थिर अन्तःस्थ अचिह्नित दीर्घ गणः_दोषाः_shared(संरचना numa_गणः *ng);

स्थिर अचिह्नित पूर्णाङ्क task_nr_अवलोकनम्_windows(संरचना task_struct *p)
{
	अचिह्नित दीर्घ rss = 0;
	अचिह्नित दीर्घ nr_अवलोकनम्_पृष्ठानि;

	/*
	 * Calculations based on RSS as non-present and empty pages are skipped
	 * by the PTE scanner and NUMA hinting faults should be trapped based
	 * on resident pages
	 */
	nr_अवलोकनम्_पृष्ठानि = MB_गन्तव्यम्_पृष्ठानि(sysctl_numa_balancing_अवलोकनम्_आकार);
	rss = get_mm_rss(p->mm);
	यदि (!rss)
		rss = nr_अवलोकनम्_पृष्ठानि;

	rss = आवर्तनम्_ऊर्ध्वम्(rss, nr_अवलोकनम्_पृष्ठानि);
	प्रत्यागम rss / nr_अवलोकनम्_पृष्ठानि;
}

/* For sanity's sake, never scan more PTEs than MAX_SCAN_WINDOW MB/sec. */
#परिभाषय MAX_अवलोकनम्_गवाक्षः 2560

स्थिर अचिह्नित पूर्णाङ्क task_अवलोकनम्_min(संरचना task_struct *p)
{
	अचिह्नित पूर्णाङ्क अवलोकनम्_आकार = READ_ONCE(sysctl_numa_balancing_अवलोकनम्_आकार);
	अचिह्नित पूर्णाङ्क अवलोकनम्, floor;
	अचिह्नित पूर्णाङ्क windows = 1;

	यदि (अवलोकनम्_आकार < MAX_अवलोकनम्_गवाक्षः)
		windows = MAX_अवलोकनम्_गवाक्षः / अवलोकनम्_आकार;
	floor = 1000 / windows;

	अवलोकनम् = sysctl_numa_balancing_अवलोकनम्_period_min / task_nr_अवलोकनम्_windows(p);
	प्रत्यागम max_t(अचिह्नित पूर्णाङ्क, floor, अवलोकनम्);
}

स्थिर अचिह्नित पूर्णाङ्क task_अवलोकनम्_start(संरचना task_struct *p)
{
	अचिह्नित दीर्घ smin = task_अवलोकनम्_min(p);
	अचिह्नित दीर्घ period = smin;
	संरचना numa_गणः *ng;

	/* Scale the maximum scan period with the amount of shared memory. */
	rcu_read_तालकम्();
	ng = rcu_dereference_all(p->numa_गणः);
	यदि (ng) {
		अचिह्नित दीर्घ shared = गणः_दोषाः_shared(ng);
		अचिह्नित दीर्घ गुप्त = गणः_दोषाः_priv(ng);

		period *= refcount_read(&ng->refcount);
		period *= shared + 1;
		period /= गुप्त + shared + 1;
	}
	rcu_read_unlock();

	प्रत्यागम max(smin, period);
}

स्थिर अचिह्नित पूर्णाङ्क task_अवलोकनम्_max(संरचना task_struct *p)
{
	अचिह्नित दीर्घ smin = task_अवलोकनम्_min(p);
	अचिह्नित दीर्घ smax;
	संरचना numa_गणः *ng;

	/* Watch for min being lower than max due to floor calculations */
	smax = sysctl_numa_balancing_अवलोकनम्_period_max / task_nr_अवलोकनम्_windows(p);

	/* Scale the maximum scan period with the amount of shared memory. */
	ng = deref_curr_numa_गणः(p);
	यदि (ng) {
		अचिह्नित दीर्घ shared = गणः_दोषाः_shared(ng);
		अचिह्नित दीर्घ गुप्त = गणः_दोषाः_priv(ng);
		अचिह्नित दीर्घ period = smax;

		period *= refcount_read(&ng->refcount);
		period *= shared + 1;
		period /= गुप्त + shared + 1;

		smax = max(smax, period);
	}

	प्रत्यागम max(smin, smax);
}

स्थिर शून्य account_numa_पङ्क्तौ_निक्षिप(संरचना rq *rq, संरचना task_struct *p)
{
	rq->nr_numa_running += (p->numa_preferred_nid != NUMA_NO_ग्रन्थिः);
	rq->nr_preferred_running += (p->numa_preferred_nid == task_ग्रन्थिः(p));
}

स्थिर शून्य account_numa_पङ्क्तेः_निष्कासय(संरचना rq *rq, संरचना task_struct *p)
{
	rq->nr_numa_running -= (p->numa_preferred_nid != NUMA_NO_ग्रन्थिः);
	rq->nr_preferred_running -= (p->numa_preferred_nid == task_ग्रन्थिः(p));
}

/* Shared or private faults. */
#परिभाषय NR_NUMA_HINT_FAULT_TYPES 2

/* Memory and CPU locality */
#परिभाषय NR_NUMA_HINT_FAULT_STATS (NR_NUMA_HINT_FAULT_TYPES * 2)

/* Averaged statistics, and temporary buffers. */
#परिभाषय NR_NUMA_HINT_FAULT_BUCKETS (NR_NUMA_HINT_FAULT_STATS * 2)

pid_t task_numa_गणः_id(संरचना task_struct *p)
{
	संरचना numa_गणः *ng;
	pid_t gid = 0;

	rcu_read_तालकम्();
	ng = rcu_dereference_all(p->numa_गणः);
	यदि (ng)
		gid = ng->gid;
	rcu_read_unlock();

	प्रत्यागम gid;
}

/*
 * The averaged statistics, shared & private, memory & CPU,
 * occupy the first half of the array. The second half of the
 * array is for current counters, which are averaged into the
 * first set by task_numa_placement.
 */
स्थिर अन्तःस्थ पूर्णाङ्क task_दोषाः_idx(गणना numa_दोषाः_stats s, पूर्णाङ्क nid, पूर्णाङ्क priv)
{
	प्रत्यागम NR_NUMA_HINT_FAULT_TYPES * (s * nr_ग्रन्थिः_ids + nid) + priv;
}

स्थिर अन्तःस्थ अचिह्नित दीर्घ task_दोषाः(संरचना task_struct *p, पूर्णाङ्क nid)
{
	यदि (!p->numa_दोषाः)
		प्रत्यागम 0;

	प्रत्यागम p->numa_दोषाः[task_दोषाः_idx(NUMA_MEM, nid, 0)] +
		p->numa_दोषाः[task_दोषाः_idx(NUMA_MEM, nid, 1)];
}

स्थिर अन्तःस्थ अचिह्नित दीर्घ गणः_दोषाः(संरचना task_struct *p, पूर्णाङ्क nid)
{
	संरचना numa_गणः *ng = deref_task_numa_गणः(p);

	यदि (!ng)
		प्रत्यागम 0;

	प्रत्यागम ng->दोषाः[task_दोषाः_idx(NUMA_MEM, nid, 0)] +
		ng->दोषाः[task_दोषाः_idx(NUMA_MEM, nid, 1)];
}

स्थिर अन्तःस्थ अचिह्नित दीर्घ गणः_दोषाः_cpu(संरचना numa_गणः *गणः, पूर्णाङ्क nid)
{
	प्रत्यागम गणः->दोषाः[task_दोषाः_idx(NUMA_CPU, nid, 0)] +
		गणः->दोषाः[task_दोषाः_idx(NUMA_CPU, nid, 1)];
}

स्थिर अन्तःस्थ अचिह्नित दीर्घ गणः_दोषाः_priv(संरचना numa_गणः *ng)
{
	अचिह्नित दीर्घ दोषाः = 0;
	पूर्णाङ्क ग्रन्थिः;

	for_each_online_ग्रन्थिः(ग्रन्थिः) {
		दोषाः += ng->दोषाः[task_दोषाः_idx(NUMA_MEM, ग्रन्थिः, 1)];
	}

	प्रत्यागम दोषाः;
}

स्थिर अन्तःस्थ अचिह्नित दीर्घ गणः_दोषाः_shared(संरचना numa_गणः *ng)
{
	अचिह्नित दीर्घ दोषाः = 0;
	पूर्णाङ्क ग्रन्थिः;

	for_each_online_ग्रन्थिः(ग्रन्थिः) {
		दोषाः += ng->दोषाः[task_दोषाः_idx(NUMA_MEM, ग्रन्थिः, 0)];
	}

	प्रत्यागम दोषाः;
}

/*
 * A node triggering more than 1/3 as many NUMA faults as the maximum is
 * considered part of a numa group's pseudo-interleaving set. Migrations
 * between these nodes are slowed down, to allow things to settle down.
 */
#परिभाषय सक्रिय_ग्रन्थिः_FRACTION 3

स्थिर bool numa_is_सक्रिय_ग्रन्थिः(पूर्णाङ्क nid, संरचना numa_गणः *ng)
{
	प्रत्यागम गणः_दोषाः_cpu(ng, nid) * सक्रिय_ग्रन्थिः_FRACTION > ng->max_दोषाः_cpu;
}

/* Handle placement on systems where not all nodes are directly connected. */
स्थिर अचिह्नित दीर्घ score_nearby_nodes(संरचना task_struct *p, पूर्णाङ्क nid,
					पूर्णाङ्क lim_dist, bool task)
{
	अचिह्नित दीर्घ score = 0;
	पूर्णाङ्क ग्रन्थिः, max_dist;

	/*
	 * All nodes are directly connected, and the same distance
	 * from each other. No need for fancy placement algorithms.
	 */
	यदि (sched_numa_topology_type == NUMA_प्रत्यक्षम्)
		प्रत्यागम 0;

	/* sched_max_numa_distance may be changed in parallel. */
	max_dist = READ_ONCE(sched_max_numa_distance);
	/*
	 * This code is called for each node, introducing N^2 complexity,
	 * which should be OK given the number of nodes rarely exceeds 8.
	 */
	for_each_online_ग्रन्थिः(ग्रन्थिः) {
		अचिह्नित दीर्घ दोषाः;
		पूर्णाङ्क dist = ग्रन्थिः_distance(nid, ग्रन्थिः);

		/*
		 * The furthest away nodes in the system are not interesting
		 * for placement; nid was already counted.
		 */
		यदि (dist >= max_dist || ग्रन्थिः == nid)
			अनुवर्त;

		/*
		 * On systems with a backplane NUMA topology, compare groups
		 * of nodes, and move tasks towards the group with the most
		 * memory accesses. When comparing two nodes at distance
		 * "hoplimit", only nodes closer by than "hoplimit" are part
		 * of each group. Skip other nodes.
		 */
		यदि (sched_numa_topology_type == NUMA_BACKPLANE && dist >= lim_dist)
			अनुवर्त;

		/* Add up the faults from nearby nodes. */
		यदि (task)
			दोषाः = task_दोषाः(p, ग्रन्थिः);
		अन्यथा
			दोषाः = गणः_दोषाः(p, ग्रन्थिः);

		/*
		 * On systems with a glueless mesh NUMA topology, there are
		 * no fixed "groups of nodes". Instead, nodes that are not
		 * directly connected bounce traffic through intermediate
		 * nodes; a numa_group can occupy any set of nodes.
		 * The further away a node is, the less the faults count.
		 * This seems to result in good task placement.
		 */
		यदि (sched_numa_topology_type == NUMA_GLUELESS_MESH) {
			दोषाः *= (max_dist - dist);
			दोषाः /= (max_dist - LOCAL_DISTANCE);
		}

		score += दोषाः;
	}

	प्रत्यागम score;
}

/*
 * These return the fraction of accesses done by a particular task, or
 * task group, on a particular numa node.  The group weight is given a
 * larger multiplier, in order to group tasks together that are almost
 * evenly spread out between numa nodes.
 */
स्थिर अन्तःस्थ अचिह्नित दीर्घ task_weight(संरचना task_struct *p, पूर्णाङ्क nid,
					पूर्णाङ्क dist)
{
	अचिह्नित दीर्घ दोषाः, योग_दोषाः;

	यदि (!p->numa_दोषाः)
		प्रत्यागम 0;

	योग_दोषाः = p->योग_numa_दोषाः;

	यदि (!योग_दोषाः)
		प्रत्यागम 0;

	दोषाः = task_दोषाः(p, nid);
	दोषाः += score_nearby_nodes(p, nid, dist, true);

	प्रत्यागम 1000 * दोषाः / योग_दोषाः;
}

स्थिर अन्तःस्थ अचिह्नित दीर्घ गणः_weight(संरचना task_struct *p, पूर्णाङ्क nid,
					 पूर्णाङ्क dist)
{
	संरचना numa_गणः *ng = deref_task_numa_गणः(p);
	अचिह्नित दीर्घ दोषाः, योग_दोषाः;

	यदि (!ng)
		प्रत्यागम 0;

	योग_दोषाः = ng->योग_दोषाः;

	यदि (!योग_दोषाः)
		प्रत्यागम 0;

	दोषाः = गणः_दोषाः(p, nid);
	दोषाः += score_nearby_nodes(p, nid, dist, false);

	प्रत्यागम 1000 * दोषाः / योग_दोषाः;
}

/*
 * If memory tiering mode is enabled, cpupid of slow memory page is
 * used to record scan time instead of CPU and PID.  When tiering mode
 * is disabled at run time, the scan time (in cpupid) will be
 * interpreted as CPU and PID.  So CPU needs to be checked to avoid to
 * access out of array bound.
 */
स्थिर अन्तःस्थ bool cpupid_valid(पूर्णाङ्क cpupid)
{
	प्रत्यागम cpupid_गन्तव्यम्_cpu(cpupid) < nr_cpu_ids;
}

/*
 * For memory tiering mode, if there are enough free pages (more than
 * enough watermark defined here) in fast memory node, to take full
 * advantage of fast memory capacity, all recently accessed slow
 * memory pages will be migrated to fast memory node without
 * considering hot threshold.
 */
स्थिर bool pgdat_free_space_enough(संरचना pglist_दत्त *pgdat)
{
	पूर्णाङ्क z;
	अचिह्नित दीर्घ enough_wmark;

	enough_wmark = max(1UL * 1024 * 1024 * 1024 >> PAGE_सरणम्,
			   pgdat->ग्रन्थिः_present_पृष्ठानि >> 4);
	प्रति (z = pgdat->nr_zones - 1; z >= 0; z--) {
		संरचना zone *zone = pgdat->ग्रन्थिः_zones + z;

		यदि (!populated_zone(zone))
			अनुवर्त;

		यदि (zone_watermark_ok(zone, 0,
				      promo_wmark_पृष्ठानि(zone) + enough_wmark,
				      ZONE_MOVABLE, 0))
			प्रत्यागम true;
	}
	प्रत्यागम false;
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
स्थिर पूर्णाङ्क numa_hint_fault_latency(संरचना folio *folio)
{
	पूर्णाङ्क चरमः_time, time;

	time = jiffies_गन्तव्यम्_msecs(jiffies);
	चरमः_time = folio_xchg_access_time(folio, time);

	प्रत्यागम (time - चरमः_time) & PAGE_ACCESS_TIME_आवरणम्;
}

/*
 * For memory tiering mode, too high promotion/demotion throughput may
 * hurt application latency.  So we provide a mechanism to rate limit
 * the number of pages that are tried to be promoted.
 */
स्थिर bool numa_promotion_दरः_सीमा(संरचना pglist_दत्त *pgdat,
				      अचिह्नित दीर्घ दरः_सीमा, पूर्णाङ्क nr)
{
	अचिह्नित दीर्घ nr_cand;
	अचिह्नित पूर्णाङ्क now, start;

	now = jiffies_गन्तव्यम्_msecs(jiffies);
	mod_ग्रन्थिः_page_state(pgdat, PGPROMOTE_CANDIDATE, nr);
	nr_cand = ग्रन्थिः_page_state(pgdat, PGPROMOTE_CANDIDATE);
	start = pgdat->nbp_rl_start;
	यदि (now - start > MSEC_प्रत्येकम्_SEC &&
	    cmpxchg(&pgdat->nbp_rl_start, start, now) == start)
		pgdat->nbp_rl_nr_cand = nr_cand;
	यदि (nr_cand - pgdat->nbp_rl_nr_cand >= दरः_सीमा)
		प्रत्यागम true;
	प्रत्यागम false;
}

#परिभाषय NUMA_MIGRATION_ADJUST_चरणाः	16

स्थिर शून्य numa_promotion_adjust_देहली(संरचना pglist_दत्त *pgdat,
					    अचिह्नित दीर्घ दरः_सीमा,
					    अचिह्नित पूर्णाङ्क ref_th)
{
	अचिह्नित पूर्णाङ्क now, start, th_period, unit_th, th;
	अचिह्नित दीर्घ nr_cand, ref_cand, diff_cand;

	now = jiffies_गन्तव्यम्_msecs(jiffies);
	th_period = sysctl_numa_balancing_अवलोकनम्_period_max;
	start = pgdat->nbp_th_start;
	यदि (now - start > th_period &&
	    cmpxchg(&pgdat->nbp_th_start, start, now) == start) {
		ref_cand = दरः_सीमा *
			sysctl_numa_balancing_अवलोकनम्_period_max / MSEC_प्रत्येकम्_SEC;
		nr_cand = ग्रन्थिः_page_state(pgdat, PGPROMOTE_CANDIDATE);
		diff_cand = nr_cand - pgdat->nbp_th_nr_cand;
		unit_th = ref_th * 2 / NUMA_MIGRATION_ADJUST_चरणाः;
		th = pgdat->nbp_देहली ? : ref_th;
		यदि (diff_cand > ref_cand * 11 / 10)
			th = max(th - unit_th, unit_th);
		अन्यथा यदि (diff_cand < ref_cand * 9 / 10)
			th = min(th + unit_th, ref_th * 2);
		pgdat->nbp_th_nr_cand = nr_cand;
		pgdat->nbp_देहली = th;
	}
}

bool should_numa_migrate_स्मृतिः(संरचना task_struct *p, संरचना folio *folio,
				पूर्णाङ्क src_nid, पूर्णाङ्क dst_cpu)
{
	संरचना numa_गणः *ng = deref_curr_numa_गणः(p);
	पूर्णाङ्क dst_nid = cpu_गन्तव्यम्_ग्रन्थिः(dst_cpu);
	पूर्णाङ्क चरमः_cpupid, this_cpupid;

	/*
	 * Cannot migrate to memoryless nodes.
	 */
	यदि (!ग्रन्थिः_state(dst_nid, N_स्मृतिः))
		प्रत्यागम false;

	/*
	 * The pages in slow memory node should be migrated according
	 * to hot/cold instead of private/shared.
	 */
	यदि (folio_use_access_time(folio)) {
		संरचना pglist_दत्त *pgdat;
		अचिह्नित दीर्घ दरः_सीमा;
		अचिह्नित पूर्णाङ्क latency, th, def_th;
		दीर्घ nr = folio_nr_पृष्ठानि(folio);

		pgdat = ग्रन्थिः_दत्त(dst_nid);
		यदि (pgdat_free_space_enough(pgdat)) {
			/* workload changed, reset hot threshold */
			pgdat->nbp_देहली = 0;
			mod_ग्रन्थिः_page_state(pgdat, PGPROMOTE_CANDIDATE_NRL, nr);
			प्रत्यागम true;
		}

		def_th = sysctl_numa_balancing_hot_देहली;
		दरः_सीमा = MB_गन्तव्यम्_पृष्ठानि(sysctl_numa_balancing_promote_दरः_सीमा);
		numa_promotion_adjust_देहली(pgdat, दरः_सीमा, def_th);

		th = pgdat->nbp_देहली ? : def_th;
		latency = numa_hint_fault_latency(folio);
		यदि (latency >= th)
			प्रत्यागम false;

		प्रत्यागम !numa_promotion_दरः_सीमा(pgdat, दरः_सीमा, nr);
	}

	this_cpupid = cpu_pid_गन्तव्यम्_cpupid(dst_cpu, current->pid);
	चरमः_cpupid = folio_xchg_चरमः_cpupid(folio, this_cpupid);

	यदि (!(sysctl_numa_balancing_बहुलकम् & NUMA_BALANCING_स्मृतिः_TIERING) &&
	    !ग्रन्थिः_is_toptier(src_nid) && !cpupid_valid(चरमः_cpupid))
		प्रत्यागम false;

	/*
	 * Allow first faults or private faults to migrate immediately early in
	 * the lifetime of a task. The magic number 4 is based on waiting for
	 * two full passes of the "multi-stage node selection" test that is
	 * executed below.
	 */
	यदि ((p->numa_preferred_nid == NUMA_NO_ग्रन्थिः || p->numa_अवलोकनम्_seq <= 4) &&
	    (cpupid_pid_unset(चरमः_cpupid) || cpupid_मेलनम्_pid(p, चरमः_cpupid)))
		प्रत्यागम true;

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
	यदि (!cpupid_pid_unset(चरमः_cpupid) &&
				cpupid_गन्तव्यम्_nid(चरमः_cpupid) != dst_nid)
		प्रत्यागम false;

	/* Always allow migrate on private faults */
	यदि (cpupid_मेलनम्_pid(p, चरमः_cpupid))
		प्रत्यागम true;

	/* A shared fault, but p->numa_group has not been set up yet. */
	यदि (!ng)
		प्रत्यागम true;

	/*
	 * Destination node is much more heavily used than the source
	 * node? Allow migration.
	 */
	यदि (गणः_दोषाः_cpu(ng, dst_nid) > गणः_दोषाः_cpu(ng, src_nid) *
					सक्रिय_ग्रन्थिः_FRACTION)
		प्रत्यागम true;

	/*
	 * Distribute memory according to CPU & memory use on each node,
	 * with 3/4 hysteresis to avoid unnecessary memory migrations:
	 *
	 * faults_cpu(dst)   3   faults_cpu(src)
	 * --------------- * - > ---------------
	 * faults_mem(dst)   4   faults_mem(src)
	 */
	प्रत्यागम गणः_दोषाः_cpu(ng, dst_nid) * गणः_दोषाः(p, src_nid) * 3 >
	       गणः_दोषाः_cpu(ng, src_nid) * गणः_दोषाः(p, dst_nid) * 4;
}

/*
 * 'numa_type' describes the node at the moment of load balancing.
 */
गणना numa_type {
	/* The node has spare capacity that can be used to run more tasks.  */
	ग्रन्थिः_has_अतिरिक्तः = 0,
	/*
	 * The node is fully used and the tasks don't compete for more CPU
	 * cycles. Nevertheless, some tasks might wait before running.
	 */
	ग्रन्थिः_fully_busy,
	/*
	 * The node is overloaded and can't provide expected CPU cycles to all
	 * tasks.
	 */
	ग्रन्थिः_overloaded
};

/* Cached statistics for all CPUs within a node */
संरचना numa_stats {
	अचिह्नित दीर्घ आरोपय;
	अचिह्नित दीर्घ runnable;
	अचिह्नित दीर्घ util;
	/* Total compute capacity of CPUs on a node */
	अचिह्नित दीर्घ compute_क्षमता;
	अचिह्नित पूर्णाङ्क nr_running;
	अचिह्नित पूर्णाङ्क weight;
	गणना numa_type ग्रन्थिः_type;
	पूर्णाङ्क idle_cpu;
};

संरचना task_numa_env {
	संरचना task_struct *p;

	पूर्णाङ्क src_cpu, src_nid;
	पूर्णाङ्क dst_cpu, dst_nid;
	पूर्णाङ्क imb_numa_nr;

	संरचना numa_stats src_stats, dst_stats;

	पूर्णाङ्क imbalance_pct;
	पूर्णाङ्क dist;

	संरचना task_struct *best_task;
	दीर्घ best_imp;
	पूर्णाङ्क best_cpu;
};

स्थिर अचिह्नित दीर्घ cpu_आरोपय(संरचना rq *rq);
स्थिर अचिह्नित दीर्घ cpu_runnable(संरचना rq *rq);

स्थिर अन्तःस्थ गणना
numa_type numa_classify(अचिह्नित पूर्णाङ्क imbalance_pct,
			 संरचना numa_stats *ns)
{
	यदि ((ns->nr_running > ns->weight) &&
	    (((ns->compute_क्षमता * 100) < (ns->util * imbalance_pct)) ||
	     ((ns->compute_क्षमता * imbalance_pct) < (ns->runnable * 100))))
		प्रत्यागम ग्रन्थिः_overloaded;

	यदि ((ns->nr_running < ns->weight) ||
	    (((ns->compute_क्षमता * 100) > (ns->util * imbalance_pct)) &&
	     ((ns->compute_क्षमता * imbalance_pct) > (ns->runnable * 100))))
		प्रत्यागम ग्रन्थिः_has_अतिरिक्तः;

	प्रत्यागम ग्रन्थिः_fully_busy;
}

/* Forward declarations of select_idle_sibling helpers */
स्थिर अन्तःस्थ bool test_idle_cores(पूर्णाङ्क cpu);
स्थिर अन्तःस्थ पूर्णाङ्क numa_idle_core(पूर्णाङ्क idle_core, पूर्णाङ्क cpu)
{
	यदि (!sched_smt_सक्रिय() ||
	    idle_core >= 0 || !test_idle_cores(cpu))
		प्रत्यागम idle_core;

	/*
	 * Prefer cores instead of packing HT siblings
	 * and triggering future load balancing.
	 */
	यदि (is_core_idle(cpu))
		idle_core = cpu;

	प्रत्यागम idle_core;
}

/*
 * Gather all necessary information to make NUMA balancing placement
 * decisions that are compatible with standard load balancer. This
 * borrows code and logic from update_sg_lb_stats but sharing a
 * common implementation is impractical.
 */
स्थिर शून्य update_numa_stats(संरचना task_numa_env *env,
			      संरचना numa_stats *ns, पूर्णाङ्क nid,
			      bool find_idle)
{
	पूर्णाङ्क cpu, idle_core = -1;

	स्मृतिं_पूरय(ns, 0, परिमाण(*ns));
	ns->idle_cpu = -1;

	rcu_read_तालकम्();
	for_each_cpu(cpu, cpumask_सम्बन्धि_ग्रन्थिः(nid)) {
		संरचना rq *rq = cpu_rq(cpu);

		ns->आरोपय += cpu_आरोपय(rq);
		ns->runnable += cpu_runnable(rq);
		ns->util += cpu_util_cfs(cpu);
		ns->nr_running += rq->cfs.h_nr_runnable;
		ns->compute_क्षमता += क्षमता_सम्बन्धि(cpu);

		यदि (find_idle && idle_core < 0 && !rq->nr_running && idle_cpu(cpu)) {
			यदि (READ_ONCE(rq->numa_migrate_on) ||
			    !cpumask_test_cpu(cpu, env->p->cpus_ptr))
				अनुवर्त;

			यदि (ns->idle_cpu == -1)
				ns->idle_cpu = cpu;

			idle_core = numa_idle_core(idle_core, cpu);
		}
	}
	rcu_read_unlock();

	ns->weight = cpumask_weight(cpumask_सम्बन्धि_ग्रन्थिः(nid));

	ns->ग्रन्थिः_type = numa_classify(env->imbalance_pct, ns);

	यदि (idle_core >= 0)
		ns->idle_cpu = idle_core;
}

स्थिर शून्य task_numa_नियोजय(संरचना task_numa_env *env,
			     संरचना task_struct *p, दीर्घ imp)
{
	संरचना rq *rq = cpu_rq(env->dst_cpu);

	/* Check if run-queue part of active NUMA balance. */
	यदि (env->best_cpu != env->dst_cpu && xchg(&rq->numa_migrate_on, 1)) {
		पूर्णाङ्क cpu;
		पूर्णाङ्क start = env->dst_cpu;

		/* Find alternative idle CPU. */
		for_each_cpu_wrap(cpu, cpumask_सम्बन्धि_ग्रन्थिः(env->dst_nid), start + 1) {
			यदि (cpu == env->best_cpu || !idle_cpu(cpu) ||
			    !cpumask_test_cpu(cpu, env->p->cpus_ptr)) {
				अनुवर्त;
			}

			env->dst_cpu = cpu;
			rq = cpu_rq(env->dst_cpu);
			यदि (!xchg(&rq->numa_migrate_on, 1))
				गच्छ नियोजय;
		}

		/* Failed to find an alternative idle CPU */
		प्रत्यागम;
	}

नियोजय:
	/*
	 * Clear previous best_cpu/rq numa-migrate flag, since task now
	 * found a better CPU to move/swap.
	 */
	यदि (env->best_cpu != -1 && env->best_cpu != env->dst_cpu) {
		rq = cpu_rq(env->best_cpu);
		WRITE_ONCE(rq->numa_migrate_on, 0);
	}

	यदि (env->best_task)
		put_task_struct(env->best_task);
	यदि (p)
		get_task_struct(p);

	env->best_task = p;
	env->best_imp = imp;
	env->best_cpu = env->dst_cpu;
}

स्थिर bool आरोपय_too_imbalanced(दीर्घ src_आरोपय, दीर्घ dst_आरोपय,
				संरचना task_numa_env *env)
{
	दीर्घ imb, old_imb;
	दीर्घ orig_src_आरोपय, orig_dst_आरोपय;
	दीर्घ src_क्षमता, dst_क्षमता;

	/*
	 * The load is corrected for the CPU capacity available on each node.
	 *
	 * src_load        dst_load
	 * ------------ vs ---------
	 * src_capacity    dst_capacity
	 */
	src_क्षमता = env->src_stats.compute_क्षमता;
	dst_क्षमता = env->dst_stats.compute_क्षमता;

	imb = निरपेक्षम्(dst_आरोपय * src_क्षमता - src_आरोपय * dst_क्षमता);

	orig_src_आरोपय = env->src_stats.आरोपय;
	orig_dst_आरोपय = env->dst_stats.आरोपय;

	old_imb = निरपेक्षम्(orig_dst_आरोपय * src_क्षमता - orig_src_आरोपय * dst_क्षमता);

	/* Would this change make things worse? */
	प्रत्यागम (imb > old_imb);
}

/*
 * Maximum NUMA importance can be 1998 (2*999);
 * SMALLIMP @ 30 would be close to 1998/64.
 * Used to deter task migration.
 */
#परिभाषय SMALLIMP	30

/*
 * This checks if the overall compute and NUMA accesses of the system would
 * be improved if the source tasks was migrated to the target dst_cpu taking
 * into account that it might be best if task running on the dst_cpu should
 * be exchanged with the source task
 */
स्थिर bool task_numa_compare(संरचना task_numa_env *env,
			      दीर्घ taskimp, दीर्घ groupimp, bool maymove)
{
	संरचना numa_गणः *cur_ng, *p_ng = deref_curr_numa_गणः(env->p);
	संरचना rq *dst_rq = cpu_rq(env->dst_cpu);
	दीर्घ imp = p_ng ? groupimp : taskimp;
	संरचना task_struct *cur;
	दीर्घ src_आरोपय, dst_आरोपय;
	पूर्णाङ्क dist = env->dist;
	दीर्घ moveimp = imp;
	दीर्घ आरोपय;
	bool stopsearch = false;

	यदि (READ_ONCE(dst_rq->numa_migrate_on))
		प्रत्यागम false;

	rcu_read_तालकम्();
	cur = rcu_dereference_all(dst_rq->curr);
	यदि (cur && ((cur->flags & (PF_EXITING | PF_KTHREAD)) ||
		    !cur->mm))
		cur = रिक्त;

	/*
	 * Because we have preemption enabled we can get migrated around and
	 * end try selecting ourselves (current == env->p) as a swap candidate.
	 */
	यदि (cur == env->p) {
		stopsearch = true;
		गच्छ unlock;
	}

	यदि (!cur) {
		यदि (maymove && moveimp >= env->best_imp)
			गच्छ नियोजय;
		अन्यथा
			गच्छ unlock;
	}

	/* Skip this swap candidate if cannot move to the source cpu. */
	यदि (!cpumask_test_cpu(env->src_cpu, cur->cpus_ptr))
		गच्छ unlock;

	/*
	 * Skip this swap candidate if it is not moving to its preferred
	 * node and the best task is.
	 */
	यदि (env->best_task &&
	    env->best_task->numa_preferred_nid == env->src_nid &&
	    cur->numa_preferred_nid != env->src_nid) {
		गच्छ unlock;
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
	cur_ng = rcu_dereference_all(cur->numa_गणः);
	यदि (cur_ng == p_ng) {
		/*
		 * Do not swap within a group or between tasks that have
		 * no group if there is spare capacity. Swapping does
		 * not address the load imbalance and helps one task at
		 * the cost of punishing another.
		 */
		यदि (env->dst_stats.ग्रन्थिः_type == ग्रन्थिः_has_अतिरिक्तः)
			गच्छ unlock;

		imp = taskimp + task_weight(cur, env->src_nid, dist) -
		      task_weight(cur, env->dst_nid, dist);
		/*
		 * Add some hysteresis to prevent swapping the
		 * tasks within a group over tiny differences.
		 */
		यदि (cur_ng)
			imp -= imp / 16;
	} अन्यथा {
		/*
		 * Compare the group weights. If a task is all by itself
		 * (not part of a group), use the task weight instead.
		 */
		यदि (cur_ng && p_ng)
			imp += गणः_weight(cur, env->src_nid, dist) -
			       गणः_weight(cur, env->dst_nid, dist);
		अन्यथा
			imp += task_weight(cur, env->src_nid, dist) -
			       task_weight(cur, env->dst_nid, dist);
	}

	/* Discourage picking a task already on its preferred node */
	यदि (cur->numa_preferred_nid == env->dst_nid)
		imp -= imp / 16;

	/*
	 * Encourage picking a task that moves to its preferred node.
	 * This potentially makes imp larger than it's maximum of
	 * 1998 (see SMALLIMP and task_weight for why) but in this
	 * case, it does not matter.
	 */
	यदि (cur->numa_preferred_nid == env->src_nid)
		imp += imp / 8;

	यदि (maymove && moveimp > imp && moveimp > env->best_imp) {
		imp = moveimp;
		cur = रिक्त;
		गच्छ नियोजय;
	}

	/*
	 * Prefer swapping with a task moving to its preferred node over a
	 * task that is not.
	 */
	यदि (env->best_task && cur->numa_preferred_nid == env->src_nid &&
	    env->best_task->numa_preferred_nid != env->src_nid) {
		गच्छ नियोजय;
	}

	/*
	 * If the NUMA importance is less than SMALLIMP,
	 * task migration might only result in ping pong
	 * of tasks and also hurt performance due to cache
	 * misses.
	 */
	यदि (imp < SMALLIMP || imp <= env->best_imp + SMALLIMP / 2)
		गच्छ unlock;

	/*
	 * In the overloaded case, try and keep the load balanced.
	 */
	आरोपय = task_h_आरोपय(env->p) - task_h_आरोपय(cur);
	यदि (!आरोपय)
		गच्छ नियोजय;

	dst_आरोपय = env->dst_stats.आरोपय + आरोपय;
	src_आरोपय = env->src_stats.आरोपय - आरोपय;

	यदि (आरोपय_too_imbalanced(src_आरोपय, dst_आरोपय, env))
		गच्छ unlock;

नियोजय:
	/* Evaluate an idle CPU for a task numa move. */
	यदि (!cur) {
		पूर्णाङ्क cpu = env->dst_stats.idle_cpu;

		/* Nothing cached so current CPU went idle since the search. */
		यदि (cpu < 0)
			cpu = env->dst_cpu;

		/*
		 * If the CPU is no longer truly idle and the previous best CPU
		 * is, keep using it.
		 */
		यदि (!idle_cpu(cpu) && env->best_cpu >= 0 &&
		    idle_cpu(env->best_cpu)) {
			cpu = env->best_cpu;
		}

		env->dst_cpu = cpu;
	}

	task_numa_नियोजय(env, cur, imp);

	/*
	 * If a move to idle is allowed because there is capacity or load
	 * balance improves then stop the search. While a better swap
	 * candidate may exist, a search is not free.
	 */
	यदि (maymove && !cur && env->best_cpu >= 0 && idle_cpu(env->best_cpu))
		stopsearch = true;

	/*
	 * If a swap candidate must be identified and the current best task
	 * moves its preferred node then stop the search.
	 */
	यदि (!maymove && env->best_task &&
	    env->best_task->numa_preferred_nid == env->src_nid) {
		stopsearch = true;
	}
unlock:
	rcu_read_unlock();

	प्रत्यागम stopsearch;
}

स्थिर शून्य task_numa_find_cpu(संरचना task_numa_env *env,
				दीर्घ taskimp, दीर्घ groupimp)
{
	bool maymove = false;
	पूर्णाङ्क cpu;

	/*
	 * If dst node has spare capacity, then check if there is an
	 * imbalance that would be overruled by the load balancer.
	 */
	यदि (env->dst_stats.ग्रन्थिः_type == ग्रन्थिः_has_अतिरिक्तः) {
		अचिह्नित पूर्णाङ्क imbalance;
		पूर्णाङ्क src_running, dst_running;

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
		यदि (!imbalance) {
			maymove = true;
			यदि (env->dst_stats.idle_cpu >= 0) {
				env->dst_cpu = env->dst_stats.idle_cpu;
				task_numa_नियोजय(env, रिक्त, 0);
				प्रत्यागम;
			}
		}
	} अन्यथा {
		दीर्घ src_आरोपय, dst_आरोपय, आरोपय;
		/*
		 * If the improvement from just moving env->p direction is better
		 * than swapping tasks around, check if a move is possible.
		 */
		आरोपय = task_h_आरोपय(env->p);
		dst_आरोपय = env->dst_stats.आरोपय + आरोपय;
		src_आरोपय = env->src_stats.आरोपय - आरोपय;
		maymove = !आरोपय_too_imbalanced(src_आरोपय, dst_आरोपय, env);
	}

	/* Skip CPUs if the source task cannot migrate */
	for_each_cpu_and(cpu, cpumask_सम्बन्धि_ग्रन्थिः(env->dst_nid), env->p->cpus_ptr) {
		env->dst_cpu = cpu;
		यदि (task_numa_compare(env, taskimp, groupimp, maymove))
			भङ्ग;
	}
}

स्थिर पूर्णाङ्क task_numa_migrate(संरचना task_struct *p)
{
	संरचना task_numa_env env = {
		.p = p,

		.src_cpu = task_cpu(p),
		.src_nid = task_ग्रन्थिः(p),

		.imbalance_pct = 112,

		.best_task = रिक्त,
		.best_imp = 0,
		.best_cpu = -1,
	};
	अचिह्नित दीर्घ taskweight, groupweight;
	संरचना sched_domain *sd;
	दीर्घ taskimp, groupimp;
	संरचना numa_गणः *ng;
	संरचना rq *best_rq;
	पूर्णाङ्क nid, ret, dist;

	/*
	 * Pick the lowest SD_NUMA domain, as that would have the smallest
	 * imbalance and would be the first to start moving tasks about.
	 *
	 * And we want to avoid any moving of tasks about, as that would create
	 * random movement of tasks -- counter the numa conditions we're trying
	 * to satisfy here.
	 */
	rcu_read_तालकम्();
	sd = rcu_dereference_all(प्रत्येकम्_cpu(sd_numa, env.src_cpu));
	यदि (sd) {
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
	यदि (unlikely(!sd)) {
		sched_setnuma(p, task_ग्रन्थिः(p));
		प्रत्यागम -EINVAL;
	}

	env.dst_nid = p->numa_preferred_nid;
	dist = env.dist = ग्रन्थिः_distance(env.src_nid, env.dst_nid);
	taskweight = task_weight(p, env.src_nid, dist);
	groupweight = गणः_weight(p, env.src_nid, dist);
	update_numa_stats(&env, &env.src_stats, env.src_nid, false);
	taskimp = task_weight(p, env.dst_nid, dist) - taskweight;
	groupimp = गणः_weight(p, env.dst_nid, dist) - groupweight;
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
	ng = deref_curr_numa_गणः(p);
	यदि (env.best_cpu == -1 || (ng && ng->सक्रिय_nodes > 1)) {
		for_each_ग्रन्थिः_state(nid, N_CPU) {
			यदि (nid == env.src_nid || nid == p->numa_preferred_nid)
				अनुवर्त;

			dist = ग्रन्थिः_distance(env.src_nid, env.dst_nid);
			यदि (sched_numa_topology_type == NUMA_BACKPLANE &&
						dist != env.dist) {
				taskweight = task_weight(p, env.src_nid, dist);
				groupweight = गणः_weight(p, env.src_nid, dist);
			}

			/* Only consider nodes where both task and groups benefit */
			taskimp = task_weight(p, nid, dist) - taskweight;
			groupimp = गणः_weight(p, nid, dist) - groupweight;
			यदि (taskimp < 0 && groupimp < 0)
				अनुवर्त;

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
	यदि (ng) {
		यदि (env.best_cpu == -1)
			nid = env.src_nid;
		अन्यथा
			nid = cpu_गन्तव्यम्_ग्रन्थिः(env.best_cpu);

		यदि (nid != p->numa_preferred_nid)
			sched_setnuma(p, nid);
	}

	/* No better CPU than the current one was found. */
	यदि (env.best_cpu == -1) {
		trace_sched_stick_numa(p, env.src_cpu, रिक्त, -1);
		प्रत्यागम -EAGAIN;
	}

	best_rq = cpu_rq(env.best_cpu);
	यदि (env.best_task == रिक्त) {
		ret = migrate_task_गन्तव्यम्(p, env.best_cpu);
		WRITE_ONCE(best_rq->numa_migrate_on, 0);
		यदि (ret != 0)
			trace_sched_stick_numa(p, env.src_cpu, रिक्त, env.best_cpu);
		प्रत्यागम ret;
	}

	ret = migrate_swap(p, env.best_task, env.best_cpu, env.src_cpu);
	WRITE_ONCE(best_rq->numa_migrate_on, 0);

	यदि (ret != 0)
		trace_sched_stick_numa(p, env.src_cpu, env.best_task, env.best_cpu);
	put_task_struct(env.best_task);
	प्रत्यागम ret;
}

/* Attempt to migrate a task to a CPU on the preferred node. */
स्थिर शून्य numa_migrate_preferred(संरचना task_struct *p)
{
	अचिह्नित दीर्घ interval = HZ;

	/* This task has no NUMA fault statistics yet */
	यदि (unlikely(p->numa_preferred_nid == NUMA_NO_ग्रन्थिः || !p->numa_दोषाः))
		प्रत्यागम;

	/* Periodically retry migrating the task to the preferred node */
	interval = min(interval, msecs_गन्तव्यम्_jiffies(p->numa_अवलोकनम्_period) / 16);
	p->numa_migrate_retry = jiffies + interval;

	/* Success if task is already running on preferred CPU */
	यदि (task_ग्रन्थिः(p) == p->numa_preferred_nid)
		प्रत्यागम;

	/* Otherwise, try migrate to a CPU on the preferred node */
	task_numa_migrate(p);
}

/*
 * Find out how many nodes the workload is actively running on. Do this by
 * tracking the nodes from which NUMA hinting faults are triggered. This can
 * be different from the set of nodes where the workload's memory is currently
 * located.
 */
स्थिर शून्य numa_गणः_गणनम्_सक्रिय_nodes(संरचना numa_गणः *numa_गणः)
{
	अचिह्नित दीर्घ दोषाः, max_दोषाः = 0;
	पूर्णाङ्क nid, सक्रिय_nodes = 0;

	for_each_ग्रन्थिः_state(nid, N_CPU) {
		दोषाः = गणः_दोषाः_cpu(numa_गणः, nid);
		यदि (दोषाः > max_दोषाः)
			max_दोषाः = दोषाः;
	}

	for_each_ग्रन्थिः_state(nid, N_CPU) {
		दोषाः = गणः_दोषाः_cpu(numa_गणः, nid);
		यदि (दोषाः * सक्रिय_ग्रन्थिः_FRACTION > max_दोषाः)
			सक्रिय_nodes++;
	}

	numa_गणः->max_दोषाः_cpu = max_दोषाः;
	numa_गणः->सक्रिय_nodes = सक्रिय_nodes;
}

/*
 * When adapting the scan rate, the period is divided into NUMA_PERIOD_SLOTS
 * increments. The more local the fault statistics are, the higher the scan
 * period will be for the next scan window. If local/(local+remote) ratio is
 * below NUMA_PERIOD_THRESHOLD (where range of ratio is 1..NUMA_PERIOD_SLOTS)
 * the scan period will decrease. Aim for 70% local accesses.
 */
#परिभाषय NUMA_PERIOD_SLOTS 10
#परिभाषय NUMA_PERIOD_देहली 7

/*
 * Increase the scan period (slow down scanning) if the majority of
 * our memory is already on our local node, or if the majority of
 * the page accesses are shared with other processes.
 * Otherwise, decrease the scan period.
 */
स्थिर शून्य update_task_अवलोकनम्_period(संरचना task_struct *p,
			अचिह्नित दीर्घ shared, अचिह्नित दीर्घ गुप्त)
{
	अचिह्नित पूर्णाङ्क period_slot;
	पूर्णाङ्क lr_अनुपातः, ps_अनुपातः;
	पूर्णाङ्क diff;

	अचिह्नित दीर्घ remote = p->numa_दोषाः_locality[0];
	अचिह्नित दीर्घ local = p->numa_दोषाः_locality[1];

	/*
	 * If there were no record hinting faults then either the task is
	 * completely idle or all activity is in areas that are not of interest
	 * to automatic numa balancing. Related to that, if there were failed
	 * migration then it implies we are migrating too quickly or the local
	 * node is overloaded. In either case, scan slower
	 */
	यदि (local + shared == 0 || p->numa_दोषाः_locality[2]) {
		p->numa_अवलोकनम्_period = min(p->numa_अवलोकनम्_period_max,
			p->numa_अवलोकनम्_period << 1);

		p->mm->numa_अग्र_अवलोकनम् = jiffies +
			msecs_गन्तव्यम्_jiffies(p->numa_अवलोकनम्_period);

		गच्छ बहिर्गमनम्;
	}

	/*
	 * Prepare to scale scan period relative to the current period.
	 *	 == NUMA_PERIOD_THRESHOLD scan period stays the same
	 *       <  NUMA_PERIOD_THRESHOLD scan period decreases (scan faster)
	 *	 >= NUMA_PERIOD_THRESHOLD scan period increases (scan slower)
	 */
	period_slot = DIV_आवर्तनम्_ऊर्ध्वम्(p->numa_अवलोकनम्_period, NUMA_PERIOD_SLOTS);
	lr_अनुपातः = (local * NUMA_PERIOD_SLOTS) / (local + remote);
	ps_अनुपातः = (गुप्त * NUMA_PERIOD_SLOTS) / (गुप्त + shared);

	यदि (ps_अनुपातः >= NUMA_PERIOD_देहली) {
		/*
		 * Most memory accesses are local. There is no need to
		 * do fast NUMA scanning, since memory is already local.
		 */
		पूर्णाङ्क slot = ps_अनुपातः - NUMA_PERIOD_देहली;
		यदि (!slot)
			slot = 1;
		diff = slot * period_slot;
	} अन्यथा यदि (lr_अनुपातः >= NUMA_PERIOD_देहली) {
		/*
		 * Most memory accesses are shared with other tasks.
		 * There is no point in continuing fast NUMA scanning,
		 * since other tasks may just move the memory elsewhere.
		 */
		पूर्णाङ्क slot = lr_अनुपातः - NUMA_PERIOD_देहली;
		यदि (!slot)
			slot = 1;
		diff = slot * period_slot;
	} अन्यथा {
		/*
		 * Private memory faults exceed (SLOTS-THRESHOLD)/SLOTS,
		 * yet they are not on the local NUMA node. Speed up
		 * NUMA scanning to get the memory moved over.
		 */
		पूर्णाङ्क अनुपातः = max(lr_अनुपातः, ps_अनुपातः);
		diff = -(NUMA_PERIOD_देहली - अनुपातः) * period_slot;
	}

	p->numa_अवलोकनम्_period = clamp(p->numa_अवलोकनम्_period + diff,
			task_अवलोकनम्_min(p), task_अवलोकनम्_max(p));

बहिर्गमनम्:
	स्मृतिं_पूरय(p->numa_दोषाः_locality, 0,
	       परिमाण(p->numa_दोषाः_locality));
}

/*
 * Get the fraction of time the task has been running since the last
 * NUMA placement cycle. The scheduler keeps similar statistics, but
 * decays those on a 32ms period, which is orders of magnitude off
 * from the dozens-of-seconds NUMA balancing period. Use the scheduler
 * stats only if the task is so new there are no NUMA statistics yet.
 */
स्थिर u64 numa_get_avg_runtime(संरचना task_struct *p, u64 *period)
{
	u64 runtime, delta, now;
	/* Use the start of this time slice to avoid calculations. */
	now = p->se.exec_start;
	runtime = p->se.योग_exec_runtime;

	यदि (p->चरमः_task_numa_placement) {
		delta = runtime - p->चरमः_योग_exec_runtime;
		*period = now - p->चरमः_task_numa_placement;

		/* Avoid time going backwards, prevent potential divide error: */
		यदि (unlikely((s64)*period < 0))
			*period = 0;
	} अन्यथा {
		delta = p->se.avg.आरोपय_योग;
		*period = आरोपय_AVG_MAX;
	}

	p->चरमः_योग_exec_runtime = runtime;
	p->चरमः_task_numa_placement = now;

	प्रत्यागम delta;
}

/*
 * Determine the preferred nid for a task in a numa_group. This needs to
 * be done in a way that produces consistent results with group_weight,
 * otherwise workloads might not converge.
 */
स्थिर पूर्णाङ्क preferred_गणः_nid(संरचना task_struct *p, पूर्णाङ्क nid)
{
	nodemask_t nodes;
	पूर्णाङ्क dist;

	/* Direct connections between all NUMA nodes. */
	यदि (sched_numa_topology_type == NUMA_प्रत्यक्षम्)
		प्रत्यागम nid;

	/*
	 * On a system with glueless mesh NUMA topology, group_weight
	 * scores nodes according to the number of NUMA hinting faults on
	 * both the node itself, and on nearby nodes.
	 */
	यदि (sched_numa_topology_type == NUMA_GLUELESS_MESH) {
		अचिह्नित दीर्घ score, max_score = 0;
		पूर्णाङ्क ग्रन्थिः, max_ग्रन्थिः = nid;

		dist = sched_max_numa_distance;

		for_each_ग्रन्थिः_state(ग्रन्थिः, N_CPU) {
			score = गणः_weight(p, ग्रन्थिः, dist);
			यदि (score > max_score) {
				max_score = score;
				max_ग्रन्थिः = ग्रन्थिः;
			}
		}
		प्रत्यागम max_ग्रन्थिः;
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
	nodes = ग्रन्थिः_अवस्थाः[N_CPU];
	प्रति (dist = sched_max_numa_distance; dist > LOCAL_DISTANCE; dist--) {
		अचिह्नित दीर्घ max_दोषाः = 0;
		nodemask_t max_गणः = ग्रन्थिः_आवरणम्_NONE;
		पूर्णाङ्क a, b;

		/* Are there nodes at this distance from each other? */
		यदि (!find_numa_distance(dist))
			अनुवर्त;

		for_each_ग्रन्थिः_आवरणम्(a, nodes) {
			अचिह्नित दीर्घ दोषाः = 0;
			nodemask_t this_गणः;
			nodes_clear(this_गणः);

			/* Sum group's NUMA faults; includes a==b case. */
			for_each_ग्रन्थिः_आवरणम्(b, nodes) {
				यदि (ग्रन्थिः_distance(a, b) < dist) {
					दोषाः += गणः_दोषाः(p, b);
					ग्रन्थिः_समुच्चयः(b, this_गणः);
					ग्रन्थिः_clear(b, nodes);
				}
			}

			/* Remember the top group. */
			यदि (दोषाः > max_दोषाः) {
				max_दोषाः = दोषाः;
				max_गणः = this_गणः;
				/*
				 * subtle: at the smallest distance there is
				 * just one node left in each "group", the
				 * winner is the preferred nid.
				 */
				nid = a;
			}
		}
		/* Next round, evaluate the nodes within max_group. */
		यदि (!max_दोषाः)
			भङ्ग;
		nodes = max_गणः;
	}
	प्रत्यागम nid;
}

स्थिर शून्य task_numa_placement(संरचना task_struct *p)
	__context_unsafe(/* conditional locking */)
{
	संरचना sched_cache_गणः __maybe_unused *grp;
	पूर्णाङ्क seq, nid, max_nid = NUMA_NO_ग्रन्थिः;
	अचिह्नित दीर्घ max_दोषाः = 0;
	अचिह्नित दीर्घ fault_types[2] = { 0, 0 };
	अचिह्नित दीर्घ योग_दोषाः;
	u64 runtime, period;
	spinlock_t *गणः_तालकम् = रिक्त;
	दीर्घ __maybe_unused new_fp;
	संरचना numa_गणः *ng;

	/*
	 * The p->mm->numa_scan_seq field gets updated without
	 * exclusive access. Use READ_ONCE() here to ensure
	 * that the field is read in a single access:
	 */
	seq = READ_ONCE(p->mm->numa_अवलोकनम्_seq);
	यदि (p->numa_अवलोकनम्_seq == seq)
		प्रत्यागम;
	p->numa_अवलोकनम्_seq = seq;
	p->numa_अवलोकनम्_period_max = task_अवलोकनम्_max(p);

	योग_दोषाः = p->numa_दोषाः_locality[0] +
		       p->numa_दोषाः_locality[1];
	runtime = numa_get_avg_runtime(p, &period);

	/* If the task is part of a group prevent parallel updates to group stats */
	ng = deref_curr_numa_गणः(p);
	यदि (ng) {
		गणः_तालकम् = &ng->तालकम्;
		spin_तालकम्_irq(गणः_तालकम्);
	}

	/* Find the node with the highest number of faults */
	for_each_online_ग्रन्थिः(nid) {
		/* Keep track of the offsets in numa_faults array */
		पूर्णाङ्क mem_idx, membuf_idx, cpu_idx, cpubuf_idx;
		अचिह्नित दीर्घ दोषाः = 0, गणः_दोषाः = 0;
		पूर्णाङ्क priv;

		प्रति (priv = 0; priv < NR_NUMA_HINT_FAULT_TYPES; priv++) {
			दीर्घ diff, f_diff, f_weight;

			mem_idx = task_दोषाः_idx(NUMA_MEM, nid, priv);
			membuf_idx = task_दोषाः_idx(NUMA_MEMBUF, nid, priv);
			cpu_idx = task_दोषाः_idx(NUMA_CPU, nid, priv);
			cpubuf_idx = task_दोषाः_idx(NUMA_CPUBUF, nid, priv);

			/* Decay existing window, copy faults since last scan */
			diff = p->numa_दोषाः[membuf_idx] - p->numa_दोषाः[mem_idx] / 2;
			fault_types[priv] += p->numa_दोषाः[membuf_idx];
			p->numa_दोषाः[membuf_idx] = 0;

			/*
			 * Normalize the faults_from, so all tasks in a group
			 * count according to CPU use, instead of by the raw
			 * number of faults. Tasks with little runtime have
			 * little over-all impact on throughput, and thus their
			 * faults are less important.
			 */
			f_weight = div64_u64(runtime << 16, period + 1);
			f_weight = (f_weight * p->numa_दोषाः[cpubuf_idx]) /
				   (योग_दोषाः + 1);
			f_diff = f_weight - p->numa_दोषाः[cpu_idx] / 2;
			p->numa_दोषाः[cpubuf_idx] = 0;

			p->numa_दोषाः[mem_idx] += diff;
			p->numa_दोषाः[cpu_idx] += f_diff;
			दोषाः += p->numa_दोषाः[mem_idx];
			p->योग_numa_दोषाः += diff;
			यदि (ng) {
				/*
				 * safe because we can only change our own group
				 *
				 * mem_idx represents the offset for a given
				 * nid and priv in a specific region because it
				 * is at the beginning of the numa_faults array.
				 */
				ng->दोषाः[mem_idx] += diff;
				ng->दोषाः[cpu_idx] += f_diff;
				ng->योग_दोषाः += diff;
				गणः_दोषाः += ng->दोषाः[mem_idx];
			}
#यदि_परिभाषितम् CONFIG_SCHED_CACHE
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

				यदि (grp) {
					new_fp = (दीर्घ)READ_ONCE(grp->footprint) + diff;
					WRITE_ONCE(grp->footprint, max(new_fp, 0L));
				}
			}
#यदि_अन्तः
		}

		यदि (!ng) {
			यदि (दोषाः > max_दोषाः) {
				max_दोषाः = दोषाः;
				max_nid = nid;
			}
		} अन्यथा यदि (गणः_दोषाः > max_दोषाः) {
			max_दोषाः = गणः_दोषाः;
			max_nid = nid;
		}
	}

	/* Cannot migrate task to CPU-less node */
	max_nid = numa_निकटतमः_ग्रन्थिः(max_nid, N_CPU);

	यदि (ng) {
		numa_गणः_गणनम्_सक्रिय_nodes(ng);
		spin_unlock_irq(गणः_तालकम्);
		max_nid = preferred_गणः_nid(p, max_nid);
	}

	यदि (max_दोषाः) {
		/* Set the new preferred node */
		यदि (max_nid != p->numa_preferred_nid)
			sched_setnuma(p, max_nid);
	}

	update_task_अवलोकनम्_period(p, fault_types[0], fault_types[1]);
}

स्थिर अन्तःस्थ पूर्णाङ्क get_numa_गणः(संरचना numa_गणः *grp)
{
	प्रत्यागम refcount_inc_not_zero(&grp->refcount);
}

स्थिर अन्तःस्थ शून्य put_numa_गणः(संरचना numa_गणः *grp)
{
	यदि (refcount_dec_and_test(&grp->refcount))
		kfree_rcu(grp, rcu);
}

स्थिर शून्य task_numa_गणः(संरचना task_struct *p, पूर्णाङ्क cpupid, पूर्णाङ्क flags,
			पूर्णाङ्क *priv)
{
	संरचना numa_गणः *grp, *my_grp;
	संरचना task_struct *tsk;
	bool join = false;
	पूर्णाङ्क cpu = cpupid_गन्तव्यम्_cpu(cpupid);
	पूर्णाङ्क i;

	यदि (unlikely(!deref_curr_numa_गणः(p))) {
		अचिह्नित पूर्णाङ्क आकार = परिमाण(संरचना numa_गणः) +
				    NR_NUMA_HINT_FAULT_STATS *
				    nr_ग्रन्थिः_ids * परिमाण(अचिह्नित दीर्घ);

		grp = kzalloc(आकार, GFP_KERNEL | __GFP_NOWARN);
		यदि (!grp)
			प्रत्यागम;

		refcount_समुच्चयः(&grp->refcount, 1);
		grp->सक्रिय_nodes = 1;
		grp->max_दोषाः_cpu = 0;
		spin_तालकम्_init(&grp->तालकम्);
		grp->gid = p->pid;

		प्रति (i = 0; i < NR_NUMA_HINT_FAULT_STATS * nr_ग्रन्थिः_ids; i++)
			grp->दोषाः[i] = p->numa_दोषाः[i];

		grp->योग_दोषाः = p->योग_numa_दोषाः;

		grp->nr_tasks++;
		rcu_नियोजय_pointer(p->numa_गणः, grp);
	}

	rcu_read_तालकम्();
	tsk = READ_ONCE(cpu_rq(cpu)->curr);

	यदि (!cpupid_मेलनम्_pid(tsk, cpupid))
		गच्छ no_join;

	grp = rcu_dereference_all(tsk->numa_गणः);
	यदि (!grp)
		गच्छ no_join;

	my_grp = deref_curr_numa_गणः(p);
	यदि (grp == my_grp)
		गच्छ no_join;

	/*
	 * Only join the other group if its bigger; if we're the bigger group,
	 * the other task will join us.
	 */
	यदि (my_grp->nr_tasks > grp->nr_tasks)
		गच्छ no_join;

	/*
	 * Tie-break on the grp address.
	 */
	यदि (my_grp->nr_tasks == grp->nr_tasks && my_grp > grp)
		गच्छ no_join;

	/* Always join threads in the same process. */
	यदि (tsk->mm == current->mm)
		join = true;

	/* Simple filter to avoid false positives due to PID collisions */
	यदि (flags & TNF_SHARED)
		join = true;

	/* Update priv based on whether false sharing was detected */
	*priv = !join;

	यदि (join && !get_numa_गणः(grp))
		गच्छ no_join;

	rcu_read_unlock();

	यदि (!join)
		प्रत्यागम;

	WARN_ON_ONCE(irqs_disabled());
	double_तालकम्_irq(&my_grp->तालकम्, &grp->तालकम्);

	प्रति (i = 0; i < NR_NUMA_HINT_FAULT_STATS * nr_ग्रन्थिः_ids; i++) {
		my_grp->दोषाः[i] -= p->numa_दोषाः[i];
		grp->दोषाः[i] += p->numa_दोषाः[i];
	}
	my_grp->योग_दोषाः -= p->योग_numa_दोषाः;
	grp->योग_दोषाः += p->योग_numa_दोषाः;

	my_grp->nr_tasks--;
	grp->nr_tasks++;

	spin_unlock(&my_grp->तालकम्);
	spin_unlock_irq(&grp->तालकम्);

	rcu_नियोजय_pointer(p->numa_गणः, grp);

	put_numa_गणः(my_grp);
	प्रत्यागम;

no_join:
	rcu_read_unlock();
	प्रत्यागम;
}

/*
 * Get rid of NUMA statistics associated with a task (either current or dead).
 * If @final is set, the task is dead and has reached refcount zero, so we can
 * safely free all relevant data structures. Otherwise, there might be
 * concurrent reads from places like load balancing and procfs, and we should
 * reset the data back to default state without freeing ->numa_faults.
 */
शून्य task_numa_free(संरचना task_struct *p, bool final)
{
	/* safe: p either is current or is being freed by current */
	संरचना numa_गणः *grp = rcu_dereference_raw(p->numa_गणः);
	अचिह्नित दीर्घ *numa_दोषाः = p->numa_दोषाः;
	अचिह्नित दीर्घ flags;
	पूर्णाङ्क i;

	यदि (!numa_दोषाः)
		प्रत्यागम;

	यदि (grp) {
		spin_तालकम्_irqsave(&grp->तालकम्, flags);
		प्रति (i = 0; i < NR_NUMA_HINT_FAULT_STATS * nr_ग्रन्थिः_ids; i++)
			grp->दोषाः[i] -= p->numa_दोषाः[i];
		grp->योग_दोषाः -= p->योग_numa_दोषाः;

		grp->nr_tasks--;
		spin_unlock_irqrestore(&grp->तालकम्, flags);
		RCU_INIT_POINTER(p->numa_गणः, रिक्त);
		put_numa_गणः(grp);
	}

	यदि (final) {
		p->numa_दोषाः = रिक्त;
		kfree(numa_दोषाः);
	} अन्यथा {
		p->योग_numa_दोषाः = 0;
		प्रति (i = 0; i < NR_NUMA_HINT_FAULT_STATS * nr_ग्रन्थिः_ids; i++)
			numa_दोषाः[i] = 0;
	}
}

/*
 * Got a PROT_NONE fault for a page on @node.
 */
शून्य task_numa_fault(पूर्णाङ्क चरमः_cpupid, पूर्णाङ्क mem_ग्रन्थिः, पूर्णाङ्क पृष्ठानि, पूर्णाङ्क flags)
{
	संरचना task_struct *p = current;
	bool migrated = flags & TNF_MIGRATED;
	पूर्णाङ्क cpu_ग्रन्थिः = task_ग्रन्थिः(current);
	पूर्णाङ्क local = !!(flags & TNF_FAULT_LOCAL);
	संरचना numa_गणः *ng;
	पूर्णाङ्क priv;

	यदि (!static_branch_likely(&sched_numa_balancing))
		प्रत्यागम;

	/* for example, ksmd faulting in a user's mm */
	यदि (!p->mm)
		प्रत्यागम;

	/*
	 * NUMA faults statistics are unnecessary for the slow memory
	 * node for memory tiering mode.
	 */
	यदि (!ग्रन्थिः_is_toptier(mem_ग्रन्थिः) &&
	    (sysctl_numa_balancing_बहुलकम् & NUMA_BALANCING_स्मृतिः_TIERING ||
	     !cpupid_valid(चरमः_cpupid)))
		प्रत्यागम;

	/* Allocate buffer to track faults on a per-node basis */
	यदि (unlikely(!p->numa_दोषाः)) {
		पूर्णाङ्क आकार = परिमाण(*p->numa_दोषाः) *
			   NR_NUMA_HINT_FAULT_BUCKETS * nr_ग्रन्थिः_ids;

		p->numa_दोषाः = kzalloc(आकार, GFP_KERNEL|__GFP_NOWARN);
		यदि (!p->numa_दोषाः)
			प्रत्यागम;

		p->योग_numa_दोषाः = 0;
		स्मृतिं_पूरय(p->numa_दोषाः_locality, 0, परिमाण(p->numa_दोषाः_locality));
	}

	/*
	 * First accesses are treated as private, otherwise consider accesses
	 * to be private if the accessing pid has not changed
	 */
	यदि (unlikely(चरमः_cpupid == (-1 & चरमः_CPUPID_आवरणम्))) {
		priv = 1;
	} अन्यथा {
		priv = cpupid_मेलनम्_pid(p, चरमः_cpupid);
		यदि (!priv && !(flags & TNF_NO_गणः))
			task_numa_गणः(p, चरमः_cpupid, flags, &priv);
	}

	/*
	 * If a workload spans multiple NUMA nodes, a shared fault that
	 * occurs wholly within the set of nodes that the workload is
	 * actively using should be counted as local. This allows the
	 * scan rate to slow down when a workload has settled down.
	 */
	ng = deref_curr_numa_गणः(p);
	यदि (!priv && !local && ng && ng->सक्रिय_nodes > 1 &&
				numa_is_सक्रिय_ग्रन्थिः(cpu_ग्रन्थिः, ng) &&
				numa_is_सक्रिय_ग्रन्थिः(mem_ग्रन्थिः, ng))
		local = 1;

	/*
	 * Retry to migrate task to preferred node periodically, in case it
	 * previously failed, or the scheduler moved us.
	 */
	यदि (time_after(jiffies, p->numa_migrate_retry)) {
		task_numa_placement(p);
		numa_migrate_preferred(p);
	}

	यदि (migrated)
		p->numa_पृष्ठानि_migrated += पृष्ठानि;
	यदि (flags & TNF_MIGRATE_FAIL)
		p->numa_दोषाः_locality[2] += पृष्ठानि;

	p->numa_दोषाः[task_दोषाः_idx(NUMA_MEMBUF, mem_ग्रन्थिः, priv)] += पृष्ठानि;
	p->numa_दोषाः[task_दोषाः_idx(NUMA_CPUBUF, cpu_ग्रन्थिः, priv)] += पृष्ठानि;
	p->numa_दोषाः_locality[local] += पृष्ठानि;
}

स्थिर शून्य reset_ptenuma_अवलोकनम्(संरचना task_struct *p)
{
	/*
	 * We only did a read acquisition of the mmap sem, so
	 * p->mm->numa_scan_seq is written to without exclusive access
	 * and the update is not guaranteed to be atomic. That's not
	 * much of an issue though, since this is just used for
	 * statistical sampling. Use READ_ONCE/WRITE_ONCE, which are not
	 * expensive, to avoid any form of compiler optimizations:
	 */
	WRITE_ONCE(p->mm->numa_अवलोकनम्_seq, READ_ONCE(p->mm->numa_अवलोकनम्_seq) + 1);
	p->mm->numa_अवलोकनम्_विस्थापनम् = 0;
}

स्थिर bool vma_is_accessed(संरचना mm_struct *mm, संरचना vm_area_struct *vma)
{
	अचिह्नित दीर्घ pids;
	/*
	 * Allow unconditional access first two times, so that all the (pages)
	 * of VMAs get prot_none fault introduced irrespective of accesses.
	 * This is also done to avoid any side effect of task scanning
	 * amplifying the unfairness of disjoint set of VMAs' access.
	 */
	यदि ((READ_ONCE(current->mm->numa_अवलोकनम्_seq) - vma->numab_state->start_अवलोकनम्_seq) < 2)
		प्रत्यागम true;

	pids = vma->numab_state->pids_सक्रिय[0] | vma->numab_state->pids_सक्रिय[1];
	यदि (test_bit(hash_32(current->pid, ilog2(द्वयङ्काः_प्रत्येकम्_LONG)), &pids))
		प्रत्यागम true;

	/*
	 * Complete a scan that has already started regardless of PID access, or
	 * some VMAs may never be scanned in multi-threaded applications:
	 */
	यदि (mm->numa_अवलोकनम्_विस्थापनम् > vma->vm_start) {
		trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_IGNORE_PID);
		प्रत्यागम true;
	}

	/*
	 * This vma has not been accessed for a while, and if the number
	 * the threads in the same process is low, which means no other
	 * threads can help scan this vma, force a vma scan.
	 */
	यदि (READ_ONCE(mm->numa_अवलोकनम्_seq) >
	   (vma->numab_state->पूर्व_अवलोकनम्_seq + get_nr_तन्तवः(current)))
		प्रत्यागम true;

	प्रत्यागम false;
}

#परिभाषय VMA_PID_RESET_PERIOD (4 * sysctl_numa_balancing_अवलोकनम्_delay)

/*
 * The expensive part of numa migration is done from task_work context.
 * Triggered from task_tick_numa().
 */
स्थिर शून्य task_numa_work(संरचना callback_शीर्ष *work)
{
	अचिह्नित दीर्घ migrate, अग्र_अवलोकनम्, now = jiffies;
	संरचना task_struct *p = current;
	संरचना mm_struct *mm = p->mm;
	u64 runtime = p->se.योग_exec_runtime;
	संरचना vm_area_struct *vma;
	अचिह्नित दीर्घ start, अन्तः;
	अचिह्नित दीर्घ nr_pte_updates = 0;
	दीर्घ पृष्ठानि, virtpages;
	संरचना vma_iterator vmi;
	bool vma_pids_skipped;
	bool vma_pids_forced = false;

	WARN_ON_ONCE(p != container_सम्बन्धि(work, संरचना task_struct, numa_work));

	work->अग्र = work;
	/*
	 * Who cares about NUMA placement when they're dying.
	 *
	 * NOTE: make sure not to dereference p->mm before this check,
	 * exit_task_work() happens _after_ exit_mm() so we could be called
	 * without p->mm even though we still had it when we enqueued this
	 * work.
	 */
	यदि (p->flags & PF_EXITING)
		प्रत्यागम;

	/*
	 * Memory is pinned to only one NUMA node via cpuset.mems, naturally
	 * no page can be migrated.
	 */
	यदि (cpusets_enabled() && nodes_weight(cpuset_current_mems_allowed) == 1) {
		trace_sched_skip_cpuset_numa(current, &cpuset_current_mems_allowed);
		प्रत्यागम;
	}

	यदि (!mm->numa_अग्र_अवलोकनम्) {
		mm->numa_अग्र_अवलोकनम् = now +
			msecs_गन्तव्यम्_jiffies(sysctl_numa_balancing_अवलोकनम्_delay);
	}

	/*
	 * Enforce maximal scan/migration frequency..
	 */
	migrate = mm->numa_अग्र_अवलोकनम्;
	यदि (time_before(now, migrate))
		प्रत्यागम;

	यदि (p->numa_अवलोकनम्_period == 0) {
		p->numa_अवलोकनम्_period_max = task_अवलोकनम्_max(p);
		p->numa_अवलोकनम्_period = task_अवलोकनम्_start(p);
	}

	अग्र_अवलोकनम् = now + msecs_गन्तव्यम्_jiffies(p->numa_अवलोकनम्_period);
	यदि (!try_cmpxchg(&mm->numa_अग्र_अवलोकनम्, &migrate, अग्र_अवलोकनम्))
		प्रत्यागम;

	/*
	 * Delay this task enough that another task of this mm will likely win
	 * the next time around.
	 */
	p->ग्रन्थिः_stamp += 2 * TICK_NSEC;

	पृष्ठानि = sysctl_numa_balancing_अवलोकनम्_आकार;
	पृष्ठानि <<= 20 - PAGE_सरणम्; /* MB in pages */
	virtpages = पृष्ठानि * 8;	   /* Scan up to this much virtual space */
	यदि (!पृष्ठानि)
		प्रत्यागम;


	यदि (!mmap_read_trylock(mm))
		प्रत्यागम;

	/*
	 * VMAs are skipped if the current PID has not trapped a fault within
	 * the VMA recently. Allow scanning to be forced if there is no
	 * suitable VMA remaining.
	 */
	vma_pids_skipped = false;

retry_pids:
	start = mm->numa_अवलोकनम्_विस्थापनम्;
	vma_iter_init(&vmi, mm, start);
	vma = vma_अग्र(&vmi);
	यदि (!vma) {
		reset_ptenuma_अवलोकनम्(p);
		start = 0;
		vma_iter_समुच्चयः(&vmi, start);
		vma = vma_अग्र(&vmi);
	}

	प्रति (; vma; vma = vma_अग्र(&vmi)) {
		यदि (!vma_migratable(vma) || !vma_policy_mof(vma) ||
			is_vm_hugetlb_page(vma) || (vma->vm_flags & VM_MIXEDMAP)) {
			trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_UNSUITABLE);
			अनुवर्त;
		}

		/*
		 * Shared library pages mapped by multiple processes are not
		 * migrated as it is expected they are cache replicated. Avoid
		 * hinting faults in read-only file-backed mappings or the vDSO
		 * as migrating the pages will be of marginal benefit.
		 */
		यदि (!vma->vm_mm ||
		    (vma->vm_file && (vma->vm_flags & (VM_READ|VM_WRITE)) == (VM_READ))) {
			trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_SHARED_RO);
			अनुवर्त;
		}

		/*
		 * Skip inaccessible VMAs to avoid any confusion between
		 * PROT_NONE and NUMA hinting PTEs
		 */
		यदि (!vma_is_accessible(vma)) {
			trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_INACCESSIBLE);
			अनुवर्त;
		}

		/* Initialise new per-VMA NUMAB state. */
		यदि (!vma->numab_state) {
			संरचना vma_numab_state *ptr;

			ptr = kzalloc_obj(*ptr);
			यदि (!ptr)
				अनुवर्त;

			यदि (cmpxchg(&vma->numab_state, रिक्त, ptr)) {
				kfree(ptr);
				अनुवर्त;
			}

			vma->numab_state->start_अवलोकनम्_seq = mm->numa_अवलोकनम्_seq;

			vma->numab_state->अग्र_अवलोकनम् = now +
				msecs_गन्तव्यम्_jiffies(sysctl_numa_balancing_अवलोकनम्_delay);

			/* Reset happens after 4 times scan delay of scan start */
			vma->numab_state->pids_सक्रिय_reset =  vma->numab_state->अग्र_अवलोकनम् +
				msecs_गन्तव्यम्_jiffies(VMA_PID_RESET_PERIOD);

			/*
			 * Ensure prev_scan_seq does not match numa_scan_seq,
			 * to prevent VMAs being skipped prematurely on the
			 * first scan:
			 */
			 vma->numab_state->पूर्व_अवलोकनम्_seq = mm->numa_अवलोकनम्_seq - 1;
		}

		/*
		 * Scanning the VMAs of short lived tasks add more overhead. So
		 * delay the scan for new VMAs.
		 */
		यदि (mm->numa_अवलोकनम्_seq && time_before(jiffies,
						vma->numab_state->अग्र_अवलोकनम्)) {
			trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_अवलोकनम्_DELAY);
			अनुवर्त;
		}

		/* RESET access PIDs regularly for old VMAs. */
		यदि (mm->numa_अवलोकनम्_seq &&
				time_after(jiffies, vma->numab_state->pids_सक्रिय_reset)) {
			vma->numab_state->pids_सक्रिय_reset = vma->numab_state->pids_सक्रिय_reset +
				msecs_गन्तव्यम्_jiffies(VMA_PID_RESET_PERIOD);
			vma->numab_state->pids_सक्रिय[0] = READ_ONCE(vma->numab_state->pids_सक्रिय[1]);
			vma->numab_state->pids_सक्रिय[1] = 0;
		}

		/* Do not rescan VMAs twice within the same sequence. */
		यदि (vma->numab_state->पूर्व_अवलोकनम्_seq == mm->numa_अवलोकनम्_seq) {
			mm->numa_अवलोकनम्_विस्थापनम् = vma->vm_अन्तः;
			trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_SEQ_COMPLETED);
			अनुवर्त;
		}

		/*
		 * Do not scan the VMA if task has not accessed it, unless no other
		 * VMA candidate exists.
		 */
		यदि (!vma_pids_forced && !vma_is_accessed(mm, vma)) {
			vma_pids_skipped = true;
			trace_sched_skip_vma_numa(mm, vma, NUMAB_SKIP_PID_INACTIVE);
			अनुवर्त;
		}

		कुरु {
			start = max(start, vma->vm_start);
			अन्तः = ALIGN(start + (पृष्ठानि << PAGE_सरणम्), HPAGE_आकार);
			अन्तः = min(अन्तः, vma->vm_अन्तः);
			nr_pte_updates = change_prot_numa(vma, start, अन्तः);

			/*
			 * Try to scan sysctl_numa_balancing_size worth of
			 * hpages that have at least one present PTE that
			 * is not already PTE-numa. If the VMA contains
			 * areas that are unused or already full of prot_numa
			 * PTEs, scan up to virtpages, to skip through those
			 * areas faster.
			 */
			यदि (nr_pte_updates)
				पृष्ठानि -= (अन्तः - start) >> PAGE_सरणम्;
			virtpages -= (अन्तः - start) >> PAGE_सरणम्;

			start = अन्तः;
			यदि (पृष्ठानि <= 0 || virtpages <= 0)
				गच्छ बहिर्गमनम्;

			cond_resched();
		} यावत् (अन्तः != vma->vm_अन्तः);

		/* VMA scan is complete, do not scan until next sequence. */
		vma->numab_state->पूर्व_अवलोकनम्_seq = mm->numa_अवलोकनम्_seq;

		/*
		 * Only force scan within one VMA at a time, to limit the
		 * cost of scanning a potentially uninteresting VMA.
		 */
		यदि (vma_pids_forced)
			भङ्ग;
	}

	/*
	 * If no VMAs are remaining and VMAs were skipped due to the PID
	 * not accessing the VMA previously, then force a scan to ensure
	 * forward progress:
	 */
	यदि (!vma && !vma_pids_forced && vma_pids_skipped) {
		vma_pids_forced = true;
		गच्छ retry_pids;
	}

बहिर्गमनम्:
	/*
	 * It is possible to reach the end of the VMA list but the last few
	 * VMAs are not guaranteed to the vma_migratable. If they are not, we
	 * would find the !migratable VMA on the next scan but not reset the
	 * scanner to the start so check it now.
	 */
	यदि (vma)
		mm->numa_अवलोकनम्_विस्थापनम् = start;
	अन्यथा
		reset_ptenuma_अवलोकनम्(p);
	mmap_read_unlock(mm);

	/*
	 * Make sure tasks use at least 32x as much time to run other code
	 * than they used here, to limit NUMA PTE scanning overhead to 3% max.
	 * Usually update_task_scan_period slows down scanning enough; on an
	 * overloaded system we need to limit overhead on a per task basis.
	 */
	यदि (unlikely(p->se.योग_exec_runtime != runtime)) {
		u64 diff = p->se.योग_exec_runtime - runtime;
		p->ग्रन्थिः_stamp += 32 * diff;
	}
}

शून्य init_numa_balancing(u64 clone_flags, संरचना task_struct *p)
{
	पूर्णाङ्क mm_users = 0;
	संरचना mm_struct *mm = p->mm;

	यदि (mm) {
		mm_users = atomic_read(&mm->mm_users);
		यदि (mm_users == 1) {
			mm->numa_अग्र_अवलोकनम् = jiffies + msecs_गन्तव्यम्_jiffies(sysctl_numa_balancing_अवलोकनम्_delay);
			mm->numa_अवलोकनम्_seq = 0;
		}
	}
	p->ग्रन्थिः_stamp			= 0;
	p->numa_अवलोकनम्_seq		= mm ? mm->numa_अवलोकनम्_seq : 0;
	p->numa_अवलोकनम्_period		= sysctl_numa_balancing_अवलोकनम्_delay;
	p->numa_migrate_retry		= 0;
	/* Protect against double add, see task_tick_numa and task_numa_work */
	p->numa_work.अग्र		= &p->numa_work;
	p->numa_दोषाः			= रिक्त;
	p->numa_पृष्ठानि_migrated		= 0;
	p->योग_numa_दोषाः		= 0;
	RCU_INIT_POINTER(p->numa_गणः, रिक्त);
	p->चरमः_task_numa_placement	= 0;
	p->चरमः_योग_exec_runtime	= 0;

	init_task_work(&p->numa_work, task_numa_work);

	/* New address space, reset the preferred nid */
	यदि (!(clone_flags & CLONE_VM)) {
		p->numa_preferred_nid = NUMA_NO_ग्रन्थिः;
		प्रत्यागम;
	}

	/*
	 * New thread, keep existing numa_preferred_nid which should be copied
	 * already by arch_dup_task_struct but stagger when scans start.
	 */
	यदि (mm) {
		अचिह्नित पूर्णाङ्क delay;

		delay = min_t(अचिह्नित पूर्णाङ्क, task_अवलोकनम्_max(current),
			current->numa_अवलोकनम्_period * mm_users * NSEC_प्रत्येकम्_MSEC);
		delay += 2 * TICK_NSEC;
		p->ग्रन्थिः_stamp = delay;
	}
}

/*
 * Drive the periodic memory faults..
 */
स्थिर शून्य task_tick_numa(संरचना rq *rq, संरचना task_struct *curr)
{
	संरचना callback_शीर्ष *work = &curr->numa_work;
	u64 period, now;

	/*
	 * We don't care about NUMA placement if we don't have memory.
	 */
	यदि (!curr->mm || (curr->flags & (PF_EXITING | PF_KTHREAD)) || work->अग्र != work)
		प्रत्यागम;

	/*
	 * Using runtime rather than walltime has the dual advantage that
	 * we (mostly) drive the selection from busy threads and that the
	 * task needs to have done some actual work before we bother with
	 * NUMA placement.
	 */
	now = curr->se.योग_exec_runtime;
	period = (u64)curr->numa_अवलोकनम्_period * NSEC_प्रत्येकम्_MSEC;

	यदि (now > curr->ग्रन्थिः_stamp + period) {
		यदि (!curr->ग्रन्थिः_stamp)
			curr->numa_अवलोकनम्_period = task_अवलोकनम्_start(curr);
		curr->ग्रन्थिः_stamp += period;

		यदि (!time_before(jiffies, curr->mm->numa_अग्र_अवलोकनम्))
			task_work_सङ्कलय(curr, work, TWA_RESUME);
	}
}

स्थिर शून्य update_अवलोकनम्_period(संरचना task_struct *p, पूर्णाङ्क new_cpu)
{
	पूर्णाङ्क src_nid = cpu_गन्तव्यम्_ग्रन्थिः(task_cpu(p));
	पूर्णाङ्क dst_nid = cpu_गन्तव्यम्_ग्रन्थिः(new_cpu);

	यदि (!static_branch_likely(&sched_numa_balancing))
		प्रत्यागम;

	यदि (!p->mm || !p->numa_दोषाः || (p->flags & PF_EXITING))
		प्रत्यागम;

	यदि (src_nid == dst_nid)
		प्रत्यागम;

	/*
	 * Allow resets if faults have been trapped before one scan
	 * has completed. This is most likely due to a new task that
	 * is pulled cross-node due to wakeups or load balancing.
	 */
	यदि (p->numa_अवलोकनम्_seq) {
		/*
		 * Avoid scan adjustments if moving to the preferred
		 * node or if the task was not previously running on
		 * the preferred node.
		 */
		यदि (dst_nid == p->numa_preferred_nid ||
		    (p->numa_preferred_nid != NUMA_NO_ग्रन्थिः &&
			src_nid != p->numa_preferred_nid))
			प्रत्यागम;
	}

	p->numa_अवलोकनम्_period = task_अवलोकनम्_start(p);
}

#अन्यथा /* !CONFIG_NUMA_BALANCING: */

स्थिर शून्य task_tick_numa(संरचना rq *rq, संरचना task_struct *curr)
{
}

स्थिर अन्तःस्थ शून्य account_numa_पङ्क्तौ_निक्षिप(संरचना rq *rq, संरचना task_struct *p)
{
}

स्थिर अन्तःस्थ शून्य account_numa_पङ्क्तेः_निष्कासय(संरचना rq *rq, संरचना task_struct *p)
{
}

स्थिर अन्तःस्थ शून्य update_अवलोकनम्_period(संरचना task_struct *p, पूर्णाङ्क new_cpu)
{
}

#यदि_अन्तः /* !CONFIG_NUMA_BALANCING */

स्थिर शून्य
account_entity_पङ्क्तौ_निक्षिप(संरचना cfs_rq *cfs_rq, संरचना sched_entity *se)
{
	WARN_ON_ONCE(cfs_rq != cfs_rq_सम्बन्धि(se));
	update_आरोपय_सङ्कलय(&cfs_rq->आरोपय, se->आरोपय.weight);
	यदि (entity_is_task(se)) {
		संरचना task_struct *p = task_सम्बन्धि(se);
		संरचना rq *rq = rq_सम्बन्धि(cfs_rq);

		account_numa_पङ्क्तौ_निक्षिप(rq, p);
		account_llc_पङ्क्तौ_निक्षिप(rq, p);
		list_सङ्कलय(&se->गणः_ग्रन्थिः, &rq->cfs_tasks);
	}
	cfs_rq->nr_queued++;
}

स्थिर शून्य
account_entity_पङ्क्तेः_निष्कासय(संरचना cfs_rq *cfs_rq, संरचना sched_entity *se)
{
	WARN_ON_ONCE(cfs_rq != cfs_rq_सम्बन्धि(se));
	update_आरोपय_sub(&cfs_rq->आरोपय, se->आरोपय.weight);
	यदि (entity_is_task(se)) {
		संरचना task_struct *p = task_सम्बन्धि(se);
		संरचना rq *rq = rq_सम्बन्धि(cfs_rq);

		account_numa_पङ्क्तेः_निष्कासय(rq, p);
		account_llc_पङ्क्तेः_निष्कासय(rq, p);
		list_del_init(&se->गणः_ग्रन्थिः);
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
#परिभाषय सङ्कलय_धनात्मकम्(_ptr, _val) कुरु {                           \
	typeof(_ptr) ptr = (_ptr);                              \
	__signed_scalar_typeof(*ptr) val = (_val);              \
	typeof(*ptr) res, var = READ_ONCE(*ptr);                \
								\
	res = var + val;                                        \
								\
	यदि (val < 0 && res > var)                               \
		res = 0;                                        \
								\
	WRITE_ONCE(*ptr, res);                                  \
} यावत् (0)

/*
 * Remove and clamp on negative, from a local variable.
 *
 * A variant of sub_positive(), which does not use explicit load-store
 * and is thus optimized for local variable updates.
 */
#परिभाषय lsub_धनात्मकम्(_ptr, _val) कुरु {				\
	typeof(_ptr) ptr = (_ptr);				\
	*ptr -= min_t(typeof(*ptr), *ptr, _val);		\
} यावत् (0)


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
#परिभाषय __update_sa(sa, name, delta_avg, delta_योग) कुरु {	\
	सङ्कलय_धनात्मकम्(&(sa)->name##_avg, delta_avg);		\
	सङ्कलय_धनात्मकम्(&(sa)->name##_sum, delta_योग);		\
	(sa)->name##_sum = max_t(typeof((sa)->name##_sum),	\
			       (sa)->name##_sum,		\
			       (sa)->name##_avg * PELT_MIN_DIVIDER); \
} यावत् (0)

स्थिर अन्तःस्थ शून्य
पङ्क्तौ_निक्षिप_आरोपय_avg(संरचना cfs_rq *cfs_rq, संरचना sched_entity *se)
{
	__update_sa(&cfs_rq->avg, आरोपय, se->avg.आरोपय_avg,
		    se_weight(se) * se->avg.आरोपय_योग);
}

स्थिर अन्तःस्थ शून्य
पङ्क्तेः_निष्कासय_आरोपय_avg(संरचना cfs_rq *cfs_rq, संरचना sched_entity *se)
{
	__update_sa(&cfs_rq->avg, आरोपय, -se->avg.आरोपय_avg,
		    se_weight(se) * -se->avg.आरोपय_योग);
}

स्थिर शून्य
rescale_entity(संरचना sched_entity *se, अचिह्नित दीर्घ weight, bool rel_vprot)
{
	दीर्घ old_weight = se->h_आरोपय.weight;

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
	यदि (se->rel_deadline)
		se->deadline = div64_long(se->deadline * old_weight, weight);

	यदि (rel_vprot)
		se->vprot = div64_long(se->vprot * old_weight, weight);
}

स्थिर शून्य reweight_eevdf(संरचना cfs_rq *cfs_rq, संरचना sched_entity *se,
			   अचिह्नित दीर्घ weight, bool on_rq)
{
	bool curr = cfs_rq->curr == se;
	bool rel_vprot = false;
	u64 avruntime = 0;

	यदि (se->h_आरोपय.weight == weight)
		प्रत्यागम;

	यदि (on_rq) {
		avruntime = avg_vruntime(cfs_rq);
		se->vlag = entity_lag(cfs_rq, se, avruntime);
		se->deadline -= avruntime;
		se->rel_deadline = 1;
		यदि (curr && protect_खण्डिका(se)) {
			se->vprot -= avruntime;
			rel_vprot = true;
		}

		cfs_rq->h_nr_queued--;
		यदि (!curr)
			__dequeue_entity(cfs_rq, se);
	}

	rescale_entity(se, weight, rel_vprot);

	update_आरोपय_समुच्चयः(&se->h_आरोपय, weight);

	यदि (on_rq) {
		यदि (rel_vprot)
			se->vprot += avruntime;
		se->deadline += avruntime;
		se->rel_deadline = 0;
		se->vruntime = avruntime - se->vlag;

		यदि (!curr)
			__enqueue_entity(cfs_rq, se);
		cfs_rq->h_nr_queued++;
	}
}

स्थिर शून्य reweight_entity(संरचना cfs_rq *cfs_rq, संरचना sched_entity *se,
			    अचिह्नित दीर्घ weight)
{
	यदि (se->आरोपय.weight == weight)
		प्रत्यागम;

	यदि (se->on_rq) {
		WARN_ON_ONCE(cfs_rq != cfs_rq_सम्बन्धि(se));
		update_आरोपय_sub(&cfs_rq->आरोपय, se->आरोपय.weight);
	}
	पङ्क्तेः_निष्कासय_आरोपय_avg(cfs_rq, se);

	update_आरोपय_समुच्चयः(&se->आरोपय, weight);

	कुरु {
		u32 divider = get_pelt_divider(&se->avg);
		se->avg.आरोपय_avg = div_u64(se_weight(se) * se->avg.आरोपय_योग, divider);
	} यावत् (0);

	पङ्क्तौ_निक्षिप_आरोपय_avg(cfs_rq, se);

	यदि (se->on_rq)
		update_आरोपय_सङ्कलय(&cfs_rq->आरोपय, se->आरोपय.weight);
}

/*
 * weight = NICE_0_LOAD;
 * for_each_entity_se(se)
 *   weight = __calc_prop_weight(cfs_rq_of(se), se, weight);
 */
स्थिर __always_inline
अचिह्नित दीर्घ __calc_prop_weight(संरचना cfs_rq *cfs_rq, संरचना sched_entity *se,
				 अचिह्नित दीर्घ weight)
{
	weight *= se->आरोपय.weight;
	यदि (parent_entity(se))
		weight /= cfs_rq->आरोपय.weight;
	अन्यथा
		weight /= NICE_0_आरोपय;

	प्रत्यागम max(weight, MIN_SHARES);
}

स्थिर शून्य reweight_task_fair(संरचना rq *rq, संरचना task_struct *p,
			       ध्रुव संरचना आरोपय_weight *lw)
{
	संरचना sched_entity *se = &p->se;
	अचिह्नित दीर्घ weight = NICE_0_आरोपय;

	यदि (se->on_rq)
		update_curr_fair(rq);

	reweight_entity(cfs_rq_सम्बन्धि(se), se, lw->weight);
	se->आरोपय.inv_weight = lw->inv_weight;

	यदि (!se->on_rq)
		प्रत्यागम;

	for_each_sched_entity(se)
		weight = __calc_prop_weight(cfs_rq_सम्बन्धि(se), se, weight);

	reweight_eevdf(&rq->cfs, &p->se, weight, p->se.on_rq);
}

स्थिर अन्तःस्थ पूर्णाङ्क throttled_hierarchy(संरचना cfs_rq *cfs_rq);

#यदि_परिभाषितम् CONFIG_FAIR_गणः_SCHED
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
स्थिर दीर्घ __calc_smp_shares(संरचना cfs_rq *cfs_rq, दीर्घ tg_shares, दीर्घ shares_max)
{
	संरचना task_गणः *tg = cfs_rq->tg;
	दीर्घ tg_weight, आरोपय, shares;

	आरोपय = max(scale_आरोपय_अधस्तात्(cfs_rq->आरोपय.weight), cfs_rq->avg.आरोपय_avg);

	tg_weight = atomic_long_read(&tg->आरोपय_avg);

	/* Ensure tg_weight >= load */
	tg_weight -= cfs_rq->tg_आरोपय_avg_contrib;
	tg_weight += आरोपय;

	shares = (tg_shares * आरोपय);
	यदि (tg_weight)
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
	प्रत्यागम clamp_t(दीर्घ, shares, MIN_SHARES, shares_max);
}

स्थिर पूर्णाङ्क tg_cpus(संरचना task_गणः *tg)
{
	पूर्णाङ्क nr = अङ्कः_online_cpus();

	यदि (cpusets_enabled()) {
		संरचना cgroup *cgrp = tg->css.cgroup;
		यदि (cgrp)
			nr = cpuset_अङ्कः_cpus(cgrp);
	}

	/*
	 * An empty cpuset would propagate a 0 shares_max into
	 * __calc_smp_shares(), where clamp() yields hi when hi < lo and so
	 * defeats the MIN_SHARES floor. Match tg_tasks(), which floors at 1.
	 */
	प्रत्यागम max(nr, 1);
}

स्थिर अन्तःस्थ पूर्णाङ्क tg_tasks(संरचना task_गणः *tg)
{
	प्रत्यागम max(1, atomic_long_read(&tg->runnable_avg) >> SCHED_क्षमता_सरणम्);
}

/*
 * Func: fraction(nr_tasks * tg->shares)
 *
 * Scale tg->shares by the number of tasks.
 */
स्थिर दीर्घ calc_tasks_shares(संरचना cfs_rq *cfs_rq)
{
	संरचना task_गणः *tg = cfs_rq->tg;
	पूर्णाङ्क nr = tg_tasks(tg);
	दीर्घ tg_shares = READ_ONCE(tg->shares);
	प्रत्यागम __calc_smp_shares(cfs_rq, nr * tg_shares, nr * tg_shares);
}

/*
 * Func: min(fraction(nr_cpus * tg->shares), nice -20)
 *
 * Scale tg->shares by the maximal number of CPUs; but clip the max shares at
 * nice -20, otherwise a single spinner on a 512 CPU machine would result in
 * 512*NICE_0_LOAD, which is also crazy.
 */
स्थिर दीर्घ calc_max_shares(संरचना cfs_rq *cfs_rq)
{
	संरचना task_गणः *tg = cfs_rq->tg;
	पूर्णाङ्क nr = tg_cpus(tg);
	दीर्घ tg_shares = READ_ONCE(tg->shares);
	दीर्घ max_shares = scale_आरोपय(sched_prio_गन्तव्यम्_weight[0]);
	प्रत्यागम __calc_smp_shares(cfs_rq, tg_shares * nr, max_shares);
}

/*
 * Func: fraction(nr * tg->shares); nr = min(nr_tasks, nr_cpus)
 *
 * Scales between "smp" and "max" in a natural way. No longer needs clipping
 * since there are no unnatural inflations like with "max".
 */
स्थिर दीर्घ calc_concur_shares(संरचना cfs_rq *cfs_rq)
{
	संरचना task_गणः *tg = cfs_rq->tg;
	पूर्णाङ्क nr = min(tg_tasks(tg), tg_cpus(tg));
	दीर्घ tg_shares = READ_ONCE(tg->shares);
	प्रत्यागम __calc_smp_shares(cfs_rq, nr * tg_shares, nr * tg_shares);
}

/*
 * Func: fraction(tg->shares)
 *
 * This infamously results in tiny shares when you have many CPUs.
 */
स्थिर दीर्घ calc_smp_shares(संरचना cfs_rq *cfs_rq)
{
	संरचना task_गणः *tg = cfs_rq->tg;
	दीर्घ tg_shares = READ_ONCE(tg->shares);
	प्रत्यागम __calc_smp_shares(cfs_rq, tg_shares, tg_shares);
}

/*
 * Ignore this pesky SMP stuff, use (4).
 */
स्थिर दीर्घ calc_ऊर्ध्वम्_shares(संरचना cfs_rq *cfs_rq)
{
	संरचना task_गणः *tg = cfs_rq->tg;
	प्रत्यागम READ_ONCE(tg->shares);
}

DEFINE_STATIC_CALL(calc_गणः_shares, calc_concur_shares);

शून्य __sched_cgroup_mode_update(पूर्णाङ्क बहुलकम्)
{
	दीर्घ (*func)(संरचना cfs_rq *);
	विकल्प (बहुलकम्) {
	पक्ष 0:
		func = &calc_ऊर्ध्वम्_shares;
		भङ्ग;
	पक्ष 1:
		func = &calc_smp_shares;
		भङ्ग;
	पक्ष 2:
	स्वभाव:
		func = &calc_concur_shares;
		भङ्ग;
	पक्ष 3:
		func = &calc_max_shares;
		भङ्ग;
	पक्ष 4:
		func = &calc_tasks_shares;
		भङ्ग;
	}
	static_call_update(calc_गणः_shares, func);
}

/*
 * Recomputes the group entity based on the current state of its group
 * runqueue.
 */
स्थिर शून्य update_cfs_गणः(संरचना sched_entity *se)
{
	संरचना cfs_rq *gcfs_rq = गणः_cfs_rq(se);
	दीर्घ shares;

	/*
	 * When a group becomes empty, preserve its weight. This matters for
	 * DELAY_DEQUEUE.
	 */
	यदि (!gcfs_rq || !gcfs_rq->आरोपय.weight)
		प्रत्यागम;

	shares = static_call(calc_गणः_shares)(gcfs_rq);
	reweight_entity(cfs_rq_सम्बन्धि(se), se, shares);
}

#अन्यथा /* !CONFIG_FAIR_GROUP_SCHED: */
स्थिर अन्तःस्थ शून्य update_cfs_गणः(संरचना sched_entity *se)
{
}
#यदि_अन्तः /* !CONFIG_FAIR_GROUP_SCHED */

स्थिर अन्तःस्थ शून्य cfs_rq_util_change(संरचना cfs_rq *cfs_rq, पूर्णाङ्क flags)
{
	संरचना rq *rq = rq_सम्बन्धि(cfs_rq);

	यदि (&rq->cfs == cfs_rq) {
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

स्थिर अन्तःस्थ bool आरोपय_avg_is_decayed(संरचना sched_avg *sa)
{
	यदि (sa->आरोपय_योग)
		प्रत्यागम false;

	यदि (sa->util_योग)
		प्रत्यागम false;

	यदि (sa->runnable_योग)
		प्रत्यागम false;

	/*
	 * _avg must be null when _sum are null because _avg = _sum / divider
	 * Make sure that rounding and/or propagation of PELT values never
	 * break this.
	 */
	WARN_ON_ONCE(sa->आरोपय_avg ||
		      sa->util_avg ||
		      sa->runnable_avg);

	प्रत्यागम true;
}

स्थिर अन्तःस्थ u64 cfs_rq_चरमः_update_time(संरचना cfs_rq *cfs_rq)
{
	प्रत्यागम u64_u32_आरोपय_copy(cfs_rq->avg.चरमः_update_time,
				 cfs_rq->चरमः_update_time_copy);
}
#यदि_परिभाषितम् CONFIG_FAIR_गणः_SCHED
/*
 * Because list_add_leaf_cfs_rq always places a child cfs_rq on the list
 * immediately before a parent cfs_rq, and cfs_rqs are removed from the list
 * bottom-up, we only have to test whether the cfs_rq before us on the list
 * is our child.
 * If cfs_rq is not on the list, test whether a child needs its to be added to
 * connect a branch to the tree  * (see list_add_leaf_cfs_rq() for details).
 */
स्थिर अन्तःस्थ bool child_cfs_rq_on_list(संरचना cfs_rq *cfs_rq)
{
	संरचना cfs_rq *पूर्व_cfs_rq;
	संरचना list_शीर्ष *पूर्व;
	संरचना rq *rq = rq_सम्बन्धि(cfs_rq);

	यदि (cfs_rq->on_list) {
		पूर्व = cfs_rq->पर्णम्_cfs_rq_list.पूर्व;
	} अन्यथा {
		पूर्व = rq->tmp_alone_branch;
	}

	यदि (पूर्व == &rq->पर्णम्_cfs_rq_list)
		प्रत्यागम false;

	पूर्व_cfs_rq = container_सम्बन्धि(पूर्व, संरचना cfs_rq, पर्णम्_cfs_rq_list);

	प्रत्यागम (पूर्व_cfs_rq->tg->parent == cfs_rq->tg);
}

स्थिर अन्तःस्थ bool cfs_rq_is_decayed(संरचना cfs_rq *cfs_rq)
{
	यदि (cfs_rq->आरोपय.weight)
		प्रत्यागम false;

	यदि (!आरोपय_avg_is_decayed(&cfs_rq->avg))
		प्रत्यागम false;

	यदि (child_cfs_rq_on_list(cfs_rq))
		प्रत्यागम false;

	यदि (cfs_rq->tg_आरोपय_avg_contrib)
		प्रत्यागम false;

	प्रत्यागम true;
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
स्थिर अन्तःस्थ शून्य update_tg_आरोपय_avg(संरचना cfs_rq *cfs_rq)
{
	दीर्घ dl, dr;
	u64 now;

	/*
	 * No need to update load_avg for root_task_group as it is not used.
	 */
	यदि (cfs_rq->tg == &मूलम्_task_गणः)
		प्रत्यागम;

	/* rq has been offline and doesn't contribute to the share anymore: */
	यदि (!cpu_सक्रिय(cpu_सम्बन्धि(rq_सम्बन्धि(cfs_rq))))
		प्रत्यागम;

	/*
	 * For migration heavy workloads, access to tg->load_avg can be
	 * unbound. Limit the update rate to at most once per ms.
	 */
	now = rq_clock(rq_सम्बन्धि(cfs_rq));
	यदि (now - cfs_rq->चरमः_update_tg_आरोपय_avg < NSEC_प्रत्येकम्_MSEC)
		प्रत्यागम;

	dl = cfs_rq->avg.आरोपय_avg - cfs_rq->tg_आरोपय_avg_contrib;
	dr = cfs_rq->avg.runnable_avg - cfs_rq->tg_runnable_avg_contrib;
	यदि (निरपेक्षम्(dl) > cfs_rq->tg_आरोपय_avg_contrib / 64 ||
	    निरपेक्षम्(dr) > cfs_rq->tg_runnable_avg_contrib / 64) {
		atomic_long_सङ्कलय(dl, &cfs_rq->tg->आरोपय_avg);
		atomic_long_सङ्कलय(dr, &cfs_rq->tg->runnable_avg);
		cfs_rq->tg_आरोपय_avg_contrib = cfs_rq->avg.आरोपय_avg;
		cfs_rq->tg_runnable_avg_contrib = cfs_rq->avg.runnable_avg;
		cfs_rq->चरमः_update_tg_आरोपय_avg = now;
	}
}

स्थिर अन्तःस्थ शून्य clear_tg_आरोपय_avg(संरचना cfs_rq *cfs_rq)
{
	दीर्घ dl, dr;
	u64 now;

	/*
	 * No need to update load_avg for root_task_group, as it is not used.
	 */
	यदि (cfs_rq->tg == &मूलम्_task_गणः)
		प्रत्यागम;

	now = rq_clock(rq_सम्बन्धि(cfs_rq));
	dl = 0 - cfs_rq->tg_आरोपय_avg_contrib;
	dr = 0 - cfs_rq->tg_runnable_avg_contrib;
	atomic_long_सङ्कलय(dl, &cfs_rq->tg->आरोपय_avg);
	atomic_long_सङ्कलय(dr, &cfs_rq->tg->runnable_avg);
	cfs_rq->tg_आरोपय_avg_contrib = 0;
	cfs_rq->tg_runnable_avg_contrib = 0;
	cfs_rq->चरमः_update_tg_आरोपय_avg = now;
}

/* CPU offline callback: */
स्थिर शून्य __maybe_unused clear_tg_offline_cfs_rqs(संरचना rq *rq)
{
	संरचना task_गणः *tg;

	lockdep_assert_rq_held(rq);

	/*
	 * The rq clock has already been updated in
	 * set_rq_offline(), so we should skip updating
	 * the rq clock again in unthrottle_cfs_rq().
	 */
	rq_clock_start_loop_update(rq);

	guard(rcu)();

	list_for_each_प्रविष्टिः_rcu(tg, &task_groups, list) {
		संरचना cfs_rq *cfs_rq = tg_cfs_rq(tg, cpu_सम्बन्धि(rq));

		clear_tg_आरोपय_avg(cfs_rq);
	}

	rq_clock_stop_loop_update(rq);
}

/*
 * Called within set_task_rq() right before setting a task's CPU. The
 * caller only guarantees p->pi_lock is held; no other assumptions,
 * including the state of rq->lock, should be made.
 */
शून्य समुच्चयः_task_rq_fair(संरचना sched_entity *se,
		      संरचना cfs_rq *पूर्व, संरचना cfs_rq *अग्र)
{
	u64 p_चरमः_update_time;
	u64 n_चरमः_update_time;

	यदि (!sched_feat(ATTACH_AGE_आरोपय))
		प्रत्यागम;

	/*
	 * We are supposed to update the task to "current" time, then its up to
	 * date and ready to go to new CPU/cfs_rq. But we have difficulty in
	 * getting what current time is, so simply throw away the out-of-date
	 * time. This will result in the wakee task is less decayed, but giving
	 * the wakee more load sounds not bad.
	 */
	यदि (!(se->avg.चरमः_update_time && पूर्व))
		प्रत्यागम;

	p_चरमः_update_time = cfs_rq_चरमः_update_time(पूर्व);
	n_चरमः_update_time = cfs_rq_चरमः_update_time(अग्र);

	__update_load_avg_blocked_se(p_चरमः_update_time, se);
	se->avg.चरमः_update_time = n_चरमः_update_time;
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
स्थिर अन्तःस्थ शून्य
update_tg_cfs_util(संरचना cfs_rq *cfs_rq, संरचना sched_entity *se, संरचना cfs_rq *gcfs_rq)
{
	दीर्घ delta_योग, delta_avg = gcfs_rq->avg.util_avg - se->avg.util_avg;
	u32 new_योग, divider;

	/* Nothing to update */
	यदि (!delta_avg)
		प्रत्यागम;

	/*
	 * cfs_rq->avg.period_contrib can be used for both cfs_rq and se.
	 * See ___update_load_avg() for details.
	 */
	divider = get_pelt_divider(&cfs_rq->avg);

	/* Set new sched_entity's utilization */
	se->avg.util_avg = gcfs_rq->avg.util_avg;
	new_योग = se->avg.util_avg * divider;
	delta_योग = (दीर्घ)new_योग - (दीर्घ)se->avg.util_योग;
	se->avg.util_योग = new_योग;

	/* Update parent cfs_rq utilization */
	__update_sa(&cfs_rq->avg, util, delta_avg, delta_योग);
}

स्थिर अन्तःस्थ शून्य
update_tg_cfs_runnable(संरचना cfs_rq *cfs_rq, संरचना sched_entity *se, संरचना cfs_rq *gcfs_rq)
{
	दीर्घ delta_योग, delta_avg = gcfs_rq->avg.runnable_avg - se->avg.runnable_avg;
	u64 new_योग;
	u32 divider;

	/* Nothing to update */
	यदि (!delta_avg)
		प्रत्यागम;

	/*
	 * cfs_rq->avg.period_contrib can be used for both cfs_rq and se.
	 * See ___update_load_avg() for details.
	 */
	divider = get_pelt_divider(&cfs_rq->avg);

	/* Set new sched_entity's runnable */
	se->avg.runnable_avg = gcfs_rq->avg.runnable_avg;
	new_योग = (u64)se->avg.runnable_avg * divider;
	delta_योग = (दीर्घ)new_योग - (दीर्घ)se->avg.runnable_योग;
	se->avg.runnable_योग = new_योग;

	/* Update parent cfs_rq runnable */
	__update_sa(&cfs_rq->avg, runnable, delta_avg, delta_योग);
}

स्थिर अन्तःस्थ शून्य
update_tg_cfs_आरोपय(संरचना cfs_rq *cfs_rq, संरचना sched_entity *se, संरचना cfs_rq *gcfs_rq)
{
	दीर्घ delta_avg, running_योग, runnable_योग = gcfs_rq->prop_runnable_योग;
	अचिह्नित दीर्घ आरोपय_avg;
	u64 आरोपय_योग = 0;
	s64 delta_योग;
	u32 divider;

	यदि (!runnable_योग)
		प्रत्यागम;

	gcfs_rq->prop_runnable_योग = 0;

	/*
	 * cfs_rq->avg.period_contrib can be used for both cfs_rq and se.
	 * See ___update_load_avg() for details.
	 */
	divider = get_pelt_divider(&cfs_rq->avg);

	यदि (runnable_योग >= 0) {
		/*
		 * Add runnable; clip at LOAD_AVG_MAX. Reflects that until
		 * the CPU is saturated running == runnable.
		 */
		runnable_योग += se->avg.आरोपय_योग;
		runnable_योग = min_t(दीर्घ, runnable_योग, divider);
	} अन्यथा {
		/*
		 * Estimate the new unweighted runnable_sum of the gcfs_rq by
		 * assuming all tasks are equally runnable.
		 */
		यदि (scale_आरोपय_अधस्तात्(gcfs_rq->आरोपय.weight)) {
			आरोपय_योग = div_u64(gcfs_rq->avg.आरोपय_योग,
				scale_आरोपय_अधस्तात्(gcfs_rq->आरोपय.weight));
		}

		/* But make sure to not inflate se's runnable */
		runnable_योग = min(se->avg.आरोपय_योग, आरोपय_योग);
	}

	/*
	 * runnable_sum can't be lower than running_sum
	 * Rescale running sum to be in the same range as runnable sum
	 * running_sum is in [0 : LOAD_AVG_MAX <<  SCHED_CAPACITY_SHIFT]
	 * runnable_sum is in [0 : LOAD_AVG_MAX]
	 */
	running_योग = se->avg.util_योग >> SCHED_क्षमता_सरणम्;
	runnable_योग = max(runnable_योग, running_योग);

	आरोपय_योग = se_weight(se) * runnable_योग;
	आरोपय_avg = div_u64(आरोपय_योग, divider);

	delta_avg = आरोपय_avg - se->avg.आरोपय_avg;
	यदि (!delta_avg)
		प्रत्यागम;

	delta_योग = आरोपय_योग - (s64)se_weight(se) * se->avg.आरोपय_योग;

	se->avg.आरोपय_योग = runnable_योग;
	se->avg.आरोपय_avg = आरोपय_avg;
	__update_sa(&cfs_rq->avg, आरोपय, delta_avg, delta_योग);
}

स्थिर अन्तःस्थ शून्य सङ्कलय_tg_cfs_propagate(संरचना cfs_rq *cfs_rq, दीर्घ runnable_योग)
{
	cfs_rq->propagate = 1;
	cfs_rq->prop_runnable_योग += runnable_योग;
}

/* Update task and its cfs_rq load average */
स्थिर अन्तःस्थ पूर्णाङ्क propagate_entity_आरोपय_avg(संरचना sched_entity *se)
{
	संरचना cfs_rq *cfs_rq, *gcfs_rq;

	यदि (entity_is_task(se))
		प्रत्यागम 0;

	gcfs_rq = गणः_cfs_rq(se);
	यदि (!gcfs_rq->propagate)
		प्रत्यागम 0;

	gcfs_rq->propagate = 0;

	cfs_rq = cfs_rq_सम्बन्धि(se);

	सङ्कलय_tg_cfs_propagate(cfs_rq, gcfs_rq->prop_runnable_योग);

	update_tg_cfs_util(cfs_rq, se, gcfs_rq);
	update_tg_cfs_runnable(cfs_rq, se, gcfs_rq);
	update_tg_cfs_आरोपय(cfs_rq, se, gcfs_rq);

	trace_pelt_cfs_tp(cfs_rq);
	trace_pelt_se_tp(se);

	प्रत्यागम 1;
}

/*
 * Check if we need to update the load and the utilization of a blocked
 * group_entity:
 */
स्थिर अन्तःस्थ bool skip_blocked_update(संरचना sched_entity *se)
{
	संरचना cfs_rq *gcfs_rq = गणः_cfs_rq(se);

	/*
	 * If sched_entity still have not zero load or utilization, we have to
	 * decay it:
	 */
	यदि (se->avg.आरोपय_avg || se->avg.util_avg)
		प्रत्यागम false;

	/*
	 * If there is a pending propagation, we have to update the load and
	 * the utilization of the sched_entity:
	 */
	यदि (gcfs_rq->propagate)
		प्रत्यागम false;

	/*
	 * Otherwise, the load and the utilization of the sched_entity is
	 * already zero and there is no pending propagation, so it will be a
	 * waste of time to try to decay it:
	 */
	प्रत्यागम true;
}

#अन्यथा /* !CONFIG_FAIR_GROUP_SCHED: */

स्थिर अन्तःस्थ शून्य update_tg_आरोपय_avg(संरचना cfs_rq *cfs_rq) {}

स्थिर अन्तःस्थ शून्य clear_tg_offline_cfs_rqs(संरचना rq *rq) {}

स्थिर अन्तःस्थ पूर्णाङ्क propagate_entity_आरोपय_avg(संरचना sched_entity *se)
{
	प्रत्यागम 0;
}

स्थिर अन्तःस्थ शून्य सङ्कलय_tg_cfs_propagate(संरचना cfs_rq *cfs_rq, दीर्घ runnable_योग) {}

#यदि_अन्तः /* !CONFIG_FAIR_GROUP_SCHED */

#यदि_परिभाषितम् CONFIG_NO_HZ_COMMON
स्थिर अन्तःस्थ शून्य migrate_se_pelt_lag(संरचना sched_entity *se)
{
	u64 throttled = 0, now, lut;
	संरचना cfs_rq *cfs_rq;
	संरचना rq *rq;
	bool is_idle;

	यदि (आरोपय_avg_is_decayed(&se->avg))
		प्रत्यागम;

	cfs_rq = cfs_rq_सम्बन्धि(se);
	rq = rq_सम्बन्धि(cfs_rq);

	rcu_read_तालकम्();
	is_idle = is_idle_task(rcu_dereference_all(rq->curr));
	rcu_read_unlock();

	/*
	 * The lag estimation comes with a cost we don't want to pay all the
	 * time. Hence, limiting to the case where the source CPU is idle and
	 * we know we are at the greatest risk to have an outdated clock.
	 */
	यदि (!is_idle)
		प्रत्यागम;

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

#यदि_परिभाषितम् CONFIG_CFS_BANDWIDTH
	throttled = u64_u32_आरोपय(cfs_rq->throttled_pelt_idle);
	/* The clock has been stopped for throttling */
	यदि (throttled == U64_MAX)
		प्रत्यागम;
#यदि_अन्तः
	now = u64_u32_आरोपय(rq->clock_pelt_idle);
	/*
	 * Paired with _update_idle_rq_clock_pelt(). It ensures at the worst case
	 * is observed the old clock_pelt_idle value and the new clock_idle,
	 * which lead to an underestimation. The opposite would lead to an
	 * overestimation.
	 */
	smp_rmb();
	lut = cfs_rq_चरमः_update_time(cfs_rq);

	now -= throttled;
	यदि (now < lut)
		/*
		 * cfs_rq->avg.last_update_time is more recent than our
		 * estimation, let's use it.
		 */
		now = lut;
	अन्यथा
		now += sched_clock_cpu(cpu_सम्बन्धि(rq)) - u64_u32_आरोपय(rq->clock_idle);

	__update_load_avg_blocked_se(now, se);
}
#अन्यथा /* !CONFIG_NO_HZ_COMMON: */
स्थिर शून्य migrate_se_pelt_lag(संरचना sched_entity *se) {}
#यदि_अन्तः /* !CONFIG_NO_HZ_COMMON */

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
स्थिर अन्तःस्थ पूर्णाङ्क
update_cfs_rq_आरोपय_avg(u64 now, संरचना cfs_rq *cfs_rq)
{
	अचिह्नित दीर्घ removed_आरोपय = 0, removed_util = 0, removed_runnable = 0;
	संरचना sched_avg *sa = &cfs_rq->avg;
	पूर्णाङ्क decayed = 0;

	यदि (cfs_rq->removed.nr) {
		अचिह्नित दीर्घ r;
		u32 divider = get_pelt_divider(&cfs_rq->avg);

		raw_spin_तालकम्(&cfs_rq->removed.तालकम्);
		swap(cfs_rq->removed.util_avg, removed_util);
		swap(cfs_rq->removed.आरोपय_avg, removed_आरोपय);
		swap(cfs_rq->removed.runnable_avg, removed_runnable);
		cfs_rq->removed.nr = 0;
		raw_spin_unlock(&cfs_rq->removed.तालकम्);

		r = removed_आरोपय;
		__update_sa(sa, आरोपय, -r, -r*divider);

		r = removed_util;
		__update_sa(sa, util, -r, -r*divider);

		r = removed_runnable;
		__update_sa(sa, runnable, -r, -r*divider);

		/*
		 * removed_runnable is the unweighted version of removed_load so we
		 * can use it to estimate removed_load_sum.
		 */
		सङ्कलय_tg_cfs_propagate(cfs_rq,
			-(दीर्घ)(removed_runnable * divider) >> SCHED_क्षमता_सरणम्);

		decayed = 1;
	}

	decayed |= __update_load_avg_cfs_rq(now, cfs_rq);
	u64_u32_store_copy(sa->चरमः_update_time,
			   cfs_rq->चरमः_update_time_copy,
			   sa->चरमः_update_time);
	प्रत्यागम decayed;
}

/**
 * attach_entity_load_avg - attach this entity to its cfs_rq load avg
 * @cfs_rq: cfs_rq to attach to
 * @se: sched_entity to attach
 *
 * Must call update_cfs_rq_load_avg() before this, since we rely on
 * cfs_rq->avg.last_update_time being current.
 */
स्थिर शून्य attach_entity_आरोपय_avg(संरचना cfs_rq *cfs_rq, संरचना sched_entity *se)
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
	se->avg.चरमः_update_time = cfs_rq->avg.चरमः_update_time;
	se->avg.period_contrib = cfs_rq->avg.period_contrib;

	/*
	 * Hell(o) Nasty stuff.. we need to recompute _sum based on the new
	 * period_contrib. This isn't strictly correct, but since we're
	 * entirely outside of the PELT hierarchy, nobody cares if we truncate
	 * _sum a little.
	 */
	se->avg.util_योग = se->avg.util_avg * divider;

	se->avg.runnable_योग = se->avg.runnable_avg * divider;

	se->avg.आरोपय_योग = se->avg.आरोपय_avg * divider;
	यदि (se_weight(se) < se->avg.आरोपय_योग)
		se->avg.आरोपय_योग = div_u64(se->avg.आरोपय_योग, se_weight(se));
	अन्यथा
		se->avg.आरोपय_योग = 1;

	पङ्क्तौ_निक्षिप_आरोपय_avg(cfs_rq, se);
	cfs_rq->avg.util_avg += se->avg.util_avg;
	cfs_rq->avg.util_योग += se->avg.util_योग;
	cfs_rq->avg.runnable_avg += se->avg.runnable_avg;
	cfs_rq->avg.runnable_योग += se->avg.runnable_योग;

	सङ्कलय_tg_cfs_propagate(cfs_rq, se->avg.आरोपय_योग);

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
स्थिर शून्य detach_entity_आरोपय_avg(संरचना cfs_rq *cfs_rq, संरचना sched_entity *se)
{
	पङ्क्तेः_निष्कासय_आरोपय_avg(cfs_rq, se);
	__update_sa(&cfs_rq->avg, util, -se->avg.util_avg, -se->avg.util_योग);
	__update_sa(&cfs_rq->avg, runnable, -se->avg.runnable_avg, -se->avg.runnable_योग);

	सङ्कलय_tg_cfs_propagate(cfs_rq, -se->avg.आरोपय_योग);

	cfs_rq_util_change(cfs_rq, 0);

	trace_pelt_cfs_tp(cfs_rq);
}

#परिभाषय UTIL_EST_MARGIN (SCHED_क्षमता_SCALE / 100)

स्थिर अन्तःस्थ शून्य util_est_update(संरचना sched_entity *se)
{
	अचिह्नित पूर्णाङ्क ewma, dequeued, चरमः_ewma_diff;

	यदि (!sched_feat(UTIL_EST))
		प्रत्यागम;

	/* Get current estimate of utilization */
	ewma = READ_ONCE(se->avg.util_est);

	/*
	 * If the PELT values haven't changed since enqueue time,
	 * skip the util_est update.
	 */
	यदि (ewma & UTIL_AVG_UNCHANGED)
		प्रत्यागम;

	/* Get utilization at dequeue */
	dequeued = READ_ONCE(se->avg.util_avg);

	/*
	 * Reset EWMA on utilization increases, the moving average is used only
	 * to smooth utilization decreases.
	 */
	यदि (ewma <= dequeued) {
		ewma = dequeued;
		गच्छ समाप्तम्;
	}

	/*
	 * Skip update of task's estimated utilization when its members are
	 * already ~1% close to its last activation value.
	 */
	चरमः_ewma_diff = ewma - dequeued;
	यदि (चरमः_ewma_diff < UTIL_EST_MARGIN)
		गच्छ समाप्तम्;

	/*
	 * To avoid underestimate of task utilization, skip updates of EWMA if
	 * we cannot grant that thread got all CPU time it wanted.
	 */
	यदि ((dequeued + UTIL_EST_MARGIN) < READ_ONCE(se->avg.runnable_avg))
		गच्छ समाप्तम्;

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
	ewma <<= UTIL_EST_WEIGHT_सरणम्;
	ewma  -= चरमः_ewma_diff;
	ewma >>= UTIL_EST_WEIGHT_सरणम्;
समाप्तम्:
	ewma |= UTIL_AVG_UNCHANGED;
	WRITE_ONCE(se->avg.util_est, ewma);

	trace_sched_util_est_se_tp(se);
}

/*
 * Optional action to be done while updating the load average
 */
#परिभाषय UPDATE_TG	0x01
#परिभाषय SKIP_AGE_आरोपय	0x02
#परिभाषय DO_ATTACH	0x04
#परिभाषय DO_DETACH	0x08
#परिभाषय UPDATE_UTIL_EST	0x10

/* Update task and its cfs_rq load average */
स्थिर अन्तःस्थ शून्य update_आरोपय_avg(संरचना cfs_rq *cfs_rq, संरचना sched_entity *se, पूर्णाङ्क flags)
{
	u64 now = cfs_rq_clock_pelt(cfs_rq);
	पूर्णाङ्क decayed;

	/*
	 * Track task load average for carrying it to new CPU after migrated, and
	 * track group sched_entity load average for task_h_load calculation in migration
	 */
	यदि (se->avg.चरमः_update_time && !(flags & SKIP_AGE_आरोपय))
		__update_load_avg_se(now, cfs_rq, se);

	decayed  = update_cfs_rq_आरोपय_avg(now, cfs_rq);
	decayed |= propagate_entity_आरोपय_avg(se);

	यदि (!se->avg.चरमः_update_time && (flags & DO_ATTACH)) {

		/*
		 * DO_ATTACH means we're here from enqueue_entity().
		 * !last_update_time means we've passed through
		 * migrate_task_rq_fair() indicating we migrated.
		 *
		 * IOW we're enqueueing a task on a new CPU.
		 */
		attach_entity_आरोपय_avg(cfs_rq, se);
		update_tg_आरोपय_avg(cfs_rq);

	} अन्यथा यदि (flags & DO_DETACH) {
		/*
		 * DO_DETACH means we're here from dequeue_entity()
		 * and we are migrating task out of the CPU.
		 */
		detach_entity_आरोपय_avg(cfs_rq, se);
		update_tg_आरोपय_avg(cfs_rq);
	} अन्यथा यदि (decayed) {
		cfs_rq_util_change(cfs_rq, 0);

		यदि (flags & UPDATE_TG)
			update_tg_आरोपय_avg(cfs_rq);
	}

	यदि (flags & UPDATE_UTIL_EST)
		util_est_update(se);
}

/*
 * Synchronize entity load avg of dequeued entity without locking
 * the previous rq.
 */
स्थिर शून्य sync_entity_आरोपय_avg(संरचना sched_entity *se)
{
	संरचना cfs_rq *cfs_rq = cfs_rq_सम्बन्धि(se);
	u64 चरमः_update_time;

	चरमः_update_time = cfs_rq_चरमः_update_time(cfs_rq);
	__update_load_avg_blocked_se(चरमः_update_time, se);
}

/*
 * Task first catches up with cfs_rq, and then subtract
 * itself from the cfs_rq (task must be off the queue now).
 */
स्थिर शून्य remove_entity_आरोपय_avg(संरचना sched_entity *se)
{
	संरचना cfs_rq *cfs_rq = cfs_rq_सम्बन्धि(se);
	अचिह्नित दीर्घ flags;

	/*
	 * tasks cannot exit without having gone through wake_up_new_task() ->
	 * enqueue_task_fair() which will have added things to the cfs_rq,
	 * so we can remove unconditionally.
	 */

	sync_entity_आरोपय_avg(se);

	raw_spin_तालकम्_irqsave(&cfs_rq->removed.तालकम्, flags);
	++cfs_rq->removed.nr;
	cfs_rq->removed.util_avg	+= se->avg.util_avg;
	cfs_rq->removed.आरोपय_avg	+= se->avg.आरोपय_avg;
	cfs_rq->removed.runnable_avg	+= se->avg.runnable_avg;
	raw_spin_unlock_irqrestore(&cfs_rq->removed.तालकम्, flags);
}

स्थिर अन्तःस्थ अचिह्नित दीर्घ cfs_rq_runnable_avg(संरचना cfs_rq *cfs_rq)
{
	प्रत्यागम cfs_rq->avg.runnable_avg;
}

स्थिर अन्तःस्थ अचिह्नित दीर्घ cfs_rq_आरोपय_avg(संरचना cfs_rq *cfs_rq)
{
	प्रत्यागम cfs_rq->avg.आरोपय_avg;
}

स्थिर पूर्णाङ्क sched_balance_newidle(संरचना rq *this_rq, संरचना rq_flags *rf)
	__must_hold(__rq_lockp(this_rq));

स्थिर अन्तःस्थ अचिह्नित दीर्घ task_util(संरचना task_struct *p)
{
	प्रत्यागम READ_ONCE(p->se.avg.util_avg);
}

स्थिर अन्तःस्थ अचिह्नित दीर्घ _task_util_est(संरचना task_struct *p)
{
	प्रत्यागम READ_ONCE(p->se.avg.util_est) & ~UTIL_AVG_UNCHANGED;
}

स्थिर अन्तःस्थ अचिह्नित दीर्घ task_util_est(संरचना task_struct *p)
{
	प्रत्यागम max(task_util(p), _task_util_est(p));
}

स्थिर अन्तःस्थ शून्य util_est_पङ्क्तौ_निक्षिप(संरचना cfs_rq *cfs_rq,
				    संरचना task_struct *p)
{
	अचिह्नित पूर्णाङ्क enqueued;

	यदि (!sched_feat(UTIL_EST))
		प्रत्यागम;

	/* Update root cfs_rq's estimated utilization */
	enqueued  = cfs_rq->avg.util_est;
	enqueued += _task_util_est(p);
	WRITE_ONCE(cfs_rq->avg.util_est, enqueued);

	trace_sched_util_est_cfs_tp(cfs_rq);
}

स्थिर अन्तःस्थ शून्य util_est_पङ्क्तेः_निष्कासय(संरचना cfs_rq *cfs_rq,
				    संरचना task_struct *p)
{
	अचिह्नित पूर्णाङ्क enqueued;

	यदि (!sched_feat(UTIL_EST))
		प्रत्यागम;

	/* Update root cfs_rq's estimated utilization */
	enqueued  = cfs_rq->avg.util_est;
	enqueued -= min_t(अचिह्नित पूर्णाङ्क, enqueued, _task_util_est(p));
	WRITE_ONCE(cfs_rq->avg.util_est, enqueued);

	trace_sched_util_est_cfs_tp(cfs_rq);
}

स्थिर अन्तःस्थ अचिह्नित दीर्घ get_actual_cpu_क्षमता(पूर्णाङ्क cpu)
{
	अचिह्नित दीर्घ क्षमता = arch_scale_cpu_क्षमता(cpu);

	क्षमता -= max(hw_आरोपय_avg(cpu_rq(cpu)), cpufreq_get_pressure(cpu));

	प्रत्यागम क्षमता;
}

स्थिर अन्तःस्थ पूर्णाङ्क util_माति_cpu(अचिह्नित दीर्घ util,
				अचिह्नित दीर्घ uclamp_min,
				अचिह्नित दीर्घ uclamp_max,
				पूर्णाङ्क cpu)
{
	अचिह्नित दीर्घ क्षमता = क्षमता_सम्बन्धि(cpu);
	अचिह्नित दीर्घ क्षमता_orig;
	bool माति, uclamp_max_माति;

	/*
	 * Check if the real util fits without any uclamp boost/cap applied.
	 */
	माति = माति_क्षमता(util, क्षमता);

	यदि (!uclamp_is_प्रयुक्तम्())
		प्रत्यागम माति;

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
	क्षमता_orig = arch_scale_cpu_क्षमता(cpu);

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
	uclamp_max_माति = (क्षमता_orig == SCHED_क्षमता_SCALE) && (uclamp_max == SCHED_क्षमता_SCALE);
	uclamp_max_माति = !uclamp_max_माति && (uclamp_max <= क्षमता_orig);
	माति = माति || uclamp_max_माति;

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
	यदि (माति && (util < uclamp_min) &&
	    (uclamp_min > get_actual_cpu_क्षमता(cpu)))
		प्रत्यागम -1;

	प्रत्यागम माति;
}

स्थिर अन्तःस्थ पूर्णाङ्क task_माति_cpu(संरचना task_struct *p, पूर्णाङ्क cpu)
{
	अचिह्नित दीर्घ uclamp_min = uclamp_eff_मानम्(p, UCLAMP_MIN);
	अचिह्नित दीर्घ uclamp_max = uclamp_eff_मानम्(p, UCLAMP_MAX);
	अचिह्नित दीर्घ util = task_util_est(p);
	/*
	 * Return true only if the cpu fully fits the task requirements, which
	 * include the utilization but also the performance hints.
	 */
	प्रत्यागम (util_माति_cpu(util, uclamp_min, uclamp_max, cpu) > 0);
}

स्थिर अन्तःस्थ शून्य update_misfit_स्थितिः(संरचना task_struct *p, संरचना rq *rq)
{
	पूर्णाङ्क cpu = cpu_सम्बन्धि(rq);

	यदि (!sched_asym_cpucap_सक्रिय())
		प्रत्यागम;

	/*
	 * Affinity allows us to go somewhere higher?  Or are we on biggest
	 * available CPU already? Or do we fit into this CPU ?
	 */
	यदि (!p || (p->nr_cpus_allowed == 1) ||
	    (arch_scale_cpu_क्षमता(cpu) == p->max_allowed_क्षमता) ||
	    task_माति_cpu(p, cpu)) {

		rq->misfit_task_आरोपय = 0;
		प्रत्यागम;
	}

	/*
	 * Make sure that misfit_task_load will not be null even if
	 * task_h_load() returns 0.
	 */
	rq->misfit_task_आरोपय = max_t(अचिह्नित दीर्घ, task_h_आरोपय(p), 1);
}

शून्य __setparam_fair(संरचना task_struct *p, ध्रुव संरचना sched_attr *attr)
{
	संरचना sched_entity *se = &p->se;

	p->static_prio = NICE_गन्तव्यम्_PRIO(attr->sched_nice);
	यदि (attr->sched_runtime) {
		se->custom_खण्डिका = 1;
		se->खण्डिका = clamp_t(u64, attr->sched_runtime,
				      NSEC_प्रत्येकम्_MSEC/10,   /* HZ=1000 * 10 */
				      NSEC_प्रत्येकम्_MSEC*100); /* HZ=100  / 10 */
	} अन्यथा {
		se->custom_खण्डिका = 0;
		se->खण्डिका = sysctl_sched_आधारः_खण्डिका;
	}
}

स्थिर शून्य
place_entity(संरचना cfs_rq *cfs_rq, संरचना sched_entity *se, पूर्णाङ्क flags)
{
	u64 vslice, vruntime = avg_vruntime(cfs_rq);
	अचिह्नित पूर्णाङ्क nr_queued = cfs_rq->h_nr_queued;
	bool update_zero = false;
	s64 lag = 0;

	यदि (!se->custom_खण्डिका)
		se->खण्डिका = sysctl_sched_आधारः_खण्डिका;
	vslice = calc_delta_fair(se->खण्डिका, se);

	यदि (flags & पङ्क्तौ_निक्षिप_QUEUED)
		nr_queued -= 1;

	/*
	 * Due to how V is constructed as the weighted average of entities,
	 * adding tasks with positive lag, or removing tasks with negative lag
	 * will move 'time' backwards, this can screw around with the lag of
	 * other tasks.
	 *
	 * EEVDF: placement strategy #1 / #2
	 */
	यदि (sched_feat(PLACE_LAG) && nr_queued && se->vlag) {
		संरचना sched_entity *curr = cfs_rq->curr;
		दीर्घ आरोपय, weight;

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
		आरोपय = cfs_rq->योग_weight;
		यदि (curr && curr->on_rq)
			आरोपय += avg_vruntime_weight(cfs_rq, curr->h_आरोपय.weight);
		यदि (WARN_ON_ONCE(!आरोपय))
			आरोपय = 1;

		weight = avg_vruntime_weight(cfs_rq, se->h_आरोपय.weight);

		/*
		 * Rewrite, because the product (W + w_i)*vl'_i might
		 * overflow s64:
		 *
		 *   vl_i = (W + w_i)*vl'_i / W
		 *        = (W*vl'_i + w_i*vl'_i) / W
		 *        = vl'_i + w_i*vl'_i/W
		 */
		lag += div64_long(lag * weight, आरोपय);

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
		यदि (weight > आरोपय)
			update_zero = true;
	}

	se->vruntime = vruntime - lag;

	यदि (update_zero)
		update_zero_vruntime(cfs_rq, -lag);

	यदि (sched_feat(PLACE_REL_DEADLINE) && se->rel_deadline) {
		se->deadline += se->vruntime;
		se->rel_deadline = 0;
		प्रत्यागम;
	}

	/*
	 * When joining the competition; the existing tasks will be,
	 * on average, halfway through their slice, as such start tasks
	 * off with half a slice to ease into the competition.
	 */
	यदि (sched_feat(PLACE_DEADLINE_INITIAL) && (flags & पङ्क्तौ_निक्षिप_INITIAL))
		vslice /= 2;

	/*
	 * EEVDF: vd_i = ve_i + r_i/w_i
	 */
	se->deadline = se->vruntime + vslice;
}

स्थिर शून्य check_पङ्क्तौ_निक्षिप_throttle(संरचना cfs_rq *cfs_rq);
स्थिर अन्तःस्थ पूर्णाङ्क cfs_rq_throttled(संरचना cfs_rq *cfs_rq);

स्थिर शून्य
पङ्क्तौ_निक्षिप_entity(संरचना cfs_rq *cfs_rq, संरचना sched_entity *se, पूर्णाङ्क flags)
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
	update_आरोपय_avg(cfs_rq, se, UPDATE_TG | DO_ATTACH);
	se_update_runnable(se);
	/*
	 * XXX update_load_avg() above will have attached us to the pelt sum;
	 * but update_cfs_group() here will re-adjust the weight and have to
	 * undo/redo all that. Seems wasteful.
	 */
	update_cfs_गणः(se);

	account_entity_पङ्क्तौ_निक्षिप(cfs_rq, se);

	/* Entity has migrated, no longer consider this task hot */
	यदि (flags & पङ्क्तौ_निक्षिप_MIGRATED)
		se->exec_start = 0;

	check_schedstat_required();
	update_stats_पङ्क्तौ_निक्षिप_fair(cfs_rq, se, flags);
	se->on_rq = 1;

	यदि (cfs_rq->nr_queued == 1) {
		check_पङ्क्तौ_निक्षिप_throttle(cfs_rq);
		list_सङ्कलय_पर्णम्_cfs_rq(cfs_rq);
#यदि_परिभाषितम् CONFIG_CFS_BANDWIDTH
		यदि (cfs_rq->pelt_clock_throttled) {
			संरचना rq *rq = rq_सम्बन्धि(cfs_rq);

			cfs_rq->throttled_clock_pelt_time += rq_clock_pelt(rq) -
				cfs_rq->throttled_clock_pelt;
			cfs_rq->pelt_clock_throttled = 0;
		}
#यदि_अन्तः
	}
}

स्थिर शून्य समुच्चयः_अग्र_buddy(संरचना cfs_rq *cfs_rq, संरचना sched_entity *se)
{
	यदि (WARN_ON_ONCE(!se->on_rq || se->sched_delayed))
		प्रत्यागम;
	यदि (se_is_idle(se))
		प्रत्यागम;
	cfs_rq->अग्र = se;
}

स्थिर शून्य clear_buddies(संरचना cfs_rq *cfs_rq, संरचना sched_entity *se)
{
	यदि (cfs_rq->अग्र == se)
		cfs_rq->अग्र = रिक्त;
}

स्थिर __always_inline शून्य return_cfs_rq_runtime(संरचना cfs_rq *cfs_rq);

स्थिर शून्य समुच्चयः_delayed(संरचना sched_entity *se)
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
	यदि (!entity_is_task(se)) {
		se->sched_delayed = 1;
		प्रत्यागम;
	}

	/*
	 * Drop a task leaving the runnable set.
	 * Needs to be called before sched_delayed is set.
	 * clear_delayed() mirrors this after clearing the flag.
	 */
	pref_llc_running_dec(rq_सम्बन्धि(cfs_rq_सम्बन्धि(se)), task_सम्बन्धि(se));
	se->sched_delayed = 1;

	for_each_sched_entity(se) {
		संरचना cfs_rq *cfs_rq = cfs_rq_सम्बन्धि(se);

		cfs_rq->h_nr_runnable--;
	}
}

स्थिर शून्य clear_delayed(संरचना sched_entity *se)
{
	se->sched_delayed = 0;

	/*
	 * Delayed se of cfs_rq have no tasks queued on them.
	 * Do not adjust h_nr_runnable since a dequeue has
	 * already accounted for it or an enqueue of a task
	 * below it will account for it in enqueue_task_fair().
	 */
	यदि (!entity_is_task(se))
		प्रत्यागम;

	/*
	 * Re-add on wake, after sched_delayed is cleared. On a final delayed
	 * dequeue account_llc_dequeue() already cleared pref_llc_queued, so
	 * this does nothing.
	 */
	pref_llc_running_inc(rq_सम्बन्धि(cfs_rq_सम्बन्धि(se)), task_सम्बन्धि(se));

	for_each_sched_entity(se) {
		संरचना cfs_rq *cfs_rq = cfs_rq_सम्बन्धि(se);

		cfs_rq->h_nr_runnable++;
	}
}

स्थिर शून्य
पङ्क्तेः_निष्कासय_entity(संरचना cfs_rq *cfs_rq, संरचना sched_entity *se, पूर्णाङ्क flags)
{
	पूर्णाङ्क action = UPDATE_TG;

	यदि (entity_is_task(se)) {
		यदि (task_on_rq_migrating(task_सम्बन्धि(se)))
			action |= DO_DETACH;

		यदि ((flags & पङ्क्तेः_निष्कासय_SLEEP) && !(flags & पङ्क्तेः_निष्कासय_DELAYED))
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
	update_आरोपय_avg(cfs_rq, se, action);
	se_update_runnable(se);

	update_stats_पङ्क्तेः_निष्कासय_fair(cfs_rq, se, flags);

	se->on_rq = 0;
	account_entity_पङ्क्तेः_निष्कासय(cfs_rq, se);

	/* return excess runtime on last dequeue */
	return_cfs_rq_runtime(cfs_rq);

	update_cfs_गणः(se);

	यदि (cfs_rq->nr_queued == 0) {
		update_idle_cfs_rq_clock_pelt(cfs_rq);
#यदि_परिभाषितम् CONFIG_CFS_BANDWIDTH
		यदि (throttled_hierarchy(cfs_rq)) {
			संरचना rq *rq = rq_सम्बन्धि(cfs_rq);

			list_del_पर्णम्_cfs_rq(cfs_rq);
			cfs_rq->throttled_clock_pelt = rq_clock_pelt(rq);
			cfs_rq->pelt_clock_throttled = 1;
		}
#यदि_अन्तः
	}
}

स्थिर शून्य
समुच्चयः_अग्र_entity(संरचना cfs_rq *cfs_rq, संरचना sched_entity *se)
{
	/* 'current' is not kept within the tree. */
	यदि (se->on_rq) {
		/*
		 * Any task has to be enqueued before it get to execute on
		 * a CPU. So account for the time it spent waiting on the
		 * runqueue.
		 */
		update_stats_wait_अन्तः_fair(cfs_rq, se);
		update_आरोपय_avg(cfs_rq, se, UPDATE_TG);
	}

	update_stats_curr_start(cfs_rq, se);
	WARN_ON_ONCE(cfs_rq->h_curr);
	cfs_rq->h_curr = se;

	/*
	 * Track our maximum slice length, if the CPU's load is at
	 * least twice that of our own weight (i.e. don't track it
	 * when there are only lesser-weight tasks around):
	 */
	यदि (schedstat_enabled() &&
	    rq_सम्बन्धि(cfs_rq)->cfs.आरोपय.weight >= 2*se->आरोपय.weight) {
		संरचना sched_statistics *stats;

		stats = __schedstats_from_se(se);
		__schedstat_set(stats->खण्डिका_max,
				max((u64)stats->खण्डिका_max,
				    se->योग_exec_runtime - se->पूर्व_योग_exec_runtime));
	}

	se->पूर्व_योग_exec_runtime = se->योग_exec_runtime;
}

स्थिर bool __dequeue_task(संरचना rq *rq, संरचना task_struct *p, पूर्णाङ्क flags);

स्थिर संरचना sched_entity *
pick_अग्र_entity(संरचना rq *rq, bool protect)
{
	संरचना cfs_rq *cfs_rq = &rq->cfs;
	संरचना sched_entity *se;

	se = pick_eevdf(cfs_rq, protect);
	यदि (se->sched_delayed) {
		__dequeue_task(rq, task_सम्बन्धि(se), पङ्क्तेः_निष्कासय_SLEEP | पङ्क्तेः_निष्कासय_DELAYED);
		/*
		 * Must not reference @se again, see __block_task().
		 */
		प्रत्यागम रिक्त;
	}
	प्रत्यागम se;
}

स्थिर शून्य put_पूर्व_entity(संरचना cfs_rq *cfs_rq, संरचना sched_entity *पूर्व)
{
	/*
	 * If still on the runqueue then deactivate_task()
	 * was not called and update_curr() has to be done:
	 */
	यदि (पूर्व->on_rq)
		update_curr(cfs_rq);

	यदि (पूर्व->on_rq) {
		update_stats_wait_start_fair(cfs_rq, पूर्व);
		/* in !on_rq case, update occurred at dequeue */
		update_आरोपय_avg(cfs_rq, पूर्व, 0);
	}
	WARN_ON_ONCE(cfs_rq->h_curr != पूर्व);
	cfs_rq->h_curr = रिक्त;
}

स्थिर शून्य
entity_tick(संरचना cfs_rq *cfs_rq, संरचना sched_entity *curr, पूर्णाङ्क queued)
{
	/*
	 * Update run-time statistics of the 'current'.
	 */
	update_curr(cfs_rq);

	/*
	 * Ensure that runnable average is periodically updated.
	 */
	update_आरोपय_avg(cfs_rq, curr, UPDATE_TG);
	update_cfs_गणः(curr);

#यदि_परिभाषितम् CONFIG_SCHED_HRTICK
	/*
	 * queued ticks are scheduled to match the slice, so don't bother
	 * validating it and just reschedule.
	 */
	यदि (queued) {
		resched_curr(rq_सम्बन्धि(cfs_rq));
		प्रत्यागम;
	}
#यदि_अन्तः
}


/**************************************************
 * CFS bandwidth control machinery
 */

#यदि_परिभाषितम् CONFIG_CFS_BANDWIDTH

#यदि_परिभाषितम् CONFIG_JUMP_LABEL
स्थिर संरचना static_कुञ्चिका __cfs_bandwidth_used;

स्थिर अन्तःस्थ bool cfs_bandwidth_प्रयुक्तम्(शून्य)
{
	प्रत्यागम static_कुञ्चिका_false(&__cfs_bandwidth_used);
}

शून्य cfs_bandwidth_usage_inc(शून्य)
{
	static_कुञ्चिका_slow_inc_cpuslocked(&__cfs_bandwidth_used);
}

शून्य cfs_bandwidth_usage_dec(शून्य)
{
	static_कुञ्चिका_slow_dec_cpuslocked(&__cfs_bandwidth_used);
}
#अन्यथा /* !CONFIG_JUMP_LABEL: */
स्थिर bool cfs_bandwidth_प्रयुक्तम्(शून्य)
{
	प्रत्यागम true;
}

शून्य cfs_bandwidth_usage_inc(शून्य) {}
शून्य cfs_bandwidth_usage_dec(शून्य) {}
#यदि_अन्तः /* !CONFIG_JUMP_LABEL */

स्थिर अन्तःस्थ u64 sched_cfs_bandwidth_खण्डिका(शून्य)
{
	प्रत्यागम (u64)sysctl_sched_cfs_bandwidth_खण्डिका * NSEC_प्रत्येकम्_USEC;
}

/*
 * Replenish runtime according to assigned quota. We use sched_clock_cpu
 * directly instead of rq->clock to avoid adding additional synchronization
 * around rq->lock.
 *
 * requires cfs_b->lock
 */
शून्य __refill_cfs_bandwidth_runtime(संरचना cfs_bandwidth *cfs_b)
{
	s64 runtime;

	यदि (unlikely(cfs_b->quota == RUNTIME_INF))
		प्रत्यागम;

	cfs_b->runtime += cfs_b->quota;
	runtime = cfs_b->runtime_snap - cfs_b->runtime;
	यदि (runtime > 0) {
		cfs_b->burst_time += runtime;
		cfs_b->nr_burst++;
	}

	cfs_b->runtime = min(cfs_b->runtime, cfs_b->quota + cfs_b->burst);
	cfs_b->runtime_snap = cfs_b->runtime;
}

स्थिर अन्तःस्थ संरचना cfs_bandwidth *tg_cfs_bandwidth(संरचना task_गणः *tg)
{
	प्रत्यागम &tg->cfs_bandwidth;
}

/* returns 0 on failure to allocate runtime */
स्थिर पूर्णाङ्क __assign_cfs_rq_runtime(संरचना cfs_bandwidth *cfs_b,
				   संरचना cfs_rq *cfs_rq, u64 target_runtime)
{
	u64 min_amount, amount = 0;

	lockdep_assert_held(&cfs_b->तालकम्);

	/* note: this is a positive sum as runtime_remaining <= 0 */
	min_amount = target_runtime - cfs_rq->runtime_remaining;

	यदि (cfs_b->quota == RUNTIME_INF)
		amount = min_amount;
	अन्यथा {
		start_cfs_bandwidth(cfs_b);

		यदि (cfs_b->runtime > 0) {
			amount = min(cfs_b->runtime, min_amount);
			cfs_b->runtime -= amount;
			cfs_b->idle = 0;
		}
	}

	cfs_rq->runtime_remaining += amount;

	प्रत्यागम cfs_rq->runtime_remaining > 0;
}

स्थिर bool throttle_cfs_rq(संरचना cfs_rq *cfs_rq);

स्थिर bool __account_cfs_rq_runtime(संरचना cfs_rq *cfs_rq, u64 delta_exec)
{
	/* dock delta_exec before expiring quota (as it could span periods) */
	cfs_rq->runtime_remaining -= delta_exec;

	यदि (likely(cfs_rq->runtime_remaining > 0))
		प्रत्यागम false;

	यदि (cfs_rq->throttled)
		प्रत्यागम true;
	/*
	 * throttle_cfs_rq() will try to extend the runtime first
	 * before throttling the hierarchy.
	 */
	प्रत्यागम throttle_cfs_rq(cfs_rq);
}

स्थिर __always_inline
bool account_cfs_rq_runtime(संरचना cfs_rq *cfs_rq, u64 delta_exec)
{
	यदि (!cfs_bandwidth_प्रयुक्तम्() || !cfs_rq->runtime_enabled)
		प्रत्यागम false;

	प्रत्यागम __account_cfs_rq_runtime(cfs_rq, delta_exec);
}

स्थिर अन्तःस्थ पूर्णाङ्क cfs_rq_throttled(संरचना cfs_rq *cfs_rq)
{
	प्रत्यागम cfs_bandwidth_प्रयुक्तम्() && cfs_rq->throttled;
}

स्थिर अन्तःस्थ bool cfs_rq_pelt_clock_throttled(संरचना cfs_rq *cfs_rq)
{
	प्रत्यागम cfs_bandwidth_प्रयुक्तम्() && cfs_rq->pelt_clock_throttled;
}

/* check whether cfs_rq, or any parent, is throttled */
स्थिर अन्तःस्थ पूर्णाङ्क throttled_hierarchy(संरचना cfs_rq *cfs_rq)
{
	प्रत्यागम cfs_bandwidth_प्रयुक्तम्() && cfs_rq->throttle_गणनम्;
}

स्थिर अन्तःस्थ पूर्णाङ्क lb_throttled_hierarchy(संरचना task_struct *p, पूर्णाङ्क dst_cpu)
{
	प्रत्यागम throttled_hierarchy(tg_cfs_rq(task_गणः(p), dst_cpu));
}

स्थिर अन्तःस्थ bool task_is_throttled(संरचना task_struct *p)
{
	प्रत्यागम cfs_bandwidth_प्रयुक्तम्() && p->throttled;
}

स्थिर bool पङ्क्तेः_निष्कासय_task_fair(संरचना rq *rq, संरचना task_struct *p, पूर्णाङ्क flags);
स्थिर शून्य throttle_cfs_rq_work(संरचना callback_शीर्ष *work)
{
	संरचना task_struct *p = container_सम्बन्धि(work, संरचना task_struct, sched_throttle_work);
	संरचना sched_entity *se;
	संरचना cfs_rq *cfs_rq;
	संरचना rq *rq;

	WARN_ON_ONCE(p != current);
	p->sched_throttle_work.अग्र = &p->sched_throttle_work;

	/*
	 * If task is exiting, then there won't be a return to userspace, so we
	 * don't have to bother with any of this.
	 */
	यदि ((p->flags & PF_EXITING))
		प्रत्यागम;

	scoped_guard(task_rq_तालकम्_2, p) {
		se = &p->se;
		cfs_rq = cfs_rq_सम्बन्धि(se);

		/* Raced, forget */
		यदि (p->sched_class != &fair_sched_class)
			प्रत्यागम;

		/*
		 * If not in limbo, then either replenish has happened or this
		 * task got migrated out of the throttled cfs_rq, move along.
		 */
		यदि (!cfs_rq->throttle_गणनम्)
			प्रत्यागम;
		rq = scope.rq;
		update_rq_clock(rq);
		WARN_ON_ONCE(p->throttled || !list_रिक्तानि(&p->throttle_ग्रन्थिः));
		पङ्क्तेः_निष्कासय_task_fair(rq, p, पङ्क्तेः_निष्कासय_SLEEP | पङ्क्तेः_निष्कासय_THROTTLE);
		list_सङ्कलय(&p->throttle_ग्रन्थिः, &cfs_rq->throttled_limbo_list);
		/*
		 * Must not set throttled before dequeue or dequeue will
		 * mistakenly regard this task as an already throttled one.
		 */
		p->throttled = true;
		resched_curr(rq);
	}
}

शून्य init_cfs_throttle_work(संरचना task_struct *p)
{
	init_task_work(&p->sched_throttle_work, throttle_cfs_rq_work);
	/* Protect against double add, see throttle_cfs_rq() and throttle_cfs_rq_work() */
	p->sched_throttle_work.अग्र = &p->sched_throttle_work;
	INIT_LIST_शीर्ष(&p->throttle_ग्रन्थिः);
}

/*
 * Task is throttled and someone wants to dequeue it again:
 * it could be sched/core when core needs to do things like
 * task affinity change, task group change, task sched class
 * change etc. and in these cases, DEQUEUE_SLEEP is not set;
 * or the task is blocked after throttled due to freezer etc.
 * and in these cases, DEQUEUE_SLEEP is set.
 */
स्थिर शून्य detach_task_cfs_rq(संरचना task_struct *p);
स्थिर शून्य पङ्क्तेः_निष्कासय_throttled_task(संरचना task_struct *p, पूर्णाङ्क flags)
{
	WARN_ON_ONCE(p->se.on_rq);
	list_del_init(&p->throttle_ग्रन्थिः);

	/* task blocked after throttled */
	यदि (flags & पङ्क्तेः_निष्कासय_SLEEP) {
		p->throttled = false;
		प्रत्यागम;
	}

	/*
	 * task is migrating off its old cfs_rq, detach
	 * the task's load from its old cfs_rq.
	 */
	यदि (task_on_rq_migrating(p))
		detach_task_cfs_rq(p);
}

स्थिर bool पङ्क्तौ_निक्षिप_throttled_task(संरचना task_struct *p)
{
	संरचना cfs_rq *cfs_rq = cfs_rq_सम्बन्धि(&p->se);

	/* @p should have gone through dequeue_throttled_task() first */
	WARN_ON_ONCE(!list_रिक्तानि(&p->throttle_ग्रन्थिः));

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
	यदि (throttled_hierarchy(cfs_rq) &&
	    !task_current_donor(rq_सम्बन्धि(cfs_rq), p)) {
		list_सङ्कलय(&p->throttle_ग्रन्थिः, &cfs_rq->throttled_limbo_list);
		प्रत्यागम true;
	}

	/* we can't take the fast path, do an actual enqueue*/
	p->throttled = false;
	प्रत्यागम false;
}

स्थिर शून्य पङ्क्तौ_निक्षिप_task_fair(संरचना rq *rq, संरचना task_struct *p, पूर्णाङ्क flags);
स्थिर पूर्णाङ्क tg_unthrottle_ऊर्ध्वम्(संरचना task_गणः *tg, शून्य *दत्त)
{
	संरचना rq *rq = दत्त;
	संरचना cfs_rq *cfs_rq = tg_cfs_rq(tg, cpu_सम्बन्धि(rq));
	संरचना task_struct *p, *tmp;
	LIST_शीर्ष(throttled_tasks);

	/*
	 * If cfs_rq->curr is set, the cfs_rq might not have caught up
	 * since the last clock update. Do it now before we begin
	 * queueing task onto it to save the need for unnecessarily
	 * unthrottle the hierarchy for this cfs_rq to be throttled
	 * right back again.
	 */
	update_curr(cfs_rq);

	यदि (--cfs_rq->throttle_गणनम्)
		प्रत्यागम 0;

	यदि (cfs_rq->pelt_clock_throttled) {
		cfs_rq->throttled_clock_pelt_time += rq_clock_pelt(rq) -
					     cfs_rq->throttled_clock_pelt;
		cfs_rq->pelt_clock_throttled = 0;
	}

	यदि (cfs_rq->throttled_clock_self) {
		u64 delta = rq_clock(rq) - cfs_rq->throttled_clock_self;

		cfs_rq->throttled_clock_self = 0;

		यदि (WARN_ON_ONCE((s64)delta < 0))
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
	list_for_each_प्रविष्टिः_safe(p, tmp, &throttled_tasks, throttle_ग्रन्थिः) {
		/*
		 * Back to being throttled! Break out and put the remaining
		 * tasks back onto the limbo_list to prevent running them
		 * unnecessarily.
		 */
		यदि (cfs_rq->throttle_गणनम्)
			भङ्ग;

		list_del_init(&p->throttle_ग्रन्थिः);
		p->throttled = false;
		पङ्क्तौ_निक्षिप_task_fair(rq, p, पङ्क्तौ_निक्षिप_WAKEUP);
	}

	list_splice(&throttled_tasks, &cfs_rq->throttled_limbo_list);

	/* Add cfs_rq with load or one or more already running entities to the list */
	यदि (!cfs_rq_is_decayed(cfs_rq))
		list_सङ्कलय_पर्णम्_cfs_rq(cfs_rq);

	प्रत्यागम 0;
}

स्थिर अन्तःस्थ bool task_has_throttle_work(संरचना task_struct *p)
{
	प्रत्यागम p->sched_throttle_work.अग्र != &p->sched_throttle_work;
}

स्थिर अन्तःस्थ शून्य task_throttle_सज्जता_work(संरचना task_struct *p)
{
	यदि (task_has_throttle_work(p))
		प्रत्यागम;

	/*
	 * Kthreads and exiting tasks don't return to userspace, so adding the
	 * work is pointless
	 */
	यदि ((p->flags & (PF_EXITING | PF_KTHREAD)))
		प्रत्यागम;

	task_work_सङ्कलय(p, &p->sched_throttle_work, TWA_RESUME);
}

स्थिर शून्य लेखः_throttle_clock(संरचना cfs_rq *cfs_rq)
{
	संरचना rq *rq = rq_सम्बन्धि(cfs_rq);

	यदि (cfs_rq_throttled(cfs_rq) && !cfs_rq->throttled_clock)
		cfs_rq->throttled_clock = rq_clock(rq);

	यदि (!cfs_rq->throttled_clock_self)
		cfs_rq->throttled_clock_self = rq_clock(rq);
}

स्थिर पूर्णाङ्क tg_throttle_अधस्तात्(संरचना task_गणः *tg, शून्य *दत्त)
{
	संरचना rq *rq = दत्त;
	संरचना cfs_rq *cfs_rq = tg_cfs_rq(tg, cpu_सम्बन्धि(rq));

	यदि (cfs_rq->throttle_गणनम्++)
		प्रत्यागम 0;

	/*
	 * For cfs_rqs that still have entities enqueued, PELT clock
	 * stop happens at dequeue time when all entities are dequeued.
	 */
	यदि (!cfs_rq->nr_queued) {
		list_del_पर्णम्_cfs_rq(cfs_rq);
		cfs_rq->throttled_clock_pelt = rq_clock_pelt(rq);
		cfs_rq->pelt_clock_throttled = 1;
	}

	WARN_ON_ONCE(cfs_rq->throttled_clock_self);
	WARN_ON_ONCE(!list_रिक्तानि(&cfs_rq->throttled_limbo_list));
	प्रत्यागम 0;
}

स्थिर bool throttle_cfs_rq(संरचना cfs_rq *cfs_rq)
{
	संरचना cfs_bandwidth *cfs_b = tg_cfs_bandwidth(cfs_rq->tg);
	संरचना sched_entity *curr = cfs_rq->h_curr;
	संरचना rq *rq = rq_सम्बन्धि(cfs_rq);

	scoped_guard(raw_spinlock, &cfs_b->तालकम्) {
		u64 target_runtime = 1;

		/*
		 * If cfs_rq->h_curr is still runnable, we are here from an
		 * update_curr(). Request sysctl_sched_cfs_bandwidth_slice
		 * worth of bandwidth to continue running.
		 *
		 * If the curr is not runnable, just request enough bandwidth
		 * to be runnable next time the pick selects this cfs_rq.
		 */
		यदि (curr && curr->on_rq)
			target_runtime = sched_cfs_bandwidth_खण्डिका();

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
		यदि (__assign_cfs_rq_runtime(cfs_b, cfs_rq, target_runtime))
			प्रत्यागम false;

		/*
		 * No bandwidth available; Add ourselves on the list to be
		 * unthrottled later.
		 */
		list_सङ्कलय_पुच्छ_rcu(&cfs_rq->throttled_list,
				  &cfs_b->throttled_cfs_rq);
	}

	/* freeze hierarchy runnable averages while throttled */
	scoped_guard(rcu)
		walk_tg_वृक्षः_from(cfs_rq->tg, tg_throttle_अधस्तात्, tg_nop, (शून्य *)rq);

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
	यदि (curr && curr->on_rq)
		task_throttle_सज्जता_work(rq->donor);

	प्रत्यागम true;
}

शून्य unthrottle_cfs_rq(संरचना cfs_rq *cfs_rq)
{
	संरचना rq *rq = rq_सम्बन्धि(cfs_rq);
	संरचना cfs_bandwidth *cfs_b = tg_cfs_bandwidth(cfs_rq->tg);
	संरचना sched_entity *se = cfs_rq_se(cfs_rq);

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
	यदि (cfs_rq->runtime_enabled && cfs_rq->runtime_remaining <= 0)
		प्रत्यागम;

	cfs_rq->throttled = 0;

	scoped_guard(raw_spinlock, &cfs_b->तालकम्) {
		list_del_rcu(&cfs_rq->throttled_list);

		यदि (!cfs_rq->throttled_clock)
			भङ्ग;

		cfs_b->throttled_time += rq_clock(rq) - cfs_rq->throttled_clock;
		cfs_rq->throttled_clock = 0;
	}

	/* update hierarchical throttle state */
	walk_tg_वृक्षः_from(cfs_rq->tg, tg_nop, tg_unthrottle_ऊर्ध्वम्, (शून्य *)rq);

	यदि (!cfs_rq->आरोपय.weight) {
		यदि (!cfs_rq->on_list)
			प्रत्यागम;
		/*
		 * Nothing to run but something to decay (on_list)?
		 * Complete the branch.
		 */
		for_each_sched_entity(se) {
			यदि (list_सङ्कलय_पर्णम्_cfs_rq(cfs_rq_सम्बन्धि(se)))
				भङ्ग;
		}
	}

	assert_list_पर्णम्_cfs_rq(rq);

	/* Determine whether we need to wake up potentially idle CPU: */
	यदि (rq->curr == rq->idle && rq->cfs.h_nr_queued)
		resched_curr(rq);
}

स्थिर शून्य __cfsb_csd_unthrottle(शून्य *arg)
{
	संरचना cfs_rq *cursor, *tmp;
	संरचना rq *rq = arg;

	guard(rq_तालकम्)(rq);

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

	list_for_each_प्रविष्टिः_safe(cursor, tmp, &rq->cfsb_csd_list,
				 throttled_csd_list) {
		list_del_init(&cursor->throttled_csd_list);

		यदि (cfs_rq_throttled(cursor))
			unthrottle_cfs_rq(cursor);
	}

	rq_clock_stop_loop_update(rq);
}

स्थिर अन्तःस्थ शून्य __unthrottle_cfs_rq_async(संरचना cfs_rq *cfs_rq)
{
	संरचना rq *rq = rq_सम्बन्धि(cfs_rq);
	bool first;

	यदि (rq == this_rq()) {
		update_rq_clock(rq);
		unthrottle_cfs_rq(cfs_rq);
		प्रत्यागम;
	}

	/* Already enqueued */
	यदि (WARN_ON_ONCE(!list_रिक्तानि(&cfs_rq->throttled_csd_list)))
		प्रत्यागम;

	first = list_रिक्तानि(&rq->cfsb_csd_list);
	list_सङ्कलय_पुच्छ(&cfs_rq->throttled_csd_list, &rq->cfsb_csd_list);
	यदि (first)
		smp_call_function_single_async(cpu_सम्बन्धि(rq), &rq->cfsb_csd);
}

स्थिर शून्य unthrottle_cfs_rq_async(संरचना cfs_rq *cfs_rq)
{
	lockdep_assert_rq_held(rq_सम्बन्धि(cfs_rq));

	यदि (WARN_ON_ONCE(!cfs_rq_throttled(cfs_rq) ||
	    cfs_rq->runtime_remaining <= 0))
		प्रत्यागम;

	__unthrottle_cfs_rq_async(cfs_rq);
}

स्थिर bool distribute_cfs_runtime(संरचना cfs_bandwidth *cfs_b)
{
	bool throttled = false, unthrottle_local = false;
	पूर्णाङ्क this_cpu = smp_processor_id();
	u64 runtime, remaining = 1;
	संरचना cfs_rq *cfs_rq;
	संरचना rq *rq;

	guard(rcu)();

	list_for_each_प्रविष्टिः_rcu(cfs_rq, &cfs_b->throttled_cfs_rq,
				throttled_list) {
		rq = rq_सम्बन्धि(cfs_rq);

		यदि (!remaining) {
			throttled = true;
			भङ्ग;
		}

		guard(rq_तालकम्_irqsave)(rq);

		यदि (!cfs_rq_throttled(cfs_rq))
			अनुवर्त;

		/* Already queued for async unthrottle */
		यदि (!list_रिक्तानि(&cfs_rq->throttled_csd_list))
			अनुवर्त;

		यदि (cfs_rq->h_curr) {
			update_rq_clock(rq);
			update_curr(cfs_rq);
		}

		/* By the above checks, this should never be true */
		WARN_ON_ONCE(cfs_rq->runtime_remaining > 0);

		scoped_guard(raw_spinlock, &cfs_b->तालकम्) {
			runtime = -cfs_rq->runtime_remaining + 1;
			यदि (runtime > cfs_b->runtime)
				runtime = cfs_b->runtime;
			cfs_b->runtime -= runtime;
			remaining = cfs_b->runtime;
		}

		cfs_rq->runtime_remaining += runtime;

		/*
		 * Ran out of bandwidth during distribution!
		 * Indicate throttled entities and break early.
		 */
		यदि (cfs_rq->runtime_remaining <= 0) {
			throttled = true;
			भङ्ग;
		}

		/* we check whether we're throttled above */
		यदि (cpu_सम्बन्धि(rq) != this_cpu) {
			unthrottle_cfs_rq_async(cfs_rq);
			अनुवर्त;
		}

		/*
		 * Allow a parallel async unthrottle to unthrottle
		 * this cfs_rq too via __cfsb_csd_unthrottle().
		 * If we are first, do it ourselves at the end and
		 * save on an IPI from remote CPUs.
		 */
		unthrottle_local = list_रिक्तानि(&rq->cfsb_csd_list);
		list_सङ्कलय_पुच्छ(&cfs_rq->throttled_csd_list, &rq->cfsb_csd_list);
	}

	यदि (unthrottle_local) {
		/*
		 * Protect against an IPI that is also trying to flush
		 * the unthrottled cfs_rq(s) from this CPU's csd_list.
		 */
		scoped_guard(irqsave)
			__cfsb_csd_unthrottle(cpu_rq(this_cpu));
	}

	प्रत्यागम throttled;
}

/*
 * Responsible for refilling a task_group's bandwidth and unthrottling its
 * cfs_rqs as appropriate. If there has been no activity within the last
 * period the timer is deactivated until scheduling resumes; cfs_b->idle is
 * used to track this state.
 */
स्थिर पूर्णाङ्क do_sched_cfs_period_timer(संरचना cfs_bandwidth *cfs_b, पूर्णाङ्क overrun, अचिह्नित दीर्घ flags)
	__must_hold(&cfs_b->तालकम्)
{
	पूर्णाङ्क throttled;

	/* no need to continue the timer with no bandwidth constraint */
	यदि (cfs_b->quota == RUNTIME_INF)
		गच्छ बहिर्गमनम्_deactivate;

	throttled = !list_रिक्तानि(&cfs_b->throttled_cfs_rq);
	cfs_b->nr_periods += overrun;

	/* Refill extra burst quota even if cfs_b->idle */
	__refill_cfs_bandwidth_runtime(cfs_b);

	/*
	 * idle depends on !throttled (for the case of a large deficit), and if
	 * we're going inactive then everything else can be deferred
	 */
	यदि (cfs_b->idle && !throttled)
		गच्छ बहिर्गमनम्_deactivate;

	यदि (!throttled) {
		/* mark as potentially idle for the upcoming period */
		cfs_b->idle = 1;
		प्रत्यागम 0;
	}

	/* account preceding periods in which throttling occurred */
	cfs_b->nr_throttled += overrun;

	/*
	 * This check is repeated as we release cfs_b->lock while we unthrottle.
	 */
	यावत् (throttled && cfs_b->runtime > 0) {
		raw_spin_unlock_irqrestore(&cfs_b->तालकम्, flags);
		/* we can't nest cfs_b->lock while distributing bandwidth */
		throttled = distribute_cfs_runtime(cfs_b);
		raw_spin_तालकम्_irqsave(&cfs_b->तालकम्, flags);
	}

	/*
	 * While we are ensured activity in the period following an
	 * unthrottle, this also covers the case in which the new bandwidth is
	 * insufficient to cover the existing bandwidth deficit.  (Forcing the
	 * timer to remain active while there are any throttled entities.)
	 */
	cfs_b->idle = 0;

	प्रत्यागम 0;

बहिर्गमनम्_deactivate:
	प्रत्यागम 1;
}

/* a cfs_rq won't donate quota below this amount */
स्थिर ध्रुव u64 min_cfs_rq_runtime = 1 * NSEC_प्रत्येकम्_MSEC;
/* minimum remaining period time to redistribute slack quota */
स्थिर ध्रुव u64 min_bandwidth_expiration = 2 * NSEC_प्रत्येकम्_MSEC;
/* how long we wait to gather additional slack before distributing */
स्थिर ध्रुव u64 cfs_bandwidth_slack_period = 5 * NSEC_प्रत्येकम्_MSEC;

/*
 * Are we near the end of the current quota period?
 *
 * Requires cfs_b->lock for hrtimer_expires_remaining to be safe against the
 * hrtimer base being cleared by hrtimer_start. In the case of
 * migrate_hrtimers, base is never cleared, so we are fine.
 */
स्थिर पूर्णाङ्क runtime_refresh_within(संरचना cfs_bandwidth *cfs_b, u64 min_expire)
{
	संरचना hrtimer *refresh_timer = &cfs_b->period_timer;
	s64 remaining;

	/* if the call-back is running a quota refresh is already occurring */
	यदि (hrtimer_callback_running(refresh_timer))
		प्रत्यागम 1;

	/* is a quota refresh about to occur? */
	remaining = ktime_गन्तव्यम्_ns(hrtimer_expires_remaining(refresh_timer));
	यदि (remaining < (s64)min_expire)
		प्रत्यागम 1;

	प्रत्यागम 0;
}

स्थिर शून्य start_cfs_slack_bandwidth(संरचना cfs_bandwidth *cfs_b)
{
	u64 min_वामः = cfs_bandwidth_slack_period + min_bandwidth_expiration;

	/* if there's a quota refresh soon don't bother with slack */
	यदि (runtime_refresh_within(cfs_b, min_वामः))
		प्रत्यागम;

	/* don't push forwards an existing deferred unthrottle */
	यदि (cfs_b->slack_started)
		प्रत्यागम;
	cfs_b->slack_started = true;

	hrtimer_start(&cfs_b->slack_timer,
			ns_गन्तव्यम्_ktime(cfs_bandwidth_slack_period),
			HRTIMER_बहुलकम्_REL);
}

/* we know any runtime found here is valid as update_curr() precedes return */
स्थिर शून्य __return_cfs_rq_runtime(संरचना cfs_rq *cfs_rq)
{
	संरचना cfs_bandwidth *cfs_b = tg_cfs_bandwidth(cfs_rq->tg);
	s64 slack_runtime = cfs_rq->runtime_remaining - min_cfs_rq_runtime;

	यदि (slack_runtime <= 0)
		प्रत्यागम;

	guard(raw_spinlock)(&cfs_b->तालकम्);

	यदि (cfs_b->quota != RUNTIME_INF) {
		cfs_b->runtime += slack_runtime;

		/* we are under rq->lock, defer unthrottling using a timer */
		यदि (cfs_b->runtime > sched_cfs_bandwidth_खण्डिका() &&
		    !list_रिक्तानि(&cfs_b->throttled_cfs_rq))
			start_cfs_slack_bandwidth(cfs_b);
	}

	/* even if it's not valid for return we don't want to try again */
	cfs_rq->runtime_remaining -= slack_runtime;
}

स्थिर __always_inline शून्य return_cfs_rq_runtime(संरचना cfs_rq *cfs_rq)
{
	यदि (!cfs_bandwidth_प्रयुक्तम्())
		प्रत्यागम;

	यदि (!cfs_rq->runtime_enabled || cfs_rq->nr_queued)
		प्रत्यागम;

	__return_cfs_rq_runtime(cfs_rq);
}

/*
 * This is done with a timer (instead of inline with bandwidth return) since
 * it's necessary to juggle rq->locks to unthrottle their respective cfs_rqs.
 */
स्थिर शून्य do_sched_cfs_slack_timer(संरचना cfs_bandwidth *cfs_b)
{
	/* confirm we're still not at a refresh boundary */
	scoped_guard(raw_spinlock_irqsave, &cfs_b->तालकम्) {
		u64 runtime = 0, खण्डिका = sched_cfs_bandwidth_खण्डिका();

		cfs_b->slack_started = false;

		यदि (runtime_refresh_within(cfs_b, min_bandwidth_expiration))
			प्रत्यागम;

		यदि (cfs_b->quota != RUNTIME_INF && cfs_b->runtime > खण्डिका)
			runtime = cfs_b->runtime;

		यदि (!runtime)
			प्रत्यागम;
	}

	distribute_cfs_runtime(cfs_b);
}

/*
 * When a group wakes up we want to make sure that its quota is not already
 * expired/exceeded, otherwise it may be allowed to steal additional ticks of
 * runtime as update_curr() throttling can not trigger until it's on-rq.
 */
स्थिर शून्य check_पङ्क्तौ_निक्षिप_throttle(संरचना cfs_rq *cfs_rq)
{
	यदि (!cfs_bandwidth_प्रयुक्तम्())
		प्रत्यागम;

	/* an active group must be handled by the update_curr() path */
	यदि (!cfs_rq->runtime_enabled || cfs_rq->h_curr)
		प्रत्यागम;

	/* ensure the group is not already throttled */
	यदि (cfs_rq_throttled(cfs_rq))
		प्रत्यागम;

	/* update runtime allocation */
	account_cfs_rq_runtime(cfs_rq, 0);
}

स्थिर शून्य sync_throttle(संरचना task_गणः *tg, पूर्णाङ्क cpu)
{
	संरचना cfs_rq *pcfs_rq, *cfs_rq;

	यदि (!cfs_bandwidth_प्रयुक्तम्())
		प्रत्यागम;

	यदि (!tg->parent)
		प्रत्यागम;

	cfs_rq = tg_cfs_rq(tg, cpu);
	pcfs_rq = tg_cfs_rq(tg->parent, cpu);

	cfs_rq->throttle_गणनम् = pcfs_rq->throttle_गणनम्;
	cfs_rq->throttled_clock_pelt = rq_clock_pelt(cpu_rq(cpu));

	/*
	 * It is not enough to sync the "pelt_clock_throttled" indicator
	 * with the parent cfs_rq when the hierarchy is not queued.
	 * Always join a throttled hierarchy with PELT clock throttled
	 * and leaf it to the first enqueue, or distribution to
	 * unthrottle the PELT clock.
	 */
	यदि (cfs_rq->throttle_गणनम्)
		cfs_rq->pelt_clock_throttled = 1;
}

स्थिर गणना hrtimer_restart sched_cfs_slack_timer(संरचना hrtimer *timer)
{
	संरचना cfs_bandwidth *cfs_b =
		container_सम्बन्धि(timer, संरचना cfs_bandwidth, slack_timer);

	do_sched_cfs_slack_timer(cfs_b);

	प्रत्यागम HRTIMER_NORESTART;
}

स्थिर गणना hrtimer_restart sched_cfs_period_timer(संरचना hrtimer *timer)
{
	संरचना cfs_bandwidth *cfs_b =
		container_सम्बन्धि(timer, संरचना cfs_bandwidth, period_timer);
	पूर्णाङ्क overrun;
	पूर्णाङ्क idle = 0;
	पूर्णाङ्क गणनम् = 0;

	CLASS(raw_spinlock_irqsave, cfsb_guard)(&cfs_b->तालकम्);

	प्रति (;;) {
		overrun = hrtimer_forward_now(timer, cfs_b->period);
		यदि (!overrun)
			भङ्ग;

		idle = do_sched_cfs_period_timer(cfs_b, overrun, cfsb_guard.flags);

		यदि (++गणनम् > 3) {
			u64 new, old = ktime_गन्तव्यम्_ns(cfs_b->period);

			/*
			 * Grow period by a factor of 2 to avoid losing precision.
			 * Precision loss in the quota/period ratio can cause __cfs_schedulable
			 * to fail.
			 */
			new = old * 2;
			यदि (new < max_bw_quota_period_us * NSEC_प्रत्येकम्_USEC) {
				cfs_b->period = ns_गन्तव्यम्_ktime(new);
				cfs_b->quota *= 2;
				cfs_b->burst *= 2;

				pr_warn_ratelimited(
	"cfs_period_timer[cpu%d]: period too short, scaling up (new cfs_period_us = %lld, cfs_quota_us = %lld)\n",
					smp_processor_id(),
					div_u64(new, NSEC_प्रत्येकम्_USEC),
					div_u64(cfs_b->quota, NSEC_प्रत्येकम्_USEC));
			} अन्यथा {
				pr_warn_ratelimited(
	"cfs_period_timer[cpu%d]: period too short, but cannot scale up without losing precision (cfs_period_us = %lld, cfs_quota_us = %lld)\n",
					smp_processor_id(),
					div_u64(old, NSEC_प्रत्येकम्_USEC),
					div_u64(cfs_b->quota, NSEC_प्रत्येकम्_USEC));
			}

			/* reset count so we don't come right back in here */
			गणनम् = 0;
		}
	}

	यदि (idle) {
		cfs_b->period_सक्रिय = 0;
		प्रत्यागम HRTIMER_NORESTART;
	}

	प्रत्यागम HRTIMER_RESTART;
}

शून्य init_cfs_bandwidth(संरचना cfs_bandwidth *cfs_b, संरचना cfs_bandwidth *parent)
{
	raw_spin_तालकम्_init(&cfs_b->तालकम्);
	cfs_b->runtime = 0;
	cfs_b->quota = RUNTIME_INF;
	cfs_b->period = us_गन्तव्यम्_ktime(default_bw_period_us());
	cfs_b->burst = 0;
	cfs_b->श्रेणीबद्धम्_quota = parent ? parent->श्रेणीबद्धम्_quota : RUNTIME_INF;

	INIT_LIST_शीर्ष(&cfs_b->throttled_cfs_rq);
	hrtimer_सज्जता(&cfs_b->period_timer, sched_cfs_period_timer, CLOCK_MONOTONIC,
		      HRTIMER_बहुलकम्_ABS_PINNED);

	/* Add a random offset so that timers interleave */
	hrtimer_समुच्चयः_expires(&cfs_b->period_timer,
			    get_random_u32_अधः(cfs_b->period));
	hrtimer_सज्जता(&cfs_b->slack_timer, sched_cfs_slack_timer, CLOCK_MONOTONIC,
		      HRTIMER_बहुलकम्_REL);
	cfs_b->slack_started = false;
}

स्थिर शून्य init_cfs_rq_runtime(संरचना cfs_rq *cfs_rq)
{
	cfs_rq->runtime_enabled = 0;
	INIT_LIST_शीर्ष(&cfs_rq->throttled_list);
	INIT_LIST_शीर्ष(&cfs_rq->throttled_csd_list);
	INIT_LIST_शीर्ष(&cfs_rq->throttled_limbo_list);
}

शून्य start_cfs_bandwidth(संरचना cfs_bandwidth *cfs_b)
{
	lockdep_assert_held(&cfs_b->तालकम्);

	यदि (cfs_b->period_सक्रिय)
		प्रत्यागम;

	cfs_b->period_सक्रिय = 1;
	hrtimer_forward_now(&cfs_b->period_timer, cfs_b->period);
	hrtimer_start_expires(&cfs_b->period_timer, HRTIMER_बहुलकम्_ABS_PINNED);
}

स्थिर शून्य destroy_cfs_bandwidth(संरचना cfs_bandwidth *cfs_b)
{
	पूर्णाङ्क __maybe_unused i;

	/* init_cfs_bandwidth() was not called */
	यदि (!cfs_b->throttled_cfs_rq.अग्र)
		प्रत्यागम;

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
		संरचना rq *rq = cpu_rq(i);

		यदि (list_रिक्तानि(&rq->cfsb_csd_list))
			अनुवर्त;

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
स्थिर शून्य __maybe_unused update_runtime_enabled(संरचना rq *rq)
{
	संरचना task_गणः *tg;

	lockdep_assert_rq_held(rq);

	guard(rcu)();

	list_for_each_प्रविष्टिः_rcu(tg, &task_groups, list) {
		संरचना cfs_bandwidth *cfs_b = &tg->cfs_bandwidth;
		संरचना cfs_rq *cfs_rq = tg_cfs_rq(tg, cpu_सम्बन्धि(rq));

		scoped_guard(raw_spinlock, &cfs_b->तालकम्)
			cfs_rq->runtime_enabled = cfs_b->quota != RUNTIME_INF;
	}
}

/* cpu offline callback */
स्थिर शून्य __maybe_unused unthrottle_offline_cfs_rqs(संरचना rq *rq)
{
	संरचना task_गणः *tg;

	lockdep_assert_rq_held(rq);

	// Do not unthrottle for an active CPU
	यदि (cpumask_test_cpu(cpu_सम्बन्धि(rq), cpu_सक्रिय_आवरणम्))
		प्रत्यागम;

	/*
	 * The rq clock has already been updated in the
	 * set_rq_offline(), so we should skip updating
	 * the rq clock again in unthrottle_cfs_rq().
	 */
	rq_clock_start_loop_update(rq);

	guard(rcu)();

	list_for_each_प्रविष्टिः_rcu(tg, &task_groups, list) {
		संरचना cfs_rq *cfs_rq = tg_cfs_rq(tg, cpu_सम्बन्धि(rq));

		यदि (!cfs_rq->runtime_enabled)
			अनुवर्त;

		/*
		 * Offline rq is schedulable till CPU is completely disabled
		 * in take_cpu_down(), so we prevent new cfs throttling here.
		 */
		cfs_rq->runtime_enabled = 0;

		यदि (!cfs_rq_throttled(cfs_rq))
			अनुवर्त;

		/*
		 * clock_task is not advancing so we just need to make sure
		 * there's some valid quota amount
		 */
		cfs_rq->runtime_remaining = 1;
		unthrottle_cfs_rq(cfs_rq);
	}

	rq_clock_stop_loop_update(rq);
}

bool cfs_task_bw_constrained(संरचना task_struct *p)
{
	संरचना cfs_rq *cfs_rq = task_cfs_rq(p);

	यदि (!cfs_bandwidth_प्रयुक्तम्())
		प्रत्यागम false;

	यदि (cfs_rq->runtime_enabled ||
	    tg_cfs_bandwidth(cfs_rq->tg)->श्रेणीबद्धम्_quota != RUNTIME_INF)
		प्रत्यागम true;

	प्रत्यागम false;
}

#यदि_परिभाषितम् CONFIG_NO_HZ_पूर्णम्
/* called from pick_next_task_fair() */
स्थिर शून्य sched_fair_update_stop_tick(संरचना rq *rq, संरचना task_struct *p)
{
	पूर्णाङ्क cpu = cpu_सम्बन्धि(rq);

	यदि (!cfs_bandwidth_प्रयुक्तम्())
		प्रत्यागम;

	यदि (!tick_nohz_पूर्णम्_cpu(cpu))
		प्रत्यागम;

	यदि (rq->nr_running != 1)
		प्रत्यागम;

	/*
	 *  We know there is only one task runnable and we've just picked it. The
	 *  normal enqueue path will have cleared TICK_DEP_BIT_SCHED if we will
	 *  be otherwise able to stop the tick. Just need to check if we are using
	 *  bandwidth control.
	 */
	यदि (cfs_task_bw_constrained(p))
		tick_nohz_dep_समुच्चयः_cpu(cpu, TICK_DEP_BIT_SCHED);
}
#यदि_अन्तः /* CONFIG_NO_HZ_FULL */

#अन्यथा /* !CONFIG_CFS_BANDWIDTH: */

स्थिर bool account_cfs_rq_runtime(संरचना cfs_rq *cfs_rq, u64 delta_exec) { प्रत्यागम false; }
स्थिर शून्य check_पङ्क्तौ_निक्षिप_throttle(संरचना cfs_rq *cfs_rq) {}
स्थिर अन्तःस्थ शून्य sync_throttle(संरचना task_गणः *tg, पूर्णाङ्क cpu) {}
स्थिर __always_inline शून्य return_cfs_rq_runtime(संरचना cfs_rq *cfs_rq) {}
स्थिर शून्य task_throttle_सज्जता_work(संरचना task_struct *p) {}
स्थिर bool task_is_throttled(संरचना task_struct *p) { प्रत्यागम false; }
स्थिर शून्य पङ्क्तेः_निष्कासय_throttled_task(संरचना task_struct *p, पूर्णाङ्क flags) {}
स्थिर bool पङ्क्तौ_निक्षिप_throttled_task(संरचना task_struct *p) { प्रत्यागम false; }
स्थिर शून्य लेखः_throttle_clock(संरचना cfs_rq *cfs_rq) {}

स्थिर अन्तःस्थ पूर्णाङ्क cfs_rq_throttled(संरचना cfs_rq *cfs_rq)
{
	प्रत्यागम 0;
}

स्थिर अन्तःस्थ bool cfs_rq_pelt_clock_throttled(संरचना cfs_rq *cfs_rq)
{
	प्रत्यागम false;
}

स्थिर अन्तःस्थ पूर्णाङ्क throttled_hierarchy(संरचना cfs_rq *cfs_rq)
{
	प्रत्यागम 0;
}

स्थिर अन्तःस्थ पूर्णाङ्क lb_throttled_hierarchy(संरचना task_struct *p, पूर्णाङ्क dst_cpu)
{
	प्रत्यागम 0;
}

#यदि_परिभाषितम् CONFIG_FAIR_गणः_SCHED
शून्य init_cfs_bandwidth(संरचना cfs_bandwidth *cfs_b, संरचना cfs_bandwidth *parent) {}
स्थिर शून्य init_cfs_rq_runtime(संरचना cfs_rq *cfs_rq) {}
#यदि_अन्तः

स्थिर अन्तःस्थ संरचना cfs_bandwidth *tg_cfs_bandwidth(संरचना task_गणः *tg)
{
	प्रत्यागम रिक्त;
}
स्थिर अन्तःस्थ शून्य destroy_cfs_bandwidth(संरचना cfs_bandwidth *cfs_b) {}
स्थिर अन्तःस्थ शून्य update_runtime_enabled(संरचना rq *rq) {}
स्थिर अन्तःस्थ शून्य unthrottle_offline_cfs_rqs(संरचना rq *rq) {}
#यदि_परिभाषितम् CONFIG_CGROUP_SCHED
bool cfs_task_bw_constrained(संरचना task_struct *p)
{
	प्रत्यागम false;
}
#यदि_अन्तः
#यदि_अन्तः /* !CONFIG_CFS_BANDWIDTH */

#यदि !defined(CONFIG_CFS_BANDWIDTH) || !defined(CONFIG_NO_HZ_पूर्णम्)
स्थिर अन्तःस्थ शून्य sched_fair_update_stop_tick(संरचना rq *rq, संरचना task_struct *p) {}
#यदि_अन्तः

/**************************************************
 * CFS operations on tasks:
 */

#यदि_परिभाषितम् CONFIG_SCHED_HRTICK
स्थिर शून्य hrtick_start_fair(संरचना rq *rq, संरचना task_struct *p)
{
	संरचना sched_entity *se = &p->se;
	अचिह्नित दीर्घ scale = 1024;
	अचिह्नित दीर्घ util = 0;
	u64 vdelta;
	u64 delta;

	WARN_ON_ONCE(task_rq(p) != rq);

	यदि (rq->cfs.h_nr_queued <= 1)
		प्रत्यागम;

	/*
	 * Compute time until virtual deadline
	 */
	vdelta = se->deadline - se->vruntime;
	यदि ((s64)vdelta < 0) {
		यदि (task_current_donor(rq, p))
			resched_curr(rq);
		प्रत्यागम;
	}
	delta = (se->h_आरोपय.weight * vdelta) / NICE_0_आरोपय;

	/*
	 * Correct for instantaneous load of other classes.
	 */
	util += cpu_util_irq(rq);
	यदि (util && util < 1024) {
		scale *= 1024;
		scale /= (1024 - util);
	}

	hrtick_start(rq, (scale * delta) / 1024);
}

/*
 * Called on enqueue to start the hrtick when h_nr_queued becomes more than 1.
 */
स्थिर शून्य hrtick_update(संरचना rq *rq)
{
	संरचना task_struct *donor = rq->donor;

	यदि (!hrtick_enabled_fair(rq) || donor->sched_class != &fair_sched_class)
		प्रत्यागम;

	यदि (hrtick_सक्रिय(rq))
		प्रत्यागम;

	hrtick_start_fair(rq, donor);
}
#अन्यथा /* !CONFIG_SCHED_HRTICK: */
स्थिर अन्तःस्थ शून्य
hrtick_start_fair(संरचना rq *rq, संरचना task_struct *p)
{
}

स्थिर अन्तःस्थ शून्य hrtick_update(संरचना rq *rq)
{
}
#यदि_अन्तः /* !CONFIG_SCHED_HRTICK */

स्थिर अन्तःस्थ bool cpu_overutilized(पूर्णाङ्क cpu)
{
	अचिह्नित दीर्घ rq_util_max;

	यदि (!sched_energy_enabled())
		प्रत्यागम false;

	rq_util_max = uclamp_rq_get(cpu_rq(cpu), UCLAMP_MAX);

	/* Return true only if the utilization doesn't fit CPU's capacity */
	प्रत्यागम !util_माति_cpu(cpu_util_cfs(cpu), 0, rq_util_max, cpu);
}

/*
 * overutilized value make sense only if EAS is enabled
 */
स्थिर अन्तःस्थ bool is_rd_overutilized(संरचना मूलम्_domain *rd)
{
	प्रत्यागम !sched_energy_enabled() || READ_ONCE(rd->overutilized);
}

स्थिर अन्तःस्थ शून्य समुच्चयः_rd_overutilized(संरचना मूलम्_domain *rd, bool flag)
{
	यदि (!sched_energy_enabled())
		प्रत्यागम;

	WRITE_ONCE(rd->overutilized, flag);
	trace_sched_overutilized_tp(rd, flag);
}

स्थिर अन्तःस्थ शून्य check_update_overutilized_स्थितिः(संरचना rq *rq)
{
	/*
	 * overutilized field is used for load balancing decisions only
	 * if energy aware scheduler is being used
	 */

	यदि (!is_rd_overutilized(rq->rd) && cpu_overutilized(rq->cpu))
		समुच्चयः_rd_overutilized(rq->rd, 1);
}

/* Runqueue only has SCHED_IDLE tasks enqueued */
स्थिर पूर्णाङ्क sched_idle_rq(संरचना rq *rq)
{
	प्रत्यागम unlikely(rq->nr_running == rq->cfs.h_nr_idle &&
			rq->nr_running);
}

स्थिर पूर्णाङ्क वृणु_sched_idle_rq(संरचना rq *rq, संरचना task_struct *p)
{
	प्रत्यागम sched_idle_rq(rq) && !task_has_idle_policy(p);
}

स्थिर पूर्णाङ्क वृणु_idle_cpu(पूर्णाङ्क cpu, संरचना task_struct *p)
{
	प्रत्यागम उपलब्धम्_idle_cpu(cpu) ||
	       वृणु_sched_idle_rq(cpu_rq(cpu), p);
}

स्थिर शून्य
requeue_delayed_entity(संरचना cfs_rq *cfs_rq, संरचना sched_entity *se)
{
	/*
	 * se->sched_delayed should imply: se->on_rq == 1.
	 * Because a delayed entity is one that is still on
	 * the runqueue competing until elegibility.
	 */
	WARN_ON_ONCE(!se->sched_delayed);
	WARN_ON_ONCE(!se->on_rq);

	यदि (update_entity_lag(cfs_rq, se)) {
		cfs_rq->h_nr_queued--;
		यदि (se != cfs_rq->curr)
			__dequeue_entity(cfs_rq, se);
		place_entity(cfs_rq, se, 0);
		यदि (se != cfs_rq->curr)
			__enqueue_entity(cfs_rq, se);
		cfs_rq->h_nr_queued++;
	}

	update_आरोपय_avg(cfs_rq, se, 0);
	clear_delayed(se);
}

स्थिर अचिह्नित दीर्घ पङ्क्तौ_निक्षिप_hierarchy(संरचना task_struct *p, पूर्णाङ्क flags)
{
	अचिह्नित दीर्घ weight = NICE_0_आरोपय;
	पूर्णाङ्क task_new = !(flags & पङ्क्तौ_निक्षिप_WAKEUP);
	संरचना sched_entity *se = &p->se;
	पूर्णाङ्क h_nr_idle = task_has_idle_policy(p);
	पूर्णाङ्क h_nr_runnable = 1;

	यदि (task_new && se->sched_delayed)
		h_nr_runnable = 0;

	for_each_sched_entity(se) {
		संरचना cfs_rq *cfs_rq = cfs_rq_सम्बन्धि(se);

		update_curr(cfs_rq);

		यदि (!se->on_rq) {
			पङ्क्तौ_निक्षिप_entity(cfs_rq, se, flags);
		} अन्यथा {
			update_आरोपय_avg(cfs_rq, se, UPDATE_TG);
			se_update_runnable(se);
			update_cfs_गणः(se);
		}

		cfs_rq->h_nr_runnable += h_nr_runnable;
		cfs_rq->h_nr_queued++;
		cfs_rq->h_nr_idle += h_nr_idle;

		यदि (cfs_rq_is_idle(cfs_rq))
			h_nr_idle = 1;

		weight = __calc_prop_weight(cfs_rq, se, weight);

		flags = पङ्क्तौ_निक्षिप_WAKEUP;
	}

	प्रत्यागम weight;
}

/* Update curr's vruntime before placing entity or updating lag */
स्थिर अन्तःस्थ शून्य update_curr_eevdf(संरचना cfs_rq *cfs_rq)
{
	यदि (!cfs_rq->curr)
		प्रत्यागम;

	update_curr(cfs_rq_सम्बन्धि(cfs_rq->curr));
}

/*
 * The enqueue_task method is called before nr_running is
 * increased. Here we update the fair scheduling stats and
 * then put the task into the rbtree:
 */
स्थिर शून्य
पङ्क्तौ_निक्षिप_task_fair(संरचना rq *rq, संरचना task_struct *p, पूर्णाङ्क flags)
{
	पूर्णाङ्क rq_h_nr_queued = rq->cfs.h_nr_queued;
	पूर्णाङ्क task_new = !(flags & पङ्क्तौ_निक्षिप_WAKEUP);
	संरचना sched_entity *se = &p->se;
	संरचना cfs_rq *cfs_rq = &rq->cfs;
	अचिह्नित दीर्घ weight;
	bool curr;

	यदि (task_is_throttled(p) && पङ्क्तौ_निक्षिप_throttled_task(p))
		प्रत्यागम;

	/*
	 * The code below (indirectly) updates schedutil which looks at
	 * the cfs_rq utilization to select a frequency.
	 * Let's add the task's estimated utilization to the cfs_rq's
	 * estimated utilization, before we update schedutil.
	 */
	यदि (!p->se.sched_delayed || (flags & पङ्क्तौ_निक्षिप_DELAYED))
		util_est_पङ्क्तौ_निक्षिप(cfs_rq, p);

	update_curr_eevdf(cfs_rq);

	यदि (flags & पङ्क्तौ_निक्षिप_DELAYED) {
		requeue_delayed_entity(cfs_rq, se);
		प्रत्यागम;
	}

	/*
	 * If in_iowait is set, the code below may not trigger any cpufreq
	 * utilization updates, so do it here explicitly with the IOWAIT flag
	 * passed.
	 */
	यदि (p->अन्तर्_iowait)
		cpufreq_update_util(rq, SCHED_CPUFREQ_IOWAIT);

	curr = (cfs_rq->curr == se);
	/*
	 * This can actually happen with CFS_BANDWIDTH, a task can be throttled
	 * and dequeued, unthrottled from another CPU and then enqueued again
	 * before schedule().
	 */
	यदि (curr)
		place_entity(cfs_rq, se, flags);

	यदि (se->on_rq && se->sched_delayed)
		requeue_delayed_entity(cfs_rq, se);

	weight = पङ्क्तौ_निक्षिप_hierarchy(p, flags);

	यदि (!curr) {
		reweight_eevdf(cfs_rq, se, weight, false);
		place_entity(cfs_rq, se, flags | पङ्क्तौ_निक्षिप_QUEUED);
		__enqueue_entity(cfs_rq, se);
	}

	यदि (!rq_h_nr_queued && rq->cfs.h_nr_queued)
		dl_server_start(&rq->fair_server);

	/* At this point se is NULL and we are at root level*/
	सङ्कलय_nr_running(rq, 1);

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
	यदि (!task_new)
		check_update_overutilized_स्थितिः(rq);

	assert_list_पर्णम्_cfs_rq(rq);

	hrtick_update(rq);
}

स्थिर शून्य पङ्क्तेः_निष्कासय_hierarchy(संरचना task_struct *p, पूर्णाङ्क flags)
{
	संरचना sched_entity *se = &p->se;
	bool task_sleep = flags & पङ्क्तेः_निष्कासय_SLEEP;
	bool task_delayed = flags & पङ्क्तेः_निष्कासय_DELAYED;
	bool task_throttled = flags & पङ्क्तेः_निष्कासय_THROTTLE;
	पूर्णाङ्क h_nr_runnable = 0;
	पूर्णाङ्क h_nr_idle = task_has_idle_policy(p);
	bool पङ्क्तेः_निष्कासय = true;

	यदि (task_sleep || task_delayed || !se->sched_delayed)
		h_nr_runnable = 1;

	for_each_sched_entity(se) {
		संरचना cfs_rq *cfs_rq = cfs_rq_सम्बन्धि(se);

		update_curr(cfs_rq);

		यदि (पङ्क्तेः_निष्कासय) {
			पङ्क्तेः_निष्कासय_entity(cfs_rq, se, flags);
			/* Don't dequeue parent if it has other entities besides us */
			यदि (cfs_rq->आरोपय.weight)
				पङ्क्तेः_निष्कासय = false;
		} अन्यथा {
			update_आरोपय_avg(cfs_rq, se, UPDATE_TG);
			se_update_runnable(se);
			update_cfs_गणः(se);
		}

		cfs_rq->h_nr_runnable -= h_nr_runnable;
		cfs_rq->h_nr_queued--;
		cfs_rq->h_nr_idle -= h_nr_idle;

		यदि (cfs_rq_is_idle(cfs_rq))
			h_nr_idle = 1;

		यदि (throttled_hierarchy(cfs_rq) && task_throttled)
			लेखः_throttle_clock(cfs_rq);

		flags |= पङ्क्तेः_निष्कासय_SLEEP;
		flags &= ~(पङ्क्तेः_निष्कासय_DELAYED | पङ्क्तेः_निष्कासय_SPECIAL);
	}
}

/*
 * The part of dequeue_task_fair() that is needed to dequeue delayed tasks.
 *
 * Returns:
 *   true  - dequeued
 *   false - delayed
 */
स्थिर bool __dequeue_task(संरचना rq *rq, संरचना task_struct *p, पूर्णाङ्क flags)
{
	संरचना sched_entity *se = &p->se;
	संरचना cfs_rq *cfs_rq = &rq->cfs;
	bool was_sched_idle = sched_idle_rq(rq);
	bool task_sleep = flags & पङ्क्तेः_निष्कासय_SLEEP;
	bool task_delayed = flags & पङ्क्तेः_निष्कासय_DELAYED;

	clear_buddies(cfs_rq, se);

	update_curr_eevdf(cfs_rq);
	update_entity_lag(cfs_rq, se);

	यदि (flags & पङ्क्तेः_निष्कासय_DELAYED) {
		WARN_ON_ONCE(!se->sched_delayed);
	} अन्यथा {
		bool delay = task_sleep;
		/*
		 * DELAY_DEQUEUE relies on spurious wakeups, special task
		 * states must not suffer spurious wakeups, excempt them.
		 */
		यदि (flags & (पङ्क्तेः_निष्कासय_SPECIAL | पङ्क्तेः_निष्कासय_THROTTLE))
			delay = false;

		WARN_ON_ONCE(delay && se->sched_delayed);

		यदि (sched_feat(DELAY_पङ्क्तेः_निष्कासय) && delay &&
		    !entity_eligible(cfs_rq, se)) {
			update_आरोपय_avg(cfs_rq_सम्बन्धि(se), se, UPDATE_UTIL_EST);
			समुच्चयः_delayed(se);
			प्रत्यागम false;
		}
	}

	पङ्क्तेः_निष्कासय_hierarchy(p, flags);

	यदि (sched_feat(PLACE_REL_DEADLINE) && !task_sleep) {
		se->deadline -= se->vruntime;
		se->rel_deadline = 1;
	}
	यदि (se != cfs_rq->curr)
		__dequeue_entity(cfs_rq, se);

	sub_nr_running(rq, 1);

	/* balance early to pull high priority tasks */
	यदि (unlikely(!was_sched_idle && sched_idle_rq(rq)))
		rq->अग्र_balance = jiffies;

	यदि (task_delayed) {
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

	प्रत्यागम true;
}

/*
 * The dequeue_task method is called before nr_running is
 * decreased. We remove the task from the rbtree and
 * update the fair scheduling stats:
 */
स्थिर bool पङ्क्तेः_निष्कासय_task_fair(संरचना rq *rq, संरचना task_struct *p, पूर्णाङ्क flags)
{
	यदि (task_is_throttled(p)) {
		पङ्क्तेः_निष्कासय_throttled_task(p, flags);
		प्रत्यागम true;
	}

	यदि (!p->se.sched_delayed)
		util_est_पङ्क्तेः_निष्कासय(&rq->cfs, p);

	यदि (!__dequeue_task(rq, p, flags))
		प्रत्यागम false;

	/*
	 * Must not reference @p after __dequeue_task(DEQUEUE_DELAYED).
	 */
	प्रत्यागम true;
}

स्थिर अन्तःस्थ अचिह्नित पूर्णाङ्क cfs_h_nr_delayed(संरचना rq *rq)
{
	प्रत्यागम (rq->cfs.h_nr_queued - rq->cfs.h_nr_runnable);
}

/* Working cpumask for: sched_balance_rq(), sched_balance_newidle(). */
स्थिर DEFINE_प्रत्येकम्_CPU(cpumask_var_t, आरोपय_balance_आवरणम्);
स्थिर DEFINE_प्रत्येकम्_CPU(cpumask_var_t, select_rq_आवरणम्);
स्थिर DEFINE_प्रत्येकम्_CPU(cpumask_var_t, should_we_balance_tmpmask);

#यदि_परिभाषितम् CONFIG_NO_HZ_COMMON

स्थिर संरचना {
	cpumask_var_t idle_cpus_आवरणम्;
	पूर्णाङ्क has_blocked_आरोपय;		/* Idle CPUS has blocked load */
	पूर्णाङ्क needs_update;		/* Newly idle CPUs need their next_balance collated */
	अचिह्नित दीर्घ अग्र_balance;     /* in jiffy units */
	अचिह्नित दीर्घ अग्र_blocked;	/* Next update of blocked load in jiffies */
} nohz ____cacheline_aligned;

#यदि_अन्तः /* CONFIG_NO_HZ_COMMON */

स्थिर अचिह्नित दीर्घ cpu_आरोपय(संरचना rq *rq)
{
	प्रत्यागम cfs_rq_आरोपय_avg(&rq->cfs);
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
स्थिर अचिह्नित दीर्घ cpu_आरोपय_without(संरचना rq *rq, संरचना task_struct *p)
{
	संरचना cfs_rq *cfs_rq;
	अचिह्नित पूर्णाङ्क आरोपय;

	/* Task has no contribution or is new */
	यदि (cpu_सम्बन्धि(rq) != task_cpu(p) || !READ_ONCE(p->se.avg.चरमः_update_time))
		प्रत्यागम cpu_आरोपय(rq);

	cfs_rq = &rq->cfs;
	आरोपय = READ_ONCE(cfs_rq->avg.आरोपय_avg);

	/* Discount task's util from CPU's util */
	lsub_धनात्मकम्(&आरोपय, task_h_आरोपय(p));

	प्रत्यागम आरोपय;
}

स्थिर अचिह्नित दीर्घ cpu_runnable(संरचना rq *rq)
{
	प्रत्यागम cfs_rq_runnable_avg(&rq->cfs);
}

स्थिर अचिह्नित दीर्घ cpu_runnable_without(संरचना rq *rq, संरचना task_struct *p)
{
	संरचना cfs_rq *cfs_rq;
	अचिह्नित पूर्णाङ्क runnable;

	/* Task has no contribution or is new */
	यदि (cpu_सम्बन्धि(rq) != task_cpu(p) || !READ_ONCE(p->se.avg.चरमः_update_time))
		प्रत्यागम cpu_runnable(rq);

	cfs_rq = &rq->cfs;
	runnable = READ_ONCE(cfs_rq->avg.runnable_avg);

	/* Discount task's runnable from CPU's runnable */
	lsub_धनात्मकम्(&runnable, p->se.avg.runnable_avg);

	प्रत्यागम runnable;
}

स्थिर अचिह्नित दीर्घ क्षमता_सम्बन्धि(पूर्णाङ्क cpu)
{
	प्रत्यागम cpu_rq(cpu)->cpu_क्षमता;
}

स्थिर शून्य लेखः_wakee(संरचना task_struct *p)
{
	/*
	 * Only decay a single time; tasks that have less then 1 wakeup per
	 * jiffy will not have built up many flips.
	 */
	यदि (time_after(jiffies, current->wakee_flip_decay_ts + HZ)) {
		current->wakee_flips >>= 1;
		current->wakee_flip_decay_ts = jiffies;
	}

	यदि (current->चरमः_wakee != p) {
		current->चरमः_wakee = p;
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
स्थिर पूर्णाङ्क wake_wide(संरचना task_struct *p)
{
	अचिह्नित पूर्णाङ्क master = current->wakee_flips;
	अचिह्नित पूर्णाङ्क slave = p->wakee_flips;
	पूर्णाङ्क गुणकः = __this_cpu_read(sd_llc_आकार);

	यदि (master < slave)
		swap(master, slave);
	यदि (slave < गुणकः || master < slave * गुणकः)
		प्रत्यागम 0;
	प्रत्यागम 1;
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
स्थिर पूर्णाङ्क
wake_affine_idle(पूर्णाङ्क this_cpu, पूर्णाङ्क पूर्व_cpu, पूर्णाङ्क sync)
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
	यदि (उपलब्धम्_idle_cpu(this_cpu) && cpus_share_cache(this_cpu, पूर्व_cpu))
		प्रत्यागम उपलब्धम्_idle_cpu(पूर्व_cpu) ? पूर्व_cpu : this_cpu;

	यदि (sync) {
		संरचना rq *rq = cpu_rq(this_cpu);

		यदि ((rq->nr_running - cfs_h_nr_delayed(rq)) == 1)
			प्रत्यागम this_cpu;
	}

	यदि (उपलब्धम्_idle_cpu(पूर्व_cpu))
		प्रत्यागम पूर्व_cpu;

	प्रत्यागम nr_cpumask_द्वयङ्काः;
}

स्थिर पूर्णाङ्क
wake_affine_weight(संरचना sched_domain *sd, संरचना task_struct *p,
		   पूर्णाङ्क this_cpu, पूर्णाङ्क पूर्व_cpu, पूर्णाङ्क sync)
{
	s64 this_eff_आरोपय, पूर्व_eff_आरोपय;
	अचिह्नित दीर्घ task_आरोपय;

	this_eff_आरोपय = cpu_आरोपय(cpu_rq(this_cpu));

	यदि (sync) {
		अचिह्नित दीर्घ current_आरोपय = task_h_आरोपय(current);

		यदि (current_आरोपय > this_eff_आरोपय)
			प्रत्यागम this_cpu;

		this_eff_आरोपय -= current_आरोपय;
	}

	task_आरोपय = task_h_आरोपय(p);

	this_eff_आरोपय += task_आरोपय;
	यदि (sched_feat(WA_BIAS))
		this_eff_आरोपय *= 100;
	this_eff_आरोपय *= क्षमता_सम्बन्धि(पूर्व_cpu);

	पूर्व_eff_आरोपय = cpu_आरोपय(cpu_rq(पूर्व_cpu));
	पूर्व_eff_आरोपय -= task_आरोपय;
	यदि (sched_feat(WA_BIAS))
		पूर्व_eff_आरोपय *= 100 + (sd->imbalance_pct - 100) / 2;
	पूर्व_eff_आरोपय *= क्षमता_सम्बन्धि(this_cpu);

	/*
	 * If sync, adjust the weight of prev_eff_load such that if
	 * prev_eff == this_eff that select_idle_sibling() will consider
	 * stacking the wakee on top of the waker if no other CPU is
	 * idle.
	 */
	यदि (sync)
		पूर्व_eff_आरोपय += 1;

	प्रत्यागम this_eff_आरोपय < पूर्व_eff_आरोपय ? this_cpu : nr_cpumask_द्वयङ्काः;
}

स्थिर पूर्णाङ्क wake_affine(संरचना sched_domain *sd, संरचना task_struct *p,
		       पूर्णाङ्क this_cpu, पूर्णाङ्क पूर्व_cpu, पूर्णाङ्क sync)
{
	पूर्णाङ्क target = nr_cpumask_द्वयङ्काः;

	यदि (sched_feat(WA_IDLE))
		target = wake_affine_idle(this_cpu, पूर्व_cpu, sync);

	यदि (sched_feat(WA_WEIGHT) && target == nr_cpumask_द्वयङ्काः)
		target = wake_affine_weight(sd, p, this_cpu, पूर्व_cpu, sync);

	schedstat_inc(p->stats.nr_wakeups_affine_attempts);
	यदि (target != this_cpu)
		प्रत्यागम पूर्व_cpu;

	schedstat_inc(sd->ttwu_move_affine);
	schedstat_inc(p->stats.nr_wakeups_affine);
	प्रत्यागम target;
}

स्थिर संरचना sched_गणः *
sched_balance_find_dst_गणः(संरचना sched_domain *sd, संरचना task_struct *p, पूर्णाङ्क this_cpu);

/*
 * sched_balance_find_dst_group_cpu - find the idlest CPU among the CPUs in the group.
 */
स्थिर पूर्णाङ्क
sched_balance_find_dst_गणः_cpu(संरचना sched_गणः *गणः, संरचना task_struct *p, पूर्णाङ्क this_cpu)
{
	अचिह्नित दीर्घ आरोपय, min_आरोपय = ULONG_MAX;
	u64 min_exit_latency = U64_MAX;
	अचिह्नित पूर्णाङ्क nr_candidates = 0;
	पूर्णाङ्क न्यूनतमम्_loaded_cpu = this_cpu;
	पूर्णाङ्क shallowest_idle_cpu = -1;
	पूर्णाङ्क i;

	/* Check if we have any choice: */
	यदि (गणः->गणः_weight == 1)
		प्रत्यागम cpumask_first(sched_गणः_विस्तृतिः(गणः));

	/* Traverse only the allowed CPUs */
	for_each_cpu_and(i, sched_गणः_विस्तृतिः(गणः), p->cpus_ptr) {
		संरचना rq *rq = cpu_rq(i);

		यदि (!sched_core_cookie_मेलनम्(rq, p))
			अनुवर्त;

		यदि (वृणु_sched_idle_rq(rq, p))
			प्रत्यागम i;

		यदि (उपलब्धम्_idle_cpu(i)) {
			संरचना cpuidle_state *idle = idle_get_state(rq);
			u64 exit_latency = idle ? idle->exit_latency : U64_MAX;

			यदि (shallowest_idle_cpu == -1 || exit_latency < min_exit_latency) {
				min_exit_latency = exit_latency;
				shallowest_idle_cpu = i;
				nr_candidates = 1;
			} अन्यथा यदि (exit_latency == min_exit_latency) {
				nr_candidates++;
				यदि (!reciprocal_scale(sched_rng(), nr_candidates))
					shallowest_idle_cpu = i;
			}
		} अन्यथा यदि (shallowest_idle_cpu == -1) {
			आरोपय = cpu_आरोपय(cpu_rq(i));
			यदि (आरोपय < min_आरोपय) {
				min_आरोपय = आरोपय;
				न्यूनतमम्_loaded_cpu = i;
			}
		}
	}

	प्रत्यागम shallowest_idle_cpu != -1 ? shallowest_idle_cpu : न्यूनतमम्_loaded_cpu;
}

स्थिर अन्तःस्थ पूर्णाङ्क sched_balance_find_dst_cpu(संरचना sched_domain *sd, संरचना task_struct *p,
				  पूर्णाङ्क cpu, पूर्णाङ्क पूर्व_cpu, पूर्णाङ्क sd_flag)
{
	पूर्णाङ्क new_cpu = cpu;

	यदि (!cpumask_intersects(sched_domain_विस्तृतिः(sd), p->cpus_ptr))
		प्रत्यागम पूर्व_cpu;

	/*
	 * We need task's util for cpu_util_without, sync it up to
	 * prev_cpu's last_update_time.
	 */
	यदि (!(sd_flag & SD_BALANCE_FORK))
		sync_entity_आरोपय_avg(&p->se);

	यावत् (sd) {
		संरचना sched_गणः *गणः;
		संरचना sched_domain *tmp;
		पूर्णाङ्क weight;

		यदि (!(sd->flags & sd_flag)) {
			sd = sd->child;
			अनुवर्त;
		}

		गणः = sched_balance_find_dst_गणः(sd, p, cpu);
		यदि (!गणः) {
			sd = sd->child;
			अनुवर्त;
		}

		new_cpu = sched_balance_find_dst_गणः_cpu(गणः, p, cpu);
		यदि (new_cpu == cpu) {
			/* Now try balancing at a lower domain level of 'cpu': */
			sd = sd->child;
			अनुवर्त;
		}

		/* Now try balancing at a lower domain level of 'new_cpu': */
		cpu = new_cpu;
		weight = sd->विस्तृतिः_weight;
		sd = रिक्त;
		for_each_domain(cpu, tmp) {
			यदि (weight <= tmp->विस्तृतिः_weight)
				भङ्ग;
			यदि (tmp->flags & sd_flag)
				sd = tmp;
		}
	}

	प्रत्यागम new_cpu;
}

स्थिर अन्तःस्थ पूर्णाङ्क __select_idle_cpu(पूर्णाङ्क cpu, संरचना task_struct *p)
{
	यदि (वृणु_idle_cpu(cpu, p) && sched_cpu_cookie_मेलनम्(cpu_rq(cpu), p))
		प्रत्यागम cpu;

	प्रत्यागम -1;
}

DEFINE_STATIC_कुञ्चिका_FALSE(sched_smt_present);
EXPORT_चिह्नम्_GPL(sched_smt_present);

स्थिर अन्तःस्थ शून्य समुच्चयः_idle_cores(पूर्णाङ्क cpu, पूर्णाङ्क val)
{
	संरचना sched_domain_shared *sds;

	sds = rcu_dereference_all(प्रत्येकम्_cpu(sd_balance_shared, cpu));
	यदि (sds)
		WRITE_ONCE(sds->has_idle_cores, val);
}

स्थिर अन्तःस्थ bool test_idle_cores(पूर्णाङ्क cpu)
{
	संरचना sched_domain_shared *sds;

	sds = rcu_dereference_all(प्रत्येकम्_cpu(sd_balance_shared, cpu));
	यदि (sds)
		प्रत्यागम READ_ONCE(sds->has_idle_cores);

	प्रत्यागम false;
}

/*
 * Redirect a CPU to a higher-priority available sibling in its SMT domain,
 * subject to task affinity.
 */
स्थिर अन्तःस्थ पूर्णाङ्क select_idle_smt_cpu(संरचना task_struct *p, पूर्णाङ्क cpu)
{
	संरचना sched_domain *sd;
	पूर्णाङ्क best = cpu;
	पूर्णाङ्क sibling;

	यदि (!sched_smt_सक्रिय())
		प्रत्यागम cpu;

	sd = rcu_dereference_all(cpu_rq(cpu)->sd);
	यदि (!sd || !(sd->flags & SD_SHARE_CPUCAPACITY) ||
	    !(sd->flags & SD_ASYM_PACKING))
		प्रत्यागम cpu;

	for_each_cpu_and(sibling, sched_domain_विस्तृतिः(sd), p->cpus_ptr) {
		यदि (sibling == best || !वृणु_idle_cpu(sibling, p))
			अनुवर्त;

		यदि (sched_asym_prefer(sibling, best))
			best = sibling;
	}

	प्रत्यागम best;
}

/*
 * Scans the local SMT mask to see if the entire core is idle, and records this
 * information in sd_balance_shared->has_idle_cores.
 *
 * Since SMT siblings share all cache levels, inspecting this limited remote
 * state should be fairly cheap.
 */
शून्य __update_idle_core(संरचना rq *rq)
{
	पूर्णाङ्क core = cpu_सम्बन्धि(rq);
	पूर्णाङ्क cpu;

	rcu_read_तालकम्();
	यदि (test_idle_cores(core))
		गच्छ unlock;

	for_each_cpu(cpu, cpu_smt_आवरणम्(core)) {
		यदि (cpu == core)
			अनुवर्त;

		यदि (!उपलब्धम्_idle_cpu(cpu))
			गच्छ unlock;
	}

	समुच्चयः_idle_cores(core, 1);
unlock:
	rcu_read_unlock();
}

/*
 * Scan the entire LLC domain for idle cores; this dynamically switches off if
 * there are no idle cores left in the system; tracked through
 * sd_balance_shared->has_idle_cores and enabled through update_idle_core()
 * above.
 */
स्थिर पूर्णाङ्क select_idle_core(संरचना task_struct *p, पूर्णाङ्क core, संरचना cpumask *cpus, पूर्णाङ्क *idle_cpu)
{
	bool idle = true;
	पूर्णाङ्क cpu;

	for_each_cpu(cpu, cpu_smt_आवरणम्(core)) {
		यदि (!उपलब्धम्_idle_cpu(cpu)) {
			idle = false;
			यदि (*idle_cpu == -1) {
				यदि (वृणु_sched_idle_rq(cpu_rq(cpu), p) &&
				    cpumask_test_cpu(cpu, cpus)) {
					*idle_cpu = cpu;
					भङ्ग;
				}
				अनुवर्त;
			}
			भङ्ग;
		}
		यदि (*idle_cpu == -1 && cpumask_test_cpu(cpu, cpus))
			*idle_cpu = cpu;
	}

	यदि (idle)
		प्रत्यागम core;

	cpumask_andnot(cpus, cpus, cpu_smt_आवरणम्(core));
	प्रत्यागम -1;
}

/*
 * Scan the local SMT mask for idle CPUs.
 */
स्थिर पूर्णाङ्क select_idle_smt(संरचना task_struct *p, संरचना sched_domain *sd, पूर्णाङ्क target)
{
	पूर्णाङ्क cpu;

	for_each_cpu_and(cpu, cpu_smt_आवरणम्(target), p->cpus_ptr) {
		यदि (cpu == target)
			अनुवर्त;
		/*
		 * Check if the CPU is in the LLC scheduling domain of @target.
		 * Due to isolcpus, there is no guarantee that all the siblings are in the domain.
		 */
		यदि (!cpumask_test_cpu(cpu, sched_domain_विस्तृतिः(sd)))
			अनुवर्त;
		यदि (वृणु_idle_cpu(cpu, p))
			प्रत्यागम cpu;
	}

	प्रत्यागम -1;
}

/*
 * Scan the LLC domain for idle CPUs; this is dynamically regulated by
 * comparing the average scan cost (tracked in sd->avg_scan_cost) against the
 * average idle time for this rq (as found in rq->avg_idle).
 */
स्थिर पूर्णाङ्क select_idle_cpu(संरचना task_struct *p, संरचना sched_domain *sd, bool has_idle_core, पूर्णाङ्क target)
{
	संरचना cpumask *cpus = this_cpu_cpumask_var_ptr(select_rq_आवरणम्);
	पूर्णाङ्क i, cpu, idle_cpu = -1, nr = अधिकतम_पूर्णाङ्कः;

	यदि (sched_feat(SIS_UTIL) && sd->shared) {
		/*
		 * Increment because !--nr is the condition to stop scan.
		 *
		 * Since "sd" is "sd_llc" for target CPU dereferenced in the
		 * caller, it is safe to directly dereference "sd->shared".
		 * Topology bits always ensure it assigned for "sd_llc" abd it
		 * cannot disappear as long as we have a RCU protected
		 * reference to one the associated "sd" here.
		 */
		nr = READ_ONCE(sd->shared->nr_idle_अवलोकनम्) + 1;
		/* overloaded LLC is unlikely to have idle cpu/core */
		यदि (nr == 1)
			प्रत्यागम -1;
	}

	यदि (!cpumask_and(cpus, sched_domain_विस्तृतिः(sd), p->cpus_ptr))
		प्रत्यागम -1;

	यदि (static_branch_unlikely(&sched_cluster_सक्रिय)) {
		संरचना sched_गणः *sg = sd->groups;

		यदि (sg->flags & SD_CLUSTER) {
			for_each_cpu_wrap(cpu, sched_गणः_विस्तृतिः(sg), target + 1) {
				यदि (!cpumask_test_cpu(cpu, cpus))
					अनुवर्त;

				यदि (has_idle_core) {
					i = select_idle_core(p, cpu, cpus, &idle_cpu);
					यदि ((अचिह्नित पूर्णाङ्क)i < nr_cpumask_द्वयङ्काः)
						प्रत्यागम i;
				} अन्यथा {
					यदि (--nr <= 0)
						प्रत्यागम -1;
					idle_cpu = __select_idle_cpu(cpu, p);
					यदि ((अचिह्नित पूर्णाङ्क)idle_cpu < nr_cpumask_द्वयङ्काः)
						प्रत्यागम idle_cpu;
				}
			}
			cpumask_andnot(cpus, cpus, sched_गणः_विस्तृतिः(sg));
		}
	}

	for_each_cpu_wrap(cpu, cpus, target + 1) {
		यदि (has_idle_core) {
			i = select_idle_core(p, cpu, cpus, &idle_cpu);
			यदि ((अचिह्नित पूर्णाङ्क)i < nr_cpumask_द्वयङ्काः)
				प्रत्यागम i;

		} अन्यथा {
			यदि (--nr <= 0)
				प्रत्यागम -1;
			idle_cpu = __select_idle_cpu(cpu, p);
			यदि ((अचिह्नित पूर्णाङ्क)idle_cpu < nr_cpumask_द्वयङ्काः)
				भङ्ग;
		}
	}

	यदि (has_idle_core)
		समुच्चयः_idle_cores(target, false);

	प्रत्यागम idle_cpu;
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
गणना asym_माति_state {
	ASYM_IDLE_UCLAMP_MISFIT = -4,
	ASYM_IDLE_COMPLETE_MISFIT,
	ASYM_IDLE_THREAD_माति,
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
स्थिर पूर्णाङ्क
select_idle_क्षमता(संरचना task_struct *p, संरचना sched_domain *sd, पूर्णाङ्क target)
{
	/*
	 * On !SMT systems, has_idle_core is always false and preferred_core
	 * is always true (CPU == core), so the SMT preference logic below
	 * collapses to the plain capacity scan.
	 */
	bool has_idle_core = sched_smt_सक्रिय() && test_idle_cores(target);
	अचिह्नित दीर्घ task_util, util_min, util_max, best_cap = 0;
	पूर्णाङ्क माति, best_माति = ASYM_IDLE_THREAD_MISFIT;
	पूर्णाङ्क cpu, best_cpu = -1;
	संरचना cpumask *cpus;
	पूर्णाङ्क nr = अधिकतम_पूर्णाङ्कः;

	cpus = this_cpu_cpumask_var_ptr(select_rq_आवरणम्);
	cpumask_and(cpus, sched_domain_विस्तृतिः(sd), p->cpus_ptr);

	task_util = task_util_est(p);
	util_min = uclamp_eff_मानम्(p, UCLAMP_MIN);
	util_max = uclamp_eff_मानम्(p, UCLAMP_MAX);

	यदि (sched_feat(SIS_UTIL) && sd->shared) {
		/*
		 * Same nr_idle_scan hint as select_idle_cpu(), nr only limits
		 * the scan when not preferring an idle core.
		 */
		nr = READ_ONCE(sd->shared->nr_idle_अवलोकनम्) + 1;
		/* overloaded domain is unlikely to have idle cpu/core */
		यदि (nr == 1)
			प्रत्यागम -1;
	}

	for_each_cpu_wrap(cpu, cpus, target) {
		bool preferred_core = !has_idle_core || is_core_idle(cpu);
		अचिह्नित दीर्घ cpu_cap = क्षमता_सम्बन्धि(cpu);

		/*
		 * Stop when the nr_idle_scan is exhausted (mirrors
		 * select_idle_cpu() logic).
		 */
		यदि (!has_idle_core && --nr <= 0)
			प्रत्यागम best_cpu;

		यदि (!वृणु_idle_cpu(cpu, p))
			अनुवर्त;

		माति = util_माति_cpu(task_util, util_min, util_max, cpu);

		/*
		 * Perfect fit: capacity satisfies util + uclamp and the CPU
		 * sits on a fully-idle SMT core, this is a !SMT system, or
		 * there is no idle core to find.
		 * Short-circuit the rank-based selection and return
		 * immediately.
		 */
		यदि (माति > 0 && preferred_core)
			प्रत्यागम cpu;
		/*
		 * Only the min performance hint (i.e. uclamp_min) doesn't fit.
		 * Look for the CPU with best capacity.
		 */
		अन्यथा यदि (माति < 0)
			cpu_cap = get_actual_cpu_क्षमता(cpu);
		/*
		 * fits > 0 implies we are not on a preferred core, but the util
		 * fits CPU capacity. Set fits to ASYM_IDLE_THREAD_FITS
		 * so the effective range becomes
		 * [ASYM_IDLE_THREAD_FITS, ASYM_IDLE_THREAD_MISFIT], where:
		 *    ASYM_IDLE_THREAD_MISFIT - does not fit
		 *    ASYM_IDLE_THREAD_UCLAMP_MISFIT - fits with the exception of UCLAMP_MIN
		 *    ASYM_IDLE_THREAD_FITS - fits with the exception of preferred_core
		 */
		अन्यथा यदि (माति > 0)
			माति = ASYM_IDLE_THREAD_माति;

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
		यदि (preferred_core)
			माति += ASYM_IDLE_CORE_BIAS;

		/*
		 * First, select CPU which fits better (lower is more preferred).
		 * Then, select the one with best capacity at same level.
		 */
		यदि ((माति < best_माति) ||
		    ((माति == best_माति) && (cpu_cap > best_cap))) {
			best_cap = cpu_cap;
			best_cpu = cpu;
			best_माति = माति;
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
	यदि (has_idle_core && best_माति > ASYM_IDLE_COMPLETE_MISFIT)
		समुच्चयः_idle_cores(target, false);

	प्रत्यागम best_cpu;
}

स्थिर अन्तःस्थ bool asym_माति_cpu(अचिह्नित दीर्घ util,
				 अचिह्नित दीर्घ util_min,
				 अचिह्नित दीर्घ util_max,
				 पूर्णाङ्क cpu)
{
	यदि (sched_asym_cpucap_सक्रिय()) {
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
		प्रत्यागम (!sched_smt_सक्रिय() || is_core_idle(cpu)) &&
		       (util_माति_cpu(util, util_min, util_max, cpu) > 0);
	}

	प्रत्यागम true;
}

/*
 * Try and locate an idle core/thread in the LLC cache domain.
 */
स्थिर पूर्णाङ्क select_idle_sibling(संरचना task_struct *p, पूर्णाङ्क पूर्व, पूर्णाङ्क target)
{
	bool has_idle_core = false;
	संरचना sched_domain *sd;
	अचिह्नित दीर्घ task_util, util_min, util_max;
	पूर्णाङ्क i, recent_प्रयुक्तम्_cpu, पूर्व_aff = -1;

	/*
	 * On asymmetric system, update task utilization because we will check
	 * that the task fits with CPU's capacity.
	 */
	यदि (sched_asym_cpucap_सक्रिय()) {
		sync_entity_आरोपय_avg(&p->se);
		task_util = task_util_est(p);
		util_min = uclamp_eff_मानम्(p, UCLAMP_MIN);
		util_max = uclamp_eff_मानम्(p, UCLAMP_MAX);
	}

	/*
	 * per-cpu select_rq_mask usage
	 */
	lockdep_assert_irqs_disabled();

	यदि (वृणु_idle_cpu(target, p) &&
	    asym_माति_cpu(task_util, util_min, util_max, target))
		गच्छ select_smt_प्राथमिकता;

	/*
	 * If the previous CPU is cache affine and idle, don't be stupid:
	 */
	यदि (पूर्व != target && cpus_share_cache(पूर्व, target) &&
	    वृणु_idle_cpu(पूर्व, p) &&
	    asym_माति_cpu(task_util, util_min, util_max, पूर्व)) {

		यदि (!static_branch_unlikely(&sched_cluster_सक्रिय) ||
		    cpus_share_resources(पूर्व, target)) {
			target = पूर्व;
			गच्छ select_smt_प्राथमिकता;
		}

		पूर्व_aff = पूर्व;
	}

	/*
	 * Allow a per-cpu kthread to stack with the wakee if the
	 * kworker thread and the tasks previous CPUs are the same.
	 * The assumption is that the wakee queued work for the
	 * per-cpu kthread that is now complete and the wakeup is
	 * essentially a sync wakeup. An obvious example of this
	 * pattern is IO completions.
	 */
	यदि (is_प्रत्येकम्_cpu_kthread(current) &&
	    अन्तर्_task() &&
	    पूर्व == smp_processor_id() &&
	    this_rq()->nr_running <= 1 &&
	    asym_माति_cpu(task_util, util_min, util_max, पूर्व)) {
		target = पूर्व;
		गच्छ select_smt_प्राथमिकता;
	}

	/* Check a recently used CPU as a potential idle candidate: */
	recent_प्रयुक्तम्_cpu = p->recent_प्रयुक्तम्_cpu;
	p->recent_प्रयुक्तम्_cpu = पूर्व;
	यदि (recent_प्रयुक्तम्_cpu != पूर्व &&
	    recent_प्रयुक्तम्_cpu != target &&
	    cpus_share_cache(recent_प्रयुक्तम्_cpu, target) &&
	    वृणु_idle_cpu(recent_प्रयुक्तम्_cpu, p) &&
	    cpumask_test_cpu(recent_प्रयुक्तम्_cpu, p->cpus_ptr) &&
	    asym_माति_cpu(task_util, util_min, util_max, recent_प्रयुक्तम्_cpu)) {

		यदि (!static_branch_unlikely(&sched_cluster_सक्रिय) ||
		    cpus_share_resources(recent_प्रयुक्तम्_cpu, target)) {
			target = recent_प्रयुक्तम्_cpu;
			गच्छ select_smt_प्राथमिकता;
		}

	} अन्यथा {
		recent_प्रयुक्तम्_cpu = -1;
	}

	/*
	 * For asymmetric CPU capacity systems, our domain of interest is
	 * sd_asym_cpucapacity rather than sd_llc.
	 */
	यदि (sched_asym_cpucap_सक्रिय()) {
		sd = rcu_dereference_all(प्रत्येकम्_cpu(sd_asym_cpucapacity, target));
		/*
		 * On an asymmetric CPU capacity system where an exclusive
		 * cpuset defines a symmetric island (i.e. one unique
		 * capacity_orig value through the cpuset), the key will be set
		 * but the CPUs within that cpuset will not have a domain with
		 * SD_ASYM_CPUCAPACITY. These should follow the usual symmetric
		 * capacity path.
		 */
		यदि (sd) {
			i = select_idle_क्षमता(p, sd, target);
			यदि ((अचिह्नित पूर्णाङ्क)i < nr_cpumask_द्वयङ्काः) {
				target = i;
				गच्छ select_smt_प्राथमिकता;
			}
			प्रत्यागम target;
		}
	}

	sd = rcu_dereference_all(प्रत्येकम्_cpu(sd_llc, target));
	यदि (!sd)
		प्रत्यागम target;

	यदि (sched_smt_सक्रिय()) {
		has_idle_core = test_idle_cores(target);

		यदि (!has_idle_core && cpus_share_cache(पूर्व, target)) {
			i = select_idle_smt(p, sd, पूर्व);
			यदि ((अचिह्नित पूर्णाङ्क)i < nr_cpumask_द्वयङ्काः) {
				target = i;
				गच्छ select_smt_प्राथमिकता;
			}
		}
	}

	i = select_idle_cpu(p, sd, has_idle_core, target);
	यदि ((अचिह्नित पूर्णाङ्क)i < nr_cpumask_द्वयङ्काः) {
		target = i;
		गच्छ select_smt_प्राथमिकता;
	}

	/*
	 * For cluster machines which have lower sharing cache like L2 or
	 * LLC Tag, we tend to find an idle CPU in the target's cluster
	 * first. But prev_cpu or recent_used_cpu may also be a good candidate,
	 * use them if possible when no idle CPU found in select_idle_cpu().
	 */
	यदि ((अचिह्नित पूर्णाङ्क)पूर्व_aff < nr_cpumask_द्वयङ्काः) {
		target = पूर्व_aff;
		गच्छ select_smt_प्राथमिकता;
	}
	यदि ((अचिह्नित पूर्णाङ्क)recent_प्रयुक्तम्_cpu < nr_cpumask_द्वयङ्काः) {
		target = recent_प्रयुक्तम्_cpu;
		गच्छ select_smt_प्राथमिकता;
	}

	प्रत्यागम target;

select_smt_प्राथमिकता:
	प्रत्यागम select_idle_smt_cpu(p, target);
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
स्थिर अचिह्नित दीर्घ
cpu_util(पूर्णाङ्क cpu, संरचना task_struct *p, पूर्णाङ्क dst_cpu, पूर्णाङ्क boost)
{
	bool सङ्कलय_task = p && task_cpu(p) != cpu && dst_cpu == cpu;
	bool sub_task = p && task_cpu(p) == cpu && dst_cpu != cpu;
	संरचना cfs_rq *cfs_rq = &cpu_rq(cpu)->cfs;
	अचिह्नित दीर्घ util = READ_ONCE(cfs_rq->avg.util_avg);
	अचिह्नित दीर्घ runnable;

	/*
	 * If @dst_cpu is -1 or @p migrates from @cpu to @dst_cpu remove its
	 * contribution. If @p migrates from another CPU to @cpu add its
	 * contribution. In all the other cases @cpu is not impacted by the
	 * migration so its util_avg is already correct.
	 */
	यदि (सङ्कलय_task)
		util += task_util(p);
	अन्यथा यदि (sub_task)
		lsub_धनात्मकम्(&util, task_util(p));

	यदि (boost) {
		runnable = READ_ONCE(cfs_rq->avg.runnable_avg);
		यदि (सङ्कलय_task)
			runnable += READ_ONCE(p->se.avg.runnable_avg);
		अन्यथा यदि (sub_task)
			lsub_धनात्मकम्(&runnable,
				      READ_ONCE(p->se.avg.runnable_avg));
		util = max(util, runnable);
	}

	यदि (sched_feat(UTIL_EST)) {
		अचिह्नित दीर्घ util_est;

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
		यदि (dst_cpu == cpu)
			util_est += _task_util_est(p);
		अन्यथा यदि (p && unlikely(task_on_rq_queued(p) || current == p))
			lsub_धनात्मकम्(&util_est, _task_util_est(p));

		util = max(util, util_est);
	}

	प्रत्यागम min(util, arch_scale_cpu_क्षमता(cpu));
}

अचिह्नित दीर्घ cpu_util_cfs(पूर्णाङ्क cpu)
{
	प्रत्यागम cpu_util(cpu, रिक्त, -1, 0);
}

अचिह्नित दीर्घ cpu_util_cfs_boost(पूर्णाङ्क cpu)
{
	प्रत्यागम cpu_util(cpu, रिक्त, -1, 1);
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
स्थिर अचिह्नित दीर्घ cpu_util_without(पूर्णाङ्क cpu, संरचना task_struct *p)
{
	/* Task has no contribution or is new */
	यदि (cpu != task_cpu(p) || !READ_ONCE(p->se.avg.चरमः_update_time))
		p = रिक्त;

	प्रत्यागम cpu_util(cpu, p, -1, 0);
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
अचिह्नित दीर्घ effective_cpu_util(पूर्णाङ्क cpu, अचिह्नित दीर्घ util_cfs,
				 अचिह्नित दीर्घ *min,
				 अचिह्नित दीर्घ *max)
{
	अचिह्नित दीर्घ util, irq, scale;
	संरचना rq *rq = cpu_rq(cpu);

	scale = arch_scale_cpu_क्षमता(cpu);

	/*
	 * Early check to see if IRQ/steal time saturates the CPU, can be
	 * because of inaccuracies in how we track these -- see
	 * update_irq_load_avg().
	 */
	irq = cpu_util_irq(rq);
	यदि (unlikely(irq >= scale)) {
		यदि (min)
			*min = scale;
		यदि (max)
			*max = scale;
		प्रत्यागम scale;
	}

	यदि (min) {
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
		यदि (!uclamp_is_प्रयुक्तम्() && rt_rq_is_runnable(&rq->rt))
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
	यदि (max)
		*max = min(scale, uclamp_rq_get(rq, UCLAMP_MAX));

	यदि (util >= scale)
		प्रत्यागम scale;

	/*
	 * There is still idle time; further improve the number by using the
	 * IRQ metric. Because IRQ/steal time is hidden from the task clock we
	 * need to scale the task numbers:
	 *
	 *              max - irq
	 *   U' = irq + --------- * U
	 *                 max
	 */
	util = scale_irq_क्षमता(util, irq, scale);
	util += irq;

	प्रत्यागम min(scale, util);
}

अचिह्नित दीर्घ sched_cpu_util(पूर्णाङ्क cpu)
{
	प्रत्यागम effective_cpu_util(cpu, cpu_util_cfs(cpu), रिक्त, रिक्त);
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
संरचना energy_env {
	अचिह्नित दीर्घ task_busy_time;
	अचिह्नित दीर्घ pd_busy_time;
	अचिह्नित दीर्घ cpu_cap;
	अचिह्नित दीर्घ pd_cap;
};

/*
 * Compute the task busy time for compute_energy(). This time cannot be
 * injected directly into effective_cpu_util() because of the IRQ scaling.
 * The latter only makes sense with the most recent CPUs where the task has
 * run.
 */
स्थिर अन्तःस्थ शून्य eenv_task_busy_time(संरचना energy_env *eenv,
				       संरचना task_struct *p, पूर्णाङ्क पूर्व_cpu)
{
	अचिह्नित दीर्घ busy_time, max_cap = arch_scale_cpu_क्षमता(पूर्व_cpu);
	अचिह्नित दीर्घ irq = cpu_util_irq(cpu_rq(पूर्व_cpu));

	यदि (unlikely(irq >= max_cap))
		busy_time = max_cap;
	अन्यथा
		busy_time = scale_irq_क्षमता(task_util_est(p), irq, max_cap);

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
स्थिर अन्तःस्थ शून्य eenv_pd_busy_time(संरचना energy_env *eenv,
				     संरचना cpumask *pd_cpus,
				     संरचना task_struct *p)
{
	अचिह्नित दीर्घ busy_time = 0;
	पूर्णाङ्क cpu;

	for_each_cpu(cpu, pd_cpus) {
		अचिह्नित दीर्घ util = cpu_util(cpu, p, -1, 0);

		busy_time += effective_cpu_util(cpu, util, रिक्त, रिक्त);
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
स्थिर अन्तःस्थ अचिह्नित दीर्घ
eenv_pd_max_util(संरचना energy_env *eenv, संरचना cpumask *pd_cpus,
		 संरचना task_struct *p, पूर्णाङ्क dst_cpu)
{
	अचिह्नित दीर्घ max_util = 0;
	पूर्णाङ्क cpu;

	for_each_cpu(cpu, pd_cpus) {
		संरचना task_struct *tsk = (cpu == dst_cpu) ? p : रिक्त;
		अचिह्नित दीर्घ util = cpu_util(cpu, p, dst_cpu, 1);
		अचिह्नित दीर्घ eff_util, min, max;

		/*
		 * Performance domain frequency: utilization clamping
		 * must be considered since it affects the selection
		 * of the performance domain frequency.
		 * NOTE: in case RT tasks are running, by default the min
		 * utilization can be max OPP.
		 */
		eff_util = effective_cpu_util(cpu, util, &min, &max);

		/* Task's uclamp can modify min and max value */
		यदि (tsk && uclamp_is_प्रयुक्तम्()) {
			min = max(min, uclamp_eff_मानम्(p, UCLAMP_MIN));

			/*
			 * If there is no active max uclamp constraint,
			 * directly use task's one, otherwise keep max.
			 */
			यदि (uclamp_rq_is_idle(cpu_rq(cpu)))
				max = uclamp_eff_मानम्(p, UCLAMP_MAX);
			अन्यथा
				max = max(max, uclamp_eff_मानम्(p, UCLAMP_MAX));
		}

		eff_util = sugov_effective_cpu_perf(cpu, eff_util, min, max);
		max_util = max(max_util, eff_util);
	}

	प्रत्यागम min(max_util, eenv->cpu_cap);
}

/*
 * compute_energy(): Use the Energy Model to estimate the energy that @pd would
 * consume for a given utilization landscape @eenv. When @dst_cpu < 0, the task
 * contribution is ignored.
 */
स्थिर अन्तःस्थ अचिह्नित दीर्घ
compute_energy(संरचना energy_env *eenv, संरचना perf_domain *pd,
	       संरचना cpumask *pd_cpus, संरचना task_struct *p, पूर्णाङ्क dst_cpu)
{
	अचिह्नित दीर्घ max_util = eenv_pd_max_util(eenv, pd_cpus, p, dst_cpu);
	अचिह्नित दीर्घ busy_time = eenv->pd_busy_time;
	अचिह्नित दीर्घ energy;

	यदि (dst_cpu >= 0)
		busy_time = min(eenv->pd_cap, busy_time + eenv->task_busy_time);

	energy = em_cpu_energy(pd->em_pd, max_util, busy_time, eenv->cpu_cap);

	trace_sched_compute_energy_tp(p, dst_cpu, energy, max_util, busy_time);

	प्रत्यागम energy;
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
स्थिर पूर्णाङ्क find_energy_efficient_cpu(संरचना task_struct *p, पूर्णाङ्क पूर्व_cpu)
{
	संरचना cpumask *cpus = this_cpu_cpumask_var_ptr(select_rq_आवरणम्);
	अचिह्नित दीर्घ पूर्व_delta = ULONG_MAX, best_delta = ULONG_MAX;
	अचिह्नित दीर्घ p_util_min = uclamp_is_प्रयुक्तम्() ? uclamp_eff_मानम्(p, UCLAMP_MIN) : 0;
	अचिह्नित दीर्घ p_util_max = uclamp_is_प्रयुक्तम्() ? uclamp_eff_मानम्(p, UCLAMP_MAX) : 1024;
	संरचना मूलम्_domain *rd = this_rq()->rd;
	पूर्णाङ्क cpu, best_energy_cpu, target = -1;
	पूर्णाङ्क पूर्व_माति = -1, best_माति = -1;
	अचिह्नित दीर्घ best_actual_cap = 0;
	अचिह्नित दीर्घ पूर्व_actual_cap = 0;
	संरचना sched_domain *sd;
	संरचना perf_domain *pd;
	संरचना energy_env eenv;

	pd = rcu_dereference_all(rd->pd);
	यदि (!pd)
		प्रत्यागम target;

	/*
	 * Energy-aware wake-up happens on the lowest sched_domain starting
	 * from sd_asym_cpucapacity spanning over this_cpu and prev_cpu.
	 */
	sd = rcu_dereference_all(*this_cpu_ptr(&sd_asym_cpucapacity));
	यावत् (sd && !cpumask_test_cpu(पूर्व_cpu, sched_domain_विस्तृतिः(sd)))
		sd = sd->parent;
	यदि (!sd)
		प्रत्यागम target;

	target = पूर्व_cpu;

	sync_entity_आरोपय_avg(&p->se);
	यदि (!task_util_est(p) && p_util_min == 0)
		प्रत्यागम target;

	eenv_task_busy_time(&eenv, p, पूर्व_cpu);

	प्रति (; pd; pd = pd->अग्र) {
		अचिह्नित दीर्घ util_min = p_util_min, util_max = p_util_max;
		अचिह्नित दीर्घ cpu_cap, cpu_actual_cap, util;
		दीर्घ पूर्व_अतिरिक्तः_cap = -1, max_अतिरिक्तः_cap = -1;
		अचिह्नित दीर्घ rq_util_min, rq_util_max;
		अचिह्नित दीर्घ cur_delta, आधारः_energy;
		पूर्णाङ्क max_अतिरिक्तः_cap_cpu = -1;
		पूर्णाङ्क माति, max_माति = -1;

		यदि (!cpumask_and(cpus, perf_domain_विस्तृतिः(pd), cpu_online_आवरणम्))
			अनुवर्त;

		/* Account external pressure for the energy estimation */
		cpu = cpumask_first(cpus);
		cpu_actual_cap = get_actual_cpu_क्षमता(cpu);

		eenv.cpu_cap = cpu_actual_cap;
		eenv.pd_cap = 0;

		for_each_cpu(cpu, cpus) {
			संरचना rq *rq = cpu_rq(cpu);

			eenv.pd_cap += cpu_actual_cap;

			यदि (!cpumask_test_cpu(cpu, sched_domain_विस्तृतिः(sd)))
				अनुवर्त;

			यदि (!cpumask_test_cpu(cpu, p->cpus_ptr))
				अनुवर्त;

			util = cpu_util(cpu, p, cpu, 0);
			cpu_cap = क्षमता_सम्बन्धि(cpu);

			/*
			 * Skip CPUs that cannot satisfy the capacity request.
			 * IOW, placing the task there would make the CPU
			 * overutilized. Take uclamp into account to see how
			 * much capacity we can get out of the CPU; this is
			 * aligned with sched_cpu_util().
			 */
			यदि (uclamp_is_प्रयुक्तम्() && !uclamp_rq_is_idle(rq)) {
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

			माति = util_माति_cpu(util, util_min, util_max, cpu);
			यदि (!माति)
				अनुवर्त;

			lsub_धनात्मकम्(&cpu_cap, util);

			यदि (cpu == पूर्व_cpu) {
				/* Always use prev_cpu as a candidate. */
				पूर्व_अतिरिक्तः_cap = cpu_cap;
				पूर्व_माति = माति;
			} अन्यथा यदि ((माति > max_माति) ||
				   ((माति == max_माति) && ((दीर्घ)cpu_cap > max_अतिरिक्तः_cap))) {
				/*
				 * Find the CPU with the maximum spare capacity
				 * among the remaining CPUs in the performance
				 * domain.
				 */
				max_अतिरिक्तः_cap = cpu_cap;
				max_अतिरिक्तः_cap_cpu = cpu;
				max_माति = माति;
			}
		}

		यदि (max_अतिरिक्तः_cap_cpu < 0 && पूर्व_अतिरिक्तः_cap < 0)
			अनुवर्त;

		eenv_pd_busy_time(&eenv, cpus, p);
		/* Compute the 'base' energy of the pd, without @p */
		आधारः_energy = compute_energy(&eenv, pd, cpus, p, -1);

		/* Evaluate the energy impact of using prev_cpu. */
		यदि (पूर्व_अतिरिक्तः_cap > -1) {
			पूर्व_delta = compute_energy(&eenv, pd, cpus, p,
						    पूर्व_cpu);
			/* CPU utilization has changed */
			यदि (पूर्व_delta < आधारः_energy)
				प्रत्यागम target;
			पूर्व_delta -= आधारः_energy;
			पूर्व_actual_cap = cpu_actual_cap;
			best_delta = min(best_delta, पूर्व_delta);
		}

		/* Evaluate the energy impact of using max_spare_cap_cpu. */
		यदि (max_अतिरिक्तः_cap_cpu >= 0 && max_अतिरिक्तः_cap > पूर्व_अतिरिक्तः_cap) {
			/* Current best energy cpu fits better */
			यदि (max_माति < best_माति)
				अनुवर्त;

			/*
			 * Both don't fit performance hint (i.e. uclamp_min)
			 * but best energy cpu has better capacity.
			 */
			यदि ((max_माति < 0) &&
			    (cpu_actual_cap <= best_actual_cap))
				अनुवर्त;

			cur_delta = compute_energy(&eenv, pd, cpus, p,
						   max_अतिरिक्तः_cap_cpu);
			/* CPU utilization has changed */
			यदि (cur_delta < आधारः_energy)
				प्रत्यागम target;
			cur_delta -= आधारः_energy;

			/*
			 * Both fit for the task but best energy cpu has lower
			 * energy impact.
			 */
			यदि ((max_माति > 0) && (best_माति > 0) &&
			    (cur_delta >= best_delta))
				अनुवर्त;

			best_delta = cur_delta;
			best_energy_cpu = max_अतिरिक्तः_cap_cpu;
			best_माति = max_माति;
			best_actual_cap = cpu_actual_cap;
		}
	}

	यदि ((best_माति > पूर्व_माति) ||
	    ((best_माति > 0) && (best_delta < पूर्व_delta)) ||
	    ((best_माति < 0) && (best_actual_cap > पूर्व_actual_cap)))
		target = best_energy_cpu;

	प्रत्यागम target;
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
स्थिर पूर्णाङ्क
select_task_rq_fair(संरचना task_struct *p, पूर्णाङ्क पूर्व_cpu, पूर्णाङ्क wake_flags)
{
	पूर्णाङ्क sync = (wake_flags & WF_SYNC) && !(current->flags & PF_EXITING);
	संरचना sched_domain *tmp, *sd = रिक्त;
	पूर्णाङ्क cpu = smp_processor_id();
	पूर्णाङ्क new_cpu = पूर्व_cpu;
	पूर्णाङ्क want_affine = 0;
	/* SD_flags and WF_flags share the first nibble */
	पूर्णाङ्क sd_flag = wake_flags & 0xF;

	/*
	 * required for stable ->cpus_allowed
	 */
	lockdep_assert_held(&p->pi_तालकम्);
	यदि (wake_flags & WF_TTWU) {
		लेखः_wakee(p);

		यदि ((wake_flags & WF_CURRENT_CPU) &&
		    cpumask_test_cpu(cpu, p->cpus_ptr))
			प्रत्यागम cpu;

		यदि (!is_rd_overutilized(this_rq()->rd)) {
			new_cpu = find_energy_efficient_cpu(p, पूर्व_cpu);
			यदि (new_cpu >= 0)
				प्रत्यागम new_cpu;
			new_cpu = पूर्व_cpu;
		}

		want_affine = !wake_wide(p) && cpumask_test_cpu(cpu, p->cpus_ptr);
	}

	for_each_domain(cpu, tmp) {
		/*
		 * If both 'cpu' and 'prev_cpu' are part of this domain,
		 * cpu is a valid SD_WAKE_AFFINE target.
		 */
		यदि (want_affine && (tmp->flags & SD_WAKE_AFFINE) &&
		    cpumask_test_cpu(पूर्व_cpu, sched_domain_विस्तृतिः(tmp))) {
			यदि (cpu != पूर्व_cpu)
				new_cpu = wake_affine(tmp, p, cpu, पूर्व_cpu, sync);

			sd = रिक्त; /* Prefer wake_affine over balance flags */
			भङ्ग;
		}

		/*
		 * Usually only true for WF_EXEC and WF_FORK, as sched_domains
		 * usually do not have SD_BALANCE_WAKE set. That means wakeup
		 * will usually go to the fast path.
		 */
		यदि (tmp->flags & sd_flag)
			sd = tmp;
		अन्यथा यदि (!want_affine)
			भङ्ग;
	}

	/* Slow path */
	यदि (unlikely(sd)) {
		new_cpu = sched_balance_find_dst_cpu(sd, p, cpu, पूर्व_cpu, sd_flag);
		प्रत्यागम select_idle_smt_cpu(p, new_cpu);
	}

	/* Fast path */
	यदि (wake_flags & WF_TTWU)
		प्रत्यागम select_idle_sibling(p, पूर्व_cpu, new_cpu);

	प्रत्यागम new_cpu;
}

/*
 * Called immediately before a task is migrated to a new CPU; task_cpu(p) and
 * cfs_rq_of(p) references at time of call are still valid and identify the
 * previous CPU. The caller guarantees p->pi_lock or task_rq(p)->lock is held.
 */
स्थिर शून्य migrate_task_rq_fair(संरचना task_struct *p, पूर्णाङ्क new_cpu)
{
	संरचना sched_entity *se = &p->se;

	यदि (!task_on_rq_migrating(p)) {
		remove_entity_आरोपय_avg(se);

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
	se->avg.चरमः_update_time = 0;

	update_अवलोकनम्_period(p, new_cpu);
}

स्थिर शून्य task_dead_fair(संरचना task_struct *p)
{
	संरचना sched_entity *se = &p->se;
	remove_entity_आरोपय_avg(se);
}

/*
 * Set the max capacity the task is allowed to run at for misfit detection.
 */
स्थिर शून्य समुच्चयः_task_max_allowed_क्षमता(संरचना task_struct *p)
{
	संरचना asym_cap_दत्त *प्रविष्टिः;

	यदि (!sched_asym_cpucap_सक्रिय())
		प्रत्यागम;

	rcu_read_तालकम्();
	list_for_each_प्रविष्टिः_rcu(प्रविष्टिः, &asym_cap_list, link) {
		cpumask_t *cpumask;

		cpumask = cpu_क्षमता_विस्तृतिः(प्रविष्टिः);
		यदि (!cpumask_intersects(p->cpus_ptr, cpumask))
			अनुवर्त;

		p->max_allowed_क्षमता = प्रविष्टिः->क्षमता;
		भङ्ग;
	}
	rcu_read_unlock();
}

स्थिर शून्य समुच्चयः_cpus_allowed_fair(संरचना task_struct *p, संरचना affinity_context *ctx)
{
	समुच्चयः_cpus_allowed_common(p, ctx);
	समुच्चयः_task_max_allowed_क्षमता(p);
}

गणना preempt_wakeup_action {
	PREEMPT_WAKEUP_NONE,	/* No preemption. */
	PREEMPT_WAKEUP_SHORT,	/* Ignore slice protection. */
	PREEMPT_WAKEUP_PICK,	/* Let pick_eevdf() decide. */
	PREEMPT_WAKEUP_RESCHED,	/* Force reschedule. */
};

स्थिर अन्तःस्थ bool समुच्चयः_preempt_buddy(संरचना cfs_rq *cfs_rq, संरचना sched_entity *pse)
{
	/*
	 * Keep existing buddy if the deadline is sooner than pse.
	 * The older buddy may be cache cold and completely unrelated
	 * to the current wakeup but that is unpredictable where as
	 * obeying the deadline is more in line with EEVDF objectives.
	 */
	यदि (cfs_rq->अग्र && entity_before(cfs_rq->अग्र, pse))
		प्रत्यागम false;

	समुच्चयः_अग्र_buddy(cfs_rq, pse);
	प्रत्यागम true;
}

स्थिर अन्तःस्थ bool समुच्चयः_short_buddy(संरचना cfs_rq *cfs_rq, संरचना sched_entity *pse)
{
	यदि (cfs_rq->अग्र) {
		यदि (cfs_rq->अग्र->खण्डिका < pse->खण्डिका)
			प्रत्यागम false;

		यदि (cfs_rq->अग्र->खण्डिका == pse->खण्डिका &&
		    entity_before(cfs_rq->अग्र, pse))
			प्रत्यागम false;
	}

	समुच्चयः_अग्र_buddy(cfs_rq, pse);
	प्रत्यागम true;
}

/*
 * WF_SYNC|WF_TTWU indicates the waker expects to sleep but it is not
 * strictly enforced because the hint is either misunderstood or
 * multiple tasks must be woken up.
 */
स्थिर अन्तःस्थ गणना preempt_wakeup_action
preempt_sync(संरचना rq *rq, पूर्णाङ्क wake_flags,
	     संरचना sched_entity *pse, संरचना sched_entity *se)
{
	u64 देहली, delta;

	/*
	 * WF_SYNC without WF_TTWU is not expected so warn if it happens even
	 * though it is likely harmless.
	 */
	WARN_ON_ONCE(!(wake_flags & WF_TTWU));

	देहली = sysctl_sched_migration_cost;
	delta = rq_clock_task(rq) - se->exec_start;
	यदि ((s64)delta < 0)
		delta = 0;

	/*
	 * WF_RQ_SELECTED implies the tasks are stacking on a CPU when they
	 * could run on other CPUs. Reduce the threshold before preemption is
	 * allowed to an arbitrary lower value as it is more likely (but not
	 * guaranteed) the waker requires the wakee to finish.
	 */
	यदि (wake_flags & WF_RQ_SELECTED)
		देहली >>= 2;

	/*
	 * As WF_SYNC is not strictly obeyed, allow some runtime for batch
	 * wakeups to be issued.
	 */
	यदि (entity_before(pse, se) && delta >= देहली)
		प्रत्यागम PREEMPT_WAKEUP_RESCHED;

	प्रत्यागम PREEMPT_WAKEUP_NONE;
}

/*
 * Preempt the current task with a newly woken task if needed:
 */
स्थिर शून्य wakeup_preempt_fair(संरचना rq *rq, संरचना task_struct *p, पूर्णाङ्क wake_flags)
{
	गणना preempt_wakeup_action preempt_action = PREEMPT_WAKEUP_PICK;
	संरचना task_struct *donor = rq->donor;
	संरचना sched_entity *nse, *se = &donor->se, *pse = &p->se;
	संरचना cfs_rq *cfs_rq = &rq->cfs;
	पूर्णाङ्क cse_is_idle, pse_is_idle;

	/*
	 * XXX Getting preempted by higher class, try and find idle CPU?
	 */
	यदि (p->sched_class != &fair_sched_class ||
	    donor->sched_class != &fair_sched_class)
		प्रत्यागम;

	यदि (unlikely(se == pse))
		प्रत्यागम;

	/*
	 * This is possible from callers such as attach_tasks(), in which we
	 * unconditionally wakeup_preempt() after an enqueue (which may have
	 * lead to a throttle).  This both saves work and prevents false
	 * next-buddy nomination below.
	 */
	यदि (task_is_throttled(p))
		प्रत्यागम;

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
	यदि (!sched_feat(PREEMPT_SHORT) && test_tsk_need_resched(rq->curr))
		प्रत्यागम;

	यदि (!sched_feat(WAKEUP_PREEMPTION))
		प्रत्यागम;

	WARN_ON_ONCE(!pse);

	cse_is_idle = se_is_idle(se);
	pse_is_idle = se_is_idle(pse);

	nse = se;
	/*
	 * Preempt an idle entity in favor of a non-idle entity (and don't preempt
	 * in the inverse case).
	 */
	यदि (cse_is_idle && !pse_is_idle)
		गच्छ preempt;

	update_curr_fair(rq);

	यदि (cse_is_idle != pse_is_idle)
		गच्छ update;

	/*
	 * BATCH and IDLE tasks do not preempt others.
	 */
	यदि (unlikely(!प्रसामान्यम्_policy(p->policy)))
		गच्छ update;

	/*
	 * Do not preempt for tasks that are sched_delayed as it would violate
	 * EEVDF to forcibly queue an ineligible task.
	 */
	यदि (pse->sched_delayed)
		गच्छ update;

	/*
	 * If @p has a shorter slice than current and @p is eligible, override
	 * current's slice protection in order to allow preemption.
	 */
	यदि (sched_feat(PREEMPT_SHORT) && (pse->खण्डिका < se->खण्डिका)) {
		preempt_action = PREEMPT_WAKEUP_SHORT;
		गच्छ pick;
	}

	/*
	 * Ignore wakee preemption on WF_FORK as it is less likely that
	 * there is shared data as exec often follow fork.
	 */
	यदि (wake_flags & WF_FORK)
		गच्छ update;

	/* Prefer picking wakee soon if appropriate. */
	यदि (sched_feat(अग्र_BUDDY) && समुच्चयः_preempt_buddy(cfs_rq, pse)) {
		/*
		 * Decide whether to obey WF_SYNC hint for a new buddy. Old
		 * buddies are ignored as they may not be relevant to the
		 * waker and less likely to be cache hot.
		 */
		यदि (wake_flags & WF_SYNC)
			preempt_action = preempt_sync(rq, wake_flags, pse, se);
	}

	विकल्प (preempt_action) {
	पक्ष PREEMPT_WAKEUP_NONE:
		प्रत्यागम;
	पक्ष PREEMPT_WAKEUP_RESCHED:
		गच्छ preempt;
	पक्ष PREEMPT_WAKEUP_SHORT:
		fallthrough;
	पक्ष PREEMPT_WAKEUP_PICK:
		भङ्ग;
	}

pick:
	यदि (cfs_rq->h_nr_queued) {
		nse = pick_अग्र_entity(rq, preempt_action != PREEMPT_WAKEUP_SHORT);
		यदि (unlikely(!nse))
			गच्छ pick;

		/* If @p has become the most eligible task, force preemption */
		यदि (nse == pse)
			गच्छ preempt;
	}

	/*
	 * If @p is eligible but not the next task to run then cancel protection
	 * to prevent large scheduling latency
	 */
	यदि (preempt_action == PREEMPT_WAKEUP_SHORT && entity_eligible(cfs_rq, pse))
		गच्छ preempt;
update:
	यदि (sched_feat(सततम्_गन्तव्यम्_PARITY))
		update_protect_खण्डिका(cfs_rq, se);

	प्रत्यागम;

preempt:
	cancel_protect_खण्डिका(se);

	यदि (preempt_action == PREEMPT_WAKEUP_SHORT)
		समुच्चयः_short_buddy(cfs_rq, pse);

	resched_curr_lazy(rq);
}

संरचना task_struct *pick_task_fair(संरचना rq *rq, संरचना rq_flags *rf)
	__must_hold(__rq_lockp(rq))
{
	संरचना cfs_rq *cfs_rq = &rq->cfs;
	संरचना sched_entity *se;
	संरचना task_struct *p;
	पूर्णाङ्क new_tasks;

again:
	यदि (!cfs_rq->h_nr_queued)
		गच्छ idle;

	/* Might not have done put_prev_entity() */
	यदि (cfs_rq->curr && cfs_rq->curr->on_rq)
		update_curr_eevdf(cfs_rq);

	se = pick_अग्र_entity(rq, true);
	यदि (!se)
		गच्छ again;

	p = task_सम्बन्धि(se);
	प्रत्यागम p;

idle:
	यदि (sched_core_enabled(rq))
		प्रत्यागम रिक्त;

	new_tasks = sched_balance_newidle(rq, rf);
	यदि (new_tasks < 0)
		प्रत्यागम RETRY_TASK;
	यदि (new_tasks > 0)
		गच्छ again;
	प्रत्यागम रिक्त;
}

स्थिर संरचना task_struct *
fair_server_pick_task(संरचना sched_dl_entity *dl_se, संरचना rq_flags *rf)
	__must_hold(__rq_lockp(dl_se->rq))
{
	प्रत्यागम pick_task_fair(dl_se->rq, rf);
}

शून्य fair_server_init(संरचना rq *rq)
{
	संरचना sched_dl_entity *dl_se = &rq->fair_server;

	init_dl_entity(dl_se);

	dl_server_init(dl_se, rq, fair_server_pick_task);
}

/*
 * Account for a descheduled task:
 */
स्थिर शून्य put_पूर्व_task_fair(संरचना rq *rq, संरचना task_struct *पूर्व, संरचना task_struct *अग्र)
{
	संरचना sched_entity *se = &पूर्व->se;
	संरचना cfs_rq *cfs_rq = &rq->cfs;
	संरचना sched_entity *nse = रिक्त;

#यदि_परिभाषितम् CONFIG_FAIR_गणः_SCHED
	यदि (अग्र && अग्र->sched_class == &fair_sched_class)
		nse = &अग्र->se;
#यदि_अन्तः

	यावत् (se) {
		cfs_rq = cfs_rq_सम्बन्धि(se);
		यदि (!nse || cfs_rq->h_curr)
			put_पूर्व_entity(cfs_rq, se);
#यदि_परिभाषितम् CONFIG_FAIR_गणः_SCHED
		यदि (nse) {
			यदि (is_same_गणः(se, nse))
				भङ्ग;

			पूर्णाङ्क d = nse->गभीरता - se->गभीरता;
			यदि (d >= 0) {
				/* nse has equal or greater depth, ascend */
				nse = parent_entity(nse);
				/* if nse is the deeper, do not ascend se */
				यदि (d > 0)
					अनुवर्त;
			}
		}
#यदि_अन्तः
		se = parent_entity(se);
	}

	/* Put 'current' back into the tree. */
	cfs_rq = &rq->cfs;
	se = &पूर्व->se;
	WARN_ON_ONCE(cfs_rq->curr != se);
	cfs_rq->curr = रिक्त;
	यदि (se->on_rq)
		__enqueue_entity(cfs_rq, se);
}

/*
 * sched_yield() is very simple
 */
स्थिर शून्य yield_task_fair(संरचना rq *rq)
{
	संरचना task_struct *curr = rq->donor;
	संरचना sched_entity *se = &curr->se;
	संरचना cfs_rq *cfs_rq = &rq->cfs;

	/*
	 * Are we the only task in the tree?
	 */
	यदि (unlikely(rq->nr_running == 1))
		प्रत्यागम;

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
	यदि (entity_eligible(cfs_rq, se)) {
		se->vruntime = se->deadline;
		update_deadline(cfs_rq, se);
	}
}

स्थिर bool yield_गन्तव्यम्_task_fair(संरचना rq *rq, संरचना task_struct *p)
{
	संरचना sched_entity *se = &p->se;

	/* !se->on_rq also covers throttled task */
	यदि (!se->on_rq || se->sched_delayed)
		प्रत्यागम false;

	/* Tell the scheduler that we'd really like se to run next. */
	समुच्चयः_अग्र_buddy(&task_rq(p)->cfs, se);

	yield_task_fair(rq);

	प्रत्यागम true;
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

स्थिर अचिह्नित दीर्घ __read_mostly max_आरोपय_balance_interval = HZ/10;

गणना fbq_type { regular, remote, all };

/*
 * 'group_type' describes the group of CPUs at the moment of load balancing.
 *
 * The enum is ordered by pulling priority, with the group with lowest priority
 * first so the group_type can simply be compared when selecting the busiest
 * group. See update_sd_pick_busiest().
 */
गणना गणः_type {
	/* The group has spare capacity that can be used to run more tasks.  */
	गणः_has_अतिरिक्तः = 0,
	/*
	 * The group is fully used and the tasks don't compete for more CPU
	 * cycles. Nevertheless, some tasks might wait before running.
	 */
	गणः_fully_busy,
	/*
	 * One task doesn't fit with CPU's capacity and must be migrated to a
	 * more powerful CPU.
	 */
	गणः_misfit_task,
	/*
	 * Balance SMT group that's fully busy. Can benefit from migration
	 * a task on SMT with busy sibling to another CPU on idle core.
	 */
	गणः_smt_balance,
	/*
	 * SD_ASYM_PACKING only: One local CPU with higher capacity is available,
	 * and the task should be migrated to it instead of running on the
	 * current CPU.
	 */
	गणः_asym_packing,
	/*
	 * The tasks' affinity constraints previously prevented the scheduler
	 * from balancing the load across the system.
	 */
	गणः_imbalanced,
	/*
	 * There are tasks running on non-preferred LLC, possible to move
	 * them to their preferred LLC without creating too much imbalance.
	 * The priority of group_llc_balance is lower than that of
	 * group_overloaded and higher than that of all other group types.
	 * This is because group_llc_balance may exacerbate load imbalance.
	 * If the LLC balancing attempt fails, the nr_balance_failed
	 * mechanism will trigger other group types to rebalance the load.
	 */
	गणः_llc_balance,
	/*
	 * The CPU is overloaded and can't provide expected CPU cycles to all
	 * tasks.
	 */
	गणः_overloaded
};

गणना migration_type {
	migrate_आरोपय = 0,
	migrate_util,
	migrate_task,
	migrate_misfit,
	migrate_llc_task
};

#परिभाषय LBF_ALL_PINNED	0x01
#परिभाषय LBF_NEED_BREAK	0x02
#परिभाषय LBF_DST_PINNED  0x04
#परिभाषय LBF_SOME_PINNED	0x08
#परिभाषय LBF_सक्रिय_LB	0x10
#परिभाषय LBF_LLC_PINNED	0x20
#परिभाषय LBF_सक्रिय_LB_LLC	0x40

संरचना lb_env {
	संरचना sched_domain	*sd;

	संरचना rq		*src_rq;
	पूर्णाङ्क			src_cpu;

	पूर्णाङ्क			dst_cpu;
	संरचना rq		*dst_rq;
	bool			dst_core_idle;

	संरचना cpumask		*dst_grpmask;
	पूर्णाङ्क			new_dst_cpu;
	गणना cpu_idle_type	idle;
	दीर्घ			imbalance;
	/* The set of CPUs under consideration for load-balancing */
	संरचना cpumask		*cpus;

	अचिह्नित पूर्णाङ्क		flags;

	अचिह्नित पूर्णाङ्क		loop;
	अचिह्नित पूर्णाङ्क		loop_break;
	अचिह्नित पूर्णाङ्क		loop_max;

	गणना fbq_type		fbq_type;
	गणना migration_type	migration_type;
	संरचना list_शीर्ष	tasks;
};

/*
 * Is this task likely cache-hot:
 */
स्थिर पूर्णाङ्क task_hot(संरचना task_struct *p, संरचना lb_env *env)
{
	s64 delta;

	lockdep_assert_rq_held(env->src_rq);

	यदि (p->sched_class != &fair_sched_class)
		प्रत्यागम 0;

	यदि (unlikely(task_has_idle_policy(p)))
		प्रत्यागम 0;

	/* SMT siblings share cache */
	यदि (env->sd->flags & SD_SHARE_CPUCAPACITY)
		प्रत्यागम 0;

	/*
	 * Buddy candidates are cache hot:
	 */
	यदि (sched_feat(CACHE_HOT_BUDDY) && env->dst_rq->nr_running &&
	    (&p->se == cfs_rq_सम्बन्धि(&p->se)->अग्र))
		प्रत्यागम 1;

	यदि (sysctl_sched_migration_cost == -1)
		प्रत्यागम 1;

	/*
	 * Don't migrate task if the task's cookie does not match
	 * with the destination CPU's core cookie.
	 */
	यदि (!sched_core_cookie_मेलनम्(cpu_rq(env->dst_cpu), p))
		प्रत्यागम 1;

	यदि (sysctl_sched_migration_cost == 0)
		प्रत्यागम 0;

	delta = rq_clock_task(env->src_rq) - p->se.exec_start;

	प्रत्यागम delta < (s64)sysctl_sched_migration_cost;
}

#यदि_परिभाषितम् CONFIG_NUMA_BALANCING
/*
 * Returns a positive value, if task migration degrades locality.
 * Returns 0, if task migration is not affected by locality.
 * Returns a negative value, if task migration improves locality i.e migration preferred.
 */
स्थिर दीर्घ migrate_degrades_locality(संरचना task_struct *p, संरचना lb_env *env)
{
	संरचना numa_गणः *numa_गणः = rcu_dereference_all(p->numa_गणः);
	अचिह्नित दीर्घ src_weight, dst_weight;
	पूर्णाङ्क src_nid, dst_nid, dist;

	यदि (!static_branch_likely(&sched_numa_balancing))
		प्रत्यागम 0;

	यदि (!p->numa_दोषाः || !(env->sd->flags & SD_NUMA))
		प्रत्यागम 0;

	src_nid = cpu_गन्तव्यम्_ग्रन्थिः(env->src_cpu);
	dst_nid = cpu_गन्तव्यम्_ग्रन्थिः(env->dst_cpu);

	यदि (src_nid == dst_nid)
		प्रत्यागम 0;

	/* Migrating away from the preferred node is always bad. */
	यदि (src_nid == p->numa_preferred_nid) {
		यदि (env->src_rq->nr_running > env->src_rq->nr_preferred_running)
			प्रत्यागम 1;
		अन्यथा
			प्रत्यागम 0;
	}

	/* Encourage migration to the preferred node. */
	यदि (dst_nid == p->numa_preferred_nid)
		प्रत्यागम -1;

	/* Leaving a core idle is often worse than degrading locality. */
	यदि (env->idle == CPU_IDLE)
		प्रत्यागम 0;

	dist = ग्रन्थिः_distance(src_nid, dst_nid);
	यदि (numa_गणः) {
		src_weight = गणः_weight(p, src_nid, dist);
		dst_weight = गणः_weight(p, dst_nid, dist);
	} अन्यथा {
		src_weight = task_weight(p, src_nid, dist);
		dst_weight = task_weight(p, dst_nid, dist);
	}

	प्रत्यागम src_weight - dst_weight;
}

#अन्यथा /* !CONFIG_NUMA_BALANCING: */
स्थिर अन्तःस्थ दीर्घ migrate_degrades_locality(संरचना task_struct *p,
					     संरचना lb_env *env)
{
	प्रत्यागम 0;
}
#यदि_अन्तः /* !CONFIG_NUMA_BALANCING */

/*
 * Check whether the task is ineligible on the destination cpu
 *
 * When the PLACE_LAG scheduling feature is enabled and
 * dst_cfs_rq->nr_queued is greater than 1, if the task
 * is ineligible, it will also be ineligible when
 * it is migrated to the destination cpu.
 */
स्थिर अन्तःस्थ पूर्णाङ्क task_is_ineligible_on_dst_cpu(संरचना task_struct *p, पूर्णाङ्क dest_cpu)
{
	संरचना cfs_rq *dst_cfs_rq = &cpu_rq(dest_cpu)->cfs;

	यदि (sched_feat(PLACE_LAG) && dst_cfs_rq->h_nr_queued &&
	    !entity_eligible(&task_rq(p)->cfs, &p->se))
		प्रत्यागम 1;

	प्रत्यागम 0;
}

#यदि_परिभाषितम् CONFIG_SCHED_CACHE
/*
 * The margin used when comparing LLC utilization with CPU capacity.
 * It determines the LLC load level where active LLC aggregation is
 * done.
 * Derived from fits_capacity().
 *
 * (default: ~50%, tunable via debugfs)
 */
स्थिर bool माति_llc_क्षमता(अचिह्नित दीर्घ util, अचिह्नित दीर्घ max)
{
	u32 aggr_pct = llc_overaggr_pct;

	/*
	 * For single core systems, raise the aggregation
	 * threshold to accommodate more tasks.
	 */
	यदि (cpu_smt_अङ्कः_तन्तवः == 1)
		aggr_pct = (aggr_pct * 3 / 2);

	प्रत्यागम util * 100 < max * aggr_pct;
}

/*
 * The margin used when comparing utilization.
 * is 'util1' noticeably greater than 'util2'
 * Derived from capacity_greater().
 * Bias is in perentage.
 */
/* Allows dst util to be bigger than src util by up to bias percent */
#परिभाषय util_greater(util1, util2) \
	((util1) * 100 > (util2) * (100 + llc_imb_pct))

स्थिर __maybe_unused bool get_llc_stats(पूर्णाङ्क cpu, अचिह्नित दीर्घ *util,
					 अचिह्नित दीर्घ *cap)
{
	संरचना sched_domain_shared *sd_share;

	sd_share = rcu_dereference_all(प्रत्येकम्_cpu(sd_llc_shared, cpu));
	यदि (!sd_share)
		प्रत्यागम false;

	*util = READ_ONCE(sd_share->util_avg);
	*cap = READ_ONCE(sd_share->क्षमता);

	प्रत्यागम true;
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
गणना llc_mig {
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
स्थिर गणना llc_mig can_migrate_llc(पूर्णाङ्क src_cpu, पूर्णाङ्क dst_cpu,
				    अचिह्नित दीर्घ tsk_util,
				    bool गन्तव्यम्_pref)
{
	अचिह्नित दीर्घ src_util, dst_util, src_cap, dst_cap;

	यदि (!get_llc_stats(src_cpu, &src_util, &src_cap) ||
	    !get_llc_stats(dst_cpu, &dst_util, &dst_cap))
		प्रत्यागम mig_unrestricted;

	src_util = src_util < tsk_util ? 0 : src_util - tsk_util;
	dst_util = dst_util + tsk_util;

	यदि (!माति_llc_क्षमता(dst_util, dst_cap) &&
	    !माति_llc_क्षमता(src_util, src_cap))
		प्रत्यागम mig_unrestricted;

	यदि (गन्तव्यम्_pref) {
		/*
		 * Don't migrate if we will get preferred LLC too
		 * heavily loaded and if the dest is much busier
		 * than the src, in which case migration will
		 * increase the imbalance too much.
		 */
		यदि (!माति_llc_क्षमता(dst_util, dst_cap) &&
		    util_greater(dst_util, src_util))
			प्रत्यागम mig_forbid;
	} अन्यथा {
		/*
		 * Don't migrate if we will leave preferred LLC
		 * too idle, or if this migration leads to the
		 * non-preferred LLC falls within sysctl_aggr_imb percent
		 * of preferred LLC, leading to migration again
		 * back to preferred LLC.
		 */
		यदि (माति_llc_क्षमता(src_util, src_cap) ||
		    !util_greater(src_util, dst_util))
			प्रत्यागम mig_forbid;
	}
	प्रत्यागम mig_llc;
}

स्थिर अन्तःस्थ bool task_misfits_asym_cpu(संरचना lb_env *env, संरचना task_struct *p)
{
	/*
	 * On asymmetric CPU capacity domains, do not let cache-aware
	 * balancing pull the task onto a destination CPU that cannot
	 * accommodate it. Doing so would turn the task into a misfit on
	 * the destination, trading a cache-locality gain for a capacity
	 * loss. If the task already does not fit its source CPU, the move
	 * cannot make things worse, so let the LLC preference decide.
	 */
	यदि ((env->sd->flags & SD_ASYM_CPUCAPACITY) && p &&
	    !task_माति_cpu(p, env->dst_cpu) &&
	    task_माति_cpu(p, env->src_cpu))
		प्रत्यागम true;

	प्रत्यागम false;
}

/*
 * Check if task p can migrate from source LLC to
 * destination LLC in terms of cache aware load balance.
 */
स्थिर गणना llc_mig can_migrate_llc_task(संरचना lb_env *env,
					 संरचना task_struct *p)
{
	संरचना sched_cache_गणः *grp;
	bool गन्तव्यम्_pref;
	पूर्णाङ्क cpu, src_cpu, dst_cpu;

	यदि (task_misfits_asym_cpu(env, p))
		प्रत्यागम mig_forbid;

	src_cpu = env->src_cpu;
	dst_cpu = env->dst_cpu;
	grp = rcu_dereference_all(p->sched_cache_grp);
	यदि (!grp)
		प्रत्यागम mig_unrestricted;

	cpu = READ_ONCE(grp->cpu);
	यदि (cpu < 0 || cpus_share_cache(src_cpu, dst_cpu))
		प्रत्यागम mig_unrestricted;

	/* skip cache aware load balance for too many threads */
	यदि (invalid_llc_nr(grp, p, dst_cpu) ||
	    exceed_llc_क्षमता(grp, dst_cpu)) {
		यदि (READ_ONCE(grp->cpu) != -1)
			WRITE_ONCE(grp->cpu, -1);
		प्रत्यागम mig_unrestricted;
	}

	यदि (cpus_share_cache(dst_cpu, cpu))
		गन्तव्यम्_pref = true;
	अन्यथा यदि (cpus_share_cache(src_cpu, cpu))
		गन्तव्यम्_pref = false;
	अन्यथा
		प्रत्यागम mig_unrestricted;

	प्रत्यागम can_migrate_llc(src_cpu, dst_cpu,
			       task_util(p), गन्तव्यम्_pref);
}

/*
 * Check if active load balance breaks LLC locality in
 * terms of cache aware load balance. The load level and
 * imbalance do not warrant breaking LLC preference per
 * the can_migrate_llc() policy. Here, the benefit of
 * LLC locality outweighs the power efficiency gained from
 * migrating the only runnable task away.
 */
स्थिर अन्तःस्थ bool
alb_break_llc(संरचना lb_env *env)
{
	यदि (!sched_cache_enabled())
		प्रत्यागम false;

	यदि (cpus_share_cache(env->src_cpu, env->dst_cpu))
		प्रत्यागम false;
	/*
	 * All tasks prefer to stay on their current CPU.
	 * Do not pull a task from its preferred CPU if:
	 * 1. It is the only task running and does not exceed
	 *    imbalance allowance; OR
	 * 2. Migrating it away from its preferred LLC would violate
	 *    the cache-aware scheduling policy.
	 */
	यदि (env->src_rq->nr_pref_llc_running &&
	    env->src_rq->nr_pref_llc_running == env->src_rq->cfs.h_nr_runnable) {
		अचिह्नित दीर्घ util = 0;
		संरचना task_struct *cur;

		/*
		 * Migrating misfit tasks from current CPU
		 * to CPU with a better fit.
		 * Prioritize that over LLC preference.
		 */
		यदि (env->migration_type == migrate_misfit)
			प्रत्यागम false;

		यदि (env->src_rq->nr_running <= 1)
			प्रत्यागम true;

		cur = rcu_dereference_all(env->src_rq->curr);
		यदि (cur && cur->sched_class == &fair_sched_class)
			util = task_util(cur);

		यदि (task_misfits_asym_cpu(env, cur) ||
		    can_migrate_llc(env->src_cpu, env->dst_cpu,
				    util, false) == mig_forbid)
			प्रत्यागम true;
	}

	प्रत्यागम false;
}

/*
 * Returns true if p's preferred LLC does not match the destination CPU
 * under migrate_llc_task semantics. Passive LB passes migrate_llc_task
 * in env->migration_type, while active LB carries LBF_ACTIVE_LB_LLC in
 * env->flags to avoid overwriting env->migration_type.
 */
स्थिर अन्तःस्थ bool
migrate_llc_task_wrong_dst(संरचना task_struct *p, संरचना lb_env *env)
{
	प्रत्यागम sched_cache_enabled() &&
	       (env->migration_type == migrate_llc_task ||
		env->flags & LBF_सक्रिय_LB_LLC) &&
	       READ_ONCE(p->preferred_llc) != llc_id(env->dst_cpu);
}

/*
 * Check if migrating task p from env->src_cpu to
 * env->dst_cpu breaks LLC localiy.
 */
स्थिर bool migrate_degrades_llc(संरचना task_struct *p, संरचना lb_env *env)
{
	यदि (!sched_cache_enabled())
		प्रत्यागम false;

	यदि (task_has_sched_core(p))
		प्रत्यागम false;
	/*
	 * Skip over tasks that would degrade LLC locality;
	 * only when nr_balanced_failed is sufficiently high do we
	 * ignore this constraint.
	 *
	 * Threshold of cache_nice_tries is set to 1 higher
	 * than nr_balance_failed to avoid excessive task
	 * migration at the same time.
	 */
	यदि (env->sd->nr_balance_failed >= env->sd->cache_nice_tries + 1)
		प्रत्यागम false;

	/*
	 * We know the env->src_cpu has some tasks prefer to
	 * run on env->dst_cpu, skip the tasks do not prefer
	 * env->dst_cpu, and find the one that prefers.
	 */
	यदि (migrate_llc_task_wrong_dst(p, env))
		प्रत्यागम true;

	यदि (can_migrate_llc_task(env, p) != mig_forbid)
		प्रत्यागम false;

	प्रत्यागम true;
}

#अन्यथा
स्थिर अन्तःस्थ bool get_llc_stats(पूर्णाङ्क cpu, अचिह्नित दीर्घ *util,
				 अचिह्नित दीर्घ *cap)
{
	प्रत्यागम false;
}

स्थिर अन्तःस्थ bool
alb_break_llc(संरचना lb_env *env)
{
	प्रत्यागम false;
}

स्थिर अन्तःस्थ bool
migrate_llc_task_wrong_dst(संरचना task_struct *p, संरचना lb_env *env)
{
	प्रत्यागम false;
}

स्थिर अन्तःस्थ bool
migrate_degrades_llc(संरचना task_struct *p, संरचना lb_env *env)
{
	प्रत्यागम false;
}
#यदि_अन्तः
/*
 * can_migrate_task - may task p from runqueue rq be migrated to this_cpu?
 */
स्थिर
पूर्णाङ्क can_migrate_task(संरचना task_struct *p, संरचना lb_env *env)
{
	दीर्घ degrades, hot;

	lockdep_assert_rq_held(env->src_rq);
	यदि (p->sched_task_hot)
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
	यदि ((p->se.sched_delayed) && (env->migration_type != migrate_आरोपय))
		प्रत्यागम 0;

	यदि (lb_throttled_hierarchy(p, env->dst_cpu))
		प्रत्यागम 0;

	/*
	 * We want to prioritize the migration of eligible tasks.
	 * For ineligible tasks we soft-limit them and only allow
	 * them to migrate when nr_balance_failed is non-zero to
	 * avoid load-balancing trying very hard to balance the load.
	 */
	यदि (!env->sd->nr_balance_failed &&
	    task_is_ineligible_on_dst_cpu(p, env->dst_cpu))
		प्रत्यागम 0;

	/* Disregard percpu kthreads; they are where they need to be. */
	यदि (kthread_is_प्रत्येकम्_cpu(p))
		प्रत्यागम 0;

	यदि (task_is_blocked(p))
		प्रत्यागम 0;

	यदि (!cpumask_test_cpu(env->dst_cpu, p->cpus_ptr)) {
		पूर्णाङ्क cpu;

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
		यदि (env->idle == CPU_NEWLY_IDLE ||
		    env->flags & (LBF_DST_PINNED | LBF_सक्रिय_LB))
			प्रत्यागम 0;

		/* Prevent to re-select dst_cpu via env's CPUs: */
		cpu = cpumask_first_and_and(env->dst_grpmask, env->cpus, p->cpus_ptr);

		यदि (cpu < nr_cpu_ids) {
			env->flags |= LBF_DST_PINNED;
			env->new_dst_cpu = cpu;
		}

		प्रत्यागम 0;
	}

	/* Record that we found at least one task that could run on dst_cpu */
	env->flags &= ~LBF_ALL_PINNED;

	यदि (task_on_cpu(env->src_rq, p) ||
	    task_current_donor(env->src_rq, p)) {
		schedstat_inc(p->stats.nr_failed_migrations_running);
		प्रत्यागम 0;
	}

	/*
	 * Aggressive migration if:
	 * 1) active balance
	 * 2) destination numa is preferred
	 * 3) task is cache cold, or
	 * 4) too many balance attempts have failed.
	 */
	यदि (env->flags & LBF_सक्रिय_LB)
		प्रत्यागम !migrate_llc_task_wrong_dst(p, env);

	degrades = migrate_degrades_locality(p, env);
	यदि (!degrades) {
		/*
		 * If the NUMA locality is not broken,
		 * further check if migration would hurt
		 * LLC locality.
		 */
		यदि (migrate_degrades_llc(p, env)) {
			/*
			 * If regular load balancing fails to pull a task
			 * due to LLC locality, this is expected behavior
			 * and we set LBF_LLC_PINNED so we don't increase
			 * nr_balance_failed unecessarily.
			 */
			यदि (env->migration_type != migrate_llc_task)
				env->flags |= LBF_LLC_PINNED;

			प्रत्यागम 0;
		}

		hot = task_hot(p, env);
	} अन्यथा {
		hot = degrades > 0;
	}

	यदि (!hot || env->sd->nr_balance_failed > env->sd->cache_nice_tries) {
		यदि (hot)
			p->sched_task_hot = 1;
		प्रत्यागम 1;
	}

	schedstat_inc(p->stats.nr_failed_migrations_hot);
	प्रत्यागम 0;
}

/*
 * detach_task() -- detach the task for the migration specified in env
 */
स्थिर शून्य detach_task(संरचना task_struct *p, संरचना lb_env *env)
{
	lockdep_assert_rq_held(env->src_rq);

	यदि (p->sched_task_hot) {
		p->sched_task_hot = 0;
		schedstat_inc(env->sd->lb_hot_gained[env->idle]);
		schedstat_inc(p->stats.nr_forced_migrations);
	}

	WARN_ON(task_current(env->src_rq, p));
	WARN_ON(task_current_donor(env->src_rq, p));

	deactivate_task(env->src_rq, p, पङ्क्तेः_निष्कासय_NOCLOCK);
	समुच्चयः_task_cpu(p, env->dst_cpu);
}

/*
 * detach_one_task() -- tries to dequeue exactly one task from env->src_rq, as
 * part of active balancing operations within "domain".
 *
 * Returns a task if successful and NULL otherwise.
 */
स्थिर संरचना task_struct *detach_one_task(संरचना lb_env *env)
{
	संरचना task_struct *p;

	lockdep_assert_rq_held(env->src_rq);

	list_for_each_प्रविष्टिः_reverse(p,
			&env->src_rq->cfs_tasks, se.गणः_ग्रन्थिः) {
		यदि (!can_migrate_task(p, env))
			अनुवर्त;

		detach_task(p, env);

		/*
		 * Right now, this is only the second place where
		 * lb_gained[env->idle] is updated (other is detach_tasks)
		 * so we can safely collect stats here rather than
		 * inside detach_tasks().
		 */
		schedstat_inc(env->sd->lb_gained[env->idle]);
		प्रत्यागम p;
	}
	प्रत्यागम रिक्त;
}

/*
 * detach_tasks() -- tries to detach up to imbalance load/util/tasks from
 * busiest_rq, as part of a balancing operation within domain "sd".
 *
 * Returns number of detached tasks if successful and 0 otherwise.
 */
स्थिर पूर्णाङ्क detach_tasks(संरचना lb_env *env)
{
	संरचना list_शीर्ष *tasks = &env->src_rq->cfs_tasks;
	अचिह्नित दीर्घ util, आरोपय;
	संरचना task_struct *p;
	पूर्णाङ्क detached = 0;

	lockdep_assert_rq_held(env->src_rq);

	/*
	 * Source run queue has been emptied by another CPU, clear
	 * LBF_ALL_PINNED flag as we will not test any task.
	 */
	यदि (env->src_rq->nr_running <= 1) {
		env->flags &= ~LBF_ALL_PINNED;
		प्रत्यागम 0;
	}

	यदि (env->imbalance <= 0)
		प्रत्यागम 0;

	यावत् (!list_रिक्तानि(tasks)) {
		/*
		 * We don't want to steal all, otherwise we may be treated likewise,
		 * which could at worst lead to a livelock crash.
		 */
		यदि (env->idle && env->src_rq->nr_running <= 1)
			भङ्ग;

		env->loop++;
		/* We've more or less seen every task there is, call it quits */
		यदि (env->loop > env->loop_max)
			भङ्ग;

		/* take a breather every nr_migrate tasks */
		यदि (env->loop > env->loop_break) {
			env->loop_break += SCHED_NR_MIGRATE_BREAK;
			env->flags |= LBF_NEED_BREAK;
			भङ्ग;
		}

		p = list_चरमः_प्रविष्टिः(tasks, संरचना task_struct, se.गणः_ग्रन्थिः);

		यदि (!can_migrate_task(p, env))
			गच्छ अग्र;

		विकल्प (env->migration_type) {
		पक्ष migrate_आरोपय:
			/*
			 * Depending of the number of CPUs and tasks and the
			 * cgroup hierarchy, task_h_load() can return a null
			 * value. Make sure that env->imbalance decreases
			 * otherwise detach_tasks() will stop only after
			 * detaching up to loop_max tasks.
			 */
			आरोपय = max_t(अचिह्नित दीर्घ, task_h_आरोपय(p), 1);

			यदि (sched_feat(LB_MIN) &&
			    आरोपय < 16 && !env->sd->nr_balance_failed)
				गच्छ अग्र;

			/*
			 * Make sure that we don't migrate too much load.
			 * Nevertheless, let relax the constraint if
			 * scheduler fails to find a good waiting task to
			 * migrate.
			 */
			यदि (shr_bound(आरोपय, env->sd->nr_balance_failed) > env->imbalance)
				गच्छ अग्र;

			env->imbalance -= आरोपय;
			भङ्ग;

		पक्ष migrate_util:
			util = task_util_est(p);

			यदि (shr_bound(util, env->sd->nr_balance_failed) > env->imbalance)
				गच्छ अग्र;

			env->imbalance -= util;
			भङ्ग;

		पक्ष migrate_task:
			env->imbalance--;
			भङ्ग;

		पक्ष migrate_misfit:
			/* This is not a misfit task */
			यदि (task_माति_cpu(p, env->src_cpu))
				गच्छ अग्र;

			env->imbalance = 0;
			भङ्ग;

		पक्ष migrate_llc_task:
			env->imbalance--;
			भङ्ग;
		}

		detach_task(p, env);
		list_सङ्कलय(&p->se.गणः_ग्रन्थिः, &env->tasks);

		detached++;

#यदि_परिभाषितम् CONFIG_PREEMPTION
		/*
		 * NEWIDLE balancing is a source of latency, so preemptible
		 * kernels will stop after the first task is detached to minimize
		 * the critical section.
		 */
		यदि (env->idle == CPU_NEWLY_IDLE)
			भङ्ग;
#यदि_अन्तः

		/*
		 * We only want to steal up to the prescribed amount of
		 * load/util/tasks.
		 */
		यदि (env->imbalance <= 0)
			भङ्ग;

		अनुवर्त;
अग्र:
		यदि (p->sched_task_hot)
			schedstat_inc(p->stats.nr_failed_migrations_hot);

		list_move(&p->se.गणः_ग्रन्थिः, tasks);
	}

	/*
	 * Right now, this is one of only two places we collect this stat
	 * so we can safely collect detach_one_task() stats here rather
	 * than inside detach_one_task().
	 */
	schedstat_सङ्कलय(env->sd->lb_gained[env->idle], detached);

	प्रत्यागम detached;
}

/*
 * attach_tasks() -- attaches all tasks detached by detach_tasks() to their
 * new rq.
 */
स्थिर शून्य attach_tasks(संरचना lb_env *env)
{
	__attach_tasks(env->dst_rq, &env->tasks);
}

#यदि_परिभाषितम् CONFIG_NO_HZ_COMMON
स्थिर अन्तःस्थ bool cfs_rq_has_blocked_आरोपय(संरचना cfs_rq *cfs_rq)
{
	यदि (cfs_rq->avg.आरोपय_avg)
		प्रत्यागम true;

	यदि (cfs_rq->avg.util_avg)
		प्रत्यागम true;

	प्रत्यागम false;
}

स्थिर अन्तःस्थ bool others_have_blocked(संरचना rq *rq)
{
	यदि (cpu_util_rt(rq))
		प्रत्यागम true;

	यदि (cpu_util_dl(rq))
		प्रत्यागम true;

	यदि (hw_आरोपय_avg(rq))
		प्रत्यागम true;

	यदि (cpu_util_irq(rq))
		प्रत्यागम true;

	प्रत्यागम false;
}

स्थिर अन्तःस्थ शून्य update_blocked_आरोपय_tick(संरचना rq *rq)
{
	WRITE_ONCE(rq->चरमः_blocked_आरोपय_update_tick, jiffies);
}

स्थिर अन्तःस्थ शून्य update_has_blocked_आरोपय_स्थितिः(संरचना rq *rq, bool has_blocked_आरोपय)
{
	यदि (!has_blocked_आरोपय)
		rq->has_blocked_आरोपय = 0;
}
#अन्यथा /* !CONFIG_NO_HZ_COMMON: */
स्थिर अन्तःस्थ bool cfs_rq_has_blocked_आरोपय(संरचना cfs_rq *cfs_rq) { प्रत्यागम false; }
स्थिर अन्तःस्थ bool others_have_blocked(संरचना rq *rq) { प्रत्यागम false; }
स्थिर अन्तःस्थ शून्य update_blocked_आरोपय_tick(संरचना rq *rq) {}
स्थिर अन्तःस्थ शून्य update_has_blocked_आरोपय_स्थितिः(संरचना rq *rq, bool has_blocked_आरोपय) {}
#यदि_अन्तः /* !CONFIG_NO_HZ_COMMON */

स्थिर bool __update_blocked_others(संरचना rq *rq, bool *समाप्तम्)
{
	bool updated;

	/*
	 * update_load_avg() can call cpufreq_update_util(). Make sure that RT,
	 * DL and IRQ signals have been updated before updating CFS.
	 */
	updated = update_other_आरोपय_avgs(rq);

	यदि (others_have_blocked(rq))
		*समाप्तम् = false;

	प्रत्यागम updated;
}

#यदि_परिभाषितम् CONFIG_FAIR_गणः_SCHED

स्थिर bool __update_blocked_fair(संरचना rq *rq, bool *समाप्तम्)
{
	संरचना cfs_rq *cfs_rq, *pos;
	bool decayed = false;

	/*
	 * Iterates the task_group tree in a bottom up fashion, see
	 * list_add_leaf_cfs_rq() for details.
	 */
	for_each_पर्णम्_cfs_rq_safe(rq, cfs_rq, pos) {
		संरचना sched_entity *se;

		यदि (update_cfs_rq_आरोपय_avg(cfs_rq_clock_pelt(cfs_rq), cfs_rq)) {
			update_tg_आरोपय_avg(cfs_rq);

			यदि (cfs_rq->nr_queued == 0)
				update_idle_cfs_rq_clock_pelt(cfs_rq);

			यदि (cfs_rq == &rq->cfs)
				decayed = true;
		}

		/* Propagate pending load changes to the parent, if any: */
		se = cfs_rq_se(cfs_rq);
		यदि (se && !skip_blocked_update(se))
			update_आरोपय_avg(cfs_rq_सम्बन्धि(se), se, UPDATE_TG);

		/*
		 * There can be a lot of idle CPU cgroups.  Don't let fully
		 * decayed cfs_rqs linger on the list.
		 */
		यदि (cfs_rq_is_decayed(cfs_rq))
			list_del_पर्णम्_cfs_rq(cfs_rq);

		/* Don't need periodic decay once load/util_avg are null */
		यदि (cfs_rq_has_blocked_आरोपय(cfs_rq))
			*समाप्तम् = false;
	}

	प्रत्यागम decayed;
}

/*
 * Compute the hierarchical load factor for cfs_rq and all its ascendants.
 * This needs to be done in a top-down fashion because the load of a child
 * group is a fraction of its parents load.
 */
स्थिर शून्य update_cfs_rq_h_आरोपय(संरचना cfs_rq *cfs_rq)
{
	संरचना sched_entity *se = cfs_rq_se(cfs_rq);
	अचिह्नित दीर्घ now = jiffies;
	अचिह्नित दीर्घ आरोपय;

	यदि (cfs_rq->चरमः_h_आरोपय_update == now)
		प्रत्यागम;

	WRITE_ONCE(cfs_rq->h_आरोपय_अग्र, रिक्त);
	for_each_sched_entity(se) {
		cfs_rq = cfs_rq_सम्बन्धि(se);
		WRITE_ONCE(cfs_rq->h_आरोपय_अग्र, se);
		यदि (cfs_rq->चरमः_h_आरोपय_update == now)
			भङ्ग;
	}

	यदि (!se) {
		cfs_rq->h_आरोपय = cfs_rq_आरोपय_avg(cfs_rq);
		cfs_rq->चरमः_h_आरोपय_update = now;
	}

	यावत् ((se = READ_ONCE(cfs_rq->h_आरोपय_अग्र)) != रिक्त) {
		आरोपय = cfs_rq->h_आरोपय;
		आरोपय = div64_ul(आरोपय * se->avg.आरोपय_avg,
				cfs_rq_आरोपय_avg(cfs_rq) + 1);
		cfs_rq = गणः_cfs_rq(se);
		cfs_rq->h_आरोपय = आरोपय;
		cfs_rq->चरमः_h_आरोपय_update = now;
	}
}

स्थिर अचिह्नित दीर्घ task_h_आरोपय(संरचना task_struct *p)
{
	संरचना cfs_rq *cfs_rq = task_cfs_rq(p);

	update_cfs_rq_h_आरोपय(cfs_rq);
	प्रत्यागम div64_ul(p->se.avg.आरोपय_avg * cfs_rq->h_आरोपय,
			cfs_rq_आरोपय_avg(cfs_rq) + 1);
}
#अन्यथा /* !CONFIG_FAIR_GROUP_SCHED: */
स्थिर bool __update_blocked_fair(संरचना rq *rq, bool *समाप्तम्)
{
	संरचना cfs_rq *cfs_rq = &rq->cfs;
	bool decayed;

	decayed = update_cfs_rq_आरोपय_avg(cfs_rq_clock_pelt(cfs_rq), cfs_rq);
	यदि (cfs_rq_has_blocked_आरोपय(cfs_rq))
		*समाप्तम् = false;

	प्रत्यागम decayed;
}

स्थिर अचिह्नित दीर्घ task_h_आरोपय(संरचना task_struct *p)
{
	प्रत्यागम p->se.avg.आरोपय_avg;
}
#यदि_अन्तः /* !CONFIG_FAIR_GROUP_SCHED */

स्थिर शून्य __sched_balance_update_blocked_averages(संरचना rq *rq)
{
	bool decayed = false, समाप्तम् = true;

	update_blocked_आरोपय_tick(rq);

	decayed |= __update_blocked_others(rq, &समाप्तम्);
	decayed |= __update_blocked_fair(rq, &समाप्तम्);

	update_has_blocked_आरोपय_स्थितिः(rq, !समाप्तम्);
	यदि (decayed)
		cpufreq_update_util(rq, 0);
}

स्थिर शून्य sched_balance_update_blocked_averages(पूर्णाङ्क cpu)
{
	संरचना rq *rq = cpu_rq(cpu);

	guard(rq_तालकम्_irqsave)(rq);
	update_rq_clock(rq);
	__sched_balance_update_blocked_averages(rq);
}

/********** Helpers for sched_balance_find_src_group ************************/

/*
 * sg_lb_stats - stats of a sched_group required for load-balancing:
 */
संरचना sg_lb_stats {
	अचिह्नित दीर्घ avg_आरोपय;			/* Avg load            over the CPUs of the group */
	अचिह्नित दीर्घ गणः_आरोपय;		/* Total load          over the CPUs of the group */
	अचिह्नित दीर्घ गणः_क्षमता;		/* Capacity            over the CPUs of the group */
	अचिह्नित दीर्घ गणः_util;		/* Total utilization   over the CPUs of the group */
	अचिह्नित दीर्घ गणः_runnable;		/* Total runnable time over the CPUs of the group */
	अचिह्नित पूर्णाङ्क योग_nr_running;		/* Nr of all tasks running in the group */
	अचिह्नित पूर्णाङ्क योग_h_nr_running;		/* Nr of CFS tasks running in the group */
	अचिह्नित पूर्णाङ्क idle_cpus;                 /* Nr of idle CPUs         in the group */
	अचिह्नित पूर्णाङ्क गणः_weight;
	गणना गणः_type गणः_type;
	अचिह्नित पूर्णाङ्क गणः_asym_packing;	/* Tasks should be moved to preferred CPU */
	अचिह्नित पूर्णाङ्क गणः_smt_balance;		/* Task on busy SMT be moved */
	अचिह्नित पूर्णाङ्क गणः_llc_balance;		/* Tasks should be moved to preferred LLC */
	अचिह्नित दीर्घ गणः_misfit_task_आरोपय;	/* A CPU has a task too big for its capacity */
	अचिह्नित पूर्णाङ्क गणः_overutilized;	/* At least one CPU is overutilized in the group */
#यदि_परिभाषितम् CONFIG_NUMA_BALANCING
	अचिह्नित पूर्णाङ्क nr_numa_running;
	अचिह्नित पूर्णाङ्क nr_preferred_running;
#यदि_अन्तः
#यदि_परिभाषितम् CONFIG_SCHED_CACHE
	अचिह्नित पूर्णाङ्क nr_pref_dst_llc;
#यदि_अन्तः
};

/*
 * sd_lb_stats - stats of a sched_domain required for load-balancing:
 */
संरचना sd_lb_stats {
	संरचना sched_गणः *busiest;		/* Busiest group in this sd */
	संरचना sched_गणः *local;		/* Local group in this sd */
	अचिह्नित दीर्घ योग_आरोपय;		/* Total load of all groups in sd */
	अचिह्नित दीर्घ योग_क्षमता;		/* Total capacity of all groups in sd */
	अचिह्नित दीर्घ avg_आरोपय;			/* Average load across all groups in sd */
	अचिह्नित पूर्णाङ्क prefer_sibling;		/* Tasks should go to sibling first */

	संरचना sg_lb_stats busiest_stat;	/* Statistics of the busiest group */
	संरचना sg_lb_stats local_stat;		/* Statistics of the local group */
};

स्थिर अन्तःस्थ शून्य init_sd_lb_stats(संरचना sd_lb_stats *sds)
{
	/*
	 * Skimp on the clearing to avoid duplicate work. We can avoid clearing
	 * local_stat because update_sg_lb_stats() does a full clear/assignment.
	 * We must however set busiest_stat::group_type and
	 * busiest_stat::idle_cpus to the worst busiest group because
	 * update_sd_pick_busiest() reads these before assignment.
	 */
	*sds = (संरचना sd_lb_stats){
		.busiest = रिक्त,
		.local = रिक्त,
		.योग_आरोपय = 0UL,
		.योग_क्षमता = 0UL,
		.busiest_stat = {
			.idle_cpus = UINT_MAX,
			.गणः_type = गणः_has_अतिरिक्तः,
		},
	};
}

स्थिर अचिह्नित दीर्घ scale_rt_क्षमता(पूर्णाङ्क cpu)
{
	अचिह्नित दीर्घ max = get_actual_cpu_क्षमता(cpu);
	संरचना rq *rq = cpu_rq(cpu);
	अचिह्नित दीर्घ प्रयुक्तम्, मुक्त;
	अचिह्नित दीर्घ irq;

	irq = cpu_util_irq(rq);

	यदि (unlikely(irq >= max))
		प्रत्यागम 1;

	/*
	 * avg_rt.util_avg and avg_dl.util_avg track binary signals
	 * (running and not running) with weights 0 and 1024 respectively.
	 */
	प्रयुक्तम् = cpu_util_rt(rq);
	प्रयुक्तम् += cpu_util_dl(rq);

	यदि (unlikely(प्रयुक्तम् >= max))
		प्रत्यागम 1;

	मुक्त = max - प्रयुक्तम्;

	प्रत्यागम scale_irq_क्षमता(मुक्त, irq, max);
}

स्थिर शून्य update_cpu_क्षमता(संरचना sched_domain *sd, पूर्णाङ्क cpu)
{
	अचिह्नित दीर्घ क्षमता = scale_rt_क्षमता(cpu);
	संरचना sched_गणः *sdg = sd->groups;

	यदि (!क्षमता)
		क्षमता = 1;

	cpu_rq(cpu)->cpu_क्षमता = क्षमता;
	trace_sched_cpu_क्षमता_tp(cpu_rq(cpu));

	sdg->sgc->क्षमता = क्षमता;
	sdg->sgc->min_क्षमता = क्षमता;
	sdg->sgc->max_क्षमता = क्षमता;
}

शून्य update_गणः_क्षमता(संरचना sched_domain *sd, पूर्णाङ्क cpu)
{
	संरचना sched_domain *child = sd->child;
	संरचना sched_गणः *गणः, *sdg = sd->groups;
	अचिह्नित दीर्घ क्षमता, min_क्षमता, max_क्षमता;
	अचिह्नित दीर्घ interval;

	interval = msecs_गन्तव्यम्_jiffies(sd->balance_interval);
	interval = clamp(interval, 1UL, max_आरोपय_balance_interval);
	sdg->sgc->अग्र_update = jiffies + interval;

	यदि (!child) {
		update_cpu_क्षमता(sd, cpu);
		प्रत्यागम;
	}

	क्षमता = 0;
	min_क्षमता = ULONG_MAX;
	max_क्षमता = 0;

	यदि (child->flags & SD_NUMA) {
		/*
		 * SD_NUMA domains cannot assume that child groups
		 * span the current group.
		 */

		for_each_cpu(cpu, sched_गणः_विस्तृतिः(sdg)) {
			अचिह्नित दीर्घ cpu_cap = क्षमता_सम्बन्धि(cpu);

			क्षमता += cpu_cap;
			min_क्षमता = min(cpu_cap, min_क्षमता);
			max_क्षमता = max(cpu_cap, max_क्षमता);
		}
	} अन्यथा  {
		/*
		 * !SD_NUMA domains can assume that child groups
		 * span the current group.
		 */

		गणः = child->groups;
		कुरु {
			संरचना sched_गणः_क्षमता *sgc = गणः->sgc;

			क्षमता += sgc->क्षमता;
			min_क्षमता = min(sgc->min_क्षमता, min_क्षमता);
			max_क्षमता = max(sgc->max_क्षमता, max_क्षमता);
			गणः = गणः->अग्र;
		} यावत् (गणः != child->groups);
	}

	sdg->sgc->क्षमता = क्षमता;
	sdg->sgc->min_क्षमता = min_क्षमता;
	sdg->sgc->max_क्षमता = max_क्षमता;
}

/*
 * Check whether the capacity of the rq has been noticeably reduced by side
 * activity. The imbalance_pct is used for the threshold.
 * Return true is the capacity is reduced
 */
स्थिर अन्तःस्थ पूर्णाङ्क
check_cpu_क्षमता(संरचना rq *rq, संरचना sched_domain *sd)
{
	प्रत्यागम ((rq->cpu_क्षमता * sd->imbalance_pct) <
				(arch_scale_cpu_क्षमता(cpu_सम्बन्धि(rq)) * 100));
}

/* Check if the rq has a misfit task */
स्थिर अन्तःस्थ bool check_misfit_स्थितिः(संरचना rq *rq)
{
	प्रत्यागम rq->misfit_task_आरोपय;
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

स्थिर अन्तःस्थ पूर्णाङ्क sg_imbalanced(संरचना sched_गणः *गणः)
{
	प्रत्यागम गणः->sgc->imbalance;
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
स्थिर अन्तःस्थ bool
गणः_has_क्षमता(अचिह्नित पूर्णाङ्क imbalance_pct, संरचना sg_lb_stats *sgs)
{
	यदि (sgs->योग_nr_running < sgs->गणः_weight)
		प्रत्यागम true;

	यदि ((sgs->गणः_क्षमता * imbalance_pct) <
			(sgs->गणः_runnable * 100))
		प्रत्यागम false;

	यदि ((sgs->गणः_क्षमता * 100) >
			(sgs->गणः_util * imbalance_pct))
		प्रत्यागम true;

	प्रत्यागम false;
}

/*
 *  group_is_overloaded returns true if the group has more tasks than it can
 *  handle.
 *  group_is_overloaded is not equals to !group_has_capacity because a group
 *  with the exact right number of tasks, has no more spare capacity but is not
 *  overloaded so both group_has_capacity and group_is_overloaded return
 *  false.
 */
स्थिर अन्तःस्थ bool
गणः_is_overloaded(अचिह्नित पूर्णाङ्क imbalance_pct, संरचना sg_lb_stats *sgs)
{
	/*
	 * With EAS and uclamp, 1 CPU in the group must be overutilized to
	 * consider the group overloaded.
	 */
	यदि (sched_energy_enabled() && !sgs->गणः_overutilized)
		प्रत्यागम false;

	यदि (sgs->योग_nr_running <= sgs->गणः_weight)
		प्रत्यागम false;

	यदि ((sgs->गणः_क्षमता * 100) <
			(sgs->गणः_util * imbalance_pct))
		प्रत्यागम true;

	यदि ((sgs->गणः_क्षमता * imbalance_pct) <
			(sgs->गणः_runnable * 100))
		प्रत्यागम true;

	प्रत्यागम false;
}

स्थिर अन्तःस्थ गणना
गणः_type गणः_classify(अचिह्नित पूर्णाङ्क imbalance_pct,
			  संरचना sched_गणः *गणः,
			  संरचना sg_lb_stats *sgs)
{
	यदि (गणः_is_overloaded(imbalance_pct, sgs))
		प्रत्यागम गणः_overloaded;

	यदि (sgs->गणः_llc_balance)
		प्रत्यागम गणः_llc_balance;

	यदि (sg_imbalanced(गणः))
		प्रत्यागम गणः_imbalanced;

	यदि (sgs->गणः_asym_packing)
		प्रत्यागम गणः_asym_packing;

	यदि (sgs->गणः_smt_balance)
		प्रत्यागम गणः_smt_balance;

	यदि (sgs->गणः_misfit_task_आरोपय)
		प्रत्यागम गणः_misfit_task;

	यदि (!गणः_has_क्षमता(imbalance_pct, sgs))
		प्रत्यागम गणः_fully_busy;

	प्रत्यागम गणः_has_अतिरिक्तः;
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
स्थिर bool sched_use_asym_prio(संरचना sched_domain *sd, पूर्णाङ्क cpu)
{
	यदि (!(sd->flags & SD_ASYM_PACKING))
		प्रत्यागम false;

	यदि (!sched_smt_सक्रिय())
		प्रत्यागम true;

	प्रत्यागम sd->flags & SD_SHARE_CPUCAPACITY || is_core_idle(cpu);
}

स्थिर अन्तःस्थ bool sched_asym(संरचना sched_domain *sd, पूर्णाङ्क dst_cpu, पूर्णाङ्क src_cpu)
{
	/*
	 * First check if @dst_cpu can do asym_packing load balance. Only do it
	 * if it has higher priority than @src_cpu.
	 */
	प्रत्यागम sched_use_asym_prio(sd, dst_cpu) &&
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
स्थिर अन्तःस्थ bool
sched_गणः_asym(संरचना lb_env *env, संरचना sg_lb_stats *sgs, संरचना sched_गणः *गणः)
{
	/*
	 * CPU priorities do not make sense for SMT cores with more than one
	 * busy sibling.
	 */
	यदि ((गणः->flags & SD_SHARE_CPUCAPACITY) &&
	    (sgs->गणः_weight - sgs->idle_cpus != 1))
		प्रत्यागम false;

	प्रत्यागम sched_asym(env->sd, env->dst_cpu, READ_ONCE(गणः->asym_prefer_cpu));
}

/* One group has more than one SMT CPU while the other group does not */
स्थिर अन्तःस्थ bool smt_vs_nonsmt_groups(संरचना sched_गणः *sg1,
				    संरचना sched_गणः *sg2)
{
	यदि (!sg1 || !sg2)
		प्रत्यागम false;

	प्रत्यागम (sg1->flags & SD_SHARE_CPUCAPACITY) !=
		(sg2->flags & SD_SHARE_CPUCAPACITY);
}

स्थिर अन्तःस्थ bool smt_balance(संरचना lb_env *env, संरचना sg_lb_stats *sgs,
			       संरचना sched_गणः *गणः)
{
	यदि (!env->idle)
		प्रत्यागम false;

	/*
	 * For SMT source group, it is better to move a task
	 * to a CPU that doesn't have multiple tasks sharing its CPU capacity.
	 * Note that if a group has a single SMT, SD_SHARE_CPUCAPACITY
	 * will not be on.
	 */
	यदि (गणः->flags & SD_SHARE_CPUCAPACITY &&
	    sgs->योग_h_nr_running > 1)
		प्रत्यागम true;

	प्रत्यागम false;
}

स्थिर अन्तःस्थ दीर्घ sibling_imbalance(संरचना lb_env *env,
				    संरचना sd_lb_stats *sds,
				    संरचना sg_lb_stats *busiest,
				    संरचना sg_lb_stats *local)
{
	पूर्णाङ्क ncores_busiest, ncores_local;
	दीर्घ imbalance;

	यदि (!env->idle || !busiest->योग_nr_running)
		प्रत्यागम 0;

	ncores_busiest = sds->busiest->cores;
	ncores_local = sds->local->cores;

	यदि (ncores_busiest == ncores_local) {
		imbalance = busiest->योग_nr_running;
		lsub_धनात्मकम्(&imbalance, local->योग_nr_running);
		प्रत्यागम imbalance;
	}

	/* Balance such that nr_running/ncores ratio are same on both groups */
	imbalance = ncores_local * busiest->योग_nr_running;
	lsub_धनात्मकम्(&imbalance, ncores_busiest * local->योग_nr_running);
	/* Normalize imbalance and do rounding on normalization */
	imbalance = 2 * imbalance + ncores_local + ncores_busiest;
	imbalance /= ncores_local + ncores_busiest;

	/* Take advantage of resource in an empty sched group */
	यदि (imbalance <= 1 && local->योग_nr_running == 0 &&
	    busiest->योग_nr_running > 1)
		imbalance = 2;

	प्रत्यागम imbalance;
}

स्थिर अन्तःस्थ bool
sched_reduced_क्षमता(संरचना rq *rq, संरचना sched_domain *sd)
{
	/*
	 * When there is more than 1 task, the group_overloaded case already
	 * takes care of cpu with reduced capacity
	 */
	यदि (rq->cfs.h_nr_runnable != 1)
		प्रत्यागम false;

	प्रत्यागम check_cpu_क्षमता(rq, sd);
}

#यदि_परिभाषितम् CONFIG_SCHED_CACHE
/*
 * Record the statistics for this scheduler group for later
 * use. These values guide load balancing on aggregating tasks
 * to a LLC.
 */
स्थिर शून्य लेखः_sg_llc_stats(संरचना lb_env *env,
				संरचना sg_lb_stats *sgs,
				संरचना sched_गणः *गणः)
{
	संरचना sched_domain_shared *sd_share;
	पूर्णाङ्क cpu;

	यदि (!sched_cache_enabled() || env->idle == CPU_NEWLY_IDLE)
		प्रत्यागम;

	/* Only care about sched domain spanning multiple LLCs */
	यदि (env->sd->child != rcu_dereference_all(प्रत्येकम्_cpu(sd_llc, env->dst_cpu)))
		प्रत्यागम;

	/*
	 * A redo repeats stats published on the first pass. Hotplug can
	 * still leave env->cpus partial, so util would not match capacity.
	 */
	यदि ((env->flags & LBF_ALL_PINNED) ||
	    !cpumask_subset(sched_गणः_विस्तृतिः(गणः), env->cpus))
		प्रत्यागम;

	/*
	 * At this point we know this group spans a LLC domain.
	 * Record the statistic of this group in its corresponding
	 * shared LLC domain.
	 * Note: sd_share cannot be obtained via sd->child->shared,
	 * because the latter refers to the domain that covers the
	 * local group. Instead, sd_share should be located using
	 * the first CPU of the LLC group.
	 */
	cpu = cpumask_first(sched_गणः_विस्तृतिः(गणः));
	sd_share = rcu_dereference_all(प्रत्येकम्_cpu(sd_llc_shared, cpu));
	यदि (!sd_share)
		प्रत्यागम;

	यदि (READ_ONCE(sd_share->util_avg) != sgs->गणः_util)
		WRITE_ONCE(sd_share->util_avg, sgs->गणः_util);

	यदि (unlikely(READ_ONCE(sd_share->क्षमता) != sgs->गणः_क्षमता))
		WRITE_ONCE(sd_share->क्षमता, sgs->गणः_क्षमता);
}

/*
 * Do LLC balance on sched group that contains LLC, and have tasks preferring
 * to run on LLC in idle dst_cpu.
 */
स्थिर अन्तःस्थ bool llc_balance(संरचना lb_env *env, संरचना sg_lb_stats *sgs,
			       संरचना sched_गणः *गणः)
{
	यदि (!sched_cache_enabled())
		प्रत्यागम false;

	यदि (env->sd->flags & SD_SHARE_LLC)
		प्रत्यागम false;

	/*
	 * On asymmetric domains, group_misfit_task_load
	 * should be prioritized to move tasks to CPU that fit them
	 * over aggregating tasks to their preferred LLC.
	 */
	यदि ((env->sd->flags & SD_ASYM_CPUCAPACITY) &&
	    sgs->गणः_misfit_task_आरोपय)
		प्रत्यागम false;

	/*
	 * Skip cache aware tagging if nr_balanced_failed is sufficiently high.
	 * Threshold of cache_nice_tries is set to 1 higher than nr_balance_failed
	 * to avoid excessive task migration at the same time.
	 */
	यदि (env->sd->nr_balance_failed >= env->sd->cache_nice_tries + 1)
		प्रत्यागम false;

	यदि (sgs->nr_pref_dst_llc &&
	    can_migrate_llc(cpumask_first(sched_गणः_विस्तृतिः(गणः)),
			    env->dst_cpu, 0, true) == mig_llc)
		प्रत्यागम true;

	प्रत्यागम false;
}

स्थिर bool update_llc_busiest(संरचना lb_env *env,
			       संरचना sg_lb_stats *busiest,
			       संरचना sg_lb_stats *sgs)
{
	/*
	 * There are more tasks that want to run on dst_cpu's LLC.
	 */
	प्रत्यागम sgs->nr_pref_dst_llc > busiest->nr_pref_dst_llc;
}
#अन्यथा
स्थिर अन्तःस्थ शून्य लेखः_sg_llc_stats(संरचना lb_env *env, संरचना sg_lb_stats *sgs,
				       संरचना sched_गणः *गणः)
{
}

स्थिर अन्तःस्थ bool llc_balance(संरचना lb_env *env, संरचना sg_lb_stats *sgs,
			       संरचना sched_गणः *गणः)
{
	प्रत्यागम false;
}

स्थिर bool update_llc_busiest(संरचना lb_env *env,
			       संरचना sg_lb_stats *busiest,
			       संरचना sg_lb_stats *sgs)
{
	प्रत्यागम false;
}
#यदि_अन्तः

/**
 * update_sg_lb_stats - Update sched_group's statistics for load balancing.
 * @env: The load balancing environment.
 * @sds: Load-balancing data with statistics of the local group.
 * @group: sched_group whose statistics are to be updated.
 * @sgs: variable to hold the statistics for this group.
 * @sg_overloaded: sched_group is overloaded
 */
स्थिर अन्तःस्थ शून्य update_sg_lb_stats(संरचना lb_env *env,
				      संरचना sd_lb_stats *sds,
				      संरचना sched_गणः *गणः,
				      संरचना sg_lb_stats *sgs,
				      bool *sg_overloaded)
{
	पूर्णाङ्क i, nr_running, local_गणः, sd_flags = env->sd->flags;
	bool balancing_at_rd = !env->sd->parent;

	स्मृतिं_पूरय(sgs, 0, परिमाण(*sgs));

	local_गणः = गणः == sds->local;

	for_each_cpu_and(i, sched_गणः_विस्तृतिः(गणः), env->cpus) {
		संरचना rq *rq = cpu_rq(i);
		अचिह्नित दीर्घ आरोपय = cpu_आरोपय(rq);

		sgs->गणः_आरोपय += आरोपय;
		sgs->गणः_util += cpu_util_cfs(i);
		sgs->गणः_runnable += cpu_runnable(rq);
		sgs->योग_h_nr_running += rq->cfs.h_nr_runnable;

		nr_running = rq->nr_running;
		sgs->योग_nr_running += nr_running;

		यदि (cpu_overutilized(i))
			sgs->गणः_overutilized = 1;

#यदि_परिभाषितम् CONFIG_SCHED_CACHE
		यदि (sched_cache_enabled()) {
			संरचना sched_domain *sd_tmp;
			पूर्णाङ्क dst_llc;

			dst_llc = llc_id(env->dst_cpu);
			यदि (llc_id(i) != dst_llc) {
				sd_tmp = rcu_dereference_all(rq->sd);
				यदि (sd_tmp && (अचिह्नित पूर्णाङ्क)dst_llc < sd_tmp->llc_max)
					sgs->nr_pref_dst_llc += sd_tmp->llc_counts[dst_llc];
			}
		}
#यदि_अन्तः

		/*
		 * No need to call idle_cpu() if nr_running is not 0
		 */
		यदि (!nr_running && idle_cpu(i)) {
			sgs->idle_cpus++;
			/* Idle cpu can't have misfit task */
			अनुवर्त;
		}

		/* Overload indicator is only updated at root domain */
		यदि (balancing_at_rd && nr_running > 1)
			*sg_overloaded = 1;

#यदि_परिभाषितम् CONFIG_NUMA_BALANCING
		/* Only fbq_classify_group() uses this to classify NUMA groups */
		यदि (sd_flags & SD_NUMA) {
			sgs->nr_numa_running += rq->nr_numa_running;
			sgs->nr_preferred_running += rq->nr_preferred_running;
		}
#यदि_अन्तः
		यदि (local_गणः)
			अनुवर्त;

		यदि (sd_flags & SD_ASYM_CPUCAPACITY) {
			यदि (rq->misfit_task_आरोपय) {
				/*
				 * Always mark the root domain overloaded so big
				 * CPUs can pick up misfit tasks via newly idle
				 * balance.
				 */
				यदि (balancing_at_rd)
					*sg_overloaded = 1;

				/*
				 * Only account misfit load if @dst_cpu can
				 * help; otherwise, the group may be classified
				 * as misfit_task and update_sd_pick_busiest()
				 * will skip it.
				 */
				यदि (क्षमता_greater(क्षमता_सम्बन्धि(env->dst_cpu),
						     गणः->sgc->max_क्षमता) &&
				    (sgs->गणः_misfit_task_आरोपय < rq->misfit_task_आरोपय))
					sgs->गणः_misfit_task_आरोपय = rq->misfit_task_आरोपय;
			}
		} अन्यथा यदि (env->idle && sched_reduced_क्षमता(rq, env->sd)) {
			/* Check for a task running on a CPU with reduced capacity */
			यदि (sgs->गणः_misfit_task_आरोपय < आरोपय)
				sgs->गणः_misfit_task_आरोपय = आरोपय;
		}
	}

	sgs->गणः_क्षमता = गणः->sgc->क्षमता;

	sgs->गणः_weight = गणः->गणः_weight;

	यदि (!local_गणः) {
		/* Check if dst CPU is idle and preferred to this group */
		यदि (env->idle && sgs->योग_h_nr_running &&
		    sched_गणः_asym(env, sgs, गणः))
			sgs->गणः_asym_packing = 1;

		/* Check for loaded SMT group to be balanced to dst CPU */
		यदि (smt_balance(env, sgs, गणः))
			sgs->गणः_smt_balance = 1;

		/* Check for tasks in this group can be moved to their preferred LLC */
		यदि (llc_balance(env, sgs, गणः))
			sgs->गणः_llc_balance = 1;
	}

	sgs->गणः_type = गणः_classify(env->sd->imbalance_pct, गणः, sgs);

	लेखः_sg_llc_stats(env, sgs, गणः);
	/* Computing avg_load makes sense only when group is overloaded */
	यदि (sgs->गणः_type == गणः_overloaded)
		sgs->avg_आरोपय = (sgs->गणः_आरोपय * SCHED_क्षमता_SCALE) /
				sgs->गणः_क्षमता;
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
स्थिर bool update_sd_pick_busiest(संरचना lb_env *env,
				   संरचना sd_lb_stats *sds,
				   संरचना sched_गणः *sg,
				   संरचना sg_lb_stats *sgs)
{
	संरचना sg_lb_stats *busiest = &sds->busiest_stat;

	/* Make sure that there is at least one task to pull */
	यदि (!sgs->योग_h_nr_running)
		प्रत्यागम false;

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
	यदि ((env->sd->flags & SD_ASYM_CPUCAPACITY) &&
	    (sgs->गणः_type == गणः_misfit_task) &&
	    (!env->dst_core_idle ||
	     !क्षमता_greater(क्षमता_सम्बन्धि(env->dst_cpu), sg->sgc->max_क्षमता) ||
	     sds->local_stat.गणः_type != गणः_has_अतिरिक्तः))
		प्रत्यागम false;

	/*
	 * Candidate sg has no more than one task per CPU and has higher
	 * per-CPU capacity. Migrating tasks to less capable CPUs may harm
	 * throughput. Maximize throughput, power/energy consequences are not
	 * considered.
	 */
	यदि ((env->sd->flags & SD_ASYM_CPUCAPACITY) &&
	    (sgs->गणः_type <= गणः_fully_busy) &&
	    (क्षमता_greater(sg->sgc->min_क्षमता, क्षमता_सम्बन्धि(env->dst_cpu))))
		प्रत्यागम false;

	यदि (sgs->गणः_type > busiest->गणः_type)
		प्रत्यागम true;

	यदि (sgs->गणः_type < busiest->गणः_type)
		प्रत्यागम false;

	/*
	 * The candidate and the current busiest group are the same type of
	 * group. Let check which one is the busiest according to the type.
	 */

	विकल्प (sgs->गणः_type) {
	पक्ष गणः_overloaded:
		/* Select the overloaded group with highest avg_load. */
		प्रत्यागम sgs->avg_आरोपय > busiest->avg_आरोपय;

	पक्ष गणः_llc_balance:
		/* Select the group with most tasks preferring dst LLC */
		प्रत्यागम update_llc_busiest(env, busiest, sgs);

	पक्ष गणः_imbalanced:
		/*
		 * Select the 1st imbalanced group as we don't have any way to
		 * choose one more than another.
		 */
		प्रत्यागम false;

	पक्ष गणः_asym_packing:
		/* Prefer to move from lowest priority CPU's work */
		प्रत्यागम sched_asym_prefer(READ_ONCE(sds->busiest->asym_prefer_cpu),
					 READ_ONCE(sg->asym_prefer_cpu));

	पक्ष गणः_misfit_task:
		/*
		 * If we have more than one misfit sg go with the biggest
		 * misfit.
		 */
		प्रत्यागम sgs->गणः_misfit_task_आरोपय > busiest->गणः_misfit_task_आरोपय;

	पक्ष गणः_smt_balance:
		/*
		 * Check if we have spare CPUs on either SMT group to
		 * choose has spare or fully busy handling.
		 */
		यदि (sgs->idle_cpus != 0 || busiest->idle_cpus != 0)
			गच्छ has_अतिरिक्तः;

		fallthrough;

	पक्ष गणः_fully_busy:
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

		यदि (sgs->avg_आरोपय < busiest->avg_आरोपय)
			प्रत्यागम false;

		यदि (sgs->avg_आरोपय == busiest->avg_आरोपय) {
			/*
			 * SMT sched groups need more help than non-SMT groups.
			 * If @sg happens to also be SMT, either choice is good.
			 */
			यदि (sds->busiest->flags & SD_SHARE_CPUCAPACITY)
				प्रत्यागम false;
		}

		भङ्ग;

	पक्ष गणः_has_अतिरिक्तः:
		/*
		 * Do not pick sg with SMT CPUs over sg with pure CPUs,
		 * as we do not want to pull task off SMT core with one task
		 * and make the core idle.
		 */
		यदि (smt_vs_nonsmt_groups(sds->busiest, sg)) {
			यदि (sg->flags & SD_SHARE_CPUCAPACITY && sgs->योग_h_nr_running <= 1)
				प्रत्यागम false;
			अन्यथा
				प्रत्यागम true;
		}
has_अतिरिक्तः:

		/*
		 * Select not overloaded group with lowest number of idle CPUs
		 * and highest number of running tasks. We could also compare
		 * the spare capacity which is more stable but it can end up
		 * that the group has less spare capacity but finally more idle
		 * CPUs which means less opportunity to pull tasks.
		 */
		यदि (sgs->idle_cpus > busiest->idle_cpus)
			प्रत्यागम false;
		अन्यथा यदि ((sgs->idle_cpus == busiest->idle_cpus) &&
			 (sgs->योग_nr_running <= busiest->योग_nr_running))
			प्रत्यागम false;

		भङ्ग;
	}

	प्रत्यागम true;
}

#यदि_परिभाषितम् CONFIG_NUMA_BALANCING
स्थिर अन्तःस्थ गणना fbq_type fbq_classify_गणः(संरचना sg_lb_stats *sgs)
{
	यदि (sgs->योग_h_nr_running > sgs->nr_numa_running)
		प्रत्यागम regular;
	यदि (sgs->योग_h_nr_running > sgs->nr_preferred_running)
		प्रत्यागम remote;
	प्रत्यागम all;
}

स्थिर अन्तःस्थ गणना fbq_type fbq_classify_rq(संरचना rq *rq)
{
	यदि (rq->nr_running > rq->nr_numa_running)
		प्रत्यागम regular;
	यदि (rq->nr_running > rq->nr_preferred_running)
		प्रत्यागम remote;
	प्रत्यागम all;
}
#अन्यथा /* !CONFIG_NUMA_BALANCING: */
स्थिर अन्तःस्थ गणना fbq_type fbq_classify_गणः(संरचना sg_lb_stats *sgs)
{
	प्रत्यागम all;
}

स्थिर अन्तःस्थ गणना fbq_type fbq_classify_rq(संरचना rq *rq)
{
	प्रत्यागम regular;
}
#यदि_अन्तः /* !CONFIG_NUMA_BALANCING */


संरचना sg_lb_stats;

/*
 * task_running_on_cpu - return 1 if @p is running on @cpu.
 */

स्थिर अचिह्नित पूर्णाङ्क task_running_on_cpu(पूर्णाङ्क cpu, संरचना task_struct *p)
{
	/* Task has no contribution or is new */
	यदि (cpu != task_cpu(p) || !READ_ONCE(p->se.avg.चरमः_update_time))
		प्रत्यागम 0;

	यदि (task_on_rq_queued(p))
		प्रत्यागम 1;

	प्रत्यागम 0;
}

/**
 * idle_cpu_without - would a given CPU be idle without p ?
 * @cpu: the processor on which idleness is tested.
 * @p: task which should be ignored.
 *
 * Return: 1 if the CPU would be idle. 0 otherwise.
 */
स्थिर पूर्णाङ्क idle_cpu_without(पूर्णाङ्क cpu, संरचना task_struct *p)
{
	संरचना rq *rq = cpu_rq(cpu);

	यदि (rq->curr != rq->idle && rq->curr != p)
		प्रत्यागम 0;

	/*
	 * rq->nr_running can't be used but an updated version without the
	 * impact of p on cpu must be used instead. The updated nr_running
	 * be computed and tested before calling idle_cpu_without().
	 */

	यदि (rq->ttwu_pending)
		प्रत्यागम 0;

	प्रत्यागम 1;
}

/*
 * update_sg_wakeup_stats - Update sched_group's statistics for wakeup.
 * @sd: The sched_domain level to look for idlest group.
 * @group: sched_group whose statistics are to be updated.
 * @sgs: variable to hold the statistics for this group.
 * @p: The task for which we look for the idlest group/CPU.
 */
स्थिर अन्तःस्थ शून्य update_sg_wakeup_stats(संरचना sched_domain *sd,
					  संरचना sched_गणः *गणः,
					  संरचना sg_lb_stats *sgs,
					  संरचना task_struct *p)
{
	पूर्णाङ्क i, nr_running;

	स्मृतिं_पूरय(sgs, 0, परिमाण(*sgs));

	/* Assume that task can't fit any CPU of the group */
	यदि (sd->flags & SD_ASYM_CPUCAPACITY)
		sgs->गणः_misfit_task_आरोपय = 1;

	for_each_cpu_and(i, sched_गणः_विस्तृतिः(गणः), p->cpus_ptr) {
		संरचना rq *rq = cpu_rq(i);
		अचिह्नित पूर्णाङ्क local;

		sgs->गणः_आरोपय += cpu_आरोपय_without(rq, p);
		sgs->गणः_util += cpu_util_without(i, p);
		sgs->गणः_runnable += cpu_runnable_without(rq, p);
		local = task_running_on_cpu(i, p);
		sgs->योग_h_nr_running += rq->cfs.h_nr_runnable - local;

		nr_running = rq->nr_running - local;
		sgs->योग_nr_running += nr_running;

		/*
		 * No need to call idle_cpu_without() if nr_running is not 0
		 */
		यदि (!nr_running && idle_cpu_without(i, p))
			sgs->idle_cpus++;

		/* Check if task fits in the CPU */
		यदि (sd->flags & SD_ASYM_CPUCAPACITY &&
		    sgs->गणः_misfit_task_आरोपय &&
		    task_माति_cpu(p, i))
			sgs->गणः_misfit_task_आरोपय = 0;

	}

	sgs->गणः_क्षमता = गणः->sgc->क्षमता;

	sgs->गणः_weight = गणः->गणः_weight;

	sgs->गणः_type = गणः_classify(sd->imbalance_pct, गणः, sgs);

	/*
	 * Computing avg_load makes sense only when group is fully busy or
	 * overloaded
	 */
	यदि (sgs->गणः_type == गणः_fully_busy ||
		sgs->गणः_type == गणः_overloaded)
		sgs->avg_आरोपय = (sgs->गणः_आरोपय * SCHED_क्षमता_SCALE) /
				sgs->गणः_क्षमता;
}

स्थिर bool update_pick_idlest(संरचना sched_गणः *idlest,
			       संरचना sg_lb_stats *idlest_sgs,
			       संरचना sched_गणः *गणः,
			       संरचना sg_lb_stats *sgs)
{
	यदि (sgs->गणः_type < idlest_sgs->गणः_type)
		प्रत्यागम true;

	यदि (sgs->गणः_type > idlest_sgs->गणः_type)
		प्रत्यागम false;

	/*
	 * The candidate and the current idlest group are the same type of
	 * group. Let check which one is the idlest according to the type.
	 */

	विकल्प (sgs->गणः_type) {
	पक्ष गणः_overloaded:
	पक्ष गणः_fully_busy:
		/* Select the group with lowest avg_load. */
		यदि (idlest_sgs->avg_आरोपय <= sgs->avg_आरोपय)
			प्रत्यागम false;
		भङ्ग;

	पक्ष गणः_llc_balance:
	पक्ष गणः_imbalanced:
	पक्ष गणः_asym_packing:
	पक्ष गणः_smt_balance:
		/* Those types are not used in the slow wakeup path */
		प्रत्यागम false;

	पक्ष गणः_misfit_task:
		/* Select group with the highest max capacity */
		यदि (idlest->sgc->max_क्षमता >= गणः->sgc->max_क्षमता)
			प्रत्यागम false;
		भङ्ग;

	पक्ष गणः_has_अतिरिक्तः:
		/* Select group with most idle CPUs */
		यदि (idlest_sgs->idle_cpus > sgs->idle_cpus)
			प्रत्यागम false;

		/* Select group with lowest group_util */
		यदि (idlest_sgs->idle_cpus == sgs->idle_cpus &&
			idlest_sgs->गणः_util <= sgs->गणः_util)
			प्रत्यागम false;

		भङ्ग;
	}

	प्रत्यागम true;
}

/*
 * sched_balance_find_dst_group() finds and returns the least busy CPU group within the
 * domain.
 *
 * Assumes p is allowed on at least one CPU in sd.
 */
स्थिर संरचना sched_गणः *
sched_balance_find_dst_गणः(संरचना sched_domain *sd, संरचना task_struct *p, पूर्णाङ्क this_cpu)
{
	संरचना sched_गणः *idlest = रिक्त, *local = रिक्त, *गणः = sd->groups;
	संरचना sg_lb_stats local_sgs, tmp_sgs;
	संरचना sg_lb_stats *sgs;
	अचिह्नित दीर्घ imbalance;
	संरचना sg_lb_stats idlest_sgs = {
			.avg_आरोपय = UINT_MAX,
			.गणः_type = गणः_overloaded,
	};

	कुरु {
		पूर्णाङ्क local_गणः;

		/* Skip over this group if it has no CPUs allowed */
		यदि (!cpumask_intersects(sched_गणः_विस्तृतिः(गणः),
					p->cpus_ptr))
			अनुवर्त;

		/* Skip over this group if no cookie matched */
		यदि (!sched_गणः_cookie_मेलनम्(cpu_rq(this_cpu), p, गणः))
			अनुवर्त;

		local_गणः = cpumask_test_cpu(this_cpu,
					       sched_गणः_विस्तृतिः(गणः));

		यदि (local_गणः) {
			sgs = &local_sgs;
			local = गणः;
		} अन्यथा {
			sgs = &tmp_sgs;
		}

		update_sg_wakeup_stats(sd, गणः, sgs, p);

		यदि (!local_गणः && update_pick_idlest(idlest, &idlest_sgs, गणः, sgs)) {
			idlest = गणः;
			idlest_sgs = *sgs;
		}

	} यावत् (गणः = गणः->अग्र, गणः != sd->groups);


	/* There is no idlest group to push tasks to */
	यदि (!idlest)
		प्रत्यागम रिक्त;

	/* The local group has been skipped because of CPU affinity */
	यदि (!local)
		प्रत्यागम idlest;

	/*
	 * If the local group is idler than the selected idlest group
	 * don't try and push the task.
	 */
	यदि (local_sgs.गणः_type < idlest_sgs.गणः_type)
		प्रत्यागम रिक्त;

	/*
	 * If the local group is busier than the selected idlest group
	 * try and push the task.
	 */
	यदि (local_sgs.गणः_type > idlest_sgs.गणः_type)
		प्रत्यागम idlest;

	विकल्प (local_sgs.गणः_type) {
	पक्ष गणः_overloaded:
	पक्ष गणः_fully_busy:

		/* Calculate allowed imbalance based on load */
		imbalance = scale_आरोपय_अधस्तात्(NICE_0_आरोपय) *
				(sd->imbalance_pct-100) / 100;

		/*
		 * When comparing groups across NUMA domains, it's possible for
		 * the local domain to be very lightly loaded relative to the
		 * remote domains but "imbalance" skews the comparison making
		 * remote CPUs look much more favourable. When considering
		 * cross-domain, add imbalance to the load on the remote node
		 * and consider staying local.
		 */

		यदि ((sd->flags & SD_NUMA) &&
		    ((idlest_sgs.avg_आरोपय + imbalance) >= local_sgs.avg_आरोपय))
			प्रत्यागम रिक्त;

		/*
		 * If the local group is less loaded than the selected
		 * idlest group don't try and push any tasks.
		 */
		यदि (idlest_sgs.avg_आरोपय >= (local_sgs.avg_आरोपय + imbalance))
			प्रत्यागम रिक्त;

		यदि (100 * local_sgs.avg_आरोपय <= sd->imbalance_pct * idlest_sgs.avg_आरोपय)
			प्रत्यागम रिक्त;
		भङ्ग;

	पक्ष गणः_llc_balance:
	पक्ष गणः_imbalanced:
	पक्ष गणः_asym_packing:
	पक्ष गणः_smt_balance:
		/* Those type are not used in the slow wakeup path */
		प्रत्यागम रिक्त;

	पक्ष गणः_misfit_task:
		/* Select group with the highest max capacity */
		यदि (local->sgc->max_क्षमता >= idlest->sgc->max_क्षमता)
			प्रत्यागम रिक्त;
		भङ्ग;

	पक्ष गणः_has_अतिरिक्तः:
#यदि_परिभाषितम् CONFIG_NUMA
		यदि (sd->flags & SD_NUMA) {
			पूर्णाङ्क imb_numa_nr = sd->imb_numa_nr;
#यदि_परिभाषितम् CONFIG_NUMA_BALANCING
			पूर्णाङ्क idlest_cpu;
			/*
			 * If there is spare capacity at NUMA, try to select
			 * the preferred node
			 */
			यदि (cpu_गन्तव्यम्_ग्रन्थिः(this_cpu) == p->numa_preferred_nid)
				प्रत्यागम रिक्त;

			idlest_cpu = cpumask_first(sched_गणः_विस्तृतिः(idlest));
			यदि (cpu_गन्तव्यम्_ग्रन्थिः(idlest_cpu) == p->numa_preferred_nid)
				प्रत्यागम idlest;
#यदि_अन्तः /* CONFIG_NUMA_BALANCING */
			/*
			 * Otherwise, keep the task close to the wakeup source
			 * and improve locality if the number of running tasks
			 * would remain below threshold where an imbalance is
			 * allowed while accounting for the possibility the
			 * task is pinned to a subset of CPUs. If there is a
			 * real need of migration, periodic load balance will
			 * take care of it.
			 */
			यदि (p->nr_cpus_allowed != NR_CPUS) {
				अचिह्नित पूर्णाङ्क w = cpumask_weight_and(p->cpus_ptr,
								sched_गणः_विस्तृतिः(local));
				imb_numa_nr = min(w, sd->imb_numa_nr);
			}

			imbalance = निरपेक्षम्(local_sgs.idle_cpus - idlest_sgs.idle_cpus);
			यदि (!adjust_numa_imbalance(imbalance,
						   local_sgs.योग_nr_running + 1,
						   imb_numa_nr)) {
				प्रत्यागम रिक्त;
			}
		}
#यदि_अन्तः /* CONFIG_NUMA */

		/*
		 * Select group with highest number of idle CPUs. We could also
		 * compare the utilization which is more stable but it can end
		 * up that the group has less spare capacity but finally more
		 * idle CPUs which means more opportunity to run task.
		 */
		यदि (local_sgs.idle_cpus >= idlest_sgs.idle_cpus)
			प्रत्यागम रिक्त;
		भङ्ग;
	}

	प्रत्यागम idlest;
}

स्थिर शून्य update_idle_cpu_अवलोकनम्(संरचना lb_env *env,
				 अचिह्नित दीर्घ योग_util)
{
	संरचना sched_domain_shared *sd_share;
	संरचना sched_domain *sd = env->sd;
	पूर्णाङ्क llc_weight, pct;
	u64 x, y, tmp;
	/*
	 * Update the number of CPUs to scan in LLC domain, which could
	 * be used as a hint in select_idle_cpu(). The update of sd_share
	 * could be expensive because it is within a shared cache line.
	 * So the write of this hint only occurs during periodic load
	 * balancing, rather than CPU_NEWLY_IDLE, because the latter
	 * can fire way more frequently than the former.
	 */
	यदि (!sched_feat(SIS_UTIL) || env->idle == CPU_NEWLY_IDLE)
		प्रत्यागम;

	sd_share = sd->shared;
	यदि (!sd_share)
		प्रत्यागम;

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
	x = योग_util;
	llc_weight = sd->विस्तृतिः_weight;
	do_div(x, llc_weight);

	/* equation [4] */
	pct = sd->imbalance_pct;
	tmp = x * x * pct * pct;
	do_div(tmp, 10000 * SCHED_क्षमता_SCALE);
	tmp = min_t(दीर्घ, tmp, SCHED_क्षमता_SCALE);
	y = SCHED_क्षमता_SCALE - tmp;

	/* equation [2] */
	y *= llc_weight;
	do_div(y, SCHED_क्षमता_SCALE);
	यदि ((पूर्णाङ्क)y != sd_share->nr_idle_अवलोकनम्)
		WRITE_ONCE(sd_share->nr_idle_अवलोकनम्, (पूर्णाङ्क)y);
}

/**
 * update_sd_lb_stats - Update sched_domain's statistics for load balancing.
 * @env: The load balancing environment.
 * @sds: variable to hold the statistics for this sched_domain.
 */

स्थिर अन्तःस्थ शून्य update_sd_lb_stats(संरचना lb_env *env, संरचना sd_lb_stats *sds)
{
	संरचना sched_गणः *sg = env->sd->groups;
	संरचना sg_lb_stats *local = &sds->local_stat;
	संरचना sg_lb_stats tmp_sgs;
	अचिह्नित दीर्घ योग_util = 0;
	bool sg_overloaded = 0, sg_overutilized = 0;

	env->dst_core_idle = !sched_smt_सक्रिय() || is_core_idle(env->dst_cpu);

	कुरु {
		संरचना sg_lb_stats *sgs = &tmp_sgs;
		पूर्णाङ्क local_गणः;

		local_गणः = cpumask_test_cpu(env->dst_cpu, sched_गणः_विस्तृतिः(sg));
		यदि (local_गणः) {
			sds->local = sg;
			sgs = local;

			यदि (env->idle != CPU_NEWLY_IDLE ||
			    time_after_eq(jiffies, sg->sgc->अग्र_update))
				update_गणः_क्षमता(env->sd, env->dst_cpu);
		}

		update_sg_lb_stats(env, sds, sg, sgs, &sg_overloaded);

		यदि (!local_गणः && update_sd_pick_busiest(env, sds, sg, sgs)) {
			sds->busiest = sg;
			sds->busiest_stat = *sgs;
		}

		sg_overutilized |= sgs->गणः_overutilized;

		/* Now, start updating sd_lb_stats */
		sds->योग_आरोपय += sgs->गणः_आरोपय;
		sds->योग_क्षमता += sgs->गणः_क्षमता;

		योग_util += sgs->गणः_util;
		sg = sg->अग्र;
	} यावत् (sg != env->sd->groups);

	/*
	 * Indicate that the child domain of the busiest group prefers tasks
	 * go to a child's sibling domains first. NB the flags of a sched group
	 * are those of the child domain.
	 */
	यदि (sds->busiest)
		sds->prefer_sibling = !!(sds->busiest->flags & SD_PREFER_SIBLING);


	यदि (env->sd->flags & SD_NUMA)
		env->fbq_type = fbq_classify_गणः(&sds->busiest_stat);

	यदि (!env->sd->parent) {
		/* update overload indicator if we are at root domain */
		समुच्चयः_rd_overloaded(env->dst_rq->rd, sg_overloaded);

		/* Update over-utilization (tipping point, U >= 0) indicator */
		समुच्चयः_rd_overutilized(env->dst_rq->rd, sg_overutilized);
	} अन्यथा यदि (sg_overutilized) {
		समुच्चयः_rd_overutilized(env->dst_rq->rd, sg_overutilized);
	}

	update_idle_cpu_अवलोकनम्(env, योग_util);
}

/**
 * calculate_imbalance - Calculate the amount of imbalance present within the
 *			 groups of a given sched_domain during load balance.
 * @env: load balance environment
 * @sds: statistics of the sched_domain whose imbalance is to be calculated.
 */
स्थिर अन्तःस्थ शून्य calculate_imbalance(संरचना lb_env *env, संरचना sd_lb_stats *sds)
{
	संरचना sg_lb_stats *local, *busiest;

	local = &sds->local_stat;
	busiest = &sds->busiest_stat;

	यदि (busiest->गणः_type == गणः_misfit_task) {
		यदि (env->sd->flags & SD_ASYM_CPUCAPACITY) {
			/* Set imbalance to allow misfit tasks to be balanced. */
			env->migration_type = migrate_misfit;
			env->imbalance = 1;
		} अन्यथा {
			/*
			 * Set load imbalance to allow moving task from cpu
			 * with reduced capacity.
			 */
			env->migration_type = migrate_आरोपय;
			env->imbalance = busiest->गणः_misfit_task_आरोपय;
		}
		प्रत्यागम;
	}

	यदि (busiest->गणः_type == गणः_asym_packing) {
		/*
		 * In case of asym capacity, we will try to migrate all load to
		 * the preferred CPU.
		 */
		env->migration_type = migrate_task;
		env->imbalance = busiest->योग_h_nr_running;
		प्रत्यागम;
	}

	यदि (busiest->गणः_type == गणः_smt_balance) {
		/* Reduce number of tasks sharing CPU capacity */
		env->migration_type = migrate_task;
		env->imbalance = 1;
		प्रत्यागम;
	}

#यदि_परिभाषितम् CONFIG_SCHED_CACHE
	यदि (busiest->गणः_type == गणः_llc_balance) {
		/* Move a task that prefer local LLC */
		env->migration_type = migrate_llc_task;
		env->imbalance = 1;
		प्रत्यागम;
	}
#यदि_अन्तः

	यदि (busiest->गणः_type == गणः_imbalanced) {
		/*
		 * In the group_imb case we cannot rely on group-wide averages
		 * to ensure CPU-load equilibrium, try to move any task to fix
		 * the imbalance. The next load balance will take care of
		 * balancing back the system.
		 */
		env->migration_type = migrate_task;
		env->imbalance = 1;
		प्रत्यागम;
	}

	/*
	 * Try to use spare capacity of local group without overloading it or
	 * emptying busiest.
	 */
	यदि (local->गणः_type == गणः_has_अतिरिक्तः) {
		यदि ((busiest->गणः_type > गणः_fully_busy) &&
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
			env->imbalance = max(local->गणः_क्षमता, local->गणः_util) -
					 local->गणः_util;

			/*
			 * In some cases, the group's utilization is max or even
			 * higher than capacity because of migrations but the
			 * local CPU is (newly) idle. There is at least one
			 * waiting task in this overloaded busiest group. Let's
			 * try to pull it.
			 */
			यदि (env->idle && env->imbalance == 0) {
				env->migration_type = migrate_task;
				env->imbalance = 1;
			}

			प्रत्यागम;
		}

		यदि (busiest->गणः_weight == 1 || sds->prefer_sibling) {
			/*
			 * When prefer sibling, evenly spread running tasks on
			 * groups.
			 */
			env->migration_type = migrate_task;
			env->imbalance = sibling_imbalance(env, sds, busiest, local);
		} अन्यथा {

			/*
			 * If there is no overload, we just want to even the number of
			 * idle CPUs.
			 */
			env->migration_type = migrate_task;
			env->imbalance = max_t(दीर्घ, 0,
					       (local->idle_cpus - busiest->idle_cpus));
		}

#यदि_परिभाषितम् CONFIG_NUMA
		/* Consider allowing a small imbalance between NUMA groups */
		यदि (env->sd->flags & SD_NUMA) {
			env->imbalance = adjust_numa_imbalance(env->imbalance,
							       local->योग_nr_running + 1,
							       env->sd->imb_numa_nr);
		}
#यदि_अन्तः

		/* Number of tasks to move to restore balance */
		env->imbalance >>= 1;

		प्रत्यागम;
	}

	/*
	 * Local is fully busy but has to take more load to relieve the
	 * busiest group
	 */
	यदि (local->गणः_type < गणः_overloaded) {
		/*
		 * Local will become overloaded so the avg_load metrics are
		 * finally needed.
		 */

		local->avg_आरोपय = (local->गणः_आरोपय * SCHED_क्षमता_SCALE) /
				  local->गणः_क्षमता;

		/*
		 * If the local group is more loaded than the selected
		 * busiest group don't try to pull any tasks.
		 */
		यदि (local->avg_आरोपय >= busiest->avg_आरोपय) {
			env->imbalance = 0;
			प्रत्यागम;
		}

		sds->avg_आरोपय = (sds->योग_आरोपय * SCHED_क्षमता_SCALE) /
				sds->योग_क्षमता;

		/*
		 * If the local group is more loaded than the average system
		 * load, don't try to pull any tasks.
		 */
		यदि (local->avg_आरोपय >= sds->avg_आरोपय) {
			env->imbalance = 0;
			प्रत्यागम;
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
	env->migration_type = migrate_आरोपय;
	env->imbalance = min(
		(busiest->avg_आरोपय - sds->avg_आरोपय) * busiest->गणः_क्षमता,
		(sds->avg_आरोपय - local->avg_आरोपय) * local->गणः_क्षमता
	) / SCHED_क्षमता_SCALE;
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
स्थिर संरचना sched_गणः *sched_balance_find_src_गणः(संरचना lb_env *env)
{
	संरचना sg_lb_stats *local, *busiest;
	संरचना sd_lb_stats sds;

	init_sd_lb_stats(&sds);

	/*
	 * Compute the various statistics relevant for load balancing at
	 * this level.
	 */
	update_sd_lb_stats(env, &sds);

	/* There is no busy sibling group to pull tasks from */
	यदि (!sds.busiest)
		गच्छ बहिर्गमनम्_balanced;

	busiest = &sds.busiest_stat;

	/* Misfit tasks should be dealt with regardless of the avg load */
	यदि (busiest->गणः_type == गणः_misfit_task)
		गच्छ force_balance;

	यदि (!is_rd_overutilized(env->dst_rq->rd) &&
	    rcu_dereference_all(env->dst_rq->rd->pd))
		गच्छ बहिर्गमनम्_balanced;

	/* ASYM feature bypasses nice load balance check */
	यदि (busiest->गणः_type == गणः_asym_packing)
		गच्छ force_balance;

	/*
	 * If the busiest group is imbalanced the below checks don't
	 * work because they assume all things are equal, which typically
	 * isn't true due to cpus_ptr constraints and the like.
	 */
	यदि (busiest->गणः_type == गणः_imbalanced)
		गच्छ force_balance;

	local = &sds.local_stat;
	/*
	 * If the local group is busier than the selected busiest group
	 * don't try and pull any tasks.
	 */
	यदि (local->गणः_type > busiest->गणः_type)
		गच्छ बहिर्गमनम्_balanced;

	/*
	 * When groups are overloaded, use the avg_load to ensure fairness
	 * between tasks.
	 */
	यदि (local->गणः_type == गणः_overloaded) {
		/*
		 * If the local group is more loaded than the selected
		 * busiest group don't try to pull any tasks.
		 */
		यदि (local->avg_आरोपय >= busiest->avg_आरोपय)
			गच्छ बहिर्गमनम्_balanced;

		/* XXX broken for overlapping NUMA groups */
		sds.avg_आरोपय = (sds.योग_आरोपय * SCHED_क्षमता_SCALE) /
				sds.योग_क्षमता;

		/*
		 * Don't pull any tasks if this group is already above the
		 * domain average load.
		 */
		यदि (local->avg_आरोपय >= sds.avg_आरोपय)
			गच्छ बहिर्गमनम्_balanced;

		/*
		 * If the busiest group is more loaded, use imbalance_pct to be
		 * conservative.
		 */
		यदि (100 * busiest->avg_आरोपय <=
				env->sd->imbalance_pct * local->avg_आरोपय)
			गच्छ बहिर्गमनम्_balanced;
	}

	/*
	 * Try to move all excess tasks to a sibling domain of the busiest
	 * group's child domain.
	 */
	यदि (sds.prefer_sibling && local->गणः_type == गणः_has_अतिरिक्तः &&
	    (busiest->गणः_type == गणः_llc_balance ||
	    sibling_imbalance(env, &sds, busiest, local) > 1))
		गच्छ force_balance;

	यदि (busiest->गणः_type != गणः_overloaded) {
		यदि (!env->idle) {
			/*
			 * If the busiest group is not overloaded (and as a
			 * result the local one too) but this CPU is already
			 * busy, let another idle CPU try to pull task.
			 */
			गच्छ बहिर्गमनम्_balanced;
		}

		यदि (busiest->गणः_type == गणः_smt_balance &&
		    smt_vs_nonsmt_groups(sds.local, sds.busiest)) {
			/* Let non SMT CPU pull from SMT CPU sharing with sibling */
			गच्छ force_balance;
		}

		यदि (busiest->गणः_weight > 1 &&
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
			गच्छ बहिर्गमनम्_balanced;
		}

		यदि (busiest->योग_h_nr_running == 1) {
			/*
			 * busiest doesn't have any tasks waiting to run
			 */
			गच्छ बहिर्गमनम्_balanced;
		}
	}

force_balance:
	/* Looks like there is an imbalance. Compute it */
	calculate_imbalance(env, &sds);
	प्रत्यागम env->imbalance ? sds.busiest : रिक्त;

बहिर्गमनम्_balanced:
	env->imbalance = 0;
	प्रत्यागम रिक्त;
}

/*
 * sched_balance_find_src_rq - find the busiest runqueue among the CPUs in the group.
 */
स्थिर संरचना rq *sched_balance_find_src_rq(संरचना lb_env *env,
				     संरचना sched_गणः *गणः)
{
	संरचना rq *busiest = रिक्त, *rq;
	अचिह्नित दीर्घ busiest_util = 0, busiest_आरोपय = 0, busiest_क्षमता = 1;
	अचिह्नित पूर्णाङ्क __maybe_unused busiest_pref_llc = 0;
	संरचना sched_domain __maybe_unused *sd_tmp;
	अचिह्नित पूर्णाङ्क busiest_nr = 0;
	पूर्णाङ्क __maybe_unused dst_llc;
	पूर्णाङ्क i;

	for_each_cpu_and(i, sched_गणः_विस्तृतिः(गणः), env->cpus) {
		अचिह्नित दीर्घ क्षमता, आरोपय, util;
		अचिह्नित पूर्णाङ्क nr_running;
		गणना fbq_type rt;

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
		यदि (rt > env->fbq_type)
			अनुवर्त;

		nr_running = rq->cfs.h_nr_runnable;
		यदि (!nr_running)
			अनुवर्त;

		क्षमता = क्षमता_सम्बन्धि(i);

		/*
		 * For ASYM_CPUCAPACITY domains, don't pick a CPU that could
		 * eventually lead to active_balancing high->low capacity.
		 * Higher per-CPU capacity is considered better than balancing
		 * average load.
		 */
		यदि (env->sd->flags & SD_ASYM_CPUCAPACITY &&
		    nr_running == 1) {
			bool cluster_equal_cap = static_branch_unlikely(&sched_cluster_सक्रिय) &&
						 (get_actual_cpu_क्षमता(env->dst_cpu) ==
						  get_actual_cpu_क्षमता(i));
			bool smt_degraded_cap = sched_smt_सक्रिय() && !is_core_idle(i);

			/*
			 * Busy SMT siblings reduce the capacity of CPU @i. Do
			 * not skip it in this case.
			 *
			 * CONFIG_SCHED_CLUSTER requires balancing load across
			 * clusters of identical capacity, accounting for
			 * hardware and cpufreq pressure.
			 */
			यदि (!smt_degraded_cap && !cluster_equal_cap &&
			    !क्षमता_greater(क्षमता_सम्बन्धि(env->dst_cpu), क्षमता))
				अनुवर्त;
		}

		/*
		 * Make sure we only pull tasks from a CPU of lower priority
		 * when balancing between SMT siblings.
		 *
		 * If balancing between cores, let lower priority CPUs help
		 * SMT cores with more than one busy sibling.
		 */
		यदि (sched_asym(env->sd, i, env->dst_cpu) && nr_running == 1)
			अनुवर्त;

		विकल्प (env->migration_type) {
		पक्ष migrate_आरोपय:
			/*
			 * When comparing with load imbalance, use cpu_load()
			 * which is not scaled with the CPU capacity.
			 */
			आरोपय = cpu_आरोपय(rq);

			यदि (nr_running == 1 && आरोपय > env->imbalance &&
			    !check_cpu_क्षमता(rq, env->sd))
				भङ्ग;

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
			यदि (आरोपय * busiest_क्षमता > busiest_आरोपय * क्षमता) {
				busiest_आरोपय = आरोपय;
				busiest_क्षमता = क्षमता;
				busiest = rq;
			}
			भङ्ग;

		पक्ष migrate_util:
			util = cpu_util_cfs_boost(i);

			/*
			 * Don't try to pull utilization from a CPU with one
			 * running task. Whatever its utilization, we will fail
			 * detach the task.
			 */
			यदि (nr_running <= 1)
				अनुवर्त;

			यदि (busiest_util < util) {
				busiest_util = util;
				busiest = rq;
			}
			भङ्ग;

		पक्ष migrate_task:
			यदि (busiest_nr < nr_running) {
				busiest_nr = nr_running;
				busiest = rq;
			}
			भङ्ग;

		पक्ष migrate_misfit:
			/*
			 * For ASYM_CPUCAPACITY domains with misfit tasks we
			 * simply seek the "biggest" misfit task.
			 */
			यदि (rq->misfit_task_आरोपय > busiest_आरोपय) {
				busiest_आरोपय = rq->misfit_task_आरोपय;
				busiest = rq;
			}

			भङ्ग;

		पक्ष migrate_llc_task:
#यदि_परिभाषितम् CONFIG_SCHED_CACHE
			sd_tmp = rcu_dereference_all(rq->sd);
			dst_llc = llc_id(env->dst_cpu);

			यदि (sd_tmp && (अचिह्नित)dst_llc < sd_tmp->llc_max) {
				अचिह्नित पूर्णाङ्क this_pref_llc =
					sd_tmp->llc_counts[dst_llc];

				यदि (busiest_pref_llc < this_pref_llc) {
					busiest_pref_llc = this_pref_llc;
					busiest = rq;
				}
			}
#यदि_अन्तः
			भङ्ग;

		}
	}

	प्रत्यागम busiest;
}

/*
 * Max backoff if we encounter pinned tasks. Pretty arbitrary value, but
 * so long as it is large enough.
 */
#परिभाषय MAX_PINNED_INTERVAL	512

स्थिर अन्तःस्थ bool
asym_सक्रिय_balance(संरचना lb_env *env)
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
	प्रत्यागम env->idle && sched_use_asym_prio(env->sd, env->dst_cpu) &&
	       (sched_asym_prefer(env->dst_cpu, env->src_cpu) ||
		!sched_use_asym_prio(env->sd, env->src_cpu));
}

स्थिर अन्तःस्थ bool
imbalanced_सक्रिय_balance(संरचना lb_env *env)
{
	संरचना sched_domain *sd = env->sd;

	/*
	 * The imbalanced case includes the case of pinned tasks preventing a fair
	 * distribution of the load on the system but also the even distribution of the
	 * threads on a system with spare capacity
	 */
	यदि ((env->migration_type == migrate_task) &&
	    (sd->nr_balance_failed > sd->cache_nice_tries+2))
		प्रत्यागम 1;

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क need_सक्रिय_balance(संरचना lb_env *env)
{
	संरचना sched_domain *sd = env->sd;

	यदि (alb_break_llc(env))
		प्रत्यागम 0;

	यदि (asym_सक्रिय_balance(env))
		प्रत्यागम 1;

	यदि (imbalanced_सक्रिय_balance(env))
		प्रत्यागम 1;

	/*
	 * The dst_cpu is idle and the src_cpu CPU has only 1 CFS task.
	 * It's worth migrating the task if the src_cpu's capacity is reduced
	 * because of other sched_class or IRQs if more capacity stays
	 * available on dst_cpu.
	 */
	यदि (env->idle &&
	    (env->src_rq->cfs.h_nr_runnable == 1)) {
		यदि ((check_cpu_क्षमता(env->src_rq, sd)) &&
		    (क्षमता_सम्बन्धि(env->src_cpu)*sd->imbalance_pct < क्षमता_सम्बन्धि(env->dst_cpu)*100))
			प्रत्यागम 1;
	}

	यदि (env->migration_type == migrate_misfit ||
	    env->migration_type == migrate_llc_task)
		प्रत्यागम 1;

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क सक्रिय_आरोपय_balance_cpu_stop(शून्य *दत्त);
स्थिर पूर्णाङ्क सक्रिय_आरोपय_balance_llc_cpu_stop(शून्य *दत्त);

/*
 * migration_type is checked elsewhere to decide migration policy, so
 * it shouldn't be repurposed just to flag an LLC-directed active
 * balance across the stopper. Pick the callback here instead.
 */
स्थिर अन्तःस्थ cpu_stop_fn_t alb_stop_fn(संरचना lb_env *env)
{
	यदि (env->migration_type == migrate_llc_task)
		प्रत्यागम सक्रिय_आरोपय_balance_llc_cpu_stop;

	प्रत्यागम सक्रिय_आरोपय_balance_cpu_stop;
}

स्थिर पूर्णाङ्क should_we_balance(संरचना lb_env *env)
{
	संरचना cpumask *swb_cpus = this_cpu_cpumask_var_ptr(should_we_balance_tmpmask);
	संरचना sched_गणः *sg = env->sd->groups;
	पूर्णाङ्क cpu, idle_smt = -1;

	/*
	 * Ensure the balancing environment is consistent; can happen
	 * when the softirq triggers 'during' hotplug.
	 */
	यदि (!cpumask_test_cpu(env->dst_cpu, env->cpus))
		प्रत्यागम 0;

	/*
	 * In the newly idle case, we will allow all the CPUs
	 * to do the newly idle load balance.
	 *
	 * However, we bail out if we already have tasks or a wakeup pending,
	 * to optimize wakeup latency.
	 */
	यदि (env->idle == CPU_NEWLY_IDLE) {
		यदि (env->dst_rq->nr_running > 0 || env->dst_rq->ttwu_pending)
			प्रत्यागम 0;
		प्रत्यागम 1;
	}

	cpumask_copy(swb_cpus, गणः_balance_आवरणम्(sg));
	/* Try to find first idle CPU */
	for_each_cpu_and(cpu, swb_cpus, env->cpus) {
		यदि (!idle_cpu(cpu))
			अनुवर्त;

		/*
		 * Don't balance to idle SMT in busy core right away when
		 * balancing cores, but remember the first idle SMT CPU for
		 * later consideration.  Find CPU on an idle core first.
		 */
		यदि (sched_smt_सक्रिय() &&
		    !(env->sd->flags & SD_SHARE_CPUCAPACITY) &&
		    !is_core_idle(cpu)) {
			यदि (idle_smt == -1)
				idle_smt = cpu;
			/*
			 * If the core is not idle, and first SMT sibling which is
			 * idle has been found, then its not needed to check other
			 * SMT siblings for idleness:
			 */
			cpumask_andnot(swb_cpus, swb_cpus, cpu_smt_आवरणम्(cpu));
			अनुवर्त;
		}

		/*
		 * Are we the first idle core in a non-SMT domain or higher,
		 * or the first idle CPU in a SMT domain?
		 */
		प्रत्यागम cpu == env->dst_cpu;
	}

	/* Are we the first idle CPU with busy siblings? */
	यदि (idle_smt != -1)
		प्रत्यागम idle_smt == env->dst_cpu;

	/* Are we the first CPU of this group ? */
	प्रत्यागम गणः_balance_cpu(sg) == env->dst_cpu;
}

स्थिर शून्य update_lb_imbalance_stat(संरचना lb_env *env, संरचना sched_domain *sd,
				     गणना cpu_idle_type idle)
{
	यदि (!schedstat_enabled())
		प्रत्यागम;

	विकल्प (env->migration_type) {
	पक्ष migrate_आरोपय:
		__schedstat_add(sd->lb_imbalance_आरोपय[idle], env->imbalance);
		भङ्ग;
	पक्ष migrate_util:
		__schedstat_add(sd->lb_imbalance_util[idle], env->imbalance);
		भङ्ग;
	पक्ष migrate_task:
		__schedstat_add(sd->lb_imbalance_task[idle], env->imbalance);
		भङ्ग;
	पक्ष migrate_misfit:
		__schedstat_add(sd->lb_imbalance_misfit[idle], env->imbalance);
		भङ्ग;
	पक्ष migrate_llc_task:
		भङ्ग;
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
स्थिर atomic_t sched_balance_running = ATOMIC_INIT(0);

/*
 * Check this_cpu to ensure it is balanced within domain. Attempt to move
 * tasks if there is an imbalance.
 */
स्थिर पूर्णाङ्क sched_balance_rq(पूर्णाङ्क this_cpu, संरचना rq *this_rq,
			संरचना sched_domain *sd, गणना cpu_idle_type idle,
			पूर्णाङ्क *continue_balancing)
{
	पूर्णाङ्क ld_moved, cur_ld_moved, सक्रिय_balance = 0;
	संरचना sched_domain *sd_parent = sd->parent;
	संरचना sched_गणः *गणः;
	संरचना rq *busiest;
	संरचना rq_flags rf;
	संरचना cpumask *cpus = this_cpu_cpumask_var_ptr(आरोपय_balance_आवरणम्);
	संरचना lb_env env = {
		.sd		= sd,
		.dst_cpu	= this_cpu,
		.dst_rq		= this_rq,
		.dst_grpmask    = गणः_balance_आवरणम्(sd->groups),
		.idle		= idle,
		.loop_break	= SCHED_NR_MIGRATE_BREAK,
		.cpus		= cpus,
		.fbq_type	= all,
		.tasks		= LIST_शीर्ष_INIT(env.tasks),
	};
	bool need_unlock = false;

	cpumask_and(cpus, sched_domain_विस्तृतिः(sd), cpu_preferred_आवरणम्);

	schedstat_inc(sd->lb_गणनम्[idle]);

redo:
	यदि (!should_we_balance(&env)) {
		*continue_balancing = 0;
		गच्छ बहिर्गमनम्_balanced;
	}

	यदि (!need_unlock && (sd->flags & SD_SERIALIZE)) {
		पूर्णाङ्क zero = 0;
		यदि (!atomic_try_cmpxchg_acquire(&sched_balance_running, &zero, 1))
			गच्छ बहिर्गमनम्_balanced;

		need_unlock = true;
	}

	गणः = sched_balance_find_src_गणः(&env);
	यदि (!गणः) {
		schedstat_inc(sd->lb_nobusyg[idle]);
		गच्छ बहिर्गमनम्_balanced;
	}

	busiest = sched_balance_find_src_rq(&env, गणः);
	यदि (!busiest) {
		schedstat_inc(sd->lb_nobusyq[idle]);
		गच्छ बहिर्गमनम्_balanced;
	}

	WARN_ON_ONCE(busiest == env.dst_rq);

	update_lb_imbalance_stat(&env, sd, idle);

	env.src_cpu = busiest->cpu;
	env.src_rq = busiest;

	ld_moved = 0;
	/* Clear this flag as soon as we find a pullable task */
	env.flags |= LBF_ALL_PINNED;
	यदि (busiest->nr_running > 1) {
		/*
		 * Attempt to move tasks. If sched_balance_find_src_group has found
		 * an imbalance but busiest->nr_running <= 1, the group is
		 * still unbalanced. ld_moved simply stays zero, so it is
		 * correctly treated as an imbalance.
		 */
		env.loop_max  = min(sysctl_sched_nr_migrate, busiest->nr_running);

अधिकम्_balance:
		rq_तालकम्_irqsave(busiest, &rf);
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

		यदि (cur_ld_moved) {
			attach_tasks(&env);
			ld_moved += cur_ld_moved;
		}

		local_irq_restore(rf.flags);

		यदि (env.flags & LBF_NEED_BREAK) {
			env.flags &= ~LBF_NEED_BREAK;
			गच्छ अधिकम्_balance;
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
		यदि ((env.flags & LBF_DST_PINNED) && env.imbalance > 0) {

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
			गच्छ अधिकम्_balance;
		}

		/*
		 * We failed to reach balance because of affinity.
		 */
		यदि (sd_parent) {
			पूर्णाङ्क *गणः_imbalance = &sd_parent->groups->sgc->imbalance;

			यदि ((env.flags & LBF_SOME_PINNED) && env.imbalance > 0)
				*गणः_imbalance = 1;
		}

		/* All tasks on this runqueue were pinned by CPU affinity */
		यदि (unlikely(env.flags & LBF_ALL_PINNED)) {
			__cpumask_clear_cpu(cpu_सम्बन्धि(busiest), cpus);
			/*
			 * Attempting to continue load balancing at the current
			 * sched_domain level only makes sense if there are
			 * active CPUs remaining as possible busiest CPUs to
			 * pull load from which are not contained within the
			 * destination group that is receiving any migrated
			 * load.
			 */
			यदि (!cpumask_subset(cpus, env.dst_grpmask)) {
				env.loop = 0;
				env.loop_break = SCHED_NR_MIGRATE_BREAK;
				गच्छ redo;
			}
			गच्छ बहिर्गमनम्_all_pinned;
		}
	}

	यदि (ld_moved) {
		sd->nr_balance_failed = 0;
		गच्छ बहिर्गमनम्_unbalanced;
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
	यदि (idle != CPU_NEWLY_IDLE &&
	    env.migration_type != migrate_misfit &&
	    !(env.flags & LBF_LLC_PINNED))
		sd->nr_balance_failed++;

	यदि (!need_सक्रिय_balance(&env))
		गच्छ बहिर्गमनम्_unbalanced;

	scoped_guard (raw_spin_rq_तालकम्_irqsave, busiest) {
		/*
		 * Don't kick the active_load_balance_cpu_stop,
		 * if the curr task on busiest CPU can't be
		 * moved to this_cpu:
		 */
		यदि (!cpumask_test_cpu(this_cpu, busiest->curr->cpus_ptr))
			गच्छ बहिर्गमनम्_one_pinned;

		/* Record that we found at least one task that could run on this_cpu */
		env.flags &= ~LBF_ALL_PINNED;

		/*
		 * ->active_balance synchronizes accesses to
		 * ->active_balance_work.  Once set, it's cleared
		 * only after active load balance is finished.
		 */
		यदि (busiest->सक्रिय_balance)
			गच्छ बहिर्गमनम्_unbalanced;

		/*
		 * @busiest dropped its rq_lock in the middle of
		 * scheduling out its ->curr task (->on_rq := 0), no
		 * need to forcefully punt it away with active balance.
		 */
		यदि (!busiest->curr->on_rq)
			गच्छ बहिर्गमनम्_unbalanced;

		busiest->सक्रिय_balance = 1;
		busiest->प्रक्षिप_cpu = this_cpu;
		सक्रिय_balance = 1;
		preempt_disable();
	}
	यदि (सक्रिय_balance) {
		stop_one_cpu_nowait(cpu_सम्बन्धि(busiest),
				    alb_stop_fn(&env), busiest,
				    &busiest->सक्रिय_balance_work);
	}
	preempt_enable();

बहिर्गमनम्_unbalanced:
	/* We were unbalanced, so reset the balancing interval */
	sd->balance_interval = sd->min_interval;
	गच्छ बहिर्गमनम्;

बहिर्गमनम्_balanced:
	/*
	 * We reach balance although we may have faced some affinity
	 * constraints. Clear the imbalance flag only if other tasks got
	 * a chance to move and fix the imbalance.
	 */
	यदि (sd_parent && !(env.flags & LBF_ALL_PINNED)) {
		पूर्णाङ्क *गणः_imbalance = &sd_parent->groups->sgc->imbalance;

		यदि (*गणः_imbalance)
			*गणः_imbalance = 0;
	}

बहिर्गमनम्_all_pinned:
	/*
	 * We reach balance because all tasks are pinned at this level so
	 * we can't migrate them. Let the imbalance flag set so parent level
	 * can try to migrate them.
	 */
	schedstat_inc(sd->lb_balanced[idle]);

	sd->nr_balance_failed = 0;

बहिर्गमनम्_one_pinned:
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
	यदि (env.idle == CPU_NEWLY_IDLE ||
	    env.migration_type == migrate_misfit)
		गच्छ बहिर्गमनम्;

	/* tune up the balancing interval */
	यदि ((env.flags & LBF_ALL_PINNED &&
	     sd->balance_interval < MAX_PINNED_INTERVAL) ||
	    sd->balance_interval < sd->max_interval)
		sd->balance_interval *= 2;
बहिर्गमनम्:
	यदि (need_unlock)
		atomic_समुच्चयः_release(&sched_balance_running, 0);

	प्रत्यागम ld_moved;
}

स्थिर अन्तःस्थ अचिह्नित दीर्घ
get_sd_balance_interval(संरचना sched_domain *sd, पूर्णाङ्क cpu_busy)
{
	अचिह्नित दीर्घ interval = sd->balance_interval;

	यदि (cpu_busy)
		interval *= sd->busy_गुणकः;

	/* scale ms to jiffies */
	interval = msecs_गन्तव्यम्_jiffies(interval);

	/*
	 * Reduce likelihood of busy balancing at higher domains racing with
	 * balancing at lower domains by preventing their balancing periods
	 * from being multiples of each other.
	 */
	यदि (cpu_busy)
		interval -= 1;

	interval = clamp(interval, 1UL, max_आरोपय_balance_interval);

	प्रत्यागम interval;
}

स्थिर अन्तःस्थ शून्य
update_अग्र_balance(संरचना sched_domain *sd, अचिह्नित दीर्घ *अग्र_balance)
{
	अचिह्नित दीर्घ interval, अग्र;

	/* used by idle balance, so cpu_busy = 0 */
	interval = get_sd_balance_interval(sd, 0);
	अग्र = sd->चरमः_balance + interval;

	यदि (time_after(*अग्र_balance, अग्र))
		*अग्र_balance = अग्र;
}

/*
 * active_load_balance_cpu_stop is run by the CPU stopper. It pushes
 * running tasks off the busiest CPU onto idle CPUs. It requires at
 * least 1 task to be running on each physical CPU where possible, and
 * avoids physical / logical imbalances.
 */
स्थिर पूर्णाङ्क __active_load_balance_cpu_stop(शून्य *दत्त, अचिह्नित पूर्णाङ्क lb_flags)
{
	संरचना rq *busiest_rq = दत्त;
	पूर्णाङ्क busiest_cpu = cpu_सम्बन्धि(busiest_rq);
	पूर्णाङ्क target_cpu = busiest_rq->प्रक्षिप_cpu;
	संरचना rq *target_rq = cpu_rq(target_cpu);
	संरचना sched_domain *sd;
	संरचना task_struct *p = रिक्त;
	संरचना rq_flags rf;

	rq_तालकम्_irq(busiest_rq, &rf);
	/*
	 * Between queueing the stop-work and running it is a hole in which
	 * CPUs can become inactive. We should not move tasks from or to
	 * inactive CPUs.
	 */
	यदि (!cpu_सक्रिय(busiest_cpu) || !cpu_सक्रिय(target_cpu))
		गच्छ बहिर्गमनम्_unlock;

	/* Make sure the requested CPU hasn't gone down in the meantime: */
	यदि (unlikely(busiest_cpu != smp_processor_id() ||
		     !busiest_rq->सक्रिय_balance))
		गच्छ बहिर्गमनम्_unlock;

	/* Is there any task to move? */
	यदि (busiest_rq->nr_running <= 1)
		गच्छ बहिर्गमनम्_unlock;

	/*
	 * This condition is "impossible", if it occurs
	 * we need to fix it. Originally reported by
	 * Bjorn Helgaas on a 128-CPU setup.
	 */
	WARN_ON_ONCE(busiest_rq == target_rq);

	/* Search for an sd spanning us and the target CPU. */
	rcu_read_तालकम्();
	for_each_domain(target_cpu, sd) {
		यदि (cpumask_test_cpu(busiest_cpu, sched_domain_विस्तृतिः(sd)))
			भङ्ग;
	}

	यदि (likely(sd)) {
		संरचना lb_env env = {
			.sd		= sd,
			.dst_cpu	= target_cpu,
			.dst_rq		= target_rq,
			.src_cpu	= busiest_rq->cpu,
			.src_rq		= busiest_rq,
			.idle		= CPU_IDLE,
			.flags		= LBF_सक्रिय_LB | lb_flags,
		};

		schedstat_inc(sd->alb_गणनम्);
		update_rq_clock(busiest_rq);

		p = detach_one_task(&env);
		यदि (p) {
			schedstat_inc(sd->alb_pushed);
			/* Active balancing done, reset the failure counter. */
			sd->nr_balance_failed = 0;
		} अन्यथा {
			schedstat_inc(sd->alb_failed);
		}
	}
	rcu_read_unlock();
बहिर्गमनम्_unlock:
	busiest_rq->सक्रिय_balance = 0;
	rq_unlock(busiest_rq, &rf);

	यदि (p)
		attach_one_task(target_rq, p);

	local_irq_enable();

	प्रत्यागम 0;
}

स्थिर पूर्णाङ्क सक्रिय_आरोपय_balance_cpu_stop(शून्य *दत्त)
{
	प्रत्यागम __active_load_balance_cpu_stop(दत्त, 0);
}

स्थिर पूर्णाङ्क सक्रिय_आरोपय_balance_llc_cpu_stop(शून्य *दत्त)
{
	प्रत्यागम __active_load_balance_cpu_stop(दत्त, LBF_सक्रिय_LB_LLC);
}

/*
 * Scale the max sched_balance_rq interval with the number of CPUs in the system.
 * This trades load-balance latency on larger machines for less cross talk.
 */
शून्य update_max_interval(शून्य)
{
	max_आरोपय_balance_interval = HZ*अङ्कः_online_cpus()/10;
}

स्थिर अन्तःस्थ शून्य update_newidle_stats(संरचना sched_domain *sd, अचिह्नित पूर्णाङ्क success)
{
	sd->newidle_call++;
	sd->newidle_success += success;

	यदि (sd->newidle_call >= 1024) {
		u64 now = sched_clock();
		s64 delta = now - sd->newidle_stamp;
		sd->newidle_stamp = now;
		पूर्णाङ्क अनुपातः = 0;

		यदि (delta < 0)
			delta = 0;

		यदि (sched_feat(NI_दरः)) {
			/*
			 * ratio  delta   freq
			 *
			 * 1024 -  4  s -  128 Hz
			 *  512 -  2  s -  256 Hz
			 *  256 -  1  s -  512 Hz
			 *  128 - .5  s - 1024 Hz
			 *   64 - .25 s - 2048 Hz
			 */
			अनुपातः = delta >> 22;
		}

		अनुपातः += sd->newidle_success;

		sd->newidle_अनुपातः = min(1024, अनुपातः);
		sd->newidle_call /= 2;
		sd->newidle_success /= 2;
	}
}

स्थिर अन्तःस्थ bool
update_newidle_cost(संरचना sched_domain *sd, u64 cost, अचिह्नित पूर्णाङ्क success)
{
	अचिह्नित दीर्घ अग्र_decay = sd->चरमः_decay_max_lb_cost + HZ;
	अचिह्नित दीर्घ now = jiffies;

	यदि (cost)
		update_newidle_stats(sd, success);

	यदि (cost > sd->max_newidle_lb_cost) {
		/*
		 * Track max cost of a domain to make sure to not delay the
		 * next wakeup on the CPU.
		 */
		sd->max_newidle_lb_cost = cost;
		sd->चरमः_decay_max_lb_cost = now;

	} अन्यथा यदि (time_after(now, अग्र_decay)) {
		/*
		 * Decay the newidle max times by ~1% per second to ensure that
		 * it is not outdated and the current max cost is actually
		 * shorter.
		 */
		sd->max_newidle_lb_cost = (sd->max_newidle_lb_cost * 253) / 256;
		sd->चरमः_decay_max_lb_cost = now;
		प्रत्यागम true;
	}

	प्रत्यागम false;
}

/*
 * It checks each scheduling domain to see if it is due to be balanced,
 * and initiates a balancing operation if so.
 *
 * Balancing parameters are set up in init_sched_domains.
 */
स्थिर शून्य sched_balance_domains(संरचना rq *rq, गणना cpu_idle_type idle)
{
	पूर्णाङ्क continue_balancing = 1;
	पूर्णाङ्क cpu = rq->cpu;
	पूर्णाङ्क busy = idle != CPU_IDLE && !sched_idle_rq(rq);
	अचिह्नित दीर्घ interval;
	संरचना sched_domain *sd;
	/* Earliest time when we have to do rebalance again */
	अचिह्नित दीर्घ अग्र_balance = jiffies + 60*HZ;
	पूर्णाङ्क update_अग्र_balance = 0;
	पूर्णाङ्क need_decay = 0;
	u64 max_cost = 0;

	rcu_read_तालकम्();
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
		यदि (!continue_balancing) {
			यदि (need_decay)
				अनुवर्त;
			भङ्ग;
		}

		interval = get_sd_balance_interval(sd, busy);
		यदि (time_after_eq(jiffies, sd->चरमः_balance + interval)) {
			यदि (sched_balance_rq(cpu, rq, sd, idle, &continue_balancing)) {
				/*
				 * The LBF_DST_PINNED logic could have changed
				 * env->dst_cpu, so we can't know our idle
				 * state even if we migrated tasks. Update it.
				 */
				idle = idle_cpu(cpu);
				busy = !idle && !sched_idle_rq(rq);
			}
			sd->चरमः_balance = jiffies;
			interval = get_sd_balance_interval(sd, busy);
		}
		यदि (time_after(अग्र_balance, sd->चरमः_balance + interval)) {
			अग्र_balance = sd->चरमः_balance + interval;
			update_अग्र_balance = 1;
		}
	}
	यदि (need_decay) {
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
	यदि (likely(update_अग्र_balance))
		rq->अग्र_balance = अग्र_balance;

}

स्थिर अन्तःस्थ पूर्णाङ्क on_null_domain(संरचना rq *rq)
{
	प्रत्यागम unlikely(!rcu_dereference_sched(rq->sd));
}

#यदि_परिभाषितम् CONFIG_NO_HZ_COMMON
/*
 * NOHZ idle load balancing (ILB) details:
 *
 * - When one of the busy CPUs notices that there may be an idle rebalancing
 *   needed, they will kick the idle load balancer, which then does idle
 *   load balancing for all the idle CPUs.
 */
स्थिर अन्तःस्थ पूर्णाङ्क find_new_ilb(शून्य)
{
	संरचना cpumask *ilb_cpus;
	पूर्णाङ्क ilb_cpu, fallback = -1;

	lockdep_assert_irqs_disabled();

	/*
	 * Reuse the per-CPU select_rq_mask, which is protected from concurrent
	 * use on this CPU by having interrupts disabled.
	 */
	ilb_cpus = this_cpu_cpumask_var_ptr(select_rq_आवरणम्);
	cpumask_and(ilb_cpus, nohz.idle_cpus_आवरणम्,
		    housekeeping_cpumask(HK_TYPE_KERNEL_NOISE));

	for_each_cpu(ilb_cpu, ilb_cpus) {
		यदि (!idle_cpu(ilb_cpu)) {
			/*
			 * Once an idle fallback exists, a busy CPU proves that
			 * this core cannot be fully idle. Skip its siblings.
			 */
			यदि (sched_smt_सक्रिय() && fallback >= 0)
				cpumask_andnot(ilb_cpus, ilb_cpus, cpu_smt_आवरणम्(ilb_cpu));
			अनुवर्त;
		}

		/*
		 * Running the idle load balancer on an idle sibling of a busy
		 * SMT core can reduce the capacity available to its sibling. Prefer
		 * a CPU whose entire core is idle, but retain the first idle CPU as
		 * a fallback so idle balancing can still make progress when no fully
		 * idle core exists.
		 */
		यदि (sched_smt_सक्रिय() && !is_core_idle(ilb_cpu)) {
			यदि (fallback < 0)
				fallback = ilb_cpu;

			/*
			 * The core is not idle, so there is no need to check
			 * any of its other SMT siblings.
			 */
			cpumask_andnot(ilb_cpus, ilb_cpus,
				       cpu_smt_आवरणम्(ilb_cpu));
			अनुवर्त;
		}

		प्रत्यागम ilb_cpu;
	}

	प्रत्यागम fallback;
}

/*
 * Kick a CPU to do the NOHZ balancing, if it is time for it, via a cross-CPU
 * SMP function call (IPI).
 *
 * Prefer a CPU on a fully idle core in the HK_TYPE_KERNEL_NOISE housekeeping
 * set. Fall back to the first idle CPU when no fully idle core exists.
 */
स्थिर शून्य kick_ilb(अचिह्नित पूर्णाङ्क flags)
{
	पूर्णाङ्क ilb_cpu;

	/*
	 * Increase nohz.next_balance only when if full ilb is triggered but
	 * not if we only update stats.
	 */
	यदि (flags & NOHZ_BALANCE_KICK)
		nohz.अग्र_balance = jiffies+1;

	ilb_cpu = find_new_ilb();
	यदि (ilb_cpu < 0)
		प्रत्यागम;

	/*
	 * Don't bother if no new NOHZ balance work items for ilb_cpu,
	 * i.e. all bits in flags are already set in ilb_cpu.
	 */
	यदि ((atomic_read(nohz_flags(ilb_cpu)) & flags) == flags)
		प्रत्यागम;

	/*
	 * Access to rq::nohz_csd is serialized by NOHZ_KICK_MASK; he who sets
	 * the first flag owns it; cleared by nohz_csd_func().
	 */
	flags = atomic_fetch_or(flags, nohz_flags(ilb_cpu));
	यदि (flags & NOHZ_KICK_आवरणम्)
		प्रत्यागम;

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
स्थिर शून्य nohz_balancer_kick(संरचना rq *rq)
{
	अचिह्नित दीर्घ now = jiffies;
	संरचना sched_domain_shared *sds;
	संरचना sched_domain *sd;
	पूर्णाङ्क nr_busy, i, cpu = rq->cpu;
	अचिह्नित पूर्णाङ्क flags = 0;

	यदि (unlikely(rq->idle_balance))
		प्रत्यागम;

	/*
	 * We may be recently in ticked or tickless idle mode. At the first
	 * busy tick after returning from idle, we will update the busy stats.
	 */
	nohz_balance_exit_idle(rq);

	यदि (READ_ONCE(nohz.has_blocked_आरोपय) &&
	    time_after(now, READ_ONCE(nohz.अग्र_blocked)))
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
	यदि (time_before(now, nohz.अग्र_balance))
		गच्छ बहिर्गमनम्;

	/*
	 * None are in tickless mode and hence no need for NOHZ idle load
	 * balancing
	 */
	यदि (unlikely(cpumask_रिक्तानि(nohz.idle_cpus_आवरणम्)))
		प्रत्यागम;

	यदि (rq->nr_running >= 2) {
		flags = NOHZ_STATS_KICK | NOHZ_BALANCE_KICK;
		गच्छ बहिर्गमनम्;
	}

	sd = rcu_dereference_all(rq->sd);
	यदि (sd) {
		/*
		 * If there's a runnable CFS task and the current CPU has reduced
		 * capacity, kick the ILB to see if there's a better CPU to run on:
		 */
		यदि (rq->cfs.h_nr_runnable >= 1 && check_cpu_क्षमता(rq, sd)) {
			flags |= NOHZ_STATS_KICK | NOHZ_BALANCE_KICK;
			गच्छ बहिर्गमनम्;
		}
	}

	sd = rcu_dereference_all(प्रत्येकम्_cpu(sd_asym_packing, cpu));
	यदि (sd) {
		/*
		 * When ASYM_PACKING; see if there's a more preferred CPU
		 * currently idle; in which case, kick the ILB to move tasks
		 * around.
		 *
		 * When balancing between cores, all the SMT siblings of the
		 * preferred CPU must be idle.
		 */
		for_each_cpu_and(i, sched_domain_विस्तृतिः(sd), nohz.idle_cpus_आवरणम्) {
			यदि (sched_asym(sd, i, cpu)) {
				flags |= NOHZ_STATS_KICK | NOHZ_BALANCE_KICK;
				गच्छ बहिर्गमनम्;
			}
		}
	}

	sd = rcu_dereference_all(प्रत्येकम्_cpu(sd_asym_cpucapacity, cpu));
	यदि (sd) {
		/*
		 * When ASYM_CPUCAPACITY; see if there's a higher capacity CPU
		 * to run the misfit task on.
		 */
		यदि (check_misfit_स्थितिः(rq))
			flags |= NOHZ_STATS_KICK | NOHZ_BALANCE_KICK;

		/*
		 * For asymmetric systems, we do not want to nicely balance
		 * cache use, instead we want to embrace asymmetry and only
		 * ensure tasks have enough CPU capacity.
		 *
		 * Skip the LLC logic because it's not relevant in that case.
		 */
		गच्छ बहिर्गमनम्;
	}

	sds = rcu_dereference_all(प्रत्येकम्_cpu(sd_balance_shared, cpu));
	यदि (sds) {
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
		यदि (nr_busy > 1)
			flags |= NOHZ_STATS_KICK | NOHZ_BALANCE_KICK;
	}
बहिर्गमनम्:
	यदि (READ_ONCE(nohz.needs_update))
		flags |= NOHZ_अग्र_KICK;

	यदि (flags)
		kick_ilb(flags);
}

स्थिर शून्य समुच्चयः_cpu_sd_state_busy(पूर्णाङ्क cpu)
{
	संरचना sched_domain *sd;
	sd = rcu_dereference_all(प्रत्येकम्_cpu(sd_llc, cpu));

	/*
	 * sd->nohz_idle only pairs with nr_busy_cpus on sd->shared; if this
	 * domain has no shared object there is nothing to clear or account.
	 */
	यदि (!sd || !sd->shared || !sd->nohz_idle)
		प्रत्यागम;
	sd->nohz_idle = 0;

	atomic_inc(&sd->shared->nr_busy_cpus);
}

शून्य nohz_balance_exit_idle(संरचना rq *rq)
{
	WARN_ON_ONCE(rq != this_rq());

	यदि (likely(!rq->nohz_tick_stopped))
		प्रत्यागम;

	rq->nohz_tick_stopped = 0;
	cpumask_clear_cpu(rq->cpu, nohz.idle_cpus_आवरणम्);

	समुच्चयः_cpu_sd_state_busy(rq->cpu);
}

स्थिर शून्य समुच्चयः_cpu_sd_state_idle(पूर्णाङ्क cpu)
{
	संरचना sched_domain *sd;
	sd = rcu_dereference_all(प्रत्येकम्_cpu(sd_llc, cpu));

	/* See set_cpu_sd_state_busy(): nohz_idle is only used with sd->shared. */
	यदि (!sd || !sd->shared || sd->nohz_idle)
		प्रत्यागम;
	sd->nohz_idle = 1;

	atomic_dec(&sd->shared->nr_busy_cpus);
}

/*
 * This routine will record that the CPU is going idle with tick stopped.
 * This info will be used in performing idle load balancing in the future.
 */
शून्य nohz_balance_enter_idle(पूर्णाङ्क cpu)
{
	संरचना rq *rq = cpu_rq(cpu);

	WARN_ON_ONCE(cpu != smp_processor_id());

	/* If this CPU is going down, then nothing needs to be done: */
	यदि (!cpu_सक्रिय(cpu))
		प्रत्यागम;

	/*
	 * Can be set safely without rq->lock held
	 * If a clear happens, it will have evaluated last additions because
	 * rq->lock is held during the check and the clear
	 */
	rq->has_blocked_आरोपय = 1;

	/*
	 * The tick is still stopped but load could have been added in the
	 * meantime. We set the nohz.has_blocked_load flag to trig a check of the
	 * *_avg. The CPU is already part of nohz.idle_cpus_mask so the clear
	 * of nohz.has_blocked_load can only happen after checking the new load
	 */
	यदि (rq->nohz_tick_stopped)
		गच्छ बहिर्गमनम्;

	/* If we're a completely isolated CPU, we don't play: */
	यदि (on_null_domain(rq))
		प्रत्यागम;

	rq->nohz_tick_stopped = 1;

	cpumask_समुच्चयः_cpu(cpu, nohz.idle_cpus_आवरणम्);

	/*
	 * Ensures that if nohz_idle_balance() fails to observe our
	 * @idle_cpus_mask store, it must observe the @has_blocked_load
	 * and @needs_update stores.
	 */
	smp_mb__after_atomic();

	समुच्चयः_cpu_sd_state_idle(cpu);

	WRITE_ONCE(nohz.needs_update, 1);
बहिर्गमनम्:
	/*
	 * Each time a cpu enter idle, we assume that it has blocked load and
	 * enable the periodic update of the load of idle CPUs
	 */
	WRITE_ONCE(nohz.has_blocked_आरोपय, 1);
}

स्थिर bool update_nohz_stats(संरचना rq *rq)
{
	अचिह्नित पूर्णाङ्क cpu = rq->cpu;

	यदि (!rq->has_blocked_आरोपय)
		प्रत्यागम false;

	यदि (!cpumask_test_cpu(cpu, nohz.idle_cpus_आवरणम्))
		प्रत्यागम false;

	यदि (!time_after(jiffies, READ_ONCE(rq->चरमः_blocked_आरोपय_update_tick)))
		प्रत्यागम true;

	sched_balance_update_blocked_averages(cpu);

	प्रत्यागम rq->has_blocked_आरोपय;
}

/*
 * Internal function that runs load balance for all idle CPUs. The load balance
 * can be a simple update of blocked load or a complete load balance with
 * tasks movement depending of flags.
 */
स्थिर शून्य _nohz_idle_balance(संरचना rq *this_rq, अचिह्नित पूर्णाङ्क flags)
{
	/* Earliest time when we have to do rebalance again */
	अचिह्नित दीर्घ now = jiffies;
	अचिह्नित दीर्घ अग्र_balance = now + 60*HZ;
	bool has_blocked_आरोपय = false;
	पूर्णाङ्क update_अग्र_balance = 0;
	पूर्णाङ्क this_cpu = this_rq->cpu;
	पूर्णाङ्क balance_cpu;
	संरचना rq *rq;

	WARN_ON_ONCE((flags & NOHZ_KICK_आवरणम्) == NOHZ_BALANCE_KICK);

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
	यदि (flags & NOHZ_STATS_KICK)
		WRITE_ONCE(nohz.has_blocked_आरोपय, 0);
	यदि (flags & NOHZ_अग्र_KICK)
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
	for_each_cpu_wrap(balance_cpu,  nohz.idle_cpus_आवरणम्, this_cpu+1) {
		यदि (!idle_cpu(balance_cpu))
			अनुवर्त;

		/*
		 * If this CPU gets work to do, stop the load balancing
		 * work being done for other CPUs. Next load
		 * balancing owner will pick it up.
		 */
		यदि (!idle_cpu(this_cpu) && need_resched()) {
			यदि (flags & NOHZ_STATS_KICK)
				has_blocked_आरोपय = true;
			यदि (flags & NOHZ_अग्र_KICK)
				WRITE_ONCE(nohz.needs_update, 1);
			गच्छ abort;
		}

		rq = cpu_rq(balance_cpu);

		यदि (flags & NOHZ_STATS_KICK)
			has_blocked_आरोपय |= update_nohz_stats(rq);

		/*
		 * If time for next balance is due,
		 * do the balance.
		 */
		यदि (time_after_eq(jiffies, rq->अग्र_balance)) {
			संरचना rq_flags rf;

			rq_तालकम्_irqsave(rq, &rf);
			update_rq_clock(rq);
			rq_unlock_irqrestore(rq, &rf);

			यदि (flags & NOHZ_BALANCE_KICK)
				sched_balance_domains(rq, CPU_IDLE);
		}

		यदि (time_after(अग्र_balance, rq->अग्र_balance)) {
			अग्र_balance = rq->अग्र_balance;
			update_अग्र_balance = 1;
		}
	}

	/*
	 * next_balance will be updated only when there is a need.
	 * When the CPU is attached to null domain for ex, it will not be
	 * updated.
	 */
	यदि (likely(update_अग्र_balance))
		nohz.अग्र_balance = अग्र_balance;

	यदि (flags & NOHZ_STATS_KICK)
		WRITE_ONCE(nohz.अग्र_blocked,
			   now + msecs_गन्तव्यम्_jiffies(आरोपय_AVG_PERIOD));

abort:
	/* There is still blocked load, enable periodic update */
	यदि (has_blocked_आरोपय)
		WRITE_ONCE(nohz.has_blocked_आरोपय, 1);
}

/*
 * In CONFIG_NO_HZ_COMMON case, the idle balance kickee will do the
 * rebalancing for all the CPUs for whom scheduler ticks are stopped.
 */
स्थिर bool nohz_idle_balance(संरचना rq *this_rq, गणना cpu_idle_type idle)
{
	अचिह्नित पूर्णाङ्क flags = this_rq->nohz_idle_balance;

	यदि (!flags)
		प्रत्यागम false;

	this_rq->nohz_idle_balance = 0;

	यदि (idle != CPU_IDLE)
		प्रत्यागम false;

	_nohz_idle_balance(this_rq, flags);

	प्रत्यागम true;
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
शून्य nohz_सततम्_idle_balance(पूर्णाङ्क cpu)
{
	अचिह्नित पूर्णाङ्क flags;

	flags = atomic_fetch_andnot(NOHZ_NEWILB_KICK, nohz_flags(cpu));

	/*
	 * Update the blocked load only if no SCHED_SOFTIRQ is about to happen
	 * (i.e. NOHZ_STATS_KICK set) and will do the same.
	 */
	यदि ((flags == NOHZ_NEWILB_KICK) && !need_resched())
		_nohz_idle_balance(cpu_rq(cpu), NOHZ_STATS_KICK);
}

स्थिर शून्य nohz_newidle_balance(संरचना rq *this_rq)
{
	पूर्णाङ्क this_cpu = this_rq->cpu;

	/* Will wake up very soon. No time for doing anything else*/
	यदि (this_rq->avg_idle < sysctl_sched_migration_cost)
		प्रत्यागम;

	/* Don't need to update blocked load of idle CPUs*/
	यदि (!READ_ONCE(nohz.has_blocked_आरोपय) ||
	    time_before(jiffies, READ_ONCE(nohz.अग्र_blocked)))
		प्रत्यागम;

	/*
	 * Set the need to trigger ILB in order to update blocked load
	 * before entering idle state.
	 */
	atomic_or(NOHZ_NEWILB_KICK, nohz_flags(this_cpu));
}

#अन्यथा /* !CONFIG_NO_HZ_COMMON: */
स्थिर अन्तःस्थ शून्य nohz_balancer_kick(संरचना rq *rq) { }

स्थिर अन्तःस्थ bool nohz_idle_balance(संरचना rq *this_rq, गणना cpu_idle_type idle)
{
	प्रत्यागम false;
}

स्थिर अन्तःस्थ शून्य nohz_newidle_balance(संरचना rq *this_rq) { }
#यदि_अन्तः /* !CONFIG_NO_HZ_COMMON */

/*
 * sched_balance_newidle is called by schedule() if this_cpu is about to become
 * idle. Attempts to pull tasks from other CPUs.
 *
 * Returns:
 *   < 0 - we released the lock and there are !fair tasks present
 *     0 - failed, no new tasks
 *   > 0 - success, new (fair) tasks present
 */
स्थिर पूर्णाङ्क sched_balance_newidle(संरचना rq *this_rq, संरचना rq_flags *rf)
	__must_hold(__rq_lockp(this_rq))
{
	अचिह्नित दीर्घ अग्र_balance = jiffies + HZ;
	पूर्णाङ्क this_cpu = this_rq->cpu;
	पूर्णाङ्क continue_balancing = 1;
	u64 t0, t1, curr_cost = 0;
	संरचना sched_domain *sd;
	पूर्णाङ्क pulled_task = 0;

	update_misfit_स्थितिः(रिक्त, this_rq);

	/*
	 * There is a task waiting to run. No need to search for one.
	 * Return 0; the task will be enqueued when switching to idle.
	 */
	यदि (this_rq->ttwu_pending)
		प्रत्यागम 0;

	/*
	 * We must set idle_stamp _before_ calling sched_balance_rq()
	 * for CPU_NEWLY_IDLE, such that we measure the this duration
	 * as idle time.
	 */
	this_rq->idle_stamp = rq_clock(this_rq);

	/* Do not pull tasks towards !preferred CPUs */
	यदि (!cpu_preferred(this_cpu))
		प्रत्यागम 0;

	/*
	 * This is OK, because current is on_cpu, which avoids it being picked
	 * for load-balance and preemption/IRQs are still disabled avoiding
	 * further scheduler activity on it and we're being very careful to
	 * re-start the picking loop.
	 */
	rq_unpin_तालकम्(this_rq, rf);

	sd = rcu_dereference_sched_domain(this_rq->sd);
	यदि (!sd)
		गच्छ बहिर्गमनम्;

	यदि (!get_rd_overloaded(this_rq->rd) ||
	    this_rq->avg_idle < sd->max_newidle_lb_cost) {

		update_अग्र_balance(sd, &अग्र_balance);
		गच्छ बहिर्गमनम्;
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

		update_अग्र_balance(sd, &अग्र_balance);

		यदि (this_rq->avg_idle < curr_cost + sd->max_newidle_lb_cost)
			भङ्ग;

		यदि (sd->flags & SD_BALANCE_NEWIDLE) {
			अचिह्नित पूर्णाङ्क weight = 1;

			यदि (sched_feat(NI_RANDOM) && sd->newidle_अनुपातः < 1024) {
				/*
				 * Throw a 1k sided dice; and only run
				 * newidle_balance according to the success
				 * rate.
				 */
				u32 d1k = sched_rng() % 1024;
				weight = 1 + sd->newidle_अनुपातः;
				यदि (d1k > weight) {
					update_newidle_stats(sd, 0);
					अनुवर्त;
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
		यदि (pulled_task || !continue_balancing)
			भङ्ग;
	}

	raw_spin_rq_तालकम्(this_rq);

	यदि (curr_cost > this_rq->max_idle_balance_cost)
		this_rq->max_idle_balance_cost = curr_cost;

	/*
	 * While browsing the domains, we released the rq lock, a task could
	 * have been enqueued in the meantime. Since we're not going idle,
	 * pretend we pulled a task.
	 */
	यदि (this_rq->cfs.h_nr_queued && !pulled_task)
		pulled_task = 1;

	/* If a higher prio class was modified, restart the pick */
	यदि (rq_modified_above(this_rq, &fair_sched_class))
		pulled_task = -1;

बहिर्गमनम्:
	/* Move the next balance forward */
	यदि (time_after(this_rq->अग्र_balance, अग्र_balance))
		this_rq->अग्र_balance = अग्र_balance;

	यदि (pulled_task)
		this_rq->idle_stamp = 0;
	अन्यथा
		nohz_newidle_balance(this_rq);

	rq_repin_तालकम्(this_rq, rf);

	प्रत्यागम pulled_task;
}

/*
 * This softirq handler is triggered via SCHED_SOFTIRQ from two places:
 *
 * - directly from the local sched_tick() for periodic load balancing
 *
 * - indirectly from a remote sched_tick() for NOHZ idle balancing
 *   through the SMP cross-call nohz_csd_func()
 */
स्थिर __latent_entropy शून्य sched_balance_softirq(शून्य)
{
	संरचना rq *this_rq = this_rq();
	गणना cpu_idle_type idle = this_rq->idle_balance;
	/*
	 * If this CPU has a pending NOHZ_BALANCE_KICK, then do the
	 * balancing on behalf of the other idle CPUs whose ticks are
	 * stopped. Do nohz_idle_balance *before* sched_balance_domains to
	 * give the idle CPUs a chance to load balance. Else we may
	 * load balance only within the local sched_domain hierarchy
	 * and abort nohz_idle_balance altogether if we pull some load.
	 */
	यदि (nohz_idle_balance(this_rq, idle))
		प्रत्यागम;

	/* normal load balance */
	sched_balance_update_blocked_averages(this_rq->cpu);
	sched_balance_domains(this_rq, idle);
}

/*
 * Trigger the SCHED_SOFTIRQ if it is time to do periodic load balancing.
 */
शून्य sched_balance_trigger(संरचना rq *rq)
{
	/*
	 * Don't need to rebalance while attached to NULL domain or
	 * runqueue CPU is not active
	 */
	यदि (unlikely(on_null_domain(rq) || !cpu_सक्रिय(cpu_सम्बन्धि(rq))))
		प्रत्यागम;

	यदि (time_after_eq(jiffies, rq->अग्र_balance))
		raise_softirq(SCHED_SOFTIRQ);

	nohz_balancer_kick(rq);
}

स्थिर शून्य rq_online_fair(संरचना rq *rq)
{
	update_sysctl();

	update_runtime_enabled(rq);
}

स्थिर शून्य rq_offline_fair(संरचना rq *rq)
{
	update_sysctl();

	/* Ensure any throttled groups are reachable by pick_next_task */
	unthrottle_offline_cfs_rqs(rq);

	/* Ensure that we remove rq contribution to group share: */
	clear_tg_offline_cfs_rqs(rq);
}

#यदि_परिभाषितम् CONFIG_SCHED_CORE
स्थिर अन्तःस्थ bool
__entity_slice_used(संरचना sched_entity *se, पूर्णाङ्क min_nr_tasks)
{
	u64 rtime = se->योग_exec_runtime - se->पूर्व_योग_exec_runtime;
	u64 खण्डिका = se->खण्डिका;

	प्रत्यागम (rtime * min_nr_tasks > खण्डिका);
}

#परिभाषय MIN_NR_TASKS_DURING_FORCEIDLE	2
स्थिर अन्तःस्थ शून्य task_tick_core(संरचना rq *rq, संरचना task_struct *curr)
{
	यदि (!sched_core_enabled(rq))
		प्रत्यागम;

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
	यदि (rq->core->core_forceidle_गणनम् && rq->cfs.h_nr_queued == 1 &&
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
स्थिर शून्य se_fi_update(ध्रुव संरचना sched_entity *se, अचिह्नित पूर्णाङ्क fi_seq,
			 bool forceidle)
{
	for_each_sched_entity(se) {
		संरचना cfs_rq *cfs_rq = cfs_rq_सम्बन्धि(se);

		यदि (forceidle) {
			यदि (cfs_rq->forceidle_seq == fi_seq)
				भङ्ग;
			cfs_rq->forceidle_seq = fi_seq;
		}

		cfs_rq->zero_vruntime_fi = cfs_rq->zero_vruntime;
	}
}

शून्य task_vruntime_update(संरचना rq *rq, संरचना task_struct *p, bool अन्तर्_fi)
{
	संरचना sched_entity *se = &p->se;

	यदि (p->sched_class != &fair_sched_class)
		प्रत्यागम;

	se_fi_update(se, rq->core->core_forceidle_seq, अन्तर्_fi);
}

bool cfs_prio_less(ध्रुव संरचना task_struct *a, ध्रुव संरचना task_struct *b,
			bool अन्तर्_fi)
{
	संरचना rq *rq = task_rq(a);
	ध्रुव संरचना sched_entity *sea = &a->se;
	ध्रुव संरचना sched_entity *seb = &b->se;
	संरचना cfs_rq *cfs_rqa;
	संरचना cfs_rq *cfs_rqb;
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

	प्रत्यागम delta > 0;
}

स्थिर पूर्णाङ्क task_is_throttled_fair(संरचना task_struct *p, पूर्णाङ्क cpu)
{
	संरचना cfs_rq *cfs_rq;

#यदि_परिभाषितम् CONFIG_FAIR_गणः_SCHED
	cfs_rq = tg_cfs_rq(task_गणः(p), cpu);
#अन्यथा
	cfs_rq = &cpu_rq(cpu)->cfs;
#यदि_अन्तः
	प्रत्यागम throttled_hierarchy(cfs_rq);
}
#अन्यथा /* !CONFIG_SCHED_CORE: */
स्थिर अन्तःस्थ शून्य task_tick_core(संरचना rq *rq, संरचना task_struct *curr) {}
#यदि_अन्तः /* !CONFIG_SCHED_CORE */

/*
 * scheduler tick hitting a task of our scheduling class.
 *
 * NOTE: This function can be called remotely by the tick offload that
 * goes along full dynticks. Therefore no local assumption can be made
 * and everything must be accessed through the @rq and @curr passed in
 * parameters.
 */
स्थिर शून्य task_tick_fair(संरचना rq *rq, संरचना task_struct *curr, पूर्णाङ्क queued)
{
	संरचना sched_entity *se = &curr->se;

	यदि (se->on_rq) {
		अचिह्नित दीर्घ weight = NICE_0_आरोपय;
		संरचना cfs_rq *cfs_rq;

		for_each_sched_entity(se) {
			cfs_rq = cfs_rq_सम्बन्धि(se);
			entity_tick(cfs_rq, se, queued);

			weight = __calc_prop_weight(cfs_rq, se, weight);
		}

		se = &curr->se;
		reweight_eevdf(cfs_rq, se, weight, se->on_rq);
	}

	यदि (queued)
		प्रत्यागम;

	यदि (static_branch_unlikely(&sched_numa_balancing))
		task_tick_numa(rq, curr);

	task_tick_cache(rq, curr);

	update_misfit_स्थितिः(curr, rq);
	check_update_overutilized_स्थितिः(task_rq(curr));

	task_tick_core(rq, curr);
}

/*
 * called on fork with the child task as argument from the parent's context
 *  - child not yet on the tasklist
 *  - preemption disabled
 */
स्थिर शून्य task_fork_fair(संरचना task_struct *p)
{
	समुच्चयः_task_max_allowed_क्षमता(p);
}

/*
 * Priority of the task has changed. Check to see if we preempt
 * the current task.
 */
स्थिर शून्य
prio_परिवर्तितम्_fair(संरचना rq *rq, संरचना task_struct *p, u64 oldprio)
{
	यदि (!task_on_rq_queued(p))
		प्रत्यागम;

	यदि (p->prio == oldprio)
		प्रत्यागम;

	यदि (rq->cfs.h_nr_queued == 1)
		प्रत्यागम;

	/*
	 * Reschedule if we are currently running on this runqueue and
	 * our priority decreased, or if we are not currently running on
	 * this runqueue and our priority is higher than the current's
	 */
	यदि (task_current_donor(rq, p)) {
		यदि (p->prio > oldprio)
			resched_curr(rq);
	} अन्यथा {
		wakeup_preempt(rq, p, 0);
	}
}

#यदि_परिभाषितम् CONFIG_FAIR_गणः_SCHED
/*
 * Propagate the changes of the sched_entity across the tg tree to make it
 * visible to the root
 */
स्थिर शून्य propagate_entity_cfs_rq(संरचना sched_entity *se)
{
	संरचना cfs_rq *cfs_rq = cfs_rq_सम्बन्धि(se);

	/*
	 * If a task gets attached to this cfs_rq and before being queued,
	 * it gets migrated to another CPU due to reasons like affinity
	 * change, make sure this cfs_rq stays on leaf cfs_rq list to have
	 * that removed load decayed or it can cause faireness problem.
	 */
	यदि (!cfs_rq_pelt_clock_throttled(cfs_rq))
		list_सङ्कलय_पर्णम्_cfs_rq(cfs_rq);

	/* Start to propagate at parent */
	se = se->parent;

	for_each_sched_entity(se) {
		cfs_rq = cfs_rq_सम्बन्धि(se);

		update_आरोपय_avg(cfs_rq, se, UPDATE_TG);

		यदि (!cfs_rq_pelt_clock_throttled(cfs_rq))
			list_सङ्कलय_पर्णम्_cfs_rq(cfs_rq);
	}

	assert_list_पर्णम्_cfs_rq(rq_सम्बन्धि(cfs_rq));
}
#अन्यथा /* !CONFIG_FAIR_GROUP_SCHED: */
स्थिर शून्य propagate_entity_cfs_rq(संरचना sched_entity *se) { }
#यदि_अन्तः /* !CONFIG_FAIR_GROUP_SCHED */

स्थिर शून्य detach_entity_cfs_rq(संरचना sched_entity *se)
{
	संरचना cfs_rq *cfs_rq = cfs_rq_सम्बन्धि(se);

	/*
	 * In case the task sched_avg hasn't been attached:
	 * - A forked task which hasn't been woken up by wake_up_new_task().
	 * - A task which has been woken up by try_to_wake_up() but is
	 *   waiting for actually being woken up by sched_ttwu_pending().
	 */
	यदि (!se->avg.चरमः_update_time)
		प्रत्यागम;

	/* Catch up with the cfs_rq and remove our load when we leave */
	update_आरोपय_avg(cfs_rq, se, 0);
	detach_entity_आरोपय_avg(cfs_rq, se);
	update_tg_आरोपय_avg(cfs_rq);
	propagate_entity_cfs_rq(se);
}

स्थिर शून्य attach_entity_cfs_rq(संरचना sched_entity *se)
{
	संरचना cfs_rq *cfs_rq = cfs_rq_सम्बन्धि(se);

	/* Synchronize entity with its cfs_rq */
	update_आरोपय_avg(cfs_rq, se, sched_feat(ATTACH_AGE_आरोपय) ? 0 : SKIP_AGE_आरोपय);
	attach_entity_आरोपय_avg(cfs_rq, se);
	update_tg_आरोपय_avg(cfs_rq);
	propagate_entity_cfs_rq(se);
}

स्थिर शून्य detach_task_cfs_rq(संरचना task_struct *p)
{
	संरचना sched_entity *se = &p->se;

	detach_entity_cfs_rq(se);
}

स्थिर शून्य attach_task_cfs_rq(संरचना task_struct *p)
{
	संरचना sched_entity *se = &p->se;

	attach_entity_cfs_rq(se);
}

स्थिर शून्य switching_from_fair(संरचना rq *rq, संरचना task_struct *p)
{
	यदि (p->se.sched_delayed)
		पङ्क्तेः_निष्कासय_task(rq, p, पङ्क्तेः_निष्कासय_SLEEP | पङ्क्तेः_निष्कासय_DELAYED | पङ्क्तेः_निष्कासय_NOCLOCK);
}

स्थिर शून्य switched_from_fair(संरचना rq *rq, संरचना task_struct *p)
{
	detach_task_cfs_rq(p);
}

स्थिर शून्य switched_गन्तव्यम्_fair(संरचना rq *rq, संरचना task_struct *p)
{
	WARN_ON_ONCE(p->se.sched_delayed);

	attach_task_cfs_rq(p);

	समुच्चयः_task_max_allowed_क्षमता(p);

	यदि (task_on_rq_queued(p)) {
		/*
		 * We were most likely switched from sched_rt, so
		 * kick off the schedule if running, otherwise just see
		 * if we can still preempt the current task.
		 */
		यदि (task_current_donor(rq, p))
			resched_curr(rq);
		अन्यथा
			wakeup_preempt(rq, p, 0);
	}
}

स्थिर शून्य समुच्चयः_अग्र_task_fair(संरचना rq *rq, संरचना task_struct *p, गणना snt_e type)
{
	संरचना sched_entity *se = &p->se;
	संरचना cfs_rq *cfs_rq = &rq->cfs;
	अचिह्नित दीर्घ weight = NICE_0_आरोपय;
	bool first = type == SNT_PICK;
	bool throttled = false;
	bool on_rq = se->on_rq;

	यदि (type == SNT_REPICK)
		गच्छ repick;

	clear_buddies(cfs_rq, se);

	यदि (on_rq)
		__dequeue_entity(cfs_rq, se);

	for_each_sched_entity(se) {
		cfs_rq = cfs_rq_सम्बन्धि(se);

		यदि (!IS_ENABLED(CONFIG_FAIR_गणः_SCHED) ||
		    !first || !cfs_rq->h_curr)
			समुच्चयः_अग्र_entity(cfs_rq, se);

		/* ensure bandwidth has been allocated on our new cfs_rq */
		throttled |= account_cfs_rq_runtime(cfs_rq, 0);

		यदि (on_rq)
			weight = __calc_prop_weight(cfs_rq, se, weight);
	}

	यदि (throttled)
		task_throttle_सज्जता_work(p);

	se = &p->se;
	cfs_rq->curr = se;

	यदि (on_rq) {
		reweight_eevdf(cfs_rq, se, weight, se->on_rq);
		यदि (first)
			समुच्चयः_protect_खण्डिका(cfs_rq, se);
	}

	यदि (task_on_rq_queued(p)) {
		/*
		 * Move the next running task to the front of the list, so our
		 * cfs_tasks list becomes MRU one.
		 */
		list_move(&se->गणः_ग्रन्थिः, &rq->cfs_tasks);
	}
	यदि (!first)
		प्रत्यागम;

	WARN_ON_ONCE(se->sched_delayed);

	update_misfit_स्थितिः(p, rq);
	sched_fair_update_stop_tick(rq, p);

repick:
	/*
	 * A same-task repick skips put_prev_task_fair(), but
	 * pick_task_fair() refreshed the entity hrtick_start_fair() reads
	 * before selecting it again. rq->cfs.curr identifies that entity,
	 * including with group scheduling.
	 */
	यदि (hrtick_enabled_fair(rq))
		hrtick_start_fair(rq, p);
}

शून्य init_cfs_rq(संरचना cfs_rq *cfs_rq)
{
	cfs_rq->tasks_timeline = RB_मूलम्_CACHED;
	cfs_rq->zero_vruntime = (u64)(-(1LL << 20));
	raw_spin_तालकम्_init(&cfs_rq->removed.तालकम्);
}

#यदि_परिभाषितम् CONFIG_FAIR_गणः_SCHED
स्थिर शून्य task_change_गणः_fair(संरचना task_struct *p)
{
	/*
	 * We couldn't detach or attach a forked task which
	 * hasn't been woken up by wake_up_new_task().
	 */
	यदि (READ_ONCE(p->__state) == TASK_NEW)
		प्रत्यागम;

	detach_task_cfs_rq(p);

	/* Tell se's cfs_rq has been changed -- migrated */
	p->se.avg.चरमः_update_time = 0;
	समुच्चयः_task_rq(p, task_cpu(p));
	attach_task_cfs_rq(p);
}

शून्य free_fair_sched_गणः(संरचना task_गणः *tg)
{
	free_percpu(tg->cfs_rq);
}

पूर्णाङ्क alloc_fair_sched_गणः(संरचना task_गणः *tg, संरचना task_गणः *parent)
{
	संरचना cfs_tg_state __percpu *state;
	संरचना sched_entity *se;
	संरचना cfs_rq *cfs_rq;
	पूर्णाङ्क i;

	state = alloc_percpu_gfp(संरचना cfs_tg_state, GFP_KERNEL);
	यदि (!state)
		गच्छ err;

	tg->cfs_rq = &state->cfs_rq;
	tg->shares = NICE_0_आरोपय;

	init_cfs_bandwidth(tg_cfs_bandwidth(tg), tg_cfs_bandwidth(parent));

	for_each_possible_cpu(i) {
		cfs_rq = tg_cfs_rq(tg, i);
		यदि (!cfs_rq)
			गच्छ err;

		se = tg_se(tg, i);
		init_cfs_rq(cfs_rq);
		init_tg_cfs_प्रविष्टिः(tg, cfs_rq, se, i, tg_se(parent, i));
		init_entity_runnable_मध्यमानम्(se);
	}

	प्रत्यागम 1;

err:
	प्रत्यागम 0;
}

शून्य online_fair_sched_गणः(संरचना task_गणः *tg)
{
	संरचना sched_entity *se;
	संरचना rq_flags rf;
	संरचना rq *rq;
	पूर्णाङ्क i;

	for_each_possible_cpu(i) {
		rq = cpu_rq(i);
		se = tg_se(tg, i);
		rq_तालकम्_irq(rq, &rf);
		update_rq_clock(rq);
		attach_entity_cfs_rq(se);
		sync_throttle(tg, i);
		rq_unlock_irq(rq, &rf);
	}
}

शून्य unregister_fair_sched_गणः(संरचना task_गणः *tg)
{
	पूर्णाङ्क cpu;

	destroy_cfs_bandwidth(tg_cfs_bandwidth(tg));

	for_each_possible_cpu(cpu) {
		संरचना cfs_rq *cfs_rq = tg_cfs_rq(tg, cpu);
		संरचना sched_entity *se = tg_se(tg, cpu);
		संरचना rq *rq = cpu_rq(cpu);

		यदि (se)
			remove_entity_आरोपय_avg(se);

		/*
		 * Only empty task groups can be destroyed; so we can speculatively
		 * check on_list without danger of it being re-added.
		 */
		यदि (cfs_rq->on_list) {
			guard(rq_तालकम्_irqsave)(rq);
			list_del_पर्णम्_cfs_rq(cfs_rq);
		}
	}
}

शून्य init_tg_cfs_प्रविष्टिः(संरचना task_गणः *tg, संरचना cfs_rq *cfs_rq,
			संरचना sched_entity *se, पूर्णाङ्क cpu,
			संरचना sched_entity *parent)
{
	संरचना rq *rq = cpu_rq(cpu);

	cfs_rq->tg = tg;
	cfs_rq->rq = rq;
	init_cfs_rq_runtime(cfs_rq);

	/* se could be NULL for root_task_group */
	यदि (!se)
		प्रत्यागम;

	यदि (!parent) {
		se->cfs_rq = &rq->cfs;
		se->गभीरता = 0;
	} अन्यथा {
		se->cfs_rq = parent->my_q;
		se->गभीरता = parent->गभीरता + 1;
	}

	se->my_q = cfs_rq;
	/* guarantee group entities always have weight */
	update_आरोपय_समुच्चयः(&se->आरोपय, NICE_0_आरोपय);
	se->parent = parent;
}

स्थिर DEFINE_MUTEX(shares_mutex);

स्थिर पूर्णाङ्क __sched_group_set_shares(संरचना task_गणः *tg, अचिह्नित दीर्घ shares)
{
	पूर्णाङ्क i;

	lockdep_assert_held(&shares_mutex);

	/*
	 * We can't change the weight of the root cgroup.
	 */
	यदि (is_मूलम्_task_गणः(tg))
		प्रत्यागम -EINVAL;

	shares = clamp(shares, scale_आरोपय(MIN_SHARES), scale_आरोपय(MAX_SHARES));

	यदि (tg->shares == shares)
		प्रत्यागम 0;

	tg->shares = shares;
	for_each_possible_cpu(i) {
		संरचना rq *rq = cpu_rq(i);
		संरचना sched_entity *se = tg_se(tg, i);
		संरचना rq_flags rf;

		/* Propagate contribution to hierarchy */
		rq_तालकम्_irqsave(rq, &rf);
		update_rq_clock(rq);
		for_each_sched_entity(se) {
			update_आरोपय_avg(cfs_rq_सम्बन्धि(se), se, UPDATE_TG);
			update_cfs_गणः(se);
		}
		rq_unlock_irqrestore(rq, &rf);
	}

	प्रत्यागम 0;
}

पूर्णाङ्क sched_गणः_समुच्चयः_shares(संरचना task_गणः *tg, अचिह्नित दीर्घ shares)
{
	पूर्णाङ्क ret;

	mutex_तालकम्(&shares_mutex);
	यदि (tg_is_idle(tg))
		ret = -EINVAL;
	अन्यथा
		ret = __sched_group_set_shares(tg, shares);
	mutex_unlock(&shares_mutex);

	प्रत्यागम ret;
}

पूर्णाङ्क sched_गणः_समुच्चयः_idle(संरचना task_गणः *tg, दीर्घ idle)
{
	पूर्णाङ्क i;

	यदि (tg == &मूलम्_task_गणः)
		प्रत्यागम -EINVAL;

	यदि (idle < 0 || idle > 1)
		प्रत्यागम -EINVAL;

	mutex_तालकम्(&shares_mutex);

	यदि (tg->idle == idle) {
		mutex_unlock(&shares_mutex);
		प्रत्यागम 0;
	}

	tg->idle = idle;

	for_each_possible_cpu(i) {
		संरचना rq *rq = cpu_rq(i);
		संरचना sched_entity *se = tg_se(tg, i);
		संरचना cfs_rq *grp_cfs_rq = tg_cfs_rq(tg, i);
		bool was_idle = cfs_rq_is_idle(grp_cfs_rq);
		दीर्घ idle_task_delta;
		संरचना rq_flags rf;

		rq_तालकम्_irqsave(rq, &rf);

		grp_cfs_rq->idle = idle;
		यदि (WARN_ON_ONCE(was_idle == cfs_rq_is_idle(grp_cfs_rq)))
			गच्छ अग्र_cpu;

		idle_task_delta = grp_cfs_rq->h_nr_queued -
				  grp_cfs_rq->h_nr_idle;
		यदि (!cfs_rq_is_idle(grp_cfs_rq))
			idle_task_delta *= -1;

		for_each_sched_entity(se) {
			संरचना cfs_rq *cfs_rq = cfs_rq_सम्बन्धि(se);

			यदि (!se->on_rq)
				भङ्ग;

			cfs_rq->h_nr_idle += idle_task_delta;

			/* Already accounted at parent level and above. */
			यदि (cfs_rq_is_idle(cfs_rq))
				भङ्ग;
		}

अग्र_cpu:
		rq_unlock_irqrestore(rq, &rf);
	}

	/* Idle groups have minimum weight. */
	यदि (tg_is_idle(tg))
		__sched_group_set_shares(tg, scale_आरोपय(WEIGHT_IDLEPRIO));
	अन्यथा
		__sched_group_set_shares(tg, NICE_0_आरोपय);

	mutex_unlock(&shares_mutex);
	प्रत्यागम 0;
}

#यदि_अन्तः /* CONFIG_FAIR_GROUP_SCHED */


स्थिर अचिह्नित पूर्णाङ्क get_rr_interval_fair(संरचना rq *rq, संरचना task_struct *task)
{
	संरचना sched_entity *se = &task->se;
	अचिह्नित पूर्णाङ्क rr_interval = 0;

	/*
	 * Time slice is 0 for SCHED_OTHER tasks that are on an otherwise
	 * idle runqueue:
	 */
	यदि (rq->cfs.आरोपय.weight)
		rr_interval = NS_गन्तव्यम्_JIFFIES(se->खण्डिका);

	प्रत्यागम rr_interval;
}

/*
 * All the scheduling class methods:
 */
DEFINE_SCHED_CLASS(fair) = {
	.पङ्क्तौ_निक्षिप_task		= पङ्क्तौ_निक्षिप_task_fair,
	.पङ्क्तेः_निष्कासय_task		= पङ्क्तेः_निष्कासय_task_fair,
	.yield_task		= yield_task_fair,
	.yield_गन्तव्यम्_task		= yield_गन्तव्यम्_task_fair,

	.wakeup_preempt		= wakeup_preempt_fair,

	.pick_task		= pick_task_fair,
	.put_पूर्व_task		= put_पूर्व_task_fair,
	.समुच्चयः_अग्र_task          = समुच्चयः_अग्र_task_fair,

	.select_task_rq		= select_task_rq_fair,
	.migrate_task_rq	= migrate_task_rq_fair,

	.rq_online		= rq_online_fair,
	.rq_offline		= rq_offline_fair,

	.task_dead		= task_dead_fair,
	.समुच्चयः_cpus_allowed	= समुच्चयः_cpus_allowed_fair,

	.task_tick		= task_tick_fair,
	.task_fork		= task_fork_fair,

	.reweight_task		= reweight_task_fair,
	.prio_परिवर्तितम्		= prio_परिवर्तितम्_fair,
	.switching_from		= switching_from_fair,
	.switched_from		= switched_from_fair,
	.switched_गन्तव्यम्		= switched_गन्तव्यम्_fair,

	.get_rr_interval	= get_rr_interval_fair,

	.update_curr		= update_curr_fair,

#यदि_परिभाषितम् CONFIG_FAIR_गणः_SCHED
	.task_change_गणः	= task_change_गणः_fair,
#यदि_अन्तः

#यदि_परिभाषितम् CONFIG_SCHED_CORE
	.task_is_throttled	= task_is_throttled_fair,
#यदि_अन्तः

#यदि_परिभाषितम् CONFIG_UCLAMP_TASK
	.uclamp_enabled		= 1,
#यदि_अन्तः
};

शून्य print_cfs_stats(संरचना seq_file *m, पूर्णाङ्क cpu)
{
	संरचना cfs_rq *cfs_rq, *pos;

	rcu_read_तालकम्();
	for_each_पर्णम्_cfs_rq_safe(cpu_rq(cpu), cfs_rq, pos)
		print_cfs_rq(m, cpu, cfs_rq);
	rcu_read_unlock();
}

#यदि_परिभाषितम् CONFIG_NUMA_BALANCING
शून्य show_numa_stats(संरचना task_struct *p, संरचना seq_file *m)
{
	पूर्णाङ्क ग्रन्थिः;
	अचिह्नित दीर्घ tsf = 0, tpf = 0, gsf = 0, gpf = 0;
	संरचना numa_गणः *ng;

	rcu_read_तालकम्();
	ng = rcu_dereference_all(p->numa_गणः);
	for_each_online_ग्रन्थिः(ग्रन्थिः) {
		यदि (p->numa_दोषाः) {
			tsf = p->numa_दोषाः[task_दोषाः_idx(NUMA_MEM, ग्रन्थिः, 0)];
			tpf = p->numa_दोषाः[task_दोषाः_idx(NUMA_MEM, ग्रन्थिः, 1)];
		}
		यदि (ng) {
			gsf = ng->दोषाः[task_दोषाः_idx(NUMA_MEM, ग्रन्थिः, 0)];
			gpf = ng->दोषाः[task_दोषाः_idx(NUMA_MEM, ग्रन्थिः, 1)];
		}
		print_numa_stats(m, ग्रन्थिः, tsf, tpf, gsf, gpf);
	}
	rcu_read_unlock();
}
#यदि_अन्तः /* CONFIG_NUMA_BALANCING */

__init शून्य init_sched_fair_class(शून्य)
{
	पूर्णाङ्क i;

	for_each_possible_cpu(i) {
		zalloc_cpumask_var_ग्रन्थिः(&प्रत्येकम्_cpu(आरोपय_balance_आवरणम्, i), GFP_KERNEL, cpu_गन्तव्यम्_ग्रन्थिः(i));
		zalloc_cpumask_var_ग्रन्थिः(&प्रत्येकम्_cpu(select_rq_आवरणम्,    i), GFP_KERNEL, cpu_गन्तव्यम्_ग्रन्थिः(i));
		zalloc_cpumask_var_ग्रन्थिः(&प्रत्येकम्_cpu(should_we_balance_tmpmask, i),
					GFP_KERNEL, cpu_गन्तव्यम्_ग्रन्थिः(i));

#यदि_परिभाषितम् CONFIG_CFS_BANDWIDTH
		INIT_CSD(&cpu_rq(i)->cfsb_csd, __cfsb_csd_unthrottle, cpu_rq(i));
		INIT_LIST_शीर्ष(&cpu_rq(i)->cfsb_csd_list);
#यदि_अन्तः
	}

	open_softirq(SCHED_SOFTIRQ, sched_balance_softirq);

#यदि_परिभाषितम् CONFIG_NO_HZ_COMMON
	nohz.अग्र_balance = jiffies;
	nohz.अग्र_blocked = jiffies;
	zalloc_cpumask_var(&nohz.idle_cpus_आवरणम्, GFP_NOWAIT);
#यदि_अन्तः
}
